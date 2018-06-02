
#define streq(a, b) (strcmp(a, b) == 0)

static BOOL is_numeric1(char c)
{
  return strchr("0123456789+-.eE", c) != 0;
}
int init_char_mask(char* mask, const char* char_set)
{
  memset(mask, 0, 256 / 8);

  while (*char_set) {
    unsigned c = (unsigned)(*char_set++) & 0xFF;
    mask[c >> 3] |= 1 << (c & 7);
  }

  return 1;
}
bool contains(const char* mask, unsigned c)
{
  return (mask[(c >> 3) & (256 / 8 - 1)] & (1 << (c & 7))) != 0;
}
bool is_command(unsigned c)
{
  static const char s_commands[] = "+-MmZzLlHhVvCcSsQqTtAaFfPp";
  static char m_commands_mask[256 / 8];
  static int inited = 0;
  inited = inited ? 1 : init_char_mask(m_commands_mask, s_commands);
  return contains(m_commands_mask, c);
}
bool is_numeric(unsigned c)
{
  static const char s_numeric[] = ".Ee0123456789";
  static char m_numeric_mask[256 / 8];
  static int inited = 0;
  inited = inited ? 1 : init_char_mask(m_numeric_mask, s_numeric);
  return contains(m_numeric_mask, c);
}
bool is_separator(unsigned c)
{
  static const char s_separators[] = " ,\t\n\r";
  char m_separators_mask[256 / 8];
  static int inited = 0;
  inited = inited ? 1 : init_char_mask(m_separators_mask, s_separators);
  return contains(m_separators_mask, c);
}
typedef struct _named_color {
  char name[22];
  int8u r, g, b, a;
} named_color;
named_color colors[] =  {
#define NAMECOLORSDEF(N, R, G, B, A)  { N, R, G, B, A },
#include "svg_namecolors.inl"
#undef NAMECOLORSDEF
};

