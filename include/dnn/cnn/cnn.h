
#ifndef _CNN_INL_
#define _CNN_INL_
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef __ANDROID__
#include <android/log.h>
#endif //__ANDROID__ 
#ifdef _MSC_VER
#pragma warning(disable:4996)
#endif
#include "mathfun.h"

enum LogLevel {
  EASYCNN_LOG_LEVEL_VERBOSE,
  EASYCNN_LOG_LEVEL_CRITICAL,
  EASYCNN_LOG_LEVEL_FATAL,
  EASYCNN_LOG_LEVEL_NONE
};
#if MSC_VER<1300
#ifndef vsnprintf
#define vsnprintf _vsnprintf
#endif // vsnprintf
#endif
//default : console
#if 0
static char* formatString(const char* fmt, va_list args)
{
  char* content;
  const int size = vsnprintf(NULL, 0, fmt, args);
  if (size > 0) {
    content.resize(size);
    vsprintf(const_cast<char*>(content.data()), fmt, args);
  }
  return content;
}
#endif
static const char* level2str(enum LogLevel level)
{
  switch (level) {
  case EASYCNN_LOG_LEVEL_VERBOSE:
    return "verbose";
  case EASYCNN_LOG_LEVEL_CRITICAL:
    return "critical";
  case EASYCNN_LOG_LEVEL_FATAL:
    return "fatal";
  default:
    break;
  }
  return "unknown";
}
static char* buildInnerContent(char* buf, int len, enum LogLevel level, const char* content)
{
  const time_t t = time(NULL);
  const struct tm* local = localtime(&t);
  //_snprintf(buf, len, "[%04d/%02d/%02d %02d:%02d:%02d][%s]%s\n", local->tm_year + 1990, local->tm_mon + 1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec, level2str(level), content);
  _snprintf(buf, len, "[%02d:%02d:%02d]%s\n", local->tm_hour, local->tm_min, local->tm_sec, content);
  return buf;
}
static const char* g_logfile = NULL;
static void defaultLogRoute(enum LogLevel level, const char* content)
{
  char buf[1024];
  const char* innerContent = buildInnerContent(buf, 1024, level, content);
#ifdef __ANDROID__
  __android_log_print(ANDROID_LOG_INFO, "digit", "log : %s", innerContent);
#else
  printf(innerContent);
  if (g_logfile) {
    FILE* pf = fopen(g_logfile, "a");
    if (pf) {
      fprintf(pf, "%s", innerContent);
      fclose(pf);
    }
  }
#endif //__ANDROID__
}
//////////////////////////////////////////////////////////////////////////
static enum LogLevel globalLogLevel = EASYCNN_LOG_LEVEL_VERBOSE;
typedef void(*LogCb_t)(enum LogLevel, const char* content);
static LogCb_t globalLogCb = NULL;//defaultLogRoute;
//log level setting
static void setLogLevel(enum LogLevel level)
{
  globalLogLevel = level;
}
static enum LogLevel getLogLevel()
{
  return globalLogLevel;
}
//log route setting
static void setLogRedirect(LogCb_t logCb)
{
  globalLogCb = logCb;
}
//log function
static void logVerbose(const char* fmt, ...)
{
  if (globalLogCb) {
    enum LogLevel level = EASYCNN_LOG_LEVEL_VERBOSE;
    char buf[1024];
    va_list args;
    if (globalLogLevel > level) {
      return;
    }
    va_start(args, fmt);
    vsnprintf(buf, 1024, fmt, args);
    va_end(args);
    globalLogCb(level, buf);
  }
}
static void logCritical(const char* fmt, ...)
{
  if (globalLogCb){
    enum LogLevel level = EASYCNN_LOG_LEVEL_CRITICAL;
    char buf[1024];
    va_list args;
    if (globalLogLevel > level) {
      return;
    }
    va_start(args, fmt);
    vsnprintf(buf, 1024, fmt, args);
    va_end(args);
    globalLogCb(level, buf);
  }
}
static void logFatal(const char* fmt, ...)
{
  if (globalLogCb) {
    enum LogLevel level = EASYCNN_LOG_LEVEL_FATAL;
    char buf[1024];
    va_list args;
    if (globalLogLevel > level) {
      return;
    }
    va_start(args, fmt);
    vsnprintf(buf, 1024, fmt, args);
    va_end(args);
    globalLogCb(level, buf);
  }
}
#include <assert.h>
static char const * __FILE = NULL;
static char const * __FUNCTION = NULL;
static int __LINE = 0;
#if 0
#define easyAssert (__FILE = __FILE__, __FUNCTION = __FUNCTION__, __LINE = __LINE__),easyAssertCore
#else
#define easyAssert (__FILE = __FILE__, __FUNCTION = "", __LINE = __LINE__),easyAssertCore
#endif
static void* globalAssertFatalUserData = NULL;
static void(*globalAssertFatalCB)(void* userData, const char* errorStr) = NULL;
static void setAssertFatalCallback(void(*cb)(void* userData, const char* errorStr), void* userData)
{
  globalAssertFatalCB = cb;
  globalAssertFatalUserData = userData;
}
static int easyAssertCore(int condition, const char* fmt, ...)
{
  const char* file = __FILE;
  const char* function = __FUNCTION;
  long line = __LINE;
  if (!condition) {
    char errorStr[256];
    va_list args;
    va_start(args, fmt);
    vsnprintf(errorStr, 256, fmt, args);
    logFatal("FILE:%s,FUNCTION:%s,LINE:%d", file, function, line);
    logFatal(fmt, args);
    va_end(args);
    if (globalAssertFatalCB) {
      globalAssertFatalCB(globalAssertFatalUserData, errorStr);
    }
    {
      assert(0);
      exit(0);
    }
  }
  return 0;
}

#define kMaxBlobAxes    MAX_DIM
#define MAX_BLOB_BUFF_NUM    16

