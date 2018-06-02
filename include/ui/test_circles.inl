enum default_num_points_e { default_num_points = 1000 };
enum start_size_e {
  start_width = 400,
  start_height = 400
};
static double spline_r_x[] = { 0.000000, 0.200000, 0.400000, 0.910484, 0.957258, 1.000000 };
static double spline_r_y[] = { 1.000000, 0.800000, 0.600000, 0.066667, 0.169697, 0.600000 };
static double spline_r_z[] = { 1.000000, 0.800000, 0.600000, 0.066667, 0.169697, 0.600000 };
static double spline_g_x[] = { 0.000000, 0.292244, 0.485655, 0.564859, 0.795607, 1.000000 };
static double spline_g_y[] = { 0.000000, 0.607260, 0.964065, 0.892558, 0.435571, 0.000000 };
static double spline_g_z[] = { 0.000000, 0.607260, 0.964065, 0.892558, 0.435571, 0.000000 };
static double spline_b_x[] = { 0.000000, 0.055045, 0.143034, 0.433082, 0.764859, 1.000000 };
static double spline_b_y[] = { 0.385480, 0.128493, 0.021416, 0.271507, 0.713974, 1.000000 };
static double spline_b_z[] = { 0.385480, 0.128493, 0.021416, 0.271507, 0.713974, 1.000000 };
typedef struct scatter_point {
  double x;
  double y;
  double z;
  COLORREF color;
} scatter_point;
CC_INLINE double random_dbl(double start, double end)
{
  unsigned r = rand() & 0x7FFF;
  return (double)(r) * (end - start) / 32768.0 + start;
}
#if 0
virtual void on_idle()
{
  unsigned i;
  for (i = 0; i < m_num_points; i++) {
    m_points[i].x += random_dbl(0, s->m_sel->m_value) - s->m_sel->m_value * 0.5;
    m_points[i].y += random_dbl(0, s->m_sel->m_value) - s->m_sel->m_value * 0.5;
    m_points[i].z += random_dbl(0, s->m_sel->m_value * 0.01) - s->m_sel->m_value * 0.005;
    if (m_points[i].z < 0.0) {
      m_points[i].z = 0.0;
    }
    if (m_points[i].z > 1.0) {
      m_points[i].z = 1.0;
    }
  }
  force_redraw(c);
}
#endif
typedef struct test_circles_t {
  ctrl_t c[1];
  scale_t m_z[1];
  slider_t m_sel[1];
  slider_t m_size[1];
  layout_t lay1[1];
  int m_num_points;
  scatter_point m_points[default_num_points];
} test_circles_t;
static LRESULT test_circles_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  test_circles_t* s = (test_circles_t*)c->x;
  //IRECT rc = c->rc;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  scatter_point* m_points;
  m_points = s->m_points;
  switch (uMsg) {
  case WM_CTRLINIT:
    if (s) {
      const char* lay =
          " xxx;"
          "x ;"
          "a %-;"
          "a %-;"
          "a %-;"
          ;
      slider_set(s->m_sel, c, "Selectivity=%.2f", 0, 5, 2);
      slider_set(s->m_size, c, "Size=%.2f", 0, 5, 2);
      scale_set(s->m_z, c, c);
      layout_set(s->lay1, c, 4, 0, lay, s->m_sel, s->m_size, s->m_z);
      s->m_num_points = default_num_points;
      bspline_set(6, spline_r_x, spline_r_y, spline_r_z);
      bspline_set(6, spline_g_x, spline_g_y, spline_g_z);
      bspline_set(6, spline_b_x, spline_b_y, spline_b_z);
      //ctrl_addsub_tail(c, s->lay1->c);
      return 0;
    }
    break;
  case WM_COMMAND:
    break;
  case WM_GETMINMAXINFO:
    if (1) {
      MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
      if (c->sub) {
        c->sub->ctrlproc(c, c->sub, uMsg, wParam, lParam);
      }
      mminfo->ptMinTrackSize.x = 600;
      mminfo->ptMinTrackSize.y = 400;
      //printf("WM_MOUSEMOVE\n");
    }
    break;
  case WM_TIMER:
    if (s) {
      force_redraw(c);
    }
    break;
  case WM_PAINT:
    if (c->recalcsize) {
      ctrl_setrect(c->sub, c->rc);
    }
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      int i;
      int n_drawn = 0;
      IRECT rc = c->rc;
      IRECT rcclip = c->rcclip;
      iRectOff0(&rcclip);
      iRectOff0(&rc);
      //s->lay1->c->isshow = 0;
      imdraw_fill(im, _rgb(1, 1, 1));
      {
        double rx = (im)->w / 3.5;
        double ry = (im)->h / 3.5;
        for (i = 0; i < s->m_num_points; i++) {
          double z = s->m_points[i].z = random_dbl(0.0, 1.0);
          double x = cos(z * 2.0 * PI) * rx;
          double y = sin(z * 2.0 * PI) * ry;
          double r0, g0, b0;
          double dist = random_dbl(0.0, rx / 2.0);
          double angle = random_dbl(0.0, PI * 2.0);
          m_points[i].x = (im)->w / 2.0 + x + cos(angle) * dist;
          m_points[i].y = (im)->h / 2.0 + y + sin(angle) * dist;
          r0 = bspline_get(6, spline_r_x, spline_r_y, spline_r_z, z) * 0.8;
          g0 = bspline_get(6, spline_g_x, spline_g_y, spline_g_z, z) * 0.8;
          b0 = bspline_get(6, spline_b_x, spline_b_y, spline_b_z, z) * 0.8;
          s->m_points[i].color = _rgba(r0, g0, b0, 1.0);
        }
      }
      if (0) {
        double t;
        utime_start(_start_time);
        for (i = 0; i < s->m_num_points; i++) {
          double z = s->m_points[i].z;
          double alpha = 1.0;
          if (z < s->m_z->m_value1) {
            alpha = 1.0 - (s->m_z->m_value1 - z) * s->m_sel->m_value * 100.0;
          }
          if (z > s->m_z->m_value2) {
            alpha = 1.0 - (z - s->m_z->m_value2) * s->m_sel->m_value * 100.0;
          }
          alpha = BOUND(alpha, 0, 1.);
          if (alpha > 0.0) {
            double r = s->m_size->m_value * 5.0;
            COLORREF clr = m_points[i].color;
            BYTE _a = (int)(alpha * 255);
            clr = _RGB_A(clr, _a);
#if 1
            imdrawaa_ellipse(im, &c->rc, m_points[i].x, m_points[i].y, r, r, clr, 0, 0);
#else
            imdraw_circle(im, m_points[i].x, m_points[i].y, r, clr, 0, 0);
#endif
            n_drawn++;
          }
        }
        t = utime_elapsed(_start_time);
        {
          char buf[256];
          COLOR m_text_color = _rgb(0, 0, 0);
          IRECT rc = iRECT(0, 0, 1000, 10000);
          _snprintf(buf, 256, "%u time=%.4f", n_drawn, t);
          imdraw_text_line_str(im, 0, rc, 50, 50, buf, -1, 3, c->fo, m_text_color, 0);
        }
        printf("%f\n", utime_elapsed(_start_time));
      }
      if (1) {
        test_imdrawchart(im, 0, c->fo);
      }
      c->redraw = 0;
    }
    break;
  }
  return 0;
}

