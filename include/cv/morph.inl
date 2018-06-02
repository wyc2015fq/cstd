#include <limits.h>
#include <stdio.h>

#define IPCC_MORPHOLOGY_PTRS(morphtype, flavor)               \
    icv##morphtype##Rect_##flavor##_C1R_t                       \
        icv##morphtype##Rect_##flavor##_C1R_p = 0;              \
    icv##morphtype##Rect_GetBufSize_##flavor##_C1R_t            \
        icv##morphtype##Rect_GetBufSize_##flavor##_C1R_p = 0;   \
    icv##morphtype##Rect_##flavor##_C3R_t                       \
        icv##morphtype##Rect_##flavor##_C3R_p = 0;              \
    icv##morphtype##Rect_GetBufSize_##flavor##_C3R_t            \
        icv##morphtype##Rect_GetBufSize_##flavor##_C3R_p = 0;   \
    icv##morphtype##Rect_##flavor##_C4R_t                       \
        icv##morphtype##Rect_##flavor##_C4R_p = 0;              \
    icv##morphtype##Rect_GetBufSize_##flavor##_C4R_t            \
        icv##morphtype##Rect_GetBufSize_##flavor##_C4R_p = 0;   \
                                                                \
    icv##morphtype##_##flavor##_C1R_t                           \
        icv##morphtype##_##flavor##_C1R_p = 0;                  \
    icv##morphtype##_##flavor##_C3R_t                           \
        icv##morphtype##_##flavor##_C3R_p = 0;                  \
    icv##morphtype##_##flavor##_C4R_t                           \
        icv##morphtype##_##flavor##_C4R_p = 0;

#define IPCC_MORPHOLOGY_INITALLOC_PTRS(flavor)                \
    icvMorphInitAlloc_##flavor##_C1R_t                          \
        icvMorphInitAlloc_##flavor##_C1R_p = 0;                 \
    icvMorphInitAlloc_##flavor##_C3R_t                          \
        icvMorphInitAlloc_##flavor##_C3R_p = 0;                 \
    icvMorphInitAlloc_##flavor##_C4R_t                          \
        icvMorphInitAlloc_##flavor##_C4R_p = 0;

IPCC_MORPHOLOGY_PTRS(Erode, 8u)
IPCC_MORPHOLOGY_PTRS(Erode, 32f)
IPCC_MORPHOLOGY_PTRS(Dilate, 8u)
IPCC_MORPHOLOGY_PTRS(Dilate, 32f)
IPCC_MORPHOLOGY_INITALLOC_PTRS(8u)
IPCC_MORPHOLOGY_INITALLOC_PTRS(32f)

icvMorphFree_t icvMorphFree_p = 0;

/****************************************************************************************\
                     Basic Morphological Operations: Erosion & Dilation
\****************************************************************************************/

static void icvErodeRectRow_8u(const uchar* src, uchar* dst, void* params);
static void icvErodeRectRow_32f(const int* src, int* dst, void* params);
static void icvDilateRectRow_8u(const uchar* src, uchar* dst, void* params);
static void icvDilateRectRow_32f(const int* src, int* dst, void* params);

static void icvErodeRectCol_8u(const uchar** src, uchar* dst, int dst_step,
                                int count, void* params);
static void icvErodeRectCol_32f(const int** src, int* dst, int dst_step,
                                 int count, void* params);
static void icvDilateRectCol_8u(const uchar** src, uchar* dst, int dst_step,
                                 int count, void* params);
static void icvDilateRectCol_32f(const int** src, int* dst, int dst_step,
                                  int count, void* params);

static void icvErodeAny_8u(const uchar** src, uchar* dst, int dst_step,
                            int count, void* params);
static void icvErodeAny_32f(const int** src, int* dst, int dst_step,
                             int count, void* params);
static void icvDilateAny_8u(const uchar** src, uchar* dst, int dst_step,
                             int count, void* params);
static void icvDilateAny_32f(const int** src, int* dst, int dst_step,
                              int count, void* params);

CvMorphology::CvMorphology()
{
    element = 0;
    el_sparse = 0;
}

CvMorphology::CvMorphology(int _operation, int _max_width, int _src_dst_type,
                            int _element_shape, img_t* _element,
                            CSize _ksize, CPoint _anchor,
                            int _border_mode, CScalar _border_value)
{
    element = 0;
    el_sparse = 0;
    init(_operation, _max_width, _src_dst_type,
          _element_shape, _element, _ksize, _anchor,
          _border_mode, _border_value);
}


void CvMorphology::clear()
{
    cvReleaseMat(&element);
    cFree(&el_sparse);
    CvBaseImageFilter::clear();
}


CvMorphology::~CvMorphology()
{
    clear();
}


