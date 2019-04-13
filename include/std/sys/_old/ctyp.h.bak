#define CLASSFUN_INIT(CLASS, PREFIX, FUNNAME)   (CLASS)->FUNNAME = PREFIX##_##FUNNAME
#define CVAPI_EXPORTS
#include <assert.h>
#include <ctype.h>
#include <float.h>
#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <errno.h>
#include <stdarg.h> // for va_list
#ifdef __ARM_NEON
#define __arm__
#endif
#ifdef __cplusplus
#define CC_DEFAULT(val) = val
#else
#define CC_DEFAULT(val)
#define bool int
#define true 1
#define false 0
#endif
#ifndef DEFAULT
#define DEFAULT CC_DEFAULT
#endif
#if defined __cplusplus
#define CC_INLINE inline
#elif (defined _WIN32 || defined WIN64 || defined _WIN64 || defined WINCE) && !defined __GNUC__
#define CC_INLINE __inline
#else
#define CC_INLINE static
#endif
#define CC_IMPL static

#if defined __GNUC__
#define CC_Func __func__
#elif defined(_MSC_VER) && (_MSC_VER >= 1400 )
#define CC_Func __FUNCTION__
#else
#define CC_Func ""
#endif

//! @addtogroup core_utils
//! @{

#if !defined _CRT_SECURE_NO_DEPRECATE && defined _MSC_VER && _MSC_VER > 1300
#  define _CRT_SECURE_NO_DEPRECATE /* to avoid multiple Visual Studio warnings */
#endif

// undef problematic defines sometimes defined by system headers (windows.h in particular)
#undef small
#undef min
#undef max
#undef abs
#undef Complex

#if !defined _CRT_SECURE_NO_DEPRECATE && defined _MSC_VER && _MSC_VER > 1300
#  define _CRT_SECURE_NO_DEPRECATE /* to avoid multiple Visual Studio warnings */
#endif



#if defined __ICL
#  define CC_ICC   __ICL
#elif defined __ICC
#  define CC_ICC   __ICC
#elif defined __ECL
#  define CC_ICC   __ECL
#elif defined __ECC
#  define CC_ICC   __ECC
#elif defined __INTEL_COMPILER
#  define CC_ICC   __INTEL_COMPILER
#endif

#ifndef CC_INLINE
#  if defined __cplusplus
#    define CC_INLINE static inline
#  elif defined _MSC_VER
#    define CC_INLINE __inline
#  else
#    define CC_INLINE static
#  endif
#endif

#if defined CC_ICC && !defined CC_ENABLE_UNROLLED
#  define CC_ENABLE_UNROLLED 0
#else
#  define CC_ENABLE_UNROLLED 1
#endif

#ifdef __GNUC__
#  define CC_DECL_ALIGNED(x) __attribute__ ((aligned (x)))
#elif defined _MSC_VER
#  define CC_DECL_ALIGNED(x) __declspec(align(x))
#else
#  define CC_DECL_ALIGNED(x)
#endif

// do not include SSE/AVX/NEON headers for NVCC compiler
#ifndef __CUDACC__

#if defined __SSE2__ || defined _M_X64 || (defined _M_IX86_FP && _M_IX86_FP >= 2) || (defined _MSC_VER && _MSC_VER >= 1200)
#  include <emmintrin.h>
#  define CC_MMX 1
#  define CC_SSE 1
#  define CC_SSE2 1
#  define USE_SSE2 0
// a += b*c
#define _mm_muladd_ps(a, b, c)  a = _mm_add_ps(a, _mm_mul_ps(b, c))
#  if defined __SSE3__ || (defined _MSC_VER && _MSC_VER >= 1500)
#    include <pmmintrin.h>
#    define CC_SSE3 1
#  endif
#  if defined __SSSE3__  || (defined _MSC_VER && _MSC_VER >= 1500)
#    include <tmmintrin.h>
#    define CC_SSSE3 1
#  endif
#  if defined __SSE4_1__ || (defined _MSC_VER && _MSC_VER >= 1500)
#    include <smmintrin.h>
#    define CC_SSE4_1 0
#  endif
#  if defined __SSE4_2__ || (defined _MSC_VER && _MSC_VER >= 1500)
#    include <nmmintrin.h>
#    define CC_SSE4_2 1
#  endif
#  if defined __POPCNT__ || (defined _MSC_VER && _MSC_VER >= 1500)
#    ifdef _MSC_VER
#      include <nmmintrin.h>
#    else
#      include <popcntintrin.h>
#    endif
#    define CC_POPCNT 1
#  endif
#  if defined __AVX__ || (defined _MSC_VER && _MSC_VER >= 1600 && 0)
// MS Visual Studio 2010 (2012?) has no macro pre-defined to identify the use of /arch:AVX
// See: http://connect.microsoft.com/VisualStudio/feedback/details/605858/arch-avx-should-define-a-predefined-macro-in-x64-and-set-a-unique-value-for-m-ix86-fp-in-win32
#    include <immintrin.h>
#    define CC_AVX 1
#    if defined(_XCR_XFEATURE_ENABLED_MASK)
#      define __xgetbv() _xgetbv(_XCR_XFEATURE_ENABLED_MASK)
#    else
#      define __xgetbv() 0
#    endif
#  endif
#  if defined __AVX2__ || (defined _MSC_VER && _MSC_VER >= 1800 && 0)
#    include <immintrin.h>
#    define CC_AVX2 1
#    if defined __FMA__
#      define CC_FMA3 1
#    endif
#  endif
#endif

#if (defined WIN32 || defined _WIN32) && defined(_M_ARM)
# include <Intrin.h>
# include <arm_neon.h>
# define CC_NEON 1
# define CPU_HAS_NEON_FEATURE (true)
#elif defined(__ARM_NEON__) || (defined (__ARM_NEON) && defined(__aarch64__))
#  include <arm_neon.h>
#  define CC_NEON 1
#endif

#if defined __GNUC__ && defined __arm__ && (defined __ARM_PCS_VFP || defined __ARM_VFPV3__ || defined __ARM_NEON__) && !defined __SOFTFP__
#  define CC_VFP 1
#endif

#if (defined _MSC_VER && _MSC_VER >= 1300) || defined(_M_ARM) || defined(__linux__)
#define _OPENMP
#endif

#ifndef _WIN32
#define _OPENMP
#endif

#if defined _OPENMP || (defined _MSC_VER && _MSC_VER >= 1300) || defined(_M_ARM) || defined(__linux__)
#define CC_OPENMP 1
#include <omp.h>
#endif

#endif // __CUDACC__

#ifndef CC_OPENMP
#define CC_OPENMP 0
#endif
#ifndef CC_POPCNT
#define CC_POPCNT 0
#endif
#ifndef CC_MMX
#  define CC_MMX 0
#endif
#ifndef CC_SSE
#  define CC_SSE 0
#endif
#ifndef CC_SSE2
#  define CC_SSE2 0
#endif
#ifndef CC_SSE3
#  define CC_SSE3 0
#endif
#ifndef CC_SSSE3
#  define CC_SSSE3 0
#endif
#ifndef CC_SSE4_1
#  define CC_SSE4_1 0
#endif
#ifndef CC_SSE4_2
#  define CC_SSE4_2 0
#endif
#ifndef CC_AVX
#  define CC_AVX 0
#endif
#ifndef CC_AVX2
#  define CC_AVX2 0
#endif
#ifndef CC_FMA3
#  define CC_FMA3 0
#endif
#ifndef CC_AVX_512F
#  define CC_AVX_512F 0
#endif
#ifndef CC_AVX_512BW
#  define CC_AVX_512BW 0
#endif
#ifndef CC_AVX_512CD
#  define CC_AVX_512CD 0
#endif
#ifndef CC_AVX_512DQ
#  define CC_AVX_512DQ 0
#endif
#ifndef CC_AVX_512ER
#  define CC_AVX_512ER 0
#endif
#ifndef CC_AVX_512IFMA512
#  define CC_AVX_512IFMA512 0
#endif
#ifndef CC_AVX_512PF
#  define CC_AVX_512PF 0
#endif
#ifndef CC_AVX_512VBMI
#  define CC_AVX_512VBMI 0
#endif
#ifndef CC_AVX_512VL
#  define CC_AVX_512VL 0
#endif

