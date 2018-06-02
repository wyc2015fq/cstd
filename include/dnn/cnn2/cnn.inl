#include "utils.inl"
#include "mathfunctions.inl"
#include "blob.inl"

#define DECLARE_LAYER_TYPE static const string layerType;
#define DEFINE_LAYER_TYPE(class_type,type_string) const string class_type::layerType = type_string;
#define FRIEND_WITH_NETWORK friend class struct NetWork;
#define MAXIDS 16
#define BLOB(_i) (net->blobs+(_i))
#define LIBLOB(i) (layer->id[i])
#define LOBLOB(i) (layer->id[i+layer->i_size])
#define LWBLOB(i) (layer->id[i+layer->i_size+layer->o_size])
#define IBLOB(i) BLOB(LIBLOB(i))
#define OBLOB(i) BLOB(LOBLOB(i))
#define WBLOB(i) BLOB(LWBLOB(i))
#define METHOD_DEFDEF(DEF) \
  DEF(Method, Max) \
  DEF(Method, Mean) \
  DEF(Method, VALID) \
  DEF(Method, SAME) \
  DEF(Method, Train) \
  DEF(Method, Test)
enum Method {
#define METHOD_DEF(type, name) M_ ## name,
  METHOD_DEFDEF(METHOD_DEF)
#undef METHOD_DEF
};
#define METHODTYPE_DEFDEF(DEF) \
  DEF(Method, paddingType) \
  DEF(Method, poolingType) \
  DEF(Method, VALID) \
  DEF(Method, SAME)
#define LAYERARGTYPE_DEFDEF(DEF) \
  DEF(Int, input_c) \
  DEF(Int, input_h) \
  DEF(Int, input_w) \
  DEF(Int, kernel_w) \
  DEF(Int, kernel_h) \
  DEF(Int, stride_w) \
  DEF(Int, stride_h) \
  DEF(Int, dilation) \
  DEF(Int, num_output) \
  DEF(Float, rate) \
  METHODTYPE_DEFDEF(DEF)
#define NETARGTYPE_DEFDEF(DEF) \
  DEF(Float, lr) \
  DEF(Float, momentum) \
  DEF(Method, phase)
