
#ifndef ___FACEDETECT_H__
#define ___FACEDETECT_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
//#include "../cascade.h"
//#include "../cstd.h"

#include "libfacerecog.h"
//#include "../imresize.h"
//#include "../sort.h"
#include "seqpartition.h"
#include "../std/objdetect.h"
#include "../std/imgopt.h"
#include "../std/img_c.h"


#ifndef WIN32 
//#define WIN32
#endif // WIN32

#if defined WIN32// || defined __GNUC__
#define __PC
#else
#undef __PC
#define __DSP
#endif

#ifdef __PC
// 转置时左右翻转
//#define TRANX

#define INCLUDETABLE 
//#include "xdraw.h"
//#include "xtypes.h"
//#include "utime.h"
#define STS_set(A, B) {}
#define STS_delta(A, B) {}
#else 
//#include <std.h>
//#include <log.h>
//#include <sts.h>
//#include "I:\\SOFT\\hard\\dsp\\TI\\ICETEK-DM642-FR\\mytest\\VideoLoopback_debug\\videocfg.h"
#ifndef USE_FIX_FLOAT
#define USE_FIX_FLOAT         // 使用定点数
#endif

#endif // __PC

//#define USE_INT_FUN2          // 使用全查表
#define BIGRECT_TO_SMALLRECT  // 从大到小搜索矩形框

#undef XROUND
#undef XFLOOR
#if 1
#define XROUND(x) (int) ((x) + 0.5f)
#define XFLOOR(x) (int) ((x))
#else
#define XROUND(x) cvRound(x)
#define XFLOOR(x) cvFloor(x)
#endif

typedef int ( *boostpass_fun_type ) ( const FIX_FLOAT* tmpSamp, const void* ih2, const void* stage2, int stagelen, const void* pWirk2, FIX_FLOAT invvar2, int stp, double* pscore );
typedef int ( *setoffset_fun_type ) ( int len, const void* f, void* p, int al, int ai, double cs, double iv );
typedef struct boostdetect_work_type {
  setoffset_fun_type setoffset;
  boostpass_fun_type boostpass;
}
boostdetect_work_type;

#define CAISHAVEFLOAT(ca, ih) ((ca)->ihlen > 0 && (ih)[0].ti == 0)
#define CAISNESTING(_h, _s)  (!((_h)[(_s)->len].ft0.w))

// 基本的boost算法
// 把上一级的强分类器值对第一个特殊特征查表，之后的每个特征算法和第一级一样。
#define BOOSTPASS_BASE(A1, ih, stage, irl, pW, vv, GET_VAL, GET_FEAT, vel, ispassed)  { int f = 0, l = irl; int pp = 0, qq; vel = 0.f; for (; f != l; ++f) { qq = pp + (stage + f)->len; for (vel = 0.f; pp != qq; ++pp) { vel += GET_VAL(GET_FEAT(A1, vv, (pW + pp)), (ih + pp)); } if (vel < (stage + f)->thd) { break; } } ispassed = l == f; }

// nesting 的boost算法
// 第一级：对每个haar特征的特征值查表并累加
// 第二级到最后一级：把上一级的强分类器值对第一个特殊特征查表，之后的每个特征
//         算法和第一级一样。
#define BOOSTPASS_NESTING(A1, ih, stage, irl, pW, vv, GET_VAL, GET_FEAT, vel, ispassed) \
  { \
    int f = 0, l = irl; \
    int pp = 0, qq; \
    qq = pp + (stage + f)->len; \
    vel = 0.f; \
    for (; pp != qq; ++pp) \
    { \
      vel += GET_VAL(GET_FEAT(A1, vv, (pW + pp)), (ih + pp)); \
    } \
    if (vel >= (stage + f)->thd) \
    { \
      for (++f; f != l; ++f) \
      { \
        qq = pp + (stage + f)->len; \
        vel = GET_VAL(vel, (ih + pp)); \
        ++pp; \
        for (; pp != qq; ++pp) \
        { \
          vel += GET_VAL(GET_FEAT(A1, vv, (pW + pp)), (ih + pp)); \
        } \
        if (vel < (stage + f)->thd) \
        { \
          break; \
        } \
      } \
    } \
    ispassed = l == f; \
  }


// 设置偏移量
#define SETOFFSET131(RECT, OUT, area)  { int cx = XROUND((RECT).w * cs); int cy = XROUND((RECT).h * cs); int a = XROUND((RECT).y * cs) * al + XROUND((RECT).x * cs) * ai; int b = cx * ai, c = cy * al; area = cx * cy; (OUT).p0 = a; (OUT).p1 = a + b; (OUT).p2 = a + c; (OUT).p3 = a + b + c; }

#define SETOFFSET(RECT, OUT, area)  { cx = XROUND((RECT).w * cs); cy = XROUND((RECT).h * cs); a = XROUND((RECT).y * cs) * al + XROUND((RECT).x * cs) * ai; b = cx * ai, c = cy * al; area = cx * cy; (OUT).p0 = a; (OUT).p1 = a + b; (OUT).p2 = a + c; (OUT).p3 = a + b + c; }


