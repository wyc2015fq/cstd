#ifndef CAFFE_COMMON_HPP_
#define CAFFE_COMMON_HPP_

#include "cnn_config.h"
#include <wstd/flags.hpp>
#include <climits>
#include <cmath>
#include <fstream>  // NOLINT(readability/streams)
#include <iostream>  // NOLINT(readability/streams)
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>  // pair
#include <vector>
#include <process.h>
#include <direct.h>

#include "device_alternate.hpp"
#include "rng.hpp"

// Convert macro to string
#define STRINGIFY(m) #m
#define AS_STRING(m) STRINGIFY(m)


// Disable the copy and assignment operator for a class.
#define DISABLE_COPY_AND_ASSIGN(classname) \
private:\
  classname(const classname&);\
  classname& operator=(const classname&)

// Instantiate a class with float and double specifications.
#define INSTANTIATE_CLASS(classname) \
  char gInstantiationGuard##classname; \
  template class classname<float>; \
  template class classname<double>

#define INSTANTIATE_LAYER_GPU_FORWARD(classname) \
  template void classname<float>::Forward_gpu( \
      const std::vector<Blob<float>*>& bottom, \
      const std::vector<Blob<float>*>& top); \
  template void classname<double>::Forward_gpu( \
      const std::vector<Blob<double>*>& bottom, \
      const std::vector<Blob<double>*>& top);

#define INSTANTIATE_LAYER_GPU_BACKWARD(classname) \
  template void classname<float>::Backward_gpu( \
      const std::vector<Blob<float>*>& top, \
      const std::vector<bool>& propagate_down, \
      const std::vector<Blob<float>*>& bottom); \
  template void classname<double>::Backward_gpu( \
      const std::vector<Blob<double>*>& top, \
      const std::vector<bool>& propagate_down, \
      const std::vector<Blob<double>*>& bottom)

#define INSTANTIATE_LAYER_GPU_FUNCS(classname) \
  INSTANTIATE_LAYER_GPU_FORWARD(classname); \
  INSTANTIATE_LAYER_GPU_BACKWARD(classname)

// A simple macro to mark codes that are not implemented, so that when the code
// is executed we will see a fatal log.
#define NOT_IMPLEMENTED LOG(FATAL) << "Not Implemented Yet"

// See PR #1236

// We will use the boost SHARED_PTR instead of the new C++11 one mainly
// because cuda does not work (at least now) well with C++11 features.
  //using std::SHARED_PTR;

  // random seeding
int64_t cluster_seedgen(void)
{
  int64_t s, seed, pid;
#ifndef _MSC_VER
  FILE* f = fopen("/dev/urandom", "rb");
  if (f && fread(&seed, 1, sizeof(seed), f) == sizeof(seed)) {
    fclose(f);
    return seed;
  }
  LOG(INFO) << "System entropy source not available, "
    "using fallback algorithm to generate seed instead.";
  if (f) {
    fclose(f);
  }
  pid = getpid();
#else
  pid = _getpid();
#endif
  s = time(NULL);
  seed = std::abs(((s * 181) * ((pid - 83) * 359)) % 104729);
  return seed;
}


void initGlog()
{
  FLAGS_log_dir = ".\\log\\";
  _mkdir(FLAGS_log_dir.c_str());
  std::string LOG_INFO_FILE;
  std::string LOG_WARNING_FILE;
  std::string LOG_ERROR_FILE;
  std::string LOG_FATAL_FILE;
  std::string now_time = "";// boost::posix_time::to_iso_extended_string(boost::posix_time::second_clock::local_time());
                            //now_time[13] = '-';
                            //now_time[16] = '-';
  LOG_INFO_FILE = FLAGS_log_dir + "INFO" + now_time + ".txt";
  wstd::SetLogDestination(wstd::GLOG_INFO, LOG_INFO_FILE.c_str());
  LOG_WARNING_FILE = FLAGS_log_dir + "WARNING" + now_time + ".txt";
  wstd::SetLogDestination(wstd::GLOG_WARNING, LOG_WARNING_FILE.c_str());
  LOG_ERROR_FILE = FLAGS_log_dir + "ERROR" + now_time + ".txt";
  wstd::SetLogDestination(wstd::GLOG_ERROR, LOG_ERROR_FILE.c_str());
  LOG_FATAL_FILE = FLAGS_log_dir + "FATAL" + now_time + ".txt";
  wstd::SetLogDestination(wstd::GLOG_FATAL, LOG_FATAL_FILE.c_str());
}

