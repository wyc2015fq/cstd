/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                          License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000-2008, Intel Corporation, all rights reserved.
// Copyright (C) 2009, Willow Garage Inc., all rights reserved.
// Copyright (C) 2013, OpenCV Foundation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

#ifndef OPENCC_CORE_TYPES_H
#define OPENCC_CORE_TYPES_H

#ifdef HAVE_IPL
#  ifndef __IMG_H__
#    if defined WIN32 || defined _WIN32
#      include <ipl.h>
#    else
#      include <ipl/ipl.h>
#    endif
#  endif
#elif defined __IMG_H__
#  define HAVE_IPL
#endif

#include "opencv2/core/cvdef.h"

#ifndef SKIP_INCLUDES
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#endif // SKIP_INCLUDES

#if defined WIN32 || defined _WIN32
#  define C_CDECL __cdecl
#  define C_STDCALL __stdcall
#else
#  define C_CDECL
#  define C_STDCALL
#endif

#ifndef CC_DEFAULT
#  ifdef __cplusplus
#    define CC_DEFAULT(val) = val
#  else
#    define CC_DEFAULT(val)
#  endif
#endif

#ifndef CC_EXTERN_C_FUNCPTR
#  ifdef __cplusplus
#    define CC_EXTERN_C_FUNCPTR(x) extern "C" { typedef x; }
#  else
#    define CC_EXTERN_C_FUNCPTR(x) typedef x
#  endif
#endif

#ifndef CVAPI
#  define CVAPI(rettype) CC_EXTERN_C CC_EXPORTS rettype C_CDECL
#endif

#ifndef CC_IMPL
#  define CC_IMPL CC_EXTERN_C
#endif

#ifdef __cplusplus
#  include "opencv2/core.hpp"
#endif

/** @addtogroup core_c
    @{
*/

/** @brief This is the "metatype" used *only* as a function parameter.

It denotes that the function accepts arrays of multiple types, such as img_t*, img_t* or even
CvSeq* sometimes. The particular array type is determined at runtime by analyzing the first 4
bytes of the header. In C++ interface the role of img_t is played by const img_t* and img_t*.
 */
typedef void img_t;

typedef int CVStatus;

/** @see CC_Code */

/****************************************************************************************\
*                             Common macros and inline functions                         *
\****************************************************************************************/

#define CC_SWAP(a,b,t) ((t) = (a), (a) = (b), (b) = (t))

/** min & max without jumps */
#define  CC_IMIN(a, b)  ((a) ^ (((a)^(b)) & (((a) < (b)) - 1)))

#define  CC_IMAX(a, b)  ((a) ^ (((a)^(b)) & (((a) > (b)) - 1)))

/** absolute value without jumps */
#ifndef __cplusplus
#  define  CC_IABS(a)     (((a) ^ ((a) < 0 ? -1 : 0)) - ((a) < 0 ? -1 : 0))
#else
#  define  CC_IABS(a)     abs(a)
#endif
#define  CC_CMP(a,b)    (((a) > (b)) - ((a) < (b)))
#define  CC_SIGN(a)     CC_CMP((a),0)

#define cvInvSqrt(value) ((float)(1./sqrt(value)))
#define cvSqrt(value)  ((float)sqrt(value))


/*************** Random number generation *******************/

typedef uint64 CRNG;

#define CC_RNG_COEFF 4164903690U

/** @brief Initializes a random number generator state.

The function initializes a random number generator and returns the state. The pointer to the state
can be then passed to the cvRandInt, cvRandReal and cvRandArr functions. In the current
implementation a multiply-with-carry generator is used.
@param seed 64-bit value used to initiate a random sequence
@sa the C++ class RNG replaced CRNG.
 */
CC_INLINE CRNG cRNG( int64 seed CC_DEFAULT(-1))
{
    CRNG rng = seed ? (uint64)seed : (uint64)(int64)-1;
    return rng;
}

/** @brief Returns a 32-bit unsigned integer and updates RNG.

The function returns a uniformly-distributed random 32-bit unsigned integer and updates the RNG
state. It is similar to the rand() function from the C runtime library, except that OpenCV functions
always generates a 32-bit random number, regardless of the platform.
@param rng CRNG state initialized by cRNG.
 */
CC_INLINE unsigned cvRandInt( CRNG* rng )
{
    uint64 temp = *rng;
    temp = (uint64)(unsigned)temp*CC_RNG_COEFF + (temp >> 32);
    *rng = temp;
    return (unsigned)temp;
}

/** @brief Returns a floating-point random number and updates RNG.

The function returns a uniformly-distributed random floating-point number between 0 and 1 (1 is not
included).
@param rng RNG state initialized by cRNG
 */
CC_INLINE double cvRandReal( CRNG* rng )
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

/** The img_t is taken from the Intel Image Processing Library, in which the format is native. OpenCV
only supports a subset of possible img_t formats, as outlined in the parameter list above.

In addition to the above restrictions, OpenCV handles ROIs differently. OpenCV functions require
that the image size or ROI size of all source and destination images match exactly. On the other
hand, the Intel Image Processing Library processes the area of intersection between the source and
destination images (or ROIs), allowing them to vary independently.
*/
typedef struct
#ifdef __cplusplus
  CC_EXPORTS
#endif
_IplImage
{
    int  nSize;             /**< sizeof(img_t) */
    int  ID;                /**< version (=0)*/
    int  nChannels;         /**< Most of OpenCV functions support 1,2,3 or 4 channels */
    int  alphaChannel;      /**< Ignored by OpenCV */
    int  depth;             /**< Pixel depth in bits: IMG_DEPTH_8U, IMG_DEPTH_8S, IMG_DEPTH_16S,
                               IMG_DEPTH_32S, IMG_DEPTH_32F and IMG_DEPTH_64F are supported.  */
    char colorModel[4];     /**< Ignored by OpenCV */
    char channelSeq[4];     /**< ditto */
    int  dataOrder;         /**< 0 - interleaved color channels, 1 - separate color channels.
                               cvCreateImage can only create interleaved images */
    int  origin;            /**< 0 - top-left origin,
                               1 - bottom-left origin (Windows bitmaps style).  */
    int  align;             /**< Alignment of image rows (4 or 8).
                               OpenCV ignores it and uses widthStep instead.    */
    int  width;             /**< Image width in pixels.                           */
    int  height;            /**< Image height in pixels.                          */
    struct _IplROI *roi;    /**< Image ROI. If NULL, the whole image is selected. */
    struct _IplImage *maskROI;      /**< Must be NULL. */
    void  *imageId;                 /**< "           " */
    struct _IplTileInfo *tileInfo;  /**< "           " */
    int  imageSize;         /**< Image data size in bytes
                               (==image->height*image->widthStep
                               in case of interleaved data)*/
    char *imageData;        /**< Pointer to aligned image data.         */
    int  widthStep;         /**< Size of aligned image row in bytes.    */
    int  BorderMode[4];     /**< Ignored by OpenCV.                     */
    int  BorderConst[4];    /**< Ditto.                                 */
    char *imageDataOrigin;  /**< Pointer to very origin of image data
                               (not necessarily aligned) -
                               needed for correct deallocation */

#ifdef __cplusplus
    _IplImage() {}
    _IplImage(const img_t& m);
#endif
}
img_t;

