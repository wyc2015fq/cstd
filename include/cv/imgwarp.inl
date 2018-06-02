
/* ////////////////////////////////////////////////////////////////////
//
//  Geometrical transforms on images and matrices: rotation, zoom etc.
//
// */


#define CC_MORPH_ALIGN  4

#define CC_WHOLE   0
#define CC_START   1
#define CC_END     2
#define CC_MIDDLE  4

#define ICC_WARP_SHIFT          10
#define ICC_WARP_MASK           ((1 << ICC_WARP_SHIFT) - 1)

#define ICC_LINEAR_TAB_SIZE     (ICC_WARP_MASK+1)
#define ICC_CUBIC_TAB_SIZE   (ICC_WARP_MASK+1)

/************** interpolation constants and tables ***************/

#define ICC_WARP_MUL_ONE_8U(x)  ((x) << ICC_WARP_SHIFT)
#define ICC_WARP_DESCALE_8U(x)  CC_DESCALE((x), ICC_WARP_SHIFT*2)
#define ICC_WARP_CLIP_X(x)      ((unsigned)(x) < (unsigned)ssize.width ? \
                                (x) : (x) < 0 ? 0 : ssize.width - 1)
#define ICC_WARP_CLIP_Y(y)      ((unsigned)(y) < (unsigned)ssize.height ? \
                                (y) : (y) < 0 ? 0 : ssize.height - 1)

static float icvLinearCoeffs[(ICC_LINEAR_TAB_SIZE+1)*2];

static void icvInitLinearCoeffTab()
{
    static int inittab = 0;
    if(!inittab)
    {
        for(int i = 0; i <= ICC_LINEAR_TAB_SIZE; i++)
        {
            float x = (float)i/ICC_LINEAR_TAB_SIZE;
            icvLinearCoeffs[i*2] = x;
            icvLinearCoeffs[i*2+1] = 1.f - x;
        }

        inittab = 1;
    }
}


static float icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE+1)*2];

static void icvInitCubicCoeffTab()
{
    static int inittab = 0;
    if(!inittab)
    {
#if 0
        // classical Mitchell-Netravali filter
        const double B = 1./3;
        const double C = 1./3;
        const double p0 = (6 - 2*B)/6.;
        const double p2 = (-18 + 12*B + 6*C)/6.;
        const double p3 = (12 - 9*B - 6*C)/6.;
        const double q0 = (8*B + 24*C)/6.;
        const double q1 = (-12*B - 48*C)/6.;
        const double q2 = (6*B + 30*C)/6.;
        const double q3 = (-B - 6*C)/6.;

        #define ICC_CUBIC_1(x)  (((x)*p3 + p2)*(x)*(x) + p0)
        #define ICC_CUBIC_2(x)  ((((x)*q3 + q2)*(x) + q1)*(x) + q0)
#else
        // alternative "sharp" filter
        const double A = -0.75;
        #define ICC_CUBIC_1(x)  (((A + 2)*(x) - (A + 3))*(x)*(x) + 1)
        #define ICC_CUBIC_2(x)  (((A*(x) - 5*A)*(x) + 8*A)*(x) - 4*A)
#endif
        for(int i = 0; i <= ICC_CUBIC_TAB_SIZE; i++)
        {
            float x = (float)i/ICC_CUBIC_TAB_SIZE;
            icvCubicCoeffs[i*2] = (float)ICC_CUBIC_1(x);
            x += 1.f;
            icvCubicCoeffs[i*2+1] = (float)ICC_CUBIC_2(x);
        }

        inittab = 1;
    }
}


/****************************************************************************************\
*                                         Resize                                         *
\****************************************************************************************/

static CStatus C_STDCALL
icvResize_NN_8u_C1R(const uchar* src, int srcstep, CSize ssize,
                     uchar* dst, int dststep, CSize dsize, int pix_size)
{
    int* x_ofs = (int*)cvStackAlloc(dsize.width * sizeof(x_ofs[0]));
    int pix_size4 = pix_size / sizeof(int);
    int x, y, t;

    for(x = 0; x < dsize.width; x++)
    {
        t = (ssize.width*x*2 + MIN(ssize.width, dsize.width) - 1)/(dsize.width*2);
        t -= t >= ssize.width;
        x_ofs[x] = t*pix_size;
    }

    for(y = 0; y < dsize.height; y++, dst += dststep)
    {
        const uchar* tsrc;
        t = (ssize.height*y*2 + MIN(ssize.height, dsize.height) - 1)/(dsize.height*2);
        t -= t >= ssize.height;
        tsrc = src + srcstep*t;

        switch(pix_size)
        {
        case 1:
            for(x = 0; x <= dsize.width - 2; x += 2)
            {
                uchar t0 = tsrc[x_ofs[x]];
                uchar t1 = tsrc[x_ofs[x+1]];

                dst[x] = t0;
                dst[x+1] = t1;
            }

            for(; x < dsize.width; x++)
                dst[x] = tsrc[x_ofs[x]];
            break;
        case 2:
            for(x = 0; x < dsize.width; x++)
                *(ushort*)(dst + x*2) = *(ushort*)(tsrc + x_ofs[x]);
            break;
        case 3:
            for(x = 0; x < dsize.width; x++)
            {
                const uchar* _tsrc = tsrc + x_ofs[x];
                dst[x*3] = _tsrc[0]; dst[x*3+1] = _tsrc[1]; dst[x*3+2] = _tsrc[2];
            }
            break;
        case 4:
            for(x = 0; x < dsize.width; x++)
                *(int*)(dst + x*4) = *(int*)(tsrc + x_ofs[x]);
            break;
        case 6:
            for(x = 0; x < dsize.width; x++)
            {
                const ushort* _tsrc = (const ushort*)(tsrc + x_ofs[x]);
                ushort* _tdst = (ushort*)(dst + x*6);
                _tdst[0] = _tsrc[0]; _tdst[1] = _tsrc[1]; _tdst[2] = _tsrc[2];
            }
            break;
        default:
            for(x = 0; x < dsize.width; x++)
                CC_MEMCPY_INT(dst + x*pix_size, tsrc + x_ofs[x], pix_size4);
        }
    }

    return CC_OK;
}


typedef struct CvResizeAlpha
{
    int idx;
    union
    {
        float alpha;
        int ialpha;
    };
}
CvResizeAlpha;


#define  ICC_DEF_RESIZE_BILINEAR_FUNC(flavor, arrtype, worktype, alpha_field,  \
                                       mul_one_macro, descale_macro)           \
static CStatus C_STDCALL                                                      \
icvResize_Bilinear_##flavor##_CnR(const arrtype* src, int srcstep, CSize ssize,\
                                   arrtype* dst, int dststep, CSize dsize,     \
                                   int cn, int xmax,                            \
                                   const CvResizeAlpha* xofs,                   \
                                   const CvResizeAlpha* yofs,                   \
                                   worktype* buf0, worktype* buf1)             \
{                                                                               \
    int prev_sy0 = -1, prev_sy1 = -1;                                           \
    int k, dx, dy;                                                              \
                                                                                \
    srcstep /= sizeof(src[0]);                                                  \
    dststep /= sizeof(dst[0]);                                                  \
    dsize.width *= cn;                                                          \
    xmax *= cn;                                                                 \
                                                                                \
    for(dy = 0; dy < dsize.height; dy++, dst += dststep)                      \
    {                                                                           \
        worktype fy = yofs[dy].alpha_field, *swap_t;                            \
        int sy0 = yofs[dy].idx, sy1 = sy0 + (fy > 0 && sy0 < ssize.height-1);   \
                                                                                \
        if(sy0 == prev_sy0 && sy1 == prev_sy1)                                \
            k = 2;                                                              \
        else if(sy0 == prev_sy1)                                              \
        {                                                                       \
            CC_SWAP(buf0, buf1, swap_t);                                      \
            k = 1;                                                              \
        }                                                                       \
        else                                                                    \
            k = 0;                                                              \
                                                                                \
        for(; k < 2; k++)                                                     \
        {                                                                       \
            worktype* _buf = k == 0 ? buf0 : buf1;                              \
            const arrtype* _src;                                                \
            int sy = k == 0 ? sy0 : sy1;                                        \
            if(k == 1 && sy1 == sy0)                                          \
            {                                                                   \
                memcpy(buf1, buf0, dsize.width*sizeof(buf0[0]));              \
                continue;                                                       \
            }                                                                   \
                                                                                \
            _src = src + sy*srcstep;                                            \
            for(dx = 0; dx < xmax; dx++)                                      \
            {                                                                   \
                int sx = xofs[dx].idx;                                          \
                worktype fx = xofs[dx].alpha_field;                             \
                worktype t = _src[sx];                                          \
                _buf[dx] = mul_one_macro(t) + fx*(_src[sx+cn] - t);             \
            }                                                                   \
                                                                                \
            for(; dx < dsize.width; dx++)                                     \
                _buf[dx] = mul_one_macro(_src[xofs[dx].idx]);                   \
        }                                                                       \
                                                                                \
        prev_sy0 = sy0;                                                         \
        prev_sy1 = sy1;                                                         \
                                                                                \
        if(sy0 == sy1)                                                        \
            for(dx = 0; dx < dsize.width; dx++)                               \
                dst[dx] = (arrtype)descale_macro(mul_one_macro(buf0[dx]));     \
        else                                                                    \
            for(dx = 0; dx < dsize.width; dx++)                               \
                dst[dx] = (arrtype)descale_macro(mul_one_macro(buf0[dx]) +     \
                                                  fy*(buf1[dx] - buf0[dx]));    \
    }                                                                           \
                                                                                \
    return CC_OK;                                                               \
}


