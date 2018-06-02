
#define CCFUNC(NAME)  NAME
#define ISDOUBLE 0
#include "lapack_impl.inl"

#define CCFUNC(NAME)  NAME
#define ISDOUBLE 1
#include "lapack_impl.inl"

#include "matrix_decomp.inl"
#include "matmul.inl"

static int LU(float* A, size_t astep, int m, float* b, size_t bstep, int n)
{
    CC_INSTRUMENT_REGION()

    return hal_LU32f(A, astep, m, b, bstep, n);
}

static int LU(double* A, size_t astep, int m, double* b, size_t bstep, int n)
{
    CC_INSTRUMENT_REGION()

    return hal_LU64f(A, astep, m, b, bstep, n);
}

static int Cholesky(float* A, size_t astep, int m, float* b, size_t bstep, int n)
{
    CC_INSTRUMENT_REGION()

    return hal_Cholesky32f(A, astep, m, b, bstep, n);
}

static int Cholesky(double* A, size_t astep, int m, double* b, size_t bstep, int n)
{
    CC_INSTRUMENT_REGION()

    return hal_Cholesky64f(A, astep, m, b, bstep, n);
}

static int Jacobi( float* S, size_t sstep, float* e, float* E, size_t estep, int n, uchar* buf )
{
    return JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

static int Jacobi( double* S, size_t sstep, double* e, double* E, size_t estep, int n, uchar* buf )
{
    return JacobiImpl_(S, sstep, e, E, estep, n, buf);
}

static void hal_SVD32f(float* At, size_t astep, float* W, float* U, size_t ustep, float* Vt, size_t vstep, int m, int n, int n1)
{
    CALL_HAL(SVD32f, cv_hal_SVD32f, (At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1)))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, FLT_MIN, FLT_EPSILON*2);
}
static void hal_SVD64f(double* At, size_t astep, double* W, double* U, size_t ustep, double* Vt, size_t vstep, int m, int n, int n1)
{
    CALL_HAL(SVD64f, cv_hal_SVD64f, (At, astep, W, U, ustep, Vt, vstep, m, n, decodeSVDParameters(U, Vt, m, n, n1)))
    JacobiSVDImpl_(At, astep, W, Vt, vstep, m, n, !Vt ? 0 : n1 < 0 ? n : n1, DBL_MIN, DBL_EPSILON*10);
}
static void JacobiSVD(float* At, size_t astep, float* W, float* Vt, size_t vstep, int m, int n, int n1=-1)
{
    hal_SVD32f(At, astep, W, NULL, astep, Vt, vstep, m, n, n1);
}

static void JacobiSVD(double* At, size_t astep, double* W, double* Vt, size_t vstep, int m, int n, int n1=-1)
{
    hal_SVD64f(At, astep, W, NULL, astep, Vt, vstep, m, n, n1);
}

static void
SVBkSb( int m, int n, const float* w, size_t wstep,
        const float* u, size_t ustep, int uT,
        const float* v, size_t vstep, int vT,
        const float* b, size_t bstep, int nb,
        float* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (int)(wstep/sizeof(w[0])) : 1,
                u, (int)(ustep/sizeof(u[0])), uT,
                v, (int)(vstep/sizeof(v[0])), vT,
                b, (int)(bstep/sizeof(b[0])), nb,
                x, (int)(xstep/sizeof(x[0])),
                cAlignPtr(float, buffer, sizeof(double)), (float)(DBL_EPSILON*2) );
}

static void
SVBkSb( int m, int n, const double* w, size_t wstep,
       const double* u, size_t ustep, int uT,
       const double* v, size_t vstep, int vT,
       const double* b, size_t bstep, int nb,
       double* x, size_t xstep, uchar* buffer )
{
    SVBkSbImpl_(m, n, w, wstep ? (int)(wstep/sizeof(w[0])) : 1,
                u, (int)(ustep/sizeof(u[0])), uT,
                v, (int)(vstep/sizeof(v[0])), vT,
                b, (int)(bstep/sizeof(b[0])), nb,
                x, (int)(xstep/sizeof(x[0])),
                cAlignPtr(double, buffer, sizeof(double)), DBL_EPSILON*2 );
}



/****************************************************************************************\
*                                 Determinant of the matrix                              *
\****************************************************************************************/

#define _det2(m)   ((double)m(0,0)*m(1,1) - (double)m(0,1)*m(1,0))
#define _det3(m)   (m(0,0)*((double)m(1,1)*m(2,2) - (double)m(1,2)*m(2,1)) -  \
                   m(0,1)*((double)m(1,0)*m(2,2) - (double)m(1,2)*m(2,0)) +  \
                   m(0,2)*((double)m(1,0)*m(2,1) - (double)m(1,1)*m(2,0)))