typedef struct _IplTileInfo IplTileInfo;

typedef struct _IplROI
{
    int  coi; /**< 0 - no COI (all channels are selected), 1 - 0th channel is selected ...*/
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

/** extra border mode */
#define IMG_BORDER_REFLECT_101    4
#define IMG_BORDER_TRANSPARENT    5

#define IMG_IMAGE_MAGIC_VAL  ((int)sizeof(img_t))
#define CC_TYPE_NAME_IMAGE "opencv-image"

#define CC_IS_IMAGE_HDR(img) \
    ((img) != NULL && ((const img_t*)(img))->nSize == sizeof(img_t))

#define CC_IS_IMAGE(img) \
    (CC_IS_IMAGE_HDR(img) && ((img_t*)img)->imageData != NULL)

/** for storing double-precision
   floating point data in img_t's */
#define IMG_DEPTH_64F  64

/** get reference to pixel at (col,row),
   for multi-channel images (col) should be multiplied by number of channels */
#define CC_IMAGE_ELEM( image, elemtype, row, col )       \
    (((elemtype*)((image)->imageData + (image)->widthStep*(row)))[(col)])

/****************************************************************************************\
*                                  Matrix type (img_t)                                   *
\****************************************************************************************/

#define CC_AUTO_STEP  0x7fffffff
#define CC_WHOLE_ARR  cSlice( 0, 0x3fffffff )

#define CC_MAGIC_MASK       0xFFFF0000
#define CC_MAT_MAGIC_VAL    0x42420000
#define CC_TYPE_NAME_MAT    "opencv-matrix"

/** Matrix elements are stored row by row. Element (i, j) (i - 0-based row index, j - 0-based column
index) of a matrix can be retrieved or modified using CC_MAT_ELEM macro:

    uchar pixval = CC_MAT_ELEM(grayimg, uchar, i, j)
    CC_MAT_ELEM(cameraMatrix, float, 0, 2) = image.width*0.5f;

To access multiple-channel matrices, you can use
CC_MAT_ELEM(matrix, type, i, j\*nchannels + channel_idx).

@deprecated img_t is now obsolete; consider using img_t instead.
 */
typedef struct img_t
{
    int type;
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


#ifdef __cplusplus
    img_t() {}
    img_t(const img_t& m) { memcpy(this, &m, sizeof(img_t));}
    img_t(const img_t& m);
#endif

}
img_t;


#define CC_IS_MAT_HDR(mat) \
    ((mat) != NULL && \
    (((const img_t*)(mat))->tid & CC_MAGIC_MASK) == CC_MAT_MAGIC_VAL && \
    ((const img_t*)(mat))->cols > 0 && ((const img_t*)(mat))->rows > 0)

#define CC_IS_MAT_HDR_Z(mat) \
    ((mat) != NULL && \
    (((const img_t*)(mat))->tid & CC_MAGIC_MASK) == CC_MAT_MAGIC_VAL && \
    ((const img_t*)(mat))->cols >= 0 && ((const img_t*)(mat))->rows >= 0)

#define CC_IS_MAT(mat) \
    (CC_IS_MAT_HDR(mat) && ((const img_t*)(mat))->data.ptr != NULL)

#define CC_IS_MASK_ARR(mat) \
    (((mat)->tid & (CC_MAT_TYPE_MASK & ~CC_8SC1)) == 0)

#define CC_ARE_TYPES_EQ(mat1, mat2) \
    ((((mat1)->tid ^ (mat2)->tid) & CC_MAT_TYPE_MASK) == 0)

#define CC_ARE_CNS_EQ(mat1, mat2) \
    ((((mat1)->tid ^ (mat2)->tid) & CC_MAT_CN_MASK) == 0)

#define CC_ARE_DEPTHS_EQ(mat1, mat2) \
    ((((mat1)->tid ^ (mat2)->tid) & CC_MAT_DEPTH_MASK) == 0)

#define CC_ARE_SIZES_EQ(mat1, mat2) \
    ((mat1)->rows == (mat2)->rows && (mat1)->cols == (mat2)->cols)

#define CC_IS_MAT_CONST(mat)  \
    (((mat)->rows|(mat)->cols) == 1)

#define IPL2CC_DEPTH(depth) \
    ((((CC_8U)+(CC_16U<<4)+(CC_32F<<8)+(CC_64F<<16)+(CC_8S<<20)+ \
    (CC_16S<<24)+(CC_32S<<28)) >> ((((depth) & 0xF0) >> 2) + \
    (((depth) & IMG_DEPTH_SIGN) ? 20 : 0))) & 15)

/** Inline constructor. No data is allocated internally!!!
 * (Use together with cvCreateData, or use cvCreateMat instead to
 * get a matrix with allocated data):
 */
CC_INLINE img_t cvMat( int rows, int cols, int type, void* data CC_DEFAULT(NULL))
{
    img_t m;

    assert( (unsigned)CC_MAT_DEPTH(type) <= CC_64F );
    type = CC_MAT_DEPTH(type);
    m.type = CC_MAT_MAGIC_VAL | CC_MAT_CONT_FLAG | type;
    m.cols = cols;
    m.rows = rows;
    m.step = m.cols*CC_ELEM_SIZE(type);
    m.data.ptr = (uchar*)data;
    m.refcount = NULL;
    m.hdr_refcount = 0;

    return m;
}

#ifdef __cplusplus
inline img_t::img_t(const img_t& m)
{
    CC_DbgAssert(m.dims <= 2);
    *this = cvMat(m.rows, m.dims == 1 ? 1 : m.cols, m CC_MAT_TYPE(), m.data);
    step = (int)m.step[0];
    type = (type & ~img_t::CONTINUOUS_FLAG) | (m.flags & img_t::CONTINUOUS_FLAG);
}
#endif


#define CC_MAT_ELEM_PTR_FAST( mat, row, col, pix_size )  \
    (assert( (unsigned)(row) < (unsigned)(mat).rows &&   \
             (unsigned)(col) < (unsigned)(mat).cols ),   \
     (mat).data.ptr + (size_t)(mat).step*(row) + (pix_size)*(col))

#define CC_MAT_ELEM_PTR( mat, row, col )                 \
    CC_MAT_ELEM_PTR_FAST( mat, row, col, CC_ELEM_SIZE((mat).type) )

#define CC_MAT_ELEM( mat, elemtype, row, col )           \
    (*(elemtype*)CC_MAT_ELEM_PTR_FAST( mat, row, col, sizeof(elemtype)))

/** @brief Returns the particular element of single-channel floating-point matrix.

The function is a fast replacement for cvGetReal2D in the case of single-channel floating-point
matrices. It is faster because it is inline, it does fewer checks for array type and array element
type, and it checks for the row and column ranges only in debug mode.
@param mat Input matrix
@param row The zero-based index of row
@param col The zero-based index of column
 */
CC_INLINE  double  cvmGet( const img_t* mat, int row, int col )
{
    int type;

    type = CC_MAT_DEPTH(mat->tid);
    assert( (unsigned)row < (unsigned)mat->rows &&
            (unsigned)col < (unsigned)mat->cols );

    if( type == CC_32FC1 )
        return ((float*)(void*)(mat->data.ptr + (size_t)mat->step*row))[col];
    else
    {
        assert( type == CC_64FC1 );
        return ((double*)(void*)(mat->data.ptr + (size_t)mat->step*row))[col];
    }
}

/** @brief Sets a specific element of a single-channel floating-point matrix.

The function is a fast replacement for cvSetReal2D in the case of single-channel floating-point
matrices. It is faster because it is inline, it does fewer checks for array type and array element
type, and it checks for the row and column ranges only in debug mode.
@param mat The matrix
@param row The zero-based index of row
@param col The zero-based index of column
@param value The new value of the matrix element
 */
CC_INLINE  void  cvmSet( img_t* mat, int row, int col, double value )
{
    int type;
    type = CC_MAT_DEPTH(mat->tid);
    assert( (unsigned)row < (unsigned)mat->rows &&
            (unsigned)col < (unsigned)mat->cols );

    if( type == CC_32FC1 )
        ((float*)(void*)(mat->data.ptr + (size_t)mat->step*row))[col] = (float)value;
    else
    {
        assert( type == CC_64FC1 );
        ((double*)(void*)(mat->data.ptr + (size_t)mat->step*row))[col] = value;
    }
}


CC_INLINE int cvIplDepth( int type )
{
    int depth = CC_MAT_DEPTH(type);
    return CC_ELEM_SIZE1(depth)*8 | (depth == CC_8S || depth == CC_16S ||
           depth == CC_32S ? IMG_DEPTH_SIGN : 0);
}


/****************************************************************************************\
*                       Multi-dimensional dense array (CvMatND)                          *
\****************************************************************************************/

#define CC_MATND_MAGIC_VAL    0x42430000
#define CC_TYPE_NAME_MATND    "opencv-nd-matrix"

#define CC_MAX_DIM            32
#define CC_MAX_DIM_HEAP       1024

/**
  @deprecated consider using img_t instead
  */
typedef struct
#ifdef __cplusplus
  CC_EXPORTS
#endif
CvMatND
{
    int type;
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

#ifdef __cplusplus
    CvMatND() {}
    CvMatND(const img_t& m);
#endif
}
CvMatND;

#define CC_IS_MATND_HDR(mat) \
    ((mat) != NULL && (((const CvMatND*)(mat))->tid & CC_MAGIC_MASK) == CC_MATND_MAGIC_VAL)

#define CC_IS_MATND(mat) \
    (CC_IS_MATND_HDR(mat) && ((const CvMatND*)(mat))->data.ptr != NULL)


/****************************************************************************************\
*                      Multi-dimensional sparse array (CvSparseMat)                      *
\****************************************************************************************/

#define CC_SPARSE_MAT_MAGIC_VAL    0x42440000
#define CC_TYPE_NAME_SPARSE_MAT    "opencv-sparse-matrix"

struct CvSet;

typedef struct
#ifdef __cplusplus
  CC_EXPORTS
#endif
CvSparseMat
{
    int type;
    int dims;
    int* refcount;
    int hdr_refcount;

    struct CvSet* heap;
    void** hashtable;
    int hashsize;
    int valoffset;
    int idxoffset;
    int size[CC_MAX_DIM];

#ifdef __cplusplus
    void copyToSparseMat(SparseMat& m) const;
#endif
}
CvSparseMat;

#ifdef __cplusplus
    CC_EXPORTS CvSparseMat* cvCreateSparseMat(const SparseMat& m);
#endif

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

/** indicates whether bin ranges are set already or not */
#define CC_HIST_RANGES_FLAG   (1 << 11)

#define CC_HIST_ARRAY         0
#define CC_HIST_SPARSE        1
#define CC_HIST_TREE          CC_HIST_SPARSE

/** should be used as a parameter only,
   it turns to CC_HIST_UNIFORM_FLAG of hist->tid */
#define CC_HIST_UNIFORM       1

typedef struct CvHistogram
{
    int     type;
    img_t*  bins;
    float   thresh[CC_MAX_DIM][2];  /**< For uniform histograms.                      */
    float** thresh2;                /**< For non-uniform histograms.                  */
    CvMatND mat;                    /**< Embedded matrix header for array histograms. */
}
CvHistogram;

#define CC_IS_HIST( hist ) \
    ((hist) != NULL  && \
     (((CvHistogram*)(hist))->tid & CC_MAGIC_MASK) == CC_HIST_MAGIC_VAL && \
     (hist)->bins != NULL)

#define CC_IS_UNIFORM_HIST( hist ) \
    (((hist)->tid & CC_HIST_UNIFORM_FLAG) != 0)

#define CC_IS_SPARSE_HIST( hist ) \
    CC_IS_SPARSE_MAT((hist)->bins)

#define CC_HIST_HAS_RANGES( hist ) \
    (((hist)->tid & CC_HIST_RANGES_FLAG) != 0)

/****************************************************************************************\
*                      Other supplementary data type definitions                         *
\****************************************************************************************/

/*************************************** CRect *****************************************/
/** @sa Rect_ */
typedef struct CRect
{
    int x;
    int y;
    int width;
    int height;

#ifdef __cplusplus
    CRect(int _x = 0, int _y = 0, int w = 0, int h = 0): x(_x), y(_y), width(w), height(h) {}
    template<typename _Tp>
    CRect(const Rect_<_Tp>& r): x(saturate_cast<int>(r.x)), y(saturate_cast<int>(r.y)), width(saturate_cast<int>(r.width)), height(saturate_cast<int>(r.height)) {}
    template<typename _Tp>
    operator Rect_<_Tp>() const { return Rect_<_Tp>((_Tp)x, (_Tp)y, (_Tp)width, (_Tp)height); }
#endif
}
CRect;

/** constructs CRect structure. */
CC_INLINE  CRect  cRect( int x, int y, int width, int height )
{
    CRect r;

    r.x = x;
    r.y = y;
    r.width = width;
    r.height = height;

    return r;
}


CC_INLINE  IplROI  cvRectToROI( CRect rect, int coi )
{
    IplROI roi;
    roi.xOffset = rect.x;
    roi.yOffset = rect.y;
    roi.width = rect.width;
    roi.height = rect.height;
    roi.coi = coi;

    return roi;
}


CC_INLINE  CRect  cvROIToRect( IplROI roi )
{
    return cRect( roi.xOffset, roi.yOffset, roi.width, roi.height );
}

/*********************************** CTermCriteria *************************************/

#define CC_TERMCRIT_ITER    1
#define CC_TERMCRIT_NUMBER  CC_TERMCRIT_ITER
#define CC_TERMCRIT_EPS     2

/** @sa TermCriteria
 */
typedef struct CTermCriteria
{
    int    type;  /**< may be combination of
                     CC_TERMCRIT_ITER
                     CC_TERMCRIT_EPS */
    int    max_iter;
    double epsilon;

#ifdef __cplusplus
    CTermCriteria(int _type = 0, int _iter = 0, double _eps = 0) : type(_type), max_iter(_iter), epsilon(_eps)  {}
    CTermCriteria(const TermCriteria& t) : type(t.type), max_iter(t.maxCount), epsilon(t.epsilon)  {}
    operator TermCriteria() const { return TermCriteria(type, max_iter, epsilon); }
#endif

}
CTermCriteria;

CC_INLINE  CTermCriteria  cTermCriteria( int type, int max_iter, double epsilon )
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

#ifdef __cplusplus
    CPoint(int _x = 0, int _y = 0): x(_x), y(_y) {}
    template<typename _Tp>
    CPoint(const Point_<_Tp>& pt): x((int)pt.x), y((int)pt.y) {}
    template<typename _Tp>
    operator Point_<_Tp>() const { return Point_<_Tp>(saturate_cast<_Tp>(x), saturate_cast<_Tp>(y)); }
#endif
}
CPoint;

