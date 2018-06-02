
#include "cstd.h"
#define CPU_ONLY 1
typedef float Dtype;

#define MAX_NAME   32
#define MAX_DIM    4
#define kMaxBlobAxes    MAX_DIM


#include "util/math_functions.inl"
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


// mat process
enum {
  BORDER_CONSTANT = 0,
  BORDER_REPLICATE = 1,
};


// exchange-add operation for atomic operations on reference counters
#if defined __INTEL_COMPILER && !(defined WIN32 || defined _WIN32)
// atomic increment on the linux version of the Intel(tm) compiler
#  define NCNN_XADD(addr, delta) (int)_InterlockedExchangeAdd(const_cast<void*>(reinterpret_cast<volatile void*>(addr)), delta)
#elif defined __GNUC__
#  if defined __clang__ && __clang_major__ >= 3 && !defined __ANDROID__ && !defined __EMSCRIPTEN__ && !defined(__CUDACC__)
#    ifdef __ATOMIC_ACQ_REL
#      define NCNN_XADD(addr, delta) __c11_atomic_fetch_add((_Atomic(int)*)(addr), delta, __ATOMIC_ACQ_REL)
#    else
#      define NCNN_XADD(addr, delta) __atomic_fetch_add((_Atomic(int)*)(addr), delta, 4)
#    endif
#  else
#    if defined __ATOMIC_ACQ_REL && !defined __clang__
// version for gcc >= 4.7
#      define NCNN_XADD(addr, delta) (int)__atomic_fetch_add((unsigned*)(addr), (unsigned)(delta), __ATOMIC_ACQ_REL)
#    else
#      define NCNN_XADD(addr, delta) (int)__sync_fetch_and_add((unsigned*)(addr), (unsigned)(delta))
#    endif
#  endif
#elif defined _MSC_VER && _MSC_VER>1200 && !defined RC_INVOKED
#  include <intrin.h>
#  define NCNN_XADD(addr, delta) (int)_InterlockedExchangeAdd((long volatile*)addr, delta)
#else

CC_INLINE int NCNN_XADD(int* addr, int delta)
{
  int tmp = *addr;
  *addr += delta;
  return tmp;
}
#endif

struct BSIZE {
  int w;
  int h;
  int c;
  int f;
};
CC_INLINE BSIZE bBSIZE(int w, int h, int c DEFAULT(1), int f DEFAULT(1)) {
  BSIZE b={w, h, c, f};
  return b;
}
struct Blob
{
  // blob name
  char name[MAX_NAME];
  // layer index which produce this blob as output
  int producer;
  // layer index which need this blob as input
  //int consumers[MAX_BLOB];
  //int consumers_size;
  img_t data[1];
  img_t diff[1];
  img_t im[4];

#if 0
  int dim_[MAX_DIM];
  Dtype* data_;
  Dtype* diff_;
  Dtype* shape_data_;
  int capacity_;
#endif
};

typedef img_t pBox;

#ifdef HAVE_OPENCL1
bool used_gpu = 0;
#endif

buf_t bfbf(buf_t* bf)
{
  return *bf;
}

//#include "cv1/imgproc/imgwarp.inl"

// resize' : undeclared identifier
// cSize' : undeclared identifier
// cvGetSubRect' : undeclared identifier

//using namespace std;

struct udata_t;

typedef struct umem_t {
  struct udata_t* u;
  bool modifyp, modifyu;
  int datasize;
  Dtype* p;
} umem_t;

#ifdef HAVE_OPENCL1
bool umem_empty(const umem_t* a)
{
  return NULL == a || NULL == a->p;
}
#define UMEM_SETU(a)    (a->modifyp) ? umem_setu(a) : a->udata
#define UMEM_GETU(a)    (a->modifyu) ? umem_getu(a) : a->pdata
udata_t* umem_setu0(const umem_t* a)
{
  if (used_gpu) {
    ocl_device_t* dev = ocl_device_get();
    clEnqueueWriteBuffer(dev->cmdQueue, (cl_mem)a->u, CL_TRUE, 0, a->datasize, a->p, 0, NULL, NULL);
    ((umem_t*)a)->modifyp = 0;
  }

  return a->u;
}
CC_INLINE udata_t* umem_setu(const umem_t* a)
{
  if (a->modifyp && used_gpu) {
    tic;
    ocl_device_t* dev = ocl_device_get();
    clEnqueueWriteBuffer(dev->cmdQueue, (cl_mem)a->u, CL_TRUE, 0, a->datasize, a->p, 0, NULL, NULL);
    ((umem_t*)a)->modifyp = 0;
    toc;
  }

  return a->u;
}

CC_INLINE Dtype* umem_getu(const umem_t* a)
{
  if (a->modifyu && used_gpu) {
    tic;
    ocl_device_t* dev = ocl_device_get();
    clEnqueueReadBuffer(dev->cmdQueue, (cl_mem)a->u, CL_TRUE, 0, a->datasize, a->p, 0, NULL, NULL);
    ((umem_t*)a)->modifyu = 0;
    toc;
  }

  return a->p;
}
#else
#define umem_getu(a)   (a)
#define umem_setu(a)   (a)
#define umem_empty(a)  (a)==NULL
#define umem_setu0(a)  (a)
#endif

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

static double dist_L2(const float* a, const float* b, int n)
{
  double sum = 0;
  int i;

  for (i = 0; i < n; ++i) {
    double d = a[i] - b[i];
    sum += d * d;

    if (d * d > 0.000001) {
      int asdf = 0;
    }
  }

  sum /= n;
  sum = sqrt(sum);
  return sum;
}
#ifdef HAVE_OPENCL1
float* umem_map(const umem_t* a)
{
  ocl_device_t* dev = ocl_device_get();
  cl_int retval = 0;
  float* data = (float*)clEnqueueMapBuffer(dev->cmdQueue, (cl_mem)a->u,
      CL_TRUE, (CL_MAP_READ | CL_MAP_WRITE), 0, a->datasize, 0, 0, 0, &retval);
  return data;
}
float* umem_unmap(const umem_t* a, float* data)
{
  ocl_device_t* dev = ocl_device_get();
  cl_int retval = 0;
  clEnqueueUnmapMemObject(dev->cmdQueue, (cl_mem)a->u, data, 0, NULL, NULL);
  return data;
}

