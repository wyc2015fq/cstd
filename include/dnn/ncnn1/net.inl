
#ifndef _NET_INL_
#define _NET_INL_
//#include "paramdict.inl"
#include "ncnn.inl"
#include "cfile.h"
static int find_blob_index_by_name(Net* net, const char* name)
{
  for (int i = 0; i < net->blobs_size; i++) {
    if (0==strcmp(net->blobs[i].name, name)) {
      return i;
    }
  }
  
  //fprintf(stderr, "find_blob_index_by_name %s failed\n", name);
  return -1;
}
static Blob* net_blob_by_name(Net* net, const char* name) {
  for (int i = 0; i < net->blobs_size; i++) {
    if (0==strcmp(net->blobs[i].name, name)) {
      return net->blobs + i;
    }
  }
  return NULL;
}

static Layer* net_layer_by_name(Net* net, const char* name) {
  for (int i = 0; i < net->blobs_size; i++) {
    if (0==strcmp(net->layers[i].name, name)) {
      return net->layers+i;
    }
  }
  return NULL;
}
static int net_forward_layer(Net* net, int layer_index)
{
  int i=0, ret = 0;
  const Layer* layer = net->layers + layer_index;
#if 0
  //     fprintf(stderr, "forward_layer %d %s\n", layer_index, layer->name.c_str());
  const int *bottoms = layer->blobs;
  const int *tops = layer->blobs + layer->bottoms_size;
  // load bottom blobs
  for (i = 0; i < layer->bottoms_size; i++) {
    if (imempty(net->blob_mats+bottoms[i])) {
      ret = net_forward_layer(net, net->blobs[bottoms[i]].producer);
      
      if (ret != 0) {
        return ret;
      }
    }
  }
  
  ret = Layer_forward(net, layer_index);
#endif
  //     fprintf(stderr, "forward_layer %d %s done\n", layer_index, layer->name.c_str());
  //     const Mat& blob = blob_mats[layer->tops[0]];
  //     fprintf(stderr, "[%-2d %-16s %-16s]  %d    blobs count = %-3d   size = %-3d x %-3d\n", layer_index, layer->type.c_str(), layer->name.c_str(), layer->tops[0], blob.c, blob.h, blob.w);
  return ret;
}
static
int net_input(Net* net, const char* blob_name, const img_t* im, int cn, float a, float b)
{
  int blob_index = find_blob_index_by_name(net, blob_name);
  //MYREALLOC(net->blob_mats, net->blobs_size);
  //BZERO(net->blob_mats, net->blobs_size);
  
  ASSERT((unsigned)blob_index < (int)net->blobs_size);
  if (blob_index == -1) {
    return -1;
  }
  
  Layer* layer = net->layers + net->blobs[blob_index].producer;
  //blob_mats[blob_index] = in;
  image2Matrix(im, net->blobs[blob_index].data, cn, a, b);
  return 0;
}

