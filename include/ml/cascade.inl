
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "macro.h"
#include "imresize.h"
#include "face/_faceDetect.h"
#include "cstd.h"
#include "macro.h"



#include <stdio.h>
#include "xtypes.h"
#include "macro.h"
//#include "faceDetect.h"
//#include "debug.h"

#define ESP (0.00001f)

#include "sys\console.h"
#define STATIC static

#define _PRINT_PRO          // 打印进度
#define LUT_BIN (32)
#define X_ESP   (0.00001f)
#define DELIMT    " "
#define _D(_F)   "%"#_F DELIMT
#define _DF()    FLOATFMT DELIMT
#if 0
//typedef float real;
//typedef double real;
#define _F()   "%f" DELIMT
#else
//typedef double real;
#define _F()   "%lf" DELIMT
#endif
typedef unsigned int size_t;


#define INIFILENAME ".\\adaboost.ini"
#define ADABOOSTSECT  "adaboost"

// typedef double(*_FEAT_VAL_FUN_T)(double* samp, int al, void* feat, int i, double iv);
// typedef double(*_WEAK_VAL_FUN_T)(double featval, void*, int i);
// typedef int (*_FPRINT_FUN_T)(FILE* pf, const void* ca);
// typedef int (*_FSCANF_FUN_T)(FILE* pf, void* ca);
// typedef int (*_SETOFFSET_FUN_T)(void* eoh, int al, int ai, double cs);
// typedef int (*_INTEGRAL_FUN_T)(int h, int w, const unsigned char* img, int al, double* mag, int ml, int mi);

// 求 eohex 特征
const static int eohex_idx0[ 27 ] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9,
};
const static int eohex_idx1[ 27 ] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8,
};
const static int eohex_idx2[ 27 ] = {
  1, 2, 3, 4, 5, 6, 7, 8, 9, 9, 1, 2, 3, 4, 5, 6, 7, 8, 8, 9, 1, 2, 3, 4, 5, 6, 7,
};

//#define EOHEX_FEAT_VAL EOH_FEAT_VAL

typedef struct _LUTWEAK {
  int id;
  double minval;
  double binval; // = bin/(maxval-minval);
  double val[ LUT_BIN ];
}
LUTWEAK;

typedef struct _GENWEAK {
  int id;
  double thd;
  double val[ 2 ];
}
GENWEAK;

typedef struct _TANWEAK {
  double thd;
  double gm;
}
TANWEAK;

typedef struct _carrect {
  unsigned char x, y, w, h, id;// 对应block 的位置
} carrect;

typedef struct _carnode {
  double constrain;   // 阈值
  double coeffs[2];   // [0]小于阈值的得分, [1]大于阈值的得分
  unsigned char chr[2];         // [0]左孩子的索引 0为空, [1]右孩子的索引
  unsigned char x, y, w, h, id;// 对应block 的位置
  //int dim;            // 对应特征向量的第几维
} carnode;

#define CARTMAXSPLIT   5
typedef struct CARTFFEAT {
  carnode nodes[CARTMAXSPLIT];
} CARTFFEAT;

// 强分类器结构
#ifndef STAGE_DEFED
#define STAGE_DEFED
typedef struct _STAGE {
  int len;  // 包含弱分类器数目
  double thd; // 强分类器阈值
}
STAGE;
#endif // STAGE_DEFED

typedef struct _RANKWEAK {
  int id;
  double alpha;
}
RANKWEAK;

#define TAN_VAL(x, weak)   (double)(atan((weak)->gm*((x)-(weak)->thd))*(2./PI))

#define GUS_BIN_POS  100
#define GUS_BIN_NEG  100
#define GUS_BIN  (GUS_BIN_POS+GUS_BIN_NEG)

typedef struct GUSWEAK {
  double val[GUS_BIN];
} GUSWEAK;

typedef struct _CASCADE {
  int maxstagelen;  // 最大强分类器数
  int maxweaklen;   // 最大弱分类器数
  int stagelen;     // 强分类器数
  int weaklen;      // 弱分类器数
  int elemsize;     // 弱分类器大小
  int is_nesting;   // 使用nesting结构
  int w, h;         // 最小检测框
  STAGE* stage;
  void* weak;
}
CASCADE;

