

#ifndef OPENCC_CORE_MATRIX_OPERATIONS_HPP
#define OPENCC_CORE_MATRIX_OPERATIONS_HPP

#ifndef __cplusplus
#  error mat.inl.hpp header must be compiled as C++
#endif

namespace cv
{

//! @cond IGNORED

//////////////////////// Input/Output Arrays ////////////////////////

inline void _InputArray::init(int _flags, const void* _obj)
{ flags = _flags; obj = (void*)_obj; }

inline void _InputArray::init(int _flags, const void* _obj, Size _sz)
{ flags = _flags; obj = (void*)_obj; sz = _sz; }

inline void* _InputArray::getObj() const { return obj; }
inline int _InputArray::getFlags() const { return flags; }
inline Size _InputArray::getSz() const { return sz; }

inline _InputArray::_InputArray() { init(NONE, 0); }
inline _InputArray::_InputArray(int _flags, void* _obj) { init(_flags, _obj); }
inline _InputArray::_InputArray(const CvMat& m) { init(MAT+ACCESS_READ, &m); }
inline _InputArray::_InputArray(const std::vector<CvMat>& vec) { init(STD_VECTOR_MAT+ACCESS_READ, &vec); }
inline _InputArray::_InputArray(const UMat& m) { init(UMAT+ACCESS_READ, &m); }
inline _InputArray::_InputArray(const std::vector<UMat>& vec) { init(STD_VECTOR_UMAT+ACCESS_READ, &vec); }

template<typename _Tp> inline
_InputArray::_InputArray(const std::vector<_Tp>& vec)
{ init(FIXED_TYPE + STD_VECTOR + DataType<_Tp>::type + ACCESS_READ, &vec); }

inline
_InputArray::_InputArray(const std::vector<bool>& vec)
{ init(FIXED_TYPE + STD_BOOL_VECTOR + DataType<bool>::type + ACCESS_READ, &vec); }

template<typename _Tp> inline
_InputArray::_InputArray(const std::vector<std::vector<_Tp> >& vec)
{ init(FIXED_TYPE + STD_VECTOR_VECTOR + DataType<_Tp>::type + ACCESS_READ, &vec); }

template<typename _Tp> inline
_InputArray::_InputArray(const std::vector<Mat_<_Tp> >& vec)
{ init(FIXED_TYPE + STD_VECTOR_MAT + DataType<_Tp>::type + ACCESS_READ, &vec); }

template<typename _Tp, int m, int n> inline
_InputArray::_InputArray(const Matx<_Tp, m, n>& mtx)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_READ, &mtx, Size(n, m)); }

template<typename _Tp> inline
_InputArray::_InputArray(const _Tp* vec, int n)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_READ, vec, Size(n, 1)); }

template<typename _Tp> inline
_InputArray::_InputArray(const Mat_<_Tp>& m)
{ init(FIXED_TYPE + MAT + DataType<_Tp>::type + ACCESS_READ, &m); }

inline _InputArray::_InputArray(const double& val)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + CC_64F + ACCESS_READ, &val, Size(1,1)); }

inline _InputArray::_InputArray(const MatExpr& expr)
{ init(FIXED_TYPE + FIXED_SIZE + EXPR + ACCESS_READ, &expr); }

inline _InputArray::_InputArray(const cuda::GpuMat& d_mat)
{ init(CUDA_GPU_MAT + ACCESS_READ, &d_mat); }

inline _InputArray::_InputArray(const std::vector<cuda::GpuMat>& d_mat)
{	init(STD_VECTOR_CUDA_GPU_MAT + ACCESS_READ, &d_mat);}

inline _InputArray::_InputArray(const ogl::Buffer& buf)
{ init(OPENGL_BUFFER + ACCESS_READ, &buf); }

inline _InputArray::_InputArray(const cuda::HostMem& cuda_mem)
{ init(CUDA_HOST_MEM + ACCESS_READ, &cuda_mem); }

inline _InputArray::~_InputArray() {}

inline CvMat _InputArray::getMat(int i) const
{
    if( kind() == MAT && i < 0 )
        return *(const CvMat*)obj;
    return getMat_(i);
}

inline bool _InputArray::isMat() const { return kind() == _InputArray::MAT; }
inline bool _InputArray::isUMat() const  { return kind() == _InputArray::UMAT; }
inline bool _InputArray::isMatVector() const { return kind() == _InputArray::STD_VECTOR_MAT; }
inline bool _InputArray::isUMatVector() const  { return kind() == _InputArray::STD_VECTOR_UMAT; }
inline bool _InputArray::isMatx() const { return kind() == _InputArray::MATX; }
inline bool _InputArray::isVector() const { return kind() == _InputArray::STD_VECTOR || kind() == _InputArray::STD_BOOL_VECTOR; }
inline bool _InputArray::isGpuMatVector() const { return kind() == _InputArray::STD_VECTOR_CUDA_GPU_MAT; }

////////////////////////////////////////////////////////////////////////////////////////

inline _OutputArray::_OutputArray() { init(ACCESS_WRITE, 0); }
inline _OutputArray::_OutputArray(int _flags, void* _obj) { init(_flags|ACCESS_WRITE, _obj); }
inline _OutputArray::_OutputArray(CvMat& m) { init(MAT+ACCESS_WRITE, &m); }
inline _OutputArray::_OutputArray(std::vector<CvMat>& vec) { init(STD_VECTOR_MAT+ACCESS_WRITE, &vec); }
inline _OutputArray::_OutputArray(UMat& m) { init(UMAT+ACCESS_WRITE, &m); }
inline _OutputArray::_OutputArray(std::vector<UMat>& vec) { init(STD_VECTOR_UMAT+ACCESS_WRITE, &vec); }

template<typename _Tp> inline
_OutputArray::_OutputArray(std::vector<_Tp>& vec)
{ init(FIXED_TYPE + STD_VECTOR + DataType<_Tp>::type + ACCESS_WRITE, &vec); }

inline
_OutputArray::_OutputArray(std::vector<bool>&)
{ CC_Error(Error::StsUnsupportedFormat, "std::vector<bool> cannot be an output array\n"); }

template<typename _Tp> inline
_OutputArray::_OutputArray(std::vector<std::vector<_Tp> >& vec)
{ init(FIXED_TYPE + STD_VECTOR_VECTOR + DataType<_Tp>::type + ACCESS_WRITE, &vec); }

template<typename _Tp> inline
_OutputArray::_OutputArray(std::vector<Mat_<_Tp> >& vec)
{ init(FIXED_TYPE + STD_VECTOR_MAT + DataType<_Tp>::type + ACCESS_WRITE, &vec); }

template<typename _Tp> inline
_OutputArray::_OutputArray(Mat_<_Tp>& m)
{ init(FIXED_TYPE + MAT + DataType<_Tp>::type + ACCESS_WRITE, &m); }

template<typename _Tp, int m, int n> inline
_OutputArray::_OutputArray(Matx<_Tp, m, n>& mtx)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_WRITE, &mtx, Size(n, m)); }

template<typename _Tp> inline
_OutputArray::_OutputArray(_Tp* vec, int n)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_WRITE, vec, Size(n, 1)); }

template<typename _Tp> inline
_OutputArray::_OutputArray(const std::vector<_Tp>& vec)
{ init(FIXED_TYPE + FIXED_SIZE + STD_VECTOR + DataType<_Tp>::type + ACCESS_WRITE, &vec); }

template<typename _Tp> inline
_OutputArray::_OutputArray(const std::vector<std::vector<_Tp> >& vec)
{ init(FIXED_TYPE + FIXED_SIZE + STD_VECTOR_VECTOR + DataType<_Tp>::type + ACCESS_WRITE, &vec); }

template<typename _Tp> inline
_OutputArray::_OutputArray(const std::vector<Mat_<_Tp> >& vec)
{ init(FIXED_TYPE + FIXED_SIZE + STD_VECTOR_MAT + DataType<_Tp>::type + ACCESS_WRITE, &vec); }

template<typename _Tp> inline
_OutputArray::_OutputArray(const Mat_<_Tp>& m)
{ init(FIXED_TYPE + FIXED_SIZE + MAT + DataType<_Tp>::type + ACCESS_WRITE, &m); }

template<typename _Tp, int m, int n> inline
_OutputArray::_OutputArray(const Matx<_Tp, m, n>& mtx)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_WRITE, &mtx, Size(n, m)); }

template<typename _Tp> inline
_OutputArray::_OutputArray(const _Tp* vec, int n)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_WRITE, vec, Size(n, 1)); }

inline _OutputArray::_OutputArray(cuda::GpuMat& d_mat)
{ init(CUDA_GPU_MAT + ACCESS_WRITE, &d_mat); }

inline _OutputArray::_OutputArray(std::vector<cuda::GpuMat>& d_mat)
{	init(STD_VECTOR_CUDA_GPU_MAT + ACCESS_WRITE, &d_mat);}

inline _OutputArray::_OutputArray(ogl::Buffer& buf)
{ init(OPENGL_BUFFER + ACCESS_WRITE, &buf); }

inline _OutputArray::_OutputArray(cuda::HostMem& cuda_mem)
{ init(CUDA_HOST_MEM + ACCESS_WRITE, &cuda_mem); }

inline _OutputArray::_OutputArray(const CvMat& m)
{ init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_WRITE, &m); }

inline _OutputArray::_OutputArray(const std::vector<CvMat>& vec)
{ init(FIXED_SIZE + STD_VECTOR_MAT + ACCESS_WRITE, &vec); }

inline _OutputArray::_OutputArray(const UMat& m)
{ init(FIXED_TYPE + FIXED_SIZE + UMAT + ACCESS_WRITE, &m); }

inline _OutputArray::_OutputArray(const std::vector<UMat>& vec)
{ init(FIXED_SIZE + STD_VECTOR_UMAT + ACCESS_WRITE, &vec); }

inline _OutputArray::_OutputArray(const cuda::GpuMat& d_mat)
{ init(FIXED_TYPE + FIXED_SIZE + CUDA_GPU_MAT + ACCESS_WRITE, &d_mat); }


inline _OutputArray::_OutputArray(const ogl::Buffer& buf)
{ init(FIXED_TYPE + FIXED_SIZE + OPENGL_BUFFER + ACCESS_WRITE, &buf); }

inline _OutputArray::_OutputArray(const cuda::HostMem& cuda_mem)
{ init(FIXED_TYPE + FIXED_SIZE + CUDA_HOST_MEM + ACCESS_WRITE, &cuda_mem); }

///////////////////////////////////////////////////////////////////////////////////////////

