
#ifndef _IMDRAWAA_INL_
#define _IMDRAWAA_INL_

#include "softgc_c.h"

#define IMDRAWAA_BEGIN  { softgc s[1] = {0}; *s->im = *im; softgc_reset(s); s->sl->pclip = pclip; {int w = im->w, h = im->h
#define IMDRAWAA_END  } softgc_free(s);}

int imdraw_recti_solid(img_t* im, bool flip_y, const IRECT* pclip, IRECT rect, COLOR clr) {
  IMDRAWAA_BEGIN;
  brush_set_solid(s->brush, clr);
  scanline_set_recti(s->sl, rect);
  IMDRAWAA_END;
  return 0;
}
int imdraw_poly_solid(img_t* im, bool flip_y, const IRECT* pclip, const FPOINT* pt, const int* lens, int n, COLOR clr) {
  IMDRAWAA_BEGIN;
  brush_set_solid(s->brush, clr);
  scanline_set_poly(s->sl, pt, lens, n, false);
  IMDRAWAA_END;
  return 0;
}
int imdraw_ellipse_solid(img_t* im, bool flip_y, const IRECT* pclip, double x, double y, double rx, double ry, double begin_angle, double end_angle, COLOR clr) {
  IMDRAWAA_BEGIN;
  FPOINT pt[64];
  int n = vcgen_ellipse(pt, countof(pt), 0, x, y, rx, ry, begin_angle, end_angle);
  brush_set_solid(s->brush, clr);
  scanline_set_poly(s->sl, pt, &n, 1, false);
  IMDRAWAA_END;
  return 0;
}
int imdraw_poly_stroke(img_t* im, bool flip_y, const IRECT* pclip, const FPOINT* pt, int ptlen, bool closed, COLOR clr, double lineWidth)
{
  IMDRAWAA_BEGIN;
  brush_set_solid(s->brush, clr);
  s->penstyle->lineWidth = lineWidth;
  scanline_set_poly_dash_stroke(s->sl, pt, ptlen, closed, s->penstyle);
  IMDRAWAA_END;
  return 0;
}
int imdraw_poly(img_t* im, bool flip_y, const IRECT* pclip, const FPOINT* pt, int ptlen, bool closed, COLOR clrFill, COLOR clrLine, double lineWidth) {
  if (clrFill) {
    imdraw_poly_solid(im, flip_y, pclip, pt, &ptlen, 1, clrFill);
  }
  if (clrLine) {
    imdraw_poly_stroke(im, flip_y, pclip, pt, ptlen, closed, clrLine, lineWidth);
  }
  return 0;
}
CC_INLINE int imdraw_poly(img_t* im, bool flip_y, const IRECT* pclip, const IPOINT* pt, int n, int closed, COLOR crFill, COLOR crLine, int wline)
{
  IMDRAWAA_BEGIN;
  if (crFill) {
    brush_set_solid(s->brush, crFill);
    scanline_bin_fillpoly(s->sl, pt, &n, 1, 0);
  }
  
  if (crLine) {
    brush_set_solid(s->brush, crLine);
    scanline_bin_polyline(s->sl, pt, &n, 1, wline);
  }
  IMDRAWAA_END;
  return 0;
}
CC_INLINE int imdraw_dpoint(img_t* im, bool flip_y, const IRECT* pclip, const DPOINT* pts, int n, COLOR clr) {
  IMDRAWAA_BEGIN;
  brush_set_solid(s->brush, clr);
  scanline_dpoints(s->sl, pts, n);
  IMDRAWAA_END;
  return 0;
}
int imdraw_rect_stroke(img_t* im, bool flip_y, const IRECT* pclip, DRECT drect, COLOR clr, float lineWidth) {
  FPOINT pt[4];
  vcgen_rect(pt, 0, drect.l, drect.t, drect.r, drect.b);
  return imdraw_poly_stroke(im, flip_y, pclip, pt, 4, true, clr, lineWidth);
}
int imdraw_text(img_t* im, bool flip_y, const IRECT* pclip, const char* text, IRECT rc, COLOR clr, int fmt, font_t* font, float size) {
  IMDRAWAA_BEGIN;
  brush_set_solid(s->brush, clr);
  scanline_set_string(s->sl, 0, text, NULL, font, size, rc.l, rc.t, RCW(&rc), RCH(&rc), fmt);
  IMDRAWAA_END;
  return 0;
}
#if 0
int imdraw_textv(img_t* im, bool flip_y, const IRECT* pclip, const char* text, int org_x, int org_y, enum CC_FONT_FACE font_face, double font_scale, COLOR color, double lineWidth)
{
  IMDRAWAA_BEGIN;
  brush_set_solid(s->brush, color);
  s->penstyle->lineWidth = lineWidth;
  scanline_bin_text_v(s->sl, text, org_x, org_y, font_face, font_scale, s->penstyle);
  IMDRAWAA_END;
  return 0;
}
#endif
int imdraw_ellipse(img_t* im, bool flip_y, const IRECT* pclip, double x, double y, double rx, double ry, double begin_angle, double end_angle, COLOR clrFill, COLOR clrLine, double lineWidth) {
  FPOINT pt[64];
  int n = vcgen_ellipse(pt, countof(pt), 0, x, y, rx, ry, begin_angle, end_angle);
  return imdraw_poly(im, flip_y, pclip, pt, n, true, clrFill, clrLine, lineWidth);
}
int imdraw_circle(img_t* im, const IRECT* pclip, double x, double y, double r, COLOR clrFill, COLOR clrLine, double lineWidth) {
  return imdraw_ellipse(im, false, pclip, x, y, r, r, clrFill, clrLine, lineWidth, 0, 360);
}
int imdraw_line(img_t* im, const IRECT* pclip, double x1, double y1, double x2, double y2, COLOR clrLine, double lineWidth) {
  FPOINT pt[2] = { (float)x1, (float)y1, (float)x2, (float)y2 };
  return imdraw_poly_stroke(im, false, pclip, pt, 2, false, clrLine, lineWidth);
}
int imdraw_line(img_t* im, const IRECT* pclip, IPOINT p1, IPOINT p2, COLOR clrLine, double lineWidth) {
  FPOINT pt[2] = { (float)p1.x, (float)p1.y, (float)p2.x, (float)p2.y };
  return imdraw_poly_stroke(im, false, pclip, pt, 2, false, clrLine, lineWidth);
}
int imdraw_rect(img_t* im, bool flip_y, const IRECT* pclip, DRECT drect, COLOR clrFill, COLOR clrLine, float lineWidth) {
  FPOINT pt[4];
  vcgen_rect(pt, 0, drect.l, drect.t, drect.r, drect.b);
  return imdraw_poly(im, flip_y, pclip, pt, 4, true, clrFill, clrLine, lineWidth);
}
int imdraw_rect(img_t* im, bool flip_y, const IRECT* pclip, IRECT drect, COLOR clrFill, COLOR clrLine, float lineWidth) {
  FPOINT pt[4];
  vcgen_rect(pt, 0, drect.l, drect.t, drect.r, drect.b);
  return imdraw_poly(im, flip_y, pclip, pt, 4, true, clrFill, clrLine, lineWidth);
}
#if 0
int imdraw_poly_dash_stroke_brush(img_t* im, bool flip_y, const IRECT* pclip, const FPOINT* pt, int ptlen, bool closed, brush_t* br, float lineWidth, const float* m_dashes, int m_num_dashes, double m_dash_start) {

  return 0;
}
int imdraw_poly_stroke_solid(img_t* im, bool flip_y, const IRECT* pclip, const FPOINT* pt, int ptlen, bool closed, COLOR clr, float lineWidth)
{
  brush_t br[1] = {0};
  brush_set_solid(br, clr);
  imdraw_poly_stroke_brush(im, flip_y, pclip, pt, ptlen, closed, br, lineWidth);
  return 0;
}
int imdraw_poly_dash_stroke_solid(img_t* im, bool flip_y, const IRECT* pclip, const FPOINT* pt, int ptlen, bool closed, COLOR clr, float lineWidth, const float* m_dashes, int m_num_dashes, double m_dash_start) {
  brush_t br[1] = {0};
  brush_set_solid(br, clr);
  imdraw_poly_dash_stroke_brush(im, flip_y, pclip, pt, ptlen, closed, br, lineWidth, m_dashes, m_num_dashes, m_dash_start);
  return 0;
}
#endif

