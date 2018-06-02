
#ifndef _CASCADE_H_
#define _CASCADE_H_

#include <stdio.h>
#include "xtypes.h"
#include "adaboost_config.h"
#include "macro.h"
//#include "faceDetect.h"
//#include "debug.h"

#ifdef __cplusplus
extern "C" {
#endif

  enum {
    FT_NUL = 0,
    FT_CHI,
    FT_EOH,
    FT_HAAR,
    FT_EOHHAAR,
    FT_BASE,
    FT_EOHEX,
    FT_HAAR3,
    FEATNUM,
  };

  enum {
    WT_GEN = 1,       // gentle boost
    WT_LUT,           // lut boost
    WT_RANK,          // rank boost
    WT_TAN,           // tan boost
    BOOSTNUM,
  };

  static const char* feature_name[] = {
    "unknow feature",
    "卡方距离特征",
    "HOG特征",
    "Haar特征",
    "EOHHAAR特征",
    "纯数据特征",
    "EOHEX特征",
  };

  static const char* feature_description[] = {
    "unknow feature",
    "任意距离特征(类内距离是正样本，类间距离是负样本)",
    "Histograms of Oriented Gradients",
    "Haar Like",
    "EOH + HAAR",
    "BASE 纯数据特征",
    "EOHEX",
  };

  static const char* boost_description[] = {
    "unknown boost",
    "gentle boost",
    "lut boost",
    "rank boost",
    "tan boost",
  };

#ifndef XRECT_DEFINED
#define XRECT_DEFINED
  // 矩形框
  typedef struct XRECT {
    int x, y, w, h;      /* 一个目标的所有框的位置平均值 */
    real score;       /* 一个目标的所有框的得分平均值 */
    int count;             /* 一个目标的所有框的个数       */
    int id;
  }
  XRECT;
#endif // XRECT_DEFINED

  typedef struct tagIRECTW {
    int x, y, w, h, d;
    int p0, p1, p2, p3;
    real dd;
  }
  IRECTW;

  typedef struct tagHAARFEAT {
    IRECTW ft0;
    IRECTW ft1;
  }
  HAARFEAT;

  typedef struct tagHAAR3FEAT {
    IRECTW ft0;
    IRECTW ft1;
    IRECTW ft2;
  }
  HAAR3FEAT;

  // HOG (Histograms of Oriented Gradients)
  // [x, y, w, h]  - 矩形框 左上角x坐标, 左上角y坐标, 矩形框宽, 矩形框高
  // bi              - [0-8] 表示9种角度 [0~40][40~80]..[320~360]
  typedef struct tagEOHFEAT {
    int x, y, w, h, bi;
    int p0, p1, p2, p3;
    //real iarea; // 面积的倒数
    //int type;
  }
  EOHFEAT, EOHEXFEAT;

  typedef int CHIFEAT;
  typedef int BASEFEAT;

  typedef struct tagEOHHAARFEAT {
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

#define RECT_VAL(_A, p0, p1, p2, p3)   (*(_A + (p0)) - *(_A + (p1)) - *(_A + (p2)) + *(_A + (p3)))
#define RECT_VAL2(_A, _a, _b)          (*(_A) - *(_A+(_a)) - *(_A+(_b)) + *(_A+(_a)+(_b)))
  // 求 haar 特征
#define HAAR_RECT_VAL(A, rect)         (RECT_VAL(A, (rect).p0, (rect).p1, (rect).p2, (rect).p3) * (rect).dd)
#define HAAR_FEAT_VAL(A, haar, IV)     (IV*(HAAR_RECT_VAL(A, (haar).ft0) + HAAR_RECT_VAL(A, (haar).ft1)))
#define HAAR3_FEAT_VAL(A, haar, IV)    (IV*(HAAR_RECT_VAL(A, (haar).ft0) + HAAR_RECT_VAL(A, (haar).ft1) + ((haar).ft2.w ? HAAR_RECT_VAL(A, (haar).ft2) : 0)))
  // 求 eoh 特征
#define EOH_RECT_VAL(_A, rect)         (RECT_VAL(_A, (rect).p0, (rect).p1, (rect).p2, (rect).p3))
#define EOH_FEAT_VAL(A, rect, IV)      ((EOH_RECT_VAL((A+(rect).bi+1), rect))/(EOH_RECT_VAL(A, rect)+0.00001f))
  // 求 chi 特征
#define BASE_FEAT_VAL(A1, pw, IV) (*((A1) + (pw)))

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

#define EOHEX_FEAT_VAL(A, rect, IV)  ( ( (EOH_RECT_VAL((A+eohex_idx0[(rect).bi]), rect)) + \
    ((rect).bi<9?0.f:(EOH_RECT_VAL((A+eohex_idx1[(rect).bi]), rect)) ) +  \
    ((rect).bi<18?0.f:(EOH_RECT_VAL((A+eohex_idx2[(rect).bi]), rect)) ) )/ \
    (EOH_RECT_VAL(A, rect)+0.00001f))

  //#define EOHEX_FEAT_VAL EOH_FEAT_VAL

  // 求 ed 特征
