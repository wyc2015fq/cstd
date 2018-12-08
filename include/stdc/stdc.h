
#ifndef _STDC_H_
#define _STDC_H_

#ifdef _WIN32
#include <windows.h>
#else
#endif

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>

#undef ASSERT
#define ASSERT assert

#define CC_INLINE static

#ifndef UNUSED
#define UNUSED(__X)  { const void* _P0 = &(__X); _P0=&_P0; }
//#define UNUSED(x) (void)x
#endif
#ifdef __cplusplus
#define _INIT0
#else
#define _INIT0   ={0}
#endif
//
#define CC_ALIGN(n, x)   (((n)+((x)-1))&(~((x)-1)))
#define ARRSIZE(_T, _N)   (sizeof(_T)*(_N))
///
#define MALLOC(_TYPE, _N)             (_TYPE*)malloc((unsigned int)(sizeof(_TYPE)*(_N)))
#define CALLOC(_TYPE, _N)             (_TYPE*)calloc((unsigned int)(sizeof(_TYPE)*(_N)))
#define REALLOC(_TYPE, _PTR, _N)      _PTR=(_TYPE*)realloc(_PTR, (unsigned int)(sizeof(_TYPE)*(_N)))
#undef MYREALLOC
#define MYREALLOC(ptr, n)             *(void**)(&ptr) = realloc(ptr, n*sizeof(*ptr))
#define MYMALLOC(ptr, n)             *(void**)(&ptr) = malloc(n*sizeof(*ptr))
#undef FREE
#define FREE(_PTR)                    ((_PTR) ? (free((void*)(_PTR)), _PTR=0) : 0), (_PTR)=0
/////
#ifndef bzero
#define bzero(arr, n)  memset(arr, 0, n)
#endif
#define BZERO(arr, n)  memset(arr, 0, sizeof(*arr)*n)
#define BZERO1(arr)  memset(arr, 0, sizeof(*arr))
////
#define _1K   (1024)
#define _1M   ((_1K)*(_1K))
#define _1G   ((_1K)*(_1M))
#define CC_PI   (3.14159265358979323846264338327950288419716939937511)
#define CC_2PI 6.283185307179586476925286766559
#define CC_LOG2 0.69314718055994530941723212145818
#define _LOG2   CC_LOG2

////
#define MEM_GET16B(p, i, x) x = (i+=2, (((uchar*)p)[i-2]<<8) | (((uchar*)p)[i-1]))
#define MEM_GET16L(p, i, x) x = (i+=2, (((uchar*)p)[i-1]<<8) | (((uchar*)p)[i-2]))
#define MEM_GET32B(p, i, x) x = (i+=4, (((uchar*)p)[i-4]<<24) | (((uchar*)p)[i-3]<<16) | (((uchar*)p)[i-2]<<8) | (((uchar*)p)[i-1]))
#define MEM_GET32L(p, i, x) x = (i+=4, (((uchar*)p)[i-1]<<24) | (((uchar*)p)[i-2]<<16) | (((uchar*)p)[i-3]<<8) | (((uchar*)p)[i-4]))

#define MEM_SET16B(p, i, x) (((uchar*)p)[i+0] = ((x)>>8), ((uchar*)p)[i+1] = (x)&0xff, i+=2)
#define MEM_SET16L(p, i, x) (((uchar*)p)[i+1] = ((x)>>8), ((uchar*)p)[i+0] = (x)&0xff, i+=2)
#define MEM_SET32B(p, i, x) (((uchar*)p)[i+0] = ((x)>>24), ((uchar*)p)[i+1] = ((x)>>16), ((uchar*)p)[i+2] = ((x)>>8), ((uchar*)p)[i+3] = (x), i+=4)
#define MEM_SET32L(p, i, x) (((uchar*)p)[i+3] = ((x)>>24), ((uchar*)p)[i+2] = ((x)>>16), ((uchar*)p)[i+1] = ((x)>>8), ((uchar*)p)[i+0] = (x), i+=4)
#define MEM_SETMEM(p, i, x, n) (memcpy((uchar*)p + i, x, n), i+=n)

////
#define VEC_FREES(_P, _N, OBJ_FREE) { int i; for (i=0;i<_N; ++i) {OBJ_FREE((_P+i));} }
#define VEC_SETSIZE(_P, _N, _NEWN, OBJ_FREE) { \
  if (_NEWN<_N) { \
    int i; \
    for (i = _NEWN; i < _N; ++i) { \
      OBJ_FREE(_P + i); \
    } \
  } else if (_NEWN>_N) { \
    MYREALLOC(_P, _NEWN); \
    memset(_P+_N, 0, (_NEWN-_N)*sizeof(*_P)); \
  } \
  _N = _NEWN; \
}

/////////////////////////////////////////////////////////////////////////////////////////////////

#undef FALSE
#undef TRUE
#undef BOOL
typedef int BOOL;
enum { FALSE = 0, TRUE = 1 };
#define GETBUFVAL(_T, _P, _I)  ((_P) ? ((_T*)(_P))[_I] : 0)
#define GETBUFPTR(_T, _P, _I)  ((_P) ? (((_T*)(_P))+(_I)) : 0)
#define ABSSUB(a, b)  ((a)>(b) ? ((a)-(b)) : ((b)-(a)))

/* absolute value without jumps */
#ifndef __cplusplus
#define IABS(a)     (((a) ^ ((a) < 0 ? -1 : 0)) - ((a) < 0 ? -1 : 0))
#else
#define IABS(a)     abs(a)
#endif
#ifndef FLOOR
#define FLOOR(X)  (int)(X)
#endif
#ifndef ROUND
#define ROUND(x)   (int)((x)+0.5)
#endif
#ifndef CAST
#define CAST(TYPE, VALUE) ((TYPE)(VALUE))
#endif

//#define CC_SWAP(a,b,t) ((t) = (a), (a) = (b), (b) = (t))
#ifndef MIN
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif
#ifndef MAX
#define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif
#define MINMAX_UPDATE(x, mi, ma) if (x<mi) { mi = x; } else if (x>ma) { ma = x; }
#define MINMAX(x1, x2, t)  if ((x1)>(x2)) { CC_SWAP((x1), (x2), t); }
/* min & max without jumps */
#define CC_IMIN(a, b)  ((a) ^ (((a)^(b)) & (((a) < (b)) - 1)))
#define CC_IMAX(a, b)  ((a) ^ (((a)^(b)) & (((a) > (b)) - 1)))
/* absolute value without jumps */
#ifndef __cplusplus
#define CC_IABS(a)     (((a) ^ ((a) < 0 ? -1 : 0)) - ((a) < 0 ? -1 : 0))
#else
#define CC_IABS(a)     abs(a)
#endif
#define CC_CMP(a,b)    (((a) > (b)) - ((a) < (b)))
#define CC_SIGN(a)     CC_CMP((a),0)

/* CC_SWAP */
#ifndef CC_SWAP
#define CC_SWAP(_A,_B,tmp) (tmp = (_A), _A = (_B), _B = (tmp))
#endif
#ifndef SWAP
#define SWAP(a, b, t)   (t=a,a=b,b=t)
#endif
#ifndef T_SWAP
#define T_SWAP(_TYPE, _A,_B) {_TYPE _TMP; CC_SWAP(_A, _B, _TMP);}
#define SWAP_T(_A,_B, _TYPE) {_TYPE _TMP; CC_SWAP(_A, _B, _TMP);}
//#define T_SWAP(TYPE, a, b) {TYPE t=a; a=b, b=t;}
#endif

#define XCMPMEM(N0, D0, N1, D1, _OUT) { int I=0; _OUT=CC_CMP(N0,N1); for (;0==_OUT && I<N0; ++I) {_OUT=CC_CMP((D0)[I], (D1)[I]);} }

