
#include <algorithm>
//#include <algorithm>
//#include <iterator>
//#include <limits>

//! type of the robust estimation algorithm
enum { LMEDS  = 4, //!< least-median algorithm
       RANSAC = 8, //!< RANSAC algorithm
       RHO    = 16 //!< RHO algorithm
     };


static int RANSACUpdateNumIters( double p, double ep, int modelPoints, int maxIters )
{
    if( modelPoints <= 0 )
        CC_Error( CC_StsOutOfRange, "the number of model points should be positive" );

    p = MAX(p, 0.);
    p = MIN(p, 1.);
    ep = MAX(ep, 0.);
    ep = MIN(ep, 1.);

    // avoid inf's & nan's
    double num = MAX(1. - p, DBL_MIN);
    double denom = 1. - pow(1. - ep, modelPoints);
    if( denom < DBL_MIN )
        return 0;

    num = log(num);
    denom = log(denom);

    return denom >= 0 || -num >= maxIters*(-denom) ? maxIters : cRound(num/denom);
}

class RANSACPointSetRegistrator : public PointSetRegistrator
{
public:
    RANSACPointSetRegistrator(const PointSetRegistrator::Callback* _cb=NULL,
                              int _modelPoints=0, double _threshold=0, double _confidence=0.99, int _maxIters=1000)
    : cb(_cb), modelPoints(_modelPoints), threshold(_threshold), confidence(_confidence), maxIters(_maxIters)
    {
        checkPartialSubsets = false;
    }

    int findInliers( const img_t* m1, const img_t* m2, const img_t* model, img_t* err, img_t* mask, double thresh ) const
    {
        cb->computeError( m1, m2, model, err );
        cvSetMat(mask, err->h, err->w, CC_8U);

        
        CC_Assert( CC_IS_CONT_MAT(err) && err->tid == CC_32F && CC_IS_CONT_MAT(mask) && mask->tid == CC_8U);
        const float* errptr = img_ptr(float, err);
        uchar* maskptr = img_ptr(uchar, mask);
        float t = (float)(thresh*thresh);
        int i, n = (int)CC_MAT_TOTAL(err), nz = 0;
        for( i = 0; i < n; i++ )
        {
            int f = errptr[i] <= t;
            maskptr[i] = (uchar)f;
            nz += f;
        }
        return nz;
    }

    bool getSubset( const img_t* m1, const img_t* m2,
                    img_t* ms1, img_t* ms2, CRNG* rng,
                    int maxAttempts=1000 ) const
    {
        int* _idx = MALLOC(int, modelPoints);
        int* idx = _idx;
        int i = 0, j, k, iters = 0;
        int d1 = CC_MAT_CN(m1) > 1 ? CC_MAT_CN(m1) : m1->cols;
        int d2 = CC_MAT_CN(m2) > 1 ? CC_MAT_CN(m2) : m2->cols;
        int esz1 = (int)CC_ELEMTYPE_SIZE(m1)*d1, esz2 = (int)CC_ELEMTYPE_SIZE(m2)*d2;
        int count = cvCheckVector(m1, d1), count2 = cvCheckVector(m2, d2);
        const int *m1ptr = m1->tt.i, *m2ptr = m2->tt.i;

        cvSetMat(ms1, modelPoints, 1, CC_MAT_DEPTH(m1), d1);
        cvSetMat(ms2, modelPoints, 1, CC_MAT_DEPTH(m2), d2);

        int *ms1ptr = ms1->tt.i, *ms2ptr = ms2->tt.i;

        CC_Assert( count >= modelPoints && count == count2 );
        CC_Assert( (esz1 % sizeof(int)) == 0 && (esz2 % sizeof(int)) == 0 );
        esz1 /= sizeof(int);
        esz2 /= sizeof(int);

        for(; iters < maxAttempts; iters++)
        {
            for( i = 0; i < modelPoints && iters < maxAttempts; )
            {
                int idx_i = 0;
                for(;;)
                {
                    idx_i = idx[i] = RNG_uniformi(rng, 0, count);
                    for( j = 0; j < i; j++ )
                        if( idx_i == idx[j] )
                            break;
                    if( j == i )
                        break;
                }
                for( k = 0; k < esz1; k++ )
                    ms1ptr[i*esz1 + k] = m1ptr[idx_i*esz1 + k];
                for( k = 0; k < esz2; k++ )
                    ms2ptr[i*esz2 + k] = m2ptr[idx_i*esz2 + k];
                if( checkPartialSubsets && !cb->checkSubset( ms1, ms2, i+1 ))
                {
                    // we may have selected some bad points;
                    // so, let's remove some of them randomly
                    i = RNG_uniformi(rng, 0, i+1);
                    iters++;
                    continue;
                }
                i++;
            }
            if( !checkPartialSubsets && i == modelPoints && !cb->checkSubset(ms1, ms2, i))
                continue;
            break;
        }

        FREE(_idx);
        return i == modelPoints && iters < maxAttempts;
    }

