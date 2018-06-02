
#include "_ml.h"

static inline double
log_ratio(double val)
{
    const double eps = 1e-5;
    
    val = MAX(val, eps);
    val = MIN(val, 1. - eps);
    return log(val/(1. - val));
}


CvBoostParams::CvBoostParams()
{
    boost_type = CvBoost::REAL;
    weak_count = 100;
    weight_trim_rate = 0.95;
    cv_folds = 0;
    max_type = 1;
}


CvBoostParams::CvBoostParams(int _boost_type, int _weak_count,
                                        double _weight_trim_rate, int _max_type,
                                        bool _use_surrogates, const float* _priors)
{
    boost_type = _boost_type;
    weak_count = _weak_count;
    weight_trim_rate = _weight_trim_rate;
    split_criteria = CvBoost::DEFAULT;
    cv_folds = 0;
    max_type = _max_type;
    use_surrogates = _use_surrogates;
    priors = _priors;
}



///////////////////////////////// CvBoostTree ///////////////////////////////////

CvBoostTree::CvBoostTree()
{
    ensemble = 0;
}


CvBoostTree::~CvBoostTree()
{
    clear();
}


void
CvBoostTree::clear()
{
    CvDTree::clear();
    ensemble = 0;
}


bool
CvBoostTree::train(CvDTreeTrainData* _train_data,
                    const img_t* _subsample_idx, CvBoost* _ensemble)
{
    clear();
    ensemble = _ensemble;
    data = _train_data;
    data->shared = true;
    
    return do_train(_subsample_idx);
}


bool
CvBoostTree::train(const img_t*, int, const img_t*, const img_t*,
                    const img_t*, const img_t*, const img_t*, CvDTreeParams)
{
    assert(0);
    return false;
}


bool
CvBoostTree::train(CvDTreeTrainData*, const img_t*)
{
    assert(0);
    return false;
}


void
CvBoostTree::scale(double scale)
{
    CvDTreeNode* node = root;

    // traverse the tree and scale all the node values
    for(;;)
    {
        CvDTreeNode* parent;
        for(;;)
        {
            node->value *= scale;
            if(!node->left)
                break;
            node = node->left;
        }
        
        for(parent = node->parent; parent && parent->right == node;
            node = parent, parent = parent->parent)
            ;

        if(!parent)
            break;

        node = parent->right;
    }
}


void
CvBoostTree::try_split_node(CvDTreeNode* node)
{
    CvDTree::try_split_node(node);

    if(!node->left)
    {
        // if the node has not been split,
        // store the responses for the corresponding training samples
        double* weak_eval = ensemble->get_weak_response()->tt.db;
        int* labels = data->get_labels(node);
        int i, count = node->sample_count;
        double value = node->value;

        for(i = 0; i < count; i++)
            weak_eval[labels[i]] = value;
    }
}


double
CvBoostTree::calc_node_dir(CvDTreeNode* node)
{
    char* dir = (char*)data->direction->tt.data;
    const double* weights = ensemble->get_subtree_weights()->tt.db;
    int i, n = node->sample_count, vi = node->split->var_idx;
    double L, R;

    assert(!node->split->inversed);

    if(data->get_var_type(vi) >= 0) // split on categorical var
    {
        const int* cat_labels = data->get_cat_var_data(node, vi);
        const int* subset = node->split->subset;
        double sum = 0, sum_abs = 0;

        for(i = 0; i < n; i++)
        {
            int idx = cat_labels[i];
            double w = weights[i];
            int d = idx >= 0 ? CC_DTREE_CAT_DIR(idx,subset) : 0;
            sum += d*w; sum_abs += (d & 1)*w;
            dir[i] = (char)d;
        }

        R = (sum_abs + sum) * 0.5;
        L = (sum_abs - sum) * 0.5;
    }
    else // split on ordered var
    {
        const CvPair32s32f* sorted = data->get_ord_var_data(node,vi);
        int split_point = node->split->ord.split_point;
        int n1 = node->get_num_valid(vi);

        assert(0 <= split_point && split_point < n1-1);
        L = R = 0;

        for(i = 0; i <= split_point; i++)
        {
            int idx = sorted[i].i;
            double w = weights[idx];
            dir[idx] = (char)-1;
            L += w;
        }

        for(; i < n1; i++)
        {
            int idx = sorted[i].i;
            double w = weights[idx];
            dir[idx] = (char)1;
            R += w;
        }

        for(; i < n; i++)
            dir[sorted[i].i] = (char)0;
    }

    node->maxlr = MAX(L, R);
    return node->split->quality/(L + R);
}


CvDTreeSplit*
CvBoostTree::find_split_ord_class(CvDTreeNode* node, int vi)
{
    const float epsilon = FLT_EPSILON*2;
    const CvPair32s32f* sorted = data->get_ord_var_data(node, vi);
    const int* responses = data->get_class_labels(node);
    const double* weights = ensemble->get_subtree_weights()->tt.db;
    int n = node->sample_count;
    int n1 = node->get_num_valid(vi);
    const double* rcw0 = weights + n;
    double lcw[2] = {0,0}, rcw[2];
    int i, best_i = -1;
    double best_val = 0;
    int boost_type = ensemble->get_params().boost_type;
    int split_criteria = ensemble->get_params().split_criteria;

    rcw[0] = rcw0[0]; rcw[1] = rcw0[1];
    for(i = n1; i < n; i++)
    {
        int idx = sorted[i].i;
        double w = weights[idx];
        rcw[responses[idx]] -= w;
    }

    if(split_criteria != CvBoost::GINI && split_criteria != CvBoost::MISCLASS)
        split_criteria = boost_type == CvBoost::DISCRETE ? CvBoost::MISCLASS : CvBoost::GINI;

    if(split_criteria == CvBoost::GINI)
    {
        double L = 0, R = rcw[0] + rcw[1];
        double lsum2 = 0, rsum2 = rcw[0]*rcw[0] + rcw[1]*rcw[1];

        for(i = 0; i < n1 - 1; i++)
        {
            int idx = sorted[i].i;
            double w = weights[idx], w2 = w*w;
            double lv, rv;
            idx = responses[idx];
            L += w; R -= w;
            lv = lcw[idx]; rv = rcw[idx];
            lsum2 += 2*lv*w + w2;
            rsum2 -= 2*rv*w - w2;
            lcw[idx] = lv + w; rcw[idx] = rv - w;

            if(sorted[i].val + epsilon < sorted[i+1].val)
            {
                double val = (lsum2*R + rsum2*L)/(L*R);
                if(best_val < val)
                {
                    best_val = val;
                    best_i = i;
                }
            }
        }
    }
    else
    {
        for(i = 0; i < n1 - 1; i++)
        {
            int idx = sorted[i].i;
            double w = weights[idx];
            idx = responses[idx];
            lcw[idx] += w;
            rcw[idx] -= w;

            if(sorted[i].val + epsilon < sorted[i+1].val)
            {
                double val = lcw[0] + rcw[1], val2 = lcw[1] + rcw[0];
                val = MAX(val, val2);
                if(best_val < val)
                {
                    best_val = val;
                    best_i = i;
                }
            }
        }
    }

    return best_i >= 0 ? data->new_split_ord(vi,
        (sorted[best_i].val + sorted[best_i+1].val)*0.5f, best_i,
        0, (float)best_val) : 0;
}


