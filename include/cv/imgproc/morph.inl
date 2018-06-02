
//#include "opencl_kernels_imgproc.inl"

/****************************************************************************************\
                     Basic Morphological Operations: Erosion & Dilation
\****************************************************************************************/


template<typename T> struct MinOp
{
    typedef T type1;
    typedef T type2;
    typedef T rtype;
    T operator ()(const T a, const T b) const { return MIN(a, b); }
};

template<typename T> struct MaxOp
{
    typedef T type1;
    typedef T type2;
    typedef T rtype;
    T operator ()(const T a, const T b) const { return MAX(a, b); }
};

#undef CC_MIN_8U
#undef CC_MAX_8U
#define CC_MIN_8U(a,b)       ((a) - CC_FAST_CAST_8U((a) - (b)))
#define CC_MAX_8U(a,b)       ((a) + CC_FAST_CAST_8U((b) - (a)))

template<> inline uchar MinOp<uchar>::operator ()(const uchar a, const uchar b) const { return CC_MIN_8U(a, b); }
template<> inline uchar MaxOp<uchar>::operator ()(const uchar a, const uchar b) const { return CC_MAX_8U(a, b); }

struct MorphRowNoVec
{
    MorphRowNoVec(int, int) {}
    int operator()(const uchar*, uchar*, int, int) const { return 0; }
};

struct MorphColumnNoVec
{
    MorphColumnNoVec(int, int) {}
    int operator()(const uchar**, uchar*, int, int, int) const { return 0; }
};

struct MorphNoVec
{
    int operator()(uchar**, int, uchar*, int) const { return 0; }
};

#if CC_SSE2

template<class VecUpdate> struct MorphRowIVec
{
    enum { ESZ = VecUpdate::ESZ };

    MorphRowIVec(int _ksize, int _anchor) : ksize(_ksize), anchor(_anchor) {}
    int operator()(const uchar* src, uchar* dst, int width, int cn) const
    {
        if( !checkHardwareSupport(CC_CPU_SSE2) )
            return 0;

        cn *= ESZ;
        int i, k, _ksize = ksize*cn;
        width = (width & -4)*cn;
        VecUpdate updateOp;

        for( i = 0; i <= width - 16; i += 16 )
        {
            __m128i s = _mm_loadu_si128((const __m128i*)(src + i));
            for( k = cn; k < _ksize; k += cn )
            {
                __m128i x = _mm_loadu_si128((const __m128i*)(src + i + k));
                s = updateOp(s, x);
            }
            _mm_storeu_si128((__m128i*)(dst + i), s);
        }

        for( ; i < width; i += 4 )
        {
            __m128i s = _mm_cvtsi32_si128(*(const int*)(src + i));
            for( k = cn; k < _ksize; k += cn )
            {
                __m128i x = _mm_cvtsi32_si128(*(const int*)(src + i + k));
                s = updateOp(s, x);
            }
            *(int*)(dst + i) = _mm_cvtsi128_si32(s);
        }

        return i/ESZ;
    }

    int ksize, anchor;
};


template<class VecUpdate> struct MorphRowFVec
{
    MorphRowFVec(int _ksize, int _anchor) : ksize(_ksize), anchor(_anchor) {}
    int operator()(const uchar* src, uchar* dst, int width, int cn) const
    {
        if( !checkHardwareSupport(CC_CPU_SSE) )
            return 0;

        int i, k, _ksize = ksize*cn;
        width = (width & -4)*cn;
        VecUpdate updateOp;

        for( i = 0; i < width; i += 4 )
        {
            __m128 s = _mm_loadu_ps((const float*)src + i);
            for( k = cn; k < _ksize; k += cn )
            {
                __m128 x = _mm_loadu_ps((const float*)src + i + k);
                s = updateOp(s, x);
            }
            _mm_storeu_ps((float*)dst + i, s);
        }

        return i;
    }

    int ksize, anchor;
};


template<class VecUpdate> struct MorphColumnIVec
{
    enum { ESZ = VecUpdate::ESZ };

    MorphColumnIVec(int _ksize, int _anchor) : ksize(_ksize), anchor(_anchor) {}
    int operator()(const uchar** src, uchar* dst, int dststep, int count, int width) const
    {
        if( !checkHardwareSupport(CC_CPU_SSE2) )
            return 0;

        int i = 0, k, _ksize = ksize;
        width *= ESZ;
        VecUpdate updateOp;

        for( i = 0; i < count + ksize - 1; i++ )
            CC_Assert( ((size_t)src[i] & 15) == 0 );

        for( ; _ksize > 1 && count > 1; count -= 2, dst += dststep*2, src += 2 )
        {
            for( i = 0; i <= width - 32; i += 32 )
            {
                const uchar* sptr = src[1] + i;
                __m128i s0 = _mm_load_si128((const __m128i*)sptr);
                __m128i s1 = _mm_load_si128((const __m128i*)(sptr + 16));
                __m128i x0, x1;

                for( k = 2; k < _ksize; k++ )
                {
                    sptr = src[k] + i;
                    x0 = _mm_load_si128((const __m128i*)sptr);
                    x1 = _mm_load_si128((const __m128i*)(sptr + 16));
                    s0 = updateOp(s0, x0);
                    s1 = updateOp(s1, x1);
                }

                sptr = src[0] + i;
                x0 = _mm_load_si128((const __m128i*)sptr);
                x1 = _mm_load_si128((const __m128i*)(sptr + 16));
                _mm_storeu_si128((__m128i*)(dst + i), updateOp(s0, x0));
                _mm_storeu_si128((__m128i*)(dst + i + 16), updateOp(s1, x1));

                sptr = src[k] + i;
                x0 = _mm_load_si128((const __m128i*)sptr);
                x1 = _mm_load_si128((const __m128i*)(sptr + 16));
                _mm_storeu_si128((__m128i*)(dst + dststep + i), updateOp(s0, x0));
                _mm_storeu_si128((__m128i*)(dst + dststep + i + 16), updateOp(s1, x1));
            }

            for( ; i <= width - 8; i += 8 )
            {
                __m128i s0 = _mm_loadl_epi64((const __m128i*)(src[1] + i)), x0;

                for( k = 2; k < _ksize; k++ )
                {
                    x0 = _mm_loadl_epi64((const __m128i*)(src[k] + i));
                    s0 = updateOp(s0, x0);
                }

                x0 = _mm_loadl_epi64((const __m128i*)(src[0] + i));
                _mm_storel_epi64((__m128i*)(dst + i), updateOp(s0, x0));
                x0 = _mm_loadl_epi64((const __m128i*)(src[k] + i));
                _mm_storel_epi64((__m128i*)(dst + dststep + i), updateOp(s0, x0));
            }
        }

        for( ; count > 0; count--, dst += dststep, src++ )
        {
            for( i = 0; i <= width - 32; i += 32 )
            {
                const uchar* sptr = src[0] + i;
                __m128i s0 = _mm_load_si128((const __m128i*)sptr);
                __m128i s1 = _mm_load_si128((const __m128i*)(sptr + 16));
                __m128i x0, x1;

                for( k = 1; k < _ksize; k++ )
                {
                    sptr = src[k] + i;
                    x0 = _mm_load_si128((const __m128i*)sptr);
                    x1 = _mm_load_si128((const __m128i*)(sptr + 16));
                    s0 = updateOp(s0, x0);
                    s1 = updateOp(s1, x1);
                }
                _mm_storeu_si128((__m128i*)(dst + i), s0);
                _mm_storeu_si128((__m128i*)(dst + i + 16), s1);
            }

            for( ; i <= width - 8; i += 8 )
            {
                __m128i s0 = _mm_loadl_epi64((const __m128i*)(src[0] + i)), x0;

                for( k = 1; k < _ksize; k++ )
                {
                    x0 = _mm_loadl_epi64((const __m128i*)(src[k] + i));
                    s0 = updateOp(s0, x0);
                }
                _mm_storel_epi64((__m128i*)(dst + i), s0);
            }
        }

        return i/ESZ;
    }

    int ksize, anchor;
};


template<class VecUpdate> struct MorphColumnFVec
{
    MorphColumnFVec(int _ksize, int _anchor) : ksize(_ksize), anchor(_anchor) {}
    int operator()(const uchar** _src, uchar* _dst, int dststep, int count, int width) const
    {
        if( !checkHardwareSupport(CC_CPU_SSE) )
            return 0;

        int i = 0, k, _ksize = ksize;
        VecUpdate updateOp;

        for( i = 0; i < count + ksize - 1; i++ )
            CC_Assert( ((size_t)_src[i] & 15) == 0 );

        const float** src = (const float**)_src;
        float* dst = (float*)_dst;
        dststep /= sizeof(dst[0]);

        for( ; _ksize > 1 && count > 1; count -= 2, dst += dststep*2, src += 2 )
        {
            for( i = 0; i <= width - 16; i += 16 )
            {
                const float* sptr = src[1] + i;
                __m128 s0 = _mm_load_ps(sptr);
                __m128 s1 = _mm_load_ps(sptr + 4);
                __m128 s2 = _mm_load_ps(sptr + 8);
                __m128 s3 = _mm_load_ps(sptr + 12);
                __m128 x0, x1, x2, x3;

                for( k = 2; k < _ksize; k++ )
                {
                    sptr = src[k] + i;
                    x0 = _mm_load_ps(sptr);
                    x1 = _mm_load_ps(sptr + 4);
                    s0 = updateOp(s0, x0);
                    s1 = updateOp(s1, x1);
                    x2 = _mm_load_ps(sptr + 8);
                    x3 = _mm_load_ps(sptr + 12);
                    s2 = updateOp(s2, x2);
                    s3 = updateOp(s3, x3);
                }

                sptr = src[0] + i;
                x0 = _mm_load_ps(sptr);
                x1 = _mm_load_ps(sptr + 4);
                x2 = _mm_load_ps(sptr + 8);
                x3 = _mm_load_ps(sptr + 12);
                _mm_storeu_ps(dst + i, updateOp(s0, x0));
                _mm_storeu_ps(dst + i + 4, updateOp(s1, x1));
                _mm_storeu_ps(dst + i + 8, updateOp(s2, x2));
                _mm_storeu_ps(dst + i + 12, updateOp(s3, x3));

                sptr = src[k] + i;
                x0 = _mm_load_ps(sptr);
                x1 = _mm_load_ps(sptr + 4);
                x2 = _mm_load_ps(sptr + 8);
                x3 = _mm_load_ps(sptr + 12);
                _mm_storeu_ps(dst + dststep + i, updateOp(s0, x0));
                _mm_storeu_ps(dst + dststep + i + 4, updateOp(s1, x1));
                _mm_storeu_ps(dst + dststep + i + 8, updateOp(s2, x2));
                _mm_storeu_ps(dst + dststep + i + 12, updateOp(s3, x3));
            }

            for( ; i <= width - 4; i += 4 )
            {
                __m128 s0 = _mm_load_ps(src[1] + i), x0;

                for( k = 2; k < _ksize; k++ )
                {
                    x0 = _mm_load_ps(src[k] + i);
                    s0 = updateOp(s0, x0);
                }

                x0 = _mm_load_ps(src[0] + i);
                _mm_storeu_ps(dst + i, updateOp(s0, x0));
                x0 = _mm_load_ps(src[k] + i);
                _mm_storeu_ps(dst + dststep + i, updateOp(s0, x0));
            }
        }

        for( ; count > 0; count--, dst += dststep, src++ )
        {
            for( i = 0; i <= width - 16; i += 16 )
            {
                const float* sptr = src[0] + i;
                __m128 s0 = _mm_load_ps(sptr);
                __m128 s1 = _mm_load_ps(sptr + 4);
                __m128 s2 = _mm_load_ps(sptr + 8);
                __m128 s3 = _mm_load_ps(sptr + 12);
                __m128 x0, x1, x2, x3;

                for( k = 1; k < _ksize; k++ )
                {
                    sptr = src[k] + i;
                    x0 = _mm_load_ps(sptr);
                    x1 = _mm_load_ps(sptr + 4);
                    s0 = updateOp(s0, x0);
                    s1 = updateOp(s1, x1);
                    x2 = _mm_load_ps(sptr + 8);
                    x3 = _mm_load_ps(sptr + 12);
                    s2 = updateOp(s2, x2);
                    s3 = updateOp(s3, x3);
                }
                _mm_storeu_ps(dst + i, s0);
                _mm_storeu_ps(dst + i + 4, s1);
                _mm_storeu_ps(dst + i + 8, s2);
                _mm_storeu_ps(dst + i + 12, s3);
            }

            for( i = 0; i <= width - 4; i += 4 )
            {
                __m128 s0 = _mm_load_ps(src[0] + i), x0;
                for( k = 1; k < _ksize; k++ )
                {
                    x0 = _mm_load_ps(src[k] + i);
                    s0 = updateOp(s0, x0);
                }
                _mm_storeu_ps(dst + i, s0);
            }
        }

        return i;
    }

    int ksize, anchor;
};


