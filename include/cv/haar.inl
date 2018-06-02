
#include <stdio.h>

/* these settings affect the quality of detection: change with care */
#define CC_ADJUST_FEATURES 1
#define CC_ADJUST_WEIGHTS  0

typedef int sumtype;
typedef double sqsumtype;

typedef struct CvHidHaarFeature
{
    struct
    {
        sumtype *p0, *p1, *p2, *p3;
        float weight;
    }
    rect[CC_HAAR_FEATURE_MAX];
}
CvHidHaarFeature;


typedef struct CvHidHaarTreeNode
{
    CvHidHaarFeature feature;
    float threshold;
    int left;
    int right;
}
CvHidHaarTreeNode;


typedef struct CvHidHaarClassifier
{
    int count;
    //CvHaarFeature* orig_feature;
    CvHidHaarTreeNode* node;
    float* alpha;
}
CvHidHaarClassifier;


typedef struct CvHidHaarStageClassifier
{
    int  count;
    float threshold;
    CvHidHaarClassifier* classifier;
    int two_rects;
    
    struct CvHidHaarStageClassifier* next;
    struct CvHidHaarStageClassifier* child;
    struct CvHidHaarStageClassifier* parent;
}
CvHidHaarStageClassifier;


struct CvHidHaarClassifierCascade
{
    int  count;
    int  is_stump_based;
    int  has_tilted_features;
    int  is_tree;
    double inv_window_area;
    img_t sum, sqsum, tilted;
    CvHidHaarStageClassifier* stage_classifier;
    sqsumtype *pq0, *pq1, *pq2, *pq3;
    sumtype *p0, *p1, *p2, *p3;

    void** ipp_stages;
};


/* IPP functions for object detection */
icvHaarClassifierInitAlloc_32f_t icvHaarClassifierInitAlloc_32f_p = 0;
icvHaarClassifierFree_32f_t icvHaarClassifierFree_32f_p = 0;
icvApplyHaarClassifier_32s32f_C1R_t icvApplyHaarClassifier_32s32f_C1R_p = 0;
icvRectStdDev_32s32f_C1R_t icvRectStdDev_32s32f_C1R_p = 0;

const int icv_object_win_border = 1;
const float icv_stage_threshold_bias = 0.0001f;

static CvHaarClassifierCascade*
icvCreateHaarClassifierCascade(int stage_count)
{
    CvHaarClassifierCascade* cascade = 0;
    
    CC_FUNCNAME("icvCreateHaarClassifierCascade");

    __BEGIN__;

    int block_size = sizeof(*cascade) + stage_count*sizeof(*cascade->stage_classifier);

    if(stage_count <= 0)
        CC_ERROR(CC_StsOutOfRange, "Number of stages should be positive");

    CC_CALL(cascade = (CvHaarClassifierCascade*)cAlloc(block_size));
    memset(cascade, 0, block_size);

    cascade->stage_classifier = (CvHaarStageClassifier*)(cascade + 1);
    cascade->flags = CC_HAAR_MAGIC_VAL;
    cascade->count = stage_count;

    __END__;

    return cascade;
}

static void
icvReleaseHidHaarClassifierCascade(CvHidHaarClassifierCascade** _cascade)
{
    if(_cascade && *_cascade)
    {
        CvHidHaarClassifierCascade* cascade = *_cascade;
        if(cascade->ipp_stages && icvHaarClassifierFree_32f_p)
        {
            int i;
            for(i = 0; i < cascade->count; i++)
            {
                if(cascade->ipp_stages[i])
                    icvHaarClassifierFree_32f_p(cascade->ipp_stages[i]);
            }
        }
        cFree(&cascade->ipp_stages);
        cFree(_cascade);
    }
}

/* create more efficient internal representation of haar classifier cascade */
static CvHidHaarClassifierCascade*
icvCreateHidHaarClassifierCascade(CvHaarClassifierCascade* cascade)
{
    CRect* ipp_features = 0;
    float *ipp_weights = 0, *ipp_thresholds = 0, *ipp_val1 = 0, *ipp_val2 = 0;
    int* ipp_counts = 0;

    CvHidHaarClassifierCascade* out = 0;

    CC_FUNCNAME("icvCreateHidHaarClassifierCascade");

    __BEGIN__;

    int i, j, k, l;
    int datasize;
    int total_classifiers = 0;
    int total_nodes = 0;
    char errorstr[100];
    CvHidHaarClassifier* haar_classifier_ptr;
    CvHidHaarTreeNode* haar_node_ptr;
    CSize orig_window_size;
    int has_tilted_features = 0;
    int max_count = 0;

    if(!CC_IS_HAAR_CLASSIFIER(cascade))
        CC_ERROR(!cascade ? CC_StsNullPtr : CC_StsBadArg, "Invalid classifier pointer");

    if(cascade->hid_cascade)
        CC_ERROR(CC_StsError, "hid_cascade has been already created");

    if(!cascade->stage_classifier)
        CC_ERROR(CC_StsNullPtr, "");

    if(cascade->count <= 0)
        CC_ERROR(CC_StsOutOfRange, "Negative number of cascade stages");

    orig_window_size = cascade->orig_window_size;
    
    /* check input structure correctness and calculate total memory size needed for
       internal representation of the classifier cascade */
    for(i = 0; i < cascade->count; i++)
    {
        CvHaarStageClassifier* stage_classifier = cascade->stage_classifier + i;

        if(!stage_classifier->classifier ||
            stage_classifier->count <= 0)
        {
            sprintf(errorstr, "header of the stage classifier #%d is invalid "
                     "(has null pointers or non-positive classfier count)", i);
            CC_ERROR(CC_StsError, errorstr);
        }

        max_count = MAX(max_count, stage_classifier->count);
        total_classifiers += stage_classifier->count;

        for(j = 0; j < stage_classifier->count; j++)
        {
            CvHaarClassifier* classifier = stage_classifier->classifier + j;

            total_nodes += classifier->count;
            for(l = 0; l < classifier->count; l++)
            {
                for(k = 0; k < CC_HAAR_FEATURE_MAX; k++)
                {
                    if(classifier->haar_feature[l].rect[k].r.width)
                    {
                        CRect r = classifier->haar_feature[l].rect[k].r;
                        int tilted = classifier->haar_feature[l].tilted;
                        has_tilted_features |= tilted != 0;
                        if(r.width < 0 || r.height < 0 || r.y < 0 ||
                            r.x + r.width > orig_window_size.width
                            ||
                            (!tilted &&
                            (r.x < 0 || r.y + r.height > orig_window_size.height))
                            ||
                            (tilted && (r.x - r.height < 0 ||
                            r.y + r.width + r.height > orig_window_size.height)))
                        {
                            sprintf(errorstr, "rectangle #%d of the classifier #%d of "
                                     "the stage classifier #%d is not inside "
                                     "the reference (original) cascade window", k, j, i);
                            CC_ERROR(CC_StsNullPtr, errorstr);
                        }
                    }
                }
            }
        }
    }

    // this is an upper boundary for the whole hidden cascade size
    datasize = sizeof(CvHidHaarClassifierCascade) +
               sizeof(CvHidHaarStageClassifier)*cascade->count +
               sizeof(CvHidHaarClassifier) * total_classifiers +
               sizeof(CvHidHaarTreeNode) * total_nodes +
               sizeof(void*)*(total_nodes + total_classifiers);

    CC_CALL(out = (CvHidHaarClassifierCascade*)cAlloc(datasize));
    memset(out, 0, sizeof(*out));

    /* init header */
    out->count = cascade->count;
    out->stage_classifier = (CvHidHaarStageClassifier*)(out + 1);
    haar_classifier_ptr = (CvHidHaarClassifier*)(out->stage_classifier + cascade->count);
    haar_node_ptr = (CvHidHaarTreeNode*)(haar_classifier_ptr + total_classifiers);

    out->is_stump_based = 1;
    out->has_tilted_features = has_tilted_features;
    out->is_tree = 0;

    /* initialize internal representation */
    for(i = 0; i < cascade->count; i++)
    {
        CvHaarStageClassifier* stage_classifier = cascade->stage_classifier + i;
        CvHidHaarStageClassifier* hid_stage_classifier = out->stage_classifier + i;

        hid_stage_classifier->count = stage_classifier->count;
        hid_stage_classifier->threshold = stage_classifier->threshold - icv_stage_threshold_bias;
        hid_stage_classifier->classifier = haar_classifier_ptr;
        hid_stage_classifier->two_rects = 1;
        haar_classifier_ptr += stage_classifier->count;

        hid_stage_classifier->parent = (stage_classifier->parent == -1)
            ? NULL : out->stage_classifier + stage_classifier->parent;
        hid_stage_classifier->next = (stage_classifier->next == -1)
            ? NULL : out->stage_classifier + stage_classifier->next;
        hid_stage_classifier->child = (stage_classifier->child == -1)
            ? NULL : out->stage_classifier + stage_classifier->child;
        
        out->is_tree |= hid_stage_classifier->next != NULL;

        for(j = 0; j < stage_classifier->count; j++)
        {
            CvHaarClassifier* classifier = stage_classifier->classifier + j;
            CvHidHaarClassifier* hid_classifier = hid_stage_classifier->classifier + j;
            int node_count = classifier->count;
            float* alpha_ptr = (float*)(haar_node_ptr + node_count);

            hid_classifier->count = node_count;
            hid_classifier->node = haar_node_ptr;
            hid_classifier->alpha = alpha_ptr;
            
            for(l = 0; l < node_count; l++)
            {
                CvHidHaarTreeNode* node = hid_classifier->node + l;
                CvHaarFeature* feature = classifier->haar_feature + l;
                memset(node, -1, sizeof(*node));
                node->threshold = classifier->threshold[l];
                node->left = classifier->left[l];
                node->right = classifier->right[l];

                if(fabs(feature->rect[2].weight) < DBL_EPSILON ||
                    feature->rect[2].r.width == 0 ||
                    feature->rect[2].r.height == 0)
                    memset(&(node->feature.rect[2]), 0, sizeof(node->feature.rect[2]));
                else
                    hid_stage_classifier->two_rects = 0;
            }

            memcpy(alpha_ptr, classifier->alpha, (node_count+1)*sizeof(alpha_ptr[0]));
            haar_node_ptr =
                (CvHidHaarTreeNode*)cvAlignPtr(alpha_ptr+node_count+1, sizeof(void*));

            out->is_stump_based &= node_count == 1;
        }
    }

    //
    // NOTE: Currently, OpenMP is implemented and IPP modes are incompatible.
    // 
#ifndef _OPENMP
    {
    int can_use_ipp = icvHaarClassifierInitAlloc_32f_p != 0 &&
        icvHaarClassifierFree_32f_p != 0 &&
                      icvApplyHaarClassifier_32s32f_C1R_p != 0 &&
                      icvRectStdDev_32s32f_C1R_p != 0 &&
                      !out->has_tilted_features && !out->is_tree && out->is_stump_based;

    if(can_use_ipp)
    {
        int ipp_datasize = cascade->count*sizeof(out->ipp_stages[0]);
        float ipp_weight_scale=(float)(1./((orig_window_size.width-icv_object_win_border*2)*
            (orig_window_size.height-icv_object_win_border*2)));

        CC_CALL(out->ipp_stages = (void**)cAlloc(ipp_datasize));
        memset(out->ipp_stages, 0, ipp_datasize);

        CC_CALL(ipp_features = (CRect*)cAlloc(max_count*3*sizeof(ipp_features[0])));
        CC_CALL(ipp_weights = (float*)cAlloc(max_count*3*sizeof(ipp_weights[0])));
        CC_CALL(ipp_thresholds = (float*)cAlloc(max_count*sizeof(ipp_thresholds[0])));
        CC_CALL(ipp_val1 = (float*)cAlloc(max_count*sizeof(ipp_val1[0])));
        CC_CALL(ipp_val2 = (float*)cAlloc(max_count*sizeof(ipp_val2[0])));
        CC_CALL(ipp_counts = (int*)cAlloc(max_count*sizeof(ipp_counts[0])));

        for(i = 0; i < cascade->count; i++)
        {
            CvHaarStageClassifier* stage_classifier = cascade->stage_classifier + i;
            for(j = 0, k = 0; j < stage_classifier->count; j++)
            {
                CvHaarClassifier* classifier = stage_classifier->classifier + j;
                int rect_count = 2 + (classifier->haar_feature->rect[2].r.width != 0);

                ipp_thresholds[j] = classifier->threshold[0];
                ipp_val1[j] = classifier->alpha[0];
                ipp_val2[j] = classifier->alpha[1];
                ipp_counts[j] = rect_count;
                
                for(l = 0; l < rect_count; l++, k++)
                {
                    ipp_features[k] = classifier->haar_feature->rect[l].r;
                    //ipp_features[k].y = orig_window_size.height - ipp_features[k].y - ipp_features[k].height;
                    ipp_weights[k] = classifier->haar_feature->rect[l].weight*ipp_weight_scale;
                }
            }
            
            if(icvHaarClassifierInitAlloc_32f_p(&out->ipp_stages[i],
                ipp_features, ipp_weights, ipp_thresholds,
                ipp_val1, ipp_val2, ipp_counts, stage_classifier->count) < 0)
                break;
        }

        if(i < cascade->count)
        {
            for(j = 0; j < i; j++)
                if(icvHaarClassifierFree_32f_p && out->ipp_stages[i])
                    icvHaarClassifierFree_32f_p(out->ipp_stages[i]);
            cFree(&out->ipp_stages);
        }
    }
    }
#endif

    cascade->hid_cascade = out;
    assert((char*)haar_node_ptr - (char*)out <= datasize);

    __END__;

    if(cvGetErrStatus() < 0)
        icvReleaseHidHaarClassifierCascade(&out);

    cFree(&ipp_features);
    cFree(&ipp_weights);
    cFree(&ipp_thresholds);
    cFree(&ipp_val1);
    cFree(&ipp_val2);
    cFree(&ipp_counts);

    return out;
}


