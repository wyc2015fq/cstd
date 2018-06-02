
#include <stdlib.h>
#include <string.h>

#ifndef _CSTD_H_

typedef struct img_t {
  //IMGHEADFEILD;
  int w, h, s, f;
  short c, t;
  union {
    unsigned char* data;
  } tt;
}
img_t;
#define CC_INLINE static
typedef struct IRECT {
  int l, t, r, b;
} IRECT;
typedef unsigned int COLOR;
#define RCSET(prc, L, T, R, B) ((prc)->l=L, (prc)->t=T, (prc)->r=R, (prc)->b=B)
CC_INLINE IRECT iRECT(int l, int t, int r, int b)
{
  IRECT rc;
  RCSET(&rc, l, t, r, b);
  return rc;
}
CC_INLINE int iPtInRect(const IRECT* rc, int x, int y)
{
  return (rc->l <= x && x < rc->r) && (rc->t <= y && y < rc->b);
}
CC_INLINE int iRectNormalize(IRECT* prc)
{
  int nTemp;

  if (prc->l > prc->r) {
    nTemp = prc->l;
    prc->l = prc->r;
    prc->r = nTemp;
  }

  if (prc->t > prc->b) {
    nTemp = prc->t;
    prc->t = prc->b;
    prc->b = nTemp;
  }

  return 0;
}
CC_INLINE IRECT iRectInter(IRECT r1, IRECT r2)
{
  IRECT temp = {0};
  iRectNormalize(&r1);
  iRectNormalize(&r2);

  // check for total exclusion
  if ((r1.r > r2.l) && (r1.l < r2.r) && (r1.b > r2.t) && (r1.t < r2.b)) {
    temp.l = (r1.l > r2.l) ? r1.l : r2.l;
    temp.r = (r1.r < r2.r) ? r1.r : r2.r;
    temp.t = (r1.t > r2.t) ? r1.t : r2.t;
    temp.b = (r1.b < r2.b) ? r1.b : r2.b;
  }

  return temp;
}
CC_INLINE IRECT iRectClip(const IRECT* pclip, int l, int t, int r, int b)
{
  IRECT rc = iRECT(l, t, r, b);

  if (pclip) {
    rc = iRectInter(rc, *pclip);
  }

  return rc;
}
#endif // 

#define PUTPT(X, Y)  if (iPtInRect(pclip, X, Y)) { memcpy(im->tt.data + (Y)*im->s + (X)*im->c, &clr, im->c); }

// 画八个点
int CirclePlot(img_t* im, const IRECT* pclip, int xc, int yc, int x, int y, COLOR clr)
{
  PUTPT(xc + x, yc + y);
  PUTPT(xc - x, yc + y);
  PUTPT(xc + x, yc - y);
  PUTPT(xc - x, yc - y);

  PUTPT(xc + y, yc + x);
  PUTPT(xc - y, yc + x);
  PUTPT(xc + y, yc - x);
  PUTPT(xc - y, yc - x);
  return 0;
}
#undef PUTPT
// 中点画圆法
int mp_circle(img_t* im, const IRECT* pclip, int xc , int yc , int r, COLOR clr)
{
  int x, y;
  double d;
  IRECT rc1 = iRectClip(pclip, 0, 0, im->w, im->h);
  x = 0;
  y = r;
  d = 1.25 - r;
  CirclePlot(im, &rc1, xc , yc , x , y, clr);

  while (x < y) {
    if (d < 0) {
      d = d + 2 * x + 3;
    }
    else {
      d = d + 2 * (x - y) + 5;
      y--;
    }

    x++;
    CirclePlot(im, &rc1, xc , yc , x , y, clr);
  }

  return 0;
}
// 改进的中点画圆法－Bresenham算法
int bresenham_circle(img_t* im, const IRECT* pclip, int xc , int yc , int r, COLOR clr)
{
  int x, y, d;
  IRECT rc1 = iRectClip(pclip, 0, 0, im->w, im->h);
  x = 0;
  y = r;
  d = 3 - 2 * r;
  CirclePlot(im, &rc1, xc , yc , x , y, clr);

  while (x < y) {
    if (d < 0) {
      d = d + 4 * x + 6;
    }
    else {
      d = d + 4 * (x - y) + 10;
      y--;
    }

    x++;
    CirclePlot(im, &rc1, xc , yc , x , y, clr);
  }

  return 0;
}

