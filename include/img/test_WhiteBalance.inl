
#include "cstd.h"
//#include "color.inl"
//#include "cap.inl"
//#include "imdraw.inl"
#include "window.inl"
#include "..\fmtimg\fmtimg.inl"

#include "wbGW.h"

int test_WhiteBalance()
{
  img_t* im = imread2("aaa.bmp", 3);
  img_t* im2 = imclone(im);
  char ch = '9';

  for (;;) {
    if ('q' == ch || 'Q' == ch) {
      break;
    }

    switch (ch) {
      //case '1': { test_mycell(im, im2); } break;
    default:
      break;
    }

    imshow(im2);
    ch = cvWaitKey(20);
  }

  imfree(im);
  imfree(im2);
  return 0;
}


int main()
{
  test_WhiteBalance();
  return 0;
}
