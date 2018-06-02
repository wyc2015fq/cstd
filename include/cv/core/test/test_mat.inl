

#include <map>

#include "cstd.h"
#include "cv/core/matx.hpp"

int test_matx() {
  Matx<double, 2, 2> a(10, 11, 12, 13);
  Matx<double, 2, 2> b(10, 11, 12, 13);
  //Matx<double, 2, 2> c = a + b;
  //Matx<double, 2, 2> d = a * b;
  //d.MatMul(a, b);
  //Matx22d b(a);
  return 0;
}

#if 0

class Core_ReduceTest : public cvtest::BaseTest
{
public:
    Core_ReduceTest() {}
protected:
    void run( int);
    int checkOp( const img_t& src, int dstType, int opType, const img_t& opRes, int dim );
    int checkCase( int srcType, int dstType, int dim, Size sz );
    int checkDim( int dim, Size sz );
    int checkSize( Size sz );
};

template<class Type>
void testReduce( const img_t& src, img_t& sum, img_t& avg, img_t& max, img_t& min, int dim )
{
    assert( src CC_MAT_CN() == 1 );
    if( dim == 0 ) // row
    {
        sum cvSetMat( 1, src->cols, CC_64FC1 );
        max cvSetMat( 1, src->cols, CC_64FC1 );
        min cvSetMat( 1, src->cols, CC_64FC1 );
    }
    else
    {
        sum cvSetMat( src->rows, 1, CC_64FC1 );
        max cvSetMat( src->rows, 1, CC_64FC1 );
        min cvSetMat( src->rows, 1, CC_64FC1 );
    }
    sum.setTo(Scalar(0));
    max.setTo(Scalar(-DBL_MAX));
    min.setTo(Scalar(DBL_MAX));

    const Mat_<Type>& src_ = src;
    Mat_<double>& sum_ = (Mat_<double>&)sum;
    Mat_<double>& min_ = (Mat_<double>&)min;
    Mat_<double>& max_ = (Mat_<double>&)max;

    if( dim == 0 )
    {
        for( int ri = 0; ri < src->rows; ri++ )
        {
            for( int ci = 0; ci < src->cols; ci++ )
            {
                sum_(0, ci) += src_(ri, ci);
                max_(0, ci) = MAX( max_(0, ci), (double)src_(ri, ci) );
                min_(0, ci) = MIN( min_(0, ci), (double)src_(ri, ci) );
            }
        }
    }
    else
    {
        for( int ci = 0; ci < src->cols; ci++ )
        {
            for( int ri = 0; ri < src->rows; ri++ )
            {
                sum_(ri, 0) += src_(ri, ci);
                max_(ri, 0) = MAX( max_(ri, 0), (double)src_(ri, ci) );
                min_(ri, 0) = MIN( min_(ri, 0), (double)src_(ri, ci) );
            }
        }
    }
    sum.convertTo( avg, CC_64FC1 );
    avg = avg * (1.0 / (dim==0 ? (double)src->rows : (double)src->cols));
}

void getMatTypeStr( int type, string& str)
{
    str = type == CC_8UC1 ? "CC_8UC1" :
    type == CC_8SC1 ? "CC_8SC1" :
    type == CC_16UC1 ? "CC_16UC1" :
    type == CC_16SC1 ? "CC_16SC1" :
    type == CC_32SC1 ? "CC_32SC1" :
    type == CC_32FC1 ? "CC_32FC1" :
    type == CC_64FC1 ? "CC_64FC1" : "unsupported matrix type";
}

int Core_ReduceTest::checkOp( const img_t& src, int dstType, int opType, const img_t& opRes, int dim )
{
    int srcType = src CC_MAT_TYPE();
    bool support = false;
    if( opType == CC_REDUCE_SUM || opType == CC_REDUCE_AVG )
    {
        if( srcType == CC_8U && (dstType == CC_32S || dstType == CC_32F || dstType == CC_64F) )
            support = true;
        if( srcType == CC_16U && (dstType == CC_32F || dstType == CC_64F) )
            support = true;
        if( srcType == CC_16S && (dstType == CC_32F || dstType == CC_64F) )
            support = true;
        if( srcType == CC_32F && (dstType == CC_32F || dstType == CC_64F) )
            support = true;
        if( srcType == CC_64F && dstType == CC_64F)
            support = true;
    }
    else if( opType == CC_REDUCE_MAX )
    {
        if( srcType == CC_8U && dstType == CC_8U )
            support = true;
        if( srcType == CC_32F && dstType == CC_32F )
            support = true;
        if( srcType == CC_64F && dstType == CC_64F )
            support = true;
    }
    else if( opType == CC_REDUCE_MIN )
    {
        if( srcType == CC_8U && dstType == CC_8U)
            support = true;
        if( srcType == CC_32F && dstType == CC_32F)
            support = true;
        if( srcType == CC_64F && dstType == CC_64F)
            support = true;
    }
    if( !support )
        return cvtest::TS::OK;

    double eps = 0.0;
    if ( opType == CC_REDUCE_SUM || opType == CC_REDUCE_AVG )
    {
        if ( dstType == CC_32F )
            eps = 1.e-5;
        else if( dstType == CC_64F )
            eps = 1.e-8;
        else if ( dstType == CC_32S )
            eps = 0.6;
    }

    assert( opRes CC_MAT_TYPE() == CC_64FC1 );
    img_t _dst, dst, diff;
    reduce( src, _dst, dim, opType, dstType );
    _dst.convertTo( dst, CC_64FC1 );

    absdiff( opRes,dst,diff );
    bool check = false;
    if (dstType == CC_32F || dstType == CC_64F)
        check = countNonZero(diff>eps*dst) > 0;
    else
        check = countNonZero(diff>eps) > 0;
    if( check )
    {
        char msg[100];
        const char* opTypeStr = opType == CC_REDUCE_SUM ? "CC_REDUCE_SUM" :
        opType == CC_REDUCE_AVG ? "CC_REDUCE_AVG" :
        opType == CC_REDUCE_MAX ? "CC_REDUCE_MAX" :
        opType == CC_REDUCE_MIN ? "CC_REDUCE_MIN" : "unknown operation type";
        string srcTypeStr, dstTypeStr;
        getMatTypeStr( src CC_MAT_TYPE(), srcTypeStr );
        getMatTypeStr( dstType, dstTypeStr );
        const char* dimStr = dim == 0 ? "ROWS" : "COLS";

        sprintf( msg, "bad accuracy with srcType = %s, dstType = %s, opType = %s, dim = %s",
                srcTypeStr.c_str(), dstTypeStr.c_str(), opTypeStr, dimStr );
        ts->printf( cvtest::TS::LOG, msg );
        return cvtest::TS::FAIL_BAD_ACCURACY;
    }
    return cvtest::TS::OK;
}

int Core_ReduceTest::checkCase( int srcType, int dstType, int dim, Size sz )
{
    int code = cvtest::TS::OK, tempCode;
    img_t src, sum, avg, max, min;

    src cvSetMat( sz, srcType );
    randu( src, Scalar(0), Scalar(100) );

    if( srcType == CC_8UC1 )
        testReduce<uchar>( src, sum, avg, max, min, dim );
    else if( srcType == CC_8SC1 )
        testReduce<char>( src, sum, avg, max, min, dim );
    else if( srcType == CC_16UC1 )
        testReduce<unsigned short int>( src, sum, avg, max, min, dim );
    else if( srcType == CC_16SC1 )
        testReduce<short int>( src, sum, avg, max, min, dim );
    else if( srcType == CC_32SC1 )
        testReduce<int>( src, sum, avg, max, min, dim );
    else if( srcType == CC_32FC1 )
        testReduce<float>( src, sum, avg, max, min, dim );
    else if( srcType == CC_64FC1 )
        testReduce<double>( src, sum, avg, max, min, dim );
    else
        assert( 0 );

    // 1. sum
    tempCode = checkOp( src, dstType, CC_REDUCE_SUM, sum, dim );
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    // 2. avg
    tempCode = checkOp( src, dstType, CC_REDUCE_AVG, avg, dim );
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    // 3. max
    tempCode = checkOp( src, dstType, CC_REDUCE_MAX, max, dim );
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    // 4. min
    tempCode = checkOp( src, dstType, CC_REDUCE_MIN, min, dim );
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    return code;
}

