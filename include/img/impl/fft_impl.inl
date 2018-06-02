

/* FFT of real vector
   output vector format:
     re(0), re(1), im(1), ... , re(n/2-1), im((n+1)/2-1) [, re((n+1)/2)] OR ...
     re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */

static int icvRealDFT( const datatype* src, datatype* dst,
                     int n, int nf, int* factors, const int* itab,
                     const CvComplex* wave, int tab_size,
                     const void* spec, CvComplex* buf,
                     int flags, double scale )
{
    int complex_output = (flags & ICC_DFT_COMPLEX_INPUT_OR_OUTPUT) != 0;
    int j, n2 = n >> 1;
    dst += complex_output;

    assert( tab_size == n );

    if( n == 1 )
    {
        dst[0] = (datatype)(src[0]*scale);
    }
    else if( n == 2 )
    {
        double t = (src[0] + src[1])*scale;
        dst[1] = (datatype)((src[0] - src[1])*scale);
        dst[0] = (datatype)t;
    }
    else if( n & 1 )
    {
        CvComplex* _dst;
        dst -= complex_output;
        _dst = (CvComplex*)dst;
        _dst[0].re = (datatype)(src[0]*scale);
        _dst[0].im = 0;
        for( j = 1; j < n; j += 2 )
        {
            double t0 = src[itab[j]]*scale;
            double t1 = src[itab[j+1]]*scale;
            _dst[j].re = (datatype)t0;
            _dst[j].im = 0;
            _dst[j+1].re = (datatype)t1;
            _dst[j+1].im = 0;
        }
        icvDFTc( _dst, _dst, n, nf, factors, itab, wave,
                            tab_size, 0, buf, ICC_DFT_NO_PERMUTE, 1. );
        if( !complex_output )
            dst[1] = dst[0];
        return CC_OK;
    }
    else
    {
        double t0, t;
        double h1_re, h1_im, h2_re, h2_im;
        double scale2 = scale*0.5;
        factors[0] >>= 1;

        icvDFTc( (CvComplex*)src,
                            (CvComplex*)dst, n2,
                            nf - (factors[0] == 1),
                            factors + (factors[0] == 1),
                            itab, wave, tab_size, 0, buf, 0, 1. );
        factors[0] <<= 1;

        t = dst[0] - dst[1];
        dst[0] = (datatype)((dst[0] + dst[1])*scale);
        dst[1] = (datatype)(t*scale);

        t0 = dst[n2];
        t = dst[n-1];
        dst[n-1] = dst[1];

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

            dst[j-1] = (datatype)(h1_re + h2_re);
            dst[n-j-1] = (datatype)(h1_re - h2_re);
            dst[j] = (datatype)(h1_im + h2_im);
            dst[n-j] = (datatype)(h2_im - h1_im);
        }

        if( j <= n2 )
        {
            dst[n2-1] = (datatype)(t0*scale);
            dst[n2] = (datatype)(-t*scale);
        }
    }

//finalize:
    if( complex_output )
    {
        dst[-1] = dst[0];
        dst[0] = 0;
        if( (n & 1) == 0 )
            dst[n] = 0;
    }

    return CC_OK;
}


/* Inverse FFT of complex conjugate-symmetric vector
   input vector format:
      re[0], re[1], im[1], ... , re[n/2-1], im[n/2-1], re[n/2] OR
      re(0), 0, re(1), im(1), ..., re(n/2-1), im((n+1)/2-1) [, re((n+1)/2), 0] */

