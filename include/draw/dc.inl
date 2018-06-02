
#ifndef _DC_INL_
#define _DC_INL_

#include "imdraw.inl"

typedef struct dc_t {
  img_t im[1];
  font_t* ft;
  IPOINT pt;
  IPOINT org;
  IRECT clip;
  COLOR clrFill;
  COLOR clrLine;
  int wline;
  int isaa;

  int pen_alignment;
  int m_line_cap;
  int m_line_join;
  int m_inner_join;


  int brtype;
  COLOR alpha_mask;
  uchar alpha_lut[256];
  COLOR clrs[256];
  int args[256];
  img_t texture[1];


} dc_t;

#define DC_INIT(dc)  int inited = dc_init(dc)
CC_INLINE int dc_init(dc_t* dc)
{
  int i;
  dc->alpha_mask = rgba(1, 1, 1, 1);

  for (i = 0; i < 256; ++i) {
    dc->alpha_lut[i] = i;
  }

  return 0;
}

CC_INLINE int dc_set_solid(dc_t* dc, COLOR clr)
{
  DC_INIT(dc);
  dc->clrs[0] = clr;
  dc->brtype = br_solid;
  return 0;
}
CC_INLINE int dc_set_lut_gradient(dc_t* dc, double x1, double y1, double x2, double y2, const COLOR* clrs)
{
  DC_INIT(dc);
  int alpha0 = 255;
  double ds = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
  double dx = (x2 - x1) / ds;
  double dy = (y2 - y1) / ds;
  int it = -(int)((x1 * dx + y1 * dy) * 256);
  int ix = (int)(dx * 256 * 256);
  int iy = (int)(dy * 256 * 256);
  int n1 = MAX(1, (int)(ds + 0.0001));
  dc->args[0] = ix, dc->args[1] = iy, dc->args[2] = it;
  MEMCPY(dc->clrs, clrs, 256);
  dc->brtype = br_lut_gradient;
  return 0;
}

CC_INLINE int dc_set_gradient(dc_t* dc, double x1, double y1, double x2, double y2, COLOR clr1, COLOR clr2)
{
  DC_INIT(dc);
  fill_color_array(256, dc->clrs, clr1, clr2);
  dc_set_lut_gradient(dc, x1, y1, x2, y2, dc->clrs);
  return 0;
}

CC_INLINE int dc_set_colormatrix(dc_t* dc, const double* A)
{
  DC_INIT(dc);
  int iA[12], i;

  for (i = 0; i < 12; ++i) {
    dc->args[i] = iA[i] = F2FIXN(A[i], 8);
  }

  dc->brtype = br_colormatrix;
  return 0;
}

CC_INLINE int dc_set_lut_radial(dc_t* dc, double x1, double y1, double rx, double ry, const COLOR* clrs)
{
  DC_INIT(dc);
  int ix1 = F2FIXN(x1, 0), iy1 = F2FIXN(y1, 0), invrx = F2FIXN(1 / rx, 16), invry = F2FIXN(1 / ry, 16);
  dc->args[0] = ix1, dc->args[1] = iy1, dc->args[2] = invrx, dc->args[3] = invry;
  MEMCPY(dc->clrs, clrs, 256);
  dc->brtype = br_lut_radial;
  return 0;
}

CC_INLINE int dc_set_radial(dc_t* dc, double x1, double y1, double rx, double ry, COLOR clr1, COLOR clr2)
{
  DC_INIT(dc);
  fill_color_array(256, dc->clrs, clr1, clr2);
  dc_set_lut_radial(dc, x1, y1, rx, ry, dc->clrs);
  return 0;
}

CC_INLINE int dc_set_lut_swirled(dc_t* dc, double x1, double y1, double rx, double ry, const COLOR* clrs)
{
  DC_INIT(dc);
  int ix1 = F2FIXN(x1, 8), iy1 = F2FIXN(y1, 8), invrx = F2FIXN(rx, 8), invry = F2FIXN(ry, 8);
  dc->args[0] = ix1, dc->args[1] = iy1, dc->args[2] = invrx, dc->args[3] = invry;
  MEMCPY(dc->clrs, clrs, 256);
  dc->brtype = br_lut_swirled;
  return 0;
}

CC_INLINE int dc_set_lut_charicature(dc_t* dc, double x1, double y1, double rx, double ry, const COLOR* clrs)
{
  DC_INIT(dc);
  int ix1 = F2FIXN(x1, 8), iy1 = F2FIXN(y1, 8), invrx = F2FIXN(rx, 8), invry = F2FIXN(ry, 8);
  dc->args[0] = ix1, dc->args[1] = iy1, dc->args[2] = invrx, dc->args[3] = invry;
  MEMCPY(dc->clrs, clrs, 256);
  dc->brtype = br_lut_charicature;
  return 0;
}