int Core_ReduceTest::checkDim( int dim, Size sz )
{
    int code = cvtest::TS::OK, tempCode;

    // CC_8UC1
    tempCode = checkCase( CC_8UC1, CC_8UC1, dim, sz );
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    tempCode = checkCase( CC_8UC1, CC_32SC1, dim, sz );
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    tempCode = checkCase( CC_8UC1, CC_32FC1, dim, sz );
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    tempCode = checkCase( CC_8UC1, CC_64FC1, dim, sz );
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    // CC_16UC1
    tempCode = checkCase( CC_16UC1, CC_32FC1, dim, sz );
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    tempCode = checkCase( CC_16UC1, CC_64FC1, dim, sz );
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    // CC_16SC1
    tempCode = checkCase( CC_16SC1, CC_32FC1, dim, sz );
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    tempCode = checkCase( CC_16SC1, CC_64FC1, dim, sz );
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    // CC_32FC1
    tempCode = checkCase( CC_32FC1, CC_32FC1, dim, sz );
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    tempCode = checkCase( CC_32FC1, CC_64FC1, dim, sz );
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    // CC_64FC1
    tempCode = checkCase( CC_64FC1, CC_64FC1, dim, sz );
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    return code;
}

int Core_ReduceTest::checkSize( Size sz )
{
    int code = cvtest::TS::OK, tempCode;

    tempCode = checkDim( 0, sz ); // rows
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    tempCode = checkDim( 1, sz ); // cols
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    return code;
}

void Core_ReduceTest::run( int )
{
    int code = cvtest::TS::OK, tempCode;

    tempCode = checkSize( Size(1,1) );
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    tempCode = checkSize( Size(1,100) );
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    tempCode = checkSize( Size(100,1) );
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    tempCode = checkSize( Size(1000,500) );
    code = tempCode != cvtest::TS::OK ? tempCode : code;

    ts->set_failed_test_info( code );
}


#define CHECK_C

