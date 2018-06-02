#include "_cv.h"

#if 0

IPCVAPI(CStatus, icvCalcContrastHist8uC1R, (uchar** img, int step, CSize size,
                                              CvHistogram* hist, int dont_clear))

IPCVAPI(CStatus, icvCalcContrastHistMask8uC1R, (uchar** img, int step, 
                                                  uchar*  mask, int mask_step, CSize size,
                                                  CvHistogram* hist, int dont_clear))

/*F///////////////////////////////////////////////////////////////////////////////////////
//    Name:       icvCalcContrastHist8uC1R
//    Purpose:    Calculating the histogram of contrast from one-channel images
//    Context:
//    Parameters:
//    Returns:
//    Notes:      if dont_clear parameter is NULL then histogram clearing before
//                calculating (all values sets to NULL)
//F*/
static CStatus C_STDCALL
icvCalcContrastHist8uC1R(uchar** img, int step, CSize size,
                          CvHistogram* hist, int dont_clear)
{
    int i, j, t, x = 0, y = 0;
    int dims;

    if(!hist || !img)
        return CC_NULLPTR_ERR;

    dims = hist->c_dims;
    if(dims != 1)
        return CC_BADSIZE_ERR;

    if(hist->tid != CC_HIST_ARRAY)
        return CC_BADFLAG_ERR;

    for(i = 0; i < dims; i++)
        if(!img[i])
            return CC_NULLPTR_ERR;

    for(i = 0; i < hist->c_dims; i++)
    {
        if(!hist->thresh[i])
            return CC_NULLPTR_ERR;
        assert(hist->chdims[i]);
    }

    j = hist->dims[0] * hist->mdims[0];

    int *n = (int *)cAlloc((size_t)hist->dims[0] * sizeof(int));

    if(hist->tid == CC_HIST_ARRAY)
    {
        if(!dont_clear)
            for(i = 0; i < j; i++)
            {
                hist->array[i] = 0;
                n[i] = 0;
            }

        switch (hist->c_dims)
        {
        case 1:
            {
                uchar *data0 = img[0];
                int *array = (int *) hist->array;
                int *chdims = hist->chdims[0];

                for(i = 0; i < j; i++)
                    array[i] = cRound(hist->array[i]);

                for(y = 0; y < size.height; y++, data0 += step)
                {
                    for(x = 0; x <= size.width - 1; x += 2)
                    {
                        int v1_r = MIN(data0[x], data0[x + 1]);
                        int v2_r = MAX(data0[x], data0[x + 1]);

//    calculate contrast for the right-left pair 
                        for(t = v1_r; t < v2_r; t++)
                        {
                            int val0 = chdims[t + 128];

                            array[val0] += MIN(t - v1_r, v2_r - t);
                            n[val0]++;
                        }

                        if(y < size.height - 1)
                        {
                            int v1_d = MIN(data0[x], data0[x + step]);
                            int v2_d = MAX(data0[x], data0[x + step]);

//    calculate contrast for the top-down pair 
                            for(t = v1_d; t < v2_d; t++)
                            {
                                int val0 = chdims[t + 128];

                                array[val0] += MIN(t - v1_d, v2_d - t);
                                n[val0]++;
                            }
                        }
                    }
                }

//  convert int to float 
                for(i = 0; i < j; i++)
                {
                    if(n[i] != 0)
                        hist->array[i] = (float) array[i] / n[i];
                    else
                        hist->array[i] = 0;
                }
            }
            break;
        default:
            return CC_BADSIZE_ERR;
        }
    }

    cFree(&n);
    return CC_NO_ERR;
}

