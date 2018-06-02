
#define  ICC_DEF_ACC_FUNC(name, srctype, dsttype, cvtmacro)            \
IPCVAPI_IMPL(CStatus,                                                  \
name,(const srctype *src, int srcstep, dsttype *dst,                    \
       int dststep, CSize size), (src, srcstep, dst, dststep, size))   \
                                                                        \
{                                                                       \
    srcstep /= sizeof(src[0]);                                          \
    dststep /= sizeof(dst[0]);                                          \
                                                                        \
    for(; size.height--; src += srcstep, dst += dststep)                \
    {                                                                   \
        int x;                                                          \
        for(x = 0; x <= size.width - 4; x += 4)                         \
        {                                                               \
            dsttype t0 = dst[x] + cvtmacro(src[x]);                     \
            dsttype t1 = dst[x + 1] + cvtmacro(src[x + 1]);             \
            dst[x] = t0;  dst[x + 1] = t1;                              \
                                                                        \
            t0 = dst[x + 2] + cvtmacro(src[x + 2]);                     \
            t1 = dst[x + 3] + cvtmacro(src[x + 3]);                     \
            dst[x + 2] = t0;  dst[x + 3] = t1;                          \
        }                                                               \
                                                                        \
        for(; x < size.width; x++)                                      \
            dst[x] += cvtmacro(src[x]);                                 \
    }                                                                   \
                                                                        \
    return CC_OK;                                                       \
}


ICC_DEF_ACC_FUNC(icvAdd_8u32f_C1IR, uchar, float, CC_8TO32F)
ICC_DEF_ACC_FUNC(icvAdd_32f_C1IR, float, float, CC_NOP)
ICC_DEF_ACC_FUNC(icvAddSquare_8u32f_C1IR, uchar, float, CC_8TO32F_SQR)
ICC_DEF_ACC_FUNC(icvAddSquare_32f_C1IR, float, float, CC_SQR)


#define  ICC_DEF_ACCPROD_FUNC(flavor, srctype, dsttype, cvtmacro)         \
IPCVAPI_IMPL(CStatus, icvAddProduct_##flavor##_C1IR,                      \
(const srctype *src1, int step1, const srctype *src2, int step2,           \
  dsttype *dst, int dststep, CSize size),                                 \
 (src1, step1, src2, step2, dst, dststep, size))                           \
{                                                                           \
    step1 /= sizeof(src1[0]);                                               \
    step2 /= sizeof(src2[0]);                                               \
    dststep /= sizeof(dst[0]);                                              \
                                                                            \
    for(; size.height--; src1 += step1, src2 += step2, dst += dststep)    \
    {                                                                       \
        int x;                                                              \
        for(x = 0; x <= size.width - 4; x += 4)                           \
        {                                                                   \
            dsttype t0 = dst[x] + cvtmacro(src1[x])*cvtmacro(src2[x]);      \
            dsttype t1 = dst[x+1] + cvtmacro(src1[x+1])*cvtmacro(src2[x+1]);\
            dst[x] = t0;  dst[x + 1] = t1;                                  \
                                                                            \
            t0 = dst[x + 2] + cvtmacro(src1[x + 2])*cvtmacro(src2[x + 2]);  \
            t1 = dst[x + 3] + cvtmacro(src1[x + 3])*cvtmacro(src2[x + 3]);  \
            dst[x + 2] = t0;  dst[x + 3] = t1;                              \
        }                                                                   \
                                                                            \
        for(; x < size.width; x++)                                        \
            dst[x] += cvtmacro(src1[x])*cvtmacro(src2[x]);                  \
    }                                                                       \
                                                                            \
    return CC_OK;                                                           \
}


ICC_DEF_ACCPROD_FUNC(8u32f, uchar, float, CC_8TO32F)
ICC_DEF_ACCPROD_FUNC(32f, float, float, CC_NOP)


