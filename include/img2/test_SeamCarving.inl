#include "SeamCarving.inl"
int test_SeamCarving()
{
  img_t im[10] = {0};
  img_t* im1 = im + 1;
  sys_chdir("E:/pub/bin/img2");
  imread("christmas_original.jpg", 3, 1, im);
  //imsetsize(im1, 512, 512, 3, 1);
  {
    imclone2(im, im1);
    SeamCarving(im, 100);
  }
  imshow(im);
  imshow(im1);
  cvWaitKey(-1);
  imfrees(im, 10);
  return 0;
}

