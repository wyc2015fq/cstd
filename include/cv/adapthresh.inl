

static void
icvAdaptiveThreshold_MeanC(const img_t* src, img_t* dst, int method,
                            int maxValue, int type, int size, double delta)
{
    img_t tmp[1] = {0};
    CC_FUNCNAME("icvAdaptiveThreshold_MeanC");

    __BEGIN__;

    img_t* mean = NULL;
    int i, j, rows, cols;
    int idelta = type == CC_THRESH_BINARY ? cCeil(delta) : cFloor(delta);
    uchar tab[768];

    if(size <= 1 || (size&1) == 0)
        CC_ERROR(CC_StsOutOfRange, "Neighborhood size must be >=3 and odd (3, 5, 7, ...)");

    if(maxValue < 0)
    {
        CC_CALL(cvZeroMat(dst));
        EXIT;
    }

    rows = src->rows;
    cols = src->cols;

    if(src->tt.data != dst->tt.data) {
        mean = dst;
    } else
        CC_CALL(mean = cvCreateMat(tmp, rows, cols, CC_8U));

    CC_CALL(cvSmooth(src, mean, method == CC_ADAPTIVE_THRESH_MEAN_C ?
                       CC_BLUR : CC_GAUSSIAN, size, size));
    if(maxValue > 255)
        maxValue = 255;

    if(type == CC_THRESH_BINARY)
        for(i = 0; i < 768; i++)
            tab[i] = (uchar)(i - 255 > -idelta ? maxValue : 0);
    else
        for(i = 0; i < 768; i++)
            tab[i] = (uchar)(i - 255 <= -idelta ? maxValue : 0);

    for(i = 0; i < rows; i++)
    {
        const uchar* s = src->tt.data + i*src->step;
        const uchar* m = mean->tt.data + i*mean->step;
        uchar* d = dst->tt.data + i*dst->step;

        for(j = 0; j < cols; j++)
            d[j] = tab[s[j] - m[j] + 255];
    }

    __END__;

    imfree(tmp);
}


CC_IMPL void
cvAdaptiveThreshold(const img_t *srcIm, img_t *dstIm, double maxValue,
                     int method, int type, int blockSize, double param1)
{
    img_t src_stub, dst_stub;
    img_t *src = 0, *dst = 0;

    CC_FUNCNAME("cvAdaptiveThreshold");

    __BEGIN__;

    if(type != CC_THRESH_BINARY && type != CC_THRESH_BINARY_INV)
        CC_ERROR(CC_StsBadArg, "Only CC_TRESH_BINARY and CC_THRESH_BINARY_INV "
                                "threshold types are acceptable");

    CC_CALL(src = cvGetMat(srcIm, &src_stub));
    CC_CALL(dst = cvGetMat(dstIm, &dst_stub));

    if(!CC_ARE_CNS_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedFormats, "");

    if(CC_MAT_TYPE(dst) != CC_8U || CC_MAT_CN(dst)!=1)
        CC_ERROR(CC_StsUnsupportedFormat, "");

    if(!CC_ARE_SIZES_EQ(src, dst))
        CC_ERROR(CC_StsUnmatchedSizes, "");

    switch(method)
    {
    case CC_ADAPTIVE_THRESH_MEAN_C:
    case CC_ADAPTIVE_THRESH_GAUSSIAN_C:
        CC_CALL(icvAdaptiveThreshold_MeanC(src, dst, method, cRound(maxValue),type,
                                             blockSize, param1));
        break;
    default:
        CC_ERROR(CC_BADCOEF_ERR, "");
    }

    __END__;
}


