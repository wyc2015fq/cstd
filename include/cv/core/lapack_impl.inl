
#if ISDOUBLE
#define T double
#define TBLAS(NAME)  my_s ## NAME
#define EPSILON  DBL_EPSILON
#else
#define T float
#define TBLAS(NAME)  my_d ## NAME
#define EPSILON  FLT_EPSILON
#endif

#define _Tp T

static int
LUImpl(_Tp* A, size_t astep, int m, _Tp* b, size_t bstep, int n, _Tp eps)
{
    int i, j, k, p = 1;
    _Tp t;
    astep /= sizeof(A[0]);
    bstep /= sizeof(b[0]);

    for( i = 0; i < m; i++ )
    {
        k = i;

        for( j = i+1; j < m; j++ )
            if( fabs(A[j*astep + i]) > fabs(A[k*astep + i]) )
                k = j;

        if( fabs(A[k*astep + i]) < eps )
            return 0;

        if( k != i )
        {
            for( j = i; j < m; j++ )
                CC_SWAP(A[i*astep + j], A[k*astep + j], t);
            if( b )
                for( j = 0; j < n; j++ )
                    CC_SWAP(b[i*bstep + j], b[k*bstep + j], t);
            p = -p;
        }

        _Tp d = -1/A[i*astep + i];

        for( j = i+1; j < m; j++ )
        {
            _Tp alpha = A[j*astep + i]*d;

            for( k = i+1; k < m; k++ )
                A[j*astep + k] += alpha*A[i*astep + k];

            if( b )
                for( k = 0; k < n; k++ )
                    b[j*bstep + k] += alpha*b[i*bstep + k];
        }
    }

    if( b )
    {
        for( i = m-1; i >= 0; i-- )
            for( j = 0; j < n; j++ )
            {
                _Tp s = b[i*bstep + j];
                for( k = i+1; k < m; k++ )
                    s -= A[i*astep + k]*b[k*bstep + j];
                b[i*bstep + j] = s/A[i*astep + i];
            }
    }

    return p;
}