static double determinant( const img_t* mat )
{
    CC_INSTRUMENT_REGION()
    double result = 0;
    TypeId type = CC_MAT_DEPTH(mat);
    int rows = mat->rows;
    size_t step = mat->step;
    const uchar* m = mat->tt.data;

    CC_Assert( mat->rows == mat->cols && (type == CC_32F || type == CC_64F));

    #define Mf(y, x) ((float*)(m + y*step))[x]
    #define Md(y, x) ((double*)(m + y*step))[x]

    if( type == CC_32F )
    {
        if( rows == 2 )
            result = _det2(Mf);
        else if( rows == 3 )
            result = _det3(Mf);
        else if( rows == 1 )
            result = Mf(0,0);
        else
        {
            int a_step = mat->cols*sizeof(float);
            float* a = (float*)memdup2d(mat->tt.data, mat->step, mat->rows, a_step);

            result = hal_LU32f(a, a_step, rows, 0, 0, 0);
            if( result )
            {
                for( int i = 0; i < rows; i++ )
                    result *= a[i*a_step+i];
            }
            FREE(a);
        }
    }
    else
    {
        if( rows == 2 )
            result = _det2(Md);
        else if( rows == 3 )
            result = _det3(Md);
        else if( rows == 1 )
            result = Md(0,0);
        else
        {
            int a_step = mat->cols*sizeof(double);
            double* a = (double*)memdup2d(mat->tt.data, mat->step, mat->rows, a_step);

            result = hal_LU64f(a, a_step, rows, 0, 0, 0);
            if( result )
            {
                for( int i = 0; i < rows; i++ )
                    result *= a[i*a_step+i];
            }
            FREE(a);
        }
    }

    #undef Mf
    #undef Md

    return result;
}

/****************************************************************************************\
*                              Solving a linear system                                   *
\****************************************************************************************/

