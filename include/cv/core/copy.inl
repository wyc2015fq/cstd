
#if 0
/* ////////////////////////////////////////////////////////////////////
//
//  img_t basic operations: Copy, Set
//
// */


//#include "opencl_kernels_core.hpp"

template<typename T> static void
copyMask_(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    for( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const T* src = (const T*)_src;
        T* dst = (T*)_dst;
        int x = 0;
         #if CC_ENABLE_UNROLLED
        for( ; x <= size.width - 4; x += 4 )
        {
            if( mask[x] )
                dst[x] = src[x];
            if( mask[x+1] )
                dst[x+1] = src[x+1];
            if( mask[x+2] )
                dst[x+2] = src[x+2];
            if( mask[x+3] )
                dst[x+3] = src[x+3];
        }
        #endif
        for( ; x < size.width; x++ )
            if( mask[x] )
                dst[x] = src[x];
    }
}

template<> void
copyMask_<uchar>(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CC_IPP_RUN_FAST(CC_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1MR, _src, (int)sstep, _dst, (int)dstep, ippiSize(size), mask, (int)mstep) >= 0)

    for( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const uchar* src = (const uchar*)_src;
        uchar* dst = (uchar*)_dst;
        int x = 0;
        #if CC_SSE4_2
        if(USE_SSE4_2)//
        {
            __m128i zero = _mm_setzero_si128 ();

             for( ; x <= size.width - 16; x += 16 )
             {
                 const __m128i rSrc = _mm_lddqu_si128((const __m128i*)(src+x));
                 __m128i _mask = _mm_lddqu_si128((const __m128i*)(mask+x));
                 __m128i rDst = _mm_lddqu_si128((__m128i*)(dst+x));
                 __m128i _negMask = _mm_cmpeq_epi8(_mask, zero);
                 rDst = _mm_blendv_epi8(rSrc, rDst, _negMask);
                 _mm_storeu_si128((__m128i*)(dst + x), rDst);
             }
        }
        #elif CC_NEON
        uint8x16_t v_one = vdupq_n_u8(1);
        for( ; x <= size.width - 16; x += 16 )
        {
            uint8x16_t v_mask = vcgeq_u8(vld1q_u8(mask + x), v_one);
            uint8x16_t v_dst = vld1q_u8(dst + x), v_src = vld1q_u8(src + x);
            vst1q_u8(dst + x, vbslq_u8(v_mask, v_src, v_dst));
        }
        #endif
        for( ; x < size.width; x++ )
            if( mask[x] )
                dst[x] = src[x];
    }
}

template<> void
copyMask_<ushort>(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size)
{
    CC_IPP_RUN_FAST(CC_INSTRUMENT_FUN_IPP(ippiCopy_16u_C1MR, (const Ipp16u *)_src, (int)sstep, (Ipp16u *)_dst, (int)dstep, ippiSize(size), mask, (int)mstep) >= 0)

    for( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const ushort* src = (const ushort*)_src;
        ushort* dst = (ushort*)_dst;
        int x = 0;
        #if CC_SSE4_2
        if(USE_SSE4_2)//
        {
            __m128i zero = _mm_setzero_si128 ();
            for( ; x <= size.width - 8; x += 8 )
            {
                 const __m128i rSrc =_mm_lddqu_si128((const __m128i*)(src+x));
                 __m128i _mask = _mm_loadl_epi64((const __m128i*)(mask+x));
                 _mask = _mm_unpacklo_epi8(_mask, _mask);
                 __m128i rDst = _mm_lddqu_si128((const __m128i*)(dst+x));
                 __m128i _negMask = _mm_cmpeq_epi8(_mask, zero);
                 rDst = _mm_blendv_epi8(rSrc, rDst, _negMask);
                 _mm_storeu_si128((__m128i*)(dst + x), rDst);
             }
        }
        #elif CC_NEON
        uint8x8_t v_one = vdup_n_u8(1);
        for( ; x <= size.width - 8; x += 8 )
        {
            uint8x8_t v_mask = vcge_u8(vld1_u8(mask + x), v_one);
            uint8x8x2_t v_mask2 = vzip_u8(v_mask, v_mask);
            uint16x8_t v_mask_res = vreinterpretq_u16_u8(vcombine_u8(v_mask2.val[0], v_mask2.val[1]));

            uint16x8_t v_src = vld1q_u16(src + x), v_dst = vld1q_u16(dst + x);
            vst1q_u16(dst + x, vbslq_u16(v_mask_res, v_src, v_dst));
        }
        #endif
        for( ; x < size.width; x++ )
            if( mask[x] )
                dst[x] = src[x];
    }
}

static void
copyMaskGeneric(const uchar* _src, size_t sstep, const uchar* mask, size_t mstep, uchar* _dst, size_t dstep, Size size, void* _esz)
{
    size_t k, esz = *(size_t*)_esz;
    for( ; size.height--; mask += mstep, _src += sstep, _dst += dstep )
    {
        const uchar* src = _src;
        uchar* dst = _dst;
        int x = 0;
        for( ; x < size.width; x++, src += esz, dst += esz )
        {
            if( !mask[x] )
                continue;
            for( k = 0; k < esz; k++ )
                dst[k] = src[k];
        }
    }
}


#define DEF_COPY_MASK(suffix, type) \
static void copyMask##suffix(const uchar* src, size_t sstep, const uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, void*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}

