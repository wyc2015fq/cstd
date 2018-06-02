#include "rtcprgb2gray.inl"
int test_rtcprgb2gray()
{
  img_t im[10] = {0};
  img_t* im1 = im + 1;
  sys_chdir("E:/pub/bin/img2");
  imread("18150856-14d1f40231404218bc0a538c28d9cf6d.png", 4, 1, im);
  imshow(im);
  rtcprgb2gray(im, im1);
  cvWaitKey(-1);
  imfrees(im, 10);
  return 0;
}