#define  ICC_DEF_ACCWEIGHT_FUNC(flavor, srctype, dsttype, cvtmacro)   \
IPCVAPI_IMPL(CStatus, icvAddWeighted_##flavor##_C1IR,                 \
(const srctype *src, int srcstep, dsttype *dst, int dststep,           \
  CSize size, dsttype alpha), (src, srcstep, dst, dststep, size, alpha))\
{                                                                       \
    dsttype beta = (dsttype)(1 - alpha);                                \
    srcstep /= sizeof(src[0]);                                          \
    dststep /= sizeof(dst[0]);                                          \
                                                                        \
    for(; size.height--; src += srcstep, dst += dststep)              \
    {                                                                   \
        int x;                                                          \
        for(x = 0; x <= size.width - 4; x += 4)                       \
        {                                                               \
            dsttype t0 = dst[x]*beta + cvtmacro(src[x])*alpha;          \
            dsttype t1 = dst[x+1]*beta + cvtmacro(src[x+1])*alpha;      \
            dst[x] = t0; dst[x + 1] = t1;                               \
                                                                        \
            t0 = dst[x + 2]*beta + cvtmacro(src[x + 2])*alpha;          \
            t1 = dst[x + 3]*beta + cvtmacro(src[x + 3])*alpha;          \
            dst[x + 2] = t0; dst[x + 3] = t1;                           \
        }                                                               \
                                                                        \
        for(; x < size.width; x++)                                    \
            dst[x] = dst[x]*beta + cvtmacro(src[x])*alpha;              \
    }                                                                   \
                                                                        \
    return CC_OK;                                                       \
}


ICC_DEF_ACCWEIGHT_FUNC(8u32f, uchar, float, CC_8TO32F)
ICC_DEF_ACCWEIGHT_FUNC(32f, float, float, CC_NOP)


#define  ICC_DEF_ACCMASK_FUNC_C1(name, srctype, dsttype, cvtmacro)    \
IPCVAPI_IMPL(CStatus,                                                 \
name,(const srctype *src, int srcstep, const uchar* mask, int maskstep,\
       dsttype *dst, int dststep, CSize size),                        \
       (src, srcstep, mask, maskstep, dst, dststep, size))             \
{                                                                       \
    srcstep /= sizeof(src[0]);                                          \
    dststep /= sizeof(dst[0]);                                          \
                                                                        \
    for(; size.height--; src += srcstep,                               \
                          dst += dststep, mask += maskstep)            \
    {                                                                   \
        int x;                                                          \
        for(x = 0; x <= size.width - 2; x += 2)                       \
        {                                                               \
            if(mask[x])                                               \
                dst[x] += cvtmacro(src[x]);                             \
            if(mask[x+1])                                             \
                dst[x+1] += cvtmacro(src[x+1]);                         \
        }                                                               \
                                                                        \
        for(; x < size.width; x++)                                    \
            if(mask[x])                                               \
                dst[x] += cvtmacro(src[x]);                             \
    }                                                                   \
                                                                        \
    return CC_OK;                                                       \
}


ICC_DEF_ACCMASK_FUNC_C1(icvAdd_8u32f_C1IMR, uchar, float, CC_8TO32F)
ICC_DEF_ACCMASK_FUNC_C1(icvAdd_32f_C1IMR, float, float, CC_NOP)
ICC_DEF_ACCMASK_FUNC_C1(icvAddSquare_8u32f_C1IMR, uchar, float, CC_8TO32F_SQR)
ICC_DEF_ACCMASK_FUNC_C1(icvAddSquare_32f_C1IMR, float, float, CC_SQR)