enum LayerArgId {
#define LAYERARGTYPE_DEF(type, name) LA_ ## name,
  LAYERARGTYPE_DEFDEF(LAYERARGTYPE_DEF)
#undef LAYERARGTYPE_DEF
  LayerArgCount,
};
enum NetArgId {
#define NETARGTYPE_DEF(type, name) NA_ ## name,
  NETARGTYPE_DEFDEF(NETARGTYPE_DEF)
#undef NETARGTYPE_DEF
  NetArgCount,
};
#define IARG(name) layer->x[LA_ ## name].i
#define FARG(name) layer->x[LA_ ## name].f
#define MARG(name) layer->x[LA_ ## name].m
#define INARG(name) net->x[NA_ ## name].i
#define FNARG(name) net->x[NA_ ## name].f
#define MNARG(name) net->x[NA_ ## name].m
enum LayerOp {
  Op_Init,
  Op_Forward,
  Op_Backward,
};
#define Layer_forward(layer) (layer)->run(Op_Forward, net, layer)
#define Layer_backward(layer) (layer)->run(Op_Backward, net, layer)
typedef void (*LayerFun)(enum LayerOp op, struct NetWork* net, struct Layer* layer);
union LayerArg {
  int i;
  float f;
  enum Method m;
};
struct Layer {
  LayerFun run;
  char name[MAXNAME];
  const char* type;
  union LayerArg x[LayerArgCount];
  int id[MAXIDS];
  int i_size, o_size, w_size;
};
struct NetWork {
  struct Layer* layers;
  struct Blob* blobs;
  int l_size, b_size;
  struct Layer lossFunctor[1];
  union LayerArg x[NetArgCount];
};
void net_free(struct NetWork* net) {
  int i;
  for (i=0; i<net->b_size; ++i) {
    Blob_free(net->blobs + i);
  }
  free(net->layers);
  memset(net, 0, sizeof(*net));
}
struct Layer* initLayer(struct Layer* layer, int i_size, int o_size, int w_size, LayerFun run, const char* type)
{
  setzero(layer);
  layer->i_size = i_size;
  layer->o_size = o_size;
  layer->w_size = w_size;
  layer->run = run;
  layer->type = type;
  return layer;
}
static int find_or_insert_blob(struct NetWork* net, const char* name, int bInsert)
{
  int i=0;
  for (i = 0; i < net->b_size; i++) {
    if (0==stricmp(net->blobs[i].name, name)) {
      return i;
    }
  }
  if (bInsert) {
    i = net->b_size++;
    resetArr(&net->blobs, net->b_size);
    setzero(net->blobs + i);
    strncpy(net->blobs[i].name, name, MAXNAME);
  }
  
  //fprintf(stderr, "find_blob_index_by_name %s failed\n", name);
  return i;
}
static int addBlob(struct NetWork* net, DataSize size, const char* type, const char* name)
{
  int i = net->b_size;
  char buf[MAXNAME];
  if (name && *name) {
    _snprintf(buf, MAXNAME, "%s_%s_%d", type, name, i);
  } else {
    _snprintf(buf, MAXNAME, "%s_%d", type, i);
  }
  i = find_or_insert_blob(net, buf, 1);
  //struct Blob* blob = resetBlob(net->blobs + i, size);
  net->blobs[i].size = size;
  return i;
}
static int countLayerBuType(const struct NetWork* net, const char* type) {
  int i, c=0;
  for (i=0; i<net->l_size; ++i) {
    c += (0==strcmp(type, net->layers[i].type));
  }
  return c;
}
struct Layer* addLayer(struct NetWork* net, int i_size, int o_size, int w_size, LayerFun run, const char* type)
{
  int cnt = countLayerBuType(net, type);
  int i = net->l_size++;
  struct Layer* layer;
  logVerbose("struct NetWork addayer begin , type : %s", type);
  resetArr(&net->layers, net->l_size);
  layer = initLayer(net->layers + i, i_size, o_size, w_size, run, type);
  _snprintf(layer->name, MAXNAME, "%s_%d", type, cnt);
  //dataBucket setting params
  logVerbose("struct NetWork addayer end. add data blob done.");
  return net->layers + i;
}
static void InputLayer(enum LayerOp op, struct NetWork* net, struct Layer* layer)
{
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  switch (op) {
  case Op_Forward:
    break;
  case Op_Backward:
    break;
  default:
    break;
  }
}
static int net_null_1_1(struct NetWork* net, int input, LayerFun run, const char* type)
{
  struct Layer* layer = addLayer(net, 1, 1, 0, run, type);
  LIBLOB(0) = input;
  return LOBLOB(0) = addBlob(net, IBLOB(0)->size, type, "o");
}
#define net_sigmod(net, input) net_null_1_1(net, input, SigmodLayer, "sigmod")
static void SigmodLayer(enum LayerOp op, struct NetWork* net, struct Layer* layer)
{
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  switch (op) {
  case Op_Forward:
    next->size = prev->size;
    {
    const float* prevData = getData(prev);
    float* nextData = setDataZero(next);
    DISPATCH_WORKER_BEGIN(prev->size.n) {
      const int offset = start * Data_3DSize(prev->size);
      const int total_size = (stop - start) * Data_3DSize(prev->size);
      sigmoid(prevData + offset, nextData + offset, total_size);
    };
    DISPATCH_WORKER_END;
  }
  break;
  case Op_Backward: {
    const float* nextData = getData(next);
    float* prevDiff = setDiffZero(prev);
    const float* nextDiff = getDiff(next);
    easyAssert(Data_4DEqu(prev->size, next->size), "size must be equal!");
    //update prevDiff
    DISPATCH_WORKER_BEGIN(prev->size.n) {
      const int offset = start * Data_3DSize(prev->size);
      const int total_size = (stop - start) * Data_3DSize(prev->size);
      //calculate current inner diff
      df_sigmoid(nextData + offset, prevDiff + offset, total_size);
      //multiply next diff
      mul_inplace(prevDiff + offset, nextDiff + offset, total_size);
    };
    DISPATCH_WORKER_END;
    //update this layer's param
    //Tanh layer : nop
  }
  break;
  default:
    break;
  }
}
#define net_tanh(net, input) net_null_1_1(net, input, TanhLayer, "tanh")
static void TanhLayer(enum LayerOp op, struct NetWork* net, struct Layer* layer)
{
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  switch (op) {
  case Op_Forward: 
    next->size = prev->size;{
    const float* prevData = getData(prev);
    float* nextData = setDataZero(next);
    DISPATCH_WORKER_BEGIN(prev->size.n) {
      const int offset = start * Data_3DSize(prev->size);
      const int total_size = (stop - start) * Data_3DSize(prev->size);
      tanh1(prevData + offset, nextData + offset, total_size);
    };
    DISPATCH_WORKER_END;
  }
  break;
  case Op_Backward: {
    const float* nextData = getData(next);
    float* prevDiff = setDiffZero(prev);
    const float* nextDiff = setDiffZero(next);
    easyAssert(Data_4DEqu(prev->size, next->size), "size must be equal!");
    easyAssert(Data_4DEqu(prev->size, prev->size), "size of prevDiff and size of prev must be equals");
    //update prevDiff
    DISPATCH_WORKER_BEGIN(prev->size.n) {
      const int offset = start * Data_3DSize(prev->size);
      const int total_size = (stop - start) * Data_3DSize(prev->size);
      //calculate current inner diff
      df_tanh(nextData + offset, prevDiff + offset, total_size);
      //multiply next diff
      mul_inplace(prevDiff + offset, nextDiff + offset, total_size);
    };
    DISPATCH_WORKER_END;
    //update this layer's param
    //Tanh layer : nop
  }
  break;
  default:
    break;
  }
}
#define net_relu(net, input) net_null_1_1(net, input, ReluLayer, "relu")
static void ReluLayer(enum LayerOp op, struct NetWork* net, struct Layer* layer)
{
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  switch (op) {
  case Op_Forward:
    next->size = prev->size; {
    const float* prevData = getData(prev);
    float* nextData = setDataZero(next);
    DISPATCH_WORKER_BEGIN(prev->size.n) {
      const int offset = start * Data_3DSize(prev->size);
      const int total_size = (stop - start) * Data_3DSize(prev->size);
      relu(prevData + offset, nextData + offset, total_size);
    };
    DISPATCH_WORKER_END;
  }
  break;
  case Op_Backward: {
    const float* nextData = getData(next);
    const float* nextDiff = getDiff(next);
    float* prevDiff = setDiffZero(prev);
    easyAssert(Data_4DEqu(prev->size, next->size), "size must be equal!");
    //update prevDiff
    DISPATCH_WORKER_BEGIN(prev->size.n) {
      const int offset = start * Data_3DSize(prev->size);
      const int total_size = (stop - start) * Data_3DSize(prev->size);
      //calculate current inner diff
      df_relu(nextData + offset, prevDiff + offset, total_size);
      //multiply next diff
      mul_inplace(prevDiff + offset, nextDiff + offset, total_size);
    };
    DISPATCH_WORKER_END;
    //update this layer's param
    //RELU layer : nop
  }
  break;
  default:
    break;
  }
}
static void BatchNormalizationLayer(enum LayerOp op, struct NetWork* net, struct Layer* layer)
{
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  switch (op) {
  case Op_Forward: {
    easyAssert(Data_4DEqu(prev->size, next->size), "size must be equal!");
  }
  break;
  case Op_Backward: {
    float* prevDiff = setDiffZero(prev);
    easyAssert(Data_4DEqu(prev->size, next->size), "size must be equal!");
  }
  break;
  default:
    break;
  }
}
static DataSize conv_calc_outsize(DataSize input, DataSize kernel, int stride_w, int stride_h, int dilation, enum Method paddingType)
{
  DataSize output;
  const int kernel_extent_h = dilation * (kernel.h - 1) + 1;
  const int kernel_extent_w = dilation * (kernel.w - 1) + 1;

  output.n = input.n;
  output.c = kernel.n;
  if (paddingType == M_VALID) {
    output.w = (input.w - kernel_extent_w) / stride_w + 1;
    output.h = (input.h - kernel_extent_h) / stride_h + 1;
  }
  else if (paddingType == M_SAME) {
    output.w = input.w;
    output.h = input.h;
  }
  return output;
}
static void ConvolutionLayer(enum LayerOp op, struct NetWork* net, struct Layer* layer)
{
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  struct Blob* kernel = WBLOB(0);
  struct Blob* bias = layer->w_size>1 ? WBLOB(1) : NULL;
  int stride_w = MAX(1, IARG(stride_w));
  int stride_h = MAX(1, IARG(stride_h));
  int dilation = MAX(1, IARG(dilation));
  enum Method paddingType = MARG(paddingType);
  const char* mode = paddingType==M_SAME?"S":"V";
  switch (op) {
  case Op_Forward:
    //test_conv();
    next->size = conv_calc_outsize(prev->size, kernel->size, stride_w, stride_h, dilation, paddingType);
    {
      const float* prevData = getData(prev);
      const float* kernelData = getData(kernel);
      const float* biasData = bias ? getData(bias) : NULL;
      float* nextData = setDataZero(next);
      DISPATCH_WORKER_BEGIN(prev->size.n) {
        DataSize inputSize = prev->size;
        inputSize.n = stop - start;
        //((1) ? conv2d_gen : conv2d_cpu_gemm)
        //conv2d_gen
        //conv2d_fast
        conv2d_forward(prevData + start * Data_3DSize(prev->size), kernelData, biasData, nextData + start * Data_3DSize(next->size),
          inputSize, kernel->size, next->size, stride_w, stride_h, dilation, mode);
      };
      DISPATCH_WORKER_END;
      //showmat("prev", prev->data, prev->size);    showmat("kernel", kernel->data, kernel->size);    WaitKey(-1);
    }
    break;
  case Op_Backward:
    {
      const float* nextDiff = getDiff(next);
      const float* kernelData = getData(kernel);
      float* prevDiff = setDiffZero(prev);
      //update bias gradient
      //tic;
      if (bias) {
        float* biasDiff = setDiffZero(bias);
        conv2d_backward_bias(biasDiff, nextDiff, next->size);
        div_inplace(biasDiff, (float)next->size.n, next->size.c);
      }
      //toc;
      //update this layer's param
      //tic;
      if (kernel) {
        const float* prevData = getData(prev);
        float* kernelDiff = setDiffZero(kernel);
        conv2d_backward_weight(prevData, nextDiff, kernelDiff, prev->size, kernel->size, next->size, stride_h, stride_w, dilation, mode);
        div_inplace(kernelDiff, (float)next->size.n, Data_total(kernel->size));
      }
      //toc;
      conv2d_backward(nextDiff, kernelData, prevDiff, prev->size, kernel->size, next->size, stride_h, stride_w, dilation, mode);
    }
    break;
  default:
    break;
  }
}
static int net_init_normal(struct NetWork* net, int i, const float mean_value, const float standard_deviation)
{
  struct Blob* blob = BLOB(i);
  normal_distribution_init(setData(blob), Data_total(blob->size), mean_value, standard_deviation);
  return i;
}
static int net_init_const(struct NetWork* net, int i, float const_value)
{
  struct Blob* blob = BLOB(i);
  const_distribution_init(setData(blob), Data_total(blob->size), const_value);
  return i;
}
static int net_input(struct NetWork* net, DataSize size) {
  const char* type = "input";
  struct Layer* layer = addLayer(net, 0, 1, 0, InputLayer, type);
  IARG(input_c) = size.c;
  IARG(input_h) = size.h;
  IARG(input_w) = size.w;
  return LOBLOB(0) = addBlob(net, size, type, "");
}
static int net_conv(struct NetWork* net, int input, int* w, int* b, int num_output, int kernel_w, int kernel_h, int stride_w, int stride_h, enum Method paddingType)
{
  const char* type = "conv";
  int enabledBias = b != NULL;
  struct Layer* layer = addLayer(net, 1, 1, 1 + enabledBias, ConvolutionLayer, type);
  DataSize prevSize, kernelSize;
  int dilation = 1;
  easyAssert(stride_w > 0 && stride_h > 0, "step is invalidate.");
  IARG(kernel_w) = kernel_w;
  IARG(kernel_h) = kernel_h;
  IARG(stride_w) = stride_w;
  IARG(stride_h) = stride_h;
  IARG(num_output) = num_output;
  IARG(dilation) = dilation;
  MARG(paddingType) = paddingType;
  LIBLOB(0) = input;
  prevSize = IBLOB(0)->size;
  kernelSize = dataSize(num_output, prevSize.c, kernel_h, kernel_w);
  LWBLOB(0) = *w = addBlob(net, kernelSize, type, "w");
  net_init_normal(net, *w, 0.0f, 0.1f);
  if (b) {
    LWBLOB(1) = *b = addBlob(net, dataSize(num_output, 1, 1, 1), type, "b");
    net_init_const(net, *b, 0.f);
  }
  return LOBLOB(0) = addBlob(net, conv_calc_outsize(prevSize, kernelSize, stride_w, stride_h, dilation, paddingType), type, "o");
}
static void DropoutLayer(enum LayerOp op, struct NetWork* net, struct Layer* layer)
{
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  float rate = FARG(rate);
  int i, j;
  DataSize maskSize = prev->size;
  maskSize.n = 1;
  switch (op) {
  case Op_Forward:
    next->size = prev->size;
    {
    const float* prevData = getData(prev);
    float* nextData = setData(next);
    easyAssert(Data_4DEqu(prev->size, next->size), "size must be equal!");
    //init rand seed
    srand((unsigned int)time(NULL));
    if (MNARG(phase) == M_Train) {
      //fill mask
      float* maskData = reset(&next->bufs[0], maskSize);
      //const_distribution_init(maskData, maskSize Data_total(), 1.0f);
      for (i = 0; i < Data_total(maskSize); i++) {
        maskData[i] = (float)(bernoulli(rate));
      }
      for (i = 0; i < next->size.n; i++) {
        for (j = 0; j < Data_3DSize(next->size); j++) {
          const int dataIdx = i * Data_3DSize(next->size) + j;
          nextData[dataIdx] = prevData[dataIdx] * maskData[j] / rate;
        }
      }
    }
    else {
      for (i = 0; i < Data_total(next->size); i++) {
        nextData[i] = prevData[i];
      }
    }
  }
    break;
  case Op_Backward: {
    const float* maskData = getBuff(next, 0);
    const float* nextDiff = getDiff(next);
    float* prevDiff = setDiffZero(prev);
    easyAssert(Data_4DEqu(prev->size, next->size), "size must be equal!");
    //update prevDiff
    //calculate current inner diff && multiply next diff
    for (i = 0; i < next->size.n; i++) {
      for (j = 0; j < Data_3DSize(next->size); j++) {
        const int dataIdx = i * Data_3DSize(next->size) + j;
        prevDiff[dataIdx] = nextDiff[dataIdx] * maskData[j] / rate;
      }
    }
  }
  break;
  default:
    break;
  }
}
static int net_dropout(struct NetWork* net, int input, float rate)
{
  const char* type = "dropout";
  struct Layer* layer = addLayer(net, 1, 1, 0, DropoutLayer, type);
  FARG(rate) = rate;
  LIBLOB(0) = input;
  return LOBLOB(0) = addBlob(net, IBLOB(0)->size, type, "o");
}
static void FullconnectLayer(enum LayerOp op, struct NetWork* net, struct Layer* layer)
{
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  struct Blob* weight = WBLOB(0);
  struct Blob* bias = layer->w_size>1 ? WBLOB(1) : NULL;
  int num_output = IARG(num_output);
  switch (op) {
  case Op_Forward:
    next->size = dataSize(prev->size.n, num_output, 1, 1);
    {
      const float* prevData = getData(prev);
      const float* weightData = getData(weight);
      const float* biasData = bias ? getData(bias) : NULL;
      float* nextData = setDataZero(next);
      ASSERT(weight->size.h == Data_3DSize(next->size));
      ASSERT(weight->size.w == Data_3DSize(prev->size));
      DISPATCH_WORKER_BEGIN(prev->size.n) {
        fullconnect_forword(prevData + start * Data_3DSize(prev->size), weightData, biasData, nextData + start * Data_3DSize(next->size), stop - start, weight->size);
      };
      DISPATCH_WORKER_END;
    }
  break;
  case Op_Backward:
    {
      const float* prevData = getData(prev);
      const float* nextData = getData(next);
      const float* nextDiff = getDiff(next);
      const float* weightData = getData(weight);
      const float* biasData = bias ? getData(bias) : NULL;
      float* prevDiff = setDiffZero(prev);
      ASSERT(weight->size.h == Data_3DSize(next->size));
      ASSERT(weight->size.w == Data_3DSize(prev->size));
      easyAssert(next->size.w == 1 && next->size.h == 1, "use channel only!");
      easyAssert(Data_total(weight->size) == Data_3DSize(prev->size) * Data_3DSize(next->size), "weight size is invalidate!");
      //update prevDiff
      //calculate current inner diff && multiply next diff
      //update this layer's param
      fullconnect_backword(prevDiff, weightData, nextDiff, next->size.n, weight->size);
      //get weight gradient
      if (weight) {
        float* weightDiff = setDiffZero(weight);
        fullconnect_backword_weight(weightDiff, prevData, nextDiff, next->size.n, weight->size);
        //div by batch size
        div_inplace(weightDiff, (float)next->size.n, Data_total(weight->size));
      }
      //update bias
      if (bias) {
        float* biasDiff = setDiffZero(bias);
        easyAssert(Data_total(bias->size) == Data_3DSize(next->size), "bias size is invalidate!");
        fullconnect_backword_bias(biasDiff, nextDiff, next->size.n, Data_3DSize(bias->size));
        //div by batch size
        div_inplace(biasDiff, (float)next->size.n, Data_total(bias->size));
      }
    }
    break;
  default:
    break;
  }
}
int net_fc(struct NetWork* net, int input, int* w, int* b, int num_output)
{
  DataSize prevSize, weightSize;
  const char* type = "fc";
  int enabledBias = b != NULL;
  struct Layer* layer = addLayer(net, 1, 1, 1 + enabledBias, FullconnectLayer, type);
  IARG(num_output) = num_output;
  LIBLOB(0) = input;
  prevSize = IBLOB(0)->size;
  weightSize = dataSize(1, 1, num_output, Data_3DSize(prevSize));
  LWBLOB(0) = *w = addBlob(net, weightSize, type, "w");
  net_init_normal(net, *w, 0.0f, 0.1f);
  if (b) {
    LWBLOB(1) = *b = addBlob(net, dataSize(1, num_output, 1, 1), type, "b");
    net_init_const(net, *b, 0.f);
  }
  return LOBLOB(0) = addBlob(net, dataSize(prevSize.n, num_output, 1, 1), type, "o");
}
DataSize pool_calc_size(DataSize prevSize, int kernel_w, int kernel_h, int stride_w, int stride_h, enum Method paddingType)
{
  DataSize outputSize;
  easyAssert(prevSize.n && prevSize.w >= kernel_w && prevSize.h >= kernel_h, "poolingKernelSize parameters invalidate.");
  outputSize.n = prevSize.n;
  outputSize.c = prevSize.c;
  if (paddingType == M_VALID) {
    outputSize.w = (prevSize.w - kernel_w) / stride_w + 1;
    outputSize.h = (prevSize.h - kernel_h) / stride_h + 1;
  }
  else if (paddingType == M_SAME) {
    outputSize.w = (int)ceil((float)prevSize.w / (float)stride_w);
    outputSize.h = (int)ceil((float)prevSize.h / (float)stride_h);
  }
  return outputSize;
}
void PoolingLayer(enum LayerOp op, struct NetWork* net, struct Layer* layer)
{
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  int stride_w = IARG(stride_w);
  int stride_h = IARG(stride_h);
  int kernel_w = IARG(kernel_w);
  int kernel_h = IARG(kernel_h);
  enum Method poolingType = MARG(poolingType);
  enum Method paddingType = MARG(paddingType);
  int kernel_size = kernel_w * kernel_h;
  DataSize prevSize = prev->size;
  int nn, nc, nh, nw, ph, pw;
  switch (op) {
  case Op_Forward:
    next->size = pool_calc_size(prevSize, kernel_w, kernel_h, stride_w, stride_h, paddingType); {
    const float* prevData = getData(prev);
    float* nextData = setDataZero(next);
    float* maxIdxesData = (MNARG(phase) == M_Train && poolingType == M_Max) ? setBuff(next, 0) : NULL;
    for (nn = 0; nn < next->size.n; nn++) {
      for (nc = 0; nc < next->size.c; nc++) {
        for (nh = 0; nh < next->size.h; nh++) {
          for (nw = 0; nw < next->size.w; nw++) {
            const int inStartX = nw * stride_w;
            const int inStartY = nh * stride_h;
            const int nextDataIdx = Data_4DIndex(next->size, nn, nc, nh, nw);
            float result = 0;
            int maxIdx = 0;
            if (poolingType == M_Max) {
              for (ph = 0; ph < kernel_h; ph++) {
                for (pw = 0; pw < kernel_w; pw++) {
                  const int inY = inStartY + ph;
                  const int inX = inStartX + pw;
                  if (inY >= 0 && inY < prevSize.h && inX >= 0 && inX < prevSize.w) {
                    const int prevDataIdx = Data_4DIndex(prev->size, nn, nc, inY, inX);
                    if (result < prevData[prevDataIdx]) {
                      result = prevData[prevDataIdx];
                      maxIdx = ph * kernel_w + pw;
                    }
                  }
                }
              }
              if (maxIdxesData) {
                maxIdxesData[nextDataIdx] = (float)maxIdx;
              }
            }
            else if (poolingType == M_Mean) {
              for (ph = 0; ph < kernel_h; ph++) {
                for (pw = 0; pw < kernel_w; pw++) {
                  const int inY = inStartY + ph;
                  const int inX = inStartX + pw;
                  if (inY >= 0 && inY < prevSize.h && inX >= 0 && inX < prevSize.w) {
                    const int prevDataIdx = Data_3DIndex(prev->size, nc, inY, inX);
                    result += prevData[prevDataIdx];
                  }
                }
              }
              result /= kernel_w * kernel_h;
            }
            nextData[nextDataIdx] = result;
          }//ow
        }//oh
      }//oc
    }//on
    //showmat("prev", prev->data, prev->size);    showmat("next", next->data, next->size);    WaitKey(-1);
  }
  break;
  case Op_Backward: {
    //update prevDiff
    const float* maxIdxesData = (poolingType == M_Max) ? getBuff(next, 0) : NULL;
    float* prevDiff0 = setDiffZero(prev);
    //calculate current inner diff
    //none
    //pass next layer's diff to previous layer
    for (nn = 0; nn < next->size.n; nn++) {
      const float* nextDiff = getDiff(next) + nn * Data_3DSize(next->size);
      float* prevDiff = prevDiff0 + nn * Data_3DSize(prev->size);
      for (nc = 0; nc < next->size.c; nc++) {
        for (nh = 0; nh < next->size.h; nh++) {
          for (nw = 0; nw < next->size.w; nw++) {
            const int inStartX = nw * stride_w;
            const int inStartY = nh * stride_h;
            const int nextDataIdx = Data_3DIndex(next->size, nc, nh, nw);
            if (poolingType == M_Max) {
              for (ph = 0; ph < kernel_h; ph++) {
                for (pw = 0; pw < kernel_w; pw++) {
                  const int inY = inStartY + ph;
                  const int inX = inStartX + pw;
                  if (inY >= 0 && inY < prevSize.h && inX >= 0 && inX < prevSize.w) {
                    const int prevDiffIdx = Data_3DIndex(prev->size, nc, inY, inX);
                    if (ph * kernel_w + pw == maxIdxesData[nextDataIdx]) {
                      prevDiff[prevDiffIdx] += nextDiff[nextDataIdx];
                    }
                  }
                }
              }
            }
            else if (poolingType == M_Mean) {
              const float meanDiff = nextDiff[nextDataIdx] / (float)(kernel_size);
              for (ph = 0; ph < kernel_h; ph++) {
                for (pw = 0; pw < kernel_w; pw++) {
                  const int inY = inStartY + ph;
                  const int inX = inStartX + pw;
                  if (inY >= 0 && inY < prevSize.h && inX >= 0 && inX < prevSize.w) {
                    const int prevDiffIdx = Data_3DIndex(prev->size, nc, inY, inX);
                    prevDiff[prevDiffIdx] += meanDiff;
                  }
                }
              }
            }
          }
        }
      }
    }
    //update this layer's param
    //nop
  }
  break;
  default:
    break;
  }
}
int net_pooling(struct NetWork* net, int input, enum Method poolingType, int kernel_w, int kernel_h, int stride_w, int stride_h, enum Method paddingType)
{
  const char* type = "pool";
  struct Layer* layer = addLayer(net, 1, 1, 0, PoolingLayer, type);
  DataSize prevSize;
  easyAssert(kernel_w > 1 && kernel_h > 1 && stride_w > 0 && stride_h > 0, "parameters invalidate.");
  IARG(kernel_w) = kernel_w;
  IARG(kernel_h) = kernel_h;
  IARG(stride_w) = stride_w;
  IARG(stride_h) = stride_h;
  MARG(poolingType) = poolingType;
  MARG(paddingType) = paddingType;
  LIBLOB(0) = input;
  prevSize = IBLOB(0)->size;
  return LOBLOB(0) = addBlob(net, pool_calc_size(prevSize, kernel_w, kernel_h, stride_w, stride_h, paddingType), type, "o");
}
void SoftmaxLayer(enum LayerOp op, struct NetWork* net, struct Layer* layer)
{
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  int pn, nn, prevDataIdx, nextDiffIdx, prevDiffIdx;
  switch (op) {
  case Op_Forward:
    next->size = prev->size; {
    const float* prevData0 = getData(prev);
    float* nextData0 = setDataZero(next);
    for (nn = 0; nn < next->size.n; nn++) {
      const float* prevData = prevData0 + nn * Data_3DSize(prev->size);
      float* nextData = nextData0 + nn * Data_3DSize(next->size);
      //step1 : find max value
      float maxVal = prevData[0];
      float sum = 0;
      for (prevDataIdx = 0; prevDataIdx < Data_3DSize(prev->size); prevDataIdx++) {
        maxVal = MAX(maxVal, prevData[prevDataIdx]);
      }
      //step2 : sum
      for (prevDataIdx = 0; prevDataIdx < Data_3DSize(prev->size); prevDataIdx++) {
        nextData[prevDataIdx] = expf(prevData[prevDataIdx] - maxVal);
        sum += nextData[prevDataIdx];
      }
      //step3 : div
      for (prevDataIdx = 0; prevDataIdx < Data_3DSize(prev->size); prevDataIdx++) {
        nextData[prevDataIdx] = nextData[prevDataIdx] / sum;
      }
    }
  }
  break;
  case Op_Backward: {
    const float* prevData0 = getData(prev);
    const float* nextData0 = getData(next);
    const float* nextDiff0 = getDiff(next);
    float* prevDiff0 = setDiffZero(prev);
    easyAssert(Data_4DEqu(prev->size, next->size), "data size must be equal!");
    //update prevDiff
    for (pn = 0; pn < prev->size.n; pn++) {
      const float* prevData = prevData0 + pn * Data_3DSize(prev->size);
      const float* nextData = nextData0 + pn * Data_3DSize(next->size);
      const float* nextDiff = nextDiff0 + pn * Data_3DSize(next->size);
      float* prevDiff = prevDiff0 + pn * Data_3DSize(prev->size);
      for (prevDiffIdx = 0; prevDiffIdx < Data_3DSize(prev->size); prevDiffIdx++) {
        for (nextDiffIdx = 0; nextDiffIdx < Data_3DSize(next->size); nextDiffIdx++) {
          if (nextDiffIdx == prevDiffIdx) {
            prevDiff[prevDiffIdx] += nextData[prevDiffIdx] * (1.0f - nextData[prevDiffIdx]) * nextDiff[nextDiffIdx];
          }
          else {
            prevDiff[prevDiffIdx] -= nextData[prevDiffIdx] * nextData[nextDiffIdx] * nextDiff[nextDiffIdx];
          }
        }
      }
    }
    //update this layer's param
    //softmax layer : nop
  }
  break;
  default:
    break;
  }
}
#define net_softmax(net, input) net_null_1_1(net, input, SoftmaxLayer, "softmax")
int net_loss(struct NetWork* net, int input, int* label, LayerFun run)
{
  //struct Layer* layer = addLayer(net, 2, 1, 0, run, type);
  const char* type = "loss";
  struct Layer* layer = net->lossFunctor;
  initLayer(layer, 2, 1, 0, run, type);
  LIBLOB(0) = input;
  LIBLOB(1) = *label = addBlob(net, IBLOB(0)->size, type, "l");
  return LOBLOB(0) = addBlob(net, dataSize(1, 1, 1, 1), type, "o");
}
#define net_CrossEntropyLoss(net, input, label) net_loss(net, input, label, CrossEntropyFunctor, "CrossEntropy")
void CrossEntropyFunctor(enum LayerOp op, struct NetWork* net, struct Layer* layer)
{
  struct Blob* prev = IBLOB(0);
  struct Blob* label = IBLOB(1);
  struct Blob* next = OBLOB(0);
  int i, on, nextDiffIdx;
  switch (op) {
  case Op_Forward:
    next->size = dataSize(1, 1, 1, 1); {
    const float* labelData = getData(label);
    const float* prevData = getData(prev);
    float* nextData = setDataZero(next);
    float avgLoss = 0.0f;
    for (i = 0; i < Data_total(prev->size); i++) {
      const float curLoss = -labelData[i] * logf(prevData[i]);
      avgLoss = moving_average(avgLoss, i + 1, curLoss);
    }
    nextData[0] = avgLoss * Data_3DSize(prev->size);
  }
  break;
  case Op_Backward: {
    const DataSize labelSize = label->size;
    const DataSize diffSize = prev->size;
    float* prevDiff0 = setDiffZero(prev);
    for (on = 0; on < prev->size.n; on++) {
      const float* labelData = getData(label) + on * Data_3DSize(labelSize);
      const float* prevData = getData(prev) + on * Data_3DSize(prev->size);
      float* diffData = prevDiff0 + on * Data_3DSize(diffSize);
      for (nextDiffIdx = 0; nextDiffIdx < Data_3DSize(diffSize); nextDiffIdx++) {
        const int dataIdx = nextDiffIdx;
        diffData[nextDiffIdx] -= ((labelData[dataIdx] / (prevData[dataIdx])));
      }
    }
  }
  break;
  default:
    break;
  }
}
#define net_MSELoss(net, input, label) net_loss(net, input, label, MSEFunctor, "MSE")
void MSEFunctor(enum LayerOp op, struct NetWork* net, struct Layer* layer)
{
  struct Blob* prev = IBLOB(0);
  struct Blob* label = IBLOB(1);
  struct Blob* next = OBLOB(0);
  int i, on, nextDiffIdx;
  assert(Data_4DEqu(prev->size, label->size));
  switch (op) {
  case Op_Forward:
    next->size = dataSize(1, 1, 1, 1); {
    const float* labelData = getData(label);
    const float* prevData = getData(prev);
    float* nextData = setDataZero(next);
    float loss = 0.0f;
    for (i = 0; i < Data_total(prev->size); i++) {
      const float curLoss = (prevData[i] - labelData[i]) * (prevData[i] - labelData[i]);
      //loss = moving_average(loss, i+1, curLoss);
      loss += curLoss;
    }
    loss /= Data_total(prev->size);
    nextData[0] = loss * Data_3DSize(prev->size);
  }
  break;
  case Op_Backward: {
    const DataSize labelSize = label->size;
    const DataSize diffSize = prev->size;
    float* prevDiff0 = setDiffZero(prev);
    for (on = 0; on < prev->size.n; on++) {
      const float* labelData = getData(label) + on * Data_3DSize(labelSize);
      const float* prevData = getData(prev) + on * Data_3DSize(prev->size);
      float* prevDiff = prevDiff0 + on * Data_3DSize(diffSize);
      for (nextDiffIdx = 0; nextDiffIdx < Data_3DSize(diffSize); nextDiffIdx++) {
        const int dataIdx = nextDiffIdx;
        prevDiff[nextDiffIdx] += 2.0f * (prevData[dataIdx] - labelData[dataIdx]);
      }
    }
  }
  break;
  default:
    break;
  }
}
//SGD
//w -= lr*g
void net_updateSGD(struct NetWork* net)
{
  int i, j, k;
  float lr = FNARG(lr);
  for (k = 0; k < net->l_size; ++k) {
    struct Layer* layer = net->layers + k;
    for (i = 0; i < layer->w_size; i++) {
      struct Blob* param = WBLOB(i);
      const float* paramDiff = getDiff(param);
      float* paramData = regetData(param);
      for (j = 0; j < Data_total(param->size); j++) {
        paramData[j] -= lr * paramDiff[j];
      }
    }
  }
}
//SGDWithMomentum
//prev_m = momentum*prev_m+g[t]
//w -= lr*prev_m
void net_updateSGDWithMomentum(struct NetWork* net)
{
  int i, j, k;
  float lr = FNARG(lr);
  float momentum = FNARG(momentum);
  for (k = 0; k < net->l_size; ++k) {
    struct Layer* layer = net->layers + k;
    for (i = 0; i < layer->w_size; i++) {
      struct Blob* param = WBLOB(i);
      float* paramData = regetData(param);
      const float* paramDiff = param->diff;
      float* prevMData = param->bufs[0];
      for (j = 0; j < Data_total(param->size); j++) {
        prevMData[j] = momentum * prevMData[j] + paramDiff[j];
        paramData[j] -= lr * prevMData[j];
      }
    }
  }
}
int net_forward(struct NetWork* net)
{
  int i;
  struct Layer* layer = net->layers;
  logVerbose("struct NetWork forward begin.");
  easyAssert(net->l_size > 1, "layer count is less than 2.");
  easyAssert(net->b_size > 0, "data blobs is not ready.");
  for (i = 0; i < net->l_size; i++) {
    layer = net->layers + i;
    logVerbose("struct NetWork layer[%d](%s) forward begin.", i, layer->name);
    //for (j = 0; j < layer->o_size; ++j) { setDataZero(OBLOB(j)->data); }
    //{utime_start(_start_time);
    Layer_forward(layer);
    //printf("%f %s\n", utime_elapsed(_start_time), layer->name);}
    logVerbose("struct NetWork layer[%d](%s) forward end.", i, layer->name);
  }
  logVerbose("struct NetWork forward end.");
  return LOBLOB(0);
}
float net_getLoss(struct NetWork* net)
{
  float loss = 0;
  struct Layer* layer = net->layers;
  layer = net->lossFunctor;
  Layer_forward(net->lossFunctor);
  assert(Data_4DEqu(OBLOB(0)->size, dataSize(1, 1, 1, 1)));
  loss = OBLOB(0)->data[0];
  return loss;
}
float net_backward(struct NetWork* net)
{
  int i;
  float loss = 0;
  struct Layer* layer = net->layers;
  easyAssert(MNARG(phase) == M_Train, "phase must be train!");
  logVerbose("struct NetWork backward begin.");
  easyAssert(net->l_size > 1, "layer count is less than 2.");
  easyAssert(net->lossFunctor->run != NULL, "loss functor can't be empty!");
  //get loss
  loss = net_getLoss(net);
  layer = net->lossFunctor;
  Layer_backward(net->lossFunctor);
  //other layer backward
  for (i = (int)(net->l_size) - 1; i >= 0; i--) {
    layer = net->layers + i;
    logVerbose("struct NetWork layer[%d](%s) backward begin.", i, layer->name);
    //for (j = 0; j < layer->o_size; ++j) { setDiffZero(IBLOB(j)); }
    //{utime_start(_start_time);
    Layer_backward(layer);
    //printf("%f %s\n", utime_elapsed(_start_time), layer->name);}
    logVerbose("struct NetWork layer[%d](%s) backward end.", i, layer->name);
  }
  //update parameters
  logVerbose("struct NetWork backward end.");
  return loss;
}
//train net->phase may use this
int net_testBatch(struct NetWork* net)
{
  MNARG(phase) = M_Test;
  return net_forward(net);
}
float net_forward_backward(struct NetWork* net)
{
  float loss;
  MNARG(phase) = M_Train;
  //tic;
  net_forward(net);
  //toc;
  //tic;
  loss = net_backward(net);
  //toc;
  //tictoc;
  return loss;
}
static int getMaxIdxInArray(const float* start, const int len)
{
  int i, result = 0;
  assert(len > 1);
  for (i = 1; i < len; i++) {
    if (start[i] > start[result]) {
      result = i;
    }
  }
  return result;
}

