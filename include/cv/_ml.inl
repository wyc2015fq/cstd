
#ifndef __ML_INTERNAL_H__
#define __ML_INTERNAL_H__

#if _MSC_VER >= 1200
#pragma warning(disable: 4514 4710 4711 4710)
#endif

#include "ml.h"
#include "cxmisc.h"

#include <assert.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifndef FALSE
#define FALSE 0
#endif
#ifndef TRUE
#define TRUE 1
#endif

#define ML_IMPL CC_IMPL

#define CC_MAT_ELEM_FLAG(mat, type, comp, vect, tflag)    \
    ((tflag == CC_ROW_SAMPLE)                             \
    ? (CC_MAT_ELEM(mat, type, comp, vect))                \
    : (CC_MAT_ELEM(mat, type, vect, comp)))

/* Convert matrix to vector */
#define ICC_MAT2VEC(mat, vdata, vstep, num)      \
    if(MIN((mat).rows, (mat).cols) != 1)       \
        CC_ERROR(CC_StsBadArg, "");              \
    (vdata) = ((mat).tt.data);                    \
    if((mat).rows == 1)                          \
    {                                              \
        (vstep) = CC_ELEM_SIZE((mat).type);      \
        (num) = (mat).cols;                        \
    }                                              \
    else                                           \
    {                                              \
        (vstep) = (mat).step;                      \
        (num) = (mat).rows;                        \
    }

/* get raw data */
#define ICC_RAWDATA(mat, flags, rdata, sstep, cstep, m, n)         \
    (rdata) = (mat).tt.data;                                        \
    if(CC_IS_ROW_SAMPLE(flags))                                  \
    {                                                                \
        (sstep) = (mat).step;                                        \
        (cstep) = CC_ELEM_SIZE((mat).type);                        \
        (m) = (mat).rows;                                            \
        (n) = (mat).cols;                                            \
    }                                                                \
    else                                                             \
    {                                                                \
        (cstep) = (mat).step;                                        \
        (sstep) = CC_ELEM_SIZE((mat).type);                        \
        (n) = (mat).rows;                                            \
        (m) = (mat).cols;                                            \
    }

#define ICC_IS_MAT_OF_TYPE(mat, mat_type) (CC_MAT_TYPE(mat) == (mat_type) && (mat)->cols > 0 && (mat)->rows > 0)

/*
    uchar* data; int sstep, cstep;      - trainData->data
    uchar* classes; int clstep; int ncl;- trainClasses
    uchar* tmask; int tmstep; int ntm;  - typeMask
    uchar* missed;int msstep, mcstep;   -missedMeasurements...
    int mm, mn;                         == m,n == size,dim
    uchar* sidx;int sistep;             - sampleIdx
    uchar* cidx;int cistep;             - compIdx
    int k, l;                           == n,m == dim,size (length of cidx, sidx)
    int m, n;                           == size,dim
*/
#define ICC_DECLARE_TRAIN_ARGS()                                                    \
    uchar* data;                                                                    \
    int sstep, cstep;                                                               \
    uchar* classes;                                                                 \
    int clstep;                                                                     \
    int ncl;                                                                        \
    uchar* tmask;                                                                   \
    int tmstep;                                                                     \
    int ntm;                                                                        \
    uchar* missed;                                                                  \
    int msstep, mcstep;                                                             \
    int mm, mn;                                                                     \
    uchar* sidx;                                                                    \
    int sistep;                                                                     \
    uchar* cidx;                                                                    \
    int cistep;                                                                     \
    int k, l;                                                                       \
    int m, n;                                                                       \
                                                                                    \
    data = classes = tmask = missed = sidx = cidx = NULL;                           \
    sstep = cstep = clstep = ncl = tmstep = ntm = msstep = mcstep = mm = mn = 0;    \
    sistep = cistep = k = l = m = n = 0;

