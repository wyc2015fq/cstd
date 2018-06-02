#include "test_precomp.hpp"

using namespace cv;
using namespace std;

namespace cvtest
{

static img_t initDFTWave( int n, bool inv )
{
    int i;
    double angle = (inv ? 1 : -1)*CC_PI*2/n;
    CComplex64f wi, w1;
    img_t wave(1, n, CC_64FC2);
    CComplex64f* w = wave.ptr<CComplex64f>();

    w1.re = cos(angle);
    w1.im = sin(angle);
    w[0].re = wi.re = 1.;
    w[0].im = wi.im = 0.;

    for( i = 1; i < n; i++ )
    {
        double t = wi.re*w1.re - wi.im*w1.im;
        wi.im = wi.re*w1.im + wi.im*w1.re;
        wi.re = t;
        w[i] = wi;
    }

    return wave;
}


static void DFT_1D( const img_t& _src, img_t& _dst, int flags, const img_t& _wave=img_t())
{
    _dst cvSetMat(_cvGetSize(src), _src CC_MAT_TYPE());
    int i, j, k, n = _dst->cols + _dst->rows - 1;
    img_t wave = _wave;
    double scale = (flags & DFT_SCALE) ? 1./n : 1.;
    size_t esz = _src CC_ELEM_SIZE();
    size_t srcstep = esz, dststep = esz;
    const uchar* src0 = _src->tt.data;
    uchar* dst0 = _dst->tt.data;

    CC_Assert( _src->cols + _src->rows - 1 == n );

    if( wave.empty() )
        wave = initDFTWave( n, (flags & DFT_INVERSE) != 0 );

    const CComplex64f* w = wave.ptr<CComplex64f>();
    if( !_src CC_IS_CONT_MAT() )
        srcstep = _src->step;
    if( !_dst CC_IS_CONT_MAT() )
        dststep = _dst->step;

    if( _src CC_MAT_TYPE() == CC_32FC2 )
    {
        for( i = 0; i < n; i++ )
        {
            CComplex32f* dst = (CComplex32f*)(dst0 + i*dststep);
            CComplex64f sum(0,0);
            int delta = i;
            k = 0;

            for( j = 0; j < n; j++ )
            {
                const CComplex32f* src = (const CComplex32f*)(src0 + j*srcstep);
                sum.re += src->re*w[k].re - src->im*w[k].im;
                sum.im += src->re*w[k].im + src->im*w[k].re;
                k += delta;
                k -= (k >= n ? n : 0);
            }

            dst->re = (float)(sum.re*scale);
            dst->im = (float)(sum.im*scale);
        }
    }
    else if( _src CC_MAT_TYPE() == CC_64FC2 )
    {
        for( i = 0; i < n; i++ )
        {
            CComplex64f* dst = (CComplex64f*)(dst0 + i*dststep);
            CComplex64f sum(0,0);
            int delta = i;
            k = 0;

            for( j = 0; j < n; j++ )
            {
                const CComplex64f* src = (const CComplex64f*)(src0 + j*srcstep);
                sum.re += src->re*w[k].re - src->im*w[k].im;
                sum.im += src->re*w[k].im + src->im*w[k].re;
                k += delta;
                k -= (k >= n ? n : 0);
            }

            dst->re = sum.re*scale;
            dst->im = sum.im*scale;
        }
    }
    else
        CC_Error(CC_StsUnsupportedFormat, "");
}


static void DFT_2D( const img_t& src, img_t& dst, int flags )
{
    const int cn = 2;
    int i;
    dst cvSetMat(cvGetSize(src), src CC_MAT_TYPE());
    img_t tmp( src->cols, src->rows, src CC_MAT_TYPE());
    img_t wave = initDFTWave( dst->cols, (flags & DFT_INVERSE) != 0 );

    // 1. row-wise transform
    for( i = 0; i < dst->rows; i++ )
    {
        img_t srci = src.row(i) cvReshape(,cn, src->cols), dsti = tmp.col(i);
        DFT_1D(srci, dsti, flags, wave );
    }

    if( (flags & DFT_ROWS) == 0 )
    {
        if( dst->cols != dst->rows )
            wave = initDFTWave( dst->rows, (flags & DFT_INVERSE) != 0 );

        // 2. column-wise transform
        for( i = 0; i < dst->cols; i++ )
        {
            img_t srci = tmp.row(i) cvReshape(,cn, tmp->cols), dsti = dst.col(i);
            DFT_1D(srci, dsti, flags, wave );
        }
    }
    else
        cvtest::transpose(tmp, dst);
}


static img_t initDCTWave( int n, bool inv )
{
    int i, k;
    double angle = CC_PI*0.5/n;
    img_t wave(n, n, CC_64F);

    double scale = sqrt(1./n);
    for( k = 0; k < n; k++ )
        wave.at<double>(0, k) = scale;
    scale *= sqrt(2.);
    for( i = 1; i < n; i++ )
        for( k = 0; k < n; k++ )
            wave.at<double>(i, k) = scale*cos( angle*i*(2*k + 1) );

    if( inv )
        transpose( wave, wave );

    return wave;
}


static void DCT_1D( const img_t& _src, img_t& _dst, int flags, const img_t& _wave=img_t() )
{
    _dst cvSetMat( _cvGetSize(src), _src CC_MAT_TYPE() );
    int i, j, n = _dst->cols + _dst->rows - 1;
    img_t wave = _wave;
    int srcstep = 1, dststep = 1;
    double* w;

    CC_Assert( _src->cols + _src->rows - 1 == n);

    if( wave.empty() )
        wave = initDCTWave( n, (flags & DFT_INVERSE) != 0 );
    w = wave img_ptr(double, );

    if( !_src CC_IS_CONT_MAT() )
        srcstep = (int)(_src->step/_src CC_ELEM_SIZE());
    if( !_dst CC_IS_CONT_MAT() )
        dststep = (int)(_dst->step/_dst CC_ELEM_SIZE());

    if( _src CC_MAT_TYPE() == CC_32FC1 )
    {
        float *dst = _dst img_ptr(float, );

        for( i = 0; i < n; i++, dst += dststep )
        {
            const float* src = _src img_ptr(float, );
            double sum = 0;

            for( j = 0; j < n; j++, src += srcstep )
                sum += src[0]*w[j];
            w += n;
            dst[0] = (float)sum;
        }
    }
    else if( _src CC_MAT_TYPE() == CC_64FC1 )
    {
        double *dst = _dst img_ptr(double, );

        for( i = 0; i < n; i++, dst += dststep )
        {
            const double* src = _src img_ptr(double, );
            double sum = 0;

            for( j = 0; j < n; j++, src += srcstep )
                sum += src[0]*w[j];
            w += n;
            dst[0] = sum;
        }
    }
    else
        assert(0);
}


static void DCT_2D( const img_t& src, img_t& dst, int flags )
{
    const int cn = 1;
    int i;
    dst cvSetMat( cvGetSize(src), src CC_MAT_TYPE() );
    img_t tmp(dst->cols, dst->rows, dst CC_MAT_TYPE() );
    img_t wave = initDCTWave( dst->cols, (flags & DCT_INVERSE) != 0 );

    // 1. row-wise transform
    for( i = 0; i < dst->rows; i++ )
    {
        img_t srci = src.row(i) cvReshape(,cn, src->cols);
        img_t dsti = tmp.col(i);
        DCT_1D(srci, dsti, flags, wave);
    }

    if( (flags & DCT_ROWS) == 0 )
    {
        if( dst->cols != dst->rows )
            wave = initDCTWave( dst->rows, (flags & DCT_INVERSE) != 0 );

        // 2. column-wise transform
        for( i = 0; i < dst->cols; i++ )
        {
            img_t srci = tmp.row(i) cvReshape(,cn, tmp->cols);
            img_t dsti = dst.col(i);
            DCT_1D( srci, dsti, flags, wave );
        }
    }
    else
        cvtest::transpose( tmp, dst );
}


static void convertFromCCS( const img_t& _src0, const img_t& _src1, img_t& _dst, int flags )
{
    if( _dst->rows > 1 && (_dst->cols > 1 || (flags & DFT_ROWS)) )
    {
        int i, count = _dst->rows, len = _dst->cols;
        bool is2d = (flags & DFT_ROWS) == 0;
        img_t src0row, src1row, dstrow;
        for( i = 0; i < count; i++ )
        {
            int j = !is2d || i == 0 ? i : count - i;
            src0row = _src0.row(i);
            src1row = _src1.row(j);
            dstrow = _dst.row(i);
            convertFromCCS( src0row, src1row, dstrow, 0 );
        }

        if( is2d )
        {
            src0row = _src0.col(0);
            dstrow = _dst.col(0);
            convertFromCCS( src0row, src0row, dstrow, 0 );
            if( (len & 1) == 0 )
            {
                src0row = _src0.col(_src0->cols - 1);
                dstrow = _dst.col(len/2);
                convertFromCCS( src0row, src0row, dstrow, 0 );
            }
        }
    }
    else
    {
        int i, n = _dst->cols + _dst->rows - 1, n2 = (n+1) >> 1;
        int cn = _src0 CC_MAT_CN();
        int srcstep = cn, dststep = 1;

        if( !_dst CC_IS_CONT_MAT() )
            dststep = (int)(_dst->step/_dst CC_ELEM_SIZE());

        if( !_src0 CC_IS_CONT_MAT() )
            srcstep = (int)(_src0->step/_src0 CC_ELEM_SIZE1());

        if( _dst CC_MAT_DEPTH() == CC_32F )
        {
            CComplex32f* dst = _dst.ptr<CComplex32f>();
            const float* src0 = _src0 img_ptr(float, );
            const float* src1 = _src1 img_ptr(float, );
            int delta0, delta1;

            dst->re = src0[0];
            dst->im = 0;

            if( (n & 1) == 0 )
            {
                dst[n2*dststep].re = src0[(cn == 1 ? n-1 : n2)*srcstep];
                dst[n2*dststep].im = 0;
            }

            delta0 = srcstep;
            delta1 = delta0 + (cn == 1 ? srcstep : 1);
            if( cn == 1 )
                srcstep *= 2;

            for( i = 1; i < n2; i++, delta0 += srcstep, delta1 += srcstep )
            {
                float t0 = src0[delta0];
                float t1 = src0[delta1];

                dst[i*dststep].re = t0;
                dst[i*dststep].im = t1;

                t0 = src1[delta0];
                t1 = -src1[delta1];

                dst[(n-i)*dststep].re = t0;
                dst[(n-i)*dststep].im = t1;
            }
        }
        else
        {
            CComplex64f* dst = _dst.ptr<CComplex64f>();
            const double* src0 = _src0 img_ptr(double, );
            const double* src1 = _src1 img_ptr(double, );
            int delta0, delta1;

            dst->re = src0[0];
            dst->im = 0;

            if( (n & 1) == 0 )
            {
                dst[n2*dststep].re = src0[(cn == 1 ? n-1 : n2)*srcstep];
                dst[n2*dststep].im = 0;
            }

            delta0 = srcstep;
            delta1 = delta0 + (cn == 1 ? srcstep : 1);
            if( cn == 1 )
                srcstep *= 2;

            for( i = 1; i < n2; i++, delta0 += srcstep, delta1 += srcstep )
            {
                double t0 = src0[delta0];
                double t1 = src0[delta1];

                dst[i*dststep].re = t0;
                dst[i*dststep].im = t1;

                t0 = src1[delta0];
                t1 = -src1[delta1];

                dst[(n-i)*dststep].re = t0;
                dst[(n-i)*dststep].im = t1;
            }
        }
    }
}


static void fixCCS( img_t& mat, int cols, int flags )
{
    int i, rows = mat->rows;
    int rows2 = (flags & DFT_ROWS) ? rows : rows/2 + 1, cols2 = cols/2 + 1;

    CC_Assert( cols2 == mat->cols );

    if( mat CC_MAT_TYPE() == CC_32FC2 )
    {
        for( i = 0; i < rows2; i++ )
        {
            CComplex32f* row = mat.ptr<CComplex32f>(i);
            if( (flags & DFT_ROWS) || i == 0 || (i == rows2 - 1 && rows % 2 == 0) )
            {
                row[0].im = 0;
                if( cols % 2 == 0 )
                    row[cols2-1].im = 0;
            }
            else
            {
                CComplex32f* row2 = mat.ptr<CComplex32f>(rows-i);
                row2[0].re = row[0].re;
                row2[0].im = -row[0].im;

                if( cols % 2 == 0 )
                {
                    row2[cols2-1].re = row[cols2-1].re;
                    row2[cols2-1].im = -row[cols2-1].im;
                }
            }
        }
    }
    else if( mat CC_MAT_TYPE() == CC_64FC2 )
    {
        for( i = 0; i < rows2; i++ )
        {
            CComplex64f* row = mat.ptr<CComplex64f>(i);
            if( (flags & DFT_ROWS) || i == 0 || (i == rows2 - 1 && rows % 2 == 0) )
            {
                row[0].im = 0;
                if( cols % 2 == 0 )
                    row[cols2-1].im = 0;
            }
            else
            {
                CComplex64f* row2 = mat.ptr<CComplex64f>(rows-i);
                row2[0].re = row[0].re;
                row2[0].im = -row[0].im;

                if( cols % 2 == 0 )
                {
                    row2[cols2-1].re = row[cols2-1].re;
                    row2[cols2-1].im = -row[cols2-1].im;
                }
            }
        }
    }
}

static void mulComplex( const img_t& src1, const img_t& src2, img_t& dst, int flags )
{
    dst cvSetMat(src1->rows, src1->cols, src1 CC_MAT_TYPE());
    int i, j, depth = src1 CC_MAT_DEPTH(), cols = src1->cols*2;

    CC_Assert( src1.size == src2.size && src1 CC_MAT_TYPE() == src2 CC_MAT_TYPE() &&
              (src1 CC_MAT_TYPE() == CC_32FC2 || src1 CC_MAT_TYPE() == CC_64FC2) );

    const img_t* src1_ = &src1;
    img_t src1_tmp;
    if (dst.data == src1.data)
    {
        src1_tmp = src1.clone();
        src1_ = &src1_tmp;
    }
    const img_t* src2_ = &src2;
    img_t src2_tmp;
    if (dst.data == src2.data)
    {
        src2_tmp = src2.clone();
        src2_ = &src2_tmp;
    }

    for( i = 0; i < dst->rows; i++ )
    {
        if( depth == CC_32F )
        {
            const float* a = src1_->ptr<float>(i);
            const float* b = src2_->ptr<float>(i);
            float* c = dst.ptr<float>(i);

            if( !(flags & CC_DXT_MUL_CONJ) )
                for( j = 0; j < cols; j += 2 )
                {
                    double re = (double)a[j]*(double)b[j] - (double)a[j+1]*(double)b[j+1];
                    double im = (double)a[j+1]*(double)b[j] + (double)a[j]*(double)b[j+1];

                    c[j] = (float)re;
                    c[j+1] = (float)im;
                }
            else
                for( j = 0; j < cols; j += 2 )
                {
                    double re = (double)a[j]*(double)b[j] + (double)a[j+1]*(double)b[j+1];
                    double im = (double)a[j+1]*(double)b[j] - (double)a[j]*(double)b[j+1];

                    c[j] = (float)re;
                    c[j+1] = (float)im;
                }
        }
        else
        {
            const double* a = src1_->ptr<double>(i);
            const double* b = src2_->ptr<double>(i);
            double* c = dst.ptr<double>(i);

            if( !(flags & CC_DXT_MUL_CONJ) )
                for( j = 0; j < cols; j += 2 )
                {
                    double re = a[j]*b[j] - a[j+1]*b[j+1];
                    double im = a[j+1]*b[j] + a[j]*b[j+1];

                    c[j] = re;
                    c[j+1] = im;
                }
            else
                for( j = 0; j < cols; j += 2 )
                {
                    double re = a[j]*b[j] + a[j+1]*b[j+1];
                    double im = a[j+1]*b[j] - a[j]*b[j+1];

                    c[j] = re;
                    c[j+1] = im;
                }
        }
    }
}

}


