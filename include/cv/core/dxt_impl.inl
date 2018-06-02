
#if ISDOUBLE
#define T double
#define CCOMPLEX CComplex64f
#else
#define T float
#define CCOMPLEX CComplex32f
#endif

// mixed-radix complex discrete Fourier transform: double-precision version
static void CCFUNC(DFT)(const OcvDftOptions & c, const CCOMPLEX* src, CCOMPLEX* dst)
{
    static const T sin_120 = (T)0.86602540378443864676372317075294;
    static const T fft5_2 = (T)0.559016994374947424102293417182819;
    static const T fft5_3 = (T)-0.951056516295153572116439333379382;
    static const T fft5_4 = (T)-1.538841768587626701285145288018455;
    static const T fft5_5 = (T)0.363271264002680442947733378740309;

    const CCOMPLEX* wave = (CCOMPLEX*)c.wave;
    const int * itab = c.itab;

    int n = c.n;
    int f_idx, nx;
    int inv = c.isInverse;
    int dw0 = c.tab_size, dw;
    int i, j, k;
    CCOMPLEX t;
    T scale = (T)c.scale;

    if( c.useIpp )
    {
#ifdef USE_IPP_DFT
        if( !inv )
        {
            if (ippsDFTFwd_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CC_IMPL_ADD(CC_IMPL_IPP);
                return;
            }
        }
        else
        {
            if (ippsDFTInv_CToC( src, dst, c.ipp_spec, c.ipp_work ) >= 0)
            {
                CC_IMPL_ADD(CC_IMPL_IPP);
                return;
            }
        }
        setIppErrorStatus();
#endif
    }

    int tab_step = c.tab_size == n ? 1 : c.tab_size == n*2 ? 2 : c.tab_size/n;

    // 0. shuffle data
    if( dst != src )
    {
        assert( !c.noPermute );
        if( !inv )
        {
            for( i = 0; i <= n - 2; i += 2, itab += 2*tab_step )
            {
                int k0 = itab[0], k1 = itab[tab_step];
                assert( (unsigned)k0 < (unsigned)n && (unsigned)k1 < (unsigned)n );
                dst[i] = src[k0]; dst[i+1] = src[k1];
            }

            if( i < n )
                dst[n-1] = src[n-1];
        }
        else
        {
            for( i = 0; i <= n - 2; i += 2, itab += 2*tab_step )
            {
                int k0 = itab[0], k1 = itab[tab_step];
                assert( (unsigned)k0 < (unsigned)n && (unsigned)k1 < (unsigned)n );
                t.re = src[k0].re; t.im = -src[k0].im;
                dst[i] = t;
                t.re = src[k1].re; t.im = -src[k1].im;
                dst[i+1] = t;
            }

            if( i < n )
            {
                t.re = src[n-1].re; t.im = -src[n-1].im;
                dst[i] = t;
            }
        }
    }
    else
    {
        if( !c.noPermute )
        {
            CC_Assert( c.factors[0] == c.factors[c.nf-1] );
            if( c.nf == 1 )
            {
                if( (n & 3) == 0 )
                {
                    int n2 = n/2;
                    CCOMPLEX* dsth = dst + n2;

                    for( i = 0; i < n2; i += 2, itab += tab_step*2 )
                    {
                        j = itab[0];
                        assert( (unsigned)j < (unsigned)n2 );

                        CC_SWAP(dst[i+1], dsth[j], t);
                        if( j > i )
                        {
                            CC_SWAP(dst[i], dst[j], t);
                            CC_SWAP(dsth[i+1], dsth[j+1], t);
                        }
                    }
                }
                // else do nothing
            }
            else
            {
                for( i = 0; i < n; i++, itab += tab_step )
                {
                    j = itab[0];
                    assert( (unsigned)j < (unsigned)n );
                    if( j > i )
                        CC_SWAP(dst[i], dst[j], t);
                }
            }
        }

        if( inv )
        {
            for( i = 0; i <= n - 2; i += 2 )
            {
                T t0 = -dst[i].im;
                T t1 = -dst[i+1].im;
                dst[i].im = t0; dst[i+1].im = t1;
            }

            if( i < n )
                dst[n-1].im = -dst[n-1].im;
        }
    }

    n = 1;
    // 1. power-2 transforms
    if( (c.factors[0] & 1) == 0 )
    {
        if( c.factors[0] >= 4 && c.haveSSE3)
        {
            n = CCFUNC(DFT_VecR4)(dst, c.factors[0], c.n, dw0, wave);
        }

        // radix-4 transform
        for( ; n*4 <= c.factors[0]; )
        {
            nx = n;
            n *= 4;
            dw0 /= 4;

            for( i = 0; i < c.n; i += n )
            {
                CCOMPLEX *v0, *v1;
                T r0, i0, r1, i1, r2, i2, r3, i3, r4, i4;

                v0 = dst + i;
                v1 = v0 + nx*2;

                r0 = v1[0].re; i0 = v1[0].im;
                r4 = v1[nx].re; i4 = v1[nx].im;

                r1 = r0 + r4; i1 = i0 + i4;
                r3 = i0 - i4; i3 = r4 - r0;

                r2 = v0[0].re; i2 = v0[0].im;
                r4 = v0[nx].re; i4 = v0[nx].im;

                r0 = r2 + r4; i0 = i2 + i4;
                r2 -= r4; i2 -= i4;

                v0[0].re = r0 + r1; v0[0].im = i0 + i1;
                v1[0].re = r0 - r1; v1[0].im = i0 - i1;
                v0[nx].re = r2 + r3; v0[nx].im = i2 + i3;
                v1[nx].re = r2 - r3; v1[nx].im = i2 - i3;

                for( j = 1, dw = dw0; j < nx; j++, dw += dw0 )
                {
                    v0 = dst + i + j;
                    v1 = v0 + nx*2;

                    r2 = v0[nx].re*wave[dw*2].re - v0[nx].im*wave[dw*2].im;
                    i2 = v0[nx].re*wave[dw*2].im + v0[nx].im*wave[dw*2].re;
                    r0 = v1[0].re*wave[dw].im + v1[0].im*wave[dw].re;
                    i0 = v1[0].re*wave[dw].re - v1[0].im*wave[dw].im;
                    r3 = v1[nx].re*wave[dw*3].im + v1[nx].im*wave[dw*3].re;
                    i3 = v1[nx].re*wave[dw*3].re - v1[nx].im*wave[dw*3].im;

                    r1 = i0 + i3; i1 = r0 + r3;
                    r3 = r0 - r3; i3 = i3 - i0;
                    r4 = v0[0].re; i4 = v0[0].im;

                    r0 = r4 + r2; i0 = i4 + i2;
                    r2 = r4 - r2; i2 = i4 - i2;

                    v0[0].re = r0 + r1; v0[0].im = i0 + i1;
                    v1[0].re = r0 - r1; v1[0].im = i0 - i1;
                    v0[nx].re = r2 + r3; v0[nx].im = i2 + i3;
                    v1[nx].re = r2 - r3; v1[nx].im = i2 - i3;
                }
            }
        }

        for( ; n < c.factors[0]; )
        {
            // do the remaining radix-2 transform
            nx = n;
            n *= 2;
            dw0 /= 2;

            for( i = 0; i < c.n; i += n )
            {
                CCOMPLEX* v = dst + i;
                T r0 = v[0].re + v[nx].re;
                T i0 = v[0].im + v[nx].im;
                T r1 = v[0].re - v[nx].re;
                T i1 = v[0].im - v[nx].im;
                v[0].re = r0; v[0].im = i0;
                v[nx].re = r1; v[nx].im = i1;

                for( j = 1, dw = dw0; j < nx; j++, dw += dw0 )
                {
                    v = dst + i + j;
                    r1 = v[nx].re*wave[dw].re - v[nx].im*wave[dw].im;
                    i1 = v[nx].im*wave[dw].re + v[nx].re*wave[dw].im;
                    r0 = v[0].re; i0 = v[0].im;

                    v[0].re = r0 + r1; v[0].im = i0 + i1;
                    v[nx].re = r0 - r1; v[nx].im = i0 - i1;
                }
            }
        }
    }

    // 2. all the other transforms
    for( f_idx = (c.factors[0]&1) ? 0 : 1; f_idx < c.nf; f_idx++ )
    {
        int factor = c.factors[f_idx];
        nx = n;
        n *= factor;
        dw0 /= factor;

        if( factor == 3 )
        {
            // radix-3
            for( i = 0; i < c.n; i += n )
            {
                CCOMPLEX* v = dst + i;

                T r1 = v[nx].re + v[nx*2].re;
                T i1 = v[nx].im + v[nx*2].im;
                T r0 = v[0].re;
                T i0 = v[0].im;
                T r2 = sin_120*(v[nx].im - v[nx*2].im);
                T i2 = sin_120*(v[nx*2].re - v[nx].re);
                v[0].re = r0 + r1; v[0].im = i0 + i1;
                r0 -= (T)0.5*r1; i0 -= (T)0.5*i1;
                v[nx].re = r0 + r2; v[nx].im = i0 + i2;
                v[nx*2].re = r0 - r2; v[nx*2].im = i0 - i2;

                for( j = 1, dw = dw0; j < nx; j++, dw += dw0 )
                {
                    v = dst + i + j;
                    r0 = v[nx].re*wave[dw].re - v[nx].im*wave[dw].im;
                    i0 = v[nx].re*wave[dw].im + v[nx].im*wave[dw].re;
                    i2 = v[nx*2].re*wave[dw*2].re - v[nx*2].im*wave[dw*2].im;
                    r2 = v[nx*2].re*wave[dw*2].im + v[nx*2].im*wave[dw*2].re;
                    r1 = r0 + i2; i1 = i0 + r2;

                    r2 = sin_120*(i0 - r2); i2 = sin_120*(i2 - r0);
                    r0 = v[0].re; i0 = v[0].im;
                    v[0].re = r0 + r1; v[0].im = i0 + i1;
                    r0 -= (T)0.5*r1; i0 -= (T)0.5*i1;
                    v[nx].re = r0 + r2; v[nx].im = i0 + i2;
                    v[nx*2].re = r0 - r2; v[nx*2].im = i0 - i2;
                }
            }
        }
        else if( factor == 5 )
        {
            // radix-5
            for( i = 0; i < c.n; i += n )
            {
                for( j = 0, dw = 0; j < nx; j++, dw += dw0 )
                {
                    CCOMPLEX* v0 = dst + i + j;
                    CCOMPLEX* v1 = v0 + nx*2;
                    CCOMPLEX* v2 = v1 + nx*2;

                    T r0, i0, r1, i1, r2, i2, r3, i3, r4, i4, r5, i5;

                    r3 = v0[nx].re*wave[dw].re - v0[nx].im*wave[dw].im;
                    i3 = v0[nx].re*wave[dw].im + v0[nx].im*wave[dw].re;
                    r2 = v2[0].re*wave[dw*4].re - v2[0].im*wave[dw*4].im;
                    i2 = v2[0].re*wave[dw*4].im + v2[0].im*wave[dw*4].re;

                    r1 = r3 + r2; i1 = i3 + i2;
                    r3 -= r2; i3 -= i2;

                    r4 = v1[nx].re*wave[dw*3].re - v1[nx].im*wave[dw*3].im;
                    i4 = v1[nx].re*wave[dw*3].im + v1[nx].im*wave[dw*3].re;
                    r0 = v1[0].re*wave[dw*2].re - v1[0].im*wave[dw*2].im;
                    i0 = v1[0].re*wave[dw*2].im + v1[0].im*wave[dw*2].re;

                    r2 = r4 + r0; i2 = i4 + i0;
                    r4 -= r0; i4 -= i0;

                    r0 = v0[0].re; i0 = v0[0].im;
                    r5 = r1 + r2; i5 = i1 + i2;

                    v0[0].re = r0 + r5; v0[0].im = i0 + i5;

                    r0 -= (T)0.25*r5; i0 -= (T)0.25*i5;
                    r1 = fft5_2*(r1 - r2); i1 = fft5_2*(i1 - i2);
                    r2 = -fft5_3*(i3 + i4); i2 = fft5_3*(r3 + r4);

                    i3 *= -fft5_5; r3 *= fft5_5;
                    i4 *= -fft5_4; r4 *= fft5_4;

                    r5 = r2 + i3; i5 = i2 + r3;
                    r2 -= i4; i2 -= r4;

                    r3 = r0 + r1; i3 = i0 + i1;
                    r0 -= r1; i0 -= i1;

                    v0[nx].re = r3 + r2; v0[nx].im = i3 + i2;
                    v2[0].re = r3 - r2; v2[0].im = i3 - i2;

                    v1[0].re = r0 + r5; v1[0].im = i0 + i5;
                    v1[nx].re = r0 - r5; v1[nx].im = i0 - i5;
                }
            }
        }
        else
        {
            // radix-"factor" - an odd number
            int p, q, factor2 = (factor - 1)/2;
            int d, dd, dw_f = c.tab_size/factor;
            CCOMPLEX* buf = MALLOC(CCOMPLEX, factor2 * 2);
            CCOMPLEX* a = buf;
            CCOMPLEX* b = a + factor2;

            for( i = 0; i < c.n; i += n )
            {
                for( j = 0, dw = 0; j < nx; j++, dw += dw0 )
                {
                    CCOMPLEX* v = dst + i + j;
                    CCOMPLEX v_0 = v[0];
                    CCOMPLEX vn_0 = v_0;

                    if( j == 0 )
                    {
                        for( p = 1, k = nx; p <= factor2; p++, k += nx )
                        {
                            T r0 = v[k].re + v[n-k].re;
                            T i0 = v[k].im - v[n-k].im;
                            T r1 = v[k].re - v[n-k].re;
                            T i1 = v[k].im + v[n-k].im;

                            vn_0.re += r0; vn_0.im += i1;
                            a[p-1].re = r0; a[p-1].im = i0;
                            b[p-1].re = r1; b[p-1].im = i1;
                        }
                    }
                    else
                    {
                        const CCOMPLEX* wave_ = wave + dw*factor;
                        d = dw;

                        for( p = 1, k = nx; p <= factor2; p++, k += nx, d += dw )
                        {
                            T r2 = v[k].re*wave[d].re - v[k].im*wave[d].im;
                            T i2 = v[k].re*wave[d].im + v[k].im*wave[d].re;

                            T r1 = v[n-k].re*wave_[-d].re - v[n-k].im*wave_[-d].im;
                            T i1 = v[n-k].re*wave_[-d].im + v[n-k].im*wave_[-d].re;

                            T r0 = r2 + r1;
                            T i0 = i2 - i1;
                            r1 = r2 - r1;
                            i1 = i2 + i1;

                            vn_0.re += r0; vn_0.im += i1;
                            a[p-1].re = r0; a[p-1].im = i0;
                            b[p-1].re = r1; b[p-1].im = i1;
                        }
                    }

                    v[0] = vn_0;

                    for( p = 1, k = nx; p <= factor2; p++, k += nx )
                    {
                        CCOMPLEX s0 = v_0, s1 = v_0;
                        d = dd = dw_f*p;

                        for( q = 0; q < factor2; q++ )
                        {
                            T r0 = wave[d].re * a[q].re;
                            T i0 = wave[d].im * a[q].im;
                            T r1 = wave[d].re * b[q].im;
                            T i1 = wave[d].im * b[q].re;

                            s1.re += r0 + i0; s0.re += r0 - i0;
                            s1.im += r1 - i1; s0.im += r1 + i1;

                            d += dd;
                            d -= -(d >= c.tab_size) & c.tab_size;
                        }

                        v[k] = s0;
                        v[n-k] = s1;
                    }
                }
            }
            FREE(buf);
        }
    }

    if( scale != 1 )
    {
        T re_scale = scale, im_scale = scale;
        if( inv )
            im_scale = -im_scale;

        for( i = 0; i < c.n; i++ )
        {
            T t0 = dst[i].re*re_scale;
            T t1 = dst[i].im*im_scale;
            dst[i].re = t0;
            dst[i].im = t1;
        }
    }
    else if( inv )
    {
        for( i = 0; i <= c.n - 2; i += 2 )
        {
            T t0 = -dst[i].im;
            T t1 = -dst[i+1].im;
            dst[i].im = t0;
            dst[i+1].im = t1;
        }

        if( i < c.n )
            dst[c.n-1].im = -dst[c.n-1].im;
    }
}