#define ICC_TRAIN_DATA_REQUIRED(param, flags)                                     \
    if(!ICC_IS_MAT_OF_TYPE((param), CC_32FC1))                                  \
    {                                                                               \
        CC_ERROR(CC_StsBadArg, "Invalid " #param " parameter");                   \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        ICC_RAWDATA(*(param), (flags), data, sstep, cstep, m, n);                 \
        k = n;                                                                      \
        l = m;                                                                      \
    }

#define ICC_TRAIN_CLASSES_REQUIRED(param)                                         \
    if(!ICC_IS_MAT_OF_TYPE((param), CC_32FC1))                                  \
    {                                                                               \
        CC_ERROR(CC_StsBadArg, "Invalid " #param " parameter");                   \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        ICC_MAT2VEC(*(param), classes, clstep, ncl);                              \
        if(m != ncl)                                                              \
        {                                                                           \
            CC_ERROR(CC_StsBadArg, "Unmatched sizes");                            \
        }                                                                           \
    }

#define ICC_ARG_NULL(param)                                                       \
    if((param) != NULL)                                                           \
    {                                                                               \
        CC_ERROR(CC_StsBadArg, #param " parameter must be NULL");                 \
    }

#define ICC_MISSED_MEASUREMENTS_OPTIONAL(param, flags)                            \
    if(param)                                                                     \
    {                                                                               \
        if(!ICC_IS_MAT_OF_TYPE(param, CC_8UC1))                                 \
        {                                                                           \
            CC_ERROR(CC_StsBadArg, "Invalid " #param " parameter");               \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            ICC_RAWDATA(*(param), (flags), missed, msstep, mcstep, mm, mn);       \
            if(mm != m || mn != n)                                                \
            {                                                                       \
                CC_ERROR(CC_StsBadArg, "Unmatched sizes");                        \
            }                                                                       \
        }                                                                           \
    }

#define ICC_COMP_IDX_OPTIONAL(param)                                              \
    if(param)                                                                     \
    {                                                                               \
        if(!ICC_IS_MAT_OF_TYPE(param, CC_32SC1))                                \
        {                                                                           \
            CC_ERROR(CC_StsBadArg, "Invalid " #param " parameter");               \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            ICC_MAT2VEC(*(param), cidx, cistep, k);                               \
            if(k > n)                                                             \
                CC_ERROR(CC_StsBadArg, "Invalid " #param " parameter");           \
        }                                                                           \
    }

#define ICC_SAMPLE_IDX_OPTIONAL(param)                                            \
    if(param)                                                                     \
    {                                                                               \
        if(!ICC_IS_MAT_OF_TYPE(param, CC_32SC1))                                \
        {                                                                           \
            CC_ERROR(CC_StsBadArg, "Invalid " #param " parameter");               \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            ICC_MAT2VEC(*sampleIdx, sidx, sistep, l);                             \
            if(l > m)                                                             \
                CC_ERROR(CC_StsBadArg, "Invalid " #param " parameter");           \
        }                                                                           \
    }

/****************************************************************************************/
#define ICC_CONVERT_FLOAT_ARRAY_TO_MATRICE(array, matrice)        \
{                                                                   \
    img_t a, b;                                                     \
    int dims = (matrice)->cols;                                     \
    int nsamples = (matrice)->rows;                                 \
    int type = CC_MAT_DEPTH((matrice)->tid);                        \
    int i, offset = dims;                                           \
                                                                    \
    CC_ASSERT(type == CC_32F && cn==1 || type == CC_64F && 1==cn);              \
    offset *= ((type == CC_32F && cn==1) ? sizeof(float) : sizeof(double));\
                                                                    \
    b = cvMat(1, dims, CC_32FC1);                                 \
    cvGetRow(matrice, &a, 0);                                     \
    for(i = 0; i < nsamples; i++, a.tt.data += offset)           \
    {                                                               \
        b.tt.fl = (float*)array[i];                               \
        CC_CALL(cvConvert(&b, &a));                             \
    }                                                               \
}

/****************************************************************************************\
*                       Auxiliary functions declarations                                 *
\****************************************************************************************/

/* Generates a set of classes centers in quantity <num_of_clusters> that are generated as
   uniform random vectors in parallelepiped, where <data> is concentrated. Vectors in
   <data> should have horizontal orientation. If <centers> != NULL, the function doesn't
   allocate any memory and stores generated centers in <centers>, returns <centers>.
   If <centers> == NULL, the function allocates memory and creates the matrice. Centers
   are supposed to be oriented horizontally. */
img_t* icvGenerateRandomClusterCenters(int seed,
                                        const img_t* data,
                                        int num_of_clusters,
                                        img_t* centers CC_DEFAULT(0));

/* Fills the <labels> using <probs> by choosing the maximal probability. Outliers are
   fixed by <oulier_tresh> and have cluster label (-1). Function also controls that there
   weren't "empty" clusters by filling empty clusters with the maximal probability vector.
   If probs_sums != NULL, filles it with the sums of probabilities for each sample (it is
   useful for normalizing probabilities' matrice of FCM) */
void icvFindClusterLabels(const img_t* probs, float outlier_thresh, float r,
                           const img_t* labels);

typedef struct CvSparseVecElem32f
{
    int idx;
    float val;
}
CvSparseVecElem32f;

/* Prepare training data and related parameters */
#define CC_TRAIN_STATMODEL_DEFRAGMENT_TRAIN_DATA    1
#define CC_TRAIN_STATMODEL_SAMPLES_AS_ROWS          2
#define CC_TRAIN_STATMODEL_SAMPLES_AS_COLUMNS       4
#define CC_TRAIN_STATMODEL_CATEGORICAL_RESPONSE     8
#define CC_TRAIN_STATMODEL_ORDERED_RESPONSE         16
#define CC_TRAIN_STATMODEL_RESPONSES_ON_OUTPUT      32
#define CC_TRAIN_STATMODEL_ALWAYS_COPY_TRAIN_DATA   64
#define CC_TRAIN_STATMODEL_SPARSE_AS_SPARSE         128

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
                    img_t** out_sample_idx=0);

void
cvSortSamplesByClasses(const float** samples, const img_t* classes, 
                        int* class_ranges, const uchar** mask CC_DEFAULT(0));

void 
cvCombineResponseMaps (img_t*  _responses,
                 const img_t*  old_response_map,
                       img_t*  new_response_map,
                       img_t** out_response_map);

void
cvPreparePredictData(const img_t* sample, int dims_all, const img_t* comp_idx,
                      int class_count, const img_t* prob, float** row_sample,
                      int as_sparse CC_DEFAULT(0));

/* copies clustering [or batch "predict"] results
   (labels and/or centers and/or probs) back to the output arrays */
void
cvWritebackLabels(const img_t* labels, img_t* dst_labels,
                   const img_t* centers, img_t* dst_centers,
                   const img_t* probs, img_t* dst_probs,
                   const img_t* sample_idx, int samples_all,
                   const img_t* comp_idx, int dims_all);
#define cvWritebackResponses cvWritebackLabels

#define XML_FIELD_NAME "_name"
CvFileNode* icvFileNodeGetChild(CvFileNode* father, const char* name);
CvFileNode* icvFileNodeGetChildArrayElem(CvFileNode* father, const char* name,int index);
CvFileNode* icvFileNodeGetNext(CvFileNode* n, const char* name);


void cvCheckTrainData(const img_t* train_data, int tflag,
                       const img_t* missing_mask, 
                       int* var_all, int* sample_all);

img_t* cvPreprocessIndexArray(const img_t* idx_arr, int data_arr_size, bool check_for_duplicates=false);

img_t* cvPreprocessVarType(const img_t* type_mask, const img_t* var_idx,
                            int var_all, int* response_type);

img_t* cvPreprocessOrderedResponses(const img_t* responses,
                const img_t* sample_idx, int sample_all);

img_t* cvPreprocessCategoricalResponses(const img_t* responses,
                const img_t* sample_idx, int sample_all,
                img_t** out_response_map, img_t** class_counts=0);

const float** cvGetTrainSamples(const img_t* train_data, int tflag,
                   const img_t* var_idx, const img_t* sample_idx,
                   int* _var_count, int* _sample_count,
                   bool always_copy_data=false);

#endif /* __ML_H__ */
