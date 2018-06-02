
#include "chalg.h"
//#include "color.inl"
//#include "cap.inl"
//#include "imdraw.inl"
#include "..\imgproc\window.inl"
#include "..\fmtimg\fmtimg.inl"
#include "monofilt.inl"


int test_monofilt() {
  img_t* im = imread2("test_1_100x100.bmp", 1);
  img_t* im2 = imclone(im);
  char ch='9';
  for (;;) {
    if ('q'==ch||'Q'==ch) {break;}
    switch(ch) {
    case '1':
      {
        int minWaveLength       =  4;
        int region_num          =  8;

        double sigmaOnf            =  0.64;
        double mult                =  1.7;
        double nscale              =  3;
        double orientWrap          =  0;
        double bh_n                =  4;
        double bw_n                =  5;
        double sh_n                =  1;
        double sw_n                =  1;
        double neigh               =  8;
        double radius              =  3;
        double NumTarget           =  10;
        double f1, h1f1, h2f1, A1, theta1, psi1;
        monofilt(im, nscale, minWaveLength, mult, sigmaOnf, orientWrap, 
          &f1, &h1f1, &h2f1, &A1, &theta1, &psi1);
      } break;
    default: break;
    }
    imshow(im2);
    ch = cvWaitKey(20);
  }
  imfree(im);
  imfree(im2);
  return 0;
}


int main() {
  test_demhist();
  return 0;
}
