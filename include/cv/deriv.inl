

/****************************************************************************************/

/* lightweight convolution with 3x3 kernel */
static void icvSepConvSmall3_32f(float* src, int src_step, float* dst, int dst_step,
            CSize src_size, const float* kx, const float* ky, float* buffer)
{
    int  dst_width, buffer_step = 0;
    int  x, y;

    assert(src && dst && src_size.width > 2 && src_size.height > 2 &&
            (src_step & 3) == 0 && (dst_step & 3) == 0 &&
            (kx || ky) && (buffer || !kx || !ky));

    src_step /= sizeof(src[0]);
    dst_step /= sizeof(dst[0]);

    dst_width = src_size.width - 2;

    if(!kx)
    {
        /* set vars, so that vertical convolution
           will write results into destination ROI and
           horizontal convolution won't run */
        src_size.width = dst_width;
        buffer_step = dst_step;
        buffer = dst;
        dst_width = 0;
    }

    assert(src_step >= src_size.width && dst_step >= dst_width);

    src_size.height -= 3;
    if(!ky)
    {
        /* set vars, so that vertical convolution won't run and
           horizontal convolution will write results into destination ROI */
        src_size.height += 3;
        buffer_step = src_step;
        buffer = src;
        src_size.width = 0;
    }

    for(y = 0; y <= src_size.height; y++, src += src_step,
                                           dst += dst_step,
                                           buffer += buffer_step)
    {
        float* src2 = src + src_step;
        float* src3 = src + src_step*2;
        for(x = 0; x < src_size.width; x++)
        {
            buffer[x] = (float)(ky[0]*src[x] + ky[1]*src2[x] + ky[2]*src3[x]);
        }

        for(x = 0; x < dst_width; x++)
        {
            dst[x] = (float)(kx[0]*buffer[x] + kx[1]*buffer[x+1] + kx[2]*buffer[x+2]);
        }
    }
}


/****************************************************************************************\
                             Sobel & Scharr Derivative Filters
\****************************************************************************************/

/////////////////////////////// Old IPP derivative filters ///////////////////////////////
// still used in corner detectors (see cvcorner.cpp)


//////////////////////////////////////////////////////////////////////////////////////////

CC_IMPL void
cvSobel(const img_t* src, img_t* dst, int dx, int dy, int aperture_size)
{
    CSepFilter filter;
    void* buffer = 0;

    CC_FUNCNAME("cvSobel");

    __BEGIN__;

    int origin = 0;
    TypeId src_type, dst_type;
    int src_cn, dst_cn;
    
    src_type = CC_MAT_DEPTH(src);
    dst_type = CC_MAT_DEPTH(dst);
    src_cn = CC_MAT_CN(src);
    dst_cn = CC_MAT_CN(dst);

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR(CC_StsBadArg, "src and dst have different sizes");

    CC_CALL(filter.init_deriv(src->cols, src_type, src_cn, dst_type, dst_cn, dx, dy,
                aperture_size, origin ? CSepFilter::FLIP_KERNEL : 0));
    CC_CALL(filter.process(src, dst));

    __END__;

    if(buffer)
        cFree(&buffer);
}


/****************************************************************************************\
                                     Laplacian Filter
\****************************************************************************************/

static void icvLaplaceRow_8u32s(const uchar* src, int* dst, void* params);
static void icvLaplaceRow_8u32f(const uchar* src, float* dst, void* params);
static void icvLaplaceRow_32f(const float* src, float* dst, void* params);
static void icvLaplaceCol_32s16s(const int** src, short* dst, int dst_step,
                                  int count, void* params);
static void icvLaplaceCol_32f(const float** src, float* dst, int dst_step,
                               int count, void* params);

CLaplaceFilter::CLaplaceFilter()
{
    normalized = basic_laplacian = false;
}


CLaplaceFilter::CLaplaceFilter(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn, bool _normalized,
                                  int _ksize, CBorderTypes _border_mode, CScalar _border_value)
{
    normalized = basic_laplacian = false;
    init(_max_width, _src_type, _src_cn, _dst_type, _dst_cn, _normalized, _ksize, _border_mode, _border_value);
}


CLaplaceFilter::~CLaplaceFilter()
{
    clear();
}