    bool run(const img_t* _m1, const img_t* _m2, img_t* _model, img_t* _mask) const
    {
        bool result = false;
        IM_BEGIN(im, 20);
        const img_t *m1 = _m1, *m2 = _m2;
        img_t *err=im++, *mask=im++, *model=im++, *bestModel=im++, *ms1=im++, *ms2=im++;

        int iter, niters = MAX(maxIters, 1);
        int d1 = CC_MAT_CN(m1) > 1 ? CC_MAT_CN(m1) : m1->cols;
        int d2 = CC_MAT_CN(m2) > 1 ? CC_MAT_CN(m2) : m2->cols;
        int count = cvCheckVector(m1, d1), count2 = cvCheckVector(m2, d2), maxGoodCount = 0;

        CRNG rng = ((uint64)-1);

        CC_Assert( cb );
        CC_Assert( confidence > 0 && confidence < 1 );

        CC_Assert( count >= 0 && count2 == count );
        if( count < modelPoints )
            return false;

        img_t *bestMask0 = NULL, *bestMask = NULL;

        if( _mask )
        {
            cvSetMat(_mask, count, 1, CC_8U);
            bestMask0 = bestMask = _mask;
            CC_Assert( (bestMask->cols == 1 || bestMask->rows == 1) && (int)CC_MAT_TOTAL(bestMask) == count );
        }
        else
        {
            bestMask = cvSetMat(im++, count, 1, CC_8U);
            bestMask0 = bestMask;
        }

        if( count == modelPoints )
        {
            bestModel = im++;
            if( cb->runKernel(m1, m2, bestModel) <= 0 )
                return false;
            cvCopyTo(bestModel, _model);
            cvSetReal(bestMask, 1);
            return true;
        }

        for( iter = 0; iter < niters; iter++ )
        {
            int i, nmodels;
            if( count > modelPoints )
            {
                bool found = getSubset( m1, m2, ms1, ms2, &rng, 10000 );
                if( !found )
                {
                    if( iter == 0 )
                        return false;
                    break;
                }
            }

            nmodels = cb->runKernel( ms1, ms2, model );
            if( nmodels <= 0 )
                continue;
            CC_Assert( model->rows % nmodels == 0 );
            CSize modelSize = {model->cols, model->rows/nmodels};

            for( i = 0; i < nmodels; i++ )
            {
                img_t model_i[1] = {0};
                cvGetRows( model, model_i, i*modelSize.height, (i+1)*modelSize.height );
                int goodCount = findInliers( m1, m2, model_i, err, mask, threshold );

                if( goodCount > MAX(maxGoodCount, modelPoints-1) )
                {
                    T_SWAP(img_t*, mask, bestMask);
                    cvCopyTo(model_i, bestModel);
                    maxGoodCount = goodCount;
                    niters = RANSACUpdateNumIters( confidence, (double)(count - goodCount)/count, modelPoints, niters );
                }
            }
        }

        if( maxGoodCount > 0 )
        {
            if( bestMask != bestMask0 )
            {
                if( CC_ARE_SIZES_EQ(bestMask, bestMask0) )
                    cvCopyTo(bestMask, bestMask0);
                else
                    cvTranspose(bestMask, bestMask0);
            }
            cvCopyTo(bestModel, _model);
            result = true;
        }
        else {
            imfree(_model);
        }

        IM_END(im);
        return result;
    }

    void setCallback(const PointSetRegistrator::Callback* _cb) { cb = _cb; }

    const PointSetRegistrator::Callback* cb;
    int modelPoints;
    bool checkPartialSubsets;
    double threshold;
    double confidence;
    int maxIters;
};

