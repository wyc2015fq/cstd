
/////////////////////////////////////////////////////////////////////////////////////////

#define icvGivens_64f(n, x, y, c, s) \
{                                      \
    int _i;                            \
    double* _x = (x);                  \
    double* _y = (y);                  \
                                       \
    for(_i = 0; _i < n; _i++)        \
    {                                  \
        double t0 = _x[_i];            \
        double t1 = _y[_i];            \
        _x[_i] = t0*c + t1*s;          \
        _y[_i] = -t0*s + t1*c;         \
    }                                  \
}


/* y[0:m,0:n] += diag(a[0:1,0:m]) * x[0:m,0:n] */
static  void
icvMatrAXPY_64f(int m, int n, const double* x, int dx,
                 const double* a, double* y, int dy)
{
    int i, j;

    for(i = 0; i < m; i++, x += dx, y += dy)
    {
        double s = a[i];

        for(j = 0; j <= n - 4; j += 4)
        {
            double t0 = y[j]   + s*x[j];
            double t1 = y[j+1] + s*x[j+1];
            y[j]   = t0;
            y[j+1] = t1;
            t0 = y[j+2] + s*x[j+2];
            t1 = y[j+3] + s*x[j+3];
            y[j+2] = t0;
            y[j+3] = t1;
        }

        for(; j < n; j++) y[j] += s*x[j];
    }
}


/* y[1:m,-1] = h*y[1:m,0:n]*x[0:1,0:n]'*x[-1]  (this is used for U&V reconstruction)
   y[1:m,0:n] += h*y[1:m,0:n]*x[0:1,0:n]'*x[0:1,0:n] */
static void
icvMatrAXPY3_64f(int m, int n, const double* x, int l, double* y, double h)
{
    int i, j;

    for(i = 1; i < m; i++)
    {
        double s = 0;

        y += l;

        for(j = 0; j <= n - 4; j += 4)
            s += x[j]*y[j] + x[j+1]*y[j+1] + x[j+2]*y[j+2] + x[j+3]*y[j+3];

        for(; j < n; j++)  s += x[j]*y[j];

        s *= h;
        y[-1] = s*x[-1];

        for(j = 0; j <= n - 4; j += 4)
        {
            double t0 = y[j]   + s*x[j];
            double t1 = y[j+1] + s*x[j+1];
            y[j]   = t0;
            y[j+1] = t1;
            t0 = y[j+2] + s*x[j+2];
            t1 = y[j+3] + s*x[j+3];
            y[j+2] = t0;
            y[j+3] = t1;
        }

        for(; j < n; j++) y[j] += s*x[j];
    }
}


#define icvGivens_32f(n, x, y, c, s) \
{                                      \
    int _i;                            \
    float* _x = (x);                   \
    float* _y = (y);                   \
                                       \
    for(_i = 0; _i < n; _i++)        \
    {                                  \
        double t0 = _x[_i];            \
        double t1 = _y[_i];            \
        _x[_i] = (float)(t0*c + t1*s); \
        _y[_i] = (float)(-t0*s + t1*c);\
    }                                  \
}

static  void
icvMatrAXPY_32f(int m, int n, const float* x, int dx,
                 const float* a, float* y, int dy)
{
    int i, j;

    for(i = 0; i < m; i++, x += dx, y += dy)
    {
        double s = a[i];

        for(j = 0; j <= n - 4; j += 4)
        {
            double t0 = y[j]   + s*x[j];
            double t1 = y[j+1] + s*x[j+1];
            y[j]   = (float)t0;
            y[j+1] = (float)t1;
            t0 = y[j+2] + s*x[j+2];
            t1 = y[j+3] + s*x[j+3];
            y[j+2] = (float)t0;
            y[j+3] = (float)t1;
        }

        for(; j < n; j++)
            y[j] = (float)(y[j] + s*x[j]);
    }
}


static void
icvMatrAXPY3_32f(int m, int n, const float* x, int l, float* y, double h)
{
    int i, j;

    for(i = 1; i < m; i++)
    {
        double s = 0;
        y += l;

        for(j = 0; j <= n - 4; j += 4)
            s += x[j]*y[j] + x[j+1]*y[j+1] + x[j+2]*y[j+2] + x[j+3]*y[j+3];

        for(; j < n; j++)  s += x[j]*y[j];

        s *= h;
        y[-1] = (float)(s*x[-1]);

        for(j = 0; j <= n - 4; j += 4)
        {
            double t0 = y[j]   + s*x[j];
            double t1 = y[j+1] + s*x[j+1];
            y[j]   = (float)t0;
            y[j+1] = (float)t1;
            t0 = y[j+2] + s*x[j+2];
            t1 = y[j+3] + s*x[j+3];
            y[j+2] = (float)t0;
            y[j+3] = (float)t1;
        }

        for(; j < n; j++) y[j] = (float)(y[j] + s*x[j]);
    }
}

/* accurate hypotenuse calculation */
static double
pythag(double a, double b)
{
    a = fabs(a);
    b = fabs(b);
    if(a > b)
    {
        b /= a;
        a *= sqrt(1. + b * b);
    }
    else if(b != 0)
    {
        a /= b;
        a = b * sqrt(1. + a * a);
    }

    return a;
}

/****************************************************************************************/
/****************************************************************************************/

#define MAX_ITERS  30

