
#include "_ml.h"

#if 0
/****************************************************************************************\
*                         Auxilary functions declarations                                *
\****************************************************************************************/
/*---------------------- functions for the CNN classifier ------------------------------*/
static float icvCNNModelPredict(
        const CvStatModel* cnn_model,
        const img_t* image,
        img_t* probs CC_DEFAULT(0));

static void icvCNNModelUpdate(
        CvStatModel* cnn_model, const img_t* images, int tflag,
        const img_t* responses, const CvStatModelParams* params,
        const img_t* CC_DEFAULT(0), const img_t* sample_idx CC_DEFAULT(0),
        const img_t* CC_DEFAULT(0), const img_t* CC_DEFAULT(0));

static void icvCNNModelRelease(CvStatModel** cnn_model);

static void icvTrainCNNetwork(CvCNNetwork* network,
                               const float** images,
                               const img_t* responses,
                               const img_t* etalons,
                               int grad_estim_type,
                               int max_iter,
                               int start_iter);

/*------------------------- functions for the CNN network ------------------------------*/
static void icvCNNetworkAddLayer(CvCNNetwork* network, CvCNNLayer* layer);
static void icvCNNetworkRelease(CvCNNetwork** network);

/* In all layer functions we denote input by X and output by Y, where
   X and Y are column-vectors, so that
   length(X)==<n_input_planes>*<input_height>*<input_width>,
   length(Y)==<n_output_planes>*<output_height>*<output_width>.
*/
/*------------------------ functions for convolutional layer ---------------------------*/
static void icvCNNConvolutionRelease(CvCNNLayer** p_layer);

static void icvCNNConvolutionForward(CvCNNLayer* layer, const img_t* X, img_t* Y);

static void icvCNNConvolutionBackward(CvCNNLayer*  layer, int t,
    const img_t* X, const img_t* dE_dY, img_t* dE_dX);

/*------------------------ functions for sub-sampling layer ----------------------------*/
static void icvCNNSubSamplingRelease(CvCNNLayer** p_layer);

static void icvCNNSubSamplingForward(CvCNNLayer* layer, const img_t* X, img_t* Y);

static void icvCNNSubSamplingBackward(CvCNNLayer*  layer, int t,
    const img_t* X, const img_t* dE_dY, img_t* dE_dX);

/*------------------------ functions for full connected layer --------------------------*/
static void icvCNNFullConnectRelease(CvCNNLayer** p_layer);

static void icvCNNFullConnectForward(CvCNNLayer* layer, const img_t* X, img_t* Y);

static void icvCNNFullConnectBackward(CvCNNLayer* layer, int,
    const img_t*, const img_t* dE_dY, img_t* dE_dX);

/****************************************************************************************\
*                             Functions implementations                                  *
\****************************************************************************************/

#define ICC_CHECK_CNN_NETWORK(network)                                                  \
{                                                                                       \
    CvCNNLayer* first_layer, *layer, *last_layer;                                       \
    int n_layers, i;                                                                    \
    if(!network)                                                                      \
        CC_ERROR(CC_StsNullPtr,                                                        \
        "Null <network> pointer. Network must be created by user.");                   \
    n_layers = network->n_layers;                                                       \
    first_layer = last_layer = network->layers;                                         \
    for(i = 0, layer = first_layer; i < n_layers && layer; i++)                       \
    {                                                                                   \
        if(!ICC_IS_CNN_LAYER(layer))                                                  \
            CC_ERROR(CC_StsNullPtr, "Invalid network");                               \
        last_layer = layer;                                                             \
        layer = layer->next_layer;                                                      \
    }                                                                                   \
                                                                                        \
    if(i == 0 || i != n_layers || first_layer->prev_layer || layer)                   \
        CC_ERROR(CC_StsNullPtr, "Invalid network");                                   \
                                                                                        \
    if(first_layer->n_input_planes != 1)                                              \
        CC_ERROR(CC_StsBadArg, "First layer must contain only one input plane");      \
                                                                                        \
    if(img_size != first_layer->input_height*first_layer->input_width)                \
        CC_ERROR(CC_StsBadArg, "Invalid input sizes of the first layer");             \
                                                                                        \
    if(params->etalons->cols != last_layer->n_output_planes*                           \
        last_layer->output_height*last_layer->output_width)                            \
        CC_ERROR(CC_StsBadArg, "Invalid output sizes of the last layer");             \
}

#define ICC_CHECK_CNN_MODEL_PARAMS(params)                                              \
{                                                                                       \
    if(!params)                                                                       \
        CC_ERROR(CC_StsNullPtr, "Null <params> pointer");                             \
                                                                                        \
    if(!ICC_IS_MAT_OF_TYPE(params->etalons, CC_32FC1))                                \
        CC_ERROR(CC_StsBadArg, "<etalons> must be CC_32FC1 type");                    \
    if(params->etalons->rows != cnn_model->cls_labels->cols)                          \
        CC_ERROR(CC_StsBadArg, "Invalid <etalons> size");                             \
                                                                                        \
    if(params->grad_estim_type != CC_CNN_GRAD_ESTIM_RANDOM &&                          \
        params->grad_estim_type != CC_CNN_GRAD_ESTIM_BY_WORST_IMG)                     \
        CC_ERROR(CC_StsBadArg, "Invalid <grad_estim_type>");                          \
                                                                                        \
    if(params->start_iter < 0)                                                        \
        CC_ERROR(CC_StsBadArg, "Parameter <start_iter> must be positive or zero");    \
                                                                                        \
    if(params->max_iter < 1)                                                \
        params->max_iter = 1;                                                 \
}

/****************************************************************************************\
*                              Classifier functions                                      *
\****************************************************************************************/
ML_IMPL CvStatModel*
cvTrainCNNClassifier(const img_t* _train_data, int tflag,
            const img_t* _responses,
            const CvStatModelParams* _params,
            const img_t*, const img_t* _sample_idx, const img_t*, const img_t*)
{
    CvCNNStatModel* cnn_model    = 0;
    const float** out_train_data = 0;
    img_t* responses             = 0;

    CC_FUNCNAME("cvTrainCNNClassifier");
    __BEGIN__;

    int n_images;
    int img_size;
    CvCNNStatModelParams* params = (CvCNNStatModelParams*)_params;

    CC_CALL(cnn_model = (CvCNNStatModel*)cvCreateStatModel(
        CC_STAT_MODEL_MAGIC_VAL|CC_CNN_MAGIC_VAL, sizeof(CvCNNStatModel),
        icvCNNModelRelease, icvCNNModelPredict, icvCNNModelUpdate));

    CC_CALL(cvPrepareTrainData("cvTrainCNNClassifier",
        _train_data, tflag, _responses, CC_VAR_CATEGORICAL,
        0, _sample_idx, false, &out_train_data,
        &n_images, &img_size, &img_size, &responses,
        &cnn_model->cls_labels, 0));

    ICC_CHECK_CNN_MODEL_PARAMS(params);
    ICC_CHECK_CNN_NETWORK(params->network);

    cnn_model->network = params->network;
    CC_CALL(cnn_model->etalons = cvClone(params->etalons));

    CC_CALL(icvTrainCNNetwork(cnn_model->network, out_train_data, responses,
        cnn_model->etalons, params->grad_estim_type, params->max_iter,
        params->start_iter));

    __END__;

    if(cvGetErrStatus() < 0 && cnn_model)
    {
        cnn_model->release((CvStatModel**)&cnn_model);
    }
    cFree(&out_train_data);
    cvReleaseMat(&responses);

    return (CvStatModel*)cnn_model;
}