inline _InputOutputArray::_InputOutputArray() { init(ACCESS_RW, 0); }
inline _InputOutputArray::_InputOutputArray(int _flags, void* _obj) { init(_flags|ACCESS_RW, _obj); }
inline _InputOutputArray::_InputOutputArray(CvMat& m) { init(MAT+ACCESS_RW, &m); }
inline _InputOutputArray::_InputOutputArray(std::vector<CvMat>& vec) { init(STD_VECTOR_MAT+ACCESS_RW, &vec); }
inline _InputOutputArray::_InputOutputArray(UMat& m) { init(UMAT+ACCESS_RW, &m); }
inline _InputOutputArray::_InputOutputArray(std::vector<UMat>& vec) { init(STD_VECTOR_UMAT+ACCESS_RW, &vec); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(std::vector<_Tp>& vec)
{ init(FIXED_TYPE + STD_VECTOR + DataType<_Tp>::type + ACCESS_RW, &vec); }

inline _InputOutputArray::_InputOutputArray(std::vector<bool>&)
{ CC_Error(Error::StsUnsupportedFormat, "std::vector<bool> cannot be an input/output array\n"); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(std::vector<std::vector<_Tp> >& vec)
{ init(FIXED_TYPE + STD_VECTOR_VECTOR + DataType<_Tp>::type + ACCESS_RW, &vec); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(std::vector<Mat_<_Tp> >& vec)
{ init(FIXED_TYPE + STD_VECTOR_MAT + DataType<_Tp>::type + ACCESS_RW, &vec); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(Mat_<_Tp>& m)
{ init(FIXED_TYPE + MAT + DataType<_Tp>::type + ACCESS_RW, &m); }

template<typename _Tp, int m, int n> inline
_InputOutputArray::_InputOutputArray(Matx<_Tp, m, n>& mtx)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_RW, &mtx, Size(n, m)); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(_Tp* vec, int n)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_RW, vec, Size(n, 1)); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(const std::vector<_Tp>& vec)
{ init(FIXED_TYPE + FIXED_SIZE + STD_VECTOR + DataType<_Tp>::type + ACCESS_RW, &vec); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(const std::vector<std::vector<_Tp> >& vec)
{ init(FIXED_TYPE + FIXED_SIZE + STD_VECTOR_VECTOR + DataType<_Tp>::type + ACCESS_RW, &vec); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(const std::vector<Mat_<_Tp> >& vec)
{ init(FIXED_TYPE + FIXED_SIZE + STD_VECTOR_MAT + DataType<_Tp>::type + ACCESS_RW, &vec); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(const Mat_<_Tp>& m)
{ init(FIXED_TYPE + FIXED_SIZE + MAT + DataType<_Tp>::type + ACCESS_RW, &m); }

template<typename _Tp, int m, int n> inline
_InputOutputArray::_InputOutputArray(const Matx<_Tp, m, n>& mtx)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_RW, &mtx, Size(n, m)); }

template<typename _Tp> inline
_InputOutputArray::_InputOutputArray(const _Tp* vec, int n)
{ init(FIXED_TYPE + FIXED_SIZE + MATX + DataType<_Tp>::type + ACCESS_RW, vec, Size(n, 1)); }

inline _InputOutputArray::_InputOutputArray(cuda::GpuMat& d_mat)
{ init(CUDA_GPU_MAT + ACCESS_RW, &d_mat); }

inline _InputOutputArray::_InputOutputArray(ogl::Buffer& buf)
{ init(OPENGL_BUFFER + ACCESS_RW, &buf); }

inline _InputOutputArray::_InputOutputArray(cuda::HostMem& cuda_mem)
{ init(CUDA_HOST_MEM + ACCESS_RW, &cuda_mem); }

inline _InputOutputArray::_InputOutputArray(const CvMat& m)
{ init(FIXED_TYPE + FIXED_SIZE + MAT + ACCESS_RW, &m); }

inline _InputOutputArray::_InputOutputArray(const std::vector<CvMat>& vec)
{ init(FIXED_SIZE + STD_VECTOR_MAT + ACCESS_RW, &vec); }

inline _InputOutputArray::_InputOutputArray(const UMat& m)
{ init(FIXED_TYPE + FIXED_SIZE + UMAT + ACCESS_RW, &m); }

inline _InputOutputArray::_InputOutputArray(const std::vector<UMat>& vec)
{ init(FIXED_SIZE + STD_VECTOR_UMAT + ACCESS_RW, &vec); }

inline _InputOutputArray::_InputOutputArray(const cuda::GpuMat& d_mat)
{ init(FIXED_TYPE + FIXED_SIZE + CUDA_GPU_MAT + ACCESS_RW, &d_mat); }

inline _InputOutputArray::_InputOutputArray(const std::vector<cuda::GpuMat>& d_mat)
{ init(FIXED_TYPE + FIXED_SIZE + STD_VECTOR_CUDA_GPU_MAT + ACCESS_RW, &d_mat);}

template<> inline _InputOutputArray::_InputOutputArray(std::vector<cuda::GpuMat>& d_mat)
{ init(FIXED_TYPE + FIXED_SIZE + STD_VECTOR_CUDA_GPU_MAT + ACCESS_RW, &d_mat);}

inline _InputOutputArray::_InputOutputArray(const ogl::Buffer& buf)
{ init(FIXED_TYPE + FIXED_SIZE + OPENGL_BUFFER + ACCESS_RW, &buf); }

inline _InputOutputArray::_InputOutputArray(const cuda::HostMem& cuda_mem)
{ init(FIXED_TYPE + FIXED_SIZE + CUDA_HOST_MEM + ACCESS_RW, &cuda_mem); }

//////////////////////////////////////////// CvMat //////////////////////////////////////////

inline
CvMat::CvMat()
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{}

inline
CvMat::CvMat(int _rows, int _cols, int _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    create(_rows, _cols, _type);
}

inline
CvMat::CvMat(int _rows, int _cols, int _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    create(_rows, _cols, _type);
    *this = _s;
}

inline
CvMat::CvMat(Size _sz, int _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    create( _sz.height, _sz.width, _type );
}

inline
CvMat::CvMat(Size _sz, int _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    create(_sz.height, _sz.width, _type);
    *this = _s;
}

inline
CvMat::CvMat(int _dims, const int* _sz, int _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    create(_dims, _sz, _type);
}

inline
CvMat::CvMat(int _dims, const int* _sz, int _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    create(_dims, _sz, _type);
    *this = _s;
}

inline
CvMat::CvMat(const std::vector<int>& _sz, int _type)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    create(_sz, _type);
}

inline
CvMat::CvMat(const std::vector<int>& _sz, int _type, const Scalar& _s)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    create(_sz, _type);
    *this = _s;
}

inline
CvMat::CvMat(const CvMat& m)
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(&rows)
{
    if( u )
        CC_XADD(&u->refcount, 1);
    if( m.dims <= 2 )
    {
        step[0] = m.step[0]; step[1] = m.step[1];
    }
    else
    {
        dims = 0;
        copySize(m);
    }
}

inline
CvMat::CvMat(int _rows, int _cols, int _type, void* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_rows), cols(_cols),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(&rows)
{
    CC_Assert(total() == 0 || data != NULL);

    size_t esz = CC_ELEM_SIZE(_type), esz1 = CC_ELEM_SIZE1(_type);
    size_t minstep = cols * esz;
    if( _step == AUTO_STEP )
    {
        _step = minstep;
        flags |= CONTINUOUS_FLAG;
    }
    else
    {
        if( rows == 1 ) _step = minstep;
        CC_DbgAssert( _step >= minstep );

        if (_step % esz1 != 0)
        {
            CC_Error(Error::BadStep, "Step must be a multiple of esz1");
        }

        flags |= _step == minstep ? CONTINUOUS_FLAG : 0;
    }
    step[0] = _step;
    step[1] = esz;
    datalimit = datastart + _step * rows;
    dataend = datalimit - _step + minstep;
}

inline
CvMat::CvMat(Size _sz, int _type, void* _data, size_t _step)
    : flags(MAGIC_VAL + (_type & TYPE_MASK)), dims(2), rows(_sz.height), cols(_sz.width),
      data((uchar*)_data), datastart((uchar*)_data), dataend(0), datalimit(0),
      allocator(0), u(0), size(&rows)
{
    CC_Assert(total() == 0 || data != NULL);

    size_t esz = CC_ELEM_SIZE(_type), esz1 = CC_ELEM_SIZE1(_type);
    size_t minstep = cols*esz;
    if( _step == AUTO_STEP )
    {
        _step = minstep;
        flags |= CONTINUOUS_FLAG;
    }
    else
    {
        if( rows == 1 ) _step = minstep;
        CC_DbgAssert( _step >= minstep );

        if (_step % esz1 != 0)
        {
            CC_Error(Error::BadStep, "Step must be a multiple of esz1");
        }

        flags |= _step == minstep ? CONTINUOUS_FLAG : 0;
    }
    step[0] = _step;
    step[1] = esz;
    datalimit = datastart + _step*rows;
    dataend = datalimit - _step + minstep;
}

template<typename _Tp> inline
CvMat::CvMat(const std::vector<_Tp>& vec, bool copyData)
    : flags(MAGIC_VAL | DataType<_Tp>::type | CC_MAT_CONT_FLAG), dims(2), rows((int)vec.size()),
      cols(1), data(0), datastart(0), dataend(0), allocator(0), u(0), size(&rows)
{
    if(vec.empty())
        return;
    if( !copyData )
    {
        step[0] = step[1] = sizeof(_Tp);
        datastart = data = (uchar*)&vec[0];
        datalimit = dataend = datastart + rows * step[0];
    }
    else
        CvMat((int)vec.size(), 1, DataType<_Tp>::type, (uchar*)&vec[0]).copyTo(*this);
}

template<typename _Tp, int n> inline
CvMat::CvMat(const Vec<_Tp, n>& vec, bool copyData)
    : flags(MAGIC_VAL | DataType<_Tp>::type | CC_MAT_CONT_FLAG), dims(2), rows(n), cols(1), data(0),
      datastart(0), dataend(0), allocator(0), u(0), size(&rows)
{
    if( !copyData )
    {
        step[0] = step[1] = sizeof(_Tp);
        datastart = data = (uchar*)vec.val;
        datalimit = dataend = datastart + rows * step[0];
    }
    else
        CvMat(n, 1, DataType<_Tp>::type, (void*)vec.val).copyTo(*this);
}


template<typename _Tp, int m, int n> inline
CvMat::CvMat(const Matx<_Tp,m,n>& M, bool copyData)
    : flags(MAGIC_VAL | DataType<_Tp>::type | CC_MAT_CONT_FLAG), dims(2), rows(m), cols(n), data(0),
      datastart(0), dataend(0), allocator(0), u(0), size(&rows)
{
    if( !copyData )
    {
        step[0] = cols * sizeof(_Tp);
        step[1] = sizeof(_Tp);
        datastart = data = (uchar*)M.val;
        datalimit = dataend = datastart + rows * step[0];
    }
    else
        CvMat(m, n, DataType<_Tp>::type, (uchar*)M.val).copyTo(*this);
}

template<typename _Tp> inline
CvMat::CvMat(const Point_<_Tp>& pt, bool copyData)
    : flags(MAGIC_VAL | DataType<_Tp>::type | CC_MAT_CONT_FLAG), dims(2), rows(2), cols(1), data(0),
      datastart(0), dataend(0), allocator(0), u(0), size(&rows)
{
    if( !copyData )
    {
        step[0] = step[1] = sizeof(_Tp);
        datastart = data = (uchar*)&pt.x;
        datalimit = dataend = datastart + rows * step[0];
    }
    else
    {
        create(2, 1, DataType<_Tp>::type);
        ((_Tp*)data)[0] = pt.x;
        ((_Tp*)data)[1] = pt.y;
    }
}

template<typename _Tp> inline
CvMat::CvMat(const Point3_<_Tp>& pt, bool copyData)
    : flags(MAGIC_VAL | DataType<_Tp>::type | CC_MAT_CONT_FLAG), dims(2), rows(3), cols(1), data(0),
      datastart(0), dataend(0), allocator(0), u(0), size(&rows)
{
    if( !copyData )
    {
        step[0] = step[1] = sizeof(_Tp);
        datastart = data = (uchar*)&pt.x;
        datalimit = dataend = datastart + rows * step[0];
    }
    else
    {
        create(3, 1, DataType<_Tp>::type);
        ((_Tp*)data)[0] = pt.x;
        ((_Tp*)data)[1] = pt.y;
        ((_Tp*)data)[2] = pt.z;
    }
}

template<typename _Tp> inline
CvMat::CvMat(const MatCommaInitializer_<_Tp>& commaInitializer)
    : flags(MAGIC_VAL | DataType<_Tp>::type | CC_MAT_CONT_FLAG), dims(0), rows(0), cols(0), data(0),
      datastart(0), dataend(0), allocator(0), u(0), size(&rows)
{
    *this = commaInitializer.operator Mat_<_Tp>();
}

inline
CvMat::~CvMat()
{
    release();
    if( step.p != step.buf )
        fastFree(step.p);
}

inline
CvMat& CvMat::operator = (const CvMat& m)
{
    if( this != &m )
    {
        if( m.u )
            CC_XADD(&m.u->refcount, 1);
        release();
        flags = m.flags;
        if( dims <= 2 && m.dims <= 2 )
        {
            dims = m.dims;
            rows = m.rows;
            cols = m.cols;
            step[0] = m.step[0];
            step[1] = m.step[1];
        }
        else
            copySize(m);
        data = m.data;
        datastart = m.datastart;
        dataend = m.dataend;
        datalimit = m.datalimit;
        allocator = m.allocator;
        u = m.u;
    }
    return *this;
}

inline
CvMat CvMat::row(int y) const
{
    return CvMat(*this, Range(y, y + 1), Range::all());
}

inline
CvMat CvMat::col(int x) const
{
    return CvMat(*this, Range::all(), Range(x, x + 1));
}

inline
CvMat CvMat::rowRange(int startrow, int endrow) const
{
    return CvMat(*this, Range(startrow, endrow), Range::all());
}

inline
CvMat CvMat::rowRange(const Range& r) const
{
    return CvMat(*this, r, Range::all());
}

inline
CvMat CvMat::colRange(int startcol, int endcol) const
{
    return CvMat(*this, Range::all(), Range(startcol, endcol));
}

inline
CvMat CvMat::colRange(const Range& r) const
{
    return CvMat(*this, Range::all(), r);
}

inline
CvMat CvMat::clone() const
{
    CvMat m;
    copyTo(m);
    return m;
}

inline
void CvMat::assignTo( CvMat& m, int _type ) const
{
    if( _type < 0 )
        m = *this;
    else
        convertTo(m, _type);
}

inline
void CvMat::create(int _rows, int _cols, int _type)
{
    _type &= TYPE_MASK;
    if( dims <= 2 && rows == _rows && cols == _cols && type() == _type && data )
        return;
    int sz[] = {_rows, _cols};
    create(2, sz, _type);
}

inline
void CvMat::create(Size _sz, int _type)
{
    create(_sz.height, _sz.width, _type);
}

inline
void CvMat::addref()
{
    if( u )
        CC_XADD(&u->refcount, 1);
}

inline
void CvMat::release()
{
    if( u && CC_XADD(&u->refcount, -1) == 1 )
        deallocate();
    u = NULL;
    datastart = dataend = datalimit = data = 0;
    for(int i = 0; i < dims; i++)
        size.p[i] = 0;
#ifdef _DEBUG
    flags = MAGIC_VAL;
    dims = rows = cols = 0;
    if(step.p != step.buf)
    {
        fastFree(step.p);
        step.p = step.buf;
        size.p = &rows;
    }
#endif
}

inline
CvMat CvMat::operator()( Range _rowRange, Range _colRange ) const
{
    return CvMat(*this, _rowRange, _colRange);
}

inline
CvMat CvMat::operator()( const Rect& roi ) const
{
    return CvMat(*this, roi);
}

inline
CvMat CvMat::operator()(const Range* ranges) const
{
    return CvMat(*this, ranges);
}

inline
CvMat CvMat::operator()(const std::vector<Range>& ranges) const
{
    return CvMat(*this, ranges);
}

inline
bool CvMat::isContinuous() const
{
    return (flags & CONTINUOUS_FLAG) != 0;
}

inline
bool CvMat::isSubmatrix() const
{
    return (flags & SUBMATRIX_FLAG) != 0;
}

inline
size_t CvMat::elemSize() const
{
    return dims > 0 ? step.p[dims - 1] : 0;
}

inline
size_t CvMat::elemSize1() const
{
    return CC_ELEM_SIZE1(flags);
}

inline
int CvMat::type() const
{
    return CC_MAT_TYPE(flags);
}

inline
int CvMat::depth() const
{
    return CC_MAT_DEPTH(flags);
}

inline
int CvMat::channels() const
{
    return CC_MAT_CN(flags);
}

inline
size_t CvMat::step1(int i) const
{
    return step.p[i] / elemSize1();
}

inline
bool CvMat::empty() const
{
    return data == 0 || total() == 0;
}

inline
size_t CvMat::total() const
{
    if( dims <= 2 )
        return (size_t)rows * cols;
    size_t p = 1;
    for( int i = 0; i < dims; i++ )
        p *= size[i];
    return p;
}

inline
uchar* CvMat::ptr(int y)
{
    CC_DbgAssert( y == 0 || (data && dims >= 1 && (unsigned)y < (unsigned)size.p[0]) );
    return data + step.p[0] * y;
}

inline
const uchar* CvMat::ptr(int y) const
{
    CC_DbgAssert( y == 0 || (data && dims >= 1 && (unsigned)y < (unsigned)size.p[0]) );
    return data + step.p[0] * y;
}

template<typename _Tp> inline
_Tp* CvMat::ptr(int y)
{
    CC_DbgAssert( y == 0 || (data && dims >= 1 && (unsigned)y < (unsigned)size.p[0]) );
    return (_Tp*)(data + step.p[0] * y);
}

template<typename _Tp> inline
const _Tp* CvMat::ptr(int y) const
{
    CC_DbgAssert( y == 0 || (data && dims >= 1 && data && (unsigned)y < (unsigned)size.p[0]) );
    return (const _Tp*)(data + step.p[0] * y);
}

inline
uchar* CvMat::ptr(int i0, int i1)
{
    CC_DbgAssert(dims >= 2);
    CC_DbgAssert(data);
    CC_DbgAssert((unsigned)i0 < (unsigned)size.p[0]);
    CC_DbgAssert((unsigned)i1 < (unsigned)size.p[1]);
    return data + i0 * step.p[0] + i1 * step.p[1];
}

inline
const uchar* CvMat::ptr(int i0, int i1) const
{
    CC_DbgAssert(dims >= 2);
    CC_DbgAssert(data);
    CC_DbgAssert((unsigned)i0 < (unsigned)size.p[0]);
    CC_DbgAssert((unsigned)i1 < (unsigned)size.p[1]);
    return data + i0 * step.p[0] + i1 * step.p[1];
}

template<typename _Tp> inline
_Tp* CvMat::ptr(int i0, int i1)
{
    CC_DbgAssert(dims >= 2);
    CC_DbgAssert(data);
    CC_DbgAssert((unsigned)i0 < (unsigned)size.p[0]);
    CC_DbgAssert((unsigned)i1 < (unsigned)size.p[1]);
    return (_Tp*)(data + i0 * step.p[0] + i1 * step.p[1]);
}

template<typename _Tp> inline
const _Tp* CvMat::ptr(int i0, int i1) const
{
    CC_DbgAssert(dims >= 2);
    CC_DbgAssert(data);
    CC_DbgAssert((unsigned)i0 < (unsigned)size.p[0]);
    CC_DbgAssert((unsigned)i1 < (unsigned)size.p[1]);
    return (const _Tp*)(data + i0 * step.p[0] + i1 * step.p[1]);
}

inline
uchar* CvMat::ptr(int i0, int i1, int i2)
{
    CC_DbgAssert(dims >= 3);
    CC_DbgAssert(data);
    CC_DbgAssert((unsigned)i0 < (unsigned)size.p[0]);
    CC_DbgAssert((unsigned)i1 < (unsigned)size.p[1]);
    CC_DbgAssert((unsigned)i2 < (unsigned)size.p[2]);
    return data + i0 * step.p[0] + i1 * step.p[1] + i2 * step.p[2];
}

inline
const uchar* CvMat::ptr(int i0, int i1, int i2) const
{
    CC_DbgAssert(dims >= 3);
    CC_DbgAssert(data);
    CC_DbgAssert((unsigned)i0 < (unsigned)size.p[0]);
    CC_DbgAssert((unsigned)i1 < (unsigned)size.p[1]);
    CC_DbgAssert((unsigned)i2 < (unsigned)size.p[2]);
    return data + i0 * step.p[0] + i1 * step.p[1] + i2 * step.p[2];
}

template<typename _Tp> inline
_Tp* CvMat::ptr(int i0, int i1, int i2)
{
    CC_DbgAssert(dims >= 3);
    CC_DbgAssert(data);
    CC_DbgAssert((unsigned)i0 < (unsigned)size.p[0]);
    CC_DbgAssert((unsigned)i1 < (unsigned)size.p[1]);
    CC_DbgAssert((unsigned)i2 < (unsigned)size.p[2]);
    return (_Tp*)(data + i0 * step.p[0] + i1 * step.p[1] + i2 * step.p[2]);
}

template<typename _Tp> inline
const _Tp* CvMat::ptr(int i0, int i1, int i2) const
{
    CC_DbgAssert(dims >= 3);
    CC_DbgAssert(data);
    CC_DbgAssert((unsigned)i0 < (unsigned)size.p[0]);
    CC_DbgAssert((unsigned)i1 < (unsigned)size.p[1]);
    CC_DbgAssert((unsigned)i2 < (unsigned)size.p[2]);
    return (const _Tp*)(data + i0 * step.p[0] + i1 * step.p[1] + i2 * step.p[2]);
}

inline
uchar* CvMat::ptr(const int* idx)
{
    int i, d = dims;
    uchar* p = data;
    CC_DbgAssert( d >= 1 && p );
    for( i = 0; i < d; i++ )
    {
        CC_DbgAssert( (unsigned)idx[i] < (unsigned)size.p[i] );
        p += idx[i] * step.p[i];
    }
    return p;
}

inline
const uchar* CvMat::ptr(const int* idx) const
{
    int i, d = dims;
    uchar* p = data;
    CC_DbgAssert( d >= 1 && p );
    for( i = 0; i < d; i++ )
    {
        CC_DbgAssert( (unsigned)idx[i] < (unsigned)size.p[i] );
        p += idx[i] * step.p[i];
    }
    return p;
}

template<typename _Tp> inline
_Tp& CvMat::at(int i0, int i1)
{
    CC_DbgAssert(dims <= 2);
    CC_DbgAssert(data);
    CC_DbgAssert((unsigned)i0 < (unsigned)size.p[0]);
    CC_DbgAssert((unsigned)(i1 * DataType<_Tp>::channels) < (unsigned)(size.p[1] * channels()));
    CC_DbgAssert(CC_ELEM_SIZE1(DataType<_Tp>::depth) == elemSize1());
    return ((_Tp*)(data + step.p[0] * i0))[i1];
}

template<typename _Tp> inline
const _Tp& CvMat::at(int i0, int i1) const
{
    CC_DbgAssert(dims <= 2);
    CC_DbgAssert(data);
    CC_DbgAssert((unsigned)i0 < (unsigned)size.p[0]);
    CC_DbgAssert((unsigned)(i1 * DataType<_Tp>::channels) < (unsigned)(size.p[1] * channels()));
    CC_DbgAssert(CC_ELEM_SIZE1(DataType<_Tp>::depth) == elemSize1());
    return ((const _Tp*)(data + step.p[0] * i0))[i1];
}

template<typename _Tp> inline
_Tp& CvMat::at(Point pt)
{
    CC_DbgAssert(dims <= 2);
    CC_DbgAssert(data);
    CC_DbgAssert((unsigned)pt.y < (unsigned)size.p[0]);
    CC_DbgAssert((unsigned)(pt.x * DataType<_Tp>::channels) < (unsigned)(size.p[1] * channels()));
    CC_DbgAssert(CC_ELEM_SIZE1(DataType<_Tp>::depth) == elemSize1());
    return ((_Tp*)(data + step.p[0] * pt.y))[pt.x];
}

template<typename _Tp> inline
const _Tp& CvMat::at(Point pt) const
{
    CC_DbgAssert(dims <= 2);
    CC_DbgAssert(data);
    CC_DbgAssert((unsigned)pt.y < (unsigned)size.p[0]);
    CC_DbgAssert((unsigned)(pt.x * DataType<_Tp>::channels) < (unsigned)(size.p[1] * channels()));
    CC_DbgAssert(CC_ELEM_SIZE1(DataType<_Tp>::depth) == elemSize1());
    return ((const _Tp*)(data + step.p[0] * pt.y))[pt.x];
}

template<typename _Tp> inline
_Tp& CvMat::at(int i0)
{
    CC_DbgAssert(dims <= 2);
    CC_DbgAssert(data);
    CC_DbgAssert((unsigned)i0 < (unsigned)(size.p[0] * size.p[1]));
    CC_DbgAssert(elemSize() == CC_ELEM_SIZE(DataType<_Tp>::type));
    if( isContinuous() || size.p[0] == 1 )
        return ((_Tp*)data)[i0];
    if( size.p[1] == 1 )
        return *(_Tp*)(data + step.p[0] * i0);
    int i = i0 / cols, j = i0 - i * cols;
    return ((_Tp*)(data + step.p[0] * i))[j];
}

template<typename _Tp> inline
const _Tp& CvMat::at(int i0) const
{
    CC_DbgAssert(dims <= 2);
    CC_DbgAssert(data);
    CC_DbgAssert((unsigned)i0 < (unsigned)(size.p[0] * size.p[1]));
    CC_DbgAssert(elemSize() == CC_ELEM_SIZE(DataType<_Tp>::type));
    if( isContinuous() || size.p[0] == 1 )
        return ((const _Tp*)data)[i0];
    if( size.p[1] == 1 )
        return *(const _Tp*)(data + step.p[0] * i0);
    int i = i0 / cols, j = i0 - i * cols;
    return ((const _Tp*)(data + step.p[0] * i))[j];
}

template<typename _Tp> inline
_Tp& CvMat::at(int i0, int i1, int i2)
{
    CC_DbgAssert( elemSize() == CC_ELEM_SIZE(DataType<_Tp>::type) );
    return *(_Tp*)ptr(i0, i1, i2);
}

template<typename _Tp> inline
const _Tp& CvMat::at(int i0, int i1, int i2) const
{
    CC_DbgAssert( elemSize() == CC_ELEM_SIZE(DataType<_Tp>::type) );
    return *(const _Tp*)ptr(i0, i1, i2);
}

template<typename _Tp> inline
_Tp& CvMat::at(const int* idx)
{
    CC_DbgAssert( elemSize() == CC_ELEM_SIZE(DataType<_Tp>::type) );
    return *(_Tp*)ptr(idx);
}

template<typename _Tp> inline
const _Tp& CvMat::at(const int* idx) const
{
    CC_DbgAssert( elemSize() == CC_ELEM_SIZE(DataType<_Tp>::type) );
    return *(const _Tp*)ptr(idx);
}

template<typename _Tp, int n> inline
_Tp& CvMat::at(const Vec<int, n>& idx)
{
    CC_DbgAssert( elemSize() == CC_ELEM_SIZE(DataType<_Tp>::type) );
    return *(_Tp*)ptr(idx.val);
}

template<typename _Tp, int n> inline
const _Tp& CvMat::at(const Vec<int, n>& idx) const
{
    CC_DbgAssert( elemSize() == CC_ELEM_SIZE(DataType<_Tp>::type) );
    return *(const _Tp*)ptr(idx.val);
}

template<typename _Tp> inline
MatConstIterator_<_Tp> CvMat::begin() const
{
    CC_DbgAssert( elemSize() == sizeof(_Tp) );
    return MatConstIterator_<_Tp>((const Mat_<_Tp>*)this);
}

template<typename _Tp> inline
MatConstIterator_<_Tp> CvMat::end() const
{
    CC_DbgAssert( elemSize() == sizeof(_Tp) );
    MatConstIterator_<_Tp> it((const Mat_<_Tp>*)this);
    it += total();
    return it;
}

template<typename _Tp> inline
MatIterator_<_Tp> CvMat::begin()
{
    CC_DbgAssert( elemSize() == sizeof(_Tp) );
    return MatIterator_<_Tp>((Mat_<_Tp>*)this);
}

template<typename _Tp> inline
MatIterator_<_Tp> CvMat::end()
{
    CC_DbgAssert( elemSize() == sizeof(_Tp) );
    MatIterator_<_Tp> it((Mat_<_Tp>*)this);
    it += total();
    return it;
}

template<typename _Tp, typename Functor> inline
void CvMat::forEach(const Functor& operation) {
    this->forEach_impl<_Tp>(operation);
}

template<typename _Tp, typename Functor> inline
void CvMat::forEach(const Functor& operation) const {
    // call as not const
    (const_cast<CvMat*>(this))->forEach<const _Tp>(operation);
}

template<typename _Tp> inline
CvMat::operator std::vector<_Tp>() const
{
    std::vector<_Tp> v;
    copyTo(v);
    return v;
}

template<typename _Tp, int n> inline
CvMat::operator Vec<_Tp, n>() const
{
    CC_Assert( data && dims <= 2 && (rows == 1 || cols == 1) &&
               rows + cols - 1 == n && channels() == 1 );

    if( isContinuous() && type() == DataType<_Tp>::type )
        return Vec<_Tp, n>((_Tp*)data);
    Vec<_Tp, n> v;
    CvMat tmp(rows, cols, DataType<_Tp>::type, v.val);
    convertTo(tmp, tmp CC_MAT_TYPE());
    return v;
}

template<typename _Tp, int m, int n> inline
CvMat::operator Matx<_Tp, m, n>() const
{
    CC_Assert( data && dims <= 2 && rows == m && cols == n && channels() == 1 );

    if( isContinuous() && type() == DataType<_Tp>::type )
        return Matx<_Tp, m, n>((_Tp*)data);
    Matx<_Tp, m, n> mtx;
    CvMat tmp(rows, cols, DataType<_Tp>::type, mtx.val);
    convertTo(tmp, tmp CC_MAT_TYPE());
    return mtx;
}

template<typename _Tp> inline
void CvMat::push_back(const _Tp& elem)
{
    if( !data )
    {
        *this = CvMat(1, 1, DataType<_Tp>::type, (void*)&elem).clone();
        return;
    }
    CC_Assert(DataType<_Tp>::type == type() && cols == 1
              /* && dims == 2 (cols == 1 implies dims == 2) */);
    const uchar* tmp = dataend + step[0];
    if( !isSubmatrix() && isContinuous() && tmp <= datalimit )
    {
        *(_Tp*)(data + (size.p[0]++) * step.p[0]) = elem;
        dataend = tmp;
    }
    else
        push_back_(&elem);
}

template<typename _Tp> inline
void CvMat::push_back(const Mat_<_Tp>& m)
{
    push_back((const CvMat&)m);
}

template<> inline
void CvMat::push_back(const MatExpr& expr)
{
    push_back(static_cast<CvMat>(expr));
}

#ifdef CC_CXX_MOVE_SEMANTICS

inline
CvMat::CvMat(CvMat&& m)
    : flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), data(m.data),
      datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit), allocator(m.allocator),
      u(m.u), size(&rows)
{
    if (m.dims <= 2)  // move new step/size info
    {
        step[0] = m.step[0];
        step[1] = m.step[1];
    }
    else
    {
        CC_DbgAssert(m.step.p != m.step.buf);
        step.p = m.step.p;
        size.p = m.size.p;
        m.step.p = m.step.buf;
        m.size.p = &m.rows;
    }
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.data = NULL; m.datastart = NULL; m.dataend = NULL; m.datalimit = NULL;
    m.allocator = NULL;
    m.u = NULL;
}