// A global initialization function that you should call in your main function.
// Currently it initializes google flags and google logging.
void GlobalInit(int argc, char** argv)
{
  // wstd flags.
  wstd::ParseCommandLineFlags(argc, argv, true);
  // Provide a backtrace on segfault.
  //::wstd::InstallFailureSignalHandler();
  // wstd logging.
  initGlog();
  ::wstd::InitGoogleLogging(argv[0]);
}

// A singleton class to hold common caffe stuff, such as the handler that
// caffe is going to use for cublas, curand, etc.
enum Brew { CPU, GPU };

struct Caffe
{
#ifndef CPU_ONLY
  cublasHandle_t cublas_handle_;
  curandGenerator_t curand_generator_;
#endif
  //SHARED_PTR<RNG> random_generator_;
  Brew mode_;
  // Parallel training
  int solver_count_;
  int solver_rank_;
  bool multiprocess_;
  Caffe();
  ~Caffe();
};

// Thread local context for Caffe. Moved to common.cpp instead of
// including boost/thread.hpp to avoid a boost/NVCC issues (#1009, #1010)
// on OSX. Also fails on Linux with CUDA 7.0.18.


static Caffe & Get()
{
  // Make sure each thread can have different values.
  static Caffe thread_instance_;
  return thread_instance_;
}

#ifdef CPU_ONLY  // CPU-only Caffe.

Caffe::Caffe() {
  Caffe* s = this;
  s->mode_ = (CPU),
    s->solver_count_ = (1), s->solver_rank_ = (0), s->multiprocess_ = (false);
}

Caffe::~Caffe() {}


static void set_random_seed(const unsigned int seed)
{
  // RNG seed
  caffe_rng()->seed(seed);
}

static void SetDevice(const int device_id)
{
  NO_GPU;
}

static void DeviceQuery()
{
  NO_GPU;
}

static bool CheckDevice(const int device_id)
{
  NO_GPU;
  return false;
}

static int FindDevice(const int start_id = 0)
{
  NO_GPU;
  return -1;
}


#else  // Normal GPU + CPU Caffe.

inline static cublasHandle_t cublas_handle() { return Get().cublas_handle_; }
inline static curandGenerator_t curand_generator() {
  return Get().curand_generator_;
}

Caffe::Caffe() {
  Caffe* s = this;
  cublas_handle_ = (NULL), s->curand_generator_ = (NULL),
    s->mode_ = (CPU),
    s->solver_count_ = (1), s->solver_rank_ = (0), s->multiprocess_ = (false);
  // Try to create a cublas handler, and report an error if failed (but we will
// keep the program running as one might just want to run CPU code).
  if (cublasCreate(&s->cublas_handle_) != CUBLAS_STATUS_SUCCESS) {
    LOG(ERROR) << "Cannot create Cublas handle. Cublas won't be available.";
  }
  // Try to create a curand handler.
  if (curandCreateGenerator(&s->curand_generator_, CURAND_RNG_PSEUDO_DEFAULT)
    != CURAND_STATUS_SUCCESS ||
    curandSetPseudoRandomGeneratorSeed(s->curand_generator_, cluster_seedgen())
    != CURAND_STATUS_SUCCESS) {
    LOG(ERROR) << "Cannot create Curand generator. Curand won't be available.";
  }
}

Caffe::~Caffe()
{
  if (cublas_handle_) { CUBLAS_CHECK(cublasDestroy(cublas_handle_)); }
  if (curand_generator_) {
    CURAND_CHECK(curandDestroyGenerator(curand_generator_));
  }
}

static void set_random_seed(const unsigned int seed)
{
  // Curand seed
  static bool g_curand_availability_logged = false;
  if (Get().curand_generator_) {
    CURAND_CHECK(curandSetPseudoRandomGeneratorSeed(curand_generator(),
      seed));
    CURAND_CHECK(curandSetGeneratorOffset(curand_generator(), 0));
  }
  else {
    if (!g_curand_availability_logged) {
      LOG(ERROR) <<
        "Curand not available. Skipping setting the curand seed.";
      g_curand_availability_logged = true;
    }
  }
  // RNG seed
  caffe_rng()->seed(seed);
}

