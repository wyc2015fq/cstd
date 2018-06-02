

/* ////////////////////////////////////////////////////////////////////
//
//  img_t basic operations: cvCopy, cvSet
//
// */


/****************************************************************************************\
*                                          Clearing                                      *
\****************************************************************************************/

static CStatus C_STDCALL
icvSetZero_8u_C1R(uchar* dst, int dststep, CSize size)
{
    for(; size.height--; dst += dststep)
        memset(dst, 0, size.width);

    return CC_OK;
}

#define cvZero cvZeroMat
CC_IMPL void
cvZeroMat(img_t* mat)
{
    CC_FUNCNAME("cvSetZero");
    
    __BEGIN__;

    CSize size;
    int mat_step;

    size = cvGetMatSize(mat);
    size.width *= CC_TYPE_SIZE(mat->tid);
    mat_step = mat->step;

    if(CC_IS_MAT_CONT(mat))
    {
        size.width *= size.height;

        if(size.width <= CC_MAX_INLINE_MAT_OP_SIZE*(int)sizeof(double))
        {
            memset(mat->tt.data, 0, size.width);
            EXIT;
        }

        mat_step = CC_STUB_STEP;
        size.height = 1;
    }

    IPPI_CALL(icvSetZero_8u_C1R(mat->tt.data, mat_step, size));

    __END__;
}

CC_IMPL img_t*
cvSetZero(img_t* mat, int rows, int cols, TypeId type, int cn CC_DEFAULT(1) )
{
  cvSetMat(mat, rows, cols, type, cn, NULL, CC_AUTO_STEP);
  cvZeroMat(mat);
  return mat;
}

/////////////////////////////////////////////////////////////////////////////////////////
//                                                                                     //
//                                  L/L COPY & SET FUNCTIONS                           //
//                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////


IPCVAPI_IMPL(CStatus, icvCopy_8u_C1R, (const uchar* src, int srcstep,
                                          uchar* dst, int dststep, CSize size),
                                          (src, srcstep, dst, dststep, size))
{
    for(; size.height--; src += srcstep, dst += dststep)
        memcpy(dst, src, size.width);

    return  CC_OK;
}


static CStatus C_STDCALL
icvSet_8u_C1R(uchar* dst, int dst_step, CSize size,
               const void* scalar, int pix_size)
{
    int copy_len = 12*pix_size;
    uchar* dst_limit = dst + size.width;
    
    if(size.height--)
    {
        while(dst + copy_len <= dst_limit)
        {
            memcpy(dst, scalar, copy_len);
            dst += copy_len;
        }

        memcpy(dst, scalar, dst_limit - dst);
    }

    if(size.height)
    {
        dst = dst_limit - size.width + dst_step;

        for(; size.height--; dst += dst_step)
            memcpy(dst, dst - dst_step, size.width);
    }

    return CC_OK;
}


/////////////////////////////////////////////////////////////////////////////////////////
//                                                                                     //
//                                L/L COPY WITH MASK FUNCTIONS                         //
//                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////


#define ICC_DEF_COPY_MASK_C1_CASE(type)   \
    for(i = 0; i <= size.width-2; i += 2) \
    {                                       \
        if(mask[i])                       \
            dst[i] = src[i];                \
        if(mask[i+1])                     \
            dst[i+1] = src[i+1];            \
    }                                       \
                                            \
    for(; i < size.width; i++)            \
    {                                       \
        if(mask[i])                       \
            dst[i] = src[i];                \
    }

#define ICC_DEF_COPY_MASK_C3_CASE(type)   \
    for(i = 0; i < size.width; i++)       \
        if(mask[i])                       \
        {                                   \
            type t0 = src[i*3];             \
            type t1 = src[i*3+1];           \
            type t2 = src[i*3+2];           \
                                            \
            dst[i*3] = t0;                  \
            dst[i*3+1] = t1;                \
            dst[i*3+2] = t2;                \
        }



#define ICC_DEF_COPY_MASK_C4_CASE(type)   \
    for(i = 0; i < size.width; i++)       \
        if(mask[i])                       \
        {                                   \
            type t0 = src[i*4];             \
            type t1 = src[i*4+1];           \
            dst[i*4] = t0;                  \
            dst[i*4+1] = t1;                \
                                            \
            t0 = src[i*4+2];                \
            t1 = src[i*4+3];                \
            dst[i*4+2] = t0;                \
            dst[i*4+3] = t1;                \
        }


#define ICC_DEF_COPY_MASK_2D(name, type, cn)              \
IPCVAPI_IMPL(CStatus,                                     \
name,(const type* src, int srcstep, type* dst, int dststep,\
       CSize size, const uchar* mask, int maskstep),      \
       (src, srcstep, dst, dststep, size, mask, maskstep))  \
{                                                           \
    srcstep /= sizeof(src[0]); dststep /= sizeof(dst[0]);   \
    for(; size.height--; src += srcstep,                   \
            dst += dststep, mask += maskstep)              \
    {                                                       \
        int i;                                              \
        ICC_DEF_COPY_MASK_C##cn##_CASE(type)              \
    }                                                       \
                                                            \
    return  CC_OK;                                          \
}


#define ICC_DEF_SET_MASK_C1_CASE(type)    \
    for(i = 0; i <= size.width-2; i += 2) \
    {                                       \
        if(mask[i])                       \
            dst[i] = s0;                    \
        if(mask[i+1])                     \
            dst[i+1] = s0;                  \
    }                                       \
                                            \
    for(; i < size.width; i++)            \
    {                                       \
        if(mask[i])                       \
            dst[i] = s0;                    \
    }


#define ICC_DEF_SET_MASK_C3_CASE(type)    \
    for(i = 0; i < size.width; i++)       \
        if(mask[i])                       \
        {                                   \
            dst[i*3] = s0;                  \
            dst[i*3+1] = s1;                \
            dst[i*3+2] = s2;                \
        }

#define ICC_DEF_SET_MASK_C4_CASE(type)    \
    for(i = 0; i < size.width; i++)       \
        if(mask[i])                       \
        {                                   \
            dst[i*4] = s0;                  \
            dst[i*4+1] = s1;                \
            dst[i*4+2] = s2;                \
            dst[i*4+3] = s3;                \
        }

