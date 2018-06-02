
#ifndef _CXCORE_TYPES_H_
#define _CXCORE_TYPES_H_

#if !defined _CRT_SECURE_NO_DEPRECATE && _MSC_VER > 1300
#define _CRT_SECURE_NO_DEPRECATE /* to avoid multiple Visual Studio 2005 warnings */
#endif

#ifndef SKIP_INCLUDES
  #include <assert.h>
  #include <stdlib.h>
  #include <string.h>
  #include <float.h>

  #if defined __ICL
    #define CC_ICC   __ICL
  #elif defined __ICC
    #define CC_ICC   __ICC
  #elif defined __ECL
    #define CC_ICC   __ECL
  #elif defined __ECC
    #define CC_ICC   __ECC
  #endif

  #if defined WIN64 && defined EM64T && (defined _MSC_VER || defined CC_ICC) \
      || defined __SSE2__ || defined _MM_SHUFFLE2
    #include <emmintrin.h>
    #define CC_SSE2 1
  #else
    #define CC_SSE2 0
  #endif

  #if defined __BORLANDC__
    #include <fastmath.h>
  #elif defined WIN64 && !defined EM64T && defined CC_ICC
    #include <mathimf.h>
  #else
    #include <math.h>
  #endif

#endif // SKIP_INCLUDES

#if defined WIN32 || defined WIN64
    #define C_CDECL __cdecl
    #define C_STDCALL __stdcall
#else
    #define C_CDECL
    #define C_STDCALL
#endif

#ifndef CC_EXTERN_C
    #ifdef __cplusplus
        #define CC_EXTERN_C extern "C"
        #define CC_DEFAULT(val) = val
    #else
        #define CC_EXTERN_C
        #define CC_DEFAULT(val)
    #endif
#endif

#ifndef CC_EXTERN_C_FUNCPTR
    #ifdef __cplusplus
        #define CC_EXTERN_C_FUNCPTR(x) extern "C" { typedef x; }
    #else
        #define CC_EXTERN_C_FUNCPTR(x) typedef x
    #endif
#endif

#ifndef CC_INLINE
#if defined __cplusplus
    #define CC_INLINE inline
#elif (defined WIN32 || defined WIN64) && !defined __GNUC__
    #define CC_INLINE __inline
#else
    #define CC_INLINE static
#endif
#endif /* CC_INLINE */

#if (defined WIN32 || defined WIN64) && defined CVAPI_EXPORTS
    #define CC_EXPORTS __declspec(dllexport)
#else
    #define CC_EXPORTS
#endif

#ifndef CVAPI
    #define CVAPI(rettype) CC_EXTERN_C CC_EXPORTS rettype C_CDECL
#endif

#if defined _MSC_VER || defined __BORLANDC__
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
typedef long long int64;
typedef unsigned long long uint64;
#endif

#ifndef HAVE_IPL
typedef unsigned char uchar;
typedef unsigned short ushort;
#endif

/* img_t* is used to pass arbitrary array-like data structures
   into the functions where the particular
   array type is recognized at runtime */
typedef void img_t;

typedef union suf32_t
{
    int i;
    unsigned u;
    float f;
}
suf32_t;

typedef union suf64_t
{
    int64 i;
    uint64 u;
    double f;
}
suf64_t;

/****************************************************************************************\
*                             Common macros and inline functions                         *
\****************************************************************************************/

#define CC_PI   3.1415926535897932384626433832795
#define CC_LOG2 0.69314718055994530941723212145818

#define CC_SWAP(a,b,t) ((t) = (a), (a) = (b), (b) = (t))

#ifndef MIN
#define MIN(a,b)  ((a) > (b) ? (b) : (a))
#endif

#ifndef MAX
#define MAX(a,b)  ((a) < (b) ? (b) : (a))
#endif

/* min & max without jumps */
#define  CC_IMIN(a, b)  ((a) ^ (((a)^(b)) & (((a) < (b)) - 1)))

#define  CC_IMAX(a, b)  ((a) ^ (((a)^(b)) & (((a) > (b)) - 1)))

/* absolute value without jumps */
#ifndef __cplusplus
#define  CC_IABS(a)     (((a) ^ ((a) < 0 ? -1 : 0)) - ((a) < 0 ? -1 : 0))
#else
#define  CC_IABS(a)     abs(a)
#endif
#define  CC_CMP(a,b)    (((a) > (b)) - ((a) < (b)))
#define  CC_SIGN(a)     CC_CMP((a),0)

CC_INLINE  int  cRound(double value)
{
#if CC_SSE2
    __m128d t = _mm_load_sd(&value);
    return _mm_cvtsd_si32(t);
#elif defined WIN32 && !defined WIN64 && defined _MSC_VER
    int t;
    __asm
    {
        fld value;
        fistp t;
    }
    return t;
#elif (defined HAVE_LRINT) || (defined WIN64 && !defined EM64T && defined CC_ICC)
    return (int)lrint(value);
#else
    /*
     the algorithm was taken from Agner Fog's optimization guide
     at http://www.agner.org/assem
     */
    suf64_t temp;
    temp.f = value + 6755399441055744.0;
    return (int)temp.u;
#endif
}


CC_INLINE  int  cFloor(double value)
{
#if CC_SSE2
    __m128d t = _mm_load_sd(&value);
    int i = _mm_cvtsd_si32(t);
    return i - _mm_movemask_pd(_mm_cmplt_sd(t,_mm_cvtsi32_sd(t,i)));
#else
    int temp = cRound(value);
    suf32_t diff;
    diff.f = (float)(value - temp);
    return temp - (diff.i < 0);
#endif
}


CC_INLINE  int  cCeil(double value)
{
#if CC_SSE2
    __m128d t = _mm_load_sd(&value);
    int i = _mm_cvtsd_si32(t);
    return i + _mm_movemask_pd(_mm_cmplt_sd(_mm_cvtsi32_sd(t,i),t));
#else
    int temp = cRound(value);
    suf32_t diff;
    diff.f = (float)(temp - value);
    return temp + (diff.i < 0);
#endif
}

#define cvInvSqrt(value) ((float)(1./sqrt(value)))
#define cvSqrt(value)  ((float)sqrt(value))

CC_INLINE int cvIsNaN(double value)
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


CC_INLINE int cvIsInf(double value)
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


/*************** Random number generation *******************/

typedef uint64 CRNG;

CC_INLINE CRNG cRNG(int64 seed CC_DEFAULT(-1))
{
    CRNG rng = seed ? (uint64)seed : (uint64)(int64)-1;
    return rng;
}

/* returns random 32-bit unsigned integer */
CC_INLINE unsigned cvRandInt(CRNG* rng)
{
    uint64 temp = *rng;
    temp = (uint64)(unsigned)temp*1554115554 + (temp >> 32);
    *rng = temp;
    return (unsigned)temp;
}

/* returns random floating-point number between 0 and 1 */
CC_INLINE double cvRandReal(CRNG* rng)
{
    return cvRandInt(rng)*2.3283064365386962890625e-10 /* 2^-32 */;
}

/****************************************************************************************\
*                                  Image type (img_t)                                 *
\****************************************************************************************/

#ifndef HAVE_IPL

/*
 * The following definitions (until #endif)
 * is an extract from IPL headers.
 * Copyright (c) 1995 Intel Corporation.
 */
