#include <random>
#include <limits>
#include "cpu.hpp"

#define _CONTEXT CPUContext* context

void caffe_memset(_CONTEXT,const size_t N, const int alpha, void* X)
{
  memset(X, alpha, N);
}
void caffe_memcpy(_CONTEXT, const size_t N, const void* X, void* Y) {
  memcpy(Y, X, N);
}

template <typename Dtype>
void caffe_copy(_CONTEXT, const int N, const Dtype* X, Dtype* Y)
{
  if (X != Y) {
    memcpy(Y, X, sizeof(Dtype) * N);  // NOLINT(caffe/alt_fn)
  }
}

template<typename Dtype>
inline int8_t caffe_sign(Dtype val)
{
  return (Dtype(0) < val) - (val < Dtype(0));
}

#define DEFINE_CAFFE_CPU_UNARY_FUNC(name, operation) \
  template<typename Dtype> \
  void caffe_##name(_CONTEXT, const int n, const Dtype* x, Dtype* y) { \
    CHECK_GT(n, 0); CHECK(x); CHECK(y); \
    for (int i = 0; i < n; ++i) { \
      operation; \
    } \
  }

DEFINE_CAFFE_CPU_UNARY_FUNC(sign, y[i] = caffe_sign<Dtype>(x[i]));
DEFINE_CAFFE_CPU_UNARY_FUNC(sgnbit, y[i] = static_cast<bool>((std::signbit)(x[i])));
DEFINE_CAFFE_CPU_UNARY_FUNC(fabs, y[i] = std::fabs(x[i]));

template<>
void caffe_gemm<float>(_CONTEXT,const CBLAS_TRANSPOSE TransA,
  const CBLAS_TRANSPOSE TransB, const int M, const int N, const int K,
  const float alpha, const float* A, const float* B, const float beta,
  float* C)
{
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  cblas_sgemm(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B,
    ldb, beta, C, N);
}

template<>
void caffe_gemm<double>(_CONTEXT,const CBLAS_TRANSPOSE TransA,
  const CBLAS_TRANSPOSE TransB, const int M, const int N, const int K,
  const double alpha, const double* A, const double* B, const double beta,
  double* C)
{
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  cblas_dgemm(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B,
    ldb, beta, C, N);
}

template <>
void caffe_gemv<float>(_CONTEXT,const CBLAS_TRANSPOSE TransA, const int M,
  const int N, const float alpha, const float* A, const float* x,
  const float beta, float* y)
{
  cblas_sgemv(CblasRowMajor, TransA, M, N, alpha, A, N, x, 1, beta, y, 1);
}

template <>
void caffe_gemv<double>(_CONTEXT,const CBLAS_TRANSPOSE TransA, const int M,
  const int N, const double alpha, const double* A, const double* x,
  const double beta, double* y)
{
  cblas_dgemv(CblasRowMajor, TransA, M, N, alpha, A, N, x, 1, beta, y, 1);
}

template <>
void caffe_axpy<float>(_CONTEXT,const int N, const float alpha, const float* X, float* Y) {
  cblas_saxpy(N, alpha, X, 1, Y, 1);
}

template <>
void caffe_axpy<double>(_CONTEXT,const int N, const double alpha, const double* X,
  double* Y) {
  cblas_daxpy(N, alpha, X, 1, Y, 1);
}

template <typename Dtype>
void caffe_set(_CONTEXT, const int N, const Dtype alpha, Dtype* Y)
{
  if (alpha == 0) {
    memset(Y, 0, sizeof(Dtype) * N);  // NOLINT(caffe/alt_fn)
    return;
  }
  for (int i = 0; i < N; ++i) {
    Y[i] = alpha;
  }
}

template <>
void caffe_add_scalar<float>(_CONTEXT,const int N, const float alpha, float* Y)
{
  for (int i = 0; i < N; ++i) {
    Y[i] += alpha;
  }
}

template <>
void caffe_add_scalar<double>(_CONTEXT,const int N, const double alpha, double* Y)
{
  for (int i = 0; i < N; ++i) {
    Y[i] += alpha;
  }
}

template <>
void caffe_scal<float>(_CONTEXT,const int N, const float alpha, float* X)
{
  cblas_sscal(N, alpha, X, 1);
}

template <>
void caffe_scal<double>(_CONTEXT,const int N, const double alpha, double* X)
{
  cblas_dscal(N, alpha, X, 1);
}

template <>
void caffe_axpby<float>(_CONTEXT,const int N, const float alpha, const float* X,
  const float beta, float* Y)
{
  cblas_saxpby(N, alpha, X, 1, beta, Y, 1);
}

template <>
void caffe_axpby<double>(_CONTEXT,const int N, const double alpha, const double* X,
  const double beta, double* Y)
{
  cblas_daxpby(N, alpha, X, 1, beta, Y, 1);
}

template <>
void caffe_add<float>(_CONTEXT,const int n, const float* a, const float* b,
  float* y)
{
  vsAdd(n, a, b, y);
}

template <>
void caffe_add<double>(_CONTEXT,const int n, const double* a, const double* b,
  double* y)
{
  vdAdd(n, a, b, y);
}

