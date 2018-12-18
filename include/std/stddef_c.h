
#ifndef _STDC_STDDEF_H_
#define _STDC_STDDEF_H_

#include <assert.h>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <winsock.h>
#undef max
#undef min
//#pragma comment(lib, "ws32_2.lib")
#pragma comment(lib,"ws2_32.lib")
#else
#include <linux/types.h>
#endif // _WIN32


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
//
#define MEMCPY(_P, _F, _N)     memcpy(_P, _F, sizeof(*(_F))*(_N))
#ifndef MEMMOVE
#define MEMMOVE(_P, _F, _N)    memmove(_P, _F, sizeof(*(_F))*(_N))
#endif
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
#define SIZEOFARR(_T, _N)           (sizeof(_T)*(_N))

#ifdef _DEBUG
#define BUFUSEBEGIN(_BUF, _BUFSZ)   { unsigned char* __BUFEND__ = (unsigned char*)(_BUF)+(_BUFSZ), * __BUFBEG__ = (unsigned char*)(_BUF)/**/
#define BUFUSEEND()                 ASSERT(__BUFBEG__<=__BUFEND__);/**/ }
#else
#define BUFUSEBEGIN(_BUF, _BUFSZ)   { unsigned char* __BUFEND__ = (unsigned char*)(_BUF)+(_BUFSZ), * __BUFBEG__ = (unsigned char*)(_BUF)/**/
#define BUFUSEEND()                 /*ASSERT(__BUFBEG__<=__BUFEND__);*/ }
#endif


#define GETBUFBEG()                 (__BUFBEG__)
#define GETBUFLEN()                 (__BUFEND__-__BUFBEG__)
CC_INLINE void ptr2ptr(void* pp, void* p) {
  *(void**)pp = p;
}
#define BUFMALLOC2(_P, _N)          ptr2ptr(&_P, (void*)(__BUFBEG__+=SIZEOFARR(*_P, _N), __BUFBEG__-SIZEOFARR(*_P, _N)))
#define BUFFREE2(_P, _N)            (__BUFBEG__-=SIZEOFARR(*_P, _N), _P = NULL)
#define BUFMALLOC(_T, _N)           (_T*)(__BUFBEG__+=SIZEOFARR(_T, _N), __BUFBEG__-SIZEOFARR(_T, _N))
#define BUFFREE(_T, _N)             (__BUFBEG__-=SIZEOFARR(_T, _N))
#define BUFBOUNDCHECK()             ASSERT(__BUFBEG__<=__BUFEND__)


/////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
#ifdef _WIN32
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <winsock.h>
//#pragma comment(lib, "ws32_2.lib")
#pragma comment(lib,"ws2_32.lib")
#endif
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _WIN32
#if _MSC_VER < 1300
#define for if(false);else for
#define vsnprintf _vsnprintf
#define snprintf _snprintf
#define _strdup strdup
#define nextafter _nextafter
#endif
#else
#define _vsnprintf vsnprintf
#define _snprintf snprintf
#define _msize malloc_usable_size
#endif

#ifdef _WIN32
#define snprintf _snprintf
#define strncasecmp _strnicmp
#define strcasecmp _stricmp
#else
#define strnicmp strncasecmp
#define stricmp strcasecmp
#define _stricoll strcasecoll
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////

#undef FALSE
#undef TRUE
#undef BOOL
typedef int BOOL;
enum { FALSE = 0, TRUE = 1 };
#define GETBUFVAL(_T, _P, _I)  ((_P) ? ((_T*)(_P))[_I] : 0)
#define GETBUFPTR(_T, _P, _I)  ((_P) ? (((_T*)(_P))+(_I)) : 0)
#define ABSSUB(a, b)  ((a)>(b) ? ((a)-(b)) : ((b)-(a)))

/////////////////////////////////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////////////////////////////////
//#define CC_SWAP(a,b,t) ((t) = (a), (a) = (b), (b) = (t))
#ifndef MIN
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif
#ifndef MAX
#define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif
#define MINMAX_UPDATE(x, mi, ma) if (x<mi) { mi = x; } else if (x>ma) { ma = x; }
#define MINMAX(x1, x2, t)  if ((x1)>(x2)) { CC_SWAP((x1), (x2), t); }
/////////////////////////////////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////////////////////////////////
// CC_SWAP
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
/////////////////////////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////////////////////////
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
#ifndef _countof
#define _countof(_ARR)  (int)(sizeof(_ARR)/sizeof((_ARR)[0]))
#endif // _countof
#define CC_8TO32F(x)  (x)
#define SQR(_X)    ((_X)*(_X))
#define SQR3(_X)   ((_X)*(_X)*(_X))
#define SQR4(_X)   ((_X)*(_X)*(_X)*(_X))
#define MAX_INT             0x7fffffff
#define MIN_INT             0x80000000
#define MAX_DOUBLE          1.7976931348623158E+308
#define MIN_DOUBLE         -1.7976931348623158E+308

