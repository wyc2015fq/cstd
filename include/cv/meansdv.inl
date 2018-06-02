
/****************************************************************************************\
*                              Mean value over the region                                *
\****************************************************************************************/

#define ICC_MEAN_CASE_C1(len)         \
    for(; x <= (len) - 2; x += 2)     \
    {                                   \
        if(mask[x])                   \
             s0 += src[x], pix++;       \
        if(mask[x+1])                 \
            s0 += src[x+1], pix++;      \
    }                                   \
                                        \
    for(; x < (len); x++)             \
        if(mask[x])                   \
            s0 += src[x], pix++


#define ICC_MEAN_CASE_C2(len)           \
    for(; x < (len); x++)               \
        if(mask[x])                     \
        {                               \
            s0 += src[x*2];             \
            s1 += src[x*2+1];           \
            pix++;                      \
        }


#define ICC_MEAN_CASE_C3(len)           \
    for(; x < (len); x++)               \
        if(mask[x])                     \
        {                               \
            s0 += src[x*3];             \
            s1 += src[x*3+1];           \
            s2 += src[x*3+2];           \
            pix++;                      \
        }


#define ICC_MEAN_CASE_C4(len)           \
    for(; x < (len); x++)               \
        if(mask[x])                     \
        {                               \
            s0 += src[x*4];             \
            s1 += src[x*4+1];           \
            s2 += src[x*4+2];           \
            s3 += src[x*4+3];           \
            pix++;                      \
        }


#define ICC_MEAN_COI_CASE(len, cn)      \
    for(; x <= (len) - 2; x += 2)       \
    {                                   \
        if(mask[x])                     \
             s0 += src[x*(cn)], pix++;  \
        if(mask[x+1])                   \
            s0+=src[(x+1)*(cn)], pix++; \
    }                                   \
                                        \
    for(; x < (len); x++)               \
        if(mask[x])                     \
            s0 += src[x*(cn)], pix++;


////////////////////////////////////// entry macros //////////////////////////////////////

#define ICC_MEAN_ENTRY_COMMON()         \
    int pix = 0;                        \
    step /= sizeof(src[0])

#define ICC_MEAN_ENTRY_C1(sumtype)      \
    sumtype s0 = 0;                     \
    ICC_MEAN_ENTRY_COMMON()

#define ICC_MEAN_ENTRY_C2(sumtype)      \
    sumtype s0 = 0, s1 = 0;             \
    ICC_MEAN_ENTRY_COMMON()

#define ICC_MEAN_ENTRY_C3(sumtype)      \
    sumtype s0 = 0, s1 = 0, s2 = 0;     \
    ICC_MEAN_ENTRY_COMMON()

#define ICC_MEAN_ENTRY_C4(sumtype)          \
    sumtype s0 = 0, s1 = 0, s2 = 0, s3 = 0; \
    ICC_MEAN_ENTRY_COMMON()


#define ICC_MEAN_ENTRY_BLOCK_COMMON(block_size)   \
    int remaining = block_size;                   \
    ICC_MEAN_ENTRY_COMMON()

#define ICC_MEAN_ENTRY_BLOCK_C1(sumtype, worktype, block_size)  \
    sumtype sum0 = 0;                                           \
    worktype s0 = 0;                                            \
    ICC_MEAN_ENTRY_BLOCK_COMMON(block_size)

#define ICC_MEAN_ENTRY_BLOCK_C2(sumtype, worktype, block_size)  \
    sumtype sum0 = 0, sum1 = 0;                                 \
    worktype s0 = 0, s1 = 0;                                    \
    ICC_MEAN_ENTRY_BLOCK_COMMON(block_size)

#define ICC_MEAN_ENTRY_BLOCK_C3(sumtype, worktype, block_size)  \
    sumtype sum0 = 0, sum1 = 0, sum2 = 0;                       \
    worktype s0 = 0, s1 = 0, s2 = 0;                            \
    ICC_MEAN_ENTRY_BLOCK_COMMON(block_size)

#define ICC_MEAN_ENTRY_BLOCK_C4(sumtype, worktype, block_size)  \
    sumtype sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;             \
    worktype s0 = 0, s1 = 0, s2 = 0, s3 = 0;                    \
    ICC_MEAN_ENTRY_BLOCK_COMMON(block_size)


/////////////////////////////////////// exit macros //////////////////////////////////////

#define ICC_MEAN_EXIT_COMMON()          \
    double scale = pix ? 1./pix : 0

#define ICC_MEAN_EXIT_C1(tmp)           \
    ICC_MEAN_EXIT_COMMON();             \
    mean[0] = scale*(double)tmp##0

#define ICC_MEAN_EXIT_C2(tmp)           \
    ICC_MEAN_EXIT_COMMON();             \
    double t0 = scale*(double)tmp##0;   \
    double t1 = scale*(double)tmp##1;   \
    mean[0] = t0;                       \
    mean[1] = t1

#define ICC_MEAN_EXIT_C3(tmp)           \
    ICC_MEAN_EXIT_COMMON();             \
    double t0 = scale*(double)tmp##0;   \
    double t1 = scale*(double)tmp##1;   \
    double t2 = scale*(double)tmp##2;   \
    mean[0] = t0;                       \
    mean[1] = t1;                       \
    mean[2] = t2

#define ICC_MEAN_EXIT_C4(tmp)           \
    ICC_MEAN_EXIT_COMMON();             \
    double t0 = scale*(double)tmp##0;   \
    double t1 = scale*(double)tmp##1;   \
    mean[0] = t0;                       \
    mean[1] = t1;                       \
    t0 = scale*(double)tmp##2;          \
    t1 = scale*(double)tmp##3;          \
    mean[2] = t0;                       \
    mean[3] = t1

#define ICC_MEAN_EXIT_BLOCK_C1()    \
    sum0 += s0;                     \
    ICC_MEAN_EXIT_C1(sum)

#define ICC_MEAN_EXIT_BLOCK_C2()    \
    sum0 += s0; sum1 += s1;         \
    ICC_MEAN_EXIT_C2(sum)

#define ICC_MEAN_EXIT_BLOCK_C3()    \
    sum0 += s0; sum1 += s1;         \
    sum2 += s2;                     \
    ICC_MEAN_EXIT_C3(sum)

#define ICC_MEAN_EXIT_BLOCK_C4()    \
    sum0 += s0; sum1 += s1;         \
    sum2 += s2; sum3 += s3;         \
    ICC_MEAN_EXIT_C4(sum)

