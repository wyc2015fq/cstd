#include "_cv.h"

#define  cmp_features(f1, f2)  (*(f1) > *(f2))

static CC_IMPLEMENT_QSORT(icvSortFeatures, int *, cmp_features)

CC_IMPL void
cvGoodFeaturesToTrack(const void* image, void* eigImage, void* tempImage,
                       CPoint2D32f* corners, int *corner_count,
                       double quality_level, double min_distance,
                       const void* maskImage, int block_size,
                       int use_harris, double harris_k)
{
    img_t* _eigImg = 0;
    img_t* _tmpImg = 0;
    
    CC_FUNCNAME("cvGoodFeaturesToTrack");

    __BEGIN__;

    double max_val = 0;
    int max_count = 0;
    int count = 0;
    int x, y, i, k = 0;
    int min_dist;
    int eig_step, tmp_step;

    /* when selecting points, use integer coordinates */
    CPoint *ptr = (CPoint *) corners;

    /* process floating-point images using integer arithmetics */
    int *eig_data = 0;
    int *tmp_data = 0;
    int **ptr_data = 0;
    uchar *mask_data = 0;
    int  mask_step = 0;
    CSize size;

    int    coi1 = 0, coi2 = 0, coi3 = 0;
    img_t stub, *img = image;
    img_t eig_stub, *eig = eigImage;
    img_t tmp_stub, *tmp = tempImage;
    img_t mask_stub, *mask = maskImage;

    if(corner_count)
    {
        max_count = *corner_count;
        *corner_count = 0;
    }

    CC_CALL(img = cvGetMat(img, &stub, &coi1));
    if(eig)
    {
        CC_CALL(eig = cvGetMat(eig, &eig_stub, &coi2));
    }
    else
    {
        CC_CALL(_eigImg = cvCreateMat(img->rows, img->cols, CC_32FC1));
        eig = _eigImg;
    }

    if(tmp)
    {
        CC_CALL(tmp = cvGetMat(tmp, &tmp_stub, &coi3));
    }
    else
    {
        CC_CALL(_tmpImg = cvCreateMat(img->rows, img->cols, CC_32FC1));
        tmp = _tmpImg;
    }

    if(mask)
    {
        CC_CALL(mask = cvGetMat(mask, &mask_stub));
        if(!CC_IS_MASK_ARR(mask))
        {
            CC_ERROR(CC_StsBadMask, "");
        }
    }

    if(coi1 != 0 || coi2 != 0 || coi3 != 0)
        CC_ERROR(CC_BadCOI, "");

    if(CC_MAT_CN(img->tid) != 1 ||
        CC_MAT_CN(eig->tid) != 1 ||
        CC_MAT_CN(tmp->tid) != 1)
        CC_ERROR(CC_BadNumChannels, cvUnsupportedFormat);

    if(CC_MAT_TYPE(tmp->tid) != CC_32F ||
        CC_MAT_TYPE(eig->tid) != CC_32F)
        CC_ERROR(CC_BadDepth, cvUnsupportedFormat);

    if(!corners || !corner_count)
        CC_ERROR(CC_StsNullPtr, "");

    if(max_count <= 0)
        CC_ERROR(CC_StsBadArg, "maximal corners number is non positive");

    if(quality_level <= 0 || min_distance < 0)
        CC_ERROR(CC_StsBadArg, "quality level or min distance are non positive");

    if(use_harris)
    {
        CC_CALL(cvCornerHarris(img, eig, block_size, 3, harris_k));
    }
    else
    {
        CC_CALL(cvCornerMinEigenVal(img, eig, block_size, 3));
    }
    CC_CALL(cvMinMaxLoc(eig, 0, &max_val, 0, 0, mask));
    CC_CALL(cvThreshold(eig, eig, max_val * quality_level,
                          0, CC_THRESH_TOZERO));
    CC_CALL(cvDilate(eig, tmp));

    min_dist = cRound(min_distance * min_distance);

    size = cvGetMatSize(img);
    ptr_data = (int**)(tmp->tt.data);
    eig_data = (int*)(eig->tt.data);
    tmp_data = (int*)(tmp->tt.data);
    if(mask)
    {
        mask_data = (uchar*)(mask->tt.data);
        mask_step = mask->step;
    }

    eig_step = eig->step / sizeof(eig_data[0]);
    tmp_step = tmp->step / sizeof(tmp_data[0]);

    /* collect list of pointers to features - put them into temporary image */
    for(y = 1, k = 0; y < size.height - 1; y++)
    {
        eig_data += eig_step;
        tmp_data += tmp_step;
        mask_data += mask_step;

        for(x = 1; x < size.width - 1; x++)
        {
            int val = eig_data[x];
            if(val != 0 && val == tmp_data[x] && (!mask || mask_data[x]))
                ptr_data[k++] = eig_data + x;
        }
    }

    icvSortFeatures(ptr_data, k, 0);

    /* select the strongest features */
    for(i = 0; i < k; i++)
    {
        int j = count, ofs = (int)((uchar*)(ptr_data[i]) - eig->tt.data);
        y = ofs / eig->step;
        x = (ofs - y * eig->step)/sizeof(float);

        if(min_dist != 0)
        {
            for(j = 0; j < count; j++)
            {
                int dx = x - ptr[j].x;
                int dy = y - ptr[j].y;
                int dist = dx * dx + dy * dy;

                if(dist < min_dist)
                    break;
            }
        }

        if(j == count)
        {
            ptr[count].x = x;
            ptr[count].y = y;
            if(++count >= max_count)
                break;
        }
    }
    
    /* convert points to floating-point format */
    for(i = 0; i < count; i++)
    {
        assert((unsigned)ptr[i].x < (unsigned)size.width &&
                (unsigned)ptr[i].y < (unsigned)size.height);

        corners[i].x = (float)ptr[i].x;
        corners[i].y = (float)ptr[i].y;
    }

    *corner_count = count;

    __END__;

    cvReleaseMat(&_eigImg);
    cvReleaseMat(&_tmpImg);
}