#define MAXNAME 64
struct Blob {
  char name[MAXNAME];
  union {
    DataSize size;
    struct {int n, c, h, w;};
    int dim[4];
  };
#if 1
  float* data;
  float* diff;
  float* bufs[MAX_BLOB_BUFF_NUM];
#else
  udata_t data[1];
  udata_t diff[1];
  udata_t bufs[MAX_BLOB_BUFF_NUM];
#endif
};
static int Blob_dims(const Blob* blob) {
  int dims = blob->size.n>1 ? 4 : blob->size.c>1 ? 3 : blob->size.h>1 ? 2 : 1;
  return dims;
}
static int Blob_count(const Blob* blob) {
  return Data_4DSize(blob->size);
}
static int Blob_count_range(const Blob* blob, int start, int end) {
  start = BOUND(start, 0, end);
  end = BOUND(end, start, 4);
  int i=start, count = 1;
  for (; i<end; ++i) {
    count *= blob->dim[i];
  }
  return count;
}
float* Blob_data_at(const Blob* blob, int n, int c, int h, int w) {
  int k = Data_4DIndex(blob->size, n, c, h, w);
  return ((float*)blob->data) + k;
}
float* Blob_diff_at(const Blob* blob, int n, int c, int h, int w) {
  int k = Data_4DIndex(blob->size, n, c, h, w);
  return (float*)blob->diff+k;
}
static int Blob_channels(const struct Blob* const blob) {return blob->c;}
static const float* getData(const struct Blob* const blob)
{
  return (float*)blob->data;
}
static const float* getDiff(const struct Blob* const blob)
{
  return (float*)blob->diff;
}
static const float* getBuff(const struct Blob* const blob, int i)
{
  return (float*)blob->bufs[i];
}
static float* setDataSize(struct Blob* blob, DataSize size)
{
  return reset(&blob->data, blob->size = size);
}
static float* setData(struct Blob* blob)
{
  return reset(&blob->data, blob->size);
}
static float* setDiff(struct Blob* blob)
{
  return reset(&blob->diff, blob->size);
}
static float* setBuff(struct Blob* blob, int i)
{
  return reset(&blob->bufs[i], blob->size);
}
static float* setBuffSize(struct Blob* blob, int i, DataSize size)
{
  return reset(&blob->bufs[i], blob->size = size);
}
static float* regetData(struct Blob* blob)
{
  return blob->data;
}
static float* regetDiff(struct Blob* blob)
{
  return blob->diff;
}
static float* regetBuff(struct Blob* blob, int i)
{
  return blob->bufs[i];
}
static float* setDataZero(struct Blob* blob)
{
  return resetZero(&blob->data, blob->size);
}
static float* setDiffZero(struct Blob* blob)
{
  return resetZero(&blob->diff, blob->size);
}
static float* setBuffZero(struct Blob* blob, int i)
{
  return resetZero(&blob->bufs[i], blob->size);
}
static float* resetData(struct Blob* blob)
{
  return blob->data ? blob->data : setDataZero(blob);
}
static float* resetDiff(struct Blob* blob)
{
  return blob->diff ? blob->diff : setDiffZero(blob);
}
static float* resetBuff(struct Blob* blob, int i)
{
  return blob->bufs[i] ? blob->bufs[i] : setBuffZero(blob, i);
}
CC_INLINE void Blob_free(struct Blob* blob)
{
  int i;
  free(blob->data);
  free(blob->diff);
  for (i=0; i<MAX_BLOB_BUFF_NUM; ++i) {
    free(blob->bufs[i]);
  }
  memset(blob, 0, sizeof(struct Blob));
}
#define Blob_frees2(x) Blob_frees(x, countof(x))
CC_INLINE void Blob_frees(struct Blob* blob, int n)
{
  for (; --n>=0; ) {
    Blob_free(blob+n);
  }
}
static void Blob_Update(struct Blob* blob) {
  blas_saxpy(Data_total(blob->size), (float)(-1), blob->diff, blob->data);
}

typedef struct Blob pBox;

#define SETMODIFYP(a, b)

struct udata_t;

typedef struct umem_t {
  struct udata_t* u;
  bool modifyp, modifyu;
  int datasize;
  Dtype* p;
} umem_t;

static void umem_setsize(umem_t* a, int datasize)
{
  if (a->datasize < datasize) {
    a->p = (Dtype*)realloc(a->p, datasize);
    a->datasize = datasize;

#ifdef HAVE_OPENCL1

    if (a->u) {
      clReleaseMemObject((cl_mem)a->u);
      a->u = NULL;
    }

    if (used_gpu) {
      ocl_device_t* dev = ocl_device_get();
      a->u = (udata_t*)clCreateBuffer(dev->context, CL_MEM_READ_WRITE, datasize, NULL, NULL);
    }

#endif
  }

  a->modifyp = 0;
  a->modifyu = 0;
  memset(a->p, 0, datasize);
}

static void umem_free(umem_t* a)
{
  if (a->p) {
    free(a->p);
    a->p = NULL;
  }

#ifdef HAVE_OPENCL1

  if (a->u) {
    clReleaseMemObject((cl_mem)a->u);
    a->u = NULL;
  }

#endif

  a->datasize = 0;
  a->modifyp = 0;
  a->modifyu = 0;
}

#define WEIGHT_MAX_DATA  8
typedef struct Weight {
  umem_t data[WEIGHT_MAX_DATA];
  DataSize size[WEIGHT_MAX_DATA];
  int n_data;
  DataSize strideSize;
  int pad;
} Weight;

CC_INLINE void freeWeight(struct Weight* weight)
{
  int i;
  for (i=0; i<WEIGHT_MAX_DATA; ++i) {
    umem_free(weight->data+i);
  }
}
CC_INLINE void pBoxsetsize(pBox* box, int h, int w, int c, int f) {
  box->size = dataSize(f, c, h, w);
  resetZero(&box->data, box->size);
}
CC_INLINE pBox* pBox_free(pBox* box) {
  freeArr(box->data);
  return NULL;
}
CC_INLINE pBox* pBox_clone(const pBox* box, pBox* outBox) {
  outBox->size = box->size;
  resetZero(&outBox->data, outBox->size);
  memcpy(outBox->data, box->data, Data_count(outBox->size)*sizeof(*box->data));
  return outBox;
}
CC_INLINE int pBox_channels(const pBox* box) {
  return box->size.c;
}
CC_INLINE float* pBox_datap(const pBox* box) {
  return (float*)box->data;
}
CC_INLINE float* pBox_datapf(const pBox* box, int f) {
  return (float*)box->data + Data_4DIndex1(box->size, f);
}
#define umem_getu(x)

enum {
  PIXEL_CONVERT_SHIFT = 16,
  PIXEL_FORMAT_MASK = 0x0000ffff,
  PIXEL_CONVERT_MASK = 0xffff0000,

  PIXEL_RGB       = 1,
  PIXEL_BGR       = (1 << 1),
  PIXEL_GRAY      = (1 << 2),
  PIXEL_RGBA      = (1 << 3),

  PIXEL_RGB2BGR   = PIXEL_RGB | (PIXEL_BGR << PIXEL_CONVERT_SHIFT),
  PIXEL_RGB2GRAY  = PIXEL_RGB | (PIXEL_GRAY << PIXEL_CONVERT_SHIFT),

  PIXEL_BGR2RGB   = PIXEL_BGR | (PIXEL_RGB << PIXEL_CONVERT_SHIFT),
  PIXEL_BGR2GRAY  = PIXEL_BGR | (PIXEL_GRAY << PIXEL_CONVERT_SHIFT),