typedef struct CvDecimateAlpha
{
    int si, di;
    float alpha;
}
CvDecimateAlpha;


#define  ICC_DEF_RESIZE_AREA_FAST_FUNC(flavor, arrtype, worktype, cast_macro) \
static CStatus C_STDCALL                                                      \
icvResize_AreaFast_##flavor##_CnR(const arrtype* src, int srcstep, CSize ssize,\
                              arrtype* dst, int dststep, CSize dsize, int cn,  \
                              const int* ofs, const int* xofs)                 \
{                                                                               \
    int dy, dx, k = 0;                                                          \
    int scale_x = ssize.width/dsize.width;                                      \
    int scale_y = ssize.height/dsize.height;                                    \
    int area = scale_x*scale_y;                                                 \
    float scale = 1.f/(scale_x*scale_y);                                        \
                                                                                \
    srcstep /= sizeof(src[0]);                                                  \
    dststep /= sizeof(dst[0]);                                                  \
    dsize.width *= cn;                                                          \
                                                                                \
    for(dy = 0; dy < dsize.height; dy++, dst += dststep)                      \
        for(dx = 0; dx < dsize.width; dx++)                                   \
        {                                                                       \
            const arrtype* _src = src + dy*scale_y*srcstep + xofs[dx];          \
            worktype sum = 0;                                                   \
                                                                                \
            for(k = 0; k <= area - 4; k += 4)                                 \
                sum += _src[ofs[k]] + _src[ofs[k+1]] +                          \
                       _src[ofs[k+2]] + _src[ofs[k+3]];                         \
                                                                                \
            for(; k < area; k++)                                              \
                sum += _src[ofs[k]];                                            \
                                                                                \
            dst[dx] = (arrtype)cast_macro(sum*scale);                         \
        }                                                                       \
                                                                                \
    return CC_OK;                                                               \
}


#define  ICC_DEF_RESIZE_AREA_FUNC(flavor, arrtype, load_macro, cast_macro)    \
static CStatus C_STDCALL                                                      \
icvResize_Area_##flavor##_CnR(const arrtype* src, int srcstep, CSize ssize,   \
                               arrtype* dst, int dststep, CSize dsize,         \
                               int cn, const CvDecimateAlpha* xofs,             \
                               int xofs_count, float* buf, float* sum)         \
{                                                                               \
    int k, sy, dx, cur_dy = 0;                                                  \
    float scale_y = (float)ssize.height/dsize.height;                           \
                                                                                \
    srcstep /= sizeof(src[0]);                                                  \
    dststep /= sizeof(dst[0]);                                                  \
    dsize.width *= cn;                                                          \
                                                                                \
    for(sy = 0; sy < ssize.height; sy++, src += srcstep)                      \
    {                                                                           \
        if(cn == 1)                                                           \
            for(k = 0; k < xofs_count; k++)                                   \
            {                                                                   \
                int dxn = xofs[k].di;                                           \
                float alpha = xofs[k].alpha;                                    \
                buf[dxn] = buf[dxn] + load_macro(src[xofs[k].si])*alpha;        \
            }                                                                   \
        else if(cn == 2)                                                      \
            for(k = 0; k < xofs_count; k++)                                   \
            {                                                                   \
                int sxn = xofs[k].si;                                           \
                int dxn = xofs[k].di;                                           \
                float alpha = xofs[k].alpha;                                    \
                float t0 = buf[dxn] + load_macro(src[sxn])*alpha;               \
                float t1 = buf[dxn+1] + load_macro(src[sxn+1])*alpha;           \
                buf[dxn] = t0; buf[dxn+1] = t1;                                 \
            }                                                                   \
        else if(cn == 3)                                                      \
            for(k = 0; k < xofs_count; k++)                                   \
            {                                                                   \
                int sxn = xofs[k].si;                                           \
                int dxn = xofs[k].di;                                           \
                float alpha = xofs[k].alpha;                                    \
                float t0 = buf[dxn] + load_macro(src[sxn])*alpha;               \
                float t1 = buf[dxn+1] + load_macro(src[sxn+1])*alpha;           \
                float t2 = buf[dxn+2] + load_macro(src[sxn+2])*alpha;           \
                buf[dxn] = t0; buf[dxn+1] = t1; buf[dxn+2] = t2;                \
            }                                                                   \
        else                                                                    \
            for(k = 0; k < xofs_count; k++)                                   \
            {                                                                   \
                int sxn = xofs[k].si;                                           \
                int dxn = xofs[k].di;                                           \
                float alpha = xofs[k].alpha;                                    \
                float t0 = buf[dxn] + load_macro(src[sxn])*alpha;               \
                float t1 = buf[dxn+1] + load_macro(src[sxn+1])*alpha;           \
                buf[dxn] = t0; buf[dxn+1] = t1;                                 \
                t0 = buf[dxn+2] + load_macro(src[sxn+2])*alpha;                 \
                t1 = buf[dxn+3] + load_macro(src[sxn+3])*alpha;                 \
                buf[dxn+2] = t0; buf[dxn+3] = t1;                               \
            }                                                                   \
                                                                                \
        if((cur_dy + 1)*scale_y <= sy + 1 || sy == ssize.height - 1)          \
        {                                                                       \
            float beta = sy + 1 - (cur_dy+1)*scale_y, beta1;                    \
            beta = MAX(beta, 0);                                              \
            beta1 = 1 - beta;                                                   \
            if(fabs(beta) < 1e-3)                                             \
                for(dx = 0; dx < dsize.width; dx++)                           \
                {                                                               \
                    dst[dx] = (arrtype)cast_macro(sum[dx] + buf[dx]);           \
                    sum[dx] = buf[dx] = 0;                                      \
                }                                                               \
            else                                                                \
                for(dx = 0; dx < dsize.width; dx++)                           \
                {                                                               \
                    dst[dx] = (arrtype)cast_macro(sum[dx] + buf[dx]*beta1);     \
                    sum[dx] = buf[dx]*beta;                                     \
                    buf[dx] = 0;                                                \
                }                                                               \
            dst += dststep;                                                     \
            cur_dy++;                                                           \
        }                                                                       \
        else                                                                    \
            for(dx = 0; dx < dsize.width; dx += 2)                            \
            {                                                                   \
                float t0 = sum[dx] + buf[dx];                                   \
                float t1 = sum[dx+1] + buf[dx+1];                               \
                sum[dx] = t0; sum[dx+1] = t1;                                   \
                buf[dx] = buf[dx+1] = 0;                                        \
            }                                                                   \
    }                                                                           \
                                                                                \
    return CC_OK;                                                               \
}


#define  ICC_DEF_RESIZE_BICUBIC_FUNC(flavor, arrtype, worktype, load_macro,    \
                                      cast_macro1, cast_macro2)                \