double umem_checku(const umem_t* a)
{
  ocl_device_t* dev = ocl_device_get();
  cl_int retval = 0;
  int n = a->datasize / sizeof(a->p[0]);
  float* data = (float*)clEnqueueMapBuffer(dev->cmdQueue, (cl_mem)a->u,
      CL_TRUE, (CL_MAP_READ | CL_MAP_WRITE), 0, a->datasize, 0, 0, 0, &retval);
  double dis = dist_L2(data, a->p, n);
  clEnqueueUnmapMemObject(dev->cmdQueue, (cl_mem)a->u, data, 0, NULL, NULL);
  return dis;
}

pBox {
  umem_t data[1];
  int flags;
  union{ int width; int w; };
  union{ int height; int h; };
  union{ int channel; int c; };
};
#endif


#define MAX_BLOB 16

#define LAYER_ENUM_ARGDEFDEF(LAYER_ENUM_ARGDEF) \
LAYER_ENUM_ARGDEF(Enum, null)  \
LAYER_ENUM_ARGDEF(Enum, full)  \
LAYER_ENUM_ARGDEF(Enum, same)  \
LAYER_ENUM_ARGDEF(Enum, valid)  \
LAYER_ENUM_ARGDEF(Enum, MAX)  \
LAYER_ENUM_ARGDEF(Enum, MAXI)  \
LAYER_ENUM_ARGDEF(Enum, DEF)  \
LAYER_ENUM_ARGDEF(Enum, DEF_ALL)  \
LAYER_ENUM_ARGDEF(Enum, DEF_ALL2)  \
LAYER_ENUM_ARGDEF(Enum, DEF_ALL3)  \
LAYER_ENUM_ARGDEF(Enum, DEF_ALL4)  \
LAYER_ENUM_ARGDEF(Enum, LOWRES)  \
LAYER_ENUM_ARGDEF(Enum, STOCHASTIC)  \
LAYER_ENUM_ARGDEF(Enum, AVERAGE)  \
LAYER_ENUM_ARGDEF(Enum, FAN_OUT)  \
LAYER_ENUM_ARGDEF(Enum, FAN_IN)  \
LAYER_ENUM_ARGDEF(Enum, AVE)  \
LAYER_ENUM_ARGDEF(Enum, fixed) \
LAYER_ENUM_ARGDEF(Enum, step) \
LAYER_ENUM_ARGDEF(Enum, exp) \
LAYER_ENUM_ARGDEF(Enum, inv) \
LAYER_ENUM_ARGDEF(Enum, multistep) \
LAYER_ENUM_ARGDEF(Enum, poly) \
LAYER_ENUM_ARGDEF(Enum, sigmoid) \
LAYER_ENUM_ARGDEF(Enum, L1) \
LAYER_ENUM_ARGDEF(Enum, L2) \
LAYER_ENUM_ARGDEF(Enum, Const)


#define LAYERARGDEFDEF(DEF) \
DEF(Int, kernel_h)  \
DEF(Int, kernel_w)  \
DEF(Int, stride_h)  \
DEF(Int, stride_w)  \
DEF(Int, num_output)  \
DEF(Int, pad)  \
DEF(Int, bias_term)  \
DEF(Int, transpose)  \
DEF(Int, axis)  \
DEF(Int, out_max_val)  \
DEF(Int, topk)  \
DEF(Int, dilation)  \
DEF(Int, group)  \
DEF(Int, data_size) \
DEF(Int, global) \
DEF(Int, weight_filler) \
DEF(Int, bias_filler) \
DEF(Float, weight_filler_std) \
DEF(Float, bias_filler_std) \
DEF(Float, scale) \
DEF(Float, slope) \
DEF(Enum, pool)


enum Method {
#define LAYERARGDEF(type, name)     Method_ ## name, 
LAYER_ENUM_ARGDEFDEF(LAYERARGDEF)
#undef LAYERARGDEF
};


struct IFArg {
  union {
    int i;
    int f;
    enum Method e;
  };
};

enum LayerArg {
#define LAYERARGDEF(type, name)     la_ ## name, 
LAYERARGDEFDEF(LAYERARGDEF)
#undef LAYERARGDEF
LayerArgCount
};

enum IFArgType {IF_Int, IF_Float, IF_Enum, IF_Index, IF_WIndex};
const static enum IFArgType ifArgType[] = {
#define LAYERARGDEF(type, name)     IF_ ## type, 
LAYERARGDEFDEF(LAYERARGDEF)
#undef LAYERARGDEF
};

#define BLOB(i)        (net->blobs+i)
#define ILBLOB(i)       (layer->blobs[i])
#define ITBLOB(i)       (layer->blobs[layer->bottoms_size+i])
#define IWBLOB(i)       (layer->blobs[layer->bottoms_size+layer->tops_size+i])

#define LBLOB(i)       (net->blobs+ILBLOB(i))
#define TBLOB(i)       (net->blobs+ITBLOB(i))
#define WBLOB(i)       (net->blobs+IWBLOB(i))
#define LARG0(name)   (0)
#define LLARG(name)   layer->name
#define LARG(name)   layer->x[la_ ## name].i
#define FARG(name)   layer->x[la_ ## name].f
#define EARG(name)   layer->x[la_ ## name].e
//#define LAYERARG(name)   int name = layer->x[la_ ## name];

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

static const char* layerarg_name[] = {
#define LAYERARGDEF(type, name)     #name , 
  LAYERARGDEFDEF(LAYERARGDEF)
#undef LAYERARGDEF
};
static enum LayerArg getLayerArg(const char* name, int len) {
  return (enum LayerArg)strlist_find(name, len, layerarg_name, countof(layerarg_name));
}