#define CC_CMP_NUM_PTR(a,b) (*(a) < *(b))
static CC_IMPLEMENT_QSORT_EX(icvSortDblPtr, double*, CC_CMP_NUM_PTR, int)

CvDTreeSplit*
CvBoostTree::find_split_cat_class(CvDTreeNode* node, int vi)
{
    CvDTreeSplit* split;
    const int* cat_labels = data->get_cat_var_data(node, vi);
    const int* responses = data->get_class_labels(node);
    int ci = data->get_var_type(vi);
    int n = node->sample_count;
    int mi = data->cat_count->tt.i[ci];
    double lcw[2]={0,0}, rcw[2]={0,0};
    double* cjk = (double*)cvStackAlloc(2*(mi+1)*sizeof(cjk[0]))+2;
    const double* weights = ensemble->get_subtree_weights()->tt.db;
    double** dbl_ptr = (double**)cvStackAlloc(mi*sizeof(dbl_ptr[0]));
    int i, j, k, idx;
    double L = 0, R;
    double best_val = 0;
    int best_subset = -1, subset_i;
    int boost_type = ensemble->get_params().boost_type;
    int split_criteria = ensemble->get_params().split_criteria;

    // init array of counters:
    // c_{jk} - number of samples that have vi-th input variable = j and response = k.
    for(j = -1; j < mi; j++)
        cjk[j*2] = cjk[j*2+1] = 0;

    for(i = 0; i < n; i++)
    {
        double w = weights[i];
        j = cat_labels[i];
        k = responses[i];
        cjk[j*2 + k] += w;
    }

    for(j = 0; j < mi; j++)
    {
        rcw[0] += cjk[j*2];
        rcw[1] += cjk[j*2+1];
        dbl_ptr[j] = cjk + j*2 + 1;
    }

    R = rcw[0] + rcw[1];

    if(split_criteria != CvBoost::GINI && split_criteria != CvBoost::MISCLASS)
        split_criteria = boost_type == CvBoost::DISCRETE ? CvBoost::MISCLASS : CvBoost::GINI;

    // sort rows of c_jk by increasing c_j,1
    // (i.e. by the weight of samples in j-th category that belong to class 1)
    icvSortDblPtr(dbl_ptr, mi, 0);

    for(subset_i = 0; subset_i < mi-1; subset_i++)
    {
        idx = (int)(dbl_ptr[subset_i] - cjk)/2;
        const double* crow = cjk + idx*2;
        double w0 = crow[0], w1 = crow[1];
        double weight = w0 + w1;

        if(weight < FLT_EPSILON)
            continue;

        lcw[0] += w0; rcw[0] -= w0;
        lcw[1] += w1; rcw[1] -= w1;

        if(split_criteria == CvBoost::GINI)
        {
            double lsum2 = lcw[0]*lcw[0] + lcw[1]*lcw[1];
            double rsum2 = rcw[0]*rcw[0] + rcw[1]*rcw[1];
        
            L += weight;
            R -= weight;

            if(L > FLT_EPSILON && R > FLT_EPSILON)
            {
                double val = (lsum2*R + rsum2*L)/(L*R);
                if(best_val < val)
                {
                    best_val = val;
                    best_subset = subset_i;
                }
            }
        }
        else
        {
            double val = lcw[0] + rcw[1];
            double val2 = lcw[1] + rcw[0];

            val = MAX(val, val2);
            if(best_val < val)
            {
                best_val = val;
                best_subset = subset_i;
            }
        }
    }

    if(best_subset < 0)
        return 0;

    split = data->new_split_cat(vi, (float)best_val);

    for(i = 0; i <= best_subset; i++)
    {
        idx = (int)(dbl_ptr[i] - cjk) >> 1;
        split->subset[idx >> 5] |= 1 << (idx & 31);
    }

    return split;
}


CvDTreeSplit*
CvBoostTree::find_split_ord_reg(CvDTreeNode* node, int vi)
{
    const float epsilon = FLT_EPSILON*2;
    const CvPair32s32f* sorted = data->get_ord_var_data(node, vi);
    const float* responses = data->get_ord_responses(node);
    const double* weights = ensemble->get_subtree_weights()->tt.db;
    int n = node->sample_count;
    int n1 = node->get_num_valid(vi);
    int i, best_i = -1;
    double best_val = 0, lsum = 0, rsum = node->value*n;
    double L = 0, R = weights[n];

    // compensate for missing values
    for(i = n1; i < n; i++)
    {
        int idx = sorted[i].i;
        double w = weights[idx];
        rsum -= responses[idx]*w;
        R -= w;
    }

    // find the optimal split
    for(i = 0; i < n1 - 1; i++)
    {
        int idx = sorted[i].i;
        double w = weights[idx];
        double t = responses[idx]*w;
        L += w; R -= w;
        lsum += t; rsum -= t;

        if(sorted[i].val + epsilon < sorted[i+1].val)
        {
            double val = (lsum*lsum*R + rsum*rsum*L)/(L*R);
            if(best_val < val)
            {
                best_val = val;
                best_i = i;
            }
        }
    }

    return best_i >= 0 ? data->new_split_ord(vi,
        (sorted[best_i].val + sorted[best_i+1].val)*0.5f, best_i,
        0, (float)best_val) : 0;
}