static float getAccuracy(const struct Blob* probBlob, const struct Blob* labelBlob)
{
  const DataSize probSize = probBlob->size;
  const DataSize labelSize = labelBlob->size;
  const int itemSize = Data_3DSize(labelSize);
  const float* probData = getData(probBlob);
  const float* labelData = getData(labelBlob);
  int correctCount = 0;
  int totalCount = 0;
  int n;
  float result;
  assert(Data_4DEqu(probSize, labelSize));
  for (n = 0; n < probSize.n; n++) {
    const int stdProb = getMaxIdxInArray(labelData + n * itemSize, itemSize);
    const int testProb = getMaxIdxInArray(probData + n * itemSize, itemSize);
    if (stdProb == testProb) {
      correctCount++;
    }
    totalCount++;
  }
  result = (float)correctCount / (float)totalCount;
  return result;
}

enum IFArgType {IF_Int, IF_Float, IF_Method};
const static enum IFArgType ifArgType[] = {
#define LAYERARGDEF(type, name)     IF_ ## type, 
LAYERARGTYPE_DEFDEF(LAYERARGDEF)
#undef LAYERARGDEF
};
static const char* layerarg_name[] = {
#define LAYERARGDEF(type, name)     #name , 
  LAYERARGTYPE_DEFDEF(LAYERARGDEF)
#undef LAYERARGDEF
};

