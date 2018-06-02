//#include "geo/geo.inl"
#define xn(i) m_polygon[i].x
#define yn(i) m_polygon[i].y
#if 0
int imdrawaa_bezier(img_t* im, const DPOINT2* m_polygon, int m_num_points, double wl)
{
  DPOINT pt[1000];
  int lens[10];
  int i, n;
  COLOR m_color;
  COLOR m_node_color;
  m_color = (rgba(0.0, 0.0, 0.0, 0.8));
  m_node_color = (rgba(0.0, 0.5, 0.0, 0.5));
  if (4 == s->m_num_points) {
    // Control line 1
    n = vcgen_curve4_div(xn(0), yn(0), (xn(0) + xn(1)) * 0.5, (yn(0) + yn(1)) * 0.5, (xn(0) + xn(1)) * 0.5, (yn(0) + yn(1)) * 0.5, xn(1), yn(1), pt);
    drawaa_strokepoly(hDC, pt, n, 0, m_color, wl);
    // Control line 2
    n = vcgen_curve4_div(xn(2), yn(2), (xn(2) + xn(3)) * 0.5, (yn(2) + yn(3)) * 0.5, (xn(2) + xn(3)) * 0.5, (yn(2) + yn(3)) * 0.5, xn(3), yn(3), pt);
    drawaa_strokepoly(hDC, pt, n, 0, m_color, wl);
    // Curve itself
    n = vcgen_curve4_div(xn(0), yn(0), xn(1), yn(1), xn(2), yn(2), xn(3), yn(3), pt);
    drawaa_strokepoly(hDC, pt, n, 0, m_color, wl);
  }
  else {
    // Control line
    n = vcgen_curve3_div(xn(0), yn(0), (xn(0) + xn(1)) * 0.5, (yn(0) + yn(1)) * 0.5, xn(1), yn(1), pt);
    drawaa_strokepoly(hDC, pt, n, 0, m_color, wl);
    // Control line 2
    n = vcgen_curve3_div(xn(1), yn(1), (xn(1) + xn(2)) * 0.5, (yn(1) + yn(2)) * 0.5, xn(2), yn(2), pt);
    drawaa_strokepoly(hDC, pt, n, 0, m_color, wl);
    // Curve itself
    n = vcgen_curve3_div(xn(0), yn(0), xn(1), yn(1), xn(2), yn(2), pt);
    drawaa_strokepoly(hDC, pt, n, 0, m_color, wl);
  }
  for (i = 0; i < m_num_points; ++i) {
    double r = i == s->hit ? s->m_point_radius * 1.2 : s->m_point_radius;
    lens[0] = vcgen_ellipse(s->m_polygon[i].x, s->m_polygon[i].y, r, r, 20, 0, pt);
    imdrawaa_ellipse(hDC, pt, lens, 1, m_node_color);
  }
  return 0;
}
#endif
int test_aa_demo()
{
  enum {mainwin, mainwin2};
  xdisp_t disp[1] = {0};
  int hit = 0;
  int i;
  xmsg_t msg[1] = {0};
  DPOINT m_pt[30];
  int npt = 8;
  double m_dx = 0;
  double m_dy = 0;
  int m_idx = 0;
  XCTRLADD(disp, layout, lay1);
  XCTRLADD2(disp, button, btn1, (btn1, "按钮1"));
  XCTRLADD2(disp, slider, sli1, (sli1, 0, 100, "滑动条1 %.2f"));
  XCTRLADD(disp, curve, curve1);
  XCTRLADD2(disp, cbox, cb1, (cb1, "复选"));
  XCTRLADD2(disp, gamma, m_spline, (m_spline, 0.786681, 1.239889, 0.726049, 0.710140));
  const char* rb1str2[] = {
    "Random",
    "13---24",
    "Smooth Cusp 1",
    "Smooth Cusp 2",
    "Real Cusp 1",
    "Real Cusp 2",
    "Fancy Stroke",
    "Jaw",
    "Ugly Jaw"
  };
  const char* rb1str[] = {"单选1", "单选2", "单选3", "单选4"};
  XCTRLADD2(disp, rbox, rb1, (rb1, countof(rb1str), rb1str));
  int iid[] = {
    0, 100, layx, 200,
    layx, 0, 0, 0,
    120, id_rb1, id_curve1, 0,
    20, id_cb1, 0, 0,
    20, id_btn1, id_sli1, 0,
  };
  uistyle_default_set(disp->drawer);
  m_idx = -1;
  //RANDPT(npt, m_pt, 100, 400, 100, 400);
  randsamploy(npt, m_pt, 100, 400, 100, 400);
  xlayout_set(lay1, countof(iid) / 4 - 1, 3, iid, 4, 4);
  XCreateWindow(disp, mainwin, "test_aa_demo", 100, 100, 500, 500, id_lay1);
  //XCreateWindow(disp, mainwin2, "test_aa_demo", 100, 100, 500, 500, test_aa_demo_paint, s);
  for (; XWaitMsg(disp, msg, 50, MSG_MASK_KEY | MSG_MASK_MOUSE) >= 0;) {
    xwin_t* win = msg->win;
    if (win && mainwin == win->winid) {
      IRECT crc = xGetClient(win);
      int redraw = 0;
      int x = msg->x - crc.l, y = msg->y - crc.t;
      //IRECT rc = iRECT(100, 100, 200, 200);
      //button_msg_proc(btn, msg, &rc, win->im);
      switch (msg->msg) {
      case MSG_INIT:
        if (1) {
          redraw = 1;
        }
        break;
      case MSG_COMMAND:
        if (1) {
          printf("MSG_COMMAND\n");
        }
        break;
      case MSG_LBUTTONUP:
        if (1) {
          m_idx = -1;
          //polygon_save(m_polygon, TEST_AA_DEMO_POLYGON_FN);
          //msg->redraw = 1;
        }
        break;
      case MSG_LBUTTONDOWN:
        if (1) {
          for (i = 0; i < npt; i++) {
            if (calc_distance(x, y, m_pt[i].x, m_pt[i].y) < 10.0) {
              m_dx = x - m_pt[i].x;
              m_dy = y - m_pt[i].y;
              m_idx = i;
              break;
            }
          }
          if (i == npt) {
            if (point_in_poly(m_pt, npt, x, y)) {
              m_dx = x - m_pt[0].x;
              m_dy = y - m_pt[0].y;
              m_idx = countof(m_pt);
            }
          }
          redraw = 1;
        }
        break;
      case MSG_MOUSEMOVE:
        if (1) {
          if (m_idx == countof(m_pt)) {
            double dx = x - m_dx;
            double dy = y - m_dy;
            poly_offset(m_pt, npt, dx - m_pt[0].x, dy - m_pt[0].y);
            redraw = 1;
          }
          else if (m_idx >= 0) {
            m_pt[m_idx].x = x - m_dx;
            m_pt[m_idx].y = y - m_dy;
            redraw = 1;
          }
        }
        break;
      }
      if (redraw) {
        int i, j;
        img_t im[1] = {0};
        //img_t im2[1] = {0};
        int gamma[256];
        imsubref(win->im, xGetClient(win), im);
        imdraw_fill(im, rgb(1, 1, 1));
        //gamma_pow(gamma, m_slider2->m_value);
        printf("redraw\n");
        //imsubref(im, iRECT(100, 100, 200, 200), im2);
        //imdrawui_win2000_button(im2, btn);
        //imdraw_taichi(im, 300, 300, 150);
        if (1) {
          int size_mul = 20;//(int)(m_slider1->m_value);
          DPOINT pt[countof(m_pt)];
          int len = npt;
          for (i = 0; i < npt; ++i) {
            pt[i].x = (m_pt[i].x) / size_mul;
            pt[i].y = (m_pt[i].y) / size_mul;
          }
          //printf("%f\n", m_pt[0].y);
          //imdrawaa_poly_enlarge(im, pt, &len, 1, _RGBA(0, 0, 0, 100), size_mul);
          imdrawaa_poly_solid(im, pt, &len, 1, rgb(1, 0, 0));
          imdrawaa_spline(im, m_pt, len, 1, rgba(1, 0, 0, 0.2), 0, 0);
        }
        gamma_none(gamma);
        if (1) {
          for (i = 0; i < 20; ++i) {
            imdraw_circle(im, 50 + i * i + i * 2, 200, i, rgb(0, 0, 1), rgb(1, 0, 0), 1 + (int)sli1->m_value);
          }
        }
        if (1) {
          for (j = 0; j < npt; ++j) {
            int k = (j + 1) % npt;
            imdrawaa_line(im, m_pt[j].x, m_pt[j].y, m_pt[k].x, m_pt[k].y, _RGBA(0, 150, 160, 200), 2);
          }
        }
        if (1) {
          for (j = 0; j < npt; ++j) {
            imdrawaa_ellipse(im, m_pt[j].x, m_pt[j].y, 5, 5, rgba(0, 0.3, 0.5, 0.6), 0, 0);
          }
        }
        if (0) {
          int x = 100, y = 100, d = 50;
          COLORREF clr = rgb(1, 0, 0);
          int r = 10;//(int)(m_slider2->m_value * 10);
          //imdraw_circle(im, x, y, r - r / 6, clr, r / 3), x += d;
          imdrawaa_ellipse(im, x, y, r - r / 6, r - r / 6, 0, clr, r / 3), x += d;
          //imdrawaa_ellipse(im, x, y, r, r, clr, 0, 0), x += d;
        }
        //draw_polygon_ctrl(m_polygon, e);
        xEndPaint(win);
      }
    }
  }
  return 0;
}

