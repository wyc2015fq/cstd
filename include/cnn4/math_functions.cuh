

#ifndef Dtype


void caffe_memset(_CONTEXT, const size_t N, const int alpha, void* X){
  CUDA_CHECK(cudaMemset(X, alpha, N));
}

template <>
void caffe_gemm<float>(_CONTEXT, const CBLAS_TRANSPOSE TransA,
  const CBLAS_TRANSPOSE TransB, const int M, const int N, const int K,
  const float alpha, const float* A, const float* B, const float beta,
  float* C) {
  // Note that cublas follows fortran order.
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  cublasOperation_t cuTransA =
    (TransA == CblasNoTrans) ? CUBLAS_OP_N : CUBLAS_OP_T;
  cublasOperation_t cuTransB =
    (TransB == CblasNoTrans) ? CUBLAS_OP_N : CUBLAS_OP_T;
  CUBLAS_CHECK(cublasSgemm(cublas_handle(), cuTransB, cuTransA,
    N, M, K, &alpha, B, ldb, A, lda, &beta, C, N));
}

template <>
void caffe_gemm<double>(_CONTEXT, const CBLAS_TRANSPOSE TransA,
  const CBLAS_TRANSPOSE TransB, const int M, const int N, const int K,
  const double alpha, const double* A, const double* B, const double beta,
  double* C) {
  // Note that cublas follows fortran order.
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  cublasOperation_t cuTransA =
    (TransA == CblasNoTrans) ? CUBLAS_OP_N : CUBLAS_OP_T;
  cublasOperation_t cuTransB =
    (TransB == CblasNoTrans) ? CUBLAS_OP_N : CUBLAS_OP_T;
  CUBLAS_CHECK(cublasDgemm(cublas_handle(), cuTransB, cuTransA,
    N, M, K, &alpha, B, ldb, A, lda, &beta, C, N));
}

template <>
void caffe_gemv<float>(_CONTEXT, const CBLAS_TRANSPOSE TransA, const int M,
  const int N, const float alpha, const float* A, const float* x,
  const float beta, float* y) {
  cublasOperation_t cuTransA =
    (TransA == CblasNoTrans) ? CUBLAS_OP_T : CUBLAS_OP_N;
  CUBLAS_CHECK(cublasSgemv(cublas_handle(), cuTransA, N, M, &alpha,
    A, N, x, 1, &beta, y, 1));
}

template <>
void caffe_gemv<double>(_CONTEXT, const CBLAS_TRANSPOSE TransA, const int M,
  const int N, const double alpha, const double* A, const double* x,
  const double beta, double* y) {
  cublasOperation_t cuTransA =
    (TransA == CblasNoTrans) ? CUBLAS_OP_T : CUBLAS_OP_N;
  CUBLAS_CHECK(cublasDgemv(cublas_handle(), cuTransA, N, M, &alpha,
    A, N, x, 1, &beta, y, 1));
}

template <>
void caffe_axpy<float>(_CONTEXT, const int N, const float alpha, const float* X,
  float* Y) {
  CUBLAS_CHECK(cublasSaxpy(cublas_handle(), N, &alpha, X, 1, Y, 1));
}

template <>
void caffe_axpy<double>(_CONTEXT, const int N, const double alpha, const double* X,
  double* Y) {
  CUBLAS_CHECK(cublasDaxpy(cublas_handle(), N, &alpha, X, 1, Y, 1));
}

void caffe_memcpy(_CONTEXT,const size_t N, const void* X, void* Y) {
  if (X != Y) {
    CUDA_CHECK(cudaMemcpy(Y, X, N, cudaMemcpyDefault));  // NOLINT(caffe/alt_fn)
  }
}

template <>
void caffe_scal<float>(_CONTEXT, const int N, const float alpha, float *X) {
  CUBLAS_CHECK(cublasSscal(cublas_handle(), N, &alpha, X, 1));
}

template <>
void caffe_scal<double>(_CONTEXT, const int N, const double alpha, double *X) {
  CUBLAS_CHECK(cublasDscal(cublas_handle(), N, &alpha, X, 1));
}

template <>
void caffe_axpby<float>(_CONTEXT, const int N, const float alpha, const float* X,
  const float beta, float* Y) {
  caffe_scal<float>(context, N, beta, Y);
  caffe_axpy<float>(context, N, alpha, X, Y);
}

template <>
void caffe_axpby<double>(_CONTEXT, const int N, const double alpha, const double* X,
  const double beta, double* Y) {
  caffe_scal<double>(context, N, beta, Y);
  caffe_axpy<double>(context, N, alpha, X, Y);
}

template <>
void caffe_dot<float>(_CONTEXT, const int n, const float* x, const float* y,
  float* out) {
  CUBLAS_CHECK(cublasSdot(cublas_handle(), n, x, 1, y, 1, out));
}