static CStatus C_STDCALL                                                      \
icvResize_Bicubic_##flavor##_CnR(const arrtype* src, int srcstep, CSize ssize,\
                                  arrtype* dst, int dststep, CSize dsize,      \
                                  int cn, int xmin, int xmax,                   \
                                  const CvResizeAlpha* xofs, float** buf)      \
{                                                                               \
    float scale_y = (float)ssize.height/dsize.height;                           \
    int dx, dy, sx, sy, sy2, ify;                                               \
    int prev_sy2 = -2;                                                          \
                                                                                \
    xmin *= cn; xmax *= cn;                                                     \
    dsize.width *= cn;                                                          \
    ssize.width *= cn;                                                          \
    srcstep /= sizeof(src[0]);                                                  \
    dststep /= sizeof(dst[0]);                                                  \
                                                                                \
    for(dy = 0; dy < dsize.height; dy++, dst += dststep)                      \
    {                                                                           \
        float w0, w1, w2, w3;                                                   \
        float fy, x, sum;                                                       \
        float *row, *row0, *row1, *row2, *row3;                                 \
        int k1, k = 4;                                                          \
                                                                                \
        fy = dy*scale_y;                                                        \
        sy = cFloor(fy);                                                       \
        fy -= sy;                                                               \
        ify = cRound(fy*ICC_CUBIC_TAB_SIZE);                                   \
        sy2 = sy + 2;                                                           \
                                                                                \
        if(sy2 > prev_sy2)                                                    \
        {                                                                       \
            int delta = prev_sy2 - sy + 2;                                      \
            for(k = 0; k < delta; k++)                                        \
                CC_SWAP(buf[k], buf[k+4-delta], row);                         \
        }                                                                       \
                                                                                \
        for(sy += k - 1; k < 4; k++, sy++)                                    \
        {                                                                       \
            const arrtype* _src = src + sy*srcstep;                             \
                                                                                \
            row = buf[k];                                                       \
            if(sy < 0)                                                        \
                continue;                                                       \
            if(sy >= ssize.height)                                            \
            {                                                                   \
                assert(k > 0);                                                \
                memcpy(row, buf[k-1], dsize.width*sizeof(row[0]));            \
                continue;                                                       \
            }                                                                   \
                                                                                \
            for(dx = 0; dx < xmin; dx++)                                      \
            {                                                                   \
                int ifx = xofs[dx].ialpha, sx0 = xofs[dx].idx;                  \
                sx = sx0 + cn*2;                                                \
                while(sx >= ssize.width)                                      \
                    sx -= cn;                                                   \
                x = load_macro(_src[sx]);                                       \
                sum = x*icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ifx)*2 + 1];       \
                if((unsigned)(sx = sx0 + cn) < (unsigned)ssize.width)         \
                    x = load_macro(_src[sx]);                                   \
                sum += x*icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ifx)*2];          \
                if((unsigned)(sx = sx0) < (unsigned)ssize.width)              \
                    x = load_macro(_src[sx]);                                   \
                sum += x*icvCubicCoeffs[ifx*2];                                 \
                if((unsigned)(sx = sx0 - cn) < (unsigned)ssize.width)         \
                    x = load_macro(_src[sx]);                                   \
                row[dx] = sum + x*icvCubicCoeffs[ifx*2 + 1];                    \
            }                                                                   \
                                                                                \
            for(; dx < xmax; dx++)                                            \
            {                                                                   \
                int ifx = xofs[dx].ialpha;                                      \
                int sx0 = xofs[dx].idx;                                         \
                row[dx] = _src[sx0 - cn]*icvCubicCoeffs[ifx*2 + 1] +            \
                    _src[sx0]*icvCubicCoeffs[ifx*2] +                           \
                    _src[sx0 + cn]*icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE-ifx)*2] + \
                    _src[sx0 + cn*2]*icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE-ifx)*2+1];\
            }                                                                   \
                                                                                \
            for(; dx < dsize.width; dx++)                                     \
            {                                                                   \
                int ifx = xofs[dx].ialpha, sx0 = xofs[dx].idx;                  \
                x = load_macro(_src[sx0 - cn]);                                 \
                sum = x*icvCubicCoeffs[ifx*2 + 1];                              \
                if((unsigned)(sx = sx0) < (unsigned)ssize.width)              \
                    x = load_macro(_src[sx]);                                   \
                sum += x*icvCubicCoeffs[ifx*2];                                 \
                if((unsigned)(sx = sx0 + cn) < (unsigned)ssize.width)         \
                    x = load_macro(_src[sx]);                                   \
                sum += x*icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ifx)*2];          \
                if((unsigned)(sx = sx0 + cn*2) < (unsigned)ssize.width)       \
                    x = load_macro(_src[sx]);                                   \
                row[dx] = sum + x*icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE-ifx)*2+1]; \
            }                                                                   \
                                                                                \
            if(sy == 0)                                                       \
                for(k1 = 0; k1 < k; k1++)                                     \
                    memcpy(buf[k1], row, dsize.width*sizeof(row[0]));          \
        }                                                                       \
                                                                                \
        prev_sy2 = sy2;                                                         \
                                                                                \
        row0 = buf[0]; row1 = buf[1];                                           \
        row2 = buf[2]; row3 = buf[3];                                           \
                                                                                \
        w0 = icvCubicCoeffs[ify*2+1];                                           \
        w1 = icvCubicCoeffs[ify*2];                                             \
        w2 = icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ify)*2];                      \
        w3 = icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE - ify)*2 + 1];                  \
                                                                                \
        for(dx = 0; dx < dsize.width; dx++)                                   \
        {                                                                       \
            worktype val = cast_macro1(row0[dx]*w0 + row1[dx]*w1 +             \
                                        row2[dx]*w2 + row3[dx]*w3);            \
            dst[dx] = cast_macro2(val);                                         \
        }                                                                       \
    }                                                                           \
                                                                                \
    return CC_OK;                                                               \
}


ICC_DEF_RESIZE_BILINEAR_FUNC(8u, uchar, int, ialpha,
                              ICC_WARP_MUL_ONE_8U, ICC_WARP_DESCALE_8U)
ICC_DEF_RESIZE_BILINEAR_FUNC(16u, ushort, float, alpha, CC_NOP, cRound)
ICC_DEF_RESIZE_BILINEAR_FUNC(32f, float, float, alpha, CC_NOP, CC_NOP)

ICC_DEF_RESIZE_BICUBIC_FUNC(8u, uchar, int, CC_8TO32F, cRound, CC_CAST_8U)
ICC_DEF_RESIZE_BICUBIC_FUNC(16u, ushort, int, CC_NOP, cRound, CC_CAST_16U)
ICC_DEF_RESIZE_BICUBIC_FUNC(32f, float, float, CC_NOP, CC_NOP, CC_NOP)

ICC_DEF_RESIZE_AREA_FAST_FUNC(8u, uchar, int, cRound)
ICC_DEF_RESIZE_AREA_FAST_FUNC(16u, ushort, int, cRound)
ICC_DEF_RESIZE_AREA_FAST_FUNC(32f, float, float, CC_NOP)

ICC_DEF_RESIZE_AREA_FUNC(8u, uchar, CC_8TO32F, cRound)
ICC_DEF_RESIZE_AREA_FUNC(16u, ushort, CC_NOP, cRound)
ICC_DEF_RESIZE_AREA_FUNC(32f, float, CC_NOP, CC_NOP)


static void icvInitResizeTab(CvFuncTable  bilin_tab,
                              CvFuncTable  bicube_tab,
                              CvFuncTable  areafast_tab,
                              CvFuncTable  area_tab)
{
    bilin_tab[CC_8U] = (void*)icvResize_Bilinear_8u_CnR;
    bilin_tab[CC_16U] = (void*)icvResize_Bilinear_16u_CnR;
    bilin_tab[CC_32F] = (void*)icvResize_Bilinear_32f_CnR;

    bicube_tab[CC_8U] = (void*)icvResize_Bicubic_8u_CnR;
    bicube_tab[CC_16U] = (void*)icvResize_Bicubic_16u_CnR;
    bicube_tab[CC_32F] = (void*)icvResize_Bicubic_32f_CnR;

    areafast_tab[CC_8U] = (void*)icvResize_AreaFast_8u_CnR;
    areafast_tab[CC_16U] = (void*)icvResize_AreaFast_16u_CnR;
    areafast_tab[CC_32F] = (void*)icvResize_AreaFast_32f_CnR;

    area_tab[CC_8U] = (void*)icvResize_Area_8u_CnR;
    area_tab[CC_16U] = (void*)icvResize_Area_16u_CnR;
    area_tab[CC_32F] = (void*)icvResize_Area_32f_CnR;
}


typedef CStatus (C_STDCALL * CvResizeBilinearFunc)
                    (const void* src, int srcstep, CSize ssize,
                      void* dst, int dststep, CSize dsize,
                      int cn, int xmax, const CvResizeAlpha* xofs,
                      const CvResizeAlpha* yofs, float* buf0, float* buf1);

typedef CStatus (C_STDCALL * CvResizeBicubicFunc)
                    (const void* src, int srcstep, CSize ssize,
                      void* dst, int dststep, CSize dsize,
                      int cn, int xmin, int xmax,
                      const CvResizeAlpha* xofs, float** buf);

typedef CStatus (C_STDCALL * CvResizeAreaFastFunc)
                    (const void* src, int srcstep, CSize ssize,
                      void* dst, int dststep, CSize dsize,
                      int cn, const int* ofs, const int *xofs);

typedef CStatus (C_STDCALL * CvResizeAreaFunc)
                    (const void* src, int srcstep, CSize ssize,
                      void* dst, int dststep, CSize dsize,
                      int cn, const CvDecimateAlpha* xofs,
                      int xofs_count, float* buf, float* sum);


//////////////////////////////////////////////////////////////////////////////////////////

