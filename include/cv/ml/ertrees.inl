/*M///////////////////////////////////////////////////////////////////////////////////////

  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.

  By downloading, copying, installing or using the software you agree to this license.
  If you do not agree to this license, do not download, install,
  copy or use the software.


                        Intel License Agreement

 Copyright (C) 2000, Intel Corporation, all rights reserved.
 Third party copyrights are property of their respective owners.

 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:

   * Redistribution's of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.

   * Redistribution's in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

   * The name of Intel Corporation may not be used to endorse or promote products
     derived from this software without specific prior written permission.

 This software is provided by the copyright holders and contributors "as is" and
 any express or implied warranties, including, but not limited to, the implied
 warranties of merchantability and fitness for a particular purpose are disclaimed.
 In no event shall the Intel Corporation or contributors be liable for any direct,
 indirect, incidental, special, exemplary, or consequential damages
 (including, but not limited to, procurement of substitute goods or services;
 loss of use, data, or profits; or business interruption) however caused
 and on any theory of liability, whether in contract, strict liability,
 or tort (including negligence or otherwise) arising in any way out of
 the use of this software, even if advised of the possibility of such damage.

M*/



static const float ord_nan = FLT_MAX*0.5f;
static const int min_block_size = 1 << 16;
static const int block_size_delta = 1 << 10;

#define CC_CMP_NUM_PTR(a,b) (*(a) < *(b))
static CC_IMPLEMENT_QSORT_EX(icvSortIntPtr, int*, CC_CMP_NUM_PTR, int)

#define CC_CMP_PAIRS(a,b) (*((a).i) < *((b).i))
static CC_IMPLEMENT_QSORT_EX(icvSortPairs, CvPair16u32s, CC_CMP_PAIRS, int)

///