  PIXEL_GRAY2RGB  = PIXEL_GRAY | (PIXEL_RGB << PIXEL_CONVERT_SHIFT),
  PIXEL_GRAY2BGR  = PIXEL_GRAY | (PIXEL_BGR << PIXEL_CONVERT_SHIFT),

  PIXEL_RGBA2RGB  = PIXEL_RGBA | (PIXEL_RGB << PIXEL_CONVERT_SHIFT),
  PIXEL_RGBA2BGR  = PIXEL_RGBA | (PIXEL_BGR << PIXEL_CONVERT_SHIFT),
  PIXEL_RGBA2GRAY = PIXEL_RGBA | (PIXEL_GRAY << PIXEL_CONVERT_SHIFT),
};

#define DECLARE_LAYER_TYPE static const string layerType;
#define DEFINE_LAYER_TYPE(class_type,type_string) const string class_type::layerType = type_string;
#define FRIEND_WITH_NETWORK friend class struct Net;
#define MAXIDS 16
#define BLOB(_i)  (net->blobs+(_i))
#define LIBLOB(i) (layer->id[i])
#define LOBLOB(i) (layer->id[i+layer->i_size])
#define LWBLOB(i) (layer->id[i+layer->i_size+layer->o_size])
#define IBLOB(i) BLOB(LIBLOB(i))
#define OBLOB(i) BLOB(LOBLOB(i))
#define WBLOB(i) BLOB(LWBLOB(i))
#define METHOD_DEFDEF(DEF) \
  DEF(Method, Null) \
  DEF(Method, Max) \
  DEF(Method, L1) \
  DEF(Method, L2) \
  DEF(Method, Mean) \
  DEF(Method, Softmax) \
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
  DEF(Method, pad_type) \
  DEF(Method, pool) \
  DEF(Method, distance_type) \
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
  DEF(Int, axis) \
  DEF(Int, transpose) \
  DEF(Int, num_output) \
  DEF(Float, rate) \
  DEF(Float, eps) \
  DEF(Float, scale) \
  DEF(Float, alpha) \
  DEF(Float, loss_weight) \
  DEF(Float, slope) \
  DEF(Float, power) \
  DEF(Float, shift) \
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
  Op_Reshape,
  Op_Forward,
  Op_Backward,
};
#define Layer_forward(layer) (layer)->run(Op_Forward, net, layer)
#define Layer_backward(layer) (layer)->run(Op_Backward, net, layer)
typedef Dtype (*LayerFun)(enum LayerOp op, struct Net* net, struct Layer* layer);
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
struct Net {
  struct Layer* layers;
  struct Blob* blobs;
  int l_size, b_size;
  union LayerArg x[NetArgCount];
};
static void net_free(struct Net* net) {
  int i;
  for (i=0; i<net->b_size; ++i) {
    Blob_free(net->blobs + i);
  }
  free(net->layers);
  memset(net, 0, sizeof(*net));
}
static struct Layer* initLayer(struct Layer* layer, int i_size, int o_size, int w_size, LayerFun run, const char* type)
{
  setzero(layer);
  layer->i_size = i_size;
  layer->o_size = o_size;
  layer->w_size = w_size;
  layer->run = run;
  layer->type = type;
  return layer;
}
static int find_or_insert_blob(struct Net* net, const char* name, int bInsert)
{
  int i = 0;
  ASSERT(strlen(name) < MAXNAME);
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
    return i;
  }
  //fprintf(stderr, "find_blob_index_by_name %s failed\n", name);
  return -1;
}
static int net_addBlob(struct Net* net, DataSize size, const char* type, const char* name)
{
  int i = -1;
  char buf[MAXNAME];
  if (type==NULL) {
    type = name;
    name = NULL;
  }
  while(1) {
    if (i>=0) {
      if (name && *name) {
        _snprintf(buf, MAXNAME, "%s_%s_%d", type, name, i);
      } else {
        _snprintf(buf, MAXNAME, "%s_%d", type, i);
      }
    } else {
      if (name && *name) {
        _snprintf(buf, MAXNAME, "%s_%s", type, name);
      } else {
        _snprintf(buf, MAXNAME, "%s", type);
      }
    }
    if (find_or_insert_blob(net, buf, 0)<0) {
      int j = find_or_insert_blob(net, buf, 1);
      net->blobs[j].size = size;
      return j;
    }
    ++i;
  }
  //struct Blob* blob = resetBlob(net->blobs + i, size);
  return i;
}
static int countLayerBuType(const struct Net* net, const char* type) {
  int i, c=0;
  for (i=0; i<net->l_size; ++i) {
    c += (0==strcmp(type, net->layers[i].type));
  }
  return c;
}
static struct Layer* net_addLayer(struct Net* net, int i_size, int o_size, int w_size, LayerFun run, const char* type)
{
  int cnt = countLayerBuType(net, type);
  int i = net->l_size++;
  struct Layer* layer;
  // logVerbose("struct NetWork addayer begin , type : %s", type);
  resetArr(&net->layers, net->l_size);
  layer = initLayer(net->layers + i, i_size, o_size, w_size, run, type);
  _snprintf(layer->name, MAXNAME, "%s_%d", type, cnt);
  //dataBucket setting params
  //logVerbose("struct NetWork addayer end. add data blob done.");
  return net->layers + i;
}
#include "blob.h"