class Core_PCATest : public cvtest::BaseTest
{
public:
    Core_PCATest() {}
protected:
    void run(int)
    {
        const Size sz(200, 500);

        double diffPrjEps, diffBackPrjEps,
        prjEps, backPrjEps,
        evalEps, evecEps;
        int maxComponents = 100;
        double retainedVariance = 0.95;
        img_t rPoints(sz, CC_32FC1), rTestPoints(sz, CC_32FC1);
        RNG& rng = ts->get_rng();

        rng.fill( rPoints, RNG::UNIFORM, Scalar::all(0.0), Scalar::all(1.0) );
        rng.fill( rTestPoints, RNG::UNIFORM, Scalar::all(0.0), Scalar::all(1.0) );

        PCA rPCA( rPoints, img_t(), CC_PCA_DATA_AS_ROW, maxComponents ), cPCA;

        // 1. check C++ PCA & ROW
        img_t rPrjTestPoints = rPCA.project( rTestPoints );
        img_t rBackPrjTestPoints = rPCA.backProject( rPrjTestPoints );

        img_t avg(1, sz.width, CC_32FC1 );
        reduce( rPoints, avg, 0, CC_REDUCE_AVG );
        img_t Q = rPoints - repeat( avg, rPoints->rows, 1 ), Qt = Q.t(), eval, evec;
        Q = Qt * Q;
        Q = Q /(float)rPoints->rows;

        eigen( Q, eval, evec );
        /*CSVD svd(Q);
         evec = svd.vt;
         eval = svd.w;*/

        img_t subEval( maxComponents, 1, eval CC_MAT_TYPE(), eval->tt.data ),
        subEvec( maxComponents, evec->cols, evec CC_MAT_TYPE(), evec->tt.data );

    #ifdef CHECK_C
        img_t prjTestPoints, backPrjTestPoints, cPoints = rPoints.t(), cTestPoints = rTestPoints.t();
        img_t _points, _testPoints, _avg, _eval, _evec, _prjTestPoints, _backPrjTestPoints;
    #endif

        // check eigen()
        double eigenEps = 1e-6;
        double err;
        for(int i = 0; i < Q->rows; i++ )
        {
            img_t v = evec.row(i).t();
            img_t Qv = Q * v;

            img_t lv = eval.at<float>(i,0) * v;
            err = cvtest::norm( Qv, lv, NORM_L2 );
            if( err > eigenEps )
            {
                ts->printf( cvtest::TS::LOG, "bad accuracy of eigen(); err = %f\n", err );
                ts->set_failed_test_info( cvtest::TS::FAIL_BAD_ACCURACY );
                return;
            }
        }
        // check pca eigenvalues
        evalEps = 1e-6, evecEps = 1e-3;
        err = cvtest::norm( rPCA.eigenvalues, subEval, NORM_L2 );
        if( err > evalEps )
        {
            ts->printf( cvtest::TS::LOG, "pca.eigenvalues is incorrect (CC_PCA_DATA_AS_ROW); err = %f\n", err );
            ts->set_failed_test_info( cvtest::TS::FAIL_BAD_ACCURACY );
            return;
        }
        // check pca eigenvectors
        for(int i = 0; i < subEvec->rows; i++)
        {
            img_t r0 = rPCA.eigenvectors.row(i);
            img_t r1 = subEvec.row(i);
            err = cvtest::norm( r0, r1, CC_L2 );
            if( err > evecEps )
            {
                r1 *= -1;
                double err2 = cvtest::norm(r0, r1, CC_L2);
                if( err2 > evecEps )
                {
                    img_t tmp;
                    absdiff(rPCA.eigenvectors, subEvec, tmp);
                    double mval = 0; Point mloc;
                    minMaxLoc(tmp, 0, &mval, 0, &mloc);

                    ts->printf( cvtest::TS::LOG, "pca.eigenvectors is incorrect (CC_PCA_DATA_AS_ROW); err = %f\n", err );
                    ts->printf( cvtest::TS::LOG, "max diff is %g at (i=%d, j=%d) (%g vs %g)\n",
                               mval, mloc.y, mloc.x, rPCA.eigenvectors.at<float>(mloc.y, mloc.x),
                               subEvec.at<float>(mloc.y, mloc.x));
                    ts->set_failed_test_info( cvtest::TS::FAIL_BAD_ACCURACY );
                    return;
                }
            }
        }

        prjEps = 1.265, backPrjEps = 1.265;
        for( int i = 0; i < rTestPoints->rows; i++ )
        {
            // check pca project
            img_t subEvec_t = subEvec.t();
            img_t prj = rTestPoints.row(i) - avg; prj *= subEvec_t;
            err = cvtest::norm(rPrjTestPoints.row(i), prj, CC_RELATIVE_L2);
            if( err > prjEps )
            {
                ts->printf( cvtest::TS::LOG, "bad accuracy of project() (CC_PCA_DATA_AS_ROW); err = %f\n", err );
                ts->set_failed_test_info( cvtest::TS::FAIL_BAD_ACCURACY );
                return;
            }
            // check pca backProject
            img_t backPrj = rPrjTestPoints.row(i) * subEvec + avg;
            err = cvtest::norm( rBackPrjTestPoints.row(i), backPrj, CC_RELATIVE_L2 );
            if( err > backPrjEps )
            {
                ts->printf( cvtest::TS::LOG, "bad accuracy of backProject() (CC_PCA_DATA_AS_ROW); err = %f\n", err );
                ts->set_failed_test_info( cvtest::TS::FAIL_BAD_ACCURACY );
                return;
            }
        }

        // 2. check C++ PCA & COL
        cPCA( rPoints.t(), img_t(), CC_PCA_DATA_AS_COL, maxComponents );
        diffPrjEps = 1, diffBackPrjEps = 1;
        img_t ocvPrjTestPoints = cPCA.project(rTestPoints.t());
        err = cvtest::norm(abs(ocvPrjTestPoints), abs(rPrjTestPoints.t()), CC_RELATIVE_L2 );
        if( err > diffPrjEps )
        {
            ts->printf( cvtest::TS::LOG, "bad accuracy of project() (CC_PCA_DATA_AS_COL); err = %f\n", err );
            ts->set_failed_test_info( cvtest::TS::FAIL_BAD_ACCURACY );
            return;
        }
        err = cvtest::norm(cPCA.backProject(ocvPrjTestPoints), rBackPrjTestPoints.t(), CC_RELATIVE_L2 );
        if( err > diffBackPrjEps )
        {
            ts->printf( cvtest::TS::LOG, "bad accuracy of backProject() (CC_PCA_DATA_AS_COL); err = %f\n", err );
            ts->set_failed_test_info( cvtest::TS::FAIL_BAD_ACCURACY );
            return;
        }

        // 3. check C++ PCA w/retainedVariance
        cPCA( rPoints.t(), img_t(), CC_PCA_DATA_AS_COL, retainedVariance );
        diffPrjEps = 1, diffBackPrjEps = 1;
        img_t rvPrjTestPoints = cPCA.project(rTestPoints.t());

        if( cPCA.eigenvectors->rows > maxComponents)
            err = cvtest::norm(abs(rvPrjTestPoints cvGetRows(,0,maxComponents)), abs(rPrjTestPoints.t()), CC_RELATIVE_L2 );
        else
            err = cvtest::norm(abs(rvPrjTestPoints), abs(rPrjTestPoints.colRange(0,cPCA.eigenvectors->rows).t()), CC_RELATIVE_L2 );

        if( err > diffPrjEps )
        {
            ts->printf( cvtest::TS::LOG, "bad accuracy of project() (CC_PCA_DATA_AS_COL); retainedVariance=0.95; err = %f\n", err );
            ts->set_failed_test_info( cvtest::TS::FAIL_BAD_ACCURACY );
            return;
        }
        err = cvtest::norm(cPCA.backProject(rvPrjTestPoints), rBackPrjTestPoints.t(), CC_RELATIVE_L2 );
        if( err > diffBackPrjEps )
        {
            ts->printf( cvtest::TS::LOG, "bad accuracy of backProject() (CC_PCA_DATA_AS_COL); retainedVariance=0.95; err = %f\n", err );
            ts->set_failed_test_info( cvtest::TS::FAIL_BAD_ACCURACY );
            return;
        }

    #ifdef CHECK_C
        // 4. check C PCA & ROW
        _points = rPoints;
        _testPoints = rTestPoints;
        _avg = avg;
        _eval = eval;
        _evec = evec;
        prjTestPoints cvSetMat(rTestPoints->rows, maxComponents, rTestPoints CC_MAT_TYPE() );
        backPrjTestPoints cvSetMat(rPoints.size(), rPoints CC_MAT_TYPE() );
        _prjTestPoints = prjTestPoints;
        _backPrjTestPoints = backPrjTestPoints;

        cvCalcPCA( &_points, &_avg, &_eval, &_evec, CC_PCA_DATA_AS_ROW );
        cvProjectPCA( &_testPoints, &_avg, &_evec, &_prjTestPoints );
        cvBackProjectPCA( &_prjTestPoints, &_avg, &_evec, &_backPrjTestPoints );

        err = cvtest::norm(prjTestPoints, rPrjTestPoints, CC_RELATIVE_L2);
        if( err > diffPrjEps )
        {
            ts->printf( cvtest::TS::LOG, "bad accuracy of cvProjectPCA() (CC_PCA_DATA_AS_ROW); err = %f\n", err );
            ts->set_failed_test_info( cvtest::TS::FAIL_BAD_ACCURACY );
            return;
        }
        err = cvtest::norm(backPrjTestPoints, rBackPrjTestPoints, CC_RELATIVE_L2);
        if( err > diffBackPrjEps )
        {
            ts->printf( cvtest::TS::LOG, "bad accuracy of cvBackProjectPCA() (CC_PCA_DATA_AS_ROW); err = %f\n", err );
            ts->set_failed_test_info( cvtest::TS::FAIL_BAD_ACCURACY );
            return;
        }

        // 5. check C PCA & COL
        _points = cPoints;
        _testPoints = cTestPoints;
        avg = avg.t(); _avg = avg;
        eval = eval.t(); _eval = eval;
        evec = evec.t(); _evec = evec;
        prjTestPoints = prjTestPoints.t(); _prjTestPoints = prjTestPoints;
        backPrjTestPoints = backPrjTestPoints.t(); _backPrjTestPoints = backPrjTestPoints;

        cvCalcPCA( &_points, &_avg, &_eval, &_evec, CC_PCA_DATA_AS_COL );
        cvProjectPCA( &_testPoints, &_avg, &_evec, &_prjTestPoints );
        cvBackProjectPCA( &_prjTestPoints, &_avg, &_evec, &_backPrjTestPoints );

        err = cvtest::norm(abs(prjTestPoints), abs(rPrjTestPoints.t()), CC_RELATIVE_L2 );
        if( err > diffPrjEps )
        {
            ts->printf( cvtest::TS::LOG, "bad accuracy of cvProjectPCA() (CC_PCA_DATA_AS_COL); err = %f\n", err );
            ts->set_failed_test_info( cvtest::TS::FAIL_BAD_ACCURACY );
            return;
        }
        err = cvtest::norm(backPrjTestPoints, rBackPrjTestPoints.t(), CC_RELATIVE_L2);
        if( err > diffBackPrjEps )
        {
            ts->printf( cvtest::TS::LOG, "bad accuracy of cvBackProjectPCA() (CC_PCA_DATA_AS_COL); err = %f\n", err );
            ts->set_failed_test_info( cvtest::TS::FAIL_BAD_ACCURACY );
            return;
        }
    #endif
        // Test read and write
        FileStorage fs( "PCA_store.yml", FileStorage::WRITE );
        rPCA.write( fs );
        fs.release();

        PCA lPCA;
        fs.open( "PCA_store.yml", FileStorage::READ );
        lPCA.read( fs.root() );
        err = cvtest::norm( rPCA.eigenvectors, lPCA.eigenvectors, CC_RELATIVE_L2 );
        if( err > 0 )
        {
            ts->printf( cvtest::TS::LOG, "bad accuracy of write/load functions (YML); err = %f\n", err );
            ts->set_failed_test_info( cvtest::TS::FAIL_BAD_ACCURACY );
        }
        err = cvtest::norm( rPCA.eigenvalues, lPCA.eigenvalues, CC_RELATIVE_L2 );
        if( err > 0 )
        {
            ts->printf( cvtest::TS::LOG, "bad accuracy of write/load functions (YML); err = %f\n", err );
            ts->set_failed_test_info( cvtest::TS::FAIL_BAD_ACCURACY );
        }
        err = cvtest::norm( rPCA.mean, lPCA.mean, CC_RELATIVE_L2 );
        if( err > 0 )
        {
            ts->printf( cvtest::TS::LOG, "bad accuracy of write/load functions (YML); err = %f\n", err );
            ts->set_failed_test_info( cvtest::TS::FAIL_BAD_ACCURACY );
        }
    }
};

class Core_ArrayOpTest : public cvtest::BaseTest
{
public:
    Core_ArrayOpTest();
    ~Core_ArrayOpTest();
protected:
    void run(int);
};


Core_ArrayOpTest::Core_ArrayOpTest()
{
}
Core_ArrayOpTest::~Core_ArrayOpTest() {}

static string idx2string(const int* idx, int dims)
{
    char buf[256];
    char* ptr = buf;
    for( int k = 0; k < dims; k++ )
    {
        sprintf(ptr, "%4d ", idx[k]);
        ptr += strlen(ptr);
    }
    ptr[-1] = '\0';
    return string(buf);
}