#ifndef CC_NEON
#  define CC_NEON 0
#endif

#ifndef CC_VFP
#  define CC_VFP 0
#endif

/* fundamental constants */
#define _1K   (1024)
#define _1M   ((_1K)*(_1K))
#define _1G   ((_1K)*(_1M))
#define CC_PI   (3.14159265358979323846264338327950288419716939937511)
#define CC_2PI 6.283185307179586476925286766559
#define CC_LOG2 0.69314718055994530941723212145818
#define _LOG2   CC_LOG2
#ifndef PI
//#define PI      CC_PI
#endif

#include <limits.h>

#if !defined _I64_MIN
#define _I64_MIN LONG_LONG_MIN
#define _I64_MAX LONG_LONG_MAX
#endif

#if defined __ARM_FP16_FORMAT_IEEE \
    && !defined __CUDACC__
#  define CC_FP16_TYPE 1
#else
#  define CC_FP16_TYPE 0
#endif

#define OPENCC_ABI_COMPATIBILITY 300

#ifdef __OPENCC_BUILD
#  define DISABLE_OPENCC_24_COMPATIBILITY
#endif

#define CC_EXPORTS

#ifndef CC_EXTERN_C
#  ifdef __cplusplus
#    define CC_EXTERN_C extern "C"
#  else
#    define CC_EXTERN_C
#  endif
#endif

/* special informative macros for wrapper generators */
#define CC_EXPORTS_W CC_EXPORTS
#define CC_EXPORTS_W_SIMPLE CC_EXPORTS
#define CC_EXPORTS_AS(synonym) CC_EXPORTS
#define CC_EXPORTS_W_MAP CC_EXPORTS
#define CC_IN_OUT
#define CC_OUT
#define CC_PROP
#define CC_PROP_RW
#define CC_WRAP
#define CC_WRAP_AS(synonym)

/****************************************************************************************\
*          exchange-add operation for atomic operations on reference counters            *
\****************************************************************************************/

#ifdef CC_XADD
  // allow to use user-defined macro
#elif defined __GNUC__
#  if defined __clang__ && __clang_major__ >= 3 && !defined __ANDROID__ && !defined __EMSCRIPTEN__ && !defined(__CUDACC__)
#    ifdef __ATOMIC_ACQ_REL
#      define CC_XADD(addr, delta) __c11_atomic_fetch_add((_Atomic(int)*)(addr), delta, __ATOMIC_ACQ_REL)
#    else
#      define CC_XADD(addr, delta) __atomic_fetch_add((_Atomic(int)*)(addr), delta, 4)
#    endif
#  else
#    if defined __ATOMIC_ACQ_REL && !defined __clang__
       // version for gcc >= 4.7
#      define CC_XADD(addr, delta) (int)__atomic_fetch_add((unsigned*)(addr), (unsigned)(delta), __ATOMIC_ACQ_REL)
#    else
#      define CC_XADD(addr, delta) (int)__sync_fetch_and_add((unsigned*)(addr), (unsigned)(delta))
#    endif
#  endif
#elif defined _MSC_VER && _MSC_VER>1200 && !defined RC_INVOKED
#  include <intrin.h>
#  define CC_XADD(addr, delta) (int)_InterlockedExchangeAdd((long volatile*)addr, delta)
#else
   CC_INLINE CC_XADD(int* addr, int delta) { int tmp = *addr; *addr += delta; return tmp; }
#endif


/****************************************************************************************\
*                                  CC_NORETURN attribute                                 *
\****************************************************************************************/

#ifndef CC_NORETURN
#  if defined(__GNUC__)
#    define CC_NORETURN __attribute__((__noreturn__))
#  elif defined(_MSC_VER) && (_MSC_VER >= 1300)
#    define CC_NORETURN __declspec(noreturn)
#  else
#    define CC_NORETURN /* nothing by default */
#  endif
#endif


/****************************************************************************************\
*                                    C++ Move semantics                                  *
\****************************************************************************************/

#ifndef CC_CXX_MOVE_SEMANTICS
#  if __cplusplus >= 201103L || defined(__GXX_EXPERIMENTAL_CXX0X__) || defined(_MSC_VER) && _MSC_VER >= 1600
#    define CC_CXX_MOVE_SEMANTICS 1
#  elif defined(__clang)
#    if __has_feature(cxx_rvalue_references)
#      define CC_CXX_MOVE_SEMANTICS 1
#    endif
#  endif
#else
#  if CC_CXX_MOVE_SEMANTICS == 0
#    undef CC_CXX_MOVE_SEMANTICS
#  endif
#endif

#define C_CDECL
#define C_STDCALL
#define IPPI_CALL(x)  x
#if 0
typedef char *  myva_list;
#define myva_start(ap,v)  ( ap = ((char*)&v) + sizeof(v) )
#define myva_arg(ap,t)    ( *(t *)((ap += sizeof(t)) - sizeof(t)) )
#define myva_end(ap)      ( ap = (char*)0 )
#else
#if 0
#define myva_list va_list
#define myva_start(ap,v) va_start(ap,v)
#define myva_arg(ap,t) va_arg(ap,t)
#define myva_end(ap) va_end(ap)
#endif
#endif

#if defined(_DEBUG)
#if defined(_IRR_WINDOWS_API_) && defined(_MSC_VER) && !defined (_WIN32_WCE)
#if defined(WIN64) || defined(_WIN64) // using portable common solution for x64 configuration
#include <crtdbg.h>
#define _IRR_DEBUG_BREAK_IF( _CONDITION_ ) if (_CONDITION_) {_CrtDbgBreak();}
#else
#define _IRR_DEBUG_BREAK_IF( _CONDITION_ ) if (_CONDITION_) {_asm int 3}
#endif
#else
#include <assert.h>
#define _IRR_DEBUG_BREAK_IF( _CONDITION_ ) assert( !(_CONDITION_) );
#endif
#else
#define _IRR_DEBUG_BREAK_IF( _CONDITION_ )
#endif

#if defined _MSC_VER || defined __BORLANDC__
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
//typedef long long int int64;
//typedef unsigned long long int uint64;
#include <inttypes.h>
typedef int64_t int64;
typedef uint64_t uint64;
#endif
#define VOID void
typedef signed char schar;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned long dword;
typedef unsigned long ulong;

typedef char    CHAR;
typedef short   SHORT;
typedef long    LONG;
typedef byte    BYTE;
typedef dword   DWORD;
typedef word    WORD;
typedef uchar   UCHAR;
typedef ushort  USHORT;
typedef uint    UINT;
typedef ulong   ULONG;
typedef char    int8;
typedef short   int16;
typedef int     int32;
typedef uchar   uint8;
typedef ushort  uint16;
typedef uint    uint32;
typedef int16   INT16;
typedef int32   INT32;
typedef int64   INT64;
typedef uint16  UINT16;
typedef uint32  UINT32;
typedef uint64  UINT64;

typedef uint8  int8u;
typedef uint16 int16u;
typedef uint32 int32u;

typedef char   c8;
typedef uint8  u8;
typedef int8   s8;
typedef uint16 u16;
typedef int16  s16;
typedef uint32 u32;
typedef int32  s32;
typedef uint64  u64;
typedef int64   s64;
typedef float   f32;
typedef double  f64;

typedef char   c8_t;
typedef uint8  u8_t;
typedef int8   s8_t;
typedef uint16 u16_t;
typedef int16  s16_t;
typedef uint32 u32_t;
typedef int32  s32_t;
typedef uint64  u64_t;
typedef int64   s64_t;
typedef float   f32_t;
typedef double  f64_t;

typedef wchar_t wchar;
typedef wchar_t WCHAR;

