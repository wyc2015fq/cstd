
CC_INLINE int nulprintf(const char* fmt, ...) {
  (void)fmt;
  return 0;
}

#define FNEAR(a, b, e)  (((a)-(b)) < (e) && ((a)-(b)) > -(e))

#ifndef ASSERT
#ifdef _DEBUG
#ifndef ASSERT
#define ASSERT( exp )           assert( exp )
#define VERIFY( exp )           assert( exp )
//#define TRACE                   OutputDebugString
#define TRACE  nulprintf
#endif
#else
#define TRACE  nulprintf
#define ASSERT( exp )           ((void)0)
#define VERIFY( exp )           ((void)(exp))
//void tracenull(void* a, ...) {}
//#define TRACE
#endif //DEBUG
#endif // ASSERT
// additional macros
#define ASSERT_BREAK( exp )             { ASSERT(exp); if( !(exp) ) break; }
#define ASSERT_CONTINUE( exp )          { ASSERT(exp); if( !(exp) ) continue; }
#define ASSERT_RETURN( exp )            { ASSERT(exp); if( !(exp) ) return; }
#define ASSERT_RETURN_NULL( exp )       { ASSERT(exp); if( !(exp) ) return 0; }
#define ASSERT_RETURN_FALSE( exp )      { ASSERT(exp); if( !(exp) ) return false; }
#define SIZEOFARR(_T, _N)           (sizeof(_T)*(_N))

#ifdef _DEBUG
#define BUFUSEBEGIN(_BUF, _BUFSZ)   { unsigned char* __BUFEND__ = (unsigned char*)(_BUF)+(_BUFSZ), * __BUFBEG__ = (unsigned char*)(_BUF)/**/
#define BUFUSEEND()                 ASSERT(__BUFBEG__<=__BUFEND__);/**/ }
#else
#define BUFUSEBEGIN(_BUF, _BUFSZ)   { unsigned char* __BUFEND__ = (unsigned char*)(_BUF)+(_BUFSZ), * __BUFBEG__ = (unsigned char*)(_BUF)/**/
#define BUFUSEEND()                 /*ASSERT(__BUFBEG__<=__BUFEND__);*/ }
#endif
//#define BUFMALLOC(_T, _N)           (_T*)(__BUFEND__-=SIZEOFARR(_T, _N))
//#define BUFFREE(_T, _N)             (_T*)(__BUFEND__+=SIZEOFARR(_T, _N))
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
#define OutputDebugLastError() \
  { static int _time=0; int _err = GetLastError(); TCHAR buf[256]; TCHAR* lpMsgBuf; FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, \
        NULL, _err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &lpMsgBuf, 0, NULL );\
    _snprintf(buf, 256, "%s(%d):%d:%d:%s", __FILE__, __LINE__, _err, _time++, lpMsgBuf); if (_err) OutputDebugString( buf ); LocalFree( lpMsgBuf ); }
#define FSCANF_ALL(pf, fmt, _N, _F) { int _I = 0; for (; _I < (_N); ++_I) fscanf(pf, fmt, _F + _I); }
#ifndef INTCAST
#define INTCAST(x)  (int)(x)
#endif
#define SETBIT(X, M, B)  if (B) {X|=M;} else {X&=~(M);}
/* CC_FUNCNAME macro defines icvFuncName constant which is used by CC_ERROR macro */
#ifdef CC_NO_FUNC_NAMES
#define CC_FUNCNAME( Name )
#define cvFuncName ""
#else
#define CC_FUNCNAME( Name )  static const char cvFuncName[] = Name
#endif

#define ccError( status, func_name, err_msg, file_name, line )   printf("%s(%d):%s %s", file_name, line, func_name, err_msg)
/*
 CC_ERROR macro unconditionally raises error with passed code and message.
 After raising error, control will be transferred to the exit label.
 */
#define CC_ERROR( Code, Msg )  { ASSERT(0 && Msg); ccError( (Code), cvFuncName, Msg, __FILE__, __LINE__ ); __CC_EXIT__; }
/* Simplified form of CC_ERROR */
#define CC_ERROR_FROM_CODE( code )       CC_ERROR( code, "" )
#define CC_CHECK()

#define CC_CALL( Func ) { Func; CC_CHECK(); }
/* Runtime assertion macro */
#define CC_ASSERT( Condition ) { if( !(Condition) ) CC_ERROR( CC_StsInternal, "Assertion: " #Condition " failed" ); }
#define __CC_BEGIN__       {int __CC_BEGIN=0
#define __CC_END__         goto exit; exit: ; }
#define __CC_EXIT__        goto exit
#undef CC_ERROR
#define CC_ERROR(_ERRID, _ERRSTR) do { ASSERT(0); printf("%s", _ERRSTR); } while(0)
//#define CC_ERROR(a, b)  { fprintf(stderr, b); }
// bool
#undef FALSE
#undef TRUE
#undef BOOL
typedef int BOOL;
enum {FALSE = 0, TRUE = 1};
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

// ??????
#define MEMCMP(_N, _F, _P, _R)  {int _I=0; for (; _I<_N && 0==(_R=CC_CMP(*(_F+_I),*(_P+_I))); ++_I); }
// ??????
#define MEMRCMP(_N, _F, _P, _R) {int _I=_N; for (; (0<_I--) && 0==(_R=CC_CMP(*(_F+_I),*(_P+_I))); ); }
/* min & max without jumps */
#define IMIN(a, b)  ((a) ^ (((a)^(b)) & (((a) < (b)) - 1)))
#define IMAX(a, b)  ((a) ^ (((a)^(b)) & (((a) > (b)) - 1)))


#ifndef BOUND
//#define BOUND(VAL, MINVAL, MAXVAL)  (MAX((MINVAL), MIN((VAL), (MAXVAL))))
#define BOUND(x,a,b) (((x) < (a)) ? (a) : (((x) > (b)) ? (b) : (x)))
#endif
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


// Data structures for persistence (a.k.a serialization) functionality        *
/* "black box" file storage */

/* Storage flags: */
#define CC_STORAGE_READ          0
#define CC_STORAGE_WRITE         1
#define CC_STORAGE_WRITE_TEXT    CC_STORAGE_WRITE
#define CC_STORAGE_WRITE_BINARY  CC_STORAGE_WRITE
#define CC_STORAGE_APPEND        2
/* List of attributes: */
#if 0
#define CC_NODE_NONE        0
#define CC_NODE_INT         1
#define CC_NODE_INTEGER     CC_NODE_INT
#define CC_NODE_REAL        2
#define CC_NODE_FLOAT       CC_NODE_REAL
#define CC_NODE_STR         3
#define CC_NODE_STRING      CC_NODE_STR
#define CC_NODE_REF         4 /* not used */
#define CC_NODE_SEQ         5
#define CC_NODE_MAP         6
#define CC_NODE_TYPE_MASK   7
#define CC_NODE_TYPE(flags)  ((flags) & CC_NODE_TYPE_MASK)
#endif
/* file node flags */
#define CC_NODE_FLOW        8 /* Used only for writing structures in YAML format. */
#define CC_NODE_USER        16
#define CC_NODE_EMPTY       32
#define CC_NODE_NAMED       64
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
//                Compile-time tuning parameters                            *
/* maximal size of vector to run matrix operations on it inline (i.e. w/o ipp calls) */
#define CC_MAX_INLINE_MAT_OP_SIZE  10
/* maximal linear size of matrix to allocate it on stack. */
#define CC_MAX_LOCAL_MAT_SIZE  32
/* maximal size of local memory storage */
#define CC_MAX_LOCAL_SIZE \
  (CC_MAX_LOCAL_MAT_SIZE*CC_MAX_LOCAL_MAT_SIZE*(int)sizeof(double))
/* default image row align (in bytes) */
#define CC_DEFAULT_IMAGE_ROW_ALIGN  4
/* matrices are continuous by default */
#define CC_DEFAULT_MAT_ROW_ALIGN  1
/* maximum size of dynamic memory buffer.
   cvAlloc reports an error if a larger block is requested. */
#define CC_MAX_ALLOC_SIZE    (((size_t)1 << (sizeof(size_t)*8-2)))
/* the alignment of all the allocated buffers */
#define CC_MALLOC_ALIGN    32
/* default alignment for dynamic data strucutures, resided in storages. */
#define CC_STRUCT_ALIGN    ((int)sizeof(double))
/* default storage block size */
#define CC_STORAGE_BLOCK_SIZE   ((1<<16) - 128)
/* default memory block for sparse array elements */
#define CC_SPARSE_MAT_BLOCK    (1<<12)
/* initial hash table size */
#define CC_SPARSE_HASH_SIZE0    (1<<10)
/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#define CC_SPARSE_HASH_RATIO    3
/* max length of strings */
#define CC_MAX_STRLEN  1024
#if 0 /*def  CC_CHECK_FOR_NANS*/
#define CC_CHECK_NANS( arr ) cvCheckArray((arr))
#else
#define CC_CHECK_NANS( arr )
#endif
#define IPCVAPI_IMPL(type, name, args, arg_names) static type name args
//                    Common declarations                                   *
/* get alloca declaration */
#ifdef __GNUC__
#undef alloca
#define alloca __builtin_alloca
#elif defined _WIN || defined _WIN || defined WIN64 || defined _WIN64 || \
      defined WINCE || defined _MSC_VER || defined __BORLANDC__
#include <malloc.h>
#elif defined HAVE_ALLOCA_H
#include <alloca.h>
#elif defined HAVE_ALLOCA
#include <stdlib.h>
#else
//#error "No alloca!"
#endif
#ifdef __GNUC__
#define CC_DECL_ALIGNED(x) __attribute__ ((aligned (x)))
#elif defined _MSC_VER
#define CC_DECL_ALIGNED(x) __declspec(align(x))
#else
#define CC_DECL_ALIGNED(x)
#endif
/* ! DO NOT make it an inline function */
#define cvStackAlloc(size) cvAlignPtr( alloca((size) + CC_MALLOC_ALIGN), CC_MALLOC_ALIGN )
#if defined _MSC_VER || defined __BORLANDC__
#define CC_BIG_INT(n)   n##I64
#define CC_BIG_UINT(n)  n##UI64
#else
#define CC_BIG_INT(n)   n##LL
#define CC_BIG_UINT(n)  n##ULL
#endif
//#define CC_IMPL CC_EXTERN_C
//#define CC_DBG_BREAK() { volatile int* crashMe = 0; *crashMe = 0; }
/* default step, set in case of continuous data
   to work around checks for valid step in some ipp functions */
#define CC_STUB_STEP     (1 << 30)
#define CC_SIZEOF_FLOAT ((int)sizeof(float))
#define CC_SIZEOF_SHORT ((int)sizeof(short))
#define CC_ORIGIN_TL  0
#define CC_ORIGIN_BL  1
/* IEEE754 constants and macros */
#define CC_POS_INF       0x7f800000
#define CC_NEG_INF       0x807fffff /* CC_TOGGLE_FLT(0xff800000) */
#define CC_1F            0x3f800000
#define CC_TOGGLE_FLT(x) ((x)^((int)(x) < 0 ? 0x7fffffff : 0))
#define CC_TOGGLE_DBL(x) \
  ((x)^((int64)(x) < 0 ? CC_BIG_INT(0x7fffffffffffffff) : 0))
#define CC_NOP(a)      (a)
#define CC_ADD(a, b)   ((a) + (b))
#define CC_SUB(a, b)   ((a) - (b))
#define CC_MUL(a, b)   ((a) * (b))
#define CC_AND(a, b)   ((a) & (b))
#define CC_OR(a, b)    ((a) | (b))
#define CC_XOR(a, b)   ((a) ^ (b))
#define CC_ANDN(a, b)  (~(a) & (b))
#define CC_ORN(a, b)   (~(a) | (b))
#define CC_SQR(a)      ((a) * (a))
#define CC_LT(a, b)    ((a) < (b))
#define CC_LE(a, b)    ((a) <= (b))
#define CC_EQ(a, b)    ((a) == (b))
#define CC_NE(a, b)    ((a) != (b))
#define CC_GT(a, b)    ((a) > (b))
#define CC_GE(a, b)    ((a) >= (b))
#define CC_NONZERO(a)      ((a) != 0)
#define CC_NONZERO_FLT(a)  (((a)+(a)) != 0)
#define CC_PASTE2(a,b) a##b
#define CC_PASTE(a,b)  CC_PASTE2(a,b)
/* general-purpose saturation macros */
#define CC_CAST_8U(t)  (unsigned char)(!((t) & ~255) ? (t) : (t) > 0 ? 255 : 0)
#define CC_CAST_8S(t)  (schar)(!(((t)+128) & ~255) ? (t) : (t) > 0 ? 127 : -128)
#define CC_CAST_16U(t) (ushort)(!((t) & ~65535) ? (t) : (t) > 0 ? 65535 : 0)
#define CC_CAST_16S(t) (short)(!(((t)+32768) & ~65535) ? (t) : (t) > 0 ? 32767 : -32768)
#define CC_CAST_32S(t) (int)(t)
#define CC_CAST_64S(t) (int64)(t)
#define CC_CAST_32F(t) (float)(t)
#define CC_CAST_64F(t) (double)(t)
#define CC_PASTE2(a,b) a##b
#define CC_PASTE(a,b)  CC_PASTE2(a,b)
#define CC_EMPTY
#define CC_MAKE_STR(a) #a
#define CC_DEFINE_MASK \
  float maskTab[2]; maskTab[0] = 0.f; maskTab[1] = 1.f;
#define CC_ANDMASK( m, x )     ((x) & (((m) == 0) - 1))
/* (x) * ((m) == 1 ? 1.f : (m) == 0 ? 0.f : <ERR> */
#define CC_MULMASK( m, x )       (maskTab[(m) != 0]*(x))
/* (x) * ((m) == -1 ? 1.f : (m) == 0 ? 0.f : <ERR> */
#define CC_MULMASK1( m, x )      (maskTab[(m)+1]*(x))
#define CC_ZERO_OBJ(x)  memset((x), 0, sizeof(*(x)))
#define CC_DIM(static_array) ((int)(sizeof(static_array)/sizeof((static_array)[0])))
#define CC_UN_ENTRY_C1(worktype) worktype s0 = scalar[0]
#define CC_UN_ENTRY_C2(worktype) worktype s0 = scalar[0], s1 = scalar[1]
#define CC_UN_ENTRY_C3(worktype) worktype s0 = scalar[0], s1 = scalar[1], s2 = scalar[2]
#define CC_UN_ENTRY_C4(worktype) worktype s0 = scalar[0], s1 = scalar[1], s2 = scalar[2], s3 = scalar[3]
#define cvUnsupportedFormat "Unsupported format"
CC_INLINE void* cvAlignPtr(const void* ptr, int align CC_DEFAULT(32))
{
  assert((align & (align - 1)) == 0);
  return (void*)(((size_t)ptr + align - 1) & ~(size_t)(align - 1));
}
CC_INLINE int cvAlign(int size, int align)
{
  assert((align & (align - 1)) == 0 && size < INT_MAX);
  return (size + align - 1) & -align;
}
#define CC_DESCALE(x,n)     (((x) + (1 << ((n)-1))) >> (n))

typedef struct {
  float r, i;
} SCOMPLEX;
typedef struct {
  double r, i;
} DCOMPLEX;
#if 0
typedef struct {
  float re, im;
} CvComplex32f;
typedef struct {
  float re, im;
} CvComplex64f;
CC_INLINE CvComplex32f cvComplex32f(float re, float im)
{
  CvComplex32f s;
  s.re = re;
  s.im = im;
  return s;
}
CC_INLINE CvComplex64f cvComplex64f(double re, double im)
{
  CvComplex64f s;
  s.re = re;
  s.im = im;
  return s;
}
CC_INLINE CvComplex64f cvComplex64f32f(CvComplex32f c)
{
  CvComplex64f s;
  s.re = c.re;
  s.im = c.im;
  return s;
}
CC_INLINE CvComplex32f cvComplex32f64f(CvComplex64f c)
{
  CvComplex32f s;
  s.re = c.re;
  s.im = c.im;
  return s;
}
#endif
CC_INLINE SCOMPLEX sCOMPLEX(float re, float im)
{
  SCOMPLEX s;
  s.r = re;
  s.i = im;
  return s;
}
CC_INLINE DCOMPLEX dCOMPLEX(double re, double im)
{
  DCOMPLEX s;
  s.r = re;
  s.i = im;
  return s;
}
#define FCOMPLEX_SET(c, RE, IM)   (c.r=RE, c.i=IM)
#define FCOMPLEX_ADD(c,a,b)       FCOMPLEX_SET( c, a.r + b.r, a.i + b.i )
#define FCOMPLEX_SUB(c,a,b)       FCOMPLEX_SET( a.r - b.r, a.i - b.i )
#define FCOMPLEX_NEG(c,a)         FCOMPLEX_SET( c, -a.r, -a.i )
#define FCOMPLEX_MUL(c,a,b)       FCOMPLEX_SET( c, a.r*b.r - a.i*b.i, a.r*b.i + a.i*b.r )
#define FCOMPLEX_ABS(a)           sqrt( a.r*a.r + a.i*a.i )
#define FCOMPLEX_CONJ(c, a)       FCOMPLEX_SET( c, a.r, -a.i )
#define FCOMPLEX_DIV(c,a,b)       ( c.i = 1./(b.r*b.r + b.i*b.i), FCOMPLEX_SET( c, ((a.r*b.r + a.i*b.i)*c.i), ((-a.r*b.i + a.i*b.r)*c.i) ) )
#define FCOMPLEX_SCALE(c,a,b)     FCOMPLEX_SET( c, (a.r*b), (a.i*b) )
#define CvComplex_set(c,a,b)      (c.r=a, c.i=b)
#define CvComplex_neg(c,a)        (c.r=-a.r, c.i=-a.i )
#define CvComplex_conj(c,a)       (c.r=a.r, c.i=-a.i )
#define CvComplex_add(c,a,b)      (c.r=a.r + b.r, c.i=a.i + b.i )
#define CvComplex_sub(c,a,b)      (c.r=a.r - b.r, c.i=a.i - b.i )
#define CvComplex_mul(c,a,b)      (c.r=a.r*b.r - a.i*b.i, c.i=a.r*b.i + a.i*b.r )
#define CvComplex_muladd(c,a,b)   (c.r+=a.r*b.r - a.i*b.i, c.i+=a.r*b.i + a.i*b.r )
#define CvComplex_mula(c,a,b)     (c.r=a*b.r, c.i=a*b.i )
#define CvComplex_div(c,a,b)      (c.i = 1./(b.r*b.r + b.i*b.i),c.r= (TYPE)((a.r*b.r + a.i*b.i)*c.i),c.i=(TYPE)((-a.r*b.i + a.i*b.r)*c.i) )
#define CvComplex_abs2(a)         (( a.r*a.r + a.i*a.i ))
#define CvComplex_abs(a)          (sqrt(CvComplex_abs2(a)))
#define complex_set(c,a,b)        (c[0]=a, c[1]=b)
#define complex_neg(c,a)          (c[0]=-a[0], c[1]=-a[1] )
#define complex_conj(c,a)         (c[0]=a[0], c[1]=-a[1] )
#define complex_add(c,a,b)        (c[0]=a[0] + b[0], c[1]=a[1] + b[1] )
#define complex_sub(c,a,b)        (c[0]=a[0] - b[0], c[1]=a[1] - b[1] )
#define complex_mul(c,a,b)        (c[0]=a[0]*b[0] - a[1]*b[1], c[1]=a[0]*b[1] + a[1]*b[0] )
#define complex_mula(c,a,b)       (c[0]=a*b[0], c[1]=a*b[1] )
#define complex_div(c,a,b)        (c[1] = 1/(b[0]*b[0] + b[1]*b[1]), c[0]= ((a[0]*b[0] + a[1]*b[1])*c[1]),c[1]=((a[1]*b[0]-a[0]*b[1])*c[1]) )
//#define complex_abs(c,a)         (sqrt( a[0]*a[0] + a[1]*a[1] ))
/* This is a small engine for performing fast division of multiple numbers
   by the same constant. Most compilers do it too if they know the divisor value
   at compile-time. The algorithm was taken from Agner Fog's optimization guide
   at http://www.agner.org/assem */