#define ICC_DEF_SET_MASK_2D(name, type, cn)       \
IPCVAPI_IMPL(CStatus,                             \
name,(type* dst, int dststep,                      \
       const uchar* mask, int maskstep,             \
       CSize size, const type* scalar),           \
       (dst, dststep, mask, maskstep, size, scalar))\
{                                                   \
    CC_UN_ENTRY_C##cn(type);                      \
    dststep /= sizeof(dst[0]);                      \
                                                    \
    for(; size.height--; mask += maskstep,         \
                          dst += dststep)          \
    {                                               \
        int i;                                      \
        ICC_DEF_SET_MASK_C##cn##_CASE(type)       \
    }                                               \
                                                    \
    return CC_OK;                                   \
}


ICC_DEF_SET_MASK_2D(icvSet_8u_C1MR, uchar, 1)
ICC_DEF_SET_MASK_2D(icvSet_16s_C1MR, ushort, 1)
ICC_DEF_SET_MASK_2D(icvSet_8u_C3MR, uchar, 3)
ICC_DEF_SET_MASK_2D(icvSet_8u_C4MR, int, 1)
ICC_DEF_SET_MASK_2D(icvSet_16s_C3MR, ushort, 3)
ICC_DEF_SET_MASK_2D(icvSet_16s_C4MR, int64, 1)
ICC_DEF_SET_MASK_2D(icvSet_32f_C3MR, int, 3)
ICC_DEF_SET_MASK_2D(icvSet_32f_C4MR, int, 4)
ICC_DEF_SET_MASK_2D(icvSet_64s_C3MR, int64, 3)
ICC_DEF_SET_MASK_2D(icvSet_64s_C4MR, int64, 4)

ICC_DEF_COPY_MASK_2D(icvCopy_8u_C1MR, uchar, 1)
ICC_DEF_COPY_MASK_2D(icvCopy_16s_C1MR, ushort, 1)
ICC_DEF_COPY_MASK_2D(icvCopy_8u_C3MR, uchar, 3)
ICC_DEF_COPY_MASK_2D(icvCopy_8u_C4MR, int, 1)
ICC_DEF_COPY_MASK_2D(icvCopy_16s_C3MR, ushort, 3)
ICC_DEF_COPY_MASK_2D(icvCopy_16s_C4MR, int64, 1)
ICC_DEF_COPY_MASK_2D(icvCopy_32f_C3MR, int, 3)
ICC_DEF_COPY_MASK_2D(icvCopy_32f_C4MR, int, 4)
ICC_DEF_COPY_MASK_2D(icvCopy_64s_C3MR, int64, 3)
ICC_DEF_COPY_MASK_2D(icvCopy_64s_C4MR, int64, 4)

#define CC_DEF_INIT_COPYSET_TAB_2D(FUNCNAME, FLAG)                \
static void icvInit##FUNCNAME##FLAG##Table(CvBtFuncTable  table)  \
{                                                                   \
    table[1]  = (void*)icv##FUNCNAME##_8u_C1##FLAG;          \
    table[2]  = (void*)icv##FUNCNAME##_16s_C1##FLAG;         \
    table[3]  = (void*)icv##FUNCNAME##_8u_C3##FLAG;          \
    table[4]  = (void*)icv##FUNCNAME##_8u_C4##FLAG;          \
    table[6]  = (void*)icv##FUNCNAME##_16s_C3##FLAG;         \
    table[8]  = (void*)icv##FUNCNAME##_16s_C4##FLAG;         \
    table[12] = (void*)icv##FUNCNAME##_32f_C3##FLAG;         \
    table[16] = (void*)icv##FUNCNAME##_32f_C4##FLAG;         \
    table[24] = (void*)icv##FUNCNAME##_64s_C3##FLAG;         \
    table[32] = (void*)icv##FUNCNAME##_64s_C4##FLAG;         \
}

CC_DEF_INIT_COPYSET_TAB_2D(Set, MR)
CC_DEF_INIT_COPYSET_TAB_2D(Copy, MR)

/////////////////////////////////////////////////////////////////////////////////////////
//                                                                                     //
//                                H/L COPY & SET FUNCTIONS                             //
//                                                                                     //
/////////////////////////////////////////////////////////////////////////////////////////


static CvCopyMaskFunc
icvGetCopyMaskFunc(int elem_size)
{
    static CvBtFuncTable copym_tab;
    static int inittab = 0;

    if(!inittab)
    {
        icvInitCopyMRTable(copym_tab);
        inittab = 1;
    }
    return (CvCopyMaskFunc)copym_tab[elem_size];
}