static int 
icvCCSIDFT( const datatype* src, datatype* dst,
                     int n, int nf, int* factors, const int* itab,
                     const CvComplex* wave, int tab_size,
                     const void* spec, CvComplex* buf,
                     int flags, double scale )
{
    int complex_input = (flags & ICC_DFT_COMPLEX_INPUT_OR_OUTPUT) != 0;
    int j, k, n2 = (n+1) >> 1;
    double save_s1 = 0.;
    double t0, t1, t2, t3, t;

    assert( tab_size == n );

    if( complex_input )
    {
        assert( src != dst );
        save_s1 = src[1];
        ((datatype*)src)[1] = src[0];
        src++;
    }

    if( n == 1 )
    {
        dst[0] = (datatype)(src[0]*scale);
    }
    else if( n == 2 )
    {
        t = (src[0] + src[1])*scale;
        dst[1] = (datatype)((src[0] - src[1])*scale);
        dst[0] = (datatype)t;
    }
    else if( n & 1 )
    {
        CvComplex* _src = (CvComplex*)(src-1);
        CvComplex* _dst = (CvComplex*)dst;

        _dst[0].re = src[0];
        _dst[0].im = 0;
        for( j = 1; j < n2; j++ )
        {
            int k0 = itab[j], k1 = itab[n-j];
            t0 = _src[j].re; t1 = _src[j].im;
            _dst[k0].re = (datatype)t0; _dst[k0].im = (datatype)-t1;
            _dst[k1].re = (datatype)t0; _dst[k1].im = (datatype)t1;
        }

        icvDFTc( _dst, _dst, n, nf, factors, itab, wave,
                            tab_size, 0, buf, ICC_DFT_NO_PERMUTE, 1. );
        dst[0] = (datatype)(dst[0]*scale);
        for( j = 1; j < n; j += 2 )
        {
            t0 = dst[j*2]*scale;
            t1 = dst[j*2+2]*scale;
            dst[j] = (datatype)t0;
            dst[j+1] = (datatype)t1;
        }
    }
    else
    {
        int inplace = src == dst;
        const CvComplex* w = wave;

        t = src[1];
        t0 = (src[0] + src[n-1]);
        t1 = (src[n-1] - src[0]);
        dst[0] = (datatype)t0;
        dst[1] = (datatype)t1;

        for( j = 2, w++; j < n2; j += 2, w++ )
        {
            double h1_re, h1_im, h2_re, h2_im;

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
                dst[j] = (datatype)t0;
                dst[j+1] = (datatype)t1;
                dst[n-j] = (datatype)t2;
                dst[n-j+1]= (datatype)t3;
            }
            else
            {
                int j2 = j >> 1;
                k = itab[j2];
                dst[k] = (datatype)t0;
                dst[k+1] = (datatype)t1;
                k = itab[n2-j2];
                dst[k] = (datatype)t2;
                dst[k+1]= (datatype)t3;
            }
        }

        if( j <= n2 )
        {
            t0 = t*2;
            t1 = src[n2]*2;

            if( inplace )
            {
                dst[n2] = (datatype)t0;
                dst[n2+1] = (datatype)t1;
            }
            else
            {
                k = itab[n2];
                dst[k*2] = (datatype)t0;
                dst[k*2+1] = (datatype)t1;
            }
        }

        factors[0] >>= 1;
        icvDFTc( (CvComplex*)dst,
                            (CvComplex*)dst, n2,
                            nf - (factors[0] == 1),
                            factors + (factors[0] == 1), itab,
                            wave, tab_size, 0, buf,
                            inplace ? 0 : ICC_DFT_NO_PERMUTE, 1. );
        factors[0] <<= 1;

        for( j = 0; j < n; j += 2 )
        {
            t0 = dst[j]*scale;
            t1 = dst[j+1]*(-scale);
            dst[j] = (datatype)t0;
            dst[j+1] = (datatype)t1;
        }
    }

//finalize:
    if( complex_input )
        ((datatype*)src)[0] = (datatype)save_s1;

    return CC_OK;
}