typedef struct CvFastDiv {
  unsigned delta, scale, divisor;
}
CvFastDiv;
#define CC_FAST_DIV_SHIFT 32
CC_INLINE CvFastDiv cvFastDiv(int divisor)
{
  CvFastDiv fastdiv;
  uint64 temp;
  assert(divisor >= 1);
  temp = ((uint64)1 << CC_FAST_DIV_SHIFT) / divisor;
  fastdiv.divisor = divisor;
  fastdiv.delta = (unsigned)(((temp & 1) ^ 1) + divisor - 1);
  fastdiv.scale = (unsigned)((temp + 1) >> 1);
  return fastdiv;
}
#define CC_FAST_DIV( x, fastdiv ) \
  ((int)(((int64)((x)*2 + (int)(fastdiv).delta))*(int)(fastdiv).scale>>CC_FAST_DIV_SHIFT))
#define CC_FAST_UDIV( x, fastdiv ) \
  ((int)(((uint64)((x)*2 + (fastdiv).delta))*(fastdiv).scale>>CC_FAST_DIV_SHIFT))
#define CC_MEMCPY_CHAR( dst, src, len )   { \
    size_t _icv_memcpy_i_, _icv_memcpy_len_ = (len); \
    char* _icv_memcpy_dst_ = (char*)(dst); \
    const char* _icv_memcpy_src_ = (const char*)(src); \
    for( _icv_memcpy_i_ = 0; _icv_memcpy_i_ < _icv_memcpy_len_; _icv_memcpy_i_++ ) \
      _icv_memcpy_dst_[_icv_memcpy_i_] = _icv_memcpy_src_[_icv_memcpy_i_]; \
  }
#define CC_MEMCPY_INT( dst, src, len )   { \
    size_t _icv_memcpy_i_, _icv_memcpy_len_ = (len); \
    int* _icv_memcpy_dst_ = (int*)(dst); \
    const int* _icv_memcpy_src_ = (const int*)(src); \
    assert( ((size_t)_icv_memcpy_src_&(sizeof(int)-1)) == 0 && \
        ((size_t)_icv_memcpy_dst_&(sizeof(int)-1)) == 0 ); \
    for(_icv_memcpy_i_=0;_icv_memcpy_i_<_icv_memcpy_len_;_icv_memcpy_i_++) \
      _icv_memcpy_dst_[_icv_memcpy_i_] = _icv_memcpy_src_[_icv_memcpy_i_]; \
  }
#define CC_MEMCPY_AUTO( dst, src, len )   { \
    size_t _icv_memcpy_i_, _icv_memcpy_len_ = (len); \
    char* _icv_memcpy_dst_ = (char*)(dst); \
    const char* _icv_memcpy_src_ = (const char*)(src); \
    if( (_icv_memcpy_len_ & (sizeof(int)-1)) == 0 )     { \
      assert( ((size_t)_icv_memcpy_src_&(sizeof(int)-1)) == 0 && \
          ((size_t)_icv_memcpy_dst_&(sizeof(int)-1)) == 0 ); \
      for( _icv_memcpy_i_ = 0; _icv_memcpy_i_ < _icv_memcpy_len_; \
          _icv_memcpy_i_+=sizeof(int) ) \
      { \
        *(int*)(_icv_memcpy_dst_+_icv_memcpy_i_) = \
            *(const int*)(_icv_memcpy_src_+_icv_memcpy_i_); \
      } \
    } \
    else \
    { \
      for(_icv_memcpy_i_ = 0; _icv_memcpy_i_ < _icv_memcpy_len_; _icv_memcpy_i_++)\
        _icv_memcpy_dst_[_icv_memcpy_i_] = _icv_memcpy_src_[_icv_memcpy_i_]; \
    } \
  }
#define CC_ZERO_CHAR( dst, len ) \
  { \
    size_t _icv_memcpy_i_, _icv_memcpy_len_ = (len); \
    char* _icv_memcpy_dst_ = (char*)(dst); \
    \
    for( _icv_memcpy_i_ = 0; _icv_memcpy_i_ < _icv_memcpy_len_; _icv_memcpy_i_++ ) \
      _icv_memcpy_dst_[_icv_memcpy_i_] = '\0'; \
  }
#define CC_ZERO_INT( dst, len ) \
  { \
    size_t _icv_memcpy_i_, _icv_memcpy_len_ = (len); \
    int* _icv_memcpy_dst_ = (int*)(dst); \
    assert( ((size_t)_icv_memcpy_dst_&(sizeof(int)-1)) == 0 ); \
    \
    for(_icv_memcpy_i_=0;_icv_memcpy_i_<_icv_memcpy_len_;_icv_memcpy_i_++) \
      _icv_memcpy_dst_[_icv_memcpy_i_] = 0; \
  }
//       Structures and macros for integration with IPP                     *
/* IPP-compatible return codes */

#define CC_ERROR_FROM_STATUS( result )   CC_ERROR( cvErrorFromIppStatus( result ), "OpenCV function failed" )
#define CC_PLUGIN_NONE      0
#define CC_PLUGIN_OPTCV     1 /* custom "emerged" ippopencv library */
#define CC_PLUGIN_IPPCV     2 /* IPP: computer vision */
#define CC_PLUGIN_IPPI      3 /* IPP: image processing */
#define CC_PLUGIN_IPPS      4 /* IPP: signal processing */
#define CC_PLUGIN_IPPVM     5 /* IPP: vector math functions */
#define CC_PLUGIN_IPPCC     6 /* IPP: color space conversion */
#define CC_PLUGIN_MKL       8 /* Intel Math Kernel Library */
#define CC_PLUGIN_MAX      16
#define CC_PLUGINS1(lib1) ((lib1)&15)
#define CC_PLUGINS2(lib1,lib2) (((lib1)&15)|(((lib2)&15)<<4))
#define CC_PLUGINS3(lib1,lib2,lib3) (((lib1)&15)|(((lib2)&15)<<4)|(((lib2)&15)<<8))
#define CC_NOTHROW throw()
#ifndef IPCVAPI
#define IPCVAPI(type,declspec,name,args) \
  /* function pointer */ \
  typedef type (declspec* name##_t) args; \
  extern name##_t name##_p; \
  type declspec name args;
#endif
#ifndef IPCVAPI_IMPL
#define IPCVAPI_IMPL(type,name,args,arg_names) \
  static type CC_STDCALL name##_f args; \
  name##_t name##_p = name##_f; \
  type CC_STDCALL name args { return name##_p arg_names; } \
  static type CC_STDCALL name##_f args
#endif
/* IPP types' enumeration */
typedef enum CvDataType {
  cv1u,
  cv8u, cv8s,
  cv16u, cv16s, cv16sc,
  cv32u, cv32s, cv32sc,
  cv32f, cv32fc,
  cv64u, cv64s, cv64sc,
  cv64f, cv64fc
} CvDataType;
typedef enum CvHintAlgorithm {
  cvAlgHintNone,
  cvAlgHintFast,
  cvAlgHintAccurate
} CvHintAlgorithm;
typedef enum CvCmpOp {
  cvCmpLess,
  cvCmpLessEq,
  cvCmpEq,
  cvCmpGreaterEq,
  cvCmpGreater
} CvCmpOp;
#if defined _MSC_VER || defined __BORLANDC__
#define CC_BIG_INT(n)   n##I64
#define CC_BIG_UINT(n)  n##UI64
#else
#define CC_BIG_INT(n)   n##LL
#define CC_BIG_UINT(n)  n##ULL
#endif
#ifndef _WIN
typedef struct _FIXED1 {
#ifndef _MAC
  uint16    fract;
  short   value;
#else
  short   value;
  uint16    fract;
#endif
} FIXED1;
#endif // _WIN
/*****************************************************************************/
#define cvInvSqrt(value) ((float)(1./sqrt(value)))
#define cvSqrt(value)  ((float)sqrt(value))
/*****************************************************************************/

#define CC_INF  FLT_MAX
#define CC_EPS  FLT_EPSILON
#ifndef ABS
#define ABS(a) ( (a) > -(a) ? (a) : -(a) )
#endif
#ifndef FABS
//#define FABS(a) (double)fabs(a)
#define FABS(x) (((x)>0.f) ? (x) : (-(x)))
#endif
#ifndef SIN
#define SIN   (float)sin
#endif
#ifndef COS
#define COS   (float)cos
#endif
#ifndef ACOS
#define ACOS   (float)acos
#endif
#ifndef ATAN
#define ATAN   (float)atan
#endif
#ifndef SQRT
#define SQRT   (float)sqrt
#endif
// ????????
typedef int INT;
//typedef float double;
//typedef double double;
#ifndef REAL_TYPE
#define USEFLOT
#ifdef USEFLOT
#define REAL_TYPE double
#else
#define REAL_TYPE int
#endif // USEFLOT
#endif
#if defined _MSC_VER && _MSC_VER >= 1200
// disable warnings related to inline functions
#pragma warning( disable: 4251 4711 4710 4514 )
#endif

/* helper tables */
//static const uchar icvSaturate8u[] = {#include table/icvSaturate8u.txt};
//#define CC_FAST_CAST_8U(t)  (assert(-256 <= (t) || (t) <= 512), icvSaturate8u_cv[(t)+256])
#define CC_FAST_CAST_8U(t)  ((t)<0 ? 0 : ((t)>255 ? 255 : (t)))
#define CC_CALC_MIN_8U(a,b) (a) -= CC_FAST_CAST_8U((a) - (b))
#define CC_CALC_MAX_8U(a,b) (a) += CC_FAST_CAST_8U((b) - (a))
#define CC_COPY( dst, src, len, idx )     for( (idx) = 0; (idx) < (len); (idx)++) (dst)[idx] = (src)[idx]
#define CC_SET( dst, val, len, idx )      for( (idx) = 0; (idx) < (len); (idx)++) (dst)[idx] = (val)
#undef   CC_CALC_MIN
#define CC_CALC_MIN(a, b) if((a) > (b)) (a) = (b)
#undef   CC_CALC_MAX
#define CC_CALC_MAX(a, b) if((a) < (b)) (a) = (b)
/* Image smooth methods */
enum {
  CC_BLUR_NO_SCALE = 0,
  CC_BLUR  = 1,
  CC_GAUSSIAN  = 2,
  CC_MEDIAN = 3,
  CC_BILATERAL = 4
};
/* Filters used in pyramid decomposition */
typedef enum CvFilter
{
    CC_GAUSSIAN_5x5 = 7
}
CvFilter;
/* Inpainting algorithms */
enum {
  CC_INPAINT_NS      = 0,
  CC_INPAINT_TELEA   = 1
};
/* Special filters */
enum {
  CC_SCHARR = -1,
  CC_MAX_SOBEL_KSIZE = 7
};
/* Sub-pixel interpolation methods */
enum InterpolationFlags {
  CC_INTER_NN        = 0,
  CC_INTER_NEAREST   = 0,
  CC_INTER_LINEAR    = 1,
  CC_INTER_CUBIC     = 2,
  CC_INTER_AREA      = 3,
  CC_INTER_LANCZOS4  = 4,
  CC_INTER_DOWN,
  CC_INTER_MAX       = 7,
  CC_WARP_FILL_OUTLIERS   = 8,
  CC_WARP_INVERSE_MAP     = 16,
};
enum InterpolationMasks {
  CC_INTER_BITS      = 5,
  CC_INTER_BITS2     = CC_INTER_BITS * 2,
  CC_INTER_TAB_SIZE  = 1 << CC_INTER_BITS,
  CC_INTER_TAB_SIZE2 = CC_INTER_TAB_SIZE * CC_INTER_TAB_SIZE
};

/* Shapes of a structuring element for morphological operations */
enum {
  CC_SHAPE_RECT      = 0,
  CC_SHAPE_CROSS     = 1,
  CC_SHAPE_ELLIPSE   = 2,
  CC_SHAPE_CUSTOM    = 100
};
/* Morphological operations */
enum {
  CC_MOP_ERODE        = 0,
  CC_MOP_DILATE       = 1,
  CC_MOP_OPEN         = 2,
  CC_MOP_CLOSE        = 3,
  CC_MOP_GRADIENT     = 4,
  CC_MOP_TOPHAT       = 5,
  CC_MOP_BLACKHAT     = 6
};
/* Template matching methods */
enum {
  CC_TM_SQDIFF        = 0,
  CC_TM_SQDIFF_NORMED = 1,
  CC_TM_CCORR         = 2,
  CC_TM_CCORR_NORMED  = 3,
  CC_TM_CCOEFF        = 4,
  CC_TM_CCOEFF_NORMED = 5
};
/* Contour retrieval modes */
enum {
  CC_RETR_EXTERNAL = 0,//!< retrieve only the most external (top-level) contours
  CC_RETR_LIST = 1,//!< retrieve all the contours without any hierarchical information
  CC_RETR_CCOMP = 2,//!< retrieve the connected components (that can possibly be nested)
  CC_RETR_TREE = 3,//!< retrieve all the contours and the whole hierarchy
  CC_RETR_FLOODFILL = 4
};
/* Contour approximation methods */
enum {
  CC_CHAIN_CODE = 0,
  CC_CHAIN_APPROX_NONE = 1,
  CC_CHAIN_APPROX_SIMPLE = 2,
  CC_CHAIN_APPROX_TC89_L1 = 3,
  CC_CHAIN_APPROX_TC89_KCOS = 4,
  CC_LINK_RUNS = 5
};
/* initializes 8-element array for fast access to 3x3 neighborhood of a pixel */
#define CC_INIT_3X3_DELTAS( deltas, step, nch ) \
  ((deltas)[0] =  (nch),  (deltas)[1] = -(step) + (nch), \
      (deltas)[2] = -(step), (deltas)[3] = -(step) - (nch), \
      (deltas)[4] = -(nch),  (deltas)[5] =  (step) - (nch), \
      (deltas)[6] =  (step), (deltas)[7] =  (step) + (nch))
//                Planar subdivisions                                       *
typedef enum CSubdiv2DPointLocation {
  CC_PTLOC_ERROR = -2,
  CC_PTLOC_OUTSIDE_RECT = -1,
  CC_PTLOC_INSIDE = 0,
  CC_PTLOC_VERTEX = 1,
  CC_PTLOC_ON_EDGE = 2
}
CSubdiv2DPointLocation;
typedef enum CNextEdgeType {
  CC_NEXT_AROUND_ORG   = 0x00,
  CC_NEXT_AROUND_DST   = 0x22,
  CC_PREV_AROUND_ORG   = 0x11,
  CC_PREV_AROUND_DST   = 0x33,
  CC_NEXT_AROUND_LEFT  = 0x13,
  CC_NEXT_AROUND_RIGHT = 0x31,
  CC_PREV_AROUND_LEFT  = 0x20,
  CC_PREV_AROUND_RIGHT = 0x02
}
CNextEdgeType;
/* get the next edge with the same origin point (counterwise) */
#define CC_SUBDIV2D_NEXT_EDGE( edge )  (((CvQuadEdge2D*)((edge) & ~3))->next[(edge)&3])
/* Contour approximation algorithms */
enum {
  CC_POLY_APPROX_DP = 0
};
/* Shape matching methods */
enum {
  CC_CONTOURS_MATCH_I1  = 1,
  CC_CONTOURS_MATCH_I2  = 2,
  CC_CONTOURS_MATCH_I3  = 3
};
/* Shape orientation */
enum {
  CC_CLOCKWISE         = 1,
  CC_COUNTER_CLOCKWISE = 2
};
/* Histogram comparison methods */
enum {
  CC_COMP_CORREL        = 0,
  CC_COMP_CHISQR        = 1,
  CC_COMP_INTERSECT     = 2,
  CC_COMP_BHATTACHARYYA = 3
};
/* Mask size for distance transform */
enum {
  CC_DIST_MASK_3   = 3,
  CC_DIST_MASK_5   = 5,
  CC_DIST_MASK_PRECISE = 0
};
/* Distance types for Distance Transform and M-estimators */
enum {
  CC_DIST_USER    = -1, /* User defined distance */
  CC_DIST_L1      = 1,  /* distance = |x1-x2| + |y1-y2| */
  CC_DIST_L2      = 2,  /* the simple euclidean distance */
  CC_DIST_C       = 3,  /* distance = max(|x1-x2|,|y1-y2|) */
  CC_DIST_L12     = 4,  /* L1-L2 metric: distance = 2(sqrt(1+x*x/2) - 1)) */
  CC_DIST_FAIR    = 5,  /* distance = c^2(|x|/c-log(1+|x|/c)), c = 1.3998 */
  CC_DIST_WELSCH  = 6,  /* distance = c^2/2(1-exp(-(x/c)^2)), c = 2.9846 */
  CC_DIST_HUBER   = 7   /* distance = |x|<c ? x^2/2 : c(|x|-c/2), c=1.345 */
};
/* Threshold types */
enum {
  CC_THRESH_BINARY      = 0, /* value = value > threshold ? max_value : 0       */
  CC_THRESH_BINARY_INV  = 1, /* value = value > threshold ? 0 : max_value       */
  CC_THRESH_TRUNC       = 2, /* value = value > threshold ? threshold : value   */
  CC_THRESH_TOZERO      = 3, /* value = value > threshold ? value : 0           */
  CC_THRESH_TOZERO_INV  = 4, /* value = value > threshold ? 0 : value           */
  CC_THRESH_MASK        = 7,
  CC_THRESH_OTSU        = 8  /* use Otsu algorithm to choose the optimal threshold value;
                                 combine the flag with one of the above CC_THRESH_* values */
};
/* Adaptive threshold methods */
enum {
  CC_ADAPTIVE_THRESH_MEAN_C  = 0,
  CC_ADAPTIVE_THRESH_GAUSSIAN_C  = 1
};
/* FloodFill flags */
enum {
  CC_FLOODFILL_FIXED_RANGE = (1 << 16),
  CC_FLOODFILL_MASK_ONLY   = (1 << 17)
};
/* Canny edge detector flags */
enum {
  CC_CANNY_L2_GRADIENT  = (1 << 31)
};
/* Variants of a Hough transform */
enum {
  CC_HOUGH_STANDARD = 0,
  CC_HOUGH_PROBABILISTIC = 1,
  CC_HOUGH_MULTI_SCALE = 2,
  CC_HOUGH_GRADIENT = 3
};
/* Fast search data structures  */
struct CvFeatureTree;
struct CvLSH;
struct CvLSHOperations;
#include <stdio.h>
#ifdef __GNUC__
#define CC_Error( code, err ) printf("%s:%d: error: (%d) %s in function %s\n", __FILE__, __LINE__, code, err, __func__)
#else
#define CC_Error( code, err ) printf("%s:%d: error: (%d) %s\n", __FILE__, __LINE__, code, err)
#endif
#define CC_Error_( code, args ) CC_Error(code, _format args)
#define CC_Assert( expr ) { if(!(expr)) CC_Error(CC_StsAssert, #expr); }
#define CC_StaticAssert(condition, reason)    {int _a[condition]={0}; const char* _s=reason;}
#ifdef _DEBUG
#define CC_DbgAssert(expr) CC_Assert(expr)
#else
#define CC_DbgAssert(expr)
#endif
#define BOUND_MAX(x, min, max)  x=((max)+(x)%(max))%(max)
#define BOUND_CLIP(_x, _min, _max)  { if (_x<_min) _x = _min; if (_x>_max) _x = _max; }
#define BOUND_NUL(x, min, max)
#define POINT_DIST2(x, y) ((x)*(x) + (y)*(y))
#define POINT_DIST(a, b) (sqrt((double)POINT_DIST2(a.x - b.x, a.y - b.y)))
// ?锟斤拷?AoB
// A(x1, y1)
// B(x2, y2)
// ???? sinv cosv
//         A * B
// sin = ---------
//        |A|*|B|
#define ANGLE_SIN_COS(x10, y10, x20, y20, sinv, cosv) do { \
    double x1=x10, y1=y10, x2=x20, y2=y20; \
    double d = (x1*x1+y1*y1) * (x2*x2+y2*y2); \
    d = sqrt(d); \
    if (d) { \
      sinv = (x1*y2-y1*x2)/d; \
      cosv = (x1*x2+y1*y2)/d; \
    } \
  } while(0)
typedef struct MATRIX3X2 {
  int a, b, c, d, e, f;
}
MATRIX3X2;
#define MAKE_MATRIX3X2(m, A, B, C, D, E, F) \
  (  m.a=(int)(A), m.b=(int)(B), m.c=(int)(C), m.d=(int)(D), m.e=(int)(E), m.f=(int)(F)  )
#define MAKE_MATRIX3X2_TOINT(m, A, B, C, D, E, F) \
  m.a=_SHIFT_TOINT(A), m.b=_SHIFT_TOINT(B), m.c=_SHIFT_TOINT(C), m.d=_SHIFT_TOINT(D), m.e=_SHIFT_TOINT(E), m.f=_SHIFT_TOINT(F)
// |out_x|   |a c e|   |x|
// |out_y| = |b d f| * |y|
// |     |   |0 0 0|   |1|
#define MATRIX3X2_MUL_POINT(m, x, y, out_x, out_y)  out_x=m.a*(x)+m.c*(y)+m.e, out_y=m.b*(x)+m.d*(y)+m.f
//m=x*y
// |a c e|   |y.a y.c y.e|   |x.a x.c x.e|   | (a1*a2+c1*b2) ()
// |b d f| = |y.b y.d y.f| * |x.b x.d x.f| = |
// |0 0 0|   |y.0 y.0 y.0|   |x.0 x.0 x.0|   |
#define MATRIX3X2_MUL_MATRIX3X2(m, x, y)  MAKE_MATRIX3X2(m, \
    (int)_SHIFT_TOFLOAT((double)x.a * y.a + (double)x.b * y.c      ), \
    (int)_SHIFT_TOFLOAT((double)x.a * y.b + (double)x.b * y.d      ), \
    (int)_SHIFT_TOFLOAT((double)x.c * y.a + (double)x.d * y.c      ), \
    (int)_SHIFT_TOFLOAT((double)x.c * y.b + (double)x.d * y.d      ), \
    (int)_SHIFT_TOFLOAT((double)x.e * y.a + (double)x.f * y.c + y.e), \
    (int)_SHIFT_TOFLOAT((double)x.e * y.b + (double)x.f * y.d + y.f) )
