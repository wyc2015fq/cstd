
#ifndef _FACEIDENTIFICATION_INL_
#define _FACEIDENTIFICATION_INL_

#define MYREALLOC2(a, len)  MYREALLOC(a, len), MEMSET(a, 0, len)


#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string.h>
#include <math.h>
#include <vector>
#include <memory>
#include <algorithm>
#include <map>


#if defined(__unix__) || defined(__APPLE__)
#define _BEGIN_INFO_  "\033[1;32m"
#define _BEGIN_ERROR_ "\033[1;31m"
#define _BEGIN_DEBUG_ "\033[1;33m"
#define _END_COLOR_   "\033[0m"

#define ERROR _BEGIN_ERROR_ "[ERROR] "
#define DEBUG _BEGIN_DEBUG_ "[DEBUG] "
#define INFO _BEGIN_INFO_   "[INFO ] "
#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : __FILE__)
#else
#define _BEGIN_INFO_ "LOG_COLOR_G"
#define _BEGIN_ERROR_ "LOG_COLOR_R"
#define _BEGIN_DEBUG_ "LOG_COLOR_Y"
#define _END_COLOR_ "LOG_COLOR_W"


#define ERROR _BEGIN_ERROR_ "[ERROR] "
#define DEBUG _BEGIN_DEBUG_ "[DEBUG] "
#define INFO _BEGIN_INFO_   "[INFO ] "
#define __FILENAME__ (strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1) : __FILE__)
#endif // __unix__, __APPLE__

class ViplLog {
 public:
  template<class T>
  ViplLog(const T &options) {
    my_cout_ << options;
  }
  ~ViplLog();
  template<class T>
  inline ViplLog &operator << (const T &x) {
#ifdef __VIPL_LOG__
    my_cout_ << x;
#endif
    return *this;
  }
 private:
  std::ostringstream my_cout_;
};

#define LOG(OPTION) ViplLog(OPTION) << "[" << __FILENAME__ << ":" << __LINE__ << "] " <<  _END_COLOR_

// check A == B
#define CHECK_EQ(A, B) if ((A) != (B)) LOG(ERROR) << "Check failed:" << "("    \
<< #A << " == " << #B  << ") " << "(" << A << " vs. " << B <<") " << "Inputs " \
<< #A<<" must be equal " << #B << "."

// check A != B
#define CHECK_NE(A, B) if ((A) == (B)) LOG(ERROR) << "Check failed:" << "("    \
<< #A << " != " << #B  << ") " << "(" << A << " vs. " << B <<") " << "Inputs " \
<< #A<<" must be not equal " << #B << "."

// check A < B
#define CHECK_LT(A, B) if ((A) >= (B)) LOG(ERROR) << "Check failed:" << "("    \
<< #A << " < " << #B  << ") " << "(" << A << " vs. " << B <<") " << "Inputs "  \
<< #A <<" must be less than " << #B <<"."

// check A > B
#define CHECK_GT(A, B) if ((A) <= (B)) LOG(ERROR) << "Check failed:" << "("    \
<< #A << " > " << #B << ") " << "(" << A << " vs. " << B <<") " << "Inputs "   \
<< #A << " must be great than " << #B << "."

// check A <= B
#define CHECK_LE(A, B) if ((A) > (B)) LOG(ERROR) << "Check failed:" << "("     \
 << #A << " <= " << #B  << ") " << "(" << A << " vs. " << B <<") " << "Inputs "\
<< #A <<" must be less than or equal to " << #B <<"."

// check A >= B
#define CHECK_GE(A, B) if ((A) < (B)) LOG(ERROR) << "Check failed:" << "("     \
<< #A << " >= " << #B << ") " << "(" << A << " vs. " << B <<") " << "Inputs "  \
<< #A << " must be great than or equal to " << #B << "."

// check int A is near to int B
#define CHECK_INT_NEAR(A, B, C) if (abs(A-B) > C) LOG(ERROR) << "Check failed:"\
<< #A << " is not near to " << #B << " within the range " << #C <<"."

// check double A is neart to double B
#define CHECK_DOUBLE_NEAR(A, B, C) if (fabs(A-B) > C) LOG(ERROR)               \
<< "Check failed: "  << #A << " is not near to " << #B << " within the range " \
<< #C <<"."

// check A is true
#define CHECK_TRUE(A) if (!(A)) LOG(ERROR) << "Check failed:" << "(" << #A     \
<< " Must be true" << ")."

#if defined(__unix__) || defined(__APPLE__)
#ifndef fopen_s
#define fopen_s(pFile,filename,mode) ((*(pFile))=fopen((filename),(mode)))==NULL
#endif //fopen_s
#endif //__unix__, __APPLE__


ViplLog::~ViplLog() {
#ifdef __VIPL_LOG__
#ifdef _WIN32
  std::string out_str = my_cout_.str();
  std::string sys_cmd = "color 0";
  std::string log_color_str = "LOG_COLOR_";
  std::size_t s_idx = 0, p = out_str.find(log_color_str);
  std::size_t log_color_str_size = log_color_str.size();
  while (p != std::string::npos) {
    std::cout << out_str.substr(s_idx, p - s_idx);
    HANDLE hdl = GetStdHandle(STD_OUTPUT_HANDLE);
    WORD f_color;
    if (out_str[p + log_color_str_size] == 'R') {
      f_color = FOREGROUND_RED;
    }
    else if (out_str[p + log_color_str_size] == 'G') {
      f_color = FOREGROUND_GREEN;
    }
    else if (out_str[p + log_color_str_size] == 'W') {
      f_color = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
    }
    else if (out_str[p + log_color_str_size] == 'Y') {
      f_color = FOREGROUND_RED | FOREGROUND_GREEN;
    }
    SetConsoleTextAttribute(hdl, f_color | FOREGROUND_INTENSITY);
    s_idx = p + log_color_str_size + 1;
    p = out_str.find(log_color_str, s_idx);
  }
  std::cout << out_str.substr(s_idx) << std::endl;
#else
  std::cout << my_cout_.str() << std::endl;
#endif // _WIN32
#endif // __VIPL_LOG__
}




#ifdef _BLAS
#ifdef _WIN64
#pragma comment( lib, "blas_win64_MT" )
#pragma comment( lib, "lapack_win64_MT" )
#else
#pragma comment( lib, "libblas" )
#pragma comment( lib, "liblapack" )
#endif
#include <armadillo>
#endif

#include <xmmintrin.h>
//#include <cstdint>

#ifdef _WIN32
//#include <intrin.h>
#else
#include <x86intrin.h>
#endif

float simd_dot(const float* x, const float* y, const long& len) {
  float inner_prod = 0.0f;
  __m128 X, Y; // 128-bit values
  __m128 acc = _mm_setzero_ps(); // set to (0, 0, 0, 0)
  float temp[4];

  long i;
  for (i = 0; i + 4 < len; i += 4) {
      X = _mm_loadu_ps(x + i); // load chunk of 4 floats
      Y = _mm_loadu_ps(y + i);
      acc = _mm_add_ps(acc, _mm_mul_ps(X, Y));
  }
  _mm_storeu_ps(&temp[0], acc); // store acc into an array
  inner_prod = temp[0] + temp[1] + temp[2] + temp[3];

  // add the remaining values
  for (; i < len; ++i) {
      inner_prod += x[i] * y[i];
  }
  return inner_prod;
}

void matrix_procuct(const float* A, const float* B, float* C, const int n,
    const int m, const int k, bool ta, bool tb) {
#ifdef _BLAS
  arma::fmat mA = ta ? arma::fmat(A, k, n).t() : arma::fmat(A, n, k);
  arma::fmat mB = tb ? arma::fmat(B, m, k).t() : arma::fmat(B, k, m);
  arma::fmat mC(C, n, m, false);
  mC = mA * mB;
#else
  CHECK_TRUE(ta && !tb);
  const float* x = B;
  for (int i = 0, idx = 0; i < m; ++i) {
    const float* y = A;
    for (int j = 0; j < n; ++j, ++idx) {
      C[idx] = simd_dot(x, y, k);
      y += k;
    }
    x += k;
  }
#endif
}


class SeetaBlob {
 public:
  SeetaBlob();
  SeetaBlob(const SeetaBlob &source);
  SeetaBlob(int n, int c, int h, int w);
  SeetaBlob(int n, int c, int h, int w, float* data);
  SeetaBlob(FILE* file);

  virtual ~SeetaBlob();
  void reshape(int n, int c, int h, int w);
  void Permute(int dim1, int dim2, int dim3, int dim4);
  void Release();
  inline int offset(const int n, const int c=0, const int h=0,
      const int w=0) const {
    if (n < 0 || n >= num() || c < 0 || c >= channels() ||
        h < 0 || h >= height() || w < 0 || w >= width()) {
      LOG(ERROR)
        << "Index: (" << n << ", " << c << ", " << h << ", " << w << "), "
        << "Bound: [" << num() << ", " << channels() << ", " << height()
        <<", " << width() << "].";
      exit(0);
    }
    return ((n * channels() + c) * height() + h) * width() + w;
  }
  void SetData();
  void SetData(const SeetaBlob &source);
  void SetData(int n, int c, int h, int w);
  void CopyData(int n, int c, int h, int w, const float* const data);
  // copy data from unsigned char
  void CopyDataU8(int n, int c, int h, int w, const unsigned char* const  data);
  // copy data to unsigned char
  void CopyTo(unsigned char* const data);
  // copy data to float
  void CopyTo(float* const data);
  void ToFile(const char* file_name);
  void ToBinaryFile(const char* file_name);

