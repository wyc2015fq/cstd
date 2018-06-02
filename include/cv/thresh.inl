
static CStatus C_STDCALL
icvThresh_8u_C1R(const uchar* src, int src_step, uchar* dst, int dst_step,
                  CSize roi, uchar thresh, uchar maxval, int type)
{
    int i, j;
    uchar tab[256];

    switch(type)
    {
    case CC_THRESH_BINARY:
        for(i = 0; i <= thresh; i++)
            tab[i] = 0;
        for(; i < 256; i++)
            tab[i] = maxval;
        break;
    case CC_THRESH_BINARY_INV:
        for(i = 0; i <= thresh; i++)
            tab[i] = maxval;
        for(; i < 256; i++)
            tab[i] = 0;
        break;
    case CC_THRESH_TRUNC:
        for(i = 0; i <= thresh; i++)
            tab[i] = (uchar)i;
        for(; i < 256; i++)
            tab[i] = thresh;
        break;
    case CC_THRESH_TOZERO:
        for(i = 0; i <= thresh; i++)
            tab[i] = 0;
        for(; i < 256; i++)
            tab[i] = (uchar)i;
        break;
    case CC_THRESH_TOZERO_INV:
        for(i = 0; i <= thresh; i++)
            tab[i] = (uchar)i;
        for(; i < 256; i++)
            tab[i] = 0;
        break;
    default:
        return CC_BADFLAG_ERR;
    }

    for(i = 0; i < roi.height; i++, src += src_step, dst += dst_step)
    {
        for(j = 0; j <= roi.width - 4; j += 4)
        {
            uchar t0 = tab[src[j]];
            uchar t1 = tab[src[j+1]];

            dst[j] = t0;
            dst[j+1] = t1;

            t0 = tab[src[j+2]];
            t1 = tab[src[j+3]];

            dst[j+2] = t0;
            dst[j+3] = t1;
        }

        for(; j < roi.width; j++)
            dst[j] = tab[src[j]];
    }

    return CC_NO_ERR;
}


static CStatus C_STDCALL
icvThresh_32f_C1R(const float *src, int src_step, float *dst, int dst_step,
                   CSize roi, float thresh, float maxval, int type)
{
    int i, j;
    const int* isrc = (const int*)src;
    int* idst = (int*)dst;
    suf32_t v;
    int iThresh, iMax;

    v.f = thresh; iThresh = CC_TOGGLE_FLT(v.i);
    v.f = maxval; iMax = v.i;

    src_step /= sizeof(src[0]);
    dst_step /= sizeof(dst[0]);

    switch(type)
    {
    case CC_THRESH_BINARY:
        for(i = 0; i < roi.height; i++, isrc += src_step, idst += dst_step)
        {
            for(j = 0; j < roi.width; j++)
            {
                int temp = isrc[j];
                idst[j] = ((CC_TOGGLE_FLT(temp) <= iThresh) - 1) & iMax;
            }
        }
        break;

    case CC_THRESH_BINARY_INV:
        for(i = 0; i < roi.height; i++, isrc += src_step, idst += dst_step)
        {
            for(j = 0; j < roi.width; j++)
            {
                int temp = isrc[j];
                idst[j] = ((CC_TOGGLE_FLT(temp) > iThresh) - 1) & iMax;
            }
        }
        break;

    case CC_THRESH_TRUNC:
        for(i = 0; i < roi.height; i++, src += src_step, dst += dst_step)
        {
            for(j = 0; j < roi.width; j++)
            {
                float temp = src[j];

                if(temp > thresh)
                    temp = thresh;
                dst[j] = temp;
            }
        }
        break;

    case CC_THRESH_TOZERO:
        for(i = 0; i < roi.height; i++, isrc += src_step, idst += dst_step)
        {
            for(j = 0; j < roi.width; j++)
            {
                int temp = isrc[j];
                idst[j] = ((CC_TOGGLE_FLT(temp) <= iThresh) - 1) & temp;
            }
        }
        break;

    case CC_THRESH_TOZERO_INV:
        for(i = 0; i < roi.height; i++, isrc += src_step, idst += dst_step)
        {
            for(j = 0; j < roi.width; j++)
            {
                int temp = isrc[j];
                idst[j] = ((CC_TOGGLE_FLT(temp) > iThresh) - 1) & temp;
            }
        }
        break;

    default:
        return CC_BADFLAG_ERR;
    }

    return CC_OK;
}


