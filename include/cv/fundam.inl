
/* Evaluation of Fundamental Matrix from point correspondences.
   The original code has been written by Valery Mosyagin */

/* The algorithms (except for RANSAC) and the notation have been taken from
   Zhengyou Zhang's research report
   "Determining the Epipolar Geometry and its Uncertainty: A Review"
   that can be found at http://www-sop.inria.fr/robotvis/personnel/zzhang/zzhang-eng.html */

/************************************** 7-point algorithm *******************************/
static int
icvFMatrix_7Point(const CPoint2D64f* m0, const CPoint2D64f* m1, double* fmatrix)
{
    double a[7*9], w[7], v[9*9], c[4], r[3];
    double* f1, *f2;
    double t0, t1, t2;
    IM_DEF(tmp, 10);
    img_t *A = cvMat(tmp++, 7, 9, CC_64F, 1, a);
    img_t *V = cvMat(tmp++, 9, 9, CC_64F, 1, v);
    img_t *W = cvMat(tmp++, 7, 1, CC_64F, 1, w);
    img_t *coeffs = cvMat(tmp++, 1, 4, CC_64F, 1, c);
    img_t *roots = cvMat(tmp++, 1, 3, CC_64F, 1, r);
    int i, k, n;

    assert(m0 && m1 && fmatrix);

    // form a linear system: i-th row of A(=a) represents
    // the equation: (m1[i], 1)'*F*(m0[i], 1) = 0
    for(i = 0; i < 7; i++)
    {
        double x0 = m0[i].x, y0 = m0[i].y;
        double x1 = m1[i].x, y1 = m1[i].y;

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
    cvSVD(A, W, 0, V, CC_SVD_MODIFY_A + CC_SVD_V_T);
    f1 = v + 7*9;
    f2 = v + 8*9;

    // f1, f2 is a basis => lambda*f1 + mu*f2 is an arbitrary f. matrix.
    // as it is determined up to a scale, normalize lambda & mu (lambda + mu = 1),
    // so f ~ lambda*f1 + (1 - lambda)*f2.
    // use the additional constraint det(f) = det(lambda*f1 + (1-lambda)*f2) to find lambda.
    // it will be a cubic equation.
    // find c - polynomial coefficients.
    for(i = 0; i < 9; i++)
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
    n = cvSolveCubic(coeffs, roots);

    if(n < 1 || n > 3)
        return n;

    for(k = 0; k < n; k++, fmatrix += 9)
    {
        // for each root form the fundamental matrix
        double lambda = r[k], mu = 1.;
        double s = f1[8]*r[k] + f2[8];

        // normalize each matrix, so that F(3,3) (~fmatrix[8]) == 1
        if(fabs(s) > DBL_EPSILON)
        {
            mu = 1./s;
            lambda *= mu;
            fmatrix[8] = 1.;
        }
        else
            fmatrix[8] = 0.;

        for(i = 0; i < 8; i++)
            fmatrix[i] = f1[i]*lambda + f2[i]*mu;
    }

    return n;
}


/*************************************** 8-point algorithm ******************************/
static int
icvFMatrix_8Point(const CPoint2D64f* m0, const CPoint2D64f* m1,
                   const uchar* mask, int count, double* fmatrix)
{
    int result = 0;
    img_t A[] = {0};

    double w[9], v[9*9];
    img_t W[1]; cvMat(W, 1, 9, CC_64F, 1, w);
    img_t V[1]; cvMat(V, 9, 9, CC_64F, 1, v);
    img_t U[1], F0[1], TF[1];

    int i, good_count = 0;
    CPoint2D64f m0c = {0,0}, m1c = {0,0};
    double t, scale0 = 0, scale1 = 0;
    double* a;
    int a_step;

    CC_FUNCNAME("icvFMatrix_8Point");

    __BEGIN__;

    assert(m0 && m1 && fmatrix);

    // compute centers and average distances for each of the two point sets
    for(i = 0; i < count; i++)
        if(!mask || mask[i])
        {
            double x = m0[i].x, y = m0[i].y;
            m0c.x += x; m0c.y += y;

            x = m1[i].x, y = m1[i].y;
            m1c.x += x; m1c.y += y;
            good_count++;
        }

    if(good_count < 8)
        EXIT;

    // calculate the normalizing transformations for each of the point sets:
    // after the transformation each set will have the mass center at the coordinate origin
    // and the average distance from the origin will be ~sqrt(2).
    t = 1./good_count;
    m0c.x *= t; m0c.y *= t;
    m1c.x *= t; m1c.y *= t;

    for(i = 0; i < count; i++)
        if(!mask || mask[i])
        {
            double x = m0[i].x - m0c.x, y = m0[i].y - m0c.y;
            scale0 += sqrt(x*x + y*y);

            x = fabs(m1[i].x - m1c.x), y = fabs(m1[i].y - m1c.y);
            scale1 += sqrt(x*x + y*y);
        }

    scale0 *= t;
    scale1 *= t;

    if(scale0 < FLT_EPSILON || scale1 < FLT_EPSILON)
        EXIT;

    scale0 = sqrt(2.)/scale0;
    scale1 = sqrt(2.)/scale1;

    CC_CALL(cvCreateMat(A, good_count, 9, CC_64F));
    a = A->tt.db;
    a_step = A->step / sizeof(a[0]);

    // form a linear system: for each selected pair of points m0 & m1,
    // the row of A(=a) represents the equation: (m1, 1)'*F*(m0, 1) = 0
    for(i = 0; i < count; i++)
    {
        if(!mask || mask[i])
        {
            double x0 = (m0[i].x - m0c.x)*scale0;
            double y0 = (m0[i].y - m0c.y)*scale0;
            double x1 = (m1[i].x - m1c.x)*scale1;
            double y1 = (m1[i].y - m1c.y)*scale1;

            a[0] = x1*x0;
            a[1] = x1*y0;
            a[2] = x1;
            a[3] = y1*x0;
            a[4] = y1*y0;
            a[5] = y1;
            a[6] = x0;
            a[7] = y0;
            a[8] = 1;
            a += a_step;
        }
    }

    cvSVD(A, W, 0, V, CC_SVD_MODIFY_A + CC_SVD_V_T);

    for(i = 0; i < 8; i++)
    {
        if(fabs(w[i]) < FLT_EPSILON)
            break;
    }

    if(i < 7)
        EXIT;

    cvMat(F0, 3, 3, CC_64F, 1, v + 9*8); // take the last column of v as a solution of Af = 0

    // make F0 singular (of rank 2) by decomposing it with CSVD,
    // zeroing the last diagonal element of W and then composing the matrices back.

    // use v as a temporary storage for different 3x3 matrices
    *W = *U = *V = *TF = *F0;
    W->tt.db = v;
    U->tt.db = v + 9;
    V->tt.db = v + 18;
    TF->tt.db = v + 27;

    cvSVD(F0, W, U, V, CC_SVD_MODIFY_A + CC_SVD_U_T + CC_SVD_V_T);
    W->tt.db[8] = 0.;

    // F0 <- U*diag([W(1), W(2), 0])*V'
    cvGEMM(U, W, 1., 0, 0., TF, CC_GEMM_A_T);
    cvGEMM(TF, V, 1., 0, 0., F0, 0/*CC_GEMM_B_T*/);

    // apply the transformation that is inverse
    // to what we used to normalize the point coordinates
    {
        double tt0[] = { scale0, 0, -scale0*m0c.x, 0, scale0, -scale0*m0c.y, 0, 0, 1 };
        double tt1[] = { scale1, 0, -scale1*m1c.x, 0, scale1, -scale1*m1c.y, 0, 0, 1 };
        img_t T0[1], T1[1];
        *T0 = *T1 = *F0;
        T0->tt.db = tt0;
        T1->tt.db = tt1;

        // F0 <- T1'*F0*T0
        cvGEMM(T1, F0, 1., 0, 0., TF, CC_GEMM_A_T);
        F0->tt.db = fmatrix;
        cvGEMM(TF, T0, 1., 0, 0., F0, 0);

        // make F(3,3) = 1
        if(fabs(F0->tt.db[8]) > FLT_EPSILON)
            cvScale(F0, F0, 1./F0->tt.db[8]);
    }

    result = 1;

    __END__;

    imfree(A);
    return result;
}


/************************************ RANSAC algorithm **********************************/
static int
icvFMatrix_RANSAC(const CPoint2D64f* m0, const CPoint2D64f* m1,
                   uchar* mask, int count, double* fmatrix,
                   double threshold, double p,
                   unsigned rng_seed, int use_8point)
{
    int result = 0;

    const int max_random_iters = 1000;
    const int sample_size = 7;
    uchar* curr_mask = 0;
    uchar* temp_mask = 0;

    CC_FUNCNAME("icvFMatrix_RANSAC");

    __BEGIN__;

    double ff[9*3];
    CRNG rng = cRNG(rng_seed);
    int i, j, k, sample_count, max_samples = 500;
    int best_good_count = 0;

    assert(m0 && m1 && fmatrix && 0 < p && p < 1 && threshold > 0);

    threshold *= threshold;

    CC_CALL(curr_mask = (uchar*)cAlloc(count));
    if(!mask && use_8point)
    {
        CC_CALL(temp_mask = (uchar*)cAlloc(count));
        mask = temp_mask;
    }

    // find the best fundamental matrix (giving the least backprojection error)
    // by picking at most <max_samples> 7-tuples of corresponding points
    // <max_samples> may be updated (decreased) within the loop based on statistics of outliers
    for(sample_count = 0; sample_count < max_samples; sample_count++)
    {
        int idx[sample_size], n;
        CPoint2D64f ms0[sample_size], ms1[sample_size];

        // choose random <sample_size> (=7) points
        for(i = 0; i < sample_size; i++)
        {
            for(k = 0; k < max_random_iters; k++)
            {
                idx[i] = cvRandInt(&rng) % count;
                for(j = 0; j < i; j++)
                    if(idx[j] == idx[i])
                        break;
                if(j == i)
                {
                    ms0[i] = m0[idx[i]];
                    ms1[i] = m1[idx[i]];
                    break;
                }
            }
            if(k >= max_random_iters)
                break;
        }

        if(i < sample_size)
            continue;

        // find 1 or 3 fundamental matrices out of the 7 point correspondences
        n = icvFMatrix_7Point(ms0, ms1, ff);

        if(n < 1 || n > 3)
            continue;

        // for each matrix calculate the backprojection error
        // (distance to the corresponding epipolar lines) for each point and thus find
        // the number of in-liers.
        for(k = 0; k < n; k++)
        {
            const double* f = ff + k*9;
            int good_count = 0;

            for(i = 0; i < count; i++)
            {
                double d0, d1, s0, s1;

                double a = f[0]*m0[i].x + f[1]*m0[i].y + f[2];
                double b = f[3]*m0[i].x + f[4]*m0[i].y + f[5];
                double c = f[6]*m0[i].x + f[7]*m0[i].y + f[8];

                s1 = a*a + b*b;
                d1 = m1[i].x*a + m1[i].y*b + c;

                a = f[0]*m1[i].x + f[3]*m1[i].y + f[6];
                b = f[1]*m1[i].x + f[4]*m1[i].y + f[7];
                c = f[2]*m1[i].x + f[5]*m1[i].y + f[8];

                s0 = a*a + b*b;
                d0 = m0[i].x*a + m0[i].y*b + c;

                curr_mask[i] = d1*d1 < threshold*s1 && d0*d0 < threshold*s0;
                good_count += curr_mask[i];
            }

            if(good_count > MAX(best_good_count, 6))
            {
                double ep, lp, lep;
                int new_max_samples;

                // update the current best fundamental matrix and "goodness" flags
                if(mask)
                    memcpy(mask, curr_mask, count);
                memcpy(fmatrix, f, 9*sizeof(f[0]));
                best_good_count = good_count;

                // try to update (decrease) <max_samples>
                ep = (double)(count - good_count)/count;
                lp = log(1. - p);
                lep = log(1. - pow(ep,7.));
                if(lp < lep || lep >= 0)
                    break;
                else
                {
                    new_max_samples = cRound(lp/lep);
                    max_samples = MIN(new_max_samples, max_samples);
                }
            }
        }
    }

    if(best_good_count < 7)
        EXIT;

    result = 1;

    // optionally, use 8-point algorithm to compute fundamental matrix using only the in-liers
    if(best_good_count >= 8 && use_8point)
        result = icvFMatrix_8Point(m0, m1, mask, count, fmatrix);

    __END__;

    cFree(&temp_mask);
    cFree(&curr_mask);

    return result;
}


/***************************** Least Median of Squares algorithm ************************/

static CC_IMPLEMENT_QSORT(icvSortDistances, int, CC_LT)

/* the algorithm is quite similar to RANSAC, but here we choose the matrix that gives
   the least median of d(m0[i], F'*m1[i])^2 + d(m1[i], F*m0[i])^2 (0<=i<count),
   instead of choosing the matrix that gives the least number of outliers (as it is done in RANSAC) */
static int
icvFMatrix_LMedS(const CPoint2D64f* m0, const CPoint2D64f* m1,
                  uchar* mask, int count, double* fmatrix,
                  double threshold, double p,
                  unsigned rng_seed, int use_8point)
{
    int result = 0;

    const int max_random_iters = 1000;
    const int sample_size = 7;

    float* dist = 0;
    uchar* curr_mask = 0;
    uchar* temp_mask = 0;

    CC_FUNCNAME("icvFMatrix_LMedS");

    __BEGIN__;

    double ff[9*3];
    CRNG rng = cRNG(rng_seed);
    int i, j, k, sample_count, max_samples = 500;
    double least_median = DBL_MAX, median;
    int best_good_count = 0;

    assert(m0 && m1 && fmatrix && 0 < p && p < 1 && threshold > 0);

    threshold *= threshold;

    CC_CALL(curr_mask = (uchar*)cAlloc(count));
    CC_CALL(dist = (float*)cAlloc(count*sizeof(dist[0])));

    if(!mask && use_8point)
    {
        CC_CALL(temp_mask = (uchar*)cAlloc(count));
        mask = temp_mask;
    }

    // find the best fundamental matrix (giving the least backprojection error)
    // by picking at most <max_samples> 7-tuples of corresponding points
    // <max_samples> may be updated (decreased) within the loop based on statistics of outliers
    for(sample_count = 0; sample_count < max_samples; sample_count++)
    {
        int idx[sample_size], n;
        CPoint2D64f ms0[sample_size], ms1[sample_size];

        // choose random <sample_size> (=7) points
        for(i = 0; i < sample_size; i++)
        {
            for(k = 0; k < max_random_iters; k++)
            {
                idx[i] = cvRandInt(&rng) % count;
                for(j = 0; j < i; j++)
                    if(idx[j] == idx[i])
                        break;
                if(j == i)
                {
                    ms0[i] = m0[idx[i]];
                    ms1[i] = m1[idx[i]];
                    break;
                }
            }
            if(k >= max_random_iters)
                break;
        }

        if(i < sample_size)
            continue;

        // find 1 or 3 fundamental matrix out of the 7 point correspondences
        n = icvFMatrix_7Point(ms0, ms1, ff);

        if(n < 1 || n > 3)
            continue;

        // for each matrix calculate the backprojection error
        // (distance to the corresponding epipolar lines) for each point and thus find
        // the number of in-liers.
        for(k = 0; k < n; k++)
        {
            const double* f = ff + k*9;
            int good_count = 0;

            for(i = 0; i < count; i++)
            {
                double d0, d1, s;

                double a = f[0]*m0[i].x + f[1]*m0[i].y + f[2];
                double b = f[3]*m0[i].x + f[4]*m0[i].y + f[5];
                double c = f[6]*m0[i].x + f[7]*m0[i].y + f[8];

                s = 1./(a*a + b*b);
                d1 = m1[i].x*a + m1[i].y*b + c;
                d1 = s*d1*d1;

                a = f[0]*m1[i].x + f[3]*m1[i].y + f[6];
                b = f[1]*m1[i].x + f[4]*m1[i].y + f[7];
                c = f[2]*m1[i].x + f[5]*m1[i].y + f[8];

                s = 1./(a*a + b*b);
                d0 = m0[i].x*a + m0[i].y*b + c;
                d0 = s*d0*d0;

                curr_mask[i] = d1 < threshold && d0 < threshold;
                good_count += curr_mask[i];

                dist[i] = (float)(d0 + d1);
            }

            icvSortDistances((int*)dist, count, 0);
            median = (double)dist[count/2];

            if(median < least_median)
            {
                double ep, lp, lep;
                int new_max_samples;

                // update the current best fundamental matrix and "goodness" flags
                if(mask)
                    memcpy(mask, curr_mask, count);
                memcpy(fmatrix, f, 9*sizeof(f[0]));
                least_median = median;
                best_good_count = good_count;

                // try to update (decrease) <max_samples>
                ep = (double)(count - good_count)/count;
                lp = log(1. - p);
                lep = log(1. - pow(ep,7.));
                if(lp < lep || lep >= 0)
                    break;
                else
                {
                    new_max_samples = cRound(lp/lep);
                    max_samples = MIN(new_max_samples, max_samples);
                }
            }
        }
    }

    if(best_good_count < 7)
        EXIT;

    result = 1;

    // optionally, use 8-point algorithm to compute fundamental matrix using only the in-liers
    if(best_good_count >= 8 && use_8point)
        result = icvFMatrix_8Point(m0, m1, mask, count, fmatrix);

    __END__;

    cFree(&temp_mask);
    cFree(&curr_mask);
    cFree(&dist);

    return result;
}

#define CC_FM_7POINT 1
#define CC_FM_8POINT 2
#define CC_FM_LMEDS_ONLY  4
#define CC_FM_RANSAC_ONLY 8
#define CC_FM_LMEDS (CC_FM_LMEDS_ONLY + CC_FM_8POINT)
#define CC_FM_RANSAC (CC_FM_RANSAC_ONLY + CC_FM_8POINT)

CC_IMPL int
cvFindFundamentalMat1(const img_t* points0, const img_t* points1,
                      img_t* fmatrix, int method,
                      double param1, double param2, img_t* status)
{
    const unsigned rng_seed = 0xffffffff;
    int result = 0;
    int pt_alloc_flag[2] = { 0, 0 };
    int i, k;
    CPoint2D64f* pt[2] = { 0, 0 };
    img_t _status[1] = {0};

    CC_FUNCNAME("cvFindFundamentalMat");

    __BEGIN__;

    int count, dims;
    TypeId type;
    int cn;
    uchar* status_data = 0;
    double fmatrix_data0[9*3];
    double* fmatrix_data = 0;

    if(!(points0))
        CC_ERROR(!points0 ? CC_StsNullPtr : CC_StsBadArg, "points0 is not a valid matrix");

    if(!(points1))
        CC_ERROR(!points1 ? CC_StsNullPtr : CC_StsBadArg, "points1 is not a valid matrix");

    if(!CC_ARE_TYPES_EQ(points0, points1))
        CC_ERROR(CC_StsUnmatchedFormats, "The matrices of points should have the same data type");

    if(!CC_ARE_SIZES_EQ(points0, points1))
        CC_ERROR(CC_StsUnmatchedSizes, "The matrices of points should have the same size");

    type = CC_MAT_DEPTH(points0);
    cn = CC_MAT_CN(points0);
    if(type != CC_32S && type != CC_32F && type != CC_64F || cn != 1 && cn != 2 && cn != 3)
        CC_ERROR(CC_StsUnsupportedFormat, "The format of point matrices is unsupported");

    if(points0->rows > points0->cols)
    {
        dims = cn*points0->cols;
        count = points0->rows;
    }
    else
    {
        if(points0->rows > 1 && cn > 1 || points0->rows == 1 && cn == 1)
            CC_ERROR(CC_StsBadSize, "The point matrices do not have a proper layout (2xn, 3xn, nx2 or nx3)");
        dims = cn * points0->rows;
        count = points0->cols;
    }

    if(dims != 2 && dims != 3)
        CC_ERROR(CC_StsOutOfRange, "The dimensionality of points must be 2 or 3");

    if(method == CC_FM_7POINT && count != 7 ||
        method != CC_FM_7POINT && count < 7 + (method == CC_FM_8POINT))
        CC_ERROR(CC_StsOutOfRange,
        "The number of points must be 7 for 7-point algorithm, "
        ">=8 for 8-point algorithm and >=7 for other algorithms");

    if(!(fmatrix))
        CC_ERROR(!fmatrix ? CC_StsNullPtr : CC_StsBadArg, "fmatrix is not a valid matrix");

    if(CC_MAT_TYPE(fmatrix) != CC_32F && CC_MAT_TYPE(fmatrix) != CC_64F)
        CC_ERROR(CC_StsUnsupportedFormat, "fundamental matrix must have 32fC1 or 64fC1 type");

    if(fmatrix->cols != 3 || (fmatrix->rows != 3 && (method != CC_FM_7POINT || fmatrix->rows != 9)))
        CC_ERROR(CC_StsBadSize, "fundamental matrix must be 3x3 or 3x9 (for 7-point method only)");

    fmatrix_data = fmatrix->tt.db;
    if(!CC_IS_MAT_CONT(fmatrix) || CC_MAT_TYPE(fmatrix) != CC_64F ||
        method == CC_FM_7POINT && fmatrix->rows != 9)
        fmatrix_data = fmatrix_data0;

    if(status)
    {
        if(!(status))
            CC_ERROR(CC_StsBadArg, "The output status is not a valid matrix");

        if(status->cols != 1 && status->rows != 1 || status->cols + status->rows - 1 != count)
            CC_ERROR(CC_StsUnmatchedSizes,
            "The status matrix must have the same size as the point matrices");

        if(method == CC_FM_7POINT || method == CC_FM_8POINT)
            cvSet(status, cScalarAll(1.));
        else
        {
            status_data = status->tt.data;
            if(!CC_IS_MAT_CONT(status) || !CC_IS_MASK_ARR(status))
            {
                CC_CALL(cvCreateMat(_status, status->rows, status->cols, CC_8U, 1));
                status_data = _status->tt.data;
            }
        }
    }

    for(k = 0; k < 2; k++)
    {
        const img_t* spt = k == 0 ? points0 : points1;
        CPoint2D64f* dpt = pt[k] = (CPoint2D64f*)spt->tt.db;
        int plane_stride, stride, elem_size;

        if(CC_IS_MAT_CONT(spt) && CC_MAT_TYPE(spt) == CC_64F &&
            dims == 2 && (spt->rows == 1 || spt->rows == count))
            continue;

        elem_size = CC_TYPE_SIZE(type);

        if(spt->rows == dims)
        {
            plane_stride = spt->step / elem_size;
            stride = 1;
        }
        else
        {
            plane_stride = 1;
            stride = spt->rows == 1 ? dims : spt->step / elem_size;
        }

        CC_CALL(dpt = pt[k] = (CPoint2D64f*)cAlloc(count*sizeof(dpt[0])));
        pt_alloc_flag[k] = 1;

        if(type == CC_32F)
        {
            const float* xp = spt->tt.fl;
            const float* yp = xp + plane_stride;
            const float* zp = dims == 3 ? yp + plane_stride : 0;

            for(i = 0; i < count; i++)
            {
                double x = *xp, y = *yp;
                xp += stride;
                yp += stride;
                if(dims == 3)
                {
                    double z = *zp;
                    zp += stride;
                    z = z ? 1./z : 1.;
                    x *= z;
                    y *= z;
                }
                dpt[i].x = x;
                dpt[i].y = y;
            }
        }
        else
        {
            const double* xp = spt->tt.db;
            const double* yp = xp + plane_stride;
            const double* zp = dims == 3 ? yp + plane_stride : 0;

            for(i = 0; i < count; i++)
            {
                double x = *xp, y = *yp;
                xp += stride;
                yp += stride;
                if(dims == 3)
                {
                    double z = *zp;
                    zp += stride;
                    z = z ? 1./z : 1.;
                    x *= z;
                    y *= z;
                }
                dpt[i].x = x;
                dpt[i].y = y;
            }
        }
    }

    if(method == CC_FM_7POINT)
        result = icvFMatrix_7Point(pt[0], pt[1], fmatrix_data);
    else if(method == CC_FM_8POINT)
        result = icvFMatrix_8Point(pt[0], pt[1], 0, count, fmatrix_data);
    else
    {
        if(param1 < 0)
            CC_ERROR(CC_StsOutOfRange, "param1 (threshold) must be > 0");

        if(param2 < 0 || param2 > 1)
            CC_ERROR(CC_StsOutOfRange, "param2 (confidence level) must be between 0 and 1");

        if(param2 < DBL_EPSILON || param2 > 1 - DBL_EPSILON)
            param2 = 0.99;

        if(method < CC_FM_RANSAC_ONLY)
            result = icvFMatrix_LMedS(pt[0], pt[1], status_data, count, fmatrix_data,
                                       param1, param2, rng_seed, method & CC_FM_8POINT);
        else
            result = icvFMatrix_RANSAC(pt[0], pt[1], status_data, count, fmatrix_data,
                                        param1, param2, rng_seed, method & CC_FM_8POINT);
    }

    if(result && fmatrix->tt.db != fmatrix_data)
    {
        img_t hdr;
        cvZeroMat(fmatrix);
        cvMat(&hdr, MIN(fmatrix->rows, result*3), fmatrix->cols, CC_64F, 1, fmatrix_data);
        cvConvert(&hdr, fmatrix);
    }

    if(status && status_data && status->tt.data != status_data)
        cvConvert(_status, status);

    __END__;

    imfree(_status);
    for(k = 0; k < 2; k++)
        if(pt_alloc_flag[k])
            cFree(&pt[k]);

    return result;
}


CC_IMPL void
cvComputeCorrespondEpilines1(const img_t* points, int pointImageID,
                             const img_t* fmatrix, img_t* lines)
{
    CC_FUNCNAME("cvComputeCorrespondEpilines");

    __BEGIN__;

    TypeId type, abc_type;
    int abc_stride, abc_plane_stride, abc_elem_size;
    int plane_stride, stride, elem_size;
    int i, dims, count, cn, abc_dims, abc_count, abc_cn;
    uchar *ap, *bp, *cp;
    const uchar *xp, *yp, *zp;
    double f[9];
    img_t F; cvMat(&F, 3, 3, CC_64F, 1, f);

    if(!(points))
        CC_ERROR(!points ? CC_StsNullPtr : CC_StsBadArg, "points parameter is not a valid matrix");

    type = CC_MAT_DEPTH(points);
    cn = CC_MAT_CN(points);
    if(type != CC_32F && type != CC_64F || cn != 1 && cn != 2 && cn != 3)
        CC_ERROR(CC_StsUnsupportedFormat, "The format of point matrix is unsupported");

    if(points->rows > points->cols)
    {
        dims = cn*points->cols;
        count = points->rows;
    }
    else
    {
        if(points->rows > 1 && cn > 1 || points->rows == 1 && cn == 1)
            CC_ERROR(CC_StsBadSize, "The point matrix does not have a proper layout (2xn, 3xn, nx2 or nx3)");
        dims = cn * points->rows;
        count = points->cols;
    }

    if(dims != 2 && dims != 3)
        CC_ERROR(CC_StsOutOfRange, "The dimensionality of points must be 2 or 3");

    if(!(fmatrix))
        CC_ERROR(!fmatrix ? CC_StsNullPtr : CC_StsBadArg, "fmatrix is not a valid matrix");

    if(CC_MAT_TYPE(fmatrix) != CC_32F && CC_MAT_TYPE(fmatrix) != CC_64F)
        CC_ERROR(CC_StsUnsupportedFormat, "fundamental matrix must have 32fC1 or 64fC1 type");

    if(fmatrix->cols != 3 || fmatrix->rows != 3)
        CC_ERROR(CC_StsBadSize, "fundamental matrix must be 3x3");

    if(!(lines))
        CC_ERROR(!lines ? CC_StsNullPtr : CC_StsBadArg, "lines parameter is not a valid matrix");

    abc_type = CC_MAT_DEPTH(lines);
    abc_cn = CC_MAT_CN(lines);
    if(abc_type != CC_32F && abc_type != CC_64F || abc_cn != 1 && abc_cn != 3)
        CC_ERROR(CC_StsUnsupportedFormat, "The format of the matrix of lines is unsupported");

    if(lines->rows > lines->cols)
    {
        abc_dims = abc_cn*lines->cols;
        abc_count = lines->rows;
    }
    else
    {
        if(lines->rows > 1 && abc_cn > 1 || lines->rows == 1 && abc_cn == 1)
            CC_ERROR(CC_StsBadSize, "The lines matrix does not have a proper layout (3xn or nx3)");
        abc_dims = abc_cn * lines->rows;
        abc_count = lines->cols;
    }

    if(abc_dims != 3)
        CC_ERROR(CC_StsOutOfRange, "The lines matrix does not have a proper layout (3xn or nx3)");

    if(abc_count != count)
        CC_ERROR(CC_StsUnmatchedSizes, "The numbers of points and lines are different");

    elem_size = CC_TYPE_SIZE(type);
    abc_elem_size = CC_TYPE_SIZE(abc_type);

    if(points->rows == dims)
    {
        plane_stride = points->step;
        stride = elem_size;
    }
    else
    {
        plane_stride = elem_size;
        stride = points->rows == 1 ? dims*elem_size : points->step;
    }

    if(lines->rows == 3)
    {
        abc_plane_stride = lines->step;
        abc_stride = abc_elem_size;
    }
    else
    {
        abc_plane_stride = abc_elem_size;
        abc_stride = lines->rows == 1 ? 3*abc_elem_size : lines->step;
    }

    CC_CALL(cvConvert(fmatrix, &F));
    if(pointImageID == 2)
        cvTranspose(&F, &F);

    xp = points->tt.data;
    yp = xp + plane_stride;
    zp = dims == 3 ? yp + plane_stride : 0;

    ap = lines->tt.data;
    bp = ap + abc_plane_stride;
    cp = bp + abc_plane_stride;

    for(i = 0; i < count; i++)
    {
        double x, y, z = 1.;
        double a, b, c, nu;

        if(type == CC_32F)
        {
            x = *(float*)xp; y = *(float*)yp;
            if(zp)
                z = *(float*)zp, zp += stride;
        }
        else
        {
            x = *(double*)xp; y = *(double*)yp;
            if(zp)
                z = *(double*)zp, zp += stride;
        }

        xp += stride; yp += stride;

        a = f[0]*x + f[1]*y + f[2]*z;
        b = f[3]*x + f[4]*y + f[5]*z;
        c = f[6]*x + f[7]*y + f[8]*z;
        nu = a*a + b*b;
        nu = nu ? 1./sqrt(nu) : 1.;
        a *= nu; b *= nu; c *= nu;

        if(abc_type == CC_32F)
        {
            *(float*)ap = (float)a;
            *(float*)bp = (float)b;
            *(float*)cp = (float)c;
        }
        else
        {
            *(double*)ap = a;
            *(double*)bp = b;
            *(double*)cp = c;
        }

        ap += abc_stride;
        bp += abc_stride;
        cp += abc_stride;
    }

    __END__;
}


CC_IMPL void
cvConvertPointsHomogenious(const img_t* src, img_t* dst)
{
    img_t temp[1] = {0};
    img_t denom[1] = {0};

    CC_FUNCNAME("cvConvertPointsHomogenious");

    __BEGIN__;

    int i, s_count, s_dims, d_count, d_dims;
    img_t _src, _dst, _ones;
    img_t* ones = 0;

    if(!(src))
        CC_ERROR(!src ? CC_StsNullPtr : CC_StsBadArg,
        "The input parameter is not a valid matrix");

    if(!(dst))
        CC_ERROR(!dst ? CC_StsNullPtr : CC_StsBadArg,
        "The output parameter is not a valid matrix");

    if(src == dst || src->tt.data == dst->tt.data)
    {
        if(src != dst && (!CC_ARE_TYPES_EQ(src, dst) || !CC_ARE_SIZES_EQ(src,dst)))
            CC_ERROR(CC_StsBadArg, "Invalid inplace operation");
        EXIT;
    }

    if(src->rows > src->cols)
    {
        if(!((src->cols > 1) ^ (CC_MAT_CN(src) > 1)))
            CC_ERROR(CC_StsBadSize, "Either the number of channels or columns or rows must be =1");

        s_dims = CC_MAT_CN(src)*src->cols;
        s_count = src->rows;
    }
    else
    {
        if(!((src->rows > 1) ^ (CC_MAT_CN(src) > 1)))
            CC_ERROR(CC_StsBadSize, "Either the number of channels or columns or rows must be =1");

        s_dims = CC_MAT_CN(src)*src->rows;
        s_count = src->cols;
    }

    if(src->rows == 1 || src->cols == 1)
        src = cvReshape(src, &_src, 1, s_count);

    if(dst->rows > dst->cols)
    {
        if(!((dst->cols > 1) ^ (CC_MAT_CN(dst) > 1)))
            CC_ERROR(CC_StsBadSize,
            "Either the number of channels or columns or rows in the input matrix must be =1");

        d_dims = CC_MAT_CN(dst)*dst->cols;
        d_count = dst->rows;
    }
    else
    {
        if(!((dst->rows > 1) ^ (CC_MAT_CN(dst) > 1)))
            CC_ERROR(CC_StsBadSize,
            "Either the number of channels or columns or rows in the output matrix must be =1");

        d_dims = CC_MAT_CN(dst)*dst->rows;
        d_count = dst->cols;
    }

    if(dst->rows == 1 || dst->cols == 1)
        dst = cvReshape(dst, &_dst, 1, d_count);

    if(s_count != d_count)
        CC_ERROR(CC_StsUnmatchedSizes, "Both matrices must have the same number of points");

    if(CC_MAT_TYPE(src) < CC_32F || CC_MAT_TYPE(dst) < CC_32F)
        CC_ERROR(CC_StsUnsupportedFormat,
        "Both matrices must be floating-point (single or double precision)");

    if(s_dims < 2 || s_dims > 4 || d_dims < 2 || d_dims > 4)
        CC_ERROR(CC_StsOutOfRange,
        "Both input and output point dimensionality must be 2, 3 or 4");

    if(s_dims < d_dims - 1 || s_dims > d_dims + 1)
        CC_ERROR(CC_StsUnmatchedSizes,
        "The dimensionalities of input and output point sets differ too much");

    if(s_dims == d_dims - 1)
    {
        if(d_count == dst->rows)
        {
            ones = cvGetSubRect(dst, &_ones, cRect(s_dims, 0, 1, d_count));
            dst = cvGetSubRect(dst, &_dst, cRect(0, 0, s_dims, d_count));
        }
        else
        {
            ones = cvGetSubRect(dst, &_ones, cRect(0, s_dims, d_count, 1));
            dst = cvGetSubRect(dst, &_dst, cRect(0, 0, d_count, s_dims));
        }
    }

    if(s_dims <= d_dims)
    {
        if(src->rows == dst->rows && src->cols == dst->cols)
        {
            if(CC_ARE_TYPES_EQ(src, dst))
                cvCopy(src, dst);
            else
                cvConvert(src, dst);
        }
        else
        {
            if(!CC_ARE_TYPES_EQ(src, dst))
            {
                CC_CALL(cvCreateMat(temp, src->rows, src->cols, dst->tid));
                cvConvert(src, temp);
                src = temp;
            }
            cvTranspose(src, dst);
        }

        if(ones)
            cvSet(ones, cRealScalar(1.));
    }
    else
    {
        int s_plane_stride, s_stride, d_plane_stride, d_stride, elem_size;

        if(!CC_ARE_TYPES_EQ(src, dst))
        {
            CC_CALL(cvCreateMat(temp, src->rows, src->cols, dst->tid));
            cvConvert(src, temp);
            src = temp;
        }

        elem_size = CC_TYPE_SIZE(src->tid);

        if(s_count == src->cols)
            s_plane_stride = src->step / elem_size, s_stride = 1;
        else
            s_stride = src->step / elem_size, s_plane_stride = 1;

        if(d_count == dst->cols)
            d_plane_stride = dst->step / elem_size, d_stride = 1;
        else
            d_stride = dst->step / elem_size, d_plane_stride = 1;

        CC_CALL(cvCreateMat(denom, 1, d_count, dst->tid));

        if(CC_MAT_TYPE(dst) == CC_32F)
        {
            const float* xs = src->tt.fl;
            const float* ys = xs + s_plane_stride;
            const float* zs = 0;
            const float* ws = xs + (s_dims - 1)*s_plane_stride;

            float* iw = denom->tt.fl;

            float* xd = dst->tt.fl;
            float* yd = xd + d_plane_stride;
            float* zd = 0;

            if(d_dims == 3)
            {
                zs = ys + s_plane_stride;
                zd = yd + d_plane_stride;
            }

            for(i = 0; i < d_count; i++, ws += s_stride)
            {
                float t = *ws;
                iw[i] = t ? t : 1.f;
            }

            cvDiv(0, denom, denom);

            if(d_dims == 3)
                for(i = 0; i < d_count; i++)
                {
                    float w = iw[i];
                    float x = *xs * w, y = *ys * w, z = *zs * w;
                    xs += s_stride; ys += s_stride; zs += s_stride;
                    *xd = x; *yd = y; *zd = z;
                    xd += d_stride; yd += d_stride; zd += d_stride;
                }
            else
                for(i = 0; i < d_count; i++)
                {
                    float w = iw[i];
                    float x = *xs * w, y = *ys * w;
                    xs += s_stride; ys += s_stride;
                    *xd = x; *yd = y;
                    xd += d_stride; yd += d_stride;
                }
        }
        else
        {
            const double* xs = src->tt.db;
            const double* ys = xs + s_plane_stride;
            const double* zs = 0;
            const double* ws = xs + (s_dims - 1)*s_plane_stride;

            double* iw = denom->tt.db;

            double* xd = dst->tt.db;
            double* yd = xd + d_plane_stride;
            double* zd = 0;

            if(d_dims == 3)
            {
                zs = ys + s_plane_stride;
                zd = yd + d_plane_stride;
            }

            for(i = 0; i < d_count; i++, ws += s_stride)
            {
                double t = *ws;
                iw[i] = t ? t : 1.;
            }

            cvDiv(0, denom, denom);

            if(d_dims == 3)
                for(i = 0; i < d_count; i++)
                {
                    double w = iw[i];
                    double x = *xs * w, y = *ys * w, z = *zs * w;
                    xs += s_stride; ys += s_stride; zs += s_stride;
                    *xd = x; *yd = y; *zd = z;
                    xd += d_stride; yd += d_stride; zd += d_stride;
                }
            else
                for(i = 0; i < d_count; i++)
                {
                    double w = iw[i];
                    double x = *xs * w, y = *ys * w;
                    xs += s_stride; ys += s_stride;
                    *xd = x; *yd = y;
                    xd += d_stride; yd += d_stride;
                }
        }
    }

    __END__;

    imfree(denom);
    imfree(temp);
}


/* Find fundamental matrix */
CC_INLINE  void  cvFindFundamentalMatrix(int* points1, int* points2,
                            int numpoints, int method, float* matrix)
{
    img_t pointsMat1[1] = {0};
    img_t pointsMat2[1] = {0};
    img_t fundMatr; cvMat(&fundMatr, 3,3,CC_32F,1,matrix);
    int i, curr = 0;
    UNUSED(method);

    cvCreateMat(pointsMat1,3,numpoints,CC_64F,1);
    cvCreateMat(pointsMat2,3,numpoints,CC_64F,1);

    for(i = 0; i < numpoints; i++)
    {
        cvmSet(pointsMat1,0,i,points1[curr]);//x
        cvmSet(pointsMat1,1,i,points1[curr+1]);//y
        cvmSet(pointsMat1,2,i,1.0);

        cvmSet(pointsMat2,0,i,points2[curr]);//x
        cvmSet(pointsMat2,1,i,points2[curr+1]);//y
        cvmSet(pointsMat2,2,i,1.0);
        curr += 2;
    }

    cvFindFundamentalMat1(pointsMat1,pointsMat2,&fundMatr,CC_FM_RANSAC,1,0.99,0);

    imfree(pointsMat1);
    imfree(pointsMat2);
}


