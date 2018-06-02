
static int
icvSklansky_32s(CPoint** array, int start, int end, int* stack, int nsign, int sign2)
{
    int incr = end > start ? 1 : -1;
    /* prepare first triangle */
    int pprev = start, pcur = pprev + incr, pnext = pcur + incr;
    int stacksize = 3;

    if(start == end ||
        (array[start]->x == array[end]->x &&
         array[start]->y == array[end]->y))
    {
        stack[0] = start;
        return 1;
    }

    stack[0] = pprev;
    stack[1] = pcur;
    stack[2] = pnext;

    end += incr; /* make end = afterend */

    while(pnext != end)
    {
        /* check the angle p1,p2,p3 */
        int cury = array[pcur]->y;
        int nexty = array[pnext]->y;
        int by = nexty - cury;

        if(CC_SIGN(by) != nsign)
        {
            int ax = array[pcur]->x - array[pprev]->x;
            int bx = array[pnext]->x - array[pcur]->x;
            int ay = cury - array[pprev]->y;
            int convexity = ay*bx - ax*by;/* if >0 then convex angle */

            if(CC_SIGN(convexity) == sign2 && (ax != 0 || ay != 0))
            {
                pprev = pcur;
                pcur = pnext;
                pnext += incr;
                stack[stacksize] = pnext;
                stacksize++;
            }
            else
            {
                if(pprev == start)
                {
                    pcur = pnext;
                    stack[1] = pcur;
                    pnext += incr;
                    stack[2] = pnext;
                }
                else
                {
                    stack[stacksize-2] = pnext;
                    pcur = pprev;
                    pprev = stack[stacksize-4];
                    stacksize--;
                }
            }
        }
        else
        {
            pnext += incr;
            stack[stacksize-1] = pnext;
        }
    }

    return --stacksize;
}


static int
icvSklansky_32f(CPoint2D32f** array, int start, int end, int* stack, int nsign, int sign2)
{
    int incr = end > start ? 1 : -1;
    /* prepare first triangle */
    int pprev = start, pcur = pprev + incr, pnext = pcur + incr;
    int stacksize = 3;

    if(start == end ||
        (array[start]->x == array[end]->x &&
         array[start]->y == array[end]->y))
    {
        stack[0] = start;
        return 1;
    }

    stack[0] = pprev;
    stack[1] = pcur;
    stack[2] = pnext;

    end += incr; /* make end = afterend */

    while(pnext != end)
    {
        /* check the angle p1,p2,p3 */
        float cury = array[pcur]->y;
        float nexty = array[pnext]->y;
        float by = nexty - cury;

        if(CC_SIGN(by) != nsign)
        {
            float ax = array[pcur]->x - array[pprev]->x;
            float bx = array[pnext]->x - array[pcur]->x;
            float ay = cury - array[pprev]->y;
            float convexity = ay*bx - ax*by;/* if >0 then convex angle */

            if(CC_SIGN(convexity) == sign2 && (ax != 0 || ay != 0))
            {
                pprev = pcur;
                pcur = pnext;
                pnext += incr;
                stack[stacksize] = pnext;
                stacksize++;
            }
            else
            {
                if(pprev == start)
                {
                    pcur = pnext;
                    stack[1] = pcur;
                    pnext += incr;
                    stack[2] = pnext;

                }
                else
                {
                    stack[stacksize-2] = pnext;
                    pcur = pprev;
                    pprev = stack[stacksize-4];
                    stacksize--;
                }
            }
        }
        else
        {
            pnext += incr;
            stack[stacksize-1] = pnext;
        }
    }

    return --stacksize;
}

typedef int (*sklansky_func)(CPoint** points, int start, int end,
                              int* stack, int sign, int sign2);

#define cmp_pts(pt1, pt2)  \
    ((pt1)->x < (pt2)->x || (pt1)->x <= (pt2)->x && (pt1)->y < (pt2)->y)
static CC_IMPLEMENT_QSORT(icvSortPointsByPointers_32s, CPoint*, cmp_pts)
static CC_IMPLEMENT_QSORT(icvSortPointsByPointers_32f, CPoint2D32f*, cmp_pts)