/****************************************************************************************/
static void icvTrainCNNetwork(CvCNNetwork* network,
                               const float** images,
                               const img_t* responses,
                               const img_t* etalons,
                               int grad_estim_type,
                               int max_iter,
                               int start_iter)
{
    img_t** X     = 0;
    img_t** dE_dX = 0;
    const int n_layers = network->n_layers;
    int k;

    CC_FUNCNAME("icvTrainCNNetwork");
    __BEGIN__;

    CvCNNLayer* first_layer = network->layers;
    const int img_height = first_layer->input_height;
    const int img_width  = first_layer->input_width;
    const int img_size   = img_width*img_height;
    const int n_images   = responses->cols;
    img_t image = cvMat(1, img_size, CC_32FC1);
    CvCNNLayer* layer;
    int n;
    CRNG rng = cRNG(-1);

    CC_CALL(X = (img_t**)cAlloc((n_layers+1)*sizeof(img_t*)));
    CC_CALL(dE_dX = (img_t**)cAlloc((n_layers+1)*sizeof(img_t*)));
    memset(X, 0, (n_layers+1)*sizeof(img_t*));
    memset(dE_dX, 0, (n_layers+1)*sizeof(img_t*));

    CC_CALL(X[0] = cvCreateMat(img_height*img_width,1,CC_32FC1));
    CC_CALL(dE_dX[0] = cvCreateMat(1, X[0]->rows, CC_32FC1));
    for(k = 0, layer = first_layer; k < n_layers; k++, layer = layer->next_layer)
    {
        CC_CALL(X[k+1] = cvCreateMat(layer->n_output_planes*layer->output_height*
            layer->output_width, 1, CC_32FC1));
        CC_CALL(dE_dX[k+1] = cvCreateMat(1, X[k+1]->rows, CC_32FC1));
    }

    for(n = 1; n <= max_iter; n++)
    {
        float loss, max_loss = 0;
        int i;
        int worst_img_idx = -1;
        int* right_etal_idx = responses->tt.i;
        img_t etalon;

        // Find the worst image (which produces the greatest loss) or use the random image
        if(grad_estim_type == CC_CNN_GRAD_ESTIM_BY_WORST_IMG)
        {
            for(i = 0; i < n_images; i++, right_etal_idx++)
            {
                image->tt.fl = (float*)images[i];
                cvTranspose(&image, X[0]);
                
                for(k = 0, layer = first_layer; k < n_layers; k++, layer = layer->next_layer)
                    CC_CALL(layer->forward(layer, X[k], X[k+1]));
                
                cvTranspose(X[n_layers], dE_dX[n_layers]);
                cvGetRow(etalons, &etalon, *right_etal_idx);
                loss = (float)cvNorm(dE_dX[n_layers], &etalon);
                if(loss > max_loss)
                {
                    max_loss = loss;
                    worst_img_idx = i;
                }
            }
        }
        else
            worst_img_idx = cvRandInt(&rng) % n_images;

        // Train network on the worst image
        // 1) Compute the network output on the <image>
        image->tt.fl = (float*)images[worst_img_idx];
        CC_CALL(cvTranspose(&image, X[0]));

        for(k = 0, layer = first_layer; k < n_layers - 1; k++, layer = layer->next_layer)
            CC_CALL(layer->forward(layer, X[k], X[k+1]));
        CC_CALL(layer->forward(layer, X[k], X[k+1]));

        // 2) Compute the gradient
        cvTranspose(X[n_layers], dE_dX[n_layers]);
        cvGetRow(etalons, &etalon, responses->tt.i[worst_img_idx]);
        cvSub(dE_dX[n_layers], &etalon, dE_dX[n_layers]);

        // 3) Update weights by the gradient descent
        for(k = n_layers; k > 0; k--, layer = layer->prev_layer)
            CC_CALL(layer->backward(layer, n + start_iter, X[k-1], dE_dX[k], dE_dX[k-1]));
    }

    __END__;

    for(k = 0; k <= n_layers; k++)
    {
        cvReleaseMat(&X[k]);
        cvReleaseMat(&dE_dX[k]);
    }
    cFree(&X);
    cFree(&dE_dX);
}

/****************************************************************************************/
static float icvCNNModelPredict(const CvStatModel* model,
                                 const img_t* _image,
                                 img_t* probs)
{
    img_t** X       = 0;
    float* img_data = 0;
    int n_layers = 0;
    int best_etal_idx = -1;
    int k;

    CC_FUNCNAME("icvCNNModelPredict");
    __BEGIN__;

    CvCNNStatModel* cnn_model = (CvCNNStatModel*)model;
    CvCNNLayer* first_layer, *layer = 0;
    int img_height, img_width, img_size;
    int nclasses, i;
    float loss, min_loss = FLT_MAX;
    float* probs_data;
    img_t etalon, image;

    if(!CC_IS_CNN(model))
        CC_ERROR(CC_StsBadArg, "Invalid model");

    nclasses = cnn_model->cls_labels->cols;
    n_layers = cnn_model->network->n_layers;
    first_layer   = cnn_model->network->layers;
    img_height = first_layer->input_height;
    img_width  = first_layer->input_width;
    img_size   = img_height*img_width;

    cvPreparePredictData(_image, img_size, 0, nclasses, probs, &img_data);

    CC_CALL(X = (img_t**)cAlloc((n_layers+1)*sizeof(img_t*)));
    memset(X, 0, (n_layers+1)*sizeof(img_t*));

    CC_CALL(X[0] = cvCreateMat(img_size,1,CC_32FC1));
    for(k = 0, layer = first_layer; k < n_layers; k++, layer = layer->next_layer)
    {
        CC_CALL(X[k+1] = cvCreateMat(layer->n_output_planes*layer->output_height*
            layer->output_width, 1, CC_32FC1));
    }

    image = cvMat(1, img_size, CC_32FC1, img_data);
    cvTranspose(&image, X[0]);
    for(k = 0, layer = first_layer; k < n_layers; k++, layer = layer->next_layer)
        CC_CALL(layer->forward(layer, X[k], X[k+1]));

    probs_data = probs ? probs->tt.fl : 0;
    etalon = cvMat(cnn_model->etalons->cols, 1, CC_32FC1, cnn_model->etalons->tt.fl);
    for(i = 0; i < nclasses; i++, etalon->tt.fl += cnn_model->etalons->cols)
    {
        loss = (float)cvNorm(X[n_layers], &etalon);
        if(loss < min_loss)
        {
            min_loss = loss;
            best_etal_idx = i;
        }
        if(probs)
            *probs_data++ = -loss;
    }

    if(probs)
    {
        cvExp(probs, probs);
        CScalar sum = cvSum(probs);
        cvConvertScale(probs, probs, 1./sum.val[0]);
    }

    __END__;

    for(k = 0; k <= n_layers; k++)
        cvReleaseMat(&X[k]);
    cFree(&X);
    if(img_data != _image->tt.fl)
        cFree(&img_data);

    return ((float) ((CvCNNStatModel*)model)->cls_labels->tt.i[best_etal_idx]);
}

/****************************************************************************************/
static void icvCNNModelUpdate(
        CvStatModel* _cnn_model, const img_t* _train_data, int tflag,
        const img_t* _responses, const CvStatModelParams* _params,
        const img_t*, const img_t* _sample_idx,
        const img_t*, const img_t*)
{
    const float** out_train_data = 0;
    img_t* responses             = 0;
    img_t* cls_labels            = 0;

    CC_FUNCNAME("icvCNNModelUpdate");
    __BEGIN__;

    int n_images, img_size, i;
    CvCNNStatModelParams* params = (CvCNNStatModelParams*)_params;
    CvCNNStatModel* cnn_model = (CvCNNStatModel*)_cnn_model;

    if(!CC_IS_CNN(cnn_model))
        CC_ERROR(CC_StsBadArg, "Invalid model");

    CC_CALL(cvPrepareTrainData("cvTrainCNNClassifier",
        _train_data, tflag, _responses, CC_VAR_CATEGORICAL,
        0, _sample_idx, false, &out_train_data,
        &n_images, &img_size, &img_size, &responses,
        &cls_labels, 0, 0));

    ICC_CHECK_CNN_MODEL_PARAMS(params);

    // Number of classes must be the same as when classifiers was created
    if(!CC_ARE_SIZES_EQ(cls_labels, cnn_model->cls_labels))
        CC_ERROR(CC_StsBadArg, "Number of classes must be left unchanged");
    for(i = 0; i < cls_labels->cols; i++)
    {
        if(cls_labels->tt.i[i] != cnn_model->cls_labels->tt.i[i])
            CC_ERROR(CC_StsBadArg, "Number of classes must be left unchanged");
    }

    CC_CALL(icvTrainCNNetwork(cnn_model->network, out_train_data, responses,
        cnn_model->etalons, params->grad_estim_type, params->max_iter,
        params->start_iter));

    __END__;

    cFree(&out_train_data);
    cvReleaseMat(&responses);
}

/****************************************************************************************/
static void icvCNNModelRelease(CvStatModel** cnn_model)
{
    CC_FUNCNAME("icvCNNModelRelease");
    __BEGIN__;

    CvCNNStatModel* cnn;
    if(!cnn_model)
        CC_ERROR(CC_StsNullPtr, "Null double pointer");

    cnn = *(CvCNNStatModel**)cnn_model;

    cvReleaseMat(&cnn->cls_labels);
    cvReleaseMat(&cnn->etalons);
    cnn->network->release(&cnn->network);

    cFree(&cnn);

    __END__;

}

