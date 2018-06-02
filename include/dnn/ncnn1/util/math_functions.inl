
#ifndef _MATH_FUNCTIONS_INL_
#define _MATH_FUNCTIONS_INL_
#include "math/rand.inl"



#ifdef CPU_ONLY  // CPU-only Caffe.


// Stub out GPU calls as unavailable.

enum GLOGFLAG {
  GLOG_INFO,
  GLOG_WARNING,
  GLOG_ERROR,
  GLOG_FATAL,
};
void SetLogFilenameExtension(const char* ext) {}
void SetLogDestination(int info, const char* file) {}

void LOG(GLOGFLAG glogflag, const char* fmt, ...) {
  return ;
}

#define NO_GPU LOG(GLOG_FATAL, "Cannot use GPU in CPU-only Caffe: check mode.")

#define STUB_GPU(classname) \
 \
void classname::Forward_gpu(const vector<Blob*>& bottom, \
    const vector<Blob*>& top) { NO_GPU; } \
 \
void classname::Backward_gpu(const vector<Blob*>& top, \
    const vector<bool>& propagate_down, \
    const vector<Blob*>& bottom) { NO_GPU; } \

#define STUB_GPU_FORWARD(classname, funcname) \
 \
void classname::funcname##_##gpu(const vector<Blob*>& bottom, \
    const vector<Blob*>& top) { NO_GPU; } \

#define STUB_GPU_BACKWARD(classname, funcname) \
 \
void classname::funcname##_##gpu(const vector<Blob*>& top, \
    const vector<bool>& propagate_down, \
    const vector<Blob*>& bottom) { NO_GPU; } \

#else  // Normal GPU + CPU Caffe.

#include <cublas_v2.h>
#include <cuda.h>
#include <cuda_runtime.h>
#include <curand.h>
#include <driver_types.h>  // cuda driver types
#ifdef USE_CUDNN  // cuDNN acceleration library.
#include "cudnn.hpp"
#endif

//
// CUDA macros
//

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
      << caffe::cublasGetErrorString(status); \
  } while (0)

#define CURAND_CHECK(condition) \
  do { \
    curandStatus_t status = condition; \
    CHECK_EQ(status, CURAND_STATUS_SUCCESS) << " " \
      << caffe::curandGetErrorString(status); \
  } while (0)

// CUDA: grid stride looping
#define CUDA_KERNEL_LOOP(i, n) \
  for (int i = blockIdx.x * blockDim.x + threadIdx.x; \
       i < (n); \
       i += blockDim.x * gridDim.x)

// CUDA: check for error after kernel execution and exit loudly if there is one.
#define CUDA_POST_KERNEL_CHECK CUDA_CHECK(cudaPeekAtLastError())



// CUDA: library error reporting.
const char* cublasGetErrorString(cublasStatus_t error);
const char* curandGetErrorString(curandStatus_t error);

// CUDA: use 512 threads per block
const int CAFFE_CUDA_NUM_THREADS = 512;

// CUDA: number of blocks for threads.
inline int CAFFE_GET_BLOCKS(int N) {
  return (N + CAFFE_CUDA_NUM_THREADS - 1) / CAFFE_CUDA_NUM_THREADS;
}



#endif  // CPU_ONLY




// Convert macro to string
#define STRINGIFY(m) #m
#define AS_STRING(m) STRINGIFY(m)
#define CHECK0(p, msg)   ((p) ? 0 : (LOG(GLOG_FATAL, msg), 1))
#define CHECK(p)   ((p) ? 0 : (LOG(GLOG_FATAL, "CHECK_OP FATAL"), 1))
#define CHECK_OP(name, op, v1, v2)   (((v1) op (v2)) ? 0 : (LOG(GLOG_FATAL, "CHECK_OP FATAL"), 1))
#define CHECK_OP0(name, op, v1, v2, msg)   (((v1) op (v2)) ? 0 : (LOG(GLOG_FATAL, msg), 1))
#define CHECK_LE(v1, v2)  CHECK_OP(_LE, <=, v1, v2)
#define CHECK_GE(v1, v2)  CHECK_OP(_GE, >=, v1, v2)
#define CHECK_LT(v1, v2)  CHECK_OP(_LT, <, v1, v2)
#define CHECK_GT(v1, v2)  CHECK_OP(_GT, >, v1, v2)
#define CHECK_EQ(v1, v2)  CHECK_OP(_EQ, ==, v1, v2)