////////////////////////////////////// update macros /////////////////////////////////////

#define ICC_MEAN_UPDATE_COMMON(block_size)  \
    remaining = block_size

#define ICC_MEAN_UPDATE_C1(block_size)      \
    ICC_MEAN_UPDATE_COMMON(block_size);     \
    sum0 += s0;                             \
    s0 = 0

#define ICC_MEAN_UPDATE_C2(block_size)      \
    ICC_MEAN_UPDATE_COMMON(block_size);     \
    sum0 += s0; sum1 += s1;                 \
    s0 = s1 = 0

#define ICC_MEAN_UPDATE_C3(block_size)      \
    ICC_MEAN_UPDATE_COMMON(block_size);     \
    sum0 += s0; sum1 += s1; sum2 += s2;     \
    s0 = s1 = s2 = 0

#define ICC_MEAN_UPDATE_C4(block_size)      \
    ICC_MEAN_UPDATE_COMMON(block_size);     \
    sum0 += s0; sum1 += s1;                 \
    sum2 += s2; sum3 += s3;                 \
    s0 = s1 = s2 = s3 = 0


#define ICC_IMPL_MEAN_BLOCK_FUNC_2D(flavor, cn,                 \
    arrtype, sumtype, worktype, block_size)                     \
IPCVAPI_IMPL(CStatus, icvMean_##flavor##_C##cn##MR,            \
    (const arrtype* src, int step,                              \
      const uchar* mask, int maskstep,                          \
      CSize size, double* mean),                               \
    (src, step, mask, maskstep, size, mean))                    \
{                                                               \
    ICC_MEAN_ENTRY_BLOCK_C##cn(sumtype, worktype, block_size);  \
                                                                \
    for(; size.height--; src += step, mask += maskstep)         \
    {                                                           \
        int x = 0;                                              \
        while(x < size.width)                                   \
        {                                                       \
            int limit = MIN(remaining, size.width - x);         \
            remaining -= limit;                                 \
            limit += x;                                         \
            ICC_MEAN_CASE_C##cn(limit);                         \
            if(remaining == 0)                                  \
            {                                                   \
                ICC_MEAN_UPDATE_C##cn(block_size);              \
            }                                                   \
        }                                                       \
    }                                                           \
                                                                \
    { ICC_MEAN_EXIT_BLOCK_C##cn(); }                            \
    return CC_OK;                                               \
}


#define ICC_IMPL_MEAN_FUNC_2D(flavor, cn,                       \
                arrtype, sumtype, worktype)                     \
IPCVAPI_IMPL(CStatus, icvMean_##flavor##_C##cn##MR,            \
    (const arrtype* src, int step,                              \
      const uchar* mask, int maskstep,                          \
      CSize size, double* mean),                               \
    (src, step, mask, maskstep, size, mean))                    \
{                                                               \
    ICC_MEAN_ENTRY_C##cn(sumtype);                              \
                                                                \
    for(; size.height--; src += step, mask += maskstep)         \
    {                                                           \
        int x = 0;                                              \
        ICC_MEAN_CASE_C##cn(size.width);                        \
    }                                                           \
                                                                \
    { ICC_MEAN_EXIT_C##cn(s); }                                 \
    return CC_OK;                                               \
}


#define ICC_IMPL_MEAN_BLOCK_FUNC_2D_COI(flavor,                 \
        arrtype, sumtype, worktype, block_size)                 \
static CStatus C_STDCALL                                      \
icvMean_##flavor##_CnCMR(const arrtype* src, int step,          \
                          const uchar* mask, int maskstep,      \
                          CSize size, int cn,                  \
                          int coi, double* mean)                \
{                                                               \
    ICC_MEAN_ENTRY_BLOCK_C1(sumtype, worktype, block_size);     \
    src += coi - 1;                                             \
                                                                \
    for(; size.height--; src += step, mask += maskstep)         \
    {                                                           \
        int x = 0;                                              \
        while(x < size.width)                                   \
        {                                                       \
            int limit = MIN(remaining, size.width - x);         \
            remaining -= limit;                                 \
            limit += x;                                         \
            ICC_MEAN_COI_CASE(limit, cn);                       \
            if(remaining == 0)                                  \
            {                                                   \
                ICC_MEAN_UPDATE_C1(block_size);                 \
            }                                                   \
        }                                                       \
    }                                                           \
                                                                \
    { ICC_MEAN_EXIT_BLOCK_C1(); }                               \
    return CC_OK;                                               \
}


#define ICC_IMPL_MEAN_FUNC_2D_COI(flavor,                       \
                arrtype, sumtype, worktype)                     \
static CStatus C_STDCALL                                      \
icvMean_##flavor##_CnCMR(const arrtype* src, int step,          \
                          const uchar* mask, int maskstep,      \
                          CSize size, int cn,                  \
                          int coi, double* mean)               \
{                                                               \
    ICC_MEAN_ENTRY_C1(sumtype);                               \
    src += coi - 1;                                             \
                                                                \
    for(; size.height--; src += step, mask += maskstep)       \
    {                                                           \
        int x = 0;                                              \
        ICC_MEAN_COI_CASE(size.width, cn);                    \
    }                                                           \
                                                                \
    { ICC_MEAN_EXIT_C1(s); }                                  \
    return CC_OK;                                               \
}


#define ICC_IMPL_MEAN_BLOCK_ALL(flavor, arrtype, sumtype,      \
                                 worktype, block_size)         \
    ICC_IMPL_MEAN_BLOCK_FUNC_2D(flavor, 1, arrtype, sumtype,   \
                                 worktype, block_size)         \
    ICC_IMPL_MEAN_BLOCK_FUNC_2D(flavor, 2, arrtype, sumtype,   \
                                 worktype, block_size)         \
    ICC_IMPL_MEAN_BLOCK_FUNC_2D(flavor, 3, arrtype, sumtype,   \
                                 worktype, block_size)         \
    ICC_IMPL_MEAN_BLOCK_FUNC_2D(flavor, 4, arrtype, sumtype,   \
                                 worktype, block_size)         \
    ICC_IMPL_MEAN_BLOCK_FUNC_2D_COI(flavor, arrtype, sumtype,  \
                                 worktype, block_size)

