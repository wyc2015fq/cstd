
/**************************************************************************************\
*                                   line samplers                                      *
\**************************************************************************************/

CC_IMPL int
cvSampleLine(const img_t* mat, CPoint pt1, CPoint pt2,
              void* _buffer, int connectivity)
{
    int count = -1;
    
    CC_FUNCNAME("cvSampleLine");

    __BEGIN__;
    
    int i, coi = 0, pix_size;
    CLineIterator iterator;
    uchar* buffer = (uchar*)_buffer;

    if(coi != 0)
        CC_ERROR(CC_BadCOI, "");

    if(!buffer)
        CC_ERROR(CC_StsNullPtr, "");

    CC_CALL(count = cvInitLineIterator(mat, pt1, pt2, &iterator, connectivity));

    pix_size = CC_ELEM_SIZE(mat->tid);
    for(i = 0; i < count; i++)
    {
        CC_MEMCPY_AUTO(buffer, iterator.ptr, pix_size);
        buffer += pix_size;
        CC_NEXT_LINE_POINT(iterator);
    }

    __END__;

    return count;
}


static const void*
icvAdjustRect(const void* srcptr, int src_step, int pix_size,
               CSize src_size, CSize win_size,
               CPoint ip, CRect* pRect)
{
    CRect rect;
    const char* src = (const char*)srcptr;

    if(ip.x >= 0)
    {
        src += ip.x*pix_size;
        rect.x = 0;
    }
    else
    {
        rect.x = -ip.x;
        if(rect.x > win_size.width)
            rect.x = win_size.width;
    }

    if(ip.x + win_size.width < src_size.width)
        rect.width = win_size.width;
    else
    {
        rect.width = src_size.width - ip.x - 1;
        if(rect.width < 0)
        {
            src += rect.width*pix_size;
            rect.width = 0;
        }
        assert(rect.width <= win_size.width);
    }

    if(ip.y >= 0)
    {
        src += ip.y * src_step;
        rect.y = 0;
    }
    else
        rect.y = -ip.y;

    if(ip.y + win_size.height < src_size.height)
        rect.height = win_size.height;
    else
    {
        rect.height = src_size.height - ip.y - 1;
        if(rect.height < 0)
        {
            src += rect.height*src_step;
            rect.height = 0;
        }
    }

    *pRect = rect;
    return src - rect.x*pix_size;
}


#define  ICC_DEF_GET_RECT_SUB_PIX_FUNC(flavor, srctype, dsttype, worktype, \
                                        cast_macro, scale_macro, cast_macro2)\