static void
icvSVD_64f(double* a, int lda, int m, int n,
            double* w,
            double* uT, int lduT, int nu,
            double* vT, int ldvT,
            double* buffer)
{
    double* e;
    double* temp;
    double *w1, *e1;
    double *hv;
    double ku0 = 0, kv0 = 0;
    double anorm = 0;
    double *a1, *u0 = uT, *v0 = vT;
    double scale, h;
    int i, j, k, l;
    int nm, m1, n1;
    int nv = n;
    int iters = 0;
    double* hv0 = (double*)cvStackAlloc((m+2)*sizeof(hv0[0])) + 1; 

    e = buffer;
    w1 = w;
    e1 = e + 1;
    nm = n;
    
    temp = buffer + nm;

    memset(w, 0, nm * sizeof(w[0]));
    memset(e, 0, nm * sizeof(e[0]));

    m1 = m;
    n1 = n;

    /* transform a to bi-diagonal form */
    for(;;)
    {
        int update_u;
        int update_v;
        
        if(m1 == 0)
            break;

        scale = h = 0;
        update_u = uT && m1 > m - nu;
        hv = update_u ? uT : hv0;

        for(j = 0, a1 = a; j < m1; j++, a1 += lda)
        {
            double t = a1[0];
            scale += fabs(hv[j] = t);
        }

        if(scale != 0)
        {
            double f = 1./scale, g, s = 0;

            for(j = 0; j < m1; j++)
            {
                double t = (hv[j] *= f);
                s += t * t;
            }

            g = sqrt(s);
            f = hv[0];
            if(f >= 0)
                g = -g;
            hv[0] = f - g;
            h = 1. / (f * g - s);

            memset(temp, 0, n1 * sizeof(temp[0]));

            /* calc temp[0:n-i] = a[i:m,i:n]'*hv[0:m-i] */
            icvMatrAXPY_64f(m1, n1 - 1, a + 1, lda, hv, temp + 1, 0);
            for(k = 1; k < n1; k++) temp[k] *= h;

            /* modify a: a[i:m,i:n] = a[i:m,i:n] + hv[0:m-i]*temp[0:n-i]' */
            icvMatrAXPY_64f(m1, n1 - 1, temp + 1, 0, hv, a + 1, lda);
            *w1 = g*scale;
        }
        w1++;

        /* store -2/(hv'*hv) */
        if(update_u)
        {
            if(m1 == m)
                ku0 = h;
            else
                hv[-1] = h;
        }

        a++;
        n1--;
        if(vT)
            vT += ldvT + 1;

        if(n1 == 0)
            break;

        scale = h = 0;
        update_v = vT && n1 > n - nv;

        hv = update_v ? vT : hv0;

        for(j = 0; j < n1; j++)
        {
            double t = a[j];
            scale += fabs(hv[j] = t);
        }

        if(scale != 0)
        {
            double f = 1./scale, g, s = 0;

            for(j = 0; j < n1; j++)
            {
                double t = (hv[j] *= f);
                s += t * t;
            }

            g = sqrt(s);
            f = hv[0];
            if(f >= 0)
                g = -g;
            hv[0] = f - g;
            h = 1. / (f * g - s);
            hv[-1] = 0.;

            /* update a[i:m:i+1:n] = a[i:m,i+1:n] + (a[i:m,i+1:n]*hv[0:m-i])*... */
            icvMatrAXPY3_64f(m1, n1, hv, lda, a, h);

            *e1 = g*scale;
        }
        e1++;

        /* store -2/(hv'*hv) */
        if(update_v)
        {
            if(n1 == n)
                kv0 = h;
            else
                hv[-1] = h;
        }

        a += lda;
        m1--;
        if(uT)
            uT += lduT + 1;
    }

    m1 -= m1 != 0;
    n1 -= n1 != 0;

    /* accumulate left transformations */
    if(uT)
    {
        m1 = m - m1;
        uT = u0 + m1 * lduT;
        for(i = m1; i < nu; i++, uT += lduT)
        {
            memset(uT + m1, 0, (m - m1) * sizeof(uT[0]));
            uT[i] = 1.;
        }

        for(i = m1 - 1; i >= 0; i--)
        {
            double s;
            int lh = nu - i;

            l = m - i;

            hv = u0 + (lduT + 1) * i;
            h = i == 0 ? ku0 : hv[-1];

            assert(h <= 0);

            if(h != 0)
            {
                uT = hv;
                icvMatrAXPY3_64f(lh, l-1, hv+1, lduT, uT+1, h);

                s = hv[0] * h;
                for(k = 0; k < l; k++) hv[k] *= s;
                hv[0] += 1;
            }
            else
            {
                for(j = 1; j < l; j++)
                    hv[j] = 0;
                for(j = 1; j < lh; j++)
                    hv[j * lduT] = 0;
                hv[0] = 1;
            }
        }
        uT = u0;
    }

    /* accumulate right transformations */
    if(vT)
    {
        n1 = n - n1;
        vT = v0 + n1 * ldvT;
        for(i = n1; i < nv; i++, vT += ldvT)
        {
            memset(vT + n1, 0, (n - n1) * sizeof(vT[0]));
            vT[i] = 1.;
        }

        for(i = n1 - 1; i >= 0; i--)
        {
            double s;
            int lh = nv - i;

            l = n - i;
            hv = v0 + (ldvT + 1) * i;
            h = i == 0 ? kv0 : hv[-1];

            assert(h <= 0);

            if(h != 0)
            {
                vT = hv;
                icvMatrAXPY3_64f(lh, l-1, hv+1, ldvT, vT+1, h);

                s = hv[0] * h;
                for(k = 0; k < l; k++) hv[k] *= s;
                hv[0] += 1;
            }
            else
            {
                for(j = 1; j < l; j++)
                    hv[j] = 0;
                for(j = 1; j < lh; j++)
                    hv[j * ldvT] = 0;
                hv[0] = 1;
            }
        }
        vT = v0;
    }

    for(i = 0; i < nm; i++)
    {
        double tnorm = fabs(w[i]);
        tnorm += fabs(e[i]);

        if(anorm < tnorm)
            anorm = tnorm;
    }

    anorm *= DBL_EPSILON;

    /* diagonalization of the bidiagonal form */
    for(k = nm - 1; k >= 0; k--)
    {
        double z = 0;
        iters = 0;

        for(;;)               /* do iterations */
        {
            double c, s, f, g, x, y;
            int flag = 0;

            /* test for splitting */
            for(l = k; l >= 0; l--)
            {
                if(fabs(e[l]) <= anorm)
                {
                    flag = 1;
                    break;
                }
                assert(l > 0);
                if(fabs(w[l - 1]) <= anorm)
                    break;
            }

            if(!flag)
            {
                c = 0;
                s = 1;

                for(i = l; i <= k; i++)
                {
                    f = s * e[i];

                    e[i] *= c;

                    if(anorm + fabs(f) == anorm)
                        break;

                    g = w[i];
                    h = pythag(f, g);
                    w[i] = h;
                    c = g / h;
                    s = -f / h;

                    if(uT)
                        icvGivens_64f(m, uT + lduT * (l - 1), uT + lduT * i, c, s);
                }
            }

            z = w[k];
            if(l == k || iters++ == MAX_ITERS)
                break;

            /* shift from bottom 2x2 minor */
            x = w[l];
            y = w[k - 1];
            g = e[k - 1];
            h = e[k];
            f = 0.5 * (((g + z) / h) * ((g - z) / y) + y / h - h / y);
            g = pythag(f, 1);
            if(f < 0)
                g = -g;
            f = x - (z / x) * z + (h / x) * (y / (f + g) - h);
            /* next QR transformation */
            c = s = 1;

            for(i = l + 1; i <= k; i++)
            {
                g = e[i];
                y = w[i];
                h = s * g;
                g *= c;
                z = pythag(f, h);
                e[i - 1] = z;
                c = f / z;
                s = h / z;
                f = x * c + g * s;
                g = -x * s + g * c;
                h = y * s;
                y *= c;

                if(vT)
                    icvGivens_64f(n, vT + ldvT * (i - 1), vT + ldvT * i, c, s);

                z = pythag(f, h);
                w[i - 1] = z;

                /* rotation can be arbitrary if z == 0 */
                if(z != 0)
                {
                    c = f / z;
                    s = h / z;
                }
                f = c * g + s * y;
                x = -s * g + c * y;

                if(uT)
                    icvGivens_64f(m, uT + lduT * (i - 1), uT + lduT * i, c, s);
            }

            e[l] = 0;
            e[k] = f;
            w[k] = x;
        }                       /* end of iteration loop */

        if(iters > MAX_ITERS)
            break;

        if(z < 0)
        {
            w[k] = -z;
            if(vT)
            {
                for(j = 0; j < n; j++)
                    vT[j + k * ldvT] = -vT[j + k * ldvT];
            }
        }
    }                           /* end of diagonalization loop */

    /* sort singular values and corresponding values */
    for(i = 0; i < nm; i++)
    {
        k = i;
        for(j = i + 1; j < nm; j++)
            if(w[k] < w[j])
                k = j;

        if(k != i)
        {
            double t;
            CC_SWAP(w[i], w[k], t);

            if(vT)
                for(j = 0; j < n; j++)
                    CC_SWAP(vT[j + ldvT*k], vT[j + ldvT*i], t);

            if(uT)
                for(j = 0; j < m; j++)
                    CC_SWAP(uT[j + lduT*k], uT[j + lduT*i], t);
        }
    }
}