static const char* method_name[] = {
#define LAYERARGDEF(type, name)     #name , 
  LAYER_ENUM_ARGDEFDEF(LAYERARGDEF)
#undef LAYERARGDEF
};
static enum Method getMethod(const char* name, int len) {
  return (enum Method)strlist_find(name, len, method_name, countof(method_name));
}

enum LayerOp {LO_Init, LO_Forward, LO_Backward};
enum Phase {TRAIN, TEST};
#define Layer_init(net, layer)   (layer)->run(LO_Init, layer, net)
#define Layer_forward(net, layer)   (layer)->run(LO_Forward, layer, net)
#define Layer_backward(net, layer)   (layer)->run(LO_Backward, layer, net)
typedef int (*layer_run_t)(enum LayerOp op, const struct Layer* layer, struct Net* net);

struct LayerVTable {
  char* name;
  layer_run_t run;
};
typedef struct Layer {
  char name[MAX_NAME];
  IFArg x[LayerArgCount];
  int blobs[MAX_BLOB];
  int blobs_size;
  int bottoms_size;
  int tops_size;
  int weights_size;

  char* type;
  layer_run_t run;
  enum Phase phase_;
  //int propagate_down[MAX_BLOB];
  Dtype loss[MAX_BLOB];
} Layer;

struct Net
{
  Blob* blobs;
  int blobs_size;
  Layer *layers;
  int layers_size;
  bool debug_info;
  enum Phase phase;
  //uchar* buf;
  //int param_propagate_down[MAX_BLOB];
};

static bool setLayerFArg(IFArg* x, str_t s_key, str_t s_value) {
  enum LayerArg arg = getLayerArg(s_key.s, s_key.l);
  bool ret = false;
  if (arg<LayerArgCount) {
    ret = true;
    switch (ifArgType[arg]) {
    case IF_Index:
    case IF_WIndex:
    case IF_Int:
      x[arg].i = atoi(s_value.s);
      break;
    case IF_Float:
      x[arg].f = atof(s_value.s);
      break;
    case IF_Enum:
      x[arg].e = getMethod(s_value.s, s_value.l);
      break;
    default:
      ASSERT(0);
      break;
    }
  }
  return false;
}

static bool setLayerFArg(Layer* layer, str_t s_key, str_t s_value) {
  enum LayerArg arg = getLayerArg(s_key.s, s_key.l);
  bool ret = false;
  if (arg<LayerArgCount) {
    ret = true;
    switch (ifArgType[arg]) {
    case IF_Index:
    case IF_WIndex:
    case IF_Int:
      layer->x[arg].i = atoi(s_value.s);
      break;
    case IF_Float:
      layer->x[arg].f = atof(s_value.s);
      break;
    case IF_Enum:
      layer->x[arg].e = getMethod(s_value.s, s_value.l);
      break;
    default:
      ASSERT(0);
      break;
    }
  }
  return false;
}

void net_free(struct Net* net)
{
  int i;
  for (i=0; i<net->blobs_size; ++i) {
    imfree(net->blobs[i].data);
    imfree(net->blobs[i].diff);
    imfrees2(net->blobs[i].im);
  }
  FREE(net->blobs);
  FREE(net->layers);
  BZERO1(net);
}

struct pRelu {
  umem_t data[1];
  int w;
};

typedef struct Weight {
  umem_t data[1];
  umem_t bias[1];
  int selfChannel;
  int kernel_h;
  int kernel_w;
  int stride_h;
  int stride_w;
  int pad;
  float* kernel_tm;
} Weight;

static void freepRelu(struct pRelu* prelu)
{
  umem_free(prelu->data);
}

static void freeWeight(struct Weight* weight)
{
  umem_free(weight->data);
  umem_free(weight->bias);

  if (weight->kernel_tm) {
    FREE(weight->kernel_tm);
  }
}

#if 0
#define SETMODIFYP(a, x)   (a)->data->modifyp = (x)
CC_INLINE int pBox_channels(const pBox* a)
{
  return a->channel;
}
CC_INLINE float* pBox_datap(const pBox* a)
{
  return (float*)(a->data->p);
}
CC_INLINE int pBox_size(const pBox* a)
{
  return a->height * a->width * a->channel;
}

int pBoxsetsize(pBox* a, int h, int w, int c = 1)
{
  a->height = h;
  a->width = w;
  a->channel = c;
  int datasize = h * w * c * sizeof(float);
  umem_setsize(a->data, datasize);
  memset(a->data->p, 0, datasize);
  return 0;
}

CC_INLINE void freepBox(pBox* pbox)
{
  umem_free(pbox->data);
}
CC_INLINE void pBoxfrees(pBox* pbox, int n)
{
  for (int i = 0; i < n; ++i) {
    umem_free(pbox[i].data);
  }
}
#else
#define SETMODIFYP(a, x)   (0)

inline int pBox_num(const pBox* im) { return im->f; }

CC_INLINE int pBox_channels(const pBox* a)
{
  return a->c / sizeof(float);
}
CC_INLINE int pBox_length(const pBox* a)
{
  return (a->c * a->h * a->w) / sizeof(float);
}
CC_INLINE int pBox_pixnum(const pBox* a)
{
  return a->h * a->w;
}
CC_INLINE float* pBox_datap(const pBox* a)
{
  return (float*)(a->data);
}
CC_INLINE float* pBox_pf(const pBox* a)
{
  return (float*)(a->data);
}
CC_INLINE float* pBox_datapf(const pBox* a, int f)
{
  return (float*)(a->data + f * a->h * a->w * a->c);
}
CC_INLINE int pBox_size(const pBox* a)
{
  return a->f * a->height * a->width * a->c / sizeof(float);
}
CC_INLINE int pBox_count(const pBox* im) {
  return pBox_size(im);
}
CC_INLINE int pBoxsetsize1(pBox* a, int h, int w, int c, int f CC_DEFAULT(1))
{
  cvSetMats(a, h, w, CC_32F, c, f, NULL, CC_AUTOSTEP);
  return 0;
}
CC_INLINE int pBoxsetsize(pBox* a, int h, int w, int c CC_DEFAULT(1), int f CC_DEFAULT(1))
{
  cvSetMats(a, h, w, CC_32F, c, f, NULL, CC_AUTOSTEP);
  return 0;
}
CC_INLINE int pBox_create(pBox* a, int w, int h, int c CC_DEFAULT(1), int f CC_DEFAULT(1), const Dtype* data DEFAULT(NULL))
{
  cvSetMats(a, h, w, CC_32F, c, f, data, CC_AUTOSTEP);
  return 0;
}
CC_INLINE int pBox_ReshapeLike(pBox* im, const pBox* other) {
  return pBox_create(im, other->w, other->h, pBox_channels(other), pBox_num(other));
}
CC_INLINE int pBox_dims(const pBox* a)
{
  return (a->w>1) + (a->h>0) + (a->f>0);
}