#define sum_elem_ptr(sum,row,col)  \
    ((sumtype*)CC_MAT_ELEM_PTR_FAST((sum),(row),(col),sizeof(sumtype)))

#define sqsum_elem_ptr(sqsum,row,col)  \
    ((sqsumtype*)CC_MAT_ELEM_PTR_FAST((sqsum),(row),(col),sizeof(sqsumtype)))

#define calc_sum(rect,offset) \
    ((rect).p0[offset] - (rect).p1[offset] - (rect).p2[offset] + (rect).p3[offset])


CC_IMPL void
cvSetImagesForHaarClassifierCascade(CvHaarClassifierCascade* _cascade,
                                     const img_t* _sum,
                                     const img_t* _sqsum,
                                     const img_t* _tilted_sum,
                                     double scale)
{
    CC_FUNCNAME("cvSetImagesForHaarClassifierCascade");

    __BEGIN__;

    img_t sum_stub, *sum = _sum;
    img_t sqsum_stub, *sqsum = _sqsum;
    img_t tilted_stub, *tilted = _tilted_sum;
    CvHidHaarClassifierCascade* cascade;
    int coi0 = 0, coi1 = 0;
    int i;
    CRect equ_rect;
    double weight_scale;

    if(!CC_IS_HAAR_CLASSIFIER(_cascade))
        CC_ERROR(!_cascade ? CC_StsNullPtr : CC_StsBadArg, "Invalid classifier pointer");

    if(scale <= 0)
        CC_ERROR(CC_StsOutOfRange, "Scale must be positive");

    CC_CALL(sum = cvGetMat(sum, &sum_stub, &coi0));
    CC_CALL(sqsum = cvGetMat(sqsum, &sqsum_stub, &coi1));

    if(coi0 || coi1)
        CC_ERROR(CC_BadCOI, "COI is not supported");

    if(!CC_ARE_SIZES_EQ(sum, sqsum))
        CC_ERROR(CC_StsUnmatchedSizes, "All integral images must have the same size");

    if(CC_MAT_TYPE(sqsum->tid) != CC_64F ||
        CC_MAT_TYPE(sum) != CC_32SC1)
        CC_ERROR(CC_StsUnsupportedFormat,
        "Only (32s, 64f, 32s) combination of (sum,sqsum,tilted_sum) formats is allowed");

    if(!_cascade->hid_cascade)
        CC_CALL(icvCreateHidHaarClassifierCascade(_cascade));

    cascade = _cascade->hid_cascade;

    if(cascade->has_tilted_features)
    {
        CC_CALL(tilted = cvGetMat(tilted, &tilted_stub, &coi1));

        if(CC_MAT_TYPE(tilted->tid) != CC_32SC1)
            CC_ERROR(CC_StsUnsupportedFormat,
            "Only (32s, 64f, 32s) combination of (sum,sqsum,tilted_sum) formats is allowed");

        if(sum->step != tilted->step)
            CC_ERROR(CC_StsUnmatchedSizes,
            "Sum and tilted_sum must have the same stride (step, widthStep)");

        if(!CC_ARE_SIZES_EQ(sum, tilted))
            CC_ERROR(CC_StsUnmatchedSizes, "All integral images must have the same size");
        cascade->tilted = *tilted;
    }
    
    _cascade->scale = scale;
    _cascade->real_window_size.width = cRound(_cascade->orig_window_size.width * scale);
    _cascade->real_window_size.height = cRound(_cascade->orig_window_size.height * scale);

    cascade->sum = *sum;
    cascade->sqsum = *sqsum;
    
    equ_rect.x = equ_rect.y = cRound(scale);
    equ_rect.width = cRound((_cascade->orig_window_size.width-2)*scale);
    equ_rect.height = cRound((_cascade->orig_window_size.height-2)*scale);
    weight_scale = 1./(equ_rect.width*equ_rect.height);
    cascade->inv_window_area = weight_scale;

    cascade->p0 = sum_elem_ptr(*sum, equ_rect.y, equ_rect.x);
    cascade->p1 = sum_elem_ptr(*sum, equ_rect.y, equ_rect.x + equ_rect.width);
    cascade->p2 = sum_elem_ptr(*sum, equ_rect.y + equ_rect.height, equ_rect.x);
    cascade->p3 = sum_elem_ptr(*sum, equ_rect.y + equ_rect.height,
                                     equ_rect.x + equ_rect.width);

    cascade->pq0 = sqsum_elem_ptr(*sqsum, equ_rect.y, equ_rect.x);
    cascade->pq1 = sqsum_elem_ptr(*sqsum, equ_rect.y, equ_rect.x + equ_rect.width);
    cascade->pq2 = sqsum_elem_ptr(*sqsum, equ_rect.y + equ_rect.height, equ_rect.x);
    cascade->pq3 = sqsum_elem_ptr(*sqsum, equ_rect.y + equ_rect.height,
                                          equ_rect.x + equ_rect.width);

    /* init pointers in haar features according to real window size and
       given image pointers */
    {
#ifdef _OPENMP
    int max_threads = cGetNumThreads();
    #pragma omp parallel for num_threads(max_threads), schedule(dynamic) 
#endif // _OPENMP
    for(i = 0; i < _cascade->count; i++)
    {
        int j, k, l;
        for(j = 0; j < cascade->stage_classifier[i].count; j++)
        {
            for(l = 0; l < cascade->stage_classifier[i].classifier[j].count; l++)
            {
                CvHaarFeature* feature = 
                    &_cascade->stage_classifier[i].classifier[j].haar_feature[l];
                /* CvHidHaarClassifier* classifier =
                    cascade->stage_classifier[i].classifier + j; */
                CvHidHaarFeature* hidfeature = 
                    &cascade->stage_classifier[i].classifier[j].node[l].feature;
                double sum0 = 0, area0 = 0;
                CRect r[3];
#if CC_ADJUST_FEATURES
                int base_w = -1, base_h = -1;
                int new_base_w = 0, new_base_h = 0;
                int kx, ky;
                int flagx = 0, flagy = 0;
                int x0 = 0, y0 = 0;
#endif
                int nr;

                /* align blocks */
                for(k = 0; k < CC_HAAR_FEATURE_MAX; k++)
                {
                    if(!hidfeature->rect[k].p0)
                        break;
#if CC_ADJUST_FEATURES
                    r[k] = feature->rect[k].r;
                    base_w = (int)CC_IMIN((unsigned)base_w, (unsigned)(r[k].width-1));
                    base_w = (int)CC_IMIN((unsigned)base_w, (unsigned)(r[k].x - r[0].x-1));
                    base_h = (int)CC_IMIN((unsigned)base_h, (unsigned)(r[k].height-1));
                    base_h = (int)CC_IMIN((unsigned)base_h, (unsigned)(r[k].y - r[0].y-1));
#endif
                }

                nr = k;

#if CC_ADJUST_FEATURES
                base_w += 1;
                base_h += 1;
                kx = r[0].width / base_w;
                ky = r[0].height / base_h;

                if(kx <= 0)
                {
                    flagx = 1;
                    new_base_w = cRound(r[0].width * scale) / kx;
                    x0 = cRound(r[0].x * scale);
                }

                if(ky <= 0)
                {
                    flagy = 1;
                    new_base_h = cRound(r[0].height * scale) / ky;
                    y0 = cRound(r[0].y * scale);
                }
#endif
        
                for(k = 0; k < nr; k++)
                {
                    CRect tr;
                    double correction_ratio;
            
#if CC_ADJUST_FEATURES
                    if(flagx)
                    {
                        tr.x = (r[k].x - r[0].x) * new_base_w / base_w + x0;
                        tr.width = r[k].width * new_base_w / base_w;
                    }
                    else
#endif
                    {
                        tr.x = cRound(r[k].x * scale);
                        tr.width = cRound(r[k].width * scale);
                    }

#if CC_ADJUST_FEATURES
                    if(flagy)
                    {
                        tr.y = (r[k].y - r[0].y) * new_base_h / base_h + y0;
                        tr.height = r[k].height * new_base_h / base_h;
                    }
                    else
#endif
                    {
                        tr.y = cRound(r[k].y * scale);
                        tr.height = cRound(r[k].height * scale);
                    }

#if CC_ADJUST_WEIGHTS
                    {
                    // RAINER START
                    const float orig_feature_size =  (float)(feature->rect[k].r.width)*feature->rect[k].r.height; 
                    const float orig_norm_size = (float)(_cascade->orig_window_size.width)*(_cascade->orig_window_size.height);
                    const float feature_size = float(tr.width*tr.height);
                    //const float normSize    = float(equ_rect.width*equ_rect.height);
                    float target_ratio = orig_feature_size / orig_norm_size;
                    //float isRatio = featureSize / normSize;
                    //correctionRatio = targetRatio / isRatio / normSize;
                    correction_ratio = target_ratio / feature_size;
                    // RAINER END
                    }
#else
                    correction_ratio = weight_scale * (!feature->tilted ? 1 : 0.5);
#endif

                    if(!feature->tilted)
                    {
                        hidfeature->rect[k].p0 = sum_elem_ptr(*sum, tr.y, tr.x);
                        hidfeature->rect[k].p1 = sum_elem_ptr(*sum, tr.y, tr.x + tr.width);
                        hidfeature->rect[k].p2 = sum_elem_ptr(*sum, tr.y + tr.height, tr.x);
                        hidfeature->rect[k].p3 = sum_elem_ptr(*sum, tr.y + tr.height, tr.x + tr.width);
                    }
                    else
                    {
                        hidfeature->rect[k].p2 = sum_elem_ptr(*tilted, tr.y + tr.width, tr.x + tr.width);
                        hidfeature->rect[k].p3 = sum_elem_ptr(*tilted, tr.y + tr.width + tr.height,
                                                              tr.x + tr.width - tr.height);
                        hidfeature->rect[k].p0 = sum_elem_ptr(*tilted, tr.y, tr.x);
                        hidfeature->rect[k].p1 = sum_elem_ptr(*tilted, tr.y + tr.height, tr.x - tr.height);
                    }

                    hidfeature->rect[k].weight = (float)(feature->rect[k].weight * correction_ratio);

                    if(k == 0)
                        area0 = tr.width * tr.height;
                    else
                        sum0 += hidfeature->rect[k].weight * tr.width * tr.height;
                }

                hidfeature->rect[0].weight = (float)(-sum0/area0);
            } /* l */
        } /* j */
    }
    }

    __END__;
}