int cmp_color(const void* p1, const void* p2)
{
  return strcmp(((named_color*)p1)->name, ((named_color*)p2)->name);
}
DWORD parse_color(const char* str)
{
  int i;
  unsigned c = 0;

  while (*str == ' ') {
    ++str;
  }

  if (*str == '#') {
    sscanf(str + 1, "%x", &c);
    return _RGB((c >> 16) & 0xFF, (c >> 8) & 0xFF, c & 0xFF);
  }
  // parse rgb and _rgba syntax
  else if (strncmp(str, "rgb(", 4) == 0 || strncmp(str, "_rgba(", 5) == 0) {
    BOOL has_alpha = (strncmp(str, "_rgba(", 5) == 0);
    double num[4] = {0, 0, 0, 0};

    if (!has_alpha) {
      num[3] = 1.0;
    }

    str += 4 + (int)(has_alpha);

    // extract the numbers and support % notation
    for (i = 0; i < 3 + (int)(has_alpha); i++) {
      num[i] = atof(str);

      while (isdigit(*str) ||  *str == '.') {
        str++;
      }

      if (*str == '%') {
        num[i] *= 0.01;
      }
      // alpha appears to be treated differently in _rgba() in svg
      else if (i < 3) {
        num[i] /= 255.0;
      }

      while (*str && !isdigit(*str)) {
        str++;
      }
    }

    return _rgba(num[0], num[1], num[2], num[3]);
  }
  else {
    named_color c;
    unsigned len = strlen(str);
    const named_color* pc;

    if (len > sizeof(c.name) - 1) {
      assert(0 && "parse_color: Invalid color name");
    }

    strcpy(c.name, str);
    pc = (const named_color*)bsearch(&c, colors, sizeof(colors) / sizeof
        (colors[0]), sizeof(colors[0]), cmp_color);

    if (pc == 0) {
      assert(0 && "parse_color: Invalid color name");
    }

    return _RGBA(pc->r, pc->g, pc->b, pc->a);
  }
}
double parse_double(const char* str)
{
  while (*str == ' ') {
    ++str;
  }

  return atof(str);
}
static unsigned parse_transform_args(const char* str, double* args, unsigned max_na, unsigned* na)
{
  const char* end;
  const char* ptr = str;
  *na = 0;

  while (*ptr &&  *ptr != '(') {
    ++ptr;
  }

  if (*ptr == 0) {
    assert(0 && "parse_transform_args: Invalid syntax");
  }

  end = ptr;

  while (*end &&  *end != ')') {
    ++end;
  }

  if (*end == 0) {
    assert(0 && "parse_transform_args: Invalid syntax");
  }

  while (ptr < end) {
    if (is_numeric(*ptr)) {
      if (*na >= max_na) {
        assert(0 && "parse_transform_args: Too many arguments");
      }

      args[(*na)++] = atof(ptr);

      while (ptr < end && is_numeric(*ptr)) {
        ++ptr;
      }
    }
    else {
      ++ptr;
    }
  }

  return (unsigned)(end - str);
}
#define premultiply()  affine_multiply(ta0, m)
void parse_transform(double* ta0, const char* str)
{
  double m[9];
  affine_init(m);

  while (*str) {
    if (islower(*str)) {
      if (strncmp(str, "matrix", 6) == 0) {
        double args[6];
        unsigned na = 0;
        unsigned len = parse_transform_args(str, args, 6, &na);

        if (na != 6) {
          assert(0 && "parse_matrix: Invalid number of arguments");
        }

        affine_set(m, args[0], args[1], args[2], args[3], args[4], args[5]);
        premultiply();
        str += len;
      }
      else if (strncmp(str, "translate", 9) == 0) {
        double args[2];
        unsigned na = 0;
        unsigned len = parse_transform_args(str, args, 2, &na);

        if (na == 1) {
          args[1] = 0.0;
        }

        affine_translation(m, args[0], args[1]);
        premultiply();
        str += len;
      }
      else if (strncmp(str, "rotate", 6) == 0) {
        double args[3];
        unsigned na = 0;
        unsigned len = parse_transform_args(str, args, 3, &na);

        if (na == 1) {
          affine_rotation(m, deg2rad(args[0]));
          premultiply();
        }
        else if (na == 3) {
          affine_translation(m, -args[1], -args[2]);
          affine_rotation(m, deg2rad(args[0]));
          affine_translation(m, args[1], args[2]);
          premultiply();
        }
        else {
          assert(0 && "parse_rotate: Invalid number of arguments");
        }

        str += len;
      }
      else if (strncmp(str, "scale", 5) == 0) {
        double args[2];
        unsigned na = 0;
        unsigned len = parse_transform_args(str, args, 2, &na);

        if (na == 1) {
          args[1] = args[0];
          affine_scaling(m, args[0], args[1]);
        }

        premultiply();
        str += len;
      }
      else if (strncmp(str, "skewX", 5) == 0) {
        double arg;
        unsigned na = 0;
        unsigned len = parse_transform_args(str, &arg, 1, &na);
        affine_skewing(m, deg2rad(arg), 0.0);
        premultiply();
        str += len;
      }
      else if (strncmp(str, "skewY", 5) == 0) {
        double arg;
        unsigned na = 0;
        unsigned len = parse_transform_args(str, &arg, 1, &na);
        affine_skewing(m, 0.0, deg2rad(arg));
        premultiply();
        str += len;
      }
      else {
        ++str;
      }
    }
    else {
      ++str;
    }
  }
}

int parse_style(gc_t* sf, char* str, char** attr, int maxlen)
{
  int len = 0;
  
  while (*str) {
    char* nv_start;
    char* nv_end;
    
    // Left Trim
    while (*str && isspace(*str)) {
      ++str;
    }
    
    nv_start = str;
    
    while (*str &&  *str != ';') {
      ++str;
    }
    
    nv_end = str;
    
    // Right Trim
    while (nv_end > nv_start && (*nv_end == ';' || isspace(*nv_end))) {
      --nv_end;
    } ++nv_end;
    
    {
      char* val;
      char* str1 = nv_start;
      char* m_attr_name;
      char* m_attr_value;
      
      while (str1 < nv_end &&  *str1 != ':') {
        ++str1;
      }
      
      val = str1;
      
      // Right Trim
      while (str1 > nv_start && (*str1 == ':' || isspace(*str1))) {
        --str1;
      } ++str1;
      
      m_attr_name = nv_start;
      *nv_start = 0;
      
      while (val < nv_end && (*val == ':' || isspace(*val))) {
        ++val;
      }
      
      m_attr_value = val;
      *nv_end = 0;
      attr[len++] = m_attr_name;
      attr[len++] = m_attr_value;
    }
    
    if (*str) {
      ++str;
    }
  }
  
  return len;
}

