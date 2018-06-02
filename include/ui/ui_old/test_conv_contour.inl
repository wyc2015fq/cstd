
#include "get_poly.inl"
int test_conv_contour()
{
  enum {mainwin};
  xdisp_t disp[1] = {0};
  xmsg_t msg[1] = {0};

  XCTRLADD(disp, layout, lay1);
  XCTRLADD2(disp, slider, m_width, (m_width, -100, 100, "Width=%1.2f"));
  XCTRLADD2(disp, cbox, m_auto_detect, (m_auto_detect, "Autodetect orientation if not defined"));
  const char* m_close_str[] = {"Close", "Close CW", "Close CCW"};
  XCTRLADD2(disp, rbox, m_close, (m_close, countof(m_close_str), m_close_str));

  int iid[] = {
    0,                200,             layx,             layx,
    layx,               0,                0,                0,
    20,        id_m_close,                0,             layh,
    20,              layv, id_m_auto_detect,             layh,
    20,              layv,       id_m_width,             layh,
  };

  uistyle_default_set(disp->drawer);
  xlayout_set(lay1, countof(iid) / 4 - 1, 4 - 1, iid, 4, 4);

  XCreateWindow(disp, mainwin, "AGG Example. Contour Tool & Polygon Orientation", 100, 100, 800, 600, id_lay1);

  for (; XWaitMsg(disp, msg, 50, MSG_MASK_KEY | MSG_MASK_MOUSE) >= 0;) {
    xwin_t* win = msg->win;

    if (win && mainwin == win->winid) {
      IRECT crc = win->fr->rcs[HITCLIENT].rc;
      int x = msg->x - crc.l, y = msg->y - crc.t;
      win = msg->win;

      if (MSG_COMMAND == msg->msg && XCMD_CLOSE == msg->key) {
        break;
      }

      switch (msg->msg) {
      case MSG_INIT:
        if (1) {
        }

        break;

      case MSG_EXIT:
        if (1) {
        }

        break;

      case MSG_LBUTTONUP:
        if (1) {
        }

        break;

      case MSG_COMMAND:
        if (1) {
          if (XCMD_CLOSE == msg->key) {
          }
        }

        break;

      case MSG_LBUTTONDOWN:
        if (1) {
        }

        break;

      case MSG_MOUSEMOVE:
        if (1) {
        }

        break;
      }

      if (msg->redraw) {
        int n_drawn = 0;
        img_t im[1] = {0};
        IRECT rcClient = xGetClient(win);
        //UIGetWorkArea(ui, &rcClient);
        imsubref(win->im, rcClient, im);
        imdraw_fill(im, rgb(1, 1, 1));

        {
          poly2d_t p1[1] = {0};
          get_poly(gp_a, p1);
          imdrawaa_poly2d_solid(im, p1, rgb(0, 0, 0), 0, 0);
          poly2d_free(p1);
        }
        //imdrawui_default_polygon(uidrawer, im, m_poly1);
        //imdrawui_default_polygon(uidrawer, im, m_poly2);
        xEndPaint(win);
      }
    }
  }

  return 0;
}

