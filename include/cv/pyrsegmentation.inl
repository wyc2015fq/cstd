#include "_cv.h"

typedef struct _CvRGBf
{   float blue;
    float green;
    float red;
}
_CvRGBf;

typedef struct _CvRect16u
{
    ushort x1, y1, x2, y2;
}
_CvRect16u;

typedef struct _CvPyramid
{
    float c;
    struct _CvPyramid *p;
    int a;
    _CvRect16u rect;      /*  ROI for the connected component    */
} _CvPyramid;

/* element of base layer */
typedef struct _CvPyramidBase
{
    float c;
    struct _CvPyramid *p;
}
_CvPyramidBase;

typedef struct _CvPyramidC3
{
    _CvRGBf c;
    struct _CvPyramidC3 *p;
    int a;
    _CvRect16u rect;      /*  ROI for the connected component    */
} _CvPyramidC3;

/* element of base layer */
typedef struct _CvPyramidBaseC3
{
    _CvRGBf c;
    struct _CvPyramidC3 *p;
}
_CvPyramidBaseC3;

typedef struct _CvListNode
{
    struct _CvListNode* next;
    void* data;
}
_CvListNode;


static CStatus  icvSegmentClusterC1(CvSeq* cmp_seq, CvSeq* res_seq,
                                 double threshold,
                                 _CvPyramid* first_level_end,
                                 CSize first_level_size);

static CStatus  icvSegmentClusterC3(CvSeq* cmp_seq, CvSeq* res_seq,
                                 double threshold,
                                 _CvPyramidC3* first_level_end,
                                 CSize first_level_size);

static CStatus icvUpdatePyrLinks_8u_C1
    (int layer, void *layer_data, CSize size, void *parent_layer,
     void *_writer, float threshold, int is_last_iter, void *_stub, CvWriteNodeFunction /*func*/);

static CStatus icvUpdatePyrLinks_8u_C3
    (int layer, void *layer_data, CSize size, void *parent_layer,
     void *_writer, float threshold, int is_last_iter, void *_stub, CvWriteNodeFunction /*func*/);

static void icvMaxRoi(_CvRect16u *max_rect, _CvRect16u* cur_rect);
static void icvMaxRoi1(_CvRect16u *max_rect, int x, int y);


#define _CC_CHECK(icvFun)                                             \
  {                                                                     \
    if(icvFun != CC_OK)                                               \
     goto M_END;                                                        \
  }


#define _CC_MAX3(a, b, c) ((a)>(b) ? ((a)>(c) ? (a) : (c)) : ((b)>(c) ? (b) : (c)))

/*#define _CC_RGB_DIST(a, b)  _CC_MAX3((float)fabs((a).red - (b).red),      \
                                       (float)fabs((a).green - (b).green),  \
                                       (float)fabs((a).blue - (b).blue))*/

#define _CC_NEXT_BASE_C1(p,n) (_CvPyramid*)((char*)(p) + (n)*sizeof(_CvPyramidBase))
#define _CC_NEXT_BASE_C3(p,n) (_CvPyramidC3*)((char*)(p) + (n)*sizeof(_CvPyramidBaseC3))


CC_INLINE float icvRGBDist_Max(const _CvRGBf& a, const _CvRGBf& b)
{
    float tr = (float)fabs(a.red - b.red);
    float tg = (float)fabs(a.green - b.green);
    float tb = (float)fabs(a.blue - b.blue);

    return _CC_MAX3(tr, tg, tb);
}

CC_INLINE float icvRGBDist_Sum(const _CvRGBf& a, const _CvRGBf& b)
{
    float tr = (float)fabs(a.red - b.red);
    float tg = (float)fabs(a.green - b.green);
    float tb = (float)fabs(a.blue - b.blue);
    
    return (tr + tg + tb);
}

#if 1
#define _CC_RGB_DIST  icvRGBDist_Max
#define _CC_RGB_THRESH_SCALE   1
#else
#define _CC_RGB_DIST  icvRGBDist_Sum
#define _CC_RGB_THRESH_SCALE   3
#endif

#define _CC_INV_TAB_SIZE   32

static const float icvInvTab[ /*_CC_INV_TAB_SIZE*/ ] =
{
    1.00000000f, 0.50000000f, 0.33333333f, 0.25000000f, 0.20000000f, 0.16666667f,
    0.14285714f, 0.12500000f, 0.11111111f, 0.10000000f, 0.09090909f, 0.08333333f,
    0.07692308f, 0.07142857f, 0.06666667f, 0.06250000f, 0.05882353f, 0.05555556f,
    0.05263158f, 0.05000000f, 0.04761905f, 0.04545455f, 0.04347826f, 0.04166667f,
    0.04000000f, 0.03846154f, 0.03703704f, 0.03571429f, 0.03448276f, 0.03333333f,
    0.03225806f, 0.03125000f
};

static void
icvWritePyrNode(void *elem, void *writer)
{
    CC_WRITE_SEQ_ELEM(*(_CvListNode *) elem, *(CvSeqWriter *) writer);
}