  inline const float operator [](int i) const {
    return data_[i];
  }
  inline float & operator [](int i) {
    return data_[i];
  }

  inline int num() const { return shape(0); }
  inline int channels() const { return shape(1); }
  inline int height() const { return shape(2); }
  inline int width() const { return shape(3); }
  inline std::vector<int> shape() const { return shape_; }
  int count() const { return count_; }
  float* data() const {
    return data_; }
 private:
  inline int shape(int index) const {
    return index < shape_.size() ? shape_[index] : 1;
  }
  float* data_;
  std::vector<int> shape_;
  int count_;
};

#define nullptr NULL
SeetaBlob::SeetaBlob() {
  data_ = nullptr;
  shape_.clear();
}

SeetaBlob::SeetaBlob(const SeetaBlob &source) {
  data_ = nullptr;
  SetData(source);
}

SeetaBlob::SeetaBlob(int n, int c, int h, int w) {
  data_ = nullptr;
  SetData(n, c, h, w);
}

SeetaBlob::SeetaBlob(int n, int c, int h, int w, float* data) {
  data_ = nullptr;
  CopyData(n, c, h, w, data);
}

SeetaBlob::SeetaBlob(FILE* file) {
  int n, c, h, w;
  data_ = nullptr;
  CHECK_EQ(fread(&(n), sizeof(int), 1, file), 1);
  CHECK_EQ(fread(&(c), sizeof(int), 1, file), 1);
  CHECK_EQ(fread(&(h), sizeof(int), 1, file), 1);
  CHECK_EQ(fread(&(w), sizeof(int), 1, file), 1);
  CopyData(n, c, h, w, NULL);
  CHECK_EQ(fread(data_, sizeof(float), count_, file), count_);
}

SeetaBlob::~SeetaBlob() {
  shape_.clear();
  count_ = 0;
  FREE(data_);
  data_ = nullptr;
}

void SeetaBlob::reshape(int n, int c, int h, int w) {
  SetData(n, c, h, w);
}

void SeetaBlob::Permute(int dim1, int dim2, int dim3, int dim4) {
  // todo: check permute
  std::vector<int> dim(4), redim(4), idx(4);
  dim[0] = dim1 - 1; redim[dim[0]] = 0;
  dim[1] = dim2 - 1; redim[dim[1]] = 1;
  dim[2] = dim3 - 1; redim[dim[2]] = 2;
  dim[3] = dim4 - 1; redim[dim[3]] = 3;
  float* tmp = NULL;
  float* dat = data_;
  MYREALLOC(tmp, count_);
  int cnt = 0;
  for (idx[0] = 0; idx[0] < shape_[dim[0]]; ++ idx[0]) {
    for (idx[1] = 0; idx[1] < shape_[dim[1]]; ++ idx[1]) {
      for (idx[2] = 0; idx[2] < shape_[dim[2]]; ++ idx[2]) {
        for (idx[3] = 0; idx[3] < shape_[dim[3]]; ++ idx[3]) {
          tmp[cnt] = dat[offset(idx[redim[0]], idx[redim[1]], idx[redim[2]],
              idx[redim[3]])];
          cnt ++ ;
        }
      }
    }
  }
  std::vector<int> tmp_shape(4);
  int i;
  for (i = 0; i < 4; ++ i)
    tmp_shape[i] = shape_[dim[i]];
  for (i = 0; i < 4; ++ i)
    shape_[i] = tmp_shape[i];
  memcpy(dat, tmp, sizeof(float) * count_);
  FREE(tmp);
}

void SeetaBlob::Release() {
  FREE(data_);
}

void SeetaBlob::SetData() {
  MYREALLOC2(data_, count_);
}


void SeetaBlob::SetData(const SeetaBlob &source) {
  CopyData(source.num(), source.channels(), source.height(), source.width(), source.data());
}

void SeetaBlob::SetData(int n, int c, int h, int w) {
  CopyData(n, c, h, w, NULL);
}

void SeetaBlob::CopyData(int n, int c, int h, int w, const float* const data) {
  shape_.resize(4);
  shape_[0] = n;
  shape_[1] = c;
  shape_[2] = h;
  shape_[3] = w;
  count_ = n * c * h * w;
  MYREALLOC2(data_, count_);
  if (data) {
    memcpy(data_, data, count_ * sizeof(float));
  }
}

void SeetaBlob::CopyDataU8(int n, int c, int h, int w, 
    const unsigned char* const data) {
  if (data_)
    data_ = nullptr;
  shape_.resize(4);
  shape_[0] = n;
  shape_[1] = c;
  shape_[2] = h;
  shape_[3] = w;
  count_ = n * c * h * w;
  MYREALLOC2(data_, count_);
  float* data_head = data_;
  for (int i = 0; i < count_; ++ i)
    data_head[i] = data[i];
}

void SeetaBlob::CopyTo(unsigned char* const data) {
  const float* const data_head = data_;
  for (int i = 0; i < count_; ++ i)
    data[i] = MIN(255.0f, MAX(0.0f, data_head[i]));
}

void SeetaBlob::CopyTo(float* const data) {
	float* data_head = data_;
	memcpy(data, data_head, count_ * sizeof(float));
}

void SeetaBlob::ToFile(const char* file_name) {
  std::ofstream ofs;
  ofs.open(file_name, std::ofstream::out);
  float* data = data_;
  for (int i = 0; i < count_; ++ i) {
    ofs << data[i] << " ";
  }
  ofs << std::endl;
  ofs.close();
}

void SeetaBlob::ToBinaryFile(const char* file_name) {
  FILE* file = fopen(file_name, "wb");
  if (file == nullptr) {
    LOG(ERROR) << file_name << " not exist!";
    exit(0);
  }
  for (int i = 0; i < 4; ++ i) {
    if (i < shape_.size())
      fwrite(&shape_[i], 1, sizeof(int), file);
    else {
      int one = 1;
      fwrite(&one, 1, sizeof(int), file);
    }
  }
  fwrite(data_, count_, sizeof(float), file);
  fclose(file);
}

#define PARAM_INT    1
#define PARAM_FLOAT  2
#define PARAM_STRING 3

class HyperParam {
 public:
  HyperParam() {
    params_.clear();
    v_int_.reserve(20);
    v_float_.reserve(20);
    v_str_.reserve(20);
  }
  void Load(FILE* file) {
    std::string param_name = read_str(file);
    while (param_name.compare("end") != 0) {
      int type;
      CHECK_EQ(fread(&type, sizeof(int), 1, file), 1); 
      if (type == PARAM_INT) {
        InsertInt(param_name, read_int(file));
      }
      else if (type == PARAM_FLOAT) {
        InsertFloat(param_name, read_float(file));
      }
      else if (type == PARAM_STRING) {
        InsertString(param_name, read_str(file));
      }
      param_name = read_str(file);
    }
  }
  void ToBinaryFile(FILE* file) {
    
  }
  ~HyperParam() {
    params_.clear();
    v_int_.clear();
    v_float_.clear();
    v_str_.clear();
  }
  bool has_param(std::string param_name) {
    return params_.count(param_name) != 0;
  }
  void* param(std::string param_name) {
    if (!has_param(param_name)) {
      LOG(ERROR) << "Param name " << param_name << " not exists.";
    }
    return params_[param_name];
  }
  void InsertInt(const std::string& key, const int value) {
    if (params_.count(key) != 0) {
      LOG(ERROR) << "Param name " << key << " already exists.";
      exit(0);
    }
    v_int_.push_back(value);
    params_[key] = &(v_int_.back());
    LOG(INFO) << key << ": " << value;
  }
  void InsertFloat(const std::string& key, const float value) {
    if (params_.count(key) != 0) {
      LOG(ERROR) << "Param name " << key << " already exists.";
      exit(0);
    }
    v_float_.push_back(value);
    params_[key] = &(v_float_.back());
    LOG(INFO) << key << ": " << value;
  }
  void InsertString(const std::string& key, const std::string& value) {
    if (params_.count(key) != 0) {
      LOG(ERROR) << "Param name " << key << " already exists.";
      exit(0);
    }
    v_str_.push_back(value);
    params_[key] = &(v_str_.back());
    LOG(INFO) << key << ": " << value;
  }
 private:
  std::string read_str(FILE* file) {
    int len;
    CHECK_EQ(fread(&len, sizeof(int), 1, file), 1);
    if (len <= 0) return "";
    char* c_str = new char[len + 1];
    CHECK_EQ(fread(c_str, sizeof(char), len, file), len);
    c_str[len] = '\0';
    std::string str(c_str);
    delete []c_str;
    return str;
  }
  int read_int(FILE* file) {
    int i;
    CHECK_EQ(fread(&i, sizeof(int), 1, file), 1);
    return i;
  }
  float read_float(FILE* file) {
    float f;
    CHECK_EQ(fread(&f, sizeof(int), 1, file), 1);
    return f;
  }
  int num_;
  std::map<std::string, void*> params_;
  std::vector<int> v_int_;
  std::vector<float> v_float_;
  std::vector<std::string> v_str_;
};

class SeetaNet {
 public:
  SeetaNet();
  virtual ~SeetaNet();
  // initialize the networks from a binary file
  virtual void SetUp();
  // execute the networks
  virtual void Execute() = 0;

  // check input blobs
  virtual void CheckInput();

  // check output blobs
  virtual void CheckOutput();

  virtual void Release() {
    for (int i = 0; i < output_blobs_.size(); ++i)
      output_blobs_[i].Release();
  }