static const char* method_name[] = {
#define LAYERARGDEF(type, name)     #name , 
  METHOD_DEFDEF(LAYERARGDEF)
#undef LAYERARGDEF
};
static int Layer_weightSize(const struct NetWork* net, const struct Layer* layer) {
  int i, size=0;
  for (i=0; i<layer->w_size; ++i) {
    struct Blob* blob = WBLOB(i);
    size += Data_total(blob->size);
  }
  return size;
}
static int net_print_debug(const struct NetWork* net) {
  int i, j;
  printf("%d %d\n", net->b_size, net->l_size);
  for (i=0; i<net->b_size; ++i) {
    struct Blob* blob = net->blobs+i;
    printf("%2d %16s %3d %3d %3d %3d\n", i, blob->name, blob->size.n, blob->size.c, blob->size.h, blob->size.w);
  }
  for (i=0; i<net->l_size; ++i) {
    struct Layer* layer = net->layers+i;
    int size = layer->i_size+ layer->o_size+ layer->w_size;
    printf("%8s %2d %2d %2d", layer->name, layer->i_size, layer->o_size, layer->w_size);
    for (j=0; j<size; ++j) {
      printf(" %2d", layer->id[j]);
    }
    for (j=0; j<LayerArgCount; ++j) {
      enum LayerArgId arg = (enum LayerArgId)j;
      if (layer->x[arg].i>0) {
        switch (ifArgType[arg]) {
        case IF_Int:
          printf(" %s=%d", layerarg_name[arg], layer->x[arg].i);
          break;
        case IF_Float:
          printf(" %s=%f", layerarg_name[arg], layer->x[arg].f);
          break;
        case IF_Method:
          printf(" %s=%s", layerarg_name[arg], method_name[layer->x[arg].m]);
          break;
        default:
          ASSERT(0);
          break;
        }
      }
    }
    printf("\n");
  }
  return 0;
}
static int net_print(const struct NetWork* net, FILE* pf, int bSaveParam) {
  int i, j;
  int *blob_type = NULL;
  const char* linebeg = bSaveParam ? "//": "";
  const char* lineend = "\n";
  MYREALLOC(blob_type, net->b_size);
  MEMSET(blob_type, 0, net->b_size);
  fprintf(pf, "%s%d %d%s", linebeg, net->l_size, net->b_size, lineend);
  for (i=0; i<net->l_size; ++i) {
    struct Layer* layer = net->layers+i;
    int size = layer->i_size+ layer->o_size+ layer->w_size;
    //int datasize = Layer_weightSize(net, layer);
    //printf("%8s %8d", layer->type, datasize);
    //printf("%8s %2d %2d %2d", layer->name, layer->i_size, layer->o_size, layer->w_size);
    fprintf(pf, "%s%8s %d %d %d", linebeg, layer->type, layer->i_size, layer->o_size, layer->w_size);
    for (j=0; j<layer->w_size; ++j) {
      blob_type[LWBLOB(j)] = 1;
    }
    for (j=0; j<size; ++j) {
      fprintf(pf, " %s", net->blobs[layer->id[j]].name);
    }
    for (j=0; j<LayerArgCount; ++j) {
      enum LayerArgId arg = (enum LayerArgId)j;
      if (layer->x[arg].i>0) {
        if ((arg==LA_stride_w || arg==LA_stride_h || arg==LA_dilation) && layer->x[arg].i<=1) {
          continue;
        }
        switch (ifArgType[arg]) {
        case IF_Int:
          fprintf(pf, " %s=%d", layerarg_name[arg], layer->x[arg].i);
          break;
        case IF_Float:
          fprintf(pf, " %s=%f", layerarg_name[arg], layer->x[arg].f);
          break;
        case IF_Method:
          fprintf(pf, " %s=%s", layerarg_name[arg], method_name[layer->x[arg].m]);
          break;
        default:
          ASSERT(0);
          break;
        }
      }
    }
    fprintf(pf, lineend);
  }
  for (i=0; i<net->b_size; ++i) {
    if (blob_type[i]) {
      DataSize size = net->blobs[i].size;
      fprintf(pf, "%s%16s %3d %3d %3d %3d%s", linebeg, net->blobs[i].name, size.n, size.c, size.h, size.w, lineend);
    }
  }
  if (bSaveParam) {
    for (i=0; i<net->b_size; ++i) {
      if (blob_type[i]) {
        DataSize size = net->blobs[i].size;
        int datasize = Data_total(size);
        if (0) {
          fwrite(net->blobs[i].data, datasize, sizeof(float), pf);
        } else {
          //fprintf(pf, "//%s %d %d %d %d\n", net->blobs[i].name, size.n, size.c, size.h, size.w);
          for (j=0; j<datasize; ++j) {
            fprintf(pf, "%g,\n", net->blobs[i].data[j]);
          }
        }
      }
    }
  }
  FREE(blob_type);
  return 0;
}
static int strlist_find(const char* name, int len, const char** argname, int argnamelen) {
  int i;
  for (i=0; i<argnamelen; ++i) {
    int argnamelen = strlen(argname[i]);
    if (argnamelen==len && name[0]==argname[i][0] && 0==strncmp(name, argname[i], len)) {
      return i;
    }
  }
  return argnamelen;
}
static enum Method getMethod(const char* name, int len) {
  return (enum Method)strlist_find(name, len, method_name, countof(method_name));
}
static enum LayerArgId getLayerArg(const char* name, int len) {
  return (enum LayerArgId)strlist_find(name, len, layerarg_name, countof(layerarg_name));
}
static int setLayerArg(union LayerArg* x, str_t s_key, str_t s_value) {
  enum LayerArgId arg = getLayerArg(s_key.s, s_key.l);
  bool ret = 0;
  if (arg<LayerArgCount) {
    ret = 1;
    switch (ifArgType[arg]) {
    case IF_Int:
      x[arg].i = atoi(s_value.s);
      break;
    case IF_Float:
      x[arg].f = atof(s_value.s);
      break;
    case IF_Method:
      x[arg].m = getMethod(s_value.s, s_value.l);
      break;
    default:
      {
        char buf[256];
        strncpy(buf, s_key.s, MIN(256, s_key.l));
        printf("setLayerArg err: %s\n", buf);
        ret = 0;
      }
      break;
    }
  }
  return ret;
}

