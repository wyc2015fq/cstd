
/* The header is mostly for internal use and it is likely to change.
   It contains some macro definitions that are used in cxcore, cv, cvaux
   and, probably, other libraries. If you need some of this functionality,
   the safe way is to copy it into your code and rename the macros.
*/
#ifndef _CVTYPE_INL_
#define _CVTYPE_INL_

#include "cstd.h"
#include "cpu.h"
#include <limits.h>
#ifdef _OPENMP
#include "omp.h"
#endif


/************** interpolation formulas and tables ***************/
#if 0
enum {
  CC_INTER_NEAREST = CC_INTER_NN, //!< nearest neighbor interpolation
  CC_INTER_LINEAR = CC_INTER_LINEAR, //!< bilinear interpolation
  CC_INTER_CUBIC = CC_INTER_CUBIC, //!< bicubic interpolation
  CC_INTER_AREA = CC_INTER_AREA, //!< area-based (or super) interpolation
  CC_INTER_LANCZOS4 = CC_INTER_LANCZOS4, //!< Lanczos interpolation over 8x8 neighborhood
  CC_INTER_MAX = 7,
  CC_WARP_INVERSE_MAP = CC_WARP_INVERSE_MAP,
  CC_INTER_BITS = 5, CC_INTER_BITS2 = CC_INTER_BITS * 2,
  CC_INTER_TAB_SIZE = (1 << CC_INTER_BITS),
  CC_INTER_TAB_SIZE2 = CC_INTER_TAB_SIZE * CC_INTER_TAB_SIZE,
  CC_INTER_RESIZE_COEF_BITS = 11,
  CC_INTER_RESIZE_COEF_SCALE = 1 << CC_INTER_RESIZE_COEF_BITS,
  CC_INTER_REMAP_COEF_BITS = 15,
  CC_INTER_REMAP_COEF_SCALE = 1 << CC_INTER_REMAP_COEF_BITS
};
#endif

enum {
  CC_PROJ_SPHERICAL_ORTHO = 0,
  CC_PROJ_SPHERICAL_EQRECT = 1
};
//! class of the pixel in GrabCut algorithm
enum {
  CC_GC_BGD    = 0,  //!< background
  CC_GC_FGD    = 1,  //!< foreground
  CC_GC_PR_BGD = 2,  //!< most probably background
  CC_GC_PR_FGD = 3   //!< most probably foreground
};
//! GrabCut algorithm flags
enum {
  CC_GC_INIT_WITH_RECT  = 0,
  CC_GC_INIT_WITH_MASK  = 1,
  CC_GC_EVAL            = 2
};
enum {
  CC_DIST_LABEL_CCOMP = 0,
  CC_DIST_LABEL_PIXEL = 1
};

//                      Data Persistence                                    *
/********************************** High-level functions ********************************/
/* get index of the thread being executed */
//CVAPI( int ) cGetThreadNum( void );
//! type of the kernel
enum { CC_KERNEL_GENERAL = 0, CC_KERNEL_SYMMETRICAL = 1, CC_KERNEL_ASYMMETRICAL = 2,
    CC_KERNEL_SMOOTH = 4, CC_KERNEL_INTEGER = 8
     };
//! shape of the structuring element
enum { CC_MORPH_RECT = 0, CC_MORPH_CROSS = 1, CC_MORPH_ELLIPSE = 2 };
enum {
  CC_GHT_POSITION = 0,
  CC_GHT_SCALE = 1,
  CC_GHT_ROTATION = 2
};
//! type of morphological operation
enum { CC_MORPH_ERODE = CC_MOP_ERODE, CC_MORPH_DILATE = CC_MOP_DILATE,
    CC_MORPH_OPEN = CC_MOP_OPEN, CC_MORPH_CLOSE = CC_MOP_CLOSE,
    CC_MORPH_GRADIENT = CC_MOP_GRADIENT, CC_MORPH_TOPHAT = CC_MOP_TOPHAT,
    CC_MORPH_BLACKHAT = CC_MOP_BLACKHAT
     };

typedef struct CChain {
  char* code;
  int len;
  IPOINT origin;
}
CChain;
typedef struct CBox2D {
  FPOINT center; /* Center of the box. */
  CSize2D32f size; /* Box width and length. */
  float angle; /* Angle between the horizontal axis */
  /* and the first side (i.e. length) in degrees */
}
CBox2D;
/* Line iterator state: */
typedef struct CLineIterator {
  /* Pointer to the current point: */
  uchar* ptr;
  /* Bresenham algorithm state: */
  int err;
  int plus_delta;
  int minus_delta;
  int plus_step;
  int minus_step;
}
CLineIterator;

typedef struct CPyramid
{
    uchar **ptr;
    CSize *sz;
    double *rate;
    int *step;
    uchar *state;
    int level;
}
CPyramid;