static void
icvCalcAndWritePtIndices(CPoint** pointer, int* stack, int start, int end,
                          CvSeq* ptseq, CvSeqWriter* writer)
{
    CC_FUNCNAME("icvCalcAndWritePtIndices");

    __BEGIN__;
    
    int i, incr = start < end ? 1 : -1;
    int idx, first_idx = ptseq->first->start_index;

    for(i = start; i != end; i += incr)
    {
        CPoint* ptr = (CPoint*)pointer[stack[i]];
        CvSeqBlock* block = ptseq->first;
        while((unsigned)(idx = (int)(ptr - (CPoint*)block->data)) >= (unsigned)block->count)
        {
            block = block->next;
            if(block == ptseq->first)
                CC_ERROR(CC_StsError, "Internal error");
        }
        idx += block->start_index - first_idx;
        CC_WRITE_SEQ_ELEM(idx, *writer);
    }

    __END__;
}


CC_IMPL CvSeq*
cvConvexHull2(const img_t* array, void* hull_storage,
               int orientation, int return_points)
{
    union { CvContour* c; CvSeq* s; } hull;
    CPoint** pointer = 0;
    CPoint2D32f** pointerf = 0;
    int* stack = 0;
    
    CC_FUNCNAME("cvConvexHull2");
    
    hull.s = 0;

    __BEGIN__;

    img_t* mat = 0;
    CvSeqReader reader;
    CvSeqWriter writer;
    CvContour contour_header;
    union { CvContour c; CvSeq s; } hull_header;
    CvSeqBlock block, hullblock;
    CvSeq* ptseq = 0;
    CvSeq* hullseq = 0;
    int is_float;
    int* t_stack;
    int t_count;
    int i, miny_ind = 0, maxy_ind = 0, total;
    int hulltype;
    int stop_idx;
    sklansky_func sklansky;

    if(CC_IS_SEQ(array))
    {
        ptseq = (CvSeq*)array;
        if(!CC_IS_SEQ_POINT_SET(ptseq))
            CC_ERROR(CC_StsBadArg, "Unsupported sequence type");
        if(hull_storage == 0)
            hull_storage = ptseq->storage;
    }
    else
    {
        CC_CALL(ptseq = cvPointSeqFromMat(
            CC_SEQ_KIND_GENERIC, array, &contour_header, &block));
    }

    if(CC_IS_STORAGE(hull_storage))
    {
        if(return_points)
        {
            CC_CALL(hullseq = cvCreateSeq(
                CC_SEQ_KIND_CURVE|CC_SEQ_ELTYPE(ptseq)|
                CC_SEQ_FLAG_CLOSED|CC_SEQ_FLAG_CONVEX,
                sizeof(CvContour), sizeof(CPoint),(CvMemStorage*)hull_storage));
        }
        else
        {
            CC_CALL(hullseq = cvCreateSeq(
                CC_SEQ_KIND_CURVE|CC_SEQ_ELTYPE_PPOINT|
                CC_SEQ_FLAG_CLOSED|CC_SEQ_FLAG_CONVEX,
                sizeof(CvContour), sizeof(CPoint*), (CvMemStorage*)hull_storage));
        }
    }
    else
    {
        if(!CC_IS_MAT(hull_storage))
            CC_ERROR(CC_StsBadArg, "Destination must be valid memory storage or matrix");

        mat = hull_storage;

        if(mat->cols != 1 && mat->rows != 1 || !CC_IS_MAT_CONT(mat->tid))
            CC_ERROR(CC_StsBadArg,
            "The hull matrix should be continuous and have a single row or a single column");

        if(mat->cols + mat->rows - 1 < ptseq->total)
            CC_ERROR(CC_StsBadSize, "The hull matrix size might be not enough to fit the hull");

        if(CC_MAT_TYPE(mat) != CC_SEQ_ELTYPE(ptseq) &&
            CC_MAT_TYPE(mat) != CC_32SC1)
            CC_ERROR(CC_StsUnsupportedFormat,
            "The hull matrix must have the same type as input or 32sC1 (integers)");

        CC_CALL(hullseq = cvMakeSeqHeaderForArray(
            CC_SEQ_KIND_CURVE|CC_MAT_TYPE(mat)|CC_SEQ_FLAG_CLOSED,
            sizeof(contour_header), CC_ELEM_SIZE(mat->tid), mat->tt.data,
            mat->cols + mat->rows - 1, &hull_header.s, &hullblock));

        cvClearSeq(hullseq);
    }

    total = ptseq->total;
    if(total == 0)
    {
        if(mat)
            CC_ERROR(CC_StsBadSize,
            "Point sequence can not be empty if the output is matrix");
        EXIT;
    }

    cvStartAppendToSeq(hullseq, &writer);

    is_float = CC_SEQ_ELTYPE(ptseq) == CC_32FC2;
    hulltype = CC_SEQ_ELTYPE(hullseq);
    sklansky = !is_float ? (sklansky_func)icvSklansky_32s :
                           (sklansky_func)icvSklansky_32f;

    CC_CALL(pointer = (CPoint**)cAlloc(ptseq->total*sizeof(pointer[0])));
    CC_CALL(stack = (int*)cAlloc((ptseq->total + 2)*sizeof(stack[0])));
    pointerf = (CPoint2D32f**)pointer;

    cvStartReadSeq(ptseq, &reader);

    for(i = 0; i < total; i++)
    {
        pointer[i] = (CPoint*)reader.ptr;
        CC_NEXT_SEQ_ELEM(ptseq->elem_size, reader);
    }

    // sort the point set by x-coordinate, find min and max y
    if(!is_float)
    {
        icvSortPointsByPointers_32s(pointer, total, 0);
        for(i = 1; i < total; i++)
        {
            int y = pointer[i]->y;
            if(pointer[miny_ind]->y > y)
                miny_ind = i;
            if(pointer[maxy_ind]->y < y)
                maxy_ind = i;
        }
    }
    else
    {
        icvSortPointsByPointers_32f(pointerf, total, 0);
        for(i = 1; i < total; i++)
        {
            float y = pointerf[i]->y;
            if(pointerf[miny_ind]->y > y)
                miny_ind = i;
            if(pointerf[maxy_ind]->y < y)
                maxy_ind = i;
        }
    }

    if(pointer[0]->x == pointer[total-1]->x &&
        pointer[0]->y == pointer[total-1]->y)
    {
        if(hulltype == CC_SEQ_ELTYPE_PPOINT)
        {
            CC_WRITE_SEQ_ELEM(pointer[0], writer);
        }
        else if(hulltype == CC_SEQ_ELTYPE_INDEX)
        {
            int index = 0;
            CC_WRITE_SEQ_ELEM(index, writer);
        }
        else
        {
            CPoint pt = pointer[0][0];
            CC_WRITE_SEQ_ELEM(pt, writer);
        }
        goto finish_hull;
    }

    /*upper half */
    {
        int *tl_stack = stack;
        int tl_count = sklansky(pointer, 0, maxy_ind, tl_stack, -1, 1);
        int *tr_stack = tl_stack + tl_count;
        int tr_count = sklansky(pointer, ptseq->total - 1, maxy_ind, tr_stack, -1, -1);

        /* gather upper part of convex hull to output */
        if(orientation == CC_COUNTER_CLOCKWISE)
        {
            CC_SWAP(tl_stack, tr_stack, t_stack);
            CC_SWAP(tl_count, tr_count, t_count);
        }

        if(hulltype == CC_SEQ_ELTYPE_PPOINT)
        {
            for(i = 0; i < tl_count - 1; i++)
                CC_WRITE_SEQ_ELEM(pointer[tl_stack[i]], writer);

            for(i = tr_count - 1; i > 0; i--)
                CC_WRITE_SEQ_ELEM(pointer[tr_stack[i]], writer);
        }
        else if(hulltype == CC_SEQ_ELTYPE_INDEX)
        {
            CC_CALL(icvCalcAndWritePtIndices(pointer, tl_stack,
                                               0, tl_count-1, ptseq, &writer));
            CC_CALL(icvCalcAndWritePtIndices(pointer, tr_stack,
                                               tr_count-1, 0, ptseq, &writer));
        }
        else
        {
            for(i = 0; i < tl_count - 1; i++)
                CC_WRITE_SEQ_ELEM(pointer[tl_stack[i]][0], writer);

            for(i = tr_count - 1; i > 0; i--)
                CC_WRITE_SEQ_ELEM(pointer[tr_stack[i]][0], writer);
        }
        stop_idx = tr_count > 2 ? tr_stack[1] : tl_count > 2 ? tl_stack[tl_count - 2] : -1;
    }

    /* lower half */
    {
        int *bl_stack = stack;
        int bl_count = sklansky(pointer, 0, miny_ind, bl_stack, 1, -1);
        int *br_stack = stack + bl_count;
        int br_count = sklansky(pointer, ptseq->total - 1, miny_ind, br_stack, 1, 1);

        if(orientation != CC_COUNTER_CLOCKWISE)
        {
            CC_SWAP(bl_stack, br_stack, t_stack);
            CC_SWAP(bl_count, br_count, t_count);
        }

        if(stop_idx >= 0)
        {
            int check_idx = bl_count > 2 ? bl_stack[1] :
                            bl_count + br_count > 2 ? br_stack[2-bl_count] : -1;
            if(check_idx == stop_idx || check_idx >= 0 &&
                pointer[check_idx]->x == pointer[stop_idx]->x &&
                pointer[check_idx]->y == pointer[stop_idx]->y)
            {
                /* if all the points lie on the same line, then
                   the bottom part of the convex hull is the mirrored top part
                   (except the exteme points).*/
                bl_count = MIN(bl_count, 2);
                br_count = MIN(br_count, 2);
            }
        }

        if(hulltype == CC_SEQ_ELTYPE_PPOINT)
        {
            for(i = 0; i < bl_count - 1; i++)
                CC_WRITE_SEQ_ELEM(pointer[bl_stack[i]], writer);

            for(i = br_count - 1; i > 0; i--)
                CC_WRITE_SEQ_ELEM(pointer[br_stack[i]], writer);
        }
        else if(hulltype == CC_SEQ_ELTYPE_INDEX)
        {
            CC_CALL(icvCalcAndWritePtIndices(pointer, bl_stack,
                                               0, bl_count-1, ptseq, &writer));
            CC_CALL(icvCalcAndWritePtIndices(pointer, br_stack,
                                               br_count-1, 0, ptseq, &writer));
        }
        else
        {
            for(i = 0; i < bl_count - 1; i++)
                CC_WRITE_SEQ_ELEM(pointer[bl_stack[i]][0], writer);

            for(i = br_count - 1; i > 0; i--)
                CC_WRITE_SEQ_ELEM(pointer[br_stack[i]][0], writer);
        }
    }

finish_hull:
    CC_CALL(cvEndWriteSeq(&writer));

    if(mat)
    {
        if(mat->rows > mat->cols)
            mat->rows = hullseq->total;
        else
            mat->cols = hullseq->total;
    }
    else
    {
        hull.s = hullseq;
        hull.c->rect = cvBoundingRect(ptseq,
            ptseq->header_size < (int)sizeof(CvContour) ||
            &ptseq->flags == &contour_header.flags);
        
        /*if(ptseq != (CvSeq*)&contour_header)
            hullseq->v_prev = ptseq;*/
    }

    __END__;

    cFree(&pointer);
    cFree(&stack);

    return hull.s;
}


