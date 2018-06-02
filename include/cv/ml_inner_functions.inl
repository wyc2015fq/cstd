
#include "_ml.h"


CvStatModel::CvStatModel()
{
    default_model_name = "my_stat_model";
}


CvStatModel::~CvStatModel()
{
    clear();
}


void CvStatModel::clear()
{
}


void CvStatModel::save(const char* filename, const char* name)
{
    CvFileStorage* fs = 0;
    
    CC_FUNCNAME("CvStatModel::save");

    __BEGIN__;

    CC_CALL(fs = cvOpenFileStorage(filename, 0, CC_STORAGE_WRITE));
    if(!fs)
        CC_ERROR(CC_StsError, "Could not open the file storage. Check the path and permissions");

    write(fs, name ? name : default_model_name);

    __END__;

    cvReleaseFileStorage(&fs);
}


void CvStatModel::load(const char* filename, const char* name)
{
    CvFileStorage* fs = 0;
    
    CC_FUNCNAME("CvStatModel::load");

    __BEGIN__;

    CvFileNode* model_node = 0;

    CC_CALL(fs = cvOpenFileStorage(filename, 0, CC_STORAGE_READ));
    if(!fs)
        CC_ERROR(CC_StsError, "Could not open the file storage. Check the path and permissions");

    if(name)
        model_node = cvGetFileNodeByName(fs, 0, name);
    else
    {
        CvFileNode* root = cvGetRootFileNode(fs);
        if(root->tt.seq->total > 0)
            model_node = (CvFileNode*)cvGetSeqElem(root->tt.seq, 0);
    }

    read(fs, model_node);

    __END__;

    cvReleaseFileStorage(&fs);
}


void CvStatModel::write(CvFileStorage*, const char*)
{
    OPENCC_ERROR(CC_StsNotImplemented, "CvStatModel::write", "");
}


void CvStatModel::read(CvFileStorage*, CvFileNode*)
{
    OPENCC_ERROR(CC_StsNotImplemented, "CvStatModel::read", "");
}


/* Calculates upper triangular matrix S, where A is a symmetrical matrix A=S'*S */
CC_IMPL void cvChol(img_t* A, img_t* S)
{
    int dim = A->rows;

    int i, j, k;
    float sum;
        
    for(i = 0; i < dim; i++)
    {   
        for(j = 0; j < i; j++)
            CC_MAT_ELEM(*S, float, i, j) = 0;
                    
        sum = 0;
        for(k = 0; k < i; k++) 
            sum += CC_MAT_ELEM(*S, float, k, i) * CC_MAT_ELEM(*S, float, k, i);

        CC_MAT_ELEM(*S, float, i, i) = (float)sqrt(CC_MAT_ELEM(*A, float, i, i) - sum);

        for(j = i + 1; j < dim; j++)
        {
            sum = 0;
            for(k = 0; k < i; k++) 
                sum += CC_MAT_ELEM(*S, float, k, i) * CC_MAT_ELEM(*S, float, k, j);

            CC_MAT_ELEM(*S, float, i, j) =
                (CC_MAT_ELEM(*A, float, i, j) - sum) / CC_MAT_ELEM(*S, float, i, i);
        
        }
    }
}

/* Generates <sample> from multivariate normal distribution, where <mean> - is an
   average row vector, <cov> - symmetric covariation matrix */
CC_IMPL void cvRandMVNormal(img_t* mean, img_t* cov, img_t* sample, CRNG* rng)
{
    int dim = sample->cols;
    int amount = sample->rows;

    CRNG state = rng ? *rng : cRNG(time(0));
    cvRandArr(&state, sample, CC_RAND_NORMAL, cScalarAll(0), cScalarAll(1));
    
    img_t* utmat = cvCreateMat(dim, dim, sample->tid);
    img_t* vect = cvCreateMatHeader(1, dim, sample->tid);

    cvChol(cov, utmat);

    int i;
    for(i = 0; i < amount; i++)
    {
        cvGetRow(sample, vect, i);
        cvMatMulAdd(vect, utmat, mean, vect);
    }

    cvReleaseMat(&vect); 
    cvReleaseMat(&utmat);
}


/* Generates <sample> of <amount> points from a discrete variate xi,
   where Pr{xi = k} == probs[k], 0 < k < len - 1. */
CC_IMPL void cvRandSeries(float probs[], int len, int sample[], int amount)
{
    img_t* univals = cvCreateMat(1, amount, CC_32FC1);
    float* knots = (float*)cAlloc(len * sizeof(float));

    int i, j;

    CRNG state = cRNG(-1);
    cvRandArr(&state, univals, CC_RAND_UNI, cScalarAll(0), cScalarAll(1));
    
    knots[0] = probs[0];
    for(i = 1; i < len; i++)
        knots[i] = knots[i - 1] + probs[i];

    for(i = 0; i < amount; i++)
        for(j = 0; j < len; j++)
        {
            if (CC_MAT_ELEM(*univals, float, 0, i) <= knots[j])
            {
                sample[i] = j;
                break;
            }
        }
    
    cFree(&knots);
}

/* Generates <sample> from gaussian mixture distribution */
CC_IMPL void cvRandGaussMixture(img_t* means[], 
                                 img_t* covs[],
                                 float weights[],
                                 int clsnum,
                                 img_t* sample,
                                 img_t* sampClasses)
{
    int dim = sample->cols;
    int amount = sample->rows;

    int i, clss;

    int* sample_clsnum = (int*)cAlloc(amount * sizeof(int));
    img_t** utmats = (img_t**)cAlloc(clsnum * sizeof(img_t*));
    img_t* vect = cvCreateMatHeader(1, dim, CC_32FC1);

    img_t* classes;
    if(sampClasses)
        classes = sampClasses;
    else
        classes = cvCreateMat(1, amount, CC_32FC1);
    
    CRNG state = cRNG(-1);
    cvRandArr(&state, sample, CC_RAND_NORMAL, cScalarAll(0), cScalarAll(1));
    
    cvRandSeries(weights, clsnum, sample_clsnum, amount);

    for(i = 0; i < clsnum; i++)
    {
        utmats[i] = cvCreateMat(dim, dim, CC_32FC1);
        cvChol(covs[i], utmats[i]);
    }
    
    for(i = 0; i < amount; i++)
    {
        CC_MAT_ELEM(*classes, float, 0, i) = (float)sample_clsnum[i];
        cvGetRow(sample, vect, i);
        clss = sample_clsnum[i];
        cvMatMulAdd(vect, utmats[clss], means[clss], vect);
    }

    if(!sampClasses)
        cvReleaseMat(&classes);
    for(i = 0; i < clsnum; i++)
        cvReleaseMat(&utmats[i]);
    cFree(&utmats);
    cFree(&sample_clsnum);
    cvReleaseMat(&vect);    
}


img_t* icvGenerateRandomClusterCenters (int seed, const img_t* data,
                                         int num_of_clusters, img_t* _centers)
{
    img_t* centers = _centers;

    CC_FUNCNAME("icvGenerateRandomClusterCenters");
    __BEGIN__;

    CRNG rng;
    img_t data_comp, centers_comp;
    CPoint minLoc, maxLoc; // Not used, just for function "cvMinMaxLoc"
    double minVal, maxVal;
    int i;
    int dim = data ? data->cols : 0;

    if(ICC_IS_MAT_OF_TYPE(data, CC_32FC1))
    {
        if(_centers && !ICC_IS_MAT_OF_TYPE (_centers, CC_32FC1))
        {
            CC_ERROR(CC_StsBadArg,"");
        }
        else if(!_centers)
            CC_CALL(centers = cvCreateMat (num_of_clusters, dim, CC_32FC1));
    }
    else if(ICC_IS_MAT_OF_TYPE(data, CC_64FC1))
    {
        if(_centers && !ICC_IS_MAT_OF_TYPE (_centers, CC_64FC1))
        {
            CC_ERROR(CC_StsBadArg,"");
        }
        else if(!_centers)
            CC_CALL(centers = cvCreateMat (num_of_clusters, dim, CC_64FC1));
    }
    else
        CC_ERROR (CC_StsBadArg,"");

    if(num_of_clusters < 1)
        CC_ERROR (CC_StsBadArg,"");

    rng = cRNG(seed);
    for (i = 0; i < dim; i++)
    {
        CC_CALL(cvGetCol (data, &data_comp, i));
        CC_CALL(cvMinMaxLoc (&data_comp, &minVal, &maxVal, &minLoc, &maxLoc));
        CC_CALL(cvGetCol (centers, &centers_comp, i));
        CC_CALL(cvRandArr (&rng, &centers_comp, CC_RAND_UNI, cScalarAll(minVal), cScalarAll(maxVal)));
    }

    __END__;

    if((cvGetErrStatus () < 0) || (centers != _centers))
        cvReleaseMat (&centers);

    return _centers ? _centers : centers;
} // end of icvGenerateRandomClusterCenters

