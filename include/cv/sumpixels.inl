

/****************************************************************************************\
*                             Find sum of pixels in the ROI                              *
\****************************************************************************************/

#define ICC_SUM_COI_CASE(__op__, len, cn)                 \
    for(; x <= (len) - 4*(cn); x += 4*(cn))               \
        s0 += __op__(src[x]) + __op__(src[x+(cn)]) +        \
              __op__(src[x+(cn)*2]) + __op__(src[x+(cn)*3]);\
                                                            \
    for(; x < (len); x += (cn))                           \
        s0 += __op__(src[x]);


#define ICC_SUM_CASE_C1(__op__, len)                      \
    ICC_SUM_COI_CASE(__op__, len, 1)


#define ICC_SUM_CASE_C2(__op__, len)                      \
    for(; x <= (len) - 8; x += 8)                         \
    {                                                       \
        s0 += __op__(src[x]) + __op__(src[x+2]) +           \
              __op__(src[x+4]) + __op__(src[x+6]);          \
        s1 += __op__(src[x+1]) + __op__(src[x+3]) +         \
              __op__(src[x+5]) + __op__(src[x+7]);          \
    }                                                       \
                                                            \
    for(; x < (len); x += 2)                              \
    {                                                       \
        s0 += __op__(src[x]);                               \
        s1 += __op__(src[x+1]);                             \
    }



#define ICC_SUM_CASE_C3(__op__, len)                      \
    for(; x <= (len) - 12; x += 12)                       \
    {                                                       \
        s0 += __op__(src[x]) + __op__(src[x+3]) +           \
              __op__(src[x+6]) + __op__(src[x+9]);          \
        s1 += __op__(src[x+1]) + __op__(src[x+4]) +         \
              __op__(src[x+7]) + __op__(src[x+10]);         \
        s2 += __op__(src[x+2]) + __op__(src[x+5]) +         \
              __op__(src[x+8]) + __op__(src[x+11]);         \
    }                                                       \
                                                            \
    for(; x < (len); x += 3)                              \
    {                                                       \
        s0 += __op__(src[x]);                               \
        s1 += __op__(src[x+1]);                             \
        s2 += __op__(src[x+2]);                             \
    }


#define ICC_SUM_CASE_C4(__op__, len)                      \
    for(; x <= (len) - 16; x += 16)                       \
    {                                                       \
        s0 += __op__(src[x]) + __op__(src[x+4]) +           \
              __op__(src[x+8]) + __op__(src[x+12]);         \
        s1 += __op__(src[x+1]) + __op__(src[x+5]) +         \
              __op__(src[x+9]) + __op__(src[x+13]);         \
        s2 += __op__(src[x+2]) + __op__(src[x+6]) +         \
              __op__(src[x+10]) + __op__(src[x+14]);        \
        s3 += __op__(src[x+3]) + __op__(src[x+7]) +         \
              __op__(src[x+11]) + __op__(src[x+15]);        \
    }                                                       \
                                                            \
    for(; x < (len); x += 4)                              \
    {                                                       \
        s0 += __op__(src[x]);                               \
        s1 += __op__(src[x+1]);                             \
        s2 += __op__(src[x+2]);                             \
        s3 += __op__(src[x+3]);                             \
    }


////////////////////////////////////// entry macros //////////////////////////////////////

#define ICC_SUM_ENTRY_COMMON()          \
    step /= sizeof(src[0])

#define ICC_SUM_ENTRY_C1(sumtype)     \
    sumtype s0 = 0;                     \
    ICC_SUM_ENTRY_COMMON()

#define ICC_SUM_ENTRY_C2(sumtype)     \
    sumtype s0 = 0, s1 = 0;             \
    ICC_SUM_ENTRY_COMMON()

#define ICC_SUM_ENTRY_C3(sumtype)     \
    sumtype s0 = 0, s1 = 0, s2 = 0;     \
    ICC_SUM_ENTRY_COMMON()

#define ICC_SUM_ENTRY_C4(sumtype)         \
    sumtype s0 = 0, s1 = 0, s2 = 0, s3 = 0; \
    ICC_SUM_ENTRY_COMMON()


#define ICC_SUM_ENTRY_BLOCK_COMMON(block_size)    \
    int remaining = block_size;                     \
    ICC_SUM_ENTRY_COMMON()

#define ICC_SUM_ENTRY_BLOCK_C1(sumtype, worktype, block_size) \
    sumtype sum0 = 0;                                           \
    worktype s0 = 0;                                            \
    ICC_SUM_ENTRY_BLOCK_COMMON(block_size)

#define ICC_SUM_ENTRY_BLOCK_C2(sumtype, worktype, block_size) \
    sumtype sum0 = 0, sum1 = 0;                                 \
    worktype s0 = 0, s1 = 0;                                    \
    ICC_SUM_ENTRY_BLOCK_COMMON(block_size)

#define ICC_SUM_ENTRY_BLOCK_C3(sumtype, worktype, block_size) \
    sumtype sum0 = 0, sum1 = 0, sum2 = 0;                       \
    worktype s0 = 0, s1 = 0, s2 = 0;                            \
    ICC_SUM_ENTRY_BLOCK_COMMON(block_size)

#define ICC_SUM_ENTRY_BLOCK_C4(sumtype, worktype, block_size) \
    sumtype sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;             \
    worktype s0 = 0, s1 = 0, s2 = 0, s3 = 0;                    \
    ICC_SUM_ENTRY_BLOCK_COMMON(block_size)


/////////////////////////////////////// exit macros //////////////////////////////////////

#define ICC_SUM_EXIT_C1(tmp, sumtype)     \
    sum[0] = (sumtype)tmp##0

#define ICC_SUM_EXIT_C2(tmp, sumtype)     \
    sum[0] = (sumtype)tmp##0;               \
    sum[1] = (sumtype)tmp##1;

#define ICC_SUM_EXIT_C3(tmp, sumtype)     \
    sum[0] = (sumtype)tmp##0;               \
    sum[1] = (sumtype)tmp##1;               \
    sum[2] = (sumtype)tmp##2;

#define ICC_SUM_EXIT_C4(tmp, sumtype)     \
    sum[0] = (sumtype)tmp##0;               \
    sum[1] = (sumtype)tmp##1;               \
    sum[2] = (sumtype)tmp##2;               \
    sum[3] = (sumtype)tmp##3;

#define ICC_SUM_EXIT_BLOCK_C1(sumtype)    \
    sum0 += s0;                             \
    ICC_SUM_EXIT_C1(sum, sumtype)

#define ICC_SUM_EXIT_BLOCK_C2(sumtype)    \
    sum0 += s0; sum1 += s1;                 \
    ICC_SUM_EXIT_C2(sum, sumtype)

#define ICC_SUM_EXIT_BLOCK_C3(sumtype)    \
    sum0 += s0; sum1 += s1;                 \
    sum2 += s2;                             \
    ICC_SUM_EXIT_C3(sum, sumtype)