static void SetDevice(const int device_id)
{
  int current_device;
  CUDA_CHECK(cudaGetDevice(&current_device));
  if (current_device == device_id) {
    return;
  }
  // The call to cudaSetDevice must come before any calls to Get, which
  // may perform initialization using the GPU.
  CUDA_CHECK(cudaSetDevice(device_id));
  if (Get().cublas_handle_) { CUBLAS_CHECK(cublasDestroy(Get().cublas_handle_)); }
  if (Get().curand_generator_) {
    CURAND_CHECK(curandDestroyGenerator(Get().curand_generator_));
  }
  CUBLAS_CHECK(cublasCreate(&Get().cublas_handle_));
  CURAND_CHECK(curandCreateGenerator(&Get().curand_generator_,
    CURAND_RNG_PSEUDO_DEFAULT));
  CURAND_CHECK(curandSetPseudoRandomGeneratorSeed(Get().curand_generator_,
    cluster_seedgen()));
}

static void DeviceQuery()
{
  cudaDeviceProp prop;
  int device;
  if (cudaSuccess != cudaGetDevice(&device)) {
    printf("No cuda device present.\n");
    return;
  }
  CUDA_CHECK(cudaGetDeviceProperties(&prop, device));
  LOG(INFO) << "Device id:                     " << device;
  LOG(INFO) << "Major revision number:         " << prop.major;
  LOG(INFO) << "Minor revision number:         " << prop.minor;
  LOG(INFO) << "Name:                          " << prop.name;
  LOG(INFO) << "Total global memory:           " << prop.totalGlobalMem;
  LOG(INFO) << "Total shared memory per block: " << prop.sharedMemPerBlock;
  LOG(INFO) << "Total registers per block:     " << prop.regsPerBlock;
  LOG(INFO) << "Warp size:                     " << prop.warpSize;
  LOG(INFO) << "Maximum memory pitch:          " << prop.memPitch;
  LOG(INFO) << "Maximum threads per block:     " << prop.maxThreadsPerBlock;
  LOG(INFO) << "Maximum dimension of block:    "
    << prop.maxThreadsDim[0] << ", " << prop.maxThreadsDim[1] << ", "
    << prop.maxThreadsDim[2];
  LOG(INFO) << "Maximum dimension of grid:     "
    << prop.maxGridSize[0] << ", " << prop.maxGridSize[1] << ", "
    << prop.maxGridSize[2];
  LOG(INFO) << "Clock rate:                    " << prop.clockRate;
  LOG(INFO) << "Total constant memory:         " << prop.totalConstMem;
  LOG(INFO) << "Texture alignment:             " << prop.textureAlignment;
  LOG(INFO) << "Concurrent copy and execution: "
    << (prop.deviceOverlap ? "Yes" : "No");
  LOG(INFO) << "Number of multiprocessors:     " << prop.multiProcessorCount;
  LOG(INFO) << "Kernel execution timeout:      "
    << (prop.kernelExecTimeoutEnabled ? "Yes" : "No");
  return;
}

static bool CheckDevice(const int device_id)
{
  // This function checks the availability of GPU #device_id.
  // It attempts to create a context on the device by calling cudaFree(0).
  // cudaSetDevice() alone is not sufficient to check the availability.
  // It lazily records device_id, however, does not initialize a
  // context. So it does not know if the host thread has the permission to use
  // the device or not.
  //
  // In a shared environment where the devices are set to EXCLUSIVE_PROCESS
  // or EXCLUSIVE_THREAD mode, cudaSetDevice() returns cudaSuccess
  // even if the device is exclusively occupied by another process or thread.
  // Cuda operations that initialize the context are needed to check
  // the permission. cudaFree(0) is one of those with no side effect,
  // except the context initialization.
  bool r = ((cudaSuccess == cudaSetDevice(device_id)) &&
    (cudaSuccess == cudaFree(0)));
  // reset any error that may have occurred.
  cudaGetLastError();
  return r;
}

static int FindDevice(const int start_id = 0)
{
  // This function finds the first available device by checking devices with
  // ordinal from start_id to the highest available value. In the
  // EXCLUSIVE_PROCESS or EXCLUSIVE_THREAD mode, if it succeeds, it also
  // claims the device due to the initialization of the context.
  int count = 0;
  CUDA_CHECK(cudaGetDeviceCount(&count));
  for (int i = start_id; i < count; i++) {
    if (CheckDevice(i)) { return i; }
  }
  return -1;
}

