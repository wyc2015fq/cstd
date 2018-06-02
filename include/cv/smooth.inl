

/* Box filter ("all 1's", optionally normalized) filter. */
class CC_EXPORTS CBoxFilter : public CBaseImageFilter
{
public:
    CBoxFilter();
    CBoxFilter(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                 bool _normalized, CSize _ksize,
                 CPoint _anchor=cPoint(-1,-1),
                 CBorderTypes _border_mode=CC_BORDER_REPLICATE,
                 CScalar _border_value=cScalarAll(0));
    virtual void init(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                       bool _normalized, CSize _ksize,
                       CPoint _anchor=cPoint(-1,-1),
                       CBorderTypes _border_mode=CC_BORDER_REPLICATE,
                       CScalar _border_value=cScalarAll(0));

    virtual ~CBoxFilter();
    bool is_normalized() const { return normalized; }
    double get_scale() const { return scale; }
    uchar* get_sum_buf() { return sum; }
    int* get_sum_count_ptr() { return &sum_count; }

protected:
    virtual void start_process(CSlice x_range, int width);

    uchar* sum;
    int sum_count;
    bool normalized;
    double scale;
};


/* Laplacian operator: (d2/dx + d2/dy)I. */
class CC_EXPORTS CLaplaceFilter : public CSepFilter
{
public:
    CLaplaceFilter();
    CLaplaceFilter(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                     bool _normalized, int _ksize,
                     CBorderTypes _border_mode=CC_BORDER_REPLICATE,
                     CScalar _border_value=cScalarAll(0));
    virtual ~CLaplaceFilter();
    virtual void init(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                       bool _normalized, int _ksize,
                       CBorderTypes _border_mode=CC_BORDER_REPLICATE,
                       CScalar _border_value=cScalarAll(0));

    /* dummy methods to avoid compiler warnings */
    virtual void init(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                       bool _is_separable, CSize _ksize,
                       CPoint _anchor=cPoint(-1,-1),
                       CBorderTypes _border_mode=CC_BORDER_REPLICATE,
                       CScalar _border_value=cScalarAll(0));

    virtual void init(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                       const img_t* _kx, const img_t* _ky,
                       CPoint _anchor=cPoint(-1,-1),
                       CBorderTypes _border_mode=CC_BORDER_REPLICATE,
                       CScalar _border_value=cScalarAll(0));

    bool is_normalized() const { return normalized; }
    bool is_basic_laplacian() const { return basic_laplacian; }
protected:
    void get_work_params();

    bool basic_laplacian;
    bool normalized;
};


/* basic morphological operations: erosion & dilation */
class CC_EXPORTS CMorphology : public CBaseImageFilter
{
public:
    CMorphology();
    CMorphology(int _operation, int _max_width, int _src_dst_type,
                  int _element_shape, img_t* _element,
                  CSize _ksize=cSize(0,0), CPoint _anchor=cPoint(-1,-1),
                  CBorderTypes _border_mode=CC_BORDER_REPLICATE,
                  CScalar _border_value=cScalarAll(0));
    virtual ~CMorphology();
    virtual void init(int _operation, int _max_width, int _src_dst_type,
                       int _element_shape, img_t* _element,
                       CSize _ksize=cSize(0,0), CPoint _anchor=cPoint(-1,-1),
                       CBorderTypes _border_mode=CC_BORDER_REPLICATE,
                       CScalar _border_value=cScalarAll(0));

    /* dummy method to avoid compiler warnings */
    virtual void init(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                       bool _is_separable, CSize _ksize,
                       CPoint _anchor=cPoint(-1,-1),
                       CBorderTypes _border_mode=CC_BORDER_REPLICATE,
                       CScalar _border_value=cScalarAll(0));

    virtual void clear();
    const img_t* get_element() const { return element; }
    int get_element_shape() const { return el_shape; }
    int get_operation() const { return operation; }
    uchar* get_element_sparse_buf() { return el_sparse; }
    int get_element_sparse_count() const { return el_sparse_count; }

    enum { RECT=0, CROSS=1, ELLIPSE=2, CUSTOM=100, BINARY = 0, GRAYSCALE=256 };
    enum { ERODE=0, DILATE=1 };

    static void init_binary_element(img_t* _element, int _element_shape,
                                     CPoint _anchor=cPoint(-1,-1));
protected:

    void start_process(CSlice x_range, int width);
    int fill_cyclic_buffer(const uchar* src, int src_step,
                            int y0, int y1, int y2);
    uchar* el_sparse;
    int el_sparse_count;

    img_t *element;
    int el_shape;
    int operation;
};


/****************************************************************************************\
                                         Box Filter
\****************************************************************************************/

static void icvSumRow_8u32s(const uchar* src0, int* dst, void* params);
static void icvSumRow_32f64f(const float* src0, double* dst, void* params);
static void icvSumCol_32s8u(const int** src, uchar* dst, int dst_step,
                             int count, void* params);