//
#define MEMCMP(_N, _F, _P, _R)  {int _I=0; for (; _I<_N && 0==(_R=CC_CMP(*(_F+_I),*(_P+_I))); ++_I); }
//
#define MEMRCMP(_N, _F, _P, _R) {int _I=_N; for (; (0<_I--) && 0==(_R=CC_CMP(*(_F+_I),*(_P+_I))); ); }
/* min & max without jumps */
#define IMIN(a, b)  ((a) ^ (((a)^(b)) & (((a) < (b)) - 1)))
#define IMAX(a, b)  ((a) ^ (((a)^(b)) & (((a) > (b)) - 1)))
//
#define CLAMP(_X, _MINVAL, _MAX_VAL)  ((_X)<(_MINVAL) ? (_MINVAL) : ((_X)>(_MAX_VAL) ? (_MAX_VAL) : (_X)))
#define CLAMP255(_X)    CLAMP(_X, 0, 255)
//
#ifndef BOUND
//#define BOUND(VAL, MINVAL, MAXVAL)  (MAX((MINVAL), MIN((VAL), (MAXVAL))))
#define BOUND(x,a,b) (((x) < (a)) ? (a) : (((x) > (b)) ? (b) : (x)))
#endif
#define BOUND_MAX(x, min, max)  x=((max)+(x)%(max))%(max)
#define BOUND_CLIP(_x, _min, _max)  { if (_x<_min) _x = _min; if (_x>_max) _x = _max; }
#define BOUND_NUL(x, min, max)
#define POINT_DIST2(x, y) ((x)*(x) + (y)*(y))
#define POINT_DIST(a, b) (sqrt((double)POINT_DIST2(a.x - b.x, a.y - b.y)))
///
#define MIN3(a,b,c)                      MIN(a, MIN(b, c))
#define MIN4(a,b,c,d)                    MIN3(MIN(a,b),c,d)
#define MIN5(a,b,c,d,e)                  MIN4(MIN(a,b),c,d,e)
#define MIN6(a,b,c,d,e,f)                MIN5(MIN(a,b),c,d,e,f)
#define MIN7(a,b,c,d,e,f,g)              MIN6(MIN(a,b),c,d,e,f,g)
#define MIN8(a,b,c,d,e,f,g,h)            MIN7(MIN(a,b),c,d,e,f,g,h)
#define MIN9(a,b,c,d,e,f,g,h,i)          MIN8(MIN(a,b),c,d,e,f,g,h,i)
#define MAX3(a,b,c)                      MAX(MAX(a,b), c)
#define MAX4(a,b,c,d)                    MAX3(MAX(a,b),c,d)
#define MAX5(a,b,c,d,e)                  MAX4(MAX(a,b),c,d,e)
#define MAX6(a,b,c,d,e,f)                MAX5(MAX(a,b),c,d,e,f)
#define MAX7(a,b,c,d,e,f,g)              MAX6(MAX(a,b),c,d,e,f,g)
#define MAX8(a,b,c,d,e,f,g,h)            MAX7(MAX(a,b),c,d,e,f,g,h)
#define MAX9(a,b,c,d,e,f,g,h,i)          MAX8(MAX(a,b),c,d,e,f,g,h,i)


/////////////////////////////////////////////////////////////////////////////////////////////////
#define _NOP(x)  (x)
/////////////////////////////////////////////////////////////////////////////////////////////////
#define FILL(_N, _F, _V)  do {int _I; for (_I=0; _I<(int)(_N); ++_I) *(_F+_I)=(_V);} while(0)
#define COPY(_N, _F, _P)  do {int _I; for (_I=0; _I<(int)(_N); ++_I) *(_P+_I)=*(_F+_I);} while(0)
#define SKIP_COPY_CAST(_H, _W, _A, _AL, _AI, _B, _BL, _BI, _CAST) \
  do { \
    int _A0, _A1, _B0, _B1; \
    for (_A0=0, _B0=0; _A0<((_H)*(_AL)); _A0+=(_AL), _B0+=_BL) \
      for (_A1=_A0, _B1=_B0; _A1<(_A0+(_W)*(_AI)); _A1+=(_AI), _B1+=(_BI)) \
        *(_B+_B1)=_CAST(*(_A+_A1)); \
  } while(0)
#define SKIP_COPY(_H, _W, _A, _AL, _AI, _B, _BL, _BI) \
  SKIP_COPY_CAST(_H, _W, _A, _AL, _AI, _B, _BL, _BI, _NOP)
#define MINMAX_ELEM(_N, _F, _FI, _MIN, _MAX) \
  do { \
    int _I; \
    if (_N) \
      for (_I=0, _MIN=0, _MAX=0; _I<(_N)*(_FI); _I+=(_FI)) \
        if (*(_F+_I)<*(_F+_MIN)) \
          (_MIN) = (_I); \
        else if (*(_F+_I)>*(_F+_MAX)) \
          (_MAX) = (_I); \
  } while(0)
#define MINMAX_ELEM2D(_M, _N, _F, _FL, _FI, _MIN, _MAX) \
  do { \
    int _I, _J; \
    if ((_M)*(_N)) \
      for (_J=0, _MIN=0, _MAX=0; _J<(_M)*(_FL); _J+=(_FL)) \
        for (_I=_J; _I<(_J+(_N)*(_FI)); _I+=(_FI)) \
          if (*(_F+_I)<*(_F+_MIN)) \
            (_MIN) = (_I); \
          else if (*(_F+_I)>*(_F+_MAX)) \
            (_MAX) = (_I); \
  } while(0)
/////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef countof
#define countof(_ARR)  (int)(sizeof(_ARR)/sizeof((_ARR)[0]))
#endif // countof
#define CC_8TO32F(x)  (x)
#define SQR(_X)    ((_X)*(_X))
#define SQR3(_X)   ((_X)*(_X)*(_X))
#define SQR4(_X)   ((_X)*(_X)*(_X)*(_X))
#define MAX_INT             0x7fffffff
#define MIN_INT             0x80000000
#define MAX_DOUBLE          1.7976931348623158E+308
#define MIN_DOUBLE         -1.7976931348623158E+308

