
/* ////////////////////////////////////////////////////////////////////
//
//  img_t, CvMatND, CvSparceMat and img_t support functions
//  (creation, deletion, copying, retrieving and setting elements etc.)
//
// */

CC_INLINE  CvDataType icvDepthToDataType(TypeId type)
{
    return (CvDataType)(
            ((((int)cv8u)|((int)cv8s << 4)|((int)cv16u << 8)|
              ((int)cv16s << 12)|((int)cv32s << 16)|((int)cv32f << 20)|
              ((int)cv64f << 24)) >> type*4) & 15);
}

/** @brief Returns the particular element of single-channel floating-point matrix.

The function is a fast replacement for cvGetReal2D in the case of single-channel floating-point
matrices. It is faster because it is inline, it does fewer checks for array type and array element
type, and it checks for the row and column ranges only in debug mode.
@param mat Input matrix
@param row The zero-based index of row
@param col The zero-based index of column
 */
CC_INLINE  double  cvmGet(const img_t* mat, int row, int col)
{
    TypeId type;
    int cn = CC_MAT_CN(mat);

    type = CC_MAT_DEPTH(mat);
    assert((unsigned)row < (unsigned)mat->rows &&
            (unsigned)col < (unsigned)mat->cols);

    if(type == CC_32F && cn==1)
        return ((float*)(void*)(mat->tt.data + (size_t)mat->step*row))[col];
    else
    {
        assert(type == CC_64F && 1==cn);
        return ((double*)(void*)(mat->tt.data + (size_t)mat->step*row))[col];
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
CC_INLINE  void  cvmSet(img_t* mat, int row, int col, double value)
{
    int cn = CC_MAT_CN(mat);
    TypeId type = CC_MAT_DEPTH(mat);
    assert((unsigned)row < (unsigned)mat->rows &&
            (unsigned)col < (unsigned)mat->cols);

    if(type == CC_32F && cn==1)
        ((float*)(void*)(mat->tt.data + (size_t)mat->step*row))[col] = (float)value;
    else
    {
        assert(type == CC_64F && 1==cn);
        ((double*)(void*)(mat->tt.data + (size_t)mat->step*row))[col] = value;
    }
}

static void icvCheckHuge(img_t* mat)
{
  if((int64)mat->step*mat->rows > INT_MAX) {
        ASSERT(0);
  }
}

// Initializes img_t header, allocated by the user
CC_IMPL img_t*
cvInitMatHeader(img_t* mat, int rows, int cols, TypeId type,
                int cn CC_DEFAULT(1), const void* data CC_DEFAULT(NULL), int step CC_DEFAULT(CC_AUTOSTEP))
{
    CC_FUNCNAME("cvInitMatHeader");
    
    __BEGIN__;

    if(!mat)
        CC_ERROR_FROM_CODE(CC_StsNullPtr);

    if(rows <= 0 || cols <= 0)
        CC_ERROR(CC_StsBadSize, "Non-positive cols or rows");
 
    assert((type) <= CC_64F);
    memset(mat, 0, sizeof(*mat));
    mat->f = 1;
    mat->t = type;
    mat->c = cn * CC_ELEM_SIZE(type);
    mat->cols = cols;
    mat->rows = rows;
    mat->step = rows > 1 ? mat->cols*mat->c : 0;
    mat->tt.data = (uchar*)data;
    if (step!=CC_AUTOSTEP) {
      mat->step = MAX(mat->step, step);
    }
    icvCheckHuge(mat);

    __END__;

    return mat;
}

#define CVMATDEF(name, rows, cols, type, cn, data)  img_t _##name, *name = cvMat(&_##name, rows, cols, type, cn, data)
CC_INLINE img_t* cvMat(img_t* mat, int rows, int cols, TypeId type, int cn CC_DEFAULT(1), const void* data CC_DEFAULT(NULL), int step CC_DEFAULT(CC_AUTOSTEP))
{
    assert((type) <= CC_64F);
    cvInitMatHeader(mat, rows, cols, type, cn, data, step);
    return mat;
}
CC_INLINE img_t* cvMat(img_t* mat, int rows, int cols, TypeId type, const void* data, int step CC_DEFAULT(CC_AUTOSTEP))
{
    assert((type) <= CC_64F);
    cvInitMatHeader(mat, rows, cols, type, 1, data, step);
    return mat;
}
CC_INLINE img_t cvMat(int rows, int cols, TypeId type, int cn CC_DEFAULT(1), const void* data CC_DEFAULT(NULL), int step CC_DEFAULT(CC_AUTOSTEP))
{
	img_t mat[1] = { 0 };
	assert((type) <= CC_64F);
	cvInitMatHeader(mat, rows, cols, type, cn, data, step);
	return *mat;
}
CC_INLINE img_t cvMat(int rows, int cols, TypeId type, const void* data , int step CC_DEFAULT(CC_AUTOSTEP))
{
	img_t mat[1] = { 0 };
	assert((type) <= CC_64F);
	cvInitMatHeader(mat, rows, cols, type, 1, data, step);
	return *mat;
}
CC_INLINE img_t cvMat1(int rows, int cols, TypeId type, int cn CC_DEFAULT(1), const void* data CC_DEFAULT(NULL), int step CC_DEFAULT(CC_AUTOSTEP))
{
	img_t mat[1] = { 0 };
	assert((type) <= CC_64F);
	cvInitMatHeader(mat, rows, cols, type, cn, data, step);
	return *mat;
}

CC_IMPL img_t*
cvSetMat(img_t* mat, int row, int col, TypeId type, int cn CC_DEFAULT(1)) {
  int pix_size = CC_ELEM_SIZE(type);
  imsetsize(mat, row, col, cn*pix_size, 1);
  mat->t = type;
  return mat;
}
CC_IMPL
img_t* cvSetMat(img_t* mat, int row, int col, TypeCnId typecn) {
    return cvSetMat(mat, row, col, CC_TYPECN_TYPE(typecn), CC_TYPECN_CN(typecn));
}
CC_IMPL
img_t* cvSetMat(img_t* mat, CSize size, TypeCnId typecn) {
    return cvSetMat(mat, size.h, size.w, CC_TYPECN_TYPE(typecn), CC_TYPECN_CN(typecn));
}
CC_IMPL
img_t* cvSetMat(img_t* mat, CSize size, TypeId type, int cn) {
    return cvSetMat(mat, size.h, size.w, type, cn);
}
CC_IMPL img_t*
cvSetSize(img_t* mat, int row, int col) {
  return cvSetMat(mat, row, col, mat->tid, CC_MAT_CN(mat));
}
// Creates img_t and underlying data
CC_IMPL img_t* cvCreateMat(img_t* mat, int height, int width, TypeId type, int cn CC_DEFAULT(1))
{
    CC_FUNCNAME("cvCreateMat");
    
    __BEGIN__;

    int pix_size = CC_ELEM_SIZE(type);
    imsetsize(mat, height, width, cn*pix_size, 1);
    mat->t = type;

    __END__;

    return mat;
}

/****************************************************************************************\
*                               img_t creation and basic operations                      *
\****************************************************************************************/

// Creates img_t header only
CC_IMPL img_t*
cvCreateMatHeader(img_t* mat, int rows, int cols, TypeId type, int cn)
{
    CC_FUNCNAME("cvCreateMatHeader");

    __BEGIN__;

    int min_step;

    if(rows <= 0 || cols <= 0)
        CC_ERROR(CC_StsBadSize, "Non-positive width or height");

    min_step = CC_ELEM_SIZE(type)*cols;
    if(min_step <= 0)
        CC_ERROR(CC_StsUnsupportedFormat, "Invalid matrix type");

    mat->step = rows == 1 ? 0 : cvAlign(min_step, CC_DEFAULT_MAT_ROW_ALIGN);
    mat->tid = type;
    mat->rows = rows;
    mat->cols = cols;
    mat->tt.data = 0;

    icvCheckHuge(mat);

    __END__;

    return mat;
}


// Deallocates the img_t structure and underlying data
CC_IMPL void
cvReleaseMat1(img_t* array)
{
    CC_FUNCNAME("cvReleaseMat");
    
    __BEGIN__;

    imfree(array);

    __END__;
}

// convert array (img_t or img_t) to img_t
CC_IMPL img_t*
cvGetMat(const img_t* src, img_t* mat, int* pCOI CC_DEFAULT(NULL), int allowND CC_DEFAULT(0))
{
    img_t* result = NULL;
    int coi = 0;
    
    CC_FUNCNAME("cvGetMat");

    __BEGIN__;

    if (pCOI) {
      *pCOI=0;
    }
    if (mat) {
        *mat = *src;
        result = (img_t*)mat;
    }

    __END__;

    if(pCOI)
        *pCOI = coi;

    return result;
}

/****************************************************************************************\
*                          Common for multiple array types operations                    *
\****************************************************************************************/

// Allocates underlying array data
CC_IMPL void
cvCreateData(img_t* mat)
{
    CC_FUNCNAME("cvCreateData");
    
    __BEGIN__;
    
    cvCreateMat(mat, mat->h, mat->w, mat->tid, CC_MAT_CN(mat));

    __END__;
}

// Assigns external data to array
CC_IMPL void
cvSetData(img_t* mat, void* data, int step)
{
    CC_FUNCNAME("cvSetData");

    __BEGIN__;

    __END__;
}

// Deallocates array's data
CC_IMPL void
cvReleaseData(img_t* mat)
{
    CC_FUNCNAME("cvReleaseData");
    
    __BEGIN__;

    __END__;
}

// Creates a copy of matrix
CC_IMPL img_t*
cvCloneMat(img_t* dst, const img_t* src)
{
    CC_FUNCNAME("cvCloneMat");

    __BEGIN__;

	if (src != dst)
	{
		cvCreateMat(dst, src->h, src->w, src->tid, CC_MAT_CN(src));
		//CC_CALL(dst = cvCreateMatHeader(dst, src->rows, src->cols, src->tid));

		if (src->tt.data)
		{
			//CC_CALL(cvCreateData(dst));
			//CC_CALL(cvCopy(src, dst));
			memcpy2d(dst->tt.data, dst->s, src->tt.data, src->s, src->h, dst->s);
		}
	}

    __END__;

    return dst;
}

CC_IMPL
img_t* cvSetMat(img_t* mat, int row, int col, TypeId type, int cn, void* data, int step = CC_AUTOSTEP) {
    img_t im[1] = {0};
    cvInitMatHeader(im, row, col, type, cn, data, step);
    return cvCloneMat(mat, im);
}
CC_IMPL
img_t* cvSetMatFromMat(img_t* mat, const img_t* mat2) {
    return cvSetMat(mat, mat2->h, mat2->w, mat2->tid, CC_MAT_CN(mat2));
}


// Retrieves essential information about image ROI or img_t data
CC_IMPL void
cvGetRawData(const img_t* mat, uchar** data, int* step, CSize* roi_size)
{
    CC_FUNCNAME("cvGetRawData");

    __BEGIN__;

    if(step)
        *step = mat->step;

    if(data)
        *data = mat->tt.data;

    if(roi_size)
        *roi_size = cvGetMatSize(mat);

    __END__;
}

CC_IMPL TypeId
cvGetElemType(const img_t* mat)
{
    TypeId type;

    CC_FUNCNAME("cvGetElemType");

    __BEGIN__;

    type = mat->tid;

    __END__;

    return type;
}


// Returns a number of array dimensions
CC_IMPL int
cvGetDims(const img_t* mat, int* sizes CC_DEFAULT(NULL))
{
    int dims = -1;
    CC_FUNCNAME("cvGetDims");

    __BEGIN__;

    dims = 2;
    if(sizes)
    {
        sizes[0] = mat->rows;
        sizes[1] = mat->cols;
    }


    __END__;

    return dims;
}


// Returns the size of particular array dimension
CC_IMPL int
cvGetDimSize(const img_t* mat, int index)
{
    int size = -1;
    CC_FUNCNAME("cvGetDimSize");

    __BEGIN__;

    switch(index)
    {
    case 0:
        size = mat->rows;
        break;
    case 1:
        size = mat->cols;
        break;
    case 2:
        size = CC_MAT_CN(mat);
        break;
    default:
        CC_ERROR(CC_StsOutOfRange, "bad dimension index");
    }
    
    __END__;

    return size;
}
// Selects sub-array (no data is copied)
CC_IMPL  img_t*
cvGetSubRect(const img_t* mat, img_t* submat, CRect rect)
{
    img_t* res = 0;
    
    CC_FUNCNAME("cvGetRect");

    __BEGIN__;

    if(!submat)
        CC_ERROR(CC_StsNullPtr, "");

    if((rect.x|rect.y|rect.width|rect.height) < 0)
        CC_ERROR(CC_StsBadSize, "");

    if(rect.x + rect.width > mat->cols ||
        rect.y + rect.height > mat->rows)
        CC_ERROR(CC_StsBadSize, "");

    *submat = *mat;
    submat->tt.data = mat->tt.data + (size_t)rect.y*mat->step + rect.x*mat->c;
    submat->step = mat->step & (rect.height > 1 ? -1 : 0);
    submat->tid = mat->tid;
    submat->rows = rect.height;
    submat->cols = rect.width;

    res = submat;
    
    __END__;

    return res;
}
CC_IMPL  img_t*
cvGetSubRect(const img_t* mat, img_t* submat, int x, int y, int width, int height)
{
    CRect r = {x, y, width, height};
    return cvGetSubRect(mat, submat, r);
}

// Selects array's row span.
CC_IMPL img_t*
cvGetRows(const img_t* mat, img_t* submat, int start_row, int end_row, int delta_row CC_DEFAULT(1))
{
    CC_FUNCNAME("cvGetRows");

    __BEGIN__;

    if(!submat)
        CC_ERROR(CC_StsNullPtr, "");

    if((unsigned)start_row >= (unsigned)mat->rows ||
        (unsigned)end_row > (unsigned)mat->rows || delta_row <= 0)
        CC_ERROR(CC_StsOutOfRange, "");

    *submat = *mat;
    if(delta_row == 1)
    {
        submat->rows = end_row - start_row;
        submat->step = mat->step & (submat->rows > 1 ? -1 : 0);
    }
    else
    {
        submat->rows = (end_row - start_row + delta_row - 1)/delta_row;
        submat->step = mat->step * delta_row;
    }

    submat->cols = mat->cols;
    submat->step &= submat->rows > 1 ? -1 : 0;
    submat->tt.data = mat->tt.data + (size_t)start_row*mat->step;
    submat->tid = mat->tid;
    
    __END__;

    return submat;
}

CC_IMPL img_t
cvGetRows(const img_t* mat, int start_row, int end_row, int delta_row CC_DEFAULT(1)) {
  img_t submat[1] = {0};
  return *cvGetRows(mat, submat, start_row, end_row, delta_row);
}

CC_INLINE img_t* cvGetRow(const img_t* mat, img_t* submat, int row)
{
    return cvGetRows(mat, submat, row, row + 1, 1);
}

CC_INLINE img_t cvGetRow(const img_t* mat, int row)
{
    return cvGetRows(mat, row, row + 1, 1);
}


// Selects array's column span.
CC_IMPL  img_t*
cvGetCols(const img_t* mat, img_t* submat, int start_col, int end_col)
{
    CC_FUNCNAME("cvGetCols");

    __BEGIN__;

    if(!submat)
        CC_ERROR(CC_StsNullPtr, "");

    if((unsigned)start_col >= (unsigned)mat->cols ||
        (unsigned)end_col > (unsigned)mat->cols)
        CC_ERROR(CC_StsOutOfRange, "");

    *submat = *mat;
    submat->rows = mat->rows;
    submat->cols = end_col - start_col;
    submat->step = mat->step & (submat->rows > 1 ? -1 : 0);
    submat->tt.data = mat->tt.data + (size_t)start_col*CC_ELEM_SIZE(mat->tid);
    submat->tid = mat->tid;
    
    
    __END__;

    return submat;
}
CC_IMPL  img_t
cvGetCols(const img_t* mat, int start_col, int end_col) {
    img_t submat[1] = {0};
    return *cvGetCols(mat, submat, start_col, end_col);
}

CC_INLINE  img_t* cvGetCol(const img_t* mat, img_t* submat, int col)
{
    return cvGetCols(mat, submat, col, col + 1);
}

// Selects array diagonal
CC_IMPL  img_t*
cvGetDiag(const img_t* mat, img_t* submat, int diag CC_DEFAULT(0))
{
    img_t* res = 0;
    
    CC_FUNCNAME("cvGetDiag");

    __BEGIN__;

    int len, pix_size; 

    if(!submat)
        CC_ERROR(CC_StsNullPtr, "");

    *submat = *mat;
    pix_size = CC_ELEM_SIZE(mat->tid);

    if(diag >= 0)
    {
        len = mat->cols - diag;
        
        if(len <= 0)
            CC_ERROR(CC_StsOutOfRange, "");

        len = CC_IMIN(len, mat->rows);
        submat->tt.data = mat->tt.data + diag*pix_size;
    }
    else
    {
        len = mat->rows + diag;
        
        if(len <= 0)
            CC_ERROR(CC_StsOutOfRange, "");

        len = CC_IMIN(len, mat->cols);
        submat->tt.data = mat->tt.data - diag*mat->step;
    }

    submat->rows = len;
    submat->cols = 1;
    submat->step = (mat->step + pix_size) & (submat->rows > 1 ? -1 : 0);
    submat->tid = mat->tid;
    res = submat;
    
    __END__;

    return res;
}

CC_IMPL int
cvCheckVector(const img_t* mat, int _elemChannels, TypeId type CC_DEFAULT(CC_NUL),
              bool _requireContinuous CC_DEFAULT(true))
{
    int dims = 2;
    int cn = CC_MAT_CN(mat);
    int size[4] = {0};
    int step[4] = {0};
    bool isContinuous = CC_IS_MAT_CONT(mat);
    int total = CC_MAT_TOTAL(mat);
    return (CC_MAT_TYPE(mat) == type || type == CC_NUL) &&
        (isContinuous || !_requireContinuous) &&
        ((dims == 2 && (((mat->h == 1 || mat->w == 1) && cn == _elemChannels) ||
                        (mat->w == _elemChannels && cn == 1))) ||
        (dims == 3 && cn == 1 && size[2] == _elemChannels && (size[0] == 1 || size[1] == 1) &&
         (isContinuous || step[1] == step[2]*size[2])))
    ? (int)(total*cn/_elemChannels) : -1;
}


/****************************************************************************************\
*                      Operations on CScalar and accessing array elements               *
\****************************************************************************************/

// Converts CScalar to specified type
CC_IMPL void
cvScalarToRawData(const CScalar* scalar, void* data, TypeId type, int cn, int extend_to_12 CC_DEFAULT(0))
{
    CC_FUNCNAME("cvScalarToRawData");
    
    __BEGIN__;

    int cn0 = cn;
    assert(scalar && data);
    if((unsigned)(cn - 1) >= 4)
        CC_ERROR(CC_StsOutOfRange, "The number of channels must be 1, 2, 3 or 4");

    switch(type)
    {
    case CC_8U:
        while(cn--)
        {
            int t = cRound(scalar->val[cn]);
            ((uchar*)data)[cn] = CC_CAST_8U(t);
        }
        break;
    case CC_8S:
        while(cn--)
        {
            int t = cRound(scalar->val[cn]);
            ((char*)data)[cn] = CC_CAST_8S(t);
        }
        break;
    case CC_16U:
        while(cn--)
        {
            int t = cRound(scalar->val[cn]);
            ((ushort*)data)[cn] = CC_CAST_16U(t);
        }
        break;
    case CC_16S:
        while(cn--)
        {
            int t = cRound(scalar->val[cn]);
            ((short*)data)[cn] = CC_CAST_16S(t);
        }
        break;
    case CC_32S:
        while(cn--)
            ((int*)data)[cn] = cRound(scalar->val[cn]);
        break;
    case CC_32F:
        while(cn--)
            ((float*)data)[cn] = (float)(scalar->val[cn]);
        break;
    case CC_64F:
        while(cn--)
            ((double*)data)[cn] = (double)(scalar->val[cn]);
        break;
    default:
        assert(0);
        CC_ERROR_FROM_CODE(CC_BadDepth);
    }

    if(extend_to_12)
    {
        int pix_size = CC_ELEM_SIZE(type);
        int offset = pix_size*cn0*12;

        do
        {
            offset -= pix_size;
            CC_MEMCPY_AUTO((char*)data + offset, data, pix_size);
        }
        while(offset > pix_size);
    }

    __END__;
}


// Converts data of specified type to CScalar
CC_IMPL void
cvRawDataToScalar(const void* data, TypeId type, int cn, CScalar* scalar)
{
    CC_FUNCNAME("cvRawDataToScalar");
    
    __BEGIN__;


    assert(scalar && data);
    
    if((unsigned)(cn - 1) >= 4)
        CC_ERROR(CC_StsOutOfRange, "The number of channels must be 1, 2, 3 or 4");

    memset(scalar->val, 0, sizeof(scalar->val));

    switch(type)
    {
    case CC_8U:
        while(cn--)
            scalar->val[cn] = CC_8TO32F(((uchar*)data)[cn]);
        break;
    case CC_8S:
        while(cn--)
            scalar->val[cn] = CC_8TO32F(((char*)data)[cn]);
        break;
    case CC_16U:
        while(cn--)
            scalar->val[cn] = ((ushort*)data)[cn];
        break;
    case CC_16S:
        while(cn--)
            scalar->val[cn] = ((short*)data)[cn];
        break;
    case CC_32S:
        while(cn--)
            scalar->val[cn] = ((int*)data)[cn];
        break;
    case CC_32F:
        while(cn--)
            scalar->val[cn] = ((float*)data)[cn];
        break;
    case CC_64F:
        while(cn--)
            scalar->val[cn] = ((double*)data)[cn];
        break;
    default:
        assert(0);
        CC_ERROR_FROM_CODE(CC_BadDepth);
    }

    __END__;
}

static double icvGetReal(const void* data, int type)
{
    switch(type)
    {
    case CC_8U:
        return *(uchar*)data;
    case CC_8S:
        return *(char*)data;
    case CC_16U:
        return *(ushort*)data;
    case CC_16S:
        return *(short*)data;
    case CC_32S:
        return *(int*)data;
    case CC_32F:
        return *(float*)data;
    case CC_64F:
        return *(double*)data;
    }

    return 0;
}


static void icvSetReal(double value, const void* data, int type)
{
    if(type < CC_32F)
    {
        int ivalue = cRound(value);
        switch(type)
        {
        case CC_8U:
            *(uchar*)data = CC_CAST_8U(ivalue);
            break;
        case CC_8S:
            *(char*)data = CC_CAST_8S(ivalue);
            break;
        case CC_16U:
            *(ushort*)data = CC_CAST_16U(ivalue);
            break;
        case CC_16S:
            *(short*)data = CC_CAST_16S(ivalue);
            break;
        case CC_32S:
            *(int*)data = CC_CAST_32S(ivalue);
            break;
        }
    }
    else
    {
        switch(type)
        {
        case CC_32F:
            *(float*)data = (float)value;
            break;
        case CC_64F:
            *(double*)data = value;
            break;
        }
    }
}


// Returns pointer to specified element of array (linear index is used)
CC_IMPL  uchar*
cvPtr1D(const img_t* mat, int idx, int* _type)
{
    uchar* ptr = 0;
    
    CC_FUNCNAME("cvPtr1D");

    __BEGIN__;


    TypeId type = CC_MAT_DEPTH(mat);
    int pix_size = CC_ELEM_SIZE(type);

    if(_type)
        *_type = type;
    
    // the first part is mul-free sufficient check
    // that the index is within the matrix
    if((unsigned)idx >= (unsigned)(mat->rows + mat->cols - 1) &&
        (unsigned)idx >= (unsigned)(mat->rows*mat->cols))
        CC_ERROR(CC_StsOutOfRange, "index is out of range");

    if(CC_IS_MAT_CONT(mat))
    {
        ptr = mat->tt.data + (size_t)idx*pix_size;
    }
    else
    {
        int row, col;
        if(mat->cols == 1)
            row = idx, col = 0;
        else
            row = idx/mat->cols, col = idx - row*mat->cols;
        ptr = mat->tt.data + (size_t)row*mat->step + col*pix_size;
    }


    __END__;

    return ptr;
}


// Returns pointer to specified element of 2d array
CC_IMPL  uchar*
cvPtr2D(const img_t* mat, int y, int x, TypeId* _type)
{
    uchar* ptr = 0;
    
    CC_FUNCNAME("cvPtr2D");

    __BEGIN__;

    TypeId type;

    if((unsigned)y >= (unsigned)(mat->rows) ||
        (unsigned)x >= (unsigned)(mat->cols))
        CC_ERROR(CC_StsOutOfRange, "index is out of range");

    type = CC_MAT_DEPTH(mat);
    if(_type)
        *_type = type;

    ptr = mat->tt.data + (size_t)y*mat->step + x*mat->c;

    __END__;

    return ptr;
}


// Returns pointer to specified element of 3d array
CC_IMPL  uchar*
cvPtr3D(const img_t* mat, int z, int y, int x, TypeId* _type)
{
    uchar* ptr = 0;
    
    CC_FUNCNAME("cvPtr3D");

    __BEGIN__;

    CC_ERROR(CC_StsBadArg, "unrecognized or unsupported array type");

    __END__;

    return ptr;
}


// Returns pointer to specified element of n-d array
CC_IMPL  uchar*
cvPtrND(const img_t* mat, const int* idx, TypeId* _type CC_DEFAULT(NULL),
         int create_node CC_DEFAULT(1), unsigned* precalc_hashval CC_DEFAULT(NULL))
{
    uchar* ptr = 0;
    CC_FUNCNAME("cvPtrND");

    __BEGIN__;

    ptr = cvPtr2D(mat, idx[0], idx[1], _type);
    CC_ERROR(CC_StsBadArg, "unrecognized or unsupported array type");

    __END__;

    return ptr;
}


// Returns specifed element of n-D array given linear index
CC_IMPL  CScalar
cvGet1D(const img_t* mat, int idx)
{
    CScalar scalar = {{0,0,0,0}};

    CC_FUNCNAME("cvGet1D");

    __BEGIN__;
    
    if(CC_IS_MAT_CONT(mat))
    {
        uchar* ptr;
        TypeId type = CC_MAT_DEPTH(mat);
        int pix_size = CC_ELEM_SIZE(type);
        int cn = CC_MAT_CN(mat);

        // the first part is mul-free sufficient check
        // that the index is within the matrix
        if((unsigned)idx >= (unsigned)(mat->rows + mat->cols - 1) &&
            (unsigned)idx >= (unsigned)(mat->rows*mat->cols))
            CC_ERROR(CC_StsOutOfRange, "index is out of range");

        ptr = mat->tt.data + (size_t)idx*pix_size;
        cvRawDataToScalar(ptr, type, cn, &scalar);
    }


    __END__;

    return scalar;
}


// Returns specifed element of 2D array
CC_IMPL  CScalar
cvGet2D(const img_t* mat, int y, int x)
{
    CScalar scalar = {{0,0,0,0}};

    CC_FUNCNAME("cvGet2D");

    __BEGIN__;

    uchar* ptr;

    if((unsigned)y >= (unsigned)(mat->rows) ||
        (unsigned)x >= (unsigned)(mat->cols))
        CC_ERROR(CC_StsOutOfRange, "index is out of range");

    ptr = mat->tt.data + (size_t)y*mat->step + x*mat->c;
    cvRawDataToScalar(ptr, mat->tid, CC_MAT_CN(mat), &scalar);

    __END__;

    return scalar;
}


// Returns specifed element of 3D array
CC_IMPL  CScalar
cvGet3D(const img_t* mat, int z, int y, int x)
{
    CScalar scalar = {{0,0,0,0}};

    /*CC_FUNCNAME("cvGet3D");*/

    __BEGIN__;

    TypeId type;
    uchar* ptr;

    ptr = cvPtr3D(mat, z, y, x, &type);

    cvRawDataToScalar(ptr, type, 1, &scalar);

    __END__;

    return scalar;
}


// Returns specifed element of nD array
CC_IMPL  CScalar
cvGetND(const img_t* mat, const int* idx)
{
    CScalar scalar = {{0,0,0,0}};

    /*CC_FUNCNAME("cvGetND");*/

    __BEGIN__;

    //TypeId type;
   // uchar* ptr;

    //cvRawDataToScalar(ptr, type, &scalar);

    __END__;

    return scalar;
}


// Returns specifed element of n-D array given linear index
CC_IMPL  double
cvGetReal1D(const img_t* mat, int idx)
{
    double value = 0;

    CC_FUNCNAME("cvGetReal1D");

    __BEGIN__;

    if(CC_IS_MAT_CONT(mat))
    {
        uchar* ptr;

        TypeId type = CC_MAT_DEPTH(mat);
        int pix_size = CC_ELEM_SIZE(type);

        // the first part is mul-free sufficient check
        // that the index is within the matrix
        if((unsigned)idx >= (unsigned)(mat->rows + mat->cols - 1) &&
            (unsigned)idx >= (unsigned)(mat->rows*mat->cols))
            CC_ERROR(CC_StsOutOfRange, "index is out of range");

        ptr = mat->tt.data + (size_t)idx*pix_size;
    }

    __END__;

    return value;
}


// Returns specifed element of 2D array
CC_IMPL  double
cvGetReal2D(const img_t* mat, int y, int x)
{
    double value = 0;

    CC_FUNCNAME("cvGetReal2D");

    __BEGIN__;

    uchar* ptr = NULL;

    if((unsigned)y >= (unsigned)(mat->rows) ||
        (unsigned)x >= (unsigned)(mat->cols))
        CC_ERROR(CC_StsOutOfRange, "index is out of range");

    TypeId type = CC_MAT_DEPTH(mat);
    ptr = mat->tt.data + (size_t)y*mat->step + x*mat->c;

    if(CC_MAT_CN(mat) > 1)
        CC_ERROR(CC_BadNumChannels, "cvGetReal* support only single-channel arrays");

    value = icvGetReal(ptr, type);


    __END__;

    return value;
}


// Returns specifed element of 3D array
CC_IMPL  double
cvGetReal3D(const img_t* mat, int z, int y, int x)
{
    double value = 0;

    CC_FUNCNAME("cvGetReal3D");

    __BEGIN__;

    TypeId type;
    uchar* ptr;

    ptr = cvPtr3D(mat, z, y, x, &type);
    
    if(ptr)
    {
        if(CC_MAT_CN(mat) > 1)
            CC_ERROR(CC_BadNumChannels, "cvGetReal* support only single-channel arrays");

        value = icvGetReal(ptr, type);
    }

    __END__;

    return value;
}


// Returns specifed element of nD array
CC_IMPL  double
cvGetRealND(const img_t* mat, const int* idx)
{
    double value = 0;

    CC_FUNCNAME("cvGetRealND");

    __BEGIN__;

    TypeId type;
    uchar* ptr;
    ptr = cvPtrND(mat, idx, &type);

    if(ptr)
    {
        if(CC_MAT_CN(mat) > 1)
            CC_ERROR(CC_BadNumChannels, "cvGetReal* support only single-channel arrays");

        value = icvGetReal(ptr, type);
    }

    __END__;

    return value;
}


// Assigns new value to specifed element of nD array given linear index
CC_IMPL  void
cvSet1D(img_t* mat, int idx, CScalar scalar)
{
    CC_FUNCNAME("cvSet1D");

    __BEGIN__;

    if(CC_IS_MAT_CONT(mat))
    {
        uchar* ptr;

        TypeId type = CC_MAT_DEPTH(mat);
        int pix_size = CC_ELEM_SIZE(type);

        // the first part is mul-free sufficient check
        // that the index is within the matrix
        if((unsigned)idx >= (unsigned)(mat->rows + mat->cols - 1) &&
            (unsigned)idx >= (unsigned)(mat->rows*mat->cols))
            CC_ERROR(CC_StsOutOfRange, "index is out of range");

        ptr = mat->tt.data + (size_t)idx*pix_size;
    cvScalarToRawData(&scalar, ptr, type, CC_MAT_CN(mat));
    }

    __END__;
}


// Assigns new value to specifed element of 2D array
CC_IMPL  void
cvSet2D(img_t* mat, int y, int x, CScalar scalar)
{
    CC_FUNCNAME("cvSet2D");

    __BEGIN__;

    uchar* ptr;

    if((unsigned)y >= (unsigned)(mat->rows) ||
        (unsigned)x >= (unsigned)(mat->cols))
        CC_ERROR(CC_StsOutOfRange, "index is out of range");

    TypeId type = CC_MAT_DEPTH(mat);
    ptr = mat->tt.data + (size_t)y*mat->step + x*mat->c;
    cvScalarToRawData(&scalar, ptr, type, CC_MAT_CN(mat));

    __END__;
}


// Assigns new value to specifed element of 3D array
CC_IMPL  void
cvSet3D(img_t* mat, int z, int y, int x, CScalar scalar)
{
    /*CC_FUNCNAME("cvSet3D");*/

    __BEGIN__;

    TypeId type;
    uchar* ptr;
    ptr = cvPtr3D(mat, z, y, x, &type);
    cvScalarToRawData(&scalar, ptr, type, CC_MAT_CN(mat));

    __END__;
}


// Assigns new value to specifed element of nD array
CC_IMPL  void
cvSetND(img_t* mat, const int* idx, CScalar scalar)
{
    /*CC_FUNCNAME("cvSetND");*/

    __BEGIN__;

    TypeId type;
    uchar* ptr;
    
        ptr = cvPtrND(mat, idx, &type);
    cvScalarToRawData(&scalar, ptr, type, CC_MAT_CN(mat));

    __END__;
}


CC_IMPL  void
cvSetReal1D(img_t* mat, int idx, double value)
{
    CC_FUNCNAME("cvSetReal1D");

    __BEGIN__;

    if(CC_IS_MAT_CONT(mat))
    {
		uchar* ptr;
		TypeId  type = CC_MAT_DEPTH(mat);
        int pix_size = CC_ELEM_SIZE(type);

        // the first part is mul-free sufficient check
        // that the index is within the matrix
        if((unsigned)idx >= (unsigned)(mat->rows + mat->cols - 1) &&
            (unsigned)idx >= (unsigned)(mat->rows*mat->cols))
            CC_ERROR(CC_StsOutOfRange, "index is out of range");

        ptr = mat->tt.data + (size_t)idx*pix_size;

		if(CC_MAT_CN(mat) > 1)
			CC_ERROR(CC_BadNumChannels, "cvSetReal* support only single-channel arrays");

        icvSetReal(value, ptr, type);

	}

    __END__;
}


CC_IMPL  void
cvSetReal2D(img_t* mat, int y, int x, double value)
{
    CC_FUNCNAME("cvSetReal2D");

    __BEGIN__;

    TypeId type;
    uchar* ptr;

    if((unsigned)y >= (unsigned)(mat->rows) ||
        (unsigned)x >= (unsigned)(mat->cols))
        CC_ERROR(CC_StsOutOfRange, "index is out of range");

    type = CC_MAT_DEPTH(mat);
    ptr = mat->tt.data + (size_t)y*mat->step + x*mat->c;
    
    if(CC_MAT_CN(mat) > 1)
        CC_ERROR(CC_BadNumChannels, "cvSetReal* support only single-channel arrays");

    if(ptr)
        icvSetReal(value, ptr, type);

    __END__;
}


CC_IMPL  void
cvSetReal3D(img_t* mat, int z, int y, int x, double value)
{
    CC_FUNCNAME("cvSetReal3D");

    __BEGIN__;

    TypeId type;
    uchar* ptr;
    
        ptr = cvPtr3D(mat, z, y, x, &type);
        
    if(CC_MAT_CN(mat) > 1)
        CC_ERROR(CC_BadNumChannels, "cvSetReal* support only single-channel arrays");

    if(ptr)
        icvSetReal(value, ptr, type);

    __END__;
}


CC_IMPL  void
cvSetRealND(img_t* mat, const int* idx, double value)
{
    CC_FUNCNAME("cvSetRealND");

    __BEGIN__;

    TypeId type;
    uchar* ptr;
    
        ptr = cvPtrND(mat, idx, &type);
        
    if(CC_MAT_CN(mat) > 1)
        CC_ERROR(CC_BadNumChannels, "cvSetReal* support only single-channel arrays");

    if(ptr)
        icvSetReal(value, ptr, type);

    __END__;
}


CC_IMPL void
cvClearND(img_t* mat, const int* idx)
{
    /*CC_FUNCNAME("cvClearND");*/

    __BEGIN__;

    TypeId type;
    uchar* ptr;
    ptr = cvPtrND(mat, idx, &type);
    if(ptr)
        CC_ZERO_CHAR(ptr, CC_ELEM_SIZE(type));

    __END__;
}


/****************************************************************************************\
*                             Conversion to img_t or img_t                            *
\****************************************************************************************/


CC_IMPL img_t*
cvReshapeMatND(const img_t* mat,
                int sizeof_header, img_t* _header,
                int new_cn, int new_dims, int* new_sizes)
{
    img_t* result = 0;
    CC_FUNCNAME("cvReshapeMatND");

    __BEGIN__;

    int dims, coi = 0;

    if(!mat || !_header)
        CC_ERROR(CC_StsNullPtr, "NULL pointer to array or destination header");

    if(new_cn == 0 && new_dims == 0)
        CC_ERROR(CC_StsBadArg, "None of array parameters is changed: dummy call?");

    CC_CALL(dims = cvGetDims(mat));

    if(new_dims == 0)
    {
        new_sizes = 0;
        new_dims = dims;
    }
    else if(new_dims == 1)
    {
        new_sizes = 0;
    }
    else
    {
        if(new_dims <= 0 || new_dims > CC_MAX_DIM)
            CC_ERROR(CC_StsOutOfRange, "Non-positive or too large number of dimensions");
        if(!new_sizes)
            CC_ERROR(CC_StsNullPtr, "New dimension sizes are not specified");
    }

    if(new_dims <= 2)
    {
        img_t* header = _header;
        int  total_width, new_rows, cn;

        if(sizeof_header != sizeof(img_t))
            CC_ERROR(CC_StsBadArg, "The header should be img_t");

        cn = CC_MAT_CN(mat);
        total_width = mat->cols * cn;

        if(new_cn == 0)
            new_cn = cn;

        if(new_sizes)
            new_rows = new_sizes[0];
        else if(new_dims == 1)
            new_rows = total_width*mat->rows/new_cn;
        else
        {
            new_rows = mat->rows;
            if(new_cn > total_width)
                new_rows = mat->rows * total_width / new_cn;
        }

        if(new_rows != mat->rows)
        {
            int total_size = total_width * mat->rows;

            if(!CC_IS_MAT_CONT(mat))
                CC_ERROR(CC_BadStep,
                "The matrix is not continuous so the number of rows can not be changed");

            total_width = total_size / new_rows;

            if(total_width * new_rows != total_size)
                CC_ERROR(CC_StsBadArg, "The total number of matrix elements "
                                        "is not divisible by the new number of rows");
        }

        header->rows = new_rows;
        header->cols = total_width / new_cn;

        if(header->cols * new_cn != total_width ||
            new_sizes && header->cols != new_sizes[1])
            CC_ERROR(CC_StsBadArg, "The total matrix width is not "
                            "divisible by the new number of columns");

        header->tid = mat->tid;
        header->step = header->cols * CC_ELEM_SIZE(mat->tid);
        header->step &= new_rows > 1 ? -1 : 0;
    }

    if(!coi)
        CC_ERROR(CC_BadCOI, "COI is not supported by this operation");

    result = _header;
    
    __END__;

    return result;
}

CC_IMPL img_t*
cvReshape(const img_t* mat, img_t* header, int new_cn, int new_rows)
{
    CC_FUNCNAME("cvReshape");

    __BEGIN__;

    int total_width, new_width;
	int elem_size = CC_ELEM_SIZE(mat->tid);

    if(!header)
        CC_ERROR(CC_StsNullPtr, "");

    if(new_cn == 0)
        new_cn = CC_MAT_CN(mat);
    else if((unsigned)(new_cn - 1) > 3)
        CC_ERROR(CC_BadNumChannels, "");

    if(mat != header)
    {
        *header = *mat;
    }

    total_width = mat->cols * CC_MAT_CN(mat);

    if((new_cn > total_width || total_width % new_cn != 0) && new_rows == 0)
        new_rows = mat->rows * total_width / new_cn;

    if(new_rows == 0 || new_rows == mat->rows)
    {
        header->rows = mat->rows;
        header->step = mat->step;
    }
    else
    {
        int total_size = total_width * mat->rows;
        if(!CC_IS_MAT_CONT(mat))
            CC_ERROR(CC_BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed");

        if((unsigned)new_rows > (unsigned)total_size)
            CC_ERROR(CC_StsOutOfRange, "Bad new number of rows");

        total_width = total_size / new_rows;

        if(total_width * new_rows != total_size)
            CC_ERROR(CC_StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows");

        header->rows = new_rows;
        header->step = total_width * elem_size;
    }

    new_width = total_width / new_cn;

    if(new_width * new_cn != total_width)
        CC_ERROR(CC_BadNumChannels,
        "The total width is not divisible by the new number of channels");

	header->cols = new_width;
	header->c = new_cn * elem_size;
    header->tid = mat->tid;
	if (mat->w*mat->c != mat->s) {
		int asdf = 0;
	}
	ASSERT(mat->w*mat->c == mat->s);

    __END__;

    return  header;
}

CC_IMPL img_t
cvReshape(const img_t* mat, int new_cn, int new_rows CC_DEFAULT(0)) {
    img_t header[1] = {0};
  return *cvReshape(mat, header, new_cn, new_rows);
}

// convert array (img_t or img_t) to img_t
CC_IMPL img_t*
cvGetImage(const img_t* array, img_t* img)
{
    img_t* result = 0;
    const img_t* src = (const img_t*)array;
    
    CC_FUNCNAME("cvGetImage");

    __BEGIN__;

    //TypeId type;

    if(!img)
        CC_ERROR_FROM_CODE(CC_StsNullPtr);

    result = (img_t*)src;

    __END__;

    return result;
}


/****************************************************************************************\
*                               img_t-specific functions                              *
\****************************************************************************************/

// initalize img_t header, allocated by the user
CC_IMPL img_t*
cvInitImageHeader(img_t * image, CSize size, IMG_DEPTH depth,
                   int channels, int origin CC_DEFAULT(0), int align CC_DEFAULT(4))
{
    img_t* result = 0;

    CC_FUNCNAME("cvInitImageHeader");

    __BEGIN__;

    if(!image)
        CC_ERROR(CC_HeaderIsNull, "null pointer to header");

    memset(image, 0, sizeof(*image));

    if(size.width < 0 || size.height < 0)
        CC_ERROR(CC_BadROISize, "Bad input roi");

    if((depth != (int)IMG_DEPTH_1U && depth != (int)IMG_DEPTH_8U &&
         depth != (int)IMG_DEPTH_8S && depth != (int)IMG_DEPTH_16U &&
         depth != (int)IMG_DEPTH_16S && depth != (int)IMG_DEPTH_32S &&
         depth != (int)IMG_DEPTH_32F && depth != (int)IMG_DEPTH_64F) ||
         channels < 0)
        CC_ERROR(CC_BadDepth, "Unsupported format");
    if(origin != CC_ORIGIN_BL && origin != CC_ORIGIN_TL)
        CC_ERROR(CC_BadOrigin, "Bad input origin");

    if(align != 4 && align != 8)
        CC_ERROR(CC_BadAlign, "Bad input align");
#define CVINITIMAGEHEADER_SETTYPE_CASE(a, b) case a:  image->tid = b; break
    switch(depth) {
      CVINITIMAGEHEADER_SETTYPE_CASE(IMG_DEPTH_8U, CC_8U);
      CVINITIMAGEHEADER_SETTYPE_CASE(IMG_DEPTH_8S, CC_8S);
      CVINITIMAGEHEADER_SETTYPE_CASE(IMG_DEPTH_16S, CC_16S);
      CVINITIMAGEHEADER_SETTYPE_CASE(IMG_DEPTH_32S, CC_32S);
      CVINITIMAGEHEADER_SETTYPE_CASE(IMG_DEPTH_32F, CC_32F);
      CVINITIMAGEHEADER_SETTYPE_CASE(IMG_DEPTH_64F, CC_64F);
    default:
        CC_ERROR(CC_BadDepth, "Unsupported format");
      break;
    }
    image->width = size.width;
    image->height = size.height;
    image->c = MAX(channels, 1) * CC_ELEM_SIZE(image->tid);
    image->f = 1;
    image->step = (((image->width * image->c + 7)/8)+ align - 1) & (~(align - 1));
    //image->origin = origin;
    //image->imageSize = image->widthStep * image->height;

    result = image;

    __END__;

    return result;
}

// create img_t header
CC_IMPL img_t *
cvCreateImageHeader(img_t* img, CSize size, IMG_DEPTH depth, int channels)
{
    CC_FUNCNAME("cvCreateImageHeader");

    __BEGIN__;

    CC_CALL(cvInitImageHeader(img, size, depth, channels, IMG_ORIGIN_TL,
                                CC_DEFAULT_IMAGE_ROW_ALIGN));

    __END__;

    return img;
}


// create img_t header and allocate underlying data
CC_IMPL img_t *
cvCreateImage(img_t *img, CSize size, IMG_DEPTH depth, int channels)
{
    CC_FUNCNAME("cvCreateImage");

    __BEGIN__;

    CC_CALL(img = cvCreateImageHeader(img, size, depth, channels));
    assert(img);
    CC_CALL(cvCreateData(img));

    __END__;

    return img;
}



CC_IMPL void
cvReleaseImageHeader(img_t* image)
{
    CC_FUNCNAME("cvReleaseImageHeader");

    __BEGIN__;

    if(!image)
        CC_ERROR(CC_StsNullPtr, "");

    imfree(image);

    __END__;
}


CC_IMPL void
cvReleaseImage(img_t ** image)
{
    CC_FUNCNAME("cvReleaseImage");

    __BEGIN__

    if(!image)
        CC_ERROR(CC_StsNullPtr, "");

    if(*image)
    {
        img_t* img = *image;
        *image = 0;
        
        cvReleaseData(img);
        cvReleaseImageHeader(img);
    }

    __END__;
}

CC_IMPL img_t*
cvCloneImage(img_t* dst, const img_t* src)
{
    CC_FUNCNAME("cvCloneImage");

    __BEGIN__;

    cvCloneMat(dst, src);

    __END__;

    return dst;
}


/****************************************************************************************\
*                            Additional operations on CTermCriteria                     *
\****************************************************************************************/

typedef struct CTermCriteria {
  int type;  /* may be combination of
                       CC_TERMCRIT_ITER
                       CC_TERMCRIT_EPS */
  int max_iter;
  double epsilon;
}
CTermCriteria;
CC_INLINE CTermCriteria cTermCriteria(int type, int max_iter, double epsilon)
{
  CTermCriteria t;
  t.type = type;
  t.max_iter = max_iter;
  t.epsilon = (float) epsilon;
  return t;
}
CC_IMPL CTermCriteria
cvCheckTermCriteria(CTermCriteria criteria, double default_eps,
                     int default_max_iters)
{
    CC_FUNCNAME("cvCheckTermCriteria");

    CTermCriteria crit;

    crit.type = CC_TERMCRIT_ITER|CC_TERMCRIT_EPS;
    crit.max_iter = default_max_iters;
    crit.epsilon = (float)default_eps;
    
    __BEGIN__;

    if((criteria.type & ~(CC_TERMCRIT_EPS | CC_TERMCRIT_ITER)) != 0)
        CC_ERROR(CC_StsBadArg,
                  "Unknown type of term criteria");

    if((criteria.type & CC_TERMCRIT_ITER) != 0)
    {
        if(criteria.max_iter <= 0)
            CC_ERROR(CC_StsBadArg,
                  "Iterations flag is set and maximum number of iterations is <= 0");
        crit.max_iter = criteria.max_iter;
    }
    
    if((criteria.type & CC_TERMCRIT_EPS) != 0)
    {
        if(criteria.epsilon < 0)
            CC_ERROR(CC_StsBadArg, "Accuracy flag is set and epsilon is < 0");

        crit.epsilon = criteria.epsilon;
    }

    if((criteria.type & (CC_TERMCRIT_EPS | CC_TERMCRIT_ITER)) == 0)
        CC_ERROR(CC_StsBadArg,
                  "Neither accuracy nor maximum iterations "
                  "number flags are set in criteria type");

    __END__;

    crit.epsilon = (float)MAX(0, crit.epsilon);
    crit.max_iter = MAX(1, crit.max_iter);

    return crit;
}



