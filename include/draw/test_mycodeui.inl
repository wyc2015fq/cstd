#include "cstd.h"
#include "cfile.h"
//#include "easydraw.inl"
//#include "imgui/test_imgui.inl"
//#include "mycode_ui.inl"
#include "mycode_face.inl"
//#include "mycode_test.inl"
#include "ui/window.inl"
#include "imgui/res.inl"
#include "cfile.h"
#include "cmem.h"
#if 0
static uchar pic_10_720x576x1[] = {
#include "res/pic_10_720x576x1.txt"
};
#endif
int test_mycodeui()
{
  double time1 = 0;
  int pos = 0, tpos = 0;
  const char* res[] = {
#include "E:/pub/bin/imgui/res/res.txt"
  };
  img_t im[10] = {0};
  img_t* im_color = im + 1;
  img_t screen[1] = {0};
  int screen_h = 480;
  int screen_w = 320;
  char name[256];
  char buf[256];
  cap_t cap[2] = {0};
  cap_t* cap_color = cap + 1;
  if (0) {
    int i;
    char* fn[] = {
      "E:/pub/bin/face/face_recog/001/pic_01.bmp",
      "E:/pub/bin/face/face_recog/003/pic_10.bmp"
    };
    sys_chdir("E:/code/cstd/include/easydraw/res");
    //imread("·µ»Ø.png", 4, 1, im);
    //imshow2("im", im);cvWaitKey(-1);
    for (i = 0; i < countof(fn); ++i) {
      imread(fn[i], 1, 1, im);
      _snprintf(buf, 256, "%s_%dx%dx%d.txt", GetFileNameCopy(fn[i], name, countof(name)), im->h, im->w, im->c);
      savedata_inl(buf, im->tt.data, im->h * im->s);
    }
    res_make_picarr("list.txt");
    //res_make("list.txt", "res.txt");
    //imshow_cn("asdf0", im, 1);
    //imshowmat_u1_cn(im, 0);
    //cvWaitKey(-1);
  }
  imsetsize(screen, screen_h, screen_w, 4, 1);
#if 0
  if (1) {
    buf_t bf[1] = {0};
  }
  //imsetsize(im, screen_h, screen_w, 4, 1);
  IMINIT(im, 720, 576, pic_10_720x576x1, 576, 1, 1);
#else
  cap_open(cap, 0, CAP_EROAD);
  cap_open(cap_color, 0, CAP_DSHOW);
#endif
  for (; ;) {
    cap_getframe(cap, im, T_RGB);
    cap_getframe(cap_color, im_color, T_RGB);
    myframe(im->h, im->w, im->tt.data, im->s, im->c, 0, screen_h, screen_w, screen->tt.data, screen->s, screen->c);
    imshow(screen);
    imshow(im_color);
    if ('q' == cvWaitMsg(100, true)) {
      break;
    }
  }
  return 0;
}