#define  ICC_DEF_ACCPRODUCTMASK_FUNC_C1(flavor, srctype, dsttype, cvtmacro)  \
IPCVAPI_IMPL(CStatus, icvAddProduct_##flavor##_C1IMR,                 \
(const srctype *src1, int step1, const srctype* src2, int step2,       \
  const uchar* mask, int maskstep, dsttype *dst, int dststep, CSize size),\
  (src1, step1, src2, step2, mask, maskstep, dst, dststep, size))      \
{                                                                       \
    step1 /= sizeof(src1[0]);                                           \
    step2 /= sizeof(src2[0]);                                           \
    dststep /= sizeof(dst[0]);                                          \
                                                                        \
    for(; size.height--; src1 += step1, src2 += step2,                 \
                          dst += dststep, mask += maskstep)            \
    {                                                                   \
        int x;                                                          \
        for(x = 0; x <= size.width - 2; x += 2)                       \
        {                                                               \
            if(mask[x])                                               \
                dst[x] += cvtmacro(src1[x])*cvtmacro(src2[x]);          \
            if(mask[x+1])                                             \
                dst[x+1] += cvtmacro(src1[x+1])*cvtmacro(src2[x+1]);    \
        }                                                               \
                                                                        \
        for(; x < size.width; x++)                                    \
            if(mask[x])                                               \
                dst[x] += cvtmacro(src1[x])*cvtmacro(src2[x]);          \
    }                                                                   \
                                                                        \
    return CC_OK;                                                       \
}


ICC_DEF_ACCPRODUCTMASK_FUNC_C1(8u32f, uchar, float, CC_8TO32F)
ICC_DEF_ACCPRODUCTMASK_FUNC_C1(32f, float, float, CC_NOP)

#define  ICC_DEF_ACCWEIGHTMASK_FUNC_C1(flavor, srctype, dsttype, cvtmacro) \
IPCVAPI_IMPL(CStatus, icvAddWeighted_##flavor##_C1IMR,                \
(const srctype *src, int srcstep, const uchar* mask, int maskstep,     \
  dsttype *dst, int dststep, CSize size, dsttype alpha),              \
  (src, srcstep, mask, maskstep, dst, dststep, size, alpha))           \
{                                                                       \
    dsttype beta = (dsttype)(1 - alpha);                                \
    srcstep /= sizeof(src[0]);                                          \
    dststep /= sizeof(dst[0]);                                          \
                                                                        \
    for(; size.height--; src += srcstep,                               \
                          dst += dststep, mask += maskstep)            \
    {                                                                   \
        int x;                                                          \
        for(x = 0; x <= size.width - 2; x += 2)                       \
        {                                                               \
            if(mask[x])                                               \
                dst[x] = dst[x]*beta + cvtmacro(src[x])*alpha;          \
            if(mask[x+1])                                             \
                dst[x+1] = dst[x+1]*beta + cvtmacro(src[x+1])*alpha;    \
        }                                                               \
                                                                        \
        for(; x < size.width; x++)                                    \
            if(mask[x])                                               \
                dst[x] = dst[x]*beta + cvtmacro(src[x])*alpha;          \
    }                                                                   \
                                                                        \
    return CC_OK;                                                       \
}

ICC_DEF_ACCWEIGHTMASK_FUNC_C1(8u32f, uchar, float, CC_8TO32F)
ICC_DEF_ACCWEIGHTMASK_FUNC_C1(32f, float, float, CC_NOP)


#define  ICC_DEF_ACCMASK_FUNC_C3(name, srctype, dsttype, cvtmacro)    \
IPCVAPI_IMPL(CStatus,                                                 \
name,(const srctype *src, int srcstep, const uchar* mask, int maskstep,\
       dsttype *dst, int dststep, CSize size),                        \
       (src, srcstep, mask, maskstep, dst, dststep, size))             \
{                                                                       \
    srcstep /= sizeof(src[0]);                                          \
    dststep /= sizeof(dst[0]);                                          \
                                                                        \
    for(; size.height--; src += srcstep,                               \
                          dst += dststep, mask += maskstep)            \
    {                                                                   \
        int x;                                                          \
        for(x = 0; x < size.width; x++)                               \
            if(mask[x])                                               \
            {                                                           \
                dsttype t0, t1, t2;                                     \
                t0 = dst[x*3] + cvtmacro(src[x*3]);                     \
                t1 = dst[x*3+1] + cvtmacro(src[x*3+1]);                 \
                t2 = dst[x*3+2] + cvtmacro(src[x*3+2]);                 \
                dst[x*3] = t0;                                          \
                dst[x*3+1] = t1;                                        \
                dst[x*3+2] = t2;                                        \
            }                                                           \
    }                                                                   \
                                                                        \
    return CC_OK;                                                       \
}