CC_INLINE int bf_pBoxsetsize(buf_t* bf, pBox* a, int h, int w, int c CC_DEFAULT(1), int f CC_DEFAULT(1))
{
  bf_cvSetMats(bf, a, h, w, CC_32F, c, f, NULL, CC_AUTOSTEP);
  return 0;
}
inline char* pBox_shape_string(const pBox* im, char* buf, int len) {
  int count_ = pBox_count(im);
  _snprintf(buf, len, "%d %d %d %d(%d)", im->f, pBox_channels(im), im->h, im->w, count_);
  return buf;
}
CC_INLINE void freepBox(pBox* pbox)
{
  imfree(pbox);
}
CC_INLINE void pBoxfrees(pBox* pbox, int n)
{
  freeims(pbox, n);
}
#endif
#define pBox_total pBox_size
#define pBox_empty imempty
CC_INLINE size_t cvMatTotal(const img_t* mat)
{
  return pBox_size(mat);
}
CC_INLINE void pBox_fill(img_t* mat, float _v)
{
  int i = 0, _total = cvMatTotal(mat);
  float* data = pBox_datap(mat);

  for (; i < _total-3; i += 4) {
    data[i] = _v;
    data[i + 1] = _v;
    data[i + 2] = _v;
    data[i + 3] = _v;
  }

  for (; i < _total; i++) {
    data[i] = _v;
  }
}

CC_INLINE float* pBox_channel_p(const img_t* mat, int c)
{
  return ((float*)mat->data) + mat->h * mat->w * c;
}
CC_INLINE img_t pBox_channel(const img_t* mat, int c)
{
  img_t im = *mat;
  im.data = (uchar*)pBox_channel_p(mat, c);
  im.c = sizeof(float);
  return im;
}
CC_INLINE img_t pBox_frame(const img_t* mat, int f)
{
  img_t im = *mat;
  im.data = (uchar*)pBox_datapf(mat, f);
  im.f = 1;
  return im;
}
CC_INLINE float* pBox_at(const img_t* mat, int col, int row, int c, int f)
{
  return ((float*)mat->data) + ((f * mat->c/sizeof(float) + c) * mat->h + row) * mat->w + col;
}
CC_INLINE float* pBox_crow(const img_t* mat, int c, int row)
{
  return ((float*)mat->data) + mat->h * mat->w * c + mat->w * row;
}
CC_INLINE float* pBox_row0(const img_t* mat, int row)
{
  return ((float*)mat->data) + mat->w * row;
}
static void Blob_Update(Blob* blob) {
  blas_axpy(pBox_count(blob->data), (Dtype)(-1), blob->diff->tt.f4, blob->data->tt.f4);
}
static Dtype pBox_min(const pBox* im) {
  Dtype asum = 0;
  if (!imempty(im)) {
    asum = blas_min(pBox_count(im), im->tt.f4);
  }
  return asum;
}
static Dtype pBox_max(const pBox* im) {
  Dtype asum = 0;
  if (!imempty(im)) {
    asum = blas_max(pBox_count(im), im->tt.f4);
  }
  return asum;
}
static Dtype pBox_asum(const pBox* im) {
  Dtype asum = 0;
  if (!imempty(im)) {
    asum = blas_asum(pBox_count(im), im->tt.f4);
  }
  return asum;
}
static Dtype pBox_sumsq(const pBox* im) {
  Dtype sumsq = 0;
  if (!imempty(im)) {
    const Dtype* data = pBox_datap(im);
    sumsq = blas_dot(pBox_count(im), data, data);
  }
  return sumsq;
}
static void pBox_scale(pBox* im, Dtype scale_factor) {
  if (!imempty(im)) {
    Dtype* data = pBox_datap(im);
    blas_scal(pBox_count(im), scale_factor, data);
  }
}
static void pBox_clamp(const pBox* im, Dtype lower_bound, Dtype upper_bound) {
  if (!imempty(im)) {
    Dtype* data = pBox_datap(im);
    blas_clamp(pBox_count(im), lower_bound, upper_bound, data, data);
  }
}