template <>
void caffe_dot<double>(_CONTEXT, const int n, const double* x, const double* y,
  double * out) {
  CUBLAS_CHECK(cublasDdot(cublas_handle(), n, x, 1, y, 1, out));
}

template <>
float caffe_dot<float>(_CONTEXT, const int n, const float* x, const float* y) {
  float out;
  CUBLAS_CHECK(cublasSdot(cublas_handle(), n, x, 1, y, 1, &out));
  return out;
}

template <>
double caffe_dot<double>(_CONTEXT, const int n, const double* x, const double* y) {
  double out;
  CUBLAS_CHECK(cublasDdot(cublas_handle(), n, x, 1, y, 1, &out));
  return out;
}

template <>
void caffe_asum<float>(_CONTEXT, const int n, const float* x, float* y) {
  CUBLAS_CHECK(cublasSasum(cublas_handle(), n, x, 1, y));
}

template <>
void caffe_asum<double>(_CONTEXT, const int n, const double* x, double* y) {
  CUBLAS_CHECK(cublasDasum(cublas_handle(), n, x, 1, y));
}

template <>
void caffe_scale<float>(_CONTEXT, const int n, const float alpha, const float *x,
  float* y) {
  CUBLAS_CHECK(cublasScopy(cublas_handle(), n, x, 1, y, 1));
  CUBLAS_CHECK(cublasSscal(cublas_handle(), n, &alpha, y, 1));
}

template <>
void caffe_scale<double>(_CONTEXT, const int n, const double alpha, const double *x,
  double* y) {
  CUBLAS_CHECK(cublasDcopy(cublas_handle(), n, x, 1, y, 1));
  CUBLAS_CHECK(cublasDscal(cublas_handle(), n, &alpha, y, 1));
}

template <typename Dtype>
__global__ void set_kernel(const int n, const Dtype alpha, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = alpha;
  }
}

template <typename Dtype>
__global__ void add_scalar_kernel(const int n, const Dtype alpha, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] += alpha;
  }
}

template <>
void caffe_add_scalar<float>(_CONTEXT, const int N, const float alpha, float* Y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  add_scalar_kernel<float> << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(N, alpha, Y);
}

template <>
void caffe_add_scalar<double>(_CONTEXT, const int N, const double alpha, double* Y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  add_scalar_kernel<double> << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(N, alpha, Y);
}

template <typename Dtype>
__global__ void add_kernel(const int n, const Dtype* a,
  const Dtype* b, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = a[index] + b[index];
  }
}

template <>
void caffe_add<float>(_CONTEXT, const int N, const float* a, const float* b,
  float* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  add_kernel<float> << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
    N, a, b, y);
}

template <>
void caffe_add<double>(_CONTEXT, const int N, const double* a, const double* b,
  double* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  add_kernel<double> << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
    N, a, b, y);
}

template <typename Dtype>
__global__ void sub_kernel(const int n, const Dtype* a,
  const Dtype* b, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = a[index] - b[index];
  }
}

template <typename Dtype>
__global__ void mul_kernel(const int n, const Dtype* a,
  const Dtype* b, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = a[index] * b[index];
  }
}

template <typename Dtype>
__global__ void div_kernel(const int n, const Dtype* a,
  const Dtype* b, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = a[index] / b[index];
  }
}

template <typename Dtype>
__global__ void abs_kernel(const int n, const Dtype* a, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = abs(a[index]);
  }
}

template <>
void caffe_abs<float>(_CONTEXT, const int N, const float* a, float* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  abs_kernel<float> << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
    N, a, y);
}

template <>
void caffe_abs<double>(_CONTEXT, const int N, const double* a, double* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  abs_kernel<double> << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
    N, a, y);
}


template <typename Dtype>
__global__ void exp_kernel(const int n, const Dtype* a, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = exp(a[index]);
  }
}

template <>
void caffe_exp<float>(_CONTEXT, const int N, const float* a, float* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  exp_kernel<float> << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
    N, a, y);
}

template <>
void caffe_exp<double>(_CONTEXT, const int N, const double* a, double* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  exp_kernel<double> << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
    N, a, y);
}

template <typename Dtype>
__global__ void log_kernel(const int n, const Dtype* a, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = log(a[index]);
  }
}

template <>
void caffe_log<float>(_CONTEXT, const int N, const float* a, float* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  log_kernel<float> << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
    N, a, y);
}

template <>
void caffe_log<double>(_CONTEXT, const int N, const double* a, double* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  log_kernel<double> << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
    N, a, y);
}

template <typename Dtype>
__global__ void powx_kernel(const int n, const Dtype* a,
  const Dtype alpha, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = pow(a[index], alpha);
  }
}

