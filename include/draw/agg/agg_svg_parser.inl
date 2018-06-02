
#include "agg_svg_parser.h"
#include "parser/json.inl"
//#include "parser/test_json2.inl"
#include "parser/xml.inl"



static bool is_numeric(char c)
{
  return strchr("0123456789+-.eE", c) != 0;
}


static unsigned parse_transform_args(const char* str, int len, float* args, unsigned max_na, unsigned* na)
{
  const char* ptr = str;
  const char* end1 = str + len;
  const char* end;
  *na = 0;
  while(ptr<end1 && *ptr != '(') ++ptr;
  if(ptr == end1)
  {
    return 0;//("parse_transform_args: Invalid syntax");
  }
  end = ptr;
  while(end<end1 && *end != ')') ++end;
  if(end == end1)
  {
    return 0;//("parse_transform_args: Invalid syntax");
  }
  
  while(ptr < end) {
    if(is_numeric(*ptr)) {
      if(*na >= max_na) {
        return 0;//("parse_transform_args: Too many arguments");
      }
      args[(*na)++] = atof(ptr);
      while(ptr < end && is_numeric(*ptr)) ++ptr;
    } else {
      ++ptr;
    }
  }
  return (unsigned)(end - str);
}


unsigned parse_matrix(float* m, const char* str, int slen)
{
  float args[6] = {0};
  unsigned na = 0;
  unsigned len = parse_transform_args(str, slen, args, 6, &na);
  if(na != 6)
  {
    return 0;//("parse_matrix: Invalid number of arguments");
  }
  M23Multiply(m, args, MatrixOrderPrepend);
  return len;
}


unsigned parse_translate(float* m, const char* str, int slen)
{
  float args[2];
  unsigned na = 0;
  unsigned len = parse_transform_args(str, slen, args, 2, &na);
  if(na == 1) args[1] = 0.0;
  M23Translate(m, args[0], args[1], MatrixOrderPrepend);
  return len;
}

unsigned parse_rotate(float* m, const char* str, int slen)
{
  float args[3];
  unsigned na = 0;
  unsigned len = parse_transform_args(str, slen, args, 3, &na);
  if(na == 1) {
    M23Rotate(m, args[0], MatrixOrderPrepend);
  } else if(na == 3) {
    M23Translate(m, -args[1], -args[2], MatrixOrderPrepend);
    M23Rotate(m, args[0], MatrixOrderPrepend);
    M23Translate(m, args[1], args[2], MatrixOrderPrepend);
  } else {
    return 0;//("parse_rotate: Invalid number of arguments");
  }
  return len;
}


unsigned parse_scale(float* m, const char* str, int slen)
{
  float args[2];
  unsigned na = 0;
  unsigned len = parse_transform_args(str, slen, args, 2, &na);
  if(na == 1) args[1] = args[0];
  M23Scale(m, args[0], args[1], MatrixOrderPrepend);
  return len;
}


unsigned parse_skew_x(float* m, const char* str, int slen)
{
  float arg;
  unsigned na = 0;
  unsigned len = parse_transform_args(str, slen, &arg, 1, &na);
  // M23Shear(m, deg2rad(arg), 0.0);
  ASSERT(0);
  return len;
}


unsigned parse_skew_y(float* m, const char* str, int slen)
{
  float arg;
  unsigned na = 0;
  unsigned len = parse_transform_args(str, slen, &arg, 1, &na);
  //m_path.transform().premultiply(trans_affine_skewing(0.0, deg2rad(arg)));
  ASSERT(0);
  return len;
}

void parse_transform(float* m, const char* str, int slen)
{
  const char* end = str + slen;
  while(str<end) {
    if(islower(*str)) {
      if(strncmp(str, "matrix", 6) == 0) {
        str += parse_matrix(m, str, slen);
      } else if(strncmp(str, "translate", 9) == 0) {
        str += parse_translate(m, str, slen);
      } else if(strncmp(str, "rotate", 6) == 0) {
        str += parse_rotate(m, str, slen);
      } else if(strncmp(str, "scale", 5) == 0) {
        str += parse_scale(m, str, slen);
      } else if(strncmp(str, "skewX", 5) == 0) {
        str += parse_skew_x(m, str, slen);
      } else if(strncmp(str, "skewY", 5) == 0) {
        str += parse_skew_y(m, str, slen);
      } else {
        ++str;
      }
    } else {
      ++str;
    }
  }
}



