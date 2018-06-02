#include <math.h>
#include "FillHole.inl"
// ³ý·â±Õ¿×¶´
int test_FillHole()
{
  img_t im[10] = {0};
  img_t* im1 = im + 1;
  sys_chdir("E:/pub/bin/img2");
  imread("red_cells.png", 1, 1, im);
  DoBinaryzation(im, im);
  //imsetsize(im1, 512, 512, 3, 1);
  {
    imclone2(im, im1);
    FillHole(im1, 0);
  }
  imshow(im);
  imshow(im1);
  cvWaitKey(-1);
  imfrees(im, 10);
  return 0;
}