//x*=y
#define MATRIX3X2_MUL(x, y) do { MATRIX3X2 _m; MATRIX3X2_MUL_MATRIX3X2(_m, x, y); x=_m; } while(0)
#define MATRIX3X2_GET_TRANSLATE(m, x, y)     MAKE_MATRIX3X2(m,1,0,0,1,x,y)
#define MATRIX3X2_GET_SCALE(m1, s, x, y)     MAKE_MATRIX3X2(m1,s,0,0,s, (s)*(x), (s)*(y) )
#define MATRIX3X2_INIT(m)  MAKE_MATRIX3X2(m, _SHIFT1,0,0,_SHIFT1,0,0)
#define MATRIX3X2_MOVE(m, x, y)  (m.e-=_SHIFT_TOINT(x), m.f-=_SHIFT_TOINT(y))
#define MATRIX3X2_ROTATE(m, rads)  {\
    int c=_SHIFT_TOINT(cos(rads)), s=_SHIFT_TOINT(sin(rads)); \
    MATRIX3X2 _x; MAKE_MATRIX3X2(_x, c,s,-s,c, 0, 0); MATRIX3X2_MUL(m, _x); }
// | c, s|
// |-s, c|
// | 0, 0|
#define MATRIX3X2_GET_ROTATE(m, rads, x, y) do { \
    int c=_SHIFT_TOINT(cos(rads)), s=_SHIFT_TOINT(sin(rads)); \
    MAKE_MATRIX3X2(m, c,s,-s,c,-(y)*c+(x)*s+x,-(y)*s+(x)*c+y); \
  } while(0)
#define MATRIX3X2_GET_ROTATE(m, rads, x, y) do { \
    int c=_SHIFT_TOINT(cos(rads)), s=_SHIFT_TOINT(sin(rads)); \
    MAKE_MATRIX3X2(m, c,s,-s,c,-(y)*c+(x)*s+x,-(y)*s+(x)*c+y); \
  } while(0)
#define MATRIX3X2_GET_STD_TOINT(m, c, s, s1, s2) \
  MAKE_MATRIX3X2_TOINT(m, c, -s, s, c, ((-s2.x*(c) - s2.y*(s))+s1.x), ((s2.x*(s) - s2.y*(c))+s1.y));
#define CC_AUTOSTEP      -1
/**************** matrix iterator: used for n-ary operations on dense arrays *********/
#define CC_MAX_ARR 10
#define CC_NO_DEPTH_CHECK     1
#define CC_NO_CN_CHECK        2
#define CC_NO_SIZE_CHECK      4
///* Finds selected eigen values and vectors of a symmetric matrix */
//CVAPI(void)  cvSelectedEigenVV( CvArr* mat, CvArr* evects, CvArr* evals,
//                                int lowindex, int highindex );
/* Calculates covariation matrix for a set of vectors */
/* transpose([v1-avg, v2-avg,...]) * [v1-avg,v2-avg,...] */
#define CC_COVAR_SCRAMBLED 0
/* [v1-avg, v2-avg,...] * transpose([v1-avg,v2-avg,...]) */
#define CC_COVAR_NORMAL    1
/* do not calc average (i.e. mean vector) - use the input vector instead
   (useful for calculating covariance matrix by parts) */
#define CC_COVAR_USE_AVG   2
/* scale the covariance matrix coefficients by number of the vectors */
#define CC_COVAR_SCALE     4
/* all the input vectors are stored in a single matrix, as its rows */
#define CC_COVAR_ROWS      8
/* all the input vectors are stored in a single matrix, as its columns */
#define CC_COVAR_COLS     16
#define CC_PCA_DATA_AS_ROW 0
#define CC_PCA_DATA_AS_COL 1
#define CC_PCA_USE_AVG 2
//                      Array Statistics                                    *
/* types of array norm */
#define CC_C            1
#define CC_L1           2
#define CC_L2           4
#define CC_NORM_MASK    7
#define CC_RELATIVE     8
#define CC_DIFF         16
#define CC_MINMAX       32
#define CC_DIFF_C       (CC_DIFF | CC_C)
#define CC_DIFF_L1      (CC_DIFF | CC_L1)
#define CC_DIFF_L2      (CC_DIFF | CC_L2)
#define CC_RELATIVE_C   (CC_RELATIVE | CC_C)
#define CC_RELATIVE_L1  (CC_RELATIVE | CC_L1)
#define CC_RELATIVE_L2  (CC_RELATIVE | CC_L2)
#define CC_REDUCE_SUM 0
#define CC_REDUCE_AVG 1
#define CC_REDUCE_MAX 2
#define CC_REDUCE_MIN 3
//        Discrete Linear Transforms and Related Functions                  *
#define CC_DXT_FORWARD  0
#define CC_DXT_INVERSE  1
#define CC_DXT_SCALE    2 /* divide result by size of array */
#define CC_DXT_INV_SCALE (CC_DXT_INVERSE + CC_DXT_SCALE)
#define CC_DXT_INVERSE_SCALE CC_DXT_INV_SCALE
#define CC_DXT_ROWS     4 /* transform each row individually */
#define CC_DXT_MUL_CONJ 8 /* conjugate the second argument of cvMulSpectrums */
#define CC_DXT_ABS 16
#define CC_DXT_ABSQ 32
#define CC_GRAPH_VERTEX        1
#define CC_GRAPH_TREE_EDGE     2
#define CC_GRAPH_BACK_EDGE     4
#define CC_GRAPH_FORWARD_EDGE  8
#define CC_GRAPH_CROSS_EDGE    16
#define CC_GRAPH_ANY_EDGE      30
#define CC_GRAPH_NEW_TREE      32
#define CC_GRAPH_BACKTRACKING  64
#define CC_GRAPH_OVER          -1
#define CC_GRAPH_ALL_ITEMS     -1
/* flags for graph vertices and edges */
#define CC_GRAPH_ITEM_VISITED_FLAG  (1 << 30)
#define CC_IS_GRAPH_VERTEX_VISITED(vtx) \
  (((CvGraphVtx*)(vtx))->flags & CC_GRAPH_ITEM_VISITED_FLAG)
#define CC_IS_GRAPH_EDGE_VISITED(edge) \
  (((CvGraphEdge*)(edge))->flags & CC_GRAPH_ITEM_VISITED_FLAG)
#define CC_GRAPH_SEARCH_TREE_NODE_FLAG   (1 << 29)
#define CC_GRAPH_FORWARD_EDGE_FLAG       (1 << 28)
//               ??????

#ifndef bzero
#define bzero(arr, n)  memset(arr, 0, n)
#endif
#define BZERO(arr, n)  memset(arr, 0, sizeof(*arr)*n)
#define BZERO1(arr)  memset(arr, 0, sizeof(*arr))

// xrect
#define XRECT_BOUND(rt, h, w) \
  do { \
    rt.w = BOUND(rt.w, 0, (w)); \
    rt.h = BOUND(rt.h, 0, (h)); \
    rt.x = BOUND(rt.x, 0, (w)-rt.w); \
    rt.y = BOUND(rt.y, 0, (h)-rt.h); \
  } while(0)