static const int* string2idx(const string& s, int* idx, int dims)
{
    const char* ptr = s.c_str();
    for( int k = 0; k < dims; k++ )
    {
        int n = 0;
        sscanf(ptr, "%d%n", idx + k, &n);
        ptr += n;
    }
    return idx;
}

static double getValue(SparseMat& M, const int* idx, RNG& rng)
{
    int d = M.dims();
    size_t hv = 0, *phv = 0;
    if( (unsigned)rng % 2 )
    {
        hv = d == 2 ? M.hash(idx[0], idx[1]) :
        d == 3 ? M.hash(idx[0], idx[1], idx[2]) : M.hash(idx);
        phv = &hv;
    }

    const uchar* ptr = d == 2 ? M.ptr(idx[0], idx[1], false, phv) :
    d == 3 ? M.ptr(idx[0], idx[1], idx[2], false, phv) :
    M.ptr(idx, false, phv);
    return !ptr ? 0 : M CC_MAT_TYPE() == CC_32F ? *(float*)ptr : M CC_MAT_TYPE() == CC_64F ? *(double*)ptr : 0;
}

static double getValue(const CvSparseMat* M, const int* idx)
{
    int type = 0;
    const uchar* ptr = cvPtrND(M, idx, &type, 0);
    return !ptr ? 0 : type == CC_32F ? *(float*)ptr : type == CC_64F ? *(double*)ptr : 0;
}

static void eraseValue(SparseMat& M, const int* idx, RNG& rng)
{
    int d = M.dims();
    size_t hv = 0, *phv = 0;
    if( (unsigned)rng % 2 )
    {
        hv = d == 2 ? M.hash(idx[0], idx[1]) :
        d == 3 ? M.hash(idx[0], idx[1], idx[2]) : M.hash(idx);
        phv = &hv;
    }

    if( d == 2 )
        M.erase(idx[0], idx[1], phv);
    else if( d == 3 )
        M.erase(idx[0], idx[1], idx[2], phv);
    else
        M.erase(idx, phv);
}

static void eraseValue(CvSparseMat* M, const int* idx)
{
    cvClearND(M, idx);
}

static void setValue(SparseMat& M, const int* idx, double value, RNG& rng)
{
    int d = M.dims();
    size_t hv = 0, *phv = 0;
    if( (unsigned)rng % 2 )
    {
        hv = d == 2 ? M.hash(idx[0], idx[1]) :
        d == 3 ? M.hash(idx[0], idx[1], idx[2]) : M.hash(idx);
        phv = &hv;
    }

    uchar* ptr = d == 2 ? M.ptr(idx[0], idx[1], true, phv) :
    d == 3 ? M.ptr(idx[0], idx[1], idx[2], true, phv) :
    M.ptr(idx, true, phv);
    if( M CC_MAT_TYPE() == CC_32F )
        *(float*)ptr = (float)value;
    else if( M CC_MAT_TYPE() == CC_64F )
        *(double*)ptr = value;
    else
        CC_Error(CC_StsUnsupportedFormat, "");
}

template<typename Pixel>
struct InitializerFunctor{
    /// Initializer for img_t::forEach test
    void operator()(Pixel & pixel, const int * idx) const {
        pixel.x = idx[0];
        pixel.y = idx[1];
        pixel.z = idx[2];
    }
};

