
#define CC_SYMM_ROWS  0
#define CC_SYMM_COLS  1
#define CC_SYMM_ABS   2
#define CC_SYMM_CONJ  4


#define CCFUNC(NAME)  NAME ## _32f
#define T float
#include "matrix_impl.inl"

#define CCFUNC(NAME)  NAME ## _64f
#define T double
#include "matrix_impl.inl"

static double cvIsDiag(const img_t* im) {
  int elem_size = CC_TYPE_SIZE(im->tid);
  int step = im->s/elem_size;
  double ret = 0;
  if(CC_32F==im->tid) {
    ret = isdiag_32f(im->tt.fl, step, im->rows, im->cols, 0, 1);
  } else if (CC_64F==im->tid) {
    ret = isdiag_64f(im->tt.db, step, im->rows, im->cols, 0, 1);
  } else {
    CC_ERROR(0, "");
  }
  return ret;
}


static void cvSymm(img_t* im, int flags CC_DEFAULT(0)) {
  int elem_size = CC_TYPE_SIZE(im->tid);
  int step0 = im->s/elem_size;
  int step1 = im->c/elem_size;
  if(CC_32F==im->tid) {
    mat_symm_32f(im->tt.fl, step0, step1, im->rows, im->cols, flags);
  } else if (CC_64F==im->tid) {
    mat_symm_64f(im->tt.db, step0, step1, im->rows, im->cols, flags);
  } else {
    CC_ERROR(0, "");
  }
  return ;
}

#if 0
/****************************************************************************************\
*                           [scaled] Identity matrix initialization                      *
\****************************************************************************************/

static inline void setSize( img_t* m, int _dims, const int* _sz,
                            const size_t* _steps, bool autoSteps=false )
{
    CC_Assert( 0 <= _dims && _dims <= CC_MAX_DIM );
    if( m.dims != _dims )
    {
        if( m->step.p != m->step.buf )
        {
            fastFree(m->step.p);
            m->step.p = m->step.buf;
            m.size.p = &m->rows;
        }
        if( _dims > 2 )
        {
            m->step.p = (size_t*)fastMalloc(_dims*sizeof(m->step.p[0]) + (_dims+1)*sizeof(m.size.p[0]));
            m.size.p = (int*)(m->step.p + _dims) + 1;
            m.size.p[-1] = _dims;
            m->rows = m->cols = -1;
        }
    }

    m.dims = _dims;
    if( !_sz )
        return;

    size_t esz = CC_TYPE_SIZE(m.flags), esz1 = CC_ELEM_SIZE1(m.flags), total = esz;
    for( int i = _dims-1; i >= 0; i-- )
    {
        int s = _sz[i];
        CC_Assert( s >= 0 );
        m.size.p[i] = s;

        if( _steps )
        {
            if (_steps[i] % esz1 != 0)
            {
                CC_Error(CC_BadStep, "Step must be a multiple of esz1");
            }

            m->step.p[i] = i < _dims-1 ? _steps[i] : esz;
        }
        else if( autoSteps )
        {
            m->step.p[i] = total;
            int64 total1 = (int64)total*s;
            if( (uint64)total1 != (size_t)total1 )
                CC_Error( CC_StsOutOfRange, "The total matrix size does not fit to \"size_t\" type" );
            total = (size_t)total1;
        }
    }

    if( _dims == 1 )
    {
        m.dims = 2;
        m->cols = 1;
        m->step[1] = esz;
    }
}

static void updateContinuityFlag(img_t& m)
{
    int i, j;
    for( i = 0; i < m.dims; i++ )
    {
        if( m.size[i] > 1 )
            break;
    }

    for( j = m.dims-1; j > i; j-- )
    {
        if( m->step[j]*m.size[j] < m->step[j-1] )
            break;
    }

    uint64 t = (uint64)m->step[0]*m.size[0];
    if( j <= i && t == (size_t)t )
        m.flags |= img_t::CONTINUOUS_FLAG;
    else
        m.flags &= ~img_t::CONTINUOUS_FLAG;
}

static void finalizeHdr(img_t& m)
{
    updateContinuityFlag(m);
    int d = m.dims;
    if( d > 2 )
        m->rows = m->cols = -1;
    if(m.u)
        m.datastart = m.data = m.u->data;
    if( m.data )
    {
        m.datalimit = m.datastart + m.size[0]*m->step[0];
        if( m.size[0] > 0 )
        {
            m.dataend = m->tt.data + m.size[d-1]*m->step[d-1];
            for( int i = 0; i < d-1; i++ )
                m.dataend += (m.size[i] - 1)*m->step[i];
        }
        else
            m.dataend = m.datalimit;
    }
    else
        m.dataend = m.datalimit = 0;
}


void img_t::create(int d, const int* _sizes, int _type)
{
    int i;
    CC_Assert(0 <= d && d <= CC_MAX_DIM && _sizes);
    _type = CC_MAT_DEPTH(_type);

    if( data && (d == dims || (d == 1 && dims <= 2)) && _type == type() )
    {
        if( d == 2 && rows == _sizes[0] && cols == _sizes[1] )
            return;
        for( i = 0; i < d; i++ )
            if( size[i] != _sizes[i] )
                break;
        if( i == d && (d > 1 || size[1] == 1))
            return;
    }

    int _sizes_backup[CC_MAX_DIM]; // 5991
    if (_sizes == (this->size.p))
    {
        for(i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }

    release();
    if( d == 0 )
        return;
    flags = (_type & CC_MAT_TYPE_MASK) | MAGIC_VAL;
    setSize(*this, d, _sizes, 0, true);

    if( total() > 0 )
    {
        MatAllocator *a = allocator, *a0 = getDefaultAllocator();
#ifdef HAVE_TGPU
        if( !a || a == tegra::getAllocator() )
            a = tegra::getAllocator(d, _sizes, _type);
#endif
        if(!a)
            a = a0;
        try
        {
            u = a->allocate(dims, size, _type, 0, step.p, 0, USAGE_DEFAULT);
            CC_Assert(u != 0);
        }
        catch(...)
        {
            if(a != a0)
                u = a0->allocate(dims, size, _type, 0, step.p, 0, USAGE_DEFAULT);
            CC_Assert(u != 0);
        }
        CC_Assert( step[dims-1] == (size_t)CC_TYPE_SIZE(flags) );
    }

    addref();
    finalizeHdr(*this);
}

void img_t::create(const std::vector<int>& _sizes, int _type)
{
    create((int)_sizes.size(), _sizes.data(), _type);
}

void img_t::copySize(const img_t& m)
{
    setSize(*this, m.dims, 0, 0);
    for( int i = 0; i < dims; i++ )
    {
        size[i] = m.size[i];
        step[i] = m->step[i];
    }
}

void img_t::deallocate()
{
    if(u)
        (u->currAllocator ? u->currAllocator : allocator ? allocator : getDefaultAllocator())->unmap(u);
    u = NULL;
}

img_t::img_t(const img_t& m, const CRange& _rowRange, const CRange& _colRange)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    CC_Assert( m.dims >= 2 );
    if( m.dims > 2 )
    {
        CAutoBuffer<CRange> rs(m.dims);
        rs[0] = _rowRange;
        rs[1] = _colRange;
        for( int i = 2; i < m.dims; i++ )
            rs[i] = CRange::all();
        *this = m(rs);
        return;
    }

    *this = m;
    if( _rowRange != CRange::all() && _rowRange != CRange(0,rows) )
    {
        CC_Assert( 0 <= _rowRange.start && _rowRange.start <= _rowRange.end && _rowRange.end <= m->rows );
        rows = _rowRange.size();
        data += step*_rowRange.start;
        flags |= SUBMATRIX_FLAG;
    }

    if( _colRange != CRange::all() && _colRange != CRange(0,cols) )
    {
        CC_Assert( 0 <= _colRange.start && _colRange.start <= _colRange.end && _colRange.end <= m->cols );
        cols = _colRange.size();
        data += _colRange.start*elemSize();
        flags &= cols < m->cols ? ~CONTINUOUS_FLAG : -1;
        flags |= SUBMATRIX_FLAG;
    }

    if( rows == 1 )
        flags |= CONTINUOUS_FLAG;

    if( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


img_t::img_t(const img_t& m, const Rect& roi)
    : flags(m.flags), dims(2), rows(roi.height), cols(roi.width),
    data(m.data + roi.y*m->step[0]),
    datastart(m.datastart), dataend(m.dataend), datalimit(m.datalimit),
    allocator(m.allocator), u(m.u), size(&rows)
{
    CC_Assert( m.dims <= 2 );
    flags &= roi.width < m->cols ? ~CONTINUOUS_FLAG : -1;
    flags |= roi.height == 1 ? CONTINUOUS_FLAG : 0;

    size_t esz = CC_TYPE_SIZE(flags);
    data += roi.x*esz;
    CC_Assert( 0 <= roi.x && 0 <= roi.width && roi.x + roi.width <= m->cols &&
              0 <= roi.y && 0 <= roi.height && roi.y + roi.height <= m->rows );
    if( u )
        CC_XADD(&u->refcount, 1);
    if( roi.width < m->cols || roi.height < m->rows )
        flags |= SUBMATRIX_FLAG;

    step[0] = m->step[0]; step[1] = esz;

    if( rows <= 0 || cols <= 0 )
    {
        release();
        rows = cols = 0;
    }
}


img_t::img_t(int _dims, const int* _sizes, int _type, void* _data, const size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    flags |= CC_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    setSize(*this, _dims, _sizes, _steps, true);
    finalizeHdr(*this);
}


img_t::img_t(const std::vector<int>& _sizes, int _type, void* _data, const size_t* _steps)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    flags |= CC_MAT_TYPE(_type);
    datastart = data = (uchar*)_data;
    setSize(*this, (int)_sizes.size(), _sizes.data(), _steps, true);
    finalizeHdr(*this);
}


img_t::img_t(const img_t& m, const CRange* ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
      datalimit(0), allocator(0), u(0), size(&rows)
{
    int d = m.dims;

    CC_Assert(ranges);
    for( int i = 0; i < d; i++ )
    {
        CRange r = ranges[i];
        CC_Assert( r == CRange::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]) );
    }
    *this = m;
    for( int i = 0; i < d; i++ )
    {
        CRange r = ranges[i];
        if( r != CRange::all() && r != CRange(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }
    updateContinuityFlag(*this);
}

img_t::img_t(const img_t& m, const std::vector<CRange>& ranges)
    : flags(MAGIC_VAL), dims(0), rows(0), cols(0), data(0), datastart(0), dataend(0),
    datalimit(0), allocator(0), u(0), size(&rows)
{
    int d = m.dims;

    CC_Assert((int)ranges.size() == d);
    for (int i = 0; i < d; i++)
    {
        CRange r = ranges[i];
        CC_Assert(r == CRange::all() || (0 <= r.start && r.start < r.end && r.end <= m.size[i]));
    }
    *this = m;
    for (int i = 0; i < d; i++)
    {
        CRange r = ranges[i];
        if (r != CRange::all() && r != CRange(0, size.p[i]))
        {
            size.p[i] = r.end - r.start;
            data += r.start*step.p[i];
            flags |= SUBMATRIX_FLAG;
        }
    }
    updateContinuityFlag(*this);
}

static img_t cvMatNDToMat(const CvMatND* m, bool copyData)
{
    img_t thiz;

    if( !m )
        return thiz;
    thiz.datastart = thiz.data = m->data.ptr;
    thiz.flags |= CC_MAT_TYPE(m->tid);
    int _sizes[CC_MAX_DIM];
    size_t _steps[CC_MAX_DIM];

    int d = m->dims;
    for( int i = 0; i < d; i++ )
    {
        _sizes[i] = m->dim[i].size;
        _steps[i] = m->dim[i]->step;
    }

    setSize(thiz, d, _sizes, _steps);
    finalizeHdr(thiz);

    if( copyData )
    {
        img_t temp(thiz);
        thiz.release();
        temp.copyTo(thiz);
    }

    return thiz;
}

static img_t cvMatToMat(const img_t* m, bool copyData)
{
    img_t thiz;

    if( !m )
        return thiz;

    if( !copyData )
    {
        thiz.flags = img_t::MAGIC_VAL + (m->tid & (CC_MAT_TYPE_MASK|CC_MAT_CONT_FLAG));
        thiz.dims = 2;
        thiz->rows = m->rows;
        thiz->cols = m->cols;
        thiz.datastart = thiz.data = m->data.ptr;
        size_t esz = CC_TYPE_SIZE(m->tid), minstep = thiz->cols*esz, _step = m->step;
        if( _step == 0 )
            _step = minstep;
        thiz.datalimit = thiz.datastart + _step*thiz->rows;
        thiz.dataend = thiz.datalimit - _step + minstep;
        thiz->step[0] = _step; thiz->step[1] = esz;
    }
    else
    {
        thiz.datastart = thiz.dataend = thiz.data = 0;
        img_t(m->rows, m->cols, m->tid, m->data.ptr, m->step).copyTo(thiz);
    }

    return thiz;
}


static img_t iplImageToMat(const img_t* img, bool copyData)
{
    img_t m;

    if( !img )
        return m;

    m.dims = 2;
    CC_DbgAssert(CC_IS_IMAGE(img) && img->imageData != 0);

    int imgdepth = IPL2CC_DEPTH(img->depth);
    size_t esz;
    m->step[0] = img->widthStep;

    if(!img->roi)
    {
        CC_Assert(img->dataOrder == IMG_DATA_ORDER_PIXEL);
        m.flags = img_t::MAGIC_VAL + CC_MAKETYPE(imgdepth, img->nChannels);
        m->rows = img->height;
        m->cols = img->width;
        m.datastart = m.data = (uchar*)img->imageData;
        esz = CC_TYPE_SIZE(m.flags);
    }
    else
    {
        CC_Assert(img->dataOrder == IMG_DATA_ORDER_PIXEL || img->roi->coi != 0);
        bool selectedPlane = img->roi->coi && img->dataOrder == IMG_DATA_ORDER_PLANE;
        m.flags = img_t::MAGIC_VAL + CC_MAKETYPE(imgdepth, selectedPlane ? 1 : img->nChannels);
        m->rows = img->roi->height;
        m->cols = img->roi->width;
        esz = CC_TYPE_SIZE(m.flags);
        m.datastart = m.data = (uchar*)img->imageData +
            (selectedPlane ? (img->roi->coi - 1)*m->step*img->height : 0) +
            img->roi->yOffset*m->step[0] + img->roi->xOffset*esz;
    }
    m.datalimit = m.datastart + m->step.p[0]*m->rows;
    m.dataend = m.datastart + m->step.p[0]*(m->rows-1) + esz*m->cols;
    m.flags |= (m->cols*esz == m->step.p[0] || m->rows == 1 ? img_t::CONTINUOUS_FLAG : 0);
    m->step[1] = esz;

    if( copyData )
    {
        img_t m2 = m;
        m.release();
        if( !img->roi || !img->roi->coi ||
            img->dataOrder == IMG_DATA_ORDER_PLANE)
            m2.copyTo(m);
        else
        {
            int ch[] = {img->roi->coi - 1, 0};
            m cvSetMat(m2->rows, m2->cols, m2 CC_MAT_TYPE());
            mixChannels(&m2, 1, &m, 1, ch, 1);
        }
    }

    return m;
}

img_t img_t::diag(int d) const
{
    CC_Assert( dims <= 2 );
    img_t m = *this;
    size_t esz = elemSize();
    int len;

    if( d >= 0 )
    {
        len = MIN(cols - d, rows);
        m.data += esz*d;
    }
    else
    {
        len = MIN(rows + d, cols);
        m.data -= step[0]*d;
    }
    CC_DbgAssert( len > 0 );

    m.size[0] = m->rows = len;
    m.size[1] = m->cols = 1;
    m->step[0] += (len > 1 ? esz : 0);

    if( m->rows > 1 )
        m.flags &= ~CONTINUOUS_FLAG;
    else
        m.flags |= CONTINUOUS_FLAG;

    if( size() != CSize(1,1) )
        m.flags |= SUBMATRIX_FLAG;

    return m;
}

void img_t::pop_back(size_t nelems)
{
    CC_Assert( nelems <= (size_t)size.p[0] );

    if( isSubmatrix() )
        *this = rowRange(0, size.p[0] - (int)nelems);
    else
    {
        size.p[0] -= (int)nelems;
        dataend -= nelems*step.p[0];
        /*if( size.p[0] <= 1 )
        {
            if( dims <= 2 )
                flags |= CONTINUOUS_FLAG;
            else
                updateContinuityFlag(*this);
        }*/
    }
}


void img_t::push_back_(const void* elem)
{
    int r = size.p[0];
    if( isSubmatrix() || dataend + step.p[0] > datalimit )
        reserve( MAX(r + 1, (r*3+1)/2) );

    size_t esz = elemSize();
    memcpy(data + r*step.p[0], elem, esz);
    size.p[0] = r + 1;
    dataend += step.p[0];
    if( esz < step.p[0] )
        flags &= ~CONTINUOUS_FLAG;
}

void img_t::reserve(size_t nelems)
{
    const size_t MIN_SIZE = 64;

    CC_Assert( (int)nelems >= 0 );
    if( !isSubmatrix() && data + step.p[0]*nelems <= datalimit )
        return;

    int r = size.p[0];

    if( (size_t)r >= nelems )
        return;

    size.p[0] = MAX((int)nelems, 1);
    size_t newsize = total()*elemSize();

    if( newsize < MIN_SIZE )
        size.p[0] = (int)((MIN_SIZE + newsize - 1)*nelems/newsize);

    img_t m(dims, size.p, type());
    size.p[0] = r;
    if( r > 0 )
    {
        img_t mpart = m cvGetRows(,0, r);
        copyTo(mpart);
    }

    *this = m;
    size.p[0] = r;
    dataend = data + step.p[0]*r;
}


void img_t::resize(size_t nelems)
{
    int saveRows = size.p[0];
    if( saveRows == (int)nelems )
        return;
    CC_Assert( (int)nelems >= 0 );

    if( isSubmatrix() || data + step.p[0]*nelems > datalimit )
        reserve(nelems);

    size.p[0] = (int)nelems;
    dataend += (size.p[0] - saveRows)*step.p[0];

    //updateContinuityFlag(*this);
}


void img_t::resize(size_t nelems, const Scalar& s)
{
    int saveRows = size.p[0];
    resize(nelems);

    if( size.p[0] > saveRows )
    {
        img_t part = rowRange(saveRows, size.p[0]);
        part = s;
    }
}

void img_t::push_back(const img_t& elems)
{
    int r = size.p[0], delta = elems.size.p[0];
    if( delta == 0 )
        return;
    if( this == &elems )
    {
        img_t tmp = elems;
        push_back(tmp);
        return;
    }
    if( !data )
    {
        *this = elems.clone();
        return;
    }

    size.p[0] = elems.size.p[0];
    bool eq = size == elems.size;
    size.p[0] = r;
    if( !eq )
        CC_Error(CC_StsUnmatchedSizes, "Pushed vector length is not equal to matrix row length");
    if( type() != elems CC_MAT_TYPE() )
        CC_Error(CC_StsUnmatchedFormats, "Pushed vector type is not the same as matrix type");

    if( isSubmatrix() || dataend + step.p[0]*delta > datalimit )
        reserve( MAX(r + delta, (r*3+1)/2) );

    size.p[0] += delta;
    dataend += step.p[0]*delta;

    //updateContinuityFlag(*this);

    if( isContinuous() && elems CC_IS_CONT_MAT() )
        memcpy(data + r*step.p[0], elems.data, elems CC_MAT_TOTAL()*elems CC_TYPE_SIZE());
    else
    {
        img_t part = rowRange(r, r + delta);
        elems.copyTo(part);
    }
}


img_t cvarrToMat(const img_t* arr, bool copyData,
               bool /*allowND*/, int coiMode, FREE(); double* = MALLOC(double, )* abuf )
{
    if( !arr )
        return img_t();
    if( CC_IS_MAT_HDR_Z(arr) )
        return cvMatToMat((const img_t*)arr, copyData);
    if( CC_IS_MATND(arr) )
        return cvMatNDToMat((const CvMatND*)arr, copyData );
    if( CC_IS_IMAGE(arr) )
    {
        const img_t* iplimg = (const img_t*)arr;
        if( coiMode == 0 && iplimg->roi && iplimg->roi->coi > 0 )
            CC_Error(CC_BadCOI, "COI is not supported by the function");
        return iplImageToMat(iplimg, copyData);
    }
    if( CC_IS_SEQ(arr) )
    {
        CvSeq* seq = (CvSeq*)arr;
        int total = seq->total, type = CC_MAT_DEPTH(seq->flags), esz = seq->elem_size;
        if( total == 0 )
            return img_t();
        CC_Assert(total > 0 && CC_TYPE_SIZE(seq->flags) == esz);
        if(!copyData && seq->first->next == seq->first)
            return img_t(total, 1, type, seq->first->data);
        if( abuf )
        {
            abuf->allocate(((size_t)total*esz + sizeof(double)-1)/sizeof(double));
            double* bufdata = *abuf;
            cvCvtSeqToArray(seq, bufdata, CC_WHOLE_SEQ);
            return img_t(total, 1, type, bufdata);
        }

        img_t buf(total, 1, type);
        cvCvtSeqToArray(seq, buf->tt.data, CC_WHOLE_SEQ);
        return buf;
    }
    CC_Error(CC_StsBadArg, "Unknown array type");
    return img_t();
}

void img_t::locateROI( CSize& wholeSize, Point& ofs ) const
{
    CC_Assert( dims <= 2 && step[0] > 0 );
    size_t esz = elemSize(), minstep;
    ptrdiff_t delta1 = data - datastart, delta2 = dataend - datastart;

    if( delta1 == 0 )
        ofs.x = ofs.y = 0;
    else
    {
        ofs.y = (int)(delta1/step[0]);
        ofs.x = (int)((delta1 - step[0]*ofs.y)/esz);
        CC_DbgAssert( data == datastart + ofs.y*step[0] + ofs.x*esz );
    }
    minstep = (ofs.x + cols)*esz;
    wholeSize.height = (int)((delta2 - minstep)/step[0] + 1);
    wholeSize.height = MAX(wholeSize.height, ofs.y + rows);
    wholeSize.width = (int)((delta2 - step*(wholeSize.height-1))/esz);
    wholeSize.width = MAX(wholeSize.width, ofs.x + cols);
}

img_t& img_t::adjustROI( int dtop, int dbottom, int dleft, int dright )
{
    CC_Assert( dims <= 2 && step[0] > 0 );
    CSize wholeSize; Point ofs;
    size_t esz = elemSize();
    locateROI( wholeSize, ofs );
    int row1 = MAX(ofs.y - dtop, 0), row2 = MIN(ofs.y + rows + dbottom, wholeSize.height);
    int col1 = MAX(ofs.x - dleft, 0), col2 = MIN(ofs.x + cols + dright, wholeSize.width);
    data += (row1 - ofs.y)*step + (col1 - ofs.x)*esz;
    rows = row2 - row1; cols = col2 - col1;
    size.p[0] = rows; size.p[1] = cols;
    if( esz*cols == step[0] || rows == 1 )
        flags |= CONTINUOUS_FLAG;
    else
        flags &= ~CONTINUOUS_FLAG;
    return *this;
}



void extractImageCOI(const img_t* arr, img_t* _ch, int coi)
{
    img_t mat = cvarrToMat(arr, false, true, 1);
    _ch cvSetMat(mat.dims, mat.size, mat CC_MAT_DEPTH());
    img_t ch = _ch;
    if(coi < 0)
    {
        CC_Assert( CC_IS_IMAGE(arr) );
        coi = cvGetImageCOI((const img_t*)arr)-1;
    }
    CC_Assert(0 <= coi && coi < mat CC_MAT_CN());
    int _pairs[] = { coi, 0 };
    mixChannels( &mat, 1, &ch, 1, _pairs, 1 );
}

void insertImageCOI(const img_t* _ch, img_t* arr, int coi)
{
    img_t ch = _ch, mat = cvarrToMat(arr, false, true, 1);
    if(coi < 0)
    {
        CC_Assert( CC_IS_IMAGE(arr) );
        coi = cvGetImageCOI((const img_t*)arr)-1;
    }
    CC_Assert(ch.size == mat.size && ch CC_MAT_DEPTH() == mat CC_MAT_DEPTH() && 0 <= coi && coi < mat CC_MAT_CN());
    int _pairs[] = { 0, coi };
    mixChannels( &ch, 1, &mat, 1, _pairs, 1 );
}


img_t img_t::reshape(int new_cn, int new_rows) const
{
    int cn = CC_MAT_CN();
    img_t hdr = *this;

    if( dims > 2 && new_rows == 0 && new_cn != 0 && size[dims-1]*cn % new_cn == 0 )
    {
        hdr.flags = (hdr.flags & ~CC_MAT_CN_MASK) | ((new_cn-1) << CC_CN_SHIFT);
        hdr->step[dims-1] = CC_TYPE_SIZE(hdr.flags);
        hdr.size[dims-1] = hdr.size[dims-1]*cn / new_cn;
        return hdr;
    }

    CC_Assert( dims <= 2 );

    if( new_cn == 0 )
        new_cn = cn;

    int total_width = cols * cn;

    if( (new_cn > total_width || total_width % new_cn != 0) && new_rows == 0 )
        new_rows = rows * total_width / new_cn;

    if( new_rows != 0 && new_rows != rows )
    {
        int total_size = total_width * rows;
        if( !isContinuous() )
            CC_Error( CC_BadStep,
            "The matrix is not continuous, thus its number of rows can not be changed" );

        if( (unsigned)new_rows > (unsigned)total_size )
            CC_Error( CC_StsOutOfRange, "Bad new number of rows" );

        total_width = total_size / new_rows;

        if( total_width * new_rows != total_size )
            CC_Error( CC_StsBadArg, "The total number of matrix elements "
                                    "is not divisible by the new number of rows" );

        hdr->rows = new_rows;
        hdr->step[0] = total_width * elemSize1();
    }

    int new_width = total_width / new_cn;

    if( new_width * new_cn != total_width )
        CC_Error( CC_BadNumChannels,
        "The total width is not divisible by the new number of channels" );

    hdr->cols = new_width;
    hdr.flags = (hdr.flags & ~CC_MAT_CN_MASK) | ((new_cn-1) << CC_CN_SHIFT);
    hdr->step[1] = CC_TYPE_SIZE(hdr.flags);
    return hdr;
}

img_t img_t::diag(const img_t& d)
{
    CC_Assert( d->cols == 1 || d->rows == 1 );
    int len = d->rows + d->cols - 1;
    img_t m(len, len, d CC_MAT_TYPE(), Scalar(0));
    img_t md = m.diag();
    if( d->cols == 1 )
        d.copyTo(md);
    else
        transpose(d, md);
    return m;
}

int img_t::cvCheckVector(int _elemChannels, int _depth, bool _requireContinuous) const
{
    return (depth() == _depth || _depth <= 0) &&
        (isContinuous() || !_requireContinuous) &&
        ((dims == 2 && (((rows == 1 || cols == 1) && CC_MAT_CN() == _elemChannels) ||
                        (cols == _elemChannels && CC_MAT_CN() == 1))) ||
        (dims == 3 && CC_MAT_CN() == 1 && size.p[2] == _elemChannels && (size.p[0] == 1 || size.p[1] == 1) &&
         (isContinuous() || step.p[1] == step.p[2]*size.p[2])))
    ? (int)(total()*CC_MAT_CN()/_elemChannels) : -1;
}


void scalarToRawData(const Scalar& s, void* _buf, int type, int unroll_to)
{
    CC_INSTRUMENT_REGION()

    int i, depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type);
    CC_Assert(cn <= 4);
    switch(depth)
    {
    case CC_8U:
        {
        uchar* buf = (uchar*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<uchar>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        }
        break;
    case CC_8S:
        {
        schar* buf = (schar*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<schar>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        }
        break;
    case CC_16U:
        {
        ushort* buf = (ushort*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<ushort>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        }
        break;
    case CC_16S:
        {
        short* buf = (short*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<short>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        }
        break;
    case CC_32S:
        {
        int* buf = (int*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<int>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        }
        break;
    case CC_32F:
        {
        float* buf = (float*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<float>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        }
        break;
    case CC_64F:
        {
        double* buf = (double*)_buf;
        for(i = 0; i < cn; i++)
            buf[i] = saturate_cast<double>(s.val[i]);
        for(; i < unroll_to; i++)
            buf[i] = buf[i-cn];
        break;
        }
    default:
        CC_Error(CC_StsUnsupportedFormat,"");
    }
}


/*************************************************************************************************\
                                        Input/Output Array
\*************************************************************************************************/

img_t _InputArray::getMat_(int i) const
{
    int k = kind();
    int accessFlags = flags & ACCESS_MASK;

    if( k == MAT )
    {
        const img_t* m = (const img_t*)obj;
        if( i < 0 )
            return *m;
        return m->row(i);
    }

    if( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        if( i < 0 )
            return m->getMat(accessFlags);
        return m->getMat(accessFlags).row(i);
    }

    if( k == EXPR )
    {
        CC_Assert( i < 0 );
        return (img_t)*((const MatExpr*)obj);
    }

    if( k == MATX )
    {
        CC_Assert( i < 0 );
        return img_t(sz, flags, obj);
    }

    if( k == STD_VECTOR )
    {
        CC_Assert( i < 0 );
        int t = CC_MAT_TYPE(flags);
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;

        return !v.empty() ? img_t(size(), t, (void*)&v[0]) : img_t();
    }

    if( k == STD_BOOL_VECTOR )
    {
        CC_Assert( i < 0 );
        int t = CC_8U;
        const std::vector<bool>& v = *(const std::vector<bool>*)obj;
        int j, n = (int)v.size();
        if( n == 0 )
            return img_t();
        img_t m(1, n, t);
        uchar* dst = m.data;
        for( j = 0; j < n; j++ )
            dst[j] = (uchar)v[j];
        return m;
    }

    if( k == NONE )
        return img_t();

    if( k == STD_VECTOR_VECTOR )
    {
        int t = type(i);
        const std::vector<std::vector<uchar> >& vv = *(const std::vector<std::vector<uchar> >*)obj;
        CC_Assert( 0 <= i && i < (int)vv.size() );
        const std::vector<uchar>& v = vv[i];

        return !v.empty() ? img_t(size(i), t, (void*)&v[0]) : img_t();
    }

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<img_t>& v = *(const std::vector<img_t>*)obj;
        CC_Assert( 0 <= i && i < (int)v.size() );

        return v[i];
    }

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CC_Assert( 0 <= i && i < (int)v.size() );

        return v[i].getMat(accessFlags);
    }

    if( k == OPENGL_BUFFER )
    {
        CC_Assert( i < 0 );
        CC_Error(CC_StsNotImplemented, "You should explicitly call mapHost/unmapHost methods for ogl::Buffer object");
        return img_t();
    }

    if( k == CUDA_GPU_MAT )
    {
        CC_Assert( i < 0 );
        CC_Error(CC_StsNotImplemented, "You should explicitly call download method for cuda::GpuMat object");
        return img_t();
    }

    if( k == CUDA_HOST_MEM )
    {
        CC_Assert( i < 0 );

        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;

        return cuda_mem->createMatHeader();
    }

    CC_Error(CC_StsNotImplemented, "Unknown/unsupported array type");
    return img_t();
}

UMat _InputArray::getUMat(int i) const
{
    int k = kind();
    int accessFlags = flags & ACCESS_MASK;

    if( k == UMAT )
    {
        const UMat* m = (const UMat*)obj;
        if( i < 0 )
            return *m;
        return m->row(i);
    }

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        CC_Assert( 0 <= i && i < (int)v.size() );

        return v[i];
    }

    if( k == MAT )
    {
        const img_t* m = (const img_t*)obj;
        if( i < 0 )
            return m->getUMat(accessFlags);
        return m->row(i).getUMat(accessFlags);
    }

    return getMat(i).getUMat(accessFlags);
}

void _InputArray::getMatVector(std::vector<img_t>& mv) const
{
    int k = kind();
    int accessFlags = flags & ACCESS_MASK;

    if( k == MAT )
    {
        const img_t& m = *(const img_t*)obj;
        int n = (int)m.size[0];
        mv.resize(n);

        for( int i = 0; i < n; i++ )
            mv[i] = m.dims == 2 ? img_t(1, m->cols, m CC_MAT_TYPE(), (void*)m.ptr(i)) :
                img_t(m.dims-1, &m.size[1], m CC_MAT_TYPE(), (void*)m.ptr(i), &m->step[1]);
        return;
    }

    if( k == EXPR )
    {
        img_t m = *(const MatExpr*)obj;
        int n = m.size[0];
        mv.resize(n);

        for( int i = 0; i < n; i++ )
            mv[i] = m.row(i);
        return;
    }

    if( k == MATX )
    {
        size_t n = sz.height, esz = CC_TYPE_SIZE(flags);
        mv.resize(n);

        for( size_t i = 0; i < n; i++ )
            mv[i] = img_t(1, sz.width, CC_MAT_TYPE(flags), (uchar*)obj + esz*sz.width*i);
        return;
    }

    if( k == STD_VECTOR )
    {
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;

        size_t n = v.size(), esz = CC_TYPE_SIZE(flags);
        int t = CC_MAT_DEPTH(flags), cn = CC_MAT_CN(flags);
        mv.resize(n);

        for( size_t i = 0; i < n; i++ )
            mv[i] = img_t(1, cn, t, (void*)(&v[0] + esz*i));
        return;
    }

    if( k == NONE )
    {
        mv.clear();
        return;
    }

    if( k == STD_VECTOR_VECTOR )
    {
        const std::vector<std::vector<uchar> >& vv = *(const std::vector<std::vector<uchar> >*)obj;
        int n = (int)vv.size();
        int t = CC_MAT_TYPE(flags);
        mv.resize(n);

        for( int i = 0; i < n; i++ )
        {
            const std::vector<uchar>& v = vv[i];
            mv[i] = img_t(size(i), t, (void*)&v[0]);
        }
        return;
    }

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<img_t>& v = *(const std::vector<img_t>*)obj;
        size_t n = v.size();
        mv.resize(n);

        for( size_t i = 0; i < n; i++ )
            mv[i] = v[i];
        return;
    }

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        mv.resize(n);

        for( size_t i = 0; i < n; i++ )
            mv[i] = v[i].getMat(accessFlags);
        return;
    }

    CC_Error(CC_StsNotImplemented, "Unknown/unsupported array type");
}

void _InputArray::getUMatVector(std::vector<UMat>& umv) const
{
    int k = kind();
    int accessFlags = flags & ACCESS_MASK;

    if( k == NONE )
    {
        umv.clear();
        return;
    }

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<img_t>& v = *(const std::vector<img_t>*)obj;
        size_t n = v.size();
        umv.resize(n);

        for( size_t i = 0; i < n; i++ )
            umv[i] = v[i].getUMat(accessFlags);
        return;
    }

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& v = *(const std::vector<UMat>*)obj;
        size_t n = v.size();
        umv.resize(n);

        for( size_t i = 0; i < n; i++ )
            umv[i] = v[i];
        return;
    }

    if( k == UMAT )
    {
        UMat& v = *(UMat*)obj;
        umv.resize(1);
        umv[0] = v;
        return;
    }
    if( k == MAT )
    {
        img_t& v = *(img_t*)obj;
        umv.resize(1);
        umv[0] = v.getUMat(accessFlags);
        return;
    }

    CC_Error(CC_StsNotImplemented, "Unknown/unsupported array type");
}