//  正负判定画圆法
int pnar_circle(img_t* im, const IRECT* pclip, int xc, int yc, int r, COLOR clr)
{
  int x, y, f;
  IRECT rc1 = iRectClip(pclip, 0, 0, im->w, im->h);
  x = 0;
  y = r;
  f = 0;

  while (x <= y) {
    CirclePlot(im, &rc1, xc, yc, x, y, clr);

    if (f <= 0) {
      f = f + 2 * x + 1;
      x++;
    }
    else {
      f = f - 2 * y + 1;
      y--;
    }
  }

  return 0;
}
// 快速画圆法
int fast_circle(img_t* im, const IRECT* pclip, int xc , int yc , int r, COLOR clr)
{
  int x, y, d;
  IRECT rc1 = iRectClip(pclip, 0, 0, im->w, im->h);
  x = 0;
  y = r;
  d = -r / 2;
  CirclePlot(im, &rc1, xc , yc , x , y, clr);

  if (r % 2 == 0) {
    while (x < y) {
      x++;

      if (d < 0) {
        d += x;
      }
      else {
        y--;
        d += x - y;
      }

      CirclePlot(im, &rc1, xc , yc , x , y, clr);
    }
  }
  else {
    while (x < y) {
      x++;

      if (d < 0) {
        d += x + 1;
      }
      else {
        y--;
        d += x - y + 1;
      }

      CirclePlot(im, &rc1, xc , yc , x , y, clr);
    }
  }

  return 0;
}

#include "libfacerecog.h"
#include "iris/iris.h"

typedef unsigned char uchar;

#define IMINIT(IM, H, W, A, S, C, F)  ((IM)->h=H,(IM)->w=W,(IM)->tt.data=(uchar*)(A),(IM)->s=S,(IM)->c=(short)(C),(IM)->f=F)

int DrawIris(img_t* im, const IRECT* pclip, const irisrecog_t* ir, unsigned long color0, int isflip, int wline)
{
  int i, x1 = ir->rc.x, y1 = ir->rc.y;
  int nrr[3] = {0};
  int wrr[3] = {0};
  double urr[3] = {0};
  double drr[3] = {0};
  double shift1 = 1 << IRIS_SHIFT1;
  double shift2 = 1 << IRIS_SHIFT2;

  for (i = 0; i < 3; ++i) {
    nrr[i] = (int)((ir->in[i]) / shift1);
    wrr[i] = (int)((ir->iw[i]) / shift1);
    urr[i] = ((ir->iu[i]) / shift2);
    drr[i] = ((ir->id[i]) / shift2);
  }

  if (isflip) {
    img_t im2[1] = {0};
    unsigned char* data = im->tt.data + (im->h - 1) * im->s;
    IMINIT(im2, im->h, im->w, data, -im->s, im->c, 1);
    im = im2;
  }

  //DrawRectangle2(im, ir->rc, color0, wline);
  //imdraw_ploy(im, 3, urr, x1, y1, color0, wline);
  //imdraw_ploy(im, 3, drr, x1, y1, color0, wline);
  mp_circle(im, pclip, x1 + wrr[0], y1 + wrr[1], wrr[2], color0);
  mp_circle(im, pclip, x1 + wrr[0], y1 + wrr[1], wrr[2] - 1, color0);
  mp_circle(im, pclip, x1 + nrr[0], y1 + nrr[1], nrr[2], color0);
  mp_circle(im, pclip, x1 + nrr[0], y1 + nrr[1], nrr[2] - 1, color0);
  //CvFont font1[1]={0};
  //cvInitFont(font1, CC_FONT_HERSHEY_SIMPLEX, 10, 10, 0, 3);
  //imdraw_text(im, iRECT3(ir->rc, ir->rc), s, strlen(s), zimo_songti_12, CC_RGB(255, 255, 255), 0, TT_LEFT);
  //imdraw_text_v(im, s, ir->rc.x, ir->rc.y, font1, CC_RGB(255, 255, 255));

  return 0;
}

#ifdef _CSTD_H_
int test_draw_circle()
{
  img_t im[2] = {0};
  img_t* im2 = im + 1;
  imsetsize(im2, 600, 800, 3, 1);

  for (; 1;) {
    imdraw_fill(im2, _rgb(1, 1, 1));
    imcolorcvt(im2, im, T_BGR, T_YUYV);
    mp_circle(im, 0, 200, 200, 300, 0xff00ff00);
    mp_circle(im, 0, 300, 300, 100, 0xff00ff00);
    mp_circle(im, 0, 300, 300, 99, 0xff00ff00);
    imcolorcvt(im, im2, T_YUYV, T_BGR);
    imshow(im2);
    cvWaitKey(20);
  }

  imfrees(im, 2);
  return 0;
}
#else
int test_draw_circle()
{
  img_t im[2] = {0};
  img_t* im2 = im + 1;

  for (; 1;) {
    mp_circle(im, 0, 200, 200, 300, 0xff00ff00);
    mp_circle(im, 0, 300, 300, 100, 0xff00ff00);
    mp_circle(im, 0, 300, 300, 99, 0xff00ff00);
  }

  return 0;
}

#endif //_CSTD_H_