void CvERTreeTrainData::set_data(const img_t* _train_data, int _tflag,
    const img_t* _responses, const img_t* _var_idx, const img_t* _sample_idx,
    const img_t* _var_type, const img_t* _missing_mask, const CvDTreeParams& _params,
    bool _shared, bool _add_labels, bool _update_data)
{
    img_t* sample_indices = 0;
    img_t* var_type0 = 0;
    img_t* tmp_map = 0;
    int** int_ptr = 0;
    CvPair16u32s* pair16u32s_ptr = 0;
    CvDTreeTrainData* data = 0;
    float *_fdst = 0;
    int *_idst = 0;
    unsigned short* udst = 0;
    int* idst = 0;

    CC_FUNCNAME("CvERTreeTrainData::set_data");

    __BEGIN__;

    int sample_all = 0, r_type, cv_n;
    int total_c_count = 0;
    int tree_block_size, temp_block_size, max_split_size, nv_size, cv_size = 0;
    int ds_step, dv_step, ms_step = 0, mv_step = 0; // {data|mask}{sample|var}_step
    int vi, i, size;
    char err[100];
    const int *sidx = 0, *vidx = 0;

    uint64 effective_buf_size = 0;
    int effective_buf_height = 0, effective_buf_width = 0;

    if (_params.use_surrogates)
        CC_ERROR(CC_StsBadArg, "CvERTrees do not support surrogate splits");

    if(_update_data && data_root)
    {
        CC_ERROR(CC_StsBadArg, "CvERTrees do not support data update");
    }

    clear();

    var_all = 0;
    rng = &theRNG();

    CC_CALL(set_params(_params));

    // check parameter types and sizes
    CC_CALL(cvCheckTrainData(_train_data, _tflag, _missing_mask, &var_all, &sample_all));

    train_data = _train_data;
    responses = _responses;
    missing_mask = _missing_mask;

    if(_tflag == CC_ROW_SAMPLE)
    {
        ds_step = _train_data->step/CC_ELEM_SIZE(_train_data->tid);
        dv_step = 1;
        if(_missing_mask)
            ms_step = _missing_mask->step, mv_step = 1;
    }
    else
    {
        dv_step = _train_data->step/CC_ELEM_SIZE(_train_data->tid);
        ds_step = 1;
        if(_missing_mask)
            mv_step = _missing_mask->step, ms_step = 1;
    }
    tflag = _tflag;

    sample_count = sample_all;
    var_count = var_all;

    if(_sample_idx)
    {
        CC_CALL(sample_indices = cvPreprocessIndexArray(_sample_idx, sample_all));
        sidx = sample_indices->tt.i;
        sample_count = sample_indices->rows + sample_indices->cols - 1;
    }

    if(_var_idx)
    {
        CC_CALL(var_idx = cvPreprocessIndexArray(_var_idx, var_all));
        vidx = var_idx->tt.i;
        var_count = var_idx->rows + var_idx->cols - 1;
    }

    if(!CC_IS_MAT(_responses) ||
        (CC_MAT_TYPE(_responses->tid) != CC_32SC1 &&
         CC_MAT_TYPE(_responses->tid) != CC_32F) ||
        (_responses->rows != 1 && _responses->cols != 1) ||
        _responses->rows + _responses->cols - 1 != sample_all)
        CC_ERROR(CC_StsBadArg, "The array of _responses must be an integer or "
                  "floating-point vector containing as many elements as "
                  "the total number of samples in the training data matrix");

    is_buf_16u = false;
    if (sample_count < 65536)
        is_buf_16u = true;

    r_type = CC_VAR_CATEGORICAL;
    if(_var_type)
        CC_CALL(var_type0 = cvPreprocessVarType(_var_type, var_idx, var_count, &r_type));

    CC_CALL(var_type = cvCreateMat(1, var_count+2, CC_32SC1));

    cat_var_count = 0;
    ord_var_count = -1;

    is_classifier = r_type == CC_VAR_CATEGORICAL;

    // step 0. calc the number of categorical vars
    for(vi = 0; vi < var_count; vi++)
    {
        char vt = var_type0 ? var_type0->tt.data[vi] : CC_VAR_ORDERED;
        var_type->tt.i[vi] = vt == CC_VAR_CATEGORICAL ? cat_var_count++ : ord_var_count--;
    }

    ord_var_count = ~ord_var_count;
    cv_n = params.cv_folds;
    // set the two last elements of var_type array to be able
    // to locate responses and cross-validation labels using
    // the corresponding get_* functions.
    var_type->tt.i[var_count] = cat_var_count;
    var_type->tt.i[var_count+1] = cat_var_count+1;

    // in case of single ordered predictor we need dummy cv_labels
    // for safe split_node_data() operation
    have_labels = cv_n > 0 || (ord_var_count == 1 && cat_var_count == 0) || _add_labels;

    work_var_count = cat_var_count + (is_classifier ? 1 : 0) + (have_labels ? 1 : 0);

    shared = _shared;
    buf_count = shared ? 2 : 1;

    buf_size = -1; // the member buf_size is obsolete

    effective_buf_size = (uint64)(work_var_count + 1)*(uint64)sample_count * buf_count; // this is the total size of "img_t buf" to be allocated
    effective_buf_width = sample_count;
    effective_buf_height = work_var_count+1;

    if (effective_buf_width >= effective_buf_height)
        effective_buf_height *= buf_count;
    else
        effective_buf_width *= buf_count;

    if ((uint64)effective_buf_width * (uint64)effective_buf_height != effective_buf_size)
    {
        CC_Error(CC_StsBadArg, "The memory buffer cannot be allocated since its size exceeds integer fields limit");
    }

    if (is_buf_16u)
    {
        CC_CALL(buf = cvCreateMat(effective_buf_height, effective_buf_width, CC_16UC1));
        CC_CALL(pair16u32s_ptr = (CvPair16u32s*)cAlloc(sample_count*sizeof(pair16u32s_ptr[0])));
    }
    else
    {
        CC_CALL(buf = cvCreateMat(effective_buf_height, effective_buf_width, CC_32SC1));
        CC_CALL(int_ptr = (int**)cAlloc(sample_count*sizeof(int_ptr[0])));
    }

    size = is_classifier ? cat_var_count+1 : cat_var_count;
    size = !size ? 1 : size;
    CC_CALL(cat_count = cvCreateMat(1, size, CC_32SC1));
    CC_CALL(cat_ofs = cvCreateMat(1, size, CC_32SC1));

    size = is_classifier ? (cat_var_count + 1)*params.max_categories : cat_var_count*params.max_categories;
    size = !size ? 1 : size;
    CC_CALL(cat_map = cvCreateMat(1, size, CC_32SC1));

    // now calculate the maximum size of split,
    // create memory storage that will keep nodes and splits of the decision tree
    // allocate root node and the buffer for the whole training data
    max_split_size = cvAlign(sizeof(CvDTreeSplit) +
        (MAX(0,sample_count - 33)/32)*sizeof(int),sizeof(void*));
    tree_block_size = MAX((int)sizeof(CvDTreeNode)*8, max_split_size);
    tree_block_size = MAX(tree_block_size + block_size_delta, min_block_size);
    CC_CALL(tree_storage = cvCreateMemStorage(tree_block_size));
    CC_CALL(node_heap = cvCreateSet(0, sizeof(*node_heap), sizeof(CvDTreeNode), tree_storage));

    nv_size = var_count*sizeof(int);
    nv_size = cvAlign(MAX(nv_size, (int)sizeof(CvSetElem)), sizeof(void*));

    temp_block_size = nv_size;

    if(cv_n)
    {
        if(sample_count < cv_n*MAX(params.min_sample_count,10))
            CC_ERROR(CC_StsOutOfRange,
                "The many folds in cross-validation for such a small dataset");

        cv_size = cvAlign(cv_n*(sizeof(int) + sizeof(double)*2), sizeof(double));
        temp_block_size = MAX(temp_block_size, cv_size);
    }

    temp_block_size = MAX(temp_block_size + block_size_delta, min_block_size);
    CC_CALL(temp_storage = cvCreateMemStorage(temp_block_size));
    CC_CALL(nv_heap = cvCreateSet(0, sizeof(*nv_heap), nv_size, temp_storage));
    if(cv_size)
        CC_CALL(cv_heap = cvCreateSet(0, sizeof(*cv_heap), cv_size, temp_storage));

    CC_CALL(data_root = new_node(0, sample_count, 0, 0));

    max_c_count = 1;

    _fdst = 0;
    _idst = 0;
    if (ord_var_count)
        _fdst = (float*)cAlloc(sample_count*sizeof(_fdst[0]));
    if (is_buf_16u && (cat_var_count || is_classifier))
        _idst = (int*)cAlloc(sample_count*sizeof(_idst[0]));

    // transform the training data to convenient representation
    for(vi = 0; vi <= var_count; vi++)
    {
        int ci;
        const uchar* mask = 0;
        int m_step = 0, step;
        const int* idata = 0;
        const float* fdata = 0;
        int num_valid = 0;

        if(vi < var_count) // analyze i-th input variable
        {
            int vi0 = vidx ? vidx[vi] : vi;
            ci = get_var_type(vi);
            step = ds_step; m_step = ms_step;
            if(CC_MAT_TYPE(_train_data->tid) == CC_32SC1)
                idata = _train_data->tt.i + vi0*dv_step;
            else
                fdata = _train_data->tt.fl + vi0*dv_step;
            if(_missing_mask)
                mask = _missing_mask->tt.data + vi0*mv_step;
        }
        else // analyze _responses
        {
            ci = cat_var_count;
            step = CC_IS_MAT_CONT(_responses->tid) ?
                1 : _responses->step / CC_ELEM_SIZE(_responses->tid);
            if(CC_MAT_TYPE(_responses->tid) == CC_32SC1)
                idata = _responses->tt.i;
            else
                fdata = _responses->tt.fl;
        }

        if((vi < var_count && ci>=0) ||
            (vi == var_count && is_classifier)) // process categorical variable or response
        {
            int c_count, prev_label;
            int* c_map;

            if (is_buf_16u)
                udst = (unsigned short*)(buf->tt.s + ci*sample_count);
            else
                idst = buf->tt.i + ci*sample_count;

            // copy data
            for(i = 0; i < sample_count; i++)
            {
                int val = INT_MAX, si = sidx ? sidx[i] : i;
                if(!mask || !mask[(size_t)si*m_step])
                {
                    if(idata)
                        val = idata[(size_t)si*step];
                    else
                    {
                        float t = fdata[(size_t)si*step];
                        val = cRound(t);
                        if(val != t)
                        {
                            sprintf(err, "%d-th value of %d-th (categorical) "
                                "variable is not an integer", i, vi);
                            CC_ERROR(CC_StsBadArg, err);
                        }
                    }

                    if(val == INT_MAX)
                    {
                        sprintf(err, "%d-th value of %d-th (categorical) "
                            "variable is too large", i, vi);
                        CC_ERROR(CC_StsBadArg, err);
                    }
                    num_valid++;
                }
                if (is_buf_16u)
                {
                    _idst[i] = val;
                    pair16u32s_ptr[i].u = udst + i;
                    pair16u32s_ptr[i].i = _idst + i;
                }
                else
                {
                    idst[i] = val;
                    int_ptr[i] = idst + i;
                }
            }

            c_count = num_valid > 0;

            if (is_buf_16u)
            {
                icvSortPairs(pair16u32s_ptr, sample_count, 0);
                // count the categories
                for(i = 1; i < num_valid; i++)
                    if (*pair16u32s_ptr[i].i != *pair16u32s_ptr[i-1].i)
                        c_count ++ ;
            }
            else
            {
                icvSortIntPtr(int_ptr, sample_count, 0);
                // count the categories
                for(i = 1; i < num_valid; i++)
                    c_count += *int_ptr[i] != *int_ptr[i-1];
            }

            if(vi > 0)
                max_c_count = MAX(max_c_count, c_count);
            cat_count->tt.i[ci] = c_count;
            cat_ofs->tt.i[ci] = total_c_count;

            // resize cat_map, if need
            if(cat_map->cols < total_c_count + c_count)
            {
                tmp_map = cat_map;
                CC_CALL(cat_map = cvCreateMat(1,
                    MAX(cat_map->cols*3/2,total_c_count+c_count), CC_32SC1));
                for(i = 0; i < total_c_count; i++)
                    cat_map->tt.i[i] = tmp_map->tt.i[i];
                cvReleaseMat(&tmp_map);
            }

            c_map = cat_map->tt.i + total_c_count;
            total_c_count += c_count;

            c_count = -1;
            if (is_buf_16u)
            {
                // compact the class indices and build the map
                prev_label = ~*pair16u32s_ptr[0].i;
                for(i = 0; i < num_valid; i++)
                {
                    int cur_label = *pair16u32s_ptr[i].i;
                    if(cur_label != prev_label)
                        c_map[++c_count] = prev_label = cur_label;
                    *pair16u32s_ptr[i].u = (unsigned short)c_count;
                }
                // replace labels for missing values with 65535
                for(; i < sample_count; i++)
                    *pair16u32s_ptr[i].u = 65535;
            }
            else
            {
                // compact the class indices and build the map
                prev_label = ~*int_ptr[0];
                for(i = 0; i < num_valid; i++)
                {
                    int cur_label = *int_ptr[i];
                    if(cur_label != prev_label)
                        c_map[++c_count] = prev_label = cur_label;
                    *int_ptr[i] = c_count;
                }
                // replace labels for missing values with -1
                for(; i < sample_count; i++)
                    *int_ptr[i] = -1;
            }
        }
        else if(ci < 0) // process ordered variable
        {
            for(i = 0; i < sample_count; i++)
            {
                float val = ord_nan;
                int si = sidx ? sidx[i] : i;
                if(!mask || !mask[(size_t)si*m_step])
                {
                    if(idata)
                        val = (float)idata[(size_t)si*step];
                    else
                        val = fdata[(size_t)si*step];

                    if(fabs(val) >= ord_nan)
                    {
                        sprintf(err, "%d-th value of %d-th (ordered) "
                            "variable (=%g) is too large", i, vi, val);
                        CC_ERROR(CC_StsBadArg, err);
                    }
                    num_valid++;
                }
            }
        }
        if(vi < var_count)
            data_root->set_num_valid(vi, num_valid);
    }

    // set sample labels
    if (is_buf_16u)
        udst = (unsigned short*)(buf->tt.s + get_work_var_count()*sample_count);
    else
        idst = buf->tt.i + get_work_var_count()*sample_count;

    for (i = 0; i < sample_count; i++)
    {
        if (udst)
            udst[i] = sidx ? (unsigned short)sidx[i] : (unsigned short)i;
        else
            idst[i] = sidx ? sidx[i] : i;
    }

    if(cv_n)
    {
        unsigned short* usdst = 0;
        int* idst2 = 0;

        if (is_buf_16u)
        {
            usdst = (unsigned short*)(buf->tt.s + (get_work_var_count()-1)*sample_count);
            for(i = vi = 0; i < sample_count; i++)
            {
                usdst[i] = (unsigned short)vi++;
                vi &= vi < cv_n ? -1 : 0;
            }

            for(i = 0; i < sample_count; i++)
            {
                int a = (*rng)(sample_count);
                int b = (*rng)(sample_count);
                unsigned short unsh = (unsigned short)vi;
                CC_SWAP(usdst[a], usdst[b], unsh);
            }
        }
        else
        {
            idst2 = buf->tt.i + (get_work_var_count()-1)*sample_count;
            for(i = vi = 0; i < sample_count; i++)
            {
                idst2[i] = vi++;
                vi &= vi < cv_n ? -1 : 0;
            }

            for(i = 0; i < sample_count; i++)
            {
                int a = (*rng)(sample_count);
                int b = (*rng)(sample_count);
                CC_SWAP(idst2[a], idst2[b], vi);
            }
        }
    }

    if (cat_map)
        cat_map->cols = MAX(total_c_count, 1);

    max_split_size = cvAlign(sizeof(CvDTreeSplit) +
        (MAX(0,max_c_count - 33)/32)*sizeof(int),sizeof(void*));
    CC_CALL(split_heap = cvCreateSet(0, sizeof(*split_heap), max_split_size, tree_storage));

    have_priors = is_classifier && params.priors;
    if(is_classifier)
    {
        int m = get_num_classes();
        double sum = 0;
        CC_CALL(priors = cvCreateMat(1, m, CC_64F));
        for(i = 0; i < m; i++)
        {
            double val = have_priors ? params.priors[i] : 1.;
            if(val <= 0)
                CC_ERROR(CC_StsOutOfRange, "Every class weight should be positive");
            priors->tt.db[i] = val;
            sum += val;
        }

        // normalize weights
        if(have_priors)
            cvScale(priors, priors, 1./sum);

        CC_CALL(priors_mult = cvCloneMat(priors));
        CC_CALL(counts = cvCreateMat(1, m, CC_32SC1));
    }

    CC_CALL(direction = cvCreateMat(1, sample_count, CC_8UC1));
    CC_CALL(split_buf = cvCreateMat(1, sample_count, CC_32SC1));

    __END__;

    if(data)
        delete data;

    if (_fdst)
        cFree(&_fdst);
    if (_idst)
        cFree(&_idst);
    cFree(&int_ptr);
    cvReleaseMat(&var_type0);
    cvReleaseMat(&sample_indices);
    cvReleaseMat(&tmp_map);
}

