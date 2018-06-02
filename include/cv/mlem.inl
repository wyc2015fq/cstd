/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright(C) 2000, Intel Corporation, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
//(including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort(including negligence or otherwise) arising in any way out of
// the use of this software, even ifadvised of the possibility of such damage.
//
//M*/

#include "_ml.h"


/*
   CvEM:
 * params.nclusters    - number of clusters to cluster samples to.
 * means               - calculated by the EM algorithm set of gaussians' means.
 * log_weight_div_det - auxilary vector that k-th component is equal to
                        (-2)*ln(weights_k/det(Sigma_k)^0.5),
                        where <weights_k> is the weight,
                        <Sigma_k> is the covariation matrice of k-th cluster.
 * inv_eigen_values   - set of 1*dims matrices, <inv_eigen_values>[k] contains
                        inversed eigen values of covariation matrice of the k-th cluster.
                        In the case of <cov_mat_type> == COV_MAT_DIAGONAL,
                        inv_eigen_values[k] = Sigma_k^(-1).
 * covs_rotate_mats   - used only if cov_mat_type == COV_MAT_GENERIC, in all the
                        other cases it is NULL. <covs_rotate_mats>[k] is the orthogonal
                        matrice, obtained by the CSVD-decomposition of Sigma_k.
   Both <inv_eigen_values> and <covs_rotate_mats> fields are used for representation of
   covariation matrices and simplifying EM calculations.
   For fixed k denote
   u = covs_rotate_mats[k],
   v = inv_eigen_values[k],
   w = v^(-1);
   if <cov_mat_type> == COV_MAT_GENERIC, then Sigma_k = u w u',
   else                                       Sigma_k = w.
   Symbol ' means transposition.
 */


CvEM::CvEM()
{
    means = weights = probs = inv_eigen_values = log_weight_div_det = 0;
    covs = cov_rotate_mats = 0;
}


CvEM::~CvEM()
{
    clear();
}


void CvEM::clear()
{
    int i;
    
    cvReleaseMat(&means);
    cvReleaseMat(&weights);
    cvReleaseMat(&probs);
    cvReleaseMat(&inv_eigen_values);
    cvReleaseMat(&log_weight_div_det);

    if(covs || cov_rotate_mats)
    {
        for(i = 0; i < params.nclusters; i++)
        {
            if(covs)
                cvReleaseMat(&covs[i]);
            if(cov_rotate_mats)
                cvReleaseMat(&cov_rotate_mats[i]);
        }
        cFree(&covs);
        cFree(&cov_rotate_mats);
    }
}


void CvEM::set_params(const CvEMParams& _params, const CvVectors& train_data)
{
    CC_FUNCNAME("CvEM::set_params");

    __BEGIN__;
    
    int k;

    params = _params;
    params.term_crit = cvCheckTermCriteria(params.term_crit, 1e-6, 10000);

    if(params.cov_mat_type != COV_MAT_SPHERICAL &&
        params.cov_mat_type != COV_MAT_DIAGONAL &&
        params.cov_mat_type != COV_MAT_GENERIC)
        CC_ERROR(CC_StsBadArg, "Unknown covariation matrix type");

    switch(params.start_step)
    {
    case START_M_STEP:
        if(!params.probs)
            CC_ERROR(CC_StsNullPtr, "Probabilities must be specified when EM algorithm starts with M-step");
        break;
    case START_E_STEP:
        if(!params.means)
            CC_ERROR(CC_StsNullPtr, "Mean's must be specified when EM algorithm starts with E-step");
        break;
    case START_AUTO_STEP:
        break;
    default:
        CC_ERROR(CC_StsBadArg, "Unknown start_step");
    }

    if(params.nclusters < 1)
        CC_ERROR(CC_StsOutOfRange, "The number of clusters (mixtures) should be > 0");

    if(params.probs)
    {
        const img_t* p = params.weights;
        if(!CC_IS_MAT(p) ||
            CC_MAT_TYPE(p->tid) != CC_32F  &&
            CC_MAT_TYPE(p->tid) != CC_64F ||
            p->rows != train_data.count ||
            p->cols != params.nclusters)
            CC_ERROR(CC_StsBadArg, "The array of probabilities must be a valid "
            "floating-point matrix (img_t) of 'nsamples' x 'nclusters' size");
    }

    if(params.means)
    {
        const img_t* m = params.means;
        if(!CC_IS_MAT(m) ||
            CC_MAT_TYPE(m->tid) != CC_32F  &&
            CC_MAT_TYPE(m->tid) != CC_64F ||
            m->rows != params.nclusters ||
            m->cols != train_data.dims)
            CC_ERROR(CC_StsBadArg, "The array of mean's must be a valid "
            "floating-point matrix (img_t) of 'nsamples' x 'dims' size");
    }

    if(params.weights)
    {
        const img_t* w = params.weights;
        if(!CC_IS_MAT(w) ||
            CC_MAT_TYPE(w->tid) != CC_32F  &&
            CC_MAT_TYPE(w->tid) != CC_64F ||
            w->rows != 1 && w->cols != 1 ||
            w->rows + w->cols - 1 != params.nclusters)
            CC_ERROR(CC_StsBadArg, "The array of weights must be a valid "
            "1d floating-point vector (img_t) of 'nclusters' elements");
    }

    if(params.covs)
        for(k = 0; k < params.nclusters; k++)
        {
            const img_t* cov = params.covs[k];
            if(!CC_IS_MAT(cov) ||
                CC_MAT_TYPE(cov->tid) != CC_32F  &&
                CC_MAT_TYPE(cov->tid) != CC_64F ||
                cov->rows != cov->cols || cov->cols != train_data.dims)
                CC_ERROR(CC_StsBadArg,
                "Each of covariation matrices must be a valid square "
                "floating-point matrix (img_t) of 'dims' x 'dims'");
        }

    __END__;
}