class LMeDSPointSetRegistrator : public RANSACPointSetRegistrator
{
public:
    LMeDSPointSetRegistrator(const PointSetRegistrator::Callback* _cb=NULL,
                              int _modelPoints=0, double _confidence=0.99, int _maxIters=1000)
    : RANSACPointSetRegistrator(_cb, _modelPoints, 0, _confidence, _maxIters) {}

    bool run(const img_t* _m1, const img_t* _m2, img_t* _model, img_t* _mask) const
    {
        bool result = false;
        IM_BEGIN(im, 30);
        const double outlierRatio = 0.45;
        const img_t *m1 = _m1, *m2 = _m2;
        img_t *ms1 = im++, *ms2 = im++, *err = im++, *errf = im++, *model = im++, *bestModel = im++, *mask = im++, *mask0 = im++;

        int d1 = CC_MAT_CN(m1) > 1 ? CC_MAT_CN(m1) : m1->cols;
        int d2 = CC_MAT_CN(m2) > 1 ? CC_MAT_CN(m2) : m2->cols;
        int count = cvCheckVector(m1, d1), count2 = cvCheckVector(m2, d2);
        double minMedian = DBL_MAX;

        CRNG rng = ((uint64)-1);

        CC_Assert( cb );
        CC_Assert( confidence > 0 && confidence < 1 );

        CC_Assert( count >= 0 && count2 == count );
        if( count < modelPoints )
            return false;

        if( _mask )
        {
            cvSetMat(_mask, count, 1, CC_8U);
            mask0 = mask = _mask;
            CC_Assert( (mask->cols == 1 || mask->rows == 1) && (int)CC_MAT_TOTAL(mask) == count );
        }

        if( count == modelPoints )
        {
            bestModel = im++;
            if( cb->runKernel(m1, m2, bestModel) <= 0 )
                return false;
            cvCopyTo(bestModel, _model);
            cvSetReal(mask, 1);
            return true;
        }

        int iter, niters = RANSACUpdateNumIters(confidence, outlierRatio, modelPoints, maxIters);
        niters = MAX(niters, 3);

        for( iter = 0; iter < niters; iter++ )
        {
            int i, nmodels;
            if( count > modelPoints )
            {
                bool found = getSubset( m1, m2, ms1, ms2, &rng );
                if( !found )
                {
                    if( iter == 0 )
                        return false;
                    break;
                }
            }

            nmodels = cb->runKernel( ms1, ms2, model );

            if( nmodels <= 0 )
                continue;

            CC_Assert( model->rows % nmodels == 0 );
            CSize modelSize = {model->cols, model->rows/nmodels};

            for( i = 0; i < nmodels; i++ )
            {
                img_t model_i[1] = {0};
                cvGetRows( model, model_i, i*modelSize.height, (i+1)*modelSize.height );
                cb->computeError( m1, m2, model_i, err );
                if( CC_MAT_DEPTH(err) != CC_32F )
                    cvConvertTo(err, errf, CC_32F);
                else
                    errf = err;
                CC_Assert( CC_IS_CONT_MAT(errf) && errf->tid == CC_32F && (int)CC_MAT_TOTAL(errf) == count );
                std::nth_element(errf->tt.i, errf->tt.i + count/2, errf->tt.i + count);
                double median = img_ptr(float, errf)[count/2];

                if( median < minMedian )
                {
                    minMedian = median;
                    cvCopyTo(model_i, bestModel);
                }
            }
        }

        if( minMedian < DBL_MAX )
        {
            double sigma = 2.5*1.4826*(1 + 5./(count - modelPoints))*sqrt(minMedian);
            sigma = MAX( sigma, 0.001 );

            count = findInliers( m1, m2, bestModel, err, mask, sigma );
            if( _mask && mask0 != mask )
            {
                if( CC_ARE_SIZES_EQ(mask0, mask) )
                    cvCopyTo(mask, mask0);
                else
                    transpose(mask, mask0);
            }
            cvCopyTo(bestModel, _model);
            result = count >= modelPoints;
        }
        else
            imfree(_model);

        IM_END(im);
        return result;
    }

};