void CvERTreeTrainData::get_ord_var_data(CvDTreeNode* n, int vi, float* ord_values_buf, int* missing_buf,
                                          const float** ord_values, const int** missing, int* sample_indices_buf)
{
    int vidx = var_idx ? var_idx->tt.i[vi] : vi;
    int node_sample_count = n->sample_count;
    // may use missing_buf as buffer for sample indices!
    const int* sample_indices = get_sample_indices(n, sample_indices_buf ? sample_indices_buf : missing_buf);

    int td_step = train_data->step/CC_ELEM_SIZE(train_data->tid);
    int m_step = missing_mask ? missing_mask->step/CC_ELEM_SIZE(missing_mask->tid) : 1;
    if(tflag == CC_ROW_SAMPLE)
    {
        for(int i = 0; i < node_sample_count; i++)
        {
            int idx = sample_indices[i];
            missing_buf[i] = missing_mask ? *(missing_mask->tt.data + idx * m_step + vi) : 0;
            ord_values_buf[i] = *(train_data->tt.fl + idx * td_step + vidx);
        }
    }
    else
        for(int i = 0; i < node_sample_count; i++)
        {
            int idx = sample_indices[i];
            missing_buf[i] = missing_mask ? *(missing_mask->tt.data + vi* m_step + idx) : 0;
            ord_values_buf[i] = *(train_data->tt.fl + vidx* td_step + idx);
        }
    *ord_values = ord_values_buf;
    *missing = missing_buf;
}


const int* CvERTreeTrainData::get_sample_indices(CvDTreeNode* n, int* indices_buf)
{
    return get_cat_var_data(n, var_count + (is_classifier ? 1 : 0) + (have_labels ? 1 : 0), indices_buf);
}


const int* CvERTreeTrainData::get_cv_labels(CvDTreeNode* n, int* labels_buf)
{
    if (have_labels)
        return get_cat_var_data(n, var_count + (is_classifier ? 1 : 0), labels_buf);
    return 0;
}


const int* CvERTreeTrainData::get_cat_var_data(CvDTreeNode* n, int vi, int* cat_values_buf)
{
    int ci = get_var_type(vi);
    const int* cat_values = 0;
    if(!is_buf_16u)
        cat_values = buf->tt.i + n->buf_idx*get_length_subbuf() + ci*sample_count + n->offset;
    else {
        const unsigned short* short_values = (const unsigned short*)(buf->tt.s + n->buf_idx*get_length_subbuf() +
            ci*sample_count + n->offset);
        for(int i = 0; i < n->sample_count; i++)
            cat_values_buf[i] = short_values[i];
        cat_values = cat_values_buf;
    }
    return cat_values;
}

