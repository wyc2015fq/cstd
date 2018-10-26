
//#include "log.h"
#include "wstd/logging.hpp"
#include "parser/cJSON.hpp"
#include "cpu.hpp"
#include <map>

int debug_info_ = 0;

struct DataShape {
  int dim[4];
  void set(int a, int b = 1, int c = 1, int d = 1) { dim[0] = a, dim[1] = b, dim[2] = c, dim[3] = d; }
  int count() const {    return dim[0]* dim[1]* dim[2]* dim[3];  }
};

struct DevMem {
  union {
    struct {
      void* cpu_ptr_;
      void* gpu_ptr_;
    };
    //void* ptr_[2];
    int* i32_[2];
    float* f32_[2];
    double* f64_[2];
    typedef float(*f32_10_t)[100];
    f32_10_t f32_10_[2];
  };
  int nbytes_;
  int state_;
  enum {UNINIT = 0, AT_CPU = 1<<CPU, AT_GPU = 1<<GPU, SYNCED = 3};
  void free() {
    if (cpu_ptr_) {
      CPUContext::Free(cpu_ptr_);
      cpu_ptr_ = 0;
    }
    if (gpu_ptr_) {
      GPUContext::Free(gpu_ptr_);
      gpu_ptr_ = 0;
    }
    
    state_ = UNINIT;
    nbytes_ = 0;
  }
  void init() {
    cpu_ptr_  = gpu_ptr_ = 0;
    state_ = UNINIT;
    nbytes_ = 0;
  }
  DevMem() { init(); }
  ~DevMem() { free(); }

  void* to_cpu(int state) {
    switch (state_) {
    case UNINIT:
      cpu_ptr_ = CPUContext::ReAlloc(cpu_ptr_, nbytes_);
      CPUContext::Memset(nbytes_, cpu_ptr_);
      state_ = AT_CPU;
      break;
    case AT_GPU:
#ifdef USE_GPU
      cpu_ptr_ = CPUContext::ReAlloc(cpu_ptr_, nbytes_);
      GPUContext::Memcpy(nbytes_, cpu_ptr_, gpu_ptr_);
      state_ = state;
#endif
      break;
    case AT_CPU:
    case SYNCED:
      break;
    }
    return cpu_ptr_;
  }

  void* to_gpu(int state) {
#ifdef USE_GPU
    void* new_ptr_ = nullptr;
    switch (state_) {
    case UNINIT:
      gpu_ptr_ = GPUContext::ReAlloc(gpu_ptr_, nbytes_);
      GPUContext::Memset(nbytes_, gpu_ptr_);
      state_ = AT_GPU;
      break;
    case AT_CPU:
      gpu_ptr_ = GPUContext::ReAlloc(gpu_ptr_, nbytes_);
      GPUContext::Memcpy(nbytes_, gpu_ptr_, cpu_ptr_);
      state_ = state;
      break;
    case AT_GPU:
    case SYNCED:
      break;
    }
#endif
    return gpu_ptr_;
  }

  void* cpu_ptr() {    return to_cpu(SYNCED);  }
  void* gpu_ptr() {    return to_gpu(SYNCED);  }
  void* mutable_cpu_ptr() {    return to_cpu(AT_CPU);  }
  void* mutable_gpu_ptr() {    return to_gpu(AT_GPU);  }

  template <typename Context>  void* to(int state);
  template <typename Context>  void* ptr();
  template <typename Context>  void* mutable_ptr();

  void reset(size_t size) {
    if (size > nbytes_) {
      free();
      nbytes_ = size;
      state_ = UNINIT;
    }
  }
};

template <>void* DevMem::to<CPUContext>(int state) { return to_cpu(state); }
template <>void* DevMem::to<GPUContext>(int state) { return to_gpu(state); }
template <>void* DevMem::ptr<CPUContext>() { return to_cpu(SYNCED); }
template <>void* DevMem::ptr<GPUContext>() { return to_gpu(SYNCED); }
template <>void* DevMem::mutable_ptr<CPUContext>() { return to_cpu(AT_CPU); }
template <>void* DevMem::mutable_ptr<GPUContext>() { return to_gpu(AT_GPU); }

template <typename Dtype>
struct Blob {
  char name[32];
  DataShape shape_;
  DevMem data_[1];
  DevMem diff_[1];
  int count() { return shape_.count(); }
  Blob() {
    init();
  }
  void init() {
    memset(this, 0, sizeof(Blob));
  }
  ~Blob() {
    free();
  }
  void free() {
    data_->free();
    diff_->free();
  }

  template <typename Context>
  const Dtype* data() { return (Dtype*)data_->cpu_ptr(); }

  const Dtype* cpu_data() { return (Dtype*)data_->cpu_ptr(); }
  const Dtype* cpu_diff() { return (Dtype*)diff_->cpu_ptr(); }
  Dtype* mutable_cpu_data() { return (Dtype*)data_->mutable_cpu_ptr(); }
  Dtype* mutable_cpu_diff() { return (Dtype*)diff_->mutable_cpu_ptr(); }
  const Dtype* gpu_data() { return (Dtype*)data_->gpu_ptr(); }
  const Dtype* gpu_diff() { return (Dtype*)diff_->gpu_ptr(); }
  Dtype* mutable_gpu_data() { return (Dtype*)data_->mutable_gpu_ptr(); }
  Dtype* mutable_gpu_diff() { return (Dtype*)diff_->mutable_gpu_ptr(); }
  int Reshape(DataShape shape) {
    int nbytes = shape.count() * sizeof(Dtype);
    data_->reset(nbytes);
    data_->reset(nbytes);
    shape_ = shape;
    return 0;
  }
  int FromProto(CJSON* proto) {
    Blob<Dtype>* blob = this;
    DataShape shape;
    cJSON_GetObjectNumberArray(proto, "shape", shape.dim, 4, 1);
    int count = shape.count();
    int nbytes = count * sizeof(float);
    blob->Reshape(shape);
    if (proto->GetObjectItem("data")) {
      float* data = blob->mutable_cpu_data();
      nbytes = blob->shape_.count();
      cJSON_GetObjectBinaryData(proto, "data", data, nbytes);
    }
    if (proto->GetObjectItem("diff")) {
      float* data = blob->mutable_cpu_diff();
      cJSON_GetObjectBinaryData(proto, "diff", data, count);
    }
    return 0;
  }

};
enum Phase {
  TRAINorTEST, TRAIN, TEST
};
const char* Phase_Name[]{
  "TRAINorTEST", "TRAIN", "TEST"
};

template <typename Dtype>
static void blob_reset(vector<Blob<Dtype>*>& blobs_, int blob_size) {
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
struct Layer {
  Phase phase_;
  char name[32];
  double loss_;
  virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top) = 0;
  virtual void Reshape(const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top) = 0;
  virtual void Forward_cpu(const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top) = 0;
  virtual void Forward_gpu(const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top) = 0;
  virtual void Backward_cpu(const vector<Blob<Dtype>*> & top, const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom) = 0;

  cJSON* param_;
  vector<Blob<Dtype>*> blobs_;
  //Vtbl* vtbl;
  void init() {
    vtbl = NULL;
  }
  void Free() {
    reset(0);
  }
  void reset(int blob_size) {
    blob_reset(blobs_, blob_size);
  }
#include "layer.inl"

};
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
      //layers_[layer_size_].init();
    }
  }
#include "net.inl"
};


#include "math_functions.h"
#include "math_functions.cpp"