/** constructs CPoint structure. */
CC_INLINE  CPoint  cPoint( int x, int y )
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

#ifdef __cplusplus
    CPoint2D32f(float _x = 0, float _y = 0): x(_x), y(_y) {}
    template<typename _Tp>
    CPoint2D32f(const Point_<_Tp>& pt): x((float)pt.x), y((float)pt.y) {}
    template<typename _Tp>
    operator Point_<_Tp>() const { return Point_<_Tp>(saturate_cast<_Tp>(x), saturate_cast<_Tp>(y)); }
#endif
}
CPoint2D32f;

/** constructs CPoint2D32f structure. */
CC_INLINE  CPoint2D32f  cPoint2D32f( double x, double y )
{
    CPoint2D32f p;

    p.x = (float)x;
    p.y = (float)y;

    return p;
}

/** converts CPoint to CPoint2D32f. */
CC_INLINE  CPoint2D32f  cvPointTo32f( CPoint point )
{
    return cPoint2D32f( (float)point.x, (float)point.y );
}

/** converts CPoint2D32f to CPoint. */
CC_INLINE  CPoint  cvPointFrom32f( CPoint2D32f point )
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

#ifdef __cplusplus
    CPoint3D32f(float _x = 0, float _y = 0, float _z = 0): x(_x), y(_y), z(_z) {}
    template<typename _Tp>
    CPoint3D32f(const Point3_<_Tp>& pt): x((float)pt.x), y((float)pt.y), z((float)pt.z) {}
    template<typename _Tp>
    operator Point3_<_Tp>() const { return Point3_<_Tp>(saturate_cast<_Tp>(x), saturate_cast<_Tp>(y), saturate_cast<_Tp>(z)); }