ICC_DEF_ACCMASK_FUNC_C3(icvAdd_8u32f_C3IMR, uchar, float, CC_8TO32F)
ICC_DEF_ACCMASK_FUNC_C3(icvAdd_32f_C3IMR, float, float, CC_NOP)
ICC_DEF_ACCMASK_FUNC_C3(icvAddSquare_8u32f_C3IMR, uchar, float, CC_8TO32F_SQR)
ICC_DEF_ACCMASK_FUNC_C3(icvAddSquare_32f_C3IMR, float, float, CC_SQR)


#define  ICC_DEF_ACCPRODUCTMASK_FUNC_C3(flavor, srctype, dsttype, cvtmacro)  \
IPCVAPI_IMPL(CStatus, icvAddProduct_##flavor##_C3IMR,                 \
(const srctype *src1, int step1, const srctype* src2, int step2,       \
  const uchar* mask, int maskstep, dsttype *dst, int dststep, CSize size),\
  (src1, step1, src2, step2, mask, maskstep, dst, dststep, size))      \
{                                                                       \
    step1 /= sizeof(src1[0]);                                           \
    step2 /= sizeof(src2[0]);                                           \
    dststep /= sizeof(dst[0]);                                          \
                                                                        \
    for(; size.height--; src1 += step1, src2 += step2,                 \
                          dst += dststep, mask += maskstep)            \
    {                                                                   \
        int x;                                                          \
        for(x = 0; x < size.width; x++)                               \
            if(mask[x])                                               \
            {                                                           \
                dsttype t0, t1, t2;                                     \
                t0 = dst[x*3]+cvtmacro(src1[x*3])*cvtmacro(src2[x*3]);  \
                t1 = dst[x*3+1]+cvtmacro(src1[x*3+1])*cvtmacro(src2[x*3+1]);\
                t2 = dst[x*3+2]+cvtmacro(src1[x*3+2])*cvtmacro(src2[x*3+2]);\
                dst[x*3] = t0;                                          \
                dst[x*3+1] = t1;                                        \
                dst[x*3+2] = t2;                                        \
            }                                                           \
    }                                                                   \
                                                                        \
    return CC_OK;                                                       \
}


ICC_DEF_ACCPRODUCTMASK_FUNC_C3(8u32f, uchar, float, CC_8TO32F)
ICC_DEF_ACCPRODUCTMASK_FUNC_C3(32f, float, float, CC_NOP)


#define  ICC_DEF_ACCWEIGHTMASK_FUNC_C3(flavor, srctype, dsttype, cvtmacro) \
IPCVAPI_IMPL(CStatus, icvAddWeighted_##flavor##_C3IMR,                \
(const srctype *src, int srcstep, const uchar* mask, int maskstep,     \
  dsttype *dst, int dststep, CSize size, dsttype alpha),              \
  (src, srcstep, mask, maskstep, dst, dststep, size, alpha))           \
{                                                                       \
    dsttype beta = (dsttype)(1 - alpha);                                \
    srcstep /= sizeof(src[0]);                                          \
    dststep /= sizeof(dst[0]);                                          \
                                                                        \
    for(; size.height--; src += srcstep,                               \
                          dst += dststep, mask += maskstep)            \
    {                                                                   \
        int x;                                                          \
        for(x = 0; x < size.width; x++)                               \
            if(mask[x])                                               \
            {                                                           \
                dsttype t0, t1, t2;                                     \
                t0 = dst[x*3]*beta + cvtmacro(src[x*3])*alpha;          \
                t1 = dst[x*3+1]*beta + cvtmacro(src[x*3+1])*alpha;      \
                t2 = dst[x*3+2]*beta + cvtmacro(src[x*3+2])*alpha;      \
                dst[x*3] = t0;                                          \
                dst[x*3+1] = t1;                                        \
                dst[x*3+2] = t2;                                        \
            }                                                           \
    }                                                                   \
                                                                        \
    return CC_OK;                                                       \
}