void CvERTreeTrainData::get_vectors(const img_t* _subsample_idx,
                                    float* values, uchar* missing,
                                    float* _responses, bool get_class_idx)
{
    img_t* subsample_idx = 0;
    img_t* subsample_co = 0;

    CAutoBuffer<uchar> inn_buf(sample_count*(sizeof(float) + sizeof(int)));

    CC_FUNCNAME("CvERTreeTrainData::get_vectors");

    __BEGIN__;

    int i, vi, total = sample_count, count = total, cur_ofs = 0;
    int* sidx = 0;
    int* co = 0;

    if(_subsample_idx)
    {
        CC_CALL(subsample_idx = cvPreprocessIndexArray(_subsample_idx, sample_count));
        sidx = subsample_idx->tt.i;
        CC_CALL(subsample_co = cvCreateMat(1, sample_count*2, CC_32SC1));
        co = subsample_co->tt.i;
        cvZero(subsample_co);
        count = subsample_idx->cols + subsample_idx->rows - 1;
        for(i = 0; i < count; i++)
            co[sidx[i]*2]++;
        for(i = 0; i < total; i++)
        {
            int count_i = co[i*2];
            if(count_i)
            {
                co[i*2+1] = cur_ofs*var_count;
                cur_ofs += count_i;
            }
        }
    }

    if(missing)
        memset(missing, 1, count*var_count);

    for(vi = 0; vi < var_count; vi++)
    {
        int ci = get_var_type(vi);
        if(ci >= 0) // categorical
        {
            float* dst = values + vi;
            uchar* m = missing ? missing + vi : 0;
            int* lbls_buf = (int*)(uchar*)inn_buf;
            const int* src = get_cat_var_data(data_root, vi, lbls_buf);

            for(i = 0; i < count; i++, dst += var_count)
            {
                int idx = sidx ? sidx[i] : i;
                int val = src[idx];
                *dst = (float)val;
                if(m)
                {
                    *m = (!is_buf_16u && val < 0) || (is_buf_16u && (val == 65535));
                    m += var_count;
                }
            }
        }
        else // ordered
        {
            int* mis_buf = (int*)(uchar*)inn_buf;
            const float *dst = 0;
            const int* mis = 0;
            get_ord_var_data(data_root, vi, values + vi, mis_buf, &dst, &mis, 0);
            for (int si = 0; si < total; si++)
                *(missing + vi + si) = mis[si] == 0 ? 0 : 1;
        }
    }

    // copy responses
    if(_responses)
    {
        if(is_classifier)
        {
            int* lbls_buf = (int*)(uchar*)inn_buf;
            const int* src = get_class_labels(data_root, lbls_buf);
            for(i = 0; i < count; i++)
            {
                int idx = sidx ? sidx[i] : i;
                int val = get_class_idx ? src[idx] :
                    cat_map->tt.i[cat_ofs->tt.i[cat_var_count]+src[idx]];
                _responses[i] = (float)val;
            }
        }
        else
        {
            float* _values_buf = (float*)(uchar*)inn_buf;
            int* sample_idx_buf = (int*)(_values_buf + sample_count);
            const float* _values = get_ord_responses(data_root, _values_buf, sample_idx_buf);
            for(i = 0; i < count; i++)
            {
                int idx = sidx ? sidx[i] : i;
                _responses[i] = _values[idx];
            }
        }
    }

    __END__;

    cvReleaseMat(&subsample_idx);
    cvReleaseMat(&subsample_co);
}

CvDTreeNode* CvERTreeTrainData::subsample_data(const img_t* _subsample_idx)
{
    CvDTreeNode* root = 0;

    CC_FUNCNAME("CvERTreeTrainData::subsample_data");

    __BEGIN__;

    if(!data_root)
        CC_ERROR(CC_StsError, "No training data has been set");

    if(!_subsample_idx)
    {
        // make a copy of the root node
        CvDTreeNode temp;
        int i;
        root = new_node(0, 1, 0, 0);
        temp = *root;
        *root = *data_root;
        root->num_valid = temp.num_valid;
        if(root->num_valid)
        {
            for(i = 0; i < var_count; i++)
                root->num_valid[i] = data_root->num_valid[i];
        }
        root->cv_Tn = temp.cv_Tn;
        root->cv_node_risk = temp.cv_node_risk;
        root->cv_node_error = temp.cv_node_error;
    }
    else
        CC_ERROR(CC_StsError, "_subsample_idx must be null for extra-trees");
    __END__;

    return root;
}

double CvForestERTree::calc_node_dir(CvDTreeNode* node)
{
    char* dir = (char*)data->direction->tt.data;
    int i, n = node->sample_count, vi = node->split->var_idx;
    double L, R;

    assert(!node->split->inversed);

    if(data->get_var_type(vi) >= 0) // split on categorical var
    {
        CAutoBuffer<uchar> inn_buf(n*sizeof(int)*(!data->have_priors ? 1 : 2));
        int* labels_buf = (int*)(uchar*)inn_buf;
        const int* labels = data->get_cat_var_data(node, vi, labels_buf);
        const int* subset = node->split->subset;
        if(!data->have_priors)
        {
            int sum = 0, sum_abs = 0;

            for(i = 0; i < n; i++)
            {
                int idx = labels[i];
                int d = (((idx >= 0)&&(!data->is_buf_16u)) || ((idx != 65535)&&(data->is_buf_16u))) ?
                    CC_DTREE_CAT_DIR(idx,subset) : 0;
                sum += d; sum_abs += d & 1;
                dir[i] = (char)d;
            }

            R = (sum_abs + sum) >> 1;
            L = (sum_abs - sum) >> 1;
        }
        else
        {
            const double* priors = data->priors_mult->tt.db;
            double sum = 0, sum_abs = 0;
            int *responses_buf = labels_buf + n;
            const int* responses = data->get_class_labels(node, responses_buf);

            for(i = 0; i < n; i++)
            {
                int idx = labels[i];
                double w = priors[responses[i]];
                int d = idx >= 0 ? CC_DTREE_CAT_DIR(idx,subset) : 0;
                sum += d*w; sum_abs += (d & 1)*w;
                dir[i] = (char)d;
            }

            R = (sum_abs + sum) * 0.5;
            L = (sum_abs - sum) * 0.5;
        }
    }
    else // split on ordered var
    {
        float split_val = node->split->ord.c;
        CAutoBuffer<uchar> inn_buf(n*(sizeof(int)*(!data->have_priors ? 1 : 2) + sizeof(float)));
        float* val_buf = (float*)(uchar*)inn_buf;
        int* missing_buf = (int*)(val_buf + n);
        const float* val = 0;
        const int* missing = 0;
        data->get_ord_var_data(node, vi, val_buf, missing_buf, &val, &missing, 0);

        if(!data->have_priors)
        {
            L = R = 0;
            for(i = 0; i < n; i++)
            {
                if (missing[i])
                    dir[i] = (char)0;
                else
                {
                    if (val[i] < split_val)
                    {
                        dir[i] = (char)-1;
                        L++;
                    }
                    else
                    {
                        dir[i] = (char)1;
                        R++;
                    }
                }
            }
        }
        else
        {
            const double* priors = data->priors_mult->tt.db;
            int* responses_buf = missing_buf + n;
            const int* responses = data->get_class_labels(node, responses_buf);
            L = R = 0;
            for(i = 0; i < n; i++)
            {
                if (missing[i])
                    dir[i] = (char)0;
                else
                {
                    double w = priors[responses[i]];
                    if (val[i] < split_val)
                    {
                        dir[i] = (char)-1;
                         L += w;
                    }
                    else
                    {
                        dir[i] = (char)1;
                        R += w;
                    }
                }
            }
        }
    }

    node->maxlr = MAX(L, R);
    return node->split->quality/(L + R);
}

