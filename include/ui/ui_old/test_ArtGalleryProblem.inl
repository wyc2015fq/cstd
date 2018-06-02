
#include "test_imdrawrect.inl"
//#include "ArtGalleryProblem.inl"

int test_ArtGalleryProblem()
{
  enum {mainwin};
  xdisp_t disp[1] = {0};
  xmsg_t msg[1] = {0};

  XCTRLADD2(disp, slider, sli1, (sli1, 0, 25, "Alpha=%.2f"));
  XCTRLADD2(disp, button, btn1, (btn1, "btn1"));
  XCTRLADD2(disp, button, btn2, (btn2, "btn2"));
  XCTRLADD2(disp, button, btn3, (btn3, "btn3"));
  XCTRLADD2(disp, button, btn4, (btn4, "btn4"));
  XCTRLADD2(disp, edit, edit1, (edit1));
  XCTRLADD2(disp, edit, edit2, (edit2));
  const char* rb1_str[] = {"item1", "item2", "item3"};
  XCTRLADD2(disp, rbox, rb1, (rb1, countof(rb1_str), rb1_str));
  XCTRLADD2(disp, cbox, cb1, (cb1, "cb1"));
  XCTRLADD2(disp, cbox, cb2, (cb2, "cb2"));
  XCTRLADD2(disp, listbox, lb1, (lb1));
  XCTRLADD2(disp, null, nul, (nul));

  xtreelay_node_t trnd1[] = {
    0, 0, layv,
    1, 0, layh,
    2, 0, id_nul,
    2, 0, id_lb1,
    1, 100, layh,
    2, 100, layv,
    3, 30, id_btn1,
    3, 30, id_btn2,
    2, 100, layv,
    3, 30, id_btn3,
    3, 30, id_btn4,
    2, 100, layv,
    3, 30, id_cb1,
    3, 30, id_cb2,
    2, 100, layv,
    3, 30, id_edit1,
    3, 30, id_edit2,
    2, 100, id_rb1,
    1, 20, id_sli1,
  };
  XCTRLADD2(disp, treelay, m_trlay1, (m_trlay1, 4, 4, countof(trnd1), trnd1));
  XCTRLADD2(disp, scroll, scroll1, (scroll1, id_m_trlay1));

  uistyle_default_set(disp->drawer);
  XCreateWindow(disp, mainwin, "test_ArtGalleryProblem", 100, 100, 800, 600, id_m_trlay1);

  for (; XWaitMsg(disp, msg, 50, MSG_MASK_KEY | MSG_MASK_MOUSE) >= 0;) {
    xwin_t* win = msg->win;

    if (win && mainwin == win->winid) {
      IRECT crc = disp->ci[id_nul].rc;
      int redraw = 0;
      int x = msg->x - crc.l, y = msg->y - crc.t;
      win = msg->win;

      if (MSG_COMMAND == msg->msg && XCMD_CLOSE == msg->key) {
        break;
      }

      switch (msg->msg) {
      case MSG_INIT:
        if (1) {
          redraw = 1;
        }

        break;

      case MSG_LBUTTONUP:
        if (1) {
        }

        break;

      case MSG_COMMAND:
        if (1) {
          if (XCMD_CLICK == msg->key) {
            if (id_btn1 == msg->lp) {
              char buf[256];
              static int iiii = 0;
              _snprintf(buf, 256, "item %d", ++iiii);
              printf("id_btn1\n");
              strv_ins_c_str(lb1->items, 10000, buf);
              update_ctrl(msg, id_lb1);
            }
          }
        }

        break;

      case MSG_LBUTTONDOWN:
        if (1) {
          int adsf = 0;
        }

        break;

      case MSG_MOUSEMOVE:
        if (1) {
        }

        break;
      }

      if (1) {
        int n_drawn = 0;
        img_t im[1] = {0};
        IRECT rcClient = crc;
        //UIGetWorkArea(ui, &rcClient);
        imsubref(win->im, rcClient, im);
        imdraw_fill(im, rgb(1, 1, 1));

        {
          COLOR clrs[] = {Red, Blue, Red, Blue, Red, Blue, Red, Blue};
          //IRECT rc = iRECT(0, 0, im->w, im->h);
          //test_draw_border(im, rc, fo);
          //test_colortable(im, rc, disp->drawer);
          //imdraw_circle_block(im, (im->w/2), (im->h/2), (sli1->m_value), 0, countof(clrs), clrs);
          //test_ArtGalleryProblem1(im);
          //test_triangle(im);
        }
        //imdrawui_default_polygon(uidrawer, im, m_poly1);
        //imdrawui_default_polygon(uidrawer, im, m_poly2);
        xEndPaint(win);
      }
    }
  }

  xwin_exit(disp);
  return 0;
}