CStatus C_STDCALL icvGetRectSubPix_##flavor##_C1R                         \
(const srctype* src, int src_step, CSize src_size,                        \
  dsttype* dst, int dst_step, CSize win_size, CPoint2D32f center)        \
{                                                                           \
    CPoint ip;                                                             \
    worktype  a11, a12, a21, a22, b1, b2;                                   \
    float a, b;                                                             \
    int i, j;                                                               \
                                                                            \
    center.x -= (win_size.width-1)*0.5f;                                    \
    center.y -= (win_size.height-1)*0.5f;                                   \
                                                                            \
    ip.x = cFloor(center.x);                                             \
    ip.y = cFloor(center.y);                                             \
                                                                            \
    a = center.x - ip.x;                                                    \
    b = center.y - ip.y;                                                    \
    a11 = scale_macro((1.f-a)*(1.f-b));                                     \
    a12 = scale_macro(a*(1.f-b));                                           \
    a21 = scale_macro((1.f-a)*b);                                           \
    a22 = scale_macro(a*b);                                                 \
    b1 = scale_macro(1.f - b);                                              \
    b2 = scale_macro(b);                                                    \
                                                                            \
    src_step /= sizeof(src[0]);                                             \
    dst_step /= sizeof(dst[0]);                                             \
                                                                            \
    if(0 <= ip.x && ip.x + win_size.width < src_size.width &&              \
        0 <= ip.y && ip.y + win_size.height < src_size.height)             \
    {                                                                       \
        /* extracted rectangle is totally inside the image */               \
        src += ip.y * src_step + ip.x;                                      \
                                                                            \
        if(icvCopySubpix_##flavor##_C1R_p &&                               \
            icvCopySubpix_##flavor##_C1R_p(src, src_step*sizeof(src[0]),   \
                                            dst, dst_step*sizeof(dst[0]),   \
                                            win_size, a, b) >= 0)         \
            return CC_OK;                                                   \
                                                                            \
        for(i = 0; i < win_size.height; i++, src += src_step,              \
                                              dst += dst_step)             \
        {                                                                   \
            for(j = 0; j <= win_size.width - 2; j += 2)                   \
            {                                                               \
                worktype s0 = cast_macro(src[j])*a11 +                      \
                              cast_macro(src[j+1])*a12 +                    \
                              cast_macro(src[j+src_step])*a21 +             \
                              cast_macro(src[j+src_step+1])*a22;            \
                worktype s1 = cast_macro(src[j+1])*a11 +                    \
                              cast_macro(src[j+2])*a12 +                    \
                              cast_macro(src[j+src_step+1])*a21 +           \
                              cast_macro(src[j+src_step+2])*a22;            \
                                                                            \
                dst[j] = (dsttype)cast_macro2(s0);                          \
                dst[j+1] = (dsttype)cast_macro2(s1);                        \
            }                                                               \
                                                                            \
            for(; j < win_size.width; j++)                                \
            {                                                               \
                worktype s0 = cast_macro(src[j])*a11 +                      \
                              cast_macro(src[j+1])*a12 +                    \
                              cast_macro(src[j+src_step])*a21 +             \
                              cast_macro(src[j+src_step+1])*a22;            \
                                                                            \
                dst[j] = (dsttype)cast_macro2(s0);                          \
            }                                                               \
        }                                                                   \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        CRect r;                                                           \
                                                                            \
        src = (const srctype*)icvAdjustRect(src, src_step*sizeof(*src),    \
                               sizeof(*src), src_size, win_size,ip, &r);    \
                                                                            \
        for(i = 0; i < win_size.height; i++, dst += dst_step)             \
        {                                                                   \
            const srctype *src2 = src + src_step;                           \
                                                                            \
            if(i < r.y || i >= r.height)                                  \
                src2 -= src_step;                                           \
                                                                            \
            for(j = 0; j < r.x; j++)                                      \
            {                                                               \
                worktype s0 = cast_macro(src[r.x])*b1 +                     \
                              cast_macro(src2[r.x])*b2;                     \
                                                                            \
                dst[j] = (dsttype)cast_macro2(s0);                          \
            }                                                               \
                                                                            \
            for(; j < r.width; j++)                                       \
            {                                                               \
                worktype s0 = cast_macro(src[j])*a11 +                      \
                              cast_macro(src[j+1])*a12 +                    \
                              cast_macro(src2[j])*a21 +                     \
                              cast_macro(src2[j+1])*a22;                    \
                                                                            \
                dst[j] = (dsttype)cast_macro2(s0);                          \
            }                                                               \
                                                                            \
            for(; j < win_size.width; j++)                                \
            {                                                               \
                worktype s0 = cast_macro(src[r.width])*b1 +                 \
                              cast_macro(src2[r.width])*b2;                 \
                                                                            \
                dst[j] = (dsttype)cast_macro2(s0);                          \
            }                                                               \
                                                                            \
            if(i < r.height)                                              \
                src = src2;                                                 \
        }                                                                   \
    }                                                                       \
                                                                            \
    return CC_OK;                                                           \
}


#define  ICC_DEF_GET_RECT_SUB_PIX_FUNC_C3(flavor, srctype, dsttype, worktype, \
                                        cast_macro, scale_macro, mul_macro)\