/* FFT of real vector
   output vector format:
     re(0), re(1), im(1), ... , re(n/2-1), im((n+1)/2-1) [, re((n+1)/2)] OR ...
     re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
static void
CCFUNC(RealDFT)(const OcvDftOptions & c, const T* src, T* dst)
{
    int n = c.n;
    int complex_output = c.isComplex;
    T scale = (T)c.scale;
    int j;
    dst += complex_output;

    if( c.useIpp )
    {
#ifdef USE_IPP_DFT
        if (ippsDFTFwd_RToPack( src, dst, c.ipp_spec, c.ipp_work ) >=0)
        {
            if( complex_output )
            {
                dst[-1] = dst[0];
                dst[0] = 0;
                if( (n & 1) == 0 )
                    dst[n] = 0;
            }
            CC_IMPL_ADD(CC_IMPL_IPP);
            return;
        }
        setIppErrorStatus();
#endif
    }
    assert( c.tab_size == n );

    if( n == 1 )
    {
        dst[0] = src[0]*scale;
    }
    else if( n == 2 )
    {
        T t = (src[0] + src[1])*scale;
        dst[1] = (src[0] - src[1])*scale;
        dst[0] = t;
    }
    else if( n & 1 )
    {
        dst -= complex_output;
        CCOMPLEX* _dst = (CCOMPLEX*)dst;
        _dst[0].re = src[0]*scale;
        _dst[0].im = 0;
        for( j = 1; j < n; j += 2 )
        {
            T t0 = src[c.itab[j]]*scale;
            T t1 = src[c.itab[j+1]]*scale;
            _dst[j].re = t0;
            _dst[j].im = 0;
            _dst[j+1].re = t1;
            _dst[j+1].im = 0;
        }
        OcvDftOptions sub_c = c;
        sub_c.isComplex = false;
        sub_c.isInverse = false;
        sub_c.noPermute = true;
        sub_c.scale = 1.;
        CCFUNC(DFT)(sub_c, _dst, _dst);
        if( !complex_output )
            dst[1] = dst[0];
    }
    else
    {
        T t0, t;
        T h1_re, h1_im, h2_re, h2_im;
        T scale2 = scale*(T)0.5;
        int n2 = n >> 1;

        c.factors[0] >>= 1;

        OcvDftOptions sub_c = c;
        sub_c.factors += (c.factors[0] == 1);
        sub_c.nf -= (c.factors[0] == 1);
        sub_c.isComplex = false;
        sub_c.isInverse = false;
        sub_c.noPermute = false;
        sub_c.scale = 1.;
        sub_c.n = n2;

        CCFUNC(DFT)(sub_c, (CCOMPLEX*)src, (CCOMPLEX*)dst);

        c.factors[0] <<= 1;

        t = dst[0] - dst[1];
        dst[0] = (dst[0] + dst[1])*scale;
        dst[1] = t*scale;

        t0 = dst[n2];
        t = dst[n-1];
        dst[n-1] = dst[1];

        const CCOMPLEX *wave = (const CCOMPLEX*)c.wave;

        for( j = 2, wave++; j < n2; j += 2, wave++ )
        {
            /* calc odd */
            h2_re = scale2*(dst[j+1] + t);
            h2_im = scale2*(dst[n-j] - dst[j]);

            /* calc even */
            h1_re = scale2*(dst[j] + dst[n-j]);
            h1_im = scale2*(dst[j+1] - t);

            /* rotate */
            t = h2_re*wave->re - h2_im*wave->im;
            h2_im = h2_re*wave->im + h2_im*wave->re;
            h2_re = t;
            t = dst[n-j-1];

            dst[j-1] = h1_re + h2_re;
            dst[n-j-1] = h1_re - h2_re;
            dst[j] = h1_im + h2_im;
            dst[n-j] = h2_im - h1_im;
        }

        if( j <= n2 )
        {
            dst[n2-1] = t0*scale;
            dst[n2] = -t*scale;
        }
    }

    if( complex_output && ((n & 1) == 0 || n == 1))
    {
        dst[-1] = dst[0];
        dst[0] = 0;
        if( n > 1 )
            dst[n] = 0;
    }
}