#if defined HAVE_IPP
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
static void copyMask##suffix(const uchar* src, size_t sstep, const uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, void*) \
{ \
    CC_IPP_RUN_FAST(CC_INSTRUMENT_FUN_IPP(ippiCopy_##ippfavor, (const ipptype *)src, (int)sstep, (ipptype *)dst, (int)dstep, ippiSize(size), (const Ipp8u *)mask, (int)mstep) >= 0)\
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#else
#define DEF_COPY_MASK_F(suffix, type, ippfavor, ipptype) \
static void copyMask##suffix(const uchar* src, size_t sstep, const uchar* mask, size_t mstep, \
                             uchar* dst, size_t dstep, Size size, void*) \
{ \
    copyMask_<type>(src, sstep, mask, mstep, dst, dstep, size); \
}
#endif

#if IPP_VERSION_X100 == 901 // bug in IPP 9.0.1
DEF_COPY_MASK(32sC3, Vec3i)
DEF_COPY_MASK(8uC3, Vec3b)
#else
DEF_COPY_MASK_F(8uC3, Vec3b, 8u_C3MR, Ipp8u)
DEF_COPY_MASK_F(32sC3, Vec3i, 32s_C3MR, Ipp32s)
#endif
DEF_COPY_MASK(8u, uchar)
DEF_COPY_MASK(16u, ushort)
DEF_COPY_MASK_F(32s, int, 32s_C1MR, Ipp32s)
DEF_COPY_MASK_F(16uC3, Vec3s, 16u_C3MR, Ipp16u)
DEF_COPY_MASK(32sC2, Vec2i)
DEF_COPY_MASK_F(32sC4, Vec4i, 32s_C4MR, Ipp32s)
DEF_COPY_MASK(32sC6, Vec6i)
DEF_COPY_MASK(32sC8, Vec8i)

BinaryFunc copyMaskTab[] =
{
    0,
    copyMask8u,
    copyMask16u,
    copyMask8uC3,
    copyMask32s,
    0,
    copyMask16uC3,
    0,
    copyMask32sC2,
    0, 0, 0,
    copyMask32sC3,
    0, 0, 0,
    copyMask32sC4,
    0, 0, 0, 0, 0, 0, 0,
    copyMask32sC6,
    0, 0, 0, 0, 0, 0, 0,
    copyMask32sC8
};

BinaryFunc getCopyMaskFunc(size_t esz)
{
    return esz <= 32 && copyMaskTab[esz] ? copyMaskTab[esz] : copyMaskGeneric;
}

/* dst = src */
void img_t::copyTo( img_t* _dst ) const
{
    CC_INSTRUMENT_REGION()

    int dtype = _dst CC_MAT_TYPE();
    if( _dst.fixedType() && dtype != type() )
    {
        CC_Assert( CC_MAT_CN() == CC_MAT_CN(dtype) );
        convertTo( _dst, dtype );
        return;
    }

    if( _dst.isUMat() )
    {
        if( empty() )
        {
            _dst.release();
            return;
        }
        _dst cvSetMat( dims, size.p, type() );
        UMat dst = _dst.getUMat();

        size_t i, sz[CC_MAX_DIM], dstofs[CC_MAX_DIM], esz = elemSize();
        for( i = 0; i < (size_t)dims; i++ )
            sz[i] = size.p[i];
        sz[dims-1] *= esz;
        dst.ndoffset(dstofs);
        dstofs[dims-1] *= esz;
        dst.u->currAllocator->upload(dst.u, data, dims, sz, dstofs, dst->step.p, step.p);
        return;
    }

    if( dims <= 2 )
    {
        _dst cvSetMat( rows, cols, type() );
        img_t *dst = _dst;
        if( data == dst.data )
            return;

        if( rows > 0 && cols > 0 )
        {
            // For some cases (with vector) dst.size != src.size, so force to column-based form
            // It prevents memory corruption in case of column-based src
            if (_dst.isVector())
                dst = dst cvReshape(,0, (int)dst CC_MAT_TOTAL());

            const uchar* sptr = data;
            uchar* dptr = dst.data;

            CC_IPP_RUN(
                    (size_t)cols*elemSize() <= (size_t)INT_MAX &&
                    (size_t)step <= (size_t)INT_MAX &&
                    (size_t)dst->step <= (size_t)INT_MAX
                    ,
                    CC_INSTRUMENT_FUN_IPP(ippiCopy_8u_C1R, sptr, (int)step, dptr, (int)dst->step, ippiSize((int)(cols*elemSize()), rows)) >= 0
            )

            Size sz = getContinuousSize(*this, dst);
            size_t len = sz.width*elemSize();

            for( ; sz.height--; sptr += step, dptr += dst->step )
                memcpy( dptr, sptr, len );
        }
        return;
    }

    _dst cvSetMat( dims, size, type() );
    img_t *dst = _dst;
    if( data == dst.data )
        return;

    if( total() != 0 )
    {
        const img_t* arrays[] = { this, &dst };
        uchar* ptrs[2];
        NAryMatIterator it(arrays, ptrs, 2);
        size_t sz = it.size*elemSize();

        for( size_t i = 0; i < it.nplanes; i++, ++it )
            memcpy(ptrs[1], ptrs[0], sz);
    }
}

void img_t::copyTo( img_t* _dst, const img_t* _mask ) const
{
    CC_INSTRUMENT_REGION()

    img_t mask = _mask;
    if( !mask.data )
    {
        copyTo(_dst);
        return;
    }

    int cn = CC_MAT_CN(), mcn = mask CC_MAT_CN();
    CC_Assert( mask CC_MAT_DEPTH() == CC_8U && (mcn == 1 || mcn == cn) );
    bool colorMask = mcn > 1;

    size_t esz = colorMask ? elemSize1() : elemSize();
    BinaryFunc copymask = getCopyMaskFunc(esz);

    uchar* data0 = _dst.data;
    _dst cvSetMat( dims, size, type() );
    img_t *dst = _dst;

    if( dst.data != data0 ) // do not leave dst uninitialized
        dst = Scalar(0);

    if( dims <= 2 )
    {
        CC_Assert( size() == mask.size() );
        Size sz = getContinuousSize(*this, dst, mask, mcn);
        copymask(data, step, mask.data, mask->step, dst.data, dst->step, sz, &esz);
        return;
    }

    const img_t* arrays[] = { this, &dst, &mask, 0 };
    uchar* ptrs[3];
    NAryMatIterator it(arrays, ptrs);
    Size sz((int)(it.size*mcn), 1);

    for( size_t i = 0; i < it.nplanes; i++, ++it )
        copymask(ptrs[0], 0, ptrs[2], 0, ptrs[1], 0, sz, &esz);
}

img_t& img_t::operator = (const Scalar& s)
{
    CC_INSTRUMENT_REGION()

    const img_t* arrays[] = { this };
    uchar* dptr;
    NAryMatIterator it(arrays, &dptr, 1);
    size_t elsize = it.size*elemSize();
    const int64* is = (const int64*)&s.val[0];

    if( is[0] == 0 && is[1] == 0 && is[2] == 0 && is[3] == 0 )
    {
#if defined HAVE_IPP && IPP_DISABLE_BLOCK
        CC_IPP_CHECK()
        {
            if (dims <= 2 || isContinuous())
            {
                IppiSize roisize = { cols, rows };
                if (isContinuous())
                {
                    roisize.width = (int)total();
                    roisize.height = 1;

                    if (ippsZero_8u(data, static_cast<int>(roisize.width * elemSize())) >= 0)
                    {
                        CC_IMPL_ADD(CC_IMPL_IPP)
                        return *this;
                    }
                    setIppErrorStatus();
                }
                roisize.width *= (int)elemSize();

                if (ippiSet_8u_C1R(0, data, (int)step, roisize) >= 0)
                {
                    CC_IMPL_ADD(CC_IMPL_IPP)
                    return *this;
                }
                setIppErrorStatus();
            }
        }
#endif

        for( size_t i = 0; i < it.nplanes; i++, ++it )
            memset( dptr, 0, elsize );
    }
    else
    {
        if( it.nplanes > 0 )
        {
            double scalar[12];
            scalarToRawData(s, scalar, type(), 12);
            size_t blockSize = 12*elemSize1();

            for( size_t j = 0; j < elsize; j += blockSize )
            {
                size_t sz = MIN(blockSize, elsize - j);
                memcpy( dptr + j, scalar, sz );
            }
        }

        for( size_t i = 1; i < it.nplanes; i++ )
        {
            ++it;
            memcpy( dptr, data, elsize );
        }
    }
    return *this;
}

#if defined HAVE_IPP
static bool ipp_Mat_setTo(img_t *src, img_t &value, img_t &mask)
{
    CC_INSTRUMENT_REGION_IPP()

    int cn = src->CC_MAT_CN(), depth0 = src->depth();

    if (!mask.empty() && (src->dims <= 2 || (src->isContinuous() && mask CC_IS_CONT_MAT())) &&
            (/*depth0 == CC_8U ||*/ depth0 == CC_16U || depth0 == CC_16S || depth0 == CC_32S || depth0 == CC_32F) &&
            (cn == 1 || cn == 3 || cn == 4))
    {
        uchar _buf[32];
        void * buf = _buf;
        convertAndUnrollScalar( value, src->tid(), _buf, 1 );

        IppStatus status = (IppStatus)-1;
        IppiSize roisize = { src->cols, src->rows };
        int mstep = (int)mask->step[0], dstep = (int)src->step[0];

        if (src->isContinuous() && mask CC_IS_CONT_MAT())
        {
            roisize.width = (int)src->total();
            roisize.height = 1;
        }

        if (cn == 1)
        {
            /*if (depth0 == CC_8U)
                status = ippiSet_8u_C1MR(*(Ipp8u *)buf, (Ipp8u *)data, dstep, roisize, mask.data, mstep);
            else*/ if (depth0 == CC_16U)
                status = CC_INSTRUMENT_FUN_IPP(ippiSet_16u_C1MR, *(Ipp16u *)buf, (Ipp16u *)src->data, dstep, roisize, mask.data, mstep);
            else if (depth0 == CC_16S)
                status = CC_INSTRUMENT_FUN_IPP(ippiSet_16s_C1MR, *(Ipp16s *)buf, (Ipp16s *)src->data, dstep, roisize, mask.data, mstep);
            else if (depth0 == CC_32S)
                status = CC_INSTRUMENT_FUN_IPP(ippiSet_32s_C1MR, *(Ipp32s *)buf, (Ipp32s *)src->data, dstep, roisize, mask.data, mstep);
            else if (depth0 == CC_32F)
                status = CC_INSTRUMENT_FUN_IPP(ippiSet_32f_C1MR, *(Ipp32f *)buf, (Ipp32f *)src->data, dstep, roisize, mask.data, mstep);
        }
        else if (cn == 3 || cn == 4)
        {

#define IPP_SET(ippfavor, ippcn) \
            do \
            { \
                typedef Ipp##ippfavor ipptype; \
                ipptype ippvalue[4] = { ((ipptype *)buf)[0], ((ipptype *)buf)[1], ((ipptype *)buf)[2], ((ipptype *)buf)[3] }; \
                status = CC_INSTRUMENT_FUN_IPP(ippiSet_##ippfavor##_C##ippcn##MR, ippvalue, (ipptype *)src->data, dstep, roisize, mask.data, mstep); \
            } while ((void)0, 0)

#define IPP_SET_CN(ippcn) \
            do \
            { \
                if (cn == ippcn) \
                { \
                    /*if (depth0 == CC_8U) \
                        IPP_SET(8u, ippcn); \
                    else*/ if (depth0 == CC_16U) \
                        IPP_SET(16u, ippcn); \
                    else if (depth0 == CC_16S) \
                        IPP_SET(16s, ippcn); \
                    else if (depth0 == CC_32S) \
                        IPP_SET(32s, ippcn); \
                    else if (depth0 == CC_32F) \
                        IPP_SET(32f, ippcn); \
                } \
            } while ((void)0, 0)

            IPP_SET_CN(3);
            IPP_SET_CN(4);

#undef IPP_SET_CN
#undef IPP_SET
        }

        if (status >= 0)
            return true;
    }

    return false;
}
#endif


img_t& img_t::setTo(const img_t* _value, const img_t* _mask)
{
    CC_INSTRUMENT_REGION()

    if( empty() )
        return *this;

    img_t value = _value, mask = _mask;

    CC_Assert( checkScalar(value, type(), _value.kind(), _InputArray::MAT ));
    CC_Assert( mask.empty() || (mask CC_MAT_TYPE() == CC_8U && size == mask.size) );

    CC_IPP_RUN_FAST(ipp_Mat_setTo((img_t*)this, value, mask), *this)

    size_t esz = elemSize();
    BinaryFunc copymask = getCopyMaskFunc(esz);

    const img_t* arrays[] = { this, !mask.empty() ? &mask : 0, 0 };
    uchar* ptrs[2]={0,0};
    NAryMatIterator it(arrays, ptrs);
    int totalsz = (int)it.size, blockSize0 = MIN(totalsz, (int)((BLOCK_SIZE + esz-1)/esz));
    CAutoBuffer<uchar> _scbuf(blockSize0*esz + 32);
    uchar* scbuf = cAlignPtr((uchar*)_scbuf, (int)sizeof(double));
    convertAndUnrollScalar( value, type(), scbuf, blockSize0 );

    for( size_t i = 0; i < it.nplanes; i++, ++it )
    {
        for( int j = 0; j < totalsz; j += blockSize0 )
        {
            Size sz(MIN(blockSize0, totalsz - j), 1);
            size_t blockSize = sz.width*esz;
            if( ptrs[1] )
            {
                copymask(scbuf, 0, ptrs[1], 0, ptrs[0], 0, sz, &esz);
                ptrs[1] += sz.width;
            }
            else
                memcpy(ptrs[0], scbuf, blockSize);
            ptrs[0] += blockSize;
        }
    }
    return *this;
}


static void
flipHoriz( const uchar* src, size_t sstep, uchar* dst, size_t dstep, Size size, size_t esz )
{
    int i, j, limit = (int)(((size.width + 1)/2)*esz);
    FREE(); int* = MALLOC(int, ) _tab(size.width*esz);
    int* tab = _tab;

    for( i = 0; i < size.width; i++ )
        for( size_t k = 0; k < esz; k++ )
            tab[i*esz + k] = (int)((size.width - i - 1)*esz + k);

    for( ; size.height--; src += sstep, dst += dstep )
    {
        for( i = 0; i < limit; i++ )
        {
            j = tab[i];
            uchar t0 = src[i], t1 = src[j];
            dst[i] = t1; dst[j] = t0;
        }
    }
}

static void
flipVert( const uchar* src0, size_t sstep, uchar* dst0, size_t dstep, Size size, size_t esz )
{
    const uchar* src1 = src0 + (size.height - 1)*sstep;
    uchar* dst1 = dst0 + (size.height - 1)*dstep;
    size.width *= (int)esz;

    for( int y = 0; y < (size.height + 1)/2; y++, src0 += sstep, src1 -= sstep,
                                                  dst0 += dstep, dst1 -= dstep )
    {
        int i = 0;
        if( ((size_t)src0|(size_t)dst0|(size_t)src1|(size_t)dst1) % sizeof(int) == 0 )
        {
            for( ; i <= size.width - 16; i += 16 )
            {
                int t0 = ((int*)(src0 + i))[0];
                int t1 = ((int*)(src1 + i))[0];

                ((int*)(dst0 + i))[0] = t1;
                ((int*)(dst1 + i))[0] = t0;

                t0 = ((int*)(src0 + i))[1];
                t1 = ((int*)(src1 + i))[1];

                ((int*)(dst0 + i))[1] = t1;
                ((int*)(dst1 + i))[1] = t0;

                t0 = ((int*)(src0 + i))[2];
                t1 = ((int*)(src1 + i))[2];

                ((int*)(dst0 + i))[2] = t1;
                ((int*)(dst1 + i))[2] = t0;

                t0 = ((int*)(src0 + i))[3];
                t1 = ((int*)(src1 + i))[3];

                ((int*)(dst0 + i))[3] = t1;
                ((int*)(dst1 + i))[3] = t0;
            }

            for( ; i <= size.width - 4; i += 4 )
            {
                int t0 = ((int*)(src0 + i))[0];
                int t1 = ((int*)(src1 + i))[0];

                ((int*)(dst0 + i))[0] = t1;
                ((int*)(dst1 + i))[0] = t0;
            }
        }

        for( ; i < size.width; i++ )
        {
            uchar t0 = src0[i];
            uchar t1 = src1[i];

            dst0[i] = t1;
            dst1[i] = t0;
        }
    }
}

#ifdef HAVE_OPENCL

enum { FLIP_COLS = 1 << 0, FLIP_ROWS = 1 << 1, FLIP_BOTH = FLIP_ROWS | FLIP_COLS };

static bool ocl_flip(const img_t* _src, img_t* _dst, int flipCode )
{
    CC_Assert(flipCode >= -1 && flipCode <= 1);

    const ocl::Device & dev = ocl::Device::getDefault();
    int type = _src CC_MAT_TYPE(), depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type),
            flipType, kercn = MIN(ocl::predictOptimalVectorWidth(_src, _dst), 4);

    bool doubleSupport = dev.doubleFPConfig() > 0;
    if (!doubleSupport && depth == CC_64F)
        kercn = cn;

    if (cn > 4)
        return false;

    const char * kernelName;
    if (flipCode == 0)
        kernelName = "arithm_flip_rows", flipType = FLIP_ROWS;
    else if (flipCode > 0)
        kernelName = "arithm_flip_cols", flipType = FLIP_COLS;
    else
        kernelName = "arithm_flip_rows_cols", flipType = FLIP_BOTH;

    int pxPerWIy = (dev.isIntel() && (dev CC_MAT_TYPE() & ocl::Device::TYPE_GPU)) ? 4 : 1;
    kercn = (cn!=3 || flipType == FLIP_ROWS) ? MAX(kercn, cn) : cn;

    ocl::Kernel k(kernelName, ocl::core::flip_oclsrc,
        format( "-D T=%s -D T1=%s -D cn=%d -D PIX_PER_WI_Y=%d -D kercn=%d",
                kercn != cn ? ocl::typeToStr(CC_MAKE_TYPE(depth, kercn)) : ocl::vecopTypeToStr(CC_MAKE_TYPE(depth, kercn)),
                kercn != cn ? ocl::typeToStr(depth) : ocl::vecopTypeToStr(depth), cn, pxPerWIy, kercn));
    if (k.empty())
        return false;

    Size size = _cvGetSize(src);
    _dst cvSetMat(size, type);
    UMat src = _src.getUMat(), dst = _dst.getUMat();

    int cols = size.width * cn / kercn, rows = size.height;
    cols = flipType == FLIP_COLS ? (cols + 1) >> 1 : cols;
    rows = flipType & FLIP_ROWS ? (rows + 1) >> 1 : rows;

    k.args(ocl::KernelArg::ReadOnlyNoSize(src),
           ocl::KernelArg::WriteOnly(dst, cn, kercn), rows, cols);

    size_t maxWorkGroupSize = dev.maxWorkGroupSize();
    CC_Assert(maxWorkGroupSize % 4 == 0);

    size_t globalsize[2] = { (size_t)cols, ((size_t)rows + pxPerWIy - 1) / pxPerWIy },
            localsize[2] = { maxWorkGroupSize / 4, 4 };
    return k.run(2, globalsize, (flipType == FLIP_COLS) && !dev.isIntel() ? localsize : NULL, false);
}

#endif

#if defined HAVE_IPP
static bool ipp_flip( img_t &src, img_t &dst, int flip_mode )
{
    CC_INSTRUMENT_REGION_IPP()

    int type = src CC_MAT_TYPE();

    typedef IppStatus (C_STDCALL * IppiMirror)(const void * pSrc, int srcStep, void * pDst, int dstStep, IppiSize roiSize, IppiAxis flip);
    typedef IppStatus (C_STDCALL * IppiMirrorI)(const void * pSrcDst, int srcDstStep, IppiSize roiSize, IppiAxis flip);
    IppiMirror ippiMirror = 0;
    IppiMirrorI ippiMirror_I = 0;

    if (src.data == dst.data)
    {
        CC_SUPPRESS_DEPRECATED_START
        ippiMirror_I =
            type == CC_8UC1 ? (IppiMirrorI)ippiMirror_8u_C1IR :
            type == CC_8UC3 ? (IppiMirrorI)ippiMirror_8u_C3IR :
            type == CC_8UC4 ? (IppiMirrorI)ippiMirror_8u_C4IR :
            type == CC_16UC1 ? (IppiMirrorI)ippiMirror_16u_C1IR :
            type == CC_16UC3 ? (IppiMirrorI)ippiMirror_16u_C3IR :
            type == CC_16UC4 ? (IppiMirrorI)ippiMirror_16u_C4IR :
            type == CC_16SC1 ? (IppiMirrorI)ippiMirror_16s_C1IR :
            type == CC_16SC3 ? (IppiMirrorI)ippiMirror_16s_C3IR :
            type == CC_16SC4 ? (IppiMirrorI)ippiMirror_16s_C4IR :
            type == CC_32SC1 ? (IppiMirrorI)ippiMirror_32s_C1IR :
            type == CC_32SC3 ? (IppiMirrorI)ippiMirror_32s_C3IR :
            type == CC_32SC4 ? (IppiMirrorI)ippiMirror_32s_C4IR :
            type == CC_32FC1 ? (IppiMirrorI)ippiMirror_32f_C1IR :
            type == CC_32FC3 ? (IppiMirrorI)ippiMirror_32f_C3IR :
            type == CC_32FC4 ? (IppiMirrorI)ippiMirror_32f_C4IR : 0;
        CC_SUPPRESS_DEPRECATED_END
    }
    else
    {
        ippiMirror =
            type == CC_8UC1 ? (IppiMirror)ippiMirror_8u_C1R :
            type == CC_8UC3 ? (IppiMirror)ippiMirror_8u_C3R :
            type == CC_8UC4 ? (IppiMirror)ippiMirror_8u_C4R :
            type == CC_16UC1 ? (IppiMirror)ippiMirror_16u_C1R :
            type == CC_16UC3 ? (IppiMirror)ippiMirror_16u_C3R :
            type == CC_16UC4 ? (IppiMirror)ippiMirror_16u_C4R :
            type == CC_16SC1 ? (IppiMirror)ippiMirror_16s_C1R :
            type == CC_16SC3 ? (IppiMirror)ippiMirror_16s_C3R :
            type == CC_16SC4 ? (IppiMirror)ippiMirror_16s_C4R :
            type == CC_32SC1 ? (IppiMirror)ippiMirror_32s_C1R :
            type == CC_32SC3 ? (IppiMirror)ippiMirror_32s_C3R :
            type == CC_32SC4 ? (IppiMirror)ippiMirror_32s_C4R :
            type == CC_32FC1 ? (IppiMirror)ippiMirror_32f_C1R :
            type == CC_32FC3 ? (IppiMirror)ippiMirror_32f_C3R :
            type == CC_32FC4 ? (IppiMirror)ippiMirror_32f_C4R : 0;
    }
    IppiAxis axis = flip_mode == 0 ? ippAxsHorizontal :
        flip_mode > 0 ? ippAxsVertical : ippAxsBoth;
    IppiSize roisize = { dst->cols, dst->rows };

    if (ippiMirror != 0)
    {
        if (CC_INSTRUMENT_FUN_IPP(ippiMirror, src->tt.data, (int)src->step, dst->tt.data, (int)dst->step, ippiSize(src->cols, src->rows), axis) >= 0)
            return true;
    }
    else if (ippiMirror_I != 0)
    {
        if (CC_INSTRUMENT_FUN_IPP(ippiMirror_I, dst->tt.data, (int)dst->step, roisize, axis) >= 0)
            return true;
    }

    return false;
}
#endif


void flip( const img_t* _src, img_t* _dst, int flip_mode )
{
    CC_INSTRUMENT_REGION()

    CC_Assert( _src.dims() <= 2 );
    Size size = _cvGetSize(src);

    if (flip_mode < 0)
    {
        if (size.width == 1)
            flip_mode = 0;
        if (size.height == 1)
            flip_mode = 1;
    }

    if ((size.width == 1 && flip_mode > 0) ||
        (size.height == 1 && flip_mode == 0) ||
        (size.height == 1 && size.width == 1 && flip_mode < 0))
    {
        return _src.copyTo(_dst);
    }

    CC_OCL_RUN( _dst.isUMat(), ocl_flip(_src, _dst, flip_mode))

    img_t *src = _src;
    int type = src CC_MAT_TYPE();
    _dst cvSetMat( size, type );
    img_t *dst = _dst;

    CC_IPP_RUN_FAST(ipp_flip(src, dst, flip_mode));

    size_t esz = CC_ELEM_SIZE(type);

    if( flip_mode <= 0 )
        flipVert( src->tt.data, src->step, dst->tt.data, dst->step, cvGetSize(src), esz );
    else
        flipHoriz( src->tt.data, src->step, dst->tt.data, dst->step, cvGetSize(src), esz );

    if( flip_mode < 0 )
        flipHoriz( dst->tt.data, dst->step, dst->tt.data, dst->step, dst.size(), esz );
}

#ifdef HAVE_OPENCL

static bool ocl_rotate(const img_t* _src, img_t* _dst, int rotateMode)
{
    switch (rotateMode)
    {
    case ROTATE_90_CLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 1);
        break;
    case ROTATE_180:
        flip(_src, _dst, -1);
        break;
    case ROTATE_90_COUNTERCLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 0);
        break;
    default:
        break;
    }
    return true;
}
#endif

