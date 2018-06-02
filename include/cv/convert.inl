

/****************************************************************************************\
*                            Splitting/extracting array channels                         *
\****************************************************************************************/

#define  ICC_DEF_PX2PL2PX_ENTRY_C2(arrtype_ptr, ptr)  \
    arrtype_ptr plane0 = ptr[0];                        \
    arrtype_ptr plane1 = ptr[1];

#define  ICC_DEF_PX2PL2PX_ENTRY_C3(arrtype_ptr, ptr)  \
    arrtype_ptr plane0 = ptr[0];                        \
    arrtype_ptr plane1 = ptr[1];                        \
    arrtype_ptr plane2 = ptr[2];

#define  ICC_DEF_PX2PL2PX_ENTRY_C4(arrtype_ptr, ptr)  \
    arrtype_ptr plane0 = ptr[0];                        \
    arrtype_ptr plane1 = ptr[1];                        \
    arrtype_ptr plane2 = ptr[2];                        \
    arrtype_ptr plane3 = ptr[3];


#define  ICC_DEF_PX2PL_C2(arrtype, len)           \
{                                                   \
    int j;                                          \
                                                    \
    for(j = 0; j < (len); j++, (src) += 2)        \
    {                                               \
        arrtype t0 = (src)[0];                      \
        arrtype t1 = (src)[1];                      \
                                                    \
        plane0[j] = t0;                             \
        plane1[j] = t1;                             \
    }                                               \
    plane0 += dststep;                              \
    plane1 += dststep;                              \
}


#define  ICC_DEF_PX2PL_C3(arrtype, len)           \
{                                                   \
    int j;                                          \
                                                    \
    for(j = 0; j < (len); j++, (src) += 3)        \
    {                                               \
        arrtype t0 = (src)[0];                      \
        arrtype t1 = (src)[1];                      \
        arrtype t2 = (src)[2];                      \
                                                    \
        plane0[j] = t0;                             \
        plane1[j] = t1;                             \
        plane2[j] = t2;                             \
    }                                               \
    plane0 += dststep;                              \
    plane1 += dststep;                              \
    plane2 += dststep;                              \
}


#define  ICC_DEF_PX2PL_C4(arrtype, len)           \
{                                                   \
    int j;                                          \
                                                    \
    for(j = 0; j < (len); j++, (src) += 4)        \
    {                                               \
        arrtype t0 = (src)[0];                      \
        arrtype t1 = (src)[1];                      \
                                                    \
        plane0[j] = t0;                             \
        plane1[j] = t1;                             \
                                                    \
        t0 = (src)[2];                              \
        t1 = (src)[3];                              \
                                                    \
        plane2[j] = t0;                             \
        plane3[j] = t1;                             \
    }                                               \
    plane0 += dststep;                              \
    plane1 += dststep;                              \
    plane2 += dststep;                              \
    plane3 += dststep;                              \
}


#define  ICC_DEF_PX2PL_COI(arrtype, len, cn)      \
{                                                   \
    int j;                                          \
                                                    \
    for(j = 0; j <= (len) - 4; j += 4, (src) += 4*(cn))\
    {                                               \
        arrtype t0 = (src)[0];                      \
        arrtype t1 = (src)[(cn)];                   \
                                                    \
        (dst)[j] = t0;                              \
        (dst)[j+1] = t1;                            \
                                                    \
        t0 = (src)[(cn)*2];                         \
        t1 = (src)[(cn)*3];                         \
                                                    \
        (dst)[j+2] = t0;                            \
        (dst)[j+3] = t1;                            \
    }                                               \
                                                    \
    for(; j < (len); j++, (src) += (cn))           \
    {                                               \
        (dst)[j] = (src)[0];                        \
    }                                               \
}


#define  ICC_DEF_COPY_PX2PL_FUNC_2D(arrtype, flavor,   \
                                     cn, entry_macro)  \
IPCVAPI_IMPL(CStatus, icvCopy_##flavor##_C##cn##P##cn##R,\
(const arrtype* src, int srcstep,                      \
  arrtype** dst, int dststep, CSize size),            \
  (src, srcstep, dst, dststep, size))                   \
{                                                       \
    entry_macro(arrtype*, dst);                         \
    srcstep /= sizeof(src[0]);                          \
    dststep /= sizeof(dst[0][0]);                       \
                                                        \
    for(; size.height--; src += srcstep)              \
    {                                                   \
        ICC_DEF_PX2PL_C##cn(arrtype, size.width);     \
        src -= size.width*(cn);                         \
    }                                                   \
                                                        \
    return CC_OK;                                       \
}


#define  ICC_DEF_COPY_PX2PL_FUNC_2D_COI(arrtype, flavor)\
IPCVAPI_IMPL(CStatus, icvCopy_##flavor##_CnC1CR,      \
(const arrtype* src, int srcstep, arrtype* dst, int dststep,\
  CSize size, int cn, int coi),                       \
  (src, srcstep, dst, dststep, size, cn, coi))          \
{                                                       \
    src += coi - 1;                                     \
    srcstep /= sizeof(src[0]);                          \
    dststep /= sizeof(dst[0]);                          \
                                                        \
    for(; size.height--; src += srcstep, dst += dststep)\
    {                                                   \
        ICC_DEF_PX2PL_COI(arrtype, size.width, cn);   \
        src -= size.width*(cn);                         \
    }                                                   \
                                                        \
    return CC_OK;                                       \
}


ICC_DEF_COPY_PX2PL_FUNC_2D(uchar, 8u, 2, ICC_DEF_PX2PL2PX_ENTRY_C2)
ICC_DEF_COPY_PX2PL_FUNC_2D(uchar, 8u, 3, ICC_DEF_PX2PL2PX_ENTRY_C3)
ICC_DEF_COPY_PX2PL_FUNC_2D(uchar, 8u, 4, ICC_DEF_PX2PL2PX_ENTRY_C4)
ICC_DEF_COPY_PX2PL_FUNC_2D(ushort, 16s, 2, ICC_DEF_PX2PL2PX_ENTRY_C2)
ICC_DEF_COPY_PX2PL_FUNC_2D(ushort, 16s, 3, ICC_DEF_PX2PL2PX_ENTRY_C3)
ICC_DEF_COPY_PX2PL_FUNC_2D(ushort, 16s, 4, ICC_DEF_PX2PL2PX_ENTRY_C4)
ICC_DEF_COPY_PX2PL_FUNC_2D(int, 32f, 2, ICC_DEF_PX2PL2PX_ENTRY_C2)
ICC_DEF_COPY_PX2PL_FUNC_2D(int, 32f, 3, ICC_DEF_PX2PL2PX_ENTRY_C3)
ICC_DEF_COPY_PX2PL_FUNC_2D(int, 32f, 4, ICC_DEF_PX2PL2PX_ENTRY_C4)
ICC_DEF_COPY_PX2PL_FUNC_2D(int64, 64f, 2, ICC_DEF_PX2PL2PX_ENTRY_C2)
ICC_DEF_COPY_PX2PL_FUNC_2D(int64, 64f, 3, ICC_DEF_PX2PL2PX_ENTRY_C3)
ICC_DEF_COPY_PX2PL_FUNC_2D(int64, 64f, 4, ICC_DEF_PX2PL2PX_ENTRY_C4)


ICC_DEF_COPY_PX2PL_FUNC_2D_COI(uchar, 8u)
ICC_DEF_COPY_PX2PL_FUNC_2D_COI(ushort, 16s)
ICC_DEF_COPY_PX2PL_FUNC_2D_COI(int, 32f)
ICC_DEF_COPY_PX2PL_FUNC_2D_COI(int64, 64f)


/****************************************************************************************\
*                            Merging/inserting array channels                            *
\****************************************************************************************/


#define  ICC_DEF_PL2PX_C2(arrtype, len)   \
{                                           \
    int j;                                  \
                                            \
    for(j = 0; j < (len); j++, (dst) += 2)\
    {                                       \
        arrtype t0 = plane0[j];             \
        arrtype t1 = plane1[j];             \
                                            \
        dst[0] = t0;                        \
        dst[1] = t1;                        \
    }                                       \
    plane0 += srcstep;                      \
    plane1 += srcstep;                      \
}