void cvDFT(int src_rows, int src_cols, const datatype* src, int src_step, int srccn,
           int dst_rows, int dst_cols, datatype* dst, int dst_step, int dstcn, int flags, int nonzero_rows )
{
    typedef int (*CvDFTFunc)(
    const void* src, void* dst, int n, int nf, int* factors,
    const int* itab, const void* wave, int tab_size,
    const void* spec, void* buf, int inv, double scale );

    static CvDFTFunc dft_tbl[6];
    static int inittab = 0;

    void* buffer = 0;
    int local_alloc = 1;
    void *spec_c = 0, *spec_r = 0, *spec = 0;
    const datatype* src0;

    int prev_len = 0, buf_size = 0, stage = 0;
    int nf = 0, inv = (flags & CC_DXT_INVERSE) != 0;
    int real_transform = 0;
    int complex_elem_size, elem_size;
    int factors[34], inplace_transform = 0;
    int ipp_norm_flag = 0;

    if( !inittab )
    {
        dft_tbl[0] = (CvDFTFunc)icvDFTc;
        dft_tbl[1] = (CvDFTFunc)icvRealDFT;
        dft_tbl[2] = (CvDFTFunc)icvCCSIDFT;
        dft_tbl[3] = (CvDFTFunc)icvDFTc;
        dft_tbl[4] = (CvDFTFunc)icvRealDFT;
        dft_tbl[5] = (CvDFTFunc)icvCCSIDFT;
        inittab = 1;
    }

    src0 = src;
    elem_size = sizeof(datatype);
    complex_elem_size = elem_size*2;

    // check types and sizes
    if( !inv && srccn == 1 && dstcn == 2 )
    {
        if( (src_cols != 1 || dst_cols != 1 ||
            src_rows/2+1 != dst_rows && src_rows != dst_rows) &&
            (src_cols/2+1 != dst_cols || src_rows != dst_rows) )
            CC_ERROR( CC_StsUnmatchedSizes, "" );
        real_transform = 1;
    }
    else if( inv && srccn == 2 && dstcn == 1 )
    {
        if( (src_cols != 1 || dst_cols != 1 ||
            dst_rows/2+1 != src_rows && src_rows != dst_rows) &&
            (dst_cols/2+1 != src_cols || src_rows != dst_rows) )
            CC_ERROR( CC_StsUnmatchedSizes, "" );
        real_transform = 1;
    }
    else {
        //CC_ERROR( CC_StsUnmatchedFormats, "Incorrect or unsupported combination of input & output formats" );
    }

    if( src_cols == 1 && nonzero_rows > 0 ) {
        CC_ERROR( CC_StsNotImplemented,
        "This mode (using nonzero_rows with a single-column matrix) breaks the function logic, so it is prohibited.\n"
        "For fast convolution/correlation use 2-column matrix or single-row matrix instead" );
    }

    // determine, which transform to do first - row-wise
    // (stage 0) or column-wise (stage 1) transform
    if( !(flags & CC_DXT_ROWS) && src_rows > 1 &&
        (src_cols == 1 && !CC_IS_MAT_CONT(src_type & dst_type) ||
        src_cols > 1 && inv && real_transform) )
        stage = 1;

    ipp_norm_flag = !(flags & CC_DXT_SCALE) ? 8 : (flags & CC_DXT_INVERSE) ? 2 : 1;

    for(;;)
    {
        double scale = 1;
        uchar* wave = 0;
        int* itab = 0;
        uchar* ptr;
        int i, len, count, sz = 0;
        int use_buf = 0, odd_real = 0;
        CvDFTFunc dft_func;

        if( stage == 0 ) // row-wise transform
        {
            len = !inv ? src_cols : dst_cols;
            count = src_rows;
            if( len == 1 && !(flags & CC_DXT_ROWS) )
            {
                len = !inv ? src_rows : dst_rows;
                count = 1;
            }
            odd_real = real_transform && (len & 1);
        }
        else
        {
            len = dst_rows;
            count = !inv ? src_cols : dst_cols;
            sz = 2*len*complex_elem_size;
        }

        spec = 0;
        {
            if( len != prev_len )
                nf = icvDFTFactorize( len, factors );

            inplace_transform = factors[0] == factors[nf-1];
            sz += len*(complex_elem_size + sizeof(int));
            i = nf > 1 && (factors[0] & 1) == 0;
            if( (factors[i] & 1) != 0 && factors[i] > 5 )
                sz += (factors[i]+1)*complex_elem_size;

            if( stage == 0 && (src == dst && !inplace_transform || odd_real) ||
                stage == 1 && !inplace_transform )
            {
                use_buf = 1;
                sz += len*complex_elem_size;
            }
        }

        if( sz > buf_size )
        {
            prev_len = 0; // because we release the buffer,
                          // force recalculation of
                          // twiddle factors and permutation table
            if( !local_alloc && buffer )
                pfree( buffer );
            if( sz <= CC_MAX_LOCAL_DFT_SIZE )
            {
                buf_size = sz = CC_MAX_LOCAL_DFT_SIZE;
                buffer = _alloca(sz + 32);
                local_alloc = 1;
            }
            else
            {
                buffer = pmalloc(sz + 32);
                buf_size = sz;
                local_alloc = 0;
            }
        }

        ptr = (uchar*)buffer;
        if( !spec )
        {
            wave = ptr;
            ptr += len*complex_elem_size;
            itab = (int*)ptr;
            ptr = (uchar*)cvAlignPtr( ptr + len*sizeof(int), 16 );

            if( len != prev_len || (!inplace_transform && inv && real_transform))
                icvDFTInit( len, nf, factors, itab, complex_elem_size,
                            wave, stage == 0 && inv && real_transform );
            // otherwise reuse the tables calculated on the previous stage
        }

        if( stage == 0 )
        {
            uchar* tmp_buf = 0;
            int dptr_offset = 0;
            int dst_full_len = len*elem_size;
            int _flags = inv + (srccn != dstcn ?
                         ICC_DFT_COMPLEX_INPUT_OR_OUTPUT : 0);
            if( use_buf )
            {
                tmp_buf = ptr;
                ptr += len*complex_elem_size;
                if( odd_real && !inv && len > 1 &&
                    !(_flags & ICC_DFT_COMPLEX_INPUT_OR_OUTPUT))
                    dptr_offset = elem_size;
            }

            if( !inv && (_flags & ICC_DFT_COMPLEX_INPUT_OR_OUTPUT) )
                dst_full_len += (len & 1) ? elem_size : complex_elem_size;

            dft_func = dft_tbl[(!real_transform ? 0 : !inv ? 1 : 2)];

            if( count > 1 && !(flags & CC_DXT_ROWS) && (!inv || !real_transform) )
                stage = 1;
            else if( flags & CC_DXT_SCALE )
                scale = 1./(len * (flags & CC_DXT_ROWS ? 1 : count));

            if( nonzero_rows <= 0 || nonzero_rows > count )
                nonzero_rows = count;

            for( i = 0; i < nonzero_rows; i++ )
            {
                uchar* sptr = (uchar*)src + i*src_step;
                uchar* dptr0 = (uchar*)dst + i*dst_step;
                uchar* dptr = dptr0;

                if( tmp_buf )
                    dptr = tmp_buf;

                dft_func( sptr, dptr, len, nf, factors, itab, wave, len, spec, ptr, _flags, scale );
                if( dptr != dptr0 )
                    memcpy( dptr0, dptr + dptr_offset, dst_full_len );
            }

            for( ; i < count; i++ )
            {
                uchar* dptr0 = (uchar*)dst + i*dst_step;
                memset( dptr0, 0, dst_full_len );
            }

            if( stage != 1 )
                break;
            src = dst;
        }
        else
        {
            int a = 0, b = count;
            uchar *buf0, *buf1, *dbuf0, *dbuf1;
            uchar* sptr0 = (uchar*)src;
            uchar* dptr0 = (uchar*)dst;
            buf0 = ptr;
            ptr += len*complex_elem_size;
            buf1 = ptr;
            ptr += len*complex_elem_size;
            dbuf0 = buf0, dbuf1 = buf1;

            if( use_buf )
            {
                dbuf1 = ptr;
                dbuf0 = buf1;
                ptr += len*complex_elem_size;
            }

            dft_func = dft_tbl[0];

            if( real_transform && inv && src_cols > 1 )
                stage = 0;
            else if( flags & CC_DXT_SCALE )
                scale = 1./(len * count);

            if( real_transform )
            {
                int even;
                a = 1;
                even = (count & 1) == 0;
                b = (count+1)/2;
                if( !inv )
                {
                    memset( buf0, 0, len*complex_elem_size );
                    icvCopyColumn( sptr0, src_step, buf0, complex_elem_size, len, elem_size );
                    sptr0 += dstcn*elem_size;
                    if( even )
                    {
                        memset( buf1, 0, len*complex_elem_size );
                        icvCopyColumn( sptr0 + (count-2)*elem_size, src_step,
                                       buf1, complex_elem_size, len, elem_size );
                    }
                }
                else if( srccn == 1 )
                {
                    icvCopyColumn( sptr0, src_step, buf0 + elem_size, elem_size, len, elem_size );
                    icvExpandCCS( buf0 + elem_size, len, elem_size );
                    if( even )
                    {
                        icvCopyColumn( sptr0 + (count-1)*elem_size, src_step,
                                       buf1 + elem_size, elem_size, len, elem_size );
                        icvExpandCCS( buf1 + elem_size, len, elem_size );
                    }
                    sptr0 += elem_size;
                }
                else
                {
                    icvCopyColumn( sptr0, src_step, buf0, complex_elem_size, len, complex_elem_size );
                    //memcpy( buf0 + elem_size, buf0, elem_size );
                    //icvExpandCCS( buf0 + elem_size, len, elem_size );
                    if( even )
                    {
                        icvCopyColumn( sptr0 + b*complex_elem_size, src_step,
                                       buf1, complex_elem_size, len, complex_elem_size );
                        //memcpy( buf0 + elem_size, buf0, elem_size );
                        //icvExpandCCS( buf0 + elem_size, len, elem_size );
                    }
                    sptr0 += complex_elem_size;
                }

                if( even ) {
                    dft_func( buf1, dbuf1, len, nf, factors, itab, wave, len, spec, ptr, inv, scale );
                }

                dft_func( buf0, dbuf0, len, nf, factors, itab, wave, len, spec, ptr, inv, scale );

                if( dstcn == 1 )
                {
                    if( !inv )
                    {
                        // copy the half of output vector to the first/last column.
                        // before doing that, defgragment the vector
                        memcpy( dbuf0 + elem_size, dbuf0, elem_size );
                        icvCopyColumn( dbuf0 + elem_size, elem_size, dptr0,
                                       dst_step, len, elem_size );
                        if( even )
                        {
                            memcpy( dbuf1 + elem_size, dbuf1, elem_size );
                            icvCopyColumn( dbuf1 + elem_size, elem_size,
                                           dptr0 + (count-1)*elem_size,
                                           dst_step, len, elem_size );
                        }
                        dptr0 += elem_size;
                    }
                    else
                    {
                        // copy the real part of the complex vector to the first/last column
                        icvCopyColumn( dbuf0, complex_elem_size, dptr0, dst_step, len, elem_size );
                        if( even )
                            icvCopyColumn( dbuf1, complex_elem_size, dptr0 + (count-1)*elem_size,
                                           dst_step, len, elem_size );
                        dptr0 += elem_size;
                    }
                }
                else
                {
                    assert( !inv );
                    icvCopyColumn( dbuf0, complex_elem_size, dptr0,
                                   dst_step, len, complex_elem_size );
                    if( even )
                        icvCopyColumn( dbuf1, complex_elem_size,
                                       dptr0 + b*complex_elem_size,
                                       dst_step, len, complex_elem_size );
                    dptr0 += complex_elem_size;
                }
            }

            for( i = a; i < b; i += 2 )
            {
                if( i+1 < b ) {
                    icvCopyFrom2Columns( sptr0, src_step, buf0, buf1, len, complex_elem_size );
                    dft_func( buf1, dbuf1, len, nf, factors, itab, wave, len, spec, ptr, inv, scale );
                } else {
                    icvCopyColumn( sptr0, src_step, buf0, complex_elem_size, len, complex_elem_size );
                }

                dft_func( buf0, dbuf0, len, nf, factors, itab, wave, len, spec, ptr, inv, scale );

                if( i+1 < b ) {
                    icvCopyTo2Columns( dbuf0, dbuf1, dptr0, dst_step, len, complex_elem_size );
                } else {
                    icvCopyColumn( dbuf0, complex_elem_size, dptr0, dst_step, len, complex_elem_size );
                }
                sptr0 += 2*complex_elem_size;
                dptr0 += 2*complex_elem_size;
            }

            if( stage != 0 )
                break;
            src = dst;
        }
    }

    if( buffer && !local_alloc )
        pfree( buffer );

}



