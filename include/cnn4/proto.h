

//#include "log.h"
#include "wstd/logging.hpp"
#include "parser/cJSON.hpp"
#include "cpu.hpp"
#include <map>
#include "types.h"

int debug_info_ = 0;

inline string DataShape_string(const DataShape& shape_) {
  char buf[256];
  const int* p = shape_.dim;
  _snprintf(buf, 256, "(%d %d %d %d)", p[0], p[1], p[2], p[3]);
  return buf;
}
inline DataShape dataShape(int n, int c = 1, int h = 1, int w = 1) {
  DataShape shape;
  shape.set(n, c, h, w);
  return shape;
}

enum MemStage { UNINIT = 0, AT_CPU = 1, AT_GPU = 2, SYNCED = 3 };

struct DevMem {
  CPUContext cpu_ptr_[1];
  GPUContext gpu_ptr_[1];
  int nbytes_;
  MemStage state_;
  void free() {
    Free(cpu_ptr_);
    Free(gpu_ptr_);
    state_ = UNINIT;
    nbytes_ = 0;
  }
  void init() {
    memset(this, 0, sizeof(*this));
    state_ = UNINIT;
    nbytes_ = 0;
  }
  DevMem() { init(); }
  ~DevMem() { free(); }

  void* to_cpu(MemStage state) {
    switch (state_) {
    case UNINIT:
      ReAlloc(cpu_ptr_, nbytes_);
      Memset(cpu_ptr_, nbytes_);
      state_ = AT_CPU;
      break;
    case AT_GPU:
#ifndef CPU_ONLY
      ReAlloc(cpu_ptr_, nbytes_);
      Memcpy(cpu_ptr_, gpu_ptr_, nbytes_);
      state_ = state;
#endif
      break;
    case AT_CPU:
      break;
    case SYNCED:
      state_ = state;
      break;
    }
    return cpu_ptr_->data;
  }

  void* to_gpu(MemStage state) {
#ifndef CPU_ONLY
    void* new_ptr_ = nullptr;
    switch (state_) {
    case UNINIT:
      ReAlloc(gpu_ptr_, nbytes_);
      Memset(gpu_ptr_, nbytes_);
      state_ = AT_GPU;
      break;
    case AT_CPU:
      ReAlloc(gpu_ptr_, nbytes_);
      Memcpy(gpu_ptr_, cpu_ptr_, nbytes_);
      state_ = state;
      break;
    case AT_GPU:
      break;
    case SYNCED:
      state_ = state;
      break;
    }
#endif
    return gpu_ptr_->data;
  }

  void* cpu_ptr() {    return to_cpu(SYNCED);  }
  void* gpu_ptr() {    return to_gpu(SYNCED);  }
  void* mutable_cpu_ptr() {    return to_cpu(AT_CPU);  }
  void* mutable_gpu_ptr() {    return to_gpu(AT_GPU);  }

  template <typename Context>  void* to(MemStage state);
  template <typename Context>  void* ptr();
  template <typename Context>  void* mutable_ptr();

  void reset(size_t size) {
    nbytes_ = size;
    state_ = UNINIT;
  }
};

template <>void* DevMem::to<CPUContext>(MemStage state) { return to_cpu(state); }
template <>void* DevMem::to<GPUContext>(MemStage state) { return to_gpu(state); }
template <>void* DevMem::ptr<CPUContext>() { return to_cpu(SYNCED); }
template <>void* DevMem::ptr<GPUContext>() { return to_gpu(SYNCED); }
template <>void* DevMem::mutable_ptr<CPUContext>() { return to_cpu(AT_CPU); }
template <>void* DevMem::mutable_ptr<GPUContext>() { return to_gpu(AT_GPU); }