CvDTreeSplit*
CvBoostTree::find_split_cat_reg(CvDTreeNode* node, int vi)
{
    CvDTreeSplit* split;
    const int* cat_labels = data->get_cat_var_data(node, vi);
    const float* responses = data->get_ord_responses(node);
    const double* weights = ensemble->get_subtree_weights()->tt.db;
    int ci = data->get_var_type(vi);
    int n = node->sample_count;
    int mi = data->cat_count->tt.i[ci];
    double* sum = (double*)cvStackAlloc((mi+1)*sizeof(sum[0])) + 1;
    double* counts = (double*)cvStackAlloc((mi+1)*sizeof(counts[0])) + 1;
    double** sum_ptr = (double**)cvStackAlloc(mi*sizeof(sum_ptr[0]));
    double L = 0, R = 0, best_val = 0, lsum = 0, rsum = 0;
    int i, best_subset = -1, subset_i;

    for(i = -1; i < mi; i++)
        sum[i] = counts[i] = 0;

    // calculate sum response and weight of each category of the input var
    for(i = 0; i < n; i++)
    {
        int idx = cat_labels[i];
        double w = weights[i];
        double s = sum[idx] + responses[i]*w;
        double nc = counts[idx] + w;
        sum[idx] = s;
        counts[idx] = nc;
    }

    // calculate average response in each category
    for(i = 0; i < mi; i++)
    {
        R += counts[i];
        rsum += sum[i];
        sum[i] /= counts[i];
        sum_ptr[i] = sum + i;
    }

    icvSortDblPtr(sum_ptr, mi, 0);

    // revert back to unnormalized sums
    // (there should be a very little loss in accuracy)
    for(i = 0; i < mi; i++)
        sum[i] *= counts[i];

    for(subset_i = 0; subset_i < mi-1; subset_i++)
    {
        int idx = (int)(sum_ptr[subset_i] - sum);
        double ni = counts[idx];

        if(ni > FLT_EPSILON)
        {
            double s = sum[idx];
            lsum += s; L += ni;
            rsum -= s; R -= ni;
            
            if(L > FLT_EPSILON && R > FLT_EPSILON)
            {
                double val = (lsum*lsum*R + rsum*rsum*L)/(L*R);
                if(best_val < val)
                {
                    best_val = val;
                    best_subset = subset_i;
                }
            }
        }
    }

    if(best_subset < 0)
        return 0;

    split = data->new_split_cat(vi, (float)best_val);
    for(i = 0; i <= best_subset; i++)
    {
        int idx = (int)(sum_ptr[i] - sum);
        split->subset[idx >> 5] |= 1 << (idx & 31);
    }

    return split;
}


CvDTreeSplit*
CvBoostTree::find_surrogate_split_ord(CvDTreeNode* node, int vi)
{
    const float epsilon = FLT_EPSILON*2;
    const CvPair32s32f* sorted = data->get_ord_var_data(node, vi);
    const double* weights = ensemble->get_subtree_weights()->tt.db;
    const char* dir = (char*)data->direction->tt.data;
    int n1 = node->get_num_valid(vi);
    // LL - number of samples that both the primary and the surrogate splits send to the left
    // LR - ... primary split sends to the left and the surrogate split sends to the right
    // RL - ... primary split sends to the right and the surrogate split sends to the left
    // RR - ... both send to the right
    int i, best_i = -1, best_inversed = 0;
    double best_val; 
    double LL = 0, RL = 0, LR, RR;
    double worst_val = node->maxlr;
    double sum = 0, sum_abs = 0;
    best_val = worst_val;
    
    for(i = 0; i < n1; i++)
    {
        int idx = sorted[i].i;
        double w = weights[idx];
        int d = dir[idx];
        sum += d*w; sum_abs += (d & 1)*w;
    }

    // sum_abs = R + L; sum = R - L
    RR = (sum_abs + sum)*0.5;
    LR = (sum_abs - sum)*0.5;

    // initially all the samples are sent to the right by the surrogate split,
    // LR of them are sent to the left by primary split, and RR - to the right.
    // now iteratively compute LL, LR, RL and RR for every possible surrogate split value.
    for(i = 0; i < n1 - 1; i++)
    {
        int idx = sorted[i].i;
        double w = weights[idx];
        int d = dir[idx];

        if(d < 0)
        {
            LL += w; LR -= w;
            if(LL + RR > best_val && sorted[i].val + epsilon < sorted[i+1].val)
            {
                best_val = LL + RR;
                best_i = i; best_inversed = 0;
            }
        }
        else if(d > 0)
        {
            RL += w; RR -= w;
            if(RL + LR > best_val && sorted[i].val + epsilon < sorted[i+1].val)
            {
                best_val = RL + LR;
                best_i = i; best_inversed = 1;
            }
        }
    }

    return best_i >= 0 && best_val > node->maxlr ? data->new_split_ord(vi,
        (sorted[best_i].val + sorted[best_i+1].val)*0.5f, best_i,
        best_inversed, (float)best_val) : 0;
}


