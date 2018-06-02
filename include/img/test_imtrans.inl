
#include "cstd.h"
#include "imgopt.inl"
#include "img/imgio.inl"

int test_imtrans() {
  img_t im[3] = {0};
  img_t* im1 = im+1;
  img_t* im2 = im+2;
  int i, opt = 1;
  imsetsize(im, 800, 800, 3, 1);
  imread("C://ËÕAQ5B65.jpg", 3, 1, im);
  if (1) {
    uchar aa[] = {1, 2, 3, 
      4, 5, 6};
    uchar bb[6] = {0};
    uchar cc[6] = {0};
    int h, w, bl;
    mat_trans(2, 3, aa, 3, 1, &h, &w, bb, &bl, 2, 1);
    mat_trans2(2, 3, aa, 3, 1, &h, &w, cc, &bl, 2, 1);
    mat_trans2(2, 3, aa, 3, 1, &h, &w, cc, &bl, 2, 1);
  }
  
  for (i=0; i<4; ++i) {
    imtrans2(im, im1, i, 1);
    imtrans3(im, im2, i, 1);
    ASSERT(0==memcmp(im1->tt.data, im2->tt.data, im1->h*im1->w*3));
    imtrans2(im, im1, i, 0);
    imtrans3(im, im2, i, 0);
    ASSERT(0==memcmp(im1->tt.data, im2->tt.data, im1->h*im1->w*3));
  }
  {utime_start(_start_time);
  for (i=0; i<10; ++i) {
    imtrans3(im, im1, 3, 1);
  }
  printf("%f\n", utime_elapsed(_start_time));}
  {utime_start(_start_time);
  for (i=0; i<10; ++i) {
    imtrans2(im, im1, 3, 1);
  }
  printf("%f\n", utime_elapsed(_start_time));}
  imfree(im1);
  for (;;) {
    char ch = cvWaitKey(-1);
    if ('q'==ch || 'Q'==ch) {
      break;
    } else if (' '==ch || 't'==ch || 'T'==ch) {
      opt = (opt+1)&3;
    }
  {utime_start(_start_time);
  imtrans2(im, im1, opt, 1);
printf("%f\n", utime_elapsed(_start_time));}
    imshow(im1);
  }
  imfrees(im, countof(im));
  return 0;
}