enum { MAX_NAME = 64 };
template <typename Dtype>
struct Blob {
  char name[MAX_NAME];
  Dtype loss_weight_;
  Dtype loss_;
  Dtype lr_mult_;
  Dtype decay_mult_;
  union {
    DataShape shape_;
    struct { int n, c, h, w; };
  };
  DevMem data_[1];
  DevMem diff_[1];
  bool propagate_down_;
  int top_cnt_;
  int bottom_cnt_;
  int offset(int i) {
    return i*c*h*w;
  }

  int count() const { return shape_.count(); }
  Blob() {
    init();
  }
  void init() {
    memset(this, 0, sizeof(Blob));
    bottom_cnt_ = top_cnt_ = 0;
    propagate_down_ = true;
    loss_weight_ = 0;
  }
  ~Blob() {
    free();
  }
  void free() {
    data_->free();
    diff_->free();
  }

  template <typename Context> const Dtype* data() const { return (Dtype*)((DevMem*)data_)->ptr<Context>(); }
  template <typename Context> const Dtype* diff() const { return (Dtype*)((DevMem*)diff_)->ptr<Context>(); }
  template <typename Context> Dtype* mutable_data() { return (Dtype*)(data_)->mutable_ptr<Context>(); }
  template <typename Context> Dtype* mutable_diff() { return (Dtype*)(diff_)->mutable_ptr<Context>(); }
#if 0
  const Dtype* cpu_data() { return (Dtype*)data_->cpu_ptr(); }
  const Dtype* cpu_diff() { return (Dtype*)diff_->cpu_ptr(); }
  Dtype* mutable_cpu_data() { return (Dtype*)data_->mutable_cpu_ptr(); }
  Dtype* mutable_cpu_diff() { return (Dtype*)diff_->mutable_cpu_ptr(); }
  const Dtype* data<Context>() { return (Dtype*)data_->gpu_ptr(); }
  const Dtype* gpu_diff() { return (Dtype*)diff_->gpu_ptr(); }
  Dtype* mutable_data<Context>() { return (Dtype*)data_->mutable_gpu_ptr(); }
  Dtype* mutable_gpu_diff() { return (Dtype*)diff_->mutable_gpu_ptr(); }
#endif
  int Reshape(const DataShape& shape) {
    if (shape_ != shape) {
      int nbytes = shape.count() * sizeof(Dtype);
      data_->reset(nbytes);
      diff_->reset(nbytes);
      shape_ = shape;
    }
    return 0;
  }
  int Reshape(int n, int c, int h, int w) {
    return Reshape(dataShape(n,c,h,w));
  }
  const DataShape& shape() const { return shape_; }
  void ReshapeLike(const Blob & other) {
    if (this != &other) {
      Reshape(other.shape());
    }
  }
  void CopyFrom(const Blob & other, bool copy_diff) {
    Reshape(other.shape());
    caffe_copy<Dtype>(CONTEXT, count(), other.data<Context>(), mutable_data<Context>());
    if (copy_diff) {
      caffe_copy<Dtype>(CONTEXT, count(), other.diff<Context>(), mutable_diff<Context>());
    }
  }
  void CopyTo(Blob & other, bool copy_diff) const {
    other.Reshape(shape());
    caffe_copy<Dtype>(CONTEXT, count(), data<Context>(), other.mutable_data<Context>());
    if (copy_diff) {
      caffe_copy<Dtype>(CONTEXT, count(), diff<Context>(), other.mutable_diff<Context>());
    }
  }
  void ShareData(Blob & other) const {
    CopyTo(other, false);
  }
#include "blob.inl"
};

bool dirExists(string dirStr)
{
  //     const char* dirCStr = dirStr.c_str();
  //     DIR* dir = opendir(dirCStr);
  //     if (ENOENT == errno){
  //       return false;
  //     }
  //     closedir(dir);
  return true;
}

void tryCreateDirectory(string fileName)
{
  //     vector<string> strVec;
  //     boost::split(strVec,fileName,boost::is_any_of("/"));
  //     string newStr="";
  //     for (int i=0;i<strVec.size()-1;++i){
  //       newStr += strVec[i] + (i==strVec.size()-2?"":"/");
  //     }
  //     boost::filesystem::path dirToCreate(newStr);
  //     if (!dirExists(newStr)){
  //       boost::filesystem::create_directories(dirToCreate);
  //     }
}