inline
CvMat& CvMat::operator = (CvMat&& m)
{
    if (this == &m)
      return *this;

    release();
    flags = m.flags; dims = m.dims; rows = m.rows; cols = m.cols; data = m.data;
    datastart = m.datastart; dataend = m.dataend; datalimit = m.datalimit; allocator = m.allocator;
    u = m.u;
    if (step.p != step.buf) // release self step/size
    {
        fastFree(step.p);
        step.p = step.buf;
        size.p = &rows;
    }
    if (m.dims <= 2) // move new step/size info
    {
        step[0] = m.step[0];
        step[1] = m.step[1];
    }
    else
    {
        CC_DbgAssert(m.step.p != m.step.buf);
        step.p = m.step.p;
        size.p = m.size.p;
        m.step.p = m.step.buf;
        m.size.p = &m.rows;
    }
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.data = NULL; m.datastart = NULL; m.dataend = NULL; m.datalimit = NULL;
    m.allocator = NULL;
    m.u = NULL;
    return *this;
}

#endif


///////////////////////////// MatSize ////////////////////////////

inline
MatSize::MatSize(int* _p)
    : p(_p) {}

inline
Size MatSize::operator()() const
{
    CC_DbgAssert(p[-1] <= 2);
    return Size(p[1], p[0]);
}