static CStatus
icvPyrSegmentation8uC1R(uchar * src_image, int src_step,
                         uchar * dst_image, int dst_step,
                         CSize roi, CvFilter filter,
                         CvSeq ** dst_comp, CvMemStorage * storage,
                         int level, int threshold1, int threshold2)
{
    int i, j, l;
    int step;
    const int max_iter = 3;     /* maximum number of iterations */
    int cur_iter = 0;           /* current iteration */

    _CvPyramid *pyram[16];      /* pointers to the pyramid down up to level */

    float *pyramida = 0;
    _CvPyramid stub;

    _CvPyramid *p_cur;
    _CvPyramidBase *p_base;
    _CvListNode cmp_node;

    CvSeq *cmp_seq = 0;
    CvSeq *res_seq = 0;
    CvMemStorage *temp_storage = 0;
    CSize size;
    CStatus status;
    CvSeqWriter writer;

    int buffer_size;
    char *buffer = 0;

    status = CC_OK;

    /* clear pointer to resultant sequence */
    if(dst_comp)
        *dst_comp = 0;

    /* check args */
    if(!src_image || !dst_image || !storage || !dst_comp)
        return CC_NULLPTR_ERR;
    if(roi.width <= 0 || roi.height <= 0 || src_step < roi.width || dst_step < roi.width)
        return CC_BADSIZE_ERR;
    if(filter != CC_GAUSSIAN_5x5)
        return CC_BADRANGE_ERR;
    if(threshold1 < 0 || threshold2 < 0)
        return CC_BADRANGE_ERR;
    if(level <= 0)
        return CC_BADRANGE_ERR;

    if(((roi.width | roi.height) & ((1 << level) - 1)) != 0)
        return CC_BADCOEF_ERR;

    temp_storage = cvCreateChildMemStorage(storage);

    /* sequence for temporary components */
    cmp_seq = cvCreateSeq(0, sizeof(CvSeq), sizeof(_CvListNode), temp_storage);
    assert(cmp_seq != 0);

    res_seq = cvCreateSeq(CC_SEQ_CONNECTED_COMP, sizeof(CvSeq),
                           sizeof(CConnectedComp), storage);
    assert(res_seq != 0);

    /* calculate buffer size */
    buffer_size = roi.width * roi.height * (sizeof(float) + sizeof(_CvPyramidBase));

    for(l = 1; l <= level; l++)
        buffer_size += ((roi.width >> l) + 1) * ((roi.height >> l) + 1) * sizeof(_CvPyramid);

    /* allocate buffer */
    buffer = (char *) cAlloc(buffer_size);
    if(!buffer)
    {
        status = CC_OUTOFMEM_ERR;
        goto M_END;
    }

    pyramida = (float *) buffer;

    /* initialization pyramid-linking properties down up to level */
    step = roi.width * sizeof(float);

    {
        img_t _src;
        img_t _pyramida;
        cvInitMatHeader(&_src, roi.height, roi.width, CC_8UC1, src_image, src_step);
        cvInitMatHeader(&_pyramida, roi.height, roi.width, CC_32FC1, pyramida, step);
        cvConvert(&_src, &_pyramida);
        /*_CC_CHECK(icvCvtTo_32f_C1R(src_image, src_step, pyramida, step, roi, CC_8UC1));*/
    }
    p_base = (_CvPyramidBase *) (buffer + step * roi.height);
    pyram[0] = (_CvPyramid *) p_base;

    /* fill base level of pyramid */
    for(i = 0; i < roi.height; i++)
    {
        for(j = 0; j < roi.width; j++, p_base++)
        {
            p_base->c = pyramida[i * roi.width + j];
            p_base->p = &stub;
        }
    }

    p_cur = (_CvPyramid *) p_base;
    size = roi;

    /* calculate initial pyramid */
    for(l = 1; l <= level; l++)
    {
        CSize dst_size = { size.width/2+1, size.height/2+1 };
        img_t prev_level = cvMat(size.height, size.width, CC_32FC1);
        img_t next_level = cvMat(dst_size.height, dst_size.width, CC_32FC1);

        cvSetData(&prev_level, pyramida, step);
        cvSetData(&next_level, pyramida, step);
        cvPyrDown(&prev_level, &next_level);
        
        //_CC_CHECK(icvPyrDown_Gauss5x5_32f_C1R(pyramida, step, pyramida, step, size, buff));
        //_CC_CHECK(icvPyrDownBorder_32f_CnR(pyramida, step, size, pyramida, step, dst_size, 1));
        pyram[l] = p_cur;

        size.width = dst_size.width - 1;
        size.height = dst_size.height - 1;

        /* fill layer #l */
        for(i = 0; i <= size.height; i++)
        {
            for(j = 0; j <= size.width; j++, p_cur++)
            {
                p_cur->c = pyramida[i * roi.width + j];
                p_cur->p = &stub;
                p_cur->a = 0;
                p_cur->rect.x2 = 0;
            }
        }
    }

    cvStartAppendToSeq(cmp_seq, &writer);

    /* do several iterations to determine son-father links */
    for(cur_iter = 0; cur_iter < max_iter; cur_iter++)
    {
        int is_last_iter = cur_iter == max_iter - 1;

        size = roi;

        /* build son-father links down up to level */
        for(l = 0; l < level; l++)
        {
            icvUpdatePyrLinks_8u_C1(l, pyram[l], size, pyram[l + 1], &writer,
                                      (float) threshold1, is_last_iter, &stub,
                                      icvWritePyrNode);

            /* clear last border row */
            if(l > 0)
            {
                p_cur = pyram[l] + (size.width + 1) * size.height;
                for(j = 0; j <= size.width; j++)
                    p_cur[j].c = 0;
            }

            size.width >>= 1;
            size.height >>= 1;
        }

/*  clear the old c value for the last level     */
        p_cur = pyram[level];
        for(i = 0; i <= size.height; i++, p_cur += size.width + 1)
            for(j = 0; j <= size.width; j++)
                p_cur[j].c = 0;

        size = roi;
        step = roi.width;

/* calculate average c value for the 0 < l <=level   */
        for(l = 0; l < level; l++, step = (step >> 1) + 1)
        {
            _CvPyramid *p_prev, *p_row_prev;

            stub.c = 0;

            /* calculate average c value for the next level   */
            if(l == 0)
            {
                p_base = (_CvPyramidBase *) pyram[0];
                for(i = 0; i < roi.height; i++, p_base += size.width)
                {
                    for(j = 0; j < size.width; j += 2)
                    {
                        _CvPyramid *p1 = p_base[j].p;
                        _CvPyramid *p2 = p_base[j + 1].p;

                        p1->c += p_base[j].c;
                        p2->c += p_base[j + 1].c;
                    }
                }
            }
            else
            {
                p_cur = pyram[l];
                for(i = 0; i < size.height; i++, p_cur += size.width + 1)
                {
                    for(j = 0; j < size.width; j += 2)
                    {
                        _CvPyramid *p1 = p_cur[j].p;
                        _CvPyramid *p2 = p_cur[j + 1].p;

                        float t0 = (float) p_cur[j].a * p_cur[j].c;
                        float t1 = (float) p_cur[j + 1].a * p_cur[j + 1].c;

                        p1->c += t0;
                        p2->c += t1;

                        if(!is_last_iter)
                            p_cur[j].a = p_cur[j + 1].a = 0;
                    }
                    if(!is_last_iter)
                        p_cur[size.width].a = 0;
                }
                if(!is_last_iter)
                {
                    for(j = 0; j <= size.width; j++)
                    {
                        p_cur[j].a = 0;
                    }
                }
            }

            /* assign random values of the next level null c   */
            p_cur = pyram[l + 1];
            p_row_prev = p_prev = pyram[l];

            size.width >>= 1;
            size.height >>= 1;

            for(i = 0; i <= size.height; i++, p_cur += size.width + 1)
            {
                if(i < size.height || !is_last_iter)
                {
                    for(j = 0; j < size.width; j++)
                    {
                        int a = p_cur[j].a;

                        if(a != 0)
                        {
                            if(a <= _CC_INV_TAB_SIZE)
                            {
                                p_cur[j].c *= icvInvTab[a - 1];
                            }
                            else
                            {
                                p_cur[j].c /= a;
                            }
                        }
                        else
                        {
                            p_cur[j].c = p_prev->c;
                        }
                        
                        if(l == 0)
                            p_prev = _CC_NEXT_BASE_C1(p_prev,2);
                        else
                            p_prev += 2;
                    }

                    if(p_cur[size.width].a == 0)
                    {
                        p_cur[size.width].c = p_prev[(l != 0) - 1].c;
                    }
                    else
                    {
                        p_cur[size.width].c /= p_cur[size.width].a;
                        if(is_last_iter)
                        {
                            cmp_node.data = p_cur + size.width;
                            CC_WRITE_SEQ_ELEM(cmp_node, writer);
                        }
                    }
                }
                else
                {
                    for(j = 0; j <= size.width; j++)
                    {
                        int a = p_cur[j].a;

                        if(a != 0)
                        {
                            if(a <= _CC_INV_TAB_SIZE)
                            {
                                p_cur[j].c *= icvInvTab[a - 1];
                            }
                            else
                            {
                                p_cur[j].c /= a;
                            }

                            cmp_node.data = p_cur + j;
                            CC_WRITE_SEQ_ELEM(cmp_node, writer);
                        }
                        else
                        {
                            p_cur[j].c = p_prev->c;
                        }

                        if(l == 0)
                        {
                            p_prev = _CC_NEXT_BASE_C1(p_prev, (j * 2 < step - 2 ? 2 : 1));
                        }
                        else
                        {
                            p_prev++;
                        }
                    }
                }

                if(l + 1 == level && !is_last_iter)
                    for(j = 0; j <= size.width; j++)
                        p_cur[j].a = 0;

                if(!(i & 1))
                {
                    p_prev = p_row_prev;
                }
                else
                {
                    p_prev = (_CvPyramid*)((char*)p_row_prev + step *
                        (l == 0 ? sizeof(_CvPyramidBase) : sizeof(_CvPyramid)));
                }
            }
        }
    }                           /*  end of the iteration process  */

    /* construct a connected  components   */
    size.width = roi.width >> level;
    size.height = roi.height >> level;

    p_cur = pyram[level];

    for(i = 0; i < size.height; i++, p_cur += size.width + 1)
    {
        for(j = 0; j < size.width; j++)
        {
            if(p_cur[j].a != 0)
            {
                cmp_node.data = p_cur + j;
                CC_WRITE_SEQ_ELEM(cmp_node, writer);
            }
        }
    }

    cvEndWriteSeq(&writer);

/* clusterization segmented components and construction 
   output connected components                            */
    icvSegmentClusterC1(cmp_seq, res_seq, threshold2, pyram[1], roi);

/* convert (inplace) resultant segment values to int (top level) */

/* propagate segment values top down */
    for(l = level - 1; l >= 0; l--)
    {
        p_cur = pyram[l];

        size.width <<= 1;
        size.height <<= 1;

        if(l == 0)
        {
            size.width--;
            size.height--;
        }

        for(i = 0; i <= size.height; i++)
        {
            for(j = 0; j <= size.width; j++)
            {
                _CvPyramid *p = p_cur->p;

                assert(p != 0);
                if(p != &stub)
                    p_cur->c = p->c;

                if(l == 0)
                {
                    suf32_t _c;
                    /* copy the segmented values to destination image */
                    _c.f = p_cur->c; dst_image[j] = (uchar)_c.i;
                    p_cur = _CC_NEXT_BASE_C1(p_cur, 1);
                }
                else
                {
                    p_cur++;
                }
            }
            if(l == 0)
                dst_image += dst_step;
        }
    }
  M_END:

    cFree(&buffer);
    cvReleaseMemStorage(&temp_storage);

    if(status == CC_OK)
        *dst_comp = res_seq;

    return status;
}



