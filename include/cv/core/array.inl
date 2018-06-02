/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2000, Intel Corporation, all rights reserved.
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
//   * The name of Intel Corporation may not be used to endorse or promote products
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

/* ////////////////////////////////////////////////////////////////////
//
//  img_t, CvMatND, CvSparceMat and img_t support functions
//  (creation, deletion, copying, retrieving and setting elements etc.)
//
// */



#define  CC_ORIGIN_TL  0
#define  CC_ORIGIN_BL  1

/* default image row align (in bytes) */
#define  CC_DEFAULT_IMAGE_ROW_ALIGN  4


static struct
{
    Cv_iplCreateImageHeader  createHeader;
    Cv_iplAllocateImageData  allocateData;
    Cv_iplDeallocate  deallocate;
    Cv_iplCreateROI  createROI;
    Cv_iplCloneImage  cloneImage;
}
CvIPL;

// Makes the library use native IPL image allocators
CC_IMPL void
cvSetIPLAllocators( Cv_iplCreateImageHeader createHeader,
                    Cv_iplAllocateImageData allocateData,
                    Cv_iplDeallocate deallocate,
                    Cv_iplCreateROI createROI,
                    Cv_iplCloneImage cloneImage )
{
    int count = (createHeader != 0) + (allocateData != 0) + (deallocate != 0) +
        (createROI != 0) + (cloneImage != 0);

    if( count != 0 && count != 5 )
        CC_Error( CC_StsBadArg, "Either all the pointers should be null or "
                                 "they all should be non-null" );

    CvIPL.createHeader = createHeader;
    CvIPL.allocateData = allocateData;
    CvIPL.deallocate = deallocate;
    CvIPL.createROI = createROI;
    CvIPL.cloneImage = cloneImage;
}


/****************************************************************************************\
*                               img_t creation and basic operations                      *
\****************************************************************************************/

// Creates img_t and underlying data
CC_IMPL img_t*
cvCreateMat( int height, int width, int type )
{
    img_t* arr = cvCreateMatHeader( height, width, type );
    cvCreateData( arr );

    return arr;
}


static void icvCheckHuge( img_t* arr )
{
    if( (int64)arr->step*arr->rows > INT_MAX )
        arr->tid &= ~CC_MAT_CONT_FLAG;
}

// Creates img_t header only
CC_IMPL img_t*
cvCreateMatHeader( int rows, int cols, int type )
{
    type = CC_MAT_DEPTH(type);

    if( rows < 0 || cols < 0 )
        CC_Error( CC_StsBadSize, "Non-positive width or height" );

    int min_step = CC_ELEM_SIZE(type);
    if( min_step <= 0 )
        CC_Error( CC_StsUnsupportedFormat, "Invalid matrix type" );
    min_step *= cols;

    img_t* arr = (img_t*)cAlloc( sizeof(*arr));

    arr->step = min_step;
    arr->tid = CC_MAT_MAGIC_VAL | type | CC_MAT_CONT_FLAG;
    arr->rows = rows;
    arr->cols = cols;
    arr->data.ptr = 0;
    arr->refcount = 0;
    arr->hdr_refcount = 1;

    icvCheckHuge( arr );
    return arr;
}


// Initializes img_t header, allocated by the user
CC_IMPL img_t*
cvInitMatHeader( img_t* arr, int rows, int cols,
                 int type, void* data, int step )
{
    if( !arr )
        CC_Error( CC_StsNullPtr, "" );

    if( (unsigned)CC_MAT_DEPTH(type) > CC_DEPTH_MAX )
        CC_Error( CC_BadNumChannels, "" );

    if( rows < 0 || cols < 0 )
        CC_Error( CC_StsBadSize, "Non-positive cols or rows" );

    type = CC_MAT_DEPTH( type );
    arr->tid = type | CC_MAT_MAGIC_VAL;
    arr->rows = rows;
    arr->cols = cols;
    arr->data.ptr = (uchar*)data;
    arr->refcount = 0;
    arr->hdr_refcount = 0;

    int pix_size = CC_ELEM_SIZE(type);
    int min_step = arr->cols*pix_size;

    if( step != CC_AUTOSTEP && step != 0 )
    {
        if( step < min_step )
            CC_Error( CC_BadStep, "" );
        arr->step = step;
    }
    else
    {
        arr->step = min_step;
    }

    arr->tid = CC_MAT_MAGIC_VAL | type |
        (arr->rows == 1 || arr->step == min_step ? CC_MAT_CONT_FLAG : 0);

    icvCheckHuge( arr );
    return arr;
}


// Deallocates the img_t structure and underlying data
CC_IMPL void
cvReleaseMat( img_t** array )
{
    if( !array )
        CC_Error( CC_HeaderIsNull, "" );

    if( *array )
    {
        img_t* arr = *array;

        if( !CC_IS_MAT_HDR_Z(arr) && !CC_IS_MATND_HDR(arr) )
            CC_Error( CC_StsBadFlag, "" );

        *array = 0;

        cvDecRefData( arr );
        cFree( &arr );
    }
}


// Creates a copy of matrix
CC_IMPL img_t*
cvCloneMat( const img_t* src )
{
    if( !CC_IS_MAT_HDR( src ))
        CC_Error( CC_StsBadArg, "Bad img_t header" );

    img_t* dst = cvCreateMatHeader( src->rows, src->cols, src->tid );

    if( src->data.ptr )
    {
        cvCreateData( dst );
        cvCopy( src, dst );
    }

    return dst;
}


/****************************************************************************************\
*                               CvMatND creation and basic operations                    *
\****************************************************************************************/

CC_IMPL CvMatND*
cvInitMatNDHeader( CvMatND* mat, int dims, const int* sizes,
                   int type, void* data )
{
    type = CC_MAT_DEPTH(type);
    int64 step = CC_ELEM_SIZE(type);

    if( !mat )
        CC_Error( CC_StsNullPtr, "NULL matrix header pointer" );

    if( step == 0 )
        CC_Error( CC_StsUnsupportedFormat, "invalid array data type" );

    if( !sizes )
        CC_Error( CC_StsNullPtr, "NULL <sizes> pointer" );

    if( dims <= 0 || dims > CC_MAX_DIM )
        CC_Error( CC_StsOutOfRange,
        "non-positive or too large number of dimensions" );

    for( int i = dims - 1; i >= 0; i-- )
    {
        if( sizes[i] < 0 )
            CC_Error( CC_StsBadSize, "one of dimesion sizes is non-positive" );
        mat->dim[i].size = sizes[i];
        if( step > INT_MAX )
            CC_Error( CC_StsOutOfRange, "The array is too big" );
        mat->dim[i]->step = (int)step;
        step *= sizes[i];
    }

    mat->tid = CC_MATND_MAGIC_VAL | (step <= INT_MAX ? CC_MAT_CONT_FLAG : 0) | type;
    mat->dims = dims;
    mat->data.ptr = (uchar*)data;
    mat->refcount = 0;
    mat->hdr_refcount = 0;
    return mat;
}


// Creates CvMatND and underlying data
CC_IMPL CvMatND*
cvCreateMatND( int dims, const int* sizes, int type )
{
    CvMatND* arr = cvCreateMatNDHeader( dims, sizes, type );
    cvCreateData( arr );

    return arr;
}


// Creates CvMatND header only
CC_IMPL CvMatND*
cvCreateMatNDHeader( int dims, const int* sizes, int type )
{
    if( dims <= 0 || dims > CC_MAX_DIM )
        CC_Error( CC_StsOutOfRange,
        "non-positive or too large number of dimensions" );

    CvMatND* arr = (CvMatND*)cAlloc( sizeof(*arr) );

    cvInitMatNDHeader( arr, dims, sizes, type, 0 );
    arr->hdr_refcount = 1;
    return arr;
}


// Creates a copy of nD array
CC_IMPL CvMatND*
cvCloneMatND( const CvMatND* src )
{
    if( !CC_IS_MATND_HDR( src ))
        CC_Error( CC_StsBadArg, "Bad CvMatND header" );

    CC_Assert( src->dims <= CC_MAX_DIM );
    int sizes[CC_MAX_DIM];

    for( int i = 0; i < src->dims; i++ )
        sizes[i] = src->dim[i].size;

    CvMatND* dst = cvCreateMatNDHeader( src->dims, sizes, src->tid );

    if( src->data.ptr )
    {
        cvCreateData( dst );
        img_t _src = cvarrToMat(src);
        img_t _dst = cvarrToMat(dst);
        uchar* data0 = dst->data.ptr;
        _src.copyTo(_dst);
        CC_Assert(_dst.data == data0);
        //cvCopy( src, dst );
    }

    return dst;
}


static CvMatND*
cvGetMatND( const img_t* arr, CvMatND* matnd, int* coi )
{
    CvMatND* result = 0;

    if( coi )
        *coi = 0;

    if( !matnd || !arr )
        CC_Error( CC_StsNullPtr, "NULL array pointer is passed" );

    if( CC_IS_MATND_HDR(arr))
    {
        if( !((CvMatND*)arr)->data.ptr )
            CC_Error( CC_StsNullPtr, "The matrix has NULL data pointer" );

        result = (CvMatND*)arr;
    }
    else
    {
        img_t stub, *mat = (img_t*)arr;

        if( CC_IS_IMAGE_HDR( mat ))
            mat = cvGetMat( mat, &stub, coi );

        if( !CC_IS_MAT_HDR( mat ))
            CC_Error( CC_StsBadArg, "Unrecognized or unsupported array type" );

        if( !mat->data.ptr )
            CC_Error( CC_StsNullPtr, "Input array has NULL data pointer" );

        matnd->data.ptr = mat->data.ptr;
        matnd->refcount = 0;
        matnd->hdr_refcount = 0;
        matnd->tid = mat->tid;
        matnd->dims = 2;
        matnd->dim[0].size = mat->rows;
        matnd->dim[0]->step = mat->step;
        matnd->dim[1].size = mat->cols;
        matnd->dim[1]->step = CC_ELEM_SIZE(mat->tid);
        result = matnd;
    }

    return result;
}