static double
icvGetThreshVal_Otsu(const CvHistogram* hist)
{
    double max_val = 0;
    
    CC_FUNCNAME("icvGetThreshVal_Otsu");

    __BEGIN__;

    int i, count;
    const float* h;
    double sum = 0, mu = 0;
    bool uniform = false;
    double low = 0, high = 0, delta = 0;
    float* nu_thresh = 0;
    double mu1 = 0, q1 = 0;
    double max_sigma = 0;

    if(!CC_IS_HIST(hist) || CC_IS_SPARSE_HIST(hist) || hist->mat.dims != 1)
        CC_ERROR(CC_StsBadArg,
        "The histogram in Otsu method must be a valid dense 1D histogram");

    count = hist->mat.dim[0].size;
    h = (float*)cvPtr1D(hist->bins, 0);

    if(!CC_HIST_HAS_RANGES(hist) || CC_IS_UNIFORM_HIST(hist))
    {
        if(CC_HIST_HAS_RANGES(hist))
        {
            low = hist->thresh[0][0];
            high = hist->thresh[0][1];
        }
        else
        {
            low = 0;
            high = count;
        }

        delta = (high-low)/count;
        low += delta*0.5;
        uniform = true;
    }
    else
        nu_thresh = hist->thresh2[0];

    for(i = 0; i < count; i++)
    {
        sum += h[i];
        if(uniform)
            mu += (i*delta + low)*h[i];
        else
            mu += (nu_thresh[i*2] + nu_thresh[i*2+1])*0.5*h[i];
    }
    
    sum = fabs(sum) > FLT_EPSILON ? 1./sum : 0;
    mu *= sum;

    mu1 = 0;
    q1 = 0;

    for(i = 0; i < count; i++)
    {
        double p_i, q2, mu2, val_i, sigma;

        p_i = h[i]*sum;
        mu1 *= q1;
        q1 += p_i;
        q2 = 1. - q1;

        if(MIN(q1,q2) < FLT_EPSILON || MAX(q1,q2) > 1. - FLT_EPSILON)
            continue;

        if(uniform)
            val_i = i*delta + low;
        else
            val_i = (nu_thresh[i*2] + nu_thresh[i*2+1])*0.5;

        mu1 = (mu1 + val_i*p_i)/q1;
        mu2 = (mu - q1*mu1)/q2;
        sigma = q1*q2*(mu1 - mu2)*(mu1 - mu2);
        if(sigma > max_sigma)
        {
            max_sigma = sigma;
            max_val = val_i;
        }
    }

    __END__;

    return max_val;
}


icvAndC_8u_C1R_t icvAndC_8u_C1R_p = 0;
icvCompareC_8u_C1R_cv_t icvCompareC_8u_C1R_cv_p = 0;
icvThreshold_GTVal_8u_C1R_t icvThreshold_GTVal_8u_C1R_p = 0;
icvThreshold_GTVal_32f_C1R_t icvThreshold_GTVal_32f_C1R_p = 0;
icvThreshold_LTVal_8u_C1R_t icvThreshold_LTVal_8u_C1R_p = 0;
icvThreshold_LTVal_32f_C1R_t icvThreshold_LTVal_32f_C1R_p = 0;