#endif
}
CPoint3D32f;

/** constructs CPoint3D32f structure. */
CC_INLINE  CPoint3D32f  cvPoint3D32f( double x, double y, double z )
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

/** constructs CPoint2D64f structure.*/
CC_INLINE  CPoint2D64f  cvPoint2D64f( double x, double y )
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

/** constructs CPoint3D64f structure. */
CC_INLINE  CPoint3D64f  cvPoint3D64f( double x, double y, double z )
{
    CPoint3D64f p;

    p.x = x;
    p.y = y;
    p.z = z;

    return p;
}


/******************************** CSize's & CvBox **************************************/

typedef struct CSize
{
    int width;
    int height;

#ifdef __cplusplus
    CSize(int w = 0, int h = 0): width(w), height(h) {}
    template<typename _Tp>
    CSize(const Size_<_Tp>& sz): width(saturate_cast<int>(sz.width)), height(saturate_cast<int>(sz.height)) {}
    template<typename _Tp>
    operator Size_<_Tp>() const { return Size_<_Tp>(saturate_cast<_Tp>(width), saturate_cast<_Tp>(height)); }
#endif
}
CSize;

/** constructs CSize structure. */
CC_INLINE  CSize  cSize( int width, int height )
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

#ifdef __cplusplus
    CSize2D32f(float w = 0, float h = 0): width(w), height(h) {}
    template<typename _Tp>
    CSize2D32f(const Size_<_Tp>& sz): width(saturate_cast<float>(sz.width)), height(saturate_cast<float>(sz.height)) {}
    template<typename _Tp>
    operator Size_<_Tp>() const { return Size_<_Tp>(saturate_cast<_Tp>(width), saturate_cast<_Tp>(height)); }
#endif
}
CSize2D32f;

/** constructs CSize2D32f structure. */
CC_INLINE  CSize2D32f  cSize2D32f( double width, double height )
{
    CSize2D32f s;

    s.width = (float)width;
    s.height = (float)height;

    return s;
}