ICC_DEF_ACCWEIGHTMASK_FUNC_C3(8u32f, uchar, float, CC_8TO32F)
ICC_DEF_ACCWEIGHTMASK_FUNC_C3(32f, float, float, CC_NOP)


#define  ICC_DEF_INIT_ACC_TAB(FUNCNAME)                                           \
static  void  icvInit##FUNCNAME##Table(CvFuncTable  tab, CvBigFuncTable  masktab) \
{                                                                                   \
    tab[CC_8U] = (void*)icv##FUNCNAME##_8u32f_C1IR;                          \
    tab[CC_32F] = (void*)icv##FUNCNAME##_32f_C1IR;                           \
                                                                                    \
    masktab[CC_8U][1] = (void*)icv##FUNCNAME##_8u32f_C1IMR;                   \
    masktab[CC_32F][1] = (void*)icv##FUNCNAME##_32f_C1IMR;                    \
                                                                                    \
    masktab[CC_8U][3] = (void*)icv##FUNCNAME##_8u32f_C3IMR;                   \
    masktab[CC_32F][3] = (void*)icv##FUNCNAME##_32f_C3IMR;                    \
}


ICC_DEF_INIT_ACC_TAB(Add)
ICC_DEF_INIT_ACC_TAB(AddSquare)
ICC_DEF_INIT_ACC_TAB(AddProduct)
ICC_DEF_INIT_ACC_TAB(AddWeighted)


CC_IMPL void
cvAcc(const img_t* mat, img_t* sum, const img_t* mask CC_DEFAULT(NULL))
{
    static CvFuncTable acc_tab;
    static CvBigFuncTable accmask_tab;
    static int inittab = 0;
    
    CC_FUNCNAME("cvAcc");

    __BEGIN__;

    TypeId type, sumtype;
    int mat_step, sum_step, mask_step = 0, cn;
    CSize size;

    if(!inittab)
    {
        icvInitAddTable(acc_tab, accmask_tab);
        inittab = 1;
    }

    if(CC_MAT_TYPE(sum) != CC_32F)
        CC_ERROR(CC_BadDepth, "");

    if(!CC_ARE_CNS_EQ(mat, sum))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    sumtype = CC_MAT_DEPTH(sum);
    if(sumtype != CC_32F && (mask != 0 || sumtype != CC_64F))
        CC_ERROR(CC_BadDepth, "Bad accumulator type");

    if(!CC_ARE_SIZES_EQ(mat, sum))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    size = cvGetMatSize(mat);
    type = CC_MAT_DEPTH(mat);
    cn = CC_MAT_CN(mat);

    mat_step = mat->step;
    sum_step = sum->step;

    if(!mask)
    {
        CvFunc2D_2A func=(CvFunc2D_2A)acc_tab[type];

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "Unsupported type combination");

        size.width *= CC_MAT_CN(mat);
        if(CC_IS_MAT_CONT(mat) && CC_IS_MAT_CONT(sum))
        {
            size.width *= size.height;
            mat_step = sum_step = CC_STUB_STEP;
            size.height = 1;
        }

        IPPI_CALL(func(mat->tt.data, mat_step, sum->tt.data, sum_step, size));
    }
    else
    {
        CvFunc2D_3A func = (CvFunc2D_3A)accmask_tab[type][cn];

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        if(!CC_IS_MASK_ARR(mask))
            CC_ERROR(CC_StsBadMask, "");

        if(!CC_ARE_SIZES_EQ(mat, mask))
            CC_ERROR(CC_StsUnmatchedSizes, "");            

        mask_step = mask->step;

        if(CC_IS_MAT_CONT(mat) && CC_IS_MAT_CONT(sum) && CC_IS_MAT_CONT(mask)) 
        {
            size.width *= size.height;
            mat_step = sum_step = mask_step = CC_STUB_STEP;
            size.height = 1;
        }

        IPPI_CALL(func(mat->tt.data, mat_step, mask->tt.data, mask_step,
                         sum->tt.data, sum_step, size));
    }

    __END__;
}