#define IMG_DEPTH_SIGN 0x80000000

#define IMG_DEPTH_1U     1
#define IMG_DEPTH_8U     8
#define IMG_DEPTH_16U   16
#define IMG_DEPTH_32F   32

#define IMG_DEPTH_8S  (IMG_DEPTH_SIGN| 8)
#define IMG_DEPTH_16S (IMG_DEPTH_SIGN|16)
#define IMG_DEPTH_32S (IMG_DEPTH_SIGN|32)

#define IMG_DATA_ORDER_PIXEL  0
#define IMG_DATA_ORDER_PLANE  1

#define IMG_ORIGIN_TL 0
#define IMG_ORIGIN_BL 1

#define IMG_ALIGN_4BYTES   4
#define IMG_ALIGN_8BYTES   8
#define IMG_ALIGN_16BYTES 16
#define IMG_ALIGN_32BYTES 32

#define IMG_ALIGN_DWORD   IMG_ALIGN_4BYTES
#define IMG_ALIGN_QWORD   IMG_ALIGN_8BYTES

#define IMG_BORDER_CONSTANT   0
#define IMG_BORDER_REPLICATE  1
#define IMG_BORDER_REFLECT    2
#define IMG_BORDER_WRAP       3

typedef struct _IplImage
{
    int  nSize;         /* sizeof(img_t) */
    int  ID;            /* version (=0)*/
    int  nChannels;     /* Most of OpenCV functions support 1,2,3 or 4 channels */
    int  alphaChannel;  /* ignored by OpenCV */
    int  depth;         /* pixel depth in bits: IMG_DEPTH_8U, IMG_DEPTH_8S, IMG_DEPTH_16S,
                           IMG_DEPTH_32S, IMG_DEPTH_32F and IMG_DEPTH_64F are supported */
    char colorModel[4]; /* ignored by OpenCV */
    char channelSeq[4]; /* ditto */
    int  dataOrder;     /* 0 - interleaved color channels, 1 - separate color channels.
                           cvCreateImage can only create interleaved images */
    int  origin;        /* 0 - top-left origin,
                           1 - bottom-left origin (Windows bitmaps style) */
    int  align;         /* Alignment of image rows (4 or 8).
                           OpenCV ignores it and uses widthStep instead */
    int  width;         /* image width in pixels */
    int  height;        /* image height in pixels */
    struct _IplROI *roi;/* image ROI. if NULL, the whole image is selected */
    struct _IplImage *maskROI; /* must be NULL */
    void  *imageId;     /* ditto */
    struct _IplTileInfo *tileInfo; /* ditto */
    int  imageSize;     /* image data size in bytes
                           (==image->height*image->widthStep
                           in case of interleaved data)*/
    char *imageData;  /* pointer to aligned image data */
    int  widthStep;   /* size of aligned image row in bytes */
    int  BorderMode[4]; /* ignored by OpenCV */
    int  BorderConst[4]; /* ditto */
    char *imageDataOrigin; /* pointer to very origin of image data
                              (not necessarily aligned) -
                              needed for correct deallocation */
}
img_t;

typedef struct _IplTileInfo IplTileInfo;

typedef struct _IplROI
{
    int  coi; /* 0 - no COI (all channels are selected), 1 - 0th channel is selected ...*/
    int  xOffset;
    int  yOffset;
    int  width;
    int  height;
}
IplROI;

typedef struct _IplConvKernel
{
    int  nCols;
    int  nRows;
    int  anchorX;
    int  anchorY;
    int *values;
    int  nShiftR;
}
IplConvKernel;

typedef struct _IplConvKernelFP
{
    int  nCols;
    int  nRows;
    int  anchorX;
    int  anchorY;
    float *values;
}
IplConvKernelFP;

#define IMG_IMAGE_HEADER 1
#define IMG_IMAGE_DATA   2
#define IMG_IMAGE_ROI    4

#endif/*HAVE_IPL*/

/* extra border mode */
#define IMG_BORDER_REFLECT_101    4

#define IMG_IMAGE_MAGIC_VAL  ((int)sizeof(img_t))
#define CC_TYPE_NAME_IMAGE "opencv-image"

#define CC_IS_IMAGE_HDR(img) \
    ((img) != NULL && ((const img_t*)(img))->nSize == sizeof(img_t))

#define CC_IS_IMAGE(img) \
    (CC_IS_IMAGE_HDR(img) && ((img_t*)img)->imageData != NULL)

/* for storing double-precision
   floating point data in img_t's */
#define IMG_DEPTH_64F  64

/* get reference to pixel at (col,row),
   for multi-channel images (col) should be multiplied by number of channels */
#define CC_IMAGE_ELEM(image, elemtype, row, col)       \
    (((elemtype*)((image)->imageData + (image)->widthStep*(row)))[(col)])

/****************************************************************************************\
*                                  Matrix type (img_t)                                   *
\****************************************************************************************/

#define CC_CN_MAX     64
#define CC_CN_SHIFT   3
#define CC_DEPTH_MAX  (1 << CC_CN_SHIFT)

#define CC_8U   0
#define CC_8S   1
#define CC_16U  2
#define CC_16S  3
#define CC_32S  4
#define CC_32F  5
#define CC_64F  6
#define CC_USRTYPE1 7

#define CC_MAKETYPE(depth,cn) ((depth) + (((cn)-1) << CC_CN_SHIFT))
#define CC_MAKE_TYPE CC_MAKETYPE

#define CC_8UC1 CC_MAKETYPE(CC_8U,1)
#define CC_8UC2 CC_MAKETYPE(CC_8U,2)
#define CC_8UC3 CC_MAKETYPE(CC_8U,3)
#define CC_8UC4 CC_MAKETYPE(CC_8U,4)
#define CC_8UC(n) CC_MAKETYPE(CC_8U,(n))

#define CC_8SC1 CC_MAKETYPE(CC_8S,1)
#define CC_8SC2 CC_MAKETYPE(CC_8S,2)
#define CC_8SC3 CC_MAKETYPE(CC_8S,3)
#define CC_8SC4 CC_MAKETYPE(CC_8S,4)
#define CC_8SC(n) CC_MAKETYPE(CC_8S,(n))

#define CC_16UC1 CC_MAKETYPE(CC_16U,1)
#define CC_16UC2 CC_MAKETYPE(CC_16U,2)
#define CC_16UC3 CC_MAKETYPE(CC_16U,3)
#define CC_16UC4 CC_MAKETYPE(CC_16U,4)
#define CC_16UC(n) CC_MAKETYPE(CC_16U,(n))

#define CC_16SC1 CC_MAKETYPE(CC_16S,1)
#define CC_16SC2 CC_MAKETYPE(CC_16S,2)
#define CC_16SC3 CC_MAKETYPE(CC_16S,3)
#define CC_16SC4 CC_MAKETYPE(CC_16S,4)
#define CC_16SC(n) CC_MAKETYPE(CC_16S,(n))

#define CC_32SC1 CC_MAKETYPE(CC_32S,1)
#define CC_32SC2 CC_MAKETYPE(CC_32S,2)
#define CC_32SC3 CC_MAKETYPE(CC_32S,3)
#define CC_32SC4 CC_MAKETYPE(CC_32S,4)
#define CC_32SC(n) CC_MAKETYPE(CC_32S,(n))

