
/****************************************************************************************\
*                                  Chain Approximation                                   *
\****************************************************************************************/

typedef struct _CvPtInfo
{
    CPoint pt;
    int k;                      /* support region */
    int s;                      /* curvature value */
    struct _CvPtInfo *next;
}
_CvPtInfo;


/* curvature: 0 - 1-curvature, 1 - k-cosine curvature. */
CStatus
icvApproximateChainTC89(CChain*               chain,
                         int                    header_size,
                         CvMemStorage*          storage, 
                         CvSeq**                contour, 
                         int    method)
{
    static const int abs_diff[] = { 1, 2, 3, 4, 3, 2, 1, 0, 1, 2, 3, 4, 3, 2, 1 };

    char            local_buffer[1 << 16];
    char*           buffer = local_buffer;
    int             buffer_size;

    _CvPtInfo       temp;
    _CvPtInfo       *array, *first = 0, *current = 0, *prev_current = 0;
    int             i, j, i1, i2, s, len;
    int             count;

    CvChainPtReader reader;
    CvSeqWriter     writer;
    CPoint         pt = chain->origin;
   
    assert(chain && contour && buffer);

    buffer_size = (chain->total + 8) * sizeof(_CvPtInfo);

    *contour = 0;

    if(!CC_IS_SEQ_CHAIN_CONTOUR(chain))
        return CC_BADFLAG_ERR;

    if(header_size < (int)sizeof(CvContour))
        return CC_BADSIZE_ERR;
    
    cvStartWriteSeq((chain->flags & ~CC_SEQ_ELTYPE_MASK) | CC_SEQ_ELTYPE_POINT,
                     header_size, sizeof(CPoint), storage, &writer);
    
    if(chain->total == 0)
    {        
        CC_WRITE_SEQ_ELEM(pt, writer);
        goto exit_function;
    }

    cvStartReadChainPoints(chain, &reader);

    if(method > CC_CHAIN_APPROX_SIMPLE && buffer_size > (int)sizeof(local_buffer))
    {
        buffer = (char *) cAlloc(buffer_size);
        if(!buffer)
            return CC_OUTOFMEM_ERR;
    }

    array = (_CvPtInfo *) buffer;
    count = chain->total;

    temp.next = 0;
    current = &temp;

    /* Pass 0.
       Restores all the digital curve points from the chain code.
       Removes the points (from the resultant polygon)
       that have zero 1-curvature */
    for(i = 0; i < count; i++)
    {
        int prev_code = *reader.prev_elem;

        reader.prev_elem = reader.ptr;
        CC_READ_CHAIN_POINT(pt, reader);

        /* calc 1-curvature */
        s = abs_diff[reader.code - prev_code + 7];

        if(method <= CC_CHAIN_APPROX_SIMPLE)
        {
            if(method == CC_CHAIN_APPROX_NONE || s != 0)
            {
                CC_WRITE_SEQ_ELEM(pt, writer);
            }
        }
        else
        {
            if(s != 0)
                current = current->next = array + i;
            array[i].s = s;
            array[i].pt = pt;
        }
    }

    //assert(pt.x == chain->origin.x && pt.y == chain->origin.y);

    if(method <= CC_CHAIN_APPROX_SIMPLE)
        goto exit_function;

    current->next = 0;

    len = i;
    current = temp.next;

    assert(current);

    /* Pass 1.
       Determines support region for all the remained points */
    do
    {
        CPoint pt0;
        int k, l = 0, d_num = 0;

        i = (int)(current - array);
        pt0 = array[i].pt;

        /* determine support region */
        for(k = 1;; k++)
        {
            int lk, dk_num;
            int dx, dy;
            suf32_t d;

            assert(k <= len);

            /* calc indices */
            i1 = i - k;
            i1 += i1 < 0 ? len : 0;
            i2 = i + k;
            i2 -= i2 >= len ? len : 0;

            dx = array[i2].pt.x - array[i1].pt.x;
            dy = array[i2].pt.y - array[i1].pt.y;

            /* distance between p_(i - k) and p_(i + k) */
            lk = dx * dx + dy * dy;

            /* distance between p_i and the line (p_(i-k), p_(i+k)) */
            dk_num = (pt0.x - array[i1].pt.x) * dy - (pt0.y - array[i1].pt.y) * dx;
            d.f = (float) (((double) d_num) * lk - ((double) dk_num) * l);

            if(k > 1 && (l >= lk || (d_num > 0 && d.i <= 0 || d_num < 0 && d.i >= 0)))
                break;

            d_num = dk_num;
            l = lk;
        }

        current->k = --k;

        /* determine cosine curvature if it should be used */
        if(method == CC_CHAIN_APPROX_TC89_KCOS)
        {
            /* calc k-cosine curvature */
            for(j = k, s = 0; j > 0; j--)
            {
                double temp_num;
                int dx1, dy1, dx2, dy2;
                suf32_t sk;

                i1 = i - j;
                i1 += i1 < 0 ? len : 0;
                i2 = i + j;
                i2 -= i2 >= len ? len : 0;

                dx1 = array[i1].pt.x - pt0.x;
                dy1 = array[i1].pt.y - pt0.y;
                dx2 = array[i2].pt.x - pt0.x;
                dy2 = array[i2].pt.y - pt0.y;

                if((dx1 | dy1) == 0 || (dx2 | dy2) == 0)
                    break;

                temp_num = dx1 * dx2 + dy1 * dy2;
                temp_num =
                    (float) (temp_num /
                             sqrt(((double)dx1 * dx1 + (double)dy1 * dy1) *
                                   ((double)dx2 * dx2 + (double)dy2 * dy2)));
                sk.f = (float) (temp_num + 1.1);

                assert(0 <= sk.f && sk.f <= 2.2);
                if(j < k && sk.i <= s)
                    break;

                s = sk.i;
            }
            current->s = s;
        }
        current = current->next;
    }
    while(current != 0);

    prev_current = &temp;
    current = temp.next;

    /* Pass 2.
       Performs non-maxima supression */
    do
    {
        int k2 = current->k >> 1;

        s = current->s;
        i = (int)(current - array);

        for(j = 1; j <= k2; j++)
        {
            i2 = i - j;
            i2 += i2 < 0 ? len : 0;

            if(array[i2].s > s)
                break;

            i2 = i + j;
            i2 -= i2 >= len ? len : 0;

            if(array[i2].s > s)
                break;
        }

        if(j <= k2)           /* exclude point */
        {
            prev_current->next = current->next;
            current->s = 0;     /* "clear" point */
        }
        else
            prev_current = current;
        current = current->next;
    }
    while(current != 0);

    /* Pass 3.
       Removes non-dominant points with 1-length support region */
    current = temp.next;
    assert(current);
    prev_current = &temp;

    do
    {
        if(current->k == 1)
        {
            s = current->s;
            i = (int)(current - array);

            i1 = i - 1;
            i1 += i1 < 0 ? len : 0;

            i2 = i + 1;
            i2 -= i2 >= len ? len : 0;

            if(s <= array[i1].s || s <= array[i2].s)
            {
                prev_current->next = current->next;
                current->s = 0;
            }
            else
                prev_current = current;
        }
        else
            prev_current = current;
        current = current->next;
    }
    while(current != 0);

    if(method == CC_CHAIN_APPROX_TC89_KCOS)
        goto copy_vect;

    /* Pass 4.
       Cleans remained couples of points */
    assert(temp.next);

    if(array[0].s != 0 && array[len - 1].s != 0)      /* specific case */
    {
        for(i1 = 1; i1 < len && array[i1].s != 0; i1++)
        {
            array[i1 - 1].s = 0;
        }
        if(i1 == len)
            goto copy_vect;     /* all points survived */
        i1--;

        for(i2 = len - 2; i2 > 0 && array[i2].s != 0; i2--)
        {
            array[i2].next = 0;
            array[i2 + 1].s = 0;
        }
        i2++;

        if(i1 == 0 && i2 == len - 1)  /* only two points */
        {
            i1 = (int)(array[0].next - array);
            array[len] = array[0];      /* move to the end */
            array[len].next = 0;
            array[len - 1].next = array + len;
        }
        temp.next = array + i1;
    }

    current = temp.next;
    first = prev_current = &temp;
    count = 1;

    /* do last pass */
    do
    {
        if(current->next == 0 || current->next - current != 1)
        {
            if(count >= 2)
            {
                if(count == 2)
                {
                    int s1 = prev_current->s;
                    int s2 = current->s;

                    if(s1 > s2 || s1 == s2 && prev_current->k <= current->k)
                        /* remove second */
                        prev_current->next = current->next;
                    else
                        /* remove first */
                        first->next = current;
                }
                else
                    first->next->next = current;
            }
            first = current;
            count = 1;
        }
        else
            count++;
        prev_current = current;
        current = current->next;
    }
    while(current != 0);

  copy_vect:

    /* gather points */
    current = temp.next;
    assert(current);

    do
    {
        CC_WRITE_SEQ_ELEM(current->pt, writer);
        current = current->next;
    }
    while(current != 0);

exit_function:

    *contour = cvEndWriteSeq(&writer);

    assert(writer.seq->total > 0);

    if(buffer != local_buffer)
        cFree(&buffer);
    return CC_OK;
}