#define DETECTFACE(h, w, img, al, ai, out, len) boostdetect(get_cascade(VIS_FACE_CASCADE), h, w, img, al, ai, 0, 0, 1.1f, 2, 2, 0.3f, 0.25f, out, out + len)
#define XRECT_ZOOM(rt, z)     do { rt.x -= (z)/2; rt.y -= (z)/2; rt.w += (z); rt.h += (z); } while(0)
#define XRECT_COPY(a, b)  do { (b).x = (a).x; (b).w = (a).w; (b).y = (a).y; (b).h = (a).h; } while(0)
#define INRANGE(x, _min, _max)  (((_min)<=(x)) && ((x)<(_max)))
#define XRECT_DIST(r1, r2)  POINT_DIST2(((r1).x-(r2).x)+((r1).w-(r2).w)/2, ((r1).y-(r2).y)+((r1).h-(r2).h)/2)
#define FOR1_BEGIN(_N, _AI, _IA) do { for (_IA=0; _IA<(_N)*(_AI); _IA+=_AI) {
#define FOR1_END  }} while(0);
#define FOR2_BEGIN(_H, _W, _AL, _AI, _IA) \
  do { \
    int _LA; \
    for (_LA=0; _LA<(_H)*(_AL); _LA+=_AL) { \
      for (_IA=_LA; _IA<((_W)*(_AI)+_LA); _IA+=_AI) {
#define FOR2_END  }}} while(0);
#ifdef _WIN
#define ENDL "\r\n"
#else
#define ENDL "\n"
#endif
#define FLOATFMT "%13.13g"
#define DATA2MEN( _NAME, _LEN, _PTR, _ADDR ) do { \
    FILE* fpout = fopen(_NAME, "wb"); \
    if (fpout) { \
      int i; \
      const char* HEXFOMAT = (_ADDR) ? "0x%02x%02x%02x%02x\r\n" : "0x%02x%02x%02x%02x,\r\n"; \
      if ( _ADDR ) { \
        fprintf( fpout, "1651 1 %x 0 %x\r\n", _ADDR, ( (_LEN) + 3 ) / 4 ); \
      } \
      for ( i = 0; i < (_LEN) - 3; i += 4 ) { \
        unsigned char * r_data = ( unsigned char * ) (_PTR) + i; \
        fprintf( fpout, HEXFOMAT, r_data[ 3 ], r_data[ 2 ], r_data[ 1 ], r_data[ 0 ] ); \
      } \
      if ( i < (_LEN) ) { \
        unsigned char buf[ 4 ] = { 0 }; \
        for ( ; i < (_LEN); ++i ) { \
          buf[ i ] = *( ( unsigned char * ) (_PTR) + i ); \
        } \
        fprintf( fpout, HEXFOMAT, buf[ 3 ], buf[ 2 ], buf[ 1 ], buf[ 0 ] ); \
      } \
      fclose(fpout); \
    } \
  } while(0)

#define FPRINT1D(_F, FMT, _N, _A, _I) \
  do { \
    int _IA; \
    FILE* pf = fopen(_F, "wb"); \
    fprintf(pf, "[%d]" ENDL, _N); \
    FOR1_BEGIN(_N, _I, _IA) \
    fprintf(pf, FMT, *(_A+_IA)); \
    FOR1_END; \
    fprintf(pf, ENDL); \
    fclose(pf); \
  } while(0)
#define PRINT1D(FMT, _N, _A, _I) \
  do { \
    int _IA; \
    printf("[%d]" ENDL, _N); \
    FOR1_BEGIN(_N, _I, _IA) \
    printf(FMT, *(_A+_IA)); \
    FOR1_END; \
  } while(0)
#define FPRINT2D(_F, FMT, FMTL, _H, _W, _A, _L, _I) \
  do { \
    int _IA; \
    FILE* pf = _tfopen(_F, _T("wb")); \
    /*fprintf(pf, "[%d %d]"ENDL, _H, _W);*/ \
    FOR2_BEGIN(_H, _W, _L, _I, _IA) \
    _ftprintf(pf, FMT, *(_A+_IA)); \
    FOR1_END \
    fclose(pf); \
  } while(0)
#define PRINT2D(FMT, FMTL, _H, _W, _A, _L, _I) \
  do { \
    int _IA=0; \
    FOR2_BEGIN(_H, _W, _L, _I, _IA) \
    printf(FMT, *(_A+_IA)); \
    FOR1_END \
  } while(0)
// base algorithm
#define FILL(_N, _F, _V)  do {int _I; for (_I=0; _I<(int)(_N); ++_I) *(_F+_I)=(_V);} while(0)
#define COPY(_N, _F, _P)  do {int _I; for (_I=0; _I<(int)(_N); ++_I) *(_P+_I)=*(_F+_I);} while(0)
#define COPY1D(_N, _F, _FI, _P, _PI)  do {int _I; for (_I=0; _I<(_N); ++_I) *(_P+_I*(_PI))=*(_F+_I*(_FI));} while(0)
#define BINTH(_N, _F, _P, _T, _A, _B)  do {int _I; for (_I=0; _I<(int)(_N); ++_I) *(_P+_I)=*(_F+_I)>(_T)?(_A):(_B);} while(0)
#define BINTH1D(_N, _F, _FI, _P, _PI, _T, _A, _B)  do {int _I; for (_I=0; _I<(_N); ++_I) *(_P+_I*(_PI))=*(_F+_I*(_FI))>(_T)?(_A):(_B);} while(0)
#define COPY_CAST(_N, _F, _P, _CAST)  do {int _I; for (_I=0; _I<(int)(_N); ++_I) *(_P+_I)=_CAST(*(_F+_I));} while(0)
#define COPY1D_CAST(_N, _F, _FI, _P, _PI, _CAST)  do {int _I; for (_I=0; _I<(_N); ++_I) *(_P+_I*(_PI))=_CAST(*(_F+_I*(_FI)));} while(0)
#define FILL1D(_N, _F, _FI, _VAL)  do {int _I; for (_I=0; _I<(_N); ++_I) *(_F+_I*(_FI)) = _VAL;} while(0)
#define FILLT(N, A, AI, V) {int _ia=0; for (;_ia<(AI)*(N); _ia+=(AI)) {*((A)+_ia)=V;}}
#define COPYT(N, A, AI, B, BI) {int _ia=0, _ib=0; for (;_ia<(AI)*(N); _ia+=(AI), _ib+=(BI)) {*((A)+_ia)=*((B)+_ib);}}

#define SET1(A, A0)  (A[0]=A0)
#define SET2(A, A0,A1)  (SET1(A,A0), A[1]=A1)
#define SET3(A, A0,A1,A2)  (SET2(A,A0,A1), A[2]=A2)
#define SET4(A, A0,A1,A2,A3)  (SET3(A,A0,A1,A2), A[3]=A3)
#define SET5(A, A0,A1,A2,A3,A4)  (SET4(A,A0,A1,A2,A3), A[4]=A4)
#define SET6(A, A0,A1,A2,A3,A4,A5)  (SET5(A,A0,A1,A2,A3,A4), A[5]=A5)
#define SET7(A, A0,A1,A2,A3,A4,A5,A6)  (SET6(A,A0,A1,A2,A3,A4,A5), A[6]=A6)
#define SET8(A, A0,A1,A2,A3,A4,A5,A6,A7)  (SET7(A,A0,A1,A2,A3,A4,A5,A6), A[7]=A7)
#define SET9(A, A0,A1,A2,A3,A4,A5,A6,A7,A8)  (SET8(A,A0,A1,A2,A3,A4,A5,A6,A7), A[8]=A8)
#define SET10(A,A0,A1,A2,A3,A4,A5,A6,A7,A8,A9)  (SET9(A,A0,A1,A2,A3,A4,A5,A6,A7,A8), A[9]=A9)
#define SET11(A,A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10)  (SET10(A,A0,A1,A2,A3,A4,A5,A6,A7,A8,A9), A[10]=A10)
#define SET12(A,A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11)  (SET11(A,A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10), A[11]=A11)
#define SET13(A,A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12)  (SET12(A,A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11), A[12]=A12)
#define SET14(A,A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13)  (SET13(A,A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12), A[13]=A13)
#define SET15(A,A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14)  (SET14(A,A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13), A[14]=A14)
#define SET16(A,A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14,A15)  (SET15(A,A0,A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,A11,A12,A13,A14), A[15]=A15)

#define COPY1(d, s)  (d[0]=s[0])
#define COPY2(d, s)  (COPY1(d, s), d[1]=s[1])
#define COPY3(d, s)  (COPY2(d, s), d[2]=s[2])
#define COPY4(d, s)  (COPY3(d, s), d[3]=s[3])
#define COPY5(d, s)  (COPY4(d, s), d[4]=s[4])
#define COPY6(d, s)  (COPY5(d, s), d[5]=s[5])
#define COPY7(d, s)  (COPY6(d, s), d[6]=s[6])
#define COPY8(d, s)  (COPY7(d, s), d[7]=s[7])
#define COPY9(d, s)  (COPY8(d, s), d[8]=s[8])
#define COPY10(d, s) (COPY9(d, s), d[9]=s[9])
#define COPY11(d, s) (COPY10(d, s),d[10]=s[10])
#define COPY12(d, s) (COPY11(d, s),d[11]=s[11])
#define COPY13(d, s) (COPY12(d, s),d[12]=s[12])
#define COPY14(d, s) (COPY13(d, s),d[13]=s[13])
#define COPY15(d, s) (COPY14(d, s),d[14]=s[14])
#define COPY16(d, s) (COPY15(d, s),d[15]=s[15])

#define COPYN(_N, _F, _P) \
  do { \
    int _I=0; \
    for (; _I<(_N)-3; _I+=4) { \
      *(_P+_I  ) = *(_F+_I  ); \
      *(_P+_I+1) = *(_F+_I+1); \
      *(_P+_I+2) = *(_F+_I+2); \
      *(_P+_I+3) = *(_F+_I+3); \
    } \
    for (; _I<(_N); ++_I) { \
      *(_P+_I) = *(_F+_I); \
    } \
  } while(0)
#define COPY_BACKWARD(_N, _L, _P) \
  do { \
    int _I=0; \
    for (; _I>3-(_N); _I-=4) { \
      *(_P+_I-1) = *(_L+_I-1); \
      *(_P+_I-2) = *(_L+_I-2); \
      *(_P+_I-3) = *(_L+_I-3); \
      *(_P+_I-4) = *(_L+_I-4); \
    } \
    for (; (_I--)>-(_N); ) { \
      *(_P+_I) = *(_L+_I); \
    } \
  } while(0)
#define SUM(_N, _F, _S, _SUM) do {int _I; for (_I=0; _I<(_N)*(_S); _I+=(_S)) (_SUM)+=*(_F+_I);} while(0)
#define MEMCPY1D(_P, _F, _N)  memcpy(_P, _F, (_N)*sizeof(*(_F)))
#define MEMCPY2D(_P, _PL, _F, _FL, _M, _N) \
  do { \
    int _I; for (_I=0; _I<(_M); ++_I) \
      memcpy(_P+(_PL)*_I, _F+(_FL)*_I, _N*sizeof(*(_F))); \
  } while(0)
#define MEMSET2D(_M, _N, _F, _FL, _V) \
  do { \
    int _I; for (_I=0; _I<(_FL)*(_M); _I+=_FL) \
      memset(_F+_I, _V, _N*sizeof(*(_F))); \
  } while(0)
#define FOR_EACH1D_OP(_N, _F, _FI, _A, _AI, _B, _BI, _OP) \
  do { \
    int _IF, _IA, _IB; \
    for (_IF=0, _IA=0, _IB=0; _IF<(_N)*(_FI); _IF+=(_FI),_IA+=(_AI),_IB+=(_BI)) \
      *(_F+_IF) = *(_A+_IA) _OP *(_B+_IB); \
  } while(0)
#define FOR_EACH2D_OP(_M, _N, _F, _FL, _A, _AL, _B, _BL, _OP) \
  do { \
    int _IF, _IA, _IB, _J; \
    for (_IF=0, _IA=0, _IB=0; _IF<(_M)*(_FL); _IF+=(_FL),_IA+=(_FL),_IB+=(_BL)) \
      for (_J=0; _J<(_N); ++_J) \
        *(_F+_IF+_J) = *(_A+_IA+_J) _OP *(_B+_IB+_J); \
  } while(0)
#define FOR_EACH_OP_1D1A(_N, _A, _AI, _OP) do {int _I; for (_I=0; _I<(_N)*(_AI); _I+=_AI) (*(_A+_I)) _OP;} while(0)
#define FOR_EACH_OP_1D2A(_N, _A, _AI, _B, _BI, _OP) \
  do { \
    int _IA, _IB; \
    for (_IA=0, _IB=0; _IA<(_N)*(_AI); _IA+=(_AI), _IB+=(_BI)) \
      (*(_A+_IA)) _OP (*(_B+_IB)); \
  } while(0)
#define FOR_EACH_1D1A(_N, _A, _AI, _OP) do {int _I; for (_I=0; _I<(_N)*(_AI); _I+=_AI) _OP(*(_A+_I));} while(0)

#define FOR_EACH_1D2A(_N, _A, _AI, _B, _BI, _OP) \
  do { \
    int _IA, _IB; \
    for (_IA=0, _IB=0; _IA<(_N)*(_AI); _IA+=(_AI), _IB+=(_BI)) \
      _OP(*(_A+_IA), *(_B+_IB)); \
  } while(0)
#define FOR_EACH_2D2A(_M, _N, _A, _AL, _AI, _B, _BL, _BI, _OP) \
  do { \
    int _IA, _IB, _JA, _JB; \
    for (_JA=0, _JB=0; _JA<(_M)*(_AL); _JA+=(_AL), _JB+=(_BL)) \
      for (_IA=_JA, _IB=_JB; _IA<(_JA+(_N)*(_AI)); _IA+=(_AI), _IB+=(_BI)) \
        _OP(*(_A+_IA), *(_B+_IB)); \
  } while(0)
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
#define NORM2D_C(_M, _N, _F, _FL, _FI, _NORM) \
  do { \
    int _I, _J; \
    for (_J=0, _NORM=0.f; _J<(_M)*(_FL); _J+=(_FL)) \
      for (_I=_J; _I<(_J+(_N)*(_FI)); _I+=(_FI)) \
        if ( _NORM < (*(_F+_I) > 0 ? (*(_F+_I)) : -(*(_F+_I))) ) \
          _NORM = *(_F+_I) > 0 ? (*(_F+_I)) : -(*(_F+_I)); \
  } while(0)
#define NORM2D_L1(_M, _N, _F, _FL, _FI, _NORM) \
  do { \
    int _I, _J; \
    for (_J=0, _NORM=0.f; _J<(_M)*(_FL); _J+=(_FL)) \
      for (_I=_J; _I<(_J+(_N)*(_FI)); _I+=(_FI)) \
        _NORM += *(_F+_I) > 0 ? (*(_F+_I)) : -(*(_F+_I)); \
  } while(0)
#define NORM2D_L2(_M, _N, _F, _FL, _FI, _NORM) \
  do { \
    int _I, _J; \
    for (_J=0, _NORM=0.f; _J<(_M)*(_FL); _J+=(_FL)) \
      for (_I=_J; _I<(_J+(_N)*(_FI)); _I+=(_FI)) \
        _NORM += *(_F+_I) * *(_F+_I); \
    _NORM = (float)sqrt(_NORM); \
  } while(0)
#define CONVERT_SCALE2D(_M, _N, _F, _FL, _FI, _P, _PL, _PI, _SCALE, _SHIFT) \
  do { \
    int _IF, _JF, _IP, _JP; \
    for (_JF=0, _JP=0; _JF<(_M)*(_FL); _JF+=(_FL), _JP+=(_PL)) \
      for (_IF=_JF, _IP=_JP; _IF<(_JF+(_N)*(_FI)); _IF+=(_FI), _IP+=(_PI)) \
        *(_P+_IP) = *(_F+_IF) * (_SCALE) + (_SHIFT); \
  } while(0)
#define SAFE_COPY(_N, _A, _AI, _B, _BI) \
  do { \
    int _I, _J; \
    if (_A>_B) \
      for (_I=0, _J=0; _I<(_N)*(_AI); _I+=(_AI), _J+=(_BI)) \
        *(_B+_J)=*(_A+_I); \
    else if (_A<_B) \
      for (_I=(_N-1)*(_AI), _J=(_N-1)*(_BI); _I>=0; _I-=(_AI), _J-=(_BI)) \
        *(_B+_J)=*(_A+_I); \
  } while(0)
#define SKIP_COPY_CAST(_H, _W, _A, _AL, _AI, _B, _BL, _BI, _CAST) \
  do { \
    int _A0, _A1, _B0, _B1; \
    for (_A0=0, _B0=0; _A0<((_H)*(_AL)); _A0+=(_AL), _B0+=_BL) \
      for (_A1=_A0, _B1=_B0; _A1<(_A0+(_W)*(_AI)); _A1+=(_AI), _B1+=(_BI)) \
        *(_B+_B1)=_CAST(*(_A+_A1)); \
  } while(0)
#define SKIP_BINTH(_H, _W, _A, _AL, _AI, _B, _BL, _BI, _DTH, _MAXVAL, _MINVAL) \
  do { \
    int _A0, _A1, _B0, _B1; \
    for (_A0=0, _B0=0; _A0<((_H)*(_AL)); _A0+=(_AL), _B0+=_BL) \
      for (_A1=_A0, _B1=_B0; _A1<(_A0+(_W)*(_AI)); _A1+=(_AI), _B1+=(_BI)) \
        *(_B+_B1)=(*(_A+_A1))>(_DTH) ? (_MAXVAL) : (_MINVAL); \
  } while(0)
#define SKIP_MAP(_H, _W, _A, _AL, _AI, _B, _BL, _BI, _M) \
  do { \
    int _A0, _A1, _B0, _B1; \
    for (_A0=0, _B0=0; _A0<((_H)*(_AL)); _A0+=(_AL), _B0+=_BL) \
      for (_A1=_A0, _B1=_B0; _A1<(_A0+(_W)*(_AI)); _A1+=(_AI), _B1+=(_BI)) \
        *(_B+_B1)=(_M)[(*(_A+_A1))]; \
  } while(0)
#define _NOP(x)  (x)
#define SKIP_COPY(_H, _W, _A, _AL, _AI, _B, _BL, _BI) \
  SKIP_COPY_CAST(_H, _W, _A, _AL, _AI, _B, _BL, _BI, _NOP)
#define SKIP_SWAP(_H, _W, _A, _AL, _AI, _B, _BL, _BI, t) \
  do { \
    int _A0, _A1, _B0, _B1; \
    for (_A0=0, _B0=0; _A0<((_H)*(_AL)); _A0+=(_AL), _B0+=_BL) \
      for (_A1=_A0, _B1=_B0; _A1<(_A0+(_W)*(_AI)); _A1+=(_AI), _B1+=(_BI)) \
        CC_SWAP(*(_B+_B1), *(_A+_A1), t); \
  } while(0)
#define CLONE(TYPE, _N, _F, _P) \
  do { \
    _P = (TYPE*)pmalloc(sizeof(TYPE)*(_N)); \
    memcpy(_P, _F, sizeof(TYPE)*(_N)); \
  } while(0)
#define CLONE2D(TYPE, _M, _N, _F, _FL, _P) \
  do { \
    _P = (TYPE*)pmalloc(sizeof(TYPE)*(_N)*(_M)); \
    MEMCPY2D(_M, _N, _F, _FL, _P, _N); \
  } while(0)
// ????????????????????
#define REPEAT1D(_NA, _A, _AI, _NB, _B, _BI, _OP) \
  do { \
    int _I, _IA, _IB; \
    for (_I=0; _I<(_NB); ++_I, _IA=(_I%(_NA))*_AI, _IB+=_BI) \
      *(_B+_IB) _OP *(_A+_IA); \
  } while(0)
#define REPEAT2D_OP(_Ma, _NA, _A, _AL, _AI, _MB, _NB, _B, _BL, _BI, _OP) \
  do { \
    int _I, _J, _IA, _IB, _JA, _JB; \
    for (_J=0; _I<(_MB); ++_J, _JA=(_J%(_MA))*_AL, _JB+=_BL) \
      for (_I=0, _IB=_JB; _I<(_NB); ++_I, _IA=(_I%(_NA))*_AI, _IB+=_BI) \
        *(_B+_IB) _OP *(_A+_JA+_IA); \
  } while(0)
// ????????
#define TRANSPOSE(_AH, _AW, _A, _AL, _B, _BL) SKIP_COPY(_AH, _AW, _A, _AL, 1, _B, 1, _BL)
#define SAFETRANSPOSE(_AH, _AW, _A, _AL, _B, _BL, TYPE) \
  do { \
    if ((_A)==(_B)) { \
      TYPE* _A2 = MALLOC(TYPE, (_AH)*(_AW)); \
      MEMCPY2D(_AH, _AW, _A, _AL, _A2, _AW); \
      SKIP_COPY(_AH, _AW, _A2, _AW, 1, _B, 1, _BL); \
      SAFEFREE(_A2); \
    } else { \
      SKIP_COPY(_AH, _AW, _A, _AL, 1, _B, 1, _BL); \
    } \
  } while(0)
// ?????????
// ???????A[M*N]??B[N*K]???????????????C[M*K]??
// A-?????M*N??????
// B-?????N*K??????
// C-?????M*K?????锟?????
// AL  A ???锟斤拷?
// BL  B ???锟斤拷?
// CL  C ???锟斤拷?
#define MAT_ABC_BASE(_M, _N, _K, _A, _AL, _AI, _B, _BL, _BI, _C, _CL, _CI) \
  do { \
    int _IM, _IN, _IK, _MA, _KA, _NB, _KB, _MC, _NC; \
    for (_IM=0, _MA=0, _MC=0; _IM<(_M); ++_IM, _MA+=(_AL), _MC+=(_CL)) \
      for (_IN=0, _NB=0, _NC=_MC; _IN<(_N); ++_IN, _NB+=(_BI), _NC+=(_CI)) \
        for (_IK=0, _KA=_MA, _KB=_NB; _IK<(_K); ++_IK, _KA+=(_AI), _KB+=(_BL)) \
          *(_C+_NC) += *(_A+_KA) * *(_B+_KB); \
  } while(0)
#define MAT_ABC(_M, _N, _K, _A, _AL, _B, _BL, _C, _CL)  MAT_ABC_BASE(_M, _N, _K, _A, _AL, 1, _B, _BL, 1, _C, _CL, 1)
#define MAT_ATBC(_M, _N, _K, _A, _AL, _B, _BL, _C, _CL) MAT_ABC_BASE(_M, _N, _K, _A, 1, _AL, _B, _BL, 1, _C, _CL, 1)
#define MAT_ABTC(_M, _N, _K, _A, _AL, _B, _BL, _C, _CL) MAT_ABC_BASE(_M, _N, _K, _A, _AL, 1, _B, 1, _BL, _C, _CL, 1)
#define DOT_PRODUCT1D(_N, _A, _AI, _B, _BI, _DOT) \
  do { \
    int _IA, _IB; \
    for (_IA=0, _IB=0; _IA<(_N)*(_AI); _IA+=(_AI), _IB+=(_BI)) \
      (_DOT) += *(_A+_IA) * *(_B+_IB); \
  } while(0)
#define DOT_PRODUCT2D(_H, _W, _A, _AL, _AI, _B, _BL, _BI, _DOT) \
  do { \
    int _IA, _IB, _IA1, _IB1; \
    for (_IA1=0, _IB1=0; _IA1<(_H)*(_AL); _IA1+=(_AL), _IB1+=(_BL)) \
      for (_IA=_IA1, _IB=_IB1; _IA<(_IA1+(_W)*(_AI)); _IA+=(_AI), _IB+=(_BI)) \
        (_DOT) += *(_A+_IA) * *(_B+_IB); \
  } while(0)
#define FIND(_F0, _L, _V, _F) \
  do { \
    for (_F=_F0; _F != _L; ++_F) { \
      if (*(_F) == (_V)) { break; } \
    } \
  } while(0)
#define FIND_IF(_F0, _L, _P, _F) \
  do { \
    for (_F=_F0; _F != _L; ++_F) { \
      if (_P(*(_F))) { break; } \
    } \
  } while(0)
#define COUNT(_N, _F, _V, _CNT) \
  do { \
    int _I=0; \
    for (_I=0,_CNT=0; _I!=(_N); ++_I) \
      if (_F[_I] == (_V)) \
        ++_CNT; \
  } while(0)
#define COUNT_IF(_F, _L, _P, _N) \
  do { \
    int i=0, n=(_L)-(_F); \
    for (_N = 0; i!=n; ++i) \
      if (_P(_F[i])) \
        ++_N; \
  } while(0)
#define _UNGUARDED_INSERT(_L, _V, _P) \
  do { \
    int i = 0, m = 0; \
    for (--m; _P((_V), *(_L+m)); i = m--) \
      *(_L+i) = *(_L+m); \
    *(_L+i) = (_V); \
  } while(0)
#define _INSERTION_SORT(_F0, _L0, _P, _Ty) \
  do { \
    _Ty *_F = _F0, *_L = _L0, *_M; \
    if (_F != _L) \
      for (_M = _F; ++_M != _L; ) { \
        _Ty _V = *_M; \
        if (!_P(_V, *_F)) { \
          _UNGUARDED_INSERT(_M, _V, _P); \
        } else { \
          COPY_BACKWARD(_M-_F, _M, _M + 1); \
          *_F = _V; \
        } \
      } \
  } while(0)
#define MIN_ELEMENT_IF(_N, _F, _X, _P) \
  do { \
    int _I; \
    if (_N) { \
      for (_X = 0, _I = 0; ++_I != _N; ) \
        if (_P(*(_F+_I), *(_F+_X))) \
          _X = _I; } \
  } while(0)
#define SKIP_MIN_ELEMENT_IF(_H, _W, _A, _AL, _AI, _X, _PR) \
  do { \
    int A0, A1; \
    if ((_H)&&(_W)) \
      for (_X = 0, A1=0; A1!=((_H)*(_AL)); A1+=(_AL)) \
        for (A0=A1; A0!=(A1+(_W)*(_AI)); A0+=(_AI)) \
          if (_PR(*(_A+A0), *(_A+_X))) \
            _X = A0; \
  } while(0)
#define SKIP_MINMAX_ELEMENT(_H, _W, _A, _AL, _AI, _MINIDX, _MAXIDX) \
  do { \
    int A0, A1; \
    if ((_H)&&(_W)) \
      for (_MINIDX = 0, _MAXIDX = 0, A1=0; A1!=((_H)*(_AL)); A1+=(_AL)) \
        for (A0=A1; A0!=(A1+(_W)*(_AI)); A0+=(_AI)) \
          if (*(_A+A0) < *(_A+_MINIDX)) \
            _MINIDX = A0; \
          else if (*(_A+A0) > *(_A+_MAXIDX)) \
            _MAXIDX = A0; \
  } while(0)
#define SKIP_MINMAX_MASK_ELEMENT(_H, _W, _A, _AL, _AI, _MASK, _ML, _MINIDX, _MAXIDX)\
  do { \
    unsigned char* _M=(unsigned char*)(_MASK); \
    int A0=0, A1=0, M0=0, M1=0; \
    if ((_H)&&(_W)) \
      for (_MINIDX=0, _MAXIDX=0; A1!=((_H)*(_AL)); A1+=(_AL), M1+=(_ML)) \
        for (A0=A1, M0=M1; M0!=(M1+(_W)); A0+=(_AI), ++M0) \
          if (!_M || *(_M+M0)) { \
            if (*(_A+A0) < *(_A+_MINIDX)) \
              _MINIDX = A0; \
            else if (*(_A+A0) > *(_A+_MAXIDX)) \
              _MAXIDX = A0; \
          } \
  } while(0)
#define MINMAX_ELEMENT(_N, _F, _MINIDX, _MAXIDX) \
  do { \
    int _I; \
    if (_N) \
      for (_MINIDX = 0, _MAXIDX = 0, _I = 0; ++_I != _N; ) \
        if (*(_F+_I) < *(_F+_MINIDX)) \
          _MINIDX = _I; \
        else if (*(_F+_I) > *(_F+_MAXIDX)) \
          _MAXIDX = _I; \
  } while(0)
#define _LESS(_A, _B)           ((_A) < (_B))
#define _GREATER(_A, _B)        ((_A) > (_B))
#define MIN_ELEMENT(_N, _F, _X) MIN_ELEMENT_IF(_N, _F, _X, _LESS)
#define MAX_ELEMENT(_N, _F, _X) MIN_ELEMENT_IF(_N, _F, _X, _GREATER)
#define SKIP_MIN_ELEMENT(_H, _W, _A, _AL, _AI, _X) SKIP_MIN_ELEMENT_IF(_H, _W, _A, _AL, _AI, _X, _LESS)
#define SKIP_MAX_ELEMENT(_H, _W, _A, _AL, _AI, _X) SKIP_MIN_ELEMENT_IF(_H, _W, _A, _AL, _AI, _X, _GREATER)
#define VEC_LINEARITY(_N, _F, _P, XMIN, XMAX) \
  do { \
    int _I, _AMIN=0, _AMAX=0; \
    MIN_ELEMENT(_N, _F, _AMIN); \
    MAX_ELEMENT(_N, _F, _AMAX); \
    if (_AMIN != _AMAX) { \
      double __A = 1.0f * XMAX / (*(_F+_AMAX) - *(_F+_AMIN)); \
      double __B = -__A * (*(_F+_AMIN)) + XMIN; \
      for (_I = 0; _I < _N; ++_I) { \
        *(_P + _I) = __A * (*(_F + _I)) + __B; \
      } \
    } \
  } while(0)
// ??a ?????????? q != r??
// a = q * q + r
#define ISQRT(_A, _Q, _R) \
  do { \
    for ((_Q) = (_A); (_Q) > ((_R) = (_A) / (_Q)); (_Q) = ((_Q) + (_R)) >> 1); \
    (_R) = (_A) - (_Q) * (_Q); \
  } while(0)
//
// out == 1
// out == 0 ??????? double threshold=0.3;
// ?????.w = MIN((r1?????-r2?????), (r2?????-r1?????))
// ?????? (area(r1) + area(r2))/2
#define IS_RECT_EQUAL_COVER_RETE12(r1, r2, THRESHOLD, OUT) \
  do { \
    int dx, dy; \
    if ((dx = (MIN(((r1).x+(r1).w-(r2).x), ((r2).x+(r2).w-(r1).x)))) < 0 \
    || (dy = (MIN(((r1).y+(r1).h-(r2).y), ((r2).y+(r2).h-(r1).y)))) < 0) {\
      (OUT) = 0; \
    } else { \
      (OUT) = (2*dx*dy) > (THRESHOLD * ((r1).w*(r1).h + (r2).w*(r2).h)); \
    } \
  } while(0)
// line[a0, a1] ???? line[b0, b1]
#define IS_LINE_INCLUDE(a0, a1, b0, b1)  ((a0)<=(b0) && (a1)>=(b1))
// r1 ???? r2
#define IS_RECT_INCLUDE(r1, r2) \
  ((IS_LINE_INCLUDE((r1).x, (r1).x+(r1).w, (r2).x, (r2).x+(r2).w) && \
      IS_LINE_INCLUDE((r1).y, (r1).y+(r1).h, (r2).y, (r2).y+(r2).h) ) \
      && ((r1).w < ((r2).w*2)))
#define IS_RECT_EQUAL_COVER_RETE(r1, r2, THRESHOLD, OUT) \
  do { \
    int dx, dy, covarea, avgarea; \
    (OUT) = IS_RECT_INCLUDE(r1, r2) || IS_RECT_INCLUDE(r2, r1); \
    if (OUT) { break; } \
    dx = MIN(((r1).x+(r1).w-(r2).x), ((r2).x+(r2).w-(r1).x)); \
    dy = MIN(((r1).y+(r1).h-(r2).y), ((r2).y+(r2).h-(r1).y)); \
    covarea=dx*dy; \
    if (dx < 0 || dy < 0 ) { (OUT) = 0; break; } \
    avgarea=((r1).w*(r1).h + (r2).w*(r2).h)/2; \
    (OUT) = (covarea >= (THRESHOLD)*avgarea); \
  } while(0)
// ??????锟斤拷?
#define RECT_PARTITION(II, _f, _l, FUNC, mincnt, RECT, ADD, DIV, out) \
  do { \
    II f = (_f); \
    II l = (_l); \
    II m; \
    RECT t; \
    int cnt, iseq=0; \
    for (; f!=l; ) { \
      t = *f; \
      for (m=l, cnt=1; f!=m--; ) { \
        if (f!=m) { \
          FUNC(*f, *m, iseq); \
          if (iseq) { \
            ADD(t, *m); \
            *m = *--l; \
            ++cnt; \
          } \
        } \
      } \
      if (cnt > mincnt) { \
        DIV(t, cnt); \
        *f++ = t; \
      } else { \
        *f = *--l; \
      } \
    } \
    out = f; \
  } while(0)
// ?????
#define INTEGRAL_BASE(B0_, bl1, _bl, _bi)    *(B0_+bl1) = *(B0_+_bl) + *(B0_+_bi) - *(B0_)
#define INTEGRAL_GEN(A0, _B0, bl1, _bl, _bi)  INTEGRAL_BASE(_B0, bl1, _bl, _bi) + (*(A0))
#define INTEGRAL_GQ(A0, _B0, bl1, _bl, _bi)   INTEGRAL_BASE(_B0, bl1, _bl, _bi) + (*(A0)) * (*(A0))
#define INTEGRAL_SQ(A0_, _B0, bl1, _bl, _bi) \
  do { \
    (INTEGRAL_BASE(_B0, bl1, _bl, _bi) + (*(A0_))); \
    (INTEGRAL_BASE(_B0+1, bl1, _bl, _bi) + (*(A0_)) * (*(A0_))); \
  } while(0)
#define SKIP_INTEGRAL_IMP(h0, w0, _A, al, ai, _B, _bl, _bi, INTEGRAL_OPT) \
  do { \
    int a0, a1, b0, b1, _h=(h0)*(al), _w=(w0)*(ai), bl1 = (_bl)+(_bi); \
    for (a1=0, b1=0; a1<_h; a1+=al, b1+=_bl) { \
      for (a0=a1, b0=b1; a0<(_w+a1); a0+=ai, b0+=_bi) { \
        INTEGRAL_OPT((_A+a0), (_B+b0), bl1, _bl, _bi); \
      } \
    } \
  } while(0)
// ????????
#define SKIP_INTEGRAL(h0, w0, _A, al, ai, _B, bl, bi) SKIP_INTEGRAL_IMP(h0, w0, _A, al, ai, _B, bl, bi, INTEGRAL_GEN)
#define SKIP_INTEGRAL_GQ(h0, w0, _A, al, ai, _B, bl, bi) SKIP_INTEGRAL_IMP(h0, w0, _A, al, ai, _B, bl, bi, INTEGRAL_GQ)
// ??????????????????
#define SKIP_INTEGRAL_SQ(h0, w0, _A, al, ai, _B, bl, bi) SKIP_INTEGRAL_IMP(h0, w0, _A, al, ai, _B, bl, bi, INTEGRAL_SQ)
#define RECT_VAL2(_A, _a, _b)          (*(_A) - *(_A+(_a)) - *(_A+(_b)) + *(_A+(_a)+(_b)))
// ??????
#define SKIP_INTEGRAL_VAR(_A, _AL, _AI, _B, _BL, _BI, _IV, _VAR) { \
    _VAR = RECT_VAL2(_A, (_AL), (_AI))*_IV; \
    _VAR = RECT_VAL2(_B, (_BL), (_BI))*_IV - _VAR*_VAR; \
  }
#undef _AND
#define _AND(_X, _Y)  ((_X) &&= (_Y))
#define _OR(_X, _Y)  ((_X) ||= (_Y))
#define _EVA(_X, _Y)  ((_X)  = (_Y))
#define _ADD(_X, _Y)  ((_X) += (_Y))
#define _SUB(_X, _Y)  ((_X) -= (_Y))
#define _MUL(_X, _Y)  ((_X) *= (_Y))
#define _DIV(_X, _Y)  ((_X) /= (_Y))
#define _PRO(_X, _Y)  (_Y(_X))
#define _PR2(_X, _Y)  (_X = _Y(_X))
#define VEC_OPT_XV(_N, _F, _OUT, _OPT) \
  do { \
    int _I; \
    for (_I=0; _I<_N; ++_I) { \
      _OPT(_OUT, *(_F + _I)); \
    } \
  } while(0)
// X[I]+=Y[I]
#define VEC_OPT_VV(_N, _X, _Y, OP) do { \
    int i=0; \
    for (; i<(_N)-3; i+=4) { \
      OP(*(_X+i  ), *(_Y+i  )); \
      OP(*(_X+i+1), *(_Y+i+1)); \
      OP(*(_X+i+2), *(_Y+i+2)); \
      OP(*(_X+i+3), *(_Y+i+3)); \
    } \
    for (; i<(_N); ++i) { \
      OP(*(_X+i  ), *(_Y+i  )); \
    } \
  } while(0)