#define  ICC_DEF_PL2PX_C3(arrtype, len)   \
{                                           \
    int j;                                  \
                                            \
    for(j = 0; j < (len); j++, (dst) += 3)\
    {                                       \
        arrtype t0 = plane0[j];             \
        arrtype t1 = plane1[j];             \
        arrtype t2 = plane2[j];             \
                                            \
        dst[0] = t0;                        \
        dst[1] = t1;                        \
        dst[2] = t2;                        \
    }                                       \
    plane0 += srcstep;                      \
    plane1 += srcstep;                      \
    plane2 += srcstep;                      \
}


#define  ICC_DEF_PL2PX_C4(arrtype, len)   \
{                                           \
    int j;                                  \
                                            \
    for(j = 0; j < (len); j++, (dst) += 4)\
    {                                       \
        arrtype t0 = plane0[j];             \
        arrtype t1 = plane1[j];             \
                                            \
        dst[0] = t0;                        \
        dst[1] = t1;                        \
                                            \
        t0 = plane2[j];                     \
        t1 = plane3[j];                     \
                                            \
        dst[2] = t0;                        \
        dst[3] = t1;                        \
    }                                       \
    plane0 += srcstep;                      \
    plane1 += srcstep;                      \
    plane2 += srcstep;                      \
    plane3 += srcstep;                      \
}


#define  ICC_DEF_PL2PX_COI(arrtype, len, cn)          \
{                                                       \
    int j;                                              \
                                                        \
    for(j = 0; j <= (len) - 4; j += 4, (dst) += 4*(cn))\
    {                                                   \
        arrtype t0 = (src)[j];                          \
        arrtype t1 = (src)[j+1];                        \
                                                        \
        (dst)[0] = t0;                                  \
        (dst)[(cn)] = t1;                               \
                                                        \
        t0 = (src)[j+2];                                \
        t1 = (src)[j+3];                                \
                                                        \
        (dst)[(cn)*2] = t0;                             \
        (dst)[(cn)*3] = t1;                             \
    }                                                   \
                                                        \
    for(; j < (len); j++, (dst) += (cn))               \
    {                                                   \
        (dst)[0] = (src)[j];                            \
    }                                                   \
}


#define  ICC_DEF_COPY_PL2PX_FUNC_2D(arrtype, flavor, cn, entry_macro) \
IPCVAPI_IMPL(CStatus, icvCopy_##flavor##_P##cn##C##cn##R, \
(const arrtype** src, int srcstep,                         \
  arrtype* dst, int dststep, CSize size),                 \
  (src, srcstep, dst, dststep, size))                       \
{                                                           \
    entry_macro(const arrtype*, src);                       \
    srcstep /= sizeof(src[0][0]);                           \
    dststep /= sizeof(dst[0]);                              \
                                                            \
    for(; size.height--; dst += dststep)                  \
    {                                                       \
        ICC_DEF_PL2PX_C##cn(arrtype, size.width);         \
        dst -= size.width*(cn);                             \
    }                                                       \
                                                            \
    return CC_OK;                                           \
}


#define  ICC_DEF_COPY_PL2PX_FUNC_2D_COI(arrtype, flavor)  \
IPCVAPI_IMPL(CStatus, icvCopy_##flavor##_C1CnCR,          \
(const arrtype* src, int srcstep,                          \
  arrtype* dst, int dststep,                                \
  CSize size, int cn, int coi),                           \
  (src, srcstep, dst, dststep, size, cn, coi))              \
{                                                           \
    dst += coi - 1;                                         \
    srcstep /= sizeof(src[0]); dststep /= sizeof(dst[0]);   \
                                                            \
    for(; size.height--; src += srcstep, dst += dststep)  \
    {                                                       \
        ICC_DEF_PL2PX_COI(arrtype, size.width, cn);       \
        dst -= size.width*(cn);                             \
    }                                                       \
                                                            \
    return CC_OK;                                           \
}


ICC_DEF_COPY_PL2PX_FUNC_2D(uchar, 8u, 2, ICC_DEF_PX2PL2PX_ENTRY_C2)
ICC_DEF_COPY_PL2PX_FUNC_2D(uchar, 8u, 3, ICC_DEF_PX2PL2PX_ENTRY_C3)
ICC_DEF_COPY_PL2PX_FUNC_2D(uchar, 8u, 4, ICC_DEF_PX2PL2PX_ENTRY_C4)
ICC_DEF_COPY_PL2PX_FUNC_2D(ushort, 16s, 2, ICC_DEF_PX2PL2PX_ENTRY_C2)
ICC_DEF_COPY_PL2PX_FUNC_2D(ushort, 16s, 3, ICC_DEF_PX2PL2PX_ENTRY_C3)
ICC_DEF_COPY_PL2PX_FUNC_2D(ushort, 16s, 4, ICC_DEF_PX2PL2PX_ENTRY_C4)
ICC_DEF_COPY_PL2PX_FUNC_2D(int, 32f, 2, ICC_DEF_PX2PL2PX_ENTRY_C2)
ICC_DEF_COPY_PL2PX_FUNC_2D(int, 32f, 3, ICC_DEF_PX2PL2PX_ENTRY_C3)
ICC_DEF_COPY_PL2PX_FUNC_2D(int, 32f, 4, ICC_DEF_PX2PL2PX_ENTRY_C4)
ICC_DEF_COPY_PL2PX_FUNC_2D(int64, 64f, 2, ICC_DEF_PX2PL2PX_ENTRY_C2)
ICC_DEF_COPY_PL2PX_FUNC_2D(int64, 64f, 3, ICC_DEF_PX2PL2PX_ENTRY_C3)
ICC_DEF_COPY_PL2PX_FUNC_2D(int64, 64f, 4, ICC_DEF_PX2PL2PX_ENTRY_C4)

ICC_DEF_COPY_PL2PX_FUNC_2D_COI(uchar, 8u)
ICC_DEF_COPY_PL2PX_FUNC_2D_COI(ushort, 16s)
ICC_DEF_COPY_PL2PX_FUNC_2D_COI(int, 32f)
ICC_DEF_COPY_PL2PX_FUNC_2D_COI(int64, 64f)


#define  ICC_DEF_PXPLPX_TAB(name, FROM, TO)                           \
static void                                                             \
name(CvBigFuncTable  tab)                                             \
{                                                                       \
    tab[CC_8U][2] = (void*)icvCopy##_8u_##FROM##2##TO##2R;        \
    tab[CC_8U][3] = (void*)icvCopy##_8u_##FROM##3##TO##3R;        \
    tab[CC_8U][4] = (void*)icvCopy##_8u_##FROM##4##TO##4R;        \
                                                                        \
    tab[CC_8S][2] = (void*)icvCopy##_8u_##FROM##2##TO##2R;        \
    tab[CC_8S][3] = (void*)icvCopy##_8u_##FROM##3##TO##3R;        \
    tab[CC_8S][4] = (void*)icvCopy##_8u_##FROM##4##TO##4R;        \
                                                                        \
    tab[CC_16U][2] = (void*)icvCopy##_16s_##FROM##2##TO##2R;      \
    tab[CC_16U][3] = (void*)icvCopy##_16s_##FROM##3##TO##3R;      \
    tab[CC_16U][4] = (void*)icvCopy##_16s_##FROM##4##TO##4R;      \
                                                                        \
    tab[CC_16S][2] = (void*)icvCopy##_16s_##FROM##2##TO##2R;      \
    tab[CC_16S][3] = (void*)icvCopy##_16s_##FROM##3##TO##3R;      \
    tab[CC_16S][4] = (void*)icvCopy##_16s_##FROM##4##TO##4R;      \
                                                                        \
    tab[CC_32S][2] = (void*)icvCopy##_32f_##FROM##2##TO##2R;      \
    tab[CC_32S][3] = (void*)icvCopy##_32f_##FROM##3##TO##3R;      \
    tab[CC_32S][4] = (void*)icvCopy##_32f_##FROM##4##TO##4R;      \
                                                                        \
    tab[CC_32F][2] = (void*)icvCopy##_32f_##FROM##2##TO##2R;      \
    tab[CC_32F][3] = (void*)icvCopy##_32f_##FROM##3##TO##3R;      \
    tab[CC_32F][4] = (void*)icvCopy##_32f_##FROM##4##TO##4R;      \
                                                                        \
    tab[CC_64F][2] = (void*)icvCopy##_64f_##FROM##2##TO##2R;      \
    tab[CC_64F][3] = (void*)icvCopy##_64f_##FROM##3##TO##3R;      \
    tab[CC_64F][4] = (void*)icvCopy##_64f_##FROM##4##TO##4R;      \
}