static CStatus C_STDCALL icvGetRectSubPix_##flavor##_C3R                  \
(const srctype* src, int src_step, CSize src_size,                        \
  dsttype* dst, int dst_step, CSize win_size, CPoint2D32f center)        \
{                                                                           \
    CPoint ip;                                                             \
    worktype a, b;                                                          \
    int i, j;                                                               \
                                                                            \
    center.x -= (win_size.width-1)*0.5f;                                    \
    center.y -= (win_size.height-1)*0.5f;                                   \
                                                                            \
    ip.x = cFloor(center.x);                                             \
    ip.y = cFloor(center.y);                                             \
                                                                            \
    a = scale_macro(center.x - ip.x);                                     \
    b = scale_macro(center.y - ip.y);                                     \
                                                                            \
    src_step /= sizeof(src[0]);                                           \
    dst_step /= sizeof(dst[0]);                                           \
                                                                            \
    if(0 <= ip.x && ip.x + win_size.width < src_size.width &&              \
        0 <= ip.y && ip.y + win_size.height < src_size.height)             \
    {                                                                       \
        /* extracted rectangle is totally inside the image */               \
        src += ip.y * src_step + ip.x*3;                                    \
                                                                            \
        for(i = 0; i < win_size.height; i++, src += src_step,              \
                                              dst += dst_step)             \
        {                                                                   \
            for(j = 0; j < win_size.width; j++)                           \
            {                                                               \
                worktype s0 = cast_macro(src[j*3]);                         \
                worktype s1 = cast_macro(src[j*3 + src_step]);              \
                s0 += mul_macro(a, (cast_macro(src[j*3+3]) - s0));         \
                s1 += mul_macro(a, (cast_macro(src[j*3+3+src_step]) - s1));\
                dst[j*3] = (dsttype)(s0 + mul_macro(b, (s1 - s0)));        \
                                                                            \
                s0 = cast_macro(src[j*3+1]);                                \
                s1 = cast_macro(src[j*3+1 + src_step]);                     \
                s0 += mul_macro(a, (cast_macro(src[j*3+4]) - s0));         \
                s1 += mul_macro(a, (cast_macro(src[j*3+4+src_step]) - s1));\
                dst[j*3+1] = (dsttype)(s0 + mul_macro(b, (s1 - s0)));      \
                                                                            \
                s0 = cast_macro(src[j*3+2]);                                \
                s1 = cast_macro(src[j*3+2 + src_step]);                     \
                s0 += mul_macro(a, (cast_macro(src[j*3+5]) - s0));         \
                s1 += mul_macro(a, (cast_macro(src[j*3+5+src_step]) - s1));\
                dst[j*3+2] = (dsttype)(s0 + mul_macro(b, (s1 - s0)));      \
            }                                                               \
        }                                                                   \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        CRect r;                                                           \
                                                                            \
        src = (const srctype*)icvAdjustRect(src, src_step*sizeof(*src),    \
                             sizeof(*src)*3, src_size, win_size, ip, &r);  \
                                                                            \
        for(i = 0; i < win_size.height; i++, dst += dst_step)             \
        {                                                                   \
            const srctype *src2 = src + src_step;                           \
                                                                            \
            if(i < r.y || i >= r.height)                                  \
                src2 -= src_step;                                           \
                                                                            \
            for(j = 0; j < r.x; j++)                                      \
            {                                                               \
                worktype s0 = cast_macro(src[r.x*3]);                       \
                worktype s1 = cast_macro(src2[r.x*3]);                      \
                dst[j*3] = (dsttype)(s0 + mul_macro(b, (s1 - s0)));        \
                                                                            \
                s0 = cast_macro(src[r.x*3+1]);                              \
                s1 = cast_macro(src2[r.x*3+1]);                             \
                dst[j*3+1] = (dsttype)(s0 + mul_macro(b, (s1 - s0)));      \
                                                                            \
                s0 = cast_macro(src[r.x*3+2]);                              \
                s1 = cast_macro(src2[r.x*3+2]);                             \
                dst[j*3+2] = (dsttype)(s0 + mul_macro(b, (s1 - s0)));      \
            }                                                               \
                                                                            \
            for(; j < r.width; j++)                                       \
            {                                                               \
                worktype s0 = cast_macro(src[j*3]);                         \
                worktype s1 = cast_macro(src2[j*3]);                        \
                s0 += mul_macro(a, (cast_macro(src[j*3 + 3]) - s0));       \
                s1 += mul_macro(a, (cast_macro(src2[j*3 + 3]) - s1));      \
                dst[j*3] = (dsttype)(s0 + mul_macro(b, (s1 - s0)));        \
                                                                            \
                s0 = cast_macro(src[j*3+1]);                                \
                s1 = cast_macro(src2[j*3+1]);                               \
                s0 += mul_macro(a, (cast_macro(src[j*3 + 4]) - s0));       \
                s1 += mul_macro(a, (cast_macro(src2[j*3 + 4]) - s1));      \
                dst[j*3+1] = (dsttype)(s0 + mul_macro(b, (s1 - s0)));      \
                                                                            \
                s0 = cast_macro(src[j*3+2]);                                \
                s1 = cast_macro(src2[j*3+2]);                               \
                s0 += mul_macro(a, (cast_macro(src[j*3 + 5]) - s0));       \
                s1 += mul_macro(a, (cast_macro(src2[j*3 + 5]) - s1));      \
                dst[j*3+2] = (dsttype)(s0 + mul_macro(b, (s1 - s0)));      \
            }                                                               \
                                                                            \
            for(; j < win_size.width; j++)                                \
            {                                                               \
                worktype s0 = cast_macro(src[r.width*3]);                   \
                worktype s1 = cast_macro(src2[r.width*3]);                  \
                dst[j*3] = (dsttype)(s0 + mul_macro(b, (s1 - s0)));        \
                                                                            \
                s0 = cast_macro(src[r.width*3+1]);                          \
                s1 = cast_macro(src2[r.width*3+1]);                         \
                dst[j*3+1] = (dsttype)(s0 + mul_macro(b, (s1 - s0)));      \
                                                                            \
                s0 = cast_macro(src[r.width*3+2]);                          \
                s1 = cast_macro(src2[r.width*3+2]);                         \
                dst[j*3+2] = (dsttype)(s0 + mul_macro(b, (s1 - s0)));      \
            }                                                               \
                                                                            \
            if(i < r.height)                                              \
                src = src2;                                                 \
        }                                                                   \
    }                                                                       \
                                                                            \
    return CC_OK;                                                           \
}



