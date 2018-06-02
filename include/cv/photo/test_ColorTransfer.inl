
#include "ColorTransfer.inl"
//#include "CartoonStylization.inl"
//#include "imagewarp.inl"

#include "img/imgio.inl"

int test_ColorTransfer()  
{
  img_t im[10] = {0};
  int i=0;
  img_t* src = im+i++;
  img_t* tar = im+i++;
  img_t* dst = im+i++;
  sys_chdir("F:/pub/bin/photo/pics");
  imread("0da13f9f299517fe2ed412858d8f8e79.jpg", 3, 1, src);
  imread("5b4e156f7408a6d174219eaa0b44d048.jpg", 3, 1, tar);
  imshow2("src", src);  
  imshow2("tar", tar);
  // µ÷É«
  ColorTransfer(src, tar, dst);  
  imshow2("dst", dst);
  cvWaitKey(-1);  
  imfrees(im, 10);
  return 0;  
}  
