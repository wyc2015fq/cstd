
#ifndef _CXDXT_INL_
#define _CXDXT_INL_

#include <math.h>
#include <assert.h>
#include <float.h>
#include <stdlib.h>
#include <string.h>
#include <pmalloc.h>

typedef float f32;
typedef double f64;

#undef CC_IS_MAT_CONT
#define CC_IS_MAT_CONT(a)  (0)


// On Win64 (IA64) optimized versions of DFT and DCT fail the tests
#if defined WIN64 && !defined EM64T
#pragma optimize("", off)
#endif

/****************************************************************************************\
                               Discrete Fourier Transform
\****************************************************************************************/

#define CC_MAX_LOCAL_DFT_SIZE  (1 << 15)

static const uchar log2tab[] = { 0, 0, 1, 0, 2, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0 };
static int icvlog2( int n )
{
    int m = 0;
    int f = (n >= (1 << 16))*16;
    n >>= f;
    m += f;
    f = (n >= (1 << 8))*8;
    n >>= f;
    m += f;
    f = (n >= (1 << 4))*4;
    n >>= f;
    return m + f + log2tab[n];
}

static unsigned char icvRevTable[] =
{
  0x00,0x80,0x40,0xc0,0x20,0xa0,0x60,0xe0,0x10,0x90,0x50,0xd0,0x30,0xb0,0x70,0xf0,
  0x08,0x88,0x48,0xc8,0x28,0xa8,0x68,0xe8,0x18,0x98,0x58,0xd8,0x38,0xb8,0x78,0xf8,
  0x04,0x84,0x44,0xc4,0x24,0xa4,0x64,0xe4,0x14,0x94,0x54,0xd4,0x34,0xb4,0x74,0xf4,
  0x0c,0x8c,0x4c,0xcc,0x2c,0xac,0x6c,0xec,0x1c,0x9c,0x5c,0xdc,0x3c,0xbc,0x7c,0xfc,
  0x02,0x82,0x42,0xc2,0x22,0xa2,0x62,0xe2,0x12,0x92,0x52,0xd2,0x32,0xb2,0x72,0xf2,
  0x0a,0x8a,0x4a,0xca,0x2a,0xaa,0x6a,0xea,0x1a,0x9a,0x5a,0xda,0x3a,0xba,0x7a,0xfa,
  0x06,0x86,0x46,0xc6,0x26,0xa6,0x66,0xe6,0x16,0x96,0x56,0xd6,0x36,0xb6,0x76,0xf6,
  0x0e,0x8e,0x4e,0xce,0x2e,0xae,0x6e,0xee,0x1e,0x9e,0x5e,0xde,0x3e,0xbe,0x7e,0xfe,
  0x01,0x81,0x41,0xc1,0x21,0xa1,0x61,0xe1,0x11,0x91,0x51,0xd1,0x31,0xb1,0x71,0xf1,
  0x09,0x89,0x49,0xc9,0x29,0xa9,0x69,0xe9,0x19,0x99,0x59,0xd9,0x39,0xb9,0x79,0xf9,
  0x05,0x85,0x45,0xc5,0x25,0xa5,0x65,0xe5,0x15,0x95,0x55,0xd5,0x35,0xb5,0x75,0xf5,
  0x0d,0x8d,0x4d,0xcd,0x2d,0xad,0x6d,0xed,0x1d,0x9d,0x5d,0xdd,0x3d,0xbd,0x7d,0xfd,
  0x03,0x83,0x43,0xc3,0x23,0xa3,0x63,0xe3,0x13,0x93,0x53,0xd3,0x33,0xb3,0x73,0xf3,
  0x0b,0x8b,0x4b,0xcb,0x2b,0xab,0x6b,0xeb,0x1b,0x9b,0x5b,0xdb,0x3b,0xbb,0x7b,0xfb,
  0x07,0x87,0x47,0xc7,0x27,0xa7,0x67,0xe7,0x17,0x97,0x57,0xd7,0x37,0xb7,0x77,0xf7,
  0x0f,0x8f,0x4f,0xcf,0x2f,0xaf,0x6f,0xef,0x1f,0x9f,0x5f,0xdf,0x3f,0xbf,0x7f,0xff
};

static const double icvDxtTab[][2] =
{
{ 1.00000000000000000, 0.00000000000000000 },
{-1.00000000000000000, 0.00000000000000000 },
{ 0.00000000000000000, 1.00000000000000000 },
{ 0.70710678118654757, 0.70710678118654746 },
{ 0.92387953251128674, 0.38268343236508978 },
{ 0.98078528040323043, 0.19509032201612825 },
{ 0.99518472667219693, 0.09801714032956060 },
{ 0.99879545620517241, 0.04906767432741802 },
{ 0.99969881869620425, 0.02454122852291229 },
{ 0.99992470183914450, 0.01227153828571993 },
{ 0.99998117528260111, 0.00613588464915448 },
{ 0.99999529380957619, 0.00306795676296598 },
{ 0.99999882345170188, 0.00153398018628477 },
{ 0.99999970586288223, 0.00076699031874270 },
{ 0.99999992646571789, 0.00038349518757140 },
{ 0.99999998161642933, 0.00019174759731070 },
{ 0.99999999540410733, 0.00009587379909598 },
{ 0.99999999885102686, 0.00004793689960307 },
{ 0.99999999971275666, 0.00002396844980842 },
{ 0.99999999992818922, 0.00001198422490507 },
{ 0.99999999998204725, 0.00000599211245264 },
{ 0.99999999999551181, 0.00000299605622633 },
{ 0.99999999999887801, 0.00000149802811317 },
{ 0.99999999999971945, 0.00000074901405658 },
{ 0.99999999999992983, 0.00000037450702829 },
{ 0.99999999999998246, 0.00000018725351415 },
{ 0.99999999999999567, 0.00000009362675707 },
{ 0.99999999999999889, 0.00000004681337854 },
{ 0.99999999999999978, 0.00000002340668927 },
{ 0.99999999999999989, 0.00000001170334463 },
{ 1.00000000000000000, 0.00000000585167232 },
{ 1.00000000000000000, 0.00000000292583616 }
};

#define icvBitRev(i,shift) \
   ((int)((((unsigned)icvRevTable[(i)&255] << 24)+ \
           ((unsigned)icvRevTable[((i)>> 8)&255] << 16)+ \
           ((unsigned)icvRevTable[((i)>>16)&255] <<  8)+ \
           ((unsigned)icvRevTable[((i)>>24)])) >> (shift)))

static int
icvDFTFactorize( int n, int* factors )
{
    int nf = 0, f, i, j;

    if( n <= 5 )
    {
        factors[0] = n;
        return 1;
    }
    
    f = (((n - 1)^n)+1) >> 1;
    if( f > 1 )
    {
        factors[nf++] = f;
        n = f == n ? 1 : n/f;
    }

    for( f = 3; n > 1; )
    {
        int d = n/f;
        if( d*f == n )
        {
            factors[nf++] = f;
            n = d;
        }
        else
        {
            f += 2;
            if( f*f > n )
                break;
        }
    }

    if( n > 1 )
        factors[nf++] = n;

    f = (factors[0] & 1) == 0;
    for( i = f; i < (nf+f)/2; i++ )
        CC_SWAP( factors[i], factors[nf-i-1+f], j );

    return nf;
}

