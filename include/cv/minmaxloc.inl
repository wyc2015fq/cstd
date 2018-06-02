
/****************************************************************************************\
*                                     MinMaxLoc                                          *
\****************************************************************************************/
                                                                    
#define CC_MINMAXLOC_ENTRY(_toggle_, srctype, temptype, cn) \
    temptype min_val, max_val;                  \
    int min_loc = 0, max_loc = 0;               \
    int x, loc = 0, width = size.width*(cn);    \
    step /= sizeof(src[0]);                     \
                                                \
    min_val = src[0];                           \
    min_val = max_val = _toggle_(min_val)


#define CC_MINMAXLOC_EXIT(_fin_cast_macro_)   \
    minLoc->x = min_loc;                        \
    maxLoc->x = max_loc;                        \
    minLoc->y = maxLoc->y = 0;                  \
    *minVal = _fin_cast_macro_(min_val);        \
    *maxVal = _fin_cast_macro_(max_val);        \
    return CC_OK


#define ICC_DEF_MINMAXLOC_1D_CASE_COI(_toggle_, temptype, cn) \
    for(x = 0; x < width; x += (cn), loc++)   \
    {                                           \
        temptype val = src[x];                  \
        val = _toggle_(val);                    \
                                                \
        if(val < min_val)                     \
        {                                       \
            min_val = val;                      \
            min_loc = loc;                      \
        }                                       \
        else if(val > max_val)                \
        {                                       \
            max_val = val;                      \
            max_loc = loc;                      \
        }                                       \
    }


#define ICC_DEF_MINMAXLOC_FUNC_2D(_toggle_, _fin_cast_macro_, flavor,      \
                                   srctype, temptype, extrtype)            \
IPCVAPI_IMPL(CStatus,                                                     \
icvMinMaxIndx_##flavor##_C1R,(const srctype* src, int step, CSize size,   \
    extrtype* minVal, extrtype* maxVal, CPoint* minLoc, CPoint* maxLoc), \
    (src, step, size, minVal, maxVal, minLoc, maxLoc))                     \
{                                                                           \
    CC_MINMAXLOC_ENTRY(_toggle_, srctype, temptype, 1);                   \
                                                                            \
    for(; size.height--; src += step)                                     \
    {                                                                       \
        ICC_DEF_MINMAXLOC_1D_CASE_COI(_toggle_, temptype, 1);             \
    }                                                                       \
                                                                            \
    CC_MINMAXLOC_EXIT(_fin_cast_macro_);                                  \
}


#define ICC_DEF_MINMAXLOC_FUNC_2D_COI(_toggle_, _fin_cast_macro_, flavor,  \
                                       srctype, temptype, extrtype)        \
static CStatus C_STDCALL                                                  \
icvMinMaxIndx_##flavor##_CnCR(const srctype* src, int step,                \
                          CSize size, int cn, int coi,                     \
                          extrtype* minVal, extrtype* maxVal,               \
                          CPoint* minLoc, CPoint* maxLoc)                \
{                                                                           \
    (src) += coi - 1;                                                       \
    {CC_MINMAXLOC_ENTRY(_toggle_, srctype, temptype, cn);                  \
                                                                            \
    for(; size.height--; src += step)                                     \
    {                                                                       \
        ICC_DEF_MINMAXLOC_1D_CASE_COI(_toggle_, temptype, cn);            \
    }                                                                       \
                                                                            \
    CC_MINMAXLOC_EXIT(_fin_cast_macro_);}                                  \
}


#define ICC_DEF_MINMAXLOC_ALL_INT(flavor, srctype,             \
                                   _fin_cast_macro_, extrtype) \
    ICC_DEF_MINMAXLOC_FUNC_2D(CC_NOP, _fin_cast_macro_, flavor,\
                               srctype, int, extrtype)         \
    ICC_DEF_MINMAXLOC_FUNC_2D_COI(CC_NOP, _fin_cast_macro_,    \
                            flavor, srctype, int, extrtype)

CC_INLINE float minmax_to_float(int val)
{
    suf32_t v;
    v.i = CC_TOGGLE_FLT(val);
    return v.f;
}

CC_INLINE double minmax_to_double(int64 val)
{
    suf64_t v;
    v.i = CC_TOGGLE_DBL(val);
    return v.f;
}

