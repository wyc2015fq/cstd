

#ifndef _CXCORE_H_
#define _CXCORE_H_

#ifdef __IMG_H__
#define HAVE_IPL
#endif

#ifndef SKIP_INCLUDES
  #if defined HAVE_IPL && !defined __IMG_H__
    #ifndef _INC_WINDOWS
        #define CC_PRETEND_WINDOWS
        #define _INC_WINDOWS
        typedef struct tagBITMAPINFOHEADER BITMAPINFOHEADER;
        typedef int BOOL;
    #endif
    #if defined WIN32 || defined WIN64
      #include "ipl.h"
    #else
      #include "ipl/ipl.h"
    #endif
    #ifdef CC_PRETEND_WINDOWS
        #undef _INC_WINDOWS
    #endif
  #endif
#endif // SKIP_INCLUDES

#include "cxtypes.h"
#include "cxerror.h"
#include "cvver.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************************\
*          Array allocation, deallocation, initialization and access to elements         *
\****************************************************************************************/

/* <malloc> wrapper.
   If there is no enough memory, the function
   (as well as other OpenCV functions that call cAlloc)
   raises an error. */
CVAPI(void*)  cAlloc(size_t size);

/* <free> wrapper.
   Here and further all the memory releasing functions
   (that all call cFree) take double pointer in order to
   to clear pointer to the data after releasing it.
   Passing pointer to NULL pointer is Ok: nothing happens in this case
*/
CVAPI(void)   cvFree_(void* ptr);
#define cFree(ptr) (cvFree_(*(ptr)), *(ptr)=0)

/* Allocates and initializes img_t header */
CVAPI(img_t*)  cvCreateImageHeader(CSize size, TypeId type, int channels);

/* Inializes img_t header */
CVAPI(img_t*) cvInitImageHeader(img_t* image, CSize size, TypeId type,
                                   int channels, int origin CC_DEFAULT(0),
                                   int align CC_DEFAULT(4));

/* Creates IPL image (header and data) */
CVAPI(img_t*)  cvCreateImage(CSize size, TypeId type, int channels);

/* Releases (i.e. deallocates) IPL image header */
CVAPI(void)  cvReleaseImageHeader(img_t** image);

/* Releases IPL image header and data */
CVAPI(void)  cvReleaseImage(img_t** image);

/* Creates a copy of IPL image (widthStep may differ) */
CVAPI(img_t*) cvCloneImage(const img_t* image);

/* Sets a Channel Of Interest (only a few functions support COI) - 
   use cvCopy to extract the selected channel and/or put it back */
CVAPI(void)  cvSetImageCOI(img_t* image, int coi);

/* Retrieves image Channel Of Interest */
CVAPI(int)  cvGetImageCOI(const img_t* image);

/* Sets image ROI (region of interest) (COI is not changed) */
CVAPI(void)  cvSetImageROI(img_t* image, CRect rect);

/* Resets image ROI and COI */
CVAPI(void)  cvResetImageROI(img_t* image);

/* Retrieves image ROI */
CVAPI(CRect) cvGetImageROI(const img_t* image);

/* Allocates and initalizes img_t header */
CVAPI(img_t*)  cvCreateMatHeader(int rows, int cols, int type);

#define CC_AUTOSTEP  0x7fffffff

/* Initializes img_t header */
CVAPI(img_t*) cvInitMatHeader(img_t* mat, int rows, int cols,
                              int type, void* data CC_DEFAULT(NULL),
                              int step CC_DEFAULT(CC_AUTOSTEP));

/* Allocates and initializes img_t header and allocates data */
CVAPI(img_t*)  cvCreateMat(int rows, int cols, int type);

/* Releases img_t header and deallocates matrix data
   (reference counting is used for data) */
CVAPI(void)  cvReleaseMat(img_t** mat);

/* Decrements img_t data reference counter and deallocates the data if
   it reaches 0 */
CC_INLINE  void  cvDecRefData(img_t* arr)
{
    if(CC_IS_MAT(arr))
    {
        img_t* mat = arr;
        mat->tt.data = NULL;
        if(mat->refcount != NULL && --*mat->refcount == 0)
            cFree(&mat->refcount);
        mat->refcount = NULL;
    }
    else if(CC_IS_MATND(arr))
    {
        CvMatND* mat = (CvMatND*)arr;
        mat->tt.data = NULL;
        if(mat->refcount != NULL && --*mat->refcount == 0)
            cFree(&mat->refcount);
        mat->refcount = NULL;
    }
}

/* Increments img_t data reference counter */
CC_INLINE  int  cvIncRefData(img_t* arr)
{
    int refcount = 0;
    if(CC_IS_MAT(arr))
    {
        img_t* mat = arr;
        if(mat->refcount != NULL)
            refcount = ++*mat->refcount;
    }
    else if(CC_IS_MATND(arr))
    {
        CvMatND* mat = (CvMatND*)arr;
        if(mat->refcount != NULL)
            refcount = ++*mat->refcount;
    }
    return refcount;
}


/* Creates an exact copy of the input matrix (except, may be, step value) */
CVAPI(img_t*) cvCloneMat(const img_t* mat);


/* Makes a new matrix from <rect> subrectangle of input array.
   No data is copied */
CVAPI(img_t*) cvGetSubRect(const img_t* arr, img_t* submat, CRect rect);
#define cvGetSubArr cvGetSubRect

/* Selects row span of the input array: arr(start_row:delta_row:end_row,:)
    (end_row is not included into the span). */
CVAPI(img_t*) cvGetRows(const img_t* arr, img_t* submat,
                        int start_row, int end_row,
                        int delta_row CC_DEFAULT(1));

CC_INLINE  img_t*  cvGetRow(const img_t* arr, img_t* submat, int row)
{
    return cvGetRows(arr, submat, row, row + 1, 1);
}


/* Selects column span of the input array: arr(:,start_col:end_col)
   (end_col is not included into the span) */
CVAPI(img_t*) cvGetCols(const img_t* arr, img_t* submat,
                        int start_col, int end_col);

CC_INLINE  img_t*  cvGetCol(const img_t* arr, img_t* submat, int col)
{
    return cvGetCols(arr, submat, col, col + 1);
}

/* Select a diagonal of the input array.
   (diag = 0 means the main diagonal, >0 means a diagonal above the main one,
   <0 - below the main one).
   The diagonal will be represented as a column (nx1 matrix). */
CVAPI(img_t*) cvGetDiag(const img_t* arr, img_t* submat,
                            int diag CC_DEFAULT(0));

/* low-level scalar <-> raw data conversion functions */
CVAPI(void) cvScalarToRawData(const CScalar* scalar, void* data, int type,
                              int extend_to_12 CC_DEFAULT(0));

CVAPI(void) cvRawDataToScalar(const void* data, int type, CScalar* scalar);

/* Allocates and initializes CvMatND header */
CVAPI(CvMatND*)  cvCreateMatNDHeader(int dims, const int* sizes, int type);

/* Allocates and initializes CvMatND header and allocates data */
CVAPI(CvMatND*)  cvCreateMatND(int dims, const int* sizes, int type);

/* Initializes preallocated CvMatND header */
CVAPI(CvMatND*)  cvInitMatNDHeader(CvMatND* mat, int dims, const int* sizes,
                                    int type, void* data CC_DEFAULT(NULL));

/* Releases CvMatND */
CC_INLINE  void  cvReleaseMatND(CvMatND** mat)
{
    cvReleaseMat((img_t**)mat);
}

/* Creates a copy of CvMatND (except, may be, steps) */
CVAPI(CvMatND*) cvCloneMatND(const CvMatND* mat);

/* Allocates and initializes CvSparseMat header and allocates data */
CVAPI(CvSparseMat*)  cvCreateSparseMat(int dims, const int* sizes, int type);

/* Releases CvSparseMat */
CVAPI(void)  cvReleaseSparseMat(CvSparseMat** mat);

/* Creates a copy of CvSparseMat (except, may be, zero items) */
CVAPI(CvSparseMat*) cvCloneSparseMat(const CvSparseMat* mat);

/* Initializes sparse array iterator
   (returns the first node or NULL if the array is empty) */
CVAPI(CvSparseNode*) cvInitSparseMatIterator(const CvSparseMat* mat,
                                              CvSparseMatIterator* mat_iterator);

// returns next sparse array node (or NULL if there is no more nodes)
CC_INLINE CvSparseNode* cvGetNextSparseNode(CvSparseMatIterator* mat_iterator)
{
    if(mat_iterator->node->next)
        return mat_iterator->node = mat_iterator->node->next;
    else
    {
        int idx;
        for(idx = ++mat_iterator->curidx; idx < mat_iterator->mat->hashsize; idx++)
        {
            CvSparseNode* node = (CvSparseNode*)mat_iterator->mat->hashtable[idx];
            if(node)
            {
                mat_iterator->curidx = idx;
                return mat_iterator->node = node;
            }
        }
        return NULL;
    }
}

/**************** matrix iterator: used for n-ary operations on dense arrays *********/

#define CC_MAX_ARR 10