#define ICC_IMPL_MEAN_ALL(flavor, arrtype, sumtype, worktype)     \
    ICC_IMPL_MEAN_FUNC_2D(flavor, 1, arrtype, sumtype, worktype)  \
    ICC_IMPL_MEAN_FUNC_2D(flavor, 2, arrtype, sumtype, worktype)  \
    ICC_IMPL_MEAN_FUNC_2D(flavor, 3, arrtype, sumtype, worktype)  \
    ICC_IMPL_MEAN_FUNC_2D(flavor, 4, arrtype, sumtype, worktype)  \
    ICC_IMPL_MEAN_FUNC_2D_COI(flavor, arrtype, sumtype, worktype)

ICC_IMPL_MEAN_BLOCK_ALL(8u, uchar, int64, unsigned, 1 << 24)
ICC_IMPL_MEAN_BLOCK_ALL(16u, ushort, int64, unsigned, 1 << 16)
ICC_IMPL_MEAN_BLOCK_ALL(16s, short, int64, int, 1 << 16)
ICC_IMPL_MEAN_ALL(32s, int, double, double)
ICC_IMPL_MEAN_ALL(32f, float, double, double)
ICC_IMPL_MEAN_ALL(64f, double, double, double)

#define icvMean_8s_C1MR 0
#define icvMean_8s_C2MR 0
#define icvMean_8s_C3MR 0
#define icvMean_8s_C4MR 0
#define icvMean_8s_CnCMR 0

CC_DEF_INIT_BIG_FUNC_TAB_2D(Mean, MR)
CC_DEF_INIT_FUNC_TAB_2D(Mean, CnCMR)

CC_IMPL void
cvAvg(const img_t* mat, double4 _mean, const img_t* mask CC_DEFAULT(NULL))
{
    CScalar mean = {{0,0,0,0}};

    static CvBigFuncTable mean_tab;
    static CvFuncTable meancoi_tab;
    static int inittab = 0;

    CC_FUNCNAME("cvAvg");

    __BEGIN__;

    CSize size;
    double scale;

    if(!mask)
    {
        CC_CALL(mean = cvSum(mat));
        size = cvGetSize(mat);
        size.width *= size.height;
        scale = size.width ? 1./size.width : 0;

        mean.val[0] *= scale;
        mean.val[1] *= scale;
        mean.val[2] *= scale;
        mean.val[3] *= scale;
    }
    else
    {
        TypeId type;
        int coi = 0;
        int mat_step, mask_step;
        int cn = CC_MAT_CN(mat);

        if(!inittab)
        {
            icvInitMeanMRTable(mean_tab);
            icvInitMeanCnCMRTable(meancoi_tab);
            inittab = 1;
        }

        if(!CC_IS_MASK_ARR(mask))
            CC_ERROR(CC_StsBadMask, "");

        if(!CC_ARE_SIZES_EQ(mat, mask))
            CC_ERROR(CC_StsUnmatchedSizes, "");

        type = CC_MAT_DEPTH(mat);
        size = cvGetMatSize(mat);

        mat_step = mat->step;
        mask_step = mask->step;

        if(CC_IS_MAT_CONT(mat) && CC_IS_MAT_CONT(mask))
        {
            size.width *= size.height;
            size.height = 1;
            mat_step = mask_step = CC_STUB_STEP;
        }

        if(CC_MAT_CN(mat) == 1 || coi == 0)
        {
            CvFunc2D_2A1P func;

            if(CC_MAT_CN(mat) > 4)
                CC_ERROR(CC_StsOutOfRange, "The input array must have at most 4 channels unless COI is set");

            func = (CvFunc2D_2A1P)(mean_tab[type][cn]);

            if(!func)
                CC_ERROR(CC_StsBadArg, cvUnsupportedFormat);

            IPPI_CALL(func(mat->tt.data, mat_step, mask->tt.data,
                             mask_step, size, mean.val));
        }
        else
        {
            CvFunc2DnC_2A1P func = (CvFunc2DnC_2A1P)(
                meancoi_tab[type]);

            if(!func)
                CC_ERROR(CC_StsBadArg, cvUnsupportedFormat);

            IPPI_CALL(func(mat->tt.data, mat_step, mask->tt.data,
                             mask_step, size, cn, coi, mean.val));
        }
    }

    __END__;

    COPY4(_mean, mean.val);
    return ;
}


CC_INLINE CScalar cvMean(const img_t* image, const img_t* mask CC_DEFAULT(0))
{
    CScalar mean = {0};
    cvAvg(image, mean.val, mask);
    return mean;
}



/****************************************************************************************\
*                             Mean and StdDev calculation                                *
\****************************************************************************************/

#define ICC_MEAN_SDV_COI_CASE(worktype, sqsumtype, \
                               sqr_macro, len, cn) \
    for(; x <= (len) - 4*(cn); x += 4*(cn))\
    {                                       \
        worktype t0 = src[x];               \
        worktype t1 = src[x + (cn)];        \
                                            \
        s0  += t0 + t1;                     \
        sq0 += (sqsumtype)(sqr_macro(t0)) + \
               (sqsumtype)(sqr_macro(t1));  \
                                            \
        t0 = src[x + 2*(cn)];               \
        t1 = src[x + 3*(cn)];               \
                                            \
        s0  += t0 + t1;                     \
        sq0 += (sqsumtype)(sqr_macro(t0)) + \
               (sqsumtype)(sqr_macro(t1));  \
    }                                       \
                                            \
    for(; x < (len); x += (cn))           \
    {                                       \
        worktype t0 = src[x];               \
                                            \
        s0 += t0;                           \
        sq0 += (sqsumtype)(sqr_macro(t0));  \
    }


#define ICC_MEAN_SDV_CASE_C1(worktype, sqsumtype, sqr_macro, len) \
    ICC_MEAN_SDV_COI_CASE(worktype, sqsumtype, sqr_macro, len, 1)


#define ICC_MEAN_SDV_CASE_C2(worktype, sqsumtype, \
                              sqr_macro, len) \
    for(; x < (len); x += 2)              \
    {                                       \
        worktype t0 = (src)[x];             \
        worktype t1 = (src)[x + 1];         \
                                            \
        s0 += t0;                           \
        sq0 += (sqsumtype)(sqr_macro(t0));  \
        s1 += t1;                           \
        sq1 += (sqsumtype)(sqr_macro(t1));  \
    }


#define ICC_MEAN_SDV_CASE_C3(worktype, sqsumtype, \
                              sqr_macro, len) \
    for(; x < (len); x += 3)              \
    {                                       \
        worktype t0 = (src)[x];             \
        worktype t1 = (src)[x + 1];         \
        worktype t2 = (src)[x + 2];         \
                                            \
        s0 += t0;                           \
        sq0 += (sqsumtype)(sqr_macro(t0));  \
        s1 += t1;                           \
        sq1 += (sqsumtype)(sqr_macro(t1));  \
        s2 += t2;                           \
        sq2 += (sqsumtype)(sqr_macro(t2));  \
    }