static void icvSumCol_32s16s(const int** src, short* dst, int dst_step,
                             int count, void* params);
static void icvSumCol_32s32s(const int** src, int* dst, int dst_step,
                             int count, void* params);
static void icvSumCol_64f32f(const double** src, float* dst, int dst_step,
                              int count, void* params);

CBoxFilter::CBoxFilter()
{
    min_type = CC_32S;
    sum = 0;
    sum_count = 0;
    normalized = false;
}


CBoxFilter::CBoxFilter(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                          bool _normalized, CSize _ksize,
                          CPoint _anchor, CBorderTypes _border_mode,
                          CScalar _border_value)
{
    min_type = CC_32S;
    sum = 0;
    sum_count = 0;
    normalized = false;
    init(_max_width, _src_type, _src_cn, _dst_type, _dst_cn, _normalized,
          _ksize, _anchor, _border_mode, _border_value);
}


CBoxFilter::~CBoxFilter()
{
    clear();
}


void CBoxFilter::init(int _max_width, TypeId _src_type, int _src_cn, TypeId _dst_type, int _dst_cn,
                        bool _normalized, CSize _ksize,
                        CPoint _anchor, CBorderTypes _border_mode,
                        CScalar _border_value)
{
    CC_FUNCNAME("CBoxFilter::init");

    __BEGIN__;
    
    sum = 0;
    normalized = _normalized;

    if(normalized && _src_type != _dst_type ||
        !normalized && _src_cn != _dst_cn)
        CC_ERROR(CC_StsUnmatchedFormats,
        "In case of normalized box filter input and output must have the same type.\n"
        "In case of unnormalized box filter the number of input and output channels must be the same");

    min_type = _src_type == CC_8U ? CC_32S : CC_64F;

    CBaseImageFilter::init(_max_width, _src_type, _src_cn, _dst_type, _dst_cn, 1, _ksize,
                             _anchor, _border_mode, _border_value);
    
    scale = normalized ? 1./(ksize.width*ksize.height) : 1;

    if(src_type == CC_8U)
        x_func = (CvRowFilterFunc)icvSumRow_8u32s;
    else if(src_type == CC_32F)
        x_func = (CvRowFilterFunc)icvSumRow_32f64f;
    else
        CC_ERROR(CC_StsUnsupportedFormat, "Unknown/unsupported input image format");

    if(dst_type == CC_8U)
    {
        if(!normalized)
            CC_ERROR(CC_StsBadArg, "Only normalized box filter can be used for 8u->8u transformation");
        y_func = (CvColumnFilterFunc)icvSumCol_32s8u;
    }
    else if(dst_type == CC_16S)
    {
        if(normalized || src_type != CC_8U)
            CC_ERROR(CC_StsBadArg, "Only 8u->16s unnormalized box filter is supported in case of 16s output");
        y_func = (CvColumnFilterFunc)icvSumCol_32s16s;
    }
	else if(dst_type == CC_32S)
	{
		if(normalized || src_type != CC_8U)
			CC_ERROR(CC_StsBadArg, "Only 8u->32s unnormalized box filter is supported in case of 32s output");

		y_func = (CvColumnFilterFunc)icvSumCol_32s32s;
	}
    else if(dst_type == CC_32F)
    {
        if(src_type != CC_32F)
            CC_ERROR(CC_StsBadArg, "Only 32f->32f box filter (normalized or not) is supported in case of 32f output");
        y_func = (CvColumnFilterFunc)icvSumCol_64f32f;
    }
	else{
		CC_ERROR(CC_StsBadArg, "Unknown/unsupported destination image format");
	}

    __END__;
}


void CBoxFilter::start_process(CSlice x_range, int width)
{
    CBaseImageFilter::start_process(x_range, width);
    int i, psz = CC_TYPE_SIZE(work_type);
    uchar* s;
    buf_end -= buf_step;
    buf_max_count--;
    assert(buf_max_count >= max_ky*2 + 1);
    s = sum = buf_end + cvAlign((width + ksize.width - 1)*CC_TYPE_SIZE(src_type), ALIGN);
    sum_count = 0;

    width *= psz;
    for(i = 0; i < width; i++)
        s[i] = (uchar)0;
}


static void
icvSumRow_8u32s(const uchar* src, int* dst, void* params)
{
    const CBoxFilter* state = (const CBoxFilter*)params;
    int ksize = state->get_kernel_size().width;
    int width = state->get_width();
    int cn = state->get_src_cn();
    int i, k;

    width = (width - 1)*cn; ksize *= cn;

    for(k = 0; k < cn; k++, src++, dst++)
    {
        int s = 0;
        for(i = 0; i < ksize; i += cn)
            s += src[i];
        dst[0] = s;
        for(i = 0; i < width; i += cn)
        {
            s += src[i+ksize] - src[i];
            dst[i+cn] = s;
        }
    }
}


