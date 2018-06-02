
#include "_ml.h"

CvForestTree::CvForestTree()
{
    forest = NULL;
}


CvForestTree::~CvForestTree()
{
    clear();
}


bool CvForestTree::train(CvDTreeTrainData* _data,
                          const img_t* _subsample_idx,
                          CvRTrees* _forest)
{
    bool result = false;

    CC_FUNCNAME("CvForestTree::train");

    __BEGIN__;


    clear();
    forest = _forest;

    data = _data;
    data->shared = true;
    CC_CALL(result = do_train(_subsample_idx));

    __END__;

    return result;
}


bool
CvForestTree::train(const img_t*, int, const img_t*, const img_t*,
                    const img_t*, const img_t*, const img_t*, CvDTreeParams)
{
    assert(0);
    return false;
}


bool
CvForestTree::train(CvDTreeTrainData*, const img_t*)
{
    assert(0);
    return false;
}


CvDTreeSplit* CvForestTree::find_best_split(CvDTreeNode* node)
{
    int vi;
    CvDTreeSplit *best_split = 0, *split = 0, *t;

    CC_FUNCNAME("CvForestTree::find_best_split");
    __BEGIN__;

    img_t* active_var_mask = 0;
    if(forest)
    {
        int var_count;
        CRNG* rng = forest->get_rng();

        active_var_mask = forest->get_active_var_mask();
        var_count = active_var_mask->cols;

        CC_ASSERT(var_count == data->var_count);

        for(vi = 0; vi < var_count; vi++)
        {
            uchar temp;
            int i1 = cvRandInt(rng) % var_count;
            int i2 = cvRandInt(rng) % var_count;
            CC_SWAP(active_var_mask->tt.data[i1],
                active_var_mask->tt.data[i2], temp);
        }
    }
    for(vi = 0; vi < data->var_count; vi++)
    {
        int ci = data->var_type->tt.i[vi];
        if(node->num_valid[vi] <= 1
            || (active_var_mask && !active_var_mask->tt.data[vi]))
            continue;

        if(data->is_classifier)
        {
            if(ci >= 0)
                split = find_split_cat_class(node, vi);
            else
                split = find_split_ord_class(node, vi);
        }
        else
        {
            if(ci >= 0)
                split = find_split_cat_reg(node, vi);
            else
                split = find_split_ord_reg(node, vi);
        }

        if(split)
        {
            if(!best_split || best_split->quality < split->quality)
                CC_SWAP(best_split, split, t);
            if(split)
                cvSetRemoveByPtr(data->split_heap, split);
        }
    }

    __END__;

    return best_split;
}


void CvForestTree::read(CvFileStorage* fs, CvFileNode* fnode, CvRTrees* _forest, CvDTreeTrainData* _data)
{
    CvDTree::read(fs, fnode, _data);
    forest = _forest;
}


void CvForestTree::read(CvFileStorage*, CvFileNode*)
{
    assert(0);
}

void CvForestTree::read(CvFileStorage* _fs, CvFileNode* _node,
                         CvDTreeTrainData* _data)
{
    CvDTree::read(_fs, _node, _data);
}


//////////////////////////////////////////////////////////////////////////////////////////
//                                  Random trees                                        //
//////////////////////////////////////////////////////////////////////////////////////////

CvRTrees::CvRTrees()
{
    nclasses         = 0;
    oob_error        = 0;
    ntrees           = 0;
    trees            = NULL;
    data             = NULL;
    active_var_mask  = NULL;
    var_importance   = NULL;
    rng = cRNG(0xffffffff);
    default_model_name = "my_random_trees";
}


void CvRTrees::clear()
{
    int k;
    for(k = 0; k < ntrees; k++)
        delete trees[k];
    cFree(&trees);

    delete data;
    data = 0;

    cvReleaseMat(&active_var_mask);
    cvReleaseMat(&var_importance);
    ntrees = 0;
}


CvRTrees::~CvRTrees()
{
    clear();
}


img_t* CvRTrees::get_active_var_mask()
{
    return active_var_mask;
}


CRNG* CvRTrees::get_rng()
{
    return &rng;
}