CvDTreeSplit*
CvBoostTree::find_surrogate_split_cat(CvDTreeNode* node, int vi)
{
    const int* cat_labels = data->get_cat_var_data(node, vi);
    const char* dir = (char*)data->direction->tt.data;
    const double* weights = ensemble->get_subtree_weights()->tt.db;
    int n = node->sample_count;
    // LL - number of samples that both the primary and the surrogate splits send to the left
    // LR - ... primary split sends to the left and the surrogate split sends to the right
    // RL - ... primary split sends to the right and the surrogate split sends to the left
    // RR - ... both send to the right
    CvDTreeSplit* split = data->new_split_cat(vi, 0);
    int i, mi = data->cat_count->tt.i[data->get_var_type(vi)];
    double best_val = 0;
    double* lc = (double*)cvStackAlloc((mi+1)*2*sizeof(lc[0])) + 1;
    double* rc = lc + mi + 1;
    
    for(i = -1; i < mi; i++)
        lc[i] = rc[i] = 0;

    // 1. for each category calculate the weight of samples
    // sent to the left (lc) and to the right (rc) by the primary split
    for(i = 0; i < n; i++)
    {
        int idx = cat_labels[i];
        double w = weights[i];
        int d = dir[i];
        double sum = lc[idx] + d*w;
        double sum_abs = rc[idx] + (d & 1)*w;
        lc[idx] = sum; rc[idx] = sum_abs;
    }

    for(i = 0; i < mi; i++)
    {
        double sum = lc[i];
        double sum_abs = rc[i];
        lc[i] = (sum_abs - sum) * 0.5;
        rc[i] = (sum_abs + sum) * 0.5;
    }

    // 2. now form the split.
    // in each category send all the samples to the same direction as majority
    for(i = 0; i < mi; i++)
    {
        double lval = lc[i], rval = rc[i];
        if(lval > rval)
        {
            split->subset[i >> 5] |= 1 << (i & 31);
            best_val += lval;
        }
        else
            best_val += rval;
    }

    split->quality = (float)best_val;
    if(split->quality <= node->maxlr)
        cvSetRemoveByPtr(data->split_heap, split), split = 0;

    return split;
}


void
CvBoostTree::calc_node_value(CvDTreeNode* node)
{
    int i, count = node->sample_count;
    const double* weights = ensemble->get_weights()->tt.db;
    const int* labels = data->get_labels(node);
    double* subtree_weights = ensemble->get_subtree_weights()->tt.db;
    double rcw[2] = {0,0};
    int boost_type = ensemble->get_params().boost_type;
    //const double* priors = data->priors->tt.db;

    if(data->is_classifier)
    {
        const int* responses = data->get_class_labels(node);
        
        for(i = 0; i < count; i++)
        {
            int idx = labels[i];
            double w = weights[idx]/*priors[responses[i]]*/;
            rcw[responses[i]] += w;
            subtree_weights[i] = w;
        }

        node->class_idx = rcw[1] > rcw[0];

        if(boost_type == CvBoost::DISCRETE)
        {
            // ignore cat_map for responses, and use {-1,1},
            // as the whole ensemble response is computes as sign(sum_i(weak_response_i)
            node->value = node->class_idx*2 - 1;
        }
        else
        {
            double p = rcw[1]/(rcw[0] + rcw[1]);
            assert(boost_type == CvBoost::REAL);
            
            // store log-ratio of the probability
            node->value = 0.5*log_ratio(p);
        }
    }
    else
    {
        // in case of regression tree:
        //  * node value is 1/n*sum_i(Y_i), where Y_i is i-th response,
        //    n is the number of samples in the node.
        //  * node risk is the sum of squared errors: sum_i((Y_i - <node_value>)^2)
        double sum = 0, sum2 = 0, iw;
        const float* values = data->get_ord_responses(node);
        
        for(i = 0; i < count; i++)
        {
            int idx = labels[i];
            double w = weights[idx]/*priors[values[i] > 0]*/;
            double t = values[i];
            rcw[0] += w;
            subtree_weights[i] = w;
            sum += t*w;
            sum2 += t*t*w;
        }

        iw = 1./rcw[0];
        node->value = sum*iw;
        node->node_risk = sum2 - (sum*iw)*sum;
        
        // renormalize the risk, as in try_split_node the unweighted formula
        // sqrt(risk)/n is used, rather than sqrt(risk)/sum(weights_i)
        node->node_risk *= count*iw*count*iw;
    }

    // store summary weights
    subtree_weights[count] = rcw[0];
    subtree_weights[count+1] = rcw[1];
}


void CvBoostTree::read(CvFileStorage* fs, CvFileNode* fnode, CvBoost* _ensemble, CvDTreeTrainData* _data)
{
    CvDTree::read(fs, fnode, _data);
    ensemble = _ensemble;
}


void CvBoostTree::read(CvFileStorage*, CvFileNode*)
{
    assert(0);
}

void CvBoostTree::read(CvFileStorage* _fs, CvFileNode* _node,
                        CvDTreeTrainData* _data)
{
    CvDTree::read(_fs, _node, _data);
}


/////////////////////////////////// CvBoost /////////////////////////////////////

CvBoost::CvBoost()
{
    data = 0;
    weak = 0;
    default_model_name = "my_boost_tree";
    orig_response = sum_response = weak_eval = subsample_mask =
        weights = subtree_weights = 0;

    clear();
}


void CvBoost::prune(CSlice slice)
{
    if(weak)
    {
        CvSeqReader reader;
        int i, count = cvSliceLength(slice, weak);
        
        cvStartReadSeq(weak, &reader);
        cvSetSeqReaderPos(&reader, slice.start_index);

        for(i = 0; i < count; i++)
        {
            CvBoostTree* w;
            CC_READ_SEQ_ELEM(w, reader);
            delete w;
        }

        cvSeqRemoveSlice(weak, slice);
    }
}


void CvBoost::clear()
{
    if(weak)
    {
        prune(CC_WHOLE_SEQ);
        cvReleaseMemStorage(&weak->storage);
    }
    if(data)
        delete data;
    weak = 0;
    data = 0;
    cvReleaseMat(&orig_response);
    cvReleaseMat(&sum_response);
    cvReleaseMat(&weak_eval);
    cvReleaseMat(&subsample_mask);
    cvReleaseMat(&weights);
    have_subsample = false;
}


CvBoost::~CvBoost()
{
    clear();
}


CvBoost::CvBoost(const img_t* _train_data, int _tflag,
                  const img_t* _responses, const img_t* _var_idx,
                  const img_t* _sample_idx, const img_t* _var_type,
                  const img_t* _missing_mask, CvBoostParams _params)
{
    weak = 0;
    data = 0;
    default_model_name = "my_boost_tree";
    orig_response = sum_response = weak_eval = subsample_mask = weights = 0;

    train(_train_data, _tflag, _responses, _var_idx, _sample_idx,
           _var_type, _missing_mask, _params);
}