/** @sa RotatedRect
 */
typedef struct CBox2D
{
    CPoint2D32f center;  /**< Center of the box.                          */
    CSize2D32f  size;    /**< Box width and length.                       */
    float angle;          /**< Angle between the horizontal axis           */
                          /**< and the first side (i.e. length) in degrees */

#ifdef __cplusplus
    CBox2D(CPoint2D32f c = CPoint2D32f(), CSize2D32f s = CSize2D32f(), float a = 0) : center(c), size(s), angle(a) {}
    CBox2D(const RotatedRect& rr) : center(rr.center), size(rr.size), angle(rr.angle) {}
    operator RotatedRect() const { return RotatedRect(center, size, angle); }
#endif
}
CBox2D;


/** Line iterator state: */
typedef struct CLineIterator
{
    /** Pointer to the current point: */
    uchar* ptr;

    /* Bresenham algorithm state: */
    int  err;
    int  plus_delta;
    int  minus_delta;
    int  plus_step;
    int  minus_step;
}
CLineIterator;



/************************************* CSlice ******************************************/
#define CC_WHOLE_SEQ_END_INDEX 0x3fffffff
#define CC_WHOLE_SEQ  cSlice(0, CC_WHOLE_SEQ_END_INDEX)

typedef struct CSlice
{
    int  start_index, end_index;

#if defined(__cplusplus) && !defined(__CUDACC__)
    CSlice(int start = 0, int end = 0) : start_index(start), end_index(end) {}
    CSlice(const CRange& r) { *this = (r.start != INT_MIN && r.end != INT_MAX) ? CSlice(r.start, r.end) : CSlice(0, CC_WHOLE_SEQ_END_INDEX); }
    operator CRange() const { return (start_index == 0 && end_index == CC_WHOLE_SEQ_END_INDEX ) ? CRange::all() : CRange(start_index, end_index); }
#endif
}
CSlice;

CC_INLINE  CSlice  cSlice( int start, int end )
{
    CSlice slice;
    slice.start_index = start;
    slice.end_index = end;

    return slice;
}



/************************************* CScalar *****************************************/
/** @sa Scalar_
 */
typedef struct CScalar
{
    double val[4];

#ifdef __cplusplus
    CScalar() {}
    CScalar(double d0, double d1 = 0, double d2 = 0, double d3 = 0) { val[0] = d0; val[1] = d1; val[2] = d2; val[3] = d3; }
    template<typename _Tp>
    CScalar(const Scalar_<_Tp>& s) { val[0] = s.val[0]; val[1] = s.val[1]; val[2] = s.val[2]; val[3] = s.val[3]; }
    template<typename _Tp>
    operator Scalar_<_Tp>() const { return Scalar_<_Tp>(saturate_cast<_Tp>(val[0]), saturate_cast<_Tp>(val[1]), saturate_cast<_Tp>(val[2]), saturate_cast<_Tp>(val[3])); }
    template<typename _Tp, int cn>
    CScalar(const Vec<_Tp, cn>& v)
    {
        int i;
        for( i = 0; i < (cn < 4 ? cn : 4); i++ ) val[i] = v.val[i];
        for( ; i < 4; i++ ) val[i] = 0;
    }
#endif
}
CScalar;

CC_INLINE  CScalar  cScalar( double val0, double val1 CC_DEFAULT(0),
                               double val2 CC_DEFAULT(0), double val3 CC_DEFAULT(0))
{
    CScalar scalar;
    scalar.val[0] = val0; scalar.val[1] = val1;
    scalar.val[2] = val2; scalar.val[3] = val3;
    return scalar;
}


CC_INLINE  CScalar  cRealScalar( double val0 )
{
    CScalar scalar;
    scalar.val[0] = val0;
    scalar.val[1] = scalar.val[2] = scalar.val[3] = 0;
    return scalar;
}

CC_INLINE  CScalar  cScalarAll( double val0123 )
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


/*********************************** Sequence *******************************************/

typedef struct CvSeqBlock
{
    struct CvSeqBlock*  prev; /**< Previous sequence block.                   */
    struct CvSeqBlock*  next; /**< Next sequence block.                       */
  int    start_index;         /**< Index of the first element in the block +  */
                              /**< sequence->first->start_index.              */
    int    count;             /**< Number of elements in the block.           */
    schar* data;              /**< Pointer to the first element of the block. */
}
CvSeqBlock;


#define CC_TREE_NODE_FIELDS(node_type)                               \
    int       flags;             /**< Miscellaneous flags.     */      \
    int       header_size;       /**< Size of sequence header. */      \
    struct    node_type* h_prev; /**< Previous sequence.       */      \
    struct    node_type* h_next; /**< Next sequence.           */      \
    struct    node_type* v_prev; /**< 2nd previous sequence.   */      \
    struct    node_type* v_next  /**< 2nd next sequence.       */

/**
   Read/Write sequence.
   Elements can be dynamically inserted to or deleted from the sequence.
*/
#define CC_SEQUENCE_FIELDS()                                              \
    CC_TREE_NODE_FIELDS(CvSeq);                                           \
    int       total;          /**< Total number of elements.            */  \
    int       elem_size;      /**< Size of sequence element in bytes.   */  \
    schar*    block_max;      /**< Maximal bound of the last block.     */  \
    schar*    ptr;            /**< Current write pointer.               */  \
    int       delta_elems;    /**< Grow seq this many at a time.        */  \
    CvMemStorage* storage;    /**< Where the seq is stored.             */  \
    CvSeqBlock* free_blocks;  /**< Free blocks list.                    */  \
    CvSeqBlock* first;        /**< Pointer to the first sequence block. */

typedef struct CvSeq
{
    CC_SEQUENCE_FIELDS()
}
CvSeq;

#define CC_TYPE_NAME_SEQ             "opencv-sequence"
#define CC_TYPE_NAME_SEQ_TREE        "opencv-sequence-tree"

/*************************************** Set ********************************************/
/** @brief Set
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

/** Checks whether the element pointed by ptr belongs to a set or not */
#define CC_IS_SET_ELEM( ptr )  (((CvSetElem*)(ptr))->flags >= 0)

/************************************* Graph ********************************************/