static int solve( const img_t* src, const img_t* src2, img_t* dst, int method = CC_DECOMP_LU )
{
    CC_INSTRUMENT_REGION()

    int result = true;
    TypeId type = CC_MAT_DEPTH(src);
    int cn = CC_MAT_CN(src);
    int is_normal = (method & CC_DECOMP_NORMAL) != 0;

    CC_Assert( type == CC_MAT_TYPE(src2) && (type == CC_32F || type == CC_64F) );

    method &= ~CC_DECOMP_NORMAL;
    CC_Assert( (method != CC_DECOMP_LU && method != CC_DECOMP_CHOLESKY) ||
        is_normal || src->rows == src->cols );

    // check case of a single equation and small matrix
    if( (method == CC_DECOMP_LU || method == CC_DECOMP_CHOLESKY) && !is_normal &&
        src->rows <= 3 && src->rows == src->cols && src2->cols == 1 )
    {
        cvSetMat( dst, src->cols, src2->cols, CC_MAT_DEPTH(src), CC_MAT_CN(src) );
        #define bf(y) ((float*)(bdata + y*src2step))[0]
        #define bd(y) ((double*)(bdata + y*src2step))[0]

        const uchar* srcdata = src->tt.data;
        const uchar* bdata = src2->tt.data;
        uchar* dstdata = dst->tt.data;
        size_t srcstep = src->step;
        size_t src2step = src2->step;
        size_t dststep = dst->step;

        if( src->rows == 2 )
        {
            if( type == CC_32FC1 )
            {
                double d = _det2(Sf);
                if( d != 0. )
                {
                    double t;
                    d = 1./d;
                    t = (float)(((double)bf(0)*Sf(1,1) - (double)bf(1)*Sf(0,1))*d);
                    Df(1,0) = (float)(((double)bf(1)*Sf(0,0) - (double)bf(0)*Sf(1,0))*d);
                    Df(0,0) = (float)t;
                }
                else
                    result = false;
            }
            else
            {
                double d = _det2(Sd);
                if( d != 0. )
                {
                    double t;
                    d = 1./d;
                    t = (bd(0)*Sd(1,1) - bd(1)*Sd(0,1))*d;
                    Dd(1,0) = (bd(1)*Sd(0,0) - bd(0)*Sd(1,0))*d;
                    Dd(0,0) = t;
                }
                else
                    result = false;
            }
        }
        else if( src->rows == 3 )
        {
            if( type == CC_32FC1 )
            {
                double d = _det3(Sf);
                if( d != 0. )
                {
                    float t[3];
                    d = 1./d;

                    t[0] = (float)(d*
                           (bf(0)*((double)Sf(1,1)*Sf(2,2) - (double)Sf(1,2)*Sf(2,1)) -
                            Sf(0,1)*((double)bf(1)*Sf(2,2) - (double)Sf(1,2)*bf(2)) +
                            Sf(0,2)*((double)bf(1)*Sf(2,1) - (double)Sf(1,1)*bf(2))));

                    t[1] = (float)(d*
                           (Sf(0,0)*(double)(bf(1)*Sf(2,2) - (double)Sf(1,2)*bf(2)) -
                            bf(0)*((double)Sf(1,0)*Sf(2,2) - (double)Sf(1,2)*Sf(2,0)) +
                            Sf(0,2)*((double)Sf(1,0)*bf(2) - (double)bf(1)*Sf(2,0))));

                    t[2] = (float)(d*
                           (Sf(0,0)*((double)Sf(1,1)*bf(2) - (double)bf(1)*Sf(2,1)) -
                            Sf(0,1)*((double)Sf(1,0)*bf(2) - (double)bf(1)*Sf(2,0)) +
                            bf(0)*((double)Sf(1,0)*Sf(2,1) - (double)Sf(1,1)*Sf(2,0))));

                    Df(0,0) = t[0];
                    Df(1,0) = t[1];
                    Df(2,0) = t[2];
                }
                else
                    result = false;
            }
            else
            {
                double d = _det3(Sd);
                if( d != 0. )
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
                    result = false;
            }
        }
        else
        {
            assert( src->rows == 1 );

            if( type == CC_32FC1 )
            {
                double d = Sf(0,0);
                if( d != 0. )
                    Df(0,0) = (float)(bf(0)/d);
                else
                    result = false;
            }
            else
            {
                double d = Sd(0,0);
                if( d != 0. )
                    Dd(0,0) = (bd(0)/d);
                else
                    result = false;
            }
        }
#undef bf
#undef bd
        return result;
    }

    int m = src->rows, m_ = m, n = src->cols, nb = src2->cols;
    size_t esz = CC_TYPE_SIZE(type), bufsize = 0;
    size_t vstep = cAlignSize(n*esz, 16);
    size_t astep = method == CC_DECOMP_SVD && !is_normal ? cAlignSize(m*esz, 16) : vstep;
    uchar* buffer = NULL;

    cvSetMat( dst, src->cols, src2->cols, CC_MAT_DEPTH(src), CC_MAT_CN(src) );

    if( m < n )
        CC_Error(CC_StsBadArg, "The function can not solve under-determined linear systems" );

    if( m == n )
        is_normal = false;
    else if( is_normal )
    {
        m_ = n;
        if( method == CC_DECOMP_SVD )
            method = CC_DECOMP_EIG;
    }

    size_t asize = astep*(method == CC_DECOMP_SVD || is_normal ? n : m);
    bufsize += asize + 32;

    if( is_normal )
        bufsize += n*nb*esz;
    if( method == CC_DECOMP_SVD || method == CC_DECOMP_EIG )
        bufsize += n*5*esz + n*vstep + nb*sizeof(double) + 32;

    buffer = MALLOC(uchar, bufsize);
    uchar* ptr = cAlignPtr(uchar, buffer, 16);

    img_t a[1] = {0};
    cvInitMatHeader(a, m_, n, type, cn, ptr, astep);

    if( is_normal )
        cvMulTransposed(src, a, true);
    else if( method != CC_DECOMP_SVD )
        cvCopy(src, a);
    else
    {
        cvMat(a, n, m_, type, cn, ptr, astep);
        cvTranspose(src, a);
    }
    ptr += asize;

    if( !is_normal )
    {
        if( method == CC_DECOMP_LU || method == CC_DECOMP_CHOLESKY )
            cvCopy(src2, dst);
    }
    else
    {
        // a'*b
        if( method == CC_DECOMP_LU || method == CC_DECOMP_CHOLESKY )
            gemm( src, src2, 1, NULL, 0, dst, CC_GEMM_1_T );
        else
        {
            img_t tmp[1] = {0};
            cvMat(tmp, n, nb, type, cn, ptr);
            ptr += n*nb*esz;
            gemm( src, src2, 1, NULL, 0, tmp, CC_GEMM_1_T );
            src2 = tmp;
        }
    }

    if( method == CC_DECOMP_LU )
    {
        if( type == CC_32F )
            result = hal_LU32f(a->tt.fl, a->step, n, dst->tt.fl, dst->step, nb) != 0;
        else
            result = hal_LU64f(a->tt.db, a->step, n, dst->tt.db, dst->step, nb) != 0;
    }
    else if( method == CC_DECOMP_CHOLESKY )
    {
        if( type == CC_32F )
            result = hal_Cholesky32f(a->tt.fl, a->step, n, dst->tt.fl, dst->step, nb);
        else
            result = hal_Cholesky64f(a->tt.db, a->step, n, dst->tt.db, dst->step, nb);
    }
    else if( method == CC_DECOMP_QR )
    {
		img_t rhsMat[1] = {0};
        if( is_normal || m == n )
        {
            cvCopy(src2, dst);
            cvCloneMat(rhsMat, dst);
        }
        else
        {
            cvSetMat(rhsMat, m, nb, type);
            cvCopy(src2, rhsMat);
        }

        if( type == CC_32F )
            result = hal_QR32f(a->tt.fl, a->step, a->rows, a->cols, rhsMat->cols, rhsMat->tt.fl, rhsMat->step, NULL) != 0;
        else
            result = hal_QR64f(a->tt.db, a->step, a->rows, a->cols, rhsMat->cols, rhsMat->tt.db, rhsMat->step, NULL) != 0;

        if (rhsMat->rows != dst->rows) {
            img_t tmp[1] = {0};
            cvGetRows(rhsMat, tmp, 0, dst->rows);
            cvCopy(tmp, dst);
        }
		imfree(rhsMat);
    }
    else
    {
        ptr = cAlignPtr(uchar, ptr, 16);
		img_t tmp[3] = { 0 };
        img_t* v = cvMat(tmp+0, n, n, type, cn, ptr, vstep);
        img_t* w = cvMat(tmp+1, n, 1, type, cn, ptr + vstep*n);
        img_t* u = NULL;
        ptr += n*(vstep + esz);

        if( method == CC_DECOMP_EIG )
        {
            if( type == CC_32F )
                Jacobi(a->tt.fl, a->step, w->tt.fl, v->tt.fl, v->step, n, ptr);
            else
                Jacobi(a->tt.db, a->step, w->tt.db, v->tt.db, v->step, n, ptr);
            u = v;
        }
        else
        {
            if( type == CC_32F )
                JacobiSVD(a->tt.fl, a->step, w->tt.fl, v->tt.fl, v->step, m_, n);
            else
                JacobiSVD(a->tt.db, a->step, w->tt.db, v->tt.db, v->step, m_, n);
            u = a;
        }

        if( type == CC_32F )
        {
            SVBkSb(m_, n, w->tt.fl, 0, u->tt.fl, u->step, true,
                   v->tt.fl, v->step, true, src2->tt.fl,
                   src2->step, nb, dst->tt.fl, dst->step, ptr);
        }
        else
        {
            SVBkSb(m_, n, w->tt.db, 0, u->tt.db, u->step, true,
                   v->tt.db, v->step, true, src2->tt.db,
                   src2->step, nb, dst->tt.db, dst->step, ptr);
        }
        result = true;
    }

    if( !result )
        cvZeroMat(dst);

    FREE(buffer);

    return result;
}