#ifdef _MSC_VER
typedef signed char int8_t;
typedef uint8 uint8_t;
typedef int16 int16_t;
typedef uint16 uint16_t;
typedef int32 int32_t;
typedef uint32 uint32_t;
typedef int64 int64_t;
typedef uint64 uint64_t;
#endif
////////////////////////////////////////////////////////////////////////////////
    /* Types for `void *' pointers.  */  
#if __WORDSIZE == 64  
# ifndef __intptr_t_defined  
typedef long int               intptr_t;
#  define __intptr_t_defined  
# endif
typedef unsigned long int    uintptr_t;  
#else
# ifndef __intptr_t_defined
#if _MSC_VER <= 1200
typedef long                    intptr_t;  
#endif
#  define __intptr_t_defined  
# endif
//typedef unsigned int        uintptr_t;  
#endif
////////////////////////////////////////////////////////////////////////////////
typedef int64 lutime64_t;
////////////////////////////////////////////////////////////////////////////////
#ifndef __cplusplus
#define FALSE 0
#define TRUE 1
#define false 0
#define true 1

#define sqrtf(X) (f32)sqrt((f64)(X))
#define sinf(X) (f32)sin((f64)(X))
#define cosf(X) (f32)cos((f64)(X))
#define asinf(X) (f32)asin((f64)(X))
#define acosf(X) (f32)acos((f64)(X))
#define atan2f(X,Y) (f32)atan2((f64)(X),(f64)(Y))
#define ceilf(X) (f32)ceil((f64)(X))
#define floorf(X) (f32)floor((f64)(X))
#define powf(X,Y) (f32)pow((f64)(X),(f64)(Y))
#define fmodf(X,Y) (f32)fmod((f64)(X),(f64)(Y))
#define fabsf(X) (f32)fabs((f64)(X))
#define logf(X) (f32)log((f64)(X))
#define expf(X) (f32)exp((f64)(X))

#endif

typedef int int2[2];
typedef int int3[3];
typedef int int4[4];
typedef int int5[5];
typedef int int6[6];


typedef float float2[2];
typedef float float3[3];
typedef float float4[4];
typedef float float5[5];
typedef float float6[6];
typedef float float7[7];
typedef float float8[8];
typedef float float9[9];
typedef float float16[16];
typedef float float22[2][2];
typedef float float33[3][3];
typedef float float44[4][4];
typedef double double2[2];
typedef double double3[3];
typedef double double4[4];
typedef double double5[5];
typedef double double6[6];
typedef double double7[7];
typedef double double8[8];
typedef double double9[9];
typedef double double16[16];
typedef double double22[2][2];
typedef double double33[3][3];
typedef double double44[4][4];

typedef union suf16_t
{
    short i;
#if CC_FP16_TYPE
    __fp16 h;
#endif
    struct _fp16Format
    {
        unsigned int significand : 10;
        unsigned int exponent    : 5;
        unsigned int sign        : 1;
    } fmt;
}
suf16_t;