// By S. Dilman - begin -

#define ICC_RAND_MAX    4294967296 // == 2^32

CC_IMPL void cvRandRoundUni (img_t* center,
                             float radius_small,
                             float radius_large,
                             img_t* desired_matrix,
                             CRNG* rng_state_ptr)
{
    float rad, norm, coefficient;
    int dim, size, i, j;
    img_t *cov, sample;
    CRNG rng_local;
    
    CC_FUNCNAME("cvRandRoundUni");
    __BEGIN__

    rng_local = *rng_state_ptr;

    CC_ASSERT ((radius_small >= 0) && 
               (radius_large > 0) && 
               (radius_small <= radius_large));
    CC_ASSERT (center && desired_matrix && rng_state_ptr);
    CC_ASSERT (center->rows == 1);
    CC_ASSERT (center->cols == desired_matrix->cols);

    dim = desired_matrix->cols;
    size = desired_matrix->rows;
    cov = cvCreateMat (dim, dim, CC_32FC1);
    cvSetIdentity (cov);
    cvRandMVNormal (center, cov, desired_matrix, &rng_local);

    for (i = 0; i < size; i++)
    {
        rad = (float)(cvRandReal(&rng_local)*(radius_large - radius_small) + radius_small);
        cvGetRow (desired_matrix, &sample, i);
        norm = (float) cvNorm (&sample, 0, CC_L2);
        coefficient = rad / norm;
        for (j = 0; j < dim; j++)
             CC_MAT_ELEM (sample, float, 0, j) *= coefficient;
    }

    __END__

}

// By S. Dilman - end -

/* Aij <- Aji for i > j if lower_to_upper != 0
              for i < j if lower_to_upper = 0 */
void cvCompleteSymm(img_t* matrix, int lower_to_upper)
{
    CC_FUNCNAME("cvCompleteSymm");

    __BEGIN__;

    int rows, cols;
    int i, j;
    int step;

    if(!CC_IS_MAT(matrix))
        CC_ERROR(CC_StsBadArg, "Invalid matrix argument");

    rows = matrix->rows;
    cols = matrix->cols;
    step = matrix->step / CC_ELEM_SIZE(matrix->tid);

    switch(CC_MAT_TYPE(matrix->tid))
    {
    case CC_32FC1:
        {
        float* dst = matrix->tt.fl;
        if(!lower_to_upper)
            for(i = 1; i < rows; i++)
            {
                const float* src = (const float*)(matrix->tt.fl + i);
                dst += step;
                for(j = 0; j < i; j++, src += step)
                    dst[j] = src[0];
            }
        else
            for(i = 0; i < rows-1; i++, dst += step)
            {
                const float* src = (const float*)(matrix->tt.fl + (i+1)*step + i);
                for(j = i+1; j < cols; j++, src += step)
                    dst[j] = src[0];
            }
        }
        break;
    case CC_64FC1:
        {
        double* dst = matrix->tt.db;
        if(!lower_to_upper)
            for(i = 1; i < rows; i++)
            {
                const double* src = (const double*)(matrix->tt.db + i);
                dst += step;
                for(j = 0; j < i; j++, src += step)
                    dst[j] = src[0];
            }
        else
            for(i = 0; i < rows-1; i++, dst += step)
            {
                const double* src = (const double*)(matrix->tt.db + (i+1)*step + i);
                for(j = i+1; j < cols; j++, src += step)
                    dst[j] = src[0];
            }
        }
        break;
    }

    __END__;
}


static int C_CDECL
icvCmpIntegers(const void* a, const void* b)
{
    return *(const int*)a - *(const int*)b;
}


static int C_CDECL
icvCmpIntegersPtr(const void* _a, const void* _b)
{
    int a = **(const int**)_a;
    int b = **(const int**)_b;
    return (a < b ? -1 : 0)|(a > b);
}


static int icvCmpSparseVecElems(const void* a, const void* b)
{
    return ((CvSparseVecElem32f*)a)->idx - ((CvSparseVecElem32f*)b)->idx;
}


img_t*
cvPreprocessIndexArray(const img_t* idx_arr, int data_arr_size, bool check_for_duplicates)
{
    img_t* idx = 0;

    CC_FUNCNAME("cvPreprocessIndexArray");

    __BEGIN__;

    int i, idx_total, idx_selected = 0, step, type, prev = INT_MIN, is_sorted = 1;
    uchar* srcb = 0;
    int* srci = 0;
    int* dsti;
    
    if(!CC_IS_MAT(idx_arr))
        CC_ERROR(CC_StsBadArg, "Invalid index array");

    if(idx_arr->rows != 1 && idx_arr->cols != 1)
        CC_ERROR(CC_StsBadSize, "the index array must be 1-dimensional");

    idx_total = idx_arr->rows + idx_arr->cols - 1;
    srcb = idx_arr->tt.data;
    srci = idx_arr->tt.i;

    type = CC_MAT_DEPTH(idx_arr->tid);
    step = CC_IS_MAT_CONT(idx_arr->tid) ? 1 : idx_arr->step/CC_ELEM_SIZE(type);

    switch(type)
    {
    case CC_8UC1:
    case CC_8SC1:
        // idx_arr is array of 1's and 0's -
        // i.e. it is a mask of the selected components
        if(idx_total != data_arr_size)
            CC_ERROR(CC_StsUnmatchedSizes,
            "Component mask should contain as many elements as the total number of input variables");
        
        for(i = 0; i < idx_total; i++)
            idx_selected += srcb[i*step] != 0;

        if(idx_selected == 0)
            CC_ERROR(CC_StsOutOfRange, "No components/input_variables is selected!");

        if(idx_selected == idx_total)
            EXIT;
        break;
    case CC_32SC1:
        // idx_arr is array of integer indices of selected components
        if(idx_total > data_arr_size)
            CC_ERROR(CC_StsOutOfRange,
            "index array may not contain more elements than the total number of input variables");
        idx_selected = idx_total;
        // check if sorted already
        for(i = 0; i < idx_total; i++)
        {
            int val = srci[i*step];
            if(val >= prev)
            {
                is_sorted = 0;
                break;
            }
            prev = val;
        }
        break;
    default:
        CC_ERROR(CC_StsUnsupportedFormat, "Unsupported index array data type "
                                           "(it should be 8uC1, 8sC1 or 32sC1)");
    }

    CC_CALL(idx = cvCreateMat(1, idx_selected, CC_32SC1));
    dsti = idx->tt.i;
    
    if(type < CC_32SC1)
    {
        for(i = 0; i < idx_total; i++)
            if(srcb[i*step])
                *dsti++ = i;
    }
    else
    {
        for(i = 0; i < idx_total; i++)
            dsti[i] = srci[i*step];
        
        if(!is_sorted)
            qsort(dsti, idx_total, sizeof(dsti[0]), icvCmpIntegers);
        
        if(dsti[0] < 0 || dsti[idx_total-1] >= data_arr_size)
            CC_ERROR(CC_StsOutOfRange, "the index array elements are out of range");

        if(check_for_duplicates)
        {
            for(i = 1; i < idx_total; i++)
                if(dsti[i] <= dsti[i-1])
                    CC_ERROR(CC_StsBadArg, "There are duplicated index array elements");
        }
    }

    __END__;

    if(cvGetErrStatus() < 0)
        cvReleaseMat(&idx);

    return idx;
}


