
template <typename T, typename ST, typename QT>
struct Integral_SIMD
{
    bool operator()(const T *, size_t,
                    ST *, size_t,
                    QT *, size_t,
                    ST *, size_t,
                    int, int, int) const
    {
        return false;
    }
};

#if CC_SSE2

template <>
struct Integral_SIMD<uchar, int, double>
{
    Integral_SIMD()
    {
        haveSSE2 = cpu_check_support(CC_CPU_SSE2);
    }

    bool operator()(const uchar * src, size_t _srcstep,
                    int * sum, size_t _sumstep,
                    double * sqsum, size_t,
                    int * tilted, size_t,
                    int width, int height, int cn) const
    {
        if (sqsum || tilted || cn != 1 || !haveSSE2)
            return false;

        // the first iteration
        memset(sum, 0, (width + 1) * sizeof(int));

        __m128i v_zero = _mm_setzero_si128(), prev = v_zero;
        int j = 0;

        // the others
        for (int i = 0; i < height; ++i)
        {
            const uchar * src_row = src + _srcstep * i;
            int * prev_sum_row = (int *)((uchar *)sum + _sumstep * i) + 1;
            int * sum_row = (int *)((uchar *)sum + _sumstep * (i + 1)) + 1;

            sum_row[-1] = 0;

            prev = v_zero;
            j = 0;

            for ( ; j + 7 < width; j += 8)
            {
                __m128i vsuml = _mm_loadu_si128((const __m128i *)(prev_sum_row + j));
                __m128i vsumh = _mm_loadu_si128((const __m128i *)(prev_sum_row + j + 4));

                __m128i el8shr0 = _mm_loadl_epi64((const __m128i *)(src_row + j));
                __m128i el8shr1 = _mm_slli_si128(el8shr0, 1);
                __m128i el8shr2 = _mm_slli_si128(el8shr0, 2);
                __m128i el8shr3 = _mm_slli_si128(el8shr0, 3);

                vsuml = _mm_add_epi32(vsuml, prev);
                vsumh = _mm_add_epi32(vsumh, prev);

                __m128i el8shr12 = _mm_add_epi16(_mm_unpacklo_epi8(el8shr1, v_zero),
                                                 _mm_unpacklo_epi8(el8shr2, v_zero));
                __m128i el8shr03 = _mm_add_epi16(_mm_unpacklo_epi8(el8shr0, v_zero),
                                                 _mm_unpacklo_epi8(el8shr3, v_zero));
                __m128i el8 = _mm_add_epi16(el8shr12, el8shr03);

                __m128i el4h = _mm_add_epi16(_mm_unpackhi_epi16(el8, v_zero),
                                             _mm_unpacklo_epi16(el8, v_zero));

                vsuml = _mm_add_epi32(vsuml, _mm_unpacklo_epi16(el8, v_zero));
                vsumh = _mm_add_epi32(vsumh, el4h);

                _mm_storeu_si128((__m128i *)(sum_row + j), vsuml);
                _mm_storeu_si128((__m128i *)(sum_row + j + 4), vsumh);

                prev = _mm_add_epi32(prev, _mm_shuffle_epi32(el4h, _MM_SHUFFLE(3, 3, 3, 3)));
            }

            for (int v = sum_row[j - 1] - prev_sum_row[j - 1]; j < width; ++j)
                sum_row[j] = (v += src_row[j]) + prev_sum_row[j];
        }

        return true;
    }

    bool haveSSE2;
};

#endif

