
#include "cstd.h"
#include "cfile.h"

int test_show_points() {
  img_t im[1] = {0};
  buf_t bf[1] = {0};
  double* pt3;
  int i, len;
  buf_load("C:\\test\\points.dat", bf);
  pt3 = (double*)bf->data;
  len = bf->len/(sizeof(double)*3);
  imsetsize(im, 256, 256, 1, 1);
  for (i=0; i<len; ++i) {
    int x = (int)pt3[i*3+0];
    int y = (int)pt3[i*3+1];
    int z = (int)pt3[i*3+2];
    if (x>=0 && x<im->w && y>=0 && y<im->h) {
      im->tt.data[x + y*im->s] = BOUND(z, 0, 255);
    }
  }
  imshow(im);cvWaitKey(-1);
  return 0;
}