/****************************************************************************************\
    color!!!  image segmentation by pyramid-linking   
\****************************************************************************************/
static CStatus
icvPyrSegmentation8uC3R(uchar * src_image, int src_step,
                         uchar * dst_image, int dst_step,
                         CSize roi, CvFilter filter,
                         CvSeq ** dst_comp, CvMemStorage * storage,
                         int level, int threshold1, int threshold2)
{
    int i, j, l;

    int step;
    const int max_iter = 3;     /* maximum number of iterations */
    int cur_iter = 0;           /* current iteration */

    _CvPyramidC3 *pyram[16];    /* pointers to the pyramid down up to level */

    float *pyramida = 0;
    _CvPyramidC3 stub;

    _CvPyramidC3 *p_cur;
    _CvPyramidBaseC3 *p_base;
    _CvListNode cmp_node;

    CvSeq *cmp_seq = 0;
    CvSeq *res_seq = 0;
    CvMemStorage *temp_storage = 0;
    CSize size;
    CStatus status;
    CvSeqWriter writer;

    int buffer_size;
    char *buffer = 0;

    status = CC_OK;

    threshold1 *= _CC_RGB_THRESH_SCALE;
    threshold2 *= _CC_RGB_THRESH_SCALE;

    /* clear pointer to resultant sequence */
    if(dst_comp)
        *dst_comp = 0;

    /* check args */
    if(!src_image || !dst_image || !storage || !dst_comp)
        return CC_NULLPTR_ERR;
    if(roi.width <= 0 || roi.height <= 0 ||
        src_step < roi.width * 3 || dst_step < roi.width * 3) return CC_BADSIZE_ERR;
    if(filter != CC_GAUSSIAN_5x5)
        return CC_BADRANGE_ERR;
    if(threshold1 < 0 || threshold2 < 0)
        return CC_BADRANGE_ERR;
    if(level <= 0)
        return CC_BADRANGE_ERR;

    if(((roi.width | roi.height) & ((1 << level) - 1)) != 0)
        return CC_BADCOEF_ERR;

    temp_storage = cvCreateChildMemStorage(storage);

    /* sequence for temporary components */
    cmp_seq = cvCreateSeq(0, sizeof(CvSeq), sizeof(_CvListNode), temp_storage);
    assert(cmp_seq != 0);

    res_seq = cvCreateSeq(CC_SEQ_CONNECTED_COMP, sizeof(CvSeq),
                           sizeof(CConnectedComp), storage);
    assert(res_seq != 0);

    /* calculate buffer size */
    buffer_size = roi.width * roi.height * (sizeof(_CvRGBf) + sizeof(_CvPyramidBaseC3));

    for(l = 1; l <= level; l++)
        buffer_size += ((roi.width >> l) + 1) * ((roi.height >> l) + 1) * sizeof(_CvPyramidC3);

    /* allocate buffer */
    buffer = (char *) cAlloc(buffer_size);
    if(!buffer)
    {
        status = CC_OUTOFMEM_ERR;
        goto M_END;
    }

    pyramida = (float *) buffer;

    /* initialization pyramid-linking properties down up to level */
    step = roi.width * sizeof(_CvRGBf);

    {
        img_t _src;
        img_t _pyramida;
        cvInitMatHeader(&_src, roi.height, roi.width, CC_8UC3, src_image, src_step);
        cvInitMatHeader(&_pyramida, roi.height, roi.width, CC_32FC3, pyramida, step);
        cvConvert(&_src, &_pyramida);
        /*_CC_CHECK(icvCvtTo_32f_C1R(src_image, src_step, pyramida, step,
                                 cSize(roi.width * 3, roi.height), CC_8UC1));*/
    }

    p_base = (_CvPyramidBaseC3 *) (buffer + step * roi.height);
    pyram[0] = (_CvPyramidC3 *) p_base;

    /* fill base level of pyramid */
    for(i = 0; i < roi.height; i++)
    {
        for(j = 0; j < roi.width; j++, p_base++)
        {
            p_base->c = ((_CvRGBf *) pyramida)[i * roi.width + j];
            p_base->p = &stub;
        }
    }

    p_cur = (_CvPyramidC3 *) p_base;
    size = roi;

    /* calculate initial pyramid */
    for(l = 1; l <= level; l++)
    {
        CSize dst_size = { size.width/2 + 1, size.height/2 + 1 };
        img_t prev_level = cvMat(size.height, size.width, CC_32FC3);
        img_t next_level = cvMat(dst_size.height, dst_size.width, CC_32FC3);

        cvSetData(&prev_level, pyramida, step);
        cvSetData(&next_level, pyramida, step);
        cvPyrDown(&prev_level, &next_level);

        //_CC_CHECK(icvPyrDown_Gauss5x5_32f_C3R(pyramida, step, pyramida, step, size, buff));
        //_CC_CHECK(icvPyrDownBorder_32f_CnR(pyramida, step, size, pyramida, step, dst_size, 3));
        pyram[l] = p_cur;

        size.width = dst_size.width - 1;
        size.height = dst_size.height - 1;

        /* fill layer #l */
        for(i = 0; i <= size.height; i++)
        {
            assert((char*)p_cur - buffer < buffer_size);
            for(j = 0; j <= size.width; j++, p_cur++)
            {
                p_cur->c = ((_CvRGBf *) pyramida)[i * roi.width + j];
                p_cur->p = &stub;
                p_cur->a = 0;
                p_cur->rect.x2 = 0;
            }
        }
    }

    cvStartAppendToSeq(cmp_seq, &writer);

    /* do several iterations to determine son-father links */
    for(cur_iter = 0; cur_iter < max_iter; cur_iter++)
    {
        int is_last_iter = cur_iter == max_iter - 1;

        size = roi;

        /* build son-father links down up to level */
        for(l = 0; l < level; l++)
        {
            icvUpdatePyrLinks_8u_C3(l, pyram[l], size, pyram[l + 1], &writer,
                                      (float) threshold1, is_last_iter, &stub,
                                      icvWritePyrNode);

            /* clear last border row */
            if(l > 0)
            {
                p_cur = pyram[l] + (size.width + 1) * size.height;
                for(j = 0; j <= size.width; j++)
                    p_cur[j].c.blue = p_cur[j].c.green = p_cur[j].c.red = 0;
            }

            size.width >>= 1;
            size.height >>= 1;
        }

/*  clear the old c value for the last level     */
        p_cur = pyram[level];
        for(i = 0; i <= size.height; i++, p_cur += size.width + 1)
            for(j = 0; j <= size.width; j++)
                p_cur[j].c.blue = p_cur[j].c.green = p_cur[j].c.red = 0;

        size = roi;
        step = roi.width;

/* calculate average c value for the 0 < l <=level   */
        for(l = 0; l < level; l++, step = (step >> 1) + 1)
        {
            _CvPyramidC3 *p_prev, *p_row_prev;

            stub.c.blue = stub.c.green = stub.c.red = 0;

            /* calculate average c value for the next level   */
            if(l == 0)
            {
                p_base = (_CvPyramidBaseC3 *) pyram[0];
                for(i = 0; i < roi.height; i++, p_base += size.width)
                {
                    for(j = 0; j < size.width; j++)
                    {
                        _CvPyramidC3 *p = p_base[j].p;

                        p->c.blue += p_base[j].c.blue;
                        p->c.green += p_base[j].c.green;
                        p->c.red += p_base[j].c.red;
                    }
                }
            }
            else
            {
                p_cur = pyram[l];
                for(i = 0; i < size.height; i++, p_cur += size.width + 1)
                {
                    for(j = 0; j < size.width; j++)
                    {
                        _CvPyramidC3 *p = p_cur[j].p;
                        float a = (float) p_cur[j].a;

                        p->c.blue += a * p_cur[j].c.blue;
                        p->c.green += a * p_cur[j].c.green;
                        p->c.red += a * p_cur[j].c.red;

                        if(!is_last_iter)
                            p_cur[j].a = 0;
                    }
                    if(!is_last_iter)
                        p_cur[size.width].a = 0;
                }
                if(!is_last_iter)
                {
                    for(j = 0; j <= size.width; j++)
                    {
                        p_cur[j].a = 0;
                    }
                }
            }

            /* assign random values of the next level null c   */
            p_cur = pyram[l + 1];
            p_row_prev = p_prev = pyram[l];

            size.width >>= 1;
            size.height >>= 1;

            for(i = 0; i <= size.height; i++, p_cur += size.width + 1)
            {
                if(i < size.height || !is_last_iter)
                {
                    for(j = 0; j < size.width; j++)
                    {
                        int a = p_cur[j].a;

                        if(a != 0)
                        {
                            float inv_a;

                            if(a <= _CC_INV_TAB_SIZE)
                            {
                                inv_a = icvInvTab[a - 1];
                            }
                            else
                            {
                                inv_a = 1.f / a;
                            }
                            p_cur[j].c.blue *= inv_a;
                            p_cur[j].c.green *= inv_a;
                            p_cur[j].c.red *= inv_a;
                        }
                        else
                        {
                            p_cur[j].c = p_prev->c;
                        }
                        
                        if(l == 0)
                            p_prev = _CC_NEXT_BASE_C3(p_prev, 2);
                        else
                            p_prev += 2;
                    }

                    if(p_cur[size.width].a == 0)
                    {
                        p_cur[size.width].c = p_prev[(l != 0) - 1].c;
                    }
                    else
                    {
                        p_cur[size.width].c.blue /= p_cur[size.width].a;
                        p_cur[size.width].c.green /= p_cur[size.width].a;
                        p_cur[size.width].c.red /= p_cur[size.width].a;
                        if(is_last_iter)
                        {
                            cmp_node.data = p_cur + size.width;
                            CC_WRITE_SEQ_ELEM(cmp_node, writer);
                        }
                    }
                }
                else
                {
                    for(j = 0; j <= size.width; j++)
                    {
                        int a = p_cur[j].a;

                        if(a != 0)
                        {
                            float inv_a;

                            if(a <= _CC_INV_TAB_SIZE)
                            {
                                inv_a = icvInvTab[a - 1];
                            }
                            else
                            {
                                inv_a = 1.f / a;
                            }
                            p_cur[j].c.blue *= inv_a;
                            p_cur[j].c.green *= inv_a;
                            p_cur[j].c.red *= inv_a;

                            cmp_node.data = p_cur + j;
                            CC_WRITE_SEQ_ELEM(cmp_node, writer);
                        }
                        else
                        {
                            p_cur[j].c = p_prev->c;
                        }

                        if(l == 0)
                        {
                            p_prev = _CC_NEXT_BASE_C3(p_prev, (j * 2 < step - 2 ? 2 : 1));
                        }
                        else
                        {
                            p_prev++;
                        }
                    }
                }

                if(l + 1 == level && !is_last_iter)
                    for(j = 0; j <= size.width; j++)
                        p_cur[j].a = 0;

                if(!(i & 1))
                {
                    p_prev = p_row_prev;
                }
                else
                {
                    p_prev = (_CvPyramidC3*)((char*)p_row_prev + step *
                        (l == 0 ? sizeof(_CvPyramidBaseC3) : sizeof(_CvPyramidC3)));
                }
            }
        }
    }                           /*  end of the iteration process  */

    /* construct a connected  components   */
    size.width = roi.width >> level;
    size.height = roi.height >> level;

    p_cur = pyram[level];

    for(i = 0; i < size.height; i++, p_cur += size.width + 1)
    {
        for(j = 0; j < size.width; j++)
        {
            if(p_cur[j].a != 0)
            {
                cmp_node.data = p_cur + j;
                CC_WRITE_SEQ_ELEM(cmp_node, writer);
            }
        }
    }

    cvEndWriteSeq(&writer);

/* clusterization segmented components and construction 
   output connected components                            */
    icvSegmentClusterC3(cmp_seq, res_seq, threshold2, pyram[1], roi);

/* convert (inplace) resultant segment values to int (top level) */

/* propagate segment values top down */
    for(l = level - 1; l >= 0; l--)
    {
        p_cur = pyram[l];

        size.width <<= 1;
        size.height <<= 1;

        if(l == 0)
        {
            size.width--;
            size.height--;
        }

        for(i = 0; i <= size.height; i++)
        {
            for(j = 0; j <= size.width; j++)
            {
                _CvPyramidC3 *p = p_cur->p;

                assert(p != 0);
                if(p != &stub)
                {
                    p_cur->c = p->c;
                }

                if(l == 0)
                {
                    suf32_t _c;
                    /* copy the segmented values to destination image */
                    _c.f = p_cur->c.blue; dst_image[j*3] = (uchar)_c.i;
                    _c.f = p_cur->c.green; dst_image[j*3+1] = (uchar)_c.i;
                    _c.f = p_cur->c.red; dst_image[j*3+2] = (uchar)_c.i;
                    p_cur = _CC_NEXT_BASE_C3(p_cur,1);
                }
                else
                {
                    p_cur++;
                }
            }
            if(l == 0)
                dst_image += dst_step;
        }
    }

  M_END:

    cFree(&buffer);
    cvReleaseMemStorage(&temp_storage);

    if(status == CC_OK)
        *dst_comp = res_seq;

    return status;
}