/////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum CStatus {
  CC_BADMEMBLOCK_ERR = -113,
  CC_INPLACE_NOT_SUPPORTED_ERR = -112,
  CC_UNMATCHED_ROI_ERR = -111,
  CC_NOTFOUND_ERR = -110,
  CC_BADCONVERGENCE_ERR = -109,
  CC_BADDEPTH_ERR = -107,
  CC_BADROI_ERR = -106,
  CC_BADHEADER_ERR = -105,
  CC_UNMATCHED_FORMATS_ERR = -104,
  CC_UNSUPPORTED_COI_ERR = -103,
  CC_UNSUPPORTED_CHANNELS_ERR = -102,
  CC_UNSUPPORTED_DEPTH_ERR = -101,
  CC_UNSUPPORTED_FORMAT_ERR = -100,
  CC_BADARG_ERR = -49,  //ipp comp
  CC_NOTDEFINED_ERR = -48,  //ipp comp
  CC_BADCHANNELS_ERR = -47,  //ipp comp
  CC_BADRANGE_ERR = -44,  //ipp comp
  CC_BADSTEP_ERR = -29,  //ipp comp
  CC_BADFLAG_ERR = -12,
  CC_DIV_BY_ZERO_ERR = -11, //ipp comp
  CC_BADCOEF_ERR = -10,
  CC_BADFACTOR_ERR = -7,
  CC_BADPOINT_ERR = -6,
  CC_BADSCALE_ERR = -4,
  CC_OUTOFMEM_ERR = -3,
  CC_NULLPTR_ERR = -2,
  CC_BADSIZE_ERR = -1,
  CC_FAIL = -1,
  CC_NO_ERR = 0,
  CC_OK = CC_NO_ERR,

  CC_StsOk = 0,  //!< everithing is ok
  CC_StsBackTrace = -1,  //!< pseudo error for back trace
  CC_StsError = -2,  //!< unknown /unspecified error
  CC_StsInternal = -3,  //!< internal error (bad state)
  CC_StsNoMem = -4,  //!< insufficient memory
  CC_StsBadArg = -5,  //!< function arg/param is bad
  CC_StsBadFunc = -6,  //!< unsupported function
  CC_StsNoConv = -7,  //!< iter. didn't converge
  CC_StsAutoTrace = -8,  //!< tracing
  CC_HeaderIsNull = -9,  //!< image header is NULL
  CC_BadImageSize = -10,  //!< image size is invalid
  CC_BadOffset = -11,  //!< offset is invalid
  CC_BadDataPtr = -12,  //!<
  CC_BadStep = -13,  //!<
  CC_BadModelOrChSeq = -14,  //!<
  CC_BadNumChannels = -15,  //!<
  CC_BadNumChannel1U = -16,  //!<
  CC_BadDepth = -17,  //!<
  CC_BadAlphaChannel = -18,  //!<
  CC_BadOrder = -19,  //!<
  CC_BadOrigin = -20,  //!<
  CC_BadAlign = -21,  //!<
  CC_BadCallBack = -22,  //!<
  CC_BadTileSize = -23,  //!<
  CC_BadCOI = -24,  //!<
  CC_BadROISize = -25,  //!<
  CC_MaskIsTiled = -26,  //!<
  CC_StsNullPtr = -27,  //!< null pointer
  CC_StsVecLengthErr = -28,  //!< incorrect vector length
  CC_StsFilterStructContentErr = -29,  //!< incorr. filter structure content
  CC_StsKernelStructContentErr = -30,  //!< incorr. transform kernel content
  CC_StsFilterOffsetErr = -31,  //!< incorrect filter ofset value
  CC_StsBadSize = -201, //!< the input/output structure size is incorrect
  CC_StsDivByZero = -202, //!< division by zero
  CC_StsInplaceNotSupported = -203, //!< in-place operation is not supported
  CC_StsObjectNotFound = -204, //!< request can't be completed
  CC_StsUnmatchedFormats = -205, //!< formats of input/output arrays differ
  CC_StsBadFlag = -206, //!< flag is wrong or not supported
  CC_StsBadPoint = -207, //!< bad CPoint
  CC_StsBadMask = -208, //!< bad format of mask (neither 8uC1 nor 8sC1)
  CC_StsUnmatchedSizes = -209, //!< sizes of input/output structures do not match
  CC_StsUnsupportedFormat = -210, //!< the data format/type is not supported by the function
  CC_StsOutOfRange = -211, //!< some of parameters are out of range
  CC_StsParseError = -212, //!< invalid syntax/structure of the parsed file
  CC_StsNotImplemented = -213, //!< the requested function/feature is not implemented
  CC_StsBadMemBlock = -214, //!< an allocated block has been corrupted
  CC_StsAssert = -215, //!< assertion failed
  CC_GpuNotSupported = -216,
  CC_GpuApiCallError = -217,
  CC_OpenGlNotSupported = -218,
  CC_OpenGlApiCallError = -219,
  CC_OpenCLApiCallError = -220,
  CC_OpenCLDoubleNotSupported = -221,
  CC_OpenCLInitError = -222,
  CC_OpenCLNoAMDBlasFft = -223
} CStatus;

/////////////////////////////////////////////////////////////////////////////////////////////////
typedef signed char schar;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef char int8;
typedef unsigned char uint8;
typedef short int16;
typedef unsigned short uint16;
typedef int int32;
typedef unsigned int uint32;
#ifdef _WIN32
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
#include <inttypes.h>
typedef int64_t int64;
typedef uint64_t uint64;
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////
// typedef int vec4i[4];
//

#define VECN_DEF(name, n, type) struct Vec ## n ## name { type v[n]; };
#define VECN_DEF_DEF(n) VECN_DEF(b, n, uchar)VECN_DEF(s, n, short)VECN_DEF(i, n, int)VECN_DEF(f, n, float)
VECN_DEF_DEF(2);
VECN_DEF_DEF(3);
VECN_DEF_DEF(4);
VECN_DEF_DEF(6);
VECN_DEF_DEF(8);
VECN_DEF_DEF(16);
#undef VECN_DEF_DEF
#undef VECN_DEF
/////////////////////////////////////////////////////////////////////////////////////////////////
typedef uint32 COLOR;
typedef uint64 COLOR64;

typedef union suf32_t {
  int32 i;
  uint32 u;
  float f;
  uchar c[4];
  struct _fp32Format
  {
    unsigned int significand : 23;
    unsigned int exponent : 8;
    unsigned int sign : 1;
  } fmt;
}
suf32_t;
typedef union suf64_t {
  int64 i;
  uint64 u;
  double f;
  uchar c[8];
}
suf64_t;


#if 1
#define TYPEDEF_DEF(TYPEDEF) \
  TYPEDEF(CC_8U, u8,1,uchar  ) \
  TYPEDEF(CC_8S, s8,1,char   ) \
  TYPEDEF(CC_16U,u16,2,ushort ) \
  TYPEDEF(CC_16S,s16,2,short  ) \
  TYPEDEF(CC_32S,s32,4,int    ) \
  TYPEDEF(CC_32F,f32,4,float  ) \
  TYPEDEF(CC_64F,f64,8,double ) \
  TYPEDEF(CC_32U,u32,4,uint   ) \
  TYPEDEF(CC_64U,u64,8,uint64 ) \
  TYPEDEF(CC_64S,s64,8,int64  )
#endif
#if 0
#define TYPEDEF_DEF(TYPEDEF) \
  TYPEDEF(CC_8U, u8,1,uchar  ) \
  TYPEDEF(CC_8S, s8,1,char   ) \
  TYPEDEF(CC_16U,u16,2,ushort ) \
  TYPEDEF(CC_16S,s16,2,short  ) \
  TYPEDEF(CC_32S,s32,4,int    ) \
  TYPEDEF(CC_32F,f32,4,float  ) \
  TYPEDEF(CC_64F,f64,8,double ) \
  TYPEDEF(CC_32U,u32,4,uint   ) \
  TYPEDEF(CC_64U,u64,8,uint64 ) \
  TYPEDEF(CC_64S,s64,8,int64  )
#endif
//typedef void* CFuncTableCn[32][5];
//typedef void* CFuncTable[32];

#define TYPESIZE(type)   ((type)>>16)


// Matrix type
typedef enum {
  CC_NUL = -1,
#define TYPEDEF(a,b,c,d)  a,
  TYPEDEF_DEF(TYPEDEF)
#undef TYPEDEF
  cc_uchar = CC_8U,
  cc_schar = CC_8S,
  cc_char = CC_8S,
  cc_ushort = CC_8U,
  cc_short = CC_8S,
  cc_uint = CC_32U,
  cc_int = CC_32S,
  cc_float = CC_32F,
  cc_double = CC_64F,
} TypeId;

#define CC_CN_MAX     64
#define CC_CN_SHIFT   3
#define CC_CN_MASK    ((1<<CC_CN_SHIFT)-1)
#define _MAKETYPECN(depth,cn)   ((depth) + (((cn)-1) << CC_CN_SHIFT))
#define CC_TYPECN_CN(typecn)    (( (typecn)>>CC_CN_SHIFT ) + 1 )
#define CC_TYPECN_TYPE(typecn)  (TypeId)( ((typecn) & CC_CN_MASK) )
#define CC_TYPECN_DEPTH   CC_TYPECN_TYPE
typedef enum {
#define TDD(a,b,c)  a##C##c=_MAKETYPECN(a, c)
#define TYPEDEF(a,b,c,d)  TDD(a,b,1),TDD(a,b,2),TDD(a,b,3),TDD(a,b,4),
  TYPEDEF_DEF(TYPEDEF)
#undef TYPEDEF
} TypeCnId;


