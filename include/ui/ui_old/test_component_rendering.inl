
int test_component_rendering()
{
  enum {mainwin};
  xdisp_t disp[1] = {0};
  xmsg_t msg[1] = {0};

  XCTRLADD(disp, layout, lay1);
  XCTRLADD2(disp, slider, m_alpha, (m_alpha, 0, 255, "Alpha=%1.0f"));
  XCTRLADD2(disp, null, nul, (nul));

  int iid[] = {
    0,               layx,             layx,             layx,
    layx,          id_nul,             layh,             layh,
    20,        id_m_alpha,             layh,             layh,
  };

  uistyle_default_set(disp->drawer);
  xlayout_set(lay1, countof(iid) / 4 - 1, 4 - 1, iid, 4, 4);

  XCreateWindow(disp, mainwin, "AGG gradients with Mach bands compensation", 100, 100, 800, 600, id_lay1);

  for (; XWaitMsg(disp, msg, 50, MSG_MASK_KEY | MSG_MASK_MOUSE) >= 0;) {
    xwin_t* win = msg->win;

    if (win && mainwin == win->winid) {
      IRECT crc = disp->ci[id_nul].rc;
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
        IRECT rcClient = crc;
        //UIGetWorkArea(ui, &rcClient);
        imsubref(win->im, rcClient, im);
        imdraw_fill(im, rgb(1, 1, 1));

        if (1) {
          DPOINT pt[256];
          xbrush_t br[1] = {0};
          int n, h = im->h, w = im->w, alpha = (int)m_alpha->m_value;

          xbrush_set_solid(br, rgb(0, 0, 0));

          n = vcgen_ellipse(pt, countof(pt), 0, w / 2 - 0.87 * 50, h / 2 - 0.5 * 50, 100, 100);
          br->alpha_mask = _RGBA(alpha, 0, 0, 0);
          imdrawaa_poly_brush(im, pt, &n, 1, br);

          n = vcgen_ellipse(pt, countof(pt), 0, w / 2 + 0.87 * 50, h / 2 - 0.5 * 50, 100, 100);
          br->alpha_mask = _RGBA(0, alpha, 0, 0);
          imdrawaa_poly_brush(im, pt, &n, 1, br);

          n = vcgen_ellipse(pt, countof(pt), 0, w / 2, h / 2 + 50, 100, 100);
          br->alpha_mask = _RGBA(0, 0, alpha, 0);
          imdrawaa_poly_brush(im, pt, &n, 1, br);
        }

        //imdrawui_default_polygon(uidrawer, im, m_poly1);
        //imdrawui_default_polygon(uidrawer, im, m_poly2);
        xEndPaint(win);
      }
    }
  }

  return 0;
}

