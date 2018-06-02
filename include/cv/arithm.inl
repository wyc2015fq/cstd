

/* ////////////////////////////////////////////////////////////////////
//
//  img_t arithmetic operations: +, - ...
//
// */


/****************************************************************************************\
*                      Arithmetic operations (+, -) without mask                         *
\****************************************************************************************/

#define ICC_DEF_BIN_ARI_OP_CASE(__op__, worktype, cast_macro, len)  \
{                                                                   \
    int i;                                                          \
                                                                    \
    for(i = 0; i <= (len) - 4; i += 4)                              \
    {                                                               \
        worktype t0 = __op__((src1)[i], (src2)[i]);                 \
        worktype t1 = __op__((src1)[i+1], (src2)[i+1]);             \
                                                                    \
        (dst)[i] = cast_macro(t0);                                  \
        (dst)[i+1] = cast_macro(t1);                                \
                                                                    \
        t0 = __op__((src1)[i+2],(src2)[i+2]);                       \
        t1 = __op__((src1)[i+3],(src2)[i+3]);                       \
                                                                    \
        (dst)[i+2] = cast_macro(t0);                                \
        (dst)[i+3] = cast_macro(t1);                                \
    }                                                               \
                                                                    \
    for(; i < (len); i++)                                           \
    {                                                               \
        worktype t0 = __op__((src1)[i],(src2)[i]);                  \
        (dst)[i] = cast_macro(t0);                                  \
    }                                                               \
}

#define ICC_DEF_BIN_ARI_OP_2D(__op__, name, type, worktype, cast_macro)     \
IPCVAPI_IMPL(CStatus, name,                                                \
    (const type* src1, int step1, const type* src2, int step2,              \
      type* dst, int step, CSize size),                                    \
      (src1, step1, src2, step2, dst, step, size))                          \
{                                                                           \
    step1/=sizeof(src1[0]); step2/=sizeof(src2[0]); step/=sizeof(dst[0]);   \
                                                                            \
    if(size.width == 1)                                                     \
    {                                                                       \
        for(; size.height--; src1 += step1, src2 += step2, dst += step)     \
        {                                                                   \
            worktype t0 = __op__((src1)[0],(src2)[0]);                      \
            (dst)[0] = cast_macro(t0);                                      \
        }                                                                   \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        for(; size.height--; src1 += step1, src2 += step2, dst += step)     \
        {                                                                   \
            ICC_DEF_BIN_ARI_OP_CASE(__op__, worktype,                       \
                                     cast_macro, size.width);               \
        }                                                                   \
    }                                                                       \
                                                                            \
    return CC_OK;                                                           \
}


#define ICC_DEF_BIN_ARI_OP_2D_SFS(__op__, name, type, worktype, cast_macro) \
IPCVAPI_IMPL(CStatus, name,                                                \
    (const type* src1, int step1, const type* src2, int step2,              \
      type* dst, int step, CSize size, int scalefactor),               \
      (src1, step1, src2, step2, dst, step, size, 0))                       \
{                                                                           \
    step1/=sizeof(src1[0]); step2/=sizeof(src2[0]); step/=sizeof(dst[0]);   \
                                                                            \
    if(size.width == 1)                                                     \
    {                                                                       \
        for(; size.height--; src1 += step1, src2 += step2, dst += step)     \
        {                                                                   \
            worktype t0 = __op__((src1)[0],(src2)[0]);                      \
            (dst)[0] = cast_macro(t0);                                      \
        }                                                                   \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        for(; size.height--; src1 += step1, src2 += step2, dst += step)     \
        {                                                                   \
            ICC_DEF_BIN_ARI_OP_CASE(__op__, worktype,                       \
                                     cast_macro, size.width);               \
        }                                                                   \
    }                                                                       \
                                                                            \
    return CC_OK;                                                           \
}


#define ICC_DEF_UN_ARI_OP_CASE(__op__, worktype, cast_macro,                \
                                src, scalar, dst, len)                      \
{                                                                           \
    int i;                                                                  \
                                                                            \
    for(; ((len) -= 12) >= 0; (dst) += 12, (src) += 12)                     \
    {                                                                       \
        worktype t0 = __op__((scalar)[0], (src)[0]);                        \
        worktype t1 = __op__((scalar)[1], (src)[1]);                        \
                                                                            \
        (dst)[0] = cast_macro(t0);                                          \
        (dst)[1] = cast_macro(t1);                                          \
                                                                            \
        t0 = __op__((scalar)[2], (src)[2]);                                 \
        t1 = __op__((scalar)[3], (src)[3]);                                 \
                                                                            \
        (dst)[2] = cast_macro(t0);                                          \
        (dst)[3] = cast_macro(t1);                                          \
                                                                            \
        t0 = __op__((scalar)[4], (src)[4]);                                 \
        t1 = __op__((scalar)[5], (src)[5]);                                 \
                                                                            \
        (dst)[4] = cast_macro(t0);                                          \
        (dst)[5] = cast_macro(t1);                                          \
                                                                            \
        t0 = __op__((scalar)[6], (src)[6]);                                 \
        t1 = __op__((scalar)[7], (src)[7]);                                 \
                                                                            \
        (dst)[6] = cast_macro(t0);                                          \
        (dst)[7] = cast_macro(t1);                                          \
                                                                            \
        t0 = __op__((scalar)[8], (src)[8]);                                 \
        t1 = __op__((scalar)[9], (src)[9]);                                 \
                                                                            \
        (dst)[8] = cast_macro(t0);                                          \
        (dst)[9] = cast_macro(t1);                                          \
                                                                            \
        t0 = __op__((scalar)[10], (src)[10]);                               \
        t1 = __op__((scalar)[11], (src)[11]);                               \
                                                                            \
        (dst)[10] = cast_macro(t0);                                         \
        (dst)[11] = cast_macro(t1);                                         \
    }                                                                       \
                                                                            \
    for((len) += 12, i = 0; i < (len); i++)                                 \
    {                                                                       \
        worktype t0 = __op__((scalar)[i],(src)[i]);                         \
        (dst)[i] = cast_macro(t0);                                          \
    }                                                                       \
}


#define ICC_DEF_UN_ARI_OP_2D(__op__, name, type, worktype, cast_macro)    \
static CStatus C_STDCALL name                                             \
    (const type* src, int step1, type* dst, int step,                      \
      CSize size, const worktype* scalar)                                 \
{                                                                           \
    step1 /= sizeof(src[0]); step /= sizeof(dst[0]);                        \
                                                                            \
    if(size.width == 1)                                                   \
    {                                                                       \
        for(; size.height--; src += step1, dst += step)                   \
        {                                                                   \
            worktype t0 = __op__(*(scalar),*(src));                         \
            *(dst) = cast_macro(t0);                                      \
        }                                                                   \
    }                                                                       \
    else                                                                    \
    {                                                                       \
        for(; size.height--; src += step1, dst += step)                   \
        {                                                                   \
            const type *tsrc = src;                                         \
            type *tdst = dst;                                               \
            int width = size.width;                                         \
                                                                            \
            ICC_DEF_UN_ARI_OP_CASE(__op__, worktype, cast_macro,           \
                                    tsrc, scalar, tdst, width);            \
        }                                                                   \
    }                                                                       \
                                                                            \
    return CC_OK;                                                           \
}