static void
icvSumRow_32f64f(const float* src, double* dst, void* params)
{
    const CBoxFilter* state = (const CBoxFilter*)params;
    int ksize = state->get_kernel_size().width;
    int width = state->get_width();
    int cn = state->get_src_cn();
    int i, k;

    width = (width - 1)*cn; ksize *= cn;

    for(k = 0; k < cn; k++, src++, dst++)
    {
        double s = 0;
        for(i = 0; i < ksize; i += cn)
            s += src[i];
        dst[0] = s;
        for(i = 0; i < width; i += cn)
        {
            s += (double)src[i+ksize] - src[i];
            dst[i+cn] = s;
        }
    }
}


static void
icvSumCol_32s8u(const int** src, uchar* dst,
                 int dst_step, int count, void* params)
{
#define BLUR_SHIFT 24
    CBoxFilter* state = (CBoxFilter*)params;
    int ksize = state->get_kernel_size().height;
    int i, width = state->get_width();
    int cn = state->get_src_cn();
    double scale = state->get_scale();
    int iscale = cFloor(scale*(1 << BLUR_SHIFT));
    int* sum = (int*)state->get_sum_buf();
    int* _sum_count = state->get_sum_count_ptr();
    int sum_count = *_sum_count;

    width *= cn;
    src += sum_count;
    count += ksize - 1 - sum_count;

    for(; count--; src++)
    {
        const int* sp = src[0];
        if(sum_count+1 < ksize)
        {
            for(i = 0; i <= width - 2; i += 2)
            {
                int s0 = sum[i] + sp[i], s1 = sum[i+1] + sp[i+1];
                sum[i] = s0; sum[i+1] = s1;
            }

            for(; i < width; i++)
                sum[i] += sp[i];

            sum_count++;
        }
        else
        {
            const int* sm = src[-ksize+1];
            for(i = 0; i <= width - 2; i += 2)
            {
                int s0 = sum[i] + sp[i], s1 = sum[i+1] + sp[i+1];
                int t0 = CC_DESCALE(s0*iscale, BLUR_SHIFT), t1 = CC_DESCALE(s1*iscale, BLUR_SHIFT);
                s0 -= sm[i]; s1 -= sm[i+1];
                sum[i] = s0; sum[i+1] = s1;
                dst[i] = (uchar)t0; dst[i+1] = (uchar)t1;
            }

            for(; i < width; i++)
            {
                int s0 = sum[i] + sp[i], t0 = CC_DESCALE(s0*iscale, BLUR_SHIFT);
                sum[i] = s0 - sm[i]; dst[i] = (uchar)t0;
            }
            dst += dst_step;
        }
    }

    *_sum_count = sum_count;
#undef BLUR_SHIFT
}


static void
icvSumCol_32s16s(const int** src, short* dst,
                  int dst_step, int count, void* params)
{
    CBoxFilter* state = (CBoxFilter*)params;
    int ksize = state->get_kernel_size().height;
    int ktotal = ksize*state->get_kernel_size().width;
    int i, width = state->get_width();
    int cn = state->get_src_cn();
    int* sum = (int*)state->get_sum_buf();
    int* _sum_count = state->get_sum_count_ptr();
    int sum_count = *_sum_count;

    dst_step /= sizeof(dst[0]);
    width *= cn;
    src += sum_count;
    count += ksize - 1 - sum_count;

    for(; count--; src++)
    {
        const int* sp = src[0];
        if(sum_count+1 < ksize)
        {
            for(i = 0; i <= width - 2; i += 2)
            {
                int s0 = sum[i] + sp[i], s1 = sum[i+1] + sp[i+1];
                sum[i] = s0; sum[i+1] = s1;
            }

            for(; i < width; i++)
                sum[i] += sp[i];

            sum_count++;
        }
        else if(ktotal < 128)
        {
            const int* sm = src[-ksize+1];
            for(i = 0; i <= width - 2; i += 2)
            {
                int s0 = sum[i] + sp[i], s1 = sum[i+1] + sp[i+1];
                dst[i] = (short)s0; dst[i+1] = (short)s1;
                s0 -= sm[i]; s1 -= sm[i+1];
                sum[i] = s0; sum[i+1] = s1;
            }

            for(; i < width; i++)
            {
                int s0 = sum[i] + sp[i];
                dst[i] = (short)s0;
                sum[i] = s0 - sm[i];
            }
            dst += dst_step;
        }
        else
        {
            const int* sm = src[-ksize+1];
            for(i = 0; i <= width - 2; i += 2)
            {
                int s0 = sum[i] + sp[i], s1 = sum[i+1] + sp[i+1];
                dst[i] = CC_CAST_16S(s0); dst[i+1] = CC_CAST_16S(s1);
                s0 -= sm[i]; s1 -= sm[i+1];
                sum[i] = s0; sum[i+1] = s1;
            }

            for(; i < width; i++)
            {
                int s0 = sum[i] + sp[i];
                dst[i] = CC_CAST_16S(s0);
                sum[i] = s0 - sm[i];
            }
            dst += dst_step;
        }
    }

    *_sum_count = sum_count;
}

