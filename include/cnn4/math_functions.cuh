
__global__ void FUN(set_kernel)(const int n, const Dtype alpha, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = alpha;
  }
}

void FUN(caffe_set)(const int N, const Stype alpha, Dtype* Y) {
  if (alpha == 0) {
    CUDA_CHECK(cudaMemset(Y, 0, sizeof(Dtype) * N));  // NOLINT(caffe/alt_fn)
    return;
  }
  // NOLINT_NEXT_LINE(whitespace/operators)
  FUN(set_kernel) << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(N, alpha, Y);
}

struct FUN(ones_t) {
  Buffer buf;
  FUN(ones_t)() {
    buf.data = 0; buf.size = 0;
  }
  ~FUN(ones_t)() {
    Free(&buf);
  }
  Dtype* get(int n) {
    int size = n * sizeof(Dtype);
    if (size > buf.size) {
      FUN(ReAlloc)(&buf, size);
      FUN(caffe_set)(n, 1, (Dtype*)buf.data);
    }
    return (Dtype*)buf.data;
  }
};

const Dtype* FUN(get_ones)(int n) {
  static FUN(ones_t) ones_;
  return ones_.get(n);
};

const Dtype* FUN(get_one)() {
  static Dtype oneval = 1.0;
  return &oneval;
}
const Dtype* FUN(get_zero)() {
  static Dtype zeroval = 0.0;
  return &zeroval;
}

void* FUN(caffe_malloc)(const int N) {
  void* p = NULL;
  CUDA_CHECK(cudaMalloc(&p, N));
  return p;
}
void FUN(caffe_free)(void* p) {
  if (p) {
    CUDA_CHECK(cudaFree(p));
  }
}

void FUN(caffe_copy)(const int N, const Dtype* X, Dtype* Y)
{
  if (X != Y) {
    // NOLINT_NEXT_LINE(caffe/alt_fn)
    CUDA_CHECK(cudaMemcpy(Y, X, sizeof(Dtype) * N, cudaMemcpyDefault));
  }
}

void FUN(caffe_gemm)(const CBLAS_TRANSPOSE TransA,
  const CBLAS_TRANSPOSE TransB, const int M, const int N, const int K,
  const Stype alpha, const Dtype* A, const Dtype* B, const Stype beta,
  Dtype* C) {
  // Note that cublas follows fortran order.
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  cublasOperation_t cuTransA =
    (TransA == CblasNoTrans) ? CUBLAS_OP_N : CUBLAS_OP_T;
  cublasOperation_t cuTransB =
    (TransB == CblasNoTrans) ? CUBLAS_OP_N : CUBLAS_OP_T;
  Dtype alpha_ = (Dtype)alpha;
  Dtype beta_ = (Dtype)beta;
  CUBLAS_CHECK(CBLASFUN(gemm)(cublas_handle(), cuTransB, cuTransA,
    N, M, K, &alpha_, B, ldb, A, lda, &beta_, C, N));
}

void FUN(caffe_gemv)(const CBLAS_TRANSPOSE TransA, const int M,
  const int N, const Stype alpha, const Dtype* A, const Dtype* x,
  const Stype beta, Dtype* y) {
  cublasOperation_t cuTransA =
    (TransA == CblasNoTrans) ? CUBLAS_OP_T : CUBLAS_OP_N;
  Dtype alpha_ = (Dtype)alpha;
  Dtype beta_ = (Dtype)beta;
  CUBLAS_CHECK(CBLASFUN(gemv)(cublas_handle(), cuTransA, N, M, &alpha_,
    A, N, x, 1, &beta_, y, 1));
}

void FUN(caffe_axpy)(const int N, const Stype alpha, const Dtype* X, Dtype* Y) {
  Dtype alpha_ = (Dtype)alpha;
  CUBLAS_CHECK(CBLASFUN(axpy)(cublas_handle(), N, &alpha_, X, 1, Y, 1));
}

void FUN(caffe_scal)(const int N, const Stype alpha, Dtype* X) {
  Dtype alpha_ = (Dtype)alpha;
  CUBLAS_CHECK(CBLASFUN(scal)(cublas_handle(), N, &alpha_, X, 1));
}

void FUN(caffe_axpby)(const int N, const Stype alpha, const Dtype* X,
  const Stype beta, Dtype* Y) {
  FUN(caffe_scal)(N, beta, Y);
  FUN(caffe_axpy)(N, alpha, X, Y);
}

__global__ void FUN(bound_kernel)(const int n, const Dtype* a, const Stype min, const Stype max, Dtype* y) {
  CUDA_KERNEL_LOOP(i, n) {
    y[i] = BOUND(a[i], min, max);
  }
}

void FUN(caffe_bound)(const int N, const Dtype* a, const Stype min, const Stype max, Dtype* y)
{
  FUN(bound_kernel) << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(N, a, min, max, y);
}

Stype FUN(caffe_dot)(const int n, const Dtype* x, const Dtype* y) {
  Dtype out;
  CUBLAS_CHECK(CBLASFUN(dot)(cublas_handle(), n, x, 1, y, 1, &out));
  return out;
}