CStatus C_STDCALL icvGetRectSubPix_8u32f_C1R
(const uchar* src, int src_step, CSize src_size,
  float* dst, int dst_step, CSize win_size, CPoint2D32f center)
{
    CPoint ip;
    float  a12, a22, b1, b2;
    float a, b;
    double s = 0;
    int i, j;

    center.x -= (win_size.width-1)*0.5f;
    center.y -= (win_size.height-1)*0.5f;

    ip.x = cFloor(center.x);
    ip.y = cFloor(center.y);

    if(win_size.width <= 0 || win_size.height <= 0)
        return CC_BADRANGE_ERR;

    a = center.x - ip.x;
    b = center.y - ip.y;
    a = MAX(a,0.0001f);
    a12 = a*(1.f-b);
    a22 = a*b;
    b1 = 1.f - b;
    b2 = b;
    s = (1. - a)/a;

    src_step /= sizeof(src[0]);
    dst_step /= sizeof(dst[0]);

    if(0 <= ip.x && ip.x + win_size.width < src_size.width &&
        0 <= ip.y && ip.y + win_size.height < src_size.height)
    {
        // extracted rectangle is totally inside the image
        src += ip.y * src_step + ip.x;

#if 0
        if(icvCopySubpix_8u32f_C1R_p &&
            icvCopySubpix_8u32f_C1R_p(src, src_step, dst,
                dst_step*sizeof(dst[0]), win_size, a, b) >= 0)
            return CC_OK;
#endif

        for(; win_size.height--; src += src_step, dst += dst_step)
        {
            float prev = (1 - a)*(b1*CC_8TO32F(src[0]) + b2*CC_8TO32F(src[src_step]));
            for(j = 0; j < win_size.width; j++)
            {
                float t = a12*CC_8TO32F(src[j+1]) + a22*CC_8TO32F(src[j+1+src_step]);
                dst[j] = prev + t;
                prev = (float)(t*s);
            }
        }
    }
    else
    {
        CRect r;

        src = (const uchar*)icvAdjustRect(src, src_step*sizeof(*src),
                               sizeof(*src), src_size, win_size,ip, &r);

        for(i = 0; i < win_size.height; i++, dst += dst_step)
        {
            const uchar *src2 = src + src_step;

            if(i < r.y || i >= r.height)
                src2 -= src_step;

            for(j = 0; j < r.x; j++)
            {
                float s0 = CC_8TO32F(src[r.x])*b1 +
                           CC_8TO32F(src2[r.x])*b2;

                dst[j] = (float)(s0);
            }

            if(j < r.width)
            {
                float prev = (1 - a)*(b1*CC_8TO32F(src[j]) + b2*CC_8TO32F(src2[j]));

                for(; j < r.width; j++)
                {
                    float t = a12*CC_8TO32F(src[j+1]) + a22*CC_8TO32F(src2[j+1]);
                    dst[j] = prev + t;
                    prev = (float)(t*s);
                }
            }

            for(; j < win_size.width; j++)
            {
                float s0 = CC_8TO32F(src[r.width])*b1 +
                           CC_8TO32F(src2[r.width])*b2;

                dst[j] = (float)(s0);
            }

            if(i < r.height)
                src = src2;
        }
    }

    return CC_OK;
}



#define ICC_SHIFT             16
#define ICC_SCALE(x)          cRound((x)*(1 << ICC_SHIFT))
#define ICC_MUL_SCALE(x,y)    (((x)*(y) + (1 << (ICC_SHIFT-1))) >> ICC_SHIFT)
#define ICC_DESCALE(x)        (((x)+(1 << (ICC_SHIFT-1))) >> ICC_SHIFT)

icvCopySubpix_8u_C1R_t icvCopySubpix_8u_C1R_p = 0;
icvCopySubpix_8u32f_C1R_t icvCopySubpix_8u32f_C1R_p = 0;
icvCopySubpix_32f_C1R_t icvCopySubpix_32f_C1R_p = 0;