CC_IMPL void
cvSquareAcc(const img_t* mat, img_t* sum, const img_t* mask)
{
    static CvFuncTable acc_tab;
    static CvBigFuncTable accmask_tab;
    static int inittab = 0;
    
    CC_FUNCNAME("cvSquareAcc");

    __BEGIN__;

    TypeId type;
    int mat_step, sum_step, mask_step = 0, cn;
    CSize size;

    if(!inittab)
    {
        icvInitAddSquareTable(acc_tab, accmask_tab);
        inittab = 1;
    }

    if(!CC_ARE_CNS_EQ(mat, sum))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(CC_MAT_TYPE(sum) != CC_32F)
        CC_ERROR(CC_BadDepth, "");

    if(!CC_ARE_SIZES_EQ(mat, sum))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    size = cvGetMatSize(mat);
    type = CC_MAT_DEPTH(mat);
    cn = CC_MAT_CN(mat);

    mat_step = mat->step;
    sum_step = sum->step;

    if(!mask)
    {
        CvFunc2D_2A func = (CvFunc2D_2A)acc_tab[type];

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        size.width *= CC_MAT_CN(mat);

        if(CC_IS_MAT_CONT(mat) && CC_IS_MAT_CONT(sum))
        {
            size.width *= size.height;
            mat_step = sum_step = CC_STUB_STEP;;
            size.height = 1;
        }

        IPPI_CALL(func(mat->tt.data, mat_step, sum->tt.data, sum_step, size));
    }
    else
    {
        CvFunc2D_3A func = (CvFunc2D_3A)accmask_tab[type][cn];

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        if(!CC_IS_MASK_ARR(mask))
            CC_ERROR(CC_StsBadMask, "");

        if(!CC_ARE_SIZES_EQ(mat, mask))
            CC_ERROR(CC_StsUnmatchedSizes, "");            

        mask_step = mask->step;

        if(CC_IS_MAT_CONT(mat) && CC_IS_MAT_CONT(sum) && CC_IS_MAT_CONT(mask))
        {
            size.width *= size.height;
            mat_step = sum_step = mask_step = CC_STUB_STEP;
            size.height = 1;
        }

        IPPI_CALL(func(mat->tt.data, mat_step, mask->tt.data, mask_step,
                         sum->tt.data, sum_step, size));
    }

    __END__;
}


CC_IMPL void
cvMultiplyAcc(const img_t* mat1, const img_t* mat2,
              img_t* sum, const img_t* mask)
{
    static CvFuncTable acc_tab;
    static CvBigFuncTable accmask_tab;
    static int inittab = 0;
    
    CC_FUNCNAME("cvMultiplyAcc");

    __BEGIN__;

    TypeId type;
    int mat1_step, mat2_step, sum_step, mask_step = 0, cn;
    CSize size;

    if(!inittab)
    {
        icvInitAddProductTable(acc_tab, accmask_tab);
        inittab = 1;
    }

    if(!CC_ARE_CNS_EQ(mat1, mat2) || !CC_ARE_CNS_EQ(mat1, sum))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(CC_MAT_TYPE(sum) != CC_32F)
        CC_ERROR(CC_BadDepth, "");

    if(!CC_ARE_SIZES_EQ(mat1, sum) || !CC_ARE_SIZES_EQ(mat2, sum))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    size = cvGetMatSize(mat1);
    type = CC_MAT_DEPTH(mat1);
    cn = CC_MAT_CN(mat1);

    mat1_step = mat1->step;
    mat2_step = mat2->step;
    sum_step = sum->step;

    if(!mask)
    {
        CvFunc2D_3A func = (CvFunc2D_3A)acc_tab[type];

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        size.width *= CC_MAT_CN(mat1);

        if(CC_IS_MAT_CONT(mat1) && CC_IS_MAT_CONT(mat2) && CC_IS_MAT_CONT(sum))
        {
            size.width *= size.height;
            mat1_step = mat2_step = sum_step = CC_STUB_STEP;
            size.height = 1;
        }

        IPPI_CALL(func(mat1->tt.data, mat1_step, mat2->tt.data, mat2_step,
                         sum->tt.data, sum_step, size));
    }
    else
    {
        CvFunc2D_4A func = (CvFunc2D_4A)accmask_tab[type][cn];

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        if(!CC_IS_MASK_ARR(mask))
            CC_ERROR(CC_StsBadMask, "");

        if(!CC_ARE_SIZES_EQ(mat1, mask))
            CC_ERROR(CC_StsUnmatchedSizes, "");

        mask_step = mask->step;

        if(CC_IS_MAT_CONT(mat1) && CC_IS_MAT_CONT(mat2) && CC_IS_MAT_CONT(sum) && CC_IS_MAT_CONT(mask))
        {
            size.width *= size.height;
            mat1_step = mat2_step = sum_step = mask_step = CC_STUB_STEP;
            size.height = 1;
        }

        IPPI_CALL(func(mat1->tt.data, mat1_step, mat2->tt.data, mat2_step,
                         mask->tt.data, mask_step,
                         sum->tt.data, sum_step, size));
    }

    __END__;
}