/** @name Graph

We represent a graph as a set of vertices. Vertices contain their adjacency lists (more exactly,
pointers to first incoming or outcoming edge (or 0 if isolated vertex)). Edges are stored in
another set. There is a singly-linked list of incoming/outcoming edges for each vertex.

Each edge consists of:

- Two pointers to the starting and ending vertices (vtx[0] and vtx[1] respectively).

    A graph may be oriented or not. In the latter case, edges between vertex i to vertex j are not
distinguished during search operations.

- Two pointers to next edges for the starting and ending vertices, where next[0] points to the
next edge in the vtx[0] adjacency list and next[1] points to the next edge in the vtx[1]
adjacency list.

@see CvGraphEdge, CvGraphVtx, CvGraphVtx2D, CvGraph
@{
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

/**
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

/** @} */

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

#define CC_SEQ_ELTYPE_BITS           12
#define CC_SEQ_ELTYPE_MASK           ((1 << CC_SEQ_ELTYPE_BITS) - 1)

#define CC_SEQ_ELTYPE_POINT          CC_32SC2  /**< (x,y) */
#define CC_SEQ_ELTYPE_CODE           CC_8UC1   /**< freeman code: 0..7 */
#define CC_SEQ_ELTYPE_GENERIC        0
#define CC_SEQ_ELTYPE_PTR            CC_USRTYPE1
#define CC_SEQ_ELTYPE_PPOINT         CC_SEQ_ELTYPE_PTR  /**< &(x,y) */
#define CC_SEQ_ELTYPE_INDEX          CC_32SC1  /**< #(x,y) */
#define CC_SEQ_ELTYPE_GRAPH_EDGE     0  /**< &next_o, &next_d, &vtx_o, &vtx_d */
#define CC_SEQ_ELTYPE_GRAPH_VERTEX   0  /**< first_edge, &(x,y) */
#define CC_SEQ_ELTYPE_TRIAN_ATR      0  /**< vertex of the binary tree   */
#define CC_SEQ_ELTYPE_CONNECTED_COMP 0  /**< connected component  */
#define CC_SEQ_ELTYPE_POINT3D        CC_32FC3  /**< (x,y,z)  */

#define CC_SEQ_KIND_BITS        2
#define CC_SEQ_KIND_MASK        (((1 << CC_SEQ_KIND_BITS) - 1)<<CC_SEQ_ELTYPE_BITS)

/** types of sequences */
#define CC_SEQ_KIND_GENERIC     (0 << CC_SEQ_ELTYPE_BITS)
#define CC_SEQ_KIND_CURVE       (1 << CC_SEQ_ELTYPE_BITS)
#define CC_SEQ_KIND_BIN_TREE    (2 << CC_SEQ_ELTYPE_BITS)

/** types of sparse sequences (sets) */
#define CC_SEQ_KIND_GRAPH       (1 << CC_SEQ_ELTYPE_BITS)
#define CC_SEQ_KIND_SUBDIV2D    (2 << CC_SEQ_ELTYPE_BITS)

#define CC_SEQ_FLAG_SHIFT       (CC_SEQ_KIND_BITS + CC_SEQ_ELTYPE_BITS)

/** flags for curves */
#define CC_SEQ_FLAG_CLOSED     (1 << CC_SEQ_FLAG_SHIFT)
#define CC_SEQ_FLAG_SIMPLE     (0 << CC_SEQ_FLAG_SHIFT)
#define CC_SEQ_FLAG_CONVEX     (0 << CC_SEQ_FLAG_SHIFT)
#define CC_SEQ_FLAG_HOLE       (2 << CC_SEQ_FLAG_SHIFT)

/** flags for graphs */
#define CC_GRAPH_FLAG_ORIENTED (1 << CC_SEQ_FLAG_SHIFT)

#define CC_GRAPH               CC_SEQ_KIND_GRAPH
#define CC_ORIENTED_GRAPH      (CC_SEQ_KIND_GRAPH|CC_GRAPH_FLAG_ORIENTED)

/** point sets */
#define CC_SEQ_POINT_SET       (CC_SEQ_KIND_GENERIC| CC_SEQ_ELTYPE_POINT)
#define CC_SEQ_POINT3D_SET     (CC_SEQ_KIND_GENERIC| CC_SEQ_ELTYPE_POINT3D)
#define CC_SEQ_POLYLINE        (CC_SEQ_KIND_CURVE  | CC_SEQ_ELTYPE_POINT)
#define CC_SEQ_POLYGON         (CC_SEQ_FLAG_CLOSED | CC_SEQ_POLYLINE )
#define CC_SEQ_CONTOUR         CC_SEQ_POLYGON
#define CC_SEQ_SIMPLE_POLYGON  (CC_SEQ_FLAG_SIMPLE | CC_SEQ_POLYGON  )

/** chain-coded curves */
#define CC_SEQ_CHAIN           (CC_SEQ_KIND_CURVE  | CC_SEQ_ELTYPE_CODE)
#define CC_SEQ_CHAIN_CONTOUR   (CC_SEQ_FLAG_CLOSED | CC_SEQ_CHAIN)

/** binary tree for the contour */
#define CC_SEQ_POLYGON_TREE    (CC_SEQ_KIND_BIN_TREE  | CC_SEQ_ELTYPE_TRIAN_ATR)

/** sequence of the connected components */
#define CC_SEQ_CONNECTED_COMP  (CC_SEQ_KIND_GENERIC  | CC_SEQ_ELTYPE_CONNECTED_COMP)

/** sequence of the integer numbers */
#define CC_SEQ_INDEX           (CC_SEQ_KIND_GENERIC  | CC_SEQ_ELTYPE_INDEX)

#define CC_SEQ_ELTYPE( seq )   ((seq)->flags & CC_SEQ_ELTYPE_MASK)
#define CC_SEQ_KIND( seq )     ((seq)->flags & CC_SEQ_KIND_MASK )

/** flag checking */
#define CC_IS_SEQ_INDEX( seq )      ((CC_SEQ_ELTYPE(seq) == CC_SEQ_ELTYPE_INDEX) && \
                                     (CC_SEQ_KIND(seq) == CC_SEQ_KIND_GENERIC))

#define CC_IS_SEQ_CURVE( seq )      (CC_SEQ_KIND(seq) == CC_SEQ_KIND_CURVE)
#define CC_IS_SEQ_CLOSED( seq )     (((seq)->flags & CC_SEQ_FLAG_CLOSED) != 0)
#define CC_IS_SEQ_CONVEX( seq )     0
#define CC_IS_SEQ_HOLE( seq )       (((seq)->flags & CC_SEQ_FLAG_HOLE) != 0)
#define CC_IS_SEQ_SIMPLE( seq )     1

/** type checking macros */
#define CC_IS_SEQ_POINT_SET( seq ) \
    ((CC_SEQ_ELTYPE(seq) == CC_32SC2 || CC_SEQ_ELTYPE(seq) == CC_32FC2))