CC_IMPL int
icvSolve(const void* src, int src_step, const void* src2, int src2_step, void* dst, int dst_step, int rows, int cols, TypeId type, int method )
{
  img_t srcarr[1] = {0};
  img_t src2arr[1] = {0};
  img_t dstarr[1] = {0};
  cvInitMatHeader(srcarr, rows, cols, type, 1, src, src_step);
  cvInitMatHeader(src2arr, cols, 1, type, 1, src2, src2_step);
  cvInitMatHeader(dstarr, rows, 1, type, 1, dst, dst_step);
  return solve(srcarr, src2arr, dstarr, method);
}

/////////////////// finding eigenvalues and eigenvectors of a symmetric matrix ///////////////

static int eigen( const img_t* _src, img_t* _evals, img_t* _evects CC_DEFAULT(NULL) )
{
    CC_INSTRUMENT_REGION()

    const img_t* src = _src;
    TypeId type = CC_MAT_DEPTH(src);
    int n = src->rows;

    CC_Assert( src->rows == src->cols );
    CC_Assert (type == CC_32F || type == CC_64F);

    img_t* v = NULL;
    if( CC_MAT_NEEDED(_evects) )
    {
        cvSetMat(_evects, n, n, type);
        v = _evects;
    }

    size_t elemSize = CC_TYPE_SIZE(type), astep = cAlignSize(n*elemSize, 16);
    uchar* buf = MALLOC(uchar, n*astep + n*5*elemSize + 32);
    uchar* ptr = cAlignPtr(uchar, buf, 16);
    img_t a[1] = {0}, w[1] = {0};
    cvMat(a, n, n, type, 1, ptr, astep);
    cvMat(w, n, 1, type, 1, ptr + astep*n);
    ptr += astep*n + elemSize*n;
    cvCopy(src, a);
    int ok = type == CC_32F ?
        Jacobi(a->tt.fl, a->step, w->tt.fl, v->tt.fl, v->step, n, ptr) :
        Jacobi(a->tt.db, a->step, w->tt.db, v->tt.db, v->step, n, ptr);

    cvCopy(w, _evals);
    FREE(buf);
    return ok;
}