template<class VecUpdate> struct MorphIVec
{
    enum { ESZ = VecUpdate::ESZ };

    int operator()(uchar** src, int nz, uchar* dst, int width) const
    {
        if( !checkHardwareSupport(CC_CPU_SSE2) )
            return 0;

        int i, k;
        width *= ESZ;
        VecUpdate updateOp;

        for( i = 0; i <= width - 32; i += 32 )
        {
            const uchar* sptr = src[0] + i;
            __m128i s0 = _mm_loadu_si128((const __m128i*)sptr);
            __m128i s1 = _mm_loadu_si128((const __m128i*)(sptr + 16));
            __m128i x0, x1;

            for( k = 1; k < nz; k++ )
            {
                sptr = src[k] + i;
                x0 = _mm_loadu_si128((const __m128i*)sptr);
                x1 = _mm_loadu_si128((const __m128i*)(sptr + 16));
                s0 = updateOp(s0, x0);
                s1 = updateOp(s1, x1);
            }
            _mm_storeu_si128((__m128i*)(dst + i), s0);
            _mm_storeu_si128((__m128i*)(dst + i + 16), s1);
        }

        for( ; i <= width - 8; i += 8 )
        {
            __m128i s0 = _mm_loadl_epi64((const __m128i*)(src[0] + i)), x0;

            for( k = 1; k < nz; k++ )
            {
                x0 = _mm_loadl_epi64((const __m128i*)(src[k] + i));
                s0 = updateOp(s0, x0);
            }
            _mm_storel_epi64((__m128i*)(dst + i), s0);
        }

        return i/ESZ;
    }
};


template<class VecUpdate> struct MorphFVec
{
    int operator()(uchar** _src, int nz, uchar* _dst, int width) const
    {
        if( !checkHardwareSupport(CC_CPU_SSE) )
            return 0;

        const float** src = (const float**)_src;
        float* dst = (float*)_dst;
        int i, k;
        VecUpdate updateOp;

        for( i = 0; i <= width - 16; i += 16 )
        {
            const float* sptr = src[0] + i;
            __m128 s0 = _mm_loadu_ps(sptr);
            __m128 s1 = _mm_loadu_ps(sptr + 4);
            __m128 s2 = _mm_loadu_ps(sptr + 8);
            __m128 s3 = _mm_loadu_ps(sptr + 12);
            __m128 x0, x1, x2, x3;

            for( k = 1; k < nz; k++ )
            {
                sptr = src[k] + i;
                x0 = _mm_loadu_ps(sptr);
                x1 = _mm_loadu_ps(sptr + 4);
                x2 = _mm_loadu_ps(sptr + 8);
                x3 = _mm_loadu_ps(sptr + 12);
                s0 = updateOp(s0, x0);
                s1 = updateOp(s1, x1);
                s2 = updateOp(s2, x2);
                s3 = updateOp(s3, x3);
            }
            _mm_storeu_ps(dst + i, s0);
            _mm_storeu_ps(dst + i + 4, s1);
            _mm_storeu_ps(dst + i + 8, s2);
            _mm_storeu_ps(dst + i + 12, s3);
        }

        for( ; i <= width - 4; i += 4 )
        {
            __m128 s0 = _mm_loadu_ps(src[0] + i), x0;

            for( k = 1; k < nz; k++ )
            {
                x0 = _mm_loadu_ps(src[k] + i);
                s0 = updateOp(s0, x0);
            }
            _mm_storeu_ps(dst + i, s0);
        }

        for( ; i < width; i++ )
        {
            __m128 s0 = _mm_load_ss(src[0] + i), x0;

            for( k = 1; k < nz; k++ )
            {
                x0 = _mm_load_ss(src[k] + i);
                s0 = updateOp(s0, x0);
            }
            _mm_store_ss(dst + i, s0);
        }

        return i;
    }
};

struct VMin8u
{
    enum { ESZ = 1 };
    __m128i operator()(const __m128i& a, const __m128i& b) const { return _mm_min_epu8(a,b); }
};
struct VMax8u
{
    enum { ESZ = 1 };
    __m128i operator()(const __m128i& a, const __m128i& b) const { return _mm_max_epu8(a,b); }
};
struct VMin16u
{
    enum { ESZ = 2 };
    __m128i operator()(const __m128i& a, const __m128i& b) const
    { return _mm_subs_epu16(a,_mm_subs_epu16(a,b)); }
};
struct VMax16u
{
    enum { ESZ = 2 };
    __m128i operator()(const __m128i& a, const __m128i& b) const
    { return _mm_adds_epu16(_mm_subs_epu16(a,b), b); }
};
struct VMin16s
{
    enum { ESZ = 2 };
    __m128i operator()(const __m128i& a, const __m128i& b) const
    { return _mm_min_epi16(a, b); }
};
struct VMax16s
{
    enum { ESZ = 2 };
    __m128i operator()(const __m128i& a, const __m128i& b) const
    { return _mm_max_epi16(a, b); }
};
struct VMin32f { __m128 operator()(const __m128& a, const __m128& b) const { return _mm_min_ps(a,b); }};
struct VMax32f { __m128 operator()(const __m128& a, const __m128& b) const { return _mm_max_ps(a,b); }};

typedef MorphRowIVec<VMin8u> ErodeRowVec8u;
typedef MorphRowIVec<VMax8u> DilateRowVec8u;
typedef MorphRowIVec<VMin16u> ErodeRowVec16u;
typedef MorphRowIVec<VMax16u> DilateRowVec16u;
typedef MorphRowIVec<VMin16s> ErodeRowVec16s;
typedef MorphRowIVec<VMax16s> DilateRowVec16s;
typedef MorphRowFVec<VMin32f> ErodeRowVec32f;
typedef MorphRowFVec<VMax32f> DilateRowVec32f;

typedef MorphColumnIVec<VMin8u> ErodeColumnVec8u;
typedef MorphColumnIVec<VMax8u> DilateColumnVec8u;
typedef MorphColumnIVec<VMin16u> ErodeColumnVec16u;
typedef MorphColumnIVec<VMax16u> DilateColumnVec16u;
typedef MorphColumnIVec<VMin16s> ErodeColumnVec16s;
typedef MorphColumnIVec<VMax16s> DilateColumnVec16s;
typedef MorphColumnFVec<VMin32f> ErodeColumnVec32f;
typedef MorphColumnFVec<VMax32f> DilateColumnVec32f;

typedef MorphIVec<VMin8u> ErodeVec8u;
typedef MorphIVec<VMax8u> DilateVec8u;
typedef MorphIVec<VMin16u> ErodeVec16u;
typedef MorphIVec<VMax16u> DilateVec16u;
typedef MorphIVec<VMin16s> ErodeVec16s;
typedef MorphIVec<VMax16s> DilateVec16s;
typedef MorphFVec<VMin32f> ErodeVec32f;
typedef MorphFVec<VMax32f> DilateVec32f;

#else

typedef MorphRowNoVec ErodeRowVec8u;
typedef MorphRowNoVec DilateRowVec8u;

typedef MorphColumnNoVec ErodeColumnVec8u;
typedef MorphColumnNoVec DilateColumnVec8u;

typedef MorphRowNoVec ErodeRowVec16u;
typedef MorphRowNoVec DilateRowVec16u;
typedef MorphRowNoVec ErodeRowVec16s;
typedef MorphRowNoVec DilateRowVec16s;
typedef MorphRowNoVec ErodeRowVec32f;
typedef MorphRowNoVec DilateRowVec32f;

typedef MorphColumnNoVec ErodeColumnVec16u;
typedef MorphColumnNoVec DilateColumnVec16u;
typedef MorphColumnNoVec ErodeColumnVec16s;
typedef MorphColumnNoVec DilateColumnVec16s;
typedef MorphColumnNoVec ErodeColumnVec32f;
typedef MorphColumnNoVec DilateColumnVec32f;

typedef MorphNoVec ErodeVec8u;
typedef MorphNoVec DilateVec8u;
typedef MorphNoVec ErodeVec16u;
typedef MorphNoVec DilateVec16u;
typedef MorphNoVec ErodeVec16s;
typedef MorphNoVec DilateVec16s;
typedef MorphNoVec ErodeVec32f;
typedef MorphNoVec DilateVec32f;

#endif

typedef MorphRowNoVec ErodeRowVec64f;
typedef MorphRowNoVec DilateRowVec64f;
typedef MorphColumnNoVec ErodeColumnVec64f;
typedef MorphColumnNoVec DilateColumnVec64f;
typedef MorphNoVec ErodeVec64f;
typedef MorphNoVec DilateVec64f;


template<class Op, class VecOp> struct MorphRowFilter : public BaseRowFilter
{
    typedef typename Op::rtype T;

    MorphRowFilter( int _ksize, int _anchor ) : vecOp(_ksize, _anchor)
    {
        ksize = _ksize;
        anchor = _anchor;
    }

    void operator()(const uchar* src, uchar* dst, int width, int cn)
    {
        int i, j, k, _ksize = ksize*cn;
        const T* S = (const T*)src;
        Op op;
        T* D = (T*)dst;

        if( _ksize == cn )
        {
            for( i = 0; i < width*cn; i++ )
                D[i] = S[i];
            return;
        }

        int i0 = vecOp(src, dst, width, cn);
        width *= cn;

        for( k = 0; k < cn; k++, S++, D++ )
        {
            for( i = i0; i <= width - cn*2; i += cn*2 )
            {
                const T* s = S + i;
                T m = s[cn];
                for( j = cn*2; j < _ksize; j += cn )
                    m = op(m, s[j]);
                D[i] = op(m, s[0]);
                D[i+cn] = op(m, s[j]);
            }

            for( ; i < width; i += cn )
            {
                const T* s = S + i;
                T m = s[0];
                for( j = cn; j < _ksize; j += cn )
                    m = op(m, s[j]);
                D[i] = m;
            }
        }
    }

    VecOp vecOp;
};