static PointSetRegistrator*
createRANSACPointSetRegistrator(const PointSetRegistrator::Callback* _cb,
                                int _modelPoints, double _threshold,
                                double _confidence, int _maxIters)
{
    return new RANSACPointSetRegistrator(_cb, _modelPoints, _threshold, _confidence, _maxIters);
}


static PointSetRegistrator*
createLMeDSPointSetRegistrator(const PointSetRegistrator::Callback* _cb,
                               int _modelPoints, double _confidence, int _maxIters)
{
    return new LMeDSPointSetRegistrator(_cb, _modelPoints, _confidence, _maxIters);
}


class Affine3DEstimatorCallback : public PointSetRegistrator::Callback
{
public:
    int runKernel( const img_t* _m1, const img_t* _m2, img_t* _model ) const
    {
        const img_t* m1 = _m1, *m2 = _m2;
        const CPoint3f* from = (CPoint3f*)(m1->tt.data);
        const CPoint3f* to   = (CPoint3f*)(m2->tt.data);

        const int N = 12;
        double buf[N*N + N + N];
        img_t A = cvMat1(N, N, CC_64F, 1, &buf[0]);
        img_t B = cvMat1(N, 1, CC_64F, 1, &buf[0] + N*N);
        img_t X = cvMat1(N, 1, CC_64F, 1, &buf[0] + N*N + N);
        double* Adata = img_ptr(double, &A);
        double* Bdata = img_ptr(double, &B);
        // A = Scalar::all(0);
        cvZeroMat(&A);

        for( int i = 0; i < (N/3); i++ )
        {
            Bdata[i*3] = to[i].x;
            Bdata[i*3+1] = to[i].y;
            Bdata[i*3+2] = to[i].z;

            double *aptr = Adata + i*3*N;
            for(int k = 0; k < 3; ++k)
            {
                aptr[0] = from[i].x;
                aptr[1] = from[i].y;
                aptr[2] = from[i].z;
                aptr[3] = 1.0;
                aptr += 16;
            }
        }

        solve(&A, &B, &X, CC_DECOMP_SVD);
        IM_DEF(tmp, 4);
        cvCopyTo(cvReshape(&X, tmp, 1, 3), _model);

        return 1;
    }

    void computeError( const img_t* _m1, const img_t* _m2, const img_t* _model, img_t* _err ) const
    {
        const img_t *m1 = _m1, *m2 = _m2, *model = _model;
        const CPoint3f* from = (CPoint3f*)(m1->tt.data);
        const CPoint3f* to   = (CPoint3f*)(m2->tt.data);
        const double* F = img_ptr(double, model);

        int count = cvCheckVector(m1, 3);
        CC_Assert( count > 0 );

        cvSetMat(_err, count, 1, CC_32F);
        img_t *err = _err;
        float* errptr = img_ptr(float, err);

        for(int i = 0; i < count; i++ )
        {
            const CPoint3f* f = from+i;
            const CPoint3f* t = to+i;

            double a = F[0]*f->x + F[1]*f->y + F[ 2]*f->z + F[ 3] - t->x;
            double b = F[4]*f->x + F[5]*f->y + F[ 6]*f->z + F[ 7] - t->y;
            double c = F[8]*f->x + F[9]*f->y + F[10]*f->z + F[11] - t->z;

            errptr[i] = (float)(a*a + b*b + c*c);
        }
    }

    bool checkSubset( const img_t* _ms1, const img_t* _ms2, int count ) const
    {
        const float threshold = 0.996f;
        const img_t *ms1 = _ms1, *ms2 = _ms2;

        for( int inp = 1; inp <= 2; inp++ )
        {
            int j, k, i = count - 1;
            const img_t* msi = inp == 1 ? ms1 : ms2;
            const CPoint3f* ptr = (CPoint3f*)(msi->tt.data);

            CC_Assert( count <= msi->rows );
            // check that the i-th selected point does not belong
            // to a line connecting some previously selected points
            for(j = 0; j < i; ++j)
            {
                CPoint3f d1 = {PT3OPDEF(ptr[j], -, ptr[i])};
                float n1 = d1.x*d1.x + d1.y*d1.y;

                for(k = 0; k < j; ++k)
                {
                    CPoint3f d2 = {PT3OPDEF(ptr[k], -, ptr[i])};
                    float denom = (d2.x*d2.x + d2.y*d2.y)*n1;
                    float num = d1.x*d2.x + d1.y*d2.y;

                    if( num*num > threshold*threshold*denom )
                        return false;
                }
            }
        }
        return true;
    }
};