#define del_boost(hh)  (del_boost_(hh), (hh)=0)
//#include "adaboost_config.h"
#define GetFeat_elemsize(hh)    _feat_fun[ (hh)->feat_type ].elemsize
//#define GetWeak_elemsize(hh)    _weak_fun[ (hh)->weak_type ].elemsize
#define GetPosNum(hh)    (hh)->pos_num
#define GetNegNum(hh)    ((hh)->samp_num - (hh)->pos_num)
// #define getfeat(hh, i)   (hh)->feat_vt->f_get_feat(hh, i)
// #define findsamp(hh, t)  (hh)->feat_vt->f_find_samp(hh, t)
// #define savefeat(hh, t)  (hh)->feat_vt->f_save_feat(hh, t)
// #define updateweight(hh, t)  (hh)->weak_vt->f_update_weight(hh, t)
// #define geterror(hh, t)       (hh)->weak_vt->f_get_error(hh, t)
// #define uninit_feat(hh)       (hh)->feat_vt->f_uninit_feat(hh)
// #define init_feat(hh)       (hh)->feat_vt->f_init_feat(hh)
// #define samp_data(hh, buf, len)       (hh)->feat_vt->f_samp_data(hh, &buf, &len)
// #define init_feat(hh)       (hh)->feat_vt->f_init_feat(hh)

typedef struct _WORKINFO {
  double Ha, Fa, Error, Threshold, Time;
  int id, beg, end;
  double val[ 4 ];
  CARTFFEAT cart;
}
WORKINFO;
//typedef int (*f_init_feat_t)(BOOST* hh);

// adaboost训练
typedef struct BOOST {
  double* vi;           // [samp_num] 样本得分
  double* wi;           // [samp_num] 样本权重
  double* fi;           // [samp_num] 特征数据权重
  CASCADE ca[1];        // 分类器
  void* h_feat;

  int feat_num;         // 总特征数目
  double ha;            // 正样本通过率
  double fa;            // 负样本拒识率
  double Hr;            // 正负样本权重比例
  double min_neg_rate;  // 最小负样本比例
  int pos_num;          // 正样本数目，小于它的是正样本索引，大于等于的是负样本索引
  int samp_num;         // 总样本数目
  unsigned char* flag;  // [feat_num] 特征索引，特征没有用过=0，特征已经用过=1
  int flag_unreselect;  // 不重选特征
  int weak_type;        // boost类型
  int feat_type;        // 样本类型，找样本时用的数据[ST_CHI, ...]
}
BOOST;

typedef double* (*get_feat_f)(BOOST* hh, int i);

#define DO_PRINT_PRO(_PRO, _ALL, _TIME) { if ((_ALL) < 200 || !((_PRO) % ((_ALL) / MIN(_ALL, 10000))) || ((_ALL) - (_PRO)) < 3) \
    { printf("%6d|%9g %9g| %9g  %9g  %9d [%4.1f%%]\r", pwi->id, pwi->Ha, pwi->Fa, pwi->Error, pwi->Threshold, _TIME, ((_PRO + 1) * 100.f / (_ALL)) ); } }


STATIC int TAN_print(FILE* pf, const TANWEAK* tan)
{
  fprintf(pf, _DF() _DF(), tan[ 0 ].thd, tan[ 0 ].gm);
  return 0;
}

STATIC int TAN_scanf(FILE* pf, TANWEAK* tan)
{
  fscanf(pf, _F() _F(), &tan[ 0 ].thd, &tan[ 0 ].gm);
  return 0;
}