/****************************************************************************************/
float
CvEM::predict(const img_t* _sample, img_t* _probs) const
{
    float* sample_data   = 0;
    void* buffer = 0;
    int allocated_buffer = 0;
    int cls = 0;

    CC_FUNCNAME("CvEM::predict");
    __BEGIN__;

    int i, k, dims;
    int nclusters;
    int cov_mat_type = params.cov_mat_type;
    double opt = FLT_MAX;
    size_t size;
    img_t diff, expo;

    dims = means->cols;
    nclusters = params.nclusters;

    CC_CALL(cvPreparePredictData(_sample, dims, 0, params.nclusters, _probs, &sample_data));

// allocate memory and initializing headers for calculating
    size = sizeof(double) * (nclusters + dims);
    if(size <= CC_MAX_LOCAL_SIZE)
        buffer = cvStackAlloc(size);
    else
    {
        CC_CALL(buffer = cAlloc(size));
        allocated_buffer = 1;
    }
    expo = cvMat(1, nclusters, CC_64FC1, buffer);
    diff = cvMat(1, dims, CC_64FC1, (double*)buffer + nclusters);

// calculate the probabilities
    for(k = 0; k < nclusters; k++)
    {
        const double* mean_k = (const double*)(means->tt.data + means->step*k);
        const double* w = (const double*)(inv_eigen_values->tt.data + inv_eigen_values->step*k);
        double cur = log_weight_div_det->tt.db[k];
        img_t* u = cov_rotate_mats[k];
        // cov = u w u'  -->  cov^(-1) = u w^(-1) u'
        if(cov_mat_type == COV_MAT_SPHERICAL)
        {
            double w0 = w[0];
            for(i = 0; i < dims; i++)
            {
                double val = sample_data[i] - mean_k[i];
                cur += val*val*w0;
            }
        }
        else
        {
            for(i = 0; i < dims; i++)
                diff->tt.db[i] = sample_data[i] - mean_k[i];
            if(cov_mat_type == COV_MAT_GENERIC)
                cvGEMM(&diff, u, 1, 0, 0, &diff, CC_GEMM_B_T);
            for(i = 0; i < dims; i++)
            {
                double val = diff->tt.db[i];
                cur += val*val*w[i];
            }
        }

        expo->tt.db[k] = cur;
        if(cur < opt)
        {
            cls = k;
            opt = cur;
        }
        /* probability = (2*pi)^(-dims/2)*exp(-0.5 * cur) */
    }

    if(_probs)
    {
        CC_CALL(cvConvertScale(&expo, &expo, -0.5));
        CC_CALL(cvExp(&expo, &expo));
        if(_probs->cols == 1)
            CC_CALL(cvReshape(&expo, &expo, 0, nclusters));
        CC_CALL(cvConvertScale(&expo, _probs, 1./cvSum(&expo).val[0]));
    }

    __END__;

    if(sample_data != _sample->tt.fl)
        cFree(&sample_data);
    if(allocated_buffer)
        cFree(&buffer);

    return (float)cls;
}