bool CvRTrees::train(const img_t* _train_data, int _tflag,
                        const img_t* _responses, const img_t* _var_idx,
                        const img_t* _sample_idx, const img_t* _var_type,
                        const img_t* _missing_mask, CvRTParams params)
{
    bool result = false;

    CC_FUNCNAME("CvRTrees::train");
    __BEGIN__;

    int var_count = 0;

    clear();

    CvDTreeParams tree_params(params.max_type, params.min_sample_count,
        params.regression_accuracy, params.use_surrogates, params.max_categories,
        params.cv_folds, params.use_1se_rule, false, params.priors);
    
    data = new CvDTreeTrainData();
    CC_CALL(data->set_data(_train_data, _tflag, _responses, _var_idx,
        _sample_idx, _var_type, _missing_mask, tree_params, true));

    var_count = data->var_count;
    if(params.nactive_vars > var_count)
        params.nactive_vars = var_count;
    else if(params.nactive_vars == 0)
        params.nactive_vars = (int)sqrt((double)var_count);
    else if(params.nactive_vars < 0)
        CC_ERROR(CC_StsBadArg, "<nactive_vars> must be non-negative");
    params.term_crit = cvCheckTermCriteria(params.term_crit, 0.1, 1000);

    // Create mask of active variables at the tree nodes
    CC_CALL(active_var_mask = cvCreateMat(1, var_count, CC_8UC1));
    if(params.calc_var_importance)
    {
        CC_CALL(var_importance  = cvCreateMat(1, var_count, CC_32FC1));
        cvZero(var_importance);
    }
    { // initialize active variables mask
        img_t submask1, submask2;
        cvGetCols(active_var_mask, &submask1, 0, params.nactive_vars);
        cvGetCols(active_var_mask, &submask2, params.nactive_vars, var_count);
        cvSet(&submask1, cScalar(1));
        cvZero(&submask2);
    }

    CC_CALL(result = grow_forest(params.term_crit));

    result = true;

    __END__;

    return result;
}