CC_INLINE
double icvEvalHidHaarClassifier(CvHidHaarClassifier* classifier,
                                 double variance_norm_factor,
                                 size_t p_offset)
{
    int idx = 0;
    do 
    {
        CvHidHaarTreeNode* node = classifier->node + idx;
        double t = node->threshold * variance_norm_factor;

        double sum = calc_sum(node->feature.rect[0],p_offset) * node->feature.rect[0].weight;
        sum += calc_sum(node->feature.rect[1],p_offset) * node->feature.rect[1].weight;

        if(node->feature.rect[2].p0)
            sum += calc_sum(node->feature.rect[2],p_offset) * node->feature.rect[2].weight;

        idx = sum < t ? node->left : node->right;
    }
    while(idx > 0);
    return classifier->alpha[-idx];
}


CC_IMPL int
cvRunHaarClassifierCascade(CvHaarClassifierCascade* _cascade,
                            CPoint pt, int start_stage)
{
    int result = -1;
    CC_FUNCNAME("cvRunHaarClassifierCascade");

    __BEGIN__;

    int p_offset, pq_offset;
    int i, j;
    double mean, variance_norm_factor;
    CvHidHaarClassifierCascade* cascade;

    if(!CC_IS_HAAR_CLASSIFIER(_cascade))
        CC_ERROR(!_cascade ? CC_StsNullPtr : CC_StsBadArg, "Invalid cascade pointer");

    cascade = _cascade->hid_cascade;
    if(!cascade)
        CC_ERROR(CC_StsNullPtr, "Hidden cascade has not been created.\n"
            "Use cvSetImagesForHaarClassifierCascade");

    if(pt.x < 0 || pt.y < 0 ||
        pt.x + _cascade->real_window_size.width >= cascade->sum.width-2 ||
        pt.y + _cascade->real_window_size.height >= cascade->sum.height-2)
        EXIT;

    p_offset = pt.y * (cascade->sum->step/sizeof(sumtype)) + pt.x;
    pq_offset = pt.y * (cascade->sqsum->step/sizeof(sqsumtype)) + pt.x;
    mean = calc_sum(*cascade,p_offset)*cascade->inv_window_area;
    variance_norm_factor = cascade->pq0[pq_offset] - cascade->pq1[pq_offset] -
                           cascade->pq2[pq_offset] + cascade->pq3[pq_offset];
    variance_norm_factor = variance_norm_factor*cascade->inv_window_area - mean*mean;
    if(variance_norm_factor >= 0.)
        variance_norm_factor = sqrt(variance_norm_factor);
    else
        variance_norm_factor = 1.;

    if(cascade->is_tree)
    {
        CvHidHaarStageClassifier* ptr;
        assert(start_stage == 0);

        result = 1;
        ptr = cascade->stage_classifier;

        while(ptr)
        {
            double stage_sum = 0;

            for(j = 0; j < ptr->count; j++)
            {
                stage_sum += icvEvalHidHaarClassifier(ptr->classifier + j,
                    variance_norm_factor, p_offset);
            }

            if(stage_sum >= ptr->threshold)
            {
                ptr = ptr->child;
            }
            else
            {
                while(ptr && ptr->next == NULL) ptr = ptr->parent;
                if(ptr == NULL)
                {
                    result = 0;
                    EXIT;
                }
                ptr = ptr->next;
            }
        }
    }
    else if(cascade->is_stump_based)
    {
        for(i = start_stage; i < cascade->count; i++)
        {
            double stage_sum = 0;

            if(cascade->stage_classifier[i].two_rects)
            {
                for(j = 0; j < cascade->stage_classifier[i].count; j++)
                {
                    CvHidHaarClassifier* classifier = cascade->stage_classifier[i].classifier + j;
                    CvHidHaarTreeNode* node = classifier->node;
                    double sum, t = node->threshold*variance_norm_factor, a, b;

                    sum = calc_sum(node->feature.rect[0],p_offset) * node->feature.rect[0].weight;
                    sum += calc_sum(node->feature.rect[1],p_offset) * node->feature.rect[1].weight;

                    a = classifier->alpha[0];
                    b = classifier->alpha[1];
                    stage_sum += sum < t ? a : b;
                }
            }
            else
            {
                for(j = 0; j < cascade->stage_classifier[i].count; j++)
                {
                    CvHidHaarClassifier* classifier = cascade->stage_classifier[i].classifier + j;
                    CvHidHaarTreeNode* node = classifier->node;
                    double sum, t = node->threshold*variance_norm_factor, a, b;

                    sum = calc_sum(node->feature.rect[0],p_offset) * node->feature.rect[0].weight;
                    sum += calc_sum(node->feature.rect[1],p_offset) * node->feature.rect[1].weight;

                    if(node->feature.rect[2].p0)
                        sum += calc_sum(node->feature.rect[2],p_offset) * node->feature.rect[2].weight;

                    a = classifier->alpha[0];
                    b = classifier->alpha[1];
                    stage_sum += sum < t ? a : b;
                }
            }

            if(stage_sum < cascade->stage_classifier[i].threshold)
            {
                result = -i;
                EXIT;
            }
        }
    }
    else
    {
        for(i = start_stage; i < cascade->count; i++)
        {
            double stage_sum = 0;

            for(j = 0; j < cascade->stage_classifier[i].count; j++)
            {
                stage_sum += icvEvalHidHaarClassifier(
                    cascade->stage_classifier[i].classifier + j,
                    variance_norm_factor, p_offset);
            }

            if(stage_sum < cascade->stage_classifier[i].threshold)
            {
                result = -i;
                EXIT;
            }
        }
    }

    result = 1;

    __END__;

    return result;
}