void CvMorphology::init(int _operation, int _max_width, int _src_dst_type,
                         int _element_shape, img_t* _element,
                         CSize _ksize, CPoint _anchor,
                         int _border_mode, CScalar _border_value)
{
    CC_FUNCNAME("CvMorphology::init");

    __BEGIN__;

    TypeId type = CC_MAT_DEPTH(_src_dst_type);
    int el_type = 0, nz = -1;
    
    if(_operation != ERODE && _operation != DILATE)
        CC_ERROR(CC_StsBadArg, "Unknown/unsupported morphological operation");

    if(_element_shape == CUSTOM)
    {
        if(!CC_IS_MAT(_element))
            CC_ERROR(CC_StsBadArg,
            "structuring element should be valid matrix if CUSTOM element shape is specified");

        el_type = CC_MAT_DEPTH(_element->tid);
        if(el_type != CC_8UC1 && el_type != CC_32SC1)
            CC_ERROR(CC_StsUnsupportedFormat, "the structuring element must have 8uC1 or 32sC1 type");

        _ksize = cvGetMatSize(_element);
        CC_CALL(nz = cvCountNonZero(_element));
        if(nz == _ksize.width*_ksize.height)
            _element_shape = RECT;
    }

    operation = _operation;
    el_shape = _element_shape;

    CC_CALL(CvBaseImageFilter::init(_max_width, _src_dst_type, _src_dst_type,
        _element_shape == RECT, _ksize, _anchor, _border_mode, _border_value));

    if(el_shape == RECT)
    {
        if(operation == ERODE)
        {
            if(type == CC_8U)
                x_func = (CvRowFilterFunc)icvErodeRectRow_8u,
                y_func = (CvColumnFilterFunc)icvErodeRectCol_8u;
            else if(type == CC_32F)
                x_func = (CvRowFilterFunc)icvErodeRectRow_32f,
                y_func = (CvColumnFilterFunc)icvErodeRectCol_32f;
        }
        else
        {
            assert(operation == DILATE);
            if(type == CC_8U)
                x_func = (CvRowFilterFunc)icvDilateRectRow_8u,
                y_func = (CvColumnFilterFunc)icvDilateRectCol_8u;
            else if(type == CC_32F)
                x_func = (CvRowFilterFunc)icvDilateRectRow_32f,
                y_func = (CvColumnFilterFunc)icvDilateRectCol_32f;
        }
    }
    else
    {
        int i, j, k = 0;
        int cn = CC_MAT_CN(src_type);
        CPoint* nz_loc;

        if(!(element && el_sparse &&
            _ksize.width == element->cols && _ksize.height == element->rows))
        {
            cvReleaseMat(&element);
            cFree(&el_sparse);
            CC_CALL(element = cvCreateMat(_ksize.height, _ksize.width, CC_8UC1));
            CC_CALL(el_sparse = (uchar*)cAlloc(
                ksize.width*ksize.height*(2*sizeof(int) + sizeof(uchar*))));
        }

        if(el_shape == CUSTOM)
        {
            CC_CALL(cvConvert(_element, element));
        }
        else
        {
            CC_CALL(init_binary_element(element, el_shape, anchor));
        }

        if(operation == ERODE)
        {
            if(type == CC_8U)
                y_func = (CvColumnFilterFunc)icvErodeAny_8u;
            else if(type == CC_32F)
                y_func = (CvColumnFilterFunc)icvErodeAny_32f;
        }
        else
        {
            assert(operation == DILATE);
            if(type == CC_8U)
                y_func = (CvColumnFilterFunc)icvDilateAny_8u;
            else if(type == CC_32F)
                y_func = (CvColumnFilterFunc)icvDilateAny_32f;
        }
        
        nz_loc = (CPoint*)el_sparse;

        for(i = 0; i < ksize.height; i++)
            for(j = 0; j < ksize.width; j++)
            {
                if(element->tt.data[i*element->step+j])
                    nz_loc[k++] = cPoint(j*cn,i);
            }
        if(k == 0)
            nz_loc[k++] = cPoint(anchor.x*cn,anchor.y);
        el_sparse_count = k;
    }

    if(type == CC_32F && border_mode == IMG_BORDER_CONSTANT)
    {
        int i, cn = CC_MAT_CN(src_type);
        int* bt = (int*)border_tab;
        for(i = 0; i < cn; i++)
            bt[i] = CC_TOGGLE_FLT(bt[i]);
    }

    __END__;
}


void CvMorphology::init(int _max_width, int _src_type, int _dst_type,
                         bool _is_separable, CSize _ksize,
                         CPoint _anchor, int _border_mode,
                         CScalar _border_value)
{
    CvBaseImageFilter::init(_max_width, _src_type, _dst_type, _is_separable,
                             _ksize, _anchor, _border_mode, _border_value);
}


void CvMorphology::start_process(CSlice x_range, int width)
{
    CvBaseImageFilter::start_process(x_range, width);
    if(el_shape == RECT)
    {
        // cut the cyclic buffer off by 1 line if need, to make
        // the vertical part of separable morphological filter
        // always process 2 rows at once (except, may be,
        // for the last one in a stripe).
        int t = buf_max_count - max_ky*2;
        if(t > 1 && t % 2 != 0)
        {
            buf_max_count--;
            buf_end -= buf_step;
        }
    }
}