/* dst = src */
#define cvCopy(src, dst)  cvCopyMask(src, dst, NULL)
#define cvCopyTo cvCopy
CC_IMPL img_t*
cvCopyMask(const img_t* src, img_t* dst, const img_t* mask CC_DEFAULT(NULL))
{
    CC_FUNCNAME("cvCopy");
    
    __BEGIN__;

    int pix_size;
    CSize size;
    if (dst!=src) {
      cvSetMatFromMat(dst, src);
    }

    if(!CC_ARE_TYPES_EQ(src, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedFormats);

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    size = cvGetMatSize(src);
    pix_size = CC_TYPE_SIZE(src->tid) * CC_MAT_CN(src);

    if(!mask)
    {
        int src_step = src->step, dst_step = dst->step;
        size.width *= pix_size;
        if(CC_IS_MAT_CONT(src) && CC_IS_MAT_CONT(dst) && (src_step == dst_step) && (src_step == src->width * pix_size))
        {
            size.width *= size.height;

            if(size.width <= CC_MAX_INLINE_MAT_OP_SIZE*
                              CC_MAX_INLINE_MAT_OP_SIZE*(int)sizeof(double))
            {
                memcpy(dst->tt.data, src->tt.data, size.width);
                EXIT;
            }

            size.height = 1;
            src_step = dst_step = CC_STUB_STEP;
        }

        icvCopy_8u_C1R(src->tt.data, src_step,
                        dst->tt.data, dst_step, size);
    }
    else
    {
        CvCopyMaskFunc func = icvGetCopyMaskFunc(pix_size);
        int src_step = src->step;
        int dst_step = dst->step;
        int mask_step;

        if(!CC_IS_MASK_ARR(mask))
            CC_ERROR(CC_StsBadMask, "");

        if(!CC_ARE_SIZES_EQ(src, mask))
            CC_ERROR(CC_StsUnmatchedSizes, "");

        mask_step = mask->step;
        
        if(CC_IS_MAT_CONT(src) && CC_IS_MAT_CONT(dst) && CC_IS_MAT_CONT(mask))
        {
            size.width *= size.height;
            size.height = 1;
            src_step = dst_step = mask_step = CC_STUB_STEP;
        }

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        IPPI_CALL(func(src->tt.data, src_step, dst->tt.data, dst_step,
                         size, mask->tt.data, mask_step));
    }

    __END__;
	return dst;
}

#define cvSet(mat, value) cvSetWithMask(mat, value, NULL)
/* dst(idx) = value */
CC_IMPL void
cvSetWithMask(img_t* mat, CScalar value, const img_t* mask CC_DEFAULT(NULL))
{
    static CvBtFuncTable setm_tab;
    static int inittab = 0;
    
    CC_FUNCNAME("cvSet");

    __BEGIN__;

    int pix_size;
    TypeId type;
    double buf[12];
    int mat_step, cn = CC_MAT_CN(mat);
    CSize size;

    if(!value.val[0] && !value.val[1] &&
        !value.val[2] && !value.val[3] && !mask)
    {
        cvZeroMat(mat);
        EXIT;
    }

    type = CC_MAT_DEPTH(mat);
    pix_size = CC_ELEM_SIZE_(mat);
    size = cvGetMatSize(mat);
    mat_step = mat->step;

    if(!mask)
    {
        if(CC_IS_MAT_CONT(mat))
        {
            size.width *= size.height;
        
            if(size.width <= (int)(CC_MAX_INLINE_MAT_OP_SIZE*sizeof(double)))
            {
                if(type == CC_32F && cn==1)
                {
                    float* dstdata = (float*)(mat->tt.data);
                    float val = (float)value.val[0];

                    do
                    {
                        dstdata[size.width-1] = val;
                    }
                    while(--size.width);

                    EXIT;
                }

                if(type == CC_64F && 1==cn)
                {
                    double* dstdata = (double*)(mat->tt.data);
                    double val = value.val[0];

                    do
                    {
                        dstdata[size.width-1] = val;
                    }
                    while(--size.width);

                    EXIT;
                }
            }

            mat_step = CC_STUB_STEP;
            size.height = 1;
        }
        
        size.width *= pix_size;
        CC_CALL(cvScalarToRawData(&value, buf, type, cn, 1));
        IPPI_CALL(icvSet_8u_C1R(mat->tt.data, mat_step, size, buf, CC_ELEM_SIZE_(mat)));
    }
    else
    {
        CvFunc2D_2A1P func;
        int mask_step;

        if(!CC_IS_MASK_ARR(mask))
            CC_ERROR(CC_StsBadMask, "");

        if(!inittab)
        {
            icvInitSetMRTable(setm_tab);
            inittab = 1;
        }

        if(!CC_ARE_SIZES_EQ(mat, mask))
            CC_ERROR(CC_StsUnmatchedSizes, "");

        mask_step = mask->step;

        if(CC_IS_MAT_CONT(mat) && CC_IS_MAT_CONT(mask))
        {
            size.width *= size.height;
            mat_step = mask_step = CC_STUB_STEP;
            size.height = 1;
        }

        func = (CvFunc2D_2A1P)(setm_tab[pix_size]);
        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        CC_CALL(cvScalarToRawData(&value, buf, type, cn, 0));

        IPPI_CALL(func(mat->tt.data, mat_step, mask->tt.data,
                         mask_step, size, buf));
    }

    __END__;
}

CC_IMPL void
cvSetReal(img_t* mat, double value, const img_t* mask CC_DEFAULT(NULL)) {
  cvSetWithMask(mat, cRealScalar(value), mask);
}
static void cvSetTo(img_t* mat, int rows, int cols, TypeId type, double value) {
  cvSetMat(mat, rows, cols, type, 1, NULL, CC_AUTO_STEP);
  cvSetReal(mat, value, NULL);
}

/****************************************************************************************\
*                                          Flipping                                      *
\****************************************************************************************/

#define ICC_DEF_FLIP_HZ_CASE_C1(type)   \
    for(i = 0; i < (len+1)/2; i++)      \
    {                                   \
        type t0 = src[i];               \
        type t1 = src[len - i - 1];     \
        dst[i] = t1;                    \
        dst[len - i - 1] = t0;          \
    }


#define ICC_DEF_FLIP_HZ_CASE_C3(type)   \
    for(i = 0; i < (len+1)/2; i++)      \
    {                                   \
        type t0 = src[i*3];             \
        type t1 = src[(len - i)*3 - 3]; \
        dst[i*3] = t1;                  \
        dst[(len - i)*3 - 3] = t0;      \
        t0 = src[i*3 + 1];              \
        t1 = src[(len - i)*3 - 2];      \
        dst[i*3 + 1] = t1;              \
        dst[(len - i)*3 - 2] = t0;      \
        t0 = src[i*3 + 2];              \
        t1 = src[(len - i)*3 - 1];      \
        dst[i*3 + 2] = t1;              \
        dst[(len - i)*3 - 1] = t0;      \
    }


#define ICC_DEF_FLIP_HZ_CASE_C4(type)   \
    for(i = 0; i < (len+1)/2; i++)      \
    {                                   \
        type t0 = src[i*4];             \
        type t1 = src[(len - i)*4 - 4]; \
        dst[i*4] = t1;                  \
        dst[(len - i)*4 - 4] = t0;      \
        t0 = src[i*4 + 1];              \
        t1 = src[(len - i)*4 - 3];      \
        dst[i*4 + 1] = t1;              \
        dst[(len - i)*4 - 3] = t0;      \
        t0 = src[i*4 + 2];              \
        t1 = src[(len - i)*4 - 2];      \
        dst[i*4 + 2] = t1;              \
        dst[(len - i)*4 - 2] = t0;      \
        t0 = src[i*4 + 3];              \
        t1 = src[(len - i)*4 - 1];      \
        dst[i*4 + 3] = t1;              \
        dst[(len - i)*4 - 1] = t0;      \
    }


#define ICC_DEF_FLIP_HZ_FUNC(flavor, arrtype, cn)                   \
static CStatus C_STDCALL                                          \
icvFlipHorz_##flavor(const arrtype* src, int srcstep,               \
                      arrtype* dst, int dststep, CSize size)       \
{                                                                   \
    int i, len = size.width;                                        \
    srcstep /= sizeof(src[0]); dststep /= sizeof(dst[0]);           \
                                                                    \
    for(; size.height--; src += srcstep, dst += dststep)            \
    {                                                               \
        ICC_DEF_FLIP_HZ_CASE_C##cn(arrtype)                         \
    }                                                               \
                                                                    \
    return CC_OK;                                                   \
}