  SeetaNet* const father() {
    return father_;
  }
  void SetFather(SeetaNet* father) {
    father_ = father;
  }
  std::vector<SeetaNet* >& nets() {
    return nets_;
  }
  SeetaNet* nets(int i) {
    return nets_[i];
  }
  std::vector<SeetaBlob>& input_blobs() {
    return input_blobs_;
  }
  SeetaBlob* input_blobs(int i) {
    return &(input_blobs_[i]);
  }
  std::vector<SeetaBlob>& output_blobs() {
    return output_blobs_;
  }
  SeetaBlob* output_blobs(int i) {
    return &(output_blobs_[i]);
  }
  std::vector<std::vector<SeetaBlob*> >& output_plugs() {
    return output_plugs_;
  }
  std::vector<SeetaBlob*>& output_plugs(int i) {
    return output_plugs_[i];
  }
  std::vector<std::vector<SeetaBlob*> >& input_plugs() {
    return input_plugs_;
  }
  std::vector<SeetaBlob*>& input_plugs(int i) {
    return input_plugs_[i];
  }
  HyperParam* hyper_param() {
    return &hyper_params_;
  }
  std::vector<SeetaBlob>& params() {
    return params_;
  }
  SeetaBlob* params(int i) {
    return &(params_[i]);
  }
  // count the number of unreleased output blobs
  inline int num_output() {
    int count = 0;
    for (int i = 0; i < output_blobs_.size(); ++ i)
      if (output_plugs_[i].size() == 0) count += 1;
    return count;
  }
 protected:
  // father SeetaNet
  SeetaNet* father_;
  // the limit of SeetaNet name size
  enum { MAX_NET_NAME_SIZE = 50 };
  // SeetaNet name
  std::string name_;

  // input and output blobs
  std::vector<SeetaBlob> input_blobs_;
  std::vector<SeetaBlob> output_blobs_;

  // subnet of the networks
  std::vector<SeetaNet* > nets_;

  // plugs
  std::vector<std::vector<SeetaBlob*> > output_plugs_;
  std::vector<std::vector<SeetaBlob*> > input_plugs_;

  // params in the networks
  HyperParam hyper_params_;
  std::vector<SeetaBlob> params_;
};


SeetaNet::SeetaNet() {
  nets_.clear();
  
  input_blobs_.clear();
  output_blobs_.clear();

  input_plugs_.clear();
  output_plugs_.clear();

  father_ = nullptr;
}

SeetaNet::~SeetaNet() {
  nets_.clear();
  
  input_blobs_.clear();
  output_blobs_.clear();

  input_plugs_.clear();
  output_plugs_.clear();

  params_.clear();
}

void SeetaNet::SetUp() {
  input_blobs_.clear();
  output_blobs_.clear();

  input_plugs_.clear();
  output_plugs_.clear();
  
  nets_.clear();

  params_.clear();
}

void SeetaNet::Execute() {
  // 1. check input blobs
  CheckInput();
  uint i;
  // 2. execute
  for (i = 0; i < nets_.size(); ++ i) {
    nets_[i]->Execute();
  }
  // 3. check output blobs
  CheckOutput();
}

void SeetaNet::CheckInput() {
  for (uint i = 0; i < input_blobs_.size(); ++ i) {
    if (input_blobs_[i].data() == nullptr) {
      LOG(INFO) << "SeetaNet input haven't been initialized completely!";
      exit(0);
    }
  }
}

void SeetaNet::CheckOutput() {
  uint i;
  for (i = 0; i < input_blobs_.size(); ++ i) {
    input_blobs_[i].Release();
  }
  for (i = 0; i < output_blobs_.size(); ++ i) {
    // connecting output plugs
    for (std::vector<SeetaBlob*>::iterator SeetaBlob = output_plugs_[i].begin();
        SeetaBlob != output_plugs_[i].end(); ++ SeetaBlob) {
      (*SeetaBlob)->SetData(output_blobs_[i]);
    }
    // release output blobs
    if (output_plugs_[i].size() != 0) {
      output_blobs_[i].Release();
    }
  } 
}


class NetRegistry {
 public:
  typedef SeetaNet* (*Creator)();
  typedef std::map<std::string, Creator> CreatorRegistry;

  static CreatorRegistry& Registry() {
    static CreatorRegistry* g_registry_ = new CreatorRegistry();
    return *g_registry_;
  }

  static void AddCreator(const std::string& type, Creator creator) {
    CreatorRegistry& registry = Registry();
    if (registry.count(type) != 0) {
      LOG(INFO) << "SeetaNet type " << type << " already registered.";
    }
    registry[type] = creator;
  }

  static SeetaNet* CreateNet(const std::string type) {
    CreatorRegistry& registry = Registry();
    if (registry.count(type) != 1) {
      LOG(ERROR) << "SeetaNet type " << type << " haven't registered.";
    }
    return registry[type]();
  }
 private:
  NetRegistry() {}

};

class NetRegisterer {
 public:
  NetRegisterer(const std::string& type,
                SeetaNet*(*creator)()) {
    LOG(INFO) << "Registering SeetaNet type: " << type;
    NetRegistry::AddCreator(type, creator);
  }
};

#define REGISTER_NET_CREATOR(type, creator)                                    \
  static NetRegisterer g_creator_##type(#type, creator)