CC_INLINE int cvTypeSize(TypeId type) {
  static int _type_size_tab[16] = { 0 };
  if (0 == _type_size_tab[CC_8U]) {
#define TYPEDEF(a,b,c,d)  _type_size_tab[a]=c;
    TYPEDEF_DEF(TYPEDEF);
#undef TYPEDEF
  }
  return _type_size_tab[type];
}

CC_INLINE char* cvTypeName(TypeId type) {
  static char* _type_name_tab[16] = { 0 };
  if (NULL == _type_name_tab[CC_8U]) {
#define TYPEDEF(a,b,c,d)  _type_name_tab[a]= (char*)#a ;
    TYPEDEF_DEF(TYPEDEF);
#undef TYPEDEF
  }
  return _type_name_tab[type];
}

#define CC_TYPE_SIZE(type)      cvTypeSize(type)
#define CC_TYPE_NAME(type)      cvTypeName(type)


/////////////////////////////////////////////////////////

#define TYPECVTDEF_DEF(TYPECVTDEF) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_8S,s1,1,char) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_16S,s2,2,short) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_32S,s4,4,int) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_32F,f4,4,float) \
TYPECVTDEF(CC_8U,u1,1,uchar,CC_64F,f8,8,double) \
TYPECVTDEF(CC_8S,s1,1,char,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_8S,s1,1,char,CC_8S,s1,1,char) \
TYPECVTDEF(CC_8S,s1,1,char,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_8S,s1,1,char,CC_16S,s2,2,short) \
TYPECVTDEF(CC_8S,s1,1,char,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_8S,s1,1,char,CC_32S,s4,4,int) \
TYPECVTDEF(CC_8S,s1,1,char,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_8S,s1,1,char,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_8S,s1,1,char,CC_32F,f4,4,float) \
TYPECVTDEF(CC_8S,s1,1,char,CC_64F,f8,8,double) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_8S,s1,1,char) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_16S,s2,2,short) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_32S,s4,4,int) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_32F,f4,4,float) \
TYPECVTDEF(CC_16U,u2,2,ushort,CC_64F,f8,8,double) \
TYPECVTDEF(CC_16S,s2,2,short,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_16S,s2,2,short,CC_8S,s1,1,char) \
TYPECVTDEF(CC_16S,s2,2,short,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_16S,s2,2,short,CC_16S,s2,2,short) \
TYPECVTDEF(CC_16S,s2,2,short,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_16S,s2,2,short,CC_32S,s4,4,int) \
TYPECVTDEF(CC_16S,s2,2,short,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_16S,s2,2,short,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_16S,s2,2,short,CC_32F,f4,4,float) \
TYPECVTDEF(CC_16S,s2,2,short,CC_64F,f8,8,double) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_8S,s1,1,char) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_16S,s2,2,short) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_32S,s4,4,int) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_32F,f4,4,float) \
TYPECVTDEF(CC_32U,u4,4,uint,CC_64F,f8,8,double) \
TYPECVTDEF(CC_32S,s4,4,int,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_32S,s4,4,int,CC_8S,s1,1,char) \
TYPECVTDEF(CC_32S,s4,4,int,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_32S,s4,4,int,CC_16S,s2,2,short) \
TYPECVTDEF(CC_32S,s4,4,int,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_32S,s4,4,int,CC_32S,s4,4,int) \
TYPECVTDEF(CC_32S,s4,4,int,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_32S,s4,4,int,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_32S,s4,4,int,CC_32F,f4,4,float) \
TYPECVTDEF(CC_32S,s4,4,int,CC_64F,f8,8,double) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_8S,s1,1,char) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_16S,s2,2,short) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_32S,s4,4,int) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_32F,f4,4,float) \
TYPECVTDEF(CC_64U,u8,8,uint64,CC_64F,f8,8,double) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_8S,s1,1,char) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_16S,s2,2,short) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_32S,s4,4,int) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_32F,f4,4,float) \
TYPECVTDEF(CC_64S,s8,8,int64,CC_64F,f8,8,double) \
TYPECVTDEF(CC_32F,f4,4,float,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_32F,f4,4,float,CC_8S,s1,1,char) \
TYPECVTDEF(CC_32F,f4,4,float,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_32F,f4,4,float,CC_16S,s2,2,short) \
TYPECVTDEF(CC_32F,f4,4,float,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_32F,f4,4,float,CC_32S,s4,4,int) \
TYPECVTDEF(CC_32F,f4,4,float,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_32F,f4,4,float,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_32F,f4,4,float,CC_32F,f4,4,float) \
TYPECVTDEF(CC_32F,f4,4,float,CC_64F,f8,8,double) \
TYPECVTDEF(CC_64F,f8,8,double,CC_8U,u1,1,uchar) \
TYPECVTDEF(CC_64F,f8,8,double,CC_8S,s1,1,char) \
TYPECVTDEF(CC_64F,f8,8,double,CC_16U,u2,2,ushort) \
TYPECVTDEF(CC_64F,f8,8,double,CC_16S,s2,2,short) \
TYPECVTDEF(CC_64F,f8,8,double,CC_32U,u4,4,uint) \
TYPECVTDEF(CC_64F,f8,8,double,CC_32S,s4,4,int) \
TYPECVTDEF(CC_64F,f8,8,double,CC_64U,u8,8,uint64) \
TYPECVTDEF(CC_64F,f8,8,double,CC_64S,s8,8,int64) \
TYPECVTDEF(CC_64F,f8,8,double,CC_32F,f4,4,float) \
TYPECVTDEF(CC_64F,f8,8,double,CC_64F,f8,8,double)

static void* arrcvt(void* dst, TypeId dst_type, const void* src, TypeId src_type, int n) {
  int i = 0;
  if (NULL == dst) {}
  switch ((dst_type << 8) | src_type) {
#define TYPECVTDEF(a, b, c, d, e, f, g, h)  case ((a<<8) | e): { d* _d = (d*)dst; const h* _s = (const h*) src; \
    for (; i<n-3; i+=4) {_d[i] = (d)_s[i];_d[i+1] = (d)_s[i+1];_d[i+2] = (d)_s[i+2];_d[i+3] = (d)_s[i+3]; } for (; i<n; ++i) {_d[i] = (d)_s[i]; } } break;
    TYPECVTDEF_DEF(TYPECVTDEF)
#undef TYPECVTDEF
  }
  return dst;
}
static void* arrcvt2(void* dst, TypeId dst_type, const void* src, TypeId src_type, int n, double alpha, double beta) {
  bool noScale = fabs(alpha - 1) < DBL_EPSILON && fabs(beta) < DBL_EPSILON;
  if (noScale) {
    return arrcvt(dst, dst_type, src, src_type, n);
  }
  else {
    int i = 0;
    if (NULL == dst) {}
    switch ((dst_type << 8) | src_type) {
#define OP(a)  (alpha*(a) + beta)
#define TYPECVTDEF(a, b, c, d, e, f, g, h)  case ((a<<8) | e): { d* _d = (d*)dst; const h* _s = (const h*) src; \
    for (; i<n-3; i+=4) {_d[i] = (d)OP(_s[i]);_d[i+1] = (d)OP(_s[i+1]);_d[i+2] = (d)OP(_s[i+2]);_d[i+3] = (d)OP(_s[i+3]); } for (; i<n; ++i) {_d[i] = (d)OP(_s[i]); } } break;
      TYPECVTDEF_DEF(TYPECVTDEF)
#undef TYPECVTDEF
#undef OP
    }
    return dst;
  }
}
static void* arrcvt2d(void* dst, TypeId dsttype, int dl, const void* src, TypeId srctype, int sl, int h, int w, double alpha, double beta) {
  char* d = (char*)(dst);
  const char* s = (const char*)(src);
  int dw = w*(dsttype >> 16);
  int sw = w*(srctype >> 16);
  dl = dl>0 ? dl : dw;
  sl = sl>0 ? sl : sw;
  if (dl == dw && sl == sw) {
    return arrcvt2(dst, dsttype, src, srctype, h * w, alpha, beta);
  }
  for (; h--; s += sl, d += dl) {
    arrcvt2(d, dsttype, s, srctype, w, alpha, beta);
  }
  return dst;
}