CC_IMPL img_t*
cvResize(const img_t* src, img_t* dst, int method CC_DEFAULT(CC_INTER_LINEAR))
{
    static CvFuncTable bilin_tab, bicube_tab, areafast_tab, area_tab;
    static int inittab = 0;
    void* temp_buf = 0;

    CC_FUNCNAME("cvResize");

    __BEGIN__;
    
    CSize ssize, dsize;
    float scale_x, scale_y;
    int k, sx, sy, dx, dy;
    int cn;
    TypeId type;
   
    if(CC_ARE_SIZES_EQ(src, dst))
        CC_CALL(cvCopy(src, dst));

    if(!CC_ARE_TYPES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(!inittab)
    {
        icvInitResizeTab(bilin_tab, bicube_tab, areafast_tab, area_tab);
        inittab = 1;
    }

    ssize = cvGetMatSize(src);
    dsize = cvGetMatSize(dst);
    type = CC_MAT_DEPTH(src);
    
    cn = CC_MAT_CN(src);
    scale_x = (float)ssize.width/dsize.width;
    scale_y = (float)ssize.height/dsize.height;

    if(method == CC_INTER_CUBIC &&
        (MIN(ssize.width, dsize.width) <= 4 ||
        MIN(ssize.height, dsize.height) <= 4))
        method = CC_INTER_LINEAR;

    if(method == CC_INTER_NN)
    {
        IPPI_CALL(icvResize_NN_8u_C1R(src->tt.data, src->step, ssize,
                                        dst->tt.data, dst->step, dsize,
                                        CC_TYPE_SIZE(src->tid)));
    }
    else if(method == CC_INTER_LINEAR || method == CC_INTER_AREA)
    {
        if(method == CC_INTER_AREA &&
            ssize.width >= dsize.width && ssize.height >= dsize.height)
        {
            // "area" method for (scale_x > 1 & scale_y > 1)
            int iscale_x = cRound(scale_x);
            int iscale_y = cRound(scale_y);

            if(fabs(scale_x - iscale_x) < DBL_EPSILON &&
                fabs(scale_y - iscale_y) < DBL_EPSILON)
            {
                int area = iscale_x*iscale_y;
                int srcstep = src->step / CC_TYPE_SIZE(type);
                int* ofs = (int*)cvStackAlloc((area + dsize.width*cn)*sizeof(int));
                int* xofs = ofs + area;
                CvResizeAreaFastFunc func = (CvResizeAreaFastFunc)areafast_tab[type];

                if(!func)
                    CC_ERROR(CC_StsUnsupportedFormat, "");
                
                for(sy = 0, k = 0; sy < iscale_y; sy++)
                    for(sx = 0; sx < iscale_x; sx++)
                        ofs[k++] = sy*srcstep + sx*cn;

                for(dx = 0; dx < dsize.width; dx++)
                {
                    sx = dx*iscale_x*cn;
                    for(k = 0; k < cn; k++)
                        xofs[dx*cn + k] = sx + k;
                }

                IPPI_CALL(func(src->tt.data, src->step, ssize, dst->tt.data,
                                 dst->step, dsize, cn, ofs, xofs));
            }
            else
            {
                int buf_len = dsize.width*cn + 4, buf_size, xofs_count = 0;
                float scale = 1.f/(scale_x*scale_y);
                float *buf, *sum;
                CvDecimateAlpha* xofs;
                CvResizeAreaFunc func = (CvResizeAreaFunc)area_tab[type];

                if(!func || cn > 4)
                    CC_ERROR(CC_StsUnsupportedFormat, "");

                buf_size = buf_len*2*sizeof(float) + ssize.width*2*sizeof(CvDecimateAlpha);
                if(buf_size < CC_MAX_LOCAL_SIZE)
                    buf = (float*)cvStackAlloc(buf_size);
                else
                    CC_CALL(temp_buf = buf = (float*)cAlloc(buf_size));
                sum = buf + buf_len;
                xofs = (CvDecimateAlpha*)(sum + buf_len);

                for(dx = 0, k = 0; dx < dsize.width; dx++)
                {
                    float fsx1 = dx*scale_x, fsx2 = fsx1 + scale_x;
                    int sx1 = cCeil(fsx1), sx2 = cFloor(fsx2);

                    assert((unsigned)sx1 < (unsigned)ssize.width);

                    if(sx1 > fsx1)
                    {
                        assert(k < ssize.width*2);            
                        xofs[k].di = dx*cn;
                        xofs[k].si = (sx1-1)*cn;
                        xofs[k++].alpha = (sx1 - fsx1)*scale;
                    }

                    for(sx = sx1; sx < sx2; sx++)
                    {
                        assert(k < ssize.width*2);
                        xofs[k].di = dx*cn;
                        xofs[k].si = sx*cn;
                        xofs[k++].alpha = scale;
                    }

                    if(fsx2 - sx2 > 1e-3)
                    {
                        assert(k < ssize.width*2);
                        assert((unsigned)sx2 < (unsigned)ssize.width);
                        xofs[k].di = dx*cn;
                        xofs[k].si = sx2*cn;
                        xofs[k++].alpha = (fsx2 - sx2)*scale;
                    }
                }

                xofs_count = k;
                memset(sum, 0, buf_len*sizeof(float));
                memset(buf, 0, buf_len*sizeof(float));

                IPPI_CALL(func(src->tt.data, src->step, ssize, dst->tt.data,
                                 dst->step, dsize, cn, xofs, xofs_count, buf, sum));
            }
        }
        else // true "area" method for the cases (scale_x > 1 & scale_y < 1) and
             // (scale_x < 1 & scale_y > 1) is not implemented.
             // instead, it is emulated via some variant of bilinear interpolation.
        {
            float inv_scale_x = (float)dsize.width/ssize.width;
            float inv_scale_y = (float)dsize.height/ssize.height;
            int xmax = dsize.width, width = dsize.width*cn, buf_size;
            float *buf0, *buf1;
            CvResizeAlpha *xofs, *yofs;
            int area_mode = method == CC_INTER_AREA;
            float fx, fy;
            CvResizeBilinearFunc func = (CvResizeBilinearFunc)bilin_tab[type];

            if(!func)
                CC_ERROR(CC_StsUnsupportedFormat, "");

            buf_size = width*2*sizeof(float) + (width + dsize.height)*sizeof(CvResizeAlpha);
            if(buf_size < CC_MAX_LOCAL_SIZE)
                buf0 = (float*)cvStackAlloc(buf_size);
            else
                CC_CALL(temp_buf = buf0 = (float*)cAlloc(buf_size));

            buf1 = buf0 + width;
            xofs = (CvResizeAlpha*)(buf1 + width);
            yofs = xofs + width;

            for(dx = 0; dx < dsize.width; dx++)
            {
                if(!area_mode)
                {
                    fx = (float)((dx+0.5)*scale_x - 0.5);
                    sx = cFloor(fx);
                    fx -= sx;
                }
                else
                {
                    sx = cFloor(dx*scale_x);
                    fx = (dx+1) - (sx+1)*inv_scale_x;
                    fx = fx <= 0 ? 0.f : fx - cFloor(fx);
                }

                if(sx < 0)
                    fx = 0, sx = 0;

                if(sx >= ssize.width-1)
                {
                    fx = 0, sx = ssize.width-1;
                    if(xmax >= dsize.width)
                        xmax = dx;
                }

                if(type != CC_8U)
                    for(k = 0, sx *= cn; k < cn; k++)
                        xofs[dx*cn + k].idx = sx + k, xofs[dx*cn + k].alpha = fx;
                else
                    for(k = 0, sx *= cn; k < cn; k++)
                        xofs[dx*cn + k].idx = sx + k,
                        xofs[dx*cn + k].ialpha = CC_FLT_TO_FIX(fx, ICC_WARP_SHIFT);
            }

            for(dy = 0; dy < dsize.height; dy++)
            {
                if(!area_mode)
                {
                    fy = (float)((dy+0.5)*scale_y - 0.5);
                    sy = cFloor(fy);
                    fy -= sy;
                    if(sy < 0)
                        sy = 0, fy = 0;
                }
                else
                {
                    sy = cFloor(dy*scale_y);
                    fy = (dy+1) - (sy+1)*inv_scale_y;
                    fy = fy <= 0 ? 0.f : fy - cFloor(fy);
                }

                yofs[dy].idx = sy;
                if(type != CC_8U)
                    yofs[dy].alpha = fy;
                else
                    yofs[dy].ialpha = CC_FLT_TO_FIX(fy, ICC_WARP_SHIFT);
            }

            IPPI_CALL(func(src->tt.data, src->step, ssize, dst->tt.data,
                             dst->step, dsize, cn, xmax, xofs, yofs, buf0, buf1));
        }
    }
    else if(method == CC_INTER_CUBIC)
    {
        int width = dsize.width*cn, buf_size;
        int xmin = dsize.width, xmax = -1;
        CvResizeAlpha* xofs;
        float* buf[4];
        CvResizeBicubicFunc func = (CvResizeBicubicFunc)bicube_tab[type];

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");
        
        buf_size = width*(4*sizeof(float) + sizeof(xofs[0]));
        if(buf_size < CC_MAX_LOCAL_SIZE)
            buf[0] = (float*)cvStackAlloc(buf_size);
        else
            CC_CALL(temp_buf = buf[0] = (float*)cAlloc(buf_size));

        for(k = 1; k < 4; k++)
            buf[k] = buf[k-1] + width;
        xofs = (CvResizeAlpha*)(buf[3] + width);

        icvInitCubicCoeffTab();

        for(dx = 0; dx < dsize.width; dx++)
        {
            float fx = dx*scale_x;
            sx = cFloor(fx);
            fx -= sx;
            int ifx = cRound(fx*ICC_CUBIC_TAB_SIZE);
            if(sx-1 >= 0 && xmin > dx)
                xmin = dx;
            if(sx+2 < ssize.width)
                xmax = dx + 1;
        
            // at least one of 4 points should be within the image - to
            // be able to set other points to the same value. see the loops
            // for(dx = 0; dx < xmin; dx++) ... and for(; dx < width; dx++) ...
            if(sx < -2)
                sx = -2;
            else if(sx > ssize.width)
                sx = ssize.width;

            for(k = 0; k < cn; k++)
            {
                xofs[dx*cn + k].idx = sx*cn + k;
                xofs[dx*cn + k].ialpha = ifx;
            }
        }
    
        IPPI_CALL(func(src->tt.data, src->step, ssize, dst->tt.data,
                         dst->step, dsize, cn, xmin, xmax, xofs, buf));
    }
    else
        CC_ERROR(CC_StsBadFlag, "Unknown/unsupported interpolation method");

    __END__;

    cFree(&temp_buf);
	return dst;
}

CC_IMPL img_t*
cvResize(const img_t* src, img_t* dst, CSize dsize, double fx = 0, double fy = 0,
         int method CC_DEFAULT(CC_INTER_LINEAR)) {
  IM_BEGIN(im, 1);
  if (src==dst) {
    src = cvCloneMat(im++, src);
  }
  if (dsize.h*dsize.w<=0) {
    dsize.w = cRound(fx * src->w);
    dsize.h = cRound(fy * src->h);
  }
  cvSetMat(dst, dsize.h, dsize.w, src->tid, src->c);
  cvResize(src, dst, method);
  IM_END(im);
  return dst;
}


/****************************************************************************************\
*                                     WarpAffine                                         *
\****************************************************************************************/

#define ICC_DEF_WARP_AFFINE_BILINEAR_FUNC(flavor, arrtype, worktype,       \
            scale_alpha_macro, mul_one_macro, descale_macro, cast_macro)   \