void CLaplaceFilter::get_work_params()
{
    int min_rows = max_ky*2 + 3, rows = MAX(min_rows,10), row_sz;
    int width = max_width, trow_sz = 0;
    //TypeId dst_type = dst_type;
    TypeId work_type = dst_type < CC_32F ? CC_32S : CC_32F;
    work_type = work_type;
    work_cn = dst_cn*2;
    trow_sz = cvAlign((max_width + ksize.width - 1)*CC_TYPE_SIZE(src_type), ALIGN);
    row_sz = cvAlign(width*CC_TYPE_SIZE(work_type), ALIGN);
    buf_size = rows*row_sz;
    buf_size = MIN(buf_size, 1 << 16);
    buf_size = MAX(buf_size, min_rows*row_sz);
    max_rows = (buf_size/row_sz)*3 + max_ky*2 + 8;
    buf_size += trow_sz;
}


void CLaplaceFilter::init(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn, 
                            bool _normalized, int _ksize0, CBorderTypes _border_mode, CScalar _border_value)
{
    img_t kx[1] = {0}, ky[1] = {0};

    CC_FUNCNAME("CLaplaceFilter::init");

    __BEGIN__;

    TypeId src_type = _src_type, dst_type = _dst_type;
    int _ksize = MAX(_ksize0, 3);

    normalized = _normalized;
    basic_laplacian = _ksize0 == 1;

    if((src_type != CC_8U || dst_type != CC_16S && dst_type != CC_32F) &&
        (src_type != CC_32F || dst_type != CC_32F) ||
        _src_cn != _dst_cn)
        CC_ERROR(CC_StsUnmatchedFormats,
        "Laplacian can either transform 8u->16s, or 8u->32f, or 32f->32f.\n"
        "The channel number must be the same.");

    if(_ksize < 1 || _ksize > CC_MAX_SOBEL_KSIZE || _ksize % 2 == 0)
        CC_ERROR(CC_StsOutOfRange, "kernel size must be within 1..7 and odd");

    CC_CALL(cvCreateMat(kx, 1, _ksize, CC_32F));
    CC_CALL(cvCreateMat(ky, 1, _ksize, CC_32F));

    CSepFilter::init_sobel_kernel(kx, ky, 2, 0, 0);
    CSepFilter::init(_max_width, _src_type, _src_cn, _dst_type, _dst_cn, kx, ky,
                       cPoint(-1,-1), _border_mode, _border_value);

    x_func = 0;
    y_func = 0;

    if(src_type == CC_8U)
    {
        if(dst_type == CC_16S)
        {
            x_func = (CvRowFilterFunc)icvLaplaceRow_8u32s;
            y_func = (CvColumnFilterFunc)icvLaplaceCol_32s16s;
        }
        else if(dst_type == CC_32F)
        {
            x_func = (CvRowFilterFunc)icvLaplaceRow_8u32f;
            y_func = (CvColumnFilterFunc)icvLaplaceCol_32f;
        }
    }
    else if(src_type == CC_32F)
    {
        if(dst_type == CC_32F)
        {
            x_func = (CvRowFilterFunc)icvLaplaceRow_32f;
            y_func = (CvColumnFilterFunc)icvLaplaceCol_32f;
        }
    }

    if(!x_func || !y_func)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    __END__;

    imfree(kx);
    imfree(ky);
}


void CLaplaceFilter::init(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                            bool _is_separable, CSize _ksize,
                            CPoint _anchor, CBorderTypes _border_mode,
                            CScalar _border_value)
{
    CSepFilter::init(_max_width, _src_type, _src_cn, _dst_type, _dst_cn, _is_separable,
                       _ksize, _anchor, _border_mode, _border_value);
}


void CLaplaceFilter::init(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                            const img_t* _kx, const img_t* _ky,
                            CPoint _anchor, CBorderTypes _border_mode,
                            CScalar _border_value)
{
    CSepFilter::init(_max_width, _src_type, _src_cn, _dst_type, _dst_cn, _kx, _ky,
                       _anchor, _border_mode, _border_value);
}