#define ICC_MEAN_SDV_CASE_C4(worktype, sqsumtype, \
                              sqr_macro, len) \
    for(; x < (len); x += 4)              \
    {                                       \
        worktype t0 = (src)[x];             \
        worktype t1 = (src)[x + 1];         \
                                            \
        s0 += t0;                           \
        sq0 += (sqsumtype)(sqr_macro(t0));  \
        s1 += t1;                           \
        sq1 += (sqsumtype)(sqr_macro(t1));  \
                                            \
        t0 = (src)[x + 2];                  \
        t1 = (src)[x + 3];                  \
                                            \
        s2 += t0;                           \
        sq2 += (sqsumtype)(sqr_macro(t0));  \
        s3 += t1;                           \
        sq3 += (sqsumtype)(sqr_macro(t1));  \
    }


#define ICC_MEAN_SDV_MASK_COI_CASE(worktype, sqsumtype, \
                                    sqr_macro, len, cn) \
    for(; x <= (len) - 4; x += 4)             \
    {                                           \
        worktype t0;                            \
        if(mask[x])                           \
        {                                       \
            t0 = src[x*(cn)]; pix++;            \
            s0 += t0;                           \
            sq0 += sqsumtype(sqr_macro(t0));    \
        }                                       \
                                                \
        if(mask[x+1])                         \
        {                                       \
            t0 = src[(x+1)*(cn)]; pix++;        \
            s0 += t0;                           \
            sq0 += sqsumtype(sqr_macro(t0));    \
        }                                       \
                                                \
        if(mask[x+2])                         \
        {                                       \
            t0 = src[(x+2)*(cn)]; pix++;        \
            s0 += t0;                           \
            sq0 += sqsumtype(sqr_macro(t0));    \
        }                                       \
                                                \
        if(mask[x+3])                         \
        {                                       \
            t0 = src[(x+3)*(cn)]; pix++;        \
            s0 += t0;                           \
            sq0 += sqsumtype(sqr_macro(t0));    \
        }                                       \
    }                                           \
                                                \
    for(; x < (len); x++)                     \
    {                                           \
        if(mask[x])                           \
        {                                       \
            worktype t0 = src[x*(cn)]; pix++;   \
            s0 += t0;                           \
            sq0 += sqsumtype(sqr_macro(t0));    \
        }                                       \
    }


#define ICC_MEAN_SDV_MASK_CASE_C1(worktype, sqsumtype, sqr_macro, len)    \
    ICC_MEAN_SDV_MASK_COI_CASE(worktype, sqsumtype, sqr_macro, len, 1)


#define ICC_MEAN_SDV_MASK_CASE_C2(worktype, sqsumtype,\
                                   sqr_macro, len)    \
    for(; x < (len); x++)                     \
    {                                           \
        if(mask[x])                           \
        {                                       \
            worktype t0 = src[x*2];             \
            worktype t1 = src[x*2+1];           \
            pix++;                              \
            s0 += t0;                           \
            sq0 += sqsumtype(sqr_macro(t0));    \
            s1 += t1;                           \
            sq1 += sqsumtype(sqr_macro(t1));    \
        }                                       \
    }


#define ICC_MEAN_SDV_MASK_CASE_C3(worktype, sqsumtype,\
                                   sqr_macro, len)    \
    for(; x < (len); x++)                     \
    {                                           \
        if(mask[x])                           \
        {                                       \
            worktype t0 = src[x*3];             \
            worktype t1 = src[x*3+1];           \
            worktype t2 = src[x*3+2];           \
            pix++;                              \
            s0 += t0;                           \
            sq0 += sqsumtype(sqr_macro(t0));    \
            s1 += t1;                           \
            sq1 += sqsumtype(sqr_macro(t1));    \
            s2 += t2;                           \
            sq2 += sqsumtype(sqr_macro(t2));    \
        }                                       \
    }


#define ICC_MEAN_SDV_MASK_CASE_C4(worktype, sqsumtype,\
                                   sqr_macro, len)    \
    for(; x < (len); x++)                     \
    {                                           \
        if(mask[x])                           \
        {                                       \
            worktype t0 = src[x*4];             \
            worktype t1 = src[x*4+1];           \
            pix++;                              \
            s0 += t0;                           \
            sq0 += sqsumtype(sqr_macro(t0));    \
            s1 += t1;                           \
            sq1 += sqsumtype(sqr_macro(t1));    \
            t0 = src[x*4+2];                    \
            t1 = src[x*4+3];                    \
            s2 += t0;                           \
            sq2 += sqsumtype(sqr_macro(t0));    \
            s3 += t1;                           \
            sq3 += sqsumtype(sqr_macro(t1));    \
        }                                       \
    }


////////////////////////////////////// entry macros //////////////////////////////////////

#define ICC_MEAN_SDV_ENTRY_COMMON()                 \
    int pix;                                        \
    double scale, tmp;                              \
    step /= sizeof(src[0])

#define ICC_MEAN_SDV_ENTRY_C1(sumtype, sqsumtype) \
    sumtype s0 = 0;                                 \
    sqsumtype sq0 = 0;                              \
    ICC_MEAN_SDV_ENTRY_COMMON()

#define ICC_MEAN_SDV_ENTRY_C2(sumtype, sqsumtype) \
    sumtype s0 = 0, s1 = 0;                         \
    sqsumtype sq0 = 0, sq1 = 0;                     \
    ICC_MEAN_SDV_ENTRY_COMMON()

#define ICC_MEAN_SDV_ENTRY_C3(sumtype, sqsumtype) \
    sumtype s0 = 0, s1 = 0, s2 = 0;                 \
    sqsumtype sq0 = 0, sq1 = 0, sq2 = 0;            \
    ICC_MEAN_SDV_ENTRY_COMMON()

#define ICC_MEAN_SDV_ENTRY_C4(sumtype, sqsumtype) \
    sumtype s0 = 0, s1 = 0, s2 = 0, s3 = 0;         \
    sqsumtype sq0 = 0, sq1 = 0, sq2 = 0, sq3 = 0;   \
    ICC_MEAN_SDV_ENTRY_COMMON()


#define ICC_MEAN_SDV_ENTRY_BLOCK_COMMON(block_size)   \
    int remaining = block_size;                         \
    ICC_MEAN_SDV_ENTRY_COMMON()

