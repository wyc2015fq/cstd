

#define ICC_DIST_SHIFT  16
#define ICC_INIT_DIST0  (INT_MAX >> 2)

static CStatus
icvInitTopBottom(int* temp, int tempstep, CSize size, int border)
{
    int i, j;
    for(i = 0; i < border; i++)
    {
        int* ttop = (int*)(temp + i*tempstep);
        int* tbottom = (int*)(temp + (size.height + border*2 - i - 1)*tempstep);
        
        for(j = 0; j < size.width + border*2; j++)
        {
            ttop[j] = ICC_INIT_DIST0;
            tbottom[j] = ICC_INIT_DIST0;
        }
    }

    return CC_OK;
}


static CStatus C_STDCALL
icvDistanceTransform_3x3_C1R(const uchar* src, int srcstep, int* temp,
        int step, float* dist, int dststep, CSize size, const float* metrics)
{
    const int BORDER = 1;
    int i, j;
    const int HV_DIST = CC_FLT_TO_FIX(metrics[0], ICC_DIST_SHIFT);
    const int DIAG_DIST = CC_FLT_TO_FIX(metrics[1], ICC_DIST_SHIFT);
    const float scale = 1.f/(1 << ICC_DIST_SHIFT);

    srcstep /= sizeof(src[0]);
    step /= sizeof(temp[0]);
    dststep /= sizeof(dist[0]);

    icvInitTopBottom(temp, step, size, BORDER);

    // forward pass
    for(i = 0; i < size.height; i++)
    {
        const uchar* s = src + i*srcstep;
        int* tmp = (int*)(temp + (i+BORDER)*step) + BORDER;

        for(j = 0; j < BORDER; j++)
            tmp[-j-1] = tmp[size.width + j] = ICC_INIT_DIST0;
        
        for(j = 0; j < size.width; j++)
        {
            if(!s[j])
                tmp[j] = 0;
            else
            {
                int t0 = tmp[j-step-1] + DIAG_DIST;
                int t = tmp[j-step] + HV_DIST;
                if(t0 > t) t0 = t;
                t = tmp[j-step+1] + DIAG_DIST;
                if(t0 > t) t0 = t;
                t = tmp[j-1] + HV_DIST;
                if(t0 > t) t0 = t;
                tmp[j] = t0;
            }
        }
    }

    // backward pass
    for(i = size.height - 1; i >= 0; i--)
    {
        float* d = (float*)(dist + i*dststep);
        int* tmp = (int*)(temp + (i+BORDER)*step) + BORDER;
        
        for(j = size.width - 1; j >= 0; j--)
        {
            int t0 = tmp[j];
            if(t0 > HV_DIST)
            {
                int t = tmp[j+step+1] + DIAG_DIST;
                if(t0 > t) t0 = t;
                t = tmp[j+step] + HV_DIST;
                if(t0 > t) t0 = t;
                t = tmp[j+step-1] + DIAG_DIST;
                if(t0 > t) t0 = t;
                t = tmp[j+1] + HV_DIST;
                if(t0 > t) t0 = t;
                tmp[j] = t0;
            }
            d[j] = (float)(t0 * scale);
        }
    }

    return CC_OK;
}