#define CC_IS_SEQ_POINT_SUBSET( seq ) \
    (CC_IS_SEQ_INDEX( seq ) || CC_SEQ_ELTYPE(seq) == CC_SEQ_ELTYPE_PPOINT)

#define CC_IS_SEQ_POLYLINE( seq )   \
    (CC_SEQ_KIND(seq) == CC_SEQ_KIND_CURVE && CC_IS_SEQ_POINT_SET(seq))

#define CC_IS_SEQ_POLYGON( seq )   \
    (CC_IS_SEQ_POLYLINE(seq) && CC_IS_SEQ_CLOSED(seq))

#define CC_IS_SEQ_CHAIN( seq )   \
    (CC_SEQ_KIND(seq) == CC_SEQ_KIND_CURVE && (seq)->elem_size == 1)

#define CC_IS_SEQ_CONTOUR( seq )   \
    (CC_IS_SEQ_CLOSED(seq) && (CC_IS_SEQ_POLYLINE(seq) || CC_IS_SEQ_CHAIN(seq)))

#define CC_IS_SEQ_CHAIN_CONTOUR( seq ) \
    (CC_IS_SEQ_CHAIN( seq ) && CC_IS_SEQ_CLOSED( seq ))

#define CC_IS_SEQ_POLYGON_TREE( seq ) \
    (CC_SEQ_ELTYPE (seq) ==  CC_SEQ_ELTYPE_TRIAN_ATR &&    \
    CC_SEQ_KIND( seq ) ==  CC_SEQ_KIND_BIN_TREE )

#define CC_IS_GRAPH( seq )    \
    (CC_IS_SET(seq) && CC_SEQ_KIND((CvSet*)(seq)) == CC_SEQ_KIND_GRAPH)

#define CC_IS_GRAPH_ORIENTED( seq )   \
    (((seq)->flags & CC_GRAPH_FLAG_ORIENTED) != 0)

#define CC_IS_SUBDIV2D( seq )  \
    (CC_IS_SET(seq) && CC_SEQ_KIND((CvSet*)(seq)) == CC_SEQ_KIND_SUBDIV2D)

/****************************************************************************************/
/*                            Sequence writer & reader                                  */
/****************************************************************************************/

#define CC_SEQ_WRITER_FIELDS()                                     \
    int          header_size;                                      \
    CvSeq*       seq;        /**< the sequence written */            \
    CvSeqBlock*  block;      /**< current block */                   \
    schar*       ptr;        /**< pointer to free space */           \
    schar*       block_min;  /**< pointer to the beginning of block*/\
    schar*       block_max;  /**< pointer to the end of block */

typedef struct CvSeqWriter
{
    CC_SEQ_WRITER_FIELDS()
}
CvSeqWriter;


#define CC_SEQ_READER_FIELDS()                                      \
    int          header_size;                                       \
    CvSeq*       seq;        /**< sequence, beign read */             \
    CvSeqBlock*  block;      /**< current block */                    \
    schar*       ptr;        /**< pointer to element be read next */  \
    schar*       block_min;  /**< pointer to the beginning of block */\
    schar*       block_max;  /**< pointer to the end of block */      \
    int          delta_index;/**< = seq->first->start_index   */      \
    schar*       prev_elem;  /**< pointer to previous element */

typedef struct CvSeqReader
{
    CC_SEQ_READER_FIELDS()
}
CvSeqReader;

/****************************************************************************************/
/*                                Operations on sequences                               */
/****************************************************************************************/

#define  CC_SEQ_ELEM( seq, elem_type, index )                    \
/** assert gives some guarantee that <seq> parameter is valid */  \
(   assert(sizeof((seq)->first[0]) == sizeof(CvSeqBlock) &&      \
    (seq)->elem_size == sizeof(elem_type)),                      \
    (elem_type*)((seq)->first && (unsigned)index <               \
    (unsigned)((seq)->first->count) ?                            \
    (seq)->first->data + (index) * sizeof(elem_type) :           \
    cvGetSeqElem( (CvSeq*)(seq), (index) )))
#define CC_GET_SEQ_ELEM( elem_type, seq, index ) CC_SEQ_ELEM( (seq), elem_type, (index) )

/** Add element to sequence: */
#define CC_WRITE_SEQ_ELEM_VAR( elem_ptr, writer )     \
{                                                     \
    if( (writer).ptr >= (writer).block_max )          \
    {                                                 \
        cvCreateSeqBlock( &writer);                   \
    }                                                 \
    memcpy((writer).ptr, elem_ptr, (writer).seq->elem_size);\
    (writer).ptr += (writer).seq->elem_size;          \
}

#define CC_WRITE_SEQ_ELEM( elem, writer )             \
{                                                     \
    assert( (writer).seq->elem_size == sizeof(elem)); \
    if( (writer).ptr >= (writer).block_max )          \
    {                                                 \
        cvCreateSeqBlock( &writer);                   \
    }                                                 \
    assert( (writer).ptr <= (writer).block_max - sizeof(elem));\
    memcpy((writer).ptr, &(elem), sizeof(elem));      \
    (writer).ptr += sizeof(elem);                     \
}


/** Move reader position forward: */
#define CC_NEXT_SEQ_ELEM( elem_size, reader )                 \
{                                                             \
    if( ((reader).ptr += (elem_size)) >= (reader).block_max ) \
    {                                                         \
        cvChangeSeqBlock( &(reader), 1 );                     \
    }                                                         \
}


/** Move reader position backward: */
#define CC_PREV_SEQ_ELEM( elem_size, reader )                \
{                                                            \
    if( ((reader).ptr -= (elem_size)) < (reader).block_min ) \
    {                                                        \
        cvChangeSeqBlock( &(reader), -1 );                   \
    }                                                        \
}

/** Read element and move read position forward: */
#define CC_READ_SEQ_ELEM( elem, reader )                       \
{                                                              \
    assert( (reader).seq->elem_size == sizeof(elem));          \
    memcpy( &(elem), (reader).ptr, sizeof((elem)));            \
    CC_NEXT_SEQ_ELEM( sizeof(elem), reader )                   \
}

/** Read element and move read position backward: */
#define CC_REV_READ_SEQ_ELEM( elem, reader )                     \
{                                                                \
    assert( (reader).seq->elem_size == sizeof(elem));            \
    memcpy(&(elem), (reader).ptr, sizeof((elem)));               \
    CC_PREV_SEQ_ELEM( sizeof(elem), reader )                     \
}


