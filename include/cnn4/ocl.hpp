
#ifndef _OCL_HPP_
#define _OCL_HPP_

#include "rng.hpp"
//#define USE_CUDNN
#ifdef __APPLE__
#include <opencl/cl.h>
#else
#include <CL/cl.h>
#endif
#ifdef USE_CUDNN  // cuDNN acceleration library.
#include "cudnn.hpp"
#endif

// CUDA macros
// CUDA: various checks for different function calls.
#define OCL_CHECK(condition) \
  /* Code block avoids redefinition of oclError_t error */ \
  do { \
    oclError_t error = condition; \
    CHECK_EQ(error, oclSuccess) << " " << oclGetErrorString(error); \
  } while (0)

// CUDA: grid stride looping
#define OCL_KERNEL_LOOP(i, n) \
  for (int i = blockIdx.x * blockDim.x + threadIdx.x; \
       i < (n); i += blockDim.x * gridDim.x)

// CUDA: check for error after kernel execution and exit loudly if there is one.
#define OCL_POST_KERNEL_CHECK OCL_CHECK(oclPeekAtLastError())


// CUDA: use 512 threads per block
static const int CAFFE_OCL_NUM_THREADS = 512;

// CUDA: number of blocks for threads.
static int CAFFE_GET_BLOCKS(const int N)
{
  return (N + CAFFE_OCL_NUM_THREADS - 1) / CAFFE_OCL_NUM_THREADS;
}

//为cpu平台创建上下文
cl_context createContext()
{
  cl_platform_id firstPlatformId = 0;
  cl_uint numPlatforms = 0;
  //这里选择第一个平台
  cl_int errNum = clGetPlatformIDs(1, &firstPlatformId, &numPlatforms);
  if (errNum != CL_SUCCESS || numPlatforms <= 0)
  {
    std::cerr << "Failed to find any opencl platforms." << std::endl;
    return NULL;
  }
  else
  {
    std::cout << "有opencl平台" << std::endl;
  }

  //创建平台的一个上下文，先试图创建一个gpu的，如果没有的话，就创建cpu的
  cl_context_properties contextProperties[] =
  {
    CL_CONTEXT_PLATFORM,
    (cl_context_properties)firstPlatformId,
    0
  };
  cl_context context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_GPU, NULL, NULL, &errNum);
  if (errNum != CL_SUCCESS)
  {
    std::cout << "不能创建gpu上下文 ，尝试CPU..." << std::endl;
    context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_CPU, NULL, NULL, &errNum);
    if (errNum != CL_SUCCESS)
    {
      std::cerr << "不能创建opencl gpu或者cpu上下文";
      return NULL;
    }
    else
    {
      std::cout << "能创建cpu上下文" << std::endl;
    }
  }
  else
  {
    std::cout << "能创建gpu上下文" << std::endl;
  }
  return context;
}

struct OclObject {
  OclObject() {
  }
  ~OclObject() {
  }
  static OclObject& Get() {
    static  OclObject g_ocl;
    return g_ocl;
  }
};

#define CUDAOBJECT  OclObject::Get()

static OclObject& g_ocl = CUDAOBJECT;

static void ocl_set_random_seed(const unsigned int seed)
{
  // RNG seed
  //caffe_rng()->seed(seed);
}

static void SetDevice(const int device_id)
{
}

static void DeviceQuery()
{
  return;
}

static bool CheckDevice(const int device_id)
{
  return true;
}

static int FindDevice(const int start_id = 0)
{
  return -1;
}

static const int OCL_NUM_THREADS = 1024;
#define MAX_GPUS 8

#define OCL_VERSION_MIN(major, minor, patch) \
    (OCL_VERSION >= (major * 1000 + minor * 100 + patch))

#define OCL_VERSION_MAX(major, minor, patch) \
    (OCL_VERSION < (major * 1000 + minor * 100 + patch))


