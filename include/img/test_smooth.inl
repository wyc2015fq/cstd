
#include "cv/cvfilter.inl"
#include "cv/cvsmooth.inl"
#include "cv/cvmorph.inl"
#include "cv/cvderiv.inl"
#include "cv/cxconvert.inl"
//#include "cv2/smooth.inl"

int test_smooth()
{
  /// Global Variables
  int DELAY_CAPTION = 1500;
  int DELAY_BLUR = 100;
  int MAX_KERNEL_LENGTH = 200;

  img_t im[10] = {0};
  img_t* im1 = im + 1;
  img_t* im2 = im + 2;
  img_t* im3 = im + 3;
  img_t* im4 = im + 4;
  int bdtype = 1;
  int ksize = 5;
  int shape = 1;
  int ch = 0;

  char name[] = "Filter Demo 1";

  _chdir("D:/pub/pic");

  /// Load the source image
  imread("lena512.jpg", 4, 1, im);

  imsetsamesize(im1, im);
  imsetsamesize(im2, im);
  imsetsamesize(im3, im);

  cvNamedWindow(name, 0);
  cvCreateTrackbar("bdtype", name, &bdtype, 3, 0);
  cvCreateTrackbar("shape MORPH_RECT=0, MORPH_CROSS=1, MORPH_ELLIPSE=2 ", name, &shape, 3, 0);
  cvCreateTrackbar("ksize", name, &ksize, MAX_KERNEL_LENGTH, 0);

  for (; 'q' != ch;) {
    {
      IPOINT pt = iPOINT(ksize, ksize);
      CvSize ksz = cvSize(ksize * 2 + 1, ksize * 2 + 1);
      CvSize sz = cvSize(im->w, im->h);
      img_t* im0 = im1;

      if (1) {
        {
          utime_start(_start_time);
          cvBoxFilter_8u(im->tt.data, im->s, im0->tt.data, im0->s, im->c, sz, ksz, pt, bdtype);
          printf("%.4f ", utime_elapsed(_start_time));
          ++im0;
        }
        {
          utime_start(_start_time);
          cvGaussianBlur_8u(im->tt.data, im->s, im0->tt.data, im0->s, im->c, sz, ksize, 0, bdtype);
          printf("%.4f ", utime_elapsed(_start_time));
          ++im0;
        }

        if (0) {
          utime_start(_start_time);
          //cvMedianBlur_8u(im->tt.data, im->s, im2->tt.data, im2->s, im->c, sz, ksz, pt, bdtype);
          icvMedianBlur_8u_CnR(im->tt.data, im->s, im0->tt.data, im0->s, sz, ksz.h, im->c);
          printf("%.4f ", utime_elapsed(_start_time));
          ++im0;
        }

        if (0) {
          utime_start(_start_time);
          //cvMedianBlur_8u(im->tt.data, im->s, im2->tt.data, im2->s, im->c, sz, ksz, pt, bdtype);
          icvBilateralFiltering_8u_CnR(im->tt.data, im->s, im0->tt.data, im0->s, sz, ksz.h, ksz.h / 2, im->c);
          printf("%.4f ", utime_elapsed(_start_time));
          ++im0;
        }
      }
      else {
        if (0) {
          utime_start(_start_time);
          cvErode_8u(im->tt.data, im->s, im0->tt.data, im0->s, im->c, sz, ksz, shape, pt, bdtype);
          printf("%.4f ", utime_elapsed(_start_time));
          ++im0;
        }

        if (0) {
          utime_start(_start_time);
          cvDilate_8u(im->tt.data, im->s, im0->tt.data, im0->s, im->c, sz, ksz, shape, pt, bdtype);
          printf("%.4f ", utime_elapsed(_start_time));
          ++im0;
        }

        {
          img_t tmpim[1] = {0};
          utime_start(_start_time);
          cvSobelAbs_8u(im->tt.data, im->s, im0->tt.data, im0->s, im->c, sz, bdtype);
          printf("%.4f ", utime_elapsed(_start_time));
          ++im0;
        }

        {
          img_t tmpim[1] = {0};
          utime_start(_start_time);
          cvLaplaceAbs_8u(im->tt.data, im->s, im0->tt.data, im0->s, im->c, sz, bdtype);
          printf("%.4f ", utime_elapsed(_start_time));
          ++im0;
        }
      }
    }
    imcat(2, 2, im, im4);
    imshow2(name, im4);
    ch = cvWaitKey(50);
    printf("\n");
  }

  freeims(im, 3);

#if 0

  /// Applying Gaussian blur
  if (display_caption("Gaussian Blur") != 0) {
    return 0;
  }

  for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2) {
    GaussianBlur(im, im1, Size(i, i), 0, 0);

    if (display_dst(DELAY_BLUR) != 0) {
      return 0;
    }
  }

  /// Applying Median blur
  if (display_caption("Median Blur") != 0) {
    return 0;
  }

  for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2) {
    bilateralFilter(im, im1, i, i * 2, i / 2);

    if (display_dst(DELAY_BLUR) != 0) {
      return 0;
    }
  }

  /// Wait until user press a key
  display_caption("End: Press a key!");

#endif
  cvWaitKey(0);
  return 0;
}