#define CHECK_LE0(v1, v2, msg)  CHECK_OP0(_LE, <=, v1, v2, msg)
#define CHECK_GE0(v1, v2, msg)  CHECK_OP0(_GE, >=, v1, v2, msg)
#define CHECK_LT0(v1, v2, msg)  CHECK_OP0(_LT, <, v1, v2, msg)
#define CHECK_GT0(v1, v2, msg)  CHECK_OP0(_GT, >, v1, v2, msg)
#define CHECK_EQ0(v1, v2, msg)  CHECK_OP0(_EQ, ==, v1, v2, msg)

#define EXPECT_LE(v1, v2)  ASSERT(!CHECK_OP(_LE, <=, v1, v2))
#define EXPECT_GE(v1, v2)  ASSERT(!CHECK_OP(_GE, >=, v1, v2))
#define EXPECT_LT(v1, v2)  ASSERT(!CHECK_OP(_LT, <, v1, v2))
#define EXPECT_GT(v1, v2)  ASSERT(!CHECK_OP(_GT, >, v1, v2))
#define EXPECT_EQ(v1, v2)  ASSERT(!CHECK_OP(_EQ, ==, v1, v2))

#define EXPECT_NEAR(a, b, e)  ASSERT(FNEAR(a, b, e))

#define TESTFUN(fun)  int fun(); int fun##_tested = fun(); int fun()
#define TYPED_TEST(app, fun)   TESTFUN(app ## fun)
#ifdef USE_MKL

#include <mkl.h>

#else  // If use MKL, simply include the MKL header

#ifdef USE_ACCELERATE
#include <Accelerate/Accelerate.h>
#else
extern "C" {
#include <cblas.h>
//#include "blas/cblas.inl"
  #pragma comment(lib,"libopenblas.lib")
}
#endif  // USE_ACCELERATE

#include <math.h>

#endif  // USE_MKL






// Functions that caffe uses but are not present if MKL is not linked.
// A simple way to define the vsl unary functions. The operator should
// be in the form e.g. y[i] = sqrt(a[i])
#define DEFINE_VSL_UNARY_FUNC_TYPE(Dtype, prefix, name, operator) \
  void blas_##name(int n, const Dtype* a, Dtype* y) { \
    CHECK_GT(n, 0); CHECK(a); CHECK(y); \
    for (int i = 0; i < n; ++i) { operator; } \
  }

#define DEFINE_VSL_UNARY_FUNC(name, operator)  \
  DEFINE_VSL_UNARY_FUNC_TYPE(float, s, name, operator)  \
  DEFINE_VSL_UNARY_FUNC_TYPE(double, d, name, operator)

DEFINE_VSL_UNARY_FUNC(sqr, y[i] = a[i] * a[i])
DEFINE_VSL_UNARY_FUNC(sqrt, y[i] = sqrt(a[i]))
DEFINE_VSL_UNARY_FUNC(exp, y[i] = exp(a[i]))
DEFINE_VSL_UNARY_FUNC(ln, y[i] = log(a[i]))
DEFINE_VSL_UNARY_FUNC(abs, y[i] = fabs(a[i]))

// A simple way to define the vsl unary functions with singular parameter b.
// The operator should be in the form e.g. y[i] = pow(a[i], b)
#define DEFINE_VSL_UNARY_FUNC_WITH_PARAM_TYPE(Dtype, prefix, name, operator) \
  void blas_##name(int n, const Dtype* a, const Dtype b, Dtype* y) { \
    CHECK_GT(n, 0); CHECK(a); CHECK(y); \
    for (int i = 0; i < n; ++i) { operator; } \
  }

