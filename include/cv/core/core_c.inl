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


#ifndef OPENCC_CORE_C_H
#define OPENCC_CORE_C_H

#include "opencv2/core/types_c.h"

#ifdef __cplusplus
#  ifdef _MSC_VER
/* disable warning C4190: 'function' has C-linkage specified, but returns UDT 'typename'
                          which is incompatible with C

   It is OK to disable it because we only extend few plain structures with
   C++ construrtors for simpler interoperability with C++ API of the library
*/
#    pragma warning(disable:4190)
#  elif defined __clang__ && __clang_major__ >= 3
#    pragma GCC diagnostic ignored "-Wreturn-type-c-linkage"
#  endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** @addtogroup core_c
    @{
*/

/****************************************************************************************\
*          Array allocation, deallocation, initialization and access to elements         *
\****************************************************************************************/

/** `malloc` wrapper.
   If there is no enough memory, the function
   (as well as other OpenCV functions that call cAlloc)
   raises an error. */
CVAPI(void*)  cAlloc( size_t size );

/** `free` wrapper.
   Here and further all the memory releasing functions
   (that all call cFree) take double pointer in order to
   to clear pointer to the data after releasing it.
   Passing pointer to NULL pointer is Ok: nothing happens in this case
*/
CVAPI(void)   cvFree_( void* ptr );
#define cFree(ptr) (cvFree_(*(ptr)), *(ptr)=0)

/** @brief Creates an image header but does not allocate the image data.

@param size Image width and height
@param depth Image depth (see cvCreateImage )
@param channels Number of channels (see cvCreateImage )
 */
CVAPI(img_t*)  cvCreateImageHeader( CSize size, int depth, int channels );

/** @brief Initializes an image header that was previously allocated.

The returned img_t\* points to the initialized header.
@param image Image header to initialize
@param size Image width and height
@param depth Image depth (see cvCreateImage )
@param channels Number of channels (see cvCreateImage )
@param origin Top-left IMG_ORIGIN_TL or bottom-left IMG_ORIGIN_BL
@param align Alignment for image rows, typically 4 or 8 bytes
 */
CVAPI(img_t*) cvInitImageHeader( img_t* image, CSize size, int depth,
                                   int channels, int origin CC_DEFAULT(0),
                                   int align CC_DEFAULT(4));

/** @brief Creates an image header and allocates the image data.

This function call is equivalent to the following code:
@code
    header = cvCreateImageHeader(size, depth, channels);
    cvCreateData(header);
@endcode
@param size Image width and height
@param depth Bit depth of image elements. See img_t for valid depths.
@param channels Number of channels per pixel. See img_t for details. This function only creates
images with interleaved channels.
 */
CVAPI(img_t*)  cvCreateImage( CSize size, int depth, int channels );

/** @brief Deallocates an image header.

This call is an analogue of :
@code
    if(image )
    {
        iplDeallocate(*image, IMG_IMAGE_HEADER | IMG_IMAGE_ROI);
        *image = 0;
    }
@endcode
but it does not use IPL functions by default (see the CC_TURN_ON_IMG_COMPATIBILITY macro).
@param image Double pointer to the image header
 */
CVAPI(void)  cvReleaseImageHeader( img_t** image );

/** @brief Deallocates the image header and the image data.

This call is a shortened form of :
@code
    if(*image )
    {
        cvReleaseData(*image);
        cvReleaseImageHeader(image);
    }
@endcode
@param image Double pointer to the image header
*/
CVAPI(void)  cvReleaseImage( img_t** image );

/** Creates a copy of IPL image (widthStep may differ) */
CVAPI(img_t*) cvCloneImage( const img_t* image );

/** @brief Sets the channel of interest in an img_t.

If the ROI is set to NULL and the coi is *not* 0, the ROI is allocated. Most OpenCV functions do
*not* support the COI setting, so to process an individual image/matrix channel one may copy (via
cvCopy or cvSplit) the channel to a separate image/matrix, process it and then copy the result
back (via cvCopy or cvMerge) if needed.
@param image A pointer to the image header
@param coi The channel of interest. 0 - all channels are selected, 1 - first channel is selected,
etc. Note that the channel indices become 1-based.
 */
CVAPI(void)  cvSetImageCOI( img_t* image, int coi );

/** @brief Returns the index of the channel of interest.

Returns the channel of interest of in an img_t. Returned values correspond to the coi in
cvSetImageCOI.
@param image A pointer to the image header
 */
CVAPI(int)  cvGetImageCOI( const img_t* image );

/** @brief Sets an image Region Of Interest (ROI) for a given rectangle.

If the original image ROI was NULL and the rect is not the whole image, the ROI structure is
allocated.

Most OpenCV functions support the use of ROI and treat the image rectangle as a separate image. For
example, all of the pixel coordinates are counted from the top-left (or bottom-left) corner of the
ROI, not the original image.
@param image A pointer to the image header
@param rect The ROI rectangle
 */
CVAPI(void)  cvSetImageROI( img_t* image, CRect rect );

/** @brief Resets the image ROI to include the entire image and releases the ROI structure.

This produces a similar result to the following, but in addition it releases the ROI structure. :
@code
    cvSetImageROI(image, cRect(0, 0, image->width, image->height ));
    cvSetImageCOI(image, 0);
@endcode
@param image A pointer to the image header
 */
CVAPI(void)  cvResetImageROI( img_t* image );

/** @brief Returns the image ROI.

If there is no ROI set, cRect(0,0,image-\>width,image-\>height) is returned.
@param image A pointer to the image header
 */
CVAPI(CRect) cvGetImageROI( const img_t* image );

/** @brief Creates a matrix header but does not allocate the matrix data.

The function allocates a new matrix header and returns a pointer to it. The matrix data can then be
allocated using cvCreateData or set explicitly to user-allocated data via cvSetData.
@param rows Number of rows in the matrix
@param cols Number of columns in the matrix
@param type Type of the matrix elements, see cvCreateMat
 */
CVAPI(img_t*)  cvCreateMatHeader( int rows, int cols, int type );

#define CC_AUTOSTEP  0x7fffffff

/** @brief Initializes a pre-allocated matrix header.

This function is often used to process raw data with OpenCV matrix functions. For example, the
following code computes the matrix product of two matrices, stored as ordinary arrays:
@code
    double a[] = { 1, 2, 3, 4,
                   5, 6, 7, 8,
                   9, 10, 11, 12 };

    double b[] = { 1, 5, 9,
                   2, 6, 10,
                   3, 7, 11,
                   4, 8, 12 };

    double c[9];
    img_t Ma, Mb, Mc ;

    cvInitMatHeader(&Ma, 3, 4, CC_64FC1, a);
    cvInitMatHeader(&Mb, 4, 3, CC_64FC1, b);
    cvInitMatHeader(&Mc, 3, 3, CC_64FC1, c);

    cvMatMulAdd(&Ma, &Mb, 0, &Mc);
    // the c array now contains the product of a (3x4) and b (4x3)
@endcode
@param mat A pointer to the matrix header to be initialized
@param rows Number of rows in the matrix
@param cols Number of columns in the matrix
@param type Type of the matrix elements, see cvCreateMat .
@param data Optional: data pointer assigned to the matrix header
@param step Optional: full row width in bytes of the assigned data. By default, the minimal
possible step is used which assumes there are no gaps between subsequent rows of the matrix.
 */
CVAPI(img_t*) cvInitMatHeader( img_t* mat, int rows, int cols,
                              int type, void* data CC_DEFAULT(NULL),
                              int step CC_DEFAULT(CC_AUTOSTEP) );

/** @brief Creates a matrix header and allocates the matrix data.

The function call is equivalent to the following code:
@code
    img_t* mat = cvCreateMatHeader(rows, cols, type);
    cvCreateData(mat);
@endcode
@param rows Number of rows in the matrix
@param cols Number of columns in the matrix
@param type The type of the matrix elements in the form
CC_\<bit depth\>\<S|U|F\>C\<number of channels\> , where S=signed, U=unsigned, F=float. For
example, CV _ 8UC1 means the elements are 8-bit unsigned and the there is 1 channel, and CV _
32SC2 means the elements are 32-bit signed and there are 2 channels.
 */
CVAPI(img_t*)  cvCreateMat( int rows, int cols, int type );

/** @brief Deallocates a matrix.

The function decrements the matrix data reference counter and deallocates matrix header. If the data
reference counter is 0, it also deallocates the data. :
@code
    if(*mat )
        cvDecRefData(*mat);
    cFree((void**)mat);
@endcode
@param mat Double pointer to the matrix
 */
CVAPI(void)  cvReleaseMat( img_t** mat );

/** @brief Decrements an array data reference counter.

The function decrements the data reference counter in a img_t or CvMatND if the reference counter

pointer is not NULL. If the counter reaches zero, the data is deallocated. In the current
implementation the reference counter is not NULL only if the data was allocated using the
cvCreateData function. The counter will be NULL in other cases such as: external data was assigned
to the header using cvSetData, header is part of a larger matrix or image, or the header was
converted from an image or n-dimensional matrix header.
@param arr Pointer to an array header
 */
CC_INLINE  void  cvDecRefData( img_t* arr )
{
    if( CC_IS_MAT( arr ))
    {
        img_t* mat = (img_t*)arr;
        mat->data.ptr = NULL;
        if( mat->refcount != NULL && --*mat->refcount == 0 )
            cFree( &mat->refcount );
        mat->refcount = NULL;
    }
    else if( CC_IS_MATND( arr ))
    {
        CvMatND* mat = (CvMatND*)arr;
        mat->data.ptr = NULL;
        if( mat->refcount != NULL && --*mat->refcount == 0 )
            cFree( &mat->refcount );
        mat->refcount = NULL;
    }
}

/** @brief Increments array data reference counter.

The function increments img_t or CvMatND data reference counter and returns the new counter value if
the reference counter pointer is not NULL, otherwise it returns zero.
@param arr Array header
 */
CC_INLINE  int  cvIncRefData( img_t* arr )
{
    int refcount = 0;
    if( CC_IS_MAT( arr ))
    {
        img_t* mat = (img_t*)arr;
        if( mat->refcount != NULL )
            refcount = ++*mat->refcount;
    }
    else if( CC_IS_MATND( arr ))
    {
        CvMatND* mat = (CvMatND*)arr;
        if( mat->refcount != NULL )
            refcount = ++*mat->refcount;
    }
    return refcount;
}


/** Creates an exact copy of the input matrix (except, may be, step value) */
CVAPI(img_t*) cvCloneMat( const img_t* mat );


/** @brief Returns matrix header corresponding to the rectangular sub-array of input image or matrix.

The function returns header, corresponding to a specified rectangle of the input array. In other

words, it allows the user to treat a rectangular part of input array as a stand-alone array. ROI is
taken into account by the function so the sub-array of ROI is actually extracted.
@param arr Input array
@param submat Pointer to the resultant sub-array header
@param rect Zero-based coordinates of the rectangle of interest
 */
CVAPI(img_t*) cvGetSubRect( const img_t* arr, img_t* submat, CRect rect );
#define cvGetSubArr cvGetSubRect

/** @brief Returns array row or row span.

The functions return the header, corresponding to a specified row/row span of the input array.
cvGetRow(arr, submat, row) is a shortcut for cvGetRows(arr, submat, row, row+1).
@param arr Input array
@param submat Pointer to the resulting sub-array header
@param start_row Zero-based index of the starting row (inclusive) of the span
@param end_row Zero-based index of the ending row (exclusive) of the span
@param delta_row Index step in the row span. That is, the function extracts every delta_row -th
row from start_row and up to (but not including) end_row .
 */
CVAPI(img_t*) cvGetRows( const img_t* arr, img_t* submat,
                        int start_row, int end_row,
                        int delta_row CC_DEFAULT(1));

/** @overload
@param arr Input array
@param submat Pointer to the resulting sub-array header
@param row Zero-based index of the selected row
*/
CC_INLINE  img_t*  cvGetRow( const img_t* arr, img_t* submat, int row )
{
    return cvGetRows( arr, submat, row, row + 1, 1 );
}


/** @brief Returns one of more array columns.

The functions return the header, corresponding to a specified column span of the input array. That

is, no data is copied. Therefore, any modifications of the submatrix will affect the original array.
If you need to copy the columns, use cvCloneMat. cvGetCol(arr, submat, col) is a shortcut for
cvGetCols(arr, submat, col, col+1).
@param arr Input array
@param submat Pointer to the resulting sub-array header
@param start_col Zero-based index of the starting column (inclusive) of the span
@param end_col Zero-based index of the ending column (exclusive) of the span
 */
CVAPI(img_t*) cvGetCols( const img_t* arr, img_t* submat,
                        int start_col, int end_col );

/** @overload
@param arr Input array
@param submat Pointer to the resulting sub-array header
@param col Zero-based index of the selected column
*/
CC_INLINE  img_t*  cvGetCol( const img_t* arr, img_t* submat, int col )
{
    return cvGetCols( arr, submat, col, col + 1 );
}

/** @brief Returns one of array diagonals.

The function returns the header, corresponding to a specified diagonal of the input array.
@param arr Input array
@param submat Pointer to the resulting sub-array header
@param diag Index of the array diagonal. Zero value corresponds to the main diagonal, -1
corresponds to the diagonal above the main, 1 corresponds to the diagonal below the main, and so
forth.
 */
CVAPI(img_t*) cvGetDiag( const img_t* arr, img_t* submat,
                            int diag CC_DEFAULT(0));

/** low-level scalar <-> raw data conversion functions */
CVAPI(void) cvScalarToRawData( const CScalar* scalar, void* data, int type,
                              int extend_to_12 CC_DEFAULT(0) );

CVAPI(void) cvRawDataToScalar( const void* data, int type, CScalar* scalar );

/** @brief Creates a new matrix header but does not allocate the matrix data.

The function allocates a header for a multi-dimensional dense array. The array data can further be
allocated using cvCreateData or set explicitly to user-allocated data via cvSetData.
@param dims Number of array dimensions
@param sizes Array of dimension sizes
@param type Type of array elements, see cvCreateMat
 */
CVAPI(CvMatND*)  cvCreateMatNDHeader( int dims, const int* sizes, int type );

/** @brief Creates the header and allocates the data for a multi-dimensional dense array.

This function call is equivalent to the following code:
@code
    CvMatND* mat = cvCreateMatNDHeader(dims, sizes, type);
    cvCreateData(mat);
@endcode
@param dims Number of array dimensions. This must not exceed CC_MAX_DIM (32 by default, but can be
changed at build time).
@param sizes Array of dimension sizes.
@param type Type of array elements, see cvCreateMat .
 */
CVAPI(CvMatND*)  cvCreateMatND( int dims, const int* sizes, int type );

/** @brief Initializes a pre-allocated multi-dimensional array header.

@param mat A pointer to the array header to be initialized
@param dims The number of array dimensions
@param sizes An array of dimension sizes
@param type Type of array elements, see cvCreateMat
@param data Optional data pointer assigned to the matrix header
 */
CVAPI(CvMatND*)  cvInitMatNDHeader( CvMatND* mat, int dims, const int* sizes,
                                    int type, void* data CC_DEFAULT(NULL) );

/** @brief Deallocates a multi-dimensional array.

The function decrements the array data reference counter and releases the array header. If the
reference counter reaches 0, it also deallocates the data. :
@code
    if(*mat )
        cvDecRefData(*mat);
    cFree((void**)mat);
@endcode
@param mat Double pointer to the array
 */
CC_INLINE  void  cvReleaseMatND( CvMatND** mat )
{
    cvReleaseMat( (img_t**)mat );
}

/** Creates a copy of CvMatND (except, may be, steps) */
CVAPI(CvMatND*) cvCloneMatND( const CvMatND* mat );

/** @brief Creates sparse array.

The function allocates a multi-dimensional sparse array. Initially the array contain no elements,
that is PtrND and other related functions will return 0 for every index.
@param dims Number of array dimensions. In contrast to the dense matrix, the number of dimensions is
practically unlimited (up to \f$2^{16}\f$ ).
@param sizes Array of dimension sizes
@param type Type of array elements. The same as for img_t
 */
CVAPI(CvSparseMat*)  cvCreateSparseMat( int dims, const int* sizes, int type );

/** @brief Deallocates sparse array.

The function releases the sparse array and clears the array pointer upon exit.
@param mat Double pointer to the array
 */
CVAPI(void)  cvReleaseSparseMat( CvSparseMat** mat );

/** Creates a copy of CvSparseMat (except, may be, zero items) */
CVAPI(CvSparseMat*) cvCloneSparseMat( const CvSparseMat* mat );

/** @brief Initializes sparse array elements iterator.

The function initializes iterator of sparse array elements and returns pointer to the first element,
or NULL if the array is empty.
@param mat Input array
@param mat_iterator Initialized iterator
 */
CVAPI(CvSparseNode*) cvInitSparseMatIterator( const CvSparseMat* mat,
                                              CvSparseMatIterator* mat_iterator );

/** @brief Returns the next sparse matrix element

The function moves iterator to the next sparse matrix element and returns pointer to it. In the
current version there is no any particular order of the elements, because they are stored in the
hash table. The sample below demonstrates how to iterate through the sparse matrix:
@code
    // print all the non-zero sparse matrix elements and compute their sum
    double sum = 0;
    int i, dims = cvGetDims(sparsemat);
    CvSparseMatIterator it;
    CvSparseNode* node = cvInitSparseMatIterator(sparsemat, &it);

    for(; node != 0; node = cvGetNextSparseNode(&it))
    {
        int* idx = CC_NODE_IDX(array, node);
        float val = *(float*)CC_NODE_VAL(array, node);
        printf("M");
        for(i = 0; i < dims; i++ )
            printf("[%d]", idx[i]);
        printf("=%g\n", val);

        sum += val;
    }

    printf("nTotal sum = %g\n", sum);
@endcode
@param mat_iterator Sparse array iterator
 */
CC_INLINE CvSparseNode* cvGetNextSparseNode( CvSparseMatIterator* mat_iterator )
{
    if( mat_iterator->node->next )
        return mat_iterator->node = mat_iterator->node->next;
    else
    {
        int idx;
        for( idx = ++mat_iterator->curidx; idx < mat_iterator->mat->hashsize; idx++ )
        {
            CvSparseNode* node = (CvSparseNode*)mat_iterator->mat->hashtable[idx];
            if( node )
            {
                mat_iterator->curidx = idx;
                return mat_iterator->node = node;
            }
        }
        return NULL;
    }
}


#define CC_MAX_ARR 10

/** matrix iterator: used for n-ary operations on dense arrays */
typedef struct CvNArrayIterator
{
    int count; /**< number of arrays */
    int dims; /**< number of dimensions to iterate */
    CSize size; /**< maximal common linear size: { width = size, height = 1 } */
    uchar* ptr[CC_MAX_ARR]; /**< pointers to the array slices */
    int stack[CC_MAX_DIM]; /**< for internal use */
    CvMatND* hdr[CC_MAX_ARR]; /**< pointers to the headers of the
                                 matrices that are processed */
}
CvNArrayIterator;

#define CC_NO_DEPTH_CHECK     1
#define CC_NO_CN_CHECK        2
#define CC_NO_SIZE_CHECK      4

/** initializes iterator that traverses through several arrays simulteneously
   (the function together with cvNextArraySlice is used for
    N-ari element-wise operations) */
CVAPI(int) cvInitNArrayIterator( int count, img_t** arrs,
                                 const img_t* mask, CvMatND* stubs,
                                 CvNArrayIterator* array_iterator,
                                 int flags CC_DEFAULT(0) );

/** returns zero value if iteration is finished, non-zero (slice length) otherwise */
CVAPI(int) cvNextNArraySlice( CvNArrayIterator* array_iterator );


/** @brief Returns type of array elements.

The function returns type of the array elements. In the case of img_t the type is converted to
img_t-like representation. For example, if the image has been created as:
@code
    img_t* img = cvCreateImage(cSize(640, 480), IMG_DEPTH_8U, 3);
@endcode
The code cvGetElemType(img) will return CC_8UC3.
@param arr Input array
 */
CVAPI(int) cvGetElemType( const img_t* arr );

/** @brief Return number of array dimensions

The function returns the array dimensionality and the array of dimension sizes. In the case of
img_t or img_t it always returns 2 regardless of number of image/matrix rows. For example, the
following code calculates total number of array elements:
@code
    int sizes[CC_MAX_DIM];
    int i, total = 1;
    int dims = cvGetDims(arr, size);
    for(i = 0; i < dims; i++ )
        total *= sizes[i];
@endcode
@param arr Input array
@param sizes Optional output vector of the array dimension sizes. For 2d arrays the number of rows
(height) goes first, number of columns (width) next.
 */
CVAPI(int) cvGetDims( const img_t* arr, int* sizes CC_DEFAULT(NULL) );


/** @brief Returns array size along the specified dimension.

@param arr Input array
@param index Zero-based dimension index (for matrices 0 means number of rows, 1 means number of
columns; for images 0 means height, 1 means width)
 */
CVAPI(int) cvGetDimSize( const img_t* arr, int index );


/** @brief Return pointer to a particular array element.

The functions return a pointer to a specific array element. Number of array dimension should match
to the number of indices passed to the function except for cvPtr1D function that can be used for
sequential access to 1D, 2D or nD dense arrays.

The functions can be used for sparse arrays as well - if the requested node does not exist they
create it and set it to zero.

All these as well as other functions accessing array elements ( cvGetND , cvGetRealND , cvSet
, cvSetND , cvSetRealND ) raise an error in case if the element index is out of range.
@param arr Input array
@param idx0 The first zero-based component of the element index
@param type Optional output parameter: type of matrix elements
 */
CVAPI(uchar*) cvPtr1D( const img_t* arr, int idx0, int* type CC_DEFAULT(NULL));
/** @overload */
CVAPI(uchar*) cvPtr2D( const img_t* arr, int idx0, int idx1, int* type CC_DEFAULT(NULL) );
/** @overload */
CVAPI(uchar*) cvPtr3D( const img_t* arr, int idx0, int idx1, int idx2,
                      int* type CC_DEFAULT(NULL));
/** @overload
@param arr Input array
@param idx Array of the element indices
@param type Optional output parameter: type of matrix elements
@param create_node Optional input parameter for sparse matrices. Non-zero value of the parameter
means that the requested element is created if it does not exist already.
@param precalc_hashval Optional input parameter for sparse matrices. If the pointer is not NULL,
the function does not recalculate the node hash value, but takes it from the specified location.
It is useful for speeding up pair-wise operations (TODO: provide an example)
*/
CVAPI(uchar*) cvPtrND( const img_t* arr, const int* idx, int* type CC_DEFAULT(NULL),
                      int create_node CC_DEFAULT(1),
                      unsigned* precalc_hashval CC_DEFAULT(NULL));

/** @brief Return a specific array element.

The functions return a specific array element. In the case of a sparse array the functions return 0
if the requested node does not exist (no new node is created by the functions).
@param arr Input array
@param idx0 The first zero-based component of the element index
 */
CVAPI(CScalar) cvGet1D( const img_t* arr, int idx0 );
/** @overload */
CVAPI(CScalar) cvGet2D( const img_t* arr, int idx0, int idx1 );
/** @overload */
CVAPI(CScalar) cvGet3D( const img_t* arr, int idx0, int idx1, int idx2 );
/** @overload
@param arr Input array
@param idx Array of the element indices
*/
CVAPI(CScalar) cvGetND( const img_t* arr, const int* idx );

/** @brief Return a specific element of single-channel 1D, 2D, 3D or nD array.

Returns a specific element of a single-channel array. If the array has multiple channels, a runtime
error is raised. Note that Get?D functions can be used safely for both single-channel and
multiple-channel arrays though they are a bit slower.

In the case of a sparse array the functions return 0 if the requested node does not exist (no new
node is created by the functions).
@param arr Input array. Must have a single channel.
@param idx0 The first zero-based component of the element index
 */
CVAPI(double) cvGetReal1D( const img_t* arr, int idx0 );
/** @overload */
CVAPI(double) cvGetReal2D( const img_t* arr, int idx0, int idx1 );
/** @overload */
CVAPI(double) cvGetReal3D( const img_t* arr, int idx0, int idx1, int idx2 );
/** @overload
@param arr Input array. Must have a single channel.
@param idx Array of the element indices
*/
CVAPI(double) cvGetRealND( const img_t* arr, const int* idx );

/** @brief Change the particular array element.

The functions assign the new value to a particular array element. In the case of a sparse array the
functions create the node if it does not exist yet.
@param arr Input array
@param idx0 The first zero-based component of the element index
@param value The assigned value
 */
CVAPI(void) cvSet1D( img_t* arr, int idx0, CScalar value );
/** @overload */
CVAPI(void) cvSet2D( img_t* arr, int idx0, int idx1, CScalar value );
/** @overload */
CVAPI(void) cvSet3D( img_t* arr, int idx0, int idx1, int idx2, CScalar value );
/** @overload
@param arr Input array
@param idx Array of the element indices
@param value The assigned value
*/
CVAPI(void) cvSetND( img_t* arr, const int* idx, CScalar value );

/** @brief Change a specific array element.

The functions assign a new value to a specific element of a single-channel array. If the array has
multiple channels, a runtime error is raised. Note that the Set\*D function can be used safely for
both single-channel and multiple-channel arrays, though they are a bit slower.

In the case of a sparse array the functions create the node if it does not yet exist.
@param arr Input array
@param idx0 The first zero-based component of the element index
@param value The assigned value
 */
CVAPI(void) cvSetReal1D( img_t* arr, int idx0, double value );
/** @overload */
CVAPI(void) cvSetReal2D( img_t* arr, int idx0, int idx1, double value );
/** @overload */
CVAPI(void) cvSetReal3D( img_t* arr, int idx0,
                        int idx1, int idx2, double value );
/** @overload
@param arr Input array
@param idx Array of the element indices
@param value The assigned value
*/
CVAPI(void) cvSetRealND( img_t* arr, const int* idx, double value );

/** clears element of ND dense array,
   in case of sparse arrays it deletes the specified node */
CVAPI(void) cvClearND( img_t* arr, const int* idx );

/** @brief Returns matrix header for arbitrary array.

The function returns a matrix header for the input array that can be a matrix - img_t, an image -
img_t, or a multi-dimensional dense array - CvMatND (the third option is allowed only if
allowND != 0) . In the case of matrix the function simply returns the input pointer. In the case of
img_t\* or CvMatND it initializes the header structure with parameters of the current image ROI
and returns &header. Because COI is not supported by img_t, it is returned separately.

The function provides an easy way to handle both types of arrays - img_t and img_t using the same
code. Input array must have non-zero data pointer, otherwise the function will report an error.

@note If the input array is img_t with planar data layout and COI set, the function returns the
pointer to the selected plane and COI == 0. This feature allows user to process img_t structures
with planar data layout, even though OpenCV does not support such images.
@param arr Input array
@param header Pointer to img_t structure used as a temporary buffer
@param coi Optional output parameter for storing COI
@param allowND If non-zero, the function accepts multi-dimensional dense arrays (CvMatND\*) and
returns 2D matrix (if CvMatND has two dimensions) or 1D matrix (when CvMatND has 1 dimension or
more than 2 dimensions). The CvMatND array must be continuous.
@sa cvGetImage, cvarrToMat.
 */
CVAPI(img_t*) cvGetMat( const img_t* arr, img_t* header,
                       int* coi CC_DEFAULT(NULL),
                       int allowND CC_DEFAULT(0));

/** @brief Returns image header for arbitrary array.

The function returns the image header for the input array that can be a matrix (img_t) or image
(img_t). In the case of an image the function simply returns the input pointer. In the case of
img_t it initializes an image_header structure with the parameters of the input matrix. Note that
if we transform img_t to img_t using cvGetMat and then transform img_t back to img_t using
this function, we will get different headers if the ROI is set in the original image.
@param arr Input array
@param image_header Pointer to img_t structure used as a temporary buffer
 */
CVAPI(img_t*) cvGetImage( const img_t* arr, img_t* image_header );


/** @brief Changes the shape of a multi-dimensional array without copying the data.

The function is an advanced version of cvReshape that can work with multi-dimensional arrays as
well (though it can work with ordinary images and matrices) and change the number of dimensions.

Below are the two samples from the cvReshape description rewritten using cvReshapeMatND:
@code
    img_t* color_img = cvCreateImage(cSize(320,240), IMG_DEPTH_8U, 3);
    img_t gray_img_hdr, *gray_img;
    gray_img = (img_t*)cvReshapeMatND(color_img, sizeof(gray_img_hdr), &gray_img_hdr, 1, 0, 0);
    ...
    int size[] = { 2, 2, 2 };
    CvMatND* mat = cvCreateMatND(3, size, CC_32F);
    img_t row_header, *row;
    row = (img_t*)cvReshapeMatND(mat, sizeof(row_header), &row_header, 0, 1, 0);
@endcode
In C, the header file for this function includes a convenient macro cvReshapeND that does away with
the sizeof_header parameter. So, the lines containing the call to cvReshapeMatND in the examples
may be replaced as follow:
@code
    gray_img = (img_t*)cvReshapeND(color_img, &gray_img_hdr, 1, 0, 0);
    ...
    row = (img_t*)cvReshapeND(mat, &row_header, 0, 1, 0);
@endcode
@param arr Input array
@param sizeof_header Size of output header to distinguish between img_t, img_t and CvMatND
output headers
@param header Output header to be filled
@param new_cn New number of channels. new_cn = 0 means that the number of channels remains
unchanged.
@param new_dims New number of dimensions. new_dims = 0 means that the number of dimensions
remains the same.
@param new_sizes Array of new dimension sizes. Only new_dims-1 values are used, because the
total number of elements must remain the same. Thus, if new_dims = 1, new_sizes array is not
used.
 */
CVAPI(img_t*) cvReshapeMatND( const img_t* arr,
                             int sizeof_header, img_t* header,
                             int new_cn, int new_dims, int* new_sizes );

#define cvReshapeND( arr, header, new_cn, new_dims, new_sizes )   \
      cvReshapeMatND( (arr), sizeof(*(header)), (header),         \
                      (new_cn), (new_dims), (new_sizes))

/** @brief Changes shape of matrix/image without copying data.

The function initializes the img_t header so that it points to the same data as the original array
but has a different shape - different number of channels, different number of rows, or both.

The following example code creates one image buffer and two image headers, the first is for a
320x240x3 image and the second is for a 960x240x1 image:
@code
    img_t* color_img = cvCreateImage(cSize(320,240), IMG_DEPTH_8U, 3);
    img_t gray_mat_hdr;
    img_t gray_img_hdr, *gray_img;
    cvReshape(color_img, &gray_mat_hdr, 1);
    gray_img = cvGetImage(&gray_mat_hdr, &gray_img_hdr);
@endcode
And the next example converts a 3x3 matrix to a single 1x9 vector:
@code
    img_t* mat = cvCreateMat(3, 3, CC_32F);
    img_t row_header, *row;
    row = cvReshape(mat, &row_header, 0, 1);
@endcode
@param arr Input array
@param header Output header to be filled
@param new_cn New number of channels. 'new_cn = 0' means that the number of channels remains
unchanged.
@param new_rows New number of rows. 'new_rows = 0' means that the number of rows remains
unchanged unless it needs to be changed according to new_cn value.
*/
CVAPI(img_t*) cvReshape( const img_t* arr, img_t* header,
                        int new_cn, int new_rows CC_DEFAULT(0) );

/** Repeats source 2d array several times in both horizontal and
   vertical direction to fill destination array */
CVAPI(void) cvRepeat( const img_t* src, img_t* dst );

/** @brief Allocates array data

The function allocates image, matrix or multi-dimensional dense array data. Note that in the case of
matrix types OpenCV allocation functions are used. In the case of img_t they are used unless
CC_TURN_ON_IMG_COMPATIBILITY() has been called before. In the latter case IPL functions are used
to allocate the data.
@param arr Array header
 */
CVAPI(void)  cvCreateData( img_t* arr );

/** @brief Releases array data.

The function releases the array data. In the case of img_t or CvMatND it simply calls
cvDecRefData(), that is the function can not deallocate external data. See also the note to
cvCreateData .
@param arr Array header
 */
CVAPI(void)  cvReleaseData( img_t* arr );

/** @brief Assigns user data to the array header.

The function assigns user data to the array header. Header should be initialized before using
cvCreateMatHeader, cvCreateImageHeader, cvCreateMatNDHeader, cvInitMatHeader,
cvInitImageHeader or cvInitMatNDHeader.
@param arr Array header
@param data User data
@param step Full row length in bytes
 */
CVAPI(void)  cvSetData( img_t* arr, void* data, int step );

/** @brief Retrieves low-level information about the array.

The function fills output variables with low-level information about the array data. All output

parameters are optional, so some of the pointers may be set to NULL. If the array is img_t with
ROI set, the parameters of ROI are returned.

The following example shows how to get access to array elements. It computes absolute values of the
array elements :
@code
    float* data;
    int step;
    CSize size;

    cvGetRawData(array, (uchar**)&data, &step, &size);
    step /= sizeof(data[0]);

    for(int y = 0; y < size.height; y++, data += step )
        for(int x = 0; x < size.width; x++ )
            data[x] = (float)fabs(data[x]);
@endcode
@param arr Array header
@param data Output pointer to the whole image origin or ROI origin if ROI is set
@param step Output full row length in bytes
@param roi_size Output ROI size
 */
CVAPI(void) cvGetRawData( const img_t* arr, uchar** data,
                         int* step CC_DEFAULT(NULL),
                         CSize* roi_size CC_DEFAULT(NULL));

/** @brief Returns size of matrix or image ROI.

The function returns number of rows (CSize::height) and number of columns (CSize::width) of the
input matrix or image. In the case of image the size of ROI is returned.
@param arr array header
 */
CVAPI(CSize) cvGetSize( const img_t* arr );

/** @brief Copies one array to another.

The function copies selected elements from an input array to an output array:

\f[\texttt{dst} (I)= \texttt{src} (I)  \quad \text{if} \quad \texttt{mask} (I)  \ne 0.\f]

If any of the passed arrays is of img_t type, then its ROI and COI fields are used. Both arrays
must have the same type, the same number of dimensions, and the same size. The function can also
copy sparse arrays (mask is not supported in this case).
@param src The source array
@param dst The destination array
@param mask Operation mask, 8-bit single channel array; specifies elements of the destination array
to be changed
 */
CVAPI(void)  cvCopy( const img_t* src, img_t* dst,
                     const img_t* mask CC_DEFAULT(NULL) );

/** @brief Sets every element of an array to a given value.

The function copies the scalar value to every selected element of the destination array:
\f[\texttt{arr} (I)= \texttt{value} \quad \text{if} \quad \texttt{mask} (I)  \ne 0\f]
If array arr is of img_t type, then is ROI used, but COI must not be set.
@param arr The destination array
@param value Fill value
@param mask Operation mask, 8-bit single channel array; specifies elements of the destination
array to be changed
 */
CVAPI(void)  cvSet( img_t* arr, CScalar value,
                    const img_t* mask CC_DEFAULT(NULL) );

/** @brief Clears the array.

The function clears the array. In the case of dense arrays (img_t, CvMatND or img_t),
cvZero(array) is equivalent to cvSet(array,cScalarAll(0),0). In the case of sparse arrays all the
elements are removed.
@param arr Array to be cleared
 */
CVAPI(void)  cvSetZero( img_t* arr );
#define cvZero  cvSetZero


/** Splits a multi-channel array into the set of single-channel arrays or
   extracts particular [color] plane */
CVAPI(void)  cvSplit( const img_t* src, img_t* dst0, img_t* dst1,
                      img_t* dst2, img_t* dst3 );

/** Merges a set of single-channel arrays into the single multi-channel array
   or inserts one particular [color] plane to the array */
CVAPI(void)  cvMerge( const img_t* src0, const img_t* src1,
                      const img_t* src2, const img_t* src3,
                      img_t* dst );

/** Copies several channels from input arrays to
   certain channels of output arrays */
CVAPI(void)  cvMixChannels( const img_t** src, int src_count,
                            img_t** dst, int dst_count,
                            const int* from_to, int pair_count );

/** @brief Converts one array to another with optional linear transformation.

The function has several different purposes, and thus has several different names. It copies one
array to another with optional scaling, which is performed first, and/or optional type conversion,
performed after:

\f[\texttt{dst} (I) =  \texttt{scale} \texttt{src} (I) + ( \texttt{shift} _0, \texttt{shift} _1,...)\f]

All the channels of multi-channel arrays are processed independently.

The type of conversion is done with rounding and saturation, that is if the result of scaling +
conversion can not be represented exactly by a value of the destination array element type, it is
set to the nearest representable value on the real axis.
@param src Source array
@param dst Destination array
@param scale Scale factor
@param shift Value added to the scaled source array elements
 */
CVAPI(void)  cvConvertScale( const img_t* src, img_t* dst,
                             double scale CC_DEFAULT(1),
                             double shift CC_DEFAULT(0) );
#define cvCvtScale cvConvertScale
#define cvScale  cvConvertScale
#define cvConvert( src, dst )  cvConvertScale( (src), (dst), 1, 0 )


/** Performs linear transformation on every source array element,
   stores absolute value of the result:
   dst(x,y,c) = abs(scale*src(x,y,c)+shift).
   destination array must have 8u type.
   In other cases one may use cvConvertScale + cvAbsDiffS */
CVAPI(void)  cvConvertScaleAbs( const img_t* src, img_t* dst,
                                double scale CC_DEFAULT(1),
                                double shift CC_DEFAULT(0) );
#define cvCvtScaleAbs  cvConvertScaleAbs


/** checks termination criteria validity and
   sets eps to default_eps (if it is not set),
   max_iter to default_max_iters (if it is not set)
*/
CVAPI(CTermCriteria) cvCheckTermCriteria( CTermCriteria criteria,
                                           double default_eps,
                                           int default_max_iters );

/****************************************************************************************\
*                   Arithmetic, logic and comparison operations                          *
\****************************************************************************************/

/** dst(mask) = src1(mask) + src2(mask) */
CVAPI(void)  cvAdd( const img_t* src1, const img_t* src2, img_t* dst,
                    const img_t* mask CC_DEFAULT(NULL));

/** dst(mask) = src(mask) + value */
CVAPI(void)  cvAddS( const img_t* src, CScalar value, img_t* dst,
                     const img_t* mask CC_DEFAULT(NULL));

/** dst(mask) = src1(mask) - src2(mask) */
CVAPI(void)  cvSub( const img_t* src1, const img_t* src2, img_t* dst,
                    const img_t* mask CC_DEFAULT(NULL));

/** dst(mask) = src(mask) - value = src(mask) + (-value) */
CC_INLINE  void  cvSubS( const img_t* src, CScalar value, img_t* dst,
                         const img_t* mask CC_DEFAULT(NULL))
{
    cvAddS( src, cScalar( -value.val[0], -value.val[1], -value.val[2], -value.val[3]),
            dst, mask );
}

/** dst(mask) = value - src(mask) */
CVAPI(void)  cvSubRS( const img_t* src, CScalar value, img_t* dst,
                      const img_t* mask CC_DEFAULT(NULL));

/** dst(idx) = src1(idx) * src2(idx) * scale
   (scaled element-wise multiplication of 2 arrays) */
CVAPI(void)  cvMul( const img_t* src1, const img_t* src2,
                    img_t* dst, double scale CC_DEFAULT(1) );

/** element-wise division/inversion with scaling:
    dst(idx) = src1(idx) * scale / src2(idx)
    or dst(idx) = scale / src2(idx) if src1 == 0 */
CVAPI(void)  cvDiv( const img_t* src1, const img_t* src2,
                    img_t* dst, double scale CC_DEFAULT(1));

/** dst = src1 * scale + src2 */
CVAPI(void)  cvScaleAdd( const img_t* src1, CScalar scale,
                         const img_t* src2, img_t* dst );
#define cvAXPY( A, real_scalar, B, C ) cvScaleAdd(A, cRealScalar(real_scalar), B, C)

/** dst = src1 * alpha + src2 * beta + gamma */
CVAPI(void)  cvAddWeighted( const img_t* src1, double alpha,
                            const img_t* src2, double beta,
                            double gamma, img_t* dst );

/** @brief Calculates the dot product of two arrays in Euclidean metrics.

The function calculates and returns the Euclidean dot product of two arrays.

\f[src1  \bullet src2 =  \sum _I ( \texttt{src1} (I)  \texttt{src2} (I))\f]

In the case of multiple channel arrays, the results for all channels are accumulated. In particular,
cvDotProduct(a,a) where a is a complex vector, will return \f$||\texttt{a}||^2\f$. The function can
process multi-dimensional arrays, row by row, layer by layer, and so on.
@param src1 The first source array
@param src2 The second source array
 */
CVAPI(double)  cvDotProduct( const img_t* src1, const img_t* src2 );

/** dst(idx) = src1(idx) & src2(idx) */
CVAPI(void) cvAnd( const img_t* src1, const img_t* src2,
                  img_t* dst, const img_t* mask CC_DEFAULT(NULL));

/** dst(idx) = src(idx) & value */
CVAPI(void) cvAndS( const img_t* src, CScalar value,
                   img_t* dst, const img_t* mask CC_DEFAULT(NULL));

/** dst(idx) = src1(idx) | src2(idx) */
CVAPI(void) cvOr( const img_t* src1, const img_t* src2,
                 img_t* dst, const img_t* mask CC_DEFAULT(NULL));

/** dst(idx) = src(idx) | value */
CVAPI(void) cvOrS( const img_t* src, CScalar value,
                  img_t* dst, const img_t* mask CC_DEFAULT(NULL));

/** dst(idx) = src1(idx) ^ src2(idx) */
CVAPI(void) cvXor( const img_t* src1, const img_t* src2,
                  img_t* dst, const img_t* mask CC_DEFAULT(NULL));

/** dst(idx) = src(idx) ^ value */
CVAPI(void) cvXorS( const img_t* src, CScalar value,
                   img_t* dst, const img_t* mask CC_DEFAULT(NULL));

/** dst(idx) = ~src(idx) */
CVAPI(void) cvNot( const img_t* src, img_t* dst );

/** dst(idx) = lower(idx) <= src(idx) < upper(idx) */
CVAPI(void) cvInRange( const img_t* src, const img_t* lower,
                      const img_t* upper, img_t* dst );

/** dst(idx) = lower <= src(idx) < upper */
CVAPI(void) cvInRangeS( const img_t* src, CScalar lower,
                       CScalar upper, img_t* dst );

#define CC_CMP_EQ   0
#define CC_CMP_GT   1
#define CC_CMP_GE   2
#define CC_CMP_LT   3
#define CC_CMP_LE   4
#define CC_CMP_NE   5

/** The comparison operation support single-channel arrays only.
   Destination image should be 8uC1 or 8sC1 */

/** dst(idx) = src1(idx) _cmp_op_ src2(idx) */
CVAPI(void) cvCmp( const img_t* src1, const img_t* src2, img_t* dst, int cmp_op );

/** dst(idx) = src1(idx) _cmp_op_ value */
CVAPI(void) cvCmpS( const img_t* src, double value, img_t* dst, int cmp_op );

/** dst(idx) = min(src1(idx),src2(idx)) */
CVAPI(void) cvMin( const img_t* src1, const img_t* src2, img_t* dst );

/** dst(idx) = max(src1(idx),src2(idx)) */
CVAPI(void) cvMax( const img_t* src1, const img_t* src2, img_t* dst );

/** dst(idx) = min(src(idx),value) */
CVAPI(void) cvMinS( const img_t* src, double value, img_t* dst );

/** dst(idx) = max(src(idx),value) */
CVAPI(void) cvMaxS( const img_t* src, double value, img_t* dst );

/** dst(x,y,c) = abs(src1(x,y,c) - src2(x,y,c)) */
CVAPI(void) cvAbsDiff( const img_t* src1, const img_t* src2, img_t* dst );

/** dst(x,y,c) = abs(src(x,y,c) - value(c)) */
CVAPI(void) cvAbsDiffS( const img_t* src, img_t* dst, CScalar value );
#define cvAbs( src, dst ) cvAbsDiffS( (src), (dst), cScalarAll(0))

/****************************************************************************************\
*                                Math operations                                         *
\****************************************************************************************/

/** Does cartesian->polar coordinates conversion.
   Either of output components (magnitude or angle) is optional */
CVAPI(void)  cvCartToPolar( const img_t* x, const img_t* y,
                            img_t* magnitude, img_t* angle CC_DEFAULT(NULL),
                            int angle_in_degrees CC_DEFAULT(0));

/** Does polar->cartesian coordinates conversion.
   Either of output components (magnitude or angle) is optional.
   If magnitude is missing it is assumed to be all 1's */
CVAPI(void)  cvPolarToCart( const img_t* magnitude, const img_t* angle,
                            img_t* x, img_t* y,
                            int angle_in_degrees CC_DEFAULT(0));

/** Does powering: dst(idx) = src(idx)^power */
CVAPI(void)  cvPow( const img_t* src, img_t* dst, double power );

/** Does exponention: dst(idx) = exp(src(idx)).
   Overflow is not handled yet. Underflow is handled.
   Maximal relative error is ~7e-6 for single-precision input */
CVAPI(void)  cvExp( const img_t* src, img_t* dst );

/** Calculates natural logarithms: dst(idx) = log(abs(src(idx))).
   Logarithm of 0 gives large negative number(~-700)
   Maximal relative error is ~3e-7 for single-precision output
*/
CVAPI(void)  cvLog( const img_t* src, img_t* dst );

/** Fast arctangent calculation */
CVAPI(float) cFastArctan( float y, float x );

/** Fast cubic root calculation */
CVAPI(float)  cCbrt( float value );

#define  CC_CHECK_RANGE    1
#define  CC_CHECK_QUIET    2
/** Checks array values for NaNs, Infs or simply for too large numbers
   (if CC_CHECK_RANGE is set). If CC_CHECK_QUIET is set,
   no runtime errors is raised (function returns zero value in case of "bad" values).
   Otherwise cvError is called */
CVAPI(int)  cvCheckArr( const img_t* arr, int flags CC_DEFAULT(0),
                        double min_val CC_DEFAULT(0), double max_val CC_DEFAULT(0));
#define cvCheckArray cvCheckArr

#define CC_RAND_UNI      0
#define CC_RAND_NORMAL   1

/** @brief Fills an array with random numbers and updates the RNG state.

The function fills the destination array with uniformly or normally distributed random numbers.
@param rng CRNG state initialized by cRNG
@param arr The destination array
@param dist_type Distribution type
> -   **CC_RAND_UNI** uniform distribution
> -   **CC_RAND_NORMAL** normal or Gaussian distribution
@param param1 The first parameter of the distribution. In the case of a uniform distribution it is
the inclusive lower boundary of the random numbers range. In the case of a normal distribution it
is the mean value of the random numbers.
@param param2 The second parameter of the distribution. In the case of a uniform distribution it
is the exclusive upper boundary of the random numbers range. In the case of a normal distribution
it is the standard deviation of the random numbers.
@sa randu, randn, RNG::fill.
 */
CVAPI(void) cvRandArr( CRNG* rng, img_t* arr, int dist_type,
                      CScalar param1, CScalar param2 );

CVAPI(void) cvRandShuffle( img_t* mat, CRNG* rng,
                           double iter_factor CC_DEFAULT(1.));

#define CC_SORT_EVERY_ROW 0
#define CC_SORT_EVERY_COLUMN 1
#define CC_SORT_ASCENDING 0
#define CC_SORT_DESCENDING 16

CVAPI(void) cvSort( const img_t* src, img_t* dst CC_DEFAULT(NULL),
                    img_t* idxmat CC_DEFAULT(NULL),
                    int flags CC_DEFAULT(0));

/** Finds real roots of a cubic equation */
CVAPI(int) cvSolveCubic( const img_t* coeffs, img_t* roots );

/** Finds all real and complex roots of a polynomial equation */
CVAPI(void) cvSolvePoly(const img_t* coeffs, img_t *roots2,
      int maxiter CC_DEFAULT(20), int fig CC_DEFAULT(100));

/****************************************************************************************\
*                                Matrix operations                                       *
\****************************************************************************************/

/** @brief Calculates the cross product of two 3D vectors.

The function calculates the cross product of two 3D vectors:
\f[\texttt{dst} =  \texttt{src1} \times \texttt{src2}\f]
or:
\f[\begin{array}{l} \texttt{dst} _1 =  \texttt{src1} _2  \texttt{src2} _3 -  \texttt{src1} _3  \texttt{src2} _2 \\ \texttt{dst} _2 =  \texttt{src1} _3  \texttt{src2} _1 -  \texttt{src1} _1  \texttt{src2} _3 \\ \texttt{dst} _3 =  \texttt{src1} _1  \texttt{src2} _2 -  \texttt{src1} _2  \texttt{src2} _1 \end{array}\f]
@param src1 The first source vector
@param src2 The second source vector
@param dst The destination vector
 */
CVAPI(void)  cvCrossProduct( const img_t* src1, const img_t* src2, img_t* dst );

/** Matrix transform: dst = A*B + C, C is optional */
#define cvMatMulAdd( src1, src2, src3, dst ) cvGEMM( (src1), (src2), 1., (src3), 1., (dst), 0 )
#define cvMatMul( src1, src2, dst )  cvMatMulAdd( (src1), (src2), NULL, (dst))

#define CC_GEMM_A_T 1
#define CC_GEMM_B_T 2
#define CC_GEMM_C_T 4
/** Extended matrix transform:
   dst = alpha*op(A)*op(B) + beta*op(C), where op(X) is X or X^T */
CVAPI(void)  cvGEMM( const img_t* src1, const img_t* src2, double alpha,
                     const img_t* src3, double beta, img_t* dst,
                     int tABC CC_DEFAULT(0));
#define cvMatMulAddEx cvGEMM

/** Transforms each element of source array and stores
   resultant vectors in destination array */
CVAPI(void)  cvTransform( const img_t* src, img_t* dst,
                          const img_t* transmat,
                          const img_t* shiftvec CC_DEFAULT(NULL));
#define cvMatMulAddS cvTransform

/** Does perspective transform on every element of input array */
CVAPI(void)  cvPerspectiveTransform( const img_t* src, img_t* dst,
                                     const img_t* mat );

/** Calculates (A-delta)*(A-delta)^T (order=0) or (A-delta)^T*(A-delta) (order=1) */
CVAPI(void) cvMulTransposed( const img_t* src, img_t* dst, int order,
                             const img_t* delta CC_DEFAULT(NULL),
                             double scale CC_DEFAULT(1.) );

/** Tranposes matrix. Square matrices can be transposed in-place */
CVAPI(void)  cvTranspose( const img_t* src, img_t* dst );
#define cvT cvTranspose

/** Completes the symmetric matrix from the lower (LtoR=0) or from the upper (LtoR!=0) part */
CVAPI(void)  cvCompleteSymm( img_t* matrix, int LtoR CC_DEFAULT(0) );

/** Mirror array data around horizontal (flip=0),
   vertical (flip=1) or both(flip=-1) axises:
   cvFlip(src) flips images vertically and sequences horizontally (inplace) */
CVAPI(void)  cvFlip( const img_t* src, img_t* dst CC_DEFAULT(NULL),
                     int flip_mode CC_DEFAULT(0));
#define cvMirror cvFlip


#define CC_SVD_MODIFY_A   1
#define CC_SVD_U_T        2
#define CC_SVD_V_T        4

/** Performs Singular Value Decomposition of a matrix */
CVAPI(void)   cvSVD( img_t* A, img_t* W, img_t* U CC_DEFAULT(NULL),
                     img_t* V CC_DEFAULT(NULL), int flags CC_DEFAULT(0));

/** Performs Singular Value Back Substitution (solves A*X = B):
   flags must be the same as in cvSVD */
CVAPI(void)   cvSVBkSb( const img_t* W, const img_t* U,
                        const img_t* V, const img_t* B,
                        img_t* X, int flags );

#define CC_LU  0
#define CC_SVD 1
#define CC_SVD_SYM 2
#define CC_CHOLESKY 3
#define CC_QR  4
#define CC_NORMAL 16

/** Inverts matrix */
CVAPI(double)  cvInvert( const img_t* src, img_t* dst,
                         int method CC_DEFAULT(CC_LU));
#define cvInv cvInvert

/** Solves linear system (src1)*(dst) = (src2)
   (returns 0 if src1 is a singular and CC_LU method is used) */
CVAPI(int)  cvSolve( const img_t* src1, const img_t* src2, img_t* dst,
                     int method CC_DEFAULT(CC_LU));

/** Calculates determinant of input matrix */
CVAPI(double) cvDet( const img_t* mat );

/** Calculates trace of the matrix (sum of elements on the main diagonal) */
CVAPI(CScalar) cvTrace( const img_t* mat );

/** Finds eigen values and vectors of a symmetric matrix */
CVAPI(void)  cvEigenVV( img_t* mat, img_t* evects, img_t* evals,
                        double eps CC_DEFAULT(0),
                        int lowindex CC_DEFAULT(-1),
                        int highindex CC_DEFAULT(-1));

///* Finds selected eigen values and vectors of a symmetric matrix */
//CVAPI(void)  cvSelectedEigenVV( img_t* mat, img_t* evects, img_t* evals,
//                                int lowindex, int highindex );

/** Makes an identity matrix (mat_ij = i == j) */
CVAPI(void)  cvSetIdentity( img_t* mat, CScalar value CC_DEFAULT(cRealScalar(1)) );

/** Fills matrix with given range of numbers */
CVAPI(img_t*)  cvRange( img_t* mat, double start, double end );

/**   @anchor core_c_CovarFlags
@name Flags for cvCalcCovarMatrix
@see cvCalcCovarMatrix
  @{
*/

/** flag for cvCalcCovarMatrix, transpose([v1-avg, v2-avg,...]) * [v1-avg,v2-avg,...] */
#define CC_COVAR_SCRAMBLED 0

/** flag for cvCalcCovarMatrix, [v1-avg, v2-avg,...] * transpose([v1-avg,v2-avg,...]) */
#define CC_COVAR_NORMAL    1

/** flag for cvCalcCovarMatrix, do not calc average (i.e. mean vector) - use the input vector instead
   (useful for calculating covariance matrix by parts) */
#define CC_COVAR_USE_AVG   2

/** flag for cvCalcCovarMatrix, scale the covariance matrix coefficients by number of the vectors */
#define CC_COVAR_SCALE     4

/** flag for cvCalcCovarMatrix, all the input vectors are stored in a single matrix, as its rows */
#define CC_COVAR_ROWS      8

/** flag for cvCalcCovarMatrix, all the input vectors are stored in a single matrix, as its columns */
#define CC_COVAR_COLS     16

/** @} */

/** Calculates covariation matrix for a set of vectors
@see @ref core_c_CovarFlags "flags"
*/
CVAPI(void)  cvCalcCovarMatrix( const img_t** vects, int count,
                                img_t* cov_mat, img_t* avg, int flags );

#define CC_PCA_DATA_AS_ROW 0
#define CC_PCA_DATA_AS_COL 1
#define CC_PCA_USE_AVG 2
CVAPI(void)  cvCalcPCA( const img_t* data, img_t* mean,
                        img_t* eigenvals, img_t* eigenvects, int flags );

CVAPI(void)  cvProjectPCA( const img_t* data, const img_t* mean,
                           const img_t* eigenvects, img_t* result );

CVAPI(void)  cvBackProjectPCA( const img_t* proj, const img_t* mean,
                               const img_t* eigenvects, img_t* result );

/** Calculates Mahalanobis(weighted) distance */
CVAPI(double)  cvMahalanobis( const img_t* vec1, const img_t* vec2, const img_t* mat );
#define cvMahalonobis  cvMahalanobis

/****************************************************************************************\
*                                    Array Statistics                                    *
\****************************************************************************************/

/** Finds sum of array elements */
CVAPI(CScalar)  cvSum( const img_t* arr );

/** Calculates number of non-zero pixels */
CVAPI(int)  cvCountNonZero( const img_t* arr );

/** Calculates mean value of array elements */
CVAPI(CScalar)  cvAvg( const img_t* arr, const img_t* mask CC_DEFAULT(NULL) );

/** Calculates mean and standard deviation of pixel values */
CVAPI(void)  cvAvgSdv( const img_t* arr, CScalar* mean, CScalar* std_dev,
                       const img_t* mask CC_DEFAULT(NULL) );

/** Finds global minimum, maximum and their positions */
CVAPI(void)  cvMinMaxLoc( const img_t* arr, double* min_val, double* max_val,
                          CPoint* min_loc CC_DEFAULT(NULL),
                          CPoint* max_loc CC_DEFAULT(NULL),
                          const img_t* mask CC_DEFAULT(NULL) );

/** @anchor core_c_NormFlags
  @name Flags for cvNorm and cvNormalize
  @{
*/
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
/** @} */

/** Finds norm, difference norm or relative difference norm for an array (or two arrays)
@see ref core_c_NormFlags "flags"
*/
CVAPI(double)  cvNorm( const img_t* arr1, const img_t* arr2 CC_DEFAULT(NULL),
                       int norm_type CC_DEFAULT(CC_L2),
                       const img_t* mask CC_DEFAULT(NULL) );

/** @see ref core_c_NormFlags "flags" */
CVAPI(void)  cvNormalize( const img_t* src, img_t* dst,
                          double a CC_DEFAULT(1.), double b CC_DEFAULT(0.),
                          int norm_type CC_DEFAULT(CC_L2),
                          const img_t* mask CC_DEFAULT(NULL) );

/** @anchor core_c_ReduceFlags
  @name Flags for cvReduce
  @{
*/
#define CC_REDUCE_SUM 0
#define CC_REDUCE_AVG 1
#define CC_REDUCE_MAX 2
#define CC_REDUCE_MIN 3
/** @} */

/** @see @ref core_c_ReduceFlags "flags" */
CVAPI(void)  cvReduce( const img_t* src, img_t* dst, int dim CC_DEFAULT(-1),
                       int op CC_DEFAULT(CC_REDUCE_SUM) );

/****************************************************************************************\
*                      Discrete Linear Transforms and Related Functions                  *
\****************************************************************************************/

/** @anchor core_c_DftFlags
  @name Flags for cvDFT, cvDCT and cvMulSpectrums
  @{
  */
#define CC_DXT_FORWARD  0
#define CC_DXT_INVERSE  1
#define CC_DXT_SCALE    2 /**< divide result by size of array */
#define CC_DXT_INV_SCALE (CC_DXT_INVERSE + CC_DXT_SCALE)
#define CC_DXT_INVERSE_SCALE CC_DXT_INV_SCALE
#define CC_DXT_ROWS     4 /**< transform each row individually */
#define CC_DXT_MUL_CONJ 8 /**< conjugate the second argument of cvMulSpectrums */
/** @} */

/** Discrete Fourier Transform:
    complex->complex,
    real->ccs (forward),
    ccs->real (inverse)
@see core_c_DftFlags "flags"
*/
CVAPI(void)  cvDFT( const img_t* src, img_t* dst, int flags,
                    int nonzero_rows CC_DEFAULT(0) );
#define cvFFT cvDFT

/** Multiply results of DFTs: DFT(X)*DFT(Y) or DFT(X)*conj(DFT(Y))
@see core_c_DftFlags "flags"
*/
CVAPI(void)  cvMulSpectrums( const img_t* src1, const img_t* src2,
                             img_t* dst, int flags );

/** Finds optimal DFT vector size >= size0 */
CVAPI(int)  cGetOptimalDFTSize( int size0 );

/** Discrete Cosine Transform
@see core_c_DftFlags "flags"
*/
CVAPI(void)  cvDCT( const img_t* src, img_t* dst, int flags );

/****************************************************************************************\
*                              Dynamic data structures                                   *
\****************************************************************************************/

/** Calculates length of sequence slice (with support of negative indices). */
CVAPI(int) cvSliceLength( CSlice slice, const CvSeq* seq );


/** Creates new memory storage.
   block_size == 0 means that default,
   somewhat optimal size, is used (currently, it is 64K) */
CVAPI(CvMemStorage*)  cvCreateMemStorage( int block_size CC_DEFAULT(0));


/** Creates a memory storage that will borrow memory blocks from parent storage */
CVAPI(CvMemStorage*)  cvCreateChildMemStorage( CvMemStorage* parent );


/** Releases memory storage. All the children of a parent must be released before
   the parent. A child storage returns all the blocks to parent when it is released */
CVAPI(void)  cvReleaseMemStorage( CvMemStorage** storage );


/** Clears memory storage. This is the only way(!!!) (besides cvRestoreMemStoragePos)
   to reuse memory allocated for the storage - cvClearSeq,cvClearSet ...
   do not free any memory.
   A child storage returns all the blocks to the parent when it is cleared */
CVAPI(void)  cvClearMemStorage( CvMemStorage* storage );

/** Remember a storage "free memory" position */
CVAPI(void)  cvSaveMemStoragePos( const CvMemStorage* storage, CvMemStoragePos* pos );

/** Restore a storage "free memory" position */
CVAPI(void)  cvRestoreMemStoragePos( CvMemStorage* storage, CvMemStoragePos* pos );

/** Allocates continuous buffer of the specified size in the storage */
CVAPI(void*) cvMemStorageAlloc( CvMemStorage* storage, size_t size );

/** Allocates string in memory storage */
CVAPI(CvString) cvMemStorageAllocString( CvMemStorage* storage, const char* ptr,
                                         int len CC_DEFAULT(-1) );

/** Creates new empty sequence that will reside in the specified storage */
CVAPI(CvSeq*)  cvCreateSeq( int seq_flags, size_t header_size,
                            size_t elem_size, CvMemStorage* storage );

/** Changes default size (granularity) of sequence blocks.
   The default size is ~1Kbyte */
CVAPI(void)  cvSetSeqBlockSize( CvSeq* seq, int delta_elems );


/** Adds new element to the end of sequence. Returns pointer to the element */
CVAPI(schar*)  cvSeqPush( CvSeq* seq, const void* element CC_DEFAULT(NULL));


/** Adds new element to the beginning of sequence. Returns pointer to it */
CVAPI(schar*)  cvSeqPushFront( CvSeq* seq, const void* element CC_DEFAULT(NULL));


/** Removes the last element from sequence and optionally saves it */
CVAPI(void)  cvSeqPop( CvSeq* seq, void* element CC_DEFAULT(NULL));


/** Removes the first element from sequence and optioanally saves it */
CVAPI(void)  cvSeqPopFront( CvSeq* seq, void* element CC_DEFAULT(NULL));


#define CC_FRONT 1
#define CC_BACK 0
/** Adds several new elements to the end of sequence */
CVAPI(void)  cvSeqPushMulti( CvSeq* seq, const void* elements,
                             int count, int in_front CC_DEFAULT(0) );

/** Removes several elements from the end of sequence and optionally saves them */
CVAPI(void)  cvSeqPopMulti( CvSeq* seq, void* elements,
                            int count, int in_front CC_DEFAULT(0) );

/** Inserts a new element in the middle of sequence.
   cvSeqInsert(seq,0,elem) == cvSeqPushFront(seq,elem) */
CVAPI(schar*)  cvSeqInsert( CvSeq* seq, int before_index,
                            const void* element CC_DEFAULT(NULL));

/** Removes specified sequence element */
CVAPI(void)  cvSeqRemove( CvSeq* seq, int index );


/** Removes all the elements from the sequence. The freed memory
   can be reused later only by the same sequence unless cvClearMemStorage
   or cvRestoreMemStoragePos is called */
CVAPI(void)  cvClearSeq( CvSeq* seq );


/** Retrieves pointer to specified sequence element.
   Negative indices are supported and mean counting from the end
   (e.g -1 means the last sequence element) */
CVAPI(schar*)  cvGetSeqElem( const CvSeq* seq, int index );

/** Calculates index of the specified sequence element.
   Returns -1 if element does not belong to the sequence */
CVAPI(int)  cvSeqElemIdx( const CvSeq* seq, const void* element,
                         CvSeqBlock** block CC_DEFAULT(NULL) );

/** Initializes sequence writer. The new elements will be added to the end of sequence */
CVAPI(void)  cvStartAppendToSeq( CvSeq* seq, CvSeqWriter* writer );


/** Combination of cvCreateSeq and cvStartAppendToSeq */
CVAPI(void)  cvStartWriteSeq( int seq_flags, int header_size,
                              int elem_size, CvMemStorage* storage,
                              CvSeqWriter* writer );

/** Closes sequence writer, updates sequence header and returns pointer
   to the resultant sequence
   (which may be useful if the sequence was created using cvStartWriteSeq))
*/
CVAPI(CvSeq*)  cvEndWriteSeq( CvSeqWriter* writer );


/** Updates sequence header. May be useful to get access to some of previously
   written elements via cvGetSeqElem or sequence reader */
CVAPI(void)   cvFlushSeqWriter( CvSeqWriter* writer );


/** Initializes sequence reader.
   The sequence can be read in forward or backward direction */
CVAPI(void) cvStartReadSeq( const CvSeq* seq, CvSeqReader* reader,
                           int reverse CC_DEFAULT(0) );


/** Returns current sequence reader position (currently observed sequence element) */
CVAPI(int)  cvGetSeqReaderPos( CvSeqReader* reader );


/** Changes sequence reader position. It may seek to an absolute or
   to relative to the current position */
CVAPI(void)   cvSetSeqReaderPos( CvSeqReader* reader, int index,
                                 int is_relative CC_DEFAULT(0));

/** Copies sequence content to a continuous piece of memory */
CVAPI(void*)  cvCvtSeqToArray( const CvSeq* seq, void* elements,
                               CSlice slice CC_DEFAULT(CC_WHOLE_SEQ) );

/** Creates sequence header for array.
   After that all the operations on sequences that do not alter the content
   can be applied to the resultant sequence */
CVAPI(CvSeq*) cvMakeSeqHeaderForArray( int seq_type, int header_size,
                                       int elem_size, void* elements, int total,
                                       CvSeq* seq, CvSeqBlock* block );

/** Extracts sequence slice (with or without copying sequence elements) */
CVAPI(CvSeq*) cvSeqSlice( const CvSeq* seq, CSlice slice,
                         CvMemStorage* storage CC_DEFAULT(NULL),
                         int copy_data CC_DEFAULT(0));

CC_INLINE CvSeq* cvCloneSeq( const CvSeq* seq, CvMemStorage* storage CC_DEFAULT(NULL))
{
    return cvSeqSlice( seq, CC_WHOLE_SEQ, storage, 1 );
}

/** Removes sequence slice */
CVAPI(void)  cvSeqRemoveSlice( CvSeq* seq, CSlice slice );

/** Inserts a sequence or array into another sequence */
CVAPI(void)  cvSeqInsertSlice( CvSeq* seq, int before_index, const img_t* from_arr );

/** a < b ? -1 : a > b ? 1 : 0 */
typedef int (C_CDECL* CvCmpFunc)(const void* a, const void* b, void* userdata );

/** Sorts sequence in-place given element comparison function */
CVAPI(void) cvSeqSort( CvSeq* seq, CvCmpFunc func, void* userdata CC_DEFAULT(NULL) );

/** Finds element in a [sorted] sequence */
CVAPI(schar*) cvSeqSearch( CvSeq* seq, const void* elem, CvCmpFunc func,
                           int is_sorted, int* elem_idx,
                           void* userdata CC_DEFAULT(NULL) );

/** Reverses order of sequence elements in-place */
CVAPI(void) cvSeqInvert( CvSeq* seq );

/** Splits sequence into one or more equivalence classes using the specified criteria */
CVAPI(int)  cvSeqPartition( const CvSeq* seq, CvMemStorage* storage,
                            CvSeq** labels, CvCmpFunc is_equal, void* userdata );

/************ Internal sequence functions ************/
CVAPI(void)  cvChangeSeqBlock( void* reader, int direction );
CVAPI(void)  cvCreateSeqBlock( CvSeqWriter* writer );


/** Creates a new set */
CVAPI(CvSet*)  cvCreateSet( int set_flags, int header_size,
                            int elem_size, CvMemStorage* storage );

/** Adds new element to the set and returns pointer to it */
CVAPI(int)  cvSetAdd( CvSet* set_header, CvSetElem* elem CC_DEFAULT(NULL),
                      CvSetElem** inserted_elem CC_DEFAULT(NULL) );

/** Fast variant of cvSetAdd */
CC_INLINE  CvSetElem* cvSetNew( CvSet* set_header )
{
    CvSetElem* elem = set_header->free_elems;
    if( elem )
    {
        set_header->free_elems = elem->next_free;
        elem->flags = elem->flags & CC_SET_ELEM_IDX_MASK;
        set_header->active_count++;
    }
    else
        cvSetAdd( set_header, NULL, &elem );
    return elem;
}

/** Removes set element given its pointer */
CC_INLINE  void cvSetRemoveByPtr( CvSet* set_header, void* elem )
{
    CvSetElem* _elem = (CvSetElem*)elem;
    assert( _elem->flags >= 0 /*&& (elem->flags & CC_SET_ELEM_IDX_MASK) < set_header->total*/ );
    _elem->next_free = set_header->free_elems;
    _elem->flags = (_elem->flags & CC_SET_ELEM_IDX_MASK) | CC_SET_ELEM_FREE_FLAG;
    set_header->free_elems = _elem;
    set_header->active_count--;
}

/** Removes element from the set by its index  */
CVAPI(void)   cvSetRemove( CvSet* set_header, int index );

/** Returns a set element by index. If the element doesn't belong to the set,
   NULL is returned */
CC_INLINE CvSetElem* cvGetSetElem( const CvSet* set_header, int idx )
{
    CvSetElem* elem = (CvSetElem*)(void *)cvGetSeqElem( (CvSeq*)set_header, idx );
    return elem && CC_IS_SET_ELEM( elem ) ? elem : 0;
}

/** Removes all the elements from the set */
CVAPI(void)  cvClearSet( CvSet* set_header );

/** Creates new graph */
CVAPI(CvGraph*)  cvCreateGraph( int graph_flags, int header_size,
                                int vtx_size, int edge_size,
                                CvMemStorage* storage );

/** Adds new vertex to the graph */
CVAPI(int)  cvGraphAddVtx( CvGraph* graph, const CvGraphVtx* vtx CC_DEFAULT(NULL),
                           CvGraphVtx** inserted_vtx CC_DEFAULT(NULL) );


/** Removes vertex from the graph together with all incident edges */
CVAPI(int)  cvGraphRemoveVtx( CvGraph* graph, int index );
CVAPI(int)  cvGraphRemoveVtxByPtr( CvGraph* graph, CvGraphVtx* vtx );


/** Link two vertices specifed by indices or pointers if they
   are not connected or return pointer to already existing edge
   connecting the vertices.
   Functions return 1 if a new edge was created, 0 otherwise */
CVAPI(int)  cvGraphAddEdge( CvGraph* graph,
                            int start_idx, int end_idx,
                            const CvGraphEdge* edge CC_DEFAULT(NULL),
                            CvGraphEdge** inserted_edge CC_DEFAULT(NULL) );

CVAPI(int)  cvGraphAddEdgeByPtr( CvGraph* graph,
                               CvGraphVtx* start_vtx, CvGraphVtx* end_vtx,
                               const CvGraphEdge* edge CC_DEFAULT(NULL),
                               CvGraphEdge** inserted_edge CC_DEFAULT(NULL) );

/** Remove edge connecting two vertices */
CVAPI(void)  cvGraphRemoveEdge( CvGraph* graph, int start_idx, int end_idx );
CVAPI(void)  cvGraphRemoveEdgeByPtr( CvGraph* graph, CvGraphVtx* start_vtx,
                                     CvGraphVtx* end_vtx );

/** Find edge connecting two vertices */
CVAPI(CvGraphEdge*)  cvFindGraphEdge( const CvGraph* graph, int start_idx, int end_idx );
CVAPI(CvGraphEdge*)  cvFindGraphEdgeByPtr( const CvGraph* graph,
                                           const CvGraphVtx* start_vtx,
                                           const CvGraphVtx* end_vtx );
#define cvGraphFindEdge cvFindGraphEdge
#define cvGraphFindEdgeByPtr cvFindGraphEdgeByPtr

/** Remove all vertices and edges from the graph */
CVAPI(void)  cvClearGraph( CvGraph* graph );


/** Count number of edges incident to the vertex */
CVAPI(int)  cvGraphVtxDegree( const CvGraph* graph, int vtx_idx );
CVAPI(int)  cvGraphVtxDegreeByPtr( const CvGraph* graph, const CvGraphVtx* vtx );


/** Retrieves graph vertex by given index */
#define cvGetGraphVtx( graph, idx ) (CvGraphVtx*)cvGetSetElem((CvSet*)(graph), (idx))

/** Retrieves index of a graph vertex given its pointer */
#define cvGraphVtxIdx( graph, vtx ) ((vtx)->flags & CC_SET_ELEM_IDX_MASK)

/** Retrieves index of a graph edge given its pointer */
#define cvGraphEdgeIdx( graph, edge ) ((edge)->flags & CC_SET_ELEM_IDX_MASK)

#define cvGraphGetVtxCount( graph ) ((graph)->active_count)
#define cvGraphGetEdgeCount( graph ) ((graph)->edges->active_count)

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

/** flags for graph vertices and edges */
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

/** Creates new graph scanner. */
CVAPI(CvGraphScanner*)  cvCreateGraphScanner( CvGraph* graph,
                                             CvGraphVtx* vtx CC_DEFAULT(NULL),
                                             int mask CC_DEFAULT(CC_GRAPH_ALL_ITEMS));

/** Releases graph scanner. */
CVAPI(void) cvReleaseGraphScanner( CvGraphScanner** scanner );

/** Get next graph element */
CVAPI(int)  cvNextGraphItem( CvGraphScanner* scanner );

/** Creates a copy of graph */
CVAPI(CvGraph*) cvCloneGraph( const CvGraph* graph, CvMemStorage* storage );


/** Does look-up transformation. Elements of the source array
   (that should be 8uC1 or 8sC1) are used as indexes in lutarr 256-element table */
CVAPI(void) cvLUT( const img_t* src, img_t* dst, const img_t* lut );


/******************* Iteration through the sequence tree *****************/
typedef struct CvTreeNodeIterator
{
    const void* node;
    int level;
    int max_level;
}
CvTreeNodeIterator;

CVAPI(void) cvInitTreeNodeIterator( CvTreeNodeIterator* tree_iterator,
                                   const void* first, int max_level );
CVAPI(void*) cvNextTreeNode( CvTreeNodeIterator* tree_iterator );
CVAPI(void*) cvPrevTreeNode( CvTreeNodeIterator* tree_iterator );

/** Inserts sequence into tree with specified "parent" sequence.
   If parent is equal to frame (e.g. the most external contour),
   then added contour will have null pointer to parent. */
CVAPI(void) cvInsertNodeIntoTree( void* node, void* parent, void* frame );

/** Removes contour from tree (together with the contour children). */
CVAPI(void) cvRemoveNodeFromTree( void* node, void* frame );

/** Gathers pointers to all the sequences,
   accessible from the `first`, to the single sequence */
CVAPI(CvSeq*) cvTreeToNodeSeq( const void* first, int header_size,
                              CvMemStorage* storage );

/** The function implements the K-means algorithm for clustering an array of sample
   vectors in a specified number of classes */
#define CC_KMEANS_USE_INITIAL_LABELS    1
CVAPI(int) cvKMeans2( const img_t* samples, int cluster_count, img_t* labels,
                      CTermCriteria termcrit, int attempts CC_DEFAULT(1),
                      CRNG* rng CC_DEFAULT(0), int flags CC_DEFAULT(0),
                      img_t* _centers CC_DEFAULT(0), double* compactness CC_DEFAULT(0) );

/****************************************************************************************\
*                                    System functions                                    *
\****************************************************************************************/

/** Loads optimized functions from IPP, MKL etc. or switches back to pure C code */
CVAPI(int)  cvUseOptimized( int on_off );

typedef img_t* (C_STDCALL* Cv_iplCreateImageHeader)
                            (int,int,int,char*,char*,int,int,int,int,int,
                            IplROI*,img_t*,void*,IplTileInfo*);
typedef void (C_STDCALL* Cv_iplAllocateImageData)(img_t*,int,int);
typedef void (C_STDCALL* Cv_iplDeallocate)(img_t*,int);
typedef IplROI* (C_STDCALL* Cv_iplCreateROI)(int,int,int,int,int);
typedef img_t* (C_STDCALL* Cv_iplCloneImage)(const img_t*);

/** @brief Makes OpenCV use IPL functions for allocating img_t and IplROI structures.

Normally, the function is not called directly. Instead, a simple macro
CC_TURN_ON_IMG_COMPATIBILITY() is used that calls cvSetIPLAllocators and passes there pointers
to IPL allocation functions. :
@code
    ...
    CC_TURN_ON_IMG_COMPATIBILITY()
    ...
@endcode
@param create_header pointer to a function, creating IPL image header.
@param allocate_data pointer to a function, allocating IPL image data.
@param deallocate pointer to a function, deallocating IPL image.
@param create_roi pointer to a function, creating IPL image ROI (i.e. Region of Interest).
@param clone_image pointer to a function, cloning an IPL image.
 */
CVAPI(void) cvSetIPLAllocators( Cv_iplCreateImageHeader create_header,
                               Cv_iplAllocateImageData allocate_data,
                               Cv_iplDeallocate deallocate,
                               Cv_iplCreateROI create_roi,
                               Cv_iplCloneImage clone_image );

#define CC_TURN_ON_IMG_COMPATIBILITY()                                  \
    cvSetIPLAllocators( iplCreateImageHeader, iplAllocateImage,         \
                        iplDeallocate, iplCreateROI, iplCloneImage )

/****************************************************************************************\
*                                    Data Persistence                                    *
\****************************************************************************************/

/********************************** High-level functions ********************************/

/** @brief Opens file storage for reading or writing data.

The function opens file storage for reading or writing data. In the latter case, a new file is
created or an existing file is rewritten. The type of the read or written file is determined by the
filename extension: .xml for XML, .yml or .yaml for YAML and .json for JSON.

At the same time, it also supports adding parameters like "example.xml?base64". The three ways
are the same:
@snippet samples/cpp/filestorage_base64.cpp suffix_in_file_name
@snippet samples/cpp/filestorage_base64.cpp flag_write_base64
@snippet samples/cpp/filestorage_base64.cpp flag_write_and_flag_base64

The function returns a pointer to the CvFileStorage structure.
If the file cannot be opened then the function returns NULL.
@param filename Name of the file associated with the storage
@param memstorage Memory storage used for temporary data and for
:   storing dynamic structures, such as CvSeq or CvGraph . If it is NULL, a temporary memory
    storage is created and used.
@param flags Can be one of the following:
> -   **CC_STORAGE_READ** the storage is open for reading
> -   **CC_STORAGE_WRITE** the storage is open for writing
      (use **CC_STORAGE_WRITE | CC_STORAGE_WRITE_BASE64** to write rawdata in Base64)
@param encoding
 */
CVAPI(CvFileStorage*)  cvOpenFileStorage( const char* filename, CvMemStorage* memstorage,
                                          int flags, const char* encoding CC_DEFAULT(NULL) );

/** @brief Releases file storage.

The function closes the file associated with the storage and releases all the temporary structures.
It must be called after all I/O operations with the storage are finished.
@param fs Double pointer to the released file storage
 */
CVAPI(void) cvReleaseFileStorage( CvFileStorage** fs );

/** returns attribute value or 0 (NULL) if there is no such attribute */
CVAPI(const char*) cvAttrValue( const CvAttrList* attr, const char* attr_name );

/** @brief Starts writing a new structure.

The function starts writing a compound structure (collection) that can be a sequence or a map. After
all the structure fields, which can be scalars or structures, are written, cvWriteStructEnd should
be called. The function can be used to group some objects or to implement the write function for a
some user object (see CvTypeInfo).
@param fs File storage
@param name Name of the written structure. The structure can be accessed by this name when the
storage is read.
@param struct_flags A combination one of the following values:
-   **CC_NODE_SEQ** the written structure is a sequence (see discussion of CvFileStorage ),
    that is, its elements do not have a name.
-   **CC_NODE_MAP** the written structure is a map (see discussion of CvFileStorage ), that
    is, all its elements have names.
One and only one of the two above flags must be specified
-   **CC_NODE_FLOW** the optional flag that makes sense only for YAML streams. It means that
     the structure is written as a flow (not as a block), which is more compact. It is
     recommended to use this flag for structures or arrays whose elements are all scalars.
@param type_name Optional parameter - the object type name. In
    case of XML it is written as a type_id attribute of the structure opening tag. In the case of
    YAML it is written after a colon following the structure name (see the example in
    CvFileStorage description). In case of JSON it is written as a name/value pair.
    Mainly it is used with user objects. When the storage is read, the
    encoded type name is used to determine the object type (see CvTypeInfo and cvFindType ).
@param attributes This parameter is not used in the current implementation
 */
CVAPI(void) cvWriteStructBegin( CvFileStorage* fs, const char* name,
                                int struct_flags, const char* type_name CC_DEFAULT(NULL),
                                CvAttrList attributes CC_DEFAULT(cvAttrList()));

/** @brief Finishes writing to a file node collection.
@param fs File storage
@sa cvWriteStructBegin.
 */
CVAPI(void) cvWriteStructEnd( CvFileStorage* fs );

/** @brief Writes an integer value.

The function writes a single integer value (with or without a name) to the file storage.
@param fs File storage
@param name Name of the written value. Should be NULL if and only if the parent structure is a
sequence.
@param value The written value
 */
CVAPI(void) cvWriteInt( CvFileStorage* fs, const char* name, int value );

/** @brief Writes a floating-point value.

The function writes a single floating-point value (with or without a name) to file storage. Special
values are encoded as follows: NaN (Not A Number) as .NaN, infinity as +.Inf or -.Inf.

The following example shows how to use the low-level writing functions to store custom structures,
such as termination criteria, without registering a new type. :
@code
    void write_termcriteria( CvFileStorage* fs, const char* struct_name,
                             CTermCriteria* termcrit )
    {
        cvWriteStructBegin( fs, struct_name, CC_NODE_MAP, NULL, cvAttrList(0,0));
        cvWriteComment( fs, "termination criteria", 1 ); // just a description
        if( termcrit->tid & CC_TERMCRIT_ITER )
            cvWriteInteger( fs, "max_iterations", termcrit->max_iter );
        if( termcrit->tid & CC_TERMCRIT_EPS )
            cvWriteReal( fs, "accuracy", termcrit->epsilon );
        cvWriteStructEnd( fs );
    }
@endcode
@param fs File storage
@param name Name of the written value. Should be NULL if and only if the parent structure is a
sequence.
@param value The written value
*/
CVAPI(void) cvWriteReal( CvFileStorage* fs, const char* name, double value );

/** @brief Writes a text string.

The function writes a text string to file storage.
@param fs File storage
@param name Name of the written string . Should be NULL if and only if the parent structure is a
sequence.
@param str The written text string
@param quote If non-zero, the written string is put in quotes, regardless of whether they are
required. Otherwise, if the flag is zero, quotes are used only when they are required (e.g. when
the string starts with a digit or contains spaces).
 */
CVAPI(void) cvWriteString( CvFileStorage* fs, const char* name,
                           const char* str, int quote CC_DEFAULT(0) );

/** @brief Writes a comment.

The function writes a comment into file storage. The comments are skipped when the storage is read.
@param fs File storage
@param comment The written comment, single-line or multi-line
@param eol_comment If non-zero, the function tries to put the comment at the end of current line.
If the flag is zero, if the comment is multi-line, or if it does not fit at the end of the current
line, the comment starts a new line.
 */
CVAPI(void) cvWriteComment( CvFileStorage* fs, const char* comment,
                            int eol_comment );

/** @brief Writes an object to file storage.

The function writes an object to file storage. First, the appropriate type info is found using
cvTypeOf. Then, the write method associated with the type info is called.

Attributes are used to customize the writing procedure. The standard types support the following
attributes (all the dt attributes have the same format as in cvWriteRawData):

-# CvSeq
    -   **header_dt** description of user fields of the sequence header that follow CvSeq, or
        CChain (if the sequence is a Freeman chain) or CvContour (if the sequence is a contour or
        point sequence)
    -   **dt** description of the sequence elements.
    -   **recursive** if the attribute is present and is not equal to "0" or "false", the whole
        tree of sequences (contours) is stored.
-# CvGraph
    -   **header_dt** description of user fields of the graph header that follows CvGraph;
    -   **vertex_dt** description of user fields of graph vertices
    -   **edge_dt** description of user fields of graph edges (note that the edge weight is
        always written, so there is no need to specify it explicitly)

Below is the code that creates the YAML file shown in the CvFileStorage description:
@code
    #include "cxcore.h"

    int main( int argc, char** argv )
    {
        img_t* mat = cvCreateMat( 3, 3, CC_32F );
        CvFileStorage* fs = cvOpenFileStorage( "example.yml", 0, CC_STORAGE_WRITE );

        cvSetIdentity( mat );
        cvWrite( fs, "A", mat, cvAttrList(0,0) );

        cvReleaseFileStorage( &fs );
        cvReleaseMat( &mat );
        return 0;
    }
@endcode
@param fs File storage
@param name Name of the written object. Should be NULL if and only if the parent structure is a
sequence.
@param ptr Pointer to the object
@param attributes The attributes of the object. They are specific for each particular type (see
the discussion below).
 */
CVAPI(void) cvWrite( CvFileStorage* fs, const char* name, const void* ptr,
                         CvAttrList attributes CC_DEFAULT(cvAttrList()));

/** @brief Starts the next stream.

The function finishes the currently written stream and starts the next stream. In the case of XML
the file with multiple streams looks like this:
@code{.xml}
    <opencv_storage>
    <!-- stream #1 data -->
    </opencv_storage>
    <opencv_storage>
    <!-- stream #2 data -->
    </opencv_storage>
    ...
@endcode
The YAML file will look like this:
@code{.yaml}
    %YAML 1.0
    # stream #1 data
    ...
    ---
    # stream #2 data
@endcode
This is useful for concatenating files or for resuming the writing process.
@param fs File storage
 */
CVAPI(void) cvStartNextStream( CvFileStorage* fs );

/** @brief Writes multiple numbers.

The function writes an array, whose elements consist of single or multiple numbers. The function
call can be replaced with a loop containing a few cvWriteInt and cvWriteReal calls, but a single
call is more efficient. Note that because none of the elements have a name, they should be written
to a sequence rather than a map.
@param fs File storage
@param src Pointer to the written array
@param len Number of the array elements to write
@param dt Specification of each array element, see @ref format_spec "format specification"
 */
CVAPI(void) cvWriteRawData( CvFileStorage* fs, const void* src,
                                int len, const char* dt );

/** @brief Writes multiple numbers in Base64.

If either CC_STORAGE_WRITE_BASE64 or FileStorage::WRITE_BASE64 is used,
this function will be the same as cvWriteRawData. If neither, the main
difference is that it outputs a sequence in Base64 encoding rather than
in plain text.

This function can only be used to write a sequence with a type "binary".

Consider the following two examples where their output is the same:
@snippet samples/cpp/filestorage_base64.cpp without_base64_flag
and
@snippet samples/cpp/filestorage_base64.cpp with_write_base64_flag

@param fs File storage
@param src Pointer to the written array
@param len Number of the array elements to write
@param dt Specification of each array element, see @ref format_spec "format specification"
*/
CVAPI(void) cvWriteRawDataBase64( CvFileStorage* fs, const void* src,
                                 int len, const char* dt );

/** @brief Returns a unique pointer for a given name.

The function returns a unique pointer for each particular file node name. This pointer can be then
passed to the cvGetFileNode function that is faster than cvGetFileNodeByName because it compares
text strings by comparing pointers rather than the strings' content.

Consider the following example where an array of points is encoded as a sequence of 2-entry maps:
@code
    points:
      - { x: 10, y: 10 }
      - { x: 20, y: 20 }
      - { x: 30, y: 30 }
      # ...
@endcode
Then, it is possible to get hashed "x" and "y" pointers to speed up decoding of the points. :
@code
    #include "cxcore.h"

    int main( int argc, char** argv )
    {
        CvFileStorage* fs = cvOpenFileStorage( "points.yml", 0, CC_STORAGE_READ );
        CvStringHashNode* x_key = cvGetHashedNode( fs, "x", -1, 1 );
        CvStringHashNode* y_key = cvGetHashedNode( fs, "y", -1, 1 );
        CvFileNode* points = cvGetFileNodeByName( fs, 0, "points" );

        if( CC_NODE_IS_SEQ(points->tag) )
        {
            CvSeq* seq = points->data.seq;
            int i, total = seq->total;
            CvSeqReader reader;
            cvStartReadSeq( seq, &reader, 0 );
            for( i = 0; i < total; i++ )
            {
                CvFileNode* pt = (CvFileNode*)reader.ptr;
    #if 1 // faster variant
                CvFileNode* xnode = cvGetFileNode( fs, pt, x_key, 0 );
                CvFileNode* ynode = cvGetFileNode( fs, pt, y_key, 0 );
                assert( xnode && CC_NODE_IS_INT(xnode->tag) &&
                        ynode && CC_NODE_IS_INT(ynode->tag));
                int x = xnode->data.i; // or x = cvReadInt( xnode, 0 );
                int y = ynode->data.i; // or y = cvReadInt( ynode, 0 );
    #elif 1 // slower variant; does not use x_key & y_key
                CvFileNode* xnode = cvGetFileNodeByName( fs, pt, "x" );
                CvFileNode* ynode = cvGetFileNodeByName( fs, pt, "y" );
                assert( xnode && CC_NODE_IS_INT(xnode->tag) &&
                        ynode && CC_NODE_IS_INT(ynode->tag));
                int x = xnode->data.i; // or x = cvReadInt( xnode, 0 );
                int y = ynode->data.i; // or y = cvReadInt( ynode, 0 );
    #else // the slowest yet the easiest to use variant
                int x = cvReadIntByName( fs, pt, "x", 0 );
                int y = cvReadIntByName( fs, pt, "y", 0 );
    #endif
                CC_NEXT_SEQ_ELEM( seq->elem_size, reader );
                printf("
            }
        }
        cvReleaseFileStorage( &fs );
        return 0;
    }
@endcode
Please note that whatever method of accessing a map you are using, it is still much slower than
using plain sequences; for example, in the above example, it is more efficient to encode the points
as pairs of integers in a single numeric sequence.
@param fs File storage
@param name Literal node name
@param len Length of the name (if it is known apriori), or -1 if it needs to be calculated
@param create_missing Flag that specifies, whether an absent key should be added into the hash table
*/
CVAPI(CvStringHashNode*) cvGetHashedKey( CvFileStorage* fs, const char* name,
                                        int len CC_DEFAULT(-1),
                                        int create_missing CC_DEFAULT(0));

/** @brief Retrieves one of the top-level nodes of the file storage.

The function returns one of the top-level file nodes. The top-level nodes do not have a name, they
correspond to the streams that are stored one after another in the file storage. If the index is out
of range, the function returns a NULL pointer, so all the top-level nodes can be iterated by
subsequent calls to the function with stream_index=0,1,..., until the NULL pointer is returned.
This function can be used as a base for recursive traversal of the file storage.
@param fs File storage
@param stream_index Zero-based index of the stream. See cvStartNextStream . In most cases,
there is only one stream in the file; however, there can be several.
 */
CVAPI(CvFileNode*) cvGetRootFileNode( const CvFileStorage* fs,
                                     int stream_index CC_DEFAULT(0) );

/** @brief Finds a node in a map or file storage.

The function finds a file node. It is a faster version of cvGetFileNodeByName (see
cvGetHashedKey discussion). Also, the function can insert a new node, if it is not in the map yet.
@param fs File storage
@param map The parent map. If it is NULL, the function searches a top-level node. If both map and
key are NULLs, the function returns the root file node - a map that contains top-level nodes.
@param key Unique pointer to the node name, retrieved with cvGetHashedKey
@param create_missing Flag that specifies whether an absent node should be added to the map
 */
CVAPI(CvFileNode*) cvGetFileNode( CvFileStorage* fs, CvFileNode* map,
                                 const CvStringHashNode* key,
                                 int create_missing CC_DEFAULT(0) );

/** @brief Finds a node in a map or file storage.

The function finds a file node by name. The node is searched either in map or, if the pointer is
NULL, among the top-level file storage nodes. Using this function for maps and cvGetSeqElem (or
sequence reader) for sequences, it is possible to navigate through the file storage. To speed up
multiple queries for a certain key (e.g., in the case of an array of structures) one may use a
combination of cvGetHashedKey and cvGetFileNode.
@param fs File storage
@param map The parent map. If it is NULL, the function searches in all the top-level nodes
(streams), starting with the first one.
@param name The file node name
 */
CVAPI(CvFileNode*) cvGetFileNodeByName( const CvFileStorage* fs,
                                       const CvFileNode* map,
                                       const char* name );

/** @brief Retrieves an integer value from a file node.

The function returns an integer that is represented by the file node. If the file node is NULL, the
default_value is returned (thus, it is convenient to call the function right after cvGetFileNode
without checking for a NULL pointer). If the file node has type CC_NODE_INT, then node-\>data.i is
returned. If the file node has type CC_NODE_REAL, then node-\>data.f is converted to an integer
and returned. Otherwise the error is reported.
@param node File node
@param default_value The value that is returned if node is NULL
 */
CC_INLINE int cvReadInt( const CvFileNode* node, int default_value CC_DEFAULT(0) )
{
    return !node ? default_value :
        CC_NODE_IS_INT(node->tag) ? node->data.i :
        CC_NODE_IS_REAL(node->tag) ? cRound(node->data.f) : 0x7fffffff;
}

/** @brief Finds a file node and returns its value.

The function is a simple superposition of cvGetFileNodeByName and cvReadInt.
@param fs File storage
@param map The parent map. If it is NULL, the function searches a top-level node.
@param name The node name
@param default_value The value that is returned if the file node is not found
 */
CC_INLINE int cvReadIntByName( const CvFileStorage* fs, const CvFileNode* map,
                         const char* name, int default_value CC_DEFAULT(0) )
{
    return cvReadInt( cvGetFileNodeByName( fs, map, name ), default_value );
}

/** @brief Retrieves a floating-point value from a file node.

The function returns a floating-point value that is represented by the file node. If the file node
is NULL, the default_value is returned (thus, it is convenient to call the function right after
cvGetFileNode without checking for a NULL pointer). If the file node has type CC_NODE_REAL ,
then node-\>data.f is returned. If the file node has type CC_NODE_INT , then node-:math:\>data.f
is converted to floating-point and returned. Otherwise the result is not determined.
@param node File node
@param default_value The value that is returned if node is NULL
 */
CC_INLINE double cvReadReal( const CvFileNode* node, double default_value CC_DEFAULT(0.) )
{
    return !node ? default_value :
        CC_NODE_IS_INT(node->tag) ? (double)node->data.i :
        CC_NODE_IS_REAL(node->tag) ? node->data.f : 1e300;
}

/** @brief Finds a file node and returns its value.

The function is a simple superposition of cvGetFileNodeByName and cvReadReal .
@param fs File storage
@param map The parent map. If it is NULL, the function searches a top-level node.
@param name The node name
@param default_value The value that is returned if the file node is not found
 */
CC_INLINE double cvReadRealByName( const CvFileStorage* fs, const CvFileNode* map,
                        const char* name, double default_value CC_DEFAULT(0.) )
{
    return cvReadReal( cvGetFileNodeByName( fs, map, name ), default_value );
}

/** @brief Retrieves a text string from a file node.

The function returns a text string that is represented by the file node. If the file node is NULL,
the default_value is returned (thus, it is convenient to call the function right after
cvGetFileNode without checking for a NULL pointer). If the file node has type CC_NODE_STR , then
node-:math:\>data.str.ptr is returned. Otherwise the result is not determined.
@param node File node
@param default_value The value that is returned if node is NULL
 */
CC_INLINE const char* cvReadString( const CvFileNode* node,
                        const char* default_value CC_DEFAULT(NULL) )
{
    return !node ? default_value : CC_NODE_IS_STRING(node->tag) ? node->data.str.ptr : 0;
}

/** @brief Finds a file node by its name and returns its value.

The function is a simple superposition of cvGetFileNodeByName and cvReadString .
@param fs File storage
@param map The parent map. If it is NULL, the function searches a top-level node.
@param name The node name
@param default_value The value that is returned if the file node is not found
 */
CC_INLINE const char* cvReadStringByName( const CvFileStorage* fs, const CvFileNode* map,
                        const char* name, const char* default_value CC_DEFAULT(NULL) )
{
    return cvReadString( cvGetFileNodeByName( fs, map, name ), default_value );
}


/** @brief Decodes an object and returns a pointer to it.

The function decodes a user object (creates an object in a native representation from the file
storage subtree) and returns it. The object to be decoded must be an instance of a registered type
that supports the read method (see CvTypeInfo). The type of the object is determined by the type
name that is encoded in the file. If the object is a dynamic structure, it is created either in
memory storage and passed to cvOpenFileStorage or, if a NULL pointer was passed, in temporary
memory storage, which is released when cvReleaseFileStorage is called. Otherwise, if the object is
not a dynamic structure, it is created in a heap and should be released with a specialized function
or by using the generic cvRelease.
@param fs File storage
@param node The root object node
@param attributes Unused parameter
 */
CVAPI(void*) cvRead( CvFileStorage* fs, CvFileNode* node,
                        CvAttrList* attributes CC_DEFAULT(NULL));

/** @brief Finds an object by name and decodes it.

The function is a simple superposition of cvGetFileNodeByName and cvRead.
@param fs File storage
@param map The parent map. If it is NULL, the function searches a top-level node.
@param name The node name
@param attributes Unused parameter
 */
CC_INLINE void* cvReadByName( CvFileStorage* fs, const CvFileNode* map,
                              const char* name, CvAttrList* attributes CC_DEFAULT(NULL) )
{
    return cvRead( fs, cvGetFileNodeByName( fs, map, name ), attributes );
}


/** @brief Initializes the file node sequence reader.

The function initializes the sequence reader to read data from a file node. The initialized reader
can be then passed to cvReadRawDataSlice.
@param fs File storage
@param src The file node (a sequence) to read numbers from
@param reader Pointer to the sequence reader
 */
CVAPI(void) cvStartReadRawData( const CvFileStorage* fs, const CvFileNode* src,
                               CvSeqReader* reader );

/** @brief Initializes file node sequence reader.

The function reads one or more elements from the file node, representing a sequence, to a
user-specified array. The total number of read sequence elements is a product of total and the
number of components in each array element. For example, if dt=2if, the function will read total\*3
sequence elements. As with any sequence, some parts of the file node sequence can be skipped or read
repeatedly by repositioning the reader using cvSetSeqReaderPos.
@param fs File storage
@param reader The sequence reader. Initialize it with cvStartReadRawData .
@param count The number of elements to read
@param dst Pointer to the destination array
@param dt Specification of each array element. It has the same format as in cvWriteRawData .
 */
CVAPI(void) cvReadRawDataSlice( const CvFileStorage* fs, CvSeqReader* reader,
                               int count, void* dst, const char* dt );

/** @brief Reads multiple numbers.

The function reads elements from a file node that represents a sequence of scalars.
@param fs File storage
@param src The file node (a sequence) to read numbers from
@param dst Pointer to the destination array
@param dt Specification of each array element. It has the same format as in cvWriteRawData .
 */
CVAPI(void) cvReadRawData( const CvFileStorage* fs, const CvFileNode* src,
                          void* dst, const char* dt );

/** @brief Writes a file node to another file storage.

The function writes a copy of a file node to file storage. Possible applications of the function are
merging several file storages into one and conversion between XML, YAML and JSON formats.
@param fs Destination file storage
@param new_node_name New name of the file node in the destination file storage. To keep the
existing name, use cvcvGetFileNodeName
@param node The written node
@param embed If the written node is a collection and this parameter is not zero, no extra level of
hierarchy is created. Instead, all the elements of node are written into the currently written
structure. Of course, map elements can only be embedded into another map, and sequence elements
can only be embedded into another sequence.
 */
CVAPI(void) cvWriteFileNode( CvFileStorage* fs, const char* new_node_name,
                            const CvFileNode* node, int embed );

/** @brief Returns the name of a file node.

The function returns the name of a file node or NULL, if the file node does not have a name or if
node is NULL.
@param node File node
 */
CVAPI(const char*) cvGetFileNodeName( const CvFileNode* node );

/*********************************** Adding own types ***********************************/

/** @brief Registers a new type.

The function registers a new type, which is described by info . The function creates a copy of the
structure, so the user should delete it after calling the function.
@param info Type info structure
 */
CVAPI(void) cvRegisterType( const CvTypeInfo* info );

/** @brief Unregisters the type.

The function unregisters a type with a specified name. If the name is unknown, it is possible to
locate the type info by an instance of the type using cvTypeOf or by iterating the type list,
starting from cvFirstType, and then calling cvUnregisterType(info-\>typeName).
@param type_name Name of an unregistered type
 */
CVAPI(void) cvUnregisterType( const char* type_name );

/** @brief Returns the beginning of a type list.

The function returns the first type in the list of registered types. Navigation through the list can
be done via the prev and next fields of the CvTypeInfo structure.
 */
CVAPI(CvTypeInfo*) cvFirstType(void);

/** @brief Finds a type by its name.

The function finds a registered type by its name. It returns NULL if there is no type with the
specified name.
@param type_name Type name
 */
CVAPI(CvTypeInfo*) cvFindType( const char* type_name );

/** @brief Returns the type of an object.

The function finds the type of a given object. It iterates through the list of registered types and
calls the is_instance function/method for every type info structure with that object until one of
them returns non-zero or until the whole list has been traversed. In the latter case, the function
returns NULL.
@param struct_ptr The object pointer
 */
CVAPI(CvTypeInfo*) cvTypeOf( const void* struct_ptr );

/** @brief Releases an object.

The function finds the type of a given object and calls release with the double pointer.
@param struct_ptr Double pointer to the object
 */
CVAPI(void) cvRelease( void** struct_ptr );

/** @brief Makes a clone of an object.

The function finds the type of a given object and calls clone with the passed object. Of course, if
you know the object type, for example, struct_ptr is img_t\*, it is faster to call the specific
function, like cvCloneMat.
@param struct_ptr The object to clone
 */
CVAPI(void*) cvClone( const void* struct_ptr );

/** @brief Saves an object to a file.

The function saves an object to a file. It provides a simple interface to cvWrite .
@param filename File name
@param struct_ptr Object to save
@param name Optional object name. If it is NULL, the name will be formed from filename .
@param comment Optional comment to put in the beginning of the file
@param attributes Optional attributes passed to cvWrite
 */
CVAPI(void) cvSave( const char* filename, const void* struct_ptr,
                    const char* name CC_DEFAULT(NULL),
                    const char* comment CC_DEFAULT(NULL),
                    CvAttrList attributes CC_DEFAULT(cvAttrList()));

/** @brief Loads an object from a file.

The function loads an object from a file. It basically reads the specified file, find the first
top-level node and calls cvRead for that node. If the file node does not have type information or
the type information can not be found by the type name, the function returns NULL. After the object
is loaded, the file storage is closed and all the temporary buffers are deleted. Thus, to load a
dynamic structure, such as a sequence, contour, or graph, one should pass a valid memory storage
destination to the function.
@param filename File name
@param memstorage Memory storage for dynamic structures, such as CvSeq or CvGraph . It is not used
for matrices or images.
@param name Optional object name. If it is NULL, the first top-level object in the storage will be
loaded.
@param real_name Optional output parameter that will contain the name of the loaded object
(useful if name=NULL )
 */
CVAPI(void*) cvLoad( const char* filename,
                     CvMemStorage* memstorage CC_DEFAULT(NULL),
                     const char* name CC_DEFAULT(NULL),
                     const char** real_name CC_DEFAULT(NULL) );

/*********************************** Measuring Execution Time ***************************/

/** helper functions for RNG initialization and accurate time measurement:
   uses internal clock counter on x86 */
CVAPI(int64)  cvGetTickCount( void );
CVAPI(double) cvGetTickFrequency( void );

/*********************************** CPU capabilities ***********************************/

CVAPI(int) cvCheckHardwareSupport(int feature);

/*********************************** Multi-Threading ************************************/

/** retrieve/set the number of threads used in OpenMP implementations */
CVAPI(int)  cGetNumThreads( void );
CVAPI(void) cvSetNumThreads( int threads CC_DEFAULT(0) );
/** get index of the thread being executed */
CVAPI(int)  cGetThreadNum( void );


/********************************** Error Handling **************************************/

/** Get current OpenCV error status */
CVAPI(int) cvGetErrStatus( void );

/** Sets error status silently */
CVAPI(void) cvSetErrStatus( int status );

#define CC_ErrModeLeaf     0   /* Print error and exit program */
#define CC_ErrModeParent   1   /* Print error and continue */
#define CC_ErrModeSilent   2   /* Don't print and continue */

/** Retrives current error processing mode */
CVAPI(int)  cvGetErrMode( void );

/** Sets error processing mode, returns previously used mode */
CVAPI(int) cvSetErrMode( int mode );

/** Sets error status and performs some additonal actions (displaying message box,
 writing message to stderr, terminating application etc.)
 depending on the current error mode */
CVAPI(void) cvError( int status, const char* func_name,
                    const char* err_msg, const char* file_name, int line );

/** Retrieves textual description of the error given its code */
CVAPI(const char*) cvErrorStr( int status );

/** Retrieves detailed information about the last error occured */
CVAPI(int) cvGetErrInfo( const char** errcode_desc, const char** description,
                        const char** filename, int* line );

/** Maps IPP error codes to the counterparts from OpenCV */
CVAPI(int) cvErrorFromIppStatus( int ipp_status );

typedef int (C_CDECL *CvErrorCallback)( int status, const char* func_name,
                                        const char* err_msg, const char* file_name, int line, void* userdata );

/** Assigns a new error-handling function */
CVAPI(CvErrorCallback) cvRedirectError( CvErrorCallback error_handler,
                                       void* userdata CC_DEFAULT(NULL),
                                       void** prev_userdata CC_DEFAULT(NULL) );

/** Output nothing */
CVAPI(int) cvNulDevReport( int status, const char* func_name, const char* err_msg,
                          const char* file_name, int line, void* userdata );

/** Output to console(fprintf(stderr,...)) */
CVAPI(int) cvStdErrReport( int status, const char* func_name, const char* err_msg,
                          const char* file_name, int line, void* userdata );

/** Output to MessageBox(WIN32) */
CVAPI(int) cvGuiBoxReport( int status, const char* func_name, const char* err_msg,
                          const char* file_name, int line, void* userdata );

#define OPENCC_ERROR(status,func,context)                           \
cvError((status),(func),(context),__FILE__,__LINE__)

#define OPENCC_ASSERT(expr,func,context)                            \
{if (! (expr))                                      \
{OPENCC_ERROR(CC_StsInternal,(func),(context));}}

#define OPENCC_CALL( Func )                                         \
{                                                                   \
Func;                                                           \
}


/** CC_FUNCNAME macro defines icvFuncName constant which is used by CC_ERROR macro */
#ifdef CC_NO_FUNC_NAMES
#define CC_FUNCNAME( Name )
#define cvFuncName ""
#else
#define CC_FUNCNAME( Name )  \
static char cvFuncName[] = Name
#endif


/**
 CC_ERROR macro unconditionally raises error with passed code and message.
 After raising error, control will be transferred to the exit label.
 */
#define CC_ERROR( Code, Msg )                                       \
{                                                                   \
    cvError( (Code), cvFuncName, Msg, __FILE__, __LINE__ );        \
    __CC_EXIT__;                                                   \
}

/**
 CC_CHECK macro checks error status after CV (or IPL)
 function call. If error detected, control will be transferred to the exit
 label.
 */
#define CC_CHECK()                                                  \
{                                                                   \
    if( cvGetErrStatus() < 0 )                                      \
        CC_ERROR( CC_StsBackTrace, "Inner function failed." );      \
}


/**
 CC_CALL macro calls CV (or IPL) function, checks error status and
 signals a error if the function failed. Useful in "parent node"
 error procesing mode
 */
#define CC_CALL( Func )                                             \
{                                                                   \
    Func;                                                           \
    CC_CHECK();                                                     \
}


/** Runtime assertion macro */
#define CC_ASSERT( Condition )                                          \
{                                                                       \
    if( !(Condition) )                                                  \
        CC_ERROR( CC_StsInternal, "Assertion: " #Condition " failed" ); \
}

#define __CC_BEGIN__       {
#define __CC_END__         goto exit; exit: ; }
#define __CC_EXIT__        goto exit

/** @} core_c */

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus

//! @addtogroup core_c_glue
//! @{

//! class for automatic module/RTTI data registration/unregistration
struct CC_EXPORTS CvType
{
    CvType( const char* type_name,
            CvIsInstanceFunc is_instance, CvReleaseFunc release=0,
            CvReadFunc read=0, CvWriteFunc write=0, CvCloneFunc clone=0 );
    ~CvType();
    CvTypeInfo* info;

    static CvTypeInfo* first;
    static CvTypeInfo* last;
};

//! @}

#include "opencv2/core/utility.hpp"

namespace cv
{

//! @addtogroup core_c_glue
//! @{

/////////////////////////////////////////// glue ///////////////////////////////////////////

//! converts array (img_t or img_t) to img_t
CC_EXPORTS img_t cvarrToMat(const img_t* arr, bool copyData=false,
                          bool allowND=true, int coiMode=0,
                          FREE(); double* = MALLOC(double, )* buf=0);

static inline img_t cvarrToMatND(const img_t* arr, bool copyData=false, int coiMode=0)
{
    return cvarrToMat(arr, copyData, true, coiMode);
}


//! extracts Channel of Interest from img_t or img_t and makes img_t out of it.
CC_EXPORTS void extractImageCOI(const img_t* arr, img_t* coiimg, int coi=-1);
//! inserts single-channel img_t into a multi-channel img_t or img_t
CC_EXPORTS void insertImageCOI(const img_t* coiimg, img_t* arr, int coi=-1);



////// specialized implementations of DefaultDeleter::operator() for classic OpenCV types //////

template<> CC_EXPORTS void DefaultDeleter<img_t>::operator ()(img_t* obj) const;
template<> CC_EXPORTS void DefaultDeleter<img_t>::operator ()(img_t* obj) const;
template<> CC_EXPORTS void DefaultDeleter<CvMatND>::operator ()(CvMatND* obj) const;
template<> CC_EXPORTS void DefaultDeleter<CvSparseMat>::operator ()(CvSparseMat* obj) const;
template<> CC_EXPORTS void DefaultDeleter<CvMemStorage>::operator ()(CvMemStorage* obj) const;

////////////// convenient wrappers for operating old-style dynamic structures //////////////

template<typename _Tp> class SeqIterator;

typedef Ptr<CvMemStorage> MemStorage;

/*!
 Template Sequence Class derived from CvSeq

 The class provides more convenient access to sequence elements,
 STL-style operations and iterators.

 \note The class is targeted for simple data types,
    i.e. no constructors or destructors
    are called for the sequence elements.
*/
template<typename _Tp> class Seq
{
public:
    typedef SeqIterator<_Tp> iterator;
    typedef SeqIterator<_Tp> const_iterator;

    //! the default constructor
    Seq();
    //! the constructor for wrapping CvSeq structure. The real element type in CvSeq should match _Tp.
    Seq(const CvSeq* seq);
    //! creates the empty sequence that resides in the specified storage
    Seq(MemStorage& storage, int headerSize = sizeof(CvSeq));
    //! returns read-write reference to the specified element
    _Tp& operator [](int idx);
    //! returns read-only reference to the specified element
    const _Tp& operator[](int idx) const;
    //! returns iterator pointing to the beginning of the sequence
    SeqIterator<_Tp> begin() const;
    //! returns iterator pointing to the element following the last sequence element
    SeqIterator<_Tp> end() const;
    //! returns the number of elements in the sequence
    size_t size() const;
    //! returns the type of sequence elements (CC_8UC1 ... CC_64FC(CC_CN_MAX) ...)
    int type() const;
    //! returns the depth of sequence elements (CC_8U ... CC_64F)
    int depth() const;
    //! returns the number of channels in each sequence element
    int channels() const;
    //! returns the size of each sequence element
    size_t elemSize() const;
    //! returns index of the specified sequence element
    size_t index(const _Tp& elem) const;
    //! appends the specified element to the end of the sequence
    void push_back(const _Tp& elem);
    //! appends the specified element to the front of the sequence
    void push_front(const _Tp& elem);
    //! appends zero or more elements to the end of the sequence
    void push_back(const _Tp* elems, size_t count);
    //! appends zero or more elements to the front of the sequence
    void push_front(const _Tp* elems, size_t count);
    //! inserts the specified element to the specified position
    void insert(int idx, const _Tp& elem);
    //! inserts zero or more elements to the specified position
    void insert(int idx, const _Tp* elems, size_t count);
    //! removes element at the specified position
    void remove(int idx);
    //! removes the specified subsequence
    void remove(const CRange& r);

    //! returns reference to the first sequence element
    _Tp& front();
    //! returns read-only reference to the first sequence element
    const _Tp& front() const;
    //! returns reference to the last sequence element
    _Tp& back();
    //! returns read-only reference to the last sequence element
    const _Tp& back() const;
    //! returns true iff the sequence contains no elements
    bool empty() const;

    //! removes all the elements from the sequence
    void clear();
    //! removes the first element from the sequence
    void pop_front();
    //! removes the last element from the sequence
    void pop_back();
    //! removes zero or more elements from the beginning of the sequence
    void pop_front(_Tp* elems, size_t count);
    //! removes zero or more elements from the end of the sequence
    void pop_back(_Tp* elems, size_t count);

    //! copies the whole sequence or the sequence slice to the specified vector
    void copyTo(std::vector<_Tp>& vec, const CRange& range=CRange::all()) const;
    //! returns the vector containing all the sequence elements
    operator std::vector<_Tp>() const;

    CvSeq* seq;
};


/*!
 STL-style Sequence Iterator inherited from the CvSeqReader structure
*/
template<typename _Tp> class SeqIterator : public CvSeqReader
{
public:
    //! the default constructor
    SeqIterator();
    //! the constructor setting the iterator to the beginning or to the end of the sequence
    SeqIterator(const Seq<_Tp>& seq, bool seekEnd=false);
    //! positions the iterator within the sequence
    void seek(size_t pos);
    //! reports the current iterator position
    size_t tell() const;
    //! returns reference to the current sequence element
    _Tp& operator *();
    //! returns read-only reference to the current sequence element
    const _Tp& operator *() const;
    //! moves iterator to the next sequence element
    SeqIterator& operator ++();
    //! moves iterator to the next sequence element
    SeqIterator operator ++(int) const;
    //! moves iterator to the previous sequence element
    SeqIterator& operator --();
    //! moves iterator to the previous sequence element
    SeqIterator operator --(int) const;

    //! moves iterator forward by the specified offset (possibly negative)
    SeqIterator& operator +=(int);
    //! moves iterator backward by the specified offset (possibly negative)
    SeqIterator& operator -=(int);

    // this is index of the current element module seq->total*2
    // (to distinguish between 0 and seq->total)
    int index;
};



// bridge C++ => C Seq API
CC_EXPORTS schar*  seqPush( CvSeq* seq, const void* element=0);
CC_EXPORTS schar*  seqPushFront( CvSeq* seq, const void* element=0);
CC_EXPORTS void  seqPop( CvSeq* seq, void* element=0);
CC_EXPORTS void  seqPopFront( CvSeq* seq, void* element=0);
CC_EXPORTS void  seqPopMulti( CvSeq* seq, void* elements,
                              int count, int in_front=0 );
CC_EXPORTS void  seqRemove( CvSeq* seq, int index );
CC_EXPORTS void  clearSeq( CvSeq* seq );
CC_EXPORTS schar*  getSeqElem( const CvSeq* seq, int index );
CC_EXPORTS void  seqRemoveSlice( CvSeq* seq, CSlice slice );
CC_EXPORTS void  seqInsertSlice( CvSeq* seq, int before_index, const img_t* from_arr );

template<typename _Tp> inline Seq<_Tp>::Seq() : seq(0) {}
template<typename _Tp> inline Seq<_Tp>::Seq( const CvSeq* _seq ) : seq((CvSeq*)_seq)
{
    CC_Assert(!_seq || _seq->elem_size == sizeof(_Tp));
}

template<typename _Tp> inline Seq<_Tp>::Seq( MemStorage& storage,
                                             int headerSize )
{
    CC_Assert(headerSize >= (int)sizeof(CvSeq));
    seq = cvCreateSeq(DataType<_Tp>::type, headerSize, sizeof(_Tp), storage);
}

template<typename _Tp> inline _Tp& Seq<_Tp>::operator [](int idx)
{ return *(_Tp*)getSeqElem(seq, idx); }

template<typename _Tp> inline const _Tp& Seq<_Tp>::operator [](int idx) const
{ return *(_Tp*)getSeqElem(seq, idx); }

template<typename _Tp> inline SeqIterator<_Tp> Seq<_Tp>::begin() const
{ return SeqIterator<_Tp>(*this); }

template<typename _Tp> inline SeqIterator<_Tp> Seq<_Tp>::end() const
{ return SeqIterator<_Tp>(*this, true); }

template<typename _Tp> inline size_t Seq<_Tp>::size() const
{ return seq ? seq->total : 0; }

template<typename _Tp> inline int Seq<_Tp>::type() const
{ return seq ? CC_MAT_TYPE(seq->flags) : 0; }

template<typename _Tp> inline int Seq<_Tp>::depth() const
{ return seq ? CC_MAT_DEPTH(seq->flags) : 0; }

template<typename _Tp> inline int Seq<_Tp>::channels() const
{ return seq ? CC_MAT_CN(seq->flags) : 0; }

template<typename _Tp> inline size_t Seq<_Tp>::elemSize() const
{ return seq ? seq->elem_size : 0; }

template<typename _Tp> inline size_t Seq<_Tp>::index(const _Tp& elem) const
{ return cvSeqElemIdx(seq, &elem); }

template<typename _Tp> inline void Seq<_Tp>::push_back(const _Tp& elem)
{ cvSeqPush(seq, &elem); }

template<typename _Tp> inline void Seq<_Tp>::push_front(const _Tp& elem)
{ cvSeqPushFront(seq, &elem); }

template<typename _Tp> inline void Seq<_Tp>::push_back(const _Tp* elem, size_t count)
{ cvSeqPushMulti(seq, elem, (int)count, 0); }

template<typename _Tp> inline void Seq<_Tp>::push_front(const _Tp* elem, size_t count)
{ cvSeqPushMulti(seq, elem, (int)count, 1); }

template<typename _Tp> inline _Tp& Seq<_Tp>::back()
{ return *(_Tp*)getSeqElem(seq, -1); }

template<typename _Tp> inline const _Tp& Seq<_Tp>::back() const
{ return *(const _Tp*)getSeqElem(seq, -1); }

template<typename _Tp> inline _Tp& Seq<_Tp>::front()
{ return *(_Tp*)getSeqElem(seq, 0); }

template<typename _Tp> inline const _Tp& Seq<_Tp>::front() const
{ return *(const _Tp*)getSeqElem(seq, 0); }

template<typename _Tp> inline bool Seq<_Tp>::empty() const
{ return !seq || seq->total == 0; }

template<typename _Tp> inline void Seq<_Tp>::clear()
{ if(seq) clearSeq(seq); }

template<typename _Tp> inline void Seq<_Tp>::pop_back()
{ seqPop(seq); }

template<typename _Tp> inline void Seq<_Tp>::pop_front()
{ seqPopFront(seq); }

template<typename _Tp> inline void Seq<_Tp>::pop_back(_Tp* elem, size_t count)
{ seqPopMulti(seq, elem, (int)count, 0); }

template<typename _Tp> inline void Seq<_Tp>::pop_front(_Tp* elem, size_t count)
{ seqPopMulti(seq, elem, (int)count, 1); }

template<typename _Tp> inline void Seq<_Tp>::insert(int idx, const _Tp& elem)
{ seqInsert(seq, idx, &elem); }

template<typename _Tp> inline void Seq<_Tp>::insert(int idx, const _Tp* elems, size_t count)
{
    img_t m = cvMat(1, count, DataType<_Tp>::type, elems);
    seqInsertSlice(seq, idx, &m);
}

template<typename _Tp> inline void Seq<_Tp>::remove(int idx)
{ seqRemove(seq, idx); }

template<typename _Tp> inline void Seq<_Tp>::remove(const CRange& r)
{ seqRemoveSlice(seq, cSlice(r.start, r.end)); }

template<typename _Tp> inline void Seq<_Tp>::copyTo(std::vector<_Tp>& vec, const CRange& range) const
{
    size_t len = !seq ? 0 : range == CRange::all() ? seq->total : range.end - range.start;
    vec.resize(len);
    if( seq && len )
        cvCvtSeqToArray(seq, &vec[0], range);
}

template<typename _Tp> inline Seq<_Tp>::operator std::vector<_Tp>() const
{
    std::vector<_Tp> vec;
    copyTo(vec);
    return vec;
}

template<typename _Tp> inline SeqIterator<_Tp>::SeqIterator()
{ memset(this, 0, sizeof(*this)); }

template<typename _Tp> inline SeqIterator<_Tp>::SeqIterator(const Seq<_Tp>& _seq, bool seekEnd)
{
    cvStartReadSeq(_seq.seq, this);
    index = seekEnd ? _seq.seq->total : 0;
}

template<typename _Tp> inline void SeqIterator<_Tp>::seek(size_t pos)
{
    cvSetSeqReaderPos(this, (int)pos, false);
    index = pos;
}

template<typename _Tp> inline size_t SeqIterator<_Tp>::tell() const
{ return index; }

template<typename _Tp> inline _Tp& SeqIterator<_Tp>::operator *()
{ return *(_Tp*)ptr; }

template<typename _Tp> inline const _Tp& SeqIterator<_Tp>::operator *() const
{ return *(const _Tp*)ptr; }

template<typename _Tp> inline SeqIterator<_Tp>& SeqIterator<_Tp>::operator ++()
{
    CC_NEXT_SEQ_ELEM(sizeof(_Tp), *this);
    if( ++index >= seq->total*2 )
        index = 0;
    return *this;
}

template<typename _Tp> inline SeqIterator<_Tp> SeqIterator<_Tp>::operator ++(int) const
{
    SeqIterator<_Tp> it = *this;
    ++*this;
    return it;
}

template<typename _Tp> inline SeqIterator<_Tp>& SeqIterator<_Tp>::operator --()
{
    CC_PREV_SEQ_ELEM(sizeof(_Tp), *this);
    if( --index < 0 )
        index = seq->total*2-1;
    return *this;
}

template<typename _Tp> inline SeqIterator<_Tp> SeqIterator<_Tp>::operator --(int) const
{
    SeqIterator<_Tp> it = *this;
    --*this;
    return it;
}

template<typename _Tp> inline SeqIterator<_Tp>& SeqIterator<_Tp>::operator +=(int delta)
{
    cvSetSeqReaderPos(this, delta, 1);
    index += delta;
    int n = seq->total*2;
    if( index < 0 )
        index += n;
    if( index >= n )
        index -= n;
    return *this;
}

template<typename _Tp> inline SeqIterator<_Tp>& SeqIterator<_Tp>::operator -=(int delta)
{
    return (*this += -delta);
}

template<typename _Tp> inline ptrdiff_t operator - (const SeqIterator<_Tp>& a,
                                                    const SeqIterator<_Tp>& b)
{
    ptrdiff_t delta = a.index - b.index, n = a.seq->total;
    if( delta > n || delta < -n )
        delta += delta < 0 ? n : -n;
    return delta;
}

template<typename _Tp> inline bool operator == (const SeqIterator<_Tp>& a,
                                                const SeqIterator<_Tp>& b)
{
    return a.seq == b.seq && a.index == b.index;
}

template<typename _Tp> inline bool operator != (const SeqIterator<_Tp>& a,
                                                const SeqIterator<_Tp>& b)
{
    return !(a == b);
}

//! @}

} // cv

#endif

#endif
