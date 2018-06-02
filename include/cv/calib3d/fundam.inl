
#include "rho.inl"
//#include "rho.h"

//! the algorithm for finding fundamental matrix
enum { FM_7POINT = 1, //!< 7-point algorithm
       FM_8POINT = 2, //!< 8-point algorithm
       FM_LMEDS  = 4, //!< least-median algorithm
       FM_RANSAC = 8  //!< RANSAC algorithm
     };



class HomographyEstimatorCallback : public PointSetRegistrator::Callback
{
public:
    bool checkSubset( const img_t* ms1, const img_t* ms2, int count ) const
    {
        if( haveCollinearPoints(ms1, count) || haveCollinearPoints(ms2, count) )
            return false;

        // We check whether the minimal set of points for the homography estimation
        // are geometrically consistent. We check if every 3 correspondences sets
        // fulfills the constraint.
        //
        // The usefullness of this constraint is explained in the paper:
        //
        // "Speeding-up homography estimation in mobile devices"
        // Journal of Real-Time Image Processing. 2013. DOI: 10.1007/s11554-012-0314-1
        // Pablo Marquez-Neila, Javier Lopez-Alberca, Jose M. Buenaposada, Luis Baumela
        if( count == 4 )
        {
            static const int tt[][3] = {{0, 1, 2}, {1, 2, 3}, {0, 2, 3}, {0, 1, 3}};
            const CPoint2f* src = (CPoint2f*)ms1->tt.data;
            const CPoint2f* dst = (CPoint2f*)ms2->tt.data;
            int negative = 0;

            for( int i = 0; i < 4; i++ )
            {
                const int* t = tt[i];
                double9 A = {src[t[0]].x, src[t[0]].y, 1., src[t[1]].x, src[t[1]].y, 1., src[t[2]].x, src[t[2]].y, 1.};
                double9 B = {dst[t[0]].x, dst[t[0]].y, 1., dst[t[1]].x, dst[t[1]].y, 1., dst[t[2]].x, dst[t[2]].y, 1.};

                negative += det3(A)*det3(B) < 0;
            }
            if( negative != 0 && negative != 4 )
                return false;
        }

        return true;
    }

    int runKernel( const img_t* m1, const img_t* m2, img_t* model ) const
    {
        int i, count = cvCheckVector(m1, 2);
        const CPoint2f* M = (CPoint2f*)(m1->tt.data);
        const CPoint2f* m = (CPoint2f*)(m2->tt.data);

        double LtL[9][9] = {0}, W[9][1] = {0}, V[9][9] = {0};
        img_t _LtL = cvMat1( 9, 9, CC_64F, 1, &LtL[0][0] );
        img_t matW = cvMat1( 9, 1, CC_64F, 1, W );
        img_t matV = cvMat1( 9, 9, CC_64F, 1, V );
        img_t _H0 = cvMat1( 3, 3, CC_64F, 1, V[8] );
        img_t _Htemp = cvMat1( 3, 3, CC_64F, 1, V[7] );
        CPoint2d cM={0,0}, cm = {0,0}, sM = {0,0}, sm = {0,0};

        for( i = 0; i < count; i++ )
        {
            cm.x += m[i].x; cm.y += m[i].y;
            cM.x += M[i].x; cM.y += M[i].y;
        }

        cm.x /= count;
        cm.y /= count;
        cM.x /= count;
        cM.y /= count;

        for( i = 0; i < count; i++ )
        {
            sm.x += fabs(m[i].x - cm.x);
            sm.y += fabs(m[i].y - cm.y);
            sM.x += fabs(M[i].x - cM.x);
            sM.y += fabs(M[i].y - cM.y);
        }

        if( fabs(sm.x) < DBL_EPSILON || fabs(sm.y) < DBL_EPSILON ||
            fabs(sM.x) < DBL_EPSILON || fabs(sM.y) < DBL_EPSILON )
            return 0;
        sm.x = count/sm.x; sm.y = count/sm.y;
        sM.x = count/sM.x; sM.y = count/sM.y;

        double invHnorm[9] = { 1./sm.x, 0, cm.x, 0, 1./sm.y, cm.y, 0, 0, 1 };
        double Hnorm2[9] = { sM.x, 0, -cM.x*sM.x, 0, sM.y, -cM.y*sM.y, 0, 0, 1 };
        img_t _invHnorm = cvMat1( 3, 3, CC_64F, 1, invHnorm );
        img_t _Hnorm2 = cvMat1( 3, 3, CC_64F, 1, Hnorm2 );

        cvSetReal(&_LtL, 0);
        for( i = 0; i < count; i++ )
        {
            double x = (m[i].x - cm.x)*sm.x, y = (m[i].y - cm.y)*sm.y;
            double X = (M[i].x - cM.x)*sM.x, Y = (M[i].y - cM.y)*sM.y;
            double Lx[] = { X, Y, 1, 0, 0, 0, -x*X, -x*Y, -x };
            double Ly[] = { 0, 0, 0, X, Y, 1, -y*X, -y*Y, -y };
            int j, k;
            for( j = 0; j < 9; j++ )
                for( k = j; k < 9; k++ )
                    LtL[j][k] += Lx[j]*Lx[k] + Ly[j]*Ly[k];
        }
        completeSymm( &_LtL );

        eigen( &_LtL, &matW, &matV );
        //cvOutMat(&_LtL);
        //cvOutMat(&matW);
        //cvOutMat(&matV);
        //_Htemp = _invHnorm*_H0;
        //_H0 = _Htemp*_Hnorm2;
        cvMatMul(&_invHnorm, &_H0, &_Htemp);
        cvMatMul(&_Htemp, &_Hnorm2, &_H0);
        cvConvertTo(&_H0, model, _H0.tid, 1./(*img_at(double, &_H0, 2,2)) );

        return 1;
    }