#define DEFINE_VSL_UNARY_FUNC_WITH_PARAM(name, operator)  \
  DEFINE_VSL_UNARY_FUNC_WITH_PARAM_TYPE(float, s, name, operator)  \
  DEFINE_VSL_UNARY_FUNC_WITH_PARAM_TYPE(double, d, name, operator)

DEFINE_VSL_UNARY_FUNC_WITH_PARAM(powx, y[i] = pow(a[i], b))

// A simple way to define the vsl binary functions. The operator should
// be in the form e.g. y[i] = a[i] + b[i]
#define DEFINE_VSL_BINARY_FUNC_TYPE(Dtype, prefix, name, operator) \
  void blas_##name(int n, const Dtype* a, const Dtype* b, Dtype* y) { \
    CHECK_GT(n, 0); CHECK(a); CHECK(b); CHECK(y); \
    for (int i = 0; i < n; ++i) { operator; } \
  }

#define DEFINE_VSL_BINARY_FUNC(name, operator)  \
  DEFINE_VSL_BINARY_FUNC_TYPE(float, s, name, operator)  \
  DEFINE_VSL_BINARY_FUNC_TYPE(double, d, name, operator)

DEFINE_VSL_BINARY_FUNC(add, y[i] = a[i] + b[i])
DEFINE_VSL_BINARY_FUNC(sub, y[i] = a[i] - b[i])
DEFINE_VSL_BINARY_FUNC(mul, y[i] = a[i] * b[i])
DEFINE_VSL_BINARY_FUNC(div, y[i] = a[i] / b[i])

// In addition, MKL comes with an additional function axpby that is not present
// in standard blas. We will simply use a two-step (inefficient, of course) way
// to mimic that.
inline void cblas_saxpby(int N, float alpha, const float* X, int incX, float beta, float* Y, int incY) {
  cblas_sscal(N, beta, Y, incY);
  cblas_saxpy(N, alpha, X, incX, Y, incY);
}

inline void cblas_daxpby(int N, double alpha, const double* X, int incX, double beta, double* Y, int incY) {
  cblas_dscal(N, beta, Y, incY);
  cblas_daxpy(N, alpha, X, incX, Y, incY);
}


// the branchless, type-safe version from
// http://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
inline int8_t ssign(Dtype val) {
  return (Dtype(0) < val) - (val < Dtype(0));
}

static void blas_sign(const int n, const Dtype* x, Dtype* y) {
  for (int i = 0; i < n; ++i) {
    y[i] = ssign(x[i]);
  }
}

#if (defined _WIN32 || defined _WIN64) && (_MSC_VER == 1200)
static Dtype signbit(Dtype x) {
  ASSERT(0);
  return x;
}
#endif