#define ICC_LAPLACE_ROW(flavor, srctype, dsttype, load_macro)         \
static void                                                             \
icvLaplaceRow_##flavor(const srctype* src, dsttype* dst, void* params)\
{                                                                       \
    const CLaplaceFilter* state = (const CLaplaceFilter*)params;      \
    const img_t* _kx = state->get_x_kernel();                           \
    const img_t* _ky = state->get_y_kernel();                           \
    const dsttype* kx = (dsttype*)_kx->tt.data;                        \
    const dsttype* ky = (dsttype*)_ky->tt.data;                        \
    int ksize = _kx->cols + _kx->rows - 1;                              \
    int i = 0, j, k, width = state->get_width();                        \
    int cn = state->get_src_cn();                          \
    int ksize2 = ksize/2, ksize2n = ksize2*cn;                          \
    const srctype* s = src + ksize2n;                                   \
    bool basic_laplacian = state->is_basic_laplacian();                 \
                                                                        \
    kx += ksize2;                                                       \
    ky += ksize2;                                                       \
    width *= cn;                                                        \
                                                                        \
    if(basic_laplacian)                                               \
        for(i = 0; i < width; i++)                                    \
        {                                                               \
            dsttype s0 = load_macro(s[i]);                              \
            dsttype s1 = (dsttype)(s[i-cn] - s0*2 + s[i+cn]);           \
            dst[i] = s0; dst[i+width] = s1;                             \
        }                                                               \
    else if(ksize == 3)                                               \
        for(i = 0; i < width; i++)                                    \
        {                                                               \
            dsttype s0 = (dsttype)(s[i-cn] + s[i]*2 + s[i+cn]);         \
            dsttype s1 = (dsttype)(s[i-cn] - s[i]*2 + s[i+cn]);         \
            dst[i] = s0; dst[i+width] = s1;                             \
        }                                                               \
    else if(ksize == 5)                                               \
        for(i = 0; i < width; i++)                                    \
        {                                                               \
            dsttype s0 = (dsttype)(s[i-2*cn]+(s[i-cn]+s[i+cn])*4+s[i]*6+s[i+2*cn]);\
            dsttype s1 = (dsttype)(s[i-2*cn]-s[i]*2+s[i+2*cn]);         \
            dst[i] = s0; dst[i+width] = s1;                             \
        }                                                               \
    else                                                                \
        for(i = 0; i < width; i++, s++)                               \
        {                                                               \
            dsttype s0 = ky[0]*load_macro(s[0]), s1 = kx[0]*load_macro(s[0]);\
            for(k = 1, j = cn; k <= ksize2; k++, j += cn)             \
            {                                                           \
                dsttype t = load_macro(s[j] + s[-j]);                   \
                s0 += ky[k]*t; s1 += kx[k]*t;                           \
            }                                                           \
            dst[i] = s0; dst[i+width] = s1;                             \
        }                                                               \
}

ICC_LAPLACE_ROW(8u32s, uchar, int, CC_NOP)
ICC_LAPLACE_ROW(8u32f, uchar, float, CC_8TO32F)
ICC_LAPLACE_ROW(32f, float, float, CC_NOP)