#define ICC_SUM_EXIT_BLOCK_C4(sumtype)    \
    sum0 += s0; sum1 += s1;                 \
    sum2 += s2; sum3 += s3;                 \
    ICC_SUM_EXIT_C4(sum, sumtype)

////////////////////////////////////// update macros /////////////////////////////////////

#define ICC_SUM_UPDATE_COMMON(block_size) \
    remaining = block_size

#define ICC_SUM_UPDATE_C1(block_size)     \
    ICC_SUM_UPDATE_COMMON(block_size);    \
    sum0 += s0;                             \
    s0 = 0

#define ICC_SUM_UPDATE_C2(block_size)     \
    ICC_SUM_UPDATE_COMMON(block_size);    \
    sum0 += s0; sum1 += s1;                 \
    s0 = s1 = 0

#define ICC_SUM_UPDATE_C3(block_size)     \
    ICC_SUM_UPDATE_COMMON(block_size);    \
    sum0 += s0; sum1 += s1; sum2 += s2;     \
    s0 = s1 = s2 = 0

#define ICC_SUM_UPDATE_C4(block_size)     \
    ICC_SUM_UPDATE_COMMON(block_size);    \
    sum0 += s0; sum1 += s1;                 \
    sum2 += s2; sum3 += s3;                 \
    s0 = s1 = s2 = s3 = 0


#define ICC_DEF_SUM_NOHINT_BLOCK_FUNC_2D(name, flavor, cn,     \
    __op__, arrtype, sumtype_final, sumtype, worktype, block_size)\
IPCVAPI_IMPL(CStatus, icv##name##_##flavor##_C##cn##R,(   \
    const arrtype* src, int step, CSize size,                  \
    sumtype_final* sum), (src, step, size, sum))              \
{                                                               \
    ICC_SUM_ENTRY_BLOCK_C##cn(sumtype,worktype,(block_size)*(cn)); \
    size.width *= cn;                                           \
                                                                \
    for(; size.height--; src += step)                         \
    {                                                           \
        int x = 0;                                              \
        while(x < size.width)                                 \
        {                                                       \
            int limit = MIN(remaining, size.width - x);       \
            remaining -= limit;                                 \
            limit += x;                                         \
            ICC_SUM_CASE_C##cn(__op__, limit);                \
            if(remaining == 0)                                \
            {                                                   \
                ICC_SUM_UPDATE_C##cn((block_size)*(cn));      \
            }                                                   \
        }                                                       \
    }                                                           \
                                                                \
    ICC_SUM_EXIT_BLOCK_C##cn(sumtype_final);                  \
    return CC_OK;                                               \
}


#define ICC_DEF_SUM_NOHINT_FUNC_2D(name, flavor, cn,           \
    __op__, arrtype, sumtype_final, sumtype, worktype, block_size)\
IPCVAPI_IMPL(CStatus, icv##name##_##flavor##_C##cn##R,(   \
    const arrtype* src, int step, CSize size,                  \
    sumtype_final* sum), (src, step, size, sum))              \
{                                                               \
    ICC_SUM_ENTRY_C##cn(sumtype);                             \
    size.width *= cn;                                           \
                                                                \
    for(; size.height--; src += step)                         \
    {                                                           \
        int x = 0;                                              \
        ICC_SUM_CASE_C##cn(__op__, size.width);               \
    }                                                           \
                                                                \
    ICC_SUM_EXIT_C##cn(s, sumtype_final);                     \
    return CC_OK;                                               \
}


#define ICC_DEF_SUM_HINT_FUNC_2D(name, flavor, cn,             \
    __op__, arrtype, sumtype_final, sumtype, worktype, block_size)\
IPCVAPI_IMPL(CStatus, icv##name##_##flavor##_C##cn##R,(   \
    const arrtype* src, int step, CSize size,                  \
    sumtype_final* sum, CvHintAlgorithm hint),             \
    (src, step, size, sum, cvAlgHintAccurate))                 \
{                                                               \
    ICC_SUM_ENTRY_C##cn(sumtype);                             \
    size.width *= cn;                                           \
                                                                \
    for(; size.height--; src += step)                         \
    {                                                           \
        int x = 0;                                              \
        ICC_SUM_CASE_C##cn(__op__, size.width);               \
    }                                                           \
                                                                \
    ICC_SUM_EXIT_C##cn(s, sumtype_final);                     \
    return CC_OK;                                               \
}


#define ICC_DEF_SUM_NOHINT_BLOCK_FUNC_2D_COI(name, flavor,     \
    __op__, arrtype, sumtype_final, sumtype, worktype, block_size)\
static CStatus C_STDCALL icv##name##_##flavor##_CnCR(\
    const arrtype* src, int step, CSize size, int cn,          \
    int coi, sumtype_final* sum)                               \
{                                                               \
    ICC_SUM_ENTRY_BLOCK_C1(sumtype,worktype,(block_size)*(cn)); \
    size.width *= cn;                                           \
    src += coi - 1;                                             \
                                                                \
    for(; size.height--; src += step)                         \
    {                                                           \
        int x = 0;                                              \
        while(x < size.width)                                 \
        {                                                       \
            int limit = MIN(remaining, size.width - x);       \
            remaining -= limit;                                 \
            limit += x;                                         \
            ICC_SUM_COI_CASE(__op__, limit, cn);              \
            if(remaining == 0)                                \
            {                                                   \
                ICC_SUM_UPDATE_C1((block_size)*(cn));         \
            }                                                   \
        }                                                       \
    }                                                           \
                                                                \
    ICC_SUM_EXIT_BLOCK_C1(sumtype_final);                     \
    return CC_OK;                                               \
}


#define ICC_DEF_SUM_NOHINT_FUNC_2D_COI(name, flavor,           \
    __op__, arrtype, sumtype_final, sumtype, worktype, block_size)\
static CStatus C_STDCALL icv##name##_##flavor##_CnCR(\
    const arrtype* src, int step, CSize size, int cn,          \
    int coi, sumtype_final* sum)                               \
{                                                               \
    ICC_SUM_ENTRY_C1(sumtype);                                \
    size.width *= cn;                                           \
    src += coi - 1;                                             \
                                                                \
    for(; size.height--; src += step)                         \
    {                                                           \
        int x = 0;                                              \
        ICC_SUM_COI_CASE(__op__, size.width, cn);             \
    }                                                           \
                                                                \
    ICC_SUM_EXIT_C1(s, sumtype_final);                        \
    return CC_OK;                                               \
}


#define ICC_DEF_SUM_ALL(name, flavor, __op__, arrtype, sumtype_final, sumtype, \
                         worktype, hintp_type, nohint_type, block_size)        \
    ICC_DEF_SUM_##hintp_type##_FUNC_2D(name, flavor, 1, __op__, arrtype,       \
                         sumtype_final, sumtype, worktype, block_size)         \
    ICC_DEF_SUM_##hintp_type##_FUNC_2D(name, flavor, 2, __op__, arrtype,       \
                         sumtype_final, sumtype, worktype, block_size)         \
    ICC_DEF_SUM_##hintp_type##_FUNC_2D(name, flavor, 3, __op__, arrtype,       \
                         sumtype_final, sumtype, worktype, block_size)         \
    ICC_DEF_SUM_##hintp_type##_FUNC_2D(name, flavor, 4, __op__, arrtype,       \
                         sumtype_final, sumtype, worktype, block_size)         \
    ICC_DEF_SUM_##nohint_type##_FUNC_2D_COI(name, flavor, __op__, arrtype,     \
                         sumtype_final, sumtype, worktype, block_size)