static void substract_mean_normalize(int c, int h, int w, float* data, int cstep, const float* mean_vals, const float* norm_vals)
{
  int q, size = w * h;

  if (mean_vals && !norm_vals) {
    // substract mean only
    #pragma omp parallel for
    for (q = 0; q < c; q++) {
      float* ptr = data + cstep * q;
      const float mean = mean_vals[q];

#if __ARM_NEON
      int nn = size >> 2;
      int remain = size - (nn << 2);
#else
      int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
#if __aarch64__
      float32x4_t _mean = vdupq_n_f32(mean);

      for (; nn > 0; nn--) {
        float32x4_t _ptr = vld1q_f32(ptr);
        _ptr = vsubq_f32(_ptr, _mean);
        vst1q_f32(ptr, _ptr);
        ptr += 4;
      }

#else

      if (nn > 0) {
        asm volatile(
            "vdup.f32   q1, %4              \n"
            "0:                             \n"
            "pld        [%1, #128]          \n"
            "vld1.f32   {d0-d1}, [%1 :128]  \n"
            "vsub.f32   q0, q0, q1          \n"
            "subs       %0, #1              \n"
            "vst1.f32   {d0-d1}, [%1 :128]! \n"
            "bne        0b                  \n"
            : "=r"(nn),     // %0
            "=r"(ptr)     // %1
            : "0"(nn),
            "1"(ptr),
            "r"(mean)     // %4
            : "cc", "memory", "q0", "q1"
        );
      }

#endif // __aarch64__
#endif // __ARM_NEON

      for (; remain > 0; remain--) {
        *ptr -= mean;
        ptr++;
      }
    }
  }
  else if (!mean_vals && norm_vals) {
    // normalize only
    #pragma omp parallel for
    for (q = 0; q < c; q++) {
      float* ptr = data + cstep * q;
      const float norm = norm_vals[q];

#if __ARM_NEON
      int nn = size >> 2;
      int remain = size - (nn << 2);
#else
      int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
#if __aarch64__
      float32x4_t _norm = vdupq_n_f32(norm);

      for (; nn > 0; nn--) {
        float32x4_t _ptr = vld1q_f32(ptr);
        _ptr = vmulq_f32(_ptr, _norm);
        vst1q_f32(ptr, _ptr);
        ptr += 4;
      }

#else

      if (nn > 0) {
        asm volatile(
            "vdup.f32   q1, %4              \n"
            "0:                             \n"
            "pld        [%1, #128]          \n"
            "vld1.f32   {d0-d1}, [%1 :128]  \n"
            "vmul.f32   q0, q0, q1          \n"
            "subs       %0, #1              \n"
            "vst1.f32   {d0-d1}, [%1 :128]! \n"
            "bne        0b                  \n"
            : "=r"(nn),     // %0
            "=r"(ptr)     // %1
            : "0"(nn),
            "1"(ptr),
            "r"(norm)     // %4
            : "cc", "memory", "q0", "q1"
        );
      }

#endif // __aarch64__
#endif // __ARM_NEON

      for (; remain > 0; remain--) {
        *ptr *= norm;
        ptr++;
      }
    }
  }
  else if (mean_vals && norm_vals) {
    // substract mean and normalize
    #pragma omp parallel for
    for (q = 0; q < c; q++) {
      float* ptr = data + cstep * q;
      const float mean = mean_vals[q];
      const float norm = norm_vals[q];

#if __ARM_NEON
      int nn = size >> 2;
      int remain = size - (nn << 2);
#else
      int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
#if __aarch64__
      float32x4_t _mean = vdupq_n_f32(mean);
      float32x4_t _norm = vdupq_n_f32(norm);

      for (; nn > 0; nn--) {
        float32x4_t _ptr = vld1q_f32(ptr);
        _ptr = vsubq_f32(_ptr, _mean);
        _ptr = vmulq_f32(_ptr, _norm);
        vst1q_f32(ptr, _ptr);
        ptr += 4;
      }

#else

      if (nn > 0) {
        asm volatile(
            "vdup.f32   q1, %4              \n"
            "vdup.f32   q2, %5              \n"
            "0:                             \n"
            "pld        [%1, #128]          \n"
            "vld1.f32   {d0-d1}, [%1 :128]  \n"
            "vsub.f32   q0, q0, q1          \n"
            "vmul.f32   q0, q0, q2          \n"
            "subs       %0, #1              \n"
            "vst1.f32   {d0-d1}, [%1 :128]! \n"
            "bne        0b                  \n"
            : "=r"(nn),     // %0
            "=r"(ptr)     // %1
            : "0"(nn),
            "1"(ptr),
            "r"(mean),    // %4
            "r"(norm)     // %5
            : "cc", "memory", "q0", "q1", "q2"
        );
      }

#endif // __aarch64__
#endif // __ARM_NEON

      for (; remain > 0; remain--) {
        *ptr = (*ptr - mean) * norm;
        ptr++;
      }
    }
  }
}

// convert half precision floating point to float
static float half2float(unsigned short value)
{
  // 1 : 5 : 10
  unsigned short sign = (value & 0x8000) >> 15;
  unsigned short exponent = (value & 0x7c00) >> 10;
  unsigned short significand = value & 0x03FF;

  //     fprintf(stderr, "%d %d %d\n", sign, exponent, significand);

  // 1 : 8 : 23
  union {
    unsigned int u;
    float f;
  } tmp;

  if (exponent == 0) {
    if (significand == 0) {
      // zero
      tmp.u = (sign << 31);
    }
    else {
      // denormal
      exponent = 0;

      // find non-zero bit
      while ((significand & 0x200) == 0) {
        significand <<= 1;
        exponent++;
      }

      significand <<= 1;
      significand &= 0x3FF;
      tmp.u = (sign << 31) | ((-exponent + (-15 + 127)) << 23) | (significand << 13);
    }
  }
  else if (exponent == 0x1F) {
    // infinity or NaN
    tmp.u = (sign << 31) | (0xFF << 23) | (significand << 13);
  }
  else {
    // normalized
    tmp.u = (sign << 31) | ((exponent + (-15 + 127)) << 23) | (significand << 13);
  }

  return tmp.f;
}