//两个傅立叶频谱的每个元素的乘法（Performs per-element multiplication of two Fourier spectrums）
//src1 第一输入数组
//src2 第二输入数组
//dst 输出数组，和输入数组有相同的类型和大小。
//flags 下面列举的值的组合:
//CC_DXT_ROWS - 把数组的每一行视为一个单独的频谱 (参见 cvDFT 的参数讨论).
//CC_DXT_MUL_CONJ - 在做乘法之前取第二个输入数组的共轭.
//函数 cvMulSpectrums 执行两个 CCS-packed 或者实数或复数傅立叶变换的结果复数矩阵的每个元素的乘法。
int cvMulSpectrums( int rows, int cols, int cn,
                    const datatype* srcA, int srcA_step,
                    const datatype* srcB, int srcB_step,
                datatype* dst, int dst_step, int flags )
{
    int stepA, stepB, stepC;
    int is_1d;
    int j, j0, j1, k, ncols;

    is_1d = (flags & CC_DXT_ROWS) ||
            (rows == 1 || cols == 1 &&
             CC_IS_MAT_CONT( srcA->type & srcB->type & dst->type ));

    if( is_1d && !(flags & CC_DXT_ROWS) )
        cols = cols + rows - 1, rows = 1;
    ncols = cols*cn;
    j0 = cn == 1;
    j1 = ncols - (cols % 2 == 0 && cn == 1);

    {
        const datatype* dataA = srcA;
        const datatype* dataB = srcB;
        datatype* dataC = dst;

        stepA = srcA_step/sizeof(dataA[0]);
        stepB = srcB_step/sizeof(dataB[0]);
        stepC = dst_step/sizeof(dataC[0]);

        if( !is_1d && cn == 1 )
        {
            for( k = 0; k < (cols % 2 ? 1 : 2); k++ )
            {
                if( k == 1 )
                    dataA += cols - 1, dataB += cols - 1, dataC += cols - 1;
                dataC[0] = dataA[0]*dataB[0];
                if( rows % 2 == 0 )
                    dataC[(rows-1)*stepC] = dataA[(rows-1)*stepA]*dataB[(rows-1)*stepB];
                if( !(flags & CC_DXT_MUL_CONJ) )
                    for( j = 1; j <= rows - 2; j += 2 )
                    {
                        double re = (double)dataA[j*stepA]*dataB[j*stepB] -
                                    (double)dataA[(j+1)*stepA]*dataB[(j+1)*stepB];
                        double im = (double)dataA[j*stepA]*dataB[(j+1)*stepB] +
                                    (double)dataA[(j+1)*stepA]*dataB[j*stepB];
                        dataC[j*stepC] = (datatype)re; dataC[(j+1)*stepC] = (datatype)im;
                    }
                else
                    for( j = 1; j <= rows - 2; j += 2 )
                    {
                        double re = (double)dataA[j*stepA]*dataB[j*stepB] +
                                    (double)dataA[(j+1)*stepA]*dataB[(j+1)*stepB];
                        double im = (double)dataA[(j+1)*stepA]*dataB[j*stepB] -
                                    (double)dataA[j*stepA]*dataB[(j+1)*stepB];
                        dataC[j*stepC] = (datatype)re; dataC[(j+1)*stepC] = (datatype)im;
                    }
                if( k == 1 )
                    dataA -= cols - 1, dataB -= cols - 1, dataC -= cols - 1;
            }
        }

        for( ; rows--; dataA += stepA, dataB += stepB, dataC += stepC )
        {
            if( is_1d && cn == 1 )
            {
                dataC[0] = dataA[0]*dataB[0];
                if( cols % 2 == 0 )
                    dataC[j1] = dataA[j1]*dataB[j1];
            }

            if( !(flags & CC_DXT_MUL_CONJ) )
                for( j = j0; j < j1; j += 2 )
                {
                    double re = (double)dataA[j]*dataB[j] - (double)dataA[j+1]*dataB[j+1];
                    double im = (double)dataA[j+1]*dataB[j] + (double)dataA[j]*dataB[j+1];
                    dataC[j] = (datatype)re; dataC[j+1] = (datatype)im;
                }
            else
                for( j = j0; j < j1; j += 2 )
                {
                    double re = (double)dataA[j]*dataB[j] + (double)dataA[j+1]*dataB[j+1];
                    double im = (double)dataA[j+1]*dataB[j] - (double)dataA[j]*dataB[j+1];
                    dataC[j] = (datatype)re; dataC[j+1] = (datatype)im;
                }
        }
    }
    return 0;
}