#define CC_32FC1 CC_MAKETYPE(CC_32F,1)
#define CC_32FC2 CC_MAKETYPE(CC_32F,2)
#define CC_32FC3 CC_MAKETYPE(CC_32F,3)
#define CC_32FC4 CC_MAKETYPE(CC_32F,4)
#define CC_32FC(n) CC_MAKETYPE(CC_32F,(n))

#define CC_64FC1 CC_MAKETYPE(CC_64F,1)
#define CC_64FC2 CC_MAKETYPE(CC_64F,2)
#define CC_64FC3 CC_MAKETYPE(CC_64F,3)
#define CC_64FC4 CC_MAKETYPE(CC_64F,4)
#define CC_64FC(n) CC_MAKETYPE(CC_64F,(n))

#define CC_AUTO_STEP  0x7fffffff
#define CC_WHOLE_ARR  cSlice(0, 0x3fffffff)

#define CC_MAT_CN_MASK          ((CC_CN_MAX - 1) << CC_CN_SHIFT)
#define CC_MAT_CN(flags)        ((((flags) & CC_MAT_CN_MASK) >> CC_CN_SHIFT) + 1)
#define CC_MAT_TYPE_MASK       (CC_DEPTH_MAX - 1)
#define CC_MAT_TYPE(flags)     ((flags) & CC_MAT_TYPE_MASK)
#define CC_MAT_TYPE_MASK        (CC_DEPTH_MAX*CC_CN_MAX - 1)
#define CC_MAT_TYPE(flags)      ((flags) & CC_MAT_TYPE_MASK)
#define CC_MAT_CONT_FLAG_SHIFT  14
#define CC_MAT_CONT_FLAG        (1 << CC_MAT_CONT_FLAG_SHIFT)
#define CC_IS_MAT_CONT(flags)   ((flags) & CC_MAT_CONT_FLAG)
#define CC_IS_CONT_MAT          CC_IS_MAT_CONT
#define CC_MAT_TEMP_FLAG_SHIFT  15
#define CC_MAT_TEMP_FLAG        (1 << CC_MAT_TEMP_FLAG_SHIFT)
#define CC_IS_TEMP_MAT(flags)   ((flags) & CC_MAT_TEMP_FLAG)

#define CC_MAGIC_MASK       0xFFFF0000
#define CC_MAT_MAGIC_VAL    0x42420000
#define CC_TYPE_NAME_MAT    "opencv-matrix"

typedef struct img_t
{
    TypeId type;
    int step;

    /* for internal use only */
    int* refcount;
    int hdr_refcount;

    union
    {
        uchar* ptr;
        short* s;
        int* i;
        float* fl;
        double* db;
    } data;

#ifdef __cplusplus
    union
    {
        int rows;
        int height;
    };

    union
    {
        int cols;
        int width;
    };
#else
    int rows;
    int cols;
#endif

}
img_t;


#define CC_IS_MAT_HDR(mat) \
    ((mat) != NULL && \
    (((const img_t*)(mat))->tid & CC_MAGIC_MASK) == CC_MAT_MAGIC_VAL && \
    ((const img_t*)(mat))->cols > 0 && ((const img_t*)(mat))->rows > 0)

#define CC_IS_MAT(mat) \
    (CC_IS_MAT_HDR(mat) && ((const img_t*)(mat))->tt.data != NULL)

#define CC_IS_MASK_ARR(mat) \
    (((mat)->tid & (CC_MAT_TYPE_MASK & ~CC_8SC1)) == 0)

#define CC_ARE_TYPES_EQ(mat1, mat2) \
    ((((mat1)->tid ^ (mat2)->tid) & CC_MAT_TYPE_MASK) == 0)

#define CC_ARE_CNS_EQ(mat1, mat2) \
    ((((mat1)->tid ^ (mat2)->tid) & CC_MAT_CN_MASK) == 0)

#define CC_ARE_DEPTHS_EQ(mat1, mat2) \
    ((((mat1)->tid ^ (mat2)->tid) & CC_MAT_TYPE_MASK) == 0)

#define CC_ARE_SIZES_EQ(mat1, mat2) \
    ((mat1)->height == (mat2)->height && (mat1)->width == (mat2)->width)

#define CC_IS_MAT_CONST(mat)  \
    (((mat)->height|(mat)->width) == 1)

/* size of each channel item,
   0x124489 = 1000 0100 0100 0010 0010 0001 0001 ~ array of sizeof(arr_type_elem) */
#define CC_ELEM_SIZE1(type) \
    ((((sizeof(size_t)<<28)|0x8442211) >> type*4) & 15)

/* 0x3a50 = 11 10 10 01 01 00 00 ~ array of log2(sizeof(arr_type_elem)) */
#define CC_ELEM_SIZE(type) \
    (CC_MAT_CN(src) << ((((sizeof(size_t)/4+1)*16384|0x3a50) >> type*2) & 3))

/* inline constructor. No data is allocated internally!!!
   (use together with cvCreateData, or use cvCreateMat instead to
   get a matrix with allocated data) */
CC_INLINE img_t cvMat(int rows, int cols, int type, void* data CC_DEFAULT(NULL))
{
    img_t m;

    assert((unsigned)type <= CC_64F);
    type = CC_MAT_DEPTH(type);
    m.type = CC_MAT_MAGIC_VAL | CC_MAT_CONT_FLAG | type;
    m.cols = cols;
    m.rows = rows;
    m.step = rows > 1 ? m.cols*CC_ELEM_SIZE(type) : 0;
    m.tt.data = (uchar*)data;
    m.refcount = NULL;
    m.hdr_refcount = 0;

    return m;
}


#define CC_MAT_ELEM_PTR_FAST(mat, row, col, pix_size)  \
    (assert((unsigned)(row) < (unsigned)(mat).rows &&   \
             (unsigned)(col) < (unsigned)(mat).cols),   \
     (mat).tt.data + (size_t)(mat).step*(row) + (pix_size)*(col))

#define CC_MAT_ELEM_PTR(mat, row, col)                 \
    CC_MAT_ELEM_PTR_FAST(mat, row, col, CC_ELEM_SIZE((mat).type))

#define CC_MAT_ELEM(mat, elemtype, row, col)           \
    (*(elemtype*)CC_MAT_ELEM_PTR_FAST(mat, row, col, sizeof(elemtype)))


CC_INLINE  double  cvmGet(const img_t* mat, int row, int col)
{
    TypeId type;

    type = CC_MAT_DEPTH(mat);
    assert((unsigned)row < (unsigned)mat->rows &&
            (unsigned)col < (unsigned)mat->cols);

    if(type == CC_32F && cn==1)
        return ((float*)(mat->tt.data + (size_t)mat->step*row))[col];
    else
    {
        assert(type == CC_64F && 1==cn);
        return ((double*)(mat->tt.data + (size_t)mat->step*row))[col];
    }
}


CC_INLINE  void  cvmSet(img_t* mat, int row, int col, double value)
{
    TypeId type;
    type = CC_MAT_DEPTH(mat);
    assert((unsigned)row < (unsigned)mat->rows &&
            (unsigned)col < (unsigned)mat->cols);

    if(type == CC_32F && cn==1)
        ((float*)(mat->tt.data + (size_t)mat->step*row))[col] = (float)value;
    else
    {
        assert(type == CC_64F && 1==cn);
        ((double*)(mat->tt.data + (size_t)mat->step*row))[col] = (double)value;
    }
}