#ifdef WITH_MPI_NCCL
#define NCCL_CHECK(condition) \
  do { \
    ncclResult_t status = condition; \
    CHECK_EQ(status, ncclSuccess) << "\n" << ncclGetErrorString(status); \
  } while (0)
#endif  // WITH_MPI_NCCL

inline int GET_BLOCKS(const int N) {
  return (N + OCL_NUM_THREADS - 1) / OCL_NUM_THREADS;
}

#define OCL_POST_KERNEL_CHECK OCL_CHECK(oclPeekAtLastError())

#if OCL_VERSION_MAX(9, 0, 0)
#define __hdiv hdiv
#endif

inline int NUM_DEVICES() {
  static int count = -1;
  if (count < 0) {
  }
  return count;
}

inline int CURRENT_DEVICE() {
  return ocl_id;
}

inline int POINTER_DEVICE(const void* ptr) {
  return attr.device;
}

class DeviceGuard {
public:
  DeviceGuard(int newDevice) : previous_(CURRENT_DEVICE()) {
    if (previous_ != newDevice)
      OCL_CHECK(oclSetDevice(newDevice));
  }
  ~DeviceGuard() {
    OCL_CHECK(oclSetDevice(previous_));
  }

private:
  int previous_;
};

static int allsz = 0;
static int freesz = 0;
//CPUContext() { context[CPU] = this;    }
static void ocl_ReAlloc(Buffer* ptr, size_t nbytes) {
  //nbytes *= 10;
  ptr->brew = GPU;
  if (ptr->size < nbytes) {
    void* newdata = NULL;
    OCL_CHECK(oclMalloc(&newdata, nbytes));
    if (NULL == newdata) {
      //int asdf = 0;
    }
    CHECK(newdata) << "Malloc ocl mem: " << nbytes << " bytes failed.";
    allsz += nbytes;
    if (ptr->data) {
      freesz += ptr->size;
      OCL_CHECK(oclMemcpy(newdata, ptr->data, ptr->size, oclMemcpyDeviceToDevice));
      oclFree(ptr->data);
    }
    ptr->data = newdata;
    ptr->size = nbytes;
  }
  else {
    //int asdf = 0;
  }
  return;
}

static void ocl_Free(Buffer* ptr) {
  if (ptr->data) {
    oclFree(ptr->data);
    ptr->data = NULL;
    ptr->size = 0;
  }
}
static void ocl_Memset(Buffer* ptr, size_t nbytes) {
  CHECK_LE(nbytes, ptr->size);
  oclMemset(ptr->data, 0, nbytes);
}

static void ocl_Memcpy(Buffer* dst, const Buffer* src, int nbytes) {
  enum oclMemcpyKind kind;
  CHECK_LE(nbytes, src->size);
  CHECK_LE(nbytes, dst->size);
  if (GPU == dst->brew && GPU == src->brew) {
    kind = oclMemcpyDeviceToDevice;
  }
  else if (CPU == dst->brew && GPU == src->brew) {
    kind = oclMemcpyHostToHost;
  }
  else if (GPU == dst->brew && CPU == src->brew) {
    kind = oclMemcpyHostToHost;
  }
  OCL_CHECK(oclMemcpy(dst->data, src->data, nbytes, kind));
}
#if 0
static void MemcpyAsync(Buffer* dst, const Buffer* src, Stream* stream) {
  OCL_CHECK(oclMemcpyAsync(dst, src, nbytes, oclMemcpyDefault, (oclStream_t)stream));
}

static void* BeginStream(Buffer* context) {
  oclStream_t stream;
  OCL_CHECK(oclStreamCreateWithFlags(&stream, oclStreamNonBlocking));
  return (void*)stream;
}
static void EndStream(Buffer* context, void* stream) {
  OCL_CHECK(oclStreamDestroy((oclStream_t)stream));
}
static void Synchronize(Buffer* context, void* stream) {
  OCL_CHECK(oclStreamSynchronize((oclStream_t)stream));
}
#endif

#undef Get

#endif // _OCL_HPP_