// X[I]+=V
#define VEC_OPT_VX(_N, _X, _V, OP) do { \
    int i=0; \
    for (; i<(_N)-3; i+=4) { \
      OP(*(_X+i  ), _V); \
      OP(*(_X+i+1), _V); \
      OP(*(_X+i+2), _V); \
      OP(*(_X+i+3), _V); \
    } \
    for (; i<(_N); ++i) { \
      OP(*(_X+i  ), _V); \
    } \
  } while(0)
#define VEC_OPT_VP(_N, _X, _P) VEC_OPT_VX(_N, _X, _P, _PR2)
// X[I]+=Y[I]
#define SKIP_OPT_VV1(_N, _X, ai, _Y, bi, OP) do { \
    int a0, b0; \
    for (a0=0, b0=0; a0<(_N)*(ai); a0+=(ai), b0+=(bi)) { \
      OP(*((_X)+a0), *((_Y)+b0)); \
    } \
  } while(0)
// X[I]+=V
#define SKIP_OPT_VX1(_N, _X, ai, _V, OP) do { \
    int i=0; \
    for (; i<(_N)-3; i+=4) { \
      OP(*(_X+(i  )*ai), (_V)); \
      OP(*(_X+(i+1)*ai), (_V)); \
      OP(*(_X+(i+2)*ai), (_V)); \
      OP(*(_X+(i+3)*ai), (_V)); \
    } \
    for (; i<(_N); ++i) { \
      OP(*(_X+(i  )*ai), (_V)); \
    } \
  } while(0)
#define SKIP_OPT_VX2(_H, _W, _A, al, ai, _V, OP) do { \
    int _a, _h=_H*al; \
    for (_a=0; _a!=_h; _a+=(al)) { \
      SKIP_OPT_VX1(_W, (_A+_a), ai, _V, OP); \
    } \
  } while(0)
#define SKIP_OPT_VV2(_H, _W, _A, al, ai, _B, bl, bi, OP) do { \
    int _a, _b, _h=_H*al; \
    for (_a=0, _b=0; _a!=_h; _a+=(al), _b+=(bl)) { \
      SKIP_OPT_VV1(_W, ((_A)+_a), ai, ((_B)+_b), bi, OP); \
    } \
  } while(0)
#define VEC_EVA_VV(_N, _X, _Y) VEC_OPT_VV(_N, _X, _Y, _EVA)
#define VEC_ADD_VV(_N, _X, _Y) VEC_OPT_VV(_N, _X, _Y, _ADD)
#define VEC_SUB_VV(_N, _X, _Y) VEC_OPT_VV(_N, _X, _Y, _SUB)
#define VEC_MUL_VV(_N, _X, _Y) VEC_OPT_VV(_N, _X, _Y, _MUL)
#define VEC_DIV_VV(_N, _X, _Y) VEC_OPT_VV(_N, _X, _Y, _DIV)
#define VEC_EVA_VX(_N, _X, _V) VEC_OPT_VX(_N, _X, _V, _EVA)
#define VEC_ADD_VX(_N, _X, _V) VEC_OPT_VX(_N, _X, _V, _ADD)
#define VEC_SUB_VX(_N, _X, _V) VEC_OPT_VX(_N, _X, _V, _SUB)
#define VEC_MUL_VX(_N, _X, _V) VEC_OPT_VX(_N, _X, _V, _MUL)
#define VEC_DIV_VX(_N, _X, _V) VEC_OPT_VX(_N, _X, _V, _DIV)
#define SKIP_EVA_VX1(_N, _X, ai, _V) SKIP_OPT_VX1(_N, _X, ai, _V, _EVA)
#define SKIP_ADD_VX1(_N, _X, ai, _V) SKIP_OPT_VX1(_N, _X, ai, _V, _ADD)
#define SKIP_SUB_VX1(_N, _X, ai, _V) SKIP_OPT_VX1(_N, _X, ai, _V, _SUB)
#define SKIP_MUL_VX1(_N, _X, ai, _V) SKIP_OPT_VX1(_N, _X, ai, _V, _MUL)
#define SKIP_DIV_VX1(_N, _X, ai, _V) SKIP_OPT_VX1(_N, _X, ai, _V, _DIV)
#define SKIP_EVA_VV1(_N, _X, ai, _Y, bi) SKIP_OPT_VV1(_N, _X, ai, _Y, bi, _EVA)
#define SKIP_ADD_VV1(_N, _X, ai, _Y, bi) SKIP_OPT_VV1(_N, _X, ai, _Y, bi, _ADD)
#define SKIP_SUB_VV1(_N, _X, ai, _Y, bi) SKIP_OPT_VV1(_N, _X, ai, _Y, bi, _SUB)
#define SKIP_MUL_VV1(_N, _X, ai, _Y, bi) SKIP_OPT_VV1(_N, _X, ai, _Y, bi, _MUL)
#define SKIP_DIV_VV1(_N, _X, ai, _Y, bi) SKIP_OPT_VV1(_N, _X, ai, _Y, bi, _DIV)
#define SKIP_EVA_VX2(_H, _W, _A, al, ai, _V) SKIP_OPT_VX2(_H, _W, _A, al, ai, _V, _EVA)
#define SKIP_ADD_VX2(_H, _W, _A, al, ai, _V) SKIP_OPT_VX2(_H, _W, _A, al, ai, _V, _ADD)
#define SKIP_SUB_VX2(_H, _W, _A, al, ai, _V) SKIP_OPT_VX2(_H, _W, _A, al, ai, _V, _SUB)
#define SKIP_MUL_VX2(_H, _W, _A, al, ai, _V) SKIP_OPT_VX2(_H, _W, _A, al, ai, _V, _MUL)
#define SKIP_DIV_VX2(_H, _W, _A, al, ai, _V) SKIP_OPT_VX2(_H, _W, _A, al, ai, _V, _DIV)
#define SKIP_EVA_VV2(_H, _W, _A, al, ai, _B, bl, bi) SKIP_OPT_VV2(_H, _W, _A, al, ai, _B, bl, bi, _EVA)
#define SKIP_ADD_VV2(_H, _W, _A, al, ai, _B, bl, bi) SKIP_OPT_VV2(_H, _W, _A, al, ai, _B, bl, bi, _ADD)
#define SKIP_SUB_VV2(_H, _W, _A, al, ai, _B, bl, bi) SKIP_OPT_VV2(_H, _W, _A, al, ai, _B, bl, bi, _SUB)
#define SKIP_MUL_VV2(_H, _W, _A, al, ai, _B, bl, bi) SKIP_OPT_VV2(_H, _W, _A, al, ai, _B, bl, bi, _MUL)
#define SKIP_DIV_VV2(_H, _W, _A, al, ai, _B, bl, bi) SKIP_OPT_VV2(_H, _W, _A, al, ai, _B, bl, bi, _DIV)
#define VEC_FILL(_N, _X, _V) VEC_EVA_VX(_N, _X, _V)
#define VEC_COPY(_N, _X, _Y) VEC_EVA_VV(_N, _Y, _X)
#ifndef SKIP_FILL
//#define SKIP_FILL(_H, _W, _A, al, ai, _V)  SKIP_EVA_VX2(_H, _W, _A, al, ai, _V)
#endif
#ifndef SKIP_COPY
#define SKIP_COPY(_H, _W, _A, al, ai, _B, bl, bi)  SKIP_EVA_VV2(_H, _W, _B, bl, bi, _A, al, ai)
#endif
#define FOREACH(_N, _F, _Pr)  VEC_OPT_VX(_N, _F, _Pr, _PRO)
#define VEC_FILL_COLOR(_N, _A, ai, _V) \
  do { \
    int k; \
    for (k=0; k<ai; ++k) { \
      SKIP_EVA_VX1(_N, (_A+k), ai, *(_V+k)); \
    } \
  } while(0)
#define SKIP_FILL_COLOR(_H, _W, _A, al, ai, _V) \
  do { \
    int k; \
    for (k=0; k<ai; ++k) { \
      SKIP_FILL(_H, _W, _A+k, al, ai, *(_V+k)); \
    } \
  } while(0)
#define VEC_SUM_XV(_N, _F, _OUT) VEC_OPT_XV(_N, _F, _OUT, _ADD)
// ?????
#define VEC_NOMELIZE(_N, _F) \
  do { \
    float _OUT = 0.0f; \
    VEC_SUM_XV(_N, _F, _OUT); \
    VEC_DIV_VX(_N, _F, _OUT); \
  } while(0)
// ???????
#define FLIP1D(_N, _A, _AI, t) do {int _I=0; for (; _I<(_N)/2; _I+=_AI) CC_SWAP(*(_A+_I), *(_A+(_N)-(_AI)-_I), t); } while(0)
// 
#define FLIP2D(_M, _N, _A, _AL, _AI, t) \
  do { \
    int _LA, _IA; \
    for (_LA=0; _LA<((_M)/2); ++_LA) \
      for (_IA=0; _IA<(_N); ++_IA) \
        CC_SWAP(*(_A+_LA*(_AL)+_IA*(_AI)), *(_A+(_M-_LA-1)*(_AL)+_IA*(_AI)), t);\
  } while(0)
// 左右
#define FLOP2D(_M, _N, _A, _AL, _AI, t) \
  do { \
    int _LA, _IA; \
    for (_LA=0; _LA<(_M); ++_LA) \
      for (_IA=0; _IA<(_N)/2; ++_IA) \
        CC_SWAP(*(_A+_LA*(_AL)+_IA*(_AI)), *(_A+(_LA)*(_AL)+_AI*(_N-_IA-1)), t);\
  } while(0)
#define VSPRINTF(STR, FMT, ARGLIST) \
  do { \
    myva_list ARGLIST; \
    myva_start(ARGLIST, FMT); \
    vsprintf(STR, FMT, ARGLIST); \
    myva_end(ARGLIST); \
  } while(0)
#define VSNPRINTF(STR, _N, FMT, ARGLIST) \
  do { \
    myva_list ARGLIST; \
    myva_start(ARGLIST, FMT); \
    _vsnprintf(STR, _N, FMT, ARGLIST); \
    myva_end(ARGLIST); \
  } while(0)
#define VPRINTF(FMT, ARGLIST) \
  do { \
    myva_list ARGLIST; \
    myva_start(ARGLIST, FMT); \
    vprintf(FMT, ARGLIST); \
    myva_end(ARGLIST); \
  } while(0)
// ???????????????F????
#define BSEARCH(_N, _F, _X, _I) \
  do { \
    int _bs_l=0,_bs_r=_N; \
    while(_bs_l<_bs_r){ \
      int _bs_m = (_bs_l+_bs_r)>>1; \
      if(*(_F+_bs_m)<_X) _bs_l = _bs_m+1; \
      else if(*(_F+_bs_m)>_X) _bs_r = _bs_m; \
      else { _I = _bs_m; break; } \
    } \
    _I = _bs_l; \
  } while(0)
/////////////////////////////////////////////////////////////////////////////////////////
#define _PUSH_HEAP(_F, _H0, _J0, _V, _P)\
  {int _H1=_H0, _J1=_J0, _I = (_H0 - 1) / 2; \
    for (; _J1 < _H1 && _P(*(_F + _I), _V); _I = (_H1 - 1) / 2) {\
      *(_F + _H1) = *(_F + _I), _H1 = _I; }\
    *(_F + _H1) = _V; }
#define _ADJUST_HEAP(_F, _H0, _N0, _V, _P) \
  {int _H2=_H0, _N2=_N0, _J = _H0; int _K = 2 * _H2 + 2; \
    for (; _K < _N2; _K = 2 * _K + 2) {\
    if (_P(*(_F + _K), *(_F + (_K - 1)))) --_K; \
      *(_F + _H2) = *(_F + _K), _H2 = _K; } \
    if (_K == _N2) \
      *(_F + _H2) = *(_F + (_K - 1)), _H2 = _K - 1; \
    _PUSH_HEAP(_F, _H2, _J, _V, _P); }
#define PUSH_HEAP(_N, _F, _P, _V) { _V = (*(_F + _N - 1)); _PUSH_HEAP(_F, (_N - 1), (0), _V, _P); }
#define POP_HEAP(_N, _F, _P, _V)  {_V = (*(_F + _N - 1)); (*(_F + _N - 1)) = *_F; _ADJUST_HEAP(_F, (0), (_N - 1), _V, _P); }
#define MAKE_HEAP(_N, _F, _P, _V) if (2 <= _N) {int _H = _N / 2; for (; 0 < _H; ) { --_H; _V = (*(_F + _H)); _ADJUST_HEAP(_F, _H, _N, _V, _P); } }
// TEMPLATE FUNCTION sort_heap WITH PRED
#define SORT_HEAP(_N, _F, _P, _T) { _T _V; int _N0=_N; for (; 1 < _N0; --_N0) POP_HEAP(_N0, _F, _P, _V); }
#define _POP_HEAP(_N, _F, _X, _V, _P)	{_V = *_X; *_X = *_F;	_ADJUST_HEAP(_F, (0), _N, _V, _P); }
#define PARTIAL_SORT(_N, _F, _M, _P, _T) {int _I; _T _V; MAKE_HEAP(_M, _F, _P, _V); \
for (_I = _M; _I < _N; ++_I) \
		if (_P((_F)[_I], *(_F))) _POP_HEAP(_M, _F, (_F+_I), _V, _P); \
	SORT_HEAP(_M, _F, _P, _T); }
/////////////////////////////////////////////////////////////////////////////////////////
#define RANDINT()  (unsigned int)((rand()<<16) | (rand()&0xffff))
// ???????
#define RANDOM_SHUFFLE(_N, _F, _TYPE) do { _TYPE _TMP;\
    const int _RBITS = 15; \
    const int _RMAX = (1U << _RBITS) - 1; \
    unsigned int _D, _X = 0, _Y = 0; \
    for (_D = 1; ++_X != (unsigned int)_N; ++_D) { \
      unsigned long _Rm = _RMAX, _Rn = rand() & _RMAX; \
      for (; _Rm < _D && _Rm != ~0UL; _Rm = _Rm << _RBITS | _RMAX) \
        _Rn = _Rn << _RBITS | _RMAX; \
      _Y = (_Rn % _D); \
      CC_SWAP(*(_F + _X), *(_F + _Y), _TMP); } \
  } while(0)