static int from_float16(float* data32, const unsigned short* data16, int size)
{
  float* ptr = data32;

#if __ARM_NEON && (__ARM_FP & 2)
  int nn = cpu_support_arm_vfpv4() ? size >> 2 : 0;
  int remain = size - (nn << 2);
#else
  int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON && (__ARM_FP & 2)
#if __aarch64__

  if (nn > 0) {
    asm volatile(
        "0:                             \n"
        "ldr    d0, [%1], #8            \n"
        "fcvtl  v1.4s, v0.4h            \n"
        "subs   %w0, %w0, #1            \n"
        "str    q1, [%2], #16           \n"
        "bne    0b                      \n"
        : "=r"(nn),     // %0
        "=r"(data16),   // %1
        "=r"(ptr)     // %2
        : "0"(nn),
        "1"(data16),
        "2"(ptr)
        : "cc", "memory", "v0", "v1"
    );
  }

#else

  if (nn > 0) {
    asm volatile(
        "0:                             \n"
        "pld        [%1, #64]           \n"
        "vld1.s16   {d0}, [%1 :64]!     \n"
        "vcvt.f32.f16 q1, d0            \n"
        "subs       %0, #1              \n"
        "vst1.f32   {d2-d3}, [%2 :128]! \n"
        "bne        0b                  \n"
        : "=r"(nn),     // %0
        "=r"(data16),   // %1
        "=r"(ptr)     // %2
        : "0"(nn),
        "1"(data16),
        "2"(ptr)
        : "cc", "memory", "q0", "q1"
    );
  }

#endif // __aarch64__
#endif // __ARM_NEON

  for (; remain > 0; remain--) {
    *ptr = half2float(*data16);

    data16++;
    ptr++;
  }

  return 0;
}

static void copy_make_border_image(const img_t* src, img_t* dst, int top, int left, int type, float v)
{
  int w = dst->w;
  int h = dst->h;

  const float* ptr = pBox_datap(src);
  float* outptr = pBox_datap(dst);

  if (type == BORDER_CONSTANT) {
    int y = 0;

    // fill top
    for (; y < top; y++) {
      int x = 0;

      for (; x < w; x++) {
        outptr[x] = v;
      }

      outptr += w;
    }

    // fill center
    for (; y < (top + src->h); y++) {
      int x = 0;

      for (; x < left; x++) {
        outptr[x] = v;
      }

      for (; x < (left + src->w); x++) {
        outptr[x] = ptr[x - left];
      }

      for (; x < w; x++) {
        outptr[x] = v;
      }

      ptr += src->w;
      outptr += w;
    }

    // fill bottom
    for (; y < h; y++) {
      int x = 0;

      for (; x < w; x++) {
        outptr[x] = v;
      }

      outptr += w;
    }
  }
  else if (type == BORDER_REPLICATE) {
    int y = 0;

    // fill top
    for (; y < top; y++) {
      int x = 0;

      for (; x < left; x++) {
        outptr[x] = ptr[0];
      }

      for (; x < (left + src->w); x++) {
        outptr[x] = ptr[x - left];
      }

      for (; x < w; x++) {
        outptr[x] = ptr[src->w - 1];
      }

      outptr += w;
    }

    // fill center
    for (; y < (top + src->h); y++) {
      int x = 0;

      for (; x < left; x++) {
        outptr[x] = ptr[0];
      }

      for (; x < (left + src->w); x++) {
        outptr[x] = ptr[x - left];
      }

      for (; x < w; x++) {
        outptr[x] = ptr[src->w - 1];
      }

      ptr += src->w;
      outptr += w;
    }

    // fill bottom
    ptr -= src->w;

    for (; y < h; y++) {
      int x = 0;

      for (; x < left; x++) {
        outptr[x] = ptr[0];
      }

      for (; x < (left + src->w); x++) {
        outptr[x] = ptr[x - left];
      }

      for (; x < w; x++) {
        outptr[x] = ptr[src->w - 1];
      }

      outptr += w;
    }
  }
}

static img_t* copy_make_border(const img_t* src, img_t* dst, int top, int bottom, int left, int right, int type, float v)
{
  int w = src->w + left + right;
  int h = src->h + top + bottom;
  int channels, q;

  if (w == src->w && h == src->h) {
    return cvCloneMat(dst, src);
  }

  channels = pBox_channels(src);

  pBoxsetsize(dst, h, w, channels, 1);

  if (imempty(dst)) {
    return dst;
  }

  // unroll image channel
  #pragma omp parallel for

  for (q = 0; q < channels; q++) {
    const img_t m = pBox_channel(src, q);
    img_t borderm = pBox_channel(dst, q);

    copy_make_border_image(&m, &borderm, top, left, type, v);
  }

  return dst;
}

static img_t* copy_cut_border_image(const img_t* src, img_t* dst, int top, int left)
{
  int x, y;
  int w = dst->w;
  int h = dst->h;

  const float* ptr = pBox_at(src, left, top, 0, 0);
  float* outptr = pBox_datap(dst);

  for (y = 0; y < h; y++) {
    for (x = 0; x < w; x++) {
      outptr[x] = ptr[x];
    }

    outptr += w;
    ptr += src->w;
  }

  return dst;
}

static img_t* copy_cut_border(buf_t* bf, const img_t* src, img_t* dst, int top, int bottom, int left, int right)
{
  int q, channels;
  int w = src->w - left - right;
  int h = src->h - top - bottom;

  if (w == src->w && h == src->h) {
    return cvCloneMat(dst, src);
  }

  channels = pBox_channels(src);
  
  bf_pBoxsetsize(bf, dst, h, w, channels, 1);
  
  if (imempty(dst)) {
    return dst;
  }
  
  // unroll image channel
#pragma omp parallel for
  for (q = 0; q < channels; q++) {
    const img_t m = pBox_channel(src, q);
    img_t cutm = pBox_channel(dst, q);
    
    copy_cut_border_image(&m, &cutm, top, left);
  }
  
  return dst;
}

