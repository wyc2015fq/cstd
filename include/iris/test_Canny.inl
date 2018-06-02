
#include "img\Canny.inl"
#include "cv\cvcanny.inl"
#include "img\blur_guass.inl"
#include "img\blur.inl"

int stack_blur(const img_t* src, img_t* dst, unsigned rx, unsigned ry)
{
  imsetsamesize(dst, src);

  switch (src->c) {
  case 1:
    stack_blur_cn1(src, dst, rx, ry);
    break;

  default:
    break;
  }

  return 0;
}

int imblur_guass(const img_t* im, img_t* im2, double sigma)
{
  imsetsamesize(im2, im);
  //Canny(im->h, im->w, im->data, im2->data, sigma, dRatioLow, dRatioHigh);
  blur_guass(im->h, im->w, im->tt.data, im->s, im2->tt.data, im2->s, sigma);
  return 0;
}

int imcanny(const img_t* im, img_t* im2, double sigma, double dRatioLow, double dRatioHigh)
{
  img_t im3[1] = {0};
  imsetsamesize(im2, im);
  //imblur_guass(im, im3, sigma);
  {
    utime_start(_start_time);
    stack_blur(im, im3, (int)(sigma * 3), (int)(sigma * 3));
    printf(" %.4f ", utime_elapsed(_start_time));
  }
  //Canny(im->h, im->w, im->data, im2->data, sigma, dRatioLow, dRatioHigh);
  //cvCanny(im->h, im->w, im3->data, im3->s, im2->data, im2->s, (int)dRatioLow, (int)dRatioHigh);
  imfree(im3);
  return 0;
}

int test_canny()
{
  img_t im[1] = {0};
  img_t im2[1] = {0};
  _chdir("D:/code/cstd/src/iris/total_iris_detection_7_21");
  imread("aaa.jpg", 1, 1, im);
  imread("Fig1006(a)(building).BMP", 1, 1, im);
  {
    utime_start(_start_time);
    imcanny(im, im2, 3, 10, 40);
    printf(" %.4f\n", utime_elapsed(_start_time));
  }
  //imblur_guass(im, im2, 20);
  imshow(im);
  imshow(im2);
  cvWaitKey(-1);
  imfree(im);
  imfree(im2);
  return 0;
}