#ifndef _SHIFT
#define _SHIFT                 (13)
#define _SHIFT1                (1<<_SHIFT)
#define _SHIFT_MARK            (_SHIFT1 - 1)
#define _SHIFT_MUL(x, y)       (((x)*(y))>>_SHIFT)
#define _SHIFT_DIV(x, y)       (((int)(x)<<_SHIFT)/(y))
#define _SHIFT_TOFLOAT(x)      (((double)(x))/_SHIFT1)
#define _SHIFT_TOINT(x)        (int)((x)*_SHIFT1)
#define _SHIFT_TOINT2(x, _S)   (int)((x)*(1<<(_S)))
#define _HALFSHIFT1            (1<<12)
#endif
// RGB2GRAY
#ifndef GREYSCALIZE
#define _CR                  (299*(1 << _SHIFT)/1000)
#define _CG                  (587*(1 << _SHIFT)/1000)
#define _CB                  (_SHIFT1 - _CR - _CG)
#define GREYSCALIZE(R, G, B)   ((_CR*(R) + _CG*(G) + _CB*(B) )>>_SHIFT)
#endif // GREYSCALIZE
#define BGR2GRAY(bgr, gry)   (*(gry) = (unsigned char)GREYSCALIZE(*(bgr+2), *(bgr+1), *(bgr)))
#define RGB2GRAY(bgr, gry)   (*(gry) = (unsigned char)GREYSCALIZE(*(bgr), *(bgr+1), *(bgr+2)))
#define BGRA2GRAY(bgr, gry)  BGR2GRAY(bgr, gry)
#define BGRA2BGR(bgra, bgr)  do { *(bgr) = *(bgra); *(bgr+1) = *(bgra+1); *(bgr+2) = *(bgra+2); } while(0)
#define RGBA2BGR(bgra, bgr)  do { *(bgr) = *(bgra+2); *(bgr+1) = *(bgra+1); *(bgr+2) = *(bgra); } while(0)
#define RGBA2RGB(bgra, bgr)  do { *(bgr) = *(bgra); *(bgr+1) = *(bgra+1); *(bgr+2) = *(bgra+2); } while(0)
#define RGB2BGR(rgb, bgr)    do { *(bgr) = *(rgb+2); *(bgr+1) = *(rgb+1); *(bgr+2) = *(rgb); } while(0)
#define BGR2RGB(bgr, rgb)    RGB2BGR(bgr, rgb)
#define GRAY2RGBx(gry, rgb)  do { *(rgb) = *gry; *(rgb+1) = *gry; *(rgb+2) = *gry; } while(0)
#define GRAYRANGE(P)       ((P > 255) ? 255 : (P < 0) ? 0 : P)
#define PIXELRANGE(P)      ((P > 255) ? 255 : (P < 0) ? 0 : P)
#define DOT_MULT3_3(X, Y, Z, R, G, B)  ((X)*(R)+(Y)*(G)+(Z)*(B))
#define GetY_YCbCr(R, G, B)       (DOT_MULT3_3( 0.2990, 0.5870, 0.1140,R, G, B)    )
#define GetCb_YCbCr(R, G, B)      (DOT_MULT3_3(-0.1687,-0.3313, 0.5000,R, G, B)+128)
#define GetCr_YCbCr(R, G, B)      (DOT_MULT3_3( 0.5000,-0.4187,-0.0813,R, G, B)+128)
#define GetY_RGB(R, G, B)      ((19595*R + 38470*G + 7471 *B) >> 16)
#define GetI_RGB(R, G, B)      ((39059*R - 18021*G - 21036*B) >> 16)
#define GetQ_RGB(R, G, B)      ((13894*R - 34274*G - 20381*B) >> 16)
#define BICNT_GRAY  8
#define BICNT_RGB   24
#define BICNT_RGBA  32
#define BGR565(r,g,b)  ( (((b)>>3)<<11) | (((g)>>2)<<5) | ((r)>>3) )
#define BGR555(r,g,b)  ( (((b)>>3)<<10) | (((g)>>3)<<5) | ((r)>>3) )
#define BGR5552GRAY(bgr555, gry) \
  do { \
    *gry = (unsigned char)GREYSCALIZE( \
        ((((ushort*)bgr555)[0] >> 7) & 0xf8), \
        ((((ushort*)bgr555)[0] >> 2) & 0xf8), \
        ((((ushort*)bgr555)[0] << 3) & 0xf8)); \
  } while(0)
#define BGR5652GRAY(bgr565, gry) \
  do { \
    *gry = (unsigned char)GREYSCALIZE( \
        ((((ushort*)bgr565)[0] >> 8) & 0xf8), \
        ((((ushort*)bgr565)[0] >> 3) & 0xfc), \
        ((((ushort*)bgr565)[0] << 3) & 0xf8) ); \
  } while(0)
#define BGR5552BGR(bgr555, bgr) \
  do { \
    int t0 = (*((ushort*)bgr555) << 3) & 0xf8; \
    int t1 = (*((ushort*)bgr555) >> 2) & 0xf8; \
    int t2 = (*((ushort*)bgr555) >> 7) & 0xf8; \
    *(bgr+0) = (unsigned char)t0; \
    *(bgr+1) = (unsigned char)t1; \
    *(bgr+2) = (unsigned char)t2; \
  } while(0)
#define BGR5652BGR(bgr565, bgr) \
  do { \
    int t0 = (*((ushort*)bgr565) << 3) & 0xf8; \
    int t1 = (*((ushort*)bgr565) >> 3) & 0xfc; \
    int t2 = (*((ushort*)bgr565) >> 8) & 0xf8; \
    *(bgr+0) = (unsigned char)t0; \
    *(bgr+1) = (unsigned char)t1; \
    *(bgr+2) = (unsigned char)t2; \
  } while(0)
// 4 => 3
#define CMYK2BGR(cmyk, bgr) \
  do { \
    int c = *(cmyk), m = *(cmyk+1), y = *(cmyk+2), k = *(cmyk+3); \
    c = k - ((255 - c)*k>>8); \
    m = k - ((255 - m)*k>>8); \
    y = k - ((255 - y)*k>>8); \
    *(bgr+2) = (unsigned char)c; \
    *(bgr+1) = (unsigned char)m; \
    *(bgr+0) = (unsigned char)y; \
  } while(0)
// <4, 1>
#define CMYK2GRAY(cmyk, gry) \
  do { \
    int c = *(cmyk), m = *(cmyk+1), y = *(cmyk+2), k = *(cmyk+3); \
    c = k - ((255 - c)*k>>8); \
    m = k - ((255 - m)*k>>8); \
    y = k - ((255 - y)*k>>8); \
    *(gry) = (unsigned char)GREYSCALIZE(c, m, y); \
  } while(0)
// ??????
#define RGB2YIQ_SKIN_T(in, out) \
  do { \
    int Y, I, Q; \
    Y = GetY_RGB((in+2), *(in+1), *(in)); \
    I = GetI_RGB((in+2), *(in+1), *(in)); \
    Q = GetQ_RGB((in+2), *(in+1), *(in)); \
    *(out) = (((Y < 235) && (I > 12) && (pow(I / 58, 2) + pow((Y - 142) / 104, 2)) < 1.) ? (0) : (255));\
  } while(0)
// ????????
#define SKINLIKEHOOD_T(in, out) \
  do { \
    static const double B_Mean = 117.4361, R_Mean = 156.5599; \
    static const double Brcov[4]  = { 160.1301, 12.1430, 12.1430, 299.4574 }; \
    static const double bb        = 2 * (Brcov1 * Brcov2 - Brcov0 * Brcov3); \
    double x1, x2, t; \
    x1 = GetCb_YCbCr(*(in+2), *(in+1), *(in)) - B_Mean; \
    x2 = GetCr_YCbCr(*(in+2), *(in+1), *(in)) - R_Mean; \
    t = x1 * (x1 * Brcov3 - x2 * Brcov2) + x2 * (-x1 * Brcov1 + x2 * Brcov0); \
    t = exp(t / bb) * 255; \
    *out = t;/*>80 ? 255 : 0;*/ \
  } while(0)
#define UNDEFINEDCOLOR 0
/*
  r,g,b values are from 0 to 1
  h = [0,360], s = [0,1], v = [0,1]
  if s == 0, then h = -1 (undefined)
*/
#define RGB2HSV(in, out) \
  do { \
    float R, G, B; \
    float H, S, V; \
    float min, max, delta, tmp; \
    B = *(in  ); \
    G = *(in+1); \
    R = *(in+2); \
    tmp = MIN(R, G); \
    min = MIN(tmp, B); \
    tmp = MAX(R, G); \
    max = MAX(tmp, B); \
    V = max; \
    delta = max - min; \
    if( max != 0 ) { \
      S = delta / max; \
    } else { \
      /* r = g = b = 0 s = 0, v is undefined */ \
      S = 0; \
      H = UNDEFINEDCOLOR; \
      break; \
    } \
    if( R == max ) \
      H = ( G - B ) / delta; /* between yellow & magenta */ \
    else if( G == max ) \
      H = 2 + ( B - R ) / delta; /* between cyan & yellow */ \
    else \
      H = 4 + ( R - G ) / delta; /* between magenta & cyan */ \
    \
    H *= 60; /* degrees */ \
    if( H < 0 ) { \
      H += 360; \
    } \
    *(out  ) = (unsigned char)H; \
    *(out+1) = (unsigned char)(S*255); \
    *(out+2) = (unsigned char)V; \
  } while(0)
#if 0
// HSV??????????RGB?????????
void Hsv2Rgb(float H, float S, float V, float& R, float& G, float& B)
{
  int i;
  float f, p, q, t;
  if (S == 0) {
    // achromatic (grey)
    R = G = B = V;
    return ;
  }
  H /= 60; // sector 0 to 5
  i = floor(H);
  f = H - i; // factorial part of h
  p = V * (1 - S);
  q = V * (1 - S * f);
  t = V * (1 - S * (1 - f));
  switch (i) {
  case 0:
    R = V;
    G = t;
    B = p;
    break;
  case 1:
    R = q;
    G = V;
    B = p;
    break;
  case 2:
    R = p;
    G = V;
    B = t;
    break;
  case 3:
    R = p;
    G = q;
    B = V;
    break;
  case 4:
    R = t;
    G = p;
    B = V;
    break;
  default:    // case 5:
    R = V;
    G = p;
    B = q;
    break;
  }
}
#endif
#define IMTRANS1D(aw, A, ai, B, bi, FUNC) \
  do { \
    int j, As=0, Bs=0; \
    for (j=0; j<aw; ++j, As+=ai, Bs+=bi) { \
      FUNC((A+As), (B+Bs)); \
    } \
  } while(0)
#define IMTRANS(ah, aw, A, al, ai, B, bl, bi, FUNC) \
  do { \
    int i, j; \
    int As=0, Bs=0; \
    int _al=(al)-aw*(ai); \
    int _bl=(bl)-aw*(bi); \
    for (i=0; i<ah; ++i, As+=_al, Bs+=_bl) { \
      for (j=0; j<aw; ++j, As+=ai, Bs+=bi) { \
        FUNC((A+As), (B+Bs)); \
      } \
    } \
  } while(0)
#define IM2IM(h, w, A, al, ai, B, bl, bi) \
  do { \
    if (ai==bi) { \
      SKIP_COPY(h, w*ai, A, al, 1, B, bl, 1); \
    } else { \
      int t = ai*10+bi; \
      switch (t) { \
      case 31: { IMTRANS(h, w, A, al, 3, B, bl, 1,  BGR2GRAY ); break; } \
      case 13: { IMTRANS(h, w, A, al, ai, B, bl, 3, GRAY2RGBx); break; } \
      case 23: { IMTRANS(h, w, A, al, ai, B, bl, 3, GRAY2RGBx); break; } \
      case 14: { IMTRANS(h, w, A, al, ai, B, bl, 4, GRAY2RGBx); break; } \
      case 43: { IMTRANS(h, w, A, al, ai, B, bl, 3, RGBA2RGB); break; } \
      } /* switch */ \
    } \
  } while(0)
#ifndef MAX
#define MAX(a, b) ( (a) > (b) ? (a) : (b) )
#define MIN(a, b) ( (a) > (b) ? (b) : (a) )
#endif
#ifndef FLOOR
#define FLOOR(X)  ((int)floor(X))
#endif
#ifndef CEIL
#define CEIL(X)  ((int)ceil(X))
#endif
#define fix(x,n)      (int)((x)*(1 << (n)) + 0.5)
#define cscGr_32f  0.299f
#define cscGg_32f  0.587f
#define cscGb_32f  0.114f
/* BGR/RGB -> Gray */
#define csc_shift  14
#define cscGr  fix(cscGr_32f,csc_shift)
#define cscGg  fix(cscGg_32f,csc_shift)
#define cscGb  /*fix(cscGb_32f,csc_shift)*/ ((1 << csc_shift) - cscGr - cscGg)
/* BGR/RGB -> YCrCb */
#define yuvYr_32f cscGr_32f
#define yuvYg_32f cscGg_32f
#define yuvYb_32f cscGb_32f
#define yuvCr_32f 0.713f
#define yuvCb_32f 0.564f
#define yuv_shift 14
#define yuvYr  fix(yuvYr_32f,yuv_shift)
#define yuvYg  fix(yuvYg_32f,yuv_shift)
#define yuvYb  fix(yuvYb_32f,yuv_shift)
#define yuvCr  fix(yuvCr_32f,yuv_shift)
#define yuvCb  fix(yuvCb_32f,yuv_shift)
#define yuv_descale(x)  CC_DESCALE((x), yuv_shift)
#define yuv_prescale(x) ((x) << yuv_shift)
#define yuvRCr_32f   1.403f
#define yuvGCr_32f   (-0.714f)
#define yuvGCb_32f   (-0.344f)
#define yuvBCb_32f   1.773f
#define yuvRCr   fix(yuvRCr_32f,yuv_shift)
#define yuvGCr   (-fix(-yuvGCr_32f,yuv_shift))
#define yuvGCb   (-fix(-yuvGCb_32f,yuv_shift))
#define yuvBCb   fix(yuvBCb_32f,yuv_shift)
//#define scale_macro(a)      (a)
//#define cast_macro(a)       (a)
#define CC_NOP(a)           (a)
#define DESCALE(x,n)  (((x) + (1 << ((n)-1))) >> (n))
#define descale DESCALE
//   Various 3/4-channel to 3/4-channel RGB transformations                 *
#define BGRx2BGR(src, dst, blue_idx) \
  do { \
    int b = (src)[blue_idx], g = (src)[1], r = (src)[(blue_idx)^2]; \
    (dst)[0] = b; \
    (dst)[1] = g; \
    (dst)[2] = r; \
  } while(0)
#define BGR2BGRX(src, dst, blue_idx) \
  do { \
    dst[i] = src[blue_idx]; \
    dst[i+1] = src[1]; \
    dst[i+2] = src[(blue_idx)^2]; \
    dst[i+3] = 0; \
  } while(0)
#define BGRA2RGBA(src, dst) \
  do { \
    dst[i] = src[2]; \
    dst[i+1] = src[1]; \
    dst[i+2] = src[0]; \
    dst[i+3] = src[3]; \
  } while(0)
#define COPY_MEMBER1(_S, _D, M1)                                   ((_D)->M1=(_S)->M1)
#define COPY_MEMBER2(_S, _D, M1, M2)                               (COPY_MEMBER1(_S, _D, M1), COPY_MEMBER1(_S, _D, M2))
#define COPY_MEMBER3(_S, _D, M1, M2, M3)                           (COPY_MEMBER1(_S, _D, M1), COPY_MEMBER2(_S, _D, M2, M3))
#define COPY_MEMBER4(_S, _D, M1, M2, M3, M4)                       (COPY_MEMBER1(_S, _D, M1), COPY_MEMBER3(_S, _D, M2, M3, M4))
#define COPY_MEMBER5(_S, _D, M1, M2, M3, M4, M5)                   (COPY_MEMBER1(_S, _D, M1), COPY_MEMBER4(_S, _D, M2, M3, M4, M5))
#define COPY_MEMBER6(_S, _D, M1, M2, M3, M4, M5, M6)               (COPY_MEMBER1(_S, _D, M1), COPY_MEMBER5(_S, _D, M2, M3, M4, M5, M6))
#define COPY_MEMBER7(_S, _D, M1, M2, M3, M4, M5, M6, M7)           (COPY_MEMBER1(_S, _D, M1), COPY_MEMBER6(_S, _D, M2, M3, M4, M5, M6, M7))
#define COPY_MEMBER8(_S, _D, M1, M2, M3, M4, M5, M6, M7, M8)       (COPY_MEMBER1(_S, _D, M1), COPY_MEMBER7(_S, _D, M2, M3, M4, M5, M6, M7, M8))
#define COPY_MEMBER9(_S, _D, M1, M2, M3, M4, M5, M6, M7, M8, M9)   (COPY_MEMBER1(_S, _D, M1), COPY_MEMBER8(_S, _D, M2, M3, M4, M5, M6, M7, M8, M9))
#define alpha_composite(composite, fg, alpha, bg) { \
    unsigned short temp = ((unsigned short)(fg)*(unsigned short)(alpha) + \
        (unsigned short)(bg)*(unsigned short)(255 - (unsigned short)(alpha)) + (unsigned short)128); \
    (composite) = (unsigned char)((temp + (temp >> 8)) >> 8); \
  }
#define BITCOPY(_N, _S, _D)  memcpy(_D, _S, (_N)*sizeof(*(_D)))
#define COPY_CONST_BORDER_C1(src_h, src_w, src, srcstep, dst_h, dst_w, dst, dststep, top1, left1, value )\
  do { \
    int i, j; \
    int left = (left1), top = top1; \
    int src_width = (src_w), src_height = src_h; \
    int dst_width = (dst_w), dst_height = dst_h; \
    for( i = 0; i < dst_height; i++, dst += dststep ) { \
      if( i == 0 || i == src_height + top ) { \
        int limit = i < top || i == src_height + top ? dst_width : left; \
        for( j = 0; j < limit; j++ ) \
          dst[j] = value; \
        \
        if( limit == dst_width ) \
          continue; \
        \
        for( j=src_width+left; j < dst_width; j++ ) \
          dst[j] = value; \
      } \
      \
      if( i < top || i > src_height + top ) \
        memcpy( dst, dst-dststep, dst_width*sizeof(dst[0])); \
      else { \
        if( i > 0 ) { \
          for( j = 0; j < left; j++ ) \
            dst[j] = dst[j - dststep]; \
          for( j = src_width + left; j < dst_width; j++ ) \
            dst[j] = dst[j - dststep]; \
        } \
        memcpy( dst, src, src_width*sizeof(dst[0])); \
        src += srcstep; \
      } \
    } \
  } while(0)
#define COPY_CONST_BORDER_CN(src_h, src_w, src, srcstep, dst_h, dst_w, dst, dststep, cn1, top1, left1, value )\
  do { \
    int i, j, k; \
    int left = (left1)*(cn), top = top1, cn = cn1; \
    int src_width = (src_w)*(cn), src_height = src_h; \
    int dst_width = (dst_w)*(cn), dst_height = dst_h; \
    \
    for( i = 0; i < dst_height; i++, dst += dststep ) { \
      if( i == 0 || i == src_height + top ) { \
        int limit = i < top || i == src_height + top ? dst_width : left; \
        for( j = 0; j < limit; j += cn ) \
          for( k = 0; k < cn; k++ ) \
            dst[j+k] = value[k]; \
        \
        if( limit == dst_width ) \
          continue; \
        \
        for( j=src_width+left; j < dst_width; j+=cn ) \
          for( k = 0; k < cn; k++ ) \
            dst[j+k] = value[k]; \
      } \
      \
      if( i < top || i > src_height + top ) \
        memcpy( dst, dst-dststep, dst_width*sizeof(dst[0])); \
      else { \
        if( i > 0 ) { \
          for( j = 0; j < left; j++ ) \
            dst[j] = dst[j - dststep]; \
          for( j = src_width + left; j < dst_width; j++ ) \
            dst[j] = dst[j - dststep]; \
        } \
        memcpy( dst, src, src_width*sizeof(dst[0])); \
        src += srcstep; \
      } \
    } \
  } while(0)
#define COPY_REPLICATE_BORDER(src_h, src_w, src, srcstep, dst_h, dst_w, dst, dststep, cn, top1, left1)\
  do { \
    int i, j, isrc=0, idst=0; \
    int left = (left1)*(cn), top = top1; \
    int src_width = (src_w)*(cn), src_height = src_h; \
    int dst_width = (dst_w)*(cn), dst_height = dst_h; \
    for( i = 0; i < dst_height; i++, idst += (dststep) ) { \
      /*memcpy( dst + idst + left, src + isrc, (src_width)*sizeof(*(src)) );*/ \
      COPY( (src_width), (src + isrc), (dst + idst + left) ); \
      for( j = left - 1; j >= 0; j-- ) \
        *(dst+idst+j) = *(dst+idst+j + cn); \
      for( j = left + src_width; j < (dst_width); j++ ) \
        *(dst+idst+j) = *(dst+idst+j - cn); \
      if( i >= top && i < top + src_height - 1 ) \
        isrc += srcstep; \
    } \
  } while(0)
#define COPY_REPLICATE_BORDER2(dst_h, dst_w, dst, dststep, cn, top1, left1)\
  do { \
    int i, j, isrc=top1*(dststep)+left1*(cn), idst=0, srcstep=dststep; \
    int src_h=dst_h-top1*2, src_w=dst_w-left1*2; \
    int left = (left1)*(cn), top = top1; \
    int src_width = (src_w)*(cn), src_height = src_h; \
    int dst_width = (dst_w)*(cn), dst_height = dst_h; \
    for( i = 0; i < dst_height; i++, idst += (dststep) ) { \
      if (i<top||i>top + src_height - 1) { \
        COPY( (src_width), (dst + isrc), (dst + idst + left) ); } \
      for( j = left - 1; j >= 0; j-- ) \
        *(dst+idst+j) = *(dst+idst+j + cn); \
      for( j = left + src_width; j < (dst_width); j++ ) \
        *(dst+idst+j) = *(dst+idst+j - cn); \
      if( i >= top && i < top + src_height - 1 ) \
        isrc += srcstep; \
    } \
  } while(0)