BOOL parse_attr2(gc_t* sf, char** attr, int len)
{
  COLOR fill_color = 0;
  COLOR stroke_color = 0;
  bool fill_flag = 0;
  bool stroke_flag = 0;
  int line_style = 0;
  int k;

  for (k = 0; attr[k]; k += 2) {
    char* name = attr[k];
    char* value = attr[k + 1];

    if (strcmp(name, "style") == 0) {
      char* attr2[100] = {0};
      int len2 = parse_style(sf, value, attr2, countof(attr2));
      parse_attr2(sf, attr2, len);
    }
    else if (strcmp(name, "fill") == 0) {
      if (strcmp(value, "none") == 0) {
        fill_flag = true;
      }
      else {
        fill_color = parse_color(value);
        fill_flag = true;
      }
    }
    else if (strcmp(name, "fill-opacity") == 0) {
      double a_ = parse_double(value);

      if (a_ < 0.0) {
        a_ = 0.0;
      }

      if (a_ > 1.0) {
        a_ = 1.0;
      }

      fill_color |= _rgba(0, 0, 0, a_);
    }
    else if (strcmp(name, "stroke") == 0) {
      if (strcmp(value, "none") == 0) {
        stroke_flag = false;
      }
      else {
        stroke_color = parse_color(value);
        stroke_flag = true;
      }
    }
    else if (strcmp(name, "stroke-width") == 0) {
      double stroke_width = parse_double(value);
      gcLineWidth(sf, stroke_width);
    }
    else if (strcmp(name, "stroke-linecap") == 0) {
      if (strcmp(value, "butt") == 0) {
        line_style |= CAP_BUTT;
      }
      else if (strcmp(value, "round") == 0) {
        line_style |= CAP_ROUND;
      }
      else if (strcmp(value, "square") == 0) {
        line_style |= CAP_SQUARE;
      }
    }
    else if (strcmp(name, "stroke-linejoin") == 0) {
      if (strcmp(value, "miter") == 0) {
        line_style |= JOIN_MITER;
      }
      else if (strcmp(value, "round") == 0) {
        line_style |= JOIN_ROUND;
      }
      else if (strcmp(value, "bevel") == 0) {
        line_style |= JOIN_BEVEL;
      }
    }
    else if (strcmp(name, "stroke-miterlimit") == 0) {
      double miter_limit = parse_double(value);
      gcMiterLimit(sf, miter_limit);
    }
    else if (strcmp(name, "stroke-opacity") == 0) {
      double a_ = parse_double(value);

      if (a_ < 0.0) {
        a_ = 0.0;
      }

      if (a_ > 1.0) {
        a_ = 1.0;
      }

      stroke_color |= _rgba(0, 0, 0, a_);
    }
    else if (strcmp(name, "transform") == 0) {
      double m[9] = {0};
      affine_init(m);
      parse_transform(m, value);
      gcTransform(sf, m[0], m[1], m[2], m[3], m[4], m[5]);
    }
    //else
    //if(strcmp(el, "<OTHER_ATTRIBUTES>") == 0)
    //{
    //}
    // . . .
    else {
      //return FALSE;
    }
  }

  if (line_style) {
    gcLineStyle(sf, line_style);
  }

  if (fill_color) {
    gcFillColor(sf, fill_color);
  }

  if (stroke_color) {
    gcStrokeColor(sf, stroke_color);
  }

  if (fill_flag) {
    gcFill(sf);
  }

  if (stroke_flag) {
    gcStroke(sf);
  }

  return TRUE;
}