#define ICC_DEF_MINMAXLOC_ALL_FLT(flavor, srctype, _toggle_,           \
                                   _fin_cast_macro_, extrtype)         \
                                                                        \
    ICC_DEF_MINMAXLOC_FUNC_2D(_toggle_, _fin_cast_macro_, flavor,      \
                                srctype, srctype, extrtype)            \
    ICC_DEF_MINMAXLOC_FUNC_2D_COI(_toggle_, _fin_cast_macro_, flavor,  \
                                srctype, srctype, extrtype)

ICC_DEF_MINMAXLOC_ALL_INT(8u, uchar, CC_CAST_32F, float)
ICC_DEF_MINMAXLOC_ALL_INT(16u, ushort, CC_CAST_32F, float)
ICC_DEF_MINMAXLOC_ALL_INT(16s, short, CC_CAST_32F, float)
ICC_DEF_MINMAXLOC_ALL_INT(32s, int, CC_CAST_64F, double)
ICC_DEF_MINMAXLOC_ALL_FLT(32f, int, CC_TOGGLE_FLT, minmax_to_float, float)
ICC_DEF_MINMAXLOC_ALL_FLT(64f, int64, CC_TOGGLE_DBL, minmax_to_double, double)


/****************************************************************************************\
*                              MinMaxLoc with mask                                       *
\****************************************************************************************/

#define CC_MINMAXLOC_MASK_ENTRY(_toggle_, srctype, temptype, cn)  \
    temptype min_val = 0, max_val = 0;                              \
    int min_loc = -1, max_loc = -1;                                 \
    int x = 0, y, loc = 0, width = size.width;                      \
    step /= sizeof(src[0]);                                         \
                                                                    \
    if(width*(cn) == step && width == maskStep)                   \
    {                                                               \
        width *= size.height;                                       \
        size.height = 1;                                            \
    }                                                               \
                                                                    \
    for(y = 0; y < size.height; y++, src += step,                  \
                                      mask += maskStep)            \
    {                                                               \
        for(x = 0; x < width; x++, loc++)                         \
            if(mask[x] != 0)                                      \
            {                                                       \
                min_loc = max_loc = loc;                            \
                min_val = (src)[x*(cn)];                            \
                min_val = max_val = _toggle_(min_val);            \
                goto stop_scan;                                     \
            }                                                       \
    }                                                               \
                                                                    \
    stop_scan:;


#define ICC_DEF_MINMAXLOC_1D_MASK_CASE_COI(_toggle_, temptype, cn) \
    for(; x < width; x++, loc++)      \
    {                                   \
        temptype val = src[x*(cn)];     \
        int m = mask[x] != 0;           \
        val = _toggle_(val);            \
                                        \
        if(val < min_val && m)        \
        {                               \
            min_val = val;              \
            min_loc = loc;              \
        }                               \
        else if(val > max_val && m)   \
        {                               \
            max_val = val;              \
            max_loc = loc;              \
        }                               \
    }


#define ICC_DEF_MINMAXLOC_MASK_FUNC_2D(_toggle_, _fin_cast_macro_, flavor, \
                                        srctype, temptype, extrtype)       \
IPCVAPI_IMPL(CStatus,                                                     \
icvMinMaxIndx_##flavor##_C1MR,(const srctype* src, int step,               \
    const uchar* mask, int maskStep, CSize size,                           \
    extrtype* minVal, extrtype* maxVal, CPoint* minLoc, CPoint* maxLoc), \
    (src, step, mask, maskStep, size, minVal, maxVal, minLoc, maxLoc))    \
{                                                                           \
    CC_MINMAXLOC_MASK_ENTRY(_toggle_, srctype, temptype, 1);              \
                                                                            \
    for(; y < size.height; y++, src += step, mask += maskStep)            \
    {                                                                       \
        ICC_DEF_MINMAXLOC_1D_MASK_CASE_COI(_toggle_, temptype, 1)         \
        x = 0;                                                              \
    }                                                                       \
                                                                            \
    CC_MINMAXLOC_EXIT(_fin_cast_macro_);                                  \
}


#define ICC_DEF_MINMAXLOC_MASK_FUNC_2D_COI(_toggle_, _fin_cast_macro_,     \
                                    flavor, srctype, temptype, extrtype)   \