#define SKIP_HIST(h, w, _A, al, ai, _hist) \
  do { \
    int a0, a1, _h=(h)*(al), _w=(w)*(ai); \
    if (_w==(al)) {_w*=(h), _h=1; } \
    for (a1=0; a1<_h; a1+=(al)) { \
      for (a0=a1; a0<(_w+a1); a0+=(ai)) { \
        ++(*(_hist+INTCAST( *(_A+a0) ))); /* ??????1 */ \
      } \
    } \
  } while(0)
#define SKIP_HIST_IF(h, w, _A, al, ai, _hist, _Pr) \
  do { \
    int a0, a1, _h=(h)*(al), _w=(w)*(ai); \
    if (_w==(al)) {_w*=(h), _h=1; } \
    for (a1=0; a1<_h; a1+=(al)) { \
      for (a0=a1; a0<(_w+a1); a0+=(ai)) { \
        if (_Pr((_A+a0))) { \
          ++(*(_hist+INTCAST( *(_A+a0) ))); /* ??????1 */ \
        } \
      } \
    } \
  } while(0)
#define SKIP_HIST_MARK(h, w, _A, al, ai, _M, ml, mi, _hist, nCnt) \
  do { \
    int a0, a1, m0, m1, _h=(h)*(al), _w=(w)*(ai); \
    if (_w==(al)) {_w*=(h), _h=1; } \
    for (nCnt=0, a1=0, m1=0; a1<_h; a1+=(al), m1+=ml) { \
      for (a0=a1, m0=m1; a0<(_w+a1); a0+=(ai), m0+=(mi)) { \
        if (*(_M+m0)) {++nCnt; \
          ++(*(_hist+INTCAST( *(_A+a0) ))); /* ??????1 */ \
        } \
      } \
    } \
  } while(0)
#define SKIP_COUNT(h, w, _A, al, ai, _V, nCnt) \
  do { \
    int a0, a1, _h=(h)*(al), _w=(w)*(ai); \
    if (_w==(al)) {_w*=(h), _h=1; } \
    for (a1=0, nCnt=0; a1<_h; a1+=(al)) { \
      for (a0=a1; a0<(_w+a1); a0+=(ai)) { \
        nCnt+=( (_V)==*(_A+a0) ); /* ??????1 */ \
      } \
    } \
  } while(0)
#define SKIP_FILL1D(_N, _F, _S, _VAL) { int _I=0; for (; _I<(_N)*(_S); _I+=(_S)) *(_I+_F)=(_VAL); }
#define SKIP_SUM1D(_N, _F, _S, _SUM)  { int _I=0; for (; _I<(_N)*(_S); _I+=(_S)) (_SUM)+=*(_I+_F); }
// [f, m][m, l] => [m, l][f, m]
#define SHIFT_COPY(_L, _M, _A, _AI, _B, bi) \
  do { \
    int _N = _L-_M; \
    int _N0=(_N)*(_AI), _M0=(_M)*(_AI); \
    SAFE_COPY(_N, _A+_M0, (_AI), _B, bi); \
    SAFE_COPY(_M, _A, (_AI), _B+_N0, bi); \
  } while(0)
#define SKIP_SUM(h, w, _A, al, ai, _S) \
  do { \
    int a0, a1, _j, _h=(h)*(al), _w=(w)*(ai); \
    if (_w==(al)) {_w*=(h), _h=1; } \
    for (a1=0,_j=0, _S=0; a1<_h; ++_j, a1+=(al)) \
      for (a0=a1; a0<(_w+a1); a0+=(ai)) \
        _S +=( *(_A+a0) ); \
  } while(0)
#define SKIP_SUM_MASK(h, w, _A, al, ai, _M, _ML, _S) \
  do { \
    int m0, m1, a0, a1, _j, _h=(h)*(al), _w=(w)*(ai); \
    if (_w==(al)) {_w*=(h), _h=1; } \
    for (m1=0,a1=0,_j=0, _S=0; a1<_h; ++_j, a1+=(al), m1+=_ML) \
      for (m0=m1,a0=a1; a0<(_w+a1); a0+=(ai), ++m0) \
        if (*(_M+m0)) _S +=( *(_A+a0) ); \
  } while(0)
//???????? V[w]
#define SKIP_SUM_V(h, w, _A, al, ai, _V) \
  do { \
    int a0, a1, _j, _h=(h)*(al), _w=(w)*(ai); \
    FILL(w, _V, 0); \
    for (a1=0; a1<_h; a1+=(al)) { \
      for (a0=a1,_j=0; a0<(_w+a1); ++_j, a0+=(ai)) { \
        *(_V+_j)+=( *(_A+a0) ); \
      } \
    } \
  } while(0)
//??????? H[h]
#define SKIP_SUM_H(h, w, _A, al, ai, _H) \
  do { \
    int a0, a1, _j, _h=(h)*(al), _w=(w)*(ai); \
    for (a1=0,_j=0; a1<_h; ++_j, a1+=(al)) { \
      for (*(_H+_j)=0, a0=a1; a0<(_w+a1); a0+=(ai)) { \
        *(_H+_j)+=( *(_A+a0) ); \
      } \
    } \
  } while(0)
#define SKIP_COPY_BORDER(cx, cy, _A, ax, ay, sx, sy) \
  do { \
    int i; \
    int _cx = cx*ax; \
    int _sx = sx*ax; \
    int hay = cy*ay-ay-_sx; \
    for (i=0; i<cy*ay; i+=ay) { \
      VEC_FILL_COLOR(sx, _A+i-_sx, ax, _A+i); \
      VEC_FILL_COLOR(sx, _A+i+_cx, ax, _A+i+_cx-ax); \
    } \
    for (_cx+=2*_sx, i=0; i<(sy*ay); i+=ay) { \
      BITCOPY(_cx,  _A-_sx, _A-_sx-i); \
      BITCOPY(_cx,  _A+hay, _A+hay+i); \
    } \
  } while(0)
// ?????A???????y?锟斤拷????????x??
// border ??锟斤拷???
#define SKIP_COPYEX(ah, aw, _A, al, ai, sx, sy, AE, ael, border) \
  do { \
    int wai = aw*ai; \
    SKIP_COPY(ah, wai, _A, al, 1, AE, ael, 1); \
    if (border) { \
      SKIP_COPY_BORDER(aw, ah, AE, ai, ael, sx, sy); \
    } \
  } while(0)
// ???
#define SKIP_FILTER(_H, _W, _A, _AL, _AI, _B, _BL, _BI, FUNC) \
  do { \
    int A0, A1, _B0, B1; \
    for (A1=0, B1=0; A1<((_H)*(_AL)); A1+=(_AL), B1+=(_BL)) { \
      for (A0=A1, _B0=B1; A0<(_W)*(_AI)+A1; A0+=(_AI), _B0+=(_BI)) { \
        FUNC((_A+A0), _AL, _AI, (_B+_B0)); \
      } \
    } \
  } while(0)
#define SKIP_FILTEREX(_H, _W, _A0, _al, _ai, _B, _bl, _bi, X, Y, FUNC, IT) \
  do { \
    int ael = (_W+(X)*2)*(_ai), _H2=_H+Y+Y, _W2=_W+X+X; \
    IT* AE = MALLOC(IT, (_H+(Y)*2)*ael); \
    IT* _A1 = AE+(X)*(_ai)+(Y)*ael; \
    COPY_REPLICATE_BORDER(_H, _W, _A0, _al, _H2, _W2, AE, ael, _ai, X, Y); \
    SKIP_FILTER(_H, _W, _A1, ael, _ai, _B, _bl, _bi, FUNC); \
    FREE(AE); \
  } while(0)
#define SKIP_FILTER_1D1_1(_N, al, _A, _B, FUNC) \
  do { \
    int a0; \
    for (a0=0; a0<(_N); ++a0) { \
      FUNC((_A+a0), (al), (_B+a0)); \
    } \
  } while(0)
#define SKIP_FILTER_1D1(_cx, ay, _A, ax, _B, bx, FUNC) \
  do { \
    int a0, b0; \
    for (a0=0, b0=0; a0<_cx; a0+=ax, b0+=bx) { \
      FUNC((_A+a0), (ay), (_B+b0)); \
    } \
  } while(0)
#define SKIP_FILTER_2D1(_cx, _cy, az, _A, ax, ay, _B, bx, by, FUNC) \
  do { \
    int a1, b1; \
    for (a1=0, b1=0; a1<_cy; a1+=ay, b1+=by) { \
      SKIP_FILTER_1D1(_cx, az, (_A+a1), ax, (_B+b1), bx); \
    } \
  } while(0)
#define SKIP_FILTER_2D2(_cx, _cy, _A, ax, ay, _B, bx, by, sx, _C, XFUNC, YFUNC)\
  do { \
    int a1, b1, _sx=(sx)*(ax); \
    for (a1=0, b1=0; a1<_cy; a1+=ay, b1+=by) { \
      SKIP_FILTER_1D1_1(_cx, ay, (_A+a1), _C, YFUNC); \
      VEC_FILL_COLOR(sx, (_C-_sx), ax, _C); \
      VEC_FILL_COLOR(sx, (_C+_cx), ax, (_C+_cx-ax)); \
      SKIP_FILTER_1D1_1(_cx, ax, _C, (_B+b1), XFUNC); \
    } \
  } while(0)
// ????????????????????????????????????
// ???锟斤拷?????????????
// ????????????????
// buf[N] = ???????????
#define SKIP_FILTER_RC(_H, _W, _A, al, ai, _B, bl, bi, sx, sy, _C0, XFUNC, YFUNC)\
  do { \
    int _h=(_H)*(al), _w=(_W)*(ai); \
    SKIP_FILTER_2D2(_w, _h, _A, ai, al, _B, bi, bl, sx, _C0, XFUNC, YFUNC); \
  } while(0)
#define SKIP_FILTER_3D3(cx, cy, cz, _A, ax, ay, az, _B, bx, by, bz, sx, sy, \
    _C, _D, dy, FUNC_X, FUNC_Y, FUNC_Z) \
do { \
  int a2, b2, _cx=(cx)*(ax), _cy=(cy)*(ay), _cz=(cz)*(az); \
  for (a2=0, b2=0; a2<_cz; a2+=az, b2+=bz) { \
    SKIP_FILTER_2D1(_cx, _cy, az, (_A+a2), 1, ay, _D, 1, dy, FUNC_Z); \
    SKIP_COPY_BORDER(cx, cy, _D, ax, dy, sx, sy); \
    SKIP_FILTER_RC(cy, cx, _D, ay, ax, (_B+b2), by, bx, sx, sy, \
        _C, FUNC_X, FUNC_Y); \
  } \
} while(0)
#define SKIP_FILTER_RCEX(_H, _W, _A, al, ai, _B, bl, bi, x, y, XFUNC, YFUNC, IT)\
  do { \
    int ael = (_W+(x)*2)*(ai), _H2=_H+y+y, _W2=_W+x+x; \
    IT* AE = MALLOC(IT, (_H+(y)*2)*ael); \
    IT* A1 = AE+(x)*(ai)+(y)*ael; \
    IT* _C0 = AE+(x)*(ai); \
    COPY_REPLICATE_BORDER(_H, _W, _A, al, _H2, _W2, AE, ael, ai, x, y); \
    SKIP_FILTER_RC(_H, _W, A1, ael, ai, _B, bl, bi, x, y, _C0, XFUNC, YFUNC); \
    FREE(AE); \
  } while(0)
#define SKIP_FILTER_HIST(h, w, SRC, al, ai, nCnt, FUNC) \
  do { \
    int _a1, _a0; \
    size_t _idx; \
    for (_a0=0; _a0<(h)*(al); _a0 += al) { \
      for (_a1=_a0; _a1<(w)+_a0; _a1 += ai) { \
        FUNC((SRC+_a1), al, ai, &_idx); \
        ++(*((nCnt)+_idx)); /* ??????1 */ \
      } \
    } \
  } while(0)
// A[h*w]
// [m, n] ???????
// ???? ms, ns
#define SKIP_BLOCK_FILTER_HIST(_h, _w, src, _l, _i, dst, bin, m, n, ms, ns, FUNC)\
  do { \
    int a0, a1, _al=_l*ms, _ai=_i*ns, __h=(_h)*al, __w=(_w)*ai, bini=0; \
    for (a0=0; a0<__h; a0+=_al) { \
      for (a1=a0; a1<__w+a0; a1+=_ai, bini+=bin) { \
        SKIP_FILTER_HIST(m, n, (src+a1), _l, _i, (dst+bini), FUNC); \
      } \
    } \
  } while(0)
#define IMBLOCK_FILTER_HIST(_H, _W, _A, al, ai, _B, bin, hi, wi, hs, ws, x, y, FUNC, IT) \
  do { \
    int ael = (_W+(x)*2)*ai, _H2=_H+y+y, _W2=_W+x+x; \
    IT* AE = MALLOC(IT, (_H2)*ael); \
    IT* AE1 = AE+(x)*ai+(y)*ael; \
    COPY_REPLICATE_BORDER(_H, _W, _A, al, _H2, _W2, AE, ael, ai, y, x); \
    SKIP_BLOCK_FILTER_HIST(_H, _W, AE1, ael, ai, _B, bin, hi, wi, hs, ws, FUNC);\
    FREE(AE); \
  } while(0)
#define SKIP_MAPPING(h, w, A, al, ai, B, bl, bi, m) \
  do { \
    int a0, a1, b0, b1, _h=(h)*(al), _w=w*ai; \
    if (_w==(al) && _w==(bl)) {_w*=(h), _h=1; } \
    for (a1=0, b1=0; a1<_h; a1+=al, b1+=bl) { \
      for (a0=a1, b0=b1; a0<(a1+_w); a0+=ai, b0+=bi) { \
        *(B+b0) = *(m+INTCAST(*(A+a0))); \
      } \
    } \
  } while(0)
#define SKIP_BACK_PROJECT(ah, aw, A, al, ai, B, bl, bi, hi, bin, cn) \
  do { \
    int i, bin_i; \
    for (i=0, bin_i=0; i<cn; ++i, bin_i+=(bin)) { \
      SKIP_MAPPING(ah, aw, (A+i), al, ai, (B+i), bl, bi, (hi+bin_i)); \
    } \
  } while(0)
// ????????
#define SKIP_HISTEQ(h, w, A, al, ai, B, bl, bi) \
  do { \
    int _i, _sum, _hist[256]; \
    float _scale = 255.f/(w*h); \
    memset(_hist, 0, sizeof(int)*256); \
    SKIP_HIST(h, w, A, al, ai, _hist); \
    for( _sum=0, _i = 0; _i < 256; _i++ ) { \
      _sum += ROUND(_hist[_i]); \
      _hist[_i] = (uchar)ROUND(_sum*_scale); \
    } \
    SKIP_MAPPING(h, w, A, al, ai, B, bl, bi, _hist); \
  } while(0)
#ifndef SKIP_FILL
#define SKIP_FILL(h, w, _A, al, ai, _V) \
  do { \
    int a0, a1, _h=(h)*(al), _w=(w)*(ai); \
    if (_w==(al)) {_w*=(h), _h=1; } \
    for (a1=0; a1<_h; a1+=(al)) { \
      for (a0=a1; a0<(_w+a1); a0+=(ai)) { \
        (*(_A+a0) ) = _V; \
      } \
    } \
  } while(0)
#endif

#define TRANSFORM(_IF, _IL, _OF, _OP) {int _i=0; for (; (_IF+_i)!=(_IL); ++_i) {*(_OF+_i)=_OP(*(_IF+_i));}}
#define LESS(a, b)  ((a)<(b))
#ifndef CC_SWAP
#define CC_SWAP(a,b,t) ((t) = (a), (a) = (b), (b) = (t))
#endif
#ifndef MIN
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif
#ifndef MAX
#define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif
#define MINLT(A,B,LT)  (LT((A), (B)) ? (A) : (B))
#define MAXLT(A,B,LT)  (LT((A), (B)) ? (A) : (B))
#define MIDLT(A, B, C, LT) \
  (LT((A), (B)) ? (LT((B), (C)) ? (B) : MAXLT(A, C,LT)) \
        : (LT((C), (B)) ? (B) : MINLT(A, C,LT)))
#define MINLTITER(A,B,LT)  (LT((*A), (*B)) ? (A) : (B))
#define MAXLTITER(A,B,LT)  (LT((*A), (*B)) ? (B) : (A))
#define MIDLTITER(A, B, C, LT)   (LT((*(A)), (*(B))) ? (LT((*(B)), (*(C))) ? (B) : (LT((*(A)), (*(C))) ? (C) : (A))) \
        : (LT((*(C)), (*(B))) ? (B) : (LT((*(A)), (*(C))) ? (A) : (C))))
#define SORT_INSERT1(_N, _F, _X, _T, _P) { \
    int _i=0; _T _x=_X, t; for (; _i<_N; ++_i) { \
      if (_P(_x, (_F)[_i])) {CC_SWAP(_x, (_F)[_i], t);} \
    } \
  }
#define INSERT_SORT(left, right, _P, _Ty) { \
    _Ty* ptr = left + 1, *ptr2, t; \
    for( ; ptr <= right; ++ptr ) { \
      for( ptr2 = ptr; ptr2 > left && (_P(ptr2[0],ptr2[-1])); ptr2--) \
        CC_SWAP( ptr2[0], ptr2[-1], t ); \
    } \
  }
#define PARTITION(_T, _F0, _L0, _P, _OF)   { _T t; \
    _T* _F = _F0; _T* _L = _L0; \
    for (; ; ++_F) { \
      for (; _F != _L && _P(*_F); ++_F); \
      if (_F == _L) { break; } \
      for (; _F != --_L && !_P(*_L);); \
      if (_F == _L) { break; } \
      CC_SWAP(*_F, *_L, t); \
    } \
    _OF =(_F); \
  }
// ????????
#define QUICK_SORT_VER1(_N, _F, _P, _Ty2) \
  do { \
    typedef _Ty2 _Ty; \
    _Ty t; \
    int sp = 0; \
    struct { _Ty *lb; _Ty *ub; } stack[48]; \
    stack[0].lb = (_F); \
        stack[0].ub = (_F)+(_N)-1; \
    while( sp >= 0 ) { \
      _Ty* left = stack[sp].lb; \
      _Ty* right = stack[sp--].ub; \
      for(;;) { \
        int i, n = (int)(right - left) + 1, m; \
        if( n <= 64 ) { \
          INSERT_SORT(left, right, _P, _Ty); \
          break; \
        } else { \
          _Ty *left0, *left1, *right0, *right1, *pivot, *a, *b, *c; \
          int swap_cnt = 0; \
          left0 = left; \
          right0 = right; \
          pivot = left + (n/2); \
          if( n > 40 ) { \
            int d = n / 8; \
            a = left, b = left + d, c = left + 2*d; \
            left = MIDLTITER(a, b, c, _P); \
            a = pivot - d, b = pivot, c = pivot + d; \
            pivot = MIDLTITER(a, b, c, _P); \
            a = right - 2*d, b = right - d, c = right; \
            right = MIDLTITER(a, b, c, _P); \
          } \
          a = left, b = pivot, c = right; \
          pivot = MIDLTITER(a, b, c, _P); \
          if( pivot != left0 ) { \
            CC_SWAP( *pivot, *left0, t ); \
            pivot = left0; \
          } \
          left = left1 = left0 + 1; \
          right = right1 = right0; \
          for(;;) { \
            while( left <= right && !(_P((*pivot), (*left))) ) { \
              if( !(_P((*left), (*pivot))) ) { \
                if( left > left1 ) \
                  CC_SWAP( *left1, *left, t ); \
                swap_cnt = 1; \
                left1++; \
              } \
              left++; \
            } \
            while( left <= right && !(_P((*right), (*pivot))) ) { \
              if( !(_P((*pivot), (*right))) ) { \
                if( right < right1 ) \
                  CC_SWAP( *right1, *right, t ); \
                swap_cnt = 1; \
                right1--; \
              } \
              right--; \
            } \
            if( left > right ) \
              break; \
            CC_SWAP( *left, *right, t ); \
            swap_cnt = 1; \
            left++; \
            right--; \
          } \
          if( swap_cnt == 0 ) { \
            left = left0, right = right0; \
            INSERT_SORT(left, right, _P, _Ty); \
            break; \
          } \
          n = MIN( (int)(left1 - left0), (int)(left - left1) ); \
          for( i = 0; i < n; i++ ) \
            CC_SWAP( left0[i], left[i-n], t ); \
          n = MIN( (int)(right0 - right1), (int)(right1 - right) ); \
          for( i = 0; i < n; i++ ) \
            CC_SWAP( left[i], right0[i-n+1], t ); \
          n = (int)(left - left1); \
          m = (int)(right1 - right); \
          if( n > 1 ) { \
            if( m > 1 ) { \
              if( n > m ) { \
                stack[++sp].lb = left0; \
                stack[sp].ub = left0 + n - 1; \
                left = right0 - m + 1, right = right0; \
              } else { \
                stack[++sp].lb = right0 - m + 1; \
                stack[sp].ub = right0; \
                left = left0, right = left0 + n - 1; \
              } \
            } else \
              left = left0, right = left0 + n - 1; \
          } else if( m > 1 ) \
            left = right0 - m + 1, right = right0; \
          else \
            break; \
        } \
      } \
    } \
  } while(0)