ICC_DEF_FLIP_HZ_FUNC(8u_C1R, uchar, 1)
ICC_DEF_FLIP_HZ_FUNC(8u_C2R, ushort, 1)
ICC_DEF_FLIP_HZ_FUNC(8u_C3R, uchar, 3)
ICC_DEF_FLIP_HZ_FUNC(16u_C2R, int, 1)
ICC_DEF_FLIP_HZ_FUNC(16u_C3R, ushort, 3)
ICC_DEF_FLIP_HZ_FUNC(32s_C2R, int64, 1)
ICC_DEF_FLIP_HZ_FUNC(32s_C3R, int, 3)
ICC_DEF_FLIP_HZ_FUNC(64s_C2R, int, 4)
ICC_DEF_FLIP_HZ_FUNC(64s_C3R, int64, 3)
ICC_DEF_FLIP_HZ_FUNC(64s_C4R, int64, 4)

CC_DEF_INIT_PIXSIZE_TAB_2D(FlipHorz, R)


static CStatus
icvFlipVert_8u_C1R(const uchar* src, int srcstep,
                    uchar* dst, int dststep, CSize size)
{
    int y, i;
    const uchar* src1 = src + (size.height - 1)*srcstep;
    uchar* dst1 = dst + (size.height - 1)*dststep;

    for(y = 0; y < (size.height + 1)/2; y++, src += srcstep, src1 -= srcstep,
                                              dst += dststep, dst1 -= dststep)
    {
        i = 0;
        if(((size_t)(src)|(size_t)(dst)|(size_t)src1|(size_t)dst1) % sizeof(int) == 0)
        {
            for(; i <= size.width - 16; i += 16)
            {
                int t0 = ((int*)(src + i))[0];
                int t1 = ((int*)(src1 + i))[0];

                ((int*)(dst + i))[0] = t1;
                ((int*)(dst1 + i))[0] = t0;

                t0 = ((int*)(src + i))[1];
                t1 = ((int*)(src1 + i))[1];

                ((int*)(dst + i))[1] = t1;
                ((int*)(dst1 + i))[1] = t0;

                t0 = ((int*)(src + i))[2];
                t1 = ((int*)(src1 + i))[2];

                ((int*)(dst + i))[2] = t1;
                ((int*)(dst1 + i))[2] = t0;

                t0 = ((int*)(src + i))[3];
                t1 = ((int*)(src1 + i))[3];

                ((int*)(dst + i))[3] = t1;
                ((int*)(dst1 + i))[3] = t0;
            }

            for(; i <= size.width - 4; i += 4)
            {
                int t0 = ((int*)(src + i))[0];
                int t1 = ((int*)(src1 + i))[0];

                ((int*)(dst + i))[0] = t1;
                ((int*)(dst1 + i))[0] = t0;
            }
        }

        for(; i < size.width; i++)
        {
            uchar t0 = src[i];
            uchar t1 = src1[i];

            dst[i] = t1;
            dst1[i] = t0;
        }
    }

    return CC_OK;
}


CC_IMPL void
cvFlip(const img_t* src, img_t* dst, int flip_mode)
{
    static CvBtFuncTable tab;
    static int inittab = 0;
    
    CC_FUNCNAME("cvFlip");
    
    __BEGIN__;

    CSize size;
    CvFunc2D_2A func = 0;
    int pix_size;

    if(!inittab)
    {
        icvInitFlipHorzRTable(tab);
        inittab = 1;
    }

    if(!dst)
        dst = (img_t*)src;

    if(!CC_ARE_TYPES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    size = cvGetMatSize(src);
    pix_size = CC_TYPE_SIZE(src->tid);

    if(flip_mode == 0)
    {
        size.width *= pix_size;
        
        IPPI_CALL(icvFlipVert_8u_C1R(src->tt.data, src->step,
                                       dst->tt.data, dst->step, size));
    }
    else
    {
        int inplace = src->tt.data == dst->tt.data;
        uchar* dst_data = dst->tt.data;
        int dst_step = dst->step;

        func = (CvFunc2D_2A)(tab[pix_size]);

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        if(flip_mode < 0 && !inplace)
        {
            dst_data += dst_step * (dst->height - 1);
            dst_step = -dst_step;
        }

        IPPI_CALL(func(src->tt.data, src->step, dst_data, dst_step, size));
        
        if(flip_mode < 0 && inplace)
        {
            size.width *= pix_size;
            IPPI_CALL(icvFlipVert_8u_C1R(dst->tt.data, dst->step,
                                           dst->tt.data, dst->step, size));
        }
    }

    __END__;
}


CC_IMPL void
cvRepeat(const img_t* src, img_t* dst)
{
    CC_FUNCNAME("cvRepeat");
    
    __BEGIN__;

    CSize srcsize, dstsize;
    int pix_size;
    int x, y, k, l;

    if(!CC_ARE_TYPES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    srcsize = cvGetMatSize(src);
    dstsize = cvGetMatSize(dst);
    pix_size = CC_TYPE_SIZE(src->tid);

    for(y = 0, k = 0; y < dstsize.height; y++)
    {
        for(x = 0; x < dstsize.width; x += srcsize.width)
        {
            l = srcsize.width;
            if(l > dstsize.width - x)
                l = dstsize.width - x;
            memcpy(dst->tt.data + y*dst->step + x*pix_size,
                    src->tt.data + k*src->step, l*pix_size);
        }
        if(++k == srcsize.height)
            k = 0;
    }

    __END__;
}




/****************************************************************************************\
*                                     Matrix transpose                                   *
\****************************************************************************************/

/////////////////// macros for inplace transposition of square matrix ////////////////////

#define ICC_DEF_TRANSP_INP_CASE_C1( \
    arrtype, len)                   \
{                                   \
    arrtype* arr1 = arr, *arr2, *arr3; \
    step /= sizeof(arr[0]);         \
                                    \
    while(--len)                    \
    {                               \
        arr += step, arr1++;        \
        arr2 = arr;        \
        arr3 = arr1;       \
                                    \
        do                          \
        {                           \
            arrtype t0 = arr2[0];   \
            arrtype t1 = arr3[0];   \
            arr2[0] = t1;           \
            arr3[0] = t0;           \
                                    \
            arr2++;                 \
            arr3 += step;           \
        }                           \
        while(arr2 != arr3);        \
    }                               \
}


#define ICC_DEF_TRANSP_INP_CASE_C3( \
    arrtype, len)                   \
{                                   \
    arrtype* arr1 = arr, *arr2, *arr3; \
    int y;                          \
    step /= sizeof(arr[0]);         \
                                    \
    for(y = 1; y < len; y++)        \
    {                               \
        arr += step, arr1 += 3;     \
        arr2 = arr;        \
        arr3 = arr1;       \
                                    \
        for(; arr2!=arr3; arr2+=3,  \
                        arr3+=step) \
        {                           \
            arrtype t0 = arr2[0];   \
            arrtype t1 = arr3[0];   \
            arr2[0] = t1;           \
            arr3[0] = t0;           \
            t0 = arr2[1];           \
            t1 = arr3[1];           \
            arr2[1] = t1;           \
            arr3[1] = t0;           \
            t0 = arr2[2];           \
            t1 = arr3[2];           \
            arr2[2] = t1;           \
            arr3[2] = t0;           \
        }                           \
    }                               \
}