inline
const int& MatSize::operator[](int i) const
{
    return p[i];
}

inline
int& MatSize::operator[](int i)
{
    return p[i];
}

inline
MatSize::operator const int*() const
{
    return p;
}

inline
bool MatSize::operator == (const MatSize& sz) const
{
    int d = p[-1];
    int dsz = sz.p[-1];
    if( d != dsz )
        return false;
    if( d == 2 )
        return p[0] == sz.p[0] && p[1] == sz.p[1];

    for( int i = 0; i < d; i++ )
        if( p[i] != sz.p[i] )
            return false;
    return true;
}

inline
bool MatSize::operator != (const MatSize& sz) const
{
    return !(*this == sz);
}



///////////////////////////// MatStep ////////////////////////////

inline
MatStep::MatStep()
{
    p = buf; p[0] = p[1] = 0;
}

inline
MatStep::MatStep(size_t s)
{
    p = buf; p[0] = s; p[1] = 0;
}

inline
const size_t& MatStep::operator[](int i) const
{
    return p[i];
}

inline
size_t& MatStep::operator[](int i)
{
    return p[i];
}

inline MatStep::operator size_t() const
{
    CC_DbgAssert( p == buf );
    return buf[0];
}

inline MatStep& MatStep::operator = (size_t s)
{
    CC_DbgAssert( p == buf );
    buf[0] = s;
    return *this;
}



////////////////////////////// Mat_<_Tp> ////////////////////////////

template<typename _Tp> inline
Mat_<_Tp>::Mat_()
    : CvMat()
{
    flags = (flags & ~CC_MAT_TYPE_MASK) | DataType<_Tp>::type;
}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(int _rows, int _cols)
    : CvMat(_rows, _cols, DataType<_Tp>::type)
{
}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(int _rows, int _cols, const _Tp& value)
    : CvMat(_rows, _cols, DataType<_Tp>::type)
{
    *this = value;
}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(Size _sz)
    : CvMat(_sz.height, _sz.width, DataType<_Tp>::type)
{}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(Size _sz, const _Tp& value)
    : CvMat(_sz.height, _sz.width, DataType<_Tp>::type)
{
    *this = value;
}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(int _dims, const int* _sz)
    : CvMat(_dims, _sz, DataType<_Tp>::type)
{}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(int _dims, const int* _sz, const _Tp& _s)
    : CvMat(_dims, _sz, DataType<_Tp>::type, Scalar(_s))
{}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(int _dims, const int* _sz, _Tp* _data, const size_t* _steps)
    : CvMat(_dims, _sz, DataType<_Tp>::type, _data, _steps)
{}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(const Mat_<_Tp>& m, const Range* ranges)
    : CvMat(m, ranges)
{}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(const Mat_<_Tp>& m, const std::vector<Range>& ranges)
    : CvMat(m, ranges)
{}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(const CvMat& m)
    : CvMat()
{
    flags = (flags & ~CC_MAT_TYPE_MASK) | DataType<_Tp>::type;
    *this = m;
}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(const Mat_& m)
    : CvMat(m)
{}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(int _rows, int _cols, _Tp* _data, size_t steps)
    : CvMat(_rows, _cols, DataType<_Tp>::type, _data, steps)
{}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(const Mat_& m, const Range& _rowRange, const Range& _colRange)
    : CvMat(m, _rowRange, _colRange)
{}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(const Mat_& m, const Rect& roi)
    : CvMat(m, roi)
{}

template<typename _Tp> template<int n> inline
Mat_<_Tp>::Mat_(const Vec<typename DataType<_Tp>::channel_type, n>& vec, bool copyData)
    : CvMat(n / DataType<_Tp>::channels, 1, DataType<_Tp>::type, (void*)&vec)
{
    CC_Assert(n%DataType<_Tp>::channels == 0);
    if( copyData )
        *this = clone();
}

template<typename _Tp> template<int m, int n> inline
Mat_<_Tp>::Mat_(const Matx<typename DataType<_Tp>::channel_type, m, n>& M, bool copyData)
    : CvMat(m, n / DataType<_Tp>::channels, DataType<_Tp>::type, (void*)&M)
{
    CC_Assert(n % DataType<_Tp>::channels == 0);
    if( copyData )
        *this = clone();
}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(const Point_<typename DataType<_Tp>::channel_type>& pt, bool copyData)
    : CvMat(2 / DataType<_Tp>::channels, 1, DataType<_Tp>::type, (void*)&pt)
{
    CC_Assert(2 % DataType<_Tp>::channels == 0);
    if( copyData )
        *this = clone();
}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(const Point3_<typename DataType<_Tp>::channel_type>& pt, bool copyData)
    : CvMat(3 / DataType<_Tp>::channels, 1, DataType<_Tp>::type, (void*)&pt)
{
    CC_Assert(3 % DataType<_Tp>::channels == 0);
    if( copyData )
        *this = clone();
}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(const MatCommaInitializer_<_Tp>& commaInitializer)
    : CvMat(commaInitializer)
{}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(const std::vector<_Tp>& vec, bool copyData)
    : CvMat(vec, copyData)
{}

template<typename _Tp> inline
Mat_<_Tp>& Mat_<_Tp>::operator = (const CvMat& m)
{
    if( DataType<_Tp>::type == m CC_MAT_TYPE() )
    {
        CvMat::operator = (m);
        return *this;
    }
    if( DataType<_Tp>::depth == m.depth() )
    {
        return (*this = m.reshape(DataType<_Tp>::channels, m.dims, 0));
    }
    CC_DbgAssert(DataType<_Tp>::channels == m.channels());
    m.convertTo(*this, type());
    return *this;
}

template<typename _Tp> inline
Mat_<_Tp>& Mat_<_Tp>::operator = (const Mat_& m)
{
    CvMat::operator=(m);
    return *this;
}

template<typename _Tp> inline
Mat_<_Tp>& Mat_<_Tp>::operator = (const _Tp& s)
{
    typedef typename DataType<_Tp>::vec_type VT;
    CvMat::operator=(Scalar((const VT&)s));
    return *this;
}

template<typename _Tp> inline
void Mat_<_Tp>::create(int _rows, int _cols)
{
    CvMat::create(_rows, _cols, DataType<_Tp>::type);
}

template<typename _Tp> inline
void Mat_<_Tp>::create(Size _sz)
{
    CvMat::create(_sz, DataType<_Tp>::type);
}

template<typename _Tp> inline
void Mat_<_Tp>::create(int _dims, const int* _sz)
{
    CvMat::create(_dims, _sz, DataType<_Tp>::type);
}

template<typename _Tp> inline
Mat_<_Tp> Mat_<_Tp>::cross(const Mat_& m) const
{
    return Mat_<_Tp>(CvMat::cross(m));
}

template<typename _Tp> template<typename T2> inline
Mat_<_Tp>::operator Mat_<T2>() const
{
    return Mat_<T2>(*this);
}

template<typename _Tp> inline
Mat_<_Tp> Mat_<_Tp>::row(int y) const
{
    return Mat_(*this, Range(y, y+1), Range::all());
}

template<typename _Tp> inline
Mat_<_Tp> Mat_<_Tp>::col(int x) const
{
    return Mat_(*this, Range::all(), Range(x, x+1));
}

template<typename _Tp> inline
Mat_<_Tp> Mat_<_Tp>::diag(int d) const
{
    return Mat_(CvMat::diag(d));
}

template<typename _Tp> inline
Mat_<_Tp> Mat_<_Tp>::clone() const
{
    return Mat_(CvMat::clone());
}

template<typename _Tp> inline
size_t Mat_<_Tp>::elemSize() const
{
    CC_DbgAssert( CvMat::elemSize() == sizeof(_Tp) );
    return sizeof(_Tp);
}

template<typename _Tp> inline
size_t Mat_<_Tp>::elemSize1() const
{
    CC_DbgAssert( CvMat::elemSize1() == sizeof(_Tp) / DataType<_Tp>::channels );
    return sizeof(_Tp) / DataType<_Tp>::channels;
}

template<typename _Tp> inline
int Mat_<_Tp>::type() const
{
    CC_DbgAssert( CvMat::type() == DataType<_Tp>::type );
    return DataType<_Tp>::type;
}

template<typename _Tp> inline
int Mat_<_Tp>::depth() const
{
    CC_DbgAssert( CvMat::depth() == DataType<_Tp>::depth );
    return DataType<_Tp>::depth;
}

template<typename _Tp> inline
int Mat_<_Tp>::channels() const
{
    CC_DbgAssert( CvMat::channels() == DataType<_Tp>::channels );
    return DataType<_Tp>::channels;
}

template<typename _Tp> inline
size_t Mat_<_Tp>::stepT(int i) const
{
    return step.p[i] / elemSize();
}

template<typename _Tp> inline
size_t Mat_<_Tp>::step1(int i) const
{
    return step.p[i] / elemSize1();
}

template<typename _Tp> inline
Mat_<_Tp>& Mat_<_Tp>::adjustROI( int dtop, int dbottom, int dleft, int dright )
{
    return (Mat_<_Tp>&)(CvMat::adjustROI(dtop, dbottom, dleft, dright));
}

template<typename _Tp> inline
Mat_<_Tp> Mat_<_Tp>::operator()( const Range& _rowRange, const Range& _colRange ) const
{
    return Mat_<_Tp>(*this, _rowRange, _colRange);
}

template<typename _Tp> inline
Mat_<_Tp> Mat_<_Tp>::operator()( const Rect& roi ) const
{
    return Mat_<_Tp>(*this, roi);
}

template<typename _Tp> inline
Mat_<_Tp> Mat_<_Tp>::operator()( const Range* ranges ) const
{
    return Mat_<_Tp>(*this, ranges);
}

template<typename _Tp> inline
Mat_<_Tp> Mat_<_Tp>::operator()(const std::vector<Range>& ranges) const
{
    return Mat_<_Tp>(*this, ranges);
}

template<typename _Tp> inline
_Tp* Mat_<_Tp>::operator [](int y)
{
    CC_DbgAssert( 0 <= y && y < rows );
    return (_Tp*)(data + y*step.p[0]);
}

template<typename _Tp> inline
const _Tp* Mat_<_Tp>::operator [](int y) const
{
    CC_DbgAssert( 0 <= y && y < rows );
    return (const _Tp*)(data + y*step.p[0]);
}

template<typename _Tp> inline
_Tp& Mat_<_Tp>::operator ()(int i0, int i1)
{
    CC_DbgAssert(dims <= 2);
    CC_DbgAssert(data);
    CC_DbgAssert((unsigned)i0 < (unsigned)size.p[0]);
    CC_DbgAssert((unsigned)i1 < (unsigned)size.p[1]);
    CC_DbgAssert(type() == DataType<_Tp>::type);
    return ((_Tp*)(data + step.p[0] * i0))[i1];
}

template<typename _Tp> inline
const _Tp& Mat_<_Tp>::operator ()(int i0, int i1) const
{
    CC_DbgAssert(dims <= 2);
    CC_DbgAssert(data);
    CC_DbgAssert((unsigned)i0 < (unsigned)size.p[0]);
    CC_DbgAssert((unsigned)i1 < (unsigned)size.p[1]);
    CC_DbgAssert(type() == DataType<_Tp>::type);
    return ((const _Tp*)(data + step.p[0] * i0))[i1];
}

