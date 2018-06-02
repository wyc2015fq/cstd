#include "TextureTransferTool.inl"
int test_TextureTransfer()
{
  img_t im[10] = {0};
  img_t* im1 = im + 1;
  img_t* im2 = im + 2;
  sys_chdir("E:/pub/bin/img2");
  imread("fire-big_src.jpg", 4, 1, im1);
  imwrite("C:/aaa.bmp", im1);
  imread("norton-big_tgt.jpg", 4, 1, im2);
  imshow(im1);
  imshow(im2);
  cvWaitKey(-1);
  imfrees(im, 10);
  return 0;
}

