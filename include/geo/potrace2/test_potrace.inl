
#include "cstd.h"
#include "img/imgopt.inl"

#include "potrace.inl"

DPOINT2 pt_mul(DPOINT2 pt, double x)
{
  return dPOINT(pt.x * x, pt.y * x);
}


#include "ui/window.inl"

int test_potrace()
{
  img_t im[1] = {0};
  img_t im2[1] = {0};
  img_t bb[1] = {0};
  int i, j, k;
  potrace_state_t st[1] = {0};
  potrace_param_t Potrace[1] = {0};
  potrace_path_t* path = 0;
  double factor = 2;
  _chdir("D:/pub/bin/potrace");
  imread("BEA_logo_Large.jpg", 1, 1, im);
  im2bw(im, 100, bb);
  potrace_param_default(Potrace);
  {
    potrace_bitmap_t bm[1] = {0};
    int dy = (im->s + BM_WORDBITS - 1) / BM_WORDBITS;
    Color32 pal[256] = {0};
    bm->map = im->tt.data;
    bm->h = im->h, bm->w = im->w;
    bm->dy = dy;
    bm->map = MALLOC(potrace_word, im->h * dy);
    //memcpy2d(bm->map, dy*4, im->tt.data, im->s, im->h, im->w);
    rgb2rgb(bb->h, bb->w, bb->tt.data, bb->s, 8, (uchar*)bm->map, dy * BM_WORDSIZE, 1);

    for (i = 0; i < 256; ++i) {
      pal[i].r = pal[i].g = pal[i].b = i;
      pal[i].a = 255;
    }

    pal[1].r = pal[1].g = pal[1].b = 255;
    //imshow(im);
    //cvWaitKey(-1);
    memset(im->tt.data, 0, im->h * im->w);
    {
      img_t im3[1] = {0};
      IMINIT(im3, im->h, im->w, (uchar*)bm->map, dy * BM_WORDSIZE, 1, 1);
      imsavebmpfile("adsf.bmp", im3, 1, pal);
    }
    idx2rgb(im->h, im->w, (uchar*)bm->map, dy * BM_WORDSIZE, 1, im->tt.data, im->s, 8, pal);
    //imshow(im);    cvWaitKey(-1);

    {
      utime_start(_start_time);
      potrace_trace(Potrace, bm, st);
      printf("%f\n", utime_elapsed(_start_time));
    }

    FREE(bm->map);
  }

  imsetsize(im2, 1000, 1000, 3, 1);
  path = st->plist;

  if (1) {
    int n = 0, nn = 0;
    int len[100] = {0};
    DPOINT2 pt[10000];

    for (i = 0; path; i++, path = path->next) {
      int j = 0, n0 = n;
      DPOINT2 A = pt_mul(path->curve.c[path->curve.n - 1][2], factor);

      for (k = 0; k < path->curve.n; k++) {
        if (path->curve.tag[k] == POTRACE_CURVETO) {
          int nn0 = n;
          pt[n++] = A;
          pt[n++] = pt_mul(path->curve.c[k][0], factor);
          pt[n++] = pt_mul(path->curve.c[k][1], factor);
          pt[n++] = pt_mul(path->curve.c[k][2], factor);

          if (1) {
            n = nn0;
            n += vcgen_curve(4, pt + n, pt + n);
          }
        }
        else {
          //n = vcgen_curve(3, path->curve.c[k], pt);
          //pt[n++] = path->curve.c[k][0];
          pt[n++] = pt_mul(path->curve.c[k][1], factor);
          pt[n++] = pt_mul(path->curve.c[k][2], factor);
        }

        ASSERT(n < countof(pt));
        A = pt[n - 1];
      }

#define VECREV(_N, _F, _T)   {_T t; int _i=0; for (;_i<(_N)/2; ++_i) {t=(_F)[_i], (_F)[_i]=(_F)[_N-1-_i], (_F)[_N-1-_i]=t; } }
      //if (i>0)
      len[nn++] = n - n0;

      if (nn > 100) {
        for (j = n0; j < n; ++j) {
          pt[j - n0] = pt[j];
          printf("%.f %.f\n", pt[j].x, pt[j].y);
        }

        //len[0] = n-n0;
        //nn=1;
        break;
      }
    }

    imdrawaa_poly_solid(im2, pt, len, nn, rgba(1, 1, 1, 0.5));
  }

  if (0) {
    DPOINT2 pt[10];
    path = st->plist;

    for (i = 0; path; i++, path = path->next) {
      for (k = 0; k < path->curve.n; k++) {
        int n = 0;
        COLOR clr[] = {Red, White, Yellow};

        if (path->curve.tag[k] == POTRACE_CURVETO) {
          pt[n++] = pt_mul(path->curve.c[k][0], factor);
          pt[n++] = pt_mul(path->curve.c[k][1], factor);
          pt[n++] = pt_mul(path->curve.c[k][2], factor);
        }
        else {
          pt[n++] = pt_mul(path->curve.c[k][1], factor);
          pt[n++] = pt_mul(path->curve.c[k][2], factor);
        }

        for (j = 0; j < n; ++j) {
          double x = (pt[j].x) - 1.5, y = (pt[j].y) - 1.5;
          imdrawaa_rect(im2, x, y, x + 3, y + 3, clr[j], 0, 0);
        }
      }
    }
  }

  imshow(im);
  imshow(im2);
  cvWaitKey(-1);
  potrace_state_free(st);
  return 0;
}