ICC_DEF_SUM_ALL(Sum, 8u, CC_NOP, uchar, double, int64, unsigned,
                 NOHINT_BLOCK, NOHINT_BLOCK, 1 << 24)
ICC_DEF_SUM_ALL(Sum, 16u, CC_NOP, ushort, double, int64, unsigned,
                 NOHINT_BLOCK, NOHINT_BLOCK, 1 << 16)
ICC_DEF_SUM_ALL(Sum, 16s, CC_NOP, short, double, int64, int,
                 NOHINT_BLOCK, NOHINT_BLOCK, 1 << 16)
ICC_DEF_SUM_ALL(Sum, 32s, CC_NOP, int, double, double, double, NOHINT, NOHINT, 0)
ICC_DEF_SUM_ALL(Sum, 32f, CC_NOP, float, double, double, double, HINT, NOHINT, 0)
ICC_DEF_SUM_ALL(Sum, 64f, CC_NOP, double, double, double, double, NOHINT, NOHINT, 0)

#define icvSum_8s_C1R   0
#define icvSum_8s_C2R   0
#define icvSum_8s_C3R   0
#define icvSum_8s_C4R   0
#define icvSum_8s_CnCR  0

CC_DEF_INIT_BIG_FUNC_TAB_2D(Sum, R)
CC_DEF_INIT_FUNC_TAB_2D(Sum, CnCR)

CC_IMPL CScalar
cvSum(const img_t* mat)
{
    static CvBigFuncTable sum_tab;
    static CvFuncTable sumcoi_tab;
    static int inittab = 0;

    CScalar sum = {{0,0,0,0}};

    CC_FUNCNAME("cvSum");

    __BEGIN__;

    TypeId type;
    int coi = 0;
    int mat_step;
    CSize size;
    int cn = CC_MAT_CN(mat);

    if(!inittab)
    {
        icvInitSumRTable(sum_tab);
        icvInitSumCnCRTable(sumcoi_tab);
        inittab = 1;
    }

    type = CC_MAT_DEPTH(mat);
    size = cvGetMatSize(mat);

    mat_step = mat->step;

    if(CC_IS_MAT_CONT(mat))
    {
        size.width *= size.height;
        
        if(size.width <= CC_MAX_INLINE_MAT_OP_SIZE)
        {
            if(type == CC_32F && cn==1)
            {
                float* data = mat->tt.fl;

                do
                {
                    sum.val[0] += data[size.width - 1];
                }
                while(--size.width);

                EXIT;
            }

            if(type == CC_64F && 1==cn)
            {
                double* data = mat->tt.db;

                do
                {
                    sum.val[0] += data[size.width - 1];
                }
                while(--size.width);

                EXIT;
            }
        }
        size.height = 1;
        mat_step = CC_STUB_STEP;
    }

    if(CC_MAT_CN(mat) == 1 || coi == 0)
    {
        int pass_hint = type == CC_32F;

        if(CC_MAT_CN(mat) > 4)
            CC_ERROR(CC_StsOutOfRange, "The input array must have at most 4 channels");

        if(!pass_hint)
        {
            CvFunc2D_1A1P func = (CvFunc2D_1A1P)(sum_tab[type][cn]);

            if(!func)
                CC_ERROR(CC_StsBadArg, cvUnsupportedFormat);

            IPPI_CALL(func(mat->tt.data, mat_step, size, sum.val));
        }
        else
        {
            CvFunc2D_1A1P1I func = (CvFunc2D_1A1P1I)(sum_tab[type][cn]);

            if(!func)
                CC_ERROR(CC_StsBadArg, cvUnsupportedFormat);

            IPPI_CALL(func(mat->tt.data, mat_step, size, sum.val, cvAlgHintAccurate));
        }
    }
    else
    {
        CvFunc2DnC_1A1P func = (CvFunc2DnC_1A1P)(sumcoi_tab[type]);

        if(!func)
            CC_ERROR(CC_StsBadArg, cvUnsupportedFormat);

        IPPI_CALL(func(mat->tt.data, mat_step, size,
                         CC_MAT_CN(mat), coi, sum.val));
    }

    __END__;

    return  sum;
}

CC_IMPL double
cvSum_C1(const img_t* mat) {
  return cvSum(mat).val[0];
}

#define ICC_DEF_NONZERO_ALL(flavor, __op__, arrtype)              \
    ICC_DEF_SUM_NOHINT_FUNC_2D(CountNonZero, flavor, 1, __op__,    \
                                arrtype, int, int, int, 0)         \
    ICC_DEF_SUM_NOHINT_FUNC_2D_COI(CountNonZero, flavor, __op__,   \
                                    arrtype, int, int, int, 0)

#undef  CC_NONZERO_DBL
#define CC_NONZERO_DBL(x) (((x) & CC_BIG_INT(0x7fffffffffffffff)) != 0)

ICC_DEF_NONZERO_ALL(8u, CC_NONZERO, uchar)
ICC_DEF_NONZERO_ALL(16s, CC_NONZERO, ushort)
ICC_DEF_NONZERO_ALL(32s, CC_NONZERO, int)
ICC_DEF_NONZERO_ALL(32f, CC_NONZERO_FLT, int)
ICC_DEF_NONZERO_ALL(64f, CC_NONZERO_DBL, int64)

#define icvCountNonZero_8s_C1R icvCountNonZero_8u_C1R
#define icvCountNonZero_8s_CnCR icvCountNonZero_8u_CnCR
#define icvCountNonZero_16u_C1R icvCountNonZero_16s_C1R
#define icvCountNonZero_16u_CnCR icvCountNonZero_16s_CnCR

CC_DEF_INIT_FUNC_TAB_2D(CountNonZero, C1R)
CC_DEF_INIT_FUNC_TAB_2D(CountNonZero, CnCR)