/////////////////////////////////////////////////////////

typedef struct DPOINT3 {
  double x, y, z;
} DPOINT3;
typedef DPOINT3 D3VECTOR;
typedef struct FPOINT3 {
  float x, y, z;
} FPOINT3;
typedef FPOINT3 F3VECTOR;
typedef struct DPOINT4 {
  double x, y, z, w;
} DPOINT4;
typedef DPOINT4 D4VECTOR;
typedef struct FPOINT4 {
  float x, y, z, w;
} FPOINT4, FPoint4;
typedef FPOINT4 F4VECTOR;
typedef struct {
  short x, y;
} SPOINT, SPoint;
typedef struct {
  int x, y;
} IPOINT, IPOINT2, IPoint, IPoint2;
typedef struct IPOINT3 {
  int x, y, z;
} IPOINT3, IPoint3;
typedef struct {
  int64 x, y;
} QPOINT, QPoint;
typedef QPOINT LPOINT2;
typedef struct {
  int64 x, y, z;
} QPOINT3;
typedef QPOINT3 LPOINT3;
typedef struct {
  double x, y;
} DPOINT, DPoint;
typedef DPOINT D2VECTOR;
#ifndef __FPOINT_DEFINED__
#define __FPOINT_DEFINED__
typedef struct {
  float x, y;
} FPOINT, FPoint, FPOINT2, FPoint2;
#endif // __FPOINT_DEFINED__
typedef FPOINT F2VECTOR;
typedef struct ISize {
  union { int cx; int w; int width; int c; };
  union { int cy; int h; int height; int r; };
} ISIZE;
typedef struct {
  union { int64 w; int64 width; };
  union { int64 h; int64 height; };
} LSIZE;
typedef struct {
  union { float w; float width; };
  union { float h; float height; };
} FSIZE;
typedef struct {
  union { double w; float width; };
  union { double h; float height; };
} DSIZE;
typedef struct {
  int l, t, r, b;
} IRECT;
typedef struct {
  float l, t, r, b;
} FRECT;
typedef struct {
  double l, t, r, b;
} DRECT;
typedef struct {
  int s, e;
} IRANGE;
typedef struct {
  double s, e;
} DRANGE;
typedef struct {
  double a00, a01, a02, a10, a11, a12, a20, a21, a22;
} DMATRIX33;
typedef struct {
  IPOINT p0, p1;
} ISEGMENT;
typedef struct {
  LPOINT2 p0, p1;
} LSEGMENT;
typedef struct {
  DPOINT p0, p1;
} DSEGMENT;
typedef struct {
  DPOINT3 p0, p1, p2;
} DSEGMENT3;
typedef DPOINT DPOINT2;
//typedef DPOINT3 DPOINT3;
typedef DSEGMENT DSEGMENT2;
typedef struct {
  int x, y, cmd;
} vertex_i;
typedef struct {
  int x1, x2;
  uint32* ptr;
} row_info;

typedef struct CRange {
  int start;
  int end;
} CRange;
#define Data_2DSize(d) ((d).w * (d).h)
#define Data_3DSize(d) ((d).w * (d).h * (d).c)
#define Data_4DSize(d) ((d).w * (d).h * (d).c * (d).n)
#define Data_total(d)  Data_4DSize(d)
#define Data_count(d)  Data_4DSize(d)
#define Data_4DEqu(a, b) ((a).w == (b).w && (a).h == (b).h && (a).c == (b).c && (a).n == (b).n)
#define Data_4DNeq(a, b) (!Data_4DEqu(a, b))
#define Data_2DIndex(d, ih, iw) ((ih) * (d).w + (iw))
#define Data_3DIndex(d, ic, ih, iw) (((ic) * (d).h + ih) * (d).w + (iw))
#define Data_4DIndex(d, in, ic, ih, iw)   ((((in) * (d).c + ic) * (d).h + ih) * (d).w + (iw))
#define Data_4DIndex1(d, in)           Data_4DIndex(d, in, 0, 0, 0)
#define Data_4DIndex2(d, in, ic)       Data_4DIndex(d, in, ic, 0, 0)
#define Data_4DIndex3(d, in, ic, ih)   Data_4DIndex(d, in, ic, ih, 0)

typedef union DataSize {
  struct { int n, c, h, w; };
  int dim[4];
} DataSize;
static DataSize dataSize(int _number, int _channels, int _height, int _width)
{
  DataSize ret = { _number, _channels, _height, _width };
  ret.n = _number;
  ret.c = _channels;
  ret.h = _height;
  ret.w = _width;
  return ret;
}
static int dataCount(DataSize size, int start, int end) {
  int count = 1;
  start = BOUND(start, 0, end);
  end = BOUND(end, start, 4);
  for (; start < end; ++start) {
    count *= size.dim[start];
  }
  return count;
}
typedef FPOINT2 CPoint2D32f;
typedef FPOINT3 CPoint3D32f;
typedef DPOINT2 CPoint2D64f;
typedef DPOINT3 CPoint3D64f;

typedef IPOINT2 CPoint2i;
typedef IPOINT3 CPoint3i;
typedef LPOINT2 CPoint2l;
typedef LPOINT3 CPoint3l;
typedef FPOINT2 CPoint2f;
typedef FPOINT3 CPoint3f;
typedef DPOINT2 CPoint2d;
typedef DPOINT3 CPoint3d;

typedef LSIZE CSize2l;
typedef ISIZE CSize;
typedef IPOINT CPoint;
typedef FPOINT CPoint2D32f;
#define CC_SIZE_AREA(sz)   ((sz).width * (sz).height)
#define CC_SIZE_ISEQ(sz1, sz2)   ( ((sz1).width==(sz1).width) && ((sz1).height==(sz2).height) )

#define SETPT2(pt, _x, _y)  ((pt).x=_x, (pt).y=_y)
#define SETPT3(pt, _x, _y, _z)  ((pt).x=_x, (pt).y=_y, (pt).z=_z)