#define LAYER_DEFDEF(DEF) \
LAYER_REGISTRY(input, InputLayer)  \
LAYER_REGISTRY(conv, ConvolutionLayer)  \
LAYER_REGISTRY(fc, FullconnectLayer)  \
LAYER_REGISTRY(dropout, DropoutLayer)  \
LAYER_REGISTRY(pool, PoolingLayer)  \
LAYER_REGISTRY(relu, ReluLayer)  \
LAYER_REGISTRY(softmax, SoftmaxLayer)

static int layer_init(struct Layer* layer, const char* type)
{
  static char* layer_type[] = {
#define LAYER_REGISTRY(TYPE, RUN)  #TYPE,
    LAYER_DEFDEF(LAYER_REGISTRY)
#undef LAYER_REGISTRY
  };
  static LayerFun layer_run[] = {
#define LAYER_REGISTRY(TYPE, RUN)  RUN,
    LAYER_DEFDEF(LAYER_REGISTRY)
#undef LAYER_REGISTRY
  };
  int i;
  enum { layer_registry_entry_count = countof(layer_type) };

  for (i = 0; i < layer_registry_entry_count; i++) {
    const char* layer_name = layer_type[i];
    if (type[0]==layer_name[0] && stricmp(type, layer_type[i]) == 0) {
      layer->type = layer_type[i];
      layer->run = layer_run[i];
      //layer->name = layer_name[i];
      return 1;
    }
  }

  return 0;
}

