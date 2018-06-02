

static CStatus
icvUnDistort_8u_CnR(const uchar* src, int srcstep,
                     uchar* dst, int dststep, CSize size,
                     const float* intrinsic_matrix,
                     const float* dist_coeffs, int cn)
{
    int u, v, i;
    float u0 = intrinsic_matrix[2], v0 = intrinsic_matrix[5];
    float fx = intrinsic_matrix[0], fy = intrinsic_matrix[4];
    float _fx = 1.f/fx, _fy = 1.f/fy;
    float k1 = dist_coeffs[0], k2 = dist_coeffs[1];
    float p1 = dist_coeffs[2], p2 = dist_coeffs[3];

    srcstep /= sizeof(src[0]);
    dststep /= sizeof(dst[0]);

    for(v = 0; v < size.height; v++, dst += dststep)
    {
        float y = (v - v0)*_fy;
        float y2 = y*y;
        float ky = 1 + (k1 + k2*y2)*y2;
        float k2y = 2*k2*y2;
        float _2p1y = 2*p1*y;
        float _3p1y2 = 3*p1*y2;
        float p2y2 = p2*y2;

        for(u = 0; u < size.width; u++)
        {
            float x = (u - u0)*_fx;
            float x2 = x*x;
            float kx = (k1 + k2*x2)*x2;
            float d = kx + ky + k2y*x2;
            float _u = fx*(x*(d + _2p1y) + p2y2 + (3*p2)*x2) + u0;
            float _v = fy*(y*(d + (2*p2)*x) + _3p1y2 + p1*x2) + v0;
            int iu = cRound(_u*(1 << ICC_WARP_SHIFT));
            int iv = cRound(_v*(1 << ICC_WARP_SHIFT));
            int ifx = iu & ICC_WARP_MASK;
            int ify = iv & ICC_WARP_MASK;
            iu >>= ICC_WARP_SHIFT;
            iv >>= ICC_WARP_SHIFT;

            float a0 = icvLinearCoeffs[ifx*2];
            float a1 = icvLinearCoeffs[ifx*2 + 1];
            float b0 = icvLinearCoeffs[ify*2];
            float b1 = icvLinearCoeffs[ify*2 + 1];

            if((unsigned)iv < (unsigned)(size.height - 1) &&
                (unsigned)iu < (unsigned)(size.width - 1))
            {
                const uchar* ptr = src + iv*srcstep + iu*cn;
                for(i = 0; i < cn; i++)
                {
                    float t0 = a1*CC_8TO32F(ptr[i]) + a0*CC_8TO32F(ptr[i+cn]);
                    float t1 = a1*CC_8TO32F(ptr[i+srcstep]) + a0*CC_8TO32F(ptr[i + srcstep + cn]);
                    dst[u*cn + i] = (uchar)cRound(b1*t0 + b0*t1);
                }
            }
            else
            {
                for(i = 0; i < cn; i++)
                    dst[u*cn + i] = 0;
            }
        }
    }

    return CC_OK;
}


icvUndistortGetSize_t icvUndistortGetSize_p = 0;
icvCreateMapCameraUndistort_32f_C1R_t icvCreateMapCameraUndistort_32f_C1R_p = 0;
icvUndistortRadial_8u_C1R_t icvUndistortRadial_8u_C1R_p = 0;
icvUndistortRadial_8u_C3R_t icvUndistortRadial_8u_C3R_p = 0;

typedef CStatus (C_STDCALL * CvUndistortRadialIPPFunc)
    (const void* pSrc, int srcStep, void* pDst, int dstStep, CSize roiSize,
      float fx, float fy, float cx, float cy, float k1, float k2, uchar *pBuffer);

