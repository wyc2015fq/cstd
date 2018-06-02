
#include "imresize.h"
#include "img\imgopt.inl"
#include "cascade.inl"
#include "weakgen.inl"
#include "weaklut.inl"

typedef double intertype;
typedef LUTWEAK weaktype;

typedef struct _IRECTW {
  int x, y, w, h, d;
}
IRECTW;

typedef struct IRECTWORK {
  int p0, p1, p2, p3;
  int dd;
}
IRECTWORK;

typedef struct _HAARFEAT {
  IRECTW ft0;
  IRECTW ft1;
}
HAARFEAT;

typedef struct HAARFEATWORK {
  IRECTWORK ft0;
  IRECTWORK ft1;
}
HAARFEATWORK;

typedef struct _HAAR3FEAT {
  IRECTW ft0;
  IRECTW ft1;
  IRECTW ft2;
}
HAAR3FEAT;

// HOG (Histograms of Oriented Gradients)
// [x, y, w, h]  - 矩形框 左上角x坐标, 左上角y坐标, 矩形框宽, 矩形框高
// bi              - [0-8] 表示9种角度 [0~40][40~80]..[320~360]
typedef struct _EOHFEAT {
  int x, y, w, h, bi;
  //int p0, p1, p2, p3;
  //double iarea; // 面积的倒数
  //int type;
}
EOHFEAT, EOHEXFEAT;

typedef int HIMMFEAT;
typedef int CHIFEAT;
typedef int BASEFEAT;
typedef int BABSFEAT;

typedef struct _EOHHAARFEAT {
  int type;
  union {
    IRECTW ft0;
    IRECTW ft1;
    struct {
      int x, y, w, h, bi;
      int p0, p1, p2, p3;
    };
  };
}
EOHHAARFEAT;

#define EOH_CN                         (10)
#define HAAR_CN                        ( 2)
#define EOHHAAR_CN                     (EOH_CN+HAAR_CN)
#define EOHEX_CN                       EOH_CN

#define RECT_VAL(A, p0, p1, p2, p3)   (*(A+p0) - *(A+p1) - *(A+p2) + *(A+p3))
//#define RECT_VAL2(_A, _a, _b)          (*(_A) - *(_A+(_a)) - *(_A+(_b)) + *(_A+(_a)+(_b)))
// 求 haar 特征
#define HAAR_RECT_VAL(A, rect)            (RECT_VAL(A, (rect).p0, (rect).p1, (rect).p2, (rect).p3) * (rect).dd)
#define HAAR_FEAT_VAL(A, haar, IV)     (IV*(HAAR_RECT_VAL(A, (haar).ft0) + HAAR_RECT_VAL(A, (haar).ft1)))
#define HAAR3_FEAT_VAL(A, haar, IV)    (IV*(HAAR_RECT_VAL(A, (haar).ft0) + HAAR_RECT_VAL(A, (haar).ft1) + ((haar).ft2.w ? HAAR_RECT_VAL(A, (haar).ft2) : 0)))
// 求 eoh 特征
//#define EOH_RECT_VAL(_A, rect)         (RECT_VAL(_A, (rect).p0, (rect).p1, (rect).p2, (rect).p3))
//#define EOH_FEAT_VAL(A, rect, IV)      ((EOH_RECT_VAL((A+(rect).bi+1), rect))/(EOH_RECT_VAL(A, rect)+0.00001f))
// 求 chi 特征

#define EOHEX_FEAT_VAL(A, rect, IV)  ( ( (EOH_RECT_VAL((A+eohex_idx0[(rect).bi]), rect)) + \
    ((rect).bi<9?0.f:(EOH_RECT_VAL((A+eohex_idx1[(rect).bi]), rect)) ) +  \
    ((rect).bi<18?0.f:(EOH_RECT_VAL((A+eohex_idx2[(rect).bi]), rect)) ) )/ \
    (EOH_RECT_VAL(A, rect)+0.00001f))

// 求 ed 特征
#define ED_FEAT_VAL(A, rect, IV)       ((EOH_RECT_VAL(A, rect)*(rect).iarea)
#define EOHED_FEAT_VAL(A, rect, IV)    (((rect).type==FT_EOH) ? EOH_FEAT_VAL(A, rect, IV) : ED_FEAT_VAL(A, rect, IV))
// 求 eohhaar 特征
#define EOHHAAR_FEAT_VAL(A, rect, IV)  ( (FT_EOH==(rect).type) ? (EOH_FEAT_VAL(A, rect, IV)) : (HAAR_FEAT_VAL((A+EOH_CN), rect, IV)) )

