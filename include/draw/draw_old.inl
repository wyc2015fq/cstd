
#ifndef _DRAW_INL_
#define _DRAW_INL_

#include "cstd.h"
#include "geo/scancell.inl"
int render_scanlines_colormatrix(HDDC hDC, const cell_aa* cells, int cellslen, double* A)
{
  int m_scan_y = 0, bw = hDC->bw >> 2;
  int min_y = cells[0].y;
  int max_y = cells[cellslen - 1].y;
  min_y = MAX(min_y, 0);
  max_y = MIN(max_y, hDC->h - 1);

  while (cells->y < 0) {
    ++cells;
  }

  for (m_scan_y = min_y; m_scan_y <= max_y;) {
    int cover = 0;
    DWORD* ptr = hDC->data + m_scan_y * bw;

    while (m_scan_y == cells->y) {
      int y = m_scan_y;
      const cell_aa* cur_cell = cells;
      int x = cur_cell->x;
      int area = cur_cell->area;
      unsigned alpha, alpha1;
      DWORD clr;
      double r, g, b, a;
      cover += cur_cell->cover;

      //accumulate all cells with the same X
      while (m_scan_y == cells->y) {
        cur_cell = ++cells;

        if (cur_cell->x != x) {
          break;
        }

        area += cur_cell->area;
        cover += cur_cell->cover;
      }

      if (area) {
        alpha = calculate_alpha((cover << (poly_subpixel_shift + 1)) - area);

        if (alpha && x >= 0 && x < hDC->w) {
          r = (A[0]) * x + (A[1]) * y + A[2];
          g = (A[3]) * x + (A[4]) * y + A[5];
          b = (A[6]) * x + (A[7]) * y + A[8];
          a = (A[9]) * x + (A[10]) * y + A[11];
          r = BOUND(r, 0, 255);
          g = BOUND(g, 0, 255);
          b = BOUND(b, 0, 255);
          a = BOUND(a, 0, 255);
          clr = _RGBA(r, g, b, a);
          alpha = (alpha * GetAV(clr)) / 255;
          ptr[x] = RGBBLEN(ptr[x], clr, alpha);
          //ptr[x] = GRADIENTRGB(256, alpha, clr, ptr[x]);
        }

        x++;
      }

      if (cur_cell->x > x) {
        alpha = calculate_alpha(cover << (poly_subpixel_shift + 1));

        if (alpha) {
          for (x = MAX(x, 0); x < cur_cell->x && x < hDC->w; ++x) {
            r = (A[0]) * x + (A[1]) * y + A[2];
            g = (A[3]) * x + (A[4]) * y + A[5];
            b = (A[6]) * x + (A[7]) * y + A[8];
            a = (A[9]) * x + (A[10]) * y + A[11];
            r = BOUND(r, 0, 255);
            g = BOUND(g, 0, 255);
            b = BOUND(b, 0, 255);
            a = BOUND(a, 0, 255);
            clr = _RGBA(r, g, b, a);
            alpha1 = (alpha * GetAV(clr)) / 255;
            //ptr[x]=clr;
            ptr[x] = RGBBLEN(ptr[x], clr, alpha1);
          }
        }
      }
    }

    ++m_scan_y;
  }

  return 0;
}
#define FillPolyCell1(hDC, pt, len, clr) drawaa_fillpoly_solid(hDC, pt, &len, 1, clr)
#define DRAWAA_FILLPOLY_SOLID(hDC, po, clr)  drawaa_fillpoly_solid(hDC, (po)->pt, (po)->len, (po)->n, clr)
int drawaa_fillpoly_solid(HDDC hDC, const DPOINT* pt0, int* lens, int n, COLORREF clr)
{
  int alpha0 = GetAV(clr);
#define ADD_CELL(PIX, _a) { PIX = RGBBLEN(PIX, clr, alpha); }
#include "drawaa_render_scanlines.inl"
#undef ADD_CELL
  return 0;
}
int drawaa_fillpoly_gradient(HDDC hDC, const DPOINT* pt0, int* lens, int n, double x1, double y1, double x2, double y2, COLORREF clr1, COLORREF clr2)
{
  int alpha0 = 255;
  double ds = calc_distance(x1, y1, x2, y2);
  double dx = (x2 - x1) / ds;
  double dy = (y2 - y1) / ds;
  int alpha1, n1 = MAX(1, (int)(ds + 0.0001));
  COLORREF clr = _RGB(255, 255, 255);
#define ADD_CELL(PIX, _a) { int d = (int)((x - x1) * dx + (y - y1) * dy); d = BOUND(d, 0, n1 - 1); \
    clr = n1 ? RGBBLEN2(clr1, clr2, d, n1) : clr1; alpha1 = (alpha * GetAV(clr)) / 255; ptr[x] = RGBBLEN(ptr[x], clr, alpha1); }