static int cGetNumThreads() {
  return 1;
}
static int cGetThreadNum() {
  return 0;
}

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
//?��?????????????sin(x*PI)/(x*PI)??? //PI=3.1415926535897932385;
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
// ???��???????????
int threeorder(const unsigned char* in, int ix, int iy, int wl, int bi, unsigned char* out)
{
  int i, j, bii, u;
  double afu[ 4 ], afv[ 4 ];
  double sA = 0, aA = 0;
  double x = _SHIFT_TOFLOAT(ix);
  double y = _SHIFT_TOFLOAT(iy);
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

typedef CStatus (C_STDCALL *CvCopyMaskFunc)(const void* src, int src_step,void* dst, int dst_step, CSize size,const void* mask, int mask_step);
typedef CStatus (C_STDCALL *CvFunc2D_1A)(void* arr, int step, CSize size);
typedef CStatus (C_STDCALL *CvFunc2D_1A1P)(void* arr, int step, CSize size, void* param);
typedef CStatus (C_STDCALL *CvFunc2D_1A1P1I)(void* arr, int step, CSize size,void* param, int flag);
typedef CStatus (C_STDCALL *CvFunc2DnC_1A1P)( void* arr, int step, CSize size,int cn, int coi, void* param );
typedef CStatus (C_STDCALL *CvFunc2DnC_1A1P)( void* arr, int step, CSize size,int cn, int coi, void* param );
typedef CStatus (C_STDCALL *CvFunc2D_1A2P)( void* arr, int step, CSize size,void* param1, void* param2 );
typedef CStatus (C_STDCALL *CvFunc2DnC_1A2P)( void* arr, int step,CSize size, int cn, int coi,void* param1, void* param2 );
typedef CStatus (C_STDCALL *CvFunc2D_1A4P)( void* arr, int step, CSize size,void* param1, void* param2,void* param3, void* param4 );
typedef CStatus (C_STDCALL *CvFunc2DnC_1A4P)( void* arr, int step,CSize size, int cn, int coi,void* param1, void* param2,void* param3, void* param4 );
typedef CStatus (C_STDCALL *CvFunc2D_2A)( void* arr0, int step0,void* arr1, int step1, CSize size );typedef CStatus (C_STDCALL *CvFunc2D_2A1P)( void* arr0, int step0,void* arr1, int step1,CSize size, void* param );
typedef CStatus (C_STDCALL *CvFunc2DnC_2A1P)( void* arr0, int step0,void* arr1, int step1,CSize size, int cn,int coi, void* param );
typedef CStatus (C_STDCALL *CvFunc2DnC_2A1P)( void* arr0, int step0,void* arr1, int step1,CSize size, int cn,int coi, void* param );
typedef CStatus (C_STDCALL *CvFunc2D_2A2P)( void* arr0, int step0,void* arr1, int step1, CSize size,void* param1, void* param2 );
typedef CStatus (C_STDCALL *CvFunc2DnC_2A2P)( void* arr0, int step0,void* arr1, int step1,CSize size, int cn, int coi,void* param1, void* param2 );
typedef CStatus (C_STDCALL *CvFunc2D_2A1P1I)( void* arr0, int step0,void* arr1, int step1, CSize size,void* param, int flag );
typedef CStatus (C_STDCALL *CvFunc2D_2A4P)( void* arr0, int step0,void* arr1, int step1, CSize size,void* param1, void* param2,void* param3, void* param4 );
typedef CStatus (C_STDCALL *CvFunc2DnC_2A4P)( void* arr0, int step0,void* arr1, int step1, CSize size,int cn, int coi,void* param1, void* param2,void* param3, void* param4 );
typedef CStatus (C_STDCALL *CvFunc2D_3A)( void* arr0, int step0,void* arr1, int step1,void* arr2, int step2, CSize size );
typedef CStatus (C_STDCALL *CvFunc2D_3A1P)( void* arr0, int step0,void* arr1, int step1,void* arr2, int step2,CSize size, void* param );
typedef CStatus (C_STDCALL *CvFunc2D_3A1I)( void* arr0, int step0,void* arr1, int step1,void* arr2, int step2,CSize size, int flag );
typedef CStatus (C_STDCALL *CvFunc2DnC_3A1P)( void* arr0, int step0,void* arr1, int step1,void* arr2, int step2,CSize size, int cn,int coi, void* param );
typedef CStatus (C_STDCALL *CvFunc2D_4A)( void* arr0, int step0,void* arr1, int step1,void* arr2, int step2,void* arr3, int step3,CSize size );



/****************************************************************************************\
*                              Compile-time tuning parameters                            *
\****************************************************************************************/

/* maximal size of vector to run matrix operations on it inline (i.e. w/o ipp calls) */
#define CC_MAX_INLINE_MAT_OP_SIZE  10

/* maximal linear size of matrix to allocate it on stack. */
#define CC_MAX_LOCAL_MAT_SIZE  32

/* maximal size of local memory storage */
#define CC_MAX_LOCAL_SIZE  \
    (CC_MAX_LOCAL_MAT_SIZE*CC_MAX_LOCAL_MAT_SIZE*(int)sizeof(double))

/* default image row align (in bytes) */
#define CC_DEFAULT_IMAGE_ROW_ALIGN  4

/* matrices are continuous by default */
#define CC_DEFAULT_MAT_ROW_ALIGN  1

/* maximum size of dynamic memory buffer.
   cAlloc reports an error if a larger block is requested. */
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

/* maximum possible number of threads in parallel implementations */
#ifdef _OPENMP
#define CC_MAX_THREADS 128
#else
#define CC_MAX_THREADS 1
#endif

#if 0 /*def  CC_CHECK_FOR_NANS*/
    #define CC_CHECK_NANS(arr) cvCheckArray((arr))  
#else
    #define CC_CHECK_NANS(arr)
#endif

/****************************************************************************************\
*                                  Common declarations                                   *
\****************************************************************************************/

/* get alloca declaration */
#ifdef __GNUC__
    #undef alloca
    #define alloca __builtin_alloca
#elif defined WIN32 || defined WIN64
    #if defined _MSC_VER || defined __BORLANDC__
        #include <malloc.h>
    #endif
#elif defined HAVE_ALLOCA_H
    #include <alloca.h>
#elif defined HAVE_ALLOCA
    #include <stdlib.h>
#else
    //#error 
#endif

/* ! DO NOT make it an inline function */
//#define cvStackAlloc(size) cvAlignPtr(alloca((size) + CC_MALLOC_ALIGN), CC_MALLOC_ALIGN)

#if defined _MSC_VER || defined __BORLANDC__
    #define CC_BIG_INT(n)   n##I64
    #define CC_BIG_UINT(n)  n##UI64
#else
    #define CC_BIG_INT(n)   n##LL
    #define CC_BIG_UINT(n)  n##ULL
#endif


#if defined WIN32 && !defined WIN64 && (_MSC_VER >= 1200 || defined CC_ICC)
    #define CC_DBG_BREAK() __asm int 3
#else
    #define CC_DBG_BREAK() assert(0);
#endif

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

/* general-purpose saturation macros */ 
#undef CC_CAST_8U
#undef CC_CAST_8S
#undef CC_CAST_16U
#undef CC_CAST_16S
#define CC_CAST_8U(t)  (uchar)(!((t) & ~255) ? (t) : (t) > 0 ? 255 : 0)
#define CC_CAST_8S(t)  (char)(!(((t)+128) & ~255) ? (t) : (t) > 0 ? 127 : -128)
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

#define CC_DEFINE_MASK         \
    float maskTab[2]; maskTab[0] = 0.f; maskTab[1] = 1.f;
#define CC_ANDMASK(m, x)     ((x) & (((m) == 0) - 1))

/* (x) * ((m) == 1 ? 1.f : (m) == 0 ? 0.f : <ERR> */
#define CC_MULMASK(m, x)       (maskTab[(m) != 0]*(x))

/* (x) * ((m) == -1 ? 1.f : (m) == 0 ? 0.f : <ERR> */
#define CC_MULMASK1(m, x)      (maskTab[(m)+1]*(x))

#define CC_ZERO_OBJ(x)  memset((x), 0, sizeof(*(x)))

#define CC_DIM(static_array) ((int)(sizeof(static_array)/sizeof((static_array)[0])))

#define CC_UN_ENTRY_C1(worktype)   worktype s0 = scalar[0]
    
#define CC_UN_ENTRY_C2(worktype)   worktype s0 = scalar[0], s1 = scalar[1]

#define CC_UN_ENTRY_C3(worktype)   worktype s0 = scalar[0], s1 = scalar[1], s2 = scalar[2]

#define CC_UN_ENTRY_C4(worktype)   worktype s0 = scalar[0], s1 = scalar[1], s2 = scalar[2], s3 = scalar[3]

#define cvUnsupportedFormat "Unsupported format"

#define CC_DESCALE(x,n)     (((x) + (1 << ((n)-1))) >> (n))
#define CC_FLT_TO_FIX(x,n)  cRound((x)*(1<<(n)))

#if 0
/* This is a small engine for performing fast division of multiple numbers
   by the same constant. Most compilers do it too if they know the divisor value
   at compile-time. The algorithm was taken from Agner Fog's optimization guide
   at http://www.agner.org/assem */
typedef struct CvFastDiv
{
    unsigned delta, scale, divisor;
}
CvFastDiv;

#define CC_FAST_DIV_SHIFT 32

CC_INLINE CvFastDiv cvFastDiv(int divisor)
{
    CvFastDiv fastdiv;
    
    assert(divisor >= 1);
    uint64 temp = ((uint64)1 << CC_FAST_DIV_SHIFT)/divisor;

    fastdiv.divisor = divisor;
    fastdiv.delta = (unsigned)(((temp & 1) ^ 1) + divisor - 1);
    fastdiv.scale = (unsigned)((temp + 1) >> 1);
    
    return fastdiv;
}

#define CC_FAST_DIV(x, fastdiv)  \
    ((int)(((int64)((x)*2 + (int)(fastdiv).delta))*(int)(fastdiv).scale>>CC_FAST_DIV_SHIFT))

#define CC_FAST_UDIV(x, fastdiv)  \
    ((int)(((uint64)((x)*2 + (fastdiv).delta))*(fastdiv).scale>>CC_FAST_DIV_SHIFT))
#endif



// ***************************************************************************************\
//   
//   Generic implementation of QuickSort algorithm.
//   ----------------------------------------------
//   Using this macro user can declare customized sort function that can be much faster
//   than built-in qsort function because of lower overhead on elements
//   comparison and exchange. The macro takes less_than (or LT) argument - a macro or function
//   that takes 2 arguments returns non-zero if the first argument should be before the second
//   one in the sorted sequence and zero otherwise.
// 
//   Example:
// 
//     Suppose that the task is to sort points by ascending of y coordinates and if
//     y's are equal x's should ascend.
// 
//     The code is:
//     ------------------------------------------------------------------------------
//            #define cmp_pts(pt1, pt2) \
//                ((pt1).y < (pt2).y || ((pt1).y < (pt2).y && (pt1).x < (pt2).x))
// 
//            [static] CC_IMPLEMENT_QSORT(icvSortPoints, CPoint, cmp_pts)
//     ------------------------------------------------------------------------------
// 
//     After that the function "void icvSortPoints(CPoint* array, size_t total, int aux);"
//     is available to user.
// 
//   aux is an additional parameter, which can be used when comparing elements.
//   The current implementation was derived from *BSD system qsort():
// 
//     * Copyright (c) 1992, 1993
//     *  The Regents of the University of California.  All rights reserved.
//     *
//     * Redistribution and use in source and binary forms, with or without
//     * modification, are permitted provided that the following conditions
//     * are met:
//     * 1. Redistributions of source code must retain the above copyright
//     *    notice, this list of conditions and the following disclaimer.
//     * 2. Redistributions in binary form must reproduce the above copyright
//     *    notice, this list of conditions and the following disclaimer in the
//     *    documentation and/or other materials provided with the distribution.
//     * 3. All advertising materials mentioning features or use of this software
//     *    must display the following acknowledgement:
//     *  This product includes software developed by the University of
//     *  California, Berkeley and its contributors.
//     * 4. Neither the name of the University nor the names of its contributors
//     *    may be used to endorse or promote products derived from this software
//     *    without specific prior written permission.
//     *
//     * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
//     * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//     * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//     * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
//     * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//     * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
//     * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
//     * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//     * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
//     * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
//     * SUCH DAMAGE.
// 
// \***************************************************************************************

#define CC_IMPLEMENT_QSORT_EX(func_name, T, LT, user_data_type)                     \
void func_name(T *array, size_t total, user_data_type aux)                          \
{                                                                                   \
    int isort_thresh = 7;                                                           \
    T t;                                                                            \
    int sp = 0;                                                                     \
                                                                                    \
    struct                                                                          \
    {                                                                               \
        T *lb;                                                                      \
        T *ub;                                                                      \
    }                                                                               \
    stack[48];                                                                      \
                                                                                    \
    aux = aux;                                                                      \
                                                                                    \
    if(total <= 1)                                                                  \
        return;                                                                     \
                                                                                    \
    stack[0].lb = array;                                                            \
    stack[0].ub = array + (total - 1);                                              \
                                                                                    \
    while(sp >= 0)                                                                  \
    {                                                                               \
        T* left = stack[sp].lb;                                                     \
        T* right = stack[sp--].ub;                                                  \
                                                                                    \
        for(;;)                                                                     \
        {                                                                           \
            int i, n = (int)(right - left) + 1, m;                                  \
            T* ptr;                                                                 \
            T* ptr2;                                                                \
                                                                                    \
            if(n <= isort_thresh)                                                   \
            {                                                                       \
            insert_sort:                                                            \
                for(ptr = left + 1; ptr <= right; ptr++)                            \
                {                                                                   \
                    for(ptr2 = ptr; ptr2 > left && LT(ptr2[0],ptr2[-1]); ptr2--)    \
                        CC_SWAP(ptr2[0], ptr2[-1], t);                              \
                }                                                                   \
                break;                                                              \
            }                                                                       \
            else                                                                    \
            {                                                                       \
                T* left0;                                                           \
                T* left1;                                                           \
                T* right0;                                                          \
                T* right1;                                                          \
                T* pivot;                                                           \
                T* a;                                                               \
                T* b;                                                               \
                T* c;                                                               \
                int swap_cnt = 0;                                                   \
                                                                                    \
                left0 = left;                                                       \
                right0 = right;                                                     \
                pivot = left + (n/2);                                               \
                                                                                    \
                if(n > 40)                                                          \
                {                                                                   \
                    int d = n / 8;                                                  \
                    a = left, b = left + d, c = left + 2*d;                         \
                    left = LT(*a, *b) ? (LT(*b, *c) ? b : (LT(*a, *c) ? c : a))     \
                                      : (LT(*c, *b) ? b : (LT(*a, *c) ? a : c));    \
                                                                                    \
                    a = pivot - d, b = pivot, c = pivot + d;                        \
                    pivot = LT(*a, *b) ? (LT(*b, *c) ? b : (LT(*a, *c) ? c : a))    \
                                      : (LT(*c, *b) ? b : (LT(*a, *c) ? a : c));    \
                                                                                    \
                    a = right - 2*d, b = right - d, c = right;                      \
                    right = LT(*a, *b) ? (LT(*b, *c) ? b : (LT(*a, *c) ? c : a))    \
                                      : (LT(*c, *b) ? b : (LT(*a, *c) ? a : c));    \
                }                                                                   \
                                                                                    \
                a = left, b = pivot, c = right;                                     \
                pivot = LT(*a, *b) ? (LT(*b, *c) ? b : (LT(*a, *c) ? c : a))        \
                                   : (LT(*c, *b) ? b : (LT(*a, *c) ? a : c));       \
                if(pivot != left0)                                                  \
                {                                                                   \
                    CC_SWAP(*pivot, *left0, t);                                     \
                    pivot = left0;                                                  \
                }                                                                   \
                left = left1 = left0 + 1;                                           \
                right = right1 = right0;                                            \
                                                                                    \
                for(;;)                                                             \
                {                                                                   \
                    while(left <= right && !LT(*pivot, *left))                      \
                    {                                                               \
                        if(!LT(*left, *pivot))                                      \
                        {                                                           \
                            if(left > left1)                                        \
                                CC_SWAP(*left1, *left, t);                          \
                            swap_cnt = 1;                                           \
                            left1++;                                                \
                        }                                                           \
                        left++;                                                     \
                    }                                                               \
                                                                                    \
                    while(left <= right && !LT(*right, *pivot))                     \
                    {                                                               \
                        if(!LT(*pivot, *right))                                     \
                        {                                                           \
                            if(right < right1)                                      \
                                CC_SWAP(*right1, *right, t);                        \
                            swap_cnt = 1;                                           \
                            right1--;                                               \
                        }                                                           \
                        right--;                                                    \
                    }                                                               \
                                                                                    \
                    if(left > right)                                                \
                        break;                                                      \
                    CC_SWAP(*left, *right, t);                                      \
                    swap_cnt = 1;                                                   \
                    left++;                                                         \
                    right--;                                                        \
                }                                                                   \
                                                                                    \
                if(swap_cnt == 0)                                                   \
                {                                                                   \
                    left = left0, right = right0;                                   \
                    goto insert_sort;                                               \
                }                                                                   \
                                                                                    \
                n = MIN((int)(left1 - left0), (int)(left - left1));                 \
                for(i = 0; i < n; i++)                                              \
                    CC_SWAP(left0[i], left[i-n], t);                                \
                                                                                    \
                n = MIN((int)(right0 - right1), (int)(right1 - right));             \
                for(i = 0; i < n; i++)                                              \
                    CC_SWAP(left[i], right0[i-n+1], t);                             \
                n = (int)(left - left1);                                            \
                m = (int)(right1 - right);                                          \
                if(n > 1)                                                           \
                {                                                                   \
                    if(m > 1)                                                       \
                    {                                                               \
                        if(n > m)                                                   \
                        {                                                           \
                            stack[++sp].lb = left0;                                 \
                            stack[sp].ub = left0 + n - 1;                           \
                            left = right0 - m + 1, right = right0;                  \
                        }                                                           \
                        else                                                        \
                        {                                                           \
                            stack[++sp].lb = right0 - m + 1;                        \
                            stack[sp].ub = right0;                                  \
                            left = left0, right = left0 + n - 1;                    \
                        }                                                           \
                    }                                                               \
                    else                                                            \
                        left = left0, right = left0 + n - 1;                        \
                }                                                                   \
                else if(m > 1)                                                      \
                    left = right0 - m + 1, right = right0;                          \
                else                                                                \
                    break;                                                          \
            }                                                                       \
        }                                                                           \
    }                                                                               \
}

#define CC_IMPLEMENT_QSORT(func_name, T, cmp)  \
    CC_IMPLEMENT_QSORT_EX(func_name, T, cmp, int)

/****************************************************************************************\
*                     Structures and macros for integration with IPP                     *
\****************************************************************************************/

/* IPP-compatible return codes */

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
#define IPCVAPI(type,declspec,name,args)                        \
    /* function pointer */                                      \
    typedef type (declspec* name##_t) args;                     \
    extern name##_t name##_p;                                   \
    type declspec name args;
#endif

#define IPCVAPI_EX(type,name,ipp_name,ipp_search_modules,args)  \
    IPCVAPI(type,C_STDCALL,name,args) 

#define IPCVAPI_C_EX(type,name,ipp_name,ipp_search_modules,args)\
    IPCVAPI(type,C_CDECL,name,args) 

#ifndef IPCVAPI_IMPL
#define IPCVAPI_IMPL(type,name,args,arg_names)                  \
    static type C_STDCALL name##_f args;                       \
    name##_t name##_p = name##_f;                               \
    type C_STDCALL name args { return name##_p arg_names; }    \
    static type C_STDCALL name##_f args
#endif

typedef void*   (CvFuncTable)[CC_DEPTH_MAX];
typedef void*   (CvBtFuncTable)[33];
typedef void*   (CvBigFuncTable)[CC_DEPTH_MAX][CC_CN_MAX];


typedef CStatus (C_STDCALL *CvFunc2D_1A)(void* arr, int step, CSize size);
typedef CStatus (C_STDCALL *CvFunc2D_1A1P)(void* arr, int step, CSize size, void* param);
typedef CStatus (C_STDCALL *CvFunc2D_1A1P1I)(void* arr, int step, CSize size,void* param, int flag);
typedef CStatus (C_STDCALL *CvFunc2DnC_1A1P)(void* arr, int step, CSize size,int cn, int coi, void* param);
typedef CStatus (C_STDCALL *CvFunc2DnC_1A1P)(void* arr, int step, CSize size,int cn, int coi, void* param);
typedef CStatus (C_STDCALL *CvFunc2D_1A2P)(void* arr, int step, CSize size,void* param1, void* param2);
typedef CStatus (C_STDCALL *CvFunc2DnC_1A2P)(void* arr, int step,CSize size, int cn, int coi,void* param1, void* param2);
typedef CStatus (C_STDCALL *CvFunc2D_1A4P)(void* arr, int step, CSize size,void* param1, void* param2,void* param3, void* param4);
typedef CStatus (C_STDCALL *CvFunc2DnC_1A4P)(void* arr, int step,CSize size, int cn, int coi,void* param1, void* param2,void* param3, void* param4);
typedef CStatus (C_STDCALL *CvFunc2D_2A)(void* arr0, int step0,void* arr1, int step1, CSize size);
typedef CStatus (C_STDCALL *CvFunc2D_2A1P)(void* arr0, int step0,void* arr1, int step1,CSize size, void* param);
typedef CStatus (C_STDCALL *CvFunc2DnC_2A1P)(void* arr0, int step0,void* arr1, int step1,CSize size, int cn,int coi, void* param);
typedef CStatus (C_STDCALL *CvFunc2DnC_2A1P)(void* arr0, int step0,void* arr1, int step1,CSize size, int cn,int coi, void* param);
typedef CStatus (C_STDCALL *CvFunc2D_2A2P)(void* arr0, int step0,void* arr1, int step1, CSize size,void* param1, void* param2);
typedef CStatus (C_STDCALL *CvFunc2DnC_2A2P)(void* arr0, int step0,void* arr1, int step1,CSize size, int cn, int coi,void* param1, void* param2);
typedef CStatus (C_STDCALL *CvFunc2D_2A1P1I)(void* arr0, int step0,void* arr1, int step1, CSize size,void* param, int flag);
typedef CStatus (C_STDCALL *CvFunc2D_2A4P)(void* arr0, int step0,void* arr1, int step1, CSize size,void* param1, void* param2,void* param3, void* param4);
typedef CStatus (C_STDCALL *CvFunc2DnC_2A4P)(void* arr0, int step0,void* arr1, int step1, CSize size,int cn, int coi,void* param1, void* param2,void* param3, void* param4);
typedef CStatus (C_STDCALL *CvFunc2D_3A)(void* arr0, int step0,void* arr1, int step1,void* arr2, int step2, CSize size);
typedef CStatus (C_STDCALL *CvFunc2D_3A1P)(void* arr0, int step0,void* arr1, int step1,void* arr2, int step2,CSize size, void* param);
typedef CStatus (C_STDCALL *CvFunc2D_3A1I)(void* arr0, int step0,void* arr1, int step1,void* arr2, int step2,CSize size, int flag);
typedef CStatus (C_STDCALL *CvFunc2DnC_3A1P)(void* arr0, int step0,void* arr1, int step1,void* arr2, int step2,CSize size, int cn,int coi, void* param);
typedef CStatus (C_STDCALL *CvFunc2D_4A)(void* arr0, int step0,void* arr1, int step1,void* arr2, int step2,void* arr3, int step3,CSize size);
typedef CStatus (C_STDCALL *CvFunc0D)(const void* src, void* dst, int param);
#define CC_DEF_INIT_FUNC_TAB_2D(FUNCNAME, FLAG)               \
static void  icvInit##FUNCNAME##FLAG##Table(CvFuncTable  tab) \
{                                                             \
    assert(tab);                                              \
                                                              \
    tab[CC_8U]  = (void*)icv##FUNCNAME##_8u_##FLAG;           \
    tab[CC_8S]  = (void*)icv##FUNCNAME##_8s_##FLAG;           \
    tab[CC_16U] = (void*)icv##FUNCNAME##_16u_##FLAG;          \
    tab[CC_16S] = (void*)icv##FUNCNAME##_16s_##FLAG;          \
    tab[CC_32S] = (void*)icv##FUNCNAME##_32s_##FLAG;          \
    tab[CC_32F] = (void*)icv##FUNCNAME##_32f_##FLAG;          \
    tab[CC_64F] = (void*)icv##FUNCNAME##_64f_##FLAG;          \
}


#define CC_DEF_INIT_BIG_FUNC_TAB_2D(FUNCNAME, FLAG)               \
static void  icvInit##FUNCNAME##FLAG##Table(CvBigFuncTable  tab)  \
{                                                             \
    assert(tab);                                              \
                                                              \
    tab[CC_8U][1]  = (void*)icv##FUNCNAME##_8u_C1##FLAG;      \
    tab[CC_8U][2]  = (void*)icv##FUNCNAME##_8u_C2##FLAG;      \
    tab[CC_8U][3]  = (void*)icv##FUNCNAME##_8u_C3##FLAG;      \
    tab[CC_8U][4]  = (void*)icv##FUNCNAME##_8u_C4##FLAG;      \
                                                              \
    tab[CC_8S][1]  = (void*)icv##FUNCNAME##_8s_C1##FLAG;      \
    tab[CC_8S][2]  = (void*)icv##FUNCNAME##_8s_C2##FLAG;      \
    tab[CC_8S][3]  = (void*)icv##FUNCNAME##_8s_C3##FLAG;      \
    tab[CC_8S][4]  = (void*)icv##FUNCNAME##_8s_C4##FLAG;      \
                                                              \
    tab[CC_16U][1] = (void*)icv##FUNCNAME##_16u_C1##FLAG;     \
    tab[CC_16U][2] = (void*)icv##FUNCNAME##_16u_C2##FLAG;     \
    tab[CC_16U][3] = (void*)icv##FUNCNAME##_16u_C3##FLAG;     \
    tab[CC_16U][4] = (void*)icv##FUNCNAME##_16u_C4##FLAG;     \
                                                              \
    tab[CC_16S][1] = (void*)icv##FUNCNAME##_16s_C1##FLAG;     \
    tab[CC_16S][2] = (void*)icv##FUNCNAME##_16s_C2##FLAG;     \
    tab[CC_16S][3] = (void*)icv##FUNCNAME##_16s_C3##FLAG;     \
    tab[CC_16S][4] = (void*)icv##FUNCNAME##_16s_C4##FLAG;     \
                                                              \
    tab[CC_32S][1] = (void*)icv##FUNCNAME##_32s_C1##FLAG;     \
    tab[CC_32S][2] = (void*)icv##FUNCNAME##_32s_C2##FLAG;     \
    tab[CC_32S][3] = (void*)icv##FUNCNAME##_32s_C3##FLAG;     \
    tab[CC_32S][4] = (void*)icv##FUNCNAME##_32s_C4##FLAG;     \
                                                              \
    tab[CC_32F][1] = (void*)icv##FUNCNAME##_32f_C1##FLAG;     \
    tab[CC_32F][2] = (void*)icv##FUNCNAME##_32f_C2##FLAG;     \
    tab[CC_32F][3] = (void*)icv##FUNCNAME##_32f_C3##FLAG;     \
    tab[CC_32F][4] = (void*)icv##FUNCNAME##_32f_C4##FLAG;     \
                                                              \
    tab[CC_64F][1] = (void*)icv##FUNCNAME##_64f_C1##FLAG;     \
    tab[CC_64F][2] = (void*)icv##FUNCNAME##_64f_C2##FLAG;     \
    tab[CC_64F][3] = (void*)icv##FUNCNAME##_64f_C3##FLAG;     \
    tab[CC_64F][4] = (void*)icv##FUNCNAME##_64f_C4##FLAG;     \
}

#define CC_DEF_INIT_FUNC_TAB_0D(FUNCNAME)                     \
static void  icvInit##FUNCNAME##Table(CvFuncTable  tab)       \
{                                                             \
    tab[CC_8U]  = (void*)icv##FUNCNAME##_8u;                  \
    tab[CC_8S]  = (void*)icv##FUNCNAME##_8s;                  \
    tab[CC_16U] = (void*)icv##FUNCNAME##_16u;                 \
    tab[CC_16S] = (void*)icv##FUNCNAME##_16s;                 \
    tab[CC_32S] = (void*)icv##FUNCNAME##_32s;                 \
    tab[CC_32F] = (void*)icv##FUNCNAME##_32f;                 \
    tab[CC_64F] = (void*)icv##FUNCNAME##_64f;                 \
}

#define CC_DEF_INIT_FUNC_TAB_1D  CC_DEF_INIT_FUNC_TAB_0D


#define CC_DEF_INIT_PIXSIZE_TAB_2D(FUNCNAME, FLAG)            \
static void icvInit##FUNCNAME##FLAG##Table(CvBtFuncTable  table) \
{                                                             \
    table[1]  = (void*)icv##FUNCNAME##_8u_C1##FLAG;           \
    table[2]  = (void*)icv##FUNCNAME##_8u_C2##FLAG;           \
    table[3]  = (void*)icv##FUNCNAME##_8u_C3##FLAG;           \
    table[4]  = (void*)icv##FUNCNAME##_16u_C2##FLAG;          \
    table[6]  = (void*)icv##FUNCNAME##_16u_C3##FLAG;          \
    table[8]  = (void*)icv##FUNCNAME##_32s_C2##FLAG;          \
    table[12] = (void*)icv##FUNCNAME##_32s_C3##FLAG;          \
    table[16] = (void*)icv##FUNCNAME##_64s_C2##FLAG;          \
    table[24] = (void*)icv##FUNCNAME##_64s_C3##FLAG;          \
    table[32] = (void*)icv##FUNCNAME##_64s_C4##FLAG;          \
}

#define CC_GET_FUNC_PTR(func, table_entry)    \
    func = (table_entry);                      \
                                               \
    if(!func)                                  \
        CC_ERROR(CC_StsUnsupportedFormat, "")




#if defined _MSC_VER && _MSC_VER >= 1200
    /* disable warnings related to inline functions */
    #pragma warning(disable: 4711 4710 4514)
#endif

typedef unsigned long ulong;

#ifdef __BORLANDC__
    #define     WIN32
    #define     CC_DLL
    #undef      _CC_ALWAYS_PROFILE_
    #define     _CC_ALWAYS_NO_PROFILE_
#endif


//#undef CC_FAST_CAST_8U
//#undef CC_MIN_8U
//#undef CC_MAX_8U

typedef CvFunc2D_3A1I CvArithmBinMaskFunc2D;
typedef CvFunc2D_2A1P1I CvArithmUniMaskFunc2D;


/****************************************************************************************\
*                                   Complex arithmetics                                  *
\****************************************************************************************/


#if 1
struct CComplex64f;
typedef struct CComplex32f {
  float re, im;
  typedef CComplex32f self;
  CComplex32f() {re=0;im=0;}
  CComplex32f(const CComplex32f& v) {  re = v.re, im = v.im;  }
  CComplex32f(const CComplex64f& v);
  CComplex32f(float r, float  i) {  re = r;  im = i;}
  CComplex32f(float r) {  re = r;  im = 0; }

  self operator + (self b) const { const self& a = *this; return self(a.re + b.re, a.im + b.im); }
  self operator - (self b) const { const self& a = *this; return self(a.re - b.re, a.im - b.im); }
  self operator * (self b) const { const self& a = *this; return self(a.re*b.re - a.im*b.im, a.re*b.im + a.im*b.re); }
  self operator * (double b) const { const self& a = *this; return self(a.re * b, a.im * b); }
  self& operator += (self b) { self& a = *this; a.re += b.re, a.im += b.im; return *this; }
  self& operator -= (self b) { self& a = *this; a.re -= b.re, a.im -= b.im; return *this; }
  self operator *= (self b) { self& a = *this; return *this = self(a.re*b.re - a.im*b.im, a.re*b.im + a.im*b.re); }
  self operator *= (double b) { self& a = *this; a.re *= b, a.im *= b; return *this; }
} CComplex32f;
typedef struct CComplex64f {
  double re, im;
  typedef CComplex64f self;
  CComplex64f() {re=0;im=0;}
  CComplex64f(const CComplex64f& v) {    re = v.re, im = v.im;  }
  CComplex64f(const CComplex32f& v) {    re = v.re, im = v.im;  }
  CComplex64f(double r, double  i) {    re = r;  im = i;  }
  CComplex64f(double r) {    re = r;  im = 0;}
  
  self operator + (self b) const { const self& a = *this; return self(a.re + b.re, a.im + b.im); }
  self operator - (self b) const { const self& a = *this; return self(a.re - b.re, a.im - b.im); }
  self operator * (self b) const { const self& a = *this; return self(a.re*b.re - a.im*b.im, a.re*b.im + a.im*b.re); }
  self operator * (double b) const { const self& a = *this; return self(a.re * b, a.im * b); }
  self& operator += (self b) { self& a = *this; a.re += b.re, a.im += b.im; return *this; }
  self& operator -= (self b) { self& a = *this; a.re -= b.re, a.im -= b.im; return *this; }
  self operator *= (self b) { self& a = *this; return *this = self(a.re*b.re - a.im*b.im, a.re*b.im + a.im*b.re); }
  self operator *= (double b) { self& a = *this; a.re *= b, a.im *= b; return *this; }
} CComplex64f;
CComplex32f::CComplex32f(const CComplex64f& v) {    re = v.re, im = v.im;  }
#if 0
static const CComplex64f CComplex64f_0 = {0, 0};
static const CComplex32f CComplex32f_0 = {0, 0};
#else
static const CComplex64f CComplex64f_0(0, 0);
static const CComplex32f CComplex32f_0(0, 0);
#endif

#define CCOMPLEX_OPT(faver, CComplex, elemtype)   \
CC_INLINE CComplex CComplex_add_ ## faver(CComplex a, CComplex b) { CComplex c; c.re = a.re + b.re; c.im = a.im + b.im; return c; } \
CC_INLINE CComplex CComplex_sub_ ## faver(CComplex a, CComplex b) { CComplex c; c.re = a.re - b.re; c.im = a.im - b.im; return c; } \
CC_INLINE void CComplex_add2_ ## faver(CComplex* a, CComplex b) { a->re += b.re; a->im += b.im; } \
CC_INLINE void CComplex_sub2_ ## faver(CComplex* a, CComplex b) { a->re -= b.re; a->im -= b.im; } \
CC_INLINE CComplex CComplex_mul_ ## faver(CComplex a, CComplex b) { CComplex c; c.re = a.re*b.re - a.im*b.im; c.im = a.re*b.im + a.im*b.re; return c; } \
CC_INLINE CComplex CComplex_muladd_ ## faver(CComplex a, CComplex b, CComplex d) { CComplex c; c.re = d.re + a.re*b.re - a.im*b.im; c.im = d.im + a.re*b.im + a.im*b.re; return c; } \
CC_INLINE CComplex CComplex_muls_ ## faver(CComplex a, double b) { CComplex c; c.re = a.re*b; c.im = a.im*b; return c; } \
CC_INLINE CComplex CComplex_mulsadd_ ## faver(CComplex a, double b, CComplex d) { CComplex c; c.re = d.re + a.re*b; c.im = d.im + a.im*b; return c; } \
CC_INLINE CComplex CComplex_div_ ## faver(CComplex a, CComplex b) { CComplex c; double t = 1./((double)b.re*b.re + (double)b.im*b.im); \
    c.re = (a.re*b.re + a.im*b.im)*t; c.im = (-a.re*b.im + a.im*b.re)*t; return c; } \
CC_INLINE double CComplex_abs_## faver(CComplex a) { return sqrt((double)a.re*a.re + (double)a.im*a.im); } \
CC_INLINE CComplex CComplex_conj_## faver(CComplex32f a) { CComplex c; c.re = a.re; c.im = -a.im; return c; }

CCOMPLEX_OPT(32f, CComplex32f, float)
CCOMPLEX_OPT(64f, CComplex64f, double)

CC_INLINE CComplex32f cComplex_32f(float re, float im) {
  CComplex32f c;
  c.re = re;
  c.im = im;
  return c;
}
CC_INLINE CComplex64f cComplex_64f(double re, double im) {
  CComplex64f c;
  c.re = re;
  c.im = im;
  return c;
}
CC_INLINE CComplex64f cvtCComplex_32f_64f(CComplex32f f) {
  CComplex64f c;
  c.re = f.re;
  c.im = f.im;
  return c;
}
CC_INLINE CComplex32f cvtCComplex_64f_32f(CComplex64f f) {
  CComplex32f c;
  c.re = f.re;
  c.im = f.im;
  return c;
}
#else
struct CComplex32f
{
    float re, im;

    CComplex32f() {}
    CComplex32f(float _re, float _im=0) : re(_re), im(_im) {}
    CComplex32f(const CComplex64f& v);
    //CComplex32f(const CComplex32f& v) : re(v.re), im(v.im) {}
    //CComplex32f& operator = (const CComplex32f& v) { re = v.re; im = v.im; return *this; }
    operator CComplex64f() const;
};

struct CComplex64f
{
    double re, im;

    CComplex64f() {}
    CComplex64f(double _re, double _im=0) : re(_re), im(_im) {}
    explicit CComplex64f(const CComplex32f& v);
    //CComplex64f(const CComplex64f& v) : re(v.re), im(v.im) {}
    //CComplex64f& operator = (const CComplex64f& v) { re = v.re; im = v.im; return *this; }
    operator CComplex32f() const;
};

inline CComplex32f::CComplex32f(const CComplex64f& v) : re((float)v.re), im((float)v.im) {}
inline CComplex64f::CComplex64f(const CComplex32f& v) : re(v.re), im(v.im) {}

inline CComplex32f operator + (CComplex32f a, CComplex32f b)
{
    return CComplex32f(a.re + b.re, a.im + b.im);
}

inline CComplex32f& operator += (CComplex32f& a, CComplex32f b)
{
    a.re += b.re;
    a.im += b.im;
    return a;
}

inline CComplex32f operator - (CComplex32f a, CComplex32f b)
{
    return CComplex32f(a.re - b.re, a.im - b.im);
}

inline CComplex32f& operator -= (CComplex32f& a, CComplex32f b)
{
    a.re -= b.re;
    a.im -= b.im;
    return a;
}

inline CComplex32f operator - (CComplex32f a)
{
    return CComplex32f(-a.re, -a.im);
}

inline CComplex32f operator * (CComplex32f a, CComplex32f b)
{
    return CComplex32f(a.re*b.re - a.im*b.im, a.re*b.im + a.im*b.re);
}

inline double abs(CComplex32f a)
{
    return sqrt((double)a.re*a.re + (double)a.im*a.im);
}

inline CComplex32f conj(CComplex32f a)
{
    return CComplex32f(a.re, -a.im);
}


inline CComplex32f operator / (CComplex32f a, CComplex32f b)
{
    double t = 1./((double)b.re*b.re + (double)b.im*b.im);
    return CComplex32f((float)((a.re*b.re + a.im*b.im)*t),
                         (float)((-a.re*b.im + a.im*b.re)*t));
}

inline CComplex32f operator * (double a, CComplex32f b)
{
    return CComplex32f((float)(a*b.re), (float)(a*b.im));
}

inline CComplex32f operator * (CComplex32f a, double b)
{
    return CComplex32f((float)(a.re*b), (float)(a.im*b));
}

inline CComplex32f::operator CComplex64f() const
{
    return CComplex64f(re,im);
}


inline CComplex64f operator + (CComplex64f a, CComplex64f b)
{
    return CComplex64f(a.re + b.re, a.im + b.im);
}

inline CComplex64f& operator += (CComplex64f& a, CComplex64f b)
{
    a.re += b.re;
    a.im += b.im;
    return a;
}

inline CComplex64f operator - (CComplex64f a, CComplex64f b)
{
    return CComplex64f(a.re - b.re, a.im - b.im);
}

inline CComplex64f& operator -= (CComplex64f& a, CComplex64f b)
{
    a.re -= b.re;
    a.im -= b.im;
    return a;
}

inline CComplex64f operator - (CComplex64f a)
{
    return CComplex64f(-a.re, -a.im);
}

inline CComplex64f operator * (CComplex64f a, CComplex64f b)
{
    return CComplex64f(a.re*b.re - a.im*b.im, a.re*b.im + a.im*b.re);
}

inline double abs(CComplex64f a)
{
    return sqrt((double)a.re*a.re + (double)a.im*a.im);
}

inline CComplex64f operator / (CComplex64f a, CComplex64f b)
{
    double t = 1./((double)b.re*b.re + (double)b.im*b.im);
    return CComplex64f((a.re*b.re + a.im*b.im)*t,
                         (-a.re*b.im + a.im*b.re)*t);
}

inline CComplex64f operator * (double a, CComplex64f b)
{
    return CComplex64f(a*b.re, a*b.im);
}

inline CComplex64f operator * (CComplex64f a, double b)
{
    return CComplex64f(a.re*b, a.im*b);
}

inline CComplex64f::operator CComplex32f() const
{
    return CComplex32f((float)re,(float)im);
}

inline CComplex64f conj(CComplex64f a)
{
    return CComplex64f(a.re, -a.im);
}

inline CComplex64f operator + (CComplex64f a, CComplex32f b)
{
    return CComplex64f(a.re + b.re, a.im + b.im);
}

inline CComplex64f operator + (CComplex32f a, CComplex64f b)
{
    return CComplex64f(a.re + b.re, a.im + b.im);
}

inline CComplex64f operator - (CComplex64f a, CComplex32f b)
{
    return CComplex64f(a.re - b.re, a.im - b.im);
}

inline CComplex64f operator - (CComplex32f a, CComplex64f b)
{
    return CComplex64f(a.re - b.re, a.im - b.im);
}

inline CComplex64f operator * (CComplex64f a, CComplex32f b)
{
    return CComplex64f(a.re*b.re - a.im*b.im, a.re*b.im + a.im*b.re);
}

inline CComplex64f operator * (CComplex32f a, CComplex64f b)
{
    return CComplex64f(a.re*b.re - a.im*b.im, a.re*b.im + a.im*b.re);
}

#endif

typedef CStatus (C_STDCALL * CvCopyMaskFunc)(const void* src, int src_step,
                                               void* dst, int dst_step, CSize size,
                                               const void* mask, int mask_step);

CvCopyMaskFunc icvGetCopyMaskFunc(int elem_size);

typedef CStatus (C_STDCALL * CvLUT_TransformFunc)(const void* src, int srcstep, void* dst,
                                                     int dststep, CSize size, const void* lut);

#if 0
CC_INLINE CStatus
icvLUT_Transform8u_8s_C1R(const uchar* src, int srcstep, char* dst,
                            int dststep, CSize size, const char* lut)
{
    return icvLUT_Transform8u_8u_C1R(src, srcstep, (uchar*)dst,
                                      dststep, size, (const uchar*)lut);
}

CC_INLINE CStatus
icvLUT_Transform8u_16s_C1R(const uchar* src, int srcstep, short* dst,
                            int dststep, CSize size, const short* lut)
{
    return icvLUT_Transform8u_16u_C1R(src, srcstep, (ushort*)dst,
                                       dststep, size, (const ushort*)lut);
}

CC_INLINE CStatus
icvLUT_Transform8u_32f_C1R(const uchar* src, int srcstep, float* dst,
                            int dststep, CSize size, const float* lut)
{
    return icvLUT_Transform8u_32s_C1R(src, srcstep, (int*)dst,
                                       dststep, size, (const int*)lut);
}
#endif


//////////////////////////////////////////////////////////////////////////////////////////

/****************************************************************************************\
*                                      Copy/Set                                          *
\****************************************************************************************/

/* temporary disable ipp zero and copy functions as they affect subsequent functions' performance */
IPCVAPI_EX(CStatus, icvCopy_8u_C1R, "ippiCopy_8u_C1R", 0/*CC_PLUGINS1(CC_PLUGIN_IPPI)*/,
                  (const uchar* src, int src_step,
                    uchar* dst, int dst_step, CSize size))

IPCVAPI_EX(CStatus, icvSetByte_8u_C1R, "ippiSet_8u_C1R", 0/*CC_PLUGINS1(CC_PLUGIN_IPPI)*/,
                  (uchar value, uchar* dst, int dst_step, CSize size))

IPCVAPI_EX(CStatus, icvCvt_32f64f, "ippsConvert_32f64f",
            CC_PLUGINS1(CC_PLUGIN_IPPS), (const float* src, double* dst, int len))
IPCVAPI_EX(CStatus, icvCvt_64f32f, "ippsConvert_64f32f",
            CC_PLUGINS1(CC_PLUGIN_IPPS), (const double* src, float* dst, int len))


/****************************************************************************************\
*                                       Arithmetics                                      *
\****************************************************************************************/

/****************************************************************************************\
*                                     Logical operations                                 *
\****************************************************************************************/

IPCVAPI_EX(CStatus, icvNot_8u_C1R, "ippiNot_8u_C1R", CC_PLUGINS1(CC_PLUGIN_IPPI),
(const uchar* src, int step1, uchar* dst, int step, CSize size))

/****************************************************************************************\
*                                Image Statistics                                        *
\****************************************************************************************/

///////////////////////////////////////// Mean //////////////////////////////////////////


//////////////////////////////////// Mean_StdDev ////////////////////////////////////////

#undef IPCC_MEAN_SDV_PLUGIN
#define ICC_MEAN_SDV_PLUGIN 0 /* CC_PLUGINS1(IPPCV) */


//////////////////////////////////// MinMaxIndx /////////////////////////////////////////

////////////////////////////////////////// Sum //////////////////////////////////////////

////////////////////////////////////////// CountNonZero /////////////////////////////////

////////////////////////////////////////// Norms /////////////////////////////////

////////////////////////////////////// AbsDiff ///////////////////////////////////////////

////////////////////////////// Comparisons //////////////////////////////////////////

/****************************************************************************************\
*                                       Utilities                                        *
\****************************************************************************************/

////////////////////////////// Copy Pixel <-> Plane /////////////////////////////////

/****************************************************************************************/
/*                            Math routines and RNGs                                    */
/****************************************************************************************/

IPCVAPI_EX(CStatus, icvInvSqrt_32f, "ippsInvSqrt_32f_A21",
           CC_PLUGINS1(CC_PLUGIN_IPPVM),
           (const float* src, float* dst, int len))
IPCVAPI_EX(CStatus, icvSqrt_32f, "ippsSqrt_32f_A21, ippsSqrt_32f",
           CC_PLUGINS2(CC_PLUGIN_IPPVM,CC_PLUGIN_IPPS),
           (const float* src, float* dst, int len))
IPCVAPI_EX(CStatus, icvInvSqrt_64f, "ippsInvSqrt_64f_A50",
           CC_PLUGINS1(CC_PLUGIN_IPPVM),
           (const double* src, double* dst, int len))
IPCVAPI_EX(CStatus, icvSqrt_64f, "ippsSqrt_64f_A50, ippsSqrt_64f",
           CC_PLUGINS2(CC_PLUGIN_IPPVM,CC_PLUGIN_IPPS),
           (const double* src, double* dst, int len))

IPCVAPI_EX(CStatus, icvLog_32f, "ippsLn_32f_A21, ippsLn_32f",
           CC_PLUGINS2(CC_PLUGIN_IPPVM,CC_PLUGIN_IPPS),
           (const float *x, float *y, int n))
IPCVAPI_EX(CStatus, icvLog_64f, "ippsLn_64f_A50, ippsLn_64f",
           CC_PLUGINS2(CC_PLUGIN_IPPVM,CC_PLUGIN_IPPS),
           (const double *x, double *y, int n))
IPCVAPI_EX(CStatus, icvExp_32f, "ippsExp_32f_A21, ippsExp_32f",
           CC_PLUGINS2(CC_PLUGIN_IPPVM,CC_PLUGIN_IPPS),
           (const float *x, float *y, int n))
IPCVAPI_EX(CStatus, icvExp_64f, "ippsExp_64f_A50, ippsExp_64f",
           CC_PLUGINS2(CC_PLUGIN_IPPVM,CC_PLUGIN_IPPS),
           (const double *x, double *y, int n))
IPCVAPI_EX(CStatus, icvFastArctan_32f, "ippibFastArctan_32f",
           CC_PLUGINS1(CC_PLUGIN_IPPCV),
           (const float* y, const float* x, float* angle, int len))

/****************************************************************************************/
/*                                  Error handling functions                            */
/****************************************************************************************/

IPCVAPI_EX(CStatus, icvCheckArray_32f_C1R,
           "ippiCheckArray_32f_C1R", 0/*CC_PLUGINS1(CC_PLUGIN_OPTCV)*/,
           (const float* src, int srcstep,
             CSize size, int flags,
             double min_val, double max_val))

IPCVAPI_EX(CStatus, icvCheckArray_64f_C1R,
           "ippiCheckArray_64f_C1R", 0/*CC_PLUGINS1(CC_PLUGIN_OPTCV)*/,
           (const double* src, int srcstep,
             CSize size, int flags,
             double min_val, double max_val))

/****************************************************************************************/
/*                    Affine transformations of matrix/image elements                   */
/****************************************************************************************/

/****************************************************************************************/
/*                  Matrix routines from BLAS/LAPACK compatible libraries               */
/****************************************************************************************/

IPCVAPI_C_EX(void, icvBLAS_GEMM_32f, "sgemm, mkl_sgemm", CC_PLUGINS2(CC_PLUGIN_MKL,CC_PLUGIN_MKL),
                        (const char *transa, const char *transb, int *n, int *m, int *k,
                         const void *alpha, const void *a, int *lda, const void *b, int *ldb,
                         const void *beta, void *c, int *ldc))

IPCVAPI_C_EX(void, icvBLAS_GEMM_64f, "dgemm, mkl_dgemm", CC_PLUGINS2(CC_PLUGIN_MKL,CC_PLUGIN_MKL),
                        (const char *transa, const char *transb, int *n, int *m, int *k,
                         const void *alpha, const void *a, int *lda, const void *b, int *ldb,
                         const void *beta, void *c, int *ldc))

IPCVAPI_C_EX(void, icvBLAS_GEMM_32fc, "cgemm, mkl_cgemm", CC_PLUGINS2(CC_PLUGIN_MKL,CC_PLUGIN_MKL),
                        (const char *transa, const char *transb, int *n, int *m, int *k,
                         const void *alpha, const void *a, int *lda, const void *b, int *ldb,
                         const void *beta, void *c, int *ldc))

IPCVAPI_C_EX(void, icvBLAS_GEMM_64fc, "zgemm, mkl_zgemm", CC_PLUGINS2(CC_PLUGIN_MKL,CC_PLUGIN_MKL),
                        (const char *transa, const char *transb, int *n, int *m, int *k,
                         const void *alpha, const void *a, int *lda, const void *b, int *ldb,
                         const void *beta, void *c, int *ldc))


//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////


/****************************************************************************************\
*                                  Creating Borders                                      *
\****************************************************************************************/

/****************************************************************************************\
*                                        Moments                                         *
\****************************************************************************************/

IPCVAPI_EX(CStatus, icvMomentInitAlloc_64f,
            "ippiMomentInitAlloc_64f", CC_PLUGINS1(CC_PLUGIN_IPPI),
            (void** momentstate, CvHintAlgorithm hint))

IPCVAPI_EX(CStatus, icvMomentFree_64f,
            "ippiMomentFree_64f", CC_PLUGINS1(CC_PLUGIN_IPPI),
            (void* momentstate))

IPCVAPI_EX(CStatus, icvGetSpatialMoment_64f,
            "ippiGetSpatialMoment_64f", CC_PLUGINS1(CC_PLUGIN_IPPI),
            (const void* momentstate, int mOrd, int nOrd,
             int nChannel, CPoint roiOffset, double* value))

/****************************************************************************************\
*                                  Background differencing                               *
\****************************************************************************************/

/////////////////////////////////// Accumulation /////////////////////////////////////////

/****************************************************************************************\
*                                       Pyramids                                         *
\****************************************************************************************/

IPCVAPI_EX(CStatus, icvPyrDownGetBufSize_Gauss5x5,
           "ippiPyrDownGetBufSize_Gauss5x5", CC_PLUGINS1(CC_PLUGIN_IPPCV),
            (int roiWidth, CvDataType dataType, int channels, int* bufSize))

IPCVAPI_EX(CStatus, icvPyrUpGetBufSize_Gauss5x5,
           "ippiPyrUpGetBufSize_Gauss5x5", CC_PLUGINS1(CC_PLUGIN_IPPCV),
            (int roiWidth, CvDataType dataType, int channels, int* bufSize))

#define ICC_PYRDOWN(flavor, cn)                                           \
IPCVAPI_EX(CStatus, icvPyrDown_Gauss5x5_##flavor##_C##cn##R,              \
"ippiPyrDown_Gauss5x5_" #flavor "_C" #cn "R", CC_PLUGINS1(CC_PLUGIN_IPPCV), \
(const void* pSrc, int srcStep, void* pDst, int dstStep,                   \
  CSize roiSize, void* pBuffer))

#define ICC_PYRUP(flavor, cn)                                             \
IPCVAPI_EX(CStatus, icvPyrUp_Gauss5x5_##flavor##_C##cn##R,                \
"ippiPyrUp_Gauss5x5_" #flavor "_C" #cn "R", CC_PLUGINS1(CC_PLUGIN_IPPCV),   \
(const void* pSrc, int srcStep, void* pDst, int dstStep,                   \
  CSize roiSize, void* pBuffer))

ICC_PYRDOWN(8u, 1)
ICC_PYRDOWN(8u, 3)
ICC_PYRDOWN(32f, 1)
ICC_PYRDOWN(32f, 3)

ICC_PYRUP(8u, 1)
ICC_PYRUP(8u, 3)
ICC_PYRUP(32f, 1)
ICC_PYRUP(32f, 3)

#undef ICC_PYRDOWN
#undef ICC_PYRUP

/****************************************************************************************\
*                                Geometric Transformations                               *
\****************************************************************************************/

/****************************************************************************************\
*                                      Morphology                                        *
\****************************************************************************************/

IPCVAPI_EX(CStatus, icvMorphFree, "ippiMorphologyFree",
    CC_PLUGINS1(CC_PLUGIN_IPPCV), (void* morphstate))


/****************************************************************************************\
*                                 Smoothing Filters                                      *
\****************************************************************************************/

/****************************************************************************************\
*                                 Derivative Filters                                     *
\****************************************************************************************/


/////////////////////////////////////////////////////////////////////////////////////////

/****************************************************************************************\
*                                   Generic Filters                                      *
\****************************************************************************************/


/****************************************************************************************\
*                                  Color Transformations                                 *
\****************************************************************************************/

/****************************************************************************************\
*                                  Motion Templates                                      *
\****************************************************************************************/

IPCVAPI_EX(CStatus, icvUpdateMotionHistory_8u32f_C1IR,
    "ippiUpdateMotionHistory_8u32f_C1IR", CC_PLUGINS1(CC_PLUGIN_IPPCV),
    (const uchar* silIm, int silStep, float* mhiIm, int mhiStep,
      CSize size,float  timestamp, float  mhi_duration))

/****************************************************************************************\
*                                 Template Matching                                      *
\****************************************************************************************/

/****************************************************************************************/
/*                                Distance Transform                                    */
/****************************************************************************************/

IPCVAPI_EX(CStatus, icvDistanceTransform_3x3_8u32f_C1R,
    "ippiDistanceTransform_3x3_8u32f_C1R", CC_PLUGINS1(CC_PLUGIN_IPPCV),
    (const uchar* pSrc, int srcStep, float* pDst,
      int dstStep, CSize roiSize, const float* pMetrics))

IPCVAPI_EX(CStatus, icvDistanceTransform_5x5_8u32f_C1R,
    "ippiDistanceTransform_5x5_8u32f_C1R", CC_PLUGINS1(CC_PLUGIN_IPPCV),
    (const uchar* pSrc, int srcStep, float* pDst,
      int dstStep, CSize roiSize, const float* pMetrics))

/****************************************************************************************\
*                               Thresholding functions                                   *
\****************************************************************************************/

IPCVAPI_EX(CStatus, icvCompareC_8u_C1R_cv,
            "ippiCompareC_8u_C1R", CC_PLUGINS1(CC_PLUGIN_IPPI),
            (const uchar* src1, int srcstep1, uchar scalar,
              uchar* dst, int dststep, CSize size, int cmp_op))
IPCVAPI_EX(CStatus, icvAndC_8u_C1R,
            "ippiAndC_8u_C1R", CC_PLUGINS1(CC_PLUGIN_IPPI),
            (const uchar* src1, int srcstep1, uchar scalar,
              uchar* dst, int dststep, CSize size))
IPCVAPI_EX(CStatus, icvThreshold_GTVal_8u_C1R,
            "ippiThreshold_GTVal_8u_C1R", CC_PLUGINS1(CC_PLUGIN_IPPI),
            (const uchar* pSrc, int srcstep, uchar* pDst, int dststep,
              CSize size, uchar threshold, uchar value))
IPCVAPI_EX(CStatus, icvThreshold_GTVal_32f_C1R,
            "ippiThreshold_GTVal_32f_C1R", CC_PLUGINS1(CC_PLUGIN_IPPI),
            (const float* pSrc, int srcstep, float* pDst, int dststep,
              CSize size, float threshold, float value))
IPCVAPI_EX(CStatus, icvThreshold_LTVal_8u_C1R,
            "ippiThreshold_LTVal_8u_C1R", CC_PLUGINS1(CC_PLUGIN_IPPI),
            (const uchar* pSrc, int srcstep, uchar* pDst, int dststep,
              CSize size, uchar threshold, uchar value))
IPCVAPI_EX(CStatus, icvThreshold_LTVal_32f_C1R,
            "ippiThreshold_LTVal_32f_C1R", CC_PLUGINS1(CC_PLUGIN_IPPI),
            (const float* pSrc, int srcstep, float* pDst, int dststep,
              CSize size, float threshold, float value))

/****************************************************************************************\
*                                 Canny Edge Detector                                    *
\****************************************************************************************/

IPCVAPI_EX(CStatus, icvCannyGetSize, "ippiCannyGetSize", 0/*CC_PLUGINS1(CC_PLUGIN_IPPCV)*/,
           (CSize roiSize, int* bufferSize))

IPCVAPI_EX(CStatus, icvCanny_16s8u_C1R, "ippiCanny_16s8u_C1R", 0/*CC_PLUGINS1(CC_PLUGIN_IPPCV)*/,
    (const short* pSrcDx, int srcDxStep, const short* pSrcDy, int srcDyStep,
      uchar*  pDstEdges, int dstEdgeStep, CSize roiSize, float lowThresh,
      float  highThresh, void* pBuffer))


/****************************************************************************************\
*                                 Radial Distortion Removal                              *
\****************************************************************************************/

IPCVAPI_EX(CStatus, icvUndistortGetSize, "ippiUndistortGetSize",
            CC_PLUGINS1(CC_PLUGIN_IPPCV), (CSize roiSize, int *pBufsize))

IPCVAPI_EX(CStatus, icvCreateMapCameraUndistort_32f_C1R,
            "ippiCreateMapCameraUndistort_32f_C1R", CC_PLUGINS1(CC_PLUGIN_IPPCV),
            (float *pxMap, int xStep, float *pyMap, int yStep, CSize roiSize,
            float fx, float fy, float cx, float cy, float k1, float k2,
            float p1, float p2, uchar *pBuffer))

#define ICC_UNDISTORT_RADIAL(flavor, cn, arrtype)                                 \
IPCVAPI_EX(CStatus, icvUndistortRadial_##flavor##_C##cn##R,                       \
    "ippiUndistortRadial_" #flavor "_C" #cn "R", CC_PLUGINS1(CC_PLUGIN_IPPCV),      \
    (const arrtype* pSrc, int srcStep, uchar* pDst, int dstStep, CSize roiSize,   \
      float fx, float fy, float cx, float cy, float k1, float k2, uchar *pBuffer))

ICC_UNDISTORT_RADIAL(8u, 1, uchar)
ICC_UNDISTORT_RADIAL(8u, 3, uchar)

#undef ICC_UNDISTORT_RADIAL

/****************************************************************************************\
*                            Subpixel-accurate rectangle extraction                      *
\****************************************************************************************/

#define ICC_COPY_SUBPIX(flavor, cn, srctype, dsttype)                     \
IPCVAPI_EX(CStatus, icvCopySubpix_##flavor##_C##cn##R,                    \
    "ippiCopySubpix_" #flavor "_C" #cn "R", CC_PLUGINS1(CC_PLUGIN_IPPCV),   \
    (const srctype* pSrc, int srcStep, dsttype* pDst, int dstStep,         \
    CSize size, float dx, float dy))

ICC_COPY_SUBPIX(8u, 1, uchar, uchar)
ICC_COPY_SUBPIX(8u32f, 1, uchar, float)
//ICC_COPY_SUBPIX(32f, 1, float, float)

IPCVAPI_EX(CStatus, icvCopySubpix_32f_C1R,
    "ippiCopySubpix_32f_C1R", 0,
    (const float* pSrc, int srcStep, float* pDst, int dstStep,
    CSize size, float dx, float dy))

#undef ICC_COPY_SUBPIX

/****************************************************************************************\
*                                Lucas-Kanade Optical Flow                               *
\****************************************************************************************/

IPCVAPI_EX(CStatus, icvOpticalFlowPyrLKInitAlloc_8u_C1R,
            "ippiOpticalFlowPyrLKInitAlloc_8u_C1R", CC_PLUGINS1(CC_PLUGIN_IPPCV),
            (void** ppState, CSize roiSize, int winSize, int hint))

IPCVAPI_EX(CStatus, icvOpticalFlowPyrLKFree_8u_C1R,
            "ippiOpticalFlowPyrLKFree_8u_C1R", CC_PLUGINS1(CC_PLUGIN_IPPCV),
            (void* pState))

IPCVAPI_EX(CStatus, icvOpticalFlowPyrLK_8u_C1R,
            "ippiOpticalFlowPyrLK_8u_C1R", CC_PLUGINS1(CC_PLUGIN_IPPCV),
            (CPyramid *pPyr1, CPyramid *pPyr2,
            const float *pPrev, float* pNext, char *pStatus,
            float *pError, int numFeat, int winSize,
            int maxLev, int maxIter, float threshold, void* state))


/****************************************************************************************\
*                                 Haar Object Detector                                   *
\****************************************************************************************/

//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
#endif /*_CVTYPE_INL_*/