img_t* net_extract(Net* net, int blob_index)
{
  int ret = -1;
  if (blob_index < 0 || blob_index >= (int)net->blobs_size) {
    return NULL;
  }
  
  if (imempty(net->blobs[blob_index].data)) {
    
#ifdef _OPENMP
    int dynamic_current = 0;
    int num_threads_current = 1;
    
    if (num_threads) {
      dynamic_current = omp_get_dynamic();
      num_threads_current = omp_get_num_threads();
      omp_set_dynamic(0);
      omp_set_num_threads(num_threads);
    }
    
#endif
    
    ret = net_forward_layer(net, net->blobs[blob_index].producer);
    
#ifdef _OPENMP
    
    if (num_threads) {
      omp_set_dynamic(dynamic_current);
      omp_set_num_threads(num_threads_current);
    }
    
#endif
  }
  
  return net->blobs[blob_index].data;
}
static img_t* net_extract_name(Net* net, const char* blob_name)
{
  int blob_index = find_blob_index_by_name(net, blob_name);
  return net_extract(net, blob_index);
}
static int net_load_param(Net* net, const char* str, int len)
{
  int layer_cnt = 0;
  int blob_cnt = 0;
  str_t s = STR2(str, len), line;
  int i, j=0, nscan=0, p=0;
  
  // parse
  net->layers_size = 0;
  net->blobs_size = 0;
  cstr_getline(s, &j, &line);
  nscan = cstr_getint(line, &p, &net->layers_size, 0) &&
    cstr_getint(line, &p, &net->blobs_size, 0);
  if (!nscan) {
    return 0;
  }
  
  for (j=0; cstr_getline(s, &j, &line); net->layers_size++) {
  }
  
  MYREALLOC(net->layers, net->layers_size);
  MEMZERO(net->layers, net->layers_size);
  net->blobs_size = net->layers_size;
  MYREALLOC(net->blobs, net->blobs_size);
  MEMZERO(net->blobs, net->blobs_size);
  
  for (j=0; cstr_getline(s, &j, &line); ) {
    Layer *layer = net->layers + layer_cnt;
    char layer_type[256] = {0};
    char layer_name[256] = {0};
    int bottoms_size=0;
    int tops_size=0;
    p=0;
    nscan = cstr_getstr(line, &p, NULL, layer_type, countof(layer_type), NULL) &&
      cstr_getstr(line, &p, NULL, layer_name, countof(layer_name), NULL) &&
      cstr_getint(line, &p, &bottoms_size, 0) &&
      cstr_getint(line, &p, &tops_size, 0);
    
    if (!nscan) {
      printf("warning! fscanf\n");
      continue;
    }
    LLARG(bottoms_size) = bottoms_size;
    LLARG(tops_size) = tops_size;
    
    layer->blobs_size = bottoms_size + tops_size;
    
    ASSERT(layer->blobs_size <= MAX_BLOB);
    
    for (i = 0; i < layer->blobs_size; i++) {
      char blob_name[256];
      cstr_getstr(line, &p, NULL, blob_name, countof(blob_name), NULL);
      int bottom_blob_index = find_blob_index_by_name(net, blob_name);
      
      if (bottom_blob_index == -1) {
        Blob* blob;
        bottom_blob_index = blob_cnt++;
        MYREALLOC(net->blobs, blob_cnt);
        blob = net->blobs + bottom_blob_index;
        strncpy(blob->name, blob_name, countof(blob->name));
        blob->producer = layer_cnt;
        printf("new blob: %s\n", blob_name);
      }
      
      layer->blobs[i] = bottom_blob_index;
    }
    
    if (layer_init(layer, layer_type) < 0) {
      ASSERT(0);
    }
    str_t s_key, s_value;
    while (cstr_getkv(line, &p, &s_key, &s_value)) {
      bool ret = setLayerFArg(layer, s_key, s_value);
    }
    //net->data_size += LARG(data_size);
    //layer_registry[layer->type_index].load_param(layer, p);
    layer_cnt++;
  }
#if 0
  MYREALLOC(net->data, net->data_size);
  net->blobs_size = blob_cnt;
  net->layers_size = layer_cnt;
  int offset = 0;
  for (i = 0; i < net->layers_size; i++) {
    Layer *layer = net->layers + i;
    layer->data_offset = offset;
    offset += LARG(data_size);
  }
#endif
  
  return 1;
}
static int net_load_param_from_file(Net* net, const char* protopath, const char* binpath)
{
  int ret=0;
  buf_t bf[1] = {0};
  if (buf_load(protopath, bf)) {
    ret = net_load_param(net, (char*)bf->data, bf->len);
    bffree(bf);
  } else {
    fprintf(stderr, "fopen %s failed\n", protopath);
  }
  if (ret) {
#if 0
    if (ret = buf_load(binpath, bf)) {
      ASSERT(net->data_size*sizeof(float)==bf->len);
      if (net->data_size*sizeof(float)==bf->len) {
        memcpy(net->data, bf->data, bf->len);
      }
      ASSERT(net->data_size*sizeof(float)==bf->len);
    }
#endif
  }
  bffree(bf);
  
  return ret;
}
static int net_DebugInfo(Net* net, const int layer_id, const char* str) {
  Layer* layer = net->layers+layer_id;
  Dtype data_abs_val_mean;
  for (int i = 0; i < layer->blobs_size; ++i) {
    const Blob* blob = net->blobs+layer->blobs[i];
    data_abs_val_mean = pBox_asum(blob->data) / pBox_count(blob->data);
    Dtype data_min = pBox_min(blob->data);
    Dtype data_max = pBox_max(blob->data);
    printf("    [%s] Layer %s, Blob %s data: %f\n", str, layer->name, blob->name, data_abs_val_mean);
  }
  //data_abs_val_mean = blas_asum(LARG(data_size), net->data + layer->data_offset)/LARG(data_size);
  //LOG(GLOG_INFO, "    [Forward] Layer %s data: %f", layer->name, data_abs_val_mean);
  return 0;
}
static void net_Forward(Net* net) {
  for (int i = 0; i < net->layers_size; ++i) {
    Layer_forward(net, net->layers+i);
    if (net->debug_info) { net_DebugInfo(net, i, "Forward"); }
  }
}
static void net_Backward(Net* net) {
  int i = net->layers_size - 1;
  for (; i >= 0; --i) {
    Layer_backward(net, net->layers+i);
    if (net->debug_info) { net_DebugInfo(net, i, "Backward"); }
  }
}