static void
icvSVD_32f(float* a, int lda, int m, int n,
            float* w,
            float* uT, int lduT, int nu,
            float* vT, int ldvT,
            float* buffer)
{
    float* e;
    float* temp;
    float *w1, *e1;
    float *hv;
    double ku0 = 0, kv0 = 0;
    double anorm = 0;
    float *a1, *u0 = uT, *v0 = vT;
    double scale, h;
    int i, j, k, l;
    int nm, m1, n1;
    int nv = n;
    int iters = 0;
    float* hv0 = (float*)cvStackAlloc((m+2)*sizeof(hv0[0])) + 1;

    e = buffer;

    w1 = w;
    e1 = e + 1;
    nm = n;
    
    temp = buffer + nm;

    memset(w, 0, nm * sizeof(w[0]));
    memset(e, 0, nm * sizeof(e[0]));

    m1 = m;
    n1 = n;

    /* transform a to bi-diagonal form */
    for(;;)
    {
        int update_u;
        int update_v;
        
        if(m1 == 0)
            break;

        scale = h = 0;

        update_u = uT && m1 > m - nu;
        hv = update_u ? uT : hv0;

        for(j = 0, a1 = a; j < m1; j++, a1 += lda)
        {
            double t = a1[0];
            scale += fabs(hv[j] = (float)t);
        }

        if(scale != 0)
        {
            double f = 1./scale, g, s = 0;

            for(j = 0; j < m1; j++)
            {
                double t = (hv[j] = (float)(hv[j]*f));
                s += t * t;
            }

            g = sqrt(s);
            f = hv[0];
            if(f >= 0)
                g = -g;
            hv[0] = (float)(f - g);
            h = 1. / (f * g - s);

            memset(temp, 0, n1 * sizeof(temp[0]));

            /* calc temp[0:n-i] = a[i:m,i:n]'*hv[0:m-i] */
            icvMatrAXPY_32f(m1, n1 - 1, a + 1, lda, hv, temp + 1, 0);

            for(k = 1; k < n1; k++) temp[k] = (float)(temp[k]*h);

            /* modify a: a[i:m,i:n] = a[i:m,i:n] + hv[0:m-i]*temp[0:n-i]' */
            icvMatrAXPY_32f(m1, n1 - 1, temp + 1, 0, hv, a + 1, lda);
            *w1 = (float)(g*scale);
        }
        w1++;
        
        /* store -2/(hv'*hv) */
        if(update_u)
        {
            if(m1 == m)
                ku0 = h;
            else
                hv[-1] = (float)h;
        }

        a++;
        n1--;
        if(vT)
            vT += ldvT + 1;

        if(n1 == 0)
            break;

        scale = h = 0;
        update_v = vT && n1 > n - nv;
        hv = update_v ? vT : hv0;

        for(j = 0; j < n1; j++)
        {
            double t = a[j];
            scale += fabs(hv[j] = (float)t);
        }

        if(scale != 0)
        {
            double f = 1./scale, g, s = 0;

            for(j = 0; j < n1; j++)
            {
                double t = (hv[j] = (float)(hv[j]*f));
                s += t * t;
            }

            g = sqrt(s);
            f = hv[0];
            if(f >= 0)
                g = -g;
            hv[0] = (float)(f - g);
            h = 1. / (f * g - s);
            hv[-1] = 0.f;

            /* update a[i:m:i+1:n] = a[i:m,i+1:n] + (a[i:m,i+1:n]*hv[0:m-i])*... */
            icvMatrAXPY3_32f(m1, n1, hv, lda, a, h);

            *e1 = (float)(g*scale);
        }
        e1++;

        /* store -2/(hv'*hv) */
        if(update_v)
        {
            if(n1 == n)
                kv0 = h;
            else
                hv[-1] = (float)h;
        }

        a += lda;
        m1--;
        if(uT)
            uT += lduT + 1;
    }

    m1 -= m1 != 0;
    n1 -= n1 != 0;

    /* accumulate left transformations */
    if(uT)
    {
        m1 = m - m1;
        uT = u0 + m1 * lduT;
        for(i = m1; i < nu; i++, uT += lduT)
        {
            memset(uT + m1, 0, (m - m1) * sizeof(uT[0]));
            uT[i] = 1.;
        }

        for(i = m1 - 1; i >= 0; i--)
        {
            double s;
            int lh = nu - i;

            l = m - i;

            hv = u0 + (lduT + 1) * i;
            h = i == 0 ? ku0 : hv[-1];

            assert(h <= 0);

            if(h != 0)
            {
                uT = hv;
                icvMatrAXPY3_32f(lh, l-1, hv+1, lduT, uT+1, h);

                s = hv[0] * h;
                for(k = 0; k < l; k++) hv[k] = (float)(hv[k]*s);
                hv[0] += 1;
            }
            else
            {
                for(j = 1; j < l; j++)
                    hv[j] = 0;
                for(j = 1; j < lh; j++)
                    hv[j * lduT] = 0;
                hv[0] = 1;
            }
        }
        uT = u0;
    }

    /* accumulate right transformations */
    if(vT)
    {
        n1 = n - n1;
        vT = v0 + n1 * ldvT;
        for(i = n1; i < nv; i++, vT += ldvT)
        {
            memset(vT + n1, 0, (n - n1) * sizeof(vT[0]));
            vT[i] = 1.;
        }

        for(i = n1 - 1; i >= 0; i--)
        {
            double s;
            int lh = nv - i;

            l = n - i;
            hv = v0 + (ldvT + 1) * i;
            h = i == 0 ? kv0 : hv[-1];

            assert(h <= 0);

            if(h != 0)
            {
                vT = hv;
                icvMatrAXPY3_32f(lh, l-1, hv+1, ldvT, vT+1, h);

                s = hv[0] * h;
                for(k = 0; k < l; k++) hv[k] = (float)(hv[k]*s);
                hv[0] += 1;
            }
            else
            {
                for(j = 1; j < l; j++)
                    hv[j] = 0;
                for(j = 1; j < lh; j++)
                    hv[j * ldvT] = 0;
                hv[0] = 1;
            }
        }
        vT = v0;
    }

    for(i = 0; i < nm; i++)
    {
        double tnorm = fabs(w[i]);
        tnorm += fabs(e[i]);

        if(anorm < tnorm)
            anorm = tnorm;
    }

    anorm *= FLT_EPSILON;

    /* diagonalization of the bidiagonal form */
    for(k = nm - 1; k >= 0; k--)
    {
        double z = 0;
        iters = 0;

        for(;;)               /* do iterations */
        {
            double c, s, f, g, x, y;
            int flag = 0;

            /* test for splitting */
            for(l = k; l >= 0; l--)
            {
                if(fabs(e[l]) <= anorm)
                {
                    flag = 1;
                    break;
                }
                assert(l > 0);
                if(fabs(w[l - 1]) <= anorm)
                    break;
            }

            if(!flag)
            {
                c = 0;
                s = 1;

                for(i = l; i <= k; i++)
                {
                    f = s * e[i];
                    e[i] = (float)(e[i]*c);

                    if(anorm + fabs(f) == anorm)
                        break;

                    g = w[i];
                    h = pythag(f, g);
                    w[i] = (float)h;
                    c = g / h;
                    s = -f / h;

                    if(uT)
                        icvGivens_32f(m, uT + lduT * (l - 1), uT + lduT * i, c, s);
                }
            }

            z = w[k];
            if(l == k || iters++ == MAX_ITERS)
                break;

            /* shift from bottom 2x2 minor */
            x = w[l];
            y = w[k - 1];
            g = e[k - 1];
            h = e[k];
            f = 0.5 * (((g + z) / h) * ((g - z) / y) + y / h - h / y);
            g = pythag(f, 1);
            if(f < 0)
                g = -g;
            f = x - (z / x) * z + (h / x) * (y / (f + g) - h);
            /* next QR transformation */
            c = s = 1;

            for(i = l + 1; i <= k; i++)
            {
                g = e[i];
                y = w[i];
                h = s * g;
                g *= c;
                z = pythag(f, h);
                e[i - 1] = (float)z;
                c = f / z;
                s = h / z;
                f = x * c + g * s;
                g = -x * s + g * c;
                h = y * s;
                y *= c;

                if(vT)
                    icvGivens_32f(n, vT + ldvT * (i - 1), vT + ldvT * i, c, s);

                z = pythag(f, h);
                w[i - 1] = (float)z;

                /* rotation can be arbitrary if z == 0 */
                if(z != 0)
                {
                    c = f / z;
                    s = h / z;
                }
                f = c * g + s * y;
                x = -s * g + c * y;

                if(uT)
                    icvGivens_32f(m, uT + lduT * (i - 1), uT + lduT * i, c, s);
            }

            e[l] = 0;
            e[k] = (float)f;
            w[k] = (float)x;
        }                       /* end of iteration loop */

        if(iters > MAX_ITERS)
            break;

        if(z < 0)
        {
            w[k] = (float)(-z);
            if(vT)
            {
                for(j = 0; j < n; j++)
                    vT[j + k * ldvT] = -vT[j + k * ldvT];
            }
        }
    }                           /* end of diagonalization loop */

    /* sort singular values and corresponding vectors */
    for(i = 0; i < nm; i++)
    {
        k = i;
        for(j = i + 1; j < nm; j++)
            if(w[k] < w[j])
                k = j;

        if(k != i)
        {
            float t;
            CC_SWAP(w[i], w[k], t);

            if(vT)
                for(j = 0; j < n; j++)
                    CC_SWAP(vT[j + ldvT*k], vT[j + ldvT*i], t);

            if(uT)
                for(j = 0; j < m; j++)
                    CC_SWAP(uT[j + lduT*k], uT[j + lduT*i], t);
        }
    }
}