img_t*
cvPreprocessVarType(const img_t* var_type, const img_t* var_idx,
                     int var_all, int* response_type)
{
    img_t* out_var_type = 0;
    CC_FUNCNAME("cvPreprocessVarType");

    if(response_type)
        *response_type = -1;

    __BEGIN__;

    int i, tm_size, tm_step;
    int* map = 0;
    const uchar* src;
    uchar* dst;
    int var_count = var_all;
        
    if(!CC_IS_MAT(var_type))
        CC_ERROR(var_type ? CC_StsBadArg : CC_StsNullPtr, "Invalid or absent var_type array");

    if(var_type->rows != 1 && var_type->cols != 1)
        CC_ERROR(CC_StsBadSize, "var_type array must be 1-dimensional");

    if(!CC_IS_MASK_ARR(var_type))
        CC_ERROR(CC_StsUnsupportedFormat, "type mask must be 8uC1 or 8sC1 array");

    tm_size = var_type->rows + var_type->cols - 1;
    tm_step = var_type->step ? var_type->step/CC_ELEM_SIZE(var_type->tid) : 1;

    if(/*tm_size != var_count &&*/ tm_size != var_count + 1)
        CC_ERROR(CC_StsBadArg,
        "type mask must be of <input var count> + 1 size");

    if(response_type && tm_size > var_count)
        *response_type = var_type->tt.data[var_count*tm_step] != 0;

    if(var_idx)
    {
        if(!CC_IS_MAT(var_idx) || CC_MAT_TYPE(var_idx->tid) != CC_32SC1 ||
            var_idx->rows != 1 && var_idx->cols != 1 || !CC_IS_MAT_CONT(var_idx->tid))
            CC_ERROR(CC_StsBadArg, "var index array should be continuous 1-dimensional integer vector");
        if(var_idx->rows + var_idx->cols - 1 > var_count)
            CC_ERROR(CC_StsBadSize, "var index array is too large");
        map = var_idx->tt.i;
        var_count = var_idx->rows + var_idx->cols - 1;
    }

    CC_CALL(out_var_type = cvCreateMat(1, var_count, CC_8UC1));
    src = var_type->tt.data;
    dst = out_var_type->tt.data;

    for(i = 0; i < var_count; i++)
    {
        int idx = map ? map[i] : i;
        assert((unsigned)idx < (unsigned)tm_size);
        dst[i] = (uchar)(src[idx*tm_step] != 0);
    }

    __END__;

    return out_var_type;
}


img_t*
cvPreprocessOrderedResponses(const img_t* responses, const img_t* sample_idx, int sample_all)
{
    img_t* out_responses = 0;

    CC_FUNCNAME("cvPreprocessOrderedResponses");

    __BEGIN__;

    int i, r_type, r_step;
    const int* map = 0;
    float* dst;
    int sample_count = sample_all;

    if(!CC_IS_MAT(responses))
        CC_ERROR(CC_StsBadArg, "Invalid response array");

    if(responses->rows != 1 && responses->cols != 1)
        CC_ERROR(CC_StsBadSize, "Response array must be 1-dimensional");

    if(responses->rows + responses->cols - 1 != sample_count)
        CC_ERROR(CC_StsUnmatchedSizes,
        "Response array must contain as many elements as the total number of samples");

    r_type = CC_MAT_DEPTH(responses->tid);
    if(r_type != CC_32F && r_type != CC_32SC1)
        CC_ERROR(CC_StsUnsupportedFormat, "Unsupported response type");

    r_step = responses->step ? responses->step / CC_ELEM_SIZE(responses->tid) : 1;

    if(r_type == CC_32F && cn==1 && CC_IS_MAT_CONT(responses->tid) && !sample_idx)
    {
        out_responses = responses;
        EXIT;
    }

    if(sample_idx)
    {
        if(!CC_IS_MAT(sample_idx) || CC_MAT_TYPE(sample_idx->tid) != CC_32SC1 ||
            sample_idx->rows != 1 && sample_idx->cols != 1 || !CC_IS_MAT_CONT(sample_idx->tid))
            CC_ERROR(CC_StsBadArg, "sample index array should be continuous 1-dimensional integer vector");
        if(sample_idx->rows + sample_idx->cols - 1 > sample_count)
            CC_ERROR(CC_StsBadSize, "sample index array is too large");
        map = sample_idx->tt.i;
        sample_count = sample_idx->rows + sample_idx->cols - 1;
    }

    CC_CALL(out_responses = cvCreateMat(1, sample_count, CC_32FC1));
    
    dst = out_responses->tt.fl;
    if(r_type == CC_32F && cn==1)
    {
        const float* src = responses->tt.fl;
        for(i = 0; i < sample_count; i++)
        {
            int idx = map ? map[i] : i;
            assert((unsigned)idx < (unsigned)sample_all);
            dst[i] = src[idx*r_step];
        }
    }
    else
    {
        const int* src = responses->tt.i;
        for(i = 0; i < sample_count; i++)
        {
            int idx = map ? map[i] : i;
            assert((unsigned)idx < (unsigned)sample_all);
            dst[i] = (float)src[idx*r_step];
        }
    }

    __END__;

    return out_responses;
}

img_t*
cvPreprocessCategoricalResponses(const img_t* responses,
    const img_t* sample_idx, int sample_all,
    img_t** out_response_map, img_t** class_counts)
{
    img_t* out_responses = 0;
    int** response_ptr = 0;

    CC_FUNCNAME("cvPreprocessCategoricalResponses");

    if(out_response_map)
        *out_response_map = 0;

    if(class_counts)
        *class_counts = 0;

    __BEGIN__;

    int i, r_type, r_step;
    int cls_count = 1, prev_cls, prev_i;
    const int* map = 0;
    const int* srci;
    const float* srcfl;
    int* dst;
    int* cls_map;
    int* cls_counts = 0;
    int sample_count = sample_all;

    if(!CC_IS_MAT(responses))
        CC_ERROR(CC_StsBadArg, "Invalid response array");

    if(responses->rows != 1 && responses->cols != 1)
        CC_ERROR(CC_StsBadSize, "Response array must be 1-dimensional");

    if(responses->rows + responses->cols - 1 != sample_count)
        CC_ERROR(CC_StsUnmatchedSizes,
        "Response array must contain as many elements as the total number of samples");

    r_type = CC_MAT_DEPTH(responses->tid);
    if(r_type != CC_32F && r_type != CC_32SC1)
        CC_ERROR(CC_StsUnsupportedFormat, "Unsupported response type");

    r_step = responses->step ? responses->step / CC_ELEM_SIZE(responses->tid) : 1;

    if(sample_idx)
    {
        if(!CC_IS_MAT(sample_idx) || CC_MAT_TYPE(sample_idx->tid) != CC_32SC1 ||
            sample_idx->rows != 1 && sample_idx->cols != 1 || !CC_IS_MAT_CONT(sample_idx->tid))
            CC_ERROR(CC_StsBadArg, "sample index array should be continuous 1-dimensional integer vector");
        if(sample_idx->rows + sample_idx->cols - 1 > sample_count)
            CC_ERROR(CC_StsBadSize, "sample index array is too large");
        map = sample_idx->tt.i;
        sample_count = sample_idx->rows + sample_idx->cols - 1;
    }

    CC_CALL(out_responses = cvCreateMat(1, sample_count, CC_32SC1));

    if(!out_response_map)
        CC_ERROR(CC_StsNullPtr, "out_response_map pointer is NULL");

    CC_CALL(response_ptr = (int**)cAlloc(sample_count*sizeof(response_ptr[0])));

    srci = responses->tt.i;
    srcfl = responses->tt.fl;
    dst = out_responses->tt.i;

    for(i = 0; i < sample_count; i++)
    {
        int idx = map ? map[i] : i;
        assert((unsigned)idx < (unsigned)sample_all);
        if(r_type == CC_32SC1)
            dst[i] = srci[idx*r_step];
        else
        {
            float rf = srcfl[idx*r_step];
            int ri = cRound(rf);
            if(ri != rf)
            {
                char buf[100];
                sprintf(buf, "response #%d is not integral", idx);
                CC_ERROR(CC_StsBadArg, buf);
            }
            dst[i] = ri;
        }
        response_ptr[i] = dst + i;
    }

    qsort(response_ptr, sample_count, sizeof(int*), icvCmpIntegersPtr);

    // count the classes
    for(i = 1; i < sample_count; i++)
        cls_count += *response_ptr[i] != *response_ptr[i-1];

    if(cls_count < 2)
        CC_ERROR(CC_StsBadArg, "There is only a single class");

    CC_CALL(*out_response_map = cvCreateMat(1, cls_count, CC_32SC1));

    if(class_counts)
    {
        CC_CALL(*class_counts = cvCreateMat(1, cls_count, CC_32SC1));
        cls_counts = (*class_counts)->tt.i;
    }

    // compact the class indices and build the map
    prev_cls = ~*response_ptr[0];
    cls_count = -1;
    cls_map = (*out_response_map)->tt.i;

    for(i = 0, prev_i = -1; i < sample_count; i++)
    {
        int cur_cls = *response_ptr[i];
        if(cur_cls != prev_cls)
        {
            if(cls_counts && cls_count >= 0)
                cls_counts[cls_count] = i - prev_i;
            cls_map[++cls_count] = prev_cls = cur_cls;
            prev_i = i;
        }
        *response_ptr[i] = cls_count;
    }
    
    if(cls_counts)
        cls_counts[cls_count] = i - prev_i;

    __END__;

    cFree(&response_ptr);

    return out_responses;
}