/****************************************************************************************\
*                                 Network functions                                      *
\****************************************************************************************/
ML_IMPL CvCNNetwork* cvCreateCNNetwork(CvCNNLayer* first_layer)
{
    CvCNNetwork* network = 0;    
    
    CC_FUNCNAME("cvCreateCNNetwork");
    __BEGIN__;

    if(!ICC_IS_CNN_LAYER(first_layer))
        CC_ERROR(CC_StsBadArg, "Invalid layer");

    CC_CALL(network = (CvCNNetwork*)cAlloc(sizeof(CvCNNetwork)));
    memset(network, 0, sizeof(CvCNNetwork));

    network->layers    = first_layer;
    network->n_layers  = 1;
    network->release   = icvCNNetworkRelease;
    network->add_layer = icvCNNetworkAddLayer;

    __END__;

    if(cvGetErrStatus() < 0 && network)
        cFree(&network);

    return network;

}

/****************************************************************************************/
static void icvCNNetworkAddLayer(CvCNNetwork* network, CvCNNLayer* layer)
{
    CC_FUNCNAME("icvCNNetworkAddLayer");
    __BEGIN__;

    CvCNNLayer* prev_layer;

    if(network == NULL)
        CC_ERROR(CC_StsNullPtr, "Null <network> pointer");

    prev_layer = network->layers;
    while(prev_layer->next_layer)
        prev_layer = prev_layer->next_layer;

    if(ICC_IS_CNN_FULLCONNECT_LAYER(layer))
    {
        if(layer->n_input_planes != prev_layer->output_width*prev_layer->output_height*
            prev_layer->n_output_planes)
            CC_ERROR(CC_StsBadArg, "Unmatched size of the new layer");
        if(layer->input_height != 1 || layer->output_height != 1 ||
            layer->input_width != 1  || layer->output_width != 1)
            CC_ERROR(CC_StsBadArg, "Invalid size of the new layer");
    }
    else if(ICC_IS_CNN_CONVOLUTION_LAYER(layer) || ICC_IS_CNN_SUBSAMPLING_LAYER(layer))
    {
        if(prev_layer->n_output_planes != layer->n_input_planes ||
        prev_layer->output_height   != layer->input_height ||
        prev_layer->output_width    != layer->input_width)
        CC_ERROR(CC_StsBadArg, "Unmatched size of the new layer");
    }
    else
        CC_ERROR(CC_StsBadArg, "Invalid layer");

    layer->prev_layer = prev_layer;
    prev_layer->next_layer = layer;
    network->n_layers++;

    __END__;
}

/****************************************************************************************/
static void icvCNNetworkRelease(CvCNNetwork** network_pptr)
{
    CC_FUNCNAME("icvReleaseCNNetwork");
    __BEGIN__;

    CvCNNetwork* network = 0;
    CvCNNLayer* layer = 0, *next_layer = 0;
    int k;

    if(network_pptr == NULL)
        CC_ERROR(CC_StsBadArg, "Null double pointer");
    if(*network_pptr == NULL)
        return;

    network = *network_pptr;
    layer = network->layers;
    if(layer == NULL)
        CC_ERROR(CC_StsBadArg, "CNN is empty (does not contain any layer)");

    // k is the number of the layer to be deleted
    for(k = 0; k < network->n_layers && layer; k++)
    {
        next_layer = layer->next_layer;
        layer->release(&layer);
        layer = next_layer;
    }

    if(k != network->n_layers || layer)
        CC_ERROR(CC_StsBadArg, "Invalid network");

    cFree(&network);

    __END__;
}

/****************************************************************************************\
*                                  Layer functions                                       *
\****************************************************************************************/
static CvCNNLayer* icvCreateCNNLayer(int layer_type, int header_size,
    int n_input_planes, int input_height, int input_width,
    int n_output_planes, int output_height, int output_width,
    float init_learn_rate, int learn_rate_decrease_type,
    CvCNNLayerRelease release, CvCNNLayerForward forward, CvCNNLayerBackward backward)
{
    CvCNNLayer* layer = 0;

    CC_FUNCNAME("icvCreateCNNLayer");
    __BEGIN__;

    CC_ASSERT(release && forward && backward)
    CC_ASSERT(header_size >= sizeof(CvCNNLayer))

    if(n_input_planes < 1 || n_output_planes < 1 ||
        input_height   < 1 || input_width < 1 ||
        output_height  < 1 || output_width < 1 ||
        input_height < output_height ||
        input_width  < output_width)
        CC_ERROR(CC_StsBadArg, "Incorrect input or output parameters");
    if(init_learn_rate < FLT_EPSILON)
        CC_ERROR(CC_StsBadArg, "Initial learning rate must be positive");
    if(learn_rate_decrease_type != CC_CNN_LEARN_RATE_DECREASE_HYPERBOLICALLY &&
        learn_rate_decrease_type != CC_CNN_LEARN_RATE_DECREASE_SQRT_INV &&
        learn_rate_decrease_type != CC_CNN_LEARN_RATE_DECREASE_LOG_INV)
        CC_ERROR(CC_StsBadArg, "Invalid type of learning rate dynamics");

    CC_CALL(layer = (CvCNNLayer*)cAlloc(header_size));
    memset(layer, 0, header_size);

    layer->flags = ICC_CNN_LAYER|layer_type;
    CC_ASSERT(ICC_IS_CNN_LAYER(layer))

    layer->n_input_planes = n_input_planes;
    layer->input_height   = input_height;
    layer->input_width    = input_width;

    layer->n_output_planes = n_output_planes;
    layer->output_height   = output_height;
    layer->output_width    = output_width;

    layer->init_learn_rate = init_learn_rate;
    layer->learn_rate_decrease_type = learn_rate_decrease_type;

    layer->release  = release;
    layer->forward  = forward;
    layer->backward = backward;

    __END__;

    if(cvGetErrStatus() < 0 && layer)
        cFree(&layer);

    return layer;
}

/****************************************************************************************/
ML_IMPL CvCNNLayer* cvCreateCNNConvolutionLayer(
    int n_input_planes, int input_height, int input_width,
    int n_output_planes, int K,
    float init_learn_rate, int learn_rate_decrease_type,
    img_t* connect_mask, img_t* weights)

{
    CvCNNConvolutionLayer* layer = 0;

    CC_FUNCNAME("cvCreateCNNConvolutionLayer");
    __BEGIN__;

    const int output_height = input_height - K + 1;
    const int output_width = input_width - K + 1;

    if(K < 1 || init_learn_rate <= 0)
        CC_ERROR(CC_StsBadArg, "Incorrect parameters");

    CC_CALL(layer = (CvCNNConvolutionLayer*)icvCreateCNNLayer(ICC_CNN_CONVOLUTION_LAYER,
        sizeof(CvCNNConvolutionLayer), n_input_planes, input_height, input_width,
        n_output_planes, output_height, output_width,
        init_learn_rate, learn_rate_decrease_type,
        icvCNNConvolutionRelease, icvCNNConvolutionForward, icvCNNConvolutionBackward));

    layer->K = K;
    CC_CALL(layer->weights = cvCreateMat(n_output_planes, K*K+1, CC_32FC1));
    CC_CALL(layer->connect_mask = cvCreateMat(n_output_planes, n_input_planes, CC_8UC1));

    if(weights)
    {
        if(!ICC_IS_MAT_OF_TYPE(weights, CC_32FC1))
            CC_ERROR(CC_StsBadSize, "Type of initial weights matrix must be CC_32FC1");
        if(!CC_ARE_SIZES_EQ(weights, layer->weights))
            CC_ERROR(CC_StsBadSize, "Invalid size of initial weights matrix");
        CC_CALL(cvCopy(weights, layer->weights));
    }
    else
    {
        CRNG rng = cRNG(0xFFFFFFFF);
        cvRandArr(&rng, layer->weights, CC_RAND_UNI, cRealScalar(-1), cRealScalar(1));
    }
 
    if(connect_mask)
    {
        if(!ICC_IS_MAT_OF_TYPE(connect_mask, CC_8UC1))
            CC_ERROR(CC_StsBadSize, "Type of connection matrix must be CC_32FC1");
        if(!CC_ARE_SIZES_EQ(connect_mask, layer->connect_mask))
            CC_ERROR(CC_StsBadSize, "Invalid size of connection matrix");
        CC_CALL(cvCopy(connect_mask, layer->connect_mask));
    }
    else
        CC_CALL(cvSet(layer->connect_mask, cRealScalar(1)));

    __END__;

    if(cvGetErrStatus() < 0 && layer)
    {
        cvReleaseMat(&layer->weights);
        cvReleaseMat(&layer->connect_mask);
        cFree(&layer);
    }
    
    return (CvCNNLayer*)layer;
}