#define  ICC_DEF_PXPLCOI_TAB(name, FROM, TO)                          \
static void                                                             \
name(CvFuncTable  tab)                                                \
{                                                                       \
    tab[CC_8U] = (void*)icvCopy##_8u_##FROM##TO##CR;             \
    tab[CC_8S] = (void*)icvCopy##_8u_##FROM##TO##CR;             \
    tab[CC_16U] = (void*)icvCopy##_16s_##FROM##TO##CR;           \
    tab[CC_16S] = (void*)icvCopy##_16s_##FROM##TO##CR;           \
    tab[CC_32S] = (void*)icvCopy##_32f_##FROM##TO##CR;           \
    tab[CC_32F] = (void*)icvCopy##_32f_##FROM##TO##CR;           \
    tab[CC_64F] = (void*)icvCopy##_64f_##FROM##TO##CR;           \
}


ICC_DEF_PXPLPX_TAB(icvInitSplitRTable, C, P)
ICC_DEF_PXPLCOI_TAB(icvInitSplitRCoiTable, Cn, C1)
ICC_DEF_PXPLPX_TAB(icvInitCvtPlaneToPixRTable, P, C)
ICC_DEF_PXPLCOI_TAB(icvInitCvtPlaneToPixRCoiTable, C1, Cn)

typedef CStatus (C_STDCALL *CvSplitFunc)(const void* src, int srcstep,
                                                    void** dst, int dststep, CSize size);

typedef CStatus (C_STDCALL *CvExtractPlaneFunc)(const void* src, int srcstep,
                                                   void* dst, int dststep,
                                                   CSize size, int cn, int coi);

typedef CStatus (C_STDCALL *CvMergeFunc)(const void** src, int srcstep,
                                                    void* dst, int dststep, CSize size);

typedef CStatus (C_STDCALL *CvInsertPlaneFunc)(const void* src, int srcstep,
                                                  void* dst, int dststep,
                                                  CSize size, int cn, int coi);

CC_IMPL void cvSplits(const img_t* src, img_t** dst) {
    static CvBigFuncTable  pxpl_tab;
    static CvFuncTable  pxplcoi_tab;
    static int inittab = 0;

    CC_FUNCNAME("cvSplit");

    __BEGIN__;

    CSize size;
    void* dstptr[4] = { 0, 0, 0, 0 };
    TypeId type;
    int cn, coi = 0;
    int i, nzplanes = 0, nzidx = -1;
    int cont_flag;
    int src_step, dst_step = 0;

    if(!inittab)
    {
        icvInitSplitRTable(pxpl_tab);
        icvInitSplitRCoiTable(pxplcoi_tab);
        inittab = 1;
    }

    //if(coi != 0)
    //    CC_ERROR(CC_BadCOI, "");

    type = CC_MAT_DEPTH(src);
    cn = CC_MAT_CN(src);

    cont_flag = CC_IS_MAT_CONT(src);

    if(cn == 1)
        CC_ERROR(CC_BadNumChannels, "");

    for(i = 0; i < 4; i++)
    {
        if(dst[i])
        {
            nzplanes++;
            nzidx = i;
            if(CC_MAT_CN(dst[i]) != 1)
                CC_ERROR(CC_BadNumChannels, "");
            if(!CC_ARE_DEPTHS_EQ(dst[i], src))
                CC_ERROR(CC_StsUnmatchedFormats, "");
            if(!CC_ARE_SIZES_EQ(dst[i], src))
                CC_ERROR(CC_StsUnmatchedSizes, "");
            if(nzplanes > i && i > 0 && dst[i]->step != dst[i-1]->step)
                CC_ERROR(CC_BadStep, "");
            dst_step = dst[i]->step;
            dstptr[nzplanes-1] = dst[i]->tt.data;

            cont_flag &= CC_IS_MAT_CONT(dst[i]);
        }
    }

    src_step = src->step;
    size = cvGetMatSize(src);

    if(cont_flag)
    {
        size.width *= size.height;
        src_step = dst_step = CC_STUB_STEP;

        size.height = 1;
    }

    if(nzplanes == cn)
    {
        CvSplitFunc func = (CvSplitFunc)pxpl_tab[type][cn];

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        IPPI_CALL(func(src->tt.data, src_step, dstptr, dst_step, size));
    }
    else if(nzplanes == 1)
    {
        CvExtractPlaneFunc func = (CvExtractPlaneFunc)pxplcoi_tab[type];

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        IPPI_CALL(func(src->tt.data, src_step,
                         dst[nzidx]->tt.data, dst_step,
                         size, cn, nzidx + 1));
    }
    else
    {
        CC_ERROR(CC_StsBadArg,
            "Either all output planes or only one output plane should be non zero");
    }

    __END__;

}

CC_IMPL void cvSplit(const img_t* src, img_t* dstarr0, img_t* dstarr1, img_t* dstarr2, img_t* dstarr3)
{
    img_t* dst[4] = {0};
    dst[0] = dstarr0;
    dst[1] = dstarr1;
    dst[2] = dstarr2;
    dst[3] = dstarr3;

    cvSplits(src, dst);
}


CC_IMPL void
cvMerges(const img_t** src, int n, img_t* dst) {
    static CvBigFuncTable plpx_tab;
    static CvFuncTable plpxcoi_tab;
    static int inittab = 0;

    CC_FUNCNAME("cvMerge");

    __BEGIN__;

    int src_step = 0, dst_step;
    CSize size;
    const void* srcptr[4] = { 0, 0, 0, 0 };
    TypeId type;
    int cn, coi = 0;
    int i, nzplanes = 0, nzidx = -1;
    int cont_flag;

    if(!inittab)
    {
        icvInitCvtPlaneToPixRTable(plpx_tab);
        icvInitCvtPlaneToPixRCoiTable(plpxcoi_tab);
        inittab = 1;
    }

    type = CC_MAT_DEPTH(dst);
    cn = CC_MAT_CN(dst);

    cont_flag = 1;

    if(cn == 1)
        CC_ERROR(CC_BadNumChannels, "");

    for(i = 0; i < 4; i++)
    {
        if(src[i])
        {
            nzplanes++;
            nzidx = i;
            if(CC_MAT_CN(src[i]) != 1)
                CC_ERROR(CC_BadNumChannels, "");
            if(!CC_ARE_DEPTHS_EQ(src[i], dst))
                CC_ERROR(CC_StsUnmatchedFormats, "");
            if(!CC_ARE_SIZES_EQ(src[i], dst))
                CC_ERROR(CC_StsUnmatchedSizes, "");
            if(nzplanes > i && i > 0 && src[i]->step != src[i-1]->step)
                CC_ERROR(CC_BadStep, "");
            src_step = src[i]->step;
            srcptr[nzplanes-1] = (const void*)(src[i]->tt.data);

            cont_flag &= CC_IS_MAT_CONT(src[i]);
        }
    }

    size = cvGetMatSize(dst);
    dst_step = dst->step;

    if(cont_flag)
    {
        size.width *= size.height;
        src_step = dst_step = CC_STUB_STEP;
        size.height = 1;
    }

    if(nzplanes == cn)
    {
        CvMergeFunc func = (CvMergeFunc)plpx_tab[type][cn];

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        IPPI_CALL(func(srcptr, src_step, dst->tt.data, dst_step, size));
    }
    else if(nzplanes == 1)
    {
        CvInsertPlaneFunc func = (CvInsertPlaneFunc)plpxcoi_tab[type];

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        IPPI_CALL(func(src[nzidx]->tt.data, src_step,
                         dst->tt.data, dst_step,
                         size, cn, nzidx + 1));
    }
    else
    {
        CC_ERROR(CC_StsBadArg,
            "Either all input planes or only one input plane should be non zero");
    }


    __END__;
}