#define PTSET(pt, _x, _y)   ((pt).x=_x, (pt).y=_y)
CC_INLINE IRANGE iRANGE(int s, int e)
{
  IRANGE ra;
  ra.s = s, ra.e = e;
  return ra;
}
CC_INLINE void iRANGE_clame(IRANGE* r, int s, int e)
{
  r->s = BOUND(r->s, s, e);
  if (r->e < s) {
    r->e = e;
  }
  else {
    r->e = BOUND(r->e, s, e);
  }
}
CC_INLINE DRANGE dRANGE(double s, double e)
{
  DRANGE ra;
  ra.s = s, ra.e = e;
  return ra;
}
#define iPOINT_PT(pt)  iPOINT((int)(pt).x, (int)(pt).y)
CC_INLINE IPOINT iPOINT(int x, int y)
{
  IPOINT pt;
  pt.x = x, pt.y = y;
  return pt;
}
CC_INLINE IPoint iPoint(int x, int y)
{
  IPOINT pt;
  pt.x = x, pt.y = y;
  return pt;
}
CC_INLINE int iPOINT_cmp(IPOINT a, IPOINT b)
{
  return a.y == b.y ? CC_CMP(a.x, b.x) : CC_CMP(a.y, b.y);
}
CC_INLINE FPOINT fPOINT(double x, double y)
{
  FPOINT p;
  p.x = (float)x;
  p.y = (float)y;
  return p;
}
CC_INLINE DPOINT dPOINT(double x, double y)
{
  DPOINT pt;
  pt.x = x, pt.y = y;
  return pt;
}
CC_INLINE IRANGE cRANGE(int s, int e)
{
  IRANGE ra;
  ra.s = s, ra.e = e;
  return ra;
}
CC_INLINE FPOINT iPointTof(IPOINT point)
{
  return fPOINT((float)point.x, (float)point.y);
}
CC_INLINE IPOINT fPointToi(FPOINT point)
{
  IPOINT ipt;
  ipt.x = (int)(point.x + .5);
  ipt.y = (int)(point.y + .5);
  return ipt;
}
CC_INLINE FPOINT3 fPOINT3(double x, double y, double z)
{
  FPOINT3 p;
  p.x = (float)x, p.y = (float)y, p.z = (float)z;
  return p;
}
#define cPoint3d dPOINT3
CC_INLINE DPOINT3 dPOINT3(double x, double y, double z)
{
  DPOINT3 p;
  p.x = x, p.y = y, p.z = z;
  return p;
}
CC_INLINE DSEGMENT dSEGMENT(double x0, double y0, double x1, double y1)
{
  DSEGMENT d;
  d.p0 = dPOINT(x0, y0), d.p1 = dPOINT(x1, y1);
  return d;
}
CC_INLINE ISEGMENT iSEGMENT(int x0, int y0, int x1, int y1)
{
  ISEGMENT d;
  d.p0 = iPOINT(x0, y0), d.p1 = iPOINT(x1, y1);
  return d;
}


/////////////////////////////////////////////////////////

CC_INLINE void memunroll(void* p, int n, int unroll_to) {
  char* ptr = (char*)p;
  while (n < unroll_to) {
    memcpy(ptr + n, p, MIN(n, unroll_to - n));
    n *= 2;
  }
}
CC_INLINE void* matcpy(void* dst, int dl, const void* src, int sl, int h, int w)
{
  char* d = (char*)(dst);
  const char* s = (const char*)(src);
  if (dl == sl && dl == w) {
    return memcpy(dst, src, w * h);
  }
  for (; h--; s += sl, d += dl) {
    memcpy(d, s, w);
  }
  return dst;
}
CC_INLINE void* matmove(void* dst, int dl, const void* src, int sl, int h, int w)
{
  char* d = (char*)(dst);
  const char* s = (const char*)(src);
  if (dl == sl && dl == w) {
    return memmove(dst, src, w * h);
  }
  for (; h--; s += sl, d += dl) {
    memmove(d, s, w);
  }
  return dst;
}

CC_INLINE int memcpy2da(void* B, int bl, const void* A, int al, int h, int w)
{
  int i;
  for (i = 0; i < h; ++i) {
    memcpy(((uchar*)B + i * bl), ((const uchar*)A + i * al), w);
  }
  return 0;
}
CC_INLINE int copy2d(int h, int w, const void* A, int al, void* B, int bl)
{
  int i;
  for (i = 0; i < h; ++i) {
    memcpy(((uchar*)B + i * bl), ((const uchar*)A + i * al), w);
  }
  return 0;
}
CC_INLINE void* memcpy2d(void* dst, int dl, const void* src, int sl, int h, int w)
{
  char* d = (char*)(dst);
  const char* s = (const char*)(src);
  if (dl == sl && dl == w) {
    return memcpy(dst, src, h * w);
  }
  for (; h--; s += sl, d += dl) {
    memcpy(d, s, w);
  }
  return dst;
}
CC_INLINE int mem_swap(int n, void* src, void* dst)
{
  int i, t0, t1, t2, t3;
  int* s = (int*)src;
  int* d = (int*)dst;
  char* cs = (char*)src;
  char* cd = (char*)dst;
  char ct;
  for (i = 0; i < n - 15; i += 16, s += 4, d += 4) {
    CC_SWAP(s[0], d[0], t0);
    CC_SWAP(s[1], d[1], t1);
    CC_SWAP(s[2], d[2], t2);
    CC_SWAP(s[3], d[3], t3);
  }
  for (; i < n; ++i) {
    CC_SWAP(cs[i], cd[i], ct);
  }
  return 0;
}
// 上下颠倒
CC_INLINE int matflip(int h, int w, void* src, int srcstep)
{
  int i;
  char* cs = (char*)src;
  char* cd = (char*)src + (h - 1) * srcstep;
  for (i = 0; i < h / 2; ++i, cs += srcstep, cd -= srcstep) {
    mem_swap(w, cs, cd);
  }
  return 0;
}
CC_INLINE void* memflip(void* dst, int dl, const void* src, int sl, int h, int w)
{
  char* d = (char*)(dst);
  const char* s = (const char*)(src);
  if (d == s && dl == sl) {
    matflip(h, w, dst, dl);
  }
  else {
    d += (h - 1) * dl;
    for (; h--; s += sl, d -= dl) {
      memcpy(d, s, w);
    }
  }
  return 0;
}
CC_INLINE void memset2(void* adr, int val, int count)
{
  int i, n, v;
  unsigned int* p;
  unsigned short* q;
  p = (unsigned int*)adr;
  v = val | (val << 16);
  n = count >> 3;
  for (i = 0; i < n; i++) {
    p[0] = v;
    p[1] = v;
    p[2] = v;
    p[3] = v;
    p += 4;
  }
  q = (unsigned short*)p;
  n = count & 7;
  for (i = 0; i < n; i++) {
    *q++ = val;
  }
}
CC_INLINE void memset4(void* adr, int val, int count)
{
  int i, n, v;
  unsigned int* p;
  p = (unsigned int*)adr;
  v = val;
  n = count >> 2;
  for (i = 0; i < n; i++) {
    p[0] = v;
    p[1] = v;
    p[2] = v;
    p[3] = v;
    p += 4;
  }
  n = count & 3;
  for (i = 0; i < n; i++) {
    *p++ = val;
  }
}
/* count must be a multiple of 4 and >= 4 */
CC_INLINE void memset3(void* adr, int r, int v, int b, long count)
{
  long i, n;
  register long v1, v2, v3, *pt = (long*)(adr);
  unsigned char* p, R = (unsigned char)r, V = (unsigned char)v, B = (unsigned char)b;
  p = (unsigned char*)adr;
  *p++ = R;
  *p++ = V;
  *p++ = B;
  *p++ = R;
  *p++ = V;
  *p++ = B;
  *p++ = R;
  *p++ = V;
  *p++ = B;
  *p++ = R;
  *p++ = V;
  *p++ = B;
  v1 = *pt++;
  v2 = *pt++;
  v3 = *pt++;
  n = count >> 2;
  for (i = 1; i < n; i++) {
    pt[0] = v1;
    pt[1] = v2;
    pt[2] = v3;
    pt += 3;
  }
}

#define MEMSET(_F, _V, _N)     memset(_F, _V, sizeof(*(_F))*(_N))
#define MEMZERO(_F, _N)        memset(_F, 0, sizeof(*(_F))*(_N))
#define MEMCPY(_P, _F, _N)     memcpy(_P, _F, sizeof(*(_F))*(_N))
#ifndef MEMMOVE
#define MEMMOVE(_P, _F, _N)    memmove(_P, _F, sizeof(*(_F))*(_N))
#endif
/////////////////////////////////////////////////////////