/****************************************************************************************/
ML_IMPL CvCNNLayer* cvCreateCNNSubSamplingLayer(
    int n_input_planes, int input_height, int input_width,
    int sub_samp_scale, float a, float s,
    float init_learn_rate, int learn_rate_decrease_type, img_t* weights)

{
    CvCNNSubSamplingLayer* layer = 0;

    CC_FUNCNAME("cvCreateCNNSubSamplingLayer");
    __BEGIN__;

    const int output_height   = input_height/sub_samp_scale;
    const int output_width    = input_width/sub_samp_scale;
    const int n_output_planes = n_input_planes;

    if(sub_samp_scale < 1 || a <= 0 || s <= 0)
        CC_ERROR(CC_StsBadArg, "Incorrect parameters");

    CC_CALL(layer = (CvCNNSubSamplingLayer*)icvCreateCNNLayer(ICC_CNN_SUBSAMPLING_LAYER,
        sizeof(CvCNNSubSamplingLayer), n_input_planes, input_height, input_width,
        n_output_planes, output_height, output_width,
        init_learn_rate, learn_rate_decrease_type,
        icvCNNSubSamplingRelease, icvCNNSubSamplingForward, icvCNNSubSamplingBackward));

    layer->sub_samp_scale  = sub_samp_scale;
    layer->a               = a;
    layer->s               = s;

    CC_CALL(layer->sumX =
        cvCreateMat(n_output_planes*output_width*output_height, 1, CC_32FC1));
    CC_CALL(layer->exp2ssumWX =
        cvCreateMat(n_output_planes*output_width*output_height, 1, CC_32FC1));

    cvZero(layer->sumX);
    cvZero(layer->exp2ssumWX);

    CC_CALL(layer->weights = cvCreateMat(n_output_planes, 2, CC_32FC1));
    if(weights)
    {
        if(!ICC_IS_MAT_OF_TYPE(weights, CC_32FC1))
            CC_ERROR(CC_StsBadSize, "Type of initial weights matrix must be CC_32FC1");
        if(!CC_ARE_SIZES_EQ(weights, layer->weights))
            CC_ERROR(CC_StsBadSize, "Invalid size of initial weights matrix");
        CC_CALL(cvCopy(weights, layer->weights));
    }
    else
    {
        CRNG rng = cRNG(0xFFFFFFFF);
        cvRandArr(&rng, layer->weights, CC_RAND_UNI, cRealScalar(-1), cRealScalar(1));
    }

    __END__;

    if(cvGetErrStatus() < 0 && layer)
    {
        cvReleaseMat(&layer->exp2ssumWX);
        cFree(&layer);
    }

    return (CvCNNLayer*)layer;
}

/****************************************************************************************/
ML_IMPL CvCNNLayer* cvCreateCNNFullConnectLayer(
    int n_inputs, int n_outputs, float a, float s,
    float init_learn_rate, int learn_rate_decrease_type, img_t* weights)
{
    CvCNNFullConnectLayer* layer = 0;

    CC_FUNCNAME("cvCreateCNNFullConnectLayer");
    __BEGIN__;

    if(a <= 0 || s <= 0 || init_learn_rate <= 0)
        CC_ERROR(CC_StsBadArg, "Incorrect parameters");

    CC_CALL(layer = (CvCNNFullConnectLayer*)icvCreateCNNLayer(ICC_CNN_FULLCONNECT_LAYER,
        sizeof(CvCNNFullConnectLayer), n_inputs, 1, 1, n_outputs, 1, 1,
        init_learn_rate, learn_rate_decrease_type,
        icvCNNFullConnectRelease, icvCNNFullConnectForward, icvCNNFullConnectBackward));

    layer->a = a;
    layer->s = s;

    CC_CALL(layer->exp2ssumWX = cvCreateMat(n_outputs, 1, CC_32FC1));
    cvZero(layer->exp2ssumWX);

    CC_CALL(layer->weights = cvCreateMat(n_outputs, n_inputs+1, CC_32FC1));
    if(weights)
    {
        if(!ICC_IS_MAT_OF_TYPE(weights, CC_32FC1))
            CC_ERROR(CC_StsBadSize, "Type of initial weights matrix must be CC_32FC1");
        if(!CC_ARE_SIZES_EQ(weights, layer->weights))
            CC_ERROR(CC_StsBadSize, "Invalid size of initial weights matrix");
        CC_CALL(cvCopy(weights, layer->weights));
    }
    else
    {
        CRNG rng = cRNG(0xFFFFFFFF);
        cvRandArr(&rng, layer->weights, CC_RAND_UNI, cRealScalar(-1), cRealScalar(1));
    }

    __END__;

    if(cvGetErrStatus() < 0 && layer)
    {
        cvReleaseMat(&layer->exp2ssumWX);
        cvReleaseMat(&layer->weights);
        cFree(&layer);
    }

    return (CvCNNLayer*)layer;
}


/****************************************************************************************\
*                           Layer FORWARD functions                                      *
\****************************************************************************************/
static void icvCNNConvolutionForward(CvCNNLayer* _layer,
                                      const img_t* X,
                                      img_t* Y)
{
    CC_FUNCNAME("icvCNNConvolutionForward");

    if(!ICC_IS_CNN_CONVOLUTION_LAYER(_layer))
        CC_ERROR(CC_StsBadArg, "Invalid layer");

    {__BEGIN__;

    const CvCNNConvolutionLayer* layer = (CvCNNConvolutionLayer*) _layer;

    const int K = layer->K;
    const int n_weights_for_Yplane = K*K + 1;

    const int nXplanes = layer->n_input_planes;
    const int Xheight  = layer->input_height;
    const int Xwidth   = layer->input_width ;
    const int Xsize    = Xwidth*Xheight;

    const int nYplanes = layer->n_output_planes;
    const int Yheight  = layer->output_height;
    const int Ywidth   = layer->output_width;
    const int Ysize    = Ywidth*Yheight;

    int xx, yy, ni, no, kx, ky;
    float *Yplane = 0, *Xplane = 0, *w = 0;
    uchar* connect_mask_data = 0;

    CC_ASSERT(X->rows == nXplanes*Xsize && X->cols == 1);
    CC_ASSERT(Y->rows == nYplanes*Ysize && Y->cols == 1);

    cvSetZero(Y);

    Yplane = Y->tt.fl;
    connect_mask_data = layer->connect_mask->tt.data;
    w = layer->weights->tt.fl;
    for(no = 0; no < nYplanes; no++, Yplane += Ysize, w += n_weights_for_Yplane)
    {
        Xplane = X->tt.fl;
        for(ni = 0; ni < nXplanes; ni++, Xplane += Xsize, connect_mask_data++)
        {
            if(*connect_mask_data)
            {
                float* Yelem = Yplane;

                // Xheight-K+1 == Yheight && Xwidth-K+1 == Ywidth
                for(yy = 0; yy < Xheight-K+1; yy++)
                {
                    for(xx = 0; xx < Xwidth-K+1; xx++, Yelem++)
                    {
                        float* templ = Xplane+yy*Xwidth+xx;
                        float WX = 0;
                        for(ky = 0; ky < K; ky++, templ += Xwidth-K)
                        {
                            for(kx = 0; kx < K; kx++, templ++)
                            {
                                WX += *templ*w[ky*K+kx];
                            }
                        }
                        *Yelem += WX + w[K*K];
                    }
                }
            }
        }
    }
    }__END__;
}