#define EOH_SETOFFSET(eoh, al, ai, cs)                                        \
  do {                                                                          \
    int a = XFLOOR((eoh).y * cs) * al + XFLOOR((eoh).x * cs) * ai;              \
    int w = XFLOOR((eoh).w * cs), h = XFLOOR((eoh).h * cs);                 \
    int b = w * ai, c = h * al;                                               \
    (eoh).p0 = a;                                                               \
    (eoh).p1 = a+b;                                                             \
    (eoh).p2 = a+c;                                                             \
    (eoh).p3 = a+b+c;                                                           \
  } while(0)

#define EOHEX_SETOFFSET EOH_SETOFFSET

#define HAAR3_SETOFFSET(RECT, al, ai, cs)                                     \
  do {                                                                          \
    int ar0, ar1, ar2;                                                          \
    RECT_SETOFFSET((RECT).ft0, al, ai, cs, (RECT).ft0, ar0);                \
    RECT_SETOFFSET((RECT).ft1, al, ai, cs, (RECT).ft1, ar1);                \
    (RECT).ft1.dd = ((RECT).ft1.d*cs*cs);                                   \
    if ((RECT).ft2.w) {                                                      \
      RECT_SETOFFSET((RECT).ft2, al, ai, cs, (RECT).ft2, ar2);              \
      (RECT).ft2.dd = ((RECT).ft2.d*cs*cs);                                 \
      if (ar0) {                                                                \
        (RECT).ft0.dd = -(((RECT).ft1.dd * ar1 + (RECT).ft2.dd * ar2)/ar0);\
      }                                                                         \
    } else {                                                                    \
      if (ar0) {                                                                \
        (RECT).ft0.dd = -((RECT).ft1.dd * ar1/ar0);                         \
      }                                                                         \
    }                                                                           \
  } while(0)

#define EOHHAAR_SETOFFSET(RECT, al, ai, cs)                                   \
  do {                                                                          \
    if (FT_EOH==(RECT).type) {                                                  \
      EOH_SETOFFSET(RECT, al, ai, cs);                                          \
    } else if (FT_HAAR==(RECT).type) {                                          \
      HAAR_SETOFFSET(RECT, al, ai, cs);                                         \
    }                                                                           \
  } while(0)

#define FMT_D5  _D(2d)_D(2d)_D(2d)_D(2d)_D(2d)
#define HAAR_FMT _D(2d)_D(2d)_D(2d)_D(2d)_D(2d)_D(2d)_D(2d)_D(2d)_D(2d)_D(2d)

#define FPRINTF_EOH(_F, _E)    fprintf(_F, FMT_D5, (_E).x, (_E).y, (_E).w, (_E).h, (_E).bi)
#define FSCANF_EOH(_F, _E)     fscanf(_F, FMT_D5, &(_E).x, &(_E).y, &(_E).w, &(_E).h, &(_E).bi)
#define FPRINTF_IRECTW(_F, _E) fprintf(_F, FMT_D5, (_E).x, (_E).y, (_E).w, (_E).h, (_E).d)
#define FSCANF_IRECTW(_F, _E)  fscanf(_F, FMT_D5, &(_E).x, &(_E).y, &(_E).w, &(_E).h, &(_E).d)
#define FPRINTF_HAAR(_F, _H)   FPRINTF_IRECTW(_F, (_H).ft0), FPRINTF_IRECTW(_F, (_H).ft1)
#define FSCANF_HAAR(_F, _H)    FSCANF_IRECTW(_F, (_H).ft0), FSCANF_IRECTW(_F, (_H).ft1)
#define FPRINTF_HAAR3(_F, _H)   FPRINTF_IRECTW(_F, (_H).ft0), FPRINTF_IRECTW(_F, (_H).ft1), FPRINTF_IRECTW(_F, (_H).ft2)
#define FSCANF_HAAR3(_F, _H)    FSCANF_IRECTW(_F, (_H).ft0), FSCANF_IRECTW(_F, (_H).ft1), FSCANF_IRECTW(_F, (_H).ft2)

#define IRECTW_SET(S, X0, Y0, CX0, CY0, W0) ((S).x = X0,(S).y = Y0,(S).w = CX0,(S).h = CY0,(S).d = W0)
// 正
#define SETRECT2(a, b, x0, y0, cx0, cy0, w0, x1, y1, cx1, cy1, w1)  if (testx[a] && testy[b]) { if (out) {ASSERT(i<maxoutlen);IRECTW_SET((out[i]).ft0, x0, y0, cx0, cy0, w0); IRECTW_SET((out[i]).ft1, x1, y1, cx1, cy1, w1);} ++i;}
#define SETRECT2a(w0, h0, x1, y1, w1, h1)  if (testx[w0] && testy[h0]) { if (out) {IRECTW_SET((out[i]).ft0, x, y, w0*dx, h0*dy, -w1*h1); IRECTW_SET((out[i]).ft1, x+x1*dx, y+y1*dy, w1*dx, h1*dy, w0*h0);} ++i;}
#define SETRECT2b(w0, h0, x1, y1, w1, h1)   {SETRECT2a(w0, h0, x1, y1, w1, h1);SETRECT2a(h0, w0, y1, x1, h1, w1);}
// 斜
#define TILRECT2(a, b, x0, y0, cx0, cy0, w0, x1, y1, cx1, cy1, w1)  if (((x + a * dx) <= w && (x - b * dy) >= 0 && (y + a * dx + b * dy) <= h)) { (f++)->init(al, ai, 1, x0, y0, cx0, cy0, w0, x1, y1, cx1, cy1, w1, 0, 0, 0, 0, 0) }