class Affine2DEstimatorCallback : public PointSetRegistrator::Callback
{
public:
    int runKernel( const img_t* _m1, const img_t* _m2, img_t* _model ) const
    {
        const img_t *m1 = _m1, *m2 = _m2;
        const CPoint2f* from = (CPoint2f*)(m1->tt.data);
        const CPoint2f* to   = (CPoint2f*)(m2->tt.data);
        cvSetMat(_model, 2, 3, CC_64F);
        img_t *M_mat = _model;
        double *M = img_ptr(double, M_mat);

        // we need 3 points to estimate affine transform
        double x1 = from[0].x;
        double y1 = from[0].y;
        double x2 = from[1].x;
        double y2 = from[1].y;
        double x3 = from[2].x;
        double y3 = from[2].y;

        double X1 = to[0].x;
        double Y1 = to[0].y;
        double X2 = to[1].x;
        double Y2 = to[1].y;
        double X3 = to[2].x;
        double Y3 = to[2].y;

        /*
        We want to solve AX = B

            | x1 y1  1  0  0  0 |
            |  0  0  0 x1 y1  1 |
            | x2 y2  1  0  0  0 |
        A = |  0  0  0 x2 y2  1 |
            | x3 y3  1  0  0  0 |
            |  0  0  0 x3 y3  1 |
        B = (X1, Y1, X2, Y2, X3, Y3).t()
        X = (a, b, c, d, e, f).t()

        As the estimate of (a, b, c) only depends on the Xi, and (d, e, f) only
        depends on the Yi, we do the *trick* to solve each one analytically.

        | X1 |   | x1 y1 1 |   | a |
        | X2 | = | x2 y2 1 | * | b |
        | X3 |   | x3 y3 1 |   | c |

        | Y1 |   | x1 y1 1 |   | d |
        | Y2 | = | x2 y2 1 | * | e |
        | Y3 |   | x3 y3 1 |   | f |
        */

        double d = 1. / ( x1*(y2-y3) + x2*(y3-y1) + x3*(y1-y2) );

        M[0] = d * ( X1*(y2-y3) + X2*(y3-y1) + X3*(y1-y2) );
        M[1] = d * ( X1*(x3-x2) + X2*(x1-x3) + X3*(x2-x1) );
        M[2] = d * ( X1*(x2*y3 - x3*y2) + X2*(x3*y1 - x1*y3) + X3*(x1*y2 - x2*y1) );

        M[3] = d * ( Y1*(y2-y3) + Y2*(y3-y1) + Y3*(y1-y2) );
        M[4] = d * ( Y1*(x3-x2) + Y2*(x1-x3) + Y3*(x2-x1) );
        M[5] = d * ( Y1*(x2*y3 - x3*y2) + Y2*(x3*y1 - x1*y3) + Y3*(x1*y2 - x2*y1) );
        return 1;
    }

    void computeError( const img_t* _m1, const img_t* _m2, const img_t* _model, img_t* _err ) const
    {
        const img_t *m1 = _m1, *m2 = _m2, *model = _model;
        const CPoint2f* from = (CPoint2f*)(m1->tt.data);
        const CPoint2f* to   = (CPoint2f*)(m2->tt.data);
        const double* F = img_ptr(double, model);

        int count = cvCheckVector(m1, 2);
        CC_Assert( count > 0 );

        cvSetMat(_err, count, 1, CC_32F);
        img_t *err = _err;
        float* errptr = img_ptr(float, err);
        // transform matrix to floats
        float F0 = (float)F[0], F1 = (float)F[1], F2 = (float)F[2];
        float F3 = (float)F[3], F4 = (float)F[4], F5 = (float)F[5];

        for(int i = 0; i < count; i++ )
        {
            const CPoint2f& f = from[i];
            const CPoint2f& t = to[i];

            float a = F0*f.x + F1*f.y + F2 - t.x;
            float b = F3*f.x + F4*f.y + F5 - t.y;

            errptr[i] = a*a + b*b;
        }
    }

    bool checkSubset( const img_t* _ms1, const img_t*, int count ) const
    {
        const img_t *ms1 = _ms1;
        // check colinearity and also check that points are too close
        // only ms1 affects actual estimation stability
        return !haveCollinearPoints(ms1, count);
    }
};