class CxCore_DXTBaseTest : public cvtest::ArrayTest
{
public:
    typedef cvtest::ArrayTest Base;
    CxCore_DXTBaseTest( bool _allow_complex=false, bool _allow_odd=false,
                        bool _spectrum_mode=false );
protected:
    void get_test_array_types_and_sizes( int test_case_idx, vector<vector<Size> >& sizes, vector<vector<int> >& types );
    int prepare_test_case( int test_case_idx );
    double get_success_error_level( int /*test_case_idx*/, int /*i*/, int /*j*/ );
    int flags; // transformation flags
    bool allow_complex; // whether input/output may be complex or not:
                        // true for DFT and MulSpectrums, false for DCT
    bool allow_odd;     // whether input/output may be have odd (!=1) dimensions:
                        // true for DFT and MulSpectrums, false for DCT
    bool spectrum_mode; // (2 complex/ccs inputs, 1 complex/ccs output):
                        // true for MulSpectrums, false for DFT and DCT
    bool inplace;       // inplace operation (set for each individual test case)
    bool temp_dst;      // use temporary destination (for real->ccs DFT and ccs MulSpectrums)
};


CxCore_DXTBaseTest::CxCore_DXTBaseTest( bool _allow_complex, bool _allow_odd, bool _spectrum_mode )
: Base(), flags(0), allow_complex(_allow_complex), allow_odd(_allow_odd),
spectrum_mode(_spectrum_mode), inplace(false), temp_dst(false)
{
    test_array[INPUT].push_back(NULL);
    if( spectrum_mode )
        test_array[INPUT].push_back(NULL);
    test_array[OUTPUT].push_back(NULL);
    test_array[REF_OUTPUT].push_back(NULL);
    test_array[TEMP].push_back(NULL);
    test_array[TEMP].push_back(NULL);

    max_log_array_size = 9;
    element_wise_relative_error = spectrum_mode;
}