#include "drawaa_render_scanlines.inl"
#undef ADD_CELL
  return 0;
}
int drawaa_fillpoly_lut(HDDC hDC, const DPOINT* pt0, int* lens, int n, double x1, double y1, double x2, double y2, COLORREF* clrs)
{
  int alpha0 = 255;
  double ds = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
  double dx = (x2 - x1) / ds;
  double dy = (y2 - y1) / ds;
  int it = -(int)((x1 * dx + y1 * dy) * 256);
  int ix = (int)(dx * 256 * 256);
  int iy = (int)(dy * 256 * 256);
  int n1 = MAX(1, (int)(ds + 0.0001));
  COLORREF clr = _RGB(255, 255, 255);
  int alpha1;
#define ADD_CELL(PIX, _a) { int d = (it + ((x * ix + y * iy)>>8)); d = BOUND(d, 0, 255); clr = clrs[d]; \
    alpha1 = (alpha * GetAV(clr)) / 255; ptr[x] = RGBBLEN(ptr[x], clr, alpha1); }
#include "drawaa_render_scanlines.inl"
#undef ADD_CELL
  return 0;
}
unsigned short _sqrt_table[1 << 16];
int drawaa_fillpoly_lut_radial(HDDC hDC, const DPOINT* pt0, int* lens, int n, double x1, double y1, double rx, double ry, COLORREF* clrs)
{
  int alpha0 = 255;
  COLORREF clr = _RGB(255, 255, 255);
  int alpha1;
#if 1
  static int inited = 0;
  int ix1 = (int)x1;
  int iy1 = (int)y1;
  double iinvrx = (256.*256 / (rx * rx));
  double iinvry = (256.*256 / (ry * ry));

  if (0 == inited) {
    int i;
    inited = 1;

    for (i = 0; i < (1 << 16); ++i) {
      _sqrt_table[i] = (int)sqrt(i);
    }
  }

#define ADD_CELL(PIX, _a) { int d = (int)((ix1-x)*(ix1-x)*iinvrx+(iy1-y)*(iy1-y)*iinvry); d=_sqrt_table[MIN(d, 0xffff)]; clr = clrs[d]; \
    alpha1 = (alpha * GetAV(clr)) / 255; ptr[x] = RGBBLEN(ptr[x], clr, alpha1); }
#else
#define ADD_CELL1(PIX, _a) { int d = (int)(calc_distance(x1, y1, x, y)*256/r); d = BOUND(d, 0, 255); clr = clrs[d]; \
    alpha1 = (alpha * GetAV(clr)) / 255; ptr[x] = RGBBLEN(ptr[x], clr, alpha1); }