template <typename Dtype> static
void logBlob(Blob<Dtype>* B, string fileName)
{
  string dataNameStr = fileName + "_data.txt";
  string gradNameStr = fileName + "_grad.txt";
  const char* dataName = (dataNameStr).c_str();
  const char* gradName = (gradNameStr).c_str();
  tryCreateDirectory(dataName);
  tryCreateDirectory(gradName);
  std::ofstream outWriter_data(dataName, std::ofstream::out);
  std::ofstream outWriter_grad(gradName, std::ofstream::out);
  for (int n = 0; n < B->shape(0); ++n) {
    for (int c = 0; c < B->shape(1); ++c) {
      for (int h = 0; h < B->shape(2); ++h) {
        for (int w = 0; w < B->shape(3); ++w) {
          outWriter_data << B->data_at(n, c, h, w) << ",";
          outWriter_grad << B->diff_at(n, c, h, w) << ",";
        }
      }
    }
  }
  outWriter_data << std::endl;
  outWriter_grad << std::endl;
}

template <typename Dtype> static
void logBlobs(const vector<Blob<Dtype>*>& blobs, string fileName) {
  for (int i = 0; i < blobs.size(); ++i) {
    string blobStr = fileName + "_" + itos(i);
    logBlob(blobs[i], blobStr);
  }
}

template <typename Dtype>
static void blobs_reset(vector<Blob<Dtype>*>& blobs_, int blob_size) {
  int blob_size_ = blobs_.size();
  for (; blob_size_>blob_size; ) {
    --blob_size_;
    delete blobs_[blob_size_];
    blobs_[blob_size_] = NULL;
  }
  blobs_.resize(blob_size);
  for (; blob_size_<blob_size; ++blob_size_) {
    blobs_[blob_size_] = new Blob<Dtype>();
  }
}
template <typename Dtype>
int blobs_count(const vector<Blob<Dtype>*>& blobs_, const char* name) {
  int i, count = 0;
  for (i = 0; i < blobs_.size(); ++i) {
    if (0 == strcmp(name, blobs_[i]->name)) { ++count; }
  }
  return count;
}
template <typename Dtype>
Blob<Dtype>* blobs_get(const vector<Blob<Dtype>*>& blobs_, const char* name) {
  int i;
  for (i = 0; i < blobs_.size(); ++i) {
    if (0 == strcmp(name, blobs_[i]->name)) { return blobs_[i]; }
  }
  return NULL;
}

template <typename Dtype>
Blob<Dtype>* blobs_add(vector<Blob<Dtype>*>& blobs_, const char* name) {
  Blob<Dtype>* new_blob = new Blob<Dtype>();
  strncpy(new_blob->name, name, MAX_NAME);
  blobs_.push_back(new_blob);
  return new_blob;
}
template <typename Dtype>
Blob<Dtype>* blobs_aget(vector<Blob<Dtype>*>& blobs_, const char* name) {
  Blob<Dtype>* bi = NULL;
  if (blobs_count(blobs_, name) == 0) {
    bi = blobs_add(blobs_, name);
  }
  else {
    bi = blobs_get(blobs_, name);
  }
  return bi;
}