CC_IMPL void
cvMerge(const img_t* srcarr0, const img_t* srcarr1, const img_t* srcarr2,
         const img_t* srcarr3, img_t* dst)
{
    const img_t *src[4] = {0};
    src[0] = srcarr0;
    src[1] = srcarr1;
    src[2] = srcarr2;
    src[3] = srcarr3;

    cvMerges(src, 4, dst);
}


/****************************************************************************************\
*                       Generalized split/merge: mixing channels                         *
\****************************************************************************************/

#define  ICC_DEF_MIX_CH_FUNC_2D(arrtype, flavor)              \
static CStatus C_STDCALL                                      \
icvMixChannels_##flavor(const arrtype** src, int* sdelta0,     \
                         int* sdelta1, arrtype** dst,           \
                         int* ddelta0, int* ddelta1,            \
                         int n, CSize size)                   \
{                                                               \
    int i, k;                                                   \
    int block_size0 = n == 1 ? size.width : 1024;               \
                                                                \
    for(; size.height--;)                                     \
    {                                                           \
        int remaining = size.width;                             \
        for(; remaining > 0;)                                 \
        {                                                       \
            int block_size = MIN(remaining, block_size0);     \
            for(k = 0; k < n; k++)                            \
            {                                                   \
                const arrtype* s = src[k];                      \
                arrtype* d = dst[k];                            \
                int ds = sdelta1[k], dd = ddelta1[k];           \
                if(s)                                         \
                {                                               \
                    for(i = 0; i <= block_size - 2; i += 2,    \
                                        s += ds*2, d += dd*2)  \
                    {                                           \
                        arrtype t0 = s[0], t1 = s[ds];          \
                        d[0] = t0; d[dd] = t1;                  \
                    }                                           \
                    if(i < block_size)                        \
                        d[0] = s[0], s += ds, d += dd;          \
                    src[k] = s;                                 \
                }                                               \
                else                                            \
                {                                               \
                    for(i=0; i <= block_size-2; i+=2, d+=dd*2)\
                        d[0] = d[dd] = 0;                       \
                    if(i < block_size)                        \
                        d[0] = 0, d += dd;                      \
                }                                               \
                dst[k] = d;                                     \
            }                                                   \
            remaining -= block_size;                            \
        }                                                       \
        for(k = 0; k < n; k++)                                \
            src[k] += sdelta0[k], dst[k] += ddelta0[k];         \
    }                                                           \
                                                                \
    return CC_OK;                                               \
}


ICC_DEF_MIX_CH_FUNC_2D(uchar, 8u)
ICC_DEF_MIX_CH_FUNC_2D(ushort, 16u)
ICC_DEF_MIX_CH_FUNC_2D(int, 32s)
ICC_DEF_MIX_CH_FUNC_2D(int64, 64s)

static void
icvInitMixChannelsTab(CvFuncTable  tab)
{
    tab[CC_8U] = (void*)icvMixChannels_8u;
    tab[CC_8S] = (void*)icvMixChannels_8u;
    tab[CC_16U] = (void*)icvMixChannels_16u;
    tab[CC_16S] = (void*)icvMixChannels_16u;
    tab[CC_32S] = (void*)icvMixChannels_32s;
    tab[CC_32F] = (void*)icvMixChannels_32s;
    tab[CC_64F] = (void*)icvMixChannels_64s;
}

typedef CStatus (C_STDCALL * CvMixChannelsFunc)(const void** src, int* sdelta0,
        int* sdelta1, void** dst, int* ddelta0, int* ddelta1, int n, CSize size);

CC_IMPL void
cvMixChannels(const img_t** src, int src_count,
               img_t** dst, int dst_count,
               const int* from_to, int pair_count)
{
    static CvFuncTable mixcn_tab;
    static int inittab = 0;
    uchar* buffer = 0;
    int heap_alloc = 0;
    
    CC_FUNCNAME("cvMixChannels");

    __BEGIN__;
    
    CSize size = {0,0};
    TypeId type;
    int elem_size = 1;
    int *sdelta0 = 0, *sdelta1 = 0, *ddelta0 = 0, *ddelta1 = 0;
    uchar **sptr = 0, **dptr = 0;
    uchar **src0 = 0, **dst0 = 0;
    int* src_cn = 0, *dst_cn = 0;
    int* src_step = 0, *dst_step = 0;
    int buf_size, i, k;
    int cont_flag = 1;
    CvMixChannelsFunc func;

    if(!inittab)
    {
        icvInitMixChannelsTab(mixcn_tab);
        inittab = 1;
    }

    src_count = MAX(src_count, 0);

    if(!src && src_count > 0)
        CC_ERROR(CC_StsNullPtr, "The input array of arrays is NULL");

    if(!dst)
        CC_ERROR(CC_StsNullPtr, "The output array of arrays is NULL");

    if(dst_count <= 0 || pair_count <= 0)
        CC_ERROR(CC_StsOutOfRange,
        "The number of output arrays and the number of copied channels must be positive");

    if(!from_to)
        CC_ERROR(CC_StsNullPtr, "The array of copied channel indices is NULL");

    buf_size = (src_count + dst_count + 2)*
        (sizeof(src0[0]) + sizeof(src_cn[0]) + sizeof(src_step[0])) + 
        pair_count*2*(sizeof(sptr[0]) + sizeof(sdelta0[0]) + sizeof(sdelta1[0]));

    if(buf_size > CC_MAX_LOCAL_SIZE)
    {
        CC_CALL(buffer = (uchar*)cAlloc(buf_size));
        heap_alloc = 1;
    }
    else
        buffer = (uchar*)cvStackAlloc(buf_size);

    src0 = (uchar**)buffer;
    dst0 = src0 + src_count;
    src_cn = (int*)(dst0 + dst_count);
    dst_cn = src_cn + src_count + 1;
    src_step = dst_cn + dst_count + 1;
    dst_step = src_step + src_count;

    sptr = (uchar**)cvAlignPtr(dst_step + dst_count, (int)sizeof(void*));
    dptr = sptr + pair_count;
    sdelta0 = (int*)(dptr + pair_count);
    sdelta1 = sdelta0 + pair_count;
    ddelta0 = sdelta1 + pair_count;
    ddelta1 = ddelta0 + pair_count;

    src_cn[0] = dst_cn[0] = 0;

    for(k = 0; k < 2; k++)
    {
        for(i = 0; i < (k == 0 ? src_count : dst_count); i++)
        {
            img_t *mat = (img_t*)(k == 0 ? src[i] : dst[i]);
            int cn;
        
            if(i==0)
            {
                type = CC_MAT_DEPTH(mat);
                elem_size = CC_ELEMTYPE_SIZE(mat);
                size = cvGetMatSize(mat);
            }

            if(CC_MAT_TYPE(mat) != type)
                CC_ERROR(CC_StsUnmatchedFormats, "All the arrays must have the same bit depth");

            if(mat->cols != size.width || mat->rows != size.height)
                CC_ERROR(CC_StsUnmatchedSizes, "All the arrays must have the same size");

            if(k == 0)
            {
                src0[i] = mat->tt.data;
                cn = CC_MAT_CN(mat);
                src_cn[i+1] = src_cn[i] + cn;
                src_step[i] = mat->step / elem_size - size.width * cn;
            }
            else
            {
                dst0[i] = mat->tt.data;
                cn = CC_MAT_CN(mat);
                dst_cn[i+1] = dst_cn[i] + cn;
                dst_step[i] = mat->step / elem_size - size.width * cn;
            }

            cont_flag &= mat->tid;
        }
    }

    if(cont_flag)
    {
        size.width *= size.height;
        size.height = 1;
    }

    for(i = 0; i < pair_count; i++)
    {
        for(k = 0; k < 2; k++)
        {
            int cn = from_to[i*2 + k];
            const int* cn_arr = k == 0 ? src_cn : dst_cn;
            int a = 0, b = k == 0 ? src_count-1 : dst_count-1;

            if(cn < 0 || cn >= cn_arr[b+1])
            {
                if(k == 0 && cn < 0)
                {
                    sptr[i] = 0;
                    sdelta0[i] = sdelta1[i] = 0;
                    continue;
                }
                else
                {
                    char err_str[100];
                    sprintf(err_str, "channel index #%d in the array of pairs is negative "
                        "or exceeds the total number of channels in all the %s arrays", i*2+k,
                        k == 0 ? "input" : "output");
                    CC_ERROR(CC_StsOutOfRange, err_str);
                }
            }

            for(; cn >= cn_arr[a+1]; a++)
                ;
            
            if(k == 0)
            {
                sptr[i] = src0[a] + (cn - cn_arr[a])*elem_size;
                sdelta1[i] = cn_arr[a+1] - cn_arr[a];
                sdelta0[i] = src_step[a];
            }
            else
            {
                dptr[i] = dst0[a] + (cn - cn_arr[a])*elem_size;
                ddelta1[i] = cn_arr[a+1] - cn_arr[a];
                ddelta0[i] = dst_step[a];
            }
        }
    }

    func = (CvMixChannelsFunc)mixcn_tab[type];
    if(!func)
        CC_ERROR(CC_StsUnsupportedFormat, "The data type is not supported by the function");

    IPPI_CALL(func((const void**)sptr, sdelta0, sdelta1, (void**)dptr,
                     ddelta0, ddelta1, pair_count, size)); 

    __END__;

    if(buffer && heap_alloc)
        cFree(&buffer);
}