static void
icvSVBkSb_64f(int m, int n, const double* w,
               const double* uT, int lduT,
               const double* vT, int ldvT,
               const double* b, int ldb, int nb,
               double* x, int ldx, double* buffer)
{
    double threshold = 0;
    int i, j, nm = MIN(m, n);

    if(!b)
        nb = m;

    for(i = 0; i < n; i++)
        memset(x + i*ldx, 0, nb*sizeof(x[0]));

    for(i = 0; i < nm; i++)
        threshold += w[i];
    threshold *= 2*DBL_EPSILON;

    /* vT * inv(w) * uT * b */
    for(i = 0; i < nm; i++, uT += lduT, vT += ldvT)
    {
        double wi = w[i];

        if(wi > threshold)
        {
            wi = 1./wi;

            if(nb == 1)
            {
                double s = 0;
                if(b)
                {
                    if(ldb == 1)
                    {
                        for(j = 0; j <= m - 4; j += 4)
                            s += uT[j]*b[j] + uT[j+1]*b[j+1] + uT[j+2]*b[j+2] + uT[j+3]*b[j+3];
                        for(; j < m; j++)
                            s += uT[j]*b[j];
                    }
                    else
                    {
                        for(j = 0; j < m; j++)
                            s += uT[j]*b[j*ldb];
                    }
                }
                else
                    s = uT[0];
                s *= wi;
                if(ldx == 1)
                {
                    for(j = 0; j <= n - 4; j += 4)
                    {
                        double t0 = x[j] + s*vT[j];
                        double t1 = x[j+1] + s*vT[j+1];
                        x[j] = t0;
                        x[j+1] = t1;
                        t0 = x[j+2] + s*vT[j+2];
                        t1 = x[j+3] + s*vT[j+3];
                        x[j+2] = t0;
                        x[j+3] = t1;
                    }

                    for(; j < n; j++)
                        x[j] += s*vT[j];
                }
                else
                {
                    for(j = 0; j < n; j++)
                        x[j*ldx] += s*vT[j];
                }
            }
            else
            {
                if(b)
                {
                    memset(buffer, 0, nb*sizeof(buffer[0]));
                    icvMatrAXPY_64f(m, nb, b, ldb, uT, buffer, 0);
                    for(j = 0; j < nb; j++)
                        buffer[j] *= wi;
                }
                else
                {
                    for(j = 0; j < nb; j++)
                        buffer[j] = uT[j]*wi;
                }
                icvMatrAXPY_64f(n, nb, buffer, 0, vT, x, ldx);
            }
        }
    }
}