/*Applies some approximation algorithm to chain-coded contour(s) and
  converts it/them to polygonal representation */
CC_IMPL CvSeq*
cvApproxChains(CvSeq*              src_seq,
                CvMemStorage*       storage,
                int                 method,
                double              /*parameter*/, 
                int                 minimal_perimeter, 
                int                 recursive)
{
    CvSeq *prev_contour = 0, *parent = 0;
    CvSeq *dst_seq = 0;
    
    CC_FUNCNAME("cvApproxChains");

    __BEGIN__;

    if(!src_seq || !storage)
        CC_ERROR(CC_StsNullPtr, "");
    if(method > CC_CHAIN_APPROX_TC89_KCOS || method <= 0 || minimal_perimeter < 0)
        CC_ERROR(CC_StsOutOfRange, "");

    while(src_seq != 0)
    {
        int len = src_seq->total;

        if(len >= minimal_perimeter)
        {
            CvSeq *contour;
            
            switch(method)
            {
            case CC_CHAIN_APPROX_NONE:
            case CC_CHAIN_APPROX_SIMPLE:
            case CC_CHAIN_APPROX_TC89_L1:
            case CC_CHAIN_APPROX_TC89_KCOS:
                IPPI_CALL(icvApproximateChainTC89((CChain *) src_seq,
                                                    sizeof(CvContour), storage,
                                                    (CvSeq**)&contour, method));
                break;
            default:
                assert(0);
                CC_ERROR(CC_StsOutOfRange, "");
            }

            assert(contour);

            if(contour->total > 0)
            {
                cvBoundingRect(contour, 1);

                contour->v_prev = parent;
                contour->h_prev = prev_contour;

                if(prev_contour)
                    prev_contour->h_next = contour;
                else if(parent)
                    parent->v_next = contour;
                prev_contour = contour;
                if(!dst_seq)
                    dst_seq = prev_contour;
            }
            else                /* if resultant contour has zero length, skip it */
            {
                len = -1;
            }
        }

        if(!recursive)
            break;

        if(src_seq->v_next && len >= minimal_perimeter)
        {
            assert(prev_contour != 0);
            parent = prev_contour;
            prev_contour = 0;
            src_seq = src_seq->v_next;
        }
        else
        {
            while(src_seq->h_next == 0)
            {
                src_seq = src_seq->v_prev;
                if(src_seq == 0)
                    break;
                prev_contour = parent;
                if(parent)
                    parent = parent->v_prev;
            }
            if(src_seq)
                src_seq = src_seq->h_next;
        }
    }

    __END__;

    return dst_seq;
}