#define SKIPSLASH   for(; '/'==line.s[p] || '\\'==line.s[p]; ++p)
static int net_load_from_string(struct NetWork* net, const char* str, int len)
{
  int layer_cnt = 0;
  int blob_cnt = 0;
  str_t s = STR2(str, len), line;
  int i, j=0, nscan=0, p=0;
  int l_size = 0, b_size = 0;
  char buf[256] = {0};
  int count;
  
  // parse
  cstr_getline(s, &j, &line);
  SKIPSLASH;
  nscan = cstr_getint(line, &p, &l_size, 0) &&
    cstr_getint(line, &p, &b_size, 0);
  if (!nscan) {
    printf("file format error in line %d\n", j);
    return 0;
  }
  
  MYREALLOC(net->layers, l_size);
  MEMZERO(net->layers, l_size);
  //net->b_size = net->l_size;
  MYREALLOC(net->blobs, b_size);
  MEMZERO(net->blobs, b_size);
  
  for (; layer_cnt<l_size && cstr_getline(s, &j, &line); layer_cnt++) {
    struct Layer *layer = net->layers + layer_cnt;
    int size = 0;
    str_t s_key, s_value;
    p=0;
    SKIPSLASH;
    nscan = cstr_getstr(line, &p, NULL, buf, countof(buf), NULL)
       && cstr_getint(line, &p, &layer->i_size, 0)
       && cstr_getint(line, &p, &layer->o_size, 0)
       && cstr_getint(line, &p, &layer->w_size, 0)
       ;
    
    if (!nscan) {
      printf("file format error in line %d\n", j);
      continue;
    }
    
    if (!layer_init(layer, buf)) {
      printf("layer_init error in line %d\n", j);
      continue;
    }
    size = layer->i_size + layer->o_size + layer->w_size;
    
    ASSERT(size <= MAXIDS);
    for (i = 0; i < size; i++) {
      cstr_getstr(line, &p, NULL, buf, countof(buf), NULL);
      layer->id[i] = find_or_insert_blob(net, buf, 1);
      ASSERT(layer->id[i]>=0);
    }
    
    while (cstr_getkv(line, &p, &s_key, &s_value)) {
      setLayerArg(layer->x, s_key, s_value);
    }
  }
  ASSERT(l_size==layer_cnt);
  net->l_size = l_size;
  count = 0;
  for (; cstr_getline(s, &j, &line) && line.s[0]=='/'; ) {
    int id = 0;
    DataSize size = {0};
    p=0;
    SKIPSLASH;
    nscan = cstr_getstr(line, &p, NULL, buf, countof(buf), NULL)
       && cstr_getint(line, &p, &size.n, 0)
       && cstr_getint(line, &p, &size.c, 0)
       && cstr_getint(line, &p, &size.h, 0)
       && cstr_getint(line, &p, &size.w, 0)
       ;
    if (!nscan) {
      printf("blob init error in line %d\n", j);
      continue;
    }
    id = find_or_insert_blob(net, buf, 0);
    if (id<0) {
      printf("can not find blob in line %d\n", j);
      continue;
    }
    count += Data_total(size);
    net->blobs[id].size = size;
  }
  return 1;
}
#include "cfile.h"
static int net_save(const struct NetWork* net, const char* model_file, const char* param_file, int epochIdx) {
  FILE* pf;
  if (model_file) {
    pf = fopen(model_file, "wb");
    if (pf) {
      net_print(net, pf, 1);
      fclose(pf);
    }
  }
  if (param_file) {
    char buf[256];
    if (epochIdx>=0) {
      _snprintf(buf, 256, "%s_%d.dat", param_file, epochIdx);
      param_file = buf;
    }
    pf = fopen(buf, "wb");
    if (pf) {
      int i, j;
      int *blob_type = NULL;
      MYREALLOC(blob_type, net->b_size);
      MEMSET(blob_type, 0, net->b_size);
      for (i=0; i<net->l_size; ++i) {
        struct Layer* layer = net->layers+i;
        for (j=0; j<layer->w_size; ++j) {
          blob_type[LWBLOB(j)] = 1;
        }
      }
      j=0;
      for (i=0; i<net->b_size; ++i) {
        if (blob_type[i]) {
          const struct Blob* blob = net->blobs + i; 
          if (blob->data) {
            DataSize size = blob->size;
            int datasize = Data_total(size);
            fwrite(blob->data, datasize, sizeof(float), pf);
          } else {
            printf("error : blob data is NULL\n");
          }
        }
      }
      FREE(blob_type);
      fclose(pf);
    }
  }
  return 0;
}
static int net_set_param(struct NetWork* net, const float* param, int len) {
  int i, j, ret = 1;
  int *blob_type = NULL;
  MYREALLOC(blob_type, net->b_size);
  MEMSET(blob_type, 0, net->b_size);
  for (i=0; i<net->l_size; ++i) {
    struct Layer* layer = net->layers+i;
    for (j=0; j<layer->w_size; ++j) {
      blob_type[LWBLOB(j)] = 1;
    }
  }
  j=0;
  for (i=0; i<net->b_size; ++i) {
    if (blob_type[i]) {
      struct Blob* blob = net->blobs + i; 
      DataSize size = blob->size;
      int datasize = Data_total(size);
      reset(&blob->data, size);
      if ((j+datasize) <= len) {
        memcpy(blob->data, param+j, sizeof(float)*datasize);
      } else {
        ret = 0;
        printf("net_set_param param len error\n");
        break;
      }
      j += datasize;
    }
  }
  FREE(blob_type);
  return ret;
}
static int net_load(struct NetWork* net, const char* model_file, const char* param_file) {
  int ret = 0;
  if (model_file) {
    buf_t bf[1] = {0};
    if (buf_load(model_file, bf)) {
      ret = net_load_from_string(net, (char*)bf->data, bf->len);
    }
    bffree(bf);
  }
  if (ret && param_file) {
    buf_t bf[1] = {0};
    if (buf_load(param_file, bf)) {
      ret = net_set_param(net, (float*)bf->data, bf->len/sizeof(float));
    }
    bffree(bf);
  }
  return ret;
}