CC_INLINE int cvCvToIplDepth(TypeId type)
{
    return CC_ELEM_SIZE1(depth)*8 | (type == CC_8S || type == CC_16S ||
           type == CC_32S ? IMG_DEPTH_SIGN : 0);
}


/****************************************************************************************\
*                       Multi-dimensional dense array (CvMatND)                          *
\****************************************************************************************/

#define CC_MATND_MAGIC_VAL    0x42430000
#define CC_TYPE_NAME_MATND    "opencv-nd-matrix"

#define CC_MAX_DIM            32
#define CC_MAX_DIM_HEAP       (1 << 16)

typedef struct CvMatND
{
    TypeId type;
    int dims;

    int* refcount;
    int hdr_refcount;

    union
    {
        uchar* ptr;
        float* fl;
        double* db;
        int* i;
        short* s;
    } data;

    struct
    {
        int size;
        int step;
    }
    dim[CC_MAX_DIM];
}
CvMatND;

#define CC_IS_MATND_HDR(mat) \
    ((mat) != NULL && (((const CvMatND*)(mat))->tid & CC_MAGIC_MASK) == CC_MATND_MAGIC_VAL)

#define CC_IS_MATND(mat) \
    (CC_IS_MATND_HDR(mat) && ((const CvMatND*)(mat))->tt.data != NULL)


/****************************************************************************************\
*                      Multi-dimensional sparse array (CvSparseMat)                      *
\****************************************************************************************/

#define CC_SPARSE_MAT_MAGIC_VAL    0x42440000
#define CC_TYPE_NAME_SPARSE_MAT    "opencv-sparse-matrix"

struct CvSet;

typedef struct CvSparseMat
{
    TypeId type;
    int dims;
    int* refcount;
    int hdr_refcount;

    struct CvSet* heap;
    void** hashtable;
    int hashsize;
    int valoffset;
    int idxoffset;
    int size[CC_MAX_DIM];
}
CvSparseMat;

#define CC_IS_SPARSE_MAT_HDR(mat) \
    ((mat) != NULL && \
    (((const CvSparseMat*)(mat))->tid & CC_MAGIC_MASK) == CC_SPARSE_MAT_MAGIC_VAL)

#define CC_IS_SPARSE_MAT(mat) \
    CC_IS_SPARSE_MAT_HDR(mat)

/**************** iteration through a sparse array *****************/

typedef struct CvSparseNode
{
    unsigned hashval;
    struct CvSparseNode* next;
}
CvSparseNode;

typedef struct CvSparseMatIterator
{
    CvSparseMat* mat;
    CvSparseNode* node;
    int curidx;
}
CvSparseMatIterator;

#define CC_NODE_VAL(mat,node)   ((void*)((uchar*)(node) + (mat)->valoffset))
#define CC_NODE_IDX(mat,node)   ((int*)((uchar*)(node) + (mat)->idxoffset))

/****************************************************************************************\
*                                         Histogram                                      *
\****************************************************************************************/

typedef int CvHistType;

#define CC_HIST_MAGIC_VAL     0x42450000
#define CC_HIST_UNIFORM_FLAG  (1 << 10)

/* indicates whether bin ranges are set already or not */
#define CC_HIST_RANGES_FLAG   (1 << 11)

#define CC_HIST_ARRAY         0
#define CC_HIST_SPARSE        1
#define CC_HIST_TREE          CC_HIST_SPARSE

/* should be used as a parameter only,
   it turns to CC_HIST_UNIFORM_FLAG of hist->tid */
#define CC_HIST_UNIFORM       1

typedef struct CvHistogram
{
    TypeId type;
    img_t*  bins;
    float   thresh[CC_MAX_DIM][2]; /* for uniform histograms */
    float** thresh2; /* for non-uniform histograms */
    CvMatND mat; /* embedded matrix header for array histograms */
}
CvHistogram;

#define CC_IS_HIST(hist) \
    ((hist) != NULL  && \
     (((CvHistogram*)(hist))->tid & CC_MAGIC_MASK) == CC_HIST_MAGIC_VAL && \
     (hist)->bins != NULL)

#define CC_IS_UNIFORM_HIST(hist) \
    (((hist)->tid & CC_HIST_UNIFORM_FLAG) != 0)

#define CC_IS_SPARSE_HIST(hist) \
    CC_IS_SPARSE_MAT((hist)->bins)

#define CC_HIST_HAS_RANGES(hist) \
    (((hist)->tid & CC_HIST_RANGES_FLAG) != 0)

/****************************************************************************************\
*                      Other supplementary data type definitions                         *
\****************************************************************************************/

/*************************************** CRect *****************************************/

typedef struct CRect
{
    int x;
    int y;
    int width;
    int height;
}
CRect;

CC_INLINE  CRect  cRect(int x, int y, int width, int height)
{
    CRect r;

    r.x = x;
    r.y = y;
    r.width = width;
    r.height = height;

    return r;
}


CC_INLINE  IplROI  cvRectToROI(CRect rect, int coi)
{
    IplROI roi;
    roi.xOffset = rect.x;
    roi.yOffset = rect.y;
    roi.width = rect.width;
    roi.height = rect.height;
    roi.coi = coi;

    return roi;
}


CC_INLINE  CRect  cvROIToRect(IplROI roi)
{
    return cRect(roi.xOffset, roi.yOffset, roi.width, roi.height);
}

/*********************************** CTermCriteria *************************************/

#define CC_TERMCRIT_ITER    1
#define CC_TERMCRIT_NUMBER  CC_TERMCRIT_ITER
#define CC_TERMCRIT_EPS     2

typedef struct CTermCriteria
{
    int    type;  /* may be combination of
                     CC_TERMCRIT_ITER
                     CC_TERMCRIT_EPS */
    int    max_iter;
    double epsilon;
}
CTermCriteria;

CC_INLINE  CTermCriteria  cTermCriteria(int type, int max_iter, double epsilon)
{
    CTermCriteria t;

    t.type = type;
    t.max_iter = max_iter;
    t.epsilon = (float)epsilon;

    return t;
}


/******************************* CPoint and variants ***********************************/

typedef struct CPoint
{
    int x;
    int y;
}
CPoint;


CC_INLINE  CPoint  cPoint(int x, int y)
{
    CPoint p;

    p.x = x;
    p.y = y;

    return p;
}


typedef struct CPoint2D32f
{
    float x;
    float y;
}
CPoint2D32f;


CC_INLINE  CPoint2D32f  cPoint2D32f(double x, double y)
{
    CPoint2D32f p;

    p.x = (float)x;
    p.y = (float)y;

    return p;
}


CC_INLINE  CPoint2D32f  cvPointTo32f(CPoint point)
{
    return cPoint2D32f((float)point.x, (float)point.y);
}


CC_INLINE  CPoint  cvPointFrom32f(CPoint2D32f point)
{
    CPoint ipt;
    ipt.x = cRound(point.x);
    ipt.y = cRound(point.y);

    return ipt;
}


typedef struct CPoint3D32f
{
    float x;
    float y;
    float z;
}
CPoint3D32f;


CC_INLINE  CPoint3D32f  cvPoint3D32f(double x, double y, double z)
{
    CPoint3D32f p;

    p.x = (float)x;
    p.y = (float)y;
    p.z = (float)z;

    return p;
}