/****************************************************************************************\
*                               Polygonal Approximation                                  *
\****************************************************************************************/

/* Ramer-Douglas-Peucker algorithm for polygon simplification */

/* the version for integer point coordinates */
static CStatus
icvApproxPolyDP_32s(CvSeq* src_contour, int header_size, 
                     CvMemStorage* storage,
                     CvSeq** dst_contour, float eps)
{
    int             init_iters = 3;
    CSlice         slice = {0, 0}, right_slice = {0, 0};
    CvSeqReader     reader, reader2;
    CvSeqWriter     writer;
    CPoint         start_pt = {INT_MIN, INT_MIN}, end_pt = {0, 0}, pt = {0,0};
    int             i = 0, j, count = src_contour->total, new_count;
    int             is_closed = CC_IS_SEQ_CLOSED(src_contour);
    int             le_eps = 0;
    CvMemStorage*   temp_storage = 0;
    CvSeq*          stack = 0;
    
    assert(CC_SEQ_ELTYPE(src_contour) == CC_32SC2);
    cvStartWriteSeq(src_contour->flags, header_size, sizeof(pt), storage, &writer);

    if(src_contour->total == 0)
    {
        *dst_contour = cvEndWriteSeq(&writer);
        return CC_OK;
    }

    temp_storage = cvCreateChildMemStorage(storage);

    assert(src_contour->first != 0);
    stack = cvCreateSeq(0, sizeof(CvSeq), sizeof(CSlice), temp_storage);
    eps *= eps;
    cvStartReadSeq(src_contour, &reader, 0);

    if(!is_closed)
    {
        right_slice.start_index = count;
        end_pt = *(CPoint*)(reader.ptr);
        start_pt = *(CPoint*)cvGetSeqElem(src_contour, -1);

        if(start_pt.x != end_pt.x || start_pt.y != end_pt.y)
        {
            slice.start_index = 0;
            slice.end_index = count - 1;
            cvSeqPush(stack, &slice);
        }
        else
        {
            is_closed = 1;
            init_iters = 1;
        }
    }
    
    if(is_closed)
    {
        /* 1. Find approximately two farthest points of the contour */
        right_slice.start_index = 0;

        for(i = 0; i < init_iters; i++)
        {
            int max_dist = 0;
            cvSetSeqReaderPos(&reader, right_slice.start_index, 1);
            CC_READ_SEQ_ELEM(start_pt, reader);   /* read the first point */

            for(j = 1; j < count; j++)
            {
                int dx, dy, dist;

                CC_READ_SEQ_ELEM(pt, reader);
                dx = pt.x - start_pt.x;
                dy = pt.y - start_pt.y;

                dist = dx * dx + dy * dy;

                if(dist > max_dist)
                {
                    max_dist = dist;
                    right_slice.start_index = j;
                }
            }

            le_eps = max_dist <= eps;
        }

        /* 2. initialize the stack */
        if(!le_eps)
        {
            slice.start_index = cvGetSeqReaderPos(&reader);
            slice.end_index = right_slice.start_index += slice.start_index;

            right_slice.start_index -= right_slice.start_index >= count ? count : 0;
            right_slice.end_index = slice.start_index;
            if(right_slice.end_index < right_slice.start_index)
                right_slice.end_index += count;

            cvSeqPush(stack, &right_slice);
            cvSeqPush(stack, &slice);
        }
        else
            CC_WRITE_SEQ_ELEM(start_pt, writer);
    }

    /* 3. run recursive process */
    while(stack->total != 0)
    {
        cvSeqPop(stack, &slice);

        if(slice.end_index > slice.start_index + 1)
        {
            int dx, dy, dist, max_dist = 0;
            
            cvSetSeqReaderPos(&reader, slice.end_index);
            CC_READ_SEQ_ELEM(end_pt, reader);

            cvSetSeqReaderPos(&reader, slice.start_index);
            CC_READ_SEQ_ELEM(start_pt, reader);

            dx = end_pt.x - start_pt.x;
            dy = end_pt.y - start_pt.y;

            assert(dx != 0 || dy != 0);

            for(i = slice.start_index + 1; i < slice.end_index; i++)
            {
                CC_READ_SEQ_ELEM(pt, reader);
                dist = abs((pt.y - start_pt.y) * dx - (pt.x - start_pt.x) * dy);

                if(dist > max_dist)
                {
                    max_dist = dist;
                    right_slice.start_index = i;
                }
            }

            le_eps = (double)max_dist * max_dist <= eps * ((double)dx * dx + (double)dy * dy);
        }
        else
        {
            assert(slice.end_index > slice.start_index);
            le_eps = 1;
            /* read starting point */
            cvSetSeqReaderPos(&reader, slice.start_index);
            CC_READ_SEQ_ELEM(start_pt, reader);
        }

        if(le_eps)
        {
            CC_WRITE_SEQ_ELEM(start_pt, writer);
        }
        else
        {
            right_slice.end_index = slice.end_index;
            slice.end_index = right_slice.start_index;
            cvSeqPush(stack, &right_slice);
            cvSeqPush(stack, &slice);
        }
    }

    is_closed = CC_IS_SEQ_CLOSED(src_contour);
    if(!is_closed)
        CC_WRITE_SEQ_ELEM(end_pt, writer);

    *dst_contour = cvEndWriteSeq(&writer);
    
    cvStartReadSeq(*dst_contour, &reader, is_closed);
    CC_READ_SEQ_ELEM(start_pt, reader);

    reader2 = reader;
    CC_READ_SEQ_ELEM(pt, reader);

    new_count = count = (*dst_contour)->total;
    for(i = !is_closed; i < count - !is_closed && new_count > 2; i++)
    {
        int dx, dy, dist;
        CC_READ_SEQ_ELEM(end_pt, reader);

        dx = end_pt.x - start_pt.x;
        dy = end_pt.y - start_pt.y;
        dist = abs((pt.x - start_pt.x)*dy - (pt.y - start_pt.y)*dx);
        if((double)dist * dist <= 0.5*eps*((double)dx*dx + (double)dy*dy) && dx != 0 && dy != 0)
        {
            new_count--;
            *((CPoint*)reader2.ptr) = start_pt = end_pt;
            CC_NEXT_SEQ_ELEM(sizeof(pt), reader2);
            CC_READ_SEQ_ELEM(pt, reader);
            i++;
            continue;
        }
        *((CPoint*)reader2.ptr) = start_pt = pt;
        CC_NEXT_SEQ_ELEM(sizeof(pt), reader2);
        pt = end_pt;
    }

    if(!is_closed)
        *((CPoint*)reader2.ptr) = pt;

    if(new_count < count)
        cvSeqPopMulti(*dst_contour, 0, count - new_count);

    cvReleaseMemStorage(&temp_storage);

    return CC_OK;
}