/* contour must be a simple polygon */
/* it must have more than 3 points  */
CC_IMPL CvSeq*
cvConvexityDefects(const img_t* array,
                    const img_t* hullarray,
                    CvMemStorage* storage)
{
    CvSeq* defects = 0;
    
    CC_FUNCNAME("cvConvexityDefects");

    __BEGIN__;

    int i, index;
    CPoint* hull_cur;

    /* is orientation of hull different from contour one */
    int rev_orientation;

    CvContour contour_header;
    union { CvContour c; CvSeq s; } hull_header;
    CvSeqBlock block, hullblock;
    CvSeq *ptseq = (CvSeq*)array, *hull = (CvSeq*)hullarray;

    CvSeqReader hull_reader;
    CvSeqReader ptseq_reader;
    CvSeqWriter writer;
    int is_index;

    if(CC_IS_SEQ(ptseq))
    {
        if(!CC_IS_SEQ_POINT_SET(ptseq))
            CC_ERROR(CC_StsUnsupportedFormat,
                "Input sequence is not a sequence of points");
        if(!storage)
            storage = ptseq->storage;
    }
    else
    {
        CC_CALL(ptseq = cvPointSeqFromMat(
            CC_SEQ_KIND_GENERIC, array, &contour_header, &block));
    }

    if(CC_SEQ_ELTYPE(ptseq) != CC_32SC2)
        CC_ERROR(CC_StsUnsupportedFormat,
            "Floating-point coordinates are not supported here");

    if(CC_IS_SEQ(hull))
    {
        int hulltype = CC_SEQ_ELTYPE(hull);
        if(hulltype != CC_SEQ_ELTYPE_PPOINT && hulltype != CC_SEQ_ELTYPE_INDEX)
            CC_ERROR(CC_StsUnsupportedFormat,
                "Convex hull must represented as a sequence "
                "of indices or sequence of pointers");
        if(!storage)
            storage = hull->storage;
    }
    else
    {
        img_t* mat = hull;
        
        if(!CC_IS_MAT(hull))
            CC_ERROR(CC_StsBadArg, "Convex hull is neither sequence nor matrix");

        if(mat->cols != 1 && mat->rows != 1 ||
            !CC_IS_MAT_CONT(mat->tid) || CC_MAT_TYPE(mat) != CC_32SC1)
            CC_ERROR(CC_StsBadArg,
            "The matrix should be 1-dimensional and continuous array of int's");

        if(mat->cols + mat->rows - 1 > ptseq->total)
            CC_ERROR(CC_StsBadSize, "Convex hull is larger than the point sequence");
        
        CC_CALL(hull = cvMakeSeqHeaderForArray(
            CC_SEQ_KIND_CURVE|CC_MAT_TYPE(mat)|CC_SEQ_FLAG_CLOSED,
            sizeof(CvContour), CC_ELEM_SIZE(mat->tid), mat->tt.data,
            mat->cols + mat->rows - 1, &hull_header.s, &hullblock));
    }

    is_index = CC_SEQ_ELTYPE(hull) == CC_SEQ_ELTYPE_INDEX;

    if(!storage)
        CC_ERROR(CC_StsNullPtr, "NULL storage pointer");

    CC_CALL(defects = cvCreateSeq(CC_SEQ_KIND_GENERIC, sizeof(CvSeq),
                                    sizeof(CvConvexityDefect), storage));

    if(ptseq->total < 4 || hull->total < 3)
    {
        //CC_ERROR(CC_StsBadSize,
        //    "point seq size must be >= 4, convex hull size must be >= 3");
        EXIT;
    }

    /* recognize co-orientation of ptseq and its hull */
    {
        int sign = 0;
        int index1, index2, index3;

        if(!is_index)
        {
            CPoint* pos = *CC_SEQ_ELEM(hull, CPoint*, 0);
            CC_CALL(index1 = cvSeqElemIdx(ptseq, pos));

            pos = *CC_SEQ_ELEM(hull, CPoint*, 1);
            CC_CALL(index2 = cvSeqElemIdx(ptseq, pos));

            pos = *CC_SEQ_ELEM(hull, CPoint*, 2);
            CC_CALL(index3 = cvSeqElemIdx(ptseq, pos));
        }
        else
        {
            index1 = *CC_SEQ_ELEM(hull, int, 0);
            index2 = *CC_SEQ_ELEM(hull, int, 1);
            index3 = *CC_SEQ_ELEM(hull, int, 2);
        }

        sign += (index2 > index1) ? 1 : 0;
        sign += (index3 > index2) ? 1 : 0;
        sign += (index1 > index3) ? 1 : 0;

        rev_orientation = (sign == 2) ? 0 : 1;
    }

    cvStartReadSeq(ptseq, &ptseq_reader, 0);
    cvStartReadSeq(hull, &hull_reader, rev_orientation);

    if(!is_index)
    {
        hull_cur = *(CPoint**)hull_reader.prev_elem;
        index = cvSeqElemIdx(ptseq, (char*)hull_cur, 0);
    }
    else
    {
        index = *(int*)hull_reader.prev_elem;
        hull_cur = CC_GET_SEQ_ELEM(CPoint, ptseq, index);
    }
    cvSetSeqReaderPos(&ptseq_reader, index);
    cvStartAppendToSeq(defects, &writer);

    /* cycle through ptseq and hull with computing defects */
    for(i = 0; i < hull->total; i++)
    {
        CvConvexityDefect defect;
        int is_defect = 0;
        double dx0, dy0;
        double depth = 0, scale;
        CPoint* hull_next;

        if(!is_index)
            hull_next = *(CPoint**)hull_reader.ptr;
        else
        {
            int t = *(int*)hull_reader.ptr;
            hull_next = CC_GET_SEQ_ELEM(CPoint, ptseq, t);
        }

        dx0 = (double)hull_next->x - (double)hull_cur->x;
        dy0 = (double)hull_next->y - (double)hull_cur->y;
        assert(dx0 != 0 || dy0 != 0); 
        scale = 1./sqrt(dx0*dx0 + dy0*dy0);

        defect.start = hull_cur;
        defect.end = hull_next;

        for(;;)
        {
            /* go through ptseq to achieve next hull point */
            CC_NEXT_SEQ_ELEM(sizeof(CPoint), ptseq_reader);

            if(ptseq_reader.ptr == (char*)hull_next)
                break;
            else
            {
                CPoint* cur = (CPoint*)ptseq_reader.ptr;
            
                /* compute distance from current point to hull edge */
                double dx = (double)cur->x - (double)hull_cur->x;
                double dy = (double)cur->y - (double)hull_cur->y;

                /* compute depth */
                double dist = fabs(-dy0*dx + dx0*dy) * scale;

                if(dist > depth)
                {
                    depth = dist;
                    defect.depth_point = cur;
                    defect.depth = (float)depth;
                    is_defect = 1;
                }
            }
        }
        if(is_defect)
        {
            CC_WRITE_SEQ_ELEM(defect, writer);
        }

        hull_cur = hull_next;
        if(rev_orientation)
        {
            CC_PREV_SEQ_ELEM(hull->elem_size, hull_reader);
        }
        else
        {
            CC_NEXT_SEQ_ELEM(hull->elem_size, hull_reader);
        }
    }

    defects = cvEndWriteSeq(&writer);

    __END__;

    return defects;
}