template<typename _Tp> inline
_Tp& Mat_<_Tp>::operator ()(Point pt)
{
    CC_DbgAssert(dims <= 2);
    CC_DbgAssert(data);
    CC_DbgAssert((unsigned)pt.y < (unsigned)size.p[0]);
    CC_DbgAssert((unsigned)pt.x < (unsigned)size.p[1]);
    CC_DbgAssert(type() == DataType<_Tp>::type);
    return ((_Tp*)(data + step.p[0] * pt.y))[pt.x];
}

template<typename _Tp> inline
const _Tp& Mat_<_Tp>::operator ()(Point pt) const
{
    CC_DbgAssert(dims <= 2);
    CC_DbgAssert(data);
    CC_DbgAssert((unsigned)pt.y < (unsigned)size.p[0]);
    CC_DbgAssert((unsigned)pt.x < (unsigned)size.p[1]);
    CC_DbgAssert(type() == DataType<_Tp>::type);
    return ((const _Tp*)(data + step.p[0] * pt.y))[pt.x];
}

template<typename _Tp> inline
_Tp& Mat_<_Tp>::operator ()(const int* idx)
{
    return CvMat::at<_Tp>(idx);
}

template<typename _Tp> inline
const _Tp& Mat_<_Tp>::operator ()(const int* idx) const
{
    return CvMat::at<_Tp>(idx);
}

template<typename _Tp> template<int n> inline
_Tp& Mat_<_Tp>::operator ()(const Vec<int, n>& idx)
{
    return CvMat::at<_Tp>(idx);
}

template<typename _Tp> template<int n> inline
const _Tp& Mat_<_Tp>::operator ()(const Vec<int, n>& idx) const
{
    return CvMat::at<_Tp>(idx);
}

template<typename _Tp> inline
_Tp& Mat_<_Tp>::operator ()(int i0)
{
    return this->at<_Tp>(i0);
}

template<typename _Tp> inline
const _Tp& Mat_<_Tp>::operator ()(int i0) const
{
    return this->at<_Tp>(i0);
}

template<typename _Tp> inline
_Tp& Mat_<_Tp>::operator ()(int i0, int i1, int i2)
{
    return this->at<_Tp>(i0, i1, i2);
}

template<typename _Tp> inline
const _Tp& Mat_<_Tp>::operator ()(int i0, int i1, int i2) const
{
    return this->at<_Tp>(i0, i1, i2);
}

template<typename _Tp> inline
Mat_<_Tp>::operator std::vector<_Tp>() const
{
    std::vector<_Tp> v;
    copyTo(v);
    return v;
}

template<typename _Tp> template<int n> inline
Mat_<_Tp>::operator Vec<typename DataType<_Tp>::channel_type, n>() const
{
    CC_Assert(n % DataType<_Tp>::channels == 0);

#if defined _MSC_VER
    const CvMat* pMat = (const CvMat*)this; // workaround for MSVS <= 2012 compiler bugs (but GCC 4.6 dislikes this workaround)
    return pMat->operator Vec<typename DataType<_Tp>::channel_type, n>();
#else
    return this->CvMat::operator Vec<typename DataType<_Tp>::channel_type, n>();
#endif
}

template<typename _Tp> template<int m, int n> inline
Mat_<_Tp>::operator Matx<typename DataType<_Tp>::channel_type, m, n>() const
{
    CC_Assert(n % DataType<_Tp>::channels == 0);

#if defined _MSC_VER
    const CvMat* pMat = (const CvMat*)this; // workaround for MSVS <= 2012 compiler bugs (but GCC 4.6 dislikes this workaround)
    Matx<typename DataType<_Tp>::channel_type, m, n> res = pMat->operator Matx<typename DataType<_Tp>::channel_type, m, n>();
    return res;
#else
    Matx<typename DataType<_Tp>::channel_type, m, n> res = this->CvMat::operator Matx<typename DataType<_Tp>::channel_type, m, n>();
    return res;
#endif
}

template<typename _Tp> inline
MatConstIterator_<_Tp> Mat_<_Tp>::begin() const
{
    return CvMat::begin<_Tp>();
}

template<typename _Tp> inline
MatConstIterator_<_Tp> Mat_<_Tp>::end() const
{
    return CvMat::end<_Tp>();
}

template<typename _Tp> inline
MatIterator_<_Tp> Mat_<_Tp>::begin()
{
    return CvMat::begin<_Tp>();
}

template<typename _Tp> inline
MatIterator_<_Tp> Mat_<_Tp>::end()
{
    return CvMat::end<_Tp>();
}

template<typename _Tp> template<typename Functor> inline
void Mat_<_Tp>::forEach(const Functor& operation) {
    CvMat::forEach<_Tp, Functor>(operation);
}

template<typename _Tp> template<typename Functor> inline
void Mat_<_Tp>::forEach(const Functor& operation) const {
    CvMat::forEach<_Tp, Functor>(operation);
}

#ifdef CC_CXX_MOVE_SEMANTICS

template<typename _Tp> inline
Mat_<_Tp>::Mat_(Mat_&& m)
    : CvMat(m)
{
}

template<typename _Tp> inline
Mat_<_Tp>& Mat_<_Tp>::operator = (Mat_&& m)
{
    CvMat::operator = (m);
    return *this;
}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(CvMat&& m)
    : CvMat()
{
    flags = (flags & ~CC_MAT_TYPE_MASK) | DataType<_Tp>::type;
    *this = m;
}

template<typename _Tp> inline
Mat_<_Tp>& Mat_<_Tp>::operator = (CvMat&& m)
{
    if( DataType<_Tp>::type == m CC_MAT_TYPE() )
    {
        CvMat::operator = ((CvMat&&)m);
        return *this;
    }
    if( DataType<_Tp>::depth == m.depth() )
    {
        CvMat::operator = ((CvMat&&)m.reshape(DataType<_Tp>::channels, m.dims, 0));
        return *this;
    }
    CC_DbgAssert(DataType<_Tp>::channels == m.channels());
    m.convertTo(*this, type());
    return *this;
}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(MatExpr&& e)
    : CvMat()
{
    flags = (flags & ~CC_MAT_TYPE_MASK) | DataType<_Tp>::type;
    *this = CvMat(e);
}

#endif

///////////////////////////// SparseMat /////////////////////////////

inline
SparseMat::SparseMat()
    : flags(MAGIC_VAL), hdr(0)
{}

inline
SparseMat::SparseMat(int _dims, const int* _sizes, int _type)
    : flags(MAGIC_VAL), hdr(0)
{
    create(_dims, _sizes, _type);
}

inline
SparseMat::SparseMat(const SparseMat& m)
    : flags(m.flags), hdr(m.hdr)
{
    addref();
}

inline
SparseMat::~SparseMat()
{
    release();
}