#define ED_FEAT_VAL(A, rect, IV)       ((EOH_RECT_VAL(A, rect)*(rect).iarea)
#define EOHED_FEAT_VAL(A, rect, IV)    (((rect).type==FT_EOH) ? EOH_FEAT_VAL(A, rect, IV) : ED_FEAT_VAL(A, rect, IV))
  // 求 eohhaar 特征
#define EOHHAAR_FEAT_VAL(A, rect, IV)  ( (FT_EOH==(rect).type) ? (EOH_FEAT_VAL(A, rect, IV)) : (HAAR_FEAT_VAL((A+EOH_CN), rect, IV)) )

#define EOH_SETOFFSET(eoh, al, ai, cs)                                        \
  do {                                                                          \
    int a = XFLOOR((eoh).y * cs) * al + XFLOOR((eoh).x * cs) * ai;              \
    int cx = XFLOOR((eoh).w * cs), cy = XFLOOR((eoh).h * cs);                 \
    int b = cx * ai, c = cy * al;                                               \
    (eoh).p0 = a;                                                               \
    (eoh).p1 = a+b;                                                             \
    (eoh).p2 = a+c;                                                             \
    (eoh).p3 = a+b+c;                                                           \
  } while(0)

#define EOHEX_SETOFFSET EOH_SETOFFSET

  // 设置偏移量
#define RECT_SETOFFSET(RECT, al, ai, cs, OUT, area)  {                        \
    int cx = XFLOOR((RECT).w* cs);                                             \
    int cy = XFLOOR((RECT).h* cs);                                             \
    int a = XFLOOR((RECT).y * cs) * al + XFLOOR((RECT).x * cs) * ai;            \
    int b = cx * ai, c = cy * al;                                               \
    area = cx * cy;                                                             \
    (OUT).p0 = a;                                                               \
    (OUT).p1 = a+b;                                                             \
    (OUT).p2 = a+c;                                                             \
    (OUT).p3 = a+b+c;                                                           \
  }

#define HAAR_SETOFFSET(RECT, al, ai, cs)                                      \
  do {                                                                          \
    int ar0, ar1;                                                               \
    RECT_SETOFFSET((RECT).ft0, al, ai, cs, (RECT).ft0, ar0);                \
    RECT_SETOFFSET((RECT).ft1, al, ai, cs, (RECT).ft1, ar1);                \
    (RECT).ft1.dd = ((RECT).ft1.d*cs*cs);                                   \
    if (ar0) {                                                                  \
      (RECT).ft0.dd = -((RECT).ft1.dd * ar1/ar0);                           \
    }                                                                           \
  } while(0)

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

  // 计算图像偏移量