static CStatus C_STDCALL                                                  \
icvWarpAffine_Bilinear_##flavor##_CnR(                         \
    const arrtype* src, int step, CSize ssize,                             \
    arrtype* dst, int dststep, CSize dsize,                                \
    const double* matrix, int cn,                                           \
    const arrtype* fillval, const int* ofs)                                \
{                                                                           \
    int x, y, k;                                                            \
    double  A12 = matrix[1], b1 = matrix[2];                                \
    double  A22 = matrix[4], b2 = matrix[5];                                \
                                                                            \
    step /= sizeof(src[0]);                                                 \
    dststep /= sizeof(dst[0]);                                              \
                                                                            \
    for(y = 0; y < dsize.height; y++, dst += dststep)                     \
    {                                                                       \
        int xs = CC_FLT_TO_FIX(A12*y + b1, ICC_WARP_SHIFT);               \
        int ys = CC_FLT_TO_FIX(A22*y + b2, ICC_WARP_SHIFT);               \
                                                                            \
        for(x = 0; x < dsize.width; x++)                                  \
        {                                                                   \
            int ixs = xs + ofs[x*2];                                        \
            int iys = ys + ofs[x*2+1];                                      \
            worktype a = scale_alpha_macro(ixs & ICC_WARP_MASK);          \
            worktype b = scale_alpha_macro(iys & ICC_WARP_MASK);          \
            worktype p0, p1;                                                \
            ixs >>= ICC_WARP_SHIFT;                                         \
            iys >>= ICC_WARP_SHIFT;                                         \
                                                                            \
            if((unsigned)ixs < (unsigned)(ssize.width - 1) &&              \
                (unsigned)iys < (unsigned)(ssize.height - 1))              \
            {                                                               \
                const arrtype* ptr = src + step*iys + ixs*cn;               \
                                                                            \
                for(k = 0; k < cn; k++)                                   \
                {                                                           \
                    p0 = mul_one_macro(ptr[k]) +                            \
                        a * (ptr[k+cn] - ptr[k]);                           \
                    p1 = mul_one_macro(ptr[k+step]) +                       \
                        a * (ptr[k+cn+step] - ptr[k+step]);                 \
                    p0 = descale_macro(mul_one_macro(p0) + b*(p1 - p0));    \
                    dst[x*cn+k] = (arrtype)cast_macro(p0);                  \
                }                                                           \
            }                                                               \
            else if((unsigned)(ixs+1) < (unsigned)(ssize.width+1) &&       \
                     (unsigned)(iys+1) < (unsigned)(ssize.height+1))        \
            {                                                               \
                int x0 = ICC_WARP_CLIP_X(ixs);                            \
                int y0 = ICC_WARP_CLIP_Y(iys);                            \
                int x1 = ICC_WARP_CLIP_X(ixs + 1);                        \
                int y1 = ICC_WARP_CLIP_Y(iys + 1);                        \
                const arrtype* ptr0, *ptr1, *ptr2, *ptr3;                   \
                                                                            \
                ptr0 = src + y0*step + x0*cn;                               \
                ptr1 = src + y0*step + x1*cn;                               \
                ptr2 = src + y1*step + x0*cn;                               \
                ptr3 = src + y1*step + x1*cn;                               \
                                                                            \
                for(k = 0; k < cn; k++)                                   \
                {                                                           \
                    p0 = mul_one_macro(ptr0[k]) + a * (ptr1[k] - ptr0[k]);  \
                    p1 = mul_one_macro(ptr2[k]) + a * (ptr3[k] - ptr2[k]);  \
                    p0 = descale_macro(mul_one_macro(p0) + b*(p1 - p0));  \
                    dst[x*cn+k] = (arrtype)cast_macro(p0);                  \
                }                                                           \
            }                                                               \
            else if(fillval)                                              \
                for(k = 0; k < cn; k++)                                   \
                    dst[x*cn+k] = fillval[k];                               \
        }                                                                   \
    }                                                                       \
                                                                            \
    return CC_OK;                                                           \
}


#define ICC_WARP_SCALE_ALPHA(x) ((x)*(1./(ICC_WARP_MASK+1)))

ICC_DEF_WARP_AFFINE_BILINEAR_FUNC(8u, uchar, int, CC_NOP, ICC_WARP_MUL_ONE_8U,
                                   ICC_WARP_DESCALE_8U, CC_NOP)
//ICC_DEF_WARP_AFFINE_BILINEAR_FUNC(8u, uchar, double, ICC_WARP_SCALE_ALPHA, CC_NOP,
//                                   CC_NOP, ICC_WARP_CAST_8U)
ICC_DEF_WARP_AFFINE_BILINEAR_FUNC(16u, ushort, double, ICC_WARP_SCALE_ALPHA, CC_NOP,
                                   CC_NOP, cRound)
ICC_DEF_WARP_AFFINE_BILINEAR_FUNC(32f, float, double, ICC_WARP_SCALE_ALPHA, CC_NOP,
                                   CC_NOP, CC_NOP)


typedef CStatus (C_STDCALL * CvWarpAffineFunc)(
    const void* src, int srcstep, CSize ssize,
    void* dst, int dststep, CSize dsize,
    const double* matrix, int cn,
    const void* fillval, const int* ofs);

static void icvInitWarpAffineTab(CvFuncTable  bilin_tab)
{
    bilin_tab[CC_8U] = (void*)icvWarpAffine_Bilinear_8u_CnR;
    bilin_tab[CC_16U] = (void*)icvWarpAffine_Bilinear_16u_CnR;
    bilin_tab[CC_32F] = (void*)icvWarpAffine_Bilinear_32f_CnR;
}

//////////////////////////////////////////////////////////////////////////////////////////