template <typename Dtype>
struct Layer {
  Phase phase_;
  char name[MAX_NAME];
  cJSON* param_;
  vector<Blob<Dtype>*> blobs_;
  vector<Blob<Dtype>*> bottom_vecs_;
  vector<Blob<Dtype>*> top_vecs_;
  virtual inline const char* type() const { return ""; }
  virtual inline int ExactNumBottomBlobs() const { return -1; }
  virtual inline int MinBottomBlobs() const { return -1; }
  virtual inline int MaxBottomBlobs() const { return -1; }
  virtual inline int ExactNumTopBlobs() const { return -1; }
  virtual inline int MinTopBlobs() const { return -1; }
  virtual inline int MaxTopBlobs() const { return -1; }
  virtual inline bool EqualNumBottomTopBlobs() const { return false; }
  virtual inline bool AutoTopBlobs() const { return false; }
  virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top) { }
  virtual void Reshape(const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top) { }
  virtual void Forward(Context* context, const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top) { ASSERT(0); }
  virtual void Backward(Context* context, const vector<Blob<Dtype>*> & top, const vector<Blob<Dtype>*> & bottom) { ASSERT(0); }

  //Vtbl* vtbl;
  void init() {
    //memset(this, 0, sizeof(Layer));
    *name = 0;
    //vtbl = NULL;
    phase_ = TRAINorTEST;
  }
  void Free() {
    reset(0);
  }
  void reset(int blob_size) {
    blobs_reset(blobs_, blob_size);
  }
  typedef Layer<Dtype>* (*fun_type)();

  static fun_type reg(fun_type fun, const char* type) {
    static map<string, fun_type>  fmap;
    if (fun) {
      fmap[type] = fun;
    }
    if (type) {
      fun = fmap[type];
    }
    return fun;
  }
#include "layer.inl"
};

#define INSTANTIATE_CLASS(Bias) template <typename Dtype>Layer<Dtype>* new ## Bias ## Layer() {return new Bias ## Layer<Dtype>();} \
Layer<float>::fun_type f ## Bias = Layer<float>::reg(&new ## Bias ## Layer<float>, #Bias );
//Layer<double>::fun_type d ## Bias = Layer<double>::reg(&new ## Bias ## Layer<double>, #Bias )
//#define REGISTER_LAYER_CLASS(Bias)  

template <typename Dtype>
int CreateLayer(Layer<Dtype>*& layer, const char* type) {
  Layer<Dtype>::fun_type fun = Layer<Dtype>::reg(NULL, type);
  CHECK(fun) << "CreateLayer can not find unkown layer type = " << type;
  if (NULL == fun) {
    return 0;
  }
  layer = fun();
  return 1;
}

template <typename Dtype>
static int AppendName(Layer<Dtype>* layer, bool is_top, vector<Blob<Dtype>*>& net_blobs_) {
  vector<string> vec;
  const char* name = is_top ? "top" : "bottom";
  cJSON_GetObjectStringArray(layer->param_, name, vec);
  Blob<Dtype>* bi;
  vector<Blob<Dtype>*>& blobvec = is_top ? layer->top_vecs_ : layer->bottom_vecs_;
  for (int i = 0; i < vec.size(); ++i) {
    bi = blobs_aget(net_blobs_, vec[i].c_str());
    if (is_top) {
      bi->top_cnt_++;
    }
    else {
      bi->bottom_cnt_++;
    }
    blobvec.push_back(bi);
  }
  return 0;
}


struct Net {
  cJSON* param_;
  typedef float Dtype;
  vector<Layer<Dtype>* > layers_;
  vector<Blob<Dtype>* > blobs_;
  int size() { return layers_.size(); }
  Net() {
    init();
  }
  void init() {
    param_ = NULL;
    reset(0);
  }
  void Free() {
    reset(0);
    init();
  }
  void reset(int layer_size) {
    int layer_size_ = layers_.size();
    for (; layer_size_>layer_size; ) {
      --layer_size_;
      delete layers_[layer_size_];
      layers_[layer_size_] = NULL;
    }
    layers_.resize(layer_size);
    for (; layer_size_<layer_size; ++layer_size_) {
     // Layer<Dtype>* layer = layers_[layer_size_] = Layer<Dtype>::reg(NULL);
      //layer->init();
      //layers_[layer_size_].init();
    }
  }
#include "net.inl"
};

#include "math_functions.h"
#include "math_functions.inl"
#include "filler.hpp"
#include "layers/layers.hpp"