/* the version for integer point coordinates */
static CStatus
icvApproxPolyDP_32f(CvSeq* src_contour, int header_size, 
                     CvMemStorage* storage,
                     CvSeq** dst_contour, float eps)
{
    int             init_iters = 3;
    CSlice         slice = {0, 0}, right_slice = {0, 0};
    CvSeqReader     reader, reader2;
    CvSeqWriter     writer;
    CPoint2D32f    start_pt = {-1e6f, -1e6f}, end_pt = {0, 0}, pt = {0,0};
    int             i = 0, j, count = src_contour->total, new_count;
    int             is_closed = CC_IS_SEQ_CLOSED(src_contour);
    int             le_eps = 0;
    CvMemStorage*   temp_storage = 0;
    CvSeq*          stack = 0;
    
    assert(CC_SEQ_ELTYPE(src_contour) == CC_32FC2);
    cvStartWriteSeq(src_contour->flags, header_size, sizeof(pt), storage, &writer);

    if(src_contour->total == 0)
    {
        *dst_contour = cvEndWriteSeq(&writer);
        return CC_OK;
    }

    temp_storage = cvCreateChildMemStorage(storage);

    assert(src_contour->first != 0);
    stack = cvCreateSeq(0, sizeof(CvSeq), sizeof(CSlice), temp_storage);
    eps *= eps;
    cvStartReadSeq(src_contour, &reader, 0);

    if(!is_closed)
    {
        right_slice.start_index = count;
        end_pt = *(CPoint2D32f*)(reader.ptr);
        start_pt = *(CPoint2D32f*)cvGetSeqElem(src_contour, -1);

        if(fabs(start_pt.x - end_pt.x) > FLT_EPSILON ||
            fabs(start_pt.y - end_pt.y) > FLT_EPSILON)
        {
            slice.start_index = 0;
            slice.end_index = count - 1;
            cvSeqPush(stack, &slice);
        }
        else
        {
            is_closed = 1;
            init_iters = 1;
        }
    }
    
    if(is_closed)
    {
        /* 1. Find approximately two farthest points of the contour */
        right_slice.start_index = 0;

        for(i = 0; i < init_iters; i++)
        {
            double max_dist = 0;
            cvSetSeqReaderPos(&reader, right_slice.start_index, 1);
            CC_READ_SEQ_ELEM(start_pt, reader);   /* read the first point */

            for(j = 1; j < count; j++)
            {
                double dx, dy, dist;

                CC_READ_SEQ_ELEM(pt, reader);
                dx = pt.x - start_pt.x;
                dy = pt.y - start_pt.y;

                dist = dx * dx + dy * dy;

                if(dist > max_dist)
                {
                    max_dist = dist;
                    right_slice.start_index = j;
                }
            }

            le_eps = max_dist <= eps;
        }

        /* 2. initialize the stack */
        if(!le_eps)
        {
            slice.start_index = cvGetSeqReaderPos(&reader);
            slice.end_index = right_slice.start_index += slice.start_index;

            right_slice.start_index -= right_slice.start_index >= count ? count : 0;
            right_slice.end_index = slice.start_index;
            if(right_slice.end_index < right_slice.start_index)
                right_slice.end_index += count;

            cvSeqPush(stack, &right_slice);
            cvSeqPush(stack, &slice);
        }
        else
            CC_WRITE_SEQ_ELEM(start_pt, writer);
    }

    /* 3. run recursive process */
    while(stack->total != 0)
    {
        cvSeqPop(stack, &slice);

        if(slice.end_index > slice.start_index + 1)
        {
            double dx, dy, dist, max_dist = 0;
            
            cvSetSeqReaderPos(&reader, slice.end_index);
            CC_READ_SEQ_ELEM(end_pt, reader);

            cvSetSeqReaderPos(&reader, slice.start_index);
            CC_READ_SEQ_ELEM(start_pt, reader);
            
            dx = end_pt.x - start_pt.x;
            dy = end_pt.y - start_pt.y;

            assert(dx != 0 || dy != 0);

            for(i = slice.start_index + 1; i < slice.end_index; i++)
            {
                CC_READ_SEQ_ELEM(pt, reader);
                dist = fabs((pt.y - start_pt.y) * dx - (pt.x - start_pt.x) * dy);

                if(dist > max_dist)
                {
                    max_dist = dist;
                    right_slice.start_index = i;
                }
            }

            le_eps = (double)max_dist * max_dist <= eps * ((double)dx * dx + (double)dy * dy);
        }
        else
        {
            assert(slice.end_index > slice.start_index);
            le_eps = 1;
            /* read starting point */
            cvSetSeqReaderPos(&reader, slice.start_index);
            CC_READ_SEQ_ELEM(start_pt, reader);
        }

        if(le_eps)
        {
            CC_WRITE_SEQ_ELEM(start_pt, writer);
        }
        else
        {
            right_slice.end_index = slice.end_index;
            slice.end_index = right_slice.start_index;
            cvSeqPush(stack, &right_slice);
            cvSeqPush(stack, &slice);
        }
    }

    is_closed = CC_IS_SEQ_CLOSED(src_contour);
    if(!is_closed)
        CC_WRITE_SEQ_ELEM(end_pt, writer);

    *dst_contour = cvEndWriteSeq(&writer);
    
    cvStartReadSeq(*dst_contour, &reader, is_closed);
    CC_READ_SEQ_ELEM(start_pt, reader);

    reader2 = reader;
    CC_READ_SEQ_ELEM(pt, reader);

    new_count = count = (*dst_contour)->total;
    for(i = !is_closed; i < count - !is_closed && new_count > 2; i++)
    {
        double dx, dy, dist;
        CC_READ_SEQ_ELEM(end_pt, reader);

        dx = end_pt.x - start_pt.x;
        dy = end_pt.y - start_pt.y;
        dist = fabs((pt.x - start_pt.x)*dy - (pt.y - start_pt.y)*dx);
        if((double)dist * dist <= 0.5*eps*((double)dx*dx + (double)dy*dy))
        {
            new_count--;
            *((CPoint2D32f*)reader2.ptr) = start_pt = end_pt;
            CC_NEXT_SEQ_ELEM(sizeof(pt), reader2);
            CC_READ_SEQ_ELEM(pt, reader);
            i++;
            continue;
        }
        *((CPoint2D32f*)reader2.ptr) = start_pt = pt;
        CC_NEXT_SEQ_ELEM(sizeof(pt), reader2);
        pt = end_pt;
    }

    if(!is_closed)
        *((CPoint2D32f*)reader2.ptr) = pt;

    if(new_count < count)
        cvSeqPopMulti(*dst_contour, 0, count - new_count);

    cvReleaseMemStorage(&temp_storage);

    return CC_OK;
}


