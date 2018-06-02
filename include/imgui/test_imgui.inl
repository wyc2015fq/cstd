#include "cstd.h"
//#include "ui/window.inl"
#include "bdfcursor.inl"
#include "draw/HZK16.inl"
#include "imgui.inl"
#include "bbb.inl"
#include "ctrl.inl"
#include "res.inl"
#include "test_flexctrl.inl"
#include "codec/compress.inl"
#include "screenkeyboard.inl"
#include "edit.inl"
//#include "test_window.inl"
//#include "test_draw3d_ctrl.inl"
//SetCursor(hCursor);LoadCursor(IDC_CROSS);
#include "test_strlist_ctrl.inl"
void window_print(window_t* c, int ind) {
  if (c) {
    int i;
    for (i=0; i<ind; ++i) {
      printf(" ");
    }
    printf("%s\n", c->text);
    window_print(c->child, ind + 2);
    window_print(c->next, ind);
  }
}
#include "test_ctrl.inl"
#include "test_timewindow.inl"
#include "test_testwindow.inl"
#include "test_showimage.inl"
#include "imgshop/test_imgshop.inl"
//#include "facerecog/test_facerecog.inl"
#include "draw/test_isee.inl"
#include "draw/test_draw.inl"
#include "d3/test_d3.inl"
#include "str/fast_atof.h"
//#include "face/A20/arg.inl"
#include "str/test_inifile.inl"
#include "face/a20/test_show_cnsun_logo.inl"


//#include "str/fast_dtoa.h"
int test_imgui()
{
  //char* aa = cstr_vsprintf2("%d %s %f", 1, " asdfasdf ", 3.14);
  //test_printf();
  //test_facerecogapp_loadsave();
  ctrl_t* c = NULL;
  double a = atof("3.14");
  img_t im[1] = {0};
  //test_compress_base64();
  //test_inifile();
  //test_show_cnsun_logo();
  if (0) {
    img_t im[1] = {0};
    imsetsize(im, 500, 500, 3, 1);
    imshow(im); cvWaitKey(-1);
    return 0;
  }
#if 0
  parse_arg("mode=1 curid=\"2002\"");
  //SPInfo* facelib = NULL;
  //load_facelib("E:/pub/bin/slgdev/user", "*.dat", &facelib);
  {
    char s_mode[16] = "dfr";
    static int xywh[12] = {0};
    static char stdface[100*100];
    static short feature[2500];
    SPInfo* featlib = NULL;
    int featnum = 0;
    int pic_height = 480;
    int pic_width = 640;
    char* buffers = (char*)pmalloc(640*480*2);
    argA20_run(pic_height, pic_width, buffers, pic_width*2, 2, "YUYV");
  }
#endif
  //imread("C:/10_SLICO.jpg", 3, 1, im);
  //binary_to_compressed_c("E:/code/cstd/include/easydraw/HZK16", fopen("E:/code/cstd/include/easydraw/HZK16_c.txt", "wb"), "HZK16", 1, 1);
  //io->res = res;
  //HZK16_font_init(fo);
  for (; c = host_wait(30);) {
    //test_showimage(c);
    //test_imgshop(c);
    //test_d3(c);
    //test_facerecog(c);
    //test_isee(c);
    //test_agg(c);
    test_ctrl(c);
    //test_screenkeyboard_ctrl(c);
    //test_gdiplusui(c);
    //test_testwindow();
    //test_flexctrl(c);
    //test_strlist_ctrl(c);
    //test_draw3d_ctrl(c);
  }
  return 0;
}