#ifdef _MSC_VER
#pragma warning(disable: 4305)
//#pragma warning(disable: 4244)
#endif

#ifdef WIN32
#define DATA_SECTION(A, B) once
#define DSP_STATIC static
#else 
//#define SECTION section("sdram_data_bank0")
//#define DATA_SECTION(A, B)  DATA_SECTION(A, B)
#define DSP_STATIC
#endif

typedef struct fStage {
  int len;
  float thd;
}
fStage;

typedef struct iStage {
  int len;
  int thd;
}
iStage;

typedef struct fHaarWork {
  struct {
    int p0, p1, p2, p3;
    float dd;
  }
  ft0, ft1;
}
fHaarWork;

typedef struct iHaarWork2 {
  struct {
#ifdef WIN32
    unsigned int p0, p1, p2, p3;
#else
    
    unsigned short p0, p1, p2, p3;
#endif
    
    int dd;
  }
  ft0, ft1;
}
iHaarWork2;

typedef struct iHaarWork3 {
  struct {
    unsigned short p0;
    unsigned short w, h;
    short dd;
  }
  ft0, ft1;
}
iHaarWork3;

typedef struct iRectW {
  unsigned char x, y, w, h;
  char d;
}
iRectW;

// haar 结构
#define HARR_STRUCT  \
  unsigned char n;   \
  iRectW ft0, ft1;   \
  unsigned char ti

// gentle adaboost struct
typedef struct WEAKGENTLE {
  HARR_STRUCT;
  float thd;
  float val[ 2 ];
}
WEAKGENTLE, *PWEAKGENTLE;

#define LUT_BIN                         (32)

// lut adaboost struct
typedef struct fHaar {
  HARR_STRUCT;
  float minval;
  float binval;
  float val[ LUT_BIN ];
}
fHaar;

typedef struct iHaar {
  HARR_STRUCT;
  int minval;
  int binval;
  int val[ LUT_BIN ];
}
iHaar;

typedef struct tagHAARCASCADE {
  int w;
  int h;
  int stagelen;
  int ihlen;
  int stage1; // 占位 无用
  int ih1; // 占位 无用
}
HAARCASCADE, *PHAARCASCADE;

#define CASCADESIZE(_P)  ((_P) ? (sizeof( HAARCASCADE ) + ((HAARCASCADE*)(_P))->stagelen*sizeof(fStage) + ((HAARCASCADE*)(_P))->ihlen*sizeof(fHaar)) : 0)
                    
int invvar_table_init();

void* FixCascade( const void * cas );

//extern iHaar* iHaarBuf;
//extern int HAARBUFLEN;


int lut_detect_1( const void* alg );

typedef lut_detect_param_t detect_info_t;

#define DETECT_MEMSIZE(_IHLEN, _H, _W, _B_LEN) \
  SIZEOFARR(detect_info_t, 2)+\
  SIZEOFARR( fHaarWork, _IHLEN )+\
  SIZEOFARR( unsigned char, 4 * (_W) * (_H) )+\
  SIZEOFARR( FIX_FLOAT, 2 * ( (_H) + 1 ) * ( (_W) + 1 ) )+\
  SEQPARTITIONSIZE(sizeof(XRECT), _B_LEN)

#define RECT_TRANS(RECT, t) (CC_SWAP((RECT).x, (RECT).y, t), CC_SWAP((RECT).w, (RECT).h, t))
#define RECT_SET(RECT, X, Y, CX, CY) ((RECT).x=X, (RECT).y=Y, (RECT).w=CX, (RECT).h=CY)

//#include "utime.h"
#ifdef __PC
//#define IMRESIZE1(_sh, _sw, _s, _sl, _scn, _dh, _dw, _d, _dl, _dcn) IMRESIZE_AREA(_sh, _sw, _s, _sl, _dh, _dw, _d, _dl, _scn);
#define IMRESIZE1(_sh, _sw, _s, _sl, _scn, _dh, _dw, _d, _dl, _dcn) IMRESIZE(_sh, _sw, _s, _sl, _scn, _dh, _dw, _d, _dl, _dcn, INTER_BILINEARITY);
#else
void resize( int ah, int aw, const unsigned char* A, int al, int ai, int bh, \
             int bw, unsigned char* B, int bl, int bi );

#define IMRESIZE12(_sh, _sw, _s, _sl, _dh, _dw, _d, _dl, cn) \
  resize(_sh, _sw, _s, _sl, cn, _dh, _dw, _d, _dl, cn)
#define IMRESIZE1(_sh, _sw, _s, _sl, _scn, _dh, _dw, _d, _dl, _dcn) \
  IMRESIZE(_sh, _sw, _s, _sl, _scn, _dh, _dw, _d, _dl, _dcn, INTER_BILINEARITY);

#endif

#endif // ___FACEDETECT_H__
