#include "_cvlist.h"

#define halfPi ((float)(CC_PI*0.5))
#define Pi     ((float)CC_PI)
#define a0  0 /*-4.172325e-7f*/   /*(-(float)0x7)/((float)0x1000000); */
#define a1 1.000025f        /*((float)0x1922253)/((float)0x1000000)*2/Pi; */
#define a2 -2.652905e-4f    /*(-(float)0x2ae6)/((float)0x1000000)*4/(Pi*Pi); */
#define a3 -0.165624f       /*(-(float)0xa45511)/((float)0x1000000)*8/(Pi*Pi*Pi); */
#define a4 -1.964532e-3f    /*(-(float)0x30fd3)/((float)0x1000000)*16/(Pi*Pi*Pi*Pi); */
#define a5 1.02575e-2f      /*((float)0x191cac)/((float)0x1000000)*32/(Pi*Pi*Pi*Pi*Pi); */
#define a6 -9.580378e-4f    /*(-(float)0x3af27)/((float)0x1000000)*64/(Pi*Pi*Pi*Pi*Pi*Pi); */

#define _sin(x) ((((((a6*(x) + a5)*(x) + a4)*(x) + a3)*(x) + a2)*(x) + a1)*(x) + a0)
#define _cos(x) _sin(halfPi - (x))

/****************************************************************************************\
*                               Classical Hough Transform                                *
\****************************************************************************************/

typedef struct CvLinePolar
{
    float rho;
    float angle;
}
CvLinePolar;

/*=====================================================================================*/

#define hough_cmp_gt(l1,l2) (aux[l1] > aux[l2])

static CC_IMPLEMENT_QSORT_EX(icvHoughSortDescent32s, int, hough_cmp_gt, const int*)

/*
Here image is an input raster;
step is it's step; size characterizes it's ROI;
rho and theta are discretization steps (in pixels and radians correspondingly).
threshold is the minimum number of pixels in the feature for it
to be a candidate for line. lines is the output
array of (rho, theta) pairs. linesMax is the buffer size (number of pairs).
Functions return the actual number of found lines.
*/
static void
icvHoughLinesStandard(const img_t* img, float rho, float theta,
                       int threshold, CvSeq *lines, int linesMax)
{
    int *accum = 0;
    int *sort_buf=0;
    float *tabSin = 0;
    float *tabCos = 0;

    CC_FUNCNAME("icvHoughLinesStandard");

    __BEGIN__;
    
    const uchar* image;
    int step, width, height;
    int numangle, numrho;
    int total = 0;
    float ang;
    int r, n;
    int i, j;
    float irho = 1 / rho;
    double scale;

    CC_ASSERT(CC_IS_MAT(img) && CC_MAT_TYPE(img) == CC_8UC1);

    image = img->tt.data;
    step = img->step;
    width = img->cols;
    height = img->rows;

    numangle = cRound(CC_PI / theta);
    numrho = cRound(((width + height) * 2 + 1) / rho);

    CC_CALL(accum = (int*)cAlloc(sizeof(accum[0]) * (numangle+2) * (numrho+2)));
    CC_CALL(sort_buf = (int*)cAlloc(sizeof(accum[0]) * numangle * numrho));
    CC_CALL(tabSin = (float*)cAlloc(sizeof(tabSin[0]) * numangle));
    CC_CALL(tabCos = (float*)cAlloc(sizeof(tabCos[0]) * numangle));
    memset(accum, 0, sizeof(accum[0]) * (numangle+2) * (numrho+2));

    for(ang = 0, n = 0; n < numangle; ang += theta, n++)
    {
        tabSin[n] = (float)(sin(ang) * irho);
        tabCos[n] = (float)(cos(ang) * irho);
    }

    // stage 1. fill accumulator
    for(i = 0; i < height; i++)
        for(j = 0; j < width; j++)
        {
            if(image[i * step + j] != 0)
                for(n = 0; n < numangle; n++)
                {
                    r = cRound(j * tabCos[n] + i * tabSin[n]);
                    r += (numrho - 1) / 2;
                    accum[(n+1) * (numrho+2) + r+1]++;
                }
        }

    // stage 2. find local maximums 
    for(r = 0; r < numrho; r++)
        for(n = 0; n < numangle; n++)
        {
            int base = (n+1) * (numrho+2) + r+1;
            if(accum[base] > threshold &&
                accum[base] > accum[base - 1] && accum[base] >= accum[base + 1] &&
                accum[base] > accum[base - numrho - 2] && accum[base] >= accum[base + numrho + 2])
                sort_buf[total++] = base;
        }

    // stage 3. sort the detected lines by accumulator value
    icvHoughSortDescent32s(sort_buf, total, accum);
    
    // stage 4. store the first min(total,linesMax) lines to the output buffer
    linesMax = MIN(linesMax, total);
    scale = 1./(numrho+2);
    for(i = 0; i < linesMax; i++)
    {
        CvLinePolar line;
        int idx = sort_buf[i];
        int n = cFloor(idx*scale) - 1;
        int r = idx - (n+1)*(numrho+2) - 1;
        line.rho = (r - (numrho - 1)*0.5f) * rho;
        line.angle = n * theta;
        cvSeqPush(lines, &line);
    }

    __END__;

    cFree(&sort_buf);
    cFree(&tabSin);
    cFree(&tabCos);
    cFree(&accum);
}