static CStatus icvUpdatePyrLinks_8u_C1
    (int layer, void *layer_data, CSize size, void *parent_layer,
     void *_writer, float threshold, int is_last_iter, void *_stub, CvWriteNodeFunction /*func*/)
{
    int i, j;
    _CvListNode cmp_node;

    _CvPyramid *stub = (_CvPyramid *) _stub;
    _CvPyramid *p_cur = (_CvPyramid *) layer_data;
    _CvPyramid *p_next1 = (_CvPyramid *) parent_layer;
    _CvPyramid *p_next3 = p_next1 + (size.width >> 1) + 1;

    CvSeqWriter & writer = *(CvSeqWriter *) _writer;

    for(i = 0; i < size.height; i++)
    {
        for(j = 0; j < size.width; j += 2)
        {
            float c0, c1, c2, c3, c4;
            _CvPyramid *p;

/* son-father threshold linking for the current node establish */
            c0 = p_cur->c;

/* find pointer for the first pixel */
            c1 = (float) fabs(c0 - p_next1[0].c);
            c2 = (float) fabs(c0 - p_next1[1].c);
            c3 = (float) fabs(c0 - p_next3[0].c);
            c4 = (float) fabs(c0 - p_next3[1].c);

            p = p_next1;

            if(c1 > c2)
            {
                p = p_next1 + 1;
                c1 = c2;
            }
            if(c1 > c3)
            {
                p = p_next3;
                c1 = c3;
            }
            if(c1 > c4)
            {
                p = p_next3 + 1;
                c1 = c4;
            }

            if(c1 <= threshold)
            {
                p_cur->p = p;

                if(layer == 0)
                {
                    p->a++;
                    p_cur = (_CvPyramid*)((char*)p_cur + sizeof(_CvPyramidBase));
                    if(is_last_iter)
                        icvMaxRoi1(&(p->rect), j, i);
                }
                else
                {
                    int a = p_cur->a;

                    p->a += a;
                    p_cur->c = 0;
                    p_cur++;
                    if(is_last_iter && a != 0)
                        icvMaxRoi(&(p->rect), &(p_cur[-1].rect));
                }
            }
            else
            {
                p_cur->p = stub;
                if(is_last_iter)
                {
                    cmp_node.data = p_cur;
                    CC_WRITE_SEQ_ELEM(cmp_node, writer);
                }
                if(layer == 0)
                {
                    p_cur = _CC_NEXT_BASE_C1(p_cur,1);
                }
                else
                {
                    p_cur->c = 0;
                    p_cur++;
                }
            }

            /* find pointer for the second pixel */
            c0 = p_cur->c;

            c1 = (float) fabs(c0 - p_next1[0].c);
            c2 = (float) fabs(c0 - p_next1[1].c);
            c3 = (float) fabs(c0 - p_next3[0].c);
            c4 = (float) fabs(c0 - p_next3[1].c);

            p = p_next1;
            p_next1++;

            if(c1 > c2)
            {
                p = p_next1;
                c1 = c2;
            }
            if(c1 > c3)
            {
                p = p_next3;
                c1 = c3;
            }

            p_next3++;
            if(c1 > c4)
            {
                p = p_next3;
                c1 = c4;
            }

            if(c1 <= threshold)
            {
                p_cur->p = p;

                if(layer == 0)
                {
                    p->a++;
                    p_cur = _CC_NEXT_BASE_C1(p_cur,1);
                    if(is_last_iter)
                        icvMaxRoi1(&(p->rect), j + 1, i);
                }
                else
                {
                    int a = p_cur->a;

                    p->a += a;
                    p_cur->c = 0;
                    p_cur++;
                    if(is_last_iter && a != 0)
                        icvMaxRoi(&(p->rect), &(p_cur[-1].rect));
                }
            }
            else
            {
                p_cur->p = stub;
                if(is_last_iter)
                {
                    cmp_node.data = p_cur;
                    CC_WRITE_SEQ_ELEM(cmp_node, writer);
                }
                if(layer == 0)
                {
                    p_cur = _CC_NEXT_BASE_C1(p_cur,1);
                }
                else
                {
                    p_cur->c = 0;
                    p_cur++;
                }
            }
        }

        /* clear c's */
        if(layer > 0)
        {
            p_cur->c = 0;
            p_cur++;
        }

        if(!(i & 1))
        {
            p_next1 -= size.width >> 1;
            p_next3 -= size.width >> 1;
        }
        else
        {
            p_next1++;
            p_next3++;
        }
    }

    return CC_OK;
}