// returns number of dimensions to iterate.
/*
Checks whether <count> arrays have equal type, sizes (mask is optional array
that needs to have the same size, but 8uC1 or 8sC1 type).
Returns number of dimensions to iterate through:
0 means that all arrays are continuous,
1 means that all arrays are vectors of continuous arrays etc.
and the size of largest common continuous part of the arrays
*/
CC_IMPL int
cvInitNArrayIterator( int count, img_t** arrs,
                      const img_t* mask, CvMatND* stubs,
                      CvNArrayIterator* iterator, int flags )
{
    int dims = -1;
    int i, j, size, dim0 = -1;
    int64 step;
    CvMatND* hdr0 = 0;

    if( count < 1 || count > CC_MAX_ARR )
        CC_Error( CC_StsOutOfRange, "Incorrect number of arrays" );

    if( !arrs || !stubs )
        CC_Error( CC_StsNullPtr, "Some of required array pointers is NULL" );

    if( !iterator )
        CC_Error( CC_StsNullPtr, "Iterator pointer is NULL" );

    for( i = 0; i <= count; i++ )
    {
        const img_t* arr = i < count ? arrs[i] : mask;
        CvMatND* hdr;

        if( !arr )
        {
            if( i < count )
                CC_Error( CC_StsNullPtr, "Some of required array pointers is NULL" );
            break;
        }

        if( CC_IS_MATND( arr ))
            hdr = (CvMatND*)arr;
        else
        {
            int coi = 0;
            hdr = cvGetMatND( arr, stubs + i, &coi );
            if( coi != 0 )
                CC_Error( CC_BadCOI, "COI set is not allowed here" );
        }

        iterator->hdr[i] = hdr;

        if( i > 0 )
        {
            if( hdr->dims != hdr0->dims )
                CC_Error( CC_StsUnmatchedSizes,
                          "Number of dimensions is the same for all arrays" );

            if( i < count )
            {
                switch( flags & (CC_NO_DEPTH_CHECK|CC_NO_CN_CHECK))
                {
                case 0:
                    if( !CC_ARE_TYPES_EQ( hdr, hdr0 ))
                        CC_Error( CC_StsUnmatchedFormats,
                                  "Data type is not the same for all arrays" );
                    break;
                case CC_NO_DEPTH_CHECK:
                    if( !CC_ARE_CNS_EQ( hdr, hdr0 ))
                        CC_Error( CC_StsUnmatchedFormats,
                                  "Number of channels is not the same for all arrays" );
                    break;
                case CC_NO_CN_CHECK:
                    if( !CC_ARE_CNS_EQ( hdr, hdr0 ))
                        CC_Error( CC_StsUnmatchedFormats,
                                  "Depth is not the same for all arrays" );
                    break;
                }
            }
            else
            {
                if( !CC_IS_MASK_ARR( hdr ))
                    CC_Error( CC_StsBadMask, "Mask should have 8uC1 or 8sC1 data type" );
            }

            if( !(flags & CC_NO_SIZE_CHECK) )
            {
                for( j = 0; j < hdr->dims; j++ )
                    if( hdr->dim[j].size != hdr0->dim[j].size )
                        CC_Error( CC_StsUnmatchedSizes,
                                  "Dimension sizes are the same for all arrays" );
            }
        }
        else
            hdr0 = hdr;

        step = CC_ELEM_SIZE(hdr->tid);
        for( j = hdr->dims - 1; j > dim0; j-- )
        {
            if( step != hdr->dim[j]->step )
                break;
            step *= hdr->dim[j].size;
        }

        if( j == dim0 && step > INT_MAX )
            j++;

        if( j > dim0 )
            dim0 = j;

        iterator->hdr[i] = (CvMatND*)hdr;
        iterator->ptr[i] = (uchar*)hdr->data.ptr;
    }

    size = 1;
    for( j = hdr0->dims - 1; j > dim0; j-- )
        size *= hdr0->dim[j].size;

    dims = dim0 + 1;
    iterator->dims = dims;
    iterator->count = count;
    iterator->size = cSize(size,1);

    for( i = 0; i < dims; i++ )
        iterator->stack[i] = hdr0->dim[i].size;

    return dims;
}


// returns zero value if iteration is finished, non-zero otherwise
CC_IMPL int cvNextNArraySlice( CvNArrayIterator* iterator )
{
    assert( iterator != 0 );
    int i, dims;

    for( dims = iterator->dims; dims > 0; dims-- )
    {
        for( i = 0; i < iterator->count; i++ )
            iterator->ptr[i] += iterator->hdr[i]->dim[dims-1]->step;

        if( --iterator->stack[dims-1] > 0 )
            break;

        const int size = iterator->hdr[0]->dim[dims-1].size;

        for( i = 0; i < iterator->count; i++ )
            iterator->ptr[i] -= (size_t)size*iterator->hdr[i]->dim[dims-1]->step;

        iterator->stack[dims-1] = size;
    }

    return dims > 0;
}


/****************************************************************************************\
*                            CvSparseMat creation and basic operations                   *
\****************************************************************************************/


// Creates CvMatND and underlying data
CC_IMPL CvSparseMat*
cvCreateSparseMat( int dims, const int* sizes, int type )
{
    type = CC_MAT_DEPTH( type );
    int pix_size1 = CC_ELEM_SIZE1(type);
    int pix_size = pix_size1*CC_MAT_CN(type);
    int i, size;
    CvMemStorage* storage;

    if( pix_size == 0 )
        CC_Error( CC_StsUnsupportedFormat, "invalid array data type" );

    if( dims <= 0 || dims > CC_MAX_DIM_HEAP )
        CC_Error( CC_StsOutOfRange, "bad number of dimensions" );

    if( !sizes )
        CC_Error( CC_StsNullPtr, "NULL <sizes> pointer" );

    for( i = 0; i < dims; i++ )
    {
        if( sizes[i] <= 0 )
            CC_Error( CC_StsBadSize, "one of dimesion sizes is non-positive" );
    }

    CvSparseMat* arr = (CvSparseMat*)cAlloc(sizeof(*arr)+MAX(0,dims-CC_MAX_DIM)*sizeof(arr->size[0]));

    arr->tid = CC_SPARSE_MAT_MAGIC_VAL | type;
    arr->dims = dims;
    arr->refcount = 0;
    arr->hdr_refcount = 1;
    memcpy( arr->size, sizes, dims*sizeof(sizes[0]));

    arr->valoffset = (int)cvAlign(sizeof(CvSparseNode), pix_size1);
    arr->idxoffset = (int)cvAlign(arr->valoffset + pix_size, sizeof(int));
    size = (int)cvAlign(arr->idxoffset + dims*sizeof(int), sizeof(CvSetElem));

    storage = cvCreateMemStorage( CC_SPARSE_MAT_BLOCK );
    arr->heap = cvCreateSet( 0, sizeof(CvSet), size, storage );

    arr->hashsize = CC_SPARSE_HASH_SIZE0;
    size = arr->hashsize*sizeof(arr->hashtable[0]);

    arr->hashtable = (void**)cAlloc( size );
    memset( arr->hashtable, 0, size );

    return arr;
}


// Creates CvMatND and underlying data
CC_IMPL void
cvReleaseSparseMat( CvSparseMat** array )
{
    if( !array )
        CC_Error( CC_HeaderIsNull, "" );

    if( *array )
    {
        CvSparseMat* arr = *array;

        if( !CC_IS_SPARSE_MAT_HDR(arr) )
            CC_Error( CC_StsBadFlag, "" );

        *array = 0;

        CvMemStorage* storage = arr->heap->storage;
        cvReleaseMemStorage( &storage );
        cFree( &arr->hashtable );
        cFree( &arr );
    }
}


// Creates CvMatND and underlying data
CC_IMPL CvSparseMat*
cvCloneSparseMat( const CvSparseMat* src )
{
    if( !CC_IS_SPARSE_MAT_HDR(src) )
        CC_Error( CC_StsBadArg, "Invalid sparse array header" );

    CvSparseMat* dst = cvCreateSparseMat( src->dims, src->size, src->tid );
    cvCopy( src, dst );
    return dst;
}


CvSparseNode*
cvInitSparseMatIterator( const CvSparseMat* mat, CvSparseMatIterator* iterator )
{
    CvSparseNode* node = 0;
    int idx;

    if( !CC_IS_SPARSE_MAT( mat ))
        CC_Error( CC_StsBadArg, "Invalid sparse matrix header" );

    if( !iterator )
        CC_Error( CC_StsNullPtr, "NULL iterator pointer" );

    iterator->mat = (CvSparseMat*)mat;
    iterator->node = 0;

    for( idx = 0; idx < mat->hashsize; idx++ )
        if( mat->hashtable[idx] )
        {
            node = iterator->node = (CvSparseNode*)mat->hashtable[idx];
            break;
        }

    iterator->curidx = idx;
    return node;
}

#define ICC_SPARSE_MAT_HASH_MULTIPLIER  SparseMat::HASH_SCALE

static uchar*
icvGetNodePtr( CvSparseMat* mat, const int* idx, int* _type,
               int create_node, unsigned* precalc_hashval )
{
    uchar* ptr = 0;
    int i, tabidx;
    unsigned hashval = 0;
    CvSparseNode *node;
    assert( CC_IS_SPARSE_MAT( mat ));

    if( !precalc_hashval )
    {
        for( i = 0; i < mat->dims; i++ )
        {
            int t = idx[i];
            if( (unsigned)t >= (unsigned)mat->size[i] )
                CC_Error( CC_StsOutOfRange, "One of indices is out of range" );
            hashval = hashval*ICC_SPARSE_MAT_HASH_MULTIPLIER + t;
        }
    }
    else
    {
        hashval = *precalc_hashval;
    }

    tabidx = hashval & (mat->hashsize - 1);
    hashval &= INT_MAX;

    if( create_node >= -1 )
    {
        for( node = (CvSparseNode*)mat->hashtable[tabidx];
             node != 0; node = node->next )
        {
            if( node->hashval == hashval )
            {
                int* nodeidx = CC_NODE_IDX(mat,node);
                for( i = 0; i < mat->dims; i++ )
                    if( idx[i] != nodeidx[i] )
                        break;
                if( i == mat->dims )
                {
                    ptr = (uchar*)CC_NODE_VAL(mat,node);
                    break;
                }
            }
        }
    }

    if( !ptr && create_node )
    {
        if( mat->heap->active_count >= mat->hashsize*CC_SPARSE_HASH_RATIO )
        {
            void** newtable;
            int newsize = MAX( mat->hashsize*2, CC_SPARSE_HASH_SIZE0);
            int newrawsize = newsize*sizeof(newtable[0]);

            CvSparseMatIterator iterator;
            assert( (newsize & (newsize - 1)) == 0 );

            // resize hash table
            newtable = (void**)cAlloc( newrawsize );
            memset( newtable, 0, newrawsize );

            node = cvInitSparseMatIterator( mat, &iterator );
            while( node )
            {
                CvSparseNode* next = cvGetNextSparseNode( &iterator );
                int newidx = node->hashval & (newsize - 1);
                node->next = (CvSparseNode*)newtable[newidx];
                newtable[newidx] = node;
                node = next;
            }

            cFree( &mat->hashtable );
            mat->hashtable = newtable;
            mat->hashsize = newsize;
            tabidx = hashval & (newsize - 1);
        }

        node = (CvSparseNode*)cvSetNew( mat->heap );
        node->hashval = hashval;
        node->next = (CvSparseNode*)mat->hashtable[tabidx];
        mat->hashtable[tabidx] = node;
        memcpy(CC_NODE_IDX(mat,node), idx, mat->dims*sizeof(idx[0]));
        ptr = (uchar*)CC_NODE_VAL(mat,node);
        if( create_node > 0 )
            memset( ptr, 0, CC_ELEM_SIZE(mat->tid));
    }

    if( _type )
        *_type = CC_MAT_DEPTH(mat->tid);

    return ptr;
}