static CStatus C_STDCALL                                                  \
icvMinMaxIndx_##flavor##_CnCMR(const srctype* src, int step,               \
    const uchar* mask, int maskStep, CSize size, int cn, int coi,          \
    extrtype* minVal, extrtype* maxVal, CPoint* minLoc, CPoint* maxLoc)  \
{                                                                           \
    (src) += coi - 1;                                                       \
    {CC_MINMAXLOC_MASK_ENTRY(_toggle_, srctype, temptype, cn);             \
                                                                            \
    for(; y < size.height; y++, src += step, mask += maskStep)            \
    {                                                                       \
        ICC_DEF_MINMAXLOC_1D_MASK_CASE_COI(_toggle_, temptype, cn)        \
        x = 0;                                                              \
    }                                                                       \
                                                                            \
    CC_MINMAXLOC_EXIT(_fin_cast_macro_);}                                  \
}



#define ICC_DEF_MINMAXLOC_MASK_ALL_INT(flavor, srctype,                    \
                                        _fin_cast_macro_, extrtype)        \
    ICC_DEF_MINMAXLOC_MASK_FUNC_2D(CC_NOP, _fin_cast_macro_, flavor,       \
                                    srctype, int, extrtype)                \
    ICC_DEF_MINMAXLOC_MASK_FUNC_2D_COI(CC_NOP, _fin_cast_macro_, flavor,   \
                                    srctype, int, extrtype)

#define ICC_DEF_MINMAXLOC_MASK_ALL_FLT(flavor, srctype, _toggle_,          \
                                        _fin_cast_macro_, extrtype)        \
    ICC_DEF_MINMAXLOC_MASK_FUNC_2D(_toggle_, _fin_cast_macro_, flavor,     \
                                    srctype, srctype, extrtype)            \
    ICC_DEF_MINMAXLOC_MASK_FUNC_2D_COI(_toggle_, _fin_cast_macro_, flavor, \
                                    srctype, srctype, extrtype)

ICC_DEF_MINMAXLOC_MASK_ALL_INT(8u, uchar, CC_CAST_32F, float)
ICC_DEF_MINMAXLOC_MASK_ALL_INT(16u, ushort, CC_CAST_32F, float)
ICC_DEF_MINMAXLOC_MASK_ALL_INT(16s, short, CC_CAST_32F, float)
ICC_DEF_MINMAXLOC_MASK_ALL_INT(32s, int, CC_CAST_64F, double)
ICC_DEF_MINMAXLOC_MASK_ALL_FLT(32f, int, CC_TOGGLE_FLT, minmax_to_float, float)
ICC_DEF_MINMAXLOC_MASK_ALL_FLT(64f, int64, CC_TOGGLE_DBL, minmax_to_double, double)

#define icvMinMaxIndx_8s_C1R    0
#define icvMinMaxIndx_8s_CnCR   0
#define icvMinMaxIndx_8s_C1MR   0
#define icvMinMaxIndx_8s_CnCMR  0

CC_DEF_INIT_FUNC_TAB_2D(MinMaxIndx, C1R)
CC_DEF_INIT_FUNC_TAB_2D(MinMaxIndx, CnCR)
CC_DEF_INIT_FUNC_TAB_2D(MinMaxIndx, C1MR)
CC_DEF_INIT_FUNC_TAB_2D(MinMaxIndx, CnCMR)