typedef struct CPoint2D64f
{
    double x;
    double y;
}
CPoint2D64f;


CC_INLINE  CPoint2D64f  cvPoint2D64f(double x, double y)
{
    CPoint2D64f p;

    p.x = x;
    p.y = y;

    return p;
}


typedef struct CPoint3D64f
{
    double x;
    double y;
    double z;
}
CPoint3D64f;


CC_INLINE  CPoint3D64f  cvPoint3D64f(double x, double y, double z)
{
    CPoint3D64f p;

    p.x = x;
    p.y = y;
    p.z = z;

    return p;
}


/******************************** CSize's & CvBox **************************************/

typedef struct
{
    int width;
    int height;
}
CSize;

CC_INLINE  CSize  cSize(int width, int height)
{
    CSize s;

    s.width = width;
    s.height = height;

    return s;
}

typedef struct CSize2D32f
{
    float width;
    float height;
}
CSize2D32f;


CC_INLINE  CSize2D32f  cSize2D32f(double width, double height)
{
    CSize2D32f s;

    s.width = (float)width;
    s.height = (float)height;

    return s;
}

typedef struct CBox2D
{
    CPoint2D32f center;  /* center of the box */
    CSize2D32f  size;    /* box width and length */
    float angle;          /* angle between the horizontal axis
                             and the first side (i.e. length) in degrees */
}
CBox2D;


/* Line iterator state */
typedef struct CLineIterator
{
    /* pointer to the current point */
    uchar* ptr;

    /* Bresenham algorithm state */
    int  err;
    int  plus_delta;
    int  minus_delta;
    int  plus_step;
    int  minus_step;
}
CLineIterator;



/************************************* CSlice ******************************************/

typedef struct CSlice
{
    int  start_index, end_index;
}
CSlice;

CC_INLINE  CSlice  cSlice(int start, int end)
{
    CSlice slice;
    slice.start_index = start;
    slice.end_index = end;

    return slice;
}

#define CC_WHOLE_SEQ_END_INDEX 0x3fffffff
#define CC_WHOLE_SEQ  cSlice(0, CC_WHOLE_SEQ_END_INDEX)


/************************************* CScalar *****************************************/

typedef struct CScalar
{
    double val[4];
}
CScalar;

CC_INLINE  CScalar  cScalar(double val0, double val1 CC_DEFAULT(0),
                               double val2 CC_DEFAULT(0), double val3 CC_DEFAULT(0))
{
    CScalar scalar;
    scalar.val[0] = val0; scalar.val[1] = val1;
    scalar.val[2] = val2; scalar.val[3] = val3;
    return scalar;
}


CC_INLINE  CScalar  cRealScalar(double val0)
{
    CScalar scalar;
    scalar.val[0] = val0;
    scalar.val[1] = scalar.val[2] = scalar.val[3] = 0;
    return scalar;
}

CC_INLINE  CScalar  cScalarAll(double val0123)
{
    CScalar scalar;
    scalar.val[0] = val0123;
    scalar.val[1] = val0123;
    scalar.val[2] = val0123;
    scalar.val[3] = val0123;
    return scalar;
}

/****************************************************************************************\
*                                   Dynamic Data structures                              *
\****************************************************************************************/

/******************************** Memory storage ****************************************/

typedef struct CvMemBlock
{
    struct CvMemBlock*  prev;
    struct CvMemBlock*  next;
}
CvMemBlock;

#define CC_STORAGE_MAGIC_VAL    0x42890000

typedef struct CvMemStorage
{
    int signature;
    CvMemBlock* bottom;/* first allocated block */
    CvMemBlock* top;   /* current memory block - top of the stack */
    struct  CvMemStorage* parent; /* borrows new blocks from */
    int block_size;  /* block size */
    int free_space;  /* free space in the current block */
}
CvMemStorage;

#define CC_IS_STORAGE(storage)  \
    ((storage) != NULL &&       \
    (((CvMemStorage*)(storage))->signature & CC_MAGIC_MASK) == CC_STORAGE_MAGIC_VAL)


typedef struct CvMemStoragePos
{
    CvMemBlock* top;
    int free_space;
}
CvMemStoragePos;


/*********************************** Sequence *******************************************/

typedef struct CvSeqBlock
{
    struct CvSeqBlock*  prev; /* previous sequence block */
    struct CvSeqBlock*  next; /* next sequence block */
    int    start_index;       /* index of the first element in the block +
                                 sequence->first->start_index */
    int    count;             /* number of elements in the block */
    char*  data;              /* pointer to the first element of the block */
}
CvSeqBlock;


#define CC_TREE_NODE_FIELDS(node_type)                          \
    int       flags;         /* micsellaneous flags */          \
    int       header_size;   /* size of sequence header */      \
    struct    node_type* h_prev; /* previous sequence */        \
    struct    node_type* h_next; /* next sequence */            \
    struct    node_type* v_prev; /* 2nd previous sequence */    \
    struct    node_type* v_next  /* 2nd next sequence */

/*
   Read/Write sequence.
   Elements can be dynamically inserted to or deleted from the sequence.
*/
#define CC_SEQUENCE_FIELDS()                                            \
    CC_TREE_NODE_FIELDS(CvSeq);                                         \
    int       total;          /* total number of elements */            \
    int       elem_size;      /* size of sequence element in bytes */   \
    char*     block_max;      /* maximal bound of the last block */     \
    char*     ptr;            /* current write pointer */               \
    int       delta_elems;    /* how many elements allocated when the seq grows */  \
    CvMemStorage* storage;    /* where the seq is stored */             \
    CvSeqBlock* free_blocks;  /* free blocks list */                    \
    CvSeqBlock* first; /* pointer to the first sequence block */

typedef struct CvSeq
{
    CC_SEQUENCE_FIELDS()
}
CvSeq;

#define CC_TYPE_NAME_SEQ             "opencv-sequence"
#define CC_TYPE_NAME_SEQ_TREE        "opencv-sequence-tree"

/*************************************** Set ********************************************/
/*
  Set.
  Order is not preserved. There can be gaps between sequence elements.
  After the element has been inserted it stays in the same place all the time.
  The MSB(most-significant or sign bit) of the first field (flags) is 0 iff the element exists.
*/
#define CC_SET_ELEM_FIELDS(elem_type)   \
    int  flags;                         \
    struct elem_type* next_free;

typedef struct CvSetElem
{
    CC_SET_ELEM_FIELDS(CvSetElem)
}
CvSetElem;

#define CC_SET_FIELDS()      \
    CC_SEQUENCE_FIELDS()     \
    CvSetElem* free_elems;   \
    int active_count;

typedef struct CvSet
{
    CC_SET_FIELDS()
}
CvSet;


#define CC_SET_ELEM_IDX_MASK   ((1 << 26) - 1)
#define CC_SET_ELEM_FREE_FLAG  (1 << (sizeof(int)*8-1))

/* Checks whether the element pointed by ptr belongs to a set or not */
#define CC_IS_SET_ELEM(ptr)  (((CvSetElem*)(ptr))->flags >= 0)

/************************************* Graph ********************************************/