#define PT2OPDEF(p1, op, p2)  (p1).x op (p2).x, (p1).y op (p2).y
#define PT3OPDEF(p1, op, p2)   (p1).x op (p2).x, (p1).y op (p2).y, (p1).z op (p2).z
#define RAND01() (rand()*1./RAND_MAX)
#define RANDX(_X) (rand()*(_X*1.)/RAND_MAX)
#define iround(v) (int)((v < 0.0) ? v - 0.5 : v + 0.5)
#define uround(v) (unsigned)(v + 0.5)
#define ufloor(v) (unsigned)(v)
#define uceil(v) (unsigned)(ceil(v))
#define PREV_INDEX(i, n) (i-1+n)%(n)
#define NEXT_INDEX(i, n) (i+1 )%(n)
#define PTADD(c, a, b) (c.x=a.x+b.x, c.y=a.y+b.y)
#define PTSUB(c, a, b) (c.x=a.x-b.x, c.y=a.y-b.y)
#define PTCLIP(pt, l, t, r, b) iPOINT(BOUND(pt.x, l, r), BOUND(pt.y, t, b))
#define RCLT(rc) iPOINT((rc)->l, (rc)->t)
#define RCLB(rc) iPOINT((rc)->l, (rc)->b)
#define RCRB(rc) iPOINT((rc)->r, (rc)->b)
#define RCSZ(rc) iSIZE(RCW(rc), RCH(rc))
#define SZMAX(sz1, sz2) iSIZE(MAX(sz1.w, sz2.w), MAX(sz1.h, sz2.h))
#define SZADD(sz1, sz2) iSIZE(sz1.w+sz2.w, sz1.h+sz2.h)
#define SZADD2(sz1, CX, CY) iSIZE(sz1.w+CX, sz1.h+CY)
#define RCSET(prc, L, T, R, B) ((prc)->l=L, (prc)->t=T, (prc)->r=R, (prc)->b=B)
#define RCX(prc) ((prc)->r + (prc)->l)
#define RCY(prc) ((prc)->b + (prc)->t)
#define RCW(prc) ((prc)->r - (prc)->l)
#define RCH(prc) ((prc)->b - (prc)->t)
#define RCCENTER(rc)    iPOINT(RCX(rc)/2, RCY(rc)/2)
#define RCCENTERF(rc)   dPOINT(RCX(rc)/2., RCY(rc)/2.)
#define RCEQ(R1, R2)   (((R1)->l==(R2)->l) && ((R1)->t==(R2)->t) && ((R1)->r==(R2)->r) && ((R1)->b==(R2)->b))
CC_INLINE ISIZE iSIZE(int w, int h)
{
  ISIZE sz;
  sz.w = w, sz.h = h;
  return sz;
}
CC_INLINE ISize iSize(int w, int h)
{
  ISize sz;
  sz.w = w, sz.h = h;
  return sz;
}
CC_INLINE FSIZE fSIZE(float w, float h) {
  FSIZE s;
  s.w = w;
  s.h = h;
  return s;
}
CC_INLINE IRECT iRECT(int l, int t, int r, int b)
{
  IRECT rc;
  RCSET(&rc, l, t, r, b);
  return rc;
}
CC_INLINE IRECT iRECT1(int x)
{
  IRECT rc;
  RCSET(&rc, x, x, x, x);
  return rc;
}
CC_INLINE IRECT iRECT_sz(ISIZE sz)
{
  IRECT rc;
  RCSET(&rc, 0, 0, sz.w, sz.h);
  return rc;
}
CC_INLINE IRECT iRECT_ps(IPOINT pt, ISIZE sz)
{
  IRECT rc;
  RCSET(&rc, pt.x, pt.y, pt.x + sz.w, pt.y + sz.h);
  return rc;
}
CC_INLINE IRECT iRECT_pp(IPOINT p1, IPOINT p2)
{
  IRECT rc;
  RCSET(&rc, p1.x, p1.y, p2.x, p2.y);
  return rc;
}
#define iRECT3(pt, sz) iRECT2(pt.x, pt.y, sz.w, sz.h)
CC_INLINE IRECT iRECT2(int x, int y, int w, int h)
{
  IRECT rc;
  RCSET(&rc, x, y, x + w, y + h);
  return rc;
}
CC_INLINE FRECT fRECT(float l, float t, float r, float b)
{
  FRECT rc;
  RCSET(&rc, l, t, r, b);
  return rc;
}
CC_INLINE FRECT fRECT2(float l, float t, float w, float h)
{
  FRECT rc;
  RCSET(&rc, l, t, l + w, t + h);
  return rc;
}

#define RECT2RECT(fun, T, rc) fun((T)((rc)->l), (T)((rc)->t), (T)((rc)->r), (T)((rc)->b));

CC_INLINE DRECT dRECT(double l, double t, double r, double b)
{
  DRECT rc;
  RCSET(&rc, l, t, r, b);
  return rc;
}
CC_INLINE DRECT dRECT2(double x, double y, double w, double h)
{
  DRECT rc;
  RCSET(&rc, x, y, x + w, y + h);
  return rc;
}

