//#include "subsystem.h"

#include "face/face_recog.inl"
#include "img/imgopt.inl"
#include "serialport.h"

//#include "..\project\GPRS\com.inl"

int chezha_open_time = 0;
int chezha_init()
{
  utime_start(chezha_open_time);
  return 0;
}
int chezha_open(int i)
{
  if (utime_elapsed(chezha_open_time) > 2) {
    serialport com1[1] = {0};
    char buf[256];
    int len = Str2Hex("A2 0B 0A 60 01 01 00 00 00 00 00 77 2A", buf);

    if (serialport_open(com1, 0, i, 19200, 'N', 8, 1, EV_RXFLAG | EV_RXCHAR, 1024)) {
      serialport_WriteToPort(com1, buf, len);
      serialport_writechar(com1);
      serialport_close(com1);
      utime_restart(chezha_open_time);
    }
  }

  return 0;
}

int test_cam_face_recog()
{
  enum {mainwin};
  xdisp_t disp[1] = {0};
  xmsg_t msg[1] = {0};

  XCTRLADD2(disp, button, m_btn_reg, (m_btn_reg, "²É¼¯"));
  XCTRLADD2(disp, button, m_btn_del, (m_btn_del, "É¾³ý"));
  XCTRLADD2(disp, edit, m_edit_id, (m_edit_id));
  XCTRLADD2(disp, null, nul, (nul));

  xtreelay_node_t trnd1[] = {
    0, 0, layv,
    1, 30, layh,
    2, 200, id_m_btn_reg,
    2, 0, id_m_edit_id,
    2, 200, id_m_btn_del,
    1, 0, id_nul,
  };
  XCTRLADD2(disp, treelay, m_trlay1, (m_trlay1, 4, 4, countof(trnd1), trnd1));
  XCTRLADD2(disp, scroll, scroll1, (scroll1, id_m_trlay1));

  enum { MAXCAP = 16 };
  img_t m_imcap[MAXCAP] = {0};
  capdev m_cap[MAXCAP] = {0};
  int capnum = 0;
  int row, col;
  int i;
  int frame = 0;

  faceapp_t m_fa[1] = {0};
  facerecog_param_t m_fr[MAXCAP] = {0};

  uistyle_default_set(disp->drawer);
  disp->drawer->style = UIS_WIN2k;
  XCreateWindow(disp, mainwin, "test_cam_face_recog", 100, 100, 800, 600, id_m_trlay1);

  capnum = CAPNUM();
  row = (int)sqrt(capnum);
  col = (capnum + row - 1) / row;

  m_fa->mode = FR_FACE_REC;
  chezha_init();
  buf_load(FACELIB, m_fa->m_fl);

  for (; XWaitMsg(disp, msg, 50, MSG_MASK_KEY | MSG_MASK_MOUSE) >= 0;) {
    IRECT crc = disp->ci[id_nul].rc;
    int redraw = 0;

    if (msg->win && mainwin == msg->win->winid) {
      xwin_t* win = msg->win;
      int redraw = 0;
      int x = msg->x - crc.l, y = msg->y - crc.t;

      if (MSG_COMMAND == msg->msg && XCMD_CLOSE == msg->key) {
        break;
      }

      switch (msg->msg) {
      case MSG_INIT:
        if (1) {
          redraw = 1;
          SetTimer(win->hwnd, 1, 50, 0);
        }

        break;

      case MSG_LBUTTONUP:
        if (1) {
        }

        break;

      case MSG_TIMER:
        if (1) {
          redraw = 1;
        }

        break;

      case MSG_COMMAND:
        if (1) {
          if (XCMD_CLICK == msg->key) {
            if (msg->lp == id_m_btn_reg) {
              m_fa->mode = FR_FACE_REG;
              printf("m_btn_reg EVENT_LCLICK\n");
              redraw = 1;
            }
            else if (msg->lp == id_m_btn_del) {
              facelib_del(m_fa->m_fl, m_edit_id->str->s);
              buf_save(FACELIB, m_fa->m_fl);
              printf("m_btn_del EVENT_LCLICK\n");
              redraw = 1;
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

      if (redraw) {
        int n_drawn = 0;
        img_t im[1] = {0};
        IRECT rcClient = crc;
        xwin_t* win = disp->win + mainwin;
        //UIGetWorkArea(ui, &rcClient);
        imsubref(win->im, rcClient, im);
        imdraw_fill(im, rgb(1, 1, 1));

        {
          int rec_ok = 0;
          m_fa->m_strEditLen = m_edit_id->str->l;
          //capnum=MIN(1, capnum);
          //printf("cap %d\n", capnum);
          capnum = capnum;

          if (0 == capnum) {
            continue;
          }

          sf_get_time();

          for (i = 0; i < capnum; ++i) {
            if (!m_cap[i].x) {
              CAPOPEN(&m_cap[i], i);
            }

            if (m_cap[i].x) {
              CAPGETFRAME(&m_cap[i], &m_imcap[i]);
              imflip_lr(&m_imcap[i]);
              m_fa->m_strEdit = m_edit_id->str->s;
              faceapp_run(m_fa, m_fr + i, &m_imcap[i]);

              if (0) {
                img_t im[1] = {0};
                img_t imgry[1] = {0};
                img_t ft[1] = {0};
                imsubref(&m_imcap[i], iRECT(0, 0, 130, 150), im);
                im2gry(im, imgry);
                //DFD_FEAT(imgry, ft);
                imfree(ft);
              }

              if (m_fa->rec_ok) {
                rec_ok = 1;
              }
            }
          }

          if (rec_ok) {
            //chezha_open(m_edit_com->str->s[0] - '0');
          }

          if (1) {
            IRECT rcs[MAXCAP] = {0};
            int i;
            //static int ddd=0;
            //printf("%d\n", ddd++);
            iRectMatrix(iRECT(0, 0, RCW(&rcClient), RCH(&rcClient)), row, col, 4, 4, capnum, rcs);

            //imdraw_clear(im, _GetSysColor(COLOR_BTNFACE));

            for (i = 0; i < capnum; ++i) {
              int idx = i;
              const img_t* imcap = m_imcap + idx;

              if (imcap->w) {
                IRECT rc = rcs[i];
                IRECT rc2 = iRECT(0, 0, imcap->w, imcap->h);
                iRectScaling(rc, imcap->w, imcap->h, &rc);
                imdraw_image(im, rc, imcap, rc2);
                faceapp_draw(im, rc, imcap, m_fa, m_fr + idx, disp->drawer);
              }
            }

          }
        }
        //imdrawui_default_polygon(uidrawer, im, m_poly1);
        //imdrawui_default_polygon(uidrawer, im, m_poly2);
        xEndPaint(win);
      }
    }
  }

  for (i = 0; i < capnum; ++i) {
    CAPCLOSE(m_cap + i);
  }

  freeims(m_imcap, capnum);
  xwin_exit(disp);
  return 0;
}