#if 0

CC_INLINE int imdraw_spline(scanline* sl, const DPOINT* pt0, int n0, int closed, COLOR crFill, COLOR crLine, double wline)
{
  DPOINT pt[128];
  int n = vcgen_bspline(countof(pt), pt, n0, pt0, closed);
  imdraw_poly(sl, pt, n, closed, crFill, crLine, wline);
  return 0;
}

CC_INLINE int imdraw_line(scanline* sl, double x1, double y1, double x2, double y2, COLOR crLine, double wline)
{
  DPOINT pt[2];
  pt[0] = dPOINT(x1, y1);
  pt[1] = dPOINT(x2, y2);
  imdraw_poly(sl, pt, 2, 0, 0, crLine, wline);
  return 0;
}

//#define F2FIX8(x)  F2FIXN(x, 8)

CC_INLINE int imdraw_ellipse(scanline* sl, double x, double y, double rx, double ry, COLOR crFill, COLOR crLine, double wline)
{
  DPOINT pt[64];
  int n = vcgen_ellipse(pt, countof(pt), 0, x, y, rx, ry);
  imdraw_poly(sl, pt, n, 1, crFill, crLine, wline);
  return 0;
}
CC_INLINE int imdraw_ellipse_irect(scanline* sl, IRECT rc, COLOR crFill, COLOR crLine, double wline)
{
  return imdraw_ellipse(sl, (rc.l + rc.r) * 0.5, (rc.t + rc.b) * 0.5, (rc.r - rc.l) * 0.5, (rc.b - rc.t) * 0.5, crFill, crLine, wline);
}