void CxCore_DXTBaseTest::get_test_array_types_and_sizes( int test_case_idx,
                                                         vector<vector<Size> >& sizes,
                                                         vector<vector<int> >& types )
{
    RNG& rng = ts->get_rng();
    int bits = cvtest::randInt(rng);
    int depth = cvtest::randInt(rng)%2 + CC_32F;
    int cn = !allow_complex || !(bits & 256) ? 1 : 2;
    Size size;
    Base::get_test_array_types_and_sizes( test_case_idx, sizes, types );

    flags = bits & (CC_DXT_INVERSE | CC_DXT_SCALE | CC_DXT_ROWS | CC_DXT_MUL_CONJ);
    if( spectrum_mode )
        flags &= ~CC_DXT_INVERSE;
    types[TEMP][0] = types[TEMP][1] = types[INPUT][0] =
    types[OUTPUT][0] = CC_MAKETYPE(depth, cn);
    size = sizes[INPUT][0];

    temp_dst = false;

    if( flags & CC_DXT_ROWS && (bits&1024) )
    {
        if( bits&16 )
            size.width = 1;
        else
            size.height = 1;
        flags &= ~CC_DXT_ROWS;
    }

    const int P2_MIN_SIZE = 32;
    if( ((bits >> 10) & 1) == 0 )
    {
        size.width = (size.width / P2_MIN_SIZE)*P2_MIN_SIZE;
        size.width = MAX(size.width, 1);
        size.height = (size.height / P2_MIN_SIZE)*P2_MIN_SIZE;
        size.height = MAX(size.height, 1);
    }

    if( !allow_odd )
    {
        if( size.width > 1 && (size.width&1) != 0 )
            size.width = (size.width + 1) & -2;

        if( size.height > 1 && (size.height&1) != 0 && !(flags & CC_DXT_ROWS) )
            size.height = (size.height + 1) & -2;
    }

    sizes[INPUT][0] = sizes[OUTPUT][0] = size;
    sizes[TEMP][0] = sizes[TEMP][1] = cSize(0,0);

    if( spectrum_mode )
    {
        if( cn == 1 )
        {
            types[OUTPUT][0] = depth + 8;
            sizes[TEMP][0] = size;
        }
        sizes[INPUT][0] = sizes[INPUT][1] = size;
        types[INPUT][1] = types[INPUT][0];
    }
    else if( /*(cn == 2 && (bits&32)) ||*/ (cn == 1 && allow_complex) )
    {
        types[TEMP][0] = depth + 8; // CC_??FC2
        sizes[TEMP][0] = size;
        size = cSize(size.width/2+1, size.height);

        if( flags & CC_DXT_INVERSE )
        {
            if( cn == 2 )
            {
                types[OUTPUT][0] = depth;
                sizes[INPUT][0] = size;
            }
            types[TEMP][1] = types[TEMP][0];
            sizes[TEMP][1] = sizes[TEMP][0];
        }
        else
        {
            if( allow_complex )
                types[OUTPUT][0] = depth + 8;

            if( cn == 2 )
            {
                types[INPUT][0] = depth;
                types[TEMP][1] = types[TEMP][0];
                sizes[TEMP][1] = size;
            }
            else
            {
                types[TEMP][1] = depth;
                sizes[TEMP][1] = sizes[TEMP][0];
            }
            temp_dst = true;
        }
    }

    inplace = false;
    if( spectrum_mode ||
       (!temp_dst && types[INPUT][0] == types[OUTPUT][0]) ||
       (temp_dst && types[INPUT][0] == types[TEMP][1]) )
        inplace = (bits & 64) != 0;

    types[REF_OUTPUT][0] = types[OUTPUT][0];
    sizes[REF_OUTPUT][0] = sizes[OUTPUT][0];
}