// 指令   参数  名称  描述
// M  x,y   moveto  移动虚拟画笔到指定的（x,y）坐标，仅移动不绘制
// m  x,y   moveto  移动虚拟画笔到指定的（x,y）坐标，这个坐标是相对于当前画笔的坐标，仅移动不绘制
// L  x,y   lineto  从当前画笔所在位置绘制一条直线到指定的（x,y）坐标
// l  x,y   lineto  从当前画笔所在位置绘制一条直线到指定的（x,y）坐标，(x,y)坐标是相对于花瓣位置的点
// H  x   horizontal lineto   绘制一条水平直线到参数指定的x坐标点，y坐标为画笔的y坐标
// h  x   horizontal lineto   绘制一条水平直线到参数指定的x坐标点（当前x + 指定的x），x坐标相对于当前画笔x坐标
// V  y   vertical lineto   从当前位置绘制一条垂直直线到参数指定的y坐标
// v  y   horizontal lineto   从当前位置绘制一条垂直直线到参数指定的y坐标，y坐标相对于当前画笔的y坐标
// C  x1,y1 x2,y2 x,y   curveto   从当前画笔位置绘制一条三次贝兹曲线到参数（x,y）指定的坐标。x1，y1和x2,y2是曲线的开始和结束控制点，用于控制曲线的弧度
// c  x1,y1 x2,y2 x,y   curveto   于大小C指令相同，但是坐标是相对于画笔的坐标
// S  x2,y2 x,y   shorthand / smooth curveto  从当前画笔位置绘制一条三次贝兹曲线到参数（x,y）指定的坐标。x2,y2是结束控制点。开始控制点和前一条曲线的结束控制点相同
// s  x2,y2 x,y   shorthand / smooth curveto  和大小的S指令相同，但是坐标是相对于当前画笔的坐标点
// Q  x1,y1 x,y   二次贝兹曲线  从当前画笔位置绘制一条二次贝兹曲线到参数（x,y）指定的坐标。x1,y1是控制点，用于控制曲线的弧度
// q  x1,y1 x,y   二次贝兹曲线  和大小的Q指令相同，但是坐标是相对于当前画笔的坐标点
// T  x,y   平滑的二次贝兹曲线  从当前画笔位置绘制一条二次贝兹曲线到参数（x,y）指定的坐标。控制点被假定为最后一次使用的控制点
// t  x,y   平滑的二次贝兹曲线  和大小的T指令相同，但是坐标是相对于当前画笔的坐标点
// A  rx,ry x-axis-rotation large-arc-flag,sweepflag x,y  椭圆弧线  从当前画笔位置开始绘制一条椭圆弧线到（x,y）指定的坐标。rx和ry分别为椭圆弧线水平和垂直方向上的半径。x-axis-rotation指定弧线绕x轴旋转的度数。它只在rx和ry的值不相同是有效果。large-arc-flag是大弧标志位，取值0或1，用于决定绘制大弧还是小弧。sweep-flag用于决定弧线绘制的方向
// a  rx,ry x-axis-rotation large-arc-flag,sweepflag x,y  椭圆弧线  和大写的A指令相同，但是坐标是相对于当前画笔的坐标点
// Z  无  闭合路径  从结束点绘制一条直线到开始点，闭合路径
// z  无  闭合路径  从结束点绘制一条直线