CvDTreeSplit* CvForestERTree::find_split_ord_class(CvDTreeNode* node, int vi, float init_quality, CvDTreeSplit* _split,
                                                    uchar* _ext_buf)
{
    const float epsilon = FLT_EPSILON*2;
    const float split_delta = (1 + FLT_EPSILON) * FLT_EPSILON;

    int n = node->sample_count;
    int m = data->get_num_classes();

    CAutoBuffer<uchar> inn_buf;
    if(!_ext_buf)
        inn_buf.allocate(n*(2*sizeof(int) + sizeof(float)));
    uchar* ext_buf = _ext_buf ? _ext_buf : (uchar*)inn_buf;
    float* values_buf = (float*)ext_buf;
    int* missing_buf = (int*)(values_buf + n);
    const float* values = 0;
    const int* missing = 0;
    data->get_ord_var_data(node, vi, values_buf, missing_buf, &values, &missing, 0);
    int* responses_buf = missing_buf + n;
    const int* responses = data->get_class_labels(node, responses_buf);

    double lbest_val = 0, rbest_val = 0, best_val = init_quality, split_val = 0;
    const double* priors = data->have_priors ? data->priors_mult->tt.db : 0;
    bool is_find_split = false;
    float pmin, pmax;
    int smpi = 0;
    while (missing[smpi] && (smpi < n))
        smpi++;
    assert(smpi < n);

    pmin = values[smpi];
    pmax = pmin;
    for (; smpi < n; smpi++)
    {
        float ptemp = values[smpi];
        int ms = missing[smpi];
        if (ms) continue;
        if (ptemp < pmin)
            pmin = ptemp;
        if (ptemp > pmax)
            pmax = ptemp;
    }
    float fdiff = pmax-pmin;
    if (fdiff > epsilon)
    {
        is_find_split = true;
        RNG* rng = data->rng;
        split_val = pmin + rng->uniform(0.f, 1.f) * fdiff ;
        if (split_val - pmin <= FLT_EPSILON)
            split_val = pmin + split_delta;
        if (pmax - split_val <= FLT_EPSILON)
            split_val = pmax - split_delta;

        // calculate Gini index
        if (!priors)
        {
            FREE(); int* = MALLOC(int, ) lrc(m*2);
            int *lc = lrc, *rc = lc + m;
            int L = 0, R = 0;

            // init arrays of class instance counters on both sides of the split
            for(int i = 0; i < m; i++)
            {
                lc[i] = 0;
                rc[i] = 0;
            }
            for(int si = 0; si < n; si++)
            {
                int r = responses[si];
                float val = values[si];
                int ms = missing[si];
                if (ms) continue;
                if (val < split_val)
                {
                    lc[r]++;
                    L++;
                }
                else
                {
                    rc[r]++;
                    R++;
                }
            }
            for (int i = 0; i < m; i++)
            {
                lbest_val += lc[i]*lc[i];
                rbest_val += rc[i]*rc[i];
            }
            best_val = (lbest_val*R + rbest_val*L) / ((double)(L*R));
        }
        else
        {
            FREE(); double* = MALLOC(double, ) lrc(m*2);
            double *lc = lrc, *rc = lc + m;
            double L = 0, R = 0;

            // init arrays of class instance counters on both sides of the split
            for(int i = 0; i < m; i++)
            {
                lc[i] = 0;
                rc[i] = 0;
            }
            for(int si = 0; si < n; si++)
            {
                int r = responses[si];
                float val = values[si];
                int ms = missing[si];
                double p = priors[r];
                if (ms) continue;
                if (val < split_val)
                {
                    lc[r] += p;
                    L += p;
                }
                else
                {
                    rc[r] += p;
                    R += p;
                }
            }
            for (int i = 0; i < m; i++)
            {
                lbest_val += lc[i]*lc[i];
                rbest_val += rc[i]*rc[i];
            }
            best_val = (lbest_val*R + rbest_val*L) / (L*R);
        }

    }

    CvDTreeSplit* split = 0;
    if(is_find_split)
    {
        split = _split ? _split : data->new_split_ord(0, 0.0f, 0, 0, 0.0f);
        split->var_idx = vi;
        split->ord.c = (float)split_val;
        split->ord.split_point = -1;
        split->inversed = 0;
        split->quality = (float)best_val;
    }
    return split;
}