static CStatus icvUpdatePyrLinks_8u_C3
    (int layer, void *layer_data, CSize size, void *parent_layer,
     void *_writer, float threshold, int is_last_iter, void *_stub, CvWriteNodeFunction /*func*/)
{
    int i, j;
    _CvListNode cmp_node;

    _CvPyramidC3 *stub = (_CvPyramidC3 *) _stub;
    _CvPyramidC3 *p_cur = (_CvPyramidC3 *) layer_data;
    _CvPyramidC3 *p_next1 = (_CvPyramidC3 *) parent_layer;
    _CvPyramidC3 *p_next3 = p_next1 + (size.width >> 1) + 1;

    CvSeqWriter & writer = *(CvSeqWriter *) _writer;

    for(i = 0; i < size.height; i++)
    {
        for(j = 0; j < size.width; j += 2)
        {
            float c1, c2, c3, c4;
            _CvPyramidC3 *p;

/* find pointer for the first pixel */
            c1 = _CC_RGB_DIST(p_cur->c, p_next1[0].c);
            c2 = _CC_RGB_DIST(p_cur->c, p_next1[1].c);
            c3 = _CC_RGB_DIST(p_cur->c, p_next3[0].c);
            c4 = _CC_RGB_DIST(p_cur->c, p_next3[1].c);

            p = p_next1;

            if(c1 > c2)
            {
                p = p_next1 + 1;
                c1 = c2;
            }
            if(c1 > c3)
            {
                p = p_next3;
                c1 = c3;
            }
            if(c1 > c4)
            {
                p = p_next3 + 1;
                c1 = c4;
            }

            if(c1 < threshold)
            {
                p_cur->p = p;

                if(layer == 0)
                {
                    p->a++;
                    p_cur = _CC_NEXT_BASE_C3(p_cur,1);
                    if(is_last_iter)
                        icvMaxRoi1(&(p->rect), j, i);
                }
                else
                {
                    int a = p_cur->a;

                    p->a += a;
                    p_cur->c.blue = p_cur->c.green = p_cur->c.red = 0;
                    p_cur++;
                    if(is_last_iter && a != 0)
                        icvMaxRoi(&(p->rect), &(p_cur[-1].rect));
                }
            }
            else
            {
                p_cur->p = stub;
                if(is_last_iter /* && (== 0 || p_cur->a != 0) */)
                {
                    cmp_node.data = p_cur;
                    CC_WRITE_SEQ_ELEM(cmp_node, writer);
                }

                if(layer == 0)
                {
                    p_cur = _CC_NEXT_BASE_C3(p_cur,1);
                }
                else
                {
                    p_cur->c.blue = p_cur->c.green = p_cur->c.red = 0;
                    p_cur++;
                }
            }

            /* find pointer for the second pixel */
            c1 = _CC_RGB_DIST(p_cur->c, p_next1[0].c);
            c2 = _CC_RGB_DIST(p_cur->c, p_next1[1].c);
            c3 = _CC_RGB_DIST(p_cur->c, p_next3[0].c);
            c4 = _CC_RGB_DIST(p_cur->c, p_next3[1].c);

            p = p_next1;
            p_next1++;

            if(c1 > c2)
            {
                p = p_next1;
                c1 = c2;
            }
            if(c1 > c3)
            {
                p = p_next3;
                c1 = c3;
            }

            p_next3++;
            if(c1 > c4)
            {
                p = p_next3;
                c1 = c4;
            }

            if(c1 < threshold)
            {
                p_cur->p = p;

                if(layer == 0)
                {
                    p->a++;
                    p_cur = _CC_NEXT_BASE_C3(p_cur,1);
                    if(is_last_iter)
                        icvMaxRoi1(&(p->rect), j + 1, i);
                }
                else
                {
                    int a = p_cur->a;

                    p->a += a;
                    p_cur->c.blue = p_cur->c.green = p_cur->c.red = 0;
                    p_cur++;
                    if(is_last_iter && a != 0)
                        icvMaxRoi(&(p->rect), &(p_cur[-1].rect));
                }
            }
            else
            {
                p_cur->p = stub;
                if(is_last_iter /* && (== 0 || p_cur->a != 0) */)
                {
                    cmp_node.data = p_cur;
                    CC_WRITE_SEQ_ELEM(cmp_node, writer);
                }
                if(layer == 0)
                {
                    p_cur = _CC_NEXT_BASE_C3(p_cur,1);
                }
                else
                {
                    p_cur->c.blue = p_cur->c.green = p_cur->c.red = 0;
                    p_cur++;
                }
            }
        }

        /* clear c's */
        if(layer > 0)
        {
            p_cur->c.blue = p_cur->c.green = p_cur->c.red = 0;
            p_cur++;
        }

        if(!(i & 1))
        {
            p_next1 -= size.width >> 1;
            p_next3 -= size.width >> 1;
        }
        else
        {
            p_next1++;
            p_next3++;
        }
    }

    return CC_OK;
}