/****************************************************************************************\
*                                   cvConvertScaleAbs                                    *
\****************************************************************************************/

#define ICC_DEF_CVT_SCALE_ABS_CASE(srctype, worktype,                  \
            scale_macro, abs_macro, cast_macro, a, b)                  \
                                                                        \
{                                                                       \
    const srctype* _src = (const srctype*)src;                          \
    srcstep /= sizeof(_src[0]); /*dststep /= sizeof(_dst[0]);*/         \
                                                                        \
    for(; size.height--; _src += srcstep, dst += dststep)             \
    {                                                                   \
        int i;                                                          \
                                                                        \
        for(i = 0; i <= size.width - 4; i += 4)                       \
        {                                                               \
            worktype t0 = scale_macro((a)*_src[i] + (b));               \
            worktype t1 = scale_macro((a)*_src[i+1] + (b));             \
                                                                        \
            t0 = (worktype)abs_macro(t0);                               \
            t1 = (worktype)abs_macro(t1);                               \
                                                                        \
            dst[i] = cast_macro(t0);                                    \
            dst[i+1] = cast_macro(t1);                                  \
                                                                        \
            t0 = scale_macro((a)*_src[i+2] + (b));                      \
            t1 = scale_macro((a)*_src[i+3] + (b));                      \
                                                                        \
            t0 = (worktype)abs_macro(t0);                               \
            t1 = (worktype)abs_macro(t1);                               \
                                                                        \
            dst[i+2] = cast_macro(t0);                                  \
            dst[i+3] = cast_macro(t1);                                  \
        }                                                               \
                                                                        \
        for(; i < size.width; i++)                                    \
        {                                                               \
            worktype t0 = scale_macro((a)*_src[i] + (b));               \
            t0 = (worktype)abs_macro(t0);                               \
            dst[i] = cast_macro(t0);                                    \
        }                                                               \
    }                                                                   \
}


#define ICC_FIX_SHIFT  15
#define ICC_SCALE(x)   (((x) + (1 << (ICC_FIX_SHIFT-1))) >> ICC_FIX_SHIFT)

static CStatus C_STDCALL
icvCvtScaleAbsTo_8u_C1R(const uchar* src, int srcstep,
                         uchar* dst, int dststep,
                         CSize size, double scale, double shift,
                         int param, int src_cn)
{
    TypeId srctype = (TypeId)param;

    size.width *= src_cn;

    switch(srctype)
    {
    case  CC_8S:
    case  CC_8U:
        {
        uchar lut[256];
        int i;
        double val = shift;
        
        for(i = 0; i < 128; i++, val += scale)
        {
            int t = cRound(fabs(val));
            lut[i] = CC_CAST_8U(t);
        }
        
        if(srctype == CC_8S)
            val = -val;
        
        for(; i < 256; i++, val += scale)
        {
            int t = cRound(fabs(val));
            lut[i] = CC_CAST_8U(t);
        }

        icvLUT_Transform8u_8u_C1R(src, srcstep, dst,
                                   dststep, size, lut);
        }
        break;
    case  CC_16U:
        if(fabs(scale) <= 1. && fabs(shift) < DBL_EPSILON)
        {
            int iscale = cRound(scale*(1 << ICC_FIX_SHIFT));

            if(iscale == ICC_FIX_SHIFT)
            {
                ICC_DEF_CVT_SCALE_ABS_CASE(ushort, int, CC_NOP, CC_IABS,
                                            CC_CAST_8U, 1, 0);
            }
            else
            {
                ICC_DEF_CVT_SCALE_ABS_CASE(ushort, int, ICC_SCALE, CC_IABS,
                                            CC_CAST_8U, iscale, 0);
            }
        }
        else
        {
            ICC_DEF_CVT_SCALE_ABS_CASE(ushort, int, cRound, CC_IABS,
                                        CC_CAST_8U, scale, shift);
        }
        break;
    case  CC_16S:
        if(fabs(scale) <= 1. &&
            fabs(shift) <= (INT_MAX*0.5)/(1 << ICC_FIX_SHIFT))
        {
            int iscale = cRound(scale*(1 << ICC_FIX_SHIFT));
            int ishift = cRound(shift*(1 << ICC_FIX_SHIFT));

            if(iscale == ICC_FIX_SHIFT && ishift == 0)
            {
                ICC_DEF_CVT_SCALE_ABS_CASE(short, int, CC_NOP, CC_IABS,
                                            CC_CAST_8U, 1, 0);
            }
            else
            {
                ICC_DEF_CVT_SCALE_ABS_CASE(short, int, ICC_SCALE, CC_IABS,
                                            CC_CAST_8U, iscale, ishift);
            }
        }
        else
        {
            ICC_DEF_CVT_SCALE_ABS_CASE(short, int, cRound, CC_IABS,
                                        CC_CAST_8U, scale, shift);
        }
        break;
    case  CC_32S:
        ICC_DEF_CVT_SCALE_ABS_CASE(int, int, cRound, CC_IABS,
                                    CC_CAST_8U, scale, shift);
        break;
    case  CC_32F:
        ICC_DEF_CVT_SCALE_ABS_CASE(float, int, cRound, CC_IABS,
                                    CC_CAST_8U, scale, shift);
        break;
    case  CC_64F:
        ICC_DEF_CVT_SCALE_ABS_CASE(double, int, cRound, CC_IABS,
                                    CC_CAST_8U, scale, shift);
        break;
    default:
        assert(0);
        return CC_BADFLAG_ERR;
    }

    return  CC_OK;
}