template <>
void caffe_sub<float>(_CONTEXT,const int n, const float* a, const float* b,
  float* y)
{
  vsSub(n, a, b, y);
}

template <>
void caffe_sub<double>(_CONTEXT,const int n, const double* a, const double* b,
  double* y)
{
  vdSub(n, a, b, y);
}

template <>
void caffe_mul<float>(_CONTEXT,const int n, const float* a, const float* b,
  float* y)
{
  vsMul(n, a, b, y);
}

template <>
void caffe_mul<double>(_CONTEXT,const int n, const double* a, const double* b,
  double* y)
{
  vdMul(n, a, b, y);
}

template <>
void caffe_div<float>(_CONTEXT,const int n, const float* a, const float* b,
  float* y)
{
  vsDiv(n, a, b, y);
}

template <>
void caffe_div<double>(_CONTEXT,const int n, const double* a, const double* b,
  double* y)
{
  vdDiv(n, a, b, y);
}

template <>
void caffe_powx<float>(_CONTEXT,const int n, const float* a, const float b,
  float* y)
{
  vsPowx(n, a, b, y);
}

template <>
void caffe_powx<double>(_CONTEXT,const int n, const double* a, const double b,
  double* y)
{
  vdPowx(n, a, b, y);
}

template <>
void caffe_sqr<float>(_CONTEXT,const int n, const float* a, float* y)
{
  vsSqr(n, a, y);
}

template <>
void caffe_sqr<double>(_CONTEXT,const int n, const double* a, double* y)
{
  vdSqr(n, a, y);
}

template <>
void caffe_exp<float>(_CONTEXT,const int n, const float* a, float* y)
{
  vsExp(n, a, y);
}

template <>
void caffe_exp<double>(_CONTEXT,const int n, const double* a, double* y)
{
  vdExp(n, a, y);
}

template <>
void caffe_log<float>(_CONTEXT,const int n, const float* a, float* y)
{
  vsLn(n, a, y);
}

template <>
void caffe_log<double>(_CONTEXT,const int n, const double* a, double* y)
{
  vdLn(n, a, y);
}

template <>
void caffe_abs<float>(_CONTEXT,const int n, const float* a, float* y)
{
  vsAbs(n, a, y);
}

template <>
void caffe_abs<double>(_CONTEXT,const int n, const double* a, double* y)
{
  vdAbs(n, a, y);
}

template <>
void caffe_bound<float>(_CONTEXT,const int N, const float* a, const float min,
  const float max, float* y)
{
  for (int i = 0; i < N; ++i) {
    y[i] = std::min(std::max(a[i], min), max);
  }
}

template <>
void caffe_bound<double>(_CONTEXT,const int N, const double* a, const double min,
  const double max, double* y)
{
  for (int i = 0; i < N; ++i) {
    y[i] = std::min(std::max(a[i], min), max);
  }
}

unsigned int caffe_rng_rand()
{
  return (*caffe_rng())();
}

template <typename Dtype>
Dtype caffe_nextafter(const Dtype b)
{
  return std::nextafter<Dtype>(
    b, std::numeric_limits<Dtype>::max());
}

template
float caffe_nextafter(const float b);

template
double caffe_nextafter(const double b);

template <typename Dtype>
void caffe_rng_uniform(const int n, const Dtype a, const Dtype b, Dtype* r)
{
  CHECK_GE(n, 0);
  CHECK(r);
  CHECK_LE(a, b);
  std::uniform_real<Dtype> random_distribution(a, caffe_nextafter<Dtype>(b));
  //std::variate_generator<caffe::rng_t*, std::uniform_real<Dtype> >
  //variate_generator(caffe_rng(), random_distribution);
  caffe_rng();
  rng_t* rng = caffe_rng();
  for (int i = 0; i < n; ++i) {
    r[i] = random_distribution(*rng);
  }
}

template
void caffe_rng_uniform<float>(_CONTEXT,const int n, const float a, const float b,
  float* r);

template
void caffe_rng_uniform<double>(_CONTEXT,const int n, const double a, const double b,
  double* r);

template <typename Dtype>
void caffe_rng_gaussian(const int n, const Dtype a,
  const Dtype sigma, Dtype* r)
{
  CHECK_GE(n, 0);
  CHECK(r);
  CHECK_GT(sigma, 0);
  std::normal_distribution<Dtype> random_distribution(a, sigma);
  //std::variate_generator<caffe::rng_t*, boost::normal_distribution<Dtype> >
  //variate_generator(caffe_rng(), random_distribution);
  rng_t* rng = caffe_rng();
  for (int i = 0; i < n; ++i) {
    r[i] = random_distribution(*rng);
  }
}

template
void caffe_rng_gaussian<float>(_CONTEXT,const int n, const float mu,
  const float sigma, float* r);

template
void caffe_rng_gaussian<double>(_CONTEXT,const int n, const double mu,
  const double sigma, double* r);