// cublasSdot
Stype FUN(caffe_strided_dot)(const int n, const Dtype* x, const int incx, const Dtype* y, const int incy) {
  Dtype out;
  CUBLAS_CHECK(CBLASFUN(dot)(cublas_handle(), n, x, incx, y, incy, &out));
  return out;
}

Stype FUN(caffe_asum)(const int n, const Dtype* x) {
  Dtype out;
  CUBLAS_CHECK(CBLASFUN(asum)(cublas_handle(), n, x, 1, &out));
  return out;
}

void FUN(caffe_asum2)(const int n, const Dtype* x, Dtype* y) {
  CUBLAS_CHECK(CBLASFUN(asum)(cublas_handle(), n, x, 1, y));
}

void FUN(caffe_scale)(const int n, const Stype alpha, const Dtype* x,
  Dtype* y) {
  Dtype alpha_ = (Dtype)alpha;
  CUBLAS_CHECK(CBLASFUN(copy)(cublas_handle(), n, x, 1, y, 1));
  CUBLAS_CHECK(CBLASFUN(scal)(cublas_handle(), n, &alpha_, y, 1));
}

__global__ void FUN(add_scalar_kernel)(const int n, const Dtype alpha, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] += alpha;
  }
}

void FUN(caffe_add_scalar)(const int N, const Stype alpha, Dtype* Y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  FUN(add_scalar_kernel) << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(N, alpha, Y);
}

#define DEFINE_VSL_UNARY_FUNC(name, operation) \
  __global__ void FUN(name##_kernel)(const int n, const Dtype* a, Dtype* y) { \
    CUDA_KERNEL_LOOP(i, n) { operation; } \
  } \
void FUN(name)(const int n, const Dtype* a, Dtype* y) { \
     FUN(name##_kernel) << <CAFFE_GET_BLOCKS(n), CAFFE_CUDA_NUM_THREADS >> >(n, a, y);\
}

DEFINE_VSL_UNARY_FUNC(caffe_sqr, y[i] = a[i] * a[i]);
//DEFINE_VSL_UNARY_FUNC(caffe_exp, y[i] = exp(a[i]));
//DEFINE_VSL_UNARY_FUNC(caffe_log, y[i] = log(a[i]));
//DEFINE_VSL_UNARY_FUNC(caffe_abs, y[i] = fabs(a[i]));
#undef DEFINE_VSL_UNARY_FUNC


__global__ void FUN(add_kernel)(const int n, const Dtype* a,
  const Dtype* b, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = a[index] + b[index];
  }
}


void FUN(caffe_add)(const int N, const Dtype* a, const Dtype* b,
  Dtype* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  FUN(add_kernel) << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
    N, a, b, y);
}

__global__ void FUN(sub_kernel)(const int n, const Dtype* a,
  const Dtype* b, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = a[index] - b[index];
  }
}

void FUN(caffe_sub)(const int N, const Dtype* a, const Dtype* b, Dtype* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
 FUN(sub_kernel) << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> > (
    N, a, b, y);
}

__global__ void FUN(mul_kernel)(const int n, const Dtype* a,
  const Dtype* b, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = a[index] * b[index];
  }
}


void FUN(caffe_mul)(const int N, const Dtype* a,
  const Dtype* b, Dtype* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
 FUN(mul_kernel) << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> > (
    N, a, b, y);
}


__global__ void FUN(div_kernel)(const int n, const Dtype* a,
  const Dtype* b, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = a[index] / b[index];
  }
}

void FUN(caffe_div)(const int N, const Dtype* a, const Dtype* b, Dtype* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
 FUN(div_kernel) << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
    N, a, b, y);
}

__global__ void FUN(abs_kernel)(const int n, const Dtype* a, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = abs(a[index]);
  }
}


void FUN(caffe_abs)(const int N, const Dtype* a, Dtype* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  FUN(abs_kernel) << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
    N, a, y);
}


__global__ void FUN(exp_kernel)(const int n, const Dtype* a, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = exp(a[index]);
  }
}


void FUN(caffe_exp)(const int N, const Dtype* a, Dtype* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  FUN(exp_kernel) << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
    N, a, y);
}


__global__ void FUN(log_kernel)(const int n, const Dtype* a, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = log(a[index]);
  }
}


void FUN(caffe_log)(const int N, const Dtype* a, Dtype* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  FUN(log_kernel) << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
    N, a, y);
}


__global__ void FUN(powx_kernel)(const int n, const Dtype* a,
  const Dtype alpha, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = pow(a[index], alpha);
  }
}


void FUN(caffe_powx)(const int N, const Dtype* a,
  const Stype alpha, Dtype* y) {
  // NOLINT_NEXT_LINE(whitespace/operators)
  FUN(powx_kernel) << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
    N, a, alpha, y);
}

__global__ void FUN(sign_kernel)(const int n, const Dtype* x, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = (Dtype(0) < x[index]) - (x[index] < Dtype(0));
  }
}