cuda::GpuMat _InputArray::getGpuMat() const
{
    int k = kind();

    if (k == CUDA_GPU_MAT)
    {
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        return *d_mat;
    }

    if (k == CUDA_HOST_MEM)
    {
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        return cuda_mem->createGpuMatHeader();
    }

    if (k == OPENGL_BUFFER)
    {
        CC_Error(CC_StsNotImplemented, "You should explicitly call mapDevice/unmapDevice methods for ogl::Buffer object");
        return cuda::GpuMat();
    }

    if (k == NONE)
        return cuda::GpuMat();

    CC_Error(CC_StsNotImplemented, "getGpuMat is available only for cuda::GpuMat and cuda::HostMem");
    return cuda::GpuMat();
}
void _InputArray::getGpuMatVector(std::vector<cuda::GpuMat>& gpumv) const
{
    int k = kind();
    if (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        gpumv = *(std::vector<cuda::GpuMat>*)obj;
    }
}
ogl::Buffer _InputArray::getOGlBuffer() const
{
    int k = kind();

    CC_Assert(k == OPENGL_BUFFER);

    const ogl::Buffer* gl_buf = (const ogl::Buffer*)obj;
    return *gl_buf;
}

int _InputArray::kind() const
{
    return flags & KIND_MASK;
}

int _InputArray::rows(int i) const
{
    return size(i).height;
}

int _InputArray::cols(int i) const
{
    return size(i).width;
}

CSize _InputArray::size(int i) const
{
    int k = kind();

    if( k == MAT )
    {
        CC_Assert( i < 0 );
        return ((const img_t*)obj)->size();
    }

    if( k == EXPR )
    {
        CC_Assert( i < 0 );
        return ((const MatExpr*)obj)->size();
    }

    if( k == UMAT )
    {
        CC_Assert( i < 0 );
        return ((const UMat*)obj)->size();
    }

    if( k == MATX )
    {
        CC_Assert( i < 0 );
        return sz;
    }

    if( k == STD_VECTOR )
    {
        CC_Assert( i < 0 );
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;
        const std::vector<int>& iv = *(const std::vector<int>*)obj;
        size_t szb = v.size(), szi = iv.size();
        return szb == szi ? CSize((int)szb, 1) : CSize((int)(szb/CC_TYPE_SIZE(flags)), 1);
    }

    if( k == STD_BOOL_VECTOR )
    {
        CC_Assert( i < 0 );
        const std::vector<bool>& v = *(const std::vector<bool>*)obj;
        return CSize((int)v.size(), 1);
    }

    if( k == NONE )
        return CSize();

    if( k == STD_VECTOR_VECTOR )
    {
        const std::vector<std::vector<uchar> >& vv = *(const std::vector<std::vector<uchar> >*)obj;
        if( i < 0 )
            return vv.empty() ? CSize() : CSize((int)vv.size(), 1);
        CC_Assert( i < (int)vv.size() );
        const std::vector<std::vector<int> >& ivv = *(const std::vector<std::vector<int> >*)obj;

        size_t szb = vv[i].size(), szi = ivv[i].size();
        return szb == szi ? CSize((int)szb, 1) : CSize((int)(szb/CC_TYPE_SIZE(flags)), 1);
    }

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<img_t>& vv = *(const std::vector<img_t>*)obj;
        if( i < 0 )
            return vv.empty() ? CSize() : CSize((int)vv.size(), 1);
        CC_Assert( i < (int)vv.size() );

        return vv[i].size();
    }

    if (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        if (i < 0)
            return vv.empty() ? CSize() : CSize((int)vv.size(), 1);
        CC_Assert(i < (int)vv.size());
        return vv[i].size();
    }

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        if( i < 0 )
            return vv.empty() ? CSize() : CSize((int)vv.size(), 1);
        CC_Assert( i < (int)vv.size() );

        return vv[i].size();
    }

    if( k == OPENGL_BUFFER )
    {
        CC_Assert( i < 0 );
        const ogl::Buffer* buf = (const ogl::Buffer*)obj;
        return buf->size();
    }

    if( k == CUDA_GPU_MAT )
    {
        CC_Assert( i < 0 );
        const cuda::GpuMat* d_mat = (const cuda::GpuMat*)obj;
        return d_mat->size();
    }

    if( k == CUDA_HOST_MEM )
    {
        CC_Assert( i < 0 );
        const cuda::HostMem* cuda_mem = (const cuda::HostMem*)obj;
        return cuda_mem->size();
    }

    CC_Error(CC_StsNotImplemented, "Unknown/unsupported array type");
    return CSize();
}

int _InputArray::sizend(int* arrsz, int i) const
{
    int j, d=0, k = kind();

    if( k == NONE )
        ;
    else if( k == MAT )
    {
        CC_Assert( i < 0 );
        const img_t& m = *(const img_t*)obj;
        d = m.dims;
        if(arrsz)
            for(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    else if( k == UMAT )
    {
        CC_Assert( i < 0 );
        const UMat& m = *(const UMat*)obj;
        d = m.dims;
        if(arrsz)
            for(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    else if( k == STD_VECTOR_MAT && i >= 0 )
    {
        const std::vector<img_t>& vv = *(const std::vector<img_t>*)obj;
        CC_Assert( i < (int)vv.size() );
        const img_t& m = vv[i];
        d = m.dims;
        if(arrsz)
            for(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    else if( k == STD_VECTOR_UMAT && i >= 0 )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CC_Assert( i < (int)vv.size() );
        const UMat& m = vv[i];
        d = m.dims;
        if(arrsz)
            for(j = 0; j < d; j++)
                arrsz[j] = m.size.p[j];
    }
    else
    {
        CSize sz2d = size(i);
        d = 2;
        if(arrsz)
        {
            arrsz[0] = sz2d.height;
            arrsz[1] = sz2d.width;
        }
    }

    return d;
}

bool _InputArray::sameSize(const _InputArray& arr) const
{
    int k1 = kind(), k2 = arr.kind();
    CSize sz1;

    if( k1 == MAT )
    {
        const img_t* m = ((const img_t*)obj);
        if( k2 == MAT )
            return m->size == ((const img_t*)arr.obj)->size;
        if( k2 == UMAT )
            return m->size == ((const UMat*)arr.obj)->size;
        if( m->dims > 2 )
            return false;
        sz1 = m->size();
    }
    else if( k1 == UMAT )
    {
        const UMat* m = ((const UMat*)obj);
        if( k2 == MAT )
            return m->size == ((const img_t*)arr.obj)->size;
        if( k2 == UMAT )
            return m->size == ((const UMat*)arr.obj)->size;
        if( m->dims > 2 )
            return false;
        sz1 = m->size();
    }
    else
        sz1 = size();
    if( arr.dims() > 2 )
        return false;
    return sz1 == arr.size();
}

int _InputArray::dims(int i) const
{
    int k = kind();

    if( k == MAT )
    {
        CC_Assert( i < 0 );
        return ((const img_t*)obj)->dims;
    }

    if( k == EXPR )
    {
        CC_Assert( i < 0 );
        return ((const MatExpr*)obj)->a.dims;
    }

    if( k == UMAT )
    {
        CC_Assert( i < 0 );
        return ((const UMat*)obj)->dims;
    }

    if( k == MATX )
    {
        CC_Assert( i < 0 );
        return 2;
    }

    if( k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        CC_Assert( i < 0 );
        return 2;
    }

    if( k == NONE )
        return 0;

    if( k == STD_VECTOR_VECTOR )
    {
        const std::vector<std::vector<uchar> >& vv = *(const std::vector<std::vector<uchar> >*)obj;
        if( i < 0 )
            return 1;
        CC_Assert( i < (int)vv.size() );
        return 2;
    }

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<img_t>& vv = *(const std::vector<img_t>*)obj;
        if( i < 0 )
            return 1;
        CC_Assert( i < (int)vv.size() );

        return vv[i].dims;
    }

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        if( i < 0 )
            return 1;
        CC_Assert( i < (int)vv.size() );

        return vv[i].dims;
    }

    if( k == OPENGL_BUFFER )
    {
        CC_Assert( i < 0 );
        return 2;
    }

    if( k == CUDA_GPU_MAT )
    {
        CC_Assert( i < 0 );
        return 2;
    }

    if( k == CUDA_HOST_MEM )
    {
        CC_Assert( i < 0 );
        return 2;
    }

    CC_Error(CC_StsNotImplemented, "Unknown/unsupported array type");
    return 0;
}

size_t _InputArray::total(int i) const
{
    int k = kind();

    if( k == MAT )
    {
        CC_Assert( i < 0 );
        return ((const img_t*)obj)->total();
    }

    if( k == UMAT )
    {
        CC_Assert( i < 0 );
        return ((const UMat*)obj)->total();
    }

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<img_t>& vv = *(const std::vector<img_t>*)obj;
        if( i < 0 )
            return vv.size();

        CC_Assert( i < (int)vv.size() );
        return vv[i] CC_MAT_TOTAL();
    }


    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        if( i < 0 )
            return vv.size();

        CC_Assert( i < (int)vv.size() );
        return vv[i] CC_MAT_TOTAL();
    }

    return size(i).area();
}

int _InputArray::type(int i) const
{
    int k = kind();

    if( k == MAT )
        return ((const img_t*)obj)->tid();

    if( k == UMAT )
        return ((const UMat*)obj)->tid();

    if( k == EXPR )
        return ((const MatExpr*)obj)->tid();

    if( k == MATX || k == STD_VECTOR || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        return CC_MAT_TYPE(flags);

    if( k == NONE )
        return -1;

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        if( vv.empty() )
        {
            CC_Assert((flags & FIXED_TYPE) != 0);
            return CC_MAT_TYPE(flags);
        }
        CC_Assert( i < (int)vv.size() );
        return vv[i >= 0 ? i : 0] CC_MAT_TYPE();
    }

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<img_t>& vv = *(const std::vector<img_t>*)obj;
        if( vv.empty() )
        {
            CC_Assert((flags & FIXED_TYPE) != 0);
            return CC_MAT_TYPE(flags);
        }
        CC_Assert( i < (int)vv.size() );
        return vv[i >= 0 ? i : 0] CC_MAT_TYPE();
    }

    if (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        if (vv.empty())
        {
            CC_Assert((flags & FIXED_TYPE) != 0);
            return CC_MAT_TYPE(flags);
        }
        CC_Assert(i < (int)vv.size());
        return vv[i >= 0 ? i : 0] CC_MAT_TYPE();
    }

    if( k == OPENGL_BUFFER )
        return ((const ogl::Buffer*)obj)->tid();

    if( k == CUDA_GPU_MAT )
        return ((const cuda::GpuMat*)obj)->tid();

    if( k == CUDA_HOST_MEM )
        return ((const cuda::HostMem*)obj)->tid();

    CC_Error(CC_StsNotImplemented, "Unknown/unsupported array type");
    return 0;
}

int _InputArray::depth(int i) const
{
    return CC_MAT_DEPTH(type(i));
}

int _InputArray::channels(int i) const
{
    return CC_MAT_CN(type(i));
}

bool _InputArray::empty() const
{
    int k = kind();

    if( k == MAT )
        return ((const img_t*)obj)->empty();

    if( k == UMAT )
        return ((const UMat*)obj)->empty();

    if( k == EXPR )
        return false;

    if( k == MATX )
        return false;

    if( k == STD_VECTOR )
    {
        const std::vector<uchar>& v = *(const std::vector<uchar>*)obj;
        return v.empty();
    }

    if( k == STD_BOOL_VECTOR )
    {
        const std::vector<bool>& v = *(const std::vector<bool>*)obj;
        return v.empty();
    }

    if( k == NONE )
        return true;

    if( k == STD_VECTOR_VECTOR )
    {
        const std::vector<std::vector<uchar> >& vv = *(const std::vector<std::vector<uchar> >*)obj;
        return vv.empty();
    }

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<img_t>& vv = *(const std::vector<img_t>*)obj;
        return vv.empty();
    }

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        return vv.empty();
    }

    if( k == OPENGL_BUFFER )
        return ((const ogl::Buffer*)obj)->empty();

    if( k == CUDA_GPU_MAT )
        return ((const cuda::GpuMat*)obj)->empty();

    if (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        return vv.empty();
    }

    if( k == CUDA_HOST_MEM )
        return ((const cuda::HostMem*)obj)->empty();

    CC_Error(CC_StsNotImplemented, "Unknown/unsupported array type");
    return true;
}

bool _InputArray::isContinuous(int i) const
{
    int k = kind();

    if( k == MAT )
        return i < 0 ? ((const img_t*)obj)->isContinuous() : true;

    if( k == UMAT )
        return i < 0 ? ((const UMat*)obj)->isContinuous() : true;

    if( k == EXPR || k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        return true;

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<img_t>& vv = *(const std::vector<img_t>*)obj;
        CC_Assert((size_t)i < vv.size());
        return vv[i] CC_IS_CONT_MAT();
    }

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CC_Assert((size_t)i < vv.size());
        return vv[i] CC_IS_CONT_MAT();
    }

    if( k == CUDA_GPU_MAT )
      return i < 0 ? ((const cuda::GpuMat*)obj)->isContinuous() : true;

    CC_Error(CC_StsNotImplemented, "Unknown/unsupported array type");
    return false;
}

bool _InputArray::isSubmatrix(int i) const
{
    int k = kind();

    if( k == MAT )
        return i < 0 ? ((const img_t*)obj)->isSubmatrix() : false;

    if( k == UMAT )
        return i < 0 ? ((const UMat*)obj)->isSubmatrix() : false;

    if( k == EXPR || k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        return false;

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<img_t>& vv = *(const std::vector<img_t>*)obj;
        CC_Assert((size_t)i < vv.size());
        return vv[i].isSubmatrix();
    }

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CC_Assert((size_t)i < vv.size());
        return vv[i].isSubmatrix();
    }

    CC_Error(CC_StsNotImplemented, "");
    return false;
}

size_t _InputArray::offset(int i) const
{
    int k = kind();

    if( k == MAT )
    {
        CC_Assert( i < 0 );
        const img_t * const m = ((const img_t*)obj);
        return (size_t)(m->ptr() - m->datastart);
    }

    if( k == UMAT )
    {
        CC_Assert( i < 0 );
        return ((const UMat*)obj)->offset;
    }

    if( k == EXPR || k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        return 0;

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<img_t>& vv = *(const std::vector<img_t>*)obj;
        if( i < 0 )
            return 1;
        CC_Assert( i < (int)vv.size() );

        return (size_t)(vv[i]->tt.data - vv[i].datastart);
    }

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CC_Assert((size_t)i < vv.size());
        return vv[i].offset;
    }

    if( k == CUDA_GPU_MAT )
    {
        CC_Assert( i < 0 );
        const cuda::GpuMat * const m = ((const cuda::GpuMat*)obj);
        return (size_t)(m->data - m->datastart);
    }

    if (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CC_Assert((size_t)i < vv.size());
        return (size_t)(vv[i].data - vv[i].datastart);
    }

    CC_Error(CC_StsNotImplemented, "");
    return 0;
}

size_t _InputArray::step(int i) const
{
    int k = kind();

    if( k == MAT )
    {
        CC_Assert( i < 0 );
        return ((const img_t*)obj)->step;
    }

    if( k == UMAT )
    {
        CC_Assert( i < 0 );
        return ((const UMat*)obj)->step;
    }

    if( k == EXPR || k == MATX || k == STD_VECTOR ||
        k == NONE || k == STD_VECTOR_VECTOR || k == STD_BOOL_VECTOR )
        return 0;

    if( k == STD_VECTOR_MAT )
    {
        const std::vector<img_t>& vv = *(const std::vector<img_t>*)obj;
        if( i < 0 )
            return 1;
        CC_Assert( i < (int)vv.size() );
        return vv[i]->step;
    }

    if( k == STD_VECTOR_UMAT )
    {
        const std::vector<UMat>& vv = *(const std::vector<UMat>*)obj;
        CC_Assert((size_t)i < vv.size());
        return vv[i]->step;
    }

    if( k == CUDA_GPU_MAT )
    {
        CC_Assert( i < 0 );
        return ((const cuda::GpuMat*)obj)->step;
    }
    if (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        const std::vector<cuda::GpuMat>& vv = *(const std::vector<cuda::GpuMat>*)obj;
        CC_Assert((size_t)i < vv.size());
        return vv[i]->step;
    }

    CC_Error(CC_StsNotImplemented, "");
    return 0;
}

void _InputArray::copyTo(const _OutputArray& arr) const
{
    int k = kind();

    if( k == NONE )
        arr.release();
    else if( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        img_t m = getMat();
        m.copyTo(arr);
    }
    else if( k == EXPR )
    {
        const MatExpr& e = *((MatExpr*)obj);
        if( arr.kind() == MAT )
            arr.getMatRef() = e;
        else
            img_t(e).copyTo(arr);
    }
    else if( k == UMAT )
        ((UMat*)obj)->copyTo(arr);
    else
        CC_Error(CC_StsNotImplemented, "");
}

void _InputArray::copyTo(const _OutputArray& arr, const _InputArray & mask) const
{
    int k = kind();

    if( k == NONE )
        arr.release();
    else if( k == MAT || k == MATX || k == STD_VECTOR || k == STD_BOOL_VECTOR )
    {
        img_t m = getMat();
        m.copyTo(arr, mask);
    }
    else if( k == UMAT )
        ((UMat*)obj)->copyTo(arr, mask);
    else
        CC_Error(CC_StsNotImplemented, "");
}

bool _OutputArray::fixedSize() const
{
    return (flags & FIXED_SIZE) == FIXED_SIZE;
}

bool _OutputArray::fixedType() const
{
    return (flags & FIXED_TYPE) == FIXED_TYPE;
}

void _OutputArray::create(CSize _sz, int mtype, int i, bool allowTransposed, int fixedDepthMask) const
{
    int k = kind();
    if( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CC_Assert(!fixedSize() || ((img_t*)obj)->size.operator()() == _sz);
        CC_Assert(!fixedType() || ((img_t*)obj)->tid() == mtype);
        ((img_t*)obj)->create(_sz, mtype);
        return;
    }
    if( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CC_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == _sz);
        CC_Assert(!fixedType() || ((UMat*)obj)->tid() == mtype);
        ((UMat*)obj)->create(_sz, mtype);
        return;
    }
    if( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CC_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == _sz);
        CC_Assert(!fixedType() || ((cuda::GpuMat*)obj)->tid() == mtype);
        ((cuda::GpuMat*)obj)->create(_sz, mtype);
        return;
    }
    if( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CC_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == _sz);
        CC_Assert(!fixedType() || ((ogl::Buffer*)obj)->tid() == mtype);
        ((ogl::Buffer*)obj)->create(_sz, mtype);
        return;
    }
    if( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CC_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == _sz);
        CC_Assert(!fixedType() || ((cuda::HostMem*)obj)->tid() == mtype);
        ((cuda::HostMem*)obj)->create(_sz, mtype);
        return;
    }
    int sizes[] = {_sz.height, _sz.width};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create(int _rows, int _cols, int mtype, int i, bool allowTransposed, int fixedDepthMask) const
{
    int k = kind();
    if( k == MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CC_Assert(!fixedSize() || ((img_t*)obj)->size.operator()() == CSize(_cols, _rows));
        CC_Assert(!fixedType() || ((img_t*)obj)->tid() == mtype);
        ((img_t*)obj)->create(_rows, _cols, mtype);
        return;
    }
    if( k == UMAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CC_Assert(!fixedSize() || ((UMat*)obj)->size.operator()() == CSize(_cols, _rows));
        CC_Assert(!fixedType() || ((UMat*)obj)->tid() == mtype);
        ((UMat*)obj)->create(_rows, _cols, mtype);
        return;
    }
    if( k == CUDA_GPU_MAT && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CC_Assert(!fixedSize() || ((cuda::GpuMat*)obj)->size() == CSize(_cols, _rows));
        CC_Assert(!fixedType() || ((cuda::GpuMat*)obj)->tid() == mtype);
        ((cuda::GpuMat*)obj)->create(_rows, _cols, mtype);
        return;
    }
    if( k == OPENGL_BUFFER && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CC_Assert(!fixedSize() || ((ogl::Buffer*)obj)->size() == CSize(_cols, _rows));
        CC_Assert(!fixedType() || ((ogl::Buffer*)obj)->tid() == mtype);
        ((ogl::Buffer*)obj)->create(_rows, _cols, mtype);
        return;
    }
    if( k == CUDA_HOST_MEM && i < 0 && !allowTransposed && fixedDepthMask == 0 )
    {
        CC_Assert(!fixedSize() || ((cuda::HostMem*)obj)->size() == CSize(_cols, _rows));
        CC_Assert(!fixedType() || ((cuda::HostMem*)obj)->tid() == mtype);
        ((cuda::HostMem*)obj)->create(_rows, _cols, mtype);
        return;
    }
    int sizes[] = {_rows, _cols};
    create(2, sizes, mtype, i, allowTransposed, fixedDepthMask);
}