    void computeError( const img_t* m1, const img_t* m2, const img_t* model, img_t* _err ) const
    {
        int i, count = cvCheckVector(m1, 2);
        const CPoint2f* M = (CPoint2f*)(m1->tt.data);
        const CPoint2f* m = (CPoint2f*)(m2->tt.data);
        const double* H = img_ptr(double, model);
        float Hf[] = { (float)H[0], (float)H[1], (float)H[2], (float)H[3], (float)H[4], (float)H[5], (float)H[6], (float)H[7] };

        cvSetMat(_err, count, 1, CC_32F);
        float* err = img_ptr(float, _err);

        for( i = 0; i < count; i++ )
        {
            float ww = 1.f/(Hf[6]*M[i].x + Hf[7]*M[i].y + 1.f);
            float dx = (Hf[0]*M[i].x + Hf[1]*M[i].y + Hf[2])*ww - m[i].x;
            float dy = (Hf[3]*M[i].x + Hf[4]*M[i].y + Hf[5])*ww - m[i].y;
            err[i] = (float)(dx*dx + dy*dy);
        }
    }
};

class HomographyRefineCallback : public LMSolver::Callback
{
public:
    HomographyRefineCallback(const img_t* _src, const img_t* _dst)
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
            CC_Assert( CC_IS_CONT_MAT(J) && J->cols == 8 );
        }

        const CPoint2f* M = (CPoint2f*)(src->tt.data);
        const CPoint2f* m = (CPoint2f*)(dst->tt.data);
        const double* h = img_ptr(double, param);
        double* errptr = img_ptr(double, err);
        double* Jptr = J ? img_ptr(double, J) : 0;

        for( i = 0; i < count; i++ )
        {
            double Mx = M[i].x, My = M[i].y;
            double ww = h[6]*Mx + h[7]*My + 1.;
            ww = fabs(ww) > DBL_EPSILON ? 1./ww : 0;
            double xi = (h[0]*Mx + h[1]*My + h[2])*ww;
            double yi = (h[3]*Mx + h[4]*My + h[5])*ww;
            errptr[i*2] = xi - m[i].x;
            errptr[i*2+1] = yi - m[i].y;

            if( Jptr )
            {
                Jptr[0] = Mx*ww; Jptr[1] = My*ww; Jptr[2] = ww;
                Jptr[3] = Jptr[4] = Jptr[5] = 0.;
                Jptr[6] = -Mx*ww*xi; Jptr[7] = -My*ww*xi;
                Jptr[8] = Jptr[9] = Jptr[10] = 0.;
                Jptr[11] = Mx*ww; Jptr[12] = My*ww; Jptr[13] = ww;
                Jptr[14] = -Mx*ww*yi; Jptr[15] = -My*ww*yi;

                Jptr += 16;
            }
        }

        return true;
    }

    const img_t *src;
    const img_t *dst;
};

static bool createAndRunRHORegistrator(double confidence,
                                       int    maxIters,
                                       double ransacReprojThreshold,
                                       int    npoints,
                                       const img_t*  _src,
                                       const img_t*  _dst,
                                       img_t* _H,
                                       img_t* _tempMask) {
    bool   result;
    IM_BEGIN(im, 10);
    const img_t *src = _src;
    const img_t *dst = _dst;
    img_t *tempMask = NULL;
    double beta = 0.35;/* 0.35 is a value that often works. */

    float tmpH_[9];
    /* Create temporary output matrix (RHO outputs a single-precision H only). */
    img_t tmpH = cvMat1(3, 3, CC_32F, 1, tmpH_);

    /* Create output mask. */
    tempMask = cvSetMat(im++, npoints, 1, CC_8U);

    /**
     * Make use of the RHO estimator API.
     *
     * This is where the math happens. A homography estimation context is
     * initialized, used, then finalized.
     */

    RHO_HEST* p = rhoInit();

    /**
     * Optional. Ideally, the context would survive across calls to
     * findHomography(), but no clean way appears to exit to do so. The price
     * to pay is marginally more computational work than strictly needed.
     */

    rhoEnsureCapacity(p, npoints, beta);

    /**
     * The critical call. All parameters are heavily documented in rhorefc.h.
     *
     * Currently, NR (Non-Randomness criterion) and Final Refinement (with
     * internal, optimized Levenberg-Marquardt method) are enabled. However,
     * while refinement seems to correctly smooth jitter most of the time, when
     * refinement fails it tends to make the estimate visually very much worse.
     * It may be necessary to remove the refinement flags in a future commit if
     * this behaviour is too problematic.
     */

    result = !!rhoHest(p,
                      (const float*)src->data,
                      (const float*)dst->data,
                      (char*)       tempMask->data,
                      (unsigned)    npoints,
                      (float)       ransacReprojThreshold,
                      (unsigned)    maxIters,
                      (unsigned)    maxIters,
                      confidence,
                      4U,
                      beta,
                      RHO_FLAG_ENABLE_NR | RHO_FLAG_ENABLE_FINAL_REFINEMENT,
                      NULL,
                      (float*)tmpH.data);

    /* Convert float homography to double precision. */
    cvConvertTo(&tmpH, _H, CC_64F);

    /* Maps non-zero mask elems to 1, for the sake of the testcase. */
    for(int k=0;k<npoints;k++){
        tempMask->data[k] = !!tempMask->data[k];
    }
    cvCopyTo(tempMask, _tempMask);
    IM_END(im);

    return result;
}