/* Inverse FFT of complex conjugate-symmetric vector
   input vector format:
      re[0], re[1], im[1], ... , re[n/2-1], im[n/2-1], re[n/2] OR
      re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */
static void
CCFUNC(CCSIDFT)(const OcvDftOptions & c, const T* src, T* dst)
{
    int n = c.n;
    int complex_input = c.isComplex;
    int j, k;
    T scale = (T)c.scale;
    T save_s1 = 0.;
    T t0, t1, t2, t3, t;

    assert( c.tab_size == n );

    if( complex_input )
    {
        assert( src != dst );
        save_s1 = src[1];
        ((T*)src)[1] = src[0];
        src++;
    }
    if( c.useIpp )
    {
#ifdef USE_IPP_DFT
        if (ippsDFTInv_PackToR( src, dst, c.ipp_spec, c.ipp_work ) >=0)
        {
            if( complex_input )
                ((T*)src)[0] = (T)save_s1;
            CC_IMPL_ADD(CC_IMPL_IPP);
            return;
        }

        setIppErrorStatus();
#endif
    }
    if( n == 1 )
    {
        dst[0] = (T)(src[0]*scale);
    }
    else if( n == 2 )
    {
        t = (src[0] + src[1])*scale;
        dst[1] = (src[0] - src[1])*scale;
        dst[0] = t;
    }
    else if( n & 1 )
    {
        CCOMPLEX* _src = (CCOMPLEX*)(src-1);
        CCOMPLEX* _dst = (CCOMPLEX*)dst;

        _dst[0].re = src[0];
        _dst[0].im = 0;

        int n2 = (n+1) >> 1;

        for( j = 1; j < n2; j++ )
        {
            int k0 = c.itab[j], k1 = c.itab[n-j];
            t0 = _src[j].re; t1 = _src[j].im;
            _dst[k0].re = t0; _dst[k0].im = -t1;
            _dst[k1].re = t0; _dst[k1].im = t1;
        }

        OcvDftOptions sub_c = c;
        sub_c.isComplex = false;
        sub_c.isInverse = false;
        sub_c.noPermute = true;
        sub_c.scale = 1.;
        sub_c.n = n;

        CCFUNC(DFT)(sub_c, _dst, _dst);
        dst[0] *= scale;
        for( j = 1; j < n; j += 2 )
        {
            t0 = dst[j*2]*scale;
            t1 = dst[j*2+2]*scale;
            dst[j] = t0;
            dst[j+1] = t1;
        }
    }
    else
    {
        int inplace = src == dst;
        const CCOMPLEX* w = (const CCOMPLEX*)c.wave;

        t = src[1];
        t0 = (src[0] + src[n-1]);
        t1 = (src[n-1] - src[0]);
        dst[0] = t0;
        dst[1] = t1;

        int n2 = (n+1) >> 1;

        for( j = 2, w++; j < n2; j += 2, w++ )
        {
            T h1_re, h1_im, h2_re, h2_im;

            h1_re = (t + src[n-j-1]);
            h1_im = (src[j] - src[n-j]);

            h2_re = (t - src[n-j-1]);
            h2_im = (src[j] + src[n-j]);

            t = h2_re*w->re + h2_im*w->im;
            h2_im = h2_im*w->re - h2_re*w->im;
            h2_re = t;

            t = src[j+1];
            t0 = h1_re - h2_im;
            t1 = -h1_im - h2_re;
            t2 = h1_re + h2_im;
            t3 = h1_im - h2_re;

            if( inplace )
            {
                dst[j] = t0;
                dst[j+1] = t1;
                dst[n-j] = t2;
                dst[n-j+1]= t3;
            }
            else
            {
                int j2 = j >> 1;
                k = c.itab[j2];
                dst[k] = t0;
                dst[k+1] = t1;
                k = c.itab[n2-j2];
                dst[k] = t2;
                dst[k+1]= t3;
            }
        }

        if( j <= n2 )
        {
            t0 = t*2;
            t1 = src[n2]*2;

            if( inplace )
            {
                dst[n2] = t0;
                dst[n2+1] = t1;
            }
            else
            {
                k = c.itab[n2];
                dst[k*2] = t0;
                dst[k*2+1] = t1;
            }
        }

        c.factors[0] >>= 1;

        OcvDftOptions sub_c = c;
        sub_c.factors += (c.factors[0] == 1);
        sub_c.nf -= (c.factors[0] == 1);
        sub_c.isComplex = false;
        sub_c.isInverse = false;
        sub_c.noPermute = !inplace;
        sub_c.scale = 1.;
        sub_c.n = n2;

        CCFUNC(DFT)(sub_c, (CCOMPLEX*)dst, (CCOMPLEX*)dst);

        c.factors[0] <<= 1;

        for( j = 0; j < n; j += 2 )
        {
            t0 = dst[j]*scale;
            t1 = dst[j+1]*(-scale);
            dst[j] = t0;
            dst[j+1] = t1;
        }
    }
    if( complex_input )
        ((T*)src)[0] = (T)save_s1;
}