static void
icvSumCol_32s32s(const int** src, int * dst,
                  int dst_step, int count, void* params)
{
    CBoxFilter* state = (CBoxFilter*)params;
    int ksize = state->get_kernel_size().height;
    int i, width = state->get_width();
    int cn = state->get_src_cn();
    int* sum = (int*)state->get_sum_buf();
    int* _sum_count = state->get_sum_count_ptr();
    int sum_count = *_sum_count;

    dst_step /= sizeof(dst[0]);
    width *= cn;
    src += sum_count;
    count += ksize - 1 - sum_count;

    for(; count--; src++)
    {
        const int* sp = src[0];
        if(sum_count+1 < ksize)
        {
            for(i = 0; i <= width - 2; i += 2)
            {
                int s0 = sum[i] + sp[i], s1 = sum[i+1] + sp[i+1];
                sum[i] = s0; sum[i+1] = s1;
            }

            for(; i < width; i++)
                sum[i] += sp[i];

            sum_count++;
        }
        else
        {
            const int* sm = src[-ksize+1];
            for(i = 0; i <= width - 2; i += 2)
            {
                int s0 = sum[i] + sp[i], s1 = sum[i+1] + sp[i+1];
                dst[i] = s0; dst[i+1] = s1;
                s0 -= sm[i]; s1 -= sm[i+1];
                sum[i] = s0; sum[i+1] = s1;
            }

            for(; i < width; i++)
            {
                int s0 = sum[i] + sp[i];
                dst[i] = s0;
                sum[i] = s0 - sm[i];
            }
            dst += dst_step;
        }
    }

    *_sum_count = sum_count;
}


static void
icvSumCol_64f32f(const double** src, float* dst,
                  int dst_step, int count, void* params)
{
    CBoxFilter* state = (CBoxFilter*)params;
    int ksize = state->get_kernel_size().height;
    int i, width = state->get_width();
    int cn = state->get_src_cn();
    double scale = state->get_scale();
    bool normalized = state->is_normalized();
    double* sum = (double*)state->get_sum_buf();
    int* _sum_count = state->get_sum_count_ptr();
    int sum_count = *_sum_count;

    dst_step /= sizeof(dst[0]);
    width *= cn;
    src += sum_count;
    count += ksize - 1 - sum_count;

    for(; count--; src++)
    {
        const double* sp = src[0];
        if(sum_count+1 < ksize)
        {
            for(i = 0; i <= width - 2; i += 2)
            {
                double s0 = sum[i] + sp[i], s1 = sum[i+1] + sp[i+1];
                sum[i] = s0; sum[i+1] = s1;
            }

            for(; i < width; i++)
                sum[i] += sp[i];

            sum_count++;
        }
        else
        {
            const double* sm = src[-ksize+1];
            if(normalized)
                for(i = 0; i <= width - 2; i += 2)
                {
                    double s0 = sum[i] + sp[i], s1 = sum[i+1] + sp[i+1];
                    double t0 = s0*scale, t1 = s1*scale;
                    s0 -= sm[i]; s1 -= sm[i+1];
                    dst[i] = (float)t0; dst[i+1] = (float)t1;
                    sum[i] = s0; sum[i+1] = s1;
                }
            else
                for(i = 0; i <= width - 2; i += 2)
                {
                    double s0 = sum[i] + sp[i], s1 = sum[i+1] + sp[i+1];
                    dst[i] = (float)s0; dst[i+1] = (float)s1;
                    s0 -= sm[i]; s1 -= sm[i+1];
                    sum[i] = s0; sum[i+1] = s1;
                }

            for(; i < width; i++)
            {
                double s0 = sum[i] + sp[i], t0 = s0*scale;
                sum[i] = s0 - sm[i]; dst[i] = (float)t0;
            }
            dst += dst_step;
        }
    }

    *_sum_count = sum_count;
}


/****************************************************************************************\
                                      Median Filter
\****************************************************************************************/

#define CC_MINMAX_8U(a,b) \
    (t = CC_FAST_CAST_8U((a) - (b)), (b) += t, a -= t)