#define ICC_DEF_BIN_ARI_ALL(__op__, name, cast_8u)                                \
ICC_DEF_BIN_ARI_OP_2D_SFS(__op__, icv##name##_8u_C1R, uchar, int, cast_8u)        \
ICC_DEF_BIN_ARI_OP_2D_SFS(__op__, icv##name##_16u_C1R, ushort, int, CC_CAST_16U)  \
ICC_DEF_BIN_ARI_OP_2D_SFS(__op__, icv##name##_16s_C1R, short, int, CC_CAST_16S)   \
ICC_DEF_BIN_ARI_OP_2D(__op__, icv##name##_32s_C1R, int, int, CC_CAST_32S)         \
ICC_DEF_BIN_ARI_OP_2D(__op__, icv##name##_32f_C1R, float, float, CC_CAST_32F)     \
ICC_DEF_BIN_ARI_OP_2D(__op__, icv##name##_64f_C1R, double, double, CC_CAST_64F)

#define ICC_DEF_UN_ARI_ALL(__op__, name)                                          \
ICC_DEF_UN_ARI_OP_2D(__op__, icv##name##_8u_C1R, uchar, int, CC_CAST_8U)          \
ICC_DEF_UN_ARI_OP_2D(__op__, icv##name##_16u_C1R, ushort, int, CC_CAST_16U)       \
ICC_DEF_UN_ARI_OP_2D(__op__, icv##name##_16s_C1R, short, int, CC_CAST_16S)        \
ICC_DEF_UN_ARI_OP_2D(__op__, icv##name##_32s_C1R, int, int, CC_CAST_32S)          \
ICC_DEF_UN_ARI_OP_2D(__op__, icv##name##_32f_C1R, float, float, CC_CAST_32F)      \
ICC_DEF_UN_ARI_OP_2D(__op__, icv##name##_64f_C1R, double, double, CC_CAST_64F)

#undef CC_SUB_R
#define CC_SUB_R(a,b) ((b) - (a))

ICC_DEF_BIN_ARI_ALL(CC_ADD, Add, CC_FAST_CAST_8U)
ICC_DEF_BIN_ARI_ALL(CC_SUB_R, Sub, CC_FAST_CAST_8U)

ICC_DEF_UN_ARI_ALL(CC_ADD, AddC)
ICC_DEF_UN_ARI_ALL(CC_SUB, SubRC)

#define ICC_DEF_INIT_ARITHM_FUNC_TAB(FUNCNAME, FLAG)          \
static  void  icvInit##FUNCNAME##FLAG##Table(CvFuncTable  tab)\
{                                                               \
    tab[CC_8U] = (void*)icv##FUNCNAME##_8u_##FLAG;       \
    tab[CC_8S] = 0;                                      \
    tab[CC_16U] = (void*)icv##FUNCNAME##_16u_##FLAG;     \
    tab[CC_16S] = (void*)icv##FUNCNAME##_16s_##FLAG;     \
    tab[CC_32S] = (void*)icv##FUNCNAME##_32s_##FLAG;     \
    tab[CC_32F] = (void*)icv##FUNCNAME##_32f_##FLAG;     \
    tab[CC_64F] = (void*)icv##FUNCNAME##_64f_##FLAG;     \
}

ICC_DEF_INIT_ARITHM_FUNC_TAB(Sub, C1R)
ICC_DEF_INIT_ARITHM_FUNC_TAB(SubRC, C1R)
ICC_DEF_INIT_ARITHM_FUNC_TAB(Add, C1R)
ICC_DEF_INIT_ARITHM_FUNC_TAB(AddC, C1R)

/****************************************************************************************\
*                       External Functions for Arithmetic Operations                     *
\****************************************************************************************/

/*************************************** S U B ******************************************/

CC_IMPL img_t*
cvSub(const img_t* src1, const img_t* src2, img_t* dst, const img_t* mask CC_DEFAULT(NULL))
{
    static CvFuncTable sub_tab;
    static int inittab = 0;
    int local_alloc = 1;
    uchar* buffer = 0;

    CC_FUNCNAME("cvSub");

    __BEGIN__;

    const img_t* tmp;
    TypeId type;
    int y, dy, cn, cont_flag = 0;
    int src1_step, src2_step, dst_step, tdst_step, mask_step;
    img_t dstbuf[1], *tdst;
    CvFunc2D_3A func;
    CvFunc2D_3A1I func_sfs;
    CvCopyMaskFunc copym_func;
    CSize size, tsize;

    if (dst!=src1 && dst!=src2) {
      cvSetMatFromMat(dst, src1);
    }

    CC_SWAP(src1, src2, tmp); // to comply with IPP

    if(!CC_ARE_TYPES_EQ(src1, src2) || !CC_ARE_TYPES_EQ(src1, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedFormats);

    if(!CC_ARE_SIZES_EQ(src1, src2) || !CC_ARE_SIZES_EQ(src1, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    type = CC_MAT_DEPTH(src1);
    size = cvGetMatSize(src1);
    cn = CC_MAT_CN(src1);

    if(!mask)
    {
        if(CC_IS_MAT_CONT(src1) && CC_IS_MAT_CONT(src2) && CC_IS_MAT_CONT(dst))
        {
            int len = size.width*size.height*cn;

            if(len <= CC_MAX_INLINE_MAT_OP_SIZE*CC_MAX_INLINE_MAT_OP_SIZE)
            {
                if(type == CC_32F)
                {
                    const float* src1data = (const float*)(src1->tt.data);
                    const float* src2data = (const float*)(src2->tt.data);
                    float* dstdata = (float*)(dst->tt.data);

                    do
                    {
                        dstdata[len-1] = (float)(src2data[len-1] - src1data[len-1]);
                    }
                    while(--len);

                    EXIT;
                }

                if(type == CC_64F)
                {
                    const double* src1data = (const double*)(src1->tt.data);
                    const double* src2data = (const double*)(src2->tt.data);
                    double* dstdata = (double*)(dst->tt.data);

                    do
                    {
                        dstdata[len-1] = src2data[len-1] - src1data[len-1];
                    }
                    while(--len);

                    EXIT;
                }
            }
            cont_flag = 1;
        }

        dy = size.height;
        copym_func = 0;
        tdst = dst;
    }
    else
    {
        int buf_size, elem_size;
        
        if(!CC_IS_MASK_ARR(mask))
            CC_ERROR(CC_StsBadMask, "");

        if(!CC_ARE_SIZES_EQ(mask, dst))
            CC_ERROR(CC_StsUnmatchedSizes, "");

        cont_flag = CC_IS_MAT_CONT(src1) && CC_IS_MAT_CONT(src2) && CC_IS_MAT_CONT(dst) && CC_IS_MAT_CONT(mask);
        elem_size = CC_TYPE_SIZE(type);

        dy = CC_MAX_LOCAL_SIZE/(elem_size*size.height);
        dy = MAX(dy,1);
        dy = MIN(dy,size.height);
        cvMat(dstbuf, dy, size.width, type, cn, NULL, CC_AUTO_STEP);
        if(!cont_flag)
            dstbuf->step = cvAlign(dstbuf->step, 8);
        buf_size = dstbuf->step ? dstbuf->step*dy : size.width*elem_size;
        if(buf_size > CC_MAX_LOCAL_SIZE)
        {
            CC_CALL(buffer = (uchar*)cAlloc(buf_size));
            local_alloc = 0;
        }
        else
            buffer = (uchar*)cvStackAlloc(buf_size);
        dstbuf->tt.data = buffer;
        tdst = dstbuf;
        
        copym_func = icvGetCopyMaskFunc(elem_size);
    }

    if(!inittab)
    {
        icvInitSubC1RTable(sub_tab);
        inittab = 1;
    }

    if(type <= CC_16S)
    {
        func = 0;
        func_sfs = (CvFunc2D_3A1I)(sub_tab[type]);
        if(!func_sfs)
            CC_ERROR(CC_StsUnsupportedFormat, "");
    }
    else
    {
        func_sfs = 0;
        func = (CvFunc2D_3A)(sub_tab[type]);
        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");
    }

    src1_step = src1->step;
    src2_step = src2->step;
    dst_step = dst->step;
    tdst_step = tdst->step;
    mask_step = mask ? mask->step : 0;

    for(y = 0; y < size.height; y += dy)
    {
        tsize.width = size.width;
        tsize.height = dy;
        if(y + dy > size.height)
            tsize.height = size.height - y;
        if(cont_flag || tsize.height == 1)
        {
            tsize.width *= tsize.height;
            tsize.height = 1;
            src1_step = src2_step = tdst_step = dst_step = mask_step = CC_STUB_STEP;
        }

        IPPI_CALL(type <= CC_16S ?
            func_sfs(src1->tt.data + y*src1->step, src1_step,
                      src2->tt.data + y*src2->step, src2_step,
                      tdst->tt.data, tdst_step,
                      cSize(tsize.width*cn, tsize.height), 0) :
            func(src1->tt.data + y*src1->step, src1_step,
                  src2->tt.data + y*src2->step, src2_step,
                  tdst->tt.data, tdst_step,
                  cSize(tsize.width*cn, tsize.height)));

        if(mask)
        {
            IPPI_CALL(copym_func(tdst->tt.data, tdst_step, dst->tt.data + y*dst->step,
                                   dst_step, tsize, mask->tt.data + y*mask->step, mask_step));
        }
    }

    __END__;

    if(!local_alloc)
        cFree(&buffer);
    return dst;
}

CC_IMPL img_t*
cvSubRS(const img_t* src, CScalar scalar, img_t* dst, const img_t* mask CC_DEFAULT(NULL))
{
    static CvFuncTable subr_tab;
    static int inittab = 0;
    int local_alloc = 1;
    uchar* buffer = 0;

    CC_FUNCNAME("cvSubRS");

    __BEGIN__;

    TypeId type, sctype;
    int y, dy, cn, coi = 0, cont_flag = 0;
    int src_step, dst_step, tdst_step, mask_step;
    img_t dstbuf[1], *tdst;
    CvFunc2D_2A1P func;
    CvCopyMaskFunc copym_func;
    double buf[12];
    CSize size, tsize;

    if(!inittab)
    {
        icvInitSubRCC1RTable(subr_tab);
        inittab = 1;
    }

	sctype = type = CC_MAT_DEPTH(src);

	cn = CC_MAT_CN(src);
	if (type < CC_32S)
		sctype = CC_32S;

	size = cvGetMatSize(src);
	cvSetMat(dst, size.height, size.width, type, cn, NULL, CC_AUTO_STEP);

    if(!CC_ARE_TYPES_EQ(src, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedFormats);

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    if(!mask)
    {
        if(CC_IS_MAT_CONT(src) && CC_IS_MAT_CONT(dst))
        {
            if(size.width <= CC_MAX_INLINE_MAT_OP_SIZE)
            {
                int len = size.width * size.height;

                if(type == CC_32F && cn==1)
                {
                    const float* srcdata = (const float*)(src->tt.data);
                    float* dstdata = (float*)(dst->tt.data);
                
                    do
                    {
                        dstdata[len-1] = (float)(scalar.val[0] - srcdata[len-1]);
                    }
                    while(--len);

                    EXIT;
                }

                if(type == CC_64F && 1==cn)
                {
                    const double* srcdata = (const double*)(src->tt.data);
                    double* dstdata = (double*)(dst->tt.data);
                
                    do
                    {
                        dstdata[len-1] = scalar.val[0] - srcdata[len-1];
                    }
                    while(--len);

                    EXIT;
                }
            }
            cont_flag = 1;
        }
        
        dy = size.height;
        copym_func = 0;
        tdst = dst;
    }
    else
    {
        int buf_size, elem_size;
        
        if(!CC_IS_MASK_ARR(mask))
            CC_ERROR(CC_StsBadMask, "");

        if(!CC_ARE_SIZES_EQ(mask, dst))
            CC_ERROR(CC_StsUnmatchedSizes, "");

        cont_flag = CC_IS_MAT_CONT(src) && CC_IS_MAT_CONT(dst) && CC_IS_MAT_CONT(mask);
        elem_size = CC_TYPE_SIZE(type);

        dy = CC_MAX_LOCAL_SIZE/(elem_size*size.height);
        dy = MAX(dy,1);
        dy = MIN(dy,size.height);
        cvMat(dstbuf, dy, size.width, type, cn, NULL, CC_AUTO_STEP);
        if(!cont_flag)
            dstbuf->step = cvAlign(dstbuf->step, 8);
        buf_size = dstbuf->step ? dstbuf->step*dy : size.width*elem_size;
        if(buf_size > CC_MAX_LOCAL_SIZE)
        {
            CC_CALL(buffer = (uchar*)cAlloc(buf_size));
            local_alloc = 0;
        }
        else
            buffer = (uchar*)cvStackAlloc(buf_size);
        dstbuf->tt.data = buffer;
        tdst = dstbuf;
        
        copym_func = icvGetCopyMaskFunc(elem_size);
    }

    func = (CvFunc2D_2A1P)(subr_tab[type]);
    if(!func)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    src_step = src->step;
    dst_step = dst->step;
    tdst_step = tdst->step;
    mask_step = mask ? mask->step : 0;

    CC_CALL(cvScalarToRawData(&scalar, buf, sctype, cn, 1));

    for(y = 0; y < size.height; y += dy)
    {
        tsize.width = size.width;
        tsize.height = dy;
        if(y + dy > size.height)
            tsize.height = size.height - y;
        if(cont_flag || tsize.height == 1)
        {
            tsize.width *= tsize.height;
            tsize.height = 1;
            src_step = tdst_step = dst_step = mask_step = CC_STUB_STEP;
        }

        IPPI_CALL(func(src->tt.data + y*src->step, src_step,
                         tdst->tt.data, tdst_step,
                         cSize(tsize.width*cn, tsize.height), buf));
        if(mask)
        {
            IPPI_CALL(copym_func(tdst->tt.data, tdst_step, dst->tt.data + y*dst->step,
                                   dst_step, tsize, mask->tt.data + y*mask->step, mask_step));
        }
    }

    __END__;

    if(!local_alloc)
        cFree(&buffer);
    return dst;
}


/******************************* A D D ********************************/

CC_IMPL img_t*
cvAdd(const img_t* src1, const img_t* src2, img_t* dst, const img_t* mask CC_DEFAULT(NULL))
{
    static CvFuncTable add_tab;
    static int inittab = 0;
    int local_alloc = 1;
    uchar* buffer = 0;

    CC_FUNCNAME("cvAdd");

    __BEGIN__;

    TypeId type;
    int y, dy, cn, cont_flag = 0;
    int src1_step, src2_step, dst_step, tdst_step, mask_step;
    img_t dstbuf[1], *tdst;
    CvFunc2D_3A func;
    CvFunc2D_3A1I func_sfs;
    CvCopyMaskFunc copym_func;
    CSize size, tsize;

    if(!CC_ARE_TYPES_EQ(src1, src2))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedFormats);

    if(!CC_ARE_SIZES_EQ(src1, src2))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    type = CC_MAT_DEPTH(src1);
    size = cvGetMatSize(src1);
    cn = CC_MAT_CN(src1);
    cvSetMat(dst, size.height, size.width, type, cn, NULL, CC_AUTO_STEP);

    if(!mask)
    {
        if(CC_IS_MAT_CONT(src1) && CC_IS_MAT_CONT(src2) && CC_IS_MAT_CONT(dst))
        {
            int len = size.width*size.height*cn;

            if(len <= CC_MAX_INLINE_MAT_OP_SIZE*CC_MAX_INLINE_MAT_OP_SIZE)
            {
                if(type == CC_32F)
                {
                    const float* src1data = (const float*)(src1->tt.data);
                    const float* src2data = (const float*)(src2->tt.data);
                    float* dstdata = (float*)(dst->tt.data);

                    do
                    {
                        dstdata[len-1] = (float)(src1data[len-1] + src2data[len-1]);
                    }
                    while(--len);

                    EXIT;
                }

                if(type == CC_64F)
                {
                    const double* src1data = (const double*)(src1->tt.data);
                    const double* src2data = (const double*)(src2->tt.data);
                    double* dstdata = (double*)(dst->tt.data);

                    do
                    {
                        dstdata[len-1] = src1data[len-1] + src2data[len-1];
                    }
                    while(--len);

                    EXIT;
                }
            }
            cont_flag = 1;
        }

        dy = size.height;
        copym_func = 0;
        tdst = dst;
    }
    else
    {
        int buf_size, elem_size;
        
        if(!CC_IS_MASK_ARR(mask))
            CC_ERROR(CC_StsBadMask, "");

        if(!CC_ARE_SIZES_EQ(mask, dst))
            CC_ERROR(CC_StsUnmatchedSizes, "");

        cont_flag = CC_IS_MAT_CONT(src1) && CC_IS_MAT_CONT(src2) && CC_IS_MAT_CONT(dst) && CC_IS_MAT_CONT(mask);
        elem_size = CC_TYPE_SIZE(type);

        dy = CC_MAX_LOCAL_SIZE/(elem_size*size.height);
        dy = MAX(dy,1);
        dy = MIN(dy,size.height);
        cvMat(dstbuf, dy, size.width, type, cn, NULL, CC_AUTO_STEP);
        if(!cont_flag)
            dstbuf->step = cvAlign(dstbuf->step, 8);
        buf_size = dstbuf->step ? dstbuf->step*dy : size.width*elem_size;
        if(buf_size > CC_MAX_LOCAL_SIZE)
        {
            CC_CALL(buffer = (uchar*)cAlloc(buf_size));
            local_alloc = 0;
        }
        else
            buffer = (uchar*)cvStackAlloc(buf_size);
        dstbuf->tt.data = buffer;
        tdst = dstbuf;
        
        copym_func = icvGetCopyMaskFunc(elem_size);
    }

    if(!inittab)
    {
        icvInitAddC1RTable(add_tab);
        inittab = 1;
    }

    if(type <= CC_16S)
    {
        func = 0;
        func_sfs = (CvFunc2D_3A1I)(add_tab[type]);
        if(!func_sfs)
            CC_ERROR(CC_StsUnsupportedFormat, "");
    }
    else
    {
        func_sfs = 0;
        func = (CvFunc2D_3A)(add_tab[type]);
        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");
    }

    src1_step = src1->step;
    src2_step = src2->step;
    dst_step = dst->step;
    tdst_step = tdst->step;
    mask_step = mask ? mask->step : 0;

    for(y = 0; y < size.height; y += dy)
    {
        tsize.width = size.width;
        tsize.height = dy;
        if(y + dy > size.height)
            tsize.height = size.height - y;
        if(cont_flag || tsize.height == 1)
        {
            tsize.width *= tsize.height;
            tsize.height = 1;
            src1_step = src2_step = tdst_step = dst_step = mask_step = CC_STUB_STEP;
        }

        IPPI_CALL(type <= CC_16S ?
            func_sfs(src1->tt.data + y*src1->step, src1_step,
                      src2->tt.data + y*src2->step, src2_step,
                      tdst->tt.data, tdst_step,
                      cSize(tsize.width*cn, tsize.height), 0) :
            func(src1->tt.data + y*src1->step, src1_step,
                  src2->tt.data + y*src2->step, src2_step,
                  tdst->tt.data, tdst_step,
                  cSize(tsize.width*cn, tsize.height)));

        if(mask)
        {
            IPPI_CALL(copym_func(tdst->tt.data, tdst_step, dst->tt.data + y*dst->step,
                                   dst_step, tsize, mask->tt.data + y*mask->step, mask_step));
        }
    }

    __END__;

    if(!local_alloc)
        cFree(&buffer);
    return dst;
}


CC_IMPL img_t*
cvAddS(const img_t* src, CScalar scalar, img_t* dst, const img_t* mask CC_DEFAULT(NULL))
{
    static CvFuncTable add_tab;
    static int inittab = 0;
    int local_alloc = 1;
    uchar* buffer = 0;

    CC_FUNCNAME("cvAddS");

    __BEGIN__;

    TypeId sctype, type;
    int y, dy, cn, coi = 0, cont_flag = 0;
    int src_step, dst_step, tdst_step, mask_step;
    img_t dstbuf[1], *tdst;
    CvFunc2D_2A1P func;
    CvCopyMaskFunc copym_func;
    double buf[12];
    CSize size, tsize; 

    if (src!=dst) {
      cvSetMatFromMat(dst, src);
    }

    if(!inittab)
    {
        icvInitAddCC1RTable(add_tab);
        inittab = 1;
    }

    if(!CC_ARE_TYPES_EQ(src, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedFormats);

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    sctype = type = CC_MAT_DEPTH(src);
    
    cn = CC_MAT_CN(src);
    if(type < CC_32S)
        sctype = CC_32S;

    size = cvGetMatSize(src);

    if(!mask)
    {
        if(CC_IS_MAT_CONT(src) && CC_IS_MAT_CONT(dst))
        {
            if(size.width <= CC_MAX_INLINE_MAT_OP_SIZE)
            {
                int len = size.width * size.height;

                if(type == CC_32F && cn==1)
                {
                    const float* srcdata = (const float*)(src->tt.data);
                    float* dstdata = (float*)(dst->tt.data);
                
                    do
                    {
                        dstdata[len-1] = (float)(scalar.val[0] + srcdata[len-1]);
                    }
                    while(--len);

                    EXIT;
                }

                if(type == CC_64F && 1==cn)
                {
                    const double* srcdata = (const double*)(src->tt.data);
                    double* dstdata = (double*)(dst->tt.data);
                
                    do
                    {
                        dstdata[len-1] = scalar.val[0] + srcdata[len-1];
                    }
                    while(--len);

                    EXIT;
                }
            }
            cont_flag = 1;
        }
        
        dy = size.height;
        copym_func = 0;
        tdst = dst;
    }
    else
    {
        int buf_size, elem_size;
        
        if(!CC_IS_MASK_ARR(mask))
            CC_ERROR(CC_StsBadMask, "");

        if(!CC_ARE_SIZES_EQ(mask, dst))
            CC_ERROR(CC_StsUnmatchedSizes, "");

        cont_flag = CC_IS_MAT_CONT(src) && CC_IS_MAT_CONT(dst) && CC_IS_MAT_CONT(mask);
        elem_size = CC_TYPE_SIZE(type);

        dy = CC_MAX_LOCAL_SIZE/(elem_size*size.height);
        dy = MAX(dy,1);
        dy = MIN(dy,size.height);
        cvMat(dstbuf, dy, size.width, type, cn, NULL, CC_AUTO_STEP);
        if(!cont_flag)
            dstbuf->step = cvAlign(dstbuf->step, 8);
        buf_size = dstbuf->step ? dstbuf->step*dy : size.width*elem_size;
        if(buf_size > CC_MAX_LOCAL_SIZE)
        {
            CC_CALL(buffer = (uchar*)cAlloc(buf_size));
            local_alloc = 0;
        }
        else
            buffer = (uchar*)cvStackAlloc(buf_size);
        dstbuf->tt.data = buffer;
        tdst = dstbuf;
        
        copym_func = icvGetCopyMaskFunc(elem_size);
    }

    func = (CvFunc2D_2A1P)(add_tab[type]);
    if(!func)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    src_step = src->step;
    dst_step = dst->step;
    tdst_step = tdst->step;
    mask_step = mask ? mask->step : 0;

    CC_CALL(cvScalarToRawData(&scalar, buf, sctype, cn, 1));

    for(y = 0; y < size.height; y += dy)
    {
        tsize.width = size.width;
        tsize.height = dy;
        if(y + dy > size.height)
            tsize.height = size.height - y;
        if(cont_flag || tsize.height == 1)
        {
            tsize.width *= tsize.height;
            tsize.height = 1;
            src_step = tdst_step = dst_step = mask_step = CC_STUB_STEP;
        }

        IPPI_CALL(func(src->tt.data + y*src->step, src_step,
                         tdst->tt.data, tdst_step,
                         cSize(tsize.width*cn, tsize.height), buf));
        if(mask)
        {
            IPPI_CALL(copym_func(tdst->tt.data, tdst_step, dst->tt.data + y*dst->step,
                                   dst_step, tsize, mask->tt.data + y*mask->step, mask_step));
        }
    }

    __END__;

    if(!local_alloc)
        cFree(&buffer);
    return dst;
}

CC_IMPL img_t*
cvAddS_C1(const img_t* src, double scalar, img_t* dst, const img_t* mask CC_DEFAULT(NULL)) {
  if (src!=dst) {
    cvSetMatFromMat(dst, src);
  }
  return cvAddS(src, cRealScalar(scalar), dst, mask);
}

/* dst(mask) = src(mask) - value = src(mask) + (-value) */
CC_INLINE img_t*
cvSubS(const img_t* src, CScalar value, img_t* dst, const img_t* mask CC_DEFAULT(NULL))
{
    return cvAddS(src, cScalar(-value.val[0], -value.val[1], -value.val[2], -value.val[3]), dst, mask);
}
CC_INLINE img_t*
cvSubS_C1(const img_t* src, double value, img_t* dst, const img_t* mask CC_DEFAULT(NULL))
{
    return cvAddS(src, cScalar(-value, 0, 0, 0), dst, mask);
}


/***************************************** M U L ****************************************/

#define ICC_DEF_MUL_OP_CASE(flavor, arrtype, worktype, _cast_macro1_,                  \
                             _cast_macro2_, _cvt_macro_)                               \
static CStatus C_STDCALL                                                              \
    icvMul_##flavor##_C1R(const arrtype* src1, int step1,                              \
                           const arrtype* src2, int step2,                              \
                           arrtype* dst, int step,                                      \
                           CSize size, double scale)                                  \
{                                                                                       \
    step1 /= sizeof(src1[0]); step2 /= sizeof(src2[0]); step /= sizeof(dst[0]);         \
                                                                                        \
    if(fabs(scale - 1.) < DBL_EPSILON)                                                \
    {                                                                                   \
        for(; size.height--; src1+=step1, src2+=step2, dst+=step)                     \
        {                                                                               \
            int i;                                                                      \
            for(i = 0; i <= size.width - 4; i += 4)                                   \
            {                                                                           \
                worktype t0 = src1[i] * src2[i];                                        \
                worktype t1 = src1[i+1] * src2[i+1];                                    \
                                                                                        \
                dst[i] = _cast_macro2_(t0);                                             \
                dst[i+1] = _cast_macro2_(t1);                                           \
                                                                                        \
                t0 = src1[i+2] * src2[i+2];                                             \
                t1 = src1[i+3] * src2[i+3];                                             \
                                                                                        \
                dst[i+2] = _cast_macro2_(t0);                                           \
                dst[i+3] = _cast_macro2_(t1);                                           \
            }                                                                           \
                                                                                        \
            for(; i < size.width; i++)                                                \
            {                                                                           \
                worktype t0 = src1[i] * src2[i];                                        \
                dst[i] = _cast_macro2_(t0);                                             \
            }                                                                           \
        }                                                                               \
    }                                                                                   \
    else                                                                                \
    {                                                                                   \
        for(; size.height--; src1+=step1, src2+=step2, dst+=step)                     \
        {                                                                               \
            int i;                                                                      \
            for(i = 0; i <= size.width - 4; i += 4)                                   \
            {                                                                           \
                double ft0 = scale*_cvt_macro_(src1[i])*_cvt_macro_(src2[i]);           \
                double ft1 = scale*_cvt_macro_(src1[i+1])*_cvt_macro_(src2[i+1]);       \
                worktype t0 = _cast_macro1_(ft0);                                       \
                worktype t1 = _cast_macro1_(ft1);                                       \
                                                                                        \
                dst[i] = _cast_macro2_(t0);                                             \
                dst[i+1] = _cast_macro2_(t1);                                           \
                                                                                        \
                ft0 = scale*_cvt_macro_(src1[i+2])*_cvt_macro_(src2[i+2]);              \
                ft1 = scale*_cvt_macro_(src1[i+3])*_cvt_macro_(src2[i+3]);              \
                t0 = _cast_macro1_(ft0);                                                \
                t1 = _cast_macro1_(ft1);                                                \
                                                                                        \
                dst[i+2] = _cast_macro2_(t0);                                           \
                dst[i+3] = _cast_macro2_(t1);                                           \
            }                                                                           \
                                                                                        \
            for(; i < size.width; i++)                                                \
            {                                                                           \
                worktype t0;                                                            \
                t0 = _cast_macro1_(scale*_cvt_macro_(src1[i])*_cvt_macro_(src2[i]));    \
                dst[i] = _cast_macro2_(t0);                                             \
            }                                                                           \
        }                                                                               \
    }                                                                                   \
                                                                                        \
    return CC_OK;                                                                       \
}


ICC_DEF_MUL_OP_CASE(8u, uchar, int, cRound, CC_CAST_8U, CC_8TO32F)
ICC_DEF_MUL_OP_CASE(16u, ushort, int, cRound, CC_CAST_16U, CC_NOP)
ICC_DEF_MUL_OP_CASE(16s, short, int, cRound, CC_CAST_16S, CC_NOP)
ICC_DEF_MUL_OP_CASE(32s, int, int, cRound, CC_CAST_32S, CC_NOP)
ICC_DEF_MUL_OP_CASE(32f, float, double, CC_NOP, CC_CAST_32F, CC_NOP)
ICC_DEF_MUL_OP_CASE(64f, double, double, CC_NOP, CC_CAST_64F, CC_NOP)


ICC_DEF_INIT_ARITHM_FUNC_TAB(Mul, C1R)


typedef CStatus (C_STDCALL * CvScaledElWiseFunc)(const void* src1, int step1,
                                                    const void* src2, int step2,
                                                    void* dst, int step,
                                                    CSize size, double scale);

CC_IMPL img_t*
cvMul(const img_t* src1, const img_t* src2, img_t* dst, double scale CC_DEFAULT(1))
{
    static CvFuncTable mul_tab;
    static int inittab = 0;

    CC_FUNCNAME("cvMul");

    __BEGIN__;

    TypeId type;
    int coi = 0;
    int src1_step, src2_step, dst_step;
    
    
    
    CSize size;
    CvScaledElWiseFunc func;

    if(!inittab)
    {
        icvInitMulC1RTable(mul_tab);
        inittab = 1;
    }

    if(!CC_ARE_TYPES_EQ(src1, src2) || !CC_ARE_TYPES_EQ(src1, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedFormats);

    if(!CC_ARE_SIZES_EQ(src1, src2) || !CC_ARE_SIZES_EQ(src1, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    type = CC_MAT_DEPTH(src1);
    size = cvGetMatSize(src1);

    size.width *= CC_MAT_CN(src1);

    if(CC_IS_MAT_CONT(src1) && CC_IS_MAT_CONT(src2) && CC_IS_MAT_CONT(dst))
    {
        size.width *= size.height;

        if(size.width <= CC_MAX_INLINE_MAT_OP_SIZE && scale == 1)
        {
            if(type == CC_32F)
            {
                const float* src1data = (const float*)(src1->tt.data);
                const float* src2data = (const float*)(src2->tt.data);
                float* dstdata = (float*)(dst->tt.data);
            
                do
                {
                    dstdata[size.width-1] = (float)
                        (src1data[size.width-1] * src2data[size.width-1]);
                }
                while(--size.width);

                EXIT;
            }

            if(type == CC_64F)
            {
                const double* src1data = (const double*)(src1->tt.data);
                const double* src2data = (const double*)(src2->tt.data);
                double* dstdata = (double*)(dst->tt.data);
            
                do
                {
                    dstdata[size.width-1] =
                        src1data[size.width-1] * src2data[size.width-1];
                }
                while(--size.width);

                EXIT;
            }
        }

        src1_step = src2_step = dst_step = CC_STUB_STEP;
        size.height = 1;
    }
    else
    {
        src1_step = src1->step;
        src2_step = src2->step;
        dst_step = dst->step;
    }

    func = (CvScaledElWiseFunc)(mul_tab[type]);

    if(!func)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    IPPI_CALL(func(src1->tt.data, src1_step, src2->tt.data, src2_step,
                     dst->tt.data, dst_step, size, scale));

    __END__;
    return dst;
}


/***************************************** D I V ****************************************/

#define ICC_DEF_DIV_OP_CASE(flavor, arrtype, worktype, checktype, _start_row_macro_,   \
    _cast_macro1_, _cast_macro2_, _cvt_macro_, _check_macro_, isrc)                    \
                                                                                        \
static CStatus C_STDCALL                                                              \
icvDiv_##flavor##_C1R(const arrtype* src1, int step1,                                  \
                       const arrtype* src2, int step2,                                  \
                       arrtype* dst, int step,                                          \
                       CSize size, double scale)                                      \
{                                                                                       \
    step1 /= sizeof(src1[0]); step2 /= sizeof(src2[0]); step /= sizeof(dst[0]);         \
                                                                                        \
    for(; size.height--; src1+=step1, src2+=step2, dst+=step)                         \
    {                                                                                   \
        _start_row_macro_(checktype, src2);                                             \
        for(i = 0; i <= size.width - 4; i += 4)                                       \
        {                                                                               \
            worktype z0, z1, z2, z3;                                                    \
            if(_check_macro_(isrc[i]) && _check_macro_(isrc[i+1]) &&                   \
                _check_macro_(isrc[i+2]) && _check_macro_(isrc[i+3]))                   \
            {                                                                           \
                double a = (double)_cvt_macro_(src2[i]) * _cvt_macro_(src2[i+1]);       \
                double b = (double)_cvt_macro_(src2[i+2]) * _cvt_macro_(src2[i+3]);     \
                double d = scale/(a * b);                                               \
                                                                                        \
                b *= d;                                                                 \
                a *= d;                                                                 \
                                                                                        \
                z0 = _cast_macro1_(src2[i+1] * _cvt_macro_(src1[i]) * b);               \
                z1 = _cast_macro1_(src2[i] * _cvt_macro_(src1[i+1]) * b);               \
                z2 = _cast_macro1_(src2[i+3] * _cvt_macro_(src1[i+2]) * a);             \
                z3 = _cast_macro1_(src2[i+2] * _cvt_macro_(src1[i+3]) * a);             \
                                                                                        \
                dst[i] = _cast_macro2_(z0);                                             \
                dst[i+1] = _cast_macro2_(z1);                                           \
                dst[i+2] = _cast_macro2_(z2);                                           \
                dst[i+3] = _cast_macro2_(z3);                                           \
            }                                                                           \
            else                                                                        \
            {                                                                           \
                z0 = _check_macro_(isrc[i]) ?                                           \
                   _cast_macro1_(_cvt_macro_(src1[i])*scale/_cvt_macro_(src2[i])) : 0;  \
                z1 = _check_macro_(isrc[i+1]) ?                                         \
                   _cast_macro1_(_cvt_macro_(src1[i+1])*scale/_cvt_macro_(src2[i+1])):0;\
                z2 = _check_macro_(isrc[i+2]) ?                                         \
                   _cast_macro1_(_cvt_macro_(src1[i+2])*scale/_cvt_macro_(src2[i+2])):0;\
                z3 = _check_macro_(isrc[i+3]) ?                                         \
                   _cast_macro1_(_cvt_macro_(src1[i+3])*scale/_cvt_macro_(src2[i+3])):0;\
                                                                                        \
                dst[i] = _cast_macro2_(z0);                                             \
                dst[i+1] = _cast_macro2_(z1);                                           \
                dst[i+2] = _cast_macro2_(z2);                                           \
                dst[i+3] = _cast_macro2_(z3);                                           \
            }                                                                           \
        }                                                                               \
                                                                                        \
        for(; i < size.width; i++)                                                    \
        {                                                                               \
            worktype z0 = _check_macro_(isrc[i]) ?                                      \
                _cast_macro1_(_cvt_macro_(src1[i])*scale/_cvt_macro_(src2[i])) : 0;     \
            dst[i] = _cast_macro2_(z0);                                                 \
        }                                                                               \
    }                                                                                   \
                                                                                        \
    return CC_OK;                                                                       \
}


#define ICC_DEF_RECIP_OP_CASE(flavor, arrtype, worktype, checktype,            \
    _start_row_macro_, _cast_macro1_, _cast_macro2_,                            \
    _cvt_macro_, _check_macro_, isrc)                                          \
                                                                                \
static CStatus C_STDCALL                                                      \
icvRecip_##flavor##_C1R(const arrtype* src, int step1,                         \
                         arrtype* dst, int step,                                \
                         CSize size, double scale)                            \
{                                                                               \
    step1 /= sizeof(src[0]); step /= sizeof(dst[0]);                            \
                                                                                \
    for(; size.height--; src+=step1, dst+=step)                               \
    {                                                                           \
        _start_row_macro_(checktype, src);                                      \
        for(i = 0; i <= size.width - 4; i += 4)                                 \
        {                                                                       \
            worktype z0, z1, z2, z3;                                            \
            if(_check_macro_(isrc[i]) && _check_macro_(isrc[i+1]) &&            \
                _check_macro_(isrc[i+2]) && _check_macro_(isrc[i+3]))           \
            {                                                                   \
                double a = (double)_cvt_macro_(src[i]) * _cvt_macro_(src[i+1]); \
                double b = (double)_cvt_macro_(src[i+2]) * _cvt_macro_(src[i+3]);\
                double d = scale/(a * b);                                       \
                                                                                \
                b *= d;                                                         \
                a *= d;                                                         \
                                                                                \
                z0 = _cast_macro1_(src[i+1] * b);                               \
                z1 = _cast_macro1_(src[i] * b);                                 \
                z2 = _cast_macro1_(src[i+3] * a);                               \
                z3 = _cast_macro1_(src[i+2] * a);                               \
                                                                                \
                dst[i] = _cast_macro2_(z0);                                     \
                dst[i+1] = _cast_macro2_(z1);                                   \
                dst[i+2] = _cast_macro2_(z2);                                   \
                dst[i+3] = _cast_macro2_(z3);                                   \
            }                                                                   \
            else                                                                \
            {                                                                   \
                z0 = _check_macro_(isrc[i]) ?                                   \
                   _cast_macro1_(scale/_cvt_macro_(src[i])) : 0;                \
                z1 = _check_macro_(isrc[i+1]) ?                                 \
                   _cast_macro1_(scale/_cvt_macro_(src[i+1])):0;                \
                z2 = _check_macro_(isrc[i+2]) ?                                 \
                   _cast_macro1_(scale/_cvt_macro_(src[i+2])):0;                \
                z3 = _check_macro_(isrc[i+3]) ?                                 \
                   _cast_macro1_(scale/_cvt_macro_(src[i+3])):0;                \
                                                                                \
                dst[i] = _cast_macro2_(z0);                                     \
                dst[i+1] = _cast_macro2_(z1);                                   \
                dst[i+2] = _cast_macro2_(z2);                                   \
                dst[i+3] = _cast_macro2_(z3);                                   \
            }                                                                   \
        }                                                                       \
                                                                                \
        for(; i < size.width; i++)                                              \
        {                                                                       \
            worktype z0 = _check_macro_(isrc[i]) ?                              \
                _cast_macro1_(scale/_cvt_macro_(src[i])) : 0;                   \
            dst[i] = _cast_macro2_(z0);                                         \
        }                                                                       \
    }                                                                           \
                                                                                \
    return CC_OK;                                                               \
}


#define div_start_row_int(checktype, divisor) \
    int i

#define div_start_row_flt(checktype, divisor) \
    const checktype* isrc = (const checktype*)divisor; int i

#define div_check_zero_flt(x)  (((x) & 0x7fffffff) != 0)
#define div_check_zero_dbl(x)  (((x) & CC_BIG_INT(0x7fffffffffffffff)) != 0)

#if defined WIN64 && defined EM64T && defined _MSC_VER && !defined CC_ICC
#pragma optimize("",off)
#endif

ICC_DEF_DIV_OP_CASE(8u, uchar, int, uchar, div_start_row_int,
                     cRound, CC_CAST_8U, CC_8TO32F, CC_NONZERO, src2)

#if defined WIN64 && defined EM64T && defined _MSC_VER && !defined CC_ICC
#pragma optimize("",on)
#endif


ICC_DEF_DIV_OP_CASE(16u, ushort, int, ushort, div_start_row_int,
                     cRound, CC_CAST_16U, CC_CAST_64F, CC_NONZERO, src2)
ICC_DEF_DIV_OP_CASE(16s, short, int, short, div_start_row_int,
                     cRound, CC_CAST_16S, CC_NOP, CC_NONZERO, src2)
ICC_DEF_DIV_OP_CASE(32s, int, int, int, div_start_row_int,
                     cRound, CC_CAST_32S, CC_CAST_64F, CC_NONZERO, src2)
ICC_DEF_DIV_OP_CASE(32f, float, double, int, div_start_row_flt,
                     CC_NOP, CC_CAST_32F, CC_NOP, div_check_zero_flt, isrc)
ICC_DEF_DIV_OP_CASE(64f, double, double, int64, div_start_row_flt,
                     CC_NOP, CC_CAST_64F, CC_NOP, div_check_zero_dbl, isrc)

ICC_DEF_RECIP_OP_CASE(8u, uchar, int, uchar, div_start_row_int,
                       cRound, CC_CAST_8U, CC_8TO32F, CC_NONZERO, src)
ICC_DEF_RECIP_OP_CASE(16u, ushort, int, ushort, div_start_row_int,
                       cRound, CC_CAST_16U, CC_CAST_64F, CC_NONZERO, src)
ICC_DEF_RECIP_OP_CASE(16s, short, int, short, div_start_row_int,
                       cRound, CC_CAST_16S, CC_NOP, CC_NONZERO, src)
ICC_DEF_RECIP_OP_CASE(32s, int, int, int, div_start_row_int,
                       cRound, CC_CAST_32S, CC_CAST_64F, CC_NONZERO, src)
ICC_DEF_RECIP_OP_CASE(32f, float, double, int, div_start_row_flt,
                       CC_NOP, CC_CAST_32F, CC_NOP, div_check_zero_flt, isrc)
ICC_DEF_RECIP_OP_CASE(64f, double, double, int64, div_start_row_flt,
                       CC_NOP, CC_CAST_64F, CC_NOP, div_check_zero_dbl, isrc)

ICC_DEF_INIT_ARITHM_FUNC_TAB(Div, C1R)
ICC_DEF_INIT_ARITHM_FUNC_TAB(Recip, C1R)

typedef CStatus (C_STDCALL * CvRecipFunc)(const void* src, int step1,
                                             void* dst, int step,
                                             CSize size, double scale);

CC_IMPL img_t*
cvDiv(const img_t* src1, const img_t* src2, img_t* dst, double scale CC_DEFAULT(1))
{
    static CvFuncTable div_tab;
    static CvFuncTable recip_tab;
    static int inittab = 0;

    CC_FUNCNAME("cvDiv");

    __BEGIN__;

    TypeId type;
    int cn;
    int src1_step, src2_step, dst_step;
    int src1_cont_flag = CC_MAT_CONT_FLAG;
    CSize size;

    if(!inittab)
    {
        icvInitDivC1RTable(div_tab);
        icvInitRecipC1RTable(recip_tab);
        inittab = 1;
    }

    if(src1)
    {
        if(!CC_ARE_TYPES_EQ(src1, src2))
            CC_ERROR_FROM_CODE(CC_StsUnmatchedFormats);

        if(!CC_ARE_SIZES_EQ(src1, src2))
            CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);
        src1_cont_flag = CC_IS_MAT_CONT(src1);
    }
    type = CC_MAT_DEPTH(src2);
    cn = CC_MAT_CN(src2);
    size = cvGetMatSize(src2);
    cvSetMat(dst, size.height, size.width, type, cn, NULL, CC_AUTO_STEP);

    if(!CC_ARE_TYPES_EQ(src2, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedFormats);

    if(!CC_ARE_SIZES_EQ(src2, dst))
        CC_ERROR_FROM_CODE(CC_StsUnmatchedSizes);

    size.width *= CC_MAT_CN(src2);

    if(src1_cont_flag && CC_IS_MAT_CONT(src2) && CC_IS_MAT_CONT(dst))
    {
        size.width *= size.height;
        src1_step = src2_step = dst_step = CC_STUB_STEP;
        size.height = 1;
    }
    else
    {
        src1_step = src1 ? src1->step : 0;
        src2_step = src2->step;
        dst_step = dst->step;
    }

    if(src1)
    {
        CvScaledElWiseFunc func = (CvScaledElWiseFunc)(div_tab[type]);

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        IPPI_CALL(func(src1->tt.data, src1_step, src2->tt.data, src2_step,
                         dst->tt.data, dst_step, size, scale));
    }
    else
    {
        CvRecipFunc func = (CvRecipFunc)(recip_tab[type]);

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        IPPI_CALL(func(src2->tt.data, src2_step,
                         dst->tt.data, dst_step, size, scale));
    }

    __END__;
    return dst;
}

/******************************* A D D   W E I G T E D ******************************/

#define ICC_DEF_ADD_WEIGHTED_OP(flavor, arrtype, worktype, load_macro,          \
                                     cast_macro1, cast_macro2)                  \
static CStatus C_STDCALL                                                      \
icvAddWeighted_##flavor##_C1R(const arrtype* src1, int step1, double alpha,    \
                               const arrtype* src2, int step2, double beta,     \
                               double gamma, arrtype* dst, int step, CSize size)\
{                                                                               \
    step1 /= sizeof(src1[0]); step2 /= sizeof(src2[0]); step /= sizeof(dst[0]); \
                                                                                \
    for(; size.height--; src1 += step1, src2 += step2, dst += step)           \
    {                                                                           \
        int i;                                                                  \
                                                                                \
        for(i = 0; i <= size.width - 4; i += 4)                               \
        {                                                                       \
            worktype t0 = cast_macro1(load_macro((src1)[i])*alpha +             \
                                      load_macro((src2)[i])*beta + gamma);      \
            worktype t1 = cast_macro1(load_macro((src1)[i+1])*alpha +           \
                                      load_macro((src2)[i+1])*beta + gamma);    \
                                                                                \
            (dst)[i] = cast_macro2(t0);                                       \
            (dst)[i+1] = cast_macro2(t1);                                     \
                                                                                \
            t0 = cast_macro1(load_macro((src1)[i+2])*alpha +                    \
                             load_macro((src2)[i+2])*beta + gamma);             \
            t1 = cast_macro1(load_macro((src1)[i+3])*alpha +                    \
                             load_macro((src2)[i+3])*beta + gamma);             \
                                                                                \
            (dst)[i+2] = cast_macro2(t0);                                     \
            (dst)[i+3] = cast_macro2(t1);                                     \
        }                                                                       \
                                                                                \
        for(; i < size.width; i++)                                            \
        {                                                                       \
            worktype t0 = cast_macro1(load_macro((src1)[i])*alpha +             \
                                      load_macro((src2)[i])*beta + gamma);      \
            (dst)[i] = cast_macro2(t0);                                       \
        }                                                                       \
    }                                                                           \
                                                                                \
    return CC_OK;                                                               \
}


#undef shift
#define shift 14

static CStatus C_STDCALL
icvAddWeighted_8u_fast_C1R(const uchar* src1, int step1, double alpha,
                            const uchar* src2, int step2, double beta,
                            double gamma, uchar* dst, int step, CSize size)
{
    int tab1[256], tab2[256];
    double t = 0;
    int j, t0, t1, t2, t3;

    alpha *= 1 << shift;
    gamma = gamma*(1 << shift) + (1 << (shift - 1));
    beta *= 1 << shift;

    for(j = 0; j < 256; j++)
    {
        tab1[j] = cRound(t);
        tab2[j] = cRound(gamma);
        t += alpha;
        gamma += beta;
    }

    t0 = (tab1[0] + tab2[0]) >> shift;
    t1 = (tab1[0] + tab2[255]) >> shift;
    t2 = (tab1[255] + tab2[0]) >> shift;
    t3 = (tab1[255] + tab2[255]) >> shift;

    if((unsigned)(t0+256) < 768 && (unsigned)(t1+256) < 768 &&
        (unsigned)(t2+256) < 768 && (unsigned)(t3+256) < 768)
    {
        // use faster table-based convertion back to 8u
        for(; size.height--; src1 += step1, src2 += step2, dst += step)
        {
            int i;

            for(i = 0; i <= size.width - 4; i += 4)
            {
                t0 = CC_FAST_CAST_8U((tab1[src1[i]] + tab2[src2[i]]) >> shift);
                t1 = CC_FAST_CAST_8U((tab1[src1[i+1]] + tab2[src2[i+1]]) >> shift);

                dst[i] = (uchar)t0;
                dst[i+1] = (uchar)t1;

                t0 = CC_FAST_CAST_8U((tab1[src1[i+2]] + tab2[src2[i+2]]) >> shift);
                t1 = CC_FAST_CAST_8U((tab1[src1[i+3]] + tab2[src2[i+3]]) >> shift);

                dst[i+2] = (uchar)t0;
                dst[i+3] = (uchar)t1;
            }

            for(; i < size.width; i++)
            {
                t0 = CC_FAST_CAST_8U((tab1[src1[i]] + tab2[src2[i]]) >> shift);
                dst[i] = (uchar)t0;
            }
        }
    }
    else
    {
        // use universal macro for convertion back to 8u
        for(; size.height--; src1 += step1, src2 += step2, dst += step)
        {
            int i;
            
            for(i = 0; i <= size.width - 4; i += 4)
            {
                t0 = (tab1[src1[i]] + tab2[src2[i]]) >> shift;
                t1 = (tab1[src1[i+1]] + tab2[src2[i+1]]) >> shift;

                dst[i] = CC_CAST_8U(t0);
                dst[i+1] = CC_CAST_8U(t1);

                t0 = (tab1[src1[i+2]] + tab2[src2[i+2]]) >> shift;
                t1 = (tab1[src1[i+3]] + tab2[src2[i+3]]) >> shift;

                dst[i+2] = CC_CAST_8U(t0);
                dst[i+3] = CC_CAST_8U(t1);
            }

            for(; i < size.width; i++)
            {
                t0 = (tab1[src1[i]] + tab2[src2[i]]) >> shift;
                dst[i] = CC_CAST_8U(t0);
            }
        }
    }

    return CC_OK;
}


ICC_DEF_ADD_WEIGHTED_OP(8u, uchar, int, CC_8TO32F, cRound, CC_CAST_8U)
ICC_DEF_ADD_WEIGHTED_OP(16u, ushort, int, CC_NOP, cRound, CC_CAST_16U)
ICC_DEF_ADD_WEIGHTED_OP(16s, short, int, CC_NOP, cRound, CC_CAST_16S)
ICC_DEF_ADD_WEIGHTED_OP(32s, int, int, CC_NOP, cRound, CC_CAST_32S)
ICC_DEF_ADD_WEIGHTED_OP(32f, float, double, CC_NOP, CC_NOP, CC_CAST_32F)
ICC_DEF_ADD_WEIGHTED_OP(64f, double, double, CC_NOP, CC_NOP, CC_CAST_64F)


ICC_DEF_INIT_ARITHM_FUNC_TAB(AddWeighted, C1R)

typedef CStatus (C_STDCALL *CvAddWeightedFunc)(const void* src1, int step1, double alpha,
                                                  const void* src2, int step2, double beta,
                                                  double gamma, void* dst,
                                                  int step, CSize size);

CC_IMPL img_t*
cvAddWeighted(const img_t* srcA, double alpha, const img_t* srcB, double beta,
               double gamma, img_t* dst)
{
    static CvFuncTable addw_tab;
    static int inittab = 0;
    
    CC_FUNCNAME("cvAddWeighted");

    __BEGIN__;

    int  srcA_step, srcB_step, dst_step;
    TypeId  type;
    int cn = CC_MAT_CN(srcA);
    CvAddWeightedFunc func;
    CSize size;

    if(!inittab)
    {
        icvInitAddWeightedC1RTable(addw_tab);
        inittab = 1;
    }

    if(!CC_ARE_TYPES_EQ(srcA, srcB) ||
        !CC_ARE_TYPES_EQ(srcA, dst))
        CC_ERROR(CC_StsUnmatchedFormats,
        "All input/output arrays should have the same type");

    if(!CC_ARE_SIZES_EQ(srcA, srcB) ||
        !CC_ARE_SIZES_EQ(srcA, dst))
        CC_ERROR(CC_StsUnmatchedSizes,
        "All input/output arrays should have the same sizes");

    size = cvGetMatSize(srcA);
    type = CC_MAT_DEPTH(srcA);
    size.width *= CC_MAT_CN(srcA);
    srcA_step = srcA->step;
    srcB_step = srcB->step;
    dst_step = dst->step;

    if(CC_IS_MAT_CONT(srcA) && CC_IS_MAT_CONT(srcB) && CC_IS_MAT_CONT(dst))
    {
        size.width *= size.height;
        size.height = 1;
        srcA_step = srcB_step = dst_step = CC_AUTOSTEP;
    }

    if(type == CC_8U && 1==cn && size.width * size.height >= 1024 &&
        fabs(alpha) < 256 && fabs(beta) < 256 && fabs(gamma) < 256*256)
    {
        func = (CvAddWeightedFunc)icvAddWeighted_8u_fast_C1R;
    }
    else
    {
        func = (CvAddWeightedFunc)addw_tab[type];
        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "This array type is not supported");
    }

    IPPI_CALL(func(srcA->tt.data, srcA_step, alpha, srcB->tt.data, srcB_step,
                     beta, gamma, dst->tt.data, dst_step, size));

    __END__;
    return dst;
}

#undef shift