/* DCT is calculated using DFT, as described here:
   http://www.ece.utexas.edu/~bevans/courses/ee381k/lectures/09_DCT/lecture9/:
*/
static void
CCFUNC(DCT)( const OcvDftOptions & c, const T* src, size_t src_step, T* dft_src, T* dft_dst, T* dst, size_t dst_step,
     const CCOMPLEX* dct_wave )
{
    static const T sin_45 = (T)0.70710678118654752440084436210485;

    int n = c.n;
    int j, n2 = n >> 1;

    src_step /= sizeof(src[0]);
    dst_step /= sizeof(dst[0]);
    T* dst1 = dst + (n-1)*dst_step;

    if( n == 1 )
    {
        dst[0] = src[0];
        return;
    }

    for( j = 0; j < n2; j++, src += src_step*2 )
    {
        dft_src[j] = src[0];
        dft_src[n-j-1] = src[src_step];
    }

    CCFUNC(RealDFT)(c, dft_src, dft_dst);
    src = dft_dst;

    dst[0] = (T)(src[0]*dct_wave->re*sin_45);
    dst += dst_step;
    for( j = 1, dct_wave++; j < n2; j++, dct_wave++,
                                    dst += dst_step, dst1 -= dst_step )
    {
        T t0 = dct_wave->re*src[j*2-1] - dct_wave->im*src[j*2];
        T t1 = -dct_wave->im*src[j*2-1] - dct_wave->re*src[j*2];
        dst[0] = t0;
        dst1[0] = t1;
    }

    dst[0] = src[n-1]*dct_wave->re;
}