double CxCore_DXTBaseTest::get_success_error_level( int test_case_idx, int i, int j )
{
    return Base::get_success_error_level( test_case_idx, i, j );
}


int CxCore_DXTBaseTest::prepare_test_case( int test_case_idx )
{
    int code = Base::prepare_test_case( test_case_idx );
    if( code > 0 )
    {
        int in_type = test_mat[INPUT][0] CC_MAT_TYPE();
        int out_type = test_mat[OUTPUT][0] CC_MAT_TYPE();

        if( CC_MAT_CN(in_type) == 2 && CC_MAT_CN(out_type) == 1 )
            cvtest::fixCCS( test_mat[INPUT][0], test_mat[OUTPUT][0]->cols, flags );

        if( inplace )
            cvtest::copy( test_mat[INPUT][test_case_idx & (int)spectrum_mode],
                     temp_dst ? test_mat[TEMP][1] :
                     in_type == out_type ? test_mat[OUTPUT][0] :
                     test_mat[TEMP][0] );
    }

    return code;
}


////////////////////// FFT ////////////////////////
class CxCore_DFTTest : public CxCore_DXTBaseTest
{
public:
    CxCore_DFTTest();
protected:
    void run_func();
    void prepare_to_validation( int test_case_idx );
};


CxCore_DFTTest::CxCore_DFTTest() : CxCore_DXTBaseTest( true, true, false )
{
}