#endif
#include "drawaa_render_scanlines.inl"
#undef ADD_CELL
  return 0;
}
int drawaa_fillpoly_colormatrix(HDDC hDC, const DPOINT* pt0, int* lens, int n, double* A)
{
  scancell* s = PolyCell(pt0, lens, n, 0, 0, hDC->w, hDC->h);
  render_scanlines_colormatrix(hDC, s->cells, s->celllen, A);
  free(s);
  return 0;
}
int drawaa_fillpoly_image_perspective(HDDC hDC, const DPOINT* pt0, int* lens, int n, const img_t* im, trans_perspective* p)
{
  int alpha0 = 255;
  COLORREF clr = _RGB(255, 255, 255);
  int alpha1;
  double dx, dy;
  int ix, iy;
  uchar pix1[4];
#define ADD_CELL(PIX, _a) { dx=x, dy=y; trans_perspective_transform(p, &dx, &dy); if (dx>=0 && dy>=0 && dx<im->w && dy<im->h) {\
      dx=BOUND(dx, 0, im->w-2); dy=BOUND(dy, 0, im->h-2); \
      ix=SHIFT_TOINT(dx), iy=SHIFT_TOINT(dy); \
      INTER_BILINEARITY(im->tt.data, ix, iy, im->s, im->c, pix1); clr = _RGBA(pix1[0], pix1[1], pix1[2], pix1[3]); \
      alpha1 = (alpha*GetAV(clr))/255; ptr[x] = RGBBLEN(ptr[x], clr, alpha1); } }
#include "drawaa_render_scanlines.inl"
#undef ADD_CELL
  return 0;
}
#define drawaa_strokeline(hDC, x1, y1, x2, y2, clr1, wline) drawaa_strokeline_gradient(hDC, x1, y1, x2, y2, clr1, clr1, wline)
int drawaa_strokeline_gradient(HDDC hDC, double x1, double y1, double x2, double y2, COLORREF clr1, COLORREF clr2, double wline)
{
  DPOINT* pt;
  DPOINT spt[2];
  stroke_line_style s[1];
  int maxptlen, n, lens[2] = {0};
  ASSERT(wline < 1000);
  maxptlen = 8 + 2 * (int)ceil(wline);
  pt = GMALLOC(DPOINT, maxptlen);
  stroke_line_set(s);
  stroke_line_set_width(s, wline);
  spt[0] = dPOINT(x1, y1);
  spt[1] = dPOINT(x2, y2);
  n = vcgen_stroke(spt, 2, wline, 0, round_cap, round_join, inner_round, maxptlen, pt, lens);

  if (clr1 == clr2) {
    drawaa_fillpoly_solid(hDC, pt, lens, n, clr1);
  }
  else {
    drawaa_fillpoly_gradient(hDC, pt, lens, n, x1, y1, x2, y2, clr1, clr2);
  }

  GFREE(pt);
  return 0;
}
#define drawaa_strokepoly(hDC, pt0, ptlen, closed, clr, wline) drawaa_strokepoly_impl(hDC, pt0, ptlen, closed, clr, wline, round_cap, round_join, inner_round)
int drawaa_strokepoly_impl(HDDC hDC, const DPOINT* pt0, int ptlen, int type, COLORREF clr, double wline,
    line_cap_e m_line_cap, line_join_e m_line_join, inner_join_e m_inner_join)
{
  int n, lens[4] = {0};
  int maxptlen = 8 + ptlen * 2 * (int)MAX(10, wline);
  DPOINT* pt = (DPOINT*)malloc(sizeof(DPOINT) * (maxptlen));
  n = vcgen_stroke(pt0, ptlen, wline, type, m_line_cap, m_line_join, m_inner_join, maxptlen, pt, lens);
  drawaa_fillpoly_solid(hDC, pt, lens, n, clr);
  free(pt);
  return 0;
}
#define drawaa_dashpoly(hDC, pt, ptlen, closed, clr1, wline, m_dash_start, m_dashes, m_num_dashes) drawaa_dashpoly_impl(hDC, pt, ptlen, closed, clr1, wline, m_dash_start, m_dashes, m_num_dashes, round_cap, round_join, inner_round)
int drawaa_dashpoly_impl(HDDC hDC, const DPOINT* pt, int ptlen, int closed, COLORREF clr,
    double wline, double m_dash_start, double* m_dashes, int m_num_dashes,
    line_cap_e m_line_cap, line_join_e m_line_join, inner_join_e m_inner_join)
{
  DPOINT pt0[1000];
  int lens[1000];
  int i, k = 0, n = vcgen_dash(pt, ptlen, closed, m_dashes, m_num_dashes, m_dash_start, countof(pt0), countof(lens), pt0, lens);

  for (i = 0; i < n; i++) {
    drawaa_strokepoly_impl(hDC, pt0 + k, lens[i], 0, clr, wline, m_line_cap, m_line_join, m_inner_join);
    k += lens[i];
    //if (i>0) break;
  }

  return 0;
}