ICC_DEF_GET_RECT_SUB_PIX_FUNC(8u, uchar, uchar, int, CC_NOP, ICC_SCALE, ICC_DESCALE)
//ICC_DEF_GET_RECT_SUB_PIX_FUNC(8u32f, uchar, float, float, CC_8TO32F, CC_NOP, CC_NOP)
ICC_DEF_GET_RECT_SUB_PIX_FUNC(32f, float, float, float, CC_NOP, CC_NOP, CC_NOP)

ICC_DEF_GET_RECT_SUB_PIX_FUNC_C3(8u, uchar, uchar, int, CC_NOP, ICC_SCALE, ICC_MUL_SCALE)
ICC_DEF_GET_RECT_SUB_PIX_FUNC_C3(8u32f, uchar, float, float, CC_8TO32F, CC_NOP, CC_MUL)
ICC_DEF_GET_RECT_SUB_PIX_FUNC_C3(32f, float, float, float, CC_NOP, CC_NOP, CC_MUL)


#define  ICC_DEF_INIT_SUBPIX_TAB(FUNCNAME, FLAG)                  \
static void icvInit##FUNCNAME##FLAG##Table(CvFuncTable  tab)      \
{                                                                   \
    tab[CC_8U] = (void*)icv##FUNCNAME##_8u_##FLAG;           \
    tab[CC_32F] = (void*)icv##FUNCNAME##_32f_##FLAG;         \
                                                                    \
    tab[1] = (void*)icv##FUNCNAME##_8u32f_##FLAG;            \
}


ICC_DEF_INIT_SUBPIX_TAB(GetRectSubPix, C1R)
ICC_DEF_INIT_SUBPIX_TAB(GetRectSubPix, C3R)

typedef CStatus (C_STDCALL *CvGetRectSubPixFunc)(const void* src, int src_step,
                                                    CSize src_size, void* dst,
                                                    int dst_step, CSize win_size,
                                                    CPoint2D32f center);

CC_IMPL void
cvGetRectSubPix(const img_t* src, img_t* dst, CPoint2D32f center)
{
    static CvFuncTable gr_tab[2];
    static int inittab = 0;
    CC_FUNCNAME("cvGetRectSubPix");

    __BEGIN__;
    
    CSize src_size, dst_size;
    CvGetRectSubPixFunc func;
    int cn, src_step, dst_step;
    TypeId type;

    if(!inittab)
    {
        icvInitGetRectSubPixC1RTable(gr_tab[0]);
        icvInitGetRectSubPixC3RTable(gr_tab[1]);
        inittab = 1;
    }

    cn = CC_MAT_CN(src);
    type = CC_MAT_DEPTH(src);

    if((cn != 1 && cn != 3) || !CC_ARE_CNS_EQ(src, dst))
        CC_ERROR(CC_StsUnsupportedFormat, "");

    src_size = cvGetMatSize(src);
    dst_size = cvGetMatSize(dst);
    src_step = src->step ? src->step : CC_STUB_STEP;
    dst_step = dst->step ? dst->step : CC_STUB_STEP;

    if(dst_size.width > src_size.width || dst_size.height > src_size.height)
        CC_ERROR(CC_StsBadSize, "destination ROI must be smaller than source ROI");

    if(CC_ARE_DEPTHS_EQ(src, dst))
    {
        func = (CvGetRectSubPixFunc)(gr_tab[cn != 1][type]);
    }
    else
    {
        if(CC_MAT_TYPE(src) != CC_8U || CC_MAT_TYPE(dst) != CC_32F)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        func = (CvGetRectSubPixFunc)(gr_tab[cn != 1][1]);
    }

    if(!func)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    IPPI_CALL(func(src->tt.data, src_step, src_size,
                     dst->tt.data, dst_step, dst_size, center));

    __END__;
}


#define ICC_32F8U(x)  ((uchar)cRound(x))

#define ICC_DEF_GET_QUADRANGLE_SUB_PIX_FUNC(flavor, srctype, dsttype,      \
                                             worktype, cast_macro, cvt)    \
