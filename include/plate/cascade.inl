
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "macro.h"
#include "imresize.h"
//#include "fmtimg.h"
#include "cstd.h"
#include "macro.h"

#define STATIC static

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


typedef real(*_FEAT_VAL_FUN_T)(real*, void*, int i, real);
typedef real(*_WEAK_VAL_FUN_T)(real, void*, int i);
typedef int (*_FPRINT_FUN_T)(FILE* pf, const void* ptr);
typedef int (*_FSCANF_FUN_T)(FILE* pf, void* ptr);
typedef int (*_SETOFFSET_FUN_T)(void* eoh, int al, int ai, real cs);
typedef int (*_INTEGRAL_FUN_T)(int h, int w, const unsigned char* img, int al, real* mag, int ml, int mi);

typedef struct {
  _FEAT_VAL_FUN_T val;
  _FPRINT_FUN_T print;
  _FSCANF_FUN_T scanf;
  _SETOFFSET_FUN_T setoffset;
  _INTEGRAL_FUN_T integral;
  size_t INTEGRAL_CN;
  size_t elemsize;
}
feat_fun_t;

typedef struct {
  _WEAK_VAL_FUN_T val;
  _FPRINT_FUN_T print;
  _FSCANF_FUN_T scanf;
  size_t elemsize;
}
weak_fun_t;

STATIC feat_fun_t _feat_fun[ FEATNUM ] = {0};
STATIC weak_fun_t _weak_fun[ BOOSTNUM ] = {0};

STATIC int GEN_print(FILE* pf, const GENWEAK* gen)
{
#if 1
  fprintf(pf, _DF() _DF() _DF(), gen->thd, gen->val[ 0 ], gen->val[ 1 ]);
#else

  int flag = gen[ 0 ].val[ 0 ] > gen[ 0 ].val[ 1 ] ? 1 : 0;
  real ww = gen[ 0 ].val[ 0 ] + gen[ 0 ].val[ 1 ];

  fprintf(pf, _D(6d) _D(1d), ptr->id[ i ], flag);
#endif

  return 0;
}

STATIC int GEN_scanf(FILE* pf, GENWEAK* gen)
{
  fscanf(pf, _F(), &gen[ 0 ].thd, &gen[ 0 ].val[ 0 ], &gen[ 0 ].val[ 1 ]);
  return 0;
}

STATIC int LUT_print(FILE* pf, const LUTWEAK* lut)
{
  int j;
  fprintf(pf, _DF() _DF(), lut->minval, lut->binval);

  for (j = 0; j < LUT_BIN; ++j) {
    fprintf(pf, _DF(), lut->val[ j ]);
  }

  return 0;
}