/****************************************************************************************\

    clusterization segmented components    

\****************************************************************************************/
static void
icvExpandBaseLevelC1(_CvPyramid * base_p, _CvPyramid * p, _CvPyramidBase * start, int width)
{
    int x = (int)((_CvPyramidBase *) base_p - start);
    int y = x / width;

    x -= y * width;
    p->a = 1;
    p->rect.x1 = (ushort) x;
    p->rect.y1 = (ushort) y;
    p->rect.x2 = (ushort) (x + 1);
    p->rect.y2 = (ushort) (y + 1);
    p->c = base_p->c;
}

CStatus
icvSegmentClusterC1(CvSeq * cmp_seq, CvSeq * res_seq,
                     double threshold, _CvPyramid * first_level_end, CSize first_level_size)
{
    const double eps = 1.;
    CvSeqWriter writer;
    CvSeqReader reader;
    _CvPyramid temp_cmp;
    _CvPyramidBase *first_level_start = (_CvPyramidBase *) first_level_end -
        first_level_size.width * first_level_size.height;
    int c, i, count = cmp_seq->total;

    cvStartReadSeq(cmp_seq, &reader, 0);
    cvStartAppendToSeq(res_seq, &writer);

    if(threshold < eps)
    {
        /* if threshold is too small then simply copy all
           the components to the output sequence */
        for(i = 0; i < count; i++)
        {
            CConnectedComp comp;
            _CvPyramid *cmp = (_CvPyramid *) (((_CvListNode *) reader.ptr)->data);
            suf32_t _c;

            if(cmp < first_level_end)
            {
                icvExpandBaseLevelC1(cmp, &temp_cmp, first_level_start,
                                      first_level_size.width);
                cmp = &temp_cmp;
            }

            _c.i = cRound(cmp->c);
            cmp->c = _c.f;
            comp.value = cRealScalar(_c.i);
            comp.area = cmp->a;
            comp.rect.x = cmp->rect.x1;
            comp.rect.y = cmp->rect.y1;
            comp.rect.width = cmp->rect.x2 - cmp->rect.x1;
            comp.rect.height = cmp->rect.y2 - cmp->rect.y1;
            comp.contour = 0;

            CC_WRITE_SEQ_ELEM(comp, writer);
            CC_NEXT_SEQ_ELEM(sizeof(_CvListNode), reader);
        }
    }
    else
    {
        _CvListNode stub_node;
        _CvListNode *prev = &stub_node;

        stub_node.next = 0;

        for(i = 0; i < count; i++)
        {
            _CvListNode *node = (_CvListNode *) reader.ptr;

            prev->next = node;
            prev = node;
            CC_NEXT_SEQ_ELEM(sizeof(_CvListNode), reader);
        }
        prev->next = 0;
        prev = stub_node.next;

        while(prev)
        {
            _CvListNode *node = prev->next;
            _CvListNode *acc = prev;
            _CvPyramid *cmp = (_CvPyramid *) (acc->data);
            CConnectedComp comp;
            float c0 = cmp->c;

            if(cmp < first_level_end)
            {
                icvExpandBaseLevelC1(cmp, &temp_cmp, first_level_start,
                                      first_level_size.width);
            }
            else
            {
                temp_cmp = *cmp;
                temp_cmp.c *= temp_cmp.a;
            }

            acc->next = 0;
            stub_node.next = 0;
            prev = &stub_node;

            while(node)
            {
                cmp = (_CvPyramid *) (node->data);
                if(fabs(c0 - cmp->c) < threshold)
                {
                    _CvPyramid temp;

                    /* exclude from global list and add to list of joint component */
                    prev->next = node->next;
                    node->next = acc;
                    acc = node;

                    if(cmp < first_level_end)
                    {
                        icvExpandBaseLevelC1(cmp, &temp, first_level_start,
                                              first_level_size.width);
                        cmp = &temp;
                    }

                    temp_cmp.a += cmp->a;
                    temp_cmp.c += cmp->c * cmp->a;
                    icvMaxRoi(&(temp_cmp.rect), &(cmp->rect));
                }
                else
                {
                    if(prev == &stub_node)
                    {
                        stub_node.next = node;
                    }
                    prev = node;
                }
                node = prev->next;
            }

            if(temp_cmp.a != 0)
            {
                c = cRound(temp_cmp.c / temp_cmp.a);
            }
            else
            {
                c = cRound(c0);
            }
            node = acc;

            while(node)
            {
                suf32_t _c;
                cmp = (_CvPyramid *) (node->data);
                _c.i = c; cmp->c = _c.f;
                node = node->next;
            }

            comp.value = cRealScalar(c);
            comp.area = temp_cmp.a;
            comp.rect.x = temp_cmp.rect.x1;
            comp.rect.y = temp_cmp.rect.y1;
            comp.rect.width = temp_cmp.rect.x2 - temp_cmp.rect.x1;
            comp.rect.height = temp_cmp.rect.y2 - temp_cmp.rect.y1;
            comp.contour = 0;

            CC_WRITE_SEQ_ELEM(comp, writer);
            prev = stub_node.next;
        }
    }

    cvEndWriteSeq(&writer);
    return CC_OK;
}