static void
icvDeleteNode( CvSparseMat* mat, const int* idx, unsigned* precalc_hashval )
{
    int i, tabidx;
    unsigned hashval = 0;
    CvSparseNode *node, *prev = 0;
    assert( CC_IS_SPARSE_MAT( mat ));

    if( !precalc_hashval )
    {
        for( i = 0; i < mat->dims; i++ )
        {
            int t = idx[i];
            if( (unsigned)t >= (unsigned)mat->size[i] )
                CC_Error( CC_StsOutOfRange, "One of indices is out of range" );
            hashval = hashval*ICC_SPARSE_MAT_HASH_MULTIPLIER + t;
        }
    }
    else
    {
        hashval = *precalc_hashval;
    }

    tabidx = hashval & (mat->hashsize - 1);
    hashval &= INT_MAX;

    for( node = (CvSparseNode*)mat->hashtable[tabidx];
         node != 0; prev = node, node = node->next )
    {
        if( node->hashval == hashval )
        {
            int* nodeidx = CC_NODE_IDX(mat,node);
            for( i = 0; i < mat->dims; i++ )
                if( idx[i] != nodeidx[i] )
                    break;
            if( i == mat->dims )
                break;
        }
    }

    if( node )
    {
        if( prev )
            prev->next = node->next;
        else
            mat->hashtable[tabidx] = node->next;
        cvSetRemoveByPtr( mat->heap, node );
    }
}


/****************************************************************************************\
*                          Common for multiple array types operations                    *
\****************************************************************************************/

// Allocates underlying array data
CC_IMPL void
cvCreateData( img_t* arr )
{
    if( CC_IS_MAT_HDR_Z( arr ))
    {
        size_t step, total_size;
        img_t* mat = (img_t*)arr;
        step = mat->step;

        if( mat->rows == 0 || mat->cols == 0 )
            return;

        if( mat->data.ptr != 0 )
            CC_Error( CC_StsError, "Data is already allocated" );

        if( step == 0 )
            step = CC_ELEM_SIZE(mat->tid)*mat->cols;

        int64 _total_size = (int64)step*mat->rows + sizeof(int) + CC_MALLOC_ALIGN;
        total_size = (size_t)_total_size;
        if(_total_size != (int64)total_size)
            CC_Error(CC_StsNoMem, "Too big buffer is allocated" );
        mat->refcount = (int*)cAlloc( (size_t)total_size );
        mat->data.ptr = (uchar*)cvAlignPtr( mat->refcount + 1, CC_MALLOC_ALIGN );
        *mat->refcount = 1;
    }
    else if( CC_IS_IMAGE_HDR(arr))
    {
        img_t* img = (img_t*)arr;

        if( img->imageData != 0 )
            CC_Error( CC_StsError, "Data is already allocated" );

        if( !CvIPL.allocateData )
        {
            const int64 imageSize_tmp = (int64)img->widthStep*(int64)img->height;
            if( (int64)img->imageSize != imageSize_tmp )
                CC_Error( CC_StsNoMem, "Overflow for imageSize" );
            img->imageData = img->imageDataOrigin =
                        (char*)cAlloc( (size_t)img->imageSize );
        }
        else
        {
            int depth = img->depth;
            int width = img->width;

            if( img->depth == IMG_DEPTH_32F || img->depth == IMG_DEPTH_64F )
            {
                img->width *= img->depth == IMG_DEPTH_32F ? sizeof(float) : sizeof(double);
                img->depth = IMG_DEPTH_8U;
            }

            CvIPL.allocateData( img, 0, 0 );

            img->width = width;
            img->depth = depth;
        }
    }
    else if( CC_IS_MATND_HDR( arr ))
    {
        CvMatND* mat = (CvMatND*)arr;
        size_t total_size = CC_ELEM_SIZE(mat->tid);

        if( mat->dim[0].size == 0 )
            return;

        if( mat->data.ptr != 0 )
            CC_Error( CC_StsError, "Data is already allocated" );

        if( CC_IS_MAT_CONT( mat->tid ))
        {
            total_size = (size_t)mat->dim[0].size*(mat->dim[0]->step != 0 ?
                         (size_t)mat->dim[0]->step : total_size);
        }
        else
        {
            int i;
            for( i = mat->dims - 1; i >= 0; i-- )
            {
                size_t size = (size_t)mat->dim[i]->step*mat->dim[i].size;

                if( total_size < size )
                    total_size = size;
            }
        }

        mat->refcount = (int*)cAlloc( total_size +
                                        sizeof(int) + CC_MALLOC_ALIGN );
        mat->data.ptr = (uchar*)cvAlignPtr( mat->refcount + 1, CC_MALLOC_ALIGN );
        *mat->refcount = 1;
    }
    else
        CC_Error( CC_StsBadArg, "unrecognized or unsupported array type" );
}


// Assigns external data to array
CC_IMPL void
cvSetData( img_t* arr, void* data, int step )
{
    int pix_size, min_step;

    if( CC_IS_MAT_HDR(arr) || CC_IS_MATND_HDR(arr) )
        cvReleaseData( arr );

    if( CC_IS_MAT_HDR( arr ))
    {
        img_t* mat = (img_t*)arr;

        int type = CC_MAT_DEPTH(mat->tid);
        pix_size = CC_ELEM_SIZE(type);
        min_step = mat->cols*pix_size;

        if( step != CC_AUTOSTEP && step != 0 )
        {
            if( step < min_step && data != 0 )
                CC_Error( CC_BadStep, "" );
            mat->step = step;
        }
        else
            mat->step = min_step;

        mat->data.ptr = (uchar*)data;
        mat->tid = CC_MAT_MAGIC_VAL | type |
                    (mat->rows == 1 || mat->step == min_step ? CC_MAT_CONT_FLAG : 0);
        icvCheckHuge( mat );
    }
    else if( CC_IS_IMAGE_HDR( arr ))
    {
        img_t* img = (img_t*)arr;

        pix_size = ((img->depth & 255) >> 3)*img->nChannels;
        min_step = img->width*pix_size;

        if( step != CC_AUTOSTEP && img->height > 1 )
        {
            if( step < min_step && data != 0 )
                CC_Error( CC_BadStep, "" );
            img->widthStep = step;
        }
        else
        {
            img->widthStep = min_step;
        }

        const int64 imageSize_tmp = (int64)img->widthStep*(int64)img->height;
        img->imageSize = (int)imageSize_tmp;
        if( (int64)img->imageSize != imageSize_tmp )
            CC_Error( CC_StsNoMem, "Overflow for imageSize" );
        img->imageData = img->imageDataOrigin = (char*)data;

        if( (((int)(size_t)data | step) & 7) == 0 &&
            cvAlign(img->width * pix_size, 8) == step )
            img->align = 8;
        else
            img->align = 4;
    }
    else if( CC_IS_MATND_HDR( arr ))
    {
        CvMatND* mat = (CvMatND*)arr;
        int i;
        int64 cur_step;

        if( step != CC_AUTOSTEP )
            CC_Error( CC_BadStep,
            "For multidimensional array only CC_AUTOSTEP is allowed here" );

        mat->data.ptr = (uchar*)data;
        cur_step = CC_ELEM_SIZE(mat->tid);

        for( i = mat->dims - 1; i >= 0; i-- )
        {
            if( cur_step > INT_MAX )
                CC_Error( CC_StsOutOfRange, "The array is too big" );
            mat->dim[i]->step = (int)cur_step;
            cur_step *= mat->dim[i].size;
        }
    }
    else
        CC_Error( CC_StsBadArg, "unrecognized or unsupported array type" );
}


// Deallocates array's data
CC_IMPL void
cvReleaseData( img_t* arr )
{
    if( CC_IS_MAT_HDR( arr ) || CC_IS_MATND_HDR( arr ))
    {
        img_t* mat = (img_t*)arr;
        cvDecRefData( mat );
    }
    else if( CC_IS_IMAGE_HDR( arr ))
    {
        img_t* img = (img_t*)arr;

        if( !CvIPL.deallocate )
        {
            char* ptr = img->imageDataOrigin;
            img->imageData = img->imageDataOrigin = 0;
            cFree( &ptr );
        }
        else
        {
            CvIPL.deallocate( img, IMG_IMAGE_DATA );
        }
    }
    else
        CC_Error( CC_StsBadArg, "unrecognized or unsupported array type" );
}


// Retrieves essential information about image ROI or img_t data
CC_IMPL void
cvGetRawData( const img_t* arr, uchar** data, int* step, CSize* roi_size )
{
    if( CC_IS_MAT( arr ))
    {
        img_t *mat = (img_t*)arr;

        if( step )
            *step = mat->step;

        if( data )
            *data = mat->data.ptr;

        if( roi_size )
            *roi_size = cvGetMatSize( mat );
    }
    else if( CC_IS_IMAGE( arr ))
    {
        img_t* img = (img_t*)arr;

        if( step )
            *step = img->widthStep;

        if( data )
            *data = cvPtr2D( img, 0, 0 );

        if( roi_size )
        {
            if( img->roi )
            {
                *roi_size = cSize( img->roi->width, img->roi->height );
            }
            else
            {
                *roi_size = cSize( img->width, img->height );
            }
        }
    }
    else if( CC_IS_MATND( arr ))
    {
        CvMatND* mat = (CvMatND*)arr;

        if( !CC_IS_MAT_CONT( mat->tid ))
            CC_Error( CC_StsBadArg, "Only continuous nD arrays are supported here" );

        if( data )
            *data = mat->data.ptr;

        if( roi_size || step )
        {
            if( roi_size )
            {
                int size1 = mat->dim[0].size, size2 = 1;

                if( mat->dims > 2 )
                {
                    int i;
                    for( i = 1; i < mat->dims; i++ )
                        size1 *= mat->dim[i].size;
                }
                else
                    size2 = mat->dim[1].size;

                roi_size->width = size2;
                roi_size->height = size1;
            }

            if( step )
                *step = mat->dim[0]->step;
        }
    }
    else
        CC_Error( CC_StsBadArg, "unrecognized or unsupported array type" );
}


CC_IMPL int
cvGetElemType( const img_t* arr )
{
    int type = -1;
    if( CC_IS_MAT_HDR(arr) || CC_IS_MATND_HDR(arr) || CC_IS_SPARSE_MAT_HDR(arr))
        type = CC_MAT_DEPTH( ((img_t*)arr)->tid );
    else if( CC_IS_IMAGE(arr))
    {
        img_t* img = (img_t*)arr;
        type = CC_MAKETYPE( IPL2CC_DEPTH(img->depth), img->nChannels );
    }
    else
        CC_Error( CC_StsBadArg, "unrecognized or unsupported array type" );

    return type;
}


// Returns a number of array dimensions
CC_IMPL int
cvGetDims( const img_t* arr, int* sizes )
{
    int dims = -1;
    if( CC_IS_MAT_HDR( arr ))
    {
        img_t* mat = (img_t*)arr;

        dims = 2;
        if( sizes )
        {
            sizes[0] = mat->rows;
            sizes[1] = mat->cols;
        }
    }
    else if( CC_IS_IMAGE( arr ))
    {
        img_t* img = (img_t*)arr;
        dims = 2;

        if( sizes )
        {
            sizes[0] = img->height;
            sizes[1] = img->width;
        }
    }
    else if( CC_IS_MATND_HDR( arr ))
    {
        CvMatND* mat = (CvMatND*)arr;
        dims = mat->dims;

        if( sizes )
        {
            int i;
            for( i = 0; i < dims; i++ )
                sizes[i] = mat->dim[i].size;
        }
    }
    else if( CC_IS_SPARSE_MAT_HDR( arr ))
    {
        CvSparseMat* mat = (CvSparseMat*)arr;
        dims = mat->dims;

        if( sizes )
            memcpy( sizes, mat->size, dims*sizeof(sizes[0]));
    }
    else
        CC_Error( CC_StsBadArg, "unrecognized or unsupported array type" );

    return dims;
}