#define ICC_MEAN_SDV_ENTRY_BLOCK_C1(sumtype, sqsumtype,        \
                        worktype, sqworktype, block_size)      \
    sumtype sum0 = 0;                                           \
    sqsumtype sqsum0 = 0;                                       \
    worktype s0 = 0;                                            \
    sqworktype sq0 = 0;                                         \
    ICC_MEAN_SDV_ENTRY_BLOCK_COMMON(block_size)

#define ICC_MEAN_SDV_ENTRY_BLOCK_C2(sumtype, sqsumtype,        \
                        worktype, sqworktype, block_size)      \
    sumtype sum0 = 0, sum1 = 0;                                 \
    sqsumtype sqsum0 = 0, sqsum1 = 0;                           \
    worktype s0 = 0, s1 = 0;                                    \
    sqworktype sq0 = 0, sq1 = 0;                                \
    ICC_MEAN_SDV_ENTRY_BLOCK_COMMON(block_size)

#define ICC_MEAN_SDV_ENTRY_BLOCK_C3(sumtype, sqsumtype,        \
                        worktype, sqworktype, block_size)      \
    sumtype sum0 = 0, sum1 = 0, sum2 = 0;                       \
    sqsumtype sqsum0 = 0, sqsum1 = 0, sqsum2 = 0;               \
    worktype s0 = 0, s1 = 0, s2 = 0;                            \
    sqworktype sq0 = 0, sq1 = 0, sq2 = 0;                       \
    ICC_MEAN_SDV_ENTRY_BLOCK_COMMON(block_size)

#define ICC_MEAN_SDV_ENTRY_BLOCK_C4(sumtype, sqsumtype,        \
                        worktype, sqworktype, block_size)      \
    sumtype sum0 = 0, sum1 = 0, sum2 = 0, sum3 = 0;             \
    sqsumtype sqsum0 = 0, sqsum1 = 0, sqsum2 = 0, sqsum3 = 0;   \
    worktype s0 = 0, s1 = 0, s2 = 0, s3 = 0;                    \
    sqworktype sq0 = 0, sq1 = 0, sq2 = 0, sq3 = 0;              \
    ICC_MEAN_SDV_ENTRY_BLOCK_COMMON(block_size)


/////////////////////////////////////// exit macros //////////////////////////////////////

#define ICC_MEAN_SDV_EXIT_COMMON()              \
    scale = pix ? 1./pix : 0

#define ICC_MEAN_SDV_EXIT_CN(total, sqtotal, idx) \
    ICC_MEAN_SDV_EXIT_COMMON();                 \
    mean[idx] = tmp = scale*(double)total##idx; \
    tmp = scale*(double)sqtotal##idx - tmp*tmp; \
    sdv[idx] = sqrt(MAX(tmp,0.))

#define ICC_MEAN_SDV_EXIT_C1(total, sqtotal)  \
    ICC_MEAN_SDV_EXIT_COMMON();                 \
    ICC_MEAN_SDV_EXIT_CN(total, sqtotal, 0)

#define ICC_MEAN_SDV_EXIT_C2(total, sqtotal)  \
    ICC_MEAN_SDV_EXIT_COMMON();                 \
    ICC_MEAN_SDV_EXIT_CN(total, sqtotal, 0);  \
    ICC_MEAN_SDV_EXIT_CN(total, sqtotal, 1)

#define ICC_MEAN_SDV_EXIT_C3(total, sqtotal)  \
    ICC_MEAN_SDV_EXIT_COMMON();                 \
    ICC_MEAN_SDV_EXIT_CN(total, sqtotal, 0);  \
    ICC_MEAN_SDV_EXIT_CN(total, sqtotal, 1);  \
    ICC_MEAN_SDV_EXIT_CN(total, sqtotal, 2)

#define ICC_MEAN_SDV_EXIT_C4(total, sqtotal)  \
    ICC_MEAN_SDV_EXIT_COMMON();                 \
    ICC_MEAN_SDV_EXIT_CN(total, sqtotal, 0);  \
    ICC_MEAN_SDV_EXIT_CN(total, sqtotal, 1);  \
    ICC_MEAN_SDV_EXIT_CN(total, sqtotal, 2);  \
    ICC_MEAN_SDV_EXIT_CN(total, sqtotal, 3)

////////////////////////////////////// update macros /////////////////////////////////////

#define ICC_MEAN_SDV_UPDATE_COMMON(block_size)\
    remaining = block_size

#define ICC_MEAN_SDV_UPDATE_C1(block_size)    \
    ICC_MEAN_SDV_UPDATE_COMMON(block_size);   \
    sum0 += s0; sqsum0 += sq0;                  \
    s0 = 0; sq0 = 0

#define ICC_MEAN_SDV_UPDATE_C2(block_size)    \
    ICC_MEAN_SDV_UPDATE_COMMON(block_size);   \
    sum0 += s0; sqsum0 += sq0;                  \
    sum1 += s1; sqsum1 += sq1;                  \
    s0 = s1 = 0; sq0 = sq1 = 0

#define ICC_MEAN_SDV_UPDATE_C3(block_size)    \
    ICC_MEAN_SDV_UPDATE_COMMON(block_size);   \
    sum0 += s0; sqsum0 += sq0;                  \
    sum1 += s1; sqsum1 += sq1;                  \
    sum2 += s2; sqsum2 += sq2;                  \
    s0 = s1 = s2 = 0; sq0 = sq1 = sq2 = 0

#define ICC_MEAN_SDV_UPDATE_C4(block_size)    \
    ICC_MEAN_SDV_UPDATE_COMMON(block_size);   \
    sum0 += s0; sqsum0 += sq0;                  \
    sum1 += s1; sqsum1 += sq1;                  \
    sum2 += s2; sqsum2 += sq2;                  \
    sum3 += s3; sqsum3 += sq3;                  \
    s0 = s1 = s2 = s3 = 0; sq0 = sq1 = sq2 = sq3 = 0



#define ICC_DEF_MEAN_SDV_BLOCK_FUNC_2D(flavor, cn, arrtype,        \
                                sumtype, sqsumtype, worktype,       \
                                sqworktype, block_size, sqr_macro) \