img_t* convertPointsFromHomogeneous( const img_t* _src, img_t* _dst );

static img_t* findHomography( img_t* H, const img_t* _points1, const img_t* _points2,
                            int method, double ransacReprojThreshold = 3, img_t* _mask=NULL,
                            const int maxIters = 2000, const double confidence = 0.995)
{
    CC_INSTRUMENT_REGION()

    IM_BEGIN(im, 10);
    const double defaultRANSACReprojThreshold = 3;
    bool result = false;

    const img_t *points1 = _points1, *points2 = _points2;
    img_t *src = im++, *dst = im++, *tempMask = im++;
    int npoints = -1;
    img_t* im_p = im++;
    IM_DEF(tmp, 10);

    for( int i = 1; i <= 2; i++ )
    {
        const img_t* p = i == 1 ? points1 : points2;
        img_t* m = i == 1 ? src : dst;
        npoints = cvCheckVector(p, 2, CC_NUL, false);
        if( npoints < 0 )
        {
            npoints = cvCheckVector(p, 3, CC_NUL, false);
            if( npoints < 0 )
                CC_Error(CC_StsBadArg, "The input arrays should be 2D or 3D point sets");
            if( npoints == 0 )
                return NULL;
            p = convertPointsFromHomogeneous(p, im_p);
        }
        cvConvertTo(cvReshape(p, tmp, 2, npoints), m, CC_32F);
    }

    CC_Assert( cvCheckVector(src, 2) == cvCheckVector(dst, 2) );

    if( ransacReprojThreshold <= 0 )
        ransacReprojThreshold = defaultRANSACReprojThreshold;

    HomographyEstimatorCallback _cb;
    PointSetRegistrator::Callback* cb = &_cb;

    if( method == 0 || npoints == 4 )
    {
        cvSetTo(tempMask, npoints, 1, CC_8U, 1);
        result = cb->runKernel(src, dst, H) > 0;
    }
    else if( method == RANSAC )
        result = createRANSACPointSetRegistrator(cb, 4, ransacReprojThreshold, confidence, maxIters)->run(src, dst, H, tempMask);
    else if( method == LMEDS )
        result = createLMeDSPointSetRegistrator(cb, 4, confidence, maxIters)->run(src, dst, H, tempMask);
    else if( method == RHO )
        result = createAndRunRHORegistrator(confidence, maxIters, ransacReprojThreshold, npoints, src, dst, H, tempMask);
    else
        CC_Error(CC_StsBadArg, "Unknown estimation method");

    if( result && npoints > 4 && method != RHO)
    {
        compressElems( (CPoint2f*)(src->tt.data), img_ptr(uchar, tempMask), 1, npoints );
        npoints = compressElems( (CPoint2f*)(dst->tt.data), img_ptr(uchar, tempMask ), 1, npoints );
        if( npoints > 0 )
        {
            img_t src1[1]; src = cvGetRows(src, src1, 0, npoints);
            img_t dst1[1]; dst = cvGetRows(dst, dst1, 0, npoints);
            if( method == RANSAC || method == LMEDS )
                cb->runKernel( src, dst, H );
            img_t H8 = cvMat1(8, 1, CC_64F, 1, img_ptr(double, H));
            HomographyRefineCallback hr(src, dst);
            createLMSolver(&hr, 10)->run(&H8);
        }
    }

    if( result )
    {
        if( _mask )
            cvCopyTo(tempMask, _mask);
    }
    else
    {
        imfree(H);
        if(_mask ) {
            cvSetZero(tempMask, npoints >= 0 ? npoints : 0, 1, CC_8U);
            cvCopyTo(tempMask, _mask);
        }
    }
    IM_END(im);

    return H;
}

static img_t* findHomography( img_t* H, const img_t* _points1, const img_t* _points2,
                      img_t* _mask, int method, double ransacReprojThreshold )
{
    return findHomography(H, _points1, _points2, method, ransacReprojThreshold, _mask);
}



/* Estimation of Fundamental Matrix from point correspondences.
   The original code has been written by Valery Mosyagin */

/* The algorithms (except for RANSAC) and the notation have been taken from
   Zhengyou Zhang's research report
   "Determining the Epipolar Geometry and its Uncertainty: A Review"
   that can be found at http://www-sop.inria.fr/robotvis/personnel/zzhang/zzhang-eng.html */

/************************************** 7-point algorithm *******************************/

