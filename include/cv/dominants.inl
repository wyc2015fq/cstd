#include "_cv.h"

typedef struct _PointInfo
{
    CPoint pt;
    int left_neigh;
    int right_neigh;

}
icvPointInfo;


static CStatus
icvFindDominantPointsIPAN(CvSeq * contour,
                           CvMemStorage * storage,
                           CvSeq ** corners, int dmin2, int dmax2, int dneigh2, float amax)
{
    CStatus status = CC_OK;

    /* variables */
    int n = contour->total;

    float *sharpness;
    float *distance;
    icvPointInfo *ptInf;

    int i, j, k;

    CvSeqWriter writer;

    float mincos = (float) cos(3.14159265359 * amax / 180);

    /* check bad arguments */
    if(contour == NULL)
        return CC_NULLPTR_ERR;
    if(storage == NULL)
        return CC_NULLPTR_ERR;
    if(corners == NULL)
        return CC_NULLPTR_ERR;
    if(dmin2 < 0)
        return CC_BADSIZE_ERR;
    if(dmax2 < dmin2)
        return CC_BADSIZE_ERR;
    if((dneigh2 > dmax2) || (dneigh2 < 0))
        return CC_BADSIZE_ERR;
    if((amax < 0) || (amax > 180))
        return CC_BADSIZE_ERR;

    sharpness = (float *) cAlloc(n * sizeof(float));
    distance = (float *) cAlloc(n * sizeof(float));

    ptInf = (icvPointInfo *) cAlloc(n * sizeof(icvPointInfo));

/*****************************************************************************************/
/*                                 First pass                                            */
/*****************************************************************************************/

    if(CC_IS_SEQ_CHAIN_CONTOUR(contour))
    {
        CvChainPtReader reader;

        cvStartReadChainPoints((CChain *) contour, &reader);

        for(i = 0; i < n; i++)
        {
            CC_READ_CHAIN_POINT(ptInf[i].pt, reader);
        }
    }
    else if(CC_IS_SEQ_POLYGON(contour))
    {
        CvSeqReader reader;

        cvStartReadSeq(contour, &reader, 0);

        for(i = 0; i < n; i++)
        {
            CC_READ_SEQ_ELEM(ptInf[i].pt, reader);
        }
    }
    else
    {
        return CC_BADFLAG_ERR;
    }

    for(i = 0; i < n; i++)
    {
        /* find nearest suitable points
           which satisfy distance constraint >dmin */
        int left_near = 0;
        int right_near = 0;
        int left_far, right_far;

        float dist_l = 0;
        float dist_r = 0;

        int i_plus = 0;
        int i_minus = 0;

        float max_cos_alpha;

        /* find  right minimum */
        while(dist_r < dmin2)
        {
            float dx, dy;
            int ind;

            if(i_plus >= n)
                goto error;

            right_near = i_plus;

            if(dist_r < dneigh2)
                ptInf[i].right_neigh = i_plus;

            i_plus++;

            ind = (i + i_plus) % n;
            dx = (float) (ptInf[i].pt.x - ptInf[ind].pt.x);
            dy = (float) (ptInf[i].pt.y - ptInf[ind].pt.y);
            dist_r = dx * dx + dy * dy;
        }
        /* find right maximum */
        while(dist_r <= dmax2)
        {
            float dx, dy;
            int ind;

            if(i_plus >= n)
                goto error;

            distance[(i + i_plus) % n] = cvSqrt(dist_r);

            if(dist_r < dneigh2)
                ptInf[i].right_neigh = i_plus;

            i_plus++;

            right_far = i_plus;

            ind = (i + i_plus) % n;

            dx = (float) (ptInf[i].pt.x - ptInf[ind].pt.x);
            dy = (float) (ptInf[i].pt.y - ptInf[ind].pt.y);
            dist_r = dx * dx + dy * dy;
        }
        right_far = i_plus;

        /* left minimum */
        while(dist_l < dmin2)
        {
            float dx, dy;
            int ind;

            if(i_minus <= -n)
                goto error;

            left_near = i_minus;

            if(dist_l < dneigh2)
                ptInf[i].left_neigh = i_minus;

            i_minus--;

            ind = i + i_minus;
            ind = (ind < 0) ? (n + ind) : ind;

            dx = (float) (ptInf[i].pt.x - ptInf[ind].pt.x);
            dy = (float) (ptInf[i].pt.y - ptInf[ind].pt.y);
            dist_l = dx * dx + dy * dy;
        }

        /* find left maximum */
        while(dist_l <= dmax2)
        {
            float dx, dy;
            int ind;

            if(i_minus <= -n)
                goto error;

            ind = i + i_minus;
            ind = (ind < 0) ? (n + ind) : ind;

            distance[ind] = cvSqrt(dist_l);

            if(dist_l < dneigh2)
                ptInf[i].left_neigh = i_minus;

            i_minus--;

            left_far = i_minus;

            ind = i + i_minus;
            ind = (ind < 0) ? (n + ind) : ind;

            dx = (float) (ptInf[i].pt.x - ptInf[ind].pt.x);
            dy = (float) (ptInf[i].pt.y - ptInf[ind].pt.y);
            dist_l = dx * dx + dy * dy;
        }
        left_far = i_minus;

        if((i_plus - i_minus) > n + 2)
            goto error;

        max_cos_alpha = -1;
        for(j = left_far + 1; j < left_near; j++)
        {
            float dx, dy;
            float a, a2;
            int leftind = i + j;

            leftind = (leftind < 0) ? (n + leftind) : leftind;

            a = distance[leftind];
            a2 = a * a;

            for(k = right_near + 1; k < right_far; k++)
            {
                int ind = (i + k) % n;
                float c2, cosalpha;
                float b = distance[ind];
                float b2 = b * b;

                /* compute cosinus */
                dx = (float) (ptInf[leftind].pt.x - ptInf[ind].pt.x);
                dy = (float) (ptInf[leftind].pt.y - ptInf[ind].pt.y);

                c2 = dx * dx + dy * dy;
                cosalpha = (a2 + b2 - c2) / (2 * a * b);

                max_cos_alpha = MAX(max_cos_alpha, cosalpha);

                if(max_cos_alpha < mincos)
                    max_cos_alpha = -1;

                sharpness[i] = max_cos_alpha;
            }
        }
    }
/*****************************************************************************************/
/*                                 Second pass                                           */
/*****************************************************************************************/

    cvStartWriteSeq((contour->flags & ~CC_SEQ_ELTYPE_MASK) | CC_SEQ_ELTYPE_INDEX,
                     sizeof(CvSeq), sizeof(int), storage, &writer);

    /* second pass - nonmaxima suppression */
    /* neighborhood of point < dneigh2 */
    for(i = 0; i < n; i++)
    {
        int suppressed = 0;
        if(sharpness[i] == -1)
            continue;

        for(j = 1; (j <= ptInf[i].right_neigh) && (suppressed == 0); j++)
        {
            if(sharpness[i] < sharpness[(i + j) % n])
                suppressed = 1;
        }

        for(j = -1; (j >= ptInf[i].left_neigh) && (suppressed == 0); j--)
        {
            int ind = i + j;

            ind = (ind < 0) ? (n + ind) : ind;
            if(sharpness[i] < sharpness[ind])
                suppressed = 1;
        }

        if(!suppressed)
            CC_WRITE_SEQ_ELEM(i, writer);
    }

    *corners = cvEndWriteSeq(&writer);

    cFree(&sharpness);
    cFree(&distance);
    cFree(&ptInf);

    return status;

  error:
    /* dmax is so big (more than contour diameter)
       that algorithm could become infinite cycle */
    cFree(&sharpness);
    cFree(&distance);
    cFree(&ptInf);

    return CC_BADRANGE_ERR;
}