void Core_ArrayOpTest::run( int /* start_from */)
{
    int errcount = 0;

    // dense matrix operations
    {
        int sz3[] = {5, 10, 15};
        MatND A(3, sz3, CC_32F), B(3, sz3, CC_16SC4);
        CvMatND matA = A, matB = B;
        RNG rng;
        rng.fill(A, CC_RAND_UNI, Scalar::all(-10), Scalar::all(10));
        rng.fill(B, CC_RAND_UNI, Scalar::all(-10), Scalar::all(10));

        int idx0[] = {3,4,5}, idx1[] = {0, 9, 7};
        float val0 = 130;
        Scalar val1(-1000, 30, 3, 8);
        cvSetRealND(&matA, idx0, val0);
        cvSetReal3D(&matA, idx1[0], idx1[1], idx1[2], -val0);
        cvSetND(&matB, idx0, val1);
        cvSet3D(&matB, idx1[0], idx1[1], idx1[2], -val1);
        Ptr<CvMatND> matC(cvCloneMatND(&matB));

        if( A.at<float>(idx0[0], idx0[1], idx0[2]) != val0 ||
           A.at<float>(idx1[0], idx1[1], idx1[2]) != -val0 ||
           cvGetReal3D(&matA, idx0[0], idx0[1], idx0[2]) != val0 ||
           cvGetRealND(&matA, idx1) != -val0 ||

           Scalar(B.at<Vec4s>(idx0[0], idx0[1], idx0[2])) != val1 ||
           Scalar(B.at<Vec4s>(idx1[0], idx1[1], idx1[2])) != -val1 ||
           Scalar(cvGet3D(matC, idx0[0], idx0[1], idx0[2])) != val1 ||
           Scalar(cvGetND(matC, idx1)) != -val1 )
        {
            ts->printf(cvtest::TS::LOG, "one of cvSetReal3D, cvSetRealND, cvSet3D, cvSetND "
                       "or the corresponding *Get* functions is not correct\n");
            errcount++;
        }
    }
    // test img_t::forEach
    {
        const int dims[3] = { 101, 107, 7 };
        typedef CPoint3i Pixel;

        img_t a = img_t::zeros(3, dims, CC_32SC3);
        InitializerFunctor<Pixel> initializer;

        a.forEach<Pixel>(initializer);

        uint64 total = 0;
        bool error_reported = false;
        for (int i0 = 0; i0 < dims[0]; ++i0) {
            for (int i1 = 0; i1 < dims[1]; ++i1) {
                for (int i2 = 0; i2 < dims[2]; ++i2) {
                    Pixel& pixel = a.at<Pixel>(i0, i1, i2);
                    if (pixel.x != i0 || pixel.y != i1 || pixel.z != i2) {
                        if (!error_reported) {
                            ts->printf(cvtest::TS::LOG, "forEach is not correct.\n"
                                "First error detected at (%d, %d, %d).\n", pixel.x, pixel.y, pixel.z);
                            error_reported = true;
                        }
                        errcount++;
                    }
                    total += pixel.x;
                    total += pixel.y;
                    total += pixel.z;
                }
            }
        }
        uint64 total2 = 0;
        for (size_t i = 0; i < sizeof(dims) / sizeof(dims[0]); ++i) {
            total2 += ((dims[i] - 1) * dims[i] / 2) * dims[0] * dims[1] * dims[2] / dims[i];
        }
        if (total != total2) {
            ts->printf(cvtest::TS::LOG, "forEach is not correct because total is invalid.\n");
            errcount++;
        }
    }

    RNG rng;
    const int MAX_DIM = 5, MAX_DIM_SZ = 10;
    // sparse matrix operations
    for( int si = 0; si < 10; si++ )
    {
        int depth = (unsigned)rng % 2 == 0 ? CC_32F : CC_64F;
        int dims = ((unsigned)rng % MAX_DIM) + 1;
        int i, k, size[MAX_DIM]={0}, idx[MAX_DIM]={0};
        vector<string> all_idxs;
        vector<double> all_vals;
        vector<double> all_vals2;
        string sidx, min_sidx, max_sidx;
        double min_val=0, max_val=0;

        int p = 1;
        for( k = 0; k < dims; k++ )
        {
            size[k] = ((unsigned)rng % MAX_DIM_SZ) + 1;
            p *= size[k];
        }
        SparseMat M( dims, size, depth );
        map<string, double> M0;

        int nz0 = (unsigned)rng % max(p/5,10);
        nz0 = min(max(nz0, 1), p);
        all_vals.resize(nz0);
        all_vals2.resize(nz0);
        Mat_<double> _all_vals(all_vals), _all_vals2(all_vals2);
        rng.fill(_all_vals, CC_RAND_UNI, Scalar(-1000), Scalar(1000));
        if( depth == CC_32F )
        {
            img_t _all_vals_f;
            _all_vals.convertTo(_all_vals_f, CC_32F);
            _all_vals_f.convertTo(_all_vals, CC_64F);
        }
        _all_vals.convertTo(_all_vals2, _all_vals2 CC_MAT_TYPE(), 2);
        if( depth == CC_32F )
        {
            img_t _all_vals2_f;
            _all_vals2.convertTo(_all_vals2_f, CC_32F);
            _all_vals2_f.convertTo(_all_vals2, CC_64F);
        }

        minMaxLoc(_all_vals, &min_val, &max_val);
        double _norm0 = cvtest::norm(_all_vals, CC_C);
        double _norm1 = cvtest::norm(_all_vals, CC_L1);
        double _norm2 = cvtest::norm(_all_vals, CC_L2);

        for( i = 0; i < nz0; i++ )
        {
            for(;;)
            {
                for( k = 0; k < dims; k++ )
                    idx[k] = (unsigned)rng % size[k];
                sidx = idx2string(idx, dims);
                if( M0.count(sidx) == 0 )
                    break;
            }
            all_idxs.push_back(sidx);
            M0[sidx] = all_vals[i];
            if( all_vals[i] == min_val )
                min_sidx = sidx;
            if( all_vals[i] == max_val )
                max_sidx = sidx;
            setValue(M, idx, all_vals[i], rng);
            double v = getValue(M, idx, rng);
            if( v != all_vals[i] )
            {
                ts->printf(cvtest::TS::LOG, "%d. immediately after SparseMat[%s]=%.20g the current value is %.20g\n",
                           i, sidx.c_str(), all_vals[i], v);
                errcount++;
                break;
            }
        }

        Ptr<CvSparseMat> M2(cvCreateSparseMat(M));
        MatND Md;
        M.copyTo(Md);
        SparseMat M3; SparseMat(Md).convertTo(M3, Md CC_MAT_TYPE(), 2);

        int nz1 = (int)M.nzcount(), nz2 = (int)M3.nzcount();
        double norm0 = norm(M, CC_C);
        double norm1 = norm(M, CC_L1);
        double norm2 = norm(M, CC_L2);
        double eps = depth == CC_32F ? FLT_EPSILON*100 : DBL_EPSILON*1000;

        if( nz1 != nz0 || nz2 != nz0)
        {
            errcount++;
            ts->printf(cvtest::TS::LOG, "%d: The number of non-zero elements before/after converting to/from dense matrix is not correct: %d/%d (while it should be %d)\n",
                       si, nz1, nz2, nz0 );
            break;
        }

        if( fabs(norm0 - _norm0) > fabs(_norm0)*eps ||
           fabs(norm1 - _norm1) > fabs(_norm1)*eps ||
           fabs(norm2 - _norm2) > fabs(_norm2)*eps )
        {
            errcount++;
            ts->printf(cvtest::TS::LOG, "%d: The norms are different: %.20g/%.20g/%.20g vs %.20g/%.20g/%.20g\n",
                       si, norm0, norm1, norm2, _norm0, _norm1, _norm2 );
            break;
        }

        int n = (unsigned)rng % max(p/5,10);
        n = min(max(n, 1), p) + nz0;

        for( i = 0; i < n; i++ )
        {
            double val1, val2, val3, val0;
            if(i < nz0)
            {
                sidx = all_idxs[i];
                string2idx(sidx, idx, dims);
                val0 = all_vals[i];
            }
            else
            {
                for( k = 0; k < dims; k++ )
                    idx[k] = (unsigned)rng % size[k];
                sidx = idx2string(idx, dims);
                val0 = M0[sidx];
            }
            val1 = getValue(M, idx, rng);
            val2 = getValue(M2, idx);
            val3 = getValue(M3, idx, rng);

            if( val1 != val0 || val2 != val0 || fabs(val3 - val0*2) > fabs(val0*2)*FLT_EPSILON )
            {
                errcount++;
                ts->printf(cvtest::TS::LOG, "SparseMat M[%s] = %g/%g/%g (while it should be %g)\n", sidx.c_str(), val1, val2, val3, val0 );
                break;
            }
        }

        for( i = 0; i < n; i++ )
        {
            double val1, val2;
            if(i < nz0)
            {
                sidx = all_idxs[i];
                string2idx(sidx, idx, dims);
            }
            else
            {
                for( k = 0; k < dims; k++ )
                    idx[k] = (unsigned)rng % size[k];
                sidx = idx2string(idx, dims);
            }
            eraseValue(M, idx, rng);
            eraseValue(M2, idx);
            val1 = getValue(M, idx, rng);
            val2 = getValue(M2, idx);
            if( val1 != 0 || val2 != 0 )
            {
                errcount++;
                ts->printf(cvtest::TS::LOG, "SparseMat: after deleting M[%s], it is =%g/%g (while it should be 0)\n", sidx.c_str(), val1, val2 );
                break;
            }
        }

        int nz = (int)M.nzcount();
        if( nz != 0 )
        {
            errcount++;
            ts->printf(cvtest::TS::LOG, "The number of non-zero elements after removing all the elements = %d (while it should be 0)\n", nz );
            break;
        }

        int idx1[MAX_DIM], idx2[MAX_DIM];
        double val1 = 0, val2 = 0;
        M3 = SparseMat(Md);
        minMaxLoc(M3, &val1, &val2, idx1, idx2);
        string s1 = idx2string(idx1, dims), s2 = idx2string(idx2, dims);
        if( val1 != min_val || val2 != max_val || s1 != min_sidx || s2 != max_sidx )
        {
            errcount++;
            ts->printf(cvtest::TS::LOG, "%d. Sparse: The value and positions of minimum/maximum elements are different from the reference values and positions:\n\t"
                       "(%g, %g, %s, %s) vs (%g, %g, %s, %s)\n", si, val1, val2, s1.c_str(), s2.c_str(),
                       min_val, max_val, min_sidx.c_str(), max_sidx.c_str());
            break;
        }

        minMaxIdx(Md, &val1, &val2, idx1, idx2);
        s1 = idx2string(idx1, dims), s2 = idx2string(idx2, dims);
        if( (min_val < 0 && (val1 != min_val || s1 != min_sidx)) ||
           (max_val > 0 && (val2 != max_val || s2 != max_sidx)) )
        {
            errcount++;
            ts->printf(cvtest::TS::LOG, "%d. Dense: The value and positions of minimum/maximum elements are different from the reference values and positions:\n\t"
                       "(%g, %g, %s, %s) vs (%g, %g, %s, %s)\n", si, val1, val2, s1.c_str(), s2.c_str(),
                       min_val, max_val, min_sidx.c_str(), max_sidx.c_str());
            break;
        }
    }

    ts->set_failed_test_info(errcount == 0 ? cvtest::TS::OK : cvtest::TS::FAIL_INVALID_OUTPUT);
}


template <class ElemType>
int calcDiffElemCountImpl(const vector<img_t>& mv, const img_t& m)
{
    int diffElemCount = 0;
    const int mChannels = m CC_MAT_CN();
    for(int y = 0; y < m->rows; y++)
    {
        for(int x = 0; x < m->cols; x++)
        {
            const ElemType* mElem = &m.at<ElemType>(y,x*mChannels);
            size_t loc = 0;
            for(size_t i = 0; i < mv.size(); i++)
            {
                const size_t mvChannel = mv[i] CC_MAT_CN();
                const ElemType* mvElem = &mv[i].at<ElemType>(y,x*(int)mvChannel);
                for(size_t li = 0; li < mvChannel; li++)
                    if(mElem[loc + li] != mvElem[li])
                        diffElemCount++;
                loc += mvChannel;
            }
            CC_Assert(loc == (size_t)mChannels);
        }
    }
    return diffElemCount;
}