bool CvRTrees::grow_forest(const CTermCriteria term_crit)
{
    bool result = false;

    img_t* sample_idx_mask_for_tree = 0;
    img_t* sample_idx_for_tree      = 0;

    img_t* oob_sample_votes	   = 0;
    img_t* oob_responses       = 0;

    float* oob_samples_perm_ptr= 0;

    float* samples_ptr     = 0;
    uchar* missing_ptr     = 0;
    float* true_resp_ptr   = 0;

    CC_FUNCNAME("CvRTrees::grow_forest");
    __BEGIN__;

    const int max_ntrees = term_crit.max_iter;
    const double max_oob_err = term_crit.epsilon;
    
    const int dims = data->var_count;
    float maximal_response = 0;

    // oob_predictions_sum[i] = sum of predicted values for the i-th sample
    // oob_num_of_predictions[i] = number of summands
    //                            (number of predictions for the i-th sample)
    // initialize these variable to avoid warning C4701
    img_t oob_predictions_sum = cvMat(1, 1, CC_32FC1);
    img_t oob_num_of_predictions = cvMat(1, 1, CC_32FC1);

    nsamples = data->sample_count;
    nclasses = data->get_num_classes();

    trees = (CvForestTree**)cAlloc(sizeof(trees[0])*max_ntrees);
    memset(trees, 0, sizeof(trees[0])*max_ntrees);

    if(data->is_classifier)
    {
        CC_CALL(oob_sample_votes = cvCreateMat(nsamples, nclasses, CC_32SC1));
        cvZero(oob_sample_votes);
    }
    else
    {
        // oob_responses[0,i] = oob_predictions_sum[i]
        //    = sum of predicted values for the i-th sample
        // oob_responses[1,i] = oob_num_of_predictions[i]
        //    = number of summands (number of predictions for the i-th sample)
        CC_CALL(oob_responses = cvCreateMat(2, nsamples, CC_32FC1));
        cvZero(oob_responses);
        cvGetRow(oob_responses, &oob_predictions_sum, 0);
        cvGetRow(oob_responses, &oob_num_of_predictions, 1);
    }
    CC_CALL(sample_idx_mask_for_tree = cvCreateMat(1, nsamples, CC_8UC1));
    CC_CALL(sample_idx_for_tree      = cvCreateMat(1, nsamples, CC_32SC1));
    CC_CALL(oob_samples_perm_ptr     = (float*)cAlloc(sizeof(float)*nsamples*dims));
    CC_CALL(samples_ptr              = (float*)cAlloc(sizeof(float)*nsamples*dims));
    CC_CALL(missing_ptr              = (uchar*)cAlloc(sizeof(uchar)*nsamples*dims));
    CC_CALL(true_resp_ptr            = (float*)cAlloc(sizeof(float)*nsamples));

    CC_CALL(data->get_vectors(0, samples_ptr, missing_ptr, true_resp_ptr));
    {
        double minval, maxval;
        img_t responses = cvMat(1, nsamples, CC_32FC1, true_resp_ptr);
        cvMinMaxLoc(&responses, &minval, &maxval);
        maximal_response = (float)MAX(MAX(fabs(minval), fabs(maxval)), 0);
    }

    ntrees = 0;
    while(ntrees < max_ntrees)
    {
        int i, oob_samples_count = 0;
        double ncorrect_responses = 0; // used for estimation of variable importance
        img_t sample, missing;
        CvForestTree* tree = 0;

        cvZero(sample_idx_mask_for_tree);
        for(i = 0; i < nsamples; i++) //form sample for creation one tree
        {
            int idx = cvRandInt(&rng) % nsamples;
            sample_idx_for_tree->tt.i[i] = idx;
            sample_idx_mask_for_tree->tt.data[idx] = 0xFF;
        }

        trees[ntrees] = new CvForestTree();
        tree = trees[ntrees];
        CC_CALL(tree->train(data, sample_idx_for_tree, this));

        // form array of OOB samples indices and get these samples
        sample   = cvMat(1, dims, CC_32FC1, samples_ptr);
        missing  = cvMat(1, dims, CC_8UC1,  missing_ptr);

        oob_error = 0;
        for(i = 0; i < nsamples; i++,
            sample->tt.fl += dims, missing->tt.data += dims)
        {
            CvDTreeNode* predicted_node = 0;
            // check if the sample is OOB
            if(sample_idx_mask_for_tree->tt.data[i])
                continue;

            // predict oob samples
            if(!predicted_node)
                CC_CALL(predicted_node = tree->predict(&sample, &missing, true));

            if(!data->is_classifier) //regression
            {
                double avg_resp, resp = predicted_node->value;
                oob_predictions_sum->tt.fl[i] += (float)resp;
                oob_num_of_predictions->tt.fl[i] += 1;

                // compute oob error
                avg_resp = oob_predictions_sum->tt.fl[i]/oob_num_of_predictions->tt.fl[i];
                avg_resp -= true_resp_ptr[i];
                oob_error += avg_resp*avg_resp;
                resp = (resp - true_resp_ptr[i])/maximal_response;
                ncorrect_responses += exp(-resp*resp);
            }
            else //classification
            {
                double prdct_resp;
                CPoint max_loc;
                img_t votes;

                cvGetRow(oob_sample_votes, &votes, i);
                votes->tt.i[predicted_node->class_idx]++;

                // compute oob error
                cvMinMaxLoc(&votes, 0, 0, 0, &max_loc);

                prdct_resp = data->cat_map->tt.i[max_loc.x];
                oob_error += (fabs(prdct_resp - true_resp_ptr[i]) < FLT_EPSILON) ? 0 : 1;

                ncorrect_responses += cRound(predicted_node->value - true_resp_ptr[i]) == 0;
            }
            oob_samples_count++;
        }
        if(oob_samples_count > 0)
            oob_error /= (double)oob_samples_count;

        // estimate variable importance
        if(var_importance && oob_samples_count > 0)
        {
            int m;

            memcpy(oob_samples_perm_ptr, samples_ptr, dims*nsamples*sizeof(float));
            for(m = 0; m < dims; m++)
            {
                double ncorrect_responses_permuted = 0;
                // randomly permute values of the m-th variable in the oob samples
                float* mth_var_ptr = oob_samples_perm_ptr + m;

                for(i = 0; i < nsamples; i++)
                {
                    int i1, i2;
                    float temp;

                    if(sample_idx_mask_for_tree->tt.data[i]) //the sample is not OOB
                        continue;
                    i1 = cvRandInt(&rng) % nsamples;
                    i2 = cvRandInt(&rng) % nsamples;
                    CC_SWAP(mth_var_ptr[i1*dims], mth_var_ptr[i2*dims], temp);

                    // turn values of (m-1)-th variable, that were permuted
                    // at the previous iteration, untouched
                    if(m > 1)
                        oob_samples_perm_ptr[i*dims+m-1] = samples_ptr[i*dims+m-1];
                }
    
                // predict "permuted" cases and calculate the number of votes for the
                // correct class in the variable-m-permuted oob data
                sample  = cvMat(1, dims, CC_32FC1, oob_samples_perm_ptr);
                missing = cvMat(1, dims, CC_8UC1, missing_ptr);
                for(i = 0; i < nsamples; i++,
                    sample->tt.fl += dims, missing->tt.data += dims)
                {
                    double predct_resp, true_resp;

                    if(sample_idx_mask_for_tree->tt.data[i]) //the sample is not OOB
                        continue;

                    predct_resp = tree->predict(&sample, &missing, true)->value;
                    true_resp   = true_resp_ptr[i];
                    if(data->is_classifier)
                        ncorrect_responses_permuted += cRound(true_resp - predct_resp) == 0;
                    else
                    {
                        true_resp = (true_resp - predct_resp)/maximal_response;
                        ncorrect_responses_permuted += exp(-true_resp*true_resp);
                    }
                }
                var_importance->tt.fl[m] += (float)(ncorrect_responses
                    - ncorrect_responses_permuted);
            }
        }
        ntrees++;
        if(term_crit.type != CC_TERMCRIT_ITER && oob_error < max_oob_err)
            break;
    }
    if(var_importance)
        CC_CALL(cvConvertScale(var_importance, var_importance, 1./ntrees/nsamples));

    result = true;

    __END__;

    cvReleaseMat(&sample_idx_mask_for_tree);
    cvReleaseMat(&sample_idx_for_tree);
    cvReleaseMat(&oob_sample_votes);
    cvReleaseMat(&oob_responses);

    cFree(&oob_samples_perm_ptr);
    cFree(&samples_ptr);
    cFree(&missing_ptr);
    cFree(&true_resp_ptr);

    return result;
}