bool CvEM::train(const img_t* _samples, const img_t* _sample_idx,
                  CvEMParams _params, img_t* labels)
{
    bool result = false;
    CvVectors train_data;
    img_t* sample_idx = 0;

    train_data->tt.fl = 0;
    train_data.count = 0;

    CC_FUNCNAME("cvEM");

    __BEGIN__;

    int i, nsamples, nclusters, dims;

    clear();

    CC_CALL(cvPrepareTrainData("cvEM",
        _samples, CC_ROW_SAMPLE, 0, CC_VAR_CATEGORICAL,
        0, _sample_idx, false, (const float***)&train_data->tt.fl,
        &train_data.count, &train_data.dims, &train_data.dims,
        0, 0, 0, &sample_idx));

    CC_CALL(set_params(_params, train_data));
    nsamples = train_data.count;
    nclusters = params.nclusters;
    dims = train_data.dims;

    if(labels && (!CC_IS_MAT(labels) || CC_MAT_TYPE(labels->tid) != CC_32SC1 ||
        labels->cols != 1 && labels->rows != 1 || labels->cols + labels->rows - 1 != nsamples))
        CC_ERROR(CC_StsBadArg,
        "labels array (when passed) must be a valid 1d integer vector of <sample_count> elements");

    if(nsamples <= nclusters)
        CC_ERROR(CC_StsOutOfRange,
        "The number of samples should be greater than the number of clusters");

    CC_CALL(log_weight_div_det = cvCreateMat(1, nclusters, CC_64FC1));
    CC_CALL(probs  = cvCreateMat(nsamples, nclusters, CC_64FC1));
    CC_CALL(means = cvCreateMat(nclusters, dims, CC_64FC1));
    CC_CALL(weights = cvCreateMat(1, nclusters, CC_64FC1));
    CC_CALL(inv_eigen_values = cvCreateMat(nclusters,
        params.cov_mat_type == COV_MAT_SPHERICAL ? 1 : dims, CC_64FC1));
    CC_CALL(covs = (img_t**)cAlloc(nclusters * sizeof(*covs)));
    CC_CALL(cov_rotate_mats = (img_t**)cAlloc(nclusters * sizeof(cov_rotate_mats[0])));

    for(i = 0; i < nclusters; i++)
    {
        CC_CALL(covs[i] = cvCreateMat(dims, dims, CC_64FC1));
        CC_CALL(cov_rotate_mats[i]  = cvCreateMat(dims, dims, CC_64FC1));
        cvZero(cov_rotate_mats[i]);
    }

    init_em(train_data);
    log_likelihood = run_em(train_data);
    if(log_likelihood <= -DBL_MAX/10000.)
        EXIT;

    if(labels)
    {
        if(nclusters == 1)
            cvZero(labels);
        else
        {
            img_t sample = cvMat(1, dims, CC_32F);
            img_t prob = cvMat(1, nclusters, CC_64F);
            int lstep = CC_IS_MAT_CONT(labels->tid) ? 1 : labels->step/sizeof(int);
            
            for(i = 0; i < nsamples; i++)
            {
                int idx = sample_idx ? sample_idx->tt.i[i] : i;
                sample->tt.data = _samples->tt.data + _samples->step*idx;
                prob->tt.data = probs->tt.data + probs->step*i;

                labels->tt.i[i*lstep] = cRound(predict(&sample, &prob));
            }
        }
    }

    result = true;

    __END__;

    if(sample_idx != _sample_idx)
        cvReleaseMat(&sample_idx);

    cFree(&train_data->tt.data);

    return result;
}