int CvMorphology::fill_cyclic_buffer(const uchar* src, int src_step,
                                      int y0, int y1, int y2)
{
    int i, y = y0, bsz1 = border_tab_sz1, bsz = border_tab_sz;
    int pix_size = CC_ELEM_SIZE(src_type);
    int width_n = (prev_x_range.end_index - prev_x_range.start_index)*pix_size;

    if(src_type != CC_32F)
        return CvBaseImageFilter::fill_cyclic_buffer(src, src_step, y0, y1, y2);

    // fill the cyclic buffer
    for(; buf_count < buf_max_count && y < y2; buf_count++, y++, src += src_step)
    {
        uchar* trow = is_separable ? buf_end : buf_tail;

        for(i = 0; i < width_n; i += sizeof(int))
        {
            int t = *(int*)(src + i);
            *(int*)(trow + i + bsz1) = CC_TOGGLE_FLT(t);
        }

        if(border_mode != IMG_BORDER_CONSTANT)
        {
            for(i = 0; i < bsz1; i++)
            {
                int j = border_tab[i];
                trow[i] = trow[j];
            }
            for(; i < bsz; i++)
            {
                int j = border_tab[i];
                trow[i + width_n] = trow[j];
            }
        }
        else
        {
            const uchar *bt = (uchar*)border_tab; 
            for(i = 0; i < bsz1; i++)
                trow[i] = bt[i];

            for(; i < bsz; i++)
                trow[i + width_n] = bt[i];
        }

        if(is_separable)
            x_func(trow, buf_tail, this);

        buf_tail += buf_step;
        if(buf_tail >= buf_end)
            buf_tail = buf_start;
    }

    return y - y0;
}


void CvMorphology::init_binary_element(img_t* element, int element_shape, CPoint anchor)
{
    CC_FUNCNAME("CvMorphology::init_binary_element");

    __BEGIN__;

    TypeId type;
    int i, j, cols, rows;
    int r = 0, c = 0;
    double inv_r2 = 0;

    if(!CC_IS_MAT(element))
        CC_ERROR(CC_StsBadArg, "element must be valid matrix");

    type = CC_MAT_DEPTH(element->tid);
    if(type != CC_8UC1 && type != CC_32SC1)
        CC_ERROR(CC_StsUnsupportedFormat, "element must have 8uC1 or 32sC1 type");

    if(anchor.x == -1)
        anchor.x = element->cols/2;

    if(anchor.y == -1)
        anchor.y = element->rows/2;

    if((unsigned)anchor.x >= (unsigned)element->cols ||
        (unsigned)anchor.y >= (unsigned)element->rows)
        CC_ERROR(CC_StsOutOfRange, "anchor is outside of element");

    if(element_shape != RECT && element_shape != CROSS && element_shape != ELLIPSE)
        CC_ERROR(CC_StsBadArg, "Unknown/unsupported element shape");

    rows = element->rows;
    cols = element->cols;

    if(rows == 1 || cols == 1)
        element_shape = RECT;

    if(element_shape == ELLIPSE)
    {
        r = rows/2;
        c = cols/2;
        inv_r2 = r ? 1./((double)r*r) : 0;
    }

    for(i = 0; i < rows; i++)
    {
        uchar* ptr = element->tt.data + i*element->step;
        int j1 = 0, j2 = 0, jx, t = 0;

        if(element_shape == RECT || element_shape == CROSS && i == anchor.y)
            j2 = cols;
        else if(element_shape == CROSS)
            j1 = anchor.x, j2 = j1 + 1;
        else
        {
            int dy = i - r;
            if(abs(dy) <= r)
            {
                int dx = cRound(c*sqrt(((double)r*r - dy*dy)*inv_r2));
                j1 = MAX(c - dx, 0);
                j2 = MIN(c + dx + 1, cols);
            }
        }

        for(j = 0, jx = j1; j < cols;)
        {
            for(; j < jx; j++)
            {
                if(type == CC_8UC1)
                    ptr[j] = (uchar)t;
                else
                    ((int*)ptr)[j] = t;
            }
            if(jx == j2)
                jx = cols, t = 0;
            else
                jx = j2, t = 1;
        }
    }

    __END__;
}


#define ICC_MORPH_RECT_ROW(name, flavor, arrtype,          \
                            worktype, update_extr_macro)   \
static void                                                 \
icv##name##RectRow_##flavor(const arrtype* src,            \
                             arrtype* dst, void* params)   \
{                                                           \
    const CvMorphology* state = (const CvMorphology*)params;\
    int ksize = state->get_kernel_size().width;             \
    int width = state->get_width();                         \
    int cn = CC_MAT_CN(state->get_src_type());              \
    int i, j, k;                                            \
                                                            \
    width *= cn; ksize *= cn;                               \
                                                            \
    if(ksize == cn)                                       \
    {                                                       \
        for(i = 0; i < width; i++)                        \
            dst[i] = src[i];                                \
        return;                                             \
    }                                                       \
                                                            \
    for(k = 0; k < cn; k++, src++, dst++)                 \
    {                                                       \
        for(i = 0; i <= width - cn*2; i += cn*2)          \
        {                                                   \
            const arrtype* s = src + i;                     \
            worktype m = s[cn], t;                          \
            for(j = cn*2; j < ksize; j += cn)             \
            {                                               \
                t = s[j]; update_extr_macro(m,t);           \
            }                                               \
            t = s[0]; update_extr_macro(t,m);               \
            dst[i] = (arrtype)t;                            \
            t = s[j]; update_extr_macro(t,m);               \
            dst[i+cn] = (arrtype)t;                         \
        }                                                   \
                                                            \
        for(; i < width; i += cn)                         \
        {                                                   \
            const arrtype* s = src + i;                     \
            worktype m = s[0], t;                           \
            for(j = cn; j < ksize; j += cn)               \
            {                                               \
                t = s[j]; update_extr_macro(m,t);           \
            }                                               \
            dst[i] = (arrtype)m;                            \
        }                                                   \
    }                                                       \
}