const img_t* CvRTrees::get_var_importance()
{
    return var_importance;
}


float CvRTrees::get_proximity(const img_t* sample1, const img_t* sample2,
                              const img_t* missing1, const img_t* missing2) const
{
    float result = 0;

    CC_FUNCNAME("CvRTrees::get_proximity");

    __BEGIN__;

    int i;
    for(i = 0; i < ntrees; i++)
        result += trees[i]->predict(sample1, missing1) ==
        trees[i]->predict(sample2, missing2) ?  1 : 0;
    result = result/(float)ntrees;

    __END__;

    return result;
}


float CvRTrees::predict(const img_t* sample, const img_t* missing) const
{
    double result = -1;

    CC_FUNCNAME("CvRTrees::predict");
    __BEGIN__;

    int k;

    if(nclasses > 0) //classification
    {
        int max_nvotes = 0;
        int* votes = (int*)alloca(sizeof(int)*nclasses);
        memset(votes, 0, sizeof(*votes)*nclasses);
        for(k = 0; k < ntrees; k++)
        {
            CvDTreeNode* predicted_node = trees[k]->predict(sample, missing);
            int nvotes;
            int class_idx = predicted_node->class_idx;
            CC_ASSERT(0 <= class_idx && class_idx < nclasses);

            nvotes = ++votes[class_idx];
            if(nvotes > max_nvotes)
            {
                max_nvotes = nvotes;
                result = predicted_node->value;
            }
        }
    }
    else // regression
    {
        result = 0;
        for(k = 0; k < ntrees; k++)
            result += trees[k]->predict(sample, missing)->value;
        result /= (double)ntrees;
    }

    __END__;

    return (float)result;
}


