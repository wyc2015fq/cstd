

//#include "log.h"
#include "wstd/logging.hpp"
#include "parser/cJSON.hpp"
#include "cpu.hpp"
#include <map>
#include "types.h"

int debug_info_ = 0;

struct DevMem {
  CPUContext cpu_ptr_[1];
  GPUContext gpu_ptr_[1];
  int nbytes_;
  int state_;
  enum {UNINIT = 0, AT_CPU = 1<<CPU, AT_GPU = 1<<GPU, SYNCED = 3};
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

  void* to_cpu(int state) {
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
    case SYNCED:
      break;
    }
    return cpu_ptr_->data;
  }

  void* to_gpu(int state) {
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
    case SYNCED:
      break;
    }
#endif
    return gpu_ptr_->data;
  }

  void* cpu_ptr() {    return to_cpu(SYNCED);  }
  void* gpu_ptr() {    return to_gpu(SYNCED);  }
  void* mutable_cpu_ptr() {    return to_cpu(AT_CPU);  }
  void* mutable_gpu_ptr() {    return to_gpu(AT_GPU);  }

  template <typename Context>  void* to(int state);
  template <typename Context>  void* ptr();
  template <typename Context>  void* mutable_ptr();

  void reset(size_t size) {
    nbytes_ = size;
    state_ = UNINIT;
  }
};

template <>void* DevMem::to<CPUContext>(int state) { return to_cpu(state); }
template <>void* DevMem::to<GPUContext>(int state) { return to_gpu(state); }
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
  union {
    DataShape shape_;
    struct { int n, c, h, w; };
  };
  DevMem data_[1];
  DevMem diff_[1];
  bool propagate_down;
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
    propagate_down = true;
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
  const Dtype* gpu_data() { return (Dtype*)data_->gpu_ptr(); }
  const Dtype* gpu_diff() { return (Dtype*)diff_->gpu_ptr(); }
  Dtype* mutable_gpu_data() { return (Dtype*)data_->mutable_gpu_ptr(); }
  Dtype* mutable_gpu_diff() { return (Dtype*)diff_->mutable_gpu_ptr(); }
#endif
  int Reshape(const DataShape& shape) {
    int nbytes = shape.count() * sizeof(Dtype);
    data_->reset(nbytes);
    data_->reset(nbytes);
    shape_ = shape;
    return 0;
  }
  const DataShape& shape() const { return shape_; }
  void ReshapeLike(const Blob & other) {
    Reshape(other.shape());
  }
  void CopyTo(Blob & other) const {
    other.Reshape(shape());
    caffe_copy(CONTEXT, count(), data<Context>(), other.data<Context>());
  }
  void ShareData(Blob & other) const {
    CopyTo(other);
  }
#include "blob.inl"
};
enum Phase {
  TRAINorTEST, TRAIN, TEST
};
const char* Phase_Name[]{
  "TRAINorTEST", "TRAIN", "TEST"
};

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
int blobs_count(const vector<Blob<Dtype>*>& blobs_, const string& name) {
  int i, count = 0;
  for (i = 0; i < blobs_.size(); ++i) {
    if (0 == strcmp(name.c_str(), blobs_[i]->name)) { ++count; }
  }
  return count;
}
template <typename Dtype>
Blob<Dtype>* blobs_get(const vector<Blob<Dtype>*>& blobs_, const string& name) {
  int i;
  for (i = 0; i < blobs_.size(); ++i) {
    if (0 == strcmp(name.c_str(), blobs_[i]->name)) { return blobs_[i]; }
  }
  return NULL;
}
template <typename Dtype>
Blob<Dtype>* blobs_add(vector<Blob<Dtype>*>& blobs_, const string& name) {
  Blob<Dtype>* new_blob = new Blob<Dtype>();
  strncpy(new_blob->name, name.c_str(), MAX_NAME);
  blobs_.push_back(new_blob);
  return new_blob;
}

template <typename Dtype>
struct Layer {
  Phase phase_;
  char name[MAX_NAME];
  int has_loss_weights_;
  cJSON* param_;
  vector<Blob<Dtype>*> blobs_;
  vector<Blob<Dtype>*> bottom_vecs_;
  vector<Blob<Dtype>*> top_vecs_;
  virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top) { ASSERT(0); }
  virtual void Reshape(const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top) { ASSERT(0); }
  virtual void Forward(Context* context, const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top) { ASSERT(0); }
  virtual void Backward(Context* context, const vector<Blob<Dtype>*> & top, const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom) { ASSERT(0); }

  //Vtbl* vtbl;
  void init() {
    //memset(this, 0, sizeof(Layer));
    *name = 0;
    has_loss_weights_ = false;
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

#define REGISTER_LAYER_CLASS(Bias)  

template <typename Dtype>
struct Net {
  cJSON* param_;
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
#include "math_functions.cpp"
#include "filler.hpp"
#include "layers/layers.hpp"