template<class Op, class VecOp> struct MorphColumnFilter : public BaseColumnFilter
{
    typedef typename Op::rtype T;

    MorphColumnFilter( int _ksize, int _anchor ) : vecOp(_ksize, _anchor)
    {
        ksize = _ksize;
        anchor = _anchor;
    }

    void operator()(const uchar** _src, uchar* dst, int dststep, int count, int width)
    {
        int i, k, _ksize = ksize;
        const T** src = (const T**)_src;
        T* D = (T*)dst;
        Op op;

        int i0 = vecOp(_src, dst, dststep, count, width);
        dststep /= sizeof(D[0]);

        for( ; _ksize > 1 && count > 1; count -= 2, D += dststep*2, src += 2 )
        {
            i = i0;
            #if CC_ENABLE_UNROLLED
            for( ; i <= width - 4; i += 4 )
            {
                const T* sptr = src[1] + i;
                T s0 = sptr[0], s1 = sptr[1], s2 = sptr[2], s3 = sptr[3];

                for( k = 2; k < _ksize; k++ )
                {
                    sptr = src[k] + i;
                    s0 = op(s0, sptr[0]); s1 = op(s1, sptr[1]);
                    s2 = op(s2, sptr[2]); s3 = op(s3, sptr[3]);
                }

                sptr = src[0] + i;
                D[i] = op(s0, sptr[0]);
                D[i+1] = op(s1, sptr[1]);
                D[i+2] = op(s2, sptr[2]);
                D[i+3] = op(s3, sptr[3]);

                sptr = src[k] + i;
                D[i+dststep] = op(s0, sptr[0]);
                D[i+dststep+1] = op(s1, sptr[1]);
                D[i+dststep+2] = op(s2, sptr[2]);
                D[i+dststep+3] = op(s3, sptr[3]);
            }
            #endif
            for( ; i < width; i++ )
            {
                T s0 = src[1][i];

                for( k = 2; k < _ksize; k++ )
                    s0 = op(s0, src[k][i]);

                D[i] = op(s0, src[0][i]);
                D[i+dststep] = op(s0, src[k][i]);
            }
        }

        for( ; count > 0; count--, D += dststep, src++ )
        {
            i = i0;
            #if CC_ENABLE_UNROLLED
            for( ; i <= width - 4; i += 4 )
            {
                const T* sptr = src[0] + i;
                T s0 = sptr[0], s1 = sptr[1], s2 = sptr[2], s3 = sptr[3];

                for( k = 1; k < _ksize; k++ )
                {
                    sptr = src[k] + i;
                    s0 = op(s0, sptr[0]); s1 = op(s1, sptr[1]);
                    s2 = op(s2, sptr[2]); s3 = op(s3, sptr[3]);
                }

                D[i] = s0; D[i+1] = s1;
                D[i+2] = s2; D[i+3] = s3;
            }
            #endif
            for( ; i < width; i++ )
            {
                T s0 = src[0][i];
                for( k = 1; k < _ksize; k++ )
                    s0 = op(s0, src[k][i]);
                D[i] = s0;
            }
        }
    }

    VecOp vecOp;
};


template<class Op, class VecOp> struct MorphFilter : BaseFilter
{
    typedef typename Op::rtype T;

    MorphFilter( const img_t& _kernel, Point _anchor )
    {
        anchor = _anchor;
        ksize = _kernel.size();
        CC_Assert( _kernel->tid == CC_8U );

        std::vector<uchar> coeffs; // we do not really the values of non-zero
        // kernel elements, just their locations
        preprocess2DKernel( _kernel, coords, coeffs );
        ptrs.resize( coords.size() );
    }

    void operator()(const uchar** src, uchar* dst, int dststep, int count, int width, int cn)
    {
        const Point* pt = &coords[0];
        const T** kp = (const T**)&ptrs[0];
        int i, k, nz = (int)coords.size();
        Op op;

        width *= cn;
        for( ; count > 0; count--, dst += dststep, src++ )
        {
            T* D = (T*)dst;

            for( k = 0; k < nz; k++ )
                kp[k] = (const T*)src[pt[k].y] + pt[k].x*cn;

            i = vecOp(&ptrs[0], nz, dst, width);
            #if CC_ENABLE_UNROLLED
            for( ; i <= width - 4; i += 4 )
            {
                const T* sptr = kp[0] + i;
                T s0 = sptr[0], s1 = sptr[1], s2 = sptr[2], s3 = sptr[3];

                for( k = 1; k < nz; k++ )
                {
                    sptr = kp[k] + i;
                    s0 = op(s0, sptr[0]); s1 = op(s1, sptr[1]);
                    s2 = op(s2, sptr[2]); s3 = op(s3, sptr[3]);
                }

                D[i] = s0; D[i+1] = s1;
                D[i+2] = s2; D[i+3] = s3;
            }
            #endif
            for( ; i < width; i++ )
            {
                T s0 = kp[0][i];
                for( k = 1; k < nz; k++ )
                    s0 = op(s0, kp[k][i]);
                D[i] = s0;
            }
        }
    }

    std::vector<Point> coords;
    std::vector<uchar*> ptrs;
    VecOp vecOp;
};



/////////////////////////////////// External Interface /////////////////////////////////////

Ptr<BaseRowFilter> getMorphologyRowFilter(int op, int type, int ksize, int anchor)
{
    int depth = CC_MAT_DEPTH(type);
    if( anchor < 0 )
        anchor = ksize/2;
    CC_Assert( op == CC_MORPH_ERODE || op == CC_MORPH_DILATE );
    if( op == CC_MORPH_ERODE )
    {
        if( depth == CC_8U )
            return makePtr<MorphRowFilter<MinOp<uchar>,
                                      ErodeRowVec8u> >(ksize, anchor);
        if( depth == CC_16U )
            return makePtr<MorphRowFilter<MinOp<ushort>,
                                      ErodeRowVec16u> >(ksize, anchor);
        if( depth == CC_16S )
            return makePtr<MorphRowFilter<MinOp<short>,
                                      ErodeRowVec16s> >(ksize, anchor);
        if( depth == CC_32F )
            return makePtr<MorphRowFilter<MinOp<float>,
                                      ErodeRowVec32f> >(ksize, anchor);
        if( depth == CC_64F )
            return makePtr<MorphRowFilter<MinOp<double>,
                                      ErodeRowVec64f> >(ksize, anchor);
    }
    else
    {
        if( depth == CC_8U )
            return makePtr<MorphRowFilter<MaxOp<uchar>,
                                      DilateRowVec8u> >(ksize, anchor);
        if( depth == CC_16U )
            return makePtr<MorphRowFilter<MaxOp<ushort>,
                                      DilateRowVec16u> >(ksize, anchor);
        if( depth == CC_16S )
            return makePtr<MorphRowFilter<MaxOp<short>,
                                      DilateRowVec16s> >(ksize, anchor);
        if( depth == CC_32F )
            return makePtr<MorphRowFilter<MaxOp<float>,
                                      DilateRowVec32f> >(ksize, anchor);
        if( depth == CC_64F )
            return makePtr<MorphRowFilter<MaxOp<double>,
                                      DilateRowVec64f> >(ksize, anchor);
    }

    CC_Error_( CC_StsNotImplemented, ("Unsupported data type (=%d)", type));
    return Ptr<BaseRowFilter>();
}

Ptr<BaseColumnFilter> getMorphologyColumnFilter(int op, int type, int ksize, int anchor)
{
    int depth = CC_MAT_DEPTH(type);
    if( anchor < 0 )
        anchor = ksize/2;
    CC_Assert( op == CC_MORPH_ERODE || op == CC_MORPH_DILATE );
    if( op == CC_MORPH_ERODE )
    {
        if( depth == CC_8U )
            return makePtr<MorphColumnFilter<MinOp<uchar>,
                                         ErodeColumnVec8u> >(ksize, anchor);
        if( depth == CC_16U )
            return makePtr<MorphColumnFilter<MinOp<ushort>,
                                         ErodeColumnVec16u> >(ksize, anchor);
        if( depth == CC_16S )
            return makePtr<MorphColumnFilter<MinOp<short>,
                                         ErodeColumnVec16s> >(ksize, anchor);
        if( depth == CC_32F )
            return makePtr<MorphColumnFilter<MinOp<float>,
                                         ErodeColumnVec32f> >(ksize, anchor);
        if( depth == CC_64F )
            return makePtr<MorphColumnFilter<MinOp<double>,
                                         ErodeColumnVec64f> >(ksize, anchor);
    }
    else
    {
        if( depth == CC_8U )
            return makePtr<MorphColumnFilter<MaxOp<uchar>,
                                         DilateColumnVec8u> >(ksize, anchor);
        if( depth == CC_16U )
            return makePtr<MorphColumnFilter<MaxOp<ushort>,
                                         DilateColumnVec16u> >(ksize, anchor);
        if( depth == CC_16S )
            return makePtr<MorphColumnFilter<MaxOp<short>,
                                         DilateColumnVec16s> >(ksize, anchor);
        if( depth == CC_32F )
            return makePtr<MorphColumnFilter<MaxOp<float>,
                                         DilateColumnVec32f> >(ksize, anchor);
        if( depth == CC_64F )
            return makePtr<MorphColumnFilter<MaxOp<double>,
                                         DilateColumnVec64f> >(ksize, anchor);
    }

    CC_Error_( CC_StsNotImplemented, ("Unsupported data type (=%d)", type));
    return Ptr<BaseColumnFilter>();
}


Ptr<BaseFilter> getMorphologyFilter(int op, int type, const img_t* _kernel, Point anchor)
{
    img_t kernel = _kernel;
    int depth = CC_MAT_DEPTH(type);
    anchor = normalizeAnchor(anchor, kernel.size());
    CC_Assert( op == CC_MORPH_ERODE || op == CC_MORPH_DILATE );
    if( op == CC_MORPH_ERODE )
    {
        if( depth == CC_8U )
            return makePtr<MorphFilter<MinOp<uchar>, ErodeVec8u> >(kernel, anchor);
        if( depth == CC_16U )
            return makePtr<MorphFilter<MinOp<ushort>, ErodeVec16u> >(kernel, anchor);
        if( depth == CC_16S )
            return makePtr<MorphFilter<MinOp<short>, ErodeVec16s> >(kernel, anchor);
        if( depth == CC_32F )
            return makePtr<MorphFilter<MinOp<float>, ErodeVec32f> >(kernel, anchor);
        if( depth == CC_64F )
            return makePtr<MorphFilter<MinOp<double>, ErodeVec64f> >(kernel, anchor);
    }
    else
    {
        if( depth == CC_8U )
            return makePtr<MorphFilter<MaxOp<uchar>, DilateVec8u> >(kernel, anchor);
        if( depth == CC_16U )
            return makePtr<MorphFilter<MaxOp<ushort>, DilateVec16u> >(kernel, anchor);
        if( depth == CC_16S )
            return makePtr<MorphFilter<MaxOp<short>, DilateVec16s> >(kernel, anchor);
        if( depth == CC_32F )
            return makePtr<MorphFilter<MaxOp<float>, DilateVec32f> >(kernel, anchor);
        if( depth == CC_64F )
            return makePtr<MorphFilter<MaxOp<double>, DilateVec64f> >(kernel, anchor);
    }

    CC_Error_( CC_StsNotImplemented, ("Unsupported data type (=%d)", type));
    return Ptr<BaseFilter>();
}