CC_IMPL int
cvCheckContourConvexity(const img_t* array)
{
    int flag = -1;

    CC_FUNCNAME("cvCheckContourConvexity");

    __BEGIN__;

    int i;
    int orientation = 0;
    CvSeqReader reader;
    CvContour contour_header;
    CvSeqBlock block;
    CvSeq* contour = (CvSeq*)array;

    if(CC_IS_SEQ(contour))
    {
        if(!CC_IS_SEQ_POLYGON(contour))
            CC_ERROR(CC_StsUnsupportedFormat,
                "Input sequence must be polygon (closed 2d curve)");
    }
    else
    {
        CC_CALL(contour = cvPointSeqFromMat(
            CC_SEQ_KIND_CURVE|CC_SEQ_FLAG_CLOSED, array, &contour_header, &block));
    }

    if(contour->total == 0)
        EXIT;

    cvStartReadSeq(contour, &reader, 0);
    
    flag = 1;

    if(CC_SEQ_ELTYPE(contour) == CC_32SC2)
    {
        CPoint *prev_pt = (CPoint*)reader.prev_elem;
        CPoint *cur_pt = (CPoint*)reader.ptr;
    
        int dx0 = cur_pt->x - prev_pt->x;
        int dy0 = cur_pt->y - prev_pt->y;

        for(i = 0; i < contour->total; i++)
        {
            int dxdy0, dydx0;
            int dx, dy;

            /*int orient; */
            CC_NEXT_SEQ_ELEM(sizeof(CPoint), reader);
            prev_pt = cur_pt;
            cur_pt = (CPoint *) reader.ptr;

            dx = cur_pt->x - prev_pt->x;
            dy = cur_pt->y - prev_pt->y;
            dxdy0 = dx * dy0;
            dydx0 = dy * dx0;

            /* find orientation */
            /*orient = -dy0 * dx + dx0 * dy;
               orientation |= (orient > 0) ? 1 : 2;
             */
            orientation |= (dydx0 > dxdy0) ? 1 : ((dydx0 < dxdy0) ? 2 : 3);

            if(orientation == 3)
            {
                flag = 0;
                break;
            }

            dx0 = dx;
            dy0 = dy;
        }
    }
    else
    {
        assert(CC_SEQ_ELTYPE(contour) == CC_32FC2);

        CPoint2D32f *prev_pt = (CPoint2D32f*)reader.prev_elem;
        CPoint2D32f *cur_pt = (CPoint2D32f*)reader.ptr;
    
        float dx0 = cur_pt->x - prev_pt->x;
        float dy0 = cur_pt->y - prev_pt->y;

        for(i = 0; i < contour->total; i++)
        {
            float dxdy0, dydx0;
            float dx, dy;

            /*int orient; */
            CC_NEXT_SEQ_ELEM(sizeof(CPoint2D32f), reader);
            prev_pt = cur_pt;
            cur_pt = (CPoint2D32f*) reader.ptr;

            dx = cur_pt->x - prev_pt->x;
            dy = cur_pt->y - prev_pt->y;
            dxdy0 = dx * dy0;
            dydx0 = dy * dx0;

            /* find orientation */
            /*orient = -dy0 * dx + dx0 * dy;
               orientation |= (orient > 0) ? 1 : 2;
             */
            orientation |= (dydx0 > dxdy0) ? 1 : ((dydx0 < dxdy0) ? 2 : 3);

            if(orientation == 3)
            {
                flag = 0;
                break;
            }

            dx0 = dx;
            dy0 = dy;
        }
    }

    __END__;

    return flag;
}