/*F///////////////////////////////////////////////////////////////////////////////////////
//    Name:       icvCalcContrastHistMask8uC1R
//    Purpose:    Calculating the mask histogram of contrast from one-channel images
//    Context:
//    Parameters:
//    Returns:
//    Notes:      if dont_clear parameter is NULL then histogram clearing before
//                calculating (all values sets to NULL)
//F*/
static CStatus C_STDCALL
icvCalcContrastHistMask8uC1R(uchar** img, int step, uchar* mask, int mask_step,
                              CSize size, CvHistogram * hist, int dont_clear)
{
    int i, j, t, x = 0, y = 0;
    int dims;


    if(!hist || !img || !mask)
        return CC_NULLPTR_ERR;

    dims = hist->c_dims;
    if(dims != 1)
        return CC_BADSIZE_ERR;

    if(hist->tid != CC_HIST_ARRAY)
        return CC_BADFLAG_ERR;

    for(i = 0; i < dims; i++)
        if(!img[i])
            return CC_NULLPTR_ERR;

    for(i = 0; i < hist->c_dims; i++)
    {
        if(!hist->thresh[i])
            return CC_NULLPTR_ERR;
        assert(hist->chdims[i]);
    }

    j = hist->dims[0] * hist->mdims[0];

    int *n = (int *)cAlloc((size_t) hist->dims[0] * sizeof(int));

    if(hist->tid == CC_HIST_ARRAY)
    {
        if(!dont_clear)
            for(i = 0; i < j; i++)
            {
                hist->array[i] = 0;
                n[i] = 0;
            }

        switch (hist->c_dims)
        {
        case 1:
            {
                uchar *data0 = img[0];
                uchar *maskp = mask;
                int *array = (int *) hist->array;
                int *chdims = hist->chdims[0];

                for(i = 0; i < j; i++)
                    array[i] = cRound(hist->array[i]);

                for(y = 0; y < size.height; y++, data0 += step, maskp += mask_step)
                {
                    for(x = 0; x <= size.width - 2; x++)
                    {
                        if(maskp[x])
                        {
                            if(maskp[x + 1])
                            {
                                int v1_r = MIN(data0[x], data0[x + 1]);
                                int v2_r = MAX(data0[x], data0[x + 1]);


                                //    calculate contrast for the right-left pair 
                                for(t = v1_r; t < v2_r; t++)
                                {
                                    int val0 = chdims[t + 128];

                                    array[val0] += MIN(t - v1_r, v2_r - t);
                                    n[val0]++;

                                }
                            }

                            if(y < size.height - 1)
                            {
                                if(maskp[x + mask_step])
                                {
                                    int v1_d = MIN(data0[x], data0[x + step]);
                                    int v2_d = MAX(data0[x], data0[x + step]);

                                    //    calculate contrast for the top-down pair 
                                    for(t = v1_d; t < v2_d; t++)
                                    {
                                        int val0 = chdims[t + 128];

                                        array[val0] += MIN(t - v1_d, v2_d - t);
                                        n[val0]++;

                                    }
                                }
                            }
                        }
                    }
                }

//  convert int to float 
                for(i = 0; i < j; i++)
                {
                    if(n[i] != 0)
                        hist->array[i] = (float) array[i] / n[i];
                    else
                        hist->array[i] = 0;
                }
            }
            break;
        default:
            return CC_BADSIZE_ERR;
        }
    }

    cFree(&n);
    return CC_NO_ERR;
}

/*
CC_IMPL void cvCalcContrastHist(img_t** img, CvHistogram* hist, int dont_clear)
{
    CC_FUNCNAME("cvCalcContrastHist");
    uchar*   data[CC_HIST_MAX_DIM];
    int      step = 0;
    CSize roi = {0,0};

    __BEGIN__;

    {for(int i = 0; i < hist->c_dims; i++)
        CC_CALL(CC_CHECK_IMAGE(img[i]));}

    {for(int i = 0; i < hist->c_dims; i++)
        cvGetImageRawData(img[i], &data[i], &step, &roi);}

    if(img[0]->nChannels != 1) 
        CC_ERROR(IMG_BadNumChannels, "bad channels numbers");

    if(img[0]->depth != IMG_DEPTH_8U) 
        CC_ERROR(IMG_BadDepth, "bad image depth");

    switch(img[0]->depth)
    {
    case IMG_DEPTH_8U:
        IPPI_CALL(icvCalcContrastHist8uC1R(data, step, roi, hist, dont_clear));
        break;
    default:  CC_ERROR(IMG_BadDepth, "bad image depth");
    }

    __CLEANUP__;
    __END__;
}
*/

CC_IMPL void
cvCalcContrastHist(img_t ** img, CvHistogram * hist, int dont_clear, img_t * mask)
{
    CC_FUNCNAME("cvCalcContrastHist");
    uchar *data[CC_HIST_MAX_DIM];
    uchar *mask_data = 0;
    int step = 0;
    int mask_step = 0;
    CSize roi = { 0, 0 };

    __BEGIN__;

    {
        for(int i = 0; i < hist->c_dims; i++)
            CC_CALL(CC_CHECK_IMAGE(img[i]));
    }
    if(mask)
    {
        CC_CALL(CC_CHECK_IMAGE(mask));
        if(mask->depth != IMG_DEPTH_8U)
            CC_ERROR(CC_BadDepth, "bad mask depth");
        cvGetImageRawData(mask, &mask_data, &mask_step, 0);
    }


    {
        for(int i = 0; i < hist->c_dims; i++)
            cvGetImageRawData(img[i], &data[i], &step, &roi);
    }

    if(img[0]->nChannels != 1)
        CC_ERROR(CC_BadNumChannels, "bad channels numbers");

    if(img[0]->depth != IMG_DEPTH_8U)
        CC_ERROR(CC_BadDepth, "bad image depth");


    switch (img[0]->depth)
    {
    case IMG_DEPTH_8U:
        if(!mask)
        {
            IPPI_CALL(icvCalcContrastHist8uC1R(data, step, roi, hist, dont_clear));
        }
        else
        {
            IPPI_CALL(icvCalcContrastHistMask8uC1R(data, step, mask_data,
                                                     mask_step, roi, hist, dont_clear));
        }
        break;
    default:
        CC_ERROR(CC_BadDepth, "bad image depth");
    }

    __CLEANUP__;
    __END__;
}

#endif