template <typename Dtype>
void caffe_rng_bernoulli(const int n, const Dtype p, int* r)
{
  CHECK_GE(n, 0);
  CHECK(r);
  CHECK_GE(p, 0);
  CHECK_LE(p, 1);
  std::bernoulli_distribution random_distribution(p);
  rng_t* rng = caffe_rng();
  for (int i = 0; i < n; ++i) {
    r[i] = random_distribution(*rng);
  }
}

template <typename Dtype>
void caffe_rng_bernoulli(const int n, const Dtype p, unsigned int* r)
{
  CHECK_GE(n, 0);
  CHECK(r);
  CHECK_GE(p, 0);
  CHECK_LE(p, 1);
  std::bernoulli_distribution random_distribution(p);
  rng_t* rng = caffe_rng();
  for (int i = 0; i < n; ++i) {
    r[i] = static_cast<unsigned int>(random_distribution(*rng));
  }
}

template <>
float caffe_strided_dot<float>(_CONTEXT,const int n, const float* x, const int incx,
  const float* y, const int incy)
{
  return cblas_sdot(n, x, incx, y, incy);
}

template <>
double caffe_strided_dot<double>(_CONTEXT,const int n, const double* x,
  const int incx, const double* y, const int incy)
{
  return cblas_ddot(n, x, incx, y, incy);
}

template <typename Dtype>
Dtype caffe_dot(_CONTEXT, const int n, const Dtype* x, const Dtype* y)
{
  return caffe_strided_dot(context, n, x, 1, y, 1);
}

template <>
float caffe_asum<float>(_CONTEXT, const int n, const float* x)
{
  return cblas_sasum(n, x, 1);
}

template <>
double caffe_asum<double>(_CONTEXT, const int n, const double* x)
{
  return cblas_dasum(n, x, 1);
}

template <>
void caffe_asum<float>(_CONTEXT, const int n, const float* x, float* y)
{
  *y = cblas_sasum(n, x, 1);
}

template <>
void caffe_asum<double>(_CONTEXT, const int n, const double* x, double* y)
{
  *y = cblas_dasum(n, x, 1);
}

template <>
void caffe_scale<float>(_CONTEXT,const int n, const float alpha, const float* x,
  float* y)
{
  cblas_scopy(n, x, 1, y, 1);
  cblas_sscal(n, alpha, y, 1);
}

template <>
void caffe_scale<double>(_CONTEXT,const int n, const double alpha, const double* x,
  double* y)
{
  cblas_dcopy(n, x, 1, y, 1);
  cblas_dscal(n, alpha, y, 1);
}

////////////////////////////////////////

template <typename Dtype>
void sgd_update(_CONTEXT, int N, Dtype* g, Dtype* h, Dtype momentum,
  Dtype local_rate) {
  CPU_KERNEL_LOOP(i, N) {
    g[i] = h[i] = momentum*h[i] + local_rate*g[i];
  }
}

template <typename Dtype>
void adadelta_update(_CONTEXT, int N, Dtype* g, Dtype* h, Dtype* h2, Dtype momentum,
  Dtype delta, Dtype local_rate) {
  CPU_KERNEL_LOOP(i, N) {
    float gi = g[i];
    float hi = h[i] = momentum * h[i] + (1 - momentum) * gi * gi;
    gi = gi * sqrt((h2[i] + delta) / (hi + delta));
    h2[i] = momentum * h2[i] + (1 - momentum) * gi * gi;
    g[i] = local_rate * gi;
  }
}

template <typename Dtype>
void adagrad_update(_CONTEXT, int N, Dtype* g, Dtype* h, Dtype delta, Dtype local_rate) {
  CPU_KERNEL_LOOP(i, N) {
    float gi = g[i];
    float hi = h[i] = h[i] + gi*gi;
    g[i] = local_rate * gi / (sqrt(hi) + delta);
  }
}

template <typename Dtype>
void adam_update(_CONTEXT, int N, Dtype* g, Dtype* m, Dtype* v, Dtype beta1,
  Dtype beta2, Dtype eps_hat, Dtype corrected_local_rate) {
  CPU_KERNEL_LOOP(i, N) {
    float gi = g[i];
    float mi = m[i] = m[i] * beta1 + gi*(1 - beta1);
    float vi = v[i] = v[i] * beta2 + gi*gi*(1 - beta2);
    g[i] = corrected_local_rate * mi / (sqrt(vi) + eps_hat);
  }
}

//////////////////////////////////////


//////////////////////////////////////

template <typename Dtype>
void relu_forward(_CONTEXT, const int n, const Dtype* in, Dtype* out, Dtype negative_slope) {
  CPU_KERNEL_LOOP(index, n) {
    out[index] = in[index] > 0 ? in[index] : in[index] * negative_slope;
  }
}

template <typename Dtype>
void relu_backward(_CONTEXT, const int n, const Dtype* in_diff, const Dtype* in_data, Dtype* out_diff, Dtype negative_slope) {
  CPU_KERNEL_LOOP(index, n) {
    out_diff[index] = in_diff[index] * ((in_data[index] > 0)
      + (in_data[index] <= 0) * negative_slope);
  }
}

#include "im2col.inl"
#include "layers/layers.inl"

#undef _CONTEXT
