#include "_cv.h"

CC_IMPL void
cvFindCornerSubPix(const void* srcarr, CPoint2D32f* corners,
                    int count, CSize win, CSize zeroZone,
                    CTermCriteria criteria)
{
    float* buffer = 0;
    
    CC_FUNCNAME("cvFindCornerSubPix");

    __BEGIN__;

    const int MAX_ITERS = 100;
    const float drv_x[] = { -1.f, 0.f, 1.f };
    const float drv_y[] = { 0.f, 0.5f, 0.f };
    float *maskX;
    float *maskY;
    float *mask;
    float *src_buffer;
    float *gx_buffer;
    float *gy_buffer;
    int win_w = win.width * 2 + 1, win_h = win.height * 2 + 1;
    int win_rect_size = (win_w + 4) * (win_h + 4);
    double coeff;
    CSize size, src_buf_size;
    int i, j, k, pt_i;
    int max_iters, buffer_size;
    double eps;

    img_t stub, *src = srcarr;
    CC_CALL(src = cvGetMat(srcarr, &stub));

    if(CC_MAT_TYPE(src) != CC_8UC1)
        CC_ERROR(CC_StsBadMask, "");

    if(!corners)
        CC_ERROR(CC_StsNullPtr, "");

    if(count < 0)
        CC_ERROR(CC_StsBadSize, "");

    if(count == 0)
        EXIT;

    if(win.width <= 0 || win.height <= 0)
        CC_ERROR(CC_StsBadSize, "");

    size = cvGetMatSize(src);

    if(size.width < win_w + 4 || size.height < win_h + 4)
        CC_ERROR(CC_StsBadSize, "");

    /* initialize variables, controlling loop termination */
    switch(criteria.type)
    {
    case CC_TERMCRIT_ITER:
        eps = 0.f;
        max_iters = criteria.max_iter;
        break;
    case CC_TERMCRIT_EPS:
        eps = criteria.epsilon;
        max_iters = MAX_ITERS;
        break;
    case CC_TERMCRIT_ITER | CC_TERMCRIT_EPS:
        eps = criteria.epsilon;
        max_iters = criteria.max_iter;
        break;
    default:
        assert(0);
        CC_ERROR(CC_StsBadFlag, "");
    }

    eps = MAX(eps, 0);
    eps *= eps;                 /* use square of error in comparsion operations. */

    max_iters = MAX(max_iters, 1);
    max_iters = MIN(max_iters, MAX_ITERS);

    /* setup buffer */
    buffer_size = (win_rect_size * 5 + win_w + win_h + 32) * sizeof(float);
    buffer = (float*)cAlloc(buffer_size);

    /* assign pointers */
    maskX = buffer;
    maskY = maskX + win_w + 4;
    mask = maskY + win_h + 4;
    src_buffer = mask + win_w * win_h;
    gx_buffer = src_buffer + win_rect_size;
    gy_buffer = gx_buffer + win_rect_size;

    coeff = 1. / (win.width * win.width);

    /* calculate mask */
    for(i = -win.width, k = 0; i <= win.width; i++, k++)
    {
        maskX[k] = (float)exp(-i * i * coeff);
    }

    if(win.width == win.height)
    {
        maskY = maskX;
    }
    else
    {
        coeff = 1. / (win.height * win.height);
        for(i = -win.height, k = 0; i <= win.height; i++, k++)
        {
            maskY[k] = (float) exp(-i * i * coeff);
        }
    }

    for(i = 0; i < win_h; i++)
    {
        for(j = 0; j < win_w; j++)
        {
            mask[i * win_w + j] = maskX[j] * maskY[i];
        }
    }


    /* make zero_zone */
    if(zeroZone.width >= 0 && zeroZone.height >= 0 &&
        zeroZone.width * 2 + 1 < win_w && zeroZone.height * 2 + 1 < win_h)
    {
        for(i = win.height - zeroZone.height; i <= win.height + zeroZone.height; i++)
        {
            for(j = win.width - zeroZone.width; j <= win.width + zeroZone.width; j++)
            {
                mask[i * win_w + j] = 0;
            }
        }
    }

    /* set sizes of image rectangles, used in convolutions */
    src_buf_size.width = win_w + 2;
    src_buf_size.height = win_h + 2;

    /* do optimization loop for all the points */
    for(pt_i = 0; pt_i < count; pt_i++)
    {
        CPoint2D32f cT = corners[pt_i], cI = cT;
        int iter = 0;
        double err;

        do
        {
            CPoint2D32f cI2;
            double a, b, c, bb1, bb2;

            IPPI_CALL(icvGetRectSubPix_8u32f_C1R((uchar*)src->tt.data, src->step, size,
                                        src_buffer, (win_w + 2) * sizeof(src_buffer[0]),
                                        cSize(win_w + 2, win_h + 2), cI));

            /* calc derivatives */
            icvSepConvSmall3_32f(src_buffer, src_buf_size.width * sizeof(src_buffer[0]),
                                  gx_buffer, win_w * sizeof(gx_buffer[0]),
                                  src_buf_size, drv_x, drv_y, buffer);

            icvSepConvSmall3_32f(src_buffer, src_buf_size.width * sizeof(src_buffer[0]),
                                  gy_buffer, win_w * sizeof(gy_buffer[0]),
                                  src_buf_size, drv_y, drv_x, buffer);

            a = b = c = bb1 = bb2 = 0;

            /* process gradient */
            for(i = 0, k = 0; i < win_h; i++)
            {
                double py = i - win.height;

                for(j = 0; j < win_w; j++, k++)
                {
                    double m = mask[k];
                    double tgx = gx_buffer[k];
                    double tgy = gy_buffer[k];
                    double gxx = tgx * tgx * m;
                    double gxy = tgx * tgy * m;
                    double gyy = tgy * tgy * m;
                    double px = j - win.width;

                    a += gxx;
                    b += gxy;
                    c += gyy;

                    bb1 += gxx * px + gxy * py;
                    bb2 += gxy * px + gyy * py;
                }
            }

            {
                double A[4];
                double InvA[4];
                img_t matA, matInvA;

                A[0] = a;
                A[1] = A[2] = b;
                A[3] = c;

                cvInitMatHeader(&matA, 2, 2, CC_64F, A);
                cvInitMatHeader(&matInvA, 2, 2, CC_64FC1, InvA);

                cvInvert(&matA, &matInvA, CC_SVD);
                cI2.x = (float)(cI.x + InvA[0]*bb1 + InvA[1]*bb2);
                cI2.y = (float)(cI.y + InvA[2]*bb1 + InvA[3]*bb2);
            }

            err = (cI2.x - cI.x) * (cI2.x - cI.x) + (cI2.y - cI.y) * (cI2.y - cI.y);
            cI = cI2;
        }
        while(++iter < max_iters && err > eps);

        /* if new point is too far from initial, it means poor convergence.
           leave initial point as the result */
        if(fabs(cI.x - cT.x) > win.width || fabs(cI.y - cT.y) > win.height)
        {
            cI = cT;
        }

        corners[pt_i] = cI;     /* store result */
    }

    __CLEANUP__;
    __END__;

    cFree(&buffer);
}