CC_IMPL int
cvCountNonZero(const img_t* mat)
{
    static CvFuncTable nz_tab;
    static CvFuncTable nzcoi_tab;
    static int inittab = 0;

    int count = 0;

    CC_FUNCNAME("cvCountNonZero");

    __BEGIN__;

    TypeId type;
    int coi = 0;
    int mat_step;
    CSize size;

    if(!inittab)
    {
        icvInitCountNonZeroC1RTable(nz_tab);
        icvInitCountNonZeroCnCRTable(nzcoi_tab);
        inittab = 1;
    }

    type = CC_MAT_DEPTH(mat);
    size = cvGetMatSize(mat);

    mat_step = mat->step;

    if(CC_IS_MAT_CONT(mat))
    {
        size.width *= size.height;
        size.height = 1;
        mat_step = CC_STUB_STEP;
    }

    if(CC_MAT_CN(mat) == 1 || coi == 0)
    {
        CvFunc2D_1A1P func = (CvFunc2D_1A1P)(nz_tab[type]);

        if(CC_MAT_CN(mat) != 1)
            CC_ERROR(CC_BadNumChannels,
            "The function can handle only a single channel at a time (use COI)");

        if(!func)
            CC_ERROR(CC_StsBadArg, cvUnsupportedFormat);

        IPPI_CALL(func(mat->tt.data, mat_step, size, &count));
    }
    else
    {
        CvFunc2DnC_1A1P func = (CvFunc2DnC_1A1P)(nzcoi_tab[type]);

        if(!func)
            CC_ERROR(CC_StsBadArg, cvUnsupportedFormat);

        IPPI_CALL(func(mat->tt.data, mat_step, size, CC_MAT_CN(mat), coi, &count));
    }

    __END__;

    return  count;
}


/****************************************************************************************\
*                                Reduce Matrix to Vector                                 *
\****************************************************************************************/

#define ICC_ACC_ROWS_FUNC(name, flavor, arrtype, acctype,      \
                           __op__, load_macro)                 \
static CStatus C_STDCALL                                      \
icv##name##Rows_##flavor##_C1R(const arrtype* src, int srcstep,\
                           acctype* dst, CSize size)          \
{                                                               \
    int i, width = size.width;                                  \
    srcstep /= sizeof(src[0]);                                  \
                                                                \
    for(i = 0; i < width; i++)                                \
        dst[i] = load_macro(src[i]);                            \
                                                                \
    for(; --size.height;)                                    \
    {                                                           \
        src += srcstep;                                         \
        for(i = 0; i <= width - 4; i += 4)                    \
        {                                                       \
            acctype s0 = load_macro(src[i]);                    \
            acctype s1 = load_macro(src[i+1]);                  \
            acctype a0 = dst[i], a1 = dst[i+1];                 \
            a0 = (acctype)__op__(a0,s0); a1 = (acctype)__op__(a1,s1); \
            dst[i] = a0; dst[i+1] = a1;                         \
                                                                \
            s0 = load_macro(src[i+2]);                          \
            s1 = load_macro(src[i+3]);                          \
            a0 = dst[i+2]; a1 = dst[i+3];                       \
            a0 = (acctype)__op__(a0,s0); a1 = (acctype)__op__(a1,s1);  \
            dst[i+2] = a0; dst[i+3] = a1;                       \
        }                                                       \
                                                                \
        for(; i < width; i++)                                 \
        {                                                       \
            acctype s0 = load_macro(src[i]), a0 = dst[i];       \
            a0 = (acctype)__op__(a0,s0);                        \
            dst[i] = a0;                                        \
        }                                                       \
    }                                                           \
                                                                \
    return CC_OK;                                               \
}


#define ICC_ACC_COLS_FUNC_C1(name, flavor, arrtype, worktype, acctype, __op__)\
static CStatus C_STDCALL                                              \
icv##name##Cols_##flavor##_C1R(const arrtype* src, int srcstep,        \
                                acctype* dst, int dststep, CSize size)\
{                                                                       \
    int i, width = size.width;                                          \
    srcstep /= sizeof(src[0]);                                          \
    dststep /= sizeof(dst[0]);                                          \
                                                                        \
    for(; size.height--; src += srcstep, dst += dststep)              \
    {                                                                   \
        if(width == 1)                                                \
            dst[0] = (acctype)src[0];                                   \
        else                                                            \
        {                                                               \
            worktype a0 = src[0], a1 = src[1];                          \
            for(i = 2; i <= width - 4; i += 4)                        \
            {                                                           \
                worktype s0 = src[i], s1 = src[i+1];                    \
                a0 = __op__(a0, s0);                                    \
                a1 = __op__(a1, s1);                                    \
                s0 = src[i+2]; s1 = src[i+3];                           \
                a0 = __op__(a0, s0);                                    \
                a1 = __op__(a1, s1);                                    \
            }                                                           \
                                                                        \
            for(; i < width; i++)                                     \
            {                                                           \
                worktype s0 = src[i];                                   \
                a0 = __op__(a0, s0);                                    \
            }                                                           \
            a0 = __op__(a0, a1);                                        \
            dst[0] = (acctype)a0;                                       \
        }                                                               \
    }                                                                   \
                                                                        \
    return CC_OK;                                                       \
}


#define ICC_ACC_COLS_FUNC_C3(name, flavor, arrtype, worktype, acctype, __op__) \
static CStatus C_STDCALL                                              \
icv##name##Cols_##flavor##_C3R(const arrtype* src, int srcstep,        \
                                acctype* dst, int dststep, CSize size)\
{                                                                       \
    int i, width = size.width*3;                                        \
    srcstep /= sizeof(src[0]);                                          \
    dststep /= sizeof(dst[0]);                                          \
                                                                        \
    for(; size.height--; src += srcstep, dst += dststep)              \
    {                                                                   \
        worktype a0 = src[0], a1 = src[1], a2 = src[2];                 \
        for(i = 3; i < width; i += 3)                                 \
        {                                                               \
            worktype s0 = src[i], s1 = src[i+1], s2 = src[i+2];         \
            a0 = __op__(a0, s0);                                        \
            a1 = __op__(a1, s1);                                        \
            a2 = __op__(a2, s2);                                        \
        }                                                               \
                                                                        \
        dst[0] = (acctype)a0;                                           \
        dst[1] = (acctype)a1;                                           \
        dst[2] = (acctype)a2;                                           \
    }                                                                   \
                                                                        \
    return CC_OK;                                                       \
}


#define ICC_ACC_COLS_FUNC_C4(name, flavor, arrtype, worktype, acctype, __op__) \
static CStatus C_STDCALL                                              \
icv##name##Cols_##flavor##_C4R(const arrtype* src, int srcstep,        \
                                acctype* dst, int dststep, CSize size)\
{                                                                       \
    int i, width = size.width*4;                                        \
    srcstep /= sizeof(src[0]);                                          \
    dststep /= sizeof(dst[0]);                                          \
                                                                        \
    for(; size.height--; src += srcstep, dst += dststep)              \
    {                                                                   \
        worktype a0 = src[0], a1 = src[1], a2 = src[2], a3 = src[3];    \
        for(i = 4; i < width; i += 4)                                 \
        {                                                               \
            worktype s0 = src[i], s1 = src[i+1];                        \
            a0 = __op__(a0, s0);                                        \
            a1 = __op__(a1, s1);                                        \
            s0 = src[i+2]; s1 = src[i+3];                               \
            a2 = __op__(a2, s0);                                        \
            a3 = __op__(a3, s1);                                        \
        }                                                               \
                                                                        \
        dst[0] = (acctype)a0;                                           \
        dst[1] = (acctype)a1;                                           \
        dst[2] = (acctype)a2;                                           \
        dst[3] = (acctype)a3;                                           \
    }                                                                   \
                                                                        \
    return CC_OK;                                                       \
}


