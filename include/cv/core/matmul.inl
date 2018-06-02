
//! generalized matrix multiplication flags
typedef enum {
  CC_GEMM_1_T = 1, //!< transposes src1
    CC_GEMM_2_T = 2, //!< transposes src2
    CC_GEMM_3_T = 4 //!< transposes src3
} GemmFlags;


/****************************************************************************************\
*                                         GEMM                                           *
\****************************************************************************************/

static void
GEMM_CopyBlock( const uchar* src, size_t src_step,
                uchar* dst, size_t dst_step,
                CSize size, size_t pix_size )
{
    int j;
    size.width *= (int)(pix_size / sizeof(int));

    for( ; size.height--; src += src_step, dst += dst_step )
    {
        j=0;
         #if CC_ENABLE_UNROLLED
        for( ; j <= size.width - 4; j += 4 )
        {
            int t0 = ((const int*)src)[j];
            int t1 = ((const int*)src)[j+1];
            ((int*)dst)[j] = t0;
            ((int*)dst)[j+1] = t1;
            t0 = ((const int*)src)[j+2];
            t1 = ((const int*)src)[j+3];
            ((int*)dst)[j+2] = t0;
            ((int*)dst)[j+3] = t1;
        }
        #endif
        for( ; j < size.width; j++ )
            ((int*)dst)[j] = ((const int*)src)[j];
    }
}


static void
GEMM_TransposeBlock( const uchar* src, size_t src_step,
                     uchar* dst, size_t dst_step,
                     CSize size, size_t pix_size )
{
    int i, j;
    for( i = 0; i < size.width; i++, dst += dst_step, src += pix_size )
    {
        const uchar* _src = src;
        switch( pix_size )
        {
        case sizeof(int):
            for( j = 0; j < size.height; j++, _src += src_step )
                ((int*)dst)[j] = ((int*)_src)[0];
            break;
        case sizeof(int)*2:
            for( j = 0; j < size.height*2; j += 2, _src += src_step )
            {
                int t0 = ((int*)_src)[0];
                int t1 = ((int*)_src)[1];
                ((int*)dst)[j] = t0;
                ((int*)dst)[j+1] = t1;
            }
            break;
        case sizeof(int)*4:
            for( j = 0; j < size.height*4; j += 4, _src += src_step )
            {
                int t0 = ((int*)_src)[0];
                int t1 = ((int*)_src)[1];
                ((int*)dst)[j] = t0;
                ((int*)dst)[j+1] = t1;
                t0 = ((int*)_src)[2];
                t1 = ((int*)_src)[3];
                ((int*)dst)[j+2] = t0;
                ((int*)dst)[j+3] = t1;
            }
            break;
        default:
            assert(0);
            return;
        }
    }
}