CC_IMPL void
cvWarpAffine(const img_t* src, img_t* dst, const img_t* matrix,
              int flags, CScalar fillval)
{
    static CvFuncTable bilin_tab;
    static int inittab = 0;

    CC_FUNCNAME("cvWarpAffine");

    __BEGIN__;
    
    TypeId type;
    int k, cn, *ofs = 0;
    double src_matrix[6], dst_matrix[6];
    double fillbuf[4];
    int method = flags & 3;
    img_t srcAb = cvMat1(2, 3, CC_64F, 1, src_matrix),
          dstAb = cvMat1(2, 3, CC_64F, 1, dst_matrix),
          A, b, invA, invAb;
    CvWarpAffineFunc func;
    CSize ssize, dsize;
    
    if(!inittab)
    {
        icvInitWarpAffineTab(bilin_tab);
        inittab = 1;
    }

    if(!CC_ARE_TYPES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if( CC_MAT_CN(matrix) != 1 ||
        CC_MAT_TYPE(matrix) < CC_32F || matrix->rows != 2 || matrix->cols != 3)
        CC_ERROR(CC_StsBadArg,
        "Transformation matrix should be 2x3 floating-point single-channel matrix");

    if(flags & CC_WARP_INVERSE_MAP)
        cvConvertScale(matrix, &dstAb);
    else
    {
        // [R|t] -> [R^-1 | -(R^-1)*t]
        cvConvertScale(matrix, &srcAb);
        cvGetCols(&srcAb, &A, 0, 2);
        cvGetCol(&srcAb, &b, 2);
        cvGetCols(&dstAb, &invA, 0, 2);
        cvGetCol(&dstAb, &invAb, 2);
        cvInvert(&A, &invA, CC_SVD);
        cvGEMM(&invA, &b, -1, 0, 0, &invAb);
    }

    type = CC_MAT_DEPTH(src);
    
    cn = CC_MAT_CN(src);
    if(cn > 4)
        CC_ERROR(CC_BadNumChannels, "");

    ssize = cvGetMatSize(src);
    dsize = cvGetMatSize(dst);

    cvScalarToRawData(&fillval, fillbuf, CC_MAT_DEPTH(src), CC_MAT_CN(src), 0);
    ofs = (int*)cvStackAlloc(dst->cols*2*sizeof(ofs[0]));
    for(k = 0; k < dst->cols; k++)
    {
        ofs[2*k] = CC_FLT_TO_FIX(dst_matrix[0]*k, ICC_WARP_SHIFT);
        ofs[2*k+1] = CC_FLT_TO_FIX(dst_matrix[3]*k, ICC_WARP_SHIFT);
    }

    /*if(method == CC_INTER_LINEAR)*/
    {
        func = (CvWarpAffineFunc)bilin_tab[type];
        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        IPPI_CALL(func(src->tt.data, src->step, ssize, dst->tt.data,
                         dst->step, dsize, dst_matrix, cn,
                         flags & CC_WARP_FILL_OUTLIERS ? fillbuf : 0, ofs));
    }

    __END__;
}


CC_IMPL img_t*
cv2DRotationMatrix(CPoint2D32f center, double angle,
                    double scale, img_t* matrix)
{
    CC_FUNCNAME("cvGetRotationMatrix");

    __BEGIN__;

    double m[2][3];
    img_t M = cvMat1(2, 3, CC_64F, 1, m);
    double alpha, beta;

    if(!matrix)
        CC_ERROR(CC_StsNullPtr, "");

    angle *= CC_PI/180;
    alpha = cos(angle)*scale;
    beta = sin(angle)*scale;

    m[0][0] = alpha;
    m[0][1] = beta;
    m[0][2] = (1-alpha)*center.x - beta*center.y;
    m[1][0] = -beta;
    m[1][1] = alpha;
    m[1][2] = beta*center.x + (1-alpha)*center.y;

    cvConvert(&M, matrix);

    __END__;

    return matrix;
}


/****************************************************************************************\
*                                    WarpPerspective                                     *
\****************************************************************************************/

#define ICC_DEF_WARP_PERSPECTIVE_BILINEAR_FUNC(flavor, arrtype, load_macro, cast_macro)\
static CStatus C_STDCALL                                                  \
icvWarpPerspective_Bilinear_##flavor##_CnR(                    \
    const arrtype* src, int step, CSize ssize,                             \
    arrtype* dst, int dststep, CSize dsize,                                \
    const double* matrix, int cn,                                           \
    const arrtype* fillval)                                                \
{                                                                           \
    int x, y, k;                                                            \
    float A11 = (float)matrix[0], A12 = (float)matrix[1], A13 = (float)matrix[2];\
    float A21 = (float)matrix[3], A22 = (float)matrix[4], A23 = (float)matrix[5];\
    float A31 = (float)matrix[6], A32 = (float)matrix[7], A33 = (float)matrix[8];\
                                                                            \
    step /= sizeof(src[0]);                                                 \
    dststep /= sizeof(dst[0]);                                              \
                                                                            \
    for(y = 0; y < dsize.height; y++, dst += dststep)                     \
    {                                                                       \
        float xs0 = A12*y + A13;                                            \
        float ys0 = A22*y + A23;                                            \
        float ws = A32*y + A33;                                             \
                                                                            \
        for(x = 0; x < dsize.width; x++, xs0 += A11, ys0 += A21, ws += A31)\
        {                                                                   \
            float inv_ws = 1.f/ws;                                          \
            float xs = xs0*inv_ws;                                          \
            float ys = ys0*inv_ws;                                          \
            int ixs = cFloor(xs);                                          \
            int iys = cFloor(ys);                                          \
            float a = xs - ixs;                                             \
            float b = ys - iys;                                             \
            float p0, p1;                                                   \
                                                                            \
            if((unsigned)ixs < (unsigned)(ssize.width - 1) &&              \
                (unsigned)iys < (unsigned)(ssize.height - 1))              \
            {                                                               \
                const arrtype* ptr = src + step*iys + ixs*cn;               \
                                                                            \
                for(k = 0; k < cn; k++)                                   \
                {                                                           \
                    p0 = load_macro(ptr[k]) +                               \
                        a * (load_macro(ptr[k+cn]) - load_macro(ptr[k]));   \
                    p1 = load_macro(ptr[k+step]) +                          \
                        a * (load_macro(ptr[k+cn+step]) -                   \
                             load_macro(ptr[k+step]));                      \
                    dst[x*cn+k] = (arrtype)cast_macro(p0 + b*(p1 - p0));    \
                }                                                           \
            }                                                               \
            else if((unsigned)(ixs+1) < (unsigned)(ssize.width+1) &&       \
                     (unsigned)(iys+1) < (unsigned)(ssize.height+1))        \
            {                                                               \
                int x0 = ICC_WARP_CLIP_X(ixs);                            \
                int y0 = ICC_WARP_CLIP_Y(iys);                            \
                int x1 = ICC_WARP_CLIP_X(ixs + 1);                        \
                int y1 = ICC_WARP_CLIP_Y(iys + 1);                        \
                const arrtype* ptr0, *ptr1, *ptr2, *ptr3;                   \
                                                                            \
                ptr0 = src + y0*step + x0*cn;                               \
                ptr1 = src + y0*step + x1*cn;                               \
                ptr2 = src + y1*step + x0*cn;                               \
                ptr3 = src + y1*step + x1*cn;                               \
                                                                            \
                for(k = 0; k < cn; k++)                                   \
                {                                                           \
                    p0 = load_macro(ptr0[k]) +                              \
                        a * (load_macro(ptr1[k]) - load_macro(ptr0[k]));    \
                    p1 = load_macro(ptr2[k]) +                              \
                        a * (load_macro(ptr3[k]) - load_macro(ptr2[k]));    \
                    dst[x*cn+k] = (arrtype)cast_macro(p0 + b*(p1 - p0));    \
                }                                                           \
            }                                                               \
            else if(fillval)                                              \
                for(k = 0; k < cn; k++)                                   \
                    dst[x*cn+k] = fillval[k];                               \
        }                                                                   \
    }                                                                       \
                                                                            \
    return CC_OK;                                                           \
}


#define ICC_WARP_SCALE_ALPHA(x) ((x)*(1./(ICC_WARP_MASK+1)))

ICC_DEF_WARP_PERSPECTIVE_BILINEAR_FUNC(8u, uchar, CC_8TO32F, cRound)
ICC_DEF_WARP_PERSPECTIVE_BILINEAR_FUNC(16u, ushort, CC_NOP, cRound)
ICC_DEF_WARP_PERSPECTIVE_BILINEAR_FUNC(32f, float, CC_NOP, CC_NOP)

typedef CStatus (C_STDCALL * CvWarpPerspectiveFunc)(
    const void* src, int srcstep, CSize ssize,
    void* dst, int dststep, CSize dsize,
    const double* matrix, int cn, const void* fillval);

static void icvInitWarpPerspectiveTab(CvFuncTable  bilin_tab)
{
    bilin_tab[CC_8U] = (void*)icvWarpPerspective_Bilinear_8u_CnR;
    bilin_tab[CC_16U] = (void*)icvWarpPerspective_Bilinear_16u_CnR;
    bilin_tab[CC_32F] = (void*)icvWarpPerspective_Bilinear_32f_CnR;
}

//////////////////////////////////////////////////////////////////////////////////////////