/*
  Graph is represented as a set of vertices.
  Vertices contain their adjacency lists (more exactly, pointers to first incoming or
  outcoming edge (or 0 if isolated vertex)). Edges are stored in another set.
  There is a single-linked list of incoming/outcoming edges for each vertex.

  Each edge consists of:
    two pointers to the starting and the ending vertices (vtx[0] and vtx[1],
    respectively). Graph may be oriented or not. In the second case, edges between
    vertex i to vertex j are not distingueshed (during the search operations).

    two pointers to next edges for the starting and the ending vertices.
    next[0] points to the next edge in the vtx[0] adjacency list and
    next[1] points to the next edge in the vtx[1] adjacency list.
*/
#define CC_GRAPH_EDGE_FIELDS()      \
    int flags;                      \
    float weight;                   \
    struct CvGraphEdge* next[2];    \
    struct CvGraphVtx* vtx[2];


#define CC_GRAPH_VERTEX_FIELDS()    \
    int flags;                      \
    struct CvGraphEdge* first;


typedef struct CvGraphEdge
{
    CC_GRAPH_EDGE_FIELDS()
}
CvGraphEdge;

typedef struct CvGraphVtx
{
    CC_GRAPH_VERTEX_FIELDS()
}
CvGraphVtx;

typedef struct CvGraphVtx2D
{
    CC_GRAPH_VERTEX_FIELDS()
    CPoint2D32f* ptr;
}
CvGraphVtx2D;

/*
   Graph is "derived" from the set (this is set a of vertices)
   and includes another set (edges)
*/
#define  CC_GRAPH_FIELDS()   \
    CC_SET_FIELDS()          \
    CvSet* edges;

typedef struct CvGraph
{
    CC_GRAPH_FIELDS()
}
CvGraph;

#define CC_TYPE_NAME_GRAPH "opencv-graph"

/*********************************** Chain/Countour *************************************/

typedef struct CChain
{
    CC_SEQUENCE_FIELDS()
    CPoint  origin;
}
CChain;

#define CC_CONTOUR_FIELDS()  \
    CC_SEQUENCE_FIELDS()     \
    CRect rect;             \
    int color;               \
    int reserved[3];

typedef struct CvContour
{
    CC_CONTOUR_FIELDS()
}
CvContour;

typedef CvContour CvPoint2DSeq;

/****************************************************************************************\
*                                    Sequence types                                      *
\****************************************************************************************/

#define CC_SEQ_MAGIC_VAL             0x42990000

#define CC_IS_SEQ(seq) \
    ((seq) != NULL && (((CvSeq*)(seq))->flags & CC_MAGIC_MASK) == CC_SEQ_MAGIC_VAL)

#define CC_SET_MAGIC_VAL             0x42980000
#define CC_IS_SET(set) \
    ((set) != NULL && (((CvSeq*)(set))->flags & CC_MAGIC_MASK) == CC_SET_MAGIC_VAL)

#define CC_SEQ_ELTYPE_BITS           9
#define CC_SEQ_ELTYPE_MASK           ((1 << CC_SEQ_ELTYPE_BITS) - 1)

#define CC_SEQ_ELTYPE_POINT          CC_32SC2  /* (x,y) */
#define CC_SEQ_ELTYPE_CODE           CC_8UC1   /* freeman code: 0..7 */
#define CC_SEQ_ELTYPE_GENERIC        0
#define CC_SEQ_ELTYPE_PTR            CC_USRTYPE1
#define CC_SEQ_ELTYPE_PPOINT         CC_SEQ_ELTYPE_PTR  /* &(x,y) */
#define CC_SEQ_ELTYPE_INDEX          CC_32SC1  /* #(x,y) */
#define CC_SEQ_ELTYPE_GRAPH_EDGE     0  /* &next_o, &next_d, &vtx_o, &vtx_d */
#define CC_SEQ_ELTYPE_GRAPH_VERTEX   0  /* first_edge, &(x,y) */
#define CC_SEQ_ELTYPE_TRIAN_ATR      0  /* vertex of the binary tree   */
#define CC_SEQ_ELTYPE_CONNECTED_COMP 0  /* connected component  */
#define CC_SEQ_ELTYPE_POINT3D        CC_32FC3  /* (x,y,z)  */

#define CC_SEQ_KIND_BITS        3
#define CC_SEQ_KIND_MASK        (((1 << CC_SEQ_KIND_BITS) - 1)<<CC_SEQ_ELTYPE_BITS)

/* types of sequences */
#define CC_SEQ_KIND_GENERIC     (0 << CC_SEQ_ELTYPE_BITS)
#define CC_SEQ_KIND_CURVE       (1 << CC_SEQ_ELTYPE_BITS)
#define CC_SEQ_KIND_BIN_TREE    (2 << CC_SEQ_ELTYPE_BITS)

/* types of sparse sequences (sets) */
#define CC_SEQ_KIND_GRAPH       (3 << CC_SEQ_ELTYPE_BITS)
#define CC_SEQ_KIND_SUBDIV2D    (4 << CC_SEQ_ELTYPE_BITS)

#define CC_SEQ_FLAG_SHIFT       (CC_SEQ_KIND_BITS + CC_SEQ_ELTYPE_BITS)

/* flags for curves */
#define CC_SEQ_FLAG_CLOSED     (1 << CC_SEQ_FLAG_SHIFT)
#define CC_SEQ_FLAG_SIMPLE     (2 << CC_SEQ_FLAG_SHIFT)
#define CC_SEQ_FLAG_CONVEX     (4 << CC_SEQ_FLAG_SHIFT)
#define CC_SEQ_FLAG_HOLE       (8 << CC_SEQ_FLAG_SHIFT)

/* flags for graphs */
#define CC_GRAPH_FLAG_ORIENTED (1 << CC_SEQ_FLAG_SHIFT)

#define CC_GRAPH               CC_SEQ_KIND_GRAPH
#define CC_ORIENTED_GRAPH      (CC_SEQ_KIND_GRAPH|CC_GRAPH_FLAG_ORIENTED)

/* point sets */
#define CC_SEQ_POINT_SET       (CC_SEQ_KIND_GENERIC| CC_SEQ_ELTYPE_POINT)
#define CC_SEQ_POINT3D_SET     (CC_SEQ_KIND_GENERIC| CC_SEQ_ELTYPE_POINT3D)
#define CC_SEQ_POLYLINE        (CC_SEQ_KIND_CURVE  | CC_SEQ_ELTYPE_POINT)
#define CC_SEQ_POLYGON         (CC_SEQ_FLAG_CLOSED | CC_SEQ_POLYLINE)
#define CC_SEQ_CONTOUR         CC_SEQ_POLYGON
#define CC_SEQ_SIMPLE_POLYGON  (CC_SEQ_FLAG_SIMPLE | CC_SEQ_POLYGON)

/* chain-coded curves */
#define CC_SEQ_CHAIN           (CC_SEQ_KIND_CURVE  | CC_SEQ_ELTYPE_CODE)
#define CC_SEQ_CHAIN_CONTOUR   (CC_SEQ_FLAG_CLOSED | CC_SEQ_CHAIN)

/* binary tree for the contour */
#define CC_SEQ_POLYGON_TREE    (CC_SEQ_KIND_BIN_TREE  | CC_SEQ_ELTYPE_TRIAN_ATR)

/* sequence of the connected components */
#define CC_SEQ_CONNECTED_COMP  (CC_SEQ_KIND_GENERIC  | CC_SEQ_ELTYPE_CONNECTED_COMP)

