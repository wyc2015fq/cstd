
#include "cstd.h"
#include "color.inl"

int test_color() {
  img_t im[10] = {0};
  imchessboard(im, 100,  100, 2, 20);
  imsetsize(im+1, 100,  100, 3, 1);
  imcolorcvt(im, im+1, T_YUYV, T_BGRA);
  //imchessboard(im+1, 100,  100, 3, 10);
  imshow(im+1);cvWaitKey(-1);
  imcolorcvt(im, im+1, T_YUYV, T_BGR);
  imfrees(im, 10);
  return 0;
}