int cmp_color(const void* p1, const void* p2)
{
  return strcmp(((named_color*)p1)->name, ((named_color*)p2)->name);
}


COLOR parse_color(const char* str)
{
  unsigned c = 0;
  while(*str == ' ') ++str;
  if(*str == '#') {
    sscanf(str + 1, "%x", &c);
    return rgb8_packed(c);
  }
  else
  {
    named_color c;
    unsigned len = strlen(str);
    const void* p;
    const named_color* pc;
    if(len > sizeof(c.name) - 1) {
      return 0;//("parse_color: Invalid color name '%s'", str);
    }
    strcpy(c.name, str);
    p = bsearch(&c, colors, sizeof(colors) / sizeof(colors[0]), sizeof(colors[0]), cmp_color);
    if(p == 0) {
      return 0;
    }
    pc = (const named_color*)p;
    return _RGBA(pc->r, pc->g, pc->b, pc->a);
  }
}

double parse_double(const char* str) {
  while(*str == ' ') ++str;
  return atof(str);
}

bool parse_attr2(gc_t* g, int* opt, float* m, const char* name, int namelen, const char* value, int valuelen);

bool parse_name_value(gc_t* g, int* opt, float* m, const char* nv_start, const char* nv_end)
{
  const char* str;
  const char* m_attr_name = NULL;
  const char* m_attr_value = NULL;
  int m_attr_name_len=0;
  int m_attr_value_len=0;
  const char* val;
  if ('\"'==*nv_start) {
    ++nv_start;
  }
  str = nv_start;
  while(str < nv_end && *str != ':') ++str;
  val = str;
  // Right Trim
  while(str > nv_start && (*str == ':' || isspace(*str))) --str;
  ++str;
  m_attr_name = nv_start;
  m_attr_name_len = (unsigned)(str - nv_start);

  while(val < nv_end && (*val == ':' || isspace(*val))) ++val;
  m_attr_value = val;
  m_attr_value_len = nv_end - val;
  return parse_attr2(g, opt, m, m_attr_name, m_attr_name_len, m_attr_value, m_attr_value_len);
}

int parse_style(gc_t* g, int* opt, float* m, const char* str, int len)
{
  const char* end = str + len;
  while(str<end) {
    const char* nv_start;
    const char* nv_end;
    // Left Trim
    while(*str && isspace(*str)) ++str;
    nv_start = str;
    while(*str && *str != ';') ++str;
    nv_end = str;
    
    // Right Trim
    while(nv_end > nv_start && (*nv_end == ';' || isspace(*nv_end))) --nv_end;
    ++nv_end;
    parse_name_value(g, opt, m, nv_start, nv_end);
    ++str;
  }
  return 0;
}