static int is_equal(const void* _r1, const void* _r2, void*)
{
    const CRect* r1 = (const CRect*)_r1;
    const CRect* r2 = (const CRect*)_r2;
    int distance = cRound(r1->width*0.2);

    return r2->x <= r1->x + distance &&
           r2->x >= r1->x - distance &&
           r2->y <= r1->y + distance &&
           r2->y >= r1->y - distance &&
           r2->width <= cRound(r1->width * 1.2) &&
           cRound(r2->width * 1.2) >= r1->width;
}


CC_IMPL CvSeq*
cvHaarDetectObjects(const img_t* _img,
                     CvHaarClassifierCascade* cascade,
                     CvMemStorage* storage, double scale_factor,
                     int min_neighbors, int flags, CSize min_size)
{
    int split_stage = 2;

    img_t stub, *img = _img;
    img_t *temp = 0, *sum = 0, *tilted = 0, *sqsum = 0, *norm_img = 0, *sumcanny = 0, *img_small = 0;
    CvSeq* seq = 0;
    CvSeq* seq2 = 0;
    CvSeq* idx_seq = 0;
    CvSeq* result_seq = 0;
    CvMemStorage* temp_storage = 0;
    CvAvgComp* comps = 0;
    int i;
    
#ifdef _OPENMP
    CvSeq* seq_thread[CC_MAX_THREADS] = {0};
    int max_threads = 0;
#endif
    
    CC_FUNCNAME("cvHaarDetectObjects");

    __BEGIN__;

    double factor;
    int npass = 2, coi;
    int do_canny_pruning = flags & CC_HAAR_DO_CANNY_PRUNING;

    if(!CC_IS_HAAR_CLASSIFIER(cascade))
        CC_ERROR(!cascade ? CC_StsNullPtr : CC_StsBadArg, "Invalid classifier cascade");

    if(!storage)
        CC_ERROR(CC_StsNullPtr, "Null storage pointer");

    CC_CALL(img = cvGetMat(img, &stub, &coi));
    if(coi)
        CC_ERROR(CC_BadCOI, "COI is not supported");

    if(CC_MAT_TYPE(img) != CC_8U)
        CC_ERROR(CC_StsUnsupportedFormat, "Only 8-bit images are supported");

    CC_CALL(temp = cvCreateMat(img->rows, img->cols, CC_8UC1));
    CC_CALL(sum = cvCreateMat(img->rows + 1, img->cols + 1, CC_32SC1));
    CC_CALL(sqsum = cvCreateMat(img->rows + 1, img->cols + 1, CC_64FC1));
    CC_CALL(temp_storage = cvCreateChildMemStorage(storage));

#ifdef _OPENMP
    max_threads = cGetNumThreads();
    for(i = 0; i < max_threads; i++)
    {
        CvMemStorage* temp_storage_thread;
        CC_CALL(temp_storage_thread = cvCreateMemStorage(0));
        CC_CALL(seq_thread[i] = cvCreateSeq(0, sizeof(CvSeq),
                        sizeof(CRect), temp_storage_thread));
    }
#endif

    if(!cascade->hid_cascade)
        CC_CALL(icvCreateHidHaarClassifierCascade(cascade));

    if(cascade->hid_cascade->has_tilted_features)
        tilted = cvCreateMat(img->rows + 1, img->cols + 1, CC_32SC1);

    seq = cvCreateSeq(0, sizeof(CvSeq), sizeof(CRect), temp_storage);
    seq2 = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvAvgComp), temp_storage);
    result_seq = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvAvgComp), storage);

    if(min_neighbors == 0)
        seq = result_seq;

    if(CC_MAT_CN(img->tid) > 1)
    {
        cvCvtColor(img, temp, CC_BGR2GRAY);
        img = temp;
    }
    
    if(flags & CC_HAAR_SCALE_IMAGE)
    {
        CSize win_size0 = cascade->orig_window_size;
        int use_ipp = cascade->hid_cascade->ipp_stages != 0 &&
                    icvApplyHaarClassifier_32s32f_C1R_p != 0;

        if(use_ipp)
            CC_CALL(norm_img = cvCreateMat(img->rows, img->cols, CC_32FC1));
        CC_CALL(img_small = cvCreateMat(img->rows + 1, img->cols + 1, CC_8UC1));

        for(factor = 1; ; factor *= scale_factor)
        {
            int positive = 0;
            int x, y;
            CSize win_size = { cRound(win_size0.width*factor),
                                cRound(win_size0.height*factor) };
            CSize sz = { cRound(img->cols/factor), cRound(img->rows/factor) };
            CSize sz1 = { sz.width - win_size0.width, sz.height - win_size0.height };
            CRect rect1 = { icv_object_win_border, icv_object_win_border,
                win_size0.width - icv_object_win_border*2,
                win_size0.height - icv_object_win_border*2 };
            img_t img1, sum1, sqsum1, norm1, tilted1, mask1;
            img_t* _tilted = 0;

            if(sz1.width <= 0 || sz1.height <= 0)
                break;
            if(win_size.width < min_size.width || win_size.height < min_size.height)
                continue;

            img1 = cvMat(sz.height, sz.width, CC_8UC1, img_small->tt.data);
            sum1 = cvMat(sz.height+1, sz.width+1, CC_32SC1, sum->tt.data);
            sqsum1 = cvMat(sz.height+1, sz.width+1, CC_64FC1, sqsum->tt.data);
            if(tilted)
            {
                tilted1 = cvMat(sz.height+1, sz.width+1, CC_32SC1, tilted->tt.data);
                _tilted = &tilted1;
            }
            norm1 = cvMat(sz1.height, sz1.width, CC_32FC1, norm_img ? norm_img->tt.data : 0);
            mask1 = cvMat(sz1.height, sz1.width, CC_8UC1, temp->tt.data);

            cvResize(img, &img1, CC_INTER_LINEAR);
            cvIntegral(&img1, &sum1, &sqsum1, _tilted);

            if(use_ipp && icvRectStdDev_32s32f_C1R_p(sum1->tt.i, sum1->step,
                sqsum1->tt.db, sqsum1->step, norm1->tt.fl, norm1->step, sz1, rect1) < 0)
                use_ipp = 0;

            if(use_ipp)
            {
                positive = mask1->cols*mask1->rows;
                cvSet(&mask1, cScalarAll(255));
                for(i = 0; i < cascade->count; i++)
                {
                    if(icvApplyHaarClassifier_32s32f_C1R_p(sum1->tt.i, sum1->step,
                        norm1->tt.fl, norm1->step, mask1->tt.data, mask1->step,
                        sz1, &positive, cascade->hid_cascade->stage_classifier[i].threshold,
                        cascade->hid_cascade->ipp_stages[i]) < 0)
                    {
                        use_ipp = 0;
                        break;
                    }
                    if(positive <= 0)
                        break;
                }
            }
            
            if(!use_ipp)
            {
                cvSetImagesForHaarClassifierCascade(cascade, &sum1, &sqsum1, 0, 1.);
                for(y = 0, positive = 0; y < sz1.height; y++)
                    for(x = 0; x < sz1.width; x++)
                    {
                        mask1->tt.data[mask1->step*y + x] =
                            cvRunHaarClassifierCascade(cascade, cPoint(x,y), 0) > 0;
                        positive += mask1->tt.data[mask1->step*y + x];
                    }
            }

            if(positive > 0)
            {
                for(y = 0; y < sz1.height; y++)
                    for(x = 0; x < sz1.width; x++)
                        if(mask1->tt.data[mask1->step*y + x] != 0)
                        {
                            CRect obj_rect = { cRound(y*factor), cRound(x*factor),
                                                win_size.width, win_size.height };
                            cvSeqPush(seq, &obj_rect);
                        }
            }
        }
    }
    else
    {
        cvIntegral(img, sum, sqsum, tilted);
    
        if(do_canny_pruning)
        {
            sumcanny = cvCreateMat(img->rows + 1, img->cols + 1, CC_32SC1);
            cvCanny(img, temp, 0, 50, 3);
            cvIntegral(temp, sumcanny);
        }
    
        if((unsigned)split_stage >= (unsigned)cascade->count ||
            cascade->hid_cascade->is_tree)
        {
            split_stage = cascade->count;
            npass = 1;
        }

        for(factor = 1; factor*cascade->orig_window_size.width < img->cols - 10 &&
                         factor*cascade->orig_window_size.height < img->rows - 10;
             factor *= scale_factor)
        {
            const double ystep = MAX(2, factor);
            CSize win_size = { cRound(cascade->orig_window_size.width * factor),
                                cRound(cascade->orig_window_size.height * factor)};
            CRect equ_rect = { 0, 0, 0, 0 };
            int *p0 = 0, *p1 = 0, *p2 = 0, *p3 = 0;
            int *pq0 = 0, *pq1 = 0, *pq2 = 0, *pq3 = 0;
            int pass, stage_offset = 0;
            int stop_height = cRound((img->rows - win_size.height) / ystep);

            if(win_size.width < min_size.width || win_size.height < min_size.height)
                continue;

            cvSetImagesForHaarClassifierCascade(cascade, sum, sqsum, tilted, factor);
            cvZero(temp);

            if(do_canny_pruning)
            {
                equ_rect.x = cRound(win_size.width*0.15);
                equ_rect.y = cRound(win_size.height*0.15);
                equ_rect.width = cRound(win_size.width*0.7);
                equ_rect.height = cRound(win_size.height*0.7);

                p0 = (int*)(sumcanny->tt.data + equ_rect.y*sumcanny->step) + equ_rect.x;
                p1 = (int*)(sumcanny->tt.data + equ_rect.y*sumcanny->step)
                            + equ_rect.x + equ_rect.width;
                p2 = (int*)(sumcanny->tt.data + (equ_rect.y + equ_rect.height)*sumcanny->step) + equ_rect.x;
                p3 = (int*)(sumcanny->tt.data + (equ_rect.y + equ_rect.height)*sumcanny->step)
                            + equ_rect.x + equ_rect.width;

                pq0 = (int*)(sum->tt.data + equ_rect.y*sum->step) + equ_rect.x;
                pq1 = (int*)(sum->tt.data + equ_rect.y*sum->step)
                            + equ_rect.x + equ_rect.width;
                pq2 = (int*)(sum->tt.data + (equ_rect.y + equ_rect.height)*sum->step) + equ_rect.x;
                pq3 = (int*)(sum->tt.data + (equ_rect.y + equ_rect.height)*sum->step)
                            + equ_rect.x + equ_rect.width;
            }

            cascade->hid_cascade->count = split_stage;

            for(pass = 0; pass < npass; pass++)
            {
#ifdef _OPENMP
    #pragma omp parallel for num_threads(max_threads), schedule(dynamic)
#endif
                for(int _iy = 0; _iy < stop_height; _iy++)
                {
                    int iy = cRound(_iy*ystep);
                    int _ix, _xstep = 1;
                    int stop_width = cRound((img->cols - win_size.width) / ystep);
                    uchar* mask_row = temp->tt.data + temp->step * iy;

                    for(_ix = 0; _ix < stop_width; _ix += _xstep)
                    {
                        int ix = cRound(_ix*ystep); // it really should be ystep
                    
                        if(pass == 0)
                        {
                            int result;
                            _xstep = 2;

                            if(do_canny_pruning)
                            {
                                int offset;
                                int s, sq;
                        
                                offset = iy*(sum->step/sizeof(p0[0])) + ix;
                                s = p0[offset] - p1[offset] - p2[offset] + p3[offset];
                                sq = pq0[offset] - pq1[offset] - pq2[offset] + pq3[offset];
                                if(s < 100 || sq < 20)
                                    continue;
                            }

                            result = cvRunHaarClassifierCascade(cascade, cPoint(ix,iy), 0);
                            if(result > 0)
                            {
                                if(pass < npass - 1)
                                    mask_row[ix] = 1;
                                else
                                {
                                    CRect rect = cRect(ix,iy,win_size.width,win_size.height);
#ifndef _OPENMP
                                    cvSeqPush(seq, &rect);
#else
                                    cvSeqPush(seq_thread[omp_get_thread_num()], &rect);
#endif
                                }
                            }
                            if(result < 0)
                                _xstep = 1;
                        }
                        else if(mask_row[ix])
                        {
                            int result = cvRunHaarClassifierCascade(cascade, cPoint(ix,iy),
                                                                     stage_offset);
                            if(result > 0)
                            {
                                if(pass == npass - 1)
                                {
                                    CRect rect = cRect(ix,iy,win_size.width,win_size.height);
#ifndef _OPENMP
                                    cvSeqPush(seq, &rect);
#else
                                    cvSeqPush(seq_thread[omp_get_thread_num()], &rect);
#endif
                                }
                            }
                            else
                                mask_row[ix] = 0;
                        }
                    }
                }
                stage_offset = cascade->hid_cascade->count;
                cascade->hid_cascade->count = cascade->count;
            }
        }
    }