#define CC_READ_CHAIN_POINT( _pt, reader )                              \
{                                                                       \
    (_pt) = (reader).pt;                                                \
    if( (reader).ptr )                                                  \
    {                                                                   \
        CC_READ_SEQ_ELEM( (reader).code, (reader));                     \
        assert( ((reader).code & ~7) == 0 );                            \
        (reader).pt.x += (reader).deltas[(int)(reader).code][0];        \
        (reader).pt.y += (reader).deltas[(int)(reader).code][1];        \
    }                                                                   \
}

#define CC_CURRENT_POINT( reader )  (*((CPoint*)((reader).ptr)))
#define CC_PREV_POINT( reader )     (*((CPoint*)((reader).prev_elem)))

#define CC_READ_EDGE( pt1, pt2, reader )               \
{                                                      \
    assert( sizeof(pt1) == sizeof(CPoint) &&          \
            sizeof(pt2) == sizeof(CPoint) &&          \
            reader.seq->elem_size == sizeof(CPoint)); \
    (pt1) = CC_PREV_POINT( reader );                   \
    (pt2) = CC_CURRENT_POINT( reader );                \
    (reader).prev_elem = (reader).ptr;                 \
    CC_NEXT_SEQ_ELEM( sizeof(CPoint), (reader));      \
}

/************ Graph macros ************/

/** Return next graph edge for given vertex: */
#define  CC_NEXT_GRAPH_EDGE( edge, vertex )                              \
     (assert((edge)->vtx[0] == (vertex) || (edge)->vtx[1] == (vertex)),  \
      (edge)->next[(edge)->vtx[1] == (vertex)])



/****************************************************************************************\
*             Data structures for persistence (a.k.a serialization) functionality        *
\****************************************************************************************/

/** "black box" file storage */
typedef struct CvFileStorage CvFileStorage;

/** Storage flags: */
#define CC_STORAGE_READ          0
#define CC_STORAGE_WRITE         1
#define CC_STORAGE_WRITE_TEXT    CC_STORAGE_WRITE
#define CC_STORAGE_WRITE_BINARY  CC_STORAGE_WRITE
#define CC_STORAGE_APPEND        2
#define CC_STORAGE_MEMORY        4
#define CC_STORAGE_FORMAT_MASK   (7<<3)
#define CC_STORAGE_FORMAT_AUTO   0
#define CC_STORAGE_FORMAT_XML    8
#define CC_STORAGE_FORMAT_YAML  16
#define CC_STORAGE_FORMAT_JSON  24
#define CC_STORAGE_BASE64       64
#define CC_STORAGE_WRITE_BASE64  (CC_STORAGE_BASE64 | CC_STORAGE_WRITE)

/** @brief List of attributes. :

In the current implementation, attributes are used to pass extra parameters when writing user
objects (see cvWrite). XML attributes inside tags are not supported, aside from the object type
specification (type_id attribute).
@see cvAttrList, cvAttrValue
 */
typedef struct CvAttrList
{
    const char** attr;         /**< NULL-terminated array of (attribute_name,attribute_value) pairs. */
    struct CvAttrList* next;   /**< Pointer to next chunk of the attributes list.                    */
}
CvAttrList;

/** initializes CvAttrList structure */
CC_INLINE CvAttrList cvAttrList( const char** attr CC_DEFAULT(NULL),
                                 CvAttrList* next CC_DEFAULT(NULL) )
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
#define CC_NODE_REF         4 /**< not used */
#define CC_NODE_SEQ         5
#define CC_NODE_MAP         6
#define CC_NODE_TYPE_MASK   7

#define CC_NODE_TYPE(flags)  ((flags) & CC_NODE_TYPE_MASK)

/** file node flags */
#define CC_NODE_FLOW        8 /**<Used only for writing structures in YAML format. */
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

/** All the keys (names) of elements in the readed file storage
   are stored in the hash to speed up the lookup operations: */
typedef struct CvStringHashNode
{
    unsigned hashval;
    CvString str;
    struct CvStringHashNode* next;
}
CvStringHashNode;

typedef struct CvGenericHash CvFileNodeHash;

/** Basic element of the file storage - scalar or collection: */
typedef struct CFileNode
{
    int tag;
    struct CvTypeInfo* info; /**< type information
            (only for user-defined object, for others it is 0) */
    union
    {
        double f; /**< scalar floating-point number */
        int i;    /**< scalar integer number */
        CvString str; /**< text string */
        CvSeq* seq; /**< sequence (ordered collection of file nodes) */
        CvFileNodeHash* map; /**< map (collection of named file nodes) */
    } data;
}
CvFileNode;

#ifdef __cplusplus
extern "C" {
#endif
typedef int (C_CDECL *CvIsInstanceFunc)( const void* struct_ptr );
typedef void (C_CDECL *CvReleaseFunc)( void** struct_dblptr );
typedef void* (C_CDECL *CvReadFunc)( CvFileStorage* storage, CvFileNode* node );
typedef void (C_CDECL *CvWriteFunc)( CvFileStorage* storage, const char* name,
                                      const void* struct_ptr, CvAttrList attributes );
typedef void* (C_CDECL *CvCloneFunc)( const void* struct_ptr );
#ifdef __cplusplus
}
#endif

/** @brief Type information

The structure contains information about one of the standard or user-defined types. Instances of the
type may or may not contain a pointer to the corresponding CvTypeInfo structure. In any case, there
is a way to find the type info structure for a given object using the cvTypeOf function.
Alternatively, type info can be found by type name using cvFindType, which is used when an object
is read from file storage. The user can register a new type with cvRegisterType that adds the type
information structure into the beginning of the type list. Thus, it is possible to create
specialized types from generic standard types and override the basic methods.
 */
typedef struct CvTypeInfo
{
    int flags; /**< not used */
    int header_size; /**< sizeof(CvTypeInfo) */
    struct CvTypeInfo* prev; /**< previous registered type in the list */
    struct CvTypeInfo* next; /**< next registered type in the list */
    const char* type_name; /**< type name, written to file storage */
    CvIsInstanceFunc is_instance; /**< checks if the passed object belongs to the type */
    CvReleaseFunc release; /**< releases object (memory etc.) */
    CvReadFunc read; /**< reads object from file storage */
    CvWriteFunc write; /**< writes object to file storage */
    CvCloneFunc clone; /**< creates a copy of the object */
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

/** @} */

#endif /*OPENCC_CORE_TYPES_H*/