void FUN(caffe_sign)(const int n, const Dtype* x, Dtype* y) {
  FUN(sign_kernel)<<<CAFFE_GET_BLOCKS(n), CAFFE_CUDA_NUM_THREADS>>>(n, x, y);
}

__global__ void FUN(sgnbit_kernel)(const int n, const Dtype* x, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = signbit(x[index]);
  }
}

void FUN(caffe_sgnbit)(const int n, const Dtype* x, Dtype* y) {
  FUN(sgnbit_kernel) << <CAFFE_GET_BLOCKS(n), CAFFE_CUDA_NUM_THREADS >> >(
    n, x, y);
}

__global__ void FUN(fabs_kernel)(const int n, const Dtype* x, Dtype* y) {
  CUDA_KERNEL_LOOP(index, n) {
    y[index] = std::fabs(x[index]);
  }
}

void FUN(caffe_fabs)(const int n, const Dtype* x, Dtype* y) {
  FUN(fabs_kernel) << <CAFFE_GET_BLOCKS(n), CAFFE_CUDA_NUM_THREADS >> >(
    n, x, y);
}

void FUN(caffe_rng_uniform_i)(const int n, unsigned int* r) {
  CURAND_CHECK(curandGenerate(curand_generator(), r, n));
}


void FUN(caffe_rng_uniform)(const int n, const Stype a, const Stype b, Dtype* r) {
  CURAND_CHECK(curandGenerateUniform(curand_generator(), r, n));
  const Dtype range = b - a;
  if (range != static_cast<Dtype>(1)) {
    FUN(caffe_scal)(n, range, r);
  }
  if (a != static_cast<Stype>(0)) {
    FUN(caffe_add_scalar)(n, a, r);
  }
}


void FUN(caffe_rng_gaussian)(const int n, const Stype mu, const Stype sigma,
  Dtype* r) {
  CURAND_CHECK(
    curandGenerateNormal(curand_generator(), r, n, mu, sigma));
}

/////////////////////////////////////////

__global__ void FUN(SGDUpdate)(int N, Dtype* g, Dtype* h, Dtype momentum, Dtype local_rate) {
  CUDA_KERNEL_LOOP(i, N) {
    g[i] = h[i] = momentum*h[i] + local_rate*g[i];
  }
}

void FUN(sgd_update)(int N, Dtype* g, Dtype* h, Stype momentum, Stype local_rate) {
  FUN(SGDUpdate) << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(N, g, h, momentum, local_rate);
  CUDA_POST_KERNEL_CHECK;
}

__global__ void FUN(AdaDeltaUpdate)(int N, Dtype* g, Dtype* h, Dtype* h2,
  Dtype momentum, Dtype delta, Dtype local_rate) {
  CUDA_KERNEL_LOOP(i, N) {
    Stype gi = g[i];
    Stype hi = h[i] = momentum * h[i] + (1 - momentum) * gi * gi;
    gi = gi * sqrt((h2[i] + delta) / (hi + delta));
    h2[i] = momentum * h2[i] + (1 - momentum) * gi * gi;
    g[i] = local_rate * gi;
  }
}

void FUN(adadelta_update)(int N, Dtype* g, Dtype* h, Dtype* h2, Stype momentum, Stype delta, Stype local_rate) {
  FUN(AdaDeltaUpdate) << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >( N, g, h, h2, momentum, delta, local_rate);
  CUDA_POST_KERNEL_CHECK;
}

__global__ void FUN(AdaGradUpdate)(int N, Dtype* g, Dtype* h, Dtype delta, Dtype local_rate) {
  CUDA_KERNEL_LOOP(i, N) {
    Stype gi = g[i];
    Stype hi = h[i] = h[i] + gi*gi;
    g[i] = local_rate * gi / (sqrt(hi) + delta);
  }
}

void FUN(adagrad_update)(int N, Dtype* g, Dtype* h, Stype delta, Stype local_rate) {
  FUN(AdaGradUpdate) << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >( N, g, h, delta, local_rate);
  CUDA_POST_KERNEL_CHECK;
}

__global__ void FUN(AdamUpdate)(int N, Dtype* g, Dtype* m, Dtype* v,
  Dtype beta1, Dtype beta2, Dtype eps_hat, Dtype corrected_local_rate) {
  CUDA_KERNEL_LOOP(i, N) {
    Stype gi = g[i];
    Stype mi = m[i] = m[i] * beta1 + gi*(1 - beta1);
    Stype vi = v[i] = v[i] * beta2 + gi*gi*(1 - beta2);
    g[i] = corrected_local_rate * mi / (sqrt(vi) + eps_hat);
  }
}
void FUN(adam_update)(int N, Dtype* g, Dtype* m, Dtype* v, Stype beta1,
  Stype beta2, Stype eps_hat, Stype corrected_local_rate) {
  FUN(AdamUpdate) << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
      N, g, m, v, beta1, beta2, eps_hat, corrected_local_rate);
  CUDA_POST_KERNEL_CHECK;
}

/////////////////////////////////////////


#include "layers/kernel.cuh"