/****************************************************************************************\
*                     Multi-Scale variant of Classical Hough Transform                   *
\****************************************************************************************/

#if defined _MSC_VER && _MSC_VER >= 1200
#pragma warning(disable: 4714)
#endif

//DECLARE_AND_IMPLEMENT_LIST(_index, h_);
IMPLEMENT_LIST(_index, h_)

static void
icvHoughLinesSDiv(const img_t* img,
                   float rho, float theta, int threshold,
                   int srn, int stn,
                   CvSeq* lines, int linesMax)
{
    uchar *caccum = 0;
    uchar *buffer = 0;
    float *sinTable = 0;
    int *x = 0;
    int *y = 0;
    _CVLIST *list = 0;

    CC_FUNCNAME("icvHoughLinesSDiv");

    __BEGIN__;

#define _POINT(row, column)\
    (image_src[(row)*step+(column)])

    uchar *mcaccum = 0;
    int rn, tn;                 /* number of rho and theta discrete values */
    int index, i;
    int ri, ti, ti1, ti0;
    int row, col;
    float r, t;                 /* Current rho and theta */
    float rv;                   /* Some temporary rho value */
    float irho;
    float itheta;
    float srho, stheta;
    float isrho, istheta;

    const uchar* image_src;
    int w, h, step;
    int fn = 0;
    float xc, yc;

    const float d2r = (float)(Pi / 180);
    int sfn = srn * stn;
    int fi;
    int count;
    int cmax = 0;
    
    CVPOS pos;
    _index *pindex;
    _index vi;

    CC_ASSERT(CC_IS_MAT(img) && CC_MAT_TYPE(img) == CC_8UC1);
    CC_ASSERT(linesMax > 0 && rho > 0 && theta > 0);
    
    threshold = MIN(threshold, 255);

    image_src = img->tt.data;
    step = img->step;
    w = img->cols;
    h = img->rows;

    irho = 1 / rho;
    itheta = 1 / theta;
    srho = rho / srn;
    stheta = theta / stn;
    isrho = 1 / srho;
    istheta = 1 / stheta;

    rn = cFloor(sqrt((double)w * w + (double)h * h) * irho);
    tn = cFloor(2 * Pi * itheta);

    list = h_create_list__index(linesMax < 1000 ? linesMax : 1000);
    vi.value = threshold;
    vi.rho = -1;
    h_add_head__index(list, &vi);

    /* Precalculating sin */
    CC_CALL(sinTable = (float*)cAlloc(5 * tn * stn * sizeof(float)));

    for(index = 0; index < 5 * tn * stn; index++)
    {
        sinTable[index] = (float)cos(stheta * index * 0.2f);
    }

    CC_CALL(caccum = (uchar*)cAlloc(rn * tn * sizeof(caccum[0])));
    memset(caccum, 0, rn * tn * sizeof(caccum[0]));

    /* Counting all feature pixels */
    for(row = 0; row < h; row++)
        for(col = 0; col < w; col++)
            fn += _POINT(row, col) != 0;

    CC_CALL(x = (int*)cAlloc(fn * sizeof(x[0])));
    CC_CALL(y = (int*)cAlloc(fn * sizeof(y[0])));

    /* Full Hough Transform (it's accumulator update part) */
    fi = 0;
    for(row = 0; row < h; row++)
    {
        for(col = 0; col < w; col++)
        {
            if(_POINT(row, col))
            {
                int halftn;
                float r0;
                float scale_factor;
                int iprev = -1;
                float phi, phi1;
                float theta_it;     /* Value of theta for iterating */

                /* Remember the feature point */
                x[fi] = col;
                y[fi] = row;
                fi++;

                yc = (float) row + 0.5f;
                xc = (float) col + 0.5f;

                /* Update the accumulator */
                t = (float) fabs(cFastArctan(yc, xc) * d2r);
                r = (float) sqrt((double)xc * xc + (double)yc * yc);
                r0 = r * irho;
                ti0 = cFloor((t + Pi / 2) * itheta);

                caccum[ti0]++;

                theta_it = rho / r;
                theta_it = theta_it < theta ? theta_it : theta;
                scale_factor = theta_it * itheta;
                halftn = cFloor(Pi / theta_it);
                for(ti1 = 1, phi = theta_it - halfPi, phi1 = (theta_it + t) * itheta;
                     ti1 < halftn; ti1++, phi += theta_it, phi1 += scale_factor)
                {
                    rv = r0 * _cos(phi);
                    i = cFloor(rv) * tn;
                    i += cFloor(phi1);
                    assert(i >= 0);
                    assert(i < rn * tn);
                    caccum[i] = (uchar) (caccum[i] + ((i ^ iprev) != 0));
                    iprev = i;
                    if(cmax < caccum[i])
                        cmax = caccum[i];
                }
            }
        }
    }

    /* Starting additional analysis */
    count = 0;
    for(ri = 0; ri < rn; ri++)
    {
        for(ti = 0; ti < tn; ti++)
        {
            if(caccum[ri * tn + ti > threshold])
            {
                count++;
            }
        }
    }

    if(count * 100 > rn * tn)
    {
        icvHoughLinesStandard(img, rho, theta, threshold, lines, linesMax);
        EXIT;
    }

    CC_CALL(buffer = (uchar *) cAlloc(srn * stn + 2));
    mcaccum = buffer + 1;

    count = 0;
    for(ri = 0; ri < rn; ri++)
    {
        for(ti = 0; ti < tn; ti++)
        {
            if(caccum[ri * tn + ti] > threshold)
            {
                count++;
                memset(mcaccum, 0, sfn * sizeof(uchar));

                for(index = 0; index < fn; index++)
                {
                    int ti2;
                    float r0;

                    yc = (float) y[index] + 0.5f;
                    xc = (float) x[index] + 0.5f;

                    /* Update the accumulator */
                    t = (float) fabs(cFastArctan(yc, xc) * d2r);
                    r = (float) sqrt((double)xc * xc + (double)yc * yc) * isrho;
                    ti0 = cFloor((t + Pi * 0.5f) * istheta);
                    ti2 = (ti * stn - ti0) * 5;
                    r0 = (float) ri *srn;

                    for(ti1 = 0 /*, phi = ti*theta - Pi/2 - t */ ; ti1 < stn; ti1++, ti2 += 5
                         /*phi += stheta */)
                    {
                        /*rv = r*_cos(phi) - r0; */
                        rv = r * sinTable[(int) (abs(ti2))] - r0;
                        i = cFloor(rv) * stn + ti1;

                        i = CC_IMAX(i, -1);
                        i = CC_IMIN(i, sfn);
                        mcaccum[i]++;
                        assert(i >= -1);
                        assert(i <= sfn);
                    }
                }

                /* Find peaks in maccum... */
                for(index = 0; index < sfn; index++)
                {
                    i = 0;
                    pos = h_get_tail_pos__index(list);
                    if(h_get_prev__index(&pos)->value < mcaccum[index])
                    {
                        vi.value = mcaccum[index];
                        vi.rho = index / stn * srho + ri * rho;
                        vi.theta = index % stn * stheta + ti * theta - halfPi;
                        while(h_is_pos__index(pos))
                        {
                            if(h_get__index(pos)->value > mcaccum[index])
                            {
                                h_insert_after__index(list, pos, &vi);
                                if(h_get_count__index(list) > linesMax)
                                {
                                    h_remove_tail__index(list);
                                }
                                break;
                            }
                            h_get_prev__index(&pos);
                        }
                        if(!h_is_pos__index(pos))
                        {
                            h_add_head__index(list, &vi);
                            if(h_get_count__index(list) > linesMax)
                            {
                                h_remove_tail__index(list);
                            }
                        }
                    }
                }
            }
        }
    }

    pos = h_get_head_pos__index(list);
    if(h_get_count__index(list) == 1)
    {
        if(h_get__index(pos)->rho < 0)
        {
            h_clear_list__index(list);
        }
    }
    else
    {
        while(h_is_pos__index(pos))
        {
            CvLinePolar line;
            pindex = h_get__index(pos);
            if(pindex->rho < 0)
            {
                /* This should be the last element... */
                h_get_next__index(&pos);
                assert(!h_is_pos__index(pos));
                break;
            }
            line.rho = pindex->rho;
            line.angle = pindex->theta;
            cvSeqPush(lines, &line);

            if(lines->total >= linesMax)
                EXIT;
            h_get_next__index(&pos);
        }
    }

    __END__;

    h_destroy_list__index(list);
    cFree(&sinTable);
    cFree(&x);
    cFree(&y);
    cFree(&caccum);
    cFree(&buffer);
}