#define REGISTER_NET_CLASS(type)                                               \
  SeetaNet* Creator_##type##Net()                                   \
  {                                                                            \
    return (new type##Net());                              \
  }                                                                            \
  REGISTER_NET_CREATOR(type, Creator_##type##Net);                             \
  static type##Net type



class CommonNet : public SeetaNet {
 public:
  CommonNet();
  ~CommonNet();
  // load model
  static SeetaNet* Load(FILE* file);
  // initialize the networks from a binary file
  virtual void SetUp();
  // execute the networks
  virtual void Execute();
};

CommonNet::CommonNet() {
  nets_.clear();
  
  input_blobs_.clear();
  output_blobs_.clear();

  input_plugs_.clear();
  output_plugs_.clear();

  params_.clear();
}

CommonNet::~CommonNet() {
  nets_.clear();
  
  input_blobs_.clear();
  output_blobs_.clear();

  input_plugs_.clear();
  output_plugs_.clear();

  params_.clear();
}

SeetaNet* CommonNet::Load(FILE* file) {
  // Todo: assert file format
  int len;
  CHECK_EQ(fread(&len, sizeof(int), 1, file), 1);
  char* net_type = new char[len + 1];
  // SeetaNet type
  CHECK_EQ(fread(net_type, sizeof(char), len, file), len);
  net_type[len] = '\0';
  LOG(INFO) << "Creating " << net_type << " SeetaNet ...";
  SeetaNet* net = NetRegistry::CreateNet(net_type);
  // params
  net->hyper_param()->Load(file);
  // Todo: name

  net->SetUp();
  int i, j;
  for (i = 0; i < net->params().size(); ++ i) {
    SeetaBlob param(file);
    LOG(INFO) << net_type << " SeetaNet blobs[" << i << "]: (" << param.num() << "," 
    << param.channels() << "," << param.height() << ","<< param.width() << ")";
    net->params(i)->SetData(param);
  }

  int num_subnet = net->nets().size();
  int num_in = net->input_blobs().size();
  int num_out = net->output_blobs().size();
 
  std::vector<SeetaNet* >& nets = net->nets();
  std::vector<SeetaBlob>& input_blobs = net->input_blobs();
  std::vector<SeetaBlob>& output_blobs = net->output_blobs();
  std::vector<std::vector<SeetaBlob*> >& output_plugs = net->output_plugs();
  std::vector<std::vector<SeetaBlob*> >& input_plugs = net->input_plugs();

  // subnet
  for (i = 0; i < num_subnet; ++ i) {
    nets[i] = Load(file);
    nets[i]->SetFather(net);
  }
  // input and output plugs
  if (num_subnet == 0) {
    for (i = 0; i < num_in; ++ i) {
      input_plugs[i].push_back(&(input_blobs[i]));
    }
  }
  else {
    // connection of each subnet
    for (i = 0; i < num_subnet; ++ i) {
      int input_num = nets[i]->input_blobs().size();
      for (j = 0; j < input_num; ++ j) {
        int net_idx, blob_idx;
        CHECK_EQ(fread(&net_idx, sizeof(int), 1, file), 1);
        CHECK_EQ(fread(&blob_idx, sizeof(int), 1, file), 1);
        if (net_idx == -1) { // connected to father SeetaNet
          input_plugs[blob_idx].push_back(nets[i]->input_blobs(j));
        }
        else {
          nets[net_idx]->output_plugs(blob_idx).push_back(
            nets[i]->input_blobs(j)); 
        }
      }
    }
    // get output blobs
    for (i = 0; i < num_out; ++ i) {
      int net_idx, blob_idx;
      CHECK_EQ(fread(&net_idx, sizeof(int), 1, file), 1);
      CHECK_EQ(fread(&blob_idx, sizeof(int), 1, file), 1);
      nets[net_idx]->output_plugs(blob_idx).push_back(
          &(output_blobs[i]));
    }
    for (i = 0; i < num_subnet; ++ i) {
      if (nets[i]->num_output() > 0) {
        LOG(ERROR) << "There are " << nets[i]->num_output() 
          << " output blobs unlinked!";
        exit(0);
      }
    }
  }
  delete []net_type;
  return net;
}

void CommonNet::SetUp() {
  int num_subnet = *(int*)(this->hyper_param()->param("num_subnet"));
  int num_in = *(int*)(this->hyper_param()->param("num_in"));
  int num_out = *(int*)(this->hyper_param()->param("num_out"));
  
  input_blobs_.resize(num_in);
  output_blobs_.resize(num_out);

  input_plugs_.resize(num_in);
  output_plugs_.resize(num_out);
  
  nets_.resize(num_subnet);

  params_.clear();
}

void CommonNet::Execute() {
  LOG(DEBUG) << "Common SeetaNet executing ...";
  int i;
  // 1. check input blobs
  for (i = 0; i < input_blobs_.size(); ++ i) {
    if (input_blobs_[i].data() == nullptr) {
      LOG(INFO) << "SeetaNet input haven't been initialized completely!";
      return ;
    }
    // connecting input plugs
    for (std::vector<SeetaBlob*>::iterator SeetaBlob = input_plugs_[i].begin();
        SeetaBlob != input_plugs_[i].end(); ++ SeetaBlob) {
      (*SeetaBlob)->SetData(input_blobs_[i]);
    }
    // release input blobs
    input_blobs_[i].Release();
  }
  LOG(DEBUG) << "Check input blobs done!";
  // 2. execute
  for (i = 0; i < nets_.size(); ++ i) {
    nets_[i]->Execute();
  }
  /*freopen("debug.txt", "w", stdout);
  for (int i = 0; i < output_blobs_[0].count(); ++i) {
	std::cout << (output_blobs_[0].data().get())[i] << std::endl;
  }*/
  // 3. check output blobs
  for (i = 0; i < output_blobs_.size(); ++ i) {
    // connecting output plugs
    for (std::vector<SeetaBlob*>::iterator SeetaBlob = output_plugs_[i].begin();
        SeetaBlob != output_plugs_[i].end(); ++ SeetaBlob) {
      (*SeetaBlob)->SetData(output_blobs_[i]);
    }
    // release output blobs
    if (output_plugs_[i].size() != 0) {
      output_blobs_[i].Release();
    }
  }
}

REGISTER_NET_CLASS(Common);


class BiasAdderNet : public SeetaNet {
  public:
    BiasAdderNet():SeetaNet() {}
    virtual ~BiasAdderNet(){}
    virtual void SetUp();
    virtual void Execute();
};

void BiasAdderNet::SetUp() {
  this->input_blobs().resize(1);
  this->output_blobs().resize(1);
  this->input_plugs().resize(1);
  this->output_plugs().resize(1);
  this->params().resize(1);
}

void BiasAdderNet::Execute() {
  CheckInput();

  const SeetaBlob* const input = this->input_blobs(0);
  const SeetaBlob* const bias = this->params(0);
  SeetaBlob* const output = this->output_blobs(0);

  int channels = bias->channels();
  CHECK_EQ(channels, input->channels());

  int height = input->height();
  int width = input->width();
  int num = input->num();
  
  LOG(DEBUG) << "input SeetaBlob: (" << num << "," << input->channels() << "," 
    << height << "," << width << ")";
  LOG(DEBUG) << "bias SeetaBlob: (" << bias->num() << "," << bias->channels() 
    << "," << bias->height() << "," << bias->width() << ")";
  
  int count_ = num*channels*height*width;
  float* dst_head = NULL;
  MYREALLOC2(dst_head, count_);

  int size = height * width;
  for (int n = 0, offset = 0; n < num; ++n) {
    for (int ichannel = 0; ichannel < channels; ++ichannel) {
      for(int i = 0; i < size; ++i, ++offset) {
        dst_head[offset] = (*input)[offset] + (*bias)[ichannel];
      }
    }
  }

  output->CopyData(num, channels, height, width, dst_head);
  FREE(dst_head);
  CheckOutput();
}

REGISTER_NET_CLASS(BiasAdder);

class BnNet : public SeetaNet {
 public:
  BnNet(): SeetaNet() {}
  virtual ~BnNet() {}
  virtual void SetUp();
  virtual void Execute();

 private:
  float epsilon_;
};

void BnNet::SetUp() {
  epsilon_ = *(float*)(this->hyper_param()->param("epsilon"));
 
 //check input and output SeetaBlob size
  this->input_blobs().resize(1);
  this->output_blobs().resize(1);
  this->input_plugs().resize(1);
  this->output_plugs().resize(1);
  this->params().resize(3);
}

void BnNet::Execute() {
  CheckInput();
 
  const SeetaBlob* const input = this->input_blobs(0);
  const SeetaBlob* const para_mean = this->params(0);
  const SeetaBlob* const para_var = this->params(1);
  const SeetaBlob* const para_scale = this->params(2);
  SeetaBlob* const output = this->output_blobs(0);
  
  int channels = input->channels();
  CHECK_EQ(channels, para_mean->channels());
  CHECK_EQ(channels, para_var->channels());

  int height = input->height();
  int width = input->width();
  int num = input->num();
  
  int count_ = num*channels*height*width;
  float* dst_head = NULL;
  MYREALLOC2(dst_head, count_);

  float scale = (*para_scale)[0];
  if (scale > 0){
	scale = 1.0f / scale;
  } else if (scale < 0){
	scale = 1.0f;
  } else {  // scale and epsilon should not both be equal to 0.
	if (epsilon_ < 1e-5){
		epsilon_ = 1e-5;
	}
  }

  int size = height * width;
  for (int n = 0, offset = 0; n < num; ++n) {
    for (int ichannel = 0; ichannel < channels; ++ichannel) {
      float mean = (*para_mean)[ichannel] * scale;
      float var = sqrt((*para_var)[ichannel] * scale + epsilon_);
      for (int i = 0; i < size; ++i, ++offset) {
        dst_head[offset] = ((*input)[offset] - mean) / var;
      }
	}
  }
  output->CopyData(num, channels, height, width, dst_head); 
  FREE(dst_head);
  CheckOutput(); 
}

REGISTER_NET_CLASS(Bn);

class ConvNet: public SeetaNet {
 public:
  ConvNet(): SeetaNet() {}
  virtual ~ConvNet() {}
  virtual void SetUp();
  virtual void Execute();
 
 protected:
  int stride_h_;
  int stride_w_;
};

void ConvNet::SetUp() {
  stride_h_ = stride_w_ =
      *(int*)(this->hyper_param()->param("stride"));

  // check input and output SeetaBlob size
  this->input_blobs().resize(1);
  this->output_blobs().resize(1);
  this->input_plugs().resize(1);
  this->output_plugs().resize(1);
  this->params().resize(1);
}

void ConvNet::Execute() {
  // *** Argument *** //
  const bool is_binary = false;
  // *** //

  CheckInput();
  const SeetaBlob* const input = this->input_blobs(0);
  const SeetaBlob* const weight = this->params(0);
  SeetaBlob* const output = this->output_blobs(0);

  int src_num = input->num();
  int src_channels = input->channels();
  int src_h = input->height();
  int src_w = input->width();
  int dst_channels = weight->num();
  int kernel_h = weight->height();
  int kernel_w = weight->width();

  LOG(DEBUG) << "input SeetaBlob: (" <<src_num << "," << src_channels << "," << src_h
    << "," << src_w << ")";

  int dst_h = (src_h - kernel_h) / stride_h_ + 1;
  int dst_w = (src_w - kernel_w) / stride_w_ + 1;
  int end_h = src_h - kernel_h + 1;
  int end_w = src_w - kernel_w + 1;
  int dst_size = dst_h * dst_w;
  int kernel_size = src_channels * kernel_h * kernel_w;

  const int src_num_offset = src_channels * src_h * src_w;
  int dst_count = src_num * dst_size * dst_channels;
  int mat_count = dst_size * kernel_size;
  float* dst_head = NULL;
  float* mat_head = NULL;
  MYREALLOC2(dst_head, dst_count);
  MYREALLOC2(mat_head, mat_count);

  tic;
  const float* src_data = input->data();
  float* dst_data = dst_head;
  int didx = 0;
  for (int sn = 0; sn < src_num; ++sn) {
    float* mat_data = mat_head;
    for (int sh = 0; sh < end_h; sh += stride_h_) {
      for (int sw = 0; sw < end_w; sw += stride_w_) {
        for (int sc = 0; sc < src_channels; ++sc) {
          int src_off = (sc * src_h + sh) * src_w + sw;
          for (int hidx = 0; hidx < kernel_h; ++hidx) {
            memcpy(mat_data, src_data + src_off, sizeof(float) * kernel_w);
            mat_data += kernel_w;
            src_off += src_w;
          }
        } // for sc
      } // for sw
    } // for sh
    src_data += src_num_offset;

    const float* weight_head = weight->data();
    matrix_procuct(mat_head, weight_head, dst_data, dst_size, dst_channels,
      kernel_size, true, false);
    dst_data += dst_channels * dst_size;
  } // for sn

  toc;
  output->CopyData(src_num, dst_channels, dst_h, dst_w, dst_head);
  FREE(dst_head);
  FREE(mat_head);

  LOG(DEBUG) << "output SeetaBlob: (" << output->num() << "," << output->channels()
    << "," << output->height() << "," << output->width() << ")";
  CheckOutput();
}

REGISTER_NET_CLASS(Conv);

class EltwiseNet: public SeetaNet {
 public:
  EltwiseNet(): SeetaNet() {}
  virtual ~EltwiseNet() {}
  virtual void SetUp();
  virtual void Execute();
 protected:
  std::string op_;
  float scale_;
  float upper_;
  float lower_;
};


void EltwiseNet::SetUp() {
  op_ = *(std::string*)(this->hyper_param()->param("eltwise_op"));
  if (op_ == "SCALE") {
    scale_ = *(float*)(this->hyper_param()->param("scale"));
    this->nets().resize(0);
    this->params().resize(0);
    this->input_blobs().resize(1);
    this->output_blobs().resize(1);
    this->input_plugs().resize(1);
    this->output_plugs().resize(1);
  }
  else if (op_ == "BAIS_ADDER") {
    this->nets().resize(0);
    this->params().resize(1);
    this->input_blobs().resize(1);
    this->output_blobs().resize(1);
    this->input_plugs().resize(1);
    this->output_plugs().resize(1);
  }
  else if (op_ == "CLOSE") {
    lower_ = *(float*)(this->hyper_param()->param("lower"));
    upper_ = *(float*)(this->hyper_param()->param("upper"));
    this->nets().resize(0);
    this->params().resize(0);
    this->input_blobs().resize(1);
    this->output_blobs().resize(1);
    this->input_plugs().resize(1);
    this->output_plugs().resize(1);
  }
  // SUM
  // PROD
  // MAX
}

void EltwiseNet::Execute() {
  CheckInput();
  if (op_ == "BAIS_ADDER") {
    const SeetaBlob* const input = this->input_blobs(0);
    const SeetaBlob* const bias = this->params(0);
    SeetaBlob* const output = this->output_blobs(0);

    int channels = bias->channels();
    CHECK_EQ(channels, input->channels());

    int height = input->height();
    int width = input->width();
    int num = input->num();
    
    LOG(DEBUG) << "input SeetaBlob: (" << num << "," << input->channels() << "," 
      << height << "," << width << ")";
    LOG(DEBUG) << "bias SeetaBlob: (" << bias->num() << "," << bias->channels() 
      << "," << bias->height() << "," << bias->width() << ")";
    
    int dst_count = num*channels*height*width;
    float* dst_head = NULL;
    MYREALLOC2(dst_head, dst_count);

    int bn = (bias->num() != 1);
    int bc = (bias->channels() != 1);
    int bh = (bias->height() != 1);
    int bw = (bias->width() != 1);
    for (int n = 0, offset = 0; n < num; ++n) {
      for (int c = 0; c < channels; ++ c) {
        for (int h = 0; h < height; ++ h) {
          for (int w = 0; w < width; ++ w, ++ offset) {
            dst_head[offset] = (*input)[offset] 
              + (*bias)[bias->offset(n*bn, c*bc, h*bh, w*bw)];
          }
        }
      }
    }

    output->CopyData(num, channels, height, width, dst_head);    
    FREE(dst_head);
  }
  else if (op_ == "SCALE") {
    const SeetaBlob* const input = this->input_blobs(0);
	LOG(DEBUG) << "input SeetaBlob: (" << input->num() << ","
		<< input->channels() << ","
		<< input->height() << ","
		<< input->width() << ")";
    int count = input->count();
    SeetaBlob* const output = this->output_blobs(0);
    float* dst_head = NULL;
    MYREALLOC2(dst_head, count);
    for (int i = 0; i < count; ++ i)
      dst_head[i] = (*input)[i] * scale_;
    output->CopyData(input->num(), input->channels(), input->height(), 
                    input->width(), dst_head);
	
    FREE(dst_head);
  }
  else if (op_ == "CLOSE") {
    const SeetaBlob* const input = this->input_blobs(0);
	LOG(DEBUG) << "input SeetaBlob: (" << input->num() << ","
		<< input->channels() << ","
		<< input->height() << ","
		<< input->width() << ")";
    int count = input->count();
    SeetaBlob* const output = this->output_blobs(0);
    float* dst_head = NULL;
    MYREALLOC2(dst_head, count);
    for (int i = 0; i < count; ++ i) {
      dst_head[i] = (*input)[i];
      dst_head[i] = MIN(dst_head[i], upper_);
      dst_head[i] = MAX(dst_head[i], lower_);
    }
    output->CopyData(input->num(), input->channels(), input->height(), 
                    input->width(), dst_head);  
    FREE(dst_head);
  }
  // SUM
  // PROD
  // MAX
  CheckOutput();
}

REGISTER_NET_CLASS(Eltwise);

class InnerProductNet: public SeetaNet {
 public:
  InnerProductNet(): SeetaNet() {}
  virtual ~InnerProductNet() {}
  virtual void SetUp();
  virtual void Execute();
 
 protected:
};


void InnerProductNet::SetUp() {
  // check input and output SeetaBlob size
  this->input_blobs().resize(1);
  this->output_blobs().resize(1);
  this->input_plugs().resize(1);
  this->output_plugs().resize(1);
  this->params().resize(1);
}

void InnerProductNet::Execute() {
  CheckInput();
  const SeetaBlob* const input = this->input_blobs(0); // src_num * vec_len
  const SeetaBlob* const weight = this->params(0);  // dst_channels * vec_len
  SeetaBlob* const output = this->output_blobs(0); // src_num * dst_channels
  
  int src_num = input->num();
  int src_channels = input->channels();
  int src_h = input->height();
  int src_w = input->width();
  int dst_channels = weight->num();
  
  LOG(DEBUG) << "input SeetaBlob: (" <<src_num << "," << src_channels << "," << src_h 
    << "," << src_w << ")";

  const int vec_len = src_channels * src_h * src_w;
  int dst_count = src_num * dst_channels;
  float* dst_head = NULL;
  MYREALLOC2(dst_head, dst_count);
  const float* src_data = input->data();
  for (int sn = 0, didx = 0; sn < src_num; ++sn) {
    const float* weight_data = weight->data();
    for (int dc = 0; dc < dst_channels; ++dc) {
      dst_head[didx++] = simd_dot(src_data, weight_data, vec_len);
      weight_data += vec_len;
    } // for dc
    
    src_data += vec_len;
  } // for sn
  
  output->CopyData(src_num, dst_channels, 1, 1, dst_head);
  FREE(dst_head);
  LOG(DEBUG) << "output SeetaBlob: (" << output->num() << "," << output->channels() 
    << "," << output->height() << "," << output->width() << ")";
  CheckOutput();
}

REGISTER_NET_CLASS(InnerProduct);

class MaxPoolingNet: public SeetaNet {
 public:
  MaxPoolingNet(): SeetaNet() {}
  virtual ~MaxPoolingNet() {}
  virtual void SetUp();
  virtual void Execute();
 
 protected:
  int kernel_h_;
  int kernel_w_;
  int stride_h_;
  int stride_w_;
};

void MaxPoolingNet::SetUp() {
  kernel_h_ = kernel_w_ = 
      *(int*)(this->hyper_param()->param("kernel_size"));
  stride_h_ = stride_w_ = 
      *(int*)(this->hyper_param()->param("stride"));

  // check input and output SeetaBlob size
  this->input_blobs().resize(1);
  this->output_blobs().resize(1);
  this->input_plugs().resize(1);
  this->output_plugs().resize(1);
}

void MaxPoolingNet::Execute() {
  // *** Argument *** //
  const float MIN_THRESHOLD = 0.0f;
  // *** //
  
  CheckInput();
  const SeetaBlob* const input = this->input_blobs(0);
  SeetaBlob* const output = this->output_blobs(0);
  
  int src_h = input->height();
  int src_w = input->width();
  int num = input->num();
  int channels = input->channels();

  int dst_h = static_cast<int>(ceil(static_cast<float>(
    src_h - kernel_h_) / stride_h_)) + 1;
  int dst_w = static_cast<int>(ceil(static_cast<float>(
    src_w - kernel_w_) / stride_w_)) + 1;

  int dst_count = num * channels * dst_h * dst_w;
  float* dst_head = NULL;
  MYREALLOC2(dst_head, dst_count);
  const float* src_data = input->data();
  float* dst_data = dst_head;
  int src_channel_off = src_h * src_w;
  int dst_channel_off = dst_h * dst_w;
  for (int n = 0; n < num; ++n) {
    for (int c = 0; c < channels; ++c) {
      for (int dh = 0, hstart = 0; dh < dst_h;
            ++dh, hstart += stride_h_) {
        int hend = MIN(hstart + kernel_h_, src_h);
        for (int dw = 0, wstart = 0; dw < dst_w;
            ++dw, wstart += stride_w_) {
          int wend = MIN(wstart + kernel_w_, src_w);
          int didx = dh * dst_w + dw;

          float max_val = MIN_THRESHOLD;
          for (int sh = hstart; sh < hend; ++sh) {
            for (int sw = wstart; sw < wend; ++sw) {
              int sidx = sh * src_w + sw;
              if (src_data[sidx] > max_val) {
                max_val = src_data[sidx];
              }
            } // for sw
          } // for sh
          dst_data[didx] = max_val;

        } // for dw
      } // for dh

      src_data += src_channel_off;
      dst_data += dst_channel_off;
    } // for c
  } // for n

  output->CopyData(num, channels, dst_h, dst_w, dst_head);
  FREE(dst_head);
  CheckOutput();
}

REGISTER_NET_CLASS(MaxPooling);

class PadNet: public SeetaNet {
 public:
  PadNet(): SeetaNet() {}
  virtual ~PadNet() {}
  virtual void SetUp();
  virtual void Execute();
 
 protected:
  int left_, right_, bottom_, top_;
};

void PadNet::SetUp() {
  left_ = right_ = bottom_ = top_ = *(int*)(this->hyper_param()->param("pad"));
  // check input and output SeetaBlob size
  this->nets().resize(0);
  this->params().resize(0);
  this->input_blobs().resize(1);
  this->output_blobs().resize(1);
  this->input_plugs().resize(1);
  this->output_plugs().resize(1);
}

void PadNet::Execute() {
  CheckInput();
  const SeetaBlob* const input = this->input_blobs(0);
  SeetaBlob* const output = this->output_blobs(0);
  int src_w = input->width();
  int src_h = input->height();
  int num = input->num();
  int channels = input->channels();

  LOG(DEBUG) << "input SeetaBlob: (" << num << "," << channels << "," << src_h 
    << "," << src_w << ")";

  int dst_w = src_w + left_ + right_;
  int dst_h = src_h + top_ + bottom_;

  int height = MIN(src_h, dst_h);
  int width = MIN(src_w, dst_w);

  output->reshape(num, channels, dst_h, dst_w);

  int count_ = num * channels * dst_h * dst_w;
  float* data = NULL;
  MYREALLOC2(data, count_);

  for (int n = 0; n < num; ++ n) {
    int src_off = input->offset(n), dst_off = output->offset(n);
    for (int c = 0; c < channels; ++ c) {
      if (top_ < 0 && left_ < 0) {
        src_off = input->offset(n, c, -top_, -left_);
      }
      else if(top_ >= 0 && left_ >= 0) {
        dst_off = output->offset(n, c, top_, left_);
      }
      else {
        LOG(ERROR) << "Illegal padding parameters!";
        exit(0);
      }
      for (int h = 0; h < height; ++ h) {
        memcpy(data + dst_off, input->data() + src_off, 
               sizeof(float) * width);
        src_off += src_w;
        dst_off += dst_w;
      }
    }
  }
  output->CopyData(num, channels, dst_h, dst_w, data);
  FREE(data);
  CheckOutput();
}

REGISTER_NET_CLASS(Pad);


class SpatialTransformNet : public SeetaNet {
 public:
  SpatialTransformNet() : SeetaNet() {}
  virtual ~SpatialTransformNet() {}
  virtual void SetUp();
  virtual void Execute();
 
 protected:
  // sampling for common SeetaBlob data
  virtual double Sampling(const float* const feat_map, int H, int W, double x, 
      double y, double scale = 1.0);

  // sampling for cv::Mat::data
  virtual double Sampling(const unsigned char* const feat_map, int c, int H, 
      int W, int C, double x, double y, double scale = 1.0);

  virtual double Cubic(double x);

  inline void Norm(std::vector<double>& weights) {
    float sum = 0;
    int i;
    for (i = 0; i < weights.size(); ++ i)
      sum += weights[i];
    for (i = 0; i < weights.size(); ++ i)
      weights[i] /= sum;
  }

  // transformation type: linear or bicubic
  std::string type_;
  // whether input with cv::Mat::data
  int is_mat_data_;
  // output feature map height and width
  int new_height_, new_width_;
};


void SpatialTransformNet::SetUp() {
  type_ = *(std::string *)(this->hyper_param()->param("type"));
  new_height_ = *(int *)(this->hyper_param()->param("new_height"));
  new_width_ = *(int *)(this->hyper_param()->param("new_width"));
  if (this->hyper_param()->has_param("is_mat_data")) {
    is_mat_data_ = *(int *)(this->hyper_param()->param("is_mat_data"));
  }
  else {
    is_mat_data_ = false;
  }
  // check input and output SeetaBlob size
  this->input_blobs().resize(2);
  this->output_blobs().resize(1);
  this->input_plugs().resize(2);
  this->output_plugs().resize(1);
}

void SpatialTransformNet::Execute() {
  CheckInput();
  const SeetaBlob* const input = this->input_blobs(0);
  const SeetaBlob* const theta = this->input_blobs(1);
  SeetaBlob* const output = this->output_blobs(0);

  CHECK_EQ(input->num(), theta->num());

  int num = input->num();
  int channels = input->channels();
  
  int src_w = input->width();
  int src_h = input->height();

  LOG(DEBUG) << "Input blobs: (" << num << "," << channels << "," << src_h
    << "," << src_w << ")";

  int dst_h = new_height_;
  int dst_w = new_width_;

  float* input_data = input->data();
  const float* theta_data = theta->data();
  int tform_size = theta->count() / num;

  LOG(DEBUG) << "Theta: [" << theta_data[0] << "," << theta_data[1] << ","
    << theta_data[2] << "," << theta_data[3] << "," << theta_data[4] << ","
    << theta_data[5] << "]";

  CHECK_EQ(tform_size, 6);

  output->SetData(num, channels, dst_h, dst_w);

  float* output_data = output->data();

  for (int n = 0; n < num; ++ n) {
    double scale = sqrt(theta_data[0] * theta_data[0] 
        + theta_data[3] * theta_data[3]);
    for (int x = 0; x < dst_h; ++ x)
      for (int y = 0; y < dst_w; ++ y) {
        // Get the source position of each point on the destination feature map.
        double src_y = theta_data[0] * y + theta_data[1] * x + theta_data[2];
        double src_x = theta_data[3] * y + theta_data[4] * x + theta_data[5];
        for (int c = 0; c < channels; ++ c) {
          if (!is_mat_data_) {
            output_data[output->offset(n, c, x, y)]
              = Sampling(input_data + input->offset(n, c), src_h, src_w,
              src_x, src_y, 1.0 / scale);
          }
          else {
            output_data[output->offset(n, c, x, y)]
              = Sampling(reinterpret_cast<unsigned char*>(
                  input_data + input->offset(n)), c, src_h, src_w,
              this->output_blobs(0)->channels(), src_x, src_y, 1.0 / scale);
          }
        }
      }
    theta_data += tform_size;
  }

  LOG(DEBUG) << "Output blobs: (" << this->output_blobs(0)->num() << "," 
    << this->output_blobs(0)->channels() << "," 
    << this->output_blobs(0)->height() << ","
    << this->output_blobs(0)->width() << ")";
  CheckOutput();
}

double SpatialTransformNet::Sampling(const float* const feat_map, int H, int W, 
    double x, double y, double scale) {
  if (type_ == "linear") {
    // bilinear subsampling
    int ux = floor(x), uy = floor(y);
    double ans = 0;
    if (ux >= 0 && ux < H - 1 && uy >= 0 && uy < W - 1) {
      int offset = ux * W + uy;
      double cof_x = x - ux;
      double cof_y = y - uy;
      ans = (1 - cof_y) * feat_map[offset] + cof_y * feat_map[offset + 1];
      ans = (1 - cof_x) * ans + cof_x * ((1 - cof_y) * feat_map[offset + W] 
          + cof_y * feat_map[offset + W + 1]);
    }
    return ans;
  }
  else if (type_ == "bicubic") { // Need to be sped up
    // bicubic subsampling
    double ans = 0;
    if (x >= 0 && x < H && y >= 0 && y < W) {
      scale = MIN(scale, double(1.0));
      double kernel_width =  MAX(8.0, 4.0 / scale); // bicubic kernel width
      std::vector<double> weights_x, weights_y; 
      std::vector<int>  indices_x, indices_y;
      weights_x.reserve(5), indices_x.reserve(5);
      weights_y.reserve(5), indices_y.reserve(5);
      // get indices and weight along x axis
      for (int ux = ceil(x - kernel_width / 2); 
          ux <= floor(x + kernel_width / 2); ++ ux) {
        indices_x.push_back(MAX(MIN(H - 1, ux), 0));
        weights_x.push_back(Cubic((x - ux) * scale));
      }
      // get indices and weight along y axis
       for (int uy = ceil(y - kernel_width / 2); 
          uy <= floor(y + kernel_width / 2); ++ uy) {
        indices_y.push_back(MAX(MIN(W - 1, uy), 0));
        weights_y.push_back(Cubic((y - uy) * scale));
      }     
      // normalize the weights
      Norm(weights_x);
      Norm(weights_y);
      double val = 0;
	  int lx = weights_x.size(), ly = weights_y.size();
      for (int i = 0; i < lx; ++ i) {
        if (i == 0 || indices_x[i] != indices_x[i - 1]) {
          val = 0;
          int offset = indices_x[i] * W;
          for (int j = 0; j < ly; ++ j) {
            val += feat_map[offset + indices_y[j]] * weights_y[j];
          }
        }
        ans += val * weights_x[i];
      }
    }
    return ans;
  }
  return 0;
}

double SpatialTransformNet::Sampling(const unsigned char* const feat_map, 
    int c, int H, int W, int C, double x, double y, double scale) {
  if (type_ == "linear") {
    // bilinear subsampling
    int ux = floor(x), uy = floor(y);
    double ans = 0;
    if (ux >= 0 && ux < H - 1 && uy >= 0 && uy < W - 1) {
      int offset = (ux * W + uy) * C + c;
      double cof_x = x - ux;
      double cof_y = y - uy;
      ans = (1 - cof_y) * feat_map[offset] + cof_y * feat_map[offset + C];
      ans = (1 - cof_x) * ans + cof_x * ((1 - cof_y) * feat_map[offset + W * C]
        + cof_y * feat_map[offset + W * C + C]);
    }
    return ans;
  }
  else if (type_ == "bicubic") { // Need to be sped up
    // bicubic subsampling
    double ans = 0;
    if (x >= 0 && x < H && y >= 0 && y < W) {
      scale = MIN(scale, double(1.0));
      double kernel_width = MAX(8.0, 4.0 / scale); // bicubic kernel width
      std::vector<double> weights_x, weights_y;
      std::vector<int>  indices_x, indices_y;
      weights_x.reserve(5), indices_x.reserve(5);
      weights_y.reserve(5), indices_y.reserve(5);
      // get indices and weight along x axis
      for (int ux = ceil(x - kernel_width / 2);
        ux <= floor(x + kernel_width / 2); ++ux) {
        indices_x.push_back(MAX(MIN(H - 1, ux), 0));
        weights_x.push_back(Cubic((x - ux) * scale));
      }
      // get indices and weight along y axis
      for (int uy = ceil(y - kernel_width / 2);
        uy <= floor(y + kernel_width / 2); ++uy) {
        indices_y.push_back(MAX(MIN(W - 1, uy), 0));
        weights_y.push_back(Cubic((y - uy) * scale));
      }
      // normalize the weights
      Norm(weights_x);
      Norm(weights_y);
      double val = 0;
      int lx = weights_x.size(), ly = weights_y.size();
      for (int i = 0; i < lx; ++i) {
        if (i == 0 || indices_x[i] != indices_x[i - 1]) {
          val = 0;
          int offset = indices_x[i] * W * C;
          for (int j = 0; j < ly; ++j) {
            val += feat_map[offset + indices_y[j] * C + c] * weights_y[j];
          }
        }
        ans += val * weights_x[i];
      }
    }
    return ans;
  }
  return 0;
}

double SpatialTransformNet::Cubic(double x) {
  double ax = fabs(x), ax2, ax3;
  ax2 = ax * ax;
  ax3 = ax2 * ax;
  if (ax <= 1) return 1.5 * ax3 - 2.5 * ax2 + 1;
  if (ax <= 2) return -0.5 * ax3 + 2.5 * ax2 -4 * ax + 2;
  return 0;
}

REGISTER_NET_CLASS(SpatialTransform);



// Calculate affine transformation according to feature points
class TransformationMakerNet: public SeetaNet {
 public:
  TransformationMakerNet(): SeetaNet() {}
  virtual ~TransformationMakerNet() {}
  virtual void SetUp();
  virtual void Execute();
 protected:
  // the number of feature points
  int points_num_;
};


void TransformationMakerNet::SetUp() {
  points_num_ = *(int*)(this->hyper_param()->param("points_num"));
  // check input and output SeetaBlob size
  this->input_blobs().resize(1);
  this->output_blobs().resize(1);
  this->input_plugs().resize(1);
  this->output_plugs().resize(1);
  this->params().resize(1);
}

void TransformationMakerNet::Execute() {
  const float EPS = 1e-4;
  const int TFORM_SIZE = 6;
  CheckInput();
  CHECK_EQ(points_num_, this->input_blobs(0)->channels());

  SeetaBlob* const input = this->input_blobs(0);
  SeetaBlob* const param = this->params(0);
  const float* feat_points = input->data();
  const float* std_points = param->data();
  int count_ = input->num() * TFORM_SIZE;
  float* out_data = NULL;
  MYREALLOC2(out_data, count_);

  for (int n = 0; n < input->num(); ++ n) {
    double sum_x = 0, sum_y = 0;
    double sum_u = 0, sum_v = 0;
    double sum_xx_yy = 0;
    double sum_ux_vy = 0;
    double sum_vx__uy = 0;
    {
      for (int c = 0; c < points_num_; ++ c) {
      int x_off = n * points_num_ * 2 + c * 2;
      int y_off = x_off + 1; 
      sum_x += std_points[c * 2];
      sum_y += std_points[c * 2 + 1];
      sum_u += feat_points[x_off];
      sum_v += feat_points[y_off];
      sum_xx_yy += std_points[c * 2] * std_points[c * 2] + 
                   std_points[c * 2 + 1] * std_points[c * 2 + 1];
      sum_ux_vy += std_points[c * 2] * feat_points[x_off] +
                   std_points[c * 2 + 1] * feat_points[y_off];
      sum_vx__uy += feat_points[y_off] * std_points[c * 2] -
                    feat_points[x_off] * std_points[c * 2 + 1];
    }
    }
    CHECK_GT(sum_xx_yy, EPS);
    double q = sum_u - sum_x * sum_ux_vy / sum_xx_yy 
                     + sum_y * sum_vx__uy / sum_xx_yy;
    double p = sum_v - sum_y * sum_ux_vy / sum_xx_yy 
                     - sum_x * sum_vx__uy / sum_xx_yy;

    double r = points_num_ - (sum_x * sum_x + sum_y * sum_y) / sum_xx_yy;
    
    CHECK_TRUE(r > EPS || r < -EPS) << " r = " << r;

    double a = (sum_ux_vy - sum_x * q / r - sum_y * p / r) / sum_xx_yy;

    double b = (sum_vx__uy + sum_y * q / r - sum_x * p / r) / sum_xx_yy; 

    double c = q / r;

    double d = p / r;

    float* tform = out_data + n * TFORM_SIZE;
    tform[0] = tform[4] = a;
    tform[1] = -b;
    tform[3] = b;
    tform[2] = c;
    tform[5] = d;
  }
  this->output_blobs(0)->CopyData(input->num(), TFORM_SIZE, 1, 1, out_data);
  
  FREE(out_data);
  CheckOutput();
}


class Aligner {
 public:
  Aligner();
  Aligner(int crop_height, int crop_width, std::string type = "bicubic");
  ~Aligner();
  // Alignment and return to a ImageData
  void Alignment(const ImageData &src_img, 
      const float* const llpoint, 
      const ImageData &dst_img); 
  // Alignment and return to a SeetaBlob
  void Alignment(const ImageData &src_img, 
      const float* const llpoint, 
      SeetaBlob* const dst_blob); 

  void set_height(int height) { crop_height_ = height; }
  void set_width(int width) {crop_width_ = width; }

  int crop_height() { return crop_height_; }
  int crop_width() { return crop_width_; }
 private:
  int crop_height_;
  int crop_width_;
  SeetaNet* net_;
};

Aligner::Aligner():
    crop_height_(256),
    crop_width_(256) {
  // build a aligner networks
  net_ = (new CommonNet());
  HyperParam* common_param = net_->hyper_param();
  common_param->InsertInt("num_subnet", 2);
  common_param->InsertInt("num_in", 2);
  common_param->InsertInt("num_out", 1);
  net_->SetUp();
 
  SeetaNet* tform_maker_net = net_->nets(0);
  tform_maker_net->SetFather(net_); 
  // left_eye, right_eye, nose, left_mouse_corner and right_mouse_corner
  float std_points[10] = {
    89.3095, 72.9025,
    169.3095, 72.9025,
    127.8949, 127.0441,
    96.8796, 184.8907,
    159.1065, 184.7601,
  };
  HyperParam* tform_param = tform_maker_net->hyper_param();
  tform_param->InsertInt("points_num", 5);
  tform_maker_net->SetUp();
  SeetaBlob* tform_blob = tform_maker_net->params(0);
  tform_blob->CopyData(1, 5, 2, 1, std_points);

  SeetaNet*align_net = net_->nets(1);
  align_net->SetFather(net_); 

  HyperParam* align_param = align_net->hyper_param();
  align_param->InsertInt("new_height", crop_height_);
  align_param->InsertInt("new_width", crop_width_);
  align_param->InsertString("type", "bicubic");
  // Input with unpermuted mat data
  align_param->InsertInt("is_mat_data", 1);
  align_net->SetUp();
  
  net_->input_plugs(0).push_back(tform_maker_net->input_blobs(0));
  net_->input_plugs(1).push_back(align_net->input_blobs(0));
  tform_maker_net->output_plugs(0).push_back(align_net->input_blobs(1));
  align_net->output_plugs(0).push_back(net_->output_blobs(0));
}

Aligner::Aligner(int crop_height, int crop_width, std::string type):
    crop_height_(crop_height),
    crop_width_(crop_width) {
  // build a aligner networks
  net_ = (new CommonNet());
  HyperParam* common_param = net_->hyper_param();
  common_param->InsertInt("num_subnet", 2);
  common_param->InsertInt("num_in", 2);
  common_param->InsertInt("num_out", 1);
  net_->SetUp();

  std::vector<SeetaNet* >& sub_nets = net_->nets();
 
  sub_nets[0] = (new TransformationMakerNet());
  sub_nets[1] = (new SpatialTransformNet());
  SeetaNet* tform_maker_net = net_->nets(0);
  tform_maker_net->SetFather(net_); 
  // left_eye, right_eye, nose, left_mouse_corner and right_mouse_corner
  float std_points[10] = {
    89.3095, 72.9025,
    169.3095, 72.9025,
    127.8949, 127.0441,
    96.8796, 184.8907,
    159.1065, 184.7601,
  };
  for (int i = 0; i < 5; ++ i) {
    std_points[i * 2] *= crop_height_ / 256.0;
    std_points[i * 2 + 1] *= crop_width_ / 256.0;
  }
  HyperParam* tform_param = tform_maker_net->hyper_param();
  tform_param->InsertInt("points_num", 5);
  tform_maker_net->SetUp();
  SeetaBlob* tform_blob = tform_maker_net->params(0);
  tform_blob->CopyData(1, 5, 2, 1, std_points);

  SeetaNet* align_net = net_->nets(1);
  align_net->SetFather(net_); 

  HyperParam* align_param = align_net->hyper_param();
  align_param->InsertInt("new_height", crop_height_);
  align_param->InsertInt("new_width", crop_width_);
  align_param->InsertString("type", type);
  // Input with unpermuted mat data
  align_param->InsertInt("is_mat_data", 1);
  align_net->SetUp();
 
  //set connections 
  net_->input_plugs(0).push_back(tform_maker_net->input_blobs(0));
  net_->input_plugs(1).push_back(align_net->input_blobs(0));
  tform_maker_net->output_plugs(0).push_back(align_net->input_blobs(1));
  align_net->output_plugs(0).push_back(net_->output_blobs(0));
}

Aligner::~Aligner() {}

void Aligner::Alignment(const ImageData &src_img,
    const float* const points,
    SeetaBlob* const dst_blob) {
  SeetaBlob* const input_data = net_->input_blobs(1);
  input_data->reshape(1, src_img.num_channels, src_img.height, src_img.width);
  input_data->SetData();
  // input with mat::data avoid coping data
  memcpy(input_data->data(), src_img.data, input_data->count() * sizeof(unsigned char));
  /*input_data->CopyData(1, src_img.height, src_img.width, src_img.channels,
    src_img.data);
  input_data->Permute(1, 4, 2, 3);*/
  SeetaBlob* const input_point = net_->input_blobs(0);
  
  input_point->CopyData(1, 5, 2, 1, points);

  net_->Execute();
  dst_blob->SetData(*(net_->output_blobs(0)));
}

void Aligner::Alignment(const ImageData &src_img,
	  const float* const points,
	  const ImageData &dst_img) {
	SeetaBlob out_blob;
	Alignment(src_img, points, &out_blob);
	out_blob.Permute(1, 3, 4, 2);
	out_blob.CopyTo(dst_img.data);
}

typedef float* FaceFeatures;

class FaceIdentification {
public:
  // For cropping face only, you don't need a identification model, modelPath  \
   can be set NULL.
  // For identification, you need to create a 'FaceIdentification' object with \
  the path of identification model, or call function 'LoadModel' after create  \
  an object.
   FaceIdentification(const char* model_path = nullptr);

   ~FaceIdentification();

  // If you create an 'FaceIdentification' object without identification model \
  , you need to call LoadModel for recognition use.
   uint32_t LoadModel(const char* model_path);

  // Get feature's dimension.
   uint32_t feature_size();

  // Get cropping face width.
   uint32_t crop_width();

  // Get cropping face height.
   uint32_t crop_height();

  // Get cropping face channel.
   uint32_t crop_channels();

  // Crop face with 3-channels image and 5 located landmark points.
  // 'dst_image' can be initialized as a cv::Mat which cols equal to           \
  crop_width(), rows equal to crop_height() and channels equal to              \
  crop_channels().
   uint8_t CropFace(const ImageData &src_image,
      const FacialLandmark *llpoint,
      const ImageData &dst_image);

  // Extract feature with a cropping face.
  // 'feats' must be initialized with size of feature_size().
   uint8_t ExtractFeature(const ImageData &crop_image,
      FaceFeatures const feats);

  // Extract feature for face in a 3-channels image given 5 located landmark   \
  points.
  // 'feats' must be initialized with size of GetFeatureSize().
   uint8_t ExtractFeatureWithCrop(const ImageData &src_image,
      const FacialLandmark *llpoint,
      FaceFeatures const feats);

  // Calculate similarity of face features fc1 and fc2.
  // dim = -1 default feature size
   float CalcSimilarity(FaceFeatures const fc1,
      FaceFeatures const fc2, 
      long dim = -1);
  
private:
  class Recognizer;
  Recognizer* recognizer;
};


class FaceIdentification::Recognizer {
public:
  Recognizer(const char* model_path = nullptr) {
    if (model_path == nullptr) {
      crop_width_ = 256;
      crop_height_ = 256;
      crop_channels_ = 3;
      net_ = nullptr;
      aligner_ = (new Aligner(crop_height_, crop_width_, "linear"));
      feat_size_ = 0;
    } 
    else {
	  crop_width_ = 0;
	  crop_height_ = 0;
	  crop_channels_ = 0;
	  net_ = nullptr;
	  aligner_ = nullptr;
	  feat_size_ = 0;
      LoadModel(model_path);
    }
  }

  ~Recognizer() {
  }

  uint8_t LoadModel(const char* model_path) {
    FILE* file = fopen(model_path, "rb");
    if (file == 0) {
      std::cout<< model_path << " not exist!" << std::endl;
      exit(-1);
    }
    CHECK_EQ(fread(&crop_channels_, sizeof(int), 1, file), 1); 
    CHECK_EQ(fread(&crop_height_, sizeof(int), 1, file), 1);
    CHECK_EQ(fread(&crop_width_, sizeof(int), 1, file), 1);
	CHECK_EQ(fread(&feat_size_, sizeof(int), 1, file), 1);
    /*if (!aligner_ || crop_height_ != aligner_->CropHeight() 
		||  crop_width_ != aligner_->CropHeight())*/
    aligner_ = (new Aligner(crop_height_, crop_width_, "linear"));
    net_ = CommonNet::Load(file);
    return 1;
  }

  uint8_t Crop(const ImageData &src_img, const float* const points,
      const ImageData &dst_img) {
    aligner_->Alignment(src_img, points, dst_img);
    return 1;
  }

  uint8_t ExtractFeature(unsigned char* const u_data, float* const feat, 
      int n = 1) {
    net_->input_blobs(0)->CopyDataU8(n, crop_height_, crop_width_, crop_channels_, u_data);
    net_->input_blobs(0)->Permute(1, 4, 2, 3);
    net_->Execute();

    net_->output_blobs(0)->CopyTo(feat);
    net_->Release();
    return 1;
  }

  uint8_t ExtractFeatureWithCrop(const ImageData &src_img, 
      float* const points, float* const feat) {
    // crop
    SeetaBlob crop_blob;
    aligner_->Alignment(src_img, points, &crop_blob);
    // extract feature
    net_->input_blobs(0)->SetData(crop_blob);
    net_->Execute();

    net_->output_blobs(0)->CopyTo(feat);
    net_->Release();
	return 1;
  }

  uint8_t ExtractFeature(unsigned char* const u_data, uint16_t* const feat) {
	//To do: compress features 
    return 1;
  }
  
  uint32_t crop_width() { return crop_width_; }
  uint32_t crop_height() { return crop_height_; }
  uint32_t crop_channels() { return crop_channels_; }
  uint32_t crop_mem_size() { return crop_width_ * crop_height_ * crop_channels_; }
  uint32_t feature_size() { return feat_size_; }

private:
  SeetaNet* net_;
  Aligner* aligner_;
  uint32_t crop_width_;
  uint32_t crop_height_;
  uint32_t crop_channels_;

  uint32_t feat_size_;
  uint8_t isLoadModel() {
    return net_ != nullptr;
  }
};


FaceIdentification::FaceIdentification(const char* model_path) {
  recognizer = new Recognizer(model_path);
}

FaceIdentification::~FaceIdentification() {
  delete recognizer;
}

uint32_t FaceIdentification::LoadModel(const char* model_path) {
  return recognizer->LoadModel(model_path);
}

uint32_t FaceIdentification::feature_size() {
  return recognizer->feature_size();
}

uint32_t FaceIdentification::crop_width() {
  return recognizer->crop_width();
}

uint32_t FaceIdentification::crop_height() {
  return recognizer->crop_height();
}

uint32_t FaceIdentification::crop_channels() {
  return recognizer->crop_channels();
}

uint8_t FaceIdentification::CropFace(const ImageData &src_image, const FacialLandmark* llpoint, const ImageData &dst_image) {
  if (src_image.num_channels != recognizer->crop_channels() ||
    src_image.data == NULL) {
    std::cout << "Face Recognizer: Error input image." << std::endl;
    return 0;
  }
  if (dst_image.data == NULL) {
    std::cout << "Face Recognizer: Error output image." << std::endl;
    return 0;
  }
  float point_data[10];
  for (int i = 0; i < 5; ++i) {
	  point_data[i * 2] = llpoint[i].x;
	  point_data[i * 2 + 1] = llpoint[i].y;
  }
  recognizer->Crop(src_image, point_data, dst_image);
  return 1;
}

uint8_t FaceIdentification::ExtractFeature(const ImageData &cropImg, 
    FaceFeatures const feats) {
  if (feats == NULL) {
    std::cout << "Face Recognizer: 'feats' must be initialized with size \
           of GetFeatureSize(). " << std::endl;
    return 0;
  }
  recognizer->ExtractFeature(cropImg.data, feats);
  return 1;
}

uint8_t FaceIdentification::ExtractFeatureWithCrop(const ImageData &src_image, 
    const FacialLandmark* llpoint, 
    FaceFeatures const feats) {
  float point_data[10];
  for (int i = 0; i < 5; ++i) {
	point_data[i * 2] = llpoint[i].x;
	point_data[i * 2 + 1] = llpoint[i].y;
	//printf("%f %f\n", llpoint[i].x, llpoint[i].y);
  }
  recognizer->ExtractFeatureWithCrop(src_image, point_data, feats);
  return 1;
}

float FaceIdentification::CalcSimilarity(FaceFeatures const fc1,
    FaceFeatures const fc2,
    long dim) {
  if (dim == -1) {
    dim = recognizer->feature_size();
  }
  return simd_dot(fc1, fc2, dim)
	  / (sqrt(simd_dot(fc1, fc1, dim))
	  * sqrt(simd_dot(fc2, fc2, dim)));
}



#undef ERROR

#endif // _FACEIDENTIFICATION_INL_