void rotate(const img_t* _src, img_t* _dst, int rotateMode)
{
    CC_Assert(_src.dims() <= 2);

    CC_OCL_RUN(_dst.isUMat(), ocl_rotate(_src, _dst, rotateMode))

    switch (rotateMode)
    {
    case ROTATE_90_CLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 1);
        break;
    case ROTATE_180:
        flip(_src, _dst, -1);
        break;
    case ROTATE_90_COUNTERCLOCKWISE:
        transpose(_src, _dst);
        flip(_dst, _dst, 0);
        break;
    default:
        break;
    }
}

#if defined HAVE_OPENCL && !defined __APPLE__

static bool ocl_repeat(const img_t* _src, int ny, int nx, img_t* _dst)
{
    if (ny == 1 && nx == 1)
    {
        _src.copyTo(_dst);
        return true;
    }

    int type = _src CC_MAT_TYPE(), depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1,
            kercn = ocl::predictOptimalVectorWidth(_src, _dst);

    ocl::Kernel k("repeat", ocl::core::repeat_oclsrc,
                  format("-D T=%s -D nx=%d -D ny=%d -D rowsPerWI=%d -D cn=%d",
                         ocl::memopTypeToStr(CC_MAKE_TYPE(depth, kercn)),
                         nx, ny, rowsPerWI, kercn));
    if (k.empty())
        return false;

    UMat src = _src.getUMat(), dst = _dst.getUMat();
    k.args(ocl::KernelArg::ReadOnly(src, cn, kercn), ocl::KernelArg::WriteOnlyNoSize(dst));

    size_t globalsize[] = { (size_t)src->cols * cn / kercn, ((size_t)src->rows + rowsPerWI - 1) / rowsPerWI };
    return k.run(2, globalsize, NULL, false);
}