CC_INLINE int imdraw_circle(scanline* sl, double x, double y, double r, COLOR crFill, COLOR crLine, double wline)
{
  return imdraw_ellipse(sl, x, y, r, r, crFill, crLine, wline);
}

// 扇形
// x1 y1 外围矩形左上点XY坐标
// x2 y2 外围矩形右下点XY坐标
// rx1 ry1 起点XY坐标
// rx2 ry2 终点XY坐标
#define imdraw_pieR(im, pclip, rc, a1, a2, crFill, crLine, wline)  imdraw_pie(im, pclip, RCX(&rc)/2., RCY(&rc)/2., RCW(&rc)/2., RCH(&rc)/2., a1, a2, crFill, crLine, wline)
CC_INLINE int imdraw_pie(scanline* sl, double x, double y, double rx, double ry, double a1, double a2, COLOR crFill, COLOR crLine, double wline)
{
  int n;
  DPOINT pt[64];
  //int n = vcgen_pie(pt, 0, countof(pt), 0, x, y, rx, ry, rx1, ry1, rx2, ry2);
  //double a1 = atan2(ry1 - y, rx1 - x);
  //double a2 = atan2(ry2 - y, rx2 - x);
  a1 *= PI / 180;
  a2 *= PI / 180;
  n = vcgen_arc(pt, 0, 60, 1, x, y, rx, ry, a1, a2);
  pt[n++] = dPOINT(x, y);
  imdraw_poly(im, pclip, pt, n, 1, crFill, crLine, wline);
  return 0;
}

CC_INLINE int imdraw_triangle(scanline* sl, double x1, double y1, double x2, double y2, double x3, double y3, COLOR clrFill, COLOR clrLine, double wline)
{
  DPOINT pt[10];
  int n = 3;
  pt[0] = dPOINT(x1, y1);
  pt[1] = dPOINT(x2, y2);
  pt[2] = dPOINT(x3, y3);
  imdraw_poly(im, pclip, pt, n, 1, clrFill, clrLine, wline);
  return 0;
}

#define drawaa_roundrectR(hDC, rc, r, clr, wline) drawaa_roundrect(hDC, (rc)->left,(rc)->top,(rc)->right,(rc)->bottom, r, clr, wline)
CC_INLINE int drawaa_roundrect(scanline* sl, double x1, double y1, double x2, double y2, int r, COLOR clrFill, COLOR clrLine, double wline)
{
  DPOINT pt[100];
  int n = vcgen_roundrect1(pt, 0, countof(pt), x1, y1, x2, y2, r);
  imdraw_poly(im, pclip, pt, n, 1, clrFill, clrLine, wline);
  return 0;
}

CC_INLINE int imdraw_dashpoly_impl(scanline* sl, const DPOINT* pt, int ptlen, COLOR clr,
    double wline, int m_dash_start, const double* m_dashes, int m_num_dashes, int m_line_style)
{
  DPOINT pt0[1000];
  int lens[1000];
  int closed = (m_line_style & ALIGN_MASK) != ALIGN_NUL;
  int i, k = 0, n = vcgen_dash(pt, ptlen, closed, m_dashes, m_num_dashes, m_dash_start, countof(pt0), countof(lens), pt0, lens);

  for (i = 0; i < n; i++) {
    imdraw_poly_stroke_solid_impl(im, pclip, pt0 + k, lens[i], clr, wline, m_line_style);
    k += lens[i];
    //if (i>0) break;
  }

  return 0;
}

