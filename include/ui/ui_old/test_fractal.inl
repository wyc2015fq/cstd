//#include "bessel.inl"

#include "draw/fractal/imdraw_fractal.inl"

int test_cantor(img_t* im)
{
  return 0;
}

int test_koch(img_t* im)
{
  return 0;
}

#define DRAWLINES(pt, len, clr, isclose, DRAWLINE ) {int i; \
    for (i = 0; i < len - 1; i++) { DRAWLINE(im, pt[i].x, pt[i].y, pt[i + 1].x, pt[i + 1].y, clr); } \
    if (isclose) { DRAWLINE(im, pt[len - 1].x, pt[len - 1].y, pt[0].x, pt[0].y, clr); } }

int test_fractal()
{
  enum {mainwin};
  xdisp_t disp[1] = {0};
  xmsg_t msg[1] = {0};

  //rbox_ctrl m_trans_type[1];
  //tab_ctrl m_tab[1];
  //slider_ctrl m_sss[1];
  //layout_ctrl m_lay[1];
  //polygon_ctrl m_poly[1];
  //combobox_ctrl m_combo[1];

  const char* rb1str[] = {
#define TEST_FRACTALDEF(x, a) #a ,
#include "test_fractaldef.txt"
#undef TEST_FRACTALDEF
  };

  XCTRLADD(disp, layout, lay1);
  XCTRLADD2(disp, slider, m_alpha, (m_alpha, 0, 255, "Alpha=%1.0f"));
  XCTRLADD2(disp, rbox, rb1, (rb1, countof(rb1str), rb1str));
  XCTRLADD2(disp, null, nul, (nul));

  int iid[] = {
    0,                300,             layx,             layx,
    layx,          id_rb1,           id_nul,             layh,
    //20,        id_m_alpha,             layh,             layh,
  };

  uistyle_default_set(disp->drawer);
  xlayout_set(lay1, countof(iid) / 4 - 1, 4 - 1, iid, 4, 4);
  XCreateWindow(disp, mainwin, "test_wykobi", 100, 100, 1200, 800, id_lay1);

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
          // SetTimer(ui->hwnd, 11, 30, 0);
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
        IRECT rcClient = disp->di[id_nul].rc;
        DPOINT pts[100] = {0};
        DPOINT pt[100] = {0};
        int len = 4, n;
        //rand_dpt();
        //UIGetWorkArea(ui, &rcClient);
        imsubref(win->im, rcClient, im);
        randsamploy(len, pt, 10, im->w, 10, im->h);
        //imdraw_fill(im, rgb(1, 1, 1));

        if (1) {
          enum {
#define TEST_FRACTALDEF(x, a) id_##x,
#include "test_fractaldef.txt"
#undef TEST_FRACTALDEF
          };
          //drawaa_listbox_ctrl(hDC, m_listbox);
          //rcClient = m_tab->rcClient;

#define TEST_FRACTALDEF(x, a) if (id_##x ==rb1->hit) { test_##x (im); }
          //#include "test_fractaldef.txt"
#undef TEST_FRACTALDEF

          switch (rb1->hit) {
          case id_cantor:
            DRAWLINES(pt, len, rgb(0, 0, 0), 1, imdraw_fractal_canto);
            break;

          case id_koch:
            n = vcgen_fractal_koch(pts, countof(pts), pt[0].x, pt[0].y, pt[1].x, pt[1].y);
            imdrawaa_poly(im, pts, n, 0, 0, rgb(1, 0, 0), 3);
            break;

          case id_arboresent:
            imdraw_fractal_arboresent(im, pt[0].x, pt[0].y, pt[1].x, pt[1].y, rgb(0, 0, 0));
            break;

          case id_sierpinski:
            imdraw_fractal_sierpinski(im, pt[0], pt[1], pt[2], rgb(0, 0, 0));
            break;

          case id_sierpinski_sier:
            //DRAWLINES(pt, len, rgb(0, 0, 0), 1, imdraw_fractal_sierpinski_sier);
            imdraw_fractal_sierpinski_sier(im, pt[0].x, pt[0].y, pt[1].x, pt[1].y, rgb(0, 0, 0));
            break;

          case id_perspective:
            //imdraw_bessel(hDC);
            break;

          case id_leaf:
            imdraw_fractal_leaf(im, pt[0].x, pt[0].y, rgb(0, 0, 0));
            break;

          case id_rose:
            imdraw_fractal_rose(im, pt[0].x, pt[0].y, rgb(0, 0, 0));
            break;

          case id_couch:
            //imdraw_fractal_couch(im, pt[0].x, pt[0].y, pt[1].x, pt[1].y, 5, rgb(0, 0, 0));
            //imdraw_fractal_sierpinski_sier(im, pt[0].x, pt[0].y, pt[1].x, pt[1].y, 5, rgb(0, 0, 0));
            break;
          };
        }

        //imdrawui_default_polygon(uidrawer, im, m_poly1);
        //imdrawui_default_polygon(uidrawer, im, m_poly2);
        xEndPaint(win);
      }
    }
  }

  return 0;
}

