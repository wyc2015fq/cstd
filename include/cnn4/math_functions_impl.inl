
static void FUN(conv2d)(const Dtype* inData, float* outData, const Dtype* weights, const Dtype* biasData,
  DataShape inSize, DataShape outSize, int kernel_h, int kernel_w, int stride_h, int stride_w,
  int dilation_h, int dilation_w, int pad_h, int pad_w, int groups, bool cross_correlation) {
  // Groups
  int o_c = outSize.c;
  int o_g = outSize.c / groups;
  int k_g = inSize.c / groups;
  int o_head, k_head;
  int n, g, o, k, y, x, p, q;
  // Convolution
    for (n = 0; n < outSize.n; n++) {
      for (g = 0; g < groups; g++) {
        o_head = o_g * g;
        k_head = k_g * g;
        for (o = 0; o < o_g; o++) {
          for (k = 0; k < k_g; k++) {
            for (y = 0; y < outSize.h; y++) {
              for (x = 0; x < outSize.w; x++) {
                int out_offset = ((n*o_g + o + o_head)*outSize.h + y)*outSize.w + x;
                const Dtype* w = weights + ((o + o_head)*k_g + k)*kernel_h*kernel_w;
                for (p = 0; p < kernel_h; p++) {
                  for (q = 0; q < kernel_w; q++) {
                    int in_y = y * stride_h - pad_h + p * dilation_h;
                    int in_x = x * stride_w - pad_w + q * dilation_w;
                    if (is_a_ge_zero_and_a_lt_b(in_y, inSize.h) && is_a_ge_zero_and_a_lt_b(in_x, inSize.w)) {
                      int weight_offset = cross_correlation ? (((o + o_head)*k_g + k)*kernel_h + p)*kernel_w + q
                        : (((o + o_head)*k_g + k)*kernel_h + kernel_h -1-p)*kernel_w + kernel_w - 1 - q;
                      int in_offset = ((n*k_g + k + k_head)*inSize.h + in_y)*inSize.w + in_x;
                      outData[out_offset] += inData[in_offset] * weights[weight_offset];
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
  
  // Bias
  if (biasData) {
    for (n = 0; n < outSize.n; n++) {
      for (o = 0; o < o_c; o++) {
        for (y = 0; y < outSize.h; y++) {
          for (x = 0; x < outSize.w; x++) {
            int out_offset = ((n*o_c + o)*outSize.h + y)*outSize.w + x;
            outData[out_offset] += biasData[o];
          }
        }
      }
    }
  }
}


struct FUN(ones_t) {
  Buffer buf;
  FUN(ones_t)() {
    buf.data = 0; buf.size = 0; }
  ~FUN(ones_t)() {
    Free(&buf);
  }
  Dtype* get(int n) {
    int size = n * sizeof(Dtype);
    if (size > buf.size) {
      cpu_ReAlloc(&buf, size);
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

void FUN(caffe_copy)(const int N, const Dtype* X, Dtype* Y) {
  if (X != Y) {
    memcpy(Y, X, sizeof(Dtype) * N);
  }
}

inline int8_t FUN(caffe_sign_)(Dtype val)
{
  return (Dtype(0) < val) - (val < Dtype(0));
}

#define DEFINE_CAFFE_CPU_UNARY_FUNC(name, operation) \
  void FUN(name)(const int n, const Dtype* x, Dtype* y) { \
    CHECK_GT(n, 0); CHECK(x); CHECK(y); \
    for (int i = 0; i < n; ++i) { \
      y[i] = operation; \
    } \
  }

DEFINE_CAFFE_CPU_UNARY_FUNC(caffe_sign, y[i] = FUN(caffe_sign_)(x[i]));
DEFINE_CAFFE_CPU_UNARY_FUNC(caffe_sgnbit, y[i] = static_cast<bool>((std::signbit)(x[i])));
DEFINE_CAFFE_CPU_UNARY_FUNC(caffe_fabs, y[i] = std::fabs(x[i]));

#undef DEFINE_CAFFE_CPU_UNARY_FUNC

void FUN(caffe_gemm)(const CBLAS_TRANSPOSE TransA,
  const CBLAS_TRANSPOSE TransB, const int M, const int N, const int K,
  const Stype alpha, const Dtype* A, const Dtype* B, const Stype beta,
  Dtype* C)
{
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  CBLASFUN(gemm)(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B,
    ldb, beta, C, N);
}


void FUN(caffe_gemv)(const CBLAS_TRANSPOSE TransA, const int M,
  const int N, const Stype alpha, const Dtype* A, const Dtype* x,
  const Stype beta, Dtype* y)
{
  CBLASFUN(gemv)(CblasRowMajor, TransA, M, N, alpha, A, N, x, 1, beta, y, 1);
}

void FUN(caffe_axpy)(const int N, const Stype alpha, const Dtype* X, Dtype* Y) {
  CBLASFUN(axpy)(N, alpha, X, 1, Y, 1);
}
void FUN(caffe_set)(const int N, const Stype alpha, Dtype* Y)
{
  if (alpha == 0) {
    memset(Y, 0, sizeof(Dtype) * N);  // NOLINT(caffe/alt_fn)
    return;
  }
  for (int i = 0; i < N; ++i) {
    Y[i] = alpha;
  }
}
void FUN(caffe_add_scalar)(const int N, const Stype alpha, Dtype* Y)
{
  for (int i = 0; i < N; ++i) {
    Y[i] += alpha;
  }
}

void FUN(caffe_scal)(const int N, const Stype alpha, Dtype* X)
{
  CBLASFUN(scal)(N, alpha, X, 1);
}

Stype FUN(caffe_strided_dot)(const int n, const Dtype* x, const int incx, const Dtype* y, const int incy)
{
  return CBLASFUN(dot)(n, x, incx, y, incy);
}

Stype FUN(caffe_dot)(const int n, const Dtype* x, const Dtype* y)
{
  return CBLASFUN(dot)(n, x, 1, y, 1);
}

Stype FUN(caffe_asum)(const int n, const Dtype* x)
{
  return CBLASFUN(asum)(n, x, 1);
}


// Functions that caffe uses but are not present if MKL is not linked.
// A simple way to define the vsl unary functions. The operation should
// be in the form e.g. y[i] = sqrt(a[i])
#define DEFINE_VSL_UNARY_FUNC(name, operation) \
  void FUN(name)(const int n, const Dtype* a, Dtype* y) { \
    CHECK_GT(n, 0); CHECK(a); CHECK(y); \
    for (int i = 0; i < n; ++i) { operation; } \
  }

DEFINE_VSL_UNARY_FUNC(caffe_sqr, y[i] = a[i] * a[i]);
DEFINE_VSL_UNARY_FUNC(caffe_exp, y[i] = exp(a[i]));
DEFINE_VSL_UNARY_FUNC(caffe_log, y[i] = log(a[i]));
DEFINE_VSL_UNARY_FUNC(caffe_abs, y[i] = fabs(a[i]));
#undef DEFINE_VSL_UNARY_FUNC

// A simple way to define the vsl unary functions with singular parameter b.
// The operation should be in the form e.g. y[i] = pow(a[i], b)
void FUN(caffe_powx)(const int n, const Dtype* a, const Stype b, Dtype* y) {
  CHECK_GT(n, 0); CHECK(a); CHECK(y);
  for (int i = 0; i < n; ++i) { y[i] = pow(a[i], b); }
}

// A simple way to define the vsl binary functions. The operation should
// be in the form e.g. y[i] = a[i] + b[i]
#define DEFINE_VSL_BINARY_FUNC(name, operation) \
  void FUN(name)(const int n, const Dtype* a, const Dtype* b, Dtype* y) { \
    CHECK_GT(n, 0); CHECK(a); CHECK(b); CHECK(y); \
    for (int i = 0; i < n; ++i) { operation; } \
  }

DEFINE_VSL_BINARY_FUNC(caffe_add, y[i] = a[i] + b[i]);
DEFINE_VSL_BINARY_FUNC(caffe_sub, y[i] = a[i] - b[i]);
DEFINE_VSL_BINARY_FUNC(caffe_mul, y[i] = a[i] * b[i]);
DEFINE_VSL_BINARY_FUNC(caffe_div, y[i] = a[i] / b[i]);

// In addition, MKL comes with an additional function axpby that is not present
// in standard blas. We will simply use a two-step (inefficient, of course) way
// to mimic that.
void CBLASFUN(axpby)(const int N, const Stype alpha, const Dtype* X,
  const int incX, const Stype beta, Dtype* Y,
  const int incY)
{
  CBLASFUN(scal)(N, beta, Y, incY);
  CBLASFUN(axpy)(N, alpha, X, incX, Y, incY);
}
// Y=alpha * X +beta*Y
void FUN(caffe_axpby)(const int N, const Stype alpha, const Dtype* X, const Stype beta, Dtype* Y)
{
  CBLASFUN(axpby)(N, alpha, X, 1, beta, Y, 1);
}

void FUN(caffe_bound)(const int N, const Dtype* a, const Stype min,
  const Stype max, Dtype* y)
{
  for (int i = 0; i < N; ++i) {
    y[i] = BOUND(a[i], min, max);
  }
}

Dtype FUN(caffe_nextafter)(const Stype b)
{
  return std::nextafter<Dtype>(b, std::numeric_limits<Dtype>::max());
}



void FUN(caffe_rng_uniform)(const int n, const Stype a, const Stype b, Dtype* r)
{
  CHECK_GE(n, 0);
  CHECK(r);
  CHECK_LE(a, b);
  std::uniform_real<Dtype> random_distribution(a, FUN(caffe_nextafter)(b));
  //std::variate_generator<caffe::rng_t*, std::uniform_real<Dtype> >
  //variate_generator(caffe_rng(), random_distribution);
  //caffe_rng();
  rng_t* rng = caffe_rng();
  for (int i = 0; i < n; ++i) {
    r[i] = random_distribution(*rng);
  }
}

void FUN(caffe_rng_gaussian)(const int n, const Stype a, const Stype sigma, Dtype* r)
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

void FUN(caffe_rng_bernoulli)(const int n, const Stype p, int* r)
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

void FUN(caffe_asum2)(const int n, const Dtype* x, Dtype* y)
{
  *y = CBLASFUN(asum)(n, x, 1);
}

// y[i] = alpha * x[i]
void FUN(caffe_scale)(const int n, const Stype alpha, const Dtype* x, Dtype* y)
{
  CBLASFUN(copy)(n, x, 1, y, 1);
  CBLASFUN(scal)(n, alpha, y, 1);
}

////////////////////////////////////////

void FUN(sgd_update)(int N, Dtype* g, Dtype* h, Stype momentum, Stype local_rate) {
  CPU_KERNEL_LOOP(i, N) {
    g[i] = h[i] = momentum*h[i] + local_rate*g[i];
  }
}


void FUN(adadelta_update)(int N, Dtype* g, Dtype* h, Dtype* h2, Stype momentum,
  Stype delta, Stype local_rate) {
  CPU_KERNEL_LOOP(i, N) {
    Stype gi = g[i];
    Stype hi = h[i] = momentum * h[i] + (1 - momentum) * gi * gi;
    gi = gi * sqrt((h2[i] + delta) / (hi + delta));
    h2[i] = momentum * h2[i] + (1 - momentum) * gi * gi;
    g[i] = local_rate * gi;
  }
}


void FUN(adagrad_update)(int N, Dtype* g, Dtype* h, Stype delta, Stype local_rate) {
  CPU_KERNEL_LOOP(i, N) {
    Stype gi = g[i];
    Stype hi = h[i] = h[i] + gi*gi;
    g[i] = local_rate * gi / (sqrt(hi) + delta);
  }
}


void FUN(adam_update)(int N, Dtype* g, Dtype* m, Dtype* v, Stype beta1,
  Stype beta2, Stype eps_hat, Stype corrected_local_rate) {
  CPU_KERNEL_LOOP(i, N) {
    Stype gi = g[i];
    Stype mi = m[i] = m[i] * beta1 + gi*(1 - beta1);
    Stype vi = v[i] = v[i] * beta2 + gi*gi*(1 - beta2);
    g[i] = corrected_local_rate * mi / (sqrt(vi) + eps_hat);
  }
}

//////////////////////////////////////


//////////////////////////////////////


void FUN(relu_forward)(const int n, const Dtype* in, Dtype* out, Stype negative_slope) {
  CPU_KERNEL_LOOP(index, n) {
    out[index] = in[index] > 0 ? in[index] : in[index] * negative_slope;
  }
}


void FUN(relu_backward)(const int n, const Dtype* in_diff, const Dtype* in_data, Dtype* out_diff, Stype negative_slope) {
  CPU_KERNEL_LOOP(index, n) {
    out_diff[index] = in_diff[index] * ((in_data[index] > 0)
      + (in_data[index] <= 0) * negative_slope);
  }
}

#include "filler.inl"
#include "im2col.inl"
#include "layers/layers.inl"