const float**
cvGetTrainSamples(const img_t* train_data, int tflag,
                   const img_t* var_idx, const img_t* sample_idx,
                   int* _var_count, int* _sample_count,
                   bool always_copy_data)
{
    float** samples = 0;
    
    CC_FUNCNAME("cvGetTrainSamples");

    __BEGIN__;
    
    int i, j, var_count, sample_count, s_step, v_step;
    bool copy_data;
    const float* data;
    const int *s_idx, *v_idx;

    if(!CC_IS_MAT(train_data))
        CC_ERROR(CC_StsBadArg, "Invalid or NULL training data matrix");

    var_count = var_idx ? var_idx->cols + var_idx->rows - 1 :
                tflag == CC_ROW_SAMPLE ? train_data->cols : train_data->rows;
    sample_count = sample_idx ? sample_idx->cols + sample_idx->rows - 1 :
                   tflag == CC_ROW_SAMPLE ? train_data->rows : train_data->cols;

    if(_var_count)
        *_var_count = var_count;

    if(_sample_count)
        *_sample_count = sample_count;

    copy_data = tflag != CC_ROW_SAMPLE || var_idx || always_copy_data;
    
    CC_CALL(samples = (float**)cAlloc(sample_count*sizeof(samples[0]) +
                (copy_data ? 1 : 0)*var_count*sample_count*sizeof(samples[0][0])));
    data = train_data->tt.fl;
    s_step = train_data->step / sizeof(samples[0][0]);
    v_step = 1;
    s_idx = sample_idx ? sample_idx->tt.i : 0;
    v_idx = var_idx ? var_idx->tt.i : 0;

    if(!copy_data)
    {
        for(i = 0; i < sample_count; i++)
            samples[i] = (float*)(data + (s_idx ? s_idx[i] : i)*s_step);
    }
    else
    {
        samples[0] = (float*)(samples + sample_count);
        if(tflag != CC_ROW_SAMPLE)
            CC_SWAP(s_step, v_step, i);

        for(i = 0; i < sample_count; i++)
        {
            float* dst = samples[i] = samples[0] + i*var_count;
            const float* src = data + (s_idx ? s_idx[i] : i)*s_step;

            if(!v_idx)
                for(j = 0; j < var_count; j++)
                    dst[j] = src[j*v_step];
            else
                for(j = 0; j < var_count; j++)
                    dst[j] = src[v_idx[j]*v_step];
        }
    }

    __END__;

    return (const float**)samples;
}


void
cvCheckTrainData(const img_t* train_data, int tflag,
                  const img_t* missing_mask, 
                  int* var_all, int* sample_all)
{
    CC_FUNCNAME("cvCheckTrainData");

    if(var_all)
        *var_all = 0;

    if(sample_all)
        *sample_all = 0;

    __BEGIN__;

    // check parameter types and sizes
    if(!CC_IS_MAT(train_data) || CC_MAT_TYPE(train_data->tid) != CC_32F)
        CC_ERROR(CC_StsBadArg, "train data must be floating-point matrix");

    if(missing_mask)
    {
        if(!CC_IS_MAT(missing_mask) || !CC_IS_MASK_ARR(missing_mask) ||
            !CC_ARE_SIZES_EQ(train_data, missing_mask))
            CC_ERROR(CC_StsBadArg,
            "missing value mask must be 8-bit matrix of the same size as training data");
    }

    if(tflag != CC_ROW_SAMPLE && tflag != CC_COL_SAMPLE)
        CC_ERROR(CC_StsBadArg,
        "Unknown training data layout (must be CC_ROW_SAMPLE or CC_COL_SAMPLE)");

    if(var_all)
        *var_all = tflag == CC_ROW_SAMPLE ? train_data->cols : train_data->rows;
    
    if(sample_all)
        *sample_all = tflag == CC_ROW_SAMPLE ? train_data->rows : train_data->cols;

    __END__;
}


int
cvPrepareTrainData(const char* /*funcname*/,
                    const img_t* train_data, int tflag,
                    const img_t* responses, int response_type,
                    const img_t* var_idx,
                    const img_t* sample_idx,
                    bool always_copy_data,
                    const float*** out_train_samples,
                    int* _sample_count,
                    int* _var_count,
                    int* _var_all,
                    img_t** out_responses,
                    img_t** out_response_map,
                    img_t** out_var_idx,
                    img_t** out_sample_idx)
{
    int ok = 0; 
    img_t* _var_idx = 0;
    img_t* _sample_idx = 0;
    img_t* _responses = 0;
    int sample_all = 0, sample_count = 0, var_all = 0, var_count = 0;

    CC_FUNCNAME("cvPrepareTrainData");

    // step 0. clear all the output pointers to ensure we do not try
    // to call free() with uninitialized pointers
    if(out_responses)
        *out_responses = 0;

    if(out_response_map)
        *out_response_map = 0;

    if(out_var_idx)
        *out_var_idx = 0;

    if(out_sample_idx)
        *out_sample_idx = 0;

    if(out_train_samples)
        *out_train_samples = 0;

    if(_sample_count)
        *_sample_count = 0;

    if(_var_count)
        *_var_count = 0;

    if(_var_all)
        *_var_all = 0;
    
    __BEGIN__;

    if(!out_train_samples)
        CC_ERROR(CC_StsBadArg, "output pointer to train samples is NULL");

    CC_CALL(cvCheckTrainData(train_data, tflag, 0, &var_all, &sample_all));

    if(sample_idx)
        CC_CALL(_sample_idx = cvPreprocessIndexArray(sample_idx, sample_all));
    if(var_idx)
        CC_CALL(_var_idx = cvPreprocessIndexArray(var_idx, var_all));

    if(responses)
    {
        if(!out_responses)
            CC_ERROR(CC_StsNullPtr, "output response pointer is NULL");
        
        if(response_type == CC_VAR_NUMERICAL)
        {
            CC_CALL(_responses = cvPreprocessOrderedResponses(responses,
                                                _sample_idx, sample_all));
        }
        else
        {
            CC_CALL(_responses = cvPreprocessCategoricalResponses(responses,
                                _sample_idx, sample_all, out_response_map, 0));
        }
    }

    CC_CALL(*out_train_samples =
                cvGetTrainSamples(train_data, tflag, _var_idx, _sample_idx,
                                   &var_count, &sample_count, always_copy_data)); 

    ok = 1;

    __END__;

    if(ok)
    {
        if(out_responses)
            *out_responses = _responses, _responses = 0;

        if(out_var_idx)
            *out_var_idx = _var_idx, _var_idx = 0;

        if(out_sample_idx)
            *out_sample_idx = _sample_idx, _sample_idx = 0;

        if(_sample_count)
            *_sample_count = sample_count;

        if(_var_count)
            *_var_count = var_count;

        if(_var_all)
            *_var_all = var_all;
    }
    else
    {
        if(out_response_map)
            cvReleaseMat(out_response_map);
        cFree(out_train_samples);
    }

    if(_responses != responses)
        cvReleaseMat(&_responses);
    cvReleaseMat(&_var_idx);
    cvReleaseMat(&_sample_idx);

    return ok;
}