#ifdef _OPENMP
	// gather the results
	for(i = 0; i < max_threads; i++)
	{
		CvSeq* s = seq_thread[i];
        int j, total = s->total;
        CvSeqBlock* b = s->first;
        for(j = 0; j < total; j += b->count, b = b->next)
            cvSeqPushMulti(seq, b->data, b->count);
	}
#endif

    if(min_neighbors != 0)
    {
        // group retrieved rectangles in order to filter out noise 
        int ncomp = cvSeqPartition(seq, 0, &idx_seq, is_equal, 0);
        CC_CALL(comps = (CvAvgComp*)cAlloc((ncomp+1)*sizeof(comps[0])));
        memset(comps, 0, (ncomp+1)*sizeof(comps[0]));

        // count number of neighbors
        for(i = 0; i < seq->total; i++)
        {
            CRect r1 = *(CRect*)cvGetSeqElem(seq, i);
            int idx = *(int*)cvGetSeqElem(idx_seq, i);
            assert((unsigned)idx < (unsigned)ncomp);

            comps[idx].neighbors++;
             
            comps[idx].rect.x += r1.x;
            comps[idx].rect.y += r1.y;
            comps[idx].rect.width += r1.width;
            comps[idx].rect.height += r1.height;
        }

        // calculate average bounding box
        for(i = 0; i < ncomp; i++)
        {
            int n = comps[i].neighbors;
            if(n >= min_neighbors)
            {
                CvAvgComp comp;
                comp.rect.x = (comps[i].rect.x*2 + n)/(2*n);
                comp.rect.y = (comps[i].rect.y*2 + n)/(2*n);
                comp.rect.width = (comps[i].rect.width*2 + n)/(2*n);
                comp.rect.height = (comps[i].rect.height*2 + n)/(2*n);
                comp.neighbors = comps[i].neighbors;

                cvSeqPush(seq2, &comp);
            }
        }

        // filter out small face rectangles inside large face rectangles
        for(i = 0; i < seq2->total; i++)
        {
            CvAvgComp r1 = *(CvAvgComp*)cvGetSeqElem(seq2, i);
            int j, flag = 1;

            for(j = 0; j < seq2->total; j++)
            {
                CvAvgComp r2 = *(CvAvgComp*)cvGetSeqElem(seq2, j);
                int distance = cRound(r2.rect.width * 0.2);
            
                if(i != j &&
                    r1.rect.x >= r2.rect.x - distance &&
                    r1.rect.y >= r2.rect.y - distance &&
                    r1.rect.x + r1.rect.width <= r2.rect.x + r2.rect.width + distance &&
                    r1.rect.y + r1.rect.height <= r2.rect.y + r2.rect.height + distance &&
                    (r2.neighbors > MAX(3, r1.neighbors) || r1.neighbors < 3))
                {
                    flag = 0;
                    break;
                }
            }

            if(flag)
            {
                cvSeqPush(result_seq, &r1);
                /* cvSeqPush(result_seq, &r1.rect); */
            }
        }
    }

    __END__;

#ifdef _OPENMP
	for(i = 0; i < max_threads; i++)
	{
		if(seq_thread[i])
            cvReleaseMemStorage(&seq_thread[i]->storage);
	}
#endif

    cvReleaseMemStorage(&temp_storage);
    cvReleaseMat(&sum);
    cvReleaseMat(&sqsum);
    cvReleaseMat(&tilted);
    cvReleaseMat(&temp);
    cvReleaseMat(&sumcanny);
    cvReleaseMat(&norm_img);
    cvReleaseMat(&img_small);
    cFree(&comps);

    return result_seq;
}


static CvHaarClassifierCascade*
icvLoadCascadeCART(const char** input_cascade, int n, CSize orig_window_size)
{
    int i;
    CvHaarClassifierCascade* cascade = icvCreateHaarClassifierCascade(n);
    cascade->orig_window_size = orig_window_size;

    for(i = 0; i < n; i++)
    {
        int j, count, l;
        float threshold = 0;
        const char* stage = input_cascade[i];
        int dl = 0;

        /* tree links */
        int parent = -1;
        int next = -1;

        sscanf(stage, "%d%n", &count, &dl);
        stage += dl;
        
        assert(count > 0);
        cascade->stage_classifier[i].count = count;
        cascade->stage_classifier[i].classifier =
            (CvHaarClassifier*)cAlloc(count*sizeof(cascade->stage_classifier[i].classifier[0]));

        for(j = 0; j < count; j++)
        {
            CvHaarClassifier* classifier = cascade->stage_classifier[i].classifier + j;
            int k, rects = 0;
            char str[100];
            
            sscanf(stage, "%d%n", &classifier->count, &dl);
            stage += dl;

            classifier->haar_feature = (CvHaarFeature*) cAlloc(
                classifier->count * (sizeof(*classifier->haar_feature) +
                                      sizeof(*classifier->threshold) +
                                      sizeof(*classifier->left) +
                                      sizeof(*classifier->right)) +
                (classifier->count + 1) * sizeof(*classifier->alpha));
            classifier->threshold = (float*) (classifier->haar_feature+classifier->count);
            classifier->left = (int*) (classifier->threshold + classifier->count);
            classifier->right = (int*) (classifier->left + classifier->count);
            classifier->alpha = (float*) (classifier->right + classifier->count);
            
            for(l = 0; l < classifier->count; l++)
            {
                sscanf(stage, "%d%n", &rects, &dl);
                stage += dl;

                assert(rects >= 2 && rects <= CC_HAAR_FEATURE_MAX);

                for(k = 0; k < rects; k++)
                {
                    CRect r;
                    int band = 0;
                    sscanf(stage, "%d%d%d%d%d%f%n",
                            &r.x, &r.y, &r.width, &r.height, &band,
                            &(classifier->haar_feature[l].rect[k].weight), &dl);
                    stage += dl;
                    classifier->haar_feature[l].rect[k].r = r;
                }
                sscanf(stage, "%s%n", str, &dl);
                stage += dl;
            
                classifier->haar_feature[l].tilted = strncmp(str, "tilted", 6) == 0;
            
                for(k = rects; k < CC_HAAR_FEATURE_MAX; k++)
                {
                    memset(classifier->haar_feature[l].rect + k, 0,
                            sizeof(classifier->haar_feature[l].rect[k]));
                }
                
                sscanf(stage, "%f%d%d%n", &(classifier->threshold[l]), 
                                       &(classifier->left[l]),
                                       &(classifier->right[l]), &dl);
                stage += dl;
            }
            for(l = 0; l <= classifier->count; l++)
            {
                sscanf(stage, "%f%n", &(classifier->alpha[l]), &dl);
                stage += dl;
            }
        }
       
        sscanf(stage, "%f%n", &threshold, &dl);
        stage += dl;

        cascade->stage_classifier[i].threshold = threshold;

        /* load tree links */
        if(sscanf(stage, "%d%d%n", &parent, &next, &dl) != 2)
        {
            parent = i - 1;
            next = -1;
        }
        stage += dl;

        cascade->stage_classifier[i].parent = parent;
        cascade->stage_classifier[i].next = next;
        cascade->stage_classifier[i].child = -1;

        if(parent != -1 && cascade->stage_classifier[parent].child == -1)
        {
            cascade->stage_classifier[parent].child = i;
        }
    }

    return cascade;
}