CC_INLINE int iRectIsEmpty(const IRECT* rc)
{
  return rc->l >= rc->r || rc->t >= rc->b;
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
CC_INLINE int fRectNormalize(FRECT* prc)
{
  float nTemp;
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
CC_INLINE int iRectInclude(IRECT r1, IRECT r2)
{
  iRectNormalize(&r1);
  iRectNormalize(&r2);
  return r1.l <= r2.l && r1.t <= r2.l && r1.r >= r2.r && r1.b >= r2.b;
}
CC_INLINE int iRectBoundUpdate(IRECT* prc, int x, int y) {
  if (x<prc->l) {
    prc->l = x;
  }
  else if (x>prc->r) {
    prc->r = x;
  }
  if (y<prc->t) {
    prc->t = y;
  }
  else if (y>prc->b) {
    prc->b = y;
  }
  return 0;
}
CC_INLINE IRECT iRectInter(IRECT r1, IRECT r2)
{
  IRECT temp = { 0,0,0,0 };
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
CC_INLINE FRECT fRectInter(FRECT r1, FRECT r2)
{
  FRECT temp = { 0,0,0,0 };
  fRectNormalize(&r1);
  fRectNormalize(&r2);
  // check for total exclusion
  if ((r1.r > r2.l) && (r1.l < r2.r) && (r1.b > r2.t) && (r1.t < r2.b)) {
    temp.l = (r1.l > r2.l) ? r1.l : r2.l;
    temp.r = (r1.r < r2.r) ? r1.r : r2.r;
    temp.t = (r1.t > r2.t) ? r1.t : r2.t;
    temp.b = (r1.b < r2.b) ? r1.b : r2.b;
  }
  return temp;
}
CC_INLINE BOOL iRectIsNull(const IRECT* pRect)
{
  return (pRect->l >= pRect->r || pRect->t >= pRect->b);
}
CC_INLINE BOOL iRectSet(IRECT* pRect, int l, int t, int r, int b)
{
  pRect->l = l, pRect->r = r, pRect->t = t, pRect->b = b;
  return 0;
}
CC_INLINE BOOL iRectSetEmpty(IRECT* pRect)
{
  pRect->l = pRect->r = pRect->t = pRect->b = 0;
  return 0;
}

#define iRectIn(rc, d)  iRectOffset(rc, d, d, -(d), -(d))
#define iRectOff2(rc, x, y)  iRectOffset((rc), x, y, x, y)
#define iRectOff0(rc)  iRectOff2((rc), -(rc)->l, -(rc)->t)
CC_INLINE IRECT iRectOffset(IRECT rc, int l, int t, int r, int b)
{
  rc.l += l, rc.t += t;
  rc.r += r, rc.b += b;
  return rc;
}
CC_INLINE IRECT iRectOffset2(IRECT rc, int x, int y)
{
  rc.l += x, rc.t += y;
  rc.r += x, rc.b += y;
  return rc;
}
CC_INLINE IRECT iRectOffsetTo2(IRECT rc, int x, int y) {
  rc.l += x, rc.t += y;
  rc.r += x, rc.b += y;
  return rc;
}
CC_INLINE IRECT iRectDeflate(IRECT rc, int l, int t, int r, int b)
{
  rc.l += l, rc.t += t;
  rc.r -= r, rc.b -= b;
  return rc;
}
CC_INLINE IRECT iRectDeflate2(IRECT rc, int x, int y)
{
  rc.l += x, rc.t += y;
  rc.r -= x, rc.b -= y;
  return rc;
}
CC_INLINE IRECT iRectInflate(IRECT rc, int l, int t, int r, int b)
{
  rc.l -= l, rc.t -= t;
  rc.r += r, rc.b += b;
  return rc;
}
CC_INLINE IRECT iRectOffsetXY(IRECT rc, int x, int y)
{
  rc.l = rc.l + x, rc.t = rc.t + y;
  rc.r = rc.r + x, rc.b = rc.b + y;
  return rc;
}
CC_INLINE IRECT iRectAdd(IRECT rc, const IRECT rc2)
{
  rc.l += rc2.l, rc.t += rc2.t;
  rc.r += rc2.r, rc.b += rc2.b;
  return rc;
}
CC_INLINE IRECT iRectDeflateR(IRECT rc, const IRECT rc2)
{
  rc.l += rc2.l, rc.t += rc2.t;
  rc.r -= rc2.r, rc.b -= rc2.b;
  return rc;
}
CC_INLINE IRECT iRectInflateR(IRECT rc, const IRECT rc2)
{
  rc.l -= rc2.l, rc.t -= rc2.t;
  rc.r += rc2.r, rc.b += rc2.b;
  return rc;
}
CC_INLINE IRECT iRectInflate2(IRECT rc, int x, int y)
{
  rc.l -= x, rc.t -= y;
  rc.r += x, rc.b += y;
  return rc;
}
CC_INLINE IRECT iRectInflate1(IRECT rc, int x)
{
  rc.l -= x, rc.t -= x;
  rc.r += x, rc.b += x;
  return rc;
}
#define iPtInRect2(rc, pt)  iPtInRect(rc, (int)(pt).x, (int)(pt).y)
CC_INLINE int iPtInRect(const IRECT* rc, int x, int y)
{
  return (rc->l <= x && x < rc->r) && (rc->t <= y && y < rc->b);
}

CC_INLINE int iPtInRects(int n, const IRECT* rc, int x, int y)
{
  int i;
  for (i = 0; i < n; ++i) {
    if (iPtInRect(rc + i, x, y)) {
      return i;
    }
  }
  return -1;
}
CC_INLINE int iPtInRectsBorder(IRECT rc, BOOL horz, int n, const float* v, int gap, int x, int y, int ex) {
  int i;
  float l = (float)rc.l;
  float t = (float)rc.t;
  if (horz) {
    for (i = 0; i<(n - 1); ++i) {
      float r = l + v[i];
      float l1 = r + gap;
      if (x>(r - ex) && x<(l1 + ex) && y>rc.t && y<rc.b) {
        return i;
      }
      l = l1;
    }
  }
  else {
    for (i = 0; i<(n - 1); ++i) {
      float b = t + v[i];
      float t1 = b + gap;
      if (y>(b - ex) && y<(t1 + ex) && x>rc.l && x<rc.r) {
        return i;
      }
      t = t1;
    }
  }
  return -1;
}

CC_INLINE int iPtInRectBorder(const IRECT* prc, int x, int y, int l, int t, int r, int b)
{
  IRECT rc = iRectOffset(*prc, -l, -t, r, b);

  if (iPtInRect(&rc, x, y) && !iPtInRect(prc, x, y)) {
    return (x < prc->l) << 0 | (x > prc->r) << 1 | (y < prc->t) << 2 | (y > prc->b) << 3;
  }

  return 0;
}
CC_INLINE IRECT iRectToSquare(IRECT rc)
{
  IRECT rc1 = rc;
  int d = MIN(RCH(&rc), RCW(&rc));
  rc1.r = rc.l + d, rc1.b = rc.t + d;
  return rc1;
}

// 自定义函数，计算两矩形 IOU，传入为均为矩形对角线，（x,y）  坐标。·
CC_INLINE double iRectIOU(IRECT Reframe, IRECT GTframe) {
  double ratio;
  int x1 = Reframe.l;
  int y1 = Reframe.t;
  int width1 = Reframe.r - Reframe.l;
  int height1 = Reframe.b - Reframe.t;
  int x2 = GTframe.l;
  int y2 = GTframe.t;
  int width2 = GTframe.r - GTframe.l;
  int height2 = GTframe.b - GTframe.t;

  int endx = MAX(x1 + width1, x2 + width2);
  int startx = MIN(x1, x2);
  int width = width1 + width2 - (endx - startx);

  int endy = MAX(y1 + height1, y2 + height2);
  int starty = MIN(y1, y2);
  int height = height1 + height2 - (endy - starty);

  if (width <= 0 || height <= 0) {
    ratio = 0; // 重叠率为 0
  }
  else {
    int Area = width*height; // 两矩形相交面积
    int Area1 = width1*height1;
    int Area2 = width2*height2;
    ratio = Area*1. / (Area1 + Area2 - Area);
  }
  // return IOU
  return ratio;
}
/////////////////////////////////////////////////////////

struct IRect {
  int x, y, width, height;
};
IRect iRect(int _x, int _y, int _width, int _height) {
  IRect r;
  r.x = _x;
  r.y = _y;
  r.width = _width;
  r.height = _height;
  return r;
}
IRect iRect(IPoint pt, ISize sz) {
  IRect r;
  r.x = pt.x;
  r.y = pt.y;
  r.width = sz.width;
  r.height = sz.height;
  return r;
}
template <typename IRect, typename IPoint>
bool contains(IRect r, IPoint pt) {
  return r.x <= pt.x && pt.x < r.x + r.width && r.y <= pt.y && pt.y < r.y + r.height;
}
bool contains(IRect r, IPoint pt) {
  return r.x <= pt.x && pt.x < r.x + r.width && r.y <= pt.y && pt.y < r.y + r.height;
}
bool inside(IPoint pt, IRect r) {
  return r.x <= pt.x && pt.x < r.x + r.width && r.y <= pt.y && pt.y < r.y + r.height;
}
/////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////
enum MemType {CPU, GPU};
struct mem_t {
  MemType type_;
  void* (*realloc_)(void* p, size_t n);
  void(*free_)(void* p);
};
static void* cpu_realloc(void* p, size_t n) { return realloc(p, n); }
static void cpu_free(void* p) { free(p); }
static void* copy_cpu2cpu(void* dst, const void* src, size_t n) { return memcpy(dst, src, n); }
typedef void* (*mem_copy_t)(void* dst, const void* src, size_t n);
static mem_copy_t mem_copy[2][2] = { copy_cpu2cpu ,NULL, NULL, NULL};
static mem_t cpu_mem[1] = { CPU, cpu_realloc , cpu_free };
static mem_t cpu_mem_nul[1] = { CPU, NULL , NULL };
static void* mem_realloc(void* p, size_t newn, mem_t* newmem, size_t oldn, mem_t* oldmem) {
  void* newp = NULL;
  newmem = newmem ? newmem : cpu_mem;
  oldmem = oldmem ? oldmem : cpu_mem;
  if (newmem == oldmem) {
    newp = newmem->realloc_(p, newn);
  }
  else {
    newp = newmem->realloc_(NULL, newn);
    mem_copy[newmem->type_][oldmem->type_](newp, p, oldn);
    if (oldmem->free_) { oldmem->free_(p); }
  }
  return newp;
}
/////////////////////////////////////////////////////////

#endif // _STDC_H_