ICC_ACC_ROWS_FUNC(Sum, 8u32s, uchar, int, CC_ADD, CC_NOP)
ICC_ACC_ROWS_FUNC(Sum, 8u32f, uchar, float, CC_ADD, CC_8TO32F)
ICC_ACC_ROWS_FUNC(Sum, 16u32f, ushort, float, CC_ADD, CC_NOP)
ICC_ACC_ROWS_FUNC(Sum, 16u64f, ushort, double, CC_ADD, CC_NOP)
ICC_ACC_ROWS_FUNC(Sum, 16s32f, short, float, CC_ADD, CC_NOP)
ICC_ACC_ROWS_FUNC(Sum, 16s64f, short, double, CC_ADD, CC_NOP)
ICC_ACC_ROWS_FUNC(Sum, 32f, float, float, CC_ADD, CC_NOP)
ICC_ACC_ROWS_FUNC(Sum, 32f64f, float, double, CC_ADD, CC_NOP)
ICC_ACC_ROWS_FUNC(Sum, 64f, double, double, CC_ADD, CC_NOP)

ICC_ACC_ROWS_FUNC(Max, 8u, uchar, uchar, CC_MAX_8U, CC_NOP)
ICC_ACC_ROWS_FUNC(Max, 32f, float, float, MAX, CC_NOP)
ICC_ACC_ROWS_FUNC(Max, 64f, double, double, MAX, CC_NOP)

ICC_ACC_ROWS_FUNC(Min, 8u, uchar, uchar, CC_MIN_8U, CC_NOP)
ICC_ACC_ROWS_FUNC(Min, 32f, float, float, MIN, CC_NOP)
ICC_ACC_ROWS_FUNC(Min, 64f, double, double, MIN, CC_NOP)

ICC_ACC_COLS_FUNC_C1(Sum, 8u32s, uchar, int, int, CC_ADD)
ICC_ACC_COLS_FUNC_C1(Sum, 8u32f, uchar, int, float, CC_ADD)
ICC_ACC_COLS_FUNC_C1(Sum, 16u32f, ushort, float, float, CC_ADD)
ICC_ACC_COLS_FUNC_C1(Sum, 16u64f, ushort, double, double, CC_ADD)
ICC_ACC_COLS_FUNC_C1(Sum, 16s32f, short, float, float, CC_ADD)
ICC_ACC_COLS_FUNC_C1(Sum, 16s64f, short, double, double, CC_ADD)

ICC_ACC_COLS_FUNC_C1(Sum, 32f, float, float, float, CC_ADD)
ICC_ACC_COLS_FUNC_C1(Sum, 32f64f, float, double, double, CC_ADD)
ICC_ACC_COLS_FUNC_C1(Sum, 64f, double, double, double, CC_ADD)
ICC_ACC_COLS_FUNC_C3(Sum, 8u32s, uchar, int, int, CC_ADD)
ICC_ACC_COLS_FUNC_C3(Sum, 8u32f, uchar, int, float, CC_ADD)
ICC_ACC_COLS_FUNC_C3(Sum, 32f, float, float, float, CC_ADD)
ICC_ACC_COLS_FUNC_C3(Sum, 64f, double, double, double, CC_ADD)
ICC_ACC_COLS_FUNC_C4(Sum, 8u32s, uchar, int, int, CC_ADD)
ICC_ACC_COLS_FUNC_C4(Sum, 8u32f, uchar, int, float, CC_ADD)
ICC_ACC_COLS_FUNC_C4(Sum, 32f, float, float, float, CC_ADD)
ICC_ACC_COLS_FUNC_C4(Sum, 64f, double, double, double, CC_ADD)

ICC_ACC_COLS_FUNC_C1(Max, 8u, uchar, int, uchar, CC_MAX_8U)
ICC_ACC_COLS_FUNC_C1(Max, 32f, float, float, float, MAX)
ICC_ACC_COLS_FUNC_C1(Max, 64f, double, double, double, MAX)

ICC_ACC_COLS_FUNC_C1(Min, 8u, uchar, int, uchar, CC_MIN_8U)
ICC_ACC_COLS_FUNC_C1(Min, 32f, float, float, float, MIN)
ICC_ACC_COLS_FUNC_C1(Min, 64f, double, double, double, MIN)

typedef CStatus (C_STDCALL * CvReduceToRowFunc)
    (const void* src, int srcstep, void* dst, CSize size);

typedef CStatus (C_STDCALL * CvReduceToColFunc)
    (const void* src, int srcstep, void* dst, int dststep, CSize size);