#ifndef _MAX_PATH
#define _MAX_PATH 1024
#endif

CC_IMPL CvHaarClassifierCascade*
cvLoadHaarClassifierCascade(const char* directory, CSize orig_window_size)
{
    const char** input_cascade = 0; 
    CvHaarClassifierCascade *cascade = 0;

    CC_FUNCNAME("cvLoadHaarClassifierCascade");

    __BEGIN__;

    int i, n;
    const char* slash;
    char name[_MAX_PATH];
    int size = 0;
    char* ptr = 0;

    if(!directory)
        CC_ERROR(CC_StsNullPtr, "Null path is passed");

    n = (int)strlen(directory)-1;
    slash = directory[n] == '\\' || directory[n] == '/' ? "" : "/";

    /* try to read the classifier from directory */
    for(n = 0; ; n++)
    {
        sprintf(name, "%s%s%d/AdaBoostCARTHaarClassifier.txt", directory, slash, n);
        FILE* f = fopen(name, "rb");
        if(!f)
            break;
        fseek(f, 0, SEEK_END);
        size += ftell(f) + 1;
        fclose(f);
    }

    if(n == 0 && slash[0])
    {
        CC_CALL(cascade = (CvHaarClassifierCascade*)cvLoad(directory));
        EXIT;
    }
    else if(n == 0)
        CC_ERROR(CC_StsBadArg, "Invalid path");
    
    size += (n+1)*sizeof(char*);
    CC_CALL(input_cascade = (const char**)cAlloc(size));
    ptr = (char*)(input_cascade + n + 1);

    for(i = 0; i < n; i++)
    {
        sprintf(name, "%s/%d/AdaBoostCARTHaarClassifier.txt", directory, i);
        FILE* f = fopen(name, "rb");
        if(!f)
            CC_ERROR(CC_StsError, "");
        fseek(f, 0, SEEK_END);
        size = ftell(f);
        fseek(f, 0, SEEK_SET);
        fread(ptr, 1, size, f);
        fclose(f);
        input_cascade[i] = ptr;
        ptr += size;
        *ptr++ = '\0';
    }

    input_cascade[n] = 0;
    cascade = icvLoadCascadeCART(input_cascade, n, orig_window_size);

    __END__;

    if(input_cascade)
        cFree(&input_cascade);

    if(cvGetErrStatus() < 0)
        cvReleaseHaarClassifierCascade(&cascade);

    return cascade;
}


CC_IMPL void
cvReleaseHaarClassifierCascade(CvHaarClassifierCascade** _cascade)
{
    if(_cascade && *_cascade)
    {
        int i, j;
        CvHaarClassifierCascade* cascade = *_cascade;

        for(i = 0; i < cascade->count; i++)
        {
            for(j = 0; j < cascade->stage_classifier[i].count; j++)
                cFree(&cascade->stage_classifier[i].classifier[j].haar_feature);
            cFree(&cascade->stage_classifier[i].classifier);
        }
        icvReleaseHidHaarClassifierCascade(&cascade->hid_cascade);
        cFree(_cascade);
    }
}


/****************************************************************************************\
*                                  Persistence functions                                 *
\****************************************************************************************/

/* field names */

#define ICC_HAAR_SIZE_NAME            "size"
#define ICC_HAAR_STAGES_NAME          "stages"
#define ICC_HAAR_TREES_NAME             "trees"
#define ICC_HAAR_FEATURE_NAME             "feature"
#define ICC_HAAR_RECTS_NAME                 "rects"
#define ICC_HAAR_TILTED_NAME                "tilted"
#define ICC_HAAR_THRESHOLD_NAME           "threshold"
#define ICC_HAAR_LEFT_NODE_NAME           "left_node"
#define ICC_HAAR_LEFT_VAL_NAME            "left_val"
#define ICC_HAAR_RIGHT_NODE_NAME          "right_node"
#define ICC_HAAR_RIGHT_VAL_NAME           "right_val"
#define ICC_HAAR_STAGE_THRESHOLD_NAME   "stage_threshold"
#define ICC_HAAR_PARENT_NAME            "parent"
#define ICC_HAAR_NEXT_NAME              "next"

static int
icvIsHaarClassifier(const void* struct_ptr)
{
    return CC_IS_HAAR_CLASSIFIER(struct_ptr);
}