#define FEAT_SETOFFSET(_PTR, _LEN, _AL, _AI, _CS, _FEAT)                      \
  do {                                                                          \
    int _I;                                                                     \
    for (_I=0; _I<_LEN; ++_I) {                                                 \
      _FEAT##_SETOFFSET(*((_PTR)+_I), _AL, _AI, _CS);                           \
    }                                                                           \
  } while(0)

  typedef struct tagLUTWEAK {
    real minval;
    real binval; // = bin/(maxval-minval);
    real val[ LUT_BIN ];
  }
  LUTWEAK;

  typedef struct tagGENWEAK {
    real thd;
    real val[ 2 ];
  }
  GENWEAK;

  typedef struct tagTANWEAK {
    real thd;
    real gm;
  }
  TANWEAK;

  // 强分类器结构
#ifndef STAGE_DEFED
#define STAGE_DEFED
  typedef struct STAGE {
    int len;  // 包含弱分类器数目
    real thd; // 强分类器阈值
  }
  STAGE;
#endif // STAGE_DEFED

  typedef struct tagRANKWEAK {
    int id;
    real alpha;
  }
  RANKWEAK;

  // LUT adaboost
#define LUT_VAL(x, lut)  ( (lut)->val[(int)(((x)-(lut)->minval)*(lut)->binval) & (LUT_BIN-1)] )

#define GEN_VAL(x, gen)  ((gen)->val[(x) > (gen)->thd])

#define TAN_VAL(x, weak)   (real)(atan((weak)->gm*((x)-(weak)->thd))*(2./PI))

  typedef struct CASCADE {
    int feat_type;    // 样本类型
    int weak_type;    // boost 类型
    int stagelen;     // 强分类器数
    int weaklen;      // 弱分类器数
    int maxstagelen;  // 最大强分类器数
    int maxweaklen;   // 最大弱分类器数
    int w, h;       // 最小检测框
    int is_nesting;   // 使用nesting结构
    int* id;
    STAGE* stage;
    union {
      void* weakptr;
      GENWEAK* gen;
      LUTWEAK* lut;
      TANWEAK* tan;
      RANKWEAK* rank;
    };
    union {
      void* featptr;
      int* chi;
      EOHFEAT* eoh;
      HAARFEAT* haar;
      HAAR3FEAT* haar3;
      EOHHAARFEAT* eohhaar;
    };
    //int (*setoffset)(const CASCADE* ca, int step, int cn, real cs);
    //int (*pass)(const CASCADE* ca, const real* Samp, real* score);
  }
  CASCADE;

  // 构建
  CASCADE* new_cascade(int maxstagelen, int maxweaklen, int type, int weak_type);

  // 析构
  void del_cascade(CASCADE* ptr);

  // 格式化打印
  void cascade_printf(const CASCADE* ptr, FILE* pf);

  // 格式化输入
  //pf = fopen("cas.txt", "rb");
  //cascade_scanf(pf, 1000, 10000);//载入分类器
  CASCADE* cascade_scanf(FILE* pf, int maxstagelen, int maxweaklen);

  int EOH_grads_integral(int h, int w, const unsigned char* img, int al, real* mag, int ml, int mi);

  int detect(CASCADE* ca, int height, int width, const unsigned char* data, int step,
      real ssmin, real ssmax, real ss, int stepxy, XRECT* B, int B_len);

  int detect1(CASCADE* ca, int height, int width, const unsigned char* data, int step,
      real ssmin, real ssmax, real ss, int stepxy, XRECT* B, int B_len);

  int read_rect_from_file(const char* fname, EOHFEAT* out, int cn);
  int get_haar_feat(int h, int w, int mincx, int mincy, int a, int b, HAARFEAT* out);

  void cascade_pass(CASCADE* _CA, void* _SAMP, real _IV, int* _ISPASS, real* _PSCORE);

  void cascade2hpp(const CASCADE* ptr, const char* name);

  CASCADE* read_cascade_xml(const char* xml);

#ifdef __cplusplus
}
#endif

#endif // _CASCADE_H_