/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////
//#define CC_TYPE_NAME_IMAGE "opencv-image"
//#define CC_IS_IMAGE_HDR(img)   ((img) != NULL && ((const IplImage*)(img))->nSize == sizeof(IplImage))
//#define CC_IS_IMAGE(img)   (CC_IS_IMAGE_HDR(img) && ((IplImage*)img)->imageData != NULL)
/* for storing double-precision
floating point data in IplImage's */
#define IMG_DEPTH_64F  64
/* get reference to pixel at (col,row),
for multi-channel images (col) should be multiplied by number of channels */
#define CC_IMAGE_ELEM( image, elemtype, row, col )        *img_at(elemtype, image, row, col)
#define COLOR_REV
//#undef RGB
//#define RGB(r,g,b)          ((COLORREF)(((BYTE)(b)|((uint16)((BYTE)(g))<<8))|(((uint32)(BYTE)(r))<<16)))
#ifdef COLOR_REV
#define _RGBA(r,g,b,a)         (((uint32)(((uint8)(b)|((uint16)((uint8)(g))<<8))|(((uint32)(uint8)(r))<<16)))|(( (uint32)(uint8)(a) )<<24))
#define _RGBA16(r,g,b,a)       (((uint64)(((uint16)(b)|((uint32)((uint16)(g))<<16))|(((uint64)(uint16)(r))<<32)))|(( (uint64)(uint16)(a) )<<48))
#else
#define _RGBA(r,g,b,a)         (((uint32)(((uint8)(r)|((uint16)((uint8)(g))<<8))|(((uint32)(uint8)(b))<<16)))|(( (uint32)(uint8)(a) )<<24))
#define _RGBA16(r,g,b,a)       (((uint64)(((uint16)(r)|((uint32)((uint16)(g))<<16))|(((uint64)(uint16)(b))<<32)))|(( (uint64)(uint16)(a) )<<48))
#endif
#define BOUND_RGBA(r,g,b,a)        _RGBA(BOUND(r, 0, 255), BOUND(g, 0, 255), BOUND(b, 0, 255), BOUND(a, 0, 255))
#define _RGB(r,g,b)         _RGBA(r,g,b,255)
#define _RGB16(r,g,b)         _RGBA16(r,g,b,255)
#define INT2RGB(i)          _RGBA(((i>>16)&0xff), ((i>>8)&0xff), ((i)&0xff), 255)
//#define HEX2INT1(H)   (('0'<=(H) && (H)<='9') ? (H)-'0' : ('a'<=(H) && (H)<='f') ? (H)-'a'+10 : ('A'<=(H) && (H)<='F') ? (H)-'A'+10 : 0)
//#define HEX2INT2(H)   ((HEX2INT1((H)[0])<<4)|(HEX2INT1((H)[1])))
//#define S_RGB(S)        _RGBA(HEX2INT2((S)),HEX2INT2((S)+2),HEX2INT2((S)+4),255)
//#define CC_RGB( r, g, b )  cvScalar( (b), (g), (r), 0 )
#define GetBV(rgb) ((BYTE)(rgb))
#define GetGV(rgb) ((BYTE)(((uint16)(rgb)) >> 8))
#define GetRV(rgb) ((BYTE)((rgb)>>16))
#define GetAV(rgb) ((BYTE)((rgb)>>24))
#define CC_RGB( r, g, b )  cScalar(b,g,r, 255)
#define _rgb(r,g,b)  _RGB(255*(r), 255*(g), 255*(b))
#define _rgba(r,g,b,a)  _RGBA(255*(r), 255*(g), 255*(b), 255*(a))
#define _RGB_A(clr, _a)   _RGBA(GetRV(clr),GetGV(clr),GetBV(clr),_a)
#define _rgb_a(clr, _a)   _RGBA(GetRV(clr),GetGV(clr),GetBV(clr),(_a)*255)
#define _A_RGB(_a, clr)    ((clr&0x00ffffff)|(_a<<24))
#define _ARGB(a,r,g,b)  _RGBA(r,g,b,a)
#define _SetAlpha _RGB_A
#define CVRGBDEF( r, g, b )  _RGB(r, g, b)
#define GetRGBV(rgb) ((rgb)&_RGBA(255,255,255,0))
#define SWAPRB(_COL)  _RGBA(GetBV(_COL), GetGV(_COL), GetRV(_COL), GetAV(_COL))
#define CVSCALARDATA(scalar, _I) (((unsigned char*)scalar)[_I])
#define RGBDEF _RGB
#define SCALARDATA(scalar, _I) (((unsigned char*)(scalar))[_I])
#define RGBMUL(rgb, _x)  CC_RGB(GetRV(rgb)*(_x), GetGV(rgb)*(_x), GetBV(rgb)*(_x))
#define SCALARTORAWDATA(_scalar, _data, _cn) {int _cn1 = _cn; while(_cn1--) { (_data)[_cn1] = CVSCALARDATA(_scalar, _cn1); } }

/////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct {
  int s, e;
} IRANGE;
typedef struct {
  double s, e;
} DRANGE;
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


#endif // _STDC_STDDEF_H_