CvDTreeSplit* CvForestERTree::find_split_cat_class(CvDTreeNode* node, int vi, float init_quality, CvDTreeSplit* _split,
                                                    uchar* _ext_buf)
{
    int ci = data->get_var_type(vi);
    int n = node->sample_count;
    int cm = data->get_num_classes();
    int vm = data->cat_count->tt.i[ci];
    double best_val = init_quality;
    CvDTreeSplit *split = 0;

    if (vm > 1)
    {
        FREE(); int* = MALLOC(int, ) inn_buf;
        if(!_ext_buf)
            inn_buf.allocate(2*n);
        int* ext_buf = _ext_buf ? (int*)_ext_buf : (int*)inn_buf;

        const int* labels = data->get_cat_var_data(node, vi, ext_buf);
        const int* responses = data->get_class_labels(node, ext_buf + n);

        const double* priors = data->have_priors ? data->priors_mult->tt.db : 0;

        // create random class mask
        FREE(); int* = MALLOC(int, ) valid_cidx(vm);
        for (int i = 0; i < vm; i++)
        {
            valid_cidx[i] = -1;
        }
        for (int si = 0; si < n; si++)
        {
            int c = labels[si];
            if (((c == 65535) && data->is_buf_16u) || ((c<0) && (!data->is_buf_16u)))
                continue;
            valid_cidx[c]++;
        }

        int valid_ccount = 0;
        for (int i = 0; i < vm; i++)
            if (valid_cidx[i] >= 0)
            {
                valid_cidx[i] = valid_ccount;
                valid_ccount++;
            }
        if (valid_ccount > 1)
        {
            CRNG* rng = forest->get_rng();
            int l_cval_count = 1 + cvRandInt(rng) % (valid_ccount-1);

            img_t* var_class_mask = cvCreateMat(1, valid_ccount, CC_8UC1);
            img_t submask;
            memset(var_class_mask->tt.data, 0, valid_ccount*CC_ELEM_SIZE(var_class_mask->tid));
            cvGetCols(var_class_mask, &submask, 0, l_cval_count);
            cvSet(&submask, cScalar(1));
            for (int i = 0; i < valid_ccount; i++)
            {
                uchar temp;
                int i1 =  cvRandInt(rng) % valid_ccount;
                int i2 = cvRandInt(rng) % valid_ccount;
                CC_SWAP(var_class_mask->tt.data[i1], var_class_mask->tt.data[i2], temp);
            }

            split = _split ? _split : data->new_split_cat(0, -1.0f);
            split->var_idx = vi;
            memset(split->subset, 0, (data->max_c_count + 31)/32 * sizeof(int));

            // calculate Gini index
            double lbest_val = 0, rbest_val = 0;
            if(!priors)
            {
                FREE(); int* = MALLOC(int, ) lrc(cm*2);
                int *lc = lrc, *rc = lc + cm;
                int L = 0, R = 0;
                // init arrays of class instance counters on both sides of the split
                for(int i = 0; i < cm; i++)
                {
                    lc[i] = 0;
                    rc[i] = 0;
                }
                for(int si = 0; si < n; si++)
                {
                    int r = responses[si];
                    int var_class_idx = labels[si];
                    if (((var_class_idx == 65535) && data->is_buf_16u) || ((var_class_idx<0) && (!data->is_buf_16u)))
                        continue;
                    int mask_class_idx = valid_cidx[var_class_idx];
                    if (var_class_mask->tt.data[mask_class_idx])
                    {
                        lc[r]++;
                        L++;
                        split->subset[var_class_idx >> 5] |= 1 << (var_class_idx & 31);
                    }
                    else
                    {
                        rc[r]++;
                        R++;
                    }
                }
                for (int i = 0; i < cm; i++)
                {
                    lbest_val += lc[i]*lc[i];
                    rbest_val += rc[i]*rc[i];
                }
                best_val = (lbest_val*R + rbest_val*L) / ((double)(L*R));
            }
            else
            {
                FREE(); int* = MALLOC(int, ) lrc(cm*2);
                int *lc = lrc, *rc = lc + cm;
                double L = 0, R = 0;
                // init arrays of class instance counters on both sides of the split
                for(int i = 0; i < cm; i++)
                {
                    lc[i] = 0;
                    rc[i] = 0;
                }
                for(int si = 0; si < n; si++)
                {
                    int r = responses[si];
                    int var_class_idx = labels[si];
                    if (((var_class_idx == 65535) && data->is_buf_16u) || ((var_class_idx<0) && (!data->is_buf_16u)))
                        continue;
                    double p = priors[si];
                    int mask_class_idx = valid_cidx[var_class_idx];

                    if (var_class_mask->tt.data[mask_class_idx])
                    {
                        lc[r]+=(int)p;
                        L+=p;
                        split->subset[var_class_idx >> 5] |= 1 << (var_class_idx & 31);
                    }
                    else
                    {
                        rc[r]+=(int)p;
                        R+=p;
                    }
                }
                for (int i = 0; i < cm; i++)
                {
                    lbest_val += lc[i]*lc[i];
                    rbest_val += rc[i]*rc[i];
                }
                best_val = (lbest_val*R + rbest_val*L) / (L*R);
            }
            split->quality = (float)best_val;

            cvReleaseMat(&var_class_mask);
        }
    }

    return split;
}

CvDTreeSplit* CvForestERTree::find_split_ord_reg(CvDTreeNode* node, int vi, float init_quality, CvDTreeSplit* _split,
                                                  uchar* _ext_buf)
{
    const float epsilon = FLT_EPSILON*2;
    const float split_delta = (1 + FLT_EPSILON) * FLT_EPSILON;
    int n = node->sample_count;
    CAutoBuffer<uchar> inn_buf;
    if(!_ext_buf)
        inn_buf.allocate(n*(2*sizeof(int) + 2*sizeof(float)));
    uchar* ext_buf = _ext_buf ? _ext_buf : (uchar*)inn_buf;
    float* values_buf = (float*)ext_buf;
    int* missing_buf = (int*)(values_buf + n);
    const float* values = 0;
    const int* missing = 0;
    data->get_ord_var_data(node, vi, values_buf, missing_buf, &values, &missing, 0);
    float* responses_buf =  (float*)(missing_buf + n);
    int* sample_indices_buf =  (int*)(responses_buf + n);
    const float* responses = data->get_ord_responses(node, responses_buf, sample_indices_buf);

    double best_val = init_quality, split_val = 0, lsum = 0, rsum = 0;
    int L = 0, R = 0;

    bool is_find_split = false;
    float pmin, pmax;
    int smpi = 0;
    while (missing[smpi] && (smpi < n))
        smpi++;

    assert(smpi < n);

    pmin = values[smpi];
    pmax = pmin;
    for (; smpi < n; smpi++)
    {
        float ptemp = values[smpi];
        int m = missing[smpi];
        if (m) continue;
        if (ptemp < pmin)
            pmin = ptemp;
        if (ptemp > pmax)
            pmax = ptemp;
    }
    float fdiff = pmax-pmin;
    if (fdiff > epsilon)
    {
        is_find_split = true;
        RNG* rng = data->rng;
        split_val = pmin + rng->uniform(0.f, 1.f) * fdiff ;
        if (split_val - pmin <= FLT_EPSILON)
            split_val = pmin + split_delta;
        if (pmax - split_val <= FLT_EPSILON)
            split_val = pmax - split_delta;

        for (int si = 0; si < n; si++)
        {
            float r = responses[si];
            float val = values[si];
            int m = missing[si];
            if (m) continue;
            if (val < split_val)
            {
                lsum += r;
                L++;
            }
            else
            {
                rsum += r;
                R++;
            }
        }
        best_val = (lsum*lsum*R + rsum*rsum*L)/((double)L*R);
    }

    CvDTreeSplit* split = 0;
    if(is_find_split)
    {
        split = _split ? _split : data->new_split_ord(0, 0.0f, 0, 0, 0.0f);
        split->var_idx = vi;
        split->ord.c = (float)split_val;
        split->ord.split_point = -1;
        split->inversed = 0;
        split->quality = (float)best_val;
    }
    return split;
}