CC_INLINE int imdraw_dashpoly(scanline* sl, const DPOINT* pt, int ptlen, int closed, COLOR clr1, double wline, int m_dash_start, const double* m_dashes, int m_num_dashes)
{
  int m_line_style = CAP_ROUND | JOIN_ROUND | INNER_ROUND | (closed ? ALIGN_INSET : ALIGN_NUL);
  return imdraw_dashpoly_impl(im, pclip, pt, ptlen, clr1, wline, m_dash_start, m_dashes, m_num_dashes, m_line_style);
}

CC_INLINE int imdraw_dashpoly2(scanline* sl, const DPOINT* pt, int ptlen, int closed, COLOR clr1, double wline, double dashe1, double dashe2)
{
  double m_dashes[2] = {0};
  m_dashes[0] = dashe1, m_dashes[1] = dashe2;
  int m_line_style = CAP_ROUND | JOIN_ROUND | INNER_ROUND | (closed ? ALIGN_INSET : ALIGN_NUL);
  return imdraw_dashpoly_impl(im, pclip, pt, ptlen, clr1, wline, 0, m_dashes, countof(m_dashes), m_line_style);
}


CC_INLINE int imdraw_line_gradient(scanline* sl, double x1, double y1, double x2, double y2, COLOR clr1, COLOR clr2, double wline)
{
  DPOINT* pt;
  DPOINT spt[2];
  stroke_line_style s[1];
  int maxptlen, n, lens[2] = {0};
  int m_line_style = CAP_ROUND | JOIN_ROUND | INNER_ROUND;
  //ASSERT(wline < 1000);
  maxptlen = 8 + 2 * (int)ceil(wline);
  pt = MALLOC(DPOINT, maxptlen);
  stroke_line_set(s);
  stroke_line_set_width(s, wline);
  spt[0] = dPOINT(x1, y1);
  spt[1] = dPOINT(x2, y2);
  n = vcgen_stroke(spt, 2, wline, m_line_style, maxptlen, pt, lens);

  if (clr1 == clr2) {
    imdraw_poly_solid(im, pclip, pt, lens, n, clr1);
  }
  else {
    imdraw_poly_gradient(im, pclip, pt, lens, n, x1, y1, x2, y2, clr1, clr2);
  }

  FREE(pt);
  return 0;
}

CC_INLINE int imdraw_dashline_gradient(scanline* sl, double x1, double y1, double x2, double y2, COLOR clr1, COLOR clr2,
    double wline, int m_dash_start, double* m_dashes, int m_num_dashes)
{
  DPOINT pt[2];
  int i, k = 0, n;
  DPOINT pt0[1000];
  int lens[1000];

  pt[0] = dPOINT(x1, y1);
  pt[1] = dPOINT(x2, y2);

  if (m_num_dashes > 0) {
    n = vcgen_dash(pt, 2, 0, m_dashes, m_num_dashes, m_dash_start, countof(pt0), countof(lens), pt0, lens);

    for (i = 0; i < n; i++) {
      imdraw_poly(im, pclip, pt0 + k, lens[i], 0, 0, clr1, wline);
      k += lens[i];
    }
  }
  else {
    imdraw_line_gradient(im, pclip, x1, y1, x2, y2, clr1, clr2, wline);
  }

  return 0;
}

CC_INLINE int imdraw_dashline(scanline* sl, double x1, double y1, double x2, double y2, COLOR clr1,
    double wline, int m_dash_start, double* m_dashes, int m_num_dashes)
{
  return imdraw_dashline_gradient(im, pclip, x1, y1, x2, y2, clr1, clr1, wline, m_dash_start, m_dashes, m_num_dashes);
}


CC_INLINE int imdraw_dashed_line(scanline* sl, double x1, double y1, double x2, double y2, double wline, double dash_length, COLOR clr)
{
  DPOINT pt[2];
  pt[0] = dPOINT(x1, y1);
  pt[1] = dPOINT(x2, y2);

  if (dash_length > 0) {
    double dd[2] = {dash_length, dash_length};
    imdraw_dashline(im, pclip, x1, y1, x2, y2, clr, wline, 0, dd, 2);
  }
  else {
    imdraw_line(im, pclip, x1, y1, x2, y2, clr, wline);
  }

  return 0;
}