IPCVAPI_IMPL(CStatus, icvMean_StdDev_##flavor##_C##cn##R,         \
                        (const arrtype* src, int step,             \
                          CSize size, double* mean, double* sdv), \
                          (src, step, size, mean, sdv))            \
{                                                                   \
    ICC_MEAN_SDV_ENTRY_BLOCK_C##cn(sumtype, sqsumtype,             \
                worktype, sqworktype, (block_size)*(cn));          \
    pix = size.width * size.height;                                 \
    size.width *= (cn);                                             \
                                                                    \
    for(; size.height--; src += step)                             \
    {                                                               \
        int x = 0;                                                  \
        while(x < size.width)                                     \
        {                                                           \
            int limit = MIN(remaining, size.width - x);           \
            remaining -= limit;                                     \
            limit += x;                                             \
            ICC_MEAN_SDV_CASE_C##cn(worktype, sqworktype,          \
                                     sqr_macro, limit);            \
            if(remaining == 0)                                    \
            {                                                       \
                ICC_MEAN_SDV_UPDATE_C##cn((block_size)*(cn));     \
            }                                                       \
        }                                                           \
    }                                                               \
                                                                    \
    ICC_MEAN_SDV_UPDATE_C##cn(0);                                   \
    ICC_MEAN_SDV_EXIT_C##cn(sum, sqsum);                          \
    return CC_OK;                                                   \
}


#define ICC_DEF_MEAN_SDV_FUNC_2D(flavor, cn, arrtype,              \
                                  sumtype, sqsumtype, worktype)    \
IPCVAPI_IMPL(CStatus, icvMean_StdDev_##flavor##_C##cn##R,         \
                        (const arrtype* src, int step,             \
                          CSize size, double* mean, double* sdv), \
                          (src, step, size, mean, sdv))            \
{                                                                   \
    ICC_MEAN_SDV_ENTRY_C##cn(sumtype, sqsumtype);                 \
    pix = size.width * size.height;                                 \
    size.width *= (cn);                                             \
                                                                    \
    for(; size.height--; src += step)                             \
    {                                                               \
        int x = 0;                                                  \
        ICC_MEAN_SDV_CASE_C##cn(worktype, sqsumtype,               \
                                 CC_SQR, size.width);              \
    }                                                               \
                                                                    \
    ICC_MEAN_SDV_EXIT_C##cn(s, sq);                               \
    return CC_OK;                                                   \
}


#define ICC_DEF_MEAN_SDV_BLOCK_FUNC_2D_COI(flavor, arrtype,        \
                                sumtype, sqsumtype, worktype,       \
                                sqworktype, block_size, sqr_macro) \
static CStatus C_STDCALL icvMean_StdDev_##flavor##_CnCR           \
                        (const arrtype* src, int step,             \
                          CSize size, int cn, int coi,             \
                          double* mean, double* sdv)               \
{                                                                   \
    ICC_MEAN_SDV_ENTRY_BLOCK_C1(sumtype, sqsumtype,                \
                worktype, sqworktype, (block_size)*(cn));          \
    pix = size.width * size.height;                                 \
    size.width *= (cn);                                             \
    src += coi - 1;                                                 \
                                                                    \
    for(; size.height--; src += step)                             \
    {                                                               \
        int x = 0;                                                  \
        while(x < size.width)                                     \
        {                                                           \
            int limit = MIN(remaining, size.width - x);           \
            remaining -= limit;                                     \
            limit += x;                                             \
            ICC_MEAN_SDV_COI_CASE(worktype, sqworktype,            \
                                   sqr_macro, limit, cn);           \
            if(remaining == 0)                                    \
            {                                                       \
                ICC_MEAN_SDV_UPDATE_C1((block_size)*(cn));        \
            }                                                       \
        }                                                           \
    }                                                               \
                                                                    \
    ICC_MEAN_SDV_UPDATE_C1(0);                                      \
    ICC_MEAN_SDV_EXIT_C1(sum, sqsum);                             \
    return CC_OK;                                                   \
}


#define ICC_DEF_MEAN_SDV_FUNC_2D_COI(flavor, arrtype,              \
                                      sumtype, sqsumtype, worktype)\
static CStatus C_STDCALL icvMean_StdDev_##flavor##_CnCR           \
                        (const arrtype* src, int step, CSize size,\
                        int cn, int coi, double* mean, double* sdv)\
{                                                                   \
    ICC_MEAN_SDV_ENTRY_C1(sumtype, sqsumtype);                    \
    pix = size.width * size.height;                                 \
    size.width *= (cn);                                             \
    src += coi - 1;                                                 \
                                                                    \
    for(; size.height--; src += step)                             \
    {                                                               \
        int x = 0;                                                  \
        ICC_MEAN_SDV_COI_CASE(worktype, sqsumtype,                 \
                               CC_SQR, size.width, cn);            \
    }                                                               \
                                                                    \
    ICC_MEAN_SDV_EXIT_C1(s, sq);                                  \
    return CC_OK;                                                   \
}


#define ICC_DEF_MEAN_SDV_MASK_BLOCK_FUNC_2D(flavor, cn,            \
                        arrtype, sumtype, sqsumtype, worktype,      \
                        sqworktype, block_size, sqr_macro)         \
IPCVAPI_IMPL(CStatus, icvMean_StdDev_##flavor##_C##cn##MR,        \
                        (const arrtype* src, int step,             \
                          const uchar* mask, int maskstep,          \
                          CSize size, double* mean, double* sdv), \
                       (src, step, mask, maskstep, size, mean, sdv))\
{                                                                   \
    ICC_MEAN_SDV_ENTRY_BLOCK_C##cn(sumtype, sqsumtype,             \
                    worktype, sqworktype, block_size);             \
    pix = 0;                                                        \
                                                                    \
    for(; size.height--; src += step, mask += maskstep)           \
    {                                                               \
        int x = 0;                                                  \
        while(x < size.width)                                     \
        {                                                           \
            int limit = MIN(remaining, size.width - x);           \
            remaining -= limit;                                     \
            limit += x;                                             \
            ICC_MEAN_SDV_MASK_CASE_C##cn(worktype, sqworktype,     \
                                          sqr_macro, limit);       \
            if(remaining == 0)                                    \
            {                                                       \
                ICC_MEAN_SDV_UPDATE_C##cn(block_size);            \
            }                                                       \
        }                                                           \
    }                                                               \
                                                                    \
    ICC_MEAN_SDV_UPDATE_C##cn(0);                                   \
    ICC_MEAN_SDV_EXIT_C##cn(sum, sqsum);                          \
    return CC_OK;                                                   \
}


#define ICC_DEF_MEAN_SDV_MASK_FUNC_2D(flavor, cn, arrtype,         \
                                       sumtype, sqsumtype, worktype)\