void CvEM::init_em(const CvVectors& train_data)
{
    img_t *w = 0, *u = 0, *tcov = 0;
    
    CC_FUNCNAME("CvEM::init_em");

    __BEGIN__;

    double maxval = 0;
    int i, force_symm_plus = 0;
    int nclusters = params.nclusters, nsamples = train_data.count, dims = train_data.dims;

    if(params.start_step == START_AUTO_STEP || nclusters == 1 || nclusters == nsamples)
        init_auto(train_data);
    else if(params.start_step == START_M_STEP)
    {
        for(i = 0; i < nsamples; i++)
        {
            img_t prob;
            cvGetRow(params.probs, &prob, i);
            cvMaxS(&prob, 0., &prob);
            cvMinMaxLoc(&prob, 0, &maxval);
            if(maxval < FLT_EPSILON)
                cvSet(&prob, cScalar(1./nclusters));
            else
                cvNormalize(&prob, &prob, 1., 0, CC_L1);
        }
        EXIT; // do not preprocess covariation matrices,
              // as in this case they are initialized at the first iteration of EM
    }
    else
    {
        CC_ASSERT(params.start_step == START_E_STEP && params.means);
        if(params.weights && params.covs)
        {
            cvConvert(params.means, means);
            cvReshape(weights, weights, 1, params.weights->rows);
            cvConvert(params.weights, weights);
            cvReshape(weights, weights, 1, 1);
            cvMaxS(weights, 0., weights);
            cvMinMaxLoc(weights, 0, &maxval);
            if(maxval < FLT_EPSILON)
                cvSet(&weights, cScalar(1./nclusters));
            cvNormalize(weights, weights, 1., 0, CC_L1);
            for(i = 0; i < nclusters; i++)
                CC_CALL(cvConvert(params.covs[i], covs[i]));
            force_symm_plus = 1;
        }
        else
            init_auto(train_data);
    }

    CC_CALL(tcov = cvCreateMat(dims, dims, CC_64FC1));
    CC_CALL(w = cvCreateMat(dims, dims, CC_64FC1));
    if(params.cov_mat_type == COV_MAT_GENERIC)
        CC_CALL(u = cvCreateMat(dims, dims, CC_64FC1));

    for(i = 0; i < nclusters; i++)
    {
        if(force_symm_plus)
        {
            cvTranspose(covs[i], tcov);
            cvAddWeighted(covs[i], 0.5, tcov, 0.5, 0, tcov);
        }
        else
            cvCopy(covs[i], tcov);
        cvSVD(tcov, w, u, 0, CC_SVD_MODIFY_A + CC_SVD_U_T + CC_SVD_V_T);
        if(params.cov_mat_type == COV_MAT_SPHERICAL)
            cvSetIdentity(covs[i], cScalar(cvTrace(w).val[0]/dims));
        else if(params.cov_mat_type == COV_MAT_DIAGONAL)
            cvCopy(w, covs[i]);
        else
        {
            // generic case: covs[i] = (u')'*max(w,0)*u'
            cvGEMM(u, w, 1, 0, 0, tcov, CC_GEMM_A_T);
            cvGEMM(tcov, u, 1, 0, 0, covs[i], 0);
        }
    }

    __END__;

    cvReleaseMat(&w);
    cvReleaseMat(&u);
    cvReleaseMat(&tcov);
}


void CvEM::init_auto(const CvVectors& train_data)
{
    img_t* hdr = 0;
    const void** vec = 0;
    img_t* class_ranges = 0;
    img_t* labels = 0;

    CC_FUNCNAME("CvEM::init_auto");

    __BEGIN__;

    int nclusters = params.nclusters, nsamples = train_data.count, dims = train_data.dims;
    int i, j;

    if(nclusters == nsamples)
    {
        img_t src = cvMat(1, dims, CC_32F);
        img_t dst = cvMat(1, dims, CC_64F);
        for(i = 0; i < nsamples; i++)
        {
            src->tt.data = train_data->tt.data[i];
            dst->tt.data = means->tt.data + means->step*i;
            cvConvert(&src, &dst);
            cvZero(covs[i]);
            cvSetIdentity(cov_rotate_mats[i]);
        }
        cvSetIdentity(probs);
        cvSet(weights, cScalar(1./nclusters));
    }
    else
    {
        int max_count = 0;
        
        CC_CALL(class_ranges = cvCreateMat(1, nclusters+1, CC_32SC1));
        if(nclusters > 1)
        {
            CC_CALL(labels = cvCreateMat(1, nsamples, CC_32SC1));
            kmeans(train_data, nclusters, labels, cTermCriteria(CC_TERMCRIT_ITER,
                    params.means ? 1 : 10, 0.5), params.means);
            CC_CALL(cvSortSamplesByClasses((const float**)train_data->tt.fl,
                                            labels, class_ranges->tt.i));
        }
        else
        {
            class_ranges->tt.i[0] = 0;
            class_ranges->tt.i[1] = nsamples;
        }

        for(i = 0; i < nclusters; i++)
        {
            int left = class_ranges->tt.i[i], right = class_ranges->tt.i[i+1];
            max_count = MAX(max_count, right - left);
        }
        CC_CALL(hdr = cAlloc(max_count*sizeof(hdr[0])));
        CC_CALL(vec = (const void**)cAlloc(max_count*sizeof(vec[0])));
        hdr[0] = cvMat(1, dims, CC_32F);
        for(i = 0; i < max_count; i++)
        {
            vec[i] = hdr + i;
            hdr[i] = hdr[0];
        }
                
        for(i = 0; i < nclusters; i++)
        {
            int left = class_ranges->tt.i[i], right = class_ranges->tt.i[i+1];
            int cluster_size = right - left;
            img_t avg;
            
            if(cluster_size <= 0)
                continue;

            for(j = left; j < right; j++)
                hdr[j - left]->tt.fl = train_data->tt.fl[j];
            
            CC_CALL(cvGetRow(means, &avg, i));
            CC_CALL(cvCalcCovarMatrix(vec, cluster_size, covs[i],
                &avg, CC_COVAR_NORMAL | CC_COVAR_SCALE));
            weights->tt.db[i] = (double)cluster_size/(double)nsamples;
        }
    }

    __END__;

    cvReleaseMat(&class_ranges);
    cvReleaseMat(&labels);
    cFree(&hdr);
    cFree(&vec);
}