static
int calcDiffElemCount(const vector<img_t>& mv, const img_t& m)
{
    int depth = m CC_MAT_DEPTH();
    switch (depth)
    {
    case CC_8U:
        return calcDiffElemCountImpl<uchar>(mv, m);
    case CC_8S:
        return calcDiffElemCountImpl<char>(mv, m);
    case CC_16U:
        return calcDiffElemCountImpl<unsigned short>(mv, m);
    case CC_16S:
        return calcDiffElemCountImpl<short int>(mv, m);
    case CC_32S:
        return calcDiffElemCountImpl<int>(mv, m);
    case CC_32F:
        return calcDiffElemCountImpl<float>(mv, m);
    case CC_64F:
        return calcDiffElemCountImpl<double>(mv, m);
    }

    return INT_MAX;
}

class Core_MergeSplitBaseTest : public cvtest::BaseTest
{
protected:
    virtual int run_case(int depth, size_t channels, const Size& size, RNG& rng) = 0;

    virtual void run(int)
    {
        // m is img_t
        // mv is vector<img_t>
        const int minMSize = 1;
        const int maxMSize = 100;
        const size_t maxMvSize = 10;

        RNG& rng = theRNG();
        Size mSize(rng.uniform(minMSize, maxMSize), rng.uniform(minMSize, maxMSize));
        size_t mvSize = rng.uniform(1, maxMvSize);

        int res = cvtest::TS::OK, curRes = res;
        curRes = run_case(CC_8U, mvSize, mSize, rng);
        res = curRes != cvtest::TS::OK ? curRes : res;

        curRes = run_case(CC_8S, mvSize, mSize, rng);
        res = curRes != cvtest::TS::OK ? curRes : res;

        curRes = run_case(CC_16U, mvSize, mSize, rng);
        res = curRes != cvtest::TS::OK ? curRes : res;

        curRes = run_case(CC_16S, mvSize, mSize, rng);
        res = curRes != cvtest::TS::OK ? curRes : res;

        curRes = run_case(CC_32S, mvSize, mSize, rng);
        res = curRes != cvtest::TS::OK ? curRes : res;

        curRes = run_case(CC_32F, mvSize, mSize, rng);
        res = curRes != cvtest::TS::OK ? curRes : res;

        curRes = run_case(CC_64F, mvSize, mSize, rng);
        res = curRes != cvtest::TS::OK ? curRes : res;

        ts->set_failed_test_info(res);
    }
};

class Core_MergeTest : public Core_MergeSplitBaseTest
{
public:
    Core_MergeTest() {}
    ~Core_MergeTest() {}

protected:
    virtual int run_case(int depth, size_t matCount, const Size& size, RNG& rng)
    {
        const int maxMatChannels = 10;

        vector<img_t> src(matCount);
        int channels = 0;
        for(size_t i = 0; i < cvGetSize(src); i++)
        {
            img_t m(size, CC_MAKETYPE(depth, rng.uniform(1,maxMatChannels)));
            rng.fill(m, RNG::UNIFORM, 0, 100, true);
            channels += m CC_MAT_CN();
            src[i] = m;
        }

        img_t dst;
        merge(src, dst);

        // check result
        stringstream commonLog;
        commonLog << "Depth " << depth << " :";
        if(dst CC_MAT_DEPTH() != depth)
        {
            ts->printf(cvtest::TS::LOG, "%s incorrect depth of dst (%d instead of %d)\n",
                       commonLog.str().c_str(), dst CC_MAT_DEPTH(), depth);
            return cvtest::TS::FAIL_INVALID_OUTPUT;
        }
        if(dst.size() != size)
        {
            ts->printf(cvtest::TS::LOG, "%s incorrect size of dst (%d x %d instead of %d x %d)\n",
                       commonLog.str().c_str(), dst->rows, dst->cols, size.height, size.width);
            return cvtest::TS::FAIL_INVALID_OUTPUT;
        }
        if(dst CC_MAT_CN() != channels)
        {
            ts->printf(cvtest::TS::LOG, "%s: incorrect channels count of dst (%d instead of %d)\n",
                       commonLog.str().c_str(), dst CC_MAT_CN(), channels);
            return cvtest::TS::FAIL_INVALID_OUTPUT;
        }

        int diffElemCount = calcDiffElemCount(src, dst);
        if(diffElemCount > 0)
        {
            ts->printf(cvtest::TS::LOG, "%s: there are incorrect elements in dst (part of them is %f)\n",
                       commonLog.str().c_str(), static_cast<float>(diffElemCount)/(channels*size.area()));
            return cvtest::TS::FAIL_INVALID_OUTPUT;
        }

        return cvtest::TS::OK;
    }
};

class Core_SplitTest : public Core_MergeSplitBaseTest
{
public:
    Core_SplitTest() {}
    ~Core_SplitTest() {}

protected:
    virtual int run_case(int depth, size_t channels, const Size& size, RNG& rng)
    {
        img_t src(size, CC_MAKETYPE(depth, (int)channels));
        rng.fill(src, RNG::UNIFORM, 0, 100, true);

        vector<img_t> dst;
        split(src, dst);

        // check result
        stringstream commonLog;
        commonLog << "Depth " << depth << " :";
        if(dst.size() != channels)
        {
            ts->printf(cvtest::TS::LOG, "%s incorrect count of matrices in dst (%d instead of %d)\n",
                       commonLog.str().c_str(), dst.size(), channels);
            return cvtest::TS::FAIL_INVALID_OUTPUT;
        }
        for(size_t i = 0; i < dst.size(); i++)
        {
            if(dst[i].size() != size)
            {
                ts->printf(cvtest::TS::LOG, "%s incorrect size of dst[%d] (%d x %d instead of %d x %d)\n",
                           commonLog.str().c_str(), i, dst[i]->rows, dst[i]->cols, size.height, size.width);
                return cvtest::TS::FAIL_INVALID_OUTPUT;
            }
            if(dst[i] CC_MAT_DEPTH() != depth)
            {
                ts->printf(cvtest::TS::LOG, "%s: incorrect depth of dst[%d] (%d instead of %d)\n",
                           commonLog.str().c_str(), i, dst[i] CC_MAT_DEPTH(), depth);
                return cvtest::TS::FAIL_INVALID_OUTPUT;
            }
            if(dst[i] CC_MAT_CN() != 1)
            {
                ts->printf(cvtest::TS::LOG, "%s: incorrect channels count of dst[%d] (%d instead of %d)\n",
                           commonLog.str().c_str(), i, dst[i] CC_MAT_CN(), 1);
                return cvtest::TS::FAIL_INVALID_OUTPUT;
            }
        }

        int diffElemCount = calcDiffElemCount(dst, src);
        if(diffElemCount > 0)
        {
            ts->printf(cvtest::TS::LOG, "%s: there are incorrect elements in dst (part of them is %f)\n",
                       commonLog.str().c_str(), static_cast<float>(diffElemCount)/(channels*size.area()));
            return cvtest::TS::FAIL_INVALID_OUTPUT;
        }

        return cvtest::TS::OK;
    }
};

TEST(Core_PCA, accuracy) { Core_PCATest test; test.safe_run(); }
TEST(Core_Reduce, accuracy) { Core_ReduceTest test; test.safe_run(); }
TEST(Core_Array, basic_operations) { Core_ArrayOpTest test; test.safe_run(); }

TEST(Core_Merge, shape_operations) { Core_MergeTest test; test.safe_run(); }
TEST(Core_Split, shape_operations) { Core_SplitTest test; test.safe_run(); }


TEST(Core_IOArray, submat_assignment)
{
    Mat1f A = Mat1f::zeros(2,2);
    Mat1f B = Mat1f::ones(1,3);

    EXPECT_THROW( B.colRange(0,3).copyTo(A.row(0)), Exception );

    EXPECT_NO_THROW( B.colRange(0,2).copyTo(A.row(0)) );

    EXPECT_EQ( 1.0f, A(0,0) );
    EXPECT_EQ( 1.0f, A(0,1) );
}

void OutputArray_create1(img_t* m) { m cvSetMat(1, 2, CC_32S); }
void OutputArray_create2(img_t* m) { m cvSetMat(1, 3, CC_32F); }

TEST(Core_IOArray, submat_create)
{
    Mat1f A = Mat1f::zeros(2,2);

    EXPECT_THROW( OutputArray_create1(A.row(0)), Exception );
    EXPECT_THROW( OutputArray_create2(A.row(0)), Exception );
}