#define last_number(_X) (_X=strtod(s, &s), s+=!!(*s))
//#define set_last_xy(_x, _y)  (last_x = _x, last_y = _y)
#define rel_to_abs(a, b)   { a += last_x, b += last_y;}
void parse_path(gc_t* sf, char** attr, int len)
{
  int i;
  double last_x2 = 0, last_y2 = 0;
  double last_x = 0, last_y = 0;
  int last_is_curve = 0;

  for (i = 0; i < len; i += 2) {
    // The <path> tag can consist of the path itself ("d=")
    // as well as of other parameters like "style=", "transform=", etc.
    // In the last case we simply rely on the function of parsing
    // attributes (see 'else' branch).
    if (strcmp(attr[i], "d") == 0) {
      char* s = (char*)attr[i + 1];

      while (*s) {
        double a, b, c, d, e, f;
        char cmd = (*s++);

        switch (cmd) {
        case 'M':
        case 'm':
          last_number(a);
          last_number(b);

          if (cmd == 'm') {
            a += last_x;
            b += last_y;
          }

          gcMoveTo(sf, a, b);
          last_x2 = last_x, last_y2 = last_y;
          last_x = a;
          last_y = b;
          break;

        case 'L':
        case 'l':
          last_number(a);
          last_number(b);

          if (cmd == 'l') {
            a += last_x;
            b += last_y;
          }

          gcLineTo(sf, a, b);
          last_x2 = last_x, last_y2 = last_y;
          last_x = a;
          last_y = b;
          break;

        case 'H':
        case 'h'://horizontal lineto
          last_number(a);

          if (cmd == 'h') {
            a += last_x;
          }

          gcLineTo(sf, a, last_y);
          last_x2 = last_x;
          last_x = a;
          break;

        case 'V':
        case 'v':// vertical lineto
          last_number(b);

          if (cmd == 'v') {
            b += last_y;
          }

          gcLineTo(sf, last_x, b);
          last_y2 = last_y;
          last_y = b;
          break;

        case 'Q':
        case 'q'://quadratic Belzier curve
          last_number(a);
          last_number(b);
          last_number(c);
          last_number(d);

          if (cmd == 'q') {
            a += last_x;
            b += last_y;
            c += last_x;
            d += last_y;
          }

          gcQuadraticCurveTo(sf, a, b, c, d);
          last_is_curve = true;
          last_x2 = a, last_y2 = b;
          last_x = c;
          last_y = d;
          break;

        case 'T':
        case 't':
          last_number(c);
          last_number(d);

          if (cmd == 't') {
            c += last_x;
            d += last_y;
          }

          a = last_x;
          b = last_y;

          if (last_is_curve) {
            a = a + a - last_x2, b = b + b - last_y2;
          }

          gcQuadraticCurveTo(sf, a, b, c, d);
          last_is_curve = true;
          last_x2 = a, last_y2 = b;
          last_x = c;
          last_y = d;
          break;

        case 'C':
        case 'c':
          last_number(a);
          last_number(b);
          last_number(c);
          last_number(d);
          last_number(e);
          last_number(f);

          if (cmd == 'c') {
            a += last_x;
            b += last_y;
            c += last_x;
            d += last_y;
            e += last_x;
            f += last_y;
          }

          gcBezierCurveTo(sf, a, b, c, d, e, f);
          last_is_curve = true;
          last_x2 = c, last_y2 = d;
          last_x = e;
          last_y = f;

          break;

        case 'S':
        case 's':
          last_number(c);
          last_number(d);
          last_number(e);
          last_number(f);

          if (cmd == 's') {
            a += last_x;
            b += last_y;
            c += last_x;
            d += last_y;
            e += last_x;
            f += last_y;
          }

          a = last_x;
          b = last_y;

          if (last_is_curve) {
            a = a + a - last_x2, b = b + b - last_y2;
          }

          gcBezierCurveTo(sf, a, b, c, d, e, f);
          last_is_curve = true;
          last_x2 = c, last_y2 = d;
          last_x = e;
          last_y = f;
          break;

        case 'A':
        case 'a':
          ASSERT(0 && "parse_path: Command A: NOT IMPLEMENTED YET");

        case 'Z':
        case 'z':
          gcClosePath(sf);
          break;

        default: {
          ASSERT(0 && "parse_path: Invalid Command");
        }
        }
      }
    }
    else {
      // Create a temporary single pair "name-value" in order
      // to avoid multiple calls for the same attribute.
    }
  }

  parse_attr2(sf, attr, len);
}
void parse_line(gc_t* sf, char** attr, int len)
{
  int i;
  double x1 = 0.0;
  double y1 = 0.0;
  double x2 = 0.0;
  double y2 = 0.0;

  for (i = 0; attr[i]; i += 2) {
    if (strcmp(attr[i], "x1") == 0) {
      x1 = parse_double(attr[i + 1]);
    }
    else    if (strcmp(attr[i], "y1") == 0) {
      y1 = parse_double(attr[i + 1]);
    }
    else    if (strcmp(attr[i], "x2") == 0) {
      x2 = parse_double(attr[i + 1]);
    }
    else    if (strcmp(attr[i], "y2") == 0) {
      y2 = parse_double(attr[i + 1]);
    }
  }

  gcMoveTo(sf, x1, y1);
  gcLineTo(sf, x2, y2);
  parse_attr2(sf, attr, len);
}
void parse_rect(gc_t* sf, char** attr, int len)
{
  int i;
  double x = 0.0;
  double y = 0.0;
  double w = 0.0;
  double h = 0.0;

  for (i = 0; i < len; i += 2) {
    if (strcmp(attr[i], "x") == 0) {
      x = parse_double(attr[i + 1]);
    }
    else if (strcmp(attr[i], "y") == 0) {
      y = parse_double(attr[i + 1]);
    }
    else if (strcmp(attr[i], "width") == 0) {
      w = parse_double(attr[i + 1]);
    }
    else if (strcmp(attr[i], "height") == 0) {
      h = parse_double(attr[i + 1]);
    }
  }

  gcRect(sf, x, y, w, h);
  parse_attr2(sf, attr, len);
}
void parse_poly(gc_t* sf, char** attr, BOOL close_flag)
{
  int i;
  double x = 0.0;
  double y = 0.0;
  double* pts = NULL;
  int len = 0;
  int size = 0;
  
  for (i = 0; attr[i]; i += 2) {
    if (strcmp(attr[i], "points") == 0) {
      char* s = (char*)attr[i + 1];
      if (size<=(len+8)) {
        size = (len+8)*3/2;
        MYREALLOC(pts, size);
      }
      
      if (! *s) {
        assert(0 && "parse_poly: Too few coordinates");
      }
      
      last_number(x);
      pts[len++] = x;
      
      if (! *s) {
        assert(0 && "parse_poly: Too few coordinates");
      }
      
      last_number(y);
      pts[len++] = y;
      
      while (*s) {
        last_number(x);
        pts[len++] = x;
        
        if (! *s) {
          assert(0 && "parse_poly: Odd number of coordinates");
        }
        
        last_number(y);
        pts[len++] = y;
      }
    }
  }
  
  gcPolygon(sf, pts, len/2, close_flag, GCTYPE_DOUBLE, sizeof(double)*2);
  parse_attr2(sf, attr, len);
}
int svg_parser(gc_t* sf, const char* fname)
{
  //char msg[1024];
  STRTREE mm[1] =  {0};
  int i, j, k, m = 0, n = 0, ptLen = 0, attrLen = 0;
  //int m2, m1;
  str_t s[1] = {0};
  int width = 0, height = 0;
  mm->mode = XML_MARK_ALL;

  if (!str_loadasni(fname, s)) {
    //sprintf(msg, "Couldn't open file %s", fname);
    return 0;
  }

  mm->s = s->s;
  mm->l = s->l;

  ParseXml(mm, 0);
  mm->node = (STRNODE*)malloc(mm->maxlen * sizeof(STRNODE));
  mm->pos = 0;
  //mm->doecho = 1;
  ParseXml(mm, 0);

  for (i = 0; i < mm->len; ++i) {
    str_t* s1 = &mm->node[i].name;
    s1->s[s1->l - 1] = 0;
  }

  for (i = 0; i < mm->len; ++i) {
    if (mm->node[i].type == XML_ELEMENTNAME) {
      int ind = mm->node[i].ind + 1;

      for (k = 0, j = i + 1; j < mm->len && ind <= mm->node[j].ind; ++j) {
        if (ind == mm->node[j].ind && mm->node[j].type == XML_ATTRIBUTENAME) {
          if (streq(mm->node[j].name.s, "d") || streq(mm->node[j].name.s, "points")) {
            char* s = mm->node[j + 1].name.s;

            for (; *s; ++s) {
              if (*s == ' ' || is_command(*s)) {
                ptLen++;
              }
            }
          }
        }
      }

      attrLen++;
    }
  }

  for (i = 0; i < mm->len; ++i) {
    char* attr[100];
    int attrlen = 0;

    if (mm->node[i].type == XML_ELEMENTNAME) {
      int ind = mm->node[i].ind + 1;

      for (k = 0, j = i + 1; j < mm->len && ind <= mm->node[j].ind; ++j) {
        if (ind == mm->node[j].ind && mm->node[j].type == XML_ATTRIBUTENAME) {
          attr[k++] = mm->node[j].name.s;
          attr[k++] = mm->node[j + 1].name.s;
        }
      }

      attrlen = k;
      attr[k] = 0;

      {
        char* el = mm->node[i].name.s;

        if (streq(el, "svg")) {
          for (k = 0; attr[k]; k += 2) {
            if (streq(attr[k], "width")) {
              width = atoi(attr[k + 1]);
            }
            else if (streq(attr[k], "height")) {
              height = atoi(attr[k + 1]);
            }
          }
        }
        else if (strcmp(el, "g") == 0) {
          parse_attr2(sf, attr, attrlen);
        }
        else if (strcmp(el, "title") == 0) {
          int title = 0;
        }
        else if (strcmp(el, "path") == 0) {
          gcBeginPath(sf);
          parse_path(sf, attr, attrlen);
          //gcEndPath(sf);
        }
        else if (strcmp(el, "rect") == 0) {
          parse_rect(sf, attr, attrlen);
        }
        else if (strcmp(el, "line") == 0) {
          parse_line(sf, attr, attrlen);
        }
        else if (strcmp(el, "polyline") == 0) {
          parse_poly(sf, attr, FALSE);
        }
        else if (strcmp(el, "polygon") == 0) {
          parse_poly(sf, attr, TRUE);
        }
      }
    }
  }

  FreeXml(mm);
  return 0;
}

