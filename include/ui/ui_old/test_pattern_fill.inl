
int create_star(DPOINT* pt, int n, double xc, double yc, const double* r1, const double* r2, int nr, double start_angle)
{
  int i;
  start_angle *= PI / 180.0;
  n *= nr;

  for (i = 0; i < n; i++) {
    double a = PI * 2.0 * i / n + start_angle;
    pt[i] = dPOINT(xc + cos(a) * r1[i % nr], yc + sin(a) * r2[i % nr]);
  }

  return n;
}


int test_pattern_fill()
{
  xdisp_t disp[1] = {0};
  xmsg_t msg[1] = {0};
  double m_polygon_cx = 300, m_polygon_cy = 300;
  double m_dx, m_dy;
  DPOINT pt[100];
  int m_flag = 0;
  enum {mainwin, menu1};

  XCTRLADD(disp, layout, lay1);
  XCTRLADD2(disp, slider, m_polygon_angle, (m_polygon_angle, -180.0, 180.0, "Polygon Angle=%3.2f"));
  XCTRLADD2(disp, slider, m_polygon_scale, (m_polygon_scale, 0.1, 5.0, "Polygon Scale=%3.2f"));
  XCTRLADD2(disp, slider, m_pattern_angle, (m_pattern_angle, -180.0, 180.0, "Pattern Angle=%3.2f"));
  XCTRLADD2(disp, slider, m_pattern_size, (m_pattern_size, 10, 40, "Pattern Size=%3.2f"));
  XCTRLADD2(disp, slider, m_pattern_alpha, (m_pattern_alpha, 0, 1, "Background Alpha=%.2f"));
  XCTRLADD2(disp, null, nul, (nul));

  int iid[] = {
    0,                     layx,                       layx,                      layx,
    layx,                id_nul,                       layh,                      layh,
    20,      id_m_polygon_angle,         id_m_polygon_scale,                      0,
    20,      id_m_pattern_angle,          id_m_pattern_size,                      0,
    20,      id_m_pattern_alpha,                       layh,                      layh,
  };

  uistyle_default_set(disp->drawer);
  disp->drawer->style = UIS_WIN2K;
  xlayout_set(lay1, countof(iid) / 4 - 1, 4 - 1, iid, 4, 4);

  XCreateWindow(disp, mainwin, "test_pattern_fill", 100, 100, 800, 600, id_lay1, WS_CAPTION | WS_BORDER);

  for (; XWaitMsg(disp, msg, 50) >= 0;) {
    xwin_t* win = msg->win;

    if (!win) {
      continue;
    }

    if (menu1 == win->winid) {
      if (MSG_COMMAND == msg->msg && XCMD_CLOSE == msg->key) {
        ShowWindow(win->hwnd, 0);
      }

      if (msg->redraw) {
        xEndPaint(win);
      }
    }

    if (mainwin == win->winid) {
      IRECT crc = disp->ci[id_nul].rc;
      int x = msg->x - crc.l, y = msg->y - crc.t;

      double tt = m_polygon_scale->m_value * 100 + 8.0;
      double r1[] = {tt, tt / 1.45, tt / 2, tt / 1.45};
      double r2[] = {tt, tt / 1.45, tt / 2, tt / 1.45};
      int len;

      len = create_star(pt, 5, m_polygon_cx, m_polygon_cy, r1, r2, countof(r1), m_polygon_angle->m_value);

      if (MSG_COMMAND == msg->msg && XCMD_CLOSE == msg->key) {
        break;
      }

      switch (msg->msg) {
      case MSG_INIT:
        if (1) {
          msg->redraw = 1;
          SetTimer(win->hwnd, 1, 50, 0);
        }

        break;

      case MSG_TIMER:
        if (1) {
          //msg->redraw = 1;
        }

        break;

      case MSG_EXIT:
        if (1) {
        }

        break;

      case MSG_COMMAND:
        if (1) {
          if (XCMD_CLOSE == msg->key) {
          }
        }

        break;

      case MSG_LBUTTONUP:
        if (1) {
          m_flag = 0;
        }

        break;

      case MSG_RBUTTONDOWN:
        if (iPtInRect(&crc, msg->x, msg->y)) {
          IRECT rc = xwin_rect(win);
          XCreateWindow(disp, menu1, "123", msg->x + rc.l, msg->y + rc.t, 300, 300, -1, WS_CAPTION | WS_BORDER);
        }

        break;

      case MSG_LBUTTONDOWN:
        if (iPtInRect(&crc, msg->x, msg->y)) {
          if (point_in_polygon(pt, len, x, y)) {
            m_dx = x - m_polygon_cx;
            m_dy = y - m_polygon_cy;
            m_flag = 1;
          }
        }

        break;

      case MSG_MOUSEMOVE:
        if (1) {
          if (m_flag) {
            m_polygon_cx = x - m_dx;
            m_polygon_cy = y - m_dy;
            msg->redraw = 1;
          }
        }

        break;
      }

      if (msg->redraw) {
        int n_drawn = 0;
        img_t im[1] = {0};
        IRECT rcClient = crc;
        //UIGetWorkArea(ui, &rcClient);
        imsubref(win->im, rcClient, im);
        imdraw_fill(im, rgb(1, 1, 1));

        if (1) {
          img_t tex[1] = {0};
          xbrush_t br[1] = {0};
          {
            DPOINT pt2[100];
            unsigned size = (unsigned)(m_pattern_size->m_value);
            double alpha = m_pattern_alpha->m_value;
            double s = m_pattern_size->m_value;
            double rr1[] = {s / 2.5, s / 6, s / 4, s / 6};
            double rr2[] = {s / 2.5, s / 6, s / 4, s / 6};
            int len2 = create_star(pt2, 3, s / 2.0, s / 2.0, rr1, rr2, countof(rr1), m_pattern_angle->m_value);
            imsetsize(tex, size, size, 4, 1);
            imdraw_fill(tex, rgba(0.4 * alpha, 0.0 * alpha, 0.1 * alpha, alpha));
            imdrawaa_poly(tex, pt2, len2, 1, _RGB(110, 130, 50), _RGB(0, 50, 80), m_pattern_size->m_value / 15.0);
          }
          {
            xbrush_set_texture(br, wrap_mode_reflect, wrap_mode_reflect, tex->h, tex->w, tex->tt.data, tex->s, tex->c);
            {
              utime_start(_start_time);
              imdrawaa_poly_brush(im, pt, &len, 1, br);
              printf("%f\n", utime_elapsed(_start_time));
            }
          }
          imfree(tex);
        }

        xEndPaint(win);
      }
    }
  }

  return 0;
}