inline
SparseMat& SparseMat::operator = (const SparseMat& m)
{
    if( this != &m )
    {
        if( m.hdr )
            CC_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    return *this;
}

inline
SparseMat& SparseMat::operator = (const CvMat& m)
{
    return (*this = SparseMat(m));
}

inline
SparseMat SparseMat::clone() const
{
    SparseMat temp;
    this->copyTo(temp);
    return temp;
}

inline
void SparseMat::assignTo( SparseMat& m, int _type ) const
{
    if( _type < 0 )
        m = *this;
    else
        convertTo(m, _type);
}

inline
void SparseMat::addref()
{
    if( hdr )
        CC_XADD(&hdr->refcount, 1);
}

inline
void SparseMat::release()
{
    if( hdr && CC_XADD(&hdr->refcount, -1) == 1 )
        delete hdr;
    hdr = 0;
}

inline
size_t SparseMat::elemSize() const
{
    return CC_ELEM_SIZE(flags);
}

inline
size_t SparseMat::elemSize1() const
{
    return CC_ELEM_SIZE1(flags);
}

inline
int SparseMat::type() const
{
    return CC_MAT_TYPE(flags);
}

inline
int SparseMat::depth() const
{
    return CC_MAT_DEPTH(flags);
}

inline
int SparseMat::channels() const
{
    return CC_MAT_CN(flags);
}

inline
const int* SparseMat::size() const
{
    return hdr ? hdr->size : 0;
}

inline
int SparseMat::size(int i) const
{
    if( hdr )
    {
        CC_DbgAssert((unsigned)i < (unsigned)hdr->dims);
        return hdr->size[i];
    }
    return 0;
}

inline
int SparseMat::dims() const
{
    return hdr ? hdr->dims : 0;
}

inline
size_t SparseMat::nzcount() const
{
    return hdr ? hdr->nodeCount : 0;
}

inline
size_t SparseMat::hash(int i0) const
{
    return (size_t)i0;
}

inline
size_t SparseMat::hash(int i0, int i1) const
{
    return (size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1;
}

inline
size_t SparseMat::hash(int i0, int i1, int i2) const
{
    return ((size_t)(unsigned)i0 * HASH_SCALE + (unsigned)i1) * HASH_SCALE + (unsigned)i2;
}

inline
size_t SparseMat::hash(const int* idx) const
{
    size_t h = (unsigned)idx[0];
    if( !hdr )
        return 0;
    int d = hdr->dims;
    for(int i = 1; i < d; i++ )
        h = h * HASH_SCALE + (unsigned)idx[i];
    return h;
}

template<typename _Tp> inline
_Tp& SparseMat::ref(int i0, size_t* hashval)
{
    return *(_Tp*)((SparseMat*)this)->ptr(i0, true, hashval);
}

template<typename _Tp> inline
_Tp& SparseMat::ref(int i0, int i1, size_t* hashval)
{
    return *(_Tp*)((SparseMat*)this)->ptr(i0, i1, true, hashval);
}

template<typename _Tp> inline
_Tp& SparseMat::ref(int i0, int i1, int i2, size_t* hashval)
{
    return *(_Tp*)((SparseMat*)this)->ptr(i0, i1, i2, true, hashval);
}

template<typename _Tp> inline
_Tp& SparseMat::ref(const int* idx, size_t* hashval)
{
    return *(_Tp*)((SparseMat*)this)->ptr(idx, true, hashval);
}

template<typename _Tp> inline
_Tp SparseMat::value(int i0, size_t* hashval) const
{
    const _Tp* p = (const _Tp*)((SparseMat*)this)->ptr(i0, false, hashval);
    return p ? *p : _Tp();
}

template<typename _Tp> inline
_Tp SparseMat::value(int i0, int i1, size_t* hashval) const
{
    const _Tp* p = (const _Tp*)((SparseMat*)this)->ptr(i0, i1, false, hashval);
    return p ? *p : _Tp();
}

template<typename _Tp> inline
_Tp SparseMat::value(int i0, int i1, int i2, size_t* hashval) const
{
    const _Tp* p = (const _Tp*)((SparseMat*)this)->ptr(i0, i1, i2, false, hashval);
    return p ? *p : _Tp();
}

template<typename _Tp> inline
_Tp SparseMat::value(const int* idx, size_t* hashval) const
{
    const _Tp* p = (const _Tp*)((SparseMat*)this)->ptr(idx, false, hashval);
    return p ? *p : _Tp();
}

template<typename _Tp> inline
const _Tp* SparseMat::find(int i0, size_t* hashval) const
{
    return (const _Tp*)((SparseMat*)this)->ptr(i0, false, hashval);
}

template<typename _Tp> inline
const _Tp* SparseMat::find(int i0, int i1, size_t* hashval) const
{
    return (const _Tp*)((SparseMat*)this)->ptr(i0, i1, false, hashval);
}

template<typename _Tp> inline
const _Tp* SparseMat::find(int i0, int i1, int i2, size_t* hashval) const
{
    return (const _Tp*)((SparseMat*)this)->ptr(i0, i1, i2, false, hashval);
}

template<typename _Tp> inline
const _Tp* SparseMat::find(const int* idx, size_t* hashval) const
{
    return (const _Tp*)((SparseMat*)this)->ptr(idx, false, hashval);
}

template<typename _Tp> inline
_Tp& SparseMat::value(Node* n)
{
    return *(_Tp*)((uchar*)n + hdr->valueOffset);
}

template<typename _Tp> inline
const _Tp& SparseMat::value(const Node* n) const
{
    return *(const _Tp*)((const uchar*)n + hdr->valueOffset);
}

inline
SparseMat::Node* SparseMat::node(size_t nidx)
{
    return (Node*)(void*)&hdr->pool[nidx];
}

inline
const SparseMat::Node* SparseMat::node(size_t nidx) const
{
    return (const Node*)(const void*)&hdr->pool[nidx];
}

inline
SparseMatIterator SparseMat::begin()
{
    return SparseMatIterator(this);
}

inline
SparseMatConstIterator SparseMat::begin() const
{
    return SparseMatConstIterator(this);
}

inline
SparseMatIterator SparseMat::end()
{
    SparseMatIterator it(this);
    it.seekEnd();
    return it;
}

inline
SparseMatConstIterator SparseMat::end() const
{
    SparseMatConstIterator it(this);
    it.seekEnd();
    return it;
}

template<typename _Tp> inline
SparseMatIterator_<_Tp> SparseMat::begin()
{
    return SparseMatIterator_<_Tp>(this);
}

template<typename _Tp> inline
SparseMatConstIterator_<_Tp> SparseMat::begin() const
{
    return SparseMatConstIterator_<_Tp>(this);
}

template<typename _Tp> inline
SparseMatIterator_<_Tp> SparseMat::end()
{
    SparseMatIterator_<_Tp> it(this);
    it.seekEnd();
    return it;
}

template<typename _Tp> inline
SparseMatConstIterator_<_Tp> SparseMat::end() const
{
    SparseMatConstIterator_<_Tp> it(this);
    it.seekEnd();
    return it;
}



///////////////////////////// SparseMat_ ////////////////////////////

template<typename _Tp> inline
SparseMat_<_Tp>::SparseMat_()
{
    flags = MAGIC_VAL | DataType<_Tp>::type;
}

template<typename _Tp> inline
SparseMat_<_Tp>::SparseMat_(int _dims, const int* _sizes)
    : SparseMat(_dims, _sizes, DataType<_Tp>::type)
{}

template<typename _Tp> inline
SparseMat_<_Tp>::SparseMat_(const SparseMat& m)
{
    if( m CC_MAT_TYPE() == DataType<_Tp>::type )
        *this = (const SparseMat_<_Tp>&)m;
    else
        m.convertTo(*this, DataType<_Tp>::type);
}

template<typename _Tp> inline
SparseMat_<_Tp>::SparseMat_(const SparseMat_<_Tp>& m)
{
    this->flags = m.flags;
    this->hdr = m.hdr;
    if( this->hdr )
        CC_XADD(&this->hdr->refcount, 1);
}

template<typename _Tp> inline
SparseMat_<_Tp>::SparseMat_(const CvMat& m)
{
    SparseMat sm(m);
    *this = sm;
}

template<typename _Tp> inline
SparseMat_<_Tp>& SparseMat_<_Tp>::operator = (const SparseMat_<_Tp>& m)
{
    if( this != &m )
    {
        if( m.hdr ) CC_XADD(&m.hdr->refcount, 1);
        release();
        flags = m.flags;
        hdr = m.hdr;
    }
    return *this;
}

template<typename _Tp> inline
SparseMat_<_Tp>& SparseMat_<_Tp>::operator = (const SparseMat& m)
{
    if( m CC_MAT_TYPE() == DataType<_Tp>::type )
        return (*this = (const SparseMat_<_Tp>&)m);
    m.convertTo(*this, DataType<_Tp>::type);
    return *this;
}

template<typename _Tp> inline
SparseMat_<_Tp>& SparseMat_<_Tp>::operator = (const CvMat& m)
{
    return (*this = SparseMat(m));
}

template<typename _Tp> inline
SparseMat_<_Tp> SparseMat_<_Tp>::clone() const
{
    SparseMat_<_Tp> m;
    this->copyTo(m);
    return m;
}

template<typename _Tp> inline
void SparseMat_<_Tp>::create(int _dims, const int* _sizes)
{
    SparseMat::create(_dims, _sizes, DataType<_Tp>::type);
}

template<typename _Tp> inline
int SparseMat_<_Tp>::type() const
{
    return DataType<_Tp>::type;
}

template<typename _Tp> inline
int SparseMat_<_Tp>::depth() const
{
    return DataType<_Tp>::depth;
}

template<typename _Tp> inline
int SparseMat_<_Tp>::channels() const
{
    return DataType<_Tp>::channels;
}

template<typename _Tp> inline
_Tp& SparseMat_<_Tp>::ref(int i0, size_t* hashval)
{
    return SparseMat::ref<_Tp>(i0, hashval);
}

template<typename _Tp> inline
_Tp SparseMat_<_Tp>::operator()(int i0, size_t* hashval) const
{
    return SparseMat::value<_Tp>(i0, hashval);
}

template<typename _Tp> inline
_Tp& SparseMat_<_Tp>::ref(int i0, int i1, size_t* hashval)
{
    return SparseMat::ref<_Tp>(i0, i1, hashval);
}

template<typename _Tp> inline
_Tp SparseMat_<_Tp>::operator()(int i0, int i1, size_t* hashval) const
{
    return SparseMat::value<_Tp>(i0, i1, hashval);
}

template<typename _Tp> inline
_Tp& SparseMat_<_Tp>::ref(int i0, int i1, int i2, size_t* hashval)
{
    return SparseMat::ref<_Tp>(i0, i1, i2, hashval);
}

template<typename _Tp> inline
_Tp SparseMat_<_Tp>::operator()(int i0, int i1, int i2, size_t* hashval) const
{
    return SparseMat::value<_Tp>(i0, i1, i2, hashval);
}

template<typename _Tp> inline
_Tp& SparseMat_<_Tp>::ref(const int* idx, size_t* hashval)
{
    return SparseMat::ref<_Tp>(idx, hashval);
}

template<typename _Tp> inline
_Tp SparseMat_<_Tp>::operator()(const int* idx, size_t* hashval) const
{
    return SparseMat::value<_Tp>(idx, hashval);
}

template<typename _Tp> inline
SparseMatIterator_<_Tp> SparseMat_<_Tp>::begin()
{
    return SparseMatIterator_<_Tp>(this);
}

template<typename _Tp> inline
SparseMatConstIterator_<_Tp> SparseMat_<_Tp>::begin() const
{
    return SparseMatConstIterator_<_Tp>(this);
}

template<typename _Tp> inline
SparseMatIterator_<_Tp> SparseMat_<_Tp>::end()
{
    SparseMatIterator_<_Tp> it(this);
    it.seekEnd();
    return it;
}

template<typename _Tp> inline
SparseMatConstIterator_<_Tp> SparseMat_<_Tp>::end() const
{
    SparseMatConstIterator_<_Tp> it(this);
    it.seekEnd();
    return it;
}



////////////////////////// MatConstIterator /////////////////////////

inline
MatConstIterator::MatConstIterator()
    : m(0), elemSize(0), ptr(0), sliceStart(0), sliceEnd(0)
{}

inline
MatConstIterator::MatConstIterator(const CvMat* _m)
    : m(_m), elemSize(_m->elemSize()), ptr(0), sliceStart(0), sliceEnd(0)
{
    if( m && m->isContinuous() )
    {
        sliceStart = m->ptr();
        sliceEnd = sliceStart + m->total()*elemSize;
    }
    seek((const int*)0);
}

inline
MatConstIterator::MatConstIterator(const CvMat* _m, int _row, int _col)
    : m(_m), elemSize(_m->elemSize()), ptr(0), sliceStart(0), sliceEnd(0)
{
    CC_Assert(m && m->dims <= 2);
    if( m->isContinuous() )
    {
        sliceStart = m->ptr();
        sliceEnd = sliceStart + m->total()*elemSize;
    }
    int idx[] = {_row, _col};
    seek(idx);
}

inline
MatConstIterator::MatConstIterator(const CvMat* _m, Point _pt)
    : m(_m), elemSize(_m->elemSize()), ptr(0), sliceStart(0), sliceEnd(0)
{
    CC_Assert(m && m->dims <= 2);
    if( m->isContinuous() )
    {
        sliceStart = m->ptr();
        sliceEnd = sliceStart + m->total()*elemSize;
    }
    int idx[] = {_pt.y, _pt.x};
    seek(idx);
}

inline
MatConstIterator::MatConstIterator(const MatConstIterator& it)
    : m(it.m), elemSize(it.elemSize), ptr(it.ptr), sliceStart(it.sliceStart), sliceEnd(it.sliceEnd)
{}

inline
MatConstIterator& MatConstIterator::operator = (const MatConstIterator& it )
{
    m = it.m; elemSize = it.elemSize; ptr = it.ptr;
    sliceStart = it.sliceStart; sliceEnd = it.sliceEnd;
    return *this;
}

inline
const uchar* MatConstIterator::operator *() const
{
    return ptr;
}

inline MatConstIterator& MatConstIterator::operator += (ptrdiff_t ofs)
{
    if( !m || ofs == 0 )
        return *this;
    ptrdiff_t ofsb = ofs*elemSize;
    ptr += ofsb;
    if( ptr < sliceStart || sliceEnd <= ptr )
    {
        ptr -= ofsb;
        seek(ofs, true);
    }
    return *this;
}

inline
MatConstIterator& MatConstIterator::operator -= (ptrdiff_t ofs)
{
    return (*this += -ofs);
}

inline
MatConstIterator& MatConstIterator::operator --()
{
    if( m && (ptr -= elemSize) < sliceStart )
    {
        ptr += elemSize;
        seek(-1, true);
    }
    return *this;
}

inline
MatConstIterator MatConstIterator::operator --(int)
{
    MatConstIterator b = *this;
    *this += -1;
    return b;
}

inline
MatConstIterator& MatConstIterator::operator ++()
{
    if( m && (ptr += elemSize) >= sliceEnd )
    {
        ptr -= elemSize;
        seek(1, true);
    }
    return *this;
}

inline MatConstIterator MatConstIterator::operator ++(int)
{
    MatConstIterator b = *this;
    *this += 1;
    return b;
}


static inline
bool operator == (const MatConstIterator& a, const MatConstIterator& b)
{
    return a.m == b.m && a.ptr == b.ptr;
}

static inline
bool operator != (const MatConstIterator& a, const MatConstIterator& b)
{
    return !(a == b);
}

static inline
bool operator < (const MatConstIterator& a, const MatConstIterator& b)
{
    return a.ptr < b.ptr;
}

static inline
bool operator > (const MatConstIterator& a, const MatConstIterator& b)
{
    return a.ptr > b.ptr;
}

static inline
bool operator <= (const MatConstIterator& a, const MatConstIterator& b)
{
    return a.ptr <= b.ptr;
}

static inline
bool operator >= (const MatConstIterator& a, const MatConstIterator& b)
{
    return a.ptr >= b.ptr;
}

static inline
ptrdiff_t operator - (const MatConstIterator& b, const MatConstIterator& a)
{
    if( a.m != b.m )
        return ((size_t)(-1) >> 1);
    if( a.sliceEnd == b.sliceEnd )
        return (b.ptr - a.ptr)/static_cast<ptrdiff_t>(b.elemSize);

    return b.lpos() - a.lpos();
}

static inline
MatConstIterator operator + (const MatConstIterator& a, ptrdiff_t ofs)
{
    MatConstIterator b = a;
    return b += ofs;
}

static inline
MatConstIterator operator + (ptrdiff_t ofs, const MatConstIterator& a)
{
    MatConstIterator b = a;
    return b += ofs;
}

static inline
MatConstIterator operator - (const MatConstIterator& a, ptrdiff_t ofs)
{
    MatConstIterator b = a;
    return b += -ofs;
}


inline
const uchar* MatConstIterator::operator [](ptrdiff_t i) const
{
    return *(*this + i);
}



///////////////////////// MatConstIterator_ /////////////////////////

template<typename _Tp> inline
MatConstIterator_<_Tp>::MatConstIterator_()
{}

template<typename _Tp> inline
MatConstIterator_<_Tp>::MatConstIterator_(const Mat_<_Tp>* _m)
    : MatConstIterator(_m)
{}

template<typename _Tp> inline
MatConstIterator_<_Tp>::MatConstIterator_(const Mat_<_Tp>* _m, int _row, int _col)
    : MatConstIterator(_m, _row, _col)
{}

template<typename _Tp> inline
MatConstIterator_<_Tp>::MatConstIterator_(const Mat_<_Tp>* _m, Point _pt)
    : MatConstIterator(_m, _pt)
{}

template<typename _Tp> inline
MatConstIterator_<_Tp>::MatConstIterator_(const MatConstIterator_& it)
    : MatConstIterator(it)
{}

template<typename _Tp> inline
MatConstIterator_<_Tp>& MatConstIterator_<_Tp>::operator = (const MatConstIterator_& it )
{
    MatConstIterator::operator = (it);
    return *this;
}

template<typename _Tp> inline
const _Tp& MatConstIterator_<_Tp>::operator *() const
{
    return *(_Tp*)(this->ptr);
}

template<typename _Tp> inline
MatConstIterator_<_Tp>& MatConstIterator_<_Tp>::operator += (ptrdiff_t ofs)
{
    MatConstIterator::operator += (ofs);
    return *this;
}

template<typename _Tp> inline
MatConstIterator_<_Tp>& MatConstIterator_<_Tp>::operator -= (ptrdiff_t ofs)
{
    return (*this += -ofs);
}

template<typename _Tp> inline
MatConstIterator_<_Tp>& MatConstIterator_<_Tp>::operator --()
{
    MatConstIterator::operator --();
    return *this;
}

template<typename _Tp> inline
MatConstIterator_<_Tp> MatConstIterator_<_Tp>::operator --(int)
{
    MatConstIterator_ b = *this;
    MatConstIterator::operator --();
    return b;
}

template<typename _Tp> inline
MatConstIterator_<_Tp>& MatConstIterator_<_Tp>::operator ++()
{
    MatConstIterator::operator ++();
    return *this;
}

template<typename _Tp> inline
MatConstIterator_<_Tp> MatConstIterator_<_Tp>::operator ++(int)
{
    MatConstIterator_ b = *this;
    MatConstIterator::operator ++();
    return b;
}


template<typename _Tp> inline
Point MatConstIterator_<_Tp>::pos() const
{
    if( !m )
        return Point();
    CC_DbgAssert( m->dims <= 2 );
    if( m->isContinuous() )
    {
        ptrdiff_t ofs = (const _Tp*)ptr - (const _Tp*)m->data;
        int y = (int)(ofs / m->cols);
        int x = (int)(ofs - (ptrdiff_t)y * m->cols);
        return Point(x, y);
    }
    else
    {
        ptrdiff_t ofs = (uchar*)ptr - m->data;
        int y = (int)(ofs / m->step);
        int x = (int)((ofs - y * m->step)/sizeof(_Tp));
        return Point(x, y);
    }
}


template<typename _Tp> static inline
bool operator == (const MatConstIterator_<_Tp>& a, const MatConstIterator_<_Tp>& b)
{
    return a.m == b.m && a.ptr == b.ptr;
}

template<typename _Tp> static inline
bool operator != (const MatConstIterator_<_Tp>& a, const MatConstIterator_<_Tp>& b)
{
    return a.m != b.m || a.ptr != b.ptr;
}

template<typename _Tp> static inline
MatConstIterator_<_Tp> operator + (const MatConstIterator_<_Tp>& a, ptrdiff_t ofs)
{
    MatConstIterator t = (const MatConstIterator&)a + ofs;
    return (MatConstIterator_<_Tp>&)t;
}

template<typename _Tp> static inline
MatConstIterator_<_Tp> operator + (ptrdiff_t ofs, const MatConstIterator_<_Tp>& a)
{
    MatConstIterator t = (const MatConstIterator&)a + ofs;
    return (MatConstIterator_<_Tp>&)t;
}

template<typename _Tp> static inline
MatConstIterator_<_Tp> operator - (const MatConstIterator_<_Tp>& a, ptrdiff_t ofs)
{
    MatConstIterator t = (const MatConstIterator&)a - ofs;
    return (MatConstIterator_<_Tp>&)t;
}

template<typename _Tp> inline
const _Tp& MatConstIterator_<_Tp>::operator [](ptrdiff_t i) const
{
    return *(_Tp*)MatConstIterator::operator [](i);
}



//////////////////////////// MatIterator_ ///////////////////////////

template<typename _Tp> inline
MatIterator_<_Tp>::MatIterator_()
    : MatConstIterator_<_Tp>()
{}

template<typename _Tp> inline
MatIterator_<_Tp>::MatIterator_(Mat_<_Tp>* _m)
    : MatConstIterator_<_Tp>(_m)
{}

template<typename _Tp> inline
MatIterator_<_Tp>::MatIterator_(Mat_<_Tp>* _m, int _row, int _col)
    : MatConstIterator_<_Tp>(_m, _row, _col)
{}

template<typename _Tp> inline
MatIterator_<_Tp>::MatIterator_(Mat_<_Tp>* _m, Point _pt)
    : MatConstIterator_<_Tp>(_m, _pt)
{}

template<typename _Tp> inline
MatIterator_<_Tp>::MatIterator_(Mat_<_Tp>* _m, const int* _idx)
    : MatConstIterator_<_Tp>(_m, _idx)
{}

template<typename _Tp> inline
MatIterator_<_Tp>::MatIterator_(const MatIterator_& it)
    : MatConstIterator_<_Tp>(it)
{}

template<typename _Tp> inline
MatIterator_<_Tp>& MatIterator_<_Tp>::operator = (const MatIterator_<_Tp>& it )
{
    MatConstIterator::operator = (it);
    return *this;
}

template<typename _Tp> inline
_Tp& MatIterator_<_Tp>::operator *() const
{
    return *(_Tp*)(this->ptr);
}

template<typename _Tp> inline
MatIterator_<_Tp>& MatIterator_<_Tp>::operator += (ptrdiff_t ofs)
{
    MatConstIterator::operator += (ofs);
    return *this;
}

template<typename _Tp> inline
MatIterator_<_Tp>& MatIterator_<_Tp>::operator -= (ptrdiff_t ofs)
{
    MatConstIterator::operator += (-ofs);
    return *this;
}

template<typename _Tp> inline
MatIterator_<_Tp>& MatIterator_<_Tp>::operator --()
{
    MatConstIterator::operator --();
    return *this;
}

template<typename _Tp> inline
MatIterator_<_Tp> MatIterator_<_Tp>::operator --(int)
{
    MatIterator_ b = *this;
    MatConstIterator::operator --();
    return b;
}

template<typename _Tp> inline
MatIterator_<_Tp>& MatIterator_<_Tp>::operator ++()
{
    MatConstIterator::operator ++();
    return *this;
}

template<typename _Tp> inline
MatIterator_<_Tp> MatIterator_<_Tp>::operator ++(int)
{
    MatIterator_ b = *this;
    MatConstIterator::operator ++();
    return b;
}

template<typename _Tp> inline
_Tp& MatIterator_<_Tp>::operator [](ptrdiff_t i) const
{
    return *(*this + i);
}


template<typename _Tp> static inline
bool operator == (const MatIterator_<_Tp>& a, const MatIterator_<_Tp>& b)
{
    return a.m == b.m && a.ptr == b.ptr;
}

template<typename _Tp> static inline
bool operator != (const MatIterator_<_Tp>& a, const MatIterator_<_Tp>& b)
{
    return a.m != b.m || a.ptr != b.ptr;
}

template<typename _Tp> static inline
MatIterator_<_Tp> operator + (const MatIterator_<_Tp>& a, ptrdiff_t ofs)
{
    MatConstIterator t = (const MatConstIterator&)a + ofs;
    return (MatIterator_<_Tp>&)t;
}

template<typename _Tp> static inline
MatIterator_<_Tp> operator + (ptrdiff_t ofs, const MatIterator_<_Tp>& a)
{
    MatConstIterator t = (const MatConstIterator&)a + ofs;
    return (MatIterator_<_Tp>&)t;
}

template<typename _Tp> static inline
MatIterator_<_Tp> operator - (const MatIterator_<_Tp>& a, ptrdiff_t ofs)
{
    MatConstIterator t = (const MatConstIterator&)a - ofs;
    return (MatIterator_<_Tp>&)t;
}



/////////////////////// SparseMatConstIterator //////////////////////

inline
SparseMatConstIterator::SparseMatConstIterator()
    : m(0), hashidx(0), ptr(0)
{}

inline
SparseMatConstIterator::SparseMatConstIterator(const SparseMatConstIterator& it)
    : m(it.m), hashidx(it.hashidx), ptr(it.ptr)
{}

inline SparseMatConstIterator& SparseMatConstIterator::operator = (const SparseMatConstIterator& it)
{
    if( this != &it )
    {
        m = it.m;
        hashidx = it.hashidx;
        ptr = it.ptr;
    }
    return *this;
}

template<typename _Tp> inline
const _Tp& SparseMatConstIterator::value() const
{
    return *(const _Tp*)ptr;
}

inline
const SparseMat::Node* SparseMatConstIterator::node() const
{
    return (ptr && m && m->hdr) ? (const SparseMat::Node*)(const void*)(ptr - m->hdr->valueOffset) : 0;
}

inline
SparseMatConstIterator SparseMatConstIterator::operator ++(int)
{
    SparseMatConstIterator it = *this;
    ++*this;
    return it;
}

inline
void SparseMatConstIterator::seekEnd()
{
    if( m && m->hdr )
    {
        hashidx = m->hdr->hashtab.size();
        ptr = 0;
    }
}


static inline
bool operator == (const SparseMatConstIterator& it1, const SparseMatConstIterator& it2)
{
    return it1.m == it2.m && it1.ptr == it2.ptr;
}

static inline
bool operator != (const SparseMatConstIterator& it1, const SparseMatConstIterator& it2)
{
    return !(it1 == it2);
}



///////////////////////// SparseMatIterator /////////////////////////

inline
SparseMatIterator::SparseMatIterator()
{}

inline
SparseMatIterator::SparseMatIterator(SparseMat* _m)
    : SparseMatConstIterator(_m)
{}

inline
SparseMatIterator::SparseMatIterator(const SparseMatIterator& it)
    : SparseMatConstIterator(it)
{}

inline
SparseMatIterator& SparseMatIterator::operator = (const SparseMatIterator& it)
{
    (SparseMatConstIterator&)*this = it;
    return *this;
}

template<typename _Tp> inline
_Tp& SparseMatIterator::value() const
{
    return *(_Tp*)ptr;
}

inline
SparseMat::Node* SparseMatIterator::node() const
{
    return (SparseMat::Node*)SparseMatConstIterator::node();
}

inline
SparseMatIterator& SparseMatIterator::operator ++()
{
    SparseMatConstIterator::operator ++();
    return *this;
}

inline
SparseMatIterator SparseMatIterator::operator ++(int)
{
    SparseMatIterator it = *this;
    ++*this;
    return it;
}



////////////////////// SparseMatConstIterator_ //////////////////////

template<typename _Tp> inline
SparseMatConstIterator_<_Tp>::SparseMatConstIterator_()
{}

template<typename _Tp> inline
SparseMatConstIterator_<_Tp>::SparseMatConstIterator_(const SparseMat_<_Tp>* _m)
    : SparseMatConstIterator(_m)
{}

template<typename _Tp> inline
SparseMatConstIterator_<_Tp>::SparseMatConstIterator_(const SparseMat* _m)
    : SparseMatConstIterator(_m)
{
    CC_Assert( _m->tid() == DataType<_Tp>::type );
}

template<typename _Tp> inline
SparseMatConstIterator_<_Tp>::SparseMatConstIterator_(const SparseMatConstIterator_<_Tp>& it)
    : SparseMatConstIterator(it)
{}

template<typename _Tp> inline
SparseMatConstIterator_<_Tp>& SparseMatConstIterator_<_Tp>::operator = (const SparseMatConstIterator_<_Tp>& it)
{
    return reinterpret_cast<SparseMatConstIterator_<_Tp>&>
         (*reinterpret_cast<SparseMatConstIterator*>(this) =
           reinterpret_cast<const SparseMatConstIterator&>(it));
}

template<typename _Tp> inline
const _Tp& SparseMatConstIterator_<_Tp>::operator *() const
{
    return *(const _Tp*)this->ptr;
}

template<typename _Tp> inline
SparseMatConstIterator_<_Tp>& SparseMatConstIterator_<_Tp>::operator ++()
{
    SparseMatConstIterator::operator ++();
    return *this;
}

template<typename _Tp> inline
SparseMatConstIterator_<_Tp> SparseMatConstIterator_<_Tp>::operator ++(int)
{
    SparseMatConstIterator_<_Tp> it = *this;
    SparseMatConstIterator::operator ++();
    return it;
}



///////////////////////// SparseMatIterator_ ////////////////////////

template<typename _Tp> inline
SparseMatIterator_<_Tp>::SparseMatIterator_()
{}

template<typename _Tp> inline
SparseMatIterator_<_Tp>::SparseMatIterator_(SparseMat_<_Tp>* _m)
    : SparseMatConstIterator_<_Tp>(_m)
{}

template<typename _Tp> inline
SparseMatIterator_<_Tp>::SparseMatIterator_(SparseMat* _m)
    : SparseMatConstIterator_<_Tp>(_m)
{}

template<typename _Tp> inline
SparseMatIterator_<_Tp>::SparseMatIterator_(const SparseMatIterator_<_Tp>& it)
    : SparseMatConstIterator_<_Tp>(it)
{}

template<typename _Tp> inline
SparseMatIterator_<_Tp>& SparseMatIterator_<_Tp>::operator = (const SparseMatIterator_<_Tp>& it)
{
    return reinterpret_cast<SparseMatIterator_<_Tp>&>
         (*reinterpret_cast<SparseMatConstIterator*>(this) =
           reinterpret_cast<const SparseMatConstIterator&>(it));
}

template<typename _Tp> inline
_Tp& SparseMatIterator_<_Tp>::operator *() const
{
    return *(_Tp*)this->ptr;
}

template<typename _Tp> inline
SparseMatIterator_<_Tp>& SparseMatIterator_<_Tp>::operator ++()
{
    SparseMatConstIterator::operator ++();
    return *this;
}

template<typename _Tp> inline
SparseMatIterator_<_Tp> SparseMatIterator_<_Tp>::operator ++(int)
{
    SparseMatIterator_<_Tp> it = *this;
    SparseMatConstIterator::operator ++();
    return it;
}



//////////////////////// MatCommaInitializer_ ///////////////////////

template<typename _Tp> inline
MatCommaInitializer_<_Tp>::MatCommaInitializer_(Mat_<_Tp>* _m)
    : it(_m)
{}

template<typename _Tp> template<typename T2> inline
MatCommaInitializer_<_Tp>& MatCommaInitializer_<_Tp>::operator , (T2 v)
{
    CC_DbgAssert( this->it < ((const Mat_<_Tp>*)this->it.m)->end() );
    *this->it = _Tp(v);
    ++this->it;
    return *this;
}

template<typename _Tp> inline
MatCommaInitializer_<_Tp>::operator Mat_<_Tp>() const
{
    CC_DbgAssert( this->it == ((const Mat_<_Tp>*)this->it.m)->end() );
    return Mat_<_Tp>(*this->it.m);
}


template<typename _Tp, typename T2> static inline
MatCommaInitializer_<_Tp> operator << (const Mat_<_Tp>& m, T2 val)
{
    MatCommaInitializer_<_Tp> commaInitializer((Mat_<_Tp>*)&m);
    return (commaInitializer, val);
}



///////////////////////// Matrix Expressions ////////////////////////

inline
CvMat& CvMat::operator = (const MatExpr& e)
{
    e.op->assign(e, *this);
    return *this;
}

template<typename _Tp> inline
Mat_<_Tp>::Mat_(const MatExpr& e)
{
    e.op->assign(e, *this, DataType<_Tp>::type);
}

template<typename _Tp> inline
Mat_<_Tp>& Mat_<_Tp>::operator = (const MatExpr& e)
{
    e.op->assign(e, *this, DataType<_Tp>::type);
    return *this;
}

template<typename _Tp> inline
MatExpr Mat_<_Tp>::zeros(int rows, int cols)
{
    return CvMat::zeros(rows, cols, DataType<_Tp>::type);
}

template<typename _Tp> inline
MatExpr Mat_<_Tp>::zeros(Size sz)
{
    return CvMat::zeros(sz, DataType<_Tp>::type);
}

template<typename _Tp> inline
MatExpr Mat_<_Tp>::ones(int rows, int cols)
{
    return CvMat::ones(rows, cols, DataType<_Tp>::type);
}

template<typename _Tp> inline
MatExpr Mat_<_Tp>::ones(Size sz)
{
    return CvMat::ones(sz, DataType<_Tp>::type);
}

template<typename _Tp> inline
MatExpr Mat_<_Tp>::eye(int rows, int cols)
{
    return CvMat::eye(rows, cols, DataType<_Tp>::type);
}

template<typename _Tp> inline
MatExpr Mat_<_Tp>::eye(Size sz)
{
    return CvMat::eye(sz, DataType<_Tp>::type);
}

inline
MatExpr::MatExpr()
    : op(0), flags(0), a(CvMat()), b(CvMat()), c(CvMat()), alpha(0), beta(0), s()
{}

inline
MatExpr::MatExpr(const MatOp* _op, int _flags, const CvMat& _a, const CvMat& _b,
                 const CvMat& _c, double _alpha, double _beta, const Scalar& _s)
    : op(_op), flags(_flags), a(_a), b(_b), c(_c), alpha(_alpha), beta(_beta), s(_s)
{}

inline
MatExpr::operator CvMat() const
{
    CvMat m;
    op->assign(*this, m);
    return m;
}

template<typename _Tp> inline
MatExpr::operator Mat_<_Tp>() const
{
    Mat_<_Tp> m;
    op->assign(*this, m, DataType<_Tp>::type);
    return m;
}


template<typename _Tp> static inline
MatExpr min(const Mat_<_Tp>& a, const Mat_<_Tp>& b)
{
    return min((const CvMat&)a, (const CvMat&)b);
}

template<typename _Tp> static inline
MatExpr min(const Mat_<_Tp>& a, double s)
{
    return min((const CvMat&)a, s);
}

template<typename _Tp> static inline
MatExpr min(double s, const Mat_<_Tp>& a)
{
    return min((const CvMat&)a, s);
}

template<typename _Tp> static inline
MatExpr max(const Mat_<_Tp>& a, const Mat_<_Tp>& b)
{
    return max((const CvMat&)a, (const CvMat&)b);
}

template<typename _Tp> static inline
MatExpr max(const Mat_<_Tp>& a, double s)
{
    return max((const CvMat&)a, s);
}

template<typename _Tp> static inline
MatExpr max(double s, const Mat_<_Tp>& a)
{
    return max((const CvMat&)a, s);
}

template<typename _Tp> static inline
MatExpr abs(const Mat_<_Tp>& m)
{
    return abs((const CvMat&)m);
}


static inline
CvMat& operator += (CvMat& a, const MatExpr& b)
{
    b.op->augAssignAdd(b, a);
    return a;
}

static inline
const CvMat& operator += (const CvMat& a, const MatExpr& b)
{
    b.op->augAssignAdd(b, (CvMat&)a);
    return a;
}

template<typename _Tp> static inline
Mat_<_Tp>& operator += (Mat_<_Tp>& a, const MatExpr& b)
{
    b.op->augAssignAdd(b, a);
    return a;
}

template<typename _Tp> static inline
const Mat_<_Tp>& operator += (const Mat_<_Tp>& a, const MatExpr& b)
{
    b.op->augAssignAdd(b, (CvMat&)a);
    return a;
}

static inline
CvMat& operator -= (CvMat& a, const MatExpr& b)
{
    b.op->augAssignSubtract(b, a);
    return a;
}

static inline
const CvMat& operator -= (const CvMat& a, const MatExpr& b)
{
    b.op->augAssignSubtract(b, (CvMat&)a);
    return a;
}

template<typename _Tp> static inline
Mat_<_Tp>& operator -= (Mat_<_Tp>& a, const MatExpr& b)
{
    b.op->augAssignSubtract(b, a);
    return a;
}

template<typename _Tp> static inline
const Mat_<_Tp>& operator -= (const Mat_<_Tp>& a, const MatExpr& b)
{
    b.op->augAssignSubtract(b, (CvMat&)a);
    return a;
}

static inline
CvMat& operator *= (CvMat& a, const MatExpr& b)
{
    b.op->augAssignMultiply(b, a);
    return a;
}

static inline
const CvMat& operator *= (const CvMat& a, const MatExpr& b)
{
    b.op->augAssignMultiply(b, (CvMat&)a);
    return a;
}

template<typename _Tp> static inline
Mat_<_Tp>& operator *= (Mat_<_Tp>& a, const MatExpr& b)
{
    b.op->augAssignMultiply(b, a);
    return a;
}

template<typename _Tp> static inline
const Mat_<_Tp>& operator *= (const Mat_<_Tp>& a, const MatExpr& b)
{
    b.op->augAssignMultiply(b, (CvMat&)a);
    return a;
}

static inline
CvMat& operator /= (CvMat& a, const MatExpr& b)
{
    b.op->augAssignDivide(b, a);
    return a;
}

static inline
const CvMat& operator /= (const CvMat& a, const MatExpr& b)
{
    b.op->augAssignDivide(b, (CvMat&)a);
    return a;
}

template<typename _Tp> static inline
Mat_<_Tp>& operator /= (Mat_<_Tp>& a, const MatExpr& b)
{
    b.op->augAssignDivide(b, a);
    return a;
}

template<typename _Tp> static inline
const Mat_<_Tp>& operator /= (const Mat_<_Tp>& a, const MatExpr& b)
{
    b.op->augAssignDivide(b, (CvMat&)a);
    return a;
}


//////////////////////////////// UMat ////////////////////////////////

inline
UMat::UMat(UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0), size(&rows)
{}

inline
UMat::UMat(int _rows, int _cols, int _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0), size(&rows)
{
    create(_rows, _cols, _type);
}

inline
UMat::UMat(int _rows, int _cols, int _type, const Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0), size(&rows)
{
    create(_rows, _cols, _type);
    *this = _s;
}

inline
UMat::UMat(Size _sz, int _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0), size(&rows)
{
    create( _sz.height, _sz.width, _type );
}

inline
UMat::UMat(Size _sz, int _type, const Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0), size(&rows)
{
    create(_sz.height, _sz.width, _type);
    *this = _s;
}

inline
UMat::UMat(int _dims, const int* _sz, int _type, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0), size(&rows)
{
    create(_dims, _sz, _type);
}

inline
UMat::UMat(int _dims, const int* _sz, int _type, const Scalar& _s, UMatUsageFlags _usageFlags)
: flags(MAGIC_VAL), dims(0), rows(0), cols(0), allocator(0), usageFlags(_usageFlags), u(0), offset(0), size(&rows)
{
    create(_dims, _sz, _type);
    *this = _s;
}

inline
UMat::UMat(const UMat& m)
: flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(&rows)
{
    addref();
    if( m.dims <= 2 )
    {
        step[0] = m.step[0]; step[1] = m.step[1];
    }
    else
    {
        dims = 0;
        copySize(m);
    }
}


template<typename _Tp> inline
UMat::UMat(const std::vector<_Tp>& vec, bool copyData)
: flags(MAGIC_VAL | DataType<_Tp>::type | CC_MAT_CONT_FLAG), dims(2), rows((int)vec.size()),
cols(1), allocator(0), usageFlags(USAGE_DEFAULT), u(0), offset(0), size(&rows)
{
    if(vec.empty())
        return;
    if( !copyData )
    {
        // !!!TODO!!!
        CC_Error(Error::StsNotImplemented, "");
    }
    else
        CvMat((int)vec.size(), 1, DataType<_Tp>::type, (uchar*)&vec[0]).copyTo(*this);
}


inline
UMat& UMat::operator = (const UMat& m)
{
    if( this != &m )
    {
        const_cast<UMat&>(m).addref();
        release();
        flags = m.flags;
        if( dims <= 2 && m.dims <= 2 )
        {
            dims = m.dims;
            rows = m.rows;
            cols = m.cols;
            step[0] = m.step[0];
            step[1] = m.step[1];
        }
        else
            copySize(m);
        allocator = m.allocator;
        if (usageFlags == USAGE_DEFAULT)
            usageFlags = m.usageFlags;
        u = m.u;
        offset = m.offset;
    }
    return *this;
}

inline
UMat UMat::row(int y) const
{
    return UMat(*this, Range(y, y + 1), Range::all());
}

inline
UMat UMat::col(int x) const
{
    return UMat(*this, Range::all(), Range(x, x + 1));
}

inline
UMat UMat::rowRange(int startrow, int endrow) const
{
    return UMat(*this, Range(startrow, endrow), Range::all());
}

inline
UMat UMat::rowRange(const Range& r) const
{
    return UMat(*this, r, Range::all());
}

inline
UMat UMat::colRange(int startcol, int endcol) const
{
    return UMat(*this, Range::all(), Range(startcol, endcol));
}

inline
UMat UMat::colRange(const Range& r) const
{
    return UMat(*this, Range::all(), r);
}

inline
UMat UMat::clone() const
{
    UMat m;
    copyTo(m);
    return m;
}

inline
void UMat::assignTo( UMat& m, int _type ) const
{
    if( _type < 0 )
        m = *this;
    else
        convertTo(m, _type);
}

inline
void UMat::create(int _rows, int _cols, int _type, UMatUsageFlags _usageFlags)
{
    _type &= TYPE_MASK;
    if( dims <= 2 && rows == _rows && cols == _cols && type() == _type && u )
        return;
    int sz[] = {_rows, _cols};
    create(2, sz, _type, _usageFlags);
}

inline
void UMat::create(Size _sz, int _type, UMatUsageFlags _usageFlags)
{
    create(_sz.height, _sz.width, _type, _usageFlags);
}

inline
void UMat::addref()
{
    if( u )
        CC_XADD(&(u->urefcount), 1);
}

inline void UMat::release()
{
    if( u && CC_XADD(&(u->urefcount), -1) == 1 )
        deallocate();
    for(int i = 0; i < dims; i++)
        size.p[i] = 0;
    u = 0;
}

inline
UMat UMat::operator()( Range _rowRange, Range _colRange ) const
{
    return UMat(*this, _rowRange, _colRange);
}

inline
UMat UMat::operator()( const Rect& roi ) const
{
    return UMat(*this, roi);
}

inline
UMat UMat::operator()(const Range* ranges) const
{
    return UMat(*this, ranges);
}

inline
UMat UMat::operator()(const std::vector<Range>& ranges) const
{
    return UMat(*this, ranges);
}

inline
bool UMat::isContinuous() const
{
    return (flags & CONTINUOUS_FLAG) != 0;
}

inline
bool UMat::isSubmatrix() const
{
    return (flags & SUBMATRIX_FLAG) != 0;
}

inline
size_t UMat::elemSize() const
{
    return dims > 0 ? step.p[dims - 1] : 0;
}

inline
size_t UMat::elemSize1() const
{
    return CC_ELEM_SIZE1(flags);
}

inline
int UMat::type() const
{
    return CC_MAT_TYPE(flags);
}

inline
int UMat::depth() const
{
    return CC_MAT_DEPTH(flags);
}

inline
int UMat::channels() const
{
    return CC_MAT_CN(flags);
}

inline
size_t UMat::step1(int i) const
{
    return step.p[i] / elemSize1();
}

inline
bool UMat::empty() const
{
    return u == 0 || total() == 0;
}

inline
size_t UMat::total() const
{
    if( dims <= 2 )
        return (size_t)rows * cols;
    size_t p = 1;
    for( int i = 0; i < dims; i++ )
        p *= size[i];
    return p;
}

#ifdef CC_CXX_MOVE_SEMANTICS

inline
UMat::UMat(UMat&& m)
: flags(m.flags), dims(m.dims), rows(m.rows), cols(m.cols), allocator(m.allocator),
  usageFlags(m.usageFlags), u(m.u), offset(m.offset), size(&rows)
{
    if (m.dims <= 2)  // move new step/size info
    {
        step[0] = m.step[0];
        step[1] = m.step[1];
    }
    else
    {
        CC_DbgAssert(m.step.p != m.step.buf);
        step.p = m.step.p;
        size.p = m.size.p;
        m.step.p = m.step.buf;
        m.size.p = &m.rows;
    }
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.allocator = NULL;
    m.u = NULL;
    m.offset = 0;
}

inline
UMat& UMat::operator = (UMat&& m)
{
    if (this == &m)
      return *this;
    release();
    flags = m.flags; dims = m.dims; rows = m.rows; cols = m.cols;
    allocator = m.allocator; usageFlags = m.usageFlags;
    u = m.u;
    offset = m.offset;
    if (step.p != step.buf) // release self step/size
    {
        fastFree(step.p);
        step.p = step.buf;
        size.p = &rows;
    }
    if (m.dims <= 2) // move new step/size info
    {
        step[0] = m.step[0];
        step[1] = m.step[1];
    }
    else
    {
        CC_DbgAssert(m.step.p != m.step.buf);
        step.p = m.step.p;
        size.p = m.size.p;
        m.step.p = m.step.buf;
        m.size.p = &m.rows;
    }
    m.flags = MAGIC_VAL; m.dims = m.rows = m.cols = 0;
    m.allocator = NULL;
    m.u = NULL;
    m.offset = 0;
    return *this;
}

#endif


inline bool UMatData::hostCopyObsolete() const { return (flags & HOST_COPY_OBSOLETE) != 0; }
inline bool UMatData::deviceCopyObsolete() const { return (flags & DEVICE_COPY_OBSOLETE) != 0; }
inline bool UMatData::deviceMemMapped() const { return (flags & DEVICE_MEM_MAPPED) != 0; }
inline bool UMatData::copyOnMap() const { return (flags & COPY_ON_MAP) != 0; }
inline bool UMatData::tempUMat() const { return (flags & TEMP_UMAT) != 0; }
inline bool UMatData::tempCopiedUMat() const { return (flags & TEMP_COPIED_UMAT) == TEMP_COPIED_UMAT; }

inline void UMatData::markDeviceMemMapped(bool flag)
{
  if(flag)
    flags |= DEVICE_MEM_MAPPED;
  else
    flags &= ~DEVICE_MEM_MAPPED;
}

inline void UMatData::markHostCopyObsolete(bool flag)
{
    if(flag)
        flags |= HOST_COPY_OBSOLETE;
    else
        flags &= ~HOST_COPY_OBSOLETE;
}
inline void UMatData::markDeviceCopyObsolete(bool flag)
{
    if(flag)
        flags |= DEVICE_COPY_OBSOLETE;
    else
        flags &= ~DEVICE_COPY_OBSOLETE;
}

inline UMatDataAutoLock::UMatDataAutoLock(UMatData* _u) : u(_u) { u->lock(); }
inline UMatDataAutoLock::~UMatDataAutoLock() { u->unlock(); }

//! @endcond

} //cv

#endif
