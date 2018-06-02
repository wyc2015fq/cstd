

#include "wykobi/math.inl"
#include "wykobi/base.inl"

#include "wykobi/matrix.inl"
#include "wykobi/hull.inl"
#include "wykobi/minimum_bounding_ball.inl"
#include "wykobi/clipping.inl"
#include "wykobi/ordered_polygon.inl"
#include "wykobi/duplicates.inl"
#include "wykobi/naive_group_intersections.inl"
#include "wykobi/axis_projection_descriptor.inl"
#include "wykobi/earclipping.inl"
#include "wykobi/test_unit.inl"

int test_wykobi()
{
  enum {mainwin};
  xdisp_t disp[1] = {0};
  xmsg_t msg[1] = {0};

  enum {
#define TEST_WYKOBIDEF(x) id_##x,
#include "test_wykobidef.txt"
#undef TEST_WYKOBIDEF
  };
  const char* rb1str[] = {
#define TEST_WYKOBIDEF(x) #x ,
#include "test_wykobidef.txt"
#undef TEST_WYKOBIDEF
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
        IRECT rcClient = disp->ci[id_nul].rc;
        //UIGetWorkArea(ui, &rcClient);
        imsubref(win->im, rcClient, im);
        imdraw_fill(im, rgb(1, 1, 1));
        //rb1->hit=2;
        //srand(0);
        {
#define TEST_WYKOBIDEF(x) if (id_##x ==rb1->hit) { test_##x (im); }
#include "test_wykobidef.txt"
#undef TEST_WYKOBIDEF
        }
        //imdrawui_default_polygon(uidrawer, im, m_poly1);
        //imdrawui_default_polygon(uidrawer, im, m_poly2);
        xEndPaint(win);
      }
    }
  }

  return 0;
}