template<typename T, typename ST, typename QT>
void integral_( const T* src, size_t _srcstep, ST* sum, size_t _sumstep,
                QT* sqsum, size_t _sqsumstep, ST* tilted, size_t _tiltedstep,
                int width, int height, int cn )
{
    int x, y, k;

    if (Integral_SIMD<T, ST, QT>()(src, _srcstep,
                                   sum, _sumstep,
                                   sqsum, _sqsumstep,
                                   tilted, _tiltedstep,
                                   width, height, cn))
        return;

    int srcstep = (int)(_srcstep/sizeof(T));
    int sumstep = (int)(_sumstep/sizeof(ST));
    int tiltedstep = (int)(_tiltedstep/sizeof(ST));
    int sqsumstep = (int)(_sqsumstep/sizeof(QT));

    width *= cn;

    memset( sum, 0, (width+cn)*sizeof(sum[0]));
    sum += sumstep + cn;

    if( sqsum )
    {
        memset( sqsum, 0, (width+cn)*sizeof(sqsum[0]));
        sqsum += sqsumstep + cn;
    }

    if( tilted )
    {
        memset( tilted, 0, (width+cn)*sizeof(tilted[0]));
        tilted += tiltedstep + cn;
    }

    if( sqsum == 0 && tilted == 0 )
    {
        for( y = 0; y < height; y++, src += srcstep - cn, sum += sumstep - cn )
        {
            for( k = 0; k < cn; k++, src++, sum++ )
            {
                ST s = sum[-cn] = 0;
                for( x = 0; x < width; x += cn )
                {
                    s += src[x];
                    sum[x] = sum[x - sumstep] + s;
                }
            }
        }
    }
    else if( tilted == 0 )
    {
        for( y = 0; y < height; y++, src += srcstep - cn,
                        sum += sumstep - cn, sqsum += sqsumstep - cn )
        {
            for( k = 0; k < cn; k++, src++, sum++, sqsum++ )
            {
                ST s = sum[-cn] = 0;
                QT sq = sqsum[-cn] = 0;
                for( x = 0; x < width; x += cn )
                {
                    T it = src[x];
                    s += it;
                    sq += (QT)it*it;
                    ST t = sum[x - sumstep] + s;
                    QT tq = sqsum[x - sqsumstep] + sq;
                    sum[x] = t;
                    sqsum[x] = tq;
                }
            }
        }
    }
    else
    {
        ST* _buf = MALLOC(ST, width+cn);
        ST* buf = _buf;
        ST s;
        QT sq;
        for( k = 0; k < cn; k++, src++, sum++, tilted++, buf++ )
        {
            sum[-cn] = tilted[-cn] = 0;

            for( x = 0, s = 0, sq = 0; x < width; x += cn )
            {
                T it = src[x];
                buf[x] = tilted[x] = it;
                s += it;
                sq += (QT)it*it;
                sum[x] = s;
                if( sqsum )
                    sqsum[x] = sq;
            }

            if( width == cn )
                buf[cn] = 0;

            if( sqsum )
            {
                sqsum[-cn] = 0;
                sqsum++;
            }
        }

        for( y = 1; y < height; y++ )
        {
            src += srcstep - cn;
            sum += sumstep - cn;
            tilted += tiltedstep - cn;
            buf += -cn;

            if( sqsum )
                sqsum += sqsumstep - cn;

            for( k = 0; k < cn; k++, src++, sum++, tilted++, buf++ )
            {
                T it = src[0];
                ST t0 = s = it;
                QT tq0 = sq = (QT)it*it;

                sum[-cn] = 0;
                if( sqsum )
                    sqsum[-cn] = 0;
                tilted[-cn] = tilted[-tiltedstep];

                sum[0] = sum[-sumstep] + t0;
                if( sqsum )
                    sqsum[0] = sqsum[-sqsumstep] + tq0;
                tilted[0] = tilted[-tiltedstep] + t0 + buf[cn];

                for( x = cn; x < width - cn; x += cn )
                {
                    ST t1 = buf[x];
                    buf[x - cn] = t1 + t0;
                    t0 = it = src[x];
                    tq0 = (QT)it*it;
                    s += t0;
                    sq += tq0;
                    sum[x] = sum[x - sumstep] + s;
                    if( sqsum )
                        sqsum[x] = sqsum[x - sqsumstep] + sq;
                    t1 += buf[x + cn] + t0 + tilted[x - tiltedstep - cn];
                    tilted[x] = t1;
                }

                if( width > cn )
                {
                    ST t1 = buf[x];
                    buf[x - cn] = t1 + t0;
                    t0 = it = src[x];
                    tq0 = (QT)it*it;
                    s += t0;
                    sq += tq0;
                    sum[x] = sum[x - sumstep] + s;
                    if( sqsum )
                        sqsum[x] = sqsum[x - sqsumstep] + sq;
                    tilted[x] = t0 + t1 + tilted[x - tiltedstep - cn];
                    buf[x] = t0;
                }

                if( sqsum )
                    sqsum++;
            }
        }
        FREE(_buf);
    }
}


#ifdef HAVE_OPENCL