#define ICC_DEF_TRANSP_INP_CASE_C4( \
    arrtype, len)                   \
{                                   \
    arrtype* arr1 = arr, *arr2, *arr3; \
    int y;                          \
    step /= sizeof(arr[0]);         \
                                    \
    for(y = 1; y < len; y++)        \
    {                               \
        arr += step, arr1 += 4;     \
        arr2 = arr;        \
        arr3 = arr1;       \
                                    \
        for(; arr2!=arr3; arr2+=4,  \
                        arr3+=step) \
        {                           \
            arrtype t0 = arr2[0];   \
            arrtype t1 = arr3[0];   \
            arr2[0] = t1;           \
            arr3[0] = t0;           \
            t0 = arr2[1];           \
            t1 = arr3[1];           \
            arr2[1] = t1;           \
            arr3[1] = t0;           \
            t0 = arr2[2];           \
            t1 = arr3[2];           \
            arr2[2] = t1;           \
            arr3[2] = t0;           \
            t0 = arr2[3];           \
            t1 = arr3[3];           \
            arr2[3] = t1;           \
            arr3[3] = t0;           \
        }                           \
    }                               \
}


//////////////// macros for non-inplace transposition of rectangular matrix //////////////

#define ICC_DEF_TRANSP_CASE_C1(arrtype)         \
{                                               \
    int x, y;                                   \
    srcstep /= sizeof(src[0]);                  \
    dststep /= sizeof(dst[0]);                  \
                                                \
    for(y = 0; y <= size.height - 2; y += 2,    \
                src += 2*srcstep, dst += 2)     \
    {                                           \
        const arrtype* src1 = src + srcstep;    \
        arrtype* dst1 = dst;                    \
                                                \
        for(x = 0; x <= size.width - 2;         \
                x += 2, dst1 += dststep)        \
        {                                       \
            arrtype t0 = src[x];                \
            arrtype t1 = src1[x];               \
            dst1[0] = t0;                       \
            dst1[1] = t1;                       \
            dst1 += dststep;                    \
                                                \
            t0 = src[x + 1];                    \
            t1 = src1[x + 1];                   \
            dst1[0] = t0;                       \
            dst1[1] = t1;                       \
        }                                       \
                                                \
        if(x < size.width)                      \
        {                                       \
            arrtype t0 = src[x];                \
            arrtype t1 = src1[x];               \
            dst1[0] = t0;                       \
            dst1[1] = t1;                       \
        }                                       \
    }                                           \
                                                \
    if(y < size.height)                         \
    {                                           \
        arrtype* dst1 = dst;                    \
        for(x = 0; x <= size.width - 2;         \
                x += 2, dst1 += 2*dststep)      \
        {                                       \
            arrtype t0 = src[x];                \
            arrtype t1 = src[x + 1];            \
            dst1[0] = t0;                       \
            dst1[dststep] = t1;                 \
        }                                       \
                                                \
        if(x < size.width)                      \
        {                                       \
            arrtype t0 = src[x];                \
            dst1[0] = t0;                       \
        }                                       \
    }                                           \
}


#define ICC_DEF_TRANSP_CASE_C3(arrtype)         \
{                                               \
    size.width *= 3;                            \
    srcstep /= sizeof(src[0]);                  \
    dststep /= sizeof(dst[0]);                  \
                                                \
    for(; size.height--; src+=srcstep, dst+=3)  \
    {                                           \
        int x;                                  \
        arrtype* dst1 = dst;                    \
                                                \
        for(x = 0; x < size.width; x += 3,      \
                            dst1 += dststep)    \
        {                                       \
            arrtype t0 = src[x];                \
            arrtype t1 = src[x + 1];            \
            arrtype t2 = src[x + 2];            \
                                                \
            dst1[0] = t0;                       \
            dst1[1] = t1;                       \
            dst1[2] = t2;                       \
        }                                       \
    }                                           \
}