static CStatus C_STDCALL
icvMedianBlur_8u_CnR(uchar* src, int src_step, uchar* dst, int dst_step,
                      CSize size, int m, int cn)
{
    #define N  16
    int     zone0[4][N];
    int     zone1[4][N*N];
    int     x, y;
    int     n2 = m*m/2;
    int     nx = (m + 1)/2 - 1;
    uchar*  src_max = src + size.height*src_step;
    uchar*  src_right = src + size.width*cn;

    #define UPDATE_ACC01(pix, cn, op) \
    {                                   \
        int p = (pix);                  \
        zone1[cn][p] op;                \
        zone0[cn][p >> 4] op;           \
    }

    if(size.height < nx || size.width < nx)
        return CC_BADSIZE_ERR;

    if(m == 3)
    {
        size.width *= cn;

        for(y = 0; y < size.height; y++, dst += dst_step)
        {
            const uchar* src0 = src + src_step*(y-1);
            const uchar* src1 = src0 + src_step;
            const uchar* src2 = src1 + src_step;
            if(y == 0)
                src0 = src1;
            else if(y == size.height - 1)
                src2 = src1;

            for(x = 0; x < 2*cn; x++)
            {
                int x0 = x < cn ? x : size.width - 3*cn + x;
                int x2 = x < cn ? x + cn : size.width - 2*cn + x;
                int x1 = x < cn ? x0 : x2, t;

                int p0 = src0[x0], p1 = src0[x1], p2 = src0[x2];
                int p3 = src1[x0], p4 = src1[x1], p5 = src1[x2];
                int p6 = src2[x0], p7 = src2[x1], p8 = src2[x2];

                CC_MINMAX_8U(p1, p2); CC_MINMAX_8U(p4, p5);
                CC_MINMAX_8U(p7, p8); CC_MINMAX_8U(p0, p1);
                CC_MINMAX_8U(p3, p4); CC_MINMAX_8U(p6, p7);
                CC_MINMAX_8U(p1, p2); CC_MINMAX_8U(p4, p5);
                CC_MINMAX_8U(p7, p8); CC_MINMAX_8U(p0, p3);
                CC_MINMAX_8U(p5, p8); CC_MINMAX_8U(p4, p7);
                CC_MINMAX_8U(p3, p6); CC_MINMAX_8U(p1, p4);
                CC_MINMAX_8U(p2, p5); CC_MINMAX_8U(p4, p7);
                CC_MINMAX_8U(p4, p2); CC_MINMAX_8U(p6, p4);
                CC_MINMAX_8U(p4, p2);
                dst[x1] = (uchar)p4;
            }

            for(x = cn; x < size.width - cn; x++)
            {
                int p0 = src0[x-cn], p1 = src0[x], p2 = src0[x+cn];
                int p3 = src1[x-cn], p4 = src1[x], p5 = src1[x+cn];
                int p6 = src2[x-cn], p7 = src2[x], p8 = src2[x+cn];
                int t;

                CC_MINMAX_8U(p1, p2); CC_MINMAX_8U(p4, p5);
                CC_MINMAX_8U(p7, p8); CC_MINMAX_8U(p0, p1);
                CC_MINMAX_8U(p3, p4); CC_MINMAX_8U(p6, p7);
                CC_MINMAX_8U(p1, p2); CC_MINMAX_8U(p4, p5);
                CC_MINMAX_8U(p7, p8); CC_MINMAX_8U(p0, p3);
                CC_MINMAX_8U(p5, p8); CC_MINMAX_8U(p4, p7);
                CC_MINMAX_8U(p3, p6); CC_MINMAX_8U(p1, p4);
                CC_MINMAX_8U(p2, p5); CC_MINMAX_8U(p4, p7);
                CC_MINMAX_8U(p4, p2); CC_MINMAX_8U(p6, p4);
                CC_MINMAX_8U(p4, p2);

                dst[x] = (uchar)p4;
            }
        }

        return CC_OK;
    }

    for(x = 0; x < size.width; x++, dst += cn)
    {
        uchar* dst_cur = dst;
        uchar* src_top = src;
        uchar* src_bottom = src;
        int    k, c;
        int    x0 = -1;

        if(x <= m/2)
            nx++;

        if(nx < m)
            x0 = x < m/2 ? 0 : (nx-1)*cn;

        // init accumulator
        memset(zone0, 0, sizeof(zone0[0])*cn);
        memset(zone1, 0, sizeof(zone1[0])*cn);

        for(y = -m/2; y < m/2; y++)
        {
            for(c = 0; c < cn; c++)
            {
                if(x0 >= 0)
                    UPDATE_ACC01(src_bottom[x0+c], c, += (m - nx));
                for(k = 0; k < nx*cn; k += cn)
                    UPDATE_ACC01(src_bottom[k+c], c, ++);
            }

            if((unsigned)y < (unsigned)(size.height-1))
                src_bottom += src_step;
        }

        for(y = 0; y < size.height; y++, dst_cur += dst_step)
        {
            if(cn == 1)
            {
                for(k = 0; k < nx; k++)
                    UPDATE_ACC01(src_bottom[k], 0, ++);
            }
            else if(cn == 3)
            {
                for(k = 0; k < nx*3; k += 3)
                {
                    UPDATE_ACC01(src_bottom[k], 0, ++);
                    UPDATE_ACC01(src_bottom[k+1], 1, ++);
                    UPDATE_ACC01(src_bottom[k+2], 2, ++);
                }
            }
            else
            {
                assert(cn == 4);
                for(k = 0; k < nx*4; k += 4)
                {
                    UPDATE_ACC01(src_bottom[k], 0, ++);
                    UPDATE_ACC01(src_bottom[k+1], 1, ++);
                    UPDATE_ACC01(src_bottom[k+2], 2, ++);
                    UPDATE_ACC01(src_bottom[k+3], 3, ++);
                }
            }

            if(x0 >= 0)
            {
                for(c = 0; c < cn; c++)
                    UPDATE_ACC01(src_bottom[x0+c], c, += (m - nx));
            }

            if(src_bottom + src_step < src_max)
                src_bottom += src_step;

            // find median
            for(c = 0; c < cn; c++)
            {
                int s = 0;
                for(k = 0; ; k++)
                {
                    int t = s + zone0[c][k];
                    if(t > n2) break;
                    s = t;
                }

                for(k *= N; ;k++)
                {
                    s += zone1[c][k];
                    if(s > n2) break;
                }

                dst_cur[c] = (uchar)k;
            }

            if(cn == 1)
            {
                for(k = 0; k < nx; k++)
                    UPDATE_ACC01(src_top[k], 0, --);
            }
            else if(cn == 3)
            {
                for(k = 0; k < nx*3; k += 3)
                {
                    UPDATE_ACC01(src_top[k], 0, --);
                    UPDATE_ACC01(src_top[k+1], 1, --);
                    UPDATE_ACC01(src_top[k+2], 2, --);
                }
            }
            else
            {
                assert(cn == 4);
                for(k = 0; k < nx*4; k += 4)
                {
                    UPDATE_ACC01(src_top[k], 0, --);
                    UPDATE_ACC01(src_top[k+1], 1, --);
                    UPDATE_ACC01(src_top[k+2], 2, --);
                    UPDATE_ACC01(src_top[k+3], 3, --);
                }
            }

            if(x0 >= 0)
            {
                for(c = 0; c < cn; c++)
                    UPDATE_ACC01(src_top[x0+c], c, -= (m - nx));
            }

            if(y >= m/2)
                src_top += src_step;
        }

        if(x >= m/2)
            src += cn;
        if(src + nx*cn > src_right) nx--;
    }
#undef N
#undef UPDATE_ACC
    return CC_OK;
}