typedef union suf32_t {
  int32 i;
  uint32 u;
  float f;
  uchar c[4];
  struct _fp32Format
  {
      unsigned int significand : 23;
      unsigned int exponent    : 8;
      unsigned int sign        : 1;
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

CC_INLINE int cIsNaN(double value)
{
#if 1/*defined _MSC_VER || defined __BORLANDC__
    return _isnan(value);
#elif defined __GNUC__
    return isnan(value);
#else*/
    suf64_t ieee754;
    ieee754.f = value;
    return ((unsigned)(ieee754.u >> 32) & 0x7fffffff) +
           ((unsigned)ieee754.u != 0) > 0x7ff00000;
#endif
}


CC_INLINE int cIsInf(double value)
{
#if 1/*defined _MSC_VER || defined __BORLANDC__
    return !_finite(value);
#elif defined __GNUC__
    return isinf(value);
#else*/
    suf64_t ieee754;
    ieee754.f = value;
    return ((unsigned)(ieee754.u >> 32) & 0x7fffffff) == 0x7ff00000 &&
           (unsigned)ieee754.u == 0;
#endif
}


/* special informative macros for wrapper generators */
#define CC_EXPORTS_W CC_EXPORTS
#define CC_EXPORTS_W_SIMPLE CC_EXPORTS
#define CC_EXPORTS_AS(synonym) CC_EXPORTS
#define CC_EXPORTS_W_MAP CC_EXPORTS
#define CC_IN_OUT
#define CC_OUT
#define CC_PROP
#define CC_PROP_RW
#define CC_WRAP
#define CC_WRAP_AS(synonym)

typedef enum CStatus {
  CC_BADMEMBLOCK_ERR          = -113,
  CC_INPLACE_NOT_SUPPORTED_ERR = -112,
  CC_UNMATCHED_ROI_ERR        = -111,
  CC_NOTFOUND_ERR             = -110,
  CC_BADCONVERGENCE_ERR       = -109,
  CC_BADDEPTH_ERR             = -107,
  CC_BADROI_ERR               = -106,
  CC_BADHEADER_ERR            = -105,
  CC_UNMATCHED_FORMATS_ERR    = -104,
  CC_UNSUPPORTED_COI_ERR      = -103,
  CC_UNSUPPORTED_CHANNELS_ERR = -102,
  CC_UNSUPPORTED_DEPTH_ERR    = -101,
  CC_UNSUPPORTED_FORMAT_ERR   = -100,
  CC_BADARG_ERR      = -49,  //ipp comp
  CC_NOTDEFINED_ERR  = -48,  //ipp comp
  CC_BADCHANNELS_ERR = -47,  //ipp comp
  CC_BADRANGE_ERR    = -44,  //ipp comp
  CC_BADSTEP_ERR     = -29,  //ipp comp
  CC_BADFLAG_ERR     =  -12,
  CC_DIV_BY_ZERO_ERR =  -11, //ipp comp
  CC_BADCOEF_ERR     =  -10,
  CC_BADFACTOR_ERR   =  -7,
  CC_BADPOINT_ERR    =  -6,
  CC_BADSCALE_ERR    =  -4,
  CC_OUTOFMEM_ERR    =  -3,
  CC_NULLPTR_ERR     =  -2,
  CC_BADSIZE_ERR     =  -1,
  CC_FAIL     =  -1,
  CC_NO_ERR          =   0,
  CC_OK              =   CC_NO_ERR,
  
  CC_StsOk=                       0,  //!< everithing is ok
  CC_StsBackTrace=               -1,  //!< pseudo error for back trace
  CC_StsError=                   -2,  //!< unknown /unspecified error
  CC_StsInternal=                -3,  //!< internal error (bad state)
  CC_StsNoMem=                   -4,  //!< insufficient memory
  CC_StsBadArg=                  -5,  //!< function arg/param is bad
  CC_StsBadFunc=                 -6,  //!< unsupported function
  CC_StsNoConv=                  -7,  //!< iter. didn't converge
  CC_StsAutoTrace=               -8,  //!< tracing
  CC_HeaderIsNull=               -9,  //!< image header is NULL
  CC_BadImageSize=              -10,  //!< image size is invalid
  CC_BadOffset=                 -11,  //!< offset is invalid
  CC_BadDataPtr=                -12,  //!<
  CC_BadStep=                   -13,  //!<
  CC_BadModelOrChSeq=           -14,  //!<
  CC_BadNumChannels=            -15,  //!<
  CC_BadNumChannel1U=           -16,  //!<
  CC_BadDepth=                  -17,  //!<
  CC_BadAlphaChannel=           -18,  //!<
  CC_BadOrder=                  -19,  //!<
  CC_BadOrigin=                 -20,  //!<
  CC_BadAlign=                  -21,  //!<
  CC_BadCallBack=               -22,  //!<
  CC_BadTileSize=               -23,  //!<
  CC_BadCOI=                    -24,  //!<
  CC_BadROISize=                -25,  //!<
  CC_MaskIsTiled=               -26,  //!<
  CC_StsNullPtr=                -27,  //!< null pointer
  CC_StsVecLengthErr=           -28,  //!< incorrect vector length
  CC_StsFilterStructContentErr= -29,  //!< incorr. filter structure content
  CC_StsKernelStructContentErr= -30,  //!< incorr. transform kernel content
  CC_StsFilterOffsetErr=        -31,  //!< incorrect filter ofset value
  CC_StsBadSize=                -201, //!< the input/output structure size is incorrect
  CC_StsDivByZero=              -202, //!< division by zero
  CC_StsInplaceNotSupported=    -203, //!< in-place operation is not supported
  CC_StsObjectNotFound=         -204, //!< request can't be completed
  CC_StsUnmatchedFormats=       -205, //!< formats of input/output arrays differ
  CC_StsBadFlag=                -206, //!< flag is wrong or not supported
  CC_StsBadPoint=               -207, //!< bad CPoint
  CC_StsBadMask=                -208, //!< bad format of mask (neither 8uC1 nor 8sC1)
  CC_StsUnmatchedSizes=         -209, //!< sizes of input/output structures do not match
  CC_StsUnsupportedFormat=      -210, //!< the data format/type is not supported by the function
  CC_StsOutOfRange=             -211, //!< some of parameters are out of range
  CC_StsParseError=             -212, //!< invalid syntax/structure of the parsed file
  CC_StsNotImplemented=         -213, //!< the requested function/feature is not implemented
  CC_StsBadMemBlock=            -214, //!< an allocated block has been corrupted
  CC_StsAssert=                 -215, //!< assertion failed
  CC_GpuNotSupported=           -216,
  CC_GpuApiCallError=           -217,
  CC_OpenGlNotSupported=        -218,
  CC_OpenGlApiCallError=        -219,
  CC_OpenCLApiCallError=        -220,
  CC_OpenCLDoubleNotSupported=  -221,
  CC_OpenCLInitError=           -222,
  CC_OpenCLNoAMDBlasFft=        -223
} CStatus;


#define PIXFMTDEF_DEF(PIXFMTDEF) \
  PIXFMTDEF( 1, 1, Indexed, PixelFormatIndexed) \
  PIXFMTDEF( 2, 4, Indexed, PixelFormatIndexed) \
  PIXFMTDEF( 3, 8, Indexed, PixelFormatIndexed) \
  PIXFMTDEF( 4, 1, Gray, 0) \
  PIXFMTDEF( 5, 4, Gray, 0) \
  PIXFMTDEF( 6, 8, Gray, PixelFormatExtended) \
  PIXFMTDEF( 7, 16, Gray, 0) \
  PIXFMTDEF( 8, 32, GrayFloat, 0) \
  PIXFMTDEF( 9, 64, GrayDouble, 0) \
  PIXFMTDEF(10, 16, RGB555, 0) \
  PIXFMTDEF(11, 16, RGB565, 0) \
  PIXFMTDEF(12, 16, ARGB1555, PixelFormatAlpha) \
  PIXFMTDEF(13, 24, RGB, 0) \
  PIXFMTDEF(14, 32, RGB, 0) \
  PIXFMTDEF(15, 32, ARGB, PixelFormatAlpha | PixelFormatCanonical) \
  PIXFMTDEF(16, 32, PARGB, PixelFormatAlpha | PixelFormatPAlpha) \
  PIXFMTDEF(17, 48, RGB, PixelFormatExtended) \
  PIXFMTDEF(18, 64, ARGB, PixelFormatAlpha | PixelFormatCanonical | PixelFormatExtended) \
  PIXFMTDEF(19, 64, PARGB, PixelFormatAlpha | PixelFormatPAlpha | PixelFormatExtended) \
  PIXFMTDEF(20, 128, ARGBFloat, PixelFormatAlpha | PixelFormatCanonical | PixelFormatExtended) \
  PIXFMTDEF(21, 128, ABGRFloat, PixelFormatAlpha | PixelFormatCanonical | PixelFormatExtended) \
  PIXFMTDEF(22, 96, BGRFloat, PixelFormatAlpha | PixelFormatCanonical | PixelFormatExtended) \

typedef enum {
  PixelFormatIndexed  = 0x00010000, // Indexes into a palette
    PixelFormatAlpha    = 0x00040000, // Has an alpha component
    PixelFormatPAlpha   = 0x00080000, // Pre-multiplied alpha
    PixelFormatExtended = 0x00100000, // Extended color 16 bits/channel
    PixelFormatCanonical = 0x00200000,
    PixelFormatUndefined = 0,
    PixelFormatDontCare  = 0,
    
#define PIXFMTDEF(_ID, _B, _NAME, _FLAG)  PF_ ## _B ##bpp## _NAME  = (_ID) | (_B<<8)|(_FLAG), PixelFormat ## _B ##bpp## _NAME = PF_ ## _B ##bpp## _NAME,
    PIXFMTDEF_DEF(PIXFMTDEF)
    PixFmtMax=23,
#undef PIXFMTDEF
    PixelFormat16bppGrayScale = PixelFormat16bppGray
} PixFmt;
typedef PixFmt PixelFormat;

#define PixelFormatBpp(pf)   (pf>>8)&0xff

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
  CC_NUL=-1,
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
typedef enum {
#define TDD(a,b,c)  a##C##c=_MAKETYPECN(a, c)
#define TYPEDEF(a,b,c,d)  TDD(a,b,1),TDD(a,b,2),TDD(a,b,3),TDD(a,b,4),
TYPEDEF_DEF(TYPEDEF)
#undef TYPEDEF
} TypeCnId;


CC_INLINE int cvTypeSize(TypeId type) {
  static int _type_size_tab[16] = {0};
  if (0==_type_size_tab[CC_8U]) {
#define TYPEDEF(a,b,c,d)  _type_size_tab[a]=c;
    TYPEDEF_DEF(TYPEDEF);
#undef TYPEDEF
  }
  return _type_size_tab[type];
}

CC_INLINE char* cvTypeName(TypeId type) {
  static char* _type_name_tab[16] = {0};
  if (NULL==_type_name_tab[CC_8U]) {
#define TYPEDEF(a,b,c,d)  _type_name_tab[a]= (char*)#a ;
    TYPEDEF_DEF(TYPEDEF);
#undef TYPEDEF
  }
  return _type_name_tab[type];
}

#define CC_TYPE_SIZE(type)      cvTypeSize(type)
#define CC_TYPE_NAME(type)      cvTypeName(type)


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
  int i=0;
  if (NULL==dst) {}
  switch ((dst_type<<8) | src_type) {
#define TYPECVTDEF(a, b, c, d, e, f, g, h)  case ((a<<8) | e): { d* _d = (d*)dst; const h* _s = (const h*) src; \
    for (; i<n-3; i+=4) {_d[i] = (d)_s[i];_d[i+1] = (d)_s[i+1];_d[i+2] = (d)_s[i+2];_d[i+3] = (d)_s[i+3]; } for (; i<n; ++i) {_d[i] = (d)_s[i]; } } break;
    TYPECVTDEF_DEF(TYPECVTDEF)
#undef TYPECVTDEF
  }
  return dst;
}
static void* arrcvt2d(void* dst, TypeId dsttype, int dl, const void* src, TypeId srctype, int sl, int h, int w) {
  char* d = (char*)(dst);
  const char* s = (const char*)(src);
  int dw = w*(dsttype>>16);
  int sw = w*(srctype>>16);
  dl = dl>0 ? dl : dw;
  sl = sl>0 ? sl : sw;
  if (dl == dw && sl == sw) {
    return arrcvt(dst, dsttype, src, srctype, h * w);
  }
  for (; h--; s += sl, d += dl) {
    arrcvt(d, dsttype, s, srctype, w);
  }
  return dst;
}