static img_t* resize_bilinear_image(const img_t* src, img_t* dst, int w, int h)
{
  double scale_x = (double)src->w / w;
  double scale_y = (double)src->h / h;

  int* buf = MALLOC(int, w + h + w * 2 + h * 2);

  int* xofs = buf;//new int[w];
  int* yofs = buf + w;//new int[h];

  float* alpha = (float*)(buf + w + h);//new float[w * 2];
  float* beta = (float*)(buf + w + h + w * 2); //new float[h * 2];

  float fx, fy;
  float* _rowsbuf, *rows0, *rows1;
  int sx, sy, dy, dx;
  int prev_sy1;

  for (dx = 0; dx < w; dx++) {
    fx = (float)((dx + 0.5) * scale_x - 0.5);
    sx = fx;//cvFloor(fx);
    fx -= sx;

    if (sx >= src->w - 1) {
      sx = src->w - 2;
      fx = 1.f;
    }

    xofs[dx] = sx;

    alpha[dx * 2    ] = 1.f - fx;
    alpha[dx * 2 + 1] = fx;
  }

  for (dy = 0; dy < h; dy++) {
    fy = (float)((dy + 0.5) * scale_y - 0.5);
    sy = fy;//cvFloor(fy);
    fy -= sy;

    if (sy >= src->h - 1) {
      sy = src->h - 2;
      fy = 1.f;
    }

    yofs[dy] = sy;

    beta[dy * 2    ] = 1.f - fy;
    beta[dy * 2 + 1] = fy;
  }

  // loop body
  _rowsbuf = MALLOC(float, 2 * (w + 1));
  rows0 = _rowsbuf;
  rows1 = _rowsbuf + w + 1;

  prev_sy1 = -1;

  for (dy = 0; dy < h; dy++) {
    int sy = yofs[dy];

    if (sy == prev_sy1) {
      // hresize one row
      float* rows1p;
      int dx = 0;
      const float* S1 = pBox_crow(src, 0, sy + 1);
      const float* alphap = alpha;
      float* rows0_old = rows0;
      rows0 = rows1;
      rows1 = rows0_old;
      rows1p = rows1;
#if __ARM_NEON

      for (; dx + 1 < w; dx += 2) {
        int sx = xofs[dx];
        int sxn = xofs[dx + 1];
        const float* S1p = S1 + sx;
        const float* S1np = S1 + sxn;

        float32x4_t _a = vld1q_f32(alphap);
        float32x2_t _S1 = vld1_f32(S1p);
        float32x2_t _S1n = vld1_f32(S1np);

        float32x4_t _S1S1n = vcombine_f32(_S1, _S1n);
        float32x4_t _ms1 = vmulq_f32(_S1S1n, _a);
        float32x2_t _rows1 = vpadd_f32(vget_low_f32(_ms1), vget_high_f32(_ms1));

        vst1_f32(rows1p + dx, _rows1);

        alphap += 4;
      }

#endif // __ARM_NEON

      for (; dx < w; dx++) {
        int sx = xofs[dx];
        const float* S1p = S1 + sx;

        float a0 = alphap[0];
        float a1 = alphap[1];
        rows1p[dx] = S1p[0] * a0 + S1p[1] * a1;

        alphap += 2;
      }
    }
    else {
      // hresize two rows
      const float* S0 = pBox_crow(src, 0, sy);
      const float* S1 = pBox_crow(src, 0, sy + 1);

      const float* alphap = alpha;
      float* rows0p = rows0;
      float* rows1p = rows1;
      int dx = 0;
#if __ARM_NEON

      for (; dx + 1 < w; dx += 2) {
        int sx = xofs[dx];
        int sxn = xofs[dx + 1];
        const float* S0p = S0 + sx;
        const float* S1p = S1 + sx;
        const float* S0np = S0 + sxn;
        const float* S1np = S1 + sxn;

        float32x4_t _a = vld1q_f32(alphap);
        float32x2_t _S0 = vld1_f32(S0p);
        float32x2_t _S1 = vld1_f32(S1p);
        float32x2_t _S0n = vld1_f32(S0np);
        float32x2_t _S1n = vld1_f32(S1np);

        float32x4_t _S0S0n = vcombine_f32(_S0, _S0n);
        float32x4_t _S1S1n = vcombine_f32(_S1, _S1n);
        float32x4_t _ms0 = vmulq_f32(_S0S0n, _a);
        float32x4_t _ms1 = vmulq_f32(_S1S1n, _a);
        float32x2_t _rows0 = vpadd_f32(vget_low_f32(_ms0), vget_high_f32(_ms0));
        float32x2_t _rows1 = vpadd_f32(vget_low_f32(_ms1), vget_high_f32(_ms1));

        vst1_f32(rows0p + dx, _rows0);
        vst1_f32(rows1p + dx, _rows1);

        alphap += 4;
      }

#endif // __ARM_NEON

      for (; dx < w; dx++) {
        int sx = xofs[dx];
        const float* S0p = S0 + sx;
        const float* S1p = S1 + sx;

        float a0 = alphap[0];
        float a1 = alphap[1];
        rows0p[dx] = S0p[0] * a0 + S0p[1] * a1;
        rows1p[dx] = S1p[0] * a0 + S1p[1] * a1;

        alphap += 2;
      }
    }

    prev_sy1 = sy + 1;

    {
    // vresize
    float b0 = beta[0];
    float b1 = beta[1];

    float* rows0p = rows0;
    float* rows1p = rows1;
    float* Dp = pBox_crow(dst, 0, dy);

#if __ARM_NEON
    int nn = w >> 3;
#else
    int nn = 0;
#endif
    int remain = w - (nn << 3);

#if __ARM_NEON
    float32x4_t _b0 = vdupq_n_f32(b0);
    float32x4_t _b1 = vdupq_n_f32(b1);

    for (; nn > 0; nn--) {
      float32x4_t _rows0 = vld1q_f32(rows0p);
      float32x4_t _rows1 = vld1q_f32(rows1p);

      float32x4_t _D = vmulq_f32(_rows0, _b0);
      _D = vmlaq_f32(_D, _rows1, _b1);

      vst1q_f32(Dp, _D);

      float32x4_t _rows0n = vld1q_f32(rows0p + 4);
      float32x4_t _rows1n = vld1q_f32(rows1p + 4);

      float32x4_t _Dn = vmulq_f32(_rows0n, _b0);
      _Dn = vmlaq_f32(_Dn, _rows1n, _b1);

      vst1q_f32(Dp + 4, _Dn);

      Dp += 8;
      rows0p += 8;
      rows1p += 8;
    }

#endif // __ARM_NEON

    for (; remain; --remain) {
      //             D[x] = rows0[x]*b0 + rows1[x]*b1;
      *Dp++ = *rows0p++ * b0 + *rows1p++ * b1;
    }


    }
    beta += 2;
  }

  FREE(_rowsbuf);
  FREE(buf);
  return dst;
}