CC_INLINE int dc_set_lut_diamond(dc_t* dc, double x1, double y1, double rx, double ry, const COLOR* clrs)
{
  DC_INIT(dc);
  int ix1 = F2FIXN(x1, 8), iy1 = F2FIXN(y1, 8), invrx = F2FIXN(1 / rx, 8), invry = F2FIXN(1 / ry, 8);
  dc->args[0] = ix1, dc->args[1] = iy1, dc->args[2] = invrx, dc->args[3] = invry;
  MEMCPY(dc->clrs, clrs, 256);
  dc->brtype = br_lut_diamond;
  return 0;
}

CC_INLINE int dc_set_texture(dc_t* dc, int wrap_mode_x, int wrap_mode_y, int h, int w, const uchar* data, int al, int ai)
{
  DC_INIT(dc);
  IMINIT(dc->texture, h, w, data, al, ai, 1);
  dc->args[0] = wrap_mode_x;
  dc->args[1] = wrap_mode_y;
  dc->brtype = br_texture;
  return 0;
}

CC_INLINE int dc_set_gouraud(dc_t* dc, double x1, double y1, double x2, double y2, double x3, double y3, COLOR c1, COLOR c2, COLOR c3)
{
  DC_INIT(dc);
  double x[12], A[9] = {0}, b[12];
  const uchar* p1 = (const uchar*)&c1;
  const uchar* p2 = (const uchar*)&c2;
  const uchar* p3 = (const uchar*)&c3;
  V9SET(A, x1, x2, x3, y1, y2, y3, 1, 1, 1);
  V12SET(b, GetRV(c1), GetRV(c2), GetRV(c3), GetGV(c1), GetGV(c2), GetGV(c3), GetBV(c1), GetBV(c2), GetBV(c3), GetAV(c1), GetAV(c2), GetAV(c3));
  slove3n(A, b, x, countof(b) / 3);
  dc_set_colormatrix(dc, x);
  return 0;
}


#include "dcdraw.inl"

int dc_moveto(dc_t* dc, int x, int y)
{
  dc->pt.x = x, dc->pt.y = y;
  return 0;
}

int dc_lineto(dc_t* dc, int x, int y, COLOR clrLine, int wline)
{
  return 0;
}

int dc_polygon(dc_t* dc, const IPOINT* pts, int npts, COLOR clrFill, COLOR clrLine, int wline)
{
  FillPolyM(dc->im, pts, &npts, 1, clrFill, 0);
  imdraw_poly(dc->im, pts, &npts, 1, 1, clrLine, wline);
  return 0;
}
int dc_rect(dc_t* dc, IRECT rc, COLOR clrFill, COLOR clrLine, int wline)
{
  //imdraw_rect(dc->im, rc, clrFill, clrLine, wline);
  imdraw_rect_clip(dc->im, &dc->clip, rc, clrFill, clrLine, wline);
  return 0;
}
// Rounded Rectangle
int dc_rrect(dc_t* dc, IRECT rc, COLOR clrFill, COLOR clrLine, int wline)
{
  // Approximate a round rect with some cut off corners
  IPOINT pts[] = {
    rc.l + 2, rc.t,
    rc.r - 2, rc.t,
    rc.r, rc.t + 2,
    rc.r, rc.b - 2,
    rc.r - 2, rc.b,
    rc.l + 2, rc.b,
    rc.l, rc.b - 2,
    rc.l, rc.t + 2,
  };
  //dcPolygon(dc, pts, countof(pts), clrFill, clrLine, wline);
  return 0;
}

int dcCopy(dc_t* dc, IRECT rc, IPOINT from, dc_t* dcsrc)
{
  return 0;
}

int dcSetFont(const LOGFONT* pLogFont)
{
  return 0;
}
int dc_image(dc_t* dc, IRECT rc, const img_t* im)
{
  return 0;
}
int dc_ellipse(dc_t* dc, IRECT rc, COLOR fore, COLOR back)
{
  return 0;
}
//int DrawTextNoClip(dc_t* dc, IRECT rc, Font &font_, XYPOSITION ybase, const char *s, int len, COLOR fore, COLOR back)=0;
// DrawTextClipped
int dc_text(dc_t* dc, IRECT rc, int x, int y, const char* s, int len, COLOR fore, COLOR back)
{
  return 0;
}
//int MeasureWidths(Font &font_, const char *s, int len, XYPOSITION *positions)=0;
//XYPOSITION WidthText(Font &font_, const char *s, int len)=0;
//XYPOSITION WidthChar(Font &font_, char ch)=0;
//XYPOSITION Ascent(Font &font_)=0;
//XYPOSITION Descent(Font &font_)=0;
//XYPOSITION InternalLeading(Font &font_)=0;
//XYPOSITION ExternalLeading(Font &font_)=0;
//XYPOSITION Height(Font &font_)=0;
//XYPOSITION AverageCharWidth(Font &font_)=0;

int dc_setclip(dc_t* dc, IRECT rc)
{
  dc->clip = rc;
  return 0;
}

#endif // _DC_INL_