static void
icvSVBkSb_32f(int m, int n, const float* w,
               const float* uT, int lduT,
               const float* vT, int ldvT,
               const float* b, int ldb, int nb,
               float* x, int ldx, float* buffer)
{
    float threshold = 0.f;
    int i, j, nm = MIN(m, n);

    if(!b)
        nb = m;

    for(i = 0; i < n; i++)
        memset(x + i*ldx, 0, nb*sizeof(x[0]));

    for(i = 0; i < nm; i++)
        threshold += w[i];
    threshold *= 2*FLT_EPSILON;

    /* vT * inv(w) * uT * b */
    for(i = 0; i < nm; i++, uT += lduT, vT += ldvT)
    {
        double wi = w[i];
        
        if(wi > threshold)
        {
            wi = 1./wi;

            if(nb == 1)
            {
                double s = 0;
                if(b)
                {
                    if(ldb == 1)
                    {
                        for(j = 0; j <= m - 4; j += 4)
                            s += uT[j]*b[j] + uT[j+1]*b[j+1] + uT[j+2]*b[j+2] + uT[j+3]*b[j+3];
                        for(; j < m; j++)
                            s += uT[j]*b[j];
                    }
                    else
                    {
                        for(j = 0; j < m; j++)
                            s += uT[j]*b[j*ldb];
                    }
                }
                else
                    s = uT[0];
                s *= wi;

                if(ldx == 1)
                {
                    for(j = 0; j <= n - 4; j += 4)
                    {
                        double t0 = x[j] + s*vT[j];
                        double t1 = x[j+1] + s*vT[j+1];
                        x[j] = (float)t0;
                        x[j+1] = (float)t1;
                        t0 = x[j+2] + s*vT[j+2];
                        t1 = x[j+3] + s*vT[j+3];
                        x[j+2] = (float)t0;
                        x[j+3] = (float)t1;
                    }

                    for(; j < n; j++)
                        x[j] = (float)(x[j] + s*vT[j]);
                }
                else
                {
                    for(j = 0; j < n; j++)
                        x[j*ldx] = (float)(x[j*ldx] + s*vT[j]);
                }
            }
            else
            {
                if(b)
                {
                    memset(buffer, 0, nb*sizeof(buffer[0]));
                    icvMatrAXPY_32f(m, nb, b, ldb, uT, buffer, 0);
                    for(j = 0; j < nb; j++)
                        buffer[j] = (float)(buffer[j]*wi);
                }
                else
                {
                    for(j = 0; j < nb; j++)
                        buffer[j] = (float)(uT[j]*wi);
                }
                icvMatrAXPY_32f(n, nb, buffer, 0, vT, x, ldx);
            }
        }
    }
}