CC_IMPL void
cvReduce(const img_t* src, img_t* dst, int dim, int op)
{
    img_t temp[1] = {0};
    
    CC_FUNCNAME("cvReduce");

    __BEGIN__;

    TypeId stype, dtype;
    int cn, op0 = op;
    CSize size;
    img_t* dst0 = dst;

    if(!CC_ARE_CNS_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "Input and output arrays must have the same number of channels");

    stype = CC_MAT_DEPTH(src);
    dtype = CC_MAT_DEPTH(dst);
    cn = CC_MAT_CN(src);

    size = cvGetMatSize(src);

    if(dim < 0)
        dim = src->rows > dst->rows ? 0 : src->cols > dst->cols ? 1 : dst->cols == 1;

    if(dim > 1)
        CC_ERROR(CC_StsOutOfRange, "The reduced dimensionality index is out of range");

    if(dim == 0 && (dst->cols != src->cols || dst->rows != 1) ||
        dim == 1 && (dst->rows != src->rows || dst->cols != 1))
        CC_ERROR(CC_StsBadSize, "The output array size is incorrect");

    if(op == CC_REDUCE_AVG)
    {
        TypeId ttype = stype == CC_8U ? CC_32S : dst->tid;
        if(ttype != dst->tid)
            CC_CALL(dst = cvCreateMat(temp, dst->rows, dst->cols, ttype, cn));
        op = CC_REDUCE_SUM;
        dtype = ttype;
    }

    if(op != CC_REDUCE_SUM && op != CC_REDUCE_MAX && op != CC_REDUCE_MIN)
        CC_ERROR(CC_StsBadArg, "Unknown reduce operation index, must be one of CC_REDUCE_*");

    if(dim == 0)
    {
        CvReduceToRowFunc rfunc =
            op == CC_REDUCE_SUM ?
            (stype == CC_8U && dtype == CC_32S ? (CvReduceToRowFunc)icvSumRows_8u32s_C1R :
             stype == CC_8U && dtype == CC_32F ? (CvReduceToRowFunc)icvSumRows_8u32f_C1R :
             stype == CC_16U && dtype == CC_32F ? (CvReduceToRowFunc)icvSumRows_16u32f_C1R :
             stype == CC_16U && dtype == CC_64F ? (CvReduceToRowFunc)icvSumRows_16u64f_C1R :
             stype == CC_16S && dtype == CC_32F ? (CvReduceToRowFunc)icvSumRows_16s32f_C1R :
             stype == CC_16S && dtype == CC_64F ? (CvReduceToRowFunc)icvSumRows_16s64f_C1R :
             stype == CC_32F && dtype == CC_32F ? (CvReduceToRowFunc)icvSumRows_32f_C1R :
             stype == CC_32F && dtype == CC_64F ? (CvReduceToRowFunc)icvSumRows_32f64f_C1R :        
             stype == CC_64F && dtype == CC_64F ? (CvReduceToRowFunc)icvSumRows_64f_C1R : 0) :
            op == CC_REDUCE_MAX ?
            (stype == CC_8U && dtype == CC_8U ? (CvReduceToRowFunc)icvMaxRows_8u_C1R :
             stype == CC_32F && dtype == CC_32F ? (CvReduceToRowFunc)icvMaxRows_32f_C1R :
             stype == CC_64F && dtype == CC_64F ? (CvReduceToRowFunc)icvMaxRows_64f_C1R : 0) :

            (stype == CC_8U && dtype == CC_8U ? (CvReduceToRowFunc)icvMinRows_8u_C1R :
             stype == CC_32F && dtype == CC_32F ? (CvReduceToRowFunc)icvMinRows_32f_C1R :
             stype == CC_64F && dtype == CC_64F ? (CvReduceToRowFunc)icvMinRows_64f_C1R : 0);

        if(!rfunc)
            CC_ERROR(CC_StsUnsupportedFormat,
            "Unsupported combination of input and output array formats");

        size.width *= cn;
        IPPI_CALL(rfunc(src->tt.data, src->step ? src->step : CC_STUB_STEP,
                          dst->tt.data, size));
    }
    else
    {
        CvReduceToColFunc cfunc =
            op == CC_REDUCE_SUM ?
            (stype == CC_8U && dtype == CC_32S ?
            (CvReduceToColFunc)(cn == 1 ? icvSumCols_8u32s_C1R :
                                cn == 3 ? icvSumCols_8u32s_C3R :
                                cn == 4 ? icvSumCols_8u32s_C4R : 0) :
             stype == CC_8U && dtype == CC_32F ?
            (CvReduceToColFunc)(cn == 1 ? icvSumCols_8u32f_C1R :
                                cn == 3 ? icvSumCols_8u32f_C3R :
                                cn == 4 ? icvSumCols_8u32f_C4R : 0) :
             stype == CC_16U && dtype == CC_32F ?
            (CvReduceToColFunc)(cn == 1 ? icvSumCols_16u32f_C1R : 0) :
             stype == CC_16U && dtype == CC_64F ?
            (CvReduceToColFunc)(cn == 1 ? icvSumCols_16u64f_C1R : 0) :
             stype == CC_16S && dtype == CC_32F ?
            (CvReduceToColFunc)(cn == 1 ? icvSumCols_16s32f_C1R : 0) :
             stype == CC_16S && dtype == CC_64F ?
            (CvReduceToColFunc)(cn == 1 ? icvSumCols_16s64f_C1R : 0) :
             stype == CC_32F && dtype == CC_32F ?
            (CvReduceToColFunc)(cn == 1 ? icvSumCols_32f_C1R :
                                cn == 3 ? icvSumCols_32f_C3R :
                                cn == 4 ? icvSumCols_32f_C4R : 0) :
             stype == CC_32F && dtype == CC_64F ?
            (CvReduceToColFunc)(cn == 1 ? icvSumCols_32f64f_C1R : 0) :
             stype == CC_64F && dtype == CC_64F ?
            (CvReduceToColFunc)(cn == 1 ? icvSumCols_64f_C1R :
                                cn == 3 ? icvSumCols_64f_C3R :
                                cn == 4 ? icvSumCols_64f_C4R : 0) : 0) :
             op == CC_REDUCE_MAX && cn == 1 ?
             (stype == CC_8U && dtype == CC_8U ? (CvReduceToColFunc)icvMaxCols_8u_C1R :
              stype == CC_32F && dtype == CC_32F ? (CvReduceToColFunc)icvMaxCols_32f_C1R :
              stype == CC_64F && dtype == CC_64F ? (CvReduceToColFunc)icvMaxCols_64f_C1R : 0) :
             op == CC_REDUCE_MIN && cn == 1 ?
             (stype == CC_8U && dtype == CC_8U ? (CvReduceToColFunc)icvMinCols_8u_C1R :
              stype == CC_32F && dtype == CC_32F ? (CvReduceToColFunc)icvMinCols_32f_C1R :
              stype == CC_64F && dtype == CC_64F ? (CvReduceToColFunc)icvMinCols_64f_C1R : 0) : 0;

        if(!cfunc)
            CC_ERROR(CC_StsUnsupportedFormat,
            "Unsupported combination of input and output array formats");

        IPPI_CALL(cfunc(src->tt.data, src->step ? src->step : CC_STUB_STEP,
                          dst->tt.data, dst->step ? dst->step : CC_STUB_STEP, size));
    }

    if(op0 == CC_REDUCE_AVG)
        cvScale(dst, dst0, 1./(dim == 0 ? src->rows : src->cols), 0);

    __END__;

    imfree(temp);
}



#define ICC_DEF_INTEGRAL_OP_C1(flavor, arrtype, sumtype, sqsumtype, worktype,  \
                                cast_macro, cast_sqr_macro)    \