typedef struct CvSampleResponsePair
{
    const float* sample;
    const uchar* mask;
    int response;
    int index;
}
CvSampleResponsePair;


static int
C_CDECL icvCmpSampleResponsePairs(const void* a, const void* b)
{
    int ra = ((const CvSampleResponsePair*)a)->response;
    int rb = ((const CvSampleResponsePair*)b)->response;
    int ia = ((const CvSampleResponsePair*)a)->index;
    int ib = ((const CvSampleResponsePair*)b)->index;

    return ra < rb ? -1 : ra > rb ? 1 : ia - ib;
    //return (ra > rb ? -1 : 0)|(ra < rb);
}


void
cvSortSamplesByClasses(const float** samples, const img_t* classes, 
                        int* class_ranges, const uchar** mask)
{
    CvSampleResponsePair* pairs = 0;
    CC_FUNCNAME("cvSortSamplesByClasses");

    __BEGIN__;

    int i, k = 0, sample_count;

    if(!samples || !classes || !class_ranges)
        CC_ERROR(CC_StsNullPtr, "INTERNAL ERROR: some of the args are NULL pointers");

    if(classes->rows != 1 || CC_MAT_TYPE(classes->tid) != CC_32SC1)
        CC_ERROR(CC_StsBadArg, "classes array must be a single row of integers");

    sample_count = classes->cols;
    CC_CALL(pairs = (CvSampleResponsePair*)cAlloc((sample_count+1)*sizeof(pairs[0])));
    
    for(i = 0; i < sample_count; i++)
    {
        pairs[i].sample = samples[i];
        pairs[i].mask = (mask) ? (mask[i]) : 0;
        pairs[i].response = classes->tt.i[i];
        pairs[i].index = i;
        assert(classes->tt.i[i] >= 0);
    }
    
    qsort(pairs, sample_count, sizeof(pairs[0]), icvCmpSampleResponsePairs);
    pairs[sample_count].response = -1;
    class_ranges[0] = 0;

    for(i = 0; i < sample_count; i++)
    {
        samples[i] = pairs[i].sample;
        if (mask)
            mask[i] = pairs[i].mask;
        classes->tt.i[i] = pairs[i].response;

        if(pairs[i].response != pairs[i+1].response)
            class_ranges[++k] = i+1;
    }

    __END__;

    cFree(&pairs);
}


void
cvPreparePredictData(const img_t* _sample, int dims_all,
                      const img_t* comp_idx, int class_count,
                      const img_t* prob, float** _row_sample,
                      int as_sparse)
{
    float* row_sample = 0;
    int* inverse_comp_idx = 0;
    
    CC_FUNCNAME("cvPreparePredictData");

    __BEGIN__;

    const img_t* sample = (const img_t*)_sample;
    float* sample_data;
    int sample_step;
    int is_sparse = CC_IS_SPARSE_MAT(sample);
    int d, sizes[CC_MAX_DIM];
    int i, dims_selected;
    int vec_size;

    if(!is_sparse && !CC_IS_MAT(sample))
        CC_ERROR(!sample ? CC_StsNullPtr : CC_StsBadArg, "The sample is not a valid vector");

    if(cvGetElemType(sample) != CC_32F)
        CC_ERROR(CC_StsUnsupportedFormat, "Input sample must have 32fC1 type");

    CC_CALL(d = cvGetDims(sample, sizes));

    if(!(is_sparse && d == 1 || !is_sparse && d == 2 && (sample->rows == 1 || sample->cols == 1)))
        CC_ERROR(CC_StsBadSize, "Input sample must be 1-dimensional vector");
    
    if(d == 1)
        sizes[1] = 1;

    if(sizes[0] + sizes[1] - 1 != dims_all)
        CC_ERROR(CC_StsUnmatchedSizes,
        "The sample size is different from what has been used for training");

    if(!_row_sample)
        CC_ERROR(CC_StsNullPtr, "INTERNAL ERROR: The row_sample pointer is NULL");

    if(comp_idx && (!CC_IS_MAT(comp_idx) || comp_idx->rows != 1 ||
        CC_MAT_TYPE(comp_idx->tid) != CC_32SC1))
        CC_ERROR(CC_StsBadArg, "INTERNAL ERROR: invalid comp_idx");

    dims_selected = comp_idx ? comp_idx->cols : dims_all;
    
    if(prob)
    {
        if(!CC_IS_MAT(prob))
            CC_ERROR(CC_StsBadArg, "The output matrix of probabilities is invalid");

        if((prob->rows != 1 && prob->cols != 1) ||
            CC_MAT_TYPE(prob->tid) != CC_32F &&
            CC_MAT_TYPE(prob->tid) != CC_64F)
            CC_ERROR(CC_StsBadSize,
            "The matrix of probabilities must be 1-dimensional vector of 32fC1 type");

        if(prob->rows + prob->cols - 1 != class_count)
            CC_ERROR(CC_StsUnmatchedSizes,
            "The vector of probabilities must contain as many elements as "
            "the number of classes in the training set");
    }

    vec_size = !as_sparse ? dims_selected*sizeof(row_sample[0]) :
                (dims_selected + 1)*sizeof(CvSparseVecElem32f);

    if(CC_IS_MAT(sample))
    {
        sample_data = sample->tt.fl;
        sample_step = sample->step / sizeof(row_sample[0]);

        if(!comp_idx && sample_step <= 1 && !as_sparse)
            *_row_sample = sample_data;
        else
        {
            CC_CALL(row_sample = (float*)cAlloc(vec_size));

            if(!comp_idx)
                for(i = 0; i < dims_selected; i++)
                    row_sample[i] = sample_data[sample_step*i];
            else
            {
                int* comp = comp_idx->tt.i;
                if(!sample_step)
                    for(i = 0; i < dims_selected; i++)
                        row_sample[i] = sample_data[comp[i]];
                else
                    for(i = 0; i < dims_selected; i++)
                        row_sample[i] = sample_data[sample_step*comp[i]];
            }

            *_row_sample = row_sample;
        }

        if(as_sparse)
        {
            const float* src = (const float*)row_sample;
            CvSparseVecElem32f* dst = (CvSparseVecElem32f*)row_sample;

            dst[dims_selected].idx = -1;
            for(i = dims_selected - 1; i >= 0; i--)
            {
                dst[i].idx = i;
                dst[i].val = src[i];
            }
        }
    }
    else
    {
        CvSparseNode* node;
        CvSparseMatIterator mat_iterator;
        const CvSparseMat* sparse = (const CvSparseMat*)sample;
        assert(is_sparse);

        node = cvInitSparseMatIterator(sparse, &mat_iterator);
        CC_CALL(row_sample = (float*)cAlloc(vec_size));

        if(comp_idx)
        {
            CC_CALL(inverse_comp_idx = (int*)cAlloc(dims_all*sizeof(int)));
            memset(inverse_comp_idx, -1, dims_all*sizeof(int));
            for(i = 0; i < dims_selected; i++)
                inverse_comp_idx[comp_idx->tt.i[i]] = i;
        }
        
        if(!as_sparse)
        {
            memset(row_sample, 0, vec_size);

            for(; node != 0; node = cvGetNextSparseNode(&mat_iterator))
            {
                int idx = *CC_NODE_IDX(sparse, node);
                if(inverse_comp_idx)
                {
                    idx = inverse_comp_idx[idx];
                    if(idx < 0)
                        continue;
                }
                row_sample[idx] = *(float*)CC_NODE_VAL(sparse, node);
            }
        }
        else
        {
            CvSparseVecElem32f* ptr = (CvSparseVecElem32f*)row_sample;
            
            for(; node != 0; node = cvGetNextSparseNode(&mat_iterator))
            {
                int idx = *CC_NODE_IDX(sparse, node);
                if(inverse_comp_idx)
                {
                    idx = inverse_comp_idx[idx];
                    if(idx < 0)
                        continue;
                }
                ptr->idx = idx;
                ptr->val = *(float*)CC_NODE_VAL(sparse, node);
                ptr++;
            }

            qsort(row_sample, ptr - (CvSparseVecElem32f*)row_sample,
                   sizeof(ptr[0]), icvCmpSparseVecElems);
            ptr->idx = -1;
        }

        *_row_sample = row_sample;
    }

    __END__;

    if(inverse_comp_idx)
        cFree(&inverse_comp_idx);

    if(cvGetErrStatus() < 0 && _row_sample)
    {
        cFree(&row_sample);
        *_row_sample = 0;
    }
}