/*F///////////////////////////////////////////////////////////////////////////////////////
//    Name: icvFindDominantPoints
//    Purpose:
//      Applies some algorithm to find dominant points (corners) of contour
//     
//    Context:
//    Parameters:
//      contours - pointer to input contour object.
//      out_numbers - array of dominant points indices
//      count - length of out_numbers array on input
//              and numbers of founded dominant points on output   
//                  
//      method - only CC_DOMINANT_IPAN now
//      parameters - array of parameters
//                   for IPAN algorithm
//                   [0] - minimal distance
//                   [1] - maximal distance
//                   [2] - neighborhood distance (must be not greater than dmaximal distance)
//                   [3] - maximal possible angle of curvature
//    Returns:
//      CC_OK or error code
//    Notes:
//      User must allocate out_numbers array. If it is small - function fills array 
//      with part of points and returns  error
//F*/
CC_IMPL CvSeq*
cvFindDominantPoints(CvSeq * contour, CvMemStorage * storage, int method,
                      double parameter1, double parameter2, double parameter3, double parameter4)
{
    CvSeq* corners = 0;

    CC_FUNCNAME("cvFindDominantPoints");
    __BEGIN__;

    if(!contour)
        CC_ERROR(CC_StsNullPtr, "");

    if(!storage)
        storage = contour->storage;

    if(!storage)
        CC_ERROR(CC_StsNullPtr, "");

    switch (method)
    {
    case CC_DOMINANT_IPAN:
        {
            int dmin = cRound(parameter1);
            int dmax = cRound(parameter2);
            int dneigh = cRound(parameter3);
            int amax = cRound(parameter4);

            if(amax == 0)
                amax = 150;
            if(dmin == 0)
                dmin = 7;
            if(dmax == 0)
                dmax = dmin + 2;
            if(dneigh == 0)
                dneigh = dmin;

            IPPI_CALL(icvFindDominantPointsIPAN(contour, storage, &corners,
                                                  dmin*dmin, dmax*dmax, dneigh*dneigh, (float)amax));
        }
        break;
    default:
        CC_ERROR_FROM_STATUS(CC_BADFLAG_ERR);
    }

    __END__;

    return corners;
}