ICC_MORPH_RECT_ROW(Erode, 8u, uchar, int, CC_CALC_MIN_8U)
ICC_MORPH_RECT_ROW(Dilate, 8u, uchar, int, CC_CALC_MAX_8U)
ICC_MORPH_RECT_ROW(Erode, 32f, int, int, CC_CALC_MIN)
ICC_MORPH_RECT_ROW(Dilate, 32f, int, int, CC_CALC_MAX)


#define ICC_MORPH_RECT_COL(name, flavor, arrtype,          \
        worktype, update_extr_macro, toggle_macro)         \
static void                                                 \
icv##name##RectCol_##flavor(const arrtype** src,           \
    arrtype* dst, int dst_step, int count, void* params)   \
{                                                           \
    const CvMorphology* state = (const CvMorphology*)params;\
    int ksize = state->get_kernel_size().height;            \
    int width = state->get_width();                         \
    int cn = CC_MAT_CN(state->get_src_type());              \
    int i, k;                                               \
                                                            \
    width *= cn;                                            \
    dst_step /= sizeof(dst[0]);                             \
                                                            \
    for(; ksize > 1 && count > 1; count -= 2,              \
                        dst += dst_step*2, src += 2)       \
    {                                                       \
        for(i = 0; i <= width - 4; i += 4)                \
        {                                                   \
            const arrtype* sptr = src[1] + i;               \
            worktype s0 = sptr[0], s1 = sptr[1],            \
                s2 = sptr[2], s3 = sptr[3], t0, t1;         \
                                                            \
            for(k = 2; k < ksize; k++)                    \
            {                                               \
                sptr = src[k] + i;                          \
                t0 = sptr[0]; t1 = sptr[1];                 \
                update_extr_macro(s0,t0);                   \
                update_extr_macro(s1,t1);                   \
                t0 = sptr[2]; t1 = sptr[3];                 \
                update_extr_macro(s2,t0);                   \
                update_extr_macro(s3,t1);                   \
            }                                               \
                                                            \
            sptr = src[0] + i;                              \
            t0 = sptr[0]; t1 = sptr[1];                     \
            update_extr_macro(t0,s0);                       \
            update_extr_macro(t1,s1);                       \
            dst[i] = (arrtype)toggle_macro(t0);             \
            dst[i+1] = (arrtype)toggle_macro(t1);           \
            t0 = sptr[2]; t1 = sptr[3];                     \
            update_extr_macro(t0,s2);                       \
            update_extr_macro(t1,s3);                       \
            dst[i+2] = (arrtype)toggle_macro(t0);           \
            dst[i+3] = (arrtype)toggle_macro(t1);           \
                                                            \
            sptr = src[k] + i;                              \
            t0 = sptr[0]; t1 = sptr[1];                     \
            update_extr_macro(t0,s0);                       \
            update_extr_macro(t1,s1);                       \
            dst[i+dst_step] = (arrtype)toggle_macro(t0);    \
            dst[i+dst_step+1] = (arrtype)toggle_macro(t1);  \
            t0 = sptr[2]; t1 = sptr[3];                     \
            update_extr_macro(t0,s2);                       \
            update_extr_macro(t1,s3);                       \
            dst[i+dst_step+2] = (arrtype)toggle_macro(t0);  \
            dst[i+dst_step+3] = (arrtype)toggle_macro(t1);  \
        }                                                   \
                                                            \
        for(; i < width; i++)                             \
        {                                                   \
            const arrtype* sptr = src[1] + i;               \
            worktype s0 = sptr[0], t0;                      \
                                                            \
            for(k = 2; k < ksize; k++)                    \
            {                                               \
                sptr = src[k] + i; t0 = sptr[0];            \
                update_extr_macro(s0,t0);                   \
            }                                               \
                                                            \
            sptr = src[0] + i; t0 = sptr[0];                \
            update_extr_macro(t0,s0);                       \
            dst[i] = (arrtype)toggle_macro(t0);             \
                                                            \
            sptr = src[k] + i; t0 = sptr[0];                \
            update_extr_macro(t0,s0);                       \
            dst[i+dst_step] = (arrtype)toggle_macro(t0);    \
        }                                                   \
    }                                                       \
                                                            \
    for(; count > 0; count--, dst += dst_step, src++)     \
    {                                                       \
        for(i = 0; i <= width - 4; i += 4)                \
        {                                                   \
            const arrtype* sptr = src[0] + i;               \
            worktype s0 = sptr[0], s1 = sptr[1],            \
                s2 = sptr[2], s3 = sptr[3], t0, t1;         \
                                                            \
            for(k = 1; k < ksize; k++)                    \
            {                                               \
                sptr = src[k] + i;                          \
                t0 = sptr[0]; t1 = sptr[1];                 \
                update_extr_macro(s0,t0);                   \
                update_extr_macro(s1,t1);                   \
                t0 = sptr[2]; t1 = sptr[3];                 \
                update_extr_macro(s2,t0);                   \
                update_extr_macro(s3,t1);                   \
            }                                               \
            dst[i] = (arrtype)toggle_macro(s0);             \
            dst[i+1] = (arrtype)toggle_macro(s1);           \
            dst[i+2] = (arrtype)toggle_macro(s2);           \
            dst[i+3] = (arrtype)toggle_macro(s3);           \
        }                                                   \
                                                            \
        for(; i < width; i++)                             \
        {                                                   \
            const arrtype* sptr = src[0] + i;               \
            worktype s0 = sptr[0], t0;                      \
                                                            \
            for(k = 1; k < ksize; k++)                    \
            {                                               \
                sptr = src[k] + i; t0 = sptr[0];            \
                update_extr_macro(s0,t0);                   \
            }                                               \
            dst[i] = (arrtype)toggle_macro(s0);             \
        }                                                   \
    }                                                       \
}