static void
icvConvertDataToSparse(const uchar* src, int src_step, int src_type,
                        uchar* dst, int dst_step, int dst_type,
                        CSize size, int* idx)
{
    CC_FUNCNAME("icvConvertDataToSparse");

    __BEGIN__;
    
    int i, j;
    src_type = src_type;
    dst_type = dst_type;

    if(CC_MAT_CN(src_type) != 1 || CC_MAT_CN(dst_type) != 1)
        CC_ERROR(CC_StsUnsupportedFormat, "The function supports only single-channel arrays");

    if(src_step == 0)
        src_step = CC_ELEM_SIZE(src_type);

    if(dst_step == 0)
        dst_step = CC_ELEM_SIZE(dst_type);

    // if there is no "idx" and if both arrays are continuous,
    // do the whole processing (copying or conversion) in a single loop
    if(!idx && CC_ELEM_SIZE(src_type)*size.width == src_step &&
        CC_ELEM_SIZE(dst_type)*size.width == dst_step)
    {
        size.width *= size.height;
        size.height = 1;
    }

    if(src_type == dst_type)
    {
        int full_width = CC_ELEM_SIZE(dst_type)*size.width;
        
        if(full_width == sizeof(int)) // another common case: copy int's or float's
            for(i = 0; i < size.height; i++, src += src_step)
                *(int*)(dst + dst_step*(idx ? idx[i] : i)) = *(int*)src;
        else
            for(i = 0; i < size.height; i++, src += src_step)
                memcpy(dst + dst_step*(idx ? idx[i] : i), src, full_width);
    }
    else if(src_type == CC_32SC1 && (dst_type == CC_32F && cn==1 || dst_type == CC_64F && 1==cn))
        for(i = 0; i < size.height; i++, src += src_step)
        {
            uchar* _dst = dst + dst_step*(idx ? idx[i] : i);
            if(dst_type == CC_32F && cn==1)
                for(j = 0; j < size.width; j++)
                    ((float*)_dst)[j] = (float)((int*)src)[j];
            else
                for(j = 0; j < size.width; j++)
                    ((double*)_dst)[j] = ((int*)src)[j];
        }
    else if((src_type == CC_32F && cn==1 || src_type == CC_64F && 1==cn) && dst_type == CC_32SC1)
        for(i = 0; i < size.height; i++, src += src_step)
        {
            uchar* _dst = dst + dst_step*(idx ? idx[i] : i);
            if(src_type == CC_32F && cn==1)
                for(j = 0; j < size.width; j++)
                    ((int*)_dst)[j] = cRound(((float*)src)[j]);
            else
                for(j = 0; j < size.width; j++)
                    ((int*)_dst)[j] = cRound(((double*)src)[j]);
        }
    else if(src_type == CC_32F && cn==1 && dst_type == CC_64F && 1==cn ||
             src_type == CC_64F && 1==cn && dst_type == CC_32F && cn==1)
        for(i = 0; i < size.height; i++, src += src_step)
        {
            uchar* _dst = dst + dst_step*(idx ? idx[i] : i);
            if(src_type == CC_32F && cn==1)
                for(j = 0; j < size.width; j++)
                    ((double*)_dst)[j] = ((float*)src)[j];
            else
                for(j = 0; j < size.width; j++)
                    ((float*)_dst)[j] = (float)((double*)src)[j];
        }
    else
        CC_ERROR(CC_StsUnsupportedFormat, "Unsupported combination of input and output vectors");

    __END__;
}


void
cvWritebackLabels(const img_t* labels, img_t* dst_labels,
                   const img_t* centers, img_t* dst_centers,
                   const img_t* probs, img_t* dst_probs,
                   const img_t* sample_idx, int samples_all,
                   const img_t* comp_idx, int dims_all)
{
    CC_FUNCNAME("cvWritebackLabels");

    __BEGIN__;

    int samples_selected = samples_all, dims_selected = dims_all;

    if(dst_labels && !CC_IS_MAT(dst_labels))
        CC_ERROR(CC_StsBadArg, "Array of output labels is not a valid matrix");

    if(dst_centers)
        if(!ICC_IS_MAT_OF_TYPE(dst_centers, CC_32FC1) &&
            !ICC_IS_MAT_OF_TYPE(dst_centers, CC_64FC1))
            CC_ERROR(CC_StsBadArg, "Array of cluster centers is not a valid matrix");

    if(dst_probs && !CC_IS_MAT(dst_probs))
        CC_ERROR(CC_StsBadArg, "Probability matrix is not valid");

    if(sample_idx)
    {
        CC_ASSERT(sample_idx->rows == 1 && CC_MAT_TYPE(sample_idx->tid) == CC_32SC1);
        samples_selected = sample_idx->cols;
    }

    if(comp_idx)
    {
        CC_ASSERT(comp_idx->rows == 1 && CC_MAT_TYPE(comp_idx->tid) == CC_32SC1);
        dims_selected = comp_idx->cols;
    }

    if(dst_labels && (!labels || labels->tt.data != dst_labels->tt.data))
    {
        if(!labels)
            CC_ERROR(CC_StsNullPtr, "NULL labels");

        CC_ASSERT(labels->rows == 1);

        if(dst_labels->rows != 1 && dst_labels->cols != 1)
            CC_ERROR(CC_StsBadSize, "Array of output labels should be 1d vector");

        if(dst_labels->rows + dst_labels->cols - 1 != samples_all)
            CC_ERROR(CC_StsUnmatchedSizes, 
            "CSize of vector of output labels is not equal to the total number of input samples");

        CC_ASSERT(labels->cols == samples_selected);

        CC_CALL(icvConvertDataToSparse(labels->tt.data, labels->step, labels->tid,
                        dst_labels->tt.data, dst_labels->step, dst_labels->tid,
                        cSize(1, samples_selected), sample_idx ? sample_idx->tt.i : 0));
    }

    if(dst_centers && (!centers || centers->tt.data != dst_centers->tt.data))
    {
        int i;
        
        if(!centers)
            CC_ERROR(CC_StsNullPtr, "NULL centers");

        if(centers->rows != dst_centers->rows)
            CC_ERROR(CC_StsUnmatchedSizes, "Invalid number of rows in matrix of output centers");

        if(dst_centers->cols != dims_all)
            CC_ERROR(CC_StsUnmatchedSizes,
            "Number of columns in matrix of output centers is "
            "not equal to the total number of components in the input samples");

        CC_ASSERT(centers->cols == dims_selected);

        for(i = 0; i < centers->rows; i++)
            CC_CALL(icvConvertDataToSparse(centers->tt.data + i*centers->step, 0, centers->tid,
                        dst_centers->tt.data + i*dst_centers->step, 0, dst_centers->tid,
                        cSize(1, dims_selected), comp_idx ? comp_idx->tt.i : 0));
    }

    if(dst_probs && (!probs || probs->tt.data != dst_probs->tt.data))
    {
        if(!probs)
            CC_ERROR(CC_StsNullPtr, "NULL probs");

        if(probs->cols != dst_probs->cols)
            CC_ERROR(CC_StsUnmatchedSizes, "Invalid number of columns in output probability matrix");

        if(dst_probs->rows != samples_all)
            CC_ERROR(CC_StsUnmatchedSizes,
            "Number of rows in output probability matrix is "
            "not equal to the total number of input samples");

        CC_ASSERT(probs->rows == samples_selected);

        CC_CALL(icvConvertDataToSparse(probs->tt.data, probs->step, probs->tid,
                        dst_probs->tt.data, dst_probs->step, dst_probs->tid,
                        cSize(probs->cols, samples_selected),
                        sample_idx ? sample_idx->tt.i : 0));
    }

    __END__;
}