#define CC_DEPTH_MAX  (1 << CC_CN_SHIFT)
#define CC_MAT_DEPTH_MASK       (CC_DEPTH_MAX - 1)
//#define CC_MAKE_TYPE CC_MAKETYPE
#define CC_AUTO_STEP  0x7fffffff
#define CC_WHOLE_ARR  cvSlice( 0, 0x3fffffff )
#define CC_MAT_CN(im)             ((im)->c)/CC_TYPE_SIZE((im->tid))
#define CC_MAT_NEEDED(mat)        (1)
#define CC_MAKETYPECN(depth,cn)   ((TypeCnId)_MAKETYPECN(depth,cn))
#define CC_MAT_TYPECN(mat)        CC_MAKETYPECN(mat->tid, CC_MAT_CN(mat))
#define CC_MAT_TYPE(mat)          CC_MAT_TYPECN(mat)
#define CC_MAT_TOTAL(mat)         ((mat)->rows * (mat)->cols)
#define CC_MAT_DEPTH(mat)       (mat)->tid
#define CC_MAT_ELEM( mat, elemtype, row, col ) *img_at(elemtype, mat, row, col)
#define CC_MAT_CONT_FLAG        (0xff)
#define CC_IS_MAT_CONT(im)    ((im)->w*(im)->c==(im)->s)
#define CC_IS_CONT_MAT          CC_IS_MAT_CONT
#define CC_MAT_TEMP_FLAG_SHIFT  15
#define CC_MAT_TEMP_FLAG        (1 << CC_MAT_TEMP_FLAG_SHIFT)
#define CC_IS_TEMP_MAT(flags)   ((flags) & CC_MAT_TEMP_FLAG)
#define CC_IS_MASK_ARR(mat)     ( (mat)->c ==1 )
#define CC_ARE_TYPES_EQ(mat1, mat2)   ( (mat1)->tid == (mat2)->tid && (mat1)->c == (mat2)->c )
#define CC_ARE_DEPTHS_EQ(mat1, mat2)  ( (mat1)->tid == (mat2)->tid )
#define CC_ARE_CNS_EQ(mat1, mat2)     ( CC_MAT_CN(mat1) == CC_MAT_CN(mat2) )
#define CC_ARE_SIZES_EQ(mat1, mat2)   ( (mat1)->rows == (mat2)->rows && (mat1)->cols == (mat2)->cols )
#define CC_ARE_SIZES_EQSZ(mat1, _rows, _cols)   ( (mat1)->rows == _rows && (mat1)->cols == _cols )
#define CC_ARE_SIZES_EQSZ1(mat1, _cols, _rows)   ( (mat1)->rows == _rows && (mat1)->cols == _cols )
#define CC_ARE_SIZES_EQSZ2(mat1, _rows, _cols)   ( CC_ARE_SIZES_EQSZ(mat1, _rows, _cols) || CC_ARE_SIZES_EQSZ(mat1, _cols, _rows) )
#define CC_IS_MAT_CONST(mat)   (((mat)->rows|(mat)->cols) == 1)
/* Size of each channel item,
   0x124489 = 1000 0100 0100 0010 0010 0001 0001 ~ array of sizeof(arr_type_elem) */
//#define CC_ELEM_SIZE1(mat)   cvTypeSize(mat->tid)
#define CC_ELEMTYPE_SIZE(mat)   cvTypeSize(mat->tid)
#define CC_ELEM_SIZE_(mat)      mat->c
//#define CC_ELEM_SIZE(type)   (CC_ELEM_SIZE1(mat)*(mat)->c)
/* 0x3a50 = 11 10 10 01 01 00 00 ~ array of log2(sizeof(arr_type_elem)) */
#define CC_MAT_ELEM_PTR( mat, row, col )   CC_MAT_ELEM_PTR_FAST( mat, row, col, CC_ELEM_SIZE((mat).type) )
//#define CC_MAT_ELEM( mat, elemtype, row, col )   (*(elemtype*)CC_MAT_ELEM_PTR_FAST( mat, row, col, sizeof(elemtype)))
//         Multi-dimensional dense array (CMatND)                          *
#define CC_MATND_MAGIC_VAL    0x42430000
#define CC_TYPE_NAME_MATND    "opencv-nd-matrix"
#define CC_MAX_DIM            32
#define CC_MAX_DIM_HEAP       (1 << 16)
#define CC_IS_MATND_HDR(mat) \
  ((mat) != NULL && (((const CMatND*)(mat))->type & CC_MAGIC_MASK) == CC_MATND_MAGIC_VAL)
//#define CC_IS_MATND(mat)   (CC_IS_MATND_HDR(mat) && ((const CMatND*)(mat))->tt.data.ptr != NULL)
//        Multi-dimensional sparse array (CSparseMat)                      *
#define CC_SPARSE_MAT_MAGIC_VAL    0x42440000
#define CC_TYPE_NAME_SPARSE_MAT    "opencv-sparse-matrix"
#define CC_IS_SPARSE_MAT_HDR(mat) \
  ((mat) != NULL && \
      (((const CSparseMat*)(mat))->type & CC_MAGIC_MASK) == CC_SPARSE_MAT_MAGIC_VAL)


typedef struct {
  //IMGHEADFEILD;
  union { int w;    int width; int cols; };
  union { int h;    int height; int rows; };
  union { int s;    int step; int widthStep; };
  union { int c;    int cn; int ch; };
  union { int f;    int frame; };
  union { TypeId t;    TypeId type;  TypeId tid; };
#if 1
  union {
  union {
    uchar* data;
	uchar* ptr;
    short* s;
    int* i;
	float* fl;
	double* db;
#define TYPEDEF(a,b,c,d)  d* b;
    TYPEDEF_DEF(TYPEDEF)
#undef TYPEDEF
  } tt;
  union {
	  float* fl;
	  double* db;
	  int* i32;
#define TYPEDEF(a,b,c,d)  d* b;
	  TYPEDEF_DEF(TYPEDEF)
#undef TYPEDEF
  };
  uchar* data;
  uchar* imageData;
  };
#else
  uchar* data;
#endif
#ifdef __cplusplus
  int channels() const { return CC_MAT_CN(this);  }
#endif
} img_t;
typedef img_t imgs_t;
typedef img_t imgi_t;
typedef img_t imgd_t;
typedef img_t imgf_t;
typedef struct bitmap_t_ bitmap_t;
typedef struct bitmap_t_ texture_t;
struct bitmap_t_ {
  int h, w, s;
  PixFmt fmt;
  uchar* data;
  int (*free_tex)(texture_t* tex);
};

#define img_at_(type, img, row, col)          ((type*)((img)->tt.data + (row)*(img)->s + (col)*(img)->c))
#define img_row_(type, img, row)              ((type*)((img)->tt.data + (row)*(img)->s))
#define img_ptr_(type, img)                   ((type*)((img)->tt.data))
#if 0
#define img_ptr(type, img)            (ASSERT(cc_##type == (img)->type), (type*)(img)->tt.data)
#define img_end(type, img)            ((type*)(img)->tt.data+(img)->h*(img)->s)
#define img_row(type, img, row)       ((type*)((img)->tt.data + (row)*(img)->s))
#define img_at(type, img, row, col)   ((type*)((img)->tt.data + (row)*(img)->s + (col)*(img)->c))
#else
#ifdef __cplusplus
template <typename T> struct TypeIdTrais {enum {id = CC_8U};};
//#define TYPEDEF(a, b, c, d) template <> struct type_trais<d> {enum {type = a};}
#define TYPEDEF(A, B, C, D) template <> struct TypeIdTrais<D> {enum {id = A};}; CC_INLINE void* img_ptr_impl(const img_t* img, int row, int col, D ) {assert(A==img->tid);return img->tt.data + (row)*(img)->s + (col)*(img)->c; }
TYPEDEF_DEF(TYPEDEF)
#undef TYPEDEF
#define img_at(type, img, row, col)   ((type*)img_ptr_impl(img, row, col, type() ))
#else
CC_INLINE void* img_ptr_impl(const img_t* img, int row, int col, TypeId tid) {assert(tid==img->tid);return img->tt.data + (row)*(img)->s + (col)*(img)->c; }
#define img_at(type, img, row, col)   ((type*)img_ptr_impl(img, row, col, cc_##type ))
#endif
#define img_ptr(type, img)            (img_at(type, img, 0,0 ))
#define img_end(type, img)            (img_at(type, img, img->h, 0 ))
#define img_row(type, img, row)       (img_at(type, img, row, 0 ))
#endif
#define IMINIT(IM, H, W, A, S, C, F)  ((IM)->h=H,(IM)->w=W,(IM)->tt.data=(uchar*)(A),(IM)->s=S,(IM)->c=(short)(C),(IM)->f=F)