typedef struct CvNArrayIterator
{
    int count; /* number of arrays */
    int dims; /* number of dimensions to iterate */
    CSize size; /* maximal common linear size: { width = size, height = 1 } */
    uchar* ptr[CC_MAX_ARR]; /* pointers to the array slices */
    int stack[CC_MAX_DIM]; /* for internal use */
    CvMatND* hdr[CC_MAX_ARR]; /* pointers to the headers of the
                                 matrices that are processed */
}
CvNArrayIterator;

#define CC_NO_DEPTH_CHECK     1
#define CC_NO_CN_CHECK        2
#define CC_NO_SIZE_CHECK      4

/* initializes iterator that traverses through several arrays simulteneously
   (the function together with cvNextArraySlice is used for
    N-ari element-wise operations) */
CVAPI(int) cvInitNArrayIterator(int count, img_t** arrs,
                                 const img_t* mask, CvMatND* stubs,
                                 CvNArrayIterator* array_iterator,
                                 int flags CC_DEFAULT(0));

/* returns zero value if iteration is finished, non-zero (slice length) otherwise */
CVAPI(int) cvNextNArraySlice(CvNArrayIterator* array_iterator);


/* Returns type of array elements:
   CC_8UC1 ... CC_64FC4 ... */
CVAPI(int) cvGetElemType(const img_t* arr);

/* Retrieves number of an array dimensions and
   optionally sizes of the dimensions */
CVAPI(int) cvGetDims(const img_t* arr, int* sizes CC_DEFAULT(NULL));


/* Retrieves size of a particular array dimension.
   For 2d arrays cvGetDimSize(arr,0) returns number of rows (image height)
   and cvGetDimSize(arr,1) returns number of columns (image width) */
CVAPI(int) cvGetDimSize(const img_t* arr, int index);


/* ptr = &arr(idx0,idx1,...). All indexes are zero-based,
   the major dimensions go first (e.g. (y,x) for 2D, (z,y,x) for 3D */
CVAPI(uchar*) cvPtr1D(const img_t* arr, int idx0, int* type CC_DEFAULT(NULL));
CVAPI(uchar*) cvPtr2D(const img_t* arr, int idx0, int idx1, int* type CC_DEFAULT(NULL));
CVAPI(uchar*) cvPtr3D(const img_t* arr, int idx0, int idx1, int idx2,
                      int* type CC_DEFAULT(NULL));

/* For img_t or img_t number of indices should be 2
   (row index (y) goes first, column index (x) goes next).
   For CvMatND or CvSparseMat number of infices should match number of <dims> and
   indices order should match the array dimension order. */
CVAPI(uchar*) cvPtrND(const img_t* arr, const int* idx, int* type CC_DEFAULT(NULL),
                      int create_node CC_DEFAULT(1),
                      unsigned* precalc_hashval CC_DEFAULT(NULL));

/* value = arr(idx0,idx1,...) */
CVAPI(CScalar) cvGet1D(const img_t* arr, int idx0);
CVAPI(CScalar) cvGet2D(const img_t* arr, int idx0, int idx1);
CVAPI(CScalar) cvGet3D(const img_t* arr, int idx0, int idx1, int idx2);
CVAPI(CScalar) cvGetND(const img_t* arr, const int* idx);

/* for 1-channel arrays */
CVAPI(double) cvGetReal1D(const img_t* arr, int idx0);
CVAPI(double) cvGetReal2D(const img_t* arr, int idx0, int idx1);
CVAPI(double) cvGetReal3D(const img_t* arr, int idx0, int idx1, int idx2);
CVAPI(double) cvGetRealND(const img_t* arr, const int* idx);

/* arr(idx0,idx1,...) = value */
CVAPI(void) cvSet1D(img_t* arr, int idx0, CScalar value);
CVAPI(void) cvSet2D(img_t* arr, int idx0, int idx1, CScalar value);
CVAPI(void) cvSet3D(img_t* arr, int idx0, int idx1, int idx2, CScalar value);
CVAPI(void) cvSetND(img_t* arr, const int* idx, CScalar value);

/* for 1-channel arrays */
CVAPI(void) cvSetReal1D(img_t* arr, int idx0, double value);
CVAPI(void) cvSetReal2D(img_t* arr, int idx0, int idx1, double value);
CVAPI(void) cvSetReal3D(img_t* arr, int idx0,
                        int idx1, int idx2, double value);
CVAPI(void) cvSetRealND(img_t* arr, const int* idx, double value);

/* clears element of ND dense array,
   in case of sparse arrays it deletes the specified node */
CVAPI(void) cvClearND(img_t* arr, const int* idx);

/* Converts img_t (img_t or img_t,...) to img_t.
   If the last parameter is non-zero, function can
   convert multi(>2)-dimensional array to img_t as long as
   the last array's dimension is continous. The resultant
   matrix will be have appropriate (a huge) number of rows */
CVAPI(img_t*) cvGetMat(const img_t* arr, img_t* header,
                       int* coi CC_DEFAULT(NULL),
                       int allowND CC_DEFAULT(0));

/* Converts img_t (img_t or img_t) to img_t */
CVAPI(img_t*) cvGetImage(const img_t* arr, img_t* image_header);


/* Changes a shape of multi-dimensional array.
   new_cn == 0 means that number of channels remains unchanged.
   new_dims == 0 means that number and sizes of dimensions remain the same
   (unless they need to be changed to set the new number of channels)
   if new_dims == 1, there is no need to specify new dimension sizes
   The resultant configuration should be achievable w/o data copying.
   If the resultant array is sparse, CvSparseMat header should be passed
   to the function else if the result is 1 or 2 dimensional,
   img_t header should be passed to the function
   else CvMatND header should be passed */
CVAPI(img_t*) cvReshapeMatND(const img_t* arr,
                             int sizeof_header, img_t* header,
                             int new_cn, int new_dims, int* new_sizes);

#define cvReshapeND(arr, header, new_cn, new_dims, new_sizes)   \
      cvReshapeMatND((arr), sizeof(*(header)), (header),         \
                      (new_cn), (new_dims), (new_sizes))

CVAPI(img_t*) cvReshape(const img_t* arr, img_t* header,
                        int new_cn, int new_rows CC_DEFAULT(0));

/* Repeats source 2d array several times in both horizontal and
   vertical direction to fill destination array */
CVAPI(void) cvRepeat(const img_t* src, img_t* dst);

/* Allocates array data */
CVAPI(void)  cvCreateData(img_t* arr);

/* Releases array data */
CVAPI(void)  cvReleaseData(img_t* arr);

/* Attaches user data to the array header. The step is reffered to
   the pre-last dimension. That is, all the planes of the array
   must be joint (w/o gaps) */
CVAPI(void)  cvSetData(img_t* arr, void* data, int step);

/* Retrieves raw data of img_t, img_t or CvMatND.
   In the latter case the function raises an error if
   the array can not be represented as a matrix */
CVAPI(void) cvGetRawData(const img_t* arr, uchar** data,
                         int* step CC_DEFAULT(NULL),
                         CSize* roi_size CC_DEFAULT(NULL));

/* Returns width and height of array in elements */
CVAPI(CSize) cvGetSize(const img_t* arr);

/* Copies source array to destination array */
CVAPI(void)  cvCopy(const img_t* src, img_t* dst,
                     const img_t* mask CC_DEFAULT(NULL));

/* Sets all or "masked" elements of input array
   to the same value*/
CVAPI(void)  cvSet(img_t* arr, CScalar value,
                    const img_t* mask CC_DEFAULT(NULL));

/* Clears all the array elements (sets them to 0) */
CVAPI(void)  cvSetZero(img_t* arr);
#define cvZero  cvSetZero


/* Splits a multi-channel array into the set of single-channel arrays or
   extracts particular [color] plane */
CVAPI(void)  cvSplit(const img_t* src, img_t* dst0, img_t* dst1,
                      img_t* dst2, img_t* dst3);

/* Merges a set of single-channel arrays into the single multi-channel array
   or inserts one particular [color] plane to the array */
CVAPI(void)  cvMerge(const img_t* src0, const img_t* src1,
                      const img_t* src2, const img_t* src3,
                      img_t* dst);

/* Copies several channels from input arrays to
   certain channels of output arrays */
CVAPI(void)  cvMixChannels(const img_t** src, int src_count,
                            img_t** dst, int dst_count,
                            const int* from_to, int pair_count);

/* Performs linear transformation on every source array element:
   dst(x,y,c) = scale*src(x,y,c)+shift.
   Arbitrary combination of input and output array depths are allowed
   (number of channels must be the same), thus the function can be used
   for type conversion */
CVAPI(void)  cvConvertScale(const img_t* src, img_t* dst,
                             double scale CC_DEFAULT(1),
                             double shift CC_DEFAULT(0));
#define cvCvtScale cvConvertScale
#define cvScale  cvConvertScale
#define cvConvert(src, dst)  cvConvertScale((src), (dst), 1, 0)


/* Performs linear transformation on every source array element,
   stores absolute value of the result:
   dst(x,y,c) = abs(scale*src(x,y,c)+shift).
   destination array must have 8u type.
   In other cases one may use cvConvertScale + cvAbsDiffS */