static bool ocl_integral( const img_t* src, img_t* sum, int sdepth )
{
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    if ( (src->tid != CC_8UC1) ||
        !(sdepth == CC_32S || sdepth == CC_32F || (doubleSupport && sdepth == CC_64F)))
        return false;

    static const int tileSize = 16;

    String build_opt = format("-D sumT=%s -D LOCAL_SUM_SIZE=%d%s",
                                ocl::typeToStr(sdepth), tileSize,
                                doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel kcols("integral_sum_cols", ocl::imgproc::integral_sum_oclsrc, build_opt);
    if (kcols.empty())
        return false;

    UMat src = src.getUMat();
    CSize src_size = cvGetSize(src);
    CSize bufsize(((src_size.height + tileSize - 1) / tileSize) * tileSize, ((src_size.width + tileSize - 1) / tileSize) * tileSize);
    UMat buf(bufsize, sdepth);
    kcols.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnlyNoSize(buf));
    size_t gt = src->cols, lt = tileSize;
    if (!kcols.run(1, &gt, &lt, false))
        return false;

    ocl::Kernel krows("integral_sum_rows", ocl::imgproc::integral_sum_oclsrc, build_opt);
    if (krows.empty())
        return false;

    CSize sumsize(src_size.width + 1, src_size.height + 1);
    _sum cvSetMat(sumsize, sdepth);
    UMat sum = _sum.getUMat();

    krows.args(ocl::KernelArg::ReadOnlyNoSize(buf), ocl::KernelArg::WriteOnly(sum));
    gt = src->rows;
    return krows.run(1, &gt, &lt, false);
}

static bool ocl_integral( const img_t* src, img_t* sum, img_t* sqsum, int sdepth, int sqdepth )
{
    bool doubleSupport = ocl::Device::getDefault().doubleFPConfig() > 0;

    if ( src->tid != CC_8UC1 || (!doubleSupport && (sdepth == CC_64F || sqdepth == CC_64F)) )
        return false;

    static const int tileSize = 16;

    String build_opt = format("-D SUM_SQUARE -D sumT=%s -D sumSQT=%s -D LOCAL_SUM_SIZE=%d%s",
                                ocl::typeToStr(sdepth), ocl::typeToStr(sqdepth),
                                tileSize,
                                doubleSupport ? " -D DOUBLE_SUPPORT" : "");

    ocl::Kernel kcols("integral_sum_cols", ocl::imgproc::integral_sum_oclsrc, build_opt);
    if (kcols.empty())
        return false;

    UMat src = src.getUMat();
    CSize src_size = cvGetSize(src);
    CSize bufsize(((src_size.height + tileSize - 1) / tileSize) * tileSize, ((src_size.width + tileSize - 1) / tileSize) * tileSize);
    UMat buf(bufsize, sdepth);
    UMat buf_sq(bufsize, sqdepth);
    kcols.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnlyNoSize(buf), ocl::KernelArg::WriteOnlyNoSize(buf_sq));
    size_t gt = src->cols, lt = tileSize;
    if (!kcols.run(1, &gt, &lt, false))
        return false;

    ocl::Kernel krows("integral_sum_rows", ocl::imgproc::integral_sum_oclsrc, build_opt);
    if (krows.empty())
        return false;

    CSize sumsize(src_size.width + 1, src_size.height + 1);
    _sum cvSetMat(sumsize, sdepth);
    UMat sum = _sum.getUMat();
    sqsum cvSetMat(sumsize, sqdepth);
    UMat sum_sq = sqsum.getUMat();

    krows.args(ocl::KernelArg::ReadOnlyNoSize(buf), ocl::KernelArg::ReadOnlyNoSize(buf_sq), ocl::KernelArg::WriteOnly(sum), ocl::KernelArg::WriteOnlyNoSize(sum_sq));
    gt = src->rows;
    return krows.run(1, &gt, &lt, false);
}

#endif

#if defined(HAVE_IPP)