bool parse_attr2(gc_t* g, int* opt, float* m, const char* name, int namelen, const char* value, int valuelen)
{
  COLOR clr;
  if(cstr_cmp(name, namelen, "fill", 4) == 0) {
    if(cstr_cmp(value, valuelen, "none", 4) == 0) {
      //m_path.fill_none();
      *opt &= ~GcOptFill;
    } else {
      *opt |= GcOptFill;
      clr = parse_color(value);
      gcSolidBrush(g, clr);
    }
  } else if(cstr_cmp(name, namelen, "fill-rule", -1) == 0) {
    // nonzero/evenodd
    if (cstr_cmp(value, valuelen, "evenodd", 7) == 0) {
      *opt |= GcOptEO;
    }
  } else if(cstr_cmp(name, namelen, "fill-opacity", -1) == 0) {
    //m_path.fill_opacity(parse_double(value));
    ASSERT(0);
  } else if(cstr_cmp(name, namelen, "style", 5) == 0) {
    parse_style(g, opt, m, value, valuelen);
  } else if(cstr_cmp(name, namelen, "stroke", 6) == 0) {
    if(cstr_cmp(value, valuelen, "none", 4) == 0) {
      *opt &= ~GcOptStroke;
    } else {
      *opt |= GcOptStroke;
      clr = parse_color(value);
      gcSolidPen(g, clr);
    }
  } else if(cstr_cmp(name, namelen, "stroke-width", -1) == 0) {
    gcLineWidth(g, parse_double(value));
  }
  else if(cstr_cmp(name, namelen, "stroke-linecap", -1) == 0)
  {
    if(cstr_cmp(value, valuelen, "butt", 4) == 0)        gcLineCap1(g, LineCapButt);
    else if(cstr_cmp(value, valuelen, "round", 5) == 0)  gcLineCap1(g, LineCapRound);
    else if(cstr_cmp(value, valuelen, "square", 6) == 0) gcLineCap1(g, LineCapSquare);
  }
  else if(cstr_cmp(name, namelen, "stroke-linejoin", -1) == 0)
  {
    if(cstr_cmp(value, valuelen, "miter", -1) == 0)      gcLineJoin(g, LineJoinMiter);
    else if(cstr_cmp(value, valuelen, "round", -1) == 0) gcLineJoin(g, LineJoinRound);
    else if(cstr_cmp(value, valuelen, "bevel", -1) == 0) gcLineJoin(g, LineJoinBevel);
  }
  else if(cstr_cmp(name, namelen, "stroke-miterlimit", -1) == 0) {
    gcMiterLimit(g, parse_double(value));
  } else if(cstr_cmp(name, namelen, "stroke-opacity", -1) == 0) {
    //m_path.stroke_opacity(parse_double(value));
    ASSERT(0);
  } else if(cstr_cmp(name, namelen, "transform", -1) == 0) {
    parse_transform(m, value, valuelen);
  } else {
    return false;
  }
  return true;
}

int parse_attr(gc_t* g, int* opt, float* m, const dstr_t* v, int i, int n)
{
  int d = v[i].d;
  for(; i<(n-1) && v[i].d>=d; ++i) {
    if (v[i].d == d && v[i+1].d==(d+1)) {
      parse_attr2(g, opt, m, v[i].s->s, v[i].s->l, v[i+1].s->s, v[i+1].s->l);
    }
  }
  return i;
}

int parse_path_impl(FPOINT* Points, BYTE* Types, int ipos, const char* buf, int len) {
  int i = 0;
  char cmd;
  float arg[6];
  float pre_x = 0, pre_y = 0;
  for (; i<len; ) {
    SKIPSPACE1;
    switch(cmd = buf[i++]) {
    case 'M': case 'm':
      arg[0] = str2f(buf, len, i, ",", &i);
      arg[1] = str2f(buf, len, i, ",", &i);
      move_to(arg[0], arg[1], cmd == 'm');
      break;
      
    case 'L': case 'l':
      arg[0] = str2f(buf, len, i, ",", &i);
      arg[1] = str2f(buf, len, i, ",", &i);
      line_to(arg[0], arg[1], cmd == 'l');
      break;
      
    case 'V': case 'v':
      arg[0] = str2f(buf, len, i, ",", &i);
      vline_to(arg[0], cmd == 'v');
      break;
      
    case 'H': case 'h':
      arg[0] = str2f(buf, len, i, ",", &i);
      hline_to(arg[0], cmd == 'h');
      break;
      
    case 'Q': case 'q':
      arg[0] = str2f(buf, len, i, ",", &i);
      arg[1] = str2f(buf, len, i, ",", &i);
      arg[2] = str2f(buf, len, i, ",", &i);
      arg[3] = str2f(buf, len, i, ",", &i);
      quad_to(arg[0], arg[1], arg[2], arg[3], cmd == 'q');
      break;
      
    case 'T': case 't':
      arg[0] = str2f(buf, len, i, ",", &i);
      arg[1] = str2f(buf, len, i, ",", &i);
      quadT_to(arg[0], arg[1], cmd == 't');
      break;
      
    case 'C': case 'c':
      arg[0] = str2f(buf, len, i, ",", &i);
      arg[1] = str2f(buf, len, i, ",", &i);
      arg[2] = str2f(buf, len, i, ",", &i);
      arg[3] = str2f(buf, len, i, ",", &i);
      arg[4] = str2f(buf, len, i, ",", &i);
      arg[5] = str2f(buf, len, i, ",", &i);
      cubic_to(arg[0], arg[1], arg[2], arg[3], arg[4], arg[5], cmd == 'c');
      break;
      
    case 'S': case 's':
      arg[0] = str2f(buf, len, i, ",", &i);
      arg[1] = str2f(buf, len, i, ",", &i);
      arg[2] = str2f(buf, len, i, ",", &i);
      arg[3] = str2f(buf, len, i, ",", &i);
      cubicS_to(arg[0], arg[1], arg[2], arg[3], cmd == 's');
      break;
      
    case 'A': case 'a':
      ASSERT(0);//("parse_path: Command A: NOT IMPLEMENTED YET");
      break;
    case 'Z': case 'z':
      close_subpath();
      break;
    default:
      printf("parse_path: Invalid Command %c", cmd);
      break;
    }
  }
  return ipos;
}