int make_haar_feat(int h, int w, int mincx, int mincy, int stepa, int stepb, HAARFEAT* out, int maxoutlen)
{
  int i = 0;
  // 在这计算19中Haar特征在特定的长宽下的特征数目
  // 19中特征可以分为12中情况计算
  // 开始计算特征!
  int x, y, dx, dy, len = 0;

  for (y = 0; y < h; y += stepa) {
    for (x = 0; x < w; x += stepa) {
      for (dy = mincy; dy <= h; dy += stepb) {
        for (dx = mincx; dx <= w; dx += stepb) {
          int dx2 = dx * 2, dx3 = dx * 3, dx4 = dx * 4;
          int dy2 = dy * 2, dy3 = dy * 3, dy4 = dy * 4;
          int testx[ 5 ] = { x <= w, x + dx <= w, x + dx2 <= w, x + dx3 <= w, x + dx4 <= w };
          int testy[ 5 ] = { y <= h, y + dy <= h, y + dy2 <= h, y + dy3 <= h, y + dy4 <= h };
          SETRECT2(2, 1, x, y, dx2, dy, -1, x + dx, y, dx, dy, 2);    /* 2×1 */
          SETRECT2(1, 2, x, y, dx, dy2, -1, x, y + dy, dx, dy, 2);    /* 1×2 */
          SETRECT2(3, 1, x, y, dx3, dy, -1, x + dx, y, dx, dy, 3);    /* 3×1 */
          SETRECT2(1, 3, x, y, dx, dy3, -1, x, y + dy, dx, dy, 3);    /* 1×3 */
          SETRECT2(4, 1, x, y, dx4, dy, -1, x + dx, y, dx2, dy, 2);   /* 4×1 */
          SETRECT2(1, 4, x, y, dx, dy4, -1, x, y + dy, dx, dy2, 2);   /* 1×4 */
          SETRECT2(3, 3, x, y, dx3, dy3, -1, x + dx, y + dy, dx, dy, 9);              /* 3×3 */

          // 2×2
          if (0) {
            SETRECT2(2, 2, x, y, dx2, dy2, -1, x, y, dx, dy, 4);
            SETRECT2(2, 2, x, y, dx2, dy2, -1, x + dx, y, dx, dy, 4);
            SETRECT2(2, 2, x, y, dx2, dy2, -1, x, y + dy, dx, dy, 4);
            SETRECT2(2, 2, x, y, dx2, dy2, -1, x + dx, y + dy, dx, dy, 4);
          }

          //SETRECT2(2,2,x,y,dx2,dy2,-1,x,y,dx,dy,2,x+dx,y+dy,dx,dy,2);

          // 3×3
          if (0) {
            SETRECT2(3, 3, x, y, dx3, dy3, -4, x, y, dx2, dy2, 9);
            SETRECT2(3, 3, x, y, dx3, dy3, -4, x + dx, y, dx2, dy2, 9);
            SETRECT2(3, 3, x, y, dx3, dy3, -4, x, y + dy, dx2, dy2, 9);
            SETRECT2(3, 3, x, y, dx3, dy3, -4, x + dx, y + dy, dx2, dy2, 9);
          }

          // 3×3
          if (0) {
#if 0
            SETRECT2(x, y, dx3, dy3, -1);
            SETRECT2(x, y, dx, dy, 3);
            SETRECT2(x + dx, y + dy, dx, dy, 3);
            SETRECT2(x + dx2, y + dy2, dx, dy, 3);
            SETRECT2(x, y, dx3, dy3, -1);
            SETRECT2(x, y + dy2, x + dx, dy3, 3);
            SETRECT2(x + dx, y + dy, dx2, dy2, 3);
            SETRECT2(x + dx2, y, dx3, dy, 3);
            SETRECT2(x, y, dx3, dy3, -1);
            SETRECT2(x, y + dy, dx3, dy2, 3);
            SETRECT2(x + dx, y, dx2, dy3, 3);
            SETRECT2(x + dx, y + dy, dx2, dy2, -9);
#endif
          }
        }
      }

      for (dy = 1; dy <= h; dy += stepa) {
        for (dx = 1; dx <= w; dx += stepa) {
#if 0
          int stepa = dx, stepb = dy;
          int dx2 = dx * 2, dx3 = dx * 3, dx4 = dx * 4;
          int dy2 = dy * 2, dy3 = dy * 3, dy4 = dy * 4;

          //x+stepa*dx-stepb*dy;
          //y+stepa*dx+stepb*dy;
          TILRECT2(2, 1, x, y, dx2, dy, -1, x + dx, y + dx, dx, dy, 2);               // 2×1
          TILRECT2(1, 2, x, y, dx, dy2, -1, x - dy, y + dy, dx, dy, 2);               // 1×2
          TILRECT2(3, 1, x, y, dx3, dy, -1, x + dx, y + dx, dx, dy, 3);               // 3×1
          TILRECT2(1, 3, x, y, dx, dy3, -1, x - dy, y + dy, dx, dy, 3);               // 1×3
          TILRECT2(4, 1, x, y, dx4, dy, -1, x + dx, y + dx, dx2, dy, 2);              // 4×1
          TILRECT2(1, 4, x, y, dx, dy4, -1, x - dy, y + dy, dx, dy2, 2);              // 1×4
          TILRECT2(3, 3, x, y, dx3, dy3, -1, x + dx - dy, y + dx + dy, dx, dy, 9);    // 3×3

          // 2×2
          TILRECT2(2, 2, x, y, dx2, dy2, -1, x, y, dx, dy, 4);
          TILRECT2(2, 2, x, y, dx2, dy2, -1, x + dx, y + dx, dx, dy, 4);
          TILRECT2(2, 2, x, y, dx2, dy2, -1, x - dy, y + dy, dx, dy, 4);
          TILRECT2(2, 2, x, y, dx2, dy2, -1, x + dx - dy, y + dx + dy, dx, dy, 4);

          //TILRECT2(2,2,x,y,dx2,dy2,-1,x,y,dx,dy,2,x+dx-dy,y+dx+dy,dx,dy,2);
          // 3×3
          TILRECT2(3, 3, x, y, dx3, dy3, -4, x, y, dx2, dy2, 9);
          TILRECT2(3, 3, x, y, dx3, dy3, -4, x + dx, y + dx, dx2, dy2, 9);
          TILRECT2(3, 3, x, y, dx3, dy3, -4, x - dy, y + dy, dx2, dy2, 9);
          TILRECT2(3, 3, x, y, dx3, dy3, -4, x + dx - dy, y + dx + dy, dx2, dy2, 9);
#endif
        }
      }
    }
  }

  return i;
}