template <>
void caffe_powx<float>(_CONTEXT, const int N, const float* a,
  const float alpha, float* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  powx_kernel<float> << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
    N, a, alpha, y);
}

template <>
void caffe_powx<double>(_CONTEXT, const int N, const double* a,
  const double alpha, double* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  powx_kernel<double> << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
    N, a, alpha, y);
}

#define DEFINE_AND_INSTANTIATE_GPU_UNARY_FUNC(name, operation) \
template<typename Dtype> \
__global__ void name##_kernel(const int n, const Dtype* x, Dtype* y) { \
  CUDA_KERNEL_LOOP(index, n) { \
    operation; \
  } \
} \
template <> \
void caffe_##name<float>(_CONTEXT,const int n, const float* x, float* y) { \
  /* NOLINT_NEXT_LINE(whitespace/operators) */ \
  name##_kernel<float><<<CAFFE_GET_BLOCKS(n), CAFFE_CUDA_NUM_THREADS>>>( \
      n, x, y); \
} \
template <> \
void caffe_##name<double>(_CONTEXT,const int n, const double* x, double* y) { \
  /* NOLINT_NEXT_LINE(whitespace/operators) */ \
  name##_kernel<double><<<CAFFE_GET_BLOCKS(n), CAFFE_CUDA_NUM_THREADS>>>( \
      n, x, y); \
}


DEFINE_AND_INSTANTIATE_GPU_UNARY_FUNC(sign, y[index] = (Dtype(0) < x[index]) - (x[index] < Dtype(0)));
DEFINE_AND_INSTANTIATE_GPU_UNARY_FUNC(sgnbit, y[index] = signbit(x[index]));

void caffe_rng_uniform(const int n, unsigned int* r) {
  CURAND_CHECK(curandGenerate(curand_generator(), r, n));
}

template <>
void caffe_rng_uniform<float>(_CONTEXT, const int n, const float a, const float b,
  float* r) {
  CURAND_CHECK(curandGenerateUniform(curand_generator(), r, n));
  const float range = b - a;
  if (range != static_cast<float>(1)) {
    caffe_scal<float>(context, n, range, r);
  }
  if (a != static_cast<float>(0)) {
    caffe_add_scalar<float>(context, n, a, r);
  }
}

template <>
void caffe_rng_uniform<double>(_CONTEXT, const int n, const double a, const double b,
  double* r) {
  CURAND_CHECK(curandGenerateUniformDouble(curand_generator(), r, n));
  const double range = b - a;
  if (range != static_cast<double>(1)) {
    caffe_scal<double>(context, n, range, r);
  }
  if (a != static_cast<double>(0)) {
    caffe_add_scalar<double>(context, n, a, r);
  }
}

template <>
void caffe_rng_gaussian<float>(_CONTEXT, const int n, const float mu, const float sigma,
  float* r) {
  CURAND_CHECK(
    curandGenerateNormal(curand_generator(), r, n, mu, sigma));
}

template <>
void caffe_rng_gaussian<double>(_CONTEXT, const int n, const double mu, const double sigma,
  double* r) {
  CURAND_CHECK(
    curandGenerateNormalDouble(curand_generator(), r, n, mu, sigma));
}

/////////////////////////////////////////


template <typename Dtype>
__global__ void SGDUpdate(int N, Dtype* g, Dtype* h, Dtype momentum, Dtype local_rate) {
  CUDA_KERNEL_LOOP(i, N) {
    g[i] = h[i] = momentum*h[i] + local_rate*g[i];
  }
}

template <typename Dtype>
__global__ void AdaDeltaUpdate(int N, Dtype* g, Dtype* h, Dtype* h2,
  Dtype momentum, Dtype delta, Dtype local_rate) {
  CUDA_KERNEL_LOOP(i, N) {
    float gi = g[i];
    float hi = h[i] = momentum * h[i] + (1 - momentum) * gi * gi;
    gi = gi * sqrt((h2[i] + delta) / (hi + delta));
    h2[i] = momentum * h2[i] + (1 - momentum) * gi * gi;
    g[i] = local_rate * gi;
  }
}

template <typename Dtype>
__global__ void AdaGradUpdate(int N, Dtype* g, Dtype* h, Dtype delta,
  Dtype local_rate) {
  CUDA_KERNEL_LOOP(i, N) {
    float gi = g[i];
    float hi = h[i] = h[i] + gi*gi;
    g[i] = local_rate * gi / (sqrt(hi) + delta);
  }
}

template <typename Dtype>
__global__ void AdamUpdate(int N, Dtype* g, Dtype* m, Dtype* v,
  Dtype beta1, Dtype beta2, Dtype eps_hat, Dtype corrected_local_rate) {
  CUDA_KERNEL_LOOP(i, N) {
    float gi = g[i];
    float mi = m[i] = m[i] * beta1 + gi*(1 - beta1);
    float vi = v[i] = v[i] * beta2 + gi*gi*(1 - beta2);
    g[i] = corrected_local_rate * mi / (sqrt(vi) + eps_hat);
  }
}
/////////////////////////////////////////