enum SVDFlags {
    /** allow the algorithm to modify the decomposed matrix; it can save space and speed up
        processing. currently ignored. */
    CC_SVD_MODIFY_A = 1,
    /** indicates that only a vector of singular values `w` is to be processed, while u and vt
        will be set to empty matrices */
    CC_SVD_NO_UV    = 2,
    /** when the matrix is not square, by default the algorithm produces u and vt matrices of
        sufficiently large size for the further A reconstruction; if, however, FULL_UV flag is
        specified, u and vt will be full-size square orthogonal matrices.*/
    CC_SVD_FULL_UV  = 4,
    CC_SVD_U_T      = 2,
    CC_SVD_V_T      = 4,
};


CC_IMPL  void
cvSVD(img_t* a, img_t* w, img_t* uarr, img_t* varr, int flags)
{
    uchar* buffer = 0;
    int local_alloc = 0;

    CC_FUNCNAME("cvSVD");

    __BEGIN__;

    img_t astub[1] = {0};
    img_t ustub[1] = {0};
    img_t vstub[1] = {0};
    img_t* u = uarr;
    img_t* v = varr;
    uchar* tw = 0;
    TypeId type;
    int a_buf_offset = 0, u_buf_offset = 0, buf_size, pix_size;
    int temp_u = 0, /* temporary storage for U is needed */
        t_svd; /* special case: a->rows < a->cols */
    int m, n;
    int w_rows, w_cols;
    int u_rows = 0, u_cols = 0;
    int w_is_mat = 0;
    int cn = CC_MAT_CN(a);

    ASSERT(1==cn);

    if(!CC_ARE_TYPES_EQ(a, w))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(a->rows >= a->cols)
    {
        m = a->rows;
        n = a->cols;
        w_rows = w->rows;
        w_cols = w->cols;
        t_svd = 0;
    }
    else
    {
        img_t* t;
        CC_SWAP(u, v, t);

        flags = (flags & CC_SVD_U_T ? CC_SVD_V_T : 0)|
                (flags & CC_SVD_V_T ? CC_SVD_U_T : 0);
        m = a->cols;
        n = a->rows;
        w_rows = w->cols;
        w_cols = w->rows;
        t_svd = 1;
    }

    w_is_mat = w_cols > 1 && w_rows > 1;

    if(!w_is_mat && CC_IS_MAT_CONT(w) && w_cols + w_rows - 1 == n)
        tw = w->tt.data;

    if(u)
    {
        if(!(flags & CC_SVD_U_T))
        {
            u_rows = u->rows;
            u_cols = u->cols;
        }
        else
        {
            u_rows = u->cols;
            u_cols = u->rows;
        }

        if(!CC_ARE_TYPES_EQ(a, u))
            CC_ERROR(CC_StsUnmatchedFormats, "");

        if(u_rows != m || (u_cols != m && u_cols != n))
            CC_ERROR(CC_StsUnmatchedSizes, !t_svd ? "U matrix has unappropriate size" :
                                                     "V matrix has unappropriate size");
            
        temp_u = (u_rows != u_cols && !(flags & CC_SVD_U_T)) || u->tt.data==a->tt.data;

        if(w_is_mat && u_cols != w_rows)
            CC_ERROR(CC_StsUnmatchedSizes, !t_svd ? "U and W have incompatible sizes" :
                                                     "V and W have incompatible sizes");
    }
    else
    {
        u = ustub;
        u->tt.data = 0;
        u->step = 0;
    }

    if(v)
    {
        int v_rows, v_cols;

        if(!(flags & CC_SVD_V_T))
        {
            v_rows = v->rows;
            v_cols = v->cols;
        }
        else
        {
            v_rows = v->cols;
            v_cols = v->rows;
        }

        if(!CC_ARE_TYPES_EQ(a, v))
            CC_ERROR(CC_StsUnmatchedFormats, "");

        if(v_rows != n || v_cols != n)
            CC_ERROR(CC_StsUnmatchedSizes, t_svd ? "U matrix has unappropriate size" :
                                                    "V matrix has unappropriate size");

        if(w_is_mat && w_cols != v_cols)
            CC_ERROR(CC_StsUnmatchedSizes, t_svd ? "U and W have incompatible sizes" :
                                                    "V and W have incompatible sizes");
    }
    else
    {
        v = vstub;
        v->tt.data = 0;
        v->step = 0;
    }

    type = CC_MAT_DEPTH(a);
    pix_size = CC_TYPE_SIZE(type);
    buf_size = n*2 + m;

    if(!(flags & CC_SVD_MODIFY_A))
    {
        a_buf_offset = buf_size;
        buf_size += a->rows*a->cols;
    }

    if(temp_u)
    {
        u_buf_offset = buf_size;
        buf_size += u->rows*u->cols;
    }

    buf_size *= pix_size;

    if(buf_size <= CC_MAX_LOCAL_SIZE)
    {
        buffer = (uchar*)cvStackAlloc(buf_size);
        local_alloc = 1;
    }
    else
    {
        CC_CALL(buffer = (uchar*)cAlloc(buf_size));
    }
    
    if(!(flags & CC_SVD_MODIFY_A))
    {
        cvInitMatHeader(astub, m, n, type, cn, buffer + a_buf_offset*pix_size, CC_AUTO_STEP);
        if(!t_svd)
            cvCopy(a, astub);
        else
            cvT(a, astub);
        a = astub;
    }

    if(temp_u)
    {
        cvInitMatHeader(ustub, u_cols, u_rows, type, cn, buffer + u_buf_offset*pix_size, CC_AUTO_STEP);
        u = ustub;
    }

    if(!tw)
        tw = buffer + (n + m)*pix_size;

    if(type == CC_32F && cn==1)
    {
        icvSVD_32f(a->tt.fl, a->step/sizeof(float), a->rows, a->cols,
                   (float*)tw, u->tt.fl, u->step/sizeof(float), u_cols,
                   v->tt.fl, v->step/sizeof(float), (float*)buffer);
    }
    else if(type == CC_64F && 1==cn)
    {
        icvSVD_64f(a->tt.db, a->step/sizeof(double), a->rows, a->cols,
                    (double*)tw, u->tt.db, u->step/sizeof(double), u_cols,
                    v->tt.db, v->step/sizeof(double), (double*)buffer);
    }
    else
    {
        CC_ERROR(CC_StsUnsupportedFormat, "");
    }

    if(tw != w->tt.data)
    {
        int i, shift = w->cols != 1;
        cvZeroMat(w);
        if(type == CC_32F && cn==1)
            for(i = 0; i < n; i++)
                ((float*)(w->tt.data + i*w->step))[i*shift] = ((float*)tw)[i];
        else
            for(i = 0; i < n; i++)
                ((double*)(w->tt.data + i*w->step))[i*shift] = ((double*)tw)[i];
    }

    if(uarr)
    {
        if(!(flags & CC_SVD_U_T))
            cvT(u, uarr);
        else if(temp_u)
            cvCopy(u, uarr);
        /*CC_CHECK_NANS(uarr);*/
    }

    if(varr)
    {
        if(!(flags & CC_SVD_V_T))
            cvT(v, varr);
        /*CC_CHECK_NANS(varr);*/
    }

    CC_CHECK_NANS(w);

    __END__;

    if(buffer && !local_alloc)
        cFree(&buffer);
}


