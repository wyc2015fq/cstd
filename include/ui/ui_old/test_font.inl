
#include "font/fonfmt.inl"
#include "font/ttffmt.inl"
//#include "./font/ttfdoc.h"
//#include "TTF.h"


unsigned char fondata[] = {
  //#include "8514oem.inl"
#include "font\8514fix.inl"
};

int test_print_font()
{
  FILE* pf = fopen("out.txt", "w");
  fonfmt_t* fon;
  int pos = loadfon(fondata, sizeof(fondata));
  fon = (fonfmt_t*)(fondata + pos);
  print_font(fon, pf);
  fclose(pf);
  return 0;
}

int test_font()
{
  enum {mainwin};
  xdisp_t disp[1] = {0};
  xmsg_t msg[1] = {0};

  uistyle_default_t uidrawer[1] = {0};
  XCTRLADD(disp, layout, lay1);
  XCTRLADD2(disp, slider, sli1, (sli1, 0, 14, "page:%.f"));
  enum {id_nul1 = 3, id_nul2, id_nul3};

  int iid[] = {
    0,                layx,             layx,             layx,
    20,            id_nul3,          id_sli1,             layh,
    layx,          id_nul2,          id_nul1,             layh,
  };

  IRECT rcs[256] = {0};
  int hit = 0;
  ttffmt_t ttf[1] = {0};
  fonchar_t fc[1] = {0};
  const char* fn;
  _chdir("D:\\code\\c\\Font\\Fonts");
  fn = "arial.ttf";
  fn = "庞中华行书.ttf";
  fn = "Anja Eliane.ttf";
  fn = "Celestial2.ttf";
  fn = "熊猫星星脚印字体.ttf";
  fn = "\\\\192.168.1.18\\os\\font\\new\\造字工房尚黑G0v1\\造字工房尚黑G0v1特细超长体.otf";
  fn = "YaHei.Consolas.1.11b.ttf";
  fn = "華康少女字W6.ttf";
  fn = "simfang.ttf";
  //LoadTtfDoc(pttf, fn);
  ttf_load(fn, ttf);

  uistyle_default_set(uidrawer);
  xlayout_set(lay1, countof(iid) / 4 - 1, 4 - 1, iid, 4, 4);
  XCreateWindow(disp, mainwin, "test_font", 100, 100, 800, 600, id_lay1);

  for (; XWaitMsg(disp, msg, 50, MSG_MASK_KEY | MSG_MASK_MOUSE) >= 0;) {
    xwin_t* win = msg->win;

    if (win && mainwin == win->winid) {
      IRECT crc = win->fr->rcs[HITCLIENT].rc;
      int i, x = msg->x - crc.l, y = msg->y - crc.t;
      win = msg->win;

      if (MSG_COMMAND == msg->msg && XCMD_CLOSE == msg->key) {
        break;
      }

      switch (msg->msg) {
      case MSG_INIT:
        if (1) {
          DragAcceptFiles(msg->win->hwnd, TRUE);
          sli1->m_max = (ttf->maxp->NumGlyphs) / 256;
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

      case MSG_MOUSEWHEEL:
        if (1) {
          sli1->m_value += GET_WHEEL_DELTA_WPARAM(msg->wp) > 0 ? -1 : 1;
          sli1->m_value = BOUND(sli1->m_value, sli1->m_min, sli1->m_max);
          force_redraw(msg);
        }

        break;

      case MSG_LBUTTONDOWN:
        if (1) {
          int hit1 = iPtInRects(256, rcs, msg->x, msg->y);

          if (hit1 >= 0 && hit1 != hit) {
            hit = hit1;
            force_redraw(msg);
          }
        }

        break;

      case MSG_MOUSEMOVE:
        if (1) {
        }

        break;

      case MSG_DROPFILES:
        if (1) {
          //当文件拖进来时
          TCHAR szBuf[ MAX_PATH ];
          //如果多个文件被拖拽的情况只取最后一个文件
          DragQueryFile((HDROP)msg->wp, 0, szBuf, MAX_PATH - 1);
          ttf_load(szBuf, ttf);
          sli1->m_max = (ttf->maxp->NumGlyphs) / 256;
          DragFinish((HDROP)msg->wp);
          force_redraw(msg);
        }

        break;
      }

      if (msg->redraw) {
        int n_drawn = 0;
        img_t im[1] = {0};
        IRECT rcClient = xGetClient(win);
        //UIGetWorkArea(ui, &rcClient);
        imsubref(win->im, rcClient, im);
        imdraw_fill(IMARG(im), rgb(1, 1, 1));
        *im = *win->im;

        {
          int i;
          COLOR clr;
          IRECT rc;
          char buf[256];
          IRECT nul1_rc = disp->di[id_nul1].rc;
          IRECT nul2_rc = disp->di[id_nul2].rc;
          IRECT nul3_rc = disp->di[id_nul3].rc;
          iRectMatrix(nul1_rc, 16, 16, 1, 1, 256, rcs);

          //DrawGlyph(IMARG(im), rc.left, rc.top, RCW(&rc), RCW(&rc), pttf, 8);
          clr = _RGBA(0, 0, 0, 150);

          if (0) {
            rc = iRectToSquare(disp->di[id_nul1].rc);
            imdraw_rect(IMARG(im), rc, _RGB(255, 255, 255), _RGB(0, 0, 0), 1);
            imdrawaa_ttf_index(IMARG(im), rc, ttf, 3649, clr, 0, 0);
            //drawaa_ttf_index(IMARG(im), rc, ttf, 8, clr);
          }
          else {
            int page = 256 * (int)(sli1->m_value);
            utime_start(_start_time);

            for (i = 0; i < 256; ++i) {
              rc = iRectToSquare(rcs[i]);
              imdraw_rect(IMARG(im), rc, _RGB(255, 255, 255), i == hit ? _RGB(255, 0, 0) : _RGBA(0, 0, 0, 0), 1);
              clr = i == hit ? _RGB(0, 0, 0) : _RGBA(0, 0, 0, 250);
              //imdrawaa_ttf_index(IMARG(im), rc, ttf, page + i - (i & 1), clr, 0, 0);
              imdrawaa_ttf_index(IMARG(im), rc, ttf, page + i, clr, 0, 0);

              if (i & 1) {
                img_t im2[1] = {0};
                imsubref(im, rc, im2);
                //EnhanceEdges(IMARG(im2), 6, 0);
              }
            }

            _snprintf(buf, 256, "%d × %d", RCH(&rc), RCW(&rc));
            imdraw_text(IMARG(im), nul3_rc, buf, -1, uidrawer->font, _RGB(0, 0, 0), 0, TT_VCENTER);
            rc = iRectToSquare(nul2_rc);
            imdraw_rect(IMARG(im), rc, _RGB(255, 255, 255), _RGB(0, 0, 0), 1);
            //drawaa_ttf_index(IMARG(im), rc, ttf, 1624, clr);
            imdrawaa_ttf_index(IMARG(im), rc, ttf, page + hit, clr, rgb(1, 0, 0), 2);
            _snprintf(buf, 256, "%d", page + hit);
            imdraw_text(IMARG(im), rc, buf, -1, uidrawer->font, _RGB(0, 0, 0), 0, TT_VCENTER);
            printf("%f\n", utime_elapsed(_start_time));
          }
        }
        //imdrawui_default_polygon(uidrawer, im, m_poly1);
        //imdrawui_default_polygon(uidrawer, im, m_poly2);
        xEndPaint(win, imdrawui_default, uidrawer);
      }
    }
  }

  ttf_free(ttf);
  return 0;
}