static CStatus C_STDCALL                                      \
icvIntegralImage_##flavor##_C1R(const arrtype* src, int srcstep,\
                                 sumtype* sum, int sumstep,     \
                                 sqsumtype* sqsum, int sqsumstep,\
                                 sumtype* tilted, int tiltedstep,\
                                 CSize size)                  \
{                                                               \
    int x, y;                                                   \
    sumtype s;                                                  \
    sqsumtype sq;                                               \
    sumtype* buf = 0;                                           \
                                                                \
    srcstep /= sizeof(src[0]);                                  \
                                                                \
    memset(sum, 0, (size.width+1)*sizeof(sum[0]));             \
    sumstep /= sizeof(sum[0]);                                  \
    sum += sumstep + 1;                                         \
                                                                \
    if(sqsum)                                                 \
    {                                                           \
        memset(sqsum, 0, (size.width+1)*sizeof(sqsum[0]));     \
        sqsumstep /= sizeof(sqsum[0]);                          \
        sqsum += sqsumstep + 1;                                 \
    }                                                           \
                                                                \
    if(tilted)                                                \
    {                                                           \
        memset(tilted, 0, (size.width+1)*sizeof(tilted[0]));   \
        tiltedstep /= sizeof(tilted[0]);                        \
        tilted += tiltedstep + 1;                               \
    }                                                           \
                                                                \
    if(sqsum == 0 && tilted == 0)                             \
    {                                                           \
        for(y = 0; y < size.height; y++, src += srcstep,       \
                                          sum += sumstep)      \
        {                                                       \
            sum[-1] = 0;                                        \
            for(x = 0, s = 0; x < size.width; x++)            \
            {                                                   \
                sumtype t = cast_macro(src[x]);                 \
                s += t;                                         \
                sum[x] = sum[x - sumstep] + s;                  \
            }                                                   \
        }                                                       \
    }                                                           \
    else if(tilted == 0)                                      \
    {                                                           \
        for(y = 0; y < size.height; y++, src += srcstep,       \
                        sum += sumstep, sqsum += sqsumstep)    \
        {                                                       \
            sum[-1] = 0;                                        \
            sqsum[-1] = 0;                                      \
                                                                \
            for(x = 0, s = 0, sq = 0; x < size.width; x++)    \
            {                                                   \
                worktype it = src[x];                           \
                sumtype t = cast_macro(it);                     \
                sqsumtype tq = cast_sqr_macro(it);              \
                s += t;                                         \
                sq += tq;                                       \
                t = sum[x - sumstep] + s;                       \
                tq = sqsum[x - sqsumstep] + sq;                 \
                sum[x] = t;                                     \
                sqsum[x] = tq;                                  \
            }                                                   \
        }                                                       \
    }                                                           \
    else                                                        \
    {                                                           \
        if(sqsum == 0)                                        \
        {                                                       \
            assert(0);                                          \
            return CC_NULLPTR_ERR;                              \
        }                                                       \
                                                                \
        buf = (sumtype*)cvStackAlloc((size.width + 1)* sizeof(buf[0]));\
        sum[-1] = tilted[-1] = 0;                               \
        sqsum[-1] = 0;                                          \
                                                                \
        for(x = 0, s = 0, sq = 0; x < size.width; x++)        \
        {                                                       \
            worktype it = src[x];                               \
            sumtype t = cast_macro(it);                         \
            sqsumtype tq = cast_sqr_macro(it);                  \
            buf[x] = tilted[x] = t;                             \
            s += t;                                             \
            sq += tq;                                           \
            sum[x] = s;                                         \
            sqsum[x] = sq;                                      \
        }                                                       \
                                                                \
        if(size.width == 1)                                   \
            buf[1] = 0;                                         \
                                                                \
        for(y = 1; y < size.height; y++)                      \
        {                                                       \
            worktype it;                                        \
            sumtype t0;                                         \
            sqsumtype tq0;                                      \
                                                                \
            src += srcstep;                                     \
            sum += sumstep;                                     \
            sqsum += sqsumstep;                                 \
            tilted += tiltedstep;                               \
                                                                \
            it = src[0/*x*/];                                   \
            s = t0 = cast_macro(it);                            \
            sq = tq0 = cast_sqr_macro(it);                      \
                                                                \
            sum[-1] = 0;                                        \
            sqsum[-1] = 0;                                      \
            /*tilted[-1] = buf[0];*/                            \
            tilted[-1] = tilted[-tiltedstep];                   \
                                                                \
            sum[0] = sum[-sumstep] + t0;                        \
            sqsum[0] = sqsum[-sqsumstep] + tq0;                 \
            tilted[0] = tilted[-tiltedstep] + t0 + buf[1];      \
                                                                \
            for(x = 1; x < size.width - 1; x++)               \
            {                                                   \
                sumtype t1 = buf[x];                            \
                buf[x-1] = t1 + t0;                             \
                it = src[x];                                    \
                t0 = cast_macro(it);                            \
                tq0 = cast_sqr_macro(it);                       \
                s += t0;                                        \
                sq += tq0;                                      \
                sum[x] = sum[x - sumstep] + s;                  \
                sqsum[x] = sqsum[x - sqsumstep] + sq;           \
                t1 += buf[x+1] + t0 + tilted[x - tiltedstep - 1];\
                tilted[x] = t1;                                 \
            }                                                   \
                                                                \
            if(size.width > 1)                                \
            {                                                   \
                sumtype t1 = buf[x];                            \
                buf[x-1] = t1 + t0;                             \
                it = src[x];    /*+*/                           \
                t0 = cast_macro(it);                            \
                tq0 = cast_sqr_macro(it);                       \
                s += t0;                                        \
                sq += tq0;                                      \
                sum[x] = sum[x - sumstep] + s;                  \
                sqsum[x] = sqsum[x - sqsumstep] + sq;           \
                tilted[x] = t0 + t1 + tilted[x - tiltedstep - 1];\
                buf[x] = t0;                                    \
            }                                                   \
        }                                                       \
    }                                                           \
                                                                \
    return CC_OK;                                               \
}


ICC_DEF_INTEGRAL_OP_C1(8u32s, uchar, int, double, int, CC_NOP, CC_8TO32F_SQR)
ICC_DEF_INTEGRAL_OP_C1(8u64f, uchar, double, double, int, CC_8TO32F, CC_8TO32F_SQR)
ICC_DEF_INTEGRAL_OP_C1(32f64f, float, double, double, double, CC_NOP, CC_SQR)
ICC_DEF_INTEGRAL_OP_C1(64f, double, double, double, double, CC_NOP, CC_SQR)


#define ICC_DEF_INTEGRAL_OP_CN(flavor, arrtype, sumtype, sqsumtype,    \
                                worktype, cast_macro, cast_sqr_macro)  \
static CStatus C_STDCALL                                      \
icvIntegralImage_##flavor##_CnR(const arrtype* src, int srcstep,\
                                 sumtype* sum, int sumstep,     \
                                 sqsumtype* sqsum, int sqsumstep,\
                                 CSize size, int cn)          \
{                                                               \
    int x, y;                                                   \
    srcstep /= sizeof(src[0]);                                  \
                                                                \
    memset(sum, 0, (size.width+1)*cn*sizeof(sum[0]));          \
    sumstep /= sizeof(sum[0]);                                  \
    sum += sumstep + cn;                                        \
                                                                \
    if(sqsum)                                                 \
    {                                                           \
        memset(sqsum, 0, (size.width+1)*cn*sizeof(sqsum[0]));  \
        sqsumstep /= sizeof(sqsum[0]);                          \
        sqsum += sqsumstep + cn;                                \
    }                                                           \
                                                                \
    size.width *= cn;                                           \
                                                                \
    if(sqsum == 0)                                            \
    {                                                           \
        for(y = 0; y < size.height; y++, src += srcstep,       \
                                          sum += sumstep)      \
        {                                                       \
            for(x = -cn; x < 0; x++)                          \
                sum[x] = 0;                                     \
                                                                \
            for(x = 0; x < size.width; x++)                   \
                sum[x] = cast_macro(src[x]) + sum[x - cn];      \
                                                                \
            for(x = 0; x < size.width; x++)                   \
                sum[x] = sum[x] + sum[x - sumstep];             \
        }                                                       \
    }                                                           \
    else                                                        \
    {                                                           \
        for(y = 0; y < size.height; y++, src += srcstep,       \
                        sum += sumstep, sqsum += sqsumstep)    \
        {                                                       \
            for(x = -cn; x < 0; x++)                          \
            {                                                   \
                sum[x] = 0;                                     \
                sqsum[x] = 0;                                   \
            }                                                   \
                                                                \
            for(x = 0; x < size.width; x++)                   \
            {                                                   \
                worktype it = src[x];                           \
                sumtype t = cast_macro(it) + sum[x-cn];         \
                sqsumtype tq = cast_sqr_macro(it) + sqsum[x-cn];\
                sum[x] = t;                                     \
                sqsum[x] = tq;                                  \
            }                                                   \
                                                                \
            for(x = 0; x < size.width; x++)                   \
            {                                                   \
                sumtype t = sum[x] + sum[x - sumstep];          \
                sqsumtype tq = sqsum[x] + sqsum[x - sqsumstep]; \
                sum[x] = t;                                     \
                sqsum[x] = tq;                                  \
            }                                                   \
        }                                                       \
    }                                                           \
                                                                \
    return CC_OK;                                               \
}