#if 0
CC_IMPL void
cvStatModelMultiPredict(const CvStatModel* stat_model,
                         const img_t* predict_input,
                         int flags, img_t* predict_output,
                         img_t* probs, const img_t* sample_idx)
{
    CvMemStorage* storage = 0;
    img_t* sample_idx_buffer = 0;
    CvSparseMat** sparse_rows = 0;
    int samples_selected = 0;
    
    CC_FUNCNAME("cvStatModelMultiPredict");

    __BEGIN__;

    int i;
    int predict_output_step = 1, sample_idx_step = 1;
    TypeId type;
    int d, sizes[CC_MAX_DIM];
    int tflag = flags == CC_COL_SAMPLE;
    int samples_all, dims_all;
    int is_sparse = CC_IS_SPARSE_MAT(predict_input);
    img_t predict_input_part;
    img_t* sample = &predict_input_part;
    img_t probs_part;
    img_t* probs1 = probs ? &probs_part : 0;
    
    if(!CC_IS_STAT_MODEL(stat_model))
        CC_ERROR(!stat_model ? CC_StsNullPtr : CC_StsBadArg, "Invalid statistical model");

    if(!stat_model->predict)
        CC_ERROR(CC_StsNotImplemented, "There is no \"predict\" method");

    if(!predict_input || !predict_output)
        CC_ERROR(CC_StsNullPtr, "NULL input or output matrices");

    if(!is_sparse && !CC_IS_MAT(predict_input))
        CC_ERROR(CC_StsBadArg, "predict_input should be a matrix or a sparse matrix");

    if(!CC_IS_MAT(predict_output))
        CC_ERROR(CC_StsBadArg, "predict_output should be a matrix");

    type = cvGetElemType(predict_input);
    if(type != CC_32F ||
        (CC_MAT_TYPE(predict_output->tid) != CC_32F &&
         CC_MAT_TYPE(predict_output->tid) != CC_32SC1))
         CC_ERROR(CC_StsUnsupportedFormat, "The input or output matrix has unsupported format");

    CC_CALL(d = cvGetDims(predict_input, sizes));
    if(d > 2)
        CC_ERROR(CC_StsBadSize, "The input matrix should be 1- or 2-dimensional");

    if(!tflag)
    {
        samples_all = samples_selected = sizes[0];
        dims_all = sizes[1];
    }
    else
    {
        samples_all = samples_selected = sizes[1];
        dims_all = sizes[0];
    }

    if(sample_idx)
    {
        if(!CC_IS_MAT(sample_idx))
            CC_ERROR(CC_StsBadArg, "Invalid sample_idx matrix");

        if(sample_idx->cols != 1 && sample_idx->rows != 1)
            CC_ERROR(CC_StsBadSize, "sample_idx must be 1-dimensional matrix");

        samples_selected = sample_idx->rows + sample_idx->cols - 1;

        if(CC_MAT_TYPE(sample_idx->tid) == CC_32SC1)
        {
            if(samples_selected > samples_all)
                CC_ERROR(CC_StsBadSize, "sample_idx is too large vector");
        }
        else if(samples_selected != samples_all)
            CC_ERROR(CC_StsUnmatchedSizes, "sample_idx has incorrect size");

        sample_idx_step = sample_idx->step ?
            sample_idx->step / CC_ELEM_SIZE(sample_idx->tid) : 1;
    }

    if(predict_output->rows != 1 && predict_output->cols != 1)
        CC_ERROR(CC_StsBadSize, "predict_output should be a 1-dimensional matrix");

    if(predict_output->rows + predict_output->cols - 1 != samples_all)
        CC_ERROR(CC_StsUnmatchedSizes, "predict_output and predict_input have uncoordinated sizes");

    predict_output_step = predict_output->step ?
        predict_output->step / CC_ELEM_SIZE(predict_output->tid) : 1;

    if(probs)
    {
        if(!CC_IS_MAT(probs))
            CC_ERROR(CC_StsBadArg, "Invalid matrix of probabilities");

        if(probs->rows != samples_all)
            CC_ERROR(CC_StsUnmatchedSizes,
            "matrix of probabilities must have as many rows as the total number of samples");

        if(CC_MAT_TYPE(probs->tid) != CC_32F)
            CC_ERROR(CC_StsUnsupportedFormat, "matrix of probabilities must have 32fC1 type");
    }

    if(is_sparse)
    {
        CvSparseNode* node;
        CvSparseMatIterator mat_iterator;
        CvSparseMat* sparse = (CvSparseMat*)predict_input;
        
        if(sample_idx && CC_MAT_TYPE(sample_idx->tid) == CC_32SC1)
        {
            CC_CALL(sample_idx_buffer = cvCreateMat(1, samples_all, CC_8UC1));
            cvZero(sample_idx_buffer);
            for(i = 0; i < samples_selected; i++)
                sample_idx_buffer->tt.data[sample_idx->tt.i[i*sample_idx_step]] = 1;
            samples_selected = samples_all;
            sample_idx = sample_idx_buffer;
            sample_idx_step = 1;
        }
        
        CC_CALL(sparse_rows = (CvSparseMat**)cAlloc(samples_selected*sizeof(sparse_rows[0])));
        for(i = 0; i < samples_selected; i++)
        {
            if(sample_idx && sample_idx->tt.data[i*sample_idx_step] == 0)
                continue;
            CC_CALL(sparse_rows[i] = cvCreateSparseMat(1, &dims_all, type));
            if(!storage)
                storage = sparse_rows[i]->heap->storage;
            else
            {
                // hack: to decrease memory footprint, make all the sparse matrices
                // reside in the same storage
                int elem_size = sparse_rows[i]->heap->elem_size;
                cvReleaseMemStorage(&sparse_rows[i]->heap->storage);
                sparse_rows[i]->heap = cvCreateSet(0, sizeof(CvSet), elem_size, storage);
            }
        }

        // put each row (or column) of predict_input into separate sparse matrix.
        node = cvInitSparseMatIterator(sparse, &mat_iterator);
        for(; node != 0; node = cvGetNextSparseNode(&mat_iterator))
        {
            int* idx = CC_NODE_IDX(sparse, node);
            int idx0 = idx[tflag ^ 1];
            int idx1 = idx[tflag];

            if(sample_idx && sample_idx->tt.data[idx0*sample_idx_step] == 0)
                continue;

            assert(sparse_rows[idx0] != 0);
            *(float*)cvPtrND(sparse, &idx1, 0, 1, 0) = *(float*)CC_NODE_VAL(sparse, node);
        }
    }

    for(i = 0; i < samples_selected; i++)
    {
        int idx = i;
        float response;
        
        if(sample_idx)
        {
            if(CC_MAT_TYPE(sample_idx->tid) == CC_32SC1)
            {
                idx = sample_idx->tt.i[i*sample_idx_step];
                if((unsigned)idx >= (unsigned)samples_all)
                    CC_ERROR(CC_StsOutOfRange, "Some of sample_idx elements are out of range");
            }
            else if(CC_MAT_TYPE(sample_idx->tid) == CC_8UC1 &&
                     sample_idx->tt.data[i*sample_idx_step] == 0)
                continue;
        }

        if(!is_sparse)
        {
            if(!tflag)
                cvGetRow(predict_input, &predict_input_part, idx);
            else
            {
                cvGetCol(predict_input, &predict_input_part, idx);
            }
        }
        else
            sample = sparse_rows[idx];

        if(probs)
            cvGetRow(probs, probs1, idx);

        CC_CALL(response = stat_model->predict(stat_model, (img_t*)sample, probs1));

        if(CC_MAT_TYPE(predict_output->tid) == CC_32FC1)
            predict_output->tt.fl[idx*predict_output_step] = response;
        else
        {
            CC_ASSERT(cRound(response) == response);
            predict_output->tt.i[idx*predict_output_step] = cRound(response);
        }
    }

    __END__;

    if(sparse_rows)
    {
        int i;
        for(i = 0; i < samples_selected; i++)
            if(sparse_rows[i])
            {
                sparse_rows[i]->heap->storage = 0;
                cvReleaseSparseMat(&sparse_rows[i]);
            }
        cFree(&sparse_rows);
    }

    cvReleaseMat(&sample_idx_buffer);
    cvReleaseMemStorage(&storage);
}
#endif