static void blas_signbit(const int n, const Dtype* x, Dtype* y) {
  for (int i = 0; i < n; ++i) {
    y[i] = signbit(x[i]);
  }
}
static void blas_fabs(const int n, const Dtype* x, Dtype* y) {
  for (int i = 0; i < n; ++i) {
    y[i] = fabs(x[i]);
  }
}
static void my_sgemm(CBLAS_ORDER order, const CBLAS_TRANSPOSE TransA,
    const CBLAS_TRANSPOSE TransB, int M, int N, int K,
    float alpha, const float* A, int lda, const float* B, int ldb, float beta,
    float* C, int ldc) {
  int i, j, k, ida=1, idb=1, idc=1;
  if (CblasTrans==TransA) {
    SWAP_T(lda, ida, int);
  }
  if (CblasTrans==TransB) {
    SWAP_T(ldb, idb, int);
  }
  if (CblasColMajor==order) {
    SWAP_T(ldc, idc, int);
  }
  for (i=0; i<M; ++i) {
    for (j=0; j<N; ++j) {
      float sum = 0;
      for (k=0; k<K; ++k) {
        sum += A[i*lda+k*ida]*B[k*ldb+j*idb];
      }
      C[i*ldc+j*idc] = alpha*sum + beta*C[i*ldc+j*idc];
    }
  }
  return ;
}
static void blas_gemm1(const CBLAS_TRANSPOSE TransA,
    const CBLAS_TRANSPOSE TransB, int M, int N, int K,
    float alpha, const float* A, const float* B, float beta,
    float* C) {
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  my_sgemm(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, N);
  return ;
}
// C = alpha*op( A )*op( B ) + beta*C
// M，矩阵A的行，矩阵C的行
// N，矩阵B的列，矩阵C的列
// K，矩阵A的列，矩阵B的行
static void blas_gemm(const CBLAS_TRANSPOSE TransA,
    const CBLAS_TRANSPOSE TransB, int M, int N, int K,
    float alpha, const float* A, const float* B, float beta,
    float* C) {
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  cblas_sgemm(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, N);
  return ;
}
static void blas_gemm(const CBLAS_TRANSPOSE TransA,
    const CBLAS_TRANSPOSE TransB, int M, int N, int K,
    const double alpha, const double* A, const double* B, const double beta,
    double* C) {
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  cblas_dgemm(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, N);
}
static void blas_gemv(const CBLAS_TRANSPOSE TransA, int M, int N, float alpha,
                      const float* A, const float* x, float beta, float* y) {
  cblas_sgemv(CblasRowMajor, TransA, M, N, alpha, A, N, x, 1, beta, y, 1);
}
static void blas_gemv(const CBLAS_TRANSPOSE TransA, int M, int N, const double alpha,
                      const double* A, const double* x, const double beta, double* y) {
  cblas_dgemv(CblasRowMajor, TransA, M, N, alpha, A, N, x, 1, beta, y, 1);
}
static void blas_axpy(int N, float alpha, const float* X, float* Y) {
  cblas_saxpy(N, alpha, X, 1, Y, 1);
}
static void blas_axpy_sign(int N, float alpha, const float* X, float* Y) {
  int i;
  float map_alpha_[] = {-alpha, 0, alpha};
  float* map_alpha = map_alpha_+1;
  for (i=0; i<N; ++i) {
    int sign = ssign(X[i]);
    Y[i] += map_alpha[sign];
  }
}
static void blas_axpy(int N, const double alpha, const double* X, double* Y) {
  cblas_daxpy(N, alpha, X, 1, Y, 1);
}

static void blas_set(int N, const Dtype alpha, Dtype* Y) {
  if (alpha == 0) {
    memset(Y, 0, sizeof(Dtype) * N);  // NOLINT(caffe/alt_fn)
    return;
  }
  for (int i = 0; i < N; ++i) {
    Y[i] = alpha;
  }
}
static void blas_set(int N, const int alpha, int* Y) {
  if (alpha == 0) {
    memset(Y, 0, sizeof(int) * N);  // NOLINT(caffe/alt_fn)
    return;
  }
  for (int i = 0; i < N; ++i) {
    Y[i] = alpha;
  }
}
static void blas_add_scalar(int N, float alpha, float* Y) {
  for (int i = 0; i < N; ++i) {
    Y[i] += alpha;
  }
}
static void blas_add_scalar(int N, const double alpha, double* Y) {
  for (int i = 0; i < N; ++i) {
    Y[i] += alpha;
  }
}
static void blas_copy(int N, const Dtype* X, Dtype* Y) {
  if (X != Y) {
    memcpy(Y, X, sizeof(Dtype) * N);
  }
}
static void blas_scal(int N, float alpha, float *X) {
  cblas_sscal(N, alpha, X, 1);
}
static void blas_scal(int N, const double alpha, double *X) {
  cblas_dscal(N, alpha, X, 1);
}
static void blas_axpby(int N, float alpha, const float* X, float beta, float* Y) {
  cblas_saxpby(N, alpha, X, 1, beta, Y, 1);
}
static void blas_axpby(int N, const double alpha, const double* X,
                             const double beta, double* Y) {
  cblas_daxpby(N, alpha, X, 1, beta, Y, 1);
}
static double blas_euclidean(const int n, const float* a, const float* b) {
  double sum = 0;
  int i;
  for (i=0; i<n; ++i) {
    float t = a[i] - b[i];
    sum += t*t;
  }
  return sum;
}
static float blas_min(const int n, const float* a) {
  int i;
  if (n<=0) return 0;
  Dtype _min = a[0];
  for (i=1; i<n; ++i) {
    if (a[i]<_min) {
      _min = a[i];
    }
  }
  return _min;
}
static float blas_max(const int n, const float* a) {
  int i;
  if (n<=0) return 0;
  Dtype _max = a[0];
  for (i=1; i<n; ++i) {
    if (a[i]>_max) {
      _max = a[i];
    }
  }
  return _max;
}
static void blas_minmax(const int n, const float* a, float* _min, float* _max) {
  int i;
  for (i=0; i<n; ++i) {
    if (a[i]<*_min) {
      *_min = a[i];
    }
    if (a[i]>*_max) {
      *_max = a[i];
    }
  }
  return ;
}