CVAPI(void)  cvConvertScaleAbs(const img_t* src, img_t* dst,
                                double scale CC_DEFAULT(1),
                                double shift CC_DEFAULT(0));
#define cvCvtScaleAbs  cvConvertScaleAbs


/* checks termination criteria validity and
   sets eps to default_eps (if it is not set),
   max_iter to default_max_iters (if it is not set)
*/
CVAPI(CTermCriteria) cvCheckTermCriteria(CTermCriteria criteria,
                                           double default_eps,
                                           int default_max_iters);

/****************************************************************************************\
*                   Arithmetic, logic and comparison operations                          *
\****************************************************************************************/

/* dst(mask) = src1(mask) + src2(mask) */
CVAPI(void)  cvAdd(const img_t* src1, const img_t* src2, img_t* dst,
                    const img_t* mask CC_DEFAULT(NULL));

/* dst(mask) = src(mask) + value */
CVAPI(void)  cvAddS(const img_t* src, CScalar value, img_t* dst,
                     const img_t* mask CC_DEFAULT(NULL));

/* dst(mask) = src1(mask) - src2(mask) */
CVAPI(void)  cvSub(const img_t* src1, const img_t* src2, img_t* dst,
                    const img_t* mask CC_DEFAULT(NULL));

/* dst(mask) = src(mask) - value = src(mask) + (-value) */
CC_INLINE  void  cvSubS(const img_t* src, CScalar value, img_t* dst,
                         const img_t* mask CC_DEFAULT(NULL))
{
    cvAddS(src, cScalar(-value.val[0], -value.val[1], -value.val[2], -value.val[3]),
            dst, mask);
}

/* dst(mask) = value - src(mask) */
CVAPI(void)  cvSubRS(const img_t* src, CScalar value, img_t* dst,
                      const img_t* mask CC_DEFAULT(NULL));

/* dst(idx) = src1(idx) * src2(idx) * scale
   (scaled element-wise multiplication of 2 arrays) */
CVAPI(void)  cvMul(const img_t* src1, const img_t* src2,
                    img_t* dst, double scale CC_DEFAULT(1));

/* element-wise division/inversion with scaling: 
    dst(idx) = src1(idx) * scale / src2(idx)
    or dst(idx) = scale / src2(idx) if src1 == 0 */
CVAPI(void)  cvDiv(const img_t* src1, const img_t* src2,
                    img_t* dst, double scale CC_DEFAULT(1));

/* dst = src1 * scale + src2 */
CVAPI(void)  cvScaleAdd(const img_t* src1, CScalar scale,
                         const img_t* src2, img_t* dst);
#define cvAXPY(A, real_scalar, B, C) cvScaleAdd(A, cRealScalar(real_scalar), B, C)

/* dst = src1 * alpha + src2 * beta + gamma */
CVAPI(void)  cvAddWeighted(const img_t* src1, double alpha,
                            const img_t* src2, double beta,
                            double gamma, img_t* dst);

/* result = sum_i(src1(i) * src2(i)) (results for all channels are accumulated together) */
CVAPI(double)  cvDotProduct(const img_t* src1, const img_t* src2);

/* dst(idx) = src1(idx) & src2(idx) */
CVAPI(void) cvAnd(const img_t* src1, const img_t* src2,
                  img_t* dst, const img_t* mask CC_DEFAULT(NULL));

/* dst(idx) = src(idx) & value */
CVAPI(void) cvAndS(const img_t* src, CScalar value,
                   img_t* dst, const img_t* mask CC_DEFAULT(NULL));

/* dst(idx) = src1(idx) | src2(idx) */
CVAPI(void) cvOr(const img_t* src1, const img_t* src2,
                 img_t* dst, const img_t* mask CC_DEFAULT(NULL));

/* dst(idx) = src(idx) | value */
CVAPI(void) cvOrS(const img_t* src, CScalar value,
                  img_t* dst, const img_t* mask CC_DEFAULT(NULL));

/* dst(idx) = src1(idx) ^ src2(idx) */
CVAPI(void) cvXor(const img_t* src1, const img_t* src2,
                  img_t* dst, const img_t* mask CC_DEFAULT(NULL));

/* dst(idx) = src(idx) ^ value */
CVAPI(void) cvXorS(const img_t* src, CScalar value,
                   img_t* dst, const img_t* mask CC_DEFAULT(NULL));

/* dst(idx) = ~src(idx) */
CVAPI(void) cvNot(const img_t* src, img_t* dst);

/* dst(idx) = lower(idx) <= src(idx) < upper(idx) */
CVAPI(void) cvInRange(const img_t* src, const img_t* lower,
                      const img_t* upper, img_t* dst);

/* dst(idx) = lower <= src(idx) < upper */
CVAPI(void) cvInRangeS(const img_t* src, CScalar lower,
                       CScalar upper, img_t* dst);

#define CC_CMP_EQ   0
#define CC_CMP_GT   1
#define CC_CMP_GE   2
#define CC_CMP_LT   3
#define CC_CMP_LE   4
#define CC_CMP_NE   5

/* The comparison operation support single-channel arrays only.
   Destination image should be 8uC1 or 8sC1 */

/* dst(idx) = src1(idx) _cmp_op_ src2(idx) */
CVAPI(void) cvCmp(const img_t* src1, const img_t* src2, img_t* dst, int cmp_op);

/* dst(idx) = src1(idx) _cmp_op_ value */
CVAPI(void) cvCmpS(const img_t* src, double value, img_t* dst, int cmp_op);

/* dst(idx) = min(src1(idx),src2(idx)) */
CVAPI(void) cvMin(const img_t* src1, const img_t* src2, img_t* dst);

/* dst(idx) = max(src1(idx),src2(idx)) */
CVAPI(void) cvMax(const img_t* src1, const img_t* src2, img_t* dst);

/* dst(idx) = min(src(idx),value) */
CVAPI(void) cvMinS(const img_t* src, double value, img_t* dst);

/* dst(idx) = max(src(idx),value) */
CVAPI(void) cvMaxS(const img_t* src, double value, img_t* dst);

/* dst(x,y,c) = abs(src1(x,y,c) - src2(x,y,c)) */
CVAPI(void) cvAbsDiff(const img_t* src1, const img_t* src2, img_t* dst);

/* dst(x,y,c) = abs(src(x,y,c) - value(c)) */
CVAPI(void) cvAbsDiffS(const img_t* src, img_t* dst, CScalar value);
#define cvAbs(src, dst) cvAbsDiffS((src), (dst), cScalarAll(0))

/****************************************************************************************\
*                                Math operations                                         *
\****************************************************************************************/

/* Does cartesian->polar coordinates conversion.
   Either of output components (magnitude or angle) is optional */
CVAPI(void)  cvCartToPolar(const img_t* x, const img_t* y,
                            img_t* magnitude, img_t* angle CC_DEFAULT(NULL),
                            int angle_in_degrees CC_DEFAULT(0));

/* Does polar->cartesian coordinates conversion.
   Either of output components (magnitude or angle) is optional.
   If magnitude is missing it is assumed to be all 1's */
CVAPI(void)  cvPolarToCart(const img_t* magnitude, const img_t* angle,
                            img_t* x, img_t* y,
                            int angle_in_degrees CC_DEFAULT(0));

/* Does powering: dst(idx) = src(idx)^power */
CVAPI(void)  cvPow(const img_t* src, img_t* dst, double power);

/* Does exponention: dst(idx) = exp(src(idx)).
   Overflow is not handled yet. Underflow is handled.
   Maximal relative error is ~7e-6 for single-precision input */
CVAPI(void)  cvExp(const img_t* src, img_t* dst);

/* Calculates natural logarithms: dst(idx) = log(abs(src(idx))).
   Logarithm of 0 gives large negative number(~-700)
   Maximal relative error is ~3e-7 for single-precision output
*/
CVAPI(void)  cvLog(const img_t* src, img_t* dst);

/* Fast arctangent calculation */
CVAPI(float) cFastArctan(float y, float x);

/* Fast cubic root calculation */
CVAPI(float)  cCbrt(float value);

/* Checks array values for NaNs, Infs or simply for too large numbers
   (if CC_CHECK_RANGE is set). If CC_CHECK_QUIET is set,
   no runtime errors is raised (function returns zero value in case of "bad" values).
   Otherwise cvError is called */ 
#define  CC_CHECK_RANGE    1
#define  CC_CHECK_QUIET    2
CVAPI(int)  cvCheckArr(const img_t* arr, int flags CC_DEFAULT(0),
                        double min_val CC_DEFAULT(0), double max_val CC_DEFAULT(0));
#define cvCheckArray cvCheckArr

#define CC_RAND_UNI      0
#define CC_RAND_NORMAL   1
CVAPI(void) cvRandArr(CRNG* rng, img_t* arr, int dist_type,
                      CScalar param1, CScalar param2);

CVAPI(void) cvRandShuffle(img_t* mat, CRNG* rng,
                           double iter_factor CC_DEFAULT(1.));

/* Finds real roots of a cubic equation */
CVAPI(int) cvSolveCubic(const img_t* coeffs, img_t* roots);

