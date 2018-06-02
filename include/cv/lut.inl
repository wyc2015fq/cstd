
#ifndef _LUT_INL_
#define _LUT_INL_

/****************************************************************************************\
*                                    LUT Transform                                       *
\****************************************************************************************/

#define ICC_LUT_CASE_C1(type)             \
    for(i = 0; i <= size.width-4; i += 4) \
    {                                       \
        type t0 = lut[src[i]];              \
        type t1 = lut[src[i+1]];            \
        dst[i] = t0;                        \
        dst[i+1] = t1;                      \
                                            \
        t0 = lut[src[i+2]];                 \
        t1 = lut[src[i+3]];                 \
        dst[i+2] = t0;                      \
        dst[i+3] = t1;                      \
    }                                       \
                                            \
    for(; i < size.width; i++)            \
    {                                       \
        type t0 = lut[src[i]];              \
        dst[i] = t0;                        \
    }


#define ICC_LUT_CASE_C2(type)             \
    for(i = 0; i < size.width; i += 2)    \
    {                                       \
        type t0 = lut[src[i]*2];            \
        type t1 = lut[src[i+1]*2 + 1];      \
        dst[i] = t0;                        \
        dst[i+1] = t1;                      \
    }

#define ICC_LUT_CASE_C3(type)             \
    for(i = 0; i < size.width; i += 3)    \
    {                                       \
        type t0 = lut[src[i]*3];            \
        type t1 = lut[src[i+1]*3 + 1];      \
        type t2 = lut[src[i+2]*3 + 2];      \
        dst[i] = t0;                        \
        dst[i+1] = t1;                      \
        dst[i+2] = t2;                      \
    }

#define ICC_LUT_CASE_C4(type)             \
    for(i = 0; i < size.width; i += 4)    \
    {                                       \
        type t0 = lut[src[i]*4];            \
        type t1 = lut[src[i+1]*4 + 1];      \
        dst[i] = t0;                        \
        dst[i+1] = t1;                      \
        t0 = lut[src[i+2]*4 + 2];           \
        t1 = lut[src[i+3]*4 + 3];           \
        dst[i+2] = t0;                      \
        dst[i+3] = t1;                      \
    }


#define  ICC_DEF_LUT_FUNC_8U_CN(flavor, dsttype, cn)      \
static CStatus C_STDCALL icvLUT_Transform8u_##flavor##_C##cn##R(\
    const uchar* src, int srcstep,                          \
    dsttype* dst, int dststep, CSize size,                 \
    const dsttype* lut)                                    \
{                                                           \
    size.width *= cn;                                       \
    dststep /= sizeof(dst[0]);                              \
    for(; size.height--; src += srcstep, dst += dststep)  \
    {                                                       \
        int i;                                              \
        ICC_LUT_CASE_C##cn(dsttype)                       \
    }                                                       \
                                                            \
    return CC_OK;                                           \
}


ICC_DEF_LUT_FUNC_8U_CN(8u, uchar, 1)
ICC_DEF_LUT_FUNC_8U_CN(16u, ushort, 1)
ICC_DEF_LUT_FUNC_8U_CN(32s, int, 1)
ICC_DEF_LUT_FUNC_8U_CN(64f, double, 1)

ICC_DEF_LUT_FUNC_8U_CN(8u, uchar, 2)
ICC_DEF_LUT_FUNC_8U_CN(8u, uchar, 3)
ICC_DEF_LUT_FUNC_8U_CN(8u, uchar, 4)