typedef int (*fetch_data_t)(void* s, struct Blob* inputBlob, struct Blob* labelBlob, int start, int shuffle_len, const int* shuffle);

struct net_train_test_t {
  int batch;
  int max_epoch;
  int maxBatches;
  int testAfterBatches;
  fetch_data_t fetch_data;
  void* s;
  int train_start;
  int train_end;
  int test_start;
  int test_end;
  float learningRate;
  float decayRate;
  float minLearningRate;
};

static void net_train_test_init(struct net_train_test_t* t, void* s, fetch_data_t fetch_data, int nsamps, float train_rate, int batch) {
  t->train_start = 0;
  t->train_end = (int)(nsamps * train_rate);
  t->test_start = t->train_end;
  t->test_end = nsamps;
  t->fetch_data = fetch_data;
  t->s = s;

  t->learningRate = 0.1f;
  t->decayRate = 0.8f;
  t->minLearningRate = 0.001f;
  t->testAfterBatches = 10;
  t->maxBatches = 10000;
  t->max_epoch = 5;
  t->batch = batch;

  {
    int train_number = t->train_end - t->train_start;
  int validate_number = t->test_end - t->test_start;
  logCritical("load training data done. train set's size is %d,validate set's size is %d", train_number, validate_number);
  logCritical("max_epoch:%d,testAfterBatches:%d", t->max_epoch, t->testAfterBatches);
  logCritical("learningRate:%f ,decayRate:%f , minLearningRate:%f", t->learningRate, t->decayRate, t->minLearningRate);
  }
  return ;
}