/****************************************************************************************\
*                                Matrix operations                                       *
\****************************************************************************************/

/* Calculates cross product of two 3d vectors */
CVAPI(void)  cvCrossProduct(const img_t* src1, const img_t* src2, img_t* dst);

/* Matrix transform: dst = A*B + C, C is optional */
#define cvMatMulAdd(src1, src2, src3, dst) cvGEMM((src1), (src2), 1., (src3), 1., (dst), 0)
#define cvMatMul(src1, src2, dst)  cvMatMulAdd((src1), (src2), NULL, (dst))

#define CC_GEMM_A_T 1
#define CC_GEMM_B_T 2
#define CC_GEMM_C_T 4
/* Extended matrix transform:
   dst = alpha*op(A)*op(B) + beta*op(C), where op(X) is X or X^T */
CVAPI(void)  cvGEMM(const img_t* src1, const img_t* src2, double alpha,
                     const img_t* src3, double beta, img_t* dst,
                     int tABC CC_DEFAULT(0));
#define cvMatMulAddEx cvGEMM

/* Transforms each element of source array and stores
   resultant vectors in destination array */
CVAPI(void)  cvTransform(const img_t* src, img_t* dst,
                          const img_t* transmat,
                          const img_t* shiftvec CC_DEFAULT(NULL));
#define cvMatMulAddS cvTransform

/* Does perspective transform on every element of input array */
CVAPI(void)  cvPerspectiveTransform(const img_t* src, img_t* dst,
                                     const img_t* mat);

/* Calculates (A-delta)*(A-delta)^T (order=0) or (A-delta)^T*(A-delta) (order=1) */
CVAPI(void) cvMulTransposed(const img_t* src, img_t* dst, int order,
                             const img_t* delta CC_DEFAULT(NULL),
                             double scale CC_DEFAULT(1.));

/* Tranposes matrix. Square matrices can be transposed in-place */
CVAPI(void)  cvTranspose(const img_t* src, img_t* dst);
#define cvT cvTranspose


/* Mirror array data around horizontal (flip=0),
   vertical (flip=1) or both(flip=-1) axises:
   cvFlip(src) flips images vertically and sequences horizontally (inplace) */
CVAPI(void)  cvFlip(const img_t* src, img_t* dst CC_DEFAULT(NULL),
                     int flip_mode CC_DEFAULT(0));
#define cvMirror cvFlip


#define CC_SVD_MODIFY_A   1
#define CC_SVD_U_T        2
#define CC_SVD_V_T        4

/* Performs Singular Value Decomposition of a matrix */
CVAPI(void)   cvSVD(img_t* A, img_t* W, img_t* U CC_DEFAULT(NULL),
                     img_t* V CC_DEFAULT(NULL), int flags CC_DEFAULT(0));

/* Performs Singular Value Back Substitution (solves A*X = B):
   flags must be the same as in cvSVD */
CVAPI(void)   cvSVBkSb(const img_t* W, const img_t* U,
                        const img_t* V, const img_t* B,
                        img_t* X, int flags);

#define CC_LU  0
#define CC_SVD 1
#define CC_SVD_SYM 2
/* Inverts matrix */
CVAPI(double)  cvInvert(const img_t* src, img_t* dst,
                         int method CC_DEFAULT(CC_LU));
#define cvInv cvInvert

/* Solves linear system (src1)*(dst) = (src2)
   (returns 0 if src1 is a singular and CC_LU method is used) */
CVAPI(int)  cvSolve(const img_t* src1, const img_t* src2, img_t* dst,
                     int method CC_DEFAULT(CC_LU));

/* Calculates determinant of input matrix */
CVAPI(double) cvDet(const img_t* mat);

/* Calculates trace of the matrix (sum of elements on the main diagonal) */
CVAPI(CScalar) cvTrace(const img_t* mat);

/* Finds eigen values and vectors of a symmetric matrix */
CVAPI(void)  cvEigenVV(img_t* mat, img_t* evects,
                        img_t* evals, double eps CC_DEFAULT(0));

/* Makes an identity matrix (mat_ij = i == j) */
CVAPI(void)  cvSetIdentity(img_t* mat, CScalar value CC_DEFAULT(cRealScalar(1)));

/* Fills matrix with given range of numbers */
CVAPI(img_t*)  cvRange(img_t* mat, double start, double end);

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

CVAPI(void)  cvCalcCovarMatrix(const img_t** vects, int count,
                                img_t* cov_mat, img_t* avg, int flags);

#define CC_PCA_DATA_AS_ROW 0 
#define CC_PCA_DATA_AS_COL 1
#define CC_PCA_USE_AVG 2
CVAPI(void)  cvCalcPCA(const img_t* data, img_t* mean,
                        img_t* eigenvals, img_t* eigenvects, int flags);

CVAPI(void)  cvProjectPCA(const img_t* data, const img_t* mean,
                           const img_t* eigenvects, img_t* result);

CVAPI(void)  cvBackProjectPCA(const img_t* proj, const img_t* mean,
                               const img_t* eigenvects, img_t* result);

/* Calculates Mahalanobis(weighted) distance */
CVAPI(double)  cvMahalanobis(const img_t* vec1, const img_t* vec2, img_t* mat);
#define cvMahalonobis  cvMahalanobis

/****************************************************************************************\
*                                    Array Statistics                                    *
\****************************************************************************************/

/* Finds sum of array elements */
CVAPI(CScalar)  cvSum(const img_t* arr);

/* Calculates number of non-zero pixels */
CVAPI(int)  cvCountNonZero(const img_t* arr);

/* Calculates mean value of array elements */
CVAPI(CScalar)  cvAvg(const img_t* arr, const img_t* mask CC_DEFAULT(NULL));

/* Calculates mean and standard deviation of pixel values */
CVAPI(void)  cvAvgSdv(const img_t* arr, CScalar* mean, CScalar* std_dev,
                       const img_t* mask CC_DEFAULT(NULL));

/* Finds global minimum, maximum and their positions */
CVAPI(void)  cvMinMaxLoc(const img_t* arr, double* min_val, double* max_val,
                          CPoint* min_loc CC_DEFAULT(NULL),
                          CPoint* max_loc CC_DEFAULT(NULL),
                          const img_t* mask CC_DEFAULT(NULL));

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

/* Finds norm, difference norm or relative difference norm for an array (or two arrays) */
CVAPI(double)  cvNorm(const img_t* arr1, const img_t* arr2 CC_DEFAULT(NULL),
                       int norm_type CC_DEFAULT(CC_L2),
                       const img_t* mask CC_DEFAULT(NULL));

CVAPI(void)  cvNormalize(const img_t* src, img_t* dst,
                          double a CC_DEFAULT(1.), double b CC_DEFAULT(0.),
                          int norm_type CC_DEFAULT(CC_L2),
                          const img_t* mask CC_DEFAULT(NULL));


#define CC_REDUCE_SUM 0
#define CC_REDUCE_AVG 1
#define CC_REDUCE_MAX 2
#define CC_REDUCE_MIN 3

CVAPI(void)  cvReduce(const img_t* src, img_t* dst, int dim CC_DEFAULT(-1),
                       int op CC_DEFAULT(CC_REDUCE_SUM));

/****************************************************************************************\
*                      Discrete Linear Transforms and Related Functions                  *
\****************************************************************************************/

#define CC_DXT_FORWARD  0
#define CC_DXT_INVERSE  1
#define CC_DXT_SCALE    2 /* divide result by size of array */
#define CC_DXT_INV_SCALE (CC_DXT_INVERSE + CC_DXT_SCALE)
#define CC_DXT_INVERSE_SCALE CC_DXT_INV_SCALE
#define CC_DXT_ROWS     4 /* transform each row individually */
#define CC_DXT_MUL_CONJ 8 /* conjugate the second argument of cvMulSpectrums */

/* Discrete Fourier Transform:
    complex->complex,
    real->ccs (forward),
    ccs->real (inverse) */
CVAPI(void)  cvDFT(const img_t* src, img_t* dst, int flags,
                    int nonzero_rows CC_DEFAULT(0));
#define cvFFT cvDFT

/* Multiply results of DFTs: DFT(X)*DFT(Y) or DFT(X)*conj(DFT(Y)) */
CVAPI(void)  cvMulSpectrums(const img_t* src1, const img_t* src2,
                             img_t* dst, int flags);

/* Finds optimal DFT vector size >= size0 */
CVAPI(int)  cGetOptimalDFTSize(int size0);

/* Discrete Cosine Transform */
CVAPI(void)  cvDCT(const img_t* src, img_t* dst, int flags);

/****************************************************************************************\
*                              Dynamic data structures                                   *
\****************************************************************************************/

/* Calculates length of sequence slice (with support of negative indices). */
CVAPI(int) cvSliceLength(CSlice slice, const CvSeq* seq);


/* Creates new memory storage.
   block_size == 0 means that default,
   somewhat optimal size, is used (currently, it is 64K) */
CVAPI(CvMemStorage*)  cvCreateMemStorage(int block_size CC_DEFAULT(0));