/****************************************************************************************\
*                              Probabilistic Hough Transform                             *
\****************************************************************************************/

#if defined WIN64 && defined EM64T && _MSC_VER == 1400 && !defined CC_ICC
#pragma optimize("",off)
#endif

static void
icvHoughLinesProbabalistic(img_t* image,
                            float rho, float theta, int threshold,
                            int lineLength, int lineGap,
                            CvSeq *lines, int linesMax)
{
    img_t* accum = 0;
    img_t* mask = 0;
    img_t* trigtab = 0;
    CvMemStorage* storage = 0;

    CC_FUNCNAME("icvHoughLinesProbalistic");

    __BEGIN__;
    
    CvSeq* seq;
    CvSeqWriter writer;
    int width, height;
    int numangle, numrho;
    float ang;
    int r, n, count;
    CPoint pt;
    float irho = 1 / rho;
    CRNG rng = cRNG(-1);
    const float* ttab;
    uchar* mdata0;

    CC_ASSERT(CC_IS_MAT(image) && CC_MAT_TYPE(image->tid) == CC_8UC1);

    width = image->cols;
    height = image->rows;

    numangle = cRound(CC_PI / theta);
    numrho = cRound(((width + height) * 2 + 1) / rho);

    CC_CALL(accum = cvCreateMat(numangle, numrho, CC_32SC1));
    CC_CALL(mask = cvCreateMat(height, width, CC_8UC1));
    CC_CALL(trigtab = cvCreateMat(1, numangle, CC_32FC2));
    cvZero(accum);
    
    CC_CALL(storage = cvCreateMemStorage(0));
    
    for(ang = 0, n = 0; n < numangle; ang += theta, n++)
    {
        trigtab->tt.fl[n*2] = (float)(cos(ang) * irho);
        trigtab->tt.fl[n*2+1] = (float)(sin(ang) * irho);
    }
    ttab = trigtab->tt.fl;
    mdata0 = mask->tt.data;

    CC_CALL(cvStartWriteSeq(CC_32SC2, sizeof(CvSeq), sizeof(CPoint), storage, &writer)); 

    // stage 1. collect non-zero image points
    for(pt.y = 0, count = 0; pt.y < height; pt.y++)
    {
        const uchar* data = image->tt.data + pt.y*image->step;
        uchar* mdata = mdata0 + pt.y*width;
        for(pt.x = 0; pt.x < width; pt.x++)
        {
            if(data[pt.x])
            {
                mdata[pt.x] = (uchar)1;
                CC_WRITE_SEQ_ELEM(pt, writer);
            }
            else
                mdata[pt.x] = 0;
        }
    }

    seq = cvEndWriteSeq(&writer);
    count = seq->total;

    // stage 2. process all the points in random order
    for(; count > 0; count--)
    {
        // choose random point out of the remaining ones
        int idx = cvRandInt(&rng) % count;
        int max_val = threshold-1, max_n = 0;
        CPoint* pt = (CPoint*)cvGetSeqElem(seq, idx);
        CPoint line_end[2] = {{0,0}, {0,0}};
        float a, b;
        int* adata = accum->tt.i;
        int i, j, k, x0, y0, dx0, dy0, xflag;
        int good_line;
        const int shift = 16;

        i = pt->y;
        j = pt->x;

        // "remove" it by overriding it with the last element
        *pt = *(CPoint*)cvGetSeqElem(seq, count-1);

        // check if it has been excluded already (i.e. belongs to some other line)
        if(!mdata0[i*width + j])
            continue;

        // update accumulator, find the most probable line
        for(n = 0; n < numangle; n++, adata += numrho)
        {
            r = cRound(j * ttab[n*2] + i * ttab[n*2+1]);
            r += (numrho - 1) / 2;
            int val = ++adata[r];
            if(max_val < val)
            {
                max_val = val;
                max_n = n;
            }
        }

        // if it is too "weak" candidate, continue with another point
        if(max_val < threshold)
            continue;

        // from the current point walk in each direction
        // along the found line and extract the line segment
        a = -ttab[max_n*2+1];
        b = ttab[max_n*2];
        x0 = j;
        y0 = i;
        if(fabs(a) > fabs(b))
        {
            xflag = 1;
            dx0 = a > 0 ? 1 : -1;
            dy0 = cRound(b*(1 << shift)/fabs(a));
            y0 = (y0 << shift) + (1 << (shift-1));
        }
        else
        {
            xflag = 0;
            dy0 = b > 0 ? 1 : -1;
            dx0 = cRound(a*(1 << shift)/fabs(b));
            x0 = (x0 << shift) + (1 << (shift-1));
        }

        for(k = 0; k < 2; k++)
        {
            int gap = 0, x = x0, y = y0, dx = dx0, dy = dy0;
            
            if(k > 0)
                dx = -dx, dy = -dy;

            // walk along the line using fixed-point arithmetics,
            // stop at the image border or in case of too big gap
            for(;; x += dx, y += dy)
            {
                uchar* mdata;
                int i1, j1;

                if(xflag)
                {
                    j1 = x;
                    i1 = y >> shift;
                }
                else
                {
                    j1 = x >> shift;
                    i1 = y;
                }

                if(j1 < 0 || j1 >= width || i1 < 0 || i1 >= height)
                    break;

                mdata = mdata0 + i1*width + j1;

                // for each non-zero point:
                //    update line end,
                //    clear the mask element
                //    reset the gap
                if(*mdata)
                {
                    gap = 0;
                    line_end[k].y = i1;
                    line_end[k].x = j1;
                }
                else if(++gap > lineGap)
                    break;
            }
        }

        good_line = abs(line_end[1].x - line_end[0].x) >= lineLength ||
                    abs(line_end[1].y - line_end[0].y) >= lineLength;

        for(k = 0; k < 2; k++)
        {
            int x = x0, y = y0, dx = dx0, dy = dy0;
            
            if(k > 0)
                dx = -dx, dy = -dy;

            // walk along the line using fixed-point arithmetics,
            // stop at the image border or in case of too big gap
            for(;; x += dx, y += dy)
            {
                uchar* mdata;
                int i1, j1;

                if(xflag)
                {
                    j1 = x;
                    i1 = y >> shift;
                }
                else
                {
                    j1 = x >> shift;
                    i1 = y;
                }

                mdata = mdata0 + i1*width + j1;

                // for each non-zero point:
                //    update line end,
                //    clear the mask element
                //    reset the gap
                if(*mdata)
                {
                    if(good_line)
                    {
                        adata = accum->tt.i;
                        for(n = 0; n < numangle; n++, adata += numrho)
                        {
                            r = cRound(j1 * ttab[n*2] + i1 * ttab[n*2+1]);
                            r += (numrho - 1) / 2;
                            adata[r]--;
                        }
                    }
                    *mdata = 0;
                }

                if(i1 == line_end[k].y && j1 == line_end[k].x)
                    break;
            }
        }

        if(good_line)
        {
            CRect lr = { line_end[0].x, line_end[0].y, line_end[1].x, line_end[1].y };
            cvSeqPush(lines, &lr);
            if(lines->total >= linesMax)
                EXIT;
        }
    }

    __END__;

    cvReleaseMat(&accum);
    cvReleaseMat(&mask);
    cvReleaseMat(&trigtab);
    cvReleaseMemStorage(&storage);
}