// Returns the size of particular array dimension
CC_IMPL int
cvGetDimSize( const img_t* arr, int index )
{
    int size = -1;

    if( CC_IS_MAT( arr ))
    {
        img_t *mat = (img_t*)arr;

        switch( index )
        {
        case 0:
            size = mat->rows;
            break;
        case 1:
            size = mat->cols;
            break;
        default:
            CC_Error( CC_StsOutOfRange, "bad dimension index" );
        }
    }
    else if( CC_IS_IMAGE( arr ))
    {
        img_t* img = (img_t*)arr;

        switch( index )
        {
        case 0:
            size = !img->roi ? img->height : img->roi->height;
            break;
        case 1:
            size = !img->roi ? img->width : img->roi->width;
            break;
        default:
            CC_Error( CC_StsOutOfRange, "bad dimension index" );
        }
    }
    else if( CC_IS_MATND_HDR( arr ))
    {
        CvMatND* mat = (CvMatND*)arr;

        if( (unsigned)index >= (unsigned)mat->dims )
            CC_Error( CC_StsOutOfRange, "bad dimension index" );

        size = mat->dim[index].size;
    }
    else if( CC_IS_SPARSE_MAT_HDR( arr ))
    {
        CvSparseMat* mat = (CvSparseMat*)arr;

        if( (unsigned)index >= (unsigned)mat->dims )
            CC_Error( CC_StsOutOfRange, "bad dimension index" );

        size = mat->size[index];
    }
    else
        CC_Error( CC_StsBadArg, "unrecognized or unsupported array type" );

    return size;
}


// Returns the size of img_t or img_t
CC_IMPL CSize
cvGetSize( const img_t* arr )
{
    CSize size;

    if( CC_IS_MAT_HDR_Z( arr ))
    {
        img_t *mat = (img_t*)arr;

        size.width = mat->cols;
        size.height = mat->rows;
    }
    else if( CC_IS_IMAGE_HDR( arr ))
    {
        img_t* img = (img_t*)arr;

        if( img->roi )
        {
            size.width = img->roi->width;
            size.height = img->roi->height;
        }
        else
        {
            size.width = img->width;
            size.height = img->height;
        }
    }
    else
        CC_Error( CC_StsBadArg, "Array should be img_t or img_t" );

    return size;
}


// Selects sub-array (no data is copied)
CC_IMPL  img_t*
cvGetSubRect( const img_t* arr, img_t* submat, CRect rect )
{
    img_t* res = 0;
    img_t stub, *mat = (img_t*)arr;

    if( !CC_IS_MAT( mat ))
        mat = cvGetMat( mat, &stub );

    if( !submat )
        CC_Error( CC_StsNullPtr, "" );

    if( (rect.x|rect.y|rect.width|rect.height) < 0 )
        CC_Error( CC_StsBadSize, "" );

    if( rect.x + rect.width > mat->cols ||
        rect.y + rect.height > mat->rows )
        CC_Error( CC_StsBadSize, "" );

    {
    /*
    int* refcount = mat->refcount;

    if( refcount )
        ++*refcount;

    cvDecRefData( submat );
    */
    submat->data.ptr = mat->data.ptr + (size_t)rect.y*mat->step +
                       rect.x*CC_ELEM_SIZE(mat->tid);
    submat->step = mat->step;
    submat->tid = (mat->tid & (rect.width < mat->cols ? ~CC_MAT_CONT_FLAG : -1)) |
                   (rect.height <= 1 ? CC_MAT_CONT_FLAG : 0);
    submat->rows = rect.height;
    submat->cols = rect.width;
    submat->refcount = 0;
    res = submat;
    }

    return res;
}


// Selects array's row span.
CC_IMPL  img_t*
cvGetRows( const img_t* arr, img_t* submat,
           int start_row, int end_row, int delta_row )
{
    img_t* res = 0;
    img_t stub, *mat = (img_t*)arr;

    if( !CC_IS_MAT( mat ))
        mat = cvGetMat( mat, &stub );

    if( !submat )
        CC_Error( CC_StsNullPtr, "" );

    if( (unsigned)start_row >= (unsigned)mat->rows ||
        (unsigned)end_row > (unsigned)mat->rows || delta_row <= 0 )
        CC_Error( CC_StsOutOfRange, "" );

    {
    /*
    int* refcount = mat->refcount;

    if( refcount )
        ++*refcount;

    cvDecRefData( submat );
    */
    if( delta_row == 1 )
    {
        submat->rows = end_row - start_row;
        submat->step = mat->step;
    }
    else
    {
        submat->rows = (end_row - start_row + delta_row - 1)/delta_row;
        submat->step = mat->step * delta_row;
    }

    submat->cols = mat->cols;
    submat->step &= submat->rows > 1 ? -1 : 0;
    submat->data.ptr = mat->data.ptr + (size_t)start_row*mat->step;
    submat->tid = (mat->tid | (submat->rows == 1 ? CC_MAT_CONT_FLAG : 0)) &
                   (delta_row != 1 && submat->rows > 1 ? ~CC_MAT_CONT_FLAG : -1);
    submat->refcount = 0;
    submat->hdr_refcount = 0;
    res = submat;
    }

    return res;
}


// Selects array's column span.
CC_IMPL  img_t*
cvGetCols( const img_t* arr, img_t* submat, int start_col, int end_col )
{
    img_t* res = 0;
    img_t stub, *mat = (img_t*)arr;
    int cols;

    if( !CC_IS_MAT( mat ))
        mat = cvGetMat( mat, &stub );

    if( !submat )
        CC_Error( CC_StsNullPtr, "" );

    cols = mat->cols;
    if( (unsigned)start_col >= (unsigned)cols ||
        (unsigned)end_col > (unsigned)cols )
        CC_Error( CC_StsOutOfRange, "" );

    {
    /*
    int* refcount = mat->refcount;

    if( refcount )
        ++*refcount;

    cvDecRefData( submat );
    */
    submat->rows = mat->rows;
    submat->cols = end_col - start_col;
    submat->step = mat->step;
    submat->data.ptr = mat->data.ptr + (size_t)start_col*CC_ELEM_SIZE(mat->tid);
    submat->tid = mat->tid & (submat->rows > 1 && submat->cols < cols ? ~CC_MAT_CONT_FLAG : -1);
    submat->refcount = 0;
    submat->hdr_refcount = 0;
    res = submat;
    }

    return res;
}


// Selects array diagonal
CC_IMPL  img_t*
cvGetDiag( const img_t* arr, img_t* submat, int diag )
{
    img_t* res = 0;
    img_t stub, *mat = (img_t*)arr;
    int len, pix_size;

    if( !CC_IS_MAT( mat ))
        mat = cvGetMat( mat, &stub );

    if( !submat )
        CC_Error( CC_StsNullPtr, "" );

    pix_size = CC_ELEM_SIZE(mat->tid);

    /*{
    int* refcount = mat->refcount;

    if( refcount )
        ++*refcount;

    cvDecRefData( submat );
    }*/

    if( diag >= 0 )
    {
        len = mat->cols - diag;

        if( len <= 0 )
            CC_Error( CC_StsOutOfRange, "" );

        len = CC_IMIN( len, mat->rows );
        submat->data.ptr = mat->data.ptr + diag*pix_size;
    }
    else
    {
        len = mat->rows + diag;

        if( len <= 0 )
            CC_Error( CC_StsOutOfRange, "" );

        len = CC_IMIN( len, mat->cols );
        submat->data.ptr = mat->data.ptr - diag*mat->step;
    }

    submat->rows = len;
    submat->cols = 1;
    submat->step = mat->step + (submat->rows > 1 ? pix_size : 0);
    submat->tid = mat->tid;
    if( submat->rows > 1 )
        submat->tid &= ~CC_MAT_CONT_FLAG;
    else
        submat->tid |= CC_MAT_CONT_FLAG;
    submat->refcount = 0;
    submat->hdr_refcount = 0;
    res = submat;

    return res;
}


/****************************************************************************************\
*                      Operations on CScalar and accessing array elements               *
\****************************************************************************************/

// Returns pointer to specified element of array (linear index is used)
CC_IMPL  uchar*
cvPtr1D( const img_t* arr, int idx, int* _type )
{
    uchar* ptr = 0;
    if( CC_IS_MAT( arr ))
    {
        img_t* mat = (img_t*)arr;

        int type = CC_MAT_DEPTH(mat->tid);
        int pix_size = CC_ELEM_SIZE(type);

        if( _type )
            *_type = type;

        // the first part is mul-free sufficient check
        // that the index is within the matrix
        if( (unsigned)idx >= (unsigned)(mat->rows + mat->cols - 1) &&
            (unsigned)idx >= (unsigned)(mat->rows*mat->cols))
            CC_Error( CC_StsOutOfRange, "index is out of range" );

        if( CC_IS_MAT_CONT(mat->tid))
        {
            ptr = mat->data.ptr + (size_t)idx*pix_size;
        }
        else
        {
            int row, col;
            if( mat->cols == 1 )
                row = idx, col = 0;
            else
                row = idx/mat->cols, col = idx - row*mat->cols;
            ptr = mat->data.ptr + (size_t)row*mat->step + col*pix_size;
        }
    }
    else if( CC_IS_IMAGE_HDR( arr ))
    {
        img_t* img = (img_t*)arr;
        int width = !img->roi ? img->width : img->roi->width;
        int y = idx/width, x = idx - y*width;

        ptr = cvPtr2D( arr, y, x, _type );
    }
    else if( CC_IS_MATND( arr ))
    {
        CvMatND* mat = (CvMatND*)arr;
        int j, type = CC_MAT_DEPTH(mat->tid);
        size_t size = mat->dim[0].size;

        if( _type )
            *_type = type;

        for( j = 1; j < mat->dims; j++ )
            size *= mat->dim[j].size;

        if((unsigned)idx >= (unsigned)size )
            CC_Error( CC_StsOutOfRange, "index is out of range" );

        if( CC_IS_MAT_CONT(mat->tid))
        {
            int pix_size = CC_ELEM_SIZE(type);
            ptr = mat->data.ptr + (size_t)idx*pix_size;
        }
        else
        {
            ptr = mat->data.ptr;
            for( j = mat->dims - 1; j >= 0; j-- )
            {
                int sz = mat->dim[j].size;
                if( sz )
                {
                    int t = idx/sz;
                    ptr += (idx - t*sz)*mat->dim[j]->step;
                    idx = t;
                }
            }
        }
    }
    else if( CC_IS_SPARSE_MAT( arr ))
    {
        CvSparseMat* m = (CvSparseMat*)arr;
        if( m->dims == 1 )
            ptr = icvGetNodePtr( (CvSparseMat*)arr, &idx, _type, 1, 0 );
        else
        {
            int i, n = m->dims;
            CC_DbgAssert( n <= CC_MAX_DIM_HEAP );
            int _idx[CC_MAX_DIM_HEAP];

            for( i = n - 1; i >= 0; i-- )
            {
                int t = idx / m->size[i];
                _idx[i] = idx - t*m->size[i];
                idx = t;
            }
            ptr = icvGetNodePtr( (CvSparseMat*)arr, _idx, _type, 1, 0 );
        }
    }
    else
    {
        CC_Error( CC_StsBadArg, "unrecognized or unsupported array type" );
    }

    return ptr;
}