enum IFArgType {IF_Int, IF_Float, IF_Method};
static const enum IFArgType ifArgType[] = {
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

#if 0
static int net_init_uniform_int(struct Net* net, int i, int low_value, int high_deviation)
{
  struct Blob* blob = BLOB(i);
  uniform_distribution_init_int((int*)setData(blob), Data_total(blob->size), low_value, high_deviation);
  return i;
}
static int net_init_uniform(struct Net* net, int i, const float low_value, const float high_deviation)
{
  struct Blob* blob = BLOB(i);
  uniform_distribution_init(setData(blob), Data_total(blob->size), low_value, high_deviation);
  return i;
}
#define net_init_gaussian net_init_normal
static int net_init_normal(struct Net* net, int i, const float mean_value, const float standard_deviation)
{
  struct Blob* blob = BLOB(i);
  normal_distribution_init(setData(blob), Data_total(blob->size), mean_value, standard_deviation);
  return i;
}
static int net_init_xavier(struct Net* net, int i)
{
  struct Blob* blob = BLOB(i);
  fill_Xavier(setData(blob), blob->size, FillMethod_FAN_IN);
  return i;
}
static int net_init_const(struct Net* net, int i, float const_value)
{
  struct Blob* blob = BLOB(i);
  const_distribution_init(setData(blob), Data_total(blob->size), const_value);
  return i;
}
static int net_init_label_u8(struct Net* net, int i, int n, int classes, const char* label_u8, bool onehot)
{
  return Blob_init_label_u8(BLOB(i), );
}
#endif
#define DBG_DIFF(next)  printf("%16s %f\n", next->name, Blob_diffDot(next))
static double Blob_dataDot(const struct Blob* blob) {
  double diff_dot = blas_sdot(Data_total(blob->size), blob->data, blob->data)/Data_total(blob->size);
  return diff_dot;
}
static double Blob_diffDot(const struct Blob* blob) {
  double diff_dot = blas_sdot(Data_total(blob->size), blob->diff, blob->diff)/Data_total(blob->size);
  return diff_dot;
}
static float net_forward(struct Net* net)
{
  int i;
  Dtype loss = 0;
  struct Layer* layer = net->layers;
  //logVerbose("struct NetWork forward begin.");
  easyAssert(net->l_size > 0, "layer count is less than 1.");
  easyAssert(net->b_size > 0, "data blobs is not ready.");
  for (i = 0; i < net->l_size; i++) {
    layer = net->layers + i;
    logVerbose("layer[%d](%s) forward begin.", i, layer->type);
    //for (j = 0; j < layer->o_size; ++j) { setDataZero(OBLOB(j)->data); }
    //{utime_start(_start_time);
    loss += Layer_forward(layer);
    Blob* next = OBLOB(0);
    //printf("%2d %15s %3d %3d %3d %3d\n", i, layer->type, next->n, next->c, next->h, next->w);
    //showmat_1("data", IBLOB(0)->data, IBLOB(0)->size, 0, 0);WaitKey(-1);
    //printf("%f %s\n", utime_elapsed(_start_time), layer->name);}
    logVerbose("layer[%d](%s) forward end.", i, layer->name);
    //int DestroyAllWindows();
    //DestroyAllWindows();
  }
  //logVerbose("forward end.");
  return loss;
}
static void net_clear_diff(struct Net* net) {
  //blas_set(net->data_size, 0, net->diff);
  int i, j;
  for (j=0; j<net->l_size; ++j) {
    struct Layer* layer = net->layers + j;
    for (i=0; i<layer->i_size; ++i) {
      struct Blob* blob = IBLOB(i);
      resetZero(&blob->diff, blob->size);
    }
  }
}
static void net_backward(struct Net* net)
{
  int i;
  struct Layer* layer = net->layers;
  MNARG(phase) = M_Train;
  //easyAssert(MNARG(phase) == M_Train, "phase must be train!");
  logVerbose("struct NetWork backward begin.");
  easyAssert(net->l_size > 0, "layer count is less than 1.");
  //easyAssert(net->lossFunctor->run != NULL, "loss functor can't be empty!");
  //other layer backward
  net_clear_diff(net);
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
  //return loss;
  return ;
}
static Dtype net_forward_backward(struct Net* net)
{
  MNARG(phase) = M_Train;
  //tic;
  Dtype loss = net_forward(net);
  //toc;
  //tic;
  net_backward(net);
  //toc;
  //tictoc;
  return loss;
}
static int getMaxIdxInArray(const float* start, const int len) {
  int i, result = 0;
  assert(len > 1);
  for (i = 1; i < len; i++) {
    if (start[i] > start[result]) {
      result = i;
    }
  }
  return result;
}
static int getTopK(const float* start, const int len, int k, int* id, float* score) {
  int i, result = 0;
  int* index = MALLOC(int, len);
  assert(len > 1);
  for (i = 0; i < len; i++) {
    index[i] = i;
  }
#define GETTOPK_GREATER(a, b)  start[a]>start[b]
  QSORT(index, index+len, GETTOPK_GREATER, int);
#undef GETTOPK_GREATER
  for (i=0; i<k; ++i) {
    id[i] = index[i];
    score[i] = start[index[i]];
  }
  FREE(index);
  return result;
}
static float getMaxIdxInArrayRegress(const float* start, const int len, int i) {
  float r=i*start[i];
  float w=start[i];
  if (i>0) {
    r += (i-1)*start[i-1];
    w += start[i-1];
  }
  if (i<(len-1)) {
    r += (i+1)*start[i+1];
    w += start[i+1];
  }
  return w>0 ? r/w : i;
}

static float getAccuracy(const struct Blob* prob, const struct Blob* label)
{
  const DataSize probSize = prob->size;
  const DataSize labelSize = label->size;
  const int itemSize = Data_3DSize(labelSize);
  const float* probData = getData(prob);
  const float* labelData = getData(label);
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

#include "layer/input_layer.h"
#include "layer/loss_layer.h"
//#include "layer/accuracy_layer.h"
#include "layer/dropout_layer.h"
#include "layer/conv_layer.h"
#include "layer/softmax_layer.h"
#include "layer/sigmoid_layer.h"
#include "layer/inner_product_layer.h"
#include "layer/bias_layer.h"
#include "layer/scale_layer.h"
#include "layer/batch_norm_layer.h"
#include "layer/pooling_layer.h"
#include "layer/relu_layer.h"
#include "layer/tanh_layer.h"
#include "layer/absval_layer.h"
#include "layer/arccos_layer.h"
//#include "layer/normalize_layer.h"
#include "layer/argmax_layer.h"
#include "layer/eltwise_layer.h"
#include "layer/power_layer.h"
#include "layer/split_layer.h"
#include "layer/lrn_layer.h"
#include "layer/axpy_layer.h"
#include "layer/center_loss_layer.h"
//#include "layer/triplet_loss_layer.h"
//#include "layer/softmax_loss_layer.h"
//#include "layer/slice_layer.h"
#include "layer/concat_layer.h"

//SGD
//w -= lr*g
static int net_updateSGD(struct Net* net)
{
  int i, j, k;
  float lr = FNARG(lr);
  int out_debug = 0;
  for (k = 0; k < net->l_size; ++k) {
    struct Layer* layer = net->layers + k;
    if (out_debug) {
      for (i = 0; i < layer->o_size; i++) {
        struct Blob* param = OBLOB(i);
        const float* paramDiff = getDiff(param);
        float* paramData = regetData(param);
        double data_dot = Blob_dataDot(param);
        double diff_dot = Blob_diffDot(param);
        if (data_dot<0.0001 || data_dot>10) {
          printf("layer:%d %10s data_dot=%f\n", k, param->name, data_dot);
        }
        if (diff_dot<0.001 || diff_dot>10) {
          printf("layer:%d %10s diff_dot=%f\n", k, param->name, diff_dot);
        }
      }
    }
    for (i = 0; i < layer->w_size; i++) {
      struct Blob* param = WBLOB(i);
      const float* paramDiff = getDiff(param);
      float* paramData = regetData(param);
      int id = LWBLOB(i);
      if (paramDiff) {
        if (out_debug) {
          double diff_dot = Blob_diffDot(param);
          if (diff_dot<0.001 || diff_dot>10) {
            printf("layer:%d %10s diff_dot=%f\n", k, param->name, diff_dot);
          }
        }
        for (j = 0; j < Data_total(param->size); j++) {
          float d = BOUND(paramDiff[j], -1, 1);
          paramData[j] -= lr * d;
        }
      }
    }
  }
  return 0;
}
//SGDWithMomentum
//prev_m = momentum*prev_m+g[t]
//w -= lr*prev_m
static void net_updateSGDWithMomentum(struct Net* net)
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

//train net->phase may use this
static Dtype net_testBatch(struct Net* net)
{
  MNARG(phase) = M_Test;
  return net_forward(net);
}

static int Layer_weightSize(const struct Net* net, const struct Layer* layer) {
  int i, size=0;
  for (i=0; i<layer->w_size; ++i) {
    struct Blob* blob = WBLOB(i);
    size += Data_total(blob->size);
  }
  return size;
}
static int net_print_debug(const struct Net* net) {
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
static int net_print_impl(const struct Net* net, FILE* pf, int bSaveParam, const char* linebeg, const char* lineend) {
  int i, j;
  int *blob_type = NULL;
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
    fprintf(pf, "%s", lineend);
  }
  for (i=0; i<net->b_size; ++i) {
    if (blob_type[i]) {
      DataSize size = net->blobs[i].size;
      fprintf(pf, "%s%16s %3d %3d %3d %3d%s", linebeg, net->blobs[i].name, size.n, size.c, size.h, size.w, lineend);
    }
  }
  if (bSaveParam) {
    fprintf(pf, "// param\n");
    for (i=0; i<net->b_size; ++i) {
      if (blob_type[i]) {
        DataSize size = net->blobs[i].size;
        int datasize = Data_total(size);
        if (0) {
          fwrite(net->blobs[i].data, datasize, sizeof(float), pf);
        } else {
          fprintf(pf, "//%16s %3d %3d %3d %3d\n", net->blobs[i].name, size.n, size.c, size.h, size.w);
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
static int net_print(const struct Net* net, FILE* pf, int bSaveParam) {
  const char* linebeg = bSaveParam ? "//": "";
  const char* lineend = "\n";
  return net_print_impl(net, pf, bSaveParam, linebeg, lineend);
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

static Dtype InputLayer(enum LayerOp op, struct Net* net, struct Layer* layer)
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
  return 0;
}
static int net_null_1_1(struct Net* net, int input, LayerFun run, const char* type)
{
  struct Layer* layer = net_addLayer(net, 1, 1, 0, run, type);
  LIBLOB(0) = input;
  return LOBLOB(0) = net_addBlob(net, IBLOB(0)->size, type, "o");
}
static int net_input(struct Net* net, DataSize size) {
  const char* type = "input";
  struct Layer* layer = net_addLayer(net, 0, 1, 0, InputLayer, type);
  IARG(input_c) = size.c;
  IARG(input_h) = size.h;
  IARG(input_w) = size.w;
  return LOBLOB(0) = net_addBlob(net, size, type, "");
}

#define LAYER_DEFDEF(DEF) \
LAYER_REGISTRY(input, InputLayer)  \
LAYER_REGISTRY(conv, ConvolutionLayer)  \
LAYER_REGISTRY(fc, FullconnectLayer)  \
LAYER_REGISTRY(dropout, DropoutLayer)  \
LAYER_REGISTRY(pool, PoolingLayer)  \
LAYER_REGISTRY(relu, ReluLayer)  \
LAYER_REGISTRY(bn, BatchNormLayer)  \
LAYER_REGISTRY(scale, ScaleLayer)  \
LAYER_REGISTRY(sigmoid, SigmoidLayer)  \
LAYER_REGISTRY(softmax, SoftmaxLayer)

static int layer_init(struct Layer* layer, const char* type)
{
  static char const * layer_type[] = {
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
    if (stricmp(type, layer_type[i]) == 0) {
      layer->type = layer_type[i];
      layer->run = layer_run[i];
      //layer->name = layer_name[i];
      return 1;
    }
  }

  return 0;
}


static int cstr_cpy(char* s0, int len, const char* s2, int l2)
{
	CSTRINITLEN(s2, l2);
	//ASSERT(l2 < len);
	l2 = MIN(l2, len - 1);
	memcpy(s0, s2, l2);
	s0[l2] = 0;
	return l2;
}
CC_INLINE int get_delimsmap(uchar* delimsmap, const char* delims) {
	for (; *delims;) {
		delimsmap[(uchar)(*delims++)] = 1;
	}
	return 0;
}
//#define isalnum_(c)   (isalnum(c) || '_'==(c) || '-'==(c))
//#define isspace_(c)   (' '==(c) || '\t'==(c))
static int cstr_getstr(str_t s, int* pi, str_t* ps, char* str, int len, const char* delims) {
	int i = *pi, ret = 0;
	str_t s2;
	uchar delimsmap[256] = { 0 };
	delims = delims ? delims : " \t";
	get_delimsmap(delimsmap, delims);
	ps = ps ? ps : &s2;
	for (; i<s.l && delimsmap[(uchar)s.s[i]]; ++i) {}
	ps->s = s.s + i;
	for (; i<s.l && !delimsmap[(uchar)s.s[i]]; ++i);
	ps->l = s.s + i - ps->s;
	if (str) {
		cstr_cpy(str, len, ps->s, ps->l);
	}
	ret = i>*pi;
	*pi = i;
	return ret;
}
static int cstr_getkv(str_t s, int* pi, str_t* pk, str_t* pv) {
	int ret = 0;
	if (cstr_getstr(s, pi, pk, NULL, 0, " \t=") && s.s[*pi] == '=') {
		*pi += 1;
		ret = cstr_getstr(s, pi, pv, NULL, 0, " \t=");
	}
	return ret;
}
static int cstr_getline(str_t s, int* pi, str_t* ps) {
	int i = *pi, ret = 0;
	ps->s = s.s + i;
	for (; i<s.l && s.s[i] != '\r' && s.s[i] != '\n'; ++i);
	ps->l = s.s + i - ps->s;
	for (; i<s.l && (s.s[i] == '\r' || s.s[i] == '\n'); ++i);
	ret = i>*pi;
	*pi = i;
	return ret;
}

static int cstr_getint(str_t s, int* pi, int* pint, int def) {
	int i = *pi, ret = 0;
	for (; i<s.l && !isdigit(s.s[i]); ++i) {}
	if (isdigit(s.s[i])) {
		char* s1 = (char*)s.s + i;
		def = strtol(s1, &s1, 10);
		i += s1 - (s.s + i);
	}
	ret = i>*pi;
	*pi = i;
	*pint = def;
	return ret;
}
#define SKIPSLASH   for(; '/'==line.s[p] || '\\'==line.s[p]; ++p)
static int net_load_from_string(struct Net* net, const char* str, int len)
{
  int layer_cnt = 0;
  int blob_cnt = 0;
  str_t s = STR2(str, len), line;
  int i, j=0, nscan=0, p=0;
  int l_size = 0;//, b_size = 0;
  char buf[256] = {0};
  int count, line_cnt=0;
  
  // parse
  cstr_getline(s, &j, &line);
  SKIPSLASH;
  nscan = cstr_getint(line, &p, &l_size, 0);// &&    cstr_getint(line, &p, &b_size, 0);
  if (!nscan) {
    printf("file format error in line %d\n", j);
    return 0;
  }
  
  MYREALLOC(net->layers, l_size);
  MEMZERO(net->layers, l_size);
  //net->b_size = net->l_size;
  //MYREALLOC(net->blobs, b_size);
  //MEMZERO(net->blobs, b_size);
  ++line_cnt;
  for (; layer_cnt<l_size && cstr_getline(s, &j, &line); layer_cnt++) {
    struct Layer *layer = net->layers + layer_cnt;
    int size = 0;
    str_t s_key, s_value;
    ++line_cnt;
    p=0;
    SKIPSLASH;
    nscan = cstr_getstr(line, &p, NULL, buf, countof(buf), NULL)
       && cstr_getint(line, &p, &layer->i_size, 0)
       && cstr_getint(line, &p, &layer->o_size, 0)
       && cstr_getint(line, &p, &layer->w_size, 0)
       ;
    
    if (!nscan) {
      printf("file format error in line %d\n", line_cnt);
      continue;
    }
    
    if (!layer_init(layer, buf)) {
      printf("layer_init error in line %d : %s\n", line_cnt, buf);
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
      int ret = setLayerArg(layer->x, s_key, s_value);
      if (!ret) {
        if (0==strnicmp("kernel_size", s_key.s, s_key.l)) {
          int kernel_size = atoi(s_value.s);
          IARG(kernel_h) = kernel_size;
          IARG(kernel_w) = kernel_size;
        } else if (0==strnicmp("stride", s_key.s, s_key.l)) {
          int stride = atoi(s_value.s);
          IARG(stride_h) = stride;
          IARG(stride_w) = stride;
        } else if (0==strnicmp("bias_term", s_key.s, s_key.l)) {
          int bias_term = atoi(s_value.s);
        } else if (0==strnicmp("pad", s_key.s, s_key.l)) {
          int pad = atoi(s_value.s);
          if (pad==(IARG(kernel_h)-1)/2) {
            MARG(pad_type) = M_SAME;
          } else if (pad==0) {
            MARG(pad_type) = M_VALID;
          } else {
            printf("pad error in line %d\n", line_cnt);
          }
        } else {
          char buf[256]={0};
          strncpy(buf, s_key.s, MIN(countof(buf), s_key.l));
          printf("setLayerArg error in line %d : %s\n", line_cnt, buf);
        }
      }
    }
  }
  ASSERT(l_size==layer_cnt);
  net->l_size = l_size;
  count = 0;
  for (; cstr_getline(s, &j, &line) && line.s[0]=='/'; ) {
    int id = 0;
    DataSize size = {0};
    ++line_cnt;
    p=0;
    SKIPSLASH;
    nscan = cstr_getstr(line, &p, NULL, buf, countof(buf), NULL)
       && cstr_getint(line, &p, &size.n, 0)
       && cstr_getint(line, &p, &size.c, 0)
       && cstr_getint(line, &p, &size.h, 0)
       && cstr_getint(line, &p, &size.w, 0)
       ;
    if (!nscan) {
      printf("blob init error in line %d\n", line_cnt);
      continue;
    }
    id = find_or_insert_blob(net, buf, 0);
    if (id<0) {
      printf("can not find blob in line %d\n", line_cnt);
      continue;
    }
    count += Data_total(size);
    net->blobs[id].size = size;
  }
  return 1;
}
#include "std/fileio_c.h"
static int net_save(const struct Net* net, const char* model_file, const char* param_file, int epochIdx) {
  FILE* pf;
  if (model_file) {
    pf = fopen(model_file, "wb");
    if (pf) {
      net_print(net, pf, 1);
      fclose(pf);
    }
#if 0
    char model_inl_file[256];
    GetFilePathCopy(model_file, model_inl_file, countof(model_inl_file));
    strcat(model_inl_file, "_str.inl");
    pf = fopen(model_inl_file, "wb");
    if (pf) {
      net_print_impl(net, pf, 0, "\"\\", "\\n\"");
      fclose(pf);
    }
#endif
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
static int net_set_param(struct Net* net, const float* param, int len) {
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
      if (param) {
        if ((j+datasize) <= len) {
          memcpy(blob->data, param+j, sizeof(float)*datasize);
        } else {
          ret = 0;
          printf("net_set_param param len error\n");
          break;
        }
      }
      j += datasize;
    }
  }
  if (param) {
    ASSERT(j==len);
  }
  FREE(blob_type);
  return j;
}
static int net_load(struct Net* net, const char* model_file, const char* param_file) {
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
      int count = bf->len/sizeof(float);
      int param_count = net_set_param(net, (float*)bf->data, count);
    }
    bffree(bf);
  }
  return ret;
}

typedef int (*fetch_data_t)(void* s, struct Blob* inputBlob, struct Blob* labelBlob, int start, int shuffle_len, bool onehot);

struct net_train_test_t {
  int batch;
  int max_epoch;
  int maxBatches;
  int testAfterBatches;
  fetch_data_t fetch_data;
  void* s;
  bool onehot;
  int n_train;
  int n_test;
  float train_rate;
  float learningRate;
  float decayRate;
  float minLearningRate;
  int isclass;
};

static void net_train_test_init(struct net_train_test_t* t, void* s, fetch_data_t fetch_data, int n_train, int n_test, int batch) {
  t->fetch_data = fetch_data;
  t->s = s;

  t->learningRate = 0.1f;
  t->decayRate = 0.8f;
  t->minLearningRate = 0.001f;
  t->testAfterBatches = 50;
  t->maxBatches = 10000;
  t->max_epoch = 100*(1+n_test/(batch*10));
  t->batch = batch;
  t->n_train = n_train;
  t->n_test = n_test;
  t->onehot = true;
  t->onehot = false;
  
  logCritical("load training data done. train set's size is %d,validate set's size is %d", n_train, n_test);
  logCritical("max_epoch:%d,testAfterBatches:%d", t->max_epoch, t->testAfterBatches);
  logCritical("learningRate:%f ,decayRate:%f , minLearningRate:%f", t->learningRate, t->decayRate, t->minLearningRate);
  return ;
}

struct test_result_t {
  float accuracy;
  float loss;
  float err_mean;
  float err_var;
  float err_std;
  float cnt_1;
  float cnt_2;
};

static struct test_result_t net_test(const struct net_train_test_t* t, struct Net* net, int input, int output, int label)
{
  int train_start = 0;
  int train_end = t->n_train;
  int test_start = train_end;
  int test_end = t->n_train + t->n_test;
  int correctCount = 0, batchs = 0, i, j;
  float loss = 0.0f, batch_loss;
  struct test_result_t ret = {0};
  float ntest = test_end-test_start;
  void* s = t->s;
  int labelSize, outputSize;
  for (i = test_start; i < test_end; i += t->batch, batchs++) {
    int len = MIN(test_end - i, t->batch);
    t->fetch_data(s, BLOB(input), BLOB(label), i, len, t->onehot);
    batch_loss = net_testBatch(net);
    loss = moving_average(loss, batchs + 1, batch_loss);
    labelSize = Data_3DSize(BLOB(label)->size);
    outputSize = Data_3DSize(BLOB(output)->size);
    
    //assert(labelSize == s->classes);
    if (t->onehot) {
      if (labelSize==outputSize) {
        const float* outputData = getData(BLOB(output));
        const float* labelData = getData(BLOB(label));
        for (j = 0; j < len; j++) {
          int labelId = getMaxIdxInArray(labelData + j * labelSize, labelSize);
          int outputId = getMaxIdxInArray(outputData + j * outputSize, outputSize);
          float err = labelId - outputId;
          ret.err_mean += err;
          ret.err_var += err*err;
          ret.cnt_1 += abs(labelId-outputId)>=1;
          ret.cnt_2 += abs(labelId-outputId)>=2;
#if 0
          if (0 && abs(labelId-outputId)>=2) {
            showmat_1("adf", BLOB(input)->data, BLOB(input)->size, j, 0);
            printf("%3d: labelId=%d outputId=%d\n", j, labelId, outputId);
            WaitKey(-1);
          }
#endif
          if (labelId == outputId) {
            correctCount++;
          }
        }
      } else {
        const float* outputData = getData(BLOB(output));
        const float* labelData = getData(BLOB(label));
        for (j = 0; j < len; j++) {
          double dis = blas_seuclidean(labelSize, labelData + j * labelSize, outputData + j * labelSize);
          ret.accuracy += sqrt(dis);
        }
      }
    } else {
      const float* outputData = getData(BLOB(output));
      const int* labelData = (int*)getData(BLOB(label));
      for (j = 0; j < len; j++) {
        int labelId = labelData[j];
        int outputId = getMaxIdxInArray(outputData + j * outputSize, outputSize);
        float err = labelId - outputId;
        ret.err_mean += err;
        ret.err_var += err*err;
        ret.cnt_1 += abs(labelId-outputId)>=1;
        ret.cnt_2 += abs(labelId-outputId)>=2;
#define showmat_1(a, b, c, d, e)
#define WaitKey(a)
#if 0
        if (0 && abs(labelId-outputId)>=2) {
          showmat_1("adf", BLOB(input)->data, BLOB(input)->size, j, 0);
          printf("%3d: labelId=%d outputId=%d\n", j, labelId, outputId);
          WaitKey(-1);
        }
#endif
        if (labelId == outputId) {
          correctCount++;
        }
      }
    }
  }
  ret.cnt_1 /= ntest;
  ret.cnt_2 /= ntest;
  ret.err_mean /= ntest;
  ret.err_var /= ntest;
  ret.err_std = sqrt(ret.err_var);
  if (labelSize>0) {
    ret.accuracy = (float)correctCount / (float)ntest;
  } else {
    ret.accuracy /= ntest;
  }
  ret.loss = loss;
  return ret;
}
static struct test_result_t net_train(const struct net_train_test_t* t, struct Net* net,
                                      int input, int output, int label, const char* model_file, const char* param_file) {
  int train_start = 0;
  int train_end = t->n_train;
  float train_loss = 0.0f;
  int train_batches = 0;
  int start = train_start;
  int end = train_end;
  struct test_result_t val;
  //train
  int epochIdx = 0;
  int n = t->n_train+t->n_test;
  float learningRate = t->learningRate;
  int maxBatches = MIN(t->maxBatches, (n+t->batch-1)/t->batch);
  logCritical("begin training...");
  logCritical("%13s|%7s|%10s|%10s|%10s", "sample", "lr", "train_loss", "val_loss", "val_accuracy");
  for (; epochIdx < t->max_epoch; ++epochIdx) {
    //before epoch start, shuffle all train data first
    int batchIdx = 0;
    for (; batchIdx < maxBatches; ++batchIdx) {
      int i = start + batchIdx * t->batch;
      int len = MIN(end - i, t->batch);
      float batch_loss;
      if (!t->fetch_data(t->s, BLOB(input), BLOB(label), i, len, t->onehot)) {
        break;
      }
      FNARG(lr) = learningRate;
      batch_loss = net_forward_backward(net);
      net_updateSGD(net);
      //logCritical("%6d/%-6d|batch_loss : %f", i, t->n_train, batch_loss);
      train_loss = moving_average(train_loss, train_batches + 1, batch_loss);
      train_batches++;
      if (batchIdx > 0 && (batchIdx % t->testAfterBatches) == 0)
      //if (batchIdx > 0)
      {
        val = net_test(t, net, input, output, label);
        net_save(net, model_file, param_file, epochIdx);
        logCritical("%6d/%-6d|%7.4f|%10.4f|%10.4f|%10.4f%%",
            i, t->n_train, t->learningRate, train_loss, val.loss, val.accuracy * 100.0f);
        train_loss = 0.0f;
        train_batches = 0;
      }
    }
    net_save(net, model_file, param_file, epochIdx);
    if (batchIdx >= t->maxBatches) {
      break;
    }
    val = net_test(t, net, input, output, label);
    //update learning rate
    learningRate = MAX(learningRate * t->decayRate, t->minLearningRate);
    logCritical("epoch[%d] val_loss : %f , val_accuracy : %.4f%%", epochIdx, val.loss, val.accuracy * 100.0f);
  }
  net_save(net, model_file, param_file, -1);
  val = net_test(t, net, input, output, label);
  logCritical("final val_loss : %f , final val_accuracy : %.4f%%", val.loss, val.accuracy * 100.0f);
  // success = net.saveModel(modelFilePath);
  logCritical("finished training.");
  return val;
}

Dtype net_forward_loss_gradient(struct Net* net, int top0, int top_id, int top_data_id) {
  Dtype loss = 0;
  int i, j;
  int ntop = 1, top[] = {top0};
  loss = net_forward(net);
  if (top0<0) {
    return loss;
  }
  if (top_id < 0) {
    // the loss will be half of the sum of squares of all outputs
    for (i = 0; i < ntop; ++i) {
      Blob* top_blob = BLOB(top[i]);
      const Dtype* top_blob_data = getData(top_blob);
      Dtype* top_blob_diff = setDiffZero(top_blob);
      int count = Blob_count(top_blob);
      for (j = 0; j < count; ++j) {
        loss += top_blob_data[j] * top_blob_data[j];
      }
      // set the diff: simply the data.
      blas_scopy(count, top_blob_data, top_blob_diff);
    }
    loss /= 2.;
  } else {
    // the loss will be the top_data_id-th element in the top_id-th blob.
    for (i = 0; i < ntop; ++i) {
      Blob* top_blob = BLOB(top[i]);
      Dtype* top_blob_diff = setDiffZero(top_blob);
      int count = Blob_count(top_blob);
      blas_sset(count, (Dtype)(0), top_blob_diff);
    }
    const Dtype loss_weight = 2;
    loss = getData(BLOB(top[top_id]))[top_data_id] * loss_weight;
    setDiff(BLOB(top[top_id]))[top_data_id] = loss_weight;
  }
  return loss;
}
static bool net_check_gradient_single(struct Net* net, int bottom, int top, float delta, float eps) {
  int check_index;
  assert(net->l_size>0);
  //net_loss(net, top, NULL, MSELoss);

  struct Blob* blob = BLOB(bottom);
  int count = Data_count(blob->size);
  for (check_index = 0; check_index < count; ++check_index) {
    float prev_w, delta_by_bprop, f_p, f_m, delta_by_numerical, err;
    float* data;
    float* diff;
    //float err = calc_delta_diff(net, layer, blob, check_index, delta);
    net_clear_diff(net);
    net_forward_loss_gradient(net, top, -1, -1);
    net_backward(net);
    data = blob->data;
    diff = blob->diff;
    delta_by_bprop = diff[check_index];
    
    // calculate dw/dE by numeric
    prev_w = data[check_index];
    data[check_index] = prev_w + delta;
    net_clear_diff(net);
    f_p = net_forward_loss_gradient(net, top, -1, -1);
    
    data[check_index] = prev_w - delta;
    net_clear_diff(net);
    f_m = net_forward_loss_gradient(net, top, -1, -1);
    
    delta_by_numerical = (f_p - f_m) / (2.0 * delta);
    
    data[check_index] = prev_w;
    err = fabs(delta_by_bprop - delta_by_numerical);
    if (err > eps) {
      printf("delta_by_bprop=%f \ndelta_by_numerical=%f", delta_by_bprop, delta_by_numerical);
      ASSERT(0);
      //return false;
    }
  }
  
  return true;
}
// gradient_check
static bool net_check_gradient(struct Net* net, int nbot, const int* bottom, int top, float delta, float eps) {
  int i;
  for (i=0; i<nbot; ++i) {
    net_check_gradient_single(net, bottom[i], top, delta, eps);
  }
  return true;
}
static int add_active_layer(struct Net* net, int input)
{
  return net_relu(net, input);
}
static int add_conv(struct Net* net, int input, int n, int ker)
{
  int w, b;
  //return net_conv(net, input, &w, &b, n, 3, 3, 1, 1, M_SAME);
  return net_conv(net, input, &w, &b, n, ker, ker, 1, 1, M_VALID);
}
static int add_conv_same(struct Net* net, int input, int n, int ker)
{
  int w, b;
  //return net_conv(net, input, &w, &b, n, 3, 3, 1, 1, M_SAME);
  return net_conv(net, input, &w, &b, n, ker, ker, 1, 1, M_SAME);
}
static int add_pool(struct Net* net, int input)
{
  return net_pooling(net, input, M_Max, 2, 2, 2, 2);
}
static int add_fc_layer(struct Net* net, int input, int output_count)
{
  int w, b;
  return net_fc(net, input, &w, &b, output_count);
}
static int add_conv_pool_relu(struct Net* net, int input, int n, int ker)
{
  int w, b;
  //return net_conv(net, input, &w, &b, n, 3, 3, 1, 1, M_SAME);
  int t = input;
  t = net_conv(net, t, &w, &b, n, ker, ker, 1, 1, M_VALID);
  t = net_relu(net, t);
  t = net_pooling(net, t, M_Max, 2, 2, 2, 2);
  return t;
}
static void initConvAndFc(float** pw, struct Weight* weight, int schannel, int lchannel, int kersize, int stride, int pad)
{
  int byteLenght;
  weight->size[0] = dataSize(schannel, lchannel, kersize, kersize);
  weight->size[1] = dataSize(1, schannel, 1, 1);
  weight->strideSize = dataSize(stride, stride, stride, stride);
  weight->n_data = 2;
  weight->pad = pad;
  weight->data[0].p = *pw;
  byteLenght = weight->size[0].n * lchannel * weight->size[0].h * weight->size[0].w;
  *pw += byteLenght;
  weight->data[1].p = *pw;
  *pw += schannel;
  //umem_setsize(weight->data+1, schannel * sizeof(Dtype));
  //umem_setsize(weight->data+0, byteLenght * sizeof(Dtype));
  return ;
}
static pBox* doFc(float** pw, const pBox* box, pBox* outBox, int schannel, int lchannel, int kersize, int stride, int pad)
{
  struct Weight weight[1] = {0};
  initConvAndFc(pw, weight, schannel, lchannel, kersize, stride, pad);
  return fullconnect(weight, box, outBox);
}

static pBox* doConv(float** pw, const pBox* box, pBox* outBox, int schannel, int lchannel, int kersize, int stride, int pad)
{
  struct Weight weight[1] = {0};
  initConvAndFc(pw, weight, schannel, lchannel, kersize, stride, pad);
  return conv(weight, box, outBox);
}
static pBox* dopRelu(float** pw, pBox* box, int width)
{
  struct Weight prelu1[1] = {0};
  initpRelu(pw, prelu1, width);
  prelu(box, prelu1->data);
  return box;
}

#endif // _CNN_INL_