#if defined WIN64 && defined EM64T && _MSC_VER == 1400 && !defined CC_ICC
#pragma optimize("",on)
#endif


/* Wrapper function for standard hough transform */
CC_IMPL CvSeq*
cvHoughLines2(img_t* src_image, void* lineStorage, int method,
               double rho, double theta, int threshold,
               double param1, double param2)
{
    CvSeq* result = 0;

    CC_FUNCNAME("cvHoughLines");

    __BEGIN__;
    
    img_t stub, *img = src_image;
    img_t* mat = 0;
    CvSeq* lines = 0;
    CvSeq lines_header;
    CvSeqBlock lines_block;
    int lineType, elemSize;
    int linesMax = INT_MAX;
    int iparam1, iparam2;

    CC_CALL(img = cvGetMat(img, &stub));

    if(!CC_IS_MASK_ARR(img))
        CC_ERROR(CC_StsBadArg, "The source image must be 8-bit, single-channel");

    if(!lineStorage)
        CC_ERROR(CC_StsNullPtr, "NULL destination");

    if(rho <= 0 || theta <= 0 || threshold <= 0)
        CC_ERROR(CC_StsOutOfRange, "rho, theta and threshold must be positive");

    if(method != CC_HOUGH_PROBABILISTIC)
    {
        lineType = CC_32FC2;
        elemSize = sizeof(float)*2;
    }
    else
    {
        lineType = CC_32SC4;
        elemSize = sizeof(int)*4;
    }

    if(CC_IS_STORAGE(lineStorage))
    {
        CC_CALL(lines = cvCreateSeq(lineType, sizeof(CvSeq), elemSize, (CvMemStorage*)lineStorage));
    }
    else if(CC_IS_MAT(lineStorage))
    {
        mat = lineStorage;

        if(!CC_IS_MAT_CONT(mat) || mat->rows != 1 && mat->cols != 1)
            CC_ERROR(CC_StsBadArg,
            "The destination matrix should be continuous and have a single row or a single column");

        if(CC_MAT_TYPE(mat) != lineType)
            CC_ERROR(CC_StsBadArg,
            "The destination matrix data type is inappropriate, see the manual");

        CC_CALL(lines = cvMakeSeqHeaderForArray(lineType, sizeof(CvSeq), elemSize, mat->tt.data,
                                                  mat->rows + mat->cols - 1, &lines_header, &lines_block));
        linesMax = lines->total;
        CC_CALL(cvClearSeq(lines));
    }
    else
    {
        CC_ERROR(CC_StsBadArg, "Destination is not CvMemStorage* nor img_t*");
    }

    iparam1 = cRound(param1);
    iparam2 = cRound(param2);

    switch(method)
    {
    case CC_HOUGH_STANDARD:
          CC_CALL(icvHoughLinesStandard(img, (float)rho,
                (float)theta, threshold, lines, linesMax));
          break;
    case CC_HOUGH_MULTI_SCALE:
          CC_CALL(icvHoughLinesSDiv(img, (float)rho, (float)theta,
                threshold, iparam1, iparam2, lines, linesMax));
          break;
    case CC_HOUGH_PROBABILISTIC:
          CC_CALL(icvHoughLinesProbabalistic(img, (float)rho, (float)theta,
                threshold, iparam1, iparam2, lines, linesMax));
          break;
    default:
        CC_ERROR(CC_StsBadArg, "Unrecognized method id");
    }

    if(mat)
    {
        if(mat->cols > mat->rows)
            mat->cols = lines->total;
        else
            mat->rows = lines->total;
    }
    else
    {
        result = lines;
    }

    __END__;
    
    return result;    
}


