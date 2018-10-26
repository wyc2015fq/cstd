#ifndef CAFFE_SYNCEDMEM_HPP_
#define CAFFE_SYNCEDMEM_HPP_

#include <cstdlib>

#include "common.hpp"
#include "wstd/logging.hpp"
#include "math_functions.hpp"

// If CUDA is available and in GPU mode, host memory will be allocated pinned,
// using cudaMallocHost. It avoids dynamic pinning for transfers (DMA).
// The improvement in performance seems negligible in the single GPU case,
// but might be more significant for parallel training. Most importantly,
// it improved stability for large models on many GPUs.
inline void CaffeMallocHost(void** ptr, size_t size, bool* use_cuda)
{
#ifndef CPU_ONLY
  if (mode() == GPU) {
    CUDA_CHECK(cudaMallocHost(ptr, size));
    *use_cuda = true;
    return;
  }
#endif
  *ptr = malloc(size);
  *use_cuda = false;
  CHECK(*ptr) << "host allocation of size " << size << " failed";
}

inline void CaffeFreeHost(void* ptr, bool use_cuda)
{
#ifndef CPU_ONLY
  if (use_cuda) {
    CUDA_CHECK(cudaFreeHost(ptr));
    return;
  }
#endif
  free(ptr);
}


/**
 * @brief Manages memory allocation and synchronization between the host (CPU)
 *        and device (GPU).
 *
 * TODO(dox): more thorough description.
 */
class SyncedMemory
{
public:
  SyncedMemory() { reset(0); }
  explicit SyncedMemory(size_t size) { reset(size); }
  ~SyncedMemory() { free(); }
  void reset(size_t size) {
    free();
    cpu_ptr_ = (NULL), gpu_ptr_ = (NULL), size_ = (size), head_ = (UNINITIALIZED),
      own_cpu_data_ = (false), cpu_malloc_use_cuda_ = (false), own_gpu_data_ = (false),
      gpu_device_ = (-1);
  }
  void free() {
    if (cpu_ptr_ && own_cpu_data_) {
      CaffeFreeHost(cpu_ptr_, cpu_malloc_use_cuda_);
    }
#ifndef CPU_ONLY
    if (gpu_ptr_ && own_gpu_data_) {
      int initial_device;
      cudaGetDevice(&initial_device);
      if (gpu_device_ != -1) {
        CUDA_CHECK(cudaSetDevice(gpu_device_));
      }
      CUDA_CHECK(cudaFree(gpu_ptr_));
      cudaSetDevice(initial_device);
    }
#endif  // CPU_ONLY
  }
  const void* cpu_data() 
  {
    to_cpu();
    return (const void*)cpu_ptr_;
  }
  void set_cpu_data(void* data)
  {
    CHECK(data);
    if (own_cpu_data_) {
      CaffeFreeHost(cpu_ptr_, cpu_malloc_use_cuda_);
    }
    cpu_ptr_ = data;
    head_ = HEAD_AT_CPU;
    own_cpu_data_ = false;
  }
  const void* gpu_data() const
  {
#ifndef CPU_ONLY
    to_gpu();
    return (const void*)gpu_ptr_;
#else
    NO_GPU;
    return NULL;
#endif
  }
  void set_gpu_data(void* data)
  {
#ifndef CPU_ONLY
    CHECK(data);
    if (own_gpu_data_) {
      int initial_device;
      cudaGetDevice(&initial_device);
      if (gpu_device_ != -1) {
        CUDA_CHECK(cudaSetDevice(gpu_device_));
      }
      CUDA_CHECK(cudaFree(gpu_ptr_));
      cudaSetDevice(initial_device);
    }
    gpu_ptr_ = data;
    head_ = HEAD_AT_GPU;
    own_gpu_data_ = false;
#else
    NO_GPU;
#endif
  }
  void* mutable_cpu_data()
  {
    to_cpu();
    head_ = HEAD_AT_CPU;
    return cpu_ptr_;
  }
  void* mutable_gpu_data()
  {
#ifndef CPU_ONLY
    to_gpu();
    head_ = HEAD_AT_GPU;
    return gpu_ptr_;
#else
    NO_GPU;
    return NULL;
#endif
  }
  enum SyncedHead { UNINITIALIZED, HEAD_AT_CPU, HEAD_AT_GPU, SYNCED };
  SyncedHead head() { return head_; }
  size_t size() { return size_; }

#ifndef CPU_ONLY
  void async_gpu_push(const cudaStream_t & stream)
  {
    CHECK(head_ == HEAD_AT_CPU);
    if (gpu_ptr_ == NULL) {
      CUDA_CHECK(cudaGetDevice(&gpu_device_));
      CUDA_CHECK(cudaMalloc(&gpu_ptr_, size_));
      own_gpu_data_ = true;
    }
    const cudaMemcpyKind put = cudaMemcpyHostToDevice;
    CUDA_CHECK(cudaMemcpyAsync(gpu_ptr_, cpu_ptr_, size_, put, stream));
    // Assume caller will synchronize on the stream before use
    head_ = SYNCED;
  }
#endif

public:
  void to_cpu()
  {
    switch (head_) {
    case UNINITIALIZED:
      CaffeMallocHost(&cpu_ptr_, size_, &cpu_malloc_use_cuda_);
      caffe_memset(size_, 0, cpu_ptr_);
      head_ = HEAD_AT_CPU;
      own_cpu_data_ = true;
      break;
    case HEAD_AT_GPU:
#ifndef CPU_ONLY
      if (cpu_ptr_ == NULL) {
        CaffeMallocHost(&cpu_ptr_, size_, &cpu_malloc_use_cuda_);
        own_cpu_data_ = true;
      }
      caffe_gpu_memcpy(size_, gpu_ptr_, cpu_ptr_);
      head_ = SYNCED;
#else
      NO_GPU;
#endif
      break;
    case HEAD_AT_CPU:
    case SYNCED:
      break;
    }
  }
  void to_gpu()
  {
#ifndef CPU_ONLY
    switch (head_) {
    case UNINITIALIZED:
      CUDA_CHECK(cudaGetDevice(&gpu_device_));
      CUDA_CHECK(cudaMalloc(&gpu_ptr_, size_));
      caffe_gpu_memset(size_, 0, gpu_ptr_);
      head_ = HEAD_AT_GPU;
      own_gpu_data_ = true;
      break;
    case HEAD_AT_CPU:
      if (gpu_ptr_ == NULL) {
        CUDA_CHECK(cudaGetDevice(&gpu_device_));
        CUDA_CHECK(cudaMalloc(&gpu_ptr_, size_));
        own_gpu_data_ = true;
      }
      caffe_gpu_memcpy(size_, cpu_ptr_, gpu_ptr_);
      head_ = SYNCED;
      break;
    case HEAD_AT_GPU:
    case SYNCED:
      break;
    }
#else
    NO_GPU;
#endif
  }
  void* cpu_ptr_;
  void* gpu_ptr_;
  size_t size_;
  SyncedHead head_;
  bool own_cpu_data_;
  bool cpu_malloc_use_cuda_;
  bool own_gpu_data_;
  int gpu_device_;

};  // class SyncedMemory


#endif  // CAFFE_SYNCEDMEM_HPP_