#define LR_POLICY_DEFDEF(DEF) \
  
struct SolverParam {
  Method lr_policy;
  Method regularization_type;
  Dtype base_lr;
  Dtype gamma;
  int stepsize;
  int max_iter;
  int iter_size;
  Dtype power;
  Dtype momentum;
  Dtype clip_weights;
  Dtype clip_gradients;
  Dtype weight_decay;
  int clamp_weights;
  Dtype clamp_weights_lower;
  Dtype clamp_weights_upper;
};

static SolverParam* SolverParam_default(SolverParam* s) {
  // The base learning rate, momentum and the weight decay of the network.
  s->base_lr = 0.01;
  s->momentum = 0.9;
  s->weight_decay = 0.0005;
  // The learning rate policy
  s->lr_policy = Method_inv;
  s->gamma = 0.0001;
  s->power = 0.75;
  // The maximum number of iterations
  s->max_iter = 10000;
  return s;
}


//#include "caffe/sgd_solvers.hpp"

// Return the current learning rate. The currently implemented learning rate
// policies are as follows:
//    - fixed: always return base_lr.
//    - step: return base_lr * gamma ^ (floor(iter / step))
//    - exp: return base_lr * gamma ^ iter
//    - inv: return base_lr * (1 + gamma * iter) ^ (- power)
//    - multistep: similar to step but it allows non uniform steps defined by
//      stepvalue
//    - poly: the effective learning rate follows a polynomial decay, to be
//      zero by the max_iter. return base_lr (1 - iter/max_iter) ^ (power)
//    - sigmoid: the effective learning rate follows a sigmod decay
//      return base_lr ( 1/(1 + exp(-gamma * (iter - stepsize))))
//
// where base_lr, max_iter, gamma, step, stepvalue and power are defined
// in the solver parameter protocol buffer, and iter is the current iteration.
static Dtype GetLearningRate(SolverParam* param, Dtype base_lr, int iter_) {
  Dtype rate=0;
  enum Method lr_policy = param->lr_policy;
  switch(lr_policy) {
  case Method_fixed:
    rate = base_lr;
    break;
  case Method_step:
    rate = base_lr * pow(param->gamma, iter_*1. / param->stepsize);
    break;
  case Method_exp:
    rate = base_lr * pow(param->gamma, iter_);
    break;
  case Method_inv:
    rate = base_lr * pow(1. + param->gamma * iter_, - param->power);
    break;
  case Method_multistep:
    rate = base_lr * pow(param->gamma, iter_);
    break;
  case Method_poly:
    rate = base_lr * pow(1. -(iter_*1. / param->max_iter), param->power);
    break;
  case Method_sigmoid:
    rate = base_lr * 1. / (1. + exp(-param->gamma * (iter_ - param->stepsize)));
    break;
  default:
    LOG(GLOG_FATAL, "Unknown learning rate policy: %d", lr_policy);
    break;
  }
  return rate;
}
static void ClipWeights(Net* net, SolverParam* param) {
  if (param->clip_weights > 0) {
    Dtype l2norm_data, sumsq_data, clip_weights = param->clip_weights;
    int i, j;
    for (j=0; j<net->layers_size; ++j) {
      Layer* layer = net->layers + j;
      for (i=0; i<layer->weights_size; ++i) {
        int data_size = pBox_count(WBLOB(i)->data);
        Dtype* data = pBox_datap(WBLOB(i)->data);
        sumsq_data = blas_sumsq(data_size, data);
        l2norm_data = sqrt(sumsq_data);
        if (l2norm_data > clip_weights) {
          Dtype scale_factor = clip_weights / l2norm_data;
          LOG(GLOG_INFO, "Weight clipping: scaling down weights (L2 norm %f > %f) by scale factor %f", l2norm_data, clip_weights, scale_factor);
          blas_scale(data_size, scale_factor, data, data);
        }
      }
    }
  }
}
static void ClipGradients(Net* net, SolverParam* param) {
  if (param->clip_gradients > 0) {
    Dtype sumsq_diff, l2norm_diff, clip_gradients = param->clip_gradients;
    int i, j;
    for (j=0; j<net->layers_size; ++j) {
      Layer* layer = net->layers + j;
      for (i=0; i<layer->weights_size; ++i) {
        int data_size = pBox_count(WBLOB(i)->diff);
        Dtype* diff = pBox_datap(WBLOB(i)->diff);
        sumsq_diff = blas_sumsq(data_size, diff);
        l2norm_diff = sqrt(sumsq_diff);
        if (l2norm_diff > clip_gradients) {
          Dtype scale_factor = clip_gradients / l2norm_diff;
          LOG(GLOG_INFO, "Gradient clipping: scaling down gradients (L2 norm %f > %f) by scale factor %f", l2norm_diff, clip_gradients, scale_factor);
          blas_scale(data_size, scale_factor, diff, diff);
        }
      }
    }
  }
}