static bool ipp_integral(
    int depth, int sdepth, int sqdepth,
    const uchar* src, size_t srcstep,
    uchar* sum, size_t sumstep,
    uchar* sqsum, size_t sqsumstep,
    int width, int height, int cn)
{
    CC_INSTRUMENT_REGION_IPP()

#if IPP_VERSION_X100 != 900 // Disabled on ICV due invalid results
    if( sdepth <= 0 )
        sdepth = depth == CC_8U ? CC_32S : CC_64F;
    if ( sqdepth <= 0 )
         sqdepth = CC_64F;
    sdepth = CC_MAT_DEPTH(sdepth), sqdepth = CC_MAT_DEPTH(sqdepth);

    if( ( depth == CC_8U ) && ( sdepth == CC_32F || sdepth == CC_32S ) && ( !sqsum || sqdepth == CC_64F ) && ( cn == 1 ) )
    {
        IppStatus status = ippStsErr;
        IppiSize srcRoiSize = ippiSize( width, height );
        if( sdepth == CC_32F )
        {
            if( sqsum )
            {
                status = CC_INSTRUMENT_FUN_IPP(ippiSqrIntegral_8u32f64f_C1R, (const Ipp8u*)src, (int)srcstep, (Ipp32f*)sum, (int)sumstep, (Ipp64f*)sqsum, (int)sqsumstep, srcRoiSize, 0, 0);
            }
            else
            {
                status = CC_INSTRUMENT_FUN_IPP(ippiIntegral_8u32f_C1R, (const Ipp8u*)src, (int)srcstep, (Ipp32f*)sum, (int)sumstep, srcRoiSize, 0);
            }
        }
        else if( sdepth == CC_32S )
        {
            if( sqsum )
            {
                status = CC_INSTRUMENT_FUN_IPP(ippiSqrIntegral_8u32s64f_C1R, (const Ipp8u*)src, (int)srcstep, (Ipp32s*)sum, (int)sumstep, (Ipp64f*)sqsum, (int)sqsumstep, srcRoiSize, 0, 0);
            }
            else
            {
                status = CC_INSTRUMENT_FUN_IPP(ippiIntegral_8u32s_C1R, (const Ipp8u*)src, (int)srcstep, (Ipp32s*)sum, (int)sumstep, srcRoiSize, 0);
            }
        }
        if (0 <= status)
        {
            CC_IMPL_ADD(CC_IMPL_IPP);
            return true;
        }
    }
#else
    CC_UNUSED(depth); CC_UNUSED(sdepth); CC_UNUSED(sqdepth);
    CC_UNUSED(src); CC_UNUSED(srcstep);
    CC_UNUSED(sum); CC_UNUSED(sumstep);
    CC_UNUSED(sqsum); CC_UNUSED(sqsumstep);
    CC_UNUSED(tilted); CC_UNUSED(tstep);
    CC_UNUSED(width); CC_UNUSED(height); CC_UNUSED(cn);
#endif
    return false;
}

#endif

static void hal_integral(TypeId depth, TypeId sdepth, TypeId sqdepth,
              const uchar* src, size_t srcstep,
              uchar* sum, size_t sumstep,
              uchar* sqsum, size_t sqsumstep,
              uchar* tilted, size_t tstep,
              int width, int height, int cn)
{
    //CALL_HAL(integral, cv_hal_integral, depth, sdepth, sqdepth, src, srcstep, sum, sumstep, sqsum, sqsumstep, tilted, tstep, width, height, cn);
    CC_IPP_RUN(( depth == CC_8U )
               && ( sdepth == CC_32F || sdepth == CC_32S )
               && ( !tilted )
               && ( !sqsum || sqdepth == CC_64F )
               && ( cn == 1 ),
               ipp_integral(depth, sdepth, sqdepth, src, srcstep, sum, sumstep, sqsum, sqsumstep, width, height, cn), 0);

#define ONE_CALL(A, B, C) integral_<A, B, C>((const A*)src, srcstep, (B*)sum, sumstep, (C*)sqsum, sqsumstep, (B*)tilted, tstep, width, height, cn)

    if( depth == CC_8U && sdepth == CC_32S && sqdepth == CC_64F )
        ONE_CALL(uchar, int, double);
    else if( depth == CC_8U && sdepth == CC_32S && sqdepth == CC_32F )
        ONE_CALL(uchar, int, float);
    else if( depth == CC_8U && sdepth == CC_32S && sqdepth == CC_32S )
        ONE_CALL(uchar, int, int);
    else if( depth == CC_8U && sdepth == CC_32F && sqdepth == CC_64F )
        ONE_CALL(uchar, float, double);
    else if( depth == CC_8U && sdepth == CC_32F && sqdepth == CC_32F )
        ONE_CALL(uchar, float, float);
    else if( depth == CC_8U && sdepth == CC_64F && sqdepth == CC_64F )
        ONE_CALL(uchar, double, double);
    else if( depth == CC_16U && sdepth == CC_64F && sqdepth == CC_64F )
        ONE_CALL(ushort, double, double);
    else if( depth == CC_16S && sdepth == CC_64F && sqdepth == CC_64F )
        ONE_CALL(short, double, double);
    else if( depth == CC_32F && sdepth == CC_32F && sqdepth == CC_64F )
        ONE_CALL(float, float, double);
    else if( depth == CC_32F && sdepth == CC_32F && sqdepth == CC_32F )
        ONE_CALL(float, float, float);
    else if( depth == CC_32F && sdepth == CC_64F && sqdepth == CC_64F )
        ONE_CALL(float, double, double);
    else if( depth == CC_64F && sdepth == CC_64F && sqdepth == CC_64F )
        ONE_CALL(double, double, double);
    else
        CC_Error( CC_StsUnsupportedFormat, "" );

#undef ONE_CALL
}