CC_IMPL void
cvConvertScaleAbs(const img_t* src, img_t* dst,
                   double scale, double shift)
{
    CC_FUNCNAME("cvConvertScaleAbs");

    __BEGIN__;

    int coi1 = 0, coi2 = 0;
    
    CSize size;
    int src_step, dst_step;

    if(coi1 != 0 || coi2 != 0)
        CC_ERROR(CC_BadCOI, "");

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    if(!CC_ARE_CNS_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(CC_MAT_TYPE(dst) != CC_8U)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    size = cvGetMatSize(src);
    src_step = src->step;
    dst_step = dst->step;

    if(CC_IS_MAT_CONT(src) && CC_IS_MAT_CONT(dst))
    {
        size.width *= size.height;
        src_step = dst_step = CC_STUB_STEP;
        size.height = 1;
    }

    IPPI_CALL(icvCvtScaleAbsTo_8u_C1R(src->tt.data, src_step,
                             (uchar*)(dst->tt.data), dst_step,
                             size, scale, shift, src->tid, CC_MAT_CN(src)));
    __END__;
}

/****************************************************************************************\
*                                      cvConvertScale                                    *
\****************************************************************************************/

#define ICC_DEF_CVT_SCALE_CASE(srctype, worktype,          \
                            scale_macro, cast_macro, a, b) \
                                                            \
{                                                           \
    const srctype* _src = (const srctype*)src;              \
    srcstep /= sizeof(_src[0]);                             \
                                                            \
    for(; size.height--; _src += srcstep, dst += dststep) \
    {                                                       \
        for(i = 0; i <= size.width - 4; i += 4)           \
        {                                                   \
            worktype t0 = scale_macro((a)*_src[i]+(b));     \
            worktype t1 = scale_macro((a)*_src[i+1]+(b));   \
                                                            \
            dst[i] = cast_macro(t0);                        \
            dst[i+1] = cast_macro(t1);                      \
                                                            \
            t0 = scale_macro((a)*_src[i+2] + (b));          \
            t1 = scale_macro((a)*_src[i+3] + (b));          \
                                                            \
            dst[i+2] = cast_macro(t0);                      \
            dst[i+3] = cast_macro(t1);                      \
        }                                                   \
                                                            \
        for(; i < size.width; i++)                        \
        {                                                   \
            worktype t0 = scale_macro((a)*_src[i] + (b));   \
            dst[i] = cast_macro(t0);                        \
        }                                                   \
    }                                                       \
}

#define  ICC_DEF_CVT_SCALE_FUNC_INT(flavor, dsttype, cast_macro)      \
static  CStatus  C_STDCALL                                            \
icvCvtScaleTo_##flavor##_C1R(const uchar* src, int srcstep,            \
                              dsttype* dst, int dststep, CSize size,   \
                              double scale, double shift, int param)   \
{                                                                       \
    int i, srctype = param;                                             \
    dsttype lut[256];                                                   \
    dststep /= sizeof(dst[0]);                                          \
                                                                        \
    switch(srctype)                                                     \
    {                                                                   \
    case  CC_8U:                                                        \
        if(size.width*size.height >= 256)                             \
        {                                                               \
            double val = shift;                                         \
            for(i = 0; i < 256; i++, val += scale)                    \
            {                                                           \
                int t = cRound(val);                                   \
                lut[i] = cast_macro(t);                                 \
            }                                                           \
                                                                        \
            icvLUT_Transform8u_##flavor##_C1R(src, srcstep, (dsttype*)dst,       \
                                dststep*sizeof(dst[0]), size, lut);    \
        }                                                               \
        else if(fabs(scale) <= 128. &&                               \
                 fabs(shift) <= (INT_MAX*0.5)/(1 << ICC_FIX_SHIFT))   \
        {                                                               \
            int iscale = cRound(scale*(1 << ICC_FIX_SHIFT));           \
            int ishift = cRound(shift*(1 << ICC_FIX_SHIFT));           \
                                                                        \
            ICC_DEF_CVT_SCALE_CASE(uchar, int, ICC_SCALE,              \
                                    cast_macro, iscale, ishift);       \
        }                                                               \
        else                                                            \
        {                                                               \
            ICC_DEF_CVT_SCALE_CASE(uchar, int, cRound,                \
                                    cast_macro, scale, shift);         \
        }                                                               \
        break;                                                          \
    case  CC_8S:                                                        \
        if(size.width*size.height >= 256)                             \
        {                                                               \
            for(i = 0; i < 256; i++)                                  \
            {                                                           \
                int t = cRound((char)i*scale + shift);               \
                lut[i] = cast_macro(t);                                 \
            }                                                           \
                                                                        \
            icvLUT_Transform8u_##flavor##_C1R(src, srcstep, dst,       \
                                dststep*sizeof(dst[0]), size, lut);    \
        }                                                               \
        else if(fabs(scale) <= 128. &&                               \
                 fabs(shift) <= (INT_MAX*0.5)/(1 << ICC_FIX_SHIFT))   \
        {                                                               \
            int iscale = cRound(scale*(1 << ICC_FIX_SHIFT));           \
            int ishift = cRound(shift*(1 << ICC_FIX_SHIFT));           \
                                                                        \
            ICC_DEF_CVT_SCALE_CASE(char, int, ICC_SCALE,               \
                                    cast_macro, iscale, ishift);       \
        }                                                               \
        else                                                            \
        {                                                               \
            ICC_DEF_CVT_SCALE_CASE(char, int, cRound,                 \
                                    cast_macro, scale, shift);         \
        }                                                               \
        break;                                                          \
    case  CC_16U:                                                       \
        if(fabs(scale) <= 1. && fabs(shift) < DBL_EPSILON)          \
        {                                                               \
            int iscale = cRound(scale*(1 << ICC_FIX_SHIFT));           \
                                                                        \
            ICC_DEF_CVT_SCALE_CASE(ushort, int, ICC_SCALE,             \
                                    cast_macro, iscale, 0);            \
        }                                                               \
        else                                                            \
        {                                                               \
            ICC_DEF_CVT_SCALE_CASE(ushort, int, cRound,               \
                                    cast_macro, scale, shift);         \
        }                                                               \
        break;                                                          \
    case  CC_16S:                                                       \
        if(fabs(scale) <= 1. &&                                      \
            fabs(shift) <= (INT_MAX*0.5)/(1 << ICC_FIX_SHIFT))        \
        {                                                               \
            int iscale = cRound(scale*(1 << ICC_FIX_SHIFT));           \
            int ishift = cRound(shift*(1 << ICC_FIX_SHIFT));           \
                                                                        \
            ICC_DEF_CVT_SCALE_CASE(short, int, ICC_SCALE,              \
                                    cast_macro, iscale, ishift);       \
        }                                                               \
        else                                                            \
        {                                                               \
            ICC_DEF_CVT_SCALE_CASE(short, int, cRound,                \
                                    cast_macro, scale, shift);         \
        }                                                               \
        break;                                                          \
    case  CC_32S:                                                       \
        ICC_DEF_CVT_SCALE_CASE(int, int, cRound,                      \
                                cast_macro, scale, shift);             \
        break;                                                          \
    case  CC_32F:                                                       \
        ICC_DEF_CVT_SCALE_CASE(float, int, cRound,                    \
                                cast_macro, scale, shift);             \
        break;                                                          \
    case  CC_64F:                                                       \
        ICC_DEF_CVT_SCALE_CASE(double, int, cRound,                   \
                                cast_macro, scale, shift);             \
        break;                                                          \
    default:                                                            \
        assert(0);                                                      \
        return CC_BADFLAG_ERR;                                          \
    }                                                                   \
                                                                        \
    return  CC_OK;                                                      \
}


#define  ICC_DEF_CVT_SCALE_FUNC_FLT(flavor, dsttype, cast_macro)      \
static  CStatus  C_STDCALL                                            \
icvCvtScaleTo_##flavor##_C1R(const uchar* src, int srcstep,            \
                              dsttype* dst, int dststep, CSize size,   \
                              double scale, double shift, int param)   \
{                                                                       \
    int i, srctype = param;                                             \
    dsttype lut[256];                                                   \
    dststep /= sizeof(dst[0]);                                          \
                                                                        \
    switch(srctype)                                                     \
    {                                                                   \
    case  CC_8U:                                                        \
        if(size.width*size.height >= 256)                             \
        {                                                               \
            double val = shift;                                         \
            for(i = 0; i < 256; i++, val += scale)                    \
                lut[i] = (dsttype)val;                                  \
                                                                        \
            icvLUT_Transform8u_##flavor##_C1R(src, srcstep, (dsttype*)dst,       \
                                dststep*sizeof(dst[0]), size, lut);    \
        }                                                               \
        else                                                            \
        {                                                               \
            ICC_DEF_CVT_SCALE_CASE(uchar, double, CC_NOP,              \
                                    cast_macro, scale, shift);         \
        }                                                               \
        break;                                                          \
    case  CC_8S:                                                        \
        if(size.width*size.height >= 256)                             \
        {                                                               \
            for(i = 0; i < 256; i++)                                  \
                lut[i] = (dsttype)((char)i*scale + shift);              \
                                                                        \
            icvLUT_Transform8u_##flavor##_C1R(src, srcstep, (dsttype*)dst,       \
                                dststep*sizeof(dst[0]), size, lut);    \
        }                                                               \
        else                                                            \
        {                                                               \
            ICC_DEF_CVT_SCALE_CASE(char, double, CC_NOP,               \
                                    cast_macro, scale, shift);         \
        }                                                               \
        break;                                                          \
    case  CC_16U:                                                       \
        ICC_DEF_CVT_SCALE_CASE(ushort, double, CC_NOP,                 \
                                cast_macro, scale, shift);             \
        break;                                                          \
    case  CC_16S:                                                       \
        ICC_DEF_CVT_SCALE_CASE(short, double, CC_NOP,                  \
                                cast_macro, scale, shift);             \
        break;                                                          \
    case  CC_32S:                                                       \
        ICC_DEF_CVT_SCALE_CASE(int, double, CC_NOP,                    \
                                cast_macro, scale, shift);             \
        break;                                                          \
    case  CC_32F:                                                       \
        ICC_DEF_CVT_SCALE_CASE(float, double, CC_NOP,                  \
                                cast_macro, scale, shift);             \
        break;                                                          \
    case  CC_64F:                                                       \
        ICC_DEF_CVT_SCALE_CASE(double, double, CC_NOP,                 \
                                cast_macro, scale, shift);             \
        break;                                                          \
    default:                                                            \
        assert(0);                                                      \
        return CC_BADFLAG_ERR;                                          \
    }                                                                   \
                                                                        \
    return  CC_OK;                                                      \
}