STATIC int HAAR_print(FILE* pf, const HAARFEAT* haar)
{
  FPRINTF_HAAR(pf, haar[ 0 ]);
  return 0;
}
STATIC int HAAR_scanf(FILE* pf, HAARFEAT* haar)
{
  FSCANF_HAAR(pf, haar[ 0 ]);
  return 0;
}
STATIC int HAAR3_print(FILE* pf, const HAAR3FEAT* haar)
{
  FPRINTF_HAAR3(pf, haar[ 0 ]);
  return 0;
}
STATIC int HAAR3_scanf(FILE* pf, HAAR3FEAT* haar)
{
  FSCANF_HAAR3(pf, haar[ 0 ]);
  return 0;
}
STATIC int EOH_print(FILE* pf, const EOHFEAT* eoh)
{
  FPRINTF_EOH(pf, eoh[ 0 ]);
  return 0;
}
STATIC int EOH_scanf(FILE* pf, EOHFEAT* eoh)
{
  FSCANF_EOH(pf, eoh[ 0 ]);
  return 0;
}

#define CHI_integral 0
#define HAAR_integral integral_sq
#define HAAR3_integral integral_sq
#define EOH_integral EOH_grads_integral
#define EOHEX_integral EOH_grads_integral

typedef CARTFFEAT CARTWEAK;
#define EOHEX_setoffset EOH_setoffset
#define HAAR3_CN HAAR_CN
#define EOHEX_print EOH_print
#define EOHEX_scanf EOH_scanf
#define EOHEX_scanf EOH_scanf

HAARFEAT allhaarfeat[70000] = {0};

double HAAR_feat_val(const HAARFEATWORK* haar, const double* A, int i, double IV)
{
  //double aa = HAAR_RECT_VAL(A, haar[ i ].t0);
  //double bb = HAAR_RECT_VAL(A, haar[ i ].t1);
  return HAAR_FEAT_VAL(A, haar[ i ], IV);
}