static img_t* resize_bilinear(buf_t* bf, const img_t* src, img_t* dst, int w, int h)
{
  int channels, q;
  if (w == src->w && h == src->h) {
    return cvCloneMat(dst, src);
  }
  
  channels = pBox_channels(src);
  
  bf_pBoxsetsize(bf, dst, h, w, channels, 1);
  
  if (imempty(dst)) {
    return dst;
  }
  
  // unroll image channel
#pragma omp parallel for
  
  for (q = 0; q < channels; q++) {
    const img_t m = pBox_channel(src, q);
    img_t resizem = pBox_channel(dst, q);
    
    resize_bilinear_image(&m, &resizem, w, h);
  }
  
  return dst;
}

// Fillers are random number generators that fills a blob using the specified
// algorithm. The expectation is that they are only going to be used during
// initialization time and will not involve any GPUs.
/// @brief Fills a Blob with constant or randomly-generated data.

/// @brief Fills a Blob with constant values @f$ x = 0 @f$.
static void pBox_ConstantFiller(pBox* im, Dtype value) {
  Dtype* data = pBox_datap(im);
  const int count = pBox_count(im);
  CHECK(count);
  for (int i = 0; i < count; ++i) {
    data[i] = value;
  }
}

/// @brief Fills a Blob with uniformly distributed values @f$ x\sim U(a, b) @f$.

static void pBox_UniformFiller(pBox* im, Dtype _min, Dtype _max) {
  Dtype* data = pBox_datap(im);
  const int count = pBox_count(im);
  CHECK(count);
  blas_rng_uniform(count, _min, _max, data);
}

/// @brief Fills a pBox with Gaussian-distributed values @f$ x = a @f$.
static void pBox_GaussianFiller(pBox* im, Dtype _mean, Dtype _std, int sparse DEFAULT(-1)) {
  Dtype* data = pBox_datap(im);
  const int count = pBox_count(im);
  CHECK(count);
  blas_rng_gaussian(count, _mean, _std, data);
  CHECK_GE(sparse, -1);
  if (sparse >= 0) {
    // Sparse initialization is implemented for "weight" blobs; i.e. matrices.
    // These have num == channels == 1; width is number of inputs; height is
    // number of outputs.  The 'sparse' variable specifies the mean number
    // of non-zero input weights for a given output.
    const int num_outputs = im->f;
    Dtype non_zero_probability = (Dtype)(sparse) / (Dtype)(num_outputs);
    for (int i = 0; i < count; ++i) {
      int mask = rng_bernoulli(NULL, non_zero_probability);
      data[i] *= mask;
    }
  }
}

static void pBox_PositiveUnitballFiller(pBox* im) {
  Dtype* data = pBox_datap(im);
  int i, j, count = pBox_count(im);
  blas_rng_uniform(count, 0, 1, data);
  // We expect the filler to not be called very frequently, so we will
  // just use a simple implementation
  int dim = count / im->f;
  for (i = 0; i < im->f; ++i) {
    Dtype sum = 0;
    for (j = 0; j < dim; ++j) {
      sum += data[i * dim + j];
    }
    for (j = 0; j < dim; ++j) {
      data[i * dim + j] /= sum;
    }
  }
}

static void pBox_XavierFiller(img_t* im, Method variance_norm DEFAULT(Method_FAN_IN)) {
  Dtype* data = pBox_datap(im);
  int count = pBox_count(im);
  int fan_in = count / im->f;
  int fan_out = count / pBox_channels(im);
  Dtype n = fan_in;  // default to fan_in
  if (variance_norm == Method_AVERAGE) {
    n = (fan_in + fan_out) / Dtype(2);
  } else if (variance_norm == Method_FAN_OUT) {
    n = fan_out;
  }
  Dtype scale = sqrt((Dtype)(3) / n);
  blas_rng_uniform(count, -scale, scale, data);
}


static void pBox_MSRAFiller(img_t* im, Method variance_norm, Dtype alpha) {
  Dtype* data = pBox_datap(im);
  int count = pBox_count(im);
  int fan_in = count / im->f;
  int fan_out = count / pBox_channels(im);
  Dtype n = fan_out;  // default to fan_in
  if (variance_norm == Method_AVERAGE) {
    n = (fan_in + fan_out) / (Dtype)(2);
  } else if (variance_norm == Method_FAN_IN) {
    n = fan_in;
  }
  Dtype std = sqrt((Dtype)(2) / n / (1 + alpha * alpha));
  blas_rng_gaussian(count, (0), std, data);
}

static void pBox_BilinearFiller(img_t* im) {
  Dtype* data = pBox_datap(im);
  int count = pBox_count(im);
  //CHECK_EQ(blob->num_axes(), 4) << "Blob must be 4 dim.";
  //CHECK_EQ(blob->width(), blob->height()) << "Filter must be square";
  int f = ceil(im->w / 2.);
  float c = (2 * f - 1 - f % 2) / (2. * f);
  for (int i = 0; i < count; ++i) {
    float x = i % im->w;
    float y = (i / im->w) % im->h;
    data[i] = (1 - fabs(x / f - c)) * (1 - fabs(y / f - c));
  }
}
static void pBox_GaussianUnitBallFiller(img_t* im) {
  Dtype* data = pBox_datap(im);
  int count = pBox_count(im);
  int fan_in = count / im->f;
  int n = fan_in;  // default to fan_in
  blas_rng_gaussian(count, (0), 1, data);
  Dtype sum_sq;
  for (int i = 0; i < im->f; i++) {
    sum_sq = blas_dot(n, data + i * n, data + i * n) + 1e-12;
    blas_scale(n, Dtype(1.0) / sqrt(sum_sq), data + i * n, data + i * n);
  }
}
