
#include "img/imgopt.inl"
#include "cv/cvsumpixels.inl"

#include "face/_faceDetect.h"
#include "face/cascadearr.h"

static int bf_xrect_partition(buf_t* bf, int total, const XRECT* seq, double threshold, int mincnt, XRECT* outrc)
{
  int i, j = 0, class_cnt = 0;
  typedef XRECT elem_type;
#define IMMALLOC  BF_MALLOC
#define IMFREE    BF_FREE
  int* labels = 0;
  XRECT* rcs = 0;
  BF_MALLOC(bf, labels, total);
#define is_equal_macro(a, b, out)    IS_RECT_EQUAL_COVER_RETE((*(a)), (*(b)), threshold, out)
#include "ml/seqpartition_impl.inl"
  BF_MALLOC(bf, rcs, class_cnt);
  MEMSET(rcs, 0, class_cnt);

  for (i = 0; i < total; ++i) {
    XRECT* r = rcs + labels[i];
    r->x += seq[i].x;
    r->y += seq[i].y;
    r->w += seq[i].w;
    r->h += seq[i].h;
    r->count++;
  }

  for (i = 0; i < class_cnt; ++i) {
    XRECT* r = rcs + i;

    if (r->count >= mincnt) {
      r->x /= r->count;
      r->y /= r->count;
      r->w /= r->count;
      r->h /= r->count;
      outrc[j++] = *r;
    }
  }

  BF_FREE(bf, rcs, class_cnt);
  BF_FREE(bf, labels, total);
  return j;
}

enum { T_TRAS = 1, T_INVX = 2, T_INVY = 4, T_RT90 = T_TRAS | T_INVX, T_RT180 = T_INVX | T_INVY, T_RT270 = T_TRAS | T_INVY };

CvRect rctrans(int x, int y, int w, int h, int cx, int cy, int flag)
{
  int t;

  if (T_TRAS & flag) {
    CV_SWAP(x, y, t);
    CV_SWAP(w, h, t);
    CV_SWAP(cx, cy, t);
  }

  if (T_INVX & flag) {
    x = cx - w - x;
  }

  if (T_INVY & flag) {
    y = cy - h - y;
  }

  return cvRect(x, y, w, h);
}

#define SETOFFSET3(RC, OUT, area) ( (OUT).p0 = ((RC).y) * al + (RC).x * ai, (OUT).w = (RC).w, (OUT).h = (RC).h * al, area = (RC).w * (RC).h )

int setoffset3(int len, const iHaar* f0, iHaarWork3* p, int al, int ai, int cx, int cy, int flag)
{
  INT ar0 = 0, ar1 = 0;
  iHaarWork3* p3 = p;
  const iHaar* f = f0;
  const iHaar* l = f + len;

  for (; f < l; ++f, ++p3) {
#if 0
    SETOFFSET3(f->ft0, p3->ft0, ar0);
    SETOFFSET3(f->ft1, p3->ft1, ar1);
#else
    CvRect rc0 = rctrans(f->ft0.x, f->ft0.y, f->ft0.w, f->ft0.h, cx, cy, flag);
    CvRect rc1 = rctrans(f->ft1.x, f->ft1.y, f->ft1.w, f->ft1.h, cx, cy, flag);
    SETOFFSET3(rc0, p3->ft0, ar0);
    SETOFFSET3(rc1, p3->ft1, ar1);
#endif
    p3->ft1.dd = (f->ft1.d);
    p3->ft0.dd = (ar0) ? (-(p3->ft1.dd * ar1 / ar0)) : (-p3->ft1.dd);
  }

  return 0;
}

#define RECT_VAL2(_A, _a, _b)       (*(_A) - *(_A + (_a)) - *(_A + (_b)) + *(_A + (_a) + (_b)))

double get_invvar_64f(const int* sum, const int* sqsum, int al, int ai, int area)
{
  double var, var2, var3, farea = area;
  var = (double)RECT_VAL2(sum, al, ai) / farea;
  var2 = (double)RECT_VAL2(sqsum, al, ai) / farea;
  var3 = var2 - var * var;
  return (var3 > 0.) ? 1. / sqrt(var3) : 0;
}
static unsigned short invvar_table[ 2 << 13 ] = {
#include "face/table/invvar_table.inl"
};
#define INVSQRT2(_X)  invvar_table[(unsigned short) (_X)];

int get_invvar_32s(const int* tmpSamp, const int* tmpSamp1, int _AL, int _AI, int area2)
{
  int invvar;
  int a, b, var, a1, b1;
  a1 = RECT_VAL2(tmpSamp, (_AL), (_AI));
  b1 = RECT_VAL2(tmpSamp1, (_AL), (_AI));
  //if (a>(1<<16)) {
  a = (a1 << 5) / area2; //四舍五入
  b = b1 / area2;
  var = b - ((a * a) >> 10);

  //} else {
  //  var = (b - (a * a) / area2) / area2;
  //}
  if (var > (2 << 13)) {
    var = (2 << 13) - 1;
  }

  //ASSERT(var<(2 << 13));
  invvar = INVSQRT2(var);
  return invvar;
}

#define SHIFT_2                     (SHIFT * 2)

// 定点版本
#include "lutpass_32s.inl"