static int run7Point( const img_t* _m1, const img_t* _m2, double* fmatrix )
{
    double a[7*9], w[7], u[9*9], v[9*9], c[4], r[3];
    double* f1, *f2;
    double t0, t1, t2;
    img_t A = cvMat1( 7, 9, CC_64F, 1, a );
    img_t U = cvMat1( 7, 9, CC_64F, 1, u );
    img_t Vt = cvMat1( 9, 9, CC_64F, 1, v );
    img_t W = cvMat1( 7, 1, CC_64F, 1, w );
    img_t coeffs = cvMat1( 1, 4, CC_64F, 1, c );
    img_t roots = cvMat1( 1, 3, CC_64F, 1, r );
    const CPoint2f* m1 = (CPoint2f*)(_m1->tt.data);
    const CPoint2f* m2 = (CPoint2f*)(_m2->tt.data);
    // double* fmatrix = img_ptr(double, _fmatrix);
    int i, k, n;

    // form a linear system: i-th row of A(=a) represents
    // the equation: (m2[i], 1)'*F*(m1[i], 1) = 0
    for( i = 0; i < 7; i++ )
    {
        double x0 = m1[i].x, y0 = m1[i].y;
        double x1 = m2[i].x, y1 = m2[i].y;

        a[i*9+0] = x1*x0;
        a[i*9+1] = x1*y0;
        a[i*9+2] = x1;
        a[i*9+3] = y1*x0;
        a[i*9+4] = y1*y0;
        a[i*9+5] = y1;
        a[i*9+6] = x0;
        a[i*9+7] = y0;
        a[i*9+8] = 1;
    }

    // A*(f11 f12 ... f33)' = 0 is singular (7 equations for 9 variables), so
    // the solution is linear subspace of dimensionality 2.
    // => use the last two singular vectors as a basis of the space
    // (according to CSVD properties)
    SVD_compute( &A, &W, &U, &Vt, CC_SVD_MODIFY_A + CC_SVD_FULL_UV );
    f1 = v + 7*9;
    f2 = v + 8*9;

    // f1, f2 is a basis => lambda*f1 + mu*f2 is an arbitrary f. matrix.
    // as it is determined up to a scale, normalize lambda & mu (lambda + mu = 1),
    // so f ~ lambda*f1 + (1 - lambda)*f2.
    // use the additional constraint det(f) = det(lambda*f1 + (1-lambda)*f2) to find lambda.
    // it will be a cubic equation.
    // find c - polynomial coefficients.
    for( i = 0; i < 9; i++ )
        f1[i] -= f2[i];

    t0 = f2[4]*f2[8] - f2[5]*f2[7];
    t1 = f2[3]*f2[8] - f2[5]*f2[6];
    t2 = f2[3]*f2[7] - f2[4]*f2[6];

    c[3] = f2[0]*t0 - f2[1]*t1 + f2[2]*t2;

    c[2] = f1[0]*t0 - f1[1]*t1 + f1[2]*t2 -
    f1[3]*(f2[1]*f2[8] - f2[2]*f2[7]) +
    f1[4]*(f2[0]*f2[8] - f2[2]*f2[6]) -
    f1[5]*(f2[0]*f2[7] - f2[1]*f2[6]) +
    f1[6]*(f2[1]*f2[5] - f2[2]*f2[4]) -
    f1[7]*(f2[0]*f2[5] - f2[2]*f2[3]) +
    f1[8]*(f2[0]*f2[4] - f2[1]*f2[3]);

    t0 = f1[4]*f1[8] - f1[5]*f1[7];
    t1 = f1[3]*f1[8] - f1[5]*f1[6];
    t2 = f1[3]*f1[7] - f1[4]*f1[6];

    c[1] = f2[0]*t0 - f2[1]*t1 + f2[2]*t2 -
    f2[3]*(f1[1]*f1[8] - f1[2]*f1[7]) +
    f2[4]*(f1[0]*f1[8] - f1[2]*f1[6]) -
    f2[5]*(f1[0]*f1[7] - f1[1]*f1[6]) +
    f2[6]*(f1[1]*f1[5] - f1[2]*f1[4]) -
    f2[7]*(f1[0]*f1[5] - f1[2]*f1[3]) +
    f2[8]*(f1[0]*f1[4] - f1[1]*f1[3]);

    c[0] = f1[0]*t0 - f1[1]*t1 + f1[2]*t2;

    // solve the cubic equation; there can be 1 to 3 roots ...
    n = solveCubic( &coeffs, &roots );

    if( n < 1 || n > 3 )
        return n;

    for( k = 0; k < n; k++, fmatrix += 9 )
    {
        // for each root form the fundamental matrix
        double lambda = r[k], mu = 1.;
        double s = f1[8]*r[k] + f2[8];

        // normalize each matrix, so that F(3,3) (~fmatrix[8]) == 1
        if( fabs(s) > DBL_EPSILON )
        {
            mu = 1./s;
            lambda *= mu;
            fmatrix[8] = 1.;
        }
        else
            fmatrix[8] = 0.;

        for( i = 0; i < 8; i++ )
            fmatrix[i] = f1[i]*lambda + f2[i]*mu;
    }

    return n;
}

static double norm(double x, double y, double z) {
	return sqrt(x*x + y*y + z*z);
}

static double norm(double x, double y) {
	return sqrt(x*x + y*y);
}

static void mul_vvT(int n, const double* v, double* out) {
  int i, j;
  for (i=0; i<n; ++i) {
    double* out1 = out + i*n;
    for (j=0; j<n; ++j) {
      out1[j] += v[i] * v[j];
    }
  }
}

static void gemdm(const double* a, int al, const double* w, const double* b, int bl, double* d, int dl, int M, int N, int K) {
  int i, j, k;
  for (i=0; i<M; ++i ) {
    for (j=0; j<N; ++j) {
      double s = 0;
      for (k=0; k<K; ++k) {
        s += a[i*al+k]*b[k*bl+j]*w[k];
      }
      d[i*dl+j] = s;
    }
  }
}