static void
icvLaplaceCol_32s16s(const int** src, short* dst,
                      int dst_step, int count, void* params)
{
    const CLaplaceFilter* state = (const CLaplaceFilter*)params;
    const img_t* _kx = state->get_x_kernel();
    const img_t* _ky = state->get_y_kernel();
    const int* kx = (const int*)_kx->tt.data;
    const int* ky = (const int*)_ky->tt.data;
    int ksize = _kx->cols + _kx->rows - 1, ksize2 = ksize/2;
    int i = 0, k, width = state->get_width();
    int cn = state->get_src_cn();
    bool basic_laplacian = state->is_basic_laplacian();
    bool normalized = state->is_normalized();
    int shift = ksize - 1, delta = (1 << shift) >> 1;
    
    width *= cn;
    src += ksize2;
    kx += ksize2;
    ky += ksize2;
    dst_step /= sizeof(dst[0]);

    if(basic_laplacian || !normalized)
    {
        normalized = false;
        shift = delta = 0;
    }

    for(; count--; dst += dst_step, src++)
    {
        if(ksize == 3)
        {
            const int *src0 = src[-1], *src1 = src[0], *src2 = src[1];
            if(basic_laplacian)
            {
                for(i = 0; i <= width - 2; i += 2)
                {
                    int s0 = src0[i] - src1[i]*2 + src2[i] + src1[i+width];
                    int s1 = src0[i+1] - src1[i+1]*2 + src2[i+1] + src1[i+width+1];
                    dst[i] = (short)s0; dst[i+1] = (short)s1;
                }

                for(; i < width; i++)
                    dst[i] = (short)(src0[i] - src1[i]*2 + src2[i] + src1[i+width]);
            }
            else if(!normalized)
                for(i = 0; i <= width - 2; i += 2)
                {
                    int s0 = src0[i] - src1[i]*2 + src2[i] +
                             src0[i+width] + src1[i+width]*2 + src2[i+width];
                    int s1 = src0[i+1] - src1[i+1]*2 + src2[i+1] +
                             src0[i+width+1] + src1[i+width+1]*2 + src2[i+width+1];
                    dst[i] = (short)s0; dst[i+1] = (short)s1;
                }
            else
                for(i = 0; i <= width - 2; i += 2)
                {
                    int s0 = CC_DESCALE(src0[i] - src1[i]*2 + src2[i] +
                             src0[i+width] + src1[i+width]*2 + src2[i+width], 2);
                    int s1 = CC_DESCALE(src0[i+1] - src1[i+1]*2 + src2[i+1] +
                             src0[i+width+1] + src1[i+width+1]*2 + src2[i+width+1],2);
                    dst[i] = (short)s0; dst[i+1] = (short)s1;
                }
        }
        else if(ksize == 5)
        {
            const int *src0 = src[-2], *src1 = src[-1], *src2 = src[0], *src3 = src[1], *src4 = src[2];

            if(!normalized)
                for(i = 0; i <= width - 2; i += 2)
                {
                    int s0 = src0[i] - src2[i]*2 + src4[i] + src0[i+width] + src4[i+width] +
                             (src1[i+width] + src3[i+width])*4 + src2[i+width]*6;
                    int s1 = src0[i+1] - src2[i+1]*2 + src4[i+1] + src0[i+width+1] +
                             src4[i+width+1] + (src1[i+width+1] + src3[i+width+1])*4 +
                             src2[i+width+1]*6;
                    dst[i] = (short)s0; dst[i+1] = (short)s1;
                }
            else
                for(i = 0; i <= width - 2; i += 2)
                {
                    int s0 = CC_DESCALE(src0[i] - src2[i]*2 + src4[i] +
                             src0[i+width] + src4[i+width] +
                             (src1[i+width] + src3[i+width])*4 + src2[i+width]*6, 4);
                    int s1 = CC_DESCALE(src0[i+1] - src2[i+1]*2 + src4[i+1] +
                             src0[i+width+1] + src4[i+width+1] +
                             (src1[i+width+1] + src3[i+width+1])*4 + src2[i+width+1]*6, 4);
                    dst[i] = (short)s0; dst[i+1] = (short)s1;
                }
        }
        else
        {
            if(!normalized)
                for(i = 0; i <= width - 2; i += 2)
                {
                    int s0 = kx[0]*src[0][i] + ky[0]*src[0][i+width];
                    int s1 = kx[0]*src[0][i+1] + ky[0]*src[0][i+width+1];

                    for(k = 1; k <= ksize2; k++)
                    {
                        const int* src1 = src[k] + i, *src2 = src[-k] + i;
                        int fx = kx[k], fy = ky[k];
                        s0 += fx*(src1[0] + src2[0]) + fy*(src1[width] + src2[width]);
                        s1 += fx*(src1[1] + src2[1]) + fy*(src1[width+1] + src2[width+1]);
                    }

                    dst[i] = CC_CAST_16S(s0); dst[i+1] = CC_CAST_16S(s1);
                }
            else
                for(i = 0; i <= width - 2; i += 2)
                {
                    int s0 = kx[0]*src[0][i] + ky[0]*src[0][i+width];
                    int s1 = kx[0]*src[0][i+1] + ky[0]*src[0][i+width+1];

                    for(k = 1; k <= ksize2; k++)
                    {
                        const int* src1 = src[k] + i, *src2 = src[-k] + i;
                        int fx = kx[k], fy = ky[k];
                        s0 += fx*(src1[0] + src2[0]) + fy*(src1[width] + src2[width]);
                        s1 += fx*(src1[1] + src2[1]) + fy*(src1[width+1] + src2[width+1]);
                    }

                    s0 = CC_DESCALE(s0, shift); s1 = CC_DESCALE(s1, shift);
                    dst[i] = (short)s0; dst[i+1] = (short)s1;
                }
        }

        for(; i < width; i++)
        {
            int s0 = kx[0]*src[0][i] + ky[0]*src[0][i+width];
            for(k = 1; k <= ksize2; k++)
            {
                const int* src1 = src[k] + i, *src2 = src[-k] + i;
                s0 += kx[k]*(src1[0] + src2[0]) + ky[k]*(src1[width] + src2[width]);
            }
            s0 = (s0 + delta) >> shift;
            dst[i] = CC_CAST_16S(s0);
        }
    }
}