/* Creates a memory storage that will borrow memory blocks from parent storage */
CVAPI(CvMemStorage*)  cvCreateChildMemStorage(CvMemStorage* parent);


/* Releases memory storage. All the children of a parent must be released before
   the parent. A child storage returns all the blocks to parent when it is released */
CVAPI(void)  cvReleaseMemStorage(CvMemStorage** storage);


/* Clears memory storage. This is the only way(!!!) (besides cvRestoreMemStoragePos)
   to reuse memory allocated for the storage - cvClearSeq,cvClearSet ...
   do not free any memory.
   A child storage returns all the blocks to the parent when it is cleared */
CVAPI(void)  cvClearMemStorage(CvMemStorage* storage);

/* Remember a storage "free memory" position */
CVAPI(void)  cvSaveMemStoragePos(const CvMemStorage* storage, CvMemStoragePos* pos);

/* Restore a storage "free memory" position */
CVAPI(void)  cvRestoreMemStoragePos(CvMemStorage* storage, CvMemStoragePos* pos);

/* Allocates continuous buffer of the specified size in the storage */
CVAPI(void*) cvMemStorageAlloc(CvMemStorage* storage, size_t size);

/* Allocates string in memory storage */
CVAPI(CvString) cvMemStorageAllocString(CvMemStorage* storage, const char* ptr,
                                        int len CC_DEFAULT(-1));

/* Creates new empty sequence that will reside in the specified storage */
CVAPI(CvSeq*)  cvCreateSeq(int seq_flags, int header_size,
                            int elem_size, CvMemStorage* storage);

/* Changes default size (granularity) of sequence blocks.
   The default size is ~1Kbyte */
CVAPI(void)  cvSetSeqBlockSize(CvSeq* seq, int delta_elems);


/* Adds new element to the end of sequence. Returns pointer to the element */
CVAPI(char*)  cvSeqPush(CvSeq* seq, void* element CC_DEFAULT(NULL));


/* Adds new element to the beginning of sequence. Returns pointer to it */
CVAPI(char*)  cvSeqPushFront(CvSeq* seq, void* element CC_DEFAULT(NULL));


/* Removes the last element from sequence and optionally saves it */
CVAPI(void)  cvSeqPop(CvSeq* seq, void* element CC_DEFAULT(NULL));


/* Removes the first element from sequence and optioanally saves it */
CVAPI(void)  cvSeqPopFront(CvSeq* seq, void* element CC_DEFAULT(NULL));


#define CC_FRONT 1
#define CC_BACK 0
/* Adds several new elements to the end of sequence */
CVAPI(void)  cvSeqPushMulti(CvSeq* seq, void* elements,
                             int count, int in_front CC_DEFAULT(0));

/* Removes several elements from the end of sequence and optionally saves them */
CVAPI(void)  cvSeqPopMulti(CvSeq* seq, void* elements,
                            int count, int in_front CC_DEFAULT(0));

/* Inserts a new element in the middle of sequence.
   cvSeqInsert(seq,0,elem) == cvSeqPushFront(seq,elem) */
CVAPI(char*)  cvSeqInsert(CvSeq* seq, int before_index,
                           void* element CC_DEFAULT(NULL));

/* Removes specified sequence element */
CVAPI(void)  cvSeqRemove(CvSeq* seq, int index);


/* Removes all the elements from the sequence. The freed memory
   can be reused later only by the same sequence unless cvClearMemStorage
   or cvRestoreMemStoragePos is called */
CVAPI(void)  cvClearSeq(CvSeq* seq);


/* Retrives pointer to specified sequence element.
   Negative indices are supported and mean counting from the end
   (e.g -1 means the last sequence element) */
CVAPI(char*)  cvGetSeqElem(const CvSeq* seq, int index);

/* Calculates index of the specified sequence element.
   Returns -1 if element does not belong to the sequence */
CVAPI(int)  cvSeqElemIdx(const CvSeq* seq, const void* element,
                         CvSeqBlock** block CC_DEFAULT(NULL));

/* Initializes sequence writer. The new elements will be added to the end of sequence */
CVAPI(void)  cvStartAppendToSeq(CvSeq* seq, CvSeqWriter* writer);


/* Combination of cvCreateSeq and cvStartAppendToSeq */
CVAPI(void)  cvStartWriteSeq(int seq_flags, int header_size,
                              int elem_size, CvMemStorage* storage,
                              CvSeqWriter* writer);

/* Closes sequence writer, updates sequence header and returns pointer
   to the resultant sequence
   (which may be useful if the sequence was created using cvStartWriteSeq))
*/
CVAPI(CvSeq*)  cvEndWriteSeq(CvSeqWriter* writer);


/* Updates sequence header. May be useful to get access to some of previously
   written elements via cvGetSeqElem or sequence reader */
CVAPI(void)   cvFlushSeqWriter(CvSeqWriter* writer);


/* Initializes sequence reader.
   The sequence can be read in forward or backward direction */
CVAPI(void) cvStartReadSeq(const CvSeq* seq, CvSeqReader* reader,
                           int reverse CC_DEFAULT(0));


/* Returns current sequence reader position (currently observed sequence element) */
CVAPI(int)  cvGetSeqReaderPos(CvSeqReader* reader);


/* Changes sequence reader position. It may seek to an absolute or
   to relative to the current position */
CVAPI(void)   cvSetSeqReaderPos(CvSeqReader* reader, int index,
                                 int is_relative CC_DEFAULT(0));

/* Copies sequence content to a continuous piece of memory */
CVAPI(void*)  cvCvtSeqToArray(const CvSeq* seq, void* elements,
                               CSlice slice CC_DEFAULT(CC_WHOLE_SEQ));

/* Creates sequence header for array.
   After that all the operations on sequences that do not alter the content
   can be applied to the resultant sequence */
CVAPI(CvSeq*) cvMakeSeqHeaderForArray(int seq_type, int header_size,
                                       int elem_size, void* elements, int total,
                                       CvSeq* seq, CvSeqBlock* block);

/* Extracts sequence slice (with or without copying sequence elements) */
CVAPI(CvSeq*) cvSeqSlice(const CvSeq* seq, CSlice slice,
                         CvMemStorage* storage CC_DEFAULT(NULL),
                         int copy_data CC_DEFAULT(0));

CC_INLINE CvSeq* cvCloneSeq(const CvSeq* seq, CvMemStorage* storage CC_DEFAULT(NULL))
{
    return cvSeqSlice(seq, CC_WHOLE_SEQ, storage, 1);
}

/* Removes sequence slice */
CVAPI(void)  cvSeqRemoveSlice(CvSeq* seq, CSlice slice);

/* Inserts a sequence or array into another sequence */
CVAPI(void)  cvSeqInsertSlice(CvSeq* seq, int before_index, const img_t* from_arr);

/* a < b ? -1 : a > b ? 1 : 0 */
typedef int (C_CDECL* CvCmpFunc)(const void* a, const void* b, void* userdata);

/* Sorts sequence in-place given element comparison function */
CVAPI(void) cvSeqSort(CvSeq* seq, CvCmpFunc func, void* userdata CC_DEFAULT(NULL));

/* Finds element in a [sorted] sequence */
CVAPI(char*) cvSeqSearch(CvSeq* seq, const void* elem, CvCmpFunc func,
                          int is_sorted, int* elem_idx,
                          void* userdata CC_DEFAULT(NULL));

/* Reverses order of sequence elements in-place */
CVAPI(void) cvSeqInvert(CvSeq* seq);

/* Splits sequence into one or more equivalence classes using the specified criteria */
CVAPI(int)  cvSeqPartition(const CvSeq* seq, CvMemStorage* storage,
                            CvSeq** labels, CvCmpFunc is_equal, void* userdata);

/************ Internal sequence functions ************/
CVAPI(void)  cvChangeSeqBlock(void* reader, int direction);
CVAPI(void)  cvCreateSeqBlock(CvSeqWriter* writer);


/* Creates a new set */
CVAPI(CvSet*)  cvCreateSet(int set_flags, int header_size,
                            int elem_size, CvMemStorage* storage);

/* Adds new element to the set and returns pointer to it */
CVAPI(int)  cvSetAdd(CvSet* set_header, CvSetElem* elem CC_DEFAULT(NULL),
                      CvSetElem** inserted_elem CC_DEFAULT(NULL));

/* Fast variant of cvSetAdd */
CC_INLINE  CvSetElem* cvSetNew(CvSet* set_header)
{
    CvSetElem* elem = set_header->free_elems;
    if(elem)
    {
        set_header->free_elems = elem->next_free;
        elem->flags = elem->flags & CC_SET_ELEM_IDX_MASK;
        set_header->active_count++;
    }
    else
        cvSetAdd(set_header, NULL, (CvSetElem**)&elem);
    return elem;
}

/* Removes set element given its pointer */
CC_INLINE  void cvSetRemoveByPtr(CvSet* set_header, void* elem)
{
    CvSetElem* _elem = (CvSetElem*)elem;
    assert(_elem->flags >= 0 /*&& (elem->flags & CC_SET_ELEM_IDX_MASK) < set_header->total*/);
    _elem->next_free = set_header->free_elems;
    _elem->flags = (_elem->flags & CC_SET_ELEM_IDX_MASK) | CC_SET_ELEM_FREE_FLAG;
    set_header->free_elems = _elem;
    set_header->active_count--;
}