/****************************************************************************************/
static void icvCNNSubSamplingForward(CvCNNLayer* _layer,
                                      const img_t* X,
                                      img_t* Y)
{
    CC_FUNCNAME("icvCNNSubSamplingForward");

    if(!ICC_IS_CNN_SUBSAMPLING_LAYER(_layer))
        CC_ERROR(CC_StsBadArg, "Invalid layer");

    {__BEGIN__;

    const CvCNNSubSamplingLayer* layer = (CvCNNSubSamplingLayer*) _layer;

    const int sub_sampl_scale = layer->sub_samp_scale;
    const int nplanes = layer->n_input_planes;

    const int Xheight = layer->input_height;
    const int Xwidth  = layer->input_width ;
    const int Xsize   = Xwidth*Xheight;

    const int Yheight = layer->output_height;
    const int Ywidth  = layer->output_width;
    const int Ysize   = Ywidth*Yheight;

    int xx, yy, ni, kx, ky;
    float* sumX_data = 0, *w = 0;
    img_t sumX_sub_col, exp2ssumWX_sub_col;

    CC_ASSERT(X->rows == nplanes*Xsize && X->cols == 1);
    CC_ASSERT(layer->exp2ssumWX->cols == 1 && layer->exp2ssumWX->rows == nplanes*Ysize);

    // update inner variable layer->exp2ssumWX, which will be used in back-progation
    cvZero(layer->sumX);
    cvZero(layer->exp2ssumWX);

    for(ky = 0; ky < sub_sampl_scale; ky++)
        for(kx = 0; kx < sub_sampl_scale; kx++)
        {
            float* Xplane = X->tt.fl;
            sumX_data = layer->sumX->tt.fl;
            for(ni = 0; ni < nplanes; ni++, Xplane += Xsize)
            {
                for(yy = 0; yy < Yheight; yy++)
                    for(xx = 0; xx < Ywidth; xx++, sumX_data++)
                        *sumX_data += Xplane[((yy+ky)*Xwidth+(xx+kx))];
            }
        }                

    w = layer->weights->tt.fl;
    cvGetRows(layer->sumX, &sumX_sub_col, 0, Ysize);
    cvGetRows(layer->exp2ssumWX, &exp2ssumWX_sub_col, 0, Ysize);
    for(ni = 0; ni < nplanes; ni++, w += 2)
    {
        CC_CALL(cvConvertScale(&sumX_sub_col, &exp2ssumWX_sub_col, w[0], w[1]));
        sumX_sub_col->tt.fl += Ysize;
        exp2ssumWX_sub_col->tt.fl += Ysize;
    }

    CC_CALL(cvScale(layer->exp2ssumWX, layer->exp2ssumWX, 2.0*layer->s));
    CC_CALL(cvExp(layer->exp2ssumWX, layer->exp2ssumWX));
    CC_CALL(cvMinS(layer->exp2ssumWX, FLT_MAX, layer->exp2ssumWX));
//#ifdef _DEBUG
    {
        float* exp2ssumWX_data = layer->exp2ssumWX->tt.fl;
        for(ni = 0; ni < layer->exp2ssumWX->rows; ni++, exp2ssumWX_data++)
        {
            if(*exp2ssumWX_data == FLT_MAX)
                cvSetErrStatus(1);
        }
    }
//#endif
    // compute the output variable Y == (a - 2a/(layer->exp2ssumWX + 1))
    CC_CALL(cvAddS(layer->exp2ssumWX, cRealScalar(1), Y));
    CC_CALL(cvDiv(0, Y, Y, -2.0*layer->a));
    CC_CALL(cvAddS(Y, cRealScalar(layer->a), Y));

    }__END__;
}

/****************************************************************************************/
static void icvCNNFullConnectForward(CvCNNLayer* _layer, const img_t* X, img_t* Y)
{
    CC_FUNCNAME("icvCNNFullConnectForward");

    if(!ICC_IS_CNN_FULLCONNECT_LAYER(_layer))
        CC_ERROR(CC_StsBadArg, "Invalid layer");

    {__BEGIN__;

    const CvCNNFullConnectLayer* layer = (CvCNNFullConnectLayer*)_layer;
    img_t* weights = layer->weights;
    img_t sub_weights, bias;

    CC_ASSERT(X->cols == 1 && X->rows == layer->n_input_planes);
    CC_ASSERT(Y->cols == 1 && Y->rows == layer->n_output_planes);

    CC_CALL(cvGetSubRect(weights, &sub_weights,
                          cRect(0, 0, weights->cols-1, weights->rows)));
    CC_CALL(cvGetCol(weights, &bias, weights->cols-1));

    // update inner variable layer->exp2ssumWX, which will be used in Back-Propagation
    CC_CALL(cvGEMM(&sub_weights, X, 2*layer->s, &bias, 2*layer->s, layer->exp2ssumWX));
    CC_CALL(cvExp(layer->exp2ssumWX, layer->exp2ssumWX));
    CC_CALL(cvMinS(layer->exp2ssumWX, FLT_MAX, layer->exp2ssumWX));
//#ifdef _DEBUG
    {
        float* exp2ssumWX_data = layer->exp2ssumWX->tt.fl;
        int i;
        for(i = 0; i < layer->exp2ssumWX->rows; i++, exp2ssumWX_data++)
        {
            if(*exp2ssumWX_data == FLT_MAX)
                cvSetErrStatus(1);
        }
    }
//#endif
    // compute the output variable Y == (a - 2a/(layer->exp2ssumWX + 1))
    CC_CALL(cvAddS(layer->exp2ssumWX, cRealScalar(1), Y));
    CC_CALL(cvDiv(0, Y, Y, -2.0*layer->a));
    CC_CALL(cvAddS(Y, cRealScalar(layer->a), Y));

    }__END__;
}

/****************************************************************************************\
*                           Layer BACKWARD functions                                     *
\****************************************************************************************/

/* <dE_dY>, <dE_dX> should be row-vectors.
   Function computes partial derivatives <dE_dX>
   of the loss function with respect to the planes components
   of the previous layer (X).
   It is a basic function for back propagation method.
   Input parameter <dE_dY> is the partial derivative of the
   loss function with respect to the planes components
   of the current layer. */
static void icvCNNConvolutionBackward(
    CvCNNLayer* _layer, int t, const img_t* X, const img_t* dE_dY, img_t* dE_dX)
{
    img_t* dY_dX = 0;
    img_t* dY_dW = 0;
    img_t* dE_dW = 0;

    CC_FUNCNAME("icvCNNConvolutionBackward");

    if(!ICC_IS_CNN_CONVOLUTION_LAYER(_layer))
        CC_ERROR(CC_StsBadArg, "Invalid layer");

    {__BEGIN__;

    const CvCNNConvolutionLayer* layer = (CvCNNConvolutionLayer*) _layer;

    const int K = layer->K;

    const int n_X_planes     = layer->n_input_planes;
    const int X_plane_height = layer->input_height;
    const int X_plane_width  = layer->input_width;
    const int X_plane_size   = X_plane_height*X_plane_width;

    const int n_Y_planes     = layer->n_output_planes;
    const int Y_plane_height = layer->output_height;
    const int Y_plane_width  = layer->output_width;
    const int Y_plane_size   = Y_plane_height*Y_plane_width;

    int no, ni, yy, xx, ky, kx;
    int X_idx = 0, Y_idx = 0;

    float *X_plane = 0, *w = 0;

    img_t* weights = layer->weights;

    CC_ASSERT(t >= 1);
    CC_ASSERT(n_Y_planes == weights->rows);

    dY_dX = cvCreateMat(n_Y_planes*Y_plane_size, X->rows, CC_32FC1);
    dY_dW = cvCreateMat(dY_dX->rows, weights->cols*weights->rows, CC_32FC1);
    dE_dW = cvCreateMat(1, dY_dW->cols, CC_32FC1);

    cvZero(dY_dX);
    cvZero(dY_dW);

    // compute gradient of the loss function with respect to X and W
    for(no = 0; no < n_Y_planes; no++, Y_idx += Y_plane_size)
    {
        w = weights->tt.fl + no*(K*K+1);
        X_idx = 0;
        X_plane = X->tt.fl;
        for(ni = 0; ni < n_X_planes; ni++, X_plane += X_plane_size)
        {
            if(layer->connect_mask->tt.data[ni*n_Y_planes+no])
            {
                for(yy = 0; yy < X_plane_height - K + 1; yy++)
                {
                    for(xx = 0; xx < X_plane_width - K + 1; xx++)
                    {
                        for(ky = 0; ky < K; ky++)
                        {
                            for(kx = 0; kx < K; kx++)
                            {
                                CC_MAT_ELEM(*dY_dX, float, Y_idx+yy*Y_plane_width+xx,
                                    X_idx+(yy+ky)*X_plane_width+(xx+kx)) = w[ky*K+kx];

                                // dY_dWi, i=1,...,K*K
                                CC_MAT_ELEM(*dY_dW, float, Y_idx+yy*Y_plane_width+xx,
                                    no*(K*K+1)+ky*K+kx) +=
                                    X_plane[(yy+ky)*X_plane_width+(xx+kx)];
                            }
                        }
                        // dY_dW(K*K+1)==1 because W(K*K+1) is bias
                        CC_MAT_ELEM(*dY_dW, float, Y_idx+yy*Y_plane_width+xx,
                            no*(K*K+1)+K*K) += 1;
                    }
                }
            }
            X_idx += X_plane_size;
        }
    }

    CC_CALL(cvMatMul(dE_dY, dY_dW, dE_dW));
    CC_CALL(cvMatMul(dE_dY, dY_dX, dE_dX));

    // update weights
    {
        img_t dE_dW_mat;
        float eta;
        if(layer->learn_rate_decrease_type == CC_CNN_LEARN_RATE_DECREASE_LOG_INV)
            eta = -layer->init_learn_rate/logf(1+(float)t);
        else if(layer->learn_rate_decrease_type == CC_CNN_LEARN_RATE_DECREASE_SQRT_INV)
            eta = -layer->init_learn_rate/sqrtf((float)t);
        else
            eta = -layer->init_learn_rate/(float)t;
        cvReshape(dE_dW, &dE_dW_mat, 0, weights->rows);
        cvScaleAdd(&dE_dW_mat, cRealScalar(eta), weights, weights);
    }

    }__END__;

    cvReleaseMat(&dY_dX);
    cvReleaseMat(&dY_dW);
    cvReleaseMat(&dE_dW);
}

