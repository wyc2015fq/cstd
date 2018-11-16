

//#include "log.h"
#include "wstd/logging.hpp"
#include "parser/cJSON.hpp"
#include "cpu.hpp"
#include <map>
#include <iostream>
#include <fstream>
#include "types.h"
#include "math_functions.h"
#include "math_functions.inl"


int debug_info_ = 0;

void Lock() {}
void Unlock() {}
void InitMutex() {}



inline string DataShape_string(const DataShape& shape) {
  char buf[256]="[";
  char buf2[32];
  int n = shape.num_axes();
  for (int i = 0; i < n; ++i) {
    if (i) strcat(buf, ",");
    strcat(buf, _itoa(shape.dim[i], buf2, 10));
  }
  strcat(buf, "(");
  strcat(buf, _itoa(shape.count(), buf2, 10));
  strcat(buf, ")]");
  return buf;
}
inline DataShape dataShape(int n, int c = 0, int h = 0, int w = 0) {
  DataShape shape;
  shape.set(n, c, h, w);
  return shape;
}

enum MemStage { UNINIT = 0, AT_CPU = 1, AT_GPU = 2, SYNCED = 3 };

struct DevMem {
  Buffer cpu_ptr_[1];
  Buffer gpu_ptr_[1];
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

  void* to(Brew brew, MemStage state) {
    if (CPU == brew) {
      switch (state_) {
      case UNINIT:
        ReAlloc(brew, cpu_ptr_, nbytes_);
        Memset(cpu_ptr_, nbytes_);
        state_ = AT_CPU;
        break;
      case AT_GPU:
#ifndef CPU_ONLY
        ReAlloc(brew, cpu_ptr_, nbytes_);
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
    else if (GPU == brew) {
#ifndef CPU_ONLY
      void* new_ptr_ = nullptr;
      switch (state_) {
      case UNINIT:
        ReAlloc(brew, gpu_ptr_, nbytes_);
        Memset(gpu_ptr_, nbytes_);
        state_ = AT_GPU;
        break;
      case AT_CPU:
        ReAlloc(brew, gpu_ptr_, nbytes_);
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
    return NULL;
  }

  void* cpu_ptr() {    return to(CPU, SYNCED);  }
  void* gpu_ptr() {    return to(GPU, SYNCED);  }
  void* cpu_mptr() {    return to(CPU, AT_CPU);  }
  void* gpu_mptr() {    return to(GPU, AT_GPU);  }

  void* ptr(Brew brew) { return to(brew, SYNCED); }
  void* mptr(Brew brew) { return to(brew, brew==CPU ? AT_CPU : AT_GPU); }

  void reset(size_t size) {
    nbytes_ = size;
    state_ = UNINIT;
  }
};


enum { MAX_NAME = 64 };
struct Blob {
  char name[MAX_NAME];
  typedef float Dtype;
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
    this->Reshape(dataShape(1));
  }
  Blob(int n, int c=0, int h=0, int w=0) {
    init();
    this->Reshape(n, c, h, w);
  }
  Blob(const std::vector<int>& shape) {
    init();
    this->Reshape(shape);
  }
  Blob(const DataShape& shape) {
    init();
    this->Reshape(shape);
  }
  void init() {
    memset(this, 0, sizeof(Blob));
    shape_.set(1);
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

  const Dtype* data() { return (const Dtype*)(data_)->ptr(BREW); }
  const Dtype* diff() { return (const Dtype*)(diff_)->ptr(BREW); }
  Dtype* mdata() { return (Dtype*)(data_)->mptr(BREW); }
  Dtype* mdiff() { return (Dtype*)(diff_)->mptr(BREW); }

  const Dtype* cpu_data() { return (Dtype*)data_->cpu_ptr(); }
  const Dtype* cpu_diff() { return (Dtype*)diff_->cpu_ptr(); }
  Dtype* cpu_mdata() { return (Dtype*)data_->cpu_mptr(); }
  Dtype* cpu_mdiff() { return (Dtype*)diff_->cpu_mptr(); }

  const Dtype* gpu_data() { return (Dtype*)data_->gpu_ptr(); }
  const Dtype* gpu_diff() { return (Dtype*)diff_->gpu_ptr(); }
  Dtype* gpu_mdata() { return (Dtype*)data_->gpu_mptr(); }
  Dtype* gpu_mdiff() { return (Dtype*)diff_->gpu_mptr(); }

  int Reshape(const DataShape& shape) {
    //if (shape_ != shape)
    {
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
  void CopyFrom(Blob* other, bool copy_diff = false, bool reshape = true) {
    if (reshape) { Reshape(other->shape()); }
    caffe_copy(count(), other->data(), mdata());
    if (copy_diff) {
      caffe_copy(count(), other->diff(), mdiff());
    }
  }
  void CopyTo(Blob* other, bool copy_diff) {
    other->Reshape(shape());
    caffe_copy(count(), data(), other->mdata());
    if (copy_diff) {
      caffe_copy(count(), diff(), other->mdiff());
    }
  }
  // this->data = other->data
  void ShareData(Blob* other) {
    assert(count()==other->count());
    caffe_copy(count(), other->data(), mdata());
  }
  void ShareDiff(Blob* other) {
    assert(count() == other->count());
    caffe_copy(count(), other->diff(), mdiff());
  }
#include "blob.inl"
};

#define SetString(name, def, type)  name##_ = (def);
#define SetBool(name, def, type)  name##_ = (def);
#define SetFloat(name, def, type)  name##_ = (def);
#define SetInt(name, def, type)  name##_ = (def);
#define SetEnum(name, def, type)  name##_ = (def);
#define SetStruct(name, def, type)  name##_.init();

#define GetString(name, def, type)  name##_ = param->getstring(#name, def);
#define GetBool(name, def, type)  name##_ = param->getbool(#name, def);
#define GetFloat(name, def, type)  name##_ = param->getfloat(#name, def);
#define GetInt(name, def, type)  name##_ = param->getint(#name, def);
#define GetEnum(name, def, type)  name##_ = param->getenum(#name, def, type##_Name, countof(type##_Name));
#define GetStruct(name, def, type)  name##_.init(param->get(#name ));

#define DefString(name, def, type)  string name##_;
#define DefBool(name, def, type)  bool name##_;
#define DefFloat(name, def, type)  double name##_;
#define DefInt(name, def, type)  int name##_;
#define DefEnum(name, def, type)  type name##_;
#define DefStruct(name, def, type)  type name##_;

static void ConstantFiller(Blob* blob, double x = 1) {
  cpu_ConstantFiller(blob->shape_, blob->cpu_mdata(), x);
}
static void UniformFiller(Blob* blob, double min = 0, double max = 1) {
  cpu_UniformFiller(blob->shape_, blob->cpu_mdata(), min, max);
}
static void GaussianFiller(Blob* blob, double mu = 0, double st = 1) {
  cpu_GaussianFiller(blob->shape_, blob->cpu_mdata(), mu, st, -1);
}

#define FILLER_PARAM_DEF(DEF) \
DEF##Enum(type, FillerMethod_constant, FillerMethod) \
DEF##Float(value, 0, 0) \
DEF##Float(min, 0, 0) \
DEF##Float(max, 1, 0) \
DEF##Float(mean, 0, 0) \
DEF##Float(std, 1, 0) \
DEF##Float(sparse, -1, 0) \
DEF##Enum(variance_norm, FAN_IN, VarianceNorm)

struct Filler {
  FILLER_PARAM_DEF(Def);
  typedef Blob::Dtype Dtype;
  typedef double Stype;
  Filler() {
    init();
  }
  void init() {
    FILLER_PARAM_DEF(Set);
  }
  void init(CJSON* param) {
    FILLER_PARAM_DEF(Get);
  }
  int Fill(Blob* blob) {
    //cpu_Filler(blob->shape_, blob->cpu_mdata(), param);
    DataShape shape = blob->shape_;
    Dtype* data = blob->cpu_mdata();
    //Blob::Dtype* data = blob->cpu_mdata();
    switch(type_) {
    case FillerMethod_constant:
      CHECK_EQ(sparse_, -1)
        << "Sparsity not supported by this Filler.";
      return cpu_ConstantFiller(shape, data, value_);
    case FillerMethod_gaussian:
      return (cpu_GaussianFiller)(shape, data, mean_, std_, sparse_);
    
    case FillerMethod_positive_unitball:
      CHECK_EQ(sparse_, -1) << "Sparsity not supported by this Filler.";
      return (cpu_PositiveUnitballFiller)(shape, data);
    
    case FillerMethod_uniform:
      CHECK_EQ(sparse_, -1) << "Sparsity not supported by this Filler.";
      return (cpu_UniformFiller)(shape, data, min_, max_);
    
    case FillerMethod_xavier:
      CHECK_EQ(sparse_, -1) << "Sparsity not supported by this Filler.";
      return (cpu_XavierFiller)(shape, data, variance_norm_);
    
    case FillerMethod_msra:
      CHECK_EQ(sparse_, -1) << "Sparsity not supported by this Filler.";
      return (cpu_MSRAFiller)(shape, data, variance_norm_);
    
    case FillerMethod_bilinear:
      CHECK_EQ(sparse_, -1)
        << "Sparsity not supported by this Filler.";
      return (cpu_BilinearFiller)(shape, data);
    
    default:
      CHECK(false) << "Unknown filler name: " << type_;
      break;
    }
    return 0;
  }

};

static int Fill(Blob* blob, Filler* filler) {
  return filler->Fill(blob);
}

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


 static void logBlob(Blob* B, string fileName)
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
 
static void logBlobs(const vector<Blob*>& blobs, const char* fileName) {
  char blobStr[256];
  for (int i = 0; i < blobs.size(); ++i) {
    _snprintf(blobStr,256, "%s_%d", fileName, i);
    logBlob(blobs[i], blobStr);
  }
}


static void blobs_reset(vector<Blob*>& blobs_, int blob_size) {
  int blob_size_ = blobs_.size();
  for (; blob_size_>blob_size; ) {
    --blob_size_;
    delete blobs_[blob_size_];
    blobs_[blob_size_] = NULL;
  }
  blobs_.resize(blob_size);
  for (; blob_size_<blob_size; ++blob_size_) {
    blobs_[blob_size_] = new Blob();
  }
}

int blobs_count(const vector<Blob*>& blobs_, const char* name) {
  int i, count = 0;
  for (i = 0; i < blobs_.size(); ++i) {
    if (0 == strcmp(name, blobs_[i]->name)) { ++count; }
  }
  return count;
}

Blob* blobs_get(const vector<Blob*>& blobs_, const char* name) {
  int i;
  for (i = 0; i < blobs_.size(); ++i) {
    if (0 == strcmp(name, blobs_[i]->name)) { return blobs_[i]; }
  }
  return NULL;
}


Blob* blobs_add(vector<Blob*>& blobs_, const char* name) {
  Blob* new_blob = new Blob();
  strncpy(new_blob->name, name, MAX_NAME);
  blobs_.push_back(new_blob);
  return new_blob;
}

Blob* blobs_aget(vector<Blob*>& blobs_, const char* name) {
  Blob* bi = NULL;
  if (blobs_count(blobs_, name) == 0) {
    bi = blobs_add(blobs_, name);
  }
  else {
    bi = blobs_get(blobs_, name);
  }
  return bi;
}

struct Layer {
  Phase phase_;
  int phase_mask_;
  char name_[MAX_NAME];
  char type_[MAX_NAME];
  //cJSON* param_;
  typedef Blob::Dtype Dtype;
  vector<Blob*> blobs_;
  vector<Blob*> bottom_vecs_;
  vector<Blob*> top_vecs_;
  virtual inline const char* type() const { return ""; }
  virtual inline int ExactNumBottomBlobs() const { return -1; }
  virtual inline int MinBottomBlobs() const { return -1; }
  virtual inline int MaxBottomBlobs() const { return -1; }
  virtual inline int ExactNumTopBlobs() const { return -1; }
  virtual inline int MinTopBlobs() const { return -1; }
  virtual inline int MaxTopBlobs() const { return -1; }
  virtual inline bool EqualNumBottomTopBlobs() const { return false; }
  virtual inline bool AutoTopBlobs() const { return false; }
  virtual void LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top) { }
  virtual void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top) { }
  // bottom -> top
  virtual void Forward_(const vector<Blob*> & bottom, const vector<Blob*> & top) { (0); }
  // top -> bottom
  virtual void Backward_(const vector<Blob*> & top, const vector<Blob*> & bottom) { (0); }

  Layer() {
    init();
  }
  //Vtbl* vtbl;
  void init() {
    //memset(this, 0, sizeof(Layer));
    *name_ = 0;
    *type_ = 0;
    //vtbl = NULL;
    phase_ = TEST;
    phase_mask_ = (1 << TRAIN) | (1 << TEST);
    phase_mask_ = 7;
  }
  void Free() {
    reset(0);
  }
  void reset(int blob_size) {
    blobs_reset(blobs_, blob_size);
  }
  typedef Layer* (*fun_type)(cJSON* param);

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
  static int AppendName(CJSON* param, Layer* layer, bool is_top, vector<Blob*>& net_blobs_) {
    vector<string> vec;
    const char* name = is_top ? "top" : "bottom";
    cJSON_GetObjectStringArray(param, name, vec);
    Blob* bi;
    vector<Blob*>& blobvec = is_top ? layer->top_vecs_ : layer->bottom_vecs_;
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

#include "layer.inl"
};

//#define REG_LAYER(name) Layer::fun_type f ## Bias = Layer::reg(new ## Bias, #name );

#define INSTANTIATE_CLASS2(name, Bias) Layer* new ## Bias ## Layer(cJSON* param) { Bias ## Layer* p = new Bias ## Layer(); p->init(param); return p;} \
Layer::fun_type f ## Bias = Layer::reg(&new ## Bias ## Layer, #name );

#define INSTANTIATE_CLASS(Bias) INSTANTIATE_CLASS2(Bias, Bias)
#define REGISTER_LAYER_CLASS INSTANTIATE_CLASS
//Layer<double>::fun_type d ## Bias = Layer<double>::reg(&new ## Bias ## Layer<double>, #Bias )
//#define REGISTER_LAYER_CLASS(Bias)  

int CreateLayer(CJSON* param, Layer*& layer, const char* type) {
  Layer::fun_type fun = Layer::reg(NULL, type);
  CHECK(fun) << "CreateLayer can not find unkown layer type = " << type;
  if (NULL == fun) {
    return 0;
  }
  layer = fun(param);
  //layer->type_ = (char*)type;
  strcpy(layer->type_, type);
  return 1;
}


struct Net {
  cJSON* param_;
  typedef Layer::Dtype Dtype;
  vector<Layer* > layers_;
  vector<Blob* > blobs_;
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
     // Layer* layer = layers_[layer_size_] = Layer::reg(NULL);
      //layer->init();
      //layers_[layer_size_].init();
    }
  }
#include "net.inl"
};

#include "layers/layers.hpp"

#undef SetString
#undef SetBool
#undef SetFloat
#undef SetInt
#undef SetEnum
#undef SetStruct

#undef GetString
#undef GetBool
#undef GetFloat
#undef GetInt
#undef GetEnum
#undef GetStruct

#undef DefString
#undef DefBool
#undef DefFloat
#undef DefInt
#undef DefEnum
#undef DefStruct