// Scale gradient to counterbalance accumulation.
static void Normalize(Net* net, SolverParam* param) {
  if (param->iter_size>1) {
    const Dtype accum_normalization = 1. / param->iter_size;
    int i, j;
    for (j=0; j<net->layers_size; ++j) {
      Layer* layer = net->layers + j;
      for (i=0; i<layer->weights_size; ++i) {
        int data_size = pBox_count(WBLOB(i)->diff);
        Dtype* diff = pBox_datap(WBLOB(i)->diff);
        blas_scal(data_size, accum_normalization, diff);
      }
    }
  }
}
static void Regularize(Net* net, SolverParam* param) {
  Dtype weight_decay = param->weight_decay;
  Dtype local_decay = weight_decay;
  if (local_decay>0) {
    int i, j;
    for (j=0; j<net->layers_size; ++j) {
      Layer* layer = net->layers + j;
      for (i=0; i<layer->weights_size; ++i) {
        int data_size = pBox_count(WBLOB(i)->diff);
        Dtype* data = pBox_datap(WBLOB(i)->data);
        Dtype* diff = pBox_datap(WBLOB(i)->diff);
        switch (param->regularization_type) {
        case Method_L2:
          // add weight decay
          blas_axpy(data_size, local_decay, data, diff);
          break;
        case Method_L1:
          //blas_sign(net->data_size, net->data, temp_[param_id]->data);
          //blas_axpy(net->data_size, local_decay, temp_[param_id]->cpu_data(), net->diff);
          blas_axpy_sign(data_size, local_decay, data, diff);
        default:
          LOG(GLOG_FATAL, "Unknown regularization type: %d", param->regularization_type);
          break;
        }
      }
    }
  }
}
static void ComputeUpdateValue(Net* net, SolverParam* param, Dtype rate) {
  Dtype momentum = param->momentum;
  Dtype local_rate = rate;
  // Compute the update to history, then copy it to the parameter diff.
  int i, j;
  for (j=0; j<net->layers_size; ++j) {
    Layer* layer = net->layers + j;
    for (i=0; i<layer->weights_size; ++i) {
      int data_size = pBox_count(WBLOB(i)->diff);
      Dtype* data = pBox_datap(WBLOB(i)->data);
      Dtype* diff = pBox_datap(WBLOB(i)->diff);
      blas_axpby(data_size, rate, diff, param->momentum, diff);
    }
  }
}
static void ClampWeights(Net* net, SolverParam* param) {
  if (param->clamp_weights) {
    const Dtype lower_bound = param->clamp_weights_lower;
    const Dtype upper_bound = param->clamp_weights_upper;
    int i, j;
    for (j=0; j<net->layers_size; ++j) {
      Layer* layer = net->layers + j;
      for (i=0; i<layer->weights_size; ++i) {
        int data_size = pBox_count(WBLOB(i)->data);
        Dtype* data = pBox_datap(WBLOB(i)->data);
        blas_clamp(data_size, lower_bound, upper_bound, data, data);
      }
    }
  }
}