// Returns pointer to specified element of 2d array
CC_IMPL  uchar*
cvPtr2D( const img_t* arr, int y, int x, int* _type )
{
    uchar* ptr = 0;
    if( CC_IS_MAT( arr ))
    {
        img_t* mat = (img_t*)arr;
        int type;

        if( (unsigned)y >= (unsigned)(mat->rows) ||
            (unsigned)x >= (unsigned)(mat->cols) )
            CC_Error( CC_StsOutOfRange, "index is out of range" );

        type = CC_MAT_DEPTH(mat->tid);
        if( _type )
            *_type = type;

        ptr = mat->data.ptr + (size_t)y*mat->step + x*CC_ELEM_SIZE(type);
    }
    else if( CC_IS_IMAGE( arr ))
    {
        img_t* img = (img_t*)arr;
        int pix_size = (img->depth & 255) >> 3;
        int width, height;
        ptr = (uchar*)img->imageData;

        if( img->dataOrder == 0 )
            pix_size *= img->nChannels;

        if( img->roi )
        {
            width = img->roi->width;
            height = img->roi->height;

            ptr += img->roi->yOffset*img->widthStep +
                   img->roi->xOffset*pix_size;

            if( img->dataOrder )
            {
                int coi = img->roi->coi;
                if( !coi )
                    CC_Error( CC_BadCOI,
                        "COI must be non-null in case of planar images" );
                ptr += (coi - 1)*img->imageSize;
            }
        }
        else
        {
            width = img->width;
            height = img->height;
        }

        if( (unsigned)y >= (unsigned)height ||
            (unsigned)x >= (unsigned)width )
            CC_Error( CC_StsOutOfRange, "index is out of range" );

        ptr += y*img->widthStep + x*pix_size;

        if( _type )
        {
            int type = IPL2CC_DEPTH(img->depth);
            if( type < 0 || (unsigned)(img->nChannels - 1) > 3 )
                CC_Error( CC_StsUnsupportedFormat, "" );

            *_type = CC_MAKETYPE( type, img->nChannels );
        }
    }
    else if( CC_IS_MATND( arr ))
    {
        CvMatND* mat = (CvMatND*)arr;

        if( mat->dims != 2 ||
            (unsigned)y >= (unsigned)(mat->dim[0].size) ||
            (unsigned)x >= (unsigned)(mat->dim[1].size) )
            CC_Error( CC_StsOutOfRange, "index is out of range" );

        ptr = mat->data.ptr + (size_t)y*mat->dim[0]->step + x*mat->dim[1]->step;
        if( _type )
            *_type = CC_MAT_DEPTH(mat->tid);
    }
    else if( CC_IS_SPARSE_MAT( arr ))
    {
        int idx[] = { y, x };
        ptr = icvGetNodePtr( (CvSparseMat*)arr, idx, _type, 1, 0 );
    }
    else
    {
        CC_Error( CC_StsBadArg, "unrecognized or unsupported array type" );
    }

    return ptr;
}


// Returns pointer to specified element of 3d array
CC_IMPL  uchar*
cvPtr3D( const img_t* arr, int z, int y, int x, int* _type )
{
    uchar* ptr = 0;
    if( CC_IS_MATND( arr ))
    {
        CvMatND* mat = (CvMatND*)arr;

        if( mat->dims != 3 ||
            (unsigned)z >= (unsigned)(mat->dim[0].size) ||
            (unsigned)y >= (unsigned)(mat->dim[1].size) ||
            (unsigned)x >= (unsigned)(mat->dim[2].size) )
            CC_Error( CC_StsOutOfRange, "index is out of range" );

        ptr = mat->data.ptr + (size_t)z*mat->dim[0]->step +
              (size_t)y*mat->dim[1]->step + x*mat->dim[2]->step;

        if( _type )
            *_type = CC_MAT_DEPTH(mat->tid);
    }
    else if( CC_IS_SPARSE_MAT( arr ))
    {
        int idx[] = { z, y, x };
        ptr = icvGetNodePtr( (CvSparseMat*)arr, idx, _type, 1, 0 );
    }
    else
    {
        CC_Error( CC_StsBadArg, "unrecognized or unsupported array type" );
    }

    return ptr;
}


// Returns pointer to specified element of n-d array
CC_IMPL  uchar*
cvPtrND( const img_t* arr, const int* idx, int* _type,
         int create_node, unsigned* precalc_hashval )
{
    uchar* ptr = 0;
    if( !idx )
        CC_Error( CC_StsNullPtr, "NULL pointer to indices" );

    if( CC_IS_SPARSE_MAT( arr ))
        ptr = icvGetNodePtr( (CvSparseMat*)arr, idx,
                             _type, create_node, precalc_hashval );
    else if( CC_IS_MATND( arr ))
    {
        CvMatND* mat = (CvMatND*)arr;
        int i;
        ptr = mat->data.ptr;

        for( i = 0; i < mat->dims; i++ )
        {
            if( (unsigned)idx[i] >= (unsigned)(mat->dim[i].size) )
                CC_Error( CC_StsOutOfRange, "index is out of range" );
            ptr += (size_t)idx[i]*mat->dim[i]->step;
        }

        if( _type )
            *_type = CC_MAT_DEPTH(mat->tid);
    }
    else if( CC_IS_MAT_HDR(arr) || CC_IS_IMAGE_HDR(arr) )
        ptr = cvPtr2D( arr, idx[0], idx[1], _type );
    else
        CC_Error( CC_StsBadArg, "unrecognized or unsupported array type" );

    return ptr;
}


// Returns specifed element of n-D array given linear index
CC_IMPL  CScalar
cvGet1D( const img_t* arr, int idx )
{
    CScalar scalar(0);
    int type = 0;
    uchar* ptr;

    if( CC_IS_MAT( arr ) && CC_IS_MAT_CONT( ((img_t*)arr)->tid ))
    {
        img_t* mat = (img_t*)arr;

        type = CC_MAT_DEPTH(mat->tid);
        int pix_size = CC_ELEM_SIZE(type);

        // the first part is mul-free sufficient check
        // that the index is within the matrix
        if( (unsigned)idx >= (unsigned)(mat->rows + mat->cols - 1) &&
            (unsigned)idx >= (unsigned)(mat->rows*mat->cols))
            CC_Error( CC_StsOutOfRange, "index is out of range" );

        ptr = mat->data.ptr + (size_t)idx*pix_size;
    }
    else if( !CC_IS_SPARSE_MAT( arr ) || ((CvSparseMat*)arr)->dims > 1 )
        ptr = cvPtr1D( arr, idx, &type );
    else
        ptr = icvGetNodePtr( (CvSparseMat*)arr, &idx, &type, 0, 0 );

    if( ptr )
        cvRawDataToScalar( ptr, type, &scalar );

    return scalar;
}


// Returns specifed element of 2D array
CC_IMPL  CScalar
cvGet2D( const img_t* arr, int y, int x )
{
    CScalar scalar(0);
    int type = 0;
    uchar* ptr;

    if( CC_IS_MAT( arr ))
    {
        img_t* mat = (img_t*)arr;

        if( (unsigned)y >= (unsigned)(mat->rows) ||
            (unsigned)x >= (unsigned)(mat->cols) )
            CC_Error( CC_StsOutOfRange, "index is out of range" );

        type = CC_MAT_DEPTH(mat->tid);
        ptr = mat->data.ptr + (size_t)y*mat->step + x*CC_ELEM_SIZE(type);
    }
    else if( !CC_IS_SPARSE_MAT( arr ))
        ptr = cvPtr2D( arr, y, x, &type );
    else
    {
        int idx[] = { y, x };
        ptr = icvGetNodePtr( (CvSparseMat*)arr, idx, &type, 0, 0 );
    }

    if( ptr )
        cvRawDataToScalar( ptr, type, &scalar );

    return scalar;
}


// Returns specifed element of 3D array
CC_IMPL  CScalar
cvGet3D( const img_t* arr, int z, int y, int x )
{
    CScalar scalar(0);
    int type = 0;
    uchar* ptr;

    if( !CC_IS_SPARSE_MAT( arr ))
        ptr = cvPtr3D( arr, z, y, x, &type );
    else
    {
        int idx[] = { z, y, x };
        ptr = icvGetNodePtr( (CvSparseMat*)arr, idx, &type, 0, 0 );
    }

    if( ptr )
        cvRawDataToScalar( ptr, type, &scalar );
    return scalar;
}


// Returns specifed element of nD array
CC_IMPL  CScalar
cvGetND( const img_t* arr, const int* idx )
{
    CScalar scalar(0);
    int type = 0;
    uchar* ptr;

    if( !CC_IS_SPARSE_MAT( arr ))
        ptr = cvPtrND( arr, idx, &type );
    else
        ptr = icvGetNodePtr( (CvSparseMat*)arr, idx, &type, 0, 0 );

    if( ptr )
        cvRawDataToScalar( ptr, type, &scalar );

    return scalar;
}


// Returns specifed element of n-D array given linear index
CC_IMPL  double
cvGetReal1D( const img_t* arr, int idx )
{
    double value = 0;
    int type = 0;
    uchar* ptr;

    if( CC_IS_MAT( arr ) && CC_IS_MAT_CONT( ((img_t*)arr)->tid ))
    {
        img_t* mat = (img_t*)arr;

        type = CC_MAT_DEPTH(mat->tid);
        int pix_size = CC_ELEM_SIZE(type);

        // the first part is mul-free sufficient check
        // that the index is within the matrix
        if( (unsigned)idx >= (unsigned)(mat->rows + mat->cols - 1) &&
            (unsigned)idx >= (unsigned)(mat->rows*mat->cols))
            CC_Error( CC_StsOutOfRange, "index is out of range" );

        ptr = mat->data.ptr + (size_t)idx*pix_size;
    }
    else if( !CC_IS_SPARSE_MAT( arr ) || ((CvSparseMat*)arr)->dims > 1 )
        ptr = cvPtr1D( arr, idx, &type );
    else
        ptr = icvGetNodePtr( (CvSparseMat*)arr, &idx, &type, 0, 0 );

    if( ptr )
    {
        if( CC_MAT_CN( type ) > 1 )
            CC_Error( CC_BadNumChannels, "cvGetReal* support only single-channel arrays" );

        value = icvGetReal( ptr, type );
    }
    return value;
}


// Returns specifed element of 2D array
CC_IMPL  double
cvGetReal2D( const img_t* arr, int y, int x )
{
    double value = 0;
    int type = 0;
    uchar* ptr;

    if( CC_IS_MAT( arr ))
    {
        img_t* mat = (img_t*)arr;

        if( (unsigned)y >= (unsigned)(mat->rows) ||
            (unsigned)x >= (unsigned)(mat->cols) )
            CC_Error( CC_StsOutOfRange, "index is out of range" );

        type = CC_MAT_DEPTH(mat->tid);
        ptr = mat->data.ptr + (size_t)y*mat->step + x*CC_ELEM_SIZE(type);
    }
    else if( !CC_IS_SPARSE_MAT( arr ))
        ptr = cvPtr2D( arr, y, x, &type );
    else
    {
        int idx[] = { y, x };
        ptr = icvGetNodePtr( (CvSparseMat*)arr, idx, &type, 0, 0 );
    }

    if( ptr )
    {
        if( CC_MAT_CN( type ) > 1 )
            CC_Error( CC_BadNumChannels, "cvGetReal* support only single-channel arrays" );

        value = icvGetReal( ptr, type );
    }

    return value;
}