/****************************************************************************************\
                                   Bilateral Filtering
\****************************************************************************************/

static CStatus C_STDCALL
icvBilateralFiltering_8u_CnR(uchar* src, int srcStep,
                              uchar* dst, int dstStep,
                              CSize size, double sigma_color,
                              double sigma_space, int channels)
{
    double i2sigma_color = 1./(sigma_color*sigma_color);
    double i2sigma_space = 1./(sigma_space*sigma_space);

    double mean1[3];
    double mean0;
    double w;
    int deltas[8];
    double weight_tab[8];

    int i, j;

#define INIT_C1\
            color = src[0]; \
            mean0 = 1; mean1[0] = color;

#define COLOR_DISTANCE_C1(c1, c2)\
            (c1 - c2)*(c1 - c2)
#define KERNEL_ELEMENT_C1(k)\
            temp_color = src[deltas[k]];\
            w = weight_tab[k] + COLOR_DISTANCE_C1(color, temp_color)*i2sigma_color;\
            w = 1./(w*w + 1); \
            mean0 += w;\
            mean1[0] += temp_color*w;

#define INIT_C3\
            mean0 = 1; mean1[0] = src[0];mean1[1] = src[1];mean1[2] = src[2];

#define UPDATE_OUTPUT_C1                   \
            dst[i] = (uchar)cRound(mean1[0]/mean0);

#define COLOR_DISTANCE_C3(c1, c2)\
            ((c1[0] - c2[0])*(c1[0] - c2[0]) + \
             (c1[1] - c2[1])*(c1[1] - c2[1]) + \
             (c1[2] - c2[2])*(c1[2] - c2[2]))
#define KERNEL_ELEMENT_C3(k)\
            temp_color = src + deltas[k];\
            w = weight_tab[k] + COLOR_DISTANCE_C3(src, temp_color)*i2sigma_color;\
            w = 1./(w*w + 1); \
            mean0 += w;\
            mean1[0] += temp_color[0]*w; \
            mean1[1] += temp_color[1]*w; \
            mean1[2] += temp_color[2]*w;

#define UPDATE_OUTPUT_C3\
            mean0 = 1./mean0;\
            dst[i*3 + 0] = (uchar)cRound(mean1[0]*mean0); \
            dst[i*3 + 1] = (uchar)cRound(mean1[1]*mean0); \
            dst[i*3 + 2] = (uchar)cRound(mean1[2]*mean0);

    CC_INIT_3X3_DELTAS(deltas, srcStep, channels);

    weight_tab[0] = weight_tab[2] = weight_tab[4] = weight_tab[6] = i2sigma_space;
    weight_tab[1] = weight_tab[3] = weight_tab[5] = weight_tab[7] = i2sigma_space*2;

    if(channels == 1)
    {
        int color, temp_color;

        for(i = 0; i < size.width; i++, src++)
        {
            INIT_C1;
            KERNEL_ELEMENT_C1(6);
            if(i > 0)
            {
                KERNEL_ELEMENT_C1(5);
                KERNEL_ELEMENT_C1(4);
            }
            if(i < size.width - 1)
            {
                KERNEL_ELEMENT_C1(7);
                KERNEL_ELEMENT_C1(0);
            }
            UPDATE_OUTPUT_C1;
        }

        src += srcStep - size.width;
        dst += dstStep;

        for(j = 1; j < size.height - 1; j++, dst += dstStep)
        {
            i = 0;
            INIT_C1;
            KERNEL_ELEMENT_C1(0);
            KERNEL_ELEMENT_C1(1);
            KERNEL_ELEMENT_C1(2);
            KERNEL_ELEMENT_C1(6);
            KERNEL_ELEMENT_C1(7);
            UPDATE_OUTPUT_C1;

            for(i = 1, src++; i < size.width - 1; i++, src++)
            {
                INIT_C1;
                KERNEL_ELEMENT_C1(0);
                KERNEL_ELEMENT_C1(1);
                KERNEL_ELEMENT_C1(2);
                KERNEL_ELEMENT_C1(3);
                KERNEL_ELEMENT_C1(4);
                KERNEL_ELEMENT_C1(5);
                KERNEL_ELEMENT_C1(6);
                KERNEL_ELEMENT_C1(7);
                UPDATE_OUTPUT_C1;
            }

            INIT_C1;
            KERNEL_ELEMENT_C1(2);
            KERNEL_ELEMENT_C1(3);
            KERNEL_ELEMENT_C1(4);
            KERNEL_ELEMENT_C1(5);
            KERNEL_ELEMENT_C1(6);
            UPDATE_OUTPUT_C1;

            src += srcStep + 1 - size.width;
        }

        for(i = 0; i < size.width; i++, src++)
        {
            INIT_C1;
            KERNEL_ELEMENT_C1(2);
            if(i > 0)
            {
                KERNEL_ELEMENT_C1(3);
                KERNEL_ELEMENT_C1(4);
            }
            if(i < size.width - 1)
            {
                KERNEL_ELEMENT_C1(1);
                KERNEL_ELEMENT_C1(0);
            }
            UPDATE_OUTPUT_C1;
        }
    }
    else
    {
        uchar* temp_color;

        if(channels != 3)
            return CC_UNSUPPORTED_CHANNELS_ERR;

        for(i = 0; i < size.width; i++, src += 3)
        {
            INIT_C3;
            KERNEL_ELEMENT_C3(6);
            if(i > 0)
            {
                KERNEL_ELEMENT_C3(5);
                KERNEL_ELEMENT_C3(4);
            }
            if(i < size.width - 1)
            {
                KERNEL_ELEMENT_C3(7);
                KERNEL_ELEMENT_C3(0);
            }
            UPDATE_OUTPUT_C3;
        }

        src += srcStep - size.width*3;
        dst += dstStep;

        for(j = 1; j < size.height - 1; j++, dst += dstStep)
        {
            i = 0;
            INIT_C3;
            KERNEL_ELEMENT_C3(0);
            KERNEL_ELEMENT_C3(1);
            KERNEL_ELEMENT_C3(2);
            KERNEL_ELEMENT_C3(6);
            KERNEL_ELEMENT_C3(7);
            UPDATE_OUTPUT_C3;

            for(i = 1, src += 3; i < size.width - 1; i++, src += 3)
            {
                INIT_C3;
                KERNEL_ELEMENT_C3(0);
                KERNEL_ELEMENT_C3(1);
                KERNEL_ELEMENT_C3(2);
                KERNEL_ELEMENT_C3(3);
                KERNEL_ELEMENT_C3(4);
                KERNEL_ELEMENT_C3(5);
                KERNEL_ELEMENT_C3(6);
                KERNEL_ELEMENT_C3(7);
                UPDATE_OUTPUT_C3;
            }

            INIT_C3;
            KERNEL_ELEMENT_C3(2);
            KERNEL_ELEMENT_C3(3);
            KERNEL_ELEMENT_C3(4);
            KERNEL_ELEMENT_C3(5);
            KERNEL_ELEMENT_C3(6);
            UPDATE_OUTPUT_C3;

            src += srcStep + 3 - size.width*3;
        }

        for(i = 0; i < size.width; i++, src += 3)
        {
            INIT_C3;
            KERNEL_ELEMENT_C3(2);
            if(i > 0)
            {
                KERNEL_ELEMENT_C3(3);
                KERNEL_ELEMENT_C3(4);
            }
            if(i < size.width - 1)
            {
                KERNEL_ELEMENT_C3(1);
                KERNEL_ELEMENT_C3(0);
            }
            UPDATE_OUTPUT_C3;
        }
    }

    return CC_OK;
#undef INIT_C1
#undef KERNEL_ELEMENT_C1
#undef UPDATE_OUTPUT_C1
#undef INIT_C3
#undef KERNEL_ELEMENT_C3
#undef UPDATE_OUTPUT_C3
#undef COLOR_DISTANCE_C3
}