int cascade_new(CASCADE* ca, const char* fn, int maxstagelen, int maxweaklen, int elemsize)
{
  FILE* pf = NULL;

  if (fn) {
    pf = fopen(fn, "rb");
  }

  if (pf) {
    fread(ca, sizeof(CASCADE), 1, pf);
    maxstagelen = MAX(maxstagelen, ca->maxstagelen);
    maxweaklen = MAX(maxweaklen, ca->maxweaklen);
    ASSERT(ca->elemsize == elemsize);
  }

  ca->maxstagelen = maxstagelen;
  ca->maxweaklen = maxweaklen;
  ca->elemsize = elemsize;
  ca->stage = MALLOC(STAGE, maxstagelen);
  ca->weak = MALLOC(char, elemsize * maxweaklen);
  fread(ca->stage, sizeof(STAGE)*ca->stagelen, 1, pf);
  fread(ca->weak, ca->elemsize * ca->weaklen, 1, pf);

  if (pf) {
    int i, weaklen = 0;

    for (i = 0; i < ca->stagelen; ++i) {
      weaklen += ca->stage[i].len;
    }

    ca->weaklen = weaklen;
    fclose(pf);
  }

  return 0;
}
int cascade_save(const CASCADE* ca, const char* fn)
{
  FILE* pf = fopen(fn, "wb");

  if (pf) {
    fwrite(ca, sizeof(CASCADE), 1, pf);
    fwrite(ca->stage, sizeof(STAGE)*ca->stagelen, 1, pf);
    fwrite(ca->weak, ca->elemsize * ca->weaklen, 1, pf);
    fclose(pf);
  }

  return 0;
}

int cascade_free(CASCADE* ca)
{
  if (ca) {
    FREE(ca->stage);
    FREE(ca->weak);
  }

  return 0;
}

int getweakcnt(const CASCADE* ca)
{
  int i = 0, cnt = 0;

  for (i = 0; i < ca->stagelen; ++i) {
    cnt += ca->stage[ i ].len;
  }

  return cnt;
}

// 求x/y的反正切值
#if 1

// %0-180
#define EOH_ATAN2(x, y) (int) ((atan(x / (y + ESP)) + (PI / 2)) * 9.f / PI)
#else

// %0-360
#define EOH_ATAN2(x, y) (int) ((atan2(x, y) + PI) * 9.f / (2 * PI))
#endif

#if 0
// 计算梯度积分矩阵 [h×w×1] => [h+1×w+1×10]
int EOH_grads_integral(int h, int w, const unsigned char* img, int al, double* mag, int ml, int mi)
{
  int i, j, k;

  //unsigned char* sob = MALLOC(unsigned char, h*w);
  //double* sobf = MALLOC(double, h*w);
  double* mag0 = mag;

  for (i = 0; i < h; ++i, img += al, mag += ml) {
    int dx, dy;
    const unsigned char* top = img - al;
    const unsigned char* mid = img;
    const unsigned char* bot = img + al;

    if (0 == i) {
      top = img;
    }

    if (h - 1 == i) {
      bot = img;
    }

    for (j = 0; j < w; ++j, ++top, ++bot, ++mid) {
      int angle;
      double grads, *mag1, *mag2;

      if (0 == j) {
        dx = mid[ 1 ] + mid[ 1 ] + top[ 1 ] + bot[ 1 ] - mid[ 0 ] - mid[ 0 ] - top[ 0 ] - bot[ 0 ];
        dy = bot[ 0 ] + bot[ 0 ] + bot[ 1 ] + bot[ 0 ] - top[ 0 ] - top[ 0 ] - top[ 0 ] - top[ 1 ];
      }
      else if (w - 1 == j) {
        dx = mid[ 0 ] + mid[ 0 ] + top[ 0 ] + bot[ 0 ] - mid[ -1 ] - mid[ -1 ] - top[ -1 ] - bot[ -1 ];
        dy = bot[ 0 ] + bot[ 0 ] + bot[ 0 ] + bot[ -1 ] - top[ 0 ] - top[ 0 ] - top[ -1 ] - top[ 0 ];
      }
      else {
        dx = mid[ 1 ] + mid[ 1 ] + top[ 1 ] + bot[ 1 ] - mid[ -1 ] - mid[ -1 ] - top[ -1 ] - bot[ -1 ];
        dy = bot[ 0 ] + bot[ 0 ] + bot[ 1 ] + bot[ -1 ] - top[ 0 ] - top[ 0 ] - top[ -1 ] - top[ 1 ];
      }

      grads = (double) sqrt((dx * dx) + (dy * dy));         //%梯度模值和
      angle = EOH_ATAN2(dx, dy);

      //assert(angle<9 && "error : EOH_grads_integral : angle out of memery!");
      if (angle > 8) {
        angle = 8;
      }

      if (angle < 0) {
        angle = 0;
      }

      mag1 = mag2 = mag + j * mi;

      for (k = 0; k < EOH_CN; ++k, ++mag2) {
        mag2[ ml + mi ] = mag2[ ml ] + mag2[ mi ] - mag2[ 0 ];
      }

      mag1[ ml + mi ] += grads;
      mag1[ ml + mi + angle + 1 ] += grads;

      //grads/=3.f; if (grads>255) {        grads = 255.f;      }
      //sobf[i*w+j] = grads;
      //sob[i*w+j] = grads;
    }
  }

  //VEC_LINEARITY(h*w, sobf, sob, 0.f, 255.f);
  //imwrite("testhog_sob.bmp", h, w, sob, w, 1);
  //free(sob);
  //free(sobf);
  //PRINT(h, w, sob, w, 1, "angle.txt", FLOATFMT", ");
  return 0;
}