typedef CStatus (C_STDCALL *CvAddWeightedFuncRun)(const void* src, int srcstep,
                                                  void* dst, int dststep,
                                                  CSize size, float alpha);

typedef CStatus (C_STDCALL *CvAddWeightedMaskFunc)(const void* src, int srcstep,
                                                      void* dst, int dststep,
                                                      const void* mask, int maskstep,
                                                      CSize size, float alpha);

CC_IMPL void
cvRunningAvg(const img_t* mat, img_t* sum,
              double alpha, const img_t* mask)
{
    static CvFuncTable acc_tab;
    static CvBigFuncTable accmask_tab;
    static int inittab = 0;
    
    CC_FUNCNAME("cvRunningAvg");

    __BEGIN__;

    TypeId type;
    int mat_step, sum_step, mask_step = 0, cn;
    CSize size;

    if(!inittab)
    {
        icvInitAddWeightedTable(acc_tab, accmask_tab);
        inittab = 1;
    }

	if(!CC_ARE_CNS_EQ(mat, sum))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(CC_MAT_TYPE(sum) != CC_32F)
        CC_ERROR(CC_BadDepth, "");

    if(!CC_ARE_SIZES_EQ(mat, sum))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    size = cvGetMatSize(mat);
    type = CC_MAT_DEPTH(mat);
    cn = CC_MAT_CN(mat);

    mat_step = mat->step;
    sum_step = sum->step;

    if(!mask)
    {
        CvAddWeightedFuncRun func = (CvAddWeightedFuncRun)acc_tab[type];

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        size.width *= CC_MAT_CN(mat);
        if(CC_IS_MAT_CONT(mat) && CC_IS_MAT_CONT(sum))
        {
            size.width *= size.height;
            mat_step = sum_step = CC_STUB_STEP;
            size.height = 1;
        }

        IPPI_CALL(func(mat->tt.data, mat_step,
                         sum->tt.data, sum_step, size, (float)alpha));
    }
    else
    {
        CvAddWeightedMaskFunc func = (CvAddWeightedMaskFunc)accmask_tab[type][cn];

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        if(!CC_IS_MASK_ARR(mask))
            CC_ERROR(CC_StsBadMask, "");

        if(!CC_ARE_SIZES_EQ(mat, mask))
            CC_ERROR(CC_StsUnmatchedSizes, "");

        mask_step = mask->step;

        if(CC_IS_MAT_CONT(mat) && CC_IS_MAT_CONT(sum) && CC_IS_MAT_CONT(mask))
        {
            size.width *= size.height;
            mat_step = sum_step = mask_step = CC_STUB_STEP;
            size.height = 1;
        }

        IPPI_CALL(func(mat->tt.data, mat_step, mask->tt.data, mask_step,
                         sum->tt.data, sum_step, size, (float)alpha));
    }

    __END__;
}