/****************************************************************************************\
                               Discrete Cosine Transform
\****************************************************************************************/

/* DCT is calculated using DFT, as described here:
   http://www.ece.utexas.edu/~bevans/courses/ee381k/lectures/09_DCT/lecture9/:
*/
static int 
icvDCT_fwd( const datatype* src, int src_step, datatype* dft_src,
                     datatype* dft_dst, datatype* dst, int dst_step,
                     int n, int nf, int* factors, const int* itab,
                     const CvComplex* dft_wave,
                     const CvComplex* dct_wave,
                     const void* spec, CvComplex* buf )
{
    int j, n2 = n >> 1;
    datatype* dst1;

    src_step /= sizeof(src[0]);
    dst_step /= sizeof(dst[0]);
    dst1 = dst + (n-1)*dst_step;

    if( n == 1 )
    {
        dst[0] = src[0];
        return CC_OK;
    }

    for( j = 0; j < n2; j++, src += src_step*2 )
    {
        dft_src[j] = src[0];
        dft_src[n-j-1] = src[src_step];
    }

    icvRealDFT( dft_src, dft_dst, n, nf, factors,
                         itab, dft_wave, n, spec, buf, 0, 1.0 );
    src = dft_dst;

    dst[0] = (datatype)(src[0]*dct_wave->re*icv_sin_45);
    dst += dst_step;
    for( j = 1, dct_wave++; j < n2; j++, dct_wave++,
                                    dst += dst_step, dst1 -= dst_step )
    {
        double t0 = dct_wave->re*src[j*2-1] - dct_wave->im*src[j*2];
        double t1 = -dct_wave->im*src[j*2-1] - dct_wave->re*src[j*2];
        dst[0] = (datatype)t0;
        dst1[0] = (datatype)t1;
    }

    dst[0] = (datatype)(src[n-1]*dct_wave->re);
    return CC_OK;
}


