#ifndef _CSTD_H_
#define _CSTD_H_

#ifdef _WIN32
#include "sys_win32def.h"
#elif defined __linux__
#include "sys_linuxdef.h"
#else
#error unkown system
#endif

#ifdef _WINDOWS
//#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

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
#include <stdarg.h>
#include <limits.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h> // for vsprintf
#ifdef __cplusplus
#define CV_DEFAULT(val) = val
#else
#define CV_DEFAULT(val)
#define bool int
#define true 1
#define false 0
#endif
#define DEFAULT CV_DEFAULT
#if defined __cplusplus
#define CV_INLINE inline
#elif (defined _WIN32 || defined WIN64 || defined _WIN64 || defined WINCE) && !defined __GNUC__
#define CV_INLINE __inline
#else
#define CV_INLINE static
#endif
#define CV_IMPL __inline

typedef char *  myva_list;
#define myva_start(ap,v)  ( ap = (char*)&v + sizeof(v) )
#define myva_arg(ap,t)    ( *(t *)((ap += sizeof(t)) - sizeof(t)) )
#define myva_end(ap)      ( ap = (char*)0 )

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
typedef long long int64;
typedef unsigned long long uint64;
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

typedef char CHAR;
typedef short SHORT;
typedef long LONG;
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
typedef int64 s64;
typedef float       f32;
typedef double      f64;

typedef float float2[3];
typedef float float3[3];
typedef float float4[4];
typedef float float16[16];
typedef double double2[3];
typedef double double3[3];
typedef double double4[4];
typedef double double16[16];

typedef union Cv32suf {
  int i;
  unsigned u;
  float f;
}
Cv32suf;
typedef union suf64_t {
  int64 i;
  uint64 u;
  double f;
}
suf64_t;
typedef int CVStatus;
enum {
  CV_StsOk = 0,   /* everithing is ok                */
  CV_StsBackTrace = -1,   /* pseudo error for back trace     */
  CV_StsError = -2,   /* unknown /unspecified error      */
  CV_StsInternal = -3,   /* internal error (bad state)      */
  CV_StsNoMem = -4,   /* insufficient memory             */
  CV_StsBadArg = -5,   /* function arg/param is bad       */
  CV_StsBadFunc = -6,   /* unsupported function            */
  CV_StsNoConv = -7,   /* iter. didn't converge           */
  CV_StsAutoTrace = -8,   /* tracing                         */
  CV_HeaderIsNull = -9,   /* image header is NULL            */
  CV_BadImageSize = -10,  /* image size is invalid           */
  CV_BadOffset = -11,  /* offset is invalid               */
  CV_BadDataPtr = -12,  /**/
  CV_BadStep = -13,  /**/
  CV_BadModelOrChSeq = -14,  /**/
  CV_BadNumChannels = -15,  /**/
  CV_BadNumChannel1U = -16,  /**/
  CV_BadDepth = -17,  /**/
  CV_BadAlphaChannel = -18,  /**/
  CV_BadOrder = -19,  /**/
  CV_BadOrigin = -20,  /**/
  CV_BadAlign = -21,  /**/
  CV_BadCallBack = -22,  /**/
  CV_BadTileSize = -23,  /**/
  CV_BadCOI = -24,  /**/
  CV_BadROISize = -25,  /**/
  CV_MaskIsTiled = -26,  /**/
  CV_StsNullPtr = -27,  /* null pointer */
  CV_StsVecLengthErr = -28,  /* incorrect vector length */
  CV_StsFilterStructContentErr = -29,  /* incorr. filter structure content */
  CV_StsKernelStructContentErr = -30,  /* incorr. transform kernel content */
  CV_StsFilterOffsetErr = -31,  /* incorrect filter ofset value */
  CV_StsBadSize = -201,  /* the input/output structure size is incorrect  */
  CV_StsDivByZero = -202,  /* division by zero */
  CV_StsInplaceNotSupported = -203,  /* in-place operation is not supported */
  CV_StsObjectNotFound = -204,  /* request can't be completed */
  CV_StsUnmatchedFormats = -205,  /* formats of input/output arrays differ */
  CV_StsBadFlag = -206,  /* flag is wrong or not supported */
  CV_StsBadPoint = -207,  /* bad CvPoint */
  CV_StsBadMask = -208,  /* bad format of mask (neither 8uC1 nor 8sC1)*/
  CV_StsUnmatchedSizes = -209,  /* sizes of input/output structures do not match */
  CV_StsUnsupportedFormat = -210,  /* the data format/type is not supported by the function*/
  CV_StsOutOfRange = -211,  /* some of parameters are out of range */
  CV_StsParseError = -212,  /* invalid syntax/structure of the parsed file */
  CV_StsNotImplemented = -213,  /* the requested function/feature is not implemented */
  CV_StsBadMemBlock = -214,  /* an allocated block has been corrupted */
  CV_StsAssert = -215 /* assertion failed */
};
//#define if_c_a_else_b(c, a, b)   ((c) ? (a) : (b))
//! creates four CC codes used in Irrlicht for simple ids
#define MAKE_IRR_ID(c0, c1, c2, c3) ((u32)(u8)(c0) | ((u32)(u8)(c1) << 8) | ((u32)(u8)(c2) << 16) | ((u32)(u8)(c3) << 24 ))

#define PixelFormatIndexed      0x00010000 // Indexes into a palette
#define PixelFormatGDI          0x00020000 // Is a GDI-supported format
#define PixelFormatAlpha        0x00040000 // Has an alpha component
#define PixelFormatPAlpha       0x00080000 // Pre-multiplied alpha
#define PixelFormatExtended     0x00100000 // Extended color 16 bits/channel
#define PixelFormatCanonical    0x00200000
#define PixelFormatUndefined       0
#define PixelFormatDontCare        0
#define PixelFormat1bppIndexed     (1 | ( 1 << 8) | PixelFormatIndexed | PixelFormatGDI)
#define PixelFormat4bppIndexed     (2 | ( 4 << 8) | PixelFormatIndexed | PixelFormatGDI)
#define PixelFormat8bppIndexed     (3 | ( 8 << 8) | PixelFormatIndexed | PixelFormatGDI)
#define PixelFormat16bppGrayScale  (4 | (16 << 8) | PixelFormatExtended)
#define PixelFormat16bppRGB555     (5 | (16 << 8) | PixelFormatGDI)
#define PixelFormat16bppRGB565     (6 | (16 << 8) | PixelFormatGDI)
#define PixelFormat16bppARGB1555   (7 | (16 << 8) | PixelFormatAlpha | PixelFormatGDI)
#define PixelFormat24bppRGB        (8 | (24 << 8) | PixelFormatGDI)
#define PixelFormat32bppRGB        (9 | (32 << 8) | PixelFormatGDI)
#define PixelFormat32bppARGB       (10 | (32 << 8) | PixelFormatAlpha | PixelFormatGDI | PixelFormatCanonical)
#define PixelFormat32bppPARGB      (11 | (32 << 8) | PixelFormatAlpha | PixelFormatPAlpha | PixelFormatGDI)
#define PixelFormat48bppRGB        (12 | (48 << 8) | PixelFormatExtended)
#define PixelFormat64bppARGB       (13 | (64 << 8) | PixelFormatAlpha  | PixelFormatCanonical | PixelFormatExtended)
#define PixelFormat64bppPARGB      (14 | (64 << 8) | PixelFormatAlpha  | PixelFormatPAlpha | PixelFormatExtended)
#define PixelFormatMax             15
  // Matrix Order
  enum MatrixOrder {
    MatrixOrderPrepend    = 0,
    MatrixOrderAppend     = 1
  };
  // Dash style constants
  enum DashStyle {
    DashStyleSolid,          // 0
    DashStyleDash,           // 1
    DashStyleDot,            // 2
    DashStyleDashDot,        // 3
    DashStyleDashDotDot,     // 4
    DashStyleCustom          // 5
  };
  // Line join constants
  enum LineJoin {
    LineJoinMiter        = 0,
    LineJoinBevel        = 1,
    LineJoinRound        = 2,
    LineJoinMiterClipped = 3
  };
  // Brush types
  enum BrushType {
    BrushTypeSolidColor       = 0,
    BrushTypeHatchFill        = 1,
    BrushTypeTextureFill      = 2,
    BrushTypePathGradient     = 3,
    BrushTypeLinearGradient   = 4
  };
  // Various wrap modes for brushes
  enum WrapMode {
    WrapModeTile,        // 0
    WrapModeTileFlipX,   // 1
    WrapModeTileFlipY,   // 2
    WrapModeTileFlipXY,  // 3
    WrapModeClamp,       // 4
    WrapModeStretch,
  };
  // FontStyle: face types and common styles
  enum FontStyle {
    FontStyleRegular    = 0,
    FontStyleBold       = 1,
    FontStyleItalic     = 2,
    FontStyleBoldItalic = 3,
    FontStyleUnderline  = 4,
    FontStyleStrikeout  = 8
  };
  // String alignment flags
  enum StringAlignment {
    // Left edge for left-to-right text,
    // right for right-to-left text,
    // and top for vertical
    StringAlignmentNear   = 0,
    StringAlignmentCenter = 1,
    StringAlignmentFar    = 2
  };
  //############## Not implemented-Used
  // Unit constants
  enum Unit {
    UnitWorld,      // 0 -- World coordinate (non-physical unit)
    UnitDisplay,    // 1 -- Variable -- for PageTransform only
    UnitPixel,      // 2 -- Each unit is one device pixel.
    UnitPoint,      // 3 -- Each unit is a printer's point, or 1/72 inch.
    UnitInch,       // 4 -- Each unit is 1 inch.
    UnitDocument,   // 5 -- Each unit is 1/300 inch.
    UnitMillimeter  // 6 -- Each unit is 1 millimeter.
  };
  // Text Rendering Hint
  enum TextRenderingHint {
    TextRenderingHintSystemDefault = 0,            // Glyph with system default rendering hint
    TextRenderingHintSingleBitPerPixelGridFit,     // Glyph bitmap with hinting
    TextRenderingHintSingleBitPerPixel,            // Glyph bitmap without hinting
    TextRenderingHintAntiAliasGridFit,             // Glyph anti-alias bitmap with hinting
    TextRenderingHintAntiAlias,                    // Glyph anti-alias bitmap without hinting
    TextRenderingHintClearTypeGridFit              // Glyph CT bitmap with hinting
  };
  // Quality mode constants
  enum QualityMode {
    QualityModeInvalid   = -1,
    QualityModeDefault   = 0,
    QualityModeLow       = 1, // Best performance
    QualityModeHigh      = 2  // Best rendering quality
  };
  // Alpha Compositing mode constants
  enum CompositingMode {
    CompositingModeSourceOver,    // 0
    CompositingModeSourceCopy     // 1
  };
  // Alpha Compositing quality constants
  enum CompositingQuality {
    CompositingQualityInvalid          = QualityModeInvalid,
    CompositingQualityDefault          = QualityModeDefault,
    CompositingQualityHighSpeed        = QualityModeLow,
    CompositingQualityHighQuality      = QualityModeHigh,
    CompositingQualityGammaCorrected,
    CompositingQualityAssumeLinear
  };
  // Smoothing Mode
  enum SmoothingMode {
    SmoothingModeInvalid     = QualityModeInvalid,
    SmoothingModeDefault     = QualityModeDefault,
    SmoothingModeHighSpeed   = QualityModeLow,
    SmoothingModeHighQuality = QualityModeHigh,
    SmoothingModeNone,
    SmoothingModeAntiAlias
  };
  // Interpolation modes
  enum InterpolationMode {
    InterpolationModeInvalid          = QualityModeInvalid,
    InterpolationModeDefault          = QualityModeDefault,
    InterpolationModeLowQuality       = QualityModeLow,
    InterpolationModeHighQuality      = QualityModeHigh,
    InterpolationModeBilinear,
    InterpolationModeBicubic,
    InterpolationModeNearestNeighbor,
    InterpolationModeHighQualityBilinear,
    InterpolationModeHighQualityBicubic
  };
  enum StringFormatFlags {
    //not supp   StringFormatFlagsDirectionRightToLeft        = 0x00000001,
    //not supp   StringFormatFlagsDirectionVertical           = 0x00000002,
    //not supp   StringFormatFlagsNoFitBlackBox               = 0x00000004,
    //not supp   StringFormatFlagsDisplayFormatControl        = 0x00000020,
    //not supp   StringFormatFlagsNoFontFallback              = 0x00000400,
    //not supp   StringFormatFlagsMeasureTrailingSpaces       = 0x00000800,
    //not supp   StringFormatFlagsNoWrap                      = 0x00001000,
    StringFormatFlagsLineLimit                   = 0x00002000,
    //not supp   StringFormatFlagsNoClip                      = 0x00004000
  };

  //               ??锟斤拷?
#define cvInvSqrt(value) ((float)(1./sqrt(value)))
#define cvSqrt(value)  ((float)sqrt(value))
  /*************** Random number generation *******************/
typedef uint64 CvRNG;
/* Return random 32-bit unsigned integer: */
CV_INLINE unsigned cvRandInt()
{
  static CvRNG rng = 128981234;
  uint64 temp = rng;
  temp = (uint64)(unsigned) temp * 4164903690U + (temp >> 32);
  rng = temp;
  return (unsigned) temp;
}
/* Returns random floating-point number between 0 and 1: */
CV_INLINE double cvRandReal()
{
  return cvRandInt() * 2.3283064365386962890625e-10 /* 2^-32 */;
}
//   Array allocation, deallocation, initialization and access to elements         *
#define CV_MAX_ARR 10
#define CV_NO_DEPTH_CHECK     1
#define CV_NO_CN_CHECK        2
#define CV_NO_SIZE_CHECK      4
#define cvCvtScaleAbs  cvConvertScaleAbs
//     Arithmetic, logic and comparison operations                          *
#define CV_CMP_EQ   0
#define CV_CMP_GT   1
#define CV_CMP_GE   2
#define CV_CMP_LT   3
#define CV_CMP_LE   4
#define CV_CMP_NE   5
//                  Math operations                                         *
/* Fast cubic root calculation */
//CVAPI( float ) cvCbrt( float value );
/* Checks array values for NaNs, Infs or simply for too large numbers
   (if CV_CHECK_RANGE is set). If CV_CHECK_QUIET is set,
   no runtime errors is raised (function returns zero value in case of "bad" values).
   Otherwise cvError is called */
#define  CV_CHECK_RANGE    1
#define  CV_CHECK_QUIET    2
#define CV_RAND_UNI      0
#define CV_RAND_NORMAL   1
//                  Matrix operations                                       *
/* Matrix transform: dst = A*B + C, C is optional */
#define cvMatMulAdd( src1, src2, src3, dst ) cvGEMM_64f( (src1), src1step, (src2), src2step, 1., (src3), 1., (dst), 0 )
#define cvMatMul( src1, src2, dst )  cvMatMulAdd( (src1), (src2), NULL, (dst))
#define CV_GEMM_A_T 1
#define CV_GEMM_B_T 2
#define CV_GEMM_C_T 4
#define CV_GEMM_D_T 8
#define CV_GEMM_COMPLEX 16
#define CV_SVD_MODIFY_A   1
#define CV_SVD_U_T        2
#define CV_SVD_V_T        4
#define CV_LU  0
#define CV_SVD 1
#define CV_SVD_SYM 2
/* Inverts matrix */
//                Dynamic data structures                                   *
/* Calculates length of sequence slice (with support of negative indices). */
//CVAPI( int ) cvSliceLength( CvSlice slice, const CvSeq * seq );
#define CV_FRONT 1
#define CV_BACK 0
/* Retrieves graph vertex by given index */
#define cvGetGraphVtx( graph, idx ) (CvGraphVtx*)cvGetSetElem((CvSet*)(graph), (idx))
/* Retrieves index of a graph edge given its pointer */
#define  CV_GRAPH_VERTEX        1
#define  CV_GRAPH_TREE_EDGE     2
#define  CV_GRAPH_BACK_EDGE     4
#define  CV_GRAPH_FORWARD_EDGE  8
#define  CV_GRAPH_CROSS_EDGE    16
#define  CV_GRAPH_ANY_EDGE      30
#define  CV_GRAPH_NEW_TREE      32
#define  CV_GRAPH_BACKTRACKING  64
#define  CV_GRAPH_OVER          -1
#define  CV_GRAPH_ALL_ITEMS    -1
/* flags for graph vertices and edges */
#define  CV_GRAPH_ITEM_VISITED_FLAG  (1 << 30)
#define  CV_IS_GRAPH_VERTEX_VISITED(vtx) \
  (((CvGraphVtx*)(vtx))->flags & CV_GRAPH_ITEM_VISITED_FLAG)
#define  CV_IS_GRAPH_EDGE_VISITED(edge) \
  (((CvGraphEdge*)(edge))->flags & CV_GRAPH_ITEM_VISITED_FLAG)
#define  CV_GRAPH_SEARCH_TREE_NODE_FLAG   (1 << 29)
#define  CV_GRAPH_FORWARD_EDGE_FLAG       (1 << 28)
//                       Drawing                                            *
//#define CV_RGB( r, g, b )  cvScalar( (b), (g), (r), 0 )
#define CV_FILLED -1
#define CV_AA 16
/* basic font types */
#define CV_FONT_HERSHEY_SIMPLEX         0
#define CV_FONT_HERSHEY_PLAIN           1
#define CV_FONT_HERSHEY_DUPLEX          2
#define CV_FONT_HERSHEY_COMPLEX         3
#define CV_FONT_HERSHEY_TRIPLEX         4
#define CV_FONT_HERSHEY_COMPLEX_SMALL   5
#define CV_FONT_HERSHEY_SCRIPT_SIMPLEX  6
#define CV_FONT_HERSHEY_SCRIPT_COMPLEX  7
/* font flags */
#define CV_FONT_ITALIC                 16
#define CV_FONT_VECTOR0    CV_FONT_HERSHEY_SIMPLEX
#define CV_FILLED -1
//                      System functions                                    *
#define CV_ErrModeLeaf     0   /* Print error and exit program */
#define CV_ErrModeParent   1   /* Print error and continue */
#define CV_ErrModeSilent   2   /* Don't print and continue */

// 用宏判断计算机处理器是大端字节序
static union { char c[4]; unsigned long mylong; } endian_test = {{ 'l', '?', '?', 'b' } };
#define ENDIANNESS ((char)endian_test.mylong)
#define BIGENDIANNESS ('b'==ENDIANNESS)

typedef BYTE* LPBYTE;
#include <float.h>
//#pragma warning( disable: 4127 )
#ifndef UNUSED
#define UNUSED(__X)  { const void* _P0 = &(__X); _P0=&_P0; }
#endif
#ifndef countof
#define countof(_ARR)  (sizeof(_ARR)/sizeof((_ARR)[0]))
#endif // countof
#define CV_8TO32F(x)  (x)
#define cvRound(X)  ROUND(X)
#define SQR(_X)    ((_X)*(_X))
#define SQR3(_X)   ((_X)*(_X)*(_X))
#define SQR4(_X)   ((_X)*(_X)*(_X)*(_X))
#define MAX_INT             0x7fffffff
#define MIN_INT             0x80000000
#define MAX_DOUBLE          1.7976931348623158E+308
#define MIN_DOUBLE         -1.7976931348623158E+308
#if 0
#ifndef MAKEWORD
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(a)) | ((WORD)((BYTE)(b))) << 8))
#endif
#ifndef MAKELONG
#define MAKELONG(a, b)      ((LONG)(((WORD)(a)) | ((DWORD)((WORD)(b))) << 16))
#endif
#ifndef LOWORD
#define LOWORD(l)           ((WORD)(l))
#endif
#ifndef HIWORD
#define HIWORD(l)           ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#endif
#ifndef LOBYTE
#define LOBYTE(w)           ((BYTE)(w))
#endif
#ifndef HIBYTE
#define HIBYTE(w)           ((BYTE)(((WORD)(w) >> 8) & 0xFF))
#endif
#endif
#ifndef ASSERT
#ifdef _DEBUG
#if 0
static int MyAssert(const char* pStatement, const char* pFileName, unsigned int LineNumber)
{
  char Message[1024];
  sprintf(Message, "%s(%d) : assert: %s",  pFileName, LineNumber, pStatement);
  printf("%s\n", Message);
  MessageBox(NULL, Message, NULL, MB_OK);
  return 0;
}
#define ASSERT(exp)  (void)( (exp) || (MyAssert(#exp, __FILE__, __LINE__), 0) )
#else
#define ASSERT( exp )           assert( exp )
#define VERIFY( exp )           assert( exp )
//#define TRACE                   OutputDebugString
#define TRACE                   printf
#endif
#else
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
CV_INLINE void ptr2ptr(void* pp, void* p) {
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
#define OPENCV_ERROR(status,func,context)       cvError((status),(func),(context),__FILE__,__LINE__)
#define cvGetErrStatus() (0)
#define OPENCV_ERRCHK(func,context)             { if (cvGetErrStatus() >= 0) {OPENCV_ERROR(CV_StsBackTrace,(func),(context));} }
#define OPENCV_ASSERT(expr,func,context)        { if (! (expr)) {OPENCV_ERROR(CV_StsInternal,(func),(context));} }
#define OPENCV_RSTERR()                         ( cvSetErrStatus(CV_StsOk) )
#define OPENCV_CALL( Func )                     { Func; }
#define SETBIT(X, M, B)  if (B) {X|=M;} else {X&=~(M);}
/* CV_FUNCNAME macro defines icvFuncName constant which is used by CV_ERROR macro */
#ifdef CV_NO_FUNC_NAMES
#define CV_FUNCNAME( Name )
#define cvFuncName ""
#else
#define CV_FUNCNAME( Name )  static char cvFuncName[] = Name
#endif
#define cvError( status, func_name, err_msg, file_name, line )   printf("%s(%d):%s %s", file_name, line, func_name, err_msg)
/*
 CV_ERROR macro unconditionally raises error with passed code and message.
 After raising error, control will be transferred to the exit label.
 */
#define CV_ERROR( Code, Msg )  { cvError( (Code), cvFuncName, Msg, __FILE__, __LINE__ ); __CV_EXIT__; }
/* Simplified form of CV_ERROR */
#define CV_ERROR_FROM_CODE( code )       CV_ERROR( code, "" )
#define CV_CHECK()
/*
 CV_CALL macro calls CV (or IPL) function, checks error status and
 signals a error if the function failed. Useful in "parent node"
 error procesing mode
 */
#define CV_CALL( Func ) { Func; CV_CHECK(); }
/* Runtime assertion macro */
#define CV_ASSERT( Condition ) { if( !(Condition) ) CV_ERROR( CV_StsInternal, "Assertion: " #Condition " failed" ); }
#define __CV_BEGIN__       {
#define __CV_END__         goto exit; exit: ; }
#define __CV_EXIT__        goto exit
#undef CV_ERROR
#define CV_ERROR(_ERRID, _ERRSTR) do { printf(_ERRSTR); } while(0)
//#define CV_ERROR(a, b)  { fprintf(stderr, b); }
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
/* CV_SWAP */
#ifndef CV_SWAP
#define CV_SWAP(_A,_B,_T) ((_T) = (_A), (_A) = (_B), (_B) = (_T))
#endif
#ifndef T_SWAP
#define T_SWAP(_TYPE, _A,_B) {_TYPE _TMP; CV_SWAP(_A, _B, _TMP);}
//#define T_SWAP(TYPE, a, b) {TYPE t=a; a=b, b=t;}
#endif
#define XCMP(A,B)    (((A) > (B)) - ((A) < (B)))
#define XCMPMEM(N0, D0, N1, D1, _OUT) { int I=0; _OUT=XCMP(N0,N1); for (;0==_OUT && I<N0; ++I) {_OUT=XCMP((D0)[I], (D1)[I]);} }
#define XSIGN(a)     XCMP((a),0)
// ??????
#define MEMCMP(_N, _F, _P, _R)  {int _I=0; for (; _I<_N && 0==(_R=XCMP(*(_F+_I),*(_P+_I))); ++_I); }
// ??????
#define MEMRCMP(_N, _F, _P, _R) {int _I=_N; for (; (0<_I--) && 0==(_R=XCMP(*(_F+_I),*(_P+_I))); ); }
/* min & max without jumps */
#define IMIN(a, b)  ((a) ^ (((a)^(b)) & (((a) < (b)) - 1)))
#define IMAX(a, b)  ((a) ^ (((a)^(b)) & (((a) > (b)) - 1)))
#ifndef MAX
#define MAX(a, b) ( (a) > (b) ? (a) : (b) )
#endif
#ifndef MIN
#define MIN(a, b) ( (a) > (b) ? (b) : (a) )
#endif
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
#ifndef COPY
#define COPY(_N, _F, _P) \
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
#endif
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
#define MEMCPY1D(_N, _F, _P)  memcpy(_P, _F, (_N)*sizeof(*(_F)))
#define MEMCPY2D(_M, _N, _F, _FL, _P, _PL) \
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
#endif
#ifndef FOR_EACH_1D2A
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
        CV_SWAP(*(_B+_B1), *(_A+_A1), t); \
  } while(0)
#define CLONE(TYPE, _N, _F, _P) \
  do { \
    _P = (TYPE*)malloc(sizeof(TYPE)*(_N)); \
    memcpy(_P, _F, sizeof(TYPE)*(_N)); \
  } while(0)
#define CLONE2D(TYPE, _M, _N, _F, _FL, _P) \
  do { \
    _P = (TYPE*)malloc(sizeof(TYPE)*(_N)*(_M)); \
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
      float __A = 1.0f * XMAX / (*(_F+_AMAX) - *(_F+_AMIN)); \
      float __B = -__A * (*(_F+_AMIN)) + XMIN; \
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
#define FLIP1D(_N, _A, _AI, t) do {int _I=0; for (; _I<(_N)/2; _I+=_AI) CV_SWAP(*(_A+_I), *(_A+(_N)-(_AI)-_I), t); } while(0)
// ???????锟斤拷??
#define FLIP2D(_M, _N, _A, _AL, _AI, t) \
  do { \
    int _LA, _IA; \
    for (_LA=0; _LA<((_M)/2); ++_LA) \
      for (_IA=0; _IA<(_N); ++_IA) \
        CV_SWAP(*(_A+_LA*(_AL)+_IA*(_AI)), *(_A+(_M-_LA-1)*(_AL)+_IA*(_AI)), t);\
  } while(0)
// ??????????
#define FLOP2D(_M, _N, _A, _AL, _AI, t) \
  do { \
    int _LA, _IA; \
    for (_LA=0; _LA<(_M); ++_LA) \
      for (_IA=0; _IA<(_N)/2; ++_IA) \
        CV_SWAP(*(_A+_LA*(_AL)+_IA*(_AI)), *(_A+(_LA)*(_AL)+_AI*(_N-_IA-1)), t);\
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
#if 1
#ifdef __CPLUSPLUS
// TEMPLATE FUNCTION push_heap WITH PRED
template <class _RI, class _Pr, class _Ty>
inline
void PUSH_HEAP(int _N, _RI _F, _Pr _P, _Ty _V)
{
  _Push_heap(_F, int(_N - 1), int(0), (*(_F + _N - 1)), _P);
}
template <class _RI, class _Ty, class _Pr>
inline
void _PUSH_HEAP(_RI _F, int _H, int _J, _Ty _V, _Pr _P)
{
  for (int _I = (_H - 1) / 2; _J < _H && _P(*(_F + _I), _V);
  _I = (_H - 1) / 2) {
    * (_F + _H) = *(_F + _I), _H = _I;
  }
  *(_F + _H) = _V;
}
// TEMPLATE FUNCTION pop_heap WITH PRED
template <class _RI, class _Pr, class _Ty>
inline
void POP_HEAP(int _N, _RI _F, _Pr _P, _Ty _V)
{
  _POP_HEAP(_N - 1, _F, _F + _N - 1, (*(_F + _N - 1)), _P);
}
template <class _RI, class _Ty, class _Pr>
inline
void _POP_HEAP(int _N, _RI _F, _RI _X, _Ty _V, _Pr _P)
{
  *_X = *_F;
  _ADJUST_HEAP(_F, int(0), _N, _V, _P);
}
template <class _RI, class _Pr, class _Ty>
inline
void _ADJUST_HEAP(_RI _F, int _H, int _N, _Ty _V, _Pr _P)
{
  int _J = _H;
  int _K = 2 * _H + 2;
  for (; _K < _N; _K = 2 * _K + 2) {
    if (_P(*(_F + _K), *(_F + (_K - 1)))) {
      --_K;
    }
    *(_F + _H) = *(_F + _K), _H = _K;
  }
  if (_K == _N) {
    * (_F + _H) = *(_F + (_K - 1)), _H = _K - 1;
  }
  _PUSH_HEAP(_F, _H, _J, _V, _P);
}
// TEMPLATE FUNCTION make_heap WITH PRED
template <class _RI, class _Pr, class _Ty>
inline
void MAKE_HEAP(int _N, _RI _F, _Pr _P, _Ty _V)
{
  if (2 <= _N)
    for (int _H = _N / 2; 0 < _H;) {
      --_H;
      _V = (*(_F + _H));
      _ADJUST_HEAP(_F, _H, _N, _V, _P);
    }
}
// TEMPLATE FUNCTION sort_heap WITH PRED
template <class _RI, class _Pr, class _Ty>
inline
void SORT_HEAP(int _N, _RI _F, _Pr _P, _Ty _V)
{
  for (; 1 < _N; --_N) {
    POP_HEAP(_N, _F, _P, _V);
  }
}
#else
#define _PUSH_HEAP(_F, _H0, _J0, _V, _P)\
  {int _H1=_H0, _J1=_J0, _I = (_H0 - 1) / 2; \
    for (; _J1 < _H1 && _P(*(_F + _I), _V);\
        _I = (_H1 - 1) / 2)\
      *(_F + _H1) = *(_F + _I), _H1 = _I;\
    *(_F + _H1) = _V; }
#define _ADJUST_HEAP(_F, _H0, _N0, _V, _P) \
  {int _H2=_H0, _N2=_N0, _J = _H0; \
    int _K = 2 * _H2 + 2; \
    for (; _K < _N2; _K = 2 * _K + 2) \
    {if (_P(*(_F + _K), *(_F + (_K - 1)))) \
        --_K; \
      *(_F + _H2) = *(_F + _K), _H2 = _K; } \
    if (_K == _N2) \
      *(_F + _H2) = *(_F + (_K - 1)), _H2 = _K - 1; \
    _PUSH_HEAP(_F, _H2, _J, _V, _P); }
#define PUSH_HEAP(_N, _F, _P, _V) { _V = (*(_F + _N - 1)); _PUSH_HEAP(_F, (_N - 1), (0), _V, _P); }
#define POP_HEAP(_N, _F, _P, _V)  {_V = (*(_F + _N - 1)); (*(_F + _N - 1)) = *_F; _ADJUST_HEAP(_F, (0), (_N - 1), _V, _P); }
#define MAKE_HEAP(_N, _F, _P, _V) if (2 <= _N) {int _H = _N / 2; for (; 0 < _H; ) { --_H; _V = (*(_F + _H)); _ADJUST_HEAP(_F, _H, _N, _V, _P); } }
// TEMPLATE FUNCTION sort_heap WITH PRED
#define SORT_HEAP(_N, _F, _P, _V) { int _N0=_N; for (; 1 < _N0; --_N0) POP_HEAP(_N0, _F, _P, _V); }
#if 0
int test_push_heap()
{
  int a[] = {5, 1, 9, 4, 6, 2, 0, 3, 8, 7};
  enum {an = countof(a)};
  int b[5] = {0};
  int i, bn = 0, t;
  for (i = 0; i < an; i++) {
    if (bn < 5) {
      b[bn++] = a[i];
    }
    PUSH_HEAP(bn, b, LESS, t);
  }
  SORT_HEAP(bn, b, LESS, t);
  return 0;
}
#endif
#endif
#endif
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
      CV_SWAP(*(_F + _X), *(_F + _Y), _TMP); } \
  } while(0)
#ifndef SHIFT
#define SHIFT                 (13)
#define SHIFT1                (1<<SHIFT)
#define SHIFT_MARK            (SHIFT1 - 1)
#define SHIFT_MUL(x, y)       (((x)*(y))>>SHIFT)
#define SHIFT_DIV(x, y)       (((int)(x)<<SHIFT)/(y))
#define SHIFT_TOFLOAT(x)      (((double)(x))/SHIFT1)
#define SHIFT_TOINT(x)        (int)((x)*SHIFT1)
#define SHIFT_TOINT2(x, _S)   (int)((x)*(1<<(_S)))
#define HALFSHIFT1            (1<<12)
#endif
// RGB2GRAY
#ifndef GREYSCALIZE
#define _CR                  (299*(1 << SHIFT)/1000)
#define _CG                  (587*(1 << SHIFT)/1000)
#define _CB                  (SHIFT1 - _CR - _CG)
#define GREYSCALIZE(R, G, B)   ((_CR*(R) + _CG*(G) + _CB*(B) )>>SHIFT)
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
#define  DOT_MULT3_3(X, Y, Z, R, G, B)  ((X)*(R)+(Y)*(G)+(Z)*(B))
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
    Y = GetY_RGB(in+2), *(in+1), *(in)); \
    I = GetI_RGB(in+2), *(in+1), *(in)); \
    Q = GetQ_RGB(in+2), *(in+1), *(in)); \
    *(out) = (((Y < 235) && (I > 12) && (pow(I / 58, 2) + pow((Y - 142) / 104, 2)) < 1.) ? (0) : (255));\
  } while(0)
// ????????
#define SKINLIKEHOOD_T(in, out) \
  do { \
    static const double B_Mean = 117.4361, R_Mean = 156.5599; \
    static const double Brcov[4)  = { 160.1301, 12.1430, 12.1430, 299.4574 }; \
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
#ifndef cvFloor
#define cvFloor(X)  ((int)floor(X))
#endif
#ifndef cvCeil
#define cvCeil(X)  ((int)ceil(X))
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
#define yuv_descale(x)  CV_DESCALE((x), yuv_shift)
#define yuv_prescale(x) ((x) << yuv_shift)
#define  yuvRCr_32f   1.403f
#define  yuvGCr_32f   (-0.714f)
#define  yuvGCb_32f   (-0.344f)
#define  yuvBCb_32f   1.773f
#define  yuvRCr   fix(yuvRCr_32f,yuv_shift)
#define  yuvGCr   (-fix(-yuvGCr_32f,yuv_shift))
#define  yuvGCb   (-fix(-yuvGCb_32f,yuv_shift))
#define  yuvBCb   fix(yuvBCb_32f,yuv_shift)
//#define  scale_macro(a)      (a)
//#define  cast_macro(a)       (a)
#define  CV_NOP(a)           (a)
#define  CV_DESCALE(x,n)     (((x) + (1 << ((n)-1))) >> (n))
#define  CV_FLT_TO_FIX(x,n)  cvRound((x)*(1<<(n)))
#define  DESCALE(x,n)  (((x) + (1 << ((n)-1))) >> (n))
#define  descale DESCALE
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
    IT* AE = GMALLOC(IT, (_H+(y)*2)*ael); \
    IT* A1 = AE+(x)*(ai)+(y)*ael; \
    IT* _C0 = AE+(x)*(ai); \
    COPY_REPLICATE_BORDER(_H, _W, _A, al, _H2, _W2, AE, ael, ai, x, y); \
    SKIP_FILTER_RC(_H, _W, A1, ael, ai, _B, bl, bi, x, y, _C0, XFUNC, YFUNC); \
    GFREE(AE); \
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
    IT* AE = GMALLOC(IT, (_H2)*ael); \
    IT* AE1 = AE+(x)*ai+(y)*ael; \
    COPY_REPLICATE_BORDER(_H, _W, _A, al, _H2, _W2, AE, ael, ai, y, x); \
    SKIP_BLOCK_FILTER_HIST(_H, _W, AE1, ael, ai, _B, bin, hi, wi, hs, ws, FUNC);\
    GFREE(AE); \
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
      _sum += cvRound(_hist[_i]); \
      _hist[_i] = (uchar)cvRound(_sum*_scale); \
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

static int arrinit(int nbyte, void* pdata, ...)
{
  myva_list ARGLIST;
  myva_start(ARGLIST, pdata);
  memcpy(pdata, (&pdata) + 1, nbyte);
  myva_end(ARGLIST);
  return 0;
}
#define TRANSFORM(_IF, _IL, _OF, _OP) {int _i=0; for (; (_IF+_i)!=(_IL); ++_i) {*(_OF+_i)=_OP(*(_IF+_i));}}
#define LESS(a, b)  ((a)<(b))
#ifndef CV_SWAP
#define CV_SWAP(a,b,t) ((t) = (a), (a) = (b), (b) = (t))
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
#define MIDLTITER(A, B, C, LT) \
  (LT(*(A), *(B)) ? (LT(*(B), *(C)) ? (B) : (LT(*(A), *(C)) ? (C) : (A))) \
        : (LT(*(C), *(B)) ? (B) : (LT(*(A), *(C)) ? (A) : (C))))
#define SORT_INSERT1(_N, _F, _X, _T, _P) { \
    int _i=0; _T _x=_X, t; for (; _i<_N; ++_i) { \
      if (_P(_x, (_F)[_i])) {CV_SWAP(_x, (_F)[_i], t);} \
    } \
  }
#define INSERT_SORT(left, right, _P, _Ty) { \
    _Ty* ptr = left + 1, *ptr2, t; \
    for( ; ptr <= right; ++ptr ) { \
      for( ptr2 = ptr; ptr2 > left && (_P(ptr2[0],ptr2[-1])); ptr2--) \
        CV_SWAP( ptr2[0], ptr2[-1], t ); \
    } \
  }
#define PARTITION(_T, _F0, _L0, _P, _OF)   { _T t; \
    _T* _F = _F0; _T* _L = _L0; \
    for (; ; ++_F) { \
      for (; _F != _L && _P(*_F); ++_F); \
      if (_F == _L) { break; } \
      for (; _F != --_L && !_P(*_L);); \
      if (_F == _L) { break; } \
      CV_SWAP(*_F, *_L, t); \
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
            CV_SWAP( *pivot, *left0, t ); \
            pivot = left0; \
          } \
          left = left1 = left0 + 1; \
          right = right1 = right0; \
          for(;;) { \
            while( left <= right && !(_P(*pivot, *left)) ) { \
              if( !(_P(*left, *pivot)) ) { \
                if( left > left1 ) \
                  CV_SWAP( *left1, *left, t ); \
                swap_cnt = 1; \
                left1++; \
              } \
              left++; \
            } \
            while( left <= right && !(_P(*right, *pivot)) ) { \
              if( !(_P(*pivot, *right)) ) { \
                if( right < right1 ) \
                  CV_SWAP( *right1, *right, t ); \
                swap_cnt = 1; \
                right1--; \
              } \
              right--; \
            } \
            if( left > right ) \
              break; \
            CV_SWAP( *left, *right, t ); \
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
            CV_SWAP( left0[i], left[i-n], t ); \
          n = MIN( (int)(right0 - right1), (int)(right1 - right) ); \
          for( i = 0; i < n; i++ ) \
            CV_SWAP( left[i], right0[i-n+1], t ); \
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
        CV_SWAP((_F)[_so_k],(_F)[_so_l+1], _so_tmp);\
        if (_Cmpr(((_F)[_so_ir]),((_F)[_so_l])))\
          CV_SWAP((_F)[_so_l],(_F)[_so_ir], _so_tmp);\
        if (_Cmpr(((_F)[_so_ir]),((_F)[_so_l+1])))\
          CV_SWAP((_F)[_so_l+1],(_F)[_so_ir], _so_tmp);\
        if (_Cmpr(((_F)[_so_l+1]),((_F)[_so_l])))\
          CV_SWAP((_F)[_so_l],(_F)[_so_l+1], _so_tmp);\
        _so_i = _so_l+1,_so_j = _so_ir;\
        _so_a = (_F)[_so_l+1];\
        for (;;){\
          do ++_so_i; while(_Cmpr(((_F)[_so_i]),_so_a));\
          do --_so_j; while(_Cmpr(_so_a,((_F)[_so_j])));\
          if (_so_j<_so_i) break;\
          CV_SWAP((_F)[_so_i],(_F)[_so_j], _so_tmp);\
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
#define SORT2(vec2, LESS, t) do { if (!LESS(*(vec2), *(vec2+1))) CV_SWAP(*(vec2), *(vec2+1), t); } while(0)
#define SORT3(vec, LESS, t) do {SORT2(vec+1, LESS, t);if(!LESS(*(vec), *(vec+1))) {CV_SWAP(*(vec), *(vec+1), t);SORT2(vec+1, LESS, t);} } while(0)
#define SORT4(vec, LESS, t) do {SORT3(vec+1, LESS, t);if(!LESS(*(vec), *(vec+1))) {CV_SWAP(*(vec), *(vec+1), t);SORT3(vec+1, LESS, t);} } while(0)
#define SORT5(vec, LESS, t) do {SORT4(vec+1, LESS, t);if(!LESS(*(vec), *(vec+1))) {CV_SWAP(*(vec), *(vec+1), t);SORT4(vec+1, LESS, t);} } while(0)
#define SORT6(vec, LESS, t) do {SORT5(vec+1, LESS, t);if(!LESS(*(vec), *(vec+1))) {CV_SWAP(*(vec), *(vec+1), t);SORT5(vec+1, LESS, t);} } while(0)
#define SORT7(vec, LESS, t) do {SORT6(vec+1, LESS, t);if(!LESS(*(vec), *(vec+1))) {CV_SWAP(*(vec), *(vec+1), t);SORT6(vec+1, LESS, t);} } while(0)
#define SORT8(vec, LESS, t) do {SORT7(vec+1, LESS, t);if(!LESS(*(vec), *(vec+1))) {CV_SWAP(*(vec), *(vec+1), t);SORT7(vec+1, LESS, t);} } while(0)
#define SORT9(vec, LESS, t) do {SORT8(vec+1, LESS, t);if(!LESS(*(vec), *(vec+1))) {CV_SWAP(*(vec), *(vec+1), t);SORT8(vec+1, LESS, t);} } while(0)
#if 1
#define QSORT(_F, _L, _P, _Ty)   QUICK_SORT_VER1(_L-_F, _F, _P, _Ty)
#else
#define QSORT(_F, _L, _P, _Ty)   QUICK_SORT_VER2(_L-_F, _F, _P, _Ty)
#endif
// ????????
#define INSERTSORT(_N, _F, _P, _Ty) \
  do { \
    int _I, _J; \
    for (_I=1; _I<_N; ++_I) { \
      _Ty temp = *(_F+_I); \
      for (_J=_I-1; _J>=0 && _P(temp, *(_F+_J)); --_J) { \
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
          CV_SWAP(*(_F+_I), *(_F+_I+1), temp); \
          _M=_I; \
        } \
      for (_J=_K+1, _K=0, _I=_M; _I>=_J; --_I) \
        if (*(_F+_I-1)>*(_F+_I)) { \
          CV_SWAP(*(_F+_I), *(_F+_I-1), temp); \
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
      CV_SWAP(*(_F), *(_F+_I), temp); \
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
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define cvAlloc      malloc
#define cvFree(_x)   (free(*(_x)), *(_x)=0)
#ifdef WIN32
#if _MSC_VER < 1400
#define snprintf     _snprintf
#endif
#else
//#define _snprintf   str_nprintf
#define _snprintf   snprintf
#define _vsnprintf   vsnprintf
#define _chdir   chdir
#endif
typedef struct rgb_color {
  BYTE r, g, b;
} rgb_color;
//base type
//typedef unsigned char   uchar;
#define NEW(T, _n)    ((T*)malloc(sizeof(T)*(_n)))
#define free_s(_p)   ((_p!=NULL)?(free((void*)(_p)), _p=0):(0))
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
//
CV_INLINE int is2pow(int i)
{
  return ((i > 0) && ((i & (i - 1)) == 0));//2??n???????????0??
}
//
CV_INLINE int ilog2(int value)
{
  int x = 0;
  while (value > 1) {
    value >>= 1;
    x++;
  }
  return x;
}
CV_INLINE size_t next_power(size_t n)
{
  size_t m = 1;
  while (m < n) {
    m <<= 1;
  }
  return m;
}
CV_INLINE size_t next_power_1_5(size_t n)
{
  size_t m = 32;
  while (m < n) {
    m = m*3/2;
  }
  return m;
}
#ifndef FP_NAN
#define FP_NORMAL    0/*正常的浮点数（不是以上结果中的任何一种）*/
#define FP_SUBNORMAL 1/*太小，以至于不能用浮点数的规格化形式表示*/
#define FP_ZERO      2/*是0*/
#define FP_INFINITE  3/*正、负无穷*/
#define FP_NAN       4/*not a number*/
#endif // FP_NAN
CV_INLINE int fpclassify_f32(float t) {
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
CV_INLINE int fpclassify_f64(double t) {
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
CV_INLINE BOOL IsFinite_f32(float t)
{
  return (!(IsNaN_f32(t) || IsInfinite_f32(t)));
}
CV_INLINE BOOL IsFinite_f64(double t)
{
  return (!(IsNaN_f64(t) || IsInfinite_f64(t)));
}
CV_INLINE float NaN_f32()
{
  union {
    float f;
    unsigned int i;
  } u;
  u.i = 0x7fC00000;
  return u.f;
}
CV_INLINE float Inf_f32()
{
  union {
    float f;
    unsigned int i;
  } u;
  u.i = 0x7f800000;
  return u.f;
}
CV_INLINE double NaN_f64()
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
CV_INLINE double Inf_f64()
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
#ifdef _MSC_VER
static int _vtrace_dbg(const char* fmt, myva_list argptr)
{
  void __stdcall OutputDebugStringA(const char* str);
  enum {_VT_BUF_LEN = 1 << 11};
  char buf[ _VT_BUF_LEN ] = {0};
  _vsnprintf(buf, _VT_BUF_LEN, fmt, argptr);
  OutputDebugStringA(buf);
  return 0;
}
static int _trace_dbg(const char* fmt, ...)
{
  myva_list argptr;
  myva_start(argptr, fmt);
  _vtrace_dbg(fmt, argptr);
  myva_end(argptr);
  return 0;
}
//#define printf _trace_dbg
#else
#define _trace_dbg printf
#endif
typedef struct my_mem_stage_t {
  unsigned int magic0;
  unsigned int n;
  int id;
  int line;
  const char* file;
  struct my_mem_stage_t* next;
  struct my_mem_stage_t* prev;
  unsigned int magic1;
} my_mem_stage_t;
static int g_mem_check_id = 0;
CV_INLINE my_mem_stage_t* my_mem_stage_opt(my_mem_stage_t* p0_add, my_mem_stage_t* p0_remove)
{
  static my_mem_stage_t* g_mem_stage = NULL;
  my_mem_stage_t* p0 = NULL;
  if (p0_add) {
    p0 = p0_add;
    if (g_mem_stage) {
      g_mem_stage->prev = p0;
      p0->next = g_mem_stage;
    }
    else {
      p0->next = NULL;
    }
    p0->prev = NULL;
    g_mem_stage = p0;
  }
  if (p0_remove) {
    p0 = p0_remove;
    if (p0->prev) {
      p0->prev->next = p0->next;
    }
    else {
      g_mem_stage = p0->next;
    }
    if (p0->next) {
      p0->next->prev = p0->prev;
    }
  }
  return g_mem_stage;
}
/*
static void test_debug_new() {
char* p = malloc(2);
MEM_CHECK_BEGIN();
p = malloc(2);
p[0] = 'A';
p[1] = 'B';
MEM_CHECK_END();
free(p);
}
MEM_CHECK_BEGIN();
MEM_CHECK_END();
*/
#define MEM_CHECK_BEGIN()  { mem_check_begin(); {
#define MEM_CHECK_END()    } mem_check_end(); }
CV_INLINE int mem_check_begin()
{
  g_mem_check_id = 1;
  return 0;
}
CV_INLINE int mem_check_end()
{
  my_mem_stage_t* p = my_mem_stage_opt(0, 0);
  int n=0;
  // 输出内存泄漏报告，显示在 Output 窗口的 Debug 页中
  for (; p; p = p->next, ++n) {
    if (p->id) {
      _trace_dbg("%s(%d) : adsf\n", p->file, p->line);
    }
  }
  g_mem_check_id = 0;
  return 0;
}
CV_INLINE int my_msize(void* p)
{
  int n = 0;
  if (p) {
    my_mem_stage_t* p0 = (my_mem_stage_t*)((char*)p - sizeof(my_mem_stage_t));
    n = p0->n;
  }
  return n;
}
CV_INLINE int my_free(void* p)
{
  if (p) {
    my_mem_stage_t* p0 = (my_mem_stage_t*)((char*)p - sizeof(my_mem_stage_t));
    my_mem_stage_t* p1 = (my_mem_stage_t*)((char*)p + p0->n);
    my_mem_stage_opt(0, p0);
    ASSERT(p0->magic0 == 0x11223344 && p0->magic1 == 0x55667788 && "头部越界");
    ASSERT(p1->magic0 == 0x11223344 && p1->magic1 == 0x55667788 && "尾部越界");
    free(p0);
  }
  return 0;
}
#define ALIGN(n, x)   (((n)+((x)-1))&(~((x)-1)))
CV_INLINE void* my_malloc(int newn, const void* file, int line)
{
  int n = newn<(1<<22) ? next_power(newn) : newn;
  int sz = n + 2 * sizeof(my_mem_stage_t);
  char* p = (char*)malloc(sz);
  my_mem_stage_t* p0 = (my_mem_stage_t*)p;
  my_mem_stage_t* p1 = (my_mem_stage_t*)(p + sz - sizeof(my_mem_stage_t));
  memset(p, 0, sz);
  p1->n = p0->n = n;
  p1->file = p0->file = (const char*)file;
  p1->line = p0->line = line;
  p1->magic0 = p0->magic0 = 0x11223344;
  p1->magic1 = p0->magic1 = 0x55667788;
  p1->id = p0->id = g_mem_check_id;
  my_mem_stage_opt(p0, 0);
  return p0 + 1;
}
CV_INLINE void* my_realloc(void* p, int newn, const void* file, int line)
{
  char* newp = NULL;
  int n = p ? my_msize(p) : 0;
  if (newn > n) {
    newn = MAX(newn, 16);
    while(newn<n) {
      newn = newn*3/2;
    }
    newp = (char*)my_malloc(newn, file, line);
    if (p) {
      memcpy(newp, p, n);
      my_free(p);
    }
    p = newp;
  }
  return p;
}
#define free(ptr) (my_free(ptr))
//#define free(ptr) ((ptr) ? (my_free((void*)(ptr)), ptr=0) : 0)
#define malloc(n) my_malloc(n, __FILE__, __LINE__)
#define calloc(x, n) my_malloc((x)*(n), __FILE__, __LINE__)
#define realloc(p, n) my_realloc(p, n, __FILE__, __LINE__)

#define MYREALLOC(_PTR, _N)      myrealloc((void**)(&(_PTR)), sizeof(*_PTR)*(_N), __FILE__, __LINE__)
CV_INLINE void myrealloc(void** ptr, int n, const void* file, int line) {
  *ptr=my_realloc(*ptr, n, file, line);
}
CV_INLINE void memset2(void* adr, int val, int count)
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
  q = (unsigned short*) p;
  n = count & 7;
  for (i = 0; i < n; i++) {
    *q++ = val;
  }
}
CV_INLINE void memset4(void* adr, int val, int count)
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
CV_INLINE void memset3(void* adr, int r, int v, int b, long count)
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
    pt+=3;
  }
}
// Data structures for persistence (a.k.a serialization) functionality        *
/* "black box" file storage */
typedef struct CvFileStorage CvFileStorage;
/* Storage flags: */
#define CV_STORAGE_READ          0
#define CV_STORAGE_WRITE         1
#define CV_STORAGE_WRITE_TEXT    CV_STORAGE_WRITE
#define CV_STORAGE_WRITE_BINARY  CV_STORAGE_WRITE
#define CV_STORAGE_APPEND        2
/* List of attributes: */
typedef struct CvAttrList {
  const char** attr;         /* NULL-terminated array of (attribute_name,attribute_value) pairs. */
  struct CvAttrList* next;   /* Pointer to next chunk of the attributes list.                    */
}
CvAttrList;
CV_INLINE CvAttrList cvAttrList(const char** attr CV_DEFAULT(NULL),
    CvAttrList* next CV_DEFAULT(NULL))
{
  CvAttrList l;
  l.attr = attr;
  l.next = next;
  return l;
}
struct CvTypeInfo;
#define CV_NODE_NONE        0
#define CV_NODE_INT         1
#define CV_NODE_INTEGER     CV_NODE_INT
#define CV_NODE_REAL        2
#define CV_NODE_FLOAT       CV_NODE_REAL
#define CV_NODE_STR         3
#define CV_NODE_STRING      CV_NODE_STR
#define CV_NODE_REF         4 /* not used */
#define CV_NODE_SEQ         5
#define CV_NODE_MAP         6
#define CV_NODE_TYPE_MASK   7
#define CV_NODE_TYPE(flags)  ((flags) & CV_NODE_TYPE_MASK)
/* file node flags */
#define CV_NODE_FLOW        8 /* Used only for writing structures in YAML format. */
#define CV_NODE_USER        16
#define CV_NODE_EMPTY       32
#define CV_NODE_NAMED       64
#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif
//                Compile-time tuning parameters                            *
/* maximal size of vector to run matrix operations on it inline (i.e. w/o ipp calls) */
#define  CV_MAX_INLINE_MAT_OP_SIZE  10
/* maximal linear size of matrix to allocate it on stack. */
#define  CV_MAX_LOCAL_MAT_SIZE  32
/* maximal size of local memory storage */
#define  CV_MAX_LOCAL_SIZE \
  (CV_MAX_LOCAL_MAT_SIZE*CV_MAX_LOCAL_MAT_SIZE*(int)sizeof(double))
/* default image row align (in bytes) */
#define  CV_DEFAULT_IMAGE_ROW_ALIGN  4
/* matrices are continuous by default */
#define  CV_DEFAULT_MAT_ROW_ALIGN  1
/* maximum size of dynamic memory buffer.
   cvAlloc reports an error if a larger block is requested. */
#define  CV_MAX_ALLOC_SIZE    (((size_t)1 << (sizeof(size_t)*8-2)))
/* the alignment of all the allocated buffers */
#define  CV_MALLOC_ALIGN    16
/* default alignment for dynamic data strucutures, resided in storages. */
#define  CV_STRUCT_ALIGN    ((int)sizeof(double))
/* default storage block size */
#define  CV_STORAGE_BLOCK_SIZE   ((1<<16) - 128)
/* default memory block for sparse array elements */
#define  CV_SPARSE_MAT_BLOCK    (1<<12)
/* initial hash table size */
#define  CV_SPARSE_HASH_SIZE0    (1<<10)
/* maximal average node_count/hash_size ratio beyond which hash table is resized */
#define  CV_SPARSE_HASH_RATIO    3
/* max length of strings */
#define  CV_MAX_STRLEN  1024
#if 0 /*def  CV_CHECK_FOR_NANS*/
#define CV_CHECK_NANS( arr ) cvCheckArray((arr))
#else
#define CV_CHECK_NANS( arr )
#endif
#define IPCVAPI_IMPL(type, name, args, arg_names) type name args
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
#define CV_DECL_ALIGNED(x) __attribute__ ((aligned (x)))
#elif defined _MSC_VER
#define CV_DECL_ALIGNED(x) __declspec(align(x))
#else
#define CV_DECL_ALIGNED(x)
#endif
/* ! DO NOT make it an inline function */
#define cvStackAlloc(size) cvAlignPtr( alloca((size) + CV_MALLOC_ALIGN), CV_MALLOC_ALIGN )
#if defined _MSC_VER || defined __BORLANDC__
#define CV_BIG_INT(n)   n##I64
#define CV_BIG_UINT(n)  n##UI64
#else
#define CV_BIG_INT(n)   n##LL
#define CV_BIG_UINT(n)  n##ULL
#endif
//#define CV_IMPL CV_EXTERN_C
#define CV_DBG_BREAK() { volatile int* crashMe = 0; *crashMe = 0; }
/* default step, set in case of continuous data
   to work around checks for valid step in some ipp functions */
#define  CV_STUB_STEP     (1 << 30)
#define  CV_SIZEOF_FLOAT ((int)sizeof(float))
#define  CV_SIZEOF_SHORT ((int)sizeof(short))
#define  CV_ORIGIN_TL  0
#define  CV_ORIGIN_BL  1
/* IEEE754 constants and macros */
#define  CV_POS_INF       0x7f800000
#define  CV_NEG_INF       0x807fffff /* CV_TOGGLE_FLT(0xff800000) */
#define  CV_1F            0x3f800000
#define  CV_TOGGLE_FLT(x) ((x)^((int)(x) < 0 ? 0x7fffffff : 0))
#define  CV_TOGGLE_DBL(x) \
  ((x)^((int64)(x) < 0 ? CV_BIG_INT(0x7fffffffffffffff) : 0))
#define  CV_NOP(a)      (a)
#define  CV_ADD(a, b)   ((a) + (b))
#define  CV_SUB(a, b)   ((a) - (b))
#define  CV_MUL(a, b)   ((a) * (b))
#define  CV_AND(a, b)   ((a) & (b))
#define  CV_OR(a, b)    ((a) | (b))
#define  CV_XOR(a, b)   ((a) ^ (b))
#define  CV_ANDN(a, b)  (~(a) & (b))
#define  CV_ORN(a, b)   (~(a) | (b))
#define  CV_SQR(a)      ((a) * (a))
#define  CV_LT(a, b)    ((a) < (b))
#define  CV_LE(a, b)    ((a) <= (b))
#define  CV_EQ(a, b)    ((a) == (b))
#define  CV_NE(a, b)    ((a) != (b))
#define  CV_GT(a, b)    ((a) > (b))
#define  CV_GE(a, b)    ((a) >= (b))
#define  CV_NONZERO(a)      ((a) != 0)
#define  CV_NONZERO_FLT(a)  (((a)+(a)) != 0)
#define  CV_PASTE2(a,b) a##b
#define  CV_PASTE(a,b)  CV_PASTE2(a,b)
/* general-purpose saturation macros */
#define  CV_CAST_8U(t)  (unsigned char)(!((t) & ~255) ? (t) : (t) > 0 ? 255 : 0)
#define  CV_CAST_8S(t)  (schar)(!(((t)+128) & ~255) ? (t) : (t) > 0 ? 127 : -128)
#define  CV_CAST_16U(t) (ushort)(!((t) & ~65535) ? (t) : (t) > 0 ? 65535 : 0)
#define  CV_CAST_16S(t) (short)(!(((t)+32768) & ~65535) ? (t) : (t) > 0 ? 32767 : -32768)
#define  CV_CAST_32S(t) (int)(t)
#define  CV_CAST_64S(t) (int64)(t)
#define  CV_CAST_32F(t) (float)(t)
#define  CV_CAST_64F(t) (double)(t)
#define  CV_PASTE2(a,b) a##b
#define  CV_PASTE(a,b)  CV_PASTE2(a,b)
#define  CV_EMPTY
#define  CV_MAKE_STR(a) #a
#define  CV_DEFINE_MASK \
  float maskTab[2]; maskTab[0] = 0.f; maskTab[1] = 1.f;
#define  CV_ANDMASK( m, x )     ((x) & (((m) == 0) - 1))
/* (x) * ((m) == 1 ? 1.f : (m) == 0 ? 0.f : <ERR> */
#define  CV_MULMASK( m, x )       (maskTab[(m) != 0]*(x))
/* (x) * ((m) == -1 ? 1.f : (m) == 0 ? 0.f : <ERR> */
#define  CV_MULMASK1( m, x )      (maskTab[(m)+1]*(x))
#define  CV_ZERO_OBJ(x)  memset((x), 0, sizeof(*(x)))
#define  CV_DIM(static_array) ((int)(sizeof(static_array)/sizeof((static_array)[0])))
#define  CV_UN_ENTRY_C1(worktype) worktype s0 = scalar[0]
#define  CV_UN_ENTRY_C2(worktype) worktype s0 = scalar[0], s1 = scalar[1]
#define  CV_UN_ENTRY_C3(worktype) worktype s0 = scalar[0], s1 = scalar[1], s2 = scalar[2]
#define  CV_UN_ENTRY_C4(worktype) worktype s0 = scalar[0], s1 = scalar[1], s2 = scalar[2], s3 = scalar[3]
#define  cvUnsupportedFormat "Unsupported format"
CV_INLINE void* cvAlignPtr(const void* ptr, int align CV_DEFAULT(32))
{
  assert((align & (align - 1)) == 0);
  return (void*)(((size_t)ptr + align - 1) & ~(size_t)(align - 1));
}
CV_INLINE int cvAlign(int size, int align)
{
  assert((align & (align - 1)) == 0 && size < INT_MAX);
  return (size + align - 1) & -align;
}
#define  CV_DESCALE(x,n)     (((x) + (1 << ((n)-1))) >> (n))
#define  CV_FLT_TO_FIX(x,n)  cvRound((x)*(1<<(n)))
typedef struct CvComplex32f {
  float re, im;
} CvComplex32f;
typedef struct CvComplex64f {
  double re, im;
} CvComplex64f;
CV_INLINE CvComplex32f cvComplex32f(float re, float im)
{
  CvComplex32f s;
  s.re = re;
  s.im = im;
  return s;
}
CV_INLINE CvComplex64f cvComplex64f(double re, double im)
{
  CvComplex64f s;
  s.re = re;
  s.im = im;
  return s;
}
//typedef struct TCOMPLEX { float re, im; } TCOMPLEX;
//typedef struct DCOMPLEX { double re, im; } DCOMPLEX;
#define FCOMPLEX_SET(c, RE, IM)   (c.re=RE, c.im=IM)
#define FCOMPLEX_ADD(c,a,b)       FCOMPLEX_SET( c, a.re + b.re, a.im + b.im )
#define FCOMPLEX_SUB(c,a,b)       FCOMPLEX_SET( a.re - b.re, a.im - b.im )
#define FCOMPLEX_NEG(c,a)         FCOMPLEX_SET( c, -a.re, -a.im )
#define FCOMPLEX_MUL(c,a,b)       FCOMPLEX_SET( c, a.re*b.re - a.im*b.im, a.re*b.im + a.im*b.re )
#define FCOMPLEX_ABS(a)           sqrt( a.re*a.re + a.im*a.im )
#define FCOMPLEX_CONJ(c, a)       FCOMPLEX_SET( c, a.re, -a.im )
#define FCOMPLEX_DIV(c,a,b)       ( c.im = 1./(b.re*b.re + b.im*b.im), FCOMPLEX_SET( c, ((a.re*b.re + a.im*b.im)*c.im), ((-a.re*b.im + a.im*b.re)*c.im) ) )
#define FCOMPLEX_SCALE(c,a,b)     FCOMPLEX_SET( c, (a.re*b), (a.im*b) )
#define CvComplex_set(c,a,b)      (c.re=a, c.im=b)
#define CvComplex_neg(c,a)        (c.re=-a.re, c.im=-a.im )
#define CvComplex_conj(c,a)       (c.re=a.re, c.im=-a.im )
#define CvComplex_add(c,a,b)      (c.re=a.re + b.re, c.im=a.im + b.im )
#define CvComplex_sub(c,a,b)      (c.re=a.re - b.re, c.im=a.im - b.im )
#define CvComplex_mul(c,a,b)      (c.re=a.re*b.re - a.im*b.im, c.im=a.re*b.im + a.im*b.re )
#define CvComplex_muladd(c,a,b)   (c.re+=a.re*b.re - a.im*b.im, c.im+=a.re*b.im + a.im*b.re )
#define CvComplex_mula(c,a,b)     (c.re=a*b.re, c.im=a*b.im )
#define CvComplex_div(c,a,b)      (c.im = 1./(b.re*b.re + b.im*b.im),c.re= (TYPE)((a.re*b.re + a.im*b.im)*c.im),c.im=(TYPE)((-a.re*b.im + a.im*b.re)*c.im) )
#define CvComplex_abs2(a)         (( a.re*a.re + a.im*a.im ))
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
#define CV_FAST_DIV_SHIFT 32
CV_INLINE CvFastDiv cvFastDiv(int divisor)
{
  CvFastDiv fastdiv;
  uint64 temp;
  assert(divisor >= 1);
  temp = ((uint64)1 << CV_FAST_DIV_SHIFT) / divisor;
  fastdiv.divisor = divisor;
  fastdiv.delta = (unsigned)(((temp & 1) ^ 1) + divisor - 1);
  fastdiv.scale = (unsigned)((temp + 1) >> 1);
  return fastdiv;
}
#define CV_FAST_DIV( x, fastdiv ) \
  ((int)(((int64)((x)*2 + (int)(fastdiv).delta))*(int)(fastdiv).scale>>CV_FAST_DIV_SHIFT))
#define CV_FAST_UDIV( x, fastdiv ) \
  ((int)(((uint64)((x)*2 + (fastdiv).delta))*(fastdiv).scale>>CV_FAST_DIV_SHIFT))
#define CV_MEMCPY_CHAR( dst, src, len ) \
  { \
    size_t _icv_memcpy_i_, _icv_memcpy_len_ = (len); \
    char* _icv_memcpy_dst_ = (char*)(dst); \
    const char* _icv_memcpy_src_ = (const char*)(src); \
    \
    for( _icv_memcpy_i_ = 0; _icv_memcpy_i_ < _icv_memcpy_len_; _icv_memcpy_i_++ ) \
      _icv_memcpy_dst_[_icv_memcpy_i_] = _icv_memcpy_src_[_icv_memcpy_i_]; \
  }
#define CV_MEMCPY_INT( dst, src, len ) \
  { \
    size_t _icv_memcpy_i_, _icv_memcpy_len_ = (len); \
    int* _icv_memcpy_dst_ = (int*)(dst); \
    const int* _icv_memcpy_src_ = (const int*)(src); \
    assert( ((size_t)_icv_memcpy_src_&(sizeof(int)-1)) == 0 && \
        ((size_t)_icv_memcpy_dst_&(sizeof(int)-1)) == 0 ); \
    \
    for(_icv_memcpy_i_=0;_icv_memcpy_i_<_icv_memcpy_len_;_icv_memcpy_i_++) \
      _icv_memcpy_dst_[_icv_memcpy_i_] = _icv_memcpy_src_[_icv_memcpy_i_]; \
  }
#define CV_MEMCPY_AUTO( dst, src, len ) \
  { \
    size_t _icv_memcpy_i_, _icv_memcpy_len_ = (len); \
    char* _icv_memcpy_dst_ = (char*)(dst); \
    const char* _icv_memcpy_src_ = (const char*)(src); \
    if( (_icv_memcpy_len_ & (sizeof(int)-1)) == 0 ) \
    { \
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
#define CV_ZERO_CHAR( dst, len ) \
  { \
    size_t _icv_memcpy_i_, _icv_memcpy_len_ = (len); \
    char* _icv_memcpy_dst_ = (char*)(dst); \
    \
    for( _icv_memcpy_i_ = 0; _icv_memcpy_i_ < _icv_memcpy_len_; _icv_memcpy_i_++ ) \
      _icv_memcpy_dst_[_icv_memcpy_i_] = '\0'; \
  }
#define CV_ZERO_INT( dst, len ) \
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
typedef enum CvStatus {
  CV_BADMEMBLOCK_ERR          = -113,
  CV_INPLACE_NOT_SUPPORTED_ERR = -112,
  CV_UNMATCHED_ROI_ERR        = -111,
  CV_NOTFOUND_ERR             = -110,
  CV_BADCONVERGENCE_ERR       = -109,
  CV_BADDEPTH_ERR             = -107,
  CV_BADROI_ERR               = -106,
  CV_BADHEADER_ERR            = -105,
  CV_UNMATCHED_FORMATS_ERR    = -104,
  CV_UNSUPPORTED_COI_ERR      = -103,
  CV_UNSUPPORTED_CHANNELS_ERR = -102,
  CV_UNSUPPORTED_DEPTH_ERR    = -101,
  CV_UNSUPPORTED_FORMAT_ERR   = -100,
  CV_BADARG_ERR      = -49,  //ipp comp
  CV_NOTDEFINED_ERR  = -48,  //ipp comp
  CV_BADCHANNELS_ERR = -47,  //ipp comp
  CV_BADRANGE_ERR    = -44,  //ipp comp
  CV_BADSTEP_ERR     = -29,  //ipp comp
  CV_BADFLAG_ERR     =  -12,
  CV_DIV_BY_ZERO_ERR =  -11, //ipp comp
  CV_BADCOEF_ERR     =  -10,
  CV_BADFACTOR_ERR   =  -7,
  CV_BADPOINT_ERR    =  -6,
  CV_BADSCALE_ERR    =  -4,
  CV_OUTOFMEM_ERR    =  -3,
  CV_NULLPTR_ERR     =  -2,
  CV_BADSIZE_ERR     =  -1,
  CV_FAIL     =  -1,
  CV_NO_ERR          =   0,
  CV_OK              =   CV_NO_ERR
}
CvStatus;
#define CV_ERROR_FROM_STATUS( result ) \
  CV_ERROR( cvErrorFromIppStatus( result ), "OpenCV function failed" )
#define CV_PLUGIN_NONE      0
#define CV_PLUGIN_OPTCV     1 /* custom "emerged" ippopencv library */
#define CV_PLUGIN_IPPCV     2 /* IPP: computer vision */
#define CV_PLUGIN_IPPI      3 /* IPP: image processing */
#define CV_PLUGIN_IPPS      4 /* IPP: signal processing */
#define CV_PLUGIN_IPPVM     5 /* IPP: vector math functions */
#define CV_PLUGIN_IPPCC     6 /* IPP: color space conversion */
#define CV_PLUGIN_MKL       8 /* Intel Math Kernel Library */
#define CV_PLUGIN_MAX      16
#define CV_PLUGINS1(lib1) ((lib1)&15)
#define CV_PLUGINS2(lib1,lib2) (((lib1)&15)|(((lib2)&15)<<4))
#define CV_PLUGINS3(lib1,lib2,lib3) (((lib1)&15)|(((lib2)&15)<<4)|(((lib2)&15)<<8))
#define CV_NOTHROW throw()
#ifndef IPCVAPI
#define IPCVAPI(type,declspec,name,args) \
  /* function pointer */ \
  typedef type (declspec* name##_t) args; \
  extern name##_t name##_p; \
  type declspec name args;
#endif
#define IPCVAPI_EX(type,name,ipp_name,ipp_search_modules,args) \
  IPCVAPI(type,CV_STDCALL,name,args)
#define IPCVAPI_C_EX(type,name,ipp_name,ipp_search_modules,args)\
  IPCVAPI(type,CV_CDECL,name,args)
#ifndef IPCVAPI_IMPL
#define IPCVAPI_IMPL(type,name,args,arg_names) \
  static type CV_STDCALL name##_f args; \
  name##_t name##_p = name##_f; \
  type CV_STDCALL name args { return name##_p arg_names; } \
  static type CV_STDCALL name##_f args
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
#define CV_BIG_INT(n)   n##I64
#define CV_BIG_UINT(n)  n##UI64
#else
#define CV_BIG_INT(n)   n##LL
#define CV_BIG_UINT(n)  n##ULL
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

#define INF  FLT_MAX
#define EPS  FLT_EPSILON
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
#if 0
/* helper tables */
//static const uchar icvSaturate8u[] = {#include table/icvSaturate8u.txt};
#define CV_FAST_CAST_8U(t)  (assert(-256 <= (t) || (t) <= 512), icvSaturate8u_cv[(t)+256])
#else
#define CV_FAST_CAST_8U(t)  ((t)<0 ? 0 : ((t)>255 ? 255 : (t)))
#endif
#define CV_CALC_MIN_8U(a,b) (a) -= CV_FAST_CAST_8U((a) - (b))
#define CV_CALC_MAX_8U(a,b) (a) += CV_FAST_CAST_8U((b) - (a))
#define  CV_COPY( dst, src, len, idx )     for( (idx) = 0; (idx) < (len); (idx)++) (dst)[idx] = (src)[idx]
#define  CV_SET( dst, val, len, idx )      for( (idx) = 0; (idx) < (len); (idx)++) (dst)[idx] = (val)
#undef   CV_CALC_MIN
#define  CV_CALC_MIN(a, b) if((a) > (b)) (a) = (b)
#undef   CV_CALC_MAX
#define  CV_CALC_MAX(a, b) if((a) < (b)) (a) = (b)
/* Image smooth methods */
enum {
  CV_BLUR_NO_SCALE = 0,
  CV_BLUR  = 1,
  CV_GAUSSIAN  = 2,
  CV_MEDIAN = 3,
  CV_BILATERAL = 4
};
/* Filters used in pyramid decomposition */
enum {
  CV_GAUSSIAN_5x5 = 7
};
/* Inpainting algorithms */
enum {
  CV_INPAINT_NS      = 0,
  CV_INPAINT_TELEA   = 1
};
/* Special filters */
enum {
  CV_SCHARR = -1,
  CV_MAX_SOBEL_KSIZE = 7
};
/* Sub-pixel interpolation methods */
enum {
  CV_INTER_NN        = 0,
  CV_INTER_LINEAR    = 1,
  CV_INTER_CUBIC     = 2,
  CV_INTER_AREA      = 3,
  CV_INTER_LANCZOS4  = 4,
  CV_INTER_DOWN,
};
/* ... and other image warping flags */
enum {
  CV_WARP_FILL_OUTLIERS = 8,
  CV_WARP_INVERSE_MAP  = 16
};
/* Shapes of a structuring element for morphological operations */
enum {
  CV_SHAPE_RECT      = 0,
  CV_SHAPE_CROSS     = 1,
  CV_SHAPE_ELLIPSE   = 2,
  CV_SHAPE_CUSTOM    = 100
};
/* Morphological operations */
enum {
  CV_MOP_ERODE        = 0,
  CV_MOP_DILATE       = 1,
  CV_MOP_OPEN         = 2,
  CV_MOP_CLOSE        = 3,
  CV_MOP_GRADIENT     = 4,
  CV_MOP_TOPHAT       = 5,
  CV_MOP_BLACKHAT     = 6
};
/* Template matching methods */
enum {
  CV_TM_SQDIFF        = 0,
  CV_TM_SQDIFF_NORMED = 1,
  CV_TM_CCORR         = 2,
  CV_TM_CCORR_NORMED  = 3,
  CV_TM_CCOEFF        = 4,
  CV_TM_CCOEFF_NORMED = 5
};
/* Contour retrieval modes */
enum {
  CV_RETR_EXTERNAL = 0,
  CV_RETR_LIST = 1,
  CV_RETR_CCOMP = 2,
  CV_RETR_TREE = 3,
  CV_RETR_FLOODFILL = 4
};
/* Contour approximation methods */
enum {
  CV_CHAIN_CODE = 0,
  CV_CHAIN_APPROX_NONE = 1,
  CV_CHAIN_APPROX_SIMPLE = 2,
  CV_CHAIN_APPROX_TC89_L1 = 3,
  CV_CHAIN_APPROX_TC89_KCOS = 4,
  CV_LINK_RUNS = 5
};
/* initializes 8-element array for fast access to 3x3 neighborhood of a pixel */
#define  CV_INIT_3X3_DELTAS( deltas, step, nch ) \
  ((deltas)[0] =  (nch),  (deltas)[1] = -(step) + (nch), \
      (deltas)[2] = -(step), (deltas)[3] = -(step) - (nch), \
      (deltas)[4] = -(nch),  (deltas)[5] =  (step) - (nch), \
      (deltas)[6] =  (step), (deltas)[7] =  (step) + (nch))
//                Planar subdivisions                                       *
typedef enum CvSubdiv2DPointLocation {
  CV_PTLOC_ERROR = -2,
  CV_PTLOC_OUTSIDE_RECT = -1,
  CV_PTLOC_INSIDE = 0,
  CV_PTLOC_VERTEX = 1,
  CV_PTLOC_ON_EDGE = 2
}
CvSubdiv2DPointLocation;
typedef enum CvNextEdgeType {
  CV_NEXT_AROUND_ORG   = 0x00,
  CV_NEXT_AROUND_DST   = 0x22,
  CV_PREV_AROUND_ORG   = 0x11,
  CV_PREV_AROUND_DST   = 0x33,
  CV_NEXT_AROUND_LEFT  = 0x13,
  CV_NEXT_AROUND_RIGHT = 0x31,
  CV_PREV_AROUND_LEFT  = 0x20,
  CV_PREV_AROUND_RIGHT = 0x02
}
CvNextEdgeType;
/* get the next edge with the same origin point (counterwise) */
#define  CV_SUBDIV2D_NEXT_EDGE( edge )  (((CvQuadEdge2D*)((edge) & ~3))->next[(edge)&3])
/* Contour approximation algorithms */
enum {
  CV_POLY_APPROX_DP = 0
};
/* Shape matching methods */
enum {
  CV_CONTOURS_MATCH_I1  = 1,
  CV_CONTOURS_MATCH_I2  = 2,
  CV_CONTOURS_MATCH_I3  = 3
};
/* Shape orientation */
enum {
  CV_CLOCKWISE         = 1,
  CV_COUNTER_CLOCKWISE = 2
};
/* Histogram comparison methods */
enum {
  CV_COMP_CORREL        = 0,
  CV_COMP_CHISQR        = 1,
  CV_COMP_INTERSECT     = 2,
  CV_COMP_BHATTACHARYYA = 3
};
/* Mask size for distance transform */
enum {
  CV_DIST_MASK_3   = 3,
  CV_DIST_MASK_5   = 5,
  CV_DIST_MASK_PRECISE = 0
};
/* Distance types for Distance Transform and M-estimators */
enum {
  CV_DIST_USER    = -1, /* User defined distance */
  CV_DIST_L1      = 1,  /* distance = |x1-x2| + |y1-y2| */
  CV_DIST_L2      = 2,  /* the simple euclidean distance */
  CV_DIST_C       = 3,  /* distance = max(|x1-x2|,|y1-y2|) */
  CV_DIST_L12     = 4,  /* L1-L2 metric: distance = 2(sqrt(1+x*x/2) - 1)) */
  CV_DIST_FAIR    = 5,  /* distance = c^2(|x|/c-log(1+|x|/c)), c = 1.3998 */
  CV_DIST_WELSCH  = 6,  /* distance = c^2/2(1-exp(-(x/c)^2)), c = 2.9846 */
  CV_DIST_HUBER   = 7   /* distance = |x|<c ? x^2/2 : c(|x|-c/2), c=1.345 */
};
/* Threshold types */
enum {
  CV_THRESH_BINARY      = 0, /* value = value > threshold ? max_value : 0       */
  CV_THRESH_BINARY_INV  = 1, /* value = value > threshold ? 0 : max_value       */
  CV_THRESH_TRUNC       = 2, /* value = value > threshold ? threshold : value   */
  CV_THRESH_TOZERO      = 3, /* value = value > threshold ? value : 0           */
  CV_THRESH_TOZERO_INV  = 4, /* value = value > threshold ? 0 : value           */
  CV_THRESH_MASK        = 7,
  CV_THRESH_OTSU        = 8  /* use Otsu algorithm to choose the optimal threshold value;
                                 combine the flag with one of the above CV_THRESH_* values */
};
/* Adaptive threshold methods */
enum {
  CV_ADAPTIVE_THRESH_MEAN_C  = 0,
  CV_ADAPTIVE_THRESH_GAUSSIAN_C  = 1
};
/* FloodFill flags */
enum {
  CV_FLOODFILL_FIXED_RANGE = (1 << 16),
  CV_FLOODFILL_MASK_ONLY   = (1 << 17)
};
/* Canny edge detector flags */
enum {
  CV_CANNY_L2_GRADIENT  = (1 << 31)
};
/* Variants of a Hough transform */
enum {
  CV_HOUGH_STANDARD = 0,
  CV_HOUGH_PROBABILISTIC = 1,
  CV_HOUGH_MULTI_SCALE = 2,
  CV_HOUGH_GRADIENT = 3
};
/* Fast search data structures  */
struct CvFeatureTree;
struct CvLSH;
struct CvLSHOperations;
#include <stdio.h>
#ifdef __GNUC__
#define CV_Error( code, err ) printf("%s:%d: error: (%d) %s in function %s\n", __FILE__, __LINE__, code, err, __func__)
#else
#define CV_Error( code, err ) printf("%s:%d: error: (%d) %s\n", __FILE__, __LINE__, code, err)
#endif
#define CV_Error_( code, args ) CV_Error(code, format args)
#define CV_Assert( expr ) { if(!(expr)) CV_Error(CV_StsAssert, #expr); }
#ifdef _DEBUG
#define CV_DbgAssert(expr) CV_Assert(expr)
#else
#define CV_DbgAssert(expr)
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
  m.a=SHIFT_TOINT(A), m.b=SHIFT_TOINT(B), m.c=SHIFT_TOINT(C), m.d=SHIFT_TOINT(D), m.e=SHIFT_TOINT(E), m.f=SHIFT_TOINT(F)
// |out_x|   |a c e|   |x|
// |out_y| = |b d f| * |y|
// |     |   |0 0 0|   |1|
#define MATRIX3X2_MUL_POINT(m, x, y, out_x, out_y)  out_x=m.a*(x)+m.c*(y)+m.e, out_y=m.b*(x)+m.d*(y)+m.f
//m=x*y
// |a c e|   |y.a y.c y.e|   |x.a x.c x.e|   | (a1*a2+c1*b2) ()
// |b d f| = |y.b y.d y.f| * |x.b x.d x.f| = |
// |0 0 0|   |y.0 y.0 y.0|   |x.0 x.0 x.0|   |
#define MATRIX3X2_MUL_MATRIX3X2(m, x, y)  MAKE_MATRIX3X2(m, \
    (int)SHIFT_TOFLOAT((double)x.a * y.a + (double)x.b * y.c      ), \
    (int)SHIFT_TOFLOAT((double)x.a * y.b + (double)x.b * y.d      ), \
    (int)SHIFT_TOFLOAT((double)x.c * y.a + (double)x.d * y.c      ), \
    (int)SHIFT_TOFLOAT((double)x.c * y.b + (double)x.d * y.d      ), \
    (int)SHIFT_TOFLOAT((double)x.e * y.a + (double)x.f * y.c + y.e), \
    (int)SHIFT_TOFLOAT((double)x.e * y.b + (double)x.f * y.d + y.f) )
//x*=y
#define MATRIX3X2_MUL(x, y) do { MATRIX3X2 _m; MATRIX3X2_MUL_MATRIX3X2(_m, x, y); x=_m; } while(0)
#define MATRIX3X2_GET_TRANSLATE(m, x, y)     MAKE_MATRIX3X2(m,1,0,0,1,x,y)
#define MATRIX3X2_GET_SCALE(m1, s, x, y)     MAKE_MATRIX3X2(m1,s,0,0,s, (s)*(x), (s)*(y) )
#define MATRIX3X2_INIT(m)  MAKE_MATRIX3X2(m, SHIFT1,0,0,SHIFT1,0,0)
#define MATRIX3X2_MOVE(m, x, y)  (m.e-=SHIFT_TOINT(x), m.f-=SHIFT_TOINT(y))
#define MATRIX3X2_ROTATE(m, rads)  {\
    int c=SHIFT_TOINT(cos(rads)), s=SHIFT_TOINT(sin(rads)); \
    MATRIX3X2 _x; MAKE_MATRIX3X2(_x, c,s,-s,c, 0, 0); MATRIX3X2_MUL(m, _x); }
// | c, s|
// |-s, c|
// | 0, 0|
#define MATRIX3X2_GET_ROTATE(m, rads, x, y) do { \
    int c=SHIFT_TOINT(cos(rads)), s=SHIFT_TOINT(sin(rads)); \
    MAKE_MATRIX3X2(m, c,s,-s,c,-(y)*c+(x)*s+x,-(y)*s+(x)*c+y); \
  } while(0)
#define MATRIX3X2_GET_ROTATE(m, rads, x, y) do { \
    int c=SHIFT_TOINT(cos(rads)), s=SHIFT_TOINT(sin(rads)); \
    MAKE_MATRIX3X2(m, c,s,-s,c,-(y)*c+(x)*s+x,-(y)*s+(x)*c+y); \
  } while(0)
#define MATRIX3X2_GET_STD_TOINT(m, c, s, s1, s2) \
  MAKE_MATRIX3X2_TOINT(m, c, -s, s, c, ((-s2.x*(c) - s2.y*(s))+s1.x), ((s2.x*(s) - s2.y*(c))+s1.y));
#define CV_AUTOSTEP  0x7fffffff
/**************** matrix iterator: used for n-ary operations on dense arrays *********/
#define CV_MAX_ARR 10
#define CV_NO_DEPTH_CHECK     1
#define CV_NO_CN_CHECK        2
#define CV_NO_SIZE_CHECK      4
///* Finds selected eigen values and vectors of a symmetric matrix */
//CVAPI(void)  cvSelectedEigenVV( CvArr* mat, CvArr* evects, CvArr* evals,
//                                int lowindex, int highindex );
/* Calculates covariation matrix for a set of vectors */
/* transpose([v1-avg, v2-avg,...]) * [v1-avg,v2-avg,...] */
#define CV_COVAR_SCRAMBLED 0
/* [v1-avg, v2-avg,...] * transpose([v1-avg,v2-avg,...]) */
#define CV_COVAR_NORMAL    1
/* do not calc average (i.e. mean vector) - use the input vector instead
   (useful for calculating covariance matrix by parts) */
#define CV_COVAR_USE_AVG   2
/* scale the covariance matrix coefficients by number of the vectors */
#define CV_COVAR_SCALE     4
/* all the input vectors are stored in a single matrix, as its rows */
#define CV_COVAR_ROWS      8
/* all the input vectors are stored in a single matrix, as its columns */
#define CV_COVAR_COLS     16
#define CV_PCA_DATA_AS_ROW 0
#define CV_PCA_DATA_AS_COL 1
#define CV_PCA_USE_AVG 2
//                      Array Statistics                                    *
/* types of array norm */
#define CV_C            1
#define CV_L1           2
#define CV_L2           4
#define CV_NORM_MASK    7
#define CV_RELATIVE     8
#define CV_DIFF         16
#define CV_MINMAX       32
#define CV_DIFF_C       (CV_DIFF | CV_C)
#define CV_DIFF_L1      (CV_DIFF | CV_L1)
#define CV_DIFF_L2      (CV_DIFF | CV_L2)
#define CV_RELATIVE_C   (CV_RELATIVE | CV_C)
#define CV_RELATIVE_L1  (CV_RELATIVE | CV_L1)
#define CV_RELATIVE_L2  (CV_RELATIVE | CV_L2)
#define CV_REDUCE_SUM 0
#define CV_REDUCE_AVG 1
#define CV_REDUCE_MAX 2
#define CV_REDUCE_MIN 3
//        Discrete Linear Transforms and Related Functions                  *
#define CV_DXT_FORWARD  0
#define CV_DXT_INVERSE  1
#define CV_DXT_SCALE    2 /* divide result by size of array */
#define CV_DXT_INV_SCALE (CV_DXT_INVERSE + CV_DXT_SCALE)
#define CV_DXT_INVERSE_SCALE CV_DXT_INV_SCALE
#define CV_DXT_ROWS     4 /* transform each row individually */
#define CV_DXT_MUL_CONJ 8 /* conjugate the second argument of cvMulSpectrums */
#define CV_DXT_ABS 16
#define CV_DXT_ABSQ 32
/* Retrieves index of a graph vertex given its pointer */
#define cvGraphVtxIdx( graph, vtx ) ((vtx)->flags & CV_SET_ELEM_IDX_MASK)
/* Retrieves index of a graph edge given its pointer */
#define cvGraphEdgeIdx( graph, edge ) ((edge)->flags & CV_SET_ELEM_IDX_MASK)
#define cvGraphGetVtxCount( graph ) ((graph)->active_count)
#define cvGraphGetEdgeCount( graph ) ((graph)->edges->active_count)
#define  CV_GRAPH_VERTEX        1
#define  CV_GRAPH_TREE_EDGE     2
#define  CV_GRAPH_BACK_EDGE     4
#define  CV_GRAPH_FORWARD_EDGE  8
#define  CV_GRAPH_CROSS_EDGE    16
#define  CV_GRAPH_ANY_EDGE      30
#define  CV_GRAPH_NEW_TREE      32
#define  CV_GRAPH_BACKTRACKING  64
#define  CV_GRAPH_OVER          -1
#define  CV_GRAPH_ALL_ITEMS    -1
/* flags for graph vertices and edges */
#define  CV_GRAPH_ITEM_VISITED_FLAG  (1 << 30)
#define  CV_IS_GRAPH_VERTEX_VISITED(vtx) \
  (((CvGraphVtx*)(vtx))->flags & CV_GRAPH_ITEM_VISITED_FLAG)
#define  CV_IS_GRAPH_EDGE_VISITED(edge) \
  (((CvGraphEdge*)(edge))->flags & CV_GRAPH_ITEM_VISITED_FLAG)
#define  CV_GRAPH_SEARCH_TREE_NODE_FLAG   (1 << 29)
#define  CV_GRAPH_FORWARD_EDGE_FLAG       (1 << 28)
//               ??????

#define _1K   (1024)
#define _1M   ((_1K)*(_1K))
#define _1G   ((_1K)*(_1M))
#define CV_PI   (3.14159265358979323846264338327950288419716939937511)
#define CV_LOG2 0.69314718055994530941723212145818
#define _LOG2   CV_LOG2
#ifndef PI
#define PI      CV_PI
#endif
//#define CV_SWAP(a,b,t) ((t) = (a), (a) = (b), (b) = (t))
#ifndef MIN
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif
#ifndef MAX
#define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif
#define MINMAX(x1, x2, t)  if ((x1)>(x2)) { CV_SWAP((x1), (x2), t); }
/* min & max without jumps */
#define  CV_IMIN(a, b)  ((a) ^ (((a)^(b)) & (((a) < (b)) - 1)))
#define  CV_IMAX(a, b)  ((a) ^ (((a)^(b)) & (((a) > (b)) - 1)))
/* absolute value without jumps */
#ifndef __cplusplus
#define  CV_IABS(a)     (((a) ^ ((a) < 0 ? -1 : 0)) - ((a) < 0 ? -1 : 0))
#else
#define  CV_IABS(a)     abs(a)
#endif
#define  CV_CMP(a,b)    (((a) > (b)) - ((a) < (b)))
#define  CV_SIGN(a)     CV_CMP((a),0)
#define bzero(arr, n)  memset(arr, 0, n)
#ifndef _GMALLOC_H_
#define gmalloc malloc
#define grealloc realloc
#define gfree free
#define GMALLOC_BEGIN(g_buf, GBUFLEN) (0)
#define GMALLOC_END() (0)
#endif // _GMALLOC_H_
#define MALLOC(_TYPE, _N)             (_TYPE*)malloc((unsigned int)(sizeof(_TYPE)*(_N)))
#define CALLOC(_TYPE, _N)             (_TYPE*)calloc((unsigned int)(sizeof(_TYPE)*(_N)))
#define REALLOC(_TYPE, _PTR, _N)      _PTR=(_TYPE*)realloc(_PTR, (unsigned int)(sizeof(_TYPE)*(_N)))
#define FREE(ptr)              ((ptr) ? (free((void*)(ptr)), ptr=0) : 0)
#define NFREE(ptr, _N)         FREE(ptr)
#define GMALLOC(_TYPE, _N)            (_TYPE*)malloc((unsigned int)(sizeof(_TYPE)*(_N)))
#define GREALLOC(_TYPE, _PTR, _N)     _PTR=(_TYPE*)realloc(_PTR, (unsigned int)(sizeof(_TYPE)*(_N)))
#define GFREE(_PTR)             (gfree((void*)(_PTR)), _PTR = NULL)
//#define GSAFEFREE(_PTR)         if (NULL!=_PTR) { gfree(_PTR); _PTR = NULL; }
//#define SAFEFREE(_PTR)         if (NULL!=_PTR) { free(_PTR); _PTR = NULL; }
#define MEMSET(_F, _V, _N)     memset(_F, _V, sizeof(*(_F))*(_N))
#define MEMZERO(_F, _N)        memset(_F, 0, sizeof(*(_F))*(_N))
#define MEMCPY(_P, _F, _N)     memcpy(_P, _F, sizeof(*(_F))*(_N))
#ifndef MEMMOVE
#define MEMMOVE(_P, _F, _N)    memmove(_P, _F, sizeof(*(_F))*(_N))
#endif
#define SAFEFCLOSE(_PF)        if (NULL!=_PF) { fclose(_PF); _PF = NULL; }
#define MALLOCSET(_TYPE, _N, _V) (_TYPE*)memset(malloc(_N*sizeof(_TYPE)), _V, _N*sizeof(_TYPE))
#ifndef MEM_CHECK_BEGIN
#define MEM_CHECK_BEGIN()
#define MEM_CHECK_END()
#endif
#define MEM_GET16B(p, i, x) x = (i+=2, (((uchar*)p)[i-2]<<8) | (((uchar*)p)[i-1]))
#define MEM_GET16L(p, i, x) x = (i+=2, (((uchar*)p)[i-1]<<8) | (((uchar*)p)[i-2]))
#define MEM_GET32B(p, i, x) x = (i+=4, (((uchar*)p)[i-4]<<24) | (((uchar*)p)[i-3]<<16) | (((uchar*)p)[i-2]<<8) | (((uchar*)p)[i-1]))
#define MEM_GET32L(p, i, x) x = (i+=4, (((uchar*)p)[i-1]<<24) | (((uchar*)p)[i-2]<<16) | (((uchar*)p)[i-3]<<8) | (((uchar*)p)[i-4]))

#define MEM_SET16B(p, i, x) (((uchar*)p)[i+0] = ((x)>>8), ((uchar*)p)[i+1] = (x)&0xff, i+=2)
#define MEM_SET16L(p, i, x) (((uchar*)p)[i+1] = ((x)>>8), ((uchar*)p)[i+0] = (x)&0xff, i+=2)
#define MEM_SET32B(p, i, x) (((uchar*)p)[i+0] = ((x)>>24), ((uchar*)p)[i+1] = ((x)>>16), ((uchar*)p)[i+2] = ((x)>>8), ((uchar*)p)[i+3] = (x), i+=4)
#define MEM_SET32L(p, i, x) (((uchar*)p)[i+3] = ((x)>>24), ((uchar*)p)[i+2] = ((x)>>16), ((uchar*)p)[i+1] = ((x)>>8), ((uchar*)p)[i+0] = (x), i+=4)
#define MEM_SETMEM(p, i, x, n) (memcpy((uchar*)p + i, x, n), i+=n)

typedef union {
  uint32 u;
  int32 s;
  float f;
} inttofloat;
CV_INLINE uint32 IR(float x)
{
  inttofloat tmp;
  tmp.f = x;
  return tmp.u;
}
CV_INLINE float FR_u32(uint32 x)
{
  inttofloat tmp;
  tmp.u = x;
  return tmp.f;
}
CV_INLINE float FR_s32(int32 x)
{
  inttofloat tmp;
  tmp.s = x;
  return tmp.f;
}
static void byterev(void* v, int n)
{
  char* s = (char*)v;
  char* e = s + n - 1, t;
  for (; s < e; ++s, --e) {
    t = *s;
    *s = *e;
    *e = t;
  }
}
#define bswap_16(X) ((((X)&0xFF) << 8) | (((X)&0xFF00) >> 8))
#define bswap_32(X) ( (((X)&0x000000FF)<<24) | (((X)&0xFF000000) >> 24) | (((X)&0x0000FF00) << 8) | (((X) &0x00FF0000) >> 8))
#define BYTESWAP_16(X)  byteswap_16(&(X))
#define BYTESWAP_32(X)  byteswap_32(&(X))
#define BYTEORDERL(X)  if ('b'==ENDIANNESS) {BYTESWAP(X);}
#define BYTEORDERB(X)  if ('b'!=ENDIANNESS) {BYTESWAP(X);}
#define BYTEORDER16L(X)  if ('b'==ENDIANNESS) {byteswap_16(&X);}
#define BYTEORDER16B(X)  if ('b'!=ENDIANNESS) {byteswap_16(&X);}
#define BYTEORDER32L(X)  if ('b'==ENDIANNESS) {byteswap_32(&X);}
#define BYTEORDER32B(X)  if ('b'!=ENDIANNESS) {byteswap_32(&X);}
CV_INLINE void byteswap_16(void* x)
{
  uchar* ux = (uchar*)x, t;
  CV_SWAP(ux[0], ux[1], t);
}
CV_INLINE void byteswap_32(void* x)
{
  uchar* ux = (uchar*)x, t;
  CV_SWAP(ux[0], ux[3], t);
  CV_SWAP(ux[1], ux[2], t);
}
#define BYTESWAP(t)   byteswap(sizeof(t), &t)
CV_INLINE void byteswap(int size, void* x)
{
  uchar* ux = (uchar*)x, t;
  int i;
  switch (size) {
  case 1:
    break;
  case 2:
    CV_SWAP(ux[0], ux[1], t);
    break;
  case 3:
    CV_SWAP(ux[0], ux[2], t);
    break;
  case 4:
    CV_SWAP(ux[0], ux[3], t);
    CV_SWAP(ux[1], ux[2], t);
    break;
  default:
    for (i = 0; i < size / 2; ++i) {
      CV_SWAP(ux[i], ux[size - 1 - i], t);
    }
    break;
  }
}

typedef struct buf_t {
  int len;
  unsigned char* data;
} buf_t;
#define bfinit(bf, _p, _n)  (bf->len=_n, bf->data = (unsigned char*)_p)
#define bfsetsize(bf, newlen)  (bf->len=newlen, bf->data = (unsigned char*)realloc(bf->data, newlen))
#define bffree(bf)   if (bf->data) {(free(bf->data), bf->data=0, bf->len=0);}
#define bffrees2(bf, n)   {int _i=0; for (_i=0; _i<(n); ++_i) {bffree((bf+_i));}}
#define bffrees(bf)   bffrees2(bf, countof(bf))
#define bfdata(bf, T)  ((T*)(bf->data))
#define bfsize(bf, T)  ((bf->len)/sizeof(T))
#define bfsetsizeT(bf, newlen, T)  bfsetsize(bf, (newlen)*sizeof(T))
#define bfzero(bf)   memset((bf)->data, 0, (bf)->len)
CV_INLINE int bfinsert(buf_t* bf, int bypos, const void* data, int datalen)
{
  int oldlen = bf->len;
  int newlen = MAX(oldlen, bypos) + datalen;
  bfsetsize(bf, newlen);
  if (bypos < oldlen) {
    memmove(bf->data + bypos + datalen, bf->data + bypos, oldlen - bypos);
  }
  else {
    memset(bf->data + oldlen, 0, bypos - oldlen);
  }
  memcpy(bf->data + bypos, data, datalen);
  return 0;
}
CV_INLINE void* matcpy(void* dst, int dl, const void* src, int sl, int h, int w)
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
CV_INLINE void* matmove(void* dst, int dl, const void* src, int sl, int h, int w)
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
//#define my_realloc(p, n, newn) realloc(p, newn)
#define MEMINST(P, N, I, F, M)  (meminst(P, (N)*sizeof(*(P)), (I)*sizeof(*(P)), F, (M)*sizeof(*(P)))/sizeof(*(P))  )
CV_INLINE int meminst(void* p, int n, int i, const void* f, int m)
{
  memmove((char*)p + i + m, (char*)p + i, n - i);
  if (f) {
    memcpy((char*)p + i, f, m);
  }
  return m + n;
}
// del m byte from p+i
#define MEMDEL(P, N, I, M)  (memdel(P, (N)*sizeof(*(P)), (I)*sizeof(*(P)), (M)*sizeof(*(P)))/sizeof(*(P))  )
CV_INLINE int memdel(void* p, int n, int i, int m)
{
  memcpy((char*)p + i, (char*)p + i + m, n - i - m);
  return n - m;
}
#define MEMINS(_T, _P, _N, _POS, _NEWN)  _P=(_T*)my_memins(_P, _N, _POS, _NEWN, sizeof(_T))
#define MEMPUSH(_T, _P, _N)  MEMINS(_T, _P, _N, _N, 1)
CV_INLINE void* my_memins(void* p, int n, int pos, int newn, int elemsize)
{
  n *= elemsize, pos *= elemsize, newn *= elemsize;
  myrealloc(&p, MAX(n, pos) + newn, __FILE__, __LINE__);
  memmove((char*)p + pos + newn, (char*)p + pos, n - pos);
  return p;
}
#define SZMIN(sz1, sz2)  iSIZE(MIN(sz1.cx, sz2.cx), MIN(sz1.cy, sz2.cy))
CV_INLINE void* mat_realloc(void* p, int h, int w, int new_h, int new_w)
{
  char* newp;
  int newn = new_w * new_h;
  int min_h = MIN(h, new_h), min_w = MIN(w, new_w);
  newp = (char*)gmalloc(newn);
  memset(newp, 0, newn);
  if (p) {
    matcpy(newp, new_w, p, w, min_h, min_w);
    free(p);
  }
  return newp;
}
CV_INLINE void* mat_inscol(void* p, int h, int w, int pos, int n)
{
  int new_w = MAX(w, pos) + n;
  p = mat_realloc(p, h, w, h, new_w);
  if (w > pos) {
    matmove((char*)p + pos + n, new_w, (char*)p + pos, new_w, h, w - pos);
  }
  return p;
}
CV_INLINE void* mat_insrow(void* p, int h, int w, int pos, int n)
{
  int new_h = MAX(h, pos) + n;
  p = mat_realloc(p, h, w, new_h, w);
  if (h > pos) {
    matmove((char*)p + (pos + n)*w, w, (char*)p + pos * w, w, h - pos, w);
  }
  return p;
}
#define MEMCPY2D(_M, _N, _F, _FL, _P, _PL) \
  do { int _I; for (_I=0; _I<(_M); ++_I) \
      memcpy(_P+(_PL)*_I, _F+(_FL)*_I, _N*sizeof(*(_F))); \
  } while(0)
CV_INLINE int memcpy2da(void* B, int bl, const void* A, int al, int h, int w)
{
  int i;
  for (i = 0; i < h; ++i) {
    memcpy(((uchar*)B + i * bl), ((const uchar*)A + i * al), w);
  }
  return 0;
}
CV_INLINE int copy2d(int h, int w, const void* A, int al, void* B, int bl)
{
  int i;
  for (i = 0; i < h; ++i) {
    memcpy(((uchar*)B + i * bl), ((const uchar*)A + i * al), w);
  }
  return 0;
}
CV_INLINE void* memcpy2d(void* dst, int dl, const void* src, int sl, int h, int w)
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
CV_INLINE int memswap(int n, void* src, void* dst)
{
  int i, t0, t1, t2, t3;
  int* s = (int*) src;
  int* d = (int*) dst;
  char* cs = (char*) src;
  char* cd = (char*) dst;
  char ct;
  for (i = 0; i < n - 15; i += 16, s += 4, d += 4) {
    CV_SWAP(s[0], d[0], t0);
    CV_SWAP(s[1], d[1], t1);
    CV_SWAP(s[2], d[2], t2);
    CV_SWAP(s[3], d[3], t3);
  }
  for (; i < n; ++i) {
    CV_SWAP(cs[i], cd[i], ct);
  }
  return 0;
}
CV_INLINE int matflip(int h, int w, void* src, int srcstep)
{
  int i;
  char* cs = (char*) src;
  char* cd = (char*) src + (h - 1) * srcstep;
  for (i = 0; i < h / 2; ++i, cs += srcstep, cd -= srcstep) {
    memswap(w, cs, cd);
  }
  return 0;
}
CV_INLINE void* memflip(void* dst, int dl, const void* src, int sl, int h, int w)
{
  char* d = (char*)(dst);
  const char* s = (const char*)(src);
  if (d==s && dl==sl) {
    matflip(h, w, dst, dl);
  } else {
    d += (h - 1) * dl;
    for (; h--; s += sl, d -= dl) {
      memcpy(d, s, w);
    }
  }
  return 0;
}
#define MEMDUP(_T, _P, _N)  ((_T*)memdup(_P, (_N)*sizeof(_T)))
CV_INLINE void* memdup1(const void* pByte, uint32 dwSize)
{
  char* ret = (char*)malloc(dwSize + 1);
  memcpy(ret, pByte, dwSize);
  ret[dwSize] = 0;
  return ret;
}
CV_INLINE void* memdup(const void* pSrc, int Length)
{
  void* pNew = malloc(Length);
  memmove(pNew, pSrc, Length);
  return pNew;
}
CV_INLINE void* memdup2d(const void* A, int al, int h, int w)
{
  void* pNew = malloc(h * w);
  memcpy2d(pNew, w, A, al, h, w);
  return pNew;
}
CV_INLINE int memmap(int n, const uchar* src, const void* pmap, int mapcn, void* dst, int dstcn, int cn)
{
  int i, j;
  uchar* ud = (uchar*)dst;
  const uchar* um = (const uchar*)pmap;
  cn = BOUND(cn, 0, MIN(mapcn, dstcn));
  for (i = 0; i < n; ++i) {
    j = src[i];
    memcpy(ud + i * dstcn, um + j * mapcn, cn);
  }
  return 0;
}
CV_INLINE int mem_insert(void* pp, int* n, int pos, const void* q, int m, int elemsize)
{
  void** p = (void**)pp;
  char* pc = 0;
  myrealloc(p, (*n + m) * elemsize, __FILE__, __LINE__);
  pc = (char*)(*p);
  memmove(pc + (pos + m)*elemsize, pc + pos * elemsize, (*n - pos)*elemsize);
  if (q) {
    memcpy(pc + pos * elemsize, q, m * elemsize);
  }
  *n += m;
  return 0;
}
CV_INLINE int mem_push_back(void* pp, int* n, const void* q, int m, int elemsize)
{
  return mem_insert(pp, n, *n, q, m, elemsize);
}
typedef uint32 COLOR;
#define FILL2D_IMPL_BEGIN()   for (y = 0; y < h; ++y) { uchar* pix = ptr + y*al;for (x=0; x<w; ++x, pix+=ai) {
#define FILL2D_IMPL_END()     }}
CV_INLINE int fill2d(int h, int w, void* A, int al, int ai, COLOR color)
{
  int x, y;
  uchar* ptr = (uchar*)A;
  uchar* pclr = (uchar*)&color;
  int _a = pclr[3];
  if (255 == _a) {
    for (x = 0; x < w; ++x) {
      memcpy(ptr + (x)*ai, &color, ai);
    }
    for (y = 1; y < h; ++y) {
      memcpy(ptr + y * al, A, w * ai);
    }
  }
  else {
    switch (ai) {
    case 1:
      FILL2D_IMPL_BEGIN();
      pix[0] += ((pclr[0] - pix[0]) * _a) >> 8;
      FILL2D_IMPL_END();
      break;
    case 2:
      FILL2D_IMPL_BEGIN();
      pix[0] += ((pclr[0] - pix[0]) * _a) >> 8;
      pix[1] += ((pclr[1] - pix[1]) * _a) >> 8;
      FILL2D_IMPL_END();
      break;
    case 3:
    case 4:
      FILL2D_IMPL_BEGIN();
      pix[0] += ((pclr[0] - pix[0]) * _a) >> 8;
      pix[1] += ((pclr[1] - pix[1]) * _a) >> 8;
      pix[2] += ((pclr[2] - pix[2]) * _a) >> 8;
      FILL2D_IMPL_END();
      break;
    }
  }
  return 0;
}
#undef FILL2D_IMPL_BEGIN
#undef FILL2D_IMPL_END
#define REVCOPY(_B, _A, _N)   { int _i=0; for (; _i<(_N); ++_i) {(_B)[(_N)-1-_i]=(_A)[_i]; } }
//#define ARRREV(_N, _A, t)   { int _i=0; for (; _i<(_N)/2; ++_i) { CV_SWAP((_A)[(_N)-_i-1], (_A)[_i], t); } }
#define ARRREV(_N, _A, t)   {int _i=0; for (;_i<(_N)/2; ++_i) {t=(_A)[_i], (_A)[_i]=(_A)[_N-1-_i], (_A)[_N-1-_i]=t;} }
CV_INLINE int memrev(void* A, int elemsize, int n)
{
  switch (elemsize) {
#define MEMREVCPY_CASE(cn)  case cn: {typedef struct {uchar t[cn];} arrtype; arrtype t, * a0=(arrtype*)A; ARRREV(n, a0, t); } break
    MEMREVCPY_CASE(1);
    MEMREVCPY_CASE(2);
    MEMREVCPY_CASE(3);
    MEMREVCPY_CASE(4);
    MEMREVCPY_CASE(5);
    MEMREVCPY_CASE(6);
    MEMREVCPY_CASE(7);
    MEMREVCPY_CASE(8);
    MEMREVCPY_CASE(9);
    MEMREVCPY_CASE(10);
    MEMREVCPY_CASE(11);
    MEMREVCPY_CASE(12);
    MEMREVCPY_CASE(13);
    MEMREVCPY_CASE(14);
    MEMREVCPY_CASE(15);
    MEMREVCPY_CASE(16);
    MEMREVCPY_CASE(17);
    MEMREVCPY_CASE(18);
    MEMREVCPY_CASE(19);
    MEMREVCPY_CASE(20);
    MEMREVCPY_CASE(21);
    MEMREVCPY_CASE(22);
    MEMREVCPY_CASE(23);
    MEMREVCPY_CASE(24);
    MEMREVCPY_CASE(25);
    MEMREVCPY_CASE(26);
    MEMREVCPY_CASE(27);
    MEMREVCPY_CASE(28);
    MEMREVCPY_CASE(29);
    MEMREVCPY_CASE(30);
    MEMREVCPY_CASE(31);
    MEMREVCPY_CASE(32);
    MEMREVCPY_CASE(33);
    MEMREVCPY_CASE(34);
    MEMREVCPY_CASE(35);
    MEMREVCPY_CASE(36);
    MEMREVCPY_CASE(37);
    MEMREVCPY_CASE(38);
    MEMREVCPY_CASE(39);
  default:
    ASSERT(0);
    break;
  }
#undef MEMREVCPY_CASE
  return 0;
}
CV_INLINE int memrevcpy(void* B, const void* A, int elemsize, int n)
{
  switch (elemsize) {
#define MEMREVCPY_CASE(cn)  case cn: {typedef struct {uchar t[cn];} arrtype; const arrtype* a0=(const arrtype*)A; arrtype* b0=(arrtype*)B; REVCOPY(b0, a0, n); } break
    MEMREVCPY_CASE(1);
    MEMREVCPY_CASE(2);
    MEMREVCPY_CASE(3);
    MEMREVCPY_CASE(4);
    MEMREVCPY_CASE(5);
    MEMREVCPY_CASE(6);
    MEMREVCPY_CASE(7);
    MEMREVCPY_CASE(8);
    MEMREVCPY_CASE(9);
    MEMREVCPY_CASE(10);
    MEMREVCPY_CASE(11);
    MEMREVCPY_CASE(12);
    MEMREVCPY_CASE(13);
    MEMREVCPY_CASE(14);
    MEMREVCPY_CASE(15);
    MEMREVCPY_CASE(16);
    MEMREVCPY_CASE(17);
    MEMREVCPY_CASE(18);
    MEMREVCPY_CASE(19);
    MEMREVCPY_CASE(20);
    MEMREVCPY_CASE(21);
    MEMREVCPY_CASE(22);
    MEMREVCPY_CASE(23);
    MEMREVCPY_CASE(24);
    MEMREVCPY_CASE(25);
    MEMREVCPY_CASE(26);
    MEMREVCPY_CASE(27);
    MEMREVCPY_CASE(28);
    MEMREVCPY_CASE(29);
    MEMREVCPY_CASE(30);
    MEMREVCPY_CASE(31);
    MEMREVCPY_CASE(32);
    MEMREVCPY_CASE(33);
    MEMREVCPY_CASE(34);
    MEMREVCPY_CASE(35);
    MEMREVCPY_CASE(36);
    MEMREVCPY_CASE(37);
    MEMREVCPY_CASE(38);
    MEMREVCPY_CASE(39);
  default:
    ASSERT(0);
    break;
  }
#undef MEMREVCPY_CASE
  return 0;
}
CV_INLINE int data2men1(FILE* fpout, const void* ca, int len, int addr, int isdat)
{
  int i;
  const char* HEXFOMAT = isdat ? "0x%02x%02x%02x%02x\r\n" : "0x%02x%02x%02x%02x,\r\n";
  if (isdat) {
    fprintf(fpout, "1651 1 %x 0 %x\r\n", addr, (len + 3) / 4);
  }
  for (i = 0; i < len - 3; i += 4) {
    unsigned char* r_data = (unsigned char*) ca + i;
    fprintf(fpout, HEXFOMAT, r_data[ 3 ], r_data[ 2 ], r_data[ 1 ], r_data[ 0 ]);
  }
  if (i < len) {
    unsigned char buf[ 4 ] = { 0 };
    for (; i < len; ++i) {
      buf[ i ] = *((unsigned char*) ca + i);
    }
    fprintf(fpout, HEXFOMAT, buf[ 3 ], buf[ 2 ], buf[ 1 ], buf[ 0 ]);
  }
  return 0;
}
#define BF_MEM_MAGIC  (0x55667788)
#define ALIGN_TO(_X, ALIGN) ( ((size_t)(_X) + ALIGN - 1) & ~(size_t)(ALIGN - 1) )
typedef struct bf_mem_head_t {
  int size;
  //int used;
  uint magic;
} bf_mem_head_t;
typedef struct bf_mem_tail_t {
  uint magic;
  //int line;
  //char file[256];
} bf_mem_tail_t;
#define BF_MEM_INFO_LEN (sizeof(bf_mem_head_t)+sizeof(bf_mem_tail_t))
#define BFMALLOC(_BF, _P, _N)  bf_malloc(_BF, (void**)&(_P), (_N)*sizeof(*(_P)))
#define BFFREE(_BF, _P, _N)    bf_free(_BF, (void**)&(_P), (_N)*sizeof(*(_P)))
#define BF_MALLOC(_T, _P, _N)  BFMALLOC(bf, _P, _N)
#define BF_FREE(_T, _P, _N)    BFFREE(bf, _P, _N)
#define SYSMALLOC(_T, _P, _N)  (_P = (_T*)malloc((_N)*sizeof(*(_P))))
#define SYSFREE(_T, _P, _N)    (free(_P), _P=NULL)
//static int bf_maxlen=0;
//static int bf_malloc_maxlen=0;
CV_INLINE int bf_malloc(buf_t* bf, void** p, int n)
{
  uchar* p0 = 0;
  bf_mem_head_t* head;
  bf_mem_tail_t* tail;
  int len = (n + BF_MEM_INFO_LEN);
  ASSERT(len <= bf->len && "?????");
  //bf_maxlen = MAX(bf_maxlen, bf->len);
  //bf_malloc_maxlen = MAX(bf_maxlen, bf->len-len);
  bf->len -= len;
  p0 = bf->data + bf->len;
  head = (bf_mem_head_t*)p0;
  tail = (bf_mem_tail_t*)(p0 + len - sizeof(bf_mem_tail_t));
  head->magic = BF_MEM_MAGIC, head->size = n;
  tail->magic = BF_MEM_MAGIC;
  *p = p0 + sizeof(bf_mem_head_t);
  return 0;
}
CV_INLINE int bf_free(buf_t* bf, void** p, int n)
{
  uchar* p0 = (uchar*)(*p);
  if (p0) {
    bf_mem_head_t* head = (bf_mem_head_t*)(p0 - sizeof(bf_mem_head_t));
    bf_mem_tail_t* tail = (bf_mem_tail_t*)(p0 + head->size);
    int len = head->size + BF_MEM_INFO_LEN;
    ASSERT((bf->data + bf->len + sizeof(bf_mem_head_t)) == p0 && "????????");
    ASSERT(head->size == n && "??锟斤拷?????");
    ASSERT(head->magic == BF_MEM_MAGIC && "??????");
    ASSERT(tail->magic == BF_MEM_MAGIC && "锟斤拷?????");
    bf->len += len;
    *p = NULL;
  }
  return 0;
}
//                   ???
typedef struct IDIM {
  int len[2];
} IDIM;
CV_INLINE IDIM iDIM(int len0, int len1)
{
  IDIM sz;
  sz.len[0] = len0, sz.len[1] = len1;
  return sz;
}
#define VOP1_1(v, op, v1)       (v[0] op v1[0])
#define VOP1_2(v, op, v1)       (v[0] op v1[0], v[1] op v1[1])
#define VOP1_3(v, op, v1)       (v[0] op v1[0], v[1] op v1[1], v[2] op v1[2])
#define VOP1_4(v, op, v1)       (v[0] op v1[0], v[1] op v1[1], v[2] op v1[2], v[3] op v1[3])
#define VOP1a_1(v, op1, v1, op2, a)   (v[0] op1 v1[0] op2 (a))
#define VOP1a_2(v, op1, v1, op2, a)   (v[0] op1 v1[0] op2 (a), v[1] op1 v1[1] op2 (a))
#define VOP1a_3(v, op1, v1, op2, a)   (v[0] op1 v1[0] op2 (a), v[1] op1 v1[1] op2 (a), v[2] op1 v1[2] op2 (a))
#define VOP1a_4(v, op1, v1, op2, a)   (v[0] op1 v1[0] op2 (a), v[1] op1 v1[1] op2 (a), v[2] op1 v1[2] op2 (a), v[3] op1 v1[3] op2 (a))
#define VOP2_1(v, op1, v1, op2, v2)  (v[0] op1 v1[0] op2 v2[0])
#define VOP2_2(v, op1, v1, op2, v2)  (v[0] op1 v1[0] op2 v2[0], v[1] op1 v1[1] op2 v2[1])
#define VOP2_3(v, op1, v1, op2, v2)  (v[0] op1 v1[0] op2 v2[0], v[1] op1 v1[1] op2 v2[1], v[2] op1 v1[2] op2 v2[2])
#define VOP2_4(v, op1, v1, op2, v2)  (v[0] op1 v1[0] op2 v2[0], v[1] op1 v1[1] op2 v2[1], v[2] op1 v1[2] op2 v2[2], v[3] op1 v1[3] op2 v2[3])
#define VOP2a_1(v, op1, v1, op2, v2, a)  (v[0] op1 (v1[0] op2 v2[0]) * (a))
#define VOP2a_2(v, op1, v1, op2, v2, a)  (v[0] op1 (v1[0] op2 v2[0]) * (a), v[1] op1 (v1[1] op2 v2[1]) * (a))
#define VOP2a_3(v, op1, v1, op2, v2, a)  (v[0] op1 (v1[0] op2 v2[0]) * (a), v[1] op1 (v1[1] op2 v2[1]) * (a), v[2] op1 (v1[2] op2 v2[2]) * (a))
#define VOP2a_4(v, op1, v1, op2, v2, a)  (v[0] op1 (v1[0] op2 v2[0]) * (a), v[1] op1 (v1[1] op2 v2[1]) * (a), v[2] op1 (v1[2] op2 v2[2]) * (a), v[3] op1 (v1[3] op2 v2[3]) * (a))
#define VOP2b_1(v, op1, v1, op2, v2, b)  (v[0] op1 v1[0] op2 v2[0] * (b))
#define VOP2b_2(v, op1, v1, op2, v2, b)  (v[0] op1 v1[0] op2 v2[0] * (b), v[1] op1 v1[1] op2 v2[1] * (b))
#define VOP2b_3(v, op1, v1, op2, v2, b)  (v[0] op1 v1[0] op2 v2[0] * (b), v[1] op1 v1[1] op2 v2[1] * (b), v[2] op1 v1[2] op2 v2[2] * (b))
#define VOP2b_4(v, op1, v1, op2, v2, b)  (v[0] op1 v1[0] op2 v2[0] * (b), v[1] op1 v1[1] op2 v2[1] * (b), v[2] op1 v1[2] op2 v2[2] * (b), v[3] op1 v1[3] op2 v2[3] * (b))
#define VSET2(v, r, g)       (v[0] = r, v[1] = g)
#define VSET3(v, r, g, b)    (v[0] = r, v[1] = g, v[2] = b)
#define VSET4(v, r, g, b, a) (v[0] = r, v[1] = g, v[2] = b, v[3] = a)
#define VSET2V(v, v1)       VSET2(v, v1[0], v1[1])
#define VSET3V(v, v1)       VSET3(v, v1[0], v1[1], v1[2])
#define VSET4V(v, v1)       VSET4(v, v1[0], v1[1], v1[2], v1[3])
#define VSET3_(v, r, g, b)   VSET4(v, r, g, b, 1.f)
#define VSET2_(v, r, g, b)   VSET3(v, r, g, 1.f)
#define VADD2(v, v1, v2)  VSET2(v, v1[0]+v2[0], v1[1]+v2[1])
#define VADD3(v, v1, v2)  VSET3(v, v1[0]+v2[0], v1[1]+v2[1], v1[2]+v2[2])
#define VSUB2(v, v1, v2)  VSET2(v, v1[0]-v2[0], v1[1]-v2[1])
#define VSUB3(v, v1, v2)  VSET3(v, v1[0]-v2[0], v1[1]-v2[1], v1[2]-v2[2])
#define VSCALE3(v, v1, a)  VSET3(v, v1[0]*(a), v1[1]*(a), v1[2]*(a))
#define VDOT2(v1, v2)   (v1[0]*v2[0]+v1[1]*v2[1])
#define VDOT3(v1, v2)   (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define VLSQ2(v)   VDOT2(v, v)
#define VLSQ3(v)   VDOT3(v, v)
#define VLEN2(v)   sqrt(VDOT2(v, v))
#define VLEN3(v)   sqrt(VDOT3(v, v))
#define _SQ(x)  (x)*(x)
#define VDISQ2(v1, v2)   (_SQ(v1[0]-v2[0])+_SQ(v1[1]-v2[1]))
#define VDISQ3(v1, v2)   (_SQ(v1[0]-v2[0])+_SQ(v1[1]-v2[1])+_SQ(v1[2]-v2[2]))
#define VDIS2(v1, v2)   sqrt(VDISQ2(v1, v2))
#define VDIS3(v1, v2)   sqrt(VDISQ3(v1, v2))
#define VCROSS3(v, v1, v2) VSET3(v, v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2], v1[0] * v2[1] - v1[1] * v2[0])
//
typedef enum _pen_alignment_e {AlignmentNul = 0, AlignmentInset, AlignmentCenter, AlignmentLeft, AlignmentOutset, AlignmentRight } pen_alignment_e;
typedef enum _line_cap_e { round_cap, butt_cap, square_cap } line_cap_e;
typedef enum _line_join_e {round_join, miter_join , miter_join_revert , bevel_join , miter_join_round ,} line_join_e;
typedef enum _inner_join_e {inner_round, inner_bevel, inner_miter, inner_jag,} inner_join_e;
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
} FPOINT4;
typedef FPOINT4 F4VECTOR;
typedef struct SPOINT {
  short x, y;
} SPOINT;
typedef struct IPOINT {
  int x, y;
} IPOINT;
typedef struct DPOINT {
  double x, y;
} DPOINT;
typedef DPOINT D2VECTOR;
typedef struct FPOINT {
  float x, y;
} FPOINT;
typedef FPOINT F2VECTOR;
typedef struct ISIZE {
  int w, h;
} ISIZE;
typedef struct FSIZE {
  float w, h;
} FSIZE;
typedef struct DSIZE {
  double w, h;
} DSIZE;
typedef struct IRECT {
  int l, t, r, b;
} IRECT;
typedef struct FRECT {
  float l, t, r, b;
} FRECT;
typedef struct DRECT {
  double l, t, r, b;
} DRECT;
typedef struct IRANGE {
  int l, r;
} IRANGE;
typedef struct DRANGE {
  double l, r;
} DRANGE;
typedef struct DMATRIX33 {
  double a00, a01, a02, a10, a11, a12, a20, a21, a22;
} DMATRIX33;
typedef struct ISEGMENT {
  IPOINT p0, p1;
} ISEGMENT;
typedef struct DSEGMENT {
  DPOINT p0, p1;
} DSEGMENT;
typedef struct DSEGMENT3 {
  DPOINT3 p0, p1, p2;
} DSEGMENT3;
typedef DPOINT DPOINT2;
typedef FPOINT CvPoint2D32f;
//typedef DPOINT3 DPOINT3;
typedef DSEGMENT DSEGMENT2;
typedef struct vertex_i {
  int x, y, cmd;
} vertex_i;
typedef struct row_info {
  int x1, x2;
  uint32* ptr;
} row_info;
typedef struct rect_i {
  int x1, y1, x2, y2;
} rect_i;
typedef struct rect_d {
  double x1, y1, x2, y2;
} rect_d;
CV_INLINE IRANGE iRANGE(int l, int r)
{
  IRANGE ra;
  ra.l = l, ra.r = r;
  return ra;
}
CV_INLINE DRANGE dRANGE(double l, double r)
{
  DRANGE ra;
  ra.l = l, ra.r = r;
  return ra;
}
#define iPOINT_PT(pt)  iPOINT((int)(pt).x, (int)(pt).y)
CV_INLINE IPOINT iPOINT(int x, int y)
{
  IPOINT pt;
  pt.x = x, pt.y = y;
  return pt;
}
CV_INLINE int iPOINT_cmp(IPOINT a, IPOINT b)
{
  return a.y == b.y ? CV_CMP(a.x, b.x) : CV_CMP(a.y, b.y);
}
CV_INLINE FPOINT fPOINT(double x, double y)
{
  FPOINT p;
  p.x = (float) x;
  p.y = (float) y;
  return p;
}
CV_INLINE DPOINT dPOINT(double x, double y)
{
  DPOINT pt;
  pt.x = x, pt.y = y;
  return pt;
}
CV_INLINE IRANGE cRANGE(int l, int r)
{
  IRANGE ra;
  ra.l = l, ra.r = r;
  return ra;
}
CV_INLINE FPOINT iPointTof(IPOINT point)
{
  return fPOINT((float) point.x, (float) point.y);
}
CV_INLINE IPOINT fPointToi(FPOINT point)
{
  IPOINT ipt;
  ipt.x = (int)(point.x + .5);
  ipt.y = (int)(point.y + .5);
  return ipt;
}
CV_INLINE FPOINT3 fPOINT3(double x, double y, double z)
{
  FPOINT3 p;
  p.x = (float) x, p.y = (float) y, p.z = (float) z;
  return p;
}
CV_INLINE DSEGMENT dSEGMENT(double x0, double y0, double x1, double y1)
{
  DSEGMENT d;
  d.p0 = dPOINT(x0, y0), d.p1 = dPOINT(x1, y1);
  return d;
}
CV_INLINE ISEGMENT iSEGMENT(int x0, int y0, int x1, int y1)
{
  ISEGMENT d;
  d.p0 = iPOINT(x0, y0), d.p1 = iPOINT(x1, y1);
  return d;
}
// CvSize's & CvBox
typedef ISIZE CvSize;
CV_INLINE CvSize cvSize(int width, int height)
{
  CvSize s;
  s.w = width, s.h = height;
  return s;
}
typedef struct CvSize2D32f {
  float w, h;
} CvSize2D32f;
CV_INLINE CvSize2D32f cvSize2D32f(double width, double height)
{
  CvSize2D32f s;
  s.w = (float) width;
  s.h = (float) height;
  return s;
}
typedef struct CvBox2D {
  FPOINT center; /* Center of the box. */
  CvSize2D32f size; /* Box width and length. */
  float angle; /* Angle between the horizontal axis */
  /* and the first side (i.e. length) in degrees */
}
CvBox2D;
/* Line iterator state: */
typedef struct CvLineIterator {
  /* Pointer to the current point: */
  uchar* ptr;
  /* Bresenham algorithm state: */
  int err;
  int plus_delta;
  int minus_delta;
  int plus_step;
  int minus_step;
}
CvLineIterator;
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
CV_INLINE ISIZE iSIZE(int w, int h)
{
  ISIZE sz;
  sz.w = w, sz.h = h;
  return sz;
}
CV_INLINE IRECT iRECT(int l, int t, int r, int b)
{
  IRECT rc;
  RCSET(&rc, l, t, r, b);
  return rc;
}
CV_INLINE IRECT iRECT1(int x)
{
  IRECT rc;
  RCSET(&rc, x, x, x, x);
  return rc;
}
CV_INLINE IRECT iRECT_ps(IPOINT pt, ISIZE sz)
{
  IRECT rc;
  RCSET(&rc, pt.x, pt.y, pt.x + sz.w, pt.y + sz.h);
  return rc;
}
CV_INLINE IRECT iRECT_pp(IPOINT p1, IPOINT p2)
{
  IRECT rc;
  RCSET(&rc, p1.x, p1.y, p2.x, p2.y);
  return rc;
}
#define iRECT3(pt, sz) iRECT2(pt.x, pt.y, sz.w, sz.h)
CV_INLINE IRECT iRECT2(int x, int y, int w, int h)
{
  IRECT rc;
  RCSET(&rc, x, y, x + w, y + h);
  return rc;
}
CV_INLINE DRECT dRECT(double l, double t, double r, double b)
{
  DRECT rc;
  RCSET(&rc, l, t, r, b);
  return rc;
}
CV_INLINE DRECT dRECT2(double l, double t, double w, double h)
{
  DRECT rc;
  RCSET(&rc, l, t, l + w, t + h);
  return rc;
}

#define DT_TOP                             0x00000000 // 顶对齐, 须 or DT_SINGLELINE
#define DT_LEFT                            0x00000000 // 左对齐
#define DT_CENTER                          0x00000001 // 水平居中
#define DT_RIGHT                           0x00000002 // 右对齐
#define DT_VCENTER                         0x00000004 // 垂直居中 只对DT_SINGLELINE有效
#define DT_BOTTOM                          0x00000008 // 底对齐
#define DT_WORDBREAK                       0x00000010 // 给超出边界的单词换行
#define DT_SINGLELINE                      0x00000020 // 单行文本 忽略\n
#define DT_EXPANDTABS                      0x00000040 // 扩展制表符, 默认制表跨 8 个字符 将“\t”显示为空格
#define DT_TABSTOP                         0x00000080 // 后添加的字符数
#define DT_NOCLIP                          0x00000100 // 是否执行裁剪
#define DT_EXTERNALLEADING                 0x00000200 // 返回的高度是否包括字体的外部行距
#define DT_CALCRECT                        0x00000400 // 自动检测容纳文本的矩形的高度和宽度, 并返回高度, 但文本不被绘制
#define DT_NOPREFIX                        0x00000800 // 不处理前缀字符(&); 默认时, & 后的字符会被添加下划线
#define DT_INTERNAL                        0x00001000 // 用系统字体来计算文本高度

#define DT_EDITCONTROL                     0x00002000
#define DT_PATH_ELLIPSIS                   0x00004000 // 对于显示的字符串，会用省略号替换字符串中间的字符，以确保结果能在矩形范围内。 如果该字符串包含反斜杠（\）字符，它会尽可能的保留最后一个反斜杠之后的正文。
#define DT_END_ELLIPSIS                    0x00008000 // 当文字无法全部显示在给定区域时，用“…”替换无法显示的部分文字
#define DT_MODIFYSTRING                    0x00010000
#define DT_RTLREADING                      0x00020000
#define DT_WORD_ELLIPSIS                   0x00040000 // 截短不符合矩形的正文，并增加省略号。
#define DT_NOFULLWIDTHCHARBREAK            0x00080000
#define DT_HIDEPREFIX                      0x00100000 // 是否将“&”解析为修饰前缀；是否显示修饰结果或只显示修饰结果
#define DT_PREFIXONLY                      0x00200000 // 将 & 解释为前缀

#define DT_WRAPLINE                        (1<<25)

//#define WRAP_NOWRAP           (0) // 不自动换行
#define WRAP_WIDGETWIDTH      (1<<27) // 在窗口部件的当前宽度自动换行（这是默认的）。默认在空白符号处自动换行，这可以使用setWrapPolicy()来改变。
//#define WRAP_FIXEDPIXELWIDTH  (1<<28) // 从窗口部件的左侧开始的固定数量的象素数自动换行。象素的数量可以通过wrapColumnOrWidth()来设置。
#define WRAP_FIXEDCOLUMNWIDTH (1<<28) // 从窗口部件左侧开始的固定数量的列数自动换行。列数可以通过wrapColumnOrWidth()设置。如果你需要使用等宽文本在设备上显示很好的格式文本，这是很有用的，例如标准的VT100终端，你可以把wrapColumnOrWidth()设置为80。 
#define WRAP_ATWHITESPACE     (1<<39) // 在空白符号处（空格或者换行）自动换行。否则是任何
//#define WRAP_ANYWHERE         (1<<31) // 在任何情况下自动换行，包括单字中。

CV_INLINE int iRectIsEmpty(const IRECT* rc)
{
  return rc->l >= rc->r || rc->t >= rc->b;
}
CV_INLINE int iRectNormalize(IRECT* prc)
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
CV_INLINE int iRectInclude(IRECT r1, IRECT r2)
{
  iRectNormalize(&r1);
  iRectNormalize(&r2);
  return r1.l <= r2.l && r1.t <= r2.l && r1.r >= r2.r && r1.b >= r2.b;
}
CV_INLINE IRECT iRectInter(IRECT r1, IRECT r2)
{
  IRECT temp = {0};
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
CV_INLINE BOOL iRectIsNull(const IRECT* pRect)
{
  return (pRect->l >= pRect->r || pRect->t >= pRect->b);
}
CV_INLINE BOOL iRectSet(IRECT* pRect, int l, int t, int r, int b)
{
  pRect->l = l, pRect->r = r, pRect->t = t, pRect->b = b;
  return 0;
}
CV_INLINE BOOL iRectSetEmpty(IRECT* pRect)
{
  pRect->l = pRect->r = pRect->t = pRect->b = 0;
  return 0;
}

#define iRectIn(rc, d)  iRectOffset(rc, d, d, -(d), -(d))
#define iRectOff2(rc, x, y)  iRectOffset((rc), x, y, x, y)
#define iRectOff0(rc)  iRectOff2((rc), -(rc)->l, -(rc)->t)
CV_INLINE int iRectOffset(IRECT* rc, int l, int t, int r, int b)
{
  IRECT* out = rc;
  out->l = rc->l + l, out->t = rc->t + t;
  out->r = rc->r + r, out->b = rc->b + b;
  return 0;
}
CV_INLINE int iRectOffset2(IRECT* rc, int x, int y)
{
  IRECT* out = rc;
  out->l = rc->l + x, out->t = rc->t + y;
  out->r = rc->r + x, out->b = rc->b + y;
  return 0;
}
CV_INLINE IRECT iRectOffsetTo2(IRECT rc, int x, int y) {
  rc.l += x, rc.t += y;
  rc.r += x, rc.b += y;
  return rc;
}
CV_INLINE int iRectDeflate(IRECT* rc, int l, int t, int r, int b)
{
  IRECT* out = rc;
  out->l = rc->l + l, out->t = rc->t + t;
  out->r = rc->r - r, out->b = rc->b - b;
  return 0;
}
CV_INLINE int iRectDeflate2(IRECT* rc, int x, int y)
{
  IRECT* out = rc;
  out->l = rc->l + x, out->t = rc->t + y;
  out->r = rc->r - x, out->b = rc->b - y;
  return 0;
}
CV_INLINE int iRectInflate(IRECT* rc, int l, int t, int r, int b)
{
  IRECT* out = rc;
  out->l = rc->l - l, out->t = rc->t - t;
  out->r = rc->r + r, out->b = rc->b + b;
  return 0;
}
CV_INLINE int iRectOffsetXY(IRECT* rc, int x, int y)
{
  rc->l = rc->l + x, rc->t = rc->t + y;
  rc->r = rc->r + x, rc->b = rc->b + y;
  return 0;
}
CV_INLINE int iRectAdd(IRECT* rc, const IRECT* rc2)
{
  rc->l += rc2->l, rc->t += rc2->t;
  rc->r += rc2->r, rc->b += rc2->b;
  return 0;
}
CV_INLINE int iRectDeflateR(IRECT* rc, const IRECT* rc2)
{
  rc->l += rc2->l, rc->t += rc2->t;
  rc->r -= rc2->r, rc->b -= rc2->b;
  return 0;
}
CV_INLINE int iRectInflateR(IRECT* rc, const IRECT* rc2)
{
  rc->l -= rc2->l, rc->t -= rc2->t;
  rc->r += rc2->r, rc->b += rc2->b;
  return 0;
}
CV_INLINE int iRectInflate2(IRECT* rc, int x, int y)
{
  rc->l -= x, rc->t -= y;
  rc->r += x, rc->b += y;
  return 0;
}

CV_INLINE int iPtInRect(const IRECT* rc, int x, int y)
{
  return (rc->l <= x && x < rc->r) && (rc->t <= y && y < rc->b);
}

CV_INLINE int iPtInRects(int n, const IRECT* rc, int x, int y)
{
  int i;

  for (i = 0; i < n; ++i) {
    if (iPtInRect(rc + i, x, y)) {
      return i;
    }
  }

  return -1;
}

CV_INLINE int iPtInRectBorder(const IRECT* prc, int x, int y, int l, int t, int r, int b)
{
  IRECT rc = *prc;
  iRectOffset(&rc, -l, -t, r, b);

  if (iPtInRect(&rc, x, y) && !iPtInRect(prc, x, y)) {
    return (x < prc->l) << 0 | (x > prc->r) << 1 | (y < prc->t) << 2 | (y > prc->b) << 3;
  }

  return 0;
}
CV_INLINE IRECT iRectToSquare(IRECT rc)
{
  IRECT rc1 = rc;
  int d = MIN(RCH(&rc), RCW(&rc));
  rc1.r = rc.l + d, rc1.b = rc.t + d;
  return rc1;
}
// 将矩形框对齐
CV_INLINE int iRectAlign(IRECT rc, int cx, int cy, UINT uFmt, IRECT* out)
{
  *out = iRECT2(rc.l, rc.t, cx, cy);

  if (DT_CENTER & uFmt) {
    out->l = rc.l + (RCW(&rc) - cx) / 2;
  }

  if (DT_VCENTER & uFmt) {
    out->t = rc.t + (RCH(&rc) - cy) / 2;
  }

  if (DT_RIGHT & uFmt) {
    out->l = rc.r - cx;
  }

  if (DT_BOTTOM & uFmt) {
    out->t = rc.b - cy;
  }

  if ((DT_CENTER | DT_RIGHT | DT_LEFT) & uFmt) {
    out->r = out->l + cx;
  }

  if ((DT_VCENTER | DT_BOTTOM) & uFmt) {
    out->b = out->t + cy;
  }

  return 0;
}
// 绕点(x, y) 缩放矩形
CV_INLINE int iRectAddMulAdd(IRECT rc, double x, double y, double ox, double oy, double tx, double ty, IRECT* out) {
  out->l = (int)((rc.l+x)*ox+tx);
  out->r = (int)((rc.r+x)*ox+tx);
  out->t = (int)((rc.t+y)*oy+ty);
  out->b = (int)((rc.b+y)*oy+ty);
  return 0;
}
// 内嵌等比缩放
// 在矩形rc中切出宽高比为cx:cy的最大矩形
CV_INLINE int iRectScaling(IRECT rc, int cx, int cy, IRECT* out)
{
  int sw = cx, sh = cy;
  int dw = RCW(&rc), dh = RCH(&rc);
  int tw = dw, th = dh;
  int x = 0, y = 0;
  tw = sw * dh;
  th = sh * dw;

  if (cx <= 0 || cy <= 0) {
    return 0;
  }

  if (tw > th) {
    tw = dw, th /= sw, y = (dh - th) / 2;
  }
  else {
    th = dh, tw /= sh, x = (dw - tw) / 2;
  }

  out->l = x + rc.l;
  out->r = out->l + tw;
  out->t = y + rc.t;
  out->b = out->t + th;
  return 0;
}
// 把矩形框切分成大小相同的块，块按矩阵排列
CV_INLINE int iRectMatrix(IRECT rcIn, int row, int col, int gapx, int gapy, int n, IRECT* rc)
{
  int i;
  int cx, cy, h = RCH(&rcIn) - (row-1)*gapy, w = RCW(&rcIn) - (col-1)*gapx;
  cx = w / col + gapx, cy = h / row + gapy;

  for (i = 0; i < n; ++i) {
    rc[i].l = rcIn.l + (cx * (i % col));
    rc[i].r = rc[i].l + cx - gapx;
    rc[i].t = rcIn.t + (int)(cy * (i / col));
    rc[i].b = rc[i].t + cy - gapy;
  }

  return 0;
}
CV_INLINE int iRectMatrix2(IRECT rcIn, int cx, int cy, int gapx, int gapy, int n, IRECT* rc)
{
  int i, col, row;
  int h = RCH(&rcIn) + gapy, w = RCW(&rcIn) + gapx;
  cx += gapx, cy += gapy;
  col = w / cx, row = h / cy;
  n = MIN(n, row * col);

  for (i = 0; i < n; ++i) {
    rc[i].l = rcIn.l + (int)(cx * (i % col));
    rc[i].r = rc[i].l - gapx + (int)cx;
    rc[i].t = rcIn.t + (int)(cy * (i / col));
    rc[i].b = rc[i].t - gapy + (int)cy;
  }

  return 0;
}
// 从矩形框上边切出高为d的一块, 如果d<0则从下边切
CV_INLINE int iRectCutT(IRECT* rc, int d, int gap, IRECT* out)
{
  if (d >= 0) {
    *out = *rc, out->b = rc->t + d, rc->t = out->b + gap;
  }
  else {
    *out = *rc, out->t = rc->b + d, rc->b = out->t - gap;
  }

  return rc->t <= rc->b;
}
// 从矩形框左边切出宽为d的一块, 如果d<0则从右边切
CV_INLINE int iRectCutL(IRECT* rc, int d, int gap, IRECT* out)
{
  if (d >= 0) {
    *out = *rc, out->r = rc->l + d, rc->l = out->r + gap;
  }
  else {
    *out = *rc, out->l = rc->r + d, rc->r = out->l - gap;
  }

  return rc->l <= rc->r;
}
CV_INLINE int iRectCutB(IRECT* rc, int d, int gap, IRECT* out)
{
  *out = *rc, out->t = rc->b + d, rc->b = out->t - gap;
  return rc->t <= rc->b;
}
CV_INLINE int iRectCutR(IRECT* rc, int d, int gap, IRECT* out)
{
  *out = *rc, out->l = rc->r + d, rc->r = out->l - gap;
  return rc->l <= rc->r;
}
CV_INLINE int iRectImageText(IRECT rc, int n, ISIZE* sz, int gap, UINT uFmt, BOOL ishoriz, IRECT* prc)
{
  ISIZE szall = iSIZE(sz[0].w, sz[0].h);
  int i;

  if (ishoriz) {
    for (i = 1; i < n; ++i) {
      szall.w += sz[i].w + gap;
      szall.h = MAX(szall.h, sz[i].h);
    }

    iRectAlign(rc, szall.w, szall.h, uFmt, prc);

    for (i = n; --i > 0;) {
      iRectCutL(prc, -sz[i].w, gap, prc + i);
      iRectAlign(prc[i], sz[i].w, sz[i].h, uFmt, prc + i);
    }

    iRectAlign(prc[0], sz[0].w, sz[0].h, uFmt, prc);
  }
  else {
    for (i = 1; i < n; ++i) {
      szall.h += sz[i].h + gap;
      szall.w = MAX(szall.w, sz[i].w);
    }

    iRectAlign(rc, szall.w, szall.h, uFmt, prc);

    for (i = n; --i > 0;) {
      iRectCutT(prc, -sz[i].h, gap, prc + i);
      iRectAlign(prc[i], sz[i].w, sz[i].h, uFmt, prc + i);
    }

    iRectAlign(prc[0], sz[0].w, sz[0].h, uFmt, prc);
  }

  return 0;
}
// s 吸附距离
CV_INLINE int iRectCut(IRECT* rc, int cmd, int d, int gap, int x, int y, int s, IRECT* out)
{
  *out = *rc;
  switch(cmd) {
  case 'l':
    {
      if (d>0) {
        d = BOUND(d, s, RCW(rc));
        out->r = rc->l + d, rc->l = out->r + gap;
      } else {
        d = BOUND(d, -RCW(rc), -d);
        rc->l = rc->r + d, out->r = rc->l - gap;
      }
      return x > (out->r - s) && x < (rc->l + s) && y > rc->t && y < rc->b;
    }
    break;
  case 'r':
    {
      if (d>0) {
        d = BOUND(d, s, RCW(rc));
        out->l = rc->r - d, rc->r = out->l - gap;
      } else {
        d = BOUND(d, -RCW(rc), -d);
        rc->r = rc->l - d, out->l = rc->r + gap;
      }
      return x > (rc->r - s) && x < (out->l + s) && y > rc->t && y < rc->b;
    }
    break;
  case 't':
    {
      if (d>0) {
        d = BOUND(d, s, RCH(rc));
        out->b = rc->t + d, rc->t = out->b + gap;
      } else {
        d = BOUND(d, -RCH(rc), -d);
        rc->t = rc->b + d, out->b = rc->t - gap;
      }
      return y > (out->b - s) && y < (rc->t + s) && x > rc->l && x < rc->r;
    }
    
    break;
  case 'b':
    {
      if (d>0) {
        d = BOUND(d, s, RCH(rc));
        out->t = rc->b - d, rc->b = out->t - gap;
      } else {
        d = BOUND(d, -RCH(rc), -d);
        rc->b = rc->t - d, out->t = rc->b + gap;
      }
      return y > (rc->b - s) && y < (out->t + s) && x > rc->l && x < rc->r;
    }
    break;
  }

  return 0;
}
// 求矩形框的邻居矩形框
CV_INLINE int iRectNeighbour(IRECT* rc, int cmd, int d, int gap)
{
  if ('r' == cmd) {
    rc->r = rc->l - gap, rc->l = rc->r - d;
  }

  if ('l' == cmd) {
    rc->l = rc->r + gap, rc->r = rc->l + d;
  }

  if ('b' == cmd) {
    rc->b = rc->t - gap, rc->t = rc->b - d;
  }

  if ('t' == cmd) {
    rc->t = rc->b + gap, rc->b = rc->t + d;
  }

  return 0;
}

CV_INLINE int x_cover(int a, int an, int b, int bn, int fmt)
{
  if (an < bn) {
    if (DT_CENTER & fmt) {
      return b + (bn - an) / 2;
    }
    else if (DT_RIGHT & fmt) {
      return b + (bn - an);
    }
    else {
      return b;
    }
  }
  else {
    a = MAX(a, b + bn - an);
    a = MIN(a, b);
  }
  
  return a;
}

// 移动rc1去覆盖rc2，使移动距离最小，覆盖面积最大
CV_INLINE IPOINT iRectCover(IRECT rc1, IRECT rc2, IRECT* outrc1, int fmt)
{
  int fmtx = fmt, fmty = (DT_VCENTER & fmt) ? DT_CENTER : (DT_BOTTOM & fmt ? DT_RIGHT : 0);
  IPOINT pt = {0, 0};
  ISIZE sz = RCSZ(&rc1);
  pt.x = x_cover(rc1.l, RCW(&rc1), rc2.l, RCW(&rc2), fmtx);
  pt.y = x_cover(rc1.t, RCH(&rc1), rc2.t, RCH(&rc2), fmty);
  *outrc1 = iRECT3(pt, sz);
  return pt;
}

CV_INLINE IRECT iRectInter3(IRECT r1, IRECT r2, IRECT r3)
{
  return iRectInter(r1, iRectInter(r2, r3));
}
CV_INLINE int iRectIntersect(IRECT* prc, IRECT r1, IRECT r2)
{
  IRECT temp = {0};
  iRectNormalize(&r1);
  iRectNormalize(&r2);
  // check for total exclusion
  if ((r1.r > r2.l) && (r1.l < r2.r) && (r1.b > r2.t) && (r1.t < r2.b)) {
    temp.l = (r1.l > r2.l) ? r1.l : r2.l;
    temp.r = (r1.r < r2.r) ? r1.r : r2.r;
    temp.t = (r1.t > r2.t) ? r1.t : r2.t;
    temp.b = (r1.b < r2.b) ? r1.b : r2.b;
  }
  if (prc) {
    *prc = temp;
  }
  return RCW(&temp) > 0 && RCH(&temp) > 0;
}
CV_INLINE IRECT iRectScale(IRECT rc, double t)
{
  IRECT rc1 = iRECT((int)(rc.l * t), (int)(rc.t * t), (int)(rc.r * t), (int)(rc.b * t));
  return rc1;
}
// a/b=c/d; a=?
CV_INLINE IRECT iRectZoom(const IRECT* b, const IRECT* c, const IRECT* d)
{
  return iRECT(b->l + (c->l - d->l) * RCW(b) / RCW(d),
      b->t + (c->t - d->t) * RCH(b) / RCH(d),
      b->r + (c->r - d->r) * RCW(b) / RCW(d),
      b->b + (c->b - d->b) * RCH(b) / RCH(d));
}
CV_INLINE IRECT iRectClip(const IRECT* pclip, int l, int t, int r, int b)
{
  IRECT rc = iRECT(l, t, r, b);
  if (pclip) {
    rc = iRectInter(rc, *pclip);
  }
  return rc;
}
CV_INLINE IRECT iRectUnion(IRECT r1, IRECT r2)
{
  IRECT temp = {0};
  iRectNormalize(&r1);
  iRectNormalize(&r2);
  temp.l = (r1.l < r2.l) ? r1.l : r2.l;
  temp.r = (r1.r > r2.r) ? r1.r : r2.r;
  temp.t = (r1.t < r2.t) ? r1.t : r2.t;
  temp.b = (r1.b > r2.b) ? r1.b : r2.b;
  return temp;
}
typedef struct tRIVERTEX {
  double x, y;
  COLOR clr;
} tRIVERTEX;
#define RectClipR(rc, rc1)  *(rc) = iRectInter(rc, *(rc1))
#define RectClip(rc, l, t, r, b)  *(rc) = iRectInter(rc, iRECT(l, t, r, b))
typedef struct inter_info2_t {
  DPOINT pt;
  //int ee[2]; // ??? (ee[0]!=ee[1]) ????????????????????? ee[0] ee[1]
  double ts[2];
  double d;
} inter_info2_t;
// ???????
typedef struct inter_info_t {
  DPOINT pt;
  //int ee[2]; // ??? (ee[0]!=ee[1]) ????????????????????? ee[0] ee[1]
  double ts[2];
  double d;
  //int next, prev;
  int next_inter0; // ?????????
  int next_inter1; // ?????????
  int flag; // 0 ????????锟斤拷???-1??1????锟斤拷????
  int samp; // ??????????
} inter_info_t;
typedef struct iinter_info_t {
  IPOINT pt;
  //int ee[2]; // ??? (ee[0]!=ee[1]) ????????????????????? ee[0] ee[1]
  //int ts[2];
  int d;
  //int next, prev;
  int next_inter0; // ?????????
  int next_inter1; // ?????????
  int flag; // 0 ????????锟斤拷???-1??1????锟斤拷????
  int samp; // ??????????
} iinter_info_t;
typedef struct iINTER {
  IPOINT pt;
  //int ee[2]; // ??? (ee[0]!=ee[1]) ????????????????????? ee[0] ee[1]
  int ts[2];
  int d;
  //int next, prev;
  int next_inter0; // ?????????
  int next_inter1; // ?????????
  int flag; // 0 ????????锟斤拷???-1??1????锟斤拷????
  int samp; // ??????????
} iINTER;
typedef struct CONTOURINFO {
  int npt;
  int flag;
  int parent;
} CONTOURINFO;
typedef struct IVC {
  IPOINT* pt;
  CONTOURINFO* cc;
  int n, maxn, l, maxl;
} IVC;
typedef struct DVC {
  DPOINT* pt;
  CONTOURINFO* cc;
  int n, maxn, l, maxl;
} DVC;
typedef struct FVC {
  FPOINT* pt;
  CONTOURINFO* cc;
  int n, maxn, l, maxl;
} FVC;
typedef struct CvChain {
  char* code;
  int len;
  IPOINT origin;
}
CvChain;
#define VC_IS_HOLE(vc)   ((vc)->cc[(vc)->l-1].flag&CV_SEQ_FLAG_HOLE)
//#define IVC_ADDPT(vc, pt)   iVC_add(vc, (pt).x, (pt).y)
//#define remove_all(vc) (vc->ptlen=0)
CV_INLINE int dVC_init(DVC* vc, DPOINT* pt, int maxn, CONTOURINFO* cc, int maxl)
{
  vc->pt = pt, vc->n = 0, vc->maxn = maxn;
  vc->cc = cc, vc->l = 0, vc->maxl = maxl;
  return 0;
}
CV_INLINE int iVC_init(IVC* vc, IPOINT* pt, int maxn, CONTOURINFO* cc, int maxl)
{
  vc->pt = pt, vc->n = 0, vc->maxn = maxn;
  vc->cc = cc, vc->l = 0, vc->maxl = maxl;
  return 0;
}
CV_INLINE int dVC_add(DVC* vc, double _x, double _y)
{
  enum {maxvalue = 100000000};
  if (vc) {
    if (vc->pt) {
      ASSERT(vc->n >= 0 && vc->n < vc->maxn);
      if (vc->n == 0 || (vc->pt[vc->n - 1].x != _x || vc->pt[vc->n - 1].y != _y)) {
        (vc->pt[vc->n].x = _x, vc->pt[vc->n].y = _y, vc->n++);
      }
    }
    if (!(_x < maxvalue && _x > -maxvalue && _y < maxvalue && _y > -maxvalue)) {
      printf("%f,%f\n", (_x), (_y));
      assert(_x < maxvalue && _x > -maxvalue && _y < maxvalue && _y > -maxvalue);
    }
  }
  return 0;
}
CV_INLINE int iVC_len(const IVC* vc)
{
  int i, len = 0;
  for (i = 0; i < vc->l; ++i) {
    len += vc->cc[i].npt;
  }
  return len;
}
CV_INLINE int iVC_end(IVC* vc)
{
  int len = iVC_len(vc);
  ASSERT(vc->l < vc->maxl);
  vc->cc[vc->l++].npt = vc->n - len;
  return 0;
}
CV_INLINE int iVC_add(IVC* vc, int _x, int _y)
{
  enum {maxvalue = 100000000};
  if (vc) {
    if (vc->pt) {
      ASSERT(vc->n >= 0 && vc->n < vc->maxn);
      if (vc->n == 0 || (vc->pt[vc->n - 1].x != _x || vc->pt[vc->n - 1].y != _y)) {
        (vc->pt[vc->n].x = _x, vc->pt[vc->n].y = _y, vc->n++);
      }
    }
    if (!(_x < maxvalue && _x > -maxvalue && _y < maxvalue && _y > -maxvalue)) {
      printf("%d,%d\n", (_x), (_y));
      assert(_x < maxvalue && _x > -maxvalue && _y < maxvalue && _y > -maxvalue);
    }
  }
  return 0;
}
#define SETRECTD(rc, l, t, r, b) ((rc)->l=l, (rc)->t=t, (rc)->r=r, (rc)->b=b)
typedef struct DSPOLYGON {
  DPOINT* pt;
  int n;
} DSPOLYGON;
// polygon
typedef struct DPOLYGON {
  DPOINT* pt;
  int* len;
  int n;
} DPOLYGON;
typedef struct ipoly_t {
  IPOINT* pt;
  int* len;
  int n;
} ipoly_t;
CV_INLINE int dPOLYGON_npt(const DPOLYGON* s)
{
  int i, m = 0;
  for (i = 0; i < s->n; ++i) {
    m += s->len[i];
  }
  return m;
}
CV_INLINE int ipoly_npt(const ipoly_t* s)
{
  int i, m = 0;
  for (i = 0; i < s->n; ++i) {
    m += s->len[i];
  }
  return m;
}
CV_INLINE int dPOLYGON_setsize(DPOLYGON* s, int n, int npt)
{
  GREALLOC(int, s->len, n);
  GREALLOC(DPOINT, s->pt, npt);
  memset(s->len, 0, sizeof(int)*n);
  s->n = n;
  return npt;
}
CV_INLINE int dPOLYGON_add(DPOLYGON* s, int isaddn, int addnpt, const DPOINT* pt)
{
  int npt = dPOLYGON_npt(s);
  isaddn = s->n ? (!!isaddn) : 1;
  if (isaddn) {
    int n = isaddn + s->n;
    GREALLOC(int, s->len, n + 1);
  }
  if (addnpt) {
    npt += addnpt;
    GREALLOC(DPOINT, s->pt, npt);
    if (pt) {
      MEMCPY(s->pt + npt - addnpt, pt, addnpt);
    }
  }
  return npt;
}
CV_INLINE int ipoly_setsize(ipoly_t* s, int n, int npt)
{
  GREALLOC(int, s->len, n);
  GREALLOC(IPOINT, s->pt, npt);
  memset(s->len, 0, sizeof(int)*n);
  s->n = n;
  return 0;
}
CV_INLINE int dPOLYGON_to_ipoly(const DPOLYGON* s, ipoly_t* c, double t)
{
  int i, npt = dPOLYGON_npt(s);
  ipoly_setsize(c, s->n, npt);
  MEMCPY(c->len, s->len, s->n);
  for (i = 0; i < npt; ++i) {
    c->pt[i].x = (int)(s->pt[i].x * t);
    c->pt[i].y = (int)(s->pt[i].y * t);
  }
  return 0;
}
CV_INLINE int ipoly_to_poly2d(const ipoly_t* s, DPOLYGON* c, double t)
{
  int i, npt = ipoly_npt(s);
  dPOLYGON_setsize(c, s->n, npt);
  MEMCPY(c->len, s->len, s->n);
  for (i = 0; i < npt; ++i) {
    c->pt[i].x = s->pt[i].x * t;
    c->pt[i].y = s->pt[i].y * t;
  }
  return 0;
}
CV_INLINE int dPOLYGON_setpts(DPOLYGON* s, int n, int npt, int* len, const DPOINT* pt)
{
  dPOLYGON_setsize(s, n, npt);
  memcpy(s->pt, pt, sizeof(DPOINT)*npt);
  memcpy(s->len, len, sizeof(int)*n);
  return 0;
}
CV_INLINE int dPOLYGON_pos(const DPOLYGON* s, int i)
{
  int j = 0, pos = 0;
  for (j = 0; j < s->n; ++j) {
    pos += s->len[j];
    if (pos >= i) {
      break;
    }
  }
  return j;
}
CV_INLINE int dPOLYGON_getspoly(DPOLYGON* s, int j, DSPOLYGON* out)
{
  int i, pos = 0;
  for (i = 0; i < s->n; ++i) {
    int npt = s->len[i];
    if (pos + npt > j) {
      out->pt = s->pt + pos;
      out->n = npt;
      return 1;
    }
    pos += npt;
  }
  return 0;
}
CV_INLINE int dPOLYGON_delpt(DPOLYGON* s, int i)
{
  int npt = dPOLYGON_npt(s);
  int pos = dPOLYGON_pos(s, i);
  memmove(s->pt + i, s->pt + i + 1, (npt - i - 1)*sizeof(DPOINT));
  s->len[pos]--;
  return 0;
}
CV_INLINE int dPOLYGON_addpt(DPOLYGON* s, int i, double x, double y)
{
  int npt = dPOLYGON_npt(s);
  int pos = dPOLYGON_pos(s, i);
  GREALLOC(DPOINT, s->pt, npt + 1);
  memmove(s->pt + i + 1, s->pt + i, (npt - i - 1)*sizeof(DPOINT));
  s->pt[i].x = x, s->pt[i].y = y;
  s->len[pos]++;
  return 0;
}
CV_INLINE int dPOLYGON_addpts(DPOLYGON* s, int len, const DPOINT* pt)
{
  int npt = dPOLYGON_npt(s);
  GREALLOC(int, s->len, s->n + 1);
  GREALLOC(DPOINT, s->pt, npt + len);
  s->len[s->n++] = len;
  if (pt) {
    memcpy(s->pt + npt, pt, sizeof(DPOINT)*len);
  }
  return npt + len;
}
CV_INLINE int ipoly_addpts(ipoly_t* s, int len, const IPOINT* pt)
{
  int npt = ipoly_npt(s);
  GREALLOC(int, s->len, s->n + 1);
  GREALLOC(IPOINT, s->pt, npt + len);
  s->len[s->n++] = len;
  memcpy(s->pt + npt, pt, sizeof(IPOINT)*len);
  return 0;
}
CV_INLINE int dPOLYGON_free(DPOLYGON* s)
{
  GFREE(s->pt);
  GFREE(s->len);
  s->n = 0;
  return 0;
}
CV_INLINE int ipoly_free(ipoly_t* s)
{
  GFREE(s->pt);
  GFREE(s->len);
  s->n = 0;
  return 0;
}
enum {
  GEO_TRANS,
  GEO_SCALING,
  GEO_SKEWING,
  GEO_ROTATION
};
// returns squared distance between two 2D points with floating-point coordinates.
CV_INLINE double icvSqDist2D32f(FPOINT pt1, FPOINT pt2)
{
  double dx = pt1.x - pt2.x;
  double dy = pt1.y - pt2.y;
  return dx * dx + dy * dy;
}
#define CV_CLOCKWISE         1   // ??????
#define CV_COUNTER_CLOCKWISE 2   // ???????
typedef DSEGMENT2 DLINE2;
typedef DSEGMENT3 DLINE3;
typedef struct DLINESEG2 {
  DPOINT2 a, b;
} DLINESEG2;
// ?????????????? a*x+b*y+c=0 ??????, ??? a>= 0
typedef struct DFLINE2 {
  double a, b, c;
} DFLINE2;
// ?????????????? a*x+b*y+c*z+d=0 ??????, ??? a>= 0
typedef struct DFLINE3 {
  double a, b, c, d;
} DFLINE3;
typedef struct DTRIANGLE2 {
  DPOINT2 p0, p1, p2;
} DTRIANGLE2;
typedef struct DTRIANGLE3 {
  DPOINT3 p0, p1, p2;
} DTRIANGLE3;
typedef struct DRECTANGLE {
  DPOINT2 p0, p1;
} DRECTANGLE;
typedef struct DQUADIX2 {
  DPOINT2 p0, p1, p2, p3;
} DQUADIX2;
typedef struct DQUADIX3 {
  DPOINT3 p0, p1, p2, p3;
} DQUADIX3;
typedef struct DCIRCLE {
  double x, y, radius;
} DCIRCLE;
typedef struct IELLIPSE {
  int x, y, ra, rb, angle;
} IELLIPSE;
typedef struct FELLIPSE {
  float x, y, ra, rb, angle;
} FELLIPSE;
typedef struct DELLIPSE {
  double x, y, ra, rb, angle;
} DELLIPSE;
typedef struct DSPHERE {
  double x, y, z, radius;
} DSPHERE;
typedef struct DHYPERSPHERE {
  DPOINT2 center;
  double radius;
} DHYPERSPHERE;
enum {SegmentPointCount = 2, LinePointCount = 2, RectanglePointCount = 2, BoxPointCount = 2, TrianglePointCount = 3, QuadixPointCount = 4, };
typedef struct DCIRCULAR_ARC {
  double x1, y1, x2, y2, cx, cy, px, py, angle1, angle2;
  int orientation;
} DCIRCULAR_ARC;
typedef struct DQUADRATIC_BEZIER2 {
  DPOINT2 p0, p1, p2;
} DQUADRATIC_BEZIER2;
typedef struct DQUADRATIC_BEZIER3 {
  DPOINT3 p0, p1, p2;
} DQUADRATIC_BEZIER3;
typedef struct DCUBIC_BEZIER2 {
  DPOINT2 p0, p1, p2, p3;
} DCUBIC_BEZIER2;
typedef struct DCUBIC_BEZIER3 {
  DPOINT3 p0, p1, p2, p3;
} DCUBIC_BEZIER3;
typedef struct DBEZIER_COEFFICIENTS2 {
  DPOINT2 p0, p1, p2, p3;
} DBEZIER_COEFFICIENTS2;
typedef struct DBEZIER_COEFFICIENTS3 {
  DPOINT3 p0, p1, p2, p3;
} DBEZIER_COEFFICIENTS3;
typedef struct DCURVE_POINT2 {
  DPOINT2 p0;
  double t;
} DCURVE_POINT2;
typedef struct DCURVE_POINT3 {
  DPOINT3 p0;
  double t;
} DCURVE_POINT3;
typedef DPOINT2 DVECTOR2;
typedef DPOINT3 DVECTOR3;
typedef struct DRAY2 {
  DPOINT2 origin;
  DVECTOR2 direction;
} DRAY2;
typedef struct DRAY3 {
  DPOINT3 origin;
  DVECTOR3 direction;
} DRAY3;
typedef struct DPLANE3 {
  double constant;
  DVECTOR3 normal;
} DPLANE3;
typedef struct DBOX2 {
  DPOINT2 p0, p1;
} DBOX2;
typedef struct DBOX3 {
  DPOINT3 p0, p1;
} DBOX3;
typedef struct DPOLYGON2 {
  DPOINT2* p;
  int n;
} DPOLYGON2;
typedef struct DPOLYGON3 {
  DPOINT3* p;
  int n;
} DPOLYGON3;
CV_INLINE int dPOLYGON2_free(DPOLYGON2* po)
{
  FREE(po->p);
  po->p = 0;
  po->n = 0;
  return 0;
}
CV_INLINE int dPOLYGON3_free(DPOLYGON3* po)
{
  FREE(po->p);
  po->p = 0;
  po->n = 0;
  return 0;
}
CV_INLINE int dPOLYGON2_setsize1(DPOLYGON2* po, int n)
{
  po->p = REALLOC(DPOINT2, po->p, n);
  po->n = n;
  return 0;
}
CV_INLINE int dPOLYGON3_setsize1(DPOLYGON3* po, int n)
{
  po->p = REALLOC(DPOINT3, po->p, n);
  po->n = n;
  return 0;
}
CV_INLINE int dPOLYGON2_insert(DPOLYGON2* po, int i, DPOINT2 pt)
{
  int n = po->n;
  dPOLYGON2_setsize1(po, po->n + 1);
  //MEMINST(po->p, po->n-1, i, &pt, 1);
  MEMMOVE(po->p + i, po->p + i + 1, n - i);
  po->p[i] = pt;
  return 0;
}
CV_INLINE int dPOLYGON2_add(DPOLYGON2* po, int i, DPOINT2 pt)
{
  dPOLYGON2_setsize1(po, po->n + 1);
  po->p[po->n - 1] = pt;
  return 0;
}
#ifndef _WIN
enum {
  SRCCOPY1
};
#endif
#define IMARGDEFTT int h=im->h, w=im->w, step=im->s, cn=im->c; uchar* img=im->tt.data
#define PIXBLEND(_d, _s, _a)    (_d += ((((_s)-(_d))*(_a))>>8))
#define PIXBLEND4(_d, _s, _a)   (PIXBLEND(_d[0], _s[0], _a), PIXBLEND(_d[1], _s[1], _a), PIXBLEND(_d[2], _s[2], _a), PIXBLEND(_d[3], _s[3], _a))
#define PIXCOPY(dst, src, cn)  { int i=0; for (; i<cn; ++i) { dst[i] = src[i]; } }
//#define CLR2GRY(C)    ((GetRV(C)+(GetGV(C)<<1)+GetBV(C))>>2)
#define CLR2GRY(C)    (((C)[0]+((C)[1]<<1)+(C)[2])>>2)
//#define showDDC(NAME, hDC) cvShowImage(NAME, hDC->h, hDC->w, (uchar*)(hDC->data), hDC->bw, 4)
#define HDDCSET_IMAGE(hDC, im)  ((hDC)->h = (im)->h, (hDC)->w = (im)->w, (hDC)->bw = (im)->s, (hDC)->data = (DWORD*)((im)->tt.data))
#define HDDCSET_RECT(hDC, hDCS, rc)  ((hDC)->h = MIN(RCH(rc), (hDCS)->h-((rc)->top)), (hDC)->w = MIN(RCW(rc), (hDCS)->w-((rc)->left)), (hDC)->bw = (hDCS)->bw, (hDC)->data = (hDCS)->data+(rc)->left+(rc)->top*((hDCS)->bw>>2))
#define HDDCSET_CLIP(hDC, prc)   (hDC->clip = ClipRect(hDC, prc))
#define HDDCGET_CLIP(hDC, prc)  if ((hDC)->clip) {*(prc)=*(hDC)->clip;} else {RCSET(prc, 0, 0, (hDC)->w, (hDC)->h);}
#define HDDC_BEGIN(hDC, prc)  {const RECT* __prc=hDC->clip; RECT __rc=*prc; hDC->clip = ClipRect(hDC, &__rc);
#define HDDC_END(hDC)    hDC->clip=__prc;}
// ???????
//#define HDDC_BEGIN2(hDC, prc)  {TDrawDC __hDC[1]; __hDC[0]=*(hDC); HDDCSET_RECT(hDC, __hDC, prc); (hDC)->clip=0;
//#define HDDC_END2(hDC)    *(hDC)=__hDC[0];}
#define INITDC(_DC, H, W, BW, _DATA)   ((_DC)->h=H, (_DC)->w=W, (_DC)->bw=BW, (_DC)->data=(DWORD*)(_DATA))
#define _Pixel(hDC, x1, y1)           ((unsigned long*)hDC->tt.data)[(int)(x1)+(int)(y1)*(hDC->s>>2)]
#define _PixelT(hDC, x1, y1, T)           (*((T*)(((char*)hDC->tt.data)+(int)(x1)*sizeof(T)+(int)(y1)*(hDC->bw))))
#define _GetPixel(hDC, x1, y1)          (((x1)>=0&&(x1)<hDC->w && (y1)>=0&&(y1)<hDC->h) ? _Pixel(hDC, (x1), (y1)):0)
#define _SetPixel(hDC, x1, y1, clr)     (((x1)>=0&&(x1)<hDC->w && (y1)>=0&&(y1)<hDC->h) ? (memcpy(hDC->tt.data + (int)(y1)*hDC->s+(int)(x1)*hDC->c, &clr, hDC->c)) : 0)
#define SWAP(a, b, t)   (t=a,a=b,b=t)
#define Fill1D(_W, _A, _V)  {int _j=0; for (_j=0;_j<_W;++_j) {*(_A+_j)=_V;}}
#define GRADIENT(_N, _I1, _V1, _V2)    (((_I1)*(_V1)+((_N)-(_I1))*(_V2))/(_N))
#define GRADIENTRGB(_N, _I1, _V1, _V2)    _RGB(GRADIENT(_N, _I1, GetRV(_V1), GetRV(_V2)), GRADIENT(_N, _I1, GetGV(_V1), GetGV(_V2)), GRADIENT(_N, _I1, GetBV(_V1), GetBV(_V2)))
#define BLEN(_A, _B, alpha)       (_A + (int)((((_B) - (_A)) * alpha) >> 8))
#define BLEN2(_A, _B, alpha, _N)  (_A + (int)((((_B) - (_A)) * alpha)/(_N) ))
#define RGBBLENSET(_A, _B)  _A = RGBBLEN(_A, _B, GetAV(_B))
#define GETRGBA(C, R, G, B, A)   (R=GetRV(C), G=GetGV(C), B=GetBV(C), A=GetAV(C))
#define SETRGBA(_P, R, G, B, A)   ((_P)[0]=B, (_P)[1]=G, (_P)[2]=R, (_P)[3]=A)
//#define SETRGB(_P, R, G, B)   ((_P)[0]=B, (_P)[1]=G, (_P)[2]=R)
#define SETGRY(_P, R, G, B)   ((_P)[0]=((B)+((G)<<1)+(R))>>2)
#define SETBLENP(_A, _B, _P, _C)  ((_A)[_P]=BLEN((_A)[_P], (_B)[_P], _C))
#define SETBLEN1(_A, _B, _C)  SETBLENP(_A, _B, 0, _C)
#define SETBLEN3(_A, _B, _C)  (SETBLENP(_A, _B, 0, _C), SETBLENP(_A, _B, 1, _C), SETBLENP(_A, _B, 2, _C))
#define SETBLEN4(_A, _B, _C)  (SETBLENP(_A, _B, 0, _C), SETBLENP(_A, _B, 1, _C), SETBLENP(_A, _B, 2, _C), SETBLENP(_A, _B, 3, _C))
#define RGBBLEN(_A, _B, _C)  _RGBA(BLEN(GetRV(_A), GetRV(_B), _C), BLEN(GetGV(_A), GetGV(_B), _C), BLEN(GetBV(_A), GetBV(_B), _C), 255)
#define RGBBLEN2(_A, _B, _C, _N)  _RGBA(BLEN2(GetRV(_A), GetRV(_B), _C, _N), BLEN2(GetGV(_A), GetGV(_B), _C, _N), BLEN2(GetBV(_A), GetBV(_B), _C, _N), 255)
#define RGBABLEN(_A, _B, _C)  _RGBA(BLEN(GetRV(_A), GetRV(_B), _C), BLEN(GetGV(_A), GetGV(_B), _C), BLEN(GetBV(_A), GetBV(_B), _C), BLEN(GetAV(_A), GetAV(_B), _C))
#define RGBABLEN2(_A, _B, _C, _N)  _RGBA(BLEN2(GetRV(_A), GetRV(_B), _C, _N), BLEN2(GetGV(_A), GetGV(_B), _C, _N), BLEN2(GetBV(_A), GetBV(_B), _C, _N), BLEN2(GetAV(_A), GetAV(_B), _C, _N))
#define MINRGB(R,G,B)  _RGB(MIN(R,255), MIN(G,255), MIN(B,255))
#define GRADIENTRGBSET(_V, _N, _I1, _V1, _V2)    {int _r=GRADIENT(_N, _I1, GetRV(_V1), GetRV(_V2)), _g=GRADIENT(_N, _I1, GetGV(_V1), GetGV(_V2)), _b=GRADIENT(_N, _I1, GetBV(_V1), GetBV(_V2)); _V=MINRGB(_r,_g,_b); if (0) {printf("%08x, %08x, %08x, %d %d %d \n", _V1, _V2, _V, _r,_g,_b); } }
#define GRADIENTRGB_FILL1D1(_W, _A, _V1, _V2)  {int _j=0; for (_j=0;_j<_W;++_j) {*(_A+_j)=GRADIENTRGB(_W, _j, _V1, _V2);}}
#define GRADIENTRGB_FILL1D(_W, _A, _V1, _V2)  {int _j=0; for (_j=0;_j<_W;++_j) {GRADIENTRGBSET(*(_A+_j), _W, _j, _V1, _V2);}}
#define GRADIENTRGB_FILL1DZB(_W, _A, _V1, _V2, _Z, _Z1, _Z2)  {int _j=0; float _z1=_Z1,dz=(_Z2-_Z1)/_W;for (_j=0;_j<_W;++_j,_z1+=dz) {if (*(_Z+_j)>_z1) {*(_Z+_j)=_z1;GRADIENTRGBSET(*(_A+_j), _W, _j, _V1, _V2);}}}
#define GRADIENTRGB_COPY1DZB(_W, _A, _V1, _V2, _Z, _Z1, _Z2)  {int _j=0; float _z1=_Z1,dz=(_Z2-_Z1)/_W;for (_j=0;_j<_W;++_j,_z1+=dz) {if (*(_Z+_j)>_z1) {*(_Z+_j)=_z1;GRADIENTRGBSET(*(_A+_j), _W, _j, _V1, _V2);}}}
#define Fill2D(_H, _W, _A, _AL, _V)  {int _i=0,_j,_ia; for (;_i<_H;++_i) {for (_j=0,_ia=_i*(_AL);_j<_W;++_j,++_ia) {*(_A+_ia)=_V;}}}
#define RCWIDTH(_PRC)   ((_PRC)->right-(_PRC)->left)
#define RCHEIGHT(_PRC)   ((_PRC)->bottom-(_PRC)->top)
#define RCX2(_PRC)   ((_PRC)->right+(_PRC)->left)
#define RCY2(_PRC)   ((_PRC)->bottom+(_PRC)->top)
#define RCOFFSET(_PRC, _L, _T, _R, _B)   ((_PRC)->l+=_L, (_PRC)->t+=_T, (_PRC)->r+=_R, (_PRC)->b+=_B)
//#define RCCENTER(_PRC)  cPOINT(((_PRC)->left+(_PRC)->right)/2, ((_PRC)->top+(_PRC)->bottom)/2)
#define DRAWHLINE(hDC, X1, X2, Y1, _V)  if (Y1>=0 && Y1<hDC->h) {int _X1=MAX(MIN(X1, X2), 0), _X2=MIN(MAX(X1, X2), hDC->w), _W=_X2-_X1; DWORD* _A=&_Pixel(hDC, _X1, Y1); Fill1D(_W, _A, _V);}
#define DRAWHLINE_GRADIENT(hDC, X1, X2, Y1, V1, V2)  if (Y1<hDC->h && Y1>=0) {int _X1=MAX(MIN(X1, X2), 0), _X2=MIN(MAX(X1, X2), hDC->w), _W=1+_X2-_X1; DWORD* _A=&_Pixel(hDC, _X1, Y1), _V1=(X1==_X1)?V1:V2, _V2=(X1==_X1)?V2:V1; GRADIENTRGB_FILL1D(_W, _A, _V1, _V2);}
#define DRAWHLINE_GRADIENTZB(hDC, hZB, X1, X2, Y1, V1, V2, _Z1,_Z2)  if (Y1<hDC->h && Y1>=0) {int _X1=MAX(MIN(X1, X2), 0), _X2=MIN(MAX(X1, X2), hDC->w), _W=1+_X2-_X1; DWORD* _A=&_Pixel(hDC, _X1, Y1), _V1=(X1==_X1)?V1:V2, _V2=(X1==_X1)?V2:V1; float* _Z=&_PixelT(hZB, _X1, Y1, float); GRADIENTRGB_FILL1DZB(_W, _A, _V2, _V1, _Z, _Z1, _Z2);}
#define ALPHAVALUE(_V1, _V2, _AL)       (((_V1)*(256-_AL)+(_V2)*(_AL))>>8)
#define ALPHACOLOR(_COL1, _COL2, _AL)  _RGB(ALPHAVALUE(GetRV(_COL1), GetRV(_COL2), _AL), ALPHAVALUE(GetGV(_COL1), GetGV(_COL2), _AL), ALPHAVALUE(GetBV(_COL1), GetBV(_COL2), _AL))
//#define SWAPRB(_COL)  _RGB(GetBV(_COL), GetGV(_COL), GetRV(_COL))
//#define SWAPRB(_COL)  (_COL)
#define GRADIENT2(_B, _I1, _V1, _V2)    (((_I1)*(_V1)+((1<<(_B))-(_I1))*(_V2))>>(_B))
#define GRADIENTRGB2(_B, _I1, _V1, _V2)    _RGB(GRADIENT2(_B, _I1, GetRV(_V1), GetRV(_V2)), GRADIENT2(_B, _I1, GetGV(_V1), GetGV(_V2)), GRADIENT2(_B, _I1, GetBV(_V1), GetBV(_V2)))
#define F2FIXN(_F, N)    ((int)((_F)*(1<<(N))))
#define FIXN2I(_F, N)    ((int)(((int64)_F)>>(N)))
#define FIXNMUL(A, B, N)    ((int)((((int64)(A))*(B))>>(N)))
#define FIXNDIV(A, B, N)    ((int)((((int64)(A))<<N)/(B)))
#define FIXNMULE(A, B, N)    (A) = FIXNMUL(A, B, N)
#define FIXNDIVE(A, B, N)    (A) = FIXNDIV(A, B, N)
#define F2FIX(_F)    ((int)((_F)*(1<<16)))
#define I2FIX(_F)    ((int)(((_F)<<16)))
#define FIX2I(_F)    ((int)((_F)>>16))
#define FIXMASK      ((1<<16)-1)
#define FIX2R(_F)    ((int)((_F)&FIXMASK))
#define F2FIX8(_F)   ((int)((_F)*(1<<8)))
#define FIX8MUL(A, B)   ((int)(((A)*(B))>>8))
#define I2FIX8(_F)   ((int)(((_F)<<8)))
#define FIX82I(_F)   ((int)((_F)>>8))
#define FIXMASK8     ((1<<8)-1)
#define FIX2R8(_F)   ((int)((_F)&FIXMASK8))
//#define ODS_OVER   0x0800
#define TDrawLine(hDC, _x1, _y1, _x2, _y2, _clr)   imdrawaa_line(hDC, 0, _x1, _y1, _x2, _y2, _clr, 1)
#define TMoveTo(hDC, _x, _y)        (_x0=(_x), _y0=(_y))
#define TLineTo(hDC, _x, _y, _clr)  (TDrawLine(hDC, (_x0), (_y0), (_x), (_y), _clr), TMoveTo(hDC, _x, _y))
#define _LineTo(hDC, _x, _y)        (TDrawLine(hDC, (_x0), (_y0), (_x), (_y), _clr), TMoveTo(hDC, _x, _y))
#define TMoveToPt(hDC, _pt)        TMoveTo(hDC, (_pt).x, (_pt).y)
#define _LineToPt(hDC, _pt)        _LineTo(hDC, (_pt).x, (_pt).y)
#ifndef ROUND
#define ROUNDL(x)  ((long)((x)+0.5))
#define ROUND(x)   ((int)((x)+0.5))
#endif
#define alpha_composite1(composite, fg, alpha, bg) { \
    unsigned short temp = ((unsigned short)(fg)*(unsigned short)(alpha) + \
        (unsigned short)(bg)*(unsigned short)(255 - (unsigned short)(alpha)) + (unsigned short)128); \
    (composite) = (uchar)((temp + (temp >> 8)) >> 8); \
  }
/*
 * The following definitions (until #endif)
 * is an extract from IPL headers.
 * Copyright (c) 1995 Intel Corporation.
 */
#define IPL_DEPTH_SIGN 0x80000000
#define IPL_DEPTH_1U     1
#define IPL_DEPTH_8U     8
#define IPL_DEPTH_16U   16
#define IPL_DEPTH_32F   32
#define IPL_DEPTH_8S  (IPL_DEPTH_SIGN| 8)
#define IPL_DEPTH_16S (IPL_DEPTH_SIGN|16)
#define IPL_DEPTH_32S (IPL_DEPTH_SIGN|32)
#define IPL_DATA_ORDER_PIXEL  0
#define IPL_DATA_ORDER_PLANE  1
#define IPL_ORIGIN_TL 0
#define IPL_ORIGIN_BL 1
#define IPL_ALIGN_4BYTES   4
#define IPL_ALIGN_8BYTES   8
#define IPL_ALIGN_16BYTES 16
#define IPL_ALIGN_32BYTES 32
#define IPL_ALIGN_DWORD   IPL_ALIGN_4BYTES
#define IPL_ALIGN_QWORD   IPL_ALIGN_8BYTES
#define IPL_BORDER_CONSTANT   0
#define IPL_BORDER_REPLICATE  1
#define IPL_BORDER_REFLECT    2
#define IPL_BORDER_WRAP       3
#define IPL_IMAGE_HEADER 1
#define IPL_IMAGE_DATA   2
#define IPL_IMAGE_ROI    4
/* extra border mode */
#define IPL_BORDER_REFLECT_101    4
#define IPL_IMAGE_MAGIC_VAL  ((int)sizeof(IplImage))
#define CV_TYPE_NAME_IMAGE "opencv-image"
#define CV_IS_IMAGE_HDR(img)   ((img) != NULL && ((const IplImage*)(img))->nSize == sizeof(IplImage))
#define CV_IS_IMAGE(img)   (CV_IS_IMAGE_HDR(img) && ((IplImage*)img)->imageData != NULL)
/* for storing double-precision
   floating point data in IplImage's */
#define IPL_DEPTH_64F  64
/* get reference to pixel at (col,row),
   for multi-channel images (col) should be multiplied by number of channels */
#define CV_IMAGE_ELEM( image, elemtype, row, col )        (((elemtype*)((image)->imageData + (image)->widthStep*(row)))[(col)])
#define COLOR_REV
//#undef RGB
//#define RGB(r,g,b)          ((COLORREF)(((BYTE)(b)|((uint16)((BYTE)(g))<<8))|(((uint32)(BYTE)(r))<<16)))
#ifdef COLOR_REV
#define _RGBA(r,g,b,a)         (((uint32)(((uint8)(b)|((uint16)((uint8)(g))<<8))|(((uint32)(uint8)(r))<<16)))|(( (uint32)(uint8)(a) )<<24))
#else
#define _RGBA(r,g,b,a)         (((uint32)(((uint8)(r)|((uint16)((uint8)(g))<<8))|(((uint32)(uint8)(g))<<16)))|(( (uint32)(uint8)(b) )<<24))
#endif
#define BOUND_RGBA(r,g,b,a)        _RGBA(BOUND(r, 0, 255), BOUND(g, 0, 255), BOUND(b, 0, 255), BOUND(a, 0, 255))
#define _RGB(r,g,b)         _RGBA(r,g,b,255)
#define INT2RGB(i)          _RGBA(((i>>16)&0xff), ((i>>8)&0xff), ((i)&0xff), 255)
//#define HEX2INT1(H)   (('0'<=(H) && (H)<='9') ? (H)-'0' : ('a'<=(H) && (H)<='f') ? (H)-'a'+10 : ('A'<=(H) && (H)<='F') ? (H)-'A'+10 : 0)
//#define HEX2INT2(H)   ((HEX2INT1((H)[0])<<4)|(HEX2INT1((H)[1])))
//#define S_RGB(S)        _RGBA(HEX2INT2((S)),HEX2INT2((S)+2),HEX2INT2((S)+4),255)
//#define CV_RGB( r, g, b )  cvScalar( (b), (g), (r), 0 )
#define CV_RGB( r, g, b )  _RGB(r,g,b)
#define _rgb(r,g,b)  _RGB(255*(r), 255*(g), 255*(b))
#define _rgba(r,g,b,a)  _RGBA(255*(r), 255*(g), 255*(b), 255*(a))
#define _RGB_A(clr, _a)   _RGBA(GetRV(clr),GetGV(clr),GetBV(clr),_a)
#define _A_RGB(_a, clr)    ((clr&0x00ffffff)|(_a<<24))
#define _ARGB(a,r,g,b)  _RGBA(r,g,b,a)
#define _SetAlpha _RGB_A
#define CVRGBDEF( r, g, b )  _RGB(r, g, b)
#define GetBV(rgb) ((BYTE)(rgb))
#define GetGV(rgb) ((BYTE)(((uint16)(rgb)) >> 8))
#define GetRV(rgb) ((BYTE)((rgb)>>16))
#define GetAV(rgb) ((BYTE)((rgb)>>24))
#define GetRGBV(rgb) ((rgb)&_RGBA(255,255,255,0))
#define SWAPRB(_COL)  _RGB(GetBV(_COL), GetGV(_COL), GetRV(_COL))
#define CVSCALARDATA(scalar, _I) (((unsigned char*)scalar)[_I])
#define RGBDEF _RGB
#define SCALARDATA(scalar, _I) (((unsigned char*)(scalar))[_I])
#define RGBMUL(rgb, _x)  CV_RGB(GetRV(rgb)*(_x), GetGV(rgb)*(_x), GetBV(rgb)*(_x))
#define SCALARTORAWDATA(_scalar, _data, _cn) {int _cn1 = _cn; while(_cn1--) { (_data)[_cn1] = CVSCALARDATA(_scalar, _cn1); } }
//                      Data Persistence                                    *
/********************************** High-level functions ********************************/
/* get index of the thread being executed */
//CVAPI( int ) cvGetThreadNum( void );
//! various border interpolation methods
enum { BORDER_REPLICATE = IPL_BORDER_REPLICATE, BORDER_CONSTANT = IPL_BORDER_CONSTANT,
    BORDER_REFLECT = IPL_BORDER_REFLECT, BORDER_WRAP = IPL_BORDER_WRAP,
    BORDER_REFLECT_101 = IPL_BORDER_REFLECT_101, BORDER_REFLECT101 = BORDER_REFLECT_101,
    //BORDER_TRANSPARENT=IPL_BORDER_TRANSPARENT,
    BORDER_DEFAULT = BORDER_REFLECT_101, BORDER_ISOLATED = 16
     };
//! type of the kernel
enum { KERNEL_GENERAL = 0, KERNEL_SYMMETRICAL = 1, KERNEL_ASYMMETRICAL = 2,
    KERNEL_SMOOTH = 4, KERNEL_INTEGER = 8
     };
//! shape of the structuring element
enum { MORPH_RECT = 0, MORPH_CROSS = 1, MORPH_ELLIPSE = 2 };
enum {
  GHT_POSITION = 0,
  GHT_SCALE = 1,
  GHT_ROTATION = 2
};
//! type of morphological operation
enum { MORPH_ERODE = CV_MOP_ERODE, MORPH_DILATE = CV_MOP_DILATE,
    MORPH_OPEN = CV_MOP_OPEN, MORPH_CLOSE = CV_MOP_CLOSE,
    MORPH_GRADIENT = CV_MOP_GRADIENT, MORPH_TOPHAT = CV_MOP_TOPHAT,
    MORPH_BLACKHAT = CV_MOP_BLACKHAT
     };
/************************************* CvScalar *****************************************/
typedef struct CvScalar {
  double val[ 4 ];
}
CvScalar;
CV_INLINE CvScalar cvScalar(double val0, double val1 CV_DEFAULT(0),
    double val2 CV_DEFAULT(0), double val3 CV_DEFAULT(0))
{
  CvScalar scalar;
  scalar.val[ 0 ] = val0;
  scalar.val[ 1 ] = val1;
  scalar.val[ 2 ] = val2;
  scalar.val[ 3 ] = val3;
  return scalar;
}
CV_INLINE CvScalar cvRealScalar(double val0)
{
  CvScalar scalar;
  scalar.val[ 0 ] = val0;
  scalar.val[ 1 ] = scalar.val[ 2 ] = scalar.val[ 3 ] = 0;
  return scalar;
}
CV_INLINE CvScalar cvScalarAll(double val0123)
{
  CvScalar scalar;
  scalar.val[ 0 ] = val0123;
  scalar.val[ 1 ] = val0123;
  scalar.val[ 2 ] = val0123;
  scalar.val[ 3 ] = val0123;
  return scalar;
}
/************** interpolation formulas and tables ***************/
enum {
  INTER_NEAREST = CV_INTER_NN, //!< nearest neighbor interpolation
  INTER_LINEAR = CV_INTER_LINEAR, //!< bilinear interpolation
  INTER_CUBIC = CV_INTER_CUBIC, //!< bicubic interpolation
  INTER_AREA = CV_INTER_AREA, //!< area-based (or super) interpolation
  INTER_LANCZOS4 = CV_INTER_LANCZOS4, //!< Lanczos interpolation over 8x8 neighborhood
  INTER_MAX = 7,
  WARP_INVERSE_MAP = CV_WARP_INVERSE_MAP,
  INTER_BITS = 5, INTER_BITS2 = INTER_BITS * 2,
  INTER_TAB_SIZE = (1 << INTER_BITS),
  INTER_TAB_SIZE2 = INTER_TAB_SIZE * INTER_TAB_SIZE,
  INTER_RESIZE_COEF_BITS = 11,
  INTER_RESIZE_COEF_SCALE = 1 << INTER_RESIZE_COEF_BITS,
  INTER_REMAP_COEF_BITS = 15,
  INTER_REMAP_COEF_SCALE = 1 << INTER_REMAP_COEF_BITS
};
//! type of the threshold operation
enum { THRESH_BINARY = CV_THRESH_BINARY, THRESH_BINARY_INV = CV_THRESH_BINARY_INV,
    THRESH_TRUNC = CV_THRESH_TRUNC, THRESH_TOZERO = CV_THRESH_TOZERO,
    THRESH_TOZERO_INV = CV_THRESH_TOZERO_INV, THRESH_MASK = CV_THRESH_MASK,
    THRESH_OTSU = CV_THRESH_OTSU
     };
//! adaptive threshold algorithm
enum { ADAPTIVE_THRESH_MEAN_C = 0, ADAPTIVE_THRESH_GAUSSIAN_C = 1 };
enum {
  PROJ_SPHERICAL_ORTHO = 0,
  PROJ_SPHERICAL_EQRECT = 1
};
//! class of the pixel in GrabCut algorithm
enum {
  GC_BGD    = 0,  //!< background
  GC_FGD    = 1,  //!< foreground
  GC_PR_BGD = 2,  //!< most probably background
  GC_PR_FGD = 3   //!< most probably foreground
};
//! GrabCut algorithm flags
enum {
  GC_INIT_WITH_RECT  = 0,
  GC_INIT_WITH_MASK  = 1,
  GC_EVAL            = 2
};
enum {
  DIST_LABEL_CCOMP = 0,
  DIST_LABEL_PIXEL = 1
};
enum { FLOODFILL_FIXED_RANGE = 1 << 16, FLOODFILL_MASK_ONLY = 1 << 17 };
//! type of the template matching operation
enum { TM_SQDIFF = 0, TM_SQDIFF_NORMED = 1, TM_CCORR = 2, TM_CCORR_NORMED = 3, TM_CCOEFF = 4, TM_CCOEFF_NORMED = 5 };
//! mode of the contour retrieval algorithm
enum {
  RETR_EXTERNAL = CV_RETR_EXTERNAL, //!< retrieve only the most external (top-level) contours
  RETR_LIST = CV_RETR_LIST, //!< retrieve all the contours without any hierarchical information
  RETR_CCOMP = CV_RETR_CCOMP, //!< retrieve the connected components (that can possibly be nested)
  RETR_TREE = CV_RETR_TREE, //!< retrieve all the contours and the whole hierarchy
  RETR_FLOODFILL = CV_RETR_FLOODFILL
};
//! the contour approximation algorithm
enum {
  CHAIN_APPROX_NONE = CV_CHAIN_APPROX_NONE,
  CHAIN_APPROX_SIMPLE = CV_CHAIN_APPROX_SIMPLE,
  CHAIN_APPROX_TC89_L1 = CV_CHAIN_APPROX_TC89_L1,
  CHAIN_APPROX_TC89_KCOS = CV_CHAIN_APPROX_TC89_KCOS
};
typedef struct CvConnectedComp {
  double area;    /* area of the connected component  */
  CvScalar value;  /* average color of the connected component */
  IRECT rect;     /* ROI of the component  */
  //CvSeq* contour; /* optional component boundary
  //                  (the contour might have child contours corresponding to the holes)*/
}
CvConnectedComp;
/* spatial and central moments */
typedef struct CvMoments {
  double m00, m10, m01, m20, m11, m02, m30, m21, m12, m03; /* spatial moments */
  double mu20, mu11, mu02, mu30, mu21, mu12, mu03; /* central moments */
  double inv_sqrt_m00; /* m00 != 0 ? 1/sqrt(m00) : 0 */
}
CvMoments;
/* Hu invariants */
typedef struct CvHuMoments {
  double hu1, hu2, hu3, hu4, hu5, hu6, hu7; /* Hu invariants */
}
CvHuMoments;
//?????????
#define INTER_NEARSAMPLE(in, x1, y1, wl, bi, B) \
  do { \
    int pos = ((y1>>SHIFT) * wl + (x1>>SHIFT) * bi), bii; \
    for (bii=0; bii<bi; ++pos, ++bii) \
      (B)[bii] = in[pos]; \
  } while(0)
#define INTER_NEARSAMPLERGB(in, x1, y1, wl, bi, B) \
  do { \
    int pos = ((y1>>SHIFT) * wl + (x1>>SHIFT) * bi), bii; \
    (B)[bii+0] = in[pos+0], (B)[bii+1] = in[pos+1], (B)[bii+2] = in[pos+2]; \
  } while(0)
// ?????????? Bilinear Bicubic
#define INTER_BILINEARITY(in, x0, y0, wl, bi, B) \
  do { \
    int x1=(x0)& SHIFT_MARK, y1=(y0)& SHIFT_MARK, bii; \
    int pos = (((y0)>>SHIFT) * wl + ((x0)>>SHIFT) * bi); \
    int pm3 = SHIFT_MUL(x1, y1); \
    int pm2 = y1-pm3, pm1 = x1-pm3, pm0 = SHIFT1-y1-x1+pm3; \
    for (bii=0; bii<bi; ++pos, ++bii) \
      *(B+bii) = (int)(pm0 * in[pos] + pm1 * in[pos + bi] \
          + pm2 * in[pos + wl] + pm3 * in[pos + wl + bi] + (SHIFT1-1))>>SHIFT;\
  } while(0)
#define INTER_BILINEARITY_GRAY(in, x0, y0, wl, bi, out) \
  do { \
    int R, G, B; \
    int x1=(x0)& SHIFT_MARK, y1=(y0)& SHIFT_MARK; \
    int pos = (((y0)>>SHIFT) * (wl) + ((x0)>>SHIFT) * (bi)); \
    int pm3 = SHIFT_MUL(x1, y1); \
    int pm2 = y1-pm3, pm1 = x1-pm3, pm0 = SHIFT1-y1-x1+pm3; \
    B = (pm0 * in[pos] + pm1 * in[pos + bi] + pm2 * in[pos + wl] \
        + pm3 * in[pos + wl + bi] + (SHIFT1-1))>>SHIFT;++pos; \
    G = (pm0 * in[pos] + pm1 * in[pos + bi] + pm2 * in[pos + wl] \
        + pm3 * in[pos + wl + bi] + (SHIFT1-1))>>SHIFT;++pos; \
    R = (pm0 * in[pos] + pm1 * in[pos + bi] + pm2 * in[pos + wl] \
        + pm3 * in[pos + wl + bi] + (SHIFT1-1))>>SHIFT;++pos; \
    *(out) = (int)GREYSCALIZE(R, G, B); \
  } while(0)
// ????????????
//        / (a+2)x^3 - (a+2)x^2 + 1    0<=x<1
// h(x) = | ax^3 - 5ax^2 + 8ax - 4a    1<=x<2
// \ 0                          2<=x
// a = {-1, 0.5, 0.75 };
// ?????????????
//        / [(-9b-6c+2)x^3 - (12b+6c-18)x + (-2b+6)]/6               0<=x<1
// h(x) = | [(-b-6c)x^3 - (6b+30c)x^2 + (-12b-48c)x - (8b+24c)]/6    1<=x<2
// \ 0                                                        2<=x
// (b, c) = {(0.33, 0.33), (1.5, -0.25)};
// B???????
//        / (x^3) / 2 - x^2 + 2/3   0<=x<1
// h(x) = | ((2-x)^3) / 6           1<=x<2
// \ 0                       2<=x
// (b, c) = {(0.33, 0.33), (1.5, -0.25)};
//?锟斤拷?????????????sin(x*PI)/(x*PI)??? //PI=3.1415926535897932385;
//???????????????????
#if 0
double SinXDivX1(double x)
{
  double x2, x3;
  const double a = -1; //a??????? a=-2,-1,-0.75,-0.5?????????????????????????
  x2 = x * x;
  x3 = x2 * x;
  if (x <= 1) {
    return (a + 2) * x3 - (a + 3) * x2 + 1;
  }
  else if (x <= 2) {
    return a * x3 - (5 * a) * x2 + (8 * a) * x - (4 * a);
  }
  else {
    return 0;
  }
}
double SinXDivX(double x)
{
#define a -1
  return (x <= 1) ? (((a + 2) * x - (a + 3)) * x * x + 1) : \
      (x <= 2) ? (((a * x - (5 * a)) * x + (8 * a)) * x - (4 * a)) : 0;
#undef a
}
// ???锟斤拷???????????
int threeorder(const unsigned char* in, int ix, int iy, int wl, int bi, unsigned char* out)
{
  int i, j, bii, u;
  double afu[ 4 ], afv[ 4 ];
  double sA = 0, aA = 0;
  double x = SHIFT_TOFLOAT(ix);
  double y = SHIFT_TOFLOAT(iy);
  const int x0 = (int) x;
  const int y0 = (int) y;
  bii = bi;
  x -= x0;
  y -= y0;
  afu[ 0 ] = SinXDivX(1 + x);
  afu[ 1 ] = SinXDivX(x);
  afu[ 2 ] = SinXDivX(1 - x);
  afu[ 3 ] = SinXDivX(2 - x);
  afv[ 0 ] = SinXDivX(1 + y);
  afv[ 1 ] = SinXDivX(y);
  afv[ 2 ] = SinXDivX(1 - y);
  afv[ 3 ] = SinXDivX(2 - y);
  for (; bii--; ++in, ++out) {
    for (sA = 0, i = 0; i < 4; ++i) {
      for (aA = 0, j = 0; j < 4; ++j) {
        u = (x0 - 1 + j) * bi + (y0 - 1 + i) * wl;
        aA += afu[ j ] * in[ u < 0 ? 0 : u ];
      }
      sA += aA * afv[ i ];
    }
    *out = sA;
  }
  return 0;
}
#endif
//printf("%d, %d, %f, %f, %f, %f\n", pos, *out, SHIFT_TOFLOAT(pm0), SHIFT_TOFLOAT(pm1), SHIFT_TOFLOAT(pm2), SHIFT_TOFLOAT(pm3));
// ????????锟斤拷??????????
// INTER - ?????
// IMRESIZE(h, w, A, al, ai, h*sc, w*sc, A0, w*sc, 1, INTER_BILINEARITY);
#define IMRESIZE(ah, aw, A, al, ai, bh, bw, B, bl, bi, _INTER) \
  do { \
    if ((ah)==(bh) && (aw)==(bw) && (ai)==(bi)) { \
      SKIP_COPY(ah, aw*ai, A, al, 1, B, bl, 1); \
    } else { \
      int _I, _J, _bh=(int)(bh); \
      int x = (int)SHIFT_DIV(aw-1, bw); \
      int y = (int)SHIFT_DIV(ah-1, bh); \
      int bwi = (int)(bw * bi); \
      int bll = (int)((int)(bl) - bwi); \
      int k = 0; \
      int Be; \
      for (_I=0; _bh--; _I+=y, k+=bll) { \
        for (_J=0, Be=k+bwi; k!=Be; _J+=x, k+=bi) { \
          _INTER((A), _J, _I, al, ai, (B+k)); \
        } \
      } \
    } \
  } while(0)
#define IMRESIZE_BASE(ah, aw, A, al, ai, bh, bw, B, bl, bi, _INTER, _IX, _IY) \
  do { \
    int _I, _J, Be, k = 0, _bh=(int)(bh); \
    int x = (int)SHIFT_DIV(aw, bw); \
    int y = (int)SHIFT_DIV(ah, bh); \
    int bwi = (int)(bw * bi); \
    int IXi = (int)(_IX * bi); \
    int bll = (int)((int)(bl) - bwi); \
    for (_I=0; _bh>(_IY); --_bh, _I+=y, k+=bll) { \
      for (_J=0, Be=k+bwi-IXi; k!=Be; _J+=x, k+=bi) { \
        _INTER((A), _J, _I, al, ai, (B+k)); \
      } \
      for (Be=k+IXi; k!=Be; _J+=x, k+=bi) { \
        INTER_NEARSAMPLE((A), _J, _I, al, ai, (B+k)); \
      } \
    } \
    for (; _bh--; _I+=y, k+=bll) { \
      for (_J=0, Be=k+bwi; k!=Be; _J+=x, k+=bi) { \
        INTER_NEARSAMPLE((A), _J, _I, al, ai, (B+k)); \
      } \
    } \
  } while(0)
#define IMRESIZE_AREA(SRCH, SRCW, SRC, SRCSTEP, DSTH, DSTW, DST, DSTSTEP, CN) \
  do { \
    int _I, _YS, _YD, _XS_, _XD_; \
    int x = (SRCW)/(DSTW); \
    int y = (SRCH)/(DSTH); \
    int _AREA = x*y; \
    if (_AREA>1) { \
      int _X2 = ((SRCW)<<14)/(DSTW); \
      int _Y2 = ((SRCH)<<14)/(DSTH); \
      int DSTWCN = (DSTW)*(CN); \
      int* OFS = (int*)malloc(_AREA*sizeof(int)); \
      for (_YD = 0; _YD < y; ++_YD) { \
        for (_XD_ = 0; _XD_ < x; ++_XD_) { \
          OFS[_YD*x+_XD_] = _YD*(SRCSTEP)+_XD_*(CN); \
        } \
      } \
      for (_YS=0,_YD=0;_YD<(DSTH)*(DSTSTEP);_YD+=(DSTSTEP),_YS+=_Y2) { \
        for (_XS_=0,_XD_=_YD;_XD_<(_YD+(DSTWCN));_XD_+=(CN),_XS_+=_X2){ \
          for (_I=0; _I<(CN); ++_I) { \
            int _K=0, _SUM=0, _XS2I=(_YS>>14)*(SRCSTEP)+(_XS_>>14)*(CN)+_I; \
            for(; _K<_AREA-3; _K+=4) { \
              int OFT = OFS[_K], OFT1 = OFS[_K+1]; \
              int OFT2 = OFS[_K+2], OFT3 = OFS[_K+3]; \
              _SUM += *(SRC+_XS2I+OFT)+*(SRC+_XS2I+OFT1) \
                  +*(SRC+_XS2I+OFT2)+*(SRC+_XS2I+OFT3); \
            } \
            for(; _K<_AREA; ++_K) { \
              int OFT = OFS[_K]; \
              _SUM += *(SRC+_XS2I+OFT); \
            } \
            *(DST+_XD_+_I) = (unsigned char)(_SUM/_AREA); \
          } \
        } \
      } \
      free(OFS); \
    } else { \
      IMRESIZE(SRCH, SRCW, SRC, SRCSTEP, CN, DSTH, DSTW, DST, DSTSTEP, CN, INTER_BILINEARITY);\
    } \
  } while(0)
#define IMRESIZE_BILIN(SRCH, SRCW, SRC, SRCSTEP, DSTH, DSTW, DST, DSTSTEP, CN) \
  IMRESIZE(SRCH, SRCW, SRC, SRCSTEP, CN, DSTH, DSTW, DST, DSTSTEP, CN, INTER_BILINEARITY)
// ???锟斤拷???锟斤拷
// co[] ????锟斤拷???
#define __IMAFFINE(ah, aw, A, al, ai, bh, bw, B, bl, bi, _MUL3X2, co, _INTER, _BOUND) \
  do { \
    int _bh=(int)(bh), _bw=(int)(bw); \
    int _I, _J, x=0, y=0; \
    int bwi = (int)(bw * bi); \
    int bll = (int)((int)(bl) - bwi); \
    int k = 0; \
    int iah = SHIFT_TOINT(ah-1), iaw = SHIFT_TOINT(aw-1); \
    for (_I=0; _I!=_bh; ++_I, k+=bll) { \
      for (_J=0; _J!=_bw; ++_J, k+=bi) { \
        _MUL3X2(co, _J, _I, x, y); \
        _BOUND(x, 0, iaw); \
        _BOUND(y, 0, iah); \
        if (x>=0 && x<iaw && y>=0 && y<iah) { \
          _INTER(A, x, y, al, ai, (B+k)); \
        } else if (0) { _INTER(A, 0, 0, al, ai, (B+k)); } \
      } \
    } \
  } while(0)
#define IMAFFINE(ah, aw, A, al, ai, bh, bw, B, bl, bi, m, INTER) \
  __IMAFFINE(ah, aw, A, al, ai, bh, bw, B, bl, bi, MATRIX3X2_MUL_POINT, m, INTER, BOUND_CLIP)
#define IMAFFINE2(ah, aw, A, al, ai, bh, bw, B, bl, bi, m, INTER) \
  __IMAFFINE(ah, aw, A, al, ai, bh, bw, B, bl, bi, MATRIX3X2_MUL_POINT, m, INTER, BOUND_NUL)
#define IMROTATE(_AH, _AW, _A, _AL, _AI, _BH, _BW, k, _BL, _BI, _ANG, _CX, _CY, _BOUND) do {\
    MATRIX3X2 _M; MATRIX3X2_INIT(_M); MATRIX3X2_MOVE(_M, _CX, _CY); MATRIX3X2_ROTATE(_M, _ANG); \
    MATRIX3X2_MOVE(_M, -(_CX), -(_CY)); __IMAFFINE( _AH, _AW, _A, _AL, _AI, _BH, _BW, k, _BL, _BI, \
        MATRIX3X2_MUL_POINT, _M, INTER_BILINEARITY, _BOUND ); } while(0)
struct XFILE;
typedef struct XFILE* ihand_t;
//#define ihand_t XFILE*
//#define IMGHEADFEILD  int width, height, channels
typedef struct Color {
  uchar b, g, r, a;
}
Color;
typedef Color PaletteEntry;
typedef Color ColorPalette;
CV_INLINE Color Color_rgb(int R, int G, int B)
{
  PaletteEntry c;
  c.a = 255, c.r = R, c.g = G, c.b = B;
  return c;
}
CV_INLINE Color Color_argb(int A, int R, int G, int B)
{
  PaletteEntry c;
  c.a = A, c.r = R, c.g = G, c.b = B;
  return c;
}
#define TYPEDEF_DEF(TYPEDEF) \
TYPEDEF(CV_1U,u1,1,uchar) \
TYPEDEF(CV_1S,s1,1,char) \
TYPEDEF(CV_2U,u2,2,ushort) \
TYPEDEF(CV_2S,s2,2,short) \
TYPEDEF(CV_4U,u4,4,uint) \
TYPEDEF(CV_4S,s4,4,int) \
TYPEDEF(CV_8U,u8,8,uint64) \
TYPEDEF(CV_8S,s8,8,int64) \
TYPEDEF(CV_4F,f4,4,float) \
TYPEDEF(CV_8F,f8,8,double)
typedef struct arr_t {
  //IMGHEADFEILD;
  int n, c;
#if 1
  union {
    uchar* data;
#define TYPEDEF(a,b,c,d)  d* b;
TYPEDEF_DEF(TYPEDEF)
#undef TYPEDEF
  } tt;
#else
  uchar* data;
#endif
}
arr_t;
CV_INLINE int arr_setsize(arr_t* arr, int n, int c)
{
  if (NULL != arr) {
    if (arr->n == n && arr->c == c && arr->tt.data != NULL) {
    }
    else {
      int sz = n * c;
      uchar* newp = arr->tt.data;
      GREALLOC(uchar, newp, sz);
      memset(newp, 0, sz);
      arr->n = n, arr->c = c, arr->tt.data = newp;
    }
  }
  return 0;
}
#define IMINIT(IM, H, W, A, S, C, F)  ((IM)->h=H,(IM)->w=W,(IM)->tt.data=(uchar*)(A),(IM)->s=S,(IM)->c=(short)(C),(IM)->f=F)
typedef struct img_t {
  //IMGHEADFEILD;
  int w, h, s, f;
  short c, t;
#if 1
  union {
    uchar* data;
#define TYPEDEF(a,b,c,d)  d* b;
TYPEDEF_DEF(TYPEDEF)
#undef TYPEDEF
  } tt;
#else
  uchar* data;
#endif
}
img_t;
static ISIZE imsize(const img_t* im)
{
  ISIZE sz;
  sz.h = im->h, sz.w = im->w;
  return sz;
}
#define IMARG(im)  (im)->h, (im)->w, (im)->tt.data, (im)->s, (im)->c
#define imcopysize( im, im2 ) imsetsize(im, (im2)->h, (im2)->w, (im2)->c, (im2)->f)
#define imref(_IM, _X, _Y, _W, _H, _IM2)  imsubref(_IM, iRECT(_X, _Y, (_X)+(_W), (_Y)+(_H)), _IM2)
#define imrefrc(_IM, RC, _IM2)  imsubref(_IM, iRECT((RC).x, (RC).y, (RC).x+(RC).w, (RC).y+(RC).h), _IM2)
CV_INLINE int imsubref(const img_t* im, IRECT rc, img_t* im2)
{
  *im2 = *im;
  rc = iRectInter(rc, iRECT(0, 0, im->w, im->h));
  im2->h = RCH(&rc);
  im2->w = RCW(&rc);
  im2->tt.data = im->tt.data + rc.t * im->s + rc.l * im->c;
  return 0;
}
CV_INLINE CvSize cvGetSize(const img_t* mat)
{
  CvSize size;
  size = cvSize(mat->w, mat->h);
  return size;
}
CV_INLINE int imswap(img_t* a, img_t* b)
{
  img_t t;
  CV_SWAP(*a, *b, t);
  return 0;
}
CV_INLINE int bf_imfree(buf_t* bf, img_t* im)
{
  int n = sizeof(uchar) * im->h * im->s * im->f;
  if (im) {
    if (im->tt.data) {
      BFFREE(bf, im->tt.data, n);
    }
    memset(im, 0, sizeof(img_t));
  }
  return 0;
}
CV_INLINE int bf_imsetsize(buf_t* bf, img_t* im, int height, int width, int channels, int frames)
{
  if (NULL != im) {
    if (im->h == height && im->w == width && im->c == channels && im->f == frames && im->tt.data != NULL) {
    }
    else {
      int n = sizeof(uchar) * height * width * channels * frames;
      uchar* newp;
      BFMALLOC(bf, newp, n);
      memset(newp, 0, n);
      IMINIT(im, height, width, newp, width * channels, channels, frames);
    }
  }
  return 0;
}
#define imzero(im)  memset((im)->tt.data, 0, (im)->h*(im)->s)
#define imsetsamesize(im, im2)  imsetsize(im, (im2)->h, (im2)->w, (im2)->c, (im2)->f)
#define imsetsize_i4(im, h, w, f)  imsetsize(im, h, w, sizeof(int), f)
#define imsetsize_f8(im, h, w, f)  imsetsize(im, h, w, sizeof(double), f)
#define bf_imsetsamesize(bf, im, im2)  bf_imsetsize(bf, im, (im2)->h, (im2)->w, (im2)->c, (im2)->f)
#if 1
CV_INLINE int imsetsize(img_t* im, int height, int width, int channels, int frames)
{
  if (NULL != im) {
    if (im->h == height && im->w == width && im->c == channels && im->f == frames && im->tt.data != NULL) {
    }
    else {
      //int step = (width*channels+3) & ~3;
      int step = width * channels;
      int n = sizeof(uchar) * height * step * frames;
      if (n > 0) {
        uchar* newp = im->tt.data;
        GREALLOC(uchar, newp, n);
        ASSERT(newp != NULL && "");
        memset(newp, 0, n);
        IMINIT(im, height, width, newp, step, channels, frames);
      }
    }
  }
  return 0;
}
#else
#define imsetsize1( im, height, width, channels, frames0 ) { int frames=frames0;\
    if ( NULL != im ) { \
      if (im->h==height && im->w==width && im->c==channels && im->f==frames && im->tt.data!=NULL) { \
      } else { \
        int n = sizeof( uchar ) * height * width * channels * frames; \
        uchar* newp = ( uchar* ) grealloc( im->tt.data, n ); \
        memset( newp, 0, n ); \
        IMINIT(im, height, width, newp, width*channels, channels, frames); \
      } \
    } \
  }
#endif
CV_INLINE int imgetframe(img_t* im, int iframe, img_t* im2)
{
  int n = im->s * im->h;
  uchar* data = im->tt.data + n * iframe;
  IMINIT(im2, im->h, im->w, data, im->s, im->c, 1);
  return 0;
}
CV_INLINE int imsetframe(img_t* im, int iframe, img_t* im2)
{
  int n;
  if (0 == im->f || im->h != im2->h || im->w != im2->w || im->c != im2->c) {
    imsetsize(im, im2->h, im2->w, im2->c, iframe + 1);
  }
  ASSERT(im->h == im2->h && im->w == im2->w && im->c == im2->c);
  n = im->s * im->h;
  if (im->f <= iframe) {
    im->f = iframe + 1;
    im->tt.data = (uchar*)grealloc(im->tt.data, n * im->f);
  }
  memcpy(im->tt.data + n * iframe, im2->tt.data, n);
  return 0;
}
CV_INLINE int imcopy(const img_t* a, img_t* b)
{
  if (a != b) {
    memcpy2d(b->tt.data, b->s, a->tt.data, a->s, a->h, b->w * b->c);
  }
  return 0;
}
CV_INLINE int imflip_ud_copy(const img_t* a, img_t* b)
{
  if (a != b) {
    memflip(b->tt.data, b->s, a->tt.data, a->s, a->h, b->w * b->c);
  }
  return 0;
}
CV_INLINE int imclone(const img_t* a, img_t* b)
{
  if (a != b) {
    memcpy2d(b->tt.data, b->s, a->tt.data, a->s, a->h, b->w * b->c);
  }
  return 0;
}
CV_INLINE int imclone2(const img_t* a, img_t* b)
{
  if (a != b) {
    imsetsamesize(b, a);
    memcpy2d(b->tt.data, b->s, a->tt.data, a->s, a->h * a->f, b->w * b->c);
  }
  return 0;
}
CV_INLINE int imclone3(const img_t* a, int f, img_t* b)
{
  if (a != b) {
    if (a->f > 0) {
      f %= a->f;
      imsetsize(b, (a)->h, (a)->w, (a)->c, 1);
      memcpy2d(b->tt.data, b->s, a->tt.data + f * a->h * a->s, a->s, a->h, b->w * b->c);
    }
  }
  return 0;
}
CV_INLINE int imsubcopy(const img_t* im, IRECT rc, img_t* im2)
{
  img_t im1[1] = {0};
  imsubref(im, rc, im1);
  imclone2(im1, im2);
  return 0;
}
CV_INLINE int bf_imclone2(buf_t* bf, const img_t* a, img_t* b)
{
  if (a != b) {
    bf_imsetsamesize(bf, b, a);
    memcpy2d(b->tt.data, b->s, a->tt.data, a->s, a->h, b->w * b->c);
  }
  return 0;
}
CV_INLINE int imfree(img_t* im)
{
  if (im) {
    if (im->tt.data) {
      GFREE(im->tt.data);
    }
    memset(im, 0, sizeof(img_t));
  }
  return 0;
}
#define imfrees(im, n) freeims(im, n)
#define imfrees2(im) freeims(im, countof(im))
CV_INLINE int freeims(img_t* im, int n)
{
  int i;
  for (i = 0; i < n; ++i) {
    if (im[i].tt.data) {
      gfree(im[i].tt.data);
    }
  }
  memset(im, 0, n * sizeof(img_t));
  return 0;
}
CV_INLINE int bf_imfrees(buf_t* bf, img_t* im, int n)
{
  int i;
  for (i = n - 1; i >= 0; --i) {
    bf_imfree(bf, im + i);
  }
  memset(im, 0, n * sizeof(img_t));
  return 0;
}
// ????16锟斤拷png???
//int fmtpng_write16( FILE* m_fp, int height, int width, const uchar* data, int step, int channels );
CV_INLINE int imwritedat(const char* picname, int height, int width, const uchar* data, int step, int channels)
{
  FILE* pf = fopen(picname, "wb");
  if (pf != NULL) {
    int i;
    fwrite(&height, sizeof(int), 1, pf);
    fwrite(&width, sizeof(int), 1, pf);
    fwrite(&channels, sizeof(int), 1, pf);
    for (i = 0; i < height; ++i, data += step) {
      fwrite(data, width * channels, 1, pf);
    }
    return 1;
  }
  return 0;
}
CV_INLINE int imreaddat2(const char* picname, img_t* im)
{
  FILE* pf = fopen(picname, "rb");
  if (pf != NULL) {
    int height, width, channels;
    fread(&height, sizeof(int), 1, pf);
    fread(&width, sizeof(int), 1, pf);
    fread(&channels, sizeof(int), 1, pf);
    imsetsize(im, height, width, channels, 1);
    fread(im->tt.data, height * width * channels, 1, pf);
  }
  return 0;
}
#define cvShowImage(n, h, w, a, al, ai) cvShowImagePal(n, h, w, a, al, ai,0)
#define imwritedat2(_FN, im)  imwritedat( _FN, im->h, im->w, im->tt.data, im->s, im->c )
#define imwriterect( _FN, img, al, ai, x, y, cx, cy ) imwrite(_FN, cy, cx, img+y*al+x*ai, al, ai)
#define imwriterect2( _FN, img, al, ai, _RC ) imwriterect(_FN, img, al, ai, (_RC).x, (_RC).y, (_RC).cx, (_RC).cy)
#define imwrite2txt(_FN, _IM) FPRINT2D(_FN, _T("%d,\r\n"), _T(""), (_IM)->h, (_IM)->w, (_IM)->tt.data, (_IM)->s, (_IM)->c)
#define img2txt( in, out ) { img_t * im = imread2( in, 0 ); imwrite2txt( out, im ); imfree( im ); }
#define imshowpal(_NAME, _IM, pal) cvShowImagePal(_NAME, (_IM)->h, (_IM)->w, (_IM)->tt.data, (_IM)->s, (_IM)->c, pal)
#define imshow(_IM) cvShowImagePal(#_IM, (_IM)->h, (_IM)->w, (_IM)->tt.data, (_IM)->s, (_IM)->c, 0)
#define imshow2(_NAME, _IM) cvShowImage(_NAME, (_IM)->h, (_IM)->w, (_IM)->tt.data, (_IM)->s, (_IM)->c)
#define imwrite3(_IM) imwrite(#_IM".bmp", (_IM)->h, (_IM)->w, (_IM)->tt.data, (_IM)->s, (_IM)->c)
//                    Matrix type (CvMat)                                   *
enum {
#define TYPEDEF(a,b,c,d)  a,
TYPEDEF_DEF(TYPEDEF)
#undef TYPEDEF
#define TDD(a,b,c)  a##C##c=((a<<8)|c)
#define TYPEDEF(a,b,c,d)  TDD(a,b,1),TDD(a,b,2),TDD(a,b,3),TDD(a,b,4),
TYPEDEF_DEF(TYPEDEF)
#undef TYPEDEF
};
#define TYPECVTDEF_DEF(TYPECVTDEF) \
TYPECVTDEF(CV_1U,u1,1,uchar,CV_1U,u1,1,uchar) \
TYPECVTDEF(CV_1U,u1,1,uchar,CV_1S,s1,1,char) \
TYPECVTDEF(CV_1U,u1,1,uchar,CV_2U,u2,2,ushort) \
TYPECVTDEF(CV_1U,u1,1,uchar,CV_2S,s2,2,short) \
TYPECVTDEF(CV_1U,u1,1,uchar,CV_4U,u4,4,uint) \
TYPECVTDEF(CV_1U,u1,1,uchar,CV_4S,s4,4,int) \
TYPECVTDEF(CV_1U,u1,1,uchar,CV_8U,u8,8,uint64) \
TYPECVTDEF(CV_1U,u1,1,uchar,CV_8S,s8,8,int64) \
TYPECVTDEF(CV_1U,u1,1,uchar,CV_4F,f4,4,float) \
TYPECVTDEF(CV_1U,u1,1,uchar,CV_8F,f8,8,double) \
TYPECVTDEF(CV_1S,s1,1,char,CV_1U,u1,1,uchar) \
TYPECVTDEF(CV_1S,s1,1,char,CV_1S,s1,1,char) \
TYPECVTDEF(CV_1S,s1,1,char,CV_2U,u2,2,ushort) \
TYPECVTDEF(CV_1S,s1,1,char,CV_2S,s2,2,short) \
TYPECVTDEF(CV_1S,s1,1,char,CV_4U,u4,4,uint) \
TYPECVTDEF(CV_1S,s1,1,char,CV_4S,s4,4,int) \
TYPECVTDEF(CV_1S,s1,1,char,CV_8U,u8,8,uint64) \
TYPECVTDEF(CV_1S,s1,1,char,CV_8S,s8,8,int64) \
TYPECVTDEF(CV_1S,s1,1,char,CV_4F,f4,4,float) \
TYPECVTDEF(CV_1S,s1,1,char,CV_8F,f8,8,double) \
TYPECVTDEF(CV_2U,u2,2,ushort,CV_1U,u1,1,uchar) \
TYPECVTDEF(CV_2U,u2,2,ushort,CV_1S,s1,1,char) \
TYPECVTDEF(CV_2U,u2,2,ushort,CV_2U,u2,2,ushort) \
TYPECVTDEF(CV_2U,u2,2,ushort,CV_2S,s2,2,short) \
TYPECVTDEF(CV_2U,u2,2,ushort,CV_4U,u4,4,uint) \
TYPECVTDEF(CV_2U,u2,2,ushort,CV_4S,s4,4,int) \
TYPECVTDEF(CV_2U,u2,2,ushort,CV_8U,u8,8,uint64) \
TYPECVTDEF(CV_2U,u2,2,ushort,CV_8S,s8,8,int64) \
TYPECVTDEF(CV_2U,u2,2,ushort,CV_4F,f4,4,float) \
TYPECVTDEF(CV_2U,u2,2,ushort,CV_8F,f8,8,double) \
TYPECVTDEF(CV_2S,s2,2,short,CV_1U,u1,1,uchar) \
TYPECVTDEF(CV_2S,s2,2,short,CV_1S,s1,1,char) \
TYPECVTDEF(CV_2S,s2,2,short,CV_2U,u2,2,ushort) \
TYPECVTDEF(CV_2S,s2,2,short,CV_2S,s2,2,short) \
TYPECVTDEF(CV_2S,s2,2,short,CV_4U,u4,4,uint) \
TYPECVTDEF(CV_2S,s2,2,short,CV_4S,s4,4,int) \
TYPECVTDEF(CV_2S,s2,2,short,CV_8U,u8,8,uint64) \
TYPECVTDEF(CV_2S,s2,2,short,CV_8S,s8,8,int64) \
TYPECVTDEF(CV_2S,s2,2,short,CV_4F,f4,4,float) \
TYPECVTDEF(CV_2S,s2,2,short,CV_8F,f8,8,double) \
TYPECVTDEF(CV_4U,u4,4,uint,CV_1U,u1,1,uchar) \
TYPECVTDEF(CV_4U,u4,4,uint,CV_1S,s1,1,char) \
TYPECVTDEF(CV_4U,u4,4,uint,CV_2U,u2,2,ushort) \
TYPECVTDEF(CV_4U,u4,4,uint,CV_2S,s2,2,short) \
TYPECVTDEF(CV_4U,u4,4,uint,CV_4U,u4,4,uint) \
TYPECVTDEF(CV_4U,u4,4,uint,CV_4S,s4,4,int) \
TYPECVTDEF(CV_4U,u4,4,uint,CV_8U,u8,8,uint64) \
TYPECVTDEF(CV_4U,u4,4,uint,CV_8S,s8,8,int64) \
TYPECVTDEF(CV_4U,u4,4,uint,CV_4F,f4,4,float) \
TYPECVTDEF(CV_4U,u4,4,uint,CV_8F,f8,8,double) \
TYPECVTDEF(CV_4S,s4,4,int,CV_1U,u1,1,uchar) \
TYPECVTDEF(CV_4S,s4,4,int,CV_1S,s1,1,char) \
TYPECVTDEF(CV_4S,s4,4,int,CV_2U,u2,2,ushort) \
TYPECVTDEF(CV_4S,s4,4,int,CV_2S,s2,2,short) \
TYPECVTDEF(CV_4S,s4,4,int,CV_4U,u4,4,uint) \
TYPECVTDEF(CV_4S,s4,4,int,CV_4S,s4,4,int) \
TYPECVTDEF(CV_4S,s4,4,int,CV_8U,u8,8,uint64) \
TYPECVTDEF(CV_4S,s4,4,int,CV_8S,s8,8,int64) \
TYPECVTDEF(CV_4S,s4,4,int,CV_4F,f4,4,float) \
TYPECVTDEF(CV_4S,s4,4,int,CV_8F,f8,8,double) \
TYPECVTDEF(CV_8U,u8,8,uint64,CV_1U,u1,1,uchar) \
TYPECVTDEF(CV_8U,u8,8,uint64,CV_1S,s1,1,char) \
TYPECVTDEF(CV_8U,u8,8,uint64,CV_2U,u2,2,ushort) \
TYPECVTDEF(CV_8U,u8,8,uint64,CV_2S,s2,2,short) \
TYPECVTDEF(CV_8U,u8,8,uint64,CV_4U,u4,4,uint) \
TYPECVTDEF(CV_8U,u8,8,uint64,CV_4S,s4,4,int) \
TYPECVTDEF(CV_8U,u8,8,uint64,CV_8U,u8,8,uint64) \
TYPECVTDEF(CV_8U,u8,8,uint64,CV_8S,s8,8,int64) \
TYPECVTDEF(CV_8U,u8,8,uint64,CV_4F,f4,4,float) \
TYPECVTDEF(CV_8U,u8,8,uint64,CV_8F,f8,8,double) \
TYPECVTDEF(CV_8S,s8,8,int64,CV_1U,u1,1,uchar) \
TYPECVTDEF(CV_8S,s8,8,int64,CV_1S,s1,1,char) \
TYPECVTDEF(CV_8S,s8,8,int64,CV_2U,u2,2,ushort) \
TYPECVTDEF(CV_8S,s8,8,int64,CV_2S,s2,2,short) \
TYPECVTDEF(CV_8S,s8,8,int64,CV_4U,u4,4,uint) \
TYPECVTDEF(CV_8S,s8,8,int64,CV_4S,s4,4,int) \
TYPECVTDEF(CV_8S,s8,8,int64,CV_8U,u8,8,uint64) \
TYPECVTDEF(CV_8S,s8,8,int64,CV_8S,s8,8,int64) \
TYPECVTDEF(CV_8S,s8,8,int64,CV_4F,f4,4,float) \
TYPECVTDEF(CV_8S,s8,8,int64,CV_8F,f8,8,double) \
TYPECVTDEF(CV_4F,f4,4,float,CV_1U,u1,1,uchar) \
TYPECVTDEF(CV_4F,f4,4,float,CV_1S,s1,1,char) \
TYPECVTDEF(CV_4F,f4,4,float,CV_2U,u2,2,ushort) \
TYPECVTDEF(CV_4F,f4,4,float,CV_2S,s2,2,short) \
TYPECVTDEF(CV_4F,f4,4,float,CV_4U,u4,4,uint) \
TYPECVTDEF(CV_4F,f4,4,float,CV_4S,s4,4,int) \
TYPECVTDEF(CV_4F,f4,4,float,CV_8U,u8,8,uint64) \
TYPECVTDEF(CV_4F,f4,4,float,CV_8S,s8,8,int64) \
TYPECVTDEF(CV_4F,f4,4,float,CV_4F,f4,4,float) \
TYPECVTDEF(CV_4F,f4,4,float,CV_8F,f8,8,double) \
TYPECVTDEF(CV_8F,f8,8,double,CV_1U,u1,1,uchar) \
TYPECVTDEF(CV_8F,f8,8,double,CV_1S,s1,1,char) \
TYPECVTDEF(CV_8F,f8,8,double,CV_2U,u2,2,ushort) \
TYPECVTDEF(CV_8F,f8,8,double,CV_2S,s2,2,short) \
TYPECVTDEF(CV_8F,f8,8,double,CV_4U,u4,4,uint) \
TYPECVTDEF(CV_8F,f8,8,double,CV_4S,s4,4,int) \
TYPECVTDEF(CV_8F,f8,8,double,CV_8U,u8,8,uint64) \
TYPECVTDEF(CV_8F,f8,8,double,CV_8S,s8,8,int64) \
TYPECVTDEF(CV_8F,f8,8,double,CV_4F,f4,4,float) \
TYPECVTDEF(CV_8F,f8,8,double,CV_8F,f8,8,double)

static void* castcpy(void* dst, const void* src, int n, int dst_type, int src_type) {
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

#define CV_CN_MAX     64
#define CV_CN_SHIFT   3
#define CV_DEPTH_MAX  (1 << CV_CN_SHIFT)
#define CV_MAT_DEPTH_MASK       (CV_DEPTH_MAX - 1)
#define CV_MAT_DEPTH(flags)     ((flags) & CV_MAT_DEPTH_MASK)
#define CV_MAKETYPE(depth,cn) (CV_MAT_DEPTH(depth) + (((cn)-1) << CV_CN_SHIFT))
#define CV_MAKE_TYPE CV_MAKETYPE
#define CV_AUTO_STEP  0x7fffffff
#define CV_WHOLE_ARR  cvSlice( 0, 0x3fffffff )
#define CV_MAT_CN_MASK          ((CV_CN_MAX - 1) << CV_CN_SHIFT)
#define CV_MAT_CN(flags)        ((((flags) & CV_MAT_CN_MASK) >> CV_CN_SHIFT) + 1)
#define CV_MAT_TYPE_MASK        (CV_DEPTH_MAX*CV_CN_MAX - 1)
#define CV_MAT_TYPE(flags)      ((flags) & CV_MAT_TYPE_MASK)
#define CV_MAT_CONT_FLAG_SHIFT  14
#define CV_MAT_CONT_FLAG        (1 << CV_MAT_CONT_FLAG_SHIFT)
#define CV_IS_MAT_CONT(flags)   ((flags) & CV_MAT_CONT_FLAG)
#define CV_IS_CONT_MAT          CV_IS_MAT_CONT
#define CV_MAT_TEMP_FLAG_SHIFT  15
#define CV_MAT_TEMP_FLAG        (1 << CV_MAT_TEMP_FLAG_SHIFT)
#define CV_IS_TEMP_MAT(flags)   ((flags) & CV_MAT_TEMP_FLAG)
#define CV_IS_MASK_ARR(mat)   (((mat)->type & (CV_MAT_TYPE_MASK & ~CV_8SC1)) == 0)
#define CV_ARE_TYPES_EQ(mat1, mat2)   ((((mat1)->type ^ (mat2)->type) & CV_MAT_TYPE_MASK) == 0)
#define CV_ARE_CNS_EQ(mat1, mat2)   ((((mat1)->type ^ (mat2)->type) & CV_MAT_CN_MASK) == 0)
#define CV_ARE_DEPTHS_EQ(mat1, mat2)   ((((mat1)->type ^ (mat2)->type) & CV_MAT_DEPTH_MASK) == 0)
#define CV_ARE_SIZES_EQ(mat1, mat2)   ((mat1)->rows == (mat2)->rows && (mat1)->cols == (mat2)->cols)
#define CV_IS_MAT_CONST(mat)   (((mat)->rows|(mat)->cols) == 1)
/* Size of each channel item,
   0x124489 = 1000 0100 0100 0010 0010 0001 0001 ~ array of sizeof(arr_type_elem) */
#define CV_ELEM_SIZE1(type)   ((((sizeof(size_t)<<28)|0x8442211) >> CV_MAT_DEPTH(type)*4) & 15)
/* 0x3a50 = 11 10 10 01 01 00 00 ~ array of log2(sizeof(arr_type_elem)) */
#define CV_ELEM_SIZE(type)   (CV_MAT_CN(type) << ((((sizeof(size_t)/4+1)*16384|0x3a50) >> CV_MAT_DEPTH(type)*2) & 3))
#define IPL2CV_DEPTH(depth) \
  ((((CV_8U)+(CV_16U<<4)+(CV_32F<<8)+(CV_64F<<16)+(CV_8S<<20)+ \
      (CV_16S<<24)+(CV_32S<<28)) >> ((((depth) & 0xF0) >> 2) + \
          (((depth) & IPL_DEPTH_SIGN) ? 20 : 0))) & 15)
#define CV_MAT_ELEM_PTR( mat, row, col )   CV_MAT_ELEM_PTR_FAST( mat, row, col, CV_ELEM_SIZE((mat).type) )
#define CV_MAT_ELEM( mat, elemtype, row, col )   (*(elemtype*)CV_MAT_ELEM_PTR_FAST( mat, row, col, sizeof(elemtype)))
//         Multi-dimensional dense array (CvMatND)                          *
#define CV_MATND_MAGIC_VAL    0x42430000
#define CV_TYPE_NAME_MATND    "opencv-nd-matrix"
#define CV_MAX_DIM            32
#define CV_MAX_DIM_HEAP       (1 << 16)
#define CV_IS_MATND_HDR(mat) \
  ((mat) != NULL && (((const CvMatND*)(mat))->type & CV_MAGIC_MASK) == CV_MATND_MAGIC_VAL)
#define CV_IS_MATND(mat) \
  (CV_IS_MATND_HDR(mat) && ((const CvMatND*)(mat))->tt.data.ptr != NULL)
//        Multi-dimensional sparse array (CvSparseMat)                      *
#define CV_SPARSE_MAT_MAGIC_VAL    0x42440000
#define CV_TYPE_NAME_SPARSE_MAT    "opencv-sparse-matrix"
#define CV_IS_SPARSE_MAT_HDR(mat) \
  ((mat) != NULL && \
      (((const CvSparseMat*)(mat))->type & CV_MAGIC_MASK) == CV_SPARSE_MAT_MAGIC_VAL)
#define CV_IS_SPARSE_MAT(mat) \
  CV_IS_SPARSE_MAT_HDR(mat)
/**************** iteration through a sparse array *****************/
#define CV_NODE_VAL(mat,node)   ((void*)((uchar*)(node) + (mat)->valoffset))
#define CV_NODE_IDX(mat,node)   ((int*)((uchar*)(node) + (mat)->idxoffset))
//                           Histogram                                      *
typedef int CvHistType;
#define CV_HIST_MAGIC_VAL     0x42450000
#define CV_HIST_UNIFORM_FLAG  (1 << 10)
/* indicates whether bin ranges are set already or not */
#define CV_HIST_RANGES_FLAG   (1 << 11)
#define CV_HIST_ARRAY         0
#define CV_HIST_SPARSE        1
#define CV_HIST_TREE          CV_HIST_SPARSE
/* should be used as a parameter only,
   it turns to CV_HIST_UNIFORM_FLAG of hist->type */
#define CV_HIST_UNIFORM       1
#define CV_IS_HIST( hist ) \
  ((hist) != NULL  && \
      (((CvHistogram*)(hist))->type & CV_MAGIC_MASK) == CV_HIST_MAGIC_VAL && \
      (hist)->bins != NULL)
#define CV_IS_UNIFORM_HIST( hist ) \
  (((hist)->type & CV_HIST_UNIFORM_FLAG) != 0)
#define CV_IS_SPARSE_HIST( hist ) \
  CV_IS_SPARSE_MAT((hist)->bins)
#define CV_HIST_HAS_RANGES( hist ) \
  (((hist)->type & CV_HIST_RANGES_FLAG) != 0)
//        Other supplementary data type definitions                         *
/*********************************** CvTermCriteria *************************************/
#define CV_TERMCRIT_ITER    1
#define CV_TERMCRIT_NUMBER  CV_TERMCRIT_ITER
#define CV_TERMCRIT_EPS     2
typedef struct CvTermCriteria {
  int type;  /* may be combination of
                       CV_TERMCRIT_ITER
                       CV_TERMCRIT_EPS */
  int max_iter;
  double epsilon;
}
CvTermCriteria;
CV_INLINE CvTermCriteria cvTermCriteria(int type, int max_iter, double epsilon)
{
  CvTermCriteria t;
  t.type = type;
  t.max_iter = max_iter;
  t.epsilon = (float) epsilon;
  return t;
}
typedef struct bgra_t {
  BYTE b, g, r, a;
} rgba_t;
typedef struct CvSlice {
  int start_index, end_index;
}
CvSlice;
CV_INLINE CvSlice cvSlice(int start, int end)
{
  CvSlice slice;
  slice.start_index = start;
  slice.end_index = end;
  return slice;
}
#define CV_WHOLE_SEQ_END_INDEX 0x3fffffff
#define CV_WHOLE_SEQ  cvSlice(0, CV_WHOLE_SEQ_END_INDEX)
//                     Dynamic Data structures                              *
/*********************************** Chain/Countour *************************************/
//                      Sequence types                                      *
#define CV_SEQ_MAGIC_VAL             0x42990000
#define CV_IS_SEQ(seq) \
  ((seq) != NULL && (((CvSeq*)(seq))->flags & CV_MAGIC_MASK) == CV_SEQ_MAGIC_VAL)
#define CV_SET_MAGIC_VAL             0x42980000
#define CV_IS_SET(set) \
  ((set) != NULL && (((CvSeq*)(set))->flags & CV_MAGIC_MASK) == CV_SET_MAGIC_VAL)
#define CV_SEQ_ELTYPE_BITS           9
#define CV_SEQ_ELTYPE_MASK           ((1 << CV_SEQ_ELTYPE_BITS) - 1)
//#define CV_SEQ_ELTYPE_POINT          CV_32SC2  /* (x,y) */
#define CV_SEQ_ELTYPE_POINT          123  /* (x,y) */
#define CV_SEQ_ELTYPE_CODE           CV_8UC1   /* freeman code: 0..7 */
#define CV_SEQ_ELTYPE_GENERIC        0
#define CV_SEQ_ELTYPE_PTR            CV_USRTYPE1
#define CV_SEQ_ELTYPE_PPOINT         CV_SEQ_ELTYPE_PTR  /* &(x,y) */
#define CV_SEQ_ELTYPE_INDEX          CV_32SC1  /* #(x,y) */
#define CV_SEQ_ELTYPE_GRAPH_EDGE     0  /* &next_o, &next_d, &vtx_o, &vtx_d */
#define CV_SEQ_ELTYPE_GRAPH_VERTEX   0  /* first_edge, &(x,y) */
#define CV_SEQ_ELTYPE_TRIAN_ATR      0  /* vertex of the binary tree   */
#define CV_SEQ_ELTYPE_CONNECTED_COMP 0  /* connected component  */
#define CV_SEQ_ELTYPE_POINT3D        CV_32FC3  /* (x,y,z)  */
#define CV_SEQ_KIND_BITS        3
#define CV_SEQ_KIND_MASK        (((1 << CV_SEQ_KIND_BITS) - 1)<<CV_SEQ_ELTYPE_BITS)
/* types of sequences */
#define CV_SEQ_KIND_GENERIC     (0 << CV_SEQ_ELTYPE_BITS)
#define CV_SEQ_KIND_CURVE       (1 << CV_SEQ_ELTYPE_BITS)
#define CV_SEQ_KIND_BIN_TREE    (2 << CV_SEQ_ELTYPE_BITS)
/* types of sparse sequences (sets) */
#define CV_SEQ_KIND_GRAPH       (3 << CV_SEQ_ELTYPE_BITS)
#define CV_SEQ_KIND_SUBDIV2D    (4 << CV_SEQ_ELTYPE_BITS)
#define CV_SEQ_FLAG_SHIFT       (CV_SEQ_KIND_BITS + CV_SEQ_ELTYPE_BITS)
/* flags for curves */
#define CV_SEQ_FLAG_CLOSED     (1 << CV_SEQ_FLAG_SHIFT)
#define CV_SEQ_FLAG_SIMPLE     (2 << CV_SEQ_FLAG_SHIFT)
#define CV_SEQ_FLAG_CONVEX     (4 << CV_SEQ_FLAG_SHIFT)
#define CV_SEQ_FLAG_HOLE       (8 << CV_SEQ_FLAG_SHIFT)
/* flags for graphs */
#define CV_GRAPH_FLAG_ORIENTED (1 << CV_SEQ_FLAG_SHIFT)
#define CV_GRAPH               CV_SEQ_KIND_GRAPH
#define CV_ORIENTED_GRAPH      (CV_SEQ_KIND_GRAPH|CV_GRAPH_FLAG_ORIENTED)
/* point sets */
#define CV_SEQ_POINT_SET       (CV_SEQ_KIND_GENERIC| CV_SEQ_ELTYPE_POINT)
#define CV_SEQ_POINT3D_SET     (CV_SEQ_KIND_GENERIC| CV_SEQ_ELTYPE_POINT3D)
#define CV_SEQ_POLYLINE        (CV_SEQ_KIND_CURVE  | CV_SEQ_ELTYPE_POINT)
#define CV_SEQ_POLYGON         (CV_SEQ_FLAG_CLOSED | CV_SEQ_POLYLINE )
#define CV_SEQ_CONTOUR         CV_SEQ_POLYGON
#define CV_SEQ_SIMPLE_POLYGON  (CV_SEQ_FLAG_SIMPLE | CV_SEQ_POLYGON  )
/* chain-coded curves */
#define CV_SEQ_CHAIN           (CV_SEQ_KIND_CURVE  | CV_SEQ_ELTYPE_CODE)
#define CV_SEQ_CHAIN_CONTOUR   (CV_SEQ_FLAG_CLOSED | CV_SEQ_CHAIN)
/* binary tree for the contour */
#define CV_SEQ_POLYGON_TREE    (CV_SEQ_KIND_BIN_TREE  | CV_SEQ_ELTYPE_TRIAN_ATR)
/* sequence of the connected components */
#define CV_SEQ_CONNECTED_COMP  (CV_SEQ_KIND_GENERIC  | CV_SEQ_ELTYPE_CONNECTED_COMP)
/* sequence of the integer numbers */
#define CV_SEQ_INDEX           (CV_SEQ_KIND_GENERIC  | CV_SEQ_ELTYPE_INDEX)
#define CV_SEQ_ELTYPE( seq )   ((seq)->flags & CV_SEQ_ELTYPE_MASK)
#define CV_SEQ_KIND( seq )     ((seq)->flags & CV_SEQ_KIND_MASK )
/* flag checking */
#define CV_IS_SEQ_INDEX( seq )      ((CV_SEQ_ELTYPE(seq) == CV_SEQ_ELTYPE_INDEX) && (CV_SEQ_KIND(seq) == CV_SEQ_KIND_GENERIC))
#define CV_IS_SEQ_CURVE( seq )      (CV_SEQ_KIND(seq) == CV_SEQ_KIND_CURVE)
#define CV_IS_SEQ_CLOSED( seq )     (((seq)->flags & CV_SEQ_FLAG_CLOSED) != 0)
#define CV_IS_SEQ_CONVEX( seq )     (((seq)->flags & CV_SEQ_FLAG_CONVEX) != 0)
#define CV_IS_SEQ_HOLE( seq )       (((seq)->flags & CV_SEQ_FLAG_HOLE) != 0)
#define CV_IS_SEQ_SIMPLE( seq )     ((((seq)->flags & CV_SEQ_FLAG_SIMPLE) != 0) || CV_IS_SEQ_CONVEX(seq))
/* type checking macros */
#define CV_IS_SEQ_POINT_SET( seq )   ((CV_SEQ_ELTYPE(seq) == CV_32SC2 || CV_SEQ_ELTYPE(seq) == CV_32FC2))
#define CV_IS_SEQ_POINT_SUBSET( seq )   (CV_IS_SEQ_INDEX( seq ) || CV_SEQ_ELTYPE(seq) == CV_SEQ_ELTYPE_PPOINT)
#define CV_IS_SEQ_POLYLINE( seq )   (CV_SEQ_KIND(seq) == CV_SEQ_KIND_CURVE && CV_IS_SEQ_POINT_SET(seq))
#define CV_IS_SEQ_POLYGON( seq )   (CV_IS_SEQ_POLYLINE(seq) && CV_IS_SEQ_CLOSED(seq))
#define CV_IS_SEQ_CHAIN( seq )   (CV_SEQ_KIND(seq) == CV_SEQ_KIND_CURVE && (seq)->elem_size == 1)
#define CV_IS_SEQ_CONTOUR( seq )   (CV_IS_SEQ_CLOSED(seq) && (CV_IS_SEQ_POLYLINE(seq) || CV_IS_SEQ_CHAIN(seq)))
#define CV_IS_SEQ_CHAIN_CONTOUR( seq )   (CV_IS_SEQ_CHAIN( seq ) && CV_IS_SEQ_CLOSED( seq ))
#define CV_IS_SEQ_POLYGON_TREE( seq )   (CV_SEQ_ELTYPE (seq) ==  CV_SEQ_ELTYPE_TRIAN_ATR && CV_SEQ_KIND( seq ) ==  CV_SEQ_KIND_BIN_TREE )
#define CV_IS_GRAPH( seq )   (CV_IS_SET(seq) && CV_SEQ_KIND((CvSet*)(seq)) == CV_SEQ_KIND_GRAPH)
#define CV_IS_GRAPH_ORIENTED( seq )   (((seq)->flags & CV_GRAPH_FLAG_ORIENTED) != 0)
#define CV_IS_SUBDIV2D( seq )   (CV_IS_SET(seq) && CV_SEQ_KIND((CvSet*)(seq)) == CV_SEQ_KIND_SUBDIV2D)
/****************************************************************************************/
///                  Operations on sequences                               */
/****************************************************************************************/
#define  CV_SEQ_ELEM( seq, elem_type, index ) \
  /* assert gives some guarantee that <seq> parameter is valid */ \
  (   assert(sizeof((seq)->first[0]) == sizeof(CvSeqBlock) && \
      (seq)->elem_size == sizeof(elem_type)), \
      (elem_type*)((seq)->first && (unsigned)index < \
          (unsigned)((seq)->first->count) ? \
          (seq)->first->tt.data + (index) * sizeof(elem_type) : \
          cvGetSeqElem( (CvSeq*)(seq), (index) )))
#define CV_GET_SEQ_ELEM( elem_type, seq, index ) CV_SEQ_ELEM( (seq), elem_type, (index) )
/* Add element to sequence: */
#define CV_WRITE_SEQ_ELEM_VAR( elem_ptr, writer ) \
  { \
    if( (writer).ptr >= (writer).block_max ) \
    { \
      cvCreateSeqBlock( &writer); \
    } \
    memcpy((writer).ptr, elem_ptr, (writer).seq->elem_size);\
    (writer).ptr += (writer).seq->elem_size; \
  }
#define CV_WRITE_SEQ_ELEM1( elem, writer ) \
  { \
    assert( (writer).seq->elem_size == sizeof(elem)); \
    if( (writer).ptr >= (writer).block_max ) \
    { \
      cvCreateSeqBlock( &writer); \
    } \
    assert( (writer).ptr <= (writer).block_max - sizeof(elem));\
    memcpy((writer).ptr, &(elem), sizeof(elem)); \
    (writer).ptr += sizeof(elem); \
  }
/* Move reader position forward: */
#define CV_NEXT_SEQ_ELEM( elem_size, reader ) \
  { \
    if( ((reader).ptr += (elem_size)) >= (reader).block_max ) \
    { \
      cvChangeSeqBlock( &(reader), 1 ); \
    } \
  }
/* Move reader position backward: */
#define CV_PREV_SEQ_ELEM( elem_size, reader ) \
  { \
    if( ((reader).ptr -= (elem_size)) < (reader).block_min ) \
    { \
      cvChangeSeqBlock( &(reader), -1 ); \
    } \
  }
/* Read element and move read position forward: */
#define CV_READ_SEQ_ELEM( elem, reader ) \
  { \
    assert( (reader).seq->elem_size == sizeof(elem)); \
    memcpy( &(elem), (reader).ptr, sizeof((elem))); \
    CV_NEXT_SEQ_ELEM( sizeof(elem), reader ) \
  }
/* Read element and move read position backward: */
#define CV_REV_READ_SEQ_ELEM( elem, reader ) \
  { \
    assert( (reader).seq->elem_size == sizeof(elem)); \
    memcpy(&(elem), (reader).ptr, sizeof((elem))); \
    CV_PREV_SEQ_ELEM( sizeof(elem), reader ) \
  }
#define CV_READ_CHAIN_POINT( _pt, reader ) \
  { \
    (_pt) = (reader).pt; \
    if( (reader).ptr ) \
    { \
      CV_READ_SEQ_ELEM( (reader).code, (reader)); \
      assert( ((reader).code & ~7) == 0 ); \
      (reader).pt.x += (reader).deltas[(int)(reader).code][0]; \
      (reader).pt.y += (reader).deltas[(int)(reader).code][1]; \
    } \
  }
#define CV_CURRENT_POINT( reader )  (*((CvPoint*)((reader).ptr)))
#define CV_PREV_POINT( reader )     (*((CvPoint*)((reader).prev_elem)))
#define CV_READ_EDGE( pt1, pt2, reader ) \
  { \
    assert( sizeof(pt1) == sizeof(CvPoint) && \
        sizeof(pt2) == sizeof(CvPoint) && \
        reader.seq->elem_size == sizeof(CvPoint)); \
    (pt1) = CV_PREV_POINT( reader ); \
    (pt2) = CV_CURRENT_POINT( reader ); \
    (reader).prev_elem = (reader).ptr; \
    CV_NEXT_SEQ_ELEM( sizeof(CvPoint), (reader)); \
  }
/************ Graph macros ************/
/* Return next graph edge for given vertex: */
#define  CV_NEXT_GRAPH_EDGE( edge, vertex ) \
  (assert((edge)->vtx[0] == (vertex) || (edge)->vtx[1] == (vertex)), \
      (edge)->next[(edge)->vtx[1] == (vertex)])
//                    Basic GUI functions                                   *
enum {
  /* 8bit, color or not */
  CV_LOAD_IMAGE_UNCHANGED  = -1,
  /* 8bit, gray */
  CV_LOAD_IMAGE_GRAYSCALE  = 0,
  /* ?, color */
  CV_LOAD_IMAGE_COLOR      = 1,
  /* any depth, ? */
  CV_LOAD_IMAGE_ANYDEPTH   = 2,
  /* ?, any color */
  CV_LOAD_IMAGE_ANYCOLOR   = 4
};
enum {
  CV_IMWRITE_JPEG_QUALITY = 1,
  CV_IMWRITE_PNG_COMPRESSION = 16,
  CV_IMWRITE_PXM_BINARY = 32
};
enum {
  CV_CVTIMG_FLIP      = 1,
  CV_CVTIMG_SWAP_RB   = 2
};
enum {
  CV_CAP_ANY      = 0,    // autodetect
  CV_CAP_MIL      = 100,  // MIL proprietary drivers
  CV_CAP_VFW      = 200,  // platform native
  CV_CAP_V4L      = 200,
  CV_CAP_V4L2     = 200,
  CV_CAP_FIREWARE = 300,  // IEEE 1394 drivers
  CV_CAP_FIREWIRE = 300,
  CV_CAP_IEEE1394 = 300,
  CV_CAP_DC1394   = 300,
  CV_CAP_CMU1394  = 300,
  CV_CAP_STEREO   = 400,  // TYZX proprietary drivers
  CV_CAP_TYZX     = 400,
  CV_TYZX_LEFT    = 400,
  CV_TYZX_RIGHT   = 401,
  CV_TYZX_COLOR   = 402,
  CV_TYZX_Z       = 403,
  CV_CAP_QT       = 500,  // QuickTime
  CV_CAP_UNICAP   = 600,  // Unicap drivers
  CV_CAP_DSHOW    = 700,  // DirectShow (via videoInput)
  CV_CAP_PVAPI    = 800  // PvAPI, Prosilica GigE SDK
};
enum {
  CV_CAP_PROP_POS_MSEC       = 0,
  CV_CAP_PROP_POS_FRAMES     = 1,
  CV_CAP_PROP_POS_AVI_RATIO  = 2,
  CV_CAP_PROP_FRAME_WIDTH    = 3,
  CV_CAP_PROP_FRAME_HEIGHT   = 4,
  CV_CAP_PROP_FPS            = 5,
  CV_CAP_PROP_FOURCC         = 6,
  CV_CAP_PROP_FRAME_COUNT    = 7,
  CV_CAP_PROP_FORMAT         = 8,
  CV_CAP_PROP_MODE           = 9,
  CV_CAP_PROP_BRIGHTNESS    = 10,
  CV_CAP_PROP_CONTRAST      = 11,
  CV_CAP_PROP_SATURATION    = 12,
  CV_CAP_PROP_HUE           = 13,
  CV_CAP_PROP_GAIN          = 14,
  CV_CAP_PROP_EXPOSURE      = 15,
  CV_CAP_PROP_CONVERT_RGB   = 16,
  CV_CAP_PROP_WHITE_BALANCE = 17,
  CV_CAP_PROP_RECTIFICATION = 18
};
/* "black box" video file writer structure */
typedef struct CvVideoWriter CvVideoWriter;
CV_INLINE int CV_FOURCC(char c1, char c2, char c3, char c4)
{
  return (c1 & 255) + ((c2 & 255) << 8) + ((c3 & 255) << 16) + ((c4 & 255) << 24);
}
#define CV_FOURCC_PROMPT -1  /* Open Codec Selection Dialog (Windows only) */
#define CV_FOURCC_DEFAULT CV_FOURCC('I', 'Y', 'U', 'V') /* Use default codec for specified filename (Linux only) */
typedef struct capcam* pcapcam;
#define CAP_HEADER \
  int height, width, channel, imageSize; \
  unsigned char* data; \
  void ( *Close ) ( pcapcam ); \
  int ( *GrabFrame ) ( pcapcam ); \
  int ( *SetFramesPerSec )( pcapcam capture, float FramesPerSec ); \
  int ( *writeFrame )( pcapcam capture, const unsigned char* image, int image_step )
typedef struct capcam {
  CAP_HEADER;
}
capcam;
typedef unsigned char* cam_iterator;
typedef struct camera_param_t {
  long   m_CaptureW;                   /*????? */
  long   m_CaptureH;                   /*?????*/
  long   m_FramRate;                   /*???*/
  //BOOL   m_Eddy;                     /*??????*/
  //BOOL   m_Repeat;                   /*???????????*/
} camera_param_t;

//#define cam_start(i)           NewCAM_VFW(i, 480, 640)
capcam* cam_start(int index, const camera_param_t* p);

// #define cam_startavi(fn)       NewAVI_VFW(fn)
capcam* cap_open_avi(const char* filename);
capcam* cap_create_avi(const char* filename, int height, int width, int nChannels);
//??? CaptureCAM_VFW ?? capture ???????????
#define cam_stop(cap)          ((capcam*)cap)->Close(cap)

#define cam_GrabFrame(cap)     ((capcam*)cap)->GrabFrame(cap)
#define cam_SetFps(cap, fps)   ((capcam*)cap)->SetFramesPerSec(cap, fps)
#define cam_begin(cap)         (cam_iterator)(((capcam*)cap)->data)
#define cam_height(cap)        (((capcam*)cap)->height)
#define cam_width(cap)         (((capcam*)cap)->width)
#define cam_step(cap)          (((capcam*)cap)->width * ((capcam*)cap)->channel)
#define cam_channel(cap)       (((capcam*)cap)->channel)
#define cap_writeFrame(cap, _A, _L)    ((capcam*)cap)->writeFrame(cap, _A, _L)
#define cam_GetFrame(cap, _H, _W, _A, _L, _I) \
  if (cap) {_H = cam_height(cap),_W = cam_width(cap),_A = cam_begin(cap),_L = cam_step(cap), _I = cam_channel(cap);} else {_H = 0,_W = 0,_A = 0,_L = 0, _I = 0;}
#define cam_QueryFrame(cap, _H, _W, _A, _L, _I) \
  if ( ((capcam*)cap)->GrabFrame(cap) ) {_H = cam_height(cap),_W = cam_width(cap),_A = cam_begin(cap),_L = cam_step(cap), _I = cam_channel(cap);} else {_H = 0,_W = 0,_A = 0,_L = 0, _I = 0;}
typedef unsigned char* seq_iterator;
#define Scalar_convertTo(s, T2, buf0, cn, unroll_to) \
  do { \
    int i; T2* buf1=(T2*)buf0; \
    CV_Assert(cn <= 4); \
    for( i = 0; i < cn; i++ ) \
      buf1[i] = (T2)(s->val[i]); \
    for( ; i < unroll_to; i++ ) \
      buf1[i] = buf1[i-cn]; \
  } while(0)
enum {
  FONT_HERSHEY_SIMPLEX = 0,
  FONT_HERSHEY_PLAIN = 1,
  FONT_HERSHEY_DUPLEX = 2,
  FONT_HERSHEY_COMPLEX = 3,
  FONT_HERSHEY_TRIPLEX = 4,
  FONT_HERSHEY_COMPLEX_SMALL = 5,
  FONT_HERSHEY_SCRIPT_SIMPLEX = 6,
  FONT_HERSHEY_SCRIPT_COMPLEX = 7
      //FONT_ITALIC = 16
};
// -128.f ... 255.f
extern const float g_8x32fTab[];
//#define CV_8TO32F(x)  cv::g_8x32fTab[(x)+128]
extern const ushort g_8x16uSqrTab[];
#define CV_SQR_8U(x)  g_8x16uSqrTab[(x)+255]
extern const char* g_HersheyGlyphs[];
extern const uchar g_Saturate8u[];
//#define CV_FAST_CAST_8U(t)   (assert(-256 <= (t) && (t) <= 512), cv::g_Saturate8u[(t)+256])
#define CV_MIN_8U(a,b)       ((a) - CV_FAST_CAST_8U((a) - (b)))
#define CV_MAX_8U(a,b)       ((a) + CV_FAST_CAST_8U((b) - (a)))
#define CV_CPU_NONE    0
#define CV_CPU_MMX     1
#define CV_CPU_SSE     2
#define CV_CPU_SSE2    3
#define CV_CPU_SSE3    4
#define CV_CPU_SSSE3   5
#define CV_CPU_SSE4_1  6
#define CV_CPU_SSE4_2  7
#define CV_CPU_AVX    10
#define CV_HARDWARE_MAX_FEATURE 255
enum { MAX_FEATURE = CV_HARDWARE_MAX_FEATURE };
CV_INLINE int64 getTickCount()
{
#if defined _WIN
  LARGE_INTEGER counter;
  QueryPerformanceCounter(&counter);
  return (int64)counter.QuadPart;
#elif (defined __linux || defined __linux__) && defined CLOCK_MONOTONIC && 0
  struct timespec tp;
  clock_gettime(CLOCK_MONOTONIC, &tp);
  return (int64)tp.tv_sec * 1000000000 + tp.tv_nsec;
#elif defined __MACH__
  return (int64)mach_absolute_time();
#elif defined __GNUC__
  //struct timeval tv1;
  //struct timezone tz;
  //gettimeofday( &tv, 0 );
  return 0;//(int64)tv1.tv_sec*1000000 + tv1.tv_usec;
#else
  return 0;
#endif
}
CV_INLINE double getTickFrequency()
{
#if defined _WIN || defined WIN64 || defined _WIN64 || defined WINCE
  LARGE_INTEGER freq;
  QueryPerformanceFrequency(&freq);
  return (double)freq.QuadPart;
#elif defined __linux || defined __linux__
  return 1e9;
#elif defined __MACH__
  static double freq = 0;
  if (freq == 0) {
    mach_timebase_info_data_t sTimebaseInfo;
    mach_timebase_info(&sTimebaseInfo);
    freq = sTimebaseInfo.denom * 1e9 / sTimebaseInfo.numer;
  }
  return freq;
#else
  return 1e6;
#endif
}
#if defined __GNUC__ && (defined __i386__ || defined __x86_64__ || defined __ppc__)
#if defined(__i386__)
CV_INLINE int64 getCPUTickCount(void)
{
  int64 x;
  __asm__ volatile(".byte 0x0f, 0x31" : "=A"(x));
  return x;
}
#elif defined(__x86_64__)
CV_INLINE int64 getCPUTickCount(void)
{
  unsigned hi, lo;
  __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
  return (int64)lo | ((int64)hi << 32);
}
#elif defined(__ppc__)
CV_INLINE int64 getCPUTickCount(void)
{
  int64 result = 0;
  unsigned upper, lower, tmp;
  __asm__ volatile(
      "0: \n"
      "\tmftbu   %0 \n"
      "\tmftb    %1 \n"
      "\tmftbu   %2 \n"
      "\tcmpw    %2,%0 \n"
      "\tbne     0b \n"
      : "=r"(upper), "=r"(lower), "=r"(tmp)
  );
  return lower | ((int64)upper << 32);
}
#else
#error "RDTSC not defined"
#endif
#elif defined _MSC_VER && defined _WIN && !defined _WIN64
#pragma warning(disable: 4035)
CV_INLINE int64 getCPUTickCount(void)
{
  __asm _emit 0x0f;
  __asm _emit 0x31;
}
#else
CV_INLINE int64 getCPUTickCount()
{
  return getTickCount();
}
#endif
CV_INLINE char* format(const char* fmt, ...)
{
  static char buf[1 << 16];
  va_list args;
  va_start(args, fmt);
  vsprintf(buf, fmt, args);
  return (buf);
}
CV_INLINE const char* cvErrorStr(int status)
{
  static char buf[256];
  switch (status) {
  case CV_StsOk :
    return "No Error";
  case CV_StsBackTrace :
    return "Backtrace";
  case CV_StsError :
    return "Unspecified error";
  case CV_StsInternal :
    return "Internal error";
  case CV_StsNoMem :
    return "Insufficient memory";
  case CV_StsBadArg :
    return "Bad argument";
  case CV_StsNoConv :
    return "Iterations do not converge";
  case CV_StsAutoTrace :
    return "Autotrace call";
  case CV_StsBadSize :
    return "Incorrect size of input array";
  case CV_StsNullPtr :
    return "Null pointer";
  case CV_StsDivByZero :
    return "Division by zero occured";
  case CV_BadStep :
    return "Image step is wrong";
  case CV_StsInplaceNotSupported :
    return "Inplace operation is not supported";
  case CV_StsObjectNotFound :
    return "Requested object was not found";
  case CV_BadDepth :
    return "Input image depth is not supported by function";
  case CV_StsUnmatchedFormats :
    return "Formats of input arguments do not match";
  case CV_StsUnmatchedSizes :
    return "Sizes of input arguments do not match";
  case CV_StsOutOfRange :
    return "One of arguments\' values is out of range";
  case CV_StsUnsupportedFormat :
    return "Unsupported format or combination of formats";
  case CV_BadCOI :
    return "Input COI is not supported";
  case CV_BadNumChannels :
    return "Bad number of channels";
  case CV_StsBadFlag :
    return "Bad flag (parameter or structure field)";
  case CV_StsBadPoint :
    return "Bad parameter of type CvPoint";
  case CV_StsBadMask :
    return "Bad type of mask argument";
  case CV_StsParseError :
    return "Parsing error";
  case CV_StsNotImplemented :
    return "The function/feature is not implemented";
  case CV_StsBadMemBlock :
    return "Memory block has been corrupted";
  case CV_StsAssert :
    return "Assertion failed";
  };
  sprintf(buf, "Unknown %s code %d", status >= 0 ? "status" : "error", status);
  return buf;
}
/* Errors */
#define HG_OK          0 /* Don't bet on it! */
#define HG_BADNAME    -1 /* Bad window or file name */
#define HG_INITFAILED -2 /* Can't initialize HigHGUI */
#define HG_WCFAILED   -3 /* Can't create a window */
#define HG_NULLPTR    -4 /* The null pointer where it should not appear */
#define HG_BADPARAM   -5
#define __BEGIN__ __CV_BEGIN__
#define __END__  __CV_END__
#define EXIT __CV_EXIT__
#define CV_WINDOW_MAGIC_VAL     0x00420042
#define CV_TRACKBAR_MAGIC_VAL   0x00420043
#define CV_CAP_GSTREAMER_1394   0
#define CV_CAP_GSTREAMER_V4L    1
#define CV_CAP_GSTREAMER_V4L2   2
#define CV_CAP_GSTREAMER_FILE   3
#define cvSet2D(_PIX, _CN, _COL, TYPE) \
  do { \
    if (1==(_CN)) { \
      *(_PIX+0) = (TYPE)((_COL).val[0]); \
    } else if (2==(_CN)) { \
      *(_PIX+0) = (TYPE)((_COL).val[0]); \
      *(_PIX+1) = (TYPE)((_COL).val[1]); \
    } else if (3==(_CN)) { \
      *(_PIX+0) = (TYPE)((_COL).val[0]); \
      *(_PIX+1) = (TYPE)((_COL).val[1]); \
      *(_PIX+2) = (TYPE)((_COL).val[2]); \
    } else if (4==(_CN)) { \
      *(_PIX+0) = (TYPE)((_COL).val[0]); \
      *(_PIX+1) = (TYPE)((_COL).val[1]); \
      *(_PIX+2) = (TYPE)((_COL).val[2]); \
      *(_PIX+3) = (TYPE)((_COL).val[3]); \
    } \
  } while(0)
//                    Eigen objects                                         *
#define CV_EIGOBJ_NO_CALLBACK     0
#define CV_EIGOBJ_INPUT_CALLBACK  1
#define CV_EIGOBJ_OUTPUT_CALLBACK 2
#define CV_EIGOBJ_BOTH_CALLBACK   3
//                         1D/2D HMM                                        *
typedef struct CvImgObsInfo {
  int obs_x;
  int obs_y;
  int obs_size;
  float* obs;//consequtive observations
  int* state;/* arr of pairs superstate/state to which observation belong */
  int* mix;  /* number of mixture to which observation belong */
}
CvImgObsInfo;/*struct for 1 image*/
typedef CvImgObsInfo Cv1DObsInfo;
typedef struct CvEHMMState {
  int num_mix;        /*number of mixtures in this state*/
  float* mu;          /*mean vectors corresponding to each mixture*/
  float* inv_var;     /* square root of inversed variances corresp. to each mixture*/
  float* log_var_val; /* sum of 0.5 (LN2PI + ln(variance[i]) ) for i=1,n */
  float* weight;      /*array of mixture weights. Summ of all weights in state is 1. */
}
CvEHMMState;
typedef struct CvEHMM {
  int level; /* 0 - lowest(i.e its states are double states), ..... */
  int num_states; /* number of HMM states */
  float*  transP;/*transition probab. matrices for states */
  float** obsProb; /* if level == 0 - array of brob matrices corresponding to hmm
                        if level == 1 - martix of matrices */
  union {
    CvEHMMState* state; /* if level == 0 points to double states array,
                               if not - points to embedded hmms */
    struct CvEHMM* ehmm; /* pointer to an embedded model or NULL, if it is a leaf */
  } u;
}
CvEHMM;
// Embedded HMMs
// Creates 2D HMM
#define CV_COUNT_OBS(roi, win, delta, numObs) ( \
  (numObs)->w =((roi)->w - (win)->w + (delta)->w)/(delta)->w, \
  (numObs)->h =((roi)->h - (win)->h + (delta)->h)/(delta)->h)

#define CV_CURRENT_INT( reader ) (*((int *)(reader).ptr))
#define CV_PREV_INT( reader ) (*((int *)(reader).prev_elem))
#define  CV_GRAPH_WEIGHTED_VERTEX_FIELDS() CV_GRAPH_VERTEX_FIELDS()\
  float weight;
#define  CV_GRAPH_WEIGHTED_EDGE_FIELDS() CV_GRAPH_EDGE_FIELDS()
typedef enum CvGraphWeightType {
  CV_NOT_WEIGHTED,
  CV_WEIGHTED_VTX,
  CV_WEIGHTED_EDGE,
  CV_WEIGHTED_ALL
} CvGraphWeightType;
// Stereo correspondence
#define CLIQUE_TIME_OFF 2
#define CLIQUE_FOUND 1
#define CLIQUE_END   0

#define CV_UNDEF_SC_PARAM         12345 //default value of parameters
#define CV_IDP_BIRCHFIELD_PARAM1  25
#define CV_IDP_BIRCHFIELD_PARAM2  5
#define CV_IDP_BIRCHFIELD_PARAM3  12
#define CV_IDP_BIRCHFIELD_PARAM4  15
#define CV_IDP_BIRCHFIELD_PARAM5  25
#define CV_DISPARITY_BIRCHFIELD  0
//    Name:    cvFindStereoCorrespondence
//    Purpose: find stereo correspondence on stereo-pair
//    Context:
//    Parameters:
//      leftImage - left image of stereo-pair (format 8uC1).
//      rightImage - right image of stereo-pair (format 8uC1).
//   mode - mode of correspondence retrieval (now CV_DISPARITY_BIRCHFIELD only)
//      dispImage - destination disparity image
//      maxDisparity - maximal disparity
//      param1, param2, param3, param4, param5 - parameters of algorithm
//    Returns:
//    Notes:
//      Images must be rectified.
//      All images must have format 8uC1.
//
///                      Older definitions 
typedef float*   CvVect32f;
typedef float*   CvMatr32f;
typedef double*  CvVect64d;
typedef double*  CvMatr64d;
typedef struct CvMatrix3 {
  float m[3][3];
}
CvMatrix3;
typedef struct CvConDensation {
  int MP;
  int DP;
  float* DynamMatr;       /* Matrix of the linear Dynamics system  */
  float* State;           /* Vector of State                       */
  int SamplesNum;         /* Number of the Samples                 */
  float** flSamples;      /* arr of the Sample Vectors             */
  float** flNewSamples;   /* temporary array of the Sample Vectors */
  float* flConfidence;    /* Confidence for each Sample            */
  float* flCumulative;    /* Cumulative confidence                 */
  float* Temp;            /* Temporary vector                      */
  float* RandomSample;    /* RandomVector to update sample set     */
  struct CvRandState* RandS; /* Array of structures to generate random vectors */
}
CvConDensation;
/*
standard Kalman filter (in G. Welch' and G. Bishop's notation):
  x(k)=A*x(k-1)+B*u(k)+w(k)  p(w)~N(0,Q)
  z(k)=H*x(k)+v(k),   p(v)~N(0,R)
*/
typedef struct CvKalman {
  int MP;                     /* number of measurement vector dimensions */
  int DP;                     /* number of state vector dimensions */
  int CP;                     /* number of control vector dimensions */
  /* backward compatibility fields */
#if 1
  float* PosterState;         /* =state_pre->tt.data.fl */
  float* PriorState;          /* =state_post->tt.data.fl */
  float* DynamMatr;           /* =transition_matrix->tt.data.fl */
  float* MeasurementMatr;     /* =measurement_matrix->tt.data.fl */
  float* MNCovariance;        /* =measurement_noise_cov->tt.data.fl */
  float* PNCovariance;        /* =process_noise_cov->tt.data.fl */
  float* KalmGainMatr;        /* =gain->tt.data.fl */
  float* PriorErrorCovariance;/* =error_cov_pre->tt.data.fl */
  float* PosterErrorCovariance;/* =error_cov_post->tt.data.fl */
  float* Temp1;               /* temp1->tt.data.fl */
  float* Temp2;               /* temp2->tt.data.fl */
#endif
#if 0
  CvMat* state_pre;           /* predicted state (x'(k)):
                                    x(k)=A*x(k-1)+B*u(k) */
  CvMat* state_post;          /* corrected state (x(k)):
                                    x(k)=x'(k)+K(k)*(z(k)-H*x'(k)) */
  CvMat* transition_matrix;   /* state transition matrix (A) */
  CvMat* control_matrix;      /* control matrix (B)
                                   (it is not used if there is no control)*/
  CvMat* measurement_matrix;  /* measurement matrix (H) */
  CvMat* process_noise_cov;   /* process noise covariance matrix (Q) */
  CvMat* measurement_noise_cov; /* measurement noise covariance matrix (R) */
  CvMat* error_cov_pre;       /* priori error estimate covariance matrix (P'(k)):
                                    P'(k)=A*P(k-1)*At + Q)*/
  CvMat* gain;                /* Kalman gain matrix (K(k)):
                                    K(k)=P'(k)*Ht*inv(H*P'(k)*Ht+R)*/
  CvMat* error_cov_post;      /* posteriori error estimate covariance matrix (P(k)):
                                    P(k)=(I-K(k)*H)*P'(k) */
  CvMat* temp1;               /* temporary matrices */
  CvMat* temp2;
  CvMat* temp3;
  CvMat* temp4;
  CvMat* temp5;
#endif
}
CvKalman;
/*********************** Haar-like Object Detection structures **************************/
#define CV_HAAR_MAGIC_VAL    0x42500000
#define CV_TYPE_NAME_HAAR    "opencv-haar-classifier"
#define CV_IS_HAAR_CLASSIFIER( haar ) \
  ((haar) != NULL && \
      (((const CvHaarClassifierCascade*)(haar))->flags & CV_MAGIC_MASK)==CV_HAAR_MAGIC_VAL)
#define CV_HAAR_FEATURE_MAX  3
//                     Contour Morphing                                     *
//                      Texture Descriptors                                 *
#define CV_GLCM_OPTIMIZATION_NONE                   -2
#define CV_GLCM_OPTIMIZATION_LUT                    -1
#define CV_GLCM_OPTIMIZATION_HISTOGRAM              0
#define CV_GLCMDESC_OPTIMIZATION_ALLOWDOUBLENEST    10
#define CV_GLCMDESC_OPTIMIZATION_ALLOWTRIPLENEST    11
#define CV_GLCMDESC_OPTIMIZATION_HISTOGRAM          4
#define CV_GLCMDESC_ENTROPY                         0
#define CV_GLCMDESC_ENERGY                          1
#define CV_GLCMDESC_HOMOGENITY                      2
#define CV_GLCMDESC_CONTRAST                        3
#define CV_GLCMDESC_CLUSTERTENDENCY                 4
#define CV_GLCMDESC_CLUSTERSHADE                    5
#define CV_GLCMDESC_CORRELATION                     6
#define CV_GLCMDESC_CORRELATIONINFO1                7
#define CV_GLCMDESC_CORRELATIONINFO2                8
#define CV_GLCMDESC_MAXIMUMPROBABILITY              9
#define CV_GLCM_ALL                                 0
#define CV_GLCM_GLCM                                1
#define CV_GLCM_DESC                                2
typedef struct CvGLCM CvGLCM;
//                    Face eyes&mouth tracking                              *
typedef struct CvFaceTracker CvFaceTracker;
#define CV_NUM_FACE_ELEMENTS    3
enum CV_FACE_ELEMENTS {
  CV_FACE_MOUTH = 0,
  CV_FACE_LEFT_EYE = 1,
  CV_FACE_RIGHT_EYE = 2
};
/****************************************************************************************
 tracking_info is a rectangular array; one row per camera, num_objects elements per row.
 The id field of any unused slots must be -1. Ids need not be ordered or consecutive. On
 completion, the return value is the number of objects located; i.e., the number of objects
 visible by more than one camera. The id field of any unused slots in tracked objects is
 set to -1.
****************************************************************************************/
//             Skeletons and Linear-Contour Models                          *
typedef enum CvLeeParameters {
  CV_LEE_INT = 0,
  CV_LEE_FLOAT = 1,
  CV_LEE_DOUBLE = 2,
  CV_LEE_AUTO = -1,
  CV_LEE_ERODE = 0,
  CV_LEE_ZOOM = 1,
  CV_LEE_NON = 2
} CvLeeParameters;
#define CV_NEXT_VORONOISITE2D( SITE ) ((SITE)->edge[0]->site[((SITE)->edge[0]->site[0] == (SITE))])
#define CV_PREV_VORONOISITE2D( SITE ) ((SITE)->edge[1]->site[((SITE)->edge[1]->site[0] == (SITE))])
#define CV_FIRST_VORONOIEDGE2D( SITE ) ((SITE)->edge[0])
#define CV_LAST_VORONOIEDGE2D( SITE ) ((SITE)->edge[1])
#define CV_NEXT_VORONOIEDGE2D( EDGE, SITE ) ((EDGE)->next[(EDGE)->site[0] != (SITE)])
#define CV_PREV_VORONOIEDGE2D( EDGE, SITE ) ((EDGE)->next[2 + ((EDGE)->site[0] != (SITE))])
#define CV_VORONOIEDGE2D_BEGINNODE( EDGE, SITE ) ((EDGE)->node[((EDGE)->site[0] != (SITE))])
#define CV_VORONOIEDGE2D_ENDNODE( EDGE, SITE ) ((EDGE)->node[((EDGE)->site[0] == (SITE))])
#define CV_TWIN_VORONOISITE2D( SITE, EDGE ) ( (EDGE)->site[((EDGE)->site[0] == (SITE))])
/* Computes Voronoi Diagram for given polygons with holes */
/*********************** Linear-Contour Model ****************************/
struct CvLCMEdge;
struct CvLCMNode;
/* Computes hybrid model from Voronoi Diagram */
/*************************** View Morphing Functions ************************/
/* The order of the function corresponds to the order they should appear in
   the view morphing pipeline */
/* Finds ending points of scanlines on left and right images of stereo-pair */
//             Background/foreground segmentation                           *
#define CV_BG_MODEL_FGD     0
#define CV_BG_MODEL_MOG     1
#define CV_BG_MODEL_FGD_SIMPLE   2
struct CvBGStatModel;
typedef void (* CvReleaseBGStatModel)(struct CvBGStatModel** bg_model);
typedef int (* CvUpdateBGStatModel)(unsigned char* imageData, int widthStep, struct CvBGStatModel* bg_model);
#define MINIVEC(_TYPE)            struct { _TYPE* data; unsigned int used, size; }
#define MINIVEC_INIT(_V, _P, _U, _N)  (((_V)->tt.data) = (void*)(_P), ((_V)->used) = (_U), ((_V)->size) = (_N))
#define MINIVEC_ALLOC(_V, _N) MINIVEC_INIT(_V, malloc((_N)*sizeof(*((_V)->tt.data))), 0, _N)
#define MINIVEC_FREE(_V)      (free((_V)->tt.data), (_V)->tt.data=0, (_V)->used = (_V)->size=0)
#define MINIVEC_AT(_V, _i)    *(((_V)->tt.data)+(_i))
//#define MINIVEC_ADD(_V, _X)   (ASSERT((_V)->used<(_V)->size), MINIVEC_AT(_V, (_V)->used++) = _X)
#define MINIVEC_ADD(_V, _X)   if((_V)->used<(_V)->size) MINIVEC_AT(_V, (_V)->used++) = _X
#define MINIVEC_POP(_V)       ASSERT((_V)->used>0), MINIVEC_AT(_V, --(_V)->used)
#define CV_BG_STAT_MODEL_FIELDS() \
  int             type, height, width, nChannels; /*type of BG model*/ \
  CvReleaseBGStatModel release; \
  CvUpdateBGStatModel update; \
  unsigned char*       background;   /*8UC3 reference background image*/ \
  unsigned char*       foreground;   /*8UC1 foreground image*/ \
  unsigned char**      layers;       /*8UC3 reference background image, can be null */ \
  int             layer_count  /* can be zero */
/*CONTOURS        foreground_regions; */
/*CvMemStorage*   storage;      storage for ?foreground_regions*/
/*CvSeq*          foreground_regions foreground object contours*/
typedef struct CvBGStatModel {
  CV_BG_STAT_MODEL_FIELDS();
} CvBGStatModel;
// Releases memory used by BGStatModel
#define cvReleaseBGStatModel( bg_model )  if( bg_model && (*bg_model) && (*bg_model)->release ) { (*bg_model)->release( bg_model ); }
// Updates statistical model and returns number of found foreground regions
#define cvUpdateBGStatModel( imageData, widthStep, bg_model )   (bg_model && bg_model->update ? bg_model->update( imageData, widthStep, bg_model ) : 0)
// Performs FG post-processing using segmentation
// (all pixels of a region will be classified as foreground if majority of pixels of the region are FG).
// parameters:
//      segments - pointer to result of segmentation (for example MeanShiftSegmentation)
//      bg_model - pointer to CvBGStatModel structure
/*
  Interface of ACM MM2003 algorithm
  (Liyuan Li, Weimin Huang, Irene Y.H. Gu, and Qi Tian.
  "Foreground Object Detection from Videos Containing Complex Background. ACM MM2003")
*/
/* default paremeters of foreground detection algorithm */
#define  CV_BGFG_FGD_LC              128
#define  CV_BGFG_FGD_N1C             15
#define  CV_BGFG_FGD_N2C             25
#define  CV_BGFG_FGD_LCC             64
#define  CV_BGFG_FGD_N1CC            25
#define  CV_BGFG_FGD_N2CC            40
/* BG reference image update parameter */
#define  CV_BGFG_FGD_ALPHA_1         0.1f
/* stat model update parameter
   0.002f ~ 1K frame(~45sec), 0.005 ~ 18sec (if 25fps and absolutely static BG) */
#define  CV_BGFG_FGD_ALPHA_2         0.005f
/* start value for alpha parameter (to fast initiate statistic model) */
#define  CV_BGFG_FGD_ALPHA_3         0.1f
#define  CV_BGFG_FGD_DELTA           2
#define  CV_BGFG_FGD_T               0.9f
#define  CV_BGFG_FGD_MINAREA         15.f
#define  CV_BGFG_FGD_BG_UPDATE_TRESH 0.5f
typedef struct CvFGDStatModelParams {
  int           Lc, N1c, N2c, Lcc, N1cc, N2cc, is_obj_without_holes, perform_morphing;
  float         alpha1, alpha2, alpha3, delta, T, minArea;
}
CvFGDStatModelParams;
typedef struct CvBGPixelCStatTable {
  float          Pv, Pvb;
  uchar          v[3];
}
CvBGPixelCStatTable;
typedef struct CvBGPixelCCStatTable {
  float          Pv, Pvb;
  uchar          v[6];
}
CvBGPixelCCStatTable;
typedef struct CvBGPixelStat {
  float                 Pbc;
  float                 Pbcc;
  CvBGPixelCStatTable*  ctable;
  CvBGPixelCCStatTable* cctable;
  uchar                 is_trained_st_model;
  uchar                 is_trained_dyn_model;
}
CvBGPixelStat;
typedef struct CvFGDStatModel {
  CV_BG_STAT_MODEL_FIELDS();
  CvBGPixelStat*         pixel_stat;
  unsigned char*         Ftd;
  unsigned char*         Fbd;
  unsigned char*         prev_frame;
  CvFGDStatModelParams   params;
}
CvFGDStatModel;
/* Creates FGD model */
/*
   Interface of Gaussian mixture algorithm
   (P. KadewTraKuPong and R. Bowden,
   "An improved adaptive background mixture model for double-time tracking with shadow detection"
   in Proc. 2nd European Workshp on Advanced Video-Based Surveillance Systems, 2001.")
*/
#define CV_BGFG_MOG_MAX_NGAUSSIANS 500
/* default parameters of gaussian background detection algorithm */
#define CV_BGFG_MOG_BACKGROUND_THRESHOLD     0.7f    // ?锟斤拷???????????????
#define CV_BGFG_MOG_STD_THRESHOLD            2.5f    // lambda=2.5 is 99% ??????
#define CV_BGFG_MOG_WINDOW_SIZE              200     // ?????????1/200??0.005
#define CV_BGFG_MOG_NGAUSSIANS               5       // ??????????
#define CV_BGFG_MOG_WEIGHT_INIT              0.05f   // ??
#define CV_BGFG_MOG_SIGMA_INIT               30      // ????
#define CV_BGFG_MOG_MINAREA                  25.f    // ?????????????????
#define CV_BGFG_MOG_NCOLORS                  3
typedef float GaussReal_t;
// win_size       ???????锟斤拷?????????????????? ???a=1/win_size;
// bg_threshold   ????????????????????????
// std_threshold  ??????????????
// weight_init    ????????????
// variance_init  ??????????????
// minArea        ??锟斤拷??????????????????????????????????????锟斤拷????minArea?????????????????
// n_gauss        ???????????????
typedef struct CvGaussBGStatModelParams {
  int     win_size;               /* = 1/alpha */
  int     n_gauss;
  GaussReal_t  bg_threshold, std_threshold, minArea;
  GaussReal_t  weight_init, variance_init;
} CvGaussBGStatModelParams;
typedef struct CvGaussBGValues {
  int         match_sum;
  GaussReal_t      weight;
  GaussReal_t      variance[CV_BGFG_MOG_NCOLORS];
  GaussReal_t      mean[CV_BGFG_MOG_NCOLORS];
}
CvGaussBGValues;
typedef struct CvGaussBGPoint {
  CvGaussBGValues* g_values;
}
CvGaussBGPoint;
typedef struct CvGaussBGModel {
  CV_BG_STAT_MODEL_FIELDS();
  CvGaussBGStatModelParams   params;
  CvGaussBGPoint*            g_point;
  int                        countFrames;
}
CvGaussBGModel;
#ifndef COLOR_TYPE_DEF_DEF
#define COLOR_TYPE_DEF_DEF(COLOR_TYPE_DEF) \
COLOR_TYPE_DEF(1, GRAY) \
COLOR_TYPE_DEF(3, BGR) \
COLOR_TYPE_DEF(3, RGB) \
COLOR_TYPE_DEF(4, BGRA) \
COLOR_TYPE_DEF(2, BGR555) \
COLOR_TYPE_DEF(2, BGR565) \
COLOR_TYPE_DEF(2, YUYV) \
COLOR_TYPE_DEF(2, UYVY) \
COLOR_TYPE_DEF(2, YUV420P) \
COLOR_TYPE_DEF(2, YUV420) \
COLOR_TYPE_DEF(2, YUV411P) \
COLOR_TYPE_DEF(1, BayerGR) \
COLOR_TYPE_DEF(1, BayerGB) \
COLOR_TYPE_DEF(1, BayerBG) \
COLOR_TYPE_DEF(3, HSV) \
COLOR_TYPE_DEF(3, XYZ) \
COLOR_TYPE_DEF(3, HLS) \
COLOR_TYPE_DEF(3, Lab) \
COLOR_TYPE_DEF(3, Luv) \
COLOR_TYPE_DEF(3, YCrCb) \
COLOR_TYPE_DEF(2, YCrYCb) \
/* COLOR_TYPE_DEF(2, SBGGR8) */ \
COLOR_TYPE_DEF(2, SN9C10X) \
COLOR_TYPE_DEF(2, SGBRG) \
COLOR_TYPE_DEF(12, HSV32F)
enum //
{
#define COLOR_TYPE_DEF(a, b)  T_##b,
  COLOR_TYPE_DEF_DEF(COLOR_TYPE_DEF)
#undef COLOR_TYPE_DEF
};
#endif // COLOR_TYPE_DEF_DEF

//#include "libfacerecog.h"
//int alg_memsize( int algtype, void* param );
//int alg_process( int algtype, void* param );
int cascadesize(const void* p);
#define MAX_CAS_NUM (4)
#ifndef USE_FIX_FLOAT
#define USE_FIX_FLOAT 1        // ????????
#endif // USE_FIX_FLOAT
#ifdef USE_FIX_FLOAT
typedef int FIX_FLOAT;
#else
typedef double FIX_FLOAT;
#endif
#ifndef XRECT_DEFINED
#define XRECT_DEFINED
// 矩形框
typedef struct XRECT {
  int x, y, w, h;      /* 一个目标的所有框的位置平均值 */
  int score;           /* 一个目标的所有框的得分平均值 */
  int count;             /* 一个目标的所有框的个数       */
  //INT id;
}
XRECT;
#endif // XRECT_DEFINED
typedef struct lut_detect_param_t {
  //????
  int height, width, datastep, maskstep, stepxy,
  mincnt, fastmode, maxoutlen, casnum;
  int is_trans, is_flop; // ??????????
  double sc, ssmin, ssmax, ss, thd;
  const unsigned char* data, *mask;
  void const* cas[MAX_CAS_NUM];
  XRECT roi;
  //???
  int outlen;
  XRECT* out;
  int buflen;
  unsigned char* buf;
  //??????
  //int isc, issmin, issmax, iss, ithd; /*???ARM??DSP?????????????DM6446?????????????*/
  int h, w; //h=height*sc; w=width*sc;
  void* pWirk[MAX_CAS_NUM]; // = MALLOC( iHaarWork, ca->ihlen );
  FIX_FLOAT* inter; // = MALLOC( FIX_FLOAT, ( h + 1 ) * ( w + 1 ) );
  FIX_FLOAT* inter1; // = MALLOC( FIX_FLOAT, ( h + 1 ) * ( w + 1 ) );
  unsigned char* tmpimg0; // = MALLOC( unsigned char, w * h );
  unsigned char* tmpmask0; // = MALLOC( unsigned char, w * h );
  unsigned char* tmpimg; // = MALLOC( unsigned char, w * h );
  unsigned char* tmpmask; // = MALLOC( unsigned char, w * h );
  unsigned char* rect_partition_buf;
} lut_detect_param_t;
int detect_init(void* p, const void* cas[], int casnum, int height, int width,
    const unsigned char* data, int datastep,
    const unsigned char* mask, int maskstep,
    double sc, double ssmin, double ssmax, double ss, int stepxy, double thd,
    int mincnt, const XRECT* roi, XRECT* out, int maxoutlen, int is_trans);
int detect_memsize(void* p);
int detect_process(void* p);
#ifndef REAL_TYPE
// ????????
#define USEFLOT
#ifdef USEFLOT
#define REAL_TYPE double
#else
#define REAL_TYPE int
#endif // USEFLOT
#endif
#define CHI_DIST(a, b, out)  { out = (a) + (b); (out) = (out) ? (((a)-(b))*((a)-(b))/(out)) : 0; }
#define ABS_DIST(a, b, out)  { (out) = ((a)>(b)) ? ((a)-(b)) : ((b)-(a)); }
#define RECOGNITION_0 (0)  /* ???锟斤拷??????????????????????????????11?????????????0????????????80*80???????????64000??????锟斤拷??????????                                    ????????????????1-10??????????????????锟斤拷??0???????50000??????0.52??????0.52???????????锟斤拷??0.52??????*/
#define RECOGNITION_1 (1)  /* ???锟斤拷?????????????????????????????????????????????????11????????????2????????????80*80??????                                               ?????64000??????????????锟斤拷??0???????288550??????0.52??锟斤拷??0.52???????????????0.52??????*/
#define RECOGNITION_2 (2)  /* ???锟斤拷???????????????????????????????1-10?????????????????11????????????2????????????80*80???????????64000???                                                     ????????????????锟斤拷??0???????169000 ??????0.52??锟斤拷??0.52???????????????0.52??????*/
#define RECOGNITION_3 (3)  /* ???锟斤拷????PC????,??????????1-600???????????????????????????????????????????????6?????????????0???????                                    ??????100*100???????????2243????????????锟斤拷??0???????8000??????0.51?????锟斤拷????0.51?????????锟斤拷??0.51??                                    ??????*/
#define RECOGNITION_4 (4)  /* ???锟斤拷??????????????????锟斤拷???锟斤拷????????????????????????????????9????????????100*100???锟斤拷????100000???????                                     ??????????? */
#define RECOGNITION_5 (5)  /* ???锟斤拷????PC????,??????????1-600???????????????????????????????????????????????12?????????????0?????                                   ??????100*100???????????2243????????????锟斤拷??0???????12154??????0.52?????锟斤拷????0.52?????????锟斤拷??0.52                                       ?????????锟斤拷?????????????????????  */
#define RECOGNITION_6 (6)  /* ??锟斤拷???????????????锟斤拷???????????????????锟斤拷????????????锟斤拷?????????2.??????????????????3???????????                                      ?2.?????????5900????????????锟斤拷??0???????11719??????0.52?????锟斤拷????0.52???????锟斤拷?????????*/
#define RECOGNITION_7 (7)  /* ??锟斤拷??????????????????????????????锟斤拷???????????????2165??????????????13?????????????3????????????                                   25000????锟斤拷??0??????0.52?????锟斤拷?锟斤拷??0.52????????????????*/
typedef enum feature_type {
  LBP8BIN = 0,          /* 8??bin??lbp????*/
  LBP59BIN,             /* 59??bin??lbp????*/
  PPLBP8BIN,            /* ??????????8??bin??lbp????*/
  PPLBP59BIN,           /* ??????????59??bin??lbp????*/
  BOOSTLBP59BIN,        /* ????boost????????59??bin??lbp???????????????*/
  PPBOOSTLBP59BIN,      /* ????boost????????59??bin??lbp???????????????*/
  PPBOOSTLBPXBIN,       /* ????boost????????X??bin??lbp?????????100??100?????*/
  PPBOOSTLBPXBINFIX,    /* ????boost????????X??bin??lbp????(fix bug)?????100??100?????*/
  PPLBPXBIN,            /* ?????boost????????59??bin??lbp?????????100??100?????*/
  LBP59RECTBIN,         /* ????????????锟斤拷???????????100??100?????*/
  PPLBP59BIN2,
  PPLGBP8BINCOMPRESS,
  BOOSTLBPXBIN,         /* ????boost????????X??bin??lbp?????????100??100????????PP??*/
  PPBOOSTLBPXBINABS,    /* ????boost????????X??bin??lbp?????????100??100???????????????????????*/
  EDGEGRADLBP,          /* EDGEGRADLBP?????????100??100???????????????????????*/
  BOOSTEDGEGRADLBP,     /* ????boost????????EDGEGRADLBP?????????100??100???????????????????????*/
  MAX_FEAT
} feature_type;
typedef enum distance_type {
  CPM_CHISQUARE,       /* ???????? */
  CPM_COSINE,          /* ??????? */
  CPM_HISTINTER,       /* ???????????? */
  CPM_ABSSUB           /* ?????????? */
} distance_type;
/* ????? */
typedef struct RECOALGO {
  int feat_type;    /* ???????? */
  int feat_dims;    /* ??????????????? */
  int std_height;   /* ??????????0???????? */
  int std_width;    /* ??????????0???????? */
  int std_cn;       /* ???????????? */
  int is_dist;      /* 1????????0???????? */
  double dist_min;  /* ??????锟斤拷?????????????????0~1??? */
  double dist_max;  /* ???????????????(?????-dist_min)/(dist_max-dist_min) */
  double dist_th;   /* ??????? */
  int dist_type;    /* ???????? */
}
RECOALGO;
typedef struct pplight_param_t {
  int height, width, srcstep, srccn, dststep;
  const unsigned char* src;
  int dstlen;
  unsigned char* dst;
  int buflen;
  char* buf;
} pplight_param_t;
int pplight_memsize(void* p0);
int pplight_process(void* p0);

// bgmodel 的工作模式
#define BGMODEL_INIT    0 // 重新初始化
#define BGMODEL_UPDATE  1 // 更新背景
typedef int FIX16;
typedef struct bgmodel {
  // 输入
  int height, width, step, nChannels; // 输入
  int mode;             // 工作模式 BGMODEL_INIT 或 BGMODEL_UPDATE
  unsigned char* frame; // 输入的当前帧
  int     update_thd;   // 更新阈值 取值范围 0~255  建议值 10
  FIX16   update_rate;  // 更新率   取值范围 0~2^13
  // 输出
  unsigned char* background;   //RGB3通道背景图像 [h,w,w*3]
  unsigned char* foreground;   //二值单通道前景   [h,w,w]
  // 输入输入
  char* buf;            // 更新的数据区，及临时缓冲区
  int   buflen;         // 更新的数据区，及临时缓冲区的字节总大小
}
bgmodel;
int bgmodel_memsize(bgmodel* b);
  int bgmodel_process(bgmodel* b);
#define ARRSIZE(_T, _N)   (sizeof(_T)*(_N))
// ???lbp????
// select_type - ??????????? 0-?????????1-2443??2-2165??????????????????
typedef struct lbpex_param_t {
  //????
  int height, width, srcstep, srccn;
  const unsigned char* src;
  int do_pp; //????? pplight
  int fillborder; //??????????????????
  //lbp????
  int select_type;
  int isall;
  //int* lbp_Feature_idyw; // ??????????????????len=[featurelen]
  //???
  int featurelen;
  unsigned short* feature;
  int buflen;
  unsigned char* buf;
} lbpex_param_t;
int lbpex_memsize(void* p0);
int lbpex_process(void* p0);
//image_feature_memsize
//321
//4 0
//567
static const IPOINT icvCodeDeltas[8] =
{ {1, 0}, {1, -1}, {0, -1}, { -1, -1}, { -1, 0}, { -1, 1}, {0, 1}, {1, 1} };
#undef CV_READ_CHAIN_POINT
#define CV_READ_CHAIN_POINT( _pt, code )  (ASSERT( (code & ~7) == 0 ), _pt.x += icvCodeDeltas[(int)(code)].x, _pt.y += icvCodeDeltas[(int)(code)].y )
#define THRESH_OTSU(H, W, A, AL, B, BL, inv) otsu_histeq(H, W, A, AL, B, BL, inv);
#define THRESH_RATE(H, W, A, AL, B, BL, M, ML, thdrate) cvThreshold(H, W, A, AL, B, BL, GetThreshVal_rate(H, W, A, AL, M, ML, thdrate), 255, CV_THRESH_BINARY);
int imcalchist(int h, int w, const unsigned char* a, int al, int ai);
typedef struct asmfit_param_t {
  int height, width, datastep;
  const unsigned char* data; /*???????????????????????*/
  IPOINT leye, reye; // ??????????????????锟斤拷?????
  const void* asm_mode; // asm???????
  // ???????
  // ?????????????????????
  FPOINT* shape_result; //? Shape_result_len/2????x??????Shape_result_len/2????y????
  int shape_result_len; // ??????????????
  int* contours_ending_points; //?????????????????????[ContoursEndingPointsNum]
  int contours_ending_points_len;
  // ????????????????????, Shape_result????????????????buf?锟斤拷?
  int buflen;// ??????????????锟斤拷 ??asmfit_memsize??????????
  char* buf; // ?????????????(??????????锟斤拷???>=buflen)
} asmfit_param_t;
int asmfit_memsize(void* p0);
int asmfit_process(void* p0);
// 2D piecewise affine warp
typedef struct tagAAMPAW {
  int pixTri;
  double alpha;
  double belta;
  double gamma;
}
AAMPAW;
typedef struct tagIMAGE11 {
  int height;
  int width;
  int cn;
  unsigned char* data;
}
IMAGE11, *PIMAGE;
typedef struct tagAAM {
  double* AllShapes;
  double* MeanShape;
  AAMPAW* paw;
  PIMAGE* AllImages;
  int* tris; // ?????? tris[i*3+j]
  int* edges; // ??? edges[i*2+j]
  // ???????
  int* mask;
  int height;
  int width;
  int step;
  int nSamples;
  int nPoints;
  int tri_num;
  int edge_num;
  int pix_num;
}
AAM, *PAAM;
AAM* new_aam(const char* txtfile);
void del_aam(AAM* paam);
typedef struct delaunay_t {
  const FPOINT* pt;
  int point_num;
  int* edges;
  int edge_num;
  int* tris;
  int tri_num;
  char* buf;
  int buflen;
} delaunay_t;
enum { CP_LZF };
typedef struct comp_param_t {
  int buflen, srclen, dstlen, mode;
  char* buf;
  char* src;
  char* dst;
} comp_param_t;
#define MEMCAT(_T, _D, _DN, _S, _SN)   _D=(_T*)memcat(_D, (_DN)*sizeof(_T), _S, (_SN)*sizeof(_T))
static void* memcat(void* pDest, size_t Len, const void* pSrc, size_t Len2)
{
  pDest = realloc(pDest, Len + Len2);
  memmove((char*)pDest + Len, pSrc, Len2);
  return pDest;
}
// mode
enum {
  CAP_EROAD, //????????
  CAP_DSHOW, //dshow???
  CAP_OPENNI, //openni????????
};
// mode
enum {
  CAP_GRAY,   //?????
  CAP_RGB,    //RGB??
  CAP_DEPTH,  //?????
};
typedef struct cap_t {
  void* x;
  int (*getframe)(struct cap_t* s, img_t* im, int flag);
  int (*close)(struct cap_t* s);
} cap_t;
typedef struct cap_t capdev;
#define cap_num(mode)  cap_open(NULL, 0, mode)
#define cap_getframe(s, im, flag)  (s)->getframe(s, im, flag)
#define cap_close(s)  (s)->close(s)
#ifdef __cplusplus
extern "C" //
{
#endif
int cap_open(cap_t* s, int idx, int mode);
#ifdef __cplusplus
}
#endif
#if 0
int caperoad_capnum();
int caperoad_open(capdev* s, int idx);
int caperoad_close(capdev* s);
int caperoad_getframe(capdev* s, img_t* im);
int caperoad_set_sensitometry(capdev* s0, int Saturation);
int test_CCameraDS();
int dshow_capnum();
int dshow_open(capdev* s, int indx);
int dshow_close(capdev* s);
int dshow_getframe(capdev* s, img_t* im);
int openni_capnum();
int openni_open(capdev* s, int indx);
int openni_close(capdev* s);
int openni_getframe(capdev* s, img_t* im);
int openni_getframe_depth(capdev* s, img_t* im);
int test_capopenni();
#endif

typedef struct str_t {
  int l;
  char* s;
} str_t;

CV_INLINE str_t STR2(const char* s, int l)
{
  str_t s0;
  s0.s = (char*)s, s0.l = l;
  return s0;
}

// http://zh.wikipedia.org/wiki/%E5%AD%97%E7%AC%A6%E9%9B%86
#define LBYTE(X)  ((X)&0xff)
#define HBYTE(X)  LBYTE((X)>>8)
//#include "str/strstr.inl"

#define isupr(ch)      ('A'<=ch && ch<='Z')
#define islow(ch)      ('a'<=ch && ch<='z')
#define isalph(ch)     (isupr(ch)||islow(ch))
#define isnmb(ch)      ('0'<=ch && ch<='9')
#define isalphnmb(ch)  (isalph(ch)||isnmb(ch))
#define isopt(ch)      ('+'==ch||'-'==ch||'*'==ch||'/'==ch||'^'==ch||'%'==ch)

CV_INLINE ushort chr_next(const void* s, int i, int* poff)
{
  ushort chr;
  uchar* us = (uchar*)s;
  int off;
  if (us[i] & 0x80) {
    chr = (us[i] << 8) | us[ i + 1 ];
    off = i + 2;
  }
  else {
    chr = (us[i]);
    off = i + 1;
  }
  if (poff) {
    *poff = off;
  }
  return chr;
}
CV_INLINE ushort chr_prev(const void* s, int i, int* poff)
{
  ushort chr;
  uchar* us = (uchar*)s;
  int off;
  if (i > 1 && (us[i - 1] & 0x80)) {
    off = i - 2;
  }
  else {
    chr = (us[i]);
    off = i - 1;
  }
  if (poff) {
    *poff = off;
  }
  return chr;
}
CV_INLINE int cstr_len(const void* s)
{
  int length = 0;
  if (s) {
    uchar* us = (uchar*)s;
    while (*us++) {
      ++length;
    }
  }
  return(length);
}
CV_INLINE char* cstr_dup(const char* s)
{
  int n = cstr_len(s);
  char* p = (char*)malloc(n + 1);
  memcpy(p, s, n + 1);
  return p;
}
#define chr_tolower(ch)   (uchar)(('A'<=i && i<='Z') ? ((ch)-'A'+'a') : (ch))
#define chr_toupper(ch)   (uchar)(('a'<=i && i<='a') ? ((ch)-'a'+'A') : (ch))
#define CSTRINITLEN(str, len)   (len = ((len)<0 ? cstr_len((const char*)(str)) : len))
#define CSTRINIT(str, len)    uchar* u ## s = (uchar*)(str); int u ## len = CSTRINITLEN(str, len)
CV_INLINE char* cstr_tolower(char* s, int l)
{
  CSTRINIT(s, l);
  int i;
  for (; i<ul; ++i) {
    us[i] = chr_tolower(us[i]);
  }
  return s;
}
CV_INLINE char* cstr_toupper(char* s, int l)
{
  CSTRINIT(s, l);
  int i;
  for (; i<ul; ++i) {
    us[i] = chr_toupper(us[i]);
  }
  return s;
}
CV_INLINE int cstr_trimleft(char* s, int l)
{
  int i;
  for (i = 0; i < l && ' ' == s[i] && '\t' == s[i]; ++i) {}
  if (i > 0) {
    memcpy(s, s + i, l - i);
    s[l - i] = 0;
  }
  return l - i;
}
CV_INLINE int cstr_trimright(char* s, int l)
{
  int i;
  for (i = l; i > 0 && ' ' == s[i - 1] && '\t' == s[i - 1]; --i) {}
  s[i] = 0;
  return i;
}
// 把 i~n 替换成
CV_INLINE int memins(const void* p, int l, int i, int n, const char* p0, int l0, char* p1, int l1)
{
  const char* s = (const char*)p;
  const char* s0 = (const char*)p0;
  char* s1 = (char*)p1;
  int l2 = l + l0 - n;
  ASSERT(l2 < l1);
  if ((l + l0 - n) < l1) {
    if (s1 != s) {
      memcpy(s1, s, i);
    }
    memmove(s1 + i + l0, s + i + n, l - (i + n));
    memcpy(s1 + i, s0, l0);
  }
  return l2;
}
CV_INLINE int cstr_del(char* p, int l, int i, int n)
{
  CSTRINITLEN(p, l);
  ASSERT((i + n) < l);
  memcpy(p + i, p + i + n, l - (i + n));
  l -= n;
  p[l] = 0;
  return l;
}
CV_INLINE int cstr_ins(const char* p, int l, int i, int n, const char* p0, int l0, char* p1, int l1)
{
  int l2;
  CSTRINITLEN(p, l);
  CSTRINITLEN(p0, l0);
  l2 = memins(p, l, i, n, p0, l0, p1, l1);
  p1[l2] = 0;
  return l2;
}
CV_INLINE int cstr_ins_c(const char* p, int l, int i, int n, int ch, char* p1, int l1)
{
  char p0[4] = {ch, 0, 0};
  return cstr_ins(p, l, i, n, p0, 1, p1, l1);
}
CV_INLINE int cstr_rev(char* s, int l)
{
  int i;
  char t;
  CSTRINITLEN(s, l);
  for (i = 0; i < l / 2; ++i) {
    SWAP(s[i], s[l - 1 - i], t);
  }
  return 0;
}
CV_INLINE int int2abc(int nNo, char* szTxt)
{
  int i = 0;
  int ndiv = nNo;
  do {
    szTxt[i++] = (char)((ndiv % 26) + 'A');
    ndiv /= 26;
  }
  while (ndiv != 0);
  szTxt[i] = 0;
  cstr_rev(szTxt, i);
  return 0;
}
CV_INLINE BOOL abc2int(const char* lpszAbc, int len, int* pn)
{
  int i, n, nttt;
  char szt[256] = {0};
  memcpy(szt, lpszAbc, len);
  if (0 == len) {
    return FALSE;
  }
  for (i = 0; i < len; i++) {
    if ((szt[i] < 'A') || (szt[i] > 'Z')) {
      return FALSE;
    }
  }
  n = 0;
  nttt = 1;
  for (i = len; i >= 0; --i) {
    n += (int)(szt[i - 1] - 'A') * nttt;
    nttt *= 26;
  }
  *pn = n;
  return TRUE;
}
#define SKIPSPACE(mm)  for (; mm->pos<mm->l && (' '>=mm->s[mm->pos] && mm->s[mm->pos]>0); mm->pos++) {}
#define mystrcmp(s1, l1, s2, l2) (((l1)==(l2))?str_ncmp((const char*)s1, (const char*)s2, l2):XCMP((l1),(l2)))
#define mystricmp(s1, l1, s2, l2) (((l1)==(l2))?str_nicmp(s1, s2, l2):XCMP((l1),(l2)))
//#define memcmp mem_cmp
CV_INLINE int mem_cmp(const void* pbuf1, const void* pbuf2, unsigned count)
{
  const char* buf1 = (const char*)pbuf1;
  const char* buf2 = (const char*)pbuf2;
  if (!count) {
    return(0);
  }
  while (--count && *buf1 == *buf2) {
    buf1++;
    buf2++;
  }
  return(*buf1 - *buf2);
}
CV_INLINE int mem_icmp(const char* s1, const char* s2, int n, int ignore_case)
{
  static uchar map[256] = {0};
  static int inited = 0;
  if (!inited) {
    int i;
    for (i = 0; i < 256; ++i) {
      map[i] = ('A' <= i && i <= 'Z') ? (i - 'A' + 'a') : i;
    }
  }
  if (ignore_case) {
    while (--n && map[(uchar)*s1] == map[(uchar)*s2]) {
      s1++;
      s2++;
    }
    return(map[(uchar) * s1] - map[(uchar) * s2]);
  }
  while (--n && *s1 == *s2) {
    s1++;
    s2++;
  }
  return(*s1 - *s2);
}
//#define strncmp str_ncmp
#define cstr_cmp(s1, s2, ignore_case) cstr_nicmp_((const char*)(s1).s, (s1).l, (const char*)(s2).s, (s2).l, -1, 0, ignore_case)
#define cstr_nicmp(s1, l1, s2, l2, n, ignore_case)  cstr_nicmp_(s1, l1, s2, l2, n, 0, ignore_case)
#define cstr_ncmp(s1, l1, s2, l2, n)  cstr_nicmp_(s1, l1, s2, l2, n, 0, 0)
#define cstr_icmp(s1, l1, s2, l2, ignore_case)  cstr_nicmp_(s1, l1, s2, l2, -1, 0, ignore_case)
#define cstr_endwith(s1, l1, s2, l2, ignore_case)  (0==cstr_nicmp_(s1, l1, s2, l2, -1, 1, ignore_case))
CV_INLINE int cstr_nicmp_(const char* s1, int l1, const char* s2, int l2, int n, int _isend, int ignore_case)
{
  CSTRINITLEN(s1, l1);
  CSTRINITLEN(s2, l2);
  if (n < 0) {
    if (_isend) {
      if (l1 < l2) {
        return -1;
      }
      n = l2;
    }
    else {
      if (l1 != l2) {
        return XCMP(l1, l2);
      }
      n = l2;
    }
  }
  else {
    n = MIN(n, l1);
    n = MIN(n, l2);
  }
  if (!n) {
    return(0);
  }
  if (_isend) {
    s1 += l1 - n;
    s2 += l2 - n;
  }
  return mem_icmp(s1, s2, n, ignore_case);
}
CV_INLINE char* cstr_chr(const char* s, int l, int ch)
{
  int ulen = CSTRINITLEN(s, l);
  const char* e = s + l;
  for (; s < e; ++s) {
    if (*s == ch) {
      return (char*)s;
    }
  }
  return 0;
}
CV_INLINE char* cstr_rchr(const char* s, int l, int ch)
{
  int ulen = CSTRINITLEN(s, l);
  const char* e = s + l;
  for (; s < e--;) {
    if (*e == ch) {
      return (char*)e;
    }
  }
  return 0;
}
CV_INLINE int cstr_findchr(const char* s, int l, int ch, int i)
{
  CSTRINITLEN(s, l);
  ASSERT(i<=l);
  for (; i < l; ++i) {
    if (s[i] == ch) {
      return i;
    }
  }
  return -1;
}
CV_INLINE int cstr_rfindchr(const char* s, int l, int ch, int i)
{
  CSTRINITLEN(s, l);
  ASSERT(i<=l);
  if (i<=0) {
    i = l;
  }
  for (; --i;) {
    if (s[i] == ch) {
      return i;
    }
  }
  return -1;
}
typedef uchar uchar256[256];
typedef char char256[256];
CV_INLINE int cstr_initmap(uchar* usmap, const char* s, int l) {
  int i;
  uchar* us = (uchar*)s;
  CSTRINITLEN(s, l);
  for (i=0; i<l; ++i) {
    usmap[us[i]] = 1;
  }
  return 0;
}
CV_INLINE int cstr_findchrs(const char* s, int l, const uchar* usmap1, int i) {
  uchar* us = (uchar*)s;
  CSTRINITLEN(s, l);
  ASSERT(i<=l);
  if (i<=0) {
    i = l;
  }
  for (; --i;) {
    if (usmap1[us[i]]) {
      return i;
    }
  }
  return 0;
}
CV_INLINE int cstr_replacechr_cpy(char* s, int l, const char* s1, int l1, int c1, int c2, int onlyone) {
  int i, t=0, j=0, ch;
  CSTRINITLEN(s1, l1);
  if (onlyone) {
    for (i=0; i<l1; ++i) {
      ch = s1[i];
      if (ch==c1) {
        if (0==t) {
          s[j++] = c2;
        }
        t = 1;
      } else {
        t = 0;
        s[j++] = ch;
      }
    }
  } else {
    for (i=0; i<l1; ++i) {
      ch = s1[i];
      s[i] = (ch==c1) ? c2 : ch;
    }
  }
  s[l1] = 0;
  return l1;
}
CV_INLINE int cstr_replacechrs_cpy(char* s, int l, const char* s1, int l1, const uchar* umap, int c2, int onlyone) {
  uchar* us1 = (uchar*)s1;
  int i, t=0, j=0, ch;
  CSTRINITLEN(s1, l1);
  if (onlyone) {
    for (i=0; i<l1; ++i) {
      ch = us1[i];
      if (umap[ch]) {
        if (0==t) {
          s[j++] = c2;
        }
        t = 1;
      } else {
        t = 0;
        s[j++] = c2;
      }
    }
  } else {
    for (i=0; i<l1; ++i) {
      ch = s1[i];
      s[i] = umap[ch] ? c2 : ch;
    }
    j = i;
  }
  s[j] = 0;
  return j;
}
CV_INLINE int cstr_countchr(const char* s, int l, int ch)
{
  int i, j = 0;
  CSTRINITLEN(s, l);
  for (i = 0; i < l; ++i) {
    if (s[i] == ch) {
      j++;
    }
  }
  return j;
}
CV_INLINE int cstr_replacechr(char* s, int l, int ch1, int ch2)
{
  int i, j = 0;
  CSTRINIT(s, l);
  for (i = 0; i < l; ++i) {
    if (us[i] == ch1) {
      us[i] = (uchar)ch2;
      j++;
    }
  }
  return j;
}
CV_INLINE char* cstr_chrs(const char* s, int l, const char* chs)
{
  int i;
  const uchar* us = (const uchar*)s;
  const uchar* uchs = (const uchar*)chs;
  uchar map[256] = {0};
  for (i = 0; uchs[i]; ++i) {
    map[uchs[i]] = 1;
  }
  CSTRINITLEN(s, l);
  for (i = 0; i < l; ++i) {
    if (uchs[us[i]]) {
      return (char*)(s + i);
    }
  }
  return 0;
}
CV_INLINE char* cstr_rchrs(const char* s, int l, const char* chs)
{
  int i;
  const uchar* us = (const uchar*)s;
  const uchar* uchs = (const uchar*)chs;
  uchar map[256] = {0};
  for (i = 0; uchs[i]; ++i) {
    map[uchs[i]] = 1;
  }
  CSTRINITLEN(s, l);
  for (i = l - 1; i >= 0; --i) {
    if (uchs[us[i]]) {
      return (char*)(s + i);
    }
  }
  return 0;
}
#define str_find_str(s1, s2, i, ignore_case)  cstr_find((s1).s, (s1).l, (s2).s, (s2).l, i, ignore_case)
CV_INLINE int cstr_find(const char* s1, int l1, const char* s2, int l2, int i, int ignore_case)
{
  CSTRINITLEN(s1, l1);
  CSTRINITLEN(s2, l2);
  if (l2 > 0) {
    i = i < 0 ? (l1 + i) : i;
    for (; i <= l1 - l2; ++i) {
      if (0 == mem_icmp(s1 + i, s2, l2, ignore_case)) {
        return i;
      }
    }
  }
  return -1;
}
#define str_rfind_str(s1, s2, i, ignore_case)  cstr_rfind((s1).s, (s1).l, (s2).s, (s2).l, i, ignore_case)
CV_INLINE int cstr_rfind(const char* s1, int l1, const char* s2, int l2, int i, int ignore_case)
{
  CSTRINITLEN(s1, l1);
  CSTRINITLEN(s2, l2);
  if (l2 > 0) {
    i = i ? i : (l1 - l2);
    i = i < 0 ? (l1 + i) : i;
    for (i = l1 - l2; i >= 0; --i) {
      if (0 == mem_icmp(s1 + i, s2, l2, ignore_case)) {
        return i;
      }
    }
  }
  return -1;
}
CV_INLINE int cstr_count(const char* s1, int l1, const char* s2, int l2, int ignore_case)
{
  int i, j = 0;
  CSTRINITLEN(s1, l1);
  CSTRINITLEN(s2, l2);
  for (i = 0; i <= l1 - l2;) {
    if (0 == mem_icmp(s1 + i, s2, l2, ignore_case)) {
      ++j;
      i += l2;
    }
    else {
      ++i;
    }
  }
  return j;
}
static int cstr_removechr(char* s, int l, int ch)
{
  int i = 0, j = 0;
  CSTRINITLEN(s, l);
  for (; i < l; ++i) {
    if (s[i] != ch) {
      s[j++] = s[i];
    }
  }
  s[j] = 0;
  return i - j;
}
CV_INLINE int cstr_replacestr(char* s1, int l1, const char* s2, int l2, const char* s3, int l3, int ignore_case, int pl1, int* pj) {
  int i, j = 0, m, ret=l1;
  CSTRINITLEN(s1, l1);
  CSTRINITLEN(s2, l2);
  CSTRINITLEN(s3, l3);
  if (l2 >= l3) {
    for (i = 0, m = 0; i <= l1 - l2;) {
      if (0 == mem_icmp(s1 + i, s2, l2, ignore_case)) {
        memcpy(s1 + m, s3, l3);
        m += l3;
        ++j;
        i += l2;
      }
      else {
        s1[m++] = s1[i++];
      }
    }
    for (; i < l1;) {
      s1[m++] = s1[i++];
    }
    ret = (l1 + j * (l3 - l2));
  } else {
    if (pl1<0) {
      j = cstr_count(s1, l1, s2, l2, ignore_case);
      ret = l1 + j * (l3 - l3);
    }
    if (s3) {
      for (i = l1, m = pl1; i >= l2;) {
        if (0 == mem_icmp(s1 + i - l2, s2, l2, ignore_case)) {
          m -= l3;
          memcpy(s1 + m, s3, l3);
          i -= l2;
          ++j;
        }
        else {
          s1[--m] = s1[--i];
        }
      }
      for (; i > 0;) {
        s1[--m] = s1[--i];
      }
      ret = pl1;
    }
  }
  if (pj) {
    *pj = j;
  }
  return ret;
}
CV_INLINE int cstr_cpy(char* s0, int len, const char* s2, int l2)
{
  CSTRINITLEN(s2, l2);
  ASSERT(l2 < len);
  memcpy(s0, s2, l2);
  s0[l2] = 0;
  return l2;
}
#define cstr_cat1(s0, len, s1)  cstr_cat(s0, len, s1, -1)
CV_INLINE int cstr_cat(char* s0, int len, const char* s1, int l1)
{
  int l0 = cstr_len(s0);
  CSTRINITLEN(s1, l1);
  ASSERT(l0 + l1 < len);
  memcpy(s0 + l0, s1, l1);
  l0 += l1;
  s0[l0] = 0;
  return l0;
}
CV_INLINE int cstr_cat2(char* s0, int len, const char* s1, int l1, const char* s2, int l2)
{
  int l0 = cstr_len(s0);
  CSTRINITLEN(s1, l1);
  CSTRINITLEN(s2, l2);
  ASSERT((l0 + l1 + l2) < len);
  memcpy(s0 + l0, s1, l1);
  l0 += l1;
  memcpy(s0 + l0, s2, l2);
  l0 += l2;
  s0[l0] = 0;
  return l0;
}
CV_INLINE int cstr_cat3(char* s0, int len, const char* s1, int l1, const char* s2, int l2, const char* s3, int l3)
{
  int l0 = cstr_len(s0);
  CSTRINITLEN(s1, l1);
  CSTRINITLEN(s2, l2);
  CSTRINITLEN(s3, l3);
  ASSERT((l0 + l1 + l2 + l3) < len);
  memcpy(s0 + l0, s1, l1);
  l0 += l1;
  memcpy(s0 + l0, s2, l2);
  l0 += l2;
  memcpy(s0 + l0, s3, l3);
  l0 += l3;
  s0[l0] = 0;
  return l0;
}
CV_INLINE int cstr_cat_c(char* s0, int len, int ch)
{
  int l0 = cstr_len(s0);
  ASSERT(l0 + 1 < len);
  s0[l0++] = ch;
  s0[l0] = 0;
  return l0;
}
CV_INLINE int _cstrmid0(const char* s0, int l0, int* beg, int* end)
{
  *beg = BOUND(*beg, 0, l0);
  if (*end < 0) {
    *end = l0;
  }
  else {
    *end = BOUND(*end, 0, l0);
  }
  return 0;
}
CV_INLINE int cstr_mid_cpy(const char* s0, int l0, int beg, int end, char* out, int len)
{
  int l2;
  CSTRINITLEN(s0, l0);
  _cstrmid0(s0, l0, &beg, &end);
  l2 = (end-beg);
  ASSERT(l2 < len);
  memcpy(out, s0+beg, l2);
  out[l2] = 0;
  return l2;
}
CV_INLINE str_t __cstrtrim_c(const char* s, int l, const uchar* spmap)
{
  const uchar* us = (const uchar*)s;
  const uchar* ue;
  CSTRINITLEN(s, l);
  ue = us + l;
  for (; us < ue && spmap[ue[-1]]; ue--);
  for (; us < ue && spmap[*us]; us++);
  return STR2((char*)us, ue - us);
}
CV_INLINE str_t _cstrtrim_c(const char* s, int l, const char* sp)
{
  const uchar* us = (const uchar*)s;
  const uchar* ue;
  const uchar* usp = (const uchar*)sp;
  uchar spmap[256] = {0};
  for (;*usp;) {
    spmap[*usp++] = 1;
  }
  CSTRINITLEN(s, l);
  ue = us + l;
  for (; us < ue && spmap[ue[-1]]; ue--);
  for (; us < ue && spmap[*us]; us++);
  return STR2((char*)us, ue - us);
}
CV_INLINE int cstr_trim(char* s1, int l1, const char* sp)
{
  str_t s;
  CSTRINITLEN(s1, l1);
  s = _cstrtrim_c(s1, l1, sp);
  memcpy(s1, s.s, s.l);
  s1[s.l] = 0;
  return s.l;
}
static char* strtrim(char* s, const char* sp)
{
  char* e = s + cstr_len(s);
  for (; s <= e--;) {
    if (strchr(sp, *e) == NULL) {
      e[1] = 0;
      break;
    }
  }
  return s;
}

// s="|str1|str2|str3|" 
// ignore_case  
CV_INLINE int cstr_splitfind(const char* s, int l, const char* s1, int l1, int ignore_case, int* ppos)
{
  int i, j = 0, first, delims;
  if (NULL == s1 || NULL == s) {
    return -1;
  }
  l = (l > 0) ? l : strlen(s);
  l1 = (l1 > 0) ? l1 : strlen(s1);
  ASSERT(l > 0);
  ASSERT(l1 > 0);
  delims = s[0];
  first = s1[0];
  for (i = 0; i < l - l1; ++i) {
    if (delims == s[i]) {
      if (delims == s[i + l1 + 1] && 0 == mem_icmp(s + i + 1, s1, l1, ignore_case)) {
        if (ppos) {
          *ppos = i + 1;
        }
        return j;
      }
      ++j;
    }
  }
  return -1;
}
CV_INLINE char* strdupcat(const char* s1, const char* s2)
{
  int s1len = strlen(s1), s2len = strlen(s2);
  char* s12 = (char*)malloc(s1len + s2len + 1);
  memcpy(s12, s1, s1len);
  memcpy(s12 + s1len, s2, s2len + 1);
  return s12;
}
static char* GetFileExt(const char* fn)
{
  char* pos = (char*)strrchr(fn, '.');
  static char null_char[] = "";
  if (pos != 0) {
    return pos + 1;
  }
  return null_char;
}
#define MAX_PATH 260
static char* GetFilePath(const char* fn)
{
  int i;
  static char szFilePath[MAX_PATH] = "";
  strcpy(szFilePath, fn);
  i = strlen(szFilePath);
  while (i > 0 && szFilePath[i] != '\\' && szFilePath[i] != '/') {
    i--;
  }
  if (i > 0) {
    szFilePath[i] = 0;
  }
  return (szFilePath);
}
static char* GetFileName(const char* fn)
{
  int i = strlen(fn);
  while (i > 0 && (fn[i] != '\\' && fn[i] != '/')) {
    i--;
  }
  if (i <= 0) {
    return ("");
  }
  return (char*)(fn + i + 1);
}
#define CSTR_LEFT        1<<1
#define CSTR_PLUS        1<<2
#define CSTR_SPACE       1<<3
#define CSTR_ZEROPAD     1<<4
#define CSTR_SMALL       1<<5
#define CSTR_SIGN        1<<6
#define CSTR_SPECIAL     1<<7
#define CSTR_CAPEXP      1<<8
#define is_digit(c)   ('0'<=(c) && (c)<='9')
CV_INLINE int skip_atoi(const char** pstr)
{
  int i = 0;
  const char* str = *pstr;
  while (is_digit(*str)) {
    i = i * 10 + (*str - '0');
    ++str;
  }
  *pstr = str;
  return i;
}
#define I2STR_IMPL(str, maxstrlen, n, radix, field_width, precision, flags) { \
    int i = 0, j, fillchr = 0, has_sign, buflen; \
    uchar buf[32] = {0}; \
    const char* intstr = NULL; \
    char ct, sign = 0; \
    fillchr = CSTR_ZEROPAD & flags ? '0' : ' '; \
    intstr = CSTR_SMALL & flags ? "0123456789abcdefghijklmnopqrstuvwxyz" : "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"; \
    if (10 == radix) { \
      if (n < 0) { sign = '-', un = -n; } \
      else if (CSTR_PLUS & flags && n) { sign = '+'; } \
    } \
    if (0 == un) { buf[i++] = intstr[0]; } \
    else { while (un) { r = un % radix;  un /= radix; buf[i++] = intstr[r]; } } \
    for (j = 0; j < i / 2; ++j) { CV_SWAP(buf[i - j - 1], buf[j], ct); } \
    buflen = i; has_sign = (sign != 0); \
    fwidth = MAX(field_width, buflen + has_sign); \
    fwidth = MIN(fwidth, maxstrlen - 1); \
    buflen = MIN(buflen, fwidth - has_sign); \
    if (str) { \
      int filllen = fwidth - buflen - has_sign; \
      j = 0; \
      if (CSTR_LEFT & flags) { \
        if (sign) { str[j++] = sign; } \
        memcpy(str + j, buf, buflen), j += buflen; \
        memset(str + j, ' ', filllen); \
      } else { \
        if (CSTR_ZEROPAD & flags) { \
          if (sign) { str[j++] = sign; } \
          memset(str + j, fillchr, fwidth - buflen - has_sign); \
          j += filllen; \
        } else { \
          memset(str + j, fillchr, fwidth - buflen - has_sign); \
          j += filllen; \
          if (sign) { str[j++] = sign; } \
        } \
        memcpy(str + j, buf, buflen); \
      } \
      str[fwidth] = '\0'; \
    } \
  }
CV_INLINE int i2str(char* str, int maxstrlen, int n, uint radix, int field_width, int precision, int flags)
{
  int fwidth = 0;
  int r;
  unsigned int un = n;
  I2STR_IMPL(str, maxstrlen, n, radix, field_width, precision, flags);
  return fwidth;
}
CV_INLINE int i2str64(char* str, int maxstrlen, int64 n, uint radix, int field_width, int precision, int flags)
{
  int fwidth = 0;
  int64 r;
  uint64 un = n;
  I2STR_IMPL(str, maxstrlen, n, radix, field_width, precision, flags);
  return fwidth;
}

#define IEEE754_FL_E  8
#define IEEE754_FL_T  (31-IEEE754_FL_E)
#define IEEE754_DB_E  11
#define IEEE754_DB_T  (63-IEEE754_DB_E)
#define IEEE754_FL_G_S(x) ((x)>>31)
#define IEEE754_FL_G_F(x) ((x)&((1<<23)-1))
#define IEEE754_FL_G_E(x) ((x>>23)&((1<<8)-1))
typedef union tfl {
  float f;
  int i;
  uchar c[4];
  struct {
uint f:
    IEEE754_FL_T;
uint e:
    IEEE754_FL_E;
    uint s: 1;
  } w;
} tfl;
typedef union tdb {
  double f;
  int64 i;
  uchar c[8];
  struct {
uint64 f:
    IEEE754_DB_T;
uint64 e:
    IEEE754_DB_E;
    uint64 s: 1;
  } w;
} tdb;
CV_INLINE int float_fmt(char* str, int maxstrlen, int sign, int expo, uint64 frac, uint radix, int field_width, int precision, int flags)
{
  char buf[128] = {0};
  const char* intstr = "0123456789abcdefghijklmnopqrstuvwxyz";
  const uint64 fracmax = ((uint64) - 1) / radix;
  const uint radix_2 = radix >> 1;
  uint64 t;
  int i = 0, j, fwidth = field_width;
  int idot = 0, fzeros = 0; // 锟斤拷锟斤拷锟斤拷锟斤拷位锟斤拷=idot, 小锟斤拷锟斤拷锟斤拷0锟侥革拷锟斤拷
  int fillchr = CSTR_ZEROPAD & flags ? '0' : ' ';
  int nzero = 0;
  if (sign) {
    *str++ = '-';
    --maxstrlen;
  }
  maxstrlen = MIN(maxstrlen, 20);
  //ultoa(frac, buf1, 2);
  while (expo < 0) {
    frac <<= 1;
    expo++;
    while (frac > fracmax) {
      frac = (frac + radix_2) / radix;
      --nzero;
    }
  }
  for (; (expo >= 64) || (0 == (frac >> expo));) {
    while (frac > fracmax) {
      frac >>= 1;
      --expo;
    }
    frac *= radix;
    ++nzero;
  }
  idot = i;
  fwidth = MAX(field_width, idot + precision + 1);
  if (frac || precision) {
    //buf[i++] = '.';
  }
  for (; frac && i < maxstrlen;) {
    while (frac > fracmax) {
      frac >>= 1;
      --expo;
    }
    frac *= radix;
    ++nzero;
    t = (expo < 64) ? (frac >> expo) : 0;
    if (t) {
      int i0 = i, ct;
      for (; t; ++i) {
        int tr = (int)(t % radix);
        buf[i] = intstr[tr];
        t /= radix;
      }
      frac &= ((uint64)1 << expo) - 1;
      //memrev(buf+i0, 1, i-i0);
      ARRREV(i - i0, buf + i0, ct);
    }
    else {
      buf[i++] = '0';
    }
    if ('0' < buf[i] && 0 == fzeros) {
      fzeros = i;
    }
  }
  for (; i > 0 && nzero > 0 && '0' == buf[i - 1];) {
    --i;
    --nzero;
  }
  buf[i] = 0;
  if ((nzero - i) > 2 || nzero < 0) {
    flags |= CSTR_CAPEXP;
    //precision-=2;    fwidth = MAX(field_width, idot + precision + 1);
  }
  if ((CSTR_CAPEXP & flags)) {
    int u = i - nzero - 1;
    memcpy(buf + 2, buf + 1, i - 1);
    buf[1] = '.';
    i = MIN(i, precision + 2);
    buf[i++] = 'e';
    i2str(buf + i, countof(buf) - i, u, 10, 4, 0, CSTR_ZEROPAD | CSTR_PLUS);
    i += 4;
    buf[i] = 0;
    idot = 1;
    fwidth = MAX(field_width, i);
  }
  else {
    int u = i - nzero;
    if (u <= 0) {
      memcpy(buf - u + 2, buf, i);
      memset(buf, '0', -u + 2);
      i += -u + 2;
      u = 1;
    }
    else {
      memcpy(buf + u + 1, buf + u, nzero);
      ++i;
    }
    buf[u] = '.';
  }
  //fwidth = field_width>0 ? MIN(fwidth, maxstrlen - 1) : 0;
  //precision = MIN(precision, i-idot);
  j = MIN(maxstrlen-1, i);
  fwidth = BOUND(field_width, j, maxstrlen - 1);
  if (CSTR_LEFT & flags) {
    memcpy(str, buf, j);
    memset(str + j, fillchr, fwidth - j);
  }
  else {
    if (sign) {
      --str;
      memset(str, ' ', fwidth - j);
      str[fwidth - j] = '-';
      memcpy(str + fwidth - j + 1, buf, j);
      fwidth++;
    }
    else {
      memset(str, ' ', fwidth - j);
      memcpy(str + fwidth - j, buf, j);
    }
  }
  if (str[fwidth-1]=='.') {
    --fwidth;
  }
  str[fwidth] = '\0';
  return fwidth;
}
CV_INLINE int d2str1(char* str, int maxstrlen, double fnum, uint radix, int field_width, int precision, int flags)
{
  int u;
  const char* intstr = "0123456789abcdefghijklmn0pqrstuvwxyz";
  int fillchr = CSTR_ZEROPAD & flags ? '0' : ' ';
  int i = 0, j = 0, idot = 0;
  char buf[256] = {0}, ct;
  while (fnum > 1) {
    u = (int)fmod(fnum, radix);
    fnum /= radix;
    buf[i++] = intstr[u];
  }
  for (j = 0; j < i / 2; ++j) {
    CV_SWAP(buf[i - j - 1], buf[j], ct);
  }
  idot = i;
  if (precision) {
    buf[i++] = '.';
  }
  field_width = MAX(field_width, idot + precision);
  j = MIN(field_width, i);
  if (i > j) {
    buf[j - 1] += buf[j] > '4';
  }
  if (CSTR_LEFT & flags) {
    memcpy(str, buf, j);
    memset(str + j, fillchr, field_width - j);
  }
  else {
    memset(str, ' ', field_width - j);
    memcpy(str + field_width - j, buf, j);
  }
  str[field_width] = '\0';
  return field_width;
}
CV_INLINE int d2str(char* str, int maxstrlen, double fnum, uint radix, int field_width, int precision, int flags)
{
  tdb tf;
  uint64 u = 0;
  enum { maxe = (1 << (IEEE754_DB_E)) - 1, maxe_2 = maxe >> 1 };
  int j = 0, e = IEEE754_DB_T, e1;
  //fnum = 0.021199700045797021;
  tf.f = fnum;
  if (0 == tf.w.f && 0 == tf.w.e) {
    if (str && maxstrlen >= 2) {
      if (tf.w.s) {
        str[j++] = tf.w.s ? '-' : '+';
      }
      str[j++] = '0';
      str[j++] = 0;
    }
  }
  else if (0 == tf.w.f && maxe == (int)tf.w.e) {
    if (str && maxstrlen >= 2) {
      if (tf.w.s) {
        str[j++] = tf.w.s ? '-' : '+';
      }
      str[j++] = 'i';
      str[j++] = 'n';
      str[j++] = 'f';
      str[j++] = 0;
    }
  }
  else if (0 == tf.w.f && maxe == (int)tf.w.e) {
    if (str && maxstrlen >= 2) {
      if (tf.w.s) {
        str[j++] = tf.w.s ? '-' : '+';
      }
      str[j++] = '1';
      str[j++] = 0;
    }
  }
  else {
    u = tf.w.f | ((uint64)1 << e);
    e1 = maxe_2 - (int)tf.w.e;
    e += e1;
    j = float_fmt(str, maxstrlen, (int)tf.w.s, e, u, radix, field_width, precision, flags);
  }
  return j;
}
CV_INLINE int f2str(char* str, int maxstrlen, float fnum, uint radix, int field_width, int precision, int flags)
{
  tfl tf;
  uint u = 0;
  enum { maxe = (1 << (IEEE754_FL_E)) - 1, maxe_2 = maxe >> 1 };
  int i = 0, j = 0, e = IEEE754_FL_T, e1;
  tf.f = fnum;
  if (0 == tf.w.f && 0 == tf.w.e) {
    if (str && maxstrlen >= 2) {
      str[j++] = '0';
      str[j++] = 0;
    }
  }
  else if (0 == tf.w.f && maxe == (int)tf.w.e) {
    if (str && maxstrlen >= 2) {
      str[j++] = tf.w.s ? '-' : '+';
      str[j++] = 'i';
      str[j++] = 'n';
      str[j++] = 'f';
      str[j++] = 0;
    }
  }
  else {
    u = tf.w.f | (1 << e);
    e1 = maxe_2 - tf.w.e;
    e += e1;
    j = float_fmt(str, maxstrlen, (int)tf.w.s, e, u, radix, field_width, precision, flags);
  }
  return j;
}
CV_INLINE int cstr_vnprintf(char* buf, int buflen, const char* fmt, char* args, char** pargs)
{
  int len, i, j = 0;
  int flags, bits = 32, field_width, is_complex = 0;
  uchar flagsmap[128] = {0};
  uchar* s;
  int* ip;
  /* width of output field */
  int precision;        /* min. # of digits for integers; max
  i2str of chars for from string */
  int qualifier;        /* 'h', 'l', or 'L' for integer fields */
  flagsmap['-'] = CSTR_LEFT;
  flagsmap['+'] = CSTR_PLUS;
  flagsmap[' '] = CSTR_SPACE;
  //flagsmap['#'] = CSTR_SPECIAL;
  flagsmap['0'] = CSTR_ZEROPAD;
  for (; *fmt ; ++fmt) {
    if (*fmt != '%') {
      buf[j++] = *fmt;
      continue;
    }
    flags = 0;
    while (flagsmap[*++fmt]) {
      flags |= flagsmap[*fmt];
      //fmt指锟斤拷%锟侥猴拷一位
    }
    /* get field width */
    field_width = -1;
    len = -1;
    if (is_digit(*fmt)) {
      field_width = skip_atoi(&fmt);
    }
    else if ('*' == *fmt) {
      /* it's the next argument */
      field_width = myva_arg(args, int);
      if (field_width < 0) {
        field_width = -field_width;
        flags |= CSTR_LEFT;
      }
      ++fmt;
    }
    else if ('#' == *fmt) {
      len = myva_arg(args, int);
      ++fmt;
    }
    /* get the precision */
    precision = -1;
    if (*fmt == '.') {
      ++fmt;
      if (is_digit(*fmt)) {
        precision = skip_atoi(&fmt);
      }
      else if (*fmt == '*') {
        /* it's the next argument */
        precision = myva_arg(args, int);
      }
      if (precision < 0) {
        precision = 0;
      }
    }
    // %I32d
    bits = 0;
    if ('I' == *fmt || 'J' == *fmt) {
      is_complex = 'J' == *fmt;
      ++fmt;
      if ('6' == fmt[0] && '4' == fmt[1]) {
        bits = 64;
        fmt += 2;
      }
      else if ('3' == fmt[0] && '2' == fmt[1]) {
        bits = 32;
        fmt += 2;
      }
      else if ('1' == fmt[0] && '6' == fmt[1]) {
        bits = 16;
        fmt += 2;
      }
      else if ('8' == fmt[0]) {
        bits = 8;
        fmt += 1;
      }
      else {
        ASSERT(0 && "bad bits");
      }
    }
    /* get the conversion qualifier */
    qualifier = -1;
    if (*fmt == 'h' || *fmt == 'l' || *fmt == 'L') {
      qualifier = *fmt;
      ++fmt;
    }
    if (NULL == buf) {
      switch (*fmt) {
      case 'c':
        j += field_width + 1;
        break;
      case 's':
        s = myva_arg(args, uchar*);
        len = len < 0 ? cstr_len(s) : len;
        j += MAX(len, field_width);
        break;
      case 'o':
      case 'p':
      case 'x':
      case 'X':
      case 'd':
      case 'i':
      case 'u':
        j += MAX(10, field_width);
        break;
      case ('e'):
      case ('E'):
      case ('G'):
      case ('f'):
      case ('g'):
        j += MAX(32, field_width);
        break;
      default:
        break;
      }
      continue;
    }
    switch (*fmt) {
    case 'c':
      if (!(flags & CSTR_LEFT)) {
        while (--field_width > 0) {
          buf[j++] = ' ';
        }
      }
      buf[j++] = (unsigned char) myva_arg(args, int);
      while (--field_width > 0) {
        buf[j++] = ' ';
      }
      break;
    case 's':
      s = myva_arg(args, uchar*);
      len = len < 0 ? cstr_len(s) : len;
      if (precision < 0) {
        precision = len;
      }
      else if (len > precision) {
        len = precision;
      }
      len = MIN(len, buflen - 1);
      field_width = MIN(field_width, buflen - 1);
      if (!(flags & CSTR_LEFT)) {
        while (len < field_width--) {
          buf[j++] = ' ';
        }
      }
      for (i = 0; i < len; ++i) {
        buf[j++] = *s++;
      }
      while (len < field_width--) {
        buf[j++] = ' ';
      }
      break;
#define va_argt(ap,t)    ( *(t *)((ap += sizeof(t)) - sizeof(t)) )
    case 'd':                                    //锟斤拷锟斤拷锟斤拷锟斤拷停锟斤拷锟斤拷锟斤拷瓒╢lags锟斤拷然锟斤拷锟斤拷锟斤拷i2str锟斤拷锟斤拷锟斤拷锟缴憋拷锟斤拷锟饺★拷锟斤拷锟斤拷锟斤拷锟斤拷锟絙ase(锟斤拷锟斤拷锟斤拷10)然锟斤拷转锟斤拷锟斤拷锟街凤拷锟斤拷锟斤拷锟斤拷锟斤拷str
    case 'i':
      flags |= CSTR_SIGN;
      j += i2str64(buf + j, buflen - j, ((64 == bits) ? va_argt(args, int64) : (32 == bits) ? va_argt(args, int32) : (16 == bits) ? va_argt(args, int16) : (8 == bits) ? va_argt(args, int8) : va_argt(args, int32)), 10, field_width, precision, flags);
      break;
#define TGETUINT(base)  j += i2str64(buf + j, buflen - j, ((64==bits) ? va_argt(args, uint64) : (32==bits) ? va_argt(args, uint32) : (16==bits) ? va_argt(args, uint16) : (8==bits) ? va_argt(args, uint8) : va_argt(args, uint32)), base, field_width, precision, flags);
    case 'o':
      TGETUINT(8);
      break;
    case 'u':
      TGETUINT(10);
      break;
    case 'x':
      flags |= CSTR_SMALL;
    case 'X':
      TGETUINT(16);
      break;
#undef TGETUINT
    case 'p':
      if (field_width == -1) {
        field_width = 8;
        flags |= CSTR_ZEROPAD;
      }
      j += i2str(buf + j, buflen - j, (unsigned long) myva_arg(args, void*), 16, field_width, precision, flags);
      break;
    case 'n':
      ip = myva_arg(args, int*);
      *ip = j;
      break;
    case ('e'):
      flags |= CSTR_SMALL;
    case ('E'):
      flags |= CSTR_CAPEXP;
    case ('G'):
    case ('f'):
    case ('g'):
      if (precision < 0) {
        precision = 6;
      }
      if (32 == bits) {
        double d = (double)(myva_arg(args, float));
        j += f2str(buf + j, buflen - j, d, 10, field_width, precision, flags);
      }
      else {
        double d = myva_arg(args, double);
        j += d2str(buf + j, buflen - j, d, 10, field_width, precision, flags);
        //fcvt(d, field_width)
      }
      break;
    case ('z'): {
      typedef int (*xtoa_t)(char * buf, int len, void * x);
      xtoa_t xtoa = myva_arg(args, xtoa_t);
      void* xptr = myva_arg(args, void*);
      j += xtoa(buf + j, buflen - j, xptr);
    }
    break;
    default:
      if (*fmt != '%') {
        buf[j++] = '%';
      }
      if (*fmt) {
        buf[j++] = *fmt;
      }
      else {
        --fmt;   //
      }
      break;
    }
  }
  buf[j] = '\0';
  if (pargs) {
    *pargs = args;
  }
  return j; //
}
CV_INLINE int cstr_nprintf(char* buf, int buflen, const char* fmt, ...)
{
  int len = 0;
  char* arglist;
  myva_start(arglist, fmt);
  len = cstr_vnprintf(buf, buflen, fmt, arglist, &arglist);
  myva_end(arglist);
  return len;
}

CV_INLINE int cstr_cat_nprintf(char* buf, int buflen, const char* fmt, ...) {
  int len = 0;
  char* arglist;
  int l = strlen(buf);
  myva_start(arglist, fmt);
  len = cstr_vnprintf(buf+l, buflen-l, fmt, arglist, &arglist);
  myva_end(arglist);
  return len;
}

CV_INLINE int cstr_cat_int32(char* buf, int buflen, int n) {
  int l = strlen(buf);
  l+=i2str(buf+l, buflen-l, n, 10, 0, 0, 0);
  return l;
}
#define str_printf_arr(fmt, n, arr, bufsz)  {char* buf = MALLOC(char, bufsz); MEMSET(buf, 0, bufsz); str_nprintf_arr(buf, bufsz, fmt, n, arr); printf(buf); FREE(buf);}
#define fprintf_arr(fn, fmt, n, arr, bufsz)  {FILE* pf = fopen(fn, "wb"); char* buf = MALLOC(char, bufsz); MEMSET(buf, 0, bufsz); str_nprintf_arr(buf, bufsz, fmt, n, arr); fprintf(pf, "%s", buf); FREE(buf); fclose(pf);}
CV_INLINE int cstr_nprintf_arr(char* buf, int buflen, const char* fmt, int n, void* arr)
{
  int i, len = 0;
  char* arglist = (char*)arr;
  for (i = 0; i < n && buflen > len + 1; ++i) {
    ASSERT(len < buflen);
    len += cstr_vnprintf(buf + len, buflen - len, fmt, arglist, &arglist);
  }
  return len;
}
CV_INLINE int cstr_nprintf_mat(char* buf, int buflen, const char* fmt, const char* endline, int m, int n, void* mat, int step)
{
  int j, len = 0;
  int endlinelen = cstr_len(endline);
  for (j = 0; j < m; ++j) {
    len += cstr_nprintf_arr(buf + len, buflen - len, fmt, n, (uchar*)mat + j * step);
    memcpy(buf + len, endline, endlinelen);
    len += endlinelen;
    buf[len] = 0;
  }
  return 0;
}
CV_INLINE int str_printf_mat(const char* fmt, int m, int n, void* mat, int step, int bufsz)
{
  char* buf = MALLOC(char, bufsz);
  MEMSET(buf, 0, bufsz);
  cstr_nprintf_mat(buf, bufsz, fmt, "\n", m, n, mat, step);
  puts(buf);
  FREE(buf);
  return 0;
}
#undef CSTR_LEFT
#undef CSTR_PLUS
#undef CSTR_SPACE
#undef CSTR_ZEROPAD
#undef CSTR_SMALL
#undef CSTR_SIGN
#undef CSTR_SPECIAL
#undef CSTR_CAPEXP
//static const char* logname(const char*) {}
static char* _logfilename()
{
  static char __logfilename[256] = ".\\log.txt";
  return __logfilename;
}
#define LOGFILENAME _logfilename()
static int fclear(const char* fname)
{
  FILE* pf = fopen(fname, "wb");
  if (pf) {
    fclose(pf);
    return 1;
  }
  return 0;
}
#define logclear()   fclear(LOGFILENAME)
#define logset(_name)   strcpy(LOGFILENAME, _name)
static int* get__logecho()
{
  static int __logecho = 1;
  return &__logecho;
}
static int* get__logtime()
{
  static int __logtime = 1;
  return &__logtime;
}
#define __logecho  (*(get__logecho()))
#define __logtime  (*(get__logtime()))
#define logecho(_ON_OFF)   (__logecho=_ON_OFF)
#define logtime(_ON_OFF)   (__logtime=_ON_OFF)
// pf - stdout?
#ifndef PTTIME
#include <time.h>
#include <stdarg.h>
//#define STRDATE()  _strdate(_time_buffer)
//#define STRTIME()  _strtime(_time_buffer)
//#define PTTIME STRTIME()
#endif
static int logprintf(const char* fmt, ...)
{
  FILE* pf;
  static char _time_buffer[ 16 ];
  pf = fopen(LOGFILENAME, "a");
  if (pf) {
    va_list ARGLIST;
    va_start(ARGLIST, fmt);
    if (__logtime) {
      //fprintf( pf, "%8s ", _strtime(_time_buffer) );
    }
    vfprintf(pf, fmt, ARGLIST);
    fflush(pf);
    if (__logecho) {
      vprintf(fmt, ARGLIST);
    }
    va_end(ARGLIST);
    fclose(pf);
  }
  return 0;
}
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
static int llogprintf(ELOG_LEVEL level, const char* fmt, ...)
{
  static bool showtime = false;
  static bool showecho = true;
  static loguser_t user = {0};
  static char _time_buffer[16];
  static char filename[64] = {0};
  static const char* levelstr[20] = {"DEBUG", "INFO", "WARNING", "ERROR", "NONE", "", "", "", "", ""};
  char buf[256];
  switch (level) {
  case ELL_SET_FILE:
    filename[0] = 0;
    if (fmt) {
      strncpy(filename, fmt, 64);
    }
    break;
  case ELL_SET_ECHO:
    showecho = fmt ? true : false;
    break;
  case ELL_SET_TIME:
    showtime = fmt ? true : false;
    break;
  case ELL_SET_USER:
    user = *(loguser_t*)fmt;
    break;
  default:
    if (1) {
      char* p = buf, *e = buf+256;
      va_list ARGLIST;
      va_start(ARGLIST, fmt);
      if (showtime) {
        _snprintf(p, e-p, "%8s ",  _strtime(_time_buffer));
        p += strlen(buf);
      }
      if (1) {
        _snprintf(p, e-p, "%8s: ", levelstr[level]);
        p += strlen(buf);
      }
      _vsnprintf(p, e-p, fmt, ARGLIST);
      va_end(ARGLIST);
      cstr_trim(buf, -1, "\r\n");
      if (filename[0]) {
        FILE* pf = fopen(filename, "a");
        if (pf) {
          fprintf(pf, "%s\n", buf);
          fclose(pf);
        }
      }
      if (showecho) {
        printf(fmt, ARGLIST);
      }
    }
    break;
  }
  return 0;
}
// Pattern Matching
// RMatch: recursively match s against pattern p, minplen
// is the min length string that can match p and
// numstars is the number of *'s in p
CV_INLINE BOOL RMatch(const char *s, const char *p, int slen, int minplen,int numstars)
{
   if (slen==0 && minplen==0)
      return TRUE;
   if (numstars==0 && minplen!=slen)
      return FALSE;
   if (minplen>slen)
      return FALSE;
   if (*p == '*') {
      return RMatch(s+1,p+1,slen-1,minplen,numstars-1) ||
         RMatch(s,p+1,slen,minplen,numstars-1) ||
         RMatch(s+1,p,slen-1,minplen,numstars);
   }
   if (*p == *s || *p == '?') {
      return RMatch(s+1,p+1,slen-1,minplen-1,numstars);
   } else {
      return FALSE;  
   }
}

/* EXPORT->DoMatch: return TRUE if s matches pattern p */
CV_INLINE BOOL DoMatch(const char *s, const char *p)
{
   int slen, minplen, numstars;
   const char *q;
   char c;
   
   slen = strlen(s);
   minplen = 0; numstars = 0; q = p;
   while ((c=*q++))
      if (c == '*') ++numstars; else ++minplen;
   return RMatch(s,p,slen,minplen,numstars);
}

/* SpRMatch: recursively match s against pattern p, minplen
   is the min length string that can match p and
   numstars is the number of *'s in p 
	   spkr is next character of the spkr name */
static BOOL SpRMatch(char *s,char *p,char *spkr,
			int slen,int minplen,int numstars)
{
   BOOL match;
   
   if (slen==0 && minplen==0)
      match=TRUE;
   else if ((numstars==0 && minplen!=slen) || minplen>slen)
      match=FALSE;
   else if (*p == '*') {
      match=(SpRMatch(s+1,p,spkr,slen-1,minplen,numstars) ||
	     SpRMatch(s,p+1,spkr,slen,minplen,numstars-1) ||
	     SpRMatch(s+1,p+1,spkr,slen-1,minplen,numstars-1));
   }
   else if (*p == '%') {
      *spkr=*s,spkr[1]=0;
      match=SpRMatch(s+1,p+1,spkr+1,slen-1,minplen-1,numstars);
      if (!match) *spkr=0;
   }
   else if (*p == *s || *p == '?')
      match=SpRMatch(s+1,p+1,spkr,slen-1,minplen-1,numstars);
   else
      match=FALSE;
   
   return(match);
}
#define MAXSTRLEN 256
/* EXPORT->MaskMatch: return spkr if s matches pattern p */
CV_INLINE BOOL MaskMatch(char *mask, char *spkr, char *str)
{
  int spkrlen, slen, minplen, numstars;
  char *q,c;
  
  if (mask == NULL || str==NULL) return(FALSE);
  slen = strlen(str);
  spkrlen = minplen = numstars = 0;
  q = mask;
  while ((c=*q++)) {
    if (c == '*') ++numstars; else ++minplen;
    if (c == '%') ++spkrlen;
  }
  if (spkrlen>=MAXSTRLEN) {
    printf("MaskMatch: Speaker name too long %d vs %d",spkrlen,MAXSTRLEN);
  }
  spkr[0]=0;
  if (SpRMatch(str,mask,spkr,slen,minplen,numstars)) {
    return(TRUE);
  }
  spkr[0]=0;
  return(FALSE);
}

#endif // _CSTD_H_