static CStatus C_STDCALL                                                  \
icvGetQuadrangleSubPix_##flavor##_C1R                                       \
(const srctype * src, int src_step, CSize src_size,                       \
  dsttype *dst, int dst_step, CSize win_size, const float *matrix)        \
{                                                                           \
    int x, y;                                                               \
    double dx = (win_size.width - 1)*0.5;                                   \
    double dy = (win_size.height - 1)*0.5;                                  \
    double A11 = matrix[0], A12 = matrix[1], A13 = matrix[2]-A11*dx-A12*dy; \
    double A21 = matrix[3], A22 = matrix[4], A23 = matrix[5]-A21*dx-A22*dy; \
                                                                            \
    src_step /= sizeof(srctype);                                            \
    dst_step /= sizeof(dsttype);                                            \
                                                                            \
    for(y = 0; y < win_size.height; y++, dst += dst_step)                 \
    {                                                                       \
        double xs = A12*y + A13;                                            \
        double ys = A22*y + A23;                                            \
        double xe = A11*(win_size.width-1) + A12*y + A13;                   \
        double ye = A21*(win_size.width-1) + A22*y + A23;                   \
                                                                            \
        if((unsigned)(cFloor(xs)-1) < (unsigned)(src_size.width - 3) &&   \
            (unsigned)(cFloor(ys)-1) < (unsigned)(src_size.height - 3) &&  \
            (unsigned)(cFloor(xe)-1) < (unsigned)(src_size.width - 3) &&   \
            (unsigned)(cFloor(ye)-1) < (unsigned)(src_size.height - 3))    \
        {                                                                   \
            for(x = 0; x < win_size.width; x++)                           \
            {                                                               \
                int ixs = cFloor(xs);                                    \
                int iys = cFloor(ys);                                    \
                const srctype *ptr = src + src_step*iys + ixs;              \
                double a = xs - ixs, b = ys - iys, a1 = 1.f - a;            \
                worktype p0 = cvt(ptr[0])*a1 + cvt(ptr[1])*a;               \
                worktype p1 = cvt(ptr[src_step])*a1 + cvt(ptr[src_step+1])*a;\
                xs += A11;                                                  \
                ys += A21;                                                  \
                                                                            \
                dst[x] = cast_macro(p0 + b * (p1 - p0));                    \
            }                                                               \
        }                                                                   \
        else                                                                \
        {                                                                   \
            for(x = 0; x < win_size.width; x++)                           \
            {                                                               \
                int ixs = cFloor(xs), iys = cFloor(ys);               \
                double a = xs - ixs, b = ys - iys, a1 = 1.f - a;            \
                const srctype *ptr0, *ptr1;                                 \
                worktype p0, p1;                                            \
                xs += A11; ys += A21;                                       \
                                                                            \
                if((unsigned)iys < (unsigned)(src_size.height-1))         \
                    ptr0 = src + src_step*iys, ptr1 = ptr0 + src_step;      \
                else                                                        \
                    ptr0 = ptr1 = src + (iys < 0 ? 0 : src_size.height-1)*src_step; \
                                                                            \
                if((unsigned)ixs < (unsigned)(src_size.width-1))          \
                {                                                           \
                    p0 = cvt(ptr0[ixs])*a1 + cvt(ptr0[ixs+1])*a;            \
                    p1 = cvt(ptr1[ixs])*a1 + cvt(ptr1[ixs+1])*a;            \
                }                                                           \
                else                                                        \
                {                                                           \
                    ixs = ixs < 0 ? 0 : src_size.width - 1;                 \
                    p0 = cvt(ptr0[ixs]); p1 = cvt(ptr1[ixs]);               \
                }                                                           \
                dst[x] = cast_macro(p0 + b * (p1 - p0));                    \
            }                                                               \
        }                                                                   \
    }                                                                       \
                                                                            \
    return CC_OK;                                                           \
}


#define ICC_DEF_GET_QUADRANGLE_SUB_PIX_FUNC_C3(flavor, srctype, dsttype,   \
                                                worktype, cast_macro, cvt) \