/* Removes element from the set by its index  */
CVAPI(void)   cvSetRemove(CvSet* set_header, int index);

/* Returns a set element by index. If the element doesn't belong to the set,
   NULL is returned */
CC_INLINE CvSetElem* cvGetSetElem(const CvSet* set_header, int index)
{
    CvSetElem* elem = (CvSetElem*)cvGetSeqElem((CvSeq*)set_header, index);
    return elem && CC_IS_SET_ELEM(elem) ? elem : 0;
}

/* Removes all the elements from the set */
CVAPI(void)  cvClearSet(CvSet* set_header);

/* Creates new graph */
CVAPI(CvGraph*)  cvCreateGraph(int graph_flags, int header_size,
                                int vtx_size, int edge_size,
                                CvMemStorage* storage);

/* Adds new vertex to the graph */
CVAPI(int)  cvGraphAddVtx(CvGraph* graph, const CvGraphVtx* vtx CC_DEFAULT(NULL),
                           CvGraphVtx** inserted_vtx CC_DEFAULT(NULL));


/* Removes vertex from the graph together with all incident edges */
CVAPI(int)  cvGraphRemoveVtx(CvGraph* graph, int index);
CVAPI(int)  cvGraphRemoveVtxByPtr(CvGraph* graph, CvGraphVtx* vtx);


/* Link two vertices specifed by indices or pointers if they
   are not connected or return pointer to already existing edge
   connecting the vertices.
   Functions return 1 if a new edge was created, 0 otherwise */
CVAPI(int)  cvGraphAddEdge(CvGraph* graph,
                            int start_idx, int end_idx,
                            const CvGraphEdge* edge CC_DEFAULT(NULL),
                            CvGraphEdge** inserted_edge CC_DEFAULT(NULL));

CVAPI(int)  cvGraphAddEdgeByPtr(CvGraph* graph,
                               CvGraphVtx* start_vtx, CvGraphVtx* end_vtx,
                               const CvGraphEdge* edge CC_DEFAULT(NULL),
                               CvGraphEdge** inserted_edge CC_DEFAULT(NULL));

/* Remove edge connecting two vertices */
CVAPI(void)  cvGraphRemoveEdge(CvGraph* graph, int start_idx, int end_idx);
CVAPI(void)  cvGraphRemoveEdgeByPtr(CvGraph* graph, CvGraphVtx* start_vtx,
                                     CvGraphVtx* end_vtx);

/* Find edge connecting two vertices */
CVAPI(CvGraphEdge*)  cvFindGraphEdge(const CvGraph* graph, int start_idx, int end_idx);
CVAPI(CvGraphEdge*)  cvFindGraphEdgeByPtr(const CvGraph* graph,
                                           const CvGraphVtx* start_vtx,
                                           const CvGraphVtx* end_vtx);
#define cvGraphFindEdge cvFindGraphEdge
#define cvGraphFindEdgeByPtr cvFindGraphEdgeByPtr

/* Remove all vertices and edges from the graph */
CVAPI(void)  cvClearGraph(CvGraph* graph);


/* Count number of edges incident to the vertex */
CVAPI(int)  cvGraphVtxDegree(const CvGraph* graph, int vtx_idx);
CVAPI(int)  cvGraphVtxDegreeByPtr(const CvGraph* graph, const CvGraphVtx* vtx);


/* Retrieves graph vertex by given index */
#define cvGetGraphVtx(graph, idx) (CvGraphVtx*)cvGetSetElem((CvSet*)(graph), (idx))

/* Retrieves index of a graph vertex given its pointer */
#define cvGraphVtxIdx(graph, vtx) ((vtx)->flags & CC_SET_ELEM_IDX_MASK)

/* Retrieves index of a graph edge given its pointer */
#define cvGraphEdgeIdx(graph, edge) ((edge)->flags & CC_SET_ELEM_IDX_MASK)

#define cvGraphGetVtxCount(graph) ((graph)->active_count)
#define cvGraphGetEdgeCount(graph) ((graph)->edges->active_count)

#define  CC_GRAPH_VERTEX        1
#define  CC_GRAPH_TREE_EDGE     2
#define  CC_GRAPH_BACK_EDGE     4
#define  CC_GRAPH_FORWARD_EDGE  8
#define  CC_GRAPH_CROSS_EDGE    16
#define  CC_GRAPH_ANY_EDGE      30
#define  CC_GRAPH_NEW_TREE      32
#define  CC_GRAPH_BACKTRACKING  64
#define  CC_GRAPH_OVER          -1

#define  CC_GRAPH_ALL_ITEMS    -1

/* flags for graph vertices and edges */
#define  CC_GRAPH_ITEM_VISITED_FLAG  (1 << 30)
#define  CC_IS_GRAPH_VERTEX_VISITED(vtx) \
    (((CvGraphVtx*)(vtx))->flags & CC_GRAPH_ITEM_VISITED_FLAG)
#define  CC_IS_GRAPH_EDGE_VISITED(edge) \
    (((CvGraphEdge*)(edge))->flags & CC_GRAPH_ITEM_VISITED_FLAG)
#define  CC_GRAPH_SEARCH_TREE_NODE_FLAG   (1 << 29)
#define  CC_GRAPH_FORWARD_EDGE_FLAG       (1 << 28)

typedef struct CvGraphScanner
{
    CvGraphVtx* vtx;       /* current graph vertex (or current edge origin) */
    CvGraphVtx* dst;       /* current graph edge destination vertex */
    CvGraphEdge* edge;     /* current edge */

    CvGraph* graph;        /* the graph */
    CvSeq*   stack;        /* the graph vertex stack */
    int      index;        /* the lower bound of certainly visited vertices */
    int      mask;         /* event mask */
}
CvGraphScanner;

/* Creates new graph scanner. */
CVAPI(CvGraphScanner*)  cvCreateGraphScanner(CvGraph* graph,
                                             CvGraphVtx* vtx CC_DEFAULT(NULL),
                                             int mask CC_DEFAULT(CC_GRAPH_ALL_ITEMS));

/* Releases graph scanner. */
CVAPI(void) cvReleaseGraphScanner(CvGraphScanner** scanner);

/* Get next graph element */
CVAPI(int)  cvNextGraphItem(CvGraphScanner* scanner);

/* Creates a copy of graph */
CVAPI(CvGraph*) cvCloneGraph(const CvGraph* graph, CvMemStorage* storage);

/****************************************************************************************\
*                                     Drawing                                            *
\****************************************************************************************/

/****************************************************************************************\
*       Drawing functions work with images/matrices of arbitrary type.                   *
*       For color images the channel order is BGR[A]                                     *
*       Antialiasing is supported only for 8-bit image now.                              *
*       All the functions include parameter color that means rgb value (that may be      *
*       constructed with CC_RGB macro) for color images and brightness                   *
*       for grayscale images.                                                            *
*       If a drawn figure is partially or completely outside of the image, it is clipped.*
\****************************************************************************************/

#define CC_RGB(r, g, b)  cScalar((b), (g), (r), 0)
#define CC_FILLED -1

#define CC_AA 16

/* Draws 4-connected, 8-connected or antialiased line segment connecting two points */
CVAPI(void)  cvLine(img_t* img, CPoint pt1, CPoint pt2,
                     CScalar color, int thickness CC_DEFAULT(1),
                     int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0));

/* Draws a rectangle given two opposite corners of the rectangle (pt1 & pt2),
   if thickness<0 (e.g. thickness == CC_FILLED), the filled box is drawn */
CVAPI(void)  cvRectangle(img_t* img, CPoint pt1, CPoint pt2,
                          CScalar color, int thickness CC_DEFAULT(1),
                          int line_type CC_DEFAULT(8),
                          int shift CC_DEFAULT(0));

/* Draws a circle with specified center and radius.
   Thickness works in the same way as with cvRectangle */
CVAPI(void)  cvCircle(img_t* img, CPoint center, int radius,
                       CScalar color, int thickness CC_DEFAULT(1),
                       int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0));

/* Draws ellipse outline, filled ellipse, elliptic arc or filled elliptic sector,
   depending on <thickness>, <start_angle> and <end_angle> parameters. The resultant figure
   is rotated by <angle>. All the angles are in degrees */
CVAPI(void)  cvEllipse(img_t* img, CPoint center, CSize axes,
                        double angle, double start_angle, double end_angle,
                        CScalar color, int thickness CC_DEFAULT(1),
                        int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0));

CC_INLINE  void  cvEllipseBox(img_t* img, CBox2D box, CScalar color,
                               int thickness CC_DEFAULT(1),
                               int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0))
{
    CSize axes;
    axes.width = cRound(box.size.height*0.5);
    axes.height = cRound(box.size.width*0.5);
    
    cvEllipse(img, cvPointFrom32f(box.center), axes, box.angle,
               0, 360, color, thickness, line_type, shift);
}