class AffinePartial2DEstimatorCallback : public Affine2DEstimatorCallback
{
public:
    int runKernel( const img_t* _m1, const img_t* _m2, img_t* _model ) const
    {
        const img_t *m1 = _m1, *m2 = _m2;
        const CPoint2f* from = (CPoint2f*)(m1->tt.data);
        const CPoint2f* to   = (CPoint2f*)(m2->tt.data);
        cvSetMat(_model, 2, 3, CC_64F);
        img_t *M_mat = _model;
        double *M = img_ptr(double, M_mat);

        // we need only 2 points to estimate transform
        double x1 = from[0].x;
        double y1 = from[0].y;
        double x2 = from[1].x;
        double y2 = from[1].y;

        double X1 = to[0].x;
        double Y1 = to[0].y;
        double X2 = to[1].x;
        double Y2 = to[1].y;

        /*
        we are solving AS = B
            | x1 -y1 1 0 |
            | y1  x1 0 1 |
        A = | x2 -y2 1 0 |
            | y2  x2 0 1 |
        B = (X1, Y1, X2, Y2).t()
        we solve that analytically
        */
        double d = 1./((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));

        // solution vector
        double S0 = d * ( (X1-X2)*(x1-x2) + (Y1-Y2)*(y1-y2) );
        double S1 = d * ( (Y1-Y2)*(x1-x2) - (X1-X2)*(y1-y2) );
        double S2 = d * ( (Y1-Y2)*(x1*y2 - x2*y1) - (X1*y2 - X2*y1)*(y1-y2) - (X1*x2 - X2*x1)*(x1-x2) );
        double S3 = d * (-(X1-X2)*(x1*y2 - x2*y1) - (Y1*x2 - Y2*x1)*(x1-x2) - (Y1*y2 - Y2*y1)*(y1-y2) );

        // set model, rotation part is antisymmetric
        M[0] = M[4] = S0;
        M[1] = -S1;
        M[2] = S2;
        M[3] = S1;
        M[5] = S3;
        return 1;
    }
};

class Affine2DRefineCallback : public LMSolver::Callback
{
public:
    Affine2DRefineCallback(const img_t* _src, const img_t* _dst)
    {
        src = _src;
        dst = _dst;
    }

    bool compute(const img_t* _param, img_t* _err, img_t* _Jac) const
    {
        int i, count = cvCheckVector(src, 2);
        const img_t *param = _param;
        cvSetMat(_err, count*2, 1, CC_64F);
        img_t *err = _err, *J = NULL;
        if( _Jac )
        {
            cvSetMat(_Jac, count*2, param->rows, CC_64F);
            J = _Jac;
            CC_Assert( CC_IS_CONT_MAT(J) && J->cols == 6 );
        }

        const CPoint2f* M = (CPoint2f*)(src->tt.data);
        const CPoint2f* m = (CPoint2f*)(dst->tt.data);
        const double* h = img_ptr(double, param);
        double* errptr = img_ptr(double, err);
        double* Jptr = J ? img_ptr(double, J) : 0;

        for( i = 0; i < count; i++ )
        {
            double Mx = M[i].x, My = M[i].y;
            double xi = h[0]*Mx + h[1]*My + h[2];
            double yi = h[3]*Mx + h[4]*My + h[5];
            errptr[i*2] = xi - m[i].x;
            errptr[i*2+1] = yi - m[i].y;

            /*
            Jacobian should be:
                {x, y, 1, 0, 0, 0}
                {0, 0, 0, x, y, 1}
            */
            if( Jptr )
            {
                Jptr[0] = Mx; Jptr[1] = My; Jptr[2] = 1.;
                Jptr[3] = Jptr[4] = Jptr[5] = 0.;
                Jptr[6] = Jptr[7] = Jptr[8] = 0.;
                Jptr[9] = Mx; Jptr[10] = My; Jptr[11] = 1.;

                Jptr += 6*2;
            }
        }

        return true;
    }

    const img_t *src;
    const img_t *dst;
};

class AffinePartial2DRefineCallback : public LMSolver::Callback
{
public:
    AffinePartial2DRefineCallback(const img_t* _src, const img_t* _dst)
    {
        src = _src;
        dst = _dst;
    }