static bool
CholImpl(_Tp* A, size_t astep, int m, _Tp* b, size_t bstep, int n)
{
    _Tp* L = A;
    int i, j, k;
    double s;
    _Tp epsilon = EPSILON;
    astep /= sizeof(A[0]);
    bstep /= sizeof(b[0]);

    for( i = 0; i < m; i++ )
    {
        for( j = 0; j < i; j++ )
        {
            s = A[i*astep + j];
            for( k = 0; k < j; k++ )
                s -= L[i*astep + k]*L[j*astep + k];
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = A[i*astep + i];
        for( k = 0; k < j; k++ )
        {
            double t = L[i*astep + k];
            s -= t*t;
        }
        if( s < epsilon )
            return false;
        L[i*astep + i] = (_Tp)(1./sqrt(s));
    }

    if (!b)
    {
        for( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        return true;
    }

    // LLt x = b
    // 1: L y = b
    // 2. Lt x = y

    /*
     [ L00             ]  y0   b0
     [ L10 L11         ]  y1 = b1
     [ L20 L21 L22     ]  y2   b2
     [ L30 L31 L32 L33 ]  y3   b3

     [ L00 L10 L20 L30 ]  x0   y0
     [     L11 L21 L31 ]  x1 = y1
     [         L22 L32 ]  x2   y2
     [             L33 ]  x3   y3
     */

    for( i = 0; i < m; i++ )
    {
        for( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            for( k = 0; k < i; k++ )
                s -= L[i*astep + k]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }

    for( i = m-1; i >= 0; i-- )
    {
        for( j = 0; j < n; j++ )
        {
            s = b[i*bstep + j];
            for( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
    for( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];

    return true;
}

static bool
CholImpl1(_Tp* A, size_t astep, int m, _Tp* b, size_t bstep, int n)
{
    _Tp* L = A;
    int i, j, k;
    double s;
    _Tp epsilon = EPSILON;
    astep /= sizeof(A[0]);
    bstep /= sizeof(b[0]);

    for( i = 0; i < m; i++ ) {
        _Tp* Ai = A+i*astep;
        _Tp* Li = L+i*astep;
        for( j = 0; j < i; j++ ) {
            _Tp* Lj = L+j*astep;
            s = Ai[j];
            k = 0;
            for( ; k < j-3; k+=4 ) {
                s -=
                  Li[k+0]*Lj[k+0]+
                  Li[k+1]*Lj[k+1]+
                  Li[k+2]*Lj[k+2]+
                  Li[k+3]*Lj[k+3];
            }
            for( ; k < j; k++ ) {
                s -= Li[k]*Lj[k];
            }
            L[i*astep + j] = (_Tp)(s*L[j*astep + j]);
        }
        s = Ai[i];
        for( k = 0; k < j; k++ )
        {
            double t = Li[k];
            s -= t*t;
        }
        if( s < epsilon )
            return false;
        Li[i] = (_Tp)(1./sqrt(s));
    }

    if (!b)
    {
        for( i = 0; i < m; i++ )
             L[i*astep + i]=1/L[i*astep + i];
        return true;
    }

    // LLt x = b
    // 1: L y = b
    // 2. Lt x = y

    /*
     [ L00             ]  y0   b0
     [ L10 L11         ]  y1 = b1
     [ L20 L21 L22     ]  y2   b2
     [ L30 L31 L32 L33 ]  y3   b3

     [ L00 L10 L20 L30 ]  x0   y0
     [     L11 L21 L31 ]  x1 = y1
     [         L22 L32 ]  x2   y2
     [             L33 ]  x3   y3
     */

    int bstep2 = bstep*2;
    int bstep3 = bstep*3;
    for( i = 0; i < m; i++ ) {
        _Tp* Li = L + i*astep;
        j = 0;
#if 1
        for( ; j < n-3; j+=4 ) {
            double s0 = b[i*bstep + j+0];
            double s1 = b[i*bstep + j+1];
            double s2 = b[i*bstep + j+2];
            double s3 = b[i*bstep + j+3];
            k = 0;

            for( ; k < i-3; k+=4 ) {
                _Tp* b0 = b+j+k*bstep, l0 = Li[k], l1 = Li[k+1], l2 = Li[k+2], l3 = Li[k+3];
                s0 -= l0*b0[0]+l1*b0[0+bstep]+l2*b0[0+bstep2]+l3*b0[0+bstep3];
                s1 -= l0*b0[1]+l1*b0[1+bstep]+l2*b0[1+bstep2]+l3*b0[1+bstep3];
                s2 -= l0*b0[2]+l1*b0[2+bstep]+l2*b0[2+bstep2]+l3*b0[2+bstep3];
                s3 -= l0*b0[3]+l1*b0[3+bstep]+l2*b0[3+bstep2]+l3*b0[3+bstep3];
            }
#if 0
            for( ; k < i-1; k+=2 ) {
                _Tp* b0 = b+j+k*bstep, l0 = Li[k], l1 = Li[k+1];
                s0 -= l0*b0[0]+l1*b0[0+bstep];
                s1 -= l0*b0[1]+l1*b0[1+bstep];
                s2 -= l0*b0[2]+l1*b0[2+bstep];
                s3 -= l0*b0[3]+l1*b0[3+bstep];
            }
#endif
            for( ; k < i; k++ ) {
                _Tp* b0 = b+j+k*bstep, l = Li[k];
                s0 -= l*b0[0];
                s1 -= l*b0[1];
                s2 -= l*b0[2];
                s3 -= l*b0[3];
            }
            b[i*bstep + j+0] = (_Tp)(s0*Li[i]);
            b[i*bstep + j+1] = (_Tp)(s1*Li[i]);
            b[i*bstep + j+2] = (_Tp)(s2*Li[i]);
            b[i*bstep + j+3] = (_Tp)(s3*Li[i]);
        }
#endif
        for( ; j < n; j++ ) {
            s = b[i*bstep + j];
            for( k = 0; k < i; k++ ) {
                s -= Li[k]*b[k*bstep + j];
            }
            b[i*bstep + j] = (_Tp)(s*Li[i]);
        }
    }

    for( i = m-1; i >= 0; i-- ) {
        _Tp* Li = L + i;
        j = 0;
#if 1
        for( ; j < n-3; j+=4 )
        {
            double s0 = b[i*bstep + j+0];
            double s1 = b[i*bstep + j+1];
            double s2 = b[i*bstep + j+2];
            double s3 = b[i*bstep + j+3];
            k = i+1;
            for( ; k < m-3; k+=4 ) {
                _Tp* b0 = b+j+k*bstep, l0 = Li[k*astep], l1 = Li[(k+1)*astep], l2 = Li[(k+2)*astep], l3 = Li[(k+3)*astep];
                s0 -= l0*b0[0]+l1*b0[0+bstep]+l2*b0[0+bstep2]+l3*b0[0+bstep3];
                s1 -= l0*b0[1]+l1*b0[1+bstep]+l2*b0[1+bstep2]+l3*b0[1+bstep3];
                s2 -= l0*b0[2]+l1*b0[2+bstep]+l2*b0[2+bstep2]+l3*b0[2+bstep3];
                s3 -= l0*b0[3]+l1*b0[3+bstep]+l2*b0[3+bstep2]+l3*b0[3+bstep3];
            }
#if 0
            for( ; k < m-1; k+=2 ) {
                _Tp* b0 = b+j+k*bstep, l0 = Li[k*astep], l1 = Li[(k+1)*astep];
                s0 -= l0*b0[0]+l1*b0[0+bstep];
                s1 -= l0*b0[1]+l1*b0[1+bstep];
                s2 -= l0*b0[2]+l1*b0[2+bstep];
                s3 -= l0*b0[3]+l1*b0[3+bstep];
            }
#endif
            for( ; k < m; k++ ) {
                _Tp* b0 = b+j+k*bstep, l = L[k*astep + i];
                s0 -= l*b0[0];
                s1 -= l*b0[1];
                s2 -= l*b0[2];
                s3 -= l*b0[3];
            }
            b[i*bstep + j+0] = (_Tp)(s0*L[i*astep+i]);
            b[i*bstep + j+1] = (_Tp)(s1*L[i*astep+i]);
            b[i*bstep + j+2] = (_Tp)(s2*L[i*astep+i]);
            b[i*bstep + j+3] = (_Tp)(s3*L[i*astep+i]);
        }
#endif
        for( ; j < n; j++ )
        {
            s = b[i*bstep + j];
            for( k = m-1; k > i; k-- ) {
                s -= L[k*astep + i]*b[k*bstep + j];
            }
            b[i*bstep + j] = (_Tp)(s*L[i*astep+i]);
        }
    }
#if 0
    for( i = 0; i < m; i++ ) {
        _Tp* Li = L + i*astep;
        for( j = 0; j < n; j++ ) {
            s = b[i*bstep + j];
            for( k = 0; k < i; k++ ) {
                s -= Li[k]*b[k*bstep + j];
            }
            b[i*bstep + j] = (_Tp)(s*Li[i]);
        }
    }

    for( i = m-1; i >= 0; i-- ) {
        for( j = 0; j < n; j++ ) {
            s = b[i*bstep + j];
            for( k = m-1; k > i; k-- )
                s -= L[k*astep + i]*b[k*bstep + j];
            b[i*bstep + j] = (_Tp)(s*L[i*astep + i]);
        }
    }
#endif
    for( i = 0; i < m; i++ )
            L[i*astep + i]=1/L[i*astep + i];
    return true;
}

static int sign(_Tp x)
{
    if (x >= (_Tp)0)
        return 1;
    else
        return -1;
}

static int
QRImpl(_Tp* A, size_t astep, int m, int n, int k, _Tp* b, size_t bstep, _Tp* hFactors, _Tp eps)
{
    _Tp* buffer = NULL;
    int i, j;
    size_t buf_size = m ? m + n : hFactors != NULL;
    astep /= sizeof(_Tp);
    bstep /= sizeof(_Tp);
    buffer = MALLOC(_Tp, buf_size);
    _Tp* vl = buffer;
    if (hFactors == NULL)
        hFactors = vl + m;

    for (int l = 0; l < n; l++)
    {
        //generate vl
        int vlSize = m - l;
        _Tp vlNorm = (_Tp)0;
        for (i = 0; i < vlSize; i++)
        {
            vl[i] = A[(l + i)*astep + l];
            vlNorm += vl[i] * vl[i];
        }
        _Tp tmpV = vl[0];
        vl[0] = vl[0] + sign(vl[0])*sqrt(vlNorm);
        vlNorm = sqrt(vlNorm + vl[0] * vl[0] - tmpV*tmpV);
        for (i = 0; i < vlSize; i++)
        {
            vl[i] /= vlNorm;
        }
        //multiply A_l*vl
        for (j = l; j < n; j++)
        {
            _Tp v_lA = (_Tp)0;
            for (i = l; i < m; i++)
            {
                v_lA += vl[i - l] * A[i*astep + j];
            }

            for (i = l; i < m; i++)
            {
                A[i*astep + j] -= 2 * vl[i - l] * v_lA;
            }
        }

        //save vl and factors
        hFactors[l] = vl[0] * vl[0];
        for (i = 1; i < vlSize; i++)
        {
            A[(l + i)*astep + l] = vl[i] / vl[0];
        }
    }

    if (b)
    {
        //generate new rhs
        for (int l = 0; l < n; l++)
        {
            //unpack vl
            vl[0] = (_Tp)1;
            for (j = 1; j < m - l; j++)
            {
              vl[j] = A[(j + l)*astep + l];
            }

            //h_l*x
            for (j = 0; j < k; j++)
            {
                _Tp v_lB = (_Tp)0;
                for (i = l; i < m; i++)
                  v_lB += vl[i - l] * b[i*bstep + j];

                for (i = l; i < m; i++)
                    b[i*bstep + j] -= 2 * vl[i - l] * v_lB * hFactors[l];
            }
        }
        //do back substitution
        for (i = n - 1; i >= 0; i--)
        {
            for (j = n - 1; j > i; j--)
            {
                for (int p = 0; p < k; p++)
                    b[i*bstep + p] -= b[j*bstep + p] * A[i*astep + j];
            }
            if (fabs(A[i*astep + i]) < eps)
                return 0;
            for (int p = 0; p < k; p++)
                b[i*bstep + p] /= A[i*astep + i];
        }
    }

    FREE(buffer);
    return 1;
}

static int VBLAS_dot(const _Tp* a, const _Tp* b, int n, _Tp* result)
{
  int k = 0;
#if CC_SSE2
#if ISDOUBLE
  if( n < 4 )
    return 0;
  __m128d s0 = _mm_setzero_pd(), s1 = _mm_setzero_pd();
  for( ; k <= n - 4; k += 4 )
  {
    __m128d a0 = _mm_load_pd(a + k), a1 = _mm_load_pd(a + k + 2);
    __m128d b0 = _mm_load_pd(b + k), b1 = _mm_load_pd(b + k + 2);
    
    s0 = _mm_add_pd(s0, _mm_mul_pd(a0, b0));
    s1 = _mm_add_pd(s1, _mm_mul_pd(a1, b1));
  }
  s0 = _mm_add_pd(s0, s1);
  double sbuf[2];
  _mm_storeu_pd(sbuf, s0);
  *result = sbuf[0] + sbuf[1];
#else
  if( n < 8 )
    return 0;
  __m128 s0 = _mm_setzero_ps(), s1 = _mm_setzero_ps();
  for( ; k <= n - 8; k += 8 )
  {
    __m128 a0 = _mm_load_ps(a + k), a1 = _mm_load_ps(a + k + 4);
    __m128 b0 = _mm_load_ps(b + k), b1 = _mm_load_ps(b + k + 4);
    
    s0 = _mm_add_ps(s0, _mm_mul_ps(a0, b0));
    s1 = _mm_add_ps(s1, _mm_mul_ps(a1, b1));
  }
  s0 = _mm_add_ps(s0, s1);
  float sbuf[4];
  _mm_storeu_ps(sbuf, s0);
  *result = sbuf[0] + sbuf[1] + sbuf[2] + sbuf[3];
#endif
#endif
  return k;
}

static int VBLAS_givens(_Tp* a, _Tp* b, int n, _Tp c, _Tp s)
{
    int k = 0;
#if CC_SSE2
#if ISDOUBLE
    if( n < 8 )
        return 0;
    __m128d c2 = _mm_set1_pd(c), s2 = _mm_set1_pd(s);
    for( ; k <= n - 2; k += 2 )
    {
        __m128d a0 = _mm_load_pd(a + k);
        __m128d b0 = _mm_load_pd(b + k);
        __m128d t0 = _mm_add_pd(_mm_mul_pd(a0, c2), _mm_mul_pd(b0, s2));
        __m128d t1 = _mm_sub_pd(_mm_mul_pd(b0, c2), _mm_mul_pd(a0, s2));
        _mm_store_pd(a + k, t0);
        _mm_store_pd(b + k, t1);
    }
#else
    if( n < 4 )
        return 0;
    __m128 c4 = _mm_set1_ps(c), s4 = _mm_set1_ps(s);
    for( ; k <= n - 4; k += 4 )
    {
        __m128 a0 = _mm_load_ps(a + k);
        __m128 b0 = _mm_load_ps(b + k);
        __m128 t0 = _mm_add_ps(_mm_mul_ps(a0, c4), _mm_mul_ps(b0, s4));
        __m128 t1 = _mm_sub_ps(_mm_mul_ps(b0, c4), _mm_mul_ps(a0, s4));
        _mm_store_ps(a + k, t0);
        _mm_store_ps(b + k, t1);
    }
#endif
#endif
    return k;
}

static int VBLAS_givensx(_Tp* a, _Tp* b, int n, _Tp c, _Tp s,
                                              _Tp* anorm, _Tp* bnorm)
{
    int k = 0;
#if CC_SSE2
#if ISDOUBLE
    __m128d c2 = _mm_set1_pd(c), s2 = _mm_set1_pd(s);
    __m128d sa = _mm_setzero_pd(), sb = _mm_setzero_pd();
    for( ; k <= n - 2; k += 2 )
    {
        __m128d a0 = _mm_load_pd(a + k);
        __m128d b0 = _mm_load_pd(b + k);
        __m128d t0 = _mm_add_pd(_mm_mul_pd(a0, c2), _mm_mul_pd(b0, s2));
        __m128d t1 = _mm_sub_pd(_mm_mul_pd(b0, c2), _mm_mul_pd(a0, s2));
        _mm_store_pd(a + k, t0);
        _mm_store_pd(b + k, t1);
        sa = _mm_add_pd(sa, _mm_mul_pd(t0, t0));
        sb = _mm_add_pd(sb, _mm_mul_pd(t1, t1));
    }
    double abuf[2], bbuf[2];
    _mm_storeu_pd(abuf, sa);
    _mm_storeu_pd(bbuf, sb);
    *anorm = abuf[0] + abuf[1];
    *bnorm = bbuf[0] + bbuf[1];
#else
    if( n < 4 )
        return 0;
    __m128 c4 = _mm_set1_ps(c), s4 = _mm_set1_ps(s);
    __m128 sa = _mm_setzero_ps(), sb = _mm_setzero_ps();
    for( ; k <= n - 4; k += 4 )
    {
        __m128 a0 = _mm_load_ps(a + k);
        __m128 b0 = _mm_load_ps(b + k);
        __m128 t0 = _mm_add_ps(_mm_mul_ps(a0, c4), _mm_mul_ps(b0, s4));
        __m128 t1 = _mm_sub_ps(_mm_mul_ps(b0, c4), _mm_mul_ps(a0, s4));
        _mm_store_ps(a + k, t0);
        _mm_store_ps(b + k, t1);
        sa = _mm_add_ps(sa, _mm_mul_ps(t0, t0));
        sb = _mm_add_ps(sb, _mm_mul_ps(t1, t1));
    }
    float abuf[4], bbuf[4];
    _mm_storeu_ps(abuf, sa);
    _mm_storeu_ps(bbuf, sb);
    *anorm = abuf[0] + abuf[1] + abuf[2] + abuf[3];
    *bnorm = bbuf[0] + bbuf[1] + bbuf[2] + bbuf[3];
#endif
#endif
    return k;
}

CC_INLINE _Tp TBLAS(hypot)(_Tp a, _Tp b)
{
    a = fabs(a);
    b = fabs(b);
    if( a > b )
    {
        b /= a;
        return a*sqrt(1 + b*b);
    }
    if( b > 0 )
    {
        a /= b;
        return b*sqrt(1 + a*a);
    }
    return 0;
}

static int JacobiImpl_( _Tp* A, size_t astep, _Tp* W, _Tp* V, size_t vstep, int n, uchar* buf )
{
    int i, j, k, m;
    static const _Tp eps = EPSILON;

    astep /= sizeof(A[0]);
    if( V )
    {
        vstep /= sizeof(V[0]);
        for( i = 0; i < n; i++ )
        {
            for( j = 0; j < n; j++ )
                V[i*vstep + j] = (_Tp)0;
            V[i*vstep + i] = (_Tp)1;
        }
    }

    int iters, maxIters = n*n*30;

    int* indR = cAlignPtr(int, buf, sizeof(int));
    int* indC = indR + n;
    _Tp mv = (_Tp)0;

    for( k = 0; k < n; k++ )
    {
        W[k] = A[(astep + 1)*k];
        if( k < n - 1 )
        {
            for( m = k+1, mv = fabs(A[astep*k + m]), i = k+2; i < n; i++ )
            {
                _Tp val = fabs(A[astep*k+i]);
                if( mv < val )
                    mv = val, m = i;
            }
            indR[k] = m;
        }
        if( k > 0 )
        {
            for( m = 0, mv = fabs(A[k]), i = 1; i < k; i++ )
            {
                _Tp val = fabs(A[astep*i+k]);
                if( mv < val )
                    mv = val, m = i;
            }
            indC[k] = m;
        }
    }

    if( n > 1 ) for( iters = 0; iters < maxIters; iters++ )
    {
        // find index (k,l) of pivot p
        for( k = 0, mv = fabs(A[indR[0]]), i = 1; i < n-1; i++ )
        {
            _Tp val = fabs(A[astep*i + indR[i]]);
            if( mv < val )
                mv = val, k = i;
        }
        int l = indR[k];
        for( i = 1; i < n; i++ )
        {
            _Tp val = fabs(A[astep*indC[i] + i]);
            if( mv < val )
                mv = val, k = indC[i], l = i;
        }

        _Tp p = A[astep*k + l];
        if( fabs(p) <= eps )
            break;
        _Tp y = (_Tp)((W[l] - W[k])*0.5);
        _Tp t = fabs(y) + TBLAS(hypot)(p, y);
        _Tp s = TBLAS(hypot)(p, t);
        _Tp c = t/s;
        s = p/s; t = (p/t)*p;
        if( y < 0 )
            s = -s, t = -t;
        A[astep*k + l] = 0;

        W[k] -= t;
        W[l] += t;

        _Tp a0, b0;

#undef rotate
#define rotate(v0, v1) a0 = v0, b0 = v1, v0 = a0*c - b0*s, v1 = a0*s + b0*c

        // rotate rows and columns k and l
        for( i = 0; i < k; i++ )
            rotate(A[astep*i+k], A[astep*i+l]);
        for( i = k+1; i < l; i++ )
            rotate(A[astep*k+i], A[astep*i+l]);
        for( i = l+1; i < n; i++ )
            rotate(A[astep*k+i], A[astep*l+i]);

        // rotate eigenvectors
        if( V )
            for( i = 0; i < n; i++ )
                rotate(V[vstep*k+i], V[vstep*l+i]);

#undef rotate

        for( j = 0; j < 2; j++ )
        {
            int idx = j == 0 ? k : l;
            if( idx < n - 1 )
            {
                for( m = idx+1, mv = fabs(A[astep*idx + m]), i = idx+2; i < n; i++ )
                {
                    _Tp val = fabs(A[astep*idx+i]);
                    if( mv < val )
                        mv = val, m = i;
                }
                indR[idx] = m;
            }
            if( idx > 0 )
            {
                for( m = 0, mv = fabs(A[idx]), i = 1; i < idx; i++ )
                {
                    _Tp val = fabs(A[astep*i+idx]);
                    if( mv < val )
                        mv = val, m = i;
                }
                indC[idx] = m;
            }
        }
    }

    // sort eigenvalues & eigenvectors
    for( k = 0; k < n-1; k++ )
    {
        m = k;
        for( i = k+1; i < n; i++ )
        {
            if( W[m] < W[i] )
                m = i;
        }
        if( k != m )
        {
            T_SWAP(_Tp, W[m], W[k]);
            if( V )
                for( i = 0; i < n; i++ )
                    T_SWAP(_Tp, V[vstep*m + i], V[vstep*k + i]);
        }
    }

    return true;
}

static void
JacobiSVDImpl_(_Tp* At, size_t astep, _Tp* _W, _Tp* Vt, size_t vstep,
               int m, int n, int n1, double minval, _Tp eps)
{
    double* Wbuf = MALLOC(double, n);
    double* W = Wbuf;
    int i, j, k, iter, max_iter = MAX(m, 30);
    _Tp c, s;
    double sd;
    astep /= sizeof(At[0]);
    vstep /= sizeof(Vt[0]);

    for( i = 0; i < n; i++ )
    {
        for( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (double)t*t;
        }
        W[i] = sd;

        if( Vt )
        {
            for( k = 0; k < n; k++ )
                Vt[i*vstep + k] = 0;
            Vt[i*vstep + i] = 1;
        }
    }

    for( iter = 0; iter < max_iter; iter++ )
    {
        int changed = false;

        for( i = 0; i < n-1; i++ )
            for( j = i+1; j < n; j++ )
            {
                _Tp *Ai = At + i*astep, *Aj = At + j*astep;
                double a = W[i], p = 0, b = W[j];

                for( k = 0; k < m; k++ )
                    p += (double)Ai[k]*Aj[k];

                if( fabs(p) <= eps*sqrt((double)a*b) )
                    continue;

                p *= 2;
                double beta = a - b, gamma = hypot((double)p, beta);
                if( beta < 0 )
                {
                    double delta = (gamma - beta)*0.5;
                    s = (_Tp)sqrt(delta/gamma);
                    c = (_Tp)(p/(gamma*s*2));
                }
                else
                {
                    c = (_Tp)sqrt((gamma + beta)/(gamma*2));
                    s = (_Tp)(p/(gamma*c*2));
                }

                a = b = 0;
                for( k = 0; k < m; k++ )
                {
                    _Tp t0 = c*Ai[k] + s*Aj[k];
                    _Tp t1 = -s*Ai[k] + c*Aj[k];
                    Ai[k] = t0; Aj[k] = t1;

                    a += (double)t0*t0; b += (double)t1*t1;
                }
                W[i] = a; W[j] = b;

                changed = true;

                if( Vt )
                {
                    _Tp *Vi = Vt + i*vstep, *Vj = Vt + j*vstep;
                    k = VBLAS_givens(Vi, Vj, n, c, s);

                    for( ; k < n; k++ )
                    {
                        _Tp t0 = c*Vi[k] + s*Vj[k];
                        _Tp t1 = -s*Vi[k] + c*Vj[k];
                        Vi[k] = t0; Vj[k] = t1;
                    }
                }
            }
        if( !changed )
            break;
    }

    for( i = 0; i < n; i++ )
    {
        for( k = 0, sd = 0; k < m; k++ )
        {
            _Tp t = At[i*astep + k];
            sd += (double)t*t;
        }
        W[i] = sqrt(sd);
    }

    for( i = 0; i < n-1; i++ )
    {
        j = i;
        for( k = i+1; k < n; k++ )
        {
            if( W[j] < W[k] )
                j = k;
        }
        if( i != j )
        {
            T_SWAP(_Tp, W[i], W[j]);
            if( Vt )
            {
                for( k = 0; k < m; k++ )
                    T_SWAP(_Tp, At[i*astep + k], At[j*astep + k]);

                for( k = 0; k < n; k++ )
                    T_SWAP(_Tp, Vt[i*vstep + k], Vt[j*vstep + k]);
            }
        }
    }

    for( i = 0; i < n; i++ )
        _W[i] = (_Tp)W[i];

    if( !Vt )
        return;

    CRNG rng = (0x12345678);
    for( i = 0; i < n1; i++ )
    {
        sd = i < n ? W[i] : 0;

        for( int ii = 0; ii < 100 && sd <= minval; ii++ )
        {
            // if we got a zero singular value, then in order to get the corresponding left singular vector
            // we generate a random vector, project it to the previously computed left singular vectors,
            // subtract the projection and normalize the difference.
            const _Tp val0 = (_Tp)(1./m);
            for( k = 0; k < m; k++ )
            {
                _Tp val = (rng_int(&rng) & 256) != 0 ? val0 : -val0;
                At[i*astep + k] = val;
            }
            for( iter = 0; iter < 2; iter++ )
            {
                for( j = 0; j < i; j++ )
                {
                    sd = 0;
                    for( k = 0; k < m; k++ )
                        sd += At[i*astep + k]*At[j*astep + k];
                    _Tp asum = 0;
                    for( k = 0; k < m; k++ )
                    {
                        _Tp t = (_Tp)(At[i*astep + k] - sd*At[j*astep + k]);
                        At[i*astep + k] = t;
                        asum += fabs(t);
                    }
                    asum = asum > eps*100 ? 1/asum : 0;
                    for( k = 0; k < m; k++ )
                        At[i*astep + k] *= asum;
                }
            }
            sd = 0;
            for( k = 0; k < m; k++ )
            {
                _Tp t = At[i*astep + k];
                sd += (double)t*t;
            }
            sd = sqrt(sd);
        }

        s = (_Tp)(sd > minval ? 1/sd : 0.);
        for( k = 0; k < m; k++ )
            At[i*astep + k] *= s;
    }
}

static int
decodeSVDParameters(const _Tp* U, const _Tp* Vt, int m, int n, int n1)
{
    int halSVDFlag = 0;
    if(Vt == NULL)
        halSVDFlag = CC_HAL_SVD_NO_UV;
    else if(n1 <= 0 || n1 == n)
    {
        halSVDFlag = CC_HAL_SVD_SHORT_UV;
        if(U == NULL)
            halSVDFlag |= CC_HAL_SVD_MODIFY_A;
    }
    else if(n1 == m)
    {
        halSVDFlag = CC_HAL_SVD_FULL_UV;
        if(U == NULL)
            halSVDFlag |= CC_HAL_SVD_MODIFY_A;
    }
    return halSVDFlag;
}


/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
static void
MatrAXPY( int m, int n, const _Tp* x, int dx,
         const _Tp* a, int inca, _Tp* y, int dy )
{
    int i;
    typedef _Tp T2;
    typedef _Tp T3;
    for( i = 0; i < m; i++, x += dx, y += dy )
    {
        T2 s = a[i*inca];
        int j = 0;
        for(; j <= n - 4; j += 4 )
        {
            T3 t0 = (T3)(y[j]   + s*x[j]);
            T3 t1 = (T3)(y[j+1] + s*x[j+1]);
            y[j]   = t0;
            y[j+1] = t1;
            t0 = (T3)(y[j+2] + s*x[j+2]);
            t1 = (T3)(y[j+3] + s*x[j+3]);
            y[j+2] = t0;
            y[j+3] = t1;
        }
        for( ; j < n; j++ )
            y[j] = (T3)(y[j] + s*x[j]);
    }
}

static void
SVBkSbImpl_( int m, int n, const T* w, int incw,
       const T* u, int ldu, int uT,
       const T* v, int ldv, int vT,
       const T* b, int ldb, int nb,
       T* x, int ldx, T* buffer, T eps )
{
    double threshold = 0;
    int udelta0 = uT ? ldu : 1, udelta1 = uT ? 1 : ldu;
    int vdelta0 = vT ? ldv : 1, vdelta1 = vT ? 1 : ldv;
    int i, j, nm = MIN(m, n);

    if( !b )
        nb = m;

    for( i = 0; i < n; i++ )
        for( j = 0; j < nb; j++ )
            x[i*ldx + j] = 0;

    for( i = 0; i < nm; i++ )
        threshold += w[i*incw];
    threshold *= eps;

    // v * inv(w) * uT * b
    for( i = 0; i < nm; i++, u += udelta0, v += vdelta0 )
    {
        double wi = w[i*incw];
        if( (double)fabs(wi) <= threshold )
            continue;
        wi = 1/wi;

        if( nb == 1 )
        {
            double s = 0;
            if( b )
                for( j = 0; j < m; j++ )
                    s += u[j*udelta1]*b[j*ldb];
            else
                s = u[0];
            s *= wi;

            for( j = 0; j < n; j++ )
                x[j*ldx] = (T)(x[j*ldx] + s*v[j*vdelta1]);
        }
        else
        {
            if( b )
            {
                for( j = 0; j < nb; j++ )
                    buffer[j] = 0;
                MatrAXPY( m, nb, b, ldb, u, udelta1, buffer, 0 );
                for( j = 0; j < nb; j++ )
                    buffer[j] *= wi;
            }
            else
            {
                for( j = 0; j < nb; j++ )
                    buffer[j] = u[j*udelta1]*wi;
            }
            MatrAXPY( n, nb, buffer, 0, v, vdelta1, x, ldx );
        }
    }
}


#undef ISDOUBLE
#undef CCFUNC
#undef T
#undef _Tp
#undef EPSILON
#undef TBLAS
