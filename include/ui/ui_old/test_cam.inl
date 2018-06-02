//#include "subsystem.h"
#include "inifile.h"
#include "sftime.h"
#include "fmtimg/fmtimg.inl"

#if 0
#define CAPNUM openni_capnum
#define CAPOPEN openni_open
#define CAPGETFRAME openni_getframe_depth
#define CAPCLOSE openni_close
#else
#if 0
#define CAPNUM dshow_capnum
#define CAPOPEN dshow_open
#define CAPGETFRAME dshow_getframe
#define CAPCLOSE dshow_close
#else
#define CAPNUM  caperoad_capnum
#define CAPOPEN caperoad_open
#define CAPGETFRAME caperoad_getframe
#define CAPCLOSE caperoad_close
#endif
#endif

int test_cam()
{
  enum {mainwin};
  xdisp_t disp[1] = {0};
  xmsg_t msg[1] = {0};

  XCTRLADD2(disp, button, m_btn_cap, (m_btn_cap, "cap"));
  XCTRLADD2(disp, button, m_btn_capv, (m_btn_capv, "capv"));
  XCTRLADD2(disp, null, nul, (nul));

  xtreelay_node_t trnd1[] = {
    0, 0, layv,
    1, 30, layh,
    2, 200, id_m_btn_capv,
    2, 200, id_m_btn_cap,
    1, 0, id_nul,
  };
  XCTRLADD2(disp, treelay, m_trlay1, (m_trlay1, 4, 4, countof(trnd1), trnd1));
  XCTRLADD2(disp, scroll, scroll1, (scroll1, id_m_trlay1));

  enum { MAXCAP = 256 };
  img_t m_imcap[MAXCAP] = {0};
  capdev m_cap[MAXCAP] = {0};
  int capnum = 0;
  int is_on_capv;
  int row, col;
  int i;
  int frame = 0;

  uistyle_default_set(disp->drawer);
  disp->drawer->style = UIS_WIN2k;
  XCreateWindow(disp, mainwin, "test_camui", 100, 100, 800, 600, id_m_trlay1);

  capnum = CAPNUM();

  if (capnum) {
    row = (int)sqrt(capnum);
    col = (capnum + row - 1) / row;

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
              if (id_m_btn_cap == msg->lp) {
                int i;
                char buf[256];
                sf_get_time();

                for (i = 0; i < capnum; ++i) {
                  img_t* im = (&m_imcap[i]);
                  _snprintf(buf, 256, ".\\cap_%d_%02d%02d%02d%03d.jpg", i,
                      sfhour(), sfminute() , sfsecond() , sfmillis());
                  imwrite2(buf, im);
                }

                printf("m_btn_cap EVENT_LCLICK\n");
              }

              if (id_m_btn_capv == msg->lp) {
                is_on_capv = !is_on_capv;
                printf("is_on_capv EVENT_LCLICK\n");
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
          {
            utime_start(_start_time);
            imdraw_fill(im, rgb(1, 1, 1));
            printf(" f%f", utime_elapsed(_start_time));
          }

          sf_get_time();
          {
            {
              utime_start(_start_time);

              for (i = 0; i < capnum; ++i) {
                if (!m_cap[i].x) {
                  CAPOPEN(&m_cap[i], i);
                }

                if (m_cap[i].x) {
                  CAPGETFRAME(&m_cap[i], &m_imcap[i]);

                  //openni_getframe(&m_cap[i], &m_imcap[i]);
                  if (is_on_capv) {
                    //char buf[256];
                    //_snprintf(buf, 256, ".\\capv_%d_%d_%02d%02d%02d%03d.jpg", i, frame, sfhour(), sfminute() , sfsecond() , sfmillis());
                    //printf("%s\n", buf);
                    //imwrite2(buf, &m_imcap[i]);
                  }
                }

                {
                  char buf[256];
                  _snprintf(buf, 256, "ud%d", i);

                  if (inigetint(buf) > 0) {
                    imflip_ud(&m_imcap[i]);
                  }

                  _snprintf(buf, 256, "lr%d", i);

                  if (inigetint(buf) > 0) {
                    imflip_lr(&m_imcap[i]);
                  }
                }
              }

              //printf("==================   %f\n", utime_elapsed(_start_time));
            }

            if (0) {
              for (i = 0; i < capnum; ++i) {
                if (m_cap[i].x) {
                  if (is_on_capv) {
                    char buf[256];
                    _snprintf(buf, 256, ".\\capv_%d_%d_%02d%02d%02d%03d.jpg", i, frame,
                        sfhour(), sfminute() , sfsecond() , sfmillis());
                    printf("%s\n", buf);
                    imwrite2(buf, &m_imcap[i]);
                  }
                }
              }
            }

            if (1) {
              IRECT rcs[MAXCAP] = {0};
              //static int ddd=0;
              //printf("%d\n", ddd++);
              iRectMatrix(iRECT(0, 0, RCW(&rcClient), RCH(&rcClient)), row, col, 4, 4, capnum, rcs);

              for (i = 0; i < capnum; ++i) {
                if (m_imcap[i].w) {
                  IRECT r = rcs[i];
                  iRectScaling(r, m_imcap[i].w, m_imcap[i].h, &r);

                  if (1) {
                    utime_start(_start_time);
                    imdraw_image(im, r, &m_imcap[i], iRECT(0, 0, m_imcap[i].w, m_imcap[i].h));
                    printf(" i%f", utime_elapsed(_start_time));
                  }
                }
              }
            }

            printf("\n");
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
  }

  xwin_exit(disp);
  return 0;
}