/****************************************************************************************/
static void icvCNNSubSamplingBackward(
    CvCNNLayer* _layer, int t, const img_t*, const img_t* dE_dY, img_t* dE_dX)
{
    // derivative of activation function
    img_t* dY_dX_elems = 0; // elements of matrix dY_dX
    img_t* dY_dW_elems = 0; // elements of matrix dY_dW
    img_t* dE_dW = 0;

    CC_FUNCNAME("icvCNNSubSamplingBackward");

    if(!ICC_IS_CNN_SUBSAMPLING_LAYER(_layer))
        CC_ERROR(CC_StsBadArg, "Invalid layer");

    {__BEGIN__;

    const CvCNNSubSamplingLayer* layer = (CvCNNSubSamplingLayer*) _layer;

    const int Xwidth  = layer->input_width;
    const int Ywidth  = layer->output_width;
    const int Yheight = layer->output_height;
    const int Ysize   = Ywidth * Yheight;
    const int scale   = layer->sub_samp_scale;
    const int k_max   = layer->n_output_planes * Yheight;

    int k, i, j, m;
    float* dY_dX_current_elem = 0, *dE_dX_start = 0, *dE_dW_data = 0, *w = 0;
    img_t dy_dw0, dy_dw1;
    img_t activ_func_der, sumX_row;
    img_t dE_dY_sub_row, dY_dX_sub_col, dy_dw0_sub_row, dy_dw1_sub_row;

    CC_CALL(dY_dX_elems = cvCreateMat(layer->sumX->rows, 1, CC_32FC1));
    CC_CALL(dY_dW_elems = cvCreateMat(2, layer->sumX->rows, CC_32FC1));
    CC_CALL(dE_dW = cvCreateMat(1, 2*layer->n_output_planes, CC_32FC1));

    // compute derivative of activ.func.
    // ==<dY_dX_elems> = 4as*(layer->exp2ssumWX)/(layer->exp2ssumWX + 1)^2
    CC_CALL(cvAddS(layer->exp2ssumWX, cRealScalar(1), dY_dX_elems));
    CC_CALL(cvPow(dY_dX_elems, dY_dX_elems, -2.0));
    CC_CALL(cvMul(dY_dX_elems, layer->exp2ssumWX, dY_dX_elems, 4.0*layer->a*layer->s));

    // compute <dE_dW>
    // a) compute <dY_dW_elems>
    cvReshape(dY_dX_elems, &activ_func_der, 0, 1);
    cvGetRow(dY_dW_elems, &dy_dw0, 0);
    cvGetRow(dY_dW_elems, &dy_dw1, 1);
    CC_CALL(cvCopy(&activ_func_der, &dy_dw0));
    CC_CALL(cvCopy(&activ_func_der, &dy_dw1));

    cvReshape(layer->sumX, &sumX_row, 0, 1);
    cvMul(&dy_dw0, &sumX_row, &dy_dw0);

    // b) compute <dE_dW> = <dE_dY>*<dY_dW_elems>
    cvGetCols(dE_dY, &dE_dY_sub_row, 0, Ysize);
    cvGetCols(&dy_dw0, &dy_dw0_sub_row, 0, Ysize);
    cvGetCols(&dy_dw1, &dy_dw1_sub_row, 0, Ysize);
    dE_dW_data = dE_dW->tt.fl;
    for(i = 0; i < layer->n_output_planes; i++)
    {
        *dE_dW_data++ = (float)cvDotProduct(&dE_dY_sub_row, &dy_dw0_sub_row);
        *dE_dW_data++ = (float)cvDotProduct(&dE_dY_sub_row, &dy_dw1_sub_row);

        dE_dY_sub_row->tt.fl += Ysize;
        dy_dw0_sub_row->tt.fl += Ysize;
        dy_dw1_sub_row->tt.fl += Ysize;
    }

    // compute <dY_dX> = layer->weights*<dY_dX>
    w = layer->weights->tt.fl;
    cvGetRows(dY_dX_elems, &dY_dX_sub_col, 0, Ysize);
    for(i = 0; i < layer->n_input_planes; i++, w++, dY_dX_sub_col->tt.fl += Ysize)
        CC_CALL(cvConvertScale(&dY_dX_sub_col, &dY_dX_sub_col, (float)*w));

    // compute <dE_dX>
    CC_CALL(cvReshape(dY_dX_elems, dY_dX_elems, 0, 1));
    CC_CALL(cvMul(dY_dX_elems, dE_dY, dY_dX_elems));

    dY_dX_current_elem = dY_dX_elems->tt.fl;
    dE_dX_start = dE_dX->tt.fl;
    for(k = 0; k < k_max; k++)
    {
        for(i = 0; i < Ywidth; i++, dY_dX_current_elem++)
        {
            float* dE_dX_current_elem = dE_dX_start;
            for(j = 0; j < scale; j++, dE_dX_current_elem += Xwidth - scale)
            {
                for(m = 0; m < scale; m++, dE_dX_current_elem++)
                    *dE_dX_current_elem = *dY_dX_current_elem;
            }
            dE_dX_start += scale;
        }
        dE_dX_start += Xwidth * (scale - 1);
    }

    // update weights
    {
        img_t dE_dW_mat, *weights = layer->weights;
        float eta;
        if(layer->learn_rate_decrease_type == CC_CNN_LEARN_RATE_DECREASE_LOG_INV)
            eta = -layer->init_learn_rate/logf(1+(float)t);
        else if(layer->learn_rate_decrease_type == CC_CNN_LEARN_RATE_DECREASE_SQRT_INV)
            eta = -layer->init_learn_rate/sqrtf((float)t);
        else
            eta = -layer->init_learn_rate/(float)t;
        cvReshape(dE_dW, &dE_dW_mat, 0, weights->rows);
        cvScaleAdd(&dE_dW_mat, cRealScalar(eta), weights, weights);
    }

    }__END__;

    cvReleaseMat(&dY_dX_elems);
    cvReleaseMat(&dY_dW_elems);
    cvReleaseMat(&dE_dW);
}

/****************************************************************************************/
/* <dE_dY>, <dE_dX> should be row-vectors.
   Function computes partial derivatives <dE_dX>, <dE_dW>
   of the loss function with respect to the planes components
   of the previous layer (X) and the weights of the current layer (W)
   and updates weights od the current layer by using <dE_dW>.
   It is a basic function for back propagation method.
   Input parameter <dE_dY> is the partial derivative of the
   loss function with respect to the planes components
   of the current layer. */