static int icvDFTInit( int n0, int nf, const int* factors, int* itab, int elem_size, void* _wave, int inv_itab )
{
    int digits[34], radix[34];
    int n = factors[0], m = 0;
    int* itab0 = itab;
    int i, j, k;
    CvComplex64f w, w1;
    double t;

    if( n0 <= 5 )
    {
        itab[0] = 0;
        itab[n0-1] = n0-1;
        
        if( n0 != 4 )
        {
          for( i = 1; i < n0-1; i++ ) {
                itab[i] = i;
          }
        }
        else
        {
            itab[1] = 2;
            itab[2] = 1;
        }
        if( n0 == 5 )
        {
          if( elem_size == sizeof(CvComplex64f) ) {
                ((CvComplex64f*)_wave)[0] = cvComplex64f(1.,0.);
          } else {
                ((CvComplex32f*)_wave)[0] = cvComplex32f(1.f,0.f);
          }
        }
        if( n0 != 4 ) {
            return 0;
        }
        m = 2;
    }
    else
    {
        radix[nf] = 1;
        digits[nf] = 0;
        for( i = 0; i < nf; i++ )
        {
            digits[i] = 0;
            radix[nf-i-1] = radix[nf-i]*factors[nf-i-1];
        }

        if( inv_itab && factors[0] != factors[nf-1] )
            itab = (int*)_wave;

        if( (n & 1) == 0 )
        {
            int a = radix[1], na2 = n*a>>1, na4 = na2 >> 1;
            m = icvlog2(n);
        
            if( n <= 2  )
            {
                itab[0] = 0;
                itab[1] = na2;
            }
            else if( n <= 256 )
            {
                int shift = 10 - m;
                for( i = 0; i <= n - 4; i += 4 )
                {
                    j = (icvRevTable[i>>2]>>shift)*a;
                    itab[i] = j;
                    itab[i+1] = j + na2;
                    itab[i+2] = j + na4;
                    itab[i+3] = j + na2 + na4;
                }
            }
            else
            {
                int shift = 34 - m;
                for( i = 0; i < n; i += 4 )
                {
                    int i4 = i >> 2;
                    j = icvBitRev(i4,shift)*a;
                    itab[i] = j;
                    itab[i+1] = j + na2;
                    itab[i+2] = j + na4;
                    itab[i+3] = j + na2 + na4;
                }
            }

            digits[1]++;
            for( i = n, j = radix[2]; i < n0; )
            {
                for( k = 0; k < n; k++ )
                    itab[i+k] = itab[k] + j;
                if( (i += n) >= n0 )
                    break;
                j += radix[2];
                for( k = 1; ++digits[k] >= factors[k]; k++ )
                {
                    digits[k] = 0;
                    j += radix[k+2] - radix[k];
                }
            }
        }
        else
        {
            for( i = 0, j = 0;; )
            {
                itab[i] = j;
                if( ++i >= n0 )
                    break;
                j += radix[1];
                for( k = 0; ++digits[k] >= factors[k]; k++ )
                {
                    digits[k] = 0;
                    j += radix[k+2] - radix[k];
                }
            }
        }

        if( itab != itab0 )
        {
            itab0[0] = 0;
            for( i = n0 & 1; i < n0; i += 2 )
            {
                int k0 = itab[i];
                int k1 = itab[i+1];
                itab0[k0] = i;
                itab0[k1] = i+1;
            }
        }
    }

    if( (n0 & (n0-1)) == 0 )
    {
        w.re = w1.re = icvDxtTab[m][0];
        w.im = w1.im = -icvDxtTab[m][1];
    }
    else
    {
        t = -CC_PI*2/n0;
        w.im = w1.im = sin(t);
        w.re = w1.re = sqrt(1. - w1.im*w1.im);
    }
    n = (n0+1)/2;

    if( elem_size == sizeof(CvComplex64f) )
    {
        CvComplex64f* wave = (CvComplex64f*)_wave;

        wave[0].re = 1.;
        wave[0].im = 0.;

        if( (n0 & 1) == 0 )
        {
            wave[n].re = -1.;
            wave[n].im = 0;
        }

        for( i = 1; i < n; i++ )
        {
            wave[i] = w;
            wave[n0-i].re = w.re;
            wave[n0-i].im = -w.im;

            t = w.re*w1.re - w.im*w1.im;
            w.im = w.re*w1.im + w.im*w1.re;
            w.re = t;
        }
    }
    else
    {
        CvComplex32f* wave = (CvComplex32f*)_wave;
        assert( elem_size == sizeof(CvComplex32f) );
        
        wave[0].re = 1.f;
        wave[0].im = 0.f;

        if( (n0 & 1) == 0 )
        {
            wave[n].re = -1.f;
            wave[n].im = 0.f;
        }

        for( i = 1; i < n; i++ )
        {
            wave[i].re = (float)w.re;
            wave[i].im = (float)w.im;
            wave[n0-i].re = (float)w.re;
            wave[n0-i].im = (float)-w.im;

            t = w.re*w1.re - w.im*w1.im;
            w.im = w.re*w1.im + w.im*w1.re;
            w.re = t;
        }
    }
    return 0;
}


static const double icv_sin_120 = 0.86602540378443864676372317075294;
static const double icv_sin_45 = 0.70710678118654752440084436210485;
static const double icv_fft5_2 = 0.559016994374947424102293417182819;
static const double icv_fft5_3 = -0.951056516295153572116439333379382;
static const double icv_fft5_4 = -1.538841768587626701285145288018455;
static const double icv_fft5_5 = 0.363271264002680442947733378740309;

#define ICC_DFT_NO_PERMUTE 2
#define ICC_DFT_COMPLEX_INPUT_OR_OUTPUT 4