#define ICC_DEF_TRANSP_CASE_C4(arrtype)         \
{                                               \
    size.width *= 4;                            \
    srcstep /= sizeof(src[0]);                  \
    dststep /= sizeof(dst[0]);                  \
                                                \
    for(; size.height--; src+=srcstep, dst+=4)  \
    {                                           \
        int x;                                  \
        arrtype* dst1 = dst;                    \
                                                \
        for(x = 0; x < size.width; x += 4,      \
                            dst1 += dststep)    \
        {                                       \
            arrtype t0 = src[x];                \
            arrtype t1 = src[x + 1];            \
                                                \
            dst1[0] = t0;                       \
            dst1[1] = t1;                       \
                                                \
            t0 = src[x + 2];                    \
            t1 = src[x + 3];                    \
                                                \
            dst1[2] = t0;                       \
            dst1[3] = t1;                       \
        }                                       \
    }                                           \
}


#define ICC_DEF_TRANSP_INP_FUNC(flavor, arrtype, cn)        \
static CStatus C_STDCALL                                    \
icvTranspose_##flavor(arrtype* arr, int step, CSize size)   \
{                                                           \
    assert(size.width == size.height);                      \
                                                            \
    ICC_DEF_TRANSP_INP_CASE_C##cn(arrtype, size.width)      \
    return CC_OK;                                           \
}


#define ICC_DEF_TRANSP_FUNC(flavor, arrtype, cn)            \
static CStatus C_STDCALL                                    \
icvTranspose_##flavor(const arrtype* src, int srcstep,      \
                    arrtype* dst, int dststep, CSize size)  \
{                                                           \
    ICC_DEF_TRANSP_CASE_C##cn(arrtype)                      \
    return CC_OK;                                           \
}


ICC_DEF_TRANSP_INP_FUNC(8u_C1IR, uchar, 1)
ICC_DEF_TRANSP_INP_FUNC(8u_C2IR, ushort, 1)
ICC_DEF_TRANSP_INP_FUNC(8u_C3IR, uchar, 3)
ICC_DEF_TRANSP_INP_FUNC(16u_C2IR, int, 1)
ICC_DEF_TRANSP_INP_FUNC(16u_C3IR, ushort, 3)
ICC_DEF_TRANSP_INP_FUNC(32s_C2IR, int64, 1)
ICC_DEF_TRANSP_INP_FUNC(32s_C3IR, int, 3)
ICC_DEF_TRANSP_INP_FUNC(64s_C2IR, int, 4)
ICC_DEF_TRANSP_INP_FUNC(64s_C3IR, int64, 3)
ICC_DEF_TRANSP_INP_FUNC(64s_C4IR, int64, 4)


ICC_DEF_TRANSP_FUNC(8u_C1R, uchar, 1)
ICC_DEF_TRANSP_FUNC(8u_C2R, ushort, 1)
ICC_DEF_TRANSP_FUNC(8u_C3R, uchar, 3)
ICC_DEF_TRANSP_FUNC(16u_C2R, int, 1)
ICC_DEF_TRANSP_FUNC(16u_C3R, ushort, 3)
ICC_DEF_TRANSP_FUNC(32s_C2R, int64, 1)
ICC_DEF_TRANSP_FUNC(32s_C3R, int, 3)
ICC_DEF_TRANSP_FUNC(64s_C2R, int, 4)
ICC_DEF_TRANSP_FUNC(64s_C3R, int64, 3)
ICC_DEF_TRANSP_FUNC(64s_C4R, int64, 4)

CC_DEF_INIT_PIXSIZE_TAB_2D(Transpose, R)
CC_DEF_INIT_PIXSIZE_TAB_2D(Transpose, IR)

#define cvT cvTranspose
CC_IMPL img_t*
cvTranspose(const img_t* src, img_t* dst)
{
    static CvBtFuncTable tab, inp_tab;
    static int inittab = 0;
    
    CC_FUNCNAME("cvTranspose");

    __BEGIN__;
    
    CSize size;
    int pix_size;
    TypeId type;

	IM_BEGIN(im, 1);
	if (src == dst) {
		src = cvCloneMat(im++, src);
	}
	cvSetMat(dst, src->w, src->h, src->tid, CC_MAT_CN(src), NULL, CC_AUTO_STEP);

    if(!inittab)
    {
        icvInitTransposeIRTable(inp_tab);
        icvInitTransposeRTable(tab);
        inittab = 1;
    }

    type = CC_MAT_DEPTH(src);
    pix_size = CC_TYPE_SIZE(type);
    size = cvGetMatSize(src);

    if(dst != src)
    {
		cvSetMat(dst, src->cols, src->rows, src->tid, CC_MAT_CN(src), NULL, CC_AUTO_STEP);
        if(!CC_ARE_TYPES_EQ(src, dst))
            CC_ERROR(CC_StsUnmatchedFormats, "");

        if(size.width != dst->height || size.height != dst->width)
            CC_ERROR(CC_StsUnmatchedSizes, "");
    }

    if(src->tt.data == dst->tt.data)
    {
        if(size.width == size.height)
        {
            CvFunc2D_1A func = (CvFunc2D_1A)(inp_tab[pix_size]);

            if(!func)
                CC_ERROR(CC_StsUnsupportedFormat, "");

            IPPI_CALL(func(src->tt.data, src->step, size));
        }
        else
        {
            if(size.width != 1 && size.height != 1) {
                //CC_ERROR(CC_StsBadSize, "Rectangular matrix can not be transposed inplace");
                img_t tmp[1] = {0};
                CvFunc2D_2A func;
                cvCloneMat(tmp, src);
                src = tmp;
                func = (CvFunc2D_2A)(tab[pix_size]);

                if(!func)
                    CC_ERROR(CC_StsUnsupportedFormat, "");

                IPPI_CALL(func(src->tt.data, src->step, dst->tt.data, dst->step, size));
                imfree(tmp);
            }
            else
            {
                if(!(CC_IS_MAT_CONT(src) && CC_IS_MAT_CONT(dst)))
                    CC_ERROR(CC_StsBadFlag, "In case of inplace column/row transposition "
                                           "both source and destination must be continuous");

                if(dst == src)
                {
                    int t;
                    CC_SWAP(dst->width, dst->height, t);
                    dst->step = dst->height == 1 ? (dst->width*pix_size) : pix_size;
                }
            }
        }
    }
    else
    {
        CvFunc2D_2A func = (CvFunc2D_2A)(tab[pix_size]);

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        IPPI_CALL(func(src->tt.data, src->step, dst->tt.data, dst->step, size));
    }

	IM_END(im);
    __END__;
    return dst;
}



