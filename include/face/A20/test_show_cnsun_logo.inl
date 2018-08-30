
#include "show_cnsun_logo.inl"

#ifdef TEST
int test_show_cnsun_logo() {
  img_t im[2] = {0};
  img_t* im1 = im+1;
  imread("D:/code/cstd/include/face/A20/cnsun-logo.jpg", 1, 1, im1);
  //imshow(im1);cvWaitKey(-1);
  imresize_1(im1, cnsun_logo_width/4, cnsun_logo_width, im1);
  //imshow(im1);cvWaitKey(-1);
  savedata_inl("D:/code/cstd/include/face/A20/cnsun-logo.txt", im1->tt.data, im1->h*im1->w);
  imfree(im1);
  imsetsize(im, 600, 800, 3, 1);
  show_cnsun_logo(im);
  imshow(im);
  cvWaitKey(-1);
  imfrees(im, 2);
  return 0;
}
#endif // TEST