// HAAR 检测
// [h, w, mag, al] 梯度积分图 通道数=10
int detect(CASCADE* ca, int height, int width, const unsigned char* data, int step,
    double ssmin, double ssmax, double ss, int stepxy, XRECT* B, int B_len)
{
  int i, len = 0, cx = ca->w, cy = ca->h;
  int w = XFLOOR(width / ssmin), h = XFLOOR(height / ssmin);
  int INTEGRAL_CN = _feat_fun[ ca->feat_type ].INTEGRAL_CN;
  int height1 = height, width1 = width, step1 = step;
  char* chptr;
  unsigned char* tmpimg = NULL;
  unsigned char* tmpimg1 = NULL;
  tmpimg = MALLOC(unsigned char, w * h);
  assert(INTEGRAL_CN != 0);

  for (; len < B_len && w >= cx && h >= cy; ssmin *= ss, w = XFLOOR(width / ssmin), h = XFLOOR(height / ssmin)) {
    int ranx = w - cx + 1;
    int rany = h - cy + 1;
    int stp = (w + 1) * INTEGRAL_CN;
    int siz = (h + 1) * stp;
    int x, y;
    int stepx = stepxy, stepy = stepxy;
    double invarea = 1.f / (cx * cy);
    double* inter;
    inter = MALLOCSET(double, siz, 0);

    if (NULL == tmpimg1) {
      IMRESIZE_BILIN(height, width, data, step, h, w, tmpimg, w, 1);
    }
    else {
      IMRESIZE_BILIN(height1, width1, tmpimg1, step1, h, w, tmpimg, w, 1);
    }

    _feat_fun[ ca->feat_type ].integral(h, w, tmpimg, w, inter, stp, INTEGRAL_CN);
    //FPRINT2D("./b.txt", "%8.0f,", ENDL, 50, 50, inter, ((w+1)*2), 2);
    //imwrite(".\\a.bmp", h, w, tmpimg, w, 1);
    //SKIP_INTEGRAL_SQ( h, w, tmpimg, w, 1, inter, stp, INTEGRAL_CN );
    //FEAT_SETOFFSET( ca->haar, ca->weaklen, stp, INTEGRAL_CN, 1.f, HAAR );
    chptr = (char*)(ca->featptr);

    for (i = 0; i < ca->weaklen; ++i, chptr += _feat_fun[ ca->feat_type ].elemsize) {
      if (_feat_fun[ ca->feat_type ].setoffset) {
        _feat_fun[ ca->feat_type ].setoffset(chptr, stp, INTEGRAL_CN, 1.f);
      }
    }

    for (y = 0; len < B_len && y < rany; y += stepy) {
      for (x = 0; len < B_len && x < ranx; x += stepx) {
        double* tmpSamp = inter + y * stp + x * INTEGRAL_CN;
        double* tmpSamp1 = tmpSamp + 1;
        double score, invvar;
        int ispass = 0;
        SKIP_INTEGRAL_VAR(tmpSamp, stp * cy, INTEGRAL_CN * cx, tmpSamp1, stp * cy, INTEGRAL_CN * cx, invarea, invvar);
        invvar = (invvar <= 0.f) ? 1.f : 1.f / (double) sqrt(invvar);
        //CASCADE_PASS(ca, tmpSamp, ca->haar, HAAR_FEAT_VAL, invvar, ispass, score);
        cascade_pass(ca, tmpSamp, stp, invvar, &ispass, &score);

        if (ispass) {
          B[ len ].x = XFLOOR(x * ssmin);
          B[ len ].y = XFLOOR(y * ssmin);
          B[ len ].w = XFLOOR(cx * ssmin);
          B[ len ].h = XFLOOR(cy * ssmin);
          B[ len ].count = 1;
          B[ len ].score = (int) score;
          ++len;
        }
      }
    }

    FREE(inter);

    //if (height1 > 2 * h)
    if (0) {
      unsigned char* tmpimg2 = NULL;

      if (NULL == tmpimg1) {
        tmpimg1 = MALLOC(unsigned char, w * h);
      }

      CV_SWAP(tmpimg1, tmpimg, tmpimg2);
      height1 = h, width1 = w , step1 = w;
    }
  }

  FREE(tmpimg);
  FREE(tmpimg1);
  return len;
}