#define drawaa_dashline(hDC, x1, y1, x2, y2, clr1, wline, m_dash_start, m_dashes, m_num_dashes) drawaa_dashline_gradient(hDC, x1, y1, x2, y2, clr1, clr1, wline, m_dash_start, m_dashes, m_num_dashes)
int drawaa_dashline_gradient(HDDC hDC, double x1, double y1, double x2, double y2, COLORREF clr1, COLORREF clr2,
    double wline, double m_dash_start, double* m_dashes, int m_num_dashes)
{
  DPOINT pt[2];
  DPOINT pt0[1000];
  int lens[1000];
  int i, k = 0, n;

  pt[0] = dPOINT(x1, y1);
  pt[1] = dPOINT(x2, y2);
  n = vcgen_dash(pt, 2, 0, m_dashes, m_num_dashes, m_dash_start, countof(pt0), countof(lens), pt0, lens);

  for (i = 0; i < n; i++) {
    drawaa_strokepoly(hDC, pt0 + k, lens[i], 0, clr1, wline);
    k += lens[i];
  }

  return 0;
}
int fill_color_array(int n, COLORREF* array, COLORREF clr1, COLORREF clr2)
{
  int i;

  for (i = 0; i < n; ++i) {
    array[i] = RGBBLEN2(clr1, clr2, i, n);
  }

  return 0;
}
int fill_color_array2(int n, COLORREF* array, int m, const COLORREF* clrs, double* pos)
{
  int i, a = (int)(pos[0] * n), b;

  for (i = 0; i < m - 1; ++i) {
    a = (int)(pos[i] * n);
    b = (int)(pos[i + 1] * n);
    fill_color_array(b - a, array + a, clrs[i], clrs[i + 1]);
  }

  return 0;
}

#define FillPolyGradient(hDC, pt, lens, n, x1, y1, x2, y2, x3, y3, c1, c2, c3) {double x[12], A[9]={0}, b[12]; V9SET(A, x1, x2, x3,y1, y2, y3,1, 1, 1); \
    V12SET(b, GetRV(c1), GetRV(c2), GetRV(c3), GetGV(c1), GetGV(c2), GetGV(c3), GetBV(c1), GetBV(c2), GetBV(c3), GetAV(c1), GetAV(c2), GetAV(c3)); \
    slove3n(A, b, x, countof(b)/3); drawaa_fillpoly_colormatrix(hDC, pt, lens, n, x); }