#define  ICC_DEF_LUT_FUNC_8U(flavor, dsttype)             \
static CStatus C_STDCALL                                  \
icvLUT_Transform8u_##flavor##_CnR(             \
    const uchar* src, int srcstep,                          \
    dsttype* dst, int dststep, CSize size,                 \
    const dsttype* _lut, int cn)                           \
{                                                           \
    int max_block_size = (1 << 10)*cn;                      \
    dsttype lutp[1024];                                     \
    int i, k;                                               \
                                                            \
    size.width *= cn;                                       \
    dststep /= sizeof(dst[0]);                              \
                                                            \
    if(size.width*size.height < 256)                      \
    {                                                       \
        for(; size.height--; src+=srcstep, dst+=dststep)  \
            for(k = 0; k < cn; k++)                       \
                for(i = 0; i < size.width; i += cn)       \
                    dst[i+k] = _lut[src[i+k]*cn+k];         \
        return CC_OK;                                       \
    }                                                       \
                                                            \
    /* repack the lut to planar layout */                   \
    for(k = 0; k < cn; k++)                               \
        for(i = 0; i < 256; i++)                          \
            lutp[i+k*256] = _lut[i*cn+k];                   \
                                                            \
    for(; size.height--; src += srcstep, dst += dststep)  \
    {                                                       \
        for(i = 0; i < size.width;)                       \
        {                                                   \
            int j, limit = MIN(size.width,i+max_block_size);\
            for(k=0; k<cn; k++, src++, dst++)             \
            {                                               \
                const dsttype* lut = lutp + k*256;          \
                for(j = i; j <= limit - cn*2; j += cn*2)  \
                {                                           \
                    dsttype t0 = lut[src[j]];               \
                    dsttype t1 = lut[src[j+cn]];            \
                    dst[j] = t0; dst[j+cn] = t1;            \
                }                                           \
                                                            \
                for(; j < limit; j += cn)                 \
                    dst[j] = lut[src[j]];                   \
            }                                               \
            src -= cn;                                      \
            dst -= cn;                                      \
            i += limit;                                     \
        }                                                   \
    }                                                       \
                                                            \
    return CC_OK;                                           \
}

ICC_DEF_LUT_FUNC_8U(8u, uchar)
ICC_DEF_LUT_FUNC_8U(16u, ushort)
ICC_DEF_LUT_FUNC_8U(32s, int)
ICC_DEF_LUT_FUNC_8U(64f, double)

#undef   icvLUT_Transform8u_8s_C1R
#undef   icvLUT_Transform8u_16s_C1R
#undef   icvLUT_Transform8u_32f_C1R

#if 0
#define  icvLUT_Transform8u_8s_C1R    icvLUT_Transform8u_8u_C1R
#define  icvLUT_Transform8u_16s_C1R   icvLUT_Transform8u_16u_C1R
#define  icvLUT_Transform8u_32f_C1R   icvLUT_Transform8u_32s_C1R

#define  icvLUT_Transform8u_8s_CnR    icvLUT_Transform8u_8u_CnR
#define  icvLUT_Transform8u_16s_CnR   icvLUT_Transform8u_16u_CnR
#define  icvLUT_Transform8u_32f_CnR   icvLUT_Transform8u_32s_CnR
#else

#define  ICC_DEF_LUT_FUNC_8S_CN(flavor, dsttype, cn, fun, type)      \
static CStatus C_STDCALL icvLUT_Transform8u_##flavor##_C##cn##R(\
    const uchar* src, int srcstep,                          \
    dsttype* dst, int dststep, CSize size,                 \
const dsttype* lut) {return fun(src, srcstep, (type*)dst, dststep, size, (type*)lut); }

ICC_DEF_LUT_FUNC_8S_CN(8s, char, 1, icvLUT_Transform8u_8u_C1R, uchar)
ICC_DEF_LUT_FUNC_8S_CN(16s, short, 1, icvLUT_Transform8u_16u_C1R, ushort)
ICC_DEF_LUT_FUNC_8S_CN(32f, float, 1, icvLUT_Transform8u_32s_C1R, int)

#define  icvLUT_Transform8u_8s_CnR    icvLUT_Transform8u_8u_CnR
#define  icvLUT_Transform8u_16s_CnR   icvLUT_Transform8u_16u_CnR
#define  icvLUT_Transform8u_32f_CnR   icvLUT_Transform8u_32s_CnR

#endif

CC_DEF_INIT_FUNC_TAB_2D(LUT_Transform8u, C1R)
CC_DEF_INIT_FUNC_TAB_2D(LUT_Transform8u, CnR)

typedef CStatus (C_STDCALL * CvLUT_TransformCnFunc)(
    const void* src, int srcstep, void* dst,
    int dststep, CSize size, const void* lut, int cn);