void CxCore_DFTTest::run_func()
{
    img_t& dst = temp_dst ? test_mat[TEMP][1] : test_mat[OUTPUT][0];
    const img_t& src = inplace ? dst : test_mat[INPUT][0];

    if(!(flags & CC_DXT_INVERSE))
        dft( src, dst, flags );
    else
        idft(src, dst, flags & ~CC_DXT_INVERSE);
}


void CxCore_DFTTest::prepare_to_validation( int /*test_case_idx*/ )
{
    img_t& src = test_mat[INPUT][0];
    img_t& dst = test_mat[REF_OUTPUT][0];
    img_t* tmp_src = &src;
    img_t* tmp_dst = &dst;
    int src_cn = src CC_MAT_CN();
    int dst_cn = dst CC_MAT_CN();

    if( src_cn != 2 || dst_cn != 2 )
    {
        tmp_src = &test_mat[TEMP][0];

        if( !(flags & CC_DXT_INVERSE ) )
        {
            img_t& cvdft_dst = test_mat[TEMP][1];
            cvtest::convertFromCCS( cvdft_dst, cvdft_dst,
                               test_mat[OUTPUT][0], flags );
            *tmp_src = Scalar::all(0);
            cvtest::insert( src, *tmp_src, 0 );
        }
        else
        {
            cvtest::convertFromCCS( src, src, *tmp_src, flags );
            tmp_dst = &test_mat[TEMP][1];
        }
    }

    if( src->rows == 1 || (src->cols == 1 && !(flags & CC_DXT_ROWS)) )
        cvtest::DFT_1D( *tmp_src, *tmp_dst, flags );
    else
        cvtest::DFT_2D( *tmp_src, *tmp_dst, flags );

    if( tmp_dst != &dst )
        cvtest::extract( *tmp_dst, dst, 0 );
}