static int run8Point( const img_t* _m1, const img_t* _m2, double9 fmatrix )
{
    CPoint2d m1c = {0,0}, m2c = {0,0};
    double t, scale1 = 0, scale2 = 0;

    const CPoint2f* m1 = (CPoint2f*)(_m1->tt.data);
    const CPoint2f* m2 = (CPoint2f*)(_m2->tt.data);
    CC_Assert( (_m1->cols == 1 || _m1->rows == 1) && CC_ARE_SIZES_EQ(_m1, _m2));
    int i, count = cvCheckVector(_m1, 2);

    // compute centers and average distances for each of the two point sets
    for( i = 0; i < count; i++ )
    {
        m1c.x += m1[i].x;
        m1c.y += m1[i].y;
        m2c.x += m2[i].x;
        m2c.y += m2[i].y;
    }

    // calculate the normalizing transformations for each of the point sets:
    // after the transformation each set will have the mass center at the coordinate origin
    // and the average distance from the origin will be ~sqrt(2).
    t = 1./count;
    m1c.x *= t;
    m1c.y *= t;
    m2c.x *= t;
    m2c.y *= t;

    for( i = 0; i < count; i++ )
    {
        scale1 += norm(m1[i].x - m1c.x, m1[i].y - m1c.y);
        scale2 += norm(m2[i].x - m2c.x, m2[i].y - m2c.y);
    }

    scale1 *= t;
    scale2 *= t;

    if( scale1 < FLT_EPSILON || scale2 < FLT_EPSILON )
        return 0;

    scale1 = sqrt(2.)/scale1;
    scale2 = sqrt(2.)/scale2;

    double A[9*9] = {0};

    // form a linear system Ax=0: for each selected pair of points m1 & m2,
    // the row of A(=a) represents the coefficients of equation: (m2, 1)'*F*(m1, 1) = 0
    // to save computation time, we compute (At*A) instead of A and then solve (At*A)x=0.
    for( i = 0; i < count; i++ )
    {
        double x1 = (m1[i].x - m1c.x)*scale1;
        double y1 = (m1[i].y - m1c.y)*scale1;
        double x2 = (m2[i].x - m2c.x)*scale2;
        double y2 = (m2[i].y - m2c.y)*scale2;
        double9 r = { x2*x1, x2*y1, x2, y2*x1, y2*y1, y2, x1, y1, 1 };
        //A += r*r.t();
        mul_vvT(9, r, A);
    }

    IM_DEF(tmp, 10);
    double9 W = {0};
    double V[9*9] = {0};

    //eigen(A, W, V);
    eigen(cvMat(tmp+0, 9, 9, CC_64F, 1, A),
      cvMat(tmp+1, 9, 1, CC_64F, 1, W),
      cvMat(tmp+2, 9, 9, CC_64F, 1, V) );


    for( i = 0; i < 9; i++ )
    {
        if( fabs(W[i]) < DBL_EPSILON )
            break;
    }

    if( i < 8 )
        return 0;

    double9 F0;
    MEMCPY( F0, V+9*8, sizeof(double)*9 ); // take the last column of v as a solution of Af = 0

    // make F0 singular (of rank 2) by decomposing it with CSVD,
    // zeroing the last diagonal element of W and then composing the matrices back.

    double3 w;
    double9 U;
    double9 Vt;

    //SVD_compute( F0, w, U, Vt);
    SVD_compute(cvMat(tmp+0, 3, 3, CC_64F, 1, F0),
      cvMat(tmp+1, 3, 1, CC_64F, 1, w),
      cvMat(tmp+2, 3, 3, CC_64F, 1, U),
      cvMat(tmp+3, 3, 3, CC_64F, 1, Vt));
    w[2] = 0.;

    //F0 = U*Matx33d::diag(w)*Vt;
    gemdm(U, 3, w, Vt, 3, F0, 3, 3, 3, 3);

    // apply the transformation that is inverse
    // to what we used to normalize the point coordinates
    double9 T1 = { scale1, 0, -scale1*m1c.x, 0, scale1, -scale1*m1c.y, 0, 0, 1 };
    double9 T2 = { scale2, 0, -scale2*m2c.x, 0, scale2, -scale2*m2c.y, 0, 0, 1 };
    double9 T3;
    //F0 = T2.t()*F0*T1;
    trans3(T2);
    matmul3(T3, T2, F0);
    matmul3(F0, T3, T1);


    // make F(3,3) = 1
    if( fabs(F0[2*3+2]) > FLT_EPSILON )
        scale9(F0, F0, 1./F0[2*3+2]);

    //img_t(F0).copyTo(_fmatrix);
    //cvSetMat(_fmatrix, 3, 3, CC_64F, 1, F0);
    MEMCPY(fmatrix, F0, 9);
    return 1;
}


class FMEstimatorCallback : public PointSetRegistrator::Callback
{
public:
    bool checkSubset( const img_t* _ms1, const img_t* _ms2, int count ) const
    {
        const img_t *ms1 = _ms1, *ms2 = _ms2;
        return !haveCollinearPoints(ms1, count) && !haveCollinearPoints(ms2, count);
    }

    int runKernel( const img_t* _m1, const img_t* _m2, img_t* _model ) const
    {
        double f[9*3];
        const img_t *m1 = _m1, *m2 = _m2;
        int count = cvCheckVector(m1, 2);
        img_t F = cvMat1(count == 7 ? 9 : 3, 3, CC_64F, 1, f);
        int n = count == 7 ? run7Point(m1, m2, f) : run8Point(m1, m2, f);
        img_t tmp[1];

        if( n == 0 )
            imfree(_model);
        else
            cvCopyTo(cvGetRows(&F,tmp, 0, n*3), _model);

        return n;
    }