//int imsavetxt(const char* filename, const img_t* im, const char* fmt);
#define BMPINIT(bmp, _H, _W, _DATA, _STEP, _BPP)  ((bmp)->h=_H, (bmp)->w=_W, (bmp)->s=_STEP, (bmp)->data=_DATA, (bmp)->fmt = bpp2PixFmt(_BPP))
#define BMPINITIM(bmp, im)  ((bmp)->h=(im)->h, (bmp)->w=(im)->w, (bmp)->s=(im)->s, (bmp)->data=(im)->tt.data, (bmp)->fmt=cn2PixFmt((im)->c))

typedef struct buf_t {
  int len;
  unsigned char* data;
} buf_t;

typedef enum {
    CC_EXPR_EQS, 
    CC_EXPR_EQ, 
    CC_EXPR_GTS, 
    CC_EXPR_GT, 
    CC_EXPR_GES, 
    CC_EXPR_GE, 
    CC_EXPR_LTS, 
    CC_EXPR_LT, 
    CC_EXPR_LES, 
    CC_EXPR_LE, 
    CC_EXPR_NES, 
    CC_EXPR_NE, 
    CC_EXPR_ADDS, 
    CC_EXPR_ADD, 
    CC_EXPR_SUBS, 
    CC_EXPR_SUB, 
    CC_EXPR_MULS, 
    CC_EXPR_MUL, 
    CC_EXPR_DIVS, 
    CC_EXPR_DIV, 
    CC_EXPR_FREE, 
    CC_EXPR_COPY, 
}ExprType;


#ifdef __cplusplus
template <typename T>
struct matx_opt {
  static void init(int n, T* a) {
    for(int i = 0; i < n; ++i) a[i] = T(0);
  }
  static void eye(int n, T* a, int step) {
    int i;
    for (i=0; i<n; ++i) a[i*step + i] = 1;
  }
  static void transpose(int h, int w, const T* src, int src_step, T* dst, int dst_step) {
    int i, j;
    for (i=0; i<h; ++i) {
      for (j=0; j<w; ++j) {
        dst[i + j*dst_step] = src[i*src_step + i];
      }
    }
  }
  static int matmul(const T* a, int al, const T* b, int bl, T* c, int cl, int h, int w, int n) {
    int i, j, k;
    for(i=0;i<h;++i) {
      const T* a0 = a+i*al;
      T* c0 = c+i*cl;
      for(j=0;j<w;++j) {
        const T* b0 = b+j;
        T s0=0, s1=0;
        for(k=0;k<w;k+=2) {
          int bidx = k*bl;
          s0 += a0[k] * b0[bidx];
          s1 += a0[k+1] * b0[bidx+al];
        }
        if (w&1) {s0 += a0[w-1]*b0[(w-1)*bl];}
        c0[j] = s0+s1;
      }
    }
    return 0;
  }

};
template <typename T, int H, int W>
struct matx {
  T val[H*W];
  enum {shortdim = (H < W ? H : W), len=H*W, rows=H, cols=W, step=W, step1 = W*sizeof(T)};
  typedef matx<T, H, W> self;
  typedef matx<T, W, H> tself;
  typedef matx_opt<T> opt;
  matx(T a0=0, T a1=0, T a2=0, T a3=0, T a4=0, T a5=0, T a6=0, T a7=0, T a8=0) {
    T a[] = {a0, a1, a2, a3, a4, a5, a6, a7, a8};
    int i, n = 9 < len ? 9 : len;
    for (i=0; i<n; ++i) {
      val[i] = a[i];
    }
    for (; i<len; ++i) {
      val[i] = 0;
    }
  }
  matx(const self& o) {memcpy(this, &o, sizeof(self));}
  self& operator = (const self& o) {memcpy(this, &o, sizeof(self));return *this;}
  T& operator[](int i) { return val[i]; }
  const T& operator[](int i) const { return val[i]; }
  T& operator()(int i) { return val[i]; }
  const T& operator()(int i) const { return val[i]; }
  T& operator()(int row, int col) { return val[row*step+col]; }
  const T& operator()(int row, int col) const { return val[row*step + col]; }
  static self eye() { self c; opt::eye(shortdim, c.val, W); return c; }
  tself t() const {tself c; opt::transpose(H, W, val, W, c.val, H); return c;}
  template <int K> matx<T, H, K> mul2(const matx<T, W, K>& b) {
    matx<T, H, K> c;
    //opt::matmul(val, W, b.val, K, c.val, K, H, K, W);
    return c;
  }
  template <int K> inline self& matmul(const matx<T, H, K>& a, const matx<T, K, W>& b) {
#if 0
    for( int i = 0; i < H; i++ ) {
      for( int j = 0; j < W; j++ ) {
        T s = 0;
        for( int k = 0; k < K; k++ )
          s += a.val[i*K+k] * b.val[k*W+j];
        val[i*W + j] = s;
      }
    }
#endif
    return *this;
  }
};

typedef matx<float, 2, 2> matx22f;
typedef matx<float, 2, 3> matx23f;
typedef matx<float, 3, 3> matx33f;
typedef matx<float, 4, 4> matx44f;

typedef matx<double, 2, 2> matx22d;
typedef matx<double, 2, 3> matx23d;
typedef matx<double, 3, 3> matx33d;
typedef matx<double, 4, 4> matx44d;

typedef matx<uchar, 2, 1> vec2b;
typedef matx<uchar, 3, 1> vec3b;
typedef matx<uchar, 4, 1> vec4b;
typedef matx<uchar, 4, 1> vec4b;
typedef matx<uchar, 5, 1> vec5b;
typedef matx<uchar, 6, 1> vec6b;

typedef matx<short, 2, 1> vec2s;
typedef matx<short, 3, 1> vec3s;
typedef matx<short, 4, 1> vec4s;
typedef matx<short, 4, 1> vec4s;
typedef matx<short, 5, 1> vec5s;
typedef matx<short, 6, 1> vec6s;

typedef matx<int, 2, 1> vec2i;
typedef matx<int, 3, 1> vec3i;
typedef matx<int, 4, 1> vec4i;
typedef matx<int, 4, 1> vec4i;
typedef matx<int, 5, 1> vec5i;
typedef matx<int, 6, 1> vec6i;
typedef matx<int, 7, 1> vec7i;
typedef matx<int, 8, 1> vec8i;

typedef matx<double, 2, 1> vec2d;
typedef matx<double, 3, 1> vec3d;
typedef matx<double, 4, 1> vec4d;
typedef matx<double, 4, 1> vec4d;
typedef matx<double, 5, 1> vec5d;
typedef matx<double, 6, 1> vec6d;

#else
typedef struct {uchar v[2];} vec2b;
typedef struct {uchar v[3];} vec3b;
typedef struct {uchar v[4];} vec4b;

typedef struct {short v[2];} vec2s;
typedef struct {short v[3];} vec3s;
typedef struct {short v[4];} vec4s;

typedef struct {ushort v[2];} vec2w;
typedef struct {ushort v[3];} vec3w;
typedef struct {ushort v[4];} vec4w;

typedef struct {int v[2];} vec2i;
typedef struct {int v[3];} vec3i;
typedef struct {int v[4];} vec4i;
typedef struct {int v[6];} vec6i;
typedef struct {int v[8];} vec8i;

typedef struct {float v[2];} vec2f;
typedef struct {float v[3];} vec3f;
typedef struct {float v[4];} vec4f;
typedef struct {float v[6];} vec6f;

typedef struct {double v[2];} vec2d;
typedef struct {double v[3];} vec3d;
typedef struct {double v[4];} vec4d;
typedef struct {double v[6];} vec6d;
#endif


#define CC_IS_MAT1(im)    (1)
#define CC_IS_MATND1(im)  (0)
#define CC_OCL_RUN(a, b)  (0);
#define CC_IPP_RUN_FAST(a)  (0);
#define CC_OVX_RUN(a, b)  (0);
#define CC_IPP_RUN(a, b, c)  (0);