STATIC int LUT_scanf(FILE* pf, LUTWEAK* lut)
{
  int j;
  fscanf(pf, _F() _F(), &lut[ 0 ].minval, &lut[ 0 ].binval);

  for (j = 0; j < LUT_BIN; ++j) {
    fscanf(pf, _F(), &lut[ 0 ].val[ j ]);
  }

  return 0;
}

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
STATIC int BASE_print(FILE* pf, const void* haar)
{
  return 0;
}
STATIC int BASE_scanf(FILE* pf, void* haar)
{
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

STATIC int EOHHAAR_print(FILE* pf, const EOHHAARFEAT* eohhaar)
{
  int type = eohhaar[ 0 ].type;
  fprintf(pf, _D(d), type);

  if (FT_EOH == type || FT_EOHEX == type) {
    FPRINTF_EOH(pf, eohhaar[ 0 ]);
  }
  else if (FT_HAAR == type) {
    FPRINTF_HAAR(pf, eohhaar[ 0 ]);
  }

  return 0;
}
STATIC int EOHHAAR_scanf(FILE* pf, EOHHAARFEAT* eohhaar)
{
  int type;
  fscanf(pf, _D(d), &type);
  eohhaar[ 0 ].type = type;

  if (FT_EOH == type || FT_EOHEX == type) {
    FSCANF_EOH(pf, eohhaar[ 0 ]);
  }
  else if (FT_HAAR == type) {
    FSCANF_HAAR(pf, eohhaar[ 0 ]);
  }

  return 0;
}

real LUT_value(real x, LUTWEAK* lut, int i)
{
  return LUT_VAL(x, lut + i);
}
real GEN_value(real x, GENWEAK* gen, int i)
{
  return GEN_VAL(x, gen + i);
}
real HAAR_feat_val(real* A, HAARFEAT* haar, int i, real IV)
{
  return HAAR_FEAT_VAL(A, haar[ i ], IV);
}
real HAAR3_feat_val(real* A, HAAR3FEAT* haar, int i, real IV)
{
  return HAAR3_FEAT_VAL(A, haar[ i ], IV);
}
real EOHHAAR_feat_val(real* A, EOHHAARFEAT* eohhaar, int i, real IV)
{
  return EOHHAAR_FEAT_VAL(A, eohhaar[ i ], IV);
}
real EOH_feat_val(real* A, EOHFEAT* eoh, int i, real IV)
{
  return EOH_FEAT_VAL(A, eoh[ i ], IV);
}
real EOHEX_feat_val(real* A, EOHFEAT* eoh, int i, real IV)
{
  return EOHEX_FEAT_VAL(A, eoh[ i ], IV);
}
real BASE_feat_val(real* A, int* chi, int i, real IV)
{
  return BASE_FEAT_VAL(A, chi[ i ], IV);
}

int HAAR_setoffset(HAARFEAT* haar, int al, int ai, real cs)
{
  HAAR_SETOFFSET(haar[ 0 ], al, ai, cs);
  return 0;
}
int HAAR3_setoffset(HAAR3FEAT* haar, int al, int ai, real cs)
{
  HAAR3_SETOFFSET(haar[ 0 ], al, ai, cs);
  return 0;
}
int EOH_setoffset(EOHFEAT* haar, int al, int ai, real cs)
{
  EOH_SETOFFSET(haar[ 0 ], al, ai, cs);
  return 0;
}
int EOHHAAR_setoffset(EOHHAARFEAT* haar, int al, int ai, real cs)
{
  EOHHAAR_SETOFFSET(haar[ 0 ], al, ai, cs);
  return 0;
}

int EOH_grads_integral(int h, int w, const unsigned char* img, int al, real* mag, int ml, int mi);
int integral_sq(int h, int w, const unsigned char* img, int al, real* mag, int ml, int mi)
{
  SKIP_INTEGRAL_SQ(h, w, img, al, 1, mag, ml, mi);
  return 0;
}
int EOHHAAR_integral(int h, int w, const unsigned char* img, int al, real* mag, int ml, int mi)
{
  EOH_grads_integral(h, w, img, al, mag, ml, mi);
  SKIP_INTEGRAL_SQ(h, w, img, al, 1, mag + EOH_CN, ml, mi);
  return 0;
}

#define BASE_integral 0
#define CHI_integral 0
#define HAAR_integral integral_sq
#define HAAR3_integral integral_sq
#define EOH_integral EOH_grads_integral
#define EOHEX_integral EOH_grads_integral

#define BASE_setoffset 0
#define CHI_setoffset 0
#define EOHEX_setoffset EOH_setoffset
#define HAAR3_CN HAAR_CN
#define BASE_CN 0
#define CHI_CN 0
#define CHI_print BASE_print
#define CHI_scanf BASE_scanf
#define CHI_feat_val BASE_feat_val
#define EOHEX_print EOH_print
#define EOHEX_scanf EOH_scanf
#define EOHEX_scanf EOH_scanf

#define INIT_FEAT_FUN(_NAME) \
  _feat_fun[FT_##_NAME].val      = _NAME##_feat_val, \
      _feat_fun[FT_##_NAME].print    = _NAME##_print,    \
          _feat_fun[FT_##_NAME].scanf    = _NAME##_scanf,    \
              _feat_fun[FT_##_NAME].integral    = _NAME##_integral, \
                  _feat_fun[FT_##_NAME].setoffset    = _NAME##_setoffset, \
                      _feat_fun[FT_##_NAME].INTEGRAL_CN    = _NAME##_CN, \
                          _feat_fun[FT_##_NAME].elemsize = sizeof(_NAME##FEAT)

#define INIT_WEAK_FUN(_NAME) \
  _weak_fun[WT_##_NAME].val      = _NAME##_value, \
      _weak_fun[WT_##_NAME].print    = _NAME##_print, \
          _weak_fun[WT_##_NAME].scanf    = _NAME##_scanf, \
              _weak_fun[WT_##_NAME].elemsize = sizeof(_NAME##WEAK)

void init_fun()
{
  STATIC int init = 0;

  if (0 == init) {
    INIT_FEAT_FUN(BASE);
    INIT_FEAT_FUN(CHI);
    INIT_FEAT_FUN(HAAR);
    INIT_FEAT_FUN(HAAR3);
    INIT_FEAT_FUN(EOHHAAR);
    INIT_FEAT_FUN(EOHEX);
    INIT_FEAT_FUN(EOH);

    INIT_WEAK_FUN(GEN);
    INIT_WEAK_FUN(LUT);
  }
}

void cascade_pass0(void* _SAMP, STAGE* _STG, int _STGLEN,
    void* _FEAT, _FEAT_VAL_FUN_T _FEAT_VAL,
    void* _WEAK, _WEAK_VAL_FUN_T _WEAK_VAL,
    real _IV, int* _ISPASS, real* _PSCORE)
{
  int _I = 0, _J = 0, _P = 0;
  real x, y, _SCORE = 0.f;

  for (_I = 0; _I < (_STGLEN); ++_I) {
    _SCORE = 0.f;

    for (_J = _P; _J < (_P + (_STG) [ _I ].len); ++_J) {
      x = _FEAT_VAL(_SAMP, _FEAT, _J, _IV);
      y = _WEAK_VAL(x, _WEAK, _J);
      _SCORE += y;
    }

    if (_SCORE < (_STG) [ _I ].thd) {
      break;
    }

    _P += (_STG) [ _I ].len;
  }

  *_ISPASS = _I == (_STGLEN);
  *_PSCORE = _SCORE;
}

// 用分类器ca过样本(NESTING结构)
void cascade_pass1(real* _SAMP, STAGE* _STG, int _STGLEN,
    void* _FEAT, _FEAT_VAL_FUN_T _FEAT_VAL,
    void* _WEAK, _WEAK_VAL_FUN_T _WEAK_VAL,
    real _IV, int* _ISPASS, real* _PSCORE)
{
  int _I = 0, _J = 0, _P = 0;
  real x, y, _SCORE = 0.f;

  if ((_STGLEN) > 0) {
    for (_J = _P; _J < (_P + (_STG) [ 0 ].len); ++_J) {
      x = _FEAT_VAL(_SAMP, _FEAT, _J, _IV);
      y = _WEAK_VAL(x, _WEAK, _J);
      _SCORE += y;
    }

    if (_SCORE >= (_STG) [ _I ].thd) {
      _P += (_STG) [ 0 ].len;

      for (_I = 1; _I < (_STGLEN); _P += (_STG) [ _I++ ].len) {
        _SCORE = _WEAK_VAL(_SCORE, _WEAK, _P);

        for (_J = _P + 1; _J < (_P + (_STG) [ _I ].len); ++_J) {
          x = _FEAT_VAL(_SAMP, _FEAT, _J, _IV);
          y = _WEAK_VAL(x, _WEAK, _J);
          _SCORE += y;
        }

        if (_SCORE < (_STG) [ _I ].thd) {
          break;
        }
      }
    }
  }

  *_ISPASS = _I == (_STGLEN);
  *_PSCORE = _SCORE;
}

void cascade_pass(CASCADE* _CA, void* _SAMP, real _IV, int* _ISPASS, real* _PSCORE)
{
  STAGE* _STG;
  int _STGLEN;
  void* _FEAT;
  void* _WEAK;
  _FEAT_VAL_FUN_T _FEAT_VAL;
  _WEAK_VAL_FUN_T _WEAK_VAL;

  //assert(NULL!=(_CA));
  if (NULL == (_CA)) {
    *_ISPASS = 1;
    *_PSCORE = 0;
    return ;
  }

  _STG = (_CA) ->stage;
  _STGLEN = (_CA) ->stagelen;
  _FEAT = (_CA) ->featptr;
  _WEAK = (_CA) ->weakptr;
  _FEAT_VAL = _feat_fun[(_CA) ->feat_type ].val;
  _WEAK_VAL = _weak_fun[(_CA) ->weak_type ].val;

  if ((_CA) ->is_nesting) {
    cascade_pass1(_SAMP, _STG, _STGLEN, _FEAT, _FEAT_VAL,
        _WEAK, _WEAK_VAL, _IV, _ISPASS, _PSCORE);
  }
  else {
    cascade_pass0(_SAMP, _STG, _STGLEN, _FEAT, _FEAT_VAL,
        _WEAK, _WEAK_VAL, _IV, _ISPASS, _PSCORE);
  }
}


CASCADE* new_cascade(int maxstagelen, int maxweaklen, int feat_type, int weak_type)
{
  CASCADE* ptr = NULL;
  init_fun();
  ptr = MALLOCSET(CASCADE, 1, 0);
  ptr->feat_type = feat_type;
  ptr->weak_type = weak_type;
  ptr->w = ptr->h = ptr->stagelen = ptr->weaklen = 0;
  ptr->maxstagelen = maxstagelen;
  ptr->maxweaklen = maxweaklen;
  ptr->stage = MALLOC(STAGE, maxstagelen);
  ptr->stage = MALLOCSET(STAGE, maxstagelen, 0);
  ptr->id = MALLOCSET(int, maxweaklen, 0);
  ptr->weakptr = MALLOCSET(char, _weak_fun[ weak_type ].elemsize * maxweaklen, 0);
  ptr->featptr = MALLOCSET(char, _feat_fun[ feat_type ].elemsize * maxweaklen, 0);
  return ptr;
}

void del_cascade(CASCADE* ptr)
{
  if (ptr) {
    FREE(ptr->stage);
    FREE(ptr->id);
    FREE(ptr->featptr);
    FREE(ptr->weakptr);
    FREE(ptr);
  }
}

#define WEAK_FUN(_PTR)  (_weak_fun[ (_PTR)->weak_type ])
#define FEAT_FUN(_PTR)  (_feat_fun[ (_PTR)->feat_type ])

char* splitname(const char* path, char* name)
{
  const char* namebeg;
  char* nameend;
  namebeg = strrchr(path, '\\');

  if (!namebeg) {
    namebeg = strrchr(path, '/');
  }

  if (!namebeg) {
    namebeg = path;
  }

  if (namebeg) {
    ++namebeg;
    nameend = strrchr(namebeg, '.');

    if (nameend) {
      int len;
      len = nameend - namebeg;
      memcpy(name, namebeg, len);
      name[ len ] = 0;
    }
    else {
      strcpy(name, namebeg);
    }
  }

  return name;
}

void cascade2hpp(const CASCADE* ptr, const char* path)
{
  int i;
  FILE* pf;
  char name[ 256 ];
  size_t weaksize, featsize;
  char* weakptr, * featptr;
  splitname(path, name);
  pf = fopen(path, "wb");

  fprintf(pf, "\n#ifndef _%s_\n", name);
  fprintf(pf, "#define _%s_\n", name);
  fprintf(pf, "#include \"STATICload.h\"\n\n");

  fprintf(pf, "STATIC STAGE %s_STAGE[] = {\n", name);

  for (i = 0; i < ptr->stagelen; ++i) {
    fprintf(pf, _D(d) _DF() ENDL, ptr->stage[ i ].len, ptr->stage[ i ].thd);
  }

  fprintf(pf, "};\n\n");
  fprintf(pf, "STATIC iHaar %s_iHaar[] = {\n", name);
  weaksize = WEAK_FUN(ptr).elemsize ;
  featsize = FEAT_FUN(ptr).elemsize;
  weakptr = (char*)(ptr->weakptr);
  featptr = (char*)(ptr->featptr);

  for (i = 0; i < ptr->weaklen; ++i, weakptr += weaksize, featptr += featsize) {
    fprintf(pf, "%6d ", ptr->id[ i ]);

    if (FEAT_FUN(ptr).print) {
      FEAT_FUN(ptr).print(pf, featptr);
    }

    if (WEAK_FUN(ptr).print) {
      WEAK_FUN(ptr).print(pf, weakptr);
    }

    fprintf(pf, ENDL);
  }

  fprintf(pf, "};\n\n");

  fprintf(pf, "static HAARCASCADE %s_cascade = {", name);
  fprintf(pf, "%2d, %2d, %2d, %s_STAGE, %s_iHaar", ptr->w, ptr->h, ptr->stagelen, name, name);
  fprintf(pf, "};\n\n");

  fprintf(pf, "#endif _%s_\n", name);
  //fflush(pf);
  fclose(pf);
}

#define CAS_FMT _D(d)_D(d)_D(d)_D(d)_D(d)_D(d)_D(d)
void cascade_printf(const CASCADE* ptr, FILE* pf)
{
  int i;
  size_t weaksize, featsize;
  char* weakptr, * featptr;
  fprintf(pf, CAS_FMT ENDL, ptr->feat_type, ptr->weak_type, ptr->stagelen, ptr->weaklen, ptr->w, ptr->h, ptr->is_nesting);

  for (i = 0; i < ptr->stagelen; ++i) {
    fprintf(pf, _D(d) _DF() ENDL, ptr->stage[ i ].len, ptr->stage[ i ].thd);
  }

  weaksize = WEAK_FUN(ptr).elemsize;
  featsize = FEAT_FUN(ptr).elemsize;
  weakptr = (char*)(ptr->weakptr);
  featptr = (char*)(ptr->featptr);

  for (i = 0; i < ptr->weaklen; ++i, weakptr += weaksize, featptr += featsize) {
    fprintf(pf, _D(6d), ptr->id[ i ]);

    if (FEAT_FUN(ptr).print) {
      FEAT_FUN(ptr).print(pf, featptr);
    }

    if (WEAK_FUN(ptr).print) {
      WEAK_FUN(ptr).print(pf, weakptr);
    }

    fprintf(pf, ENDL);
  }

  fflush(pf);
}

CASCADE* cascade_scanf(FILE* pf, int maxstagelen, int maxweaklen)
{
  int i, ret, stagelen, weaklen, type, weak_type, cx, cy, is_nesting;
  CASCADE* ptr = NULL;
  size_t weaksize, featsize;
  char* weakptr, * featptr;
  LUTWEAK lutwk = { 0 };
  STAGE stg = { 0 };
  char* fmt = CAS_FMT ENDL;
  ret = fscanf(pf, CAS_FMT ENDL, &type, &weak_type, &stagelen, &weaklen, &cx, &cy, &is_nesting);

  if (ret < 0) {
    weak_type = cx = cy = type = weaklen = stagelen = 0;
  }

  if (maxstagelen < stagelen) {
    maxstagelen = stagelen;
  }

  if (maxweaklen < weaklen) {
    maxweaklen = weaklen;
  }

  ptr = new_cascade(maxstagelen, maxweaklen, type, weak_type);
  ptr->stagelen = stagelen;
  ptr->weaklen = weaklen;
  ptr->w = cx;
  ptr->h = cy;
  ptr->is_nesting = is_nesting;

  if (ret < 0) {
    return ptr;
  }

  for (i = 0, weaklen = 0; i < ptr->stagelen; ++i) {
    fscanf(pf, _D(d) _F() ENDL, &ptr->stage[ i ].len, &ptr->stage[ i ].thd);
    weaklen += ptr->stage[ i ].len;
  }

  weaksize = WEAK_FUN(ptr).elemsize ;
  featsize = FEAT_FUN(ptr).elemsize;
  weakptr = (char*)(ptr->weakptr);
  featptr = (char*)(ptr->featptr);

  for (i = 0; i < ptr->weaklen; ++i, weakptr += weaksize, featptr += featsize) {
    fscanf(pf, _D(d), &ptr->id[ i ]);

    if (FEAT_FUN(ptr).print) {
      FEAT_FUN(ptr).scanf(pf, featptr);
    }

    if (WEAK_FUN(ptr).print) {
      WEAK_FUN(ptr).scanf(pf, weakptr);
    }
  }

  ptr->weaklen = weaklen;
  return ptr;
}

#define ESP (0.00001f)
// 求x/y的反正切值
#if 1

// %0-180
#define EOH_ATAN2(x, y) (int) ((atan(x / (y + ESP)) + (PI / 2)) * 9.f / PI)
#else

// %0-360
#define EOH_ATAN2(x, y) (int) ((atan2(x, y) + PI) * 9.f / (2 * PI))
#endif

// 计算梯度积分矩阵 [h×w×1] => [h+1×w+1×10]
int EOH_grads_integral(int h, int w, const unsigned char* img, int al, real* mag, int ml, int mi)
{
  int i, j, k;

  //unsigned char* sob = MALLOC(unsigned char, h*w);
  //real* sobf = MALLOC(real, h*w);
  real* mag0 = mag;

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
      real grads, *mag1, *mag2;

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

      grads = (real) sqrt((dx * dx) + (dy * dy));         //%梯度模值和
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
    real ssmin, real ssmax, real ss, int stepxy, XRECT* B, int B_len)
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
    real invarea = 1.f / (cx * cy);
    real* inter;
    inter = MALLOCSET(real, siz, 0);

    if (NULL == tmpimg1) {
      IMRESIZE_AREA(height, width, data, step, h, w, tmpimg, w, 1);
    }
    else {
      IMRESIZE_AREA(height1, width1, tmpimg1, step1, h, w, tmpimg, w, 1);
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
        real* tmpSamp = inter + y * stp + x * INTEGRAL_CN;
        real* tmpSamp1 = tmpSamp + 1;
        real score, invvar;
        int ispass = 0;
        SKIP_INTEGRAL_VAR(tmpSamp, stp * cy, INTEGRAL_CN * cx, tmpSamp1, stp * cy, INTEGRAL_CN * cx, invarea, invvar);
        invvar = (invvar <= 0.f) ? 1.f : 1.f / (real) sqrt(invvar);
        //CASCADE_PASS(ca, tmpSamp, ca->haar, HAAR_FEAT_VAL, invvar, ispass, score);
        cascade_pass(ca, tmpSamp, invvar, &ispass, &score);

        if (ispass) {
          B[ len ].x = XFLOOR(x * ssmin);
          B[ len ].y = XFLOOR(y * ssmin);
          B[ len ].w = XFLOOR(cx * ssmin);
          B[ len ].h = XFLOOR(cy * ssmin);
          B[ len ].count = 1;
          B[ len ].score = score;
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
    real ssmin, real ssmax, real ss, int stepxy, XRECT* B, int B_len)
{
  int len = 0, cx = ca->w, cy = ca->h, cx0 = ca->w, cy0 = ca->h;
  int i, w = (width), h = (height);
  int INTEGRAL_CN = _feat_fun[ ca->feat_type ].INTEGRAL_CN;
  int stp = (w + 1) * INTEGRAL_CN;
  int siz = (h + 1) * stp;
  char* chptr;
  unsigned char* tmpimg = MALLOC(unsigned char, w * h);
  real* inter;
  inter = MALLOCSET(real, siz, 0);
  cx = XFLOOR(cx0 * ssmin);
  cy = XFLOOR(cy0 * ssmin);
  IMRESIZE(height, width, data, step, 1, h, w, tmpimg, w, 1, INTER_BILINEARITY);
  _feat_fun[ ca->feat_type ].integral(h, w, tmpimg, w, inter, stp, INTEGRAL_CN);

  for (; len < B_len && w >= cx && h >= cy; ssmin *= ss, cx = XFLOOR(cx0 * ssmin), cy = XFLOOR(cy0 * ssmin)) {
    int x, y;
    int ranx = w - cx + 1;
    int rany = h - cy + 1;
    int stepx = stepxy, stepy = stepxy;
    real invarea = 1.f / (cx * cy);
    real invvar;
    //FEAT_SETOFFSET( ca->eohhaar, ca->weaklen, stp, INTEGRAL_CN, ssmin, EOHHAAR );
    chptr = (char*)(ca->featptr);

    for (i = 0; i < ca->weaklen; ++i, chptr += _feat_fun[ ca->feat_type ].elemsize) {
      if (_feat_fun[ ca->feat_type ].setoffset) {
        _feat_fun[ ca->feat_type ].setoffset(chptr, stp, INTEGRAL_CN, 1.f);
      }
    }

    for (y = 0; len < B_len && y < rany; y += stepy) {
      for (x = 0; len < B_len && x < ranx; x += stepx) {
        real* tmpSamp = inter + y * stp + x * INTEGRAL_CN;
        real* tmpSamp0 = tmpSamp + EOH_CN;
        real* tmpSamp1 = tmpSamp0 + 1;
        int ispass = 0;
        real score;
        SKIP_INTEGRAL_VAR(tmpSamp0, stp * cy, INTEGRAL_CN * cx, tmpSamp1, stp * cy, INTEGRAL_CN * cx, invarea, invvar);
        invvar = (invvar <= 0.f) ? 1.f : 1.f / (real) sqrt(invvar);
        cascade_pass(ca, tmpSamp, invvar, &ispass, &score);

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

#define MAKE_IRECTW(IRECTW, X0, Y0, CX0, CY0, W0) \
  do \
  { \
    (IRECTW).x = X0; \
    (IRECTW).y = Y0; \
    (IRECTW).w = CX0; \
    (IRECTW).h = CY0; \
    (IRECTW).d = W0; \
  } while (0)
// 正
#define SETRECT2(a, b, x0, y0, cx0, cy0, w0, x1, y1, cx1, cy1, w1) \
  do \
  { \
    if (testx[a] && testy[b]) \
    { \
      if (out) \
      { \
        MAKE_IRECTW((f)->ft0, x0, y0, cx0, cy0, w0); \
        MAKE_IRECTW((f)->ft1, x1, y1, cx1, cy1, w1); \
      } \
      ++(f); \
    } \
  } while (0)
// 斜
#define TILRECT2(a, b, x0, y0, cx0, cy0, w0, x1, y1, cx1, cy1, w1) \
  if (((x + a * dx) <= w && (x - b * dy) >= 0 && (y + a * dx + b * dy) <= h)) \
  { \
    (f++)->init(al, ai, 1, x0, y0, cx0, cy0, w0, x1, y1, cx1, cy1, w1, 0, 0, 0, 0, 0) \
  }

int get_haar_feat(int h, int w, int mincx, int mincy, int a, int b, HAARFEAT* out)
{
  HAARFEAT* f = out;

  // 在这计算19中Haar特征在特定的长宽下的特征数目
  // 19中特征可以分为12中情况计算
  // 开始计算特征!
  int x, y, dx, dy, len = 0;

  for (y = 0; y < h; y += a) {
    for (x = 0; x < w; x += a) {
      for (dy = mincy; dy <= h; dy += b) {
        for (dx = mincx; dx <= w; dx += b) {
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
#if 0
          // 2×2
          SETRECT2(2, 2, x, y, dx2, dy2, -1, x, y, dx, dy, 4);
          SETRECT2(2, 2, x, y, dx2, dy2, -1, x + dx, y, dx, dy, 4);
          SETRECT2(2, 2, x, y, dx2, dy2, -1, x, y + dy, dx, dy, 4);
          SETRECT2(2, 2, x, y, dx2, dy2, -1, x + dx, y + dy, dx, dy, 4);

          //SETRECT2(2,2,x,y,dx2,dy2,-1,x,y,dx,dy,2,x+dx,y+dy,dx,dy,2);

          // 3×3
          SETRECT2(3, 3, x, y, dx3, dy3, -4, x, y, dx2, dy2, 9);
          SETRECT2(3, 3, x, y, dx3, dy3, -4, x + dx, y, dx2, dy2, 9);
          SETRECT2(3, 3, x, y, dx3, dy3, -4, x, y + dy, dx2, dy2, 9);
          SETRECT2(3, 3, x, y, dx3, dy3, -4, x + dx, y + dy, dx2, dy2, 9);

          // 3×3
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

      for (dy = 1; dy <= h; dy += a) {
        for (dx = 1; dx <= w; dx += a) {
#if 0
          int a = dx, b = dy;
          int dx2 = dx * 2, dx3 = dx * 3, dx4 = dx * 4;
          int dy2 = dy * 2, dy3 = dy * 3, dy4 = dy * 4;

          //x+a*dx-b*dy;
          //y+a*dx+b*dy;
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

  return f - out;
}

#undef SETRECT2
#undef SETRECT3
int read_rect_from_file(const char* fname, EOHFEAT* out, int cn)
{
  int i, j, feat_num = 0;
  char buf[ 256 ];
  FILE* pf = fopen(fname, "r");

  if (pf) {
    EOHFEAT eoh = { 0 };

    for (i = 0; fscanf(pf, "%s", buf) > 0; ++i) {}

    feat_num = i * cn;

    if (out) {
      fseek(pf, 0, SEEK_SET);
      feat_num = i * cn;

      for (i = 0; fscanf(pf, "%d,%d,%d,%d", &(eoh.x), &(eoh.y), &(eoh.w), &(eoh.h)) > 0; ++i) {
        EOHFEAT* peoh = out + i * cn;

        for (j = 0; j < cn; ++j) {
          peoh[ j ] = eoh;
          peoh[ j ].bi = j;

          //peoh[j].type = FT_EOH;
        }

        // phog[j].type = FT_ED; ++j;
      }
    }

    fclose(pf);
  }

  return feat_num;
}

int xml_strchr(FILE* pf, int chr)
{
  for (; !feof(pf);) {
    char ch = fgetc(pf);

    if (ch == chr) {
      return 1;
    }
  }

  return 0;
}

int xml_strcmp(FILE* pf, const char* str)
{
  int i;

  for (i = 0; str[i]; ++i) {
    char ch = fgetc(pf);

    if (ch != str[i]) {
      return ch > str[i] ? 1 : -1;
    }
  }

  return 0;
}
int xml_strstr(FILE* pf, const char* str)
{
  int i;

  for (; xml_strchr(pf, str[0]);) {
    for (i = 1; str[i]; ++i) {
      char ch = fgetc(pf);

      if (ch != str[i]) {
        break;
      }
    }

    if (0 == str[i]) {
      return 1;
    }
  }

  return 0;
}

int xml_if_begin(FILE* pf, const char* name)
{
  int pos = ftell(pf);
  int ret = xml_strchr(pf, '<') && (0 == xml_strcmp(pf, name)) && xml_strchr(pf, '>');
  int pos2 = ftell(pf);

  if (!ret) {
    fseek(pf, pos, SEEK_SET);
  }

  return ret;
}

int xml_begin(FILE* pf, const char* name)
{
  int ret = xml_strchr(pf, '<') && (0 == xml_strcmp(pf, name)) && xml_strchr(pf, '>');

  if (!ret) {
    printf("xml_begin error: %s\n", name);
  }

  ASSERT(ret);
  return ret;
}

int xml_end(FILE* pf, const char* name)
{
  int ret = xml_strstr(pf, "</") && (0 == xml_strcmp(pf, name)) && xml_strchr(pf, '>');

  if (!ret) {
    printf("xml_end error: %s\n", name);
  }

  ASSERT(ret);
  return ret;
}

int fgeti(FILE* pf)
{
  int i;
  fscanf(pf, "%d", &i);
  return i;
}
double fgetf(FILE* pf)
{
  double f;
  fscanf(pf, "%lf", &f);
  return f;
}

#define XML_BEG(pf, name)          if (xml_begin(pf, name)) {const char* __NAME=name
#define XML_BEGFOR(pf, name, _I)   for (_I=0; xml_if_begin(pf, name); ++(_I)) {const char* __NAME=name
//#define XML_END(pf, name)   xml_end(pf, name);}
#define XML_END(pf)                xml_end(pf, __NAME);}

CASCADE* read_cascade_xml(const char* xml)
{
  FILE* pf = fopen(xml, "rb");
  CASCADE* ca = NULL;

  if (pf) {
    int i, len, stagelen;
    IRECTW r;
    ca = new_cascade(100, 10000, FT_HAAR3, WT_GEN);
    xml_strstr(pf, "-->");
    XML_BEG(pf, "opencv_storage");
    XML_BEG(pf, "haarcascade_frontalface_alt");
    XML_BEG(pf, "size");
    fscanf(pf, "%d %d", &ca->w, &ca->h);
    XML_END(pf);
    XML_BEG(pf, "stages");
    {
      XML_BEGFOR(pf, "_", stagelen);
      xml_strstr(pf, "-->"); // <!-- stage 0 -->
      XML_BEG(pf, "trees");
      {
        XML_BEGFOR(pf, "_", len);
        xml_strstr(pf, "-->"); //<!-- tree 0 -->
        XML_BEG(pf, "_");
        xml_strstr(pf, "-->"); //<!-- root node -->
        XML_BEG(pf, "feature");
        XML_BEG(pf, "rects");
        {
          double f;
          XML_BEGFOR(pf, "_", i);
          fscanf(pf, "%d %d %d %d %lf", &r.x, &r.y, &r.w, &r.h, &f);
          //printf("%d\t%d %d %d %d %g\n", i, r.x, r.y, r.w, r.h, f);
          r.d = (int)f;
          ASSERT(i < 3);

          if (i < 3) {
            if (0 == i) {
              ca->haar3[ca->weaklen].ft0 = r;
            }
            else if (1 == i) {
              ca->haar3[ca->weaklen].ft1 = r;
            }
            else if (2 == i) {
              ca->haar3[ca->weaklen].ft2 = r;
            }
          }

          XML_END(pf);
        }
        XML_END(pf);
        XML_BEG(pf, "tilted");
        //{int t; fscanf(pf, "%d", &t);}
        XML_END(pf);
        XML_END(pf);
        XML_BEG(pf, "threshold");
        ca->gen[ca->weaklen].thd = (real)fgetf(pf);
        XML_END(pf);
        XML_BEG(pf, "left_val");
        ca->gen[ca->weaklen].val[0] = (real)fgetf(pf);
        XML_END(pf);
        XML_BEG(pf, "right_val");
        ca->gen[ca->weaklen].val[1] = (real)fgetf(pf);
        XML_END(pf);
        XML_END(pf);
        //printf("\t%g %g %g\n", ca->gen[ca->weaklen].thd, ca->gen[ca->weaklen].val[0], ca->gen[ca->weaklen].val[0]);
        ca->weaklen++;
        ASSERT(ca->weaklen < ca->maxweaklen);
        XML_END(pf);
      }
      ca->stage[ca->stagelen].len = len;
      XML_END(pf);
      XML_BEG(pf, "stage_threshold");
      ca->stage[ca->stagelen].thd = (real)fgetf(pf);
      XML_END(pf);
      XML_BEG(pf, "parent");
      //{int t; fscanf(pf, "%d", &t);}
      XML_END(pf);
      XML_BEG(pf, "next");
      //{int t; fscanf(pf, "%d", &t);}
      XML_END(pf);
      XML_END(pf);
    }
    ca->stagelen = stagelen;
    XML_END(pf);
    XML_END(pf);
    XML_END(pf);
    fclose(pf);
  }

  return ca;
}
