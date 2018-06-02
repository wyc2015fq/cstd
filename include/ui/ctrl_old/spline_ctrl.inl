#include "geo\bspline.inl"
typedef struct spline_ctrl {
  uictrl base;
  int hit;
  int m_num_pnt;
  double m_xp[32];
  double m_yp[32];
  double m_am[32];
  double m_spline_values[256];
  int8u m_spline_values8[256];
  double m_border_width;
  double m_border_extra;
  double m_curve_width;
  double m_point_size;
  double m_xs1;
  double m_ys1;
  double m_xs2;
  double m_ys2;
  int m_vertex;
  int m_active_pnt;
  int m_move_pnt;
  double m_pdx;
  double m_pdy;
  ColorT m_background_color;
  ColorT m_border_color;
  ColorT m_curve_color;
  ColorT m_inactive_pnt_color;
  ColorT m_active_pnt_color;
} spline_ctrl;
double calc_xp(const spline_ctrl* s, int idx)
{
  return s->m_xs1 + (s->m_xs2 - s->m_xs1) * s->m_xp[idx];
}
double calc_yp(const spline_ctrl* s, int idx)
{
  return s->m_ys1 + (s->m_ys2 - s->m_ys1) * s->m_yp[idx];
}
void update_spline(spline_ctrl* s)
{
  int i;

  for (i = 0; i < s->m_num_pnt; i++) {
    s->m_xp[i] = BOUND(s->m_xp[i], 0, 1);
    s->m_yp[i] = BOUND(s->m_yp[i], 0, 1);

    if (i == 0) {
      s->m_xp[i] = 0.0;
    }
    else if (i == s->m_num_pnt - 1) {
      s->m_xp[i] = 1.0;
    }
    else {
      if (s->m_xp[i] < s->m_xp[i - 1] + 0.001) {
        s->m_xp[i] = s->m_xp[i - 1] + 0.001;
      }

      if (s->m_xp[i] > s->m_xp[i + 1] - 0.001) {
        s->m_xp[i] = s->m_xp[i + 1] - 0.001;
      }
    }
  }

  bspline_set(s->m_num_pnt, s->m_xp, s->m_yp, s->m_am);

  for (i = 0; i < 256; i++) {
    s->m_spline_values[i] = bspline_get(s->m_num_pnt, s->m_xp, s->m_yp, s->m_am, (i) / 255.0);

    if (s->m_spline_values[i] < 0.0) {
      s->m_spline_values[i] = 0.0;
    }

    if (s->m_spline_values[i] > 1.0) {
      s->m_spline_values[i] = 1.0;
    }

    s->m_spline_values8[i] = (int8u)(s->m_spline_values[i] * 255.0);
  }
}
int drawaa_spline_ctrl(const spline_ctrl* s, const event* e)
{
  UIHDC(e);
  int i;
  DPOINT pt[1000];
  int lens[10], n;
  RECT rc = s->base.rc;
  int x1 = rc.left, y1 = rc.top, x2 = rc.right, y2 = rc.bottom;
  double bw, be, bb;
  bw = s->m_border_width;
  be = s->m_border_extra;
  bb = bw + be;

  // Background
  lens[0] = vcgen_rect(pt, x1, y1, x2, y2);
  drawaa_fillpoly_solid(hDC, pt, lens, 1, s->m_background_color);
  // Border
  lens[0] = vcgen_rect2(pt, x1, y1, x2, y2, be, 0);
  lens[1] = vcgen_rect2(pt + lens[0], x1, y1, x2, y2, bb, 1);
  drawaa_fillpoly_solid(hDC, pt, lens, 2, s->m_border_color);
  // Curve
  n = 0;
  pt[n++] = dPOINT(s->m_xs1, s->m_ys1 + (s->m_ys2 - s->m_ys1) * s->m_spline_values[0]);

  for (i = 1; i < 256; i++) {
    pt[n++] = dPOINT(s->m_xs1 + (s->m_xs2 - s->m_xs1) * (i) / 255.0,
        s->m_ys1 + (s->m_ys2 - s->m_ys1) * s->m_spline_values[i]);
  }

  drawaa_strokepoly(hDC, pt, n, 0, s->m_curve_color, s->m_curve_width);

  // Inactive points
  for (i = 0; i < s->m_num_pnt; i++) {
    // Active point
    COLORREF clr = i != s->m_active_pnt ? s->m_inactive_pnt_color : s->m_active_pnt_color;
    lens[0] = vcgen_ellipse(calc_xp(s, i), calc_yp(s, i), s->m_point_size, s->m_point_size, 32, 0, pt);
    drawaa_fillpoly_solid(hDC, pt, lens, 1, clr);
  }

  return 0;
}
int spline_ctrl_event(spline_ctrl* s, const event* e)
{
  UIEVT(e);
  int i;
  RECT rc = s->base.rc;
  int x1 = rc.left, y1 = rc.top, x2 = rc.right, y2 = rc.bottom;
  double bw, be, bb;
  bw = s->m_border_width;
  be = s->m_border_extra;
  bb = bw + be;
  s->m_xs1 = x1 + bb;
  s->m_ys1 = y1 + bb;
  s->m_xs2 = x2 - bb;
  s->m_ys2 = y2 - bb;

  switch (e->msg) {
  case WM_LBUTTONDOWN:
    if (PtInRect(prc, e->pt)) {
      for (i = 0; i < s->m_num_pnt; i++) {
        double xp = calc_xp(s, i);
        double yp = calc_yp(s, i);

        if (calc_distance(e->pt.x, e->pt.y, xp, yp) <= s->m_point_size + 1) {
          s->m_pdx = xp - e->pt.x;
          s->m_pdy = yp - e->pt.y;
          s->m_active_pnt = s->m_move_pnt = (i);
          force_redraw(e);
          return TRUE;
        }
      }

      return FALSE;
    }

    break;

  case WM_LBUTTONUP: {
    s->hit = 0;

    if (s->m_move_pnt >= 0) {
      s->m_move_pnt = -1;
      force_redraw(e);
      return TRUE;
    }

    return FALSE;
  }
  break;

  case WM_MOUSEMOVE: {
    if (s->m_move_pnt >= 0) {
      double xp = e->pt.x + s->m_pdx;
      double yp = e->pt.y + s->m_pdy;
      s->m_xp[s->m_move_pnt] = (s->m_move_pnt, (xp - s->m_xs1) / (s->m_xs2 - s->m_xs1));
      s->m_yp[s->m_move_pnt] = (s->m_move_pnt, (yp - s->m_ys1) / (s->m_ys2 - s->m_ys1));
      update_spline(s);
      force_redraw(e);
      return TRUE;
    }

    return FALSE;
  }
  break;
  }

  return FALSE;
}
int spline_ctrl_set(spline_ctrl* s, int num_pnt, double* pt)
{
  int i;
  UISETCALL(s, drawaa_spline_ctrl, spline_ctrl_event);
  s->hit = 0;
  s->m_num_pnt = (num_pnt);
  s->m_border_width = (1.0);
  s->m_border_extra = (2.0);
  s->m_curve_width = (1.0);
  s->m_point_size = (3.0);
  s->m_vertex = (0);
  s->m_active_pnt = (-1);
  s->m_move_pnt = (-1);
  s->m_pdx = (0.0);
  s->m_pdy = (0.0);

  if (s->m_num_pnt < 4) {
    s->m_num_pnt = 4;
  }

  if (s->m_num_pnt > 32) {
    s->m_num_pnt = 32;
  }

  for (i = 0; i < s->m_num_pnt; i++) {
    s->m_xp[i] = (double)(i) / (s->m_num_pnt - 1);
    s->m_yp[i] = 0.5;
  }

  for (i = 0; i < s->m_num_pnt; i++) {
    s->m_xp[i] = pt[i * 2];
    s->m_yp[i] = pt[i * 2 + 1];
  }

  update_spline(s);
  s->m_background_color = (rgb(1.0, 1.0, 0.9));
  s->m_border_color = (rgb(0.0, 0.0, 0.0));
  s->m_curve_color = (rgb(0.0, 0.0, 0.0));
  s->m_inactive_pnt_color = (rgb(0.0, 0.0, 0.0));
  s->m_active_pnt_color = (rgb(1.0, 0.0, 0.0));
  return 0;
}

