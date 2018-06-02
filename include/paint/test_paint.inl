#include "imui/imui.inl"

static const char res[] = {
#include "res.txt"
  0
};
int test_paint()
{
  int i = 0;
  ImGuiHost host[1] = {0};
  ImColor clrbg = _RGB(114, 144, 154);
  window_t btn[1000] = {0};
  double time1 = 0, sumtime = 0;
  int frame = 0;
  const char* rrr = res;
  if (1) {
    char* buf = NULL;
    int len = loaddata("E:/pub/aaa/sentinel/YQZME.evc", &buf);
    FILE* pf = fopen("E:/pub/aaa/sentinel/YQZME.h", "wb");
    print_string(pf, buf, len, 128);
    fclose(pf);
  }
  if (0) {
    sys_chdir("E:/code/cstd/include/paint");
    res_pack_make("reslist.txt", "res.txt");
  }
  ImGuiDriverInit_t drv_init;
  drv_init = soft_Init;
  clrbg = myGetSysColor(CLR_BTNFACE);
  ImGuiHost_Init(host, drv_init, "test_paint", 100, 100, 800, 600);
  for (; imuiLoop(50);) {
    if (1) {
      utime_start(_start_time);
      if (host_begin(host, clrbg)) {
        GETGUIIO2();
        char* aa = (char*)io;
        if (0) {
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
        if (1) {
          static int inited = 0;
          static texture_t tex1[1] = {0};
          char* names[] = {
            "LineToolIcon",
              "AnchorChooserControl.AnchorImage",
              "CloneStampToolIcon",
              "BlendingOverwriteIcon",
              "JuliaFractalEffectIcon",
              "MenuWindowColorsIcon",
              "MenuHelpPdnWebsiteIcon",
          };
          static IRECT rcs[countof(names)] = {0};
          if (!inited) {
            int i;
            img_t im[1] = {0};
            res_pack_getimg(res, countof(res), im);
            host->DrvSetTexture(host, tex1, T_BGRA, im->h, im->w, im->tt.data, im->s);
            for (i=0; i<countof(names); ++i) {
              res_pack_find(res, countof(res), names[i], rcs + i);
            }
            //imshowrc(im, rcs[0]);
            if (0) {
              img_t im1[1] = {0};
              imsubref(im, rcs[0], im1);
              imshowmat_x4(im1);
              imshow(im);
              cvWaitKey(-1);
            }
            imfree(im);
            inited = 1;
          }
          {
            int hitid = toolbar_ctrl(tex1, countof(names), rcs);
            if (hitid>=0) {
              printf("hitid = %d\n", hitid);
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
  }
  imuiUnInit();
  return 0;
}
#include "imui/softgc.inl"
#ifdef _WIN32
#include "imui/imui_win32.inl"
//#include "imui_gdi.inl"
//#include "imui_gdip.inl"
//#include "imui_d3d9.inl"
//#include "imui_gl2.inl"
#include "imui/imui_soft.inl"
//#include "imui_d2d1.inl"
#endif
#ifdef __linux__
#include "imui/imui_linux.inl"
#include "imui/imui_x11.inl"
#endif