ICC_DEF_INTEGRAL_OP_CN(8u32s, uchar, int, double, int, CC_NOP, CC_8TO32F_SQR)
ICC_DEF_INTEGRAL_OP_CN(8u64f, uchar, double, double, int, CC_8TO32F, CC_8TO32F_SQR)
ICC_DEF_INTEGRAL_OP_CN(32f64f, float, double, double, double, CC_NOP, CC_SQR)
ICC_DEF_INTEGRAL_OP_CN(64f, double, double, double, double, CC_NOP, CC_SQR)


static void icvInitIntegralImageTable(CvFuncTable  table_c1, CvFuncTable  table_cn)
{
    table_c1[CC_8U] = (void*)icvIntegralImage_8u64f_C1R;
    table_c1[CC_32F] = (void*)icvIntegralImage_32f64f_C1R;
    table_c1[CC_64F] = (void*)icvIntegralImage_64f_C1R;

    table_cn[CC_8U] = (void*)icvIntegralImage_8u64f_CnR;
    table_cn[CC_32F] = (void*)icvIntegralImage_32f64f_CnR;
    table_cn[CC_64F] = (void*)icvIntegralImage_64f_CnR;
}


typedef CStatus (C_STDCALL * CvIntegralImageFuncC1)(
    const void* src, int srcstep, void* sum, int sumstep,
    void* sqsum, int sqsumstep, void* tilted, int tiltedstep,
    CSize size);

typedef CStatus (C_STDCALL * CvIntegralImageFuncCn)(
    const void* src, int srcstep, void* sum, int sumstep,
    void* sqsum, int sqsumstep, CSize size, int cn);


CC_IMPL void
cvIntegral(const img_t* src, img_t* sum,
            img_t* sqsum CC_DEFAULT(NULL), img_t* tilted CC_DEFAULT(NULL))
{
    static CvFuncTable tab_c1, tab_cn;
    static int inittab = 0;
    
    CC_FUNCNAME("cvIntegralImage");

    __BEGIN__;

    int coi0 = 0, coi1 = 0, coi2 = 0, coi3 = 0;
    TypeId type;
    int cn, src_step, sum_step, sqsum_step, tilted_step;
    CvIntegralImageFuncC1 func_c1 = 0;
    CvIntegralImageFuncCn func_cn = 0;
    CSize size;

    if(!inittab)
    {
        icvInitIntegralImageTable(tab_c1, tab_cn);
        inittab = 1;
    }
    
    if(sum->width != src->width + 1 ||
        sum->height != src->height + 1)
        CC_ERROR(CC_StsUnmatchedSizes, "");

    if(CC_MAT_TYPE(sum) != CC_64F &&
        (CC_MAT_TYPE(src) != CC_8U || CC_MAT_TYPE(sum) != CC_32S) )
        CC_ERROR(CC_StsUnsupportedFormat,
        "Sum array must have 64f type (or 32s type in case of 8u source array) "
        "and the same number of channels as the source array");

    if(sqsum)
    {
        if(!CC_ARE_SIZES_EQ(sum, sqsum))
            CC_ERROR(CC_StsUnmatchedSizes, "");
        if(CC_MAT_TYPE(sqsum) != CC_64F || !CC_ARE_CNS_EQ(src, sqsum))
            CC_ERROR(CC_StsUnsupportedFormat, "Squares sum array must be 64f "
                      "and the same number of channels as the source array");
    }

    if(tilted)
    {
        if(!sqsum)
            CC_ERROR(CC_StsNullPtr,
            "Squared sum array must be passed if tilted sum array is passed");

        if(!CC_ARE_SIZES_EQ(sum, tilted))
            CC_ERROR(CC_StsUnmatchedSizes, "");
        if(!CC_ARE_TYPES_EQ(sum, tilted))
            CC_ERROR(CC_StsUnmatchedFormats,
                      "Sum and tilted sum must have the same types");
        if(CC_MAT_CN(tilted) != 1)
            CC_ERROR(CC_StsNotImplemented,
                      "Tilted sum can not be computed for multi-channel arrays");
    }

    if(coi0 || coi1 || coi2 || coi3)
        CC_ERROR(CC_BadCOI, "COI is not supported by the function");

    type = CC_MAT_DEPTH(src);
    cn = CC_MAT_CN(src);

    if(CC_MAT_TYPE(sum) == CC_32S)
    {
        func_c1 = (CvIntegralImageFuncC1)icvIntegralImage_8u32s_C1R;
        func_cn = (CvIntegralImageFuncCn)icvIntegralImage_8u32s_CnR;
    }
    else
    {
        func_c1 = (CvIntegralImageFuncC1)tab_c1[type];
        func_cn = (CvIntegralImageFuncCn)tab_cn[type];
        if(!func_c1 && !func_cn)
            CC_ERROR(CC_StsUnsupportedFormat, "This source image format is unsupported");
    }

    size = cvGetMatSize(src);
    src_step = src->step ? src->step : CC_STUB_STEP;
    sum_step = sum->step ? sum->step : CC_STUB_STEP;
    sqsum_step = !sqsum ? 0 : sqsum->step ? sqsum->step : CC_STUB_STEP;
    tilted_step = !tilted ? 0 : tilted->step ? tilted->step : CC_STUB_STEP;

    if(cn == 1)
    {
        IPPI_CALL(func_c1(src->tt.data, src_step, sum->tt.data, sum_step,
                        sqsum ? sqsum->tt.data : 0, sqsum_step,
                        tilted ? tilted->tt.data : 0, tilted_step, size));
    }
    else
    {
        IPPI_CALL(func_cn(src->tt.data, src_step, sum->tt.data, sum_step,
                        sqsum ? sqsum->tt.data : 0, sqsum_step, size, cn));
    }

    __END__;
}

#if 0
void integral(const img_t* src, img_t* sum, TypeId type) {
  cvSetMat(sum, src->rows + 1, src->cols + 1, type, 1);
  cvIntegral(src, sum, NULL, NULL);
}
void integral(const img_t* src, img_t* sum, img_t* sqsum, TypeId type) {
  cvSetMat(sum, src->rows + 1, src->cols + 1, type, 1);
  cvSetMat(sqsum, src->rows + 1, src->cols + 1, type, 1);
  cvIntegral(src, sum, sqsum, NULL);
}

#endif