int detect1(CASCADE* ca, int height, int width, const unsigned char* data, int step,
    double ssmin, double ssmax, double ss, int stepxy, XRECT* B, int B_len)
{
  int len = 0, cx = ca->w, cy = ca->h, cx0 = ca->w, cy0 = ca->h;
  int i, w = (width), h = (height);
  int INTEGRAL_CN = _feat_fun[ ca->feat_type ].INTEGRAL_CN;
  int stp = (w + 1) * INTEGRAL_CN;
  int siz = (h + 1) * stp;
  char* chptr;
  unsigned char* tmpimg = MALLOC(unsigned char, w * h);
  double* inter;
  inter = MALLOCSET(double, siz, 0);
  cx = XFLOOR(cx0 * ssmin);
  cy = XFLOOR(cy0 * ssmin);
  IMRESIZE(height, width, data, step, 1, h, w, tmpimg, w, 1, INTER_BILINEARITY);
  _feat_fun[ ca->feat_type ].integral(h, w, tmpimg, w, inter, stp, INTEGRAL_CN);

  for (; len < B_len && w >= cx && h >= cy; ssmin *= ss, cx = XFLOOR(cx0 * ssmin), cy = XFLOOR(cy0 * ssmin)) {
    int x, y;
    int ranx = w - cx + 1;
    int rany = h - cy + 1;
    int stepx = stepxy, stepy = stepxy;
    double invarea = 1.f / (cx * cy);
    double invvar;
    //FEAT_SETOFFSET( ca->eohhaar, ca->weaklen, stp, INTEGRAL_CN, ssmin, EOHHAAR );
    chptr = (char*)(ca->featptr);

    for (i = 0; i < ca->weaklen; ++i, chptr += _feat_fun[ ca->feat_type ].elemsize) {
      if (_feat_fun[ ca->feat_type ].setoffset) {
        _feat_fun[ ca->feat_type ].setoffset(chptr, stp, INTEGRAL_CN, 1.f);
      }
    }

    for (y = 0; len < B_len && y < rany; y += stepy) {
      for (x = 0; len < B_len && x < ranx; x += stepx) {
        double* tmpSamp = inter + y * stp + x * INTEGRAL_CN;
        double* tmpSamp0 = tmpSamp + EOH_CN;
        double* tmpSamp1 = tmpSamp0 + 1;
        int ispass = 0;
        double score;
        SKIP_INTEGRAL_VAR(tmpSamp0, stp * cy, INTEGRAL_CN * cx, tmpSamp1, stp * cy, INTEGRAL_CN * cx, invarea, invvar);
        invvar = (invvar <= 0.f) ? 1.f : 1.f / (double) sqrt(invvar);
        cascade_pass(ca, tmpSamp, stp, invvar, &ispass, &score);

        if (ispass) {
          B[ len ].x = (x);
          B[ len ].y = (y);
          B[ len ].w = (cx);
          B[ len ].h = (cy);
          ++len;
        }
      }
    }
  }

  FREE(inter);
  FREE(tmpimg);
  return len;
}
#endif