int test_drawaa(HDDC hDC, int ch)
{
  double ax = 100.5, ay = 100.5, k = 500.;
  DPOINT pt[100];
  stroke_line_style s[1];
  int n, lens[10];
  static double w = .1, t = 0.1;
  DPOINT pt2[100];

  pt[0] = dPOINT(ax, ay);
  pt[1] = dPOINT(ax, ay + k);
  pt[2] = dPOINT(ax + k, ay);
  pt[3] = dPOINT(ax + k, ay + k);

  w += t;

  if (w > 3 || w < 0.2) {
    t = -t;
  }

  stroke_line_set(s);
  stroke_line_set_width(s, w);
  w = 1;
  n = vcgen_stroke(pt, 3, w, 1, round_cap, round_join, inner_round, countof(pt2), pt2, lens);

  if (0) {
    utime_start(_start_time);
    drawaa_fillpoly_solid(hDC, pt2, lens, n, _RGB(255, 0, 0));
    printf("%f\t", utime_elapsed(_start_time));
  }

  if (0) {
    utime_start(_start_time);
    lens[0] = 3;
    drawaa_fillpoly_solid(hDC, pt, lens, 1, _RGB(255, 0, 0));
    printf("%f\t", utime_elapsed(_start_time));
  }

  if (0) {
    utime_start(_start_time);
    //DrawPolyd(hDC, pt, 3, _RGB(255, 0, 0), 0);
    //ScanFill(hDC, pt, 3, _RGB(255, 0, 0));
    printf("%f\t", utime_elapsed(_start_time));
  }

  //printf("=============\n");
  {
    //DPOINT pt[]= {100,100,101,103,103,101};
    //ScanFill2(hDC, pt, 3, _RGB(255, 0, 0));
  }

  if (1) {
    int i, n = 180;
    double dx = hDC->w / 1.0;
    double dy = hDC->h / 1.0;
    double cx = hDC->w / 2.0;
    double cy = hDC->h / 2.0;
    static double ss = 10;
    static int ww = 1;

    if (1) {
      //ww = ((ww+1)%10)+1;
    }

    if ('1' <= ch && ch <= '9') {
      ww = ((ww + 1) % 10) + 1;
      ww = ch - '0';
    }

    if (1 || ch == '1') {
      ss += 1;
    }

    for (i = 0; i < n; i++) {
      double a = 2.0 * PI * i / n;
      double dd[2];
      V2SET(dd, i + 1, i + 1);
      //TDrawDashLine2(hDC, (cx + MIN(cx, cy) *sin(a)), (cy + MIN(cx, cy) *cos(a)), (cx), (cy), _RGB(255, 255, 255), &ss, dd, 2);
      drawaa_dashline_gradient(hDC, (cx), (cy), (cx + MIN(dx, dy) *sin(a)), (cy + MIN(dx, dy) *cos(a)), _RGB(255, 0, 0), _RGB(0, 0, 255), ww, ss, dd, 2);
      //TDrawLined(hDC, (cx + MIN(cx, cy) *sin(a)), (cy + MIN(cx, cy) *cos(a)), (cx), (cy), _RGB(255, 255, 255)); //1.0, (i < 90) ? i : 0.0
    }
  }

  //return 0;
  {
    DWORD c1 = _RGB(255, 0, 0);
    DWORD c2 = _RGB(0, 255, 0);
    DWORD c3 = _RGB(0, 0, 255);
    lens[0] = 4;
    //drawaa_fillpoly_colormatrix(hDC, pt, lens, 1, x);
    FillPolyGradient(hDC, pt, lens, 1, pt[0].x, pt[0].y, pt[1].x, pt[1].y, pt[2].x, pt[3].y, c1, c2, c3);
  }

  if (1) {
    int i, n;
    DPOINT* ptd = pt2;
    COLORREF crWhite = _RGB(255, 0, 255);
    COLORREF clr2, clr = crWhite;

    // DWORD gradient_colors[256];
    for (i = 1; i <= 20; i++) {
      // integral point sizes 1..20
      //i=3;
      n = vcgen_ellipse(20 + i * (i + 1) + 0., 20., i / 2.0, i / 2.0, 8 + i * 2, 0, ptd);
      FillPolyCell1(hDC, ptd, n, clr);
      //break;
      // fractional point sizes 0..2
      n = vcgen_ellipse(18 + i * 4 + 0.5, 33 + 0.5, i / 20.0, i / 20.0, 8, 0, ptd);
      FillPolyCell1(hDC, ptd, n, clr);
      // fractional point positioning
      n = vcgen_ellipse(18 + i * 4 + (i - 1) / 10.0 + 0.5, 27 + (i - 1) / 10.0 + 0.5, 0.5, 0.5, 8, 0, ptd);
      FillPolyCell1(hDC, ptd, n, clr);
      // integral line widths 1..20
      // fractional line positioning, 1 px H
      pt[0].x = 20.5 + i * (i + 1);
      pt[0].y = 40.5;
      pt[1].x = 20.5 + i * (i + 1) + (i - 1) * 4;
      pt[1].y = 100.5;
      //drawaa_fillpoly_solid(hDC, ptd, lens, n, clr);
      clr2 = _RGB(255 * (i % 2), 255 * ((i % 3) * 0.5), 255 * ((i % 5) * 0.25));
      drawaa_strokeline_gradient(hDC, 20.5 + i * (i + 1), 40.5, 20.5 + i * (i + 1) + (i - 1) * 4, 100.5, crWhite, clr2, i);
      // fractional line lengths H (red/blue)
      drawaa_strokeline_gradient(hDC, 17.5 + i * 4, 107.5, 17.5 + i * 4 + i / 6.66666667, 107.5, _RGB(255, 0, 0), _RGB(0, 0, 255), 1);
      // fractional line lengths V (red/blue)
      drawaa_strokeline_gradient(hDC, 18.5 + i * 4, 112.5, 18.5 + i * 4, 112.5 + i / 6.66666667, _RGB(255, 0, 0), _RGB(0, 0, 255), 1);
      // fractional line positioning (red)
      drawaa_strokeline_gradient(hDC, 21.5, 120.5 + (i - 1) * 3., 52.5, 120.5 + (i - 1) * 3., _RGB(255, 0, 0), crWhite, 1.);
      // fractional line width 2..0 (green)
      drawaa_strokeline_gradient(hDC, 52.5, 118 + i * 3, 83.5, 118 + i * 3, _RGB(0, 255, 0), crWhite, 2.0 - (i - 1) / 10.0);
      // stippled fractional width 2..0 (blue)
      drawaa_strokeline_gradient(hDC, 83.5, 119 + i * 3, 114.5, 119 + i * 3, _RGB(0, 0, 255), crWhite, 2.0 - (i - 1) / 10.0);

      // integral line width, horz aligned (mipmap test)
      if (i < 10) {
        drawaa_strokeline_gradient(hDC, 125.5, 119. + (i + 2) * (i / 2.0), 135.5, 119. + (i + 2) * (i / 2.0), crWhite, crWhite, i);
      }

      // fractional line width 0..2, 1 px H
      drawaa_strokeline_gradient(hDC, 17. + i * 4, 192, 18. + i * 4, 192, crWhite, crWhite, i / 10.0);
      // fractional line positioning, 1 px H
      drawaa_strokeline_gradient(hDC, 17. + i * 4 + (i - 1) / 10.0, 186, 18. + i * 4 + (i - 1) / 10.0, 186, crWhite, crWhite, 1.0);
      // Triangles
      {
        DPOINT pt3[3] = {0};
        pt3[0] = dPOINT(150, 220 + i * (i + 1.5));
        pt3[1] = dPOINT(20, 220 + i * (i + 1));
        pt3[2] = dPOINT(20, 220 + i * (i + 2));

        lens[0] = countof(pt3);
        clr2 = _RGB(255 * (i % 2), 255 * (i % 3) * 0.5, 255 * (i % 5) * 0.25);
        drawaa_fillpoly_gradient(hDC, pt3, lens, 1, 150, 220 + i * (i + 1.5), 20, 220 + i * (i + 1), crWhite, clr2);
      }
    }
  }

  return 0;
}
int test_curve(HDDC hDC, int ch)
{
  static DPOINT pt[10000];
  int w = (hDC->w - 120);
  int h = (hDC->h - 80);
  static int n = 0;

  if (0) {
    if (0) {
      if (0 == n) {
        //n = curve3_inc(rand() % w, rand() % h + 80, rand() % w, rand() % h + 80, rand() % w, rand() % h + 80, pt);
        n = vcgen_curve3_div(rand() % w, rand() % h + 80, rand() % w, rand() % h + 80, rand() % w, rand() % h + 80, pt);
        n = vcgen_curve4_inc(rand() % w, rand() % h + 80, rand() % w, rand() % h + 80, rand() % w, rand() % h + 80, rand() % w, rand() % h + 80, pt);
        n = vcgen_arc(300, 300, 200, 100, 0, 1, 1, pt);
        pt[n++] = dPOINT(200, 200);
        ASSERT(n < countof(pt));
      }
    }
    else {
      if (0 == n) {
        double a = 100.1, b = 200;
        pt[n++] = dPOINT(a, 0);
        pt[n++] = dPOINT(a, b);
        pt[n++] = dPOINT(b, 0);
        pt[n++] = dPOINT(b, b);
      }
    }
  }

  if (0 == n) {
    //n = curve3_inc(rand() % w, rand() % h + 80, rand() % w, rand() % h + 80, rand() % w, rand() % h + 80, pt);
    n = vcgen_curve3_div(rand() % w, rand() % h + 80, rand() % w, rand() % h + 80, rand() % w, rand() % h + 80, pt);
    //n = curve4_inc(rand() % w, rand() % h + 80, rand() % w, rand() % h + 80, rand() % w, rand() % h + 80, rand() % w, rand() % h + 80, pt);
    //n = arc_vertex(300, 300, 200, 100, 0, 1, 1, pt); pt[n++]=dPOINT(200, 200);
    ASSERT(n < countof(pt));
  }

  n = vcgen_roundrect1(pt, 100, 100, 500, 350, 25);

  //FillPolyCell1(hDC, pt, n, _RGB(255, 0, 0));
  if (1) {
    utime_start(_start_time);
    drawaa_strokepoly(hDC, pt, n, 1, _RGB(255, 0, 0), 8);
    printf("%f ", utime_elapsed(_start_time));
  }

  return 0;
}