void _OutputArray::create(int d, const int* sizes, int mtype, int i,
                          bool allowTransposed, int fixedDepthMask) const
{
    int k = kind();
    mtype = CC_MAT_DEPTH(mtype);

    if( k == MAT )
    {
        CC_Assert( i < 0 );
        img_t& m = *(img_t*)obj;
        if( allowTransposed )
        {
            if( !m CC_IS_CONT_MAT() )
            {
                CC_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            if( d == 2 && m.dims == 2 && m.data &&
                m CC_MAT_TYPE() == mtype && m->rows == sizes[1] && m->cols == sizes[0] )
                return;
        }

        if(fixedType())
        {
            if(CC_MAT_CN(mtype) == m CC_MAT_CN() && ((1 << CC_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m CC_MAT_TYPE();
            else
                CC_Assert(CC_MAT_TYPE(mtype) == m CC_MAT_TYPE());
        }
        if(fixedSize())
        {
            CC_Assert(m.dims == d);
            for(int j = 0; j < d; ++j)
                CC_Assert(m.size[j] == sizes[j]);
        }
        m cvSetMat(d, sizes, mtype);
        return;
    }

    if( k == UMAT )
    {
        CC_Assert( i < 0 );
        UMat& m = *(UMat*)obj;
        if( allowTransposed )
        {
            if( !m CC_IS_CONT_MAT() )
            {
                CC_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            if( d == 2 && m.dims == 2 && !m.empty() &&
                m CC_MAT_TYPE() == mtype && m->rows == sizes[1] && m->cols == sizes[0] )
                return;
        }

        if(fixedType())
        {
            if(CC_MAT_CN(mtype) == m CC_MAT_CN() && ((1 << CC_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m CC_MAT_TYPE();
            else
                CC_Assert(CC_MAT_TYPE(mtype) == m CC_MAT_TYPE());
        }
        if(fixedSize())
        {
            CC_Assert(m.dims == d);
            for(int j = 0; j < d; ++j)
                CC_Assert(m.size[j] == sizes[j]);
        }
        m cvSetMat(d, sizes, mtype);
        return;
    }

    if( k == MATX )
    {
        CC_Assert( i < 0 );
        int type0 = CC_MAT_TYPE(flags);
        CC_Assert( mtype == type0 || (CC_MAT_CN(mtype) == 1 && ((1 << type0) & fixedDepthMask) != 0) );
        CC_Assert( d == 2 && ((sizes[0] == sz.height && sizes[1] == sz.width) ||
                                 (allowTransposed && sizes[0] == sz.width && sizes[1] == sz.height)));
        return;
    }

    if( k == STD_VECTOR || k == STD_VECTOR_VECTOR )
    {
        CC_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
        size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0;
        std::vector<uchar>* v = (std::vector<uchar>*)obj;

        if( k == STD_VECTOR_VECTOR )
        {
            std::vector<std::vector<uchar> >& vv = *(std::vector<std::vector<uchar> >*)obj;
            if( i < 0 )
            {
                CC_Assert(!fixedSize() || len == vv.size());
                vv.resize(len);
                return;
            }
            CC_Assert( i < (int)vv.size() );
            v = &vv[i];
        }
        else
            CC_Assert( i < 0 );

        int type0 = CC_MAT_TYPE(flags);
        CC_Assert( mtype == type0 || (CC_MAT_CN(mtype) == CC_MAT_CN(type0) && ((1 << type0) & fixedDepthMask) != 0) );

        int esz = CC_TYPE_SIZE(type0);
        CC_Assert(!fixedSize() || len == ((std::vector<uchar>*)v)->size() / esz);
        switch( esz )
        {
        case 1:
            ((std::vector<uchar>*)v)->resize(len);
            break;
        case 2:
            ((std::vector<Vec2b>*)v)->resize(len);
            break;
        case 3:
            ((std::vector<Vec3b>*)v)->resize(len);
            break;
        case 4:
            ((std::vector<int>*)v)->resize(len);
            break;
        case 6:
            ((std::vector<Vec3s>*)v)->resize(len);
            break;
        case 8:
            ((std::vector<Vec2i>*)v)->resize(len);
            break;
        case 12:
            ((std::vector<Vec3i>*)v)->resize(len);
            break;
        case 16:
            ((std::vector<Vec4i>*)v)->resize(len);
            break;
        case 24:
            ((std::vector<Vec6i>*)v)->resize(len);
            break;
        case 32:
            ((std::vector<Vec8i>*)v)->resize(len);
            break;
        case 36:
            ((std::vector<Vec<int, 9> >*)v)->resize(len);
            break;
        case 48:
            ((std::vector<Vec<int, 12> >*)v)->resize(len);
            break;
        case 64:
            ((std::vector<Vec<int, 16> >*)v)->resize(len);
            break;
        case 128:
            ((std::vector<Vec<int, 32> >*)v)->resize(len);
            break;
        case 256:
            ((std::vector<Vec<int, 64> >*)v)->resize(len);
            break;
        case 512:
            ((std::vector<Vec<int, 128> >*)v)->resize(len);
            break;
        default:
            CC_Error_(CC_StsBadArg, ("Vectors with element size %d are not supported. Please, modify img_t*::create()\n", esz));
        }
        return;
    }

    if( k == NONE )
    {
        CC_Error(CC_StsNullPtr, "create() called for the missing output array" );
        return;
    }

    if( k == STD_VECTOR_MAT )
    {
        std::vector<img_t>& v = *(std::vector<img_t>*)obj;

        if( i < 0 )
        {
            CC_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CC_Assert(!fixedSize() || len == len0);
            v.resize(len);
            if( fixedType() )
            {
                int _type = CC_MAT_DEPTH(flags);
                for( size_t j = len0; j < len; j++ )
                {
                    if( v[j] CC_MAT_TYPE() == _type )
                        continue;
                    CC_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CC_MAT_TYPE_MASK) | _type;
                }
            }
            return;
        }

        CC_Assert( i < (int)v.size() );
        img_t& m = v[i];

        if( allowTransposed )
        {
            if( !m CC_IS_CONT_MAT() )
            {
                CC_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            if( d == 2 && m.dims == 2 && m.data &&
                m CC_MAT_TYPE() == mtype && m->rows == sizes[1] && m->cols == sizes[0] )
                return;
        }

        if(fixedType())
        {
            if(CC_MAT_CN(mtype) == m CC_MAT_CN() && ((1 << CC_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m CC_MAT_TYPE();
            else
                CC_Assert(CC_MAT_TYPE(mtype) == m CC_MAT_TYPE());
        }
        if(fixedSize())
        {
            CC_Assert(m.dims == d);
            for(int j = 0; j < d; ++j)
                CC_Assert(m.size[j] == sizes[j]);
        }

        m cvSetMat(d, sizes, mtype);
        return;
    }

    if( k == STD_VECTOR_UMAT )
    {
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;

        if( i < 0 )
        {
            CC_Assert( d == 2 && (sizes[0] == 1 || sizes[1] == 1 || sizes[0]*sizes[1] == 0) );
            size_t len = sizes[0]*sizes[1] > 0 ? sizes[0] + sizes[1] - 1 : 0, len0 = v.size();

            CC_Assert(!fixedSize() || len == len0);
            v.resize(len);
            if( fixedType() )
            {
                int _type = CC_MAT_DEPTH(flags);
                for( size_t j = len0; j < len; j++ )
                {
                    if( v[j] CC_MAT_TYPE() == _type )
                        continue;
                    CC_Assert( v[j].empty() );
                    v[j].flags = (v[j].flags & ~CC_MAT_TYPE_MASK) | _type;
                }
            }
            return;
        }

        CC_Assert( i < (int)v.size() );
        UMat& m = v[i];

        if( allowTransposed )
        {
            if( !m CC_IS_CONT_MAT() )
            {
                CC_Assert(!fixedType() && !fixedSize());
                m.release();
            }

            if( d == 2 && m.dims == 2 && m.u &&
                m CC_MAT_TYPE() == mtype && m->rows == sizes[1] && m->cols == sizes[0] )
                return;
        }

        if(fixedType())
        {
            if(CC_MAT_CN(mtype) == m CC_MAT_CN() && ((1 << CC_MAT_TYPE(flags)) & fixedDepthMask) != 0 )
                mtype = m CC_MAT_TYPE();
            else
                CC_Assert(CC_MAT_TYPE(mtype) == m CC_MAT_TYPE());
        }
        if(fixedSize())
        {
            CC_Assert(m.dims == d);
            for(int j = 0; j < d; ++j)
                CC_Assert(m.size[j] == sizes[j]);
        }

        m cvSetMat(d, sizes, mtype);
        return;
    }

    CC_Error(CC_StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::createSameSize(const _InputArray& arr, int mtype) const
{
    int arrsz[CC_MAX_DIM], d = arr.sizend(arrsz);
    create(d, arrsz, mtype);
}

void _OutputArray::release() const
{
    CC_Assert(!fixedSize());

    int k = kind();

    if( k == MAT )
    {
        ((img_t*)obj)->release();
        return;
    }

    if( k == UMAT )
    {
        ((UMat*)obj)->release();
        return;
    }

    if( k == CUDA_GPU_MAT )
    {
        ((cuda::GpuMat*)obj)->release();
        return;
    }

    if( k == CUDA_HOST_MEM )
    {
        ((cuda::HostMem*)obj)->release();
        return;
    }

    if( k == OPENGL_BUFFER )
    {
        ((ogl::Buffer*)obj)->release();
        return;
    }

    if( k == NONE )
        return;

    if( k == STD_VECTOR )
    {
        create(CSize(), CC_MAT_TYPE(flags));
        return;
    }

    if( k == STD_VECTOR_VECTOR )
    {
        ((std::vector<std::vector<uchar> >*)obj)->clear();
        return;
    }

    if( k == STD_VECTOR_MAT )
    {
        ((std::vector<img_t>*)obj)->clear();
        return;
    }

    if( k == STD_VECTOR_UMAT )
    {
        ((std::vector<UMat>*)obj)->clear();
        return;
    }
    if (k == STD_VECTOR_CUDA_GPU_MAT)
    {
        ((std::vector<cuda::GpuMat>*)obj)->clear();
        return;
    }
    CC_Error(CC_StsNotImplemented, "Unknown/unsupported array type");
}

void _OutputArray::clear() const
{
    int k = kind();

    if( k == MAT )
    {
        CC_Assert(!fixedSize());
        ((img_t*)obj)->resize(0);
        return;
    }

    release();
}

bool _OutputArray::needed() const
{
    return kind() != NONE;
}

img_t& _OutputArray::getMatRef(int i) const
{
    int k = kind();
    if( i < 0 )
    {
        CC_Assert( k == MAT );
        return *(img_t*)obj;
    }
    else
    {
        CC_Assert( k == STD_VECTOR_MAT );
        std::vector<img_t>& v = *(std::vector<img_t>*)obj;
        CC_Assert( i < (int)v.size() );
        return v[i];
    }
}

UMat& _OutputArray::getUMatRef(int i) const
{
    int k = kind();
    if( i < 0 )
    {
        CC_Assert( k == UMAT );
        return *(UMat*)obj;
    }
    else
    {
        CC_Assert( k == STD_VECTOR_UMAT );
        std::vector<UMat>& v = *(std::vector<UMat>*)obj;
        CC_Assert( i < (int)v.size() );
        return v[i];
    }
}

cuda::GpuMat& _OutputArray::getGpuMatRef() const
{
    int k = kind();
    CC_Assert( k == CUDA_GPU_MAT );
    return *(cuda::GpuMat*)obj;
}
std::vector<cuda::GpuMat>& _OutputArray::getGpuMatVecRef() const
{
    int k = kind();
    CC_Assert(k == STD_VECTOR_CUDA_GPU_MAT);
    return *(std::vector<cuda::GpuMat>*)obj;
}

ogl::Buffer& _OutputArray::getOGlBufferRef() const
{
    int k = kind();
    CC_Assert( k == OPENGL_BUFFER );
    return *(ogl::Buffer*)obj;
}

cuda::HostMem& _OutputArray::getHostMemRef() const
{
    int k = kind();
    CC_Assert( k == CUDA_HOST_MEM );
    return *(cuda::HostMem*)obj;
}

void _OutputArray::setTo(const _InputArray& arr, const _InputArray & mask) const
{
    int k = kind();

    if( k == NONE )
        ;
    else if( k == MAT || k == MATX || k == STD_VECTOR )
    {
        img_t m = getMat();
        m.setTo(arr, mask);
    }
    else if( k == UMAT )
        ((UMat*)obj)->setTo(arr, mask);
    else if( k == CUDA_GPU_MAT )
    {
        img_t value = arr;
        CC_Assert( checkScalar(value, type(), arr.kind(), _InputArray::CUDA_GPU_MAT) );
        ((cuda::GpuMat*)obj)->setTo(Scalar(Vec<double, 4>(value img_ptr(double, ))), mask);
    }
    else
        CC_Error(CC_StsNotImplemented, "");
}


void _OutputArray::assign(const UMat& u) const
{
    int k = kind();
    if (k == UMAT)
    {
        *(UMat*)obj = u;
    }
    else if (k == MAT)
    {
        u.copyTo(*(img_t*)obj); // TODO check u
    }
    else if (k == MATX)
    {
        u.copyTo(getMat()); // TODO check u
    }
    else
    {
        CC_Error(CC_StsNotImplemented, "");
    }
}


void _OutputArray::assign(const img_t& m) const
{
    int k = kind();
    if (k == UMAT)
    {
        m.copyTo(*(UMat*)obj); // TODO check m.getUMat()
    }
    else if (k == MAT)
    {
        *(img_t*)obj = m;
    }
    else if (k == MATX)
    {
        m.copyTo(getMat());
    }
    else
    {
        CC_Error(CC_StsNotImplemented, "");
    }
}


static _InputOutputArray _none;
InputOutputArray NULL { return _none; }

#endif


/*************************************************************************************************\
                                        Matrix Operations
\*************************************************************************************************/

static void hconcatn(const img_t** src, int nsrc, img_t* dst)
{
    int i, totalCols = 0, cols = 0;
    CC_INSTRUMENT_REGION()

    if( nsrc == 0 || !src )
    {
        imfree(dst);
        return;
    }

    for( i = 0; i < nsrc; i++ )
    {
        CC_Assert( src[i]->rows == src[0]->rows && CC_ARE_TYPES_EQ(src[i], src[0]));
        totalCols += src[i]->cols;
    }
    cvSetMat( dst, src[0]->rows, totalCols, CC_MAT_DEPTH(src[0]), CC_MAT_CN(src[0]), NULL, CC_AUTO_STEP);
    for( i = 0; i < nsrc; i++ )
    {
        img_t dpart[1] = {0};
        cvGetSubRect1(dst, dpart, cols, 0, src[i]->cols, src[i]->rows);
        cvCopyTo(src[i], dpart);
        cols += src[i]->cols;
    }
}

static void hconcat(const img_t* src1, const img_t* src2, img_t* dst)
{
    CC_INSTRUMENT_REGION()

    const img_t *src[] = {src1, src2};
    hconcatn(src, 2, dst);
}

static void hconcat(const img_t* src, int nsrc, img_t* dst)
{
    CC_INSTRUMENT_REGION()

    if (nsrc > 0)
    {
        const img_t** psrc = MALLOC(const img_t*, nsrc);
        int i;
        for (i=0; i<nsrc; ++i) {
          psrc[i] = src + i;
        }
        hconcatn(psrc, nsrc, dst);
        FREE(psrc);
    }
}

static void vconcatn(const img_t** src, int nsrc, img_t* dst0)
{
    int i, totalRows = 0, rows = 0;
	img_t dst[1] = { 0 };

    if( nsrc == 0 || !src )
    {
        imfree(dst);
        return;
    }

    for( i = 0; i < nsrc; i++ )
    {
        CC_Assert(src[i]->cols == src[0]->cols &&
                  CC_ARE_TYPES_EQ(src[i], src[0]));
        totalRows += src[i]->rows;
    }
    cvSetMat( dst, totalRows, src[0]->cols, CC_MAT_DEPTH(src[0]), CC_MAT_CN(src[0]), NULL, CC_AUTO_STEP);
    for( i = 0; i < nsrc; i++ )
    {
        img_t dpart[1] = {0};
        cvGetSubRect1(dst, dpart, 0, rows, src[i]->cols, src[i]->rows);
        cvCopyTo(src[i], dpart);
        rows += src[i]->rows;
    }
	imswap(dst, dst0);
	imfree(dst);
}

static void vconcat(const img_t* src1, const img_t* src2, img_t* dst)
{
    CC_INSTRUMENT_REGION()

    const img_t *src[] = {src1, src2};
    vconcatn(src, 2, dst);
}

static void vconcat(const img_t* src, int nsrc, img_t* dst)
{
    CC_INSTRUMENT_REGION()

    if (nsrc > 0)
    {
        const img_t** psrc = MALLOC(const img_t*, nsrc);
        int i;
        for (i=0; i<nsrc; ++i) {
          psrc[i] = src + i;
        }
        vconcatn(psrc, nsrc, dst);
        FREE(psrc);
    }
}

//////////////////////////////////////// set identity ////////////////////////////////////////////

#ifdef HAVE_OPENCL

static bool ocl_setIdentity( InputOutputArray _m, const Scalar& s )
{
    int type = _m CC_MAT_TYPE(), depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type), kercn = cn, rowsPerWI = 1;
    int sctype = CC_MAKE_TYPE(depth, cn == 3 ? 4 : cn);
    if (ocl::Device::getDefault().isIntel())
    {
        rowsPerWI = 4;
        if (cn == 1)
        {
            kercn = MIN(ocl::predictOptimalVectorWidth(_m), 4);
            if (kercn != 4)
                kercn = 1;
        }
    }

    ocl::Kernel k("setIdentity", ocl::core::set_identity_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D ST=%s -D kercn=%d -D rowsPerWI=%d",
                         ocl::memopTypeToStr(CC_MAKE_TYPE(depth, kercn)),
                         ocl::memopTypeToStr(depth), cn,
                         ocl::memopTypeToStr(sctype),
                         kercn, rowsPerWI));
    if (k.empty())
        return false;

    UMat m = _m.getUMat();
    k.args(ocl::KernelArg::WriteOnly(m, cn, kercn),
           ocl::KernelArg::Constant(img_t(1, 1, sctype, s)));

    size_t globalsize[2] = { (size_t)m->cols * cn / kercn, ((size_t)m->rows + rowsPerWI - 1) / rowsPerWI };
    return k.run(2, globalsize, NULL, false);
}

#endif

#if 0
void setIdentity( InputOutputArray m, const CScalar s )
{
    CC_INSTRUMENT_REGION()

    CC_OCL_RUN(_m.isUMat(), ocl_setIdentity(_m, s))

    int rows = m->rows, cols = m->cols, type = CC_MAT_DEPTH(m);

    if( type == CC_32FC1 )
    {
        float* data = m img_ptr(float, );
        float val = (float)s[0];
        size_t step = m->step/sizeof(data[0]);

        for( int i = 0; i < rows; i++, data += step )
        {
            for( int j = 0; j < cols; j++ )
                data[j] = 0;
            if( i < cols )
                data[i] = val;
        }
    }
    else if( type == CC_64FC1 )
    {
        double* data = m img_ptr(double, );
        double val = s[0];
        size_t step = m->step/sizeof(data[0]);

        for( int i = 0; i < rows; i++, data += step )
        {
            for( int j = 0; j < cols; j++ )
                data[j] = j == i ? val : 0;
        }
    }
    else
    {
        m = Scalar(0);
        m.diag() = s;
    }
}

//////////////////////////////////////////// trace ///////////////////////////////////////////

Scalar trace( const img_t* _m )
{
    CC_INSTRUMENT_REGION()

    img_t m = _m;
    CC_Assert( m.dims <= 2 );
    int type = m CC_MAT_TYPE();
    int nm = MIN(m->rows, m->cols);

    if( type == CC_32FC1 )
    {
        const float* ptr = m img_ptr(float, );
        size_t step = m->step/sizeof(ptr[0]) + 1;
        double _s = 0;
        for( int i = 0; i < nm; i++ )
            _s += ptr[i*step];
        return _s;
    }

    if( type == CC_64FC1 )
    {
        const double* ptr = m img_ptr(double, );
        size_t step = m->step/sizeof(ptr[0]) + 1;
        double _s = 0;
        for( int i = 0; i < nm; i++ )
            _s += ptr[i*step];
        return _s;
    }

    return sum(m.diag());
}

#endif

////////////////////////////////////// transpose /////////////////////////////////////////

#define CCFUNC(NAME)  NAME ## _8u
#define T uchar
#include "matrix_copy.inl"

#define CCFUNC(NAME)  NAME ## _16u
#define T ushort
#include "matrix_copy.inl"

#define CCFUNC(NAME)  NAME ## _8uC3
#define T vec3b
#include "matrix_copy.inl"

#define CCFUNC(NAME)  NAME ## _32s
#define T vec3b
#include "matrix_copy.inl"

#define CCFUNC(NAME)  NAME ## _16uC3
#define T vec3s
#include "matrix_copy.inl"

#define CCFUNC(NAME)  NAME ## _32sC2
#define T vec3b
#include "matrix_copy.inl"

#define CCFUNC(NAME)  NAME ## _32sC3
#define T vec3i
#include "matrix_copy.inl"

#define CCFUNC(NAME)  NAME ## _32sC4
#define T vec4i
#include "matrix_copy.inl"

#define CCFUNC(NAME)  NAME ## _32sC6
#define T vec6i
#include "matrix_copy.inl"

#define CCFUNC(NAME)  NAME ## _32sC8
#define T vec8i
#include "matrix_copy.inl"

#if 0
DEF_TRANSPOSE_FUNC(8u, uchar)
DEF_TRANSPOSE_FUNC(16u, ushort)
DEF_TRANSPOSE_FUNC(8uC3, vec3b)
DEF_TRANSPOSE_FUNC(32s, int)
DEF_TRANSPOSE_FUNC(16uC3, vec3s)
DEF_TRANSPOSE_FUNC(32sC2, vec2i)
DEF_TRANSPOSE_FUNC(32sC3, vec3i)
DEF_TRANSPOSE_FUNC(32sC4, vec4i)
DEF_TRANSPOSE_FUNC(32sC6, vec6i)
DEF_TRANSPOSE_FUNC(32sC8, vec8i)
#endif


typedef void (*TransposeFunc)( const uchar* src, size_t sstep, uchar* dst, size_t dstep, CSize sz );
typedef void (*TransposeInplaceFunc)( uchar* data, size_t step, int n );



static TransposeFunc transposeTab[] =
{
    0, transpose_8u, transpose_16u, transpose_8uC3, transpose_32s, 0, transpose_16uC3, 0,
    transpose_32sC2, 0, 0, 0, transpose_32sC3, 0, 0, 0, transpose_32sC4,
    0, 0, 0, 0, 0, 0, 0, transpose_32sC6, 0, 0, 0, 0, 0, 0, 0, transpose_32sC8
};

static TransposeInplaceFunc transposeInplaceTab[] =
{
    0, transposeI_8u, transposeI_16u, transposeI_8uC3, transposeI_32s, 0, transposeI_16uC3, 0,
    transposeI_32sC2, 0, 0, 0, transposeI_32sC3, 0, 0, 0, transposeI_32sC4,
    0, 0, 0, 0, 0, 0, 0, transposeI_32sC6, 0, 0, 0, 0, 0, 0, 0, transposeI_32sC8
};

#ifdef HAVE_OPENCL

static inline int divUp(int a, int b)
{
    return (a + b - 1) / b;
}

static bool ocl_transpose( const img_t* src, img_t* dst )
{
    const ocl::Device & dev = ocl::Device::getDefault();
    const int TILE_DIM = 32, BLOCK_ROWS = 8;
    int type = src CC_MAT_TYPE(), cn = CC_MAT_CN(type), depth = CC_MAT_DEPTH(type),
        rowsPerWI = dev.isIntel() ? 4 : 1;

    UMat src = src.getUMat();
    dst cvSetMat(src->cols, src->rows, type);
    UMat dst = dst.getUMat();

    String kernelName("transpose");
    bool inplace = dst.u == src.u;

    if (inplace)
    {
        CC_Assert(dst->cols == dst->rows);
        kernelName += "_inplace";
    }
    else
    {
        // check required local memory size
        size_t required_local_memory = (size_t) TILE_DIM*(TILE_DIM+1)*CC_TYPE_SIZE(type);
        if (required_local_memory > ocl::Device::getDefault().localMemSize())
            return false;
    }

    ocl::Kernel k(kernelName.c_str(), ocl::core::transpose_oclsrc,
                  format("-D T=%s -D T1=%s -D cn=%d -D TILE_DIM=%d -D BLOCK_ROWS=%d -D rowsPerWI=%d%s",
                         ocl::memopTypeToStr(type), ocl::memopTypeToStr(depth),
                         cn, TILE_DIM, BLOCK_ROWS, rowsPerWI, inplace ? " -D INPLACE" : ""));
    if (k.empty())
        return false;

    if (inplace)
        k.args(ocl::KernelArg::ReadWriteNoSize(dst), dst->rows);
    else
        k.args(ocl::KernelArg::ReadOnly(src),
               ocl::KernelArg::WriteOnlyNoSize(dst));

    size_t localsize[2]  = { TILE_DIM, BLOCK_ROWS };
    size_t globalsize[2] = { (size_t)src->cols, inplace ? ((size_t)src->rows + rowsPerWI - 1) / rowsPerWI : (divUp((size_t)src->rows, TILE_DIM) * BLOCK_ROWS) };

    if (inplace && dev.isIntel())
    {
        localsize[0] = 16;
        localsize[1] = dev.maxWorkGroupSize() / localsize[0];
    }

    return k.run(2, globalsize, localsize, false);
}

#endif

#ifdef HAVE_IPP
static bool ipp_transpose( img_t &src, img_t &dst )
{
    CC_INSTRUMENT_REGION_IPP()

    int type = src CC_MAT_TYPE();
    typedef IppStatus (C_STDCALL * IppiTranspose)(const void * pSrc, int srcStep, void * pDst, int dstStep, IppiSize roiSize);
    typedef IppStatus (C_STDCALL * IppiTransposeI)(const void * pSrcDst, int srcDstStep, IppiSize roiSize);
    IppiTranspose ippiTranspose = 0;
    IppiTransposeI ippiTranspose_I = 0;

    if (dst.data == src.data && dst->cols == dst->rows)
    {
        CC_SUPPRESS_DEPRECATED_START
        ippiTranspose_I =
            type == CC_8UC1 ? (IppiTransposeI)ippiTranspose_8u_C1IR :
            type == CC_8UC3 ? (IppiTransposeI)ippiTranspose_8u_C3IR :
            type == CC_8UC4 ? (IppiTransposeI)ippiTranspose_8u_C4IR :
            type == CC_16UC1 ? (IppiTransposeI)ippiTranspose_16u_C1IR :
            type == CC_16UC3 ? (IppiTransposeI)ippiTranspose_16u_C3IR :
            type == CC_16UC4 ? (IppiTransposeI)ippiTranspose_16u_C4IR :
            type == CC_16SC1 ? (IppiTransposeI)ippiTranspose_16s_C1IR :
            type == CC_16SC3 ? (IppiTransposeI)ippiTranspose_16s_C3IR :
            type == CC_16SC4 ? (IppiTransposeI)ippiTranspose_16s_C4IR :
            type == CC_32SC1 ? (IppiTransposeI)ippiTranspose_32s_C1IR :
            type == CC_32SC3 ? (IppiTransposeI)ippiTranspose_32s_C3IR :
            type == CC_32SC4 ? (IppiTransposeI)ippiTranspose_32s_C4IR :
            type == CC_32FC1 ? (IppiTransposeI)ippiTranspose_32f_C1IR :
            type == CC_32FC3 ? (IppiTransposeI)ippiTranspose_32f_C3IR :
            type == CC_32FC4 ? (IppiTransposeI)ippiTranspose_32f_C4IR : 0;
        CC_SUPPRESS_DEPRECATED_END
    }
    else
    {
        ippiTranspose =
            type == CC_8UC1 ? (IppiTranspose)ippiTranspose_8u_C1R :
            type == CC_8UC3 ? (IppiTranspose)ippiTranspose_8u_C3R :
            type == CC_8UC4 ? (IppiTranspose)ippiTranspose_8u_C4R :
            type == CC_16UC1 ? (IppiTranspose)ippiTranspose_16u_C1R :
            type == CC_16UC3 ? (IppiTranspose)ippiTranspose_16u_C3R :
            type == CC_16UC4 ? (IppiTranspose)ippiTranspose_16u_C4R :
            type == CC_16SC1 ? (IppiTranspose)ippiTranspose_16s_C1R :
            type == CC_16SC3 ? (IppiTranspose)ippiTranspose_16s_C3R :
            type == CC_16SC4 ? (IppiTranspose)ippiTranspose_16s_C4R :
            type == CC_32SC1 ? (IppiTranspose)ippiTranspose_32s_C1R :
            type == CC_32SC3 ? (IppiTranspose)ippiTranspose_32s_C3R :
            type == CC_32SC4 ? (IppiTranspose)ippiTranspose_32s_C4R :
            type == CC_32FC1 ? (IppiTranspose)ippiTranspose_32f_C1R :
            type == CC_32FC3 ? (IppiTranspose)ippiTranspose_32f_C3R :
            type == CC_32FC4 ? (IppiTranspose)ippiTranspose_32f_C4R : 0;
    }

    IppiSize roiSize = { src->cols, src->rows };
    if (ippiTranspose != 0)
    {
        if (CC_INSTRUMENT_FUN_IPP(ippiTranspose, src->tt.data, (int)src->step, dst->tt.data, (int)dst->step, roiSize) >= 0)
            return true;
    }
    else if (ippiTranspose_I != 0)
    {
        if (CC_INSTRUMENT_FUN_IPP(ippiTranspose_I, dst->tt.data, (int)dst->step, roiSize) >= 0)
            return true;
    }
    return false;
}
#endif

static img_t* transpose( const img_t* src, img_t* dst )
{
    CC_INSTRUMENT_REGION()

    TypeId type = CC_MAT_DEPTH(src);
    int esz = CC_TYPE_SIZE(type);

    CC_OCL_RUN(dst.isUMat(), ocl_transpose(src, dst))
    
    if( imempty(src) )
    {
        imfree(dst);
        return dst;
    }

    cvSetMat(dst, src->cols, src->rows, CC_MAT_DEPTH(src), CC_MAT_CN(src), NULL, CC_AUTO_STEP);

    // handle the case of single-column/single-row matrices, stored in STL vectors.
    if( src->rows != dst->cols || src->cols != dst->rows )
    {
        CC_Assert( CC_ARE_SIZES_EQ(src, dst ) && (src->cols == 1 || src->rows == 1) );
        cvCopyTo(src, dst);
        return dst;
    }

    CC_IPP_RUN_FAST(ipp_transpose(src, dst))

    if( dst == src )
    {
        if ( dst->cols == dst->rows ) {
            TransposeInplaceFunc func = transposeInplaceTab[esz];
            CC_Assert( func != 0 );
            func( dst->tt.data, dst->step, dst->rows );
        } else {
            IM_BEGIN(im, 2);
            TransposeFunc func;
            src = cvCloneMat(im, src);
            func = transposeTab[esz];
            CC_Assert( func != 0 );
            func( src->tt.data, src->step, dst->tt.data, dst->step, cvGetSize(src) );
            IM_END(im);
        }
    }
    else
    {
        TransposeFunc func = transposeTab[esz];
        CC_Assert( func != 0 );
        func( src->tt.data, src->step, dst->tt.data, dst->step, cvGetSize(src) );
    }
    return dst;
}


////////////////////////////////////// completeSymm /////////////////////////////////////////

static void completeSymm( img_t* m, bool LtoR CC_DEFAULT(false))
{
    CC_INSTRUMENT_REGION()

    size_t step = m->step, esz = CC_TYPE_SIZE(m->tid);
    int rows = m->rows;
    int i, j, j0 = 0, j1 = rows;
    uchar* data = m->tt.data;

    CC_Assert( m->rows == m->cols );
    for( i = 0; i < rows; i++ )
    {
        if( !LtoR ) j1 = i; else j0 = i+1;
        for( j = j0; j < j1; j++ )
            memcpy(data + (i*step + j*esz), data + (j*step + i*esz), esz);
    }
}

#if 0

void cross(const img_t* src1, const img_t* m, img_t* result)
{
    TypeId tp = CC_MAT_TYPE(src1);
    CC_Assert( CC_ARE_SIZES_EQ(src1, m) && tp == CC_MAT_TYPE(m) &&
        ((rows == 3 && cols == 1) || (src1->cols*CC_MAT_CN(src1) == 3 && src1->rows == 1)));
    cvSetMat(result, src1->rows, src1->cols, tp);

    if( d == CC_32F )
    {
        const float *a = (const float*)data, *b = (const float*)m.data;
        float* c = (float*)result.data;
        size_t lda = rows > 1 ? step/sizeof(a[0]) : 1;
        size_t ldb = rows > 1 ? m->step/sizeof(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }
    else if( d == CC_64F )
    {
        const double *a = (const double*)data, *b = (const double*)m.data;
        double* c = (double*)result.data;
        size_t lda = rows > 1 ? step/sizeof(a[0]) : 1;
        size_t ldb = rows > 1 ? m->step/sizeof(b[0]) : 1;

        c[0] = a[lda] * b[ldb*2] - a[lda*2] * b[ldb];
        c[1] = a[lda*2] * b[0] - a[0] * b[ldb*2];
        c[2] = a[0] * b[ldb] - a[lda] * b[0];
    }

    return result;
}

////////////////////////////////////////// reduce ////////////////////////////////////////////

template<typename T, typename ST, class Op> static void
reduceR_( const img_t& srcmat, img_t& dstmat )
{
    typedef typename Op::rtype WT;
    CSize size = srcmat.size();
    size.width *= srcmat CC_MAT_CN();
    CAutoBuffer<WT> buffer(size.width);
    WT* buf = buffer;
    ST* dst = dstmat.ptr<ST>();
    const T* src = srcmat.ptr<T>();
    size_t srcstep = srcmat->step/sizeof(src[0]);
    int i;
    Op op;

    for( i = 0; i < size.width; i++ )
        buf[i] = src[i];

    for( ; --size.height; )
    {
        src += srcstep;
        i = 0;
        #if CC_ENABLE_UNROLLED
        for(; i <= size.width - 4; i += 4 )
        {
            WT s0, s1;
            s0 = op(buf[i], (WT)src[i]);
            s1 = op(buf[i+1], (WT)src[i+1]);
            buf[i] = s0; buf[i+1] = s1;

            s0 = op(buf[i+2], (WT)src[i+2]);
            s1 = op(buf[i+3], (WT)src[i+3]);
            buf[i+2] = s0; buf[i+3] = s1;
        }
        #endif
        for( ; i < size.width; i++ )
            buf[i] = op(buf[i], (WT)src[i]);
    }

    for( i = 0; i < size.width; i++ )
        dst[i] = (ST)buf[i];
}


template<typename T, typename ST, class Op> static void
reduceC_( const img_t& srcmat, img_t& dstmat )
{
    typedef typename Op::rtype WT;
    CSize size = srcmat.size();
    int cn = srcmat CC_MAT_CN();
    size.width *= cn;
    Op op;

    for( int y = 0; y < size.height; y++ )
    {
        const T* src = srcmat.ptr<T>(y);
        ST* dst = dstmat.ptr<ST>(y);
        if( size.width == cn )
            for( int k = 0; k < cn; k++ )
                dst[k] = src[k];
        else
        {
            for( int k = 0; k < cn; k++ )
            {
                WT a0 = src[k], a1 = src[k+cn];
                int i;
                for( i = 2*cn; i <= size.width - 4*cn; i += 4*cn )
                {
                    a0 = op(a0, (WT)src[i+k]);
                    a1 = op(a1, (WT)src[i+k+cn]);
                    a0 = op(a0, (WT)src[i+k+cn*2]);
                    a1 = op(a1, (WT)src[i+k+cn*3]);
                }

                for( ; i < size.width; i += cn )
                {
                    a0 = op(a0, (WT)src[i+k]);
                }
                a0 = op(a0, a1);
                dst[k] = (ST)a0;
            }
        }
    }
}

typedef void (*ReduceFunc)( const img_t& src, img_t& dst );



#define reduceSumR8u32s  reduceR_<uchar, int,   OpAdd<int> >
#define reduceSumR8u32f  reduceR_<uchar, float, OpAdd<int> >
#define reduceSumR8u64f  reduceR_<uchar, double,OpAdd<int> >
#define reduceSumR16u32f reduceR_<ushort,float, OpAdd<float> >
#define reduceSumR16u64f reduceR_<ushort,double,OpAdd<double> >
#define reduceSumR16s32f reduceR_<short, float, OpAdd<float> >
#define reduceSumR16s64f reduceR_<short, double,OpAdd<double> >
#define reduceSumR32f32f reduceR_<float, float, OpAdd<float> >
#define reduceSumR32f64f reduceR_<float, double,OpAdd<double> >
#define reduceSumR64f64f reduceR_<double,double,OpAdd<double> >

#define reduceMaxR8u  reduceR_<uchar, uchar, OpMax<uchar> >
#define reduceMaxR16u reduceR_<ushort,ushort,OpMax<ushort> >
#define reduceMaxR16s reduceR_<short, short, OpMax<short> >
#define reduceMaxR32f reduceR_<float, float, OpMax<float> >
#define reduceMaxR64f reduceR_<double,double,OpMax<double> >

#define reduceMinR8u  reduceR_<uchar, uchar, OpMin<uchar> >
#define reduceMinR16u reduceR_<ushort,ushort,OpMin<ushort> >
#define reduceMinR16s reduceR_<short, short, OpMin<short> >
#define reduceMinR32f reduceR_<float, float, OpMin<float> >
#define reduceMinR64f reduceR_<double,double,OpMin<double> >

#ifdef HAVE_IPP
static inline bool ipp_reduceSumC_8u16u16s32f_64f(const img_t& srcmat, img_t& dstmat)
{
    int sstep = (int)srcmat->step, stype = srcmat CC_MAT_TYPE(),
            ddepth = dstmat CC_MAT_DEPTH();

    IppiSize roisize = { srcmat.size().width, 1 };

    typedef IppStatus (C_STDCALL * IppiSum)(const void * pSrc, int srcStep, IppiSize roiSize, Ipp64f* pSum);
    typedef IppStatus (C_STDCALL * IppiSumHint)(const void * pSrc, int srcStep, IppiSize roiSize, Ipp64f* pSum, IppHintAlgorithm hint);
    IppiSum ippiSum = 0;
    IppiSumHint ippiSumHint = 0;

    if(ddepth == CC_64F)
    {
        ippiSum =
            stype == CC_8UC1 ? (IppiSum)ippiSum_8u_C1R :
            stype == CC_8UC3 ? (IppiSum)ippiSum_8u_C3R :
            stype == CC_8UC4 ? (IppiSum)ippiSum_8u_C4R :
            stype == CC_16UC1 ? (IppiSum)ippiSum_16u_C1R :
            stype == CC_16UC3 ? (IppiSum)ippiSum_16u_C3R :
            stype == CC_16UC4 ? (IppiSum)ippiSum_16u_C4R :
            stype == CC_16SC1 ? (IppiSum)ippiSum_16s_C1R :
            stype == CC_16SC3 ? (IppiSum)ippiSum_16s_C3R :
            stype == CC_16SC4 ? (IppiSum)ippiSum_16s_C4R : 0;
        ippiSumHint =
            stype == CC_32FC1 ? (IppiSumHint)ippiSum_32f_C1R :
            stype == CC_32FC3 ? (IppiSumHint)ippiSum_32f_C3R :
            stype == CC_32FC4 ? (IppiSumHint)ippiSum_32f_C4R : 0;
    }

    if(ippiSum)
    {
        for(int y = 0; y < srcmat.size().height; y++)
        {
            if(CC_INSTRUMENT_FUN_IPP(ippiSum, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y)) < 0)
                return false;
        }
        return true;
    }
    else if(ippiSumHint)
    {
        for(int y = 0; y < srcmat.size().height; y++)
        {
            if(CC_INSTRUMENT_FUN_IPP(ippiSumHint, srcmat.ptr(y), sstep, roisize, dstmat.ptr<Ipp64f>(y), ippAlgHintAccurate) < 0)
                return false;
        }
        return true;
    }

    return false;
}

static inline void reduceSumC_8u16u16s32f_64f(const img_t& srcmat, img_t& dstmat)
{
    CC_IPP_RUN_FAST(ipp_reduceSumC_8u16u16s32f_64f(srcmat, dstmat));

    ReduceFunc func = 0;

    if(dstmat CC_MAT_DEPTH() == CC_64F)
    {
        int sdepth = CC_MAT_DEPTH(srcmat CC_MAT_TYPE());
        func =
            sdepth == CC_8U ? (ReduceFunc)reduceC_<uchar, double,   OpAdd<double> > :
            sdepth == CC_16U ? (ReduceFunc)reduceC_<ushort, double,   OpAdd<double> > :
            sdepth == CC_16S ? (ReduceFunc)reduceC_<short, double,   OpAdd<double> > :
            sdepth == CC_32F ? (ReduceFunc)reduceC_<float, double,   OpAdd<double> > : 0;
    }
    CC_Assert(func);

    func(srcmat, dstmat);
}

#endif

#define reduceSumC8u32s  reduceC_<uchar, int,   OpAdd<int> >
#define reduceSumC8u32f  reduceC_<uchar, float, OpAdd<int> >
#define reduceSumC16u32f reduceC_<ushort,float, OpAdd<float> >
#define reduceSumC16s32f reduceC_<short, float, OpAdd<float> >
#define reduceSumC32f32f reduceC_<float, float, OpAdd<float> >
#define reduceSumC64f64f reduceC_<double,double,OpAdd<double> >

#ifdef HAVE_IPP
#define reduceSumC8u64f  reduceSumC_8u16u16s32f_64f
#define reduceSumC16u64f reduceSumC_8u16u16s32f_64f
#define reduceSumC16s64f reduceSumC_8u16u16s32f_64f
#define reduceSumC32f64f reduceSumC_8u16u16s32f_64f
#else
#define reduceSumC8u64f  reduceC_<uchar, double,OpAdd<int> >
#define reduceSumC16u64f reduceC_<ushort,double,OpAdd<double> >
#define reduceSumC16s64f reduceC_<short, double,OpAdd<double> >
#define reduceSumC32f64f reduceC_<float, double,OpAdd<double> >
#endif

#ifdef HAVE_IPP
#define REDUCE_OP(favor, optype, type1, type2) \
static inline bool ipp_reduce##optype##C##favor(const img_t& srcmat, img_t& dstmat) \
{ \
    if((srcmat CC_MAT_CN() == 1)) \
    { \
        int sstep = (int)srcmat->step; \
        typedef Ipp##favor IppType; \
        IppiSize roisize = ippiSize(srcmat.size().width, 1);\
        for(int y = 0; y < srcmat.size().height; y++)\
        {\
            if(CC_INSTRUMENT_FUN_IPP(ippi##optype##_##favor##_C1R, srcmat.ptr<IppType>(y), sstep, roisize, dstmat.ptr<IppType>(y)) < 0)\
                return false;\
        }\
        return true;\
    }\
    return false; \
} \
static inline void reduce##optype##C##favor(const img_t& srcmat, img_t& dstmat) \
{ \
    CC_IPP_RUN_FAST(ipp_reduce##optype##C##favor(srcmat, dstmat)); \
    reduceC_ < type1, type2, Op##optype < type2 > >(srcmat, dstmat); \
}
#endif

#ifdef HAVE_IPP
REDUCE_OP(8u, Max, uchar, uchar)
REDUCE_OP(16u, Max, ushort, ushort)
REDUCE_OP(16s, Max, short, short)
REDUCE_OP(32f, Max, float, float)
#else
#define reduceMaxC8u  reduceC_<uchar, uchar, OpMax<uchar> >
#define reduceMaxC16u reduceC_<ushort,ushort,OpMax<ushort> >
#define reduceMaxC16s reduceC_<short, short, OpMax<short> >
#define reduceMaxC32f reduceC_<float, float, OpMax<float> >
#endif
#define reduceMaxC64f reduceC_<double,double,OpMax<double> >

#ifdef HAVE_IPP
REDUCE_OP(8u, Min, uchar, uchar)
REDUCE_OP(16u, Min, ushort, ushort)
REDUCE_OP(16s, Min, short, short)
REDUCE_OP(32f, Min, float, float)
#else
#define reduceMinC8u  reduceC_<uchar, uchar, OpMin<uchar> >
#define reduceMinC16u reduceC_<ushort,ushort,OpMin<ushort> >
#define reduceMinC16s reduceC_<short, short, OpMin<short> >
#define reduceMinC32f reduceC_<float, float, OpMin<float> >
#endif
#define reduceMinC64f reduceC_<double,double,OpMin<double> >

#ifdef HAVE_OPENCL

static bool ocl_reduce(const img_t* src, img_t* dst,
                       int dim, int op, int op0, int stype, int dtype)
{
    const int min_opt_cols = 128, buf_cols = 32;
    int sdepth = CC_MAT_DEPTH(stype), cn = CC_MAT_CN(stype),
            ddepth = CC_MAT_DEPTH(dtype), ddepth0 = ddepth;
    const ocl::Device &defDev = ocl::Device::getDefault();
    bool doubleSupport = defDev.doubleFPConfig() > 0;

    size_t wgs = defDev.maxWorkGroupSize();
    bool cUseOptimized = 1 == dim && src->cols() > min_opt_cols && (wgs >= buf_cols);

    if (!doubleSupport && (sdepth == CC_64F || ddepth == CC_64F))
        return false;

    if (op == CC_REDUCE_AVG)
    {
        if (sdepth < CC_32S && ddepth < CC_32S)
            ddepth = CC_32S;
    }

    const char * const ops[4] = { "OCL_CC_REDUCE_SUM", "OCL_CC_REDUCE_AVG",
                                  "OCL_CC_REDUCE_MAX", "OCL_CC_REDUCE_MIN" };
    int wdepth = MAX(ddepth, CC_32F);
    if (cUseOptimized)
    {
        size_t tileHeight = (size_t)(wgs / buf_cols);
        if (defDev.isIntel())
        {
            static const size_t maxItemInGroupCount = 16;
            tileHeight = min(tileHeight, defDev.localMemSize() / buf_cols / CC_TYPE_SIZE(CC_MAKETYPE(wdepth, cn)) / maxItemInGroupCount);
        }
        char cvt[3][40];
        String build_opt = format("-D OP_REDUCE_PRE -D BUF_COLS=%d -D TILE_HEIGHT=%d -D %s -D dim=1"
                                            " -D cn=%d -D ddepth=%d"
                                            " -D srcT=%s -D bufT=%s -D dstT=%s"
                                            " -D convertToWT=%s -D convertToBufT=%s -D convertToDT=%s%s",
                                            buf_cols, tileHeight, ops[op], cn, ddepth,
                                            ocl::typeToStr(sdepth),
                                            ocl::typeToStr(ddepth),
                                            ocl::typeToStr(ddepth0),
                                            ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0]),
                                            ocl::convertTypeStr(sdepth, ddepth, 1, cvt[1]),
                                            ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[2]),
                                            doubleSupport ? " -D DOUBLE_SUPPORT" : "");
        ocl::Kernel k("reduce_horz_opt", ocl::core::reduce2_oclsrc, build_opt);
        if (k.empty())
            return false;
        UMat src = src.getUMat();
        CSize dsize(1, src->rows);
        dst cvSetMat(dsize, dtype);
        UMat dst = dst.getUMat();

        if (op0 == CC_REDUCE_AVG)
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst), 1.0f / src->cols);
        else
            k.args(ocl::KernelArg::ReadOnly(src),
                      ocl::KernelArg::WriteOnlyNoSize(dst));

        size_t localSize[2] = { (size_t)buf_cols, (size_t)tileHeight};
        size_t globalSize[2] = { (size_t)buf_cols, (size_t)src->rows };
        return k.run(2, globalSize, localSize, false);
    }
    else
    {
        char cvt[2][40];
        String build_opt = format("-D %s -D dim=%d -D cn=%d -D ddepth=%d"
                                      " -D srcT=%s -D dstT=%s -D dstT0=%s -D convertToWT=%s"
                                      " -D convertToDT=%s -D convertToDT0=%s%s",
                                      ops[op], dim, cn, ddepth, ocl::typeToStr(cUseOptimized ? ddepth : sdepth),
                                      ocl::typeToStr(ddepth), ocl::typeToStr(ddepth0),
                                      ocl::convertTypeStr(ddepth, wdepth, 1, cvt[0]),
                                      ocl::convertTypeStr(sdepth, ddepth, 1, cvt[0]),
                                      ocl::convertTypeStr(wdepth, ddepth0, 1, cvt[1]),
                                      doubleSupport ? " -D DOUBLE_SUPPORT" : "");

        ocl::Kernel k("reduce", ocl::core::reduce2_oclsrc, build_opt);
        if (k.empty())
            return false;

        UMat src = src.getUMat();
        CSize dsize(dim == 0 ? src->cols : 1, dim == 0 ? 1 : src->rows);
        dst cvSetMat(dsize, dtype);
        UMat dst = dst.getUMat();

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src),
                temparg = ocl::KernelArg::WriteOnlyNoSize(dst);

        if (op0 == CC_REDUCE_AVG)
            k.args(srcarg, temparg, 1.0f / (dim == 0 ? src->rows : src->cols));
        else
            k.args(srcarg, temparg);

        size_t globalsize = MAX(dsize.width, dsize.height);
        return k.run(1, &globalsize, NULL, false);
    }
}



#endif

void reduce(const img_t* src, img_t* dst, int dim, int op, int dtype)
{
    CC_INSTRUMENT_REGION()

    CC_Assert( src.dims() <= 2 );
    int op0 = op;
    int stype = src CC_MAT_TYPE(), sdepth = CC_MAT_DEPTH(stype), cn = CC_MAT_CN(stype);
    if( dtype < 0 )
        dtype = dst.fixedType() ? dst CC_MAT_TYPE() : stype;
    dtype = CC_MAKETYPE(dtype >= 0 ? dtype : stype, cn);
    int ddepth = CC_MAT_DEPTH(dtype);

    CC_Assert( cn == CC_MAT_CN(dtype) );
    CC_Assert( op == CC_REDUCE_SUM || op == CC_REDUCE_MAX ||
               op == CC_REDUCE_MIN || op == CC_REDUCE_AVG );

    CC_OCL_RUN(dst.isUMat(),
               ocl_reduce(src, dst, dim, op, op0, stype, dtype))

    
    dst cvSetMat(dim == 0 ? 1 : src->rows, dim == 0 ? src->cols : 1, dtype);
    img_t dst = dst, temp = dst;

    if( op == CC_REDUCE_AVG )
    {
        op = CC_REDUCE_SUM;
        if( sdepth < CC_32S && ddepth < CC_32S )
        {
            temp cvSetMat(dst->rows, dst->cols, CC_32SC(cn));
            ddepth = CC_32S;
        }
    }

    ReduceFunc func = 0;
    if( dim == 0 )
    {
        if( op == CC_REDUCE_SUM )
        {
            if(sdepth == CC_8U && ddepth == CC_32S)
                func = GET_OPTIMIZED(reduceSumR8u32s);
            else if(sdepth == CC_8U && ddepth == CC_32F)
                func = reduceSumR8u32f;
            else if(sdepth == CC_8U && ddepth == CC_64F)
                func = reduceSumR8u64f;
            else if(sdepth == CC_16U && ddepth == CC_32F)
                func = reduceSumR16u32f;
            else if(sdepth == CC_16U && ddepth == CC_64F)
                func = reduceSumR16u64f;
            else if(sdepth == CC_16S && ddepth == CC_32F)
                func = reduceSumR16s32f;
            else if(sdepth == CC_16S && ddepth == CC_64F)
                func = reduceSumR16s64f;
            else if(sdepth == CC_32F && ddepth == CC_32F)
                func = GET_OPTIMIZED(reduceSumR32f32f);
            else if(sdepth == CC_32F && ddepth == CC_64F)
                func = reduceSumR32f64f;
            else if(sdepth == CC_64F && ddepth == CC_64F)
                func = reduceSumR64f64f;
        }
        else if(op == CC_REDUCE_MAX)
        {
            if(sdepth == CC_8U && ddepth == CC_8U)
                func = GET_OPTIMIZED(reduceMaxR8u);
            else if(sdepth == CC_16U && ddepth == CC_16U)
                func = reduceMaxR16u;
            else if(sdepth == CC_16S && ddepth == CC_16S)
                func = reduceMaxR16s;
            else if(sdepth == CC_32F && ddepth == CC_32F)
                func = GET_OPTIMIZED(reduceMaxR32f);
            else if(sdepth == CC_64F && ddepth == CC_64F)
                func = reduceMaxR64f;
        }
        else if(op == CC_REDUCE_MIN)
        {
            if(sdepth == CC_8U && ddepth == CC_8U)
                func = GET_OPTIMIZED(reduceMinR8u);
            else if(sdepth == CC_16U && ddepth == CC_16U)
                func = reduceMinR16u;
            else if(sdepth == CC_16S && ddepth == CC_16S)
                func = reduceMinR16s;
            else if(sdepth == CC_32F && ddepth == CC_32F)
                func = GET_OPTIMIZED(reduceMinR32f);
            else if(sdepth == CC_64F && ddepth == CC_64F)
                func = reduceMinR64f;
        }
    }
    else
    {
        if(op == CC_REDUCE_SUM)
        {
            if(sdepth == CC_8U && ddepth == CC_32S)
                func = GET_OPTIMIZED(reduceSumC8u32s);
            else if(sdepth == CC_8U && ddepth == CC_32F)
                func = reduceSumC8u32f;
            else if(sdepth == CC_8U && ddepth == CC_64F)
                func = reduceSumC8u64f;
            else if(sdepth == CC_16U && ddepth == CC_32F)
                func = reduceSumC16u32f;
            else if(sdepth == CC_16U && ddepth == CC_64F)
                func = reduceSumC16u64f;
            else if(sdepth == CC_16S && ddepth == CC_32F)
                func = reduceSumC16s32f;
            else if(sdepth == CC_16S && ddepth == CC_64F)
                func = reduceSumC16s64f;
            else if(sdepth == CC_32F && ddepth == CC_32F)
                func = GET_OPTIMIZED(reduceSumC32f32f);
            else if(sdepth == CC_32F && ddepth == CC_64F)
                func = reduceSumC32f64f;
            else if(sdepth == CC_64F && ddepth == CC_64F)
                func = reduceSumC64f64f;
        }
        else if(op == CC_REDUCE_MAX)
        {
            if(sdepth == CC_8U && ddepth == CC_8U)
                func = GET_OPTIMIZED(reduceMaxC8u);
            else if(sdepth == CC_16U && ddepth == CC_16U)
                func = reduceMaxC16u;
            else if(sdepth == CC_16S && ddepth == CC_16S)
                func = reduceMaxC16s;
            else if(sdepth == CC_32F && ddepth == CC_32F)
                func = GET_OPTIMIZED(reduceMaxC32f);
            else if(sdepth == CC_64F && ddepth == CC_64F)
                func = reduceMaxC64f;
        }
        else if(op == CC_REDUCE_MIN)
        {
            if(sdepth == CC_8U && ddepth == CC_8U)
                func = GET_OPTIMIZED(reduceMinC8u);
            else if(sdepth == CC_16U && ddepth == CC_16U)
                func = reduceMinC16u;
            else if(sdepth == CC_16S && ddepth == CC_16S)
                func = reduceMinC16s;
            else if(sdepth == CC_32F && ddepth == CC_32F)
                func = GET_OPTIMIZED(reduceMinC32f);
            else if(sdepth == CC_64F && ddepth == CC_64F)
                func = reduceMinC64f;
        }
    }

    if( !func )
        CC_Error( CC_StsUnsupportedFormat,
                  "Unsupported combination of input and output array formats" );

    func( src, temp );

    if( op0 == CC_REDUCE_AVG )
        temp.convertTo(dst, dst CC_MAT_TYPE(), 1./(dim == 0 ? src->rows : src->cols));
}


//////////////////////////////////////// sort ///////////////////////////////////////////

#ifdef HAVE_IPP
#define USE_IPP_SORT

typedef IppStatus (C_STDCALL * IppSortFunc)(void *, int);
typedef IppSortFunc IppFlipFunc;

static IppSortFunc getSortFunc(int depth, bool sortDescending)
{
    if (!sortDescending)
        return depth == CC_8U ? (IppSortFunc)ippsSortAscend_8u_I :
#if IPP_DISABLE_BLOCK
            depth == CC_16U ? (IppSortFunc)ippsSortAscend_16u_I :
            depth == CC_16S ? (IppSortFunc)ippsSortAscend_16s_I :
            depth == CC_32S ? (IppSortFunc)ippsSortAscend_32s_I :
            depth == CC_32F ? (IppSortFunc)ippsSortAscend_32f_I :
            depth == CC_64F ? (IppSortFunc)ippsSortAscend_64f_I :
#endif
            0;
    else
        return depth == CC_8U ? (IppSortFunc)ippsSortDescend_8u_I :
#if IPP_DISABLE_BLOCK
            depth == CC_16U ? (IppSortFunc)ippsSortDescend_16u_I :
            depth == CC_16S ? (IppSortFunc)ippsSortDescend_16s_I :
            depth == CC_32S ? (IppSortFunc)ippsSortDescend_32s_I :
            depth == CC_32F ? (IppSortFunc)ippsSortDescend_32f_I :
            depth == CC_64F ? (IppSortFunc)ippsSortDescend_64f_I :
#endif
            0;
}

static IppFlipFunc getFlipFunc(int depth)
{
    CC_SUPPRESS_DEPRECATED_START
    return
            depth == CC_8U || depth == CC_8S ? (IppFlipFunc)ippsFlip_8u_I :
            depth == CC_16U || depth == CC_16S ? (IppFlipFunc)ippsFlip_16u_I :
            depth == CC_32S || depth == CC_32F ? (IppFlipFunc)ippsFlip_32f_I :
            depth == CC_64F ? (IppFlipFunc)ippsFlip_64f_I : 0;
    CC_SUPPRESS_DEPRECATED_END
}


#endif

template<typename T> static void sort_( const img_t& src, img_t& dst, int flags )
{
    CAutoBuffer<T> buf;
    T* bptr;
    int n, len;
    bool sortRows = (flags & 1) == CC_SORT_EVERY_ROW;
    bool inplace = src.data == dst.data;
    bool sortDescending = (flags & CC_SORT_DESCENDING) != 0;

    if( sortRows )
        n = src->rows, len = src->cols;
    else
    {
        n = src->cols, len = src->rows;
        buf.allocate(len);
    }
    bptr = (T*)buf;

#ifdef USE_IPP_SORT
    int depth = src CC_MAT_DEPTH();
    IppSortFunc ippSortFunc = 0;
    IppFlipFunc ippFlipFunc = 0;
    CC_IPP_CHECK()
    {
        ippSortFunc = getSortFunc(depth, sortDescending);
        ippFlipFunc = getFlipFunc(depth);
    }
#endif

    for( int i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        if( sortRows )
        {
            T* dptr = dst.ptr<T>(i);
            if( !inplace )
            {
                const T* sptr = src.ptr<T>(i);
                memcpy(dptr, sptr, sizeof(T) * len);
            }
            ptr = dptr;
        }
        else
        {
            for( int j = 0; j < len; j++ )
                ptr[j] = src.ptr<T>(j)[i];
        }

#ifdef USE_IPP_SORT
        if (!ippSortFunc || CC_INSTRUMENT_FUN_IPP(ippSortFunc, ptr, len) < 0)
#endif
        {
#ifdef USE_IPP_SORT
            if (depth == CC_8U)
                setIppErrorStatus();
#endif
            std::sort( ptr, ptr + len );
            if( sortDescending )
            {
#ifdef USE_IPP_SORT
                if (!ippFlipFunc || CC_INSTRUMENT_FUN_IPP(ippFlipFunc, ptr, len) < 0)
#endif
                {
#ifdef USE_IPP_SORT
                    setIppErrorStatus();
#endif
                    for( int j = 0; j < len/2; j++ )
                        T_SWAP(_Tp, ptr[j], ptr[len-1-j]);
                }
#ifdef USE_IPP_SORT
                else
                {
                    CC_IMPL_ADD(CC_IMPL_IPP);
                }
#endif
            }
        }
#ifdef USE_IPP_SORT
        else
        {
            CC_IMPL_ADD(CC_IMPL_IPP);
        }
#endif

        if( !sortRows )
            for( int j = 0; j < len; j++ )
                dst.ptr<T>(j)[i] = ptr[j];
    }
}

template<typename _Tp> class LessThanIdx
{
public:
    LessThanIdx( const _Tp* _arr ) : arr(_arr) {}
    bool operator()(int a, int b) const { return arr[a] < arr[b]; }
    const _Tp* arr;
};

#if defined USE_IPP_SORT && IPP_DISABLE_BLOCK

typedef IppStatus (C_STDCALL *IppSortIndexFunc)(void *, int *, int);

static IppSortIndexFunc getSortIndexFunc(int depth, bool sortDescending)
{
    if (!sortDescending)
        return depth == CC_8U ? (IppSortIndexFunc)ippsSortIndexAscend_8u_I :
            depth == CC_16U ? (IppSortIndexFunc)ippsSortIndexAscend_16u_I :
            depth == CC_16S ? (IppSortIndexFunc)ippsSortIndexAscend_16s_I :
            depth == CC_32S ? (IppSortIndexFunc)ippsSortIndexAscend_32s_I :
            depth == CC_32F ? (IppSortIndexFunc)ippsSortIndexAscend_32f_I :
            depth == CC_64F ? (IppSortIndexFunc)ippsSortIndexAscend_64f_I : 0;
    else
        return depth == CC_8U ? (IppSortIndexFunc)ippsSortIndexDescend_8u_I :
            depth == CC_16U ? (IppSortIndexFunc)ippsSortIndexDescend_16u_I :
            depth == CC_16S ? (IppSortIndexFunc)ippsSortIndexDescend_16s_I :
            depth == CC_32S ? (IppSortIndexFunc)ippsSortIndexDescend_32s_I :
            depth == CC_32F ? (IppSortIndexFunc)ippsSortIndexDescend_32f_I :
            depth == CC_64F ? (IppSortIndexFunc)ippsSortIndexDescend_64f_I : 0;
}

#endif

template<typename T> static void sortIdx_( const img_t& src, img_t& dst, int flags )
{
    CAutoBuffer<T> buf;
    FREE(); int* = MALLOC(int, ) ibuf;
    bool sortRows = (flags & 1) == CC_SORT_EVERY_ROW;
    bool sortDescending = (flags & CC_SORT_DESCENDING) != 0;

    CC_Assert( src.data != dst.data );

    int n, len;
    if( sortRows )
        n = src->rows, len = src->cols;
    else
    {
        n = src->cols, len = src->rows;
        buf.allocate(len);
        ibuf.allocate(len);
    }
    T* bptr = (T*)buf;
    int* _iptr = (int*)ibuf;

#if defined USE_IPP_SORT && IPP_DISABLE_BLOCK
    int depth = src CC_MAT_DEPTH();
    IppSortIndexFunc ippFunc = 0;
    IppFlipFunc ippFlipFunc = 0;
    CC_IPP_CHECK()
    {
        ippFunc = getSortIndexFunc(depth, sortDescending);
        ippFlipFunc = getFlipFunc(depth);
    }
#endif

    for( int i = 0; i < n; i++ )
    {
        T* ptr = bptr;
        int* iptr = _iptr;

        if( sortRows )
        {
            ptr = (T*)(src.data + src->step*i);
            iptr = dst.ptr<int>(i);
        }
        else
        {
            for( int j = 0; j < len; j++ )
                ptr[j] = src.ptr<T>(j)[i];
        }
        for( int j = 0; j < len; j++ )
            iptr[j] = j;

#if defined USE_IPP_SORT && IPP_DISABLE_BLOCK
        if (sortRows || !ippFunc || ippFunc(ptr, iptr, len) < 0)
#endif
        {
#if defined USE_IPP_SORT && IPP_DISABLE_BLOCK
            setIppErrorStatus();
#endif
            std::sort( iptr, iptr + len, LessThanIdx<T>(ptr) );
            if( sortDescending )
            {
#if defined USE_IPP_SORT && IPP_DISABLE_BLOCK
                if (!ippFlipFunc || ippFlipFunc(iptr, len) < 0)
#endif
                {
#if defined USE_IPP_SORT && IPP_DISABLE_BLOCK
                    setIppErrorStatus();
#endif
                    for( int j = 0; j < len/2; j++ )
                        T_SWAP(_Tp, iptr[j], iptr[len-1-j]);
                }
#if defined USE_IPP_SORT && IPP_DISABLE_BLOCK
                else
                {
                    CC_IMPL_ADD(CC_IMPL_IPP);
                }
#endif
            }
        }
#if defined USE_IPP_SORT && IPP_DISABLE_BLOCK
        else
        {
            CC_IMPL_ADD(CC_IMPL_IPP);
        }
#endif

        if( !sortRows )
            for( int j = 0; j < len; j++ )
                dst.ptr<int>(j)[i] = iptr[j];
    }
}

typedef void (*SortFunc)(const img_t& src, img_t& dst, int flags);



void sort( const img_t* src, img_t* dst, int flags )
{
    CC_INSTRUMENT_REGION()

    static SortFunc tab[] =
    {
        sort_<uchar>, sort_<schar>, sort_<ushort>, sort_<short>,
        sort_<int>, sort_<float>, sort_<double>, 0
    };
    
    SortFunc func = tab[src CC_MAT_DEPTH()];
    CC_Assert( src.dims <= 2 && src CC_MAT_CN() == 1 && func != 0 );
    dst cvSetMat( cvGetSize(src), src CC_MAT_TYPE() );
    img_t dst = dst;
    func( src, dst, flags );
}

void sortIdx( const img_t* src, img_t* dst, int flags )
{
    CC_INSTRUMENT_REGION()

    static SortFunc tab[] =
    {
        sortIdx_<uchar>, sortIdx_<schar>, sortIdx_<ushort>, sortIdx_<short>,
        sortIdx_<int>, sortIdx_<float>, sortIdx_<double>, 0
    };
    
    SortFunc func = tab[src CC_MAT_DEPTH()];
    CC_Assert( src.dims <= 2 && src CC_MAT_CN() == 1 && func != 0 );

    img_t dst = dst;
    if( dst.data == src.data )
        dst.release();
    dst cvSetMat( cvGetSize(src), CC_32S );
    dst = dst;
    func( src, dst, flags );
}


CC_IMPL void cvSetIdentity1( img_t* arr, double value ) {
    cvSetIdentity(arr, cScalar(value, value, value, value));
}
CC_IMPL void cvSetIdentity( img_t* arr, CScalar value )
{
    img_t m = cvarrToMat(arr);
    setIdentity(m, value);
}


CC_IMPL CScalar cvTrace( const img_t* arr )
{
    return trace(cvarrToMat(arr));
}


CC_IMPL void cvTranspose( const img_t* srcarr, img_t* dstarr )
{
    img_t src = cvarrToMat(srcarr), dst = cvarrToMat(dstarr);

    CC_Assert( src->rows == dst->cols && src->cols == dst->rows && src CC_MAT_TYPE() == dst CC_MAT_TYPE() );
    transpose( src, dst );
}


CC_IMPL void cvCompleteSymm( img_t* matrix, int LtoR )
{
    img_t m = cvarrToMat(matrix);
    completeSymm( m, LtoR != 0 );
}


CC_IMPL void cvCrossProduct( const img_t* srcAarr, const img_t* srcBarr, img_t* dstarr )
{
    img_t srcA = cvarrToMat(srcAarr), dst = cvarrToMat(dstarr);

    CC_Assert( srccvGetSize(A) == dst.size() && srcA CC_MAT_TYPE() == dst CC_MAT_TYPE() );
    srcA.cross(cvarrToMat(srcBarr)).copyTo(dst);
}


CC_IMPL void
cvReduce( const img_t* srcarr, img_t* dstarr, int dim, int op )
{
    img_t src = cvarrToMat(srcarr), dst = cvarrToMat(dstarr);

    if( dim < 0 )
        dim = src->rows > dst->rows ? 0 : src->cols > dst->cols ? 1 : dst->cols == 1;

    if( dim > 1 )
        CC_Error( CC_StsOutOfRange, "The reduced dimensionality index is out of range" );

    if( (dim == 0 && (dst->cols != src->cols || dst->rows != 1)) ||
        (dim == 1 && (dst->rows != src->rows || dst->cols != 1)) )
        CC_Error( CC_StsBadSize, "The output array size is incorrect" );

    if( src CC_MAT_CN() != dst CC_MAT_CN() )
        CC_Error( CC_StsUnmatchedFormats, "Input and output arrays must have the same number of channels" );

    reduce(src, dst, dim, op, dst CC_MAT_TYPE());
}


CC_IMPL img_t*
cvRange( img_t* arr, double start, double end )
{
    img_t stub, *mat = (img_t*)arr;
    int step;
    double val = start;

    if( !CC_IS_MAT(mat) )
        mat = cvGetMat( mat, &stub);

    int rows = mat->rows;
    int cols = mat->cols;
    int type = CC_MAT_DEPTH(mat->tid);
    double delta = (end-start)/(rows*cols);

    if( CC_IS_MAT_CONT(mat->tid) )
    {
        cols *= rows;
        rows = 1;
        step = 1;
    }
    else
        step = mat->step / CC_TYPE_SIZE(type);

    if( type == CC_32SC1 )
    {
        int* idata = mat->data.i;
        int ival = cRound(val), idelta = cRound(delta);

        if( fabs(val - ival) < DBL_EPSILON &&
            fabs(delta - idelta) < DBL_EPSILON )
        {
            for( int i = 0; i < rows; i++, idata += step )
                for( int j = 0; j < cols; j++, ival += idelta )
                    idata[j] = ival;
        }
        else
        {
            for( int i = 0; i < rows; i++, idata += step )
                for( int j = 0; j < cols; j++, val += delta )
                    idata[j] = cRound(val);
        }
    }
    else if( type == CC_32FC1 )
    {
        float* fdata = mat->data.fl;
        for( int i = 0; i < rows; i++, fdata += step )
            for( int j = 0; j < cols; j++, val += delta )
                fdata[j] = (float)val;
    }
    else
        CC_Error( CC_StsUnsupportedFormat, "The function only supports 32sC1 and 32fC1 datatypes" );

    return arr;
}


CC_IMPL void
cvSort( const img_t* src, img_t* dst, img_t* _idx, int flags )
{
    img_t src = cvarrToMat(src);

    if( _idx )
    {
        img_t idx0 = cvarrToMat(_idx), idx = idx0;
        CC_Assert( cvGetSize(src) == idx.size() && idx CC_MAT_TYPE() == CC_32S && src.data != idx.data );
        sortIdx( src, idx, flags );
        CC_Assert( idx0.data == idx.data );
    }

    if( dst )
    {
        img_t dst0 = cvarrToMat(dst), dst = dst0;
        CC_Assert( cvGetSize(src) == dst.size() && src CC_MAT_TYPE() == dst CC_MAT_TYPE() );
        sort( src, dst, flags );
        CC_Assert( dst0.data == dst.data );
    }
}


CC_IMPL int
cvKMeans2( const img_t* _samples, int cluster_count, img_t* _labels,
           CTermCriteria termcrit, int attempts, CRNG*,
           int flags, img_t* _centers, double* _compactness )
{
    img_t data = cvarrToMat(_samples), labels = cvarrToMat(_labels), centers;
    if( _centers )
    {
        centers = cvarrToMat(_centers);

        centers = centers cvReshape(,1);
        data = data cvReshape(,1);

        CC_Assert( !centers.empty() );
        CC_Assert( centers->rows == cluster_count );
        CC_Assert( centers->cols == data->cols );
        CC_Assert( centers CC_MAT_DEPTH() == data CC_MAT_DEPTH() );
    }
    CC_Assert( labels CC_IS_CONT_MAT() && labels CC_MAT_TYPE() == CC_32S &&
        (labels->cols == 1 || labels->rows == 1) &&
        labels->cols + labels->rows - 1 == data->rows );

    double compactness = kmeans(data, cluster_count, labels, termcrit, attempts,
                                    flags, _centers ? _OutputArray(centers) : _OutputArray() );
    if( _compactness )
        *_compactness = compactness;
    return 1;
}

///////////////////////////// n-dimensional matrices ////////////////////////////

img_t img_t::reshape(int _cn, int _newndims, const int* _newsz) const
{
    if(_newndims == dims)
    {
        if(_newsz == 0)
            return reshape(_cn);
        if(_newndims == 2)
            return reshape(_cn, _newsz[0]);
    }

    if (isContinuous())
    {
        CC_Assert(_cn >= 0 && _newndims > 0 && _newndims <= CC_MAX_DIM && _newsz);

        if (_cn == 0)
            _cn = this->CC_MAT_CN();
        else
            CC_Assert(_cn <= CC_CN_MAX);

        size_t total_elem1_ref = this->total() * this->CC_MAT_CN();
        size_t total_elem1 = _cn;

        CAutoBuffer<int, 4> newsz_buf( (size_t)_newndims );

        for (int i = 0; i < _newndims; i++)
        {
            CC_Assert(_newsz[i] >= 0);

            if (_newsz[i] > 0)
                newsz_buf[i] = _newsz[i];
            else if (i < dims)
                newsz_buf[i] = this->size[i];
            else
                CC_Error(CC_StsOutOfRange, "Copy dimension (which has zero size) is not present in source matrix");

            total_elem1 *= (size_t)newsz_buf[i];
        }

        if (total_elem1 != total_elem1_ref)
            CC_Error(CC_StsUnmatchedSizes, "Requested and source matrices have different count of elements");

        img_t hdr = *this;
        hdr.flags = (hdr.flags & ~CC_MAT_CN_MASK) | ((_cn-1) << CC_CN_SHIFT);
        setSize(hdr, _newndims, (int*)newsz_buf, NULL, true);

        return hdr;
    }

    CC_Error(CC_StsNotImplemented, "Reshaping of n-dimensional non-continuous matrices is not supported yet");
    // TBD
    return img_t();
}

/////////////////////////////////// NAryMatIterator //////////////////////////////////

/** @brief n-ary multi-dimensional array iterator.

Use the class to implement unary, binary, and, generally, n-ary element-wise operations on
multi-dimensional arrays. Some of the arguments of an n-ary function may be continuous arrays, some
may be not. It is possible to use conventional MatIterator 's for each array but incrementing all of
the iterators after each small operations may be a big overhead. In this case consider using
NAryMatIterator to iterate through several matrices simultaneously as long as they have the same
geometry (dimensionality and all the dimension sizes are the same). On each iteration `it.planes[0]`,
`it.planes[1]`,... will be the slices of the corresponding matrices.

The example below illustrates how you can compute a normalized and threshold 3D color histogram:
@code
    void computeNormalizedColorHist(const img_t& image, img_t& hist, int N, double minProb)
    {
        const int histSize[] = {N, N, N};

        // make sure that the histogram has a proper size and type
        hist cvSetMat(3, histSize, CC_32F);

        // and clear it
        hist = Scalar(0);

        // the loop below assumes that the image
        // is a 8-bit 3-channel. check it.
        CC_Assert(image CC_MAT_TYPE() == CC_8UC3);
        MatConstIterator_<Vec3b> it = image.begin<Vec3b>(),
                                 it_end = image.end<Vec3b>();
        for( ; it != it_end; ++it )
        {
            const Vec3b& pix = *it;
            hist.at<float>(pix[0]*N/256, pix[1]*N/256, pix[2]*N/256) += 1.f;
        }

        minProb *= image->rows*image->cols;

        // initialize iterator (the style is different from STL).
        // after initialization the iterator will contain
        // the number of slices or planes the iterator will go through.
        // it simultaneously increments iterators for several matrices
        // supplied as a null terminated list of pointers
        const img_t* arrays[] = {&hist, 0};
        img_t planes[1];
        NAryMatIterator itNAry(arrays, planes, 1);
        double s = 0;
        // iterate through the matrix. on each iteration
        // itNAry.planes[i] (of type img_t) will be set to the current plane
        // of the i-th n-dim matrix passed to the iterator constructor.
        for(int p = 0; p < itNAry.nplanes; p++, ++itNAry)
        {
            threshold(itNAry.planes[0], itNAry.planes[0], minProb, 0, CC_THRESH_TOZERO);
            s += sum(itNAry.planes[0])[0];
        }

        s = 1./s;
        itNAry = NAryMatIterator(arrays, planes, 1);
        for(int p = 0; p < itNAry.nplanes; p++, ++itNAry)
            itNAry.planes[0] *= s;
    }
@endcode
 */
#endif
#if 0
class CC_EXPORTS NAryMatIterator
{
public:
    //! the default constructor
    NAryMatIterator();
    //! the full constructor taking arbitrary number of n-dim matrices
    NAryMatIterator(const img_t** arrays, uchar** ptrs, int narrays=-1);
    //! the full constructor taking arbitrary number of n-dim matrices
    NAryMatIterator(const img_t** arrays, img_t* planes, int narrays=-1);
    //! the separate iterator initialization method
    void init(const img_t** arrays, img_t* planes, uchar** ptrs, int narrays=-1);

    //! proceeds to the next plane of every iterated matrix
    NAryMatIterator& operator ++();
    //! proceeds to the next plane of every iterated matrix (postfix increment operator)
    NAryMatIterator operator ++(int);

    //! the iterated arrays
    const img_t** arrays;
    //! the current planes
    img_t* planes;
    //! data pointers
    uchar** ptrs;
    //! the number of arrays
    int narrays;
    //! the number of hyper-planes that the iterator steps through
    size_t nplanes;
    //! the size of each segment (in elements)
    size_t size;
protected:
    int iterdepth;
    size_t idx;
};


NAryMatIterator::NAryMatIterator()
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
}

NAryMatIterator::NAryMatIterator(const img_t** _arrays, img_t* _planes, int _narrays)
: arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, _planes, 0, _narrays);
}

NAryMatIterator::NAryMatIterator(const img_t** _arrays, uchar** _ptrs, int _narrays)
    : arrays(0), planes(0), ptrs(0), narrays(0), nplanes(0), size(0), iterdepth(0), idx(0)
{
    init(_arrays, 0, _ptrs, _narrays);
}

void NAryMatIterator::init(const img_t** _arrays, img_t* _planes, uchar** _ptrs, int _narrays)
{
    CC_Assert( _arrays && (_ptrs || _planes) );
    int i, j, d1=0, i0 = -1, d = -1;

    arrays = _arrays;
    ptrs = _ptrs;
    planes = _planes;
    narrays = _narrays;
    nplanes = 0;
    size = 0;

    if( narrays < 0 )
    {
        for( i = 0; _arrays[i] != 0; i++ )
            ;
        narrays = i;
        CC_Assert(narrays <= 1000);
    }

    iterdepth = 0;

    for( i = 0; i < narrays; i++ )
    {
        CC_Assert(arrays[i] != 0);
        const img_t* A = arrays[i];
        if( ptrs )
            ptrs[i] = A->data;

        if( !A->data )
            continue;

        if( i0 < 0 )
        {
            int size[] = {A->h, A->w, A->c};
            int step[] = {A->step, A->c, 1};
            i0 = i;
            d = countof(size);

            // find the first dimensionality which is different from 1;
            // in any of the arrays the first "d1" step do not affect the continuity
            for( d1 = 0; d1 < d; d1++ )
                if( size[d1] > 1 )
                    break;
        }
        else
            CC_Assert( A->h == arrays[i0]->h && A->w == arrays[i0]->w );

        if( !CC_IS_CONT_MAT(A) )
        {
            CC_Assert( A->step[d-1] == CC_TYPE_SIZE(A->tid) );
            for( j = d-1; j > d1; j-- )
                if( A->step[j]*size[j] < A->step[j-1] )
                    break;
            iterdepth = MAX(iterdepth, j);
        }
    }

    if( i0 >= 0 )
    {
        size = arrays[i0]->size[d-1];
        for( j = d-1; j > iterdepth; j-- )
        {
            int64 total1 = (int64)size*arrays[i0]->size[j-1];
            if( total1 != (int)total1 )
                break;
            size = (int)total1;
        }

        iterdepth = j;
        if( iterdepth == d1 )
            iterdepth = 0;

        nplanes = 1;
        for( j = iterdepth-1; j >= 0; j-- )
            nplanes *= arrays[i0]->size[j];
    }
    else
        iterdepth = 0;

    idx = 0;

    if( !planes )
        return;

    for( i = 0; i < narrays; i++ )
    {
        CC_Assert(arrays[i] != 0);
        const img_t& A = *arrays[i];

        if( !A.data )
        {
            planes[i] = img_t();
            continue;
        }

        planes[i] = img_t(1, (int)size, A CC_MAT_TYPE(), A.data);
    }
}


NAryMatIterator& NAryMatIterator::operator ++()
{
    if( idx >= nplanes-1 )
        return *this;
    ++idx;

    if( iterdepth == 1 )
    {
        if( ptrs )
        {
            for( int i = 0; i < narrays; i++ )
            {
                if( !ptrs[i] )
                    continue;
                ptrs[i] = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
        if( planes )
        {
            for( int i = 0; i < narrays; i++ )
            {
                if( !planes[i].data )
                    continue;
                planes[i].data = arrays[i]->data + arrays[i]->step[0]*idx;
            }
        }
    }
    else
    {
        for( int i = 0; i < narrays; i++ )
        {
            const img_t& A = *arrays[i];
            if( !A.data )
                continue;
            int _idx = (int)idx;
            uchar* data = A.data;
            for( int j = iterdepth-1; j >= 0 && _idx > 0; j-- )
            {
                int szi = A.size[j], t = _idx/szi;
                data += (_idx - t * szi)*A->step[j];
                _idx = t;
            }
            if( ptrs )
                ptrs[i] = data;
            if( planes )
                planes[i].data = data;
        }
    }

    return *this;
}

NAryMatIterator NAryMatIterator::operator ++(int)
{
    NAryMatIterator it = *this;
    ++*this;
    return it;
}


///////////////////////////////////////////////////////////////////////////
//                              MatConstIterator                         //
///////////////////////////////////////////////////////////////////////////

Point MatConstIterator::pos() const
{
    if( !m )
        return Point();
    CC_DbgAssert(m->dims <= 2);

    ptrdiff_t ofs = ptr - m->ptr();
    int y = (int)(ofs/m->step[0]);
    return Point((int)((ofs - y*m->step[0])/elemSize), y);
}

void MatConstIterator::pos(int* _idx) const
{
    CC_Assert(m != 0 && _idx);
    ptrdiff_t ofs = ptr - m->ptr();
    for( int i = 0; i < m->dims; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        _idx[i] = (int)v;
    }
}

ptrdiff_t MatConstIterator::lpos() const
{
    if(!m)
        return 0;
    if( m->isContinuous() )
        return (ptr - sliceStart)/elemSize;
    ptrdiff_t ofs = ptr - m->ptr();
    int i, d = m->dims;
    if( d == 2 )
    {
        ptrdiff_t y = ofs/m->step[0];
        return y*m->cols + (ofs - y*m->step[0])/elemSize;
    }
    ptrdiff_t result = 0;
    for( i = 0; i < d; i++ )
    {
        size_t s = m->step[i], v = ofs/s;
        ofs -= v*s;
        result = result*m->size[i] + v;
    }
    return result;
}

void MatConstIterator::seek(ptrdiff_t ofs, bool relative)
{
    if( m->isContinuous() )
    {
        ptr = (relative ? ptr : sliceStart) + ofs*elemSize;
        if( ptr < sliceStart )
            ptr = sliceStart;
        else if( ptr > sliceEnd )
            ptr = sliceEnd;
        return;
    }

    int d = m->dims;
    if( d == 2 )
    {
        ptrdiff_t ofs0, y;
        if( relative )
        {
            ofs0 = ptr - m->ptr();
            y = ofs0/m->step[0];
            ofs += y*m->cols + (ofs0 - y*m->step[0])/elemSize;
        }
        y = ofs/m->cols;
        int y1 = MIN(MAX((int)y, 0), m->rows-1);
        sliceStart = m->ptr(y1);
        sliceEnd = sliceStart + m->cols*elemSize;
        ptr = y < 0 ? sliceStart : y >= m->rows ? sliceEnd :
            sliceStart + (ofs - y*m->cols)*elemSize;
        return;
    }

    if( relative )
        ofs += lpos();

    if( ofs < 0 )
        ofs = 0;

    int szi = m->size[d-1];
    ptrdiff_t t = ofs/szi;
    int v = (int)(ofs - t*szi);
    ofs = t;
    ptr = m->ptr() + v*elemSize;
    sliceStart = m->ptr();

    for( int i = d-2; i >= 0; i-- )
    {
        szi = m->size[i];
        t = ofs/szi;
        v = (int)(ofs - t*szi);
        ofs = t;
        sliceStart += v*m->step[i];
    }

    sliceEnd = sliceStart + m->size[d-1]*elemSize;
    if( ofs > 0 )
        ptr = sliceEnd;
    else
        ptr = sliceStart + (ptr - m->ptr());
}

void MatConstIterator::seek(const int* _idx, bool relative)
{
    int d = m->dims;
    ptrdiff_t ofs = 0;
    if( !_idx )
        ;
    else if( d == 2 )
        ofs = _idx[0]*m->size[1] + _idx[1];
    else
    {
        for( int i = 0; i < d; i++ )
            ofs = ofs*m->size[i] + _idx[i];
    }
    seek(ofs, relative);
}

//////////////////////////////// SparseMat ////////////////////////////////

template<typename T1, typename T2> void
convertData_(const void* _from, void* _to, int cn)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    if( cn == 1 )
        *to = saturate_cast<T2>(*from);
    else
        for( int i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]);
}

template<typename T1, typename T2> void
convertScaleData_(const void* _from, void* _to, int cn, double alpha, double beta)
{
    const T1* from = (const T1*)_from;
    T2* to = (T2*)_to;
    if( cn == 1 )
        *to = saturate_cast<T2>(*from*alpha + beta);
    else
        for( int i = 0; i < cn; i++ )
            to[i] = saturate_cast<T2>(from[i]*alpha + beta);
}

typedef void (*ConvertData)(const void* from, void* to, int cn);
typedef void (*ConvertScaleData)(const void* from, void* to, int cn, double alpha, double beta);

static ConvertData getConvertElem(int fromType, int toType)
{
    static ConvertData tab[][8] =
    {{ convertData_<uchar, uchar>, convertData_<uchar, schar>,
      convertData_<uchar, ushort>, convertData_<uchar, short>,
      convertData_<uchar, int>, convertData_<uchar, float>,
      convertData_<uchar, double>, 0 },

    { convertData_<schar, uchar>, convertData_<schar, schar>,
      convertData_<schar, ushort>, convertData_<schar, short>,
      convertData_<schar, int>, convertData_<schar, float>,
      convertData_<schar, double>, 0 },

    { convertData_<ushort, uchar>, convertData_<ushort, schar>,
      convertData_<ushort, ushort>, convertData_<ushort, short>,
      convertData_<ushort, int>, convertData_<ushort, float>,
      convertData_<ushort, double>, 0 },

    { convertData_<short, uchar>, convertData_<short, schar>,
      convertData_<short, ushort>, convertData_<short, short>,
      convertData_<short, int>, convertData_<short, float>,
      convertData_<short, double>, 0 },

    { convertData_<int, uchar>, convertData_<int, schar>,
      convertData_<int, ushort>, convertData_<int, short>,
      convertData_<int, int>, convertData_<int, float>,
      convertData_<int, double>, 0 },

    { convertData_<float, uchar>, convertData_<float, schar>,
      convertData_<float, ushort>, convertData_<float, short>,
      convertData_<float, int>, convertData_<float, float>,
      convertData_<float, double>, 0 },

    { convertData_<double, uchar>, convertData_<double, schar>,
      convertData_<double, ushort>, convertData_<double, short>,
      convertData_<double, int>, convertData_<double, float>,
      convertData_<double, double>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertData func = tab[CC_MAT_DEPTH(fromType)][CC_MAT_DEPTH(toType)];
    CC_Assert( func != 0 );
    return func;
}

static ConvertScaleData getConvertScaleElem(int fromType, int toType)
{
    static ConvertScaleData tab[][8] =
    {{ convertScaleData_<uchar, uchar>, convertScaleData_<uchar, schar>,
      convertScaleData_<uchar, ushort>, convertScaleData_<uchar, short>,
      convertScaleData_<uchar, int>, convertScaleData_<uchar, float>,
      convertScaleData_<uchar, double>, 0 },

    { convertScaleData_<schar, uchar>, convertScaleData_<schar, schar>,
      convertScaleData_<schar, ushort>, convertScaleData_<schar, short>,
      convertScaleData_<schar, int>, convertScaleData_<schar, float>,
      convertScaleData_<schar, double>, 0 },

    { convertScaleData_<ushort, uchar>, convertScaleData_<ushort, schar>,
      convertScaleData_<ushort, ushort>, convertScaleData_<ushort, short>,
      convertScaleData_<ushort, int>, convertScaleData_<ushort, float>,
      convertScaleData_<ushort, double>, 0 },

    { convertScaleData_<short, uchar>, convertScaleData_<short, schar>,
      convertScaleData_<short, ushort>, convertScaleData_<short, short>,
      convertScaleData_<short, int>, convertScaleData_<short, float>,
      convertScaleData_<short, double>, 0 },

    { convertScaleData_<int, uchar>, convertScaleData_<int, schar>,
      convertScaleData_<int, ushort>, convertScaleData_<int, short>,
      convertScaleData_<int, int>, convertScaleData_<int, float>,
      convertScaleData_<int, double>, 0 },

    { convertScaleData_<float, uchar>, convertScaleData_<float, schar>,
      convertScaleData_<float, ushort>, convertScaleData_<float, short>,
      convertScaleData_<float, int>, convertScaleData_<float, float>,
      convertScaleData_<float, double>, 0 },

    { convertScaleData_<double, uchar>, convertScaleData_<double, schar>,
      convertScaleData_<double, ushort>, convertScaleData_<double, short>,
      convertScaleData_<double, int>, convertScaleData_<double, float>,
      convertScaleData_<double, double>, 0 },

    { 0, 0, 0, 0, 0, 0, 0, 0 }};

    ConvertScaleData func = tab[CC_MAT_DEPTH(fromType)][CC_MAT_DEPTH(toType)];
    CC_Assert( func != 0 );
    return func;
}

enum { HASH_SIZE0 = 8 };

static inline void copyElem(const uchar* from, uchar* to, size_t elemSize)
{
    size_t i;
    for( i = 0; i + sizeof(int) <= elemSize; i += sizeof(int) )
        *(int*)(to + i) = *(const int*)(from + i);
    for( ; i < elemSize; i++ )
        to[i] = from[i];
}

static inline bool isZeroElem(const uchar* data, size_t elemSize)
{
    size_t i;
    for( i = 0; i + sizeof(int) <= elemSize; i += sizeof(int) )
        if( *(int*)(data + i) != 0 )
            return false;
    for( ; i < elemSize; i++ )
        if( data[i] != 0 )
            return false;
    return true;
}

SparseMat::Hdr::Hdr( int _dims, const int* _sizes, int _type )
{
    refcount = 1;

    dims = _dims;
    valueOffset = (int)cAlignSize(sizeof(SparseMat::Node) - MAX_DIM*sizeof(int) +
                                 dims*sizeof(int), CC_ELEM_SIZE1(_type));
    nodeSize = cAlignSize(valueOffset +
        CC_TYPE_SIZE(_type), (int)sizeof(size_t));

    int i;
    for( i = 0; i < dims; i++ )
        size[i] = _sizes[i];
    for( ; i < CC_MAX_DIM; i++ )
        size[i] = 0;
    clear();
}

void SparseMat::Hdr::clear()
{
    hashtab.clear();
    hashtab.resize(HASH_SIZE0);
    pool.clear();
    pool.resize(nodeSize);
    nodeCount = freeList = 0;
}


SparseMat::SparseMat(const img_t& m)
: flags(MAGIC_VAL), hdr(0)
{
    create( m.dims, m.size, m CC_MAT_TYPE() );

    int i, idx[CC_MAX_DIM] = {0}, d = m.dims, lastSize = m.size[d - 1];
    size_t esz = m CC_TYPE_SIZE();
    const uchar* dptr = m->tt.data;

    for(;;)
    {
        for( i = 0; i < lastSize; i++, dptr += esz )
        {
            if( isZeroElem(dptr, esz) )
                continue;
            idx[d-1] = i;
            uchar* to = newNode(idx, hash(idx));
            copyElem( dptr, to, esz );
        }

        for( i = d - 2; i >= 0; i-- )
        {
            dptr += m->step[i] - m.size[i+1]*m->step[i+1];
            if( ++idx[i] < m.size[i] )
                break;
            idx[i] = 0;
        }
        if( i < 0 )
            break;
    }
}

void SparseMat::create(int d, const int* _sizes, int _type)
{
    CC_Assert( _sizes && 0 < d && d <= CC_MAX_DIM );
    for( int i = 0; i < d; i++ )
        CC_Assert( _sizes[i] > 0 );
    _type = CC_MAT_DEPTH(_type);
    if( hdr && _type == type() && hdr->dims == d && hdr->refcount == 1 )
    {
        int i;
        for( i = 0; i < d; i++ )
            if( _sizes[i] != hdr->size[i] )
                break;
        if( i == d )
        {
            clear();
            return;
        }
    }
    int _sizes_backup[CC_MAX_DIM]; // #5991
    if (_sizes == hdr->size)
    {
        for(int i = 0; i < d; i++ )
            _sizes_backup[i] = _sizes[i];
        _sizes = _sizes_backup;
    }
    release();
    flags = MAGIC_VAL | _type;
    hdr = new Hdr(d, _sizes, _type);
}

void SparseMat::copyTo( SparseMat& m ) const
{
    if( hdr == m.hdr )
        return;
    if( !hdr )
    {
        m.release();
        return;
    }
    m cvSetMat( hdr->dims, hdr->size, type() );
    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    for( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        uchar* to = m.newNode(n->idx, n->hashval);
        copyElem( from.ptr, to, esz );
    }
}

void SparseMat::copyTo( img_t& m ) const
{
    CC_Assert( hdr );
    int ndims = dims();
    m cvSetMat( ndims, hdr->size, type() );
    m = Scalar(0);

    SparseMatConstIterator from = begin();
    size_t N = nzcount(), esz = elemSize();

    for( size_t i = 0; i < N; i++, ++from )
    {
        const Node* n = from.node();
        copyElem( from.ptr, (ndims > 1 ? m.ptr(n->idx) : m.ptr(n->idx[0])), esz);
    }
}


void SparseMat::convertTo( SparseMat& m, int rtype, double alpha ) const
{
    int cn = CC_MAT_CN();
    if( rtype < 0 )
        rtype = type();
    rtype = CC_MAKETYPE(rtype, cn);
    if( hdr == m.hdr && rtype != type()  )
    {
        SparseMat temp;
        convertTo(temp, rtype, alpha);
        m = temp;
        return;
    }

    CC_Assert(hdr != 0);
    if( hdr != m.hdr )
        m cvSetMat( hdr->dims, hdr->size, rtype );

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    if( alpha == 1 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        for( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn );
        }
    }
    else
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        for( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = hdr == m.hdr ? from.ptr : m.newNode(n->idx, n->hashval);
            cvtfunc( from.ptr, to, cn, alpha, 0 );
        }
    }
}


void SparseMat::convertTo( img_t& m, int rtype, double alpha, double beta ) const
{
    int cn = CC_MAT_CN();
    if( rtype < 0 )
        rtype = type();
    rtype = CC_MAKETYPE(rtype, cn);

    CC_Assert( hdr );
    m cvSetMat( dims(), hdr->size, rtype );
    m = Scalar(beta);

    SparseMatConstIterator from = begin();
    size_t N = nzcount();

    if( alpha == 1 && beta == 0 )
    {
        ConvertData cvtfunc = getConvertElem(type(), rtype);
        for( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn );
        }
    }
    else
    {
        ConvertScaleData cvtfunc = getConvertScaleElem(type(), rtype);
        for( size_t i = 0; i < N; i++, ++from )
        {
            const Node* n = from.node();
            uchar* to = m.ptr(n->idx);
            cvtfunc( from.ptr, to, cn, alpha, beta );
        }
    }
}

void SparseMat::clear()
{
    if( hdr )
        hdr->clear();
}

uchar* SparseMat::ptr(int i0, bool createMissing, size_t* hashval)
{
    CC_Assert( hdr && hdr->dims == 1 );
    size_t h = hashval ? *hashval : hash(i0);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    while( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        if( elem->hashval == h && elem->idx[0] == i0 )
            return &value<uchar>(elem);
        nidx = elem->next;
    }

    if( createMissing )
    {
        int idx[] = { i0 };
        return newNode( idx, h );
    }
    return NULL;
}

uchar* SparseMat::ptr(int i0, int i1, bool createMissing, size_t* hashval)
{
    CC_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    while( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        if( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            return &value<uchar>(elem);
        nidx = elem->next;
    }

    if( createMissing )
    {
        int idx[] = { i0, i1 };
        return newNode( idx, h );
    }
    return NULL;
}

uchar* SparseMat::ptr(int i0, int i1, int i2, bool createMissing, size_t* hashval)
{
    CC_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    while( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        if( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            return &value<uchar>(elem);
        nidx = elem->next;
    }

    if( createMissing )
    {
        int idx[] = { i0, i1, i2 };
        return newNode( idx, h );
    }
    return NULL;
}

uchar* SparseMat::ptr(const int* idx, bool createMissing, size_t* hashval)
{
    CC_Assert( hdr );
    int i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx];
    uchar* pool = &hdr->pool[0];
    while( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        if( elem->hashval == h )
        {
            for( i = 0; i < d; i++ )
                if( elem->idx[i] != idx[i] )
                    break;
            if( i == d )
                return &value<uchar>(elem);
        }
        nidx = elem->next;
    }

    return createMissing ? newNode(idx, h) : NULL;
}

void SparseMat::erase(int i0, int i1, size_t* hashval)
{
    CC_Assert( hdr && hdr->dims == 2 );
    size_t h = hashval ? *hashval : hash(i0, i1);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    while( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        if( elem->hashval == h && elem->idx[0] == i0 && elem->idx[1] == i1 )
            break;
        previdx = nidx;
        nidx = elem->next;
    }

    if( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(int i0, int i1, int i2, size_t* hashval)
{
    CC_Assert( hdr && hdr->dims == 3 );
    size_t h = hashval ? *hashval : hash(i0, i1, i2);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    while( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        if( elem->hashval == h && elem->idx[0] == i0 &&
            elem->idx[1] == i1 && elem->idx[2] == i2 )
            break;
        previdx = nidx;
        nidx = elem->next;
    }

    if( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::erase(const int* idx, size_t* hashval)
{
    CC_Assert( hdr );
    int i, d = hdr->dims;
    size_t h = hashval ? *hashval : hash(idx);
    size_t hidx = h & (hdr->hashtab.size() - 1), nidx = hdr->hashtab[hidx], previdx=0;
    uchar* pool = &hdr->pool[0];
    while( nidx != 0 )
    {
        Node* elem = (Node*)(pool + nidx);
        if( elem->hashval == h )
        {
            for( i = 0; i < d; i++ )
                if( elem->idx[i] != idx[i] )
                    break;
            if( i == d )
                break;
        }
        previdx = nidx;
        nidx = elem->next;
    }

    if( nidx )
        removeNode(hidx, nidx, previdx);
}

void SparseMat::resizeHashTab(size_t newsize)
{
    newsize = MAX(newsize, (size_t)8);
    if((newsize & (newsize-1)) != 0)
        newsize = (size_t)1 << cCeil(std::log((double)newsize)/CC_LOG2);

    size_t hsize = hdr->hashtab.size();
    std::vector<size_t> _newh(newsize);
    size_t* newh = &_newh[0];
    for( size_t i = 0; i < newsize; i++ )
        newh[i] = 0;
    uchar* pool = &hdr->pool[0];
    for( size_t i = 0; i < hsize; i++ )
    {
        size_t nidx = hdr->hashtab[i];
        while( nidx )
        {
            Node* elem = (Node*)(pool + nidx);
            size_t next = elem->next;
            size_t newhidx = elem->hashval & (newsize - 1);
            elem->next = newh[newhidx];
            newh[newhidx] = nidx;
            nidx = next;
        }
    }
    hdr->hashtab = _newh;
}

uchar* SparseMat::newNode(const int* idx, size_t hashval)
{
    const int HASH_MAX_FILL_FACTOR=3;
    assert(hdr);
    size_t hsize = hdr->hashtab.size();
    if( ++hdr->nodeCount > hsize*HASH_MAX_FILL_FACTOR )
    {
        resizeHashTab(MAX(hsize*2, (size_t)8));
        hsize = hdr->hashtab.size();
    }

    if( !hdr->freeList )
    {
        size_t i, nsz = hdr->nodeSize, psize = hdr->pool.size(),
            newpsize = MAX(psize*3/2, 8*nsz);
        newpsize = (newpsize/nsz)*nsz;
        hdr->pool.resize(newpsize);
        uchar* pool = &hdr->pool[0];
        hdr->freeList = MAX(psize, nsz);
        for( i = hdr->freeList; i < newpsize - nsz; i += nsz )
            ((Node*)(pool + i))->next = i + nsz;
        ((Node*)(pool + i))->next = 0;
    }
    size_t nidx = hdr->freeList;
    Node* elem = (Node*)&hdr->pool[nidx];
    hdr->freeList = elem->next;
    elem->hashval = hashval;
    size_t hidx = hashval & (hsize - 1);
    elem->next = hdr->hashtab[hidx];
    hdr->hashtab[hidx] = nidx;

    int i, d = hdr->dims;
    for( i = 0; i < d; i++ )
        elem->idx[i] = idx[i];
    size_t esz = elemSize();
    uchar* p = &value<uchar>(elem);
    if( esz == sizeof(float) )
        *((float*)p) = 0.f;
    else if( esz == sizeof(double) )
        *((double*)p) = 0.;
    else
        memset(p, 0, esz);

    return p;
}


void SparseMat::removeNode(size_t hidx, size_t nidx, size_t previdx)
{
    Node* n = node(nidx);
    if( previdx )
    {
        Node* prev = node(previdx);
        prev->next = n->next;
    }
    else
        hdr->hashtab[hidx] = n->next;
    n->next = hdr->freeList;
    hdr->freeList = nidx;
    --hdr->nodeCount;
}


SparseMatConstIterator::SparseMatConstIterator(const SparseMat* _m)
: m((SparseMat*)_m), hashidx(0), ptr(0)
{
    if(!_m || !_m->hdr)
        return;
    SparseMat::Hdr& hdr = *m->hdr;
    const std::vector<size_t>& htab = hdr.hashtab;
    size_t i, hsize = htab.size();
    for( i = 0; i < hsize; i++ )
    {
        size_t nidx = htab[i];
        if( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            return;
        }
    }
}

SparseMatConstIterator& SparseMatConstIterator::operator ++()
{
    if( !ptr || !m || !m->hdr )
        return *this;
    SparseMat::Hdr& hdr = *m->hdr;
    size_t next = ((const SparseMat::Node*)(ptr - hdr.valueOffset))->next;
    if( next )
    {
        ptr = &hdr.pool[next] + hdr.valueOffset;
        return *this;
    }
    size_t i = hashidx + 1, sz = hdr.hashtab.size();
    for( ; i < sz; i++ )
    {
        size_t nidx = hdr.hashtab[i];
        if( nidx )
        {
            hashidx = i;
            ptr = &hdr.pool[nidx] + hdr.valueOffset;
            return *this;
        }
    }
    hashidx = sz;
    ptr = 0;
    return *this;
}


double norm( const SparseMat& src, int normType )
{
    CC_INSTRUMENT_REGION()

    SparseMatConstIterator it = src.begin();

    size_t i, N = src.nzcount();
    normType &= NORM_TYPE_MASK;
    int type = src CC_MAT_TYPE();
    double result = 0;

    CC_Assert( normType == NORM_INF || normType == NORM_L1 || normType == NORM_L2 );

    if( type == CC_32F )
    {
        if( normType == NORM_INF )
            for( i = 0; i < N; i++, ++it )
                result = MAX(result, std::abs((double)it.value<float>()));
        else if( normType == NORM_L1 )
            for( i = 0; i < N; i++, ++it )
                result += std::abs(it.value<float>());
        else
            for( i = 0; i < N; i++, ++it )
            {
                double v = it.value<float>();
                result += v*v;
            }
    }
    else if( type == CC_64F )
    {
        if( normType == NORM_INF )
            for( i = 0; i < N; i++, ++it )
                result = MAX(result, std::abs(it.value<double>()));
        else if( normType == NORM_L1 )
            for( i = 0; i < N; i++, ++it )
                result += std::abs(it.value<double>());
        else
            for( i = 0; i < N; i++, ++it )
            {
                double v = it.value<double>();
                result += v*v;
            }
    }
    else
        CC_Error( CC_StsUnsupportedFormat, "Only 32f and 64f are supported" );

    if( normType == NORM_L2 )
        result = sqrt(result);
    return result;
}

void minMaxLoc( const SparseMat& src, double* _minval, double* _maxval, int* _minidx, int* _maxidx )
{
    CC_INSTRUMENT_REGION()

    SparseMatConstIterator it = src.begin();
    size_t i, N = src.nzcount(), d = src.hdr ? src.hdr->dims : 0;
    int type = src CC_MAT_TYPE();
    const int *minidx = 0, *maxidx = 0;

    if( type == CC_32F )
    {
        float minval = FLT_MAX, maxval = -FLT_MAX;
        for( i = 0; i < N; i++, ++it )
        {
            float v = it.value<float>();
            if( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            if( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        if( _minval )
            *_minval = minval;
        if( _maxval )
            *_maxval = maxval;
    }
    else if( type == CC_64F )
    {
        double minval = DBL_MAX, maxval = -DBL_MAX;
        for( i = 0; i < N; i++, ++it )
        {
            double v = it.value<double>();
            if( v < minval )
            {
                minval = v;
                minidx = it.node()->idx;
            }
            if( v > maxval )
            {
                maxval = v;
                maxidx = it.node()->idx;
            }
        }
        if( _minval )
            *_minval = minval;
        if( _maxval )
            *_maxval = maxval;
    }
    else
        CC_Error( CC_StsUnsupportedFormat, "Only 32f and 64f are supported" );

    if( _minidx )
        for( i = 0; i < d; i++ )
            _minidx[i] = minidx[i];
    if( _maxidx )
        for( i = 0; i < d; i++ )
            _maxidx[i] = maxidx[i];
}


void normalize( const SparseMat& src, SparseMat& dst, double a, int norm_type )
{
    CC_INSTRUMENT_REGION()

    double scale = 1;
    if( norm_type == CC_L2 || norm_type == CC_L1 || norm_type == CC_C )
    {
        scale = norm( src, norm_type );
        scale = scale > DBL_EPSILON ? a/scale : 0.;
    }
    else
        CC_Error( CC_StsBadArg, "Unknown/unsupported norm type" );

    src.convertTo( dst, -1, scale );
}

////////////////////// RotatedRect //////////////////////

RotatedRect::RotatedRect(const CPoint2f& _point1, const CPoint2f& _point2, const CPoint2f& _point3)
{
    CPoint2f _center = 0.5f * (_point1 + _point3);
    Vec2f vecs[2];
    vecs[0] = Vec2f(_point1 - _point2);
    vecs[1] = Vec2f(_point2 - _point3);
    // check that given sides are perpendicular
    CC_Assert( abs(vecs[0].dot(vecs[1])) / (norm(vecs[0]) * norm(vecs[1])) <= FLT_EPSILON );

    // wd_i stores which vector (0,1) or (1,2) will make the width
    // One of them will definitely have slope within -1 to 1
    int wd_i = 0;
    if( abs(vecs[1][1]) < abs(vecs[1][0]) ) wd_i = 1;
    int ht_i = (wd_i + 1) % 2;

    float _angle = atan(vecs[wd_i][1] / vecs[wd_i][0]) * 180.0f / (float) CC_PI;
    float _width = (float) norm(vecs[wd_i]);
    float _height = (float) norm(vecs[ht_i]);

    center = _center;
    size = Size2f(_width, _height);
    angle = _angle;
}

void RotatedRect::points(CPoint2f pt[]) const
{
    double _angle = angle*CC_PI/180.;
    float b = (float)cos(_angle)*0.5f;
    float a = (float)sin(_angle)*0.5f;

    pt[0].x = center.x - a*size.height - b*size.width;
    pt[0].y = center.y + b*size.height - a*size.width;
    pt[1].x = center.x + a*size.height - b*size.width;
    pt[1].y = center.y - b*size.height - a*size.width;
    pt[2].x = 2*center.x - pt[0].x;
    pt[2].y = 2*center.y - pt[0].y;
    pt[3].x = 2*center.x - pt[1].x;
    pt[3].y = 2*center.y - pt[1].y;
}

Rect RotatedRect::boundingRect() const
{
    CPoint2f pt[4];
    points(pt);
    Rect r(cFloor(MIN(MIN(MIN(pt[0].x, pt[1].x), pt[2].x), pt[3].x)),
           cFloor(MIN(MIN(MIN(pt[0].y, pt[1].y), pt[2].y), pt[3].y)),
           cCeil(MAX(MAX(MAX(pt[0].x, pt[1].x), pt[2].x), pt[3].x)),
           cCeil(MAX(MAX(MAX(pt[0].y, pt[1].y), pt[2].y), pt[3].y)));
    r.width -= r.x - 1;
    r.height -= r.y - 1;
    return r;
}


Rect_<float> RotatedRect::boundingRect2f() const
{
    CPoint2f pt[4];
    points(pt);
    Rect_<float> r(Point_<float>(min(min(min(pt[0].x, pt[1].x), pt[2].x), pt[3].x), min(min(min(pt[0].y, pt[1].y), pt[2].y), pt[3].y)),
                   Point_<float>(max(max(max(pt[0].x, pt[1].x), pt[2].x), pt[3].x), max(max(max(pt[0].y, pt[1].y), pt[2].y), pt[3].y)));
    return r;
}



// glue

CvMatND::CvMatND(const img_t& m)
{
    cvInitMatNDHeader(this, m.dims, m.size, m CC_MAT_TYPE(), m.data );
    int i, d = m.dims;
    for( i = 0; i < d; i++ )
        dim[i]->step = (int)m->step[i];
    type |= m.flags & img_t::CONTINUOUS_FLAG;
}

_IplImage::_IplImage(const img_t& m)
{
    CC_Assert( m.dims <= 2 );
    cvInitImageHeader(this, m.size(), cvIplDepth(m.flags), m CC_MAT_CN());
    cvSetData(this, m.data, (int)m->step[0]);
}

CvSparseMat* cvCreateSparseMat(const SparseMat& sm)
{
    if( !sm.hdr )
        return 0;

    CvSparseMat* m = cvCreateSparseMat(sm.hdr->dims, sm.hdr->size, sm CC_MAT_TYPE());

    SparseMatConstIterator from = sm.begin();
    size_t i, N = sm.nzcount(), esz = sm CC_TYPE_SIZE();

    for( i = 0; i < N; i++, ++from )
    {
        const SparseMat::Node* n = from.node();
        uchar* to = cvPtrND(m, n->idx, 0, -2, 0);
        copyElem(from.ptr, to, esz);
    }
    return m;
}

void CvSparseMat::copyToSparseMat(SparseMat& m) const
{
    m cvSetMat( dims, &size[0], type );

    CvSparseMatIterator it;
    CvSparseNode* n = cvInitSparseMatIterator(this, &it);
    size_t esz = m CC_TYPE_SIZE();

    for( ; n != 0; n = cvGetNextSparseNode(&it) )
    {
        const int* idx = CC_NODE_IDX(this, n);
        uchar* to = m.newNode(idx, m.hash(idx));
        copyElem((const uchar*)CC_NODE_VAL(this, n), to, esz);
    }
}




#endif




/****************************************************************************************\
*                           [scaled] Identity matrix initialization                      *
\****************************************************************************************/

CC_IMPL img_t*
cvSetIdentity(img_t* mat, CScalar value CC_DEFAULT(cRealScalar(1)))
{
    CC_FUNCNAME("cvSetIdentity");

    __BEGIN__;

    TypeId type;
    CSize size;
    int i, k, len, step;
    int pix_size;
    uchar* data = 0;
    double buf[4];
    int cn = CC_MAT_CN(mat);

    size = cvGetMatSize(mat);
    len = CC_IMIN(size.width, size.height);

    type = CC_MAT_DEPTH(mat);
    pix_size = CC_TYPE_SIZE(type);
    size.width *= pix_size;

    if(CC_IS_MAT_CONT(mat))
    {
        size.width *= size.height;
        size.height = 1;
    }

    data = mat->tt.data;
    step = mat->step;
    if(step == 0)
        step = CC_STUB_STEP;
    IPPI_CALL(icvSetZero_8u_C1R(data, step, size));
    step += pix_size;

    if(type == CC_32F && cn==1)
    {
        float val = (float)value.val[0];
        float* _data = (float*)data;
        step /= sizeof(_data[0]);
        len *= step;

        for(i = 0; i < len; i += step)
            _data[i] = val;
    }
    else if(type == CC_64F && 1==cn)
    {
        double val = value.val[0];
        double* _data = (double*)data;
        step /= sizeof(_data[0]);
        len *= step;

        for(i = 0; i < len; i += step)
            _data[i] = val;
    }
    else
    {
        uchar* val_ptr = (uchar*)buf;
        cvScalarToRawData(&value, buf, type, cn, 0);
        len *= step;

        for(i = 0; i < len; i += step)
            for(k = 0; k < pix_size; k++)
                data[i+k] = val_ptr[k];
    }

    __END__;
    return mat;
}

CC_IMPL img_t*
cvSetIdentityTo(img_t* mat, int row, int col, TypeId type, int cn, CScalar value CC_DEFAULT(cRealScalar(1))) {
    cvSetMat(mat, row, col, type, cn, NULL, CC_AUTO_STEP);
    return cvSetIdentity(mat, value);
}
CC_IMPL img_t*
cvSetIdentityToReal(img_t* mat, int row, int col, TypeId type, double value CC_DEFAULT(1) ) {
  return cvSetIdentityTo(mat, row, col, type, 1, cRealScalar(value));
}
#define cvSetIdentity1(mat, value)  cvSetIdentity(mat, cRealScalar(value))
CC_IMPL
img_t* cvEye(img_t* mat) {
    cvSetIdentity1(mat, 1);
    return mat;
}

static img_t* cvSetEye(img_t* mat, int rows, int cols, TypeId type, double value CC_DEFAULT(1)) {
  cvSetMat(mat, rows, cols, type, 1, NULL, CC_AUTO_STEP);
  return cvSetIdentity(mat, cRealScalar(value));
}

CC_IMPL void
cvSetDiag(img_t* dst, const img_t* src) {
    CC_FUNCNAME("cvSetIdentity");

    __BEGIN__;
    int cn = CC_MAT_CN(src);
    int len = src->h*src->w;
    if (dst!=src) {
      cvSetTo(dst, len, len, src->tid, cn);
      cvZeroMat(dst);
    }
    {
      img_t _dst[1];
    img_t _src[1];
    cvGetDiag(dst, _dst, 0);
    cvReshape(src, _src, cn, len);
    cvCopy(_src, _dst);
    }

    __END__;
}

/****************************************************************************************\
*                                    Trace of the matrix                                 *
\****************************************************************************************/

CC_IMPL CScalar
cvTrace(const img_t* mat)
{
    CScalar sum = {{0,0,0,0}};
    
    CC_FUNCNAME("cvTrace");

    __BEGIN__;

    img_t diagstub[1] = {0};
    int cn = CC_MAT_CN(mat);
    TypeId type = CC_MAT_DEPTH(mat);
    int size = MIN(mat->rows,mat->cols);
    uchar* data = mat->tt.data;

    if(type == CC_32F && cn==1)
    {
        int step = mat->step + sizeof(float);

        for(; size--; data += step)
            sum.val[0] += *(float*)data;
        EXIT;
    }
    
    if(type == CC_64F && 1==cn)
    {
        int step = mat->step + sizeof(double);

        for(; size--; data += step)
            sum.val[0] += *(double*)data;
        EXIT;
    }

    CC_CALL(mat = cvGetDiag(mat, diagstub, 0));
    CC_CALL(sum = cvSum(mat));

    __END__;

    return sum;
}


/****************************************************************************************\
*                              LU decomposition/back substitution                        *
\****************************************************************************************/

//#define arrtype float
//#define temptype double

typedef  CStatus (C_STDCALL * CvLUDecompFunc)(double* A, int stepA, CSize sizeA,
                                                 void* B, int stepB, CSize sizeB,
                                                 double* det);

typedef  CStatus (C_STDCALL * CvLUBackFunc)(double* A, int stepA, CSize sizeA,
                                               void* B, int stepB, CSize sizeB);


#define ICC_DEF_LU_DECOMP_FUNC(flavor, arrtype)                               \
static CStatus C_STDCALL                                                      \
icvLUDecomp_##flavor(double* A, int stepA, CSize sizeA,                       \
                      arrtype* B, int stepB, CSize sizeB, double* _det)       \
{                                                                               \
    int n = sizeA.width;                                                        \
    int m = 0, i;                                                               \
    double det = 1, alpha;                                                      \
                                                                                \
    assert(sizeA.width == sizeA.height);                                      \
                                                                                \
    if(B)                                                                     \
    {                                                                           \
        assert(sizeA.height == sizeB.height);                                 \
        m = sizeB.width;                                                        \
    }                                                                           \
    stepA /= sizeof(A[0]);                                                      \
    stepB /= sizeof(B[0]);                                                      \
                                                                                \
    for(i = 0; i < n; i++, A += stepA, B += stepB)                            \
    {                                                                           \
        int j, k = i;                                                           \
        double* tA = A;                                                         \
        arrtype* tB = 0;                                                        \
        double kval = fabs(A[i]), tval;                                         \
                                                                                \
        /* find the pivot element */                                            \
        for(j = i + 1; j < n; j++)                                            \
        {                                                                       \
            tA += stepA;                                                        \
            tval = fabs(tA[i]);                                                 \
                                                                                \
            if(tval > kval)                                                   \
            {                                                                   \
                kval = tval;                                                    \
                k = j;                                                          \
            }                                                                   \
        }                                                                       \
                                                                                \
        if(kval == 0)                                                         \
        {                                                                       \
            det = 0;                                                            \
            break;                                                              \
        }                                                                       \
                                                                                \
        /* swap rows */                                                         \
        if(k != i)                                                            \
        {                                                                       \
            tA = A + stepA*(k - i);                                             \
            det = -det;                                                         \
                                                                                \
            for(j = i; j < n; j++)                                            \
            {                                                                   \
                double t;                                                       \
                CC_SWAP(A[j], tA[j], t);                                      \
            }                                                                   \
                                                                                \
            if(m > 0)                                                         \
            {                                                                   \
                tB = B + stepB*(k - i);                                         \
                                                                                \
                for(j = 0; j < m; j++)                                        \
                {                                                               \
                    arrtype t = B[j];                                           \
                    CC_SWAP(B[j], tB[j], t);                                  \
                }                                                               \
            }                                                                   \
        }                                                                       \
                                                                                \
        tval = 1./A[i];                                                         \
        det *= A[i];                                                            \
        tA = A;                                                                 \
        tB = B;                                                                 \
        A[i] = tval; /* to replace division with multiplication in LUBack */    \
                                                                                \
        /* update matrix and the right side of the system */                    \
        for(j = i + 1; j < n; j++)                                            \
        {                                                                       \
            tA += stepA;                                                        \
            tB += stepB;                                                        \
            alpha = -tA[i]*tval;                                                \
                                                                                \
            for(k = i + 1; k < n; k++)                                        \
                tA[k] = tA[k] + alpha*A[k];                                     \
                                                                                \
            if(m > 0)                                                         \
                for(k = 0; k < m; k++)                                        \
                    tB[k] = (arrtype)(tB[k] + alpha*B[k]);                      \
        }                                                                       \
    }                                                                           \
                                                                                \
    if(_det)                                                                  \
        *_det = det;                                                            \
                                                                                \
    return CC_OK;                                                               \
}


ICC_DEF_LU_DECOMP_FUNC(32f, float)
//ICC_DEF_LU_DECOMP_FUNC(64f, double)



static CStatus C_STDCALL                                                   
icvLUDecomp_64f(double* A, int stepA, CSize sizeA,
                double* B, int stepB, CSize sizeB, double* _det)
{
  typedef double arrtype;
    int n = sizeA.width;                                                       
    int m = 0, i;                                                              
    double det = 1;                                                            
                                                                               
    assert(sizeA.width == sizeA.height);                                     
                                                                               
    if(B)                                                                    
    {                                                                          
        assert(sizeA.height == sizeB.height);                                
        m = sizeB.width;                                                       
    }                                                                          
    stepA /= sizeof(A[0]);                                                     
    stepB /= sizeof(B[0]);                                                     
                                                                               
    for(i = 0; i < n; i++, A += stepA, B += stepB)                           
    {                                                                          
        int j, k = i;                                                          
        double* tA = A;                                                        
        arrtype* tB = 0;                                                       
        double kval = fabs(A[i]), tval, alpha;                                        
                                                                               
        /* find the pivot element */                                           
        for(j = i + 1; j < n; j++)                                           
        {                                                                      
            tA += stepA;                                                       
            tval = fabs(tA[i]);                                                
                                                                               
            if(tval > kval)                                                  
            {                                                                  
                kval = tval;                                                   
                k = j;                                                         
            }                                                                  
        }                                                                      
                                                                               
        if(kval == 0)                                                        
        {                                                                      
            det = 0;                                                           
            break;                                                             
        }                                                                      
                                                                               
        /* swap rows */                                                        
        if(k != i)                                                           
        {                                                                      
            tA = A + stepA*(k - i);                                            
            det = -det;                                                        
                                                                               
            for(j = i; j < n; j++)                                           
            {                                                                  
                double t;                                                      
                CC_SWAP(A[j], tA[j], t);                                     
            }                                                                  
                                                                               
            if(m > 0)                                                        
            {                                                                  
                tB = B + stepB*(k - i);                                        
                                                                               
                for(j = 0; j < m; j++)                                       
                {                                                              
                    arrtype t = B[j];                                          
                    CC_SWAP(B[j], tB[j], t);                                 
                }                                                              
            }                                                                  
        }                                                                      
                                                                               
        tval = 1./A[i];                                                        
        det *= A[i];                                                           
        tA = A;                                                                
        tB = B;                                                                
        A[i] = tval; /* to replace division with multiplication in LUBack */   
                                                                               
        /* update matrix and the right side of the system */                   
        for(j = i + 1; j < n; j++)                                           
        {                                                                      
            tA += stepA;                                                       
            tB += stepB;                                                       
            alpha = -tA[i]*tval;                                        
                                                                               
            for(k = i + 1; k < n; k++)                                       
                tA[k] = tA[k] + alpha*A[k];                                    
                                                                               
            if(m > 0)                                                        
                for(k = 0; k < m; k++)                                       
                    tB[k] = (arrtype)(tB[k] + alpha*B[k]);                     
        }                                                                      
    }                                                                          
                                                                               
    if(_det)                                                                 
        *_det = det;                                                           
                                                                               
    return CC_OK;                                                              
}


#define ICC_DEF_LU_BACK_FUNC(flavor, arrtype)                                 \
static CStatus C_STDCALL                                                      \
icvLUBack_##flavor(double* A, int stepA, CSize sizeA,                         \
                    arrtype* B, int stepB, CSize sizeB)                       \
{                                                                               \
    int n = sizeA.width;                                                        \
    int m = sizeB.width, i;                                                     \
                                                                                \
    assert(m > 0 && sizeA.width == sizeA.height &&                             \
            sizeA.height == sizeB.height);                                     \
    stepA /= sizeof(A[0]);                                                      \
    stepB /= sizeof(B[0]);                                                      \
                                                                                \
    A += stepA*(n - 1);                                                         \
    B += stepB*(n - 1);                                                         \
                                                                                \
    for(i = n - 1; i >= 0; i--, A -= stepA)                                   \
    {                                                                           \
        int j, k;                                                               \
        for(j = 0; j < m; j++)                                                \
        {                                                                       \
            arrtype* tB = B + j;                                                \
            double x = 0;                                                       \
                                                                                \
            for(k = n - 1; k > i; k--, tB -= stepB)                           \
                x += A[k]*tB[0];                                                \
                                                                                \
            tB[0] = (arrtype)((tB[0] - x)*A[i]);                                \
        }                                                                       \
    }                                                                           \
                                                                                \
    return CC_OK;                                                               \
}


ICC_DEF_LU_BACK_FUNC(32f, float)
//ICC_DEF_LU_BACK_FUNC(64f, double)

static CStatus C_STDCALL                                                
  icvLUBack_64f(double* A, int stepA, CSize sizeA,                 
  double* B, int stepB, CSize sizeB) 
{
  typedef double arrtype;
  int n = sizeA.width;                                                    
    int m = sizeB.width, i;
                                                                          
    assert(m > 0 && sizeA.width == sizeA.height &&                       
            sizeA.height == sizeB.height);                               
    stepA /= sizeof(A[0]);                                                
    stepB /= sizeof(B[0]);                                                
                                                                          
    A += stepA*(n - 1);                                                   
    B += stepB*(n - 1);                                                   
                                                                          
    for(i = n - 1; i >= 0; i--, A -= stepA)                             
    {                                                                     
        int j, k;                                                         
        for(j = 0; j < m; j++)                                          
        {                                                                 
            arrtype* tB = B + j;                                          
            double x = 0;                                                 
                                                                          
            for(k = n - 1; k > i; k--, tB -= stepB)                     
                x += A[k]*tB[0];                                          
                                                                          
            tB[0] = (arrtype)((tB[0] - x)*A[i]);                          
        }                                                                 
    }                                                                     
                                                                          
    return CC_OK; 
}


static CvFuncTable lu_decomp_tab, lu_back_tab;
static int lu_inittab = 0;

static void icvInitLUTable(CvFuncTable  decomp_tab,
                            CvFuncTable  back_tab)
{
    decomp_tab[0] = (void*)icvLUDecomp_32f;
    decomp_tab[1] = (void*)icvLUDecomp_64f;
    back_tab[0] = (void*)icvLUBack_32f;
    back_tab[1] = (void*)icvLUBack_64f;
}



/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#define CC_DET2(m)   (m(0,0)*m(1,1) - m(0,1)*m(1,0))
#define CC_DET3(m)   (m(0,0)*(m(1,1)*m(2,2) - m(1,2)*m(2,1)) -  \
                   m(0,1)*(m(1,0)*m(2,2) - m(1,2)*m(2,0)) +  \
                   m(0,2)*(m(1,0)*m(2,1) - m(1,1)*m(2,0)))

CC_IMPL double
cvDet(const img_t* mat)
{
    double result = 0;
    uchar* buffer = 0;
    int local_alloc = 0;
    
    CC_FUNCNAME("cvDet");

    __BEGIN__;

    int cn = CC_MAT_CN(mat);
    TypeId type = CC_MAT_DEPTH(mat);

    if(mat->width != mat->height)
        CC_ERROR(CC_StsBadSize, "The matrix must be square");

    #define Mf(y, x) ((float*)(m + y*step))[x]
    #define Md(y, x) ((double*)(m + y*step))[x]

    if(mat->width == 2)
    {
        uchar* m = mat->tt.data;
        int step = mat->step;

        if(type == CC_32F && cn==1)
        {
            result = CC_DET2(Mf);
        }
        else if(type == CC_64F && 1==cn)
        {
            result = CC_DET2(Md);
        }
        else
        {
            CC_ERROR(CC_StsUnsupportedFormat, "");
        }
    }
    else if(mat->width == 3)
    {
        uchar* m = mat->tt.data;
        int step = mat->step;
        
        if(type == CC_32F && cn==1)
        {
            result = CC_DET3(Mf);
        }
        else if(type == CC_64F && 1==cn)
        {
            result = CC_DET3(Md);
        }
        else
        {
            CC_ERROR(CC_StsUnsupportedFormat, "");
        }
    }
    else if(mat->width == 1)
    {
        if(type == CC_32F && cn==1)
        {
            result = mat->tt.fl[0];
        }
        else if(type == CC_64F && 1==cn)
        {
            result = mat->tt.db[0];
        }
        else
        {
            CC_ERROR(CC_StsUnsupportedFormat, "");
        }
    }
    else
    {
        CvLUDecompFunc decomp_func;
        CSize size = cvGetMatSize(mat);
        const TypeId worktype = CC_64F;
        int buf_size = size.width*size.height*CC_TYPE_SIZE(worktype);
        img_t tmat[1];

        if(!lu_inittab)
        {
            icvInitLUTable(lu_decomp_tab, lu_back_tab);
            lu_inittab = 1;
        }

        if(CC_MAT_CN(mat) != 1 || type < CC_32F)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        if(size.width <= CC_MAX_LOCAL_MAT_SIZE)
        {
            buffer = (uchar*)cvStackAlloc(buf_size);
            local_alloc = 1;
        }
        else
        {
            CC_CALL(buffer = (uchar*)cAlloc(buf_size));
        }

        cvInitMatHeader(tmat, size.height, size.width, worktype, 1, buffer, CC_AUTO_STEP);
        if(type == worktype)
        {
        	CC_CALL(cvCopy(mat, tmat));
        }
        else
            CC_CALL(cvConvert(mat, tmat));

        decomp_func = (CvLUDecompFunc)(lu_decomp_tab[(worktype)-CC_32F]);
        assert(decomp_func);

        IPPI_CALL(decomp_func(tmat->tt.db, tmat->step, size, 0, 0, size, &result));
    }

    #undef Mf
    #undef Md

    /*icvCheckVector_64f(&result, 1);*/

    __END__;

    if(buffer && !local_alloc)
        cFree(&buffer);

    return result;
}



/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of the matrix                     *
\****************************************************************************************/

#define Sf(y, x) ((float*)(srcdata + y*srcstep))[x]
#define Sd(y, x) ((double*)(srcdata + y*srcstep))[x]
#define Df(y, x) ((float*)(dstdata + y*dststep))[x]
#define Dd(y, x) ((double*)(dstdata + y*dststep))[x]

CC_IMPL double
cvInvert(const img_t* src, img_t* dst, int method CC_DEFAULT(CC_LU))
{
    int i = 0;
    img_t im[3] = {0};
    img_t* u = im+i++;
    img_t* v = im+i++;
    img_t* w = im+i++;

    uchar* buffer = 0;
    int local_alloc = 0;
    double result = 0;
    
    CC_FUNCNAME("cvInvert");

    __BEGIN__;

    int cn = CC_MAT_CN(src);
    TypeId type = CC_MAT_DEPTH(src);

    if(method == CC_SVD || method == CC_SVD_SYM)
    {
        int n = MIN(src->rows,src->cols);
        if(method == CC_SVD_SYM && src->rows != src->cols)
            CC_ERROR(CC_StsBadSize, "CC_SVD_SYM method is used for non-square matrix");

        cvCreateMat(u, n, src->rows, src->tid, cn);
        if(method != CC_SVD_SYM)
            cvCreateMat(v, n, src->cols, src->tid, cn);
        cvCreateMat(w, n, 1, src->tid, cn);
        cvSVD((img_t*)src, w, u, v, CC_SVD_U_T + CC_SVD_V_T);

        if(type == CC_32F && cn==1)
            result = w->tt.fl[0] >= FLT_EPSILON ?
                     w->tt.fl[w->rows-1]/w->tt.fl[0] : 0;
        else
            result = w->tt.db[0] >= FLT_EPSILON ?
                     w->tt.db[w->rows-1]/w->tt.db[0] : 0;

        CC_CALL(cvSVBkSb(w, u, v ? v : u, 0, dst, CC_SVD_U_T + CC_SVD_V_T));
        EXIT;
    }
    else if(method != CC_LU)
        CC_ERROR(CC_StsBadArg, "Unknown inversion method");

    if(!CC_ARE_TYPES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(src->width != src->height)
        CC_ERROR(CC_StsBadSize, "The matrix must be square");

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    if(1!=cn)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    if(type != CC_32F && type != CC_64F)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    if(src->width <= 3)
    {
        uchar* srcdata = src->tt.data;
        uchar* dstdata = dst->tt.data;
        int srcstep = src->step;
        int dststep = dst->step;

        if(src->width == 2)
        {
            if(type == CC_32F && cn==1)
            {
                double d = CC_DET2(Sf);
                if(d != 0.)
                {
                    double t0, t1;
                    result = d;
                    d = 1./d;
                    t0 = Sf(0,0)*d;
                    t1 = Sf(1,1)*d;
                    Df(1,1) = (float)t0;
                    Df(0,0) = (float)t1;
                    t0 = -Sf(0,1)*d;
                    t1 = -Sf(1,0)*d;
                    Df(0,1) = (float)t0;
                    Df(1,0) = (float)t1;
                }
            }
            else
            {
                double d = CC_DET2(Sd);
                if(d != 0.)
                {
                    double t0, t1;
                    result = d;
                    d = 1./d;
                    t0 = Sd(0,0)*d;
                    t1 = Sd(1,1)*d;
                    Dd(1,1) = t0;
                    Dd(0,0) = t1;
                    t0 = -Sd(0,1)*d;
                    t1 = -Sd(1,0)*d;
                    Dd(0,1) = t0;
                    Dd(1,0) = t1;
                }
            }
        }
        else if(src->width == 3)
        {
            if(type == CC_32F && cn==1)
            {
                double d = CC_DET3(Sf);
                if(d != 0.)
                {
                    float t[9];
                    result = d;
                    d = 1./d;

                    t[0] = (float)((Sf(1,1) * Sf(2,2) - Sf(1,2) * Sf(2,1)) * d);
                    t[1] = (float)((Sf(0,2) * Sf(2,1) - Sf(0,1) * Sf(2,2)) * d);
                    t[2] = (float)((Sf(0,1) * Sf(1,2) - Sf(0,2) * Sf(1,1)) * d);
                                  
                    t[3] = (float)((Sf(1,2) * Sf(2,0) - Sf(1,0) * Sf(2,2)) * d);
                    t[4] = (float)((Sf(0,0) * Sf(2,2) - Sf(0,2) * Sf(2,0)) * d);
                    t[5] = (float)((Sf(0,2) * Sf(1,0) - Sf(0,0) * Sf(1,2)) * d);
                                  
                    t[6] = (float)((Sf(1,0) * Sf(2,1) - Sf(1,1) * Sf(2,0)) * d);
                    t[7] = (float)((Sf(0,1) * Sf(2,0) - Sf(0,0) * Sf(2,1)) * d);
                    t[8] = (float)((Sf(0,0) * Sf(1,1) - Sf(0,1) * Sf(1,0)) * d);

                    Df(0,0) = t[0]; Df(0,1) = t[1]; Df(0,2) = t[2];
                    Df(1,0) = t[3]; Df(1,1) = t[4]; Df(1,2) = t[5];
                    Df(2,0) = t[6]; Df(2,1) = t[7]; Df(2,2) = t[8];
                }
            }
            else
            {
                double d = CC_DET3(Sd);
                if(d != 0.)
                {
                    double t[9];
                    result = d;
                    d = 1./d;

                    t[0] = (Sd(1,1) * Sd(2,2) - Sd(1,2) * Sd(2,1)) * d;
                    t[1] = (Sd(0,2) * Sd(2,1) - Sd(0,1) * Sd(2,2)) * d;
                    t[2] = (Sd(0,1) * Sd(1,2) - Sd(0,2) * Sd(1,1)) * d;
                           
                    t[3] = (Sd(1,2) * Sd(2,0) - Sd(1,0) * Sd(2,2)) * d;
                    t[4] = (Sd(0,0) * Sd(2,2) - Sd(0,2) * Sd(2,0)) * d;
                    t[5] = (Sd(0,2) * Sd(1,0) - Sd(0,0) * Sd(1,2)) * d;
                           
                    t[6] = (Sd(1,0) * Sd(2,1) - Sd(1,1) * Sd(2,0)) * d;
                    t[7] = (Sd(0,1) * Sd(2,0) - Sd(0,0) * Sd(2,1)) * d;
                    t[8] = (Sd(0,0) * Sd(1,1) - Sd(0,1) * Sd(1,0)) * d;

                    Dd(0,0) = t[0]; Dd(0,1) = t[1]; Dd(0,2) = t[2];
                    Dd(1,0) = t[3]; Dd(1,1) = t[4]; Dd(1,2) = t[5];
                    Dd(2,0) = t[6]; Dd(2,1) = t[7]; Dd(2,2) = t[8];
                }
            }
        }
        else
        {
            assert(src->width == 1);

            if(type == CC_32F && cn==1)
            {
                double d = Sf(0,0);
                if(d != 0.)
                {
                    result = d;
                    Df(0,0) = (float)(1./d);
                }
            }
            else
            {
                double d = Sd(0,0);
                if(d != 0.)
                {
                    result = d;
                    Dd(0,0) = 1./d;
                }
            }
        }
    }
    else
    {
        CvLUDecompFunc decomp_func;
        CvLUBackFunc back_func;
        CSize size = cvGetMatSize(src);
        const TypeId worktype = CC_64F;
        int buf_size = size.width*size.height*CC_TYPE_SIZE(worktype);
        img_t tmat[1];

        if(!lu_inittab)
        {
            icvInitLUTable(lu_decomp_tab, lu_back_tab);
            lu_inittab = 1;
        }

        if(size.width <= CC_MAX_LOCAL_MAT_SIZE)
        {
            buffer = (uchar*)cvStackAlloc(buf_size);
            local_alloc = 1;
        }
        else
        {
            CC_CALL(buffer = (uchar*)cAlloc(buf_size));
        }

        cvInitMatHeader(tmat, size.height, size.width, worktype, 1, buffer, CC_AUTO_STEP);
        if(type == worktype)
        {
            CC_CALL(cvCopy(src, tmat));
        }
        else
            CC_CALL(cvConvert(src, tmat));
        cvSetIdentity1(dst, 1);

        decomp_func = (CvLUDecompFunc)(lu_decomp_tab[type-CC_32F]);
        back_func = (CvLUBackFunc)(lu_back_tab[type-CC_32F]);
        assert(decomp_func && back_func);
        
        IPPI_CALL(decomp_func(tmat->tt.db, tmat->step, size,
                                dst->tt.data, dst->step, size, &result));
        
        if(result != 0)
        {
            IPPI_CALL(back_func(tmat->tt.db, tmat->step, size,
                                  dst->tt.data, dst->step, size));
        }
    }

    if(!result)
        CC_CALL(cvZeroMat(dst));

    __END__;

    if(buffer && !local_alloc)
        cFree(&buffer);

    imfrees2(im);
    return result;
}

/****************************************************************************************\
*                               Linear system [least-squares] solution                   *
\****************************************************************************************/

CC_IMPL int
cvSolve(const img_t* A, const img_t* b, img_t* x, int method CC_DEFAULT(CC_LU))
{
    int i = 0;
    img_t im[3] = {0};
    img_t* u = im+i++;
    img_t* v = im+i++;
    img_t* w = im+i++;

    uchar* buffer = 0;
    int local_alloc = 0;
    int result = 1;

    CC_FUNCNAME("cvSolve");

    __BEGIN__;

    img_t *src = (img_t*)A;
    img_t *dst = (img_t*)x;
    img_t *src2 = (img_t*)b;
    TypeId type;
    int cn = CC_MAT_CN(A);

    if (x!=b && x!=A) {
      int n = MIN(A->rows,A->cols);
      cvSetMat(x, n, 1, A->tid, 1, NULL, CC_AUTO_STEP);
    }

    if(method == CC_SVD || method == CC_SVD_SYM)
    {
        int n = MIN(src->rows,src->cols);

        if(method == CC_SVD_SYM && src->rows != src->cols)
            CC_ERROR(CC_StsBadSize, "CC_SVD_SYM method is used for non-square matrix");

        cvCreateMat(u, n, src->rows, src->tid, 1);
        if(method != CC_SVD_SYM)
            cvCreateMat(v, n, src->cols, src->tid, 1);
        cvCreateMat(w, n, 1, src->tid, 1);
        CC_CALL(cvSVD(src, w, u, v, CC_SVD_U_T + CC_SVD_V_T));
        CC_CALL(cvSVBkSb(w, u, v ? v : u, src2, dst, CC_SVD_U_T + CC_SVD_V_T));
        EXIT;
    }
    else if(method != CC_LU)
        CC_ERROR(CC_StsBadArg, "Unknown inversion method");

    type = CC_MAT_DEPTH(src);

    if(!CC_ARE_TYPES_EQ(src, dst) || !CC_ARE_TYPES_EQ(src, src2))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(src->width != src->height)
        CC_ERROR(CC_StsBadSize, "The matrix must be square");

    if(!CC_ARE_SIZES_EQ(src2, dst) || src->width != src2->height)
        CC_ERROR(CC_StsUnmatchedSizes, "");

    if(cn != 1)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    if(type != CC_32F && type != CC_64F)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    // check case of a single equation and small matrix
    if(src->width <= 3 && src2->width == 1)
    {
        #define bf(y) ((float*)(bdata + y*src2step))[0]
        #define bd(y) ((double*)(bdata + y*src2step))[0]

        uchar* srcdata = src->tt.data;
        uchar* bdata = src2->tt.data;
        uchar* dstdata = dst->tt.data;
        int srcstep = src->step;
        int src2step = src2->step;
        int dststep = dst->step;

        if(src->width == 2)
        {
            if(type == CC_32F && cn==1)
            {
                double d = CC_DET2(Sf);
                if(d != 0.)
                {
                    float t;
                    d = 1./d;
                    t = (float)((bf(0)*Sf(1,1) - bf(1)*Sf(0,1))*d);
                    Df(1,0) = (float)((bf(1)*Sf(0,0) - bf(0)*Sf(1,0))*d);
                    Df(0,0) = t;
                }
                else
                    result = 0;
            }
            else
            {
                double d = CC_DET2(Sd);
                if(d != 0.)
                {
                    double t;
                    d = 1./d;
                    t = (bd(0)*Sd(1,1) - bd(1)*Sd(0,1))*d;
                    Dd(1,0) = (bd(1)*Sd(0,0) - bd(0)*Sd(1,0))*d;
                    Dd(0,0) = t;
                }
                else
                    result = 0;
            }
        }
        else if(src->width == 3)
        {
            if(type == CC_32F && cn==1)
            {
                double d = CC_DET3(Sf);
                if(d != 0.)
                {
                    float t[3];
                    d = 1./d;

                    t[0] = (float)(d*
                           (bf(0)*(Sf(1,1)*Sf(2,2) - Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*(bf(1)*Sf(2,2) - Sf(1,2)*bf(2)) +
                            Sf(0,2)*(bf(1)*Sf(2,1) - Sf(1,1)*bf(2))));

                    t[1] = (float)(d*
                           (Sf(0,0)*(bf(1)*Sf(2,2) - Sf(1,2)*bf(2)) -
                            bf(0)*(Sf(1,0)*Sf(2,2) - Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*(Sf(1,0)*bf(2) - bf(1)*Sf(2,0))));

                    t[2] = (float)(d*
                           (Sf(0,0)*(Sf(1,1)*bf(2) - bf(1)*Sf(2,1)) -
                            Sf(0,1)*(Sf(1,0)*bf(2) - bf(1)*Sf(2,0)) +
                            bf(0)*(Sf(1,0)*Sf(2,1) - Sf(1,1)*Sf(2,0))));

                    Df(0,0) = t[0];
                    Df(1,0) = t[1];
                    Df(2,0) = t[2];
                }
                else
                    result = 0;
            }
            else
            {
                double d = CC_DET3(Sd);
                if(d != 0.)
                {
                    double t[9];

                    d = 1./d;
                    
                    t[0] = ((Sd(1,1) * Sd(2,2) - Sd(1,2) * Sd(2,1))*bd(0) +
                            (Sd(0,2) * Sd(2,1) - Sd(0,1) * Sd(2,2))*bd(1) +
                            (Sd(0,1) * Sd(1,2) - Sd(0,2) * Sd(1,1))*bd(2))*d;

                    t[1] = ((Sd(1,2) * Sd(2,0) - Sd(1,0) * Sd(2,2))*bd(0) +
                            (Sd(0,0) * Sd(2,2) - Sd(0,2) * Sd(2,0))*bd(1) +
                            (Sd(0,2) * Sd(1,0) - Sd(0,0) * Sd(1,2))*bd(2))*d;

                    t[2] = ((Sd(1,0) * Sd(2,1) - Sd(1,1) * Sd(2,0))*bd(0) +
                            (Sd(0,1) * Sd(2,0) - Sd(0,0) * Sd(2,1))*bd(1) +
                            (Sd(0,0) * Sd(1,1) - Sd(0,1) * Sd(1,0))*bd(2))*d;

                    Dd(0,0) = t[0];
                    Dd(1,0) = t[1];
                    Dd(2,0) = t[2];
                }
                else
                    result = 0;
            }
        }
        else
        {
            assert(src->width == 1);

            if(type == CC_32F && cn==1)
            {
                double d = Sf(0,0);
                if(d != 0.)
                    Df(0,0) = (float)(bf(0)/d);
                else
                    result = 0;
            }
            else
            {
                double d = Sd(0,0);
                if(d != 0.)
                    Dd(0,0) = (bd(0)/d);
                else
                    result = 0;
            }
        }
    }
    else
    {
        CvLUDecompFunc decomp_func;
        CvLUBackFunc back_func;
        CSize size = cvGetMatSize(src);
        CSize dstsize = cvGetMatSize(dst);
        TypeId worktype = CC_64F;
        int buf_size = size.width*size.height*CC_TYPE_SIZE(worktype);
        double d = 0;
        img_t tmat[1];

        if(!lu_inittab)
        {
            icvInitLUTable(lu_decomp_tab, lu_back_tab);
            lu_inittab = 1;
        }

        if(size.width <= CC_MAX_LOCAL_MAT_SIZE)
        {
            buffer = (uchar*)cvStackAlloc(buf_size);
            local_alloc = 1;
        }
        else
        {
            CC_CALL(buffer = (uchar*)cAlloc(buf_size));
        }

        cvInitMatHeader(tmat, size.height, size.width, worktype, cn, buffer, CC_AUTO_STEP);
        if(type == worktype)
        {
            CC_CALL(cvCopy(src, tmat));
        }
        else
            CC_CALL(cvConvert(src, tmat));

        if(src2->tt.data != dst->tt.data)
        {
            CC_CALL(cvCopy(src2, dst));
        }

        decomp_func = (CvLUDecompFunc)(lu_decomp_tab[type-CC_32F]);
        back_func = (CvLUBackFunc)(lu_back_tab[type-CC_32F]);
        assert(decomp_func && back_func);

        IPPI_CALL(decomp_func(tmat->tt.db, tmat->step, size,
                                dst->tt.data, dst->step, dstsize, &d));

        if(d != 0)
        {
            IPPI_CALL(back_func(tmat->tt.db, tmat->step, size,
                                  dst->tt.data, dst->step, dstsize));
        }
        else
            result = 0;
    }

    if(!result)
        CC_CALL(cvZeroMat(dst));

    __END__;

    if(buffer && !local_alloc)
        cFree(&buffer);

    imfrees2(im);
    return result;
}



/****************************************************************************************\
*                               3D vector cross-product                                  *
\****************************************************************************************/

CC_IMPL void
cvCrossProduct(const img_t* srcA, const img_t* srcB, img_t* dst)
{
    CC_FUNCNAME("cvCrossProduct");

    __BEGIN__;

    TypeId type = CC_MAT_DEPTH(srcA);

    if(srcA->width*srcA->height*CC_MAT_CN(srcA) != 3)
        CC_ERROR(CC_StsBadArg, "All the input arrays must be continuous 3-vectors");

    if(!srcB || !dst)
        CC_ERROR(CC_StsNullPtr, "");

    if((srcA->tid & ~CC_MAT_CONT_FLAG) == (srcB->tid & ~CC_MAT_CONT_FLAG) &&
        (srcA->tid & ~CC_MAT_CONT_FLAG) == (dst->tid & ~CC_MAT_CONT_FLAG))
    {
        if(!srcB->tt.data || !dst->tt.data)
            CC_ERROR(CC_StsNullPtr, "");
    }
    else
    {
        if(!CC_ARE_TYPES_EQ(srcA, srcB) ||
            !CC_ARE_TYPES_EQ(srcB, dst))
            CC_ERROR(CC_StsUnmatchedFormats, "");
    }

    if(!CC_ARE_SIZES_EQ(srcA, srcB) || !CC_ARE_SIZES_EQ(srcB, dst))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    if(type == CC_32F)
    {
        float* dstdata = (float*)(dst->tt.data);
        const float* src1data = (float*)(srcA->tt.data);
        const float* src2data = (float*)(srcB->tt.data);

        if(CC_IS_MAT_CONT(srcA) && CC_IS_MAT_CONT(srcB) && CC_IS_MAT_CONT(dst))
        {
            dstdata[2] = src1data[0] * src2data[1] - src1data[1] * src2data[0];
            dstdata[0] = src1data[1] * src2data[2] - src1data[2] * src2data[1];
            dstdata[1] = src1data[2] * src2data[0] - src1data[0] * src2data[2];
        }
        else
        {
            int step1 = srcA->step ? srcA->step/sizeof(src1data[0]) : 1;
            int step2 = srcB->step ? srcB->step/sizeof(src1data[0]) : 1;
            int step = dst->step ? dst->step/sizeof(src1data[0]) : 1;

            dstdata[2*step] = src1data[0] * src2data[step2] - src1data[step1] * src2data[0];
            dstdata[0] = src1data[step1] * src2data[step2*2] - src1data[step1*2] * src2data[step2];
            dstdata[step] = src1data[step1*2] * src2data[0] - src1data[0] * src2data[step2*2];
        }
    }
    else if(type == CC_64F)
    {
        double* dstdata = (double*)(dst->tt.data);
        const double* src1data = (double*)(srcA->tt.data);
        const double* src2data = (double*)(srcB->tt.data);
        
        if(CC_IS_MAT_CONT(srcA) && CC_IS_MAT_CONT(srcB) && CC_IS_MAT_CONT(dst))
        {
            dstdata[2] = src1data[0] * src2data[1] - src1data[1] * src2data[0];
            dstdata[0] = src1data[1] * src2data[2] - src1data[2] * src2data[1];
            dstdata[1] = src1data[2] * src2data[0] - src1data[0] * src2data[2];
        }
        else
        {
            int step1 = srcA->step ? srcA->step/sizeof(src1data[0]) : 1;
            int step2 = srcB->step ? srcB->step/sizeof(src1data[0]) : 1;
            int step = dst->step ? dst->step/sizeof(src1data[0]) : 1;

            dstdata[2*step] = src1data[0] * src2data[step2] - src1data[step1] * src2data[0];
            dstdata[0] = src1data[step1] * src2data[step2*2] - src1data[step1*2] * src2data[step2];
            dstdata[step] = src1data[step1*2] * src2data[0] - src1data[0] * src2data[step2*2];
        }
    }
    else
    {
        CC_ERROR(CC_StsUnsupportedFormat, "");
    }

    __END__;
}


CC_IMPL void
cvCalcPCA(const img_t* data_arr, img_t* avg_arr, img_t* eigenvals, img_t* eigenvects, int flags)
{
    int i = 0;
    img_t im[10] = {0};
    img_t* tmp_avg = im+i++;
    img_t* tmp_avg_r = im+i++;
    img_t* tmp_cov = im+i++;
    img_t* tmp_evals = im+i++;
    img_t* tmp_evects = im+i++;
    img_t* tmp_evects2 = im+i++;
    img_t* tmp_data = im+i++;
    
    CC_FUNCNAME("cvCalcPCA");

    __BEGIN__;

    img_t *data = (img_t*)data_arr;
    img_t *avg = avg_arr;
    img_t *evals = eigenvals;
    img_t *evects = eigenvects;
    int covar_flags = CC_COVAR_SCALE;
    int len, in_count, count, out_count;

    if(CC_MAT_CN(data) != 1 || CC_MAT_CN(avg) != 1 ||
        CC_MAT_CN(evals) != 1 || CC_MAT_CN(evects) != 1)
        CC_ERROR(CC_StsUnsupportedFormat, "All the input and output arrays must be 1-channel");

    if(CC_MAT_TYPE(avg) < CC_32F || !CC_ARE_DEPTHS_EQ(avg, evals) ||
        !CC_ARE_DEPTHS_EQ(avg, evects))
        CC_ERROR(CC_StsUnsupportedFormat, "All the output arrays must have the same type, 32fC1 or 64fC1");

    if(flags & CC_PCA_DATA_AS_COL)
    {
        len = data->rows;
        in_count = data->cols;
        covar_flags |= CC_COVAR_COLS;

        if(avg->cols != 1 || avg->rows != len)
            CC_ERROR(CC_StsBadSize,
            "The mean (average) vector should be data->rows x 1 when CC_PCA_DATA_AS_COL is used");

        cvCreateMat(tmp_avg, len, 1, CC_64F, 1);
    }
    else
    {
        len = data->cols;
        in_count = data->rows;
        covar_flags |= CC_COVAR_ROWS;

        if(avg->rows != 1 || avg->cols != len)
            CC_ERROR(CC_StsBadSize,
            "The mean (average) vector should be 1 x data->cols when CC_PCA_DATA_AS_ROW is used");

        cvCreateMat(tmp_avg, 1, len, CC_64F, 1);
    }

    count = MIN(len, in_count);
    out_count = evals->cols + evals->rows - 1;
    
    if((evals->cols != 1 && evals->rows != 1) || out_count > count)
        CC_ERROR(CC_StsBadSize,
        "The array of eigenvalues must be 1d vector containing "
        "no more than min(data->rows,data->cols) elements");

    if(evects->cols != len || evects->rows != out_count)
        CC_ERROR(CC_StsBadSize,
        "The matrix of eigenvalues must have the same number of columns as the input vector length "
        "and the same number of rows as the number of eigenvalues");

    // "scrambled" way to compute PCA (when cols(A)>rows(A)):
    // B = A'A; B*x=b*x; C = AA'; C*y=c*y -> AA'*y=c*y -> A'A*(A'*y)=c*(A'*y) -> c = b, x=A'*y
    if(len <= in_count)
        covar_flags |= CC_COVAR_NORMAL;

    if(flags & CC_PCA_USE_AVG){
        covar_flags |= CC_COVAR_USE_AVG;
		CC_CALL(cvConvert(avg, tmp_avg));
	}

    cvCreateMat(tmp_cov, count, count, CC_64F, 1);
    cvCreateMat(tmp_evals, 1, count, CC_64F, 1);
    cvCreateMat(tmp_evects, count, count, CC_64F, 1);

    CC_CALL(cvCalcCovarMatrix(&data_arr, 0, tmp_cov, tmp_avg, covar_flags));
    CC_CALL(cvSVD(tmp_cov, tmp_evals, tmp_evects, 0, CC_SVD_MODIFY_A + CC_SVD_U_T));
    tmp_evects->rows = out_count;
    tmp_evals->cols = out_count;
    cvZeroMat(evects);
    cvZeroMat(evals);

    if(covar_flags & CC_COVAR_NORMAL)
    {
        CC_CALL(cvConvert(tmp_evects, evects));
    }
    else
    {
        // CC_PCA_DATA_AS_ROW: cols(A)>rows(A). x=A'*y -> x'=y'*A
        // CC_PCA_DATA_AS_COL: rows(A)>cols(A). x=A''*y -> x'=y'*A'
        int block_count = 0;

        cvCreateMat(tmp_data, count, count, CC_64F, 1);
        cvCreateMat(tmp_avg_r, count, count, CC_64F, 1);
        cvCreateMat(tmp_evects2, count, count, CC_64F, 1);

        for(i = 0; i < len; i += block_count)
        {
            img_t data_part, tdata_part, part, dst_part, avg_part, tmp_avg_part;
            int gemm_flags;

            block_count = MIN(count, len - i);

            if(flags & CC_PCA_DATA_AS_COL)
            {
                data_part = cvGetRows1(data, i, i + block_count, 1);
                tdata_part = cvGetRows1(tmp_data, 0, block_count, 1);
                avg_part = cvGetRows1(tmp_avg, i, i + block_count, 1);
                tmp_avg_part = cvGetRows1(tmp_avg_r, 0, block_count, 1);
                gemm_flags = CC_GEMM_B_T;
            }
            else
            {
                data_part = cvGetCols1(data, i, i + block_count);
                tdata_part = cvGetCols1(tmp_data, 0, block_count);
                avg_part = cvGetCols1(tmp_avg, i, i + block_count);
                tmp_avg_part = cvGetCols1(tmp_avg_r, 0, block_count);
                gemm_flags = 0;
            }

            part = cvGetCols1(tmp_evects2, 0, block_count);
            dst_part = cvGetCols1(evects, i, i + block_count);

            cvConvert(&data_part, &tdata_part);
            cvRepeat(&avg_part, &tmp_avg_part);
            cvSub(&tdata_part, &tmp_avg_part, &tdata_part, NULL);
            cvGEMM(tmp_evects, &tdata_part, 1, 0, 0, &part, gemm_flags);
            cvConvert(&part, &dst_part);
        }

        // normalize eigenvectors
        for(i = 0; i < count; i++)
        {
            img_t ei = cvGetRow1(evects, i);
            cvNormalize1(&ei, &ei);
        }
    }

    if(tmp_evals->rows != evals->rows)
        *tmp_evals = cvReshape1(tmp_evals, 1, evals->rows);
    cvConvert(tmp_evals, evals);
    cvConvert(tmp_avg, avg);

    __END__;

    imfrees2(im);
}


CC_IMPL void
cvProjectPCA(const img_t* data_arr, const img_t* avg_arr,
              const img_t* eigenvects, img_t* result_arr)
{
    uchar* buffer = 0;
    int local_alloc = 0;
    
    CC_FUNCNAME("cvProjectPCA");

    __BEGIN__;

    img_t *data = (img_t*)data_arr;
    img_t *avg = (img_t*)avg_arr;
    img_t evectstub, *evects = (img_t*)eigenvects;
    img_t *result = (img_t*)result_arr;
    img_t avg_repeated;
    int i, len, in_count;
    int gemm_flags, as_cols, convert_data;
    int block_count0, block_count, buf_size, elem_size;
    uchar* tmp_data_ptr;

    if(CC_MAT_CN(data) != 1 || CC_MAT_CN(avg) != 1)
        CC_ERROR(CC_StsUnsupportedFormat, "All the input and output arrays must be 1-channel");

    if(CC_MAT_TYPE(avg) != CC_32F && CC_MAT_TYPE(avg) != CC_64F ||
        !CC_ARE_TYPES_EQ(avg, evects) || !CC_ARE_TYPES_EQ(avg, result))
        CC_ERROR(CC_StsUnsupportedFormat,
        "All the input and output arrays (except for data) must have the same type, 32fC1 or 64fC1");

    if((avg->cols != 1 || avg->rows != data->rows) &&
        (avg->rows != 1 || avg->cols != data->cols))
        CC_ERROR(CC_StsBadSize,
        "The mean (average) vector should be either 1 x data->cols or data->rows x 1");

    if(avg->cols == 1)
    {
        len = data->rows;
        in_count = data->cols;

        gemm_flags = CC_GEMM_A_T + CC_GEMM_B_T;
        as_cols = 1;
    }
    else
    {
        len = data->cols;
        in_count = data->rows;

        gemm_flags = CC_GEMM_B_T;
        as_cols = 0;
    }

    if(evects->cols != len)
        CC_ERROR(CC_StsUnmatchedSizes,
        "Eigenvectors must be stored as rows and be of the same size as input vectors");

    if(result->cols > evects->rows)
        CC_ERROR(CC_StsOutOfRange,
        "The output matrix of coefficients must have the number of columns "
        "less than or equal to the number of eigenvectors (number of rows in eigenvectors matrix)");

    evectstub = cvGetRows1(evects, 0, result->cols, 1);
    evects = &evectstub;

    block_count0 = (1 << 16)/len;
    block_count0 = MAX(block_count0, 4);
    block_count0 = MIN(block_count0, in_count);
    elem_size = CC_TYPE_SIZE(avg->tid);
    convert_data = CC_MAT_TYPE(data) < CC_MAT_TYPE(avg);

    buf_size = block_count0*len*((block_count0 > 1) + 1)*elem_size;

    if(buf_size < CC_MAX_LOCAL_SIZE)
    {
        buffer = (uchar*)cvStackAlloc(buf_size);
        local_alloc = 1;
    }
    else
        CC_CALL(buffer = (uchar*)cAlloc(buf_size));

    tmp_data_ptr = buffer;
    if(block_count0 > 1)
    {
        cvMat(&avg_repeated, as_cols ? len : block_count0, as_cols ? block_count0 : len, avg->tid, 1, buffer, CC_AUTO_STEP);
        cvRepeat(avg, &avg_repeated);
        tmp_data_ptr += block_count0*len*elem_size;
    }
    else
        avg_repeated = *avg;

    for(i = 0; i < in_count; i += block_count)
    {
        img_t data_part, norm_data, avg_part, *src = &data_part, out_part;
        
        block_count = MIN(block_count0, in_count - i);
        if(as_cols)
        {
            data_part = cvGetCols1(data, i, i + block_count);
            avg_part = cvGetCols1(&avg_repeated, 0, block_count);
            cvMat(&norm_data, len, block_count, avg->tid, 1, tmp_data_ptr, CC_AUTO_STEP);
        }
        else
        {
            data_part = cvGetRows1(data, i, i + block_count, 1);
            avg_part = cvGetRows1(&avg_repeated, 0, block_count, 1);
            cvMat(&norm_data, block_count, len, avg->tid, 1, tmp_data_ptr, CC_AUTO_STEP);
        }

        if(convert_data)
        {
            cvConvert(src, &norm_data);
            src = &norm_data;
        }
        
        cvSub(src, &avg_part, &norm_data, NULL);

        out_part = cvGetRows1(result, i, i + block_count, 1);
        cvGEMM(&norm_data, evects, 1, 0, 0, &out_part, gemm_flags);
    }

    __END__;

    if(!local_alloc)
        cFree(&buffer);
}


CC_IMPL void
cvBackProjectPCA(const img_t* proj_arr, const img_t* avg_arr,
                  const img_t* eigenvects, img_t* result_arr)
{
    uchar* buffer = 0;
    int local_alloc = 0;
    
    CC_FUNCNAME("cvProjectPCA");

    __BEGIN__;

    img_t *data = (img_t*)proj_arr;
    img_t *avg = (img_t*)avg_arr;
    img_t evectstub, *evects = (img_t*)eigenvects;
    img_t *result = result_arr;
    img_t avg_repeated;
    int i, len, in_count, as_cols;
    int block_count0, block_count, buf_size, elem_size;

    if(CC_MAT_TYPE(avg) != CC_32F && CC_MAT_TYPE(avg) != CC_64F ||
        !CC_ARE_TYPES_EQ(avg, data) || !CC_ARE_TYPES_EQ(avg, evects) || !CC_ARE_TYPES_EQ(avg, result))
        CC_ERROR(CC_StsUnsupportedFormat,
        "All the input and output arrays must have the same type, 32fC1 or 64fC1");

    if((avg->cols != 1 || avg->rows != result->rows) &&
        (avg->rows != 1 || avg->cols != result->cols))
        CC_ERROR(CC_StsBadSize,
        "The mean (average) vector should be either 1 x result->cols or result->rows x 1");

    if(avg->cols == 1)
    {
        len = result->rows;
        in_count = result->cols;
        as_cols = 1;
    }
    else
    {
        len = result->cols;
        in_count = result->rows;
        as_cols = 0;
    }

    if(evects->cols != len)
        CC_ERROR(CC_StsUnmatchedSizes,
        "Eigenvectors must be stored as rows and be of the same size as the output vectors");

    if(data->cols > evects->rows)
        CC_ERROR(CC_StsOutOfRange,
        "The input matrix of coefficients must have the number of columns "
        "less than or equal to the number of eigenvectors (number of rows in eigenvectors matrix)");

    evectstub = cvGetRows1(evects, 0, data->cols, 1);
    evects = &evectstub;

    block_count0 = (1 << 16)/len;
    block_count0 = MAX(block_count0, 4);
    block_count0 = MIN(block_count0, in_count);
    elem_size = CC_TYPE_SIZE(avg->tid);

    buf_size = block_count0*len*(block_count0 > 1)*elem_size;

    if(buf_size < CC_MAX_LOCAL_SIZE)
    {
        buffer = (uchar*)cvStackAlloc(MAX(buf_size,16));
        local_alloc = 1;
    }
    else
        CC_CALL(buffer = (uchar*)cAlloc(buf_size));

    if(block_count0 > 1)
    {
        cvMat(&avg_repeated, as_cols ? len : block_count0,
                              as_cols ? block_count0 : len, avg->tid, 1, buffer, CC_AUTO_STEP);
        cvRepeat(avg, &avg_repeated);
    }
    else
        avg_repeated = *avg;

    for(i = 0; i < in_count; i += block_count)
    {
        img_t data_part, avg_part, out_part;
        
        block_count = MIN(block_count0, in_count - i);
        data_part = cvGetRows1(data, i, i + block_count, 1);

        if(as_cols)
        {
            out_part = cvGetCols1(result, i, i + block_count);
            avg_part = cvGetCols1(&avg_repeated, 0, block_count);
            cvGEMM(evects, &data_part, 1, &avg_part, 1, &out_part, CC_GEMM_A_T + CC_GEMM_B_T);
        }
        else
        {
            out_part = cvGetRows1(result, i, i + block_count, 1);
            avg_part = cvGetRows1(&avg_repeated, 0, block_count, 1);
            cvGEMM(&data_part, evects, 1, &avg_part, 1, &out_part, 0);
        }
    }

    __END__;

    if(!local_alloc)
        cFree(&buffer);
}