CC_IMPL  void
cvMinMaxLoc(const img_t* mat, double* _minVal, double* _maxVal,
             CPoint* _minLoc CC_DEFAULT(NULL), CPoint* _maxLoc CC_DEFAULT(NULL), const void* mask CC_DEFAULT(NULL))
{
    static CvFuncTable minmax_tab, minmaxcoi_tab;
    static CvFuncTable minmaxmask_tab, minmaxmaskcoi_tab;
    static int inittab = 0;

    CC_FUNCNAME("cvMinMaxLoc");

    __BEGIN__;

    TypeId type;
    int cn, coi = 0;
    int mat_step, mask_step = 0, cont_flag;
    CSize size;
    img_t *matmask = (img_t*)mask;
    CPoint minloc, maxloc;
    double minv = 0, maxv = 0;
    float minvf = 0.f, maxvf = 0.f;
    void *pmin = &minvf, *pmax = &maxvf;

    if(!inittab)
    {
        icvInitMinMaxIndxC1RTable(minmax_tab);
        icvInitMinMaxIndxCnCRTable(minmaxcoi_tab);
        icvInitMinMaxIndxC1MRTable(minmaxmask_tab);
        icvInitMinMaxIndxCnCMRTable(minmaxmaskcoi_tab);
        inittab = 1;
    }
    
    type = CC_MAT_DEPTH(mat);
    cn = CC_MAT_CN(mat);
    size = cvGetMatSize(mat);

    if(cn > 1 && coi == 0)
        CC_ERROR(CC_StsBadArg, "");

    if(type == CC_32S || type == CC_64F)
        pmin = &minv, pmax = &maxv;
    
    mat_step = mat->step;
    cont_flag = CC_IS_MAT_CONT(mat);

    if(mask)
    {
        if(!CC_IS_MASK_ARR(matmask))
            CC_ERROR(CC_StsBadMask, "");

        if(!CC_ARE_SIZES_EQ(mat, matmask))
            CC_ERROR(CC_StsUnmatchedSizes, "");

        mask_step = matmask->step;
        cont_flag &= CC_IS_MAT_CONT(matmask);
    }

    if(cont_flag)
    {
        size.width *= size.height;
        size.height = 1;
    }

    if(size.height == 1)
        mat_step = mask_step = CC_STUB_STEP;

    if(!mask)
    {
        if(CC_MAT_CN(mat) == 1 || coi == 0)
        {
            CvFunc2D_1A4P func = (CvFunc2D_1A4P)(minmax_tab[type]);

            if(!func)
                CC_ERROR(CC_StsBadArg, cvUnsupportedFormat);

            IPPI_CALL(func(mat->tt.data, mat_step, size,
                             pmin, pmax, &minloc, &maxloc));
        }
        else
        {
            CvFunc2DnC_1A4P func = (CvFunc2DnC_1A4P)(minmaxcoi_tab[type]);

            if(!func)
                CC_ERROR(CC_StsBadArg, cvUnsupportedFormat);

            IPPI_CALL(func(mat->tt.data, mat_step, size, cn, coi,
                             pmin, pmax, &minloc, &maxloc));
        }
    }
    else
    {
        if(CC_MAT_CN(mat) == 1 || coi == 0)
        {
            CvFunc2D_2A4P func = (CvFunc2D_2A4P)(minmaxmask_tab[type]);

            if(!func)
                CC_ERROR(CC_StsBadArg, cvUnsupportedFormat);

            IPPI_CALL(func(mat->tt.data, mat_step, matmask->tt.data,
                             mask_step, size,
                             pmin, pmax, &minloc, &maxloc));
        }
        else
        {
            CvFunc2DnC_2A4P func = (CvFunc2DnC_2A4P)(minmaxmaskcoi_tab[type]);

            if(!func)
                CC_ERROR(CC_StsBadArg, cvUnsupportedFormat);

            IPPI_CALL(func(mat->tt.data, mat_step,
                             matmask->tt.data, mask_step, size, cn, coi,
                             pmin, pmax, &minloc, &maxloc));
        }
    }

    if(matmask || _minLoc || _maxLoc)
    {
        if(minloc.x >= mat->cols)
        {
            minloc.y = minloc.x / mat->cols;
            minloc.x -= minloc.y * mat->cols;
        }

        if(maxloc.x >= mat->cols)
        {
            maxloc.y = maxloc.x / mat->cols;
            maxloc.x -= maxloc.y * mat->cols;
        }

        if(matmask && ((unsigned)minloc.x >= (unsigned)mat->cols ||
            (unsigned)minloc.y >= (unsigned)mat->rows ||
            matmask->tt.data[minloc.y*matmask->step + minloc.x] == 0 ||
            (unsigned)maxloc.x >= (unsigned)mat->cols ||
            (unsigned)maxloc.y >= (unsigned)mat->rows ||
            matmask->tt.data[maxloc.y*matmask->step + maxloc.x] == 0))
        {
            minloc.x = minloc.y = maxloc.x = maxloc.y = -1;
            minv = maxv = minvf = maxvf = 0;
        }

        if(_minLoc)
            *_minLoc = minloc;

        if(_maxLoc)
            *_maxLoc = maxloc;
    }

    if(type != CC_32S && type != CC_64F)
    {
        minv = minvf;
        maxv = maxvf;
    }

    if(_minVal)
        *_minVal = minv;

    if(_maxVal)
        *_maxVal = maxv;

    __END__;
}