static int 
icvDCT_inv( const datatype* src, int src_step, datatype* dft_src,
                     datatype* dft_dst, datatype* dst, int dst_step,
                     int n, int nf, int* factors, const int* itab,
                     const CvComplex* dft_wave,
                     const CvComplex* dct_wave,
                     const void* spec, CvComplex* buf )
{
    int j, n2 = n >> 1;
    const datatype* src1;

    src_step /= sizeof(src[0]);
    dst_step /= sizeof(dst[0]);
    src1 = src + (n-1)*src_step;

    if( n == 1 )
    {
        dst[0] = src[0];
        return CC_OK;
    }

    dft_src[0] = (datatype)(src[0]*2*dct_wave->re*icv_sin_45);
    src += src_step;
    for( j = 1, dct_wave++; j < n2; j++, dct_wave++,
                                    src += src_step, src1 -= src_step )
    {
        double t0 = dct_wave->re*src[0] - dct_wave->im*src1[0];
        double t1 = -dct_wave->im*src[0] - dct_wave->re*src1[0];
        dft_src[j*2-1] = (datatype)t0;
        dft_src[j*2] = (datatype)t1;
    }

    dft_src[n-1] = (datatype)(src[0]*2*dct_wave->re);
    icvCCSIDFT( dft_src, dft_dst, n, nf, factors, itab,
                         dft_wave, n, spec, buf, CC_DXT_INVERSE, 1.0 );

    for( j = 0; j < n2; j++, dst += dst_step*2 )
    {
        dst[0] = dft_dst[j];
        dst[dst_step] = dft_dst[n-j-1];
    }
    return CC_OK;
}