static CStatus C_STDCALL                                                  \
icvGetQuadrangleSubPix_##flavor##_C3R                                       \
(const srctype * src, int src_step, CSize src_size,                       \
  dsttype *dst, int dst_step, CSize win_size, const float *matrix)        \
{                                                                           \
    int x, y;                                                               \
    double dx = (win_size.width - 1)*0.5;                                   \
    double dy = (win_size.height - 1)*0.5;                                  \
    double A11 = matrix[0], A12 = matrix[1], A13 = matrix[2]-A11*dx-A12*dy; \
    double A21 = matrix[3], A22 = matrix[4], A23 = matrix[5]-A21*dx-A22*dy; \
                                                                            \
    src_step /= sizeof(srctype);                                            \
    dst_step /= sizeof(dsttype);                                            \
                                                                            \
    for(y = 0; y < win_size.height; y++, dst += dst_step)                 \
    {                                                                       \
        double xs = A12*y + A13;                                            \
        double ys = A22*y + A23;                                            \
        double xe = A11*(win_size.width-1) + A12*y + A13;                   \
        double ye = A21*(win_size.width-1) + A22*y + A23;                   \
                                                                            \
        if((unsigned)(cFloor(xs)-1) < (unsigned)(src_size.width - 3) &&   \
            (unsigned)(cFloor(ys)-1) < (unsigned)(src_size.height - 3) &&  \
            (unsigned)(cFloor(xe)-1) < (unsigned)(src_size.width - 3) &&   \
            (unsigned)(cFloor(ye)-1) < (unsigned)(src_size.height - 3))    \
        {                                                                   \
            for(x = 0; x < win_size.width; x++)                           \
            {                                                               \
                int ixs = cFloor(xs);                                    \
                int iys = cFloor(ys);                                    \
                const srctype *ptr = src + src_step*iys + ixs*3;            \
                double a = xs - ixs, b = ys - iys, a1 = 1.f - a;            \
                worktype p0, p1;                                            \
                xs += A11;                                                  \
                ys += A21;                                                  \
                                                                            \
                p0 = cvt(ptr[0])*a1 + cvt(ptr[3])*a;                        \
                p1 = cvt(ptr[src_step])*a1 + cvt(ptr[src_step+3])*a;        \
                dst[x*3] = cast_macro(p0 + b * (p1 - p0));                  \
                                                                            \
                p0 = cvt(ptr[1])*a1 + cvt(ptr[4])*a;                        \
                p1 = cvt(ptr[src_step+1])*a1 + cvt(ptr[src_step+4])*a;      \
                dst[x*3+1] = cast_macro(p0 + b * (p1 - p0));                \
                                                                            \
                p0 = cvt(ptr[2])*a1 + cvt(ptr[5])*a;                        \
                p1 = cvt(ptr[src_step+2])*a1 + cvt(ptr[src_step+5])*a;      \
                dst[x*3+2] = cast_macro(p0 + b * (p1 - p0));                \
            }                                                               \
        }                                                                   \
        else                                                                \
        {                                                                   \
            for(x = 0; x < win_size.width; x++)                           \
            {                                                               \
                int ixs = cFloor(xs), iys = cFloor(ys);                   \
                double a = xs - ixs, b = ys - iys;                          \
                const srctype *ptr0, *ptr1;                                 \
                xs += A11; ys += A21;                                       \
                                                                            \
                if((unsigned)iys < (unsigned)(src_size.height-1))         \
                    ptr0 = src + src_step*iys, ptr1 = ptr0 + src_step;      \
                else                                                        \
                    ptr0 = ptr1 = src + (iys < 0 ? 0 : src_size.height-1)*src_step; \
                                                                            \
                if((unsigned)ixs < (unsigned)(src_size.width - 1))        \
                {                                                           \
                    double a1 = 1.f - a;                                    \
                    worktype p0, p1;                                        \
                    ptr0 += ixs*3; ptr1 += ixs*3;                           \
                    p0 = cvt(ptr0[0])*a1 + cvt(ptr0[3])*a;                  \
                    p1 = cvt(ptr1[0])*a1 + cvt(ptr1[3])*a;                  \
                    dst[x*3] = cast_macro(p0 + b * (p1 - p0));              \
                                                                            \
                    p0 = cvt(ptr0[1])*a1 + cvt(ptr0[4])*a;                  \
                    p1 = cvt(ptr1[1])*a1 + cvt(ptr1[4])*a;                  \
                    dst[x*3+1] = cast_macro(p0 + b * (p1 - p0));            \
                                                                            \
                    p0 = cvt(ptr0[2])*a1 + cvt(ptr0[5])*a;                  \
                    p1 = cvt(ptr1[2])*a1 + cvt(ptr1[5])*a;                  \
                    dst[x*3+2] = cast_macro(p0 + b * (p1 - p0));            \
                }                                                           \
                else                                                        \
                {                                                           \
                    double b1 = 1.f - b;                                    \
                    ixs = ixs < 0 ? 0 : src_size.width - 1;                 \
                    ptr0 += ixs*3; ptr1 += ixs*3;                           \
                                                                            \
                    dst[x*3] = cast_macro(cvt(ptr0[0])*b1 + cvt(ptr1[0])*b);\
                    dst[x*3+1]=cast_macro(cvt(ptr0[1])*b1 + cvt(ptr1[1])*b);\
                    dst[x*3+2]=cast_macro(cvt(ptr0[2])*b1 + cvt(ptr1[2])*b);\
                }                                                           \
            }                                                               \
        }                                                                   \
    }                                                                       \
                                                                            \
    return CC_OK;                                                           \
}