    void computeError( const img_t* _m1, const img_t* _m2, const img_t* _model, img_t* _err ) const
    {
        const img_t *__m1 = _m1, *__m2 = _m2, *__model = _model;
        int i, count = cvCheckVector(__m1, 2);
        const CPoint2f* m1 = (CPoint2f*)(__m1->tt.data);
        const CPoint2f* m2 = (CPoint2f*)(__m2->tt.data);
        const double* F = img_ptr(double, __model);
        cvSetMat(_err, count, 1, CC_32F);
        float* err = img_ptr(float, _err);

        for( i = 0; i < count; i++ )
        {
            double a, b, c, d1, d2, s1, s2;

            a = F[0]*m1[i].x + F[1]*m1[i].y + F[2];
            b = F[3]*m1[i].x + F[4]*m1[i].y + F[5];
            c = F[6]*m1[i].x + F[7]*m1[i].y + F[8];

            s2 = 1./(a*a + b*b);
            d2 = m2[i].x*a + m2[i].y*b + c;

            a = F[0]*m2[i].x + F[3]*m2[i].y + F[6];
            b = F[1]*m2[i].x + F[4]*m2[i].y + F[7];
            c = F[2]*m2[i].x + F[5]*m2[i].y + F[8];

            s1 = 1./(a*a + b*b);
            d1 = m1[i].x*a + m1[i].y*b + c;

            err[i] = (float)MAX(d1*d1*s1, d2*d2*s2);
        }
    }
};

static img_t* findFundamentalMat( img_t* F, const img_t* _points1, const img_t* _points2,
                                int method, double param1, double param2,
                                img_t* _mask )
{
    CC_INSTRUMENT_REGION()

    IM_BEGIN(im, 10);
    IM_DEF(tmp, 10);
    const img_t *points1 = _points1, *points2 = _points2;
    img_t *m1=im++, *m2=im++;
    int npoints = -1;

    for( int i = 1; i <= 2; i++ )
    {
        const img_t* p = i == 1 ? points1 : points2;
        img_t* m = i == 1 ? m1 : m2;
        npoints = cvCheckVector(p, 2, CC_NUL, false);
        if( npoints < 0 )
        {
            npoints = cvCheckVector(p, 3, CC_NUL, false);
            if( npoints < 0 )
                CC_Error(CC_StsBadArg, "The input arrays should be 2D or 3D point sets");
            if( npoints == 0 )
                return NULL;
            p = convertPointsFromHomogeneous(p, im++);
        }
        cvConvertTo(cvReshape(p, tmp, 2, npoints), m, CC_32F);
    }

    CC_Assert( cvCheckVector(m1, 2) == cvCheckVector(m2, 2) );

    if( npoints < 7 )
        F =  NULL;

    if (F)
    {
        FMEstimatorCallback cb_;
        PointSetRegistrator::Callback* cb = &cb_;
        int result;

        if( npoints == 7 || method == FM_8POINT )
        {
            result = cb->runKernel(m1, m2, F);
            if( _mask )
            {
                cvSetMat(_mask, npoints, 1, CC_8U);
                img_t *mask = _mask;
                CC_Assert( (mask->cols == 1 || mask->rows == 1) && (int)CC_MAT_TOTAL(mask) == npoints );
                cvSetReal(mask, 1);
            }
        }
        else
        {
            if( param1 <= 0 )
                param1 = 3;
            if( param2 < DBL_EPSILON || param2 > 1 - DBL_EPSILON )
                param2 = 0.99;

            if( (method & ~3) == FM_RANSAC && npoints >= 15 )
                result = createRANSACPointSetRegistrator(cb, 7, param1, param2)->run(m1, m2, F, _mask);
            else
                result = createLMeDSPointSetRegistrator(cb, 7, param2)->run(m1, m2, F, _mask);
        }
        
        if( result <= 0 )
            F = NULL;
    }


    IM_END(im);

    return F;
}

static img_t* findFundamentalMat( img_t* F, const img_t* _points1, const img_t* _points2,
                               img_t* _mask, int method, double param1, double param2 )
{
    return findFundamentalMat(F, _points1, _points2, method, param1, param2, _mask);
}