/* sequence of the integer numbers */
#define CC_SEQ_INDEX           (CC_SEQ_KIND_GENERIC  | CC_SEQ_ELTYPE_INDEX)

#define CC_SEQ_ELTYPE(seq)   ((seq)->flags & CC_SEQ_ELTYPE_MASK)
#define CC_SEQ_KIND(seq)     ((seq)->flags & CC_SEQ_KIND_MASK)

/* flag checking */
#define CC_IS_SEQ_INDEX(seq)      ((CC_SEQ_ELTYPE(seq) == CC_SEQ_ELTYPE_INDEX) && \
                                     (CC_SEQ_KIND(seq) == CC_SEQ_KIND_GENERIC))

#define CC_IS_SEQ_CURVE(seq)      (CC_SEQ_KIND(seq) == CC_SEQ_KIND_CURVE)
#define CC_IS_SEQ_CLOSED(seq)     (((seq)->flags & CC_SEQ_FLAG_CLOSED) != 0)
#define CC_IS_SEQ_CONVEX(seq)     (((seq)->flags & CC_SEQ_FLAG_CONVEX) != 0)
#define CC_IS_SEQ_HOLE(seq)       (((seq)->flags & CC_SEQ_FLAG_HOLE) != 0)
#define CC_IS_SEQ_SIMPLE(seq)     ((((seq)->flags & CC_SEQ_FLAG_SIMPLE) != 0) || \
                                    CC_IS_SEQ_CONVEX(seq))

/* type checking macros */
#define CC_IS_SEQ_POINT_SET(seq) \
    ((CC_SEQ_ELTYPE(seq) == CC_32SC2 || CC_SEQ_ELTYPE(seq) == CC_32FC2))

#define CC_IS_SEQ_POINT_SUBSET(seq) \
    (CC_IS_SEQ_INDEX(seq) || CC_SEQ_ELTYPE(seq) == CC_SEQ_ELTYPE_PPOINT)

#define CC_IS_SEQ_POLYLINE(seq)   \
    (CC_SEQ_KIND(seq) == CC_SEQ_KIND_CURVE && CC_IS_SEQ_POINT_SET(seq))

#define CC_IS_SEQ_POLYGON(seq)   \
    (CC_IS_SEQ_POLYLINE(seq) && CC_IS_SEQ_CLOSED(seq))

#define CC_IS_SEQ_CHAIN(seq)   \
    (CC_SEQ_KIND(seq) == CC_SEQ_KIND_CURVE && (seq)->elem_size == 1)

#define CC_IS_SEQ_CONTOUR(seq)   \
    (CC_IS_SEQ_CLOSED(seq) && (CC_IS_SEQ_POLYLINE(seq) || CC_IS_SEQ_CHAIN(seq)))

#define CC_IS_SEQ_CHAIN_CONTOUR(seq) \
    (CC_IS_SEQ_CHAIN(seq) && CC_IS_SEQ_CLOSED(seq))

#define CC_IS_SEQ_POLYGON_TREE(seq) \
    (CC_SEQ_ELTYPE (seq) ==  CC_SEQ_ELTYPE_TRIAN_ATR &&    \
    CC_SEQ_KIND(seq) ==  CC_SEQ_KIND_BIN_TREE)

#define CC_IS_GRAPH(seq)    \
    (CC_IS_SET(seq) && CC_SEQ_KIND((CvSet*)(seq)) == CC_SEQ_KIND_GRAPH)

#define CC_IS_GRAPH_ORIENTED(seq)   \
    (((seq)->flags & CC_GRAPH_FLAG_ORIENTED) != 0)

#define CC_IS_SUBDIV2D(seq)  \
    (CC_IS_SET(seq) && CC_SEQ_KIND((CvSet*)(seq)) == CC_SEQ_KIND_SUBDIV2D)

/****************************************************************************************/
/*                            Sequence writer & reader                                  */
/****************************************************************************************/

#define CC_SEQ_WRITER_FIELDS()                                     \
    int          header_size;                                      \
    CvSeq*       seq;        /* the sequence written */            \
    CvSeqBlock*  block;      /* current block */                   \
    char*        ptr;        /* pointer to free space */           \
    char*        block_min;  /* pointer to the beginning of block*/\
    char*        block_max;  /* pointer to the end of block */

typedef struct CvSeqWriter
{
    CC_SEQ_WRITER_FIELDS()
}
CvSeqWriter;


#define CC_SEQ_READER_FIELDS()                                      \
    int          header_size;                                       \
    CvSeq*       seq;        /* sequence, beign read */             \
    CvSeqBlock*  block;      /* current block */                    \
    char*        ptr;        /* pointer to element be read next */  \
    char*        block_min;  /* pointer to the beginning of block */\
    char*        block_max;  /* pointer to the end of block */      \
    int          delta_index;/* = seq->first->start_index   */      \
    char*        prev_elem;  /* pointer to previous element */


typedef struct CvSeqReader
{
    CC_SEQ_READER_FIELDS()
}
CvSeqReader;

/****************************************************************************************/
/*                                Operations on sequences                               */
/****************************************************************************************/

#define  CC_SEQ_ELEM(seq, elem_type, index)                    \
/* assert gives some guarantee that <seq> parameter is valid */  \
(assert(sizeof((seq)->first[0]) == sizeof(CvSeqBlock) &&      \
    (seq)->elem_size == sizeof(elem_type)),                      \
    (elem_type*)((seq)->first && (unsigned)index <               \
    (unsigned)((seq)->first->count) ?                            \
    (seq)->first->data + (index) * sizeof(elem_type) :           \
    cvGetSeqElem((CvSeq*)(seq), (index))))
#define CC_GET_SEQ_ELEM(elem_type, seq, index) CC_SEQ_ELEM((seq), elem_type, (index))

/* macro that adds element to sequence */
#define CC_WRITE_SEQ_ELEM_VAR(elem_ptr, writer)     \
{                                                     \
    if((writer).ptr >= (writer).block_max)          \
    {                                                 \
        cvCreateSeqBlock(&writer);                   \
    }                                                 \
    memcpy((writer).ptr, elem_ptr, (writer).seq->elem_size);\
    (writer).ptr += (writer).seq->elem_size;          \
}

#define CC_WRITE_SEQ_ELEM(elem, writer)             \
{                                                     \
    assert((writer).seq->elem_size == sizeof(elem)); \
    if((writer).ptr >= (writer).block_max)          \
    {                                                 \
        cvCreateSeqBlock(&writer);                   \
    }                                                 \
    assert((writer).ptr <= (writer).block_max - sizeof(elem));\
    memcpy((writer).ptr, &(elem), sizeof(elem));      \
    (writer).ptr += sizeof(elem);                     \
}


/* move reader position forward */
#define CC_NEXT_SEQ_ELEM(elem_size, reader)                 \
{                                                             \
    if(((reader).ptr += (elem_size)) >= (reader).block_max) \
    {                                                         \
        cvChangeSeqBlock(&(reader), 1);                     \
    }                                                         \
}


/* move reader position backward */
#define CC_PREV_SEQ_ELEM(elem_size, reader)                \
{                                                            \
    if(((reader).ptr -= (elem_size)) < (reader).block_min) \
    {                                                        \
        cvChangeSeqBlock(&(reader), -1);                   \
    }                                                        \
}