int drawaa_fillrect_gradientR(HDDC hDC, const RECT* rc, COLORREF clr1, COLORREF clr2, BOOL ishorz)
{
  DPOINT pt[4];
  int lens[1];
  lens[0] = vcgen_rectR(pt, rc);

  if (ishorz) {
    drawaa_fillpoly_gradient(hDC, pt, lens, 1, (rc)->left, (rc)->top, (rc)->right, (rc)->top, clr1, clr2);
  }
  else {
    drawaa_fillpoly_gradient(hDC, pt, lens, 1, (rc)->left, (rc)->top, (rc)->left, (rc)->bottom, clr1, clr2);
  }

  return 0;
}
int drawaa_rect(HDDC hDC, double x1, double y1, double x2, double y2, COLORREF crFill, COLORREF crLine, double wline)
{
  DPOINT pt[4];
  int len = vcgen_rect(pt, x1, y1, x2, y2);

  if (crFill) {
    drawaa_fillpoly_solid(hDC, pt, &len, 1, crFill);
  }

  if (crLine) {
    drawaa_strokepoly(hDC, pt, len, 1, crLine, wline);
  }

  return 0;
}
int draw_square(HDDC hDC, double x, double y, double m_size, COLORREF clr, int _a)
{
  DPOINT pt[4];
  int len[1];
  COLORREF c;
  c = _RGBA((GetRV(clr) * _a) / 255, (GetGV(clr) * _a) / 255, (GetBV(clr) * _a) / 255, (GetAV(clr) * _a) / 255);
  len[0] = vcgen_rect(pt, x * m_size, y * m_size, (x + 1) * m_size, (y + 1) * m_size);
  drawaa_fillpoly_solid(hDC, pt, len, 1, c);
  return 0;
}
int drawaa_fillpoly_enlarged(HDDC hDC, const DPOINT* pt0, int* lens, int n, COLORREF clr, double m_size)
{
  int alpha0 = 255;
#define ADD_CELL(PIX, _a) { draw_square(hDC, x, y, m_size, clr, _a); }
#include "drawaa_render_scanlines.inl"
#undef ADD_CELL
  return 0;
}

#include "drawimage.inl"
#include "drawline.inl"
#include "drawrect.inl"
#include "drawcurve.inl"
#include "drawploy.inl"
//#include "drawtext.inl"


#endif // _DRAW_INL_