/****************************************************************************************\
*                                     Circle Detection                                   *
\****************************************************************************************/

static void
icvHoughCirclesGradient(img_t* img, float dp, float min_dist,
                         int min_radius, int max_radius,
                         int canny_threshold, int acc_threshold,
                         CvSeq* circles, int circles_max)
{
    const int SHIFT = 10, ONE = 1 << SHIFT, R_THRESH = 30;
    img_t *dx = 0, *dy = 0;
    img_t *edges = 0;
    img_t *accum = 0;
    int* sort_buf = 0;
    img_t* dist_buf = 0;
    CvMemStorage* storage = 0;
    
    CC_FUNCNAME("icvHoughCirclesGradient");

    __BEGIN__;

    int x, y, i, j, center_count, nz_count;
    int rows, cols, arows, acols;
    int astep, *adata;
    float* ddata;
    CvSeq *nz, *centers;
    float idp, dr;
    CvSeqReader reader;

    CC_CALL(edges = cvCreateMat(img->rows, img->cols, CC_8UC1));
    CC_CALL(cvCanny(img, edges, MAX(canny_threshold/2,1), canny_threshold, 3));

    CC_CALL(dx = cvCreateMat(img->rows, img->cols, CC_16SC1));
    CC_CALL(dy = cvCreateMat(img->rows, img->cols, CC_16SC1));
    CC_CALL(cvSobel(img, dx, 1, 0, 3));
    CC_CALL(cvSobel(img, dy, 0, 1, 3));

    if(dp < 1.f)
        dp = 1.f;
    idp = 1.f/dp;
    CC_CALL(accum = cvCreateMat(cCeil(img->rows*idp)+2, cCeil(img->cols*idp)+2, CC_32SC1));
    CC_CALL(cvZero(accum));

    CC_CALL(storage = cvCreateMemStorage());
    CC_CALL(nz = cvCreateSeq(CC_32SC2, sizeof(CvSeq), sizeof(CPoint), storage));
    CC_CALL(centers = cvCreateSeq(CC_32SC1, sizeof(CvSeq), sizeof(int), storage));

    rows = img->rows;
    cols = img->cols;
    arows = accum->rows - 2;
    acols = accum->cols - 2;
    adata = accum->tt.i;
    astep = accum->step/sizeof(adata[0]);

    for(y = 0; y < rows; y++)
    {
        const uchar* edges_row = edges->tt.data + y*edges->step;
        const short* dx_row = (const short*)(dx->tt.data + y*dx->step);
        const short* dy_row = (const short*)(dy->tt.data + y*dy->step);

        for(x = 0; x < cols; x++)
        {
            float vx, vy;
            int sx, sy, x0, y0, x1, y1, r, k;
            CPoint pt;
            
            vx = dx_row[x];
            vy = dy_row[x];

            if(!edges_row[x] || vx == 0 && vy == 0)
                continue;
            
            if(fabs(vx) < fabs(vy))
            {
                sx = cRound(vx*ONE/fabs(vy));
                sy = vy < 0 ? -ONE : ONE;
            }
            else
            {
                assert(vx != 0);
                sy = cRound(vy*ONE/fabs(vx));
                sx = vx < 0 ? -ONE : ONE;
            }

            x0 = cRound((x*idp)*ONE) + ONE + (ONE/2);
            y0 = cRound((y*idp)*ONE) + ONE + (ONE/2);

            for(k = 0; k < 2; k++)
            {
                x0 += min_radius * sx;
                y0 += min_radius * sy;

                for(x1 = x0, y1 = y0, r = min_radius; r <= max_radius; x1 += sx, y1 += sy, r++)
                {
                    int x2 = x1 >> SHIFT, y2 = y1 >> SHIFT;
                    if((unsigned)x2 >= (unsigned)acols ||
                        (unsigned)y2 >= (unsigned)arows)
                        break;
                    adata[y2*astep + x2]++;
                }

                x0 -= min_radius * sx;
                y0 -= min_radius * sy;
                sx = -sx; sy = -sy;
            }

            pt.x = x; pt.y = y;
            cvSeqPush(nz, &pt);
        }
    }

    nz_count = nz->total;
    if(!nz_count)
        EXIT;

    for(y = 1; y < arows - 1; y++)
    {
        for(x = 1; x < acols - 1; x++)
        {
            int base = y*(acols+2) + x;
            if(adata[base] > acc_threshold &&
                adata[base] > adata[base-1] && adata[base] > adata[base+1] &&
                adata[base] > adata[base-acols-2] && adata[base] > adata[base+acols+2])
                cvSeqPush(centers, &base);
        }
    }

    center_count = centers->total;
    if(!center_count)
        EXIT;

    CC_CALL(sort_buf = (int*)cAlloc(MAX(center_count,nz_count)*sizeof(sort_buf[0])));
    cvCvtSeqToArray(centers, sort_buf);

    icvHoughSortDescent32s(sort_buf, center_count, adata);
    cvClearSeq(centers);
    cvSeqPushMulti(centers, sort_buf, center_count);

    CC_CALL(dist_buf = cvCreateMat(1, nz_count, CC_32FC1));
    ddata = dist_buf->tt.fl;

    dr = dp;
    min_dist = MAX(min_dist, dp);
    min_dist *= min_dist;

    for(i = 0; i < centers->total; i++)
    {
        int ofs = *(int*)cvGetSeqElem(centers, i);
        y = ofs/(acols+2) - 1;
        x = ofs - (y+1)*(acols+2) - 1;
        float cx = (float)(x*dp), cy = (float)(y*dp);
        int start_idx = nz_count - 1;
        float start_dist, dist_sum;
        float r_best = 0, c[3];
        int max_count = R_THRESH;

        for(j = 0; j < circles->total; j++)
        {
            float* c = (float*)cvGetSeqElem(circles, j);
            if((c[0] - cx)*(c[0] - cx) + (c[1] - cy)*(c[1] - cy) < min_dist)
                break;
        }

        if(j < circles->total)
            continue;

        cvStartReadSeq(nz, &reader);
        for(j = 0; j < nz_count; j++)
        {
            CPoint pt;
            float _dx, _dy;
            CC_READ_SEQ_ELEM(pt, reader);
            _dx = cx - pt.x; _dy = cy - pt.y;
            ddata[j] = _dx*_dx + _dy*_dy;
            sort_buf[j] = j;
        }

        cvPow(dist_buf, dist_buf, 0.5);
        icvHoughSortDescent32s(sort_buf, nz_count, (int*)ddata);
        
        dist_sum = start_dist = ddata[sort_buf[nz_count-1]];
        for(j = nz_count - 2; j >= 0; j--)
        {
            float d = ddata[sort_buf[j]];

            if(d > max_radius)
                break;

            if(d - start_dist > dr)
            {
                float r_cur = ddata[sort_buf[(j + start_idx)/2]];
                if((start_idx - j)*r_best >= max_count*r_cur ||
                    r_best < FLT_EPSILON && start_idx - j >= max_count)
                {
                    r_best = r_cur;
                    max_count = start_idx - j;
                }
                start_dist = d;
                start_idx = j;
                dist_sum = 0;
            }
            dist_sum += d;
        }

        if(max_count > R_THRESH)
        {
            c[0] = cx;
            c[1] = cy;
            c[2] = (float)r_best;
            cvSeqPush(circles, c);
            if(circles->total > circles_max)
                EXIT;
        }
    }

    __END__;

    cvReleaseMat(&dist_buf);
    cFree(&sort_buf);
    cvReleaseMemStorage(&storage);
    cvReleaseMat(&edges);
    cvReleaseMat(&dx);
    cvReleaseMat(&dy);
    cvReleaseMat(&accum);
}