static img_t* computeCorrespondEpilines( const img_t* _points, int whichImage,
                                    const img_t* _Fmat, img_t* _lines )
{
    CC_INSTRUMENT_REGION()

    IM_BEGIN(im, 10);
    double f[9];
    img_t tempF = cvMat1(3, 3, CC_64F, 1, f);
    const img_t *points = _points, *F = _Fmat;

    if( !CC_IS_CONT_MAT(points) )
        points = cvCloneMat(im++, points);

    int npoints = cvCheckVector(points, 2);
    if( npoints < 0 )
    {
        npoints = cvCheckVector(points, 3);
        if( npoints < 0 )
            CC_Error( CC_StsBadArg, "The input should be a 2D or 3D point set");

        points = convertPointsFromHomogeneous(points, im++);
    }
    TypeId depth = points->tid;
    CC_Assert( depth == CC_32F || depth == CC_32S || depth == CC_64F );

    CC_Assert(CC_ARE_SIZES_EQSZ(F,3,3));
    cvConvertTo(F, &tempF, CC_64F);
    if( whichImage == 2 )
        transpose(&tempF, &tempF);

    TypeId ltype = MAX(depth, CC_32F);//, 3;
    cvSetMat(_lines, npoints, 1, ltype, 3);
    img_t *lines = _lines;
    if( !CC_IS_CONT_MAT(lines) )
    {
        imfree(_lines);
        lines = cvSetMat(_lines, npoints, 1, ltype, 3);
    }
    CC_Assert( CC_IS_CONT_MAT(lines));

    if( depth == CC_32S || depth == CC_32F )
    {
        const CPoint* ptsi = (CPoint*)(points->tt.data);
        const CPoint2f* ptsf = (CPoint2f*)(points->tt.data);
        CPoint3f* dstf = (CPoint3f*)(lines->tt.data);
        for( int i = 0; i < npoints; i++ )
        {
            CPoint2f pt = depth == CC_32F ? ptsf[i] : fPOINT((float)ptsi[i].x, (float)ptsi[i].y);
            double a = f[0]*pt.x + f[1]*pt.y + f[2];
            double b = f[3]*pt.x + f[4]*pt.y + f[5];
            double c = f[6]*pt.x + f[7]*pt.y + f[8];
            double nu = a*a + b*b;
            nu = nu ? 1./sqrt(nu) : 1.;
            a *= nu; b *= nu; c *= nu;
            dstf[i] = fPOINT3((float)a, (float)b, (float)c);
        }
    }
    else
    {
        const CPoint2d* ptsd = (CPoint2d*)(points->tt.data);
        CPoint3d* dstd = (CPoint3d*)(lines->tt.data);
        for( int i = 0; i < npoints; i++ )
        {
            CPoint2d pt = ptsd[i];
            double a = f[0]*pt.x + f[1]*pt.y + f[2];
            double b = f[3]*pt.x + f[4]*pt.y + f[5];
            double c = f[6]*pt.x + f[7]*pt.y + f[8];
            double nu = a*a + b*b;
            nu = nu ? 1./sqrt(nu) : 1.;
            a *= nu; b *= nu; c *= nu;
            dstd[i] = dPOINT3(a, b, c);
        }
    }
    IM_END(im);
    return _lines;
}


static img_t* convertPointsFromHomogeneous( const img_t* _src, img_t* _dst )
{
    const img_t *src = _src;
    IM_BEGIN(im, 10);
    if( !CC_IS_CONT_MAT(src) )
        src = cvCloneMat(im++, _src);
    TypeId type = src->tid;
    int i, npoints = cvCheckVector(src, 3), cn = 3;
    if( npoints < 0 )
    {
        npoints = cvCheckVector(src, 4);
        CC_Assert(npoints >= 0);
        cn = 4;
    }
    CC_Assert( npoints >= 0 && (type == CC_32S || type == CC_32F || type == CC_64F));

    TypeCnId dtype = CC_MAKETYPECN(type <= CC_32F ? CC_32F : CC_64F, cn-1);
    cvSetMat1(_dst, npoints, 1, dtype);
    img_t *dst = _dst;
    if( !CC_IS_CONT_MAT(dst) )
    {
        cvSetMat1(_dst, npoints, 1, dtype);
        dst = _dst;
    }
    CC_Assert( CC_IS_CONT_MAT(dst) );

    if( type == CC_32S )
    {
        if( cn == 3 )
        {
            const CPoint3i* sptr = (CPoint3i*)src->tt.data;
            CPoint2f* dptr = (CPoint2f*)dst->tt.data;
            for( i = 0; i < npoints; i++ )
            {
                float scale = sptr[i].z != 0 ? 1.f/sptr[i].z : 1.f;
                SETPT2(dptr[i], sptr[i].x*scale, sptr[i].y*scale);
            }
        }
        else
        {
            const int4* sptr = (int4*)src->tt.data;
            CPoint3f* dptr = (CPoint3f*)dst->tt.data;
            for( i = 0; i < npoints; i++ )
            {
                float scale = sptr[i][3] != 0 ? 1.f/sptr[i][3] : 1.f;
                SETPT3(dptr[i], sptr[i][0]*scale, sptr[i][1]*scale, sptr[i][2]*scale);
            }
        }
    }
    else if( type == CC_32F )
    {
        if( cn == 3 )
        {
            const CPoint3f* sptr = (CPoint3f*)src->tt.data;
            CPoint2f* dptr = (CPoint2f*)dst->tt.data;
            for( i = 0; i < npoints; i++ )
            {
                float scale = sptr[i].z != 0.f ? 1.f/sptr[i].z : 1.f;
                SETPT2(dptr[i], sptr[i].x*scale, sptr[i].y*scale);
            }
        }
        else
        {
            const float4* sptr = (float4*)src->tt.data;
            CPoint3f* dptr = (CPoint3f*)dst->tt.data;
            for( i = 0; i < npoints; i++ )
            {
                float scale = sptr[i][3] != 0.f ? 1.f/sptr[i][3] : 1.f;
                SETPT3(dptr[i], sptr[i][0]*scale, sptr[i][1]*scale, sptr[i][2]*scale);
            }
        }
    }
    else if( type == CC_64F )
    {
        if( cn == 3 )
        {
            const CPoint3d* sptr = (CPoint3d*)src->tt.data;
            CPoint2d* dptr = (CPoint2d*)dst->tt.data;
            for( i = 0; i < npoints; i++ )
            {
                double scale = sptr[i].z != 0. ? 1./sptr[i].z : 1.;
                SETPT2(dptr[i], sptr[i].x*scale, sptr[i].y*scale);
            }
        }
        else
        {
            const double4* sptr = (double4*)src->tt.data;
            CPoint3d* dptr = (CPoint3d*)dst->tt.data;
            for( i = 0; i < npoints; i++ )
            {
                double scale = sptr[i][3] != 0.f ? 1./sptr[i][3] : 1.;
                SETPT3(dptr[i], sptr[i][0]*scale, sptr[i][1]*scale, sptr[i][2]*scale);
            }
        }
    }
    else
        CC_Error(CC_StsUnsupportedFormat, "");
    IM_END(im);
    return _dst;
}