// Returns specifed element of 3D array
CC_IMPL  double
cvGetReal3D( const img_t* arr, int z, int y, int x )
{
    double value = 0;
    int type = 0;
    uchar* ptr;

    if( !CC_IS_SPARSE_MAT( arr ))
        ptr = cvPtr3D( arr, z, y, x, &type );
    else
    {
        int idx[] = { z, y, x };
        ptr = icvGetNodePtr( (CvSparseMat*)arr, idx, &type, 0, 0 );
    }

    if( ptr )
    {
        if( CC_MAT_CN( type ) > 1 )
            CC_Error( CC_BadNumChannels, "cvGetReal* support only single-channel arrays" );

        value = icvGetReal( ptr, type );
    }

    return value;
}


// Returns specifed element of nD array
CC_IMPL  double
cvGetRealND( const img_t* arr, const int* idx )
{
    double value = 0;
    int type = 0;
    uchar* ptr;

    if( !CC_IS_SPARSE_MAT( arr ))
        ptr = cvPtrND( arr, idx, &type );
    else
        ptr = icvGetNodePtr( (CvSparseMat*)arr, idx, &type, 0, 0 );

    if( ptr )
    {
        if( CC_MAT_CN( type ) > 1 )
            CC_Error( CC_BadNumChannels, "cvGetReal* support only single-channel arrays" );

        value = icvGetReal( ptr, type );
    }

    return value;
}


// Assigns new value to specifed element of nD array given linear index
CC_IMPL  void
cvSet1D( img_t* arr, int idx, CScalar scalar )
{
    int type = 0;
    uchar* ptr;

    if( CC_IS_MAT( arr ) && CC_IS_MAT_CONT( ((img_t*)arr)->tid ))
    {
        img_t* mat = (img_t*)arr;

        type = CC_MAT_DEPTH(mat->tid);
        int pix_size = CC_ELEM_SIZE(type);

        // the first part is mul-free sufficient check
        // that the index is within the matrix
        if( (unsigned)idx >= (unsigned)(mat->rows + mat->cols - 1) &&
            (unsigned)idx >= (unsigned)(mat->rows*mat->cols))
            CC_Error( CC_StsOutOfRange, "index is out of range" );

        ptr = mat->data.ptr + (size_t)idx*pix_size;
    }
    else if( !CC_IS_SPARSE_MAT( arr ) || ((CvSparseMat*)arr)->dims > 1 )
        ptr = cvPtr1D( arr, idx, &type );
    else
        ptr = icvGetNodePtr( (CvSparseMat*)arr, &idx, &type, -1, 0 );

    cvScalarToRawData( &scalar, ptr, type );
}


// Assigns new value to specifed element of 2D array
CC_IMPL  void
cvSet2D( img_t* arr, int y, int x, CScalar scalar )
{
    int type = 0;
    uchar* ptr;

    if( CC_IS_MAT( arr ))
    {
        img_t* mat = (img_t*)arr;

        if( (unsigned)y >= (unsigned)(mat->rows) ||
            (unsigned)x >= (unsigned)(mat->cols) )
            CC_Error( CC_StsOutOfRange, "index is out of range" );

        type = CC_MAT_DEPTH(mat->tid);
        ptr = mat->data.ptr + (size_t)y*mat->step + x*CC_ELEM_SIZE(type);
    }
    else if( !CC_IS_SPARSE_MAT( arr ))
        ptr = cvPtr2D( arr, y, x, &type );
    else
    {
        int idx[] = { y, x };
        ptr = icvGetNodePtr( (CvSparseMat*)arr, idx, &type, -1, 0 );
    }
    cvScalarToRawData( &scalar, ptr, type );
}


// Assigns new value to specifed element of 3D array
CC_IMPL  void
cvSet3D( img_t* arr, int z, int y, int x, CScalar scalar )
{
    int type = 0;
    uchar* ptr;

    if( !CC_IS_SPARSE_MAT( arr ))
        ptr = cvPtr3D( arr, z, y, x, &type );
    else
    {
        int idx[] = { z, y, x };
        ptr = icvGetNodePtr( (CvSparseMat*)arr, idx, &type, -1, 0 );
    }
    cvScalarToRawData( &scalar, ptr, type );
}


// Assigns new value to specifed element of nD array
CC_IMPL  void
cvSetND( img_t* arr, const int* idx, CScalar scalar )
{
    int type = 0;
    uchar* ptr;

    if( !CC_IS_SPARSE_MAT( arr ))
        ptr = cvPtrND( arr, idx, &type );
    else
        ptr = icvGetNodePtr( (CvSparseMat*)arr, idx, &type, -1, 0 );
    cvScalarToRawData( &scalar, ptr, type );
}


CC_IMPL  void
cvSetReal1D( img_t* arr, int idx, double value )
{
    int type = 0;
    uchar* ptr;

    if( CC_IS_MAT( arr ) && CC_IS_MAT_CONT( ((img_t*)arr)->tid ))
    {
        img_t* mat = (img_t*)arr;

        type = CC_MAT_DEPTH(mat->tid);
        int pix_size = CC_ELEM_SIZE(type);

        // the first part is mul-free sufficient check
        // that the index is within the matrix
        if( (unsigned)idx >= (unsigned)(mat->rows + mat->cols - 1) &&
            (unsigned)idx >= (unsigned)(mat->rows*mat->cols))
            CC_Error( CC_StsOutOfRange, "index is out of range" );

        ptr = mat->data.ptr + (size_t)idx*pix_size;
    }
    else if( !CC_IS_SPARSE_MAT( arr ) || ((CvSparseMat*)arr)->dims > 1 )
        ptr = cvPtr1D( arr, idx, &type );
    else
        ptr = icvGetNodePtr( (CvSparseMat*)arr, &idx, &type, -1, 0 );

    if( CC_MAT_CN( type ) > 1 )
        CC_Error( CC_BadNumChannels, "cvSetReal* support only single-channel arrays" );

    if( ptr )
        icvSetReal( value, ptr, type );
}


CC_IMPL  void
cvSetReal2D( img_t* arr, int y, int x, double value )
{
    int type = 0;
    uchar* ptr;

    if( CC_IS_MAT( arr ))
    {
        img_t* mat = (img_t*)arr;

        if( (unsigned)y >= (unsigned)(mat->rows) ||
            (unsigned)x >= (unsigned)(mat->cols) )
            CC_Error( CC_StsOutOfRange, "index is out of range" );

        type = CC_MAT_DEPTH(mat->tid);
        ptr = mat->data.ptr + (size_t)y*mat->step + x*CC_ELEM_SIZE(type);
    }
    else if( !CC_IS_SPARSE_MAT( arr ))
    {
        ptr = cvPtr2D( arr, y, x, &type );
    }
    else
    {
        int idx[] = { y, x };
        ptr = icvGetNodePtr( (CvSparseMat*)arr, idx, &type, -1, 0 );
    }
    if( CC_MAT_CN( type ) > 1 )
        CC_Error( CC_BadNumChannels, "cvSetReal* support only single-channel arrays" );

    if( ptr )
        icvSetReal( value, ptr, type );
}


CC_IMPL  void
cvSetReal3D( img_t* arr, int z, int y, int x, double value )
{
    int type = 0;
    uchar* ptr;

    if( !CC_IS_SPARSE_MAT( arr ))
        ptr = cvPtr3D( arr, z, y, x, &type );
    else
    {
        int idx[] = { z, y, x };
        ptr = icvGetNodePtr( (CvSparseMat*)arr, idx, &type, -1, 0 );
    }
    if( CC_MAT_CN( type ) > 1 )
        CC_Error( CC_BadNumChannels, "cvSetReal* support only single-channel arrays" );

    if( ptr )
        icvSetReal( value, ptr, type );
}


CC_IMPL  void
cvSetRealND( img_t* arr, const int* idx, double value )
{
    int type = 0;
    uchar* ptr;

    if( !CC_IS_SPARSE_MAT( arr ))
        ptr = cvPtrND( arr, idx, &type );
    else
        ptr = icvGetNodePtr( (CvSparseMat*)arr, idx, &type, -1, 0 );

    if( CC_MAT_CN( type ) > 1 )
        CC_Error( CC_BadNumChannels, "cvSetReal* support only single-channel arrays" );

    if( ptr )
        icvSetReal( value, ptr, type );
}


CC_IMPL void
cvClearND( img_t* arr, const int* idx )
{
    if( !CC_IS_SPARSE_MAT( arr ))
    {
        int type;
        uchar* ptr;
        ptr = cvPtrND( arr, idx, &type );
        if( ptr )
            memset( ptr, 0, CC_ELEM_SIZE(type) );
    }
    else
        icvDeleteNode( (CvSparseMat*)arr, idx, 0 );
}


/****************************************************************************************\
*                             Conversion to img_t or img_t                            *
\****************************************************************************************/