CvDTreeSplit* CvForestERTree::find_split_cat_reg(CvDTreeNode* node, int vi, float init_quality, CvDTreeSplit* _split,
                                                  uchar* _ext_buf)
{
    int ci = data->get_var_type(vi);
    int n = node->sample_count;
    int vm = data->cat_count->tt.i[ci];
    double best_val = init_quality;
    CvDTreeSplit *split = 0;
    float lsum = 0, rsum = 0;

    if (vm > 1)
    {
        int base_size =  vm*sizeof(int);
        CAutoBuffer<uchar> inn_buf(base_size);
        if(!_ext_buf)
            inn_buf.allocate(base_size + n*(2*sizeof(int) + sizeof(float)));
        uchar* base_buf = (uchar*)inn_buf;
        uchar* ext_buf = _ext_buf ? _ext_buf : base_buf + base_size;
        int* labels_buf = (int*)ext_buf;
        const int* labels = data->get_cat_var_data(node, vi, labels_buf);
        float* responses_buf =  (float*)(labels_buf + n);
        int* sample_indices_buf = (int*)(responses_buf + n);
        const float* responses = data->get_ord_responses(node, responses_buf, sample_indices_buf);

        // create random class mask
        int *valid_cidx = (int*)base_buf;
        for (int i = 0; i < vm; i++)
        {
            valid_cidx[i] = -1;
        }
        for (int si = 0; si < n; si++)
        {
            int c = labels[si];
            if (((c == 65535) && data->is_buf_16u) || ((c<0) && (!data->is_buf_16u)))
                        continue;
            valid_cidx[c]++;
        }

        int valid_ccount = 0;
        for (int i = 0; i < vm; i++)
            if (valid_cidx[i] >= 0)
            {
                valid_cidx[i] = valid_ccount;
                valid_ccount++;
            }
        if (valid_ccount > 1)
        {
            CRNG* rng = forest->get_rng();
            int l_cval_count = 1 + cvRandInt(rng) % (valid_ccount-1);

            img_t* var_class_mask = cvCreateMat(1, valid_ccount, CC_8UC1);
            img_t submask;
            memset(var_class_mask->tt.data, 0, valid_ccount*CC_ELEM_SIZE(var_class_mask->tid));
            cvGetCols(var_class_mask, &submask, 0, l_cval_count);
            cvSet(&submask, cScalar(1));
            for (int i = 0; i < valid_ccount; i++)
            {
                uchar temp;
                int i1 = cvRandInt(rng) % valid_ccount;
                int i2 = cvRandInt(rng) % valid_ccount;
                CC_SWAP(var_class_mask->tt.data[i1], var_class_mask->tt.data[i2], temp);
            }

            split = _split ? _split : data->new_split_cat(0, -1.0f);
            split->var_idx = vi;
            memset(split->subset, 0, (data->max_c_count + 31)/32 * sizeof(int));

            int L = 0, R = 0;
            for(int si = 0; si < n; si++)
            {
                float r = responses[si];
                int var_class_idx = labels[si];
                if (((var_class_idx == 65535) && data->is_buf_16u) || ((var_class_idx<0) && (!data->is_buf_16u)))
                        continue;
                int mask_class_idx = valid_cidx[var_class_idx];
                if (var_class_mask->tt.data[mask_class_idx])
                {
                    lsum += r;
                    L++;
                    split->subset[var_class_idx >> 5] |= 1 << (var_class_idx & 31);
                }
                else
                {
                    rsum += r;
                    R++;
                }
            }
            best_val = (lsum*lsum*R + rsum*rsum*L)/((double)L*R);

            split->quality = (float)best_val;

            cvReleaseMat(&var_class_mask);
        }
    }

    return split;
}

void CvForestERTree::split_node_data(CvDTreeNode* node)
{
    int vi, i, n = node->sample_count, nl, nr, scount = data->sample_count;
    char* dir = (char*)data->direction->tt.data;
    CvDTreeNode *left = 0, *right = 0;
    int new_buf_idx = data->get_child_buf_idx(node);
    img_t* buf = data->buf;
    size_t length_buf_row = data->get_length_subbuf();
    FREE(); int* = MALLOC(int, ) temp_buf(n);

    complete_node_dir(node);

    for(i = nl = nr = 0; i < n; i++)
    {
        int d = dir[i];
        nr += d;
        nl += d^1;
    }

    bool split_input_data;
    node->left = left = data->new_node(node, nl, new_buf_idx, node->offset);
    node->right = right = data->new_node(node, nr, new_buf_idx, node->offset + nl);

    split_input_data = node->depth + 1 < data->params.max_type &&
        (node->left->sample_count > data->params.min_sample_count ||
        node->right->sample_count > data->params.min_sample_count);

    CAutoBuffer<uchar> inn_buf(n*(sizeof(int)+sizeof(float)));
    // split ordered vars
    for(vi = 0; vi < data->var_count; vi++)
    {
        int ci = data->get_var_type(vi);
        if (ci >= 0) continue;

        int n1 = node->get_num_valid(vi), nr1 = 0;
        float* values_buf = (float*)(uchar*)inn_buf;
        int* missing_buf = (int*)(values_buf + n);
        const float* values = 0;
        const int* missing = 0;
        data->get_ord_var_data(node, vi, values_buf, missing_buf, &values, &missing, 0);

        for(i = 0; i < n; i++)
            nr1 += ((!missing[i]) & dir[i]);
        left->set_num_valid(vi, n1 - nr1);
        right->set_num_valid(vi, nr1);
    }
    // split categorical vars, responses and cv_labels using new_idx relocation table
    for(vi = 0; vi < data->get_work_var_count() + data->ord_var_count; vi++)
    {
        int ci = data->get_var_type(vi);
        if (ci < 0) continue;

        int n1 = node->get_num_valid(vi), nr1 = 0;
        const int* src_lbls = data->get_cat_var_data(node, vi, (int*)(uchar*)inn_buf);

        for(i = 0; i < n; i++)
            temp_buf[i] = src_lbls[i];

        if (data->is_buf_16u)
        {
            unsigned short *ldst = (unsigned short *)(buf->tt.s + left->buf_idx*length_buf_row +
                ci*scount + left->offset);
            unsigned short *rdst = (unsigned short *)(buf->tt.s + right->buf_idx*length_buf_row +
                ci*scount + right->offset);

            for(i = 0; i < n; i++)
            {
                int d = dir[i];
                int idx = temp_buf[i];
                if (d)
                {
                    *rdst = (unsigned short)idx;
                    rdst++;
                    nr1 += (idx != 65535);
                }
                else
                {
                    *ldst = (unsigned short)idx;
                    ldst++;
                }
            }

            if(vi < data->var_count)
            {
                left->set_num_valid(vi, n1 - nr1);
                right->set_num_valid(vi, nr1);
            }
        }
        else
        {
            int *ldst = buf->tt.i + left->buf_idx*length_buf_row +
                ci*scount + left->offset;
            int *rdst = buf->tt.i + right->buf_idx*length_buf_row +
                ci*scount + right->offset;

            for(i = 0; i < n; i++)
            {
                int d = dir[i];
                int idx = temp_buf[i];
                if (d)
                {
                    *rdst = idx;
                    rdst++;
                    nr1 += (idx >= 0);
                }
                else
                {
                    *ldst = idx;
                    ldst++;
                }

            }

            if(vi < data->var_count)
            {
                left->set_num_valid(vi, n1 - nr1);
                right->set_num_valid(vi, nr1);
            }
        }
    }

    // split sample indices
    int *sample_idx_src_buf = (int*)(uchar*)inn_buf;
    const int* sample_idx_src = 0;
    if (split_input_data)
    {
        sample_idx_src = data->get_sample_indices(node, sample_idx_src_buf);

        for(i = 0; i < n; i++)
            temp_buf[i] = sample_idx_src[i];

        int pos = data->get_work_var_count();

        if (data->is_buf_16u)
        {
            unsigned short* ldst = (unsigned short*)(buf->tt.s + left->buf_idx*length_buf_row +
                pos*scount + left->offset);
            unsigned short* rdst = (unsigned short*)(buf->tt.s + right->buf_idx*length_buf_row +
                pos*scount + right->offset);

            for (i = 0; i < n; i++)
            {
                int d = dir[i];
                unsigned short idx = (unsigned short)temp_buf[i];
                if (d)
                {
                    *rdst = idx;
                    rdst++;
                }
                else
                {
                    *ldst = idx;
                    ldst++;
                }
            }
        }
        else
        {
            int* ldst = buf->tt.i + left->buf_idx*length_buf_row +
                pos*scount + left->offset;
            int* rdst = buf->tt.i + right->buf_idx*length_buf_row +
                pos*scount + right->offset;
            for (i = 0; i < n; i++)
            {
                int d = dir[i];
                int idx = temp_buf[i];
                if (d)
                {
                    *rdst = idx;
                    rdst++;
                }
                else
                {
                    *ldst = idx;
                    ldst++;
                }
            }
        }
    }

    // deallocate the parent node data that is not needed anymore
    data->free_node_data(node);
}