static void icvCNNFullConnectBackward(CvCNNLayer* _layer,
                                    int t,
                                    const img_t* X,
                                    const img_t* dE_dY,
                                    img_t* dE_dX)
{
    img_t* dE_dY_activ_func_der = 0;
    img_t* dE_dW = 0;
    
    CC_FUNCNAME("icvCNNFullConnectBackward");

    if(!ICC_IS_CNN_FULLCONNECT_LAYER(_layer))
        CC_ERROR(CC_StsBadArg, "Invalid layer");

    {__BEGIN__;

    const CvCNNFullConnectLayer* layer = (CvCNNFullConnectLayer*)_layer;
    const int n_outputs = layer->n_output_planes;
    const int n_inputs  = layer->n_input_planes;

    int i;
    float* dE_dY_activ_func_der_data;
    img_t* weights = layer->weights;
    img_t sub_weights, Xtemplate, Xrow, exp2ssumWXrow;

    CC_ASSERT(X->cols == 1 && X->rows == n_inputs);
    CC_ASSERT(dE_dY->rows == 1 && dE_dY->cols == n_outputs);
    CC_ASSERT(dE_dX->rows == 1 && dE_dX->cols == n_inputs);
    
    // we violate the convetion about vector's orientation because
    // here is more convenient to make this parameter a row-vector 
    CC_CALL(dE_dY_activ_func_der = cvCreateMat(1, n_outputs, CC_32FC1));
    CC_CALL(dE_dW = cvCreateMat(1, weights->rows*weights->cols, CC_32FC1));
    
    // 1) compute gradients dE_dX and dE_dW
    // activ_func_der == 4as*(layer->exp2ssumWX)/(layer->exp2ssumWX + 1)^2
    CC_CALL(cvReshape(layer->exp2ssumWX, &exp2ssumWXrow, 0, layer->exp2ssumWX->cols));
    CC_CALL(cvAddS(&exp2ssumWXrow, cRealScalar(1), dE_dY_activ_func_der));
    CC_CALL(cvPow(dE_dY_activ_func_der, dE_dY_activ_func_der, -2.0));
    CC_CALL(cvMul(dE_dY_activ_func_der, &exp2ssumWXrow, dE_dY_activ_func_der,
                   4.0*layer->a*layer->s));
    CC_CALL(cvMul(dE_dY, dE_dY_activ_func_der, dE_dY_activ_func_der));

    // sub_weights = d(W*(X|1))/dX
    CC_CALL(cvGetSubRect(weights, &sub_weights,
        cRect(0, 0, weights->cols-1, weights->rows)));
    CC_CALL(cvMatMul(dE_dY_activ_func_der, &sub_weights, dE_dX));

    cvReshape(X, &Xrow, 0, 1);
    dE_dY_activ_func_der_data = dE_dY_activ_func_der->tt.fl;
    Xtemplate = cvMat(1, n_inputs, CC_32FC1, dE_dW->tt.fl);
    for(i = 0; i < n_outputs; i++, Xtemplate->tt.fl += n_inputs + 1)
    {
        CC_CALL(cvConvertScale(&Xrow, &Xtemplate, *dE_dY_activ_func_der_data));
        Xtemplate->tt.fl[n_inputs] = *dE_dY_activ_func_der_data++;
    }

    // 2) update weights
    {
        img_t dE_dW_mat;
        float eta;
        if(layer->learn_rate_decrease_type == CC_CNN_LEARN_RATE_DECREASE_LOG_INV)
            eta = -layer->init_learn_rate/logf(1+(float)t);
        else if(layer->learn_rate_decrease_type == CC_CNN_LEARN_RATE_DECREASE_SQRT_INV)
            eta = -layer->init_learn_rate/sqrtf((float)t);
        else
            eta = -layer->init_learn_rate/(float)t;
        cvReshape(dE_dW, &dE_dW_mat, 0, n_outputs);
        cvScaleAdd(&dE_dW_mat, cRealScalar(eta), weights, weights);
    }

    }__END__;

    cvReleaseMat(&dE_dY_activ_func_der);
    cvReleaseMat(&dE_dW);
}

/****************************************************************************************\
*                           Layer RELEASE functions                                      *
\****************************************************************************************/
static void icvCNNConvolutionRelease(CvCNNLayer** p_layer)
{
    CC_FUNCNAME("icvCNNConvolutionRelease");
    __BEGIN__;

    CvCNNConvolutionLayer* layer = 0;

    if(!p_layer)
        CC_ERROR(CC_StsNullPtr, "Null double pointer");

    layer = *(CvCNNConvolutionLayer**)p_layer;

    if(!layer)
        return;
    if(!ICC_IS_CNN_CONVOLUTION_LAYER(layer))
        CC_ERROR(CC_StsBadArg, "Invalid layer");

    cvReleaseMat(&layer->weights);
    cvReleaseMat(&layer->connect_mask);
    cFree(p_layer);

    __END__;
}

/****************************************************************************************/
static void icvCNNSubSamplingRelease(CvCNNLayer** p_layer)
{
    CC_FUNCNAME("icvCNNSubSamplingRelease");
    __BEGIN__;

    CvCNNSubSamplingLayer* layer = 0;
    
    if(!p_layer)
        CC_ERROR(CC_StsNullPtr, "Null double pointer");

    layer = *(CvCNNSubSamplingLayer**)p_layer;

    if(!layer)
        return;
    if(!ICC_IS_CNN_SUBSAMPLING_LAYER(layer))
        CC_ERROR(CC_StsBadArg, "Invalid layer");

    cvReleaseMat(&layer->exp2ssumWX);
    cvReleaseMat(&layer->weights);
    cFree(p_layer);

    __END__;
}

/****************************************************************************************/
static void icvCNNFullConnectRelease(CvCNNLayer** p_layer)
{
    CC_FUNCNAME("icvCNNFullConnectRelease");
    __BEGIN__;

    CvCNNFullConnectLayer* layer = 0;
    
    if(!p_layer)
        CC_ERROR(CC_StsNullPtr, "Null double pointer");

    layer = *(CvCNNFullConnectLayer**)p_layer;

    if(!layer)
        return;
    if(!ICC_IS_CNN_FULLCONNECT_LAYER(layer))
        CC_ERROR(CC_StsBadArg, "Invalid layer");

    cvReleaseMat(&layer->exp2ssumWX);
    cvReleaseMat(&layer->weights);
    cFree(p_layer);

    __END__;
}

/****************************************************************************************\
*                              Read/Write CNN classifier                                 *
\****************************************************************************************/
static int icvIsCNNModel(const void* ptr)
{
    return CC_IS_CNN(ptr);
}

/****************************************************************************************/
static void icvReleaseCNNModel(void** ptr)
{
    CC_FUNCNAME("icvReleaseCNNModel");
    __BEGIN__;

    if(!ptr)
        CC_ERROR(CC_StsNullPtr, "NULL double pointer");
    CC_ASSERT(CC_IS_CNN(*ptr));

    icvCNNModelRelease((CvStatModel**)ptr);

    __END__;
}

/****************************************************************************************/
static CvCNNLayer* icvReadCNNLayer(CvFileStorage* fs, CvFileNode* node)
{
    CvCNNLayer* layer = 0;
    img_t* weights    = 0;
    img_t* connect_mask = 0;

    CC_FUNCNAME("icvReadCNNLayer");
    __BEGIN__;

    int n_input_planes, input_height, input_width;
    int n_output_planes, output_height, output_width;
    int learn_type, layer_type;
    float init_learn_rate;

    CC_CALL(n_input_planes  = cvReadIntByName(fs, node, "n_input_planes",  -1));
    CC_CALL(input_height    = cvReadIntByName(fs, node, "input_height",    -1));
    CC_CALL(input_width     = cvReadIntByName(fs, node, "input_width",     -1));
    CC_CALL(n_output_planes = cvReadIntByName(fs, node, "n_output_planes", -1));
    CC_CALL(output_height   = cvReadIntByName(fs, node, "output_height",   -1));
    CC_CALL(output_width    = cvReadIntByName(fs, node, "output_width",    -1));
    CC_CALL(layer_type      = cvReadIntByName(fs, node, "layer_type",      -1));

    CC_CALL(init_learn_rate = (float)cvReadRealByName(fs, node, "init_learn_rate", -1));
    CC_CALL(learn_type = cvReadIntByName(fs, node, "learn_rate_decrease_type", -1));
    CC_CALL(weights    = cvReadByName(fs, node, "weights"));

    if(n_input_planes < 0  || input_height < 0  || input_width < 0 ||
        n_output_planes < 0 || output_height < 0 || output_width < 0 ||
        init_learn_rate < 0 || learn_type < 0 || layer_type < 0 || !weights)
        CC_ERROR(CC_StsParseError, "");

    if(layer_type == ICC_CNN_CONVOLUTION_LAYER)
    {
        const int K = input_height - output_height + 1;
        if(K <= 0 || K != input_width - output_width + 1)
            CC_ERROR(CC_StsBadArg, "Invalid <K>");

        CC_CALL(connect_mask = cvReadByName(fs, node, "connect_mask"));
        if(!connect_mask)
            CC_ERROR(CC_StsParseError, "Missing <connect mask>");

        CC_CALL(layer = cvCreateCNNConvolutionLayer(
            n_input_planes, input_height, input_width, n_output_planes, K,
            init_learn_rate, learn_type, connect_mask, weights));
    }
    else if(layer_type == ICC_CNN_SUBSAMPLING_LAYER)
    {
        float a, s;
        const int sub_samp_scale = input_height/output_height;

        if(sub_samp_scale <= 0 || sub_samp_scale != input_width/output_width)
            CC_ERROR(CC_StsBadArg, "Invalid <sub_samp_scale>");

        CC_CALL(a = (float)cvReadRealByName(fs, node, "a", -1));
        CC_CALL(s = (float)cvReadRealByName(fs, node, "s", -1));
        if(a  < 0 || s  < 0)
            CC_ERROR(CC_StsParseError, "Missing <a> or <s>");

        CC_CALL(layer = cvCreateCNNSubSamplingLayer(
            n_input_planes, input_height, input_width, sub_samp_scale,
            a, s, init_learn_rate, learn_type, weights));
    }
    else if(layer_type == ICC_CNN_FULLCONNECT_LAYER)
    {
        float a, s;
        CC_CALL(a = (float)cvReadRealByName(fs, node, "a", -1));
        CC_CALL(s = (float)cvReadRealByName(fs, node, "s", -1));
        if(a  < 0 || s  < 0)
            CC_ERROR(CC_StsParseError, "");
        if(input_height != 1  || input_width != 1 ||
            output_height != 1 || output_width != 1)
            CC_ERROR(CC_StsBadArg, "");

        CC_CALL(layer = cvCreateCNNFullConnectLayer(n_input_planes, n_output_planes,
            a, s, init_learn_rate, learn_type, weights));
    }
    else
        CC_ERROR(CC_StsBadArg, "Invalid <layer_type>");

    __END__;

    if(cvGetErrStatus() < 0 && layer)
        layer->release(&layer);

    cvReleaseMat(&weights);
    cvReleaseMat(&connect_mask);

    return layer;
}