static img_t* convertPointsToHomogeneous( const img_t* _src, img_t* _dst )
{
    CC_INSTRUMENT_REGION()

    const img_t *src = _src;
    IM_BEGIN(im, 10);
    if( !CC_IS_CONT_MAT(_src) )
        src = cvCloneMat(im++, _src);
    TypeId type = src->tid;
    int i, npoints = cvCheckVector(src, 2), cn = 2;
    if( npoints < 0 )
    {
        npoints = cvCheckVector(src, 3);
        CC_Assert(npoints >= 0);
        cn = 3;
    }
    CC_Assert( npoints >= 0 && (type == CC_32S || type == CC_32F || type == CC_64F));

    TypeCnId dtype = CC_MAKETYPECN(type, cn+1);
    cvSetMat1(_dst, npoints, 1, dtype);
    img_t *dst = _dst;
    if( !CC_IS_CONT_MAT(dst) )
    {
        cvSetMat1(_dst, npoints, 1, dtype);
        dst = _dst;
    }
    CC_Assert( CC_IS_CONT_MAT(dst) );

    if( type == CC_32S )
    {
        if( cn == 2 )
        {
            const CPoint2i* sptr = (CPoint2i*)src->tt.data;
            CPoint3i* dptr = (CPoint3i*)dst->tt.data;
            for( i = 0; i < npoints; i++ )
                SETPT3(dptr[i], sptr[i].x, sptr[i].y, 1);
        }
        else
        {
            const CPoint3i* sptr = (CPoint3i*)src->tt.data;
            int4* dptr = (int4*)dst->tt.data;
            for( i = 0; i < npoints; i++ )
                SET4(dptr[i], sptr[i].x, sptr[i].y, sptr[i].z, 1);
        }
    }
    else if( type == CC_32F )
    {
        if( cn == 2 )
        {
            const CPoint2f* sptr = (CPoint2f*)src->tt.data;
            CPoint3f* dptr = (CPoint3f*)dst->tt.data;
            for( i = 0; i < npoints; i++ )
                SETPT3(dptr[i], sptr[i].x, sptr[i].y, 1.f);
        }
        else
        {
            const CPoint3f* sptr = (CPoint3f*)src->tt.data;
            float4* dptr = (float4*)dst->tt.data;
            for( i = 0; i < npoints; i++ )
                 SET4(dptr[i], sptr[i].x, sptr[i].y, sptr[i].z, 1.f);
        }
    }
    else if( type == CC_64F )
    {
        if( cn == 2 )
        {
            const CPoint2d* sptr = (CPoint2d*)src->tt.data;
            CPoint3d* dptr = (CPoint3d*)dst->tt.data;
            for( i = 0; i < npoints; i++ )
                SETPT3(dptr[i], sptr[i].x, sptr[i].y, 1.);
        }
        else
        {
            const CPoint3d* sptr = (CPoint3d*)src->tt.data;
            double4* dptr = (double4*)dst->tt.data;
            for( i = 0; i < npoints; i++ )
                SET4(dptr[i], sptr[i].x, sptr[i].y, sptr[i].z, 1.);
        }
    }
    else
        CC_Error(CC_StsUnsupportedFormat, "");
    IM_END(im);
    return _dst;
}

static img_t* convertPointsHomogeneous( const img_t* _src, img_t* _dst )
{
    CC_INSTRUMENT_REGION()

    TypeId stype = _src->tid, dtype = _dst->tid;
    //CC_Assert( _dst.fixedType() );

    if( CC_MAT_CN(_src) > CC_MAT_CN(_dst) )
        return convertPointsFromHomogeneous(_src, _dst);
    else
        return convertPointsToHomogeneous(_src, _dst);
}

static double sampsonDistance(const img_t* _pt1, const img_t* _pt2, const img_t* _F)
{
    CC_INSTRUMENT_REGION()

    CC_Assert(_pt1->tid == CC_64F && _pt2->tid == CC_64F && _F->tid == CC_64F);
    CC_DbgAssert(_pt1->rows == 3 && CC_ARE_SIZES_EQSZ(_F, 3, 3) && _pt1->rows == _pt2->rows);

    const double *pt1 = img_ptr(double, _pt1);
    const double *pt2 = img_ptr(double, _pt2);
    const double *F = img_ptr(double, _F);

    double3 F_pt1;
    double3 Ft_pt2;
    //F_pt1 = *F.ptr<Matx33d>() * *pt1.ptr<Vec3d>();
    //Ft_pt2 = F.ptr<Matx33d>()->t() * *pt2.ptr<Vec3d>();
    matvec3(F_pt1, F, pt1);
    matvec3t(Ft_pt2, F, pt2);

    double v = dot3(pt2, F_pt1);

    // square
    //Ft_pt2 = Ft_pt2.mul(Ft_pt2);
    //F_pt1 = F_pt1.mul(F_pt1);
    matmul3(Ft_pt2, Ft_pt2, Ft_pt2);
    matmul3(F_pt1, F_pt1, F_pt1);

    return v*v / (F_pt1[0] + F_pt1[1] + Ft_pt2[0] + Ft_pt2[1]);
}