CC_IMPL CvSeq*
cvHoughCircles(img_t* src_image, void* circle_storage,
                int method, double dp, double min_dist,
                double param1, double param2,
                int min_radius, int max_radius)
{
    CvSeq* result = 0;

    CC_FUNCNAME("cvHoughCircles");

    __BEGIN__;
    
    img_t stub, *img = src_image;
    img_t* mat = 0;
    CvSeq* circles = 0;
    CvSeq circles_header;
    CvSeqBlock circles_block;
    int circles_max = INT_MAX;
    int canny_threshold = cRound(param1);
    int acc_threshold = cRound(param2);

    CC_CALL(img = cvGetMat(img, &stub));

    if(!CC_IS_MASK_ARR(img))
        CC_ERROR(CC_StsBadArg, "The source image must be 8-bit, single-channel");

    if(!circle_storage)
        CC_ERROR(CC_StsNullPtr, "NULL destination");

    if(dp <= 0 || min_dist <= 0 || canny_threshold <= 0 || acc_threshold <= 0)
        CC_ERROR(CC_StsOutOfRange, "dp, min_dist, canny_threshold and acc_threshold must be all positive numbers");

    min_radius = MAX(min_radius, 0);
    if(max_radius <= 0)
        max_radius = MAX(img->rows, img->cols);
    else if(max_radius <= min_radius)
        max_radius = min_radius + 2;

    if(CC_IS_STORAGE(circle_storage))
    {
        CC_CALL(circles = cvCreateSeq(CC_32FC3, sizeof(CvSeq),
            sizeof(float)*3, (CvMemStorage*)circle_storage));
    }
    else if(CC_IS_MAT(circle_storage))
    {
        mat = circle_storage;

        if(!CC_IS_MAT_CONT(mat) || mat->rows != 1 && mat->cols != 1 ||
            CC_MAT_TYPE(mat) != CC_32FC3)
            CC_ERROR(CC_StsBadArg,
            "The destination matrix should be continuous and have a single row or a single column");

        CC_CALL(circles = cvMakeSeqHeaderForArray(CC_32FC3, sizeof(CvSeq), sizeof(float)*3,
                mat->tt.data, mat->rows + mat->cols - 1, &circles_header, &circles_block));
        circles_max = circles->total;
        CC_CALL(cvClearSeq(circles));
    }
    else
    {
        CC_ERROR(CC_StsBadArg, "Destination is not CvMemStorage* nor img_t*");
    }

    switch(method)
    {
    case CC_HOUGH_GRADIENT:
          CC_CALL(icvHoughCirclesGradient(img, (float)dp, (float)min_dist,
                                    min_radius, max_radius, canny_threshold,
                                    acc_threshold, circles, circles_max));
          break;
    default:
        CC_ERROR(CC_StsBadArg, "Unrecognized method id");
    }

    if(mat)
    {
        if(mat->cols > mat->rows)
            mat->cols = circles->total;
        else
            mat->rows = circles->total;
    }
    else
        result = circles;

    __END__;
    
    return result;    
}

#undef halfPi
#undef Pi
#undef a0
#undef a1
#undef a2
#undef a3
#undef a4
#undef a5
#undef a6
#undef _sin
#undef _cos
#undef hough_cmp_gt
#undef _POINT