const char* cublasGetErrorString(cublasStatus_t error)
{
  switch (error) {
  case CUBLAS_STATUS_SUCCESS:
    return "CUBLAS_STATUS_SUCCESS";
  case CUBLAS_STATUS_NOT_INITIALIZED:
    return "CUBLAS_STATUS_NOT_INITIALIZED";
  case CUBLAS_STATUS_ALLOC_FAILED:
    return "CUBLAS_STATUS_ALLOC_FAILED";
  case CUBLAS_STATUS_INVALID_VALUE:
    return "CUBLAS_STATUS_INVALID_VALUE";
  case CUBLAS_STATUS_ARCH_MISMATCH:
    return "CUBLAS_STATUS_ARCH_MISMATCH";
  case CUBLAS_STATUS_MAPPING_ERROR:
    return "CUBLAS_STATUS_MAPPING_ERROR";
  case CUBLAS_STATUS_EXECUTION_FAILED:
    return "CUBLAS_STATUS_EXECUTION_FAILED";
  case CUBLAS_STATUS_INTERNAL_ERROR:
    return "CUBLAS_STATUS_INTERNAL_ERROR";
#if CUDA_VERSION >= 6000
  case CUBLAS_STATUS_NOT_SUPPORTED:
    return "CUBLAS_STATUS_NOT_SUPPORTED";
#endif
#if CUDA_VERSION >= 6050
  case CUBLAS_STATUS_LICENSE_ERROR:
    return "CUBLAS_STATUS_LICENSE_ERROR";
#endif
  }
  return "Unknown cublas status";
}

const char* curandGetErrorString(curandStatus_t error)
{
  switch (error) {
  case CURAND_STATUS_SUCCESS:
    return "CURAND_STATUS_SUCCESS";
  case CURAND_STATUS_VERSION_MISMATCH:
    return "CURAND_STATUS_VERSION_MISMATCH";
  case CURAND_STATUS_NOT_INITIALIZED:
    return "CURAND_STATUS_NOT_INITIALIZED";
  case CURAND_STATUS_ALLOCATION_FAILED:
    return "CURAND_STATUS_ALLOCATION_FAILED";
  case CURAND_STATUS_TYPE_ERROR:
    return "CURAND_STATUS_TYPE_ERROR";
  case CURAND_STATUS_OUT_OF_RANGE:
    return "CURAND_STATUS_OUT_OF_RANGE";
  case CURAND_STATUS_LENGTH_NOT_MULTIPLE:
    return "CURAND_STATUS_LENGTH_NOT_MULTIPLE";
  case CURAND_STATUS_DOUBLE_PRECISION_REQUIRED:
    return "CURAND_STATUS_DOUBLE_PRECISION_REQUIRED";
  case CURAND_STATUS_LAUNCH_FAILURE:
    return "CURAND_STATUS_LAUNCH_FAILURE";
  case CURAND_STATUS_PREEXISTING_FAILURE:
    return "CURAND_STATUS_PREEXISTING_FAILURE";
  case CURAND_STATUS_INITIALIZATION_FAILED:
    return "CURAND_STATUS_INITIALIZATION_FAILED";
  case CURAND_STATUS_ARCH_MISMATCH:
    return "CURAND_STATUS_ARCH_MISMATCH";
  case CURAND_STATUS_INTERNAL_ERROR:
    return "CURAND_STATUS_INTERNAL_ERROR";
  }
  return "Unknown curand status";
}

#endif  // CPU_ONLY

// Returns the mode: running on CPU or GPU.
inline static Brew mode() { return Get().mode_; }
// The setters for the variables
// Sets the mode. It is recommended that you don't change the mode halfway
// into the program since that may cause allocation of pinned memory being
// freed in a non-pinned way, which may cause problems - I haven't verified
// it personally but better to note it here in the header file.
inline static void set_mode(Brew mode) { Get().mode_ = mode; }

// Parallel training
inline static int solver_count() { return Get().solver_count_; }
inline static void set_solver_count(int val) { Get().solver_count_ = val; }
inline static int solver_rank() { return Get().solver_rank_; }
inline static void set_solver_rank(int val) { Get().solver_rank_ = val; }
inline static bool multiprocess() { return Get().multiprocess_; }
inline static void set_multiprocess(bool val) { Get().multiprocess_ = val; }
inline static bool root_solver() { return Get().solver_rank_ == 0; }


#endif  // CAFFE_COMMON_HPP_