void CvEM::kmeans(const CvVectors& train_data, int nclusters, img_t* labels,
                   CTermCriteria termcrit, const img_t* centers0)
{
    img_t* centers = 0;
    img_t* old_centers = 0;
    img_t* counters = 0;
    
    CC_FUNCNAME("CvEM::kmeans");

    __BEGIN__;

    CRNG rng = cRNG(-1);
    int i, j, k, nsamples, dims;
    int iter = 0;
    double max_dist = DBL_MAX;

    termcrit = cvCheckTermCriteria(termcrit, 1e-6, 100);
    termcrit.epsilon *= termcrit.epsilon;
    nsamples = train_data.count;
    dims = train_data.dims;
    nclusters = MIN(nclusters, nsamples);

    CC_CALL(centers = cvCreateMat(nclusters, dims, CC_64FC1));
    CC_CALL(old_centers = cvCreateMat(nclusters, dims, CC_64FC1));
    CC_CALL(counters = cvCreateMat(1, nclusters, CC_32SC1));
    cvZero(old_centers);

    if(centers0)
    {
        CC_CALL(cvConvert(centers0, centers));
    }
    else
    {
        for(i = 0; i < nsamples; i++)
            labels->tt.i[i] = i*nclusters/nsamples;
        cvRandShuffle(labels, &rng);
    }

    for(;;)
    {
        img_t* temp;

        if(iter > 0 || centers0)
        {
            for(i = 0; i < nsamples; i++)
            {
                const float* s = train_data->tt.fl[i];
                int k_best = 0;
                double min_dist = DBL_MAX;

                for(k = 0; k < nclusters; k++)
                {
                    const double* c = (double*)(centers->tt.data + k*centers->step);
                    double dist = 0;
                
                    for(j = 0; j <= dims - 4; j += 4)
                    {
                        double t0 = c[j] - s[j];
                        double t1 = c[j+1] - s[j+1];
                        dist += t0*t0 + t1*t1;
                        t0 = c[j+2] - s[j+2];
                        t1 = c[j+3] - s[j+3];
                        dist += t0*t0 + t1*t1;
                    }

                    for(; j < dims; j++)
                    {
                        double t = c[j] - s[j];
                        dist += t*t;
                    }
                
                    if(min_dist > dist)
                    {
                        min_dist = dist;
                        k_best = k;
                    }
                }

                labels->tt.i[i] = k_best;
            }
        }

        if(++iter > termcrit.max_iter)
            break;

        CC_SWAP(centers, old_centers, temp);
        cvZero(centers);
        cvZero(counters);

        // update centers
        for(i = 0; i < nsamples; i++)
        {
            const float* s = train_data->tt.fl[i];
            k = labels->tt.i[i];
            double* c = (double*)(centers->tt.data + k*centers->step);

            for(j = 0; j <= dims - 4; j += 4)
            {
                double t0 = c[j] + s[j];
                double t1 = c[j+1] + s[j+1];

                c[j] = t0;
                c[j+1] = t1;

                t0 = c[j+2] + s[j+2];
                t1 = c[j+3] + s[j+3];

                c[j+2] = t0;
                c[j+3] = t1;
            }
            for(; j < dims; j++)
                c[j] += s[j];
            counters->tt.i[k]++;
        }

        if(iter > 1)
            max_dist = 0;

        for(k = 0; k < nclusters; k++)
        {
            double* c = (double*)(centers->tt.data + k*centers->step);
            if(counters->tt.i[k] != 0)
            {
                double scale = 1./counters->tt.i[k];
                for(j = 0; j < dims; j++)
                    c[j] *= scale;
            }
            else
            {
                const float* s;
                for(j = 0; j < 10; j++)
                {
                    i = cvRandInt(&rng) % nsamples;
                    if(counters->tt.i[labels->tt.i[i]] > 1)
                        break;
                }
                s = train_data->tt.fl[i];
                for(j = 0; j < dims; j++)
                    c[j] = s[j];
            }
            
            if(iter > 1)
            {
                double dist = 0;
                const double* c_o = (double*)(old_centers->tt.data + k*old_centers->step);
                for(j = 0; j < dims; j++)
                {
                    double t = c[j] - c_o[j];
                    dist += t*t;
                }
                if(max_dist < dist)
                    max_dist = dist;
            }
        }

        if(max_dist < termcrit.epsilon)
            break;
    }

    cvZero(counters);
    for(i = 0; i < nsamples; i++)
        counters->tt.i[labels->tt.i[i]]++;

    // ensure that we do not have empty clusters
    for(k = 0; k < nclusters; k++)
        if(counters->tt.i[k] == 0)
            for(;;)
            {
                i = cvRandInt(&rng) % nsamples;
                j = labels->tt.i[i];
                if(counters->tt.i[j] > 1)
                {
                    labels->tt.i[i] = k;
                    counters->tt.i[j]--;
                    counters->tt.i[k]++;
                    break;
                }
            }

    __END__;

    cvReleaseMat(&centers);
    cvReleaseMat(&old_centers);
    cvReleaseMat(&counters);
}


