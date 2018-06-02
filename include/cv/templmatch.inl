
static void icvCrossCorr(const img_t* img, const img_t* templ, img_t* corr, CPoint anchor CC_DEFAULT(cPoint(0,0)))
{
    const double block_scale = 4.5;
    const int min_block_size = 256;
    img_t dft_img[1] = {0};
    img_t dft_templ[1] = {0};
    void* buf = 0;
    
    CC_FUNCNAME("icvCrossCorr");
    
    __BEGIN__;

    img_t sstub, dstub, *src, *dst, temp;
    CSize dftsize, blocksize;
    img_t* planes[] = { 0, 0, 0, 0 };
    int x, y, i, yofs, buf_size = 0;
    TypeId type, templ_type, corr_type, max_type = CC_32F;
    int cn, templ_cn, corr_cn, max_cn = 1;

    if(CC_MAT_TYPE(img) != CC_8U &&
        CC_MAT_TYPE(img) != CC_16U &&
        CC_MAT_TYPE(img) != CC_32F)
        CC_ERROR(CC_StsUnsupportedFormat,
        "The function supports only 8u, 16u and 32f data types");

    if(!CC_ARE_DEPTHS_EQ(img, templ) && CC_MAT_TYPE(templ) != CC_32F)
        CC_ERROR(CC_StsUnsupportedFormat,
        "Template (kernel) must be of the same depth as the input image, or be 32f");
    
    if(!CC_ARE_DEPTHS_EQ(img, corr) && CC_MAT_TYPE(corr) != CC_32F &&
        CC_MAT_TYPE(corr) != CC_64F)
        CC_ERROR(CC_StsUnsupportedFormat,
        "The output image must have the same depth as the input image, or be 32f/64f");

    if((!CC_ARE_CNS_EQ(img, corr) || CC_MAT_CN(templ) > 1) &&
        (CC_MAT_CN(corr) > 1 || !CC_ARE_CNS_EQ(img, templ)))
        CC_ERROR(CC_StsUnsupportedFormat,
        "The output must have the same number of channels as the input (when the template has 1 channel), "
        "or the output must have 1 channel when the input and the template have the same number of channels");

    type = CC_MAT_DEPTH(img);
    cn = CC_MAT_CN(img);
    templ_type = CC_MAT_DEPTH(templ);
    templ_cn = CC_MAT_CN(templ);
    corr_type = CC_MAT_DEPTH(corr);
    corr_cn = CC_MAT_CN(corr);
    max_type = MAX(max_type, templ_type);
    max_type = MAX(max_type, type);
    max_type = MAX(max_type, corr_type);
    max_cn = MAX(max_cn, templ_cn);
    max_cn = MAX(max_cn, cn);
    max_cn = MAX(max_cn, corr_cn);
    if(type > CC_8U)
        max_type = CC_64F;

    if(img->cols < templ->cols || img->rows < templ->rows)
        CC_ERROR(CC_StsUnmatchedSizes,
        "Such a combination of image and template/filter size is not supported");

    if(corr->rows > img->rows + templ->rows - 1 ||
        corr->cols > img->cols + templ->cols - 1)
        CC_ERROR(CC_StsUnmatchedSizes,
        "output image should not be greater than (W + w - 1)x(H + h - 1)");

    blocksize.width = cRound(templ->cols*block_scale);
    blocksize.width = MAX(blocksize.width, min_block_size - templ->cols + 1);
    blocksize.width = MIN(blocksize.width, corr->cols);
    blocksize.height = cRound(templ->rows*block_scale);
    blocksize.height = MAX(blocksize.height, min_block_size - templ->rows + 1);
    blocksize.height = MIN(blocksize.height, corr->rows);

    dftsize.width = cGetOptimalDFTSize(blocksize.width + templ->cols - 1);
    if(dftsize.width == 1)
        dftsize.width = 2;
    dftsize.height = cGetOptimalDFTSize(blocksize.height + templ->rows - 1);
    if(dftsize.width <= 0 || dftsize.height <= 0)
        CC_ERROR(CC_StsOutOfRange, "the input arrays are too big");

    // recompute block size
    blocksize.width = dftsize.width - templ->cols + 1;
    blocksize.width = MIN(blocksize.width, corr->cols);
    blocksize.height = dftsize.height - templ->rows + 1;
    blocksize.height = MIN(blocksize.height, corr->rows);

    CC_CALL(cvCreateMat(dft_img, dftsize.height, dftsize.width, max_type, max_cn));
    CC_CALL(cvCreateMat(dft_templ, dftsize.height*templ_cn, dftsize.width, max_type, max_cn));

    if(templ_cn > 1 && templ_type != max_type)
        buf_size = templ->cols*templ->rows*CC_TYPE_SIZE(templ_type);

    if(cn > 1 && type != max_type)
        buf_size = MAX(buf_size, (blocksize.width + templ->cols - 1)*
            (blocksize.height + templ->rows - 1)*CC_TYPE_SIZE(type));

    if((corr_cn > 1 || cn > 1) && corr_type != max_type)
        buf_size = MAX(buf_size, blocksize.width*blocksize.height*CC_TYPE_SIZE(corr_type));

    if(buf_size > 0)
        CC_CALL(buf = cAlloc(buf_size));

    // compute DFT of each template plane
    for(i = 0; i < templ_cn; i++)
    {
        yofs = i*dftsize.height;

        src = (img_t*)templ;
        dst = cvGetSubRect(dft_templ, &dstub, cRect(0,yofs,templ->cols,templ->rows));
    
        if(templ_cn > 1)
        {
            planes[i] = templ_type == max_type ? dst :
                cvInitMatHeader(&temp, templ->rows, templ->cols, templ_type, templ_cn, buf);
            cvSplit(templ, planes[0], planes[1], planes[2], planes[3]);
            src = planes[i];
            planes[i] = 0;
        }

        if(dst != src)
            cvConvert(src, dst);

        if(dft_templ->cols > templ->cols)
        {
            cvGetSubRect(dft_templ, dst, cRect(templ->cols, yofs,
                          dft_templ->cols - templ->cols, templ->rows));
            cvZeroMat(dst);
        }
        cvGetSubRect(dft_templ, dst, cRect(0,yofs,dftsize.width,dftsize.height));
        cvDFT1(dst, dst, CC_DXT_FORWARD + CC_DXT_SCALE, templ->rows);
    }

    // calculate correlation by blocks
    for(y = 0; y < corr->rows; y += blocksize.height)
    {
        for(x = 0; x < corr->cols; x += blocksize.width)
        {
            CSize csz = { blocksize.width, blocksize.height }, isz;
            int x0 = x - anchor.x, y0 = y - anchor.y;
            int x1 = MAX(0, x0), y1 = MAX(0, y0), x2, y2;
            csz.width = MIN(csz.width, corr->cols - x);
            csz.height = MIN(csz.height, corr->rows - y);
            isz.width = csz.width + templ->cols - 1;
            isz.height = csz.height + templ->rows - 1;
            x2 = MIN(img->cols, x0 + isz.width);
            y2 = MIN(img->rows, y0 + isz.height);
            
            for(i = 0; i < cn; i++)
            {
                img_t dstub1, *dst1;
                yofs = i*dftsize.height;

                src = cvGetSubRect(img, &sstub, cRect(x1,y1,x2-x1,y2-y1));
                dst = cvGetSubRect(dft_img, &dstub, cRect(0,0,isz.width,isz.height));
                dst1 = dst;
                
                if(x2 - x1 < isz.width || y2 - y1 < isz.height)
                    dst1 = cvGetSubRect(dft_img, &dstub1,
                        cRect(x1 - x0, y1 - y0, x2 - x1, y2 - y1));

                if(cn > 1)
                {
                    planes[i] = dst1;
                    if(type != max_type)
                        planes[i] = cvInitMatHeader(&temp, y2 - y1, x2 - x1, type, cn, buf);
                    cvSplit(src, planes[0], planes[1], planes[2], planes[3]);
                    src = planes[i];
                    planes[i] = 0;
                }

                if(dst1 != src)
                    cvConvert(src, dst1);

                if(dst != dst1)
                    cvCopyMakeBorderTo(dst1, dst, cPoint(x1 - x0, y1 - y0), CC_BORDER_REPLICATE);

                if(dftsize.width > isz.width)
                {
                    cvGetSubRect(dft_img, dst, cRect(isz.width, 0,
                          dftsize.width - isz.width,dftsize.height));
                    cvZeroMat(dst);
                }

                cvDFT1(dft_img, dft_img, CC_DXT_FORWARD, isz.height);
                cvGetSubRect(dft_templ, dst,
                    cRect(0,(templ_cn>1?yofs:0),dftsize.width,dftsize.height));

                cvMulSpectrums1(dft_img, dst, dft_img, CC_DXT_MUL_CONJ);
                cvDFT1(dft_img, dft_img, CC_DXT_INVERSE, csz.height);

                src = cvGetSubRect(dft_img, &sstub, cRect(0,0,csz.width,csz.height));
                dst = cvGetSubRect(corr, &dstub, cRect(x,y,csz.width,csz.height));

                if(corr_cn > 1)
                {
                    planes[i] = src;
                    if(corr_type != max_type)
                    {
                        planes[i] = cvInitMatHeader(&temp, csz.height, csz.width, corr_type, corr_cn, buf);
                        cvConvert(src, planes[i]);
                    }
                    cvMerge(planes[0], planes[1], planes[2], planes[3], dst);
                    planes[i] = 0;                    
                }
                else
                {
                    if(i == 0)
                        cvConvert(src, dst);
                    else
                    {
                        if(max_type > corr_type)
                        {
                            cvInitMatHeader(&temp, csz.height, csz.width, corr_type, corr_cn, buf);
                            cvConvert(src, &temp);
                            src = &temp;
                        }
                        cvAcc(src, dst);
                    }
                }
            }
        }
    }

    __END__;

    imfree(dft_img);
    imfree(dft_templ);
    cFree(&buf);
}