TEST(Core_Mat, issue4457_pass_null_ptr)
{
    ASSERT_ANY_THROW(img_t mask(45, 45, CC_32F, 0));
}

TEST(Core_Mat, reshape_1942)
{
    img_t A = (Mat_<float>(2,3) << 3.4884074, 1.4159607, 0.78737736,  2.3456569, -0.88010466, 0.3009364);
    int cn = 0;
    ASSERT_NO_THROW(
        Mat_<float> M = A cvReshape(,3);
        cn = M CC_MAT_CN();
    );
    ASSERT_EQ(1, cn);
}

static void check_ndim_shape(const img_t &mat, int cn, int ndims, const int *sizes)
{
    EXPECT_EQ(mat CC_MAT_CN(), cn);
    EXPECT_EQ(mat.dims, ndims);

    if (mat.dims != ndims)
        return;

    for (int i = 0; i < ndims; i++)
        EXPECT_EQ(mat.size[i], sizes[i]);
}

TEST(Core_Mat, reshape_ndims_2)
{
    const img_t A(8, 16, CC_8UC3);
    img_t B;

    {
        int new_sizes_mask[] = { 0, 3, 4, 4 };
        int new_sizes_real[] = { 8, 3, 4, 4 };
        ASSERT_NO_THROW(B = A cvReshape(,1, 4, new_sizes_mask));
        check_ndim_shape(B, 1, 4, new_sizes_real);
    }
    {
        int new_sizes[] = { 16, 8 };
        ASSERT_NO_THROW(B = A cvReshape(,0, 2, new_sizes));
        check_ndim_shape(B, 3, 2, new_sizes);
        EXPECT_EQ(B->rows, new_sizes[0]);
        EXPECT_EQ(B->cols, new_sizes[1]);
    }
    {
        int new_sizes[] = { 2, 5, 1, 3 };
        img_t A_sliced = A(CRange::all(), CRange(0, 15));
        ASSERT_ANY_THROW(A_sliced cvReshape(,4, 4, new_sizes));
    }
}

TEST(Core_Mat, reshape_ndims_4)
{
    const int sizes[] = { 2, 6, 4, 12 };
    const img_t A(4, sizes, CC_8UC3);
    img_t B;

    {
        int new_sizes_mask[] = { 0, 864 };
        int new_sizes_real[] = { 2, 864 };
        ASSERT_NO_THROW(B = A cvReshape(,1, 2, new_sizes_mask));
        check_ndim_shape(B, 1, 2, new_sizes_real);
        EXPECT_EQ(B->rows, new_sizes_real[0]);
        EXPECT_EQ(B->cols, new_sizes_real[1]);
    }
    {
        int new_sizes_mask[] = { 4, 0, 0, 2, 3 };
        int new_sizes_real[] = { 4, 6, 4, 2, 3 };
        ASSERT_NO_THROW(B = A cvReshape(,0, 5, new_sizes_mask));
        check_ndim_shape(B, 3, 5, new_sizes_real);
    }
    {
        int new_sizes_mask[] = { 1, 1 };
        ASSERT_ANY_THROW(A cvReshape(,0, 2, new_sizes_mask));
    }
    {
        int new_sizes_mask[] = { 4, 6, 3, 3, 0 };
        ASSERT_ANY_THROW(A cvReshape(,0, 5, new_sizes_mask));
    }
}

TEST(Core_Mat, push_back)
{
    img_t a = (Mat_<float>(1,2) << 3.4884074f, 1.4159607f);
    img_t b = (Mat_<float>(1,2) << 0.78737736f, 2.3456569f);

    a.push_back(b);

    ASSERT_EQ(2, a->cols);
    ASSERT_EQ(2, a->rows);

    ASSERT_FLOAT_EQ(3.4884074f, a.at<float>(0, 0));
    ASSERT_FLOAT_EQ(1.4159607f, a.at<float>(0, 1));
    ASSERT_FLOAT_EQ(0.78737736f, a.at<float>(1, 0));
    ASSERT_FLOAT_EQ(2.3456569f, a.at<float>(1, 1));

    img_t c = (Mat_<float>(2,2) << -0.88010466f, 0.3009364f, 2.22399974f, -5.45933905f);

    ASSERT_EQ(c->rows, a->cols);

    a.push_back(c.t());

    ASSERT_EQ(2, a->cols);
    ASSERT_EQ(4, a->rows);

    ASSERT_FLOAT_EQ(3.4884074f, a.at<float>(0, 0));
    ASSERT_FLOAT_EQ(1.4159607f, a.at<float>(0, 1));
    ASSERT_FLOAT_EQ(0.78737736f, a.at<float>(1, 0));
    ASSERT_FLOAT_EQ(2.3456569f, a.at<float>(1, 1));
    ASSERT_FLOAT_EQ(-0.88010466f, a.at<float>(2, 0));
    ASSERT_FLOAT_EQ(2.22399974f, a.at<float>(2, 1));
    ASSERT_FLOAT_EQ(0.3009364f, a.at<float>(3, 0));
    ASSERT_FLOAT_EQ(-5.45933905f, a.at<float>(3, 1));

    a.push_back(img_t::ones(2, 2, CC_32FC1));

    ASSERT_EQ(6, a->rows);

    for(int row=4; row<a->rows; row++) {

        for(int col=0; col<a->cols; col++) {

            ASSERT_FLOAT_EQ(1.f, a.at<float>(row, col));
        }
    }
}

TEST(Core_Mat, copyNx1ToVector)
{
    Mat_<uchar> src(5, 1);
    Mat_<uchar> ref_dst8;
    Mat_<ushort> ref_dst16;
    std::vector<uchar> dst8;
    std::vector<ushort> dst16;

    src << 1, 2, 3, 4, 5;

    src.copyTo(ref_dst8);
    src.copyTo(dst8);

    ASSERT_PRED_FORMAT2(cvtest::MatComparator(0, 0), ref_dst8, Mat_<uchar>(dst8));

    src.convertTo(ref_dst16, CC_16U);
    src.convertTo(dst16, CC_16U);

    ASSERT_PRED_FORMAT2(cvtest::MatComparator(0, 0), ref_dst16, Mat_<ushort>(dst16));
}

TEST(Core_Matx, fromMat_)
{
    Mat_<double> a = (Mat_<double>(2,2) << 10, 11, 12, 13);
    Matx22d b(a);
    ASSERT_EQ( cvtest::norm(a, b, NORM_INF), 0.);
}

TEST(Core_InputArray, empty)
{
    vector<vector<Point> > data;
    ASSERT_TRUE( _InputArray(data).empty() );
}

TEST(Core_CopyMask, bug1918)
{
    Mat_<unsigned char> tmpSrc(100,100);
    tmpSrc = 124;
    Mat_<unsigned char> tmpMask(100,100);
    tmpMask = 255;
    Mat_<unsigned char> tmpDst(100,100);
    tmpDst = 2;
    tmpSrc.copyTo(tmpDst,tmpMask);
    ASSERT_EQ(sum(tmpDst)[0], 124*100*100);
}

TEST(Core_SVD, orthogonality)
{
    for( int i = 0; i < 2; i++ )
    {
        int type = i == 0 ? CC_32F : CC_64F;
        img_t mat_D(2, 2, type);
        mat_D.setTo(88.);
        img_t mat_U, mat_W;
        CSVD::compute(mat_D, mat_W, mat_U, NULL, CSVD::FULL_UV);
        mat_U *= mat_U.t();
        ASSERT_LT(norm(mat_U, img_t::eye(2, 2, type), NORM_INF), 1e-5);
    }
}