CvERTrees::CvERTrees()
{
}

CvERTrees::~CvERTrees()
{
}

std::string CvERTrees::getName() const
{
    return CC_TYPE_NAME_ML_ERTREES;
}

bool CvERTrees::train(const img_t* _train_data, int _tflag,
                        const img_t* _responses, const img_t* _var_idx,
                        const img_t* _sample_idx, const img_t* _var_type,
                        const img_t* _missing_mask, CvRTParams params)
{
    bool result = false;

    CC_FUNCNAME("CvERTrees::train");
    __BEGIN__
    int var_count = 0;

    clear();

    CvDTreeParams tree_params(params.max_type, params.min_sample_count,
        params.regression_accuracy, params.use_surrogates, params.max_categories,
        params.cv_folds, params.use_1se_rule, false, params.priors);

    data = new CvERTreeTrainData();
    CC_CALL(data->set_data(_train_data, _tflag, _responses, _var_idx,
        _sample_idx, _var_type, _missing_mask, tree_params, true));

    var_count = data->var_count;
    if(params.nactive_vars > var_count)
        params.nactive_vars = var_count;
    else if(params.nactive_vars == 0)
        params.nactive_vars = (int)sqrt((double)var_count);
    else if(params.nactive_vars < 0)
        CC_ERROR(CC_StsBadArg, "<nactive_vars> must be non-negative");

    // Create mask of active variables at the tree nodes
    CC_CALL(active_var_mask = cvCreateMat(1, var_count, CC_8UC1));
    if(params.calc_var_importance)
    {
        CC_CALL(var_importance  = cvCreateMat(1, var_count, CC_32FC1));
        cvZero(var_importance);
    }
    { // initialize active variables mask
        img_t submask1, submask2;
        CC_Assert((active_var_mask->cols >= 1) && (params.nactive_vars > 0) && (params.nactive_vars <= active_var_mask->cols));
        cvGetCols(active_var_mask, &submask1, 0, params.nactive_vars);
        cvSet(&submask1, cScalar(1));
        if(params.nactive_vars < active_var_mask->cols)
        {
            cvGetCols(active_var_mask, &submask2, params.nactive_vars, var_count);
            cvZero(&submask2);
        }
    }

    CC_CALL(result = grow_forest(params.term_crit));

    result = true;

    __END__
    return result;

}

bool CvERTrees::train(CvMLData* _data, CvRTParams params)
{
   bool result = false;

    CC_FUNCNAME("CvERTrees::train");

    __BEGIN__;

    CC_CALL(result = CvRTrees::train(_data, params));

    __END__;

    return result;
}

bool CvERTrees::grow_forest(const CTermCriteria term_crit)
{
    bool result = false;

    img_t* sample_idx_for_tree      = 0;

    CC_FUNCNAME("CvERTrees::grow_forest");
    __BEGIN__;

    const int max_ntrees = term_crit.max_iter;
    const double max_oob_err = term_crit.epsilon;

    const int dims = data->var_count;
    float maximal_response = 0;

    img_t* oob_sample_votes    = 0;
    img_t* oob_responses       = 0;

    float* oob_samples_perm_ptr= 0;

    float* samples_ptr     = 0;
    uchar* missing_ptr     = 0;
    float* true_resp_ptr   = 0;
    bool is_oob_or_vimportance = ((max_oob_err > 0) && (term_crit.type != CC_TERMCRIT_ITER)) || var_importance;

    // oob_predictions_sum[i] = sum of predicted values for the i-th sample
    // oob_num_of_predictions[i] = number of summands
    //                            (number of predictions for the i-th sample)
    // initialize these variable to avoid warning C4701
    img_t oob_predictions_sum = cvMat(1, 1, CC_32FC1);
    img_t oob_num_of_predictions = cvMat(1, 1, CC_32FC1);

    nsamples = data->sample_count;
    nclasses = data->get_num_classes();

    if (is_oob_or_vimportance)
    {
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
    }

    trees = (CvForestTree**)cAlloc(sizeof(trees[0])*max_ntrees);
    memset(trees, 0, sizeof(trees[0])*max_ntrees);

    CC_CALL(sample_idx_for_tree = cvCreateMat(1, nsamples, CC_32SC1));

    for (int i = 0; i < nsamples; i++)
        sample_idx_for_tree->tt.i[i] = i;
    ntrees = 0;
    while(ntrees < max_ntrees)
    {
        int i, oob_samples_count = 0;
        double ncorrect_responses = 0; // used for estimation of variable importance
        CvForestTree* tree = 0;

        trees[ntrees] = new CvForestERTree();
        tree = (CvForestERTree*)trees[ntrees];
        CC_CALL(tree->train(data, 0, this));

        if (is_oob_or_vimportance)
        {
            img_t sample, missing;
            // form array of OOB samples indices and get these samples
            sample   = cvMat(1, dims, CC_32FC1, samples_ptr);
            missing  = cvMat(1, dims, CC_8UC1,  missing_ptr);

            oob_error = 0;
            for(i = 0; i < nsamples; i++,
                sample->tt.fl += dims, missing->tt.data += dims)
            {
                CvDTreeNode* predicted_node = 0;

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

                        i1 = (*rng)(nsamples);
                        i2 = (*rng)(nsamples);
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
        }
        ntrees++;
        if(term_crit.type != CC_TERMCRIT_ITER && oob_error < max_oob_err)
            break;
    }
    if(var_importance)
    {
        for (int vi = 0; vi < var_importance->cols; vi++)
                var_importance->tt.fl[vi] = (var_importance->tt.fl[vi] > 0) ?
                    var_importance->tt.fl[vi] : 0;
        cvNormalize(var_importance, var_importance, 1., 0, CC_L1);
    }

    result = true;

    cFree(&oob_samples_perm_ptr);
    cFree(&samples_ptr);
    cFree(&missing_ptr);
    cFree(&true_resp_ptr);

    cvReleaseMat(&sample_idx_for_tree);

    cvReleaseMat(&oob_sample_votes);
    cvReleaseMat(&oob_responses);

    __END__;

    return result;
}

using namespace cv;

bool CvERTrees::train(const img_t& _train_data, int _tflag,
                      const img_t& _responses, const img_t& _var_idx,
                      const img_t& _sample_idx, const img_t& _var_type,
                      const img_t& _missing_mask, CvRTParams params)
{
    img_t tdata = _train_data, responses = _responses, vidx = _var_idx,
    sidx = _sample_idx, vtype = _var_type, mmask = _missing_mask;
    return train(&tdata, _tflag, &responses, vidx->tt.data ? &vidx : 0,
                 sidx->tt.data ? &sidx : 0, vtype->tt.data ? &vtype : 0,
                 mmask->tt.data ? &mmask : 0, params);
}

// End of file.