    bool compute(const img_t* _param, img_t* _err, img_t* _Jac) const
    {
        int i, count = cvCheckVector(src, 2);
        const img_t *param = _param;
        cvSetMat(_err, count*2, 1, CC_64F);
        img_t *err = _err, *J = NULL;
        if( _Jac )
        {
            cvSetMat(_Jac, count*2, param->rows, CC_64F);
            J = _Jac;
            CC_Assert( CC_IS_CONT_MAT(J) && J->cols == 4 );
        }

        const CPoint2f* M = (CPoint2f*)(src->tt.data);
        const CPoint2f* m = (CPoint2f*)(dst->tt.data);
        const double* h = img_ptr(double, param);
        double* errptr = img_ptr(double, err);
        double* Jptr = J ? img_ptr(double, J) : 0;

        for( i = 0; i < count; i++ )
        {
            double Mx = M[i].x, My = M[i].y;
            double xi = h[0]*Mx - h[1]*My + h[2];
            double yi = h[1]*Mx + h[0]*My + h[3];
            errptr[i*2] = xi - m[i].x;
            errptr[i*2+1] = yi - m[i].y;

            /*
            Jacobian should be:
                {x, -y, 1, 0}
                {y,  x, 0, 1}
            */
            if( Jptr )
            {
                Jptr[0] = Mx; Jptr[1] = -My; Jptr[2] = 1.; Jptr[3] = 0.;
                Jptr[4] = My; Jptr[5] =  Mx; Jptr[6] = 0.; Jptr[7] = 1.;

                Jptr += 4*2;
            }
        }

        return true;
    }

    const img_t *src;
    const img_t *dst;
};

static int estimateAffine3D(const img_t* _from, const img_t* _to,
                     img_t* _out, img_t* _inliers,
                     double param1, double param2)
{
    CC_INSTRUMENT_REGION()
    int ret = 0;
    IM_BEGIN(im, 10);
    const img_t *from = _from, *to = _to;
    int count = cvCheckVector(from, 3);

    CC_Assert( count >= 0 && cvCheckVector(to, 3) == count );

    img_t *dFrom = cvConvertTo(from, im++, CC_32F);
    img_t *dTo = cvConvertTo(to, im++, CC_32F);
    cvReshape(dFrom, dFrom, 3, count);
    cvReshape(dTo, dTo, 3, count);

    const double epsilon = DBL_EPSILON;
    param1 = param1 <= 0 ? 3 : param1;
    param2 = (param2 < epsilon) ? 0.99 : (param2 > 1 - epsilon) ? 0.99 : param2;

    Affine3DEstimatorCallback ad[1];
    ret = createRANSACPointSetRegistrator(ad, 4, param1, param2)->run(dFrom, dTo, _out, _inliers);
    IM_END(im);
    return ret;
}

static img_t* estimateAffine2D(img_t* H, const img_t* _from, const img_t* _to, img_t* _inliers,
                     const int method, const double ransacReprojThreshold,
                     const size_t maxIters, const double confidence,
                     const size_t refineIters)
{
    IM_BEGIN(im, 10);
    const img_t *from = _from, *to = _to;
    int count = cvCheckVector(from, 2);
    bool result = false;
    IM_DEF(tmp, 10);

    CC_Assert( count >= 0 && cvCheckVector(to, 2) == count );

    if (from->tid != CC_32F || to->tid != CC_32F)
    {
        from = cvConvertTo(from, im++, CC_32F);
        to = cvConvertTo(to, im++, CC_32F);
    }
    // convert to N x 1 vectors
    from = cvReshape(from, tmp++, 2, count);
    to = cvReshape(to, tmp++, 2, count);

    img_t *inliers = NULL;
    if(_inliers)
    {
        inliers = cvSetMat(_inliers, count, 1, CC_8U);
    }

    // run robust method
    Affine2DEstimatorCallback cb_;
    PointSetRegistrator::Callback* cb = &cb_;
    if( method == RANSAC )
        result = createRANSACPointSetRegistrator(cb, 3, ransacReprojThreshold, confidence, (int)(maxIters))->run(from, to, H, inliers);
    else if( method == LMEDS )
        result = createLMeDSPointSetRegistrator(cb, 3, confidence, (int)(maxIters))->run(from, to, H, inliers);
    else
        CC_Error(CC_StsBadArg, "Unknown or unsupported robust estimation method");

    if(result && count > 3 && refineIters)
    {
        // reorder to start with inliers
        compressElems((CPoint2f*)(from->tt.data), img_ptr(uchar, inliers), 1, count);
        int inliers_count = compressElems((CPoint2f*)(to->tt.data), img_ptr(uchar, inliers), 1, count);
        if(inliers_count > 0)
        {
            img_t src[1]; cvGetRows(from, src, 0, inliers_count);
            img_t dst[1]; cvGetRows(to, dst, 0, inliers_count);
            img_t Hvec[1]; cvReshape(H, Hvec, 1, 6);
            Affine2DRefineCallback ad(src, dst);
            createLMSolver(&ad, (int)(refineIters))->run(Hvec);
        }
    }

    if (!result)
    {
        imfree(H);
        if(_inliers )
        {
            cvSetZero(inliers, count, 1, CC_8U);
            cvCopyTo(inliers, _inliers);
        }
    }

    IM_END(im);
    return H;
}

