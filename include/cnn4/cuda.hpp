
#ifndef _CUDA_HPP_
#define _CUDA_HPP_

#include "rng.hpp"
//#define USE_CUDNN
#include <cublas_v2.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <curand.h>
#include <driver_types.h>  // cuda driver types
#ifdef USE_CUDNN  // cuDNN acceleration library.
#include "cudnn.hpp"
#endif

// CUDA macros


// CUDA: various checks for different function calls.
#define CUDA_CHECK(condition) \
  /* Code block avoids redefinition of cudaError_t error */ \
  do { \
    cudaError_t error = condition; \
    CHECK_EQ(error, cudaSuccess) << " " << cudaGetErrorString(error); \
  } while (0)

#define CUBLAS_CHECK(condition) \
  do { \
    cublasStatus_t status = condition; \
    CHECK_EQ(status, CUBLAS_STATUS_SUCCESS) << " " \
      << cublasGetErrorString(status); \
  } while (0)

#define CURAND_CHECK(condition) \
  do { \
    curandStatus_t status = condition; \
    CHECK_EQ(status, CURAND_STATUS_SUCCESS) << " " \
      << curandGetErrorString(status); \
  } while (0)

// CUDA: grid stride looping
#define CUDA_KERNEL_LOOP(i, n) \
  for (int i = blockIdx.x * blockDim.x + threadIdx.x; \
       i < (n); i += blockDim.x * gridDim.x)

// CUDA: check for error after kernel execution and exit loudly if there is one.
#define CUDA_POST_KERNEL_CHECK CUDA_CHECK(cudaPeekAtLastError())


// CUDA: use 512 threads per block
static const int CAFFE_CUDA_NUM_THREADS = 512;

// CUDA: number of blocks for threads.
static int CAFFE_GET_BLOCKS(const int N)
{
  return (N + CAFFE_CUDA_NUM_THREADS - 1) / CAFFE_CUDA_NUM_THREADS;
}


static const char* cublasGetErrorString(cublasStatus_t error)
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

static const char* curandGetErrorString(curandStatus_t error)
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
#define curand_generator()  CUDAOBJECT.curand_generator_
#define cublas_handle()  CUDAOBJECT.cublas_handle_
struct CudaObject {
  cublasHandle_t cublas_handle_;
  curandGenerator_t curand_generator_;
  CudaObject() {
    cublas_handle_ = (NULL);
    curand_generator_ = (NULL);
    // Try to create a cublas handler, and report an error if failed (but we will
    // keep the program running as one might just want to run CPU code).
    if (cublasCreate(&cublas_handle_) != CUBLAS_STATUS_SUCCESS) {
      LOG(ERROR) << "Cannot create Cublas handle. Cublas won't be available.";
    }
    // Try to create a curand handler.
    if (curandCreateGenerator(&curand_generator_, CURAND_RNG_PSEUDO_DEFAULT) != CURAND_STATUS_SUCCESS ||
      curandSetPseudoRandomGeneratorSeed(curand_generator_, cluster_seedgen()) != CURAND_STATUS_SUCCESS) {
      LOG(ERROR) << "Cannot create Curand generator. Curand won't be available.";
    }
  }
  ~CudaObject() {
    if (cublas_handle_) {
      CUBLAS_CHECK(cublasDestroy(cublas_handle_)); }
    if (curand_generator_) {
      CURAND_CHECK(curandDestroyGenerator(curand_generator_));
    }
  }
  static CudaObject& Get() {
    static  CudaObject g_cuda;
    return g_cuda;
  }
};

#define CUDAOBJECT  CudaObject::Get()

static CudaObject& g_cuda = CUDAOBJECT;