// min_rc_scale   最小矩形框倍数
// ratio          缩放比例
// cover_rate     覆盖率
int boostdetect(buf_t* bf, void** cas, int ncas, const img_t* im, double min_rc_scale, double ratio, double cover_rate, int flag, XRECT* out_rc, int maxoutlen)
{
  int iratio = (int)(ratio * (1 << 8));
  int i, len = 0;
  double scale = 1;
  int h2 = (int)(im->h / min_rc_scale + 0.5);
  int w2 = (int)(im->w / min_rc_scale + 0.5);
  img_t im2[1] = {0};
  iHaarWork3* pWirk[4] = {0};
  iStage* pStage[4] = {0};
  iHaar* pHaar[4] = {0};
  int cx, cy, area2;
  int cntrc = 0;
  const HAARCASCADE** ca = (const HAARCASCADE**)cas;
  ASSERT(1 == im->c);
  ASSERT(cas && ncas > 0 && ncas < 4);
  cx = ca[0]->w, cy = ca[0]->h;
  area2 = cx * cy;

  for (i = 0; i < ncas; ++i) {
    ASSERT(ca[i]->w == ca[0]->w && ca[i]->h == ca[0]->h);
    pStage[i] = (iStage*)(ca[i] + 1);
    pHaar[i] = (iHaar*)(pStage[i] + ca[i]->stagelen);
    bf_malloc(bf, &pWirk[i], ca[i]->ihlen * sizeof(iHaarWork3));
  }

  //bf_malloc(bf);
  bf_imsetsize(bf, im2, h2, w2, im->c, 1);
  bf_imresize_impl(bf, im->h, im->w, im->tt.data, im->s, im->c, h2, w2, im2->tt.data, im2->s, im2->c, CV_INTER_LINEAR);

  //imshow(im);imshow(im2);cvWaitKey(-1);
  for (; scale < 50; scale *= ratio) {
    img_t im3[1] = {0};
    img_t sum[1] = {0};
    img_t sqsum[1] = {0};
    int h3 = (int)(h2 / scale + 0.5);
    int w3 = (int)(w2 / scale + 0.5);
    int x, y, x_end = w3 - ca[0]->w, y_end = h3 - ca[0]->h;

    if (x_end < 0 || y_end < 0) {
      break;
    }

    bf_imsetsize(bf, im3, h3, w3, im->c, 1);
    bf_imresize_impl(bf, im2->h, im2->w, im2->tt.data, im2->s, im2->c, h3, w3, im3->tt.data, im3->s, im3->c, CV_INTER_LINEAR);
    bf_imsetsize(bf, sum, im3->h + 1, im3->w + 1, 4, 1);
    bf_imsetsize(bf, sqsum, im3->h + 1, im3->w + 1, 4, 1);
    icvIntegralImage_8u32s(im3->tt.data, im3->s, sum->tt.s4, sum->s, sqsum->tt.s4, sqsum->s, 0, 0, iSIZE(w3, h3), im->c);
    {
      int sumstep = (w3 + 1);
      int cystep = ca[0]->h * sumstep;

      for (i = 0; i < ncas; ++i) {
        setoffset3(ca[i]->ihlen, pHaar[i], pWirk[i], sumstep, 1, cx, cy, flag);            //0.275
      }

      for (y = 0; y <= y_end; ++y) {
        int* psum = sum->tt.s4 + y * sumstep;
        int* psqsum = sqsum->tt.s4 + y * sumstep;

        for (x = 0; x <= x_end; ++x, ++psum, ++psqsum) {
          double score = 0;
          int ispassed = 0, invvar;
          img_t im4[1] = {0};
          IMINIT(im4, cy, cx, im3->tt.data, im3->s, im3->c, 1);
          invvar = get_invvar_32s(psum, psqsum, cystep, cx, area2);
          //if (invvar<300)
          {
            //imshow(im3);imshow(im4);cvWaitKey(50);
            ispassed = lutpass_32s(psum, pHaar[0], pStage[0], ca[0]->stagelen, pWirk[0], invvar, &score);


            if (ispassed && ncas > 1) {
              for (i = 1; i < ncas; ++i) {
                ispassed = lutpass_32s(psum, pHaar[i], pStage[i], ca[i]->stagelen, pWirk[i], invvar, &score);

                if (ispassed) {
                  //cascnt[i]++;
                  break;
                }
              }
            }


            cntrc++;

            if (ispassed) {
              double t = scale * min_rc_scale;
              out_rc[ len ].x = XROUND((x) * t);
              out_rc[ len ].y = XROUND((y) * t);
              out_rc[ len ].w = XROUND(cx * t);
              out_rc[ len ].h = XROUND(cy * t);
              ++len;
            }
          }
        }
      }
    }
    //imshow(im3);cvWaitKey(-1);
    bf_imfree(bf, sqsum);
    bf_imfree(bf, sum);
    bf_imfree(bf, im3);
  }

  //imshow(im2);cvWaitKey(-1);
  bf_imfree(bf, im2);

  for (i = ncas - 1; i >= 0; --i) {
    bf_free(bf, &pWirk[i], ca[i]->ihlen * sizeof(iHaarWork3));
  }

  len = bf_xrect_partition(bf, len, out_rc, 0.8, 5, out_rc);
  return len;
}
