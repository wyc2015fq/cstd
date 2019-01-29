
//#include "fractal.h"

//#include "std/log_c.h"
#include "std/rand_c.h"
#include "std/img_c.h"
#include "std/gui_c.h"
#include "std/gdi_c.h"
#include "std/imdraw.h"
#include "std/region_c.h"
#include "std/stb_ttffont.h"
#include "utime.h"
//#include "ui/window.inl"
//#include "draw/imdraw.inl"
//#include "draw/draw.inl"
//#include "fractal.inl"
char wndname[] = "Drawing Demo";

COLOR rgb(double r, double g, double b) {
  return _RGB((int)r*255, (int)g * 255, (int)b * 255);
}

DWORD random_color(rng_t* rng)
{
  int icolor = rng_int32(rng);
  return _RGB(icolor & 255, (icolor >> 8) & 255, (icolor >> 16) & 255);
}

int randpt(rng_t* rng, int n, IPOINT* pt, int xmin, int xmax, int ymin, int ymax)
{
  int i;

  for (i = 0; i < n; ++i) {
    pt[i].x = rng_uniform(rng, xmin, xmax);
    pt[i].y = rng_uniform(rng, ymin, ymax);
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
  mt19937ar_t mt19937ar[1];
  rng_t* rng = rng_mt19937_init(mt19937ar, 11);
  imsetsize(im, 800, 1000, 3, 1);
  for (; frame < 1; frame++) {
    int h = im->h, w = im->w;
    rng = rng_mt19937_init(mt19937ar, 123);
    imfill(im, rgb(1, 1, 1));
    brush_t a;
    if (0) {
      {
        utime_start(_start_time);
        for (i = 0; i < 1; ++i) {
          //imdraw(im, "wline:i;solidfill:i;solidline:i;circle:i;", wline, clr, clr1, im->w/2, im->h/2, im->h/2);
          //icvDrawCircle(im, im->w/2, im->h/2, im->w/2, _RGB(255, 0, 0), 1);
          imdraw_ellipse(im, false, NULL, im->w / 2, im->h / 2, im->w / 3, im->h / 3, 0, 360, _RGB(255, 0, 0), _RGB(0, 255, 0), 5);
          //imshow(im);cvWaitKey(-1);
          //imdraw_eee1(im, 0, 0, im->w, im->h, _RGB(255, 0, 0));
        }
        printf("%f\n", utime_elapsed(_start_time));
      }
      imshow_(im);
      waitkey(-1);
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
        randpt(rng,1, pts, -im->w, 2 * im->w, -im->h, 2 * im->h);
        clr = random_color(rng);
        clr1 = random_color(rng);
        wline = rng_int32(rng) % 10 - 1;
        r = rng_int32(rng) % 300;
        //imdraw(im, "wline:i;solidfill:i;solidline:i;circle:i;", wline, clr, clr1, pts[0].x, pts[0].y, r);
      }

      printf("%.4f ", utime_elapsed(_start_time));
    }

    if (0) {
      utime_start(_start_time);

      for (j = 0; j < 100; ++j) {
        wline = rng_int32(rng) % 10;
        randpt(rng,2, pts, -im->w, 2 * im->w, -im->h, 2 * im->h);
        clr = random_color(rng);
        clr = _RGB(255, 0, 0);
        imdraw_line(im, NULL, pts[0], pts[1], clr, wline);
      }

      printf("%.4f ", utime_elapsed(_start_time));
    }

    if (0) {
      utime_start(_start_time);

      for (j = 0; j < 100; ++j) {
        wline = rng_int32(rng) % 10;
        randpt(rng,2, pts, -im->w, 2 * im->w, -im->h, 2 * im->h);
        clr = random_color(rng);
        //drawaa_strokeline(hdc, pts[0].x, pts[0].y, pts[1].x, pts[1].y, clr, wline);
      }

      printf("%.4f ", utime_elapsed(_start_time));
    }

    if (0) {
      utime_start(_start_time);

      for (j = 0; j < 100; ++j) {
        wline = rng_int32(rng) % 10;
        randpt(rng,2, pts, -im->w, 2 * im->w, -im->h, 2 * im->h);
        clr = random_color(rng);
        wline = rng_int32(rng) % 10 + 1;
        //imdraw_rect(im, pts[0].x, pts[0].y, pts[1].x, pts[1].y, random_color(rng), wline);
      }

      printf("%.4f ", utime_elapsed(_start_time));
    }

    if (0) {
      utime_start(_start_time);

      for (j = 0; j < 100; ++j) {
        double angle;
        wline = rng_int32(rng) % 10;
        randpt(rng,1, pts, -im->w, 2 * im->w, -im->h, 2 * im->h);
        //randpt(rng,1, pts, 0, w, 0, h);
        randpt(rng,1, pts + 1, 0, 200, 0, 200);
        clr = random_color(rng);
        wline = rng_int32(rng) % 10 - 1;
        wline = -1;
        angle = (rng_int32(rng) % 1000) * 0.180;
        //imdraw_ellipse(im, pts[0].x, pts[0].y, pts[1].y, pts[1].x, angle, angle - 100, angle + 200, clr, wline);
      }

      printf("%.4f ", utime_elapsed(_start_time));
    }

    if (0) {
      utime_start(_start_time);

      for (j = 0; j < 100; ++j) {
        int arr[2];
        arr[0] = randpt(rng,3, pts, -w, 2 * w, -h, 2 * h);
        arr[1] = randpt(rng,3, pts + 3, -w, 2 * w, -h, 2 * h);
        clr = random_color(rng);
        wline = rng_int32(rng) % 10;
        //imdraw_poly(im, pts, arr, 2, 1, random_color(rng), wline);
      }

      printf("%.4f ", utime_elapsed(_start_time));
    }

    if (0) {
      utime_start(_start_time);

      for (j = 0; j < 100; ++j) {
        //CvFont font[1] = {0};
        double hscale = (rng_int32(rng) % 100) * 0.05 + 0.1, vscale = (rng_int32(rng) % 100) * 0.05 + 0.1, shear = (rng_int32(rng) % 5) * 0.1;
        //CvSize text_size;
        wline = rng_int32(rng) % 10;
        randpt(rng,1, pts, 0, w, 0, h);
        clr = random_color(rng);
        wline = rng_int32(rng) % 10 - 1;
        wline = MAX(1, (int)hscale);
        vscale = hscale;
        shear = 0;
        //cvInitFont(font, rng_int32(rng) % 8, hscale, vscale, shear, wline);
        //imdraw_text_v(im, "Testing text rendering!", pts[0].x, pts[0].y, font, clr);
      }

      printf("%.4f ", utime_elapsed(_start_time));
    }

    if (1) {
      //CvFont font[1] = {0};
      char buf[256] = {0};
      DWORD bgclr;
      const char* astr = "我";// "我gf体结构中abcdefghijklmnopqrstuvwxyz 0123456789 ABCDEFGHIJKLMNOPQRSTUVWXYZ";
      int alen = strlen(astr);
      double hscale = (rng_int32(rng) % 100) * 0.05 + 0.1, vscale = (rng_int32(rng) % 100) * 0.05 + 0.1, shear = (rng_int32(rng) % 5) * 0.1;
      //CvSize text_size;
      wline = rng_int32(rng) % 10;
      randpt(rng, 1, pts, 0, w, 0, h);
      randpt(rng, 1, pts, 100, w - 100, 100, h - 100);
      clr = random_color(rng);
      clr = rgb(0, 0, 0);
      bgclr = ~clr;
      wline = rng_int32(rng) % 10 - 1;
      wline = MAX(1, (int)hscale);
      vscale = hscale;
      shear = 0;
      randstr_cn(rng, buf, countof(buf));
      memcpy(buf + 0, astr, alen);
      bool flip_y = false;
      const char* fontfile = "D:/code/git/cstd/include/ocr/train/chinese_fonts/huawenxihei.ttf";
      stb_ttffont_t ttffont[1] = {0};
      font_t* font = stb_ttffont_init(ttffont, fontfile);
      //randstr_cn(buf, countof(buf));
      //for (j = 0; j < 100; ++j)
      if (1) {
        utime_start(_start_time);
        int x, y, ww = 200, k = 0;

        for (y = 0; y < h; y += ww) {
          for (x = 0; x < w; x += ww) {
            clr = random_color(rng);
            bgclr = ~clr;
            IRECT rc = iRECT(x, y, x + ww, y + ww);
            IRECT rc2 = iRECT(x + 2, y + 2, x + ww - 2, y + ww - 2);
            //imdraw_rect(im, flip_y, NULL, rc, rgb(1, 1, 1), 0, 1);
            imdraw_text(im, flip_y, NULL, buf, rc2, clr, 0, font, 32);
          }
        }

        //y+=12;
        printf("%.4f ", utime_elapsed(_start_time));
      }
      font->fun->Release(font);
    }


    printf("\n");
    imshow_(im);
    ch = waitkey(-1);

    if ('[' == ch || ']' == ch) {
      r = BOUND(r + ('[' == ch ? -1 : 1), 1, 50);
    }
  }
  // Wait for a key stroke; the same function arranges events processing
  imfree(im);
  //cvDestroyAllWindows();
  return 0;
}