static CStatus C_STDCALL
icvDistanceTransform_5x5_C1R(const uchar* src, int srcstep, int* temp,
        int step, float* dist, int dststep, CSize size, const float* metrics)
{
    const int BORDER = 2;
    int i, j;
    const int HV_DIST = CC_FLT_TO_FIX(metrics[0], ICC_DIST_SHIFT);
    const int DIAG_DIST = CC_FLT_TO_FIX(metrics[1], ICC_DIST_SHIFT);
    const int LONG_DIST = CC_FLT_TO_FIX(metrics[2], ICC_DIST_SHIFT);
    const float scale = 1.f/(1 << ICC_DIST_SHIFT);

    srcstep /= sizeof(src[0]);
    step /= sizeof(temp[0]);
    dststep /= sizeof(dist[0]);

    icvInitTopBottom(temp, step, size, BORDER);

    // forward pass
    for(i = 0; i < size.height; i++)
    {
        const uchar* s = src + i*srcstep;
        int* tmp = (int*)(temp + (i+BORDER)*step) + BORDER;

        for(j = 0; j < BORDER; j++)
            tmp[-j-1] = tmp[size.width + j] = ICC_INIT_DIST0;
        
        for(j = 0; j < size.width; j++)
        {
            if(!s[j])
                tmp[j] = 0;
            else
            {
                int t0 = tmp[j-step*2-1] + LONG_DIST;
                int t = tmp[j-step*2+1] + LONG_DIST;
                if(t0 > t) t0 = t;
                t = tmp[j-step-2] + LONG_DIST;
                if(t0 > t) t0 = t;
                t = tmp[j-step-1] + DIAG_DIST;
                if(t0 > t) t0 = t;
                t = tmp[j-step] + HV_DIST;
                if(t0 > t) t0 = t;
                t = tmp[j-step+1] + DIAG_DIST;
                if(t0 > t) t0 = t;
                t = tmp[j-step+2] + LONG_DIST;
                if(t0 > t) t0 = t;
                t = tmp[j-1] + HV_DIST;
                if(t0 > t) t0 = t;
                tmp[j] = t0;
            }
        }
    }

    // backward pass
    for(i = size.height - 1; i >= 0; i--)
    {
        float* d = (float*)(dist + i*dststep);
        int* tmp = (int*)(temp + (i+BORDER)*step) + BORDER;
        
        for(j = size.width - 1; j >= 0; j--)
        {
            int t0 = tmp[j];
            if(t0 > HV_DIST)
            {
                int t = tmp[j+step*2+1] + LONG_DIST;
                if(t0 > t) t0 = t;
                t = tmp[j+step*2-1] + LONG_DIST;
                if(t0 > t) t0 = t;
                t = tmp[j+step+2] + LONG_DIST;
                if(t0 > t) t0 = t;
                t = tmp[j+step+1] + DIAG_DIST;
                if(t0 > t) t0 = t;
                t = tmp[j+step] + HV_DIST;
                if(t0 > t) t0 = t;
                t = tmp[j+step-1] + DIAG_DIST;
                if(t0 > t) t0 = t;
                t = tmp[j+step-2] + LONG_DIST;
                if(t0 > t) t0 = t;
                t = tmp[j+1] + HV_DIST;
                if(t0 > t) t0 = t;
                tmp[j] = t0;
            }
            d[j] = (float)(t0 * scale);
        }
    }

    return CC_OK;
}


static CStatus C_STDCALL
icvDistanceTransformEx_5x5_C1R(const uchar* src, int srcstep, int* temp,
                int step, float* dist, int dststep, int* labels, int lstep,
                CSize size, const float* metrics)
{
    const int BORDER = 2;
    
    int i, j;
    const int HV_DIST = CC_FLT_TO_FIX(metrics[0], ICC_DIST_SHIFT);
    const int DIAG_DIST = CC_FLT_TO_FIX(metrics[1], ICC_DIST_SHIFT);
    const int LONG_DIST = CC_FLT_TO_FIX(metrics[2], ICC_DIST_SHIFT);
    const float scale = 1.f/(1 << ICC_DIST_SHIFT);

    srcstep /= sizeof(src[0]);
    step /= sizeof(temp[0]);
    dststep /= sizeof(dist[0]);
    lstep /= sizeof(labels[0]);

    icvInitTopBottom(temp, step, size, BORDER);

    // forward pass
    for(i = 0; i < size.height; i++)
    {
        const uchar* s = src + i*srcstep;
        int* tmp = (int*)(temp + (i+BORDER)*step) + BORDER;
        int* lls = (int*)(labels + i*lstep);

        for(j = 0; j < BORDER; j++)
            tmp[-j-1] = tmp[size.width + j] = ICC_INIT_DIST0;
        
        for(j = 0; j < size.width; j++)
        {
            if(!s[j])
            {
                tmp[j] = 0;
                //assert(lls[j] != 0);
            }
            else
            {
                int t0 = ICC_INIT_DIST0, t;
                int l0 = 0;

                t = tmp[j-step*2-1] + LONG_DIST;
                if(t0 > t)
                {
                    t0 = t;
                    l0 = lls[j-lstep*2-1];
                }
                t = tmp[j-step*2+1] + LONG_DIST;
                if(t0 > t)
                {
                    t0 = t;
                    l0 = lls[j-lstep*2+1];
                }
                t = tmp[j-step-2] + LONG_DIST;
                if(t0 > t)
                {
                    t0 = t;
                    l0 = lls[j-lstep-2];
                }
                t = tmp[j-step-1] + DIAG_DIST;
                if(t0 > t)
                {
                    t0 = t;
                    l0 = lls[j-lstep-1];
                }
                t = tmp[j-step] + HV_DIST;
                if(t0 > t)
                {
                    t0 = t;
                    l0 = lls[j-lstep];
                }
                t = tmp[j-step+1] + DIAG_DIST;
                if(t0 > t)
                {
                    t0 = t;
                    l0 = lls[j-lstep+1];
                }
                t = tmp[j-step+2] + LONG_DIST;
                if(t0 > t)
                {
                    t0 = t;
                    l0 = lls[j-lstep+2];
                }
                t = tmp[j-1] + HV_DIST;
                if(t0 > t)
                {
                    t0 = t;
                    l0 = lls[j-1];
                }

                tmp[j] = t0;
                lls[j] = l0;
            }
        }
    }

    // backward pass
    for(i = size.height - 1; i >= 0; i--)
    {
        float* d = (float*)(dist + i*dststep);
        int* tmp = (int*)(temp + (i+BORDER)*step) + BORDER;
        int* lls = (int*)(labels + i*lstep);
        
        for(j = size.width - 1; j >= 0; j--)
        {
            int t0 = tmp[j];
            int l0 = lls[j];
            if(t0 > HV_DIST)
            {
                int t = tmp[j+step*2+1] + LONG_DIST;
                if(t0 > t)
                {
                    t0 = t;
                    l0 = lls[j+lstep*2+1];
                }
                t = tmp[j+step*2-1] + LONG_DIST;
                if(t0 > t)
                {
                    t0 = t;
                    l0 = lls[j+lstep*2-1];
                }
                t = tmp[j+step+2] + LONG_DIST;
                if(t0 > t)
                {
                    t0 = t;
                    l0 = lls[j+lstep+2];
                }
                t = tmp[j+step+1] + DIAG_DIST;
                if(t0 > t)
                {
                    t0 = t;
                    l0 = lls[j+lstep+1];
                }
                t = tmp[j+step] + HV_DIST;
                if(t0 > t)
                {
                    t0 = t;
                    l0 = lls[j+lstep];
                }
                t = tmp[j+step-1] + DIAG_DIST;
                if(t0 > t)
                {
                    t0 = t;
                    l0 = lls[j+lstep-1];
                }
                t = tmp[j+step-2] + LONG_DIST;
                if(t0 > t)
                {
                    t0 = t;
                    l0 = lls[j+lstep-2];
                }
                t = tmp[j+1] + HV_DIST;
                if(t0 > t)
                {
                    t0 = t;
                    l0 = lls[j+1];
                }
                tmp[j] = t0;
                lls[j] = l0;
            }
            d[j] = (float)(t0 * scale);
        }
    }

    return CC_OK;
}