CC_IMPL void
cvSVBkSb(const img_t* warr, const img_t* uarr,
          const img_t* varr, const img_t* barr,
          img_t* xarr, int flags)
{
    uchar* buffer = 0;
    int local_alloc = 0;

    CC_FUNCNAME("cvSVBkSb");

    __BEGIN__;

    img_t *w = (img_t*)warr;
    img_t *x = (img_t*)xarr;
    img_t bstub, *b = (img_t*)barr;
    img_t ustub, *u = (img_t*)uarr;
    img_t vstub, *v = (img_t*)varr;
    uchar* tw = 0;
    TypeId type;
    int temp_u = 0, temp_v = 0;
    int u_buf_offset = 0, v_buf_offset = 0, w_buf_offset = 0, t_buf_offset = 0;
    int buf_size = 0, pix_size;
    int m, n, nm;
    int u_rows, u_cols;
    int v_rows, v_cols;
    int cn = CC_MAT_CN(warr);
    ASSERT(1==cn);

    if(!CC_ARE_TYPES_EQ(w, u) || !CC_ARE_TYPES_EQ(w, v) || !CC_ARE_TYPES_EQ(w, x))
        CC_ERROR(CC_StsUnmatchedFormats, "All matrices must have the same type");

    type = CC_MAT_DEPTH(w);
    pix_size = CC_TYPE_SIZE(type);

    if(!(flags & CC_SVD_U_T))
    {
        temp_u = 1;
        u_buf_offset = buf_size;
        buf_size += u->cols*u->rows*pix_size;
        u_rows = u->rows;
        u_cols = u->cols;
    }
    else
    {
        u_rows = u->cols;
        u_cols = u->rows;
    }

    if(!(flags & CC_SVD_V_T))
    {
        temp_v = 1;
        v_buf_offset = buf_size;
        buf_size += v->cols*v->rows*pix_size;
        v_rows = v->rows;
        v_cols = v->cols;
    }
    else
    {
        v_rows = v->cols;
        v_cols = v->rows;
    }

    m = u_rows;
    n = v_rows;
    nm = MIN(n,m);

    if((u_rows != u_cols && v_rows != v_cols) || x->rows != v_rows)
        CC_ERROR(CC_StsBadSize, "V or U matrix must be square");

    if((w->rows == 1 || w->cols == 1) && w->rows + w->cols - 1 == nm)
    {
        if(CC_IS_MAT_CONT(w))
            tw = w->tt.data;
        else
        {
            w_buf_offset = buf_size;
            buf_size += nm*pix_size;
        }
    }
    else
    {
        if(w->cols != v_cols || w->rows != u_cols)
            CC_ERROR(CC_StsBadSize, "W must be 1d array of MIN(m,n) elements or "
                                    "matrix which size matches to U and V");
        w_buf_offset = buf_size;
        buf_size += nm*pix_size;
    }

    if(b)
    {
        if(!CC_ARE_TYPES_EQ(w, b))
            CC_ERROR(CC_StsUnmatchedFormats, "All matrices must have the same type");
        if(b->cols != x->cols || b->rows != m)
            CC_ERROR(CC_StsUnmatchedSizes, "b matrix must have (m x x->cols) size");
    }
    else
    {
        b = &bstub;
        memset(b, 0, sizeof(*b));
    }

    t_buf_offset = buf_size;
    buf_size += (MAX(m,n) + b->cols)*pix_size;

    if(buf_size <= CC_MAX_LOCAL_SIZE)
    {
        buffer = (uchar*)cvStackAlloc(buf_size);
        local_alloc = 1;
    }
    else
        CC_CALL(buffer = (uchar*)cAlloc(buf_size));

    if(temp_u)
    {
        cvInitMatHeader(&ustub, u_cols, u_rows, type, cn, buffer + u_buf_offset, CC_AUTO_STEP);
        cvT(u, &ustub);
        u = &ustub;
    }

    if(temp_v)
    {
        cvInitMatHeader(&vstub, v_cols, v_rows, type, cn, buffer + v_buf_offset, CC_AUTO_STEP);
        cvT(v, &vstub);
        v = &vstub;
    }

    if(!tw)
    {
        int i, shift = w->cols > 1 ? pix_size : 0;
        tw = buffer + w_buf_offset;
        for(i = 0; i < nm; i++)
            memcpy(tw + i*pix_size, w->tt.data + i*(w->step + shift), pix_size);
    }

    if(type == CC_32F && cn==1)
    {
        icvSVBkSb_32f(m, n, (float*)tw, u->tt.fl, u->step/sizeof(float),
                       v->tt.fl, v->step/sizeof(float),
                       b->tt.fl, b->step/sizeof(float), b->cols,
                       x->tt.fl, x->step/sizeof(float),
                       (float*)(buffer + t_buf_offset));
    }
    else if(type == CC_64F && 1==cn)
    {
        icvSVBkSb_64f(m, n, (double*)tw, u->tt.db, u->step/sizeof(double),
                       v->tt.db, v->step/sizeof(double),
                       b->tt.db, b->step/sizeof(double), b->cols,
                       x->tt.db, x->step/sizeof(double),
                       (double*)(buffer + t_buf_offset));
    }
    else
    {
        CC_ERROR(CC_StsUnsupportedFormat, "");
    }

    __END__;

    if(buffer && !local_alloc)
        cFree(&buffer);
}
#undef MAX_ITERS


typedef struct CSVD {
  img_t u[1];
  img_t w[1];
  img_t vt[1];
} CSVD;

static void SVD_free(CSVD* s) {
  imfree(s->u);
  imfree(s->w);
  imfree(s->vt);
}
static void SVD_init(CSVD* s, img_t* src, int flags CC_DEFAULT(0)) {
  int m = src->h, n = src->w;
  cvSetMat(s->u, m, m, src->tid, 1, NULL, CC_AUTO_STEP);
  cvSetMat(s->vt, n, n, src->tid, 1, NULL, CC_AUTO_STEP);
  cvSetMat(s->w, m, n, src->tid, 1, NULL, CC_AUTO_STEP);
  cvSVD(src, s->w, s->u, s->vt, flags);
  cvT(s->vt, s->vt);
  return ;
}