#endif

void repeat(const img_t* _src, int ny, int nx, img_t* _dst)
{
    CC_INSTRUMENT_REGION()

    CC_Assert( _src.dims() <= 2 );
    CC_Assert( ny > 0 && nx > 0 );

    Size ssize = _cvGetSize(src);
    _dst cvSetMat(ssize.height*ny, ssize.width*nx, _src CC_MAT_TYPE());

#if !defined __APPLE__
    CC_OCL_RUN(_dst.isUMat(),
               ocl_repeat(_src, ny, nx, _dst))
#endif

    img_t *src = _src, dst = _dst;
    Size dsize = dst.size();
    int esz = (int)src CC_ELEM_SIZE();
    int x, y;
    ssize.width *= esz; dsize.width *= esz;

    for( y = 0; y < ssize.height; y++ )
    {
        for( x = 0; x < dsize.width; x += ssize.width )
            memcpy( dst.ptr(y) + x, src.ptr(y), ssize.width );
    }

    for( ; y < dsize.height; y++ )
        memcpy( dst.ptr(y), dst.ptr(y - ssize.height), dsize.width );
}

img_t repeat(const img_t& src, int ny, int nx)
{
    if( nx == 1 && ny == 1 )
        return src;
    img_t dst;
    repeat(src, ny, nx, dst);
    return dst;
}


