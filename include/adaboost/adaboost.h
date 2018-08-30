
#ifndef _ADABOOST_H_
#define _ADABOOST_H_
#include <stdio.h>
#include "cstd.h"
#include "inifile.h"
#define X_ESP   (0.00001f)
#define STATIC static

typedef double(*_FEAT_VAL_FUN_T)(double* samp, int al, void* feat, int i, double iv);
typedef double(*_WEAK_VAL_FUN_T)(double featval, void*, int i);
typedef int (*_FPRINT_FUN_T)(FILE* pf, const void* ptr);
typedef int (*_FSCANF_FUN_T)(FILE* pf, void* ptr);
typedef int (*_SETOFFSET_FUN_T)(void* eoh, int al, int ai, double cs);
typedef int (*_INTEGRAL_FUN_T)(int h, int w, const unsigned char* img, int al, double* mag, int ml, int mi);

#define WEAK_FUN(_PTR)  (*((_PTR)->weakfun))
#define FEAT_FUN(_PTR)  (*((_PTR)->featfun))

#define DELIMT    " "
#define _D(_F)   "%"#_F DELIMT
#define _DF()    FLOATFMT DELIMT
//#define _F()   "%f" DELIMT
#define _F()   "%lf" DELIMT

#define DO_PRINT_PRO(_PRO, _ALL, _TIME) { if ((_ALL) < 200 || !((_PRO) % ((_ALL) / MIN(_ALL, 10000))) || ((_ALL) - (_PRO)) < 3) \
    { printf("%6d|%9g %9g| %9g  %9g  %9d [%4.1f%%]\r", pwi->id, pwi->Ha, pwi->Fa, pwi->Error, pwi->Threshold, _TIME, ((_PRO + 1) * 100.f / (_ALL)) ); } }

CC_INLINE int setptr(void** pa, void* b)
{
  *pa = b;
  return 0;
}

#define SETPTR(a, b)   setptr((void**)&(a), (void*)(b))