IPCVAPI_IMPL(CStatus, icvMean_StdDev_##flavor##_C##cn##MR,        \
                        (const arrtype* src, int step,             \
                          const uchar* mask, int maskstep,          \
                          CSize size, double* mean, double* sdv), \
                       (src, step, mask, maskstep, size, mean, sdv))\
{                                                                   \
    ICC_MEAN_SDV_ENTRY_C##cn(sumtype, sqsumtype);                 \
    pix = 0;                                                        \
                                                                    \
    for(; size.height--; src += step, mask += maskstep)           \
    {                                                               \
        int x = 0;                                                  \
        ICC_MEAN_SDV_MASK_CASE_C##cn(worktype, sqsumtype,          \
                                      CC_SQR, size.width);         \
    }                                                               \
                                                                    \
    ICC_MEAN_SDV_EXIT_C##cn(s, sq);                               \
    return CC_OK;                                                   \
}


#define ICC_DEF_MEAN_SDV_MASK_BLOCK_FUNC_2D_COI(flavor,            \
                            arrtype, sumtype, sqsumtype, worktype,  \
                            sqworktype, block_size, sqr_macro)     \
static CStatus C_STDCALL icvMean_StdDev_##flavor##_CnCMR          \
                        (const arrtype* src, int step,             \
                          const uchar* mask, int maskstep,          \
                          CSize size, int cn, int coi,             \
                          double* mean, double* sdv)               \
{                                                                   \
    ICC_MEAN_SDV_ENTRY_BLOCK_C1(sumtype, sqsumtype,                \
                    worktype, sqworktype, block_size);             \
    pix = 0;                                                        \
    src += coi - 1;                                                 \
                                                                    \
    for(; size.height--; src += step, mask += maskstep)           \
    {                                                               \
        int x = 0;                                                  \
        while(x < size.width)                                     \
        {                                                           \
            int limit = MIN(remaining, size.width - x);           \
            remaining -= limit;                                     \
            limit += x;                                             \
            ICC_MEAN_SDV_MASK_COI_CASE(worktype, sqworktype,       \
                                        sqr_macro, limit, cn);     \
            if(remaining == 0)                                    \
            {                                                       \
                ICC_MEAN_SDV_UPDATE_C1(block_size);               \
            }                                                       \
        }                                                           \
    }                                                               \
                                                                    \
    ICC_MEAN_SDV_UPDATE_C1(0);                                      \
    ICC_MEAN_SDV_EXIT_C1(sum, sqsum);                             \
    return CC_OK;                                                   \
}


#define ICC_DEF_MEAN_SDV_MASK_FUNC_2D_COI(flavor, arrtype,         \
                                    sumtype, sqsumtype, worktype)  \
static CStatus C_STDCALL icvMean_StdDev_##flavor##_CnCMR          \
                        (const arrtype* src, int step,             \
                          const uchar* mask, int maskstep,          \
                          CSize size, int cn, int coi,             \
                          double* mean, double* sdv)               \
{                                                                   \
    ICC_MEAN_SDV_ENTRY_C1(sumtype, sqsumtype);                    \
    pix = 0;                                                        \
    src += coi - 1;                                                 \
                                                                    \
    for(; size.height--; src += step, mask += maskstep)           \
    {                                                               \
        int x = 0;                                                  \
        ICC_MEAN_SDV_MASK_COI_CASE(worktype, sqsumtype,            \
                                    CC_SQR, size.width, cn);       \
    }                                                               \
                                                                    \
    ICC_MEAN_SDV_EXIT_C1(s, sq);                                  \
    return CC_OK;                                                   \
}


#define ICC_DEF_MEAN_SDV_BLOCK_ALL(flavor, arrtype, sumtype, sqsumtype,\
                            worktype, sqworktype, block_size, sqr_macro)\
ICC_DEF_MEAN_SDV_BLOCK_FUNC_2D(flavor, 1, arrtype, sumtype, sqsumtype, \
                            worktype, sqworktype, block_size, sqr_macro)\
ICC_DEF_MEAN_SDV_BLOCK_FUNC_2D(flavor, 2, arrtype, sumtype, sqsumtype, \
                            worktype, sqworktype, block_size, sqr_macro)\
ICC_DEF_MEAN_SDV_BLOCK_FUNC_2D(flavor, 3, arrtype, sumtype, sqsumtype, \
                            worktype, sqworktype, block_size, sqr_macro)\
ICC_DEF_MEAN_SDV_BLOCK_FUNC_2D(flavor, 4, arrtype, sumtype, sqsumtype, \
                            worktype, sqworktype, block_size, sqr_macro)\
ICC_DEF_MEAN_SDV_BLOCK_FUNC_2D_COI(flavor, arrtype, sumtype, sqsumtype,\
                            worktype, sqworktype, block_size, sqr_macro)\
                                                                        \
ICC_DEF_MEAN_SDV_MASK_BLOCK_FUNC_2D(flavor, 1, arrtype, sumtype,       \
            sqsumtype, worktype, sqworktype, block_size, sqr_macro)    \
ICC_DEF_MEAN_SDV_MASK_BLOCK_FUNC_2D(flavor, 2, arrtype, sumtype,       \
            sqsumtype, worktype, sqworktype, block_size, sqr_macro)    \
ICC_DEF_MEAN_SDV_MASK_BLOCK_FUNC_2D(flavor, 3, arrtype, sumtype,       \
            sqsumtype, worktype, sqworktype, block_size, sqr_macro)    \
ICC_DEF_MEAN_SDV_MASK_BLOCK_FUNC_2D(flavor, 4, arrtype, sumtype,       \
            sqsumtype, worktype, sqworktype, block_size, sqr_macro)    \
ICC_DEF_MEAN_SDV_MASK_BLOCK_FUNC_2D_COI(flavor, arrtype, sumtype,      \
            sqsumtype, worktype, sqworktype, block_size, sqr_macro)

#define ICC_DEF_MEAN_SDV_ALL(flavor, arrtype, sumtype, sqsumtype, worktype)   \
ICC_DEF_MEAN_SDV_FUNC_2D(flavor, 1, arrtype, sumtype, sqsumtype, worktype)    \
ICC_DEF_MEAN_SDV_FUNC_2D(flavor, 2, arrtype, sumtype, sqsumtype, worktype)    \
ICC_DEF_MEAN_SDV_FUNC_2D(flavor, 3, arrtype, sumtype, sqsumtype, worktype)    \
ICC_DEF_MEAN_SDV_FUNC_2D(flavor, 4, arrtype, sumtype, sqsumtype, worktype)    \
ICC_DEF_MEAN_SDV_FUNC_2D_COI(flavor, arrtype, sumtype, sqsumtype, worktype)   \
                                                                                \