static unsigned int blas_rng_rand() {
  return rng_int32(NULL);
}

// Dtype blas_nextafter(const Dtype b) { return nextafterf( b, std::numeric_limits::max());}


static void blas_rng_uniform(const int n, const Dtype a, const Dtype b, Dtype* r) {
  CHECK_GE(n, 0);
  CHECK(r);
  CHECK_LE(a, b);
  for (int i = 0; i < n; ++i) {
    r[i] = rng_uniform(NULL, a, b);
  }
}

static void blas_rng_gaussian(const int n, const Dtype _Mean, const Dtype sigma, Dtype* r) {
  CHECK_GE(n, 0);
  CHECK(r);
  CHECK_GT(sigma, 0);
  for (int i = 0; i < n; ++i) {
    r[i] = rng_normal(NULL, _Mean, sigma);
  }
}

static void blas_rng_bernoulli(const int n, const Dtype p, int* r) {
  CHECK_GE(n, 0);
  CHECK(r);
  CHECK_GE(p, 0);
  CHECK_LE(p, 1);
  for (int i = 0; i < n; ++i) {
    r[i] = rng_bernoulli(NULL, p);
  }
}

static void blas_rng_bernoulli(const int n, const Dtype p, unsigned int* r) {
  CHECK_GE(n, 0);
  CHECK(r);
  CHECK_GE(p, 0);
  CHECK_LE(p, 1);
  for (int i = 0; i < n; ++i) {
    r[i] = rng_bernoulli(NULL, p);
  }
}

static float blas_strided_dot(const int n, const float* x, const int incx,
    const float* y, const int incy) {
  return cblas_sdot(n, x, incx, y, incy);
}


static double blas_strided_dot(const int n, const double* x,
    const int incx, const double* y, const int incy) {
  return cblas_ddot(n, x, incx, y, incy);
}


static Dtype blas_dot(const int n, const Dtype* x, const Dtype* y) {
  return blas_strided_dot(n, x, 1, y, 1);
}

static float blas_asum(const int n, const float* x) {
  return cblas_sasum(n, x, 1);
}

static 
double blas_asum(const int n, const double* x) {
  return cblas_dasum(n, x, 1);
}
static 
Dtype blas_sumsq(int n, const float* x) {
  return blas_dot(n, x, x);
}
static void blas_scale(const int n, float alpha, const float *x,
                            float* y) {
  cblas_scopy(n, x, 1, y, 1);
  cblas_sscal(n, alpha, y, 1);
}


static void blas_scale(const int n, const double alpha, const double *x,
                             double* y) {
  cblas_dcopy(n, x, 1, y, 1);
  cblas_dscal(n, alpha, y, 1);
}


// y[i]= max(a*x[i], b*y[i])

static void blas_eltwise_max(int N, const Dtype alpha, const Dtype* x,
                           const Dtype beta, Dtype* y) {
  for (int i = 0; i < N; ++i) {
    Dtype a = alpha * x[i], b = beta * y[i];
    y[i] = MAX(a, b);
  }
}

// y[i]= min(a*x[i], b*y[i])
static void blas_eltwise_min(int N, const Dtype alpha, const Dtype* x,
                           const Dtype beta, Dtype* y) {
  for (int i = 0; i < N; ++i) {
    Dtype a = alpha * x[i], b = beta * y[i];
    y[i] = MIN(a, b);
  }
}

