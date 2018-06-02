#include "cstd.h"
#include "cfile.h"
#include "easydraw.inl"
//英文使用的是8*16的矩阵，中文用24*24的 矩阵显示“中”字。
#include "mycode_ui.inl"
#include "mycode_face.inl"
//#include "ui/window.inl"
const char* winname = "face recog";
int test_easydraw()
{
  window_host_t host[2] = {0};
  img_t im[1] = {0};
  //test_face_recog2();
  if (0) {
    buf_t bf[1] = {0};
    sys_chdir("E:/code/cstd/include/easydraw");
    buf_load("HZK16", bf);
    savedata_inl("HZK16.txt", bf->data, bf->len);
  }
  imsetsize(im, 480, 320, 3, 1);
  imput_str(im, 100, 100, "爱gasdfasdf我", -1, _rgb(1, 1, 1));
  imput_circle(im, 100, 100, 200, _rgb(0, 1, 0));
  //imput_chinese(im, 100, 100, "我", HZK16);
  for (; sys_waitio(100);) {
    int h = im->h, w = im->w;
    myframe(h, w, im->tt.data, im->s, im->c, 0, h, w, im->tt.data, im->s, im->c);
    show_im(winname, im);
  }
  return 0;
}