// convert array (img_t or img_t) to img_t
CC_IMPL img_t*
cvGetMat( const img_t* array, img_t* mat,
          int* pCOI, int allowND )
{
    img_t* result = 0;
    img_t* src = (img_t*)array;
    int coi = 0;

    if( !mat || !src )
        CC_Error( CC_StsNullPtr, "NULL array pointer is passed" );

    if( CC_IS_MAT_HDR(src))
    {
        if( !src->data.ptr )
            CC_Error( CC_StsNullPtr, "The matrix has NULL data pointer" );

        result = (img_t*)src;
    }
    else if( CC_IS_IMAGE_HDR(src) )
    {
        const img_t* img = (const img_t*)src;
        int depth, order;

        if( img->imageData == 0 )
            CC_Error( CC_StsNullPtr, "The image has NULL data pointer" );

        depth = IPL2CC_DEPTH( img->depth );
        if( depth < 0 )
            CC_Error( CC_BadDepth, "" );

        order = img->dataOrder & (img->nChannels > 1 ? -1 : 0);

        if( img->roi )
        {
            if( order == IMG_DATA_ORDER_PLANE )
            {
                int type = depth;

                if( img->roi->coi == 0 )
                    CC_Error( CC_StsBadFlag,
                    "Images with planar data layout should be used with COI selected" );

                cvInitMatHeader( mat, img->roi->height,
                                img->roi->width, type,
                                img->imageData + (img->roi->coi-1)*img->imageSize +
                                img->roi->yOffset*img->widthStep +
                                img->roi->xOffset*CC_ELEM_SIZE(type),
                                img->widthStep );
            }
            else /* pixel order */
            {
                int type = CC_MAKETYPE( depth, img->nChannels );
                coi = img->roi->coi;

                if( img->nChannels > CC_CN_MAX )
                    CC_Error( CC_BadNumChannels,
                        "The image is interleaved and has over CC_CN_MAX channels" );

                cvInitMatHeader( mat, img->roi->height, img->roi->width,
                                 type, img->imageData +
                                 img->roi->yOffset*img->widthStep +
                                 img->roi->xOffset*CC_ELEM_SIZE(type),
                                 img->widthStep );
            }
        }
        else
        {
            int type = CC_MAKETYPE( depth, img->nChannels );

            if( order != IMG_DATA_ORDER_PIXEL )
                CC_Error( CC_StsBadFlag, "Pixel order should be used with coi == 0" );

            cvInitMatHeader( mat, img->height, img->width, type,
                             img->imageData, img->widthStep );
        }

        result = mat;
    }
    else if( allowND && CC_IS_MATND_HDR(src) )
    {
        CvMatND* matnd = (CvMatND*)src;
        int size1 = matnd->dim[0].size, size2 = 1;

        if( !src->data.ptr )
            CC_Error( CC_StsNullPtr, "Input array has NULL data pointer" );

        if( !CC_IS_MAT_CONT( matnd->tid ))
            CC_Error( CC_StsBadArg, "Only continuous nD arrays are supported here" );

        if( matnd->dims > 2 )
        {
            int i;
            for( i = 1; i < matnd->dims; i++ )
                size2 *= matnd->dim[i].size;
        }
        else
            size2 = matnd->dims == 1 ? 1 : matnd->dim[1].size;

        mat->refcount = 0;
        mat->hdr_refcount = 0;
        mat->data.ptr = matnd->data.ptr;
        mat->rows = size1;
        mat->cols = size2;
        mat->tid = CC_MAT_DEPTH(matnd->tid) | CC_MAT_MAGIC_VAL | CC_MAT_CONT_FLAG;
        mat->step = size2*CC_ELEM_SIZE(matnd->tid);
        mat->step &= size1 > 1 ? -1 : 0;

        icvCheckHuge( mat );
        result = mat;
    }
    else
        CC_Error( CC_StsBadFlag, "Unrecognized or unsupported array type" );

    if( pCOI )
        *pCOI = coi;

    return result;
}


CC_IMPL img_t*
cvReshapeMatND( const img_t* arr,
                int sizeof_header, img_t* _header,
                int new_cn, int new_dims, int* new_sizes )
{
    img_t* result = 0;
    int dims, coi = 0;

    if( !arr || !_header )
        CC_Error( CC_StsNullPtr, "NULL pointer to array or destination header" );

    if( new_cn == 0 && new_dims == 0 )
        CC_Error( CC_StsBadArg, "None of array parameters is changed: dummy call?" );

    dims = cvGetDims( arr );

    if( new_dims == 0 )
    {
        new_sizes = 0;
        new_dims = dims;
    }
    else if( new_dims == 1 )
    {
        new_sizes = 0;
    }
    else
    {
        if( new_dims <= 0 || new_dims > CC_MAX_DIM )
            CC_Error( CC_StsOutOfRange, "Non-positive or too large number of dimensions" );
        if( !new_sizes )
            CC_Error( CC_StsNullPtr, "New dimension sizes are not specified" );
    }

    if( new_dims <= 2 )
    {
        img_t* mat = (img_t*)arr;
        img_t header;
        int* refcount = 0;
        int  hdr_refcount = 0;
        int  total_width, new_rows, cn;

        if( sizeof_header != sizeof(img_t) && sizeof_header != sizeof(CvMatND) )
            CC_Error( CC_StsBadArg, "The output header should be img_t or CvMatND" );

        if( mat == (img_t*)_header )
        {
            refcount = mat->refcount;
            hdr_refcount = mat->hdr_refcount;
        }

        if( !CC_IS_MAT( mat ))
            mat = cvGetMat( mat, &header, &coi, 1 );

        cn = CC_MAT_CN( mat->tid );
        total_width = mat->cols * cn;

        if( new_cn == 0 )
            new_cn = cn;

        if( new_sizes )
            new_rows = new_sizes[0];
        else if( new_dims == 1 )
            new_rows = total_width*mat->rows/new_cn;
        else
        {
            new_rows = mat->rows;
            if( new_cn > total_width )
                new_rows = mat->rows * total_width / new_cn;
        }

        if( new_rows != mat->rows )
        {
            int total_size = total_width * mat->rows;

            if( !CC_IS_MAT_CONT( mat->tid ))
                CC_Error( CC_BadStep,
                "The matrix is not continuous so the number of rows can not be changed" );

            total_width = total_size / new_rows;

            if( total_width * new_rows != total_size )
                CC_Error( CC_StsBadArg, "The total number of matrix elements "
                                        "is not divisible by the new number of rows" );
        }

        header->rows = new_rows;
        header->cols = total_width / new_cn;

        if( header->cols * new_cn != total_width ||
            (new_sizes && header->cols != new_sizes[1]) )
            CC_Error( CC_StsBadArg, "The total matrix width is not "
                            "divisible by the new number of columns" );

        header.type = (mat->tid & ~CC_MAT_TYPE_MASK) | CC_MAKETYPE(mat->tid, new_cn);
        header->step = header->cols * CC_ELEM_SIZE(mat->tid);
        header->step &= new_rows > 1 ? -1 : 0;
        header.refcount = refcount;
        header.hdr_refcount = hdr_refcount;

        if( sizeof_header == sizeof(img_t) )
            *(img_t*)_header = header;
        else
        {
            CvMatND* __header = (CvMatND*)_header;
            cvGetMatND(&header, __header, 0);
            if( new_dims > 0 )
                __header->dims = new_dims;
        }
    }
    else
    {
        CvMatND* header = (CvMatND*)_header;

        if( sizeof_header != sizeof(CvMatND))
            CC_Error( CC_StsBadSize, "The output header should be CvMatND" );

        if( !new_sizes )
        {
            if( !CC_IS_MATND( arr ))
                CC_Error( CC_StsBadArg, "The input array must be CvMatND" );

            {
            CvMatND* mat = (CvMatND*)arr;
            assert( new_cn > 0 );
            int last_dim_size = mat->dim[mat->dims-1].size*CC_MAT_CN(mat->tid);
            int new_size = last_dim_size/new_cn;

            if( new_size*new_cn != last_dim_size )
                CC_Error( CC_StsBadArg,
                "The last dimension full size is not divisible by new number of channels");

            if( mat != header )
            {
                memcpy( header, mat, sizeof(*header));
                header->refcount = 0;
                header->hdr_refcount = 0;
            }

            header->dim[header->dims-1].size = new_size;
            header->tid = (header->tid & ~CC_MAT_TYPE_MASK) | CC_MAKETYPE(header->tid, new_cn);
            }
        }
        else
        {
            CvMatND stub;
            CvMatND* mat = (CvMatND*)arr;
            int i, size1, size2;
            int step;

            if( new_cn != 0 )
                CC_Error( CC_StsBadArg,
                "Simultaneous change of shape and number of channels is not supported. "
                "Do it by 2 separate calls" );

            if( !CC_IS_MATND( mat ))
            {
                cvGetMatND( mat, &stub, &coi );
                mat = &stub;
            }

            if( CC_IS_MAT_CONT( mat->tid ))
                CC_Error( CC_StsBadArg, "Non-continuous nD arrays are not supported" );

            size1 = mat->dim[0].size;
            for( i = 1; i < dims; i++ )
                size1 *= mat->dim[i].size;

            size2 = 1;
            for( i = 0; i < new_dims; i++ )
            {
                if( new_sizes[i] <= 0 )
                    CC_Error( CC_StsBadSize,
                    "One of new dimension sizes is non-positive" );
                size2 *= new_sizes[i];
            }

            if( size1 != size2 )
                CC_Error( CC_StsBadSize,
                "Number of elements in the original and reshaped array is different" );

            if( header != mat )
            {
                header->refcount = 0;
                header->hdr_refcount = 0;
            }

            header->dims = new_dims;
            header->tid = mat->tid;
            header->data.ptr = mat->data.ptr;
            step = CC_ELEM_SIZE(header->tid);

            for( i = new_dims - 1; i >= 0; i-- )
            {
                header->dim[i].size = new_sizes[i];
                header->dim[i]->step = step;
                step *= new_sizes[i];
            }
        }
    }

    if( coi )
        CC_Error( CC_BadCOI, "COI is not supported by this operation" );

    result = _header;
    return result;
}