ICC_DEF_CVT_SCALE_FUNC_INT(8u, uchar, CC_CAST_8U)
ICC_DEF_CVT_SCALE_FUNC_INT(8s, char, CC_CAST_8S)
ICC_DEF_CVT_SCALE_FUNC_INT(16s, short, CC_CAST_16S)
ICC_DEF_CVT_SCALE_FUNC_INT(16u, ushort, CC_CAST_16U)
ICC_DEF_CVT_SCALE_FUNC_INT(32s, int, CC_CAST_32S)

ICC_DEF_CVT_SCALE_FUNC_FLT(32f, float, CC_CAST_32F)
ICC_DEF_CVT_SCALE_FUNC_FLT(64f, double, CC_CAST_64F)

CC_DEF_INIT_FUNC_TAB_2D(CvtScaleTo, C1R)


/****************************************************************************************\
*                             Conversion w/o scaling macros                              *
\****************************************************************************************/

#define ICC_DEF_CVT_CASE_2D(srctype, worktype,             \
                             cast_macro1, cast_macro2)     \
{                                                           \
    const srctype* _src = (const srctype*)src;              \
    srcstep /= sizeof(_src[0]);                             \
                                                            \
    for(; size.height--; _src += srcstep, dst += dststep) \
    {                                                       \
        int i;                                              \
                                                            \
        for(i = 0; i <= size.width - 4; i += 4)           \
        {                                                   \
            worktype t0 = cast_macro1(_src[i]);             \
            worktype t1 = cast_macro1(_src[i+1]);           \
                                                            \
            dst[i] = cast_macro2(t0);                       \
            dst[i+1] = cast_macro2(t1);                     \
                                                            \
            t0 = cast_macro1(_src[i+2]);                    \
            t1 = cast_macro1(_src[i+3]);                    \
                                                            \
            dst[i+2] = cast_macro2(t0);                     \
            dst[i+3] = cast_macro2(t1);                     \
        }                                                   \
                                                            \
        for(; i < size.width; i++)                        \
        {                                                   \
            worktype t0 = cast_macro1(_src[i]);             \
            dst[i] = cast_macro2(t0);                       \
        }                                                   \
    }                                                       \
}


#define ICC_DEF_CVT_FUNC_2D(flavor, dsttype, worktype, cast_macro2,    \
                             srctypeid1, srctype1, cast_macro11,         \
                             srctypeid2, srctype2, cast_macro12,         \
                             srctypeid3, srctype3, cast_macro13,         \
                             srctypeid4, srctype4, cast_macro14,         \
                             srctypeid5, srctype5, cast_macro15,         \
                             srctypeid6, srctype6, cast_macro16)        \
static CStatus C_STDCALL                                              \
icvCvtTo_##flavor##_C1R(const uchar* src, int srcstep,                 \
                         dsttype* dst, int dststep,                     \
                         CSize size, int param)                       \
{                                                                       \
    int srctype = param;                                                \
    dststep /= sizeof(dst[0]);                                          \
                                                                        \
    switch(srctype)                                     \
    {                                                                   \
    case srctypeid1:                                                     \
        ICC_DEF_CVT_CASE_2D(srctype1, worktype,                        \
                             cast_macro11, cast_macro2);               \
        break;                                                          \
    case srctypeid2:                                                     \
        ICC_DEF_CVT_CASE_2D(srctype2, worktype,                        \
                             cast_macro12, cast_macro2);               \
        break;                                                          \
    case srctypeid3:                                                     \
        ICC_DEF_CVT_CASE_2D(srctype3, worktype,                        \
                             cast_macro13, cast_macro2);               \
        break;                                                          \
    case srctypeid4:                                                     \
        ICC_DEF_CVT_CASE_2D(srctype4, worktype,                        \
                             cast_macro14, cast_macro2);               \
        break;                                                          \
    case srctypeid5:                                                     \
        ICC_DEF_CVT_CASE_2D(srctype5, worktype,                        \
                             cast_macro15, cast_macro2);               \
        break;                                                          \
    case srctypeid6:                                                     \
        ICC_DEF_CVT_CASE_2D(srctype6, worktype,                        \
                             cast_macro16, cast_macro2);               \
        break;                                                          \
    }                                                                   \
                                                                        \
    return  CC_OK;                                                      \
}


ICC_DEF_CVT_FUNC_2D(8u, uchar, int, CC_CAST_8U,
                     CC_8S,  char,   CC_NOP,
                     CC_16U, ushort, CC_NOP,
                     CC_16S, short,  CC_NOP,
                     CC_32S, int,    CC_NOP,
                     CC_32F, float,  cRound,
                     CC_64F, double, cRound)

ICC_DEF_CVT_FUNC_2D(8s, char, int, CC_CAST_8S,
                     CC_8U,  uchar,  CC_NOP,
                     CC_16U, ushort, CC_NOP,
                     CC_16S, short,  CC_NOP,
                     CC_32S, int,    CC_NOP,
                     CC_32F, float,  cRound,
                     CC_64F, double, cRound)

ICC_DEF_CVT_FUNC_2D(16u, ushort, int, CC_CAST_16U,
                     CC_8U,  uchar,  CC_NOP,
                     CC_8S,  char,   CC_NOP,
                     CC_16S, short,  CC_NOP,
                     CC_32S, int,    CC_NOP,
                     CC_32F, float,  cRound,
                     CC_64F, double, cRound)

ICC_DEF_CVT_FUNC_2D(16s, short, int, CC_CAST_16S,
                     CC_8U,  uchar,  CC_NOP,
                     CC_8S,  char,   CC_NOP,
                     CC_16U, ushort, CC_NOP,
                     CC_32S, int,    CC_NOP,
                     CC_32F, float,  cRound,
                     CC_64F, double, cRound)

ICC_DEF_CVT_FUNC_2D(32s, int, int, CC_NOP,
                     CC_8U,  uchar,  CC_NOP,
                     CC_8S,  char,   CC_NOP,
                     CC_16U, ushort, CC_NOP,
                     CC_16S, short,  CC_NOP,
                     CC_32F, float,  cRound,
                     CC_64F, double, cRound)

ICC_DEF_CVT_FUNC_2D(32f, float, float, CC_NOP,
                     CC_8U,  uchar,  CC_8TO32F,
                     CC_8S,  char,   CC_8TO32F,
                     CC_16U, ushort, CC_NOP,
                     CC_16S, short,  CC_NOP,
                     CC_32S, int,    CC_CAST_32F,
                     CC_64F, double, CC_CAST_32F)

ICC_DEF_CVT_FUNC_2D(64f, double, double, CC_NOP,
                     CC_8U,  uchar,  CC_8TO32F,
                     CC_8S,  char,   CC_8TO32F,
                     CC_16U, ushort, CC_NOP,
                     CC_16S, short,  CC_NOP,
                     CC_32S, int,    CC_NOP,
                     CC_32F, float,  CC_NOP)