ICC_MORPH_RECT_COL(Erode, 8u, uchar, int, CC_CALC_MIN_8U, CC_NOP)
ICC_MORPH_RECT_COL(Dilate, 8u, uchar, int, CC_CALC_MAX_8U, CC_NOP)
ICC_MORPH_RECT_COL(Erode, 32f, int, int, CC_CALC_MIN, CC_TOGGLE_FLT)
ICC_MORPH_RECT_COL(Dilate, 32f, int, int, CC_CALC_MAX, CC_TOGGLE_FLT)


#define ICC_MORPH_ANY(name, flavor, arrtype, worktype,     \
                       update_extr_macro, toggle_macro)    \
static void                                                 \
icv##name##Any_##flavor(const arrtype** src, arrtype* dst, \
                    int dst_step, int count, void* params) \
{                                                           \
    CvMorphology* state = (CvMorphology*)params;            \
    int width = state->get_width();                         \
    int cn = CC_MAT_CN(state->get_src_type());              \
    int i, k;                                               \
    CPoint* el_sparse = (CPoint*)state->get_element_sparse_buf();\
    int el_count = state->get_element_sparse_count();       \
    const arrtype** el_ptr = (const arrtype**)(el_sparse + el_count);\
    const arrtype** el_end = el_ptr + el_count;             \
                                                            \
    width *= cn;                                            \
    dst_step /= sizeof(dst[0]);                             \
                                                            \
    for(; count > 0; count--, dst += dst_step, src++)     \
    {                                                       \
        for(k = 0; k < el_count; k++)                     \
            el_ptr[k] = src[el_sparse[k].y]+el_sparse[k].x; \
                                                            \
        for(i = 0; i <= width - 4; i += 4)                \
        {                                                   \
            const arrtype** psptr = el_ptr;                 \
            const arrtype* sptr = *psptr++;                 \
            worktype s0 = sptr[i], s1 = sptr[i+1],          \
                     s2 = sptr[i+2], s3 = sptr[i+3], t;     \
                                                            \
            while(psptr != el_end)                        \
            {                                               \
                sptr = *psptr++;                            \
                t = sptr[i];                                \
                update_extr_macro(s0,t);                    \
                t = sptr[i+1];                              \
                update_extr_macro(s1,t);                    \
                t = sptr[i+2];                              \
                update_extr_macro(s2,t);                    \
                t = sptr[i+3];                              \
                update_extr_macro(s3,t);                    \
            }                                               \
                                                            \
            dst[i] = (arrtype)toggle_macro(s0);             \
            dst[i+1] = (arrtype)toggle_macro(s1);           \
            dst[i+2] = (arrtype)toggle_macro(s2);           \
            dst[i+3] = (arrtype)toggle_macro(s3);           \
        }                                                   \
                                                            \
        for(; i < width; i++)                             \
        {                                                   \
            const arrtype* sptr = el_ptr[0] + i;            \
            worktype s0 = sptr[0], t0;                      \
                                                            \
            for(k = 1; k < el_count; k++)                 \
            {                                               \
                sptr = el_ptr[k] + i;                       \
                t0 = sptr[0];                               \
                update_extr_macro(s0,t0);                   \
            }                                               \
                                                            \
            dst[i] = (arrtype)toggle_macro(s0);             \
        }                                                   \
    }                                                       \
}

ICC_MORPH_ANY(Erode, 8u, uchar, int, CC_CALC_MIN, CC_NOP)
ICC_MORPH_ANY(Dilate, 8u, uchar, int, CC_CALC_MAX, CC_NOP)
ICC_MORPH_ANY(Erode, 32f, int, int, CC_CALC_MIN, CC_TOGGLE_FLT)
ICC_MORPH_ANY(Dilate, 32f, int, int, CC_CALC_MAX, CC_TOGGLE_FLT)