CC_IMPL void
cvWarpPerspective(const img_t* src, img_t* dst,
                   const img_t* matrix, int flags, CScalar fillval)
{
    static CvFuncTable bilin_tab;
    static int inittab = 0;

    CC_FUNCNAME("cvWarpPerspective");

    __BEGIN__;
    
    
    
    TypeId type;
    int cn;
    int method = flags & 3;
    double src_matrix[9], dst_matrix[9];
    double fillbuf[4];
    img_t A = cvMat1(3, 3, CC_64F, 1, src_matrix),
          invA = cvMat1(3, 3, CC_64F, 1, dst_matrix);
    CvWarpPerspectiveFunc func;
    CSize ssize, dsize;

    if(method == CC_INTER_NN || method == CC_INTER_AREA)
        method = CC_INTER_LINEAR;
    
    if(!inittab)
    {
        icvInitWarpPerspectiveTab(bilin_tab);
        inittab = 1;
    }

    if(!CC_ARE_TYPES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if( CC_MAT_CN(matrix) != 1 ||
        CC_MAT_TYPE(matrix) < CC_32F || matrix->rows != 3 || matrix->cols != 3)
        CC_ERROR(CC_StsBadArg,
        "Transformation matrix should be 3x3 floating-point single-channel matrix");

    if(flags & CC_WARP_INVERSE_MAP)
        cvConvertScale(matrix, &invA);
    else
    {
        cvConvertScale(matrix, &A);
        cvInvert(&A, &invA, CC_SVD);
    }

    type = CC_MAT_DEPTH(src);
    
    cn = CC_MAT_CN(src);
    if(cn > 4)
        CC_ERROR(CC_BadNumChannels, "");
    
    ssize = cvGetMatSize(src);
    dsize = cvGetMatSize(dst);
    
    cvScalarToRawData(&fillval, fillbuf, CC_MAT_DEPTH(src), CC_MAT_CN(src), 0);

    /*if(method == CC_INTER_LINEAR)*/
    {
        func = (CvWarpPerspectiveFunc)bilin_tab[type];
        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        IPPI_CALL(func(src->tt.data, src->step, ssize, dst->tt.data,
                         dst->step, dsize, dst_matrix, cn,
                         flags & CC_WARP_FILL_OUTLIERS ? fillbuf : 0));
    }

    __END__;
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
CC_IMPL img_t*
cvGetPerspectiveTransform(const CPoint2D32f* src,
                          const CPoint2D32f* dst,
                          img_t* matrix)
{
    CC_FUNCNAME("cvGetPerspectiveTransform");

    __BEGIN__;

    double a[8][8];
    double b[8], x[9];

    img_t A = cvMat1(8, 8, CC_64F, 1, a);
    img_t B = cvMat1(8, 1, CC_64F, 1, b);
    img_t X = cvMat1(8, 1, CC_64F, 1, x);

    int i;

    if(!src || !dst || !matrix)
        CC_ERROR(CC_StsNullPtr, "");

    for(i = 0; i < 4; ++i)
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

    cvSolve(&A, &B, &X, CC_SVD);
    x[8] = 1;
    
    X = cvMat1(3, 3, CC_64F, 1, x);
    cvConvert(&X, matrix);

    __END__;

    return matrix;
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
CC_IMPL img_t*
cvGetAffineTransform(const CPoint2D32f * src, const CPoint2D32f * dst, img_t * map_matrix)
{
    CC_FUNCNAME("cvGetAffineTransform");

    __BEGIN__;

    img_t mA, mX, mB;
    double A[6*6];
    double B[6];
    double x[6];
    int i;

    cvInitMatHeader(&mA, 6, 6, CC_64F, 1, A);
    cvInitMatHeader(&mB, 6, 1, CC_64F, 1, B);
    cvInitMatHeader(&mX, 6, 1, CC_64F, 1, x);

	  if(!src || !dst || !map_matrix)
		    CC_ERROR(CC_StsNullPtr, "");

    for(i = 0; i < 3; i++)
    {
        int j = i*12;
        int k = i*12+6;
        A[j] = A[k+3] = src[i].x;
        A[j+1] = A[k+4] = src[i].y;
        A[j+2] = A[k+5] = 1;
        A[j+3] = A[j+4] = A[j+5] = 0;
        A[k] = A[k+1] = A[k+2] = 0;
        B[i*2] = dst[i].x;
        B[i*2+1] = dst[i].y;
    }
    cvSolve(&mA, &mB, &mX);

    mX = cvMat1(2, 3, CC_64F, 1, x);
    cvConvert(&mX, map_matrix);

	__END__;
    return map_matrix;
}

/****************************************************************************************\
*                          Generic Geometric Transformation: Remap                       *
\****************************************************************************************/

#define  ICC_DEF_REMAP_BILINEAR_FUNC(flavor, arrtype, load_macro, cast_macro) \
static CStatus C_STDCALL                                                      \
icvRemap_Bilinear_##flavor##_CnR(const arrtype* src, int srcstep, CSize ssize,\
                         arrtype* dst, int dststep, CSize dsize,           \
                         const float* mapx, int mxstep,                     \
                         const float* mapy, int mystep,                     \
                         int cn, const arrtype* fillval)                   \
{                                                                           \
    int i, j, k;                                                            \
    ssize.width--;                                                          \
    ssize.height--;                                                         \
                                                                            \
    srcstep /= sizeof(src[0]);                                              \
    dststep /= sizeof(dst[0]);                                              \
    mxstep /= sizeof(mapx[0]);                                              \
    mystep /= sizeof(mapy[0]);                                              \
                                                                            \
    for(i = 0; i < dsize.height; i++, dst += dststep,                      \
                                  mapx += mxstep, mapy += mystep)          \
    {                                                                       \
        for(j = 0; j < dsize.width; j++)                                  \
        {                                                                   \
            int ix = cRound(mapx[j]*(1 << ICC_WARP_SHIFT));                \
            int iy = cRound(mapy[j]*(1 << ICC_WARP_SHIFT));                \
            int ifx = ix & ICC_WARP_MASK;                                   \
            int ify = iy & ICC_WARP_MASK;                                   \
            ix >>= ICC_WARP_SHIFT;                                          \
            iy >>= ICC_WARP_SHIFT;                                          \
                                                                            \
            float x0 = icvLinearCoeffs[ifx*2];                              \
            float x1 = icvLinearCoeffs[ifx*2 + 1];                          \
            float y0 = icvLinearCoeffs[ify*2];                              \
            float y1 = icvLinearCoeffs[ify*2 + 1];                          \
                                                                            \
            if((unsigned)ix < (unsigned)ssize.width &&                     \
                (unsigned)iy < (unsigned)ssize.height)                     \
            {                                                               \
                const arrtype* s = src + iy*srcstep + ix*cn;                \
                for(k = 0; k < cn; k++, s++)                              \
                {                                                           \
                    float t0 = x1*load_macro(s[0]) + x0*load_macro(s[cn]);  \
                    float t1 = x1*load_macro(s[srcstep]) +                  \
                               x0*load_macro(s[srcstep + cn]);              \
                    dst[j*cn + k] = (arrtype)cast_macro(y1*t0 + y0*t1);     \
                }                                                           \
            }                                                               \
            else if(fillval)                                              \
                for(k = 0; k < cn; k++)                                   \
                    dst[j*cn + k] = fillval[k];                             \
        }                                                                   \
    }                                                                       \
                                                                            \
    return CC_OK;                                                           \
}


#define  ICC_DEF_REMAP_BICUBIC_FUNC(flavor, arrtype, worktype,                 \
                                     load_macro, cast_macro1, cast_macro2)     \
static CStatus C_STDCALL                                                      \
icvRemap_Bicubic_##flavor##_CnR(const arrtype* src, int srcstep, CSize ssize, \
                         arrtype* dst, int dststep, CSize dsize,               \
                         const float* mapx, int mxstep,                         \
                         const float* mapy, int mystep,                         \
                         int cn, const arrtype* fillval)                       \
{                                                                               \
    int i, j, k;                                                                \
    ssize.width = MAX(ssize.width - 3, 0);                                    \
    ssize.height = MAX(ssize.height - 3, 0);                                  \
                                                                                \
    srcstep /= sizeof(src[0]);                                                  \
    dststep /= sizeof(dst[0]);                                                  \
    mxstep /= sizeof(mapx[0]);                                                  \
    mystep /= sizeof(mapy[0]);                                                  \
                                                                                \
    for(i = 0; i < dsize.height; i++, dst += dststep,                          \
                                  mapx += mxstep, mapy += mystep)              \
    {                                                                           \
        for(j = 0; j < dsize.width; j++)                                      \
        {                                                                       \
            int ix = cRound(mapx[j]*(1 << ICC_WARP_SHIFT));                    \
            int iy = cRound(mapy[j]*(1 << ICC_WARP_SHIFT));                    \
            int ifx = ix & ICC_WARP_MASK;                                       \
            int ify = iy & ICC_WARP_MASK;                                       \
            ix >>= ICC_WARP_SHIFT;                                              \
            iy >>= ICC_WARP_SHIFT;                                              \
                                                                                \
            if((unsigned)(ix-1) < (unsigned)ssize.width &&                     \
                (unsigned)(iy-1) < (unsigned)ssize.height)                     \
            {                                                                   \
                for(k = 0; k < cn; k++)                                       \
                {                                                               \
                    const arrtype* s = src + (iy-1)*srcstep + ix*cn + k;        \
                                                                                \
                    float t0 = load_macro(s[-cn])*icvCubicCoeffs[ifx*2 + 1] +   \
                               load_macro(s[0])*icvCubicCoeffs[ifx*2] +         \
                               load_macro(s[cn])*icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE-ifx)*2] +\
                               load_macro(s[cn*2])*icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE-ifx)*2+1];\
                                                                                \
                    s += srcstep;                                               \
                                                                                \
                    float t1 = load_macro(s[-cn])*icvCubicCoeffs[ifx*2 + 1] +   \
                               load_macro(s[0])*icvCubicCoeffs[ifx*2] +         \
                               load_macro(s[cn])*icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE-ifx)*2] +\
                               load_macro(s[cn*2])*icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE-ifx)*2+1];\
                                                                                \
                    s += srcstep;                                               \
                                                                                \
                    float t2 = load_macro(s[-cn])*icvCubicCoeffs[ifx*2 + 1] +   \
                               load_macro(s[0])*icvCubicCoeffs[ifx*2] +         \
                               load_macro(s[cn])*icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE-ifx)*2] +\
                               load_macro(s[cn*2])*icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE-ifx)*2+1];\
                                                                                \
                    s += srcstep;                                               \
                                                                                \
                    float t3 = load_macro(s[-cn])*icvCubicCoeffs[ifx*2 + 1] +   \
                               load_macro(s[0])*icvCubicCoeffs[ifx*2] +         \
                               load_macro(s[cn])*icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE-ifx)*2] +\
                               load_macro(s[cn*2])*icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE-ifx)*2+1];\
                                                                                \
                    worktype t = cast_macro1(t0*icvCubicCoeffs[ify*2 + 1] +    \
                               t1*icvCubicCoeffs[ify*2] +                       \
                               t2*icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE-ify)*2] +  \
                               t3*icvCubicCoeffs[(ICC_CUBIC_TAB_SIZE-ify)*2+1]);\
                                                                                \
                    dst[j*cn + k] = cast_macro2(t);                             \
                }                                                               \
            }                                                                   \
            else if(fillval)                                                  \
                for(k = 0; k < cn; k++)                                       \
                    dst[j*cn + k] = fillval[k];                                 \
        }                                                                       \
    }                                                                           \
                                                                                \
    return CC_OK;                                                               \
}