void CvRTrees::write(CvFileStorage* fs, const char* name)
{
    CC_FUNCNAME("CvRTrees::write");

    __BEGIN__;

    int k;

    if(ntrees < 1 || !trees || nsamples < 1)
        CC_ERROR(CC_StsBadArg, "Invalid CvRTrees object");

    cvWriteStructBegin(fs, name, CC_NODE_MAP, CC_TYPE_NAME_ML_RTREES);

    cvWriteInt(fs, "nclasses", nclasses);
    cvWriteInt(fs, "nsamples", nsamples);
    cvWriteInt(fs, "nactive_vars", (int)cvSum(active_var_mask).val[0]);
    cvWriteReal(fs, "oob_error", oob_error);

    if(var_importance)
        cvWrite(fs, "var_importance", var_importance);

    cvWriteInt(fs, "ntrees", ntrees);

    CC_CALL(data->write_params(fs));

    cvWriteStructBegin(fs, "trees", CC_NODE_SEQ);

    for(k = 0; k < ntrees; k++)
    {
        cvWriteStructBegin(fs, 0, CC_NODE_MAP);
        CC_CALL(trees[k]->write(fs));
        cvWriteStructEnd(fs);
    }

    cvWriteStructEnd(fs); //trees
    cvWriteStructEnd(fs); //CC_TYPE_NAME_ML_RTREES

    __END__;
}


void CvRTrees::read(CvFileStorage* fs, CvFileNode* fnode)
{
    CC_FUNCNAME("CvRTrees::read");

    __BEGIN__;

    int nactive_vars, var_count, k;
    CvSeqReader reader;
    CvFileNode* trees_fnode = 0;

    clear();

    nclasses     = cvReadIntByName(fs, fnode, "nclasses", -1);
    nsamples     = cvReadIntByName(fs, fnode, "nsamples");
    nactive_vars = cvReadIntByName(fs, fnode, "nactive_vars", -1);
    oob_error    = cvReadRealByName(fs, fnode, "oob_error", -1);
    ntrees       = cvReadIntByName(fs, fnode, "ntrees", -1);

    var_importance = cvReadByName(fs, fnode, "var_importance");

    if(nclasses < 0 || nsamples <= 0 || nactive_vars < 0 || oob_error < 0 || ntrees <= 0)
        CC_ERROR(CC_StsParseError, "Some <nclasses>, <nsamples>, <var_count>, "
        "<nactive_vars>, <oob_error>, <ntrees> of tags are missing");

    rng = CRNG(-1);

    trees = (CvForestTree**)cAlloc(sizeof(trees[0])*ntrees);
    memset(trees, 0, sizeof(trees[0])*ntrees);

    data = new CvDTreeTrainData();
    data->read_params(fs, fnode);
    data->shared = true;

    trees_fnode = cvGetFileNodeByName(fs, fnode, "trees");
    if(!trees_fnode || !CC_NODE_IS_SEQ(trees_fnode->tag))
        CC_ERROR(CC_StsParseError, "<trees> tag is missing");

    cvStartReadSeq(trees_fnode->tt.seq, &reader);
    if(reader.seq->total != ntrees)
        CC_ERROR(CC_StsParseError,
        "<ntrees> is not equal to the number of trees saved in file");

    for(k = 0; k < ntrees; k++)
    {
        trees[k] = new CvForestTree();
        CC_CALL(trees[k]->read(fs, (CvFileNode*)reader.ptr, this, data));
        CC_NEXT_SEQ_ELEM(reader.seq->elem_size, reader);
    }

    var_count = data->var_count;
    CC_CALL(active_var_mask = cvCreateMat(1, var_count, CC_8UC1));
    {
        // initialize active variables mask
        img_t submask1, submask2;
        cvGetCols(active_var_mask, &submask1, 0, nactive_vars);
        cvGetCols(active_var_mask, &submask2, nactive_vars, var_count);
        cvSet(&submask1, cScalar(1));
        cvZero(&submask2);
    }

    __END__;
}


int CvRTrees::get_tree_count() const
{
    return ntrees;
}

CvForestTree* CvRTrees::get_tree(int i) const
{
    return (unsigned)i < (unsigned)ntrees ? trees[i] : 0;
}

// End of file.
