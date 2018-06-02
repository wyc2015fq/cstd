#include "imui.inl"
//#include "vec_cpp.inl"
#include "ctrl_draw.inl"
#include "imui_ctrl.inl"
#include "draw/drawtext.inl"
#include "gsvfont.inl"
#include "test_cpmdev_dlg.inl"
#include "test_window.inl"
#include "aaa.inl"
#include "res.inl"
#include "test_springLayout.inl"
#include "game/test_boxman.inl"
#include "game/maze.inl"
#include "game/test_15ui.inl"
//#include "game/test_astar.inl"
//#include "img/imgio.inl"
//#include "test_tetris.inl"
//#include "arch/test_zip.inl" Color_Black
typedef struct treeitem_t_ treeitem_t;
struct treeitem_t_ {
  void* x;
  int (*get_next)(treeitem_t* x);
  int (*get_first_child)(treeitem_t* x);
};
//#include "game/Astar3.inl"
int test_imui()
{
  int i = 0;
  ImGuiHost host[1] = {0};
  ImGuiHost host2[1] = {0};
  ImColor clrbg = _RGB(114, 144, 154);
  window_t btn[1000] = {0};
  double time1 = 0, sumtime = 0;
  int frame = 0;
  //test_astar();
  //test_astar3();
  ImGuiDriverInit_t drv_init;
  //test_time();
  //test_zip();
#ifdef _WIN32
  drv_init = gdi_Init;
  drv_init = gdip_Init;
  drv_init = gl2_Init;
  drv_init = d3d9_Init;
  drv_init = soft_Init;
#else
  drv_init = x11_Init;
#endif
  //test_packing_raster_font();
  //test_font();
  clrbg = myGetSysColor(CLR_BTNFACE);
  ImGuiHost_Init(host, drv_init, "imui Example", 100, 100, 1280, 800);
  //ImGuiHost_Init(host, drv_init, "ImGui DirectX9 Example", 100, 100, 800, 600, clrbg);
  //ImGuiHost_Init(host2, drv_init, "ImGui DirectX9 Example2", 100, 100, 1280, 800, clrbg);
  for (; imuiLoop(50);) {
    if (1) {
      utime_start(_start_time);
      if (host_begin(host, clrbg)) {
        GETGUIIO2();
        char* aa = (char*)io;
        if (1) {
          if (frame > 0) {
            double avgtime = sumtime / frame;
            static int showtime = 1;
            set_layout_flags(LF_alighParentTop);
            if (autolayout_begin(NULL)) {
              set_layout_flags(LF_alighParentTop | LF_alighParentLeft);
              cbox_ctrl("showtime", &showtime);
              if (showtime) {
                label_ctrl("avg time1 %5.2f ms %5.2f frame", 1000 * avgtime, 1. / avgtime);
              }
              else {
                printf("avg time %5.2f ms %5.2f frame\r", 1000 * avgtime, 1. / avgtime);
              }
              linearlayout_end();
            }
          }
        }
        if (0) {
          static test_numui_t numgame[1] = {4, 4, 0};
          test_numui(numgame);
        }
        if (0) {
          test_boxman();
        }
        if (0) {
          static DPOINT pt[10] = {0};
          static int npt = 0;
          //static int ln[];
          if (0 == npt) {
            npt = 6;
            pt_random(npt, 500, 500, &pt->x);
          }
          polygon_proc(pt, npt, 0, 1, 5);
        }
        if (0) {
          set_layout_flags(LF_alighParentTop);
          static window_t win1[1] = {0};
          static IRECT rcWin = {50, 150, 50 + 200, 150 + 200};
          if (window_begin(win1, "win 1", &rcWin, NULL, 0)) {
            test_cpmdev_dlg();
            window_end();
          }
        }
        if (0) {
          set_layout_flags(LF_alighParentTop);
          test_cpmdev_dlg();
        }
        if (0) {
          set_layout_flags(LF_alighParentTop);
          test_window();
        }
        if (0) {
          static texture_t tex[1] = {0};
          if (0 == tex->h) {
            img_t im[1] = {0};
            int x, y;
            imsetsize(im, 256, 256, 4, 1);
            for (i = 0; i < im->w * im->h; ++i) {
              im->tt.u4[i] = (i / 7) & 1 ? Color_Blue : Color_Green;
            }
            for (y = 0; y < im->h; ++y) {
              for (x = 0; x < im->w; ++x) {
                *(unsigned int*)((unsigned char*)im->tt.data + im->s * y + x * 4) = (((x / 8) + y / 8) & 1) ? Color_Blue : Color_Green;
              }
            }
            //imwrite("C:/ttt.bmp", im);
            host->DrvSetTexture(host, tex, T_BGRA, im->h, im->w, im->tt.data, im->s);
            imfree(im);
          }
          {
            IRECT rc = iRECT2(100, 100, 200, 200);
            IRECT rc1 = iRECT2(0, 0, 200, 200);
            gcClipRect(ctx, 50, 50, 150, 150);
            gcRectImageR(ctx, rc, tex, &rc1);
            gcClipReset(ctx);
          }
        }
        if (0) {
          set_layout_flags(LF_alighParentTop);
          if (autolayout_begin(NULL)) {
            set_layout_flags(LF_alighParentTop);
            //test_tetris();
            linearlayout_end();
          }
        }
        if (0) {
          test_springLayout();
        }
        if (1) {
          if (0) {
            float blendPositions[] = {0, 1};
            COLOR presetColors[] = {Color_Red, Color_Blue};
            //gcSolidCircle(ctx, 100, 100, 50, _rgb(1,0,0), 0, 0);
            gcBeginPath(ctx);
            gcArc(ctx, 200, 200, 100, 100, 0, 360);
            gcClosePath(ctx);
            gcFillGradient(ctx, 100, 100, 300, 300, 2, blendPositions, presetColors);
            gcFill(ctx);
          }
          if (0) {
            set_layout_flags(LF_alighParentTop);
            if (autolayout_begin(NULL)) {
              static uint date = 0;
              set_layout_flags(LF_alighParentLeft | LF_alighParentTop);
              if (monthcal_ctrl(&date)) {
                printf("monthcal_ctrl %d \n", date);
              }
              linearlayout_end();
            }
          }
        }
        host_end();
      }
      time1 = utime_elapsed(_start_time);
      if (frame >= 3) {
        if (frame == 3) {
          sumtime += 3 * time1;
        }
        sumtime += time1;
      }
      ++frame;
    }
    //gcFillRect(io->ctx, 20, 30, 100, 100);
    //ImDraw_RectFilled(io->ctx, 20, 30, 100, 100, ImRGBA(244, 1, 121, 50), 0, 0);
    //ImDraw_Text2(io->ctx, 100, 100, _RGB(244, 244, 0), "Hello World!", NULL);
    //printf("%d %08x %d\n", i, io->ctx->buf, io->ctx->len);
  }
  imuiUnInit();
  return 0;
}
#if 0
#include "cstd.h"imsave
#include "img/imgio.inl"
#include "sys_win32.inl"
#endif
#include "softgc.inl"
#ifdef _WIN32
#include "imui_win32.inl"
#include "imui_gdi.inl"
#include "imui_gdip.inl"
#include "imui_d3d9.inl"
#include "imui_gl2.inl"
#include "imui_soft.inl"
//#include "imui_d2d1.inl"
#endif
#ifdef __linux__
#include "imui_linux.inl"
#include "imui_x11.inl"
#endif