CC_DEF_INIT_FUNC_TAB_2D(CvtTo, C1R)


typedef  CStatus (C_STDCALL *CvCvtFunc)(const void* src, int srcstep,
                                           void* dst, int dststep, CSize size,
                                           int param);

typedef  CStatus (C_STDCALL *CvCvtScaleFunc)(const void* src, int srcstep,
                                             void* dst, int dststep, CSize size,
                                             double scale, double shift,
                                             int param);

#define cvCvtScale cvConvertScale
#define cvScale  cvConvertScale
#define cvConvert(src, dst)  cvConvertScale((src), (dst), 1, 0)

CC_IMPL img_t*
cvConvertScale(const img_t* src, img_t* dst,
                double scale CC_DEFAULT(1), double shift CC_DEFAULT(0))
{
    static CvFuncTable cvt_tab, cvtscale_tab;
    static int inittab = 0;

    CC_FUNCNAME("cvConvertScale");

    __BEGIN__;

    TypeId type;
    CSize size;
    int src_step, dst_step;
    int no_scale = scale == 1 && shift == 0;

    if(no_scale && CC_ARE_TYPES_EQ(src, dst))
    {
        cvCopy(src, dst);
        EXIT;
    }

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    size = cvGetMatSize(src);
    type = CC_MAT_DEPTH(src);
    src_step = src->step;
    dst_step = dst->step;

    if(CC_IS_MAT_CONT(src) && CC_IS_MAT_CONT(dst))
    {
        size.width *= size.height;
        src_step = dst_step = CC_STUB_STEP;
        size.height = 1;
    }

    size.width *= CC_MAT_CN(src);

    if(CC_ARE_TYPES_EQ(src, dst) && size.height == 1 &&
        size.width <= CC_MAX_INLINE_MAT_OP_SIZE)
    {
        if(type == CC_32F)
        {
            const float* srcdata = (const float*)(src->tt.data);
            float* dstdata = (float*)(dst->tt.data);

            do
            {
                dstdata[size.width - 1] = (float)(srcdata[size.width-1]*scale + shift);
            }
            while(--size.width);

            EXIT;
        }

        if(type == CC_64F)
        {
            const double* srcdata = (const double*)(src->tt.data);
            double* dstdata = (double*)(dst->tt.data);

            do
            {
                dstdata[size.width - 1] = srcdata[size.width-1]*scale + shift;
            }
            while(--size.width);

            EXIT;
        }
    }

    if(!inittab)
    {
        icvInitCvtToC1RTable(cvt_tab);
        icvInitCvtScaleToC1RTable(cvtscale_tab);
        inittab = 1;
    }

    if(!CC_ARE_CNS_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(no_scale)
    {
        CvCvtFunc func = (CvCvtFunc)(cvt_tab[CC_MAT_DEPTH(dst)]);

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        IPPI_CALL(func(src->tt.data, src_step,
                   dst->tt.data, dst_step, size, type));
    }
    else
    {
        CvCvtScaleFunc func = (CvCvtScaleFunc)
            (cvtscale_tab[CC_MAT_TYPE(dst)]);

        if(!func)
            CC_ERROR(CC_StsUnsupportedFormat, "");

        IPPI_CALL(func(src->tt.data, src_step,
                   dst->tt.data, dst_step, size,
                   scale, shift, type));
    }

    __END__;
    return dst;
}


static img_t* cvConvertTo(const img_t* src, img_t* dst, TypeId dst_type, double scale CC_DEFAULT(1), double shift CC_DEFAULT(0)) {
  IMFUN_BEGIN(src==dst, src);
  cvSetMat(dst, src->h, src->w, dst_type, CC_MAT_CN(src), NULL, CC_AUTO_STEP);
  cvConvertScale(src, dst, scale, shift);
  IMFUN_END();
  return dst;
}
static img_t* cvConvertTrans(const img_t* src, img_t* dst, TypeId dst_type, double scale CC_DEFAULT(1), double shift CC_DEFAULT(0)) {
  cvConvertTo(src, dst, dst_type, scale, shift);
  cvT(dst, dst);
  return dst;
}

/********************* helper functions for converting 32f<->64f ************************/

IPCVAPI_IMPL(CStatus, icvCvt_32f64f,
    (const float* src, double* dst, int len), (src, dst, len))
{
    int i;
    for(i = 0; i <= len - 4; i += 4)
    {
        double t0 = src[i];
        double t1 = src[i+1];

        dst[i] = t0;
        dst[i+1] = t1;

        t0 = src[i+2];
        t1 = src[i+3];

        dst[i+2] = t0;
        dst[i+3] = t1;
    }

    for(; i < len; i++)
        dst[i] = src[i];

    return CC_OK;
}


IPCVAPI_IMPL(CStatus, icvCvt_64f32f,
    (const double* src, float* dst, int len), (src, dst, len))
{
    int i = 0;
    for(; i <= len - 4; i += 4)
    {
        double t0 = src[i];
        double t1 = src[i+1];

        dst[i] = (float)t0;
        dst[i+1] = (float)t1;

        t0 = src[i+2];
        t1 = src[i+3];

        dst[i+2] = (float)t0;
        dst[i+3] = (float)t1;
    }

    for(; i < len; i++)
        dst[i] = (float)src[i];

    return CC_OK;
}


static CStatus C_STDCALL icvScale_32f(const float* src, float* dst, int len, float a, float b)
{
    int i;
    for(i = 0; i <= len - 4; i += 4)
    {
        double t0 = src[i]*a + b;
        double t1 = src[i+1]*a + b;

        dst[i] = (float)t0;
        dst[i+1] = (float)t1;

        t0 = src[i+2]*a + b;
        t1 = src[i+3]*a + b;

        dst[i+2] = (float)t0;
        dst[i+3] = (float)t1;
    }

    for(; i < len; i++)
        dst[i] = (float)(src[i]*a + b);

    return CC_OK;
}


static CStatus C_STDCALL icvScale_64f(const double* src, double* dst, int len, double a, double b)
{
    int i;
    for(i = 0; i <= len - 4; i += 4)
    {
        double t0 = src[i]*a + b;
        double t1 = src[i+1]*a + b;

        dst[i] = t0;
        dst[i+1] = t1;

        t0 = src[i+2]*a + b;
        t1 = src[i+3]*a + b;

        dst[i+2] = t0;
        dst[i+3] = t1;
    }

    for(; i < len; i++)
        dst[i] = src[i]*a + b;

    return CC_OK;
}

#undef ICC_DEF_PX2PL2PX_ENTRY_C2
#undef ICC_DEF_PX2PL2PX_ENTRY_C3
#undef ICC_DEF_PX2PL2PX_ENTRY_C4
#undef ICC_DEF_PX2PL_C2
#undef ICC_DEF_PX2PL_C3
#undef ICC_DEF_PX2PL_C4
#undef ICC_DEF_PX2PL_COI
#undef ICC_DEF_COPY_PX2PL_FUNC_2D
#undef ICC_DEF_COPY_PX2PL_FUNC_2D_COI
#undef ICC_DEF_PL2PX_C2
#undef ICC_DEF_PL2PX_C3
#undef ICC_DEF_PL2PX_C4
#undef ICC_DEF_PL2PX_COI
#undef ICC_DEF_COPY_PL2PX_FUNC_2D
#undef ICC_DEF_COPY_PL2PX_FUNC_2D_COI
#undef ICC_DEF_PXPLPX_TAB
#undef ICC_DEF_PXPLCOI_TAB
#undef ICC_DEF_MIX_CH_FUNC_2D
#undef ICC_DEF_CVT_SCALE_ABS_CASE
#undef ICC_FIX_SHIFT
#undef ICC_SCALE
#undef ICC_DEF_CVT_SCALE_CASE
#undef ICC_DEF_CVT_SCALE_FUNC_INT
#undef ICC_DEF_CVT_SCALE_FUNC_FLT
#undef ICC_DEF_CVT_CASE_2D
#undef ICC_DEF_CVT_FUNC_2D