static void net_clear_diff(Net* net) {
  //blas_set(net->data_size, 0, net->diff);
  int i, j;
  for (j=0; j<net->layers_size; ++j) {
    Layer* layer = net->layers + j;
    for (i=0; i<layer->blobs_size; ++i) {
      Blob* blob = LBLOB(i);
      pBox_ReshapeLike(blob->diff, blob->data);
      pBox_fill(blob->diff, 0);
    }
  }
}

static Dtype net_mse_loss(struct Net* net, int y, int t) {
  Blob* y_blob = BLOB(y);
  Blob* t_blob = BLOB(t);
  Dtype* y_data = pBox_datap(y_blob->data);
  Dtype* t_data = pBox_datap(t_blob->data);
  Dtype* y_diff = pBox_datap(y_blob->diff);
  int count = pBox_count(y_blob->data);
  ASSERT(count == pBox_count(t_blob->data));
  blas_sub(count, y_data, t_data, y_diff);
  Dtype loss = blas_dot(count, y_diff, y_diff);
  loss /= count;
  return loss;
}

static Dtype net_accuracy(struct Net* net, int y, int t) {
  Blob* y_blob = BLOB(y);
  Blob* t_blob = BLOB(t);
  Dtype* y_data = pBox_datap(y_blob->data);
  Dtype* t_data = pBox_datap(t_blob->data);
  //int count = pBox_count(y_blob->data);
  int c = pBox_channels(y_blob->data);
  ASSERT(y_blob->data->f==t_blob->data->f);
  int i, j, acc_count=0;
  for (i=0; i<y_blob->data->f; ++i) {
    int y_index = blas_argmax(c, y_data+i*c);
    int t_index = blas_argmax(c, t_data+i*c);
    acc_count += (y_index==t_index);
  }
  Dtype acc = acc_count * 1./y_blob->data->f;
  return acc;
}

static void net_Update(Net* net) {
  int i, j;
  for (j=0; j<net->layers_size; ++j) {
    Layer* layer = net->layers + j;
    for (i=0; i<layer->weights_size; ++i) {
      int data_size = pBox_count(WBLOB(i)->diff);
      Dtype* data = pBox_datap(WBLOB(i)->data);
      Dtype* diff = pBox_datap(WBLOB(i)->diff);
      blas_axpy(data_size, (Dtype)(-1), diff, data);
    }
  }
}
static void net_UpdateSGD(Net* net, SolverParam* param, int iter_) {
  Dtype rate = GetLearningRate(param, param->base_lr, iter_);
  ClipGradients(net, param);
  Normalize(net, param);
  Regularize(net, param);
  ComputeUpdateValue(net, param, rate);
  ClipWeights(net, param);
  ClampWeights(net, param);
  net_Update(net);
}
static int net_UpdateSGD(Net* net, Dtype rate) {
  // Compute the update to history, then copy it to the parameter diff.
  int i, j, k;
  Dtype max_diff = FLT_MIN, min_diff = FLT_MAX;
  for (j=0; j<net->layers_size; ++j) {
    Layer* layer = net->layers + j;
    for (i=0; i<layer->weights_size; ++i) {
      Blob* blob = WBLOB(i);
      int count = pBox_count(blob->diff);
      Dtype* data = pBox_datap(blob->data);
      Dtype* diff = pBox_datap(blob->diff);
      blas_axpy(count, -rate, diff, data);
      for (k=0; k<count; ++k) {
        max_diff = MAX(max_diff, diff[k]);
        min_diff = MIN(min_diff, diff[k]);
      }
    }
  }
  if (net->debug_info) {
    printf("min_diff=%f max_diff=%f\n", min_diff, max_diff);
  }
  return 1;
}
static Dtype net_trainonce(struct Net* net, int y, int t, Dtype rate, int iters) {
  net_clear_diff(net);
  Dtype loss = 0;
  int i=0;
  for (i=0; i<iters; ++i) {
    net_Forward(net);
    loss += net_mse_loss(net, y, t);
    net_Backward(net);
  }
  Blob* y_blob = BLOB(y);
  int count = pBox_count(y_blob->data);
  int f = y_blob->data->f;
  rate /= (f);
  loss /= iters;
  net_UpdateSGD(net, rate);
  return loss;
}
static int net_addBlob(Net* net, const char* blob_name, int w, int h DEFAULT(1), int c DEFAULT(1), int f DEFAULT(1), const Dtype* data DEFAULT(NULL)) {
  Blob* blob = NULL;
  int index = find_blob_index_by_name(net, blob_name);
  if (index<0) {
    index = net->blobs_size++;
    MYREALLOC(net->blobs, net->blobs_size);
  } else {
    ASSERT(0);
  }
  blob = net->blobs + index;
  BZERO1(blob);
  strncpy(blob->name, blob_name, countof(blob->name));
  pBox_create(blob->data, w, h, c, f, data);
  return index;
}
static int net_addBlob_auto(Net* net, const char* type, const char* postfix, int w, int h DEFAULT(1), int c DEFAULT(1), int f DEFAULT(1), const Dtype* data DEFAULT(NULL)) {
  char name[256];
  _snprintf(name, countof(name), "%s_%s_%d", type, postfix ? postfix : "", net->blobs_size);
  return net_addBlob(net, name, w, h, c, f, data);
}