/* Fills convex or monotonous polygon. */
CVAPI(void)  cvFillConvexPoly(img_t* img, CPoint* pts, int npts, CScalar color,
                               int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0));

/* Fills an area bounded by one or more arbitrary polygons */
CVAPI(void)  cvFillPoly(img_t* img, CPoint** pts, int* npts, int contours, CScalar color,
                         int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0));

/* Draws one or more polygonal curves */
CVAPI(void)  cvPolyLine(img_t* img, CPoint** pts, int* npts, int contours,
                         int is_closed, CScalar color, int thickness CC_DEFAULT(1),
                         int line_type CC_DEFAULT(8), int shift CC_DEFAULT(0));

#define cvDrawRect cvRectangle
#define cvDrawLine cvLine
#define cvDrawCircle cvCircle
#define cvDrawEllipse cvEllipse
#define cvDrawPolyLine cvPolyLine

/* Clips the line segment connecting *pt1 and *pt2
   by the rectangular window
   (0<=x<img_size.width, 0<=y<img_size.height). */
CVAPI(int) cvClipLine(CSize img_size, CPoint* pt1, CPoint* pt2);

/* Initializes line iterator. Initially, line_iterator->ptr will point
   to pt1 (or pt2, see left_to_right description) location in the image.
   Returns the number of pixels on the line between the ending points. */
CVAPI(int)  cvInitLineIterator(const img_t* image, CPoint pt1, CPoint pt2,
                                CLineIterator* line_iterator,
                                int connectivity CC_DEFAULT(8),
                                int left_to_right CC_DEFAULT(0));

/* Moves iterator to the next line point */
#define CC_NEXT_LINE_POINT(line_iterator)                     \
{                                                               \
    int _line_iterator_mask = (line_iterator).err < 0 ? -1 : 0; \
    (line_iterator).err += (line_iterator).minus_delta +        \
        ((line_iterator).plus_delta & _line_iterator_mask);     \
    (line_iterator).ptr += (line_iterator).minus_step +         \
        ((line_iterator).plus_step & _line_iterator_mask);      \
}


/* basic font types */
#define CC_FONT_HERSHEY_SIMPLEX         0
#define CC_FONT_HERSHEY_PLAIN           1
#define CC_FONT_HERSHEY_DUPLEX          2
#define CC_FONT_HERSHEY_COMPLEX         3 
#define CC_FONT_HERSHEY_TRIPLEX         4
#define CC_FONT_HERSHEY_COMPLEX_SMALL   5
#define CC_FONT_HERSHEY_SCRIPT_SIMPLEX  6
#define CC_FONT_HERSHEY_SCRIPT_COMPLEX  7

/* font flags */
#define CC_FONT_ITALIC                 16  

#define CC_FONT_VECTOR0    CC_FONT_HERSHEY_SIMPLEX

/* Renders text stroke with specified font and color at specified location.
   CvFont should be initialized with cvInitFont */
CVAPI(void)  cvPutText(img_t* img, const char* text, CPoint org,
                        const CvFont* font, CScalar color);

/* Calculates bounding box of text stroke (useful for alignment) */
CVAPI(void)  cvGetTextSize(const char* text_string, const CvFont* font,
                            CSize* text_size, int* baseline);

/* Unpacks color value, if arrtype is CC_8UC?, <color> is treated as
   packed color value, otherwise the first channels (depending on arrtype)
   of destination scalar are set to the same value = <color> */
CVAPI(CScalar)  cvColorToScalar(double packed_color, int arrtype);

/* Returns the polygon points which make up the given ellipse.  The ellipse is define by
   the box of size 'axes' rotated 'angle' around the 'center'.  A partial sweep
   of the ellipse arc can be done by spcifying arc_start and arc_end to be something
   other than 0 and 360, respectively.  The input array 'pts' must be large enough to
   hold the result.  The total number of points stored into 'pts' is returned by this
   function. */
CVAPI(int) cvEllipse2Poly(CPoint center, CSize axes,
                 int angle, int arc_start, int arc_end, CPoint * pts, int delta);

/* Draws contour outlines or filled interiors on the image */
CVAPI(void)  cvDrawContours(img_t *img, CvSeq* contour,
                            CScalar external_color, CScalar hole_color,
                            int max_level, int thickness CC_DEFAULT(1),
                            int line_type CC_DEFAULT(8),
                            CPoint offset CC_DEFAULT(cPoint(0,0)));

/* Does look-up transformation. Elements of the source array
   (that should be 8uC1 or 8sC1) are used as indexes in lutarr 256-element table */
CVAPI(void) cvLUT(const img_t* src, img_t* dst, const img_t* lut);


/******************* Iteration through the sequence tree *****************/
typedef struct CvTreeNodeIterator
{
    const void* node;
    int level;
    int max_level;
}
CvTreeNodeIterator;

CVAPI(void) cvInitTreeNodeIterator(CvTreeNodeIterator* tree_iterator,
                                   const void* first, int max_level);
CVAPI(void*) cvNextTreeNode(CvTreeNodeIterator* tree_iterator);
CVAPI(void*) cvPrevTreeNode(CvTreeNodeIterator* tree_iterator);

/* Inserts sequence into tree with specified "parent" sequence.
   If parent is equal to frame (e.g. the most external contour),
   then added contour will have null pointer to parent. */
CVAPI(void) cvInsertNodeIntoTree(void* node, void* parent, void* frame);

/* Removes contour from tree (together with the contour children). */
CVAPI(void) cvRemoveNodeFromTree(void* node, void* frame);

/* Gathers pointers to all the sequences,
   accessible from the <first>, to the single sequence */
CVAPI(CvSeq*) cvTreeToNodeSeq(const void* first, int header_size,
                              CvMemStorage* storage);

/* The function implements the K-means algorithm for clustering an array of sample
   vectors in a specified number of classes */
CVAPI(void)  cvKMeans2(const img_t* samples, int cluster_count,
                        img_t* labels, CTermCriteria termcrit);

/****************************************************************************************\
*                                    System functions                                    *
\****************************************************************************************/

/* Add the function pointers table with associated information to the IPP primitives list */
CVAPI(int)  cvRegisterModule(const CvModuleInfo* module_info);

/* Loads optimized functions from IPP, MKL etc. or switches back to pure C code */
CVAPI(int)  cvUseOptimized(int on_off);

/* Retrieves information about the registered modules and loaded optimized plugins */
CVAPI(void)  cvGetModuleInfo(const char* module_name,
                              const char** version,
                              const char** loaded_addon_plugins);

/* Get current OpenCV error status */
CVAPI(int) cvGetErrStatus(void);

/* Sets error status silently */
CVAPI(void) cvSetErrStatus(int status);

#define CC_ErrModeLeaf     0   /* Print error and exit program */
#define CC_ErrModeParent   1   /* Print error and continue */
#define CC_ErrModeSilent   2   /* Don't print and continue */

/* Retrives current error processing mode */
CVAPI(int)  cvGetErrMode(void);

/* Sets error processing mode, returns previously used mode */
CVAPI(int) cvSetErrMode(int mode);

/* Sets error status and performs some additonal actions (displaying message box,
   writing message to stderr, terminating application etc.)
   depending on the current error mode */
CVAPI(void) cvError(int status, const char* func_name,
                    const char* err_msg, const char* file_name, int line);

/* Retrieves textual description of the error given its code */
CVAPI(const char*) cvErrorStr(int status);

/* Retrieves detailed information about the last error occured */
CVAPI(int) cvGetErrInfo(const char** errcode_desc, const char** description,
                        const char** filename, int* line);

/* Maps IPP error codes to the counterparts from OpenCV */
CVAPI(int) cvErrorFromIppStatus(int ipp_status);

typedef int (C_CDECL *CvErrorCallback)(int status, const char* func_name,
                    const char* err_msg, const char* file_name, int line, void* userdata);

/* Assigns a new error-handling function */
CVAPI(CvErrorCallback) cvRedirectError(CvErrorCallback error_handler,
                                       void* userdata CC_DEFAULT(NULL),
                                       void** prev_userdata CC_DEFAULT(NULL));

/*
    Output to:
        cvNulDevReport - nothing
        cvStdErrReport - console(fprintf(stderr,...))
        cvGuiBoxReport - MessageBox(WIN32)
*/
CVAPI(int) cvNulDevReport(int status, const char* func_name, const char* err_msg,
                          const char* file_name, int line, void* userdata);

CVAPI(int) cvStdErrReport(int status, const char* func_name, const char* err_msg,
                          const char* file_name, int line, void* userdata);

CVAPI(int) cvGuiBoxReport(int status, const char* func_name, const char* err_msg,
                          const char* file_name, int line, void* userdata);

typedef void* (C_CDECL *CvAllocFunc)(size_t size, void* userdata);
typedef int (C_CDECL *CvFreeFunc)(void* pptr, void* userdata);

/* Set user-defined memory managment functions (substitutors for malloc and free) that
   will be called by cAlloc, cFree and higher-level functions (e.g. cvCreateImage) */
CVAPI(void) cvSetMemoryManager(CvAllocFunc alloc_func CC_DEFAULT(NULL),
                               CvFreeFunc free_func CC_DEFAULT(NULL),
                               void* userdata CC_DEFAULT(NULL));