/****************************************************************************************/
static void icvWriteCNNLayer(CvFileStorage* fs, CvCNNLayer* layer)
{
    CC_FUNCNAME ("icvWriteCNNLayer");
    __BEGIN__;

    if(!ICC_IS_CNN_LAYER(layer))
        CC_ERROR(CC_StsBadArg, "Invalid layer");

    CC_CALL(cvWriteStructBegin(fs, NULL, CC_NODE_MAP, "opencv-ml-cnn-layer"));

    CC_CALL(cvWriteInt(fs, "n_input_planes",  layer->n_input_planes));
    CC_CALL(cvWriteInt(fs, "input_height",    layer->input_height));
    CC_CALL(cvWriteInt(fs, "input_width",     layer->input_width));
    CC_CALL(cvWriteInt(fs, "n_output_planes", layer->n_output_planes));
    CC_CALL(cvWriteInt(fs, "output_height",   layer->output_height));
    CC_CALL(cvWriteInt(fs, "output_width",    layer->output_width));
    CC_CALL(cvWriteInt(fs, "learn_rate_decrease_type", layer->learn_rate_decrease_type));
    CC_CALL(cvWriteReal(fs, "init_learn_rate", layer->init_learn_rate));
    CC_CALL(cvWrite(fs, "weights", layer->weights));

    if(ICC_IS_CNN_CONVOLUTION_LAYER(layer))
    {
        CvCNNConvolutionLayer* l = (CvCNNConvolutionLayer*)layer;
        CC_CALL(cvWriteInt(fs, "layer_type", ICC_CNN_CONVOLUTION_LAYER));
        CC_CALL(cvWrite(fs, "connect_mask", l->connect_mask));
    }
    else if(ICC_IS_CNN_SUBSAMPLING_LAYER(layer))
    {
        CvCNNSubSamplingLayer* l = (CvCNNSubSamplingLayer*)layer;
        CC_CALL(cvWriteInt(fs, "layer_type", ICC_CNN_SUBSAMPLING_LAYER));
        CC_CALL(cvWriteReal(fs, "a", l->a));
        CC_CALL(cvWriteReal(fs, "s", l->s));
    }
    else if(ICC_IS_CNN_FULLCONNECT_LAYER(layer))
    {
        CvCNNFullConnectLayer* l = (CvCNNFullConnectLayer*)layer;
        CC_CALL(cvWriteInt(fs, "layer_type", ICC_CNN_FULLCONNECT_LAYER));
        CC_CALL(cvWriteReal(fs, "a", l->a));
        CC_CALL(cvWriteReal(fs, "s", l->s));
    }
    else
        CC_ERROR(CC_StsBadArg, "Invalid layer");

    CC_CALL(cvWriteStructEnd(fs)); //"opencv-ml-cnn-layer"

    __END__;
}

/****************************************************************************************/
static void* icvReadCNNModel(CvFileStorage* fs, CvFileNode* root_node)
{
    CvCNNStatModel* cnn = 0;
    CvCNNLayer* layer = 0;

    CC_FUNCNAME("icvReadCNNModel");
    __BEGIN__;

    CvFileNode* node;
    CvSeq* seq;
    CvSeqReader reader;
    int i;

    CC_CALL(cnn = (CvCNNStatModel*)cvCreateStatModel(
        CC_STAT_MODEL_MAGIC_VAL|CC_CNN_MAGIC_VAL, sizeof(CvCNNStatModel),
        icvCNNModelRelease, icvCNNModelPredict, icvCNNModelUpdate));

    CC_CALL(cnn->etalons = cvReadByName(fs, root_node, "etalons"));
    CC_CALL(cnn->cls_labels = cvReadByName(fs, root_node, "cls_labels"));

    if(!cnn->etalons || !cnn->cls_labels)
        CC_ERROR(CC_StsParseError, "No <etalons> or <cls_labels> in CNN model");

    CC_CALL(node = cvGetFileNodeByName(fs, root_node, "network"));
    seq = node->tt.seq;
    if(!CC_NODE_IS_SEQ(node->tag))
        CC_ERROR(CC_StsBadArg, "");

    CC_CALL(cvStartReadSeq(seq, &reader, 0));
    CC_CALL(layer = icvReadCNNLayer(fs, (CvFileNode*)reader.ptr));
    CC_CALL(cnn->network = cvCreateCNNetwork(layer));

    for(i = 1; i < seq->total; i++)
    {
        CC_NEXT_SEQ_ELEM(seq->elem_size, reader);
        CC_CALL(layer = icvReadCNNLayer(fs, (CvFileNode*)reader.ptr));
        CC_CALL(cnn->network->add_layer(cnn->network, layer));
    }

    __END__;

    if(cvGetErrStatus() < 0)
    {
        if(cnn) cnn->release((CvStatModel**)&cnn);
        if(layer) layer->release(&layer);
    }
    return (void*)cnn;
}

/****************************************************************************************/
static void
icvWriteCNNModel(CvFileStorage* fs, const char* name,
                  const void* struct_ptr, CvAttrList)
                                   
{
    CC_FUNCNAME ("icvWriteCNNModel");
    __BEGIN__;

    CvCNNStatModel* cnn = (CvCNNStatModel*)struct_ptr;
    int n_layers, i;
    CvCNNLayer* layer;

    if(!CC_IS_CNN(cnn))
        CC_ERROR(CC_StsBadArg, "Invalid pointer");

    n_layers = cnn->network->n_layers;
    
    CC_CALL(cvWriteStructBegin(fs, name, CC_NODE_MAP, CC_TYPE_NAME_ML_CNN));

    CC_CALL(cvWrite(fs, "etalons", cnn->etalons));
    CC_CALL(cvWrite(fs, "cls_labels", cnn->cls_labels));
    
    CC_CALL(cvWriteStructBegin(fs, "network", CC_NODE_SEQ));

    layer = cnn->network->layers;
    for(i = 0; i < n_layers && layer; i++, layer = layer->next_layer)
        CC_CALL(icvWriteCNNLayer(fs, layer));
    if(i < n_layers || layer)
        CC_ERROR(CC_StsBadArg, "Invalid network");

    CC_CALL(cvWriteStructEnd(fs)); //"network"
    CC_CALL(cvWriteStructEnd(fs)); //"opencv-ml-cnn"

    __END__;
}

static int icvRegisterCNNStatModelType()
{
    CvTypeInfo info;

    info.header_size = sizeof(info);
    info.is_instance = icvIsCNNModel;
    info.release = icvReleaseCNNModel;
    info.read = icvReadCNNModel;
    info.write = icvWriteCNNModel;
    info.clone = NULL;
    info.type_name = CC_TYPE_NAME_ML_CNN;
    cvRegisterType(&info);

    return 1;
} // End of icvRegisterCNNStatModelType

static int cnn = icvRegisterCNNStatModelType();

#endif

// End of file