/****************************************************************************************\

    clusterization segmented components    

\****************************************************************************************/
static void
icvExpandBaseLevelC3(_CvPyramidC3 * base_p, _CvPyramidC3 * p,
                      _CvPyramidBaseC3 * start, int width)
{
    int x = (int)((_CvPyramidBaseC3 *) base_p - start);
    int y = x / width;

    x -= y * width;
    p->a = 1;
    p->rect.x1 = (ushort) x;
    p->rect.y1 = (ushort) y;
    p->rect.x2 = (ushort) (x + 1);
    p->rect.y2 = (ushort) (y + 1);
    p->c = base_p->c;
}

CStatus
icvSegmentClusterC3(CvSeq * cmp_seq, CvSeq * res_seq,
                     double threshold,
                     _CvPyramidC3 * first_level_end, CSize first_level_size)
{
    const double eps = 1.;
    CvSeqWriter writer;
    CvSeqReader reader;
    _CvPyramidC3 temp_cmp;
    _CvPyramidBaseC3 *first_level_start = (_CvPyramidBaseC3 *) first_level_end -
        first_level_size.width * first_level_size.height;
    int i, count = cmp_seq->total;
    int c_blue, c_green, c_red;

    cvStartReadSeq(cmp_seq, &reader, 0);
    cvStartAppendToSeq(res_seq, &writer);

    if(threshold < eps)
    {
        /* if threshold is too small then simply copy all
           the components to the output sequence */
        for(i = 0; i < count; i++)
        {
            CConnectedComp comp;
            _CvPyramidC3 *cmp = (_CvPyramidC3 *) (((_CvListNode *) reader.ptr)->data);
            suf32_t _c;

            if(cmp < first_level_end)
            {
                icvExpandBaseLevelC3(cmp, &temp_cmp, first_level_start,
                                      first_level_size.width);
                cmp = &temp_cmp;
            }

            c_blue = cRound(cmp->c.blue);
            c_green = cRound(cmp->c.green);
            c_red = cRound(cmp->c.red);
            _c.i = c_blue; cmp->c.blue = _c.f;
            _c.i = c_green; cmp->c.green = _c.f;
            _c.i = c_red; cmp->c.red = _c.f;
            comp.value = cScalar(c_blue, c_green, c_red);
            comp.area = cmp->a;
            comp.rect.x = cmp->rect.x1;
            comp.rect.y = cmp->rect.y1;
            comp.rect.width = cmp->rect.x2 - cmp->rect.x1;
            comp.rect.height = cmp->rect.y2 - cmp->rect.y1;
            comp.contour = 0;

            CC_WRITE_SEQ_ELEM(comp, writer);
            CC_NEXT_SEQ_ELEM(sizeof(_CvListNode), reader);
        }
    }
    else
    {
        _CvListNode stub_node;
        _CvListNode *prev = &stub_node;

        stub_node.next = 0;

        for(i = 0; i < count; i++)
        {
            _CvListNode *node = (_CvListNode *) reader.ptr;

            prev->next = node;
            prev = node;
            CC_NEXT_SEQ_ELEM(sizeof(_CvListNode), reader);
        }
        prev->next = 0;
        prev = stub_node.next;

        while(prev)
        {
            _CvListNode *node = prev->next;
            _CvListNode *acc = prev;
            _CvPyramidC3 *cmp = (_CvPyramidC3 *) (acc->data);
            CConnectedComp comp;
            _CvRGBf c0 = cmp->c;

            if(cmp < first_level_end)
            {
                icvExpandBaseLevelC3(cmp, &temp_cmp, first_level_start,
                                      first_level_size.width);
            }
            else
            {
                temp_cmp = *cmp;
                temp_cmp.c.blue *= temp_cmp.a;
                temp_cmp.c.green *= temp_cmp.a;
                temp_cmp.c.red *= temp_cmp.a;
            }

            acc->next = 0;
            stub_node.next = 0;
            prev = &stub_node;

            while(node)
            {
                cmp = (_CvPyramidC3 *) (node->data);
                if(_CC_RGB_DIST(c0, cmp->c) < threshold)
                {
                    _CvPyramidC3 temp;

                    /* exclude from global list and add to list of joint component */
                    prev->next = node->next;
                    node->next = acc;
                    acc = node;

                    if(cmp < first_level_end)
                    {
                        icvExpandBaseLevelC3(cmp, &temp, first_level_start,
                                              first_level_size.width);
                        cmp = &temp;
                    }

                    temp_cmp.a += cmp->a;
                    temp_cmp.c.blue += cmp->c.blue * cmp->a;
                    temp_cmp.c.green += cmp->c.green * cmp->a;
                    temp_cmp.c.red += cmp->c.red * cmp->a;
                    icvMaxRoi(&(temp_cmp.rect), &(cmp->rect));
                }
                else
                {
                    if(prev == &stub_node)
                    {
                        stub_node.next = node;
                    }
                    prev = node;
                }
                node = prev->next;
            }

            if(temp_cmp.a != 0)
            {
                c_blue = cRound(temp_cmp.c.blue / temp_cmp.a);
                c_green = cRound(temp_cmp.c.green / temp_cmp.a);
                c_red = cRound(temp_cmp.c.red / temp_cmp.a);
            }
            else
            {
                c_blue = cRound(c0.blue);
                c_green = cRound(c0.green);
                c_red = cRound(c0.red);
            }
            node = acc;

            while(node)
            {
                suf32_t _c;
                cmp = (_CvPyramidC3 *) (node->data);
                _c.i = c_blue; cmp->c.blue = _c.f;
                _c.i = c_green; cmp->c.green = _c.f;
                _c.i = c_red; cmp->c.red = _c.f;
                node = node->next;
            }

            comp.value = cScalar(c_blue, c_green, c_red);
            comp.area = temp_cmp.a;
            comp.rect.x = temp_cmp.rect.x1;
            comp.rect.y = temp_cmp.rect.y1;
            comp.rect.width = temp_cmp.rect.x2 - temp_cmp.rect.x1;
            comp.rect.height = temp_cmp.rect.y2 - temp_cmp.rect.y1;
            comp.contour = 0;

            CC_WRITE_SEQ_ELEM(comp, writer);
            prev = stub_node.next;
        }
    }

    cvEndWriteSeq(&writer);
    return CC_OK;
}