static void
icvLaplaceCol_32f(const float** src, float* dst,
                   int dst_step, int count, void* params)
{
    const CLaplaceFilter* state = (const CLaplaceFilter*)params;
    const img_t* _kx = state->get_x_kernel();
    const img_t* _ky = state->get_y_kernel();
    const float* kx = (const float*)_kx->tt.data;
    const float* ky = (const float*)_ky->tt.data;
    int ksize = _kx->cols + _kx->rows - 1, ksize2 = ksize/2;
    int i = 0, k, width = state->get_width();
    int cn = state->get_src_cn();
    bool basic_laplacian = state->is_basic_laplacian();
    bool normalized = state->is_normalized();
    float scale = 1.f/(1 << (ksize - 1));
    
    width *= cn;
    src += ksize2;
    kx += ksize2;
    ky += ksize2;
    dst_step /= sizeof(dst[0]);

    if(basic_laplacian || !normalized)
    {
        normalized = false;
        scale = 1.f;
    }

    for(; count--; dst += dst_step, src++)
    {
        if(ksize == 3)
        {
            const float *src0 = src[-1], *src1 = src[0], *src2 = src[1];
            if(basic_laplacian)
            {
                for(i = 0; i <= width - 2; i += 2)
                {
                    float s0 = src0[i] - src1[i]*2 + src2[i] + src1[i+width];
                    float s1 = src0[i+1] - src1[i+1]*2 + src2[i+1] + src1[i+width+1];
                    dst[i] = s0; dst[i+1] = s1;
                }

                for(; i < width; i++)
                    dst[i] = src0[i] - src1[i]*2 + src2[i] + src1[i+width];
            }
            else if(!normalized)
                for(i = 0; i <= width - 2; i += 2)
                {
                    float s0 = src0[i] - src1[i]*2 + src2[i] +
                             src0[i+width] + src1[i+width]*2 + src2[i+width];
                    float s1 = src0[i+1] - src1[i+1]*2 + src2[i+1] +
                             src0[i+width+1] + src1[i+width+1]*2 + src2[i+width+1];
                    dst[i] = s0; dst[i+1] = s1;
                }
            else
                for(i = 0; i <= width - 2; i += 2)
                {
                    float s0 = (src0[i] - src1[i]*2 + src2[i] +
                             src0[i+width] + src1[i+width]*2 + src2[i+width])*scale;
                    float s1 = (src0[i+1] - src1[i+1]*2 + src2[i+1] +
                             src0[i+width+1] + src1[i+width+1]*2 + src2[i+width+1])*scale;
                    dst[i] = s0; dst[i+1] = s1;
                }
        }
        else if(ksize == 5)
        {
            const float *src0 = src[-2], *src1 = src[-1], *src2 = src[0], *src3 = src[1], *src4 = src[2];
            for(i = 0; i <= width - 2; i += 2)
            {
                float s0 = (src0[i] - src2[i]*2 + src4[i] +
                         src0[i+width] + src4[i+width] +
                         (src1[i+width] + src3[i+width])*4 + src2[i+width]*6)*scale;
                float s1 = (src0[i+1] - src2[i+1]*2 + src4[i+1] +
                         src0[i+width+1] + src4[i+width+1] +
                         (src1[i+width+1] + src3[i+width+1])*4 + src2[i+width+1]*6)*scale;
                dst[i] = s0; dst[i+1] = s1;
            }
        }
        else
        {
            for(i = 0; i <= width - 2; i += 2)
            {
                float s0 = kx[0]*src[0][i] + ky[0]*src[0][i+width];
                float s1 = kx[0]*src[0][i+1] + ky[0]*src[0][i+width+1];

                for(k = 1; k <= ksize2; k++)
                {
                    const float* src1 = src[k] + i, *src2 = src[-k] + i;
                    float fx = kx[k], fy = ky[k];
                    s0 += fx*(src1[0] + src2[0]) + fy*(src1[width] + src2[width]);
                    s1 += fx*(src1[1] + src2[1]) + fy*(src1[width+1] + src2[width+1]);
                }

                s0 *= scale; s1 *= scale;
                dst[i] = s0; dst[i+1] = s1;
            }
        }

        for(; i < width; i++)
        {
            float s0 = kx[0]*src[0][i] + ky[0]*src[0][i+width];
            for(k = 1; k <= ksize2; k++)
            {
                const float* src1 = src[k] + i, *src2 = src[-k] + i;
                s0 += kx[k]*(src1[0] + src2[0]) + ky[k]*(src1[width] + src2[width]);
            }
            dst[i] = s0*scale;
        }
    }
}


CC_IMPL void
cvLaplace(const img_t* src, img_t* dst, int aperture_size)
{
    CLaplaceFilter laplacian;
    void* buffer = 0;
    int local_alloc = 0;
    
    CC_FUNCNAME("cvLaplace");

    __BEGIN__;
   
    TypeId src_type, dst_type;
    int src_cn, dst_cn;

    src_type = CC_MAT_DEPTH(src);
    dst_type = CC_MAT_DEPTH(dst);
    src_cn = CC_MAT_CN(src);
    dst_cn = CC_MAT_CN(dst);

    CC_CALL(laplacian.init(src->cols, src_type, src_cn, dst_type, dst_cn, false, aperture_size));
    CC_CALL(laplacian.process(src, dst));

    __END__;

    if(buffer && !local_alloc)
        cFree(&buffer);
}


