
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "macro.h"
#include "imresize.h"
//#include "face/_faceDetect.h"
#include "cstd.h"

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

#define INIT_FEAT_FUN(_TYPE, _NAME) \
  _feat_fun[FT_##_TYPE].name         = _NAME, \
      _feat_fun[FT_##_TYPE].val          = _TYPE##_feat_val, \
          _feat_fun[FT_##_TYPE].print        = _TYPE##_print,    \
              _feat_fun[FT_##_TYPE].scanf        = _TYPE##_scanf,    \
                  _feat_fun[FT_##_TYPE].integral     = _TYPE##_integral, \
                      _feat_fun[FT_##_TYPE].setoffset    = _TYPE##_setoffset, \
                          _feat_fun[FT_##_TYPE].INTEGRAL_CN  = _TYPE##_CN, \
                              _feat_fun[FT_##_TYPE].elemsize     = sizeof(_TYPE##FEAT)

#define INIT_WEAK_FUN(_TYPE, _NAME) \
  _weak_fun[WT_##_TYPE].name      = _NAME, \
      _weak_fun[WT_##_TYPE].val      = _TYPE##_value, \
          _weak_fun[WT_##_TYPE].print    = _TYPE##_print, \
              _weak_fun[WT_##_TYPE].scanf    = _TYPE##_scanf, \
                  _weak_fun[WT_##_TYPE].elemsize = sizeof(_TYPE##WEAK)

#define weakelemsize(_type)  _weak_fun[ _type ].elemsize

void init_fun()
{
  STATIC int init = 0;

  if (0 == init) {
    init = 1;
#define FEATTYPEDEF(a, b)  INIT_FEAT_FUN(a, b);
#include "feattypedef.txt"
#undef FEATTYPEDEF

#define WEAKTYPEDEF(a, b)  INIT_WEAK_FUN(a, b);
#include "weaktypedef.txt"
#undef WEAKTYPEDEF
  }
}

void cascade_pass0(double* _SAMP, int al, STAGE* _STG, int _STGLEN,
    void* _FEAT, _FEAT_VAL_FUN_T _FEAT_VAL,
    void* _WEAK, _WEAK_VAL_FUN_T _WEAK_VAL,
    double _IV, int* _ISPASS, double* _PSCORE)
{
  int _I = 0, _J = 0, _P = 0;
  double x, y, _SCORE = 0.f;

  for (_I = 0, _P = 0; _I < (_STGLEN); ++_I) {
    _SCORE = 0.f;

    for (_J = _P; _J < (_P + (_STG) [ _I ].len); ++_J) {
      x = _FEAT_VAL(_SAMP, al, _FEAT, _J, _IV);
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
void cascade_pass1(double* _SAMP, int al, STAGE* _STG, int _STGLEN,
    void* _FEAT, _FEAT_VAL_FUN_T _FEAT_VAL,
    void* _WEAK, _WEAK_VAL_FUN_T _WEAK_VAL,
    double _IV, int* _ISPASS, double* _PSCORE)
{
  int _I = 0, _J = 0, _P = 0;
  double x, y, _SCORE = 0.f;

  if ((_STGLEN) > 0) {
    for (_J = _P; _J < (_P + (_STG) [ 0 ].len); ++_J) {
      x = _FEAT_VAL(_SAMP, al, _FEAT, _J, _IV);
      y = _WEAK_VAL(x, _WEAK, _J);
      _SCORE += y;
    }

    if (_SCORE >= (_STG) [ _I ].thd) {
      _P += (_STG) [ 0 ].len;

      for (_I = 1; _I < (_STGLEN); _P += (_STG) [ _I++ ].len) {
        _SCORE = _WEAK_VAL(_SCORE, _WEAK, _P);

        for (_J = _P + 1; _J < (_P + (_STG) [ _I ].len); ++_J) {
          x = _FEAT_VAL(_SAMP, al, _FEAT, _J, _IV);
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

void cascade_pass3(double* _SAMP, int al, STAGE* _STG, int _STGLEN,
    void* _FEAT, _FEAT_VAL_FUN_T _FEAT_VAL,
    double _IV, int* _ISPASS, double* _PSCORE)
{
  int _I = 0, _J = 0, _P = 0;
  double y, _SCORE = 0.f;

  for (_I = 0, _P = 0; _I < (_STGLEN); ++_I) {
    _SCORE = 0.f;

    for (_J = _P; _J < (_P + (_STG) [ _I ].len); ++_J) {
      y = _FEAT_VAL(_SAMP, al, _FEAT, _J, _IV);
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

void cascade_pass(CASCADE* _CA, double* _SAMP, int al, double _IV, int* _ISPASS, double* _PSCORE)
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

  if (NULL == _WEAK_VAL) {
    cascade_pass3(_SAMP, al, _STG, _STGLEN, _FEAT, _FEAT_VAL, _IV, _ISPASS, _PSCORE);
  }
  else {
    if ((_CA) ->is_nesting) {
      ASSERT((_CA) ->weak_type != WT_CARTW);
      cascade_pass1(_SAMP, al, _STG, _STGLEN, _FEAT, _FEAT_VAL,
          _WEAK, _WEAK_VAL, _IV, _ISPASS, _PSCORE);
    }
    else {
      cascade_pass0(_SAMP, al, _STG, _STGLEN, _FEAT, _FEAT_VAL,
          _WEAK, _WEAK_VAL, _IV, _ISPASS, _PSCORE);
    }
  }
}


CASCADE* new_cascade_o(CASCADE* o)
{
  CASCADE* ptr = NULL;
  init_fun();
  ptr = MALLOCSET(CASCADE, 1, 0);
  *ptr = *o;
  //ptr->stage = MALLOC( STAGE, ptr->maxstagelen );
  ptr->stage = MALLOCSET(STAGE, ptr->maxstagelen, 0);
  ptr->id = MALLOCSET(int, ptr->maxweaklen, 0);
  ptr->weakptr = MALLOCSET(char, _weak_fun[ ptr->weak_type ].elemsize * ptr->maxweaklen, 0);
  ptr->featptr = MALLOCSET(char, _feat_fun[ ptr->feat_type ].elemsize * ptr->maxweaklen, 0);
  return ptr;
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
  //ptr->stage = MALLOC( STAGE, maxstagelen );
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

HAARCASCADE* cascade2haarcascade(const CASCADE* ptr)
{
  int i, j;
  HAARCASCADE* cas = (HAARCASCADE*) malloc(sizeof(HAARCASCADE) + ptr->stagelen * sizeof(fStage) + ptr->weaklen * sizeof(fHaar));
  fStage* stage = (fStage*)(cas + 1);
  fHaar* ihaar = (fHaar*)(stage + ptr->stagelen);
  cas->w = ptr->w, cas->h = ptr->h;
  cas->ih = ihaar, cas->ihlen = ptr->weaklen;
  cas->stage = stage, cas->stagelen = ptr->stagelen;

  for (i = 0; i < ptr->stagelen; ++i) {
    stage[ i ].len = ptr->stage[ i ].len;
    stage[ i ].thd = ptr->stage[ i ].thd;
  }

  for (i = 0; i < ptr->weaklen; ++i) {
    ihaar[ i ].n = 2;
    ihaar[ i ].ti = 0;
#define RECTCOPY(a, b) ((a).x = (b).x,(a).y = (b).y,(a).w = (b).w,(a).h = (b).h,(a).d = (b).d)

    RECTCOPY(ihaar[ i ].ft0, ptr->haar[ i ].ft0);
    RECTCOPY(ihaar[ i ].ft1, ptr->haar[ i ].ft1);
    ihaar[ i ].minval = ptr->lut[ i ].minval;
    ihaar[ i ].binval = ptr->lut[ i ].binval;

    for (j = 0; j < LUT_BIN; ++j) {
      ihaar[ i ].val[ j ] = ptr->lut[ i ].val[ j ];
    }
  }

  return cas;
}

#undef XROUND1
#define XROUND1 XFLOOR
//#include "cstd.h"
int convet_to_int(const HAARCASCADE* ca0)
{
  int i, j;
  HAARCASCADE* ca = (HAARCASCADE*) ca0;

  if (CAISHAVEFLOAT(ca)) {
    ca->ih[ 0 ].ti = (unsigned char) - 1;
    {
      fStage* src = ca->stage;
      iStage* dst = (iStage*) ca->stage;

      for (i = 0; i < ca->stagelen; ++i) {
        dst[ i ].thd = XROUND1(SHIFT_TOINT(src[ i ].thd));
      }
    }
    {
      fHaar* src = ca->ih;
      iHaar* dst = (iHaar*) ca->ih;

      for (i = 0; i < ca->ihlen; ++i, ++src, ++dst) {
        dst->minval = XROUND1(SHIFT_TOINT(src->minval));
        dst->binval = XROUND(SHIFT_TOINT(src->binval));

        for (j = 0; j < LUT_BIN; ++j) {
          dst->val[ j ] = XROUND1(SHIFT_TOINT(src->val[ j ]));
        }
      }
    }
  }

  return 0;
}

int cascade2mem(const CASCADE* ca0, const char* filename, int addr, int is_int)
{
  unsigned char* mem = 0;
  int len = 0;
  int isdat = (NULL != strstr(filename, ".dat"));
  HAARCASCADE* ca = cascade2haarcascade(ca0);

  assert(sizeof(fHaar) == sizeof(iHaar));
  assert(sizeof(fStage) == sizeof(iStage));

  if (is_int) {
    convet_to_int(ca);
  }

  len = sizeof(HAARCASCADE) + sizeof(STAGE) * ca->stagelen + sizeof(iHaar) * ca->ihlen;
  mem = malloc(len);
  assert(mem);
  memcpy(mem, ca, sizeof(HAARCASCADE));
  memcpy(mem + sizeof(HAARCASCADE), ca->stage, sizeof(fStage) * ca->stagelen);
  memcpy(mem + sizeof(HAARCASCADE) + sizeof(fStage) * ca->stagelen, ca->ih, sizeof(fHaar) * ca->ihlen);
  {
    FILE* fout = fopen(filename, "wb");

    if (fout) {
      data2men1(fout, mem, len, addr, isdat);
      fclose(fout);
    }
  }

  free(mem);
  free(ca);
  return 0;
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
  fprintf(pf, "#include \"../_faceDetect.h\"\n\n");

  fprintf(pf, "STATIC STAGE %s_STAGE[] = {\n", name);

  for (i = 0; i < ptr->stagelen; ++i) {
    fprintf(pf, _D(d) _DF() ENDL, ptr->stage[ i ].len, ptr->stage[ i ].thd);
  }

  fprintf(pf, "};\n\n");
  fprintf(pf, "DSP_STATIC iHaar %s_iHaar[] = {\n", name);
  weaksize = WEAK_FUN(ptr).elemsize ;
  featsize = FEAT_FUN(ptr).elemsize;
  weakptr = (char*)(ptr->weakptr);
  featptr = (char*)(ptr->featptr);

  for (i = 0; i < ptr->weaklen; ++i, weakptr += weaksize, featptr += featsize) {
    //fprintf( pf, "%6d ", ptr->id[ i ] );
    if (FEAT_FUN(ptr).print) {
      FEAT_FUN(ptr).print(pf, featptr);
    }

    if (WEAK_FUN(ptr).print) {
      WEAK_FUN(ptr).print(pf, weakptr);
    }

    fprintf(pf, ENDL);
  }

  fprintf(pf, "};\n\n");

  fprintf(pf, "DSP_STATIC HAARCASCADE %s_cascade = {", name);
  fprintf(pf, "%2d, %2d, %2d, %s_STAGE, %s_iHaar", ptr->w, ptr->h, ptr->stagelen, name, name);
  fprintf(pf, "};\n\n");

  fprintf(pf, "#endif _%s_\n", name);
  //fflush(pf);
  fclose(pf);
}

#define DATFILENAME "cas.dat"
CASCADE* cascade_load_file(FILE* pf)
{
  CASCADE ca;
  CASCADE* ptr = 0;

  if (NULL == pf) {
    return 0;
  }

  fread(&ca, sizeof(CASCADE), 1, pf);

  if (ca.weaklen >= 0 && ca.weaklen < 100000 && ca.stagelen >= 0 && ca.stagelen < 1000) {
    ptr = new_cascade_o(&ca);
    fread(ptr->stage, sizeof(STAGE), ptr->stagelen, pf);
    fread(ptr->id, sizeof(int), ptr->weaklen, pf);
    fread(ptr->weakptr, sizeof(char), _weak_fun[ ptr->weak_type ].elemsize * ptr->weaklen, pf);
    fread(ptr->featptr, sizeof(char), _feat_fun[ ptr->feat_type ].elemsize * ptr->weaklen, pf);
  }

  return ptr;
}

CASCADE* cascade_read()
{
  CASCADE* ptr = 0;
  FILE* pf = fopen(DATFILENAME, "rb");

  if (NULL == pf) {
    return 0;
  }

  ptr = cascade_load_file(pf);
  fclose(pf);
  return ptr;
}
int getweakcnt(const CASCADE* ca)
{
  int i = 0, cnt = 0;

  for (i = 0; i < ca->stagelen; ++i) {
    cnt += ca->stage[ i ].len;
  }

  return cnt;
}
int cascade_save_file(const CASCADE* ptr, FILE* pf)
{
  fwrite(ptr, sizeof(CASCADE), 1, pf);
  fwrite(ptr->stage, sizeof(STAGE), ptr->stagelen, pf);
  fwrite(ptr->id, sizeof(int), ptr->weaklen, pf);
  fwrite(ptr->weakptr, sizeof(char), _weak_fun[ ptr->weak_type ].elemsize * ptr->weaklen, pf);
  fwrite(ptr->featptr, sizeof(char), _feat_fun[ ptr->feat_type ].elemsize * ptr->weaklen, pf);
  return 0;
}
int cascade_save(const CASCADE* ptr)
{
  FILE* pf = fopen(DATFILENAME, "wb");

  if (NULL == pf) {
    return 0;
  }

  cascade_save_file(ptr, pf);
  fclose(pf);
  return 0;
}

int cascade_save2(const CASCADE* ptr)
{
  FILE* pf;
  int k, i, ns = ptr->stagelen;
  char buf[256];

  for (k = 1; k <= ns; ++k) {
    CASCADE ca = *ptr;
    _snprintf(buf, 256, "cas_stage%d.dat", k);
    pf = fopen(buf, "wb");

    if (NULL == pf) {
      return 0;
    }

    ca.stagelen = k;
    ca.weaklen = 0;

    for (i = 0; i < k; ++i) {
      ca.weaklen += ca.stage[i].len;
    }

    ptr = &ca;
    fwrite(&ca, sizeof(CASCADE), 1, pf);
    fwrite(ca.stage, sizeof(STAGE), ca.stagelen, pf);
    fwrite(ca.id, sizeof(int), ca.weaklen, pf);
    fwrite(ca.weakptr, sizeof(char), _weak_fun[ ca.weak_type ].elemsize * ca.weaklen, pf);
    fwrite(ca.featptr, sizeof(char), _feat_fun[ ca.feat_type ].elemsize * ca.weaklen, pf);
    fclose(pf);
  }

  return 0;
}

#define CAS_FMT _D(d)_D(d)_D(d)_D(d)_D(d)_D(d)_D(d)
void cascade_printf(const CASCADE* ptr, FILE* pf)
{
  int i;
  size_t weaksize, featsize;
  char* weakptr, * featptr;
  //cascade_save(ptr);
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

    if (FEAT_FUN(ptr).scanf) {
      FEAT_FUN(ptr).scanf(pf, featptr);
    }

    if (WEAK_FUN(ptr).scanf) {
      WEAK_FUN(ptr).scanf(pf, weakptr);
    }
  }

  ptr->weaklen = weaklen;
  return ptr;
}

int cascade_save_txt(const CASCADE* ptr, const char* filename)
{
  FILE* pf = fopen(filename, "wb");

  if (NULL == pf) {
    return 0;
  }

  cascade_printf(ptr, pf);
  fclose(pf);
  return 0;
}

CASCADE* cascade_read_txt(const char* filename)
{
  FILE* pf = fopen(filename, "rb");
  CASCADE* ptr;

  if (NULL == pf) {
    return 0;
  }

  ptr = cascade_scanf(pf, 1000, 10000);
  fclose(pf);
  return ptr;
}


// 求x/y的反正切值
#if 1

// %0-180
#define EOH_ATAN2(x, y) (int) ((atan(x / (y + ESP)) + (PI / 2)) * 9.f / PI)
#else

// %0-360
#define EOH_ATAN2(x, y) (int) ((atan2(x, y) + PI) * 9.f / (2 * PI))
#endif

// 计算梯度积分矩阵 [h×w×1] => [h+1×w+1×10]
int EOH_grads_integral(int h, int w, const unsigned char* img, int al, double* mag, int ml, int mi)
{
  int i, j, k;

  //unsigned char* sob = MALLOC(unsigned char, h*w);
  //double* sobf = MALLOC(double, h*w);
  double* mag0 = mag;

  FILL(((h + 1) * (w + 1)*mi), mag, 0);

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

  for (i = 0; str[ i ]; ++i) {
    char ch = fgetc(pf);

    if (ch != str[ i ]) {
      return ch > str[ i ] ? 1 : -1;
    }
  }

  return 0;
}
int xml_strstr(FILE* pf, const char* str)
{
  int i;

  for (; xml_strchr(pf, str[ 0 ]);) {
    for (i = 1; str[ i ]; ++i) {
      char ch = fgetc(pf);

      if (ch != str[ i ]) {
        break;
      }
    }

    if (0 == str[ i ]) {
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
      xml_strstr(pf, "-->");   // <!-- stage 0 -->
      XML_BEG(pf, "trees");
      {
        XML_BEGFOR(pf, "_", len);
        xml_strstr(pf, "-->");   //<!-- tree 0 -->
        XML_BEG(pf, "_");
        xml_strstr(pf, "-->");   //<!-- root node -->
        XML_BEG(pf, "feature");
        XML_BEG(pf, "rects");
        {
          double f;
          XML_BEGFOR(pf, "_", i);
          fscanf(pf, "%d %d %d %d %lf", &r.x, &r.y, &r.w, &r.h, &f);
          //printf("%d\t%d %d %d %d %g\n", i, r.x, r.y, r.w, r.h, f);
          r.d = (int) f;
          ASSERT(i < 3);

          if (i < 3) {
            if (0 == i) {
              ca->haar3[ ca->weaklen ].ft0 = r;
            }

            if (1 == i) {
              ca->haar3[ ca->weaklen ].ft1 = r;
            }

            if (2 == i) {
              ca->haar3[ ca->weaklen ].ft2 = r;
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
        ca->gen[ ca->weaklen ].thd = (double) fgetf(pf);
        XML_END(pf);
        XML_BEG(pf, "left_val");
        ca->gen[ ca->weaklen ].val[ 0 ] = (double) fgetf(pf);
        XML_END(pf);
        XML_BEG(pf, "right_val");
        ca->gen[ ca->weaklen ].val[ 1 ] = (double) fgetf(pf);
        XML_END(pf);
        XML_END(pf);
        //printf("\t%g %g %g\n", ca->gen[ca->weaklen].thd, ca->gen[ca->weaklen].val[0], ca->gen[ca->weaklen].val[0]);
        ca->weaklen++;
        ASSERT(ca->weaklen < ca->maxweaklen);
        XML_END(pf);
      }
      ca->stage[ ca->stagelen ].len = len;
      XML_END(pf);
      XML_BEG(pf, "stage_threshold");
      ca->stage[ ca->stagelen ].thd = (double) fgetf(pf);
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