template <typename Dtype>
__global__ void ReLUForward(const int n, const Dtype* in, Dtype* out,
  Dtype negative_slope) {
  CUDA_KERNEL_LOOP(index, n) {
    out[index] = in[index] > 0 ? in[index] : in[index] * negative_slope;
  }
}

template <typename Dtype>
__global__ void ReLUBackward(const int n, const Dtype* in_diff,
  const Dtype* in_data, Dtype* out_diff, Dtype negative_slope) {
  CUDA_KERNEL_LOOP(index, n) {
    out_diff[index] = in_diff[index] * ((in_data[index] > 0)
      + (in_data[index] <= 0) * negative_slope);
  }
}


#else

template <>
void caffe_copy(_CONTEXT, const int N, const Dtype* X, Dtype* Y)
{
  if (X != Y) {
    // NOLINT_NEXT_LINE(caffe/alt_fn)
    CUDA_CHECK(cudaMemcpy(Y, X, sizeof(Dtype) * N, cudaMemcpyDefault));
  }
}

template <>
void caffe_set<Dtype>(_CONTEXT, const int N, const Dtype alpha, Dtype* Y) {
  if (alpha == 0) {
    CUDA_CHECK(cudaMemset(Y, 0, sizeof(Dtype) * N));  // NOLINT(caffe/alt_fn)
    return;
  }
  // NOLINT_NEXT_LINE(whitespace/operators)
  set_kernel<Dtype> << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(N, alpha, Y);
}

template <>
void caffe_sub<Dtype>(_CONTEXT, const int N, const Dtype* a, const Dtype* b, Dtype* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  sub_kernel<Dtype> << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
    N, a, b, y);
}

template <>
void caffe_mul<Dtype>(_CONTEXT, const int N, const Dtype* a,
  const Dtype* b, Dtype* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  mul_kernel<Dtype> << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
    N, a, b, y);
}

template <>
void caffe_div<Dtype>(_CONTEXT, const int N, const Dtype* a,
  const Dtype* b, Dtype* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  div_kernel<Dtype> << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
    N, a, b, y);
}

template <>
void sgd_update<Dtype>(_CONTEXT, int N, Dtype* g, Dtype* h, Dtype momentum, Dtype local_rate) {
  SGDUpdate<Dtype>  // NOLINT_NEXT_LINE(whitespace/operators)
    << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
      N, g, h, momentum, local_rate);
  CUDA_POST_KERNEL_CHECK;
}

template <>
void adadelta_update<Dtype>(_CONTEXT, int N, Dtype* g, Dtype* h, Dtype* h2, Dtype momentum,
  Dtype delta, Dtype local_rate) {
  AdaDeltaUpdate<Dtype>  // NOLINT_NEXT_LINE(whitespace/operators)
    << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
      N, g, h, h2, momentum, delta, local_rate);
  CUDA_POST_KERNEL_CHECK;
}
template <>
void adagrad_update<Dtype>(_CONTEXT, int N, Dtype* g, Dtype* h, Dtype delta, Dtype local_rate) {
  AdaGradUpdate<Dtype>  // NOLINT_NEXT_LINE(whitespace/operators)
    << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
      N, g, h, delta, local_rate);
  CUDA_POST_KERNEL_CHECK;
}

template <>
void adam_update<Dtype>(_CONTEXT, int N, Dtype* g, Dtype* m, Dtype* v, Dtype beta1,
  Dtype beta2, Dtype eps_hat, Dtype corrected_local_rate) {
  AdamUpdate<Dtype>  // NOLINT_NEXT_LINE(whitespace/operators)
    << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
      N, g, m, v, beta1, beta2, eps_hat, corrected_local_rate);
  CUDA_POST_KERNEL_CHECK;
}

template <>
void relu_forward<Dtype>(_CONTEXT,const int n, const Dtype* in, Dtype* out,
  Dtype negative_slope) {
  ReLUForward<Dtype> << <CAFFE_GET_BLOCKS(n), CAFFE_CUDA_NUM_THREADS >> >( n, in, out, negative_slope);
}

template <>
void relu_backward<Dtype>(_CONTEXT, const int n, const Dtype* in_diff, const Dtype* in_data, Dtype* out_diff, Dtype negative_slope) {
  ReLUBackward<Dtype> << <CAFFE_GET_BLOCKS(n), CAFFE_CUDA_NUM_THREADS >> >( n, in_diff, in_data, out_diff, negative_slope);
}



#endif




#include "im2col.cuh"
#include "layers/layers.cuh"