bool
CvBoost::set_params(const CvBoostParams& _params)
{
    bool ok = false;
    
    CC_FUNCNAME("CvBoost::set_params");

    __BEGIN__;

    params = _params;
    if(params.boost_type != DISCRETE && params.boost_type != REAL &&
        params.boost_type != LOGIT && params.boost_type != GENTLE)
        CC_ERROR(CC_StsBadArg, "Unknown/unsupported boosting type");

    params.weak_count = MAX(params.weak_count, 1);
    params.weight_trim_rate = MAX(params.weight_trim_rate, 0.);
    params.weight_trim_rate = MIN(params.weight_trim_rate, 1.);
    if(params.weight_trim_rate < FLT_EPSILON)
        params.weight_trim_rate = 1.f;

    if(params.boost_type == DISCRETE &&
        params.split_criteria != GINI && params.split_criteria != MISCLASS)
        params.split_criteria = MISCLASS;
    if(params.boost_type == REAL &&
        params.split_criteria != GINI && params.split_criteria != MISCLASS)
        params.split_criteria = GINI;
    if((params.boost_type == LOGIT || params.boost_type == GENTLE) &&
        params.split_criteria != SQERR)
        params.split_criteria = SQERR;
    
    ok = true;
    
    __END__;

    return ok;
}


bool
CvBoost::train(const img_t* _train_data, int _tflag,
              const img_t* _responses, const img_t* _var_idx,
              const img_t* _sample_idx, const img_t* _var_type,
              const img_t* _missing_mask,
              CvBoostParams _params, bool _update)
{
    bool ok = false;
    CvMemStorage* storage = 0;

    CC_FUNCNAME("CvBoost::train");

    __BEGIN__;

    int i;

    set_params(_params);

    if(!_update || !data)
    {
        clear();
        data = new CvDTreeTrainData(_train_data, _tflag, _responses, _var_idx,
            _sample_idx, _var_type, _missing_mask, _params, true, true);

        if(data->get_num_classes() != 2)
            CC_ERROR(CC_StsNotImplemented,
            "Boosted trees can only be used for 2-class classification.");
        CC_CALL(storage = cvCreateMemStorage());
        weak = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvBoostTree*), storage);
        storage = 0;
    }
    else
    {
        data->set_data(_train_data, _tflag, _responses, _var_idx,
            _sample_idx, _var_type, _missing_mask, _params, true, true, true);
    }

    update_weights(0);

    for(i = 0; i < params.weak_count; i++)
    {
        CvBoostTree* tree = new CvBoostTree;
        if(!tree->train(data, subsample_mask, this))
        {
            delete tree;
            continue;
        }
        //cvCheckArr(get_weak_response());
        cvSeqPush(weak, &tree);
        update_weights(tree);
        trim_weights();
    }

    data->is_classifier = true;
    ok = true;

    __END__;

    return ok;
}