////////////////////// DCT ////////////////////////
class CxCore_DCTTest : public CxCore_DXTBaseTest
{
public:
    CxCore_DCTTest();
protected:
    void run_func();
    void prepare_to_validation( int test_case_idx );
};


CxCore_DCTTest::CxCore_DCTTest() : CxCore_DXTBaseTest( false, false, false )
{
}


void CxCore_DCTTest::run_func()
{
    img_t& dst = test_mat[OUTPUT][0];
    const img_t& src = inplace ? dst : test_mat[INPUT][0];

    if(!(flags & CC_DXT_INVERSE))
        dct( src, dst, flags );
    else
        idct( src, dst, flags & ~CC_DXT_INVERSE);
}


void CxCore_DCTTest::prepare_to_validation( int /*test_case_idx*/ )
{
    const img_t& src = test_mat[INPUT][0];
    img_t& dst = test_mat[REF_OUTPUT][0];

    if( src->rows == 1 || (src->cols == 1 && !(flags & CC_DXT_ROWS)) )
        cvtest::DCT_1D( src, dst, flags );
    else
        cvtest::DCT_2D( src, dst, flags );
}


////////////////////// MulSpectrums ////////////////////////
class CxCore_MulSpectrumsTest : public CxCore_DXTBaseTest
{
public:
    CxCore_MulSpectrumsTest();
protected:
    void run_func();
    void prepare_to_validation( int test_case_idx );
    double get_success_error_level( int test_case_idx, int i, int j );
};