/////////////////////////////////// External Interface /////////////////////////////////////


CC_IMPL IplConvKernel *
cvCreateStructuringElementEx(int cols, int rows,
                              int anchorX, int anchorY,
                              int shape, int *values)
{
    IplConvKernel *element = 0;
    int i, size = rows * cols;
    int element_size = sizeof(*element) + size*sizeof(element->values[0]);

    CC_FUNCNAME("cvCreateStructuringElementEx");

    __BEGIN__;

    if(!values && shape == CC_SHAPE_CUSTOM)
        CC_ERROR_FROM_STATUS(CC_NULLPTR_ERR);

    if(cols <= 0 || rows <= 0 ||
        (unsigned) anchorX >= (unsigned) cols ||
        (unsigned) anchorY >= (unsigned) rows)
        CC_ERROR_FROM_STATUS(CC_BADSIZE_ERR);

    CC_CALL(element = (IplConvKernel *)cAlloc(element_size + 32));
    if(!element)
        CC_ERROR_FROM_STATUS(CC_OUTOFMEM_ERR);

    element->nCols = cols;
    element->nRows = rows;
    element->anchorX = anchorX;
    element->anchorY = anchorY;
    element->nShiftR = shape < CC_SHAPE_ELLIPSE ? shape : CC_SHAPE_CUSTOM;
    element->values = (int*)(element + 1);

    if(shape == CC_SHAPE_CUSTOM)
    {
        if(!values)
            CC_ERROR(CC_StsNullPtr, "Null pointer to the custom element mask");
        for(i = 0; i < size; i++)
            element->values[i] = values[i];
    }
    else
    {
        img_t el_hdr = cvMat(rows, cols, CC_32SC1, element->values);
        CC_CALL(CvMorphology::init_binary_element(&el_hdr,
                        shape, cPoint(anchorX,anchorY)));
    }

    __END__;

    if(cvGetErrStatus() < 0)
        cvReleaseStructuringElement(&element);

    return element;
}


CC_IMPL void
cvReleaseStructuringElement(IplConvKernel ** element)
{
    CC_FUNCNAME("cvReleaseStructuringElement");

    __BEGIN__;

    if(!element)
        CC_ERROR(CC_StsNullPtr, "");
    cFree(element);

    __END__;
}


typedef CStatus (C_STDCALL * CvMorphRectGetBufSizeFunc_IPP)
    (int width, CSize el_size, int* bufsize);

typedef CStatus (C_STDCALL * CvMorphRectFunc_IPP)
    (const void* src, int srcstep, void* dst, int dststep,
      CSize roi, CSize el_size, CPoint el_anchor, void* buffer);

typedef CStatus (C_STDCALL * CvMorphCustomInitAllocFunc_IPP)
    (int width, const uchar* element, CSize el_size,
      CPoint el_anchor, void** morphstate);

typedef CStatus (C_STDCALL * CvMorphCustomFunc_IPP)
    (const void* src, int srcstep, void* dst, int dststep,
      CSize roi, int bordertype, void* morphstate);