static void cuda_set_random_seed(const unsigned int seed)
{
  // Curand seed
  static bool g_curand_availability_logged = false;
  if (CUDAOBJECT.curand_generator_) {
    CURAND_CHECK(curandSetPseudoRandomGeneratorSeed(CUDAOBJECT.curand_generator_,
      seed));
    CURAND_CHECK(curandSetGeneratorOffset(CUDAOBJECT.curand_generator_, 0));
  }
  else {
    if (!g_curand_availability_logged) {
      LOG(ERROR) <<
        "Curand not available. Skipping setting the curand seed.";
      g_curand_availability_logged = true;
    }
  }
  // RNG seed
  //caffe_rng()->seed(seed);
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
  if (CUDAOBJECT.cublas_handle_) { CUBLAS_CHECK(cublasDestroy(CUDAOBJECT.cublas_handle_)); }
  if (CUDAOBJECT.curand_generator_) {
    CURAND_CHECK(curandDestroyGenerator(CUDAOBJECT.curand_generator_));
  }
  CUBLAS_CHECK(cublasCreate(&CUDAOBJECT.cublas_handle_));
  CURAND_CHECK(curandCreateGenerator(&CUDAOBJECT.curand_generator_,
    CURAND_RNG_PSEUDO_DEFAULT));
  CURAND_CHECK(curandSetPseudoRandomGeneratorSeed(CUDAOBJECT.curand_generator_,
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

static int aaaa() {
  int n = FindDevice();
  SetDevice(0);
  return 0;
}
static int ttt = aaaa();



static const int CUDA_NUM_THREADS = 1024;
#define MAX_GPUS 8

#define CUDA_VERSION_MIN(major, minor, patch) \
    (CUDA_VERSION >= (major * 1000 + minor * 100 + patch))

#define CUDA_VERSION_MAX(major, minor, patch) \
    (CUDA_VERSION < (major * 1000 + minor * 100 + patch))


#ifdef WITH_MPI_NCCL
#define NCCL_CHECK(condition) \
  do { \
    ncclResult_t status = condition; \
    CHECK_EQ(status, ncclSuccess) << "\n" << ncclGetErrorString(status); \
  } while (0)
#endif  // WITH_MPI_NCCL

inline int GET_BLOCKS(const int N) {
  return (N + CUDA_NUM_THREADS - 1) / CUDA_NUM_THREADS;
}

#define CUDA_POST_KERNEL_CHECK CUDA_CHECK(cudaPeekAtLastError())

#if CUDA_VERSION_MAX(9, 0, 0)
#define __hdiv hdiv
#endif

inline int NUM_DEVICES() {
  static int count = -1;
  if (count < 0) {
    auto err = cudaGetDeviceCount(&count);
    if (err == cudaErrorNoDevice || err == cudaErrorInsufficientDriver) count = 0;
  }
  return count;
}

inline int CURRENT_DEVICE() {
  int cuda_id;
  cudaGetDevice(&cuda_id);
  return cuda_id;
}

inline int POINTER_DEVICE(const void* ptr) {
  cudaPointerAttributes attr;
  CUDA_CHECK(cudaPointerGetAttributes(&attr, ptr));
  return attr.device;
}

class DeviceGuard {
public:
  DeviceGuard(int newDevice) : previous_(CURRENT_DEVICE()) {
    if (previous_ != newDevice)
      CUDA_CHECK(cudaSetDevice(newDevice));
  }
  ~DeviceGuard() {
    CUDA_CHECK(cudaSetDevice(previous_)); }

private:
  int previous_;
};

static int allsz = 0;
static int freesz = 0;
//CPUContext() { context[CPU] = this;    }
static void cuda_ReAlloc(Buffer* ptr, size_t nbytes) {
  //nbytes *= 10;
  ptr->brew = GPU;
  if (ptr->size < nbytes) {
    void* newdata = NULL;
    CUDA_CHECK(cudaMalloc(&newdata, nbytes));
    if (NULL == newdata) {
      int asdf = 0;
    }
    CHECK(newdata) << "Malloc cuda mem: " << nbytes << " bytes failed.";
    allsz += nbytes;
    if (ptr->data) {
      freesz += ptr->size;
      CUDA_CHECK(cudaMemcpy(newdata, ptr->data, ptr->size, cudaMemcpyDeviceToDevice));
      cudaFree(ptr->data);
    }
    ptr->data = newdata;
    ptr->size = nbytes;
  }
  else {
    //int asdf = 0;
  }
  return ;
}

static void cuda_Free(Buffer* ptr) {
  if (ptr->data) {
    cudaFree(ptr->data);
    ptr->data = NULL;
    ptr->size = 0;
  }
}
static void cuda_Memset(Buffer* ptr, size_t nbytes) {
  CHECK_LE(nbytes, ptr->size);
  cudaMemset(ptr->data, 0, nbytes);
}

static void cuda_Memcpy(Buffer* dst, const Buffer* src, int nbytes) {
  enum cudaMemcpyKind kind;
  CHECK_LE(nbytes, src->size);
  CHECK_LE(nbytes, dst->size);
  if (GPU == dst->brew && GPU == src->brew) {
    kind = cudaMemcpyDeviceToDevice;
  }
  else if (CPU == dst->brew && GPU == src->brew) {
    kind = cudaMemcpyHostToHost;
  } 
  else if (GPU == dst->brew && CPU == src->brew) {
    kind = cudaMemcpyHostToHost;
  } 
  CUDA_CHECK(cudaMemcpy(dst->data, src->data, nbytes, kind));
}
#if 0
static void MemcpyAsync(Buffer* dst, const Buffer* src, Stream* stream) {
  CUDA_CHECK(cudaMemcpyAsync(dst, src, nbytes, cudaMemcpyDefault, (cudaStream_t)stream));
}

static void* BeginStream(Buffer* context) {
  cudaStream_t stream;
  CUDA_CHECK(cudaStreamCreateWithFlags(&stream, cudaStreamNonBlocking));
  return (void*)stream;
}
static void EndStream(Buffer* context, void* stream) {
  CUDA_CHECK(cudaStreamDestroy((cudaStream_t)stream));
}
static void Synchronize(Buffer* context, void* stream) {
  CUDA_CHECK(cudaStreamSynchronize((cudaStream_t)stream));
}
#endif

#undef Get

#endif // _CUDA_HPP_