// mixed-radix complex discrete Fourier transform: double-precision version
static int 
icvDFT_64fc( const CvComplex64f* src, CvComplex64f* dst, int n,
             int nf, int* factors, const int* itab,
             const CvComplex64f* wave, int tab_size,
             const void* spec, CvComplex64f* buf,
             int flags, double scale )
{
    int n0 = n, f_idx, nx;
    int inv = flags & CC_DXT_INVERSE;
    int dw0 = tab_size, dw;
    int i, j, k;
    CvComplex64f t;
    int tab_step;

    tab_step = tab_size == n ? 1 : tab_size == n*2 ? 2 : tab_size/n;

    // 0. shuffle data
    if( dst != src )
    {
        assert( (flags & ICC_DFT_NO_PERMUTE) == 0 );
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
        if( (flags & ICC_DFT_NO_PERMUTE) == 0 )
        {
            if( factors[0] != factors[nf-1] )
                return CC_INPLACE_NOT_SUPPORTED_ERR;
            if( nf == 1 )
            {
                if( (n & 3) == 0 )
                {
                    int n2 = n/2;
                    CvComplex64f* dsth = dst + n2;
                
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
                double t0 = -dst[i].im;
                double t1 = -dst[i+1].im;
                dst[i].im = t0; dst[i+1].im = t1;
            }

            if( i < n )
                dst[n-1].im = -dst[n-1].im;
        }
    }

    n = 1;
    // 1. power-2 transforms
    if( (factors[0] & 1) == 0 )
    {
        // radix-4 transform
        for( ; n*4 <= factors[0]; )
        {
            nx = n;
            n *= 4;
            dw0 /= 4;

            for( i = 0; i < n0; i += n )
            {
                CvComplex64f* v0;
                CvComplex64f* v1;
                double r0, i0, r1, i1, r2, i2, r3, i3, r4, i4;

                v0 = dst + i;
                v1 = v0 + nx*2;

                r2 = v0[0].re; i2 = v0[0].im;
                r1 = v0[nx].re; i1 = v0[nx].im;
                
                r0 = r1 + r2; i0 = i1 + i2;
                r2 -= r1; i2 -= i1;

                i3 = v1[nx].re; r3 = v1[nx].im;
                i4 = v1[0].re; r4 = v1[0].im;

                r1 = i4 + i3; i1 = r4 + r3;
                r3 = r4 - r3; i3 = i3 - i4;

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

        for( ; n < factors[0]; )
        {
            // do the remaining radix-2 transform
            nx = n;
            n *= 2;
            dw0 /= 2;

            for( i = 0; i < n0; i += n )
            {
                CvComplex64f* v = dst + i;
                double r0 = v[0].re + v[nx].re;
                double i0 = v[0].im + v[nx].im;
                double r1 = v[0].re - v[nx].re;
                double i1 = v[0].im - v[nx].im;
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
    for( f_idx = (factors[0]&1) ? 0 : 1; f_idx < nf; f_idx++ )
    {
        int factor = factors[f_idx];
        nx = n;
        n *= factor;
        dw0 /= factor;

        if( factor == 3 )
        {
            // radix-3
            for( i = 0; i < n0; i += n )
            {
                CvComplex64f* v = dst + i;

                double r1 = v[nx].re + v[nx*2].re;
                double i1 = v[nx].im + v[nx*2].im;
                double r0 = v[0].re;
                double i0 = v[0].im;
                double r2 = icv_sin_120*(v[nx].im - v[nx*2].im);
                double i2 = icv_sin_120*(v[nx*2].re - v[nx].re);
                v[0].re = r0 + r1; v[0].im = i0 + i1;
                r0 -= 0.5*r1; i0 -= 0.5*i1;
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
                    
                    r2 = icv_sin_120*(i0 - r2); i2 = icv_sin_120*(i2 - r0);
                    r0 = v[0].re; i0 = v[0].im;
                    v[0].re = r0 + r1; v[0].im = i0 + i1;
                    r0 -= 0.5*r1; i0 -= 0.5*i1;
                    v[nx].re = r0 + r2; v[nx].im = i0 + i2;
                    v[nx*2].re = r0 - r2; v[nx*2].im = i0 - i2;
                }
            }
        }
        else if( factor == 5 )
        {
            // radix-5
            for( i = 0; i < n0; i += n )
            {
                for( j = 0, dw = 0; j < nx; j++, dw += dw0 )
                {
                    CvComplex64f* v0 = dst + i + j;
                    CvComplex64f* v1 = v0 + nx*2;
                    CvComplex64f* v2 = v1 + nx*2;

                    double r0, i0, r1, i1, r2, i2, r3, i3, r4, i4, r5, i5;

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

                    r0 -= 0.25*r5; i0 -= 0.25*i5;
                    r1 = icv_fft5_2*(r1 - r2); i1 = icv_fft5_2*(i1 - i2);
                    r2 = -icv_fft5_3*(i3 + i4); i2 = icv_fft5_3*(r3 + r4);

                    i3 *= -icv_fft5_5; r3 *= icv_fft5_5;
                    i4 *= -icv_fft5_4; r4 *= icv_fft5_4;

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
            int d, dd, dw_f = tab_size/factor;
            CvComplex64f* a = buf;
            CvComplex64f* b = buf + factor2;

            for( i = 0; i < n0; i += n )
            {
                for( j = 0, dw = 0; j < nx; j++, dw += dw0 )
                {
                    CvComplex64f* v = dst + i + j;
                    CvComplex64f v_0 = v[0];
                    CvComplex64f vn_0 = v_0;

                    if( j == 0 )
                    {
                        for( p = 1, k = nx; p <= factor2; p++, k += nx )
                        {
                            double r0 = v[k].re + v[n-k].re;
                            double i0 = v[k].im - v[n-k].im;
                            double r1 = v[k].re - v[n-k].re;
                            double i1 = v[k].im + v[n-k].im;

                            vn_0.re += r0; vn_0.im += i1;
                            a[p-1].re = r0; a[p-1].im = i0;
                            b[p-1].re = r1; b[p-1].im = i1;
                        }
                    }
                    else
                    {
                        const CvComplex64f* wave_ = wave + dw*factor;
                        d = dw;

                        for( p = 1, k = nx; p <= factor2; p++, k += nx, d += dw )
                        {
                            double r2 = v[k].re*wave[d].re - v[k].im*wave[d].im;
                            double i2 = v[k].re*wave[d].im + v[k].im*wave[d].re;

                            double r1 = v[n-k].re*wave_[-d].re - v[n-k].im*wave_[-d].im;
                            double i1 = v[n-k].re*wave_[-d].im + v[n-k].im*wave_[-d].re;
                    
                            double r0 = r2 + r1;
                            double i0 = i2 - i1;
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
                        CvComplex64f s0 = v_0, s1 = v_0;
                        d = dd = dw_f*p;

                        for( q = 0; q < factor2; q++ )
                        {
                            double r0 = wave[d].re * a[q].re;
                            double i0 = wave[d].im * a[q].im;
                            double r1 = wave[d].re * b[q].im;
                            double i1 = wave[d].im * b[q].re;
            
                            s1.re += r0 + i0; s0.re += r0 - i0;
                            s1.im += r1 - i1; s0.im += r1 + i1;

                            d += dd;
                            d -= -(d >= tab_size) & tab_size;
                        }

                        v[k] = s0;
                        v[n-k] = s1;
                    }
                }
            }
        }
    }

    if( fabs(scale - 1.) > DBL_EPSILON )
    {
        double re_scale = scale, im_scale = scale;
        if( inv )
            im_scale = -im_scale;

        for( i = 0; i < n0; i++ )
        {
            double t0 = dst[i].re*re_scale;
            double t1 = dst[i].im*im_scale;
            dst[i].re = t0;
            dst[i].im = t1;
        }
    }
    else if( inv )
    {
        for( i = 0; i <= n0 - 2; i += 2 )
        {
            double t0 = -dst[i].im;
            double t1 = -dst[i+1].im;
            dst[i].im = t0;
            dst[i+1].im = t1;
        }

        if( i < n0 )
            dst[n0-1].im = -dst[n0-1].im;
    }

    return CC_OK;
}


// mixed-radix complex discrete Fourier transform: single-precision version
static int 
icvDFT_32fc( const CvComplex32f* src, CvComplex32f* dst, int n,
             int nf, int* factors, const int* itab,
             const CvComplex32f* wave, int tab_size,
             const void* spec, CvComplex32f* buf,
             int flags, double scale )
{
    int n0 = n, f_idx, nx;
    int inv = flags & CC_DXT_INVERSE;
    int dw0 = tab_size, dw;
    int i, j, k;
    CvComplex32f t;
    int tab_step = tab_size == n ? 1 : tab_size == n*2 ? 2 : tab_size/n;

    // 0. shuffle data
    if( dst != src )
    {
        assert( (flags & ICC_DFT_NO_PERMUTE) == 0 );
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
        if( (flags & ICC_DFT_NO_PERMUTE) == 0 )
        {
            if( factors[0] != factors[nf-1] )
                return CC_INPLACE_NOT_SUPPORTED_ERR;
            if( nf == 1 )
            {
                if( (n & 3) == 0 )
                {
                    int n2 = n/2;
                    CvComplex32f* dsth = dst + n2;
                
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
            // conjugate the vector - i.e. invert sign of the imaginary part
            int* idst = (int*)dst;
            for( i = 0; i <= n - 2; i += 2 )
            {
                int t0 = idst[i*2+1] ^ 0x80000000;
                int t1 = idst[i*2+3] ^ 0x80000000;
                idst[i*2+1] = t0; idst[i*2+3] = t1;
            }

            if( i < n )
                idst[2*i+1] ^= 0x80000000;
        }
    }

    n = 1;
    // 1. power-2 transforms
    if( (factors[0] & 1) == 0 )
    {
        // radix-4 transform
        for( ; n*4 <= factors[0]; )
        {
            nx = n;
            n *= 4;
            dw0 /= 4;

            for( i = 0; i < n0; i += n )
            {
                CvComplex32f* v0;
                CvComplex32f* v1;
                double r0, i0, r1, i1, r2, i2, r3, i3, r4, i4;

                v0 = dst + i;
                v1 = v0 + nx*2;

                r2 = v0[0].re; i2 = v0[0].im;
                r1 = v0[nx].re; i1 = v0[nx].im;
                
                r0 = r1 + r2; i0 = i1 + i2;
                r2 -= r1; i2 -= i1;

                i3 = v1[nx].re; r3 = v1[nx].im;
                i4 = v1[0].re; r4 = v1[0].im;

                r1 = i4 + i3; i1 = r4 + r3;
                r3 = r4 - r3; i3 = i3 - i4;

                v0[0].re = (float)(r0 + r1); v0[0].im = (float)(i0 + i1);
                v1[0].re = (float)(r0 - r1); v1[0].im = (float)(i0 - i1);
                v0[nx].re = (float)(r2 + r3); v0[nx].im = (float)(i2 + i3);
                v1[nx].re = (float)(r2 - r3); v1[nx].im = (float)(i2 - i3);

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

                    v0[0].re = (float)(r0 + r1); v0[0].im = (float)(i0 + i1);
                    v1[0].re = (float)(r0 - r1); v1[0].im = (float)(i0 - i1);
                    v0[nx].re = (float)(r2 + r3); v0[nx].im = (float)(i2 + i3);
                    v1[nx].re = (float)(r2 - r3); v1[nx].im = (float)(i2 - i3);
                }
            }
        }

        for( ; n < factors[0]; )
        {
            // do the remaining radix-2 transform
            nx = n;
            n *= 2;
            dw0 /= 2;

            for( i = 0; i < n0; i += n )
            {
                CvComplex32f* v = dst + i;
                double r0 = v[0].re + v[nx].re;
                double i0 = v[0].im + v[nx].im;
                double r1 = v[0].re - v[nx].re;
                double i1 = v[0].im - v[nx].im;
                v[0].re = (float)r0; v[0].im = (float)i0;
                v[nx].re = (float)r1; v[nx].im = (float)i1;

                for( j = 1, dw = dw0; j < nx; j++, dw += dw0 )
                {
                    v = dst + i + j;
                    r1 = v[nx].re*wave[dw].re - v[nx].im*wave[dw].im;
                    i1 = v[nx].im*wave[dw].re + v[nx].re*wave[dw].im;
                    r0 = v[0].re; i0 = v[0].im;

                    v[0].re = (float)(r0 + r1); v[0].im = (float)(i0 + i1);
                    v[nx].re = (float)(r0 - r1); v[nx].im = (float)(i0 - i1);
                }
            }
        }
    }

    // 2. all the other transforms
    for( f_idx = (factors[0]&1) ? 0 : 1; f_idx < nf; f_idx++ )
    {
        int factor = factors[f_idx];
        nx = n;
        n *= factor;
        dw0 /= factor;

        if( factor == 3 )
        {
            // radix-3
            for( i = 0; i < n0; i += n )
            {
                CvComplex32f* v = dst + i;

                double r1 = v[nx].re + v[nx*2].re;
                double i1 = v[nx].im + v[nx*2].im;
                double r0 = v[0].re;
                double i0 = v[0].im;
                double r2 = icv_sin_120*(v[nx].im - v[nx*2].im);
                double i2 = icv_sin_120*(v[nx*2].re - v[nx].re);
                v[0].re = (float)(r0 + r1); v[0].im = (float)(i0 + i1);
                r0 -= 0.5*r1; i0 -= 0.5*i1;
                v[nx].re = (float)(r0 + r2); v[nx].im = (float)(i0 + i2);
                v[nx*2].re = (float)(r0 - r2); v[nx*2].im = (float)(i0 - i2);

                for( j = 1, dw = dw0; j < nx; j++, dw += dw0 )
                {
                    v = dst + i + j;
                    r0 = v[nx].re*wave[dw].re - v[nx].im*wave[dw].im;
                    i0 = v[nx].re*wave[dw].im + v[nx].im*wave[dw].re;
                    i2 = v[nx*2].re*wave[dw*2].re - v[nx*2].im*wave[dw*2].im;
                    r2 = v[nx*2].re*wave[dw*2].im + v[nx*2].im*wave[dw*2].re;
                    r1 = r0 + i2; i1 = i0 + r2;
                    
                    r2 = icv_sin_120*(i0 - r2); i2 = icv_sin_120*(i2 - r0);
                    r0 = v[0].re; i0 = v[0].im;
                    v[0].re = (float)(r0 + r1); v[0].im = (float)(i0 + i1);
                    r0 -= 0.5*r1; i0 -= 0.5*i1;
                    v[nx].re = (float)(r0 + r2); v[nx].im = (float)(i0 + i2);
                    v[nx*2].re = (float)(r0 - r2); v[nx*2].im = (float)(i0 - i2);
                }
            }
        }
        else if( factor == 5 )
        {
            // radix-5
            for( i = 0; i < n0; i += n )
            {
                for( j = 0, dw = 0; j < nx; j++, dw += dw0 )
                {
                    CvComplex32f* v0 = dst + i + j;
                    CvComplex32f* v1 = v0 + nx*2;
                    CvComplex32f* v2 = v1 + nx*2;

                    double r0, i0, r1, i1, r2, i2, r3, i3, r4, i4, r5, i5;

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

                    v0[0].re = (float)(r0 + r5); v0[0].im = (float)(i0 + i5);

                    r0 -= 0.25*r5; i0 -= 0.25*i5;
                    r1 = icv_fft5_2*(r1 - r2); i1 = icv_fft5_2*(i1 - i2);
                    r2 = -icv_fft5_3*(i3 + i4); i2 = icv_fft5_3*(r3 + r4);

                    i3 *= -icv_fft5_5; r3 *= icv_fft5_5;
                    i4 *= -icv_fft5_4; r4 *= icv_fft5_4;

                    r5 = r2 + i3; i5 = i2 + r3;
                    r2 -= i4; i2 -= r4;
                    
                    r3 = r0 + r1; i3 = i0 + i1;
                    r0 -= r1; i0 -= i1;

                    v0[nx].re = (float)(r3 + r2); v0[nx].im = (float)(i3 + i2);
                    v2[0].re = (float)(r3 - r2); v2[0].im = (float)(i3 - i2);

                    v1[0].re = (float)(r0 + r5); v1[0].im = (float)(i0 + i5);
                    v1[nx].re = (float)(r0 - r5); v1[nx].im = (float)(i0 - i5);
                }
            }
        }
        else
        {
            // radix-"factor" - an odd number
            int p, q, factor2 = (factor - 1)/2;
            int d, dd, dw_f = tab_size/factor;
            CvComplex32f* a = buf;
            CvComplex32f* b = buf + factor2;

            for( i = 0; i < n0; i += n )
            {
                for( j = 0, dw = 0; j < nx; j++, dw += dw0 )
                {
                    CvComplex32f* v = dst + i + j;
                    CvComplex32f v_0 = v[0];
                    CvComplex64f vn_0;
                    CvComplex_set(vn_0,v_0.re,v_0.im);

                    if( j == 0 )
                    {
                        for( p = 1, k = nx; p <= factor2; p++, k += nx )
                        {
                            double r0 = v[k].re + v[n-k].re;
                            double i0 = v[k].im - v[n-k].im;
                            double r1 = v[k].re - v[n-k].re;
                            double i1 = v[k].im + v[n-k].im;

                            vn_0.re += r0; vn_0.im += i1;
                            a[p-1].re = (float)r0; a[p-1].im = (float)i0;
                            b[p-1].re = (float)r1; b[p-1].im = (float)i1;
                        }
                    }
                    else
                    {
                        const CvComplex32f* wave_ = wave + dw*factor;
                        d = dw;

                        for( p = 1, k = nx; p <= factor2; p++, k += nx, d += dw )
                        {
                            double r2 = v[k].re*wave[d].re - v[k].im*wave[d].im;
                            double i2 = v[k].re*wave[d].im + v[k].im*wave[d].re;

                            double r1 = v[n-k].re*wave_[-d].re - v[n-k].im*wave_[-d].im;
                            double i1 = v[n-k].re*wave_[-d].im + v[n-k].im*wave_[-d].re;
                    
                            double r0 = r2 + r1;
                            double i0 = i2 - i1;
                            r1 = r2 - r1;
                            i1 = i2 + i1;

                            vn_0.re += r0; vn_0.im += i1;
                            a[p-1].re = (float)r0; a[p-1].im = (float)i0;
                            b[p-1].re = (float)r1; b[p-1].im = (float)i1;
                        }
                    }

                    v[0].re = (float)vn_0.re;
                    v[0].im = (float)vn_0.im;

                    for( p = 1, k = nx; p <= factor2; p++, k += nx )
                    {
                        CvComplex64f s0, s1 = s0;
                        d = dd = dw_f*p;
                        CvComplex_set(s0,v_0.re,v_0.im);

                        for( q = 0; q < factor2; q++ )
                        {
                            double r0 = wave[d].re * a[q].re;
                            double i0 = wave[d].im * a[q].im;
                            double r1 = wave[d].re * b[q].im;
                            double i1 = wave[d].im * b[q].re;
            
                            s1.re += r0 + i0; s0.re += r0 - i0;
                            s1.im += r1 - i1; s0.im += r1 + i1;

                            d += dd;
                            d -= -(d >= tab_size) & tab_size;
                        }

                        v[k].re = (float)s0.re;
                        v[k].im = (float)s0.im;
                        v[n-k].re = (float)s1.re;
                        v[n-k].im = (float)s1.im;
                    }
                }
            }
        }
    }

    if( fabs(scale - 1.) > DBL_EPSILON )
    {
        double re_scale = scale, im_scale = scale;
        if( inv )
            im_scale = -im_scale;

        for( i = 0; i < n0; i++ )
        {
            double t0 = dst[i].re*re_scale;
            double t1 = dst[i].im*im_scale;
            dst[i].re = (float)t0;
            dst[i].im = (float)t1;
        }
    }
    else if( inv )
    {
        for( i = 0; i <= n0 - 2; i += 2 )
        {
            double t0 = -dst[i].im;
            double t1 = -dst[i+1].im;
            dst[i].im = (float)t0;
            dst[i+1].im = (float)t1;
        }

        if( i < n0 )
            dst[n0-1].im = -dst[n0-1].im;
    }

    return CC_OK;
}

static void
icvCopyColumn( const uchar* _src, int src_step,
               uchar* _dst, int dst_step,
               int len, int elem_size )
{
    int i, t0, t1;
    const int* src = (const int*)_src;
    int* dst = (int*)_dst;
    src_step /= sizeof(src[0]);
    dst_step /= sizeof(dst[0]);

    if( elem_size == sizeof(int) )
    {
        for( i = 0; i < len; i++, src += src_step, dst += dst_step )
            dst[0] = src[0];
    }
    else if( elem_size == sizeof(int)*2 )
    {
        for( i = 0; i < len; i++, src += src_step, dst += dst_step )
        {
            t0 = src[0]; t1 = src[1];
            dst[0] = t0; dst[1] = t1;
        }
    }
    else if( elem_size == sizeof(int)*4 )
    {
        for( i = 0; i < len; i++, src += src_step, dst += dst_step )
        {
            t0 = src[0]; t1 = src[1];
            dst[0] = t0; dst[1] = t1;
            t0 = src[2]; t1 = src[3];
            dst[2] = t0; dst[3] = t1;
        }
    }
}


static void icvCopyFrom2Columns( const uchar* _src, int src_step,
                     uchar* _dst0, uchar* _dst1,
                     int len, int elem_size )
{
    int i, t0, t1;
    const int* src = (const int*)_src;
    int* dst0 = (int*)_dst0;
    int* dst1 = (int*)_dst1;
    src_step /= sizeof(src[0]);

    if( elem_size == sizeof(int) )
    {
        for( i = 0; i < len; i++, src += src_step )
        {
            t0 = src[0]; t1 = src[1];
            dst0[i] = t0; dst1[i] = t1;
        }
    }
    else if( elem_size == sizeof(int)*2 )
    {
        for( i = 0; i < len*2; i += 2, src += src_step )
        {
            t0 = src[0]; t1 = src[1];
            dst0[i] = t0; dst0[i+1] = t1;
            t0 = src[2]; t1 = src[3];
            dst1[i] = t0; dst1[i+1] = t1;
        }
    }
    else if( elem_size == sizeof(int)*4 )
    {
        for( i = 0; i < len*4; i += 4, src += src_step )
        {
            t0 = src[0]; t1 = src[1];
            dst0[i] = t0; dst0[i+1] = t1;
            t0 = src[2]; t1 = src[3];
            dst0[i+2] = t0; dst0[i+3] = t1;
            t0 = src[4]; t1 = src[5];
            dst1[i] = t0; dst1[i+1] = t1;
            t0 = src[6]; t1 = src[7];
            dst1[i+2] = t0; dst1[i+3] = t1;
        }
    }
}


static void
icvCopyTo2Columns( const uchar* _src0, const uchar* _src1,
                   uchar* _dst, int dst_step,
                   int len, int elem_size )
{
    int i, t0, t1;
    const int* src0 = (const int*)_src0;
    const int* src1 = (const int*)_src1;
    int* dst = (int*)_dst;
    dst_step /= sizeof(dst[0]);

    if( elem_size == sizeof(int) )
    {
        for( i = 0; i < len; i++, dst += dst_step )
        {
            t0 = src0[i]; t1 = src1[i];
            dst[0] = t0; dst[1] = t1;
        }
    }
    else if( elem_size == sizeof(int)*2 )
    {
        for( i = 0; i < len*2; i += 2, dst += dst_step )
        {
            t0 = src0[i]; t1 = src0[i+1];
            dst[0] = t0; dst[1] = t1;
            t0 = src1[i]; t1 = src1[i+1];
            dst[2] = t0; dst[3] = t1;
        }
    }
    else if( elem_size == sizeof(int)*4 )
    {
        for( i = 0; i < len*4; i += 4, dst += dst_step )
        {
            t0 = src0[i]; t1 = src0[i+1];
            dst[0] = t0; dst[1] = t1;
            t0 = src0[i+2]; t1 = src0[i+3];
            dst[2] = t0; dst[3] = t1;
            t0 = src1[i]; t1 = src1[i+1];
            dst[4] = t0; dst[5] = t1;
            t0 = src1[i+2]; t1 = src1[i+3];
            dst[6] = t0; dst[7] = t1;
        }
    }
}


static void
icvExpandCCS( uchar* _ptr, int len, int elem_size )
{
    int i;
    _ptr -= elem_size;
    memcpy( _ptr, _ptr + elem_size, elem_size );
    memset( _ptr + elem_size, 0, elem_size );
    if( (len & 1) == 0 )
        memset( _ptr + (len+1)*elem_size, 0, elem_size );

    if( elem_size == sizeof(float) )
    {
        CvComplex32f* ptr = (CvComplex32f*)_ptr;

        for( i = 1; i < (len+1)/2; i++ )
        {
            CvComplex32f t;
            t.re = ptr[i].re;
            t.im = -ptr[i].im;
            ptr[len-i] = t;
        }
    }
    else
    {
        CvComplex64f* ptr = (CvComplex64f*)_ptr;

        for( i = 1; i < (len+1)/2; i++ )
        {
            CvComplex64f t;
            t.re = ptr[i].re;
            t.im = -ptr[i].im;
            ptr[len-i] = t;
        }
    }
}


static void icvDCTInit( int n, int elem_size, void* _wave, int inv )
{
    static const double icvDctScale[] =
    {
    0.707106781186547570, 0.500000000000000000, 0.353553390593273790,
    0.250000000000000000, 0.176776695296636890, 0.125000000000000000,
    0.088388347648318447, 0.062500000000000000, 0.044194173824159223,
    0.031250000000000000, 0.022097086912079612, 0.015625000000000000,
    0.011048543456039806, 0.007812500000000000, 0.005524271728019903,
    0.003906250000000000, 0.002762135864009952, 0.001953125000000000,
    0.001381067932004976, 0.000976562500000000, 0.000690533966002488,
    0.000488281250000000, 0.000345266983001244, 0.000244140625000000,
    0.000172633491500622, 0.000122070312500000, 0.000086316745750311,
    0.000061035156250000, 0.000043158372875155, 0.000030517578125000
    };

    int i;
    CvComplex64f w, w1;
    double t, scale;
    
    if( n == 1 )
        return;

    assert( (n&1) == 0 );

    if( (n & (n - 1)) == 0 )
    {
        int m = icvlog2(n);
        scale = (!inv ? 2 : 1)*icvDctScale[m];
        w1.re = icvDxtTab[m+2][0];
        w1.im = -icvDxtTab[m+2][1];
    }
    else
    {
        t = 1./(2*n);
        scale = (!inv ? 2 : 1)*sqrt(t);
        w1.im = sin(-CC_PI*t);
        w1.re = sqrt(1. - w1.im*w1.im);
    }
    n >>= 1;
    
    if( elem_size == sizeof(CvComplex64f) )
    {
        CvComplex64f* wave = (CvComplex64f*)_wave;

        w.re = scale;
        w.im = 0.;

        for( i = 0; i <= n; i++ )
        {
            wave[i] = w;
            t = w.re*w1.re - w.im*w1.im;
            w.im = w.re*w1.im + w.im*w1.re;
            w.re = t;
        }
    }
    else
    {
        CvComplex32f* wave = (CvComplex32f*)_wave;
        assert( elem_size == sizeof(CvComplex32f) );
        
        w.re = (float)scale;
        w.im = 0.f;

        for( i = 0; i <= n; i++ )
        {
            wave[i].re = (float)w.re;
            wave[i].im = (float)w.im;
            t = w.re*w1.re - w.im*w1.im;
            w.im = w.re*w1.im + w.im*w1.re;
            w.re = t;
        }
    }
}

#define datatype f64
#define icvDFTc icvDFT_64fc
#define icvRealDFT icvRealDFT_64f
#define icvCCSIDFT icvCCSIDFT_64f
#define CvComplex CvComplex64f
#define cvDFT cvDFT64f
#define cvMulSpectrums cvMulSpectrums64f
#define icvDCT_fwd icvDCT_fwd_64f
#define icvDCT_inv icvDCT_inv_64f
#define cvDCT cvDCT64f
#include "impl/fft_impl.inl"

#define datatype f32
#define icvDFTc icvDFT_32fc
#define icvRealDFT icvRealDFT_32f
#define icvCCSIDFT icvCCSIDFT_32f
#define CvComplex CvComplex32f
#define cvMulSpectrums cvMulSpectrums32f
#define cvDFT cvDFT32f
#define icvDCT_fwd icvDCT_fwd_32f
#define icvDCT_inv icvDCT_inv_32f
#define cvDCT cvDCT32f
#include "impl/fft_impl.inl"



static const int icvOptimalDFTSize[] = {
1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 16, 18, 20, 24, 25, 27, 30, 32, 36, 40, 45, 48, 
50, 54, 60, 64, 72, 75, 80, 81, 90, 96, 100, 108, 120, 125, 128, 135, 144, 150, 160, 
162, 180, 192, 200, 216, 225, 240, 243, 250, 256, 270, 288, 300, 320, 324, 360, 375, 
384, 400, 405, 432, 450, 480, 486, 500, 512, 540, 576, 600, 625, 640, 648, 675, 720, 
729, 750, 768, 800, 810, 864, 900, 960, 972, 1000, 1024, 1080, 1125, 1152, 1200, 
1215, 1250, 1280, 1296, 1350, 1440, 1458, 1500, 1536, 1600, 1620, 1728, 1800, 1875, 
1920, 1944, 2000, 2025, 2048, 2160, 2187, 2250, 2304, 2400, 2430, 2500, 2560, 2592, 
2700, 2880, 2916, 3000, 3072, 3125, 3200, 3240, 3375, 3456, 3600, 3645, 3750, 3840, 
3888, 4000, 4050, 4096, 4320, 4374, 4500, 4608, 4800, 4860, 5000, 5120, 5184, 5400, 
5625, 5760, 5832, 6000, 6075, 6144, 6250, 6400, 6480, 6561, 6750, 6912, 7200, 7290, 
7500, 7680, 7776, 8000, 8100, 8192, 8640, 8748, 9000, 9216, 9375, 9600, 9720, 10000, 
10125, 10240, 10368, 10800, 10935, 11250, 11520, 11664, 12000, 12150, 12288, 12500, 
12800, 12960, 13122, 13500, 13824, 14400, 14580, 15000, 15360, 15552, 15625, 16000, 
16200, 16384, 16875, 17280, 17496, 18000, 18225, 18432, 18750, 19200, 19440, 19683, 
20000, 20250, 20480, 20736, 21600, 21870, 22500, 23040, 23328, 24000, 24300, 24576, 
25000, 25600, 25920, 26244, 27000, 27648, 28125, 28800, 29160, 30000, 30375, 30720, 
31104, 31250, 32000, 32400, 32768, 32805, 33750, 34560, 34992, 36000, 36450, 36864, 
37500, 38400, 38880, 39366, 40000, 40500, 40960, 41472, 43200, 43740, 45000, 46080, 
46656, 46875, 48000, 48600, 49152, 50000, 50625, 51200, 51840, 52488, 54000, 54675, 
55296, 56250, 57600, 58320, 59049, 60000, 60750, 61440, 62208, 62500, 64000, 64800, 
65536, 65610, 67500, 69120, 69984, 72000, 72900, 73728, 75000, 76800, 77760, 78125, 
78732, 80000, 81000, 81920, 82944, 84375, 86400, 87480, 90000, 91125, 92160, 93312, 
93750, 96000, 97200, 98304, 98415, 100000, 101250, 102400, 103680, 104976, 108000, 
109350, 110592, 112500, 115200, 116640, 118098, 120000, 121500, 122880, 124416, 125000, 
128000, 129600, 131072, 131220, 135000, 138240, 139968, 140625, 144000, 145800, 147456, 
150000, 151875, 153600, 155520, 156250, 157464, 160000, 162000, 163840, 164025, 165888, 
168750, 172800, 174960, 177147, 180000, 182250, 184320, 186624, 187500, 192000, 194400, 
196608, 196830, 200000, 202500, 204800, 207360, 209952, 216000, 218700, 221184, 225000, 
230400, 233280, 234375, 236196, 240000, 243000, 245760, 248832, 250000, 253125, 256000, 
259200, 262144, 262440, 270000, 273375, 276480, 279936, 281250, 288000, 291600, 294912, 
295245, 300000, 303750, 307200, 311040, 312500, 314928, 320000, 324000, 327680, 328050, 
331776, 337500, 345600, 349920, 354294, 360000, 364500, 368640, 373248, 375000, 384000, 
388800, 390625, 393216, 393660, 400000, 405000, 409600, 414720, 419904, 421875, 432000, 
437400, 442368, 450000, 455625, 460800, 466560, 468750, 472392, 480000, 486000, 491520, 
492075, 497664, 500000, 506250, 512000, 518400, 524288, 524880, 531441, 540000, 546750, 
552960, 559872, 562500, 576000, 583200, 589824, 590490, 600000, 607500, 614400, 622080, 
625000, 629856, 640000, 648000, 655360, 656100, 663552, 675000, 691200, 699840, 703125, 
708588, 720000, 729000, 737280, 746496, 750000, 759375, 768000, 777600, 781250, 786432, 
787320, 800000, 810000, 819200, 820125, 829440, 839808, 843750, 864000, 874800, 884736, 
885735, 900000, 911250, 921600, 933120, 937500, 944784, 960000, 972000, 983040, 984150, 
995328, 1000000, 1012500, 1024000, 1036800, 1048576, 1049760, 1062882, 1080000, 1093500, 
1105920, 1119744, 1125000, 1152000, 1166400, 1171875, 1179648, 1180980, 1200000, 
1215000, 1228800, 1244160, 1250000, 1259712, 1265625, 1280000, 1296000, 1310720, 
1312200, 1327104, 1350000, 1366875, 1382400, 1399680, 1406250, 1417176, 1440000, 
1458000, 1474560, 1476225, 1492992, 1500000, 1518750, 1536000, 1555200, 1562500, 
1572864, 1574640, 1594323, 1600000, 1620000, 1638400, 1640250, 1658880, 1679616, 
1687500, 1728000, 1749600, 1769472, 1771470, 1800000, 1822500, 1843200, 1866240, 
1875000, 1889568, 1920000, 1944000, 1953125, 1966080, 1968300, 1990656, 2000000, 
2025000, 2048000, 2073600, 2097152, 2099520, 2109375, 2125764, 2160000, 2187000, 
2211840, 2239488, 2250000, 2278125, 2304000, 2332800, 2343750, 2359296, 2361960, 
2400000, 2430000, 2457600, 2460375, 2488320, 2500000, 2519424, 2531250, 2560000, 
2592000, 2621440, 2624400, 2654208, 2657205, 2700000, 2733750, 2764800, 2799360, 
2812500, 2834352, 2880000, 2916000, 2949120, 2952450, 2985984, 3000000, 3037500, 
3072000, 3110400, 3125000, 3145728, 3149280, 3188646, 3200000, 3240000, 3276800, 
3280500, 3317760, 3359232, 3375000, 3456000, 3499200, 3515625, 3538944, 3542940, 
3600000, 3645000, 3686400, 3732480, 3750000, 3779136, 3796875, 3840000, 3888000, 
3906250, 3932160, 3936600, 3981312, 4000000, 4050000, 4096000, 4100625, 4147200, 
4194304, 4199040, 4218750, 4251528, 4320000, 4374000, 4423680, 4428675, 4478976, 
4500000, 4556250, 4608000, 4665600, 4687500, 4718592, 4723920, 4782969, 4800000, 
4860000, 4915200, 4920750, 4976640, 5000000, 5038848, 5062500, 5120000, 5184000, 
5242880, 5248800, 5308416, 5314410, 5400000, 5467500, 5529600, 5598720, 5625000, 
5668704, 5760000, 5832000, 5859375, 5898240, 5904900, 5971968, 6000000, 6075000, 
6144000, 6220800, 6250000, 6291456, 6298560, 6328125, 6377292, 6400000, 6480000, 
6553600, 6561000, 6635520, 6718464, 6750000, 6834375, 6912000, 6998400, 7031250, 
7077888, 7085880, 7200000, 7290000, 7372800, 7381125, 7464960, 7500000, 7558272, 
7593750, 7680000, 7776000, 7812500, 7864320, 7873200, 7962624, 7971615, 8000000, 
8100000, 8192000, 8201250, 8294400, 8388608, 8398080, 8437500, 8503056, 8640000, 
8748000, 8847360, 8857350, 8957952, 9000000, 9112500, 9216000, 9331200, 9375000, 
9437184, 9447840, 9565938, 9600000, 9720000, 9765625, 9830400, 9841500, 9953280, 
10000000, 10077696, 10125000, 10240000, 10368000, 10485760, 10497600, 10546875, 10616832, 
10628820, 10800000, 10935000, 11059200, 11197440, 11250000, 11337408, 11390625, 11520000, 
11664000, 11718750, 11796480, 11809800, 11943936, 12000000, 12150000, 12288000, 12301875, 
12441600, 12500000, 12582912, 12597120, 12656250, 12754584, 12800000, 12960000, 13107200, 
13122000, 13271040, 13286025, 13436928, 13500000, 13668750, 13824000, 13996800, 14062500, 
14155776, 14171760, 14400000, 14580000, 14745600, 14762250, 14929920, 15000000, 15116544, 
15187500, 15360000, 15552000, 15625000, 15728640, 15746400, 15925248, 15943230, 16000000, 
16200000, 16384000, 16402500, 16588800, 16777216, 16796160, 16875000, 17006112, 17280000, 
17496000, 17578125, 17694720, 17714700, 17915904, 18000000, 18225000, 18432000, 18662400, 
18750000, 18874368, 18895680, 18984375, 19131876, 19200000, 19440000, 19531250, 19660800, 
19683000, 19906560, 20000000, 20155392, 20250000, 20480000, 20503125, 20736000, 20971520, 
20995200, 21093750, 21233664, 21257640, 21600000, 21870000, 22118400, 22143375, 22394880, 
22500000, 22674816, 22781250, 23040000, 23328000, 23437500, 23592960, 23619600, 23887872, 
23914845, 24000000, 24300000, 24576000, 24603750, 24883200, 25000000, 25165824, 25194240, 
25312500, 25509168, 25600000, 25920000, 26214400, 26244000, 26542080, 26572050, 26873856, 
27000000, 27337500, 27648000, 27993600, 28125000, 28311552, 28343520, 28800000, 29160000, 
29296875, 29491200, 29524500, 29859840, 30000000, 30233088, 30375000, 30720000, 31104000, 
31250000, 31457280, 31492800, 31640625, 31850496, 31886460, 32000000, 32400000, 32768000, 
32805000, 33177600, 33554432, 33592320, 33750000, 34012224, 34171875, 34560000, 34992000, 
35156250, 35389440, 35429400, 35831808, 36000000, 36450000, 36864000, 36905625, 37324800, 
37500000, 37748736, 37791360, 37968750, 38263752, 38400000, 38880000, 39062500, 39321600, 
39366000, 39813120, 39858075, 40000000, 40310784, 40500000, 40960000, 41006250, 41472000, 
41943040, 41990400, 42187500, 42467328, 42515280, 43200000, 43740000, 44236800, 44286750, 
44789760, 45000000, 45349632, 45562500, 46080000, 46656000, 46875000, 47185920, 47239200, 
47775744, 47829690, 48000000, 48600000, 48828125, 49152000, 49207500, 49766400, 50000000, 
50331648, 50388480, 50625000, 51018336, 51200000, 51840000, 52428800, 52488000, 52734375, 
53084160, 53144100, 53747712, 54000000, 54675000, 55296000, 55987200, 56250000, 56623104, 
56687040, 56953125, 57600000, 58320000, 58593750, 58982400, 59049000, 59719680, 60000000, 
60466176, 60750000, 61440000, 61509375, 62208000, 62500000, 62914560, 62985600, 63281250, 
63700992, 63772920, 64000000, 64800000, 65536000, 65610000, 66355200, 66430125, 67108864, 
67184640, 67500000, 68024448, 68343750, 69120000, 69984000, 70312500, 70778880, 70858800, 
71663616, 72000000, 72900000, 73728000, 73811250, 74649600, 75000000, 75497472, 75582720, 
75937500, 76527504, 76800000, 77760000, 78125000, 78643200, 78732000, 79626240, 79716150, 
80000000, 80621568, 81000000, 81920000, 82012500, 82944000, 83886080, 83980800, 84375000, 
84934656, 85030560, 86400000, 87480000, 87890625, 88473600, 88573500, 89579520, 90000000, 
90699264, 91125000, 92160000, 93312000, 93750000, 94371840, 94478400, 94921875, 95551488, 
95659380, 96000000, 97200000, 97656250, 98304000, 98415000, 99532800, 100000000, 
100663296, 100776960, 101250000, 102036672, 102400000, 102515625, 103680000, 104857600, 
104976000, 105468750, 106168320, 106288200, 107495424, 108000000, 109350000, 110592000, 
110716875, 111974400, 112500000, 113246208, 113374080, 113906250, 115200000, 116640000, 
117187500, 117964800, 118098000, 119439360, 119574225, 120000000, 120932352, 121500000, 
122880000, 123018750, 124416000, 125000000, 125829120, 125971200, 126562500, 127401984, 
127545840, 128000000, 129600000, 131072000, 131220000, 132710400, 132860250, 134217728, 
134369280, 135000000, 136048896, 136687500, 138240000, 139968000, 140625000, 141557760, 
141717600, 143327232, 144000000, 145800000, 146484375, 147456000, 147622500, 149299200, 
150000000, 150994944, 151165440, 151875000, 153055008, 153600000, 155520000, 156250000, 
157286400, 157464000, 158203125, 159252480, 159432300, 160000000, 161243136, 162000000, 
163840000, 164025000, 165888000, 167772160, 167961600, 168750000, 169869312, 170061120, 
170859375, 172800000, 174960000, 175781250, 176947200, 177147000, 179159040, 180000000, 
181398528, 182250000, 184320000, 184528125, 186624000, 187500000, 188743680, 188956800, 
189843750, 191102976, 191318760, 192000000, 194400000, 195312500, 196608000, 196830000, 
199065600, 199290375, 200000000, 201326592, 201553920, 202500000, 204073344, 204800000, 
205031250, 207360000, 209715200, 209952000, 210937500, 212336640, 212576400, 214990848, 
216000000, 218700000, 221184000, 221433750, 223948800, 225000000, 226492416, 226748160, 
227812500, 230400000, 233280000, 234375000, 235929600, 236196000, 238878720, 239148450, 
240000000, 241864704, 243000000, 244140625, 245760000, 246037500, 248832000, 250000000, 
251658240, 251942400, 253125000, 254803968, 255091680, 256000000, 259200000, 262144000, 
262440000, 263671875, 265420800, 265720500, 268435456, 268738560, 270000000, 272097792, 
273375000, 276480000, 279936000, 281250000, 283115520, 283435200, 284765625, 286654464, 
288000000, 291600000, 292968750, 294912000, 295245000, 298598400, 300000000, 301989888, 
302330880, 303750000, 306110016, 307200000, 307546875, 311040000, 312500000, 314572800, 
314928000, 316406250, 318504960, 318864600, 320000000, 322486272, 324000000, 327680000, 
328050000, 331776000, 332150625, 335544320, 335923200, 337500000, 339738624, 340122240, 
341718750, 345600000, 349920000, 351562500, 353894400, 354294000, 358318080, 360000000, 
362797056, 364500000, 368640000, 369056250, 373248000, 375000000, 377487360, 377913600, 
379687500, 382205952, 382637520, 384000000, 388800000, 390625000, 393216000, 393660000, 
398131200, 398580750, 400000000, 402653184, 403107840, 405000000, 408146688, 409600000, 
410062500, 414720000, 419430400, 419904000, 421875000, 424673280, 425152800, 429981696, 
432000000, 437400000, 439453125, 442368000, 442867500, 447897600, 450000000, 452984832, 
453496320, 455625000, 460800000, 466560000, 468750000, 471859200, 472392000, 474609375, 
477757440, 478296900, 480000000, 483729408, 486000000, 488281250, 491520000, 492075000, 
497664000, 500000000, 503316480, 503884800, 506250000, 509607936, 510183360, 512000000, 
512578125, 518400000, 524288000, 524880000, 527343750, 530841600, 531441000, 536870912, 
537477120, 540000000, 544195584, 546750000, 552960000, 553584375, 559872000, 562500000, 
566231040, 566870400, 569531250, 573308928, 576000000, 583200000, 585937500, 589824000, 
590490000, 597196800, 597871125, 600000000, 603979776, 604661760, 607500000, 612220032, 
614400000, 615093750, 622080000, 625000000, 629145600, 629856000, 632812500, 637009920, 
637729200, 640000000, 644972544, 648000000, 655360000, 656100000, 663552000, 664301250, 
671088640, 671846400, 675000000, 679477248, 680244480, 683437500, 691200000, 699840000, 
703125000, 707788800, 708588000, 716636160, 720000000, 725594112, 729000000, 732421875, 
737280000, 738112500, 746496000, 750000000, 754974720, 755827200, 759375000, 764411904, 
765275040, 768000000, 777600000, 781250000, 786432000, 787320000, 791015625, 796262400, 
797161500, 800000000, 805306368, 806215680, 810000000, 816293376, 819200000, 820125000, 
829440000, 838860800, 839808000, 843750000, 849346560, 850305600, 854296875, 859963392, 
864000000, 874800000, 878906250, 884736000, 885735000, 895795200, 900000000, 905969664, 
906992640, 911250000, 921600000, 922640625, 933120000, 937500000, 943718400, 944784000, 
949218750, 955514880, 956593800, 960000000, 967458816, 972000000, 976562500, 983040000, 
984150000, 995328000, 996451875, 1000000000, 1006632960, 1007769600, 1012500000, 
1019215872, 1020366720, 1024000000, 1025156250, 1036800000, 1048576000, 1049760000, 
1054687500, 1061683200, 1062882000, 1073741824, 1074954240, 1080000000, 1088391168, 
1093500000, 1105920000, 1107168750, 1119744000, 1125000000, 1132462080, 1133740800, 
1139062500, 1146617856, 1152000000, 1166400000, 1171875000, 1179648000, 1180980000, 
1194393600, 1195742250, 1200000000, 1207959552, 1209323520, 1215000000, 1220703125, 
1224440064, 1228800000, 1230187500, 1244160000, 1250000000, 1258291200, 1259712000, 
1265625000, 1274019840, 1275458400, 1280000000, 1289945088, 1296000000, 1310720000, 
1312200000, 1318359375, 1327104000, 1328602500, 1342177280, 1343692800, 1350000000, 
1358954496, 1360488960, 1366875000, 1382400000, 1399680000, 1406250000, 1415577600, 
1417176000, 1423828125, 1433272320, 1440000000, 1451188224, 1458000000, 1464843750, 
1474560000, 1476225000, 1492992000, 1500000000, 1509949440, 1511654400, 1518750000, 
1528823808, 1530550080, 1536000000, 1537734375, 1555200000, 1562500000, 1572864000, 
1574640000, 1582031250, 1592524800, 1594323000, 1600000000, 1610612736, 1612431360, 
1620000000, 1632586752, 1638400000, 1640250000, 1658880000, 1660753125, 1677721600, 
1679616000, 1687500000, 1698693120, 1700611200, 1708593750, 1719926784, 1728000000, 
1749600000, 1757812500, 1769472000, 1771470000, 1791590400, 1800000000, 1811939328, 
1813985280, 1822500000, 1843200000, 1845281250, 1866240000, 1875000000, 1887436800, 
1889568000, 1898437500, 1911029760, 1913187600, 1920000000, 1934917632, 1944000000, 
1953125000, 1966080000, 1968300000, 1990656000, 1992903750, 2000000000, 2013265920, 
2015539200, 2025000000, 2038431744, 2040733440, 2048000000, 2050312500, 2073600000, 
2097152000, 2099520000, 2109375000, 2123366400, 2125764000
};


int cvGetOptimalDFTSize( int size0 )
{
    int a = 0, b = sizeof(icvOptimalDFTSize)/sizeof(icvOptimalDFTSize[0]) - 1;
    if( (unsigned)size0 >= (unsigned)icvOptimalDFTSize[b] )
        return -1;

    while( a < b )
    {
        int c = (a + b) >> 1;
        if( size0 <= icvOptimalDFTSize[c] )
            b = c;
        else
            a = c+1;
    }

    return icvOptimalDFTSize[b];
}

// flags CC_DXT_FORWARD CC_DXT_INVERSE
int imfft2(const img_t* src, int flags, img_t* dst)
{
  //实部、虚部
  ASSERT(src->c==sizeof(double)*2);
  imsetsize(dst, src->h, src->w, sizeof(double)*2, 1);
  // Application of the forward Fourier transform
  cvDFT64f(src->h, src->w, (double*)(src->tt.data), src->s, 2, dst->h, dst->w, (double*)(dst->tt.data), dst->s, 2, flags, 0);
  return 0;
}
int imfft2_real_cn(const img_t*src, int flags, img_t* dst) {
  int i, j, k, cn = src->c/sizeof(double);
  img_t im[2] = {0};
  img_t* im2 = im+1;
  int out_cn = (CC_DXT_ABS&flags) ? 1 : 2;
  imsetsize(im, src->h, src->w, sizeof(double)*2, 1);
  imsetsize(dst, src->h, src->w, sizeof(double)*out_cn*cn, 1);
  for (k=0; k<cn; ++k) {
    for (i=0; i<src->h; ++i) {
      const double* src1 = (double*)(src->tt.data + src->s*i);
      double* im1 = (double*)(im->tt.data + im->s*i);
      for (j=0; j<src->w; ++j) {
        im1[j*2+0] = src1[j*cn + k];
        im1[j*2+1] = 0;
      }
    }
  
    imfft2(im, flags, im2);
    //imshowmat_f8_cn(im2, 0); cvWaitKey(-1);
    for (i=0; i<src->h; ++i) {
      double* dst1 = ((double*)(dst->tt.data + dst->s*i) + k*out_cn);
      double* im21 = (double*)(im2->tt.data + im2->s*i);
      if (CC_DXT_ABSQ&flags) {
        for (j=0; j<src->w; ++j) {
          double a = im21[j*2+0], b = im21[j*2+1];
          dst1[j*cn] = (a*a+b*b);
        }
      } else if (CC_DXT_ABS&flags) {
        for (j=0; j<src->w; ++j) {
          double a = im21[j*2+0], b = im21[j*2+1];
          dst1[j*cn] = sqrt(a*a+b*b);
        }
      } else {
        for (j=0; j<src->w; ++j) {
          dst1[j*cn*2+0] = im21[j*2+0];
          dst1[j*cn*2+1] = im21[j*2+1];
        }
      }
    }
    //imshowmat_f8_cn(dst, 0); cvWaitKey(-1);
  }
  //imshowmat_f8_cn(dst, 0); cvWaitKey(-1);
  imfrees2(im);
  return 0;
}

#if 0

//傅里叶正变换
int u8_c64(const img_t *src, int cn, img_t* cimg)
{
  //实部、虚部
  //2 channels (image_Re, image_Im)
  imsetsize(cimg, src->h, src->w, sizeof(double)*2, 1);
  // Real part conversion from u8 to 64f (double)
  FILLT(src->w*src->h*2, (double*)(cimg->tt.data), 1, 0);
  COPYT(src->w*src->h, (double*)(cimg->tt.data), 2, (const uchar*)(src->tt.data), cn);
  return 0;
}

#undef CC_IMAGE_ELEM
#define CC_IMAGE_ELEM(src, T, j, i)  ((T*)((char*)(src->tt.data)+src->s*(j)))[i]

int mat_shift(int h, int w, const CvComplex64f *A, int al, CvComplex64f* B, int bl) {
  int i, j, cy = h/2, cx = w/2;
  for( j = 0; j < cy; j++ ) {
    for( i = 0; i < cx; i++ ) {
      CvComplex64f t1 = A[(j)*al + i];
      CvComplex64f t2 = A[(j+cy)*al + i];
      CvComplex64f t3 = A[(j)*al + (i+cx)];
      CvComplex64f t4 = A[(j+cy)*al + (i+cx)];

      B[(j)*bl + i] = t4;
      B[(j+cy)*bl + i] = t3;
      B[(j)*bl + (i+cx)] = t2;
      B[(j+cy)*bl + (i+cx)] = t1;
    }
  }
  return 0;
}

int fft2shift(const img_t *src, img_t* dst) {
  int i, j, cy = src->h/2, cx = src->w/2;
  mat_shift(src->h, src->w, (CvComplex64f*)src->tt.data, src->s/sizeof(CvComplex64f), (CvComplex64f*)dst->tt.data, dst->s/sizeof(CvComplex64f));
  return 0;
  for( j = 0; j < cy; j++ ) {
    for( i = 0; i < cx; i++ ) {
      //中心化，将整体份成四块进行对角交换
      CvComplex64f t1 = CC_IMAGE_ELEM( src, CvComplex64f, j   , i   );
      CvComplex64f t2 = CC_IMAGE_ELEM( src, CvComplex64f, j+cy, i   );
      CvComplex64f t3 = CC_IMAGE_ELEM( src, CvComplex64f, j   , i+cx);
      CvComplex64f t4 = CC_IMAGE_ELEM( src, CvComplex64f, j+cy, i+cx);
      
      CC_IMAGE_ELEM( dst, CvComplex64f, j   , i   ) = t4;
      CC_IMAGE_ELEM( dst, CvComplex64f, j+cy, i   ) = t3;
      CC_IMAGE_ELEM( dst, CvComplex64f, j   , i+cx) = t2;
      CC_IMAGE_ELEM( dst, CvComplex64f, j+cy, i+cx) = t1;
    }
  }
  return 0;
}

int c64_u8(img_t *src, int dolog, img_t* dst)
{
  img_t Mag[1] = {0};
  int i, n=src->w*src->h;
  double scale, shift;
  double* s, *m;
  uchar* d;
  double minVal = 10000000, maxVal = -10000000;
  
  imsetsize(Mag, src->h, src->w, sizeof(double)*1, 1);
  imsetsize(dst, src->h, src->w, 1, 1);
  s=(double*)(src->tt.data);
  d=(uchar*)(dst->tt.data);
  m=(double*)(Mag->tt.data);
  //Imaginary part
  //具体原理见冈萨雷斯数字图像处理p123
  // Compute the magnitude of the spectrum Mag = sqrt(Re^2 + Im^2)
  //计算傅里叶谱
  //对数变换以增强灰度级细节(这种变换使以窄带低灰度输入图像值映射
  //一宽带输出值，具体可见冈萨雷斯数字图像处理p62)
  // Compute log(1 + Mag);
  if (dolog) {
    for (i=0; i<n; ++i) {
      double Re=s[i*2+0], Im=s[i*2+1];
      m[i]=log(1+sqrt(Re*Re+Im*Im));//log(1 + Mag)
      if (m[i]<minVal) {minVal=m[i];}
      if (m[i]>maxVal) {maxVal=m[i];}
    }
  } else {
    for (i=0; i<n; ++i) {
      double Re=s[i*2+0], Im=s[i*2+1];
      m[i]=(sqrt(Re*Re+Im*Im));//log(1 + Mag)
      if (m[i]<minVal) {minVal=m[i];}
      if (m[i]>maxVal) {maxVal=m[i];}
    }
  }
  
  //归一化处理将矩阵的元素值归一为[0,255]
  //[(f(x,y)-minVal)/(maxVal-minVal)]*255
  // Localize minimum and maximum values
  //cvMinMaxLoc( image_Re, &minVal, &maxVal, 0, 0, 0 );
  //FIND(n, (double*)(Mag->tt.data), 1, <, mini);minVal=((double*)(Mag->tt.data))[mini];
  //FIND(n, (double*)(Mag->tt.data), 1, >, maxi);maxVal=((double*)(Mag->tt.data))[maxi];
  // Normalize image (0 - 255) to be observed as an u8 image
  scale = 255/(maxVal - minVal);
  shift = -minVal * scale;
  //cvConvertScale(image_Re, dst, scale, shift);
  
  for (i=0; i<n; ++i) {
    assert(m[i]*scale+shift<256);
    d[i]=(uchar)(m[i]*scale+shift);
  }
  imfree(Mag);
  return dst;
}

int test_fft()
{
  img_t im[10] = {0};
  img_t *src = im, *srcc64 = im+1;  //源图像
  img_t *dst = im+2, *dstc64 = im+3;
  img_t *src2 = im+4, *srcc642 = im+5;  //源图像
  _chdir("E:/pub/pic");

  if (1) {
    enum {nn=7};
    double a[nn*nn] = {0};
    double b[nn*nn*2] = {0};
    double c[nn*nn*2] = {0};
    int i, j, k=1;
    for (i=0; i<nn*nn*2; i+=2) {
      a[i] = k++;
      a[i+1] = 0;
    }
    if (0) {
      for (i=0; i<nn; ++i) {
        k=1;
        for (j=0; j<nn; ++j) {
          a[(i*nn+j)*2+0] = k++;
          a[(i*nn+j)*2+1] = 0;
        }
      }
    }
    cvShowMat("aaa", "%5.4I64f", nn, nn*2, a, nn*2*sizeof(a[0]), sizeof(a[0]), -1);
    if (0) {
      cvDFT64f(2, nn, a, nn*2*sizeof(a[0]), 2, 2, nn, b, nn*2*sizeof(b[0]), 2, CC_DXT_FORWARD, 0);
      cvShowMat("1bb", "%5.4I64f", 2, nn*2, b, nn*2*sizeof(b[0]), sizeof(b[0]), -1);
      cvWaitKey(-1);
    }
    
    if (1) {
      cvDFT64f(nn, nn, a, nn*2*sizeof(a[0]), 2, nn, nn, b, nn*2*sizeof(b[0]), 2, CC_DXT_FORWARD, 0);
      //mat_shift(nn, nn, (CvComplex64f*)b, nn, (CvComplex64f*)b, nn);
      cvDFT64f(nn, nn, b, nn*2*sizeof(a[0]), 2, nn, nn, c, nn*2*sizeof(b[0]), 2, CC_DXT_INV_SCALE, 0);
      cvShowMat("bbb", "%5.4I64f", nn, nn*2, b, nn*2*sizeof(b[0]), sizeof(b[0]), -1);
      cvShowMat("ccc", "%5.4I64f", nn, nn*2, c, nn*2*sizeof(b[0]), sizeof(b[0]), -1);
      cvWaitKey(-1);
    }
  }
  //加载源图像，第二个参数表示将输入的图片转为单信道
  imread("lena512.jpg", 1, 1, src);

  u8_c64(src, 1, srcc64);
  //傅里叶变换
  imfft2(srcc64, CC_DXT_FORWARD, dstc64);
  fft2shift(dstc64, dstc64);          //中心化
  c64_u8(dstc64, 1, dst);
  //savebmpfile("傅里叶谱.bmp", dst, 8, 8);
  //实现傅里叶逆变换，并对结果进行缩放
  imfft2(dstc64, CC_DXT_INV_SCALE, srcc642);
  c64_u8(srcc642, 0, src2);
  //savebmpfile("傅里叶逆变换.bmp", src2, 8, 8);

  //imshow(src);
  imshow2("傅里叶谱", dst);
  imshow2("傅里叶逆变换", src2);
  cvWaitKey(-1);
  imfrees(im, 10);
  return 0;
}

int test_fft1() {
  double A[] = {1,2,3,4};
  img_t im[10] = {0};
  img_t* S = im+1;
  imsetsize_f8(im, 2,2,1);
  MEMCPY(im->tt.f8, A, 4);
  imfft2_real_cn(im, CC_DXT_FORWARD|CC_DXT_MUL_CONJ|CC_DXT_ROWS|CC_DXT_ABSQ, S);
  imshowmat_f8(S); cvWaitKey(-1);
  imfrees(im, 10);
  return 0;
}
#endif

#endif // _CXDXT_INL_