CC_IMPL void SVD_compute( const img_t* _aarr, img_t* _w,
                 img_t* _u, img_t* _vt, int flags CC_DEFAULT(0) )
{
    const img_t* src = _aarr;
    int m = src->rows, n = src->cols;
    TypeId type = CC_MAT_DEPTH(src);
    int compute_uv = CC_MAT_NEEDED(_u) || CC_MAT_NEEDED(_vt);
    int full_uv = (flags & CC_SVD_FULL_UV) != 0;

    CC_Assert( type == CC_32F || type == CC_64F );

    if( flags & CC_SVD_NO_UV )
    {
        //_u.release();
        //_vt.release();
        compute_uv = full_uv = false;
    }

    int at = false;
    if( m < n )
    {
        T_SWAP(int, m, n);
        at = true;
    }

    int urows = full_uv ? m : n;
    size_t esz = CC_TYPE_SIZE(type), astep = cAlignSize(m*esz, 16), vstep = cAlignSize(n*esz, 16);
    uchar* _buf = MALLOC(uchar, urows*astep + n*vstep + n*esz + 32);
    uchar* buf = cAlignPtr(uchar, _buf, 16);
	IM_DEF(tmp, 10);
    img_t* temp_a = cvMat(tmp++, n, m, type, 1, buf, astep);
    img_t* temp_w = cvMat(tmp++, n, 1, type, 1, buf + urows*astep);
    img_t* temp_u = cvMat(tmp++, urows, m, type, 1, buf, astep), *temp_v = tmp++;

    if( compute_uv )
        cvMat(temp_v, n, n, type, 1, cAlignPtr(uchar, buf + urows*astep + n*esz, 16), vstep);

    if( urows > n )
        cvZeroMat(temp_u);

    if( !at )
        cvTranspose(src, temp_a);
    else
        cvCopy(src, temp_a);

    if( type == CC_32F )
    {
        JacobiSVD(temp_a->tt.fl, temp_u->step, temp_w->tt.fl,
              temp_v->tt.fl, temp_v->step, m, n, compute_uv ? urows : 0);
    }
    else
    {
        JacobiSVD(temp_a->tt.db, temp_u->step, temp_w->tt.db,
              temp_v->tt.db, temp_v->step, m, n, compute_uv ? urows : 0);
    }
    cvCopy(temp_w, _w);
    if( compute_uv )
    {
        if( !at )
        {
            if( CC_MAT_NEEDED(_u) )
                cvTranspose(temp_u, _u);
            if( CC_MAT_NEEDED(_vt) )
                cvCopy(temp_v, _vt);
        }
        else
        {
            if( CC_MAT_NEEDED(_u) )
                cvTranspose(temp_v, _u);
            if( CC_MAT_NEEDED(_vt) )
                cvCopy(temp_u, _vt);
        }
    }
    FREE(_buf);
}

static void SVD_backSubst( const img_t* _w, const img_t* _u, const img_t* _vt, const img_t* _rhs, img_t* dst )
{
    const img_t *w = _w, *u = _u, *vt = _vt, *rhs = _rhs;
    TypeId type = CC_MAT_DEPTH(w);
    int esz = CC_TYPE_SIZE(type);
    int m = u->rows, n = vt->cols, nb = rhs->tt.data ? rhs->cols : m, nm = MIN(m, n);
    size_t wstep = w->rows == 1 ? (size_t)esz : w->cols == 1 ? (size_t)w->step : (size_t)w->step + esz;
    uchar* buffer = MALLOC(uchar, nb*sizeof(double) + 16);
    CC_Assert( CC_MAT_TYPE(w) == CC_MAT_TYPE(u) && CC_MAT_TYPE(u) == CC_MAT_TYPE(vt) && u->tt.data && vt->tt.data && w->tt.data );
    CC_Assert( u->cols >= nm && vt->rows >= nm &&
              (CC_ARE_SIZES_EQSZ(w, nm, 1) || CC_ARE_SIZES_EQSZ(w, 1, nm) || CC_ARE_SIZES_EQSZ(w, u->cols, vt->rows)) );
    CC_Assert( rhs->tt.data == 0 || (CC_MAT_TYPE(rhs) == type && rhs->rows == m) );

    cvSetMat( dst, n, nb, type );
    if( type == CC_32F )
        SVBkSb(m, n, w->tt.fl, wstep, u->tt.fl, u->step, false,
               vt->tt.fl, vt->step, true, rhs->tt.fl, rhs->step, nb,
               dst->tt.fl, dst->step, buffer);
    else if( type == CC_64F )
        SVBkSb(m, n, w->tt.db, wstep, u->tt.db, u->step, false,
               vt->tt.db, vt->step, true, rhs->tt.db, rhs->step, nb,
               dst->tt.db, dst->step, buffer);
    else
        CC_Error( CC_StsUnsupportedFormat, "" );
    FREE(buffer);
}