void
CvBoost::update_weights(CvBoostTree* tree)
{
    CC_FUNCNAME("CvBoost::update_weights");

    __BEGIN__;

    int i, count = data->sample_count;
    double sumw = 0.;

    if(!tree) // before training the first tree, initialize weights and other parameters
    {
        const int* class_labels = data->get_class_labels(data->data_root);
        // in case of logitboost and gentle adaboost each weak tree is a regression tree,
        // so we need to convert class labels to floating-point values
        float* responses = data->get_ord_responses(data->data_root);
        int* labels = data->get_labels(data->data_root);
        double w0 = 1./count;
        double p[2] = { 1, 1 };
        
        cvReleaseMat(&orig_response);
        cvReleaseMat(&sum_response);
        cvReleaseMat(&weak_eval);
        cvReleaseMat(&subsample_mask);
        cvReleaseMat(&weights);

        CC_CALL(orig_response = cvCreateMat(1, count, CC_32S));
        CC_CALL(weak_eval = cvCreateMat(1, count, CC_64F));
        CC_CALL(subsample_mask = cvCreateMat(1, count, CC_8U));
        CC_CALL(weights = cvCreateMat(1, count, CC_64F));
        CC_CALL(subtree_weights = cvCreateMat(1, count + 2, CC_64F));

        if(data->have_priors)
        {
            // compute weight scale for each class from their prior probabilities
            int c1 = 0;
            for(i = 0; i < count; i++)
                c1 += class_labels[i];
            p[0] = data->priors->tt.db[0]*(c1 < count ? 1./(count - c1) : 0.);
            p[1] = data->priors->tt.db[1]*(c1 > 0 ? 1./c1 : 0.);
            p[0] /= p[0] + p[1];
            p[1] = 1. - p[0];
        }

        for(i = 0; i < count; i++)
        {
            // save original categorical responses {0,1}, convert them to {-1,1}
            orig_response->tt.i[i] = class_labels[i]*2 - 1;
            // make all the samples active at start.
            // later, in trim_weights() deactivate/reactive again some, if need
            subsample_mask->tt.data[i] = (uchar)1;
            // make all the initial weights the same.
            weights->tt.db[i] = w0*p[class_labels[i]];
            // set the labels to find (from within weak tree learning proc)
            // the particular sample weight, and where to store the response.
            labels[i] = i;
        }

        if(params.boost_type == LOGIT)
        {
            CC_CALL(sum_response = cvCreateMat(1, count, CC_64F));
            
            for(i = 0; i < count; i++)
            {
                sum_response->tt.db[i] = 0;
                responses[i] = orig_response->tt.i[i] > 0 ? 2.f : -2.f;
            }

            // in case of logitboost each weak tree is a regression tree.
            // the target function values are recalculated for each of the trees
            data->is_classifier = false;
        }
        else if(params.boost_type == GENTLE)
        {
            for(i = 0; i < count; i++)
                responses[i] = (float)orig_response->tt.i[i];

            data->is_classifier = false;
        }
    }
    else
    {
        // at this moment, for all the samples that participated in the training of the most
        // recent weak classifier we know the responses. For other samples we need to compute them
        if(have_subsample)
        {
            float* values = (float*)(data->buf->tt.data + data->buf->step);
            uchar* missing = data->buf->tt.data + data->buf->step*2;
            img_t _sample, _mask;

            // invert the subsample mask
            cvXorS(subsample_mask, cScalar(1.), subsample_mask);
            data->get_vectors(subsample_mask, values, missing, 0);
            //data->get_vectors(0, values, missing, 0);

            _sample = cvMat(1, data->var_count, CC_32F);
            _mask = cvMat(1, data->var_count, CC_8U);

            // run tree through all the non-processed samples
            for(i = 0; i < count; i++)
                if(subsample_mask->tt.data[i])
                {
                    _sample->tt.fl = values;
                    _mask->tt.data = missing;
                    values += _sample->cols;
                    missing += _mask->cols;
                    weak_eval->tt.db[i] = tree->predict(&_sample, &_mask, true)->value;
                }
        }

        // now update weights and other parameters for each type of boosting
        if(params.boost_type == DISCRETE)
        {
            // Discrete AdaBoost:
            //   weak_eval[i] (=f(x_i)) is in {-1,1}
            //   err = sum(w_i*(f(x_i) != y_i))/sum(w_i)
            //   C = log((1-err)/err)
            //   w_i *= exp(C*(f(x_i) != y_i))
            
            double C, err = 0.;
            double scale[] = { 1., 0. };

            for(i = 0; i < count; i++)
            {
                double w = weights->tt.db[i];
                sumw += w;
                err += w*(weak_eval->tt.db[i] != orig_response->tt.i[i]);
            }
            
            if(sumw != 0)
                err /= sumw;
            C = err = -log_ratio(err);
            scale[1] = exp(err);
    
            sumw = 0;
            for(i = 0; i < count; i++)
            {
                double w = weights->tt.db[i]*
                    scale[weak_eval->tt.db[i] != orig_response->tt.i[i]];
                sumw += w;
                weights->tt.db[i] = w;
            }

            tree->scale(C);
        }
        else if(params.boost_type == REAL)
        {
            // Real AdaBoost:
            //   weak_eval[i] = f(x_i) = 0.5*log(p(x_i)/(1-p(x_i))), p(x_i)=P(y=1|x_i)
            //   w_i *= exp(-y_i*f(x_i))
            
            for(i = 0; i < count; i++)
                weak_eval->tt.db[i] *= -orig_response->tt.i[i];

            cvExp(weak_eval, weak_eval);

            for(i = 0; i < count; i++)
            {
                double w = weights->tt.db[i]*weak_eval->tt.db[i];
                sumw += w;
                weights->tt.db[i] = w;
            }
        }
        else if(params.boost_type == LOGIT)
        {
            // LogitBoost:
            //   weak_eval[i] = f(x_i) in [-z_max,z_max]
            //   sum_response = F(x_i).
            //   F(x_i) += 0.5*f(x_i)
            //   p(x_i) = exp(F(x_i))/(exp(F(x_i)) + exp(-F(x_i))=1/(1+exp(-2*F(x_i)))
            //   reuse weak_eval: weak_eval[i] <- p(x_i)
            //   w_i = p(x_i)*1(1 - p(x_i))
            //   z_i = ((y_i+1)/2 - p(x_i))/(p(x_i)*(1 - p(x_i)))
            //   store z_i to the data->data_root as the new target responses

            const double lb_weight_thresh = FLT_EPSILON;
            const double lb_z_max = 10.;
            float* responses = data->get_ord_responses(data->data_root);

            /*if(weak->total == 7)
                putchar('*');*/

            for(i = 0; i < count; i++)
            {
                double s = sum_response->tt.db[i] + 0.5*weak_eval->tt.db[i];
                sum_response->tt.db[i] = s;
                weak_eval->tt.db[i] = -2*s;
            }

            cvExp(weak_eval, weak_eval);
            
            for(i = 0; i < count; i++)
            {
                double p = 1./(1. + weak_eval->tt.db[i]);
                double w = p*(1 - p), z;
                w = MAX(w, lb_weight_thresh);
                weights->tt.db[i] = w;
                sumw += w;
                if(orig_response->tt.i[i] > 0)
                {
                    z = 1./p;
                    responses[i] = (float)MIN(z, lb_z_max);
                }
                else
                {
                    z = 1./(1-p);
                    responses[i] = (float)-MIN(z, lb_z_max);
                }
            }
        }
        else
        {
            // Gentle AdaBoost:
            //   weak_eval[i] = f(x_i) in [-1,1]
            //   w_i *= exp(-y_i*f(x_i))
            assert(params.boost_type == GENTLE);
            
            for(i = 0; i < count; i++)
                weak_eval->tt.db[i] *= -orig_response->tt.i[i];

            cvExp(weak_eval, weak_eval);

            for(i = 0; i < count; i++)
            {
                double w = weights->tt.db[i] * weak_eval->tt.db[i];
                weights->tt.db[i] = w;
                sumw += w;
            }
        }
    }

    // renormalize weights
    if(sumw > FLT_EPSILON)
    {
        sumw = 1./sumw;
        for(i = 0; i < count; ++i)
            weights->tt.db[i] *= sumw;
    }

    __END__;
}


static CC_IMPLEMENT_QSORT_EX(icvSort_64f, double, CC_LT, int)


void
CvBoost::trim_weights()
{
    CC_FUNCNAME("CvBoost::trim_weights");

    __BEGIN__;

    int i, count = data->sample_count, nz_count = 0;
    double sum, threshold;

    if(params.weight_trim_rate <= 0. || params.weight_trim_rate >= 1.)
        EXIT;

    // use weak_eval as temporary buffer for sorted weights
    cvCopy(weights, weak_eval);

    icvSort_64f(weak_eval->tt.db, count, 0);

    // as weight trimming occurs immediately after updating the weights,
    // where they are renormalized, we assume that the weight sum = 1.
    sum = 1. - params.weight_trim_rate;

    for(i = 0; i < count; i++)
    {
        double w = weak_eval->tt.db[i];
        if(sum > w)
            break;
        sum -= w;
    }

    threshold = i < count ? weak_eval->tt.db[i] : DBL_MAX;

    for(i = 0; i < count; i++)
    {
        double w = weights->tt.db[i];
        int f = w > threshold;
        subsample_mask->tt.data[i] = (uchar)f;
        nz_count += f;
    }

    have_subsample = nz_count < count;

    __END__;
}