// By P. Yarykin - begin -

void cvCombineResponseMaps (img_t*  _responses,
                      const img_t*  old_response_map,
                            img_t*  new_response_map,
                            img_t** out_response_map)
{
    int** old_data = NULL;
    int** new_data = NULL;

        CC_FUNCNAME ("cvCombineResponseMaps");
        __BEGIN__

    int i,j;
    int old_n, new_n, out_n;
    int samples, free_response;
    int* first;
    int* responses;
    int* out_data;

    if(out_response_map)
        *out_response_map = 0;

// Check input data.
    if ((!ICC_IS_MAT_OF_TYPE (_responses, CC_32SC1)) ||
        (!ICC_IS_MAT_OF_TYPE (old_response_map, CC_32SC1)) ||
        (!ICC_IS_MAT_OF_TYPE (new_response_map, CC_32SC1)))
    {
        CC_ERROR (CC_StsBadArg, "Some of input arguments is not the img_t")
    }

// Prepare sorted responses.
    first = new_response_map->tt.i;
    new_n = new_response_map->cols;
    CC_CALL (new_data = (int**)cAlloc (new_n * sizeof (new_data[0])));
    for (i = 0; i < new_n; i++)
        new_data[i] = first + i;
    qsort (new_data, new_n, sizeof(int*), icvCmpIntegersPtr);

    first = old_response_map->tt.i;
    old_n = old_response_map->cols;
    CC_CALL (old_data = (int**)cAlloc (old_n * sizeof (old_data[0])));
    for (i = 0; i < old_n; i++)
        old_data[i] = first + i;
    qsort (old_data, old_n, sizeof(int*), icvCmpIntegersPtr);

// Count the number of different responses.
    for (i = 0, j = 0, out_n = 0; i < old_n && j < new_n; out_n++)
    {
        if (*old_data[i] == *new_data[j])
        {
            i++;
            j++;
        }
        else if (*old_data[i] < *new_data[j])
            i++;
        else
            j++;
    }
    out_n += old_n - i + new_n - j;

// Create and fill the result response maps.
    CC_CALL (*out_response_map = cvCreateMat (1, out_n, CC_32SC1));
    out_data = (*out_response_map)->tt.i;
    memcpy (out_data, first, old_n * sizeof (int));

    free_response = old_n;
    for (i = 0, j = 0; i < old_n && j < new_n;)
    {
        if (*old_data[i] == *new_data[j])
        {
            *new_data[j] = (int)(old_data[i] - first);
            i++;
            j++;
        }
        else if (*old_data[i] < *new_data[j])
            i++;
        else
        {
            out_data[free_response] = *new_data[j];
            *new_data[j] = free_response++;
            j++;
        }
    }
    for (; j < new_n; j++)
    {
        out_data[free_response] = *new_data[j];
        *new_data[j] = free_response++;
    }
    CC_ASSERT (free_response == out_n);

// Change <responses> according to out response map.
    samples = _responses->cols + _responses->rows - 1;
    responses = _responses->tt.i;
    first = new_response_map->tt.i;
    for (i = 0; i < samples; i++)
    {
        responses[i] = first[responses[i]];
    }

        __END__

    cFree(&old_data);
    cFree(&new_data);

}


int icvGetNumberOfCluster(double* prob_vector, int num_of_clusters, float r,
                           float outlier_thresh, int normalize_probs)
{
    int max_prob_loc = 0;
   
    CC_FUNCNAME("icvGetNumberOfCluster");
    __BEGIN__;

    double prob, maxprob, sum;
    int i;

    CC_ASSERT(prob_vector);
    CC_ASSERT(num_of_clusters >= 0);

    maxprob = prob_vector[0];
    max_prob_loc = 0;
    sum = maxprob;
    for(i = 1; i < num_of_clusters; i++)
    {
        prob = prob_vector[i];
        sum += prob;
        if(prob > maxprob) 
        {
            max_prob_loc = i;
            maxprob = prob;
        }
    }
    if(normalize_probs && fabs(sum - 1.) > FLT_EPSILON)
    {
        for(i = 0; i < num_of_clusters; i++)
            prob_vector[i] /= sum;
    }
    if(fabs(r - 1.) > FLT_EPSILON && fabs(sum - 1.) < outlier_thresh)
        max_prob_loc = -1;

    __END__;

    return max_prob_loc;

} // End of icvGetNumberOfCluster


void icvFindClusterLabels(const img_t* probs, float outlier_thresh, float r,
                          const img_t* labels)
{
    img_t* counts = 0;

    CC_FUNCNAME("icvFindClusterLabels");
    __BEGIN__;

    int nclusters, nsamples;
    int i, j;
    double* probs_data;

    CC_ASSERT(ICC_IS_MAT_OF_TYPE(probs, CC_64FC1));
    CC_ASSERT(ICC_IS_MAT_OF_TYPE(labels, CC_32SC1));

    nclusters = probs->cols;
    nsamples  = probs->rows;
    CC_ASSERT(nsamples == labels->cols);

    CC_CALL(counts = cvCreateMat(1, nclusters + 1, CC_32SC1));
    CC_CALL(cvSetZero(counts));
    for(i = 0; i < nsamples; i++)
    {
        labels->tt.i[i] = icvGetNumberOfCluster(probs->tt.db + i*probs->cols,
            nclusters, r, outlier_thresh, 1);
        counts->tt.i[labels->tt.i[i] + 1]++;
    }
    CC_ASSERT((int)cvSum(counts).val[0] == nsamples);
    // Filling empty clusters with the vector, that has the maximal probability
    for(j = 0; j < nclusters; j++) // outliers are ignored
    {
        int maxprob_loc = -1;
        double maxprob = 0;

        if(counts->tt.i[j+1]) // j-th class is not empty
            continue;
        // look for the presentative, which is not lonely in it's cluster
        // and that has a maximal probability among all these vectors
        probs_data = probs->tt.db;
        for(i = 0; i < nsamples; i++, probs_data++)
        {
            int label = labels->tt.i[i];
            double prob;
            if(counts->tt.i[label+1] == 0 ||
                (counts->tt.i[label+1] <= 1 && label != -1))
                continue;
            prob = *probs_data;
            if(prob >= maxprob)
            {
                maxprob = prob;
                maxprob_loc = i;
            }
        }
        // maxprob_loc == 0 <=> number of vectors less then number of clusters
        CC_ASSERT(maxprob_loc >= 0);
        counts->tt.i[labels->tt.i[maxprob_loc] + 1]--;
        labels->tt.i[maxprob_loc] = j;
        counts->tt.i[j + 1]++;
    }

    __END__;

    cvReleaseMat(&counts);
} // End of icvFindClusterLabels

/* End of file */