template<typename T, typename WT> static void
GEMMSingleMul( const T* a_data, size_t a_step,
               const T* b_data, size_t b_step,
               const T* c_data, size_t c_step,
               T* d_data, size_t d_step,
               CSize a_size, CSize d_size,
               double alpha, double beta, int flags )
{
    int i, j, k, n = a_size.width, m = d_size.width, drows = d_size.height;
    const T *_a_data = a_data, *_b_data = b_data, *_c_data = c_data;
    T* _a_buf = NULL;
    T* a_buf = 0;
    size_t a_step0, a_step1, c_step0, c_step1, t_step;

    a_step /= sizeof(a_data[0]);
    b_step /= sizeof(b_data[0]);
    c_step /= sizeof(c_data[0]);
    d_step /= sizeof(d_data[0]);
    a_step0 = a_step;
    a_step1 = 1;

    if( !c_data )
        c_step0 = c_step1 = 0;
    else if( !(flags & CC_GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    else
        c_step0 = 1, c_step1 = c_step;

    if( flags & CC_GEMM_1_T )
    {
        CC_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        if( a_step > 1 && n > 1 )
        {
            _a_buf = MALLOC(T, n);
            a_buf = _a_buf;
        }
    }

    if( n == 1 ) /* external product */
    {
        T* _b_buf = NULL;
        T* b_buf = 0;

        if( a_step > 1 && a_size.height > 1 )
        {
            _a_buf = MALLOC(T, drows);
            a_buf = _a_buf;
            for( k = 0; k < drows; k++ )
                a_buf[k] = a_data[a_step*k];
            a_data = a_buf;
        }

        if( b_step > 1 )
        {
            _b_buf = MALLOC(T, d_size.width);
            b_buf = _b_buf;
            for( j = 0; j < d_size.width; j++ )
                b_buf[j] = b_data[j*b_step];
            b_data = b_buf;
        }

        for( i = 0; i < drows; i++, _c_data += c_step0, d_data += d_step )
        {
            WT al = WT(a_data[i])*alpha;
            c_data = _c_data;
            for( j = 0; j <= d_size.width - 2; j += 2, c_data += 2*c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                WT s1 = al*WT(b_data[j+1]);
                if( !c_data )
                {
                    d_data[j] = T(s0);
                    d_data[j+1] = T(s1);
                }
                else
                {
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                }
            }

            for( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT s0 = al*WT(b_data[j]);
                if( !c_data )
                    d_data[j] = T(s0);
                else
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
        FREE(_b_buf);
    }
    else if( flags & CC_GEMM_2_T ) /* A * Bt */
    {
        for( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            if( a_buf )
            {
                for( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            for( j = 0; j < d_size.width; j++, b_data += b_step,
                                               c_data += c_step1 )
            {
                WT s0(0), s1(0), s2(0), s3(0);
                k = 0;
                 #if CC_ENABLE_UNROLLED
                for( ; k <= n - 4; k += 4 )
                {
                    s0 += WT(a_data[k])*WT(b_data[k]);
                    s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                    s2 += WT(a_data[k+2])*WT(b_data[k+2]);
                    s3 += WT(a_data[k+3])*WT(b_data[k+3]);
                }
                #endif
                for( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);
                s0 = (s0+s1+s2+s3)*alpha;

                if( !c_data )
                    d_data[j] = T(s0);
                else
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    else if( d_size.width*sizeof(d_data[0]) <= 1600 )
    {
        for( i = 0; i < drows; i++, _a_data += a_step0,
                                    _c_data += c_step0,
                                    d_data += d_step )
        {
            a_data = _a_data, c_data = _c_data;

            if( a_buf )
            {
                for( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            for( j = 0; j <= m - 4; j += 4, c_data += 4*c_step1 )
            {
                const T* b = _b_data + j;
                WT s0(0), s1(0), s2(0), s3(0);

                for( k = 0; k < n; k++, b += b_step )
                {
                    WT a(a_data[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                if( !c_data )
                {
                    d_data[j] = T(s0*alpha);
                    d_data[j+1] = T(s1*alpha);
                    d_data[j+2] = T(s2*alpha);
                    d_data[j+3] = T(s3*alpha);
                }
                else
                {
                    s0 = s0*alpha; s1 = s1*alpha;
                    s2 = s2*alpha; s3 = s3*alpha;
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
                    d_data[j+1] = T(s1 + WT(c_data[c_step1])*beta);
                    d_data[j+2] = T(s2 + WT(c_data[c_step1*2])*beta);
                    d_data[j+3] = T(s3 + WT(c_data[c_step1*3])*beta);
                }
            }

            for( ; j < m; j++, c_data += c_step1 )
            {
                const T* b = _b_data + j;
                WT s0(0);

                for( k = 0; k < n; k++, b += b_step )
                    s0 += WT(a_data[k]) * WT(b[0]);

                s0 = s0*alpha;
                if( !c_data )
                    d_data[j] = T(s0);
                else
                    d_data[j] = T(s0 + WT(c_data[0])*beta);
            }
        }
    }
    else
    {
        WT* _d_buf = MALLOC(WT, m);
        WT* d_buf = _d_buf;

        for( i = 0; i < drows; i++, _a_data += a_step0, _c_data += c_step0, d_data += d_step )
        {
            a_data = _a_data;
            b_data = _b_data;
            c_data = _c_data;

            if( a_buf )
            {
                for( k = 0; k < n; k++ )
                    a_buf[k] = _a_data[a_step1*k];
                a_data = a_buf;
            }

            for( j = 0; j < m; j++ )
                d_buf[j] = WT(0);

            for( k = 0; k < n; k++, b_data += b_step )
            {
                WT al(a_data[k]);
                j=0;
                 #if CC_ENABLE_UNROLLED
                for(; j <= m - 4; j += 4 )
                {
                    WT t0 = d_buf[j] + WT(b_data[j])*al;
                    WT t1 = d_buf[j+1] + WT(b_data[j+1])*al;
                    d_buf[j] = t0;
                    d_buf[j+1] = t1;
                    t0 = d_buf[j+2] + WT(b_data[j+2])*al;
                    t1 = d_buf[j+3] + WT(b_data[j+3])*al;
                    d_buf[j+2] = t0;
                    d_buf[j+3] = t1;
                }
                #endif
                for( ; j < m; j++ )
                    d_buf[j] += WT(b_data[j])*al;
            }

            if( !c_data )
                for( j = 0; j < m; j++ )
                    d_data[j] = T(d_buf[j]*alpha);
            else
                for( j = 0; j < m; j++, c_data += c_step1 )
                {
                    WT t = d_buf[j]*alpha;
                    d_data[j] = T(t + WT(c_data[0])*beta);
                }
        }
        FREE(_d_buf);
    }
    FREE(_a_buf);
}


template<typename T, typename WT> static void
GEMMBlockMul( const T* a_data, size_t a_step,
              const T* b_data, size_t b_step,
              WT* d_data, size_t d_step,
              CSize a_size, CSize d_size, int flags )
{
    int i, j, k, n = a_size.width, m = d_size.width;
    const T *_a_data = a_data, *_b_data = b_data;
    T* _a_buf = NULL;
    T* a_buf = 0;
    size_t a_step0, a_step1, t_step;
    int do_acc = flags & 16;

    a_step /= sizeof(a_data[0]);
    b_step /= sizeof(b_data[0]);
    d_step /= sizeof(d_data[0]);

    a_step0 = a_step;
    a_step1 = 1;

    if( flags & CC_GEMM_1_T )
    {
        CC_SWAP( a_step0, a_step1, t_step );
        n = a_size.height;
        _a_buf = MALLOC(T, n);
        a_buf = _a_buf;
    }

    if( flags & CC_GEMM_2_T )
    {
        /* second operand is transposed */
        for( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data; b_data = _b_data;

            if( a_buf )
            {
                for( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            for( j = 0; j < d_size.width; j++, b_data += b_step )
            {
                WT s0 = do_acc ? d_data[j]:WT(0), s1(0);
                for( k = 0; k <= n - 2; k += 2 )
                {
                    s0 += WT(a_data[k])*WT(b_data[k]);
                    s1 += WT(a_data[k+1])*WT(b_data[k+1]);
                }

                for( ; k < n; k++ )
                    s0 += WT(a_data[k])*WT(b_data[k]);

                d_data[j] = s0 + s1;
            }
        }
    }
    else
    {
        for( i = 0; i < d_size.height; i++, _a_data += a_step0, d_data += d_step )
        {
            a_data = _a_data, b_data = _b_data;

            if( a_buf )
            {
                for( k = 0; k < n; k++ )
                    a_buf[k] = a_data[a_step1*k];
                a_data = a_buf;
            }

            for( j = 0; j <= m - 4; j += 4 )
            {
                WT s0, s1, s2, s3;
                const T* b = b_data + j;

                if( do_acc )
                {
                    s0 = d_data[j]; s1 = d_data[j+1];
                    s2 = d_data[j+2]; s3 = d_data[j+3];
                }
                else
                    s0 = s1 = s2 = s3 = WT(0);

                for( k = 0; k < n; k++, b += b_step )
                {
                    WT a(a_data[k]);
                    s0 += a * WT(b[0]); s1 += a * WT(b[1]);
                    s2 += a * WT(b[2]); s3 += a * WT(b[3]);
                }

                d_data[j] = s0; d_data[j+1] = s1;
                d_data[j+2] = s2; d_data[j+3] = s3;
            }

            for( ; j < m; j++ )
            {
                const T* b = b_data + j;
                WT s0 = do_acc ? d_data[j] : WT(0);

                for( k = 0; k < n; k++, b += b_step )
                    s0 += WT(a_data[k]) * WT(b[0]);

                d_data[j] = s0;
            }
        }
    }
    FREE(_a_buf);
}


template<typename T, typename WT> static void
GEMMStore( const T* c_data, size_t c_step,
           const WT* d_buf, size_t d_buf_step,
           T* d_data, size_t d_step, CSize d_size,
           double alpha, double beta, int flags )
{
    const T* _c_data = c_data;
    int j;
    size_t c_step0, c_step1;

    c_step /= sizeof(c_data[0]);
    d_buf_step /= sizeof(d_buf[0]);
    d_step /= sizeof(d_data[0]);

    if( !c_data )
        c_step0 = c_step1 = 0;
    else if( !(flags & CC_GEMM_3_T) )
        c_step0 = c_step, c_step1 = 1;
    else
        c_step0 = 1, c_step1 = c_step;

    for( ; d_size.height--; _c_data += c_step0, d_buf += d_buf_step, d_data += d_step )
    {
        if( _c_data )
        {
            c_data = _c_data;
            j=0;
             #if CC_ENABLE_UNROLLED
            for(; j <= d_size.width - 4; j += 4, c_data += 4*c_step1 )
            {
                WT t0 = d_buf[j]*alpha;
                WT t1 = d_buf[j+1]*alpha;
                t0 += WT(c_data[0])*beta;
                t1 += WT(c_data[c_step1])*beta;
                d_data[j] = T(t0);
                d_data[j+1] = T(t1);
                t0 = d_buf[j+2]*alpha;
                t1 = d_buf[j+3]*alpha;
                t0 += WT(c_data[c_step1*2])*beta;
                t1 += WT(c_data[c_step1*3])*beta;
                d_data[j+2] = T(t0);
                d_data[j+3] = T(t1);
            }
            #endif
            for( ; j < d_size.width; j++, c_data += c_step1 )
            {
                WT t0 = d_buf[j]*alpha;
                d_data[j] = T(t0 + WT(c_data[0])*beta);
            }
        }
        else
        {
            j = 0;
             #if CC_ENABLE_UNROLLED
            for( ; j <= d_size.width - 4; j += 4 )
            {
                WT t0 = d_buf[j]*alpha;
                WT t1 = d_buf[j+1]*alpha;
                d_data[j] = T(t0);
                d_data[j+1] = T(t1);
                t0 = d_buf[j+2]*alpha;
                t1 = d_buf[j+3]*alpha;
                d_data[j+2] = T(t0);
                d_data[j+3] = T(t1);
            }
            #endif
            for( ; j < d_size.width; j++ )
                d_data[j] = T(d_buf[j]*alpha);
        }
    }
}


typedef void (*GEMMSingleMulFunc)( const void* src1, size_t step1,
                   const void* src2, size_t step2, const void* src3, size_t step3,
                   void* dst, size_t dststep, CSize srcsize, CSize dstsize,
                   double alpha, double beta, int flags );

typedef void (*GEMMBlockMulFunc)( const void* src1, size_t step1,
                   const void* src2, size_t step2, void* dst, size_t dststep,
                   CSize srcsize, CSize dstsize, int flags );

typedef void (*GEMMStoreFunc)( const void* src1, size_t step1,
                   const void* src2, size_t step2, void* dst, size_t dststep,
                   CSize dstsize, double alpha, double beta, int flags );

static void GEMMSingleMul_32f( const float* a_data, size_t a_step,
              const float* b_data, size_t b_step,
              const float* c_data, size_t c_step,
              float* d_data, size_t d_step,
              CSize a_size, CSize d_size,
              double alpha, double beta, int flags )
{
    GEMMSingleMul<float,double>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

static void GEMMSingleMul_64f( const double* a_data, size_t a_step,
                              const double* b_data, size_t b_step,
                              const double* c_data, size_t c_step,
                              double* d_data, size_t d_step,
                              CSize a_size, CSize d_size,
                              double alpha, double beta, int flags )
{
    GEMMSingleMul<double,double>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

static void GEMMSingleMul_32fc( const CComplex32f* a_data, size_t a_step,
                              const CComplex32f* b_data, size_t b_step,
                              const CComplex32f* c_data, size_t c_step,
                              CComplex32f* d_data, size_t d_step,
                              CSize a_size, CSize d_size,
                              double alpha, double beta, int flags )
{
    GEMMSingleMul<CComplex32f,CComplex64f>(a_data, a_step, b_data, b_step, c_data,
                                c_step, d_data, d_step, a_size, d_size,
                                alpha, beta, flags);
}

static void GEMMSingleMul_64fc( const CComplex64f* a_data, size_t a_step,
                              const CComplex64f* b_data, size_t b_step,
                              const CComplex64f* c_data, size_t c_step,
                              CComplex64f* d_data, size_t d_step,
                              CSize a_size, CSize d_size,
                              double alpha, double beta, int flags )
{
    GEMMSingleMul<CComplex64f,CComplex64f>(a_data, a_step, b_data, b_step, c_data,
                                 c_step, d_data, d_step, a_size, d_size,
                                 alpha, beta, flags);
}

static void GEMMBlockMul_32f( const float* a_data, size_t a_step,
             const float* b_data, size_t b_step,
             double* d_data, size_t d_step,
             CSize a_size, CSize d_size, int flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


static void GEMMBlockMul_64f( const double* a_data, size_t a_step,
                             const double* b_data, size_t b_step,
                             double* d_data, size_t d_step,
                             CSize a_size, CSize d_size, int flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


static void GEMMBlockMul_32fc( const CComplex32f* a_data, size_t a_step,
                             const CComplex32f* b_data, size_t b_step,
                             CComplex64f* d_data, size_t d_step,
                             CSize a_size, CSize d_size, int flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


static void GEMMBlockMul_64fc( const CComplex64f* a_data, size_t a_step,
                             const CComplex64f* b_data, size_t b_step,
                             CComplex64f* d_data, size_t d_step,
                             CSize a_size, CSize d_size, int flags )
{
    GEMMBlockMul(a_data, a_step, b_data, b_step, d_data, d_step, a_size, d_size, flags);
}


static void GEMMStore_32f( const float* c_data, size_t c_step,
          const double* d_buf, size_t d_buf_step,
          float* d_data, size_t d_step, CSize d_size,
          double alpha, double beta, int flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


static void GEMMStore_64f( const double* c_data, size_t c_step,
                      const double* d_buf, size_t d_buf_step,
                      double* d_data, size_t d_step, CSize d_size,
                      double alpha, double beta, int flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


static void GEMMStore_32fc( const CComplex32f* c_data, size_t c_step,
                          const CComplex64f* d_buf, size_t d_buf_step,
                          CComplex32f* d_data, size_t d_step, CSize d_size,
                          double alpha, double beta, int flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}


static void GEMMStore_64fc( const CComplex64f* c_data, size_t c_step,
                          const CComplex64f* d_buf, size_t d_buf_step,
                          CComplex64f* d_data, size_t d_step, CSize d_size,
                          double alpha, double beta, int flags )
{
    GEMMStore(c_data, c_step, d_buf, d_buf_step, d_data, d_step, d_size, alpha, beta, flags);
}

#ifdef HAVE_CLAMDBLAS

static bool ocl_gemm_amdblas( const img_t* matA, const img_t* matB, double alpha,
                      const img_t* matC, double beta, img_t* matD, int flags )
{
    int type = matA CC_MAT_TYPE(), esz = CC_TYPE_SIZE(type);
    bool haveC = matC.kind() != _InputArray::NONE;
    CSize sizeA = matcvGetSize(A), sizeB = matB.size(), sizeC = haveC ? matC.size() : cSize(0, 0);
    bool atrans = (flags & CC_GEMM_1_T) != 0, btrans = (flags & CC_GEMM_2_T) != 0, ctrans = (flags & CC_GEMM_3_T) != 0;

    if (atrans)
        sizeA = cSize(sizeA.height, sizeA.width);
    if (btrans)
        sizeB = cSize(sizeB.height, sizeB.width);
    if (haveC && ctrans)
        sizeC = cSize(sizeC.height, sizeC.width);

    CSize sizeD(sizeB.width, sizeA.height);

    CC_Assert( matB CC_MAT_TYPE() == type && (!haveC || matC CC_MAT_TYPE() == type) );
    CC_Assert( sizeA.width == sizeB.height && (!haveC || sizeC == sizeD) );

    matD cvSetMat(sizeD, type);
    if ( matA.offset() % esz != 0 || matA->step() % esz != 0 ||
         matB.offset() % esz != 0 || matB->step() % esz != 0 ||
         (haveC && (matC.offset() % esz != 0 || matC->step() % esz != 0)) )
        return false;

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();
    if (!ocl::internal::isCLBuffer(A) || !ocl::internal::isCLBuffer(B) || !ocl::internal::isCLBuffer(D))
    {
        return false;
    }
    if (haveC)
    {
        UMat C = matC.getUMat();
        if (!ocl::internal::isCLBuffer(C))
            return false;
    }
    if (haveC)
        ctrans ? transpose(matC, D) : matC.copyTo(D);
    else
        D.setTo(Scalar::all(0));

    int M = sizeD.height, N = sizeD.width, K = sizeA.width;
    int lda = (int)A->step / esz, ldb = (int)B->step / esz, ldc = (int)D->step / esz;
    int offa = (int)A.offset / esz, offb = (int)B.offset / esz, offc = (int)D.offset / esz;

    cl_command_queue clq = (cl_command_queue)ocl::Queue::getDefault()->tt.data;
    clAmdBlasTranspose transA = atrans ? clAmdBlasTrans : clAmdBlasNoTrans;
    clAmdBlasTranspose transB = btrans ? clAmdBlasTrans : clAmdBlasNoTrans;
    clAmdBlasOrder order = clAmdBlasRowMajor;
    clAmdBlasStatus status = clAmdBlasSuccess;

    if (type == CC_32FC1)
        status = clAmdBlasSgemmEx(order, transA, transB, M, N, K,
                                  (cl_float)alpha, (const cl_mem)A.handle(ACCESS_READ), offa, lda,
                                  (const cl_mem)B.handle(ACCESS_READ), offb, ldb,
                                  (cl_float)beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                                  1, &clq, 0, NULL, NULL);
    else if (type == CC_64FC1)
        status = clAmdBlasDgemmEx(order, transA, transB, M, N, K,
                                  alpha, (const cl_mem)A.handle(ACCESS_READ), offa, lda,
                                  (const cl_mem)B.handle(ACCESS_READ), offb, ldb,
                                  beta, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                                  1, &clq, 0, NULL, NULL);
    else if (type == CC_32FC2)
    {
         cl_float2 alpha_2 = { { (cl_float)alpha, 0 } };
         cl_float2 beta_2  = { { (cl_float)beta, 0 } };
         status = clAmdBlasCgemmEx(order, transA, transB, M, N, K,
                                   alpha_2, (const cl_mem)A.handle(ACCESS_READ), offa, lda,
                                   (const cl_mem)B.handle(ACCESS_READ), offb, ldb,
                                   beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                                   1, &clq, 0, NULL, NULL);
    }
    else if (type == CC_64FC2)
    {
        cl_double2 alpha_2 = { { alpha, 0 } };
        cl_double2 beta_2  = { { beta, 0 } };
        status = clAmdBlasZgemmEx(order, transA, transB, M, N, K,
                                  alpha_2, (const cl_mem)A.handle(ACCESS_READ), offa, lda,
                                  (const cl_mem)B.handle(ACCESS_READ), offb, ldb,
                                  beta_2, (cl_mem)D.handle(ACCESS_RW), offc, ldc,
                                  1, &clq, 0, NULL, NULL);
    }
    else
        CC_Error(CC_StsUnsupportedFormat, "");

    return status == clAmdBlasSuccess;
}

#endif

#ifdef HAVE_OPENCL1

static bool ocl_gemm( const img_t* matA, const img_t* matB, double alpha,
                      const img_t* matC, double beta, img_t* matD, int flags )
{
    TypeCnId type = imtype(matA);
    TypeId tid = imtid(matA);
    int cn = CC_MAT_CN(matA);

    CC_Assert( type == CC_MAT_TYPE(matB) && (type == CC_32FC1 || type == CC_64FC1 || type == CC_32FC2 || type == CC_64FC2) );

    const ocl_device_t* dev = ocl_device_get();
    bool doubleSupport = dev->doubleFPConfig_ > 0;

    if (!doubleSupport && tid == CC_64F)
        return false;

    bool haveC = !imempty(matC);
    CSize sizeA = cvGetSize(matA), sizeB = cvGetSize(matB), sizeC = haveC ? cvGetSize(matC) : cSize(0, 0);
    bool atrans = (flags & CC_GEMM_1_T) != 0, btrans = (flags & CC_GEMM_2_T) != 0, ctrans = (flags & CC_GEMM_3_T) != 0;

    if (atrans)
        sizeA = cSize(sizeA.height, sizeA.width);
    if (btrans)
        sizeB = cSize(sizeB.height, sizeB.width);
    if (haveC && ctrans)
        sizeC = cSize(sizeC.height, sizeC.width);

    //CSize sizeD = cSize(sizeB.width, sizeA.height);

#if 0
    CC_Assert( !haveC || CC_MAT_TYPE(matC) == type );
    CC_Assert( sizeA.width == sizeB.height && (!haveC || sizeC == sizeD) );

    int max_wg_size = (int)dev.maxWorkGroupSize();
    int block_size = (max_wg_size / (32*cn) < 32) ? (max_wg_size / (16*cn) < 16) ? (max_wg_size / (8*cn) < 8) ? 1 : 8 : 16 : 32;

    matD cvSetMat(sizeD, type);

    UMat A = matA.getUMat(), B = matB.getUMat(), D = matD.getUMat();

    if (atrans)
        A = A.t();

    if (btrans)
        B = B.t();

    if (haveC)
        ctrans ? transpose(matC, D) : matC.copyTo(D);
    int vectorWidths[] = { 4, 4, 2, 2, 1, 4, cn, -1 };
    int kercn = ocl::checkOptimalVectorWidth(vectorWidths, B, D);
#endif
    ASSERT(1==cn);
    int kercn = 1;
#if 0

    String opts = format("-D T=%s -D T1=%s -D WT=%s -D cn=%d -D kercn=%d -D LOCAL_SIZE=%d %s %s %s",
                          ocl::typeToStr(type), ocl::typeToStr(type), ocl::typeToStr(CC_MAKETYPE(type, kercn)),
                          cn, kercn, block_size,
                          (sizeA.width % block_size !=0) ? "-D NO_MULT" : "",
                          haveC ? "-D HAVE_C" : "",
                          doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel k("gemm", ocl::core::gemm_oclsrc, opts);
    if (k.empty())
        return false;

    if (type == CC_64F)
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, alpha, beta);
    else
        k.args(ocl::KernelArg::ReadOnlyNoSize(A),
               ocl::KernelArg::ReadOnlyNoSize(B, cn, kercn),
               ocl::KernelArg::ReadWrite(D, cn, kercn),
               sizeA.width, (float)alpha, (float)beta);

    size_t globalsize[2] = { (size_t)sizeD.width * cn / kercn, (size_t)sizeD.height};
    size_t localsize[2] = { (size_t)block_size, (size_t)block_size};
    return k.run(2, globalsize, block_size!=1 ? localsize : NULL, false);
#endif
    return 0;
}
#endif

static void gemmImpl( const img_t* A, const img_t* B, double alpha,
           const img_t* C, double beta, img_t* D, int flags )
{
    CC_INSTRUMENT_REGION()

    const int block_lin_size = 128;
    const int block_size = block_lin_size * block_lin_size;

    static double zero[] = {0,0,0,0};
    static float zerof[] = {0,0,0,0};

    CSize a_size = cvGetSize(A), d_size;
    int i, len = 0;
    int cn = CC_MAT_CN(A);
    TypeId type = CC_MAT_DEPTH(A);

    switch( flags & (CC_GEMM_1_T|CC_GEMM_2_T) )
    {
    case 0:
        d_size = cSize( B->cols, a_size.height );
        len = B->rows;
        break;
    case 1:
        d_size = cSize( B->cols, a_size.width );
        len = B->rows;
        break;
    case 2:
        d_size = cSize( B->rows, a_size.height );
        len = B->cols;
        break;
    case 3:
        d_size = cSize( B->rows, a_size.width );
        len = B->cols;
        break;
    }

    if( flags == 0 && 2 <= len && len <= 4 && (len == d_size.width || len == d_size.height) )
    {
        if( type == CC_32F )
        {
            float* d = img_ptr(float, D);
            const float *a = img_ptr(float, A),
                        *b = img_ptr(float, B),
                        *c = (const float*)C->tt.data;
            size_t d_step = D->step/sizeof(d[0]),
                a_step = A->step/sizeof(a[0]),
                b_step = B->step/sizeof(b[0]),
                c_step = C->tt.data ? C->step/sizeof(c[0]) : 0;

            if( !c )
                c = zerof;

            switch( len )
            {
            case 2:
                if( len == d_size.width && b != d )
                {
                    for( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step];
                        float t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[1] = (float)(t1*alpha + c[1]*beta);
                    }
                }
                else if( a != d )
                {
                    int c_step0 = 1;
                    if( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step];
                        float t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[d_step] = (float)(t1*alpha + c[c_step]*beta);
                    }
                }
                else
                    break;
                return;
            case 3:
                if( len == d_size.width && b != d )
                {
                    for( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        float t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        float t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[1] = (float)(t1*alpha + c[1]*beta);
                        d[2] = (float)(t2*alpha + c[2]*beta);
                    }
                }
                else if( a != d )
                {
                    int c_step0 = 1;
                    if( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        float t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        float t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[d_step] = (float)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (float)(t2*alpha + c[c_step*2]*beta);
                    }
                }
                else
                    break;
                return;
            case 4:
                if( len == d_size.width && b != d )
                {
                    for( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        float t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        float t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        float t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[1] = (float)(t1*alpha + c[1]*beta);
                        d[2] = (float)(t2*alpha + c[2]*beta);
                        d[3] = (float)(t3*alpha + c[3]*beta);
                    }
                }
                else if( len <= 16 && a != d )
                {
                    int c_step0 = 1;
                    if( c == zerof )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        float t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        float t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                   a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        float t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                   a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        float t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                   a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = (float)(t0*alpha + c[0]*beta);
                        d[d_step] = (float)(t1*alpha + c[c_step]*beta);
                        d[d_step*2] = (float)(t2*alpha + c[c_step*2]*beta);
                        d[d_step*3] = (float)(t3*alpha + c[c_step*3]*beta);
                    }
                }
                else
                    break;
                return;
            }
        }

        if( type == CC_64F )
        {
            double* d = img_ptr(double, D);
            const double *a = img_ptr(double, A),
                         *b = img_ptr(double, B),
                         *c = (const double*)C->tt.data;
            size_t d_step = D->step/sizeof(d[0]),
                a_step = A->step/sizeof(a[0]),
                b_step = B->step/sizeof(b[0]),
                c_step = C->tt.data ? C->step/sizeof(c[0]) : 0;
            if( !c )
                c = zero;

            switch( len )
            {
            case 2:
                if( len == d_size.width && b != d )
                {
                    for( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        double t0 = a[0]*b[0] + a[1]*b[b_step];
                        double t1 = a[0]*b[1] + a[1]*b[b_step+1];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                    }
                }
                else if( a != d )
                {
                    int c_step0 = 1;
                    if( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        double t0 = a[0]*b[0] + a[1]*b[b_step];
                        double t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                    }
                }
                else
                    break;
                return;
            case 3:
                if( len == d_size.width && b != d )
                {
                    for( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        double t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        double t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1];
                        double t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                    }
                }
                else if( a != d )
                {
                    int c_step0 = 1;
                    if( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        double t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2];
                        double t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] + a[a_step+2]*b[b_step*2];
                        double t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] + a[a_step*2+2]*b[b_step*2];

                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                    }
                }
                else
                    break;
                return;
            case 4:
                if( len == d_size.width && b != d )
                {
                    for( i = 0; i < d_size.height; i++, d += d_step, a += a_step, c += c_step )
                    {
                        double t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        double t1 = a[0]*b[1] + a[1]*b[b_step+1] + a[2]*b[b_step*2+1] + a[3]*b[b_step*3+1];
                        double t2 = a[0]*b[2] + a[1]*b[b_step+2] + a[2]*b[b_step*2+2] + a[3]*b[b_step*3+2];
                        double t3 = a[0]*b[3] + a[1]*b[b_step+3] + a[2]*b[b_step*2+3] + a[3]*b[b_step*3+3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[1] = t1*alpha + c[1]*beta;
                        d[2] = t2*alpha + c[2]*beta;
                        d[3] = t3*alpha + c[3]*beta;
                    }
                }
                else if( d_size.width <= 16 && a != d )
                {
                    int c_step0 = 1;
                    if( c == zero )
                    {
                        c_step0 = 0;
                        c_step = 1;
                    }

                    for( i = 0; i < d_size.width; i++, d++, b++, c += c_step0 )
                    {
                        double t0 = a[0]*b[0] + a[1]*b[b_step] + a[2]*b[b_step*2] + a[3]*b[b_step*3];
                        double t1 = a[a_step]*b[0] + a[a_step+1]*b[b_step] +
                                    a[a_step+2]*b[b_step*2] + a[a_step+3]*b[b_step*3];
                        double t2 = a[a_step*2]*b[0] + a[a_step*2+1]*b[b_step] +
                                    a[a_step*2+2]*b[b_step*2] + a[a_step*2+3]*b[b_step*3];
                        double t3 = a[a_step*3]*b[0] + a[a_step*3+1]*b[b_step] +
                                    a[a_step*3+2]*b[b_step*2] + a[a_step*3+3]*b[b_step*3];
                        d[0] = t0*alpha + c[0]*beta;
                        d[d_step] = t1*alpha + c[c_step]*beta;
                        d[d_step*2] = t2*alpha + c[c_step*2]*beta;
                        d[d_step*3] = t3*alpha + c[c_step*3]*beta;
                    }
                }
                else
                    break;
                return;
            }
        }
    }

    {
    size_t b_step = B->step;
    GEMMSingleMulFunc singleMulFunc;
    GEMMBlockMulFunc blockMulFunc;
    GEMMStoreFunc storeFunc;
    img_t *matD = D;
    const uchar* Cdata = C->tt.data;
    size_t Cstep = C->tt.data ? (size_t)C->step : 0;
    uchar* buf = NULL;
    TypeCnId typecn = CC_MAKETYPECN(type, cn);

    if( typecn == CC_32FC1 )
    {
        singleMulFunc = (GEMMSingleMulFunc)GEMMSingleMul_32f;
        blockMulFunc = (GEMMBlockMulFunc)GEMMBlockMul_32f;
        storeFunc = (GEMMStoreFunc)GEMMStore_32f;
    }
    else if( typecn == CC_64FC1 )
    {
        singleMulFunc = (GEMMSingleMulFunc)GEMMSingleMul_64f;
        blockMulFunc = (GEMMBlockMulFunc)GEMMBlockMul_64f;
        storeFunc = (GEMMStoreFunc)GEMMStore_64f;
    }
    else if( typecn == CC_32FC2 )
    {
        singleMulFunc = (GEMMSingleMulFunc)GEMMSingleMul_32fc;
        blockMulFunc = (GEMMBlockMulFunc)GEMMBlockMul_32fc;
        storeFunc = (GEMMStoreFunc)GEMMStore_32fc;
    }
    else
    {
        CC_Assert( typecn == CC_64FC2 );
        singleMulFunc = (GEMMSingleMulFunc)GEMMSingleMul_64fc;
        blockMulFunc = (GEMMBlockMulFunc)GEMMBlockMul_64fc;
        storeFunc = (GEMMStoreFunc)GEMMStore_64fc;
    }

    if( (d_size.width == 1 || len == 1) && !(flags & CC_GEMM_2_T) && CC_IS_CONT_MAT(B) )
    {
        b_step = d_size.width == 1 ? 0 : CC_TYPE_SIZE(type);
        flags |= CC_GEMM_2_T;
    }

    /*

    if( blas_func )
    {
        const char* transa = flags & CC_GEMM_1_T ? "t" : "n";
        const char* transb = flags & CC_GEMM_2_T ? "t" : "n";
        int lda, ldb, ldd;

        if( C->data.ptr )
        {
            if( C->data.ptr != D->data.ptr )
            {
                if( !(flags & CC_GEMM_3_T) )
                    cvCopy( C, D );
                else
                    cvTranspose( C, D );
            }
        }

        if( CC_MAT_DEPTH(type) == CC_32F )
        {
            Complex32f _alpha, _beta;

            lda = A->step/sizeof(float);
            ldb = b_step/sizeof(float);
            ldd = D->step/sizeof(float);
            _alpha.re = (float)alpha;
            _alpha.im = 0;
            _beta.re = C->data.ptr ? (float)beta : 0;
            _beta.im = 0;
            if( CC_MAT_CN(type) == 2 )
                lda /= 2, ldb /= 2, ldd /= 2;

            blas_func( transb, transa, &d_size.width, &d_size.height, &len,
                   &_alpha, B->data.ptr, &ldb, A->data.ptr, &lda,
                   &_beta, D->data.ptr, &ldd );
        }
        else
        {
            CComplex64f _alpha, _beta;

            lda = A->step/sizeof(double);
            ldb = b_step/sizeof(double);
            ldd = D->step/sizeof(double);
            _alpha.re = alpha;
            _alpha.im = 0;
            _beta.re = C->data.ptr ? beta : 0;
            _beta.im = 0;
            if( CC_MAT_CN(type) == 2 )
                lda /= 2, ldb /= 2, ldd /= 2;

            blas_func( transb, transa, &d_size.width, &d_size.height, &len,
                   &_alpha, B->data.ptr, &ldb, A->data.ptr, &lda,
                   &_beta, D->data.ptr, &ldd );
        }
    }
    else*/ if( ((d_size.height <= block_lin_size/2 || d_size.width <= block_lin_size/2) &&
        len <= 10000) || len <= 10 ||
        (d_size.width <= block_lin_size &&
        d_size.height <= block_lin_size && len <= block_lin_size) )
    {
        singleMulFunc( A->tt.data, A->step, B->tt.data, b_step, Cdata, Cstep,
                       matD->tt.data, matD->step, a_size, d_size, alpha, beta, flags );
    }
    else
    {
        int is_a_t = flags & CC_GEMM_1_T;
        int is_b_t = flags & CC_GEMM_2_T;
        int elem_size = CC_TYPE_SIZE(type);
        int dk0_1, dk0_2;
        size_t a_buf_size = 0, b_buf_size, d_buf_size;
        uchar* a_buf = 0;
        uchar* b_buf = 0;
        uchar* d_buf = 0;
        int j, k, di = 0, dj = 0, dk = 0;
        int dm0, dn0, dk0;
        size_t a_step0, a_step1, b_step0, b_step1, c_step0, c_step1;
        int work_elem_size = elem_size << (type == CC_32F ? 1 : 0);

        if( !is_a_t )
            a_step0 = A->step, a_step1 = elem_size;
        else
            a_step0 = elem_size, a_step1 = A->step;

        if( !is_b_t )
            b_step0 = b_step, b_step1 = elem_size;
        else
            b_step0 = elem_size, b_step1 = b_step;

        if( imempty(C) )
        {
            c_step0 = c_step1 = 0;
            flags &= ~CC_GEMM_3_T;
        }
        else if( !(flags & CC_GEMM_3_T) )
            c_step0 = C->step, c_step1 = elem_size;
        else
            c_step0 = elem_size, c_step1 = C->step;

        dm0 = MIN( block_lin_size, d_size.height );
        dn0 = MIN( block_lin_size, d_size.width );
        dk0_1 = block_size / dm0;
        dk0_2 = block_size / dn0;
        dk0 = MIN( dk0_1, dk0_2 );
        dk0 = MIN( dk0, len );
        if( dk0*dm0 > block_size )
            dm0 = block_size / dk0;
        if( dk0*dn0 > block_size )
            dn0 = block_size / dk0;

        dk0_1 = (dn0+dn0/8+2) & -2;
        b_buf_size = (size_t)(dk0+dk0/8+1)*dk0_1*elem_size;
        d_buf_size = (size_t)(dk0+dk0/8+1)*dk0_1*work_elem_size;

        if( is_a_t )
        {
            a_buf_size = (size_t)(dm0+dm0/8+1)*((dk0+dk0/8+2)&-2)*elem_size;
            flags &= ~CC_GEMM_1_T;
        }

        buf = MALLOC(uchar, d_buf_size + b_buf_size + a_buf_size);
        d_buf = (uchar*)buf;
        b_buf = d_buf + d_buf_size;

        if( is_a_t )
            a_buf = b_buf + b_buf_size;

        for( i = 0; i < d_size.height; i += di )
        {
            di = dm0;
            if( i + di >= d_size.height || 8*(i + di) + di > 8*d_size.height )
                di = d_size.height - i;

            for( j = 0; j < d_size.width; j += dj )
            {
                uchar* _d = matD->tt.data + i*matD->step + j*elem_size;
                const uchar* _c = Cdata + i*c_step0 + j*c_step1;
                size_t _d_step = matD->step;
                dj = dn0;

                if( j + dj >= d_size.width || 8*(j + dj) + dj > 8*d_size.width )
                    dj = d_size.width - j;

                flags &= 15;
                if( dk0 < len )
                {
                    _d = d_buf;
                    _d_step = dj*work_elem_size;
                }

                for( k = 0; k < len; k += dk )
                {
                    const uchar* _a = A->tt.data + i*a_step0 + k*a_step1;
                    size_t _a_step = A->step;
                    const uchar* _b = B->tt.data + k*b_step0 + j*b_step1;
                    size_t _b_step = b_step;
                    CSize a_bl_size;

                    dk = dk0;
                    if( k + dk >= len || 8*(k + dk) + dk > 8*len )
                        dk = len - k;

                    if( !is_a_t )
                        a_bl_size.width = dk, a_bl_size.height = di;
                    else
                        a_bl_size.width = di, a_bl_size.height = dk;

                    if( a_buf && is_a_t )
                    {
                        _a_step = dk*elem_size;
                        GEMM_TransposeBlock( _a, A->step, a_buf, _a_step, a_bl_size, elem_size );
                        T_SWAP( int, a_bl_size.width, a_bl_size.height );
                        _a = a_buf;
                    }

                    if( dj < d_size.width )
                    {
                        CSize b_size;
                        if( !is_b_t )
                            b_size.width = dj, b_size.height = dk;
                        else
                            b_size.width = dk, b_size.height = dj;

                        _b_step = b_size.width*elem_size;
                        GEMM_CopyBlock( _b, b_step, b_buf, _b_step, b_size, elem_size );
                        _b = b_buf;
                    }

                    if( dk0 < len )
                        blockMulFunc( _a, _a_step, _b, _b_step, _d, _d_step,
                                      a_bl_size, cSize(dj,di), flags );
                    else
                        singleMulFunc( _a, _a_step, _b, _b_step, _c, Cstep,
                                       _d, _d_step, a_bl_size, cSize(dj,di), alpha, beta, flags );
                    flags |= 16;
                }

                if( dk0 < len )
                    storeFunc( _c, Cstep, _d, _d_step,
                               matD->tt.data + i*matD->step + j*elem_size,
                               matD->step, cSize(dj,di), alpha, beta, flags );
            }
        }
    }
    }
}

template <typename fptype>inline static void
callGemmImpl(const fptype *src1, size_t src1_step, const fptype *src2, size_t src2_step, fptype alpha,
          const fptype *src3, size_t src3_step, fptype beta, fptype *dst, size_t dst_step, int m_a, int n_a, int n_d, int flags, TypeId type, int cn)
{
    CC_StaticAssert(CC_GEMM_1_T == CC_HAL_GEMM_1_T, "Incompatible CC_GEMM_1_T flag in HAL");
    CC_StaticAssert(CC_GEMM_2_T == CC_HAL_GEMM_2_T, "Incompatible CC_GEMM_2_T flag in HAL");
    CC_StaticAssert(CC_GEMM_3_T == CC_HAL_GEMM_3_T, "Incompatible CC_GEMM_3_T flag in HAL");

    int b_m, b_n, c_m, c_n, m_d;

    if(flags & CC_GEMM_2_T)
    {
        b_m = n_d;
        if(flags & CC_GEMM_1_T )
        {
            b_n = m_a;
            m_d = n_a;
        }
        else
        {
            b_n = n_a;
            m_d = m_a;
        }
    }
    else
    {
        b_n = n_d;
        if(flags & CC_GEMM_1_T )
        {
            b_m = m_a;
            m_d = n_a;
        }
        else
        {
            m_d = m_a;
            b_m = n_a;
        }
    }

    if(flags & CC_GEMM_3_T)
    {
        c_m = n_d;
        c_n = m_d;
    }
    else
    {
        c_m = m_d;
        c_n = n_d;
    }

    img_t A[1]={0}, B[1]={0}, C[1]={0}, D[1]={0};
    if(src1 != NULL)
        cvMat(A, m_a, n_a, type, cn, (void*)src1, src1_step);
    if(src2 != NULL)
        cvMat(B, b_m, b_n, type, cn, (void*)src2, src2_step);
    if(src3 != NULL && beta != 0.0)
        cvMat(C, c_m, c_n, type, cn, (void*)src3, src3_step);
    cvMat(D, m_d, n_d, type, cn, (void*)dst, dst_step);

    gemmImpl(A, B, alpha, C, beta, D, flags);
}



static void hal_gemm32f(const float* src1, size_t src1_step, const float* src2, size_t src2_step,
                        float alpha, const float* src3, size_t src3_step, float beta, float* dst, size_t dst_step,
                        int m_a, int n_a, int n_d, int flags)
{
    CALL_HAL(gemm32f, cv_hal_gemm32f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags))
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CC_32F, 1);
}
static void hal_gemm64f(const double* src1, size_t src1_step, const double* src2, size_t src2_step,
                        double alpha, const double* src3, size_t src3_step, double beta, double* dst, size_t dst_step,
                        int m_a, int n_a, int n_d, int flags)
{
    CALL_HAL(gemm64f, cv_hal_gemm64f, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags))
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CC_64F, 1);
}

static void hal_gemm32fc(const float* src1, size_t src1_step, const float* src2, size_t src2_step,
                        float alpha, const float* src3, size_t src3_step, float beta, float* dst, size_t dst_step,
                        int m_a, int n_a, int n_d, int flags)
{
    CALL_HAL(gemm32fc, cv_hal_gemm32fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags))
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CC_32F, 2);
}

static void hal_gemm64fc(const double* src1, size_t src1_step, const double* src2, size_t src2_step,
                        double alpha, const double* src3, size_t src3_step, double beta, double* dst, size_t dst_step,
                        int m_a, int n_a, int n_d, int flags)
{
    CALL_HAL(gemm64fc, cv_hal_gemm64fc, (src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags))
    callGemmImpl(src1, src1_step, src2, src2_step, alpha, src3, src3_step, beta, dst, dst_step, m_a, n_a, n_d, flags, CC_64F, 2);
}
static void gemm( const img_t* matA, const img_t* matB, double alpha,
           const img_t* matC, double beta, img_t* _matD, int flags )
{
#ifdef HAVE_CLAMDBLAS
    CC_OCL_RUN(ocl::haveAmdBlas() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2 && _matD.isUMat() &&
        matA->cols() > 20 && matA->rows() > 20 && matB->cols() > 20, // since it works incorrect for small sizes
        ocl_gemm_amdblas(matA, matB, alpha, matC, beta, _matD, flags))
#endif

#ifdef HAVE_OPENCL
    CC_OCL_RUN(_matD.isUMat() && matA.dims() <= 2 && matB.dims() <= 2 && matC.dims() <= 2,
               ocl_gemm(matA, matB, alpha, matC, beta, _matD, flags))
#endif

    const img_t *A = matA, *B = matB, *C = (matC && beta != 0.0) ? matC : NULL;
    CSize a_size = cvGetSize(A), d_size;
    int len = 0;
    TypeId type = CC_MAT_DEPTH(A);
    TypeCnId typecn = CC_MAT_TYPECN(A);

    CC_Assert( type == CC_MAT_TYPE(B) && (typecn == CC_32FC1 || typecn == CC_64FC1 || typecn == CC_32FC2 || typecn == CC_64FC2) );

    switch( flags & (CC_GEMM_1_T|CC_GEMM_2_T) )
    {
    case 0:
        d_size = cSize( B->cols, a_size.height );
        len = B->rows;
        CC_Assert( a_size.width == len );
        break;
    case 1:
        d_size = cSize( B->cols, a_size.width );
        len = B->rows;
        CC_Assert( a_size.height == len );
        break;
    case 2:
        d_size = cSize( B->rows, a_size.height );
        len = B->cols;
        CC_Assert( a_size.width == len );
        break;
    case 3:
        d_size = cSize( B->rows, a_size.width );
        len = B->cols;
        CC_Assert( a_size.height == len );
        break;
    }

    if( !imempty(C) )
    {
        CC_Assert( CC_MAT_TYPE(C) == type &&
            (((flags&CC_GEMM_3_T) == 0 && C->rows == d_size.height && C->cols == d_size.width) ||
             ((flags&CC_GEMM_3_T) != 0 && C->rows == d_size.width && C->cols == d_size.height)));
    }

    cvSetMat( _matD, d_size.height, d_size.width, type );
    img_t* D = _matD;
    if( C && (flags & CC_GEMM_3_T) != 0 && C->tt.data == D->tt.data )
    {
        cvTranspose( C, D );
        flags &= ~CC_GEMM_3_T;
    }

    img_t *DProxyPtr = D, DProxy[1] = {0};
    if( D->tt.data == A->tt.data || D->tt.data == B->tt.data )
    {
        DProxyPtr = cvSetMat(DProxy, d_size.height, d_size.width, CC_MAT_DEPTH(D), CC_MAT_CN(D));
    }

    if( type == CC_32FC1 )
        hal_gemm32f(img_ptr(float, A), A->step, img_ptr(float, B), B->step, (float)(alpha),
                     !C ? NULL : img_ptr(float, C), !C ? 0 : C->step, (float)(beta),
                     img_ptr(float, DProxyPtr), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    else if( type == CC_64FC1 )
        hal_gemm64f(img_ptr(double, A), A->step, img_ptr(double, B), B->step, alpha,
                     !C ? NULL : img_ptr(double, C), !C ? 0 : C->step, beta,
                     img_ptr(double, DProxyPtr), DProxyPtr->step,
                     a_size.height, a_size.width, DProxyPtr->cols, flags);
    else if( type == CC_32FC2 )
        hal_gemm32fc(img_ptr(float, A), A->step, img_ptr(float, B), B->step, (float)(alpha),
                      !C ? NULL : img_ptr(float, C), !C ? 0 : C->step, (float)(beta),
                      img_ptr(float, DProxyPtr), DProxyPtr->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    else
    {
        CC_Assert( type == CC_64FC2 );
        hal_gemm64fc(img_ptr(double, A), A->step, img_ptr(double, B), B->step, alpha,
                      !C ? NULL : img_ptr(double, C), !C ? 0 : C->step, beta,
                      img_ptr(double, D), D->step,
                      a_size.height, a_size.width, DProxyPtr->cols, flags);
    }

    if(DProxyPtr != D)
        cvCopy(DProxyPtr, D);

    imfree(DProxy);
}

static img_t* matmul(const img_t* matA, const img_t* matB, img_t* matC, int flags CC_DEFAULT(0) ) {
  gemm(matA, matB, 1, NULL, 0, matC, flags);
  return matC;
}

/****************************************************************************************\
*                                        Transform                                       *
\****************************************************************************************/



template<typename T, typename WT> static void
transform_( const T* src, T* dst, const WT* m, int len, int scn, int dcn )
{
    int x;

    if( scn == 2 && dcn == 2 )
    {
        for( x = 0; x < len*2; x += 2 )
        {
            WT v0 = src[x], v1 = src[x+1];
            T t0 = (T)(m[0]*v0 + m[1]*v1 + m[2]);
            T t1 = (T)(m[3]*v0 + m[4]*v1 + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    else if( scn == 3 && dcn == 3 )
    {
        for( x = 0; x < len*3; x += 3 )
        {
            WT v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            T t0 = (T)(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]);
            T t1 = (T)(m[4]*v0 + m[5]*v1 + m[6]*v2 + m[7]);
            T t2 = (T)(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    else if( scn == 3 && dcn == 1 )
    {
        for( x = 0; x < len; x++, src += 3 )
            dst[x] = (T)(m[0]*src[0] + m[1]*src[1] + m[2]*src[2] + m[3]);
    }
    else if( scn == 4 && dcn == 4 )
    {
        for( x = 0; x < len*4; x += 4 )
        {
            WT v0 = src[x], v1 = src[x+1], v2 = src[x+2], v3 = src[x+3];
            T t0 = (T)(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]*v3 + m[4]);
            T t1 = (T)(m[5]*v0 + m[6]*v1 + m[7]*v2 + m[8]*v3 + m[9]);
            dst[x] = t0; dst[x+1] = t1;
            t0 = (T)(m[10]*v0 + m[11]*v1 + m[12]*v2 + m[13]*v3 + m[14]);
            t1 = (T)(m[15]*v0 + m[16]*v1 + m[17]*v2 + m[18]*v3 + m[19]);
            dst[x+2] = t0; dst[x+3] = t1;
        }
    }
    else
    {
        for( x = 0; x < len; x++, src += scn, dst += dcn )
        {
            const WT* _m = m;
            int j, k;
            for( j = 0; j < dcn; j++, _m += scn + 1 )
            {
                WT s = _m[scn];
                for( k = 0; k < scn; k++ )
                    s += _m[k]*src[k];
                dst[j] = (T)(s);
            }
        }
    }
}

#if CC_SSE2

static inline void
load3x3Matrix( const float* m, __m128& m0, __m128& m1, __m128& m2, __m128& m3 )
{
    m0 = _mm_setr_ps(m[0], m[4], m[8], 0);
    m1 = _mm_setr_ps(m[1], m[5], m[9], 0);
    m2 = _mm_setr_ps(m[2], m[6], m[10], 0);
    m3 = _mm_setr_ps(m[3], m[7], m[11], 0);
}

static inline void
load4x4Matrix( const float* m, __m128& m0, __m128& m1, __m128& m2, __m128& m3, __m128& m4 )
{
    m0 = _mm_setr_ps(m[0], m[5], m[10], m[15]);
    m1 = _mm_setr_ps(m[1], m[6], m[11], m[16]);
    m2 = _mm_setr_ps(m[2], m[7], m[12], m[17]);
    m3 = _mm_setr_ps(m[3], m[8], m[13], m[18]);
    m4 = _mm_setr_ps(m[4], m[9], m[14], m[19]);
}

#endif

static void
transform_8u( const uchar* src, uchar* dst, const float* m, int len, int scn, int dcn )
{
#if CC_SSE2_
    const int BITS = 10, SCALE = 1 << BITS;
    const float MAX_M = (float)(1 << (15 - BITS));

    if( USE_SSE2 && scn == 3 && dcn == 3 &&
        ABS(m[0]) < MAX_M && ABS(m[1]) < MAX_M && ABS(m[2]) < MAX_M && ABS(m[3]) < MAX_M*256 &&
        ABS(m[4]) < MAX_M && ABS(m[5]) < MAX_M && ABS(m[6]) < MAX_M && ABS(m[7]) < MAX_M*256 &&
        ABS(m[8]) < MAX_M && ABS(m[9]) < MAX_M && ABS(m[10]) < MAX_M && ABS(m[11]) < MAX_M*256 )
    {
        // faster fixed-point transformation
        short m00 = saturate_cast<short>(m[0]*SCALE), m01 = saturate_cast<short>(m[1]*SCALE),
            m02 = saturate_cast<short>(m[2]*SCALE), m10 = saturate_cast<short>(m[4]*SCALE),
            m11 = saturate_cast<short>(m[5]*SCALE), m12 = saturate_cast<short>(m[6]*SCALE),
            m20 = saturate_cast<short>(m[8]*SCALE), m21 = saturate_cast<short>(m[9]*SCALE),
            m22 = saturate_cast<short>(m[10]*SCALE);
        int m03 = saturate_cast<int>((m[3]+0.5f)*SCALE), m13 = saturate_cast<int>((m[7]+0.5f)*SCALE ),
            m23 = saturate_cast<int>((m[11]+0.5f)*SCALE);

        __m128i m0 = _mm_setr_epi16(0, m00, m01, m02, m00, m01, m02, 0);
        __m128i m1 = _mm_setr_epi16(0, m10, m11, m12, m10, m11, m12, 0);
        __m128i m2 = _mm_setr_epi16(0, m20, m21, m22, m20, m21, m22, 0);
        __m128i m3 = _mm_setr_epi32(m03, m13, m23, 0);
        int x = 0;

        for( ; x <= (len - 8)*3; x += 8*3 )
        {
            __m128i z = _mm_setzero_si128(), t0, t1, t2, r0, r1;
            __m128i v0 = _mm_loadl_epi64((const __m128i*)(src + x));
            __m128i v1 = _mm_loadl_epi64((const __m128i*)(src + x + 8));
            __m128i v2 = _mm_loadl_epi64((const __m128i*)(src + x + 16)), v3;
            v0 = _mm_unpacklo_epi8(v0, z); // b0 g0 r0 b1 g1 r1 b2 g2
            v1 = _mm_unpacklo_epi8(v1, z); // r2 b3 g3 r3 b4 g4 r4 b5
            v2 = _mm_unpacklo_epi8(v2, z); // g5 r5 b6 g6 r6 b7 g7 r7

            v3 = _mm_srli_si128(v2, 2); // ? b6 g6 r6 b7 g7 r7 0
            v2 = _mm_or_si128(_mm_slli_si128(v2, 10), _mm_srli_si128(v1, 6)); // ? b4 g4 r4 b5 g5 r5 ?
            v1 = _mm_or_si128(_mm_slli_si128(v1, 6), _mm_srli_si128(v0, 10)); // ? b2 g2 r2 b3 g3 r3 ?
            v0 = _mm_slli_si128(v0, 2); // 0 b0 g0 r0 b1 g1 r1 ?

            // process pixels 0 & 1
            t0 = _mm_madd_epi16(v0, m0); // a0 b0 a1 b1
            t1 = _mm_madd_epi16(v0, m1); // c0 d0 c1 d1
            t2 = _mm_madd_epi16(v0, m2); // e0 f0 e1 f1
            v0 = _mm_unpacklo_epi32(t0, t1); // a0 c0 b0 d0
            t0 = _mm_unpackhi_epi32(t0, t1); // a1 b1 c1 d1
            t1 = _mm_unpacklo_epi32(t2, z);  // e0 0 f0 0
            t2 = _mm_unpackhi_epi32(t2, z);  // e1 0 f1 0
            r0 = _mm_add_epi32(_mm_add_epi32(_mm_unpacklo_epi64(v0, t1), _mm_unpackhi_epi64(v0,t1)), m3); // B0 G0 R0 0
            r1 = _mm_add_epi32(_mm_add_epi32(_mm_unpacklo_epi64(t0, t2), _mm_unpackhi_epi64(t0,t2)), m3); // B1 G1 R1 0
            r0 = _mm_srai_epi32(r0, BITS);
            r1 = _mm_srai_epi32(r1, BITS);
            v0 = _mm_packus_epi16(_mm_packs_epi32(_mm_slli_si128(r0, 4), r1), z); // 0 B0 G0 R0 B1 G1 R1 0

            // process pixels 2 & 3
            t0 = _mm_madd_epi16(v1, m0); // a0 b0 a1 b1
            t1 = _mm_madd_epi16(v1, m1); // c0 d0 c1 d1
            t2 = _mm_madd_epi16(v1, m2); // e0 f0 e1 f1
            v1 = _mm_unpacklo_epi32(t0, t1); // a0 c0 b0 d0
            t0 = _mm_unpackhi_epi32(t0, t1); // a1 b1 c1 d1
            t1 = _mm_unpacklo_epi32(t2, z);  // e0 0 f0 0
            t2 = _mm_unpackhi_epi32(t2, z);  // e1 0 f1 0
            r0 = _mm_add_epi32(_mm_add_epi32(_mm_unpacklo_epi64(v1, t1), _mm_unpackhi_epi64(v1,t1)), m3); // B2 G2 R2 0
            r1 = _mm_add_epi32(_mm_add_epi32(_mm_unpacklo_epi64(t0, t2), _mm_unpackhi_epi64(t0,t2)), m3); // B3 G3 R3 0
            r0 = _mm_srai_epi32(r0, BITS);
            r1 = _mm_srai_epi32(r1, BITS);
            v1 = _mm_packus_epi16(_mm_packs_epi32(_mm_slli_si128(r0, 4), r1), z); // 0 B2 G2 R2 B3 G3 R3 0

            // process pixels 4 & 5
            t0 = _mm_madd_epi16(v2, m0); // a0 b0 a1 b1
            t1 = _mm_madd_epi16(v2, m1); // c0 d0 c1 d1
            t2 = _mm_madd_epi16(v2, m2); // e0 f0 e1 f1
            v2 = _mm_unpacklo_epi32(t0, t1); // a0 c0 b0 d0
            t0 = _mm_unpackhi_epi32(t0, t1); // a1 b1 c1 d1
            t1 = _mm_unpacklo_epi32(t2, z);  // e0 0 f0 0
            t2 = _mm_unpackhi_epi32(t2, z);  // e1 0 f1 0
            r0 = _mm_add_epi32(_mm_add_epi32(_mm_unpacklo_epi64(v2, t1), _mm_unpackhi_epi64(v2,t1)), m3); // B4 G4 R4 0
            r1 = _mm_add_epi32(_mm_add_epi32(_mm_unpacklo_epi64(t0, t2), _mm_unpackhi_epi64(t0,t2)), m3); // B5 G5 R5 0
            r0 = _mm_srai_epi32(r0, BITS);
            r1 = _mm_srai_epi32(r1, BITS);
            v2 = _mm_packus_epi16(_mm_packs_epi32(_mm_slli_si128(r0, 4), r1), z); // 0 B4 G4 R4 B5 G5 R5 0

            // process pixels 6 & 7
            t0 = _mm_madd_epi16(v3, m0); // a0 b0 a1 b1
            t1 = _mm_madd_epi16(v3, m1); // c0 d0 c1 d1
            t2 = _mm_madd_epi16(v3, m2); // e0 f0 e1 f1
            v3 = _mm_unpacklo_epi32(t0, t1); // a0 c0 b0 d0
            t0 = _mm_unpackhi_epi32(t0, t1); // a1 b1 c1 d1
            t1 = _mm_unpacklo_epi32(t2, z);  // e0 0 f0 0
            t2 = _mm_unpackhi_epi32(t2, z);  // e1 0 f1 0
            r0 = _mm_add_epi32(_mm_add_epi32(_mm_unpacklo_epi64(v3, t1), _mm_unpackhi_epi64(v3,t1)), m3); // B6 G6 R6 0
            r1 = _mm_add_epi32(_mm_add_epi32(_mm_unpacklo_epi64(t0, t2), _mm_unpackhi_epi64(t0,t2)), m3); // B7 G7 R7 0
            r0 = _mm_srai_epi32(r0, BITS);
            r1 = _mm_srai_epi32(r1, BITS);
            v3 = _mm_packus_epi16(_mm_packs_epi32(_mm_slli_si128(r0, 4), r1), z); // 0 B6 G6 R6 B7 G7 R7 0

            v0 = _mm_or_si128(_mm_srli_si128(v0, 1), _mm_slli_si128(v1, 5));
            v1 = _mm_or_si128(_mm_srli_si128(v1, 3), _mm_slli_si128(v2, 3));
            v2 = _mm_or_si128(_mm_srli_si128(v2, 5), _mm_slli_si128(v3, 1));
            _mm_storel_epi64((__m128i*)(dst + x), v0);
            _mm_storel_epi64((__m128i*)(dst + x + 8), v1);
            _mm_storel_epi64((__m128i*)(dst + x + 16), v2);
        }

        for( ; x < len*3; x += 3 )
        {
            int v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            uchar t0 = saturate_cast<uchar>((m00*v0 + m01*v1 + m02*v2 + m03)>>BITS);
            uchar t1 = saturate_cast<uchar>((m10*v0 + m11*v1 + m12*v2 + m13)>>BITS);
            uchar t2 = saturate_cast<uchar>((m20*v0 + m21*v1 + m22*v2 + m23)>>BITS);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        return;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}

static void
transform_16u( const ushort* src, ushort* dst, const float* m, int len, int scn, int dcn )
{
#if CC_SSE2_
    if( USE_SSE2 && scn == 3 && dcn == 3 )
    {
        __m128 m0, m1, m2, m3;
        __m128i delta = _mm_setr_epi16(0,-32768,-32768,-32768,-32768,-32768,-32768,0);
        load3x3Matrix(m, m0, m1, m2, m3);
        m3 = _mm_sub_ps(m3, _mm_setr_ps(32768.f, 32768.f, 32768.f, 0.f));

        int x = 0;
        for( ; x <= (len - 4)*3; x += 4*3 )
        {
            __m128i z = _mm_setzero_si128();
            __m128i v0 = _mm_loadu_si128((const __m128i*)(src + x)), v1;
            __m128i v2 = _mm_loadl_epi64((const __m128i*)(src + x + 8)), v3;
            v1 = _mm_unpacklo_epi16(_mm_srli_si128(v0, 6), z); // b1 g1 r1
            v3 = _mm_unpacklo_epi16(_mm_srli_si128(v2, 2), z); // b3 g3 r3
            v2 = _mm_or_si128(_mm_srli_si128(v0, 12), _mm_slli_si128(v2, 4));
            v0 = _mm_unpacklo_epi16(v0, z); // b0 g0 r0
            v2 = _mm_unpacklo_epi16(v2, z); // b2 g2 r2
            __m128 x0 = _mm_cvtepi32_ps(v0), x1 = _mm_cvtepi32_ps(v1);
            __m128 x2 = _mm_cvtepi32_ps(v2), x3 = _mm_cvtepi32_ps(v3);
            __m128 y0 = _mm_add_ps(_mm_add_ps(_mm_add_ps(
                        _mm_mul_ps(m0, _mm_shuffle_ps(x0,x0,_MM_SHUFFLE(0,0,0,0))),
                        _mm_mul_ps(m1, _mm_shuffle_ps(x0,x0,_MM_SHUFFLE(1,1,1,1)))),
                        _mm_mul_ps(m2, _mm_shuffle_ps(x0,x0,_MM_SHUFFLE(2,2,2,2)))), m3);
            __m128 y1 = _mm_add_ps(_mm_add_ps(_mm_add_ps(
                        _mm_mul_ps(m0, _mm_shuffle_ps(x1,x1,_MM_SHUFFLE(0,0,0,0))),
                        _mm_mul_ps(m1, _mm_shuffle_ps(x1,x1,_MM_SHUFFLE(1,1,1,1)))),
                        _mm_mul_ps(m2, _mm_shuffle_ps(x1,x1,_MM_SHUFFLE(2,2,2,2)))), m3);
            __m128 y2 = _mm_add_ps(_mm_add_ps(_mm_add_ps(
                        _mm_mul_ps(m0, _mm_shuffle_ps(x2,x2,_MM_SHUFFLE(0,0,0,0))),
                        _mm_mul_ps(m1, _mm_shuffle_ps(x2,x2,_MM_SHUFFLE(1,1,1,1)))),
                        _mm_mul_ps(m2, _mm_shuffle_ps(x2,x2,_MM_SHUFFLE(2,2,2,2)))), m3);
            __m128 y3 = _mm_add_ps(_mm_add_ps(_mm_add_ps(
                        _mm_mul_ps(m0, _mm_shuffle_ps(x3,x3,_MM_SHUFFLE(0,0,0,0))),
                        _mm_mul_ps(m1, _mm_shuffle_ps(x3,x3,_MM_SHUFFLE(1,1,1,1)))),
                        _mm_mul_ps(m2, _mm_shuffle_ps(x3,x3,_MM_SHUFFLE(2,2,2,2)))), m3);
            v0 = _mm_cvtps_epi32(y0); v1 = _mm_cvtps_epi32(y1);
            v2 = _mm_cvtps_epi32(y2); v3 = _mm_cvtps_epi32(y3);

            v0 = _mm_add_epi16(_mm_packs_epi32(_mm_slli_si128(v0,4), v1), delta); // 0 b0 g0 r0 b1 g1 r1 0
            v2 = _mm_add_epi16(_mm_packs_epi32(_mm_slli_si128(v2,4), v3), delta); // 0 b2 g2 r2 b3 g3 r3 0
            v1 = _mm_or_si128(_mm_srli_si128(v0,2), _mm_slli_si128(v2,10)); // b0 g0 r0 b1 g1 r1 b2 g2
            v2 = _mm_srli_si128(v2, 6); // r2 b3 g3 r3 0 0 0 0
            _mm_storeu_si128((__m128i*)(dst + x), v1);
            _mm_storel_epi64((__m128i*)(dst + x + 8), v2);
        }

        for( ; x < len*3; x += 3 )
        {
            float v0 = src[x], v1 = src[x+1], v2 = src[x+2];
            ushort t0 = saturate_cast<ushort>(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]);
            ushort t1 = saturate_cast<ushort>(m[4]*v0 + m[5]*v1 + m[6]*v2 + m[7]);
            ushort t2 = saturate_cast<ushort>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
        return;
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}


static void
transform_32f( const float* src, float* dst, const float* m, int len, int scn, int dcn )
{
#if CC_SSE2_
    if( USE_SSE2 )
    {
        int x = 0;
        if( scn == 3 && dcn == 3 )
        {
            __m128 m0, m1, m2, m3;
            load3x3Matrix(m, m0, m1, m2, m3);

            for( ; x < (len - 1)*3; x += 3 )
            {
                __m128 x0 = _mm_loadu_ps(src + x);
                __m128 y0 = _mm_add_ps(_mm_add_ps(_mm_add_ps(
                            _mm_mul_ps(m0, _mm_shuffle_ps(x0,x0,_MM_SHUFFLE(0,0,0,0))),
                            _mm_mul_ps(m1, _mm_shuffle_ps(x0,x0,_MM_SHUFFLE(1,1,1,1)))),
                            _mm_mul_ps(m2, _mm_shuffle_ps(x0,x0,_MM_SHUFFLE(2,2,2,2)))), m3);
                _mm_storel_pi((__m64*)(dst + x), y0);
                _mm_store_ss(dst + x + 2, _mm_movehl_ps(y0,y0));
            }

            for( ; x < len*3; x += 3 )
            {
                float v0 = src[x], v1 = src[x+1], v2 = src[x+2];
                float t0 = saturate_cast<float>(m[0]*v0 + m[1]*v1 + m[2]*v2 + m[3]);
                float t1 = saturate_cast<float>(m[4]*v0 + m[5]*v1 + m[6]*v2 + m[7]);
                float t2 = saturate_cast<float>(m[8]*v0 + m[9]*v1 + m[10]*v2 + m[11]);
                dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
            }
            return;
        }

        if( scn == 4 && dcn == 4 )
        {
            __m128 m0, m1, m2, m3, m4;
            load4x4Matrix(m, m0, m1, m2, m3, m4);

            for( ; x < len*4; x += 4 )
            {
                __m128 x0 = _mm_loadu_ps(src + x);
                __m128 y0 = _mm_add_ps(_mm_add_ps(_mm_add_ps(_mm_add_ps(
                                    _mm_mul_ps(m0, _mm_shuffle_ps(x0,x0,_MM_SHUFFLE(0,0,0,0))),
                                    _mm_mul_ps(m1, _mm_shuffle_ps(x0,x0,_MM_SHUFFLE(1,1,1,1)))),
                                    _mm_mul_ps(m2, _mm_shuffle_ps(x0,x0,_MM_SHUFFLE(2,2,2,2)))),
                                    _mm_mul_ps(m3, _mm_shuffle_ps(x0,x0,_MM_SHUFFLE(3,3,3,3)))), m4);
                _mm_storeu_ps(dst + x, y0);
            }
            return;
        }
    }
#endif

    transform_(src, dst, m, len, scn, dcn);
}


static void
transform_8s(const schar* src, schar* dst, const float* m, int len, int scn, int dcn)
{
    transform_(src, dst, m, len, scn, dcn);
}

static void
transform_16s(const short* src, short* dst, const float* m, int len, int scn, int dcn)
{
    transform_(src, dst, m, len, scn, dcn);
}

static void
transform_32s(const int* src, int* dst, const double* m, int len, int scn, int dcn)
{
    transform_(src, dst, m, len, scn, dcn);
}

static void
transform_64f(const double* src, double* dst, const double* m, int len, int scn, int dcn)
{
    transform_(src, dst, m, len, scn, dcn);
}

template<typename T, typename WT> static void
diagtransform_( const T* src, T* dst, const WT* m, int len, int cn, int )
{
    int x;

    if( cn == 2 )
    {
        for( x = 0; x < len*2; x += 2 )
        {
            T t0 = (T)(m[0]*src[x] + m[2]);
            T t1 = (T)(m[4]*src[x+1] + m[5]);
            dst[x] = t0; dst[x+1] = t1;
        }
    }
    else if( cn == 3 )
    {
        for( x = 0; x < len*3; x += 3 )
        {
            T t0 = (T)(m[0]*src[x] + m[3]);
            T t1 = (T)(m[5]*src[x+1] + m[7]);
            T t2 = (T)(m[10]*src[x+2] + m[11]);
            dst[x] = t0; dst[x+1] = t1; dst[x+2] = t2;
        }
    }
    else if( cn == 4 )
    {
        for( x = 0; x < len*4; x += 4 )
        {
            T t0 = (T)(m[0]*src[x] + m[4]);
            T t1 = (T)(m[6]*src[x+1] + m[9]);
            dst[x] = t0; dst[x+1] = t1;
            t0 = (T)(m[12]*src[x+2] + m[14]);
            t1 = (T)(m[18]*src[x+3] + m[19]);
            dst[x+2] = t0; dst[x+3] = t1;
        }
    }
    else
    {
        for( x = 0; x < len; x++, src += cn, dst += cn )
        {
            const WT* _m = m;
            for( int j = 0; j < cn; j++, _m += cn + 1 )
                dst[j] = (T)(src[j]*_m[j] + _m[cn]);
        }
    }
}

static void
diagtransform_8u(const uchar* src, uchar* dst, const float* m, int len, int scn, int dcn)
{
    diagtransform_(src, dst, m, len, scn, dcn);
}

static void
diagtransform_8s(const schar* src, schar* dst, const float* m, int len, int scn, int dcn)
{
    diagtransform_(src, dst, m, len, scn, dcn);
}

static void
diagtransform_16u(const ushort* src, ushort* dst, const float* m, int len, int scn, int dcn)
{
    diagtransform_(src, dst, m, len, scn, dcn);
}

static void
diagtransform_16s(const short* src, short* dst, const float* m, int len, int scn, int dcn)
{
    diagtransform_(src, dst, m, len, scn, dcn);
}

static void
diagtransform_32s(const int* src, int* dst, const double* m, int len, int scn, int dcn)
{
    diagtransform_(src, dst, m, len, scn, dcn);
}

static void
diagtransform_32f(const float* src, float* dst, const float* m, int len, int scn, int dcn)
{
    diagtransform_(src, dst, m, len, scn, dcn);
}

static void
diagtransform_64f(const double* src, double* dst, const double* m, int len, int scn, int dcn)
{
    diagtransform_(src, dst, m, len, scn, dcn);
}


typedef void (*TransformFunc)( const uchar* src, uchar* dst, const uchar* m, int, int, int );

static TransformFunc getTransformFunc(int type)
{
    static TransformFunc transformTab[] =
    {
        (TransformFunc)transform_8u, (TransformFunc)transform_8s, (TransformFunc)transform_16u,
        (TransformFunc)transform_16s, (TransformFunc)transform_32s, (TransformFunc)transform_32f,
        (TransformFunc)transform_64f, 0
    };

    return transformTab[type];
}

static TransformFunc getDiagTransformFunc(int type)
{
    static TransformFunc diagTransformTab[] =
    {
        (TransformFunc)diagtransform_8u, (TransformFunc)diagtransform_8s, (TransformFunc)diagtransform_16u,
        (TransformFunc)diagtransform_16s, (TransformFunc)diagtransform_32s, (TransformFunc)diagtransform_32f,
        (TransformFunc)diagtransform_64f, 0
    };

    return diagTransformTab[type];
}

#if 0
void transform( const img_t* _src, img_t* _dst, const img_t* _mtx )
{
    CC_INSTRUMENT_REGION()

    const img_t *src = _src;
    const img_t *m = _mtx;
    TypeId type = CC_MAT_DEPTH(src);
    int scn = CC_MAT_CN(src), dcn = m->rows;
    CC_Assert( scn == m->cols || scn + 1 == m->cols );
    bool isDiag = false;

    cvSetMat( _dst, src->rows, src->cols, type, dcn );
    img_t* dst = _dst;

    TypeId mtype = type == CC_32S || type == CC_64F ? CC_64F : CC_32F;
    double* _mbuf = NULL;
    double* mbuf;

    if( !CC_IS_CONT_MAT(m) || CC_MAT_TYPE(m) != mtype || m->cols != scn + 1 )
    {
        _mbuf = MALLOC(double, dcn*(scn+1));
        mbuf = (double*)_mbuf;
        img_t tmp[1] = {0};
        cvMat(tmp, dcn, scn+1, mtype, 1, mbuf);
        memset(tmp->tt.data, 0, CC_MAT_TOTAL(tmp)*CC_TYPE_SIZE(mtype));
        if( m->cols == scn+1 )
            cvConvertTo(m, tmp, mtype);
        else
        {
            img_t tmppart[1] = {0};
            cvGetCols(tmp, 0, m->cols);
            cvConvertTo(m, tmppart, mtype);
        }
        m = tmp;
    }
    else
        mbuf = img_ptr(double, m);

    if( scn == dcn )
    {
        int i, j;
        double eps = mtype == CC_32F ? FLT_EPSILON : DBL_EPSILON;

        if( scn == 1 )
        {
            double alpha, beta;
            if( mtype == CC_32F )
                alpha = img_ptr(float, m)[0], beta = img_ptr(float, m)[1];
            else
                alpha = img_ptr(double, m)[0], beta = img_ptr(double, m)[1];
            cvConvertTo(src, dst, CC_MAT_TYPE(dst), alpha, beta);
            return;
        }

        for( i = 0, isDiag = true; isDiag && i < scn; i++ )
        {
            for( j = 0; isDiag && j < scn; j++ )
            {
                double v = mtype == CC_32F ? *img_at(float, m, i, j) : *img_at(double, m, i, j);
                if( i != j && fabs(v) > eps )
                    isDiag = false;
            }
        }
    }

    TransformFunc func = isDiag ? getDiagTransformFunc(type): getTransformFunc(type);
    CC_Assert( func != 0 );

    const img_t* arrays[] = {src, dst, 0};
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    for( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (int)total, scn, dcn );
    FREE(_mbuf);
}

/****************************************************************************************\
*                                  Perspective Transform                                 *
\****************************************************************************************/



template<typename T> static void
perspectiveTransform_( const T* src, T* dst, const double* m, int len, int scn, int dcn )
{
    const double eps = FLT_EPSILON;
    int i;

    if( scn == 2 && dcn == 2 )
    {
        for( i = 0; i < len*2; i += 2 )
        {
            T x = src[i], y = src[i + 1];
            double w = x*m[6] + y*m[7] + m[8];

            if( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + m[2])*w);
                dst[i+1] = (T)((x*m[3] + y*m[4] + m[5])*w);
            }
            else
                dst[i] = dst[i+1] = (T)0;
        }
    }
    else if( scn == 3 && dcn == 3 )
    {
        for( i = 0; i < len*3; i += 3 )
        {
            T x = src[i], y = src[i + 1], z = src[i + 2];
            double w = x*m[12] + y*m[13] + z*m[14] + m[15];

            if( fabs(w) > eps )
            {
                w = 1./w;
                dst[i] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3]) * w);
                dst[i+1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7]) * w);
                dst[i+2] = (T)((x*m[8] + y*m[9] + z*m[10] + m[11]) * w);
            }
            else
                dst[i] = dst[i+1] = dst[i+2] = (T)0;
        }
    }
    else if( scn == 3 && dcn == 2 )
    {
        for( i = 0; i < len; i++, src += 3, dst += 2 )
        {
            T x = src[0], y = src[1], z = src[2];
            double w = x*m[8] + y*m[9] + z*m[10] + m[11];

            if( fabs(w) > eps )
            {
                w = 1./w;
                dst[0] = (T)((x*m[0] + y*m[1] + z*m[2] + m[3])*w);
                dst[1] = (T)((x*m[4] + y*m[5] + z*m[6] + m[7])*w);
            }
            else
                dst[0] = dst[1] = (T)0;
        }
    }
    else
    {
        for( i = 0; i < len; i++, src += scn, dst += dcn )
        {
            const double* _m = m + dcn*(scn + 1);
            double w = _m[scn];
            int j, k;
            for( k = 0; k < scn; k++ )
                w += _m[k]*src[k];
            if( fabs(w) > eps )
            {
                _m = m;
                for( j = 0; j < dcn; j++, _m += scn + 1 )
                {
                    double s = _m[scn];
                    for( k = 0; k < scn; k++ )
                        s += _m[k]*src[k];
                    dst[j] = (T)(s*w);
                }
            }
            else
                for( j = 0; j < dcn; j++ )
                    dst[j] = 0;
        }
    }
}


static void
perspectiveTransform_32f(const float* src, float* dst, const double* m, int len, int scn, int dcn)
{
    perspectiveTransform_(src, dst, m, len, scn, dcn);
}

static void
perspectiveTransform_64f(const double* src, double* dst, const double* m, int len, int scn, int dcn)
{
    perspectiveTransform_(src, dst, m, len, scn, dcn);
}



void perspectiveTransform( const img_t* _src, img_t* _dst, const img_t* _mtx )
{
    CC_INSTRUMENT_REGION()

    img_t *src = _src, m = _mtx;
    int type = src.type(), scn = src CC_MAT_CN(), dcn = m->rows-1;
    CC_Assert( scn + 1 == m->cols );
    CC_Assert( type == CC_32F || type == CC_64F );

    _dst cvSetMat( cvGetSize(src), CC_MAKETYPE(type, dcn) );
    img_t *dst = _dst;

    const int mtype = CC_64F;
    FREE(); double* = MALLOC(double, ) _mbuf;
    double* mbuf = _mbuf;

    if( !m CC_IS_CONT_MAT() || m CC_MAT_TYPE() != mtype )
    {
        _mbuf.allocate((dcn+1)*(scn+1));
        img_t tmp(dcn+1, scn+1, mtype, (double*)_mbuf);
        m.convertTo(tmp, mtype);
        m = tmp;
    }
    else
        mbuf = m img_ptr(double>();

    TransformFunc func = type == CC_32F ?
        (TransformFunc)perspectiveTransform_32f :
        (TransformFunc)perspectiveTransform_64f;
    CC_Assert( func != 0 );

    const img_t* arrays[] = {src, dst, 0};
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs);
    size_t i, total = it.size;

    for( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], (uchar*)mbuf, (int)total, scn, dcn );
}

/****************************************************************************************\
*                                       ScaleAdd                                         *
\****************************************************************************************/



static void scaleAdd_32f(const float* src1, const float* src2, float* dst,
                         int len, float* _alpha)
{
    float alpha = *_alpha;
    int i = 0;
#if CC_SSE2
    if( USE_SSE2 )
    {
        __m128 a4 = _mm_set1_ps(alpha);
        if( (((size_t)src1|(size_t)src2|(size_t)dst) & 15) == 0 )
            for( ; i <= len - 8; i += 8 )
            {
                __m128 x0, x1, y0, y1, t0, t1;
                x0 = _mm_load_ps(src1 + i); x1 = _mm_load_ps(src1 + i + 4);
                y0 = _mm_load_ps(src2 + i); y1 = _mm_load_ps(src2 + i + 4);
                t0 = _mm_add_ps(_mm_mul_ps(x0, a4), y0);
                t1 = _mm_add_ps(_mm_mul_ps(x1, a4), y1);
                _mm_store_ps(dst + i, t0);
                _mm_store_ps(dst + i + 4, t1);
            }
        else
            for( ; i <= len - 8; i += 8 )
            {
                __m128 x0, x1, y0, y1, t0, t1;
                x0 = _mm_loadu_ps(src1 + i); x1 = _mm_loadu_ps(src1 + i + 4);
                y0 = _mm_loadu_ps(src2 + i); y1 = _mm_loadu_ps(src2 + i + 4);
                t0 = _mm_add_ps(_mm_mul_ps(x0, a4), y0);
                t1 = _mm_add_ps(_mm_mul_ps(x1, a4), y1);
                _mm_storeu_ps(dst + i, t0);
                _mm_storeu_ps(dst + i + 4, t1);
            }
    }
    else
#elif CC_NEON
    if (true)
    {
        for ( ; i <= len - 4; i += 4)
        {
            float32x4_t v_src1 = vld1q_f32(src1 + i), v_src2 = vld1q_f32(src2 + i);
            vst1q_f32(dst + i, vaddq_f32(vmulq_n_f32(v_src1, alpha), v_src2));
        }
    }
    else
#endif
    //vz why do we need unroll here?
    for( ; i <= len - 4; i += 4 )
    {
        float t0, t1;
        t0 = src1[i]*alpha + src2[i];
        t1 = src1[i+1]*alpha + src2[i+1];
        dst[i] = t0; dst[i+1] = t1;
        t0 = src1[i+2]*alpha + src2[i+2];
        t1 = src1[i+3]*alpha + src2[i+3];
        dst[i+2] = t0; dst[i+3] = t1;
    }
    for(; i < len; i++ )
        dst[i] = src1[i]*alpha + src2[i];
}


static void scaleAdd_64f(const double* src1, const double* src2, double* dst,
                         int len, double* _alpha)
{
    double alpha = *_alpha;
    int i = 0;
#if CC_SSE2
    if( USE_SSE2 && (((size_t)src1|(size_t)src2|(size_t)dst) & 15) == 0 )
    {
        __m128d a2 = _mm_set1_pd(alpha);
        for( ; i <= len - 4; i += 4 )
        {
            __m128d x0, x1, y0, y1, t0, t1;
            x0 = _mm_load_pd(src1 + i); x1 = _mm_load_pd(src1 + i + 2);
            y0 = _mm_load_pd(src2 + i); y1 = _mm_load_pd(src2 + i + 2);
            t0 = _mm_add_pd(_mm_mul_pd(x0, a2), y0);
            t1 = _mm_add_pd(_mm_mul_pd(x1, a2), y1);
            _mm_store_pd(dst + i, t0);
            _mm_store_pd(dst + i + 2, t1);
        }
    }
    else
#endif
     //vz why do we need unroll here?
    for( ; i <= len - 4; i += 4 )
    {
        double t0, t1;
        t0 = src1[i]*alpha + src2[i];
        t1 = src1[i+1]*alpha + src2[i+1];
        dst[i] = t0; dst[i+1] = t1;
        t0 = src1[i+2]*alpha + src2[i+2];
        t1 = src1[i+3]*alpha + src2[i+3];
        dst[i+2] = t0; dst[i+3] = t1;
    }
    for(; i < len; i++ )
        dst[i] = src1[i]*alpha + src2[i];
}

typedef void (*ScaleAddFunc)(const uchar* src1, const uchar* src2, uchar* dst, int len, const void* alpha);

#ifdef HAVE_OPENCL

static bool ocl_scaleAdd( const img_t* _src1, double alpha, const img_t* _src2, img_t* _dst, int type )
{
    const ocl::Device & d = ocl::Device::getDefault();

    bool doubleSupport = d.doubleFPConfig() > 0;
    CSize size = _src1.size();
    if ( (!doubleSupport && type == CC_64F) || size != _src2.size() )
        return false;

    _dst cvSetMat(size, type);
    int cn = CC_MAT_CN(type), wdepth = MAX(type, CC_32F);
    int kercn = ocl::predictOptimalVectorWidthMax(_src1, _src2, _dst),
        rowsPerWI = d.isIntel() ? 4 : 1;

    char cvt[2][50];
    ocl::Kernel k("KF", ocl::core::arithm_oclsrc,
                  format("-D OP_SCALE_ADD -D BINARY_OP -D dstT=%s -D workT=%s -D convertToWT1=%s"
                         " -D srcT1=dstT -D srcT2=dstT -D convertToDT=%s -D workT1=%s"
                         " -D wdepth=%d%s -D rowsPerWI=%d",
                         ocl::typeToStr(CC_MAKE_TYPE(type, kercn)),
                         ocl::typeToStr(CC_MAKE_TYPE(wdepth, kercn)),
                         ocl::convertTypeStr(type, wdepth, kercn, cvt[0]),
                         ocl::convertTypeStr(wdepth, type, kercn, cvt[1]),
                         ocl::typeToStr(wdepth), wdepth,
                         doubleSupport ? " -D DOUBLE_SUPPORT" : "", rowsPerWI));
    if (k.empty())
        return false;

    UMat src1 = _src1.getUMat(), src2 = _src2.getUMat(), dst = _dst.getUMat();

    ocl::KernelArg src1arg = ocl::KernelArg::ReadOnlyNoSize(src1),
            src2arg = ocl::KernelArg::ReadOnlyNoSize(src2),
            dstarg = ocl::KernelArg::WriteOnly(dst, cn, kercn);

    if (wdepth == CC_32F)
        k.args(src1arg, src2arg, dstarg, (float)alpha);
    else
        k.args(src1arg, src2arg, dstarg, alpha);

    size_t globalsize[2] = { (size_t)dst->cols * cn / kercn, ((size_t)dst->rows + rowsPerWI - 1) / rowsPerWI };
    return k.run(2, globalsize, NULL, false);
}

#endif



void scaleAdd( const img_t* _src1, double alpha, const img_t* _src2, img_t* _dst )
{
    CC_INSTRUMENT_REGION()

    int type = _src1 CC_MAT_TYPE(), type = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type);
    CC_Assert( type == _src2 CC_MAT_TYPE() );

    CC_OCL_RUN(_src1.dims() <= 2 && _src2.dims() <= 2 && _dst.isUMat(),
            ocl_scaleAdd(_src1, alpha, _src2, _dst, type))

    if( type < CC_32F )
    {
        addWeighted(_src1, alpha, _src2, 1, 0, _dst, type);
        return;
    }

    img_t src1 = _src1, src2 = _src2;
    CC_Assert(src1.size == src2.size);

    _dst cvSetMat(src1.dims, src1.size, type);
    img_t *dst = _dst;

    float falpha = (float)alpha;
    void* palpha = type == CC_32F ? (void*)&falpha : (void*)&alpha;

    ScaleAddFunc func = type == CC_32F ? (ScaleAddFunc)scaleAdd_32f : (ScaleAddFunc)scaleAdd_64f;

    if (src1 CC_IS_CONT_MAT() && src2 CC_IS_CONT_MAT() && dst CC_IS_CONT_MAT())
    {
        size_t len = src1 CC_MAT_TOTAL()*cn;
        func(src1->tt.data, src2->tt.data, dst->tt.data, (int)len, palpha);
        return;
    }

    const img_t* arrays[] = {&src1, &src2, &dst, 0};
    uchar* ptrs[3];
    NAryMatIterator it(arrays, ptrs);
    size_t i, len = it.size*cn;

    for( i = 0; i < it.nplanes; i++, ++it )
        func( ptrs[0], ptrs[1], ptrs[2], (int)len, palpha );
}

/****************************************************************************************\
*                                 Covariation Matrix                                     *
\****************************************************************************************/

void calcCovarMatrix( const img_t* data, int nsamples, img_t* covar, img_t* _mean, int flags, int ctype )
{
    CC_INSTRUMENT_REGION()

    CC_Assert( data && nsamples > 0 );
    CSize size = data[0].size();
    int sz = size.width * size.height, esz = (int)data[0] CC_TYPE_SIZE();
    int type = data[0] CC_MAT_TYPE();
    img_t mean;
    ctype = MAX(MAX(CC_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.type()), CC_32F);

    if( (flags & CC_COVAR_USE_AVG) != 0 )
    {
        CC_Assert( _mean.size() == size );
        if( _mean CC_IS_CONT_MAT() && _mean CC_MAT_TYPE() == ctype )
            mean = _mean cvReshape(,1, 1);
        else
        {
            _mean.convertTo(mean, ctype);
            mean = mean cvReshape(,1, 1);
        }
    }

    img_t _data(nsamples, sz, type);

    for( int i = 0; i < nsamples; i++ )
    {
        CC_Assert( data[i].size() == size && data[i] CC_MAT_TYPE() == type );
        if( data[i] CC_IS_CONT_MAT() )
            memcpy( _data.ptr(i), data[i]->tt.data, sz*esz );
        else
        {
            img_t dataRow(size.height, size.width, type, _data.ptr(i));
            data[i].copyTo(dataRow);
        }
    }

    calcCovarMatrix( _data, covar, mean, (flags & ~(CC_COVAR_ROWS|CC_COVAR_COLS)) | CC_COVAR_ROWS, ctype );
    if( (flags & CC_COVAR_USE_AVG) == 0 )
        _mean = mean cvReshape(,1, size.height);
}

void calcCovarMatrix( const img_t* _src, img_t* _covar, InputOutputArray _mean, int flags, int ctype )
{
    CC_INSTRUMENT_REGION()

    if(_src.kind() == _InputArray::STD_VECTOR_MAT)
    {
        std::vector<img_t> src;
        _src.getMatVector(src);

        CC_Assert( cvGetSize(src) > 0 );

        CSize size = src[0].size();
        int type = src[0] CC_MAT_TYPE();

        ctype = MAX(MAX(CC_MAT_DEPTH(ctype >= 0 ? ctype : type), _mean.type()), CC_32F);

        img_t _data(static_cast<int>(cvGetSize(src)), size.area(), type);

        int i = 0;
        for(std::vector<img_t>::iterator each = src.begin(); each != src.end(); ++each, ++i )
        {
            CC_Assert( (*each).size() == size && (*each) CC_MAT_TYPE() == type );
            img_t dataRow(size.height, size.width, type, _data.ptr(i));
            (*each).copyTo(dataRow);
        }

        img_t mean;
        if( (flags & CC_COVAR_USE_AVG) != 0 )
        {
            CC_Assert( _mean.size() == size );

            if( mean CC_MAT_TYPE() != ctype )
            {
                mean = _mean;
                _mean cvSetMat(mean.size(), ctype);
                img_t tmp = _mean;
                mean.convertTo(tmp, ctype);
                mean = tmp;
            }

            mean = _mean cvReshape(,1, 1);
        }

        calcCovarMatrix( _data, _covar, mean, (flags & ~(CC_COVAR_ROWS|CC_COVAR_COLS)) | CC_COVAR_ROWS, ctype );

        if( (flags & CC_COVAR_USE_AVG) == 0 )
        {
            mean = mean cvReshape(,1, size.height);
            mean.copyTo(_mean);
        }
        return;
    }

    img_t data = _src, mean;
    CC_Assert( ((flags & CC_COVAR_ROWS) != 0) ^ ((flags & CC_COVAR_COLS) != 0) );
    bool takeRows = (flags & CC_COVAR_ROWS) != 0;
    int type = data CC_MAT_TYPE();
    int nsamples = takeRows ? data->rows : data->cols;
    CC_Assert( nsamples > 0 );
    CSize size = takeRows ? cSize(data->cols, 1) : cSize(1, data->rows);

    if( (flags & CC_COVAR_USE_AVG) != 0 )
    {
        mean = _mean;
        ctype = MAX(MAX(CC_MAT_DEPTH(ctype >= 0 ? ctype : type), mean.type()), CC_32F);
        CC_Assert( mean.size() == size );
        if( mean CC_MAT_TYPE() != ctype )
        {
            _mean cvSetMat(mean.size(), ctype);
            img_t tmp = _mean;
            mean.convertTo(tmp, ctype);
            mean = tmp;
        }
    }
    else
    {
        ctype = MAX(CC_MAT_DEPTH(ctype >= 0 ? ctype : type), CC_32F);
        reduce( _src, _mean, takeRows ? 0 : 1, CC_REDUCE_AVG, ctype );
        mean = _mean;
    }

    mulTransposed( data, _covar, ((flags & CC_COVAR_NORMAL) == 0) ^ takeRows,
        mean, (flags & CC_COVAR_SCALE) != 0 ? 1./nsamples : 1, ctype );
}

/****************************************************************************************\
*                                        Mahalanobis                                     *
\****************************************************************************************/

double Mahalanobis( const img_t* _v1, const img_t* _v2, const img_t* _icovar )
{
    CC_INSTRUMENT_REGION()

    img_t v1 = _v1, v2 = _v2, icovar = _icovar;
    int type = v1 CC_MAT_TYPE(), type = v1.type();
    CSize sz = v1.size();
    int i, j, len = sz.width*sz.height*v1 CC_MAT_CN();
    FREE(); double* = MALLOC(double, ) buf(len);
    double result = 0;

    CC_Assert( type == v2 CC_MAT_TYPE() && type == icovar CC_MAT_TYPE() &&
        sz == v2.size() && len == icovar->rows && len == icovar->cols );

    sz.width *= v1 CC_MAT_CN();
    if( v1 CC_IS_CONT_MAT() && v2 CC_IS_CONT_MAT() )
    {
        sz.width *= sz.height;
        sz.height = 1;
    }

    if( type == CC_32F )
    {
        const float* src1 = v1 img_ptr(float>();
        const float* src2 = v2 img_ptr(float>();
        size_t step1 = v1->step/sizeof(src1[0]);
        size_t step2 = v2->step/sizeof(src2[0]);
        double* diff = buf;
        const float* mat = icovar img_ptr(float>();
        size_t matstep = icovar->step/sizeof(mat[0]);

        for( ; sz.height--; src1 += step1, src2 += step2, diff += sz.width )
        {
            for( i = 0; i < sz.width; i++ )
                diff[i] = src1[i] - src2[i];
        }

        diff = buf;
        for( i = 0; i < len; i++, mat += matstep )
        {
            double row_sum = 0;
            j = 0;
             #if CC_ENABLE_UNROLLED
            for(; j <= len - 4; j += 4 )
                row_sum += diff[j]*mat[j] + diff[j+1]*mat[j+1] +
                           diff[j+2]*mat[j+2] + diff[j+3]*mat[j+3];
            #endif
            for( ; j < len; j++ )
                row_sum += diff[j]*mat[j];
            result += row_sum * diff[i];
        }
    }
    else if( type == CC_64F )
    {
        const double* src1 = v1 img_ptr(double>();
        const double* src2 = v2 img_ptr(double>();
        size_t step1 = v1->step/sizeof(src1[0]);
        size_t step2 = v2->step/sizeof(src2[0]);
        double* diff = buf;
        const double* mat = icovar img_ptr(double>();
        size_t matstep = icovar->step/sizeof(mat[0]);

        for( ; sz.height--; src1 += step1, src2 += step2, diff += sz.width )
        {
            for( i = 0; i < sz.width; i++ )
                diff[i] = src1[i] - src2[i];
        }

        diff = buf;
        for( i = 0; i < len; i++, mat += matstep )
        {
            double row_sum = 0;
            j = 0;
             #if CC_ENABLE_UNROLLED
            for(; j <= len - 4; j += 4 )
                row_sum += diff[j]*mat[j] + diff[j+1]*mat[j+1] +
                           diff[j+2]*mat[j+2] + diff[j+3]*mat[j+3];
            #endif
            for( ; j < len; j++ )
                row_sum += diff[j]*mat[j];
            result += row_sum * diff[i];
        }
    }
    else
        CC_Error( CC_StsUnsupportedFormat, "" );

    return sqrt(result);
}

/****************************************************************************************\
*                                        MulTransposed                                   *
\****************************************************************************************/

#endif


template<typename sT, typename dT> static void
MulTransposedR( const img_t* srcmat, img_t* dstmat, const img_t* deltamat, double scale )
{
    int i, j, k;
    const sT* src = img_ptr(sT, srcmat);
    dT* dst = img_ptr(dT, dstmat);
    const dT* delta = img_ptr(dT, deltamat);
    size_t srcstep = srcmat->step/sizeof(src[0]);
    size_t dststep = dstmat->step/sizeof(dst[0]);
    size_t deltastep = deltamat->rows > 1 ? deltamat->step/sizeof(delta[0]) : 0;
    int delta_cols = deltamat->cols;
    CSize size = cvGetSize(srcmat);
    dT* tdst = dst;
    dT* col_buf = 0;
    dT* delta_buf = 0;
    int buf_size = size.height*sizeof(dT);
    uchar* buf = NULL;

    if( delta && delta_cols < size.width )
    {
        assert( delta_cols == 1 );
        buf_size *= 5;
    }
    buf = MALLOC(uchar, buf_size);
    col_buf = (dT*)(uchar*)buf;

    if( delta && delta_cols < size.width )
    {
        delta_buf = col_buf + size.height;
        for( i = 0; i < size.height; i++ )
            delta_buf[i*4] = delta_buf[i*4+1] =
                delta_buf[i*4+2] = delta_buf[i*4+3] = delta[i*deltastep];
        delta = delta_buf;
        deltastep = deltastep ? 4 : 0;
    }

    if( !delta ) {
        for( i = 0; i < size.width; i++, tdst += dststep )
        {
            for( k = 0; k < size.height; k++ )
                col_buf[k] = src[k*srcstep+i];

            for( j = i; j <= size.width - 4; j += 4 )
            {
                double s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                const sT *tsrc = src + j;

                for( k = 0; k < size.height; k++, tsrc += srcstep )
                {
                    double a = col_buf[k];
                    s0 += a * tsrc[0];
                    s1 += a * tsrc[1];
                    s2 += a * tsrc[2];
                    s3 += a * tsrc[3];
                }

                tdst[j] = (dT)(s0*scale);
                tdst[j+1] = (dT)(s1*scale);
                tdst[j+2] = (dT)(s2*scale);
                tdst[j+3] = (dT)(s3*scale);
            }

            for( ; j < size.width; j++ )
            {
                double s0 = 0;
                const sT *tsrc = src + j;

                for( k = 0; k < size.height; k++, tsrc += srcstep )
                    s0 += (double)col_buf[k] * tsrc[0];

                tdst[j] = (dT)(s0*scale);
            }
        }
    } else {
        for( i = 0; i < size.width; i++, tdst += dststep )
        {
            if( !delta_buf )
                for( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta[k*deltastep+i];
            else
                for( k = 0; k < size.height; k++ )
                    col_buf[k] = src[k*srcstep+i] - delta_buf[k*deltastep];

            for( j = i; j <= size.width - 4; j += 4 )
            {
                double s0 = 0, s1 = 0, s2 = 0, s3 = 0;
                const sT *tsrc = src + j;
                const dT *d = delta_buf ? delta_buf : delta + j;

                for( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                {
                    double a = col_buf[k];
                    s0 += a * (tsrc[0] - d[0]);
                    s1 += a * (tsrc[1] - d[1]);
                    s2 += a * (tsrc[2] - d[2]);
                    s3 += a * (tsrc[3] - d[3]);
                }

                tdst[j] = (dT)(s0*scale);
                tdst[j+1] = (dT)(s1*scale);
                tdst[j+2] = (dT)(s2*scale);
                tdst[j+3] = (dT)(s3*scale);
            }

            for( ; j < size.width; j++ )
            {
                double s0 = 0;
                const sT *tsrc = src + j;
                const dT *d = delta_buf ? delta_buf : delta + j;

                for( k = 0; k < size.height; k++, tsrc+=srcstep, d+=deltastep )
                    s0 += (double)col_buf[k] * (tsrc[0] - d[0]);

                tdst[j] = (dT)(s0*scale);
            }
        }
    }
    FREE(buf);
}

#if 0

template<typename sT, typename dT> static void
MulTransposedL( const img_t* srcmat, img_t* dstmat, const img_t* deltamat, double scale )
{
    int i, j, k;
    const sT* src = srcmat img_ptr(sT>();
    dT* dst = dstmat img_ptr(dT>();
    const dT* delta = deltamat img_ptr(dT>();
    size_t srcstep = srcmat->step/sizeof(src[0]);
    size_t dststep = dstmat->step/sizeof(dst[0]);
    size_t deltastep = deltamat->rows > 1 ? deltamat->step/sizeof(delta[0]) : 0;
    int delta_cols = deltamat->cols;
    CSize size = srcmat.size();
    dT* tdst = dst;

    if( !delta )
        for( i = 0; i < size.height; i++, tdst += dststep )
            for( j = i; j < size.height; j++ )
            {
                double s = 0;
                const sT *tsrc1 = src + i*srcstep;
                const sT *tsrc2 = src + j*srcstep;

                for( k = 0; k <= size.width - 4; k += 4 )
                    s += (double)tsrc1[k]*tsrc2[k] + (double)tsrc1[k+1]*tsrc2[k+1] +
                         (double)tsrc1[k+2]*tsrc2[k+2] + (double)tsrc1[k+3]*tsrc2[k+3];
                for( ; k < size.width; k++ )
                    s += (double)tsrc1[k] * tsrc2[k];
                tdst[j] = (dT)(s*scale);
            }
    else
    {
        dT delta_buf[4];
        int delta_shift = delta_cols == size.width ? 4 : 0;
        CAutoBuffer<uchar> buf(size.width*sizeof(dT));
        dT* row_buf = (dT*)(uchar*)buf;

        for( i = 0; i < size.height; i++, tdst += dststep )
        {
            const sT *tsrc1 = src + i*srcstep;
            const dT *tdelta1 = delta + i*deltastep;

            if( delta_cols < size.width )
                for( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[0];
            else
                for( k = 0; k < size.width; k++ )
                    row_buf[k] = tsrc1[k] - tdelta1[k];

            for( j = i; j < size.height; j++ )
            {
                double s = 0;
                const sT *tsrc2 = src + j*srcstep;
                const dT *tdelta2 = delta + j*deltastep;
                if( delta_cols < size.width )
                {
                    delta_buf[0] = delta_buf[1] =
                        delta_buf[2] = delta_buf[3] = tdelta2[0];
                    tdelta2 = delta_buf;
                }
                for( k = 0; k <= size.width-4; k += 4, tdelta2 += delta_shift )
                    s += (double)row_buf[k]*(tsrc2[k] - tdelta2[0]) +
                         (double)row_buf[k+1]*(tsrc2[k+1] - tdelta2[1]) +
                         (double)row_buf[k+2]*(tsrc2[k+2] - tdelta2[2]) +
                         (double)row_buf[k+3]*(tsrc2[k+3] - tdelta2[3]);
                for( ; k < size.width; k++, tdelta2++ )
                    s += (double)row_buf[k]*(tsrc2[k] - tdelta2[0]);
                tdst[j] = (dT)(s*scale);
            }
        }
    }
}

typedef void (*MulTransposedFunc)(const img_t* src, img_t* dst, const img_t* delta, double scale);



void mulTransposed( const img_t* _src, img_t* _dst, bool ata,
                        const img_t* _delta, double scale, int dtype )
{
    CC_INSTRUMENT_REGION()

    img_t *src = _src, delta = _delta;
    const int gemm_level = 100; // boundary above which GEMM is faster.
    int stype = src CC_MAT_TYPE();
    dtype = MAX(MAX(CC_MAT_DEPTH(dtype >= 0 ? dtype : stype), delta.type()), CC_32F);
    CC_Assert( src CC_MAT_CN() == 1 );

    if( !delta.empty() )
    {
        CC_Assert( delta CC_MAT_CN() == 1 &&
            (delta->rows == src->rows || delta->rows == 1) &&
            (delta->cols == src->cols || delta->cols == 1));
        if( delta CC_MAT_TYPE() != dtype )
            delta.convertTo(delta, dtype);
    }

    int dsize = ata ? src->cols : src->rows;
    _dst cvSetMat( dsize, dsize, dtype );
    img_t *dst = _dst;

    if( src->tt.data == dst->tt.data || (stype == dtype &&
        (dst->cols >= gemm_level && dst->rows >= gemm_level &&
         src->cols >= gemm_level && src->rows >= gemm_level)))
    {
        img_t src2;
        const img_t* tsrc = &src;
        if( !delta.empty() )
        {
            if( delta.size() == cvGetSize(src) )
                subtract( src, delta, src2 );
            else
            {
                repeat(delta, src->rows/delta->rows, src->cols/delta->cols, src2);
                subtract( src, src2, src2 );
            }
            tsrc = &src2;
        }
        gemm( *tsrc, *tsrc, scale, img_t(), 0, dst, ata ? CC_GEMM_1_T : CC_GEMM_2_T );
    }
    else
    {
        MulTransposedFunc func = 0;
        if(stype == CC_8U && dtype == CC_32F)
        {
            if(ata)
                func = MulTransposedR<uchar,float>;
            else
                func = MulTransposedL<uchar,float>;
        }
        else if(stype == CC_8U && dtype == CC_64F)
        {
            if(ata)
                func = MulTransposedR<uchar,double>;
            else
                func = MulTransposedL<uchar,double>;
        }
        else if(stype == CC_16U && dtype == CC_32F)
        {
            if(ata)
                func = MulTransposedR<ushort,float>;
            else
                func = MulTransposedL<ushort,float>;
        }
        else if(stype == CC_16U && dtype == CC_64F)
        {
            if(ata)
                func = MulTransposedR<ushort,double>;
            else
                func = MulTransposedL<ushort,double>;
        }
        else if(stype == CC_16S && dtype == CC_32F)
        {
            if(ata)
                func = MulTransposedR<short,float>;
            else
                func = MulTransposedL<short,float>;
        }
        else if(stype == CC_16S && dtype == CC_64F)
        {
            if(ata)
                func = MulTransposedR<short,double>;
            else
                func = MulTransposedL<short,double>;
        }
        else if(stype == CC_32F && dtype == CC_32F)
        {
            if(ata)
                func = MulTransposedR<float,float>;
            else
                func = MulTransposedL<float,float>;
        }
        else if(stype == CC_32F && dtype == CC_64F)
        {
            if(ata)
                func = MulTransposedR<float,double>;
            else
                func = MulTransposedL<float,double>;
        }
        else if(stype == CC_64F && dtype == CC_64F)
        {
            if(ata)
                func = MulTransposedR<double,double>;
            else
                func = MulTransposedL<double,double>;
        }
        if( !func )
            CC_Error( CC_StsUnsupportedFormat, "" );

        func( src, dst, delta, scale );
        completeSymm( dst, false );
    }
}

/****************************************************************************************\
*                                      Dot Product                                       *
\****************************************************************************************/



template<typename T> double
dotProd_(const T* src1, const T* src2, int len)
{
    int i = 0;
    double result = 0;

    #if CC_ENABLE_UNROLLED
    for( ; i <= len - 4; i += 4 )
        result += (double)src1[i]*src2[i] + (double)src1[i+1]*src2[i+1] +
            (double)src1[i+2]*src2[i+2] + (double)src1[i+3]*src2[i+3];
    #endif
    for( ; i < len; i++ )
        result += (double)src1[i]*src2[i];

    return result;
}


static double dotProd_8u(const uchar* src1, const uchar* src2, int len)
{
    double r = 0;
#if ARITHM_USE_IPP && IPP_DISABLE_BLOCK
    CC_IPP_CHECK()
    {
        if (0 <= CC_INSTRUMENT_FUN_IPP(ippiDotProd_8u64f_C1R, (src1, (int)(len*sizeof(src1[0])),
                                       src2, (int)(len*sizeof(src2[0])),
                                       ippiSize(len, 1), &r)))
        {
            CC_IMPL_ADD(CC_IMPL_IPP);
            return r;
        }
        setIppErrorStatus();
    }
#endif
    int i = 0;

#if CC_SSE2
    if( USE_SSE2 )
    {
        int j, len0 = len & -4, blockSize0 = (1 << 13), blockSize;
        __m128i z = _mm_setzero_si128();
        CC_DECL_ALIGNED(16) int buf[4];

        while( i < len0 )
        {
            blockSize = MIN(len0 - i, blockSize0);
            __m128i s = z;
            j = 0;
            for( ; j <= blockSize - 16; j += 16 )
            {
                __m128i b0 = _mm_loadu_si128((const __m128i*)(src1 + j));
                __m128i b1 = _mm_loadu_si128((const __m128i*)(src2 + j));
                __m128i s0, s1, s2, s3;
                s0 = _mm_unpacklo_epi8(b0, z);
                s2 = _mm_unpackhi_epi8(b0, z);
                s1 = _mm_unpacklo_epi8(b1, z);
                s3 = _mm_unpackhi_epi8(b1, z);
                s0 = _mm_madd_epi16(s0, s1);
                s2 = _mm_madd_epi16(s2, s3);
                s = _mm_add_epi32(s, s0);
                s = _mm_add_epi32(s, s2);
            }

            for( ; j < blockSize; j += 4 )
            {
                __m128i s0 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(const int*)(src1 + j)), z);
                __m128i s1 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(const int*)(src2 + j)), z);
                s0 = _mm_madd_epi16(s0, s1);
                s = _mm_add_epi32(s, s0);
            }

            _mm_store_si128((__m128i*)buf, s);
            r += buf[0] + buf[1] + buf[2] + buf[3];

            src1 += blockSize;
            src2 += blockSize;
            i += blockSize;
        }
    }
#elif CC_NEON
    int len0 = len & -8, blockSize0 = (1 << 15), blockSize;
    uint32x4_t v_zero = vdupq_n_u32(0u);
    CC_DECL_ALIGNED(16) uint buf[4];

    while( i < len0 )
    {
        blockSize = MIN(len0 - i, blockSize0);
        uint32x4_t v_sum = v_zero;

        int j = 0;
        for( ; j <= blockSize - 16; j += 16 )
        {
            uint8x16_t v_src1 = vld1q_u8(src1 + j), v_src2 = vld1q_u8(src2 + j);

            uint16x8_t v_src10 = vmovl_u8(vget_low_u8(v_src1)), v_src20 = vmovl_u8(vget_low_u8(v_src2));
            v_sum = vmlal_u16(v_sum, vget_low_u16(v_src10), vget_low_u16(v_src20));
            v_sum = vmlal_u16(v_sum, vget_high_u16(v_src10), vget_high_u16(v_src20));

            v_src10 = vmovl_u8(vget_high_u8(v_src1));
            v_src20 = vmovl_u8(vget_high_u8(v_src2));
            v_sum = vmlal_u16(v_sum, vget_low_u16(v_src10), vget_low_u16(v_src20));
            v_sum = vmlal_u16(v_sum, vget_high_u16(v_src10), vget_high_u16(v_src20));
        }

        for( ; j <= blockSize - 8; j += 8 )
        {
            uint16x8_t v_src1 = vmovl_u8(vld1_u8(src1 + j)), v_src2 = vmovl_u8(vld1_u8(src2 + j));
            v_sum = vmlal_u16(v_sum, vget_low_u16(v_src1), vget_low_u16(v_src2));
            v_sum = vmlal_u16(v_sum, vget_high_u16(v_src1), vget_high_u16(v_src2));
        }

        vst1q_u32(buf, v_sum);
        r += buf[0] + buf[1] + buf[2] + buf[3];

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
#endif
    return r + dotProd_(src1, src2, len - i);
}


static double dotProd_8s(const schar* src1, const schar* src2, int len)
{
    int i = 0;
    double r = 0.0;

#if CC_SSE2
    if( USE_SSE2 )
    {
        int j, len0 = len & -4, blockSize0 = (1 << 13), blockSize;
        __m128i z = _mm_setzero_si128();
        CC_DECL_ALIGNED(16) int buf[4];

        while( i < len0 )
        {
            blockSize = MIN(len0 - i, blockSize0);
            __m128i s = z;
            j = 0;
            for( ; j <= blockSize - 16; j += 16 )
            {
                __m128i b0 = _mm_loadu_si128((const __m128i*)(src1 + j));
                __m128i b1 = _mm_loadu_si128((const __m128i*)(src2 + j));
                __m128i s0, s1, s2, s3;
                s0 = _mm_srai_epi16(_mm_unpacklo_epi8(b0, b0), 8);
                s2 = _mm_srai_epi16(_mm_unpackhi_epi8(b0, b0), 8);
                s1 = _mm_srai_epi16(_mm_unpacklo_epi8(b1, b1), 8);
                s3 = _mm_srai_epi16(_mm_unpackhi_epi8(b1, b1), 8);
                s0 = _mm_madd_epi16(s0, s1);
                s2 = _mm_madd_epi16(s2, s3);
                s = _mm_add_epi32(s, s0);
                s = _mm_add_epi32(s, s2);
            }

            for( ; j < blockSize; j += 4 )
            {
                __m128i s0 = _mm_cvtsi32_si128(*(const int*)(src1 + j));
                __m128i s1 = _mm_cvtsi32_si128(*(const int*)(src2 + j));
                s0 = _mm_srai_epi16(_mm_unpacklo_epi8(s0, s0), 8);
                s1 = _mm_srai_epi16(_mm_unpacklo_epi8(s1, s1), 8);
                s0 = _mm_madd_epi16(s0, s1);
                s = _mm_add_epi32(s, s0);
            }

            _mm_store_si128((__m128i*)buf, s);
            r += buf[0] + buf[1] + buf[2] + buf[3];

            src1 += blockSize;
            src2 += blockSize;
            i += blockSize;
        }
    }
#elif CC_NEON
    int len0 = len & -8, blockSize0 = (1 << 14), blockSize;
    int32x4_t v_zero = vdupq_n_s32(0);
    CC_DECL_ALIGNED(16) int buf[4];

    while( i < len0 )
    {
        blockSize = MIN(len0 - i, blockSize0);
        int32x4_t v_sum = v_zero;

        int j = 0;
        for( ; j <= blockSize - 16; j += 16 )
        {
            int8x16_t v_src1 = vld1q_s8(src1 + j), v_src2 = vld1q_s8(src2 + j);

            int16x8_t v_src10 = vmovl_s8(vget_low_s8(v_src1)), v_src20 = vmovl_s8(vget_low_s8(v_src2));
            v_sum = vmlal_s16(v_sum, vget_low_s16(v_src10), vget_low_s16(v_src20));
            v_sum = vmlal_s16(v_sum, vget_high_s16(v_src10), vget_high_s16(v_src20));

            v_src10 = vmovl_s8(vget_high_s8(v_src1));
            v_src20 = vmovl_s8(vget_high_s8(v_src2));
            v_sum = vmlal_s16(v_sum, vget_low_s16(v_src10), vget_low_s16(v_src20));
            v_sum = vmlal_s16(v_sum, vget_high_s16(v_src10), vget_high_s16(v_src20));
        }

        for( ; j <= blockSize - 8; j += 8 )
        {
            int16x8_t v_src1 = vmovl_s8(vld1_s8(src1 + j)), v_src2 = vmovl_s8(vld1_s8(src2 + j));
            v_sum = vmlal_s16(v_sum, vget_low_s16(v_src1), vget_low_s16(v_src2));
            v_sum = vmlal_s16(v_sum, vget_high_s16(v_src1), vget_high_s16(v_src2));
        }

        vst1q_s32(buf, v_sum);
        r += buf[0] + buf[1] + buf[2] + buf[3];

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
#endif

    return r + dotProd_(src1, src2, len - i);
}

static double dotProd_16u(const ushort* src1, const ushort* src2, int len)
{
#if (ARITHM_USE_IPP == 1)
    CC_IPP_CHECK()
    {
        double r = 0;
        if (0 <= CC_INSTRUMENT_FUN_IPP(ippiDotProd_16u64f_C1R, src1, (int)(len*sizeof(src1[0])), src2, (int)(len*sizeof(src2[0])), ippiSize(len, 1), &r))
        {
            CC_IMPL_ADD(CC_IMPL_IPP);
            return r;
        }
        setIppErrorStatus();
    }
#endif
    return dotProd_(src1, src2, len);
}

static double dotProd_16s(const short* src1, const short* src2, int len)
{
#if (ARITHM_USE_IPP == 1) && (IPP_VERSION_X100 != 900) // bug in IPP 9.0.0
    CC_IPP_CHECK()
    {
        double r = 0;
        if (0 <= CC_INSTRUMENT_FUN_IPP(ippiDotProd_16s64f_C1R, src1, (int)(len*sizeof(src1[0])), src2, (int)(len*sizeof(src2[0])), ippiSize(len, 1), &r))
        {
            CC_IMPL_ADD(CC_IMPL_IPP);
            return r;
        }
        setIppErrorStatus();
    }
#endif
    return dotProd_(src1, src2, len);
}

static double dotProd_32s(const int* src1, const int* src2, int len)
{
#if (ARITHM_USE_IPP == 1)
    CC_IPP_CHECK()
    {
        double r = 0;
        if (0 <= CC_INSTRUMENT_FUN_IPP(ippiDotProd_32s64f_C1R, src1, (int)(len*sizeof(src1[0])), src2, (int)(len*sizeof(src2[0])), ippiSize(len, 1), &r))
        {
            CC_IMPL_ADD(CC_IMPL_IPP);
            return r;
        }
        setIppErrorStatus();
    }
#endif
    return dotProd_(src1, src2, len);
}

static double dotProd_32f(const float* src1, const float* src2, int len)
{
    double r = 0.0;
    int i = 0;

#if (ARITHM_USE_IPP == 1)
    CC_IPP_CHECK()
    {
        if (0 <= CC_INSTRUMENT_FUN_IPP(ippsDotProd_32f64f, src1, src2, len, &r))
        {
            CC_IMPL_ADD(CC_IMPL_IPP);
            return r;
        }
        setIppErrorStatus();
    }
#elif CC_NEON
    int len0 = len & -4, blockSize0 = (1 << 13), blockSize;
    float32x4_t v_zero = vdupq_n_f32(0.0f);
    CC_DECL_ALIGNED(16) float buf[4];

    while( i < len0 )
    {
        blockSize = MIN(len0 - i, blockSize0);
        float32x4_t v_sum = v_zero;

        int j = 0;
        for( ; j <= blockSize - 4; j += 4 )
            v_sum = vmlaq_f32(v_sum, vld1q_f32(src1 + j), vld1q_f32(src2 + j));

        vst1q_f32(buf, v_sum);
        r += buf[0] + buf[1] + buf[2] + buf[3];

        src1 += blockSize;
        src2 += blockSize;
        i += blockSize;
    }
#endif
    return r + dotProd_(src1, src2, len - i);
}

static double dotProd_64f(const double* src1, const double* src2, int len)
{
#if (ARITHM_USE_IPP == 1)
    CC_IPP_CHECK()
    {
        double r = 0;
        if (0 <= CC_INSTRUMENT_FUN_IPP(ippsDotProd_64f, src1, src2, len, &r))
        {
            CC_IMPL_ADD(CC_IMPL_IPP);
            return r;
        }
        setIppErrorStatus();
    }
#endif
    return dotProd_(src1, src2, len);
}


typedef double (*DotProdFunc)(const uchar* src1, const uchar* src2, int len);

static DotProdFunc getDotProdFunc(int type)
{
    static DotProdFunc dotProdTab[] =
    {
        (DotProdFunc)GET_OPTIMIZED(dotProd_8u), (DotProdFunc)GET_OPTIMIZED(dotProd_8s),
        (DotProdFunc)dotProd_16u, (DotProdFunc)dotProd_16s,
        (DotProdFunc)dotProd_32s, (DotProdFunc)GET_OPTIMIZED(dotProd_32f),
        (DotProdFunc)dotProd_64f, 0
    };

    return dotProdTab[type];
}

double img_t::dot(const img_t* _mat) const
{
    CC_INSTRUMENT_REGION()

    img_t mat = _mat;
    int cn = CC_MAT_CN();
    DotProdFunc func = getDotProdFunc(type());
    CC_Assert( mat CC_MAT_TYPE() == type() && mat.size == size && func != 0 );

    if( isContinuous() && mat CC_IS_CONT_MAT() )
    {
        size_t len = total()*cn;
        if( len == (size_t)(int)len )
            return func(data, mat->tt.data, (int)len);
    }

    const img_t* arrays[] = {this, &mat, 0};
    uchar* ptrs[2];
    NAryMatIterator it(arrays, ptrs);
    int len = (int)(it.size*cn);
    double r = 0;

    for( size_t i = 0; i < it.nplanes; i++, ++it )
        r += func( ptrs[0], ptrs[1], len );

    return r;
}



/****************************************************************************************\
*                                    Earlier API                                         *
\****************************************************************************************/

CC_IMPL void cvGEMM( const img_t* Aarr, const img_t* Barr, double alpha,
                     const img_t* Carr, double beta, img_t* Darr, int flags )
{
    img_t A = cvarrToMat(Aarr), B = cvarrToMat(Barr);
    img_t C, D = cvarrToMat(Darr);

    if( Carr )
        C = cvarrToMat(Carr);

    CC_Assert( (D->rows == ((flags & CC_GEMM_A_T) == 0 ? A->rows : A->cols)) &&
               (D->cols == ((flags & CC_GEMM_B_T) == 0 ? B->cols : B->rows)) &&
               D CC_MAT_TYPE() == A CC_MAT_TYPE() );

    gemm( A, B, alpha, C, beta, D, flags );
}


CC_IMPL void
cvTransform( const img_t* srcarr, img_t* dstarr,
             const img_t* transmat, const img_t* shiftvec )
{
    img_t m = cvarrToMat(transmat), src = cvarrToMat(srcarr), dst = cvarrToMat(dstarr);

    if( shiftvec )
    {
        img_t v = cvarrToMat(shiftvec) cvReshape(,1,m->rows),
            _m(m->rows, m->cols + 1, m CC_MAT_TYPE()), m1 = _m.colRange(0,m->cols), v1 = _m.col(m->cols);
        m.convertTo(m1, m1 CC_MAT_TYPE());
        v.convertTo(v1, v1 CC_MAT_TYPE());
        m = _m;
    }

    CC_Assert( dst.type() == src.type() && dst CC_MAT_CN() == m->rows );
    transform( src, dst, m );
}


CC_IMPL void
cvPerspectiveTransform( const img_t* srcarr, img_t* dstarr, const img_t* mat )
{
    img_t m = cvarrToMat(mat), src = cvarrToMat(srcarr), dst = cvarrToMat(dstarr);

    CC_Assert( dst CC_MAT_TYPE() == src CC_MAT_TYPE() && dst CC_MAT_CN() == m->rows-1 );
    perspectiveTransform( src, dst, m );
}


CC_IMPL void cvScaleAdd( const img_t* srcarr1, CScalar scale,
                         const img_t* srcarr2, img_t* dstarr )
{
    img_t src1 = cvarrToMat(srcarr1), dst = cvarrToMat(dstarr);

    CC_Assert( src1.size == dst.size && src1 CC_MAT_TYPE() == dst CC_MAT_TYPE() );
    scaleAdd( src1, scale.val[0], cvarrToMat(srcarr2), dst );
}


CC_IMPL void
cvCalcCovarMatrix( const img_t** vecarr, int count,
                   img_t* covarr, img_t* avgarr, int flags )
{
    img_t cov0 = cvarrToMat(covarr), cov = cov0, mean0, mean;
    CC_Assert( vecarr != 0 && count >= 1 );

    if( avgarr )
        mean = mean0 = cvarrToMat(avgarr);

    if( (flags & CC_COVAR_COLS) != 0 || (flags & CC_COVAR_ROWS) != 0 )
    {

        img_t data = cvarrToMat(vecarr[0]);
        calcCovarMatrix( data, cov, mean, flags, cov CC_MAT_TYPE() );
    }
    else
    {
        std::vector<img_t> data(count);
        for( int i = 0; i < count; i++ )
            data[i] = cvarrToMat(vecarr[i]);
        calcCovarMatrix( &data[0], count, cov, mean, flags, cov CC_MAT_TYPE() );
    }

    if( mean->tt.data != mean0->tt.data && mean0->tt.data )
        mean.convertTo(mean0, mean0 CC_MAT_TYPE());

    if( cov->tt.data != cov0->tt.data )
        cov.convertTo(cov0, cov0 CC_MAT_TYPE());
}


CC_IMPL double
cvMahalanobis( const img_t* srcAarr, const img_t* srcBarr, const img_t* matarr )
{
    return Mahalanobis(cvarrToMat(srcAarr),
        cvarrToMat(srcBarr), cvarrToMat(matarr));
}

CC_IMPL void
cvMulTransposed( const img_t* srcarr, img_t* dstarr,
                 int order, const img_t* deltaarr, double scale )
{
    img_t src = cvarrToMat(srcarr), dst0 = cvarrToMat(dstarr), dst = dst0, delta;
    if( deltaarr )
        delta = cvarrToMat(deltaarr);
    mulTransposed( src, dst, order != 0, delta, scale, dst CC_MAT_TYPE());
    if( dst->tt.data != dst0->tt.data )
        dst.convertTo(dst0, dst0 CC_MAT_TYPE());
}

CC_IMPL double cvDotProduct( const img_t* srcAarr, const img_t* srcBarr )
{
    return cvarrToMat(srcAarr).dot(cvarrToMat(srcBarr));
}


CC_IMPL void
cvCalcPCA( const img_t* data_arr, img_t* avg_arr, img_t* eigenvals, img_t* eigenvects, int flags )
{
    img_t data = cvarrToMat(data_arr), mean0 = cvarrToMat(avg_arr);
    img_t evals0 = cvarrToMat(eigenvals), evects0 = cvarrToMat(eigenvects);
    img_t mean = mean0, evals = evals0, evects = evects0;

    PCA pca;
    pca.mean = mean;
    pca.eigenvalues = evals;
    pca.eigenvectors = evects;

    pca(data, (flags & CC_PCA_USE_AVG) ? mean : img_t(),
        flags, !evals.empty() ? evals->rows + evals->cols - 1 : 0);

    if( pca.mean.size() == mean.size() )
        pca.mean.convertTo( mean, mean CC_MAT_TYPE() );
    else
    {
        img_t temp; pca.mean.convertTo( temp, mean CC_MAT_TYPE() );
        transpose( temp, mean );
    }

    evals = pca.eigenvalues;
    evects = pca.eigenvectors;
    int ecount0 = evals0->cols + evals0->rows - 1;
    int ecount = evals->cols + evals->rows - 1;

    CC_Assert( (evals0->cols == 1 || evals0->rows == 1) &&
                ecount0 <= ecount &&
                evects0->cols == evects->cols &&
                evects0->rows == ecount0 );

    img_t temp = evals0;
    if( evals->rows == 1 )
        evals.colRange(0, ecount0).convertTo(temp, evals0 CC_MAT_TYPE());
    else
        evals cvGetRows(,0, ecount0).convertTo(temp, evals0 CC_MAT_TYPE());
    if( temp->tt.data != evals0->tt.data )
        transpose(temp, evals0);
    evects cvGetRows(,0, ecount0).convertTo( evects0, evects0 CC_MAT_TYPE() );

    // otherwise some datatype's or size's were incorrect, so the output arrays have been reallocated
    CC_Assert( mean0->tt.data == mean->tt.data );
}


CC_IMPL void
cvProjectPCA( const img_t* data_arr, const img_t* avg_arr,
              const img_t* eigenvects, img_t* result_arr )
{
    img_t data = cvarrToMat(data_arr), mean = cvarrToMat(avg_arr);
    img_t evects = cvarrToMat(eigenvects), dst0 = cvarrToMat(result_arr), dst = dst0;

    PCA pca;
    pca.mean = mean;
    int n;
    if( mean->rows == 1 )
    {
        CC_Assert(dst->cols <= evects->rows && dst->rows == data->rows);
        n = dst->cols;
    }
    else
    {
        CC_Assert(dst->rows <= evects->rows && dst->cols == data->cols);
        n = dst->rows;
    }
    pca.eigenvectors = evects cvGetRows(,0, n);

    img_t result = pca.project(data);
    if( result->cols != dst->cols )
        result = result cvReshape(,1, 1);
    result.convertTo(dst, dst CC_MAT_TYPE());

    CC_Assert(dst0->tt.data == dst->tt.data);
}


CC_IMPL void
cvBackProjectPCA( const img_t* proj_arr, const img_t* avg_arr,
                  const img_t* eigenvects, img_t* result_arr )
{
    img_t data = cvarrToMat(proj_arr), mean = cvarrToMat(avg_arr);
    img_t evects = cvarrToMat(eigenvects), dst0 = cvarrToMat(result_arr), dst = dst0;

    PCA pca;
    pca.mean = mean;
    int n;
    if( mean->rows == 1 )
    {
        CC_Assert(data->cols <= evects->rows && dst->rows == data->rows);
        n = data->cols;
    }
    else
    {
        CC_Assert(data->rows <= evects->rows && dst->cols == data->cols);
        n = data->rows;
    }
    pca.eigenvectors = evects cvGetRows(,0, n);

    img_t result = pca.backProject(data);
    result.convertTo(dst, dst CC_MAT_TYPE());

    CC_Assert(dst0->tt.data == dst->tt.data);
}


#endif