int PathParse(Path* path, const char* buf, int i, int len) {
  int count = parse_path_impl(NULL, NULL, 0, buf, len);
  PathAddSize(path, count);
  path->Count = parse_path_impl(path->Points, path->Types, path->Count, buf, len);
  return path->Count;
}


int parse_path(gc_t* g, int* opt, float* m, Path* path, const dstr_t* v, int i, int n)
{
  int d = v[i].d;
  for(; i<n-1 && d==v[i].d; i += 2) {
    // The <path> tag can consist of the path itself ("d=") 
    // as well as of other parameters like "style=", "transform=", etc.
    // In the last case we simply rely on the function of parsing 
    // attributes (see 'else' branch).
    if(cstr_cmp(v[i].s->s, v[i].s->l, "d", 1) == 0) {
      PathParse(path, v[i+1].s->s, 0, v[i+1].s->l);
    } else {
      // Create a temporary single pair "name-value" in order
      // to avoid multiple calls for the same attribute.
      parse_attr2(g, opt, m, v[i].s->s, v[i].s->l, v[i+1].s->s, v[i+1].s->l);
    }
  }
  if(path->Count>0) {
    M23TransformPoints(m, path->Points, path->Count);
    //PathPrint(path);
    gcPath(g, *opt, path->Points, path->Types, path->Count);
  }
  return i;
}

int parse_rect(gc_t* g, int* opt, float* m, const dstr_t* v, int i, int n) {
  int d = v[i].d;
  float x = 0.0, y = 0.0, w = 0.0, h = 0.0;
  for(; i<n-1 && d==v[i].d; i += 2) {
    const char* name = v[i].s->s;
    int namelen = v[i].s->l;
    const char* value = v[i+1].s->s;
    if(!parse_attr2(g, opt, m, name, namelen, value, v[i+1].s->l)) {
      if(cstr_cmp(name, namelen, "x", 1) == 0)      x = parse_double(value);
      if(cstr_cmp(name, namelen, "y", 1) == 0)      y = parse_double(value);
      if(cstr_cmp(name, namelen, "width", 5) == 0)  w = parse_double(value);
      if(cstr_cmp(name, namelen, "height", 6) == 0) h = parse_double(value);
      // rx - to be implemented 
      // ry - to be implemented
    }
  }
  
  if(w >0 && h >0) {
    FPOINT Points[4];
    BYTE Types[4];
    path_rect(Points, Types, 0, x, y, x+w, y+h);
    //gcRect(g, *opt, x, y, w, h);
    M23TransformPoints(m, Points, 4);
    gcPath(g, *opt, Points, Types, 4);
  }
  return i;
}

int parse_line(gc_t* g, int* opt, float* m, const dstr_t* v, int i, int n) {
  int d = v[i].d;
  FPOINT pt[2] = {0};
  
  for(; i<n-1 && d==v[i].d; i += 2) {
    const char* name = v[i].s->s;
    int namelen = v[i].s->l;
    const char* value = v[i+1].s->s;
    if(!parse_attr2(g, opt, m, name, namelen, value, v[i+1].s->l)) {
      if(cstr_cmp(name, namelen, "x1", 2) == 0) pt[0].x = parse_double(value);
      if(cstr_cmp(name, namelen, "y1", 2) == 0) pt[0].y = parse_double(value);
      if(cstr_cmp(name, namelen, "x2", 2) == 0) pt[1].x = parse_double(value);
      if(cstr_cmp(name, namelen, "y2", 2) == 0) pt[1].y = parse_double(value);
    }
  }
  M23TransformPoints(m, pt, 2);
  gcDrawLine(g, pt[0].x,pt[0].y,pt[1].x,pt[1].y);
  return i;
}

