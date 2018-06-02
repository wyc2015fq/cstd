
#include "../core/saturate.hpp"
#include "hal_replacement.hpp"

struct CParallelLoopBody{};

//#define parallel_for_(a, b, c)  b(a)

template <typename T>
void parallel_for_(CRange range, T opt, double x) {
  opt(range);
  return ;
}


/* ////////////////////////////////////////////////////////////////////
//
//  Geometrical transforms on images and matrices: rotation, zoom etc.
//
// */

//#include "opencl_kernels_imgproc.hpp"
//#include "hal_replacement.hpp"
//#include "cv/core/openvx/ovx_defs.hpp"

#if IPP_VERSION_X100 >= 710
typedef IppStatus (C_STDCALL* ippiResizeFunc)(const void*, int, const void*, int, IppiPoint, IppiSize, IppiBorderType, void*, void*, Ipp8u*);
typedef IppStatus (C_STDCALL* ippiResizeGetBufferSize)(void*, IppiSize, Ipp32u, int*);
typedef IppStatus (C_STDCALL* ippiResizeGetSrcOffset)(void*, IppiPoint, IppiPoint*);
#endif

#if defined (HAVE_IPP) && (IPP_VERSION_X100 >= 700) && IPP_DISABLE_BLOCK
typedef IppStatus (C_STDCALL* ippiSetFunc)(const void*, void *, int, IppiSize);
typedef IppStatus (C_STDCALL* ippiWarpPerspectiveFunc)(const void*, IppiSize, int, IppiRect, void *, int, IppiRect, double [3][3], int);
typedef IppStatus (C_STDCALL* ippiWarpAffineBackFunc)(const void*, IppiSize, int, IppiRect, void *, int, IppiRect, double [2][3], int);

template <int channels, typename Type>
bool IPPSetSimple(CScalar value, void *dataPointer, int step, IppiSize &size, ippiSetFunc func)
{
    CC_INSTRUMENT_REGION_IPP()

    Type values[channels];
    for( int i = 0; i < channels; i++ )
        values[i] = saturate_cast<Type>(value[i]);
    return func(values, dataPointer, step, size) >= 0;
}

static bool IPPSet(const CScalar &value, void *dataPointer, int step, IppiSize &size, int channels, int depth)
{
    CC_INSTRUMENT_REGION_IPP()

    if( channels == 1 )
    {
        switch( depth )
        {
        case CC_8U:
            return CC_INSTRUMENT_FUN_IPP(ippiSet_8u_C1R,(saturate_cast<Ipp8u>(value[0]), (Ipp8u *)dataPointer, step, size)) >= 0;
        case CC_16U:
            return CC_INSTRUMENT_FUN_IPP(ippiSet_16u_C1R,(saturate_cast<Ipp16u>(value[0]), (Ipp16u *)dataPointer, step, size)) >= 0;
        case CC_32F:
            return CC_INSTRUMENT_FUN_IPP(ippiSet_32f_C1R,(saturate_cast<Ipp32f>(value[0]), (Ipp32f *)dataPointer, step, size)) >= 0;
        }
    }
    else
    {
        if( channels == 3 )
        {
            switch( depth )
            {
            case CC_8U:
                return IPPSetSimple<3, Ipp8u>(value, dataPointer, step, size, (ippiSetFunc)ippiSet_8u_C3R);
            case CC_16U:
                return IPPSetSimple<3, Ipp16u>(value, dataPointer, step, size, (ippiSetFunc)ippiSet_16u_C3R);
            case CC_32F:
                return IPPSetSimple<3, Ipp32f>(value, dataPointer, step, size, (ippiSetFunc)ippiSet_32f_C3R);
            }
        }
        else if( channels == 4 )
        {
            switch( depth )
            {
            case CC_8U:
                return IPPSetSimple<4, Ipp8u>(value, dataPointer, step, size, (ippiSetFunc)ippiSet_8u_C4R);
            case CC_16U:
                return IPPSetSimple<4, Ipp16u>(value, dataPointer, step, size, (ippiSetFunc)ippiSet_16u_C4R);
            case CC_32F:
                return IPPSetSimple<4, Ipp32f>(value, dataPointer, step, size, (ippiSetFunc)ippiSet_32f_C4R);
            }
        }
    }
    return false;
}
#endif

/************** interpolation formulas and tables ***************/

const int CC_INTER_RESIZE_COEF_BITS=11;
const int CC_INTER_RESIZE_COEF_SCALE=1 << CC_INTER_RESIZE_COEF_BITS;

const int CC_INTER_REMAP_COEF_BITS=15;
const int CC_INTER_REMAP_COEF_SCALE=1 << CC_INTER_REMAP_COEF_BITS;

static uchar NNDeltaTab_i[CC_INTER_TAB_SIZE2][2];

static float BilinearTab_f[CC_INTER_TAB_SIZE2][2][2];
static short BilinearTab_i[CC_INTER_TAB_SIZE2][2][2];

#if CC_SSE2_ || CC_NEON
static short BilinearTab_iC4_buf[CC_INTER_TAB_SIZE2+2][2][8];
static short (*BilinearTab_iC4)[2][8] = (short (*)[2][8])cAlignPtr(BilinearTab_iC4_buf, 16);
#endif

static float BicubicTab_f[CC_INTER_TAB_SIZE2][4][4];
static short BicubicTab_i[CC_INTER_TAB_SIZE2][4][4];

static float Lanczos4Tab_f[CC_INTER_TAB_SIZE2][8][8];
static short Lanczos4Tab_i[CC_INTER_TAB_SIZE2][8][8];

static inline void interpolateLinear( float x, float* coeffs )
{
    coeffs[0] = 1.f - x;
    coeffs[1] = x;
}

static inline void interpolateCubic( float x, float* coeffs )
{
    const float A = -0.75f;

    coeffs[0] = ((A*(x + 1) - 5*A)*(x + 1) + 8*A)*(x + 1) - 4*A;
    coeffs[1] = ((A + 2)*x - (A + 3))*x*x + 1;
    coeffs[2] = ((A + 2)*(1 - x) - (A + 3))*(1 - x)*(1 - x) + 1;
    coeffs[3] = 1.f - coeffs[0] - coeffs[1] - coeffs[2];
}

static inline void interpolateLanczos4( float x, float* coeffs )
{
    static const double s45 = 0.70710678118654752440084436210485;
    static const double cs[][2]=
    {{1, 0}, {-s45, -s45}, {0, 1}, {s45, -s45}, {-1, 0}, {s45, s45}, {0, -1}, {-s45, s45}};

    if( x < FLT_EPSILON )
    {
        for( int i = 0; i < 8; i++ )
            coeffs[i] = 0;
        coeffs[3] = 1;
        return;
    }

    float sum = 0;
    double y0=-(x+3)*CC_PI*0.25, s0 = sin(y0), c0=cos(y0);
    int i;
    for(i = 0; i < 8; i++ )
    {
        double y = -(x+3-i)*CC_PI*0.25;
        coeffs[i] = (float)((cs[i][0]*s0 + cs[i][1]*c0)/(y*y));
        sum += coeffs[i];
    }

    sum = 1.f/sum;
    for(i = 0; i < 8; i++ )
        coeffs[i] *= sum;
}

static void initInterTab1D(int method, float* tab, int tabsz)
{
    float scale = 1.f/tabsz;
    if( method == CC_INTER_LINEAR )
    {
        for( int i = 0; i < tabsz; i++, tab += 2 )
            interpolateLinear( i*scale, tab );
    }
    else if( method == CC_INTER_CUBIC )
    {
        for( int i = 0; i < tabsz; i++, tab += 4 )
            interpolateCubic( i*scale, tab );
    }
    else if( method == CC_INTER_LANCZOS4 )
    {
        for( int i = 0; i < tabsz; i++, tab += 8 )
            interpolateLanczos4( i*scale, tab );
    }
    else
        CC_Error( CC_StsBadArg, "Unknown interpolation method" );
}

static const void* initInterTab2D( int method, bool fixpt )
{
    static bool inittab[CC_INTER_MAX+1] = {false};
    float* tab = 0;
    short* itab = 0;
    int ksize = 0;
    if( method == CC_INTER_LINEAR )
        tab = BilinearTab_f[0][0], itab = BilinearTab_i[0][0], ksize=2;
    else if( method == CC_INTER_CUBIC )
        tab = BicubicTab_f[0][0], itab = BicubicTab_i[0][0], ksize=4;
    else if( method == CC_INTER_LANCZOS4 )
        tab = Lanczos4Tab_f[0][0], itab = Lanczos4Tab_i[0][0], ksize=8;
    else
        CC_Error( CC_StsBadArg, "Unknown/unsupported interpolation type" );

    if( !inittab[method] )
    {
        float *_tab = MALLOC(float, 8*CC_INTER_TAB_SIZE);
        int i, j, k1, k2;
        initInterTab1D(method, _tab, CC_INTER_TAB_SIZE);
        for( i = 0; i < CC_INTER_TAB_SIZE; i++ )
            for( j = 0; j < CC_INTER_TAB_SIZE; j++, tab += ksize*ksize, itab += ksize*ksize )
            {
                int isum = 0;
                NNDeltaTab_i[i*CC_INTER_TAB_SIZE+j][0] = j < CC_INTER_TAB_SIZE/2;
                NNDeltaTab_i[i*CC_INTER_TAB_SIZE+j][1] = i < CC_INTER_TAB_SIZE/2;

                for( k1 = 0; k1 < ksize; k1++ )
                {
                    float vy = _tab[i*ksize + k1];
                    for( k2 = 0; k2 < ksize; k2++ )
                    {
                        float v = vy*_tab[j*ksize + k2];
                        tab[k1*ksize + k2] = v;
                        isum += itab[k1*ksize + k2] = float_to_short(v*CC_INTER_REMAP_COEF_SCALE);
                    }
                }

                if( isum != CC_INTER_REMAP_COEF_SCALE )
                {
                    int diff = isum - CC_INTER_REMAP_COEF_SCALE;
                    int ksize2 = ksize/2, Mk1=ksize2, Mk2=ksize2, mk1=ksize2, mk2=ksize2;
                    for( k1 = ksize2; k1 < ksize2+2; k1++ )
                        for( k2 = ksize2; k2 < ksize2+2; k2++ )
                        {
                            if( itab[k1*ksize+k2] < itab[mk1*ksize+mk2] )
                                mk1 = k1, mk2 = k2;
                            else if( itab[k1*ksize+k2] > itab[Mk1*ksize+Mk2] )
                                Mk1 = k1, Mk2 = k2;
                        }
                    if( diff < 0 )
                        itab[Mk1*ksize + Mk2] = (short)(itab[Mk1*ksize + Mk2] - diff);
                    else
                        itab[mk1*ksize + mk2] = (short)(itab[mk1*ksize + mk2] - diff);
                }
            }
        tab -= CC_INTER_TAB_SIZE2*ksize*ksize;
        itab -= CC_INTER_TAB_SIZE2*ksize*ksize;
#if CC_SSE2_ || CC_NEON
        if( method == CC_INTER_LINEAR )
        {
            for( i = 0; i < CC_INTER_TAB_SIZE2; i++ )
                for( j = 0; j < 4; j++ )
                {
                    BilinearTab_iC4[i][0][j*2] = BilinearTab_i[i][0][0];
                    BilinearTab_iC4[i][0][j*2+1] = BilinearTab_i[i][0][1];
                    BilinearTab_iC4[i][1][j*2] = BilinearTab_i[i][1][0];
                    BilinearTab_iC4[i][1][j*2+1] = BilinearTab_i[i][1][1];
                }
        }
#endif
        inittab[method] = true;
        FREE(_tab);
    }
    return fixpt ? (const void*)itab : (const void*)tab;
}

#ifndef __MINGW32__
static bool initAllInterTab2D()
{
    return  initInterTab2D( CC_INTER_LINEAR, false ) &&
            initInterTab2D( CC_INTER_LINEAR, true ) &&
            initInterTab2D( CC_INTER_CUBIC, false ) &&
            initInterTab2D( CC_INTER_CUBIC, true ) &&
            initInterTab2D( CC_INTER_LANCZOS4, false ) &&
            initInterTab2D( CC_INTER_LANCZOS4, true );
}

static volatile bool doInitAllInterTab2D = initAllInterTab2D();
#endif

template<typename ST, typename DT> struct Cast
{
    typedef ST type1;
    typedef DT rtype;

    DT operator()(ST val) const { DT dt; saturate_cast(dt, val); return dt; }
};

template<typename ST, typename DT, int bits> struct FixedPtCast
{
    typedef ST type1;
    typedef DT rtype;
    enum { SHIFT = bits, DELTA = 1 << (bits-1) };

    DT operator()(ST val) const { DT dt; saturate_cast(dt, (val + DELTA)>>SHIFT); return dt; }
};

/****************************************************************************************\
*                                         Resize                                         *
\****************************************************************************************/

struct resizeNNInvoker : public CParallelLoopBody
{
public:
    resizeNNInvoker(const img_t* src, img_t* dst, int *_x_ofs, int _pix_size4, double _ify) :
        CParallelLoopBody(), src(src), dst(dst), x_ofs(_x_ofs), pix_size4(_pix_size4),
        ify(_ify)
    {
    }

    virtual void operator() (const CRange& range) const
    {
        CSize ssize = cvGetSize(src), dsize = cvGetSize(dst);
        int y, x, pix_size = (int)CC_TYPE_SIZE(src->tid);

        for( y = range.start; y < range.end; y++ )
        {
            uchar* D = dst->tt.data + dst->step*y;
            int sy = MIN(cFloor(y*ify), ssize.height-1);
            const uchar* S = img_row(uchar, src, sy);

            switch( pix_size )
            {
            case 1:
                for( x = 0; x <= dsize.width - 2; x += 2 )
                {
                    uchar t0 = S[x_ofs[x]];
                    uchar t1 = S[x_ofs[x+1]];
                    D[x] = t0;
                    D[x+1] = t1;
                }

                for( ; x < dsize.width; x++ )
                    D[x] = S[x_ofs[x]];
                break;
            case 2:
                for( x = 0; x < dsize.width; x++ )
                    *(ushort*)(D + x*2) = *(ushort*)(S + x_ofs[x]);
                break;
            case 3:
                for( x = 0; x < dsize.width; x++, D += 3 )
                {
                    const uchar* _tS = S + x_ofs[x];
                    D[0] = _tS[0]; D[1] = _tS[1]; D[2] = _tS[2];
                }
                break;
            case 4:
                for( x = 0; x < dsize.width; x++ )
                    *(int*)(D + x*4) = *(int*)(S + x_ofs[x]);
                break;
            case 6:
                for( x = 0; x < dsize.width; x++, D += 6 )
                {
                    const ushort* _tS = (const ushort*)(S + x_ofs[x]);
                    ushort* _tD = (ushort*)D;
                    _tD[0] = _tS[0]; _tD[1] = _tS[1]; _tD[2] = _tS[2];
                }
                break;
            case 8:
                for( x = 0; x < dsize.width; x++, D += 8 )
                {
                    const int* _tS = (const int*)(S + x_ofs[x]);
                    int* _tD = (int*)D;
                    _tD[0] = _tS[0]; _tD[1] = _tS[1];
                }
                break;
            case 12:
                for( x = 0; x < dsize.width; x++, D += 12 )
                {
                    const int* _tS = (const int*)(S + x_ofs[x]);
                    int* _tD = (int*)D;
                    _tD[0] = _tS[0]; _tD[1] = _tS[1]; _tD[2] = _tS[2];
                }
                break;
            default:
                for( x = 0; x < dsize.width; x++, D += pix_size )
                {
                    const int* _tS = (const int*)(S + x_ofs[x]);
                    int* _tD = (int*)D;
                    for( int k = 0; k < pix_size4; k++ )
                        _tD[k] = _tS[k];
                }
            }
        }
    }

    const img_t* src;
    img_t* dst;
    int* x_ofs, pix_size4;
    double ify;

    //resizeNNInvoker(const resizeNNInvoker&);
    //resizeNNInvoker& operator=(const resizeNNInvoker&);
};

static void
resizeNN( const img_t* src, img_t* dst, double fx, double fy )
{
    CSize ssize = cvGetSize(src), dsize = cvGetSize(dst);
    int* _x_ofs = MALLOC(int, dsize.width);
    int* x_ofs = _x_ofs;
    int pix_size = (int)CC_TYPE_SIZE(src->tid);
    int pix_size4 = (int)(pix_size / sizeof(int));
    double ifx = 1./fx, ify = 1./fy;
    int x;

    for( x = 0; x < dsize.width; x++ )
    {
        int sx = cFloor(x*ifx);
        x_ofs[x] = MIN(sx, ssize.width-1)*pix_size;
    }

    CRange range = {0, dsize.height};
    resizeNNInvoker invoker(src, dst, x_ofs, pix_size4, ify);
    parallel_for_(range, invoker, CC_MAT_TOTAL(dst)/(double)(1<<16));
    FREE(_x_ofs);
}


struct VResizeNoVec
{
    int operator()(const uchar**, uchar*, const uchar*, int ) const { return 0; }
};

struct HResizeNoVec
{
    int operator()(const uchar**, uchar**, int, const int*,
        const uchar*, int, int, int, int, int) const { return 0; }
};

#if CC_SSE2_

struct VResizeLinearVec_32s8u
{
    int operator()(const uchar** src, uchar* dst, const uchar* _beta, int width ) const
    {
        if( !checkHardwareSupport(CC_CPU_SSE2) )
            return 0;

        const int** src = (const int**)src;
        const short* beta = (const short*)_beta;
        const int *S0 = src[0], *S1 = src[1];
        int x = 0;
        __m128i b0 = _mm_set1_epi16(beta[0]), b1 = _mm_set1_epi16(beta[1]);
        __m128i delta = _mm_set1_epi16(2);

        if( (((size_t)S0|(size_t)S1)&15) == 0 )
            for( ; x <= width - 16; x += 16 )
            {
                __m128i x0, x1, x2, y0, y1, y2;
                x0 = _mm_load_si128((const __m128i*)(S0 + x));
                x1 = _mm_load_si128((const __m128i*)(S0 + x + 4));
                y0 = _mm_load_si128((const __m128i*)(S1 + x));
                y1 = _mm_load_si128((const __m128i*)(S1 + x + 4));
                x0 = _mm_packs_epi32(_mm_srai_epi32(x0, 4), _mm_srai_epi32(x1, 4));
                y0 = _mm_packs_epi32(_mm_srai_epi32(y0, 4), _mm_srai_epi32(y1, 4));

                x1 = _mm_load_si128((const __m128i*)(S0 + x + 8));
                x2 = _mm_load_si128((const __m128i*)(S0 + x + 12));
                y1 = _mm_load_si128((const __m128i*)(S1 + x + 8));
                y2 = _mm_load_si128((const __m128i*)(S1 + x + 12));
                x1 = _mm_packs_epi32(_mm_srai_epi32(x1, 4), _mm_srai_epi32(x2, 4));
                y1 = _mm_packs_epi32(_mm_srai_epi32(y1, 4), _mm_srai_epi32(y2, 4));

                x0 = _mm_adds_epi16(_mm_mulhi_epi16( x0, b0 ), _mm_mulhi_epi16( y0, b1 ));
                x1 = _mm_adds_epi16(_mm_mulhi_epi16( x1, b0 ), _mm_mulhi_epi16( y1, b1 ));

                x0 = _mm_srai_epi16(_mm_adds_epi16(x0, delta), 2);
                x1 = _mm_srai_epi16(_mm_adds_epi16(x1, delta), 2);
                _mm_storeu_si128( (__m128i*)(dst + x), _mm_packus_epi16(x0, x1));
            }
        else
            for( ; x <= width - 16; x += 16 )
            {
                __m128i x0, x1, x2, y0, y1, y2;
                x0 = _mm_loadu_si128((const __m128i*)(S0 + x));
                x1 = _mm_loadu_si128((const __m128i*)(S0 + x + 4));
                y0 = _mm_loadu_si128((const __m128i*)(S1 + x));
                y1 = _mm_loadu_si128((const __m128i*)(S1 + x + 4));
                x0 = _mm_packs_epi32(_mm_srai_epi32(x0, 4), _mm_srai_epi32(x1, 4));
                y0 = _mm_packs_epi32(_mm_srai_epi32(y0, 4), _mm_srai_epi32(y1, 4));

                x1 = _mm_loadu_si128((const __m128i*)(S0 + x + 8));
                x2 = _mm_loadu_si128((const __m128i*)(S0 + x + 12));
                y1 = _mm_loadu_si128((const __m128i*)(S1 + x + 8));
                y2 = _mm_loadu_si128((const __m128i*)(S1 + x + 12));
                x1 = _mm_packs_epi32(_mm_srai_epi32(x1, 4), _mm_srai_epi32(x2, 4));
                y1 = _mm_packs_epi32(_mm_srai_epi32(y1, 4), _mm_srai_epi32(y2, 4));

                x0 = _mm_adds_epi16(_mm_mulhi_epi16( x0, b0 ), _mm_mulhi_epi16( y0, b1 ));
                x1 = _mm_adds_epi16(_mm_mulhi_epi16( x1, b0 ), _mm_mulhi_epi16( y1, b1 ));

                x0 = _mm_srai_epi16(_mm_adds_epi16(x0, delta), 2);
                x1 = _mm_srai_epi16(_mm_adds_epi16(x1, delta), 2);
                _mm_storeu_si128( (__m128i*)(dst + x), _mm_packus_epi16(x0, x1));
            }

        for( ; x < width - 4; x += 4 )
        {
            __m128i x0, y0;
            x0 = _mm_srai_epi32(_mm_loadu_si128((const __m128i*)(S0 + x)), 4);
            y0 = _mm_srai_epi32(_mm_loadu_si128((const __m128i*)(S1 + x)), 4);
            x0 = _mm_packs_epi32(x0, x0);
            y0 = _mm_packs_epi32(y0, y0);
            x0 = _mm_adds_epi16(_mm_mulhi_epi16(x0, b0), _mm_mulhi_epi16(y0, b1));
            x0 = _mm_srai_epi16(_mm_adds_epi16(x0, delta), 2);
            x0 = _mm_packus_epi16(x0, x0);
            *(int*)(dst + x) = _mm_cvtsi128_si32(x0);
        }

        return x;
    }
};


template<int shiftval> struct VResizeLinearVec_32f16
{
    int operator()(const uchar** src, uchar* dst, const uchar* _beta, int width ) const
    {
        if( !checkHardwareSupport(CC_CPU_SSE2) )
            return 0;

        const float** src = (const float**)src;
        const float* beta = (const float*)_beta;
        const float *S0 = src[0], *S1 = src[1];
        ushort* dst = (ushort*)dst;
        int x = 0;

        __m128 b0 = _mm_set1_ps(beta[0]), b1 = _mm_set1_ps(beta[1]);
        __m128i preshift = _mm_set1_epi32(shiftval);
        __m128i postshift = _mm_set1_epi16((short)shiftval);

        if( (((size_t)S0|(size_t)S1)&15) == 0 )
            for( ; x <= width - 16; x += 16 )
            {
                __m128 x0, x1, y0, y1;
                __m128i t0, t1, t2;
                x0 = _mm_load_ps(S0 + x);
                x1 = _mm_load_ps(S0 + x + 4);
                y0 = _mm_load_ps(S1 + x);
                y1 = _mm_load_ps(S1 + x + 4);

                x0 = _mm_add_ps(_mm_mul_ps(x0, b0), _mm_mul_ps(y0, b1));
                x1 = _mm_add_ps(_mm_mul_ps(x1, b0), _mm_mul_ps(y1, b1));
                t0 = _mm_add_epi32(_mm_cvtps_epi32(x0), preshift);
                t2 = _mm_add_epi32(_mm_cvtps_epi32(x1), preshift);
                t0 = _mm_add_epi16(_mm_packs_epi32(t0, t2), postshift);

                x0 = _mm_load_ps(S0 + x + 8);
                x1 = _mm_load_ps(S0 + x + 12);
                y0 = _mm_load_ps(S1 + x + 8);
                y1 = _mm_load_ps(S1 + x + 12);

                x0 = _mm_add_ps(_mm_mul_ps(x0, b0), _mm_mul_ps(y0, b1));
                x1 = _mm_add_ps(_mm_mul_ps(x1, b0), _mm_mul_ps(y1, b1));
                t1 = _mm_add_epi32(_mm_cvtps_epi32(x0), preshift);
                t2 = _mm_add_epi32(_mm_cvtps_epi32(x1), preshift);
                t1 = _mm_add_epi16(_mm_packs_epi32(t1, t2), postshift);

                _mm_storeu_si128( (__m128i*)(dst + x), t0);
                _mm_storeu_si128( (__m128i*)(dst + x + 8), t1);
            }
        else
            for( ; x <= width - 16; x += 16 )
            {
                __m128 x0, x1, y0, y1;
                __m128i t0, t1, t2;
                x0 = _mm_loadu_ps(S0 + x);
                x1 = _mm_loadu_ps(S0 + x + 4);
                y0 = _mm_loadu_ps(S1 + x);
                y1 = _mm_loadu_ps(S1 + x + 4);

                x0 = _mm_add_ps(_mm_mul_ps(x0, b0), _mm_mul_ps(y0, b1));
                x1 = _mm_add_ps(_mm_mul_ps(x1, b0), _mm_mul_ps(y1, b1));
                t0 = _mm_add_epi32(_mm_cvtps_epi32(x0), preshift);
                t2 = _mm_add_epi32(_mm_cvtps_epi32(x1), preshift);
                t0 = _mm_add_epi16(_mm_packs_epi32(t0, t2), postshift);

                x0 = _mm_loadu_ps(S0 + x + 8);
                x1 = _mm_loadu_ps(S0 + x + 12);
                y0 = _mm_loadu_ps(S1 + x + 8);
                y1 = _mm_loadu_ps(S1 + x + 12);

                x0 = _mm_add_ps(_mm_mul_ps(x0, b0), _mm_mul_ps(y0, b1));
                x1 = _mm_add_ps(_mm_mul_ps(x1, b0), _mm_mul_ps(y1, b1));
                t1 = _mm_add_epi32(_mm_cvtps_epi32(x0), preshift);
                t2 = _mm_add_epi32(_mm_cvtps_epi32(x1), preshift);
                t1 = _mm_add_epi16(_mm_packs_epi32(t1, t2), postshift);

                _mm_storeu_si128( (__m128i*)(dst + x), t0);
                _mm_storeu_si128( (__m128i*)(dst + x + 8), t1);
            }

        for( ; x < width - 4; x += 4 )
        {
            __m128 x0, y0;
            __m128i t0;
            x0 = _mm_loadu_ps(S0 + x);
            y0 = _mm_loadu_ps(S1 + x);

            x0 = _mm_add_ps(_mm_mul_ps(x0, b0), _mm_mul_ps(y0, b1));
            t0 = _mm_add_epi32(_mm_cvtps_epi32(x0), preshift);
            t0 = _mm_add_epi16(_mm_packs_epi32(t0, t0), postshift);
            _mm_storel_epi64( (__m128i*)(dst + x), t0);
        }

        return x;
    }
};

typedef VResizeLinearVec_32f16<SHRT_MIN> VResizeLinearVec_32f16u;
typedef VResizeLinearVec_32f16<0> VResizeLinearVec_32f16s;

struct VResizeLinearVec_32f
{
    int operator()(const uchar** src, uchar* dst, const uchar* _beta, int width ) const
    {
        if( !checkHardwareSupport(CC_CPU_SSE) )
            return 0;

        const float** src = (const float**)src;
        const float* beta = (const float*)_beta;
        const float *S0 = src[0], *S1 = src[1];
        float* dst = (float*)dst;
        int x = 0;

        __m128 b0 = _mm_set1_ps(beta[0]), b1 = _mm_set1_ps(beta[1]);

        if( (((size_t)S0|(size_t)S1)&15) == 0 )
            for( ; x <= width - 8; x += 8 )
            {
                __m128 x0, x1, y0, y1;
                x0 = _mm_load_ps(S0 + x);
                x1 = _mm_load_ps(S0 + x + 4);
                y0 = _mm_load_ps(S1 + x);
                y1 = _mm_load_ps(S1 + x + 4);

                x0 = _mm_add_ps(_mm_mul_ps(x0, b0), _mm_mul_ps(y0, b1));
                x1 = _mm_add_ps(_mm_mul_ps(x1, b0), _mm_mul_ps(y1, b1));

                _mm_storeu_ps( dst + x, x0);
                _mm_storeu_ps( dst + x + 4, x1);
            }
        else
            for( ; x <= width - 8; x += 8 )
            {
                __m128 x0, x1, y0, y1;
                x0 = _mm_loadu_ps(S0 + x);
                x1 = _mm_loadu_ps(S0 + x + 4);
                y0 = _mm_loadu_ps(S1 + x);
                y1 = _mm_loadu_ps(S1 + x + 4);

                x0 = _mm_add_ps(_mm_mul_ps(x0, b0), _mm_mul_ps(y0, b1));
                x1 = _mm_add_ps(_mm_mul_ps(x1, b0), _mm_mul_ps(y1, b1));

                _mm_storeu_ps( dst + x, x0);
                _mm_storeu_ps( dst + x + 4, x1);
            }

        return x;
    }
};


struct VResizeCubicVec_32s8u
{
    int operator()(const uchar** src, uchar* dst, const uchar* _beta, int width ) const
    {
        if( !checkHardwareSupport(CC_CPU_SSE2) )
            return 0;

        const int** src = (const int**)src;
        const short* beta = (const short*)_beta;
        const int *S0 = src[0], *S1 = src[1], *S2 = src[2], *S3 = src[3];
        int x = 0;
        float scale = 1.f/(CC_INTER_RESIZE_COEF_SCALE*CC_INTER_RESIZE_COEF_SCALE);
        __m128 b0 = _mm_set1_ps(beta[0]*scale), b1 = _mm_set1_ps(beta[1]*scale),
            b2 = _mm_set1_ps(beta[2]*scale), b3 = _mm_set1_ps(beta[3]*scale);

        if( (((size_t)S0|(size_t)S1|(size_t)S2|(size_t)S3)&15) == 0 )
            for( ; x <= width - 8; x += 8 )
            {
                __m128i x0, x1, y0, y1;
                __m128 s0, s1, f0, f1;
                x0 = _mm_load_si128((const __m128i*)(S0 + x));
                x1 = _mm_load_si128((const __m128i*)(S0 + x + 4));
                y0 = _mm_load_si128((const __m128i*)(S1 + x));
                y1 = _mm_load_si128((const __m128i*)(S1 + x + 4));

                s0 = _mm_mul_ps(_mm_cvtepi32_ps(x0), b0);
                s1 = _mm_mul_ps(_mm_cvtepi32_ps(x1), b0);
                f0 = _mm_mul_ps(_mm_cvtepi32_ps(y0), b1);
                f1 = _mm_mul_ps(_mm_cvtepi32_ps(y1), b1);
                s0 = _mm_add_ps(s0, f0);
                s1 = _mm_add_ps(s1, f1);

                x0 = _mm_load_si128((const __m128i*)(S2 + x));
                x1 = _mm_load_si128((const __m128i*)(S2 + x + 4));
                y0 = _mm_load_si128((const __m128i*)(S3 + x));
                y1 = _mm_load_si128((const __m128i*)(S3 + x + 4));

                f0 = _mm_mul_ps(_mm_cvtepi32_ps(x0), b2);
                f1 = _mm_mul_ps(_mm_cvtepi32_ps(x1), b2);
                s0 = _mm_add_ps(s0, f0);
                s1 = _mm_add_ps(s1, f1);
                f0 = _mm_mul_ps(_mm_cvtepi32_ps(y0), b3);
                f1 = _mm_mul_ps(_mm_cvtepi32_ps(y1), b3);
                s0 = _mm_add_ps(s0, f0);
                s1 = _mm_add_ps(s1, f1);

                x0 = _mm_cvtps_epi32(s0);
                x1 = _mm_cvtps_epi32(s1);

                x0 = _mm_packs_epi32(x0, x1);
                _mm_storel_epi64( (__m128i*)(dst + x), _mm_packus_epi16(x0, x0));
            }
        else
            for( ; x <= width - 8; x += 8 )
            {
                __m128i x0, x1, y0, y1;
                __m128 s0, s1, f0, f1;
                x0 = _mm_loadu_si128((const __m128i*)(S0 + x));
                x1 = _mm_loadu_si128((const __m128i*)(S0 + x + 4));
                y0 = _mm_loadu_si128((const __m128i*)(S1 + x));
                y1 = _mm_loadu_si128((const __m128i*)(S1 + x + 4));

                s0 = _mm_mul_ps(_mm_cvtepi32_ps(x0), b0);
                s1 = _mm_mul_ps(_mm_cvtepi32_ps(x1), b0);
                f0 = _mm_mul_ps(_mm_cvtepi32_ps(y0), b1);
                f1 = _mm_mul_ps(_mm_cvtepi32_ps(y1), b1);
                s0 = _mm_add_ps(s0, f0);
                s1 = _mm_add_ps(s1, f1);

                x0 = _mm_loadu_si128((const __m128i*)(S2 + x));
                x1 = _mm_loadu_si128((const __m128i*)(S2 + x + 4));
                y0 = _mm_loadu_si128((const __m128i*)(S3 + x));
                y1 = _mm_loadu_si128((const __m128i*)(S3 + x + 4));

                f0 = _mm_mul_ps(_mm_cvtepi32_ps(x0), b2);
                f1 = _mm_mul_ps(_mm_cvtepi32_ps(x1), b2);
                s0 = _mm_add_ps(s0, f0);
                s1 = _mm_add_ps(s1, f1);
                f0 = _mm_mul_ps(_mm_cvtepi32_ps(y0), b3);
                f1 = _mm_mul_ps(_mm_cvtepi32_ps(y1), b3);
                s0 = _mm_add_ps(s0, f0);
                s1 = _mm_add_ps(s1, f1);

                x0 = _mm_cvtps_epi32(s0);
                x1 = _mm_cvtps_epi32(s1);

                x0 = _mm_packs_epi32(x0, x1);
                _mm_storel_epi64( (__m128i*)(dst + x), _mm_packus_epi16(x0, x0));
            }

        return x;
    }
};


template<int shiftval> struct VResizeCubicVec_32f16
{
    int operator()(const uchar** src, uchar* dst, const uchar* _beta, int width ) const
    {
        if( !checkHardwareSupport(CC_CPU_SSE2) )
            return 0;

        const float** src = (const float**)src;
        const float* beta = (const float*)_beta;
        const float *S0 = src[0], *S1 = src[1], *S2 = src[2], *S3 = src[3];
        ushort* dst = (ushort*)dst;
        int x = 0;
        __m128 b0 = _mm_set1_ps(beta[0]), b1 = _mm_set1_ps(beta[1]),
            b2 = _mm_set1_ps(beta[2]), b3 = _mm_set1_ps(beta[3]);
        __m128i preshift = _mm_set1_epi32(shiftval);
        __m128i postshift = _mm_set1_epi16((short)shiftval);

        for( ; x <= width - 8; x += 8 )
        {
            __m128 x0, x1, y0, y1, s0, s1;
            __m128i t0, t1;
            x0 = _mm_loadu_ps(S0 + x);
            x1 = _mm_loadu_ps(S0 + x + 4);
            y0 = _mm_loadu_ps(S1 + x);
            y1 = _mm_loadu_ps(S1 + x + 4);

            s0 = _mm_mul_ps(x0, b0);
            s1 = _mm_mul_ps(x1, b0);
            y0 = _mm_mul_ps(y0, b1);
            y1 = _mm_mul_ps(y1, b1);
            s0 = _mm_add_ps(s0, y0);
            s1 = _mm_add_ps(s1, y1);

            x0 = _mm_loadu_ps(S2 + x);
            x1 = _mm_loadu_ps(S2 + x + 4);
            y0 = _mm_loadu_ps(S3 + x);
            y1 = _mm_loadu_ps(S3 + x + 4);

            x0 = _mm_mul_ps(x0, b2);
            x1 = _mm_mul_ps(x1, b2);
            y0 = _mm_mul_ps(y0, b3);
            y1 = _mm_mul_ps(y1, b3);
            s0 = _mm_add_ps(s0, x0);
            s1 = _mm_add_ps(s1, x1);
            s0 = _mm_add_ps(s0, y0);
            s1 = _mm_add_ps(s1, y1);

            t0 = _mm_add_epi32(_mm_cvtps_epi32(s0), preshift);
            t1 = _mm_add_epi32(_mm_cvtps_epi32(s1), preshift);

            t0 = _mm_add_epi16(_mm_packs_epi32(t0, t1), postshift);
            _mm_storeu_si128( (__m128i*)(dst + x), t0);
        }

        return x;
    }
};

typedef VResizeCubicVec_32f16<SHRT_MIN> VResizeCubicVec_32f16u;
typedef VResizeCubicVec_32f16<0> VResizeCubicVec_32f16s;

struct VResizeCubicVec_32f
{
    int operator()(const uchar** src, uchar* dst, const uchar* _beta, int width ) const
    {
        if( !checkHardwareSupport(CC_CPU_SSE) )
            return 0;

        const float** src = (const float**)src;
        const float* beta = (const float*)_beta;
        const float *S0 = src[0], *S1 = src[1], *S2 = src[2], *S3 = src[3];
        float* dst = (float*)dst;
        int x = 0;
        __m128 b0 = _mm_set1_ps(beta[0]), b1 = _mm_set1_ps(beta[1]),
            b2 = _mm_set1_ps(beta[2]), b3 = _mm_set1_ps(beta[3]);

        for( ; x <= width - 8; x += 8 )
        {
            __m128 x0, x1, y0, y1, s0, s1;
            x0 = _mm_loadu_ps(S0 + x);
            x1 = _mm_loadu_ps(S0 + x + 4);
            y0 = _mm_loadu_ps(S1 + x);
            y1 = _mm_loadu_ps(S1 + x + 4);

            s0 = _mm_mul_ps(x0, b0);
            s1 = _mm_mul_ps(x1, b0);
            y0 = _mm_mul_ps(y0, b1);
            y1 = _mm_mul_ps(y1, b1);
            s0 = _mm_add_ps(s0, y0);
            s1 = _mm_add_ps(s1, y1);

            x0 = _mm_loadu_ps(S2 + x);
            x1 = _mm_loadu_ps(S2 + x + 4);
            y0 = _mm_loadu_ps(S3 + x);
            y1 = _mm_loadu_ps(S3 + x + 4);

            x0 = _mm_mul_ps(x0, b2);
            x1 = _mm_mul_ps(x1, b2);
            y0 = _mm_mul_ps(y0, b3);
            y1 = _mm_mul_ps(y1, b3);
            s0 = _mm_add_ps(s0, x0);
            s1 = _mm_add_ps(s1, x1);
            s0 = _mm_add_ps(s0, y0);
            s1 = _mm_add_ps(s1, y1);

            _mm_storeu_ps( dst + x, s0);
            _mm_storeu_ps( dst + x + 4, s1);
        }

        return x;
    }
};

#if CC_SSE4_1

struct VResizeLanczos4Vec_32f16u
{
    int operator()(const uchar** src, uchar* dst, const uchar* _beta, int width ) const
    {
        const float** src = (const float**)src;
        const float* beta = (const float*)_beta;
        const float *S0 = src[0], *S1 = src[1], *S2 = src[2], *S3 = src[3],
                    *S4 = src[4], *S5 = src[5], *S6 = src[6], *S7 = src[7];
        short * dst = (short*)dst;
        int x = 0;
        __m128 v_b0 = _mm_set1_ps(beta[0]), v_b1 = _mm_set1_ps(beta[1]),
               v_b2 = _mm_set1_ps(beta[2]), v_b3 = _mm_set1_ps(beta[3]),
               v_b4 = _mm_set1_ps(beta[4]), v_b5 = _mm_set1_ps(beta[5]),
               v_b6 = _mm_set1_ps(beta[6]), v_b7 = _mm_set1_ps(beta[7]);

        for( ; x <= width - 8; x += 8 )
        {
            __m128 v_dst0 = _mm_mul_ps(v_b0, _mm_loadu_ps(S0 + x));
            v_dst0 = _mm_add_ps(v_dst0, _mm_mul_ps(v_b1, _mm_loadu_ps(S1 + x)));
            v_dst0 = _mm_add_ps(v_dst0, _mm_mul_ps(v_b2, _mm_loadu_ps(S2 + x)));
            v_dst0 = _mm_add_ps(v_dst0, _mm_mul_ps(v_b3, _mm_loadu_ps(S3 + x)));
            v_dst0 = _mm_add_ps(v_dst0, _mm_mul_ps(v_b4, _mm_loadu_ps(S4 + x)));
            v_dst0 = _mm_add_ps(v_dst0, _mm_mul_ps(v_b5, _mm_loadu_ps(S5 + x)));
            v_dst0 = _mm_add_ps(v_dst0, _mm_mul_ps(v_b6, _mm_loadu_ps(S6 + x)));
            v_dst0 = _mm_add_ps(v_dst0, _mm_mul_ps(v_b7, _mm_loadu_ps(S7 + x)));

            __m128 v_dst1 = _mm_mul_ps(v_b0, _mm_loadu_ps(S0 + x + 4));
            v_dst1 = _mm_add_ps(v_dst1, _mm_mul_ps(v_b1, _mm_loadu_ps(S1 + x + 4)));
            v_dst1 = _mm_add_ps(v_dst1, _mm_mul_ps(v_b2, _mm_loadu_ps(S2 + x + 4)));
            v_dst1 = _mm_add_ps(v_dst1, _mm_mul_ps(v_b3, _mm_loadu_ps(S3 + x + 4)));
            v_dst1 = _mm_add_ps(v_dst1, _mm_mul_ps(v_b4, _mm_loadu_ps(S4 + x + 4)));
            v_dst1 = _mm_add_ps(v_dst1, _mm_mul_ps(v_b5, _mm_loadu_ps(S5 + x + 4)));
            v_dst1 = _mm_add_ps(v_dst1, _mm_mul_ps(v_b6, _mm_loadu_ps(S6 + x + 4)));
            v_dst1 = _mm_add_ps(v_dst1, _mm_mul_ps(v_b7, _mm_loadu_ps(S7 + x + 4)));

            __m128i v_dsti0 = _mm_cvtps_epi32(v_dst0);
            __m128i v_dsti1 = _mm_cvtps_epi32(v_dst1);

            _mm_storeu_si128((__m128i *)(dst + x), _mm_packus_epi32(v_dsti0, v_dsti1));
        }

        return x;
    }
};

#else

typedef VResizeNoVec VResizeLanczos4Vec_32f16u;

#endif

struct VResizeLanczos4Vec_32f16s
{
    int operator()(const uchar** src, uchar* dst, const uchar* _beta, int width ) const
    {
        const float** src = (const float**)src;
        const float* beta = (const float*)_beta;
        const float *S0 = src[0], *S1 = src[1], *S2 = src[2], *S3 = src[3],
                    *S4 = src[4], *S5 = src[5], *S6 = src[6], *S7 = src[7];
        short * dst = (short*)dst;
        int x = 0;
        __m128 v_b0 = _mm_set1_ps(beta[0]), v_b1 = _mm_set1_ps(beta[1]),
               v_b2 = _mm_set1_ps(beta[2]), v_b3 = _mm_set1_ps(beta[3]),
               v_b4 = _mm_set1_ps(beta[4]), v_b5 = _mm_set1_ps(beta[5]),
               v_b6 = _mm_set1_ps(beta[6]), v_b7 = _mm_set1_ps(beta[7]);

        for( ; x <= width - 8; x += 8 )
        {
            __m128 v_dst0 = _mm_mul_ps(v_b0, _mm_loadu_ps(S0 + x));
            v_dst0 = _mm_add_ps(v_dst0, _mm_mul_ps(v_b1, _mm_loadu_ps(S1 + x)));
            v_dst0 = _mm_add_ps(v_dst0, _mm_mul_ps(v_b2, _mm_loadu_ps(S2 + x)));
            v_dst0 = _mm_add_ps(v_dst0, _mm_mul_ps(v_b3, _mm_loadu_ps(S3 + x)));
            v_dst0 = _mm_add_ps(v_dst0, _mm_mul_ps(v_b4, _mm_loadu_ps(S4 + x)));
            v_dst0 = _mm_add_ps(v_dst0, _mm_mul_ps(v_b5, _mm_loadu_ps(S5 + x)));
            v_dst0 = _mm_add_ps(v_dst0, _mm_mul_ps(v_b6, _mm_loadu_ps(S6 + x)));
            v_dst0 = _mm_add_ps(v_dst0, _mm_mul_ps(v_b7, _mm_loadu_ps(S7 + x)));

            __m128 v_dst1 = _mm_mul_ps(v_b0, _mm_loadu_ps(S0 + x + 4));
            v_dst1 = _mm_add_ps(v_dst1, _mm_mul_ps(v_b1, _mm_loadu_ps(S1 + x + 4)));
            v_dst1 = _mm_add_ps(v_dst1, _mm_mul_ps(v_b2, _mm_loadu_ps(S2 + x + 4)));
            v_dst1 = _mm_add_ps(v_dst1, _mm_mul_ps(v_b3, _mm_loadu_ps(S3 + x + 4)));
            v_dst1 = _mm_add_ps(v_dst1, _mm_mul_ps(v_b4, _mm_loadu_ps(S4 + x + 4)));
            v_dst1 = _mm_add_ps(v_dst1, _mm_mul_ps(v_b5, _mm_loadu_ps(S5 + x + 4)));
            v_dst1 = _mm_add_ps(v_dst1, _mm_mul_ps(v_b6, _mm_loadu_ps(S6 + x + 4)));
            v_dst1 = _mm_add_ps(v_dst1, _mm_mul_ps(v_b7, _mm_loadu_ps(S7 + x + 4)));

            __m128i v_dsti0 = _mm_cvtps_epi32(v_dst0);
            __m128i v_dsti1 = _mm_cvtps_epi32(v_dst1);

            _mm_storeu_si128((__m128i *)(dst + x), _mm_packs_epi32(v_dsti0, v_dsti1));
        }

        return x;
    }
};


struct VResizeLanczos4Vec_32f
{
    int operator()(const uchar** src, uchar* dst, const uchar* _beta, int width ) const
    {
        const float** src = (const float**)src;
        const float* beta = (const float*)_beta;
        const float *S0 = src[0], *S1 = src[1], *S2 = src[2], *S3 = src[3],
                    *S4 = src[4], *S5 = src[5], *S6 = src[6], *S7 = src[7];
        float* dst = (float*)dst;
        int x = 0;

        __m128 v_b0 = _mm_set1_ps(beta[0]), v_b1 = _mm_set1_ps(beta[1]),
               v_b2 = _mm_set1_ps(beta[2]), v_b3 = _mm_set1_ps(beta[3]),
               v_b4 = _mm_set1_ps(beta[4]), v_b5 = _mm_set1_ps(beta[5]),
               v_b6 = _mm_set1_ps(beta[6]), v_b7 = _mm_set1_ps(beta[7]);

        for( ; x <= width - 4; x += 4 )
        {
            __m128 v_dst = _mm_mul_ps(v_b0, _mm_loadu_ps(S0 + x));
            v_dst = _mm_add_ps(v_dst, _mm_mul_ps(v_b1, _mm_loadu_ps(S1 + x)));
            v_dst = _mm_add_ps(v_dst, _mm_mul_ps(v_b2, _mm_loadu_ps(S2 + x)));
            v_dst = _mm_add_ps(v_dst, _mm_mul_ps(v_b3, _mm_loadu_ps(S3 + x)));
            v_dst = _mm_add_ps(v_dst, _mm_mul_ps(v_b4, _mm_loadu_ps(S4 + x)));
            v_dst = _mm_add_ps(v_dst, _mm_mul_ps(v_b5, _mm_loadu_ps(S5 + x)));
            v_dst = _mm_add_ps(v_dst, _mm_mul_ps(v_b6, _mm_loadu_ps(S6 + x)));
            v_dst = _mm_add_ps(v_dst, _mm_mul_ps(v_b7, _mm_loadu_ps(S7 + x)));

            _mm_storeu_ps(dst + x, v_dst);
        }

        return x;
    }
};


#elif CC_NEON

struct VResizeLinearVec_32s8u
{
    int operator()(const uchar** src, uchar* dst, const uchar* _beta, int width ) const
    {
        const int** src = (const int**)src, *S0 = src[0], *S1 = src[1];
        const short* beta = (const short*)_beta;
        int x = 0;
        int16x8_t v_b0 = vdupq_n_s16(beta[0]), v_b1 = vdupq_n_s16(beta[1]), v_delta = vdupq_n_s16(2);

        for( ; x <= width - 16; x += 16)
        {
            int32x4_t v_src00 = vshrq_n_s32(vld1q_s32(S0 + x), 4), v_src10 = vshrq_n_s32(vld1q_s32(S1 + x), 4);
            int32x4_t v_src01 = vshrq_n_s32(vld1q_s32(S0 + x + 4), 4), v_src11 = vshrq_n_s32(vld1q_s32(S1 + x + 4), 4);

            int16x8_t v_src0 = vcombine_s16(vmovn_s32(v_src00), vmovn_s32(v_src01));
            int16x8_t v_src1 = vcombine_s16(vmovn_s32(v_src10), vmovn_s32(v_src11));

            int16x8_t v_dst0 = vaddq_s16(vshrq_n_s16(vqdmulhq_s16(v_src0, v_b0), 1),
                                         vshrq_n_s16(vqdmulhq_s16(v_src1, v_b1), 1));
            v_dst0 = vshrq_n_s16(vaddq_s16(v_dst0, v_delta), 2);

            v_src00 = vshrq_n_s32(vld1q_s32(S0 + x + 8), 4);
            v_src10 = vshrq_n_s32(vld1q_s32(S1 + x + 8), 4);
            v_src01 = vshrq_n_s32(vld1q_s32(S0 + x + 12), 4);
            v_src11 = vshrq_n_s32(vld1q_s32(S1 + x + 12), 4);

            v_src0 = vcombine_s16(vmovn_s32(v_src00), vmovn_s32(v_src01));
            v_src1 = vcombine_s16(vmovn_s32(v_src10), vmovn_s32(v_src11));

            int16x8_t v_dst1 = vaddq_s16(vshrq_n_s16(vqdmulhq_s16(v_src0, v_b0), 1),
                                         vshrq_n_s16(vqdmulhq_s16(v_src1, v_b1), 1));
            v_dst1 = vshrq_n_s16(vaddq_s16(v_dst1, v_delta), 2);

            vst1q_u8(dst + x, vcombine_u8(vqmovun_s16(v_dst0), vqmovun_s16(v_dst1)));
        }

        return x;
    }
};

struct VResizeLinearVec_32f16u
{
    int operator()(const uchar** src, uchar* dst, const uchar* _beta, int width ) const
    {
        const float** src = (const float**)src;
        const float* beta = (const float*)_beta;
        const float *S0 = src[0], *S1 = src[1];
        ushort* dst = (ushort*)dst;
        int x = 0;

        float32x4_t v_b0 = vdupq_n_f32(beta[0]), v_b1 = vdupq_n_f32(beta[1]);

        for( ; x <= width - 8; x += 8 )
        {
            float32x4_t v_src00 = vld1q_f32(S0 + x), v_src01 = vld1q_f32(S0 + x + 4);
            float32x4_t v_src10 = vld1q_f32(S1 + x), v_src11 = vld1q_f32(S1 + x + 4);

            float32x4_t v_dst0 = vmlaq_f32(vmulq_f32(v_src00, v_b0), v_src10, v_b1);
            float32x4_t v_dst1 = vmlaq_f32(vmulq_f32(v_src01, v_b0), v_src11, v_b1);

            vst1q_u16(dst + x, vcombine_u16(vqmovn_u32(cv_vrndq_u32_f32(v_dst0)),
                                            vqmovn_u32(cv_vrndq_u32_f32(v_dst1))));
        }

        return x;
    }
};

struct VResizeLinearVec_32f16s
{
    int operator()(const uchar** src, uchar* dst, const uchar* _beta, int width ) const
    {
        const float** src = (const float**)src;
        const float* beta = (const float*)_beta;
        const float *S0 = src[0], *S1 = src[1];
        short* dst = (short*)dst;
        int x = 0;

        float32x4_t v_b0 = vdupq_n_f32(beta[0]), v_b1 = vdupq_n_f32(beta[1]);

        for( ; x <= width - 8; x += 8 )
        {
            float32x4_t v_src00 = vld1q_f32(S0 + x), v_src01 = vld1q_f32(S0 + x + 4);
            float32x4_t v_src10 = vld1q_f32(S1 + x), v_src11 = vld1q_f32(S1 + x + 4);

            float32x4_t v_dst0 = vmlaq_f32(vmulq_f32(v_src00, v_b0), v_src10, v_b1);
            float32x4_t v_dst1 = vmlaq_f32(vmulq_f32(v_src01, v_b0), v_src11, v_b1);

            vst1q_s16(dst + x, vcombine_s16(vqmovn_s32(cv_vrndq_s32_f32(v_dst0)),
                                            vqmovn_s32(cv_vrndq_s32_f32(v_dst1))));
        }

        return x;
    }
};

struct VResizeLinearVec_32f
{
    int operator()(const uchar** src, uchar* dst, const uchar* _beta, int width ) const
    {
        const float** src = (const float**)src;
        const float* beta = (const float*)_beta;
        const float *S0 = src[0], *S1 = src[1];
        float* dst = (float*)dst;
        int x = 0;

        float32x4_t v_b0 = vdupq_n_f32(beta[0]), v_b1 = vdupq_n_f32(beta[1]);

        for( ; x <= width - 8; x += 8 )
        {
            float32x4_t v_src00 = vld1q_f32(S0 + x), v_src01 = vld1q_f32(S0 + x + 4);
            float32x4_t v_src10 = vld1q_f32(S1 + x), v_src11 = vld1q_f32(S1 + x + 4);

            vst1q_f32(dst + x, vmlaq_f32(vmulq_f32(v_src00, v_b0), v_src10, v_b1));
            vst1q_f32(dst + x + 4, vmlaq_f32(vmulq_f32(v_src01, v_b0), v_src11, v_b1));
        }

        return x;
    }
};

typedef VResizeNoVec VResizeCubicVec_32s8u;

struct VResizeCubicVec_32f16u
{
    int operator()(const uchar** src, uchar* dst, const uchar* _beta, int width ) const
    {
        const float** src = (const float**)src;
        const float* beta = (const float*)_beta;
        const float *S0 = src[0], *S1 = src[1], *S2 = src[2], *S3 = src[3];
        ushort* dst = (ushort*)dst;
        int x = 0;
        float32x4_t v_b0 = vdupq_n_f32(beta[0]), v_b1 = vdupq_n_f32(beta[1]),
                    v_b2 = vdupq_n_f32(beta[2]), v_b3 = vdupq_n_f32(beta[3]);

        for( ; x <= width - 8; x += 8 )
        {
            float32x4_t v_dst0 = vmlaq_f32(vmlaq_f32(vmlaq_f32(vmulq_f32(v_b0, vld1q_f32(S0 + x)),
                                                                         v_b1, vld1q_f32(S1 + x)),
                                                                         v_b2, vld1q_f32(S2 + x)),
                                                                         v_b3, vld1q_f32(S3 + x));
            float32x4_t v_dst1 = vmlaq_f32(vmlaq_f32(vmlaq_f32(vmulq_f32(v_b0, vld1q_f32(S0 + x + 4)),
                                                                         v_b1, vld1q_f32(S1 + x + 4)),
                                                                         v_b2, vld1q_f32(S2 + x + 4)),
                                                                         v_b3, vld1q_f32(S3 + x + 4));

            vst1q_u16(dst + x, vcombine_u16(vqmovn_u32(cv_vrndq_u32_f32(v_dst0)),
                                            vqmovn_u32(cv_vrndq_u32_f32(v_dst1))));
        }

        return x;
    }
};

struct VResizeCubicVec_32f16s
{
    int operator()(const uchar** src, uchar* dst, const uchar* _beta, int width ) const
    {
        const float** src = (const float**)src;
        const float* beta = (const float*)_beta;
        const float *S0 = src[0], *S1 = src[1], *S2 = src[2], *S3 = src[3];
        short* dst = (short*)dst;
        int x = 0;
        float32x4_t v_b0 = vdupq_n_f32(beta[0]), v_b1 = vdupq_n_f32(beta[1]),
                    v_b2 = vdupq_n_f32(beta[2]), v_b3 = vdupq_n_f32(beta[3]);

        for( ; x <= width - 8; x += 8 )
        {
            float32x4_t v_dst0 = vmlaq_f32(vmlaq_f32(vmlaq_f32(vmulq_f32(v_b0, vld1q_f32(S0 + x)),
                                                                         v_b1, vld1q_f32(S1 + x)),
                                                                         v_b2, vld1q_f32(S2 + x)),
                                                                         v_b3, vld1q_f32(S3 + x));
            float32x4_t v_dst1 = vmlaq_f32(vmlaq_f32(vmlaq_f32(vmulq_f32(v_b0, vld1q_f32(S0 + x + 4)),
                                                                         v_b1, vld1q_f32(S1 + x + 4)),
                                                                         v_b2, vld1q_f32(S2 + x + 4)),
                                                                         v_b3, vld1q_f32(S3 + x + 4));

            vst1q_s16(dst + x, vcombine_s16(vqmovn_s32(cv_vrndq_s32_f32(v_dst0)),
                                            vqmovn_s32(cv_vrndq_s32_f32(v_dst1))));
        }

        return x;
    }
};

struct VResizeCubicVec_32f
{
    int operator()(const uchar** src, uchar* dst, const uchar* _beta, int width ) const
    {
        const float** src = (const float**)src;
        const float* beta = (const float*)_beta;
        const float *S0 = src[0], *S1 = src[1], *S2 = src[2], *S3 = src[3];
        float* dst = (float*)dst;
        int x = 0;
        float32x4_t v_b0 = vdupq_n_f32(beta[0]), v_b1 = vdupq_n_f32(beta[1]),
                    v_b2 = vdupq_n_f32(beta[2]), v_b3 = vdupq_n_f32(beta[3]);

        for( ; x <= width - 8; x += 8 )
        {
            vst1q_f32(dst + x, vmlaq_f32(vmlaq_f32(vmlaq_f32(vmulq_f32(v_b0, vld1q_f32(S0 + x)),
                                                                       v_b1, vld1q_f32(S1 + x)),
                                                                       v_b2, vld1q_f32(S2 + x)),
                                                                       v_b3, vld1q_f32(S3 + x)));
            vst1q_f32(dst + x + 4, vmlaq_f32(vmlaq_f32(vmlaq_f32(vmulq_f32(v_b0, vld1q_f32(S0 + x + 4)),
                                                                          v_b1, vld1q_f32(S1 + x + 4)),
                                                                          v_b2, vld1q_f32(S2 + x + 4)),
                                                                          v_b3, vld1q_f32(S3 + x + 4)));
        }

        return x;
    }
};

struct VResizeLanczos4Vec_32f16u
{
    int operator()(const uchar** src, uchar* dst, const uchar* _beta, int width ) const
    {
        const float** src = (const float**)src;
        const float* beta = (const float*)_beta;
        const float *S0 = src[0], *S1 = src[1], *S2 = src[2], *S3 = src[3],
                    *S4 = src[4], *S5 = src[5], *S6 = src[6], *S7 = src[7];
        ushort * dst = (ushort*)dst;
        int x = 0;
        float32x4_t v_b0 = vdupq_n_f32(beta[0]), v_b1 = vdupq_n_f32(beta[1]),
                    v_b2 = vdupq_n_f32(beta[2]), v_b3 = vdupq_n_f32(beta[3]),
                    v_b4 = vdupq_n_f32(beta[4]), v_b5 = vdupq_n_f32(beta[5]),
                    v_b6 = vdupq_n_f32(beta[6]), v_b7 = vdupq_n_f32(beta[7]);

        for( ; x <= width - 8; x += 8 )
        {
            float32x4_t v_dst0 = vmlaq_f32(vmlaq_f32(vmlaq_f32(vmulq_f32(v_b0, vld1q_f32(S0 + x)),
                                                                         v_b1, vld1q_f32(S1 + x)),
                                                                         v_b2, vld1q_f32(S2 + x)),
                                                                         v_b3, vld1q_f32(S3 + x));
            float32x4_t v_dst1 = vmlaq_f32(vmlaq_f32(vmlaq_f32(vmulq_f32(v_b4, vld1q_f32(S4 + x)),
                                                                         v_b5, vld1q_f32(S5 + x)),
                                                                         v_b6, vld1q_f32(S6 + x)),
                                                                         v_b7, vld1q_f32(S7 + x));
            float32x4_t v_dst = vaddq_f32(v_dst0, v_dst1);

            v_dst0 = vmlaq_f32(vmlaq_f32(vmlaq_f32(vmulq_f32(v_b0, vld1q_f32(S0 + x + 4)),
                                                             v_b1, vld1q_f32(S1 + x + 4)),
                                                             v_b2, vld1q_f32(S2 + x + 4)),
                                                             v_b3, vld1q_f32(S3 + x + 4));
            v_dst1 = vmlaq_f32(vmlaq_f32(vmlaq_f32(vmulq_f32(v_b4, vld1q_f32(S4 + x + 4)),
                                                             v_b5, vld1q_f32(S5 + x + 4)),
                                                             v_b6, vld1q_f32(S6 + x + 4)),
                                                             v_b7, vld1q_f32(S7 + x + 4));
            v_dst1 = vaddq_f32(v_dst0, v_dst1);

            vst1q_u16(dst + x, vcombine_u16(vqmovn_u32(cv_vrndq_u32_f32(v_dst)),
                                            vqmovn_u32(cv_vrndq_u32_f32(v_dst1))));
        }

        return x;
    }
};

struct VResizeLanczos4Vec_32f16s
{
    int operator()(const uchar** src, uchar* dst, const uchar* _beta, int width ) const
    {
        const float** src = (const float**)src;
        const float* beta = (const float*)_beta;
        const float *S0 = src[0], *S1 = src[1], *S2 = src[2], *S3 = src[3],
                    *S4 = src[4], *S5 = src[5], *S6 = src[6], *S7 = src[7];
        short * dst = (short*)dst;
        int x = 0;
        float32x4_t v_b0 = vdupq_n_f32(beta[0]), v_b1 = vdupq_n_f32(beta[1]),
                    v_b2 = vdupq_n_f32(beta[2]), v_b3 = vdupq_n_f32(beta[3]),
                    v_b4 = vdupq_n_f32(beta[4]), v_b5 = vdupq_n_f32(beta[5]),
                    v_b6 = vdupq_n_f32(beta[6]), v_b7 = vdupq_n_f32(beta[7]);

        for( ; x <= width - 8; x += 8 )
        {
            float32x4_t v_dst0 = vmlaq_f32(vmlaq_f32(vmlaq_f32(vmulq_f32(v_b0, vld1q_f32(S0 + x)),
                                                                         v_b1, vld1q_f32(S1 + x)),
                                                                         v_b2, vld1q_f32(S2 + x)),
                                                                         v_b3, vld1q_f32(S3 + x));
            float32x4_t v_dst1 = vmlaq_f32(vmlaq_f32(vmlaq_f32(vmulq_f32(v_b4, vld1q_f32(S4 + x)),
                                                                         v_b5, vld1q_f32(S5 + x)),
                                                                         v_b6, vld1q_f32(S6 + x)),
                                                                         v_b7, vld1q_f32(S7 + x));
            float32x4_t v_dst = vaddq_f32(v_dst0, v_dst1);

            v_dst0 = vmlaq_f32(vmlaq_f32(vmlaq_f32(vmulq_f32(v_b0, vld1q_f32(S0 + x + 4)),
                                                             v_b1, vld1q_f32(S1 + x + 4)),
                                                             v_b2, vld1q_f32(S2 + x + 4)),
                                                             v_b3, vld1q_f32(S3 + x + 4));
            v_dst1 = vmlaq_f32(vmlaq_f32(vmlaq_f32(vmulq_f32(v_b4, vld1q_f32(S4 + x + 4)),
                                                             v_b5, vld1q_f32(S5 + x + 4)),
                                                             v_b6, vld1q_f32(S6 + x + 4)),
                                                             v_b7, vld1q_f32(S7 + x + 4));
            v_dst1 = vaddq_f32(v_dst0, v_dst1);

            vst1q_s16(dst + x, vcombine_s16(vqmovn_s32(cv_vrndq_s32_f32(v_dst)),
                                            vqmovn_s32(cv_vrndq_s32_f32(v_dst1))));
        }

        return x;
    }
};

struct VResizeLanczos4Vec_32f
{
    int operator()(const uchar** src, uchar* dst, const uchar* _beta, int width ) const
    {
        const float** src = (const float**)src;
        const float* beta = (const float*)_beta;
        const float *S0 = src[0], *S1 = src[1], *S2 = src[2], *S3 = src[3],
                    *S4 = src[4], *S5 = src[5], *S6 = src[6], *S7 = src[7];
        float* dst = (float*)dst;
        int x = 0;
        float32x4_t v_b0 = vdupq_n_f32(beta[0]), v_b1 = vdupq_n_f32(beta[1]),
                    v_b2 = vdupq_n_f32(beta[2]), v_b3 = vdupq_n_f32(beta[3]),
                    v_b4 = vdupq_n_f32(beta[4]), v_b5 = vdupq_n_f32(beta[5]),
                    v_b6 = vdupq_n_f32(beta[6]), v_b7 = vdupq_n_f32(beta[7]);

        for( ; x <= width - 4; x += 4 )
        {
            float32x4_t v_dst0 = vmlaq_f32(vmlaq_f32(vmlaq_f32(vmulq_f32(v_b0, vld1q_f32(S0 + x)),
                                                                         v_b1, vld1q_f32(S1 + x)),
                                                                         v_b2, vld1q_f32(S2 + x)),
                                                                         v_b3, vld1q_f32(S3 + x));
            float32x4_t v_dst1 = vmlaq_f32(vmlaq_f32(vmlaq_f32(vmulq_f32(v_b4, vld1q_f32(S4 + x)),
                                                                         v_b5, vld1q_f32(S5 + x)),
                                                                         v_b6, vld1q_f32(S6 + x)),
                                                                         v_b7, vld1q_f32(S7 + x));
            vst1q_f32(dst + x, vaddq_f32(v_dst0, v_dst1));
        }

        return x;
    }
};

#else

typedef VResizeNoVec VResizeLinearVec_32s8u;
typedef VResizeNoVec VResizeLinearVec_32f16u;
typedef VResizeNoVec VResizeLinearVec_32f16s;
typedef VResizeNoVec VResizeLinearVec_32f;

typedef VResizeNoVec VResizeCubicVec_32s8u;
typedef VResizeNoVec VResizeCubicVec_32f16u;
typedef VResizeNoVec VResizeCubicVec_32f16s;
typedef VResizeNoVec VResizeCubicVec_32f;

typedef VResizeNoVec VResizeLanczos4Vec_32f16u;
typedef VResizeNoVec VResizeLanczos4Vec_32f16s;
typedef VResizeNoVec VResizeLanczos4Vec_32f;

#endif

typedef HResizeNoVec HResizeLinearVec_8u32s;
typedef HResizeNoVec HResizeLinearVec_16u32f;
typedef HResizeNoVec HResizeLinearVec_16s32f;
typedef HResizeNoVec HResizeLinearVec_32f;
typedef HResizeNoVec HResizeLinearVec_64f;


template<typename T, typename WT, typename AT, int ONE, class VecOp>
struct HResizeLinear
{
    typedef T value_type;
    typedef WT buf_type;
    typedef AT alpha_type;

    void operator()(const T** src, WT** dst, int count,
                    const int* xofs, const AT* alpha,
                    int swidth, int dwidth, int cn, int xmin, int xmax ) const
    {
        int dx, k;
        VecOp vecOp;

        int dx0 = vecOp((const uchar**)src, (uchar**)dst, count,
            xofs, (const uchar*)alpha, swidth, dwidth, cn, xmin, xmax );

        for( k = 0; k <= count - 2; k++ )
        {
            const T *S0 = src[k], *S1 = src[k+1];
            WT *D0 = dst[k], *D1 = dst[k+1];
            for( dx = dx0; dx < xmax; dx++ )
            {
                int sx = xofs[dx];
                WT a0 = alpha[dx*2], a1 = alpha[dx*2+1];
                WT t0 = S0[sx]*a0 + S0[sx + cn]*a1;
                WT t1 = S1[sx]*a0 + S1[sx + cn]*a1;
                D0[dx] = t0; D1[dx] = t1;
            }

            for( ; dx < dwidth; dx++ )
            {
                int sx = xofs[dx];
                D0[dx] = WT(S0[sx]*ONE); D1[dx] = WT(S1[sx]*ONE);
            }
        }

        for( ; k < count; k++ )
        {
            const T *S = src[k];
            WT *D = dst[k];
            for( dx = 0; dx < xmax; dx++ )
            {
                int sx = xofs[dx];
                D[dx] = S[sx]*alpha[dx*2] + S[sx+cn]*alpha[dx*2+1];
            }

            for( ; dx < dwidth; dx++ )
                D[dx] = WT(S[xofs[dx]]*ONE);
        }
    }
};


template<typename T, typename WT, typename AT, class CastOp, class VecOp>
struct VResizeLinear
{
    typedef T value_type;
    typedef WT buf_type;
    typedef AT alpha_type;

    void operator()(const WT** src, T* dst, const AT* beta, int width ) const
    {
        WT b0 = beta[0], b1 = beta[1];
        const WT *S0 = src[0], *S1 = src[1];
        CastOp castOp;
        VecOp vecOp;

        int x = vecOp((const uchar**)src, (uchar*)dst, (const uchar*)beta, width);
        #if CC_ENABLE_UNROLLED
        for( ; x <= width - 4; x += 4 )
        {
            WT t0, t1;
            t0 = S0[x]*b0 + S1[x]*b1;
            t1 = S0[x+1]*b0 + S1[x+1]*b1;
            dst[x] = castOp(t0); dst[x+1] = castOp(t1);
            t0 = S0[x+2]*b0 + S1[x+2]*b1;
            t1 = S0[x+3]*b0 + S1[x+3]*b1;
            dst[x+2] = castOp(t0); dst[x+3] = castOp(t1);
        }
        #endif
        for( ; x < width; x++ )
            dst[x] = castOp(S0[x]*b0 + S1[x]*b1);
    }
};

template<>
struct VResizeLinear<uchar, int, short, FixedPtCast<int, uchar, CC_INTER_RESIZE_COEF_BITS*2>, VResizeLinearVec_32s8u>
{
    typedef uchar value_type;
    typedef int buf_type;
    typedef short alpha_type;

    void operator()(const buf_type** src, value_type* dst, const alpha_type* beta, int width ) const
    {
        alpha_type b0 = beta[0], b1 = beta[1];
        const buf_type *S0 = src[0], *S1 = src[1];
        VResizeLinearVec_32s8u vecOp;

        int x = vecOp((const uchar**)src, (uchar*)dst, (const uchar*)beta, width);
        #if CC_ENABLE_UNROLLED
        for( ; x <= width - 4; x += 4 )
        {
            dst[x+0] = uchar(( ((b0 * (S0[x+0] >> 4)) >> 16) + ((b1 * (S1[x+0] >> 4)) >> 16) + 2)>>2);
            dst[x+1] = uchar(( ((b0 * (S0[x+1] >> 4)) >> 16) + ((b1 * (S1[x+1] >> 4)) >> 16) + 2)>>2);
            dst[x+2] = uchar(( ((b0 * (S0[x+2] >> 4)) >> 16) + ((b1 * (S1[x+2] >> 4)) >> 16) + 2)>>2);
            dst[x+3] = uchar(( ((b0 * (S0[x+3] >> 4)) >> 16) + ((b1 * (S1[x+3] >> 4)) >> 16) + 2)>>2);
        }
        #endif
        for( ; x < width; x++ )
            dst[x] = uchar(( ((b0 * (S0[x] >> 4)) >> 16) + ((b1 * (S1[x] >> 4)) >> 16) + 2)>>2);
    }
};


template<typename T, typename WT, typename AT>
struct HResizeCubic
{
    typedef T value_type;
    typedef WT buf_type;
    typedef AT alpha_type;

    void operator()(const T** src, WT** dst, int count,
                    const int* xofs, const AT* alpha,
                    int swidth, int dwidth, int cn, int xmin, int xmax ) const
    {
        for( int k = 0; k < count; k++ )
        {
            const T *S = src[k];
            WT *D = dst[k];
            int dx = 0, limit = xmin;
            for(;;)
            {
                for( ; dx < limit; dx++, alpha += 4 )
                {
                    int j, sx = xofs[dx] - cn;
                    WT v = 0;
                    for( j = 0; j < 4; j++ )
                    {
                        int sxj = sx + j*cn;
                        if( (unsigned)sxj >= (unsigned)swidth )
                        {
                            while( sxj < 0 )
                                sxj += cn;
                            while( sxj >= swidth )
                                sxj -= cn;
                        }
                        v += S[sxj]*alpha[j];
                    }
                    D[dx] = v;
                }
                if( limit == dwidth )
                    break;
                for( ; dx < xmax; dx++, alpha += 4 )
                {
                    int sx = xofs[dx];
                    D[dx] = S[sx-cn]*alpha[0] + S[sx]*alpha[1] +
                        S[sx+cn]*alpha[2] + S[sx+cn*2]*alpha[3];
                }
                limit = dwidth;
            }
            alpha -= dwidth*4;
        }
    }
};


template<typename T, typename WT, typename AT, class CastOp, class VecOp>
struct VResizeCubic
{
    typedef T value_type;
    typedef WT buf_type;
    typedef AT alpha_type;

    void operator()(const WT** src, T* dst, const AT* beta, int width ) const
    {
        WT b0 = beta[0], b1 = beta[1], b2 = beta[2], b3 = beta[3];
        const WT *S0 = src[0], *S1 = src[1], *S2 = src[2], *S3 = src[3];
        CastOp castOp;
        VecOp vecOp;

        int x = vecOp((const uchar**)src, (uchar*)dst, (const uchar*)beta, width);
        for( ; x < width; x++ )
            dst[x] = castOp(S0[x]*b0 + S1[x]*b1 + S2[x]*b2 + S3[x]*b3);
    }
};


template<typename T, typename WT, typename AT>
struct HResizeLanczos4
{
    typedef T value_type;
    typedef WT buf_type;
    typedef AT alpha_type;

    void operator()(const T** src, WT** dst, int count,
                    const int* xofs, const AT* alpha,
                    int swidth, int dwidth, int cn, int xmin, int xmax ) const
    {
        for( int k = 0; k < count; k++ )
        {
            const T *S = src[k];
            WT *D = dst[k];
            int dx = 0, limit = xmin;
            for(;;)
            {
                for( ; dx < limit; dx++, alpha += 8 )
                {
                    int j, sx = xofs[dx] - cn*3;
                    WT v = 0;
                    for( j = 0; j < 8; j++ )
                    {
                        int sxj = sx + j*cn;
                        if( (unsigned)sxj >= (unsigned)swidth )
                        {
                            while( sxj < 0 )
                                sxj += cn;
                            while( sxj >= swidth )
                                sxj -= cn;
                        }
                        v += S[sxj]*alpha[j];
                    }
                    D[dx] = v;
                }
                if( limit == dwidth )
                    break;
                for( ; dx < xmax; dx++, alpha += 8 )
                {
                    int sx = xofs[dx];
                    D[dx] = S[sx-cn*3]*alpha[0] + S[sx-cn*2]*alpha[1] +
                        S[sx-cn]*alpha[2] + S[sx]*alpha[3] +
                        S[sx+cn]*alpha[4] + S[sx+cn*2]*alpha[5] +
                        S[sx+cn*3]*alpha[6] + S[sx+cn*4]*alpha[7];
                }
                limit = dwidth;
            }
            alpha -= dwidth*8;
        }
    }
};


template<typename T, typename WT, typename AT, class CastOp, class VecOp>
struct VResizeLanczos4
{
    typedef T value_type;
    typedef WT buf_type;
    typedef AT alpha_type;

    void operator()(const WT** src, T* dst, const AT* beta, int width ) const
    {
        CastOp castOp;
        VecOp vecOp;
        int k, x = vecOp((const uchar**)src, (uchar*)dst, (const uchar*)beta, width);
        #if CC_ENABLE_UNROLLED
        for( ; x <= width - 4; x += 4 )
        {
            WT b = beta[0];
            const WT* S = src[0];
            WT s0 = S[x]*b, s1 = S[x+1]*b, s2 = S[x+2]*b, s3 = S[x+3]*b;

            for( k = 1; k < 8; k++ )
            {
                b = beta[k]; S = src[k];
                s0 += S[x]*b; s1 += S[x+1]*b;
                s2 += S[x+2]*b; s3 += S[x+3]*b;
            }

            dst[x] = castOp(s0); dst[x+1] = castOp(s1);
            dst[x+2] = castOp(s2); dst[x+3] = castOp(s3);
        }
        #endif
        for( ; x < width; x++ )
        {
            dst[x] = castOp(src[0][x]*beta[0] + src[1][x]*beta[1] +
                src[2][x]*beta[2] + src[3][x]*beta[3] + src[4][x]*beta[4] +
                src[5][x]*beta[5] + src[6][x]*beta[6] + src[7][x]*beta[7]);
        }
    }
};


static inline int clip(int x, int a, int b)
{
    return x >= a ? (x < b ? x : b-1) : a;
}

static const int MAX_ESIZE=16;

template <typename HResize, typename VResize>
class resizeGeneric_Invoker :
    public CParallelLoopBody
{
public:
    typedef typename HResize::value_type T;
    typedef typename HResize::buf_type WT;
    typedef typename HResize::alpha_type AT;

    resizeGeneric_Invoker(const img_t* src, img_t* dst, const int *_xofs, const int *_yofs,
        const AT* _alpha, const AT* __beta, const CSize& _ssize, const CSize &_dsize,
        int _ksize, int _xmin, int _xmax) :
        CParallelLoopBody(), src(src), dst(dst), xofs(_xofs), yofs(_yofs),
        alpha(_alpha), _beta(__beta), ssize(_ssize), dsize(_dsize),
        ksize(_ksize), xmin(_xmin), xmax(_xmax)
    {
        CC_Assert(ksize <= MAX_ESIZE);
    }

#if defined(__GNUC__) && (__GNUC__ == 4) && (__GNUC_MINOR__ == 8)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Warray-bounds"
#endif
    virtual void operator() (const CRange& range) const
    {
        int dy, cn = CC_MAT_CN(src);
        HResize hresize;
        VResize vresize;

        int bufstep = (int)cAlignSize(dsize.width, 16);
        WT* _buffer = MALLOC(WT, bufstep*ksize);
        const T* srows[MAX_ESIZE]={0};
        WT* rows[MAX_ESIZE]={0};
        int prev_sy[MAX_ESIZE];

        for(int k = 0; k < ksize; k++ )
        {
            prev_sy[k] = -1;
            rows[k] = (WT*)_buffer + bufstep*k;
        }

        const AT* beta = _beta + ksize * range.start;

        for( dy = range.start; dy < range.end; dy++, beta += ksize )
        {
            int sy0 = yofs[dy], k0=ksize, k1=0, ksize2 = ksize/2;

            for(int k = 0; k < ksize; k++ )
            {
                int sy = clip(sy0 - ksize2 + 1 + k, 0, ssize.height);
                for( k1 = MAX(k1, k); k1 < ksize; k1++ )
                {
                    if( sy == prev_sy[k1] ) // if the sy-th row has been computed already, reuse it.
                    {
                        if( k1 > k )
                            memcpy( rows[k], rows[k1], bufstep*sizeof(rows[0][0]) );
                        break;
                    }
                }
                if( k1 == ksize )
                    k0 = MIN(k0, k); // remember the first row that needs to be computed
                srows[k] = img_row_(T, src, sy);
                prev_sy[k] = sy;
            }

            if( k0 < ksize )
                hresize( (const T**)(srows + k0), (WT**)(rows + k0), ksize - k0, xofs, (const AT*)(alpha),
                        ssize.width, dsize.width, cn, xmin, xmax );
            vresize( (const WT**)rows, (T*)(dst->tt.data + dst->step*dy), beta, dsize.width );
        }
        FREE(_buffer);
    }
#if defined(__GNUC__) && (__GNUC__ == 4) && (__GNUC_MINOR__ == 8)
# pragma GCC diagnostic pop
#endif

private:
    const img_t *src;
    img_t *dst;
    const int* xofs, *yofs;
    const AT* alpha, *_beta;
    CSize ssize, dsize;
    const int ksize, xmin, xmax;

    resizeGeneric_Invoker& operator = (const resizeGeneric_Invoker&);
};

template<class HResize, class VResize>
struct resizeGeneric_ {
static void f( const img_t* src, img_t* dst,
                            const int* xofs, const void* _alpha,
                            const int* yofs, const void* _beta,
                            int xmin, int xmax, int ksize )
{
    typedef typename HResize::alpha_type AT;

    const AT* beta = (const AT*)_beta;
    CSize ssize = cvGetSize(src), dsize = cvGetSize(dst);
    int cn = CC_MAT_CN(src);
    ssize.width *= cn;
    dsize.width *= cn;
    xmin *= cn;
    xmax *= cn;
    // image resize is a separable operation. In case of not too strong

    CRange range = {0, dsize.height};
    resizeGeneric_Invoker<HResize, VResize> invoker(src, dst, xofs, yofs, (const AT*)_alpha, beta,
        ssize, dsize, ksize, xmin, xmax);
    parallel_for_(range, invoker, CC_MAT_TOTAL(dst)/(double)(1<<16));
}
};

template <typename T, typename WT>
struct ResizeAreaFastNoVec
{
    ResizeAreaFastNoVec(int, int) { }
    ResizeAreaFastNoVec(int, int, int, int) { }
    int operator() (const T*, T*, int) const
    { return 0; }
};

#if CC_NEON

class ResizeAreaFastVec_SIMD_8u
{
public:
    ResizeAreaFastVec_SIMD_8u(int _cn, int _step) :
        cn(_cn), step(_step)
    {
    }

    int operator() (const uchar* S, uchar* D, int w) const
    {
        int dx = 0;
        const uchar* S0 = S, * S1 = S0 + step;

        uint16x8_t v_2 = vdupq_n_u16(2);

        if (cn == 1)
        {
            for ( ; dx <= w - 16; dx += 16, S0 += 32, S1 += 32, D += 16)
            {
                uint8x16x2_t v_row0 = vld2q_u8(S0), v_row1 = vld2q_u8(S1);

                uint16x8_t v_dst0 = vaddl_u8(vget_low_u8(v_row0.val[0]), vget_low_u8(v_row0.val[1]));
                v_dst0 = vaddq_u16(v_dst0, vaddl_u8(vget_low_u8(v_row1.val[0]), vget_low_u8(v_row1.val[1])));
                v_dst0 = vshrq_n_u16(vaddq_u16(v_dst0, v_2), 2);

                uint16x8_t v_dst1 = vaddl_u8(vget_high_u8(v_row0.val[0]), vget_high_u8(v_row0.val[1]));
                v_dst1 = vaddq_u16(v_dst1, vaddl_u8(vget_high_u8(v_row1.val[0]), vget_high_u8(v_row1.val[1])));
                v_dst1 = vshrq_n_u16(vaddq_u16(v_dst1, v_2), 2);

                vst1q_u8(D, vcombine_u8(vmovn_u16(v_dst0), vmovn_u16(v_dst1)));
            }
        }
        else if (cn == 4)
        {
            for ( ; dx <= w - 8; dx += 8, S0 += 16, S1 += 16, D += 8)
            {
                uint8x16_t v_row0 = vld1q_u8(S0), v_row1 = vld1q_u8(S1);

                uint16x8_t v_row00 = vmovl_u8(vget_low_u8(v_row0));
                uint16x8_t v_row01 = vmovl_u8(vget_high_u8(v_row0));
                uint16x8_t v_row10 = vmovl_u8(vget_low_u8(v_row1));
                uint16x8_t v_row11 = vmovl_u8(vget_high_u8(v_row1));

                uint16x4_t v_p0 = vadd_u16(vadd_u16(vget_low_u16(v_row00), vget_high_u16(v_row00)),
                                           vadd_u16(vget_low_u16(v_row10), vget_high_u16(v_row10)));
                uint16x4_t v_p1 = vadd_u16(vadd_u16(vget_low_u16(v_row01), vget_high_u16(v_row01)),
                                           vadd_u16(vget_low_u16(v_row11), vget_high_u16(v_row11)));
                uint16x8_t v_dst = vshrq_n_u16(vaddq_u16(vcombine_u16(v_p0, v_p1), v_2), 2);

                vst1_u8(D, vmovn_u16(v_dst));
            }
        }

        return dx;
    }

private:
    int cn, step;
};

class ResizeAreaFastVec_SIMD_16u
{
public:
    ResizeAreaFastVec_SIMD_16u(int _cn, int _step) :
        cn(_cn), step(_step)
    {
    }

    int operator() (const ushort * S, ushort * D, int w) const
    {
        int dx = 0;
        const ushort * S0 = S, * S1 = (const ushort *)((const uchar *)(S0) + step);

        uint32x4_t v_2 = vdupq_n_u32(2);

        if (cn == 1)
        {
            for ( ; dx <= w - 8; dx += 8, S0 += 16, S1 += 16, D += 8)
            {
                uint16x8x2_t v_row0 = vld2q_u16(S0), v_row1 = vld2q_u16(S1);

                uint32x4_t v_dst0 = vaddl_u16(vget_low_u16(v_row0.val[0]), vget_low_u16(v_row0.val[1]));
                v_dst0 = vaddq_u32(v_dst0, vaddl_u16(vget_low_u16(v_row1.val[0]), vget_low_u16(v_row1.val[1])));
                v_dst0 = vshrq_n_u32(vaddq_u32(v_dst0, v_2), 2);

                uint32x4_t v_dst1 = vaddl_u16(vget_high_u16(v_row0.val[0]), vget_high_u16(v_row0.val[1]));
                v_dst1 = vaddq_u32(v_dst1, vaddl_u16(vget_high_u16(v_row1.val[0]), vget_high_u16(v_row1.val[1])));
                v_dst1 = vshrq_n_u32(vaddq_u32(v_dst1, v_2), 2);

                vst1q_u16(D, vcombine_u16(vmovn_u32(v_dst0), vmovn_u32(v_dst1)));
            }
        }
        else if (cn == 4)
        {
            for ( ; dx <= w - 4; dx += 4, S0 += 8, S1 += 8, D += 4)
            {
                uint16x8_t v_row0 = vld1q_u16(S0), v_row1 = vld1q_u16(S1);
                uint32x4_t v_dst = vaddq_u32(vaddl_u16(vget_low_u16(v_row0), vget_high_u16(v_row0)),
                                             vaddl_u16(vget_low_u16(v_row1), vget_high_u16(v_row1)));
                vst1_u16(D, vmovn_u32(vshrq_n_u32(vaddq_u32(v_dst, v_2), 2)));
            }
        }

        return dx;
    }

private:
    int cn, step;
};

class ResizeAreaFastVec_SIMD_16s
{
public:
    ResizeAreaFastVec_SIMD_16s(int _cn, int _step) :
        cn(_cn), step(_step)
    {
    }

    int operator() (const short * S, short * D, int w) const
    {
        int dx = 0;
        const short * S0 = S, * S1 = (const short *)((const uchar *)(S0) + step);

        int32x4_t v_2 = vdupq_n_s32(2);

        if (cn == 1)
        {
            for ( ; dx <= w - 8; dx += 8, S0 += 16, S1 += 16, D += 8)
            {
                int16x8x2_t v_row0 = vld2q_s16(S0), v_row1 = vld2q_s16(S1);

                int32x4_t v_dst0 = vaddl_s16(vget_low_s16(v_row0.val[0]), vget_low_s16(v_row0.val[1]));
                v_dst0 = vaddq_s32(v_dst0, vaddl_s16(vget_low_s16(v_row1.val[0]), vget_low_s16(v_row1.val[1])));
                v_dst0 = vshrq_n_s32(vaddq_s32(v_dst0, v_2), 2);

                int32x4_t v_dst1 = vaddl_s16(vget_high_s16(v_row0.val[0]), vget_high_s16(v_row0.val[1]));
                v_dst1 = vaddq_s32(v_dst1, vaddl_s16(vget_high_s16(v_row1.val[0]), vget_high_s16(v_row1.val[1])));
                v_dst1 = vshrq_n_s32(vaddq_s32(v_dst1, v_2), 2);

                vst1q_s16(D, vcombine_s16(vmovn_s32(v_dst0), vmovn_s32(v_dst1)));
            }
        }
        else if (cn == 4)
        {
            for ( ; dx <= w - 4; dx += 4, S0 += 8, S1 += 8, D += 4)
            {
                int16x8_t v_row0 = vld1q_s16(S0), v_row1 = vld1q_s16(S1);
                int32x4_t v_dst = vaddq_s32(vaddl_s16(vget_low_s16(v_row0), vget_high_s16(v_row0)),
                                            vaddl_s16(vget_low_s16(v_row1), vget_high_s16(v_row1)));
                vst1_s16(D, vmovn_s32(vshrq_n_s32(vaddq_s32(v_dst, v_2), 2)));
            }
        }

        return dx;
    }

private:
    int cn, step;
};

struct ResizeAreaFastVec_SIMD_32f
{
    ResizeAreaFastVec_SIMD_32f(int _scale_x, int _scale_y, int _cn, int _step) :
        cn(_cn), step(_step)
    {
        fast_mode = _scale_x == 2 && _scale_y == 2 && (cn == 1 || cn == 4);
    }

    int operator() (const float * S, float * D, int w) const
    {
        if (!fast_mode)
            return 0;

        const float * S0 = S, * S1 = (const float *)((const uchar *)(S0) + step);
        int dx = 0;

        float32x4_t v_025 = vdupq_n_f32(0.25f);

        if (cn == 1)
        {
            for ( ; dx <= w - 4; dx += 4, S0 += 8, S1 += 8, D += 4)
            {
                float32x4x2_t v_row0 = vld2q_f32(S0), v_row1 = vld2q_f32(S1);

                float32x4_t v_dst0 = vaddq_f32(v_row0.val[0], v_row0.val[1]);
                float32x4_t v_dst1 = vaddq_f32(v_row1.val[0], v_row1.val[1]);

                vst1q_f32(D, vmulq_f32(vaddq_f32(v_dst0, v_dst1), v_025));
            }
        }
        else if (cn == 4)
        {
            for ( ; dx <= w - 4; dx += 4, S0 += 8, S1 += 8, D += 4)
            {
                float32x4_t v_dst0 = vaddq_f32(vld1q_f32(S0), vld1q_f32(S0 + 4));
                float32x4_t v_dst1 = vaddq_f32(vld1q_f32(S1), vld1q_f32(S1 + 4));

                vst1q_f32(D, vmulq_f32(vaddq_f32(v_dst0, v_dst1), v_025));
            }
        }

        return dx;
    }

private:
    int cn;
    bool fast_mode;
    int step;
};

#elif CC_SSE2_

class ResizeAreaFastVec_SIMD_8u
{
public:
    ResizeAreaFastVec_SIMD_8u(int _cn, int _step) :
        cn(_cn), step(_step)
    {
        use_simd = checkHardwareSupport(CC_CPU_SSE2);
    }

    int operator() (const uchar* S, uchar* D, int w) const
    {
        if (!use_simd)
            return 0;

        int dx = 0;
        const uchar* S0 = S;
        const uchar* S1 = S0 + step;
        __m128i zero = _mm_setzero_si128();
        __m128i delta2 = _mm_set1_epi16(2);

        if (cn == 1)
        {
            __m128i masklow = _mm_set1_epi16(0x00ff);
            for ( ; dx <= w - 8; dx += 8, S0 += 16, S1 += 16, D += 8)
            {
                __m128i r0 = _mm_loadu_si128((const __m128i*)S0);
                __m128i r1 = _mm_loadu_si128((const __m128i*)S1);

                __m128i s0 = _mm_add_epi16(_mm_srli_epi16(r0, 8), _mm_and_si128(r0, masklow));
                __m128i s1 = _mm_add_epi16(_mm_srli_epi16(r1, 8), _mm_and_si128(r1, masklow));
                s0 = _mm_add_epi16(_mm_add_epi16(s0, s1), delta2);
                s0 = _mm_packus_epi16(_mm_srli_epi16(s0, 2), zero);

                _mm_storel_epi64((__m128i*)D, s0);
            }
        }
        else if (cn == 3)
            for ( ; dx <= w - 11; dx += 6, S0 += 12, S1 += 12, D += 6)
            {
                __m128i r0 = _mm_loadu_si128((const __m128i*)S0);
                __m128i r1 = _mm_loadu_si128((const __m128i*)S1);

                __m128i r0_16l = _mm_unpacklo_epi8(r0, zero);
                __m128i r0_16h = _mm_unpacklo_epi8(_mm_srli_si128(r0, 6), zero);
                __m128i r1_16l = _mm_unpacklo_epi8(r1, zero);
                __m128i r1_16h = _mm_unpacklo_epi8(_mm_srli_si128(r1, 6), zero);

                __m128i s0 = _mm_add_epi16(r0_16l, _mm_srli_si128(r0_16l, 6));
                __m128i s1 = _mm_add_epi16(r1_16l, _mm_srli_si128(r1_16l, 6));
                s0 = _mm_add_epi16(s1, _mm_add_epi16(s0, delta2));
                s0 = _mm_packus_epi16(_mm_srli_epi16(s0, 2), zero);
                _mm_storel_epi64((__m128i*)D, s0);

                s0 = _mm_add_epi16(r0_16h, _mm_srli_si128(r0_16h, 6));
                s1 = _mm_add_epi16(r1_16h, _mm_srli_si128(r1_16h, 6));
                s0 = _mm_add_epi16(s1, _mm_add_epi16(s0, delta2));
                s0 = _mm_packus_epi16(_mm_srli_epi16(s0, 2), zero);
                _mm_storel_epi64((__m128i*)(D+3), s0);
            }
        else
        {
            CC_Assert(cn == 4);
            int v[] = { 0, 0, -1, -1 };
            __m128i mask = _mm_loadu_si128((const __m128i*)v);

            for ( ; dx <= w - 8; dx += 8, S0 += 16, S1 += 16, D += 8)
            {
                __m128i r0 = _mm_loadu_si128((const __m128i*)S0);
                __m128i r1 = _mm_loadu_si128((const __m128i*)S1);

                __m128i r0_16l = _mm_unpacklo_epi8(r0, zero);
                __m128i r0_16h = _mm_unpackhi_epi8(r0, zero);
                __m128i r1_16l = _mm_unpacklo_epi8(r1, zero);
                __m128i r1_16h = _mm_unpackhi_epi8(r1, zero);

                __m128i s0 = _mm_add_epi16(r0_16l, _mm_srli_si128(r0_16l, 8));
                __m128i s1 = _mm_add_epi16(r1_16l, _mm_srli_si128(r1_16l, 8));
                s0 = _mm_add_epi16(s1, _mm_add_epi16(s0, delta2));
                __m128i res0 = _mm_srli_epi16(s0, 2);

                s0 = _mm_add_epi16(r0_16h, _mm_srli_si128(r0_16h, 8));
                s1 = _mm_add_epi16(r1_16h, _mm_srli_si128(r1_16h, 8));
                s0 = _mm_add_epi16(s1, _mm_add_epi16(s0, delta2));
                __m128i res1 = _mm_srli_epi16(s0, 2);
                s0 = _mm_packus_epi16(_mm_or_si128(_mm_andnot_si128(mask, res0),
                                                   _mm_and_si128(mask, _mm_slli_si128(res1, 8))), zero);
                _mm_storel_epi64((__m128i*)(D), s0);
            }
        }

        return dx;
    }

private:
    int cn;
    bool use_simd;
    int step;
};

class ResizeAreaFastVec_SIMD_16u
{
public:
    ResizeAreaFastVec_SIMD_16u(int _cn, int _step) :
        cn(_cn), step(_step)
    {
        use_simd = checkHardwareSupport(CC_CPU_SSE2);
    }

    int operator() (const ushort* S, ushort* D, int w) const
    {
        if (!use_simd)
            return 0;

        int dx = 0;
        const ushort* S0 = (const ushort*)S;
        const ushort* S1 = (const ushort*)((const uchar*)(S) + step);
        __m128i masklow = _mm_set1_epi32(0x0000ffff);
        __m128i zero = _mm_setzero_si128();
        __m128i delta2 = _mm_set1_epi32(2);

#define _mm_packus_epi32(a, zero) _mm_packs_epi32(_mm_srai_epi32(_mm_slli_epi32(a, 16), 16), zero)

        if (cn == 1)
        {
            for ( ; dx <= w - 4; dx += 4, S0 += 8, S1 += 8, D += 4)
            {
                __m128i r0 = _mm_loadu_si128((const __m128i*)S0);
                __m128i r1 = _mm_loadu_si128((const __m128i*)S1);

                __m128i s0 = _mm_add_epi32(_mm_srli_epi32(r0, 16), _mm_and_si128(r0, masklow));
                __m128i s1 = _mm_add_epi32(_mm_srli_epi32(r1, 16), _mm_and_si128(r1, masklow));
                s0 = _mm_add_epi32(_mm_add_epi32(s0, s1), delta2);
                s0 = _mm_srli_epi32(s0, 2);
                s0 = _mm_packus_epi32(s0, zero);

                _mm_storel_epi64((__m128i*)D, s0);
            }
        }
        else if (cn == 3)
            for ( ; dx <= w - 4; dx += 3, S0 += 6, S1 += 6, D += 3)
            {
                __m128i r0 = _mm_loadu_si128((const __m128i*)S0);
                __m128i r1 = _mm_loadu_si128((const __m128i*)S1);

                __m128i r0_16l = _mm_unpacklo_epi16(r0, zero);
                __m128i r0_16h = _mm_unpacklo_epi16(_mm_srli_si128(r0, 6), zero);
                __m128i r1_16l = _mm_unpacklo_epi16(r1, zero);
                __m128i r1_16h = _mm_unpacklo_epi16(_mm_srli_si128(r1, 6), zero);

                __m128i s0 = _mm_add_epi32(r0_16l, r0_16h);
                __m128i s1 = _mm_add_epi32(r1_16l, r1_16h);
                s0 = _mm_add_epi32(delta2, _mm_add_epi32(s0, s1));
                s0 = _mm_packus_epi32(_mm_srli_epi32(s0, 2), zero);
                _mm_storel_epi64((__m128i*)D, s0);
            }
        else
        {
            CC_Assert(cn == 4);
            for ( ; dx <= w - 4; dx += 4, S0 += 8, S1 += 8, D += 4)
            {
                __m128i r0 = _mm_loadu_si128((const __m128i*)S0);
                __m128i r1 = _mm_loadu_si128((const __m128i*)S1);

                __m128i r0_32l = _mm_unpacklo_epi16(r0, zero);
                __m128i r0_32h = _mm_unpackhi_epi16(r0, zero);
                __m128i r1_32l = _mm_unpacklo_epi16(r1, zero);
                __m128i r1_32h = _mm_unpackhi_epi16(r1, zero);

                __m128i s0 = _mm_add_epi32(r0_32l, r0_32h);
                __m128i s1 = _mm_add_epi32(r1_32l, r1_32h);
                s0 = _mm_add_epi32(s1, _mm_add_epi32(s0, delta2));
                s0 = _mm_packus_epi32(_mm_srli_epi32(s0, 2), zero);
                _mm_storel_epi64((__m128i*)D, s0);
            }
        }

#undef _mm_packus_epi32

        return dx;
    }

private:
    int cn;
    int step;
    bool use_simd;
};

class ResizeAreaFastVec_SIMD_16s
{
public:
    ResizeAreaFastVec_SIMD_16s(int _cn, int _step) :
        cn(_cn), step(_step)
    {
        use_simd = checkHardwareSupport(CC_CPU_SSE2);
    }

    int operator() (const short* S, short* D, int w) const
    {
        if (!use_simd)
            return 0;

        int dx = 0;
        const short* S0 = (const short*)S;
        const short* S1 = (const short*)((const uchar*)(S) + step);
        __m128i masklow = _mm_set1_epi32(0x0000ffff);
        __m128i zero = _mm_setzero_si128();
        __m128i delta2 = _mm_set1_epi32(2);

        if (cn == 1)
        {
            for ( ; dx <= w - 4; dx += 4, S0 += 8, S1 += 8, D += 4)
            {
                __m128i r0 = _mm_loadu_si128((const __m128i*)S0);
                __m128i r1 = _mm_loadu_si128((const __m128i*)S1);

                __m128i s0 = _mm_add_epi32(_mm_srai_epi32(r0, 16),
                    _mm_srai_epi32(_mm_slli_epi32(_mm_and_si128(r0, masklow), 16), 16));
                __m128i s1 = _mm_add_epi32(_mm_srai_epi32(r1, 16),
                    _mm_srai_epi32(_mm_slli_epi32(_mm_and_si128(r1, masklow), 16), 16));
                s0 = _mm_add_epi32(_mm_add_epi32(s0, s1), delta2);
                s0 = _mm_srai_epi32(s0, 2);
                s0 = _mm_packs_epi32(s0, zero);

                _mm_storel_epi64((__m128i*)D, s0);
            }
        }
        else if (cn == 3)
            for ( ; dx <= w - 4; dx += 3, S0 += 6, S1 += 6, D += 3)
            {
                __m128i r0 = _mm_loadu_si128((const __m128i*)S0);
                __m128i r1 = _mm_loadu_si128((const __m128i*)S1);

                __m128i r0_16l = _mm_srai_epi32(_mm_unpacklo_epi16(zero, r0), 16);
                __m128i r0_16h = _mm_srai_epi32(_mm_unpacklo_epi16(zero, _mm_srli_si128(r0, 6)), 16);
                __m128i r1_16l = _mm_srai_epi32(_mm_unpacklo_epi16(zero, r1), 16);
                __m128i r1_16h = _mm_srai_epi32(_mm_unpacklo_epi16(zero, _mm_srli_si128(r1, 6)), 16);

                __m128i s0 = _mm_add_epi32(r0_16l, r0_16h);
                __m128i s1 = _mm_add_epi32(r1_16l, r1_16h);
                s0 = _mm_add_epi32(delta2, _mm_add_epi32(s0, s1));
                s0 = _mm_packs_epi32(_mm_srai_epi32(s0, 2), zero);
                _mm_storel_epi64((__m128i*)D, s0);
            }
        else
        {
            CC_Assert(cn == 4);
            for ( ; dx <= w - 4; dx += 4, S0 += 8, S1 += 8, D += 4)
            {
                __m128i r0 = _mm_loadu_si128((const __m128i*)S0);
                __m128i r1 = _mm_loadu_si128((const __m128i*)S1);

                __m128i r0_32l = _mm_srai_epi32(_mm_unpacklo_epi16(zero, r0), 16);
                __m128i r0_32h = _mm_srai_epi32(_mm_unpackhi_epi16(zero, r0), 16);
                __m128i r1_32l = _mm_srai_epi32(_mm_unpacklo_epi16(zero, r1), 16);
                __m128i r1_32h = _mm_srai_epi32(_mm_unpackhi_epi16(zero, r1), 16);

                __m128i s0 = _mm_add_epi32(r0_32l, r0_32h);
                __m128i s1 = _mm_add_epi32(r1_32l, r1_32h);
                s0 = _mm_add_epi32(s1, _mm_add_epi32(s0, delta2));
                s0 = _mm_packs_epi32(_mm_srai_epi32(s0, 2), zero);
                _mm_storel_epi64((__m128i*)D, s0);
            }
        }

        return dx;
    }

private:
    int cn;
    int step;
    bool use_simd;
};

struct ResizeAreaFastVec_SIMD_32f
{
    ResizeAreaFastVec_SIMD_32f(int _scale_x, int _scale_y, int _cn, int _step) :
        cn(_cn), step(_step)
    {
        fast_mode = _scale_x == 2 && _scale_y == 2 && (cn == 1 || cn == 4);
        fast_mode = fast_mode && checkHardwareSupport(CC_CPU_SSE2);
    }

    int operator() (const float * S, float * D, int w) const
    {
        if (!fast_mode)
            return 0;

        const float * S0 = S, * S1 = (const float *)((const uchar *)(S0) + step);
        int dx = 0;

        __m128 v_025 = _mm_set1_ps(0.25f);

        if (cn == 1)
        {
            const int shuffle_lo = _MM_SHUFFLE(2, 0, 2, 0), shuffle_hi = _MM_SHUFFLE(3, 1, 3, 1);
            for ( ; dx <= w - 4; dx += 4, S0 += 8, S1 += 8, D += 4)
            {
                __m128 v_row00 = _mm_loadu_ps(S0), v_row01 = _mm_loadu_ps(S0 + 4),
                       v_row10 = _mm_loadu_ps(S1), v_row11 = _mm_loadu_ps(S1 + 4);

                __m128 v_dst0 = _mm_add_ps(_mm_shuffle_ps(v_row00, v_row01, shuffle_lo),
                                           _mm_shuffle_ps(v_row00, v_row01, shuffle_hi));
                __m128 v_dst1 = _mm_add_ps(_mm_shuffle_ps(v_row10, v_row11, shuffle_lo),
                                           _mm_shuffle_ps(v_row10, v_row11, shuffle_hi));

                _mm_storeu_ps(D, _mm_mul_ps(_mm_add_ps(v_dst0, v_dst1), v_025));
            }
        }
        else if (cn == 4)
        {
            for ( ; dx <= w - 4; dx += 4, S0 += 8, S1 += 8, D += 4)
            {
                __m128 v_dst0 = _mm_add_ps(_mm_loadu_ps(S0), _mm_loadu_ps(S0 + 4));
                __m128 v_dst1 = _mm_add_ps(_mm_loadu_ps(S1), _mm_loadu_ps(S1 + 4));

                _mm_storeu_ps(D, _mm_mul_ps(_mm_add_ps(v_dst0, v_dst1), v_025));
            }
        }

        return dx;
    }

private:
    int cn;
    bool fast_mode;
    int step;
};

#else

typedef ResizeAreaFastNoVec<uchar, uchar> ResizeAreaFastVec_SIMD_8u;
typedef ResizeAreaFastNoVec<ushort, ushort> ResizeAreaFastVec_SIMD_16u;
typedef ResizeAreaFastNoVec<short, short> ResizeAreaFastVec_SIMD_16s;
typedef ResizeAreaFastNoVec<float, float> ResizeAreaFastVec_SIMD_32f;

#endif

template<typename T, typename SIMDVecOp>
struct ResizeAreaFastVec
{
    ResizeAreaFastVec(int _scale_x, int _scale_y, int _cn, int _step) :
        scale_x(_scale_x), scale_y(_scale_y), cn(_cn), step(_step), vecOp(_cn, _step)
    {
        fast_mode = scale_x == 2 && scale_y == 2 && (cn == 1 || cn == 3 || cn == 4);
    }

    int operator() (const T* S, T* D, int w) const
    {
        if (!fast_mode)
            return 0;

        const T* nextS = (const T*)((const uchar*)S + step);
        int dx = vecOp(S, D, w);

        if (cn == 1)
            for( ; dx < w; ++dx )
            {
                int index = dx*2;
                D[dx] = (T)((S[index] + S[index+1] + nextS[index] + nextS[index+1] + 2) >> 2);
            }
        else if (cn == 3)
            for( ; dx < w; dx += 3 )
            {
                int index = dx*2;
                D[dx] = (T)((S[index] + S[index+3] + nextS[index] + nextS[index+3] + 2) >> 2);
                D[dx+1] = (T)((S[index+1] + S[index+4] + nextS[index+1] + nextS[index+4] + 2) >> 2);
                D[dx+2] = (T)((S[index+2] + S[index+5] + nextS[index+2] + nextS[index+5] + 2) >> 2);
            }
        else
            {
                CC_Assert(cn == 4);
                for( ; dx < w; dx += 4 )
                {
                    int index = dx*2;
                    D[dx] = (T)((S[index] + S[index+4] + nextS[index] + nextS[index+4] + 2) >> 2);
                    D[dx+1] = (T)((S[index+1] + S[index+5] + nextS[index+1] + nextS[index+5] + 2) >> 2);
                    D[dx+2] = (T)((S[index+2] + S[index+6] + nextS[index+2] + nextS[index+6] + 2) >> 2);
                    D[dx+3] = (T)((S[index+3] + S[index+7] + nextS[index+3] + nextS[index+7] + 2) >> 2);
                }
            }

        return dx;
    }

private:
    int scale_x, scale_y;
    int cn;
    bool fast_mode;
    int step;
    SIMDVecOp vecOp;
};

template <typename T, typename WT, typename VecOp>
class resizeAreaFast_Invoker :
    public CParallelLoopBody
{
public:
    resizeAreaFast_Invoker(const img_t* src, img_t* dst,
        int _scale_x, int _scale_y, const int* _ofs, const int* _xofs) :
        CParallelLoopBody(), src(src), dst(dst), scale_x(_scale_x),
        scale_y(_scale_y), ofs(_ofs), xofs(_xofs)
    {
    }

    virtual void operator() (const CRange& range) const
    {
        CSize ssize = cvGetSize(src), dsize = cvGetSize(dst);
        int cn = CC_MAT_CN(src);
        int area = scale_x*scale_y;
        float scale = 1.f/(area);
        int dwidth1 = (ssize.width/scale_x)*cn;
        dsize.width *= cn;
        ssize.width *= cn;
        int dy, dx, k = 0;

        VecOp vop(scale_x, scale_y, CC_MAT_CN(src), (int)src->step/*, area_ofs*/);

        for( dy = range.start; dy < range.end; dy++ )
        {
            T* D = (T*)(dst->tt.data + dst->step*dy);
            int sy0 = dy*scale_y;
            int w = sy0 + scale_y <= ssize.height ? dwidth1 : 0;

            if( sy0 >= ssize.height )
            {
                for( dx = 0; dx < dsize.width; dx++ )
                    D[dx] = 0;
                continue;
            }

            dx = vop(img_row_(T, src, sy0), D, w);
            for( ; dx < w; dx++ )
            {
                const T* S = img_row_(T, src, sy0) + xofs[dx];
                WT sum = 0;
                k = 0;
                #if CC_ENABLE_UNROLLED
                for( ; k <= area - 4; k += 4 )
                    sum += S[ofs[k]] + S[ofs[k+1]] + S[ofs[k+2]] + S[ofs[k+3]];
                #endif
                for( ; k < area; k++ )
                    sum += S[ofs[k]];

                saturate_cast(D[dx], sum * scale);
            }

            for( ; dx < dsize.width; dx++ )
            {
                WT sum = 0;
                int count = 0, sx0 = xofs[dx];
                if( sx0 >= ssize.width )
                    D[dx] = 0;

                for( int sy = 0; sy < scale_y; sy++ )
                {
                    if( sy0 + sy >= ssize.height )
                        break;
                    const T* S = img_row_(T, src, sy0 + sy) + sx0;
                    for( int sx = 0; sx < scale_x*cn; sx += cn )
                    {
                        if( sx0 + sx >= ssize.width )
                            break;
                        sum += S[sx];
                        count++;
                    }
                }

                saturate_cast(D[dx], (float)sum/count);
            }
        }
    }

private:
    const img_t *src;
    img_t *dst;
    int scale_x, scale_y;
    const int *ofs, *xofs;
};

template<typename T, typename WT, typename VecOp>
struct resizeAreaFast_ {
static void f( const img_t* src, img_t* dst, const int* ofs, const int* xofs,
                             int scale_x, int scale_y )
{
    CRange range = {0, dst->rows};
    resizeAreaFast_Invoker<T, WT, VecOp> invoker(src, dst, scale_x,
        scale_y, ofs, xofs);
    parallel_for_(range, invoker, CC_MAT_TOTAL(dst)/(double)(1<<16));
}
};

struct DecimateAlpha
{
    int si, di;
    float alpha;
};


template<typename T, typename WT> class ResizeArea_Invoker :
    public CParallelLoopBody
{
public:
    ResizeArea_Invoker( const img_t* src, img_t* dst,
                        const DecimateAlpha* _xtab, int _xtab_size,
                        const DecimateAlpha* _ytab, int _ytab_size,
                        const int* _tabofs )
    {
        src = src;
        dst = dst;
        xtab0 = _xtab;
        xtab_size0 = _xtab_size;
        ytab = _ytab;
        ytab_size = _ytab_size;
        tabofs = _tabofs;
    }

    virtual void operator() (const CRange& range) const
    {
        CSize dsize = cvGetSize(dst);
        int cn = CC_MAT_CN(dst);
        dsize.width *= cn;
        WT* _buffer = MALLOC(WT, dsize.width*2);
        const DecimateAlpha* xtab = xtab0;
        int xtab_size = xtab_size0;
        WT *buf = _buffer, *sum = buf + dsize.width;
        int j_start = tabofs[range.start], j_end = tabofs[range.end], j, k, dx, prev_dy = ytab[j_start].di;

        for( dx = 0; dx < dsize.width; dx++ )
            sum[dx] = (WT)0;

        for( j = j_start; j < j_end; j++ )
        {
            WT beta = ytab[j].alpha;
            int dy = ytab[j].di;
            int sy = ytab[j].si;

            {
                const T* S = img_row_(T, src, sy);
                for( dx = 0; dx < dsize.width; dx++ )
                    buf[dx] = (WT)0;

                if( cn == 1 )
                    for( k = 0; k < xtab_size; k++ )
                    {
                        int dxn = xtab[k].di;
                        WT alpha = xtab[k].alpha;
                        buf[dxn] += S[xtab[k].si]*alpha;
                    }
                else if( cn == 2 )
                    for( k = 0; k < xtab_size; k++ )
                    {
                        int sxn = xtab[k].si;
                        int dxn = xtab[k].di;
                        WT alpha = xtab[k].alpha;
                        WT t0 = buf[dxn] + S[sxn]*alpha;
                        WT t1 = buf[dxn+1] + S[sxn+1]*alpha;
                        buf[dxn] = t0; buf[dxn+1] = t1;
                    }
                else if( cn == 3 )
                    for( k = 0; k < xtab_size; k++ )
                    {
                        int sxn = xtab[k].si;
                        int dxn = xtab[k].di;
                        WT alpha = xtab[k].alpha;
                        WT t0 = buf[dxn] + S[sxn]*alpha;
                        WT t1 = buf[dxn+1] + S[sxn+1]*alpha;
                        WT t2 = buf[dxn+2] + S[sxn+2]*alpha;
                        buf[dxn] = t0; buf[dxn+1] = t1; buf[dxn+2] = t2;
                    }
                else if( cn == 4 )
                {
                    for( k = 0; k < xtab_size; k++ )
                    {
                        int sxn = xtab[k].si;
                        int dxn = xtab[k].di;
                        WT alpha = xtab[k].alpha;
                        WT t0 = buf[dxn] + S[sxn]*alpha;
                        WT t1 = buf[dxn+1] + S[sxn+1]*alpha;
                        buf[dxn] = t0; buf[dxn+1] = t1;
                        t0 = buf[dxn+2] + S[sxn+2]*alpha;
                        t1 = buf[dxn+3] + S[sxn+3]*alpha;
                        buf[dxn+2] = t0; buf[dxn+3] = t1;
                    }
                }
                else
                {
                    for( k = 0; k < xtab_size; k++ )
                    {
                        int sxn = xtab[k].si;
                        int dxn = xtab[k].di;
                        WT alpha = xtab[k].alpha;
                        for( int c = 0; c < cn; c++ )
                            buf[dxn + c] += S[sxn + c]*alpha;
                    }
                }
            }

            if( dy != prev_dy )
            {
                T* D = img_row_(T, dst, prev_dy);

                for( dx = 0; dx < dsize.width; dx++ )
                {
                    saturate_cast(D[dx], sum[dx]);
                    sum[dx] = beta*buf[dx];
                }
                prev_dy = dy;
            }
            else
            {
                for( dx = 0; dx < dsize.width; dx++ )
                    sum[dx] += beta*buf[dx];
            }
        }

        {
        T* D = img_row_(T, dst, prev_dy);
        for( dx = 0; dx < dsize.width; dx++ )
            saturate_cast(D[dx], sum[dx]);
        }
        FREE(_buffer);
    }

private:
    const img_t* src;
    img_t* dst;
    const DecimateAlpha* xtab0;
    const DecimateAlpha* ytab;
    int xtab_size0, ytab_size;
    const int* tabofs;
};


template <typename T, typename WT>
struct resizeArea_ {
static void f( const img_t* src, img_t* dst,
                         const DecimateAlpha* xtab, int xtab_size,
                         const DecimateAlpha* ytab, int ytab_size,
                         const int* tabofs )
{
    parallel_for_(cRange(0, dst->rows),
                 ResizeArea_Invoker<T, WT>(src, dst, xtab, xtab_size, ytab, ytab_size, tabofs),
                 CC_MAT_TOTAL(dst)/((double)(1 << 16)));
}
};

typedef void (*ResizeFunc)( const img_t* src, img_t* dst,
                            const int* xofs, const void* alpha,
                            const int* yofs, const void* beta,
                            int xmin, int xmax, int ksize );

typedef void (*ResizeAreaFastFunc)( const img_t* src, img_t* dst,
                                    const int* ofs, const int *xofs,
                                    int scale_x, int scale_y );

typedef void (*ResizeAreaFunc)( const img_t* src, img_t* dst,
                                const DecimateAlpha* xtab, int xtab_size,
                                const DecimateAlpha* ytab, int ytab_size,
                                const int* yofs);


static int computeResizeAreaTab( int ssize, int dsize, int cn, double scale, DecimateAlpha* tab )
{
    int k = 0;
    for(int dx = 0; dx < dsize; dx++ )
    {
        double fsx1 = dx * scale;
        double fsx2 = fsx1 + scale;
        double cellWidth = MIN(scale, ssize - fsx1);

        int sx1 = cCeil(fsx1), sx2 = cFloor(fsx2);

        sx2 = MIN(sx2, ssize - 1);
        sx1 = MIN(sx1, sx2);

        if( sx1 - fsx1 > 1e-3 )
        {
            assert( k < ssize*2 );
            tab[k].di = dx * cn;
            tab[k].si = (sx1 - 1) * cn;
            tab[k++].alpha = (float)((sx1 - fsx1) / cellWidth);
        }

        for(int sx = sx1; sx < sx2; sx++ )
        {
            assert( k < ssize*2 );
            tab[k].di = dx * cn;
            tab[k].si = sx * cn;
            tab[k++].alpha = float(1.0 / cellWidth);
        }

        if( fsx2 - sx2 > 1e-3 )
        {
            assert( k < ssize*2 );
            tab[k].di = dx * cn;
            tab[k].si = sx2 * cn;
            tab[k++].alpha = (float)(MIN(MIN(fsx2 - sx2, 1.), cellWidth) / cellWidth);
        }
    }
    return k;
}

#define CHECK_IPP_STATUS(STATUS) if (STATUS < 0) { *ok = false; return; }

#define SET_IPP_RESIZE_LINEAR_FUNC_PTR(TYPE, CN) \
    ippiResize = (ippiResizeFunc)ippiResizeLinear_##TYPE##_##CN##R; \
    CHECK_IPP_STATUS(ippiResizeGetSize_##TYPE(srcSize, dstSize, (IppiInterpolationType)mode, 0, &specSize, &initSize));\
    specBuf.allocate(specSize);\
    pSpec = (uchar*)specBuf;\
    CHECK_IPP_STATUS(ippiResizeLinearInit_##TYPE(srcSize, dstSize, (IppiResizeSpec_32f*)pSpec));

#define SET_IPP_RESIZE_LINEAR_FUNC_64_PTR(TYPE, CN) \
    if (mode == (int)ippCubic) { *ok = false; return; } \
    ippiResize = (ippiResizeFunc)ippiResizeLinear_##TYPE##_##CN##R; \
    CHECK_IPP_STATUS(ippiResizeGetSize_##TYPE(srcSize, dstSize, (IppiInterpolationType)mode, 0, &specSize, &initSize));\
    specBuf.allocate(specSize);\
    pSpec = (uchar*)specBuf;\
    CHECK_IPP_STATUS(ippiResizeLinearInit_##TYPE(srcSize, dstSize, (IppiResizeSpec_64f*)pSpec));\
    getBufferSizeFunc = (ippiResizeGetBufferSize)ippiResizeGetBufferSize_##TYPE;\
    getSrcOffsetFunc =  (ippiResizeGetSrcOffset) ippiResizeGetSrcOffset_##TYPE;

#define SET_IPP_RESIZE_CUBIC_FUNC_PTR(TYPE, CN) \
    ippiResize = (ippiResizeFunc)ippiResizeCubic_##TYPE##_##CN##R; \
    CHECK_IPP_STATUS(ippiResizeGetSize_##TYPE(srcSize, dstSize, (IppiInterpolationType)mode, 0, &specSize, &initSize));\
    specBuf.allocate(specSize);\
    pSpec = (uchar*)specBuf;\
    CAutoBuffer<uchar> buf(initSize);\
    uchar* pInit = (uchar*)buf;\
    CHECK_IPP_STATUS(ippiResizeCubicInit_##TYPE(srcSize, dstSize, 0.f, 0.75f, (IppiResizeSpec_32f*)pSpec, pInit));

#define SET_IPP_RESIZE_PTR(TYPE, CN) \
    if (mode == (int)ippLinear)     { SET_IPP_RESIZE_LINEAR_FUNC_PTR(TYPE, CN);} \
    else if (mode == (int)ippCubic) { SET_IPP_RESIZE_CUBIC_FUNC_PTR(TYPE, CN);} \
    else { *ok = false; return; } \
    getBufferSizeFunc = (ippiResizeGetBufferSize)ippiResizeGetBufferSize_##TYPE; \
    getSrcOffsetFunc =  (ippiResizeGetSrcOffset)ippiResizeGetSrcOffset_##TYPE;

#if IPP_VERSION_X100 >= 710
class IPPresizeInvoker :
    public CParallelLoopBody
{
public:
    IPPresizeInvoker(const img_t*  src, img_t*  dst, double _inv_scale_x, double _inv_scale_y, int _mode, bool *_ok) :
        CParallelLoopBody(), src(src), dst(dst), inv_scale_x(_inv_scale_x),
        inv_scale_y(_inv_scale_y), pSpec(NULL), mode(_mode),
        ippiResize(NULL), getBufferSizeFunc(NULL), getSrcOffsetFunc(NULL), ok(_ok)
    {
        *ok = true;
        IppiSize srcSize, dstSize;
        int type = src->tid, specSize = 0, initSize = 0;
        srcSize.width  = src->cols;
        srcSize.height = src->rows;
        dstSize.width  = dst->cols;
        dstSize.height = dst->rows;

        switch (type)
        {
#if IPP_DISABLE_BLOCK // disabled since it breaks tests for CascadeClassifier
            case CC_8UC1:  SET_IPP_RESIZE_PTR(8u,C1);  break;
            case CC_8UC3:  SET_IPP_RESIZE_PTR(8u,C3);  break;
            case CC_8UC4:  SET_IPP_RESIZE_PTR(8u,C4);  break;
#endif
            case CC_16UC1: SET_IPP_RESIZE_PTR(16u,C1); break;
            case CC_16UC3: SET_IPP_RESIZE_PTR(16u,C3); break;
            case CC_16UC4: SET_IPP_RESIZE_PTR(16u,C4); break;
            case CC_16SC1: SET_IPP_RESIZE_PTR(16s,C1); break;
            case CC_16SC3: SET_IPP_RESIZE_PTR(16s,C3); break;
            case CC_16SC4: SET_IPP_RESIZE_PTR(16s,C4); break;
            case CC_32FC1: SET_IPP_RESIZE_PTR(32f,C1); break;
            case CC_32FC3: SET_IPP_RESIZE_PTR(32f,C3); break;
            case CC_32FC4: SET_IPP_RESIZE_PTR(32f,C4); break;
            case CC_64FC1: SET_IPP_RESIZE_LINEAR_FUNC_64_PTR(64f,C1); break;
            case CC_64FC3: SET_IPP_RESIZE_LINEAR_FUNC_64_PTR(64f,C3); break;
            case CC_64FC4: SET_IPP_RESIZE_LINEAR_FUNC_64_PTR(64f,C4); break;
            default: { *ok = false; return; } break;
        }
    }

    ~IPPresizeInvoker()
    {
    }

    virtual void operator() (const CRange& range) const
    {
        if (*ok == false)
            return;

        int cn = CC_MAT_CN(src);
        int dsty = min(cRound(range.start * inv_scale_y), dst->rows);
        int dstwidth  = min(cRound(src->cols * inv_scale_x), dst->cols);
        int dstheight = min(cRound(range.end * inv_scale_y), dst->rows);

        IppiPoint dstOffset = { 0, dsty }, srcOffset = {0, 0};
        IppiSize  dstSize   = { dstwidth, dstheight - dsty };
        int bufsize = 0, itemSize = (int)src CC_ELEM_SIZE1();

        CHECK_IPP_STATUS(getBufferSizeFunc(pSpec, dstSize, cn, &bufsize));
        CHECK_IPP_STATUS(getSrcOffsetFunc(pSpec, dstOffset, &srcOffset));

        const Ipp8u* pSrc = src.ptr<Ipp8u>(srcOffset.y) + srcOffset.x * cn * itemSize;
        Ipp8u* pDst = dst.ptr<Ipp8u>(dstOffset.y) + dstOffset.x * cn * itemSize;

        CAutoBuffer<uchar> buf(bufsize + 64);
        uchar* bufptr = cAlignPtr((uchar*)buf, 32);

        if( CC_INSTRUMENT_FUN_IPP(ippiResize, pSrc, (int)src->step[0], pDst, (int)dst->step[0], dstOffset, dstSize, ippBorderRepl, 0, pSpec, bufptr) < 0 )
            *ok = false;
        else
        {
            CC_IMPL_ADD(CC_IMPL_IPP|CC_IMPL_MT);
        }
    }
private:
    const img_t*  src;
    img_t*  dst;
    double inv_scale_x;
    double inv_scale_y;
    void *pSpec;
    CAutoBuffer<uchar> specBuf;
    int mode;
    ippiResizeFunc ippiResize;
    ippiResizeGetBufferSize getBufferSizeFunc;
    ippiResizeGetSrcOffset getSrcOffsetFunc;
    bool *ok;
    const IPPresizeInvoker& operator= (const IPPresizeInvoker&);
};

#endif

#ifdef HAVE_OPENCL

static void ocl_computeResizeAreaTabs(int ssize, int dsize, double scale, int * const map_tab,
                                      float * const alpha_tab, int * const ofs_tab)
{
    int k = 0, dx = 0;
    for ( ; dx < dsize; dx++)
    {
        ofs_tab[dx] = k;

        double fsx1 = dx * scale;
        double fsx2 = fsx1 + scale;
        double cellWidth = MIN(scale, ssize - fsx1);

        int sx1 = cCeil(fsx1), sx2 = cFloor(fsx2);

        sx2 = MIN(sx2, ssize - 1);
        sx1 = MIN(sx1, sx2);

        if (sx1 - fsx1 > 1e-3)
        {
            map_tab[k] = sx1 - 1;
            alpha_tab[k++] = (float)((sx1 - fsx1) / cellWidth);
        }

        for (int sx = sx1; sx < sx2; sx++)
        {
            map_tab[k] = sx;
            alpha_tab[k++] = float(1.0 / cellWidth);
        }

        if (fsx2 - sx2 > 1e-3)
        {
            map_tab[k] = sx2;
            alpha_tab[k++] = (float)(MIN(MIN(fsx2 - sx2, 1.), cellWidth) / cellWidth);
        }
    }
    ofs_tab[dx] = k;
}

static bool ocl_resize( const img_t* src, img_t* dst, CSize dsize,
                        double fx, double fy, int interpolation)
{
    int type = src->tid, depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type);

    double inv_fx = 1.0 / fx, inv_fy = 1.0 / fy;
    float inv_fxf = (float)inv_fx, inv_fyf = (float)inv_fy;
    int iscale_x = saturate_cast<int>(inv_fx), iscale_y = saturate_cast<int>(inv_fx);
    bool is_area_fast = fabs(inv_fx - iscale_x) < DBL_EPSILON &&
        fabs(inv_fy - iscale_y) < DBL_EPSILON;

    // in case of scale_x && scale_y is equal to 2
    // CC_INTER_AREA (fast) also is equal to CC_INTER_LINEAR
    if( interpolation == CC_INTER_LINEAR && is_area_fast && iscale_x == 2 && iscale_y == 2 )
        /*interpolation = CC_INTER_AREA*/(void)0; // CC_INTER_AREA is slower

    if( !(cn <= 4 &&
           (interpolation == CC_INTER_NEAREST || interpolation == CC_INTER_LINEAR ||
            (interpolation == CC_INTER_AREA && inv_fx >= 1 && inv_fy >= 1) )) )
        return false;

    UMat src = src.getUMat();
    dst cvSetMat(dsize, type);
    UMat dst = dst.getUMat();

    CSize ssize = cvGetSize(src);
    ocl::Kernel k;
    size_t globalsize[] = { (size_t)dst->cols, (size_t)dst->rows };

    ocl::Image2D srcImage;

    // See if this could be done with a sampler.  We stick with integer
    // datatypes because the observed error is low.
    bool useSampler = (interpolation == CC_INTER_LINEAR && ocl::Device::getDefault().imageSupport() &&
                       ocl::Image2D::canCreateAlias(src) && depth <= 4 &&
                       ocl::Image2D::isFormatSupported(depth, cn, true) &&
                       src.offset==0);
    if (useSampler)
    {
        int wdepth = MAX(depth, CC_32S);
        char buf[2][32];
        String compileOpts = format("-D USE_SAMPLER -D depth=%d -D T=%s -D T1=%s "
                        "-D convertToDT=%s -D cn=%d",
                        depth, ocl::typeToStr(type), ocl::typeToStr(depth),
                        ocl::convertTypeStr(wdepth, depth, cn, buf[1]),
                        cn);
        k cvSetMat("resizeSampler", ocl::imgproc::resize_oclsrc, compileOpts);

        if (k.empty())
            useSampler = false;
        else
        {
            // Convert the input into an OpenCL image type, using normalized channel data types
            // and aliasing the UMat.
            srcImage = ocl::Image2D(src, true, true);
            k.args(srcImage, ocl::KernelArg::WriteOnly(dst),
                   (float)inv_fx, (float)inv_fy);
        }
    }

    if (interpolation == CC_INTER_LINEAR && !useSampler)
    {
        char buf[2][32];

        // integer path is slower because of CPU part, so it's disabled
        if (depth == CC_8U && ((void)0, 0))
        {
            CAutoBuffer<uchar> _buffer((dsize.width + dsize.height)*(sizeof(int) + sizeof(short)*2));
            int* xofs = (int*)(uchar*)_buffer, * yofs = xofs + dsize.width;
            short* ialpha = (short*)(yofs + dsize.height), * ibeta = ialpha + dsize.width*2;
            float fxx, fyy;
            int sx, sy;

            for (int dx = 0; dx < dsize.width; dx++)
            {
                fxx = (float)((dx+0.5)*inv_fx - 0.5);
                sx = cFloor(fxx);
                fxx -= sx;

                if (sx < 0)
                    fxx = 0, sx = 0;

                if (sx >= ssize.width-1)
                    fxx = 0, sx = ssize.width-1;

                xofs[dx] = sx;
                ialpha[dx*2 + 0] = saturate_cast<short>((1.f - fxx) * CC_INTER_RESIZE_COEF_SCALE);
                ialpha[dx*2 + 1] = saturate_cast<short>(fxx         * CC_INTER_RESIZE_COEF_SCALE);
            }

            for (int dy = 0; dy < dsize.height; dy++)
            {
                fyy = (float)((dy+0.5)*inv_fy - 0.5);
                sy = cFloor(fyy);
                fyy -= sy;

                yofs[dy] = sy;
                ibeta[dy*2 + 0] = saturate_cast<short>((1.f - fyy) * CC_INTER_RESIZE_COEF_SCALE);
                ibeta[dy*2 + 1] = saturate_cast<short>(fyy         * CC_INTER_RESIZE_COEF_SCALE);
            }

            int wdepth = MAX(depth, CC_32S), wtype = CC_MAKETYPE(wdepth, cn);
            UMat coeffs;
            img_t(1, static_cast<int>(_buffer.size()), CC_8UC1, (uchar *)_buffer).copyTo(coeffs);

            k cvSetMat("resizeLN", ocl::imgproc::resize_oclsrc,
                     format("-D INTER_LINEAR_INTEGER -D depth=%d -D T=%s -D T1=%s "
                            "-D WT=%s -D convertToWT=%s -D convertToDT=%s -D cn=%d "
                            "-D CC_INTER_RESIZE_COEF_BITS=%d",
                            depth, ocl::typeToStr(type), ocl::typeToStr(depth), ocl::typeToStr(wtype),
                            ocl::convertTypeStr(depth, wdepth, cn, buf[0]),
                            ocl::convertTypeStr(wdepth, depth, cn, buf[1]),
                            cn, CC_INTER_RESIZE_COEF_BITS));
            if (k.empty())
                return false;

            k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
                   ocl::KernelArg::PtrReadOnly(coeffs));
        }
        else
        {
            int wdepth = MAX(depth, CC_32S), wtype = CC_MAKETYPE(wdepth, cn);
            k cvSetMat("resizeLN", ocl::imgproc::resize_oclsrc,
                     format("-D CC_INTER_LINEAR -D depth=%d -D T=%s -D T1=%s "
                            "-D WT=%s -D convertToWT=%s -D convertToDT=%s -D cn=%d "
                            "-D CC_INTER_RESIZE_COEF_BITS=%d",
                            depth, ocl::typeToStr(type), ocl::typeToStr(depth), ocl::typeToStr(wtype),
                            ocl::convertTypeStr(depth, wdepth, cn, buf[0]),
                            ocl::convertTypeStr(wdepth, depth, cn, buf[1]),
                            cn, CC_INTER_RESIZE_COEF_BITS));
            if (k.empty())
                return false;

            k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
                   (float)inv_fx, (float)inv_fy);
        }
    }
    else if (interpolation == CC_INTER_NEAREST)
    {
        k cvSetMat("resizeNN", ocl::imgproc::resize_oclsrc,
                 format("-D CC_INTER_NEAREST -D T=%s -D T1=%s -D cn=%d",
                        ocl::vecopTypeToStr(type), ocl::vecopTypeToStr(depth), cn));
        if (k.empty())
            return false;

        k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst),
               (float)inv_fx, (float)inv_fy);
    }
    else if (interpolation == CC_INTER_AREA)
    {
        int wdepth = MAX(depth, is_area_fast ? CC_32S : CC_32F);
        int wtype = CC_MAKE_TYPE(wdepth, cn);

        char cvt[2][40];
        String buildOption = format("-D CC_INTER_AREA -D T=%s -D T1=%s -D WTV=%s -D convertToWTV=%s -D cn=%d",
                                    ocl::typeToStr(type), ocl::typeToStr(depth), ocl::typeToStr(wtype),
                                    ocl::convertTypeStr(depth, wdepth, cn, cvt[0]), cn);

        UMat alphaOcl, tabofsOcl, mapOcl;
        UMat dmap, smap;

        if (is_area_fast)
        {
            int wdepth2 = MAX(CC_32F, depth), wtype2 = CC_MAKE_TYPE(wdepth2, cn);
            buildOption = buildOption + format(" -D convertToT=%s -D WT2V=%s -D convertToWT2V=%s -D INTER_AREA_FAST"
                                                " -D XSCALE=%d -D YSCALE=%d -D SCALE=%ff",
                                                ocl::convertTypeStr(wdepth2, depth, cn, cvt[0]),
                                                ocl::typeToStr(wtype2), ocl::convertTypeStr(wdepth, wdepth2, cn, cvt[1]),
                                    iscale_x, iscale_y, 1.0f / (iscale_x * iscale_y));

            k cvSetMat("resizeAREA_FAST", ocl::imgproc::resize_oclsrc, buildOption);
            if (k.empty())
                return false;
        }
        else
        {
            buildOption = buildOption + format(" -D convertToT=%s", ocl::convertTypeStr(wdepth, depth, cn, cvt[0]));
            k cvSetMat("resizeAREA", ocl::imgproc::resize_oclsrc, buildOption);
            if (k.empty())
                return false;

            int xytab_size = (ssize.width + ssize.height) << 1;
            int tabofs_size = dsize.height + dsize.width + 2;

            FREE(); int* = MALLOC(int, ) _xymap_tab(xytab_size), _xyofs_tab(tabofs_size);
            FREE(); float* = MALLOC(float, ) _xyalpha_tab(xytab_size);
            int * xmap_tab = _xymap_tab, * ymap_tab = _xymap_tab + (ssize.width << 1);
            float * xalpha_tab = _xyalpha_tab, * yalpha_tab = _xyalpha_tab + (ssize.width << 1);
            int * xofs_tab = _xyofs_tab, * yofs_tab = _xyofs_tab + dsize.width + 1;

            ocl_computeResizeAreaTabs(ssize.width, dsize.width, inv_fx, xmap_tab, xalpha_tab, xofs_tab);
            ocl_computeResizeAreaTabs(ssize.height, dsize.height, inv_fy, ymap_tab, yalpha_tab, yofs_tab);

            // loading precomputed arrays to GPU
            img_t(1, xytab_size, CC_32FC1, (void *)_xyalpha_tab).copyTo(alphaOcl);
            img_t(1, xytab_size, CC_32SC1, (void *)_xymap_tab).copyTo(mapOcl);
            img_t(1, tabofs_size, CC_32SC1, (void *)_xyofs_tab).copyTo(tabofsOcl);
        }

        ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src), dstarg = ocl::KernelArg::WriteOnly(dst);

        if (is_area_fast)
            k.args(srcarg, dstarg);
        else
            k.args(srcarg, dstarg, inv_fxf, inv_fyf, ocl::KernelArg::PtrReadOnly(tabofsOcl),
                   ocl::KernelArg::PtrReadOnly(mapOcl), ocl::KernelArg::PtrReadOnly(alphaOcl));

        return k.run(2, globalsize, NULL, false);
    }

    return k.run(2, globalsize, 0, false);
}

#endif

#if IPP_VERSION_X100 >= 710
static bool ipp_resize_mt(img_t*  src, img_t*  dst,
                          double inv_scale_x, double inv_scale_y, int interpolation)
{
    CC_INSTRUMENT_REGION_IPP()

    int mode = -1;
    if (interpolation == CC_INTER_LINEAR && src->rows >= 2 && src->cols >= 2)
        mode = ippLinear;
    else if (interpolation == CC_INTER_CUBIC && src->rows >= 4 && src->cols >= 4)
        mode = ippCubic;
    else
        return false;

    bool ok = true;
    CRange range(0, src->rows);
    IPPresizeInvoker invoker(src, dst, inv_scale_x, inv_scale_y, mode, &ok);
    parallel_for_(range, invoker, CC_MAT_TOTAL(dst)/(double)(1<<16));
    if( ok )
        return true;

    return false;
}
#endif

//==================================================================================================


static void hal_resize(TypeId src_type, int cn,
            const uchar * src_data, size_t src_step, int src_width, int src_height,
            uchar * dst_data, size_t dst_step, int dst_width, int dst_height,
            double inv_scale_x, double inv_scale_y, int interpolation)
{
    CC_INSTRUMENT_REGION()

    CC_Assert((dst_width * dst_height > 0) || (inv_scale_x > 0 && inv_scale_y > 0));
    if (inv_scale_x < DBL_EPSILON || inv_scale_y < DBL_EPSILON)
    {
        inv_scale_x = static_cast<double>(dst_width) / src_width;
        inv_scale_y = static_cast<double>(dst_height) / src_height;
    }

    CALL_HAL(resize, cv_hal_resize, (src_type, cn, src_data, src_step, src_width, src_height, dst_data, dst_step, dst_width, dst_height, inv_scale_x, inv_scale_y, interpolation));

    static ResizeFunc linear_tab[] =
    {
        resizeGeneric_<
            HResizeLinear<uchar, int, short,
                CC_INTER_RESIZE_COEF_SCALE,
                HResizeLinearVec_8u32s>,
            VResizeLinear<uchar, int, short,
                FixedPtCast<int, uchar, CC_INTER_RESIZE_COEF_BITS*2>,
                VResizeLinearVec_32s8u> >::f,
        0,
        resizeGeneric_<
            HResizeLinear<ushort, float, float, 1,
                HResizeLinearVec_16u32f>,
            VResizeLinear<ushort, float, float, Cast<float, ushort>,
                VResizeLinearVec_32f16u> >::f,
        resizeGeneric_<
            HResizeLinear<short, float, float, 1,
                HResizeLinearVec_16s32f>,
            VResizeLinear<short, float, float, Cast<float, short>,
                VResizeLinearVec_32f16s> >::f,
        0,
        resizeGeneric_<
            HResizeLinear<float, float, float, 1,
                HResizeLinearVec_32f>,
            VResizeLinear<float, float, float, Cast<float, float>,
                VResizeLinearVec_32f> >::f,
        resizeGeneric_<
            HResizeLinear<double, double, float, 1,
                HResizeNoVec>,
            VResizeLinear<double, double, float, Cast<double, double>,
                VResizeNoVec> >::f,
        0
    };

    static ResizeFunc cubic_tab[] =
    {
        resizeGeneric_<
            HResizeCubic<uchar, int, short>,
            VResizeCubic<uchar, int, short,
                FixedPtCast<int, uchar, CC_INTER_RESIZE_COEF_BITS*2>,
                VResizeCubicVec_32s8u> >::f,
        0,
        resizeGeneric_<
            HResizeCubic<ushort, float, float>,
            VResizeCubic<ushort, float, float, Cast<float, ushort>,
            VResizeCubicVec_32f16u> >::f,
        resizeGeneric_<
            HResizeCubic<short, float, float>,
            VResizeCubic<short, float, float, Cast<float, short>,
            VResizeCubicVec_32f16s> >::f,
        0,
        resizeGeneric_<
            HResizeCubic<float, float, float>,
            VResizeCubic<float, float, float, Cast<float, float>,
            VResizeCubicVec_32f> >::f,
        resizeGeneric_<
            HResizeCubic<double, double, float>,
            VResizeCubic<double, double, float, Cast<double, double>,
            VResizeNoVec> >::f,
        0
    };

    static ResizeFunc lanczos4_tab[] =
    {
        resizeGeneric_<HResizeLanczos4<uchar, int, short>,
            VResizeLanczos4<uchar, int, short,
            FixedPtCast<int, uchar, CC_INTER_RESIZE_COEF_BITS*2>,
            VResizeNoVec> >::f,
        0,
        resizeGeneric_<HResizeLanczos4<ushort, float, float>,
            VResizeLanczos4<ushort, float, float, Cast<float, ushort>,
            VResizeLanczos4Vec_32f16u> >::f,
        resizeGeneric_<HResizeLanczos4<short, float, float>,
            VResizeLanczos4<short, float, float, Cast<float, short>,
            VResizeLanczos4Vec_32f16s> >::f,
        0,
        resizeGeneric_<HResizeLanczos4<float, float, float>,
            VResizeLanczos4<float, float, float, Cast<float, float>,
            VResizeLanczos4Vec_32f> >::f,
        resizeGeneric_<HResizeLanczos4<double, double, float>,
            VResizeLanczos4<double, double, float, Cast<double, double>,
            VResizeNoVec> >::f,
        0
    };

    static ResizeAreaFastFunc areafast_tab[] =
    {
        resizeAreaFast_<uchar, int, ResizeAreaFastVec<uchar, ResizeAreaFastVec_SIMD_8u> >::f,
        0,
        resizeAreaFast_<ushort, float, ResizeAreaFastVec<ushort, ResizeAreaFastVec_SIMD_16u> >::f,
        resizeAreaFast_<short, float, ResizeAreaFastVec<short, ResizeAreaFastVec_SIMD_16s> >::f,
        0,
        resizeAreaFast_<float, float, ResizeAreaFastVec_SIMD_32f>::f,
        resizeAreaFast_<double, double, ResizeAreaFastNoVec<double, double> >::f,
        0
    };

    static ResizeAreaFunc area_tab[] =
    {
        resizeArea_<uchar, float>::f, 0, resizeArea_<ushort, float>::f,
        resizeArea_<short, float>::f, 0, resizeArea_<float, float>::f,
        resizeArea_<double, double>::f, 0
    };

    TypeId depth = (src_type);
    //int cn = CC_MAT_CN(src_type);
    double scale_x = 1./inv_scale_x, scale_y = 1./inv_scale_y;

    int iscale_x = double_to_int(scale_x);
    int iscale_y = double_to_int(scale_y);

    bool is_area_fast = fabs(scale_x - iscale_x) < DBL_EPSILON &&
            fabs(scale_y - iscale_y) < DBL_EPSILON;

    CSize dsize;
    dsize.width = double_to_int(src_width*inv_scale_x);
    dsize.height = double_to_int(src_height*inv_scale_y);

    CC_Assert( dsize.width*dsize.height > 0 );

    IM_DEF(tmp, 4);
    img_t *src = cvMat(tmp++, src_height, src_width, src_type, cn, (uchar*)(src_data), src_step);
    img_t *dst = cvMat(tmp++, dsize.height, dsize.width, src_type, cn, dst_data, dst_step);

#ifdef HAVE_IPP
    int mode = -1;
    if (interpolation == CC_INTER_LINEAR && src_height >= 2 && src_width >= 2)
        mode = CC_INTER_LINEAR;
    else if (interpolation == CC_INTER_CUBIC && src_height >= 4 && src_width >= 4)
        mode = CC_INTER_CUBIC;

    const double IPP_RESIZE_EPS = 1e-10;
    double ex = fabs((double)dsize.width / src_width  - inv_scale_x) / inv_scale_x;
    double ey = fabs((double)dsize.height / src_height - inv_scale_y) / inv_scale_y;
#endif
    CC_IPP_RUN(IPP_VERSION_X100 >= 710 && ((ex < IPP_RESIZE_EPS && ey < IPP_RESIZE_EPS && depth != CC_64F) || (ex == 0 && ey == 0 && depth == CC_64F)) &&
        (interpolation == CC_INTER_LINEAR || interpolation == CC_INTER_CUBIC) &&
        !(interpolation == CC_INTER_LINEAR && is_area_fast && iscale_x == 2 && iscale_y == 2 && depth == CC_8U) &&
        mode >= 0 && (cn == 1 || cn == 3 || cn == 4) && (depth == CC_16U || depth == CC_16S || depth == CC_32F ||
        (depth == CC_64F && mode == CC_INTER_LINEAR)),
        ipp_resize_mt(src, dst, inv_scale_x, inv_scale_y, interpolation), 0)

    if( interpolation == CC_INTER_NEAREST )
    {
        resizeNN( src, dst, inv_scale_x, inv_scale_y );
        return;
    }

    int k, sx, sy, dx, dy;


    {
        // in case of scale_x && scale_y is equal to 2
        // CC_INTER_AREA (fast) also is equal to CC_INTER_LINEAR
        if( interpolation == CC_INTER_LINEAR && is_area_fast && iscale_x == 2 && iscale_y == 2 )
            interpolation = CC_INTER_AREA;

        // true "area" interpolation is only implemented for the case (scale_x <= 1 && scale_y <= 1).
        // In other cases it is emulated using some variant of bilinear interpolation
        if( interpolation == CC_INTER_AREA && scale_x >= 1 && scale_y >= 1 )
        {
            if( is_area_fast )
            {
                int area = iscale_x*iscale_y;
                size_t srcstep = src_step / CC_TYPE_SIZE(src->tid);
                int* _ofs = MALLOC(int, area + dsize.width*cn);
                int* ofs = _ofs;
                int* xofs = ofs + area;
                ResizeAreaFastFunc func = areafast_tab[depth];
                CC_Assert( func != 0 );

                for( sy = 0, k = 0; sy < iscale_y; sy++ )
                    for( sx = 0; sx < iscale_x; sx++ )
                        ofs[k++] = (int)(sy*srcstep + sx*cn);

                for( dx = 0; dx < dsize.width; dx++ )
                {
                    int j = dx * cn;
                    sx = iscale_x * j;
                    for( k = 0; k < cn; k++ )
                        xofs[j + k] = sx + k;
                }

                func( src, dst, ofs, xofs, iscale_x, iscale_y );
                FREE(_ofs);
                return;
            }

            ResizeAreaFunc func = area_tab[depth];
            CC_Assert( func != 0 && cn <= 4 );

            DecimateAlpha* _xytab = MALLOC(DecimateAlpha, (src_width + src_height)*2);
            DecimateAlpha* xtab = _xytab, *ytab = xtab + src_width*2;

            int xtab_size = computeResizeAreaTab(src_width, dsize.width, cn, scale_x, xtab);
            int ytab_size = computeResizeAreaTab(src_height, dsize.height, 1, scale_y, ytab);

            int* _tabofs = MALLOC(int, dsize.height + 1);
            int* tabofs = _tabofs;
            for( k = 0, dy = 0; k < ytab_size; k++ )
            {
                if( k == 0 || ytab[k].di != ytab[k-1].di )
                {
                    assert( ytab[k].di == dy );
                    tabofs[dy++] = k;
                }
            }
            tabofs[dy] = ytab_size;

            func( src, dst, xtab, xtab_size, ytab, ytab_size, tabofs );
            FREE(_xytab);
            FREE(_tabofs);
            return;
        }
    }

    int xmin = 0, xmax = dsize.width, width = dsize.width*cn;
    bool area_mode = interpolation == CC_INTER_AREA;
    bool fixpt = depth == CC_8U;
    float fx, fy;
    ResizeFunc func=0;
    int ksize=0, ksize2;
    if( interpolation == CC_INTER_CUBIC )
        ksize = 4, func = cubic_tab[depth];
    else if( interpolation == CC_INTER_LANCZOS4 )
        ksize = 8, func = lanczos4_tab[depth];
    else if( interpolation == CC_INTER_LINEAR || interpolation == CC_INTER_AREA )
        ksize = 2, func = linear_tab[depth];
    else
        CC_Error( CC_StsBadArg, "Unknown interpolation method" );
    ksize2 = ksize/2;

    CC_Assert( func != 0 );

    uchar* _buffer = MALLOC(uchar, (width + dsize.height)*(sizeof(int) + sizeof(float)*ksize));
    int* xofs = (int*)(uchar*)_buffer;
    int* yofs = xofs + width;
    float* alpha = (float*)(yofs + dsize.height);
    short* ialpha = (short*)alpha;
    float* beta = alpha + width*ksize;
    short* ibeta = ialpha + width*ksize;
    float cbuf[MAX_ESIZE];

    for( dx = 0; dx < dsize.width; dx++ )
    {
        if( !area_mode )
        {
            fx = (float)((dx+0.5)*scale_x - 0.5);
            sx = cFloor(fx);
            fx -= sx;
        }
        else
        {
            sx = cFloor(dx*scale_x);
            fx = (float)((dx+1) - (sx+1)*inv_scale_x);
            fx = fx <= 0 ? 0.f : fx - cFloor(fx);
        }

        if( sx < ksize2-1 )
        {
            xmin = dx+1;
            if( sx < 0 && (interpolation != CC_INTER_CUBIC && interpolation != CC_INTER_LANCZOS4))
                fx = 0, sx = 0;
        }

        if( sx + ksize2 >= src_width )
        {
            xmax = MIN( xmax, dx );
            if( sx >= src_width-1 && (interpolation != CC_INTER_CUBIC && interpolation != CC_INTER_LANCZOS4))
                fx = 0, sx = src_width-1;
        }

        for( k = 0, sx *= cn; k < cn; k++ )
            xofs[dx*cn + k] = sx + k;

        if( interpolation == CC_INTER_CUBIC )
            interpolateCubic( fx, cbuf );
        else if( interpolation == CC_INTER_LANCZOS4 )
            interpolateLanczos4( fx, cbuf );
        else
        {
            cbuf[0] = 1.f - fx;
            cbuf[1] = fx;
        }
        if( fixpt )
        {
            for( k = 0; k < ksize; k++ )
                ialpha[dx*cn*ksize + k] = float_to_short(cbuf[k]*CC_INTER_RESIZE_COEF_SCALE);
            for( ; k < cn*ksize; k++ )
                ialpha[dx*cn*ksize + k] = ialpha[dx*cn*ksize + k - ksize];
        }
        else
        {
            for( k = 0; k < ksize; k++ )
                alpha[dx*cn*ksize + k] = cbuf[k];
            for( ; k < cn*ksize; k++ )
                alpha[dx*cn*ksize + k] = alpha[dx*cn*ksize + k - ksize];
        }
    }

    for( dy = 0; dy < dsize.height; dy++ )
    {
        if( !area_mode )
        {
            fy = (float)((dy+0.5)*scale_y - 0.5);
            sy = cFloor(fy);
            fy -= sy;
        }
        else
        {
            sy = cFloor(dy*scale_y);
            fy = (float)((dy+1) - (sy+1)*inv_scale_y);
            fy = fy <= 0 ? 0.f : fy - cFloor(fy);
        }

        yofs[dy] = sy;
        if( interpolation == CC_INTER_CUBIC )
            interpolateCubic( fy, cbuf );
        else if( interpolation == CC_INTER_LANCZOS4 )
            interpolateLanczos4( fy, cbuf );
        else
        {
            cbuf[0] = 1.f - fy;
            cbuf[1] = fy;
        }

        if( fixpt )
        {
            for( k = 0; k < ksize; k++ )
                ibeta[dy*ksize + k] = float_to_short(cbuf[k]*CC_INTER_RESIZE_COEF_SCALE);
        }
        else
        {
            for( k = 0; k < ksize; k++ )
                beta[dy*ksize + k] = cbuf[k];
        }
    }

    func( src, dst, xofs, fixpt ? (void*)ialpha : (void*)alpha, yofs,
          fixpt ? (void*)ibeta : (void*)beta, xmin, xmax, ksize );
    FREE(_buffer);
}

//==================================================================================================

static void resize( const img_t* src, img_t* dst, CSize dsize, double inv_scale_x, double inv_scale_y, int interpolation )
{
    CC_INSTRUMENT_REGION()

    CSize ssize = cvGetSize(src);

    CC_Assert( ssize.width > 0 && ssize.height > 0 );
    CC_Assert( CC_SIZE_AREA(dsize) > 0 || (inv_scale_x > 0 && inv_scale_y > 0) );
    if( CC_SIZE_AREA(dsize) == 0 )
    {
        dsize.width = double_to_int(ssize.width*inv_scale_x);
        dsize.height = double_to_int(ssize.height*inv_scale_y);
        CC_Assert( CC_SIZE_AREA(dsize) > 0 );
    }
    else
    {
        inv_scale_x = (double)dsize.width/ssize.width;
        inv_scale_y = (double)dsize.height/ssize.height;
    }

    CC_OCL_RUN(src.dims() <= 2 && dst.isUMat() && src->cols > 10 && src->rows > 10,
               ocl_resize(src, dst, dsize, inv_scale_x, inv_scale_y, interpolation))

    cvSetMat(dst, dsize.height, dsize.width, src->tid, CC_MAT_CN(src));
    
    if ( CC_SIZE_ISEQ(dsize, ssize) )
    {
        // Source and destination are of same size. Use simple copy.
        cvCopy(src, dst);
        return;
    }

    hal_resize(src->tid, CC_MAT_CN(src), src->tt.data, src->step, src->cols, src->rows, dst->tt.data, dst->step, dst->cols, dst->rows, inv_scale_x, inv_scale_y, interpolation);
}

/****************************************************************************************\
*                       General warping (affine, perspective, remap)                     *
\****************************************************************************************/

template<typename T>
struct remapNearest {
static void f( const img_t* src, img_t* dst, const img_t* _xy,
                          int borderType, const CScalar& _borderValue )
{
    CSize ssize = cvGetSize(src), dsize = cvGetSize(dst);
    int cn = CC_MAT_CN(src);
    const T* S0 = (T*)(src->tt.data);
    size_t sstep = src->step/sizeof(S0[0]);
    T cval[4];
    (saturate_cast(cval[0], _borderValue.val[0]),
        saturate_cast(cval[1], _borderValue.val[1]),
        saturate_cast(cval[2], _borderValue.val[2]),
        saturate_cast(cval[3], _borderValue.val[3]));
    int dx, dy;

    unsigned width1 = ssize.width, height1 = ssize.height;

    if( CC_IS_CONT_MAT(dst) && CC_IS_CONT_MAT(_xy) )
    {
        dsize.width *= dsize.height;
        dsize.height = 1;
    }

    for( dy = 0; dy < dsize.height; dy++ )
    {
        T* D = (T*)(dst->tt.data + dst->step * dy);
        const short* XY = (short*)(_xy->tt.data + _xy->step * dy);

        if( cn == 1 )
        {
            for( dx = 0; dx < dsize.width; dx++ )
            {
                int sx = XY[dx*2], sy = XY[dx*2+1];
                if( (unsigned)sx < width1 && (unsigned)sy < height1 )
                    D[dx] = S0[sy*sstep + sx];
                else
                {
                    if( borderType == CC_BORDER_REPLICATE )
                    {
                        sx = clip(sx, 0, ssize.width);
                        sy = clip(sy, 0, ssize.height);
                        D[dx] = S0[sy*sstep + sx];
                    }
                    else if( borderType == CC_BORDER_CONSTANT )
                        D[dx] = cval[0];
                    else if( borderType != CC_BORDER_TRANSPARENT )
                    {
                        sx = cBorderInterpolate(sx, ssize.width, borderType);
                        sy = cBorderInterpolate(sy, ssize.height, borderType);
                        D[dx] = S0[sy*sstep + sx];
                    }
                }
            }
        }
        else
        {
            for( dx = 0; dx < dsize.width; dx++, D += cn )
            {
                int sx = XY[dx*2], sy = XY[dx*2+1], k;
                const T *S;
                if( (unsigned)sx < width1 && (unsigned)sy < height1 )
                {
                    if( cn == 3 )
                    {
                        S = S0 + sy*sstep + sx*3;
                        D[0] = S[0], D[1] = S[1], D[2] = S[2];
                    }
                    else if( cn == 4 )
                    {
                        S = S0 + sy*sstep + sx*4;
                        D[0] = S[0], D[1] = S[1], D[2] = S[2], D[3] = S[3];
                    }
                    else
                    {
                        S = S0 + sy*sstep + sx*cn;
                        for( k = 0; k < cn; k++ )
                            D[k] = S[k];
                    }
                }
                else if( borderType != CC_BORDER_TRANSPARENT )
                {
                    if( borderType == CC_BORDER_REPLICATE )
                    {
                        sx = clip(sx, 0, ssize.width);
                        sy = clip(sy, 0, ssize.height);
                        S = S0 + sy*sstep + sx*cn;
                    }
                    else if( borderType == CC_BORDER_CONSTANT )
                        S = &cval[0];
                    else
                    {
                        sx = cBorderInterpolate(sx, ssize.width, borderType);
                        sy = cBorderInterpolate(sy, ssize.height, borderType);
                        S = S0 + sy*sstep + sx*cn;
                    }
                    for( k = 0; k < cn; k++ )
                        D[k] = S[k];
                }
            }
        }
    }
}
};


struct RemapNoVec
{
    int operator()( const img_t*, void*, const short*, const ushort*,
                    const void*, int ) const { return 0; }
};

#if CC_SSE2_

struct RemapVec_8u
{
    int operator()( const img_t* src, void* dst, const short* XY,
                    const ushort* FXY, const void* _wtab, int width ) const
    {
        int cn = CC_MAT_CN(src), x = 0, sstep = (int)src->step;

        if( (cn != 1 && cn != 3 && cn != 4) || !checkHardwareSupport(CC_CPU_SSE2) ||
            sstep > 0x8000 )
            return 0;

        const uchar *S0 = src->tt.data, *S1 = src.ptr(1);
        const short* wtab = cn == 1 ? (const short*)_wtab : &BilinearTab_iC4[0][0][0];
        uchar* D = (uchar*)dst;
        __m128i delta = _mm_set1_epi32(CC_INTER_REMAP_COEF_SCALE/2);
        __m128i xy2ofs = _mm_set1_epi32(cn + (sstep << 16));
        __m128i z = _mm_setzero_si128();
        int CC_DECL_ALIGNED(16) iofs0[4], iofs1[4];

        if( cn == 1 )
        {
            for( ; x <= width - 8; x += 8 )
            {
                __m128i xy0 = _mm_loadu_si128( (const __m128i*)(XY + x*2));
                __m128i xy1 = _mm_loadu_si128( (const __m128i*)(XY + x*2 + 8));
                __m128i v0, v1, v2, v3, a0, a1, b0, b1;
                unsigned i0, i1;

                xy0 = _mm_madd_epi16( xy0, xy2ofs );
                xy1 = _mm_madd_epi16( xy1, xy2ofs );
                _mm_store_si128( (__m128i*)iofs0, xy0 );
                _mm_store_si128( (__m128i*)iofs1, xy1 );

                i0 = *(ushort*)(S0 + iofs0[0]) + (*(ushort*)(S0 + iofs0[1]) << 16);
                i1 = *(ushort*)(S0 + iofs0[2]) + (*(ushort*)(S0 + iofs0[3]) << 16);
                v0 = _mm_unpacklo_epi32(_mm_cvtsi32_si128(i0), _mm_cvtsi32_si128(i1));
                i0 = *(ushort*)(S1 + iofs0[0]) + (*(ushort*)(S1 + iofs0[1]) << 16);
                i1 = *(ushort*)(S1 + iofs0[2]) + (*(ushort*)(S1 + iofs0[3]) << 16);
                v1 = _mm_unpacklo_epi32(_mm_cvtsi32_si128(i0), _mm_cvtsi32_si128(i1));
                v0 = _mm_unpacklo_epi8(v0, z);
                v1 = _mm_unpacklo_epi8(v1, z);

                a0 = _mm_unpacklo_epi32(_mm_loadl_epi64((__m128i*)(wtab+FXY[x]*4)),
                                        _mm_loadl_epi64((__m128i*)(wtab+FXY[x+1]*4)));
                a1 = _mm_unpacklo_epi32(_mm_loadl_epi64((__m128i*)(wtab+FXY[x+2]*4)),
                                        _mm_loadl_epi64((__m128i*)(wtab+FXY[x+3]*4)));
                b0 = _mm_unpacklo_epi64(a0, a1);
                b1 = _mm_unpackhi_epi64(a0, a1);
                v0 = _mm_madd_epi16(v0, b0);
                v1 = _mm_madd_epi16(v1, b1);
                v0 = _mm_add_epi32(_mm_add_epi32(v0, v1), delta);

                i0 = *(ushort*)(S0 + iofs1[0]) + (*(ushort*)(S0 + iofs1[1]) << 16);
                i1 = *(ushort*)(S0 + iofs1[2]) + (*(ushort*)(S0 + iofs1[3]) << 16);
                v2 = _mm_unpacklo_epi32(_mm_cvtsi32_si128(i0), _mm_cvtsi32_si128(i1));
                i0 = *(ushort*)(S1 + iofs1[0]) + (*(ushort*)(S1 + iofs1[1]) << 16);
                i1 = *(ushort*)(S1 + iofs1[2]) + (*(ushort*)(S1 + iofs1[3]) << 16);
                v3 = _mm_unpacklo_epi32(_mm_cvtsi32_si128(i0), _mm_cvtsi32_si128(i1));
                v2 = _mm_unpacklo_epi8(v2, z);
                v3 = _mm_unpacklo_epi8(v3, z);

                a0 = _mm_unpacklo_epi32(_mm_loadl_epi64((__m128i*)(wtab+FXY[x+4]*4)),
                                        _mm_loadl_epi64((__m128i*)(wtab+FXY[x+5]*4)));
                a1 = _mm_unpacklo_epi32(_mm_loadl_epi64((__m128i*)(wtab+FXY[x+6]*4)),
                                        _mm_loadl_epi64((__m128i*)(wtab+FXY[x+7]*4)));
                b0 = _mm_unpacklo_epi64(a0, a1);
                b1 = _mm_unpackhi_epi64(a0, a1);
                v2 = _mm_madd_epi16(v2, b0);
                v3 = _mm_madd_epi16(v3, b1);
                v2 = _mm_add_epi32(_mm_add_epi32(v2, v3), delta);

                v0 = _mm_srai_epi32(v0, CC_INTER_REMAP_COEF_BITS);
                v2 = _mm_srai_epi32(v2, CC_INTER_REMAP_COEF_BITS);
                v0 = _mm_packus_epi16(_mm_packs_epi32(v0, v2), z);
                _mm_storel_epi64( (__m128i*)(D + x), v0 );
            }
        }
        else if( cn == 3 )
        {
            for( ; x <= width - 5; x += 4, D += 12 )
            {
                __m128i xy0 = _mm_loadu_si128( (const __m128i*)(XY + x*2));
                __m128i u0, v0, u1, v1;

                xy0 = _mm_madd_epi16( xy0, xy2ofs );
                _mm_store_si128( (__m128i*)iofs0, xy0 );
                const __m128i *w0, *w1;
                w0 = (const __m128i*)(wtab + FXY[x]*16);
                w1 = (const __m128i*)(wtab + FXY[x+1]*16);

                u0 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(int*)(S0 + iofs0[0])),
                                       _mm_cvtsi32_si128(*(int*)(S0 + iofs0[0] + 3)));
                v0 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(int*)(S1 + iofs0[0])),
                                       _mm_cvtsi32_si128(*(int*)(S1 + iofs0[0] + 3)));
                u1 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(int*)(S0 + iofs0[1])),
                                       _mm_cvtsi32_si128(*(int*)(S0 + iofs0[1] + 3)));
                v1 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(int*)(S1 + iofs0[1])),
                                       _mm_cvtsi32_si128(*(int*)(S1 + iofs0[1] + 3)));
                u0 = _mm_unpacklo_epi8(u0, z);
                v0 = _mm_unpacklo_epi8(v0, z);
                u1 = _mm_unpacklo_epi8(u1, z);
                v1 = _mm_unpacklo_epi8(v1, z);
                u0 = _mm_add_epi32(_mm_madd_epi16(u0, w0[0]), _mm_madd_epi16(v0, w0[1]));
                u1 = _mm_add_epi32(_mm_madd_epi16(u1, w1[0]), _mm_madd_epi16(v1, w1[1]));
                u0 = _mm_srai_epi32(_mm_add_epi32(u0, delta), CC_INTER_REMAP_COEF_BITS);
                u1 = _mm_srai_epi32(_mm_add_epi32(u1, delta), CC_INTER_REMAP_COEF_BITS);
                u0 = _mm_slli_si128(u0, 4);
                u0 = _mm_packs_epi32(u0, u1);
                u0 = _mm_packus_epi16(u0, u0);
                _mm_storel_epi64((__m128i*)D, _mm_srli_si128(u0,1));

                w0 = (const __m128i*)(wtab + FXY[x+2]*16);
                w1 = (const __m128i*)(wtab + FXY[x+3]*16);

                u0 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(int*)(S0 + iofs0[2])),
                                       _mm_cvtsi32_si128(*(int*)(S0 + iofs0[2] + 3)));
                v0 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(int*)(S1 + iofs0[2])),
                                       _mm_cvtsi32_si128(*(int*)(S1 + iofs0[2] + 3)));
                u1 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(int*)(S0 + iofs0[3])),
                                       _mm_cvtsi32_si128(*(int*)(S0 + iofs0[3] + 3)));
                v1 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(int*)(S1 + iofs0[3])),
                                       _mm_cvtsi32_si128(*(int*)(S1 + iofs0[3] + 3)));
                u0 = _mm_unpacklo_epi8(u0, z);
                v0 = _mm_unpacklo_epi8(v0, z);
                u1 = _mm_unpacklo_epi8(u1, z);
                v1 = _mm_unpacklo_epi8(v1, z);
                u0 = _mm_add_epi32(_mm_madd_epi16(u0, w0[0]), _mm_madd_epi16(v0, w0[1]));
                u1 = _mm_add_epi32(_mm_madd_epi16(u1, w1[0]), _mm_madd_epi16(v1, w1[1]));
                u0 = _mm_srai_epi32(_mm_add_epi32(u0, delta), CC_INTER_REMAP_COEF_BITS);
                u1 = _mm_srai_epi32(_mm_add_epi32(u1, delta), CC_INTER_REMAP_COEF_BITS);
                u0 = _mm_slli_si128(u0, 4);
                u0 = _mm_packs_epi32(u0, u1);
                u0 = _mm_packus_epi16(u0, u0);
                _mm_storel_epi64((__m128i*)(D + 6), _mm_srli_si128(u0,1));
            }
        }
        else if( cn == 4 )
        {
            for( ; x <= width - 4; x += 4, D += 16 )
            {
                __m128i xy0 = _mm_loadu_si128( (const __m128i*)(XY + x*2));
                __m128i u0, v0, u1, v1;

                xy0 = _mm_madd_epi16( xy0, xy2ofs );
                _mm_store_si128( (__m128i*)iofs0, xy0 );
                const __m128i *w0, *w1;
                w0 = (const __m128i*)(wtab + FXY[x]*16);
                w1 = (const __m128i*)(wtab + FXY[x+1]*16);

                u0 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(int*)(S0 + iofs0[0])),
                                       _mm_cvtsi32_si128(*(int*)(S0 + iofs0[0] + 4)));
                v0 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(int*)(S1 + iofs0[0])),
                                       _mm_cvtsi32_si128(*(int*)(S1 + iofs0[0] + 4)));
                u1 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(int*)(S0 + iofs0[1])),
                                       _mm_cvtsi32_si128(*(int*)(S0 + iofs0[1] + 4)));
                v1 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(int*)(S1 + iofs0[1])),
                                       _mm_cvtsi32_si128(*(int*)(S1 + iofs0[1] + 4)));
                u0 = _mm_unpacklo_epi8(u0, z);
                v0 = _mm_unpacklo_epi8(v0, z);
                u1 = _mm_unpacklo_epi8(u1, z);
                v1 = _mm_unpacklo_epi8(v1, z);
                u0 = _mm_add_epi32(_mm_madd_epi16(u0, w0[0]), _mm_madd_epi16(v0, w0[1]));
                u1 = _mm_add_epi32(_mm_madd_epi16(u1, w1[0]), _mm_madd_epi16(v1, w1[1]));
                u0 = _mm_srai_epi32(_mm_add_epi32(u0, delta), CC_INTER_REMAP_COEF_BITS);
                u1 = _mm_srai_epi32(_mm_add_epi32(u1, delta), CC_INTER_REMAP_COEF_BITS);
                u0 = _mm_packs_epi32(u0, u1);
                u0 = _mm_packus_epi16(u0, u0);
                _mm_storel_epi64((__m128i*)D, u0);

                w0 = (const __m128i*)(wtab + FXY[x+2]*16);
                w1 = (const __m128i*)(wtab + FXY[x+3]*16);

                u0 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(int*)(S0 + iofs0[2])),
                                       _mm_cvtsi32_si128(*(int*)(S0 + iofs0[2] + 4)));
                v0 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(int*)(S1 + iofs0[2])),
                                       _mm_cvtsi32_si128(*(int*)(S1 + iofs0[2] + 4)));
                u1 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(int*)(S0 + iofs0[3])),
                                       _mm_cvtsi32_si128(*(int*)(S0 + iofs0[3] + 4)));
                v1 = _mm_unpacklo_epi8(_mm_cvtsi32_si128(*(int*)(S1 + iofs0[3])),
                                       _mm_cvtsi32_si128(*(int*)(S1 + iofs0[3] + 4)));
                u0 = _mm_unpacklo_epi8(u0, z);
                v0 = _mm_unpacklo_epi8(v0, z);
                u1 = _mm_unpacklo_epi8(u1, z);
                v1 = _mm_unpacklo_epi8(v1, z);
                u0 = _mm_add_epi32(_mm_madd_epi16(u0, w0[0]), _mm_madd_epi16(v0, w0[1]));
                u1 = _mm_add_epi32(_mm_madd_epi16(u1, w1[0]), _mm_madd_epi16(v1, w1[1]));
                u0 = _mm_srai_epi32(_mm_add_epi32(u0, delta), CC_INTER_REMAP_COEF_BITS);
                u1 = _mm_srai_epi32(_mm_add_epi32(u1, delta), CC_INTER_REMAP_COEF_BITS);
                u0 = _mm_packs_epi32(u0, u1);
                u0 = _mm_packus_epi16(u0, u0);
                _mm_storel_epi64((__m128i*)(D + 8), u0);
            }
        }

        return x;
    }
};

#else

typedef RemapNoVec RemapVec_8u;

#endif


template<class CastOp, class VecOp, typename AT>
struct remapBilinear {
static void f( const img_t* src, img_t* dst, const img_t* _xy,
                           const img_t* _fxy, const void* _wtab,
                           int borderType, const CScalar& _borderValue )
{
    typedef typename CastOp::rtype T;
    typedef typename CastOp::type1 WT;
    CSize ssize = cvGetSize(src), dsize = cvGetSize(dst);
    int k, cn = CC_MAT_CN(src);
    const AT* wtab = (const AT*)_wtab;
    const T* S0 = (const T*)(src->tt.data);
    size_t sstep = src->step/sizeof(S0[0]);
    T cval[CC_CN_MAX];
    int dx, dy;
    CastOp castOp;
    VecOp vecOp;

    for( k = 0; k < cn; k++ )
        saturate_cast(cval[k], _borderValue[k & 3]);

    unsigned width1 = MAX(ssize.width-1, 0), height1 = MAX(ssize.height-1, 0);
    CC_Assert( CC_SIZE_AREA(ssize) > 0 );
#if CC_SSE2_
    if( src->tid == CC_8UC3 )
        width1 = MAX(ssize.width-2, 0);
#endif

    for( dy = 0; dy < dsize.height; dy++ )
    {
        T* D = img_row_(T, dst, dy);
        const short* XY = img_row_(short, _xy, dy);
        const ushort* FXY = img_row_(ushort, _fxy, dy);
        int X0 = 0;
        bool prevInlier = false;

        for( dx = 0; dx <= dsize.width; dx++ )
        {
            bool curInlier = dx < dsize.width ?
                (unsigned)XY[dx*2] < width1 &&
                (unsigned)XY[dx*2+1] < height1 : !prevInlier;
            if( curInlier == prevInlier )
                continue;

            int X1 = dx;
            dx = X0;
            X0 = X1;
            prevInlier = curInlier;

            if( !curInlier )
            {
                int len = vecOp( src, D, XY + dx*2, FXY + dx, wtab, X1 - dx );
                D += len*cn;
                dx += len;

                if( cn == 1 )
                {
                    for( ; dx < X1; dx++, D++ )
                    {
                        int sx = XY[dx*2], sy = XY[dx*2+1];
                        const AT* w = wtab + FXY[dx]*4;
                        const T* S = S0 + sy*sstep + sx;
                        *D = castOp((WT)(S[0]*w[0] + S[1]*w[1] + S[sstep]*w[2] + S[sstep+1]*w[3]));
                    }
                }
                else if( cn == 2 )
                    for( ; dx < X1; dx++, D += 2 )
                    {
                        int sx = XY[dx*2], sy = XY[dx*2+1];
                        const AT* w = wtab + FXY[dx]*4;
                        const T* S = S0 + sy*sstep + sx*2;
                        WT t0 = S[0]*w[0] + S[2]*w[1] + S[sstep]*w[2] + S[sstep+2]*w[3];
                        WT t1 = S[1]*w[0] + S[3]*w[1] + S[sstep+1]*w[2] + S[sstep+3]*w[3];
                        D[0] = castOp(t0); D[1] = castOp(t1);
                    }
                else if( cn == 3 )
                    for( ; dx < X1; dx++, D += 3 )
                    {
                        int sx = XY[dx*2], sy = XY[dx*2+1];
                        const AT* w = wtab + FXY[dx]*4;
                        const T* S = S0 + sy*sstep + sx*3;
                        WT t0 = S[0]*w[0] + S[3]*w[1] + S[sstep]*w[2] + S[sstep+3]*w[3];
                        WT t1 = S[1]*w[0] + S[4]*w[1] + S[sstep+1]*w[2] + S[sstep+4]*w[3];
                        WT t2 = S[2]*w[0] + S[5]*w[1] + S[sstep+2]*w[2] + S[sstep+5]*w[3];
                        D[0] = castOp(t0); D[1] = castOp(t1); D[2] = castOp(t2);
                    }
                else if( cn == 4 )
                    for( ; dx < X1; dx++, D += 4 )
                    {
                        int sx = XY[dx*2], sy = XY[dx*2+1];
                        const AT* w = wtab + FXY[dx]*4;
                        const T* S = S0 + sy*sstep + sx*4;
                        WT t0 = S[0]*w[0] + S[4]*w[1] + S[sstep]*w[2] + S[sstep+4]*w[3];
                        WT t1 = S[1]*w[0] + S[5]*w[1] + S[sstep+1]*w[2] + S[sstep+5]*w[3];
                        D[0] = castOp(t0); D[1] = castOp(t1);
                        t0 = S[2]*w[0] + S[6]*w[1] + S[sstep+2]*w[2] + S[sstep+6]*w[3];
                        t1 = S[3]*w[0] + S[7]*w[1] + S[sstep+3]*w[2] + S[sstep+7]*w[3];
                        D[2] = castOp(t0); D[3] = castOp(t1);
                    }
                else
                    for( ; dx < X1; dx++, D += cn )
                    {
                        int sx = XY[dx*2], sy = XY[dx*2+1];
                        const AT* w = wtab + FXY[dx]*4;
                        const T* S = S0 + sy*sstep + sx*cn;
                        for( k = 0; k < cn; k++ )
                        {
                            WT t0 = S[k]*w[0] + S[k+cn]*w[1] + S[sstep+k]*w[2] + S[sstep+k+cn]*w[3];
                            D[k] = castOp(t0);
                        }
                    }
            }
            else
            {
                if( borderType == CC_BORDER_TRANSPARENT && cn != 3 )
                {
                    D += (X1 - dx)*cn;
                    dx = X1;
                    continue;
                }

                if( cn == 1 )
                    for( ; dx < X1; dx++, D++ )
                    {
                        int sx = XY[dx*2], sy = XY[dx*2+1];
                        if( borderType == CC_BORDER_CONSTANT &&
                            (sx >= ssize.width || sx+1 < 0 ||
                             sy >= ssize.height || sy+1 < 0) )
                        {
                            D[0] = cval[0];
                        }
                        else
                        {
                            int sx0, sx1, sy0, sy1;
                            T v0, v1, v2, v3;
                            const AT* w = wtab + FXY[dx]*4;
                            if( borderType == CC_BORDER_REPLICATE )
                            {
                                sx0 = clip(sx, 0, ssize.width);
                                sx1 = clip(sx+1, 0, ssize.width);
                                sy0 = clip(sy, 0, ssize.height);
                                sy1 = clip(sy+1, 0, ssize.height);
                                v0 = S0[sy0*sstep + sx0];
                                v1 = S0[sy0*sstep + sx1];
                                v2 = S0[sy1*sstep + sx0];
                                v3 = S0[sy1*sstep + sx1];
                            }
                            else
                            {
                                sx0 = cBorderInterpolate(sx, ssize.width, borderType);
                                sx1 = cBorderInterpolate(sx+1, ssize.width, borderType);
                                sy0 = cBorderInterpolate(sy, ssize.height, borderType);
                                sy1 = cBorderInterpolate(sy+1, ssize.height, borderType);
                                v0 = sx0 >= 0 && sy0 >= 0 ? S0[sy0*sstep + sx0] : cval[0];
                                v1 = sx1 >= 0 && sy0 >= 0 ? S0[sy0*sstep + sx1] : cval[0];
                                v2 = sx0 >= 0 && sy1 >= 0 ? S0[sy1*sstep + sx0] : cval[0];
                                v3 = sx1 >= 0 && sy1 >= 0 ? S0[sy1*sstep + sx1] : cval[0];
                            }
                            D[0] = castOp(WT(v0*w[0] + v1*w[1] + v2*w[2] + v3*w[3]));
                        }
                    }
                else
                    for( ; dx < X1; dx++, D += cn )
                    {
                        int sx = XY[dx*2], sy = XY[dx*2+1];
                        if( borderType == CC_BORDER_CONSTANT &&
                            (sx >= ssize.width || sx+1 < 0 ||
                             sy >= ssize.height || sy+1 < 0) )
                        {
                            for( k = 0; k < cn; k++ )
                                D[k] = cval[k];
                        }
                        else
                        {
                            int sx0, sx1, sy0, sy1;
                            const T *v0, *v1, *v2, *v3;
                            const AT* w = wtab + FXY[dx]*4;
                            if( borderType == CC_BORDER_REPLICATE )
                            {
                                sx0 = clip(sx, 0, ssize.width);
                                sx1 = clip(sx+1, 0, ssize.width);
                                sy0 = clip(sy, 0, ssize.height);
                                sy1 = clip(sy+1, 0, ssize.height);
                                v0 = S0 + sy0*sstep + sx0*cn;
                                v1 = S0 + sy0*sstep + sx1*cn;
                                v2 = S0 + sy1*sstep + sx0*cn;
                                v3 = S0 + sy1*sstep + sx1*cn;
                            }
                            else if( borderType == CC_BORDER_TRANSPARENT &&
                                ((unsigned)sx >= (unsigned)(ssize.width-1) ||
                                (unsigned)sy >= (unsigned)(ssize.height-1)))
                                continue;
                            else
                            {
                                sx0 = cBorderInterpolate(sx, ssize.width, borderType);
                                sx1 = cBorderInterpolate(sx+1, ssize.width, borderType);
                                sy0 = cBorderInterpolate(sy, ssize.height, borderType);
                                sy1 = cBorderInterpolate(sy+1, ssize.height, borderType);
                                v0 = sx0 >= 0 && sy0 >= 0 ? S0 + sy0*sstep + sx0*cn : &cval[0];
                                v1 = sx1 >= 0 && sy0 >= 0 ? S0 + sy0*sstep + sx1*cn : &cval[0];
                                v2 = sx0 >= 0 && sy1 >= 0 ? S0 + sy1*sstep + sx0*cn : &cval[0];
                                v3 = sx1 >= 0 && sy1 >= 0 ? S0 + sy1*sstep + sx1*cn : &cval[0];
                            }
                            for( k = 0; k < cn; k++ )
                                D[k] = castOp(WT(v0[k]*w[0] + v1[k]*w[1] + v2[k]*w[2] + v3[k]*w[3]));
                        }
                    }
            }
        }
    }
}
};

template<class CastOp, typename AT, int ONE>
struct remapBicubic {
static void f( const img_t* src, img_t* dst, const img_t* _xy,
                          const img_t* _fxy, const void* _wtab,
                          int borderType, const CScalar& _borderValue )
{
    typedef typename CastOp::rtype T;
    typedef typename CastOp::type1 WT;
    CSize ssize = cvGetSize(src), dsize = cvGetSize(dst);
    int cn = CC_MAT_CN(src);
    const AT* wtab = (const AT*)_wtab;
    const T* S0 = img_ptr_(T, src);
    size_t sstep = src->step/sizeof(S0[0]);
    T cval[4];
    (saturate_cast(cval[0], _borderValue[0]),
        saturate_cast(cval[1], _borderValue[1]),
        saturate_cast(cval[2], _borderValue[2]),
        saturate_cast(cval[3], _borderValue[3]));
    int dx, dy;
    CastOp castOp;
    int borderType1 = borderType != CC_BORDER_TRANSPARENT ? borderType : CC_BORDER_REFLECT_101;

    unsigned width1 = MAX(ssize.width-3, 0), height1 = MAX(ssize.height-3, 0);

    if( CC_IS_CONT_MAT(dst) && CC_IS_CONT_MAT(_xy) && CC_IS_CONT_MAT(_fxy) )
    {
        dsize.width *= dsize.height;
        dsize.height = 1;
    }

    for( dy = 0; dy < dsize.height; dy++ )
    {
        T* D = img_row_(T, dst, dy);
        const short* XY = img_row_(short, _xy, dy);
        const ushort* FXY = img_row_(ushort, _fxy, dy);

        for( dx = 0; dx < dsize.width; dx++, D += cn )
        {
            int sx = XY[dx*2]-1, sy = XY[dx*2+1]-1;
            const AT* w = wtab + FXY[dx]*16;
            int i, k;
            if( (unsigned)sx < width1 && (unsigned)sy < height1 )
            {
                const T* S = S0 + sy*sstep + sx*cn;
                for( k = 0; k < cn; k++ )
                {
                    WT sum = S[0]*w[0] + S[cn]*w[1] + S[cn*2]*w[2] + S[cn*3]*w[3];
                    S += sstep;
                    sum += S[0]*w[4] + S[cn]*w[5] + S[cn*2]*w[6] + S[cn*3]*w[7];
                    S += sstep;
                    sum += S[0]*w[8] + S[cn]*w[9] + S[cn*2]*w[10] + S[cn*3]*w[11];
                    S += sstep;
                    sum += S[0]*w[12] + S[cn]*w[13] + S[cn*2]*w[14] + S[cn*3]*w[15];
                    S += 1 - sstep*3;
                    D[k] = castOp(sum);
                }
            }
            else
            {
                int x[4], y[4];
                if( borderType == CC_BORDER_TRANSPARENT &&
                    ((unsigned)(sx+1) >= (unsigned)ssize.width ||
                    (unsigned)(sy+1) >= (unsigned)ssize.height) )
                    continue;

                if( borderType1 == CC_BORDER_CONSTANT &&
                    (sx >= ssize.width || sx+4 <= 0 ||
                    sy >= ssize.height || sy+4 <= 0))
                {
                    for( k = 0; k < cn; k++ )
                        D[k] = cval[k];
                    continue;
                }

                for( i = 0; i < 4; i++ )
                {
                    x[i] = cBorderInterpolate(sx + i, ssize.width, borderType1)*cn;
                    y[i] = cBorderInterpolate(sy + i, ssize.height, borderType1);
                }

                for( k = 0; k < cn; k++, S0++, w -= 16 )
                {
                    WT cv = cval[k], sum = cv*ONE;
                    for( i = 0; i < 4; i++, w += 4 )
                    {
                        int yi = y[i];
                        const T* S = S0 + yi*sstep;
                        if( yi < 0 )
                            continue;
                        if( x[0] >= 0 )
                            sum += (S[x[0]] - cv)*w[0];
                        if( x[1] >= 0 )
                            sum += (S[x[1]] - cv)*w[1];
                        if( x[2] >= 0 )
                            sum += (S[x[2]] - cv)*w[2];
                        if( x[3] >= 0 )
                            sum += (S[x[3]] - cv)*w[3];
                    }
                    D[k] = castOp(sum);
                }
                S0 -= cn;
            }
        }
    }
}
};

template<class CastOp, typename AT, int ONE>
struct remapLanczos4 {
static void f( const img_t* src, img_t* dst, const img_t* _xy,
                           const img_t* _fxy, const void* _wtab,
                           int borderType, const CScalar& _borderValue )
{
    typedef typename CastOp::rtype T;
    typedef typename CastOp::type1 WT;
    CSize ssize = cvGetSize(src), dsize = cvGetSize(dst);
    int cn = CC_MAT_CN(src);
    const AT* wtab = (const AT*)_wtab;
    const T* S0 = img_ptr_(T, src);
    size_t sstep = src->step/sizeof(S0[0]);
    T cval[4];
    (saturate_cast(cval[0], _borderValue[0]),
        saturate_cast(cval[1], _borderValue[1]),
        saturate_cast(cval[2], _borderValue[2]),
        saturate_cast(cval[3], _borderValue[3]));
    int dx, dy;
    CastOp castOp;
    int borderType1 = borderType != CC_BORDER_TRANSPARENT ? borderType : CC_BORDER_REFLECT_101;

    unsigned width1 = MAX(ssize.width-7, 0), height1 = MAX(ssize.height-7, 0);

    if( CC_IS_CONT_MAT(dst) && CC_IS_CONT_MAT(_xy) && CC_IS_CONT_MAT(_fxy) )
    {
        dsize.width *= dsize.height;
        dsize.height = 1;
    }

    for( dy = 0; dy < dsize.height; dy++ )
    {
        T* D = img_row_(T, dst, dy);
        const short* XY = img_row_(short, _xy, dy);
        const ushort* FXY = img_row_(ushort, _fxy, dy);

        for( dx = 0; dx < dsize.width; dx++, D += cn )
        {
            int sx = XY[dx*2]-3, sy = XY[dx*2+1]-3;
            const AT* w = wtab + FXY[dx]*64;
            const T* S = S0 + sy*sstep + sx*cn;
            int i, k;
            if( (unsigned)sx < width1 && (unsigned)sy < height1 )
            {
                for( k = 0; k < cn; k++ )
                {
                    WT sum = 0;
                    for( int r = 0; r < 8; r++, S += sstep, w += 8 )
                        sum += S[0]*w[0] + S[cn]*w[1] + S[cn*2]*w[2] + S[cn*3]*w[3] +
                            S[cn*4]*w[4] + S[cn*5]*w[5] + S[cn*6]*w[6] + S[cn*7]*w[7];
                    w -= 64;
                    S -= sstep*8 - 1;
                    D[k] = castOp(sum);
                }
            }
            else
            {
                int x[8], y[8];
                if( borderType == CC_BORDER_TRANSPARENT &&
                    ((unsigned)(sx+3) >= (unsigned)ssize.width ||
                    (unsigned)(sy+3) >= (unsigned)ssize.height) )
                    continue;

                if( borderType1 == CC_BORDER_CONSTANT &&
                    (sx >= ssize.width || sx+8 <= 0 ||
                    sy >= ssize.height || sy+8 <= 0))
                {
                    for( k = 0; k < cn; k++ )
                        D[k] = cval[k];
                    continue;
                }

                for( i = 0; i < 8; i++ )
                {
                    x[i] = cBorderInterpolate(sx + i, ssize.width, borderType1)*cn;
                    y[i] = cBorderInterpolate(sy + i, ssize.height, borderType1);
                }

                for( k = 0; k < cn; k++, S0++, w -= 64 )
                {
                    WT cv = cval[k], sum = cv*ONE;
                    for( i = 0; i < 8; i++, w += 8 )
                    {
                        int yi = y[i];
                        const T* S1 = S0 + yi*sstep;
                        if( yi < 0 )
                            continue;
                        if( x[0] >= 0 )
                            sum += (S1[x[0]] - cv)*w[0];
                        if( x[1] >= 0 )
                            sum += (S1[x[1]] - cv)*w[1];
                        if( x[2] >= 0 )
                            sum += (S1[x[2]] - cv)*w[2];
                        if( x[3] >= 0 )
                            sum += (S1[x[3]] - cv)*w[3];
                        if( x[4] >= 0 )
                            sum += (S1[x[4]] - cv)*w[4];
                        if( x[5] >= 0 )
                            sum += (S1[x[5]] - cv)*w[5];
                        if( x[6] >= 0 )
                            sum += (S1[x[6]] - cv)*w[6];
                        if( x[7] >= 0 )
                            sum += (S1[x[7]] - cv)*w[7];
                    }
                    D[k] = castOp(sum);
                }
                S0 -= cn;
            }
        }
    }
}
};

typedef void (*RemapNNFunc)(const img_t* src, img_t* dst, const img_t* _xy,
                            int borderType, const CScalar& _borderValue );

typedef void (*RemapFunc)(const img_t* src, img_t* dst, const img_t* _xy,
                          const img_t* _fxy, const void* _wtab,
                          int borderType, const CScalar& _borderValue);

class RemapInvoker :
    public CParallelLoopBody
{
public:
    RemapInvoker(const img_t* src, img_t* dst, const img_t *_m1,
                 const img_t *_m2, int _borderType, const CScalar &_borderValue,
                 int _planar_input, RemapNNFunc _nnfunc, RemapFunc _ifunc, const void *_ctab) :
        CParallelLoopBody(), src(src), dst(dst), m1(_m1), m2(_m2),
        borderType(_borderType), borderValue(_borderValue),
        planar_input(_planar_input), nnfunc(_nnfunc), ifunc(_ifunc), ctab(_ctab)
    {
    }

    virtual void operator() (const CRange& range) const
    {
        int x, y, x1, y1;
        const int buf_size = 1 << 14;
        TypeId map_depth = CC_MAT_DEPTH(m1);
        int brows0 = MIN(128, dst->rows);
        int bcols0 = MIN(buf_size/brows0, dst->cols);
        brows0 = MIN(buf_size/bcols0, dst->rows);
    #if CC_SSE2_
        bool useSIMD = checkHardwareSupport(CC_CPU_SSE2);
    #endif
        IM_BEGIN(im, 4);

        img_t *_bufxy = cvSetMat(im++, brows0, bcols0, CC_16S, 2), *_bufa = NULL;
        if( !nnfunc )
            _bufa = cvSetMat(im++, brows0, bcols0, CC_16U, 1);

        for( y = range.start; y < range.end; y += brows0 )
        {
            for( x = 0; x < dst->cols; x += bcols0 )
            {
                int brows = MIN(brows0, range.end - y);
                int bcols = MIN(bcols0, dst->cols - x);
                IM_DEF(tmp, 10);
                img_t *dpart = cvGetSubRect1(dst, tmp++, x, y, bcols, brows);
                img_t *bufxy = cvGetSubRect1(_bufxy, tmp++, 0, 0, bcols, brows);

                if( nnfunc )
                {
                    if( CC_MAT_TYPECN(m1) == CC_16SC2 && imempty(m2) ) // the data is already in the right format
                        bufxy = cvGetSubRect1(m1, tmp++, x, y, bcols, brows);
                    else if( map_depth != CC_32F )
                    {
                        for( y1 = 0; y1 < brows; y1++ )
                        {
                            short* XY = img_row(short, bufxy, y1);
                            const short* sXY = img_row(short, m1, y+y1) + x*2;
                            const ushort* sA = img_row(ushort, m2, y+y1) + x;

                            for( x1 = 0; x1 < bcols; x1++ )
                            {
                                int a = sA[x1] & (CC_INTER_TAB_SIZE2-1);
                                XY[x1*2] = sXY[x1*2] + NNDeltaTab_i[a][0];
                                XY[x1*2+1] = sXY[x1*2+1] + NNDeltaTab_i[a][1];
                            }
                        }
                    }
                    else if( !planar_input )
                        cvConvertTo(cvGetSubRect1(m1, tmp, x, y, bcols, brows), bufxy, bufxy->tid);
                    else
                    {
                        for( y1 = 0; y1 < brows; y1++ )
                        {
                            short* XY = img_row(short, bufxy, y1);
                            const float* sX = img_row(float, m1, y+y1) + x;
                            const float* sY = img_row(float, m2, y+y1) + x;
                            x1 = 0;

                        #if CC_SSE2_
                            if( useSIMD )
                            {
                                for( ; x1 <= bcols - 8; x1 += 8 )
                                {
                                    __m128 fx0 = _mm_loadu_ps(sX + x1);
                                    __m128 fx1 = _mm_loadu_ps(sX + x1 + 4);
                                    __m128 fy0 = _mm_loadu_ps(sY + x1);
                                    __m128 fy1 = _mm_loadu_ps(sY + x1 + 4);
                                    __m128i ix0 = _mm_cvtps_epi32(fx0);
                                    __m128i ix1 = _mm_cvtps_epi32(fx1);
                                    __m128i iy0 = _mm_cvtps_epi32(fy0);
                                    __m128i iy1 = _mm_cvtps_epi32(fy1);
                                    ix0 = _mm_packs_epi32(ix0, ix1);
                                    iy0 = _mm_packs_epi32(iy0, iy1);
                                    ix1 = _mm_unpacklo_epi16(ix0, iy0);
                                    iy1 = _mm_unpackhi_epi16(ix0, iy0);
                                    _mm_storeu_si128((__m128i*)(XY + x1*2), ix1);
                                    _mm_storeu_si128((__m128i*)(XY + x1*2 + 8), iy1);
                                }
                            }
                        #endif

                            for( ; x1 < bcols; x1++ )
                            {
                                XY[x1*2] = float_to_short(sX[x1]);
                                XY[x1*2+1] = float_to_short(sY[x1]);
                            }
                        }
                    }
                    nnfunc( src, dpart, bufxy, borderType, borderValue );
                    continue;
                }

                img_t *bufa = cvGetSubRect1(_bufa, tmp++, 0, 0, bcols, brows);
                for( y1 = 0; y1 < brows; y1++ )
                {
                    short* XY = img_row(short, bufxy, y1);
                    ushort* A = img_row(ushort, bufa, y1);

                    if( CC_MAT_TYPECN(m1) == CC_16SC2 && (CC_MAT_TYPECN(m2) == CC_16UC1 || CC_MAT_TYPECN(m2) == CC_16SC1) )
                    {
                        bufxy = cvGetSubRect1(m1, tmp, x, y, bcols, brows);

                        const ushort* sA = img_row(ushort, m2, y+y1) + x;
                        x1 = 0;

                    #if CC_NEON
                        uint16x8_t v_scale = vdupq_n_u16(CC_INTER_TAB_SIZE2-1);
                        for ( ; x1 <= bcols - 8; x1 += 8)
                            vst1q_u16(A + x1, vandq_u16(vld1q_u16(sA + x1), v_scale));
                    #elif CC_SSE2_
                        __m128i v_scale = _mm_set1_epi16(CC_INTER_TAB_SIZE2-1);
                        for ( ; x1 <= bcols - 8; x1 += 8)
                            _mm_storeu_si128((__m128i *)(A + x1), _mm_and_si128(_mm_loadu_si128((const __m128i *)(sA + x1)), v_scale));
                    #endif

                        for( ; x1 < bcols; x1++ )
                            A[x1] = (ushort)(sA[x1] & (CC_INTER_TAB_SIZE2-1));
                    }
                    else if( planar_input )
                    {
                        const float* sX = img_row(float, m1, y+y1) + x;
                        const float* sY = img_row(float, m2, y+y1) + x;

                        x1 = 0;
                    #if CC_SSE2_
                        if( useSIMD )
                        {
                            __m128 scale = _mm_set1_ps((float)CC_INTER_TAB_SIZE);
                            __m128i mask = _mm_set1_epi32(CC_INTER_TAB_SIZE-1);
                            for( ; x1 <= bcols - 8; x1 += 8 )
                            {
                                __m128 fx0 = _mm_loadu_ps(sX + x1);
                                __m128 fx1 = _mm_loadu_ps(sX + x1 + 4);
                                __m128 fy0 = _mm_loadu_ps(sY + x1);
                                __m128 fy1 = _mm_loadu_ps(sY + x1 + 4);
                                __m128i ix0 = _mm_cvtps_epi32(_mm_mul_ps(fx0, scale));
                                __m128i ix1 = _mm_cvtps_epi32(_mm_mul_ps(fx1, scale));
                                __m128i iy0 = _mm_cvtps_epi32(_mm_mul_ps(fy0, scale));
                                __m128i iy1 = _mm_cvtps_epi32(_mm_mul_ps(fy1, scale));
                                __m128i mx0 = _mm_and_si128(ix0, mask);
                                __m128i mx1 = _mm_and_si128(ix1, mask);
                                __m128i my0 = _mm_and_si128(iy0, mask);
                                __m128i my1 = _mm_and_si128(iy1, mask);
                                mx0 = _mm_packs_epi32(mx0, mx1);
                                my0 = _mm_packs_epi32(my0, my1);
                                my0 = _mm_slli_epi16(my0, CC_INTER_BITS);
                                mx0 = _mm_or_si128(mx0, my0);
                                _mm_storeu_si128((__m128i*)(A + x1), mx0);
                                ix0 = _mm_srai_epi32(ix0, CC_INTER_BITS);
                                ix1 = _mm_srai_epi32(ix1, CC_INTER_BITS);
                                iy0 = _mm_srai_epi32(iy0, CC_INTER_BITS);
                                iy1 = _mm_srai_epi32(iy1, CC_INTER_BITS);
                                ix0 = _mm_packs_epi32(ix0, ix1);
                                iy0 = _mm_packs_epi32(iy0, iy1);
                                ix1 = _mm_unpacklo_epi16(ix0, iy0);
                                iy1 = _mm_unpackhi_epi16(ix0, iy0);
                                _mm_storeu_si128((__m128i*)(XY + x1*2), ix1);
                                _mm_storeu_si128((__m128i*)(XY + x1*2 + 8), iy1);
                            }
                        }
                    #elif CC_NEON
                        float32x4_t v_scale = vdupq_n_f32((float)CC_INTER_TAB_SIZE);
                        int32x4_t v_scale2 = vdupq_n_s32(CC_INTER_TAB_SIZE - 1), v_scale3 = vdupq_n_s32(CC_INTER_TAB_SIZE);

                        for( ; x1 <= bcols - 4; x1 += 4 )
                        {
                            int32x4_t v_sx = cv_vrndq_s32_f32(vmulq_f32(vld1q_f32(sX + x1), v_scale)),
                                      v_sy = cv_vrndq_s32_f32(vmulq_f32(vld1q_f32(sY + x1), v_scale));
                            int32x4_t v_v = vmlaq_s32(vandq_s32(v_sx, v_scale2), v_scale3,
                                                      vandq_s32(v_sy, v_scale2));
                            vst1_u16(A + x1, vqmovun_s32(v_v));

                            int16x4x2_t v_dst = vzip_s16(vqmovn_s32(vshrq_n_s32(v_sx, CC_INTER_BITS)),
                                                         vqmovn_s32(vshrq_n_s32(v_sy, CC_INTER_BITS)));
                            vst1q_s16(XY + (x1 << 1), vcombine_s16(v_dst.val[0], v_dst.val[1]));
                        }
                    #endif

                        for( ; x1 < bcols; x1++ )
                        {
                            int sx = cRound(sX[x1]*CC_INTER_TAB_SIZE);
                            int sy = cRound(sY[x1]*CC_INTER_TAB_SIZE);
                            int v = (sy & (CC_INTER_TAB_SIZE-1))*CC_INTER_TAB_SIZE + (sx & (CC_INTER_TAB_SIZE-1));
                            XY[x1*2] = int_to_short(sx >> CC_INTER_BITS);
                            XY[x1*2+1] = int_to_short(sy >> CC_INTER_BITS);
                            A[x1] = (ushort)v;
                        }
                    }
                    else
                    {
                        const float* sXY = img_row(float, m1, y+y1) + x*2;
                        x1 = 0;

                    #if CC_NEON
                        float32x4_t v_scale = vdupq_n_f32(CC_INTER_TAB_SIZE);
                        int32x4_t v_scale2 = vdupq_n_s32(CC_INTER_TAB_SIZE-1), v_scale3 = vdupq_n_s32(CC_INTER_TAB_SIZE);

                        for( ; x1 <= bcols - 4; x1 += 4 )
                        {
                            float32x4x2_t v_src = vld2q_f32(sXY + (x1 << 1));
                            int32x4_t v_sx = cv_vrndq_s32_f32(vmulq_f32(v_src.val[0], v_scale));
                            int32x4_t v_sy = cv_vrndq_s32_f32(vmulq_f32(v_src.val[1], v_scale));
                            int32x4_t v_v = vmlaq_s32(vandq_s32(v_sx, v_scale2), v_scale3,
                                                      vandq_s32(v_sy, v_scale2));
                            vst1_u16(A + x1, vqmovun_s32(v_v));

                            int16x4x2_t v_dst = vzip_s16(vqmovn_s32(vshrq_n_s32(v_sx, CC_INTER_BITS)),
                                                         vqmovn_s32(vshrq_n_s32(v_sy, CC_INTER_BITS)));
                            vst1q_s16(XY + (x1 << 1), vcombine_s16(v_dst.val[0], v_dst.val[1]));
                        }
                    #endif

                        for( x1 = 0; x1 < bcols; x1++ )
                        {
                            int sx = cRound(sXY[x1*2]*CC_INTER_TAB_SIZE);
                            int sy = cRound(sXY[x1*2+1]*CC_INTER_TAB_SIZE);
                            int v = (sy & (CC_INTER_TAB_SIZE-1))*CC_INTER_TAB_SIZE + (sx & (CC_INTER_TAB_SIZE-1));
                            XY[x1*2] = int_to_short(sx >> CC_INTER_BITS);
                            XY[x1*2+1] = int_to_short(sy >> CC_INTER_BITS);
                            A[x1] = (ushort)v;
                        }
                    }
                }
                ifunc(src, dpart, bufxy, bufa, ctab, borderType, borderValue);
            }
        }
        
        IM_END(im);
    }

private:
    const img_t* src;
    img_t* dst;
    const img_t *m1, *m2;
    int borderType;
    CScalar borderValue;
    int planar_input;
    RemapNNFunc nnfunc;
    RemapFunc ifunc;
    const void *ctab;
};

#ifdef HAVE_OPENCL

static bool ocl_remap(const img_t* src, img_t* dst, const img_t* _map1, const img_t* _map2,
                      int interpolation, int borderType, const CScalar& borderValue)
{
    const ocl::Device & dev = ocl::Device::getDefault();
    int cn = CC_MAT_CN(src), type = src->tid, depth = src CC_MAT_DEPTH(),
            rowsPerWI = dev.isIntel() ? 4 : 1;

    if (borderType == CC_BORDER_TRANSPARENT || !(interpolation == CC_INTER_LINEAR || interpolation == CC_INTER_NEAREST)
            || _map1->tid == CC_16SC1 || _map2->tid == CC_16SC1)
        return false;

    UMat src = src.getUMat(), map1 = _map1.getUMat(), map2 = _map2.getUMat();

    if( (map1->tid == CC_16SC2 && (map2->tid == CC_16UC1 || map2.empty())) ||
        (map2->tid == CC_16SC2 && (map1->tid == CC_16UC1 || map1.empty())) )
    {
        if (map1->tid != CC_16SC2)
            T_SWAP(_Tp, map1, map2);
    }
    else
        CC_Assert( map1->tid == CC_32FC2 || (map1->tid == CC_32FC1 && map2->tid == CC_32FC1) );

    dst cvSetMat(map1.size(), type);
    UMat dst = dst.getUMat();

    String kernelName = "remap";
    if (map1->tid == CC_32FC2 && map2.empty())
        kernelName += "_32FC2";
    else if (map1->tid == CC_16SC2)
    {
        kernelName += "_16SC2";
        if (!map2.empty())
            kernelName += "_16UC1";
    }
    else if (map1->tid == CC_32FC1 && map2->tid == CC_32FC1)
        kernelName += "_2_32FC1";
    else
        CC_Error(CC_StsBadArg, "Unsupported map types");

    static const char * const interMap[] = { "CC_INTER_NEAREST", "CC_INTER_LINEAR", "CC_INTER_CUBIC", "CC_INTER_LINEAR", "INTER_LANCZOS" };
    static const char * const borderMap[] = { "CC_BORDER_CONSTANT", "CC_BORDER_REPLICATE", "CC_BORDER_REFLECT", "CC_BORDER_WRAP",
                           "CC_BORDER_REFLECT_101", "CC_BORDER_TRANSPARENT" };
    String buildOptions = format("-D %s -D %s -D T=%s -D rowsPerWI=%d",
                                 interMap[interpolation], borderMap[borderType],
                                 ocl::typeToStr(type), rowsPerWI);

    if (interpolation != CC_INTER_NEAREST)
    {
        char cvt[3][40];
        int wdepth = MAX(CC_32F, depth);
        buildOptions = buildOptions
                      + format(" -D WT=%s -D convertToT=%s -D convertToWT=%s"
                               " -D convertToWT2=%s -D WT2=%s",
                               ocl::typeToStr(CC_MAKE_TYPE(wdepth, cn)),
                               ocl::convertTypeStr(wdepth, depth, cn, cvt[0]),
                               ocl::convertTypeStr(depth, wdepth, cn, cvt[1]),
                               ocl::convertTypeStr(CC_32S, wdepth, 2, cvt[2]),
                               ocl::typeToStr(CC_MAKE_TYPE(wdepth, 2)));
    }
    int scalarcn = cn == 3 ? 4 : cn;
    int sctype = CC_MAKETYPE(depth, scalarcn);
    buildOptions += format(" -D T=%s -D T1=%s -D cn=%d -D ST=%s -D depth=%d",
                           ocl::typeToStr(type), ocl::typeToStr(depth),
                           cn, ocl::typeToStr(sctype), depth);

    ocl::Kernel k(kernelName.c_str(), ocl::imgproc::remap_oclsrc, buildOptions);

    img_t scalar(1, 1, sctype, borderValue);
    ocl::KernelArg srcarg = ocl::KernelArg::ReadOnly(src), dstarg = ocl::KernelArg::WriteOnly(dst),
            map1arg = ocl::KernelArg::ReadOnlyNoSize(map1),
            scalararg = ocl::KernelArg::Constant((void*)scalar->tt.data, scalar CC_TYPE_SIZE());

    if (map2.empty())
        k.args(srcarg, dstarg, map1arg, scalararg);
    else
        k.args(srcarg, dstarg, map1arg, ocl::KernelArg::ReadOnlyNoSize(map2), scalararg);

    size_t globalThreads[2] = { (size_t)dst->cols, ((size_t)dst->rows + rowsPerWI - 1) / rowsPerWI };
    return k.run(2, globalThreads, NULL, false);
}

static bool ocl_linearPolar(const img_t* src, img_t* dst,
    CPoint2f center, double maxRadius, int flags)
{
    UMat src_with_border; // don't scope this variable (it holds image data)

    UMat mapx, mapy, r, cp_sp;
    UMat src = src.getUMat();
    dst cvSetMat(cvGetSize(src), src->tid);
    CSize dsize = cvGetSize(src);
    r cvSetMat(CSize(1, dsize.width), CC_32F);
    cp_sp cvSetMat(CSize(1, dsize.height), CC_32FC2);

    mapx cvSetMat(dsize, CC_32F);
    mapy cvSetMat(dsize, CC_32F);
    size_t w = dsize.width;
    size_t h = dsize.height;
    String buildOptions;
    unsigned mem_size = 32;
    if (flags & CC_WARP_INVERSE_MAP)
    {
        buildOptions = "-D InverseMap";
    }
    else
    {
        buildOptions = format("-D ForwardMap  -D MEM_SIZE=%d", mem_size);
    }
    String retval;
    ocl::Program p(ocl::imgproc::linearPolar_oclsrc, buildOptions, retval);
    ocl::Kernel k("linearPolar", p);
    ocl::KernelArg ocl_mapx = ocl::KernelArg::PtrReadWrite(mapx), ocl_mapy = ocl::KernelArg::PtrReadWrite(mapy);
    ocl::KernelArg  ocl_cp_sp = ocl::KernelArg::PtrReadWrite(cp_sp);
    ocl::KernelArg ocl_r = ocl::KernelArg::PtrReadWrite(r);

    if (!(flags & CC_WARP_INVERSE_MAP))
    {



        ocl::Kernel computeAngleRadius_Kernel("computeAngleRadius", p);
        float PI2_height = (float) CC_2PI / dsize.height;
        float maxRadius_width = (float) maxRadius / dsize.width;
        computeAngleRadius_Kernel.args(ocl_cp_sp, ocl_r, maxRadius_width, PI2_height, (unsigned)dsize.width, (unsigned)dsize.height);
        size_t max_dim = max(h, w);
        computeAngleRadius_Kernel.run(1, &max_dim, NULL, false);
        k.args(ocl_mapx, ocl_mapy, ocl_cp_sp, ocl_r, center.x, center.y, (unsigned)dsize.width, (unsigned)dsize.height);
    }
    else
    {
        const int ANGLE_BORDER = 1;

        copyMakeBorder(src, src_with_border, ANGLE_BORDER, ANGLE_BORDER, 0, 0, CC_BORDER_WRAP);
        src = src_with_border;
        CSize ssize = src_with_border.size();
        ssize.height -= 2 * ANGLE_BORDER;
        float ascale =  ssize.height / ((float)CC_2PI);
        float pscale =  ssize.width / ((float) maxRadius);

        k.args(ocl_mapx, ocl_mapy, ascale, pscale, center.x, center.y, ANGLE_BORDER, (unsigned)dsize.width, (unsigned)dsize.height);


    }
    size_t globalThreads[2] = { (size_t)dsize.width , (size_t)dsize.height };
    size_t localThreads[2] = { mem_size , mem_size };
    k.run(2, globalThreads, localThreads, false);
    remap(src, dst, mapx, mapy, flags & CC_INTER_MAX, (flags & CC_WARP_FILL_OUTLIERS) ? CC_BORDER_CONSTANT : CC_BORDER_TRANSPARENT);
    return true;
}
static bool ocl_logPolar(const img_t* src, img_t* dst,
    CPoint2f center, double M, int flags)
{
    if (M <= 0)
        CC_Error(CC_StsOutOfRange, "M should be >0");
    UMat src_with_border; // don't scope this variable (it holds image data)

    UMat mapx, mapy, r, cp_sp;
    UMat src = src.getUMat();
    dst cvSetMat(cvGetSize(src), src->tid);
    CSize dsize = cvGetSize(src);
    r cvSetMat(CSize(1, dsize.width), CC_32F);
    cp_sp cvSetMat(CSize(1, dsize.height), CC_32FC2);

    mapx cvSetMat(dsize, CC_32F);
    mapy cvSetMat(dsize, CC_32F);
    size_t w = dsize.width;
    size_t h = dsize.height;
    String buildOptions;
    unsigned mem_size = 32;
    if (flags & CC_WARP_INVERSE_MAP)
    {
        buildOptions = "-D InverseMap";
    }
    else
    {
        buildOptions = format("-D ForwardMap  -D MEM_SIZE=%d", mem_size);
    }
    String retval;
    ocl::Program p(ocl::imgproc::logPolar_oclsrc, buildOptions, retval);
    //ocl::Program p(ocl::imgproc::my_linearPolar_oclsrc, buildOptions, retval);
    //printf("%s\n", retval);
    ocl::Kernel k("logPolar", p);
    ocl::KernelArg ocl_mapx = ocl::KernelArg::PtrReadWrite(mapx), ocl_mapy = ocl::KernelArg::PtrReadWrite(mapy);
    ocl::KernelArg  ocl_cp_sp = ocl::KernelArg::PtrReadWrite(cp_sp);
    ocl::KernelArg ocl_r = ocl::KernelArg::PtrReadWrite(r);

    if (!(flags & CC_WARP_INVERSE_MAP))
    {



        ocl::Kernel computeAngleRadius_Kernel("computeAngleRadius", p);
        float PI2_height = (float) CC_2PI / dsize.height;

        computeAngleRadius_Kernel.args(ocl_cp_sp, ocl_r, (float)M, PI2_height, (unsigned)dsize.width, (unsigned)dsize.height);
        size_t max_dim = max(h, w);
        computeAngleRadius_Kernel.run(1, &max_dim, NULL, false);
        k.args(ocl_mapx, ocl_mapy, ocl_cp_sp, ocl_r, center.x, center.y, (unsigned)dsize.width, (unsigned)dsize.height);
    }
    else
    {
        const int ANGLE_BORDER = 1;

        copyMakeBorder(src, src_with_border, ANGLE_BORDER, ANGLE_BORDER, 0, 0, CC_BORDER_WRAP);
        src = src_with_border;
        CSize ssize = src_with_border.size();
        ssize.height -= 2 * ANGLE_BORDER;
        float ascale =  ssize.height / ((float)CC_2PI);


        k.args(ocl_mapx, ocl_mapy, ascale, (float)M, center.x, center.y, ANGLE_BORDER, (unsigned)dsize.width, (unsigned)dsize.height);


    }
    size_t globalThreads[2] = { (size_t)dsize.width , (size_t)dsize.height };
    size_t localThreads[2] = { mem_size , mem_size };
    k.run(2, globalThreads, localThreads, false);
    remap(src, dst, mapx, mapy, flags & CC_INTER_MAX, (flags & CC_WARP_FILL_OUTLIERS) ? CC_BORDER_CONSTANT : CC_BORDER_TRANSPARENT);
    return true;
}
#endif

#ifdef HAVE_OPENVX
static bool openvx_remap(img_t src, img_t dst, img_t map1, img_t map2, int interpolation, const CScalar& borderValue)
{
    vx_interpolation_type_e inter_type;
    switch (interpolation)
    {
    case CC_INTER_LINEAR:
#if VX_VERSION > VX_VERSION_1_0
        inter_type = VX_INTERPOLATION_BILINEAR;
#else
        inter_type = VX_INTERPOLATION_TYPE_BILINEAR;
#endif
        break;
    case CC_INTER_NEAREST:
/* NEAREST_NEIGHBOR mode disabled since OpenCV round half to even while OpenVX sample implementation round half up
#if VX_VERSION > VX_VERSION_1_0
        inter_type = VX_INTERPOLATION_NEAREST_NEIGHBOR;
#else
        inter_type = VX_INTERPOLATION_TYPE_NEAREST_NEIGHBOR;
#endif
        if (!map1.empty())
            for (int y = 0; y < map1->rows; ++y)
            {
                float* line = map1.ptr<float>(y);
                for (int x = 0; x < map1->cols; ++x)
                    line[x] = cRound(line[x]);
            }
        if (!map2.empty())
            for (int y = 0; y < map2->rows; ++y)
            {
                float* line = map2.ptr<float>(y);
                for (int x = 0; x < map2->cols; ++x)
                    line[x] = cRound(line[x]);
            }
        break;
*/
    case CC_INTER_AREA://AREA interpolation mode is unsupported
    default:
        return false;
    }

    try
    {
        ivx::Context ctx = ivx::Context::create();

        img_t a;
        if (dst->tt.data != src->tt.data)
            a = src;
        else
            src.copyTo(a);

        ivx::Image
            ia = ivx::Image::createFromHandle(ctx, VX_DF_IMAGE_U8,
                ivx::Image::createAddressing(a->cols, a->rows, 1, (vx_int32)(a->step)), a.data),
            ib = ivx::Image::createFromHandle(ctx, VX_DF_IMAGE_U8,
                ivx::Image::createAddressing(dst->cols, dst->rows, 1, (vx_int32)(dst->step)), dst->tt.data);

        //ATTENTION: VX_CONTEXT_IMMEDIATE_BORDER attribute change could lead to strange issues in multi-threaded environments
        //since OpenVX standart says nothing about thread-safety for now
        ivx::border_t prevBorder = ctx.immediateBorder();
        ctx.setImmediateBorder(VX_BORDER_CONSTANT, (vx_uint8)(borderValue[0]));

        ivx::Remap map = ivx::Remap::create(ctx, src->cols, src->rows, dst->cols, dst->rows);
        if (map1.empty()) map.setMappings(map2);
        else if (map2.empty()) map.setMappings(map1);
        else map.setMappings(map1, map2);
        ivx::IVX_CHECK_STATUS(vxuRemap(ctx, ia, map, inter_type, ib));
#ifdef VX_VERSION_1_1
        ib.swapHandle();
        ia.swapHandle();
#endif

        ctx.setImmediateBorder(prevBorder);
    }
    catch (ivx::RuntimeError & e)
    {
        CC_Error(CC_StsInternal, e.what());
        return false;
    }
    catch (ivx::WrapperError & e)
    {
        CC_Error(CC_StsInternal, e.what());
        return false;
    }
    return true;
}
#endif

#if defined HAVE_IPP && IPP_DISABLE_BLOCK

typedef IppStatus (C_STDCALL * ippiRemap)(const void * pSrc, IppiSize srcSize, int srcStep, IppiRect srcRoi,
                                           const Ipp32f* pxMap, int xMapStep, const Ipp32f* pyMap, int yMapStep,
                                           void * pDst, int dstStep, IppiSize dstRoiSize, int interpolation);

class IPPRemapInvoker :
        public CParallelLoopBody
{
public:
    IPPRemapInvoker(img_t*  src, img_t*  dst, img_t*  _xmap, img_t*  _ymap, ippiRemap _ippFunc,
                    int _ippInterpolation, int _borderType, const CScalar & _borderValue, bool * _ok) :
        CParallelLoopBody(), src(src), dst(dst), map1(_xmap), map2(_ymap), ippFunc(_ippFunc),
        ippInterpolation(_ippInterpolation), borderType(_borderType), borderValue(_borderValue), ok(_ok)
    {
        *ok = true;
    }

    virtual void operator() (const CRange & range) const
    {
        IppiRect srcRoiRect = { 0, 0, src->cols, src->rows };
        img_t dstRoi = dst cvGetRows(,range);
        IppiSize dstRoiSize = ippiSize(dstRoi.size());
        int type = dst->tid, depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type);

        if (borderType == CC_BORDER_CONSTANT &&
                !IPPSet(borderValue, dstRoi->tt.data, (int)dstRoi->step, dstRoiSize, cn, depth))
        {
            *ok = false;
            return;
        }

        if (CC_INSTRUMENT_FUN_PTR_CALL_IPP(ippFunc,(src->tt.data, ippiSize(cvGetSize(src)), (int)src->step, srcRoiRect,
                    map1.ptr<Ipp32f>(), (int)map1->step, map2.ptr<Ipp32f>(), (int)map2->step,
                    dstRoi->tt.data, (int)dstRoi->step, dstRoiSize, ippInterpolation)) < 0)
            *ok = false;
        else
        {
            CC_IMPL_ADD(CC_IMPL_IPP|CC_IMPL_MT);
        }
    }

private:
    img_t*  src, & dst, & map1, & map2;
    ippiRemap ippFunc;
    int ippInterpolation, borderType;
    CScalar borderValue;
    bool * ok;
};

#endif



static void remap( const img_t* src, img_t* dst,
                const img_t* map1, const img_t* map2,
                int interpolation, int borderType = CC_BORDER_CONSTANT,
                const CScalar& borderValue = cScalarAll(0))
{
    CC_INSTRUMENT_REGION()

    static RemapNNFunc nn_tab[] =
    {
        remapNearest<uchar>::f, remapNearest<schar>::f, remapNearest<ushort>::f, remapNearest<short>::f,
        remapNearest<int>::f, remapNearest<float>::f, remapNearest<double>::f, 0
    };

    static RemapFunc linear_tab[] =
    {
        remapBilinear<FixedPtCast<int, uchar, CC_INTER_REMAP_COEF_BITS>, RemapVec_8u, short>::f, 0,
        remapBilinear<Cast<float, ushort>, RemapNoVec, float>::f,
        remapBilinear<Cast<float, short>, RemapNoVec, float>::f, 0,
        remapBilinear<Cast<float, float>, RemapNoVec, float>::f,
        remapBilinear<Cast<double, double>, RemapNoVec, float>::f, 0
    };

    static RemapFunc cubic_tab[] =
    {
        remapBicubic<FixedPtCast<int, uchar, CC_INTER_REMAP_COEF_BITS>, short, CC_INTER_REMAP_COEF_SCALE>::f, 0,
        remapBicubic<Cast<float, ushort>, float, 1>::f,
        remapBicubic<Cast<float, short>, float, 1>::f, 0,
        remapBicubic<Cast<float, float>, float, 1>::f,
        remapBicubic<Cast<double, double>, float, 1>::f, 0
    };

    static RemapFunc lanczos4_tab[] =
    {
        remapLanczos4<FixedPtCast<int, uchar, CC_INTER_REMAP_COEF_BITS>, short, CC_INTER_REMAP_COEF_SCALE>::f, 0,
        remapLanczos4<Cast<float, ushort>, float, 1>::f,
        remapLanczos4<Cast<float, short>, float, 1>::f, 0,
        remapLanczos4<Cast<float, float>, float, 1>::f,
        remapLanczos4<Cast<double, double>, float, 1>::f, 0
    };

    IM_BEGIN(im, 10);
    CC_Assert( CC_MAT_TOTAL(map1) > 0 );
    CC_Assert( imempty(map2) || CC_ARE_SIZES_EQ(map2, map1) );

    CC_OCL_RUN(src.dims() <= 2 && dst.isUMat(),
               ocl_remap(src, dst, map1, map2, interpolation, borderType, borderValue))

    cvSetMat(dst, map1->rows, map1->cols, src->tid, CC_MAT_CN(src) );

    CC_OVX_RUN(
        src->tid == CC_8UC1 && dst->tid == CC_8UC1 &&
        (borderType& ~CC_BORDER_ISOLATED) == CC_BORDER_CONSTANT &&
        ((map1->tid == CC_32FC2 && map2.empty() && map1.size == dst.size) ||
         (map1->tid == CC_32FC1 && map2->tid == CC_32FC1 && map1.size == dst.size && map2.size == dst.size) ||
         (map1.empty() && map2->tid == CC_32FC2 && map2.size == dst.size)) &&
        ((borderType & CC_BORDER_ISOLATED) != 0 || !src.isSubmatrix()),
        openvx_remap(src, dst, map1, map2, interpolation, borderValue));

    CC_Assert( dst->cols < SHRT_MAX && dst->rows < SHRT_MAX && src->cols < SHRT_MAX && src->rows < SHRT_MAX );

    if( dst->tt.data == src->tt.data )
        src = cvCloneMat(im++, src);

    if( interpolation == CC_INTER_AREA )
        interpolation = CC_INTER_LINEAR;

    TypeId type = src->tid;

#if defined HAVE_IPP && IPP_DISABLE_BLOCK
    CC_IPP_CHECK()
    {
        if ((interpolation == CC_INTER_LINEAR || interpolation == CC_INTER_CUBIC || interpolation == CC_INTER_NEAREST) &&
                map1->tid == CC_32FC1 && map2->tid == CC_32FC1 &&
                (borderType == CC_BORDER_CONSTANT || borderType == CC_BORDER_TRANSPARENT))
        {
            int ippInterpolation =
                interpolation == CC_INTER_NEAREST ? IPPI_INTER_NN :
                interpolation == CC_INTER_LINEAR ? IPPI_INTER_LINEAR : IPPI_INTER_CUBIC;

            ippiRemap ippFunc =
                type == CC_8UC1 ? (ippiRemap)ippiRemap_8u_C1R :
                type == CC_8UC3 ? (ippiRemap)ippiRemap_8u_C3R :
                type == CC_8UC4 ? (ippiRemap)ippiRemap_8u_C4R :
                type == CC_16UC1 ? (ippiRemap)ippiRemap_16u_C1R :
                type == CC_16UC3 ? (ippiRemap)ippiRemap_16u_C3R :
                type == CC_16UC4 ? (ippiRemap)ippiRemap_16u_C4R :
                type == CC_32FC1 ? (ippiRemap)ippiRemap_32f_C1R :
                type == CC_32FC3 ? (ippiRemap)ippiRemap_32f_C3R :
                type == CC_32FC4 ? (ippiRemap)ippiRemap_32f_C4R : 0;

            if (ippFunc)
            {
                bool ok;
                IPPRemapInvoker invoker(src, dst, map1, map2, ippFunc, ippInterpolation,
                                        borderType, borderValue, &ok);
                CRange range(0, dst->rows);
                parallel_for_(range, invoker, CC_MAT_TOTAL(dst) / (double)(1 << 16));

                if (ok)
                {
                    CC_IMPL_ADD(CC_IMPL_IPP|CC_IMPL_MT);
                    return;
                }
                setIppErrorStatus();
            }
        }
    }
#endif

    RemapNNFunc nnfunc = 0;
    RemapFunc ifunc = 0;
    const void* ctab = 0;
    bool fixpt = type == CC_8U;
    bool planar_input = false;

    if( interpolation == CC_INTER_NEAREST )
    {
        nnfunc = nn_tab[type];
        CC_Assert( nnfunc != 0 );
    }
    else
    {
        if( interpolation == CC_INTER_LINEAR )
            ifunc = linear_tab[type];
        else if( interpolation == CC_INTER_CUBIC )
            ifunc = cubic_tab[type];
        else if( interpolation == CC_INTER_LANCZOS4 )
            ifunc = lanczos4_tab[type];
        else
            CC_Error( CC_StsBadArg, "Unknown interpolation method" );
        CC_Assert( ifunc != 0 );
        ctab = initInterTab2D( interpolation, fixpt );
    }

    const img_t *m1 = map1, *m2 = map2;

    if( (CC_MAT_TYPECN(map1) == CC_16SC2 && (CC_MAT_TYPECN(map2) == CC_16UC1 || CC_MAT_TYPECN(map2) == CC_16SC1 || imempty(map2))) ||
        (CC_MAT_TYPECN(map2) == CC_16SC2 && (CC_MAT_TYPECN(map1) == CC_16UC1 || CC_MAT_TYPECN(map1) == CC_16SC1 || imempty(map1))) )
    {
        if( CC_MAT_TYPECN(map1) != CC_16SC2 )
            T_SWAP(const img_t*, m1, m2);
    }
    else
    {
        CC_Assert( ((CC_MAT_TYPECN(map1) == CC_32FC2 || CC_MAT_TYPECN(map1) == CC_16SC2) && imempty(map2)) ||
            (CC_MAT_TYPECN(map1) == CC_32FC1 && CC_MAT_TYPECN(map2) == CC_32FC1) );
        planar_input = CC_MAT_CN(map1) == 1;
    }

    RemapInvoker invoker(src, dst, m1, m2,
                         borderType, borderValue, planar_input, nnfunc, ifunc,
                         ctab);
    parallel_for_(cRange(0, dst->rows), invoker, CC_MAT_TOTAL(dst)/(double)(1<<16));
    IM_END(im);
}


static void convertMaps( const img_t* _map1, const img_t* _map2,
                      img_t* dstmap1, img_t* dstmap2,
                      TypeCnId dstm1type, bool nninterpolate )
{
    CC_INSTRUMENT_REGION()

    const img_t *map1 = _map1, *map2 = _map2;
    CSize size = cvGetSize(map1);
    const img_t *m1 = map1, *m2 = map2;
    TypeCnId m1type = CC_MAT_TYPECN(m1), m2type = CC_MAT_TYPECN(m2);

    CC_Assert( (m1type == CC_16SC2 && (nninterpolate || m2type == CC_16UC1 || m2type == CC_16SC1)) ||
               (m2type == CC_16SC2 && (nninterpolate || m1type == CC_16UC1 || m1type == CC_16SC1)) ||
               (m1type == CC_32FC1 && m2type == CC_32FC1) ||
               (m1type == CC_32FC2 && imempty(m2)) );

    if( m2type == CC_16SC2 )
    {
        T_SWAP(const img_t *,  m1, m2 );
        T_SWAP(TypeCnId,  m1type, m2type );
    }

    if( dstm1type <= 0 )
        dstm1type = m1type == CC_16SC2 ? CC_32FC2 : CC_16SC2;
    CC_Assert( dstm1type == CC_16SC2 || dstm1type == CC_32FC1 || dstm1type == CC_32FC2 );
    cvSetMat1( dstmap1, size.h, size.w, dstm1type );

    if( !nninterpolate && dstm1type != CC_32FC2 )
    {
        cvSetMat1( dstmap2, size.h, size.w, dstm1type == CC_16SC2 ? CC_16UC1 : CC_32FC1 );
    }
    else
        imfree(dstmap2);

    if( m1type == dstm1type || (nninterpolate &&
        ((m1type == CC_16SC2 && dstm1type == CC_32FC2) ||
        (m1type == CC_32FC2 && dstm1type == CC_16SC2))) )
    {
        cvConvertTo( m1, dstmap1, dstmap1->tid );
        if( !imempty(dstmap2) && CC_MAT_TYPECN(dstmap2) == CC_MAT_TYPECN(m2) )
            cvCopy( m2, dstmap2 );
        return;
    }

    if( m1type == CC_32FC1 && dstm1type == CC_32FC2 )
    {
        const img_t *vdata[] = { m1, m2 };
        cvMerges( vdata, 2, dstmap1 );
        return;
    }

    if( m1type == CC_32FC2 && dstm1type == CC_32FC1 )
    {
        img_t *mv[] = { dstmap1, dstmap2 };
        cvSplits( m1, mv );
        return;
    }

    if( CC_IS_CONT_MAT(m1) && (imempty(m2) || CC_IS_CONT_MAT(m2)) &&
        CC_IS_CONT_MAT(dstmap1) && (imempty(dstmap2) || CC_IS_CONT_MAT(dstmap2)) )
    {
        size.width *= size.height;
        size.height = 1;
    }

#if CC_SSE2_
    bool useSSE2 = checkHardwareSupport(CC_CPU_SSE2);
#endif
#if CC_SSE4_1
    bool useSSE4_1 = checkHardwareSupport(CC_CPU_SSE4_1);
#endif

    const float scale = 1.f/CC_INTER_TAB_SIZE;
    int x, y;
    for( y = 0; y < size.height; y++ )
    {
        const float* src1f = img_row(float, m1, y);
        const float* src2f = img_row(float, m2, y);
        const short* src1 = (const short*)src1f;
        const ushort* src2 = (const ushort*)src2f;

        float* dst1f = img_row(float, dstmap1, y);
        float* dst2f = img_row(float, dstmap2, y);
        short* dst1 = (short*)dst1f;
        ushort* dst2 = (ushort*)dst2f;
        x = 0;

        if( m1type == CC_32FC1 && dstm1type == CC_16SC2 )
        {
            if( nninterpolate )
            {
                #if CC_NEON
                for( ; x <= size.width - 8; x += 8 )
                {
                    int16x8x2_t v_dst;
                    v_dst.val[0] = vcombine_s16(vqmovn_s32(cv_vrndq_s32_f32(vld1q_f32(src1f + x))),
                                                vqmovn_s32(cv_vrndq_s32_f32(vld1q_f32(src1f + x + 4))));
                    v_dst.val[1] = vcombine_s16(vqmovn_s32(cv_vrndq_s32_f32(vld1q_f32(src2f + x))),
                                                vqmovn_s32(cv_vrndq_s32_f32(vld1q_f32(src2f + x + 4))));

                    vst2q_s16(dst1 + (x << 1), v_dst);
                }
                #elif CC_SSE4_1
                if (useSSE4_1)
                {
                    for( ; x <= size.width - 16; x += 16 )
                    {
                        __m128i v_dst0 = _mm_packs_epi32(_mm_cvtps_epi32(_mm_loadu_ps(src1f + x)),
                                                         _mm_cvtps_epi32(_mm_loadu_ps(src1f + x + 4)));
                        __m128i v_dst1 = _mm_packs_epi32(_mm_cvtps_epi32(_mm_loadu_ps(src1f + x + 8)),
                                                         _mm_cvtps_epi32(_mm_loadu_ps(src1f + x + 12)));

                        __m128i v_dst2 = _mm_packs_epi32(_mm_cvtps_epi32(_mm_loadu_ps(src2f + x)),
                                                         _mm_cvtps_epi32(_mm_loadu_ps(src2f + x + 4)));
                        __m128i v_dst3 = _mm_packs_epi32(_mm_cvtps_epi32(_mm_loadu_ps(src2f + x + 8)),
                                                         _mm_cvtps_epi32(_mm_loadu_ps(src2f + x + 12)));

                        _mm_interleave_epi16(v_dst0, v_dst1, v_dst2, v_dst3);

                        _mm_storeu_si128((__m128i *)(dst1 + x * 2), v_dst0);
                        _mm_storeu_si128((__m128i *)(dst1 + x * 2 + 8), v_dst1);
                        _mm_storeu_si128((__m128i *)(dst1 + x * 2 + 16), v_dst2);
                        _mm_storeu_si128((__m128i *)(dst1 + x * 2 + 24), v_dst3);
                    }
                }
                #endif
                for( ; x < size.width; x++ )
                {
                    dst1[x*2] = float_to_short(src1f[x]);
                    dst1[x*2+1] = float_to_short(src2f[x]);
                }
            }
            else
            {
                #if CC_NEON
                float32x4_t v_scale = vdupq_n_f32((float)CC_INTER_TAB_SIZE);
                int32x4_t v_mask = vdupq_n_s32(CC_INTER_TAB_SIZE - 1);

                for( ; x <= size.width - 8; x += 8 )
                {
                    int32x4_t v_ix0 = cv_vrndq_s32_f32(vmulq_f32(vld1q_f32(src1f + x), v_scale));
                    int32x4_t v_ix1 = cv_vrndq_s32_f32(vmulq_f32(vld1q_f32(src1f + x + 4), v_scale));
                    int32x4_t v_iy0 = cv_vrndq_s32_f32(vmulq_f32(vld1q_f32(src2f + x), v_scale));
                    int32x4_t v_iy1 = cv_vrndq_s32_f32(vmulq_f32(vld1q_f32(src2f + x + 4), v_scale));

                    int16x8x2_t v_dst;
                    v_dst.val[0] = vcombine_s16(vqmovn_s32(vshrq_n_s32(v_ix0, CC_INTER_BITS)),
                                                vqmovn_s32(vshrq_n_s32(v_ix1, CC_INTER_BITS)));
                    v_dst.val[1] = vcombine_s16(vqmovn_s32(vshrq_n_s32(v_iy0, CC_INTER_BITS)),
                                                vqmovn_s32(vshrq_n_s32(v_iy1, CC_INTER_BITS)));

                    vst2q_s16(dst1 + (x << 1), v_dst);

                    uint16x4_t v_dst0 = vqmovun_s32(vaddq_s32(vshlq_n_s32(vandq_s32(v_iy0, v_mask), CC_INTER_BITS),
                                                              vandq_s32(v_ix0, v_mask)));
                    uint16x4_t v_dst1 = vqmovun_s32(vaddq_s32(vshlq_n_s32(vandq_s32(v_iy1, v_mask), CC_INTER_BITS),
                                                              vandq_s32(v_ix1, v_mask)));
                    vst1q_u16(dst2 + x, vcombine_u16(v_dst0, v_dst1));
                }
                #elif CC_SSE4_1
                if (useSSE4_1)
                {
                    __m128 v_its = _mm_set1_ps(CC_INTER_TAB_SIZE);
                    __m128i v_its1 = _mm_set1_epi32(CC_INTER_TAB_SIZE-1);

                    for( ; x <= size.width - 16; x += 16 )
                    {
                        __m128i v_ix0 = _mm_cvtps_epi32(_mm_mul_ps(_mm_loadu_ps(src1f + x), v_its));
                        __m128i v_ix1 = _mm_cvtps_epi32(_mm_mul_ps(_mm_loadu_ps(src1f + x + 4), v_its));
                        __m128i v_iy0 = _mm_cvtps_epi32(_mm_mul_ps(_mm_loadu_ps(src2f + x), v_its));
                        __m128i v_iy1 = _mm_cvtps_epi32(_mm_mul_ps(_mm_loadu_ps(src2f + x + 4), v_its));

                        __m128i v_dst10 = _mm_packs_epi32(_mm_srai_epi32(v_ix0, CC_INTER_BITS),
                                                          _mm_srai_epi32(v_ix1, CC_INTER_BITS));
                        __m128i v_dst12 = _mm_packs_epi32(_mm_srai_epi32(v_iy0, CC_INTER_BITS),
                                                          _mm_srai_epi32(v_iy1, CC_INTER_BITS));
                        __m128i v_dst20 = _mm_add_epi32(_mm_slli_epi32(_mm_and_si128(v_iy0, v_its1), CC_INTER_BITS),
                                                        _mm_and_si128(v_ix0, v_its1));
                        __m128i v_dst21 = _mm_add_epi32(_mm_slli_epi32(_mm_and_si128(v_iy1, v_its1), CC_INTER_BITS),
                                                        _mm_and_si128(v_ix1, v_its1));
                        _mm_storeu_si128((__m128i *)(dst2 + x), _mm_packus_epi32(v_dst20, v_dst21));

                        v_ix0 = _mm_cvtps_epi32(_mm_mul_ps(_mm_loadu_ps(src1f + x + 8), v_its));
                        v_ix1 = _mm_cvtps_epi32(_mm_mul_ps(_mm_loadu_ps(src1f + x + 12), v_its));
                        v_iy0 = _mm_cvtps_epi32(_mm_mul_ps(_mm_loadu_ps(src2f + x + 8), v_its));
                        v_iy1 = _mm_cvtps_epi32(_mm_mul_ps(_mm_loadu_ps(src2f + x + 12), v_its));

                        __m128i v_dst11 = _mm_packs_epi32(_mm_srai_epi32(v_ix0, CC_INTER_BITS),
                                                          _mm_srai_epi32(v_ix1, CC_INTER_BITS));
                        __m128i v_dst13 = _mm_packs_epi32(_mm_srai_epi32(v_iy0, CC_INTER_BITS),
                                                          _mm_srai_epi32(v_iy1, CC_INTER_BITS));
                        v_dst20 = _mm_add_epi32(_mm_slli_epi32(_mm_and_si128(v_iy0, v_its1), CC_INTER_BITS),
                                                _mm_and_si128(v_ix0, v_its1));
                        v_dst21 = _mm_add_epi32(_mm_slli_epi32(_mm_and_si128(v_iy1, v_its1), CC_INTER_BITS),
                                                _mm_and_si128(v_ix1, v_its1));
                        _mm_storeu_si128((__m128i *)(dst2 + x + 8), _mm_packus_epi32(v_dst20, v_dst21));

                        _mm_interleave_epi16(v_dst10, v_dst11, v_dst12, v_dst13);

                        _mm_storeu_si128((__m128i *)(dst1 + x * 2), v_dst10);
                        _mm_storeu_si128((__m128i *)(dst1 + x * 2 + 8), v_dst11);
                        _mm_storeu_si128((__m128i *)(dst1 + x * 2 + 16), v_dst12);
                        _mm_storeu_si128((__m128i *)(dst1 + x * 2 + 24), v_dst13);
                    }
                }
                #endif
                for( ; x < size.width; x++ )
                {
                    int ix, iy;
                    ix = float_to_int(src1f[x]*CC_INTER_TAB_SIZE);
                    iy = float_to_int(src2f[x]*CC_INTER_TAB_SIZE);
                    dst1[x*2] = int_to_short(ix >> CC_INTER_BITS);
                    dst1[x*2+1] = int_to_short(iy >> CC_INTER_BITS);
                    dst2[x] = (ushort)((iy & (CC_INTER_TAB_SIZE-1))*CC_INTER_TAB_SIZE + (ix & (CC_INTER_TAB_SIZE-1)));
                }
            }
        }
        else if( m1type == CC_32FC2 && dstm1type == CC_16SC2 )
        {
            if( nninterpolate )
            {
                #if CC_NEON
                for( ; x <= (size.width << 1) - 8; x += 8 )
                    vst1q_s16(dst1 + x, vcombine_s16(vqmovn_s32(cv_vrndq_s32_f32(vld1q_f32(src1f + x))),
                                                     vqmovn_s32(cv_vrndq_s32_f32(vld1q_f32(src1f + x + 4)))));
                #elif CC_SSE2_
                for( ; x <= (size.width << 1) - 8; x += 8 )
                {
                    _mm_storeu_si128((__m128i *)(dst1 + x), _mm_packs_epi32(_mm_cvtps_epi32(_mm_loadu_ps(src1f + x)),
                                                                            _mm_cvtps_epi32(_mm_loadu_ps(src1f + x + 4))));
                }
                #endif
                for( ; x < size.width; x++ )
                {
                    dst1[x*2] = float_to_short(src1f[x*2]);
                    dst1[x*2+1] = float_to_short(src1f[x*2+1]);
                }
            }
            else
            {
                #if CC_NEON
                float32x4_t v_scale = vdupq_n_f32((float)CC_INTER_TAB_SIZE);
                int32x4_t v_mask = vdupq_n_s32(CC_INTER_TAB_SIZE - 1);

                for( ; x <= size.width - 8; x += 8 )
                {
                    float32x4x2_t v_src0 = vld2q_f32(src1f + (x << 1)), v_src1 = vld2q_f32(src1f + (x << 1) + 8);
                    int32x4_t v_ix0 = cv_vrndq_s32_f32(vmulq_f32(v_src0.val[0], v_scale));
                    int32x4_t v_ix1 = cv_vrndq_s32_f32(vmulq_f32(v_src1.val[0], v_scale));
                    int32x4_t v_iy0 = cv_vrndq_s32_f32(vmulq_f32(v_src0.val[1], v_scale));
                    int32x4_t v_iy1 = cv_vrndq_s32_f32(vmulq_f32(v_src1.val[1], v_scale));

                    int16x8x2_t v_dst;
                    v_dst.val[0] = vcombine_s16(vqmovn_s32(vshrq_n_s32(v_ix0, CC_INTER_BITS)),
                                                vqmovn_s32(vshrq_n_s32(v_ix1, CC_INTER_BITS)));
                    v_dst.val[1] = vcombine_s16(vqmovn_s32(vshrq_n_s32(v_iy0, CC_INTER_BITS)),
                                                vqmovn_s32(vshrq_n_s32(v_iy1, CC_INTER_BITS)));

                    vst2q_s16(dst1 + (x << 1), v_dst);

                    uint16x4_t v_dst0 = vqmovun_s32(vaddq_s32(vshlq_n_s32(vandq_s32(v_iy0, v_mask), CC_INTER_BITS),
                                                              vandq_s32(v_ix0, v_mask)));
                    uint16x4_t v_dst1 = vqmovun_s32(vaddq_s32(vshlq_n_s32(vandq_s32(v_iy1, v_mask), CC_INTER_BITS),
                                                              vandq_s32(v_ix1, v_mask)));
                    vst1q_u16(dst2 + x, vcombine_u16(v_dst0, v_dst1));
                }
                #elif CC_SSE4_1
                if (useSSE4_1)
                {
                    __m128 v_its = _mm_set1_ps(CC_INTER_TAB_SIZE);
                    __m128i v_its1 = _mm_set1_epi32(CC_INTER_TAB_SIZE-1);
                    __m128i v_y_mask = _mm_set1_epi32((CC_INTER_TAB_SIZE-1) << 16);

                    for( ; x <= size.width - 4; x += 4 )
                    {
                        __m128i v_src0 = _mm_cvtps_epi32(_mm_mul_ps(_mm_loadu_ps(src1f + x * 2), v_its));
                        __m128i v_src1 = _mm_cvtps_epi32(_mm_mul_ps(_mm_loadu_ps(src1f + x * 2 + 4), v_its));

                        __m128i v_dst1 = _mm_packs_epi32(_mm_srai_epi32(v_src0, CC_INTER_BITS),
                                                         _mm_srai_epi32(v_src1, CC_INTER_BITS));
                        _mm_storeu_si128((__m128i *)(dst1 + x * 2), v_dst1);

                        // x0 y0 x1 y1 . . .
                        v_src0 = _mm_packs_epi32(_mm_and_si128(v_src0, v_its1),
                                                 _mm_and_si128(v_src1, v_its1));
                        __m128i v_dst2 = _mm_or_si128(_mm_srli_epi32(_mm_and_si128(v_src0, v_y_mask), 16 - CC_INTER_BITS), // y0 0 y1 0 . . .
                                                      _mm_and_si128(v_src0, v_its1)); // 0 x0 0 x1 . . .
                        _mm_storel_epi64((__m128i *)(dst2 + x), _mm_packus_epi32(v_dst2, v_dst2));
                    }
                }
                #endif
                for( ; x < size.width; x++ )
                {
                    int ix, iy;
                    ix = float_to_int(src1f[x*2]*CC_INTER_TAB_SIZE);
                    iy = float_to_int(src1f[x*2+1]*CC_INTER_TAB_SIZE);
                    dst1[x*2] = int_to_short(ix >> CC_INTER_BITS);
                    dst1[x*2+1] = int_to_short(iy >> CC_INTER_BITS);
                    dst2[x] = (ushort)((iy & (CC_INTER_TAB_SIZE-1))*CC_INTER_TAB_SIZE + (ix & (CC_INTER_TAB_SIZE-1)));
                }
            }
        }
        else if( m1type == CC_16SC2 && dstm1type == CC_32FC1 )
        {
            #if CC_NEON
            uint16x8_t v_mask2 = vdupq_n_u16(CC_INTER_TAB_SIZE2-1);
            uint32x4_t v_zero = vdupq_n_u32(0u), v_mask = vdupq_n_u32(CC_INTER_TAB_SIZE-1);
            float32x4_t v_scale = vdupq_n_f32(scale);

            for( ; x <= size.width - 8; x += 8)
            {
                uint32x4_t v_fxy1, v_fxy2;
                if (src2)
                {
                    uint16x8_t v_src2 = vandq_u16(vld1q_u16(src2 + x), v_mask2);
                    v_fxy1 = vmovl_u16(vget_low_u16(v_src2));
                    v_fxy2 = vmovl_u16(vget_high_u16(v_src2));
                }
                else
                    v_fxy1 = v_fxy2 = v_zero;

                int16x8x2_t v_src = vld2q_s16(src1 + (x << 1));
                float32x4_t v_dst1 = vmlaq_f32(vcvtq_f32_s32(vmovl_s16(vget_low_s16(v_src.val[0]))),
                                               v_scale, vcvtq_f32_u32(vandq_u32(v_fxy1, v_mask)));
                float32x4_t v_dst2 = vmlaq_f32(vcvtq_f32_s32(vmovl_s16(vget_low_s16(v_src.val[1]))),
                                               v_scale, vcvtq_f32_u32(vshrq_n_u32(v_fxy1, CC_INTER_BITS)));
                vst1q_f32(dst1f + x, v_dst1);
                vst1q_f32(dst2f + x, v_dst2);

                v_dst1 = vmlaq_f32(vcvtq_f32_s32(vmovl_s16(vget_high_s16(v_src.val[0]))),
                                   v_scale, vcvtq_f32_u32(vandq_u32(v_fxy2, v_mask)));
                v_dst2 = vmlaq_f32(vcvtq_f32_s32(vmovl_s16(vget_high_s16(v_src.val[1]))),
                                   v_scale, vcvtq_f32_u32(vshrq_n_u32(v_fxy2, CC_INTER_BITS)));
                vst1q_f32(dst1f + x + 4, v_dst1);
                vst1q_f32(dst2f + x + 4, v_dst2);
            }
            #elif CC_SSE2_
            __m128i v_mask2 = _mm_set1_epi16(CC_INTER_TAB_SIZE2-1);
            __m128i v_zero = _mm_setzero_si128(), v_mask = _mm_set1_epi32(CC_INTER_TAB_SIZE-1);
            __m128 v_scale = _mm_set1_ps(scale);

            for( ; x <= size.width - 16; x += 16)
            {
                __m128i v_src10 = _mm_loadu_si128((__m128i const *)(src1 + x * 2));
                __m128i v_src11 = _mm_loadu_si128((__m128i const *)(src1 + x * 2 + 8));
                __m128i v_src20 = _mm_loadu_si128((__m128i const *)(src1 + x * 2 + 16));
                __m128i v_src21 = _mm_loadu_si128((__m128i const *)(src1 + x * 2 + 24));

                _mm_deinterleave_epi16(v_src10, v_src11, v_src20, v_src21);

                __m128i v_fxy = src2 ? _mm_and_si128(_mm_loadu_si128((__m128i const *)(src2 + x)), v_mask2) : v_zero;
                __m128i v_fxy_p = _mm_unpacklo_epi16(v_fxy, v_zero);
                _mm_storeu_ps(dst1f + x, _mm_add_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(v_zero, v_src10), 16)),
                                                    _mm_mul_ps(v_scale, _mm_cvtepi32_ps(_mm_and_si128(v_fxy_p, v_mask)))));
                _mm_storeu_ps(dst2f + x, _mm_add_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(v_zero, v_src20), 16)),
                                                    _mm_mul_ps(v_scale, _mm_cvtepi32_ps(_mm_srli_epi32(v_fxy_p, CC_INTER_BITS)))));
                v_fxy_p = _mm_unpackhi_epi16(v_fxy, v_zero);
                _mm_storeu_ps(dst1f + x + 4, _mm_add_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpackhi_epi16(v_zero, v_src10), 16)),
                                                        _mm_mul_ps(v_scale, _mm_cvtepi32_ps(_mm_and_si128(v_fxy_p, v_mask)))));
                _mm_storeu_ps(dst2f + x + 4, _mm_add_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpackhi_epi16(v_zero, v_src20), 16)),
                                                        _mm_mul_ps(v_scale, _mm_cvtepi32_ps(_mm_srli_epi32(v_fxy_p, CC_INTER_BITS)))));

                v_fxy = src2 ? _mm_and_si128(_mm_loadu_si128((__m128i const *)(src2 + x + 8)), v_mask2) : v_zero;
                v_fxy_p = _mm_unpackhi_epi16(v_fxy, v_zero);
                _mm_storeu_ps(dst1f + x + 8, _mm_add_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(v_zero, v_src11), 16)),
                                                        _mm_mul_ps(v_scale, _mm_cvtepi32_ps(_mm_and_si128(v_fxy_p, v_mask)))));
                _mm_storeu_ps(dst2f + x + 8, _mm_add_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpacklo_epi16(v_zero, v_src21), 16)),
                                                        _mm_mul_ps(v_scale, _mm_cvtepi32_ps(_mm_srli_epi32(v_fxy_p, CC_INTER_BITS)))));
                v_fxy_p = _mm_unpackhi_epi16(v_fxy, v_zero);
                _mm_storeu_ps(dst1f + x + 12, _mm_add_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpackhi_epi16(v_zero, v_src11), 16)),
                                                         _mm_mul_ps(v_scale, _mm_cvtepi32_ps(_mm_and_si128(v_fxy_p, v_mask)))));
                _mm_storeu_ps(dst2f + x + 12, _mm_add_ps(_mm_cvtepi32_ps(_mm_srai_epi32(_mm_unpackhi_epi16(v_zero, v_src21), 16)),
                                                         _mm_mul_ps(v_scale, _mm_cvtepi32_ps(_mm_srli_epi32(v_fxy_p, CC_INTER_BITS)))));
            }
            #endif
            for( ; x < size.width; x++ )
            {
                int fxy = src2 ? src2[x] & (CC_INTER_TAB_SIZE2-1) : 0;
                dst1f[x] = src1[x*2] + (fxy & (CC_INTER_TAB_SIZE-1))*scale;
                dst2f[x] = src1[x*2+1] + (fxy >> CC_INTER_BITS)*scale;
            }
        }
        else if( m1type == CC_16SC2 && dstm1type == CC_32FC2 )
        {
            #if CC_NEON
            int16x8_t v_mask2 = vdupq_n_s16(CC_INTER_TAB_SIZE2-1);
            int32x4_t v_zero = vdupq_n_s32(0), v_mask = vdupq_n_s32(CC_INTER_TAB_SIZE-1);
            float32x4_t v_scale = vdupq_n_f32(scale);

            for( ; x <= size.width - 8; x += 8)
            {
                int32x4_t v_fxy1, v_fxy2;
                if (src2)
                {
                    int16x8_t v_src2 = vandq_s16(vld1q_s16((short *)src2 + x), v_mask2);
                    v_fxy1 = vmovl_s16(vget_low_s16(v_src2));
                    v_fxy2 = vmovl_s16(vget_high_s16(v_src2));
                }
                else
                    v_fxy1 = v_fxy2 = v_zero;

                int16x8x2_t v_src = vld2q_s16(src1 + (x << 1));
                float32x4x2_t v_dst;
                v_dst.val[0] = vmlaq_f32(vcvtq_f32_s32(vmovl_s16(vget_low_s16(v_src.val[0]))),
                                         v_scale, vcvtq_f32_s32(vandq_s32(v_fxy1, v_mask)));
                v_dst.val[1] = vmlaq_f32(vcvtq_f32_s32(vmovl_s16(vget_low_s16(v_src.val[1]))),
                                         v_scale, vcvtq_f32_s32(vshrq_n_s32(v_fxy1, CC_INTER_BITS)));
                vst2q_f32(dst1f + (x << 1), v_dst);

                v_dst.val[0] = vmlaq_f32(vcvtq_f32_s32(vmovl_s16(vget_high_s16(v_src.val[0]))),
                                         v_scale, vcvtq_f32_s32(vandq_s32(v_fxy2, v_mask)));
                v_dst.val[1] = vmlaq_f32(vcvtq_f32_s32(vmovl_s16(vget_high_s16(v_src.val[1]))),
                                         v_scale, vcvtq_f32_s32(vshrq_n_s32(v_fxy2, CC_INTER_BITS)));
                vst2q_f32(dst1f + (x << 1) + 8, v_dst);
            }
            #elif CC_SSE2_
            if (useSSE2)
            {
                __m128i v_mask2 = _mm_set1_epi16(CC_INTER_TAB_SIZE2-1);
                __m128i v_zero = _mm_set1_epi32(0), v_mask = _mm_set1_epi32(CC_INTER_TAB_SIZE-1);
                __m128 v_scale = _mm_set1_ps(scale);

                for ( ; x <= size.width - 8; x += 8)
                {
                    __m128i v_src = _mm_loadu_si128((__m128i const *)(src1 + x * 2));
                    __m128i v_fxy = src2 ? _mm_and_si128(_mm_loadu_si128((__m128i const *)(src2 + x)), v_mask2) : v_zero;
                    __m128i v_fxy1 = _mm_and_si128(v_fxy, v_mask);
                    __m128i v_fxy2 = _mm_srli_epi16(v_fxy, CC_INTER_BITS);

                    __m128 v_add = _mm_mul_ps(_mm_cvtepi32_ps(_mm_unpacklo_epi16(v_fxy1, v_fxy2)), v_scale);
                    _mm_storeu_ps(dst1f + x * 2, _mm_add_ps(_mm_cvtepi32_ps(_mm_unpacklo_epi16(v_src, v_zero)), v_add));

                    v_add = _mm_mul_ps(_mm_cvtepi32_ps(_mm_unpackhi_epi16(v_fxy1, v_fxy2)), v_scale);
                    _mm_storeu_ps(dst1f + x * 2, _mm_add_ps(_mm_cvtepi32_ps(_mm_unpackhi_epi16(v_src, v_zero)), v_add));
                }
            }
            #endif
            for( ; x < size.width; x++ )
            {
                int fxy = src2 ? src2[x] & (CC_INTER_TAB_SIZE2-1): 0;
                dst1f[x*2] = src1[x*2] + (fxy & (CC_INTER_TAB_SIZE-1))*scale;
                dst1f[x*2+1] = src1[x*2+1] + (fxy >> CC_INTER_BITS)*scale;
            }
        }
        else
            CC_Error( CC_StsNotImplemented, "Unsupported combination of input/output matrices" );
    }
}


class WarpAffineInvoker :
    public CParallelLoopBody
{
public:
    WarpAffineInvoker(const img_t* src, img_t* dst, int _interpolation, int _borderType,
                      const CScalar &_borderValue, int *_adelta, int *_bdelta, const double *_M) :
        CParallelLoopBody(), src(src), dst(dst), interpolation(_interpolation),
        borderType(_borderType), borderValue(_borderValue), adelta(_adelta), bdelta(_bdelta),
        M(_M)
    {
    }

    virtual void operator() (const CRange& range) const
    {
        const int BLOCK_SZ = 64;
        short XY[BLOCK_SZ*BLOCK_SZ*2], A[BLOCK_SZ*BLOCK_SZ];
        const int AB_BITS = MAX(10, (int)CC_INTER_BITS);
        const int AB_SCALE = 1 << AB_BITS;
        int round_delta = interpolation == CC_INTER_NEAREST ? AB_SCALE/2 : AB_SCALE/CC_INTER_TAB_SIZE/2, x, y, x1, y1;
    #if CC_SSE2_
        bool useSSE2 = checkHardwareSupport(CC_CPU_SSE2);
    #endif
    #if CC_SSE4_1
        bool useSSE4_1 = checkHardwareSupport(CC_CPU_SSE4_1);
    #endif

        int bh0 = MIN(BLOCK_SZ/2, dst->rows);
        int bw0 = MIN(BLOCK_SZ*BLOCK_SZ/bh0, dst->cols);
        bh0 = MIN(BLOCK_SZ*BLOCK_SZ/bw0, dst->rows);

        for( y = range.start; y < range.end; y += bh0 )
        {
            for( x = 0; x < dst->cols; x += bw0 )
            {
                IM_DEF(tmp, 10);
                int bw = MIN( bw0, dst->cols - x);
                int bh = MIN( bh0, range.end - y);

                img_t *_XY = cvMat(tmp++, bh, bw, CC_16S, 2, XY);
                img_t *dpart = cvGetSubRect1(dst, tmp++, x, y, bw, bh);

                for( y1 = 0; y1 < bh; y1++ )
                {
                    short* xy = XY + y1*bw*2;
                    int X0, Y0;
                    X0 = ROUND((M[1]*(y + y1) + M[2])*AB_SCALE) + round_delta;
                    Y0 = ROUND((M[4]*(y + y1) + M[5])*AB_SCALE) + round_delta;

                    if( interpolation == CC_INTER_NEAREST )
                    {
                        x1 = 0;
                        #if CC_NEON
                        int32x4_t v_X0 = vdupq_n_s32(X0), v_Y0 = vdupq_n_s32(Y0);
                        for( ; x1 <= bw - 8; x1 += 8 )
                        {
                            int16x8x2_t v_dst;
                            v_dst.val[0] = vcombine_s16(vqmovn_s32(vshrq_n_s32(vaddq_s32(v_X0, vld1q_s32(adelta + x + x1)), AB_BITS)),
                                                        vqmovn_s32(vshrq_n_s32(vaddq_s32(v_X0, vld1q_s32(adelta + x + x1 + 4)), AB_BITS)));
                            v_dst.val[1] = vcombine_s16(vqmovn_s32(vshrq_n_s32(vaddq_s32(v_Y0, vld1q_s32(bdelta + x + x1)), AB_BITS)),
                                                        vqmovn_s32(vshrq_n_s32(vaddq_s32(v_Y0, vld1q_s32(bdelta + x + x1 + 4)), AB_BITS)));

                            vst2q_s16(xy + (x1 << 1), v_dst);
                        }
                        #elif CC_SSE4_1
                        if (useSSE4_1)
                        {
                            __m128i v_X0 = _mm_set1_epi32(X0);
                            __m128i v_Y0 = _mm_set1_epi32(Y0);
                            for ( ; x1 <= bw - 16; x1 += 16)
                            {
                                __m128i v_x0 = _mm_packs_epi32(_mm_srai_epi32(_mm_add_epi32(v_X0, _mm_loadu_si128((__m128i const *)(adelta + x + x1))), AB_BITS),
                                                               _mm_srai_epi32(_mm_add_epi32(v_X0, _mm_loadu_si128((__m128i const *)(adelta + x + x1 + 4))), AB_BITS));
                                __m128i v_x1 = _mm_packs_epi32(_mm_srai_epi32(_mm_add_epi32(v_X0, _mm_loadu_si128((__m128i const *)(adelta + x + x1 + 8))), AB_BITS),
                                                               _mm_srai_epi32(_mm_add_epi32(v_X0, _mm_loadu_si128((__m128i const *)(adelta + x + x1 + 12))), AB_BITS));

                                __m128i v_y0 = _mm_packs_epi32(_mm_srai_epi32(_mm_add_epi32(v_Y0, _mm_loadu_si128((__m128i const *)(bdelta + x + x1))), AB_BITS),
                                                               _mm_srai_epi32(_mm_add_epi32(v_Y0, _mm_loadu_si128((__m128i const *)(bdelta + x + x1 + 4))), AB_BITS));
                                __m128i v_y1 = _mm_packs_epi32(_mm_srai_epi32(_mm_add_epi32(v_Y0, _mm_loadu_si128((__m128i const *)(bdelta + x + x1 + 8))), AB_BITS),
                                                               _mm_srai_epi32(_mm_add_epi32(v_Y0, _mm_loadu_si128((__m128i const *)(bdelta + x + x1 + 12))), AB_BITS));

                                _mm_interleave_epi16(v_x0, v_x1, v_y0, v_y1);

                                _mm_storeu_si128((__m128i *)(xy + x1 * 2), v_x0);
                                _mm_storeu_si128((__m128i *)(xy + x1 * 2 + 8), v_x1);
                                _mm_storeu_si128((__m128i *)(xy + x1 * 2 + 16), v_y0);
                                _mm_storeu_si128((__m128i *)(xy + x1 * 2 + 24), v_y1);
                            }
                        }
                        #endif
                        for( ; x1 < bw; x1++ )
                        {
                            int X = (X0 + adelta[x+x1]) >> AB_BITS;
                            int Y = (Y0 + bdelta[x+x1]) >> AB_BITS;
                            xy[x1*2] = int_to_short(X);
                            xy[x1*2+1] = int_to_short(Y);
                        }
                    }
                    else
                    {
                        short* alpha = A + y1*bw;
                        x1 = 0;
                    #if CC_SSE2_
                        if( useSSE2 )
                        {
                            __m128i fxy_mask = _mm_set1_epi32(CC_INTER_TAB_SIZE - 1);
                            __m128i XX = _mm_set1_epi32(X0), YY = _mm_set1_epi32(Y0);
                            for( ; x1 <= bw - 8; x1 += 8 )
                            {
                                __m128i tx0, tx1, ty0, ty1;
                                tx0 = _mm_add_epi32(_mm_loadu_si128((const __m128i*)(adelta + x + x1)), XX);
                                ty0 = _mm_add_epi32(_mm_loadu_si128((const __m128i*)(bdelta + x + x1)), YY);
                                tx1 = _mm_add_epi32(_mm_loadu_si128((const __m128i*)(adelta + x + x1 + 4)), XX);
                                ty1 = _mm_add_epi32(_mm_loadu_si128((const __m128i*)(bdelta + x + x1 + 4)), YY);

                                tx0 = _mm_srai_epi32(tx0, AB_BITS - CC_INTER_BITS);
                                ty0 = _mm_srai_epi32(ty0, AB_BITS - CC_INTER_BITS);
                                tx1 = _mm_srai_epi32(tx1, AB_BITS - CC_INTER_BITS);
                                ty1 = _mm_srai_epi32(ty1, AB_BITS - CC_INTER_BITS);

                                __m128i fx_ = _mm_packs_epi32(_mm_and_si128(tx0, fxy_mask),
                                                            _mm_and_si128(tx1, fxy_mask));
                                __m128i fy_ = _mm_packs_epi32(_mm_and_si128(ty0, fxy_mask),
                                                            _mm_and_si128(ty1, fxy_mask));
                                tx0 = _mm_packs_epi32(_mm_srai_epi32(tx0, CC_INTER_BITS),
                                                            _mm_srai_epi32(tx1, CC_INTER_BITS));
                                ty0 = _mm_packs_epi32(_mm_srai_epi32(ty0, CC_INTER_BITS),
                                                    _mm_srai_epi32(ty1, CC_INTER_BITS));
                                fx_ = _mm_adds_epi16(fx_, _mm_slli_epi16(fy_, CC_INTER_BITS));

                                _mm_storeu_si128((__m128i*)(xy + x1*2), _mm_unpacklo_epi16(tx0, ty0));
                                _mm_storeu_si128((__m128i*)(xy + x1*2 + 8), _mm_unpackhi_epi16(tx0, ty0));
                                _mm_storeu_si128((__m128i*)(alpha + x1), fx_);
                            }
                        }
                    #elif CC_NEON
                        int32x4_t v__X0 = vdupq_n_s32(X0), v__Y0 = vdupq_n_s32(Y0), v_mask = vdupq_n_s32(CC_INTER_TAB_SIZE - 1);
                        for( ; x1 <= bw - 8; x1 += 8 )
                        {
                            int32x4_t v_X0 = vshrq_n_s32(vaddq_s32(v__X0, vld1q_s32(adelta + x + x1)), AB_BITS - CC_INTER_BITS);
                            int32x4_t v_Y0 = vshrq_n_s32(vaddq_s32(v__Y0, vld1q_s32(bdelta + x + x1)), AB_BITS - CC_INTER_BITS);
                            int32x4_t v_X1 = vshrq_n_s32(vaddq_s32(v__X0, vld1q_s32(adelta + x + x1 + 4)), AB_BITS - CC_INTER_BITS);
                            int32x4_t v_Y1 = vshrq_n_s32(vaddq_s32(v__Y0, vld1q_s32(bdelta + x + x1 + 4)), AB_BITS - CC_INTER_BITS);

                            int16x8x2_t v_xy;
                            v_xy.val[0] = vcombine_s16(vqmovn_s32(vshrq_n_s32(v_X0, CC_INTER_BITS)), vqmovn_s32(vshrq_n_s32(v_X1, CC_INTER_BITS)));
                            v_xy.val[1] = vcombine_s16(vqmovn_s32(vshrq_n_s32(v_Y0, CC_INTER_BITS)), vqmovn_s32(vshrq_n_s32(v_Y1, CC_INTER_BITS)));

                            vst2q_s16(xy + (x1 << 1), v_xy);

                            int16x4_t v_alpha0 = vmovn_s32(vaddq_s32(vshlq_n_s32(vandq_s32(v_Y0, v_mask), CC_INTER_BITS),
                                                                     vandq_s32(v_X0, v_mask)));
                            int16x4_t v_alpha1 = vmovn_s32(vaddq_s32(vshlq_n_s32(vandq_s32(v_Y1, v_mask), CC_INTER_BITS),
                                                                     vandq_s32(v_X1, v_mask)));
                            vst1q_s16(alpha + x1, vcombine_s16(v_alpha0, v_alpha1));
                        }
                    #endif
                        for( ; x1 < bw; x1++ )
                        {
                            int X = (X0 + adelta[x+x1]) >> (AB_BITS - CC_INTER_BITS);
                            int Y = (Y0 + bdelta[x+x1]) >> (AB_BITS - CC_INTER_BITS);
                            xy[x1*2] = int_to_short(X >> CC_INTER_BITS);
                            xy[x1*2+1] = int_to_short(Y >> CC_INTER_BITS);
                            alpha[x1] = (short)((Y & (CC_INTER_TAB_SIZE-1))*CC_INTER_TAB_SIZE +
                                    (X & (CC_INTER_TAB_SIZE-1)));
                        }
                    }
                }

                if( interpolation == CC_INTER_NEAREST )
                    remap( src, dpart, _XY, NULL, interpolation, borderType, borderValue );
                else
                {
                    img_t _matA[1];
                    cvMat(_matA, bh, bw, CC_16U, 1, A);
                    remap( src, dpart, _XY, _matA, interpolation, borderType, borderValue );
                }
            }
        }
    }

private:
    const img_t *src;
    img_t *dst;
    int interpolation, borderType;
    CScalar borderValue;
    int *adelta, *bdelta;
    const double *M;
};


#if defined (HAVE_IPP) && IPP_VERSION_X100 >= 810 && IPP_DISABLE_BLOCK
class IPPWarpAffineInvoker :
    public CParallelLoopBody
{
public:
    IPPWarpAffineInvoker(img_t* src, img_t* dst, double (&_coeffs)[2][3], int &_interpolation, int _borderType,
                         const CScalar &_borderValue, ippiWarpAffineBackFunc _func, bool *_ok) :
        CParallelLoopBody(), src(src), dst(dst), mode(_interpolation), coeffs(_coeffs),
        borderType(_borderType), borderValue(_borderValue), func(_func), ok(_ok)
    {
        *ok = true;
    }

    virtual void operator() (const CRange& range) const
    {
        IppiSize srcsize = { src->cols, src->rows };
        IppiRect srcroi = { 0, 0, src->cols, src->rows };
        IppiRect dstroi = { 0, range.start, dst->cols, range.end - range.start };
        int cnn = CC_MAT_CN(src);
        if( borderType == CC_BORDER_CONSTANT )
        {
            IppiSize setSize = { dst->cols, range.end - range.start };
            void *dataPointer = dst.ptr(range.start);
            if( !IPPSet( borderValue, dataPointer, (int)dst->step[0], setSize, cnn, src CC_MAT_DEPTH() ) )
            {
                *ok = false;
                return;
            }
        }

        // Aug 2013: problem in IPP 7.1, 8.0 : sometimes function return ippStsCoeffErr
        IppStatus status = CC_INSTRUMENT_FUN_PTR_CALL_IPP(func,( src->tt.data, srcsize, (int)src->step[0], srcroi, dst->tt.data,
                                (int)dst->step[0], dstroi, coeffs, mode ));
        if( status < 0)
            *ok = false;
        else
        {
            CC_IMPL_ADD(CC_IMPL_IPP|CC_IMPL_MT);
        }
    }
private:
    img_t* src;
    img_t* dst;
    int mode;
    double (&coeffs)[2][3];
    int borderType;
    CScalar borderValue;
    ippiWarpAffineBackFunc func;
    bool *ok;
    const IPPWarpAffineInvoker& operator= (const IPPWarpAffineInvoker&);
};
#endif

#ifdef HAVE_OPENCL

enum { OCL_OP_PERSPECTIVE = 1, OCL_OP_AFFINE = 0 };

static bool ocl_warpTransform_cols4(const img_t* src, img_t* dst, const img_t* M0,
                                    CSize dsize, int flags, int borderType, const CScalar& borderValue,
                                    int op_type)
{
    CC_Assert(op_type == OCL_OP_AFFINE || op_type == OCL_OP_PERSPECTIVE);
    const ocl::Device & dev = ocl::Device::getDefault();
    int type = src->tid, dtype = dst->tid, depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type);

    int interpolation = flags & CC_INTER_MAX;
    if( interpolation == CC_INTER_AREA )
        interpolation = CC_INTER_LINEAR;

    if ( !dev.isIntel() || !(type == CC_8UC1) ||
         !(dtype == CC_8UC1) || !(dst->cols() % 4 == 0) ||
         !(borderType == CC_BORDER_CONSTANT &&
          (interpolation == CC_INTER_NEAREST || interpolation == CC_INTER_LINEAR || interpolation == CC_INTER_CUBIC)))
        return false;

    const char * const warp_op[2] = { "Affine", "Perspective" };
    const char * const interpolationMap[3] = { "nearest", "linear", "cubic" };
    ocl::ProgramSource program = ocl::imgproc::warp_transform_oclsrc;
    String kernelName = format("warp%s_%s_8u", warp_op[op_type], interpolationMap[interpolation]);

    bool is32f = (interpolation == CC_INTER_CUBIC || interpolation == CC_INTER_LINEAR) && op_type == OCL_OP_AFFINE;
    int wdepth = interpolation == CC_INTER_NEAREST ? depth : MAX(is32f ? CC_32F : CC_32S, depth);
    int sctype = CC_MAKETYPE(wdepth, cn);

    ocl::Kernel k;
    String opts = format("-D ST=%s", ocl::typeToStr(sctype));

    k cvSetMat(kernelName.c_str(), program, opts);
    if (k.empty())
        return false;

    float borderBuf[] = { 0, 0, 0, 0 };
    scalarToRawData(borderValue, borderBuf, sctype);

    UMat src = src.getUMat(), M0;
    dst cvSetMat( dsize CC_SIZE_AREA() == 0 ? cvGetSize(src) : dsize, src->tid );
    UMat dst = dst.getUMat();

    float M[9];
    int matRows = (op_type == OCL_OP_AFFINE ? 2 : 3);
    img_t matM(matRows, 3, CC_32F, M), M1 = M0;
    CC_Assert( (M1->tid == CC_32F || M1->tid == CC_64F) && M1->rows == matRows && M1->cols == 3 );
    M1.convertTo(matM, matM->tid);

    if( !(flags & CC_WARP_INVERSE_MAP) )
    {
        if (op_type == OCL_OP_PERSPECTIVE)
            invert(matM, matM);
        else
        {
            float D = M[0]*M[4] - M[1]*M[3];
            D = D != 0 ? 1.f/D : 0;
            float A11 = M[4]*D, A22=M[0]*D;
            M[0] = A11; M[1] *= -D;
            M[3] *= -D; M[4] = A22;
            float b1 = -M[0]*M[2] - M[1]*M[5];
            float b2 = -M[3]*M[2] - M[4]*M[5];
            M[2] = b1; M[5] = b2;
        }
    }
    matM.convertTo(M0, CC_32F);

    k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst), ocl::KernelArg::PtrReadOnly(M0),
           ocl::KernelArg(0, 0, 0, 0, borderBuf, CC_TYPE_SIZE(sctype)));

    size_t globalThreads[2];
    globalThreads[0] = (size_t)(dst->cols / 4);
    globalThreads[1] = (size_t)dst->rows;

    return k.run(2, globalThreads, NULL, false);
}

static bool ocl_warpTransform(const img_t* src, img_t* dst, const img_t* M0,
                              CSize dsize, int flags, int borderType, const CScalar& borderValue,
                              int op_type)
{
    CC_Assert(op_type == OCL_OP_AFFINE || op_type == OCL_OP_PERSPECTIVE);
    const ocl::Device & dev = ocl::Device::getDefault();

    int type = src->tid, depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type);
    const bool doubleSupport = dev.doubleFPConfig() > 0;

    int interpolation = flags & CC_INTER_MAX;
    if( interpolation == CC_INTER_AREA )
        interpolation = CC_INTER_LINEAR;
    int rowsPerWI = dev.isIntel() && op_type == OCL_OP_AFFINE && interpolation <= CC_INTER_LINEAR ? 4 : 1;

    if ( !(borderType == CC_BORDER_CONSTANT &&
           (interpolation == CC_INTER_NEAREST || interpolation == CC_INTER_LINEAR || interpolation == CC_INTER_CUBIC)) ||
         (!doubleSupport && depth == CC_64F) || cn > 4)
        return false;

    const char * const interpolationMap[3] = { "NEAREST", "LINEAR", "CUBIC" };
    ocl::ProgramSource program = op_type == OCL_OP_AFFINE ?
                ocl::imgproc::warp_affine_oclsrc : ocl::imgproc::warp_perspective_oclsrc;
    const char * const kernelName = op_type == OCL_OP_AFFINE ? "warpAffine" : "warpPerspective";

    int scalarcn = cn == 3 ? 4 : cn;
    bool is32f = !dev.isAMD() && (interpolation == CC_INTER_CUBIC || interpolation == CC_INTER_LINEAR) && op_type == OCL_OP_AFFINE;
    int wdepth = interpolation == CC_INTER_NEAREST ? depth : MAX(is32f ? CC_32F : CC_32S, depth);
    int sctype = CC_MAKETYPE(wdepth, scalarcn);

    ocl::Kernel k;
    String opts;
    if (interpolation == CC_INTER_NEAREST)
    {
        opts = format("-D CC_INTER_NEAREST -D T=%s%s -D T1=%s -D ST=%s -D cn=%d -D rowsPerWI=%d",
                      ocl::typeToStr(type), doubleSupport ? " -D DOUBLE_SUPPORT" : "",
                      ocl::typeToStr(CC_MAT_DEPTH(type)),
                      ocl::typeToStr(sctype), cn, rowsPerWI);
    }
    else
    {
        char cvt[2][50];
        opts = format("-D INTER_%s -D T=%s -D T1=%s -D ST=%s -D WT=%s -D depth=%d"
                      " -D convertToWT=%s -D convertToT=%s%s -D cn=%d -D rowsPerWI=%d",
                      interpolationMap[interpolation], ocl::typeToStr(type),
                      ocl::typeToStr(CC_MAT_DEPTH(type)),
                      ocl::typeToStr(sctype),
                      ocl::typeToStr(CC_MAKE_TYPE(wdepth, cn)), depth,
                      ocl::convertTypeStr(depth, wdepth, cn, cvt[0]),
                      ocl::convertTypeStr(wdepth, depth, cn, cvt[1]),
                      doubleSupport ? " -D DOUBLE_SUPPORT" : "", cn, rowsPerWI);
    }

    k cvSetMat(kernelName, program, opts);
    if (k.empty())
        return false;

    double borderBuf[] = { 0, 0, 0, 0 };
    scalarToRawData(borderValue, borderBuf, sctype);

    UMat src = src.getUMat(), M0;
    dst cvSetMat( CC_SIZE_AREA(dsize) == 0 ? cvGetSize(src) : dsize, src->tid );
    UMat dst = dst.getUMat();

    double M[9];
    int matRows = (op_type == OCL_OP_AFFINE ? 2 : 3);
    img_t matM(matRows, 3, CC_64F, M), M1 = M0;
    CC_Assert( (M1->tid == CC_32F || M1->tid == CC_64F) &&
               M1->rows == matRows && M1->cols == 3 );
    M1.convertTo(matM, matM->tid);

    if( !(flags & CC_WARP_INVERSE_MAP) )
    {
        if (op_type == OCL_OP_PERSPECTIVE)
            invert(matM, matM);
        else
        {
            double D = M[0]*M[4] - M[1]*M[3];
            D = D != 0 ? 1./D : 0;
            double A11 = M[4]*D, A22=M[0]*D;
            M[0] = A11; M[1] *= -D;
            M[3] *= -D; M[4] = A22;
            double b1 = -M[0]*M[2] - M[1]*M[5];
            double b2 = -M[3]*M[2] - M[4]*M[5];
            M[2] = b1; M[5] = b2;
        }
    }
    matM.convertTo(M0, doubleSupport ? CC_64F : CC_32F);

    k.args(ocl::KernelArg::ReadOnly(src), ocl::KernelArg::WriteOnly(dst), ocl::KernelArg::PtrReadOnly(M0),
           ocl::KernelArg(0, 0, 0, 0, borderBuf, CC_TYPE_SIZE(sctype)));

    size_t globalThreads[2] = { (size_t)dst->cols, ((size_t)dst->rows + rowsPerWI - 1) / rowsPerWI };
    return k.run(2, globalThreads, NULL, false);
}

#endif

static void hal_warpAffine(TypeId src_type, int cn,
                const uchar * src_data, size_t src_step, int src_width, int src_height,
                uchar * dst_data, size_t dst_step, int dst_width, int dst_height,
                const double M[6], int interpolation, int borderType, const double borderValue[4])
{
    CALL_HAL(warpAffine, cv_hal_warpAffine, (src_type, cn, src_data, src_step, src_width, src_height, dst_data, dst_step, dst_width, dst_height, M, interpolation, borderType, borderValue));

    IM_DEF(tmp, 4);
    img_t *src = cvMat(tmp++, src_height, src_width, src_type, cn, (uchar*)(src_data), src_step);
    img_t *dst = cvMat(tmp++, dst_height, dst_width, src_type, cn, dst_data, dst_step);

    int x;
    int* _abdelta = MALLOC(int, dst->cols*2);
    int* adelta = &_abdelta[0], *bdelta = adelta + dst->cols;
    const int AB_BITS = MAX(10, (int)CC_INTER_BITS);
    const int AB_SCALE = 1 << AB_BITS;

    for( x = 0; x < dst->cols; x++ )
    {
        adelta[x] = double_to_int(M[0]*x*AB_SCALE);
        bdelta[x] = double_to_int(M[3]*x*AB_SCALE);
    }

    CRange range = cRange(0, dst->rows);
    WarpAffineInvoker invoker(src, dst, interpolation, borderType,
                              cScalar(borderValue[0], borderValue[1], borderValue[2], borderValue[3]),
                              adelta, bdelta, M);
    parallel_for_(range, invoker, CC_MAT_TOTAL(dst)/(double)(1<<16));
    FREE(_abdelta);
}

 


static void warpAffine( const img_t* src, img_t* dst, const img_t* M0, CSize dsize,
                     int flags, int borderType = CC_BORDER_CONSTANT, const CScalar& borderValue = cScalarAll(0) )
{
    CC_INSTRUMENT_REGION()

    CC_OCL_RUN(src.dims() <= 2 && dst.isUMat() &&
               src->cols() <= SHRT_MAX && src->rows() <= SHRT_MAX,
               ocl_warpTransform_cols4(src, dst, M0, dsize, flags, borderType,
                                       borderValue, OCL_OP_AFFINE))

    CC_OCL_RUN(src.dims() <= 2 && dst.isUMat(),
               ocl_warpTransform(src, dst, M0, dsize, flags, borderType,
                                 borderValue, OCL_OP_AFFINE))

    dsize = CC_SIZE_AREA(dsize) == 0 ? cvGetSize(src) : dsize;
    cvSetMat( dst, src->rows, src->cols, src->tid, CC_MAT_CN(src) );
    CC_Assert( src->cols > 0 && src->rows > 0 );
    IM_BEGIN(im, 10);
    if( dst->tt.data == src->tt.data )
        src = cvCloneMat(im++, src);

    double M[6];
    img_t matM[1];
    cvMat(matM, 2, 3, CC_64F, 1, M);
    int interpolation = flags & CC_INTER_MAX;
    if( interpolation == CC_INTER_AREA )
        interpolation = CC_INTER_LINEAR;

    CC_Assert( (M0->tid == CC_32F || M0->tid == CC_64F) && M0->rows == 2 && M0->cols == 3 );
    cvConvertTo(M0, matM, matM->tid);

    if( !(flags & CC_WARP_INVERSE_MAP) )
    {
        double D = M[0]*M[4] - M[1]*M[3];
        D = D != 0 ? 1./D : 0;
        double A11 = M[4]*D, A22=M[0]*D;
        M[0] = A11; M[1] *= -D;
        M[3] *= -D; M[4] = A22;
        double b1 = -M[0]*M[2] - M[1]*M[5];
        double b2 = -M[3]*M[2] - M[4]*M[5];
        M[2] = b1; M[5] = b2;
    }

#if defined (HAVE_IPP) && IPP_VERSION_X100 >= 810 && IPP_DISABLE_BLOCK
    CC_IPP_CHECK()
    {
        int type = src->tid, depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type);
        if( ( depth == CC_8U || depth == CC_16U || depth == CC_32F ) &&
           ( cn == 1 || cn == 3 || cn == 4 ) &&
           ( interpolation == CC_INTER_NEAREST || interpolation == CC_INTER_LINEAR || interpolation == CC_INTER_CUBIC) &&
           ( borderType == CC_BORDER_TRANSPARENT || borderType == CC_BORDER_CONSTANT) )
        {
            ippiWarpAffineBackFunc ippFunc = 0;
            if ((flags & CC_WARP_INVERSE_MAP) != 0)
            {
                ippFunc =
                type == CC_8UC1 ? (ippiWarpAffineBackFunc)ippiWarpAffineBack_8u_C1R :
                type == CC_8UC3 ? (ippiWarpAffineBackFunc)ippiWarpAffineBack_8u_C3R :
                type == CC_8UC4 ? (ippiWarpAffineBackFunc)ippiWarpAffineBack_8u_C4R :
                type == CC_16UC1 ? (ippiWarpAffineBackFunc)ippiWarpAffineBack_16u_C1R :
                type == CC_16UC3 ? (ippiWarpAffineBackFunc)ippiWarpAffineBack_16u_C3R :
                type == CC_16UC4 ? (ippiWarpAffineBackFunc)ippiWarpAffineBack_16u_C4R :
                type == CC_32FC1 ? (ippiWarpAffineBackFunc)ippiWarpAffineBack_32f_C1R :
                type == CC_32FC3 ? (ippiWarpAffineBackFunc)ippiWarpAffineBack_32f_C3R :
                type == CC_32FC4 ? (ippiWarpAffineBackFunc)ippiWarpAffineBack_32f_C4R :
                0;
            }
            else
            {
                ippFunc =
                type == CC_8UC1 ? (ippiWarpAffineBackFunc)ippiWarpAffine_8u_C1R :
                type == CC_8UC3 ? (ippiWarpAffineBackFunc)ippiWarpAffine_8u_C3R :
                type == CC_8UC4 ? (ippiWarpAffineBackFunc)ippiWarpAffine_8u_C4R :
                type == CC_16UC1 ? (ippiWarpAffineBackFunc)ippiWarpAffine_16u_C1R :
                type == CC_16UC3 ? (ippiWarpAffineBackFunc)ippiWarpAffine_16u_C3R :
                type == CC_16UC4 ? (ippiWarpAffineBackFunc)ippiWarpAffine_16u_C4R :
                type == CC_32FC1 ? (ippiWarpAffineBackFunc)ippiWarpAffine_32f_C1R :
                type == CC_32FC3 ? (ippiWarpAffineBackFunc)ippiWarpAffine_32f_C3R :
                type == CC_32FC4 ? (ippiWarpAffineBackFunc)ippiWarpAffine_32f_C4R :
                0;
            }
            int mode =
            interpolation == CC_INTER_LINEAR ? IPPI_INTER_LINEAR :
            interpolation == CC_INTER_NEAREST ? IPPI_INTER_NN :
            interpolation == CC_INTER_CUBIC ? IPPI_INTER_CUBIC :
            0;
            CC_Assert(mode && ippFunc);

            double coeffs[2][3];
            for( int i = 0; i < 2; i++ )
                for( int j = 0; j < 3; j++ )
                    coeffs[i][j] = matM.at<double>(i, j);

            bool ok;
            CRange range(0, dst->rows);
            IPPWarpAffineInvoker invoker(src, dst, coeffs, mode, borderType, borderValue, ippFunc, &ok);
            parallel_for_(range, invoker, CC_MAT_TOTAL(dst)/(double)(1<<16));
            if( ok )
            {
                CC_IMPL_ADD(CC_IMPL_IPP|CC_IMPL_MT);
                return;
            }
            setIppErrorStatus();
        }
    }
#endif

    hal_warpAffine(src->tid, CC_MAT_CN(src), src->tt.data, src->step, src->cols, src->rows, dst->tt.data, dst->step, dst->cols, dst->rows,
                    M, interpolation, borderType, borderValue.val);
    IM_END(im);
}

class WarpPerspectiveInvoker :
    public CParallelLoopBody
{
public:
    WarpPerspectiveInvoker(const img_t* src, img_t* dst, const double *_M, int _interpolation,
                           int _borderType, const CScalar &_borderValue) :
        CParallelLoopBody(), src(src), dst(dst), M(_M), interpolation(_interpolation),
        borderType(_borderType), borderValue(_borderValue)
    {
    }

    virtual void operator() (const CRange& range) const
    {
        const int BLOCK_SZ = 32;
        short XY[BLOCK_SZ*BLOCK_SZ*2], A[BLOCK_SZ*BLOCK_SZ];
        int x, y, x1, y1, width = dst->cols, height = dst->rows;

        int bh0 = MIN(BLOCK_SZ/2, height);
        int bw0 = MIN(BLOCK_SZ*BLOCK_SZ/bh0, width);
        bh0 = MIN(BLOCK_SZ*BLOCK_SZ/bw0, height);

        #if CC_SSE4_1
        bool haveSSE4_1 = checkHardwareSupport(CC_CPU_SSE4_1);
        __m128d v_M0 = _mm_set1_pd(M[0]);
        __m128d v_M3 = _mm_set1_pd(M[3]);
        __m128d v_M6 = _mm_set1_pd(M[6]);
        __m128d v_intmax = _mm_set1_pd((double)INT_MAX);
        __m128d v_intmin = _mm_set1_pd((double)INT_MIN);
        __m128d v_2 = _mm_set1_pd(2),
                v_zero = _mm_setzero_pd(),
                v_1 = _mm_set1_pd(1),
                v_its = _mm_set1_pd(CC_INTER_TAB_SIZE);
        __m128i v_itsi1 = _mm_set1_epi32(CC_INTER_TAB_SIZE - 1);
        #endif

        for( y = range.start; y < range.end; y += bh0 )
        {
            for( x = 0; x < width; x += bw0 )
            {
                int bw = MIN( bw0, width - x);
                int bh = MIN( bh0, range.end - y); // height

                img_t _XY[1], dpart[1];
                cvMat(_XY, bh, bw, CC_16S, 2, XY);
                cvGetSubRect1(dst, dpart, x, y, bw, bh);

                for( y1 = 0; y1 < bh; y1++ )
                {
                    short* xy = XY + y1*bw*2;
                    double X0 = M[0]*x + M[1]*(y + y1) + M[2];
                    double Y0 = M[3]*x + M[4]*(y + y1) + M[5];
                    double W0 = M[6]*x + M[7]*(y + y1) + M[8];

                    if( interpolation == CC_INTER_NEAREST )
                    {
                        x1 = 0;

                        #if CC_SSE4_1
                        if (haveSSE4_1)
                        {
                            __m128d v_X0d = _mm_set1_pd(X0);
                            __m128d v_Y0d = _mm_set1_pd(Y0);
                            __m128d v_W0 = _mm_set1_pd(W0);
                            __m128d v_x1 = _mm_set_pd(1, 0);

                            for( ; x1 <= bw - 16; x1 += 16 )
                            {
                                // 0-3
                                __m128i v_X0, v_Y0;
                                {
                                    __m128d v_W = _mm_add_pd(_mm_mul_pd(v_M6, v_x1), v_W0);
                                    v_W = _mm_andnot_pd(_mm_cmpeq_pd(v_W, v_zero), _mm_div_pd(v_1, v_W));
                                    __m128d v_fX0 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_X0d, _mm_mul_pd(v_M0, v_x1)), v_W)));
                                    __m128d v_fY0 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_Y0d, _mm_mul_pd(v_M3, v_x1)), v_W)));
                                    v_x1 = _mm_add_pd(v_x1, v_2);

                                    v_W = _mm_add_pd(_mm_mul_pd(v_M6, v_x1), v_W0);
                                    v_W = _mm_andnot_pd(_mm_cmpeq_pd(v_W, v_zero), _mm_div_pd(v_1, v_W));
                                    __m128d v_fX1 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_X0d, _mm_mul_pd(v_M0, v_x1)), v_W)));
                                    __m128d v_fY1 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_Y0d, _mm_mul_pd(v_M3, v_x1)), v_W)));
                                    v_x1 = _mm_add_pd(v_x1, v_2);

                                    v_X0 = _mm_castps_si128(_mm_movelh_ps(_mm_castsi128_ps(_mm_cvtpd_epi32(v_fX0)),
                                                                          _mm_castsi128_ps(_mm_cvtpd_epi32(v_fX1))));
                                    v_Y0 = _mm_castps_si128(_mm_movelh_ps(_mm_castsi128_ps(_mm_cvtpd_epi32(v_fY0)),
                                                                          _mm_castsi128_ps(_mm_cvtpd_epi32(v_fY1))));
                                }

                                // 4-8
                                __m128i v_X1, v_Y1;
                                {
                                    __m128d v_W = _mm_add_pd(_mm_mul_pd(v_M6, v_x1), v_W0);
                                    v_W = _mm_andnot_pd(_mm_cmpeq_pd(v_W, v_zero), _mm_div_pd(v_1, v_W));
                                    __m128d v_fX0 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_X0d, _mm_mul_pd(v_M0, v_x1)), v_W)));
                                    __m128d v_fY0 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_Y0d, _mm_mul_pd(v_M3, v_x1)), v_W)));
                                    v_x1 = _mm_add_pd(v_x1, v_2);

                                    v_W = _mm_add_pd(_mm_mul_pd(v_M6, v_x1), v_W0);
                                    v_W = _mm_andnot_pd(_mm_cmpeq_pd(v_W, v_zero), _mm_div_pd(v_1, v_W));
                                    __m128d v_fX1 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_X0d, _mm_mul_pd(v_M0, v_x1)), v_W)));
                                    __m128d v_fY1 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_Y0d, _mm_mul_pd(v_M3, v_x1)), v_W)));
                                    v_x1 = _mm_add_pd(v_x1, v_2);

                                    v_X1 = _mm_castps_si128(_mm_movelh_ps(_mm_castsi128_ps(_mm_cvtpd_epi32(v_fX0)),
                                                                          _mm_castsi128_ps(_mm_cvtpd_epi32(v_fX1))));
                                    v_Y1 = _mm_castps_si128(_mm_movelh_ps(_mm_castsi128_ps(_mm_cvtpd_epi32(v_fY0)),
                                                                          _mm_castsi128_ps(_mm_cvtpd_epi32(v_fY1))));
                                }

                                // 8-11
                                __m128i v_X2, v_Y2;
                                {
                                    __m128d v_W = _mm_add_pd(_mm_mul_pd(v_M6, v_x1), v_W0);
                                    v_W = _mm_andnot_pd(_mm_cmpeq_pd(v_W, v_zero), _mm_div_pd(v_1, v_W));
                                    __m128d v_fX0 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_X0d, _mm_mul_pd(v_M0, v_x1)), v_W)));
                                    __m128d v_fY0 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_Y0d, _mm_mul_pd(v_M3, v_x1)), v_W)));
                                    v_x1 = _mm_add_pd(v_x1, v_2);

                                    v_W = _mm_add_pd(_mm_mul_pd(v_M6, v_x1), v_W0);
                                    v_W = _mm_andnot_pd(_mm_cmpeq_pd(v_W, v_zero), _mm_div_pd(v_1, v_W));
                                    __m128d v_fX1 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_X0d, _mm_mul_pd(v_M0, v_x1)), v_W)));
                                    __m128d v_fY1 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_Y0d, _mm_mul_pd(v_M3, v_x1)), v_W)));
                                    v_x1 = _mm_add_pd(v_x1, v_2);

                                    v_X2 = _mm_castps_si128(_mm_movelh_ps(_mm_castsi128_ps(_mm_cvtpd_epi32(v_fX0)),
                                                                          _mm_castsi128_ps(_mm_cvtpd_epi32(v_fX1))));
                                    v_Y2 = _mm_castps_si128(_mm_movelh_ps(_mm_castsi128_ps(_mm_cvtpd_epi32(v_fY0)),
                                                                          _mm_castsi128_ps(_mm_cvtpd_epi32(v_fY1))));
                                }

                                // 12-15
                                __m128i v_X3, v_Y3;
                                {
                                    __m128d v_W = _mm_add_pd(_mm_mul_pd(v_M6, v_x1), v_W0);
                                    v_W = _mm_andnot_pd(_mm_cmpeq_pd(v_W, v_zero), _mm_div_pd(v_1, v_W));
                                    __m128d v_fX0 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_X0d, _mm_mul_pd(v_M0, v_x1)), v_W)));
                                    __m128d v_fY0 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_Y0d, _mm_mul_pd(v_M3, v_x1)), v_W)));
                                    v_x1 = _mm_add_pd(v_x1, v_2);

                                    v_W = _mm_add_pd(_mm_mul_pd(v_M6, v_x1), v_W0);
                                    v_W = _mm_andnot_pd(_mm_cmpeq_pd(v_W, v_zero), _mm_div_pd(v_1, v_W));
                                    __m128d v_fX1 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_X0d, _mm_mul_pd(v_M0, v_x1)), v_W)));
                                    __m128d v_fY1 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_Y0d, _mm_mul_pd(v_M3, v_x1)), v_W)));
                                    v_x1 = _mm_add_pd(v_x1, v_2);

                                    v_X3 = _mm_castps_si128(_mm_movelh_ps(_mm_castsi128_ps(_mm_cvtpd_epi32(v_fX0)),
                                                                          _mm_castsi128_ps(_mm_cvtpd_epi32(v_fX1))));
                                    v_Y3 = _mm_castps_si128(_mm_movelh_ps(_mm_castsi128_ps(_mm_cvtpd_epi32(v_fY0)),
                                                                          _mm_castsi128_ps(_mm_cvtpd_epi32(v_fY1))));
                                }

                                // convert to 16s
                                v_X0 = _mm_packs_epi32(v_X0, v_X1);
                                v_X1 = _mm_packs_epi32(v_X2, v_X3);
                                v_Y0 = _mm_packs_epi32(v_Y0, v_Y1);
                                v_Y1 = _mm_packs_epi32(v_Y2, v_Y3);

                                _mm_interleave_epi16(v_X0, v_X1, v_Y0, v_Y1);

                                _mm_storeu_si128((__m128i *)(xy + x1 * 2), v_X0);
                                _mm_storeu_si128((__m128i *)(xy + x1 * 2 + 8), v_X1);
                                _mm_storeu_si128((__m128i *)(xy + x1 * 2 + 16), v_Y0);
                                _mm_storeu_si128((__m128i *)(xy + x1 * 2 + 24), v_Y1);
                            }
                        }
                        #endif

                        for( ; x1 < bw; x1++ )
                        {
                            double W = W0 + M[6]*x1;
                            W = W ? 1./W : 0;
                            double fX = MAX((double)INT_MIN, MIN((double)INT_MAX, (X0 + M[0]*x1)*W));
                            double fY = MAX((double)INT_MIN, MIN((double)INT_MAX, (Y0 + M[3]*x1)*W));
                            int X, Y;
                            X = double_int(fX);
                            Y = double_int(fY);
                            xy[x1*2] = int_short(X);
                            xy[x1*2+1] = int_short(Y);
                        }
                    }
                    else
                    {
                        short* alpha = A + y1*bw;
                        x1 = 0;

                        #if CC_SSE4_1
                        if (haveSSE4_1)
                        {
                            __m128d v_X0d = _mm_set1_pd(X0);
                            __m128d v_Y0d = _mm_set1_pd(Y0);
                            __m128d v_W0 = _mm_set1_pd(W0);
                            __m128d v_x1 = _mm_set_pd(1, 0);

                            for( ; x1 <= bw - 16; x1 += 16 )
                            {
                                // 0-3
                                __m128i v_X0, v_Y0;
                                {
                                    __m128d v_W = _mm_add_pd(_mm_mul_pd(v_M6, v_x1), v_W0);
                                    v_W = _mm_andnot_pd(_mm_cmpeq_pd(v_W, v_zero), _mm_div_pd(v_its, v_W));
                                    __m128d v_fX0 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_X0d, _mm_mul_pd(v_M0, v_x1)), v_W)));
                                    __m128d v_fY0 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_Y0d, _mm_mul_pd(v_M3, v_x1)), v_W)));
                                    v_x1 = _mm_add_pd(v_x1, v_2);

                                    v_W = _mm_add_pd(_mm_mul_pd(v_M6, v_x1), v_W0);
                                    v_W = _mm_andnot_pd(_mm_cmpeq_pd(v_W, v_zero), _mm_div_pd(v_its, v_W));
                                    __m128d v_fX1 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_X0d, _mm_mul_pd(v_M0, v_x1)), v_W)));
                                    __m128d v_fY1 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_Y0d, _mm_mul_pd(v_M3, v_x1)), v_W)));
                                    v_x1 = _mm_add_pd(v_x1, v_2);

                                    v_X0 = _mm_castps_si128(_mm_movelh_ps(_mm_castsi128_ps(_mm_cvtpd_epi32(v_fX0)),
                                                                          _mm_castsi128_ps(_mm_cvtpd_epi32(v_fX1))));
                                    v_Y0 = _mm_castps_si128(_mm_movelh_ps(_mm_castsi128_ps(_mm_cvtpd_epi32(v_fY0)),
                                                                          _mm_castsi128_ps(_mm_cvtpd_epi32(v_fY1))));
                                }

                                // 4-8
                                __m128i v_X1, v_Y1;
                                {
                                    __m128d v_W = _mm_add_pd(_mm_mul_pd(v_M6, v_x1), v_W0);
                                    v_W = _mm_andnot_pd(_mm_cmpeq_pd(v_W, v_zero), _mm_div_pd(v_its, v_W));
                                    __m128d v_fX0 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_X0d, _mm_mul_pd(v_M0, v_x1)), v_W)));
                                    __m128d v_fY0 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_Y0d, _mm_mul_pd(v_M3, v_x1)), v_W)));
                                    v_x1 = _mm_add_pd(v_x1, v_2);

                                    v_W = _mm_add_pd(_mm_mul_pd(v_M6, v_x1), v_W0);
                                    v_W = _mm_andnot_pd(_mm_cmpeq_pd(v_W, v_zero), _mm_div_pd(v_its, v_W));
                                    __m128d v_fX1 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_X0d, _mm_mul_pd(v_M0, v_x1)), v_W)));
                                    __m128d v_fY1 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_Y0d, _mm_mul_pd(v_M3, v_x1)), v_W)));
                                    v_x1 = _mm_add_pd(v_x1, v_2);

                                    v_X1 = _mm_castps_si128(_mm_movelh_ps(_mm_castsi128_ps(_mm_cvtpd_epi32(v_fX0)),
                                                                          _mm_castsi128_ps(_mm_cvtpd_epi32(v_fX1))));
                                    v_Y1 = _mm_castps_si128(_mm_movelh_ps(_mm_castsi128_ps(_mm_cvtpd_epi32(v_fY0)),
                                                                          _mm_castsi128_ps(_mm_cvtpd_epi32(v_fY1))));
                                }

                                // 8-11
                                __m128i v_X2, v_Y2;
                                {
                                    __m128d v_W = _mm_add_pd(_mm_mul_pd(v_M6, v_x1), v_W0);
                                    v_W = _mm_andnot_pd(_mm_cmpeq_pd(v_W, v_zero), _mm_div_pd(v_its, v_W));
                                    __m128d v_fX0 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_X0d, _mm_mul_pd(v_M0, v_x1)), v_W)));
                                    __m128d v_fY0 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_Y0d, _mm_mul_pd(v_M3, v_x1)), v_W)));
                                    v_x1 = _mm_add_pd(v_x1, v_2);

                                    v_W = _mm_add_pd(_mm_mul_pd(v_M6, v_x1), v_W0);
                                    v_W = _mm_andnot_pd(_mm_cmpeq_pd(v_W, v_zero), _mm_div_pd(v_its, v_W));
                                    __m128d v_fX1 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_X0d, _mm_mul_pd(v_M0, v_x1)), v_W)));
                                    __m128d v_fY1 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_Y0d, _mm_mul_pd(v_M3, v_x1)), v_W)));
                                    v_x1 = _mm_add_pd(v_x1, v_2);

                                    v_X2 = _mm_castps_si128(_mm_movelh_ps(_mm_castsi128_ps(_mm_cvtpd_epi32(v_fX0)),
                                                                          _mm_castsi128_ps(_mm_cvtpd_epi32(v_fX1))));
                                    v_Y2 = _mm_castps_si128(_mm_movelh_ps(_mm_castsi128_ps(_mm_cvtpd_epi32(v_fY0)),
                                                                          _mm_castsi128_ps(_mm_cvtpd_epi32(v_fY1))));
                                }

                                // 12-15
                                __m128i v_X3, v_Y3;
                                {
                                    __m128d v_W = _mm_add_pd(_mm_mul_pd(v_M6, v_x1), v_W0);
                                    v_W = _mm_andnot_pd(_mm_cmpeq_pd(v_W, v_zero), _mm_div_pd(v_its, v_W));
                                    __m128d v_fX0 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_X0d, _mm_mul_pd(v_M0, v_x1)), v_W)));
                                    __m128d v_fY0 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_Y0d, _mm_mul_pd(v_M3, v_x1)), v_W)));
                                    v_x1 = _mm_add_pd(v_x1, v_2);

                                    v_W = _mm_add_pd(_mm_mul_pd(v_M6, v_x1), v_W0);
                                    v_W = _mm_andnot_pd(_mm_cmpeq_pd(v_W, v_zero), _mm_div_pd(v_its, v_W));
                                    __m128d v_fX1 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_X0d, _mm_mul_pd(v_M0, v_x1)), v_W)));
                                    __m128d v_fY1 = _mm_max_pd(v_intmin, _mm_min_pd(v_intmax, _mm_mul_pd(_mm_add_pd(v_Y0d, _mm_mul_pd(v_M3, v_x1)), v_W)));
                                    v_x1 = _mm_add_pd(v_x1, v_2);

                                    v_X3 = _mm_castps_si128(_mm_movelh_ps(_mm_castsi128_ps(_mm_cvtpd_epi32(v_fX0)),
                                                                          _mm_castsi128_ps(_mm_cvtpd_epi32(v_fX1))));
                                    v_Y3 = _mm_castps_si128(_mm_movelh_ps(_mm_castsi128_ps(_mm_cvtpd_epi32(v_fY0)),
                                                                          _mm_castsi128_ps(_mm_cvtpd_epi32(v_fY1))));
                                }

                                // store alpha
                                __m128i v_alpha0 = _mm_add_epi32(_mm_slli_epi32(_mm_and_si128(v_Y0, v_itsi1), CC_INTER_BITS),
                                                                 _mm_and_si128(v_X0, v_itsi1));
                                __m128i v_alpha1 = _mm_add_epi32(_mm_slli_epi32(_mm_and_si128(v_Y1, v_itsi1), CC_INTER_BITS),
                                                                 _mm_and_si128(v_X1, v_itsi1));
                                _mm_storeu_si128((__m128i *)(alpha + x1), _mm_packs_epi32(v_alpha0, v_alpha1));

                                v_alpha0 = _mm_add_epi32(_mm_slli_epi32(_mm_and_si128(v_Y2, v_itsi1), CC_INTER_BITS),
                                                         _mm_and_si128(v_X2, v_itsi1));
                                v_alpha1 = _mm_add_epi32(_mm_slli_epi32(_mm_and_si128(v_Y3, v_itsi1), CC_INTER_BITS),
                                                         _mm_and_si128(v_X3, v_itsi1));
                                _mm_storeu_si128((__m128i *)(alpha + x1 + 8), _mm_packs_epi32(v_alpha0, v_alpha1));

                                // convert to 16s
                                v_X0 = _mm_packs_epi32(_mm_srai_epi32(v_X0, CC_INTER_BITS), _mm_srai_epi32(v_X1, CC_INTER_BITS));
                                v_X1 = _mm_packs_epi32(_mm_srai_epi32(v_X2, CC_INTER_BITS), _mm_srai_epi32(v_X3, CC_INTER_BITS));
                                v_Y0 = _mm_packs_epi32(_mm_srai_epi32(v_Y0, CC_INTER_BITS), _mm_srai_epi32(v_Y1, CC_INTER_BITS));
                                v_Y1 = _mm_packs_epi32(_mm_srai_epi32(v_Y2, CC_INTER_BITS), _mm_srai_epi32(v_Y3, CC_INTER_BITS));

                                _mm_interleave_epi16(v_X0, v_X1, v_Y0, v_Y1);

                                _mm_storeu_si128((__m128i *)(xy + x1 * 2), v_X0);
                                _mm_storeu_si128((__m128i *)(xy + x1 * 2 + 8), v_X1);
                                _mm_storeu_si128((__m128i *)(xy + x1 * 2 + 16), v_Y0);
                                _mm_storeu_si128((__m128i *)(xy + x1 * 2 + 24), v_Y1);
                            }
                        }
                        #endif

                        for( ; x1 < bw; x1++ )
                        {
                            double W = W0 + M[6]*x1;
                            W = W ? CC_INTER_TAB_SIZE/W : 0;
                            double fX = MAX((double)INT_MIN, MIN((double)INT_MAX, (X0 + M[0]*x1)*W));
                            double fY = MAX((double)INT_MIN, MIN((double)INT_MAX, (Y0 + M[3]*x1)*W));
                            int X, Y;
                            X = double_int(fX);
                            Y = double_int(fY);

                            xy[x1*2] = int_short(X >> CC_INTER_BITS);
                            xy[x1*2+1] = int_short(Y >> CC_INTER_BITS);
                            alpha[x1] = (short)((Y & (CC_INTER_TAB_SIZE-1))*CC_INTER_TAB_SIZE +
                                                (X & (CC_INTER_TAB_SIZE-1)));
                        }
                    }
                }

                if( interpolation == CC_INTER_NEAREST )
                    remap( src, dpart, _XY, NULL, interpolation, borderType, borderValue );
                else
                {
                    img_t _matA = cvMat1(bh, bw, CC_16U, 1, A);
                    remap( src, dpart, _XY, &_matA, interpolation, borderType, borderValue );
                }
            }
        }
    }

private:
    const img_t* src;
    img_t* dst;
    const double* M;
    int interpolation, borderType;
    CScalar borderValue;
};

#if defined (HAVE_IPP) && IPP_VERSION_X100 >= 810 && IPP_DISABLE_BLOCK
class IPPWarpPerspectiveInvoker :
    public CParallelLoopBody
{
public:
    IPPWarpPerspectiveInvoker(img_t* src, img_t* dst, double (&_coeffs)[3][3], int &_interpolation,
                              int &_borderType, const CScalar &_borderValue, ippiWarpPerspectiveFunc _func, bool *_ok) :
        CParallelLoopBody(), src(src), dst(dst), mode(_interpolation), coeffs(_coeffs),
        borderType(_borderType), borderValue(_borderValue), func(_func), ok(_ok)
    {
        *ok = true;
    }

    virtual void operator() (const CRange& range) const
    {
        IppiSize srcsize = {src->cols, src->rows};
        IppiRect srcroi = {0, 0, src->cols, src->rows};
        IppiRect dstroi = {0, range.start, dst->cols, range.end - range.start};
        int cnn = CC_MAT_CN(src);

        if( borderType == CC_BORDER_CONSTANT )
        {
            IppiSize setSize = {dst->cols, range.end - range.start};
            void *dataPointer = dst.ptr(range.start);
            if( !IPPSet( borderValue, dataPointer, (int)dst->step[0], setSize, cnn, src CC_MAT_DEPTH() ) )
            {
                *ok = false;
                return;
            }
        }

        IppStatus status = CC_INSTRUMENT_FUN_PTR_CALL_IPP(func,(src->tt.data, srcsize, (int)src->step[0], srcroi, dst->tt.data, (int)dst->step[0], dstroi, coeffs, mode));
        if (status != ippStsNoErr)
            *ok = false;
        else
        {
            CC_IMPL_ADD(CC_IMPL_IPP|CC_IMPL_MT);
        }
    }
private:
    img_t* src;
    img_t* dst;
    int mode;
    double (&coeffs)[3][3];
    int borderType;
    const CScalar borderValue;
    ippiWarpPerspectiveFunc func;
    bool *ok;

    const IPPWarpPerspectiveInvoker& operator= (const IPPWarpPerspectiveInvoker&);
};
#endif

static void hal_warpPerspectve(TypeId src_type, int cn,
                    const uchar * src_data, size_t src_step, int src_width, int src_height,
                    uchar * dst_data, size_t dst_step, int dst_width, int dst_height,
                    const double M[9], int interpolation, int borderType, const double borderValue[4])
{
    CALL_HAL(warpPerspective, cv_hal_warpPerspective, (src_type, cn, src_data, src_step, src_width, src_height, dst_data, dst_step, dst_width, dst_height, M, interpolation, borderType, borderValue));
    img_t src[1] = {0}, dst[1] = {0};
    int dst_total = dst_height*dst_width;
    cvMat(src, src_height, src_width, src_type, cn, src_data, src_step);
    cvMat(dst, dst_height, dst_width, src_type, cn, dst_data, dst_step);

    CRange range = {0, dst->rows};
    WarpPerspectiveInvoker invoker(src, dst, M, interpolation, borderType, cScalar(borderValue[0], borderValue[1], borderValue[2], borderValue[3]));
    parallel_for_(range, invoker, dst_total/(double)(1<<16));
}

static void warpPerspective( const img_t* src, img_t* dst, const img_t* M0,
                          CSize dsize, int flags, enum CBorderTypes borderType, const CScalar& borderValue )
{
    IM_BEGIN(im, 10);
    CC_INSTRUMENT_REGION()

    CC_Assert( CC_MAT_TOTAL(src) > 0 );

    CC_OCL_RUN(src.dims() <= 2 && dst.isUMat() &&
               src->cols() <= SHRT_MAX && src->rows() <= SHRT_MAX,
               ocl_warpTransform_cols4(src, dst, M0, dsize, flags, borderType, borderValue,
                                       OCL_OP_PERSPECTIVE))

    CC_OCL_RUN(src.dims() <= 2 && dst.isUMat(),
               ocl_warpTransform(src, dst, M0, dsize, flags, borderType, borderValue,
                              OCL_OP_PERSPECTIVE));

    dsize = CC_SIZE_AREA(dsize) == 0 ? cvGetSize(src) : dsize;
    cvSetMat( dst, dsize.h, dsize.w, src->tid, CC_MAT_CN(src) );

    if( dst->tt.data == src->tt.data )
        src = cvCloneMat(im++, src);

    double M[9];
    img_t matM = cvMat1(3, 3, CC_64F, 1, M);
    int interpolation = flags & CC_INTER_MAX;
    if( interpolation == CC_INTER_AREA )
        interpolation = CC_INTER_LINEAR;

    CC_Assert( (M0->tid == CC_32F || M0->tid == CC_64F) && M0->rows == 3 && M0->cols == 3 );
    cvConvertTo(M0, &matM, matM.tid);

#if defined (HAVE_IPP) && IPP_VERSION_X100 >= 810 && IPP_DISABLE_BLOCK
    CC_IPP_CHECK()
    {
        int type = src->tid, depth = CC_MAT_DEPTH(type), cn = CC_MAT_CN(type);
        if( (depth == CC_8U || depth == CC_16U || depth == CC_32F) &&
           (cn == 1 || cn == 3 || cn == 4) &&
           ( borderType == CC_BORDER_TRANSPARENT || borderType == CC_BORDER_CONSTANT ) &&
           (interpolation == CC_INTER_NEAREST || interpolation == CC_INTER_LINEAR || interpolation == CC_INTER_CUBIC))
        {
            ippiWarpPerspectiveFunc ippFunc = 0;
            if ((flags & CC_WARP_INVERSE_MAP) != 0)
            {
                ippFunc = type == CC_8UC1 ? (ippiWarpPerspectiveFunc)ippiWarpPerspectiveBack_8u_C1R :
                type == CC_8UC3 ? (ippiWarpPerspectiveFunc)ippiWarpPerspectiveBack_8u_C3R :
                type == CC_8UC4 ? (ippiWarpPerspectiveFunc)ippiWarpPerspectiveBack_8u_C4R :
                type == CC_16UC1 ? (ippiWarpPerspectiveFunc)ippiWarpPerspectiveBack_16u_C1R :
                type == CC_16UC3 ? (ippiWarpPerspectiveFunc)ippiWarpPerspectiveBack_16u_C3R :
                type == CC_16UC4 ? (ippiWarpPerspectiveFunc)ippiWarpPerspectiveBack_16u_C4R :
                type == CC_32FC1 ? (ippiWarpPerspectiveFunc)ippiWarpPerspectiveBack_32f_C1R :
                type == CC_32FC3 ? (ippiWarpPerspectiveFunc)ippiWarpPerspectiveBack_32f_C3R :
                type == CC_32FC4 ? (ippiWarpPerspectiveFunc)ippiWarpPerspectiveBack_32f_C4R : 0;
            }
            else
            {
                ippFunc = type == CC_8UC1 ? (ippiWarpPerspectiveFunc)ippiWarpPerspective_8u_C1R :
                type == CC_8UC3 ? (ippiWarpPerspectiveFunc)ippiWarpPerspective_8u_C3R :
                type == CC_8UC4 ? (ippiWarpPerspectiveFunc)ippiWarpPerspective_8u_C4R :
                type == CC_16UC1 ? (ippiWarpPerspectiveFunc)ippiWarpPerspective_16u_C1R :
                type == CC_16UC3 ? (ippiWarpPerspectiveFunc)ippiWarpPerspective_16u_C3R :
                type == CC_16UC4 ? (ippiWarpPerspectiveFunc)ippiWarpPerspective_16u_C4R :
                type == CC_32FC1 ? (ippiWarpPerspectiveFunc)ippiWarpPerspective_32f_C1R :
                type == CC_32FC3 ? (ippiWarpPerspectiveFunc)ippiWarpPerspective_32f_C3R :
                type == CC_32FC4 ? (ippiWarpPerspectiveFunc)ippiWarpPerspective_32f_C4R : 0;
            }
            int mode =
            interpolation == CC_INTER_NEAREST ? IPPI_INTER_NN :
            interpolation == CC_INTER_LINEAR ? IPPI_INTER_LINEAR :
            interpolation == CC_INTER_CUBIC ? IPPI_INTER_CUBIC : 0;
            CC_Assert(mode && ippFunc);

            double coeffs[3][3];
            for( int i = 0; i < 3; i++ )
                for( int j = 0; j < 3; j++ )
                    coeffs[i][j] = matM.at<double>(i, j);

            bool ok;
            CRange range(0, dst->rows);
            IPPWarpPerspectiveInvoker invoker(src, dst, coeffs, mode, borderType, borderValue, ippFunc, &ok);
            parallel_for_(range, invoker, CC_MAT_TOTAL(dst)/(double)(1<<16));
            if( ok )
            {
                CC_IMPL_ADD(CC_IMPL_IPP|CC_IMPL_MT);
                return;
            }
            setIppErrorStatus();
        }
    }
#endif

    if( !(flags & CC_WARP_INVERSE_MAP) )
        invert(&matM, &matM);

    hal_warpPerspectve(src->tid, CC_MAT_CN(src), src->tt.data, src->step, src->cols, src->rows, dst->tt.data, dst->step, dst->cols, dst->rows,
                        matM.tt.db, interpolation, borderType, borderValue.val);
    IM_END(im);
}


static void getRotationMatrix2D( double* m23, CPoint2f center, double angle, double scale )
{
    angle *= CC_PI/180;
    double alpha = cos(angle)*scale;
    double beta = sin(angle)*scale;

    double* m = m23;

    m[0] = alpha;
    m[1] = beta;
    m[2] = (1-alpha)*center.x - beta*center.y;
    m[3] = -beta;
    m[4] = alpha;
    m[5] = beta*center.x + (1-alpha)*center.y;
}

/* Calculates coefficients of perspective transformation
 * which maps (xi,yi) to (ui,vi), (i=1,2,3,4):
 *
 *      c00*xi + c01*yi + c02
 * ui = ---------------------
 *      c20*xi + c21*yi + c22
 *
 *      c10*xi + c11*yi + c12
 * vi = ---------------------
 *      c20*xi + c21*yi + c22
 *
 * Coefficients are calculated by solving linear system:
 * / x0 y0  1  0  0  0 -x0*u0 -y0*u0 \ /c00\ /u0\
 * | x1 y1  1  0  0  0 -x1*u1 -y1*u1 | |c01| |u1|
 * | x2 y2  1  0  0  0 -x2*u2 -y2*u2 | |c02| |u2|
 * | x3 y3  1  0  0  0 -x3*u3 -y3*u3 |.|c10|=|u3|,
 * |  0  0  0 x0 y0  1 -x0*v0 -y0*v0 | |c11| |v0|
 * |  0  0  0 x1 y1  1 -x1*v1 -y1*v1 | |c12| |v1|
 * |  0  0  0 x2 y2  1 -x2*v2 -y2*v2 | |c20| |v2|
 * \  0  0  0 x3 y3  1 -x3*v3 -y3*v3 / \c21/ \v3/
 *
 * where:
 *   cij - matrix coefficients, c22 = 1
 */
static void getPerspectiveTransform( double9 M, const CPoint2f src[], const CPoint2f dst[] )
{
    CC_INSTRUMENT_REGION()

    img_t X = cvMat1(8, 1, CC_64F, 1, M);
    double a[8][8], b[8];
    img_t A = cvMat1(8, 8, CC_64F, 1, a[0]), B = cvMat1(8, 1, CC_64F, 1, b);

    for( int i = 0; i < 4; ++i )
    {
        a[i][0] = a[i+4][3] = src[i].x;
        a[i][1] = a[i+4][4] = src[i].y;
        a[i][2] = a[i+4][5] = 1;
        a[i][3] = a[i][4] = a[i][5] =
        a[i+4][0] = a[i+4][1] = a[i+4][2] = 0;
        a[i][6] = -src[i].x*dst[i].x;
        a[i][7] = -src[i].y*dst[i].x;
        a[i+4][6] = -src[i].x*dst[i].y;
        a[i+4][7] = -src[i].y*dst[i].y;
        b[i] = dst[i].x;
        b[i+4] = dst[i].y;
    }

    solve( &A, &B, &X, CC_DECOMP_SVD );
    M[8] = 1.;
}

/* Calculates coefficients of affine transformation
 * which maps (xi,yi) to (ui,vi), (i=1,2,3):
 *
 * ui = c00*xi + c01*yi + c02
 *
 * vi = c10*xi + c11*yi + c12
 *
 * Coefficients are calculated by solving linear system:
 * / x0 y0  1  0  0  0 \ /c00\ /u0\
 * | x1 y1  1  0  0  0 | |c01| |u1|
 * | x2 y2  1  0  0  0 | |c02| |u2|
 * |  0  0  0 x0 y0  1 | |c10| |v0|
 * |  0  0  0 x1 y1  1 | |c11| |v1|
 * \  0  0  0 x2 y2  1 / |c12| |v2|
 *
 * where:
 *   cij - matrix coefficients
 */

static void getAffineTransform( double6 m23, const CPoint2f src[], const CPoint2f dst[] )
{
    double* M = m23;
    img_t X = cvMat1(6, 1, CC_64F, 1, M);
    double a[6*6], b[6];
    img_t A = cvMat1(6, 6, CC_64F, 1, a), B = cvMat1(6, 1, CC_64F, 1, b);

    for( int i = 0; i < 3; i++ )
    {
        int j = i*12;
        int k = i*12+6;
        a[j] = a[k+3] = src[i].x;
        a[j+1] = a[k+4] = src[i].y;
        a[j+2] = a[k+5] = 1;
        a[j+3] = a[j+4] = a[j+5] = 0;
        a[k] = a[k+1] = a[k+2] = 0;
        b[i*2] = dst[i].x;
        b[i*2+1] = dst[i].y;
    }

    solve( &A, &B, &X );
}

static void invertAffineTransform(const void* M23, void* iM23, TypeId type)
{
    enum {step = 3, istep = 3};
    //CC_Assert(matM->rows == 2 && matM->cols == 3);

    if( type == CC_32F )
    {
        const float* M = (const float*)(M23);
        float* iM = (float*)(iM23);
        //int step = (int)(matM_step/sizeof(M[0])), istep = (int)(_iM->step/sizeof(iM[0]));

        double D = M[0]*M[step+1] - M[1]*M[step];
        D = D != 0 ? 1./D : 0;
        double A11 = M[step+1]*D, A22 = M[0]*D, A12 = -M[1]*D, A21 = -M[step]*D;
        double b1 = -A11*M[2] - A12*M[step+2];
        double b2 = -A21*M[2] - A22*M[step+2];

        iM[0] = (float)A11; iM[1] = (float)A12; iM[2] = (float)b1;
        iM[istep] = (float)A21; iM[istep+1] = (float)A22; iM[istep+2] = (float)b2;
    }
    else if( type == CC_64F )
    {
        const double* M = (const double*)(M23);
        double* iM = (double*)(iM23);
        //int step = (int)(matM->step/sizeof(M[0])), istep = (int)(_iM->step/sizeof(iM[0]));

        double D = M[0]*M[step+1] - M[1]*M[step];
        D = D != 0 ? 1./D : 0;
        double A11 = M[step+1]*D, A22 = M[0]*D, A12 = -M[1]*D, A21 = -M[step]*D;
        double b1 = -A11*M[2] - A12*M[step+2];
        double b2 = -A21*M[2] - A22*M[step+2];

        iM[0] = A11; iM[1] = A12; iM[2] = b1;
        iM[istep] = A21; iM[istep+1] = A22; iM[istep+2] = b2;
    }
    else
        CC_Error( CC_StsUnsupportedFormat, "" );
}

static void getPerspectiveTransform(double9 M, const img_t* src, const img_t* dst)
{
    CC_Assert(cvCheckVector(src, 2, CC_32F) == 4 && cvCheckVector(dst, 2, CC_32F) == 4);
    getPerspectiveTransform(M, (const CPoint2f*)src->tt.data, (const CPoint2f*)dst->tt.data);
}

static void getAffineTransform(double6 m23, const img_t* src, const img_t* dst)
{
    CC_Assert(cvCheckVector(src, 2, CC_32F) == 3 && cvCheckVector(dst, 2, CC_32F) == 3);
    getAffineTransform(m23, (const CPoint2f*)src->tt.data, (const CPoint2f*)dst->tt.data);
}

CC_IMPL void
cvResize1( const img_t* src, img_t* dst, int method )
{
    CC_Assert( src->tid == dst->tid );
    resize( src, dst, cvGetSize(dst), (double)dst->cols/src->cols,
        (double)dst->rows/src->rows, method );
}


CC_IMPL void
cvWarpAffine1( const img_t* src, img_t* dst, const img_t* marr,
              int flags, CScalar fillval )
{
    CC_Assert( src->tid == dst->tid );
    warpAffine( src, dst, marr, cvGetSize(dst), flags,
        (flags & CC_WARP_FILL_OUTLIERS) ? CC_BORDER_CONSTANT : CC_BORDER_TRANSPARENT,
        fillval );
}

CC_IMPL void
cvWarpPerspective1( const img_t* src, img_t* dst, const img_t* marr,
                   int flags, CScalar fillval )
{
    CC_Assert( src->tid == dst->tid );
    warpPerspective( src, dst, marr, cvGetSize(dst), flags,
        (flags & CC_WARP_FILL_OUTLIERS) ? CC_BORDER_CONSTANT : CC_BORDER_TRANSPARENT,
        fillval );
}

CC_IMPL void
cvRemap1( const img_t* src, img_t* dst,
         const img_t* mapx, const img_t* mapy,
         int flags, CScalar fillval )
{
    CC_Assert( CC_ARE_TYPES_EQ(src, dst) && CC_ARE_SIZES_EQ(dst, mapx) );
    remap( src, dst, mapx, mapy, flags & CC_INTER_MAX,
        (flags & CC_WARP_FILL_OUTLIERS) ? CC_BORDER_CONSTANT : CC_BORDER_TRANSPARENT,
        fillval );
}


CC_IMPL img_t*
cv2DRotationMatrix1( CPoint2D32f center, double angle,
                    double scale, img_t* M0 )
{
    double M[6];
    getRotationMatrix2D(M, center, angle, scale);
    cvSetMat(M0, 2, 3, CC_64F, 1, M);
    return M0;
}


CC_IMPL img_t*
cvGetPerspectiveTransform1( const CPoint2D32f* src, const CPoint2D32f* dst, img_t* M0 )
{
    double9 M;
    getPerspectiveTransform(M, (const CPoint2f*)src, (const CPoint2f*)dst);
    cvSetMat(M0, 3, 3, CC_64F, 1, M);
    return M0;
}


CC_IMPL img_t*
cvGetAffineTransform1( const CPoint2D32f* src, const CPoint2D32f* dst, img_t* M0 )
{
    double6 M23;
    getAffineTransform(M23, (const CPoint2f*)src, (const CPoint2f*)dst);
    cvSetMat(M0, 3, 3, CC_64F, 1, M23);
    return M0;
}


CC_IMPL void
cvConvertMaps( const img_t* map1, const img_t* map2, img_t* dstmap1, img_t* dstmap2 )
{
    if( dstmap2 )
    {
        if( dstmap2->tid == CC_16S && CC_MAT_CN(dstmap2)==1 ) {
            //dstmap2 = img_t(dstmap2.size(), CC_16UC1, dstmap2->tt.data, dstmap2->step);
            dstmap2->tid = CC_16U;
        }
    }

    convertMaps( map1, map2, dstmap1, dstmap2, CC_MAT_TYPECN(dstmap1), false );
}

/****************************************************************************************\
*                                   Log-Polar Transform                                  *
\****************************************************************************************/

#if 0
/* now it is done via Remap; more correct implementation should use
   some super-sampling technique outside of the "fovea" circle */
CC_IMPL void
cvLogPolar1( const img_t* src, img_t* dst,
            CPoint2D32f center, double M, int flags )
{
    img_t src_with_border; // don't scope this variable (it holds image data)

    Ptr<img_t> mapx, mapy;

    img_t srcstub, *src = cvGetMat(srcarr, &srcstub);
    img_t dststub, *dst = cvGetMat(dstarr, &dststub);
    CSize dsize;

    if( !CC_ARE_TYPES_EQ( src, dst ))
        CC_Error( CC_StsUnmatchedFormats, "" );

    if( M <= 0 )
        CC_Error( CC_StsOutOfRange, "M should be >0" );

    dsize = cvGetMatSize(dst);

    mapx.reset(cvCreateMat( dsize.height, dsize.width, CC_32F ));
    mapy.reset(cvCreateMat( dsize.height, dsize.width, CC_32F ));

    if( !(flags & CC_WARP_INVERSE_MAP) )
    {
        int phi, rho;
        FREE(); double* = MALLOC(double, ) _exp_tab(dsize.width);
        double* exp_tab = _exp_tab;

        for( rho = 0; rho < dst->width; rho++ )
            exp_tab[rho] = exp(rho/M) - 1.0;

        for( phi = 0; phi < dsize.height; phi++ )
        {
            double cp = cos(phi*2*CC_PI/dsize.height);
            double sp = sin(phi*2*CC_PI/dsize.height);
            float* mx = (float*)(mapx->data.ptr + phi*mapx->step);
            float* my = (float*)(mapy->data.ptr + phi*mapy->step);

            for( rho = 0; rho < dsize.width; rho++ )
            {
                double r = exp_tab[rho];
                double x = r*cp + center.x;
                double y = r*sp + center.y;

                mx[rho] = (float)x;
                my[rho] = (float)y;
            }
        }
    }
    else
    {
        const int ANGLE_BORDER = 1;
        img_t src_ = cvarrToMat(src);
        copyMakeBorder(src_, src_with_border, ANGLE_BORDER, ANGLE_BORDER, 0, 0, CC_BORDER_WRAP);
        srcstub = src_with_border; src = &srcstub;
        CSize ssize = cvGetMatSize(src);
        ssize.height -= 2*ANGLE_BORDER;

        int x, y;
        img_t bufx, bufy, bufp, bufa;
        double ascale = ssize.height/(2*CC_PI);
        FREE(); float* = MALLOC(float, ) _buf(4*dsize.width);
        float* buf = _buf;

        bufx = cvMat( 1, dsize.width, CC_32F, buf );
        bufy = cvMat( 1, dsize.width, CC_32F, buf + dsize.width );
        bufp = cvMat( 1, dsize.width, CC_32F, buf + dsize.width*2 );
        bufa = cvMat( 1, dsize.width, CC_32F, buf + dsize.width*3 );

        for( x = 0; x < dsize.width; x++ )
            bufx.data.fl[x] = (float)x - center.x;

        for( y = 0; y < dsize.height; y++ )
        {
            float* mx = (float*)(mapx->data.ptr + y*mapx->step);
            float* my = (float*)(mapy->data.ptr + y*mapy->step);

            for( x = 0; x < dsize.width; x++ )
                bufy.data.fl[x] = (float)y - center.y;

#if 1
            cvCartToPolar( &bufx, &bufy, &bufp, &bufa );

            for( x = 0; x < dsize.width; x++ )
                bufp.data.fl[x] += 1.f;

            cvLog( &bufp, &bufp );

            for( x = 0; x < dsize.width; x++ )
            {
                double rho = bufp.data.fl[x]*M;
                double phi = bufa.data.fl[x]*ascale;

                mx[x] = (float)rho;
                my[x] = (float)phi + ANGLE_BORDER;
            }
#else
            for( x = 0; x < dsize.width; x++ )
            {
                double xx = bufx.data.fl[x];
                double yy = bufy.data.fl[x];

                double p = log(sqrt(xx*xx + yy*yy) + 1.)*M;
                double a = atan2(yy,xx);
                if( a < 0 )
                    a = 2*CC_PI + a;
                a *= ascale;

                mx[x] = (float)p;
                my[x] = (float)a;
            }
#endif
        }
    }

    cvRemap( src, dst, mapx, mapy, flags, cScalarAll(0) );
}
#endif

static void logPolar( const img_t* src, img_t* dst,
                   CPoint2f center, double M, int flags )
{
    IM_BEGIN(im, 10);
    CC_OCL_RUN(src.isUMat() && dst.isUMat(),
        ocl_logPolar(src, dst, center, M, flags));
    img_t *src_with_border; // don't scope this variable (it holds image data)

    img_t *mapx, *mapy;

    CSize dsize = cvGetSize(src);
    cvSetMat(dst, dsize.h, dsize.w, src->tid);

    if (M <= 0)
        CC_Error(CC_StsOutOfRange, "M should be >0");


    mapx = cvSetMat(im++, dsize.h, dsize.w, CC_32F);
    mapy = cvSetMat(im++, dsize.h, dsize.w, CC_32F);

    if (!(flags & CC_WARP_INVERSE_MAP))
    {
        int phi, rho;
        double* _exp_tab = MALLOC(double, dsize.width);
        double* exp_tab = _exp_tab;

        for (rho = 0; rho < dsize.width; rho++)
            exp_tab[rho] = exp(rho / M) - 1.0;

        for (phi = 0; phi < dsize.height; phi++)
        {
            double cp = cos(phi * 2 * CC_PI / dsize.height);
            double sp = sin(phi * 2 * CC_PI / dsize.height);
            float* mx = (float*)(mapx->data + phi*mapx->step);
            float* my = (float*)(mapy->data + phi*mapy->step);

            for (rho = 0; rho < dsize.width; rho++)
            {
                double r = exp_tab[rho];
                double x = r*cp + center.x;
                double y = r*sp + center.y;

                mx[rho] = (float)x;
                my[rho] = (float)y;
            }
        }
        FREE(_exp_tab);
    }
    else
    {
        const int ANGLE_BORDER = 1;
        src_with_border = cvCopyMakeBorder(src, im++, ANGLE_BORDER, ANGLE_BORDER, 0, 0, CC_BORDER_WRAP);
        src = src_with_border;
        CSize ssize = cvGetSize(src);
        ssize.height -= 2 * ANGLE_BORDER;

        int x, y;
        img_t *bufx, *bufy, *bufp, *bufa;
        double ascale = ssize.height / (2 * CC_PI);

        bufx = cvSetMat(im++, 1, dsize.width, CC_32F);
        bufy = cvSetMat(im++, 1, dsize.width, CC_32F);
        bufp = cvSetMat(im++, 1, dsize.width, CC_32F);
        bufa = cvSetMat(im++, 1, dsize.width, CC_32F);

        for (x = 0; x < dsize.width; x++)
            bufx->tt.fl[x] = (float)x - center.x;

        for (y = 0; y < dsize.height; y++)
        {
            float* mx = (float*)(mapx->data + y*mapx->step);
            float* my = (float*)(mapy->data + y*mapy->step);

            for (x = 0; x < dsize.width; x++)
                bufy->tt.fl[x] = (float)y - center.y;

#if 1
            cvCartToPolar(bufx, bufy, bufp, bufa);

            for (x = 0; x < dsize.width; x++)
                bufp->tt.fl[x] += 1.f;

            cvLog(bufp, bufp);

            for (x = 0; x < dsize.width; x++)
            {
                double rho = bufp->tt.fl[x] * M;
                double phi = bufa->tt.fl[x] * ascale;

                mx[x] = (float)rho;
                my[x] = (float)phi + ANGLE_BORDER;
            }
#else
            for (x = 0; x < dsize.width; x++)
            {
                double xx = bufx->tt.fl[x];
                double yy = bufy->tt.fl[x];
                double p = log(sqrt(xx*xx + yy*yy) + 1.)*M;
                double a = atan2(yy, xx);
                if (a < 0)
                    a = 2 * CC_PI + a;
                a *= ascale;
                mx[x] = (float)p;
                my[x] = (float)a;
            }
#endif
        }
    }

    remap(src, dst, mapx, mapy, flags & CC_INTER_MAX,
        (flags & CC_WARP_FILL_OUTLIERS) ? CC_BORDER_CONSTANT : CC_BORDER_TRANSPARENT);
    IM_END(im);
}

/****************************************************************************************
                                   Linear-Polar Transform
  J.L. Blanco, Apr 2009
 ****************************************************************************************/
CC_IMPL
void cvLinearPolar( const img_t* src, img_t* dst,
            CPoint2D32f center, double maxRadius, int flags )
{
    IM_BEGIN(im, 10);
    img_t *src_with_border; // don't scope this variable (it holds image data)
    img_t *mapx, *mapy;
    CSize dsize;

    if( !CC_ARE_TYPES_EQ( src, dst ))
        CC_Error( CC_StsUnmatchedFormats, "" );

    dsize = cvGetMatSize(dst);

    mapx = cvSetMat( im++, dsize.height, dsize.width, CC_32F );
    mapy = cvSetMat( im++, dsize.height, dsize.width, CC_32F );

    if( !(flags & CC_WARP_INVERSE_MAP) )
    {
        int phi, rho;

        for( phi = 0; phi < dsize.height; phi++ )
        {
            double cp = cos(phi*2*CC_PI/dsize.height);
            double sp = sin(phi*2*CC_PI/dsize.height);
            float* mx = (float*)(mapx->tt.data + phi*mapx->step);
            float* my = (float*)(mapy->tt.data + phi*mapy->step);

            for( rho = 0; rho < dsize.width; rho++ )
            {
                double r = maxRadius*rho/dsize.width;
                double x = r*cp + center.x;
                double y = r*sp + center.y;

                mx[rho] = (float)x;
                my[rho] = (float)y;
            }
        }
    }
    else
    {
        const int ANGLE_BORDER = 1;
        src_with_border = cvCopyMakeBorder(src, im++, ANGLE_BORDER, ANGLE_BORDER, 0, 0, CC_BORDER_WRAP);
        src = src_with_border;
        CSize ssize = cvGetMatSize(src);
        ssize.height -= 2*ANGLE_BORDER;

        int x, y;
        img_t bufx, bufy, bufp, bufa;
        const double ascale = ssize.height/(2*CC_PI);
        const double pscale = ssize.width/maxRadius;

        float* _buf = MALLOC(float, 4*dsize.width);
        float* buf = _buf;

        bufx = cvMat1( 1, dsize.width, CC_32F, 1, buf );
        bufy = cvMat1( 1, dsize.width, CC_32F, 1, buf + dsize.width );
        bufp = cvMat1( 1, dsize.width, CC_32F, 1, buf + dsize.width*2 );
        bufa = cvMat1( 1, dsize.width, CC_32F, 1, buf + dsize.width*3 );

        for( x = 0; x < dsize.width; x++ )
            bufx.tt.fl[x] = (float)x - center.x;

        for( y = 0; y < dsize.height; y++ )
        {
            float* mx = (float*)(mapx->tt.data + y*mapx->step);
            float* my = (float*)(mapy->tt.data + y*mapy->step);

            for( x = 0; x < dsize.width; x++ )
                bufy.tt.fl[x] = (float)y - center.y;

            cvCartToPolar( &bufx, &bufy, &bufp, &bufa, 0 );

            for( x = 0; x < dsize.width; x++ )
            {
                double rho = bufp.tt.fl[x]*pscale;
                double phi = bufa.tt.fl[x]*ascale;
                mx[x] = (float)rho;
                my[x] = (float)phi + ANGLE_BORDER;
            }
        }
        FREE(_buf);
    }

    cvRemap( src, dst, mapx, mapy, flags, cScalarAll(0) );
    IM_END(im);
}

static void linearPolar( const img_t* src, img_t* dst, CPoint2f center, double maxRadius, int flags )
{
    IM_BEGIN(im, 10);
    CC_OCL_RUN(src.isUMat() && dst.isUMat(),
        ocl_linearPolar(src, dst, center, maxRadius, flags));
    img_t *src_with_border; // don't scope this variable (it holds image data)
    img_t *mapx, *mapy;

    CSize dsize = cvGetSize(src);
    cvSetMat(dst, dsize.h, dsize.w, src->tid);

    mapx = cvSetMat(im++, dsize.h, dsize.w, CC_32F);
    mapy = cvSetMat(im++, dsize.h, dsize.w, CC_32F);

    if (!(flags & CC_WARP_INVERSE_MAP))
    {
        int phi, rho;

        for (phi = 0; phi < dsize.height; phi++)
        {
            double cp = cos(phi * 2 * CC_PI / dsize.height);
            double sp = sin(phi * 2 * CC_PI / dsize.height);
            float* mx = (float*)(mapx->data + phi*mapx->step);
            float* my = (float*)(mapy->data + phi*mapy->step);

            for (rho = 0; rho < dsize.width; rho++)
            {
                double r = maxRadius*rho / dsize.width;
                double x = r*cp + center.x;
                double y = r*sp + center.y;

                mx[rho] = (float)x;
                my[rho] = (float)y;
            }
        }
    }
    else
    {
        const int ANGLE_BORDER = 1;
        src_with_border = cvCopyMakeBorder(src, im++, ANGLE_BORDER, ANGLE_BORDER, 0, 0, CC_BORDER_WRAP);
        src = src_with_border;
        CSize ssize = cvGetSize(src);
        ssize.height -= 2 * ANGLE_BORDER;

        int x, y;
        img_t *bufx, *bufy, *bufp, *bufa;
        const double ascale = ssize.height / (2 * CC_PI);
        const double pscale = ssize.width / maxRadius;

        bufx = cvSetMat(im++, 1, dsize.width, CC_32F);
        bufy = cvSetMat(im++, 1, dsize.width, CC_32F);
        bufp = cvSetMat(im++, 1, dsize.width, CC_32F);
        bufa = cvSetMat(im++, 1, dsize.width, CC_32F);

        for (x = 0; x < dsize.width; x++)
            bufx->tt.fl[x] = (float)x - center.x;

        for (y = 0; y < dsize.height; y++)
        {
            float* mx = (float*)(mapx->data + y*mapx->step);
            float* my = (float*)(mapy->data + y*mapy->step);

            for (x = 0; x < dsize.width; x++)
                bufy->tt.fl[x] = (float)y - center.y;

            cvCartToPolar(bufx, bufy, bufp, bufa, 0);

            for (x = 0; x < dsize.width; x++)
            {
                double rho = bufp->tt.fl[x] * pscale;
                double phi = bufa->tt.fl[x] * ascale;
                mx[x] = (float)rho;
                my[x] = (float)phi + ANGLE_BORDER;
            }
        }
    }

    remap(src, dst, mapx, mapy, flags & CC_INTER_MAX, (flags & CC_WARP_FILL_OUTLIERS) ? CC_BORDER_CONSTANT : CC_BORDER_TRANSPARENT);
    IM_END(im);
}

#if 0

#endif