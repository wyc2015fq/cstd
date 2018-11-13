

#include <thrust/device_vector.h>
#include <thrust/functional.h>  // thrust::plus
#include <thrust/reduce.h>
#include <cmath>
#include "wstd/logging.hpp"
#include "cpu.hpp"
#include "math_functions.h"



void gpu_caffe_memset(const size_t N, const int alpha, void* X) {
  CUDA_CHECK(cudaMemset(X, alpha, N));
}

void gpu_caffe_memcpy(const size_t N, const void* X, void* Y) {
  if (X != Y) {
    CUDA_CHECK(cudaMemcpy(Y, X, N, cudaMemcpyDefault));  // NOLINT(caffe/alt_fn)
  }
}

#define FUN(NAME) gpu_##NAME
#define Stype double
#define Dtype float
#define CBLASFUN(NAME)  cublasS##NAME
#include "math_functions.cuh"
#undef CBLASFUN
#undef Dtype
#undef Stype
#undef FUN


#undef _CONTEXT