// 设置偏移量
int IRECTWORK_setoffset(const IRECTW* rect, int al, int ai, IRECTWORK* out)
{
  int w = (*rect).w;
  int h = (*rect).h;
  int a = (*rect).y * al + (*rect).x * ai;
  int b = w * ai, c = h * al;
  int area = w * h;
  (*out).p0 = a;
  (*out).p1 = a + b;
  (*out).p2 = a + c;
  (*out).p3 = a + b + c;
  (*out).dd = (*rect).d;
  return area;
}

// 计算图像偏移量
int HAAR_setoffset(HAARFEATWORK* haar, int id, int al, int ai)
{
  //HAAR_SETOFFSET(haar[0], al, ai, cs);
  const HAARFEAT* feat = allhaarfeat + id;
  IRECTWORK_setoffset(&(*feat).ft0, al, ai, &((*haar).ft0));
  IRECTWORK_setoffset(&(*feat).ft1, al, ai, &((*haar).ft1));
  return 0;
}
int HAAR_setoffset_all(int n, HAARFEATWORK* haar, const weaktype* weak, int al, int ai)
{
  int i;

  for (i = 0; i < n; ++i) {
    HAAR_setoffset(haar + i, weak[i].id, al, ai);
  }

  return 0;
}

int integral_sq(int h, int w, const unsigned char* img, int al, double* mag, int ml, int mi)
{
  SKIP_INTEGRAL_SQ(h, w, img, al, 1, mag, ml, mi);
  return 0;
}

typedef struct HAAR_FEAT_T {
  char postxtfile[256];
  char negtxtfile[256];
  HAARFEAT* haar;
  int allhaarlen;
  double* samp;
  int samp_size;
  int samp_step;
  int w, h;
  int stepxy;
  double zoom;
  HAARFEATWORK* haarwork;
}
HAAR_FEAT_T;

int HAAR_uninit(BOOST* hh)
{
  HAAR_FEAT_T* fs = (HAAR_FEAT_T*)(hh->h_feat);
  FREE(fs->haar);
  return 0;
}

double guiyi(int n, double* f, double* w)
{
  int i;
  double mimval = *f, maxval = *f;

  for (i = 0; i < n; ++i) {
    if (w[i] < 0.f) {
      continue;
    }

    if (f[i] > maxval) {
      maxval = f[i];
    }
    else if (f[i] < mimval) {
      mimval = f[i];
    }
  }

  for (i = 0; i < n; ++i) {
    //if (w[i]<0.f) continue;
    f[i] = (f[i] - mimval) / (maxval - mimval);
  }

  return 0;
}

double* HAAR_feat(BOOST* hh, int i)
{
  HAAR_FEAT_T* fs = (HAAR_FEAT_T*)(hh->h_feat);
  int j, cx = fs->w, stp = (cx + 1);
  const double* samp = fs->samp;
  HAARFEATWORK haar[1];
  //i = 2310;
  HAAR_setoffset(haar, i, stp, 1);

  for (j = 0; j < hh->samp_num; ++j, samp += fs->samp_step) {
    if (hh->wi[ j ] < 0.f) {
      hh->fi[ j ] = 0;
    }
    else {
      hh->fi[ j ] = HAAR_feat_val(haar, samp, 0, 1);  ///hh->vvv[j];
    }

  }

  return hh->fi;
}

int init_allhaarfeat()
{
  static int inited = 0;

  if (!inited) {
    inited = 1;
    //make_haar_feat(haar_h, haar_w, 2)
  }

  return 0;
}


int cascade_pass(const CASCADE* ca, const double* samp, const HAARFEATWORK* haar, double invvar, int* pispass, double* pscore)
{
#define weak_macro(iw)           LUT_value(HAAR_feat_val(haar, samp, iw, invvar), weak, iw)
#define weak_macro2(score, iw)   LUT_value(score, weak, iw)
#include "cascade_pass_impl.inl"
  return 0;
}