CC_IMPL void
cvEigenVV( img_t* srcarr, img_t* evectsarr, img_t* evalsarr, double,
           int, int )
{
    img_t *src = (srcarr), *evals0 = (evalsarr), *evals = evals0;
    if( evectsarr )
    {
        img_t *evects0 = (evectsarr), *evects = evects0;
        eigen(src, evals, evects);
        if( evects0->tt.data != evects->tt.data )
        {
            const uchar* p = evects0->tt.data;
            cvConvert(evects, evects0);
            CC_Assert( p == evects0->tt.data );
        }
    }
    else
        eigen(src, evals, NULL);
    if( evals0->tt.data != evals->tt.data )
    {
        const uchar* p = evals0->tt.data;
        if( CC_ARE_SIZES_EQ(evals0, evals) )
            cvConvert(evals, evals0);
        else if( CC_ARE_TYPES_EQ(evals0, evals) )
            cvTranspose(evals, evals0);
        else
            cvConvertTrans(evals, evals0, CC_MAT_DEPTH(evals0));
        CC_Assert( p == evals0->tt.data );
    }
}

#if 0

CC_IMPL void
cvSVD1( img_t* aarr, img_t* warr, img_t* uarr, img_t* varr, int flags )
{
    img_t *a = (aarr), *w = (warr), *u = NULL, *v = NULL;
    TypeId type = CC_MAT_DEPTH(a);
    int m = a->rows, n = a->cols, mn = MAX(m, n), nm = MIN(m, n);

    CC_Assert( CC_MAT_TYPE(w) == type &&
        (CC_ARE_SIZES_EQSZ(w, 1, nm) || CC_ARE_SIZES_EQSZ(w, nm, 1) ||
        CC_ARE_SIZES_EQSZ(w, nm, nm) || CC_ARE_SIZES_EQSZ(w, m, n)) );

    if( CC_ARE_SIZES_EQSZ(w, 1, nm) )
        w = img_t(nm, 1, type, w->tt.data );
    else if( w CC_IS_CONT_MAT() )
        svd.w = w;

    if( uarr )
    {
        u = cvarrToMat(uarr);
        CC_Assert( u CC_MAT_TYPE() == type );
        svd.u = u;
    }

    if( varr )
    {
        v = cvarrToMat(varr);
        CC_Assert( v CC_MAT_TYPE() == type );
        svd.vt = v;
    }

    SVD_compute(a, ((flags & CC_SVD_MODIFY_A) ? SVD_MODIFY_A : 0) |
        ((!svd.u.data && !svd.vt.data) ? SVD_NO_UV : 0) |
        ((m != n && (svd.u.size() == Size(mn, mn) ||
        svd.vt.size() == Size(mn, mn))) ? SVD_FULL_UV : 0));

    if( !u.empty() )
    {
        if( flags & CC_SVD_U_T )
            transpose( svd.u, u );
        else if( u.data != svd.u.data )
        {
            CC_Assert( u.size() == svd.u.size() );
            svd.u.copyTo(u);
        }
    }

    if( !v.empty() )
    {
        if( !(flags & CC_SVD_V_T) )
            transpose( svd.vt, v );
        else if( v.data != svd.vt.data )
        {
            CC_Assert( v.size() == svd.vt.size() );
            svd.vt.copyTo(v);
        }
    }

    if( w.data != svd.w.data )
    {
        if( w.size() == svd.w.size() )
            svd.w.copyTo(w);
        else
        {
            w = Scalar(0);
            img_t wd = w.diag();
            svd.w.copyTo(wd);
        }
    }
}


CC_IMPL void
cvSVBkSb( const img_t* warr, const img_t* uarr,
          const img_t* varr, const img_t* rhsarr,
          img_t* dstarr, int flags )
{
    img_t w = cvarrToMat(warr), u = cvarrToMat(uarr),
        v = cvarrToMat(varr), rhs,
        dst = cvarrToMat(dstarr), dst0 = dst;
    if( flags & CC_SVD_U_T )
    {
        img_t tmp;
        transpose(u, tmp);
        u = tmp;
    }
    if( !(flags & CC_SVD_V_T) )
    {
        img_t tmp;
        transpose(v, tmp);
        v = tmp;
    }
    if( rhsarr )
        rhs = cvarrToMat(rhsarr);

    SVD_backSubst(w, u, v, rhs, dst);
    CC_Assert( dst.data == dst0.data );
}


#endif

/****************************************************************************************\
*                          Inverse (or pseudo-inverse) of a matrix                       *
\****************************************************************************************/

#define Sf( y, x ) ((float*)(srcdata + y*srcstep))[x]
#define Sd( y, x ) ((double*)(srcdata + y*srcstep))[x]
#define Df( y, x ) ((float*)(dstdata + y*dststep))[x]
#define Dd( y, x ) ((double*)(dstdata + y*dststep))[x]