#define CVAUX_STR(x)  #x
#define CC_INSTRUMENT_REGION()
#define CC_ENABLE_UNROLLED 1

////////////////////////////////////////////////////////////////////////////////
#define CC_DOUBLE_MAX   DBL_MAX
#define CC_INT32_MAX    INT_MAX
#define CC_UINT32_MAX   UINT_MAX
#define CC_INT64_MIN     (-9223372036854775807 - 1)
#define CC_INT64_MAX     9223372036854775807
#define CC_UINT64_MAX    0xFFFFFFFFFFFFFFFF

#define CC_DB_MAX      DBL_MAX
#define CC_FL_MAX      FLT_MAX
#define CC_I32_MAX     INT_MAX
#define CC_U32_MAX     UINT_MAX
#define CC_I64_MIN     (-9223372036854775807 - 1)
#define CC_I64_MAX     9223372036854775807
#define CC_U64_MAX     0xFFFFFFFFFFFFFFFF

////////////////////////////////////////////////////////////////////////////////
#define QUEUE_PUSH(Q, N, I, X) if (N<countof(Q)) {Q[I=N++]=X;} else {Q[(I++)%(N=countof(Q))]=X;}
#define QUEUE_SUM(Q, N, S)  {int _i=0; for (; _i<(N); ++_i) {S+=Q[_i];}}

#ifndef CC_SHIFT
#define CC_SHIFT                 (13)
#define CC_SHIFT1                (1<<CC_SHIFT)
#define CC_SHIFT_MARK            (CC_SHIFT1 - 1)
#define CC_SHIFT_MUL(x, y)       (((x)*(y))>>CC_SHIFT)
#define CC_SHIFT_DIV(x, y)       (((int)(x)<<CC_SHIFT)/(y))
#define CC_SHIFT_TOFLOAT(x)      (((double)(x))/CC_SHIFT1)
#define CC_SHIFT_TOINT(x)        (int)((x)*CC_SHIFT1)
#define CC_SHIFT_TOINT2(x, _S)   (int)((x)*(1<<(_S)))
#define CC_HALFSHIFT1            (1<<12)
#endif
////////////////////////////////////////////////////////////////////////////////
#define LIST_PUSHBACK(list, item, tmp) if (list) { tmp = list; while(tmp->next) { tmp = tmp->next; } tmp->next = item; } else { list = item; }
#define LIST_PUSHBACK_T(list, item, T) if (list) { T* tmp = list; while(tmp->next) { tmp = tmp->next; } tmp->next = item; } else { list = item; }
#define LIST_POPFRONT(list, item) { item = list; if (list) { list = list->next; } }
#define LIST_ADDFRONT1(list, item) if (list) { item->next = list; list = item; } else { item->next = NULL; list = item; }
#define LIST_ADDFRONT(list, item) if (list) { item->prev = NULL; item->next = list; list->prev = item; list = item; } else { item->prev = item->next = NULL; list = item; }
#define LIST_DEL(list, item) if (list==item) { list->prev = NULL; list = item->next; } else { if (item->prev) { item->prev->next = item->next; } if (item->next) { item->next->prev = item->prev; } }
////////////////////////////////////////////////////////////////////////////////

//#define if_c_a_else_b(c, a, b)   ((c) ? (a) : (b))
//! creates four CC codes used in Irrlicht for simple ids
#define MAKE_IRR_ID(c0, c1, c2, c3) ((u32)(u8)(c0) | ((u32)(u8)(c1) << 8) | ((u32)(u8)(c2) << 16) | ((u32)(u8)(c3) << 24 ))

#define ccInvSqrt(value) ((float)(1./sqrt(value)))
#define ccSqrt(value)  ((float)sqrt(value))

//   Array allocation, deallocation, initialization and access to elements         *
#define CC_MAX_ARR 10
#define CC_NO_DEPTH_CHECK     1
#define CC_NO_CN_CHECK        2
#define CC_NO_SIZE_CHECK      4
#define cvCvtScaleAbs  cvConvertScaleAbs
//     Arithmetic, logic and comparison operations                          *
typedef enum {
 CC_CMP_EQ = 0, // ==
 CC_CMP_GT = 1, // >
 CC_CMP_GE = 2, // >=
 CC_CMP_LT = 3, // <
 CC_CMP_LE = 4, // <=
 CC_CMP_NE = 5, // !=
}CCmpType;
//                  Math operations                                         *
/* Fast cubic root calculation */
//CVAPI( float ) cvCbrt( float value );
/* Checks array values for NaNs, Infs or simply for too large numbers
   (if CC_CHECK_RANGE is set). If CC_CHECK_QUIET is set,
   no runtime errors is raised (function returns zero value in case of "bad" values).
   Otherwise cvError is called */
#define CC_CHECK_RANGE    1
#define CC_CHECK_QUIET    2
typedef enum {
 CC_RAND_UNI     = 0, // UNIFORM
 CC_RAND_NORMAL  = 1,
} CRandType;
//                  Matrix operations                                       *
/* Matrix transform: dst = A*B + C, C is optional */
#define CC_GEMM_A_T 1
#define CC_GEMM_B_T 2
#define CC_GEMM_C_T 4
#define CC_GEMM_D_T 8
#define CC_GEMM_COMPLEX 16

//                Dynamic data structures                                   *
/* Calculates length of sequence slice (with support of negative indices). */
//CVAPI( int ) cvSliceLength( CvSlice slice, const CvSeq * seq );
#define CC_FRONT 1
#define CC_BACK 0
/* Retrieves graph vertex by given index */
#define cvGetGraphVtx( graph, idx ) (CvGraphVtx*)cvGetSetElem((CvSet*)(graph), (idx))
/* Retrieves index of a graph edge given its pointer */
#define CC_GRAPH_VERTEX        1
#define CC_GRAPH_TREE_EDGE     2
#define CC_GRAPH_BACK_EDGE     4
#define CC_GRAPH_FORWARD_EDGE  8
#define CC_GRAPH_CROSS_EDGE    16
#define CC_GRAPH_ANY_EDGE      30
#define CC_GRAPH_NEW_TREE      32
#define CC_GRAPH_BACKTRACKING  64
#define CC_GRAPH_OVER          -1
#define CC_GRAPH_ALL_ITEMS    -1
/* flags for graph vertices and edges */
#define CC_GRAPH_ITEM_VISITED_FLAG  (1 << 30)
#define CC_IS_GRAPH_VERTEX_VISITED(vtx) \
  (((CvGraphVtx*)(vtx))->flags & CC_GRAPH_ITEM_VISITED_FLAG)
#define CC_IS_GRAPH_EDGE_VISITED(edge) \
  (((CvGraphEdge*)(edge))->flags & CC_GRAPH_ITEM_VISITED_FLAG)
#define CC_GRAPH_SEARCH_TREE_NODE_FLAG   (1 << 29)
#define CC_GRAPH_FORWARD_EDGE_FLAG       (1 << 28)
//                       Drawing                                            *
//#define CC_RGB( r, g, b )  cvScalar( (b), (g), (r), 0 )
//! type of line
enum LineTypes {
    CC_FILLED  = -1,
    CC_LINE_4  = 4, //!< 4-connected line
    CC_LINE_8  = 8, //!< 8-connected line
    CC_LINE_AA = 16 //!< antialiased line
};


//                      System functions                                    *
#define CC_ErrModeLeaf     0   /* Print error and exit program */
#define CC_ErrModeParent   1   /* Print error and continue */
#define CC_ErrModeSilent   2   /* Don't print and continue */

// 用宏判断计算机处理器是大端字节序
static union { char c[4]; unsigned long mylong; } endian_test = {{ 'l', '?', '?', 'b' } };
#define ENDIANNESS ((char)endian_test.mylong)
#define BIGENDIANNESS ('b'==ENDIANNESS)