#endif

/*
 Various border types, image boundaries are denoted with '|'

 * CC_BORDER_REPLICATE:     aaaaaa|abcdefgh|hhhhhhh
 * CC_BORDER_REFLECT:       fedcba|abcdefgh|hgfedcb
 * CC_BORDER_REFLECT_101:   gfedcb|abcdefgh|gfedcba
 * CC_BORDER_WRAP:          cdefgh|abcdefgh|abcdefg
 * CC_BORDER_CONSTANT:      iiiiii|abcdefgh|iiiiiii  with some specified 'i'
 */
static int cBorderInterpolate( int p, int len, int borderType )
{
    CC_INSTRUMENT_REGION()

    if( (unsigned)p < (unsigned)len )
        ;
    else if( borderType == CC_BORDER_REPLICATE )
        p = p < 0 ? 0 : len - 1;
    else if( borderType == CC_BORDER_REFLECT || borderType == CC_BORDER_REFLECT_101 )
    {
        int delta = borderType == CC_BORDER_REFLECT_101;
        if( len == 1 )
            return 0;
        do
        {
            if( p < 0 )
                p = -p - 1 + delta;
            else
                p = len - 1 - (p - len) - delta;
        }
        while( (unsigned)p >= (unsigned)len );
    }
    else if( borderType == CC_BORDER_WRAP )
    {
        CC_Assert(len > 0);
        if( p < 0 )
            p -= ((p-len+1)/len)*len;
        if( p >= len )
            p %= len;
    }
    else if( borderType == CC_BORDER_CONSTANT )
        p = -1;
    else
        CC_Error( CC_StsBadArg, "Unknown/unsupported border type" );
    return p;
}
#if 0