static Layer* net_addLayer(Net* net, const char* lname, int bottoms_size, int tops_size, int weights_size, layer_run_t run) {
  int index = net->layers_size++;
  MYREALLOC(net->layers, net->layers_size);
  Layer* layer = net->layers + index;
  BZERO1(layer);
  LLARG(bottoms_size) = bottoms_size;
  LLARG(tops_size) = tops_size;
  LLARG(weights_size) = weights_size;
  layer->blobs_size = bottoms_size+tops_size+weights_size;
  layer->run = run;
  _snprintf(layer->name, countof(layer->name), "%s_%d", lname, index);
  return layer;
}
static void net_rng_gaussian(Net* net, int blob, Dtype _Mean, Dtype sigma) {
  float* data = pBox_datap(BLOB(blob)->data);
  int data_size = pBox_count(BLOB(blob)->data);
  blas_rng_gaussian(data_size, _Mean, sigma, data);
  return ;
}
static void net_rng_uniform(Net* net, int blob, Dtype a, Dtype b) {
  float* data = pBox_datap(BLOB(blob)->data);
  int data_size = pBox_count(BLOB(blob)->data);
  blas_rng_uniform(data_size, a, b, data);
  return ;
}
static void net_rng_xavier(Net* net, int blob) {
  //float* data = pBox_datap(BLOB(blob)->data);
  //int data_size = pBox_count(BLOB(blob)->data);
  pBox_XavierFiller(BLOB(blob)->data);
  return ;
}
void net_constant(Net* net, int blob, const Dtype _Value) {
  float* data = pBox_datap(BLOB(blob)->data);
  int data_size = pBox_count(BLOB(blob)->data);
  blas_set(data_size, _Value, data);
  return ;
}
static int net_conv2d(Net* net, int inblob, int* pw, int* pb, int kernel_size, int num_output, int stride DEFAULT(1), int pad DEFAULT(0), int group DEFAULT(1), int dilation DEFAULT(1)) {
  bool bias_term = (pb!=NULL);
  const char* type = "conv";
  Layer* layer = net_addLayer(net, type, 1, 1, 1+bias_term, ConvolutionLayer);
  int kernel_w = kernel_size;
  int kernel_h = kernel_size;
  int input_c=0;
  int input_w=0;
  int input_h=0;
  int input_f=0;
  {
    img_t* input = net->blobs[inblob].data;
    input_c = pBox_channels(input);
    input_w = input->w;
    input_h = input->h;
    input_f = input->f;
  }
  IWBLOB(0) = *pw = net_addBlob_auto(net, type, "w", kernel_w, kernel_h, input_c, num_output);
  if (pb) {
    IWBLOB(1) = *pb = net_addBlob_auto(net, type, "b", num_output);
  }
  LARG(bias_term) = bias_term;
  LARG(kernel_h) = kernel_h;
  LARG(kernel_w) = kernel_w;
  LARG(stride_h) = stride;
  LARG(stride_w) = stride;
  LARG(dilation) = dilation;
  LARG(num_output) = num_output;
  LARG(pad) = pad;
  LARG(group) = group;
  ILBLOB(0) = inblob;
  int output_h = (input_h + 2 * pad - (dilation * (kernel_h - 1) + 1)) / stride + 1;
  int output_w = (input_w + 2 * pad - (dilation * (kernel_w - 1) + 1)) / stride + 1;
  return ITBLOB(0) = net_addBlob_auto(net, type, "o", output_w, output_h, num_output, input_f);
}
static int net_pool(Net* net, int inblob, enum Method poolType, int kernel_h, int kernel_w, int stride DEFAULT(1), int pad DEFAULT(0))
{
  const char* type = "pool";
  Layer* layer = net_addLayer(net, type, 1, 1, 0, PoolingLayer);
  img_t* input = net->blobs[inblob].data;
  int inCh = pBox_channels(input);
  int stride_w = stride;
  int stride_h = stride;
  int pad_w = pad;
  int pad_h = pad;
  LARG(kernel_h) = kernel_h;
  LARG(kernel_w) = kernel_w;
  LARG(stride_h) = stride_h;
  LARG(stride_w) = stride_w;
  LARG(pad) = pad;
  LARG(pool) = poolType;
  layer->run = PoolingLayer;
  ILBLOB(0) = inblob;
  
  int pooled_h = (int)(ceil((float)(input->h + 2 * pad_h - kernel_h) / stride_h)) + 1;
  int pooled_w = (int)(ceil((float)(input->w + 2 * pad_w - kernel_w) / stride_w)) + 1;
  if (pad_h || pad_w) {
    // If we have padding, ensure that the last pooling starts strictly
    // inside the image (instead of at the padding); otherwise clip the last.
    if ((pooled_h - 1) * stride_h >= input->h + pad_h) {
      --pooled_h;
    }
    if ((pooled_w - 1) * stride_w >= input->w + pad_w) {
      --pooled_w;
    }
    CHECK_LT((pooled_h - 1) * stride_h, input->h + pad_h);
    CHECK_LT((pooled_w - 1) * stride_w, input->w + pad_w);
  }

  return ITBLOB(0) = net_addBlob_auto(net, type, "o", pooled_w, pooled_h, inCh, input->f);
}
static int net_matmul(Net* net, int inblob, int* pw, int* pb, int num_output, bool transpose DEFAULT(false))
{
  const char* type = "matmul";
  img_t* input = net->blobs[inblob].data;
  bool bias_term = (pb!=NULL);
  int num_input = input->f;
  int len_input = pBox_length(input);
  Layer* layer = net_addLayer(net, type, 1, 1, 1+bias_term, InnerProductLayer);
  LARG(transpose) = transpose;
  
  if (transpose) {
    IWBLOB(0) = *pw = net_addBlob_auto(net, type, "w", num_output, len_input);
  } else {
    IWBLOB(0) = *pw = net_addBlob_auto(net, type, "w", len_input, num_output);
  }
  if (pb) {
    IWBLOB(1) = *pb = net_addBlob_auto(net, type, "b", num_output);
  }
  LARG(bias_term) = bias_term;
  LARG(num_output) = num_output;
  ILBLOB(0) = inblob;
  return ITBLOB(0) = net_addBlob_auto(net, type, "o", 1, 1, num_output, num_input);
}
static int net_relu(Net* net, int inblob, Dtype slope DEFAULT(0))
{
  const char* type = "relu";
  img_t* input = net->blobs[inblob].data;
  Layer* layer = net_addLayer(net, type, 1, 1, 0, ReLULayer);
  ILBLOB(0) = inblob;
  FARG(slope) = slope;
  return ITBLOB(0) = net_addBlob_auto(net, type, "o", input->w, input->h, pBox_channels(input), input->f);
}
static int net_sigmoid(Net* net, int inblob)
{
  const char* type = "sigmoid";
  img_t* input = net->blobs[inblob].data;
  Layer* layer = net_addLayer(net, type, 1, 1, 0, SigmoidLayer);
  ILBLOB(0) = inblob;
  return ITBLOB(0) = net_addBlob_auto(net, type, "o", input->w, input->h, pBox_channels(input), input->f);
}
static int net_tanh(Net* net, int inblob)
{
  const char* type = "tanh";
  img_t* input = net->blobs[inblob].data;
  Layer* layer = net_addLayer(net, type, 1, 1, 0, TanHLayer);
  ILBLOB(0) = inblob;
  return ITBLOB(0) = net_addBlob_auto(net, type, "o", input->w, input->h, pBox_channels(input), input->f);
}
static int net_softmax(Net* net, int inblob, int axis DEFAULT(2))
{
  const char* type = "softmax";
  img_t* input = net->blobs[inblob].data;
  Layer* layer = net_addLayer(net, type, 1, 1, 0, SoftmaxLayer);
  ILBLOB(0) = inblob;
  LARG(axis) = axis;
  return ITBLOB(0) = net_addBlob_auto(net, type, "o", input->w, input->h, pBox_channels(input), input->f);
}
static void pBox_zero(img_t* im) {
  memset(im->data, 0, im->h*im->w*im->c*im->f);
}
static img_t* onehot(const img_t* label, int c, img_t* prob, int* perm DEFAULT(NULL), int batch DEFAULT(0)) {
  pBox_create(prob, c, 1, 1, perm ? batch: label->f);
  float* p = pBox_datap(prob);
  uchar* l = label->data;
  int i=0;
  if (perm) {
    for (; i<prob->f; ++i) {
      p[i*c + l[perm[i]]] = 1;
    }
  } else {
    for (; i<prob->f; ++i) {
      p[i*c + l[i]] = 1;
    }
  }
  return prob;
}

