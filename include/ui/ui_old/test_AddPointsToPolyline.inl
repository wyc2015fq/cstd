
#include "AddPointsToPolyline.inl"

int test_AddPointsToPolyline()
{
  enum {mainwin};
  xdisp_t disp[1] = {0};
  xmsg_t msg[1] = {0};
  enum {N = 100};
  DPOINT2 LinePoints[N] = {0};
  DPOINT2 TempLine[N] = {0};
  VectorLine test[N] = {0};
  int nLinePoints = 0;
  int nTempLine = 0;

  XCTRLADD(disp, layout, lay1);
  XCTRLADD2(disp, cbox, RadioButton1, (RadioButton1, "Place Line points"));
  XCTRLADD2(disp, cbox, RadioButton2, (RadioButton2, "Insert Point"));
  XCTRLADD2(disp, button, Button1, (Button1, "Draw limits"));
  XCTRLADD2(disp, button, Button2, (Button2, "Start over Fresh"));
  XCTRLADD2(disp, null, nul, (nul));

  int iid[] = {
    0,                      layx,          layx,          layx,
    20,        id_RadioButton1,             0,             0,
    20,        id_RadioButton2,             0,             0,
    20,             id_Button1,             0,             0,
    20,             id_Button2,             0,             0,
    layx,                 id_nul,          layh,          layh,
  };

  uistyle_default_set(disp->drawer);
  xlayout_set(lay1, countof(iid) / 4 - 1, 4 - 1, iid, 4, 4);

  XCreateWindow(disp, mainwin, "test_AddPointsToPolyline", 100, 100, 800, 600, id_lay1);

  for (; XWaitMsg(disp, msg, 50, MSG_MASK_KEY | MSG_MASK_MOUSE) >= 0;) {
    xwin_t* win = msg->win;
    int i, j;

    if (win && mainwin == win->winid) {
      IRECT crc = disp->di[id_nul].rc;
      int x = msg->x - crc.l, y = msg->y - crc.t;
      win = msg->win;

      if (MSG_COMMAND == msg->msg && XCMD_CLOSE == msg->key) {
        break;
      }

      switch (msg->msg) {
      case MSG_INIT:
        if (1) {
          j = 0;

          for (i = 30; i <= 701; i += 50) {
            LinePoints[j].x = i;
            LinePoints[j].y = i / 2.;
            ++j;
          }

          nLinePoints = j;
          RadioButton2->hit = true;
          msg->redraw = 1;
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
          if (XCMD_CLICK == msg->key && id_Button1 == msg->lp) {
            RadioButton1->hit = true;
            nLinePoints = 0;
          }
        }

        break;

      case MSG_LBUTTONDOWN:
        if (iPtInRect(&crc, msg->x, msg->y)) {
          DPOINT2 pt = dPOINT(x, y);

          if (RadioButton1->hit == true) {
            LinePoints[nLinePoints++] = pt;
          }
          else if (RadioButton2->hit == true) {
            InsertPoint(nLinePoints, LinePoints, pt);
            ++nLinePoints;
          }

          msg->redraw = 1;
        }

        break;

      case MSG_MOUSEMOVE:
        if (iPtInRect(&crc, msg->x, msg->y)) {
          if (RadioButton2->hit == true) {
            DPOINT2 pt = dPOINT(x, y);
            MEMCPY(TempLine, LinePoints, nLinePoints);
            nTempLine = nLinePoints + 1;
            InsertPoint(nLinePoints, TempLine, pt);
            msg->redraw = 1;
          }
        }

        break;
      }

      if (msg->redraw) {
        int n_drawn = 0;
        img_t im[1] = {0};
        //UIGetWorkArea(ui, &rcClient);
        imsubref(win->im, crc, im);
        imdraw_fill(im, rgb(1, 1, 1));

        {
          //imdrawaa_poly(im, LinePoints, nLinePoints, 0, 0, Black, 2);
          imdrawaa_dashpoly2(im, TempLine, nTempLine, 0, Black, 2, 5, 5);
          //imdrawaa_dashpoly2(im, LinePoints, nLinePoints, 0, Black, 2, 5, 5);
          CalculateAllAngles(nLinePoints, LinePoints, test);

          for (i = 0; i < nLinePoints; ++i) {
            VectorLine p = test[i];
            imdrawaa_line(im, p.Point1.x, p.Point1.y, p.Point2.x, p.Point2.y, Red, 2);
          }
        }
        //imdrawui_default_polygon(uidrawer, im, m_poly1);
        //imdrawui_default_polygon(uidrawer, im, m_poly2);
        xEndPaint(win);
      }
    }
  }

  return 0;
}