void copyMakeBorder_8u( const uchar* src, size_t srcstep, Size srcroi,
                        uchar* dst, size_t dststep, Size dstroi,
                        int top, int left, int cn, int borderType )
{
    const int isz = (int)sizeof(int);
    int i, j, k, elemSize = 1;
    bool intMode = false;

    if( (cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0 )
    {
        cn /= isz;
        elemSize = isz;
        intMode = true;
    }

    FREE(); int* = MALLOC(int, ) _tab((dstroi.width - srcroi.width)*cn);
    int* tab = _tab;
    int right = dstroi.width - srcroi.width - left;
    int bottom = dstroi.height - srcroi.height - top;

    for( i = 0; i < left; i++ )
    {
        j = borderInterpolate(i - left, srcroi.width, borderType)*cn;
        for( k = 0; k < cn; k++ )
            tab[i*cn + k] = j + k;
    }

    for( i = 0; i < right; i++ )
    {
        j = borderInterpolate(srcroi.width + i, srcroi.width, borderType)*cn;
        for( k = 0; k < cn; k++ )
            tab[(i+left)*cn + k] = j + k;
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    left *= cn;
    right *= cn;

    uchar* dstInner = dst + dststep*top + left*elemSize;

    for( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        if( dstInner != src )
            memcpy(dstInner, src, srcroi.width*elemSize);

        if( intMode )
        {
            const int* isrc = (int*)src;
            int* idstInner = (int*)dstInner;
            for( j = 0; j < left; j++ )
                idstInner[j - left] = isrc[tab[j]];
            for( j = 0; j < right; j++ )
                idstInner[j + srcroi.width] = isrc[tab[j + left]];
        }
        else
        {
            for( j = 0; j < left; j++ )
                dstInner[j - left] = src[tab[j]];
            for( j = 0; j < right; j++ )
                dstInner[j + srcroi.width] = src[tab[j + left]];
        }
    }

    dstroi.width *= elemSize;
    dst += dststep*top;

    for( i = 0; i < top; i++ )
    {
        j = borderInterpolate(i - top, srcroi.height, borderType);
        memcpy(dst + (i - top)*dststep, dst + j*dststep, dstroi.width);
    }

    for( i = 0; i < bottom; i++ )
    {
        j = borderInterpolate(i + srcroi.height, srcroi.height, borderType);
        memcpy(dst + (i + srcroi.height)*dststep, dst + j*dststep, dstroi.width);
    }
}


void copyMakeConstBorder_8u( const uchar* src, size_t srcstep, Size srcroi,
                             uchar* dst, size_t dststep, Size dstroi,
                             int top, int left, int cn, const uchar* value )
{
    int i, j;
    CAutoBuffer<uchar> _constBuf(dstroi.width*cn);
    uchar* constBuf = _constBuf;
    int right = dstroi.width - srcroi.width - left;
    int bottom = dstroi.height - srcroi.height - top;

    for( i = 0; i < dstroi.width; i++ )
    {
        for( j = 0; j < cn; j++ )
            constBuf[i*cn + j] = value[j];
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    left *= cn;
    right *= cn;

    uchar* dstInner = dst + dststep*top + left;

    for( i = 0; i < srcroi.height; i++, dstInner += dststep, src += srcstep )
    {
        if( dstInner != src )
            memcpy( dstInner, src, srcroi.width );
        memcpy( dstInner - left, constBuf, left );
        memcpy( dstInner + srcroi.width, constBuf, right );
    }

    dst += dststep*top;

    for( i = 0; i < top; i++ )
        memcpy(dst + (i - top)*dststep, constBuf, dstroi.width);

    for( i = 0; i < bottom; i++ )
        memcpy(dst + (i + srcroi.height)*dststep, constBuf, dstroi.width);
}

#ifdef HAVE_OPENCL

static bool ocl_copyMakeBorder( const img_t* _src, img_t* _dst, int top, int bottom,
                                int left, int right, int borderType, const Scalar& value )
{
    int type = _src CC_MAT_TYPE(), cn = CC_MAT_CN(type), depth = CC_MAT_DEPTH(type),
            rowsPerWI = ocl::Device::getDefault().isIntel() ? 4 : 1;
    bool isolated = (borderType & CC_BORDER_ISOLATED) != 0;
    borderType &= ~CC_BORDER_ISOLATED;

    if ( !(borderType == CC_BORDER_CONSTANT || borderType == CC_BORDER_REPLICATE || borderType == CC_BORDER_REFLECT ||
           borderType == CC_BORDER_WRAP || borderType == CC_BORDER_REFLECT_101) ||
         cn > 4)
        return false;

    const char * const borderMap[] = { "CC_BORDER_CONSTANT", "CC_BORDER_REPLICATE", "CC_BORDER_REFLECT", "CC_BORDER_WRAP", "CC_BORDER_REFLECT_101" };
    int scalarcn = cn == 3 ? 4 : cn;
    int sctype = CC_MAKETYPE(depth, scalarcn);
    String buildOptions = format("-D T=%s -D %s -D T1=%s -D cn=%d -D ST=%s -D rowsPerWI=%d",
                                 ocl::memopTypeToStr(type), borderMap[borderType],
                                 ocl::memopTypeToStr(depth), cn,
                                 ocl::memopTypeToStr(sctype), rowsPerWI);

    ocl::Kernel k("copyMakeBorder", ocl::core::copymakeborder_oclsrc, buildOptions);
    if (k.empty())
        return false;

    UMat src = _src.getUMat();
    if( src.isSubmatrix() && !isolated )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        int dtop = MIN(ofs.y, top);
        int dbottom = MIN(wholeSize.height - src->rows - ofs.y, bottom);
        int dleft = MIN(ofs.x, left);
        int dright = MIN(wholeSize.width - src->cols - ofs.x, right);
        src.adjustROI(dtop, dbottom, dleft, dright);
        top -= dtop;
        left -= dleft;
        bottom -= dbottom;
        right -= dright;
    }

    _dst cvSetMat(src->rows + top + bottom, src->cols + left + right, type);
    UMat dst = _dst.getUMat();

    if (top == 0 && left == 0 && bottom == 0 && right == 0)
    {
        if(src.u != dst.u || src->step != dst->step)
            src.copyTo(dst);
        return true;
    }

    k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
           top, left, ocl::KernelArg::Constant(img_t(1, 1, sctype, value)));

    size_t globalsize[2] = { (size_t)dst->cols, ((size_t)dst->rows + rowsPerWI - 1) / rowsPerWI };
    return k.run(2, globalsize, NULL, false);
}

#endif

void copyMakeBorder( const img_t* _src, img_t* _dst, int top, int bottom,
                         int left, int right, int borderType, const Scalar& value )
{
    CC_INSTRUMENT_REGION()

    CC_Assert( top >= 0 && bottom >= 0 && left >= 0 && right >= 0 );

    CC_OCL_RUN(_dst.isUMat() && _src.dims() <= 2,
               ocl_copyMakeBorder(_src, _dst, top, bottom, left, right, borderType, value))

    img_t *src = _src;
    int type = src CC_MAT_TYPE();

    if( src.isSubmatrix() && (borderType & CC_BORDER_ISOLATED) == 0 )
    {
        Size wholeSize;
        Point ofs;
        src.locateROI(wholeSize, ofs);
        int dtop = MIN(ofs.y, top);
        int dbottom = MIN(wholeSize.height - src->rows - ofs.y, bottom);
        int dleft = MIN(ofs.x, left);
        int dright = MIN(wholeSize.width - src->cols - ofs.x, right);
        src.adjustROI(dtop, dbottom, dleft, dright);
        top -= dtop;
        left -= dleft;
        bottom -= dbottom;
        right -= dright;
    }

    _dst cvSetMat( src->rows + top + bottom, src->cols + left + right, type );
    img_t *dst = _dst;

    if(top == 0 && left == 0 && bottom == 0 && right == 0)
    {
        if(src.data != dst.data || src->step != dst->step)
            src.copyTo(dst);
        return;
    }

    borderType &= ~CC_BORDER_ISOLATED;

#if defined HAVE_IPP && IPP_DISABLE_BLOCK
    CC_IPP_CHECK()
    {
        typedef IppStatus (C_STDCALL * ippiCopyMakeBorder)(const void * pSrc, int srcStep, IppiSize srcRoiSize, void * pDst,
                                                            int dstStep, IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth);
        typedef IppStatus (C_STDCALL * ippiCopyMakeBorderI)(const void * pSrc, int srcDstStep, IppiSize srcRoiSize, IppiSize dstRoiSize,
                                                             int topBorderHeight, int leftborderwidth);
        typedef IppStatus (C_STDCALL * ippiCopyConstBorder)(const void * pSrc, int srcStep, IppiSize srcRoiSize, void * pDst, int dstStep,
                                                             IppiSize dstRoiSize, int topBorderHeight, int leftBorderWidth, void * value);

        IppiSize srcRoiSize = { src->cols, src->rows }, dstRoiSize = { dst->cols, dst->rows };
        ippiCopyMakeBorder ippFunc = 0;
        ippiCopyMakeBorderI ippFuncI = 0;
        ippiCopyConstBorder ippFuncConst = 0;
        bool inplace = dst.datastart == src.datastart;

        if (borderType == CC_BORDER_CONSTANT)
        {
             ippFuncConst =
    //             type == CC_8UC1 ? (ippiCopyConstBorder)ippiCopyConstBorder_8u_C1R : bug in IPP 8.1
                 type == CC_16UC1 ? (ippiCopyConstBorder)ippiCopyConstBorder_16u_C1R :
    //             type == CC_16SC1 ? (ippiCopyConstBorder)ippiCopyConstBorder_16s_C1R : bug in IPP 8.1
    //             type == CC_32SC1 ? (ippiCopyConstBorder)ippiCopyConstBorder_32s_C1R : bug in IPP 8.1
    //             type == CC_32FC1 ? (ippiCopyConstBorder)ippiCopyConstBorder_32f_C1R : bug in IPP 8.1
                 type == CC_8UC3 ? (ippiCopyConstBorder)ippiCopyConstBorder_8u_C3R :
                 type == CC_16UC3 ? (ippiCopyConstBorder)ippiCopyConstBorder_16u_C3R :
                 type == CC_16SC3 ? (ippiCopyConstBorder)ippiCopyConstBorder_16s_C3R :
                 type == CC_32SC3 ? (ippiCopyConstBorder)ippiCopyConstBorder_32s_C3R :
                 type == CC_32FC3 ? (ippiCopyConstBorder)ippiCopyConstBorder_32f_C3R :
                 type == CC_8UC4 ? (ippiCopyConstBorder)ippiCopyConstBorder_8u_C4R :
                 type == CC_16UC4 ? (ippiCopyConstBorder)ippiCopyConstBorder_16u_C4R :
                 type == CC_16SC4 ? (ippiCopyConstBorder)ippiCopyConstBorder_16s_C4R :
                 type == CC_32SC4 ? (ippiCopyConstBorder)ippiCopyConstBorder_32s_C4R :
                 type == CC_32FC4 ? (ippiCopyConstBorder)ippiCopyConstBorder_32f_C4R : 0;
        }
        else if (borderType == CC_BORDER_WRAP)
        {
            if (inplace)
            {
                CC_SUPPRESS_DEPRECATED_START
                ippFuncI =
                    type == CC_32SC1 ? (ippiCopyMakeBorderI)ippiCopyWrapBorder_32s_C1IR :
                    type == CC_32FC1 ? (ippiCopyMakeBorderI)ippiCopyWrapBorder_32s_C1IR : 0;
                CC_SUPPRESS_DEPRECATED_END
            }
            else
            {
                ippFunc =
                    type == CC_32SC1 ? (ippiCopyMakeBorder)ippiCopyWrapBorder_32s_C1R :
                    type == CC_32FC1 ? (ippiCopyMakeBorder)ippiCopyWrapBorder_32s_C1R : 0;
            }
        }
        else if (borderType == CC_BORDER_REPLICATE)
        {
            if (inplace)
            {
                CC_SUPPRESS_DEPRECATED_START
                ippFuncI =
                    type == CC_8UC1 ? (ippiCopyMakeBorderI)ippiCopyReplicateBorder_8u_C1IR :
                    type == CC_16UC1 ? (ippiCopyMakeBorderI)ippiCopyReplicateBorder_16u_C1IR :
                    type == CC_16SC1 ? (ippiCopyMakeBorderI)ippiCopyReplicateBorder_16s_C1IR :
                    type == CC_32SC1 ? (ippiCopyMakeBorderI)ippiCopyReplicateBorder_32s_C1IR :
                    type == CC_32FC1 ? (ippiCopyMakeBorderI)ippiCopyReplicateBorder_32f_C1IR :
                    type == CC_8UC3 ? (ippiCopyMakeBorderI)ippiCopyReplicateBorder_8u_C3IR :
                    type == CC_16UC3 ? (ippiCopyMakeBorderI)ippiCopyReplicateBorder_16u_C3IR :
                    type == CC_16SC3 ? (ippiCopyMakeBorderI)ippiCopyReplicateBorder_16s_C3IR :
                    type == CC_32SC3 ? (ippiCopyMakeBorderI)ippiCopyReplicateBorder_32s_C3IR :
                    type == CC_32FC3 ? (ippiCopyMakeBorderI)ippiCopyReplicateBorder_32f_C3IR :
                    type == CC_8UC4 ? (ippiCopyMakeBorderI)ippiCopyReplicateBorder_8u_C4IR :
                    type == CC_16UC4 ? (ippiCopyMakeBorderI)ippiCopyReplicateBorder_16u_C4IR :
                    type == CC_16SC4 ? (ippiCopyMakeBorderI)ippiCopyReplicateBorder_16s_C4IR :
                    type == CC_32SC4 ? (ippiCopyMakeBorderI)ippiCopyReplicateBorder_32s_C4IR :
                    type == CC_32FC4 ? (ippiCopyMakeBorderI)ippiCopyReplicateBorder_32f_C4IR : 0;
                CC_SUPPRESS_DEPRECATED_END
            }
            else
            {
                 ippFunc =
                     type == CC_8UC1 ? (ippiCopyMakeBorder)ippiCopyReplicateBorder_8u_C1R :
                     type == CC_16UC1 ? (ippiCopyMakeBorder)ippiCopyReplicateBorder_16u_C1R :
                     type == CC_16SC1 ? (ippiCopyMakeBorder)ippiCopyReplicateBorder_16s_C1R :
                     type == CC_32SC1 ? (ippiCopyMakeBorder)ippiCopyReplicateBorder_32s_C1R :
                     type == CC_32FC1 ? (ippiCopyMakeBorder)ippiCopyReplicateBorder_32f_C1R :
                     type == CC_8UC3 ? (ippiCopyMakeBorder)ippiCopyReplicateBorder_8u_C3R :
                     type == CC_16UC3 ? (ippiCopyMakeBorder)ippiCopyReplicateBorder_16u_C3R :
                     type == CC_16SC3 ? (ippiCopyMakeBorder)ippiCopyReplicateBorder_16s_C3R :
                     type == CC_32SC3 ? (ippiCopyMakeBorder)ippiCopyReplicateBorder_32s_C3R :
                     type == CC_32FC3 ? (ippiCopyMakeBorder)ippiCopyReplicateBorder_32f_C3R :
                     type == CC_8UC4 ? (ippiCopyMakeBorder)ippiCopyReplicateBorder_8u_C4R :
                     type == CC_16UC4 ? (ippiCopyMakeBorder)ippiCopyReplicateBorder_16u_C4R :
                     type == CC_16SC4 ? (ippiCopyMakeBorder)ippiCopyReplicateBorder_16s_C4R :
                     type == CC_32SC4 ? (ippiCopyMakeBorder)ippiCopyReplicateBorder_32s_C4R :
                     type == CC_32FC4 ? (ippiCopyMakeBorder)ippiCopyReplicateBorder_32f_C4R : 0;
            }
        }

        if (ippFunc || ippFuncI || ippFuncConst)
        {
            uchar scbuf[32];
            scalarToRawData(value, scbuf, type);

            if ( (ippFunc && ippFunc(src.data, (int)src->step, srcRoiSize, dst.data, (int)dst->step, dstRoiSize, top, left) >= 0) ||
                 (ippFuncI && ippFuncI(src.data, (int)src->step, srcRoiSize, dstRoiSize, top, left) >= 0) ||
                 (ippFuncConst && ippFuncConst(src.data, (int)src->step, srcRoiSize, dst.data, (int)dst->step,
                                               dstRoiSize, top, left, scbuf) >= 0))
            {
                CC_IMPL_ADD(CC_IMPL_IPP);
                return;
            }

            setIppErrorStatus();
        }
    }
#endif

    if( borderType != CC_BORDER_CONSTANT )
        copyMakeBorder_8u( src->tt.data, src->step, cvGetSize(src),
                           dst->tt.data, dst->step, dst.size(),
                           top, left, (int)src CC_ELEM_SIZE(), borderType );
    else
    {
        int cn = src CC_MAT_CN(), cn1 = cn;
        FREE(); double* = MALLOC(double, ) buf(cn);
        if( cn > 4 )
        {
            CC_Assert( value[0] == value[1] && value[0] == value[2] && value[0] == value[3] );
            cn1 = 1;
        }
        scalarToRawData(value, buf, CC_MAKETYPE(src CC_MAT_DEPTH(), cn1), cn);
        copyMakeConstBorder_8u( src->tt.data, src->step, cvGetSize(src),
                                dst->tt.data, dst->step, dst.size(),
                                top, left, (int)src CC_ELEM_SIZE(), (uchar*)(double*)buf );
    }
}

/* dst = src */
CC_IMPL void
cvCopy( const void* srcarr, void* dstarr, const void* maskarr )
{
    if( CC_IS_SPARSE_MAT(srcarr) && CC_IS_SPARSE_MAT(dstarr))
    {
        CC_Assert( maskarr == 0 );
        CvSparseMat* src1 = (CvSparseMat*)srcarr;
        CvSparseMat* dst1 = (CvSparseMat*)dstarr;
        CvSparseMatIterator iterator;
        CvSparseNode* node;

        dst1->dims = src1->dims;
        memcpy( dst1->size, src1->size, src1->dims*sizeof(src1->size[0]));
        dst1->valoffset = src1->valoffset;
        dst1->idxoffset = src1->idxoffset;
        cvClearSet( dst1->heap );

        if( src1->heap->active_count >= dst1->hashsize*CC_SPARSE_HASH_RATIO )
        {
            cFree( &dst1->hashtable );
            dst1->hashsize = src1->hashsize;
            dst1->hashtable =
                (void**)cAlloc( dst1->hashsize*sizeof(dst1->hashtable[0]));
        }

        memset( dst1->hashtable, 0, dst1->hashsize*sizeof(dst1->hashtable[0]));

        for( node = cvInitSparseMatIterator( src1, &iterator );
             node != 0; node = cvGetNextSparseNode( &iterator ))
        {
            CvSparseNode* node_copy = (CvSparseNode*)cvSetNew( dst1->heap );
            int tabidx = node->hashval & (dst1->hashsize - 1);
            memcpy( node_copy, node, dst1->heap->elem_size );
            node_copy->next = (CvSparseNode*)dst1->hashtable[tabidx];
            dst1->hashtable[tabidx] = node_copy;
        }
        return;
    }
    img_t src = cvarrToMat(srcarr, false, true, 1), dst = cvarrToMat(dstarr, false, true, 1);
    CC_Assert( src CC_MAT_DEPTH() == dst CC_MAT_DEPTH() && src.size == dst.size );

    int coi1 = 0, coi2 = 0;
    if( CC_IS_IMAGE(srcarr) )
        coi1 = cvGetImageCOI((const img_t*)srcarr);
    if( CC_IS_IMAGE(dstarr) )
        coi2 = cvGetImageCOI((const img_t*)dstarr);

    if( coi1 || coi2 )
    {
        CC_Assert( (coi1 != 0 || src CC_MAT_CN() == 1) &&
            (coi2 != 0 || dst CC_MAT_CN() == 1) );

        int pair[] = { MAX(coi1-1, 0), MAX(coi2-1, 0) };
        mixChannels( &src, 1, &dst, 1, pair, 1 );
        return;
    }
    else
        CC_Assert( src CC_MAT_CN() == dst CC_MAT_CN() );

    if( !maskarr )
        src.copyTo(dst);
    else
        src.copyTo(dst, cvarrToMat(maskarr));
}

CC_IMPL void
cvSet( void* arr, CScalar value, const void* maskarr )
{
    img_t m = cvarrToMat(arr);
    if( !maskarr )
        m = value;
    else
        m.setTo(Scalar(value), cvarrToMat(maskarr));
}

CC_IMPL void
cvSetZero( img_t* arr )
{
    if( CC_IS_SPARSE_MAT(arr) )
    {
        CvSparseMat* mat1 = (CvSparseMat*)arr;
        cvClearSet( mat1->heap );
        if( mat1->hashtable )
            memset( mat1->hashtable, 0, mat1->hashsize*sizeof(mat1->hashtable[0]));
        return;
    }
    img_t m = cvarrToMat(arr);
    m = Scalar(0);
}

CC_IMPL void
cvFlip( const img_t* srcarr, img_t* dstarr, int flip_mode )
{
    img_t src = cvarrToMat(srcarr);
    img_t dst;

    if (!dstarr)
      dst = src;
    else
      dst = cvarrToMat(dstarr);

    CC_Assert( src CC_MAT_TYPE() == dst CC_MAT_TYPE() && cvGetSize(src) == dst.size() );
    flip( src, dst, flip_mode );
}

CC_IMPL void
cvRepeat( const img_t* srcarr, img_t* dstarr )
{
    img_t src = cvarrToMat(srcarr), dst = cvarrToMat(dstarr);
    CC_Assert( src CC_MAT_TYPE() == dst CC_MAT_TYPE() &&
        dst->rows % src->rows == 0 && dst->cols % src->cols == 0 );
    repeat(src, dst->rows/src->rows, dst->cols/src->cols, dst);
}


#endif