// HAAR 样本
// 每张图片占空间 2*4*33*33=8712 字节
int HAAR_find_samp(BOOST* hh, int type)
{
  HAAR_FEAT_T* fs = (HAAR_FEAT_T*)(hh->h_feat);
  int j = 0, n = 0, cx = hh->ca->w, cy = hh->ca->h;
  int cy1 = cy + 1, cx1 = cx + 1;
  int _step = (cx + 1);
  int _size = (cy + 1) * _step;
  int SampNum = (type > 0) ? (hh->pos_num) : (hh->samp_num - hh->pos_num);
  double* wi = hh->wi + (type > 0 ? 0 : hh->pos_num);
  //double * vvv = hh->vvv + ( type > 0 ? 0 : hh->pos_num );
  double* vi = hh->vi + (type > 0 ? 0 : hh->pos_num);
  double zoom = fs->zoom;
  int stepxy = fs->stepxy;
  const char* piclist = type > 0 ? fs->postxtfile : fs->negtxtfile;
  int picnum = 0, sampnum = 0, allsampnum = 0;
  char buf[ 256 ];
  img_t im[1] = {0};
  FILE* pf = fopen(piclist, "r");

  if (!pf) {
    printf("%s文件打不开", piclist);
    return 0;
  }

  fs->samp_step = _size;

  //stepxy = IniGetInt( INIFILENAME, "3", "样本检测步进", 3 );
  //IniGetFloat( INIFILENAME, "3", "缩放比", zoom );
  //IniGetFloat( INIFILENAME, ADABOOSTSECT, "最小负样本比例", hh->min_neg_rate );
  for (; sampnum < SampNum * hh->min_neg_rate && stepxy >= 1; stepxy = stepxy * 2 / 3, zoom = pow(zoom, 0.7)) {
    utime_start(time);

    if (zoom < 1.1) {
      zoom = 1.1;
    }

    if (1 == type) {
    }
    else {
      FILL(SampNum, wi, -1);
    }

    for (picnum = 0, sampnum = 0, j = 0; j < SampNum && fscanf(pf, "%s", buf) > 0;) {
      if (wi[ j ] >= 0.f) {
        ++j;
        ++sampnum;
        continue;
      }

      imread(buf, 1, 1, im);

      if (!im->h) {
        continue;
      }

      {
        int step = im->s;
        unsigned char* data = im->tt.data;
        int w = im->w, h = im->h;
        intertype* inter = MALLOC(intertype, (h + 1) * (w + 1) * 2);
        intertype* inter0 = inter;
        intertype* inter1 = inter + (h + 1) * (w + 1);
        ++picnum;

        if (1 == type) {
          int x, y, ispass = 0;
          unsigned char* tmpimg = data;
          int stp = (w + 1);
          int siz = (h + 1) * stp;
          double score;
          double invarea = 1.f / (cx * cy);

          //int _al = stp * cy, _ai = cx;
          if (!(0 == h % cy && 0 == w % cx)) {
            printf("%s\n", buf);
          }

          assert(0 == h % cy && 0 == w % cx);
          FILL(siz * 2, inter, 0);
          //SKIP_HISTEQ(height, width, data, step, 1, data, step, 1);
          SKIP_INTEGRAL(h, w, data, step, 1, inter0, stp, 1);
          SKIP_INTEGRAL_GQ(h, w, data, step, 1, inter1, stp, 1);
          HAAR_setoffset_all(hh->ca->weaklen, fs->haarwork, (const weaktype*)hh->ca->weak, cx1, 1);

          for (y = 0; y < h && j < SampNum; y += cy) {
            for (x = 0; x < w && j < SampNum; x += cx) {
              double* tmpSamp0 = inter0 + y * stp + x;
              double* tmpSamp1 = inter1 + y * stp + x;
              double* Samp = fs->samp + j * _size;
              double var;
              double invvar;
              ++allsampnum;
              //FPRINT2D("a1.txt", "%8.f, ", height, width, tmpSamp, stp, HAAR_CN);
              //FILL(cx1*cy1, inter0, 0);
              //FILL(cx1*cy1, inter1, 0);
              //SKIP_INTEGRAL( hh->h, hh->w, tmpimg + y * w + x, w, 1, inter0, hh->w+1, 1 );
              //SKIP_INTEGRAL_GQ( hh->h, hh->w, tmpimg + y * w + x, w, 1, inter1, hh->w+1, 1 );
              SKIP_INTEGRAL_VAR(inter0, cx1 * cy, cx, inter1, cx1 * cy, cx, invarea, var);
              //SKIP_INTEGRAL_VAR( tmpSamp0, _al, _ai, tmpSamp1, _al, _ai, invarea, var );
              score = 0.f;
              ispass = 1;
              invvar = (var <= 0.) ? 1. : (1. / sqrt(var));
              //if (invvar>1.f) invvar=1.f;
              //CASCADE_PASS(hh->ca, tmpSamp0, hh->ca->haar, HAAR_FEAT_VAL, invvar, ispass, score);
              SKIP_COPY(cy1, cx1, tmpSamp0, stp, 1, Samp, _step, 1);
              cascade_pass(hh->ca, Samp, fs->haarwork, invvar, &ispass, &score);

              if (ispass) {
                int k;

                //FPRINT2D("a1.txt", "%8.f, ", cy1, cx1, Samp, _step, 1);
                for (k = 0; k < _size; ++k) {
                  Samp[ k ] *= invvar;
                }

                //vvv[ j ] = invvar;
                wi[ j ] = 1.f;
                vi[ j ] = score;
                ++j;
                ++sampnum;
              }
              else {
                //printf("%f %d ", invvar, hh->ca->is_nesting);
                //imwrite("a.bmp", height, width, data, step, 1);exit(0);
              }
            }
          }
        }

        if (-1 == type) {
          double ss = MIN(fs->h * 1. / im->h, fs->w * 1. / im->w);
          img_t im2[1] = {0};

          for (; j < SampNum && ss < 1; ss *= zoom) {
            int tt = imresize2(im, ss, im2);
            unsigned char* tmpimg = im2->tt.data;
            int h = im2->h, w = im2->w;
            int ranx = w - cx + 1;
            int rany = h - cy + 1;
            int stp = (w + 1);
            int siz = (h + 1) * stp;
            int x, y, k = 0;
            int stepx = stepxy, stepy = stepxy;
            double invarea = 1.f / (cx * cy);
            //fs->zoom = zoom;
            //fs->stepxy = stepxy;
            FILL(siz * 2, inter, 0);
            //IMRESIZE( im->h, im->w, im->data, im->s, 1, h, w, tmpimg, w, 1, INTER_BILINEARITY );
            //SKIP_HISTEQ(h, w, tmpimg, w, 1, tmpimg, w, 1);
            //SKIP_INTEGRAL_SQ( h, w, tmpimg, w, 1, inter, stp, 1 );
            SKIP_INTEGRAL(h, w, tmpimg, w, 1, inter0, stp, 1);
            SKIP_INTEGRAL_GQ(h, w, tmpimg, w, 1, inter1, stp, 1);

            //PRINT2D("a1.txt", "%8.f, ", 25, 25, inter, stp, HAAR_CN);
            //FPRINT2D("a0.txt", "%8.f, ", "\n", 25, 25, inter, stp, HAAR_CN);
            //FPRINT2D("a1.txt", "%8.f, ", "\n", 25, 25, inter+1, stp, HAAR_CN);
            HAAR_setoffset_all(hh->ca->weaklen, fs->haarwork, (const weaktype*)hh->ca->weak, cx1, 1);

            //for (i=0; i<times; ++i)
            for (x = 0; j < SampNum && x < ranx; x += stepx) {
              for (y = 0; j < SampNum && y < rany; y += stepy) {
                double* tmpSamp0 = inter0 + y * stp + x;
                double* tmpSamp1 = inter1 + y * stp + x;
                unsigned char* tmpimg0 = tmpimg + y * w + x;
                double* Samp = fs->samp + (j + hh->pos_num) * _size;
                int ispass = 0;
                double score, var, invvar;
#if 0
                FILL(cx1 * cy1, inter0, 0.f);
                FILL(cx1 * cy1, inter1, 0.f);
                SKIP_HISTEQ(hh->h, hh->w, tmpimg0, w, 1, tmpimg0, w, 1);
                SKIP_INTEGRAL(hh->h, hh->w, tmpimg0, w, 1, inter0, hh->w + 1, 1);
                SKIP_INTEGRAL_GQ(hh->h, hh->w, tmpimg0, w, 1, inter1, hh->w + 1, 1);
                SKIP_INTEGRAL_VAR(inter0, cx1 * cy, cx, inter1, cx1 * cy, cx, invarea, var);
                tmpSamp0 = inter0, tmpSamp1 = inter1;
                SKIP_COPY(cy1, cx1, tmpSamp0, cx1, 1, Samp, _step, 1);
#else
                SKIP_INTEGRAL_VAR(tmpSamp0, stp * cy, cx, tmpSamp1, stp * cy, cx, invarea, var);

                if (var < 10.) {
                  continue;
                }

                FILL(cx1 * cy1, Samp, 0.f);
                //SKIP_COPY( cy1, cx1, tmpSamp0, stp, 1, Samp, _step, 1 );
                SKIP_INTEGRAL(fs->h, fs->w, tmpimg0, w, 1, Samp, _step, 1);
                //if (var<1.) var=1.;
#endif
                ++allsampnum;
                invvar = (var <= 0.) ? 1. : 1. / sqrt(var);
                //invvar=((int)(invvar*(1<<13)))*1./(1<<13);
#if 0

                if (sqrt(var) < 0.01) {
                  double int0[500] = {0};
                  double int1[500] = {0};
                  SKIP_INTEGRAL(hh->h, hh->w, tmpimg0, w, 1, int0, hh->w + 1, 1);
                  SKIP_INTEGRAL_GQ(hh->h, hh->w, tmpimg0, w, 1, int1, hh->w + 1, 1);
                  FPRINT2D("int0.txt", "%8.8f, ", "\r\n", hh->h + 1, hh->w + 1, int0, hh->w + 1, 1);
                  FPRINT2D("int1.txt", "%8.8f, ", "\r\n", hh->h + 1, hh->w + 1, int1, hh->w + 1, 1);
                  FPRINT2D("86433.txt", "%8d, ", "\r\n", cy, cx, tmpimg0, w, 1);
                  SKIP_INTEGRAL_VAR(int0, cx1 * cy, cx, int1, cx1 * cy, cx, invarea, var);
                  imwrite("86433.bmp", cy, cx, tmpimg0, w, 1);
                }

#endif

                if (invvar == (int)invvar) {
                  int asdf = 0;
                }

                //if (j + hh->pos_num==9031) { imwrite("86433.bmp", cy, cx, tmpimg0, w, 1); }
                //CASCADE_PASS(hh->ca, tmpSamp, hh->ca->haar, HAAR_FEAT_VAL, invvar, ispass, score);
                cascade_pass(hh->ca, Samp, fs->haarwork, invvar, &ispass, &score);

                if (j < SampNum && ispass) {
                  int k;

                  //unsigned char* tmpimg0 =  tmpimg + y * w + x;
                  //SKIP_INTEGRAL( cy, cx, tmpimg0, w, 1, Samp, _step, 1 );
                  for (k = 0; k < _size; ++k) {
                    Samp[ k ] *= invvar;
                  }

                  //vvv[ j ] = invvar;
                  wi[ j ] = 1.f;
                  vi[ j ] = score;
                  ++j;
                  ++sampnum;
                }
              }
            }
          }

          imfree(im2);
        }

        FREE(inter);
#define STR111 "找了%5d张图片，共%9d个%s样本，找到%6d个，耗时%3d秒！%2d %3.3f"
        printf(STR111"\r", picnum, allsampnum, type > 0 ? "正" : "负", sampnum, (int) utime_elapsed(time), stepxy, zoom);
      }
    }

    fclose(pf);
    logprintf(STR111"\n", picnum, allsampnum, type > 0 ? "正" : "负", sampnum, (int) utime_elapsed(time), stepxy, zoom);
  }

  imfree(im);

  return n;
}