static void
icvMorphOp(const img_t* src, img_t* dst, IplConvKernel* element,
            int iterations, int mop)
{
    CvMorphology morphology;
    void* buffer = 0;
    int local_alloc = 0;
    void* morphstate = 0;
    img_t* temp = 0;

    CC_FUNCNAME("icvMorphOp");

    __BEGIN__;

    int i, coi1 = 0, coi2 = 0;
    
    
    img_t el_hdr, *el = 0;
    CSize size, el_size;
    CPoint el_anchor;
    int el_shape;
    TypeId type;
    bool inplace;

    if(!CC_IS_MAT(src))
        
    
    if(src != &srcstub)
    {
        srcstub = *src;
        src = &srcstub;
    }

    if(dstarr == srcarr)
        dst = src;
    else
    {
        

        if(!CC_ARE_TYPES_EQ(src, dst))
            CC_ERROR(CC_StsUnmatchedFormats, "");

        if(!CC_ARE_SIZES_EQ(src, dst))
            CC_ERROR(CC_StsUnmatchedSizes, "");
    }

    if(dst != &dststub)
    {
        dststub = *dst;
        dst = &dststub;
    }

    if(coi1 != 0 || coi2 != 0)
        CC_ERROR(CC_BadCOI, "");

    type = CC_MAT_DEPTH(src);
    size = cvGetMatSize(src);
    inplace = src->tt.data == dst->tt.data;

    if(iterations == 0 || (element && element->nCols == 1 && element->nRows == 1))
    {
        if(src->tt.data != dst->tt.data)
            cvCopy(src, dst);
        EXIT;
    }

    if(element)
    {
        el_size = cSize(element->nCols, element->nRows);
        el_anchor = cPoint(element->anchorX, element->anchorY);
        el_shape = (int)(element->nShiftR);
        el_shape = el_shape < CC_SHAPE_CUSTOM ? el_shape : CC_SHAPE_CUSTOM;
    }
    else
    {
        el_size = cSize(3,3);
        el_anchor = cPoint(1,1);
        el_shape = CC_SHAPE_RECT;
    }

    if(el_shape == CC_SHAPE_RECT && iterations > 1)
    {
        el_size.width += (el_size.width-1)*iterations;
        el_size.height += (el_size.height-1)*iterations;
        el_anchor.x *= iterations;
        el_anchor.y *= iterations;
        iterations = 1;
    }

    if(el_shape == CC_SHAPE_RECT && icvErodeRect_GetBufSize_8u_C1R_p)
    {
        CvMorphRectFunc_IPP rect_func = 0;
        CvMorphRectGetBufSizeFunc_IPP rect_getbufsize_func = 0;

        if(mop == 0)
        {
            if(type == CC_8UC1)
                rect_getbufsize_func = icvErodeRect_GetBufSize_8u_C1R_p,
                rect_func = icvErodeRect_8u_C1R_p;
            else if(type == CC_8UC3)
                rect_getbufsize_func = icvErodeRect_GetBufSize_8u_C3R_p,
                rect_func = icvErodeRect_8u_C3R_p;
            else if(type == CC_8UC4)
                rect_getbufsize_func = icvErodeRect_GetBufSize_8u_C4R_p,
                rect_func = icvErodeRect_8u_C4R_p;
            else if(type == CC_32F && cn==1)
                rect_getbufsize_func = icvErodeRect_GetBufSize_32f_C1R_p,
                rect_func = icvErodeRect_32f_C1R_p;
            else if(type == CC_32FC3)
                rect_getbufsize_func = icvErodeRect_GetBufSize_32f_C3R_p,
                rect_func = icvErodeRect_32f_C3R_p;
            else if(type == CC_32FC4)
                rect_getbufsize_func = icvErodeRect_GetBufSize_32f_C4R_p,
                rect_func = icvErodeRect_32f_C4R_p;
        }
        else
        {
            if(type == CC_8UC1)
                rect_getbufsize_func = icvDilateRect_GetBufSize_8u_C1R_p,
                rect_func = icvDilateRect_8u_C1R_p;
            else if(type == CC_8UC3)
                rect_getbufsize_func = icvDilateRect_GetBufSize_8u_C3R_p,
                rect_func = icvDilateRect_8u_C3R_p;
            else if(type == CC_8UC4)
                rect_getbufsize_func = icvDilateRect_GetBufSize_8u_C4R_p,
                rect_func = icvDilateRect_8u_C4R_p;
            else if(type == CC_32F && cn==1)
                rect_getbufsize_func = icvDilateRect_GetBufSize_32f_C1R_p,
                rect_func = icvDilateRect_32f_C1R_p;
            else if(type == CC_32FC3)
                rect_getbufsize_func = icvDilateRect_GetBufSize_32f_C3R_p,
                rect_func = icvDilateRect_32f_C3R_p;
            else if(type == CC_32FC4)
                rect_getbufsize_func = icvDilateRect_GetBufSize_32f_C4R_p,
                rect_func = icvDilateRect_32f_C4R_p;
        }

        if(rect_getbufsize_func && rect_func)
        {
            int bufsize = 0;

            CStatus status = rect_getbufsize_func(size.width, el_size, &bufsize);
            if(status >= 0 && bufsize > 0)
            {
                if(bufsize < CC_MAX_LOCAL_SIZE)
                {
                    buffer = cvStackAlloc(bufsize);
                    local_alloc = 1;
                }
                else
                    CC_CALL(buffer = cAlloc(bufsize));
            }

            if(status >= 0)
            {
                int src_step, dst_step = dst->step ? dst->step : CC_STUB_STEP;

                if(inplace)
                {
                    CC_CALL(temp = cvCloneMat(dst));
                    src = temp;
                }
                src_step = src->step ? src->step : CC_STUB_STEP;

                status = rect_func(src->tt.data, src_step, dst->tt.data,
                                    dst_step, size, el_size, el_anchor, buffer);
            }
            
            if(status >= 0)
                EXIT;
        }
    }
    else if(el_shape == CC_SHAPE_CUSTOM && icvMorphInitAlloc_8u_C1R_p && icvMorphFree_p &&
             src->tt.data != dst->tt.data)
    {
        CvMorphCustomFunc_IPP custom_func = 0;
        CvMorphCustomInitAllocFunc_IPP custom_initalloc_func = 0;
        const int bordertype = 1; // replication border

        if(type == CC_8UC1)
            custom_initalloc_func = icvMorphInitAlloc_8u_C1R_p,
            custom_func = mop == 0 ? icvErode_8u_C1R_p : icvDilate_8u_C1R_p;
        else if(type == CC_8UC3)
            custom_initalloc_func = icvMorphInitAlloc_8u_C3R_p,
            custom_func = mop == 0 ? icvErode_8u_C3R_p : icvDilate_8u_C3R_p;
        else if(type == CC_8UC4)
            custom_initalloc_func = icvMorphInitAlloc_8u_C4R_p,
            custom_func = mop == 0 ? icvErode_8u_C4R_p : icvDilate_8u_C4R_p;
        else if(type == CC_32F && cn==1)
            custom_initalloc_func = icvMorphInitAlloc_32f_C1R_p,
            custom_func = mop == 0 ? icvErode_32f_C1R_p : icvDilate_32f_C1R_p;
        else if(type == CC_32FC3)
            custom_initalloc_func = icvMorphInitAlloc_32f_C3R_p,
            custom_func = mop == 0 ? icvErode_32f_C3R_p : icvDilate_32f_C3R_p;
        else if(type == CC_32FC4)
            custom_initalloc_func = icvMorphInitAlloc_32f_C4R_p,
            custom_func = mop == 0 ? icvErode_32f_C4R_p : icvDilate_32f_C4R_p;

        if(custom_initalloc_func && custom_func)
        {
            uchar *src_ptr, *dst_ptr = dst->tt.data;
            int src_step, dst_step = dst->step ? dst->step : CC_STUB_STEP;
            int el_len = el_size.width*el_size.height;
            uchar* el_mask = (uchar*)cvStackAlloc(el_len);
            CStatus status;

            for(i = 0; i < el_len; i++)
                el_mask[i] = (uchar)(element->values[i] != 0);

            status = custom_initalloc_func(size.width, el_mask, el_size,
                                            el_anchor, &morphstate);

            if(status >= 0 && (inplace || iterations > 1))
            {
                CC_CALL(temp = cvCloneMat(dst));
                src = temp;
            }

            src_ptr = src->tt.data;
            src_step = src->step ? src->step : CC_STUB_STEP;

            for(i = 0; i < iterations && status >= 0 && morphstate; i++)
            {
                uchar* t_ptr;
                int t_step;
                status = custom_func(src_ptr, src_step, dst_ptr, dst_step,
                                      size, bordertype, morphstate);
                CC_SWAP(src_ptr, dst_ptr, t_ptr);
                CC_SWAP(src_step, dst_step, t_step);
                if(i == 0 && temp)
                {
                    dst_ptr = temp->tt.data;
                    dst_step = temp->step ? temp->step : CC_STUB_STEP;
                }
            }

            if(status >= 0)
            {
                if(iterations % 2 == 0)
                    cvCopy(temp, dst);
                EXIT;
            }
        }
    }

    if(el_shape != CC_SHAPE_RECT)
    {
        el_hdr = cvMat(element->nRows, element->nCols, CC_32SC1, element->values);
        el = &el_hdr;
        el_shape = CC_SHAPE_CUSTOM;
    }

    CC_CALL(morphology.init(mop, src->cols, src->tid,
                    el_shape, el, el_size, el_anchor));

    for(i = 0; i < iterations; i++)
    {
        CC_CALL(morphology.process(src, dst));
        src = dst;
    }

    __END__;

    if(!local_alloc)
        cFree(&buffer);
    if(morphstate)
        icvMorphFree_p(morphstate);
    cvReleaseMat(&temp);
}