Ptr<FilterEngine> createMorphologyFilter( int op, int type, const img_t* _kernel,
                                                      Point anchor, int _rowBorderType, int _columnBorderType,
                                                      const Scalar& _borderValue )
{
    img_t kernel = _kernel;
    anchor = normalizeAnchor(anchor, kernel.size());

    Ptr<BaseRowFilter> rowFilter;
    Ptr<BaseColumnFilter> columnFilter;
    Ptr<BaseFilter> filter2D;

    if( countNonZero(kernel) == kernel->rows*kernel->cols )
    {
        // rectangular structuring element
        rowFilter = getMorphologyRowFilter(op, type, kernel->cols, anchor.x);
        columnFilter = getMorphologyColumnFilter(op, type, kernel->rows, anchor.y);
    }
    else
        filter2D = getMorphologyFilter(op, type, kernel, anchor);

    Scalar borderValue = _borderValue;
    if( (_rowBorderType == CC_BORDER_CONSTANT || _columnBorderType == CC_BORDER_CONSTANT) &&
            borderValue == morphologyDefaultBorderValue() )
    {
        int depth = CC_MAT_DEPTH(type);
        CC_Assert( depth == CC_8U || depth == CC_16U || depth == CC_16S ||
                   depth == CC_32F || depth == CC_64F );
        if( op == CC_MORPH_ERODE )
            borderValue = Scalar::all( depth == CC_8U ? (double)UCHAR_MAX :
                                       depth == CC_16U ? (double)USHRT_MAX :
                                       depth == CC_16S ? (double)SHRT_MAX :
                                       depth == CC_32F ? (double)FLT_MAX : DBL_MAX);
        else
            borderValue = Scalar::all( depth == CC_8U || depth == CC_16U ?
                                           0. :
                                       depth == CC_16S ? (double)SHRT_MIN :
                                       depth == CC_32F ? (double)-FLT_MAX : -DBL_MAX);
    }

    return makePtr<FilterEngine>(filter2D, rowFilter, columnFilter,
                                 type, type, type, _rowBorderType, _columnBorderType, borderValue );
}


img_t getStructuringElement(int shape, CSize ksize, Point anchor)
{
    int i, j;
    int r = 0, c = 0;
    double inv_r2 = 0;

    CC_Assert( shape == CC_MORPH_RECT || shape == CC_MORPH_CROSS || shape == CC_MORPH_ELLIPSE );

    anchor = normalizeAnchor(anchor, ksize);

    if( ksize == CSize(1,1) )
        shape = CC_MORPH_RECT;

    if( shape == CC_MORPH_ELLIPSE )
    {
        r = ksize.height/2;
        c = ksize.width/2;
        inv_r2 = r ? 1./((double)r*r) : 0;
    }

    img_t elem(ksize, CC_8U);

    for( i = 0; i < ksize.height; i++ )
    {
        uchar* ptr = elem.ptr(i);
        int j1 = 0, j2 = 0;

        if( shape == CC_MORPH_RECT || (shape == CC_MORPH_CROSS && i == anchor.y) )
            j2 = ksize.width;
        else if( shape == CC_MORPH_CROSS )
            j1 = anchor.x, j2 = j1 + 1;
        else
        {
            int dy = i - r;
            if( std::abs(dy) <= r )
            {
                int dx = saturate_cast<int>(c*sqrt((r*r - dy*dy)*inv_r2));
                j1 = MAX( c - dx, 0 );
                j2 = MIN( c + dx + 1, ksize.width );
            }
        }

        for( j = 0; j < j1; j++ )
            ptr[j] = 0;
        for( ; j < j2; j++ )
            ptr[j] = 1;
        for( ; j < ksize.width; j++ )
            ptr[j] = 0;
    }

    return elem;
}

// ===== 1. replacement implementation

struct ReplacementMorphImpl : public hal_Morph
{
    cvhalFilter2D * ctx;
    bool isInitialized;
    bool init(int op, int src_type, int dst_type, int max_width, int max_height,
              int kernel_type, uchar * kernel_data, size_t kernel_step, int kernel_width, int kernel_height,
              int anchor_x, int anchor_y,
              int borderType, const double borderValue[4],
    int iterations, bool isSubmatrix, bool allowInplace)
    {
        int res = cv_hal_morphInit(&ctx, op, src_type, dst_type, max_width, max_height,
                                   kernel_type, kernel_data, kernel_step, kernel_width, kernel_height,
                                   anchor_x, anchor_y,
                                   borderType, borderValue,
                                   iterations, isSubmatrix, allowInplace);
        isInitialized = (res == CC_HAL_ERROR_OK);
        return isInitialized;
    }
    void apply(uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height,
               int roi_width, int roi_height, int roi_x, int roi_y,
               int roi_width2, int roi_height2, int roi_x2, int roi_y2)
    {
        if (isInitialized)
        {
            int res = cv_hal_morph(ctx, src_data, src_step, dst_data, dst_step, width, height,
                                   roi_width, roi_height,
                                   roi_x, roi_y,
                                   roi_width2, roi_height2,
                                   roi_x2, roi_y2);
            if (res != CC_HAL_ERROR_OK)
                CC_Error(CC_StsNotImplemented, "Failed to run HAL morph implementation");
        }
    }
    ~ReplacementMorphImpl()
    {
        if (isInitialized)
        {
            int res = cv_hal_morphFree(ctx);
            if (res != CC_HAL_ERROR_OK)
                CC_Error(CC_StsNotImplemented, "Failed to run HAL morph implementation");
        }
    }
};

// ===== 2. IPP implementation

#ifdef HAVE_IPP

#if IPP_VERSION_X100 >= 810

template <int cvtype> struct IppMorphTrait {};

#if IPP_VERSION_X100 >= 900