ICC_DEF_REMAP_BILINEAR_FUNC(8u, uchar, CC_8TO32F, cRound)
ICC_DEF_REMAP_BILINEAR_FUNC(16u, ushort, CC_NOP, cRound)
ICC_DEF_REMAP_BILINEAR_FUNC(32f, float, CC_NOP, CC_NOP)

ICC_DEF_REMAP_BICUBIC_FUNC(8u, uchar, int, CC_8TO32F, cRound, CC_FAST_CAST_8U)
ICC_DEF_REMAP_BICUBIC_FUNC(16u, ushort, int, CC_NOP, cRound, CC_CAST_16U)
ICC_DEF_REMAP_BICUBIC_FUNC(32f, float, float, CC_NOP, CC_NOP, CC_NOP)

typedef CStatus (C_STDCALL * CvRemapFunc)(
    const void* src, int srcstep, CSize ssize,
    void* dst, int dststep, CSize dsize,
    const float* mapx, int mxstep,
    const float* mapy, int mystep,
    int cn, const void* fillval);

static void icvInitRemapTab(CvFuncTable  bilinear_tab, CvFuncTable  bicubic_tab)
{
    bilinear_tab[CC_8U] = (void*)icvRemap_Bilinear_8u_CnR;
    bilinear_tab[CC_16U] = (void*)icvRemap_Bilinear_16u_CnR;
    bilinear_tab[CC_32F] = (void*)icvRemap_Bilinear_32f_CnR;

    bicubic_tab[CC_8U] = (void*)icvRemap_Bicubic_8u_CnR;
    bicubic_tab[CC_16U] = (void*)icvRemap_Bicubic_16u_CnR;
    bicubic_tab[CC_32F] = (void*)icvRemap_Bicubic_32f_CnR;
}


/******************** IPP remap functions *********************/

typedef CStatus (C_STDCALL * CvRemapIPPFunc)(
    const void* src, CSize srcsize, int srcstep, CRect srcroi,
    const float* xmap, int xmapstep, const float* ymap, int ymapstep,
    void* dst, int dststep, CSize dstsize, int interpolation); 

/**************************************************************/

CC_IMPL void
cvRemap(const img_t* src, img_t* dst,
         const img_t* mapx, const img_t* mapy,
         int flags CC_DEFAULT(CC_INTER_LINEAR+CC_WARP_FILL_OUTLIERS),
         CScalar fillval CC_DEFAULT(cScalarAll(0)))
{
    static CvFuncTable bilinear_tab;
    static CvFuncTable bicubic_tab;
    static int inittab = 0;

    CC_FUNCNAME("cvRemap");

    __BEGIN__;

    TypeId type;
    int cn;
    int method = flags & 3;
    double fillbuf[4];
    CSize ssize, dsize;

    if(!inittab)
    {
        icvInitRemapTab(bilinear_tab, bicubic_tab);
        icvInitLinearCoeffTab();
        icvInitCubicCoeffTab();
        inittab = 1;
    }

    if(!CC_ARE_TYPES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(!CC_ARE_TYPES_EQ(mapx, mapy) || CC_MAT_TYPE(mapx) != CC_32F)
        CC_ERROR(CC_StsUnmatchedFormats, "Both map arrays must have 32fC1 type");

    if(!CC_ARE_SIZES_EQ(mapx, mapy) || !CC_ARE_SIZES_EQ(mapx, dst))
        CC_ERROR(CC_StsUnmatchedSizes,
        "Both map arrays and the destination array must have the same size");

    type = CC_MAT_DEPTH(src);
    
    cn = CC_MAT_CN(src);
    if(cn > 4)
        CC_ERROR(CC_BadNumChannels, "");
    
    ssize = cvGetMatSize(src);
    dsize = cvGetMatSize(dst);
    
    cvScalarToRawData(&fillval, fillbuf, CC_MAT_DEPTH(src), CC_MAT_CN(src), 0);

    {
        CvRemapFunc func = method == CC_INTER_CUBIC ?
            (CvRemapFunc)bicubic_tab[type] :
            (CvRemapFunc)bilinear_tab[type];

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        func(src->tt.data, src->step, ssize, dst->tt.data, dst->step, dsize,
              mapx->tt.fl, mapx->step, mapy->tt.fl, mapy->step,
              cn, flags & CC_WARP_FILL_OUTLIERS ? fillbuf : 0);
    }

    __END__;
}


/****************************************************************************************\
*                                   Log-Polar Transform                                  *
\****************************************************************************************/

/* now it is done via Remap; more correct implementation should use
   some super-sampling technique outside of the "fovea" circle */
CC_IMPL void
cvLogPolar(const img_t* src, img_t* dst, CPoint2D32f center, double M, int flags)
{
    img_t mapx[1] = {0};
    img_t mapy[1] = {0};
    double* exp_tab = 0;
    float* buf = 0;
    
    CC_FUNCNAME("cvLogPolar");

    __BEGIN__;
    
    CSize ssize, dsize;

    if(!CC_ARE_TYPES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(M <= 0)
        CC_ERROR(CC_StsOutOfRange, "M should be >0");

    ssize = cvGetMatSize(src);
    dsize = cvGetMatSize(dst);

    CC_CALL(cvCreateMat(mapx, dsize.height, dsize.width, CC_32F));
    CC_CALL(cvCreateMat(mapy, dsize.height, dsize.width, CC_32F));

    if(!(flags & CC_WARP_INVERSE_MAP))
    {
        int phi, rho;
        
        CC_CALL(exp_tab = (double*)cAlloc(dsize.width*sizeof(exp_tab[0])));

        for(rho = 0; rho < dst->width; rho++)
            exp_tab[rho] = exp(rho/M);
    
        for(phi = 0; phi < dsize.height; phi++)
        {
            double cp = cos(phi*2*CC_PI/dsize.height);
            double sp = sin(phi*2*CC_PI/dsize.height);
            float* mx = (float*)(mapx->tt.data + phi*mapx->step);
            float* my = (float*)(mapy->tt.data + phi*mapy->step);

            for(rho = 0; rho < dsize.width; rho++)
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
        int x, y;
        img_t bufx, bufy, bufp, bufa;
        double ascale = (ssize.width-1)/(2*CC_PI);

        CC_CALL(buf = (float*)cAlloc(4*dsize.width*sizeof(buf[0])));

        bufx = cvMat1(1, dsize.width, CC_32F, 1, buf);
        bufy = cvMat1(1, dsize.width, CC_32F, 1, buf + dsize.width);
        bufp = cvMat1(1, dsize.width, CC_32F, 1, buf + dsize.width*2);
        bufa = cvMat1(1, dsize.width, CC_32F, 1, buf + dsize.width*3);

        for(x = 0; x < dsize.width; x++)
            bufx.tt.fl[x] = (float)x - center.x;

        for(y = 0; y < dsize.height; y++)
        {
            float* mx = (float*)(mapx->tt.data + y*mapx->step);
            float* my = (float*)(mapy->tt.data + y*mapy->step);
            
            for(x = 0; x < dsize.width; x++)
                bufy.tt.fl[x] = (float)y - center.y;

#if 1
            cvCartToPolar(&bufx, &bufy, &bufp, &bufa);

            for(x = 0; x < dsize.width; x++)
                bufp.tt.fl[x] += 1.f;

            cvLog(&bufp, &bufp);
            
            for(x = 0; x < dsize.width; x++)
            {
                double rho = bufp.tt.fl[x]*M;
                double phi = bufa.tt.fl[x]*ascale;

                mx[x] = (float)rho;
                my[x] = (float)phi;
            }
#else
            for(x = 0; x < dsize.width; x++)
            {
                double xx = bufx.tt.fl[x];
                double yy = bufy.tt.fl[x];

                double p = log(sqrt(xx*xx + yy*yy) + 1.)*M;
                double a = atan2(yy,xx);
                if(a < 0)
                    a = 2*CC_PI + a;
                a *= ascale;

                mx[x] = (float)p;
                my[x] = (float)a;
            }
#endif
        }
    }

    cvRemap(src, dst, mapx, mapy, flags, cScalarAll(0));

    __END__;

    cFree(&exp_tab);
    cFree(&buf);
    imfree(mapx);
    imfree(mapy);
}