typedef img_t* (C_STDCALL* Cv_iplCreateImageHeader)
                            (int,int,int,char*,char*,int,int,int,int,int,
                            IplROI*,img_t*,void*,IplTileInfo*);
typedef void (C_STDCALL* Cv_iplAllocateImageData)(img_t*,int,int);
typedef void (C_STDCALL* Cv_iplDeallocate)(img_t*,int);
typedef IplROI* (C_STDCALL* Cv_iplCreateROI)(int,int,int,int,int);
typedef img_t* (C_STDCALL* Cv_iplCloneImage)(const img_t*);

/* Makes OpenCV use IPL functions for img_t allocation/deallocation */
CVAPI(void) cvSetIPLAllocators(Cv_iplCreateImageHeader create_header,
                               Cv_iplAllocateImageData allocate_data,
                               Cv_iplDeallocate deallocate,
                               Cv_iplCreateROI create_roi,
                               Cv_iplCloneImage clone_image);

#define CC_TURN_ON_IMG_COMPATIBILITY()                                  \
    cvSetIPLAllocators(iplCreateImageHeader, iplAllocateImage,         \
                        iplDeallocate, iplCreateROI, iplCloneImage)

/****************************************************************************************\
*                                    Data Persistence                                    *
\****************************************************************************************/

/********************************** High-level functions ********************************/

/* opens existing or creates new file storage */
CVAPI(CvFileStorage*)  cvOpenFileStorage(const char* filename,
                                          CvMemStorage* memstorage,
                                          int flags);

/* closes file storage and deallocates buffers */
CVAPI(void) cvReleaseFileStorage(CvFileStorage** fs);

/* returns attribute value or 0 (NULL) if there is no such attribute */
CVAPI(const char*) cvAttrValue(const CvAttrList* attr, const char* attr_name);

/* starts writing compound structure (map or sequence) */
CVAPI(void) cvWriteStructBegin(CvFileStorage* fs, const char* name,
                                int struct_flags, const char* type_name CC_DEFAULT(NULL),
                                CvAttrList attributes CC_DEFAULT(cvAttrList()));

/* finishes writing compound structure */
CVAPI(void) cvWriteStructEnd(CvFileStorage* fs);

/* writes an integer */
CVAPI(void) cvWriteInt(CvFileStorage* fs, const char* name, int value);

/* writes a floating-point number */
CVAPI(void) cvWriteReal(CvFileStorage* fs, const char* name, double value);

/* writes a string */
CVAPI(void) cvWriteString(CvFileStorage* fs, const char* name,
                           const char* str, int quote CC_DEFAULT(0));

/* writes a comment */
CVAPI(void) cvWriteComment(CvFileStorage* fs, const char* comment,
                            int eol_comment);

/* writes instance of a standard type (matrix, image, sequence, graph etc.)
   or user-defined type */
CVAPI(void) cvWrite(CvFileStorage* fs, const char* name, const void* ptr,
                         CvAttrList attributes CC_DEFAULT(cvAttrList()));

/* starts the next stream */
CVAPI(void) cvStartNextStream(CvFileStorage* fs);

/* helper function: writes multiple integer or floating-point numbers */
CVAPI(void) cvWriteRawData(CvFileStorage* fs, const void* src,
                                int len, const char* dt);

/* returns the hash entry corresponding to the specified literal key string or 0
   if there is no such a key in the storage */
CVAPI(CvStringHashNode*) cvGetHashedKey(CvFileStorage* fs, const char* name,
                                        int len CC_DEFAULT(-1),
                                        int create_missing CC_DEFAULT(0));

/* returns file node with the specified key within the specified map
   (collection of named nodes) */
CVAPI(CvFileNode*) cvGetRootFileNode(const CvFileStorage* fs,
                                     int stream_index CC_DEFAULT(0));

/* returns file node with the specified key within the specified map
   (collection of named nodes) */
CVAPI(CvFileNode*) cvGetFileNode(CvFileStorage* fs, CvFileNode* map,
                                 const CvStringHashNode* key,
                                 int create_missing CC_DEFAULT(0));

/* this is a slower version of cvGetFileNode that takes the key as a literal string */
CVAPI(CvFileNode*) cvGetFileNodeByName(const CvFileStorage* fs,
                                       const CvFileNode* map,
                                       const char* name);

CC_INLINE int cvReadInt(const CvFileNode* node, int default_value CC_DEFAULT(0))
{
    return !node ? default_value :
        CC_NODE_IS_INT(node->tag) ? node->tt.i :
        CC_NODE_IS_REAL(node->tag) ? cRound(node->tt.f) : 0x7fffffff;
}


CC_INLINE int cvReadIntByName(const CvFileStorage* fs, const CvFileNode* map,
                         const char* name, int default_value CC_DEFAULT(0))
{
    return cvReadInt(cvGetFileNodeByName(fs, map, name), default_value);
}


CC_INLINE double cvReadReal(const CvFileNode* node, double default_value CC_DEFAULT(0.))
{
    return !node ? default_value :
        CC_NODE_IS_INT(node->tag) ? (double)node->tt.i :
        CC_NODE_IS_REAL(node->tag) ? node->tt.f : 1e300;
}


CC_INLINE double cvReadRealByName(const CvFileStorage* fs, const CvFileNode* map,
                        const char* name, double default_value CC_DEFAULT(0.))
{
    return cvReadReal(cvGetFileNodeByName(fs, map, name), default_value);
}


CC_INLINE const char* cvReadString(const CvFileNode* node,
                        const char* default_value CC_DEFAULT(NULL))
{
    return !node ? default_value : CC_NODE_IS_STRING(node->tag) ? node->tt.str.ptr : 0;
}


CC_INLINE const char* cvReadStringByName(const CvFileStorage* fs, const CvFileNode* map,
                        const char* name, const char* default_value CC_DEFAULT(NULL))
{
    return cvReadString(cvGetFileNodeByName(fs, map, name), default_value);
}


/* decodes standard or user-defined object and returns it */
CVAPI(void*) cvRead(CvFileStorage* fs, CvFileNode* node,
                        CvAttrList* attributes CC_DEFAULT(NULL));

/* decodes standard or user-defined object and returns it */
CC_INLINE void* cvReadByName(CvFileStorage* fs, const CvFileNode* map,
                              const char* name, CvAttrList* attributes CC_DEFAULT(NULL))
{
    return cvRead(fs, cvGetFileNodeByName(fs, map, name), attributes);
}


/* starts reading data from sequence or scalar numeric node */
CVAPI(void) cvStartReadRawData(const CvFileStorage* fs, const CvFileNode* src,
                               CvSeqReader* reader);

/* reads multiple numbers and stores them to array */
CVAPI(void) cvReadRawDataSlice(const CvFileStorage* fs, CvSeqReader* reader,
                               int count, void* dst, const char* dt);

/* combination of two previous functions for easier reading of whole sequences */
CVAPI(void) cvReadRawData(const CvFileStorage* fs, const CvFileNode* src,
                          void* dst, const char* dt);

/* writes a copy of file node to file storage */
CVAPI(void) cvWriteFileNode(CvFileStorage* fs, const char* new_node_name,
                            const CvFileNode* node, int embed);

/* returns name of file node */
CVAPI(const char*) cvGetFileNodeName(const CvFileNode* node);

/*********************************** Adding own types ***********************************/

CVAPI(void) cvRegisterType(const CvTypeInfo* info);
CVAPI(void) cvUnregisterType(const char* type_name);
CVAPI(CvTypeInfo*) cvFirstType(void);
CVAPI(CvTypeInfo*) cvFindType(const char* type_name);
CVAPI(CvTypeInfo*) cvTypeOf(const void* struct_ptr);

/* universal functions */
CVAPI(void) cvRelease(void** struct_ptr);
CVAPI(void*) cvClone(const void* struct_ptr);

/* simple API for reading/writing data */
CVAPI(void) cvSave(const char* filename, const void* struct_ptr,
                    const char* name CC_DEFAULT(NULL),
                    const char* comment CC_DEFAULT(NULL),
                    CvAttrList attributes CC_DEFAULT(cvAttrList()));
CVAPI(void*) cvLoad(const char* filename,
                     CvMemStorage* memstorage CC_DEFAULT(NULL),
                     const char* name CC_DEFAULT(NULL),
                     const char** real_name CC_DEFAULT(NULL));

/*********************************** Measuring Execution Time ***************************/

/* helper functions for RNG initialization and accurate time measurement:
   uses internal clock counter on x86 */
CVAPI(int64)  cvGetTickCount(void);
CVAPI(double) cvGetTickFrequency(void);

/*********************************** Multi-Threading ************************************/

/* retrieve/set the number of threads used in OpenMP implementations */
CVAPI(int)  cGetNumThreads(void);
CVAPI(void) cvSetNumThreads(int threads CC_DEFAULT(0));
/* get index of the thread being executed */
CVAPI(int)  cGetThreadNum(void);

#ifdef __cplusplus
}

#include "cxcore.hpp"
#endif

#endif /*_CXCORE_H_*/