CC_IMPL  void
cvLUT(const img_t* src, img_t* dst, const img_t* lut)
{
    static CvFuncTable lut_c1_tab, lut_cn_tab;
    static CvLUT_TransformFunc lut_8u_tab[4];
    static int inittab = 0;

    CC_FUNCNAME("cvLUT");

    __BEGIN__;

    TypeId type;
    int  coi1 = 0, coi2 = 0;
    int  cn, lut_cn;
    uchar* lut_data;
    uchar* shuffled_lut = 0;
    CSize size;

    if(!inittab)
    {
        icvInitLUT_Transform8uC1RTable(lut_c1_tab);
        icvInitLUT_Transform8uCnRTable(lut_cn_tab);
        lut_8u_tab[0] = (CvLUT_TransformFunc)icvLUT_Transform8u_8u_C1R;
        lut_8u_tab[1] = (CvLUT_TransformFunc)icvLUT_Transform8u_8u_C2R;
        lut_8u_tab[2] = (CvLUT_TransformFunc)icvLUT_Transform8u_8u_C3R;
        lut_8u_tab[3] = (CvLUT_TransformFunc)icvLUT_Transform8u_8u_C4R;
        inittab = 1;
    }

    if(coi1 != 0 || coi2 != 0)
        CC_ERROR(CC_BadCOI, "");

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    if(!CC_ARE_CNS_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(CC_MAT_TYPE(src) > CC_8S)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    type = CC_MAT_DEPTH(dst);
    cn = CC_MAT_CN(dst);
    lut_cn = CC_MAT_CN(lut);

    if(!CC_IS_MAT_CONT(lut) || (lut_cn != 1 && lut_cn != cn) ||
        !CC_ARE_DEPTHS_EQ(dst, lut) || lut->width*lut->height != 256)
        CC_ERROR(CC_StsBadArg, "The LUT must be continuous array \n"
                                "with 256 elements of the same type as destination");

    size = cvGetMatSize(src);
    if(lut_cn == 1)
    {
        size.width *= cn;
        cn = 1;
    }

    if(CC_IS_MAT_CONT(src) && CC_IS_MAT_CONT(dst))
    {
        size.width *= size.height;
        size.height = 1;
    }

    lut_data = lut->tt.data;

    if(CC_MAT_TYPE(src) == CC_8S)
    {
        int half_size = CC_ELEMTYPE_SIZE(dst)*cn*128;
        shuffled_lut = (uchar*)cvStackAlloc(half_size*2);

        // shuffle lut
        memcpy(shuffled_lut, lut_data + half_size, half_size);
        memcpy(shuffled_lut + half_size, lut_data, half_size);

        lut_data = shuffled_lut;
    }

    if(lut_cn == 1 || lut_cn <= 4 && type == CC_8U)
    {
        CvLUT_TransformFunc func = type == CC_8U ? lut_8u_tab[cn-1] :
            (CvLUT_TransformFunc)(lut_c1_tab[type]);
    
        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        IPPI_CALL(func(src->tt.data, src->step, dst->tt.data,
                         dst->step, size, lut_data));
    }
    else
    {
        CvLUT_TransformCnFunc func =
            (CvLUT_TransformCnFunc)(lut_cn_tab[type]);
    
        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        IPPI_CALL(func(src->tt.data, src->step, dst->tt.data,
                         dst->step, size, lut_data, cn));
    }

    __END__;
}

#undef ICC_LUT_CASE_C1
#undef ICC_LUT_CASE_C2
#undef ICC_LUT_CASE_C3
#undef ICC_LUT_CASE_C4
#undef ICC_DEF_LUT_FUNC_8U_CN
#undef ICC_DEF_LUT_FUNC_8U

//#undef icvLUT_Transform8u_8s_C1R
//#undef icvLUT_Transform8u_16s_C1R
//#undef icvLUT_Transform8u_32f_C1R
//#undef icvLUT_Transform8u_8s_CnR
//#undef icvLUT_Transform8u_16s_CnR
//#undef icvLUT_Transform8u_32f_CnR


#endif // _LUT_INL_