typedef BYTE* LPBYTE;
//#pragma warning( disable: 4127 )
#ifndef UNUSED
#define UNUSED(__X)  { const void* _P0 = &(__X); _P0=&_P0; }
//#define UNUSED(x) (void)x 
#endif
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

#if 0
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#define LOWORD(l)           ((WORD)(l))
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((BYTE)(w))
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))
#endif

#define LOGPRINTFTOBUF(buf, format, TIME) \
int len=0; \
va_list ap; \
if (TIME) {len += _snprintf(buf+len, countof(buf)-len, "%s: ", _strtime2()); } \
va_start(ap, format); \
len = _vsnprintf(buf+len, countof(buf)-len, format, ap); \
va_end(ap);

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2 (M_PI*2)
#endif // M_PI_2
#define ccAlloc      pmalloc
#define ccFree(_x)   (pfree(*(_x)), *(_x)=0)
#if (defined _WIN32 || defined _WIN64)
#define snprintf     _snprintf
#define vsnprintf     _vsnprintf
#else
//#define _snprintf   str_nprintf
#define _snprintf   snprintf
#define _vsnprintf   vsnprintf
#define _access access
#define stricmp strcasecmp
#define _stricmp strcasecmp
#define strnicmp strncasecmp
#define _strnicmp strncasecmp
//#define GetLastError() errno
#endif
typedef struct rgb_color {
  BYTE r, g, b;
} rgb_color;
//base type
//typedef unsigned char   uchar;
#define NEW(T, _n)    ((T*)pmalloc(sizeof(T)*(_n)))
#define free_s(_p)   ((_p!=NULL)?(pfree((void*)(_p)), _p=0):(0))
#define MAXALGNUM (100)
//#define CLIP(_X, _MIN, _MAX) (MIN(_MAX, MAX(_MIN, _X)))
#define DegreeToRadian(degree)   ((PI / 180) * degree)
#define lengthof(x) (sizeof(x)/sizeof(*x))
//#define MAX max
//#define MIN min
//#define CLAMP(x,a,b)    (MIN(b,MAX(a,x)))
#define CLAMP(_X, _MINVAL, _MAX_VAL)  ((_X)<(_MINVAL) ? (_MINVAL) : ((_X)>(_MAX_VAL) ? (_MAX_VAL) : (_X)))
#define CLAMP255(_X)    CLAMP(_X, 0, 255)
//#define CLAMP255(x)        ((x)<0?0:((x)>255)?255:(x))
#define CC_AA 16
enum { XY_SHIFT = 16, XY_ONE = 1 << XY_SHIFT, DRAWING_STORAGE_BLOCK = (1<<12) - 256 };

typedef struct sendbuf_t_ sendbuf_t;
struct sendbuf_t_ {
  sendbuf_t* next;
  int len;
  char data[1];
};

static uint64 db2u64(double db) {
  suf64_t tf;
  tf.f = db;
#if defined _WIN32 || defined _WIN64
  return tf.u;
#else
  return (tf.u<<32) | (tf.u>>32);
#endif
}

///////////////////////////////////////////////////////////////////////////

#define FILELINE  printf("%s:%d\n", GetFileNameExt(__FILE__), __LINE__)

//! Possible log levels.
typedef enum ELOG_LEVEL {
  //! Used for printing information helpful in debugging
  ELL_DEBUG,
    //! Useful information to print. For example hardware infos or something started/stopped.
    ELL_INFORMATION,
    //! Warnings that something isn't as expected and can cause oddities
    ELL_WARNING,
    //! Something did go wrong.
    ELL_ERROR,
    //! Logs with ELL_NONE will never be filtered.
    //! And used as filter it will remove all logging except ELL_NONE messages.
    ELL_NONE,
    ELL_SET_FILE,
    ELL_SET_ECHO,
    ELL_SET_USER,
    ELL_SET_TIME,
} ELOG_LEVEL;
typedef struct loguser_t {
  int (*logg)(ELOG_LEVEL level, const char* str);
} loguser_t;
// logprintf
typedef struct tm tm_t;

typedef struct {
  uint year; ///< 1601 - 30827
  uchar month; ///< 1-12
  uchar dayofweek; 
  uchar day;///< 1-31
  uchar hour;///<[0-23]
  uchar minute;///<[0-59]
  uchar second;///<[0-61], the range of [60, 61] is used for leap seconds
  ushort millis;
} sys_time_t;
// y - year
// m - months
// d - day
// h - hour
// M - minutes
// s - seconds
static char* strtimefmt(char* buf, size_t len, const char* fmt) {
  char c;
  size_t j=0;
  char fmt1[32];
  time_t timep;
  struct tm *p;
  time(&timep);
  p=localtime(&timep);
  for (;j<len-1 && (c = *fmt++);) {
    if ('%'==c) {
      int i=0, t=-1;
      fmt1[i++] = '%';
      for(;isdigit(c = *fmt++);) {
        fmt1[i++] = c;
      }
      fmt1[i++] = 'd';
      fmt1[i] = 0;
      switch(c) {
      case 'y':t = (1900+p->tm_year);break;
      case 'm':t = (1+p->tm_mon);break;
      case 'd':t = (p->tm_mday);break;
      case 'h':t = (p->tm_hour);break;
      case 'M':t = (p->tm_min);break;
      case 's':t = (p->tm_sec);break;
      }
      if (t>=0) {
        _snprintf(buf+j, len-j, fmt1, t);
        j = strlen(buf);
      }
    } else {
      buf[j++] = c;
    }
  }
  buf[j] = 0;
  return buf;
}
CC_INLINE const char* strdate1(char* buf, int len) {
  time_t timep;
  struct tm *p;
  time(&timep);
  p=localtime(&timep); /*取得当地时间*/
  _snprintf(buf, len, "%04d%02d%02d", (1900+p->tm_year), (1+p->tm_mon), p->tm_mday);
  return (const char*)buf;
}
CC_INLINE const char* _strdate1() {
  static char buf_strdate2[32];
  return strdate1(buf_strdate2, countof(buf_strdate2));
}
CC_INLINE const char* strtime2(char* buf, int len) {
  time_t timep;
  struct tm *p;
  time(&timep);
  p=localtime(&timep); /*取得当地时间*/
  _snprintf(buf, len, "%04d%02d%02d%02d%02d%02d", (1900+p->tm_year), (1+p->tm_mon), p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
  return (const char*)buf;
}
CC_INLINE const char* _strtime2() {
  static char buf_strtime2[32];
  return strtime2(buf_strtime2, countof(buf_strtime2));
}
CC_INLINE const char* strtime3(char* buf, int len, const char* fmt) {
  time_t rawtime;
  tm_t *time1;
  time( &rawtime );
  time1 = localtime( &rawtime );
  strftime(buf,len, fmt, time1);
  return buf;
}
CC_INLINE const char* _strtime3() {
  static char _time_buffer[80];
  return strtime3(_time_buffer, countof(_time_buffer), "%x - %I:%M%p");
}

//! Various border types, image boundaries are denoted with `|`
//! @see borderInterpolate, copyMakeBorder
enum CBorderTypes {
    CC_BORDER_CONSTANT    = 0, //!< `iiiiii|abcdefgh|iiiiiii`  with some specified `i`
    CC_BORDER_REPLICATE   = 1, //!< `aaaaaa|abcdefgh|hhhhhhh`
    CC_BORDER_REFLECT     = 2, //!< `fedcba|abcdefgh|hgfedcb`
    CC_BORDER_WRAP        = 3, //!< `cdefgh|abcdefgh|abcdefg`
    CC_BORDER_REFLECT_101 = 4, //!< `gfedcb|abcdefgh|gfedcba`
    CC_BORDER_TRANSPARENT = 5, //!< `uvwxyz|absdefgh|ijklmno`

    CC_BORDER_REFLECT101  = CC_BORDER_REFLECT_101, //!< same as BORDER_REFLECT_101
    CC_BORDER_DEFAULT     = CC_BORDER_REFLECT_101, //!< same as BORDER_REFLECT_101
    CC_BORDER_ISOLATED    = 16 //!< do not look outside of ROI
};