int vcgen_parse_points(FPOINT* Points, int ipos, const char* buf, int i, int len) {
  double x=0, y=0;
  for (;i<len;) {
    x = str2f(buf, len, i, NULL, &i);
    SKIPSPACE1;
    if (','==buf[i]) {
      y = str2f(buf, len, i+1, NULL, &i);
      vcgen_line_to(x, y);
    } else {
      break;
    }
  }
  return ipos;
}

int parse_poly(gc_t* g, int* opt, float* m, const dstr_t* v, int i, int n, bool closed) {
  int d = v[i].d;
  double x = 0.0, y = 0.0;
  FPOINT* Points = NULL;
  int npt = 0;
  
  for(; i<n-1 && d==v[i].d; i += 2) {
    const char* name = v[i].s->s;
    int namelen = v[i].s->l;
    const char* value = v[i+1].s->s;
    int valuelen = v[i+1].s->l;
    if(!parse_attr2(g, opt, m, name, namelen, value, valuelen)) {
      if(cstr_cmp(name, namelen, "points", 6) == 0) {
        npt = vcgen_parse_points(NULL, 0, value, 0, valuelen);
        MYREALLOC(Points, npt);
        npt = vcgen_parse_points(Points, 0, value, 0, valuelen);
      }
    }
  }
  if (Points && npt>0) {
    if (closed) {
      *opt |= GcOptFill;
    } else {
      *opt |= GcOptStroke;
    }
    M23TransformPoints(m, Points, npt);
    gcPolygon(g, *opt, Points, npt, closed, GC_FLOAT, sizeof(*Points));
    FREE(Points);
  }
  return i;
}

int parse_svg(gc_t* g, int* opt, float* m, Path* path, const dstr_t* v, int i, int n)
{
  if (i<n) {
    int d = v[i].d;
    for (; i<n && v[i].d>=d; ) {
      char* el = v[i].s->s;
      int ellen = v[i].s->l;
      int t = v[i].t;
      ++i;
      if (XML_ELEM==t) {
        if (i<n && XML_ATTR==v[i].t) {
          if (cstr_cmp(el, ellen, "g", 1)==0) {
            i = parse_attr(g, opt, m, v, i+1, n);
          } else if(cstr_cmp(el, ellen, "path", 4) == 0) {
            path->Count = 0;
            i = parse_path(g, opt, m, path, v, i+1, n);
          } else if(cstr_cmp(el, ellen, "rect", 4) == 0) {
            i = parse_rect(g, opt, m, v, i+1, n);
          } else if(cstr_cmp(el, ellen, "line", 4) == 0) {
            i = parse_line(g, opt, m, v, i+1, n);
          } else if(cstr_cmp(el, ellen, "polyline", -1) == 0) {
            i = parse_poly(g, opt, m, v, i+1, n, false);
          } else if(cstr_cmp(el, ellen, "polygon", -1) == 0) {
            i = parse_poly(g, opt, m, v, i+1, n, true);
          }
        }
      }
    }
  }
  return i;
}

int gcDrawSvg(gc_t* g, const dstr_t* v, int i, int n) {
  if (i<n) {
    int d = v[i].d, opt=0;
    float m[6] = {0};
    Path path[1] = {0};
    for (; i<n && v[i].d>=d; ++i) {
      char* el = v[i].s->s;
      int ellen = v[i].s->l;
      int t = v[i].t;
      if (v[i].d==d && XML_ELEM==t) {
        ++i;
        if (cstr_cmp(el, ellen, "svg", 3)==0) {
          if (i<n && XML_ATTR==v[i].t) {
            i = parse_attr(g, &opt, m, v, i+1, n);
          }
          M23Set(m);
          path->Count = 0;
          opt = 0;
          i = parse_svg(g, &opt, m, path, v, i, n);
        } else if(cstr_cmp(el, ellen, "title", 5) == 0) {
          printf("title\n");
        }
      }
    }
    PathFree(path);
  }
  return i;
}