struct test_result_t {
  float accuracy;
  float loss;
};

static struct test_result_t net_test(const struct net_train_test_t* t, struct NetWork* net, int input, int output, int label, const int* shuffle)
{
  int start = t->test_start;
  int end = t->test_end;
  int correctCount = 0, batchs = 0, i, j;
  float loss = 0.0f, batch_loss;
  struct test_result_t ret;
  struct Blob* inputBlob = BLOB(input);
  struct Blob* outputBlob = BLOB(output);
  struct Blob* labelBlob = BLOB(label);
  void* s = t->s;
  for (i = start; i < end; i += t->batch, batchs++) {
    int len = MIN(end - i, t->batch);
    t->fetch_data(s, inputBlob, labelBlob, i, len, shuffle);
    net_testBatch(net);
    //get loss
    batch_loss = net_getLoss(net);
    loss = moving_average(loss, batchs + 1, batch_loss);
    {
      const float* outputData = getData(outputBlob);
      const float* labelData = getData(labelBlob);
      int labelSize = Data_3DSize(outputBlob->size);
      //assert(labelSize == s->classes);
      for (j = 0; j < len; j++) {
        int labelId = getMaxIdxInArray(labelData + j * labelSize, labelSize);
        int outputId = getMaxIdxInArray(outputData + j * labelSize, labelSize);
        if (labelId == outputId) {
          correctCount++;
        }
      }
    }
  }
  ret.accuracy = (float)correctCount / (float)(end - start);
  ret.loss = loss;
  return ret;
}
static struct test_result_t net_train(const struct net_train_test_t* t, struct NetWork* net, int input, int output, int label, const char* param_file) {
  float train_loss = 0.0f;
  int train_batches = 0;
  int start = t->train_start;
  int end = t->train_end;
  struct test_result_t val;
  //train
  struct Blob* inputBlob = BLOB(input);
  struct Blob* labelBlob = BLOB(label);
  int epochIdx = 0;
  int* shuffle = NULL;
  int train_number = end - start;
  float learningRate = t->learningRate;
  logCritical("begin training...");
  logCritical("%13s|%7s|%10s|%10s|%10s", "sample", "lr", "train_loss", "val_loss", "val_accuracy");
  for (; epochIdx < t->max_epoch; ++epochIdx) {
    //before epoch start, shuffle all train data first
    int batchIdx = 0;
    get_shuffle(train_number, &shuffle);
    for (; batchIdx < t->maxBatches; ++batchIdx) {
      int i = start + batchIdx * t->batch;
      int len = MIN(end - i, t->batch);
      float batch_loss;
      if (!t->fetch_data(t->s, inputBlob, labelBlob, i, len, shuffle)) {
        break;
      }
      FNARG(lr) = learningRate;
      batch_loss = net_forward_backward(net);
      net_updateSGD(net);
      //net_save(net, NULL, param_file, epochIdx);
      //printf("batch_loss : %f\n", batch_loss); Optimizer_update
      train_loss = moving_average(train_loss, train_batches + 1, batch_loss);
      train_batches++;
      if (batchIdx > 0 && (batchIdx % t->testAfterBatches) == 0)
      {
        val = net_test(t, net, input, output, label, NULL);
        logCritical("%6d/%-6d|%7.4f|%10.4f|%10.4f|%10.4f%%",
            i, train_number, t->learningRate, train_loss, val.loss, val.accuracy * 100.0f);
        train_loss = 0.0f;
        train_batches = 0;
      }
    }
    net_save(net, NULL, param_file, epochIdx);
    if (batchIdx >= t->maxBatches) {
      break;
    }
    val = net_test(t, net, input, output, label, NULL);
    //update learning rate
    learningRate = MAX(learningRate * t->decayRate, t->minLearningRate);
    logCritical("epoch[%d] val_loss : %f , val_accuracy : %.4f%%", epochIdx, val.loss, val.accuracy * 100.0f);
  }
  net_save(net, NULL, param_file, -1);
  val = net_test(t, net, input, output, label, NULL);
  logCritical("final val_loss : %f , final val_accuracy : %.4f%%", val.loss, val.accuracy * 100.0f);
  // success = net.saveModel(modelFilePath);
  logCritical("finished training.");
  FREE(shuffle);
  return val;
}