float
CvBoost::predict(const img_t* _sample, const img_t* _missing,
                  img_t* weak_responses, CSlice slice,
                  bool raw_mode) const
{
    float* buf = 0;
    bool allocated = false;
    float value = -FLT_MAX;
    
    CC_FUNCNAME("CvBoost::predict");

    __BEGIN__;

    int i, weak_count, var_count;
    img_t sample, missing;
    CvSeqReader reader;
    double sum = 0;
    int cls_idx;
    int wstep = 0;
    const int* vtype;
    const int* cmap;
    const int* cofs;

    if(!weak)
        CC_ERROR(CC_StsError, "The boosted tree ensemble has not been trained yet");

    if(!CC_IS_MAT(_sample) || CC_MAT_TYPE(_sample->tid) != CC_32F ||
        _sample->cols != 1 && _sample->rows != 1 ||
        _sample->cols + _sample->rows - 1 != data->var_all && !raw_mode ||
        _sample->cols + _sample->rows - 1 != data->var_count && raw_mode)
            CC_ERROR(CC_StsBadArg,
        "the input sample must be 1d floating-point vector with the same "
        "number of elements as the total number of variables used for training");

    if(_missing)
    {
        if(!CC_IS_MAT(_missing) || !CC_IS_MASK_ARR(_missing) ||
            !CC_ARE_SIZES_EQ(_missing, _sample))
            CC_ERROR(CC_StsBadArg,
            "the missing data mask must be 8-bit vector of the same size as input sample");
    }

    weak_count = cvSliceLength(slice, weak);
    if(weak_count >= weak->total)
    {
        weak_count = weak->total;
        slice.start_index = 0;
    }

    if(weak_responses)
    {
        if(!CC_IS_MAT(weak_responses) ||
            CC_MAT_TYPE(weak_responses->tid) != CC_32F ||
            weak_responses->cols != 1 && weak_responses->rows != 1 ||
            weak_responses->cols + weak_responses->rows - 1 != weak_count)
            CC_ERROR(CC_StsBadArg,
            "The output matrix of weak classifier responses must be valid "
            "floating-point vector of the same number of components as the length of input slice");
        wstep = CC_IS_MAT_CONT(weak_responses->tid) ? 1 : weak_responses->step/sizeof(float);
    }

    var_count = data->var_count;
    vtype = data->var_type->tt.i;
    cmap = data->cat_map->tt.i;
    cofs = data->cat_ofs->tt.i;

    // if need, preprocess the input vector
    if(!raw_mode && (data->cat_var_count > 0 || data->var_idx))
    {
        int bufsize;
        int step, mstep = 0;
        const float* src_sample;
        const uchar* src_mask = 0;
        float* dst_sample;
        uchar* dst_mask;
        const int* vidx = data->var_idx && !raw_mode ? data->var_idx->tt.i : 0;
        bool have_mask = _missing != 0;

        bufsize = var_count*(sizeof(float) + sizeof(uchar));
        if(bufsize <= CC_MAX_LOCAL_SIZE)
            buf = (float*)cvStackAlloc(bufsize);
        else
        {
            CC_CALL(buf = (float*)cAlloc(bufsize));
            allocated = true;
        }
        dst_sample = buf;
        dst_mask = (uchar*)(buf + var_count);

        src_sample = _sample->tt.fl;
        step = CC_IS_MAT_CONT(_sample->tid) ? 1 : _sample->step/sizeof(src_sample[0]);

        if(_missing)
        {
            src_mask = _missing->tt.data;
            mstep = CC_IS_MAT_CONT(_missing->tid) ? 1 : _missing->step;
        }

        for(i = 0; i < var_count; i++)
        {
            int idx = vidx ? vidx[i] : i;
            float val = src_sample[idx*step];
            int ci = vtype[i];
            uchar m = src_mask ? src_mask[i] : (uchar)0;

            if(ci >= 0)
            {
                int a = cofs[ci], b = cofs[ci+1], c = a;
                int ival = cRound(val);
                if(ival != val)
                    CC_ERROR(CC_StsBadArg,
                    "one of input categorical variable is not an integer");

                while(a < b)
                {
                    c = (a + b) >> 1;
                    if(ival < cmap[c])
                        b = c;
                    else if(ival > cmap[c])
                        a = c+1;
                    else
                        break;
                }

                if(c < 0 || ival != cmap[c])
                {
                    m = 1;
                    have_mask = true;
                }
                else
                {
                    val = (float)(c - cofs[ci]);
                }
            }

            dst_sample[i] = val;
            dst_mask[i] = m;
        }

        sample = cvMat(1, var_count, CC_32F, dst_sample);
        _sample = &sample;

        if(have_mask)
        {
            missing = cvMat(1, var_count, CC_8UC1, dst_mask);
            _missing = &missing;
        }
    }

    cvStartReadSeq(weak, &reader);
    cvSetSeqReaderPos(&reader, slice.start_index);

    for(i = 0; i < weak_count; i++)
    {
        CvBoostTree* wtree;
        double val;

        CC_READ_SEQ_ELEM(wtree, reader);

        val = wtree->predict(_sample, _missing, true)->value;
        if(weak_responses)
            weak_responses->tt.fl[i*wstep] = (float)val;

        sum += val;
    }

    cls_idx = sum >= 0;
    if(raw_mode)
        value = (float)cls_idx;
    else
        value = (float)cmap[cofs[vtype[var_count]] + cls_idx];

    __END__;

    if(allocated)
        cFree(&buf);

    return value;
}