static void integral( const img_t* src, img_t* sum, img_t* sqsum, img_t* tilted,
              TypeId sdepth CC_DEFAULT(CC_NUL), TypeId sqdepth CC_DEFAULT(CC_NUL) )
{
    CC_INSTRUMENT_REGION()

    TypeCnId type = CC_MAT_TYPECN(src);
    TypeId depth = CC_MAT_DEPTH(src);
    int cn = CC_MAT_CN(src);
    if( sdepth <= 0 )
        sdepth = depth == CC_8U ? CC_32S : CC_64F;
    if ( sqdepth <= 0 )
         sqdepth = CC_64F;

#ifdef HAVE_OPENCL
    if (ocl::useOpenCL() && _sum.isUMat() && !tilted.needed())
    {
        if (!sqsum.needed())
        {
            CC_OCL_RUN(ocl::useOpenCL(), ocl_integral(src, _sum, sdepth))
        }
        else if (sqsum.isUMat())
            CC_OCL_RUN(ocl::useOpenCL(), ocl_integral(src, _sum, sqsum, sdepth, sqdepth))
    }
#endif

    CSize ssize = cvGetSize(src), isize = cSize(ssize.width + 1, ssize.height + 1);
    cvSetMat( sum, isize.h, isize.w, sdepth, cn );

    if( sqsum )
    {
        cvSetMat( sqsum, isize.h, isize.w, sqdepth, cn );
    };

    if( tilted )
    {
        cvSetMat( tilted, isize.h, isize.w, sdepth, cn );
    }

    hal_integral(depth, sdepth, sqdepth,
                  src->tt.data, src->step,
                  sum->tt.data, sum->step,
		sqsum ? sqsum->tt.data : NULL, sqsum ? sqsum->step : 0,
		tilted ? tilted->tt.data : NULL, tilted ? tilted->step : 0,
                  src->cols, src->rows, cn);
}

static void integral( const img_t* src, img_t* sum, TypeId sdepth CC_DEFAULT(CC_NUL) )
{
    CC_INSTRUMENT_REGION()

    integral( src, sum, NULL, NULL, sdepth );
}

static void integral( const img_t* src, img_t* sum, img_t* sqsum,
              TypeId sdepth CC_DEFAULT(CC_NUL), TypeId sqdepth CC_DEFAULT(CC_NUL) )
{
    CC_INSTRUMENT_REGION()

    integral( src, sum, sqsum, NULL, sdepth, sqdepth );
}

#if 0

CC_IMPL void
cvIntegral( const img_t* image, img_t* sumImage,
            img_t* sumSqImage, img_t* tiltedSumImage )
{
    img_t src = cvarrToMat(image), sum = cvarrToMat(sumImage), sum0 = sum;
    img_t sqsum0, sqsum, tilted0, tilted;
    img_t *psqsum = 0, *ptilted = 0;

    if( sumSqImage )
    {
        sqsum0 = sqsum = cvarrToMat(sumSqImage);
        psqsum = &sqsum;
    }

    if( tiltedSumImage )
    {
        tilted0 = tilted = cvarrToMat(tiltedSumImage);
        ptilted = &tilted;
    }
    integral( src, sum, psqsum ? _OutputArray(*psqsum) : _OutputArray(),
                  ptilted ? _OutputArray(*ptilted) : _OutputArray(), sum CC_MAT_DEPTH() );

    CC_Assert( sum.data == sum0.data && sqsum.data == sqsum0.data && tilted.data == tilted0.data );
}

#endif