static void
CCFUNC(IDCT)( const OcvDftOptions & c, const T* src, size_t src_step, T* dft_src, T* dft_dst, T* dst, size_t dst_step,
      const CCOMPLEX* dct_wave)
{
    static const T sin_45 = (T)0.70710678118654752440084436210485;
    int n = c.n;
    int j, n2 = n >> 1;

    src_step /= sizeof(src[0]);
    dst_step /= sizeof(dst[0]);
    const T* src1 = src + (n-1)*src_step;

    if( n == 1 )
    {
        dst[0] = src[0];
        return;
    }

    dft_src[0] = (T)(src[0]*2*dct_wave->re*sin_45);
    src += src_step;
    for( j = 1, dct_wave++; j < n2; j++, dct_wave++,
                                    src += src_step, src1 -= src_step )
    {
        T t0 = dct_wave->re*src[0] - dct_wave->im*src1[0];
        T t1 = -dct_wave->im*src[0] - dct_wave->re*src1[0];
        dft_src[j*2-1] = t0;
        dft_src[j*2] = t1;
    }

    dft_src[n-1] = (T)(src[0]*2*dct_wave->re);
    CCFUNC(CCSIDFT)(c, dft_src, dft_dst);

    for( j = 0; j < n2; j++, dst += dst_step*2 )
    {
        dst[0] = dft_dst[j];
        dst[dst_step] = dft_dst[n-j-1];
    }
}



#undef CCFUNC
#undef ISDOUBLE
#undef CCOMPLEX
#undef T