void CvBoost::write_params(CvFileStorage* fs)
{
    CC_FUNCNAME("CvBoost::write_params");

    __BEGIN__;

    const char* boost_type_str =
        params.boost_type == DISCRETE ? "DiscreteAdaboost" :
        params.boost_type == REAL ? "RealAdaboost" :
        params.boost_type == LOGIT ? "LogitBoost" :
        params.boost_type == GENTLE ? "GentleAdaboost" : 0;

    const char* split_crit_str =
        params.split_criteria == DEFAULT ? "Default" :
        params.split_criteria == GINI ? "Gini" :
        params.boost_type == MISCLASS ? "Misclassification" :
        params.boost_type == SQERR ? "SquaredErr" : 0;

    if(boost_type_str)
        cvWriteString(fs, "boosting_type", boost_type_str);
    else
        cvWriteInt(fs, "boosting_type", params.boost_type);

    if(split_crit_str)
        cvWriteString(fs, "splitting_criteria", split_crit_str);
    else
        cvWriteInt(fs, "splitting_criteria", params.split_criteria);

    cvWriteInt(fs, "ntrees", params.weak_count);
    cvWriteReal(fs, "weight_trimming_rate", params.weight_trim_rate);

    data->write_params(fs);

    __END__;
}


void CvBoost::read_params(CvFileStorage* fs, CvFileNode* fnode)
{
    CC_FUNCNAME("CvBoost::read_params");

    __BEGIN__;

    CvFileNode* temp;

    if(!fnode || !CC_NODE_IS_MAP(fnode->tag))
        return;

    data = new CvDTreeTrainData();
    CC_CALL(data->read_params(fs, fnode));
    data->shared = true;

    params.max_type = data->params.max_type;
    params.min_sample_count = data->params.min_sample_count;
    params.max_categories = data->params.max_categories;
    params.priors = data->params.priors;
    params.regression_accuracy = data->params.regression_accuracy;
    params.use_surrogates = data->params.use_surrogates;

    temp = cvGetFileNodeByName(fs, fnode, "boosting_type");
    if(!temp)
        return;

    if(temp && CC_NODE_IS_STRING(temp->tag))
    {
        const char* boost_type_str = cvReadString(temp, "");
        params.boost_type = strcmp(boost_type_str, "DiscreteAdaboost") == 0 ? DISCRETE :
                            strcmp(boost_type_str, "RealAdaboost") == 0 ? REAL :
                            strcmp(boost_type_str, "LogitBoost") == 0 ? LOGIT :
                            strcmp(boost_type_str, "GentleAdaboost") == 0 ? GENTLE : -1;
    }
    else
        params.boost_type = cvReadInt(temp, -1);

    if(params.boost_type < DISCRETE || params.boost_type > GENTLE)
        CC_ERROR(CC_StsBadArg, "Unknown boosting type");

    temp = cvGetFileNodeByName(fs, fnode, "splitting_criteria");
    if(temp && CC_NODE_IS_STRING(temp->tag))
    {
        const char* split_crit_str = cvReadString(temp, "");
        params.split_criteria = strcmp(split_crit_str, "Default") == 0 ? DEFAULT :
                                strcmp(split_crit_str, "Gini") == 0 ? GINI :
                                strcmp(split_crit_str, "Misclassification") == 0 ? MISCLASS :
                                strcmp(split_crit_str, "SquaredErr") == 0 ? SQERR : -1;
    }
    else
        params.split_criteria = cvReadInt(temp, -1);

    if(params.split_criteria < DEFAULT || params.boost_type > SQERR)
        CC_ERROR(CC_StsBadArg, "Unknown boosting type");

    params.weak_count = cvReadIntByName(fs, fnode, "ntrees");
    params.weight_trim_rate = cvReadRealByName(fs, fnode, "weight_trimming_rate", 0.);

    __END__;
}



void
CvBoost::read(CvFileStorage* fs, CvFileNode* node)
{
    CC_FUNCNAME("CvRTrees::read");

    __BEGIN__;

    CvSeqReader reader;
    CvFileNode* trees_fnode;
    CvMemStorage* storage;
    int i, ntrees;

    clear();
    read_params(fs, node);

    if(!data)
        EXIT;
        
    trees_fnode = cvGetFileNodeByName(fs, node, "trees");
    if(!trees_fnode || !CC_NODE_IS_SEQ(trees_fnode->tag))
        CC_ERROR(CC_StsParseError, "<trees> tag is missing");

    cvStartReadSeq(trees_fnode->tt.seq, &reader);
    ntrees = trees_fnode->tt.seq->total;

    if(ntrees != params.weak_count)
        CC_ERROR(CC_StsUnmatchedSizes,
        "The number of trees stored does not match <ntrees> tag value");

    CC_CALL(storage = cvCreateMemStorage());
    weak = cvCreateSeq(0, sizeof(CvSeq), sizeof(CvBoostTree*), storage);

    for(i = 0; i < ntrees; i++)
    {
        CvBoostTree* tree = new CvBoostTree();
        CC_CALL(tree->read(fs, (CvFileNode*)reader.ptr, this, data));
        CC_NEXT_SEQ_ELEM(reader.seq->elem_size, reader);
        cvSeqPush(weak, &tree);
    }

    __END__;
}


void
CvBoost::write(CvFileStorage* fs, const char* name)
{
    CC_FUNCNAME("CvBoost::write");

    __BEGIN__;
    
    CvSeqReader reader;
    int i;

    cvWriteStructBegin(fs, name, CC_NODE_MAP, CC_TYPE_NAME_ML_BOOSTING);

    if(!weak)
        CC_ERROR(CC_StsBadArg, "The classifier has not been trained yet");
        
    write_params(fs);
    cvWriteStructBegin(fs, "trees", CC_NODE_SEQ);

    cvStartReadSeq(weak, &reader);

    for(i = 0; i < weak->total; i++)
    {
        CvBoostTree* tree;
        CC_READ_SEQ_ELEM(tree, reader);
        cvWriteStructBegin(fs, 0, CC_NODE_MAP);
        tree->write(fs);
        cvWriteStructEnd(fs);
    }

    cvWriteStructEnd(fs);
    cvWriteStructEnd(fs);

    __END__;
}


img_t*
CvBoost::get_weights()
{
    return weights;
}


img_t*
CvBoost::get_subtree_weights()
{
    return subtree_weights;
}


img_t*
CvBoost::get_weak_response()
{
    return weak_eval;
}


const CvBoostParams&
CvBoost::get_params() const
{
    return params;
}

CvSeq* CvBoost::get_weak_predictors()
{
    return weak;
}