/****************************************************************************************/
/* log_weight_div_det[k] = -2*log(weights_k) + log(det(Sigma_k)))

   covs[k] = cov_rotate_mats[k] * cov_eigen_values[k] * (cov_rotate_mats[k])'
   cov_rotate_mats[k] are orthogonal matrices of eigenvectors and
   cov_eigen_values[k] are diagonal matrices (represented by 1D vectors) of eigen values.

   The <alpha_ik> is the probability of the vector x_i to belong to the k-th cluster:
   <alpha_ik> ~ weights_k * exp{ -0.5[ln(det(Sigma_k)) + (x_i - mu_k)' Sigma_k^(-1) (x_i - mu_k)] }
   We calculate these probabilities here by the equivalent formulae:
   Denote
   S_ik = -0.5(log(det(Sigma_k)) + (x_i - mu_k)' Sigma_k^(-1) (x_i - mu_k)) + log(weights_k),
   M_i = max_k S_ik = S_qi, so that the q-th class is the one where maximum reaches. Then
   alpha_ik = exp{ S_ik - M_i } / (1 + sum_j!=q exp{ S_ji - M_i })
*/
double CvEM::run_em(const CvVectors& train_data)
{
    img_t* centered_sample = 0;
    img_t* covs_item = 0;
    img_t* log_det = 0;
    img_t* log_weights = 0;
    img_t* cov_eigen_values = 0;
    img_t* samples = 0;
    img_t* sum_probs = 0;
    log_likelihood = -DBL_MAX;

    CC_FUNCNAME("CvEM::run_em");
    __BEGIN__;

    int nsamples = train_data.count, dims = train_data.dims, nclusters = params.nclusters;
    double min_variation = FLT_EPSILON;
    double min_det_value = MAX(DBL_MIN, pow(min_variation, dims));
    double likelihood_bias = -CC_LOG2PI * (double)nsamples * (double)dims / 2., _log_likelihood = -DBL_MAX;
    int start_step = params.start_step;
    
    int i, j, k, n;
    int is_general = 0, is_diagonal = 0, is_spherical = 0;
    double prev_log_likelihood = -DBL_MAX / 1000., det, d;
    img_t whdr, iwhdr, diag, *w, *iw;
    double* w_data;
    double* sp_data;

    if(nclusters == 1)
    {
        double log_weight;
        CC_CALL(cvSet(probs, cScalar(1.)));

        if(params.cov_mat_type == COV_MAT_SPHERICAL)
        {
            d = cvTrace(*covs).val[0]/dims;
            d = MAX(d, FLT_EPSILON);
            inv_eigen_values->tt.db[0] = 1./d;
            log_weight = pow(d, dims*0.5);
        }
        else
        {
            w_data = inv_eigen_values->tt.db;
            
            if(params.cov_mat_type == COV_MAT_GENERIC)
                cvSVD(*covs, inv_eigen_values, *cov_rotate_mats, 0, CC_SVD_U_T);
            else
                cvTranspose(cvGetDiag(*covs, &diag), inv_eigen_values);
       
            cvMaxS(inv_eigen_values, FLT_EPSILON, inv_eigen_values);
            for(j = 0, det = 1.; j < dims; j++)
                det *= w_data[j];
            log_weight = sqrt(det);
            cvDiv(0, inv_eigen_values, inv_eigen_values);
        }

        log_weight_div_det->tt.db[0] = -2*log(weights->tt.db[0]/log_weight);
        log_likelihood = DBL_MAX/1000.;
        EXIT;
    }

    if(params.cov_mat_type == COV_MAT_GENERIC)
        is_general  = 1;
    else if(params.cov_mat_type == COV_MAT_DIAGONAL)
        is_diagonal = 1;
    else if(params.cov_mat_type == COV_MAT_SPHERICAL)
        is_spherical  = 1;
    /* In the case of <cov_mat_type> == COV_MAT_DIAGONAL, the k-th row of cov_eigen_values
    contains the diagonal elements (variations). In the case of
    <cov_mat_type> == COV_MAT_SPHERICAL - the 0-ths elements of the vectors cov_eigen_values[k]
    are to be equal to the mean of the variations over all the dimensions. */

    CC_CALL(log_det = cvCreateMat(1, nclusters, CC_64FC1));
    CC_CALL(log_weights = cvCreateMat(1, nclusters, CC_64FC1));
    CC_CALL(covs_item = cvCreateMat(dims, dims, CC_64FC1));
    CC_CALL(centered_sample = cvCreateMat(1, dims, CC_64FC1));
    CC_CALL(cov_eigen_values = cvCreateMat(inv_eigen_values->rows, inv_eigen_values->cols, CC_64FC1));
    CC_CALL(samples = cvCreateMat(nsamples, dims, CC_64FC1));
    CC_CALL(sum_probs = cvCreateMat(1, nclusters, CC_64FC1));
    sp_data = sum_probs->tt.db;

    // copy the training data into double-precision matrix
    for(i = 0; i < nsamples; i++)
    {
        const float* src = train_data->tt.fl[i];
        double* dst = (double*)(samples->tt.data + samples->step*i);

        for(j = 0; j < dims; j++)
            dst[j] = src[j];
    }

    if(start_step != START_M_STEP)
    {
        for(k = 0; k < nclusters; k++)
        {
            if(is_general || is_diagonal)
            {
                w = cvGetRow(cov_eigen_values, &whdr, k);
                if(is_general)
                    cvSVD(covs[k], w, cov_rotate_mats[k], 0, CC_SVD_U_T);
                else
                    cvTranspose(cvGetDiag(covs[k], &diag), w);
                w_data = w->tt.db;
                for(j = 0, det = 1.; j < dims; j++)
                    det *= w_data[j];
                if(det < min_det_value)
                {
                    if(start_step == START_AUTO_STEP)
                        det = min_det_value;
                    else
                        EXIT;
                }
                log_det->tt.db[k] = det;
            }
            else
            {
                d = cvTrace(covs[k]).val[0]/(double)dims;
                if(d < min_variation)
                {
                    if(start_step == START_AUTO_STEP)
                        d = min_variation;
                    else
                        EXIT;
                }
                cov_eigen_values->tt.db[k] = d;
                log_det->tt.db[k] = d;
            }
        }
    
        cvLog(log_det, log_det);
        if(is_spherical)
            cvScale(log_det, log_det, dims);
    }

    for(n = 0; n < params.term_crit.max_iter; n++)
    {
        if(n > 0 || start_step != START_M_STEP)
        {
            // e-step: compute probs_ik from means_k, covs_k and weights_k.
            CC_CALL(cvLog(weights, log_weights));

            // S_ik = -0.5[log(det(Sigma_k)) + (x_i - mu_k)' Sigma_k^(-1) (x_i - mu_k)] + log(weights_k)
            for(k = 0; k < nclusters; k++)
            {
                img_t* u = cov_rotate_mats[k];
                const double* mean = (double*)(means->tt.data + means->step*k);
                w = cvGetRow(cov_eigen_values, &whdr, k);
                iw = cvGetRow(inv_eigen_values, &iwhdr, k);
                cvDiv(0, w, iw);

                w_data = (double*)(inv_eigen_values->tt.data + inv_eigen_values->step*k);

                for(i = 0; i < nsamples; i++)
                {
                    double *csample = centered_sample->tt.db, p = log_det->tt.db[k];
                    const double* sample = (double*)(samples->tt.data + samples->step*i);
                    double* pp = (double*)(probs->tt.data + probs->step*i);
                    for(j = 0; j < dims; j++)
                        csample[j] = sample[j] - mean[j];
                    if(is_general)
                        cvGEMM(centered_sample, u, 1, 0, 0, centered_sample, CC_GEMM_B_T);
                    for(j = 0; j < dims; j++)
                        p += csample[j]*csample[j]*w_data[is_spherical ? 0 : j];
                    pp[k] = -0.5*p + log_weights->tt.db[k];

                    // S_ik <- S_ik - max_j S_ij
                    if(k == nclusters - 1)
                    {
                        double max_val = 0;
                        for(j = 0; j < nclusters; j++)
                            max_val = MAX(max_val, pp[j]);
                        for(j = 0; j < nclusters; j++)
                            pp[j] -= max_val;
                    }
                }
            }

            CC_CALL(cvExp(probs, probs)); // exp(S_ik)
            cvZero(sum_probs);

            // alpha_ik = exp(S_ik) / sum_j exp(S_ij),
            // log_likelihood = sum_i log (sum_j exp(S_ij))
            for(i = 0, _log_likelihood = likelihood_bias; i < nsamples; i++)
            {
                double* pp = (double*)(probs->tt.data + probs->step*i), sum = 0;
                for(j = 0; j < nclusters; j++)
                    sum += pp[j];
                sum = 1./MAX(sum, DBL_EPSILON);
                for(j = 0; j < nclusters; j++)
                {
                    double p = pp[j] *= sum;
                    sp_data[j] += p;
                }
                _log_likelihood -= log(sum);
            }

            // check termination criteria
            if(fabs((_log_likelihood - prev_log_likelihood) / prev_log_likelihood) < params.term_crit.epsilon)
                break;
            prev_log_likelihood = _log_likelihood;
        }

        // m-step: update means_k, covs_k and weights_k from probs_ik
        cvGEMM(probs, samples, 1, 0, 0, means, CC_GEMM_A_T);

        for(k = 0; k < nclusters; k++)
        {
            double sum = sp_data[k], inv_sum = 1./sum;
            img_t* cov = covs[k], _mean, _sample;
            
            w = cvGetRow(cov_eigen_values, &whdr, k);
            w_data = w->tt.db;
            cvGetRow(means, &_mean, k);
            cvGetRow(samples, &_sample, k);

            // update weights_k
            weights->tt.db[k] = sum;
            
            // update means_k
            cvScale(&_mean, &_mean, inv_sum);

            // compute covs_k
            cvZero(cov);
            cvZero(w);

            for(i = 0; i < nsamples; i++)
            {
                double p = probs->tt.db[i*nclusters + k]*inv_sum;
                _sample->tt.db = (double*)(samples->tt.data + samples->step*i);

                if(is_general)
                {
                    cvMulTransposed(&_sample, covs_item, 1, &_mean);
                    cvScaleAdd(covs_item, cRealScalar(p), cov, cov);
                }
                else
                    for(j = 0; j < dims; j++)
                    {
                        double val = _sample->tt.db[j] - _mean->tt.db[j];
                        w_data[is_spherical ? 0 : j] += p*val*val;
                    }
            }
            
            if(is_spherical)
            {
                d = w_data[0]/(double)dims;
                d = MAX(d, min_variation);
                w->tt.db[0] = d;
                log_det->tt.db[k] = d;
            }
            else
            {
                if(is_general)
                    cvSVD(cov, w, cov_rotate_mats[k], 0, CC_SVD_U_T);
                cvMaxS(w, min_variation, w);
                for(j = 0, det = 1.; j < dims; j++)
                    det *= w_data[j];
                log_det->tt.db[k] = det;
            }
        }

        cvConvertScale(weights, weights, 1./(double)nsamples, 0);
        cvMaxS(weights, DBL_MIN, weights);

        cvLog(log_det, log_det);
        if(is_spherical)
            cvScale(log_det, log_det, dims);
    } // end of iteration process

    //log_weight_div_det[k] = -2*log(weights_k/det(Sigma_k))^0.5) = -2*log(weights_k) + log(det(Sigma_k)))
    if(log_weight_div_det)
    {
        cvScale(log_weights, log_weight_div_det, -2);
        cvAdd(log_weight_div_det, log_det, log_weight_div_det);
    }
    
    /* Now finalize all the covariation matrices:
    1) if <cov_mat_type> == COV_MAT_DIAGONAL we used array of <w> as diagonals.
       Now w[k] should be copied back to the diagonals of covs[k];
    2) if <cov_mat_type> == COV_MAT_SPHERICAL we used the 0-th element of w[k]
       as an average variation in each cluster. The value of the 0-th element of w[k]
       should be copied to the all of the diagonal elements of covs[k]. */
    if(is_spherical)
    {
        for(k = 0; k < nclusters; k++)
            cvSetIdentity(covs[k], cScalar(cov_eigen_values->tt.db[k]));
    }
    else if(is_diagonal)
    {
        for(k = 0; k < nclusters; k++)
            cvTranspose(cvGetRow(cov_eigen_values, &whdr, k),
                         cvGetDiag(covs[k], &diag));
    }
    cvDiv(0, cov_eigen_values, inv_eigen_values);

    log_likelihood = _log_likelihood;

    __END__;

    cvReleaseMat(&log_det);
    cvReleaseMat(&log_weights);
    cvReleaseMat(&covs_item);
    cvReleaseMat(&centered_sample);
    cvReleaseMat(&cov_eigen_values);
    cvReleaseMat(&samples);
    cvReleaseMat(&sum_probs);

    return log_likelihood;
}


int CvEM::get_nclusters() const
{
    return params.nclusters;
}

const img_t* CvEM::get_means() const
{
    return means;
}

const img_t** CvEM::get_covs() const
{
    return (const img_t**)covs;
}

const img_t* CvEM::get_weights() const
{
    return weights;
}

const img_t* CvEM::get_probs() const
{
    return probs;
}