int HAAR_init1(BOOST* hh)
{
  HAAR_FEAT_T* fs = MALLOC(HAAR_FEAT_T, 1);
  int mincx = 4, mincy = 4, rectxy = 2;
  hh->h_feat = fs;
  strcpy(fs->postxtfile, "listpos.txt");
  strcpy(fs->negtxtfile, "listpos.txt");
  fs->zoom = 1.1;
  fs->w = hh->ca->w = 20;
  fs->h = hh->ca->h = 20;
  fs->stepxy = 4;
  fs->samp_step = (hh->ca->h + 1) * (hh->ca->w + 1);
  hh->feat_num = make_haar_feat(hh->ca->h, hh->ca->w, mincx, mincy, rectxy, rectxy, NULL, 0);
  fs->haar = MALLOC(HAARFEAT, hh->feat_num);
  fs->haarwork = MALLOC(HAARFEATWORK, hh->ca->maxweaklen);
  make_haar_feat(hh->ca->h, hh->ca->w, mincx, mincy, rectxy, rectxy, fs->haar, hh->feat_num);

  hh->h_feat = fs;

  if (!fs->samp) {
    fs->samp_size = hh->samp_num * fs->samp_step;
    fs->samp = MALLOCSET(double, fs->samp_size, 0);
  }

  return 1;
}