// 画多角星星 角数=n*m
CC_INLINE int imdraw_star(scanline* sl, double x, double y, double* rad, int n, int m, COLOR crFill, COLOR crLine, double wline)
{
  int i, mn = n * m;
  DPOINT* pt = MALLOC(DPOINT, mn * sizeof(DPOINT));

  for (i = 0; i < mn; ++i) {
    double r = rad[i % n], ang = i * PI * 2 / mn;
    pt[i] = dPOINT(x + r * sin(ang), y + r * cos(ang));
  }

  imdraw_poly(im, pclip, pt, m * n, 1, crFill, crLine, wline);
  FREE(pt);
  return 0;
}

CC_INLINE BOOL imdraw_poly_path_gradient(scanline* sl, const DPOINT* pt, int n, const COLOR* clrs, const double* pos, int nclr, const DPOINT* pcenter)
{
  COLOR lut[256];
  fill_color_array2(256, lut, nclr, clrs, pos);
  return imdraw_poly_path_gradient_lut(sl, pt, n, lut, pcenter);
}
CC_INLINE int imdraw_poly_radial(scanline* sl, const DPOINT* pt0, const int* lens, int n, double x1, double y1, double rx, double ry, COLOR clr1, COLOR clr2)
{
  brush_t br[1] = {0};
  brush_set_radial(br, x1, y1, rx, ry, clr1, clr2);
  imdraw_poly_brush(sl, pt0, lens, n, br);
  return 0;
}

CC_INLINE int imdraw_poly_lut_radial(scanline* sl, const DPOINT* pt0, const int* lens, int n, double x1, double y1, double rx, double ry, const COLOR* clrs)
{
  brush_t br[1] = {0};
  brush_set_lut_radial(br, x1, y1, rx, ry, clrs);
  imdraw_poly_brush(sl, pt0, lens, n, br);
  return 0;
}

CC_INLINE int imdraw_poly_colormatrix(scanline* sl, const DPOINT* pt0, const int* lens, int n, const double* A)
{
  brush_t br[1] = {0};
  brush_set_colormatrix(br, A);
  imdraw_poly_brush(sl, pt0, lens, n, br);
  return 0;
}

CC_INLINE int imdraw_poly_gouraud(scanline* sl, const DPOINT* pt0, const int* lens, int n, double x1, double y1, double x2, double y2, double x3, double y3, COLOR c1, COLOR c2, COLOR c3)
{
  brush_t br[1] = {0};
  brush_set_gouraud(br, x1, y1, x2, y2, x3, y3, c1, c2, c3);
  imdraw_poly_brush(sl, pt0, lens, n, br);
  return 0;
}

CC_INLINE int imdraw_triangle_gouraud(scanline* sl, double x1, double y1, double x2, double y2, double x3, double y3, COLOR clr1, COLOR clr2, COLOR clr3)
{
  DPOINT pt[10];
  int n = 3;
  pt[0] = dPOINT(x1, y1), pt[1] = dPOINT(x2, y2), pt[2] = dPOINT(x3, y3);
  imdraw_poly_gouraud(sl, pt, &n, 1, x1, y1, x2, y2, x3, y3, clr1, clr2, clr3);
  return 0;
}
CC_INLINE BOOL imdraw_poly_gouraud_n(scanline* sl, const DPOINT* pt, const COLOR* clrs, const int* pMesh, int nMesh)
{
  int i, i1, i2, i3;
  
  for (i = 0; i < nMesh; ++i) {
    i1 = pMesh[3 * i + 0];
    i2 = pMesh[3 * i + 1];
    i3 = pMesh[3 * i + 2];
    imdraw_triangle_gouraud(sl, pt[i1].x, pt[i1].y, pt[i2].x, pt[i2].y, pt[i3].x, pt[i3].y, clrs[i1], clrs[i2], clrs[i3]);
  }
  
  return 0;
}

CC_INLINE int imdraw_poly_path_gouraud(scanline* sl, const DPOINT* pt, int n, const COLOR* clrs, COLOR clrCenter, const DPOINT* pcenter)
{
  DPOINT center = {0, 0};
  int i;
  
  if (pcenter) {
    center = *pcenter;
  }
  else {
    for (i = 0; i < n; ++i) {
      center.x += pt[i].x;
      center.y += pt[i].y;
    }
    
    center.x /= n;
    center.y /= n;
  }
  
  for (i = 0; i < n; ++i) {
    int j = (i + 1) % n;
    imdraw_triangle_gouraud(sl, center.x, center.y, pt[i].x, pt[i].y, pt[j].x, pt[j].y, clrCenter, clrs[i], clrs[j]);
  }
  
  return 0;
}

#endif
#endif // _IMDRAWAA_INL_