void cvDCT( int src_rows, int src_cols, const datatype* src, int src_step, datatype* dst, int dst_step, int flags )
{
  typedef int ( * CvDCTFunc)(
    const void* src, int src_step, void* dft_src,
    void* dft_dst, void* dst, int dst_step, int n,
    int nf, int* factors, const int* itab, const void* dft_wave,
                const void* dct_wave, const void* spec, void* buf );

    static CvDCTFunc dct_tbl[4];
    static int inittab = 0;

    void* buffer = 0;
    int local_alloc = 1;
    int inv = (flags & CC_DXT_INVERSE) != 0;
    void *spec_dft = 0, *spec = 0;

    double scale = 1.;
    int prev_len = 0, buf_size = 0, nf = 0, stage, end_stage;
    uchar *src_dft_buf = 0, *dst_dft_buf = 0;
    uchar *dft_wave = 0, *dct_wave = 0;
    int* itab = 0;
    uchar* ptr = 0;
    int complex_elem_size, elem_size;
    int factors[34], inplace_transform;
    int i, len, count;
    CvDCTFunc dct_func;

    if( !inittab )
    {
        dct_tbl[0] = (CvDCTFunc)icvDCT_fwd;
        dct_tbl[1] = (CvDCTFunc)icvDCT_inv;
        dct_tbl[2] = (CvDCTFunc)icvDCT_fwd;
        dct_tbl[3] = (CvDCTFunc)icvDCT_inv;
        inittab = 1;
    }

    elem_size = sizeof(datatype);
    complex_elem_size = elem_size*2;

    dct_func = dct_tbl[inv];

    if( (flags & CC_DXT_ROWS) || src_rows == 1 ||
        src_cols == 1 && CC_IS_MAT_CONT(src_type & dst_type))
    {
        stage = end_stage = 0;
    }
    else
    {
        stage = src_cols == 1;
        end_stage = 1;
    }

    for( ; stage <= end_stage; stage++ )
    {
        uchar *sptr = (uchar*)src, *dptr = (uchar*)dst;
        int sstep0, sstep1, dstep0, dstep1;

        if( stage == 0 )
        {
            len = src_cols;
            count = src_rows;
            if( len == 1 && !(flags & CC_DXT_ROWS) )
            {
                len = src_rows;
                count = 1;
            }
            sstep0 = src_step;
            dstep0 = dst_step;
            sstep1 = dstep1 = elem_size;
        }
        else
        {
            len = src_rows;
            count = src_cols;
            sstep1 = src_step;
            dstep1 = dst_step;
            sstep0 = dstep0 = elem_size;
        }

        if( len != prev_len )
        {
            int sz;

            if( len > 1 && (len & 1) )
                CC_ERROR( CC_StsNotImplemented, "Odd-size DCT\'s are not implemented" );

            sz = len*elem_size;
            sz += (len/2 + 1)*complex_elem_size;

            spec = 0;
            inplace_transform = 1;
            {
                sz += len*(complex_elem_size + sizeof(int)) + complex_elem_size;

                nf = icvDFTFactorize( len, factors );
                inplace_transform = factors[0] == factors[nf-1];

                i = nf > 1 && (factors[0] & 1) == 0;
                if( (factors[i] & 1) != 0 && factors[i] > 5 )
                    sz += (factors[i]+1)*complex_elem_size;

                if( !inplace_transform )
                    sz += len*elem_size;
            }

            if( sz > buf_size )
            {
                if( !local_alloc && buffer )
                    pfree( buffer );
                if( sz <= CC_MAX_LOCAL_DFT_SIZE )
                {
                    buf_size = sz = CC_MAX_LOCAL_DFT_SIZE;
                    buffer = _alloca(sz + 32);
                    local_alloc = 1;
                }
                else
                {
                    buffer = pmalloc(sz + 32);
                    buf_size = sz;
                    local_alloc = 0;
                }
            }

            ptr = (uchar*)buffer;
            if( !spec )
            {
                dft_wave = ptr;
                ptr += len*complex_elem_size;
                itab = (int*)ptr;
                ptr = (uchar*)cvAlignPtr( ptr + len*sizeof(int), 16 );
                icvDFTInit( len, nf, factors, itab, complex_elem_size, dft_wave, inv );
            }

            dct_wave = ptr;
            ptr += (len/2 + 1)*complex_elem_size;
            src_dft_buf = dst_dft_buf = ptr;
            ptr += len*elem_size;
            if( !inplace_transform )
            {
                dst_dft_buf = ptr;
                ptr += len*elem_size;
            }
            icvDCTInit( len, complex_elem_size, dct_wave, inv );
            if( !inv )
                scale += scale;
            prev_len = len;
        }
        // otherwise reuse the tables calculated on the previous stage

        for( i = 0; i < count; i++ )
        {
            dct_func( sptr + i*sstep0, sstep1, src_dft_buf, dst_dft_buf,
                      dptr + i*dstep0, dstep1, len, nf, factors,
                      itab, dft_wave, dct_wave, spec, ptr );
        }
        src = dst;
    }

    if( buffer && !local_alloc )
        pfree( buffer );
}


#undef datatype
#undef icvDFTc
#undef icvRealDFT
#undef icvCCSIDFT
#undef CvComplex
#undef cvDFT
#undef cvMulSpectrums
#undef icvDCT_fwd
#undef icvDCT_inv
#undef cvDCT