/* read element and move read position forward */
#define CC_READ_SEQ_ELEM(elem, reader)                       \
{                                                              \
    assert((reader).seq->elem_size == sizeof(elem));          \
    memcpy(&(elem), (reader).ptr, sizeof((elem)));            \
    CC_NEXT_SEQ_ELEM(sizeof(elem), reader)                   \
}

/* read element and move read position backward */
#define CC_REV_READ_SEQ_ELEM(elem, reader)                     \
{                                                                \
    assert((reader).seq->elem_size == sizeof(elem));            \
    memcpy(&(elem), (reader).ptr, sizeof((elem)));               \
    CC_PREV_SEQ_ELEM(sizeof(elem), reader)                     \
}


#define CC_READ_CHAIN_POINT(_pt, reader)                              \
{                                                                       \
    (_pt) = (reader).pt;                                                \
    if((reader).ptr)                                                  \
    {                                                                   \
        CC_READ_SEQ_ELEM((reader).code, (reader));                     \
        assert(((reader).code & ~7) == 0);                            \
        (reader).pt.x += (reader).deltas[(int)(reader).code][0];        \
        (reader).pt.y += (reader).deltas[(int)(reader).code][1];        \
    }                                                                   \
}

#define CC_CURRENT_POINT(reader)  (*((CPoint*)((reader).ptr)))
#define CC_PREV_POINT(reader)     (*((CPoint*)((reader).prev_elem)))

#define CC_READ_EDGE(pt1, pt2, reader)               \
{                                                      \
    assert(sizeof(pt1) == sizeof(CPoint) &&          \
            sizeof(pt2) == sizeof(CPoint) &&          \
            reader.seq->elem_size == sizeof(CPoint)); \
    (pt1) = CC_PREV_POINT(reader);                   \
    (pt2) = CC_CURRENT_POINT(reader);                \
    (reader).prev_elem = (reader).ptr;                 \
    CC_NEXT_SEQ_ELEM(sizeof(CPoint), (reader));      \
}

/************ Graph macros ************/

/* returns next graph edge for given vertex */
#define  CC_NEXT_GRAPH_EDGE(edge, vertex)                              \
     (assert((edge)->vtx[0] == (vertex) || (edge)->vtx[1] == (vertex)),  \
      (edge)->next[(edge)->vtx[1] == (vertex)])



/****************************************************************************************\
*             Data structures for persistence (a.k.a serialization) functionality        *
\****************************************************************************************/

/* "black box" file storage */
typedef struct CvFileStorage CvFileStorage;

/* storage flags */
#define CC_STORAGE_READ          0
#define CC_STORAGE_WRITE         1
#define CC_STORAGE_WRITE_TEXT    CC_STORAGE_WRITE
#define CC_STORAGE_WRITE_BINARY  CC_STORAGE_WRITE
#define CC_STORAGE_APPEND        2

/* list of attributes */
typedef struct CvAttrList
{
    const char** attr; /* NULL-terminated array of (attribute_name,attribute_value) pairs */
    struct CvAttrList* next; /* pointer to next chunk of the attributes list */
}
CvAttrList;

CC_INLINE CvAttrList cvAttrList(const char** attr CC_DEFAULT(NULL),
                                 CvAttrList* next CC_DEFAULT(NULL))
{
    CvAttrList l;
    l.attr = attr;
    l.next = next;

    return l;
}

struct CvTypeInfo;

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

/* file node flags */
#define CC_NODE_FLOW        8 /* used only for writing structures to YAML format */
#define CC_NODE_USER        16
#define CC_NODE_EMPTY       32
#define CC_NODE_NAMED       64

#define CC_NODE_IS_INT(flags)        (CC_NODE_TYPE(flags) == CC_NODE_INT)
#define CC_NODE_IS_REAL(flags)       (CC_NODE_TYPE(flags) == CC_NODE_REAL)
#define CC_NODE_IS_STRING(flags)     (CC_NODE_TYPE(flags) == CC_NODE_STRING)
#define CC_NODE_IS_SEQ(flags)        (CC_NODE_TYPE(flags) == CC_NODE_SEQ)
#define CC_NODE_IS_MAP(flags)        (CC_NODE_TYPE(flags) == CC_NODE_MAP)
#define CC_NODE_IS_COLLECTION(flags) (CC_NODE_TYPE(flags) >= CC_NODE_SEQ)
#define CC_NODE_IS_FLOW(flags)       (((flags) & CC_NODE_FLOW) != 0)
#define CC_NODE_IS_EMPTY(flags)      (((flags) & CC_NODE_EMPTY) != 0)
#define CC_NODE_IS_USER(flags)       (((flags) & CC_NODE_USER) != 0)
#define CC_NODE_HAS_NAME(flags)      (((flags) & CC_NODE_NAMED) != 0)

#define CC_NODE_SEQ_SIMPLE 256
#define CC_NODE_SEQ_IS_SIMPLE(seq) (((seq)->flags & CC_NODE_SEQ_SIMPLE) != 0)

typedef struct CvString
{
    int len;
    char* ptr;
}
CvString;

/* all the keys (names) of elements in the readed file storage
   are stored in the hash to speed up the lookup operations */
typedef struct CvStringHashNode
{
    unsigned hashval;
    CvString str;
    struct CvStringHashNode* next;
}
CvStringHashNode;

typedef struct CvGenericHash CvFileNodeHash;

/* basic element of the file storage - scalar or collection */
typedef struct CvFileNode
{
    int tag;
    struct CvTypeInfo* info; /* type information
            (only for user-defined object, for others it is 0) */
    union
    {
        double f; /* scalar floating-point number */
        int i;    /* scalar integer number */
        CvString str; /* text string */
        CvSeq* seq; /* sequence (ordered collection of file nodes) */
        CvFileNodeHash* map; /* map (collection of named file nodes) */
    } data;
}
CvFileNode;

#ifdef __cplusplus
extern "C" {
#endif
typedef int (C_CDECL *CvIsInstanceFunc)(const void* struct_ptr);
typedef void (C_CDECL *CvReleaseFunc)(void** struct_dblptr);
typedef void* (C_CDECL *CvReadFunc)(CvFileStorage* storage, CvFileNode* node);
typedef void (C_CDECL *CvWriteFunc)(CvFileStorage* storage, const char* name,
                                      const void* struct_ptr, CvAttrList attributes);
typedef void* (C_CDECL *CvCloneFunc)(const void* struct_ptr);
#ifdef __cplusplus
}
#endif

typedef struct CvTypeInfo
{
    int flags;
    int header_size;
    struct CvTypeInfo* prev;
    struct CvTypeInfo* next;
    const char* type_name;
    CvIsInstanceFunc is_instance;
    CvReleaseFunc release;
    CvReadFunc read;
    CvWriteFunc write;
    CvCloneFunc clone;
}
CvTypeInfo;


/**** System data types ******/

typedef struct CvPluginFuncInfo
{
    void** func_addr;
    void* default_func_addr;
    const char* func_names;
    int search_modules;
    int loaded_from;
}
CvPluginFuncInfo;

typedef struct CvModuleInfo
{
    struct CvModuleInfo* next;
    const char* name;
    const char* version;
    CvPluginFuncInfo* func_tab;
}
CvModuleInfo;

#endif /*_CXCORE_TYPES_H_*/