//////////////////////////////////////////////////////////////////////////////////////////

CC_IMPL void
cvSmooth(const img_t* src, img_t* dst, int smooth_type CC_DEFAULT(CC_GAUSSIAN),
          int param1 CC_DEFAULT(3), int param2 CC_DEFAULT(0), double param3 CC_DEFAULT(0), double param4 CC_DEFAULT(0))
{
    img_t temp[1] = {0};

    CC_FUNCNAME("cvSmooth");

    __BEGIN__;

    int coi1 = 0, coi2 = 0;
    CSize size;
    TypeId src_type, dst_type, type;
    int cn;
    double sigma1 = 0, sigma2 = 0;

    if(coi1 != 0 || coi2 != 0)
        CC_ERROR(CC_BadCOI, "");

    src_type = CC_MAT_DEPTH(src);
    dst_type = CC_MAT_DEPTH(dst);
    type = src_type;
    cn = CC_MAT_CN(src);
    size = cvGetMatSize(src);

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    if(smooth_type != CC_BLUR_NO_SCALE && !CC_ARE_TYPES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats,
        "The specified smoothing algorithm requires input and ouput arrays be of the same type");

    if(smooth_type == CC_BLUR || smooth_type == CC_BLUR_NO_SCALE ||
        smooth_type == CC_GAUSSIAN || smooth_type == CC_MEDIAN)
    {
        // automatic detection of kernel size from sigma
        if(smooth_type == CC_GAUSSIAN)
        {
            sigma1 = param3;
            sigma2 = param4 ? param4 : param3;

            if(param1 == 0 && sigma1 > 0)
                param1 = cRound(sigma1*(type == CC_8U ? 3 : 4)*2 + 1)|1;
            if(param2 == 0 && sigma2 > 0)
                param2 = cRound(sigma2*(type == CC_8U ? 3 : 4)*2 + 1)|1;
        }

        if(param2 == 0)
            param2 = size.height == 1 ? 1 : param1;
        if(param1 < 1 || (param1 & 1) == 0 || param2 < 1 || (param2 & 1) == 0)
            CC_ERROR(CC_StsOutOfRange,
                "Both mask width and height must be >=1 and odd");

        if(param1 == 1 && param2 == 1)
        {
            cvConvert(src, dst);
            EXIT;
        }
    }

    if(smooth_type == CC_BLUR || smooth_type == CC_BLUR_NO_SCALE)
    {
        CBoxFilter box_filter;
        CC_CALL(box_filter.init(src->cols, src_type, cn, dst_type, cn,
            smooth_type == CC_BLUR, cSize(param1, param2)));
        CC_CALL(box_filter.process(src, dst));
    }
    else if(smooth_type == CC_MEDIAN)
    {
        if(type != CC_8U || cn != 1 && cn != 3 && cn != 4)
            CC_ERROR(CC_StsUnsupportedFormat,
            "Median filter only supports 8uC1, 8uC3 and 8uC4 images");

        IPPI_CALL(icvMedianBlur_8u_CnR(src->tt.data, src->step,
            dst->tt.data, dst->step, size, param1, cn));
    }
    else if(smooth_type == CC_GAUSSIAN)
    {
        CSepFilter gaussian_filter;
        CSize ksize = { param1, param2 };
        float* kx = (float*)cvStackAlloc(ksize.width*sizeof(kx[0]));
        float* ky = (float*)cvStackAlloc(ksize.height*sizeof(ky[0]));
        img_t KX[1]; cvMat(KX, 1, ksize.width, CC_32F, 1, kx);
        img_t KY[1]; cvMat(KY, 1, ksize.height, CC_32F, 1, ky);
        
        CSepFilter::init_gaussian_kernel(KX, sigma1);
        if(ksize.width != ksize.height || fabs(sigma1 - sigma2) > FLT_EPSILON)
            CSepFilter::init_gaussian_kernel(KY, sigma2);
        else
            KY->tt.fl = kx;
        
        CC_CALL(gaussian_filter.init(src->cols, src_type, cn, dst_type, cn, KX, KY));
        CC_CALL(gaussian_filter.process(src, dst));
    }
    else if(smooth_type == CC_BILATERAL)
    {
        if(param1 < 0 || param2 < 0)
            CC_ERROR(CC_StsOutOfRange,
            "Thresholds in bilaral filtering should not bee negative");
        param1 += param1 == 0;
        param2 += param2 == 0;

        if(type != CC_8U || cn != 1 && cn != 3)
            CC_ERROR(CC_StsUnsupportedFormat,
            "Bilateral filter only supports 8uC1 and 8uC3 images");

        IPPI_CALL(icvBilateralFiltering_8u_CnR(src->tt.data, src->step,
            dst->tt.data, dst->step, size, param1, param2, cn));
    }

    __END__;

    imfree(temp);
}