int HAAR_init(BOOST* hh)
{
  int mincx, mincy, rectxy;
  HAAR_FEAT_T* fs = MALLOC(HAAR_FEAT_T, 1);
  memset(fs, 0, sizeof(HAAR_FEAT_T));
  strcpy(fs->postxtfile, inigetstr("正样本txt文件"));
  strcpy(fs->negtxtfile, inigetstr("负样本txt文件"));
  fs->zoom = inigetfloat("缩放比");
  fs->w = hh->ca->w = inigetint("样本宽");
  fs->h = hh->ca->h = inigetint("样本高");
  mincx = inigetint("最小宽");
  mincy = inigetint("最小高");
  rectxy = inigetint("特征穷举步进");
  fs->stepxy = inigetint("样本检测步进");
  fs->samp_step = (hh->ca->h + 1) * (hh->ca->w + 1);

  hh->feat_num = make_haar_feat(hh->ca->h, hh->ca->w, mincx, mincy, rectxy, rectxy, NULL, 0);
  fs->haar = MALLOC(HAARFEAT, hh->feat_num);
  make_haar_feat(hh->ca->h, hh->ca->w, mincx, mincy, rectxy, rectxy, fs->haar, hh->feat_num);

  hh->h_feat = fs;

  if (!fs->samp) {
    fs->samp_size = hh->samp_num * fs->samp_step;
    fs->samp = MALLOCSET(double, fs->samp_size, 0);
  }

  return 1;
}