#define INIT_FEAT_FUN(ptr, _FEAT) \
  ptr->name = #_FEAT, \
      SETPTR(ptr->val, _FEAT##_feat_val), \
      SETPTR(ptr->print, _FEAT##_print),    \
      SETPTR(ptr->scanf, _FEAT##_scanf),    \
      SETPTR(ptr->integral, _FEAT##_integral), \
      SETPTR(ptr->setoffset, _FEAT##_setoffset), \
      ptr->INTEGRAL_CN= _FEAT##_CN, \
          ptr->elemsize= sizeof(_FEAT##FEAT)

#define INIT_WEAK_FUN(ptr, _WEAK) \
  ptr->name = #_WEAK, \
      SETPTR(ptr->val, _WEAK##_value), \
      SETPTR(ptr->print, _WEAK##_print), \
      SETPTR(ptr->scanf, _WEAK##_scanf), \
      ptr->elemsize = sizeof(_WEAK##WEAK)

#define INIT_CA(ptr, _FEAT, _WEAK)  (INIT_FEAT_FUN(ptr->featfun, _FEAT), INIT_WEAK_FUN(ptr->weakfun, _WEAK))

typedef struct {
  const char* name;
  size_t elemsize;
  size_t INTEGRAL_CN;
  _FEAT_VAL_FUN_T val;
  _FPRINT_FUN_T print;
  _FSCANF_FUN_T scanf;
  _SETOFFSET_FUN_T setoffset;
  _INTEGRAL_FUN_T integral;
}
feat_fun_t;

typedef struct {
  const char* name;
  size_t elemsize;
  _WEAK_VAL_FUN_T val;
  _FPRINT_FUN_T print;
  _FSCANF_FUN_T scanf;
}
weak_fun_t;

typedef struct _STAGE {
  int len;  // 包含弱分类器数目
  double thd; // 强分类器阈值
}
STAGE;

typedef struct cascade_t {
  int stagelen;     // 强分类器数
  int weaklen;      // 弱分类器数
  int maxstagelen;  // 最大强分类器数
  int maxweaklen;   // 最大弱分类器数
  int w, h;       // 最小检测框
  int is_nesting;   // 使用nesting结构
  feat_fun_t featfun[1];
  weak_fun_t weakfun[1];
  int* id;
  STAGE* stage;
  char* weakptr;
  char* featptr;
} cascade_t;

typedef cascade_t CASCADE;



typedef struct _WORKINFO {
  double Ha, Fa, Error, Threshold, Time;
  int id, beg, end;
  double val[ 4 ];
}
WORKINFO;

typedef struct adaboost_t BOOST;
typedef int (*f_init_feat_t)(BOOST* hh);

typedef struct featlib_fun_t {
  int (*f_find_samp)(BOOST* hh, int type);
  double* (*f_get_feat)(BOOST* hh, int i);       //取所有样本的第i个特征
  int (*f_save_feat)(BOOST* hh, int id);
  int (*f_uninit_feat)(BOOST* hh);
  int (*f_samp_data)(BOOST* hh, char** buf, int* size);
  int (*f_init_feat)(BOOST* hh);
  int (*f_get_error)(BOOST* hh, WORKINFO* pwi);
  int (*f_update_weight)(BOOST* hh, WORKINFO* pwi);
} featlib_fun_t;

#define INIT_FEATLIB_FUN(ptr, _FEAT, _WEAK) \
  SETPTR(ptr->f_find_samp, _FEAT##_find_samp) , \
  SETPTR(ptr->f_get_feat, _FEAT##_feat) , \
  SETPTR(ptr->f_save_feat, _FEAT##_save_feat), \
  SETPTR(ptr->f_uninit_feat, _FEAT##_uninit), \
  SETPTR(ptr->f_samp_data, _FEAT##_samp_data), \
  SETPTR(ptr->f_init_feat, _FEAT##_init), \
  SETPTR(ptr->f_get_error, _WEAK##_get_error), \
  SETPTR(ptr->f_update_weight, _WEAK##_update_weight);



#define INIT_CA_FUN(ptr, _FEAT, _WEAK)   INIT_FEAT_FUN(ptr->featfun, _FEAT), INIT_WEAK_FUN(ptr->weakfun, _WEAK)
#define INIT_FUN(ptr, _FEAT, _WEAK)   INIT_CA_FUN(ptr->ca, _FEAT, _WEAK), INIT_FEATLIB_FUN(ptr->featlibfun, _FEAT, _WEAK)

// adaboost训练
typedef struct adaboost_t {
  const char* disc1; //描述
  featlib_fun_t featlibfun[1];
  double* vi;           // [samp_num] 样本得分
  double* wi;           // [samp_num] 样本权重
  double* fi;           // [samp_num] 特征数据权重
  cascade_t ca[1];      // 分类器
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
}
adaboost_t;

typedef adaboost_t BOOST;

//#define GetFeatNum(hh)   (hh)->feat_num
//#define GetSampNum(hh)   (hh)->samp_num
#define GetPosNum(hh)        (hh)->pos_num
#define GetNegNum(hh)        ((hh)->samp_num - (hh)->pos_num)
#define getfeat(hh, i)       (hh)->featlibfun->f_get_feat(hh, i)
#define findsamp(hh, t)      (hh)->featlibfun->f_find_samp(hh, t)
#define savefeat(hh, t)      (hh)->featlibfun->f_save_feat(hh, t)
#define updateweight(hh, t)  (hh)->featlibfun->f_update_weight(hh, t)
#define geterror(hh, t)      (hh)->featlibfun->f_get_error(hh, t)
#define uninit_feat(hh)      (hh)->featlibfun->f_uninit_feat(hh)
#define init_feat(hh)        (hh)->featlibfun->f_init_feat(hh)

void cascade_pass0(double* _SAMP, int al, STAGE* _STG, int _STGLEN,
 void* _FEAT, _FEAT_VAL_FUN_T _FEAT_VAL, void* _WEAK, _WEAK_VAL_FUN_T _WEAK_VAL,
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
  _FEAT_VAL = (_CA) ->featfun->val;
  _WEAK_VAL = (_CA) ->weakfun->val;

  if (NULL == _WEAK_VAL) {
    cascade_pass3(_SAMP, al, _STG, _STGLEN, _FEAT, _FEAT_VAL, _IV, _ISPASS, _PSCORE);
  }
  else {
    if ((_CA) ->is_nesting) {
      cascade_pass1(_SAMP, al, _STG, _STGLEN, _FEAT, _FEAT_VAL,
          _WEAK, _WEAK_VAL, _IV, _ISPASS, _PSCORE);
    }
    else {
      cascade_pass0(_SAMP, al, _STG, _STGLEN, _FEAT, _FEAT_VAL,
          _WEAK, _WEAK_VAL, _IV, _ISPASS, _PSCORE);
    }
  }
}

int getweakcnt(const CASCADE* ca)
{
  int i = 0, cnt = 0;

  for (i = 0; i < ca->stagelen; ++i) {
    cnt += ca->stage[ i ].len;
  }

  return cnt;
}

int cascade_setsize(CASCADE* ca, int maxstagelen, int maxweaklen)
{
  ASSERT(ca->stagelen < maxstagelen);
  ASSERT(ca->weaklen < maxweaklen);

  if (ca->maxstagelen < maxstagelen) {
    ca->stage = (STAGE*)realloc(ca->stage, sizeof(STAGE) * maxstagelen);
    ca->maxstagelen = maxstagelen;
  }

  ASSERT(ca->weakfun->elemsize > 0);
  ASSERT(ca->featfun->elemsize > 0);

  if (ca->maxweaklen < maxweaklen) {
    ca->weakptr = (char*)realloc(ca->weakptr, ca->weakfun->elemsize * maxweaklen);
    ca->featptr = (char*)realloc(ca->featptr, ca->featfun->elemsize * maxweaklen);
    ca->id = (int*)realloc(ca->id, sizeof(int) * maxweaklen);
    ca->maxweaklen = maxweaklen;
  }

  return 0;
}

int cascade_load_file(FILE* pf, CASCADE* ptr)
{
  CASCADE ca;

  if (NULL == pf) {
    return 0;
  }

  fread(&ca, sizeof(CASCADE), 1, pf);

  if (ca.weaklen >= 0 && ca.weaklen < 100000 && ca.stagelen >= 0 && ca.stagelen < 1000) {
    cascade_setsize(ptr, ca.maxstagelen, ca.maxweaklen);
    ptr->h = ca.h;
    ptr->w = ca.w;
    ptr->weaklen = ca.weaklen;
    ptr->stagelen = ca.stagelen;
    ptr->is_nesting = ca.is_nesting;
    fread(ptr->stage, sizeof(STAGE), ptr->stagelen, pf);
    fread(ptr->id, sizeof(int), ptr->weaklen, pf);
    fread(ptr->weakptr, sizeof(char), ptr->weakfun->elemsize * ptr->weaklen, pf);
    fread(ptr->featptr, sizeof(char), ptr->featfun->elemsize * ptr->weaklen, pf);
  }

  return 0;
}

int cascade_load(const char* datfilename, CASCADE* ptr)
{
  FILE* pf = fopen(datfilename, "rb");

  if (NULL == pf) {
    return 0;
  }

  cascade_load_file(pf, ptr);
  fclose(pf);
  return 0;
}

int cascade_save_file(const CASCADE* ptr, FILE* pf)
{
  fwrite(ptr, sizeof(CASCADE), 1, pf);
  fwrite(ptr->stage, sizeof(STAGE), ptr->stagelen, pf);
  fwrite(ptr->id, sizeof(int), ptr->weaklen, pf);
  fwrite(ptr->weakptr, sizeof(char), ptr->weakfun->elemsize * ptr->weaklen, pf);
  fwrite(ptr->featptr, sizeof(char), ptr->featfun->elemsize * ptr->weaklen, pf);
  return 0;
}
int cascade_save(const char* datfilename, const CASCADE* ptr)
{
  FILE* pf = fopen(datfilename, "wb");

  if (NULL == pf) {
    return 0;
  }

  cascade_save_file(ptr, pf);
  fclose(pf);
  return 0;
}

#define CAS_FMT _D(d)_D(d)_D(d)_D(d)_D(d)
void cascade_printf(FILE* pf, const CASCADE* ptr)
{
  int i;
  size_t weaksize, featsize;
  char* weakptr, * featptr;
  //cascade_save(ptr);
  fprintf(pf, CAS_FMT ENDL, ptr->stagelen, ptr->weaklen, ptr->w, ptr->h, ptr->is_nesting);

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

int cascade_scanf(FILE* pf, CASCADE* ptr)
{
  int i, ret, stagelen, weaklen, cx, cy, is_nesting;
  size_t weaksize, featsize;
  char* weakptr, * featptr;
  STAGE stg = { 0 };
  char* fmt = CAS_FMT ENDL;
  ret = fscanf(pf, CAS_FMT ENDL, &stagelen, &weaklen, &cx, &cy, &is_nesting);

  cascade_setsize(ptr, stagelen, weaklen);
  ptr->stagelen = stagelen;
  ptr->weaklen = weaklen;
  ptr->w = cx;
  ptr->h = cy;
  ptr->is_nesting = is_nesting;

  for (i = 0, weaklen = 0; i < ptr->stagelen; ++i) {
    fscanf(pf, _D(d) _F() ENDL, &ptr->stage[ i ].len, &ptr->stage[ i ].thd);
    weaklen += ptr->stage[ i ].len;
  }

  ASSERT(weaklen <= ptr->weaklen);
  weaksize = WEAK_FUN(ptr).elemsize;
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
  return 0;
}

int cascade_save_inl(const char* fn, const CASCADE* ptr)
{
  FILE* pf;
  pf = fopen(fn, "wb");

  if (NULL == pf) {
    return 0;
  }

  savedata2txt_impl(pf, 1, sizeof(CASCADE), ptr, 1, "", "0x%02x,\n", "");
  savedata2txt_impl(pf, 1, sizeof(STAGE)*ptr->stagelen, ptr->stage, 1, "", "0x%02x,\n", "");
  savedata2txt_impl(pf, 1, WEAK_FUN(ptr).elemsize * ptr->weaklen, ptr->weakptr, 1, "", "0x%02x,\n", "");
  savedata2txt_impl(pf, 1, FEAT_FUN(ptr).elemsize * ptr->weaklen, ptr->featptr, 1, "", "0x%02x,\n", "");
  fclose(pf);
  return 0;
}

int cascade_load_buf(const void* buf, CASCADE* ptr)
{
  CASCADE* ca = (CASCADE*)buf;
  ASSERT(WEAK_FUN(ca).elemsize == WEAK_FUN(ptr).elemsize);
  ASSERT(FEAT_FUN(ca).elemsize == FEAT_FUN(ptr).elemsize);
  ptr->h = ca->h, ptr->w = ca->w, ptr->is_nesting = ca->is_nesting;
  ptr->maxstagelen = ptr->stagelen = ca->stagelen;
  ptr->maxweaklen = ptr->weaklen = ca->weaklen;
  ptr->stage = (STAGE*)(ca + 1);
  ptr->weakptr = (char*)(ptr->stage + ca->stagelen);
  ptr->featptr = (char*)((char*)(ptr->weakptr) + WEAK_FUN(ptr).elemsize * ca->weaklen);
  return 0;
}

int cascade_save_txt(const char* filename, const CASCADE* ptr)
{
  FILE* pf = fopen(filename, "wb");

  if (NULL == pf) {
    return 0;
  }

  cascade_printf(pf, ptr);
  fclose(pf);
  return 0;
}
int cascade_load_txt(const char* filename, CASCADE* ptr)
{
  FILE* pf = fopen(filename, "rb");

  if (NULL == pf) {
    return 0;
  }

  cascade_scanf(pf, ptr);
  fclose(pf);
  return 0;
}

int cascade_free(CASCADE* ca)
{
  FREE(ca->stage);
  FREE(ca->weakptr);
  FREE(ca->featptr);
  FREE(ca->id);
  return 0;
}

#endif // _ADABOOST_H_