static CStatus
icvGetDistanceTransformMask(int maskType, float *metrics)
{
    if(!metrics)
        return CC_NULLPTR_ERR;

    switch (maskType)
    {
    case 30:
        metrics[0] = 1.0f;
        metrics[1] = 1.0f;
        break;

    case 31:
        metrics[0] = 1.0f;
        metrics[1] = 2.0f;
        break;

    case 32:
        metrics[0] = 0.955f;
        metrics[1] = 1.3693f;
        break;

    case 50:
        metrics[0] = 1.0f;
        metrics[1] = 1.0f;
        metrics[2] = 2.0f;
        break;

    case 51:
        metrics[0] = 1.0f;
        metrics[1] = 2.0f;
        metrics[2] = 3.0f;
        break;

    case 52:
        metrics[0] = 1.0f;
        metrics[1] = 1.4f;
        metrics[2] = 2.1969f;
        break;
    default:
        return CC_BADRANGE_ERR;
    }

    return CC_OK;
}


static void
icvTrueDistTrans(const img_t* src, img_t* dst)
{
    img_t buffer[1] = {0};

    CC_FUNCNAME("cvDistTransform2");

    __BEGIN__;

    int i, m, n;
    int sstep, dstep;
    const float inf = 1e6f;
    int thread_count = cGetNumThreads();
    int pass1_sz, pass2_sz;

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    if(CC_MAT_TYPE(src) != CC_8U || CC_MAT_TYPE(dst) != CC_32F)
        CC_ERROR(CC_StsUnsupportedFormat,
        "The input image must have 8uC1 type and the output one must have 32fC1 type");

    m = src->rows;
    n = src->cols;

    // (see stage 1 below):
    // sqr_tab: 2*m, sat_tab: 3*m + 1, d: m*thread_count,
    pass1_sz = src->rows*(5 + thread_count) + 1;
    // (see stage 2):
    // sqr_tab & inv_tab: n each; f & v: n*thread_count each; z: (n+1)*thread_count
    pass2_sz = src->cols*(2 + thread_count*3) + thread_count;
    CC_CALL(cvCreateMat(buffer, 1, MAX(pass1_sz, pass2_sz), CC_32F, 1));

    sstep = src->step;
    dstep = dst->step / sizeof(float);

    // stage 1: compute 1d distance transform of each column
    {
    float* sqr_tab = buffer->tt.fl;
    int* sat_tab = (int*)(sqr_tab + m*2);
    const int shift = m*2;

    for(i = 0; i < m; i++)
        sqr_tab[i] = (float)(i*i);
    for(i = m; i < m*2; i++)
        sqr_tab[i] = inf;
    for(i = 0; i < shift; i++)
        sat_tab[i] = 0;
    for(; i <= m*3; i++)
        sat_tab[i] = i - shift;

#ifdef _OPENMP
    #pragma omp parallel for num_threads(thread_count)
#endif
    for(i = 0; i < n; i++)
    {
        const uchar* sptr = src->tt.data + i + (m-1)*sstep;
        float* dptr = dst->tt.fl + i;
        int* d = (int*)(sat_tab + m*3+1+m*cGetThreadNum());
        int j, dist = m-1;

        for(j = m-1; j >= 0; j--, sptr -= sstep)
        {
            dist = (dist + 1) & (sptr[0] == 0 ? 0 : -1);
            d[j] = dist;
        }

        dist = m-1;
        for(j = 0; j < m; j++, dptr += dstep)
        {
            dist = dist + 1 - sat_tab[dist + 1 - d[j] + shift];
            d[j] = dist;
            dptr[0] = sqr_tab[dist];
        }
    }
    }

    // stage 2: compute modified distance transform for each row
    {
    float* inv_tab = buffer->tt.fl;
    float* sqr_tab = inv_tab + n;

    inv_tab[0] = sqr_tab[0] = 0.f;
    for(i = 1; i < n; i++)
    {
        inv_tab[i] = (float)(0.5/i);
        sqr_tab[i] = (float)(i*i);
    }

#ifdef _OPENMP
    #pragma omp parallel for num_threads(thread_count), schedule(dynamic)
#endif
    for(i = 0; i < m; i++)
    {
        float* d = (float*)(dst->tt.data + i*dst->step);
        float* f = sqr_tab + n + (n*3+1)*cGetThreadNum();
        float* z = f + n;
        int* v = (int*)(z + n + 1);
        int p, q, k;

        v[0] = 0;
        z[0] = -inf;
        z[1] = inf;
        f[0] = d[0];

        for(q = 1, k = 0; q < n; q++)
        {
            float fq = d[q];
            f[q] = fq;

            for(;;k--)
            {
                p = v[k];
                float s = (fq + sqr_tab[q] - d[p] - sqr_tab[p])*inv_tab[q - p];
                if(s > z[k])
                {
                    k++;
                    v[k] = q;
                    z[k] = s;
                    z[k+1] = inf;
                    break;
                }
            }
        }

        for(q = 0, k = 0; q < n; q++)
        {
            while(z[k+1] < q)
                k++;
            p = v[k];
            d[q] = sqr_tab[abs(q - p)] + f[p];
        }
    }
    }

    cvPow(dst, dst, 0.5);

    __END__;

    imfree(buffer);
}


