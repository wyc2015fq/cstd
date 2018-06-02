#ifndef _SVG_PARSER_INL_
#define _SVG_PARSER_INL_
#define rel_to_abs(a, b)   if (sf->ptLen>0) { a += sf->pt[sf->ptLen-1].x, b += sf->pt[sf->ptLen-1].y;}
#define begin_path()  {sf->attr[sf->attrLen]=sf->attr[sf->attrTop];sf->attr[sf->attrLen].beg=sf->ptLen;
#define end_path()    sf->attr[sf->attrLen].len=sf->ptLen-sf->attr[sf->attrLen].beg;sf->attrLen++;}
#define last_number(_X) (_X=strtod(s, &s), s+=!!(*s))
//#define next() 0
#define set_pt(_I, a, b, c)   {(sf->pt[_I].x=a, sf->pt[_I].y=b), sf->cmd[_I]=c;}
#define get_pt(_I, a, b)      (a=(sf->pt[_I].x, b=sf->pt[_I].y))
#define close_subpath() 0
typedef DWORD rgba8;
typedef struct path_attributes {
  unsigned index;
  COLOR fill_color;
  COLOR stroke_color;
  bool fill_flag;
  bool stroke_flag;
  bool even_odd_flag;
  line_join_e line_join;
  line_cap_e line_cap;
  double miter_limit;
  double stroke_width;
  trans_affine transform;
  int beg, len;
} path_attributes;
int init_path_attributes(path_attributes* pa)
{
  pa->index = (0);
  pa->fill_color = (rgba(0, 0, 0, 1));
  pa->stroke_color = (rgba(0, 0, 0, 1));
  pa->fill_flag = (true);
  pa->stroke_flag = (false);
  pa->even_odd_flag = (false);
  pa->line_join = (miter_join);
  pa->line_cap = (butt_cap);
  pa->miter_limit = (4.0);
  pa->stroke_width = (1.0);
  trans_affine_init(&pa->transform);
  return 0;
}
typedef struct svgfile {
  path_attributes* attr;
  char* cmd;
  DPOINT* pt;
  int attrLen;
  int attrTop;
  int ptLen;
  int width;
  int height;
  char* title;
} svgfile;
void parse_style(svgfile* sf, const char* str);

int move_to(svgfile* sf, double a, double b, bool rel)
{
  if (rel) {
    rel_to_abs(a, b);
  }

  set_pt(sf->ptLen, a, b, path_move_to);
  sf->ptLen++;
  return 0;
}
int line_to(svgfile* sf, double a, double b, bool rel)
{
  if (rel) {
    rel_to_abs(a, b);
  }

  set_pt(sf->ptLen, a, b, path_line_to);
  sf->ptLen++;
  return 0;
}
int vline_to(svgfile* sf, double y, bool rel)
{
  if (sf->ptLen > 0) {
    line_to(sf, sf->pt[sf->ptLen - 1].x, y, rel);
  }

  return 0;
}
int hline_to(svgfile* sf, double x, bool rel)
{
  if (sf->ptLen > 0) {
    line_to(sf, x, sf->pt[sf->ptLen - 1].y, rel);
  }

  return 0;
}
int curve3Q(svgfile* sf, double a, double b, double c, double d, bool rel)
{
  if (rel) {
    rel_to_abs(a, b);
    rel_to_abs(c, d);
  }

  set_pt(sf->ptLen + 0, a, b, path_curve3);
  set_pt(sf->ptLen + 1, c, d, path_curve3);
  sf->ptLen += 2;
  return 0;
}
int curve3T(svgfile* sf, double c, double d, bool rel)
{
  if (sf->ptLen > 1) {
    double x0, y0, x1, y1;

    if (rel) {
      rel_to_abs(c, d);
    }

    get_pt(sf->ptLen - 1, x0, y0);

    if (is_curve(sf->cmd[sf->ptLen - 2])) {
      get_pt(sf->ptLen - 2, x1, y1);
      x0 = x0 + x0 - x1, y0 = y0 + y0 - y1;
    }

    curve3Q(sf, x0, y0, c, d, 0);
  }

  return 0;
}
int curve4C(svgfile* sf, double a, double b, double c, double d, double e, double f, bool rel)
{
  if (rel) {
    rel_to_abs(a, b);
    rel_to_abs(c, d);
    rel_to_abs(e, f);
  }

  set_pt(sf->ptLen + 0, a, b, path_curve4);
  set_pt(sf->ptLen + 1, c, d, path_curve4);
  set_pt(sf->ptLen + 2, e, f, path_curve4);
  sf->ptLen += 3;
  return 0;
}
int curve4S(svgfile* sf, double c, double d, double e, double f, bool rel)
{
  if (sf->ptLen > 1) {
    double x0, y0, x1, y1;

    if (rel) {
      rel_to_abs(c, d);
    }

    get_pt(sf->ptLen - 1, x0, y0);

    if (is_curve(sf->cmd[sf->ptLen - 2])) {
      get_pt(sf->ptLen - 2, x1, y1);
      x0 = x0 + x0 - x1, y0 = y0 + y0 - y1;
    }

    curve4C(sf, x0, y0, c, d, e, f, 0);
  }

  return 0;
}
void parse_poly(svgfile* sf, const char** attr, BOOL close_flag)
{
  int i;
  double x = 0.0;
  double y = 0.0;
  begin_path();

  for (i = 0; attr[i]; i += 2) {
    if (!parse_attr2(sf, attr[i], attr[i + 1])) {
      if (strcmp(attr[i], "points") == 0) {
        char* s = (char*)attr[i + 1];

        if (! *s) {
          assert(0 && "parse_poly: Too few coordinates");
        }

        last_number(x);

        if (! *s) {
          assert(0 && "parse_poly: Too few coordinates");
        }

        last_number(y);
        move_to(sf, x, y, 0);

        if (x == 173 && y == 339) {
          int asdf = 0;
        }

        while (*s) {
          last_number(x);

          if (! *s) {
            assert(0 && "parse_poly: Odd number of coordinates");
          }

          last_number(y);
          line_to(sf, x, y, 0);
        }
      }
    }
  }

  if (close_flag) {
    close_subpath();
  }

  end_path();
}
#endif // _SVG_PARSER_INL_