/****************************************************************************************\

                 definition of the maximum roi size 

\****************************************************************************************/
void
icvMaxRoi(_CvRect16u * max_rect, _CvRect16u * cur_rect)
{
    if(max_rect->x2 == 0)
        *max_rect = *cur_rect;
    else
    {
        if(max_rect->x1 > cur_rect->x1)
            max_rect->x1 = cur_rect->x1;
        if(max_rect->y1 > cur_rect->y1)
            max_rect->y1 = cur_rect->y1;

        if(max_rect->x2 < cur_rect->x2)
            max_rect->x2 = cur_rect->x2;
        if(max_rect->y2 < cur_rect->y2)
            max_rect->y2 = cur_rect->y2;
    }
}

void
icvMaxRoi1(_CvRect16u * max_rect, int x, int y)
{
    if(max_rect->x2 == 0)
    {
        max_rect->x1 = (ushort) x;
        max_rect->y1 = (ushort) y;

        ++x;
        ++y;

        max_rect->x2 = (ushort) x;
        max_rect->y2 = (ushort) y;
    }
    else
    {
        if(max_rect->x1 > x)
            max_rect->x1 = (ushort) x;
        if(max_rect->y1 > y)
            max_rect->y1 = (ushort) y;

        ++x;
        ++y;

        if(max_rect->x2 < x)
            max_rect->x2 = (ushort) x;
        if(max_rect->y2 < y)
            max_rect->y2 = (ushort) y;
    }
}


/*F///////////////////////////////////////////////////////////////////////////////////////
//    Name:    cvPyrSegmentation
//    Purpose:
//      segments an image using pyramid-linking technique
//    Context: 
//    Parameters:
//      src - source image
//      dst - destination image
//      comp - pointer to returned connected component sequence
//      storage - where the sequence is stored
//      level - maximal pyramid level
//      threshold1 - first threshold, affecting on detalization level when pyramid
//                   is built.
//      threshold2 - second threshold - affects on final components merging.
//    Returns:
//    Notes:
//      Source and destination image must be equal types and channels
//F*/
CC_IMPL void
cvPyrSegmentation(img_t * src,
                   img_t * dst,
                   CvMemStorage * storage,
                   CvSeq ** comp, int level, double threshold1, double threshold2)
{
    CSize src_size, dst_size;
    uchar *src_data = 0;
    uchar *dst_data = 0;
    int src_step = 0, dst_step = 0;
    int thresh1 = cRound(threshold1);
    int thresh2 = cRound(threshold2);

    CC_FUNCNAME("cvPyrSegmentation");

    __BEGIN__;

    if(src->depth != IMG_DEPTH_8U)
        CC_ERROR(CC_BadDepth, cvUnsupportedFormat);

    if(src->depth != dst->depth || src->nChannels != dst->nChannels)
        CC_ERROR(CC_StsBadArg, "src and dst have different formats");

    cvGetRawData(src, &src_data, &src_step, &src_size);
    cvGetRawData(dst, &dst_data, &dst_step, &dst_size);

    if(src_size.width != dst_size.width ||
        src_size.height != dst_size.height)
        CC_ERROR(CC_StsBadArg, "src and dst have different ROIs");

    switch (src->nChannels)
    {
    case 1:
        IPPI_CALL(icvPyrSegmentation8uC1R(src_data, src_step,
                                            dst_data, dst_step,
                                            src_size,
                                            CC_GAUSSIAN_5x5,
                                            comp, storage, level, thresh1, thresh2));
        break;
    case 3:
        IPPI_CALL(icvPyrSegmentation8uC3R(src_data, src_step,
                                            dst_data, dst_step,
                                            src_size,
                                            CC_GAUSSIAN_5x5,
                                            comp, storage, level, thresh1, thresh2));
        break;
    default:
        CC_ERROR(CC_BadNumChannels, cvUnsupportedFormat);
    }
    __END__;
}