ICC_DEF_MEAN_SDV_MASK_FUNC_2D(flavor, 1, arrtype, sumtype, sqsumtype, worktype) \
ICC_DEF_MEAN_SDV_MASK_FUNC_2D(flavor, 2, arrtype, sumtype, sqsumtype, worktype) \
ICC_DEF_MEAN_SDV_MASK_FUNC_2D(flavor, 3, arrtype, sumtype, sqsumtype, worktype) \
ICC_DEF_MEAN_SDV_MASK_FUNC_2D(flavor, 4, arrtype, sumtype, sqsumtype, worktype) \
ICC_DEF_MEAN_SDV_MASK_FUNC_2D_COI(flavor, arrtype, sumtype, sqsumtype, worktype)


ICC_DEF_MEAN_SDV_BLOCK_ALL(8u, uchar, int64, int64, unsigned, unsigned, 1 << 16, CC_SQR_8U)
ICC_DEF_MEAN_SDV_BLOCK_ALL(16u, ushort, int64, int64, unsigned, int64, 1 << 16, CC_SQR)
ICC_DEF_MEAN_SDV_BLOCK_ALL(16s, short, int64, int64, int, int64, 1 << 16, CC_SQR)

ICC_DEF_MEAN_SDV_ALL(32s, int, double, double, double)
ICC_DEF_MEAN_SDV_ALL(32f, float, double, double, double)
ICC_DEF_MEAN_SDV_ALL(64f, double, double, double, double)

#define icvMean_StdDev_8s_C1R  0
#define icvMean_StdDev_8s_C2R  0
#define icvMean_StdDev_8s_C3R  0
#define icvMean_StdDev_8s_C4R  0
#define icvMean_StdDev_8s_CnCR 0

#define icvMean_StdDev_8s_C1MR  0
#define icvMean_StdDev_8s_C2MR  0
#define icvMean_StdDev_8s_C3MR  0
#define icvMean_StdDev_8s_C4MR  0
#define icvMean_StdDev_8s_CnCMR 0

CC_DEF_INIT_BIG_FUNC_TAB_2D(Mean_StdDev, R)
CC_DEF_INIT_FUNC_TAB_2D(Mean_StdDev, CnCR)
CC_DEF_INIT_BIG_FUNC_TAB_2D(Mean_StdDev, MR)
CC_DEF_INIT_FUNC_TAB_2D(Mean_StdDev, CnCMR)

#define cvMeanStdDev cvAvgSdv
CC_IMPL  void
cvAvgSdv(const img_t* mat, double4 _mean, double4 _sdv, const img_t* mask CC_DEFAULT(NULL))
{
    CScalar mean = {{0,0,0,0}};
    CScalar sdv = {{0,0,0,0}};

    static CvBigFuncTable meansdv_tab;
    static CvFuncTable meansdvcoi_tab;
    static CvBigFuncTable meansdvmask_tab;
    static CvFuncTable meansdvmaskcoi_tab;
    static int inittab = 0;

    CC_FUNCNAME("cvMean_StdDev");

    __BEGIN__;

    TypeId type;
    int coi = 0;
    int mat_step, mask_step = 0;
    CSize size;
    int cn = CC_MAT_CN(mat);

    if(!inittab)
    {
        icvInitMean_StdDevRTable(meansdv_tab);
        icvInitMean_StdDevCnCRTable(meansdvcoi_tab);
        icvInitMean_StdDevMRTable(meansdvmask_tab);
        icvInitMean_StdDevCnCMRTable(meansdvmaskcoi_tab);
        inittab = 1;
    }

    type = CC_MAT_DEPTH(mat);

    if(CC_MAT_CN(mat) > 4 && coi == 0)
        CC_ERROR(CC_StsOutOfRange, "The input array must have at most 4 channels unless COI is set");

    size = cvGetMatSize(mat);
    mat_step = mat->step;

    if(!mask)
    {
        if(CC_IS_MAT_CONT(mat))
        {
            size.width *= size.height;
            size.height = 1;
            mat_step = CC_STUB_STEP;
        }

        if(CC_MAT_CN(mat) == 1 || coi == 0)
        {
            CvFunc2D_1A2P func = (CvFunc2D_1A2P)(meansdv_tab[type][cn]);

            if(!func)
                CC_ERROR(CC_StsBadArg, cvUnsupportedFormat);

            IPPI_CALL(func(mat->tt.data, mat_step, size, mean.val, sdv.val));
        }
        else
        {
            CvFunc2DnC_1A2P func = (CvFunc2DnC_1A2P)
                (meansdvcoi_tab[type]);

            if(!func)
                CC_ERROR(CC_StsBadArg, cvUnsupportedFormat);

            IPPI_CALL(func(mat->tt.data, mat_step, size, cn, coi, mean.val, sdv.val));
        }
    }
    else
    {
        mask_step = mask->step;

        if(!CC_IS_MASK_ARR(mask))
            CC_ERROR(CC_StsBadMask, "");

        if(!CC_ARE_SIZES_EQ(mat, mask))
            CC_ERROR(CC_StsUnmatchedSizes, "");

        if(CC_IS_MAT_CONT(mat) && CC_IS_MAT_CONT(mask))
        {
            size.width *= size.height;
            size.height = 1;
            mat_step = mask_step = CC_STUB_STEP;
        }

        if(cn == 1 || coi == 0)
        {
            CvFunc2D_2A2P func = (CvFunc2D_2A2P)(meansdvmask_tab[type][cn]);

            if(!func)
                CC_ERROR(CC_StsBadArg, cvUnsupportedFormat);

            IPPI_CALL(func(mat->tt.data, mat_step, mask->tt.data,
                             mask_step, size, mean.val, sdv.val));
        }
        else
        {
            CvFunc2DnC_2A2P func = (CvFunc2DnC_2A2P)
                (meansdvmaskcoi_tab[type]);

            if(!func)
                CC_ERROR(CC_StsBadArg, cvUnsupportedFormat);

            IPPI_CALL(func(mat->tt.data, mat_step,
                             mask->tt.data, mask_step,
                             size, cn, coi, mean.val, sdv.val));
        }
    }

    __END__;

    if(_mean) COPY4(_mean, mean.val);

    if(_sdv) COPY4(_sdv, sdv.val);
}



