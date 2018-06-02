#include "_cv.h"

#define _CC_SNAKE_BIG 2.e+38f
#define _CC_SNAKE_IMAGE 1
#define _CC_SNAKE_GRAD  2


/*F///////////////////////////////////////////////////////////////////////////////////////
//    Name:      icvSnake8uC1R     
//    Purpose:   
//    Context:   
//    Parameters:
//               src - source image,
//               srcStep - its step in bytes,
//               roi - size of ROI,
//               pt - pointer to snake points array
//               n - size of points array, 
//               alpha - pointer to coefficient of continuity energy, 
//               beta - pointer to coefficient of curvature energy,  
//               gamma - pointer to coefficient of image energy,  
//               coeffUsage - if CC_VALUE - alpha, beta, gamma point to single value
//                            if CC_MATAY - point to arrays
//               criteria - termination criteria.
//               scheme - image energy scheme
//                         if _CC_SNAKE_IMAGE - image intensity is energy
//                         if _CC_SNAKE_GRAD  - magnitude of gradient is energy
//    Returns:   
//F*/

static CStatus
icvSnake8uC1R(unsigned char *src,
               int srcStep,
               CSize roi,
               CPoint * pt,
               int n,
               float *alpha,
               float *beta,
               float *gamma,
               int coeffUsage, CSize win, CTermCriteria criteria, int scheme)
{
    int i, j, k;
    int neighbors = win.height * win.width;

    int centerx = win.width >> 1;
    int centery = win.height >> 1;

    float invn;
    int iteration = 0;
    int converged = 0;
	

    float *Econt;
    float *Ecurv;
    float *Eimg;
    float *E;

    float _alpha, _beta, _gamma;

    /*#ifdef GRAD_SNAKE */
    float *gradient = NULL;
    uchar *map = NULL;
    int map_width = ((roi.width - 1) >> 3) + 1;
    int map_height = ((roi.height - 1) >> 3) + 1;
    CvSepFilter pX, pY;
    #define WTILE_SIZE 8
    #define TILE_SIZE (WTILE_SIZE + 2)        
    short dx[TILE_SIZE*TILE_SIZE], dy[TILE_SIZE*TILE_SIZE];
    img_t _dx = cvMat(TILE_SIZE, TILE_SIZE, CC_16SC1, dx);
    img_t _dy = cvMat(TILE_SIZE, TILE_SIZE, CC_16SC1, dy);
    img_t _src = cvMat(roi.height, roi.width, CC_8UC1, src);

    /* inner buffer of convolution process */
    //char ConvBuffer[400];

    /*#endif */


    /* check bad arguments */
    if(src == NULL)
        return CC_NULLPTR_ERR;
    if((roi.height <= 0) || (roi.width <= 0))
        return CC_BADSIZE_ERR;
    if(srcStep < roi.width)
        return CC_BADSIZE_ERR;
    if(pt == NULL)
        return CC_NULLPTR_ERR;
    if(n < 3)
        return CC_BADSIZE_ERR;
    if(alpha == NULL)
        return CC_NULLPTR_ERR;
    if(beta == NULL)
        return CC_NULLPTR_ERR;
    if(gamma == NULL)
        return CC_NULLPTR_ERR;
    if(coeffUsage != CC_VALUE && coeffUsage != CC_ARRAY)
        return CC_BADFLAG_ERR;
    if((win.height <= 0) || (!(win.height & 1)))
        return CC_BADSIZE_ERR;
    if((win.width <= 0) || (!(win.width & 1)))
        return CC_BADSIZE_ERR;

    invn = 1 / ((float) n);

    if(scheme == _CC_SNAKE_GRAD)
    {
        pX.init_deriv(TILE_SIZE+2, CC_8UC1, CC_16SC1, 1, 0, 3);
        pY.init_deriv(TILE_SIZE+2, CC_8UC1, CC_16SC1, 0, 1, 3);

        gradient = (float *) cAlloc(roi.height * roi.width * sizeof(float));

        if(!gradient)
            return CC_OUTOFMEM_ERR;
        map = (uchar *) cAlloc(map_width * map_height);
        if(!map)
        {
            cFree(&gradient);
            return CC_OUTOFMEM_ERR;
        }
        /* clear map - no gradient computed */
        memset((void *) map, 0, map_width * map_height);
    }
    Econt = (float *) cAlloc(neighbors * sizeof(float));
    Ecurv = (float *) cAlloc(neighbors * sizeof(float));
    Eimg = (float *) cAlloc(neighbors * sizeof(float));
    E = (float *) cAlloc(neighbors * sizeof(float));

    while(!converged)
    {
        float ave_d = 0;
        int moved = 0;

        converged = 0;
        iteration++;
        /* compute average distance */
        for(i = 1; i < n; i++)
        {
            int diffx = pt[i - 1].x - pt[i].x;
            int diffy = pt[i - 1].y - pt[i].y;

            ave_d += cvSqrt((float) (diffx * diffx + diffy * diffy));
        }
        ave_d += cvSqrt((float) ((pt[0].x - pt[n - 1].x) *
                                  (pt[0].x - pt[n - 1].x) +
                                  (pt[0].y - pt[n - 1].y) * (pt[0].y - pt[n - 1].y)));

        ave_d *= invn;
        /* average distance computed */
        for(i = 0; i < n; i++)
        {
            /* Calculate Econt */
            float maxEcont = 0;
            float maxEcurv = 0;
            float maxEimg = 0;
            float minEcont = _CC_SNAKE_BIG;
            float minEcurv = _CC_SNAKE_BIG;
            float minEimg = _CC_SNAKE_BIG;
            float Emin = _CC_SNAKE_BIG;

            int offsetx = 0;
            int offsety = 0;
            float tmp;

            /* compute bounds */
            int left = MIN(pt[i].x, win.width >> 1);
            int right = MIN(roi.width - 1 - pt[i].x, win.width >> 1);
            int upper = MIN(pt[i].y, win.height >> 1);
            int bottom = MIN(roi.height - 1 - pt[i].y, win.height >> 1);

            maxEcont = 0;
            minEcont = _CC_SNAKE_BIG;
            for(j = -upper; j <= bottom; j++)
            {
                for(k = -left; k <= right; k++)
                {
                    int diffx, diffy;
                    float energy;

                    if(i == 0)
                    {
                        diffx = pt[n - 1].x - (pt[i].x + k);
                        diffy = pt[n - 1].y - (pt[i].y + j);
                    }
                    else
                    {
                        diffx = pt[i - 1].x - (pt[i].x + k);
                        diffy = pt[i - 1].y - (pt[i].y + j);
                    }
                    Econt[(j + centery) * win.width + k + centerx] = energy =
                        (float) fabs(ave_d -
                                      cvSqrt((float) (diffx * diffx + diffy * diffy)));

                    maxEcont = MAX(maxEcont, energy);
                    minEcont = MIN(minEcont, energy);
                }
            }
            tmp = maxEcont - minEcont;
            tmp = (tmp == 0) ? 0 : (1 / tmp);
            for(k = 0; k < neighbors; k++)
            {
                Econt[k] = (Econt[k] - minEcont) * tmp;
            }

            /*  Calculate Ecurv */
            maxEcurv = 0;
            minEcurv = _CC_SNAKE_BIG;
            for(j = -upper; j <= bottom; j++)
            {
                for(k = -left; k <= right; k++)
                {
                    int tx, ty;
                    float energy;

                    if(i == 0)
                    {
                        tx = pt[n - 1].x - 2 * (pt[i].x + k) + pt[i + 1].x;
                        ty = pt[n - 1].y - 2 * (pt[i].y + j) + pt[i + 1].y;
                    }
                    else if(i == n - 1)
                    {
                        tx = pt[i - 1].x - 2 * (pt[i].x + k) + pt[0].x;
                        ty = pt[i - 1].y - 2 * (pt[i].y + j) + pt[0].y;
                    }
                    else
                    {
                        tx = pt[i - 1].x - 2 * (pt[i].x + k) + pt[i + 1].x;
                        ty = pt[i - 1].y - 2 * (pt[i].y + j) + pt[i + 1].y;
                    }
                    Ecurv[(j + centery) * win.width + k + centerx] = energy =
                        (float) (tx * tx + ty * ty);
                    maxEcurv = MAX(maxEcurv, energy);
                    minEcurv = MIN(minEcurv, energy);
                }
            }
            tmp = maxEcurv - minEcurv;
            tmp = (tmp == 0) ? 0 : (1 / tmp);
            for(k = 0; k < neighbors; k++)
            {
                Ecurv[k] = (Ecurv[k] - minEcurv) * tmp;
            }

            /* Calculate Eimg */
            for(j = -upper; j <= bottom; j++)
            {
                for(k = -left; k <= right; k++)
                {
                    float energy;

                    if(scheme == _CC_SNAKE_GRAD)
                    {
                        /* look at map and check status */
                        int x = (pt[i].x + k)/WTILE_SIZE;
                        int y = (pt[i].y + j)/WTILE_SIZE;

                        if(map[y * map_width + x] == 0)
                        {
                            int l, m;							

                            /* evaluate block location */
                            int upshift = y ? 1 : 0;
                            int leftshift = x ? 1 : 0;
                            int bottomshift = MIN(1, roi.height - (y + 1)*WTILE_SIZE);
                            int rightshift = MIN(1, roi.width - (x + 1)*WTILE_SIZE);
                            CRect g_roi = { x*WTILE_SIZE - leftshift, y*WTILE_SIZE - upshift,
                                leftshift + WTILE_SIZE + rightshift, upshift + WTILE_SIZE + bottomshift };
                            img_t _src1;
                            cvGetSubArr(&_src, &_src1, g_roi);

                            pX.process(&_src1, &_dx);
                            pY.process(&_src1, &_dy);

                            for(l = 0; l < WTILE_SIZE + bottomshift; l++)
                            {
                                for(m = 0; m < WTILE_SIZE + rightshift; m++)
                                {
                                    gradient[(y*WTILE_SIZE + l) * roi.width + x*WTILE_SIZE + m] =
                                        (float) (dx[(l + upshift) * TILE_SIZE + m + leftshift] *
                                                 dx[(l + upshift) * TILE_SIZE + m + leftshift] +
                                                 dy[(l + upshift) * TILE_SIZE + m + leftshift] *
                                                 dy[(l + upshift) * TILE_SIZE + m + leftshift]);
                                }
                            }
                            map[y * map_width + x] = 1;
                        }
                        Eimg[(j + centery) * win.width + k + centerx] = energy =
                            gradient[(pt[i].y + j) * roi.width + pt[i].x + k];
                    }
                    else
                    {
                        Eimg[(j + centery) * win.width + k + centerx] = energy =
                            src[(pt[i].y + j) * srcStep + pt[i].x + k];
                    }

                    maxEimg = MAX(maxEimg, energy);
                    minEimg = MIN(minEimg, energy);
                }
            }

            tmp = (maxEimg - minEimg);
            tmp = (tmp == 0) ? 0 : (1 / tmp);

            for(k = 0; k < neighbors; k++)
            {
                Eimg[k] = (minEimg - Eimg[k]) * tmp;
            }

            /* locate coefficients */
            if(coeffUsage == CC_VALUE)
            {
                _alpha = *alpha;
                _beta = *beta;
                _gamma = *gamma;
            }
            else
            {                   
                _alpha = alpha[i];
                _beta = beta[i];
                _gamma = gamma[i];
            }

            /* Find Minimize point in the neighbors */
            for(k = 0; k < neighbors; k++)
            {
                E[k] = _alpha * Econt[k] + _beta * Ecurv[k] + _gamma * Eimg[k];
            }
            Emin = _CC_SNAKE_BIG;
            for(j = -upper; j <= bottom; j++)
            {
                for(k = -left; k <= right; k++)
                {

                    if(E[(j + centery) * win.width + k + centerx] < Emin)
                    {
                        Emin = E[(j + centery) * win.width + k + centerx];
                        offsetx = k;
                        offsety = j;
                    }
                }
            }

            if(offsetx || offsety)
            {
                pt[i].x += offsetx;
                pt[i].y += offsety;
                moved++;
            }
        }
        converged = (moved == 0);
        if((criteria.type & CC_TERMCRIT_ITER) && (iteration >= criteria.max_iter))
            converged = 1;
        if((criteria.type & CC_TERMCRIT_EPS) && (moved <= criteria.epsilon))
            converged = 1;
    }

    cFree(&Econt);
    cFree(&Ecurv);
    cFree(&Eimg);
    cFree(&E);

    if(scheme == _CC_SNAKE_GRAD)
    {
        cFree(&gradient);
        cFree(&map);
    }
    return CC_OK;
}


CC_IMPL void
cvSnakeImage(const img_t* src, CPoint* points,
              int length, float *alpha,
              float *beta, float *gamma,
              int coeffUsage, CSize win,
              CTermCriteria criteria, int calcGradient)
{

    CC_FUNCNAME("cvSnakeImage");

    __BEGIN__;

    uchar *data;
    CSize size;
    int step;

    if(src->nChannels != 1)
        CC_ERROR(CC_BadNumChannels, "input image has more than one channel");

    if(src->depth != IMG_DEPTH_8U)
        CC_ERROR(CC_BadDepth, cvUnsupportedFormat);

    cvGetRawData(src, &data, &step, &size);

    IPPI_CALL(icvSnake8uC1R(data, step, size, points, length,
                              alpha, beta, gamma, coeffUsage, win, criteria,
                              calcGradient ? _CC_SNAKE_GRAD : _CC_SNAKE_IMAGE));
    __END__;
}

/* end of file */