static void blas_clamp(int n, Dtype lower_bound, Dtype upper_bound, const Dtype* x, Dtype* y) {
  for (int i = 0; i < n; ++i) {
    y[i] = MIN(MAX(x[i], lower_bound), upper_bound);
  }
}

static void blas_linspace(int n, Dtype lower_bound, Dtype upper_bound, Dtype* y) {
  Dtype s = (upper_bound - lower_bound)/(n-1);
  for (int i = 0; i < n; ++i) {
    y[i] = lower_bound + s * i;
  }
}

static int blas_argmax(int n, const Dtype* a)// 返回向量最大数的序号
{
	int i, maxIndex=0;
  if (n<0) return -1;
	for(i=1;i<n;i++){
		if(a[i]>a[maxIndex]) {
			maxIndex=i;
		}
	}
	return maxIndex;
}


// Function uses casting from int to unsigned to compare if value of
// parameter a is greater or equal to zero and lower than value of
// parameter b. The b parameter is of type signed and is always positive,
// therefore its value is always lower than 0x800... where casting
// negative value of a parameter converts it to value higher than 0x800...
// The casting allows to use one condition instead of two.
inline bool is_a_ge_zero_and_a_lt_b(int a, int b) {
  return (unsigned)(a) < (unsigned)(b);
}

int im2col_cpu(const Dtype* data_im, const int channels,
    int height, int width,
    int kernel_h, int kernel_w,
    int stride_h, int stride_w,
    int pad_h, int pad_w,
    int dilation_h, int dilation_w,
    Dtype* data_col) {
  Dtype* data_col0 = data_col;
  int output_h = (height + 2 * pad_h - (dilation_h * (kernel_h - 1) + 1)) / stride_h + 1;
  int output_w = (width + 2 * pad_w - (dilation_w * (kernel_w - 1) + 1)) / stride_w + 1;
  int channel_size = height * width;
  for (int channel = channels; channel--; data_im += channel_size) {
    for (int kernel_row = 0; kernel_row < kernel_h; kernel_row++) {
      for (int kernel_col = 0; kernel_col < kernel_w; kernel_col++) {
        int input_row = -pad_h + kernel_row * dilation_h;
        for (int output_rows = output_h; output_rows; output_rows--) {
          if (!is_a_ge_zero_and_a_lt_b(input_row, height)) {
            for (int output_cols = output_w; output_cols; output_cols--) {
              *(data_col++) = 0;
            }
          } else {
            int input_col = -pad_w + kernel_col * dilation_w;
            for (int output_col = output_w; output_col; output_col--) {
              if (is_a_ge_zero_and_a_lt_b(input_col, width)) {
                *(data_col++) = data_im[input_row * width + input_col];
              } else {
                *(data_col++) = 0;
              }
              input_col += stride_w;
            }
          }
          input_row += stride_h;
        }
      }
    }
  }
  return data_col-data_col0;
}

