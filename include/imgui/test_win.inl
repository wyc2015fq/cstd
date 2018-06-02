
#include "cstd.h"
//#include "ui/window.inl"

#include "img/imgio.inl"
int test_win() {
  uchar* data = NULL;
  void* pbuf = NULL;
  //img_t im[1] = {0};
  int h = 512, w = 512, cn = 4;
  char ch = 0;
  img_t im[1] = {0};
  imread("E:/58.jpg", 3, 1, im);
  multialloc(&pbuf, &data, h*w*cn, NULL);
  //IMINIT(im, h, w, data, w*cn, *cn);
  show_bitmap("asdf", im->h, im->w, im->tt.data, im->s, PF_24bppRGB, 1, 0);
  //show_bitmap("asdfdsf", h, w, data, w*cn, PF_32bppRGB, 1, 0);
  for (; ch = waitkey(50);) {
    if ('e'==ch) {
      break;
    }
  }
  FREE(pbuf);
  return 0;
}