CC_IMPL CvSeq*
cvApproxPoly(const void*  array, int  header_size,
              CvMemStorage*  storage, int  method, 
              double  parameter, int parameter2)
{
    CvSeq* dst_seq = 0;
    CvSeq *prev_contour = 0, *parent = 0;
    CvContour contour_header;
    CvSeq* src_seq = 0;
    CvSeqBlock block;
    int recursive = 0;

    CC_FUNCNAME("cvApproxPoly");

    __BEGIN__;

    if(CC_IS_SEQ(array))
    {
        src_seq = (CvSeq*)array;
        if(!CC_IS_SEQ_POLYLINE(src_seq))
            CC_ERROR(CC_StsBadArg, "Unsupported sequence type");

        recursive = parameter2;

        if(!storage)
            storage = src_seq->storage;
    }
    else
    {
        CC_CALL(src_seq = cvPointSeqFromMat(
            CC_SEQ_KIND_CURVE | (parameter2 ? CC_SEQ_FLAG_CLOSED : 0),
            array, &contour_header, &block));
    }

    if(!storage)
        CC_ERROR(CC_StsNullPtr, "NULL storage pointer ");

    if(header_size < 0)
        CC_ERROR(CC_StsOutOfRange, "header_size is negative. "
                  "Pass 0 to make the destination header_size == input header_size");

    if(header_size == 0)
        header_size = src_seq->header_size;

    if(!CC_IS_SEQ_POLYLINE(src_seq))
    {
        if(CC_IS_SEQ_CHAIN(src_seq))
        {
            CC_ERROR(CC_StsBadArg, "Input curves are not polygonal. "
                                    "Use cvApproxChains first");
        }
        else
        {
            CC_ERROR(CC_StsBadArg, "Input curves have uknown type");
        }
    }

    if(header_size == 0)
        header_size = src_seq->header_size;

    if(header_size < (int)sizeof(CvContour))
        CC_ERROR(CC_StsBadSize, "New header size must be non-less than sizeof(CvContour)");

    if(method != CC_POLY_APPROX_DP)
        CC_ERROR(CC_StsOutOfRange, "Unknown approximation method");

    while(src_seq != 0)
    {
        CvSeq *contour = 0;

        switch (method)
        {
        case CC_POLY_APPROX_DP:
            if(parameter < 0)
                CC_ERROR(CC_StsOutOfRange, "Accuracy must be non-negative");

            if(CC_SEQ_ELTYPE(src_seq) == CC_32SC2)
            {
                IPPI_CALL(icvApproxPolyDP_32s(src_seq, header_size, storage,
                                                &contour, (float)parameter));
            }
            else
            {
                IPPI_CALL(icvApproxPolyDP_32f(src_seq, header_size, storage,
                                                &contour, (float)parameter));
            }
            break;
        default:
            assert(0);
            CC_ERROR(CC_StsBadArg, "Invalid approximation method");
        }

        assert(contour);

        if(header_size >= (int)sizeof(CvContour))
            cvBoundingRect(contour, 1);

        contour->v_prev = parent;
        contour->h_prev = prev_contour;

        if(prev_contour)
            prev_contour->h_next = contour;
        else if(parent)
            parent->v_next = contour;
        prev_contour = contour;
        if(!dst_seq)
            dst_seq = prev_contour;

        if(!recursive)
            break;

        if(src_seq->v_next)
        {
            assert(prev_contour != 0);
            parent = prev_contour;
            prev_contour = 0;
            src_seq = src_seq->v_next;
        }
        else
        {
            while(src_seq->h_next == 0)
            {
                src_seq = src_seq->v_prev;
                if(src_seq == 0)
                    break;
                prev_contour = parent;
                if(parent)
                    parent = parent->v_prev;
            }
            if(src_seq)
                src_seq = src_seq->h_next;
        }
    }

    __END__;

    return dst_seq;
}


