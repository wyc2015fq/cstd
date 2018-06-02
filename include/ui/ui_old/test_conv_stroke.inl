
int test_conv_stroke()
{
  enum {mainwin, mainwin2};
  xdisp_t disp[1] = {0};
  int hit = 0;
  xmsg_t msg[1] = {0};
  XCTRLADD(disp, layout, lay1);
  XCTRLADD(disp, slider, m_width);
  XCTRLADD(disp, slider, m_miter_limit);
  XCTRLADD(disp, rbox, m_line_cap);
  XCTRLADD(disp, rbox, m_inner_join);
  XCTRLADD(disp, rbox, m_line_join);

  int iid[] = {
    0,             100,       layx,
    layx,          0,          0,
    140,           6,          0,
    120,           5,          0,
    40,            4,          0,
    20,         layv,          2,
    20,         layv,          3,
  };

  DPOINT m_pt[3];
  double m_dx = 0;
  double m_dy = 0;
  int m_idx = 0;
  const char* m_width_str[] = {"Round Cap", "Butt Cap", "Square Cap"};
  const char* m_inner_join_str[] = {"Inner Round", "Inner Bevel", "Inner Miter", "Inner Jag"};
  const char* m_line_join_str[] = {"Round Join", "Miter Join", "Miter Revert", "Bevel Join", "Miter Round"};

  uistyle_default_set(disp->drawer);

  xrbox_set(m_line_cap, countof(m_width_str), m_width_str);
  xrbox_set(m_inner_join, countof(m_inner_join_str), m_inner_join_str);
  xrbox_set(m_line_join, countof(m_line_join_str), m_line_join_str);

  xslider_set(m_width, 3, 40, "Width=%1.2f");
  xslider_set(m_miter_limit, 1, 10, "Miter Limit=%1.2f");
  xlayout_set(lay1, countof(iid) / 3 - 1, 3 - 1, iid, 4, 4);

  V3SET(m_pt, dPOINT(57 + 100, 60), dPOINT(369 + 100, 170), dPOINT(143 + 100, 310));
  m_idx = -1;

  XCreateWindow(disp, mainwin, "AGG Example. Line Join", 100, 100, 500, 500, id_lay1);

  for (; XWaitMsg(disp, msg, 50, MSG_MASK_KEY | MSG_MASK_MOUSE) >= 0;) {
    xwin_t* win = msg->win;

    if (win && mainwin == win->winid) {
      IRECT crc = win->fr->rcs[HITCLIENT].rc;
      int x = msg->x - crc.l, y = msg->y - crc.t;
      win = msg->win;

      switch (msg->msg) {
      case MSG_INIT:
        if (1) {
        }

        break;

      case MSG_LBUTTONUP:
        if (1) {
          m_idx = -1;
          msg->redraw = 1;
        }

        break;

      case MSG_LBUTTONDOWN:
        if (1) {
          int i;

          for (i = 0; i < 3; i++) {
            if (calc_distance(x, y, m_pt[i].x, m_pt[i].y) < 20.0) {
              m_dx = x - m_pt[i].x;
              m_dy = y - m_pt[i].y;
              m_idx = i;
              break;
            }
          }

          if (i == 3) {
            if (point_in_triangle(m_pt[0].x, m_pt[0].y, m_pt[1].x, m_pt[1].y, m_pt[2].x, m_pt[2].y, x, y)) {
              m_dx = x - m_pt[0].x;
              m_dy = y - m_pt[0].y;
              m_idx = 3;
            }
          }

          msg->redraw = 1;
        }

        break;

      case MSG_MOUSEMOVE:
        if (m_idx >= 0) {
          if (m_idx == 3) {
            double dx = x - m_dx;
            double dy = y - m_dy;
            //point_offset(m_pt, 3, m_pt[0].x - dx, m_pt[0].y - dy)
            m_pt[1].x -= m_pt[0].x - dx;
            m_pt[1].y -= m_pt[0].y - dy;
            m_pt[2].x -= m_pt[0].x - dx;
            m_pt[2].y -= m_pt[0].y - dy;
            m_pt[0].x = dx;
            m_pt[0].y = dy;
            msg->redraw = 1;
          }
          else if (m_idx >= 0) {
            m_pt[m_idx].x = x - m_dx;
            m_pt[m_idx].y = y - m_dy;
            msg->redraw = 1;
          }
        }

        break;
      }

      if (msg->redraw) {
        int i, j;
        img_t im[1] = {0};
        IRECT rcClient = xGetClient(win);
        //UIGetWorkArea(ui, &rcClient);
        imsubref(win->im, rcClient, im);

        if (1) {
          imdraw_fill(im, rgb(1, 1, 1));
          {
            DPOINT pt1[1000];
            DPOINT pt[8] = {
              m_pt[0].x, m_pt[0].y,
              //(m_pt[0].x + m_pt[1].x) / 2, (m_pt[0].y + m_pt[1].y) / 2, // This point is added only to check for numerical stability
              m_pt[1].x, m_pt[1].y,
              m_pt[2].x, m_pt[2].y, // This point is added only to check for numerical stability
              (m_pt[0].x + m_pt[1].x) / 2, (m_pt[0].y + m_pt[1].y) / 2,
              (m_pt[1].x + m_pt[2].x) / 2, (m_pt[1].y + m_pt[2].y) / 2,
              (m_pt[2].x + m_pt[0].x) / 2, (m_pt[2].y + m_pt[0].y) / 2,
            };
            int lens[2] = {3, 3};
            int lens1[2] = {3, 3};
            imdraw_fill(im, rgb(1, 1, 1));

            // (1)
            //stroke.miter_limit(m_miter_limit.value());
            for (j = 0, i = 0; i < 1; ++i) {
              double da[] = {20, 10};
              //drawaa_dashpoly(im, pt, lens[i], 1, rgb(0,0,0.3), m_width->m_value/5., 0, da, 2);
            }

            for (j = 0, i = 0; i < 2; ++i) {
              int n, k = 0, m = 0;
              n = vcgen_stroke(pt + j, lens[i], m_width->m_value, i,
                  (line_cap_e)(m_line_cap->hit),
                  (line_join_e)(m_line_join->hit),
                  (inner_join_e)(m_inner_join->hit),
                  countof(pt1), pt1, lens1);
              imdrawaa_poly_solid(im, pt1, lens1, n, rgb(0.8, 0.7, 0.6));
              j += lens[i];
            }

            for (j = 0, i = 0; i < 2; ++i) {
              int n, k, m = 0;
              n = vcgen_stroke(pt + j, lens[i], m_width->m_value, i,
                  (line_cap_e)(m_line_cap->hit),
                  (line_join_e)(m_line_join->hit),
                  (inner_join_e)(m_inner_join->hit),
                  countof(pt1), pt1, lens1);

              for (k = 0; k < n; ++k) {
                double da[] = {20.0, m_width->m_value / 2.5};
                imdrawaa_dashpoly(im, pt1 + m, lens1[k], 1, rgb(0, 0, 0.3), m_width->m_value / 5., 0, da, 2);
                m += lens1[k];
              }

              imdrawaa_poly(im, pt + j, lens[i], i, 0, rgb(0, 0, 0), 1.5);
              j += lens[i];
            }

            for (j = 0, i = 0; i < 2; ++i) {
              imdrawaa_poly_solid(im, pt + j, lens + i, 1, rgba(0.8, 0.7, 0.9, 0.5));
              j += lens[i];
            }
          }
        }

        xEndPaint(win);
      }
    }
  }

  return 0;
}