CxCore_MulSpectrumsTest::CxCore_MulSpectrumsTest() : CxCore_DXTBaseTest( true, true, true )
{
}

double CxCore_MulSpectrumsTest::get_success_error_level( int test_case_idx, int i, int j )
{
    (void)test_case_idx;
    CC_Assert(i == OUTPUT);
    CC_Assert(j == 0);
    int elem_depth = CC_MAT_DEPTH(cvGetElemType(test_array[i][j]));
    CC_Assert(elem_depth == CC_32F || elem_depth == CC_64F);

    element_wise_relative_error = false;
    double maxInputValue = 1000; // ArrayTest::get_minmax_bounds
    double err = 8 * maxInputValue;  // result = A*B + C*D
    return (elem_depth == CC_32F ? FLT_EPSILON : DBL_EPSILON) * err;
}

void CxCore_MulSpectrumsTest::run_func()
{
    img_t& dst = !test_mat[TEMP].empty() && !test_mat[TEMP][0].empty() ?
        test_mat[TEMP][0] : test_mat[OUTPUT][0];
    const img_t* src1 = &test_mat[INPUT][0], *src2 = &test_mat[INPUT][1];

    if( inplace )
    {
        if( ts->get_current_test_info()->test_case_idx & 1 )
            src2 = &dst;
        else
            src1 = &dst;
    }

    mulSpectrums( *src1, *src2, dst, flags, (flags & CC_DXT_MUL_CONJ) != 0 );
}


void CxCore_MulSpectrumsTest::prepare_to_validation( int /*test_case_idx*/ )
{
    img_t* src1 = &test_mat[INPUT][0];
    img_t* src2 = &test_mat[INPUT][1];
    img_t& dst = test_mat[OUTPUT][0];
    img_t& dst0 = test_mat[REF_OUTPUT][0];
    int cn = src1->CC_MAT_CN();

    if( cn == 1 )
    {
        cvtest::convertFromCCS( *src1, *src1, dst, flags );
        cvtest::convertFromCCS( *src2, *src2, dst0, flags );
        src1 = &dst;
        src2 = &dst0;
    }

    cvtest::mulComplex( *src1, *src2, dst0, flags );
    if( cn == 1 )
    {
        img_t& temp = test_mat[TEMP][0];
        cvtest::convertFromCCS( temp, temp, dst, flags );
    }
}

TEST(Core_DCT, accuracy) { CxCore_DCTTest test; test.safe_run(); }
TEST(Core_DFT, accuracy) { CxCore_DFTTest test; test.safe_run(); }
TEST(Core_MulSpectrums, accuracy) { CxCore_MulSpectrumsTest test; test.safe_run(); }