CC_IMPL void
cvThreshold(const img_t* src, img_t* dst, double thresh, double maxval, int type)
{
    CvHistogram* hist = 0;
    
    CC_FUNCNAME("cvThreshold");

    __BEGIN__;

    CSize roi;
    int src_step, dst_step;
    img_t src_stub, *src = srcarr;
    img_t dst_stub, *dst = dstarr;
    img_t src0, dst0;
    int coi1 = 0, coi2 = 0;
    int ithresh, imaxval, cn;
    bool use_otsu;

    CC_CALL(src = cvGetMat(src, &src_stub, &coi1));
    CC_CALL(dst = cvGetMat(dst, &dst_stub, &coi2));

    if(coi1 + coi2)
        CC_ERROR(CC_BadCOI, "COI is not supported by the function");

    if(!CC_ARE_CNS_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "Both arrays must have equal number of channels");

    cn = CC_MAT_CN(src);
    if(cn > 1)
    {
        src = cvReshape(src, &src0, 1);
        dst = cvReshape(dst, &dst0, 1);
    }

    use_otsu = (type & ~CC_THRESH_MASK) == CC_THRESH_OTSU;
    type &= CC_THRESH_MASK;

    if(use_otsu)
    {
        float _ranges[] = { 0, 256 };
        float* ranges = _ranges;
        int hist_size = 256;
        void* srcarr0 = src;

        if(CC_MAT_TYPE(src) != CC_8UC1)
            CC_ERROR(CC_StsNotImplemented, "Otsu method can only be used with 8uC1 images");

        CC_CALL(hist = cvCreateHist(1, &hist_size, CC_HIST_ARRAY, &ranges));
        cvCalcArrHist(&srcarr0, hist);
        thresh = cFloor(icvGetThreshVal_Otsu(hist));
    }

    if(!CC_ARE_DEPTHS_EQ(src, dst))
    {
        if(CC_MAT_TYPE(dst) != CC_8UC1)
            CC_ERROR(CC_StsUnsupportedFormat, "In case of different types destination should be 8uC1");

        if(type != CC_THRESH_BINARY && type != CC_THRESH_BINARY_INV)
            CC_ERROR(CC_StsBadArg,
            "In case of different types only CC_THRESH_BINARY "
            "and CC_THRESH_BINARY_INV thresholding types are supported");

        if(maxval < 0)
        {
            CC_CALL(cvSetZero(dst));
        }
        else
        {
            CC_CALL(cvCmpS(src, thresh, dst, type == CC_THRESH_BINARY ? CC_CMP_GT : CC_CMP_LE));
            if(maxval < 255)
                CC_CALL(cvAndS(dst, cScalarAll(maxval), dst));
        }
        EXIT;
    }

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    roi = cvGetMatSize(src);
    if(CC_IS_MAT_CONT(src) && CC_IS_MAT_CONT(dst))
    {
        roi.width *= roi.height;
        roi.height = 1;
        src_step = dst_step = CC_STUB_STEP;
    }
    else
    {
        src_step = src->step;
        dst_step = dst->step;
    }

    switch(CC_MAT_TYPE(src))
    {
    case CC_8U:
        
        ithresh = cFloor(thresh);
        imaxval = cRound(maxval);
        if(type == CC_THRESH_TRUNC)
            imaxval = ithresh;
        imaxval = CC_CAST_8U(imaxval);

        if(ithresh < 0 || ithresh >= 255)
        {
            if(type == CC_THRESH_BINARY || type == CC_THRESH_BINARY_INV ||
                (type == CC_THRESH_TRUNC || type == CC_THRESH_TOZERO_INV) && ithresh < 0 ||
                type == CC_THRESH_TOZERO && ithresh >= 255)
            {
                int v = type == CC_THRESH_BINARY ? (ithresh >= 255 ? 0 : imaxval) :
                        type == CC_THRESH_BINARY_INV ? (ithresh >= 255 ? imaxval : 0) :
                        type == CC_THRESH_TRUNC ? imaxval : 0;

                cvSet(dst, cScalarAll(v));
                EXIT;
            }
            else
            {
                cvCopy(src, dst);
                EXIT;
            }
        }

        if(type == CC_THRESH_BINARY || type == CC_THRESH_BINARY_INV)
        {
            if(icvCompareC_8u_C1R_cv_p && icvAndC_8u_C1R_p)
            {
                IPPI_CALL(icvCompareC_8u_C1R_cv_p(src->tt.data, src_step,
                    (uchar)ithresh, dst->tt.data, dst_step, roi,
                    type == CC_THRESH_BINARY ? cvCmpGreater : cvCmpLessEq));

                if(imaxval < 255)
                    IPPI_CALL(icvAndC_8u_C1R_p(dst->tt.data, dst_step,
                    (uchar)imaxval, dst->tt.data, dst_step, roi));
                EXIT;
            }
        }
        else if(type == CC_THRESH_TRUNC || type == CC_THRESH_TOZERO_INV)
        {
            if(icvThreshold_GTVal_8u_C1R_p)
            {
                IPPI_CALL(icvThreshold_GTVal_8u_C1R_p(src->tt.data, src_step,
                    dst->tt.data, dst_step, roi, (uchar)ithresh,
                    (uchar)(type == CC_THRESH_TRUNC ? ithresh : 0)));
                EXIT;
            }
        }
        else
        {
            assert(type == CC_THRESH_TOZERO);
            if(icvThreshold_LTVal_8u_C1R_p)
            {
                ithresh = cFloor(thresh+1.);
                ithresh = CC_CAST_8U(ithresh);
                IPPI_CALL(icvThreshold_LTVal_8u_C1R_p(src->tt.data, src_step,
                    dst->tt.data, dst_step, roi, (uchar)ithresh, 0));
                EXIT;
            }
        }

        icvThresh_8u_C1R(src->tt.data, src_step,
                          dst->tt.data, dst_step, roi,
                          (uchar)ithresh, (uchar)imaxval, type);
        break;
    case CC_32F:

        if(type == CC_THRESH_TRUNC || type == CC_THRESH_TOZERO_INV)
        {
            if(icvThreshold_GTVal_32f_C1R_p)
            {
                IPPI_CALL(icvThreshold_GTVal_32f_C1R_p(src->tt.fl, src_step,
                    dst->tt.fl, dst_step, roi, (float)thresh,
                    type == CC_THRESH_TRUNC ? (float)thresh : 0));
                EXIT;
            }
        }
        else if(type == CC_THRESH_TOZERO)
        {
            if(icvThreshold_LTVal_32f_C1R_p)
            {
                IPPI_CALL(icvThreshold_LTVal_32f_C1R_p(src->tt.fl, src_step,
                    dst->tt.fl, dst_step, roi, (float)(thresh*(1 + FLT_EPSILON)), 0));
                EXIT;
            }
        }

        icvThresh_32f_C1R(src->tt.fl, src_step, dst->tt.fl, dst_step, roi,
                           (float)thresh, (float)maxval, type);
        break;
    default:
        CC_ERROR(CC_BadDepth, cvUnsupportedFormat);
    }

    __END__;

    if(hist)
        cvReleaseHist(&hist);
}