CC_IMPL void
cvUndistort2(const img_t* src, img_t* dst, const img_t* A, const img_t* dist_coeffs)
{
    static int inittab = 0;
    uchar* buffer = 0;

    CC_FUNCNAME("cvUndistort2");

    __BEGIN__;

    float a[9], k[4];
    int coi1 = 0, coi2 = 0;
    img_t _a = cvMat(3, 3, CC_32F, 1, a), _k;
    int cn, src_step, dst_step;
    CSize size;

    if(!inittab)
    {
        icvInitLinearCoeffTab();
        icvInitCubicCoeffTab();
        inittab = 1;
    }

    
    

    if(coi1 != 0 || coi2 != 0)
        CC_ERROR(CC_BadCOI, "The function does not support COI");

    if(CC_MAT_TYPE(src) != CC_8U)
        CC_ERROR(CC_StsUnsupportedFormat, "Only 8-bit images are supported");

    if(src->tt.data == dst->tt.data)
        CC_ERROR(CC_StsNotImplemented, "In-place undistortion is not implemented");

    if(!CC_ARE_TYPES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    if(!(A) || A->rows != 3 || A->cols != 3  ||
        CC_MAT_TYPE(A) != CC_32F && CC_MAT_TYPE(A) != CC_64F)
        CC_ERROR(CC_StsBadArg, "Intrinsic matrix must be a valid 3x3 floating-point matrix");

    if(!(dist_coeffs) || dist_coeffs->rows != 1 && dist_coeffs->cols != 1 ||
        dist_coeffs->rows*dist_coeffs->cols*CC_MAT_CN(dist_coeffs) != 4 ||
        CC_MAT_TYPE(dist_coeffs) != CC_64F &&
        CC_MAT_TYPE(dist_coeffs) != CC_32F)
        CC_ERROR(CC_StsBadArg,
            "Distortion coefficients must be 1x4 or 4x1 floating-point vector");

    cvConvert(A, &_a);
    _k = cvMat(dist_coeffs->rows, dist_coeffs->cols, CC_32F, CC_MAT_CN(dist_coeffs), k);
    cvConvert(dist_coeffs, &_k);

    cn = CC_MAT_CN(src);
    size = cvGetMatSize(src);
    src_step = src->step ? src->step : CC_STUB_STEP;
    dst_step = dst->step ? dst->step : CC_STUB_STEP;

    if(fabs((double)k[2]) < 1e-5 && fabs((double)k[3]) < 1e-5 && icvUndistortGetSize_p)
    {
        int buf_size = 0;
        CvUndistortRadialIPPFunc func =
            cn == 1 ? (CvUndistortRadialIPPFunc)icvUndistortRadial_8u_C1R_p :
                      (CvUndistortRadialIPPFunc)icvUndistortRadial_8u_C3R_p;

        if(func && icvUndistortGetSize_p(size, &buf_size) >= 0 && buf_size > 0)
        {
            CC_CALL(buffer = (uchar*)cAlloc(buf_size));
            if(func(src->tt.data, src_step, dst->tt.data,
                      dst_step, size, a[0], a[4],
                      a[2], a[5], k[0], k[1], buffer) >= 0)
                EXIT;
        }
    }

    icvUnDistort_8u_CnR(src->tt.data, src_step,
        dst->tt.data, dst_step, size, a, k, cn);

    __END__;

    cFree(&buffer);
}


CC_IMPL void
cvInitUndistortMap(const img_t* A, const img_t* dist_coeffs,
                    img_t* _mapx, img_t* _mapy)
{
    uchar* buffer = 0;

    CC_FUNCNAME("cvInitUndistortMap");

    __BEGIN__;
    
    float a[9], k[4];
    int coi1 = 0, coi2 = 0;
    float *mapx, *mapy;
    img_t _a[1], _k[1];
    int mapxstep, mapystep;
    int u, v;
    float u0, v0, fx, fy, _fx, _fy, k1, k2, p1, p2;
    CSize size;

    cvMat(_a, 3, 3, CC_32F, 1, a);

    if(coi1 != 0 || coi2 != 0)
        CC_ERROR(CC_BadCOI, "The function does not support COI");

    if(CC_MAT_TYPE(_mapx) != CC_32F)
        CC_ERROR(CC_StsUnsupportedFormat, "Both maps must have 32fC1 type");

    if(!CC_ARE_TYPES_EQ(_mapx, _mapy))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(!CC_ARE_SIZES_EQ(_mapx, _mapy))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    if(!(A) || A->rows != 3 || A->cols != 3  ||
        CC_MAT_TYPE(A) != CC_32F && CC_MAT_TYPE(A) != CC_64F)
        CC_ERROR(CC_StsBadArg, "Intrinsic matrix must be a valid 3x3 floating-point matrix");

    if(!(dist_coeffs) || dist_coeffs->rows != 1 && dist_coeffs->cols != 1 ||
        dist_coeffs->rows*dist_coeffs->cols*CC_MAT_CN(dist_coeffs) != 4 ||
        CC_MAT_TYPE(dist_coeffs) != CC_64F &&
        CC_MAT_TYPE(dist_coeffs) != CC_32F)
        CC_ERROR(CC_StsBadArg,
            "Distortion coefficients must be 1x4 or 4x1 floating-point vector");

    cvConvert(A, _a);
    cvMat(_k, dist_coeffs->rows, dist_coeffs->cols, CC_32F, CC_MAT_CN(dist_coeffs), k);
    cvConvert(dist_coeffs, _k);

    u0 = a[2]; v0 = a[5];
    fx = a[0]; fy = a[4];
    _fx = 1.f/fx; _fy = 1.f/fy;
    k1 = k[0]; k2 = k[1];
    p1 = k[2]; p2 = k[3];

    mapxstep = _mapx->step ? _mapx->step : CC_STUB_STEP;
    mapystep = _mapy->step ? _mapy->step : CC_STUB_STEP;
    mapx = _mapx->tt.fl;
    mapy = _mapy->tt.fl;

    size = cvGetMatSize(_mapx);
    
    /*if(icvUndistortGetSize_p && icvCreateMapCameraUndistort_32f_C1R_p)
    {
        int buf_size = 0;
        if(icvUndistortGetSize_p(size, &buf_size) && buf_size > 0)
        {
            CC_CALL(buffer = (uchar*)cAlloc(buf_size));
            if(icvCreateMapCameraUndistort_32f_C1R_p(
                mapx, mapxstep, mapy, mapystep, size,
                a[0], a[4], a[2], a[5], k[0], k[1], k[2], k[3], buffer) >= 0)
                EXIT;
        }
    }*/
    
    mapxstep /= sizeof(mapx[0]);
    mapystep /= sizeof(mapy[0]);

    for(v = 0; v < size.height; v++, mapx += mapxstep, mapy += mapystep)
    {
        float y = (v - v0)*_fy;
        float y2 = y*y;
        float _2p1y = 2*p1*y;
        float _3p1y2 = 3*p1*y2;
        float p2y2 = p2*y2;

        for(u = 0; u < size.width; u++)
        {
            float x = (u - u0)*_fx;
            float x2 = x*x;
            float r2 = x2 + y2;
            float d = 1 + (k1 + k2*r2)*r2;
            float _u = fx*(x*(d + _2p1y) + p2y2 + (3*p2)*x2) + u0;
            float _v = fy*(y*(d + (2*p2)*x) + _3p1y2 + p1*x2) + v0;
            mapx[u] = _u;
            mapy[u] = _v;
        }
    }

    __END__;

    cFree(&buffer);
}