/***************************** IPP Match Template Functions ******************************/

typedef CStatus (C_STDCALL * CvTemplMatchIPPFunc)
    (const void* img, int imgstep, CSize imgsize,
      const void* templ, int templstep, CSize templsize,
      void* result, int rstep);

/*****************************************************************************************/

CC_IMPL void
cvMatchTemplate(const img_t* img, const img_t* templ, img_t* result, int method)
{
    img_t sum[1] = {0};
    img_t sqsum[1] = {0};
    
    CC_FUNCNAME("cvMatchTemplate");

    __BEGIN__;

    int coi1 = 0, coi2 = 0;
    TypeId type;
    int i, j, k, cn;
    double4 templ_mean = {0};
    double templ_norm = 0, templ_sum2 = 0;
    
    int idx = 0, idx2 = 0;
    double *p0, *p1, *p2, *p3;
    double *q0, *q1, *q2, *q3;
    double inv_area;
    int sum_step, sqsum_step;
    int num_type = method == CC_TM_CCORR || method == CC_TM_CCORR_NORMED ? 0 :
                   method == CC_TM_CCOEFF || method == CC_TM_CCOEFF_NORMED ? 1 : 2;
    int is_normed = method == CC_TM_CCORR_NORMED ||
                    method == CC_TM_SQDIFF_NORMED ||
                    method == CC_TM_CCOEFF_NORMED;

    if(CC_MAT_TYPE(img) != CC_8U &&
        CC_MAT_TYPE(img) != CC_32F)
        CC_ERROR(CC_StsUnsupportedFormat,
        "The function supports only 8u and 32f data types");

    if(!CC_ARE_TYPES_EQ(img, templ))
        CC_ERROR(CC_StsUnmatchedSizes, "image and template should have the same type");

    if(CC_MAT_TYPE(result) != CC_32F)
        CC_ERROR(CC_StsUnsupportedFormat, "output image should have 32f type");

    if(img->rows < templ->rows || img->cols < templ->cols)
    {
        const img_t* t;
        CC_SWAP(img, templ, t);
    }

    if(result->rows != img->rows - templ->rows + 1 ||
        result->cols != img->cols - templ->cols + 1)
        CC_ERROR(CC_StsUnmatchedSizes, "output image should be (W - w + 1)x(H - h + 1)");

    if(method < CC_TM_SQDIFF || method > CC_TM_CCOEFF_NORMED)
        CC_ERROR(CC_StsBadArg, "unknown comparison method");

    type = CC_MAT_DEPTH(img);
    cn = CC_MAT_CN(img);

    CC_CALL(icvCrossCorr(img, templ, result));

    if(method == CC_TM_CCORR)
        EXIT;

    inv_area = 1./((double)templ->rows * templ->cols);

    CC_CALL(cvCreateMat(sum, img->rows + 1, img->cols + 1, CC_64F, cn));
    if(method == CC_TM_CCOEFF)
    {
        CC_CALL(cvIntegral(img, sum, 0, 0));
        CC_CALL(cvAvg(templ, templ_mean));
        q0 = q1 = q2 = q3 = 0;
    }
    else
    {
        double4 _templ_sdv = {0, 0, 0, 0};
        CC_CALL(cvCreateMat(sqsum, img->rows + 1, img->cols + 1, CC_64F, cn));
        CC_CALL(cvIntegral(img, sum, sqsum, 0));
        CC_CALL(cvAvgSdv(templ, templ_mean, _templ_sdv));

        templ_norm = CC_SQR(_templ_sdv[0]) + CC_SQR(_templ_sdv[1]) +
                    CC_SQR(_templ_sdv[2]) + CC_SQR(_templ_sdv[3]);

        if(templ_norm < DBL_EPSILON && method == CC_TM_CCOEFF_NORMED)
        {
            cvSet(result, cScalarAll(1.));
            EXIT;
        }
        
        templ_sum2 = templ_norm +
                     CC_SQR(templ_mean[0]) + CC_SQR(templ_mean[1]) +
                     CC_SQR(templ_mean[2]) + CC_SQR(templ_mean[3]);

        if(num_type != 1)
        {
            SET4(templ_mean, 0, 0, 0, 0);
            templ_norm = templ_sum2;
        }
        
        templ_sum2 /= inv_area;
        templ_norm = sqrt(templ_norm);
        templ_norm /= sqrt(inv_area); // care of accuracy here

        q0 = (double*)sqsum->tt.data;
        q1 = q0 + templ->cols*cn;
        q2 = (double*)(sqsum->tt.data + templ->rows*sqsum->step);
        q3 = q2 + templ->cols*cn;
    }

    p0 = (double*)sum->tt.data;
    p1 = p0 + templ->cols*cn;
    p2 = (double*)(sum->tt.data + templ->rows*sum->step);
    p3 = p2 + templ->cols*cn;

    sum_step = sum ? sum->step / sizeof(double) : 0;
    sqsum_step = sqsum ? sqsum->step / sizeof(double) : 0;

    for(i = 0; i < result->rows; i++)
    {
        float* rrow = (float*)(result->tt.data + i*result->step);
        idx = i * sum_step;
        idx2 = i * sqsum_step;

        for(j = 0; j < result->cols; j++, idx += cn, idx2 += cn)
        {
            double num = rrow[j], t;
            double wnd_mean2 = 0, wnd_sum2 = 0;
            
            if(num_type == 1)
            {
                for(k = 0; k < cn; k++)
                {
                    t = p0[idx+k] - p1[idx+k] - p2[idx+k] + p3[idx+k];
                    wnd_mean2 += CC_SQR(t);
                    num -= t*templ_mean[k];
                }

                wnd_mean2 *= inv_area;
            }

            if(is_normed || num_type == 2)
            {
                for(k = 0; k < cn; k++)
                {
                    t = q0[idx2+k] - q1[idx2+k] - q2[idx2+k] + q3[idx2+k];
                    wnd_sum2 += t;
                }

                if(num_type == 2)
                    num = wnd_sum2 - 2*num + templ_sum2;
            }

            if(is_normed)
            {
                t = sqrt(MAX(wnd_sum2 - wnd_mean2,0))*templ_norm;
                if(t > DBL_EPSILON)
                {
                    num /= t;
                    if(fabs(num) > 1.)
                        num = num > 0 ? 1 : -1;
                }
                else
                    num = method != CC_TM_SQDIFF_NORMED || num < DBL_EPSILON ? 0 : 1;
            }

            rrow[j] = (float)num;
        }
    }
        
    __END__;

    imfree(sum);
    imfree(sqsum);
}