class Core_DFTComplexOutputTest : public cvtest::BaseTest
{
public:
    Core_DFTComplexOutputTest() {}
    ~Core_DFTComplexOutputTest() {}
protected:
    void run(int)
    {
        RNG& rng = theRNG();
        for( int i = 0; i < 10; i++ )
        {
            int m = rng.uniform(2, 11);
            int n = rng.uniform(2, 11);
            int depth = rng.uniform(0, 2) + CC_32F;
            img_t src8u(m, n, depth), src(m, n, depth), dst(m, n, CC_MAKETYPE(depth, 2));
            img_t z = img_t::zeros(m, n, depth), dstz;
            randu(src8u, Scalar::all(0), Scalar::all(10));
            src8u.convertTo(src, src CC_MAT_TYPE());
            dst = Scalar::all(123);
            img_t mv[] = {src, z}, srcz;
            merge(mv, 2, srcz);
            dft(srcz, dstz);
            dft(src, dst, DFT_COMPLEX_OUTPUT);
            if (cvtest::norm(dst, dstz, NORM_INF) > 1e-3)
            {
                cout << "actual:\n" << dst << endl << endl;
                cout << "reference:\n" << dstz << endl << endl;
                CC_Error(CC_StsError, "");
            }
        }
    }
};

TEST(Core_DFT, complex_output) { Core_DFTComplexOutputTest test; test.safe_run(); }

TEST(Core_DFT, complex_output2)
{
    for( int i = 0; i < 100; i++ )
    {
        int type = theRNG().uniform(0, 2) ? CC_64F : CC_32F;
        int m = theRNG().uniform(1, 10);
        int n = theRNG().uniform(1, 10);
        img_t x(m, n, type), out;
        randu(x, -1., 1.);
        dft(x, out, DFT_ROWS | DFT_COMPLEX_OUTPUT);
        double nrm = norm(out, NORM_INF);
        double thresh = n*m*2;
        if( nrm > thresh )
        {
            cout << "x: " << x << endl;
            cout << "out: " << out << endl;
            ASSERT_LT(nrm, thresh);
        }
    }
}

class Core_DXTReverseTest : public cvtest::BaseTest
{
public:
    enum Mode
    {
        ModeDFT,
        ModeDCT
    };
    Core_DXTReverseTest(Mode m) : mode(m) {}
private:
    Mode mode;
protected:
    void run(int)
    {
        for (int i = 0; i < 3; ++i)
        {
            if (mode == ModeDCT && i != 0)
                continue;
            int flags = 0;
            int flags_inv = DFT_INVERSE | DFT_SCALE;
            int cn_in = 0;
            int cn_out = 0;
            switch (i)
            {
                case 0: cn_in = 1; cn_out = 1; break;
                case 1: cn_in = 1; cn_out = 2; flags |= DFT_COMPLEX_OUTPUT; flags_inv |= DFT_REAL_OUTPUT; break;
                case 2: cn_in = 2; cn_out = 2; break;
            };
            for (int j = 0; j < 100; ++j)
            {
                RNG& rng = ts->get_rng();
                int type = rng.uniform(0, 2) ? CC_64F : CC_32F;
                int m = rng.uniform(1, 10);
                int n = rng.uniform(1, 10);
                if (mode == ModeDCT)
                {
                    m *= 2;
                    n *= 2;
                }
                img_t one(m, n, CC_MAKETYPE(type, cn_in));
                cvtest::randUni(rng, one, Scalar::all(-1.), Scalar::all(1.));
                img_t out;
                img_t two;
                if (mode == ModeDFT)
                {
                    dft(one, out, flags);
                    dft(out, two, flags_inv);
                }
                else if (mode == ModeDCT)
                {
                    dct(one, out, flags);
                    dct(out, two, flags_inv);
                }
                if (out CC_MAT_CN() != cn_out || two CC_MAT_CN() != cn_in || cvtest::norm(one, two, NORM_INF) > 1e-5)
                {
                    cout << "Test #" << j + 1 << " - "
                        << "elements: " << m << " x " << n << ", "
                        << "channels: "
                        << one CC_MAT_CN() << " (" << cn_in << ")" << " -> "
                        << out CC_MAT_CN() << " (" << cn_out << ")" << " -> "
                        << two CC_MAT_CN() << " (" << cn_in << ")"
                        << endl;
                    cout << "signal:\n" << one << endl << endl;
                    cout << "spectrum:\n" << out << endl << endl;
                    cout << "inverse:\n" << two << endl << endl;
                    ts->set_failed_test_info(cvtest::TS::FAIL_INVALID_OUTPUT);
                    break;
                }
            }
        }
    }
};

TEST(Core_DFT, reverse) { Core_DXTReverseTest test(Core_DXTReverseTest::ModeDFT); test.safe_run(); }
TEST(Core_DCT, reverse) { Core_DXTReverseTest test(Core_DXTReverseTest::ModeDCT); test.safe_run(); }