typedef CStatus (C_STDCALL * CvCopyNonConstBorderFunc)(
    const void*, int, CSize, void*, int, CSize, int, int);

typedef CStatus (C_STDCALL * CvCopyNonConstBorderFuncI)(
    const void*, int, CSize, CSize, int, int);

static CStatus C_STDCALL
icvCopyReplicateBorder_8u(const uchar* src, int srcstep, CSize srcroi,
                           uchar* dst, int dststep, CSize dstroi,
                           int top, int left, int cn)
{
    const int isz = (int)sizeof(int);
    int i, j;

    if((cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0)
    {
        const int* isrc = (const int*)src;
        int* idst = (int*)dst;
        
        cn /= isz;
        srcstep /= isz;
        dststep /= isz;

        srcroi.width *= cn;
        dstroi.width *= cn;
        left *= cn;

        for(i = 0; i < dstroi.height; i++, idst += dststep)
        {
            if(idst + left != isrc)
                for(j = 0; j < srcroi.width; j++)
                    idst[j + left] = isrc[j];
            for(j = left - 1; j >= 0; j--)
                idst[j] = idst[j + cn];
            for(j = left+srcroi.width; j < dstroi.width; j++)
                idst[j] = idst[j - cn];
            if(i >= top && i < top + srcroi.height - 1)
                isrc += srcstep;
        }
    }
    else
    {
        srcroi.width *= cn;
        dstroi.width *= cn;
        left *= cn;

        for(i = 0; i < dstroi.height; i++, dst += dststep)
        {
            if(dst + left != src)
                for(j = 0; j < srcroi.width; j++)
                    dst[j + left] = src[j];
            for(j = left - 1; j >= 0; j--)
                dst[j] = dst[j + cn];
            for(j = left+srcroi.width; j < dstroi.width; j++)
                dst[j] = dst[j - cn];
            if(i >= top && i < top + srcroi.height - 1)
                src += srcstep;
        }
    }

    return CC_OK;
}

static CStatus C_STDCALL
icvCopyReflect101Border_8u(const uchar* src, int srcstep, CSize srcroi,
                            uchar* dst, int dststep, CSize dstroi,
                            int top, int left, int cn)
{
    const int isz = (int)sizeof(int);
    int i, j, k, t, dj, tab_size, int_mode = 0;
    const int* isrc = (const int*)src;
    int* idst = (int*)dst, *tab;

    if((cn | srcstep | dststep | (size_t)src | (size_t)dst) % isz == 0)
    {
        cn /= isz;
        srcstep /= isz;
        dststep /= isz;

        int_mode = 1;
    }

    srcroi.width *= cn;
    dstroi.width *= cn;
    left *= cn;

    tab_size = dstroi.width - srcroi.width;
    tab = (int*)cvStackAlloc(tab_size*sizeof(tab[0]));

    if(srcroi.width == 1)
    {
        for(k = 0; k < cn; k++)
            for(i = 0; i < tab_size; i += cn)
                tab[i + k] = k + left;
    }
    else
    {
        j = dj = cn;
        for(i = left - cn; i >= 0; i -= cn)
        {
            for(k = 0; k < cn; k++)
                tab[i + k] = j + k + left;
            if((unsigned)(j += dj) >= (unsigned)srcroi.width)
                j -= 2*dj, dj = -dj;
        }
        
        j = srcroi.width - cn*2;
        dj = -cn;
        for(i = left; i < tab_size; j += cn)
        {
            for(k = 0; k < cn; k++)
                tab[i + k] = j + k + left;
            if((unsigned)(j += dj) >= (unsigned)srcroi.width)
                j -= 2*dj, dj = -dj;
        }
    }

    if(int_mode)
    {
        idst += top*dststep;
        for(i = 0; i < srcroi.height; i++, isrc += srcstep, idst += dststep)
        {
            if(idst + left != isrc)
                for(j = 0; j < srcroi.width; j++)
                    idst[j + left] = isrc[j];
            for(j = 0; i < left; j++)
            {
                k = tab[j]; 
                idst[j] = idst[k];
            }
            for(; j < tab_size; j++)
            {
                k = tab[j];
                idst[j + srcroi.width] = idst[k];
            }
        }
        isrc -= srcroi.height*srcstep;
        idst -= (top - srcroi.height)*dststep;
    }
    else
    {
        dst += top*dststep;
        for(i = 0; i < srcroi.height; i++, src += srcstep, dst += dststep)
        {
            if(dst + left != src)
                for(j = 0; j < srcroi.width; j++)
                    dst[j + left] = src[j];
            for(j = 0; i < left; j++)
            {
                k = tab[j]; 
                dst[j] = dst[k];
            }
            for(; j < tab_size; j++)
            {
                k = tab[j];
                dst[j + srcroi.width] = dst[k];
            }
        }
        src -= srcroi.height*srcstep;
        dst -= (top - srcroi.height)*dststep;
    }

    for(t = 0; t < 2; t++)
    {
        int i1, i2, di;
        if(t == 0)
            i1 = top-1, i2 = 0, di = -1, j = 1, dj = 1;
        else
            i1 = top+srcroi.height, i2=dstroi.height, di = 1, j = srcroi.height-2, dj = -1;
        
        for(i = i1; i != i2; i += di)
        {
            if(int_mode)
            {
                const int* s = idst + i*dststep;
                int* d = idst + (j+top)*dststep;
                for(k = 0; k < dstroi.width; k++)
                    d[k] = s[k];
            }
            else
            {
                const uchar* s = dst + i*dststep;
                uchar* d = dst + (j+top)*dststep;
                for(k = 0; k < dstroi.width; k++)
                    d[k] = s[k];
            }

            if((unsigned)(j += dj) >= (unsigned)srcroi.height)
                j -= 2*dj, dj = -dj;
        }
    }

    return CC_OK;
}


static CStatus C_STDCALL
icvCopyConstBorder_8u(const uchar* src, int srcstep, CSize srcroi,
                       uchar* dst, int dststep, CSize dstroi,
                       int top, int left, int cn, const uchar* value)
{
    const int isz = (int)sizeof(int);
    int i, j, k;
    if((cn | srcstep | dststep | (size_t)src | (size_t)dst | (size_t)value) % isz == 0)
    {
        const int* isrc = (const int*)src;
        int* idst = (int*)dst;
        const int* ivalue = (const int*)value;
        int v0 = ivalue[0];
        
        cn /= isz;
        srcstep /= isz;
        dststep /= isz;

        srcroi.width *= cn;
        dstroi.width *= cn;
        left *= cn;

        for(j = 1; j < cn; j++)
            if(ivalue[j] != ivalue[0])
                break;

        if(j == cn)
            cn = 1;

        if(dstroi.width <= 0)
            return CC_OK;

        for(i = 0; i < dstroi.height; i++, idst += dststep)
        {
            if(i < top || i >= top + srcroi.height)
            {
                if(cn == 1)
                {
                    for(j = 0; j < dstroi.width; j++)
                        idst[j] = v0;
                }
                else
                {
                    for(j = 0; j < cn; j++)
                        idst[j] = ivalue[j];
                    for(; j < dstroi.width; j++)
                        idst[j] = idst[j - cn];
                }
                continue;
            }

            if(cn == 1)
            {
                for(j = 0; j < left; j++)
                    idst[j] = v0;
                for(j = srcroi.width + left; j < dstroi.width; j++)
                    idst[j] = v0;
            }
            else
            {
                for(k = 0; k < cn; k++)
                {
                    for(j = 0; j < left; j += cn)
                        idst[j+k] = ivalue[k];
                    for(j = srcroi.width + left; j < dstroi.width; j += cn)
                        idst[j+k] = ivalue[k];
                }
            }

            if(idst + left != isrc)
                for(j = 0; j < srcroi.width; j++)
                    idst[j + left] = isrc[j];
            isrc += srcstep;
        }
    }
    else
    {
        uchar v0 = value[0];
        
        srcroi.width *= cn;
        dstroi.width *= cn;
        left *= cn;

        for(j = 1; j < cn; j++)
            if(value[j] != value[0])
                break;

        if(j == cn)
            cn = 1;

        if(dstroi.width <= 0)
            return CC_OK;

        for(i = 0; i < dstroi.height; i++, dst += dststep)
        {
            if(i < top || i >= top + srcroi.height)
            {
                if(cn == 1)
                {
                    for(j = 0; j < dstroi.width; j++)
                        dst[j] = v0;
                }
                else
                {
                    for(j = 0; j < cn; j++)
                        dst[j] = value[j];
                    for(; j < dstroi.width; j++)
                        dst[j] = dst[j - cn];
                }
                continue;
            }

            if(cn == 1)
            {
                for(j = 0; j < left; j++)
                    dst[j] = v0;
                for(j = srcroi.width + left; j < dstroi.width; j++)
                    dst[j] = v0;
            }
            else
            {
                for(k = 0; k < cn; k++)
                {
                    for(j = 0; j < left; j += cn)
                        dst[j+k] = value[k];
                    for(j = srcroi.width + left; j < dstroi.width; j += cn)
                        dst[j+k] = value[k];
                }
            }

            if(dst + left != src)
                for(j = 0; j < srcroi.width; j++)
                    dst[j + left] = src[j];
            src += srcstep;
        }
    }

    return CC_OK;
}


CC_IMPL void
cvCopyMakeBorderTo(const img_t* src, img_t* dst, CPoint offset,
                  enum CBorderTypes bordertype, CScalar value CC_DEFAULT(cScalarAll(0)))
{
    CC_FUNCNAME("cvCopyMakeBorder");

    __BEGIN__;

    CSize srcsize, dstsize;
    int srcstep, dststep;
    int pix_size, cn = CC_MAT_CN(src);
    TypeId type;

    if(offset.x < 0 || offset.y < 0)
        CC_ERROR(CC_StsOutOfRange, "Offset (left/top border width) is negative");

    if(src->rows + offset.y > dst->rows || src->cols + offset.x > dst->cols)
        CC_ERROR(CC_StsBadSize, "Source array is too big or destination array is too small");

    if(!CC_ARE_TYPES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    type = CC_MAT_DEPTH(src);
    pix_size = CC_TYPE_SIZE(type);
    srcsize = cvGetMatSize(src);
    dstsize = cvGetMatSize(dst);
    srcstep = src->step;
    dststep = dst->step;
    if(srcstep == 0)
        srcstep = CC_STUB_STEP;
    if(dststep == 0)
        dststep = CC_STUB_STEP;

    if(bordertype == CC_BORDER_REPLICATE)
    {
        icvCopyReplicateBorder_8u(src->tt.data, srcstep, srcsize,
                                   dst->tt.data, dststep, dstsize,
                                   offset.y, offset.x, pix_size);
    }
    else if(bordertype == CC_BORDER_REFLECT_101)
    {
        icvCopyReflect101Border_8u(src->tt.data, srcstep, srcsize,
                                   dst->tt.data, dststep, dstsize,
                                   offset.y, offset.x, pix_size);
    }
    else if(bordertype == CC_BORDER_CONSTANT)
    {
        double buf[4];
        cvScalarToRawData(&value, buf, src->tid, 0, 0);
        icvCopyConstBorder_8u(src->tt.data, srcstep, srcsize,
                               dst->tt.data, dststep, dstsize,
                               offset.y, offset.x, pix_size, (uchar*)buf);
    }
    else
        CC_ERROR(CC_StsBadFlag, "Unknown/unsupported border type");
    
    __END__;
}

CC_IMPL img_t*
cvCopyMakeBorder(const img_t* src, img_t* dst, int top, int bottom, int left, int right,
                 enum CBorderTypes bordertype, CScalar value CC_DEFAULT(cScalarAll(0))) {

  if (dst!=src) {
    int h = src->h + top + bottom;
    int w = src->w + left + right;
    cvSetMat(dst, h, w, src->tid, CC_MAT_CN(src), NULL, CC_AUTO_STEP);
  }
  cvCopyMakeBorderTo(src, dst, cPoint(left, top), bordertype, value);
  return dst;
}