#define INIT_TRAIT(cvtype, ipptype, flavor, channels, zerodef)\
template <>\
struct IppMorphTrait<cvtype>\
{\
    typedef Ipp##ipptype ipp_data_type;\
    enum { cn = channels };\
    IppDataType getDataType() {return ipp##ipptype;}\
\
    IppStatus getMorphSize(IppiSize roiSize, IppiSize maskSize, int* pSpecSize, int* pBufferSize) {return ippiMorphologyBorderGetSize_##flavor(roiSize, maskSize, pSpecSize, pBufferSize);}\
    IppStatus morphInit(IppiSize roiSize, const Ipp8u* pMask, IppiSize maskSize, IppiMorphState* pMorphSpec, Ipp8u* pBuffer) {return ippiMorphologyBorderInit_##flavor(roiSize, pMask, maskSize, pMorphSpec, pBuffer);}\
    IppStatus filterGetMinSize(IppiSize dstRoiSize, IppiSize maskSize, IppDataType dataType, int numChannels, int* pBufferSize) {return ippiFilterMinBorderGetBufferSize(dstRoiSize, maskSize, dataType, numChannels, pBufferSize);}\
    IppStatus filterGetMaxSize(IppiSize dstRoiSize, IppiSize maskSize, IppDataType dataType, int numChannels, int* pBufferSize) {return ippiFilterMaxBorderGetBufferSize(dstRoiSize, maskSize, dataType, numChannels, pBufferSize);}\
    IppStatus filterMinBorder(const ipp_data_type* pSrc, int srcStep, ipp_data_type* pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint, Ipp8u* pBuffer) { ipp_data_type zerodef; return CC_INSTRUMENT_FUN_IPP(ippiFilterMinBorder_##flavor, pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, ippBorderRepl, zero, pBuffer); }\
    IppStatus filterMaxBorder(const ipp_data_type* pSrc, int srcStep, ipp_data_type* pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint, Ipp8u* pBuffer) { ipp_data_type zerodef; return CC_INSTRUMENT_FUN_IPP(ippiFilterMaxBorder_##flavor, pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, ippBorderRepl, zero, pBuffer); }\
    IppStatus morphDilate(const ipp_data_type* pSrc, int srcStep, ipp_data_type* pDst, int dstStep, IppiSize roiSize, const IppiMorphState* pMorphSpec, Ipp8u* pBuffer) { ipp_data_type zerodef; return CC_INSTRUMENT_FUN_IPP(ippiDilateBorder_##flavor, pSrc, srcStep, pDst, dstStep, roiSize, ippBorderRepl, zero, pMorphSpec, pBuffer); }\
    IppStatus morphErode(const ipp_data_type* pSrc, int srcStep, ipp_data_type* pDst, int dstStep, IppiSize roiSize,  const IppiMorphState* pMorphSpec, Ipp8u* pBuffer) { ipp_data_type zerodef; return CC_INSTRUMENT_FUN_IPP(ippiErodeBorder_##flavor, pSrc, srcStep, pDst, dstStep, roiSize, ippBorderRepl, zero, pMorphSpec, pBuffer); }\
};

#else

#define INIT_TRAIT(cvtype, ipptype, flavor, channels, zerodef)\
template <>\
struct IppMorphTrait<cvtype>\
{\
    typedef Ipp##ipptype ipp_data_type;\
    enum { cn = channels };\
    IppDataType getDataType() {return ipp##ipptype;}\
\
    IppStatus getMorphSize(IppiSize roiSize, IppiSize maskSize, int* pSpecSize, int* pBufferSize) {return ippiMorphologyBorderGetSize_##flavor(roiSize.width, maskSize, pSpecSize, pBufferSize);}\
    IppStatus morphInit(IppiSize roiSize, const Ipp8u* pMask, IppiSize maskSize, IppiMorphState* pMorphSpec, Ipp8u* pBuffer) {return ippiMorphologyBorderInit_##flavor(roiSize.width, pMask, maskSize, pMorphSpec, pBuffer);}\
    IppStatus filterGetMinSize(IppiSize dstRoiSize, IppiSize maskSize, IppDataType, int, int* pBufferSize) {return ippiFilterMinGetBufferSize_##flavor(dstRoiSize.width, maskSize, pBufferSize);}\
    IppStatus filterGetMaxSize(IppiSize dstRoiSize, IppiSize maskSize, IppDataType, int, int* pBufferSize) {return ippiFilterMinGetBufferSize_##flavor(dstRoiSize.width, maskSize, pBufferSize);}\
    IppStatus filterMinBorder(const ipp_data_type* pSrc, int srcStep, ipp_data_type* pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor, Ipp8u* pBuffer) { return ippiFilterMinBorderReplicate_##flavor(pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, pBuffer); }\
    IppStatus filterMaxBorder(const ipp_data_type* pSrc, int srcStep, ipp_data_type* pDst, int dstStep, IppiSize dstRoiSize, IppiSize maskSize, IppiPoint anchor, Ipp8u* pBuffer) { return ippiFilterMaxBorderReplicate_##flavor(pSrc, srcStep, pDst, dstStep, dstRoiSize, maskSize, anchor, pBuffer); }\
    IppStatus morphDilate(const ipp_data_type* pSrc, int srcStep, ipp_data_type* pDst, int dstStep, IppiSize roiSize, const IppiMorphState* pMorphSpec, Ipp8u* pBuffer) {return ippiDilateBorder_##flavor(pSrc, srcStep, pDst, dstStep, roiSize, ippBorderRepl, 0, pMorphSpec, pBuffer);}\
    IppStatus morphErode(const ipp_data_type* pSrc, int srcStep, ipp_data_type* pDst, int dstStep, IppiSize roiSize, const IppiMorphState* pMorphSpec, Ipp8u* pBuffer) {return ippiErodeBorder_##flavor(pSrc, srcStep, pDst, dstStep, roiSize, ippBorderRepl, 0, pMorphSpec, pBuffer);}\
};

#endif

INIT_TRAIT(CC_8UC1, 8u, 8u_C1R, 1, zero = 0)
INIT_TRAIT(CC_8UC3, 8u, 8u_C3R, 3, zero[3] = {0})
INIT_TRAIT(CC_8UC4, 8u, 8u_C4R, 4, zero[4] = {0})
INIT_TRAIT(CC_32FC1, 32f, 32f_C1R, 1, zero = 0)
INIT_TRAIT(CC_32FC3, 32f, 32f_C3R, 3, zero[3] = {0})
INIT_TRAIT(CC_32FC4, 32f, 32f_C4R, 4, zero[4] = {0})

#undef INIT_TRAIT

//--------------------------------------

struct IppMorphBaseImpl : public hal_Morph
{
    virtual bool init(int _op, int _src_type, int dst_type, int max_width, int max_height,
              int kernel_type, uchar * kernel_data, size_t kernel_step, int kernel_width, int kernel_height,
              int anchor_x, int anchor_y,
              int borderType, const double borderValue[4],
              int iterations, bool isSubmatrix, bool allowInplace) = 0;
};

template <int cvtype>
struct IppMorphImpl : public IppMorphBaseImpl
{
    IppMorphTrait<cvtype> trait;
    typedef typename IppMorphTrait<cvtype>::ipp_data_type ipp_data_type;
    IppAutoBuffer<IppiMorphState> specBuf;
    IppAutoBuffer<Ipp8u> workBuf;
    IppiSize kernelSize;
    bool rectKernel;
    IppiPoint anchor;
    int op;
    int src_type;
    int border;

    bool init(int _op, int _src_type, int dst_type, int max_width, int max_height,
              int kernel_type, uchar * kernel_data, size_t kernel_step, int kernel_width, int kernel_height,
              int anchor_x, int anchor_y,
              int borderType, const double borderValue[4],
              int iterations, bool isSubmatrix, bool allowInplace)
    {
        border = borderType; // TODO: remove
        anchor = ippiPoint(anchor_x, anchor_y);
        CC_UNUSED(dst_type);
        src_type = _src_type;

        img_t kernel(CSize(kernel_width, kernel_height), kernel_type, kernel_data, kernel_step);
        int depth = CC_MAT_DEPTH(src_type), cn = CC_MAT_CN(src_type);

        if( !( depth == CC_8U || depth == CC_32F )
            || !(cn == 1 || cn == 3 || cn == 4)
            || !( borderType == CC_BORDER_REPLICATE
                  || (borderType == CC_BORDER_CONSTANT && Vec<double, 4>(borderValue) == morphologyDefaultBorderValue() && kernel.size() == CSize(3,3)))
            || !( op == CC_MORPH_DILATE || op == CC_MORPH_ERODE)
            || isSubmatrix
            || allowInplace)
            return false;

        // In case CC_BORDER_CONSTANT, IPPMorphReplicate works correct with kernels of size 3*3 only
        if( borderType == CC_BORDER_CONSTANT && kernel.data )
        {
            int x, y;
            for( y = 0; y < kernel->rows; y++ )
            {
                if( kernel.at<uchar>(y, anchor.x) != 0 )
                    continue;
                for( x = 0; x < kernel->cols; x++ )
                {
                    if( kernel.at<uchar>(y,x) != 0 )
                        return false;
                }
            }
            for( x = 0; x < kernel->cols; x++ )
            {
                if( kernel.at<uchar>(anchor.y, x) != 0 )
                    continue;
                for( y = 0; y < kernel->rows; y++ )
                {
                    if( kernel.at<uchar>(y,x) != 0 )
                        return false;
                }
            }

        }

        CSize ksize = !kernel.empty() ? kernel.size() : CSize(3,3);

        rectKernel = false;
        if( kernel.empty() )
        {
            ksize = CSize(1+iterations*2,1+iterations*2);
            anchor = ippiPoint(iterations, iterations);
            rectKernel = true;
            iterations = 1;
        }
        else if( iterations >= 1 && countNonZero(kernel) == kernel->rows*kernel->cols )
        {
            ksize = CSize(ksize.width + (iterations-1)*(ksize.width-1),
                 ksize.height + (iterations-1)*(ksize.height-1)),
            anchor = ippiPoint(anchor.x*iterations, anchor.y*iterations);
            kernel = img_t();
            rectKernel = true;
            iterations = 1;
        }

        // TODO: implement the case of iterations > 1.
        if( iterations > 1 )
            return false;

        IppiSize roiSize = {max_width, max_height};
        kernelSize = ippiSize(ksize);
        op = _op;

        IppStatus res;
        if (!rectKernel)
        {
            if (((kernel->cols - 1) / 2 != anchor.x) || ((kernel->rows - 1) / 2 != anchor.y))
                return false;
            int specSize = 0, bufferSize = 0;
            res = trait.getMorphSize(roiSize, kernelSize, &specSize, &bufferSize);
            if (res >= 0)
            {
                specBuf.Alloc(specSize);
                workBuf.Alloc(bufferSize);
                res = trait.morphInit(roiSize, kernel->tt.data, kernelSize, specBuf, workBuf);
                if (res >= 0)
                    return true;
            }
        }
        else
        {
            if (((kernelSize.width - 1) / 2 != anchor.x) || ((kernelSize.height - 1) / 2 != anchor.y))
                return false;
            if (op == CC_MORPH_ERODE)
            {
                int bufSize = 0;
                res = trait.filterGetMinSize(roiSize, kernelSize, trait.getDataType(), trait.cn, &bufSize);
                if (res >= 0)
                {
                    workBuf.Alloc(bufSize);
                    return true;
                }
            }
            else
            {
                int bufSize = 0;
                res = trait.filterGetMaxSize(roiSize, kernelSize, trait.getDataType(), trait.cn, &bufSize);
                if (res >= 0)
                {
                    workBuf.Alloc(bufSize);
                    return true;
                }
            }
        }
        return false;
    }

    void apply(uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height,
               int roi_width, int roi_height, int roi_x, int roi_y,
               int roi_width2, int roi_height2, int roi_x2, int roi_y2)
    {
        CC_INSTRUMENT_REGION_IPP()

        CC_UNUSED(roi_width); CC_UNUSED(roi_height); CC_UNUSED(roi_x); CC_UNUSED(roi_y);
        CC_UNUSED(roi_width2); CC_UNUSED(roi_height2); CC_UNUSED(roi_x2); CC_UNUSED(roi_y2);
        if (src_data == dst_data)
            CC_Error(CC_StsBadArg, "IPP Morph inplace is not alowed");

        IppiSize roiSize = {width, height};

        IppStatus res;
        if (!rectKernel)
        {
            if (op == CC_MORPH_ERODE)
                res = (trait.morphErode((ipp_data_type*)src_data, (int)src_step, (ipp_data_type*)dst_data, (int)dst_step, roiSize, specBuf, workBuf));
            else
                res = (trait.morphDilate((ipp_data_type*)src_data, (int)src_step, (ipp_data_type*)dst_data, (int)dst_step, roiSize, specBuf, workBuf));
        }
        else
        {
            if (op == CC_MORPH_ERODE)
                res = (trait.filterMinBorder((ipp_data_type*)src_data, (int)src_step, (ipp_data_type*)dst_data, (int)dst_step, roiSize, kernelSize, anchor, workBuf));
            else
                res = (trait.filterMaxBorder((ipp_data_type*)src_data, (int)src_step, (ipp_data_type*)dst_data, (int)dst_step, roiSize, kernelSize, anchor, workBuf));
        }
        if (res < 0)
            CC_Error(CC_StsBadArg, "Failed to run IPP morph");
    }
};

static IppMorphBaseImpl * createIppImpl(int type)
{
    switch (type)
    {
    case CC_8UC1: return new IppMorphImpl<CC_8UC1>();
    case CC_8UC3: return new IppMorphImpl<CC_8UC3>();
    case CC_8UC4: return new IppMorphImpl<CC_8UC4>();
    case CC_32FC1: return new IppMorphImpl<CC_32FC1>();
    case CC_32FC3: return new IppMorphImpl<CC_32FC3>();
    case CC_32FC4: return new IppMorphImpl<CC_32FC4>();
    }
    return 0;
}

#endif // IPP_VERSION_X100 >= 810
#endif // HAVE_IPP

// ===== 3. Fallback implementation

struct OcvMorphImpl : public hal_Morph
{
    Ptr<FilterEngine> f;
    int iterations;
    int src_type;
    int dst_type;
    bool init(int op, int _src_type, int _dst_type, int, int,
              int kernel_type, uchar * kernel_data, size_t kernel_step, int kernel_width, int kernel_height,
              int anchor_x, int anchor_y,
              int borderType, const double _borderValue[4],
              int _iterations, bool, bool)
    {
        iterations = _iterations;
        src_type = _src_type;
        dst_type = _dst_type;
        img_t kernel(CSize(kernel_width, kernel_height), kernel_type, kernel_data, kernel_step);
        Point anchor(anchor_x, anchor_y);
        Vec<double, 4> borderValue(_borderValue);
        f = createMorphologyFilter(op, src_type, kernel, anchor, borderType, borderType, borderValue );
        return true;
    }

    void apply(uchar * src_data, size_t src_step, uchar * dst_data, size_t dst_step, int width, int height,
               int roi_width, int roi_height, int roi_x, int roi_y,
               int roi_width2, int roi_height2, int roi_x2, int roi_y2)
    {
        img_t src(CSize(width, height), src_type, src_data, src_step);
        img_t dst(CSize(width, height), dst_type, dst_data, dst_step);
        {
            Point ofs(roi_x, roi_y);
            CSize wsz(roi_width, roi_height);
            f->apply( src, dst, wsz, ofs );
        }
        {
            Point ofs(roi_x2, roi_y2);
            CSize wsz(roi_width2, roi_height2);
            for( int i = 1; i < iterations; i++ )
                f->apply( dst, dst, wsz, ofs );
        }
    }
};

// ===== HAL interface implementation

Ptr<Morph> Morph ::create(int op, int src_type, int dst_type, int max_width, int max_height,
                                        int kernel_type, uchar * kernel_data, size_t kernel_step, int kernel_width, int kernel_height,
                                        int anchor_x, int anchor_y,
                                        int borderType, const double borderValue[4],
                                        int iterations, bool isSubmatrix, bool allowInplace)
{
    {
        ReplacementMorphImpl * impl = new ReplacementMorphImpl();
        if (impl->init(op, src_type, dst_type, max_width, max_height,
                       kernel_type, kernel_data, kernel_step, kernel_width, kernel_height,
                       anchor_x, anchor_y,
                       borderType, borderValue, iterations, isSubmatrix, allowInplace))
        {
            return Ptr<Morph>(impl);
        }
        delete impl;
    }
#if defined(HAVE_IPP) && IPP_VERSION_X100 >= 810
    CC_IPP_CHECK()
    {
        IppMorphBaseImpl * impl = createIppImpl(src_type);
        if (impl)
        {
            if (impl->init(op, src_type, dst_type, max_width, max_height,
                        kernel_type, kernel_data, kernel_step, kernel_width, kernel_height,
                        anchor_x, anchor_y,
                        borderType, borderValue, iterations, isSubmatrix, allowInplace))
            {
                return Ptr<Morph>(impl);
            }
            delete impl;
        }
    }
#endif
    {
        OcvMorphImpl * impl = new OcvMorphImpl();
        impl->init(op, src_type, dst_type, max_width, max_height,
                kernel_type, kernel_data, kernel_step, kernel_width, kernel_height,
                anchor_x, anchor_y,
                borderType, borderValue, iterations, isSubmatrix, allowInplace);
        return Ptr<Morph>(impl);
    }
}

#ifdef HAVE_OPENCL

#define ROUNDUP(sz, n)      ((sz) + (n) - 1 - (((sz) + (n) - 1) % (n)))

static bool ocl_morph3x3_8UC1( const img_t* _src, img_t* dst, const img_t* _kernel, Point anchor,
                               int op, int actual_op = -1, const img_t* _extraMat = NULL)
{
    int type = _src->tid, depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type);
    CSize ksize = _kernel.size();

    img_t kernel8u;
    String processing;

    bool haveExtraMat = !_extraMat.empty();
    CC_Assert(actual_op <= 3 || haveExtraMat);

    _kernel.convertTo(kernel8u, CC_8U);
    for (int y = 0; y < kernel8u->rows; ++y)
        for (int x = 0; x < kernel8u->cols; ++x)
            if (kernel8u.at<uchar>(y, x) != 0)
                processing += format("PROCESS(%d,%d)", y, x);

    if (anchor.x < 0)
        anchor.x = ksize.width / 2;
    if (anchor.y < 0)
        anchor.y = ksize.height / 2;

    if (actual_op < 0)
        actual_op = op;

    if (type != CC_8UC1 ||
        !((_src.offset() == 0) && (_src->step() % 4 == 0)) ||
        !((_src->cols() % 16 == 0) && (_src->rows() % 2 == 0)) ||
        !(anchor.x == 1 && anchor.y == 1) ||
        !(ksize.width == 3 && ksize.height == 3))
        return false;

    CSize size = _cvGetSize(src);
    size_t globalsize[2] = { 0, 0 };
    size_t localsize[2] = { 0, 0 };

    globalsize[0] = size.width / 16;
    globalsize[1] = size.height / 2;

    static const char * const op2str[] = { "OP_ERODE", "OP_DILATE", NULL, NULL, "OP_GRADIENT", "OP_TOPHAT", "OP_BLACKHAT" };
    String opts = format("-D PROCESS_ELEM_=%s -D %s%s", processing.c_str(), op2str[op],
                         actual_op == op ? "" : format(" -D %s", op2str[actual_op]).c_str());

    ocl::Kernel k;
    k cvSetMat("morph3x3_8UC1_cols16_rows2", ocl::imgproc::morph3x3_oclsrc, opts);

    if (k.empty())
        return false;

    UMat src = _src.getUMat();
    _dst cvSetMat(size, CC_MAKETYPE(depth, cn));
    if (!(_dst.offset() == 0 && _dst->step() % 4 == 0))
        return false;
    UMat dst = _dst.getUMat();
    UMat extraMat = _extraMat.getUMat();

    int idxArg = k.set(0, ocl::KernelArg::PtrReadOnly(src));
    idxArg = k.set(idxArg, (int)src->step);
    idxArg = k.set(idxArg, ocl::KernelArg::PtrWriteOnly(dst));
    idxArg = k.set(idxArg, (int)dst->step);
    idxArg = k.set(idxArg, (int)dst->rows);
    idxArg = k.set(idxArg, (int)dst->cols);

    if (haveExtraMat)
    {
        idxArg = k.set(idxArg, ocl::KernelArg::ReadOnlyNoSize(extraMat));
    }

    return k.run(2, globalsize, (localsize[0] == 0) ? NULL : localsize, false);
}

static bool ocl_morphSmall( const img_t* _src, img_t* dst, const img_t* _kernel, Point anchor, int borderType,
                            int op, int actual_op = -1, const img_t* _extraMat = NULL)
{
    const ocl::Device & dev = ocl::Device::getDefault();
    int type = _src->tid, depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type), esz = CC_ELEM_SIZE(type);
    bool doubleSupport = dev.doubleFPConfig() > 0;

    if (cn > 4 || (!doubleSupport && depth == CC_64F) ||
        _src.offset() % esz != 0 || _src->step() % esz != 0)
        return false;

    bool haveExtraMat = !_extraMat.empty();
    CC_Assert(actual_op <= 3 || haveExtraMat);

    CSize ksize = _kernel.size();
    if (anchor.x < 0)
        anchor.x = ksize.width / 2;
    if (anchor.y < 0)
        anchor.y = ksize.height / 2;

    CSize size = _cvGetSize(src), wholeSize;
    bool isolated = (borderType & CC_BORDER_ISOLATED) != 0;
    borderType &= ~CC_BORDER_ISOLATED;
    int wdepth = depth, wtype = type;
    if (depth == CC_8U)
    {
        wdepth = CC_32S;
        wtype = CC_MAKETYPE(wdepth, cn);
    }
    char cvt[2][40];

    const char * const borderMap[] = { "CC_BORDER_CONSTANT", "CC_BORDER_REPLICATE",
                                       "CC_BORDER_REFLECT", 0, "CC_BORDER_REFLECT_101" };
    size_t globalsize[2] = { (size_t)size.width, (size_t)size.height };

    UMat src = _src.getUMat();
    if (!isolated)
    {
        Point ofs;
        src.locateROI(wholeSize, ofs);
    }

    int h = isolated ? size.height : wholeSize.height;
    int w = isolated ? size.width : wholeSize.width;
    if (w < ksize.width || h < ksize.height)
        return false;

    // Figure out what vector size to use for loading the pixels.
    int pxLoadNumPixels = cn != 1 || size.width % 4 ? 1 : 4;
    int pxLoadVecSize = cn * pxLoadNumPixels;

    // Figure out how many pixels per work item to compute in X and Y
    // directions.  Too many and we run out of registers.
    int pxPerWorkItemX = 1, pxPerWorkItemY = 1;
    if (cn <= 2 && ksize.width <= 4 && ksize.height <= 4)
    {
        pxPerWorkItemX = size.width % 8 ? size.width % 4 ? size.width % 2 ? 1 : 2 : 4 : 8;
        pxPerWorkItemY = size.height % 2 ? 1 : 2;
    }
    else if (cn < 4 || (ksize.width <= 4 && ksize.height <= 4))
    {
        pxPerWorkItemX = size.width % 2 ? 1 : 2;
        pxPerWorkItemY = size.height % 2 ? 1 : 2;
    }
    globalsize[0] = size.width / pxPerWorkItemX;
    globalsize[1] = size.height / pxPerWorkItemY;

    // Need some padding in the private array for pixels
    int privDataWidth = ROUNDUP(pxPerWorkItemX + ksize.width - 1, pxLoadNumPixels);

    // Make the global size a nice round number so the runtime can pick
    // from reasonable choices for the workgroup size
    const int wgRound = 256;
    globalsize[0] = ROUNDUP(globalsize[0], wgRound);

    if (actual_op < 0)
        actual_op = op;

    // build processing
    String processing;
    img_t kernel8u;
    _kernel.convertTo(kernel8u, CC_8U);
    for (int y = 0; y < kernel8u->rows; ++y)
        for (int x = 0; x < kernel8u->cols; ++x)
            if (kernel8u.at<uchar>(y, x) != 0)
                processing += format("PROCESS(%d,%d)", y, x);


    static const char * const op2str[] = { "OP_ERODE", "OP_DILATE", NULL, NULL, "OP_GRADIENT", "OP_TOPHAT", "OP_BLACKHAT" };
    String opts = format("-D cn=%d "
            "-D ANCHOR_X=%d -D ANCHOR_Y=%d -D KERNEL_SIZE_X=%d -D KERNEL_SIZE_Y=%d "
            "-D PX_LOAD_VEC_SIZE=%d -D PX_LOAD_NUM_PX=%d -D DEPTH_%d "
            "-D PX_PER_WI_X=%d -D PX_PER_WI_Y=%d -D PRIV_DATA_WIDTH=%d -D %s -D %s "
            "-D PX_LOAD_X_ITERATIONS=%d -D PX_LOAD_Y_ITERATIONS=%d "
            "-D srcT=%s -D srcT1=%s -D dstT=srcT -D dstT1=srcT1 -D WT=%s -D WT1=%s "
            "-D convertToWT=%s -D convertToDstT=%s -D PX_LOAD_FLOAT_VEC_CONV=convert_%s -D PROCESS_ELEM_=%s -D %s%s",
            cn, anchor.x, anchor.y, ksize.width, ksize.height,
            pxLoadVecSize, pxLoadNumPixels, depth,
            pxPerWorkItemX, pxPerWorkItemY, privDataWidth, borderMap[borderType],
            isolated ? "CC_BORDER_ISOLATED" : "NO_BORDER_ISOLATED",
            privDataWidth / pxLoadNumPixels, pxPerWorkItemY + ksize.height - 1,
            ocl::typeToStr(type), ocl::typeToStr(depth),
            haveExtraMat ? ocl::typeToStr(wtype):"srcT",//to prevent overflow - WT
            haveExtraMat ? ocl::typeToStr(wdepth):"srcT1",//to prevent overflow - WT1
            haveExtraMat ? ocl::convertTypeStr(depth, wdepth, cn, cvt[0]) : "noconvert",//to prevent overflow - src to WT
            haveExtraMat ? ocl::convertTypeStr(wdepth, depth, cn, cvt[1]) : "noconvert",//to prevent overflow - WT to dst
            ocl::typeToStr(CC_MAKE_TYPE(haveExtraMat ? wdepth : depth, pxLoadVecSize)), //PX_LOAD_FLOAT_VEC_CONV
            processing.c_str(), op2str[op],
            actual_op == op ? "" : format(" -D %s", op2str[actual_op]).c_str());

    ocl::Kernel kernel("filterSmall", ocl::imgproc::filterSmall_oclsrc, opts);
    if (kernel.empty())
        return false;

    _dst cvSetMat(size, type);
    UMat dst = _dst.getUMat();

    UMat source;
    if(src.u != dst.u)
        source = src;
    else
    {
        Point ofs;
        int cols =  src->cols, rows = src->rows;
        src.locateROI(wholeSize, ofs);
        src.adjustROI(ofs.y, wholeSize.height - rows - ofs.y, ofs.x, wholeSize.width - cols - ofs.x);
        src.copyTo(source);

        src.adjustROI(-ofs.y, -wholeSize.height + rows + ofs.y, -ofs.x, -wholeSize.width + cols + ofs.x);
        source.adjustROI(-ofs.y, -wholeSize.height + rows + ofs.y, -ofs.x, -wholeSize.width + cols + ofs.x);
        source.locateROI(wholeSize, ofs);
    }

    UMat extraMat = _extraMat.getUMat();

    int idxArg = kernel.set(0, ocl::KernelArg::PtrReadOnly(source));
    idxArg = kernel.set(idxArg, (int)source->step);
    int srcOffsetX = (int)((source.offset % source->step) / source CC_ELEM_SIZE());
    int srcOffsetY = (int)(source.offset / source->step);
    int srcEndX = isolated ? srcOffsetX + size.width : wholeSize.width;
    int srcEndY = isolated ? srcOffsetY + size.height : wholeSize.height;
    idxArg = kernel.set(idxArg, srcOffsetX);
    idxArg = kernel.set(idxArg, srcOffsetY);
    idxArg = kernel.set(idxArg, srcEndX);
    idxArg = kernel.set(idxArg, srcEndY);
    idxArg = kernel.set(idxArg, ocl::KernelArg::WriteOnly(dst));

    if (haveExtraMat)
    {
        idxArg = kernel.set(idxArg, ocl::KernelArg::ReadOnlyNoSize(extraMat));
    }

    return kernel.run(2, globalsize, NULL, false);

}

static bool ocl_morphOp(const img_t* _src, img_t* dst, const img_t* _kernel,
                        Point anchor, int iterations, int op, int borderType,
                        const Scalar &, int actual_op = -1, const img_t* _extraMat = NULL)
{
    const ocl::Device & dev = ocl::Device::getDefault();
    int type = _src->tid, depth = CC_MAT_DEPTH(type),
            cn = CC_MAT_CN(type), esz = CC_ELEM_SIZE(type);
    img_t kernel = _kernel;
    CSize ksize = !kernel.empty() ? kernel.size() : CSize(3, 3), ssize = _cvGetSize(src);

    bool doubleSupport = dev.doubleFPConfig() > 0;
    if ((depth == CC_64F && !doubleSupport) || borderType != CC_BORDER_CONSTANT)
        return false;

    bool haveExtraMat = !_extraMat.empty();
    CC_Assert(actual_op <= 3 || haveExtraMat);

    if (kernel.empty())
    {
        kernel = getStructuringElement(CC_MORPH_RECT, CSize(1+iterations*2,1+iterations*2));
        anchor = Point(iterations, iterations);
        iterations = 1;
    }
    else if( iterations > 1 && countNonZero(kernel) == kernel->rows*kernel->cols )
    {
        anchor = Point(anchor.x*iterations, anchor.y*iterations);
        kernel = getStructuringElement(CC_MORPH_RECT,
                                       CSize(ksize.width + (iterations-1)*(ksize.width-1),
                                            ksize.height + (iterations-1)*(ksize.height-1)),
                                       anchor);
        iterations = 1;
    }

    // try to use OpenCL kernel adopted for small morph kernel
    if (dev.isIntel() && !(dev->tid & ocl::Device::TYPE_CPU) &&
        ((ksize.width < 5 && ksize.height < 5 && esz <= 4) ||
         (ksize.width == 5 && ksize.height == 5 && cn == 1)) &&
         (iterations == 1)
#if defined __APPLE__
         && cn == 1
#endif
         )
    {
        if (ocl_morph3x3_8UC1(_src, _dst, kernel, anchor, op, actual_op, _extraMat))
            return true;

        if (ocl_morphSmall(_src, _dst, kernel, anchor, borderType, op, actual_op, _extraMat))
            return true;
    }

    if (iterations == 0 || kernel->rows*kernel->cols == 1)
    {
        _src.copyTo(_dst);
        return true;
    }

#ifdef ANDROID
    size_t localThreads[2] = { 16, 8 };
#else
    size_t localThreads[2] = { 16, 16 };
#endif
    size_t globalThreads[2] = { (size_t)ssize.width, (size_t)ssize.height };

#ifdef __APPLE__
    if( actual_op != CC_MORPH_ERODE && actual_op != CC_MORPH_DILATE )
        localThreads[0] = localThreads[1] = 4;
#endif

    if (localThreads[0]*localThreads[1] * 2 < (localThreads[0] + ksize.width - 1) * (localThreads[1] + ksize.height - 1))
        return false;

#ifdef ANDROID
    if (dev.isNVidia())
        return false;
#endif

    // build processing
    String processing;
    img_t kernel8u;
    kernel.convertTo(kernel8u, CC_8U);
    for (int y = 0; y < kernel8u->rows; ++y)
        for (int x = 0; x < kernel8u->cols; ++x)
            if (kernel8u.at<uchar>(y, x) != 0)
                processing += format("PROCESS(%d,%d)", y, x);

    static const char * const op2str[] = { "OP_ERODE", "OP_DILATE", NULL, NULL, "OP_GRADIENT", "OP_TOPHAT", "OP_BLACKHAT" };

    char cvt[2][50];
    int wdepth = MAX(depth, CC_32F), scalarcn = cn == 3 ? 4 : cn;

    if (actual_op < 0)
        actual_op = op;

    std::vector<ocl::Kernel> kernels(iterations);
    for (int i = 0; i < iterations; i++)
    {
        int current_op = iterations == i + 1 ? actual_op : op;
        String buildOptions = format("-D RADIUSX=%d -D RADIUSY=%d -D LSIZE0=%d -D LSIZE1=%d -D %s%s"
                                     " -D PROCESS_ELEMS=%s -D T=%s -D DEPTH_%d -D cn=%d -D T1=%s"
                                     " -D convertToWT=%s -D convertToT=%s -D ST=%s%s",
                                     anchor.x, anchor.y, (int)localThreads[0], (int)localThreads[1], op2str[op],
                                     doubleSupport ? " -D DOUBLE_SUPPORT" : "", processing.c_str(),
                                     ocl::typeToStr(type), depth, cn, ocl::typeToStr(depth),
                                     ocl::convertTypeStr(depth, wdepth, cn, cvt[0]),
                                     ocl::convertTypeStr(wdepth, depth, cn, cvt[1]),
                                     ocl::typeToStr(CC_MAKE_TYPE(depth, scalarcn)),
                                     current_op == op ? "" : format(" -D %s", op2str[current_op]).c_str());

        kernels[i] cvSetMat("morph", ocl::imgproc::morph_oclsrc, buildOptions);
        if (kernels[i].empty())
            return false;
    }

    UMat src = _src.getUMat(), extraMat = _extraMat.getUMat();
    _dst cvSetMat(cvGetSize(src), src->tid);
    UMat dst = _dst.getUMat();

    if (iterations == 1 && src.u != dst.u)
    {
        CSize wholesize;
        Point ofs;
        src.locateROI(wholesize, ofs);
        int wholecols = wholesize.width, wholerows = wholesize.height;

        if (haveExtraMat)
            kernels[0].args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnlyNoSize(dst),
                        ofs.x, ofs.y, src->cols, src->rows, wholecols, wholerows,
                        ocl::KernelArg::ReadOnlyNoSize(extraMat));
        else
            kernels[0].args(ocl::KernelArg::ReadOnlyNoSize(src), ocl::KernelArg::WriteOnlyNoSize(dst),
                        ofs.x, ofs.y, src->cols, src->rows, wholecols, wholerows);

        return kernels[0].run(2, globalThreads, localThreads, false);
    }

    for (int i = 0; i < iterations; i++)
    {
        UMat source;
        CSize wholesize;
        Point ofs;

        if (i == 0)
        {
            int cols =  src->cols, rows = src->rows;
            src.locateROI(wholesize, ofs);
            src.adjustROI(ofs.y, wholesize.height - rows - ofs.y, ofs.x, wholesize.width - cols - ofs.x);
            if(src.u != dst.u)
                source = src;
            else
                src.copyTo(source);

            src.adjustROI(-ofs.y, -wholesize.height + rows + ofs.y, -ofs.x, -wholesize.width + cols + ofs.x);
            source.adjustROI(-ofs.y, -wholesize.height + rows + ofs.y, -ofs.x, -wholesize.width + cols + ofs.x);
        }
        else
        {
            int cols =  dst->cols, rows = dst->rows;
            dst.locateROI(wholesize, ofs);
            dst.adjustROI(ofs.y, wholesize.height - rows - ofs.y, ofs.x, wholesize.width - cols - ofs.x);
            dst.copyTo(source);
            dst.adjustROI(-ofs.y, -wholesize.height + rows + ofs.y, -ofs.x, -wholesize.width + cols + ofs.x);
            source.adjustROI(-ofs.y, -wholesize.height + rows + ofs.y, -ofs.x, -wholesize.width + cols + ofs.x);
        }
        source.locateROI(wholesize, ofs);

        if (haveExtraMat && iterations == i + 1)
            kernels[i].args(ocl::KernelArg::ReadOnlyNoSize(source), ocl::KernelArg::WriteOnlyNoSize(dst),
                ofs.x, ofs.y, source->cols, source->rows, wholesize.width, wholesize.height,
                ocl::KernelArg::ReadOnlyNoSize(extraMat));
        else
            kernels[i].args(ocl::KernelArg::ReadOnlyNoSize(source), ocl::KernelArg::WriteOnlyNoSize(dst),
                ofs.x, ofs.y, source->cols, source->rows, wholesize.width, wholesize.height);

        if (!kernels[i].run(2, globalThreads, localThreads, false))
            return false;
    }

    return true;
}

#endif

static void morphOp( int op, const img_t* _src, img_t* dst,
                     const img_t* _kernel,
                     Point anchor, int iterations,
                     int borderType, const Scalar& borderValue )
{
    img_t kernel = _kernel;
    CSize ksize = !kernel.empty() ? kernel.size() : CSize(3,3);
    anchor = normalizeAnchor(anchor, ksize);

    CC_OCL_RUN(_dst.isUMat() && _src.dims() <= 2 && _src CC_MAT_CN() <= 4 &&
               borderType == CC_BORDER_CONSTANT && borderValue == morphologyDefaultBorderValue() &&
               (op == CC_MORPH_ERODE || op == CC_MORPH_DILATE) &&
               anchor.x == ksize.width >> 1 && anchor.y == ksize.height >> 1,
               ocl_morphOp(_src, _dst, kernel, anchor, iterations, op, borderType, borderValue) )

    if (iterations == 0 || kernel->rows*kernel->cols == 1)
    {
        _src.copyTo(_dst);
        return;
    }

    if (kernel.empty())
    {
        kernel = getStructuringElement(CC_MORPH_RECT, CSize(1+iterations*2,1+iterations*2));
        anchor = Point(iterations, iterations);
        iterations = 1;
    }
    else if( iterations > 1 && countNonZero(kernel) == kernel->rows*kernel->cols )
    {
        anchor = Point(anchor.x*iterations, anchor.y*iterations);
        kernel = getStructuringElement(CC_MORPH_RECT,
                                       CSize(ksize.width + (iterations-1)*(ksize.width-1),
                                            ksize.height + (iterations-1)*(ksize.height-1)),
                                       anchor);
        iterations = 1;
    }

    img_t *src = _src;
    _dst cvSetMat( cvGetSize(src), src->tid );
    img_t *dst = _dst;

    Point s_ofs;
    CSize s_wsz(src->cols, src->rows);
    Point d_ofs;
    CSize d_wsz(dst->cols, dst->rows);
    bool isolated = (borderType&CC_BORDER_ISOLATED)?true:false;
    borderType = (borderType&~CC_BORDER_ISOLATED);

    if(!isolated)
    {
        src.locateROI(s_wsz, s_ofs);
        dst.locateROI(d_wsz, d_ofs);
    }

    Ptr<hal_Morph> ctx = hal_Morph::create(op, src->tid, dst->tid, src->cols, src->rows,
                                                           kernel->tid, kernel.data, kernel->step, kernel->cols, kernel->rows,
                                                           anchor.x, anchor.y, borderType, borderValue.val, iterations,
                                                           (src.isSubmatrix() && !isolated), src.data == dst.data);
    ctx->apply(src.data, src->step, dst.data, dst->step, src->cols, src->rows,
               s_wsz.width, s_wsz.height, s_ofs.x, s_ofs.y,
               d_wsz.width, d_wsz.height, d_ofs.x, d_ofs.y);
}



void erode( const img_t* src, img_t* dst, const img_t* kernel,
                Point anchor, int iterations,
                int borderType, const Scalar& borderValue )
{
    CC_INSTRUMENT_REGION()

    morphOp( CC_MORPH_ERODE, src, dst, kernel, anchor, iterations, borderType, borderValue );
}


void dilate( const img_t* src, img_t* dst, const img_t* kernel,
                 Point anchor, int iterations,
                 int borderType, const Scalar& borderValue )
{
    CC_INSTRUMENT_REGION()

    morphOp( CC_MORPH_DILATE, src, dst, kernel, anchor, iterations, borderType, borderValue );
}

#ifdef HAVE_OPENCL

static bool ocl_morphologyEx(const img_t* _src, img_t* dst, int op,
                             const img_t* kernel, Point anchor, int iterations,
                             int borderType, const Scalar& borderValue)
{
    _dst.createSameSize(_src, _src->tid);
    bool submat = _dst.isSubmatrix();
    UMat temp;
    _CvArr* temp = submat ? _dst : _OutputArray(temp);

    switch( op )
    {
    case CC_MORPH_ERODE:
        if (!ocl_morphOp( _src, _dst, kernel, anchor, iterations, CC_MORPH_ERODE, borderType, borderValue ))
            return false;
        break;
    case CC_MORPH_DILATE:
        if (!ocl_morphOp( _src, _dst, kernel, anchor, iterations, CC_MORPH_DILATE, borderType, borderValue ))
            return false;
        break;
    case CC_MORPH_OPEN:
        if (!ocl_morphOp( _src, _temp, kernel, anchor, iterations, CC_MORPH_ERODE, borderType, borderValue ))
            return false;
        if (!ocl_morphOp( _temp, _dst, kernel, anchor, iterations, CC_MORPH_DILATE, borderType, borderValue ))
            return false;
        break;
    case CC_MORPH_CLOSE:
        if (!ocl_morphOp( _src, _temp, kernel, anchor, iterations, CC_MORPH_DILATE, borderType, borderValue ))
            return false;
        if (!ocl_morphOp( _temp, _dst, kernel, anchor, iterations, CC_MORPH_ERODE, borderType, borderValue ))
            return false;
        break;
    case CC_MORPH_GRADIENT:
        if (!ocl_morphOp( _src, temp, kernel, anchor, iterations, CC_MORPH_ERODE, borderType, borderValue ))
            return false;
        if (!ocl_morphOp( _src, _dst, kernel, anchor, iterations, CC_MORPH_DILATE, borderType, borderValue, CC_MORPH_GRADIENT, temp ))
            return false;
        break;
    case CC_MORPH_TOPHAT:
        if (!ocl_morphOp( _src, _temp, kernel, anchor, iterations, CC_MORPH_ERODE, borderType, borderValue ))
            return false;
        if (!ocl_morphOp( _temp, _dst, kernel, anchor, iterations, CC_MORPH_DILATE, borderType, borderValue, CC_MORPH_TOPHAT, _src ))
            return false;
        break;
    case CC_MORPH_BLACKHAT:
        if (!ocl_morphOp( _src, _temp, kernel, anchor, iterations, CC_MORPH_DILATE, borderType, borderValue ))
            return false;
        if (!ocl_morphOp( _temp, _dst, kernel, anchor, iterations, CC_MORPH_ERODE, borderType, borderValue, CC_MORPH_BLACKHAT, _src ))
            return false;
        break;
    default:
        CC_Error( CC_StsBadArg, "unknown morphological operation" );
    }

    return true;
}



#endif

void morphologyEx( const img_t* _src, img_t* dst, int op,
                       const img_t* _kernel, Point anchor, int iterations,
                       int borderType, const Scalar& borderValue )
{
    CC_INSTRUMENT_REGION()

    img_t kernel = _kernel;
    if (kernel.empty())
    {
        kernel = getStructuringElement(CC_MORPH_RECT, CSize(3,3), Point(1,1));
    }
#ifdef HAVE_OPENCL
    CSize ksize = kernel.size();
    anchor = normalizeAnchor(anchor, ksize);

    CC_OCL_RUN(_dst.isUMat() && _src.dims() <= 2 && _src CC_MAT_CN() <= 4 &&
        anchor.x == ksize.width >> 1 && anchor.y == ksize.height >> 1 &&
        borderType == CC_BORDER_CONSTANT && borderValue == morphologyDefaultBorderValue(),
        ocl_morphologyEx(_src, _dst, op, kernel, anchor, iterations, borderType, borderValue))
#endif

    img_t *src = _src, temp;
    _dst cvSetMat(cvGetSize(src), src->tid);
    img_t *dst = _dst;

    img_t k1, k2, e1, e2;		//only for hit and miss op

    switch( op )
    {
    case CC_MORPH_ERODE:
        erode( src, dst, kernel, anchor, iterations, borderType, borderValue );
        break;
    case CC_MORPH_DILATE:
        dilate( src, dst, kernel, anchor, iterations, borderType, borderValue );
        break;
    case CC_MORPH_OPEN:
        erode( src, dst, kernel, anchor, iterations, borderType, borderValue );
        dilate( dst, dst, kernel, anchor, iterations, borderType, borderValue );
        break;
    case CC_MOP_CLOSE:
        dilate( src, dst, kernel, anchor, iterations, borderType, borderValue );
        erode( dst, dst, kernel, anchor, iterations, borderType, borderValue );
        break;
    case CC_MOP_GRADIENT:
        erode( src, temp, kernel, anchor, iterations, borderType, borderValue );
        dilate( src, dst, kernel, anchor, iterations, borderType, borderValue );
        dst -= temp;
        break;
    case CC_MOP_TOPHAT:
        if( src.data != dst.data )
            temp = dst;
        erode( src, temp, kernel, anchor, iterations, borderType, borderValue );
        dilate( temp, temp, kernel, anchor, iterations, borderType, borderValue );
        dst = src - temp;
        break;
    case CC_MOP_BLACKHAT:
        if( src.data != dst.data )
            temp = dst;
        dilate( src, temp, kernel, anchor, iterations, borderType, borderValue );
        erode( temp, temp, kernel, anchor, iterations, borderType, borderValue );
        dst = temp - src;
        break;
    case MORPH_HITMISS:
        CC_Assert(src->tid == CC_8UC1);
        k1 = (kernel == 1);
        k2 = (kernel == -1);
        if (countNonZero(k1) <= 0)
            e1 = src;
        else
            erode(src, e1, k1, anchor, iterations, borderType, borderValue);
        if (countNonZero(k2) <= 0)
            e2 = src;
        else
        {
            img_t src_complement;
            bitwise_not(src, src_complement);
            erode(src_complement, e2, k2, anchor, iterations, borderType, borderValue);
        }
        dst = e1 & e2;
        break;
    default:
        CC_Error( CC_StsBadArg, "unknown morphological operation" );
    }
}

CC_IMPL IplConvKernel *
cvCreateStructuringElementEx( int cols, int rows,
                              int anchorX, int anchorY,
                              int shape, int *values )
{
    CSize ksize = CSize(cols, rows);
    Point anchor = Point(anchorX, anchorY);
    CC_Assert( cols > 0 && rows > 0 && anchor.inside(Rect(0,0,cols,rows)) &&
               (shape != CC_SHAPE_CUSTOM || values != 0));

    int i, size = rows * cols;
    int element_size = sizeof(IplConvKernel) + size*sizeof(int);
    IplConvKernel *element = (IplConvKernel*)cAlloc(element_size + 32);

    element->nCols = cols;
    element->nRows = rows;
    element->anchorX = anchorX;
    element->anchorY = anchorY;
    element->nShiftR = shape < CC_SHAPE_ELLIPSE ? shape : CC_SHAPE_CUSTOM;
    element->values = (int*)(element + 1);

    if( shape == CC_SHAPE_CUSTOM )
    {
        for( i = 0; i < size; i++ )
            element->values[i] = values[i];
    }
    else
    {
        img_t elem = getStructuringElement(shape, ksize, anchor);
        for( i = 0; i < size; i++ )
            element->values[i] = elem->tt.data[i];
    }

    return element;
}


CC_IMPL void
cvReleaseStructuringElement( IplConvKernel ** element )
{
    if( !element )
        CC_Error( CC_StsNullPtr, "" );
    cFree( element );
}


static void convertConvKernel( const IplConvKernel* src, img_t& dst, Point& anchor )
{
    if(!src)
    {
        anchor = Point(1,1);
        dst.release();
        return;
    }
    anchor = Point(src->anchorX, src->anchorY);
    dst cvSetMat(src->nRows, src->nCols, CC_8U);

    int i, size = src->nRows*src->nCols;
    for( i = 0; i < size; i++ )
        dst->tt.data[i] = (uchar)(src->values[i] != 0);
}


CC_IMPL void
cvErode( const img_t* src, img_t* dst, IplConvKernel* element, int iterations )
{
    img_t src = cvarrToMat(srcarr), dst = cvarrToMat(dstarr), kernel;
    CC_Assert( cvGetSize(src) == dst.size() && src->tid == dst->tid );
    Point anchor;
    convertConvKernel( element, kernel, anchor );
    erode( src, dst, kernel, anchor, iterations, CC_BORDER_REPLICATE );
}


CC_IMPL void
cvDilate( const img_t* src, img_t* dst, IplConvKernel* element, int iterations )
{
    img_t src = cvarrToMat(srcarr), dst = cvarrToMat(dstarr), kernel;
    CC_Assert( cvGetSize(src) == dst.size() && src->tid == dst->tid );
    Point anchor;
    convertConvKernel( element, kernel, anchor );
    dilate( src, dst, kernel, anchor, iterations, CC_BORDER_REPLICATE );
}


CC_IMPL void
cvMorphologyEx( const img_t* src, img_t* dst, void*,
                IplConvKernel* element, int op, int iterations )
{
    img_t src = cvarrToMat(srcarr), dst = cvarrToMat(dstarr), kernel;
    CC_Assert( cvGetSize(src) == dst.size() && src->tid == dst->tid );
    Point anchor;
    IplConvKernel* temp_element = NULL;
    if (!element)
    {
        temp_element = cvCreateStructuringElementEx(3, 3, 1, 1, CC_SHAPE_RECT);
    } else {
        temp_element = element;
    }
    convertConvKernel( temp_element, kernel, anchor );
    if (!element)
    {
        cvReleaseStructuringElement(&temp_element);
    }
    morphologyEx( src, dst, op, kernel, anchor, iterations, CC_BORDER_REPLICATE );
}