static double invert( const img_t* src, img_t* dst, int method = CC_DECOMP_LU )
{
    CC_INSTRUMENT_REGION()

    int result = false;
    TypeId type = CC_MAT_DEPTH(src);
    CC_Assert(type == CC_32F || type == CC_64F);

    size_t esz = CC_TYPE_SIZE(type);
    int m = src->rows, n = src->cols;

    if( method == CC_DECOMP_SVD )
    {
        int nm = MIN(m, n);

        uchar* _buf = MALLOC(uchar, (m*nm + nm + nm*n)*esz + sizeof(double));
        uchar* buf = cAlignPtr(uchar, _buf, (int)esz);
        IM_DEF(tmp, 4);
        img_t *u = cvMat(tmp++, m, nm, type, 1, buf);
        img_t *w = cvMat(tmp++, nm, 1, type, 1, u->tt.data + m*nm*esz);
        img_t *vt = cvMat(tmp++, nm, n, type, 1, w->tt.data + nm*esz);

        SVD_compute(src, w, u, vt);
        SVD_backSubst(w, u, vt, NULL, dst);
        FREE(_buf);
        return type == CC_32F ?
            (w->tt.fl[0] >= FLT_EPSILON ? w->tt.fl[n-1]/w->tt.fl[0] : 0) :
            (w->tt.db[0] >= DBL_EPSILON ? w->tt.db[n-1]/w->tt.db[0] : 0);
    }

    CC_Assert( m == n );

    if( method == CC_DECOMP_EIG )
    {
        uchar* _buf = MALLOC(uchar, (n*n*2 + n)*esz + sizeof(double));
        uchar* buf = cAlignPtr(uchar, _buf, (int)esz);
        IM_DEF(tmp, 4);
        img_t *u = cvMat(tmp++, n, n, type, 1, buf);
        img_t *w = cvMat(tmp++, n, 1, type, 1, u->tt.data + n*n*esz);
        img_t *vt = cvMat(tmp++, n, n, type, 1, w->tt.data + n*esz);

        eigen(src, w, vt);
        transpose(vt, u);
        SVD_backSubst(w, u, vt, NULL, dst);
        FREE(_buf);
        return type == CC_32F ?
        (w->tt.fl[0] >= FLT_EPSILON ? w->tt.fl[n-1]/w->tt.fl[0] : 0) :
        (w->tt.db[0] >= DBL_EPSILON ? w->tt.db[n-1]/w->tt.db[0] : 0);
    }

    CC_Assert( method == CC_DECOMP_LU || method == CC_DECOMP_CHOLESKY );

    cvSetMat( dst, n, n, type );

    if( n <= 3 )
    {
        const uchar* srcdata = src->tt.data;
        uchar* dstdata = dst->tt.data;
        size_t srcstep = src->step;
        size_t dststep = dst->step;

        if( n == 2 )
        {
            if( type == CC_32FC1 )
            {
                double d = _det2(Sf);
                if( d != 0. )
                {
                    result = true;
                    d = 1./d;

                    #if CC_SSE2
                    if(USE_SSE2)
                    {
                            __m128 zero = _mm_setzero_ps();
                            __m128 t0 = _mm_loadl_pi(zero, (const __m64*)srcdata); //t0 = sf(0,0) sf(0,1)
                            __m128 t1 = _mm_loadh_pi(zero, (const __m64*)(srcdata+srcstep)); //t1 = sf(1,0) sf(1,1)
                            __m128 s0 = _mm_or_ps(t0, t1);
                            __m128 det =_mm_set1_ps((float)d);
                            s0 =  _mm_mul_ps(s0, det);
                            static const uchar CC_DECL_ALIGNED(16) inv[16] = {0,0,0,0,0,0,0,0x80,0,0,0,0x80,0,0,0,0};
                            __m128 pattern = _mm_load_ps((const float*)inv);
                            s0 = _mm_xor_ps(s0, pattern);//==-1*s0
                            s0 = _mm_shuffle_ps(s0, s0, _MM_SHUFFLE(0,2,1,3));
                            _mm_storel_pi((__m64*)dstdata, s0);
                            _mm_storeh_pi((__m64*)((float*)(dstdata+dststep)), s0);
                    }
                    else
                    #endif
                    {
                        double t0, t1;
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
            }
            else
            {
                double d = _det2(Sd);
                if( d != 0. )
                {
                    result = true;
                    d = 1./d;
                    #if CC_SSE2
                    if(USE_SSE2)
                    {
                        __m128d s0 = _mm_loadu_pd((const double*)srcdata); //s0 = sf(0,0) sf(0,1)
                        __m128d s1 = _mm_loadu_pd ((const double*)(srcdata+srcstep));//s1 = sf(1,0) sf(1,1)
                        __m128d sm = _mm_unpacklo_pd(s0, _mm_load_sd((const double*)(srcdata+srcstep)+1)); //sm = sf(0,0) sf(1,1) - main diagonal
                        __m128d ss = _mm_shuffle_pd(s0, s1, _MM_SHUFFLE2(0,1)); //ss = sf(0,1) sf(1,0) - secondary diagonal
                        __m128d det = _mm_load1_pd((const double*)&d);
                        sm =  _mm_mul_pd(sm, det);

                        static const uchar CC_DECL_ALIGNED(16) inv[8] = {0,0,0,0,0,0,0,0x80};
                        __m128d pattern = _mm_load1_pd((double*)inv);
                        ss = _mm_mul_pd(ss, det);
                        ss = _mm_xor_pd(ss, pattern);//==-1*ss

                        s0 = _mm_shuffle_pd(sm, ss, _MM_SHUFFLE2(0,1));
                        s1 = _mm_shuffle_pd(ss, sm, _MM_SHUFFLE2(0,1));
                        _mm_storeu_pd((double*)dstdata, s0);
                        _mm_storeu_pd((double*)(dstdata+dststep), s1);
                    }
                    else
                    #endif
                    {
                        double t0, t1;
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
        }
        else if( n == 3 )
        {
            if( type == CC_32FC1 )
            {
                double d = _det3(Sf);

                if( d != 0. )
                {
                    double t[12];

                    result = true;
                    d = 1./d;
                    t[0] = (((double)Sf(1,1) * Sf(2,2) - (double)Sf(1,2) * Sf(2,1)) * d);
                    t[1] = (((double)Sf(0,2) * Sf(2,1) - (double)Sf(0,1) * Sf(2,2)) * d);
                    t[2] = (((double)Sf(0,1) * Sf(1,2) - (double)Sf(0,2) * Sf(1,1)) * d);

                    t[3] = (((double)Sf(1,2) * Sf(2,0) - (double)Sf(1,0) * Sf(2,2)) * d);
                    t[4] = (((double)Sf(0,0) * Sf(2,2) - (double)Sf(0,2) * Sf(2,0)) * d);
                    t[5] = (((double)Sf(0,2) * Sf(1,0) - (double)Sf(0,0) * Sf(1,2)) * d);

                    t[6] = (((double)Sf(1,0) * Sf(2,1) - (double)Sf(1,1) * Sf(2,0)) * d);
                    t[7] = (((double)Sf(0,1) * Sf(2,0) - (double)Sf(0,0) * Sf(2,1)) * d);
                    t[8] = (((double)Sf(0,0) * Sf(1,1) - (double)Sf(0,1) * Sf(1,0)) * d);

                    Df(0,0) = (float)t[0]; Df(0,1) = (float)t[1]; Df(0,2) = (float)t[2];
                    Df(1,0) = (float)t[3]; Df(1,1) = (float)t[4]; Df(1,2) = (float)t[5];
                    Df(2,0) = (float)t[6]; Df(2,1) = (float)t[7]; Df(2,2) = (float)t[8];
                }
            }
            else
            {
                double d = _det3(Sd);
                if( d != 0. )
                {
                    result = true;
                    d = 1./d;
                    double t[9];

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
            assert( n == 1 );

            if( type == CC_32FC1 )
            {
                double d = Sf(0,0);
                if( d != 0. )
                {
                    result = true;
                    Df(0,0) = (float)(1./d);
                }
            }
            else
            {
                double d = Sd(0,0);
                if( d != 0. )
                {
                    result = true;
                    Dd(0,0) = 1./d;
                }
            }
        }
        if( !result ) {
            cvZeroMat(dst);
        }
        return result;
    }
    int elem_size = CC_TYPE_SIZE(type);
    int src1_step = n*elem_size;
    uchar* src1 = (uchar*)memdup2d(src->tt.data, src->step, n, src1_step);
    cvSetIdentity(dst);

    if( method == CC_DECOMP_LU && type == CC_32F )
        result = hal_LU32f((float*)src1, src1_step, n, dst->tt.fl, dst->step, n) != 0;
    else if( method == CC_DECOMP_LU && type == CC_64F )
        result = hal_LU64f((double*)src1, src1_step, n, dst->tt.db, dst->step, n) != 0;
    else if( method == CC_DECOMP_CHOLESKY && type == CC_32F )
        result = hal_Cholesky32f((float*)src1, src1_step, n, dst->tt.fl, dst->step, n);
    else
        result = hal_Cholesky64f((double*)src1, src1_step, n, dst->tt.db, dst->step, n);

    if( !result )
        cvZeroMat(dst);
    return result;
}