static void*
icvReadHaarClassifier(CvFileStorage* fs, CvFileNode* node)
{
    CvHaarClassifierCascade* cascade = NULL;

    CC_FUNCNAME("cvReadHaarClassifier");

    __BEGIN__;

    char buf[256];
    CvFileNode* seq_fn = NULL; /* sequence */
    CvFileNode* fn = NULL;
    CvFileNode* stages_fn = NULL;
    CvSeqReader stages_reader;
    int n;
    int i, j, k, l;
    int parent, next;

    CC_CALL(stages_fn = cvGetFileNodeByName(fs, node, ICC_HAAR_STAGES_NAME));
    if(!stages_fn || !CC_NODE_IS_SEQ(stages_fn->tag))
        CC_ERROR(CC_StsError, "Invalid stages node");

    n = stages_fn->tt.seq->total;
    CC_CALL(cascade = icvCreateHaarClassifierCascade(n));

    /* read size */
    CC_CALL(seq_fn = cvGetFileNodeByName(fs, node, ICC_HAAR_SIZE_NAME));
    if(!seq_fn || !CC_NODE_IS_SEQ(seq_fn->tag) || seq_fn->tt.seq->total != 2)
        CC_ERROR(CC_StsError, "size node is not a valid sequence.");
    CC_CALL(fn = (CvFileNode*) cvGetSeqElem(seq_fn->tt.seq, 0));
    if(!CC_NODE_IS_INT(fn->tag) || fn->tt.i <= 0)
        CC_ERROR(CC_StsError, "Invalid size node: width must be positive integer");
    cascade->orig_window_size.width = fn->tt.i;
    CC_CALL(fn = (CvFileNode*) cvGetSeqElem(seq_fn->tt.seq, 1));
    if(!CC_NODE_IS_INT(fn->tag) || fn->tt.i <= 0)
        CC_ERROR(CC_StsError, "Invalid size node: height must be positive integer");
    cascade->orig_window_size.height = fn->tt.i;

    CC_CALL(cvStartReadSeq(stages_fn->tt.seq, &stages_reader));
    for(i = 0; i < n; ++i)
    {
        CvFileNode* stage_fn;
        CvFileNode* trees_fn;
        CvSeqReader trees_reader;

        stage_fn = (CvFileNode*) stages_reader.ptr;
        if(!CC_NODE_IS_MAP(stage_fn->tag))
        {
            sprintf(buf, "Invalid stage %d", i);
            CC_ERROR(CC_StsError, buf);
        }

        CC_CALL(trees_fn = cvGetFileNodeByName(fs, stage_fn, ICC_HAAR_TREES_NAME));
        if(!trees_fn || !CC_NODE_IS_SEQ(trees_fn->tag)
            || trees_fn->tt.seq->total <= 0)
        {
            sprintf(buf, "Trees node is not a valid sequence. (stage %d)", i);
            CC_ERROR(CC_StsError, buf);
        }

        CC_CALL(cascade->stage_classifier[i].classifier =
            (CvHaarClassifier*) cAlloc(trees_fn->tt.seq->total
                * sizeof(cascade->stage_classifier[i].classifier[0])));
        for(j = 0; j < trees_fn->tt.seq->total; ++j)
        {
            cascade->stage_classifier[i].classifier[j].haar_feature = NULL;
        }
        cascade->stage_classifier[i].count = trees_fn->tt.seq->total;

        CC_CALL(cvStartReadSeq(trees_fn->tt.seq, &trees_reader));
        for(j = 0; j < trees_fn->tt.seq->total; ++j)
        {
            CvFileNode* tree_fn;
            CvSeqReader tree_reader;
            CvHaarClassifier* classifier;
            int last_idx;

            classifier = &cascade->stage_classifier[i].classifier[j];
            tree_fn = (CvFileNode*) trees_reader.ptr;
            if(!CC_NODE_IS_SEQ(tree_fn->tag) || tree_fn->tt.seq->total <= 0)
            {
                sprintf(buf, "Tree node is not a valid sequence."
                         " (stage %d, tree %d)", i, j);
                CC_ERROR(CC_StsError, buf);
            }

            classifier->count = tree_fn->tt.seq->total;
            CC_CALL(classifier->haar_feature = (CvHaarFeature*) cAlloc(
                classifier->count * (sizeof(*classifier->haar_feature) +
                                      sizeof(*classifier->threshold) +
                                      sizeof(*classifier->left) +
                                      sizeof(*classifier->right)) +
                (classifier->count + 1) * sizeof(*classifier->alpha)));
            classifier->threshold = (float*) (classifier->haar_feature+classifier->count);
            classifier->left = (int*) (classifier->threshold + classifier->count);
            classifier->right = (int*) (classifier->left + classifier->count);
            classifier->alpha = (float*) (classifier->right + classifier->count);

            CC_CALL(cvStartReadSeq(tree_fn->tt.seq, &tree_reader));
            for(k = 0, last_idx = 0; k < tree_fn->tt.seq->total; ++k)
            {
                CvFileNode* node_fn;
                CvFileNode* feature_fn;
                CvFileNode* rects_fn;
                CvSeqReader rects_reader;

                node_fn = (CvFileNode*) tree_reader.ptr;
                if(!CC_NODE_IS_MAP(node_fn->tag))
                {
                    sprintf(buf, "Tree node %d is not a valid map. (stage %d, tree %d)",
                             k, i, j);
                    CC_ERROR(CC_StsError, buf);
                }
                CC_CALL(feature_fn = cvGetFileNodeByName(fs, node_fn,
                    ICC_HAAR_FEATURE_NAME));
                if(!feature_fn || !CC_NODE_IS_MAP(feature_fn->tag))
                {
                    sprintf(buf, "Feature node is not a valid map. "
                             "(stage %d, tree %d, node %d)", i, j, k);
                    CC_ERROR(CC_StsError, buf);
                }
                CC_CALL(rects_fn = cvGetFileNodeByName(fs, feature_fn,
                    ICC_HAAR_RECTS_NAME));
                if(!rects_fn || !CC_NODE_IS_SEQ(rects_fn->tag)
                    || rects_fn->tt.seq->total < 1
                    || rects_fn->tt.seq->total > CC_HAAR_FEATURE_MAX)
                {
                    sprintf(buf, "Rects node is not a valid sequence. "
                             "(stage %d, tree %d, node %d)", i, j, k);
                    CC_ERROR(CC_StsError, buf);
                }
                CC_CALL(cvStartReadSeq(rects_fn->tt.seq, &rects_reader));
                for(l = 0; l < rects_fn->tt.seq->total; ++l)
                {
                    CvFileNode* rect_fn;
                    CRect r;

                    rect_fn = (CvFileNode*) rects_reader.ptr;
                    if(!CC_NODE_IS_SEQ(rect_fn->tag) || rect_fn->tt.seq->total != 5)
                    {
                        sprintf(buf, "Rect %d is not a valid sequence. "
                                 "(stage %d, tree %d, node %d)", l, i, j, k);
                        CC_ERROR(CC_StsError, buf);
                    }
                    
                    fn = CC_SEQ_ELEM(rect_fn->tt.seq, CvFileNode, 0);
                    if(!CC_NODE_IS_INT(fn->tag) || fn->tt.i < 0)
                    {
                        sprintf(buf, "x coordinate must be non-negative integer. "
                                 "(stage %d, tree %d, node %d, rect %d)", i, j, k, l);
                        CC_ERROR(CC_StsError, buf);
                    }
                    r.x = fn->tt.i;
                    fn = CC_SEQ_ELEM(rect_fn->tt.seq, CvFileNode, 1);
                    if(!CC_NODE_IS_INT(fn->tag) || fn->tt.i < 0)
                    {
                        sprintf(buf, "y coordinate must be non-negative integer. "
                                 "(stage %d, tree %d, node %d, rect %d)", i, j, k, l);
                        CC_ERROR(CC_StsError, buf);
                    }
                    r.y = fn->tt.i;
                    fn = CC_SEQ_ELEM(rect_fn->tt.seq, CvFileNode, 2);
                    if(!CC_NODE_IS_INT(fn->tag) || fn->tt.i <= 0
                        || r.x + fn->tt.i > cascade->orig_window_size.width)
                    {
                        sprintf(buf, "width must be positive integer and "
                                 "(x + width) must not exceed window width. "
                                 "(stage %d, tree %d, node %d, rect %d)", i, j, k, l);
                        CC_ERROR(CC_StsError, buf);
                    }
                    r.width = fn->tt.i;
                    fn = CC_SEQ_ELEM(rect_fn->tt.seq, CvFileNode, 3);
                    if(!CC_NODE_IS_INT(fn->tag) || fn->tt.i <= 0
                        || r.y + fn->tt.i > cascade->orig_window_size.height)
                    {
                        sprintf(buf, "height must be positive integer and "
                                 "(y + height) must not exceed window height. "
                                 "(stage %d, tree %d, node %d, rect %d)", i, j, k, l);
                        CC_ERROR(CC_StsError, buf);
                    }
                    r.height = fn->tt.i;
                    fn = CC_SEQ_ELEM(rect_fn->tt.seq, CvFileNode, 4);
                    if(!CC_NODE_IS_REAL(fn->tag))
                    {
                        sprintf(buf, "weight must be real number. "
                                 "(stage %d, tree %d, node %d, rect %d)", i, j, k, l);
                        CC_ERROR(CC_StsError, buf);
                    }

                    classifier->haar_feature[k].rect[l].weight = (float) fn->tt.f;
                    classifier->haar_feature[k].rect[l].r = r;

                    CC_NEXT_SEQ_ELEM(sizeof(*rect_fn), rects_reader);
                } /* for each rect */
                for(l = rects_fn->tt.seq->total; l < CC_HAAR_FEATURE_MAX; ++l)
                {
                    classifier->haar_feature[k].rect[l].weight = 0;
                    classifier->haar_feature[k].rect[l].r = cRect(0, 0, 0, 0);
                }

                CC_CALL(fn = cvGetFileNodeByName(fs, feature_fn, ICC_HAAR_TILTED_NAME));
                if(!fn || !CC_NODE_IS_INT(fn->tag))
                {
                    sprintf(buf, "tilted must be 0 or 1. "
                             "(stage %d, tree %d, node %d)", i, j, k);
                    CC_ERROR(CC_StsError, buf);
                }
                classifier->haar_feature[k].tilted = (fn->tt.i != 0);
                CC_CALL(fn = cvGetFileNodeByName(fs, node_fn, ICC_HAAR_THRESHOLD_NAME));
                if(!fn || !CC_NODE_IS_REAL(fn->tag))
                {
                    sprintf(buf, "threshold must be real number. "
                             "(stage %d, tree %d, node %d)", i, j, k);
                    CC_ERROR(CC_StsError, buf);
                }
                classifier->threshold[k] = (float) fn->tt.f;
                CC_CALL(fn = cvGetFileNodeByName(fs, node_fn, ICC_HAAR_LEFT_NODE_NAME));
                if(fn)
                {
                    if(!CC_NODE_IS_INT(fn->tag) || fn->tt.i <= k
                        || fn->tt.i >= tree_fn->tt.seq->total)
                    {
                        sprintf(buf, "left node must be valid node number. "
                                 "(stage %d, tree %d, node %d)", i, j, k);
                        CC_ERROR(CC_StsError, buf);
                    }
                    /* left node */
                    classifier->left[k] = fn->tt.i;
                }
                else
                {
                    CC_CALL(fn = cvGetFileNodeByName(fs, node_fn,
                        ICC_HAAR_LEFT_VAL_NAME));
                    if(!fn)
                    {
                        sprintf(buf, "left node or left value must be specified. "
                                 "(stage %d, tree %d, node %d)", i, j, k);
                        CC_ERROR(CC_StsError, buf);
                    }
                    if(!CC_NODE_IS_REAL(fn->tag))
                    {
                        sprintf(buf, "left value must be real number. "
                                 "(stage %d, tree %d, node %d)", i, j, k);
                        CC_ERROR(CC_StsError, buf);
                    }
                    /* left value */
                    if(last_idx >= classifier->count + 1)
                    {
                        sprintf(buf, "Tree structure is broken: too many values. "
                                 "(stage %d, tree %d, node %d)", i, j, k);
                        CC_ERROR(CC_StsError, buf);
                    }
                    classifier->left[k] = -last_idx;
                    classifier->alpha[last_idx++] = (float) fn->tt.f;
                }
                CC_CALL(fn = cvGetFileNodeByName(fs, node_fn,ICC_HAAR_RIGHT_NODE_NAME));
                if(fn)
                {
                    if(!CC_NODE_IS_INT(fn->tag) || fn->tt.i <= k
                        || fn->tt.i >= tree_fn->tt.seq->total)
                    {
                        sprintf(buf, "right node must be valid node number. "
                                 "(stage %d, tree %d, node %d)", i, j, k);
                        CC_ERROR(CC_StsError, buf);
                    }
                    /* right node */
                    classifier->right[k] = fn->tt.i;
                }
                else
                {
                    CC_CALL(fn = cvGetFileNodeByName(fs, node_fn,
                        ICC_HAAR_RIGHT_VAL_NAME));
                    if(!fn)
                    {
                        sprintf(buf, "right node or right value must be specified. "
                                 "(stage %d, tree %d, node %d)", i, j, k);
                        CC_ERROR(CC_StsError, buf);
                    }
                    if(!CC_NODE_IS_REAL(fn->tag))
                    {
                        sprintf(buf, "right value must be real number. "
                                 "(stage %d, tree %d, node %d)", i, j, k);
                        CC_ERROR(CC_StsError, buf);
                    }
                    /* right value */
                    if(last_idx >= classifier->count + 1)
                    {
                        sprintf(buf, "Tree structure is broken: too many values. "
                                 "(stage %d, tree %d, node %d)", i, j, k);
                        CC_ERROR(CC_StsError, buf);
                    }
                    classifier->right[k] = -last_idx;
                    classifier->alpha[last_idx++] = (float) fn->tt.f;
                }

                CC_NEXT_SEQ_ELEM(sizeof(*node_fn), tree_reader);
            } /* for each node */
            if(last_idx != classifier->count + 1)
            {
                sprintf(buf, "Tree structure is broken: too few values. "
                         "(stage %d, tree %d)", i, j);
                CC_ERROR(CC_StsError, buf);
            }

            CC_NEXT_SEQ_ELEM(sizeof(*tree_fn), trees_reader);
        } /* for each tree */

        CC_CALL(fn = cvGetFileNodeByName(fs, stage_fn, ICC_HAAR_STAGE_THRESHOLD_NAME));
        if(!fn || !CC_NODE_IS_REAL(fn->tag))
        {
            sprintf(buf, "stage threshold must be real number. (stage %d)", i);
            CC_ERROR(CC_StsError, buf);
        }
        cascade->stage_classifier[i].threshold = (float) fn->tt.f;

        parent = i - 1;
        next = -1;

        CC_CALL(fn = cvGetFileNodeByName(fs, stage_fn, ICC_HAAR_PARENT_NAME));
        if(!fn || !CC_NODE_IS_INT(fn->tag)
            || fn->tt.i < -1 || fn->tt.i >= cascade->count)
        {
            sprintf(buf, "parent must be integer number. (stage %d)", i);
            CC_ERROR(CC_StsError, buf);
        }
        parent = fn->tt.i;
        CC_CALL(fn = cvGetFileNodeByName(fs, stage_fn, ICC_HAAR_NEXT_NAME));
        if(!fn || !CC_NODE_IS_INT(fn->tag)
            || fn->tt.i < -1 || fn->tt.i >= cascade->count)
        {
            sprintf(buf, "next must be integer number. (stage %d)", i);
            CC_ERROR(CC_StsError, buf);
        }
        next = fn->tt.i;

        cascade->stage_classifier[i].parent = parent;
        cascade->stage_classifier[i].next = next;
        cascade->stage_classifier[i].child = -1;

        if(parent != -1 && cascade->stage_classifier[parent].child == -1)
        {
            cascade->stage_classifier[parent].child = i;
        }
        
        CC_NEXT_SEQ_ELEM(sizeof(*stage_fn), stages_reader);
    } /* for each stage */

    __END__;

    if(cvGetErrStatus() < 0)
    {
        cvReleaseHaarClassifierCascade(&cascade);
        cascade = NULL;
    }

    return cascade;
}