CC_IMPL void
cvErode(const void* src, void* dst, IplConvKernel* element, int iterations)
{
    icvMorphOp(src, dst, element, iterations, 0);
}


CC_IMPL void
cvDilate(const void* src, void* dst, IplConvKernel* element, int iterations)
{
    icvMorphOp(src, dst, element, iterations, 1);
}


CC_IMPL void
cvMorphologyEx(const void* src, void* dst,
                void* temp, IplConvKernel* element, int op, int iterations)
{
    CC_FUNCNAME("cvMorhologyEx");

    __BEGIN__;

    if((op == CC_MOP_GRADIENT ||
        (op == CC_MOP_TOPHAT || op == CC_MOP_BLACKHAT) && src == dst) && temp == 0)
        CC_ERROR(CC_HeaderIsNull, "temp image required");

    if(temp == src || temp == dst)
        CC_ERROR(CC_HeaderIsNull, "temp image is equal to src or dst");

    switch (op)
    {
    case CC_MOP_OPEN:
        CC_CALL(cvErode(src, dst, element, iterations));
        CC_CALL(cvDilate(dst, dst, element, iterations));
        break;
    case CC_MOP_CLOSE:
        CC_CALL(cvDilate(src, dst, element, iterations));
        CC_CALL(cvErode(dst, dst, element, iterations));
        break;
    case CC_MOP_GRADIENT:
        CC_CALL(cvErode(src, temp, element, iterations));
        CC_CALL(cvDilate(src, dst, element, iterations));
        CC_CALL(cvSub(dst, temp, dst));
        break;
    case CC_MOP_TOPHAT:
        if(src != dst)
            temp = dst;
        CC_CALL(cvErode(src, temp, element, iterations));
        CC_CALL(cvDilate(temp, temp, element, iterations));
        CC_CALL(cvSub(src, temp, dst));
        break;
    case CC_MOP_BLACKHAT:
        if(src != dst)
            temp = dst;
        CC_CALL(cvDilate(src, temp, element, iterations));
        CC_CALL(cvErode(temp, temp, element, iterations));
        CC_CALL(cvSub(temp, src, dst));
        break;
    default:
        CC_ERROR(CC_StsBadArg, "unknown morphological operation");
    }

    __END__;
}