/*#define srctype uchar
#define dsttype uchar
#define worktype float
#define cvt CC_8TO32F
#define cast_macro ICC_32F8U

#undef srctype
#undef dsttype
#undef worktype
#undef cvt
#undef cast_macro*/

ICC_DEF_GET_QUADRANGLE_SUB_PIX_FUNC(8u, uchar, uchar, double, ICC_32F8U, CC_8TO32F)
ICC_DEF_GET_QUADRANGLE_SUB_PIX_FUNC(32f, float, float, double, CC_CAST_32F, CC_NOP)
ICC_DEF_GET_QUADRANGLE_SUB_PIX_FUNC(8u32f, uchar, float, double, CC_CAST_32F, CC_8TO32F)

ICC_DEF_GET_QUADRANGLE_SUB_PIX_FUNC_C3(8u, uchar, uchar, double, ICC_32F8U, CC_8TO32F)
ICC_DEF_GET_QUADRANGLE_SUB_PIX_FUNC_C3(32f, float, float, double, CC_CAST_32F, CC_NOP)
ICC_DEF_GET_QUADRANGLE_SUB_PIX_FUNC_C3(8u32f, uchar, float, double, CC_CAST_32F, CC_8TO32F)

ICC_DEF_INIT_SUBPIX_TAB(GetQuadrangleSubPix, C1R)
ICC_DEF_INIT_SUBPIX_TAB(GetQuadrangleSubPix, C3R)

typedef CStatus (C_STDCALL *CvGetQuadrangleSubPixFunc)(
                                         const void* src, int src_step,
                                         CSize src_size, void* dst,
                                         int dst_step, CSize win_size,
                                         const float* matrix);

// m - "Transformation matrix must be 2x3");

CC_IMPL void
cvGetQuadrangleSubPix(const img_t* src, img_t* dst, const double6 mat)
{
    static  CvFuncTable  gq_tab[2];
    static  int inittab = 0;
    CC_FUNCNAME("cvGetQuadrangleSubPix");

    __BEGIN__;

    CSize src_size, dst_size;
    CvGetQuadrangleSubPixFunc func;
    float m[6];
    int k, cn;

    if (dst!=src) {
      TypeId dst_type = (src->tid==CC_32F || dst->tid!=CC_8U) ? CC_32F : CC_8U;
      cvSetMat(dst, dst->h, dst->w, dst_type);
      //dst->tid = dst_type;
    }

    if(!inittab)
    {
        icvInitGetQuadrangleSubPixC1RTable(gq_tab[0]);
        icvInitGetQuadrangleSubPixC3RTable(gq_tab[1]);
        inittab = 1;
    }

    cn = CC_MAT_CN(src);

    if((cn != 1 && cn != 3) || !CC_ARE_CNS_EQ(src, dst))
        CC_ERROR(CC_StsUnsupportedFormat, "");

    src_size = cvGetMatSize(src);
    dst_size = cvGetMatSize(dst);

    /*if(dst_size.width > src_size.width || dst_size.height > src_size.height)
        CC_ERROR(CC_StsBadSize, "destination ROI must not be larger than source ROI");*/


    {
        for(k = 0; k < 3; k++)
        {
            m[k] = (float)mat[k];
            m[3 + k] = (float)mat[3 + k];
        }
    }

    if(CC_ARE_DEPTHS_EQ(src, dst))
    {
        func = (CvGetQuadrangleSubPixFunc)(gq_tab[cn != 1][CC_MAT_TYPE(src)]);
    }
    else
    {
        if(CC_MAT_TYPE(src) != CC_8U || CC_MAT_TYPE(dst) != CC_32F)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        func = (CvGetQuadrangleSubPixFunc)(gq_tab[cn != 1][1]);
    }

    if(!func)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    IPPI_CALL(func(src->tt.data, src->step, src_size,
                     dst->tt.data, dst->step, dst_size, m));

    __END__;
}

#undef ICC_DEF_GET_RECT_SUB_PIX_FUNC
#undef ICC_DEF_GET_RECT_SUB_PIX_FUNC_C3
#undef ICC_SHIFT
#undef ICC_SCALE
#undef ICC_MUL_SCALE
#undef ICC_DESCALE
#undef ICC_DEF_INIT_SUBPIX_TAB
#undef ICC_32F8U
#undef ICC_DEF_GET_QUADRANGLE_SUB_PIX_FUNC
#undef ICC_DEF_GET_QUADRANGLE_SUB_PIX_FUNC_C3
#undef srctype
#undef dsttype
#undef worktype
#undef cvt
#undef cast_macro