static void
icvWriteHaarClassifier(CvFileStorage* fs, const char* name, const void* struct_ptr,
                        CvAttrList attributes)
{
    CC_FUNCNAME("cvWriteHaarClassifier");

    __BEGIN__;

    int i, j, k, l;
    char buf[256];
    const CvHaarClassifierCascade* cascade = (const CvHaarClassifierCascade*) struct_ptr;

    /* TODO: parameters check */

    CC_CALL(cvWriteStructBegin(fs, name, CC_NODE_MAP, CC_TYPE_NAME_HAAR, attributes));
    
    CC_CALL(cvWriteStructBegin(fs, ICC_HAAR_SIZE_NAME, CC_NODE_SEQ | CC_NODE_FLOW));
    CC_CALL(cvWriteInt(fs, NULL, cascade->orig_window_size.width));
    CC_CALL(cvWriteInt(fs, NULL, cascade->orig_window_size.height));
    CC_CALL(cvWriteStructEnd(fs)); /* size */
    
    CC_CALL(cvWriteStructBegin(fs, ICC_HAAR_STAGES_NAME, CC_NODE_SEQ));    
    for(i = 0; i < cascade->count; ++i)
    {
        CC_CALL(cvWriteStructBegin(fs, NULL, CC_NODE_MAP));
        sprintf(buf, "stage %d", i);
        CC_CALL(cvWriteComment(fs, buf, 1));
        
        CC_CALL(cvWriteStructBegin(fs, ICC_HAAR_TREES_NAME, CC_NODE_SEQ));

        for(j = 0; j < cascade->stage_classifier[i].count; ++j)
        {
            CvHaarClassifier* tree = &cascade->stage_classifier[i].classifier[j];

            CC_CALL(cvWriteStructBegin(fs, NULL, CC_NODE_SEQ));
            sprintf(buf, "tree %d", j);
            CC_CALL(cvWriteComment(fs, buf, 1));

            for(k = 0; k < tree->count; ++k)
            {
                CvHaarFeature* feature = &tree->haar_feature[k];

                CC_CALL(cvWriteStructBegin(fs, NULL, CC_NODE_MAP));
                if(k)
                {
                    sprintf(buf, "node %d", k);
                }
                else
                {
                    sprintf(buf, "root node");
                }
                CC_CALL(cvWriteComment(fs, buf, 1));

                CC_CALL(cvWriteStructBegin(fs, ICC_HAAR_FEATURE_NAME, CC_NODE_MAP));
                
                CC_CALL(cvWriteStructBegin(fs, ICC_HAAR_RECTS_NAME, CC_NODE_SEQ));
                for(l = 0; l < CC_HAAR_FEATURE_MAX && feature->rect[l].r.width != 0; ++l)
                {
                    CC_CALL(cvWriteStructBegin(fs, NULL, CC_NODE_SEQ | CC_NODE_FLOW));
                    CC_CALL(cvWriteInt(fs, NULL, feature->rect[l].r.x));
                    CC_CALL(cvWriteInt(fs, NULL, feature->rect[l].r.y));
                    CC_CALL(cvWriteInt(fs, NULL, feature->rect[l].r.width));
                    CC_CALL(cvWriteInt(fs, NULL, feature->rect[l].r.height));
                    CC_CALL(cvWriteReal(fs, NULL, feature->rect[l].weight));
                    CC_CALL(cvWriteStructEnd(fs)); /* rect */
                }
                CC_CALL(cvWriteStructEnd(fs)); /* rects */
                CC_CALL(cvWriteInt(fs, ICC_HAAR_TILTED_NAME, feature->tilted));
                CC_CALL(cvWriteStructEnd(fs)); /* feature */
                
                CC_CALL(cvWriteReal(fs, ICC_HAAR_THRESHOLD_NAME, tree->threshold[k]));

                if(tree->left[k] > 0)
                {
                    CC_CALL(cvWriteInt(fs, ICC_HAAR_LEFT_NODE_NAME, tree->left[k]));
                }
                else
                {
                    CC_CALL(cvWriteReal(fs, ICC_HAAR_LEFT_VAL_NAME,
                        tree->alpha[-tree->left[k]]));
                }

                if(tree->right[k] > 0)
                {
                    CC_CALL(cvWriteInt(fs, ICC_HAAR_RIGHT_NODE_NAME, tree->right[k]));
                }
                else
                {
                    CC_CALL(cvWriteReal(fs, ICC_HAAR_RIGHT_VAL_NAME,
                        tree->alpha[-tree->right[k]]));
                }

                CC_CALL(cvWriteStructEnd(fs)); /* split */
            }

            CC_CALL(cvWriteStructEnd(fs)); /* tree */
        }

        CC_CALL(cvWriteStructEnd(fs)); /* trees */

        CC_CALL(cvWriteReal(fs, ICC_HAAR_STAGE_THRESHOLD_NAME,
                              cascade->stage_classifier[i].threshold));

        CC_CALL(cvWriteInt(fs, ICC_HAAR_PARENT_NAME,
                              cascade->stage_classifier[i].parent));
        CC_CALL(cvWriteInt(fs, ICC_HAAR_NEXT_NAME,
                              cascade->stage_classifier[i].next));

        CC_CALL(cvWriteStructEnd(fs)); /* stage */
    } /* for each stage */
    
    CC_CALL(cvWriteStructEnd(fs)); /* stages */
    CC_CALL(cvWriteStructEnd(fs)); /* root */

    __END__;
}

static void*
icvCloneHaarClassifier(const void* struct_ptr)
{
    CvHaarClassifierCascade* cascade = NULL;

    CC_FUNCNAME("cvCloneHaarClassifier");

    __BEGIN__;

    int i, j, k, n;
    const CvHaarClassifierCascade* cascade_src =
        (const CvHaarClassifierCascade*) struct_ptr;

    n = cascade_src->count;
    CC_CALL(cascade = icvCreateHaarClassifierCascade(n));
    cascade->orig_window_size = cascade_src->orig_window_size;

    for(i = 0; i < n; ++i)
    {
        cascade->stage_classifier[i].parent = cascade_src->stage_classifier[i].parent;
        cascade->stage_classifier[i].next = cascade_src->stage_classifier[i].next;
        cascade->stage_classifier[i].child = cascade_src->stage_classifier[i].child;
        cascade->stage_classifier[i].threshold = cascade_src->stage_classifier[i].threshold;

        cascade->stage_classifier[i].count = 0;
        CC_CALL(cascade->stage_classifier[i].classifier =
            (CvHaarClassifier*) cAlloc(cascade_src->stage_classifier[i].count
                * sizeof(cascade->stage_classifier[i].classifier[0])));
        
        cascade->stage_classifier[i].count = cascade_src->stage_classifier[i].count;

        for(j = 0; j < cascade->stage_classifier[i].count; ++j)
        {
            cascade->stage_classifier[i].classifier[j].haar_feature = NULL;
        }

        for(j = 0; j < cascade->stage_classifier[i].count; ++j)
        {
            const CvHaarClassifier* classifier_src = 
                &cascade_src->stage_classifier[i].classifier[j];
            CvHaarClassifier* classifier = 
                &cascade->stage_classifier[i].classifier[j];

            classifier->count = classifier_src->count;
            CC_CALL(classifier->haar_feature = (CvHaarFeature*) cAlloc(
                classifier->count * (sizeof(*classifier->haar_feature) +
                                      sizeof(*classifier->threshold) +
                                      sizeof(*classifier->left) +
                                      sizeof(*classifier->right)) +
                (classifier->count + 1) * sizeof(*classifier->alpha)));
            classifier->threshold = (float*) (classifier->haar_feature+classifier->count);
            classifier->left = (int*) (classifier->threshold + classifier->count);
            classifier->right = (int*) (classifier->left + classifier->count);
            classifier->alpha = (float*) (classifier->right + classifier->count);
            for(k = 0; k < classifier->count; ++k)
            {
                classifier->haar_feature[k] = classifier_src->haar_feature[k];
                classifier->threshold[k] = classifier_src->threshold[k];
                classifier->left[k] = classifier_src->left[k];
                classifier->right[k] = classifier_src->right[k];
                classifier->alpha[k] = classifier_src->alpha[k];
            }
            classifier->alpha[classifier->count] = 
                classifier_src->alpha[classifier->count];
        }
    }

    __END__;

    return cascade;
}


CvType haar_type(CC_TYPE_NAME_HAAR, icvIsHaarClassifier,
                  (CvReleaseFunc)cvReleaseHaarClassifierCascade,
                  icvReadHaarClassifier, icvWriteHaarClassifier,
                  icvCloneHaarClassifier);