inline void im2col_nd_core_cpu(const Dtype* data_input, const bool im2col,
    int num_spatial_axes, const int* im_shape, const int* col_shape,
    const int* kernel_shape, const int* pad, const int* stride,
    const int* dilation, Dtype* data_output) {
  if (!im2col) {
    int im_size = im_shape[0];
    for (int i = 0; i < num_spatial_axes; ++i) {
      im_size *= im_shape[1 + i];
    }
    blas_set(im_size, Dtype(0), data_output);
  }
  int kernel_size = 1;
  for (int i = 0; i < num_spatial_axes; ++i) {
    kernel_size *= kernel_shape[i];
  }
  int channels_col = col_shape[0];
  int d_offset[MAX_DIM] = {0};
  int d_iter[MAX_DIM] = {0};
  for (int c_col = 0; c_col < channels_col; ++c_col) {
    // Loop over spatial axes in reverse order to compute a per-axis offset.
    int offset = c_col;
    for (int d_i = num_spatial_axes - 1; d_i >= 0; --d_i) {
      if (d_i < num_spatial_axes - 1) {
        offset /= kernel_shape[d_i + 1];
      }
      d_offset[d_i] = offset % kernel_shape[d_i];
    }
    for (bool incremented = true; incremented; ) {
      // Loop over spatial axes in forward order to compute the indices in the
      // image and column, and whether the index lies in the padding.
      int index_col = c_col;
      int index_im = c_col / kernel_size;
      bool is_padding = false;
      int d_i;
      for (d_i = 0; d_i < num_spatial_axes; ++d_i) {
        int d = d_iter[d_i];
        int d_im = d * stride[d_i] - pad[d_i] +
            d_offset[d_i] * dilation[d_i];
        is_padding |= d_im < 0 || d_im >= im_shape[d_i + 1];
        index_col *= col_shape[d_i + 1];
        index_col += d;
        index_im *= im_shape[d_i + 1];
        index_im += d_im;
      }
      if (im2col) {
        if (is_padding) {
          data_output[index_col] = 0;
        } else {
          data_output[index_col] = data_input[index_im];
        }
      } else if (!is_padding) {  // col2im
        data_output[index_im] += data_input[index_col];
      }
      // Loop over spatial axes in reverse order to choose an index,
      // like counting.
      incremented = false;
      for (d_i = num_spatial_axes - 1; d_i >= 0; --d_i) {
        int d_max = col_shape[d_i + 1];
        CHECK_LT(d_iter[d_i], d_max);
        if (d_iter[d_i] == d_max - 1) {
          d_iter[d_i] = 0;
        } else {  // d_iter[d_i] < d_max - 1
          ++d_iter[d_i];
          incremented = true;
          break;
        }
      }
    }  // while(incremented) {
  }  // for (int c = 0; c < channels_col; ++c) {
}


void im2col_nd_cpu(const Dtype* data_im, int num_spatial_axes,
    const int* im_shape, const int* col_shape,
    const int* kernel_shape, const int* pad, const int* stride,
    const int* dilation, Dtype* data_col) {
  const bool kIm2Col = true;
  im2col_nd_core_cpu(data_im, kIm2Col, num_spatial_axes, im_shape, col_shape,
                  kernel_shape, pad, stride, dilation, data_col);
}

void col2im_cpu(const Dtype* data_col, int channels,
    int height, int width,
    int kernel_h, int kernel_w,
    int stride_h, int stride_w,
    int pad_h, int pad_w,
    int dilation_h, int dilation_w,
    Dtype* data_im) {
  blas_set(height * width * channels, Dtype(0), data_im);
  int output_h = (height + 2 * pad_h - (dilation_h * (kernel_h - 1) + 1)) / stride_h + 1;
  int output_w = (width + 2 * pad_w - (dilation_w * (kernel_w - 1) + 1)) / stride_w + 1;
  int channel_size = height * width;
  for (int channel = channels; channel--; data_im += channel_size) {
    for (int kernel_row = 0; kernel_row < kernel_h; kernel_row++) {
      for (int kernel_col = 0; kernel_col < kernel_w; kernel_col++) {
        int input_row = -pad_h + kernel_row * dilation_h;
        for (int output_rows = output_h; output_rows; output_rows--) {
          if (!is_a_ge_zero_and_a_lt_b(input_row, height)) {
            data_col += output_w;
          } else {
            int input_col = -pad_w + kernel_col * dilation_w;
            for (int output_col = output_w; output_col; output_col--) {
              if (is_a_ge_zero_and_a_lt_b(input_col, width)) {
                data_im[input_row * width + input_col] += *data_col;
              }
              data_col++;
              input_col += stride_w;
            }
          }
          input_row += stride_h;
        }
      }
    }
  }
}

void col2im_nd_cpu(const Dtype* data_col, int num_spatial_axes,
    const int* im_shape, const int* col_shape,
    const int* kernel_shape, const int* pad, const int* stride,
    const int* dilation, Dtype* data_im) {
  const bool kIm2Col = false;
  im2col_nd_core_cpu(data_col, kIm2Col, num_spatial_axes, im_shape, col_shape,
                     kernel_shape, pad, stride, dilation, data_im);
}

#endif // _MATH_FUNCTIONS_INL_