static img_t* estimateAffinePartial2D(img_t* H, const img_t* _from, const img_t* _to, img_t* _inliers,
                            const int method, const double ransacReprojThreshold,
                            const size_t maxIters, const double confidence,
                            const size_t refineIters)
{
    IM_BEGIN(im, 10);
    IM_DEF(tmp, 10);
    const img_t *from = _from, *to = _to;
    const int count = cvCheckVector(from, 2);
    bool result = false;

    CC_Assert( count >= 0 && cvCheckVector(to, 2) == count );

    if (from->tid != CC_32F || to->tid != CC_32F)
    {
        from = cvConvertTo(from, tmp++, CC_32F);
        to = cvConvertTo(to, tmp++, CC_32F);
    }
    // convert to N x 1 vectors
    from = cvReshape(from, tmp++, 2, count);
    to = cvReshape(to, tmp++, 2, count);

    img_t *inliers = NULL;
    if(_inliers)
    {
        cvSetMat(_inliers, count, 1, CC_8U);
        inliers = _inliers;
    }

    // run robust estimation
    AffinePartial2DEstimatorCallback cb_;
    PointSetRegistrator::Callback* cb = &cb_;
    if( method == RANSAC )
        result = createRANSACPointSetRegistrator(cb, 2, ransacReprojThreshold, confidence, (int)(maxIters))->run(from, to, H, inliers);
    else if( method == LMEDS )
        result = createLMeDSPointSetRegistrator(cb, 2, confidence, (int)(maxIters))->run(from, to, H, inliers);
    else
        CC_Error(CC_StsBadArg, "Unknown or unsupported robust estimation method");

    if(result && count > 2 && refineIters)
    {
        // reorder to start with inliers
        compressElems((CPoint2f*)(from->tt.data), img_ptr(uchar, inliers), 1, count);
        int inliers_count = compressElems((CPoint2f*)(to->tt.data), img_ptr(uchar, inliers), 1, count);
        if(inliers_count > 0)
        {
            img_t src[1]; cvGetRows(from, src,0, inliers_count);
            img_t dst[1]; cvGetRows(to, dst,0, inliers_count);
            // H is
            //     a -b tx
            //     b  a ty
            // Hvec model for LevMarq is
            //     (a, b, tx, ty)
            double *Hptr = img_ptr(double, H);
            double Hvec_buf[4] = {Hptr[0], Hptr[3], Hptr[2], Hptr[5]};
            img_t Hvec = cvMat1(4, 1, CC_64F, 1, Hvec_buf);
            AffinePartial2DRefineCallback ad(src, dst);
            createLMSolver(&ad, (int)(refineIters))->run(&Hvec);
            // update H with refined parameters
            Hptr[0] = Hptr[4] = Hvec_buf[0];
            Hptr[1] = -Hvec_buf[1];
            Hptr[2] = Hvec_buf[2];
            Hptr[3] = Hvec_buf[1];
            Hptr[5] = Hvec_buf[3];
        }
    }

    if (!result)
    {
        imfree(H);
        if(_inliers)
        {
            cvSetZero(inliers, count, 1, CC_8U);
            cvCopyTo(inliers, _inliers);
        }
    }

    IM_END(im);
    return H;
}



#if 0

#endif
