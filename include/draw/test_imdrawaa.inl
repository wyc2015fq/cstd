
#include "draw/imdraw.inl"
#include "ui/window.inl"

int rand_dpt(int n, DPOINT* pt, int xmin, int xmax, int ymin, int ymax)
{
  int i;

  for (i = 0; i < n; ++i) {
    pt[i].x = rand_range(xmin, xmax);
    pt[i].y = rand_range(ymin, ymax);
  }

  return n;
}

int test_imdrawaa_poly()
{
  int i, j;
  DPOINT pts[100];
  int ymin = 0;
  img_t im[1] = {0};
  imsetsize(im, 800, 1000, 3, 1);

  cvNamedWindow("im", 1);

  for (i = 0; i < 100; i++) {
    int w = im->w, h = im->h, step = im->s, cn = im->c, wline;
    uchar* image = im->tt.data;
    DWORD clr1, clr2;
    DDC hdc[1] = {0};
    cvRNG(123);
    imzero(im);
    HDDCSET_IMAGE(hdc, im);

    if (1) {
      utime_start(_start_time);

      for (j = 0; j < 100; ++j) {
        wline = cvRandInt() % 10;
        rand_dpt(2, pts, -w, 2 * w, -h, 2 * h);
        clr1 = random_color(), clr2 = random_color();
        //clr1 = CC_RGB(255, 0, 0);
        imdrawaa_line(h, w, image, step, cn, pts[0].x, pts[0].y, pts[1].x, pts[1].y, clr1, wline);
      }

      printf("%.4f ", utime_elapsed(_start_time));
    }

    if (0) {
      utime_start(_start_time);

      for (j = 0; j < 100; ++j) {
        wline = cvRandInt() % 10;
        rand_dpt(2, pts, -w, 2 * w, -h, 2 * h);
        clr1 = random_color(), clr2 = random_color();
        clr1 = rgb(1, 0, 0), clr2 = rgb(1, 0, 0);
        wline = cvRandInt() % 10 - 1;
        imdrawaa_rect(h, w, image, step, cn, pts[0].x, pts[0].y, pts[1].x, pts[1].y, clr1, clr2, wline);
      }

      printf("%.4f ", utime_elapsed(_start_time));
    }

    if (0) {
      utime_start(_start_time);

      for (j = 0; j < 100; ++j) {
        double angle;
        wline = cvRandInt() % 10;
        rand_dpt(1, pts, -w, 2 * w, -h, 2 * h);
        //rand_dpt(1, pts, 0, w, 0, h);
        rand_dpt(1, pts + 1, 0, 200, 0, 200);
        clr1 = random_color(), clr2 = random_color();
        wline = cvRandInt() % 10 - 1;
        wline = -1;
        angle = (cvRandInt() % 1000) * 0.180;
        imdrawaa_ellipse(h, w, image, step, cn, pts[0].x, pts[0].y, pts[1].x, pts[1].y, clr1, clr2, wline);
      }

      printf("%.4f ", utime_elapsed(_start_time));
    }

    if (0) {
      utime_start(_start_time);

      for (j = 0; j < 100; ++j) {
        int arr;
        arr = rand_dpt(3, pts, -w, 2 * w, -h, 2 * h);
        clr1 = random_color(), clr2 = random_color();
        wline = cvRandInt() % 10;
        imdrawaa_poly(h, w, image, step, cn, pts, arr, 1, clr1, clr2, wline);
      }

      printf("%.4f ", utime_elapsed(_start_time));
    }

    if (0) {
      utime_start(_start_time);

      for (j = 0; j < 100; ++j) {
        CvFont font[1] = {0};
        double hscale = (cvRandInt() % 100) * 0.05 + 0.1, vscale = (cvRandInt() % 100) * 0.05 + 0.1, shear = (cvRandInt() % 5) * 0.1;
        //CvSize text_size;
        wline = cvRandInt() % 10;
        rand_dpt(1, pts, 0, w, 0, h);
        clr1 = random_color(), clr2 = random_color();
        wline = cvRandInt() % 10 - 1;
        wline = MAX(1, (int)hscale);
        vscale = hscale;
        shear = 0;
        cvInitFont(font, cvRandInt() % 8, hscale, vscale, shear, wline);
        //imdraw_text_v(h, w, image, step, cn, "Testing text rendering!", pts[0].x, pts[0].y, font, clr1);
      }

      printf("%.4f ", utime_elapsed(_start_time));
    }

    printf("\n");
    imshow(im);
    cvWaitKey(10);
  }

  // Wait for a key stroke; the same function arranges events processing
  imfree(im);
  cvDestroyAllWindows();
  return 0;
}