TEST(Core_SparseMat, footprint)
{
    int n = 1000000;
    int sz[] = { n, n };
    SparseMat m(2, sz, CC_64F);

    int nodeSize0 = (int)m.hdr->nodeSize;
    double dataSize0 = ((double)m.hdr->pool.size() + (double)m.hdr->hashtab.size()*sizeof(size_t))*1e-6;
    printf("before: node size=%d bytes, data size=%.0f Mbytes\n", nodeSize0, dataSize0);

    for (int i = 0; i < n; i++)
    {
        m.ref<double>(i, i) = 1;
    }

    double dataSize1 = ((double)m.hdr->pool.size() + (double)m.hdr->hashtab.size()*sizeof(size_t))*1e-6;
    double threshold = (n*nodeSize0*1.6 + n*2.*sizeof(size_t))*1e-6;
    printf("after: data size=%.0f Mbytes, threshold=%.0f MBytes\n", dataSize1, threshold);

    ASSERT_LE((int)m.hdr->nodeSize, 32);
    ASSERT_LE(dataSize1, threshold);
}


// Can't fix without dirty hacks or broken user code (PR #4159)
TEST(Core_Mat_vector, DISABLED_OutputArray_create_getMat)
{
    Mat_<uchar> src_base(5, 1);
    std::vector<uchar> dst8;

    src_base << 1, 2, 3, 4, 5;

    img_t src(src_base);
    img_t* _dst(dst8);
    {
        _dst cvSetMat(src->rows, src->cols, src CC_MAT_TYPE());
        img_t *dst = _dst;
        EXPECT_EQ(src.dims, dst.dims);
        EXPECT_EQ(src->cols, dst->cols);
        EXPECT_EQ(src->rows, dst->rows);
    }
}

TEST(Core_Mat_vector, copyTo_roi_column)
{
    Mat_<uchar> src_base(5, 2);
    std::vector<uchar> dst1;

    src_base << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10;

    img_t src_full(src_base);
    img_t src(src_full.col(0));
#if 0 // Can't fix without dirty hacks or broken user code (PR #4159)
    img_t* _dst(dst1);
    {
        _dst cvSetMat(src->rows, src->cols, src CC_MAT_TYPE());
        img_t *dst = _dst;
        EXPECT_EQ(src.dims, dst.dims);
        EXPECT_EQ(src->cols, dst->cols);
        EXPECT_EQ(src->rows, dst->rows);
    }
#endif

    std::vector<uchar> dst2;
    src.copyTo(dst2);
    std::cout << "src = " << src << std::endl;
    std::cout << "dst = " << img_t(dst2) << std::endl;
    EXPECT_EQ((size_t)5, dst2.size());
    EXPECT_EQ(1, (int)dst2[0]);
    EXPECT_EQ(3, (int)dst2[1]);
    EXPECT_EQ(5, (int)dst2[2]);
    EXPECT_EQ(7, (int)dst2[3]);
    EXPECT_EQ(9, (int)dst2[4]);
}

TEST(Core_Mat_vector, copyTo_roi_row)
{
    Mat_<uchar> src_base(2, 5);
    std::vector<uchar> dst1;

    src_base << 1, 2, 3, 4, 5, 6, 7, 8, 9, 10;

    img_t src_full(src_base);
    img_t src(src_full.row(0));
    img_t* _dst(dst1);
    {
        _dst cvSetMat(src->rows, src->cols, src CC_MAT_TYPE());
        img_t *dst = _dst;
        EXPECT_EQ(src.dims, dst.dims);
        EXPECT_EQ(src->cols, dst->cols);
        EXPECT_EQ(src->rows, dst->rows);
    }

    std::vector<uchar> dst2;
    src.copyTo(dst2);
    std::cout << "src = " << src << std::endl;
    std::cout << "dst = " << img_t(dst2) << std::endl;
    EXPECT_EQ((size_t)5, dst2.size());
    EXPECT_EQ(1, (int)dst2[0]);
    EXPECT_EQ(2, (int)dst2[1]);
    EXPECT_EQ(3, (int)dst2[2]);
    EXPECT_EQ(4, (int)dst2[3]);
    EXPECT_EQ(5, (int)dst2[4]);
}

TEST(img_t, regression_5991)
{
    int sz[] = {2,3,2};
    img_t mat(3, sz, CC_32F, Scalar(1));
    ASSERT_NO_THROW(mat.convertTo(mat, CC_8U));
    EXPECT_EQ(sz[0], mat.size[0]);
    EXPECT_EQ(sz[1], mat.size[1]);
    EXPECT_EQ(sz[2], mat.size[2]);
    EXPECT_EQ(0, cvtest::norm(mat, img_t(3, sz, CC_8U, Scalar(1)), NORM_INF));
}

#ifdef OPENCC_TEST_BIGDATA
TEST(img_t, regression_6696_BigData_8Gb)
{
    int width = 60000;
    int height = 10000;

    img_t destImageBGR = img_t(height, width, CC_8UC3, Scalar(1, 2, 3, 0));
    img_t destImageA = img_t(height, width, CC_8UC1, Scalar::all(4));

    vector<img_t> planes;
    split(destImageBGR, planes);
    planes.push_back(destImageA);
    merge(planes, destImageBGR);

    EXPECT_EQ(1, destImageBGR.at<Vec4b>(0)[0]);
    EXPECT_EQ(2, destImageBGR.at<Vec4b>(0)[1]);
    EXPECT_EQ(3, destImageBGR.at<Vec4b>(0)[2]);
    EXPECT_EQ(4, destImageBGR.at<Vec4b>(0)[3]);

    EXPECT_EQ(1, destImageBGR.at<Vec4b>(height-1, width-1)[0]);
    EXPECT_EQ(2, destImageBGR.at<Vec4b>(height-1, width-1)[1]);
    EXPECT_EQ(3, destImageBGR.at<Vec4b>(height-1, width-1)[2]);
    EXPECT_EQ(4, destImageBGR.at<Vec4b>(height-1, width-1)[3]);
}
#endif

TEST(Reduce, regression_should_fail_bug_4594)
{
    img_t src = img_t::eye(4, 4, CC_8U);
    std::vector<int> dst;

    EXPECT_THROW(reduce(src, dst, 0, CC_REDUCE_MIN, CC_32S), Exception);
    EXPECT_THROW(reduce(src, dst, 0, CC_REDUCE_MAX, CC_32S), Exception);
    EXPECT_NO_THROW(reduce(src, dst, 0, CC_REDUCE_SUM, CC_32S));
    EXPECT_NO_THROW(reduce(src, dst, 0, CC_REDUCE_AVG, CC_32S));
}

TEST(img_t, push_back_vector)
{
    img_t result(1, 5, CC_32FC1);

    std::vector<float> vec1(result->cols + 1);
    std::vector<int> vec2(result->cols);

    EXPECT_THROW(result.push_back(vec1), Exception);
    EXPECT_THROW(result.push_back(vec2), Exception);

    vec1.resize(result->cols);

    for (int i = 0; i < 5; ++i)
        result.push_back(img_t(vec1) cvReshape(,1, 1));

    ASSERT_EQ(6, result->rows);
}

TEST(img_t, regression_5917_clone_empty)
{
    img_t cloned;
    Mat_<CPoint2f> source(5, 0);

    ASSERT_NO_THROW(cloned = source.clone());
}

TEST(img_t, regression_7873_mat_vector_initialize)
{
    std::vector<int> dims;
    dims.push_back(12);
    dims.push_back(3);
    dims.push_back(2);
    img_t multi_mat(dims, CC_32FC1, Scalar(0));

    ASSERT_EQ(3, multi_mat.dims);
    ASSERT_EQ(12, multi_mat.size[0]);
    ASSERT_EQ(3, multi_mat.size[1]);
    ASSERT_EQ(2, multi_mat.size[2]);

    std::vector<CRange> ranges;
    ranges.push_back(CRange(1, 2));
    ranges.push_back(CRange::all());
    ranges.push_back(CRange::all());
    img_t sub_mat = multi_mat(ranges);

    ASSERT_EQ(3, sub_mat.dims);
    ASSERT_EQ(1, sub_mat.size[0]);
    ASSERT_EQ(3, sub_mat.size[1]);
    ASSERT_EQ(2, sub_mat.size[2]);
}

#endif