static Dtype net_get_loss(struct Net* net) {
  int y = net->layers[net->layers_size-1].blobs[1];
  Blob* y_blob = BLOB(y);
  Dtype* y_data = pBox_datap(y_blob->data);
  Dtype* y_diff = pBox_datap(y_blob->diff);
  int count = pBox_count(y_blob->data);
  blas_copy(count, y_data, y_diff);
  Dtype loss = blas_dot(count, y_diff, y_diff)/2.;
  return loss;
}

// gradient_check
bool net_gradient_check(Net* net, Dtype delta, Dtype eps) {
  assert(net->layers_size>0);
  int i, j, check_index;
  for (i=0; i<net->layers_size; ++i) { // ignore first input layer
    Layer* layer = net->layers + i;
    for (j = 0; j < layer->bottoms_size; ++j) {
      Blob* blob = LBLOB(j);
      int count = pBox_count(blob->data);
      for (check_index = 0; check_index < count; ++check_index) {
        //Dtype err = calc_delta_diff(net, layer, blob, check_index, delta);
        net_clear_diff(net);
        net_Forward(net);
        Dtype loss = net_get_loss(net);
        net_Backward(net);
        Dtype* data = pBox_pf(blob->data);
        Dtype* diff = pBox_pf(blob->diff);
        Dtype delta_by_bprop = diff[check_index];
        
        // calculate dw/dE by numeric
        Dtype prev_w = data[check_index];
        data[check_index] = prev_w + delta;
        net_clear_diff(net);
        net_Forward(net);
        Dtype f_p = net_get_loss(net);
        
        data[check_index] = prev_w - delta;
        net_clear_diff(net);
        net_Forward(net);
        Dtype f_m = net_get_loss(net);
        
        Dtype delta_by_numerical = (f_p - f_m) / (2.0 * delta);
        
        data[check_index] = prev_w;
        Dtype err = fabs(delta_by_bprop - delta_by_numerical);
        if (err > eps) {
          printf("delta_by_bprop=%f \ndelta_by_numerical=%f", delta_by_bprop, delta_by_numerical);
          ASSERT(0);
          return false;
        }
      }
    }
  }
  return true;
}


#endif // _NET_INL_