/*********************************** IPP functions *********************************/

icvDistanceTransform_3x3_8u32f_C1R_t icvDistanceTransform_3x3_8u32f_C1R_p = 0;
icvDistanceTransform_5x5_8u32f_C1R_t icvDistanceTransform_5x5_8u32f_C1R_p = 0;

typedef CStatus (C_STDCALL * CvIPPDistTransFunc)(const uchar* src, int srcstep,
                                                    float* dst, int dststep,
                                                    CSize size, const float* metrics);

/***********************************************************************************/

typedef CStatus (C_STDCALL * CvDistTransFunc)(const uchar* src, int srcstep,
                                                 int* temp, int tempstep,
                                                 float* dst, int dststep,
                                                 CSize size, const float* metrics);

/* Wrapper function for distance transform group */
CC_IMPL void
cvDistTransform(const img_t* src, img_t* dst,
                 int distType, int maskSize,
                 const float *mask,
                 void* labelsarr)
{
    img_t* temp = 0;
    img_t* src_copy = 0;
    CvMemStorage* st = 0;
    
    CC_FUNCNAME("cvDistTransform");

    __BEGIN__;

    float _mask[5];
    
    
    img_t lstub, *labels = labelsarr;
    CSize size;
    CvIPPDistTransFunc ipp_func = 0;

    CC_CALL(src = cvGetMat(src, &srcstub));
    CC_CALL(dst = cvGetMat(dst, &dststub));

    if(!CC_IS_MASK_ARR(src) || CC_MAT_TYPE(dst) != CC_32F)
        CC_ERROR(CC_StsUnsupportedFormat, "source image must be 8uC1 and the distance map must be 32fC1");

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedSizes, "the source and the destination images must be of the same size");

    if(maskSize != CC_DIST_MASK_3 && maskSize != CC_DIST_MASK_5 && maskSize != CC_DIST_MASK_PRECISE)
        CC_ERROR(CC_StsBadSize, "Mask size should be 3 or 5 or 0 (presize)");

    if(distType == CC_DIST_C || distType == CC_DIST_L1)
        maskSize = !labels ? CC_DIST_MASK_3 : CC_DIST_MASK_5;
    else if(distType == CC_DIST_L2 && labels)
        maskSize = CC_DIST_MASK_5;

    if(maskSize == CC_DIST_MASK_PRECISE)
    {
        CC_CALL(icvTrueDistTrans(src, dst));
        EXIT;
    }
    
    if(labels)
    {
        CC_CALL(labels = cvGetMat(labels, &lstub));
        if(CC_MAT_TYPE(labels->tid) != CC_32SC1)
            CC_ERROR(CC_StsUnsupportedFormat, "the output array of labels must be 32sC1");

        if(!CC_ARE_SIZES_EQ(labels, dst))
            CC_ERROR(CC_StsUnmatchedSizes, "the array of labels has a different size");

        if(maskSize == CC_DIST_MASK_3)
            CC_ERROR(CC_StsNotImplemented,
            "3x3 mask can not be used for \"labeled\" distance transform. Use 5x5 mask");
    }

    if(distType == CC_DIST_C || distType == CC_DIST_L1 || distType == CC_DIST_L2)
    {
        icvGetDistanceTransformMask((distType == CC_DIST_C ? 0 :
            distType == CC_DIST_L1 ? 1 : 2) + maskSize*10, _mask);
    }
    else if(distType == CC_DIST_USER)
    {
        if(!mask)
            CC_ERROR(CC_StsNullPtr, "");

        memcpy(_mask, mask, (maskSize/2 + 1)*sizeof(float));
    }

    if(!labels)
        ipp_func = maskSize == CC_DIST_MASK_3 ? icvDistanceTransform_3x3_8u32f_C1R_p :
                                                icvDistanceTransform_5x5_8u32f_C1R_p;

    size = cvGetMatSize(src);

    if(ipp_func && src->cols >= 4 && src->rows >= 2)
    {
        IPPI_CALL(ipp_func(src->tt.data, src->step,
                dst->tt.fl, dst->step, size, _mask));
    }
    else
    {
        int border = maskSize == CC_DIST_MASK_3 ? 1 : 2;
        CC_CALL(temp = cvCreateMat(size.height + border*2, size.width + border*2, CC_32SC1));

        if(!labels)
        {
            CvDistTransFunc func = maskSize == CC_DIST_MASK_3 ?
                icvDistanceTransform_3x3_C1R :
                icvDistanceTransform_5x5_C1R;

            func(src->tt.data, src->step, temp->tt.i, temp->step,
                  dst->tt.fl, dst->step, size, _mask);
        }
        else
        {
            CvSeq *contours = 0;
            CPoint top_left = {0,0}, bottom_right = {size.width-1,size.height-1};
            int label;

            CC_CALL(st = cvCreateMemStorage());
            CC_CALL(src_copy = cvCreateMat(size.height, size.width, src->tid));
            cvCmpS(src, 0, src_copy, CC_CMP_EQ);
            cvFindContours(src_copy, st, &contours, sizeof(CvContour),
                            CC_RETR_CCOMP, CC_CHAIN_APPROX_SIMPLE);
            cvZero(labels);
            for(label = 1; contours != 0; contours = contours->h_next, label++)
            {
                CScalar area_color = cScalarAll(label);
                cvDrawContours(labels, contours, area_color, area_color, -255, -1, 8);
            }

            cvCopy(src, src_copy);
            cvRectangle(src_copy, top_left, bottom_right, cScalarAll(255), 1, 8);

            icvDistanceTransformEx_5x5_C1R(src_copy->tt.data, src_copy->step, temp->tt.i, temp->step,
                        dst->tt.fl, dst->step, labels->tt.i, labels->step, size, _mask);
        }
    }

    __END__;

    cvReleaseMat(&temp);
    cvReleaseMat(&src_copy);
    cvReleaseMemStorage(&st);
}