// quick-sort using a comparison function _Cmpr
#define QUICK_SORT_VER2(_N, _F, _Cmpr, _Ty2)\
  do {\
    typedef _Ty2 _Ty; \
    int _so_i,_so_ir = (_N)-1,_so_j,_so_k,_so_l = 0;\
    int _so_istack[50],_so_jstack = -1;\
    _Ty _so_a, _so_tmp;\
    for (;;){\
      if (_so_ir-_so_l<9){\
        for (_so_j = _so_l+1;_so_j<=_so_ir;++_so_j){\
          _so_a = (_F)[_so_j];\
          for (_so_i = _so_j-1;_so_i>=_so_l;--_so_i){\
            if (!_Cmpr(_so_a,((_F)[_so_i]))) break;\
            (_F)[_so_i+1] = (_F)[_so_i];\
          }\
          (_F)[_so_i+1] = _so_a;\
        }\
        if (_so_jstack==-1) break;\
        _so_ir = _so_istack[_so_jstack];\
        _so_l = _so_istack[_so_jstack-1];\
        _so_jstack -= 2;\
      } else {\
        _so_k = (_so_l+_so_ir)>>1;\
        CC_SWAP((_F)[_so_k],(_F)[_so_l+1], _so_tmp);\
        if (_Cmpr(((_F)[_so_ir]),((_F)[_so_l])))\
          CC_SWAP((_F)[_so_l],(_F)[_so_ir], _so_tmp);\
        if (_Cmpr(((_F)[_so_ir]),((_F)[_so_l+1])))\
          CC_SWAP((_F)[_so_l+1],(_F)[_so_ir], _so_tmp);\
        if (_Cmpr(((_F)[_so_l+1]),((_F)[_so_l])))\
          CC_SWAP((_F)[_so_l],(_F)[_so_l+1], _so_tmp);\
        _so_i = _so_l+1,_so_j = _so_ir;\
        _so_a = (_F)[_so_l+1];\
        for (;;){\
          do ++_so_i; while(_Cmpr(((_F)[_so_i]),_so_a));\
          do --_so_j; while(_Cmpr(_so_a,((_F)[_so_j])));\
          if (_so_j<_so_i) break;\
          CC_SWAP((_F)[_so_i],(_F)[_so_j], _so_tmp);\
        }\
        (_F)[_so_l+1] = (_F)[_so_j];\
        (_F)[_so_j] = _so_a;\
        _so_jstack += 2;\
        if (_so_ir-_so_i+1>=_so_j-_so_l){\
          _so_istack[_so_jstack] = _so_ir;\
          _so_istack[_so_jstack-1] = _so_i;\
          _so_ir = _so_j-1;\
        } else {\
          _so_istack[_so_jstack] = _so_j-1;\
          _so_istack[_so_jstack-1] = _so_l;\
          _so_l = _so_i;\
        }\
      }\
    }\
  } while(0)
#define SORT2(vec2, LESS, t) do { if (!LESS(*(vec2), *(vec2+1))) CC_SWAP(*(vec2), *(vec2+1), t); } while(0)
#define SORT3(vec, LESS, t) do {SORT2(vec+1, LESS, t);if(!LESS(*(vec), *(vec+1))) {CC_SWAP(*(vec), *(vec+1), t);SORT2(vec+1, LESS, t);} } while(0)
#define SORT4(vec, LESS, t) do {SORT3(vec+1, LESS, t);if(!LESS(*(vec), *(vec+1))) {CC_SWAP(*(vec), *(vec+1), t);SORT3(vec+1, LESS, t);} } while(0)
#define SORT5(vec, LESS, t) do {SORT4(vec+1, LESS, t);if(!LESS(*(vec), *(vec+1))) {CC_SWAP(*(vec), *(vec+1), t);SORT4(vec+1, LESS, t);} } while(0)
#define SORT6(vec, LESS, t) do {SORT5(vec+1, LESS, t);if(!LESS(*(vec), *(vec+1))) {CC_SWAP(*(vec), *(vec+1), t);SORT5(vec+1, LESS, t);} } while(0)
#define SORT7(vec, LESS, t) do {SORT6(vec+1, LESS, t);if(!LESS(*(vec), *(vec+1))) {CC_SWAP(*(vec), *(vec+1), t);SORT6(vec+1, LESS, t);} } while(0)
#define SORT8(vec, LESS, t) do {SORT7(vec+1, LESS, t);if(!LESS(*(vec), *(vec+1))) {CC_SWAP(*(vec), *(vec+1), t);SORT7(vec+1, LESS, t);} } while(0)
#define SORT9(vec, LESS, t) do {SORT8(vec+1, LESS, t);if(!LESS(*(vec), *(vec+1))) {CC_SWAP(*(vec), *(vec+1), t);SORT8(vec+1, LESS, t);} } while(0)
#if 1
#define QSORT(_F, _L, _P, _Ty)   QUICK_SORT_VER1((_L-_F), _F, _P, _Ty)
#else
#define QSORT(_F, _L, _P, _Ty)   QUICK_SORT_VER2((_L-_F), _F, _P, _Ty)
#endif
// ????????
#define INSERTSORT(_N, _F, _P, _Ty) \
  do { \
    int _I, _J; \
    for (_I=1; _I<_N; ++_I) { \
      _Ty temp = *(_F+_I); \
      for (_J=_I-1; _J>=0 && _P((temp), (*(_F+_J))); --_J) { \
        *(_F+_J+1) = *(_F+_J); \
      } \
      *(_F+_J+1) = temp; \
    } \
  } while(0)
// shell????
#define SHELLSORT(_N, _F, _P, _Ty) \
  do { \
    int _I, _J, _D = _N; \
    do{ \
      _D = _D/2; /* ???????????????????????????????2???锟斤拷 */ \
      for (_I=_D; _I<_N; ++_I) { \
        if (_P(*(_F+_I), *(_F+_I-_D))) { \
          _Ty temp = *(_F+_I); \
          for (_J=_I-_D; _J>=0 && _P(temp, *(_F+_J)); _J-=_D) { \
            *(_F+_J+_D) = *(_F+_J); \
          } \
          *(_F+_J+_D) = temp; \
        } \
      } \
    } while (_D>1); \
  } while(0)
// e??????
#define BUBBLESORT(_N, _F, _P, _Ty) \
  do { \
    int _I=0, _J, _STOP; \
    do { \
      for(_STOP=0, _J=_N-1; _J>_I; --_J) { \
        if (_P(*(_F+_J), *(_F+_J-1))) { \
          _Ty temp=*(_F+_J); \
          *(_F+_J)=*(_F+_J-1); \
          *(_F+_J-1)=temp; \
          _STOP=1; \
        } \
      } \
    } while(_STOP && (_I++)<_N); \
  } while(0)
// e??????(2)
#define BUB_SORT(_N, _F, _P, _Ty) \
  do { \
    int _M,_K,_J,_I; \
    _Ty temp; \
    for (_K=0, _M=_N-1; _K<_M; ) { \
      for (_J=_M-1, _M=0, _I=_K; _I<=_J; ++_I) \
        if (*(_F+_I)>*(_F+_I+1)) { \
          CC_SWAP(*(_F+_I), *(_F+_I+1), temp); \
          _M=_I; \
        } \
      for (_J=_K+1, _K=0, _I=_M; _I>=_J; --_I) \
        if (*(_F+_I-1)>*(_F+_I)) { \
          CC_SWAP(*(_F+_I), *(_F+_I-1), temp); \
          _K=_I; \
        } \
    } \
  } while(0)
#define HEAPSIFT(_F, _I, _N, temp) \
  do { \
    int _J, _K=_I; \
    temp = *(_F+_K); \
    for (_J=2*(_K+1)-1; _J<=_N; ) { \
      if ((_J<_N)&&(*(_F+_J)<*(_F+_J+1))) \
        _J=_J+1; \
      if (temp<*(_F+_J)) { \
        *(_F+_K)=*(_F+_J); \
        _K=_J; \
        _J=2*(_K+1)-1; \
      } else \
        _J=_N+1; \
    } \
    *(_F+_K)=temp; \
  } while(0)
// ??????
#define HEAPPSORT(_N, _F, _P, _Ty) \
  do { \
    int _I, _M; \
    _Ty temp; \
    _M=_N/2; \
    for (_I=_M-1; _I>=0; _I--) \
      HEAPSIFT(_F, _I, _N-1, temp); \
    for (_I=_N-1; _I>=1; _I--) { \
      CC_SWAP(*(_F), *(_F+_I), temp); \
      HEAPSIFT(_F, 0, _I-1, temp); \
    } \
  } while(0)
//???????????????????????????
#define MERGESORTCOPY(begin1, end1, begin2, end2, out, _P) \
  do { \
    int i, i1, i2, len1=end1-begin1, len2=end2-begin2; \
    for (i=0, i1=0, i2=0;; ++i) { \
      if ( i1!=len1 && i2!=len2 ) { \
        if (_P(begin1[i1], begin2[i2])) { \
          out[i]=begin1[i1++]; \
        } else { \
          out[i]=begin2[i2++]; \
        } \
      } else if ( i1!=len1 ) { \
        out[i]=begin1[i1++]; \
      } else if ( i2!=len2 ) { \
        out[i]=begin2[i2++]; \
      } else { \
        break; \
      } \
    } \
  } while(0)
//?锟斤拷????
#define MERGESORT(_N, _F, _P, _Ty) \
  do { \
    struct { _Ty *lb, *mb, *ub; } stack[48]; \
    int sp=0; \
        _Ty* buf=MALLOC(_Ty, _N); \
        stack[0].lb = _F; \
        stack[0].mb = NULL; \
        stack[0].ub = _F + _N; \
    while(sp >= 0) { \
      _Ty *left = stack[sp].lb, *mid = stack[sp].mb, *right = stack[sp].ub; \
      int _LEN = right - left; \
      if (!mid) { \
        if (_LEN<=64) { \
          INSERTSORT(_LEN, left, _P, _Ty); \
          --sp; \
        } else { \
          stack[sp].mb = mid = left + _LEN/2; \
          stack[++sp].lb = left; \
          stack[sp].mb = NULL; \
          stack[sp].ub = mid; \
          stack[++sp].lb = mid; \
          stack[sp].mb = NULL; \
          stack[sp].ub = right; \
        } \
      } else { \
        MERGESORTCOPY(left, mid, mid, right, buf, _P); \
        memcpy(left, buf, _LEN*sizeof(_Ty)); \
        --sp; \
      } \
    } \
    SAFEFREE(buf); \
  } while(0)
#define DEFULT(x)
//#define DEFULT(x) =(x)
#define OverflowException(x)                ASSERT( 0 && x )
#define ArithmeticException(x)              ASSERT( 0 && x )
#define ArgumentNullException(x)            ASSERT( 0 && x )
#define ArgumentException(x)                ASSERT( 0 && x )
#define ArgumentOutOfRangeException(x)      ASSERT( 0 && x )
#define InvalidImagePropertiesException(x)  ASSERT( 0 && x )
#define UnsupportedImageFormatException(x)  ASSERT( 0 && x )
#define GetResourceString(a)  (a)

//
CC_INLINE int is2pow(int i)
{
  return ((i > 0) && ((i & (i - 1)) == 0));//2??n???????????0??
}
//
CC_INLINE int ilog2(int value)
{
  int x = 0;
  while (value > 1) {
    value >>= 1;
    x++;
  }
  return x;
}
CC_INLINE int NextPowerOfTwo(int x) {
  
  int z = (x > 0) ? x - 1 : 0;
  z |= z >> 1;
  z |= z >> 2;
  z |= z >> 4;
  z |= z >> 8;
  z |= z >> 16;
  
  return (int)(z + 1);
}
CC_INLINE size_t next_power(size_t x)
{
#if 0
  size_t z = 1;
  while (z < x) {
    z <<= 1;
  }
#endif
#if 1
  size_t z = (x > 0) ? x - 1 : 0;
  z |= z >> 1;
  z |= z >> 2;
  z |= z >> 4;
  z |= z >> 8;
  z |= z >> 16;
  ++z;
#endif
  return z;
}
CC_INLINE size_t next_power_1_5(size_t n)
{
  size_t m = 8;
  while (m < n) {
    m = m*3/2;
  }
  return m;
}
#if defined _WIN32 && _MSC_VER <=1200
#include <ymath.h>
#define _DENORM    (-2)
//#define _FINITE    (-1)
//#define _INFCODE   1
//#define _NANCODE   2

#define FP_INFINITE  _INFCODE /*正、负无穷*/
#define FP_NAN       _NANCODE /*not a number*/
#define FP_NORMAL    _FINITE  /*正常的浮点数（不是以上结果中的任何一种）*/
#define FP_SUBNORMAL _DENORM  /*太小，以至于不能用浮点数的规格化形式表示*/
#define FP_ZERO      0        /*是0*/
#endif

CC_INLINE int fpclassify_f32(float t) {
  union {
    float f;
    unsigned int i;
  } u;
  u.f = t;
  if (((u.i & 0x7f800000) == 0x7f800000) && ((u.i & 0x007fffff) == 0)) {
    return FP_INFINITE;
  }
  if (((u.i & 0x7f800000) == 0x7f800000) && ((u.i & 0x007fffff) != 0)) {
    return FP_NAN;
  }
  return 0==u.i ? FP_ZERO : FP_NORMAL;
}
CC_INLINE int fpclassify_f64(double t) {
  union {
    double d;
    unsigned int i[2];
  } u;
  u.d = t;
  if ('b'!=ENDIANNESS) {
    if (((u.i[1] & 0x7ff00000) == 0x7ff00000)
      && (((u.i[1] & 0x000fffff) == 0) && (u.i[0] == 0))) {
      return FP_INFINITE;
    }
    if (((u.i[1] & 0x7ff00000) == 0x7ff00000)
      && (((u.i[1] & 0x000fffff) != 0) || (u.i[0] != 0))) {
      return FP_NAN;
    }
  }
  else {
    if (((u.i[0] & 0x7ff00000) == 0x7ff00000)
      && (((u.i[0] & 0x000fffff) == 0) && (u.i[1] == 0))) {
      return FP_INFINITE;
    }
    if (((u.i[0] & 0x7ff00000) == 0x7ff00000)
      && (((u.i[0] & 0x000fffff) != 0) || (u.i[1] != 0))) {
      return FP_NAN;
    }
  }
  return FP_NORMAL;
}
#define IsNaN_f32(t)  (FP_NAN==fpclassify_f32(t))
#define IsNaN_f64(t)  (FP_NAN==fpclassify_f64(t))
#define IsInfinite_f32(t)  (FP_INFINITE==fpclassify_f32(t))
#define IsInfinite_f64(t)  (FP_INFINITE==fpclassify_f64(t))
CC_INLINE BOOL IsFinite_f32(float t)
{
  return (!(IsNaN_f32(t) || IsInfinite_f32(t)));
}
CC_INLINE BOOL IsFinite_f64(double t)
{
  return (!(IsNaN_f64(t) || IsInfinite_f64(t)));
}
CC_INLINE float NaN_f32()
{
  union {
    float f;
    unsigned int i;
  } u;
  u.i = 0x7fC00000;
  return u.f;
}
CC_INLINE float Inf_f32()
{
  union {
    float f;
    unsigned int i;
  } u;
  u.i = 0x7f800000;
  return u.f;
}
CC_INLINE double NaN_f64()
{
  union {
    double d;
    unsigned int i[2];
  } u;
  if ('b'!=ENDIANNESS) {
    u.i[0] = 0;
    u.i[1] = 0x7ffc0000;
  }
  else {
    u.i[0] = 0x7ffc0000;
    u.i[1] = 0;
  }
  return u.d;
}
CC_INLINE double Inf_f64()
{
  union {
    double d;
    unsigned int i[2];
  } u;
  if ('b'!=ENDIANNESS) {
    u.i[0] = 0;
    u.i[1] = 0x7ff00000;
  }
  else {
    u.i[0] = 0x7ff00000;
    u.i[1] = 0;
  }
  return u.d;
}

// int hash
// 乘法哈希算法
// 问题在于，低位可以影响高位，但高位不能影响低位。 分布是不均匀的。
CC_INLINE uint32 hash32address(uint32 key)
{
  return (key >> 3) * 2654435761; //这个3是对齐边界为8
}
// Tomas Wang
CC_INLINE uint32 hash32shift(uint32 key)
{
  key = ~key + (key << 15); // key = (key << 15) - key - 1;
  key = key ^ (key >> 12);
  key = key + (key << 2);
  key = key ^ (key >> 4);
  key = key * 2057; // key = (key + (key << 3)) + (key << 11);
  key = key ^ (key >> 16);
  return key;
}
// Bob Jenkins' 32 bit integer hash function
CC_INLINE uint32 hash32bit( uint32 a)
{
  a = (a+0x7ed55d16) + (a<<12);
  a = (a^0xc761c23c) ^ (a>>19);
  a = (a+0x165667b1) + (a<<5);
  a = (a+0xd3a2646c) ^ (a<<9);
  a = (a+0xfd7046c5) + (a<<3); // <<和 +的组合是可逆的
  a = (a^0xb55a4f09) ^ (a>>16); 
  return a;
}
// 这六个数是随机数， 通过设置合理的6个数，你可以找到对应的perfect hash.
// 64 bit Mix Functions
CC_INLINE uint64 hash64shift(uint64 key)
{
  key = (~key) + (key << 21); // key = (key << 21) - key - 1;
  key = key ^ (key >> 24);
  key = (key + (key << 3)) + (key << 8); // key * 265
  key = key ^ (key >> 14);
  key = (key + (key << 2)) + (key << 4); // key * 21
  key = key ^ (key >> 28);
  key = key + (key << 31);
  return key;
}
// 64 bit to 32 bit Mix Functions
CC_INLINE uint32 hash64_32shift(uint64 key)
{
  key = (~key) + (key << 18); // key = (key << 18) - key - 1;
  key = key ^ (key >> 31);
  key = key * 21; // key = (key + (key << 2)) + (key << 4);
  key = key ^ (key >> 11);
  key = key + (key << 6);
  key = key ^ (key >> 22);
  return (int) key;
}

// Bob Jenkins' 96 bit Mix Function
CC_INLINE uint32 hash96mix(uint32 a, uint32 b, uint32 c)
{
  a=a-b;  a=a-c;  a=a^(c >> 13);
  b=b-c;  b=b-a;  b=b^(a << 8);
  c=c-a;  c=c-b;  c=c^(b >> 13);
  a=a-b;  a=a-c;  a=a^(c >> 12);
  b=b-c;  b=b-a;  b=b^(a << 16);
  c=c-a;  c=c-b;  c=c^(b >> 5);
  a=a-b;  a=a-c;  a=a^(c >> 3);
  b=b-c;  b=b-a;  b=b^(a << 10);
  c=c-a;  c=c-b;  c=c^(b >> 15);
  return c;
}

// getline : get line into s, return length
CC_INLINE int mygetline(char* s, int len) {
  int c, i = 0;
  while ( i<(len-1) && (c = getchar()) != EOF && c != '\n') {
    s[i++] = c;
  }
  // if (c == '\n') {    s[i++] = c;  }
  s[i] = '\0';    
  return i;    
}
CC_INLINE int file_putline(const char* fn, const char* str) {
  FILE* pf;
  pf = fopen(fn, "a");
  if (pf) {
    int len = strlen(str);
    fwrite(str, 1, len, pf);
    fwrite("\n", 1, 1, pf);
    fclose(pf);
  }
  return 0;
}