CC_IMPL img_t*
cvReshape( const img_t* array, img_t* header,
           int new_cn, int new_rows )
{
    img_t* result = 0;
    img_t *mat = (img_t*)array;
    int total_width, new_width;

    if( !header )
        CC_Error( CC_StsNullPtr, "" );

    if( !CC_IS_MAT( mat ))
    {
        int coi = 0;
        mat = cvGetMat( mat, header, &coi, 1 );
        if( coi )
            CC_Error( CC_BadCOI, "COI is not supported" );
    }

    if( new_cn == 0 )
        new_cn = CC_MAT_CN(mat->tid);
    else if( (unsigned)(new_cn - 1) > 3 )
        CC_Error( CC_BadNumChannels, "" );

    if( mat != header )
    {
        int hdr_refcount = header->hdr_refcount;
        *header = *mat;
        header->refcount = 0;
        header->hdr_refcount = hdr_refcount;
    }

    total_width = mat->cols * CC_MAT_CN( mat->tid );

    if( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = mat->rows * total_width / new_cn;

    if( new_rows == 0 || new_rows == mat->rows )
    {
        header->rows = mat->rows;
        header->step = mat->step;
    }
    else
    {
        int total_size = total_width * mat->rows;
        if( !CC_IS_MAT_CONT( mat->tid ))
            CC_Error( CC_BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        if( (unsigned)new_rows > (unsigned)total_size )
            CC_Error( CC_StsOutOfRange, "Bad new number of rows" );

        total_width = total_size / new_rows;

        if( total_width * new_rows != total_size )
            CC_Error( CC_StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        header->rows = new_rows;
        header->step = total_width * CC_ELEM_SIZE1(mat->tid);
    }

    new_width = total_width / new_cn;

    if( new_width * new_cn != total_width )
        CC_Error( CC_BadNumChannels,
        "The total width is not divisible by the new number of channels" );

    header->cols = new_width;
    header->tid = (mat->tid  & ~CC_MAT_TYPE_MASK) | CC_MAKETYPE(mat->tid, new_cn);

    result = header;
    return  result;
}


// convert array (img_t or img_t) to img_t
CC_IMPL img_t*
cvGetImage( const img_t* array, img_t* img )
{
    img_t* result = 0;
    const img_t* src = (const img_t*)array;

    if( !img )
        CC_Error( CC_StsNullPtr, "" );

    if( !CC_IS_IMAGE_HDR(src) )
    {
        const img_t* mat = (const img_t*)src;

        if( !CC_IS_MAT_HDR(mat))
            CC_Error( CC_StsBadFlag, "" );

        if( mat->data.ptr == 0 )
            CC_Error( CC_StsNullPtr, "" );

        int depth = cvIplDepth(mat->tid);

        cvInitImageHeader( img, cSize(mat->cols, mat->rows),
                           depth, CC_MAT_CN(mat->tid) );
        cvSetData( img, mat->data.ptr, mat->step );

        result = img;
    }
    else
    {
        result = (img_t*)src;
    }

    return result;
}


/****************************************************************************************\
*                               img_t-specific functions                              *
\****************************************************************************************/

static IplROI* icvCreateROI( int coi, int xOffset, int yOffset, int width, int height )
{
    IplROI *roi = 0;
    if( !CvIPL.createROI )
    {
        roi = (IplROI*)cAlloc( sizeof(*roi));

        roi->coi = coi;
        roi->xOffset = xOffset;
        roi->yOffset = yOffset;
        roi->width = width;
        roi->height = height;
    }
    else
    {
        roi = CvIPL.createROI( coi, xOffset, yOffset, width, height );
    }

    return roi;
}

static  void
icvGetColorModel( int nchannels, const char** colorModel, const char** channelSeq )
{
    static const char* tab[][2] =
    {
        {"GRAY", "GRAY"},
        {"",""},
        {"RGB","BGR"},
        {"RGB","BGRA"}
    };

    nchannels--;
    *colorModel = *channelSeq = "";

    if( (unsigned)nchannels <= 3 )
    {
        *colorModel = tab[nchannels][0];
        *channelSeq = tab[nchannels][1];
    }
}


// create img_t header
CC_IMPL img_t *
cvCreateImageHeader( CSize size, int depth, int channels )
{
    img_t *img = 0;

    if( !CvIPL.createHeader )
    {
        img = (img_t *)cAlloc( sizeof( *img ));
        cvInitImageHeader( img, size, depth, channels, IMG_ORIGIN_TL,
                                    CC_DEFAULT_IMAGE_ROW_ALIGN );
    }
    else
    {
        const char *colorModel, *channelSeq;

        icvGetColorModel( channels, &colorModel, &channelSeq );

        img = CvIPL.createHeader( channels, 0, depth, (char*)colorModel, (char*)channelSeq,
                                  IMG_DATA_ORDER_PIXEL, IMG_ORIGIN_TL,
                                  CC_DEFAULT_IMAGE_ROW_ALIGN,
                                  size.width, size.height, 0, 0, 0, 0 );
    }

    return img;
}


// create img_t header and allocate underlying data
CC_IMPL img_t *
cvCreateImage( CSize size, int depth, int channels )
{
    img_t *img = cvCreateImageHeader( size, depth, channels );
    assert( img );
    cvCreateData( img );

    return img;
}


// initialize img_t header, allocated by the user
CC_IMPL img_t*
cvInitImageHeader( img_t * image, CSize size, int depth,
                   int channels, int origin, int align )
{
    const char *colorModel, *channelSeq;

    if( !image )
        CC_Error( CC_HeaderIsNull, "null pointer to header" );

    memset( image, 0, sizeof( *image ));
    image->nSize = sizeof( *image );

    icvGetColorModel( channels, &colorModel, &channelSeq );
    strncpy( image->colorModel, colorModel, 4 );
    strncpy( image->channelSeq, channelSeq, 4 );

    if( size.width < 0 || size.height < 0 )
        CC_Error( CC_BadROISize, "Bad input roi" );

    if( (depth != (int)IMG_DEPTH_1U && depth != (int)IMG_DEPTH_8U &&
         depth != (int)IMG_DEPTH_8S && depth != (int)IMG_DEPTH_16U &&
         depth != (int)IMG_DEPTH_16S && depth != (int)IMG_DEPTH_32S &&
         depth != (int)IMG_DEPTH_32F && depth != (int)IMG_DEPTH_64F) ||
         channels < 0 )
        CC_Error( CC_BadDepth, "Unsupported format" );
    if( origin != CC_ORIGIN_BL && origin != CC_ORIGIN_TL )
        CC_Error( CC_BadOrigin, "Bad input origin" );

    if( align != 4 && align != 8 )
        CC_Error( CC_BadAlign, "Bad input align" );

    image->width = size.width;
    image->height = size.height;

    if( image->roi )
    {
        image->roi->coi = 0;
        image->roi->xOffset = image->roi->yOffset = 0;
        image->roi->width = size.width;
        image->roi->height = size.height;
    }

    image->nChannels = MAX( channels, 1 );
    image->depth = depth;
    image->align = align;
    image->widthStep = (((image->width * image->nChannels *
         (image->depth & ~IMG_DEPTH_SIGN) + 7)/8)+ align - 1) & (~(align - 1));
    image->origin = origin;
    const int64 imageSize_tmp = (int64)image->widthStep*(int64)image->height;
    image->imageSize = (int)imageSize_tmp;
    if( (int64)image->imageSize != imageSize_tmp )
        CC_Error( CC_StsNoMem, "Overflow for imageSize" );

    return image;
}


CC_IMPL void
cvReleaseImageHeader( img_t** image )
{
    if( !image )
        CC_Error( CC_StsNullPtr, "" );

    if( *image )
    {
        img_t* img = *image;
        *image = 0;

        if( !CvIPL.deallocate )
        {
            cFree( &img->roi );
            cFree( &img );
        }
        else
        {
            CvIPL.deallocate( img, IMG_IMAGE_HEADER | IMG_IMAGE_ROI );
        }
    }
}


CC_IMPL void
cvReleaseImage( img_t ** image )
{
    if( !image )
        CC_Error( CC_StsNullPtr, "" );

    if( *image )
    {
        img_t* img = *image;
        *image = 0;

        cvReleaseData( img );
        cvReleaseImageHeader( &img );
    }
}


CC_IMPL void
cvSetImageROI( img_t* image, CRect rect )
{
    if( !image )
        CC_Error( CC_HeaderIsNull, "" );

    // allow zero ROI width or height
    CC_Assert( rect.width >= 0 && rect.height >= 0 &&
               rect.x < image->width && rect.y < image->height &&
               rect.x + rect.width >= (int)(rect.width > 0) &&
               rect.y + rect.height >= (int)(rect.height > 0) );

    rect.width += rect.x;
    rect.height += rect.y;

    rect.x = MAX(rect.x, 0);
    rect.y = MAX(rect.y, 0);
    rect.width = MIN(rect.width, image->width);
    rect.height = MIN(rect.height, image->height);

    rect.width -= rect.x;
    rect.height -= rect.y;

    if( image->roi )
    {
        image->roi->xOffset = rect.x;
        image->roi->yOffset = rect.y;
        image->roi->width = rect.width;
        image->roi->height = rect.height;
    }
    else
        image->roi = icvCreateROI( 0, rect.x, rect.y, rect.width, rect.height );
}


CC_IMPL void
cvResetImageROI( img_t* image )
{
    if( !image )
        CC_Error( CC_HeaderIsNull, "" );

    if( image->roi )
    {
        if( !CvIPL.deallocate )
        {
            cFree( &image->roi );
        }
        else
        {
            CvIPL.deallocate( image, IMG_IMAGE_ROI );
            image->roi = 0;
        }
    }
}


CC_IMPL CRect
cvGetImageROI( const img_t* img )
{
    CRect rect;
    if( !img )
        CC_Error( CC_StsNullPtr, "Null pointer to image" );

    if( img->roi )
        rect = cRect( img->roi->xOffset, img->roi->yOffset,
                       img->roi->width, img->roi->height );
    else
        rect = cRect( 0, 0, img->width, img->height );

    return rect;
}


CC_IMPL void
cvSetImageCOI( img_t* image, int coi )
{
    if( !image )
        CC_Error( CC_HeaderIsNull, "" );

    if( (unsigned)coi > (unsigned)(image->nChannels) )
        CC_Error( CC_BadCOI, "" );

    if( image->roi || coi != 0 )
    {
        if( image->roi )
        {
            image->roi->coi = coi;
        }
        else
        {
            image->roi = icvCreateROI( coi, 0, 0, image->width, image->height );
        }
    }
}


CC_IMPL int
cvGetImageCOI( const img_t* image )
{
    if( !image )
        CC_Error( CC_HeaderIsNull, "" );

    return image->roi ? image->roi->coi : 0;
}


CC_IMPL img_t*
cvCloneImage( const img_t* src )
{
    img_t* dst = 0;

    if( !CC_IS_IMAGE_HDR( src ))
        CC_Error( CC_StsBadArg, "Bad image header" );

    if( !CvIPL.cloneImage )
    {
        dst = (img_t*)cAlloc( sizeof(*dst));

        memcpy( dst, src, sizeof(*src));
        dst->imageData = dst->imageDataOrigin = 0;
        dst->roi = 0;

        if( src->roi )
        {
            dst->roi = icvCreateROI( src->roi->coi, src->roi->xOffset,
                          src->roi->yOffset, src->roi->width, src->roi->height );
        }

        if( src->imageData )
        {
            int size = src->imageSize;
            cvCreateData( dst );
            memcpy( dst->imageData, src->imageData, size );
        }
    }
    else
        dst = CvIPL.cloneImage( src );

    return dst;
}


/****************************************************************************************\
*                            Additional operations on CTermCriteria                     *
\****************************************************************************************/

CC_IMPL CTermCriteria
cvCheckTermCriteria( CTermCriteria criteria, double default_eps,
                     int default_max_iters )
{
    CTermCriteria crit;

    crit.type = CC_TERMCRIT_ITER|CC_TERMCRIT_EPS;
    crit.max_iter = default_max_iters;
    crit.epsilon = (float)default_eps;

    if( (criteria.type & ~(CC_TERMCRIT_EPS | CC_TERMCRIT_ITER)) != 0 )
        CC_Error( CC_StsBadArg,
                  "Unknown type of term criteria" );

    if( (criteria.type & CC_TERMCRIT_ITER) != 0 )
    {
        if( criteria.max_iter <= 0 )
            CC_Error( CC_StsBadArg,
                  "Iterations flag is set and maximum number of iterations is <= 0" );
        crit.max_iter = criteria.max_iter;
    }

    if( (criteria.type & CC_TERMCRIT_EPS) != 0 )
    {
        if( criteria.epsilon < 0 )
            CC_Error( CC_StsBadArg, "Accuracy flag is set and epsilon is < 0" );

        crit.epsilon = criteria.epsilon;
    }

    if( (criteria.type & (CC_TERMCRIT_EPS | CC_TERMCRIT_ITER)) == 0 )
        CC_Error( CC_StsBadArg,
                  "Neither accuracy nor maximum iterations "
                  "number flags are set in criteria type" );

    crit.epsilon = (float)MAX( 0, crit.epsilon );
    crit.max_iter = MAX( 1, crit.max_iter );

    return crit;
}

namespace cv
{

template<> void DefaultDeleter<img_t>::operator ()(img_t* obj) const
{ cvReleaseMat(&obj); }

template<> void DefaultDeleter<img_t>::operator ()(img_t* obj) const
{ cvReleaseImage(&obj); }

template<> void DefaultDeleter<CvMatND>::operator ()(CvMatND* obj) const
{ cvReleaseMatND(&obj); }

template<> void DefaultDeleter<CvSparseMat>::operator ()(CvSparseMat* obj) const
{ cvReleaseSparseMat(&obj); }

template<> void DefaultDeleter<CvMemStorage>::operator ()(CvMemStorage* obj) const
{ cvReleaseMemStorage(&obj); }

template<> void DefaultDeleter<CvFileStorage>::operator ()(CvFileStorage* obj) const
{ cvReleaseFileStorage(&obj); }

}


