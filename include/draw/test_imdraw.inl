
//#include "fractal.h"

#include "log.h"
#include "ui/window.inl"
//#include "draw/imdraw.inl"
//#include "draw/draw.inl"
//#include "fractal.inl"
char wndname[] = "Drawing Demo";
typedef CvScalar CvScalar;
typedef IPOINT IPOINT;
DWORD random_color()
{
  int icolor = cvRandInt();
  return CC_RGB(icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255);
}

int randpt(int n, IPOINT* pt, int xmin, int xmax, int ymin, int ymax)
{
  int i;

  for (i = 0; i < n; ++i) {
    pt[i].x = rand_range(xmin, xmax);
    pt[i].y = rand_range(ymin, ymax);
  }

  return n;
}

int test_imdraw()
{
  int i, j, r = 1, frame = 0;
  char ch = 0;
  IPOINT pts[100];
  int ymin = 0;
  img_t im[1] = {0};
  COLOR clr, clr1;
  int wline;
  imsetsize(im, 801, 1001, 3, 1);

  cvNamedWindow("im", 1);

  for (; frame < 100; frame++) {
    int h = im->h, w = im->w;
    cvRNG(123);
    imdraw_fill(im, rgb(1, 1, 1));

    if (1) {
      {
        utime_start(_start_time);

        for (i = 0; i < 100; ++i) {
          //imdraw(im, "wline:i;solidfill:i;solidline:i;circle:i;", wline, clr, clr1, im->w/2, im->h/2, im->h/2);
          //icvDrawCircle(im, im->w/2, im->h/2, im->w/2, _RGB(255, 0, 0), 1);
          imdraw_ellipse1(im, im->w / 2, im->h / 2, im->w / 2, im->h / 2, 0, 0, 360, _RGB(256, 0, 0), -1);
          //imshow(im);cvWaitKey(-1);
          //imdraw_eee1(im, 0, 0, im->w, im->h, _RGB(255, 0, 0));
        }

        printf("%f\n", utime_elapsed(_start_time));
      }
      //imdraw_fill(im, rgb(1,1,1));
      {
        utime_start(_start_time);

        for (i = 0; i < 100; ++i) {
          //imdraw_ellipse(im, 0, 0, im->w, im->h, _RGB(255, 0, 0), _RGB(0, 0, 255), 2);
        }

        printf("%f\n", utime_elapsed(_start_time));
      }
      imshow(im);
      cvWaitKey(-1);
    }

    if (0) {
      for (i = 10; i < 50; i++) {
        for (j = 10; j < 50; j++) {
          int c = 3;
          int n = i * j * c;
          uchar* img1 = MALLOC(uchar, n * 2);
          uchar* img2 = img1 + n;
          img_t im1[1] = {0};
          img_t im2[1] = {0};
          MEMSET(img1, 255, n * 2);
          IMINIT(im1, i, j, img1, j * c, c, 1);
          IMINIT(im2, i, j, img2, j * c, c, 1);
          //imdraw_eee1(im1, 0, 0, j, i, _RGB(255, 0, 0));
          //imdraw_eee2(im2, 0, 0, j, i, _RGB(255, 0, 0));
          //imshow(im1);imshow(im2);cvWaitKey(-1);
          ASSERT(0 == memcmp(img1, img2, n));
          free(img1);
        }
      }
    }

    if (0) {
      utime_start(_start_time);

      for (j = 0; j < 100; ++j) {
        randpt(1, pts, -im->w, 2 * im->w, -im->h, 2 * im->h);
        clr = random_color();
        clr1 = random_color();
        wline = cvRandInt() % 10 - 1;
        r = cvRandInt() % 300;
        imdraw(im, "wline:i;solidfill:i;solidline:i;circle:i;", wline, clr, clr1, pts[0].x, pts[0].y, r);
      }

      printf("%.4f ", utime_elapsed(_start_time));
    }

    if (0) {
      utime_start(_start_time);

      for (j = 0; j < 100; ++j) {
        wline = cvRandInt() % 10;
        randpt(2, pts, -im->w, 2 * im->w, -im->h, 2 * im->h);
        clr = random_color();
        clr = CC_RGB(255, 0, 0);
        imdraw_line(im, pts[0], pts[1], clr, wline);
      }

      printf("%.4f ", utime_elapsed(_start_time));
    }

    if (0) {
      utime_start(_start_time);

      for (j = 0; j < 100; ++j) {
        wline = cvRandInt() % 10;
        randpt(2, pts, -im->w, 2 * im->w, -im->h, 2 * im->h);
        clr = random_color();
        //drawaa_strokeline(hdc, pts[0].x, pts[0].y, pts[1].x, pts[1].y, clr, wline);
      }

      printf("%.4f ", utime_elapsed(_start_time));
    }

    if (0) {
      utime_start(_start_time);

      for (j = 0; j < 100; ++j) {
        wline = cvRandInt() % 10;
        randpt(2, pts, -im->w, 2 * im->w, -im->h, 2 * im->h);
        clr = random_color();
        wline = cvRandInt() % 10 + 1;
        //imdraw_rect(im, pts[0].x, pts[0].y, pts[1].x, pts[1].y, random_color(), wline);
      }

      printf("%.4f ", utime_elapsed(_start_time));
    }

    if (0) {
      utime_start(_start_time);

      for (j = 0; j < 100; ++j) {
        double angle;
        wline = cvRandInt() % 10;
        randpt(1, pts, -im->w, 2 * im->w, -im->h, 2 * im->h);
        //randpt(1, pts, 0, w, 0, h);
        randpt(1, pts + 1, 0, 200, 0, 200);
        clr = random_color();
        wline = cvRandInt() % 10 - 1;
        wline = -1;
        angle = (cvRandInt() % 1000) * 0.180;
        //imdraw_ellipse(im, pts[0].x, pts[0].y, pts[1].y, pts[1].x, angle, angle - 100, angle + 200, clr, wline);
      }

      printf("%.4f ", utime_elapsed(_start_time));
    }

    if (0) {
      utime_start(_start_time);

      for (j = 0; j < 100; ++j) {
        int arr[2];
        arr[0] = randpt(3, pts, -w, 2 * w, -h, 2 * h);
        arr[1] = randpt(3, pts + 3, -w, 2 * w, -h, 2 * h);
        clr = random_color();
        wline = cvRandInt() % 10;
        //imdraw_poly(im, pts, arr, 2, 1, random_color(), wline);
      }

      printf("%.4f ", utime_elapsed(_start_time));
    }

    if (0) {
      utime_start(_start_time);

      for (j = 0; j < 100; ++j) {
        //CvFont font[1] = {0};
        double hscale = (cvRandInt() % 100) * 0.05 + 0.1, vscale = (cvRandInt() % 100) * 0.05 + 0.1, shear = (cvRandInt() % 5) * 0.1;
        //CvSize text_size;
        wline = cvRandInt() % 10;
        randpt(1, pts, 0, w, 0, h);
        clr = random_color();
        wline = cvRandInt() % 10 - 1;
        wline = MAX(1, (int)hscale);
        vscale = hscale;
        shear = 0;
        //cvInitFont(font, cvRandInt() % 8, hscale, vscale, shear, wline);
        //imdraw_text_v(im, "Testing text rendering!", pts[0].x, pts[0].y, font, clr);
      }

      printf("%.4f ", utime_elapsed(_start_time));
    }

#if 0

    if (0) {
      //CvFont font[1] = {0};
      char buf[256];
      DWORD bgclr;
      const char* astr = "gg(!@#$%^&*(){}[] \n0123456789 \nabcdefghijklmnopqrstuvwxyz \nABCDEFGHIJKLMNOPQRSTUVWXYZ\n";
      int alen = strlen(astr);
      double hscale = (cvRandInt() % 100) * 0.05 + 0.1, vscale = (cvRandInt() % 100) * 0.05 + 0.1, shear = (cvRandInt() % 5) * 0.1;
      //CvSize text_size;
      wline = cvRandInt() % 10;
      randpt(1, pts, 0, w, 0, h);
      randpt(1, pts, 100, w - 100, 100, h - 100);
      clr = random_color();
      clr = rgb(0, 0, 0);
      bgclr = ~clr;
      wline = cvRandInt() % 10 - 1;
      wline = MAX(1, (int)hscale);
      vscale = hscale;
      shear = 0;
      randstr_cn(buf, countof(buf));
      memcpy(buf + 140, astr, alen);
      //randstr_cn(buf, countof(buf));
      //for (j = 0; j < 100; ++j)
      {
        utime_start(_start_time);
        int x, y, ww = 200, k = 0;

        for (y = -50; y < h; y += ww) {
          for (x = -50; x < w; x += ww) {
            clr = random_color();
            bgclr = ~clr;
            imdraw_rect(im, x, y, x + ww, y + ww, rgb(1, 1, 1), 1);
            imdraw_text(im, x + 2, y + 2, x + ww - 2, y + ww - 2, buf, -1, zimo_songti_12, clr, bgclr, TT_WRAPLINE, 0);
          }
        }

        //y+=12;
        printf("%.4f ", utime_elapsed(_start_time));
      }

    }

#endif

    printf("\n");
    imshow(im);
    ch = cvWaitKey(-1);

    if ('[' == ch || ']' == ch) {
      r = BOUND(r + ('[' == ch ? -1 : 1), 1, 50);
    }
  }

  // Wait for a key stroke; the same function arranges events processing
  imfree(im);
  cvDestroyAllWindows();
  return 0;
}


