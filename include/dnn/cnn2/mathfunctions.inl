#include <math.h>
#include <string.h>
#include <assert.h>
#include "cstd.h"
//#include "math/rand.inl"
#ifdef USE_MKL
#include <mkl.h>
#else  // If use MKL, simply include the MKL header
#ifdef USE_ACCELERATE
#include <Accelerate/Accelerate.h>
#else
#include <cblas.h>
//#include "blas/cblas.inl"
#pragma comment(lib,"libopenblas.lib")
#endif  // USE_ACCELERATE

#include <math.h>

#endif  // USE_MKL
//#include "math/rand.inl"
#ifndef _RAND_INL_
#define rng_int32(seed)          rand()
#define rng_uniform(seed, a, b)  (uniform() * ((b)-(a)) + (a))
#define rng_normal(seed, m, s)   (gaussrand()* (s) + (m))
#define rng_bernoulli(seed, p)   bernoulli(p)
#endif // _RAND_INL_

#ifndef _CSTD_H_
#define SWAP_T(a, b, T)  {T t=a;a=b,b=t;}
#define CC_INLINE static
#define ASSERT assert
typedef struct {
  int l, t, r, b;
} IRECT;
#endif

#if 0
#include "ThreadPool.inl"
#define DISPATCH_WORKER_BEGIN(n) { int _number = n; auto _worker = [&](int start, int stop)
#define DISPATCH_WORKER_END ; dispatch_worker(_worker, _number); }
#else
#define set_thread_num(x)
#define DISPATCH_WORKER_BEGIN(n) { int start = 0, stop = n;
#define DISPATCH_WORKER_END ; }
#endif

#define MAX_DIM 4

#define EXPECT_NEAR(a, b, e)  ASSERT(FNEAR(a, b, e))
#define TESTFUN(fun)  int fun(); int fun##_tested = fun(); int fun()
#define TYPED_TEST(app, fun)   TESTFUN(app ## fun)

#ifndef MAX
#define MAX(a, b) ((a)>(b)?(a):(b))
#define MIN(a, b) ((a)<(b)?(a):(b))
#endif // MAX

#define Data_2DSize(d) ((d).w * (d).h)
#define Data_3DSize(d) ((d).w * (d).h * (d).c)
#define Data_4DSize(d) ((d).w * (d).h * (d).c * (d).n)
#define Data_total(d)  Data_4DSize(d)
#define Data_4DEqu(a, b) ((a).w == (b).w && (a).h == (b).h && (a).c == (b).c && (a).n == (b).n)
#define Data_4DNeq(a, b) (!Data_4DEqu(a, b))
#define Data_2DIndex(d, ih, iw) ((ih) * (d).w + (iw))
#define Data_3DIndex(d, ic, ih, iw) (((ic) * (d).h + ih) * (d).w + (iw))
#define Data_4DIndex(d, in, ic, ih, iw)   ((((in) * (d).c + ic) * (d).h + ih) * (d).w + (iw))

struct Udata {
  float* p;
  void* u;
};
#if 0
float* resetArr(float** pdata, int count)
{
  return *pdata = (float*)realloc(*pdata, sizeof(float)*count);
}
#else
#define resetArr(pdata, count)   (*(char**)(pdata) = (char*)realloc(*(pdata), sizeof(**(pdata))*count), *(pdata))
#endif
float* reset(float** data, DataSize size)
{
  return resetArr(data, Data_total(size));
}
void* clone(void* dst, const void* src, int count)
{
  char** pdst = (char**)dst;
  if (*pdst!=(char*)src) {
    resetArr(pdst, count);
    memcpy(*pdst, src, count);
  }
  return *pdst;
}
float* fillData(float* data, DataSize size, const float item)
{
  int i, count = Data_total(size);
  for (i = 0; i < count; ++i) {
    data[i] = item;
  }
  return data;
}
float* resetZero(float** data, DataSize size)
{
  return fillData(resetArr(data, Data_total(size)), size, 0.f);
}
float* resetUdataArr(struct Udata* data, int count)
{
  return resetArr(&data->p, sizeof(float) * count);
}
float* resetUdata(struct Udata* data, DataSize size)
{
  return resetArr(&data->p, Data_total(size));
}
#define setzero(data)  memset(data, 0, sizeof(*(data)))

int* random_shuffle(int n, int* arr)
{
  int i, j;
  int t;
  for (i = n - 1; i > 0; --i) {
    j = rand() % i;
    t = arr[i], arr[i] = arr[j], arr[j] = t;
  }
  return arr;
}
int* init_perm(int n, int* arr) {
  int i;
  for (i = 0; i < n; ++i) {
    arr[i] = i;
  }
  return arr;
}
int* get_shuffle(int n, int** parr)
{
  int* arr = resetArr(parr, n);
  init_perm(n, arr);
  return random_shuffle(n, arr);
}
double uniform()
{
  return (double)rand() / RAND_MAX;
}
double gaussrand()
{
  double x = 0;
  int i, NSUM = 25;
  for (i = 0; i < NSUM; i++) {
    x += (double)rand() / RAND_MAX;
  }
  x -= NSUM / 2.0;
  x /= sqrt(NSUM / 12.0);
  return x;
}
static int bernoulli(double p)
{
  double u_0_1 = uniform();
  return ((u_0_1) <= p) ? 1 : 0;
}

// Functions that caffe uses but are not present if MKL is not linked.
// A simple way to define the vsl unary functions. The operator should
// be in the form e.g. y[i] = sqrt(a[i])
#define DEFINE_VSL_UNARY_FUNC_TYPE(TYPE, prefix, name, operator) \
  void blas_##prefix##name(int n, const TYPE* a, TYPE* y) { \
    int i;ASSERT(n>0); ASSERT(a); ASSERT(y); \
    for (i = 0; i < n; ++i) { operator; } \
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
#define DEFINE_VSL_UNARY_FUNC_WITH_PARAM_TYPE(TYPE, prefix, name, operator) \
  void blas_##prefix##name(int n, const TYPE* a, const TYPE b, TYPE* y) { \
    int i;ASSERT(n>0); ASSERT(a); ASSERT(y); \
    for (i = 0; i < n; ++i) { operator; } \
  }

#define DEFINE_VSL_UNARY_FUNC_WITH_PARAM(name, operator)  \
  DEFINE_VSL_UNARY_FUNC_WITH_PARAM_TYPE(float, s, name, operator)  \
  DEFINE_VSL_UNARY_FUNC_WITH_PARAM_TYPE(double, d, name, operator)

DEFINE_VSL_UNARY_FUNC_WITH_PARAM(powx, y[i] = pow(a[i], b))

// A simple way to define the vsl binary functions. The operator should
// be in the form e.g. y[i] = a[i] + b[i]
#define DEFINE_VSL_BINARY_FUNC_TYPE(TYPE, prefix, name, operator) \
  void blas_##prefix##name(int n, const TYPE* a, const TYPE* b, TYPE* y) { \
    int i;ASSERT(n>0); ASSERT(a); ASSERT(b); ASSERT(y); \
    for (i = 0; i < n; ++i) { operator; } \
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
CC_INLINE void blas_saxpby(int N, float alpha, const float* X, int incX, float beta, float* Y, int incY) {
  cblas_sscal(N, beta, Y, incY);
  cblas_saxpy(N, alpha, X, incX, Y, incY);
}

CC_INLINE void blas_daxpby(int N, double alpha, const double* X, int incX, double beta, double* Y, int incY) {
  cblas_dscal(N, beta, Y, incY);
  cblas_daxpy(N, alpha, X, incX, Y, incY);
}

// the branchless, type-safe version from
// http://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
CC_INLINE int ssign(float val) {
  return ((float)(0) < val) - (val < (float)(0));
}

static void blas_ssign(int n, const float* x, float* y) {
  int i;for (i = 0; i < n; ++i) {
    float val = x[i];
    y[i] = ((0) < val) - (val < (0));
  }
}

#if (defined _WIN32 || defined _WIN64) && (_MSC_VER == 1200)
static float ssignbit(float x) {
  ASSERT(0);
  return x;
}
#endif

static void blas_ssignbit(int n, const float* x, float* y) {
  int i;for (i = 0; i < n; ++i) {
    y[i] = ssignbit(x[i]);
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
static void blas_sgemm1(const CBLAS_TRANSPOSE TransA,
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
static void blas_sgemm(const CBLAS_TRANSPOSE TransA,
    const CBLAS_TRANSPOSE TransB, int M, int N, int K,
    float alpha, const float* A, const float* B, float beta,
    float* C) {
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  cblas_sgemm(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, N);
  return ;
}
static void blas_dgemm(const CBLAS_TRANSPOSE TransA,
    const CBLAS_TRANSPOSE TransB, int M, int N, int K,
    const double alpha, const double* A, const double* B, const double beta,
    double* C) {
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  cblas_dgemm(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, N);
}
static void blas_sgemv(const CBLAS_TRANSPOSE TransA, int M, int N, float alpha,
                      const float* A, const float* x, float beta, float* y) {
  cblas_sgemv(CblasRowMajor, TransA, M, N, alpha, A, N, x, 1, beta, y, 1);
}
static void blas_dgemv(const CBLAS_TRANSPOSE TransA, int M, int N, const double alpha,
                      const double* A, const double* x, const double beta, double* y) {
  cblas_dgemv(CblasRowMajor, TransA, M, N, alpha, A, N, x, 1, beta, y, 1);
}
// y += a * x
static void blas_saxpy(int N, float alpha, const float* X, float* Y) {
  cblas_saxpy(N, alpha, X, 1, Y, 1);
}
static void blas_saxpy_sign(int N, float alpha, const float* X, float* Y) {
  int i;
  float map_alpha_[] = {-alpha, 0, alpha};
  float* map_alpha = map_alpha_+1;
  for (i=0; i<N; ++i) {
    int sign = ssign(X[i]);
    Y[i] += map_alpha[sign];
  }
}
static void blas_daxpy(int N, const double alpha, const double* X, double* Y) {
  cblas_daxpy(N, alpha, X, 1, Y, 1);
}

static void blas_sset(int N, const float alpha, float* Y) {
  int i;
  if (alpha == 0) {
    memset(Y, 0, sizeof(float) * N);  // NOLINT(caffe/alt_fn)
    return;
  }
  for (i = 0; i < N-3; i+=4) {
    Y[i+0] = alpha;
    Y[i+1] = alpha;
    Y[i+2] = alpha;
    Y[i+3] = alpha;
  }
  for (i = 0; i < N; ++i) {
    Y[i] = alpha;
  }
}
static void blas_iset(int N, int alpha, int* Y) {
  int i;
  if (alpha == 0) {
    memset(Y, 0, sizeof(int) * N);  // NOLINT(caffe/alt_fn)
    return;
  }
  for (i = 0; i < N; ++i) {
    Y[i] = alpha;
  }
}
static void blas_sadd_scalar(int N, float alpha, float* Y) {
  int i;for (i = 0; i < N; ++i) {
    Y[i] += alpha;
  }
}
static void blas_dadd_scalar(int N, const double alpha, double* Y) {
  int i;for (i = 0; i < N; ++i) {
    Y[i] += alpha;
  }
}
static void blas_sscal(int N, float alpha, float *X) {
  cblas_sscal(N, alpha, X, 1);
}
static void blas_dscal(int N, const double alpha, double *X) {
  cblas_dscal(N, alpha, X, 1);
}
static void blas_saxpby1(int N, float alpha, const float* X, float beta, float* Y) {
  blas_saxpby(N, alpha, X, 1, beta, Y, 1);
}
static void blas_daxpby1(int N, const double alpha, const double* X,
                             const double beta, double* Y) {
  cblas_daxpby(N, alpha, X, 1, beta, Y, 1);
}
static double blas_seuclidean(int n, const float* a, const float* b) {
  double sum = 0;
  int i;
  for (i=0; i<n; ++i) {
    float t = a[i] - b[i];
    sum += t*t;
  }
  return sum;
}
static double blas_deuclidean(int n, const double* a, const double* b) {
  double sum = 0;
  int i;
  for (i=0; i<n; ++i) {
    float t = a[i] - b[i];
    sum += t*t;
  }
  return sum;
}
static float blas_smin(int n, const float* a) {
  int i;
  float _min;
  if (n<=0) return 0;
  _min = a[0];
  for (i=1; i<n; ++i) {
    if (a[i]<_min) {
      _min = a[i];
    }
  }
  return _min;
}
static float blas_smax(int n, const float* a) {
  int i;
  float _max;
  if (n<=0) return 0;
  _max = a[0];
  for (i=1; i<n; ++i) {
    if (a[i]>_max) {
      _max = a[i];
    }
  }
  return _max;
}
static void blas_sminmax(int n, const float* a, float* _min, float* _max) {
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

// float blas_nextafter(const float b) { return nextafterf( b, std::numeric_limits::max());}
static void blas_rng_uniform(int n, const float a, const float b, float* r) {
  int i;
  ASSERT(n>0);
  ASSERT(r);
  ASSERT(a<b);
  for (i = 0; i < n; ++i) {
    r[i] = rng_uniform(NULL, a, b);
  }
}

static void blas_rng_gaussian(int n, const float _Mean, const float sigma, float* r) {
  int i;
  ASSERT(n>0);
  ASSERT(r);
  ASSERT(sigma> 0);
  for (i = 0; i < n; ++i) {
    r[i] = rng_normal(NULL, _Mean, sigma);
  }
}

static void blas_srng_bernoulli(int n, const float p, int* r) {
  int i;
  ASSERT(n>0);
  ASSERT(r);
  ASSERT(p>0 && p<1);
  for (i = 0; i < n; ++i) {
    r[i] = rng_bernoulli(NULL, p);
  }
}

static void blas_irng_bernoulli(int n, const float p, unsigned int* r) {
  int i;
  ASSERT(n>0);
  ASSERT(r);
  ASSERT(p>0 && p<1);
  for (i = 0; i < n; ++i) {
    r[i] = rng_bernoulli(NULL, p);
  }
}
static float blas_sstrided_dot(int n, const float* x, int incx, const float* y, int incy) {
  return cblas_sdot(n, x, incx, y, incy);
}
static double blas_dstrided_dot(int n, const double* x, int incx, const double* y, int incy) {
  return cblas_ddot(n, x, incx, y, incy);
}
static float blas_sdot(int n, const float* x, const float* y) {
  return blas_sstrided_dot(n, x, 1, y, 1);
}
static float blas_sasum(int n, const float* x) {
  return cblas_sasum(n, x, 1);
}
static double blas_dasum(int n, const double* x) {
  return cblas_dasum(n, x, 1);
}
static float blas_ssumsq(int n, const float* x) {
  return blas_sdot(n, x, x);
}
static void blas_sscale(int n, float alpha, const float *x, float* y) {
  cblas_scopy(n, x, 1, y, 1);
  cblas_sscal(n, alpha, y, 1);
}


static void blas_scale(int n, const double alpha, const double *x,
                             double* y) {
  cblas_dcopy(n, x, 1, y, 1);
  cblas_dscal(n, alpha, y, 1);
}


// y[i]= max(a*x[i], b*y[i])

static void blas_eltwise_max(int N, const float alpha, const float* x,
                           const float beta, float* y) {
  int i;for (i = 0; i < N; ++i) {
    float a = alpha * x[i], b = beta * y[i];
    y[i] = MAX(a, b);
  }
}

// y[i]= min(a*x[i], b*y[i])
static void blas_eltwise_min(int N, const float alpha, const float* x,
                           const float beta, float* y) {
  int i;for (i = 0; i < N; ++i) {
    float a = alpha * x[i], b = beta * y[i];
    y[i] = MIN(a, b);
  }
}

static void blas_clamp(int n, float lower_bound, float upper_bound, const float* x, float* y) {
  int i;for (i = 0; i < n; ++i) {
    y[i] = MIN(MAX(x[i], lower_bound), upper_bound);
  }
}

static void blas_linspace(int n, float lower_bound, float upper_bound, float* y) {
  float s = (upper_bound - lower_bound)/(n-1);
  int i;for (i = 0; i < n; ++i) {
    y[i] = lower_bound + s * i;
  }
}

static int blas_argmax(int n, const float* a)// 返回向量最大数的序号
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
CC_INLINE bool is_a_ge_zero_and_a_lt_b(int a, int b) {
  return (unsigned)(a) < (unsigned)(b);
}

static float* im2col_cpu1(const float* data_im, float* data_col, DataSize inputSize,
    int kernel_h, int kernel_w, int stride_h, int stride_w, IRECT pad, int dilation_h, int dilation_w) {
  int output_h = (inputSize.h + (pad.t + pad.b) - (dilation_h * (kernel_h - 1) + 1)) / stride_h + 1;
  int output_w = (inputSize.w + (pad.l + pad.r) - (dilation_w * (kernel_w - 1) + 1)) / stride_w + 1;
  int channel_size = inputSize.h * inputSize.w;
  float* data_col0 = data_col;
  int channel, kernel_row, kernel_col, output_rows, output_cols, output_col;
  for (channel = inputSize.c; channel--; data_im += channel_size) {
    for (kernel_row = 0; kernel_row < kernel_h; kernel_row++) {
      for (kernel_col = 0; kernel_col < kernel_w; kernel_col++) {
        int input_row = -pad.t + kernel_row * dilation_h;
        for (output_rows = output_h; output_rows; output_rows--) {
          if (!is_a_ge_zero_and_a_lt_b(input_row, inputSize.h)) {
            for (output_cols = output_w; output_cols; output_cols--) {
              *data_col++ = 0;
            }
          } else {
            int input_col = -pad.l + kernel_col * dilation_w;
            for (output_col = output_w; output_col; output_col--) {
              if (is_a_ge_zero_and_a_lt_b(input_col, inputSize.w)) {
                *(data_col++) = data_im[input_row * inputSize.w + input_col];
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
  return data_col0;
}

static float* im2col_cpu(const float* data_im, float* data_col, DataSize inputSize,
    int kernel_h, int kernel_w, int stride_h, int stride_w, IRECT pad, int dilation_h, int dilation_w) {
  int output_h = (inputSize.h + (pad.t + pad.b) - (dilation_h * (kernel_h - 1) + 1)) / stride_h + 1;
  int output_w = (inputSize.w + (pad.l + pad.r) - (dilation_w * (kernel_w - 1) + 1)) / stride_w + 1;
  int channel_size = inputSize.h * inputSize.w;
  float* data_col0 = data_col;
  int channel, kernel_row, kernel_col, output_rows;
  for (channel = inputSize.c; channel--; data_im += channel_size) {
    for (kernel_row = 0; kernel_row < kernel_h; kernel_row++) {
      for (kernel_col = 0; kernel_col < kernel_w; kernel_col++) {
        int input_row = -pad.t + kernel_row * dilation_h;
        for (output_rows = output_h; output_rows; output_rows--) {
          if (!is_a_ge_zero_and_a_lt_b(input_row, inputSize.h)) {
            int output_col=0;
            for (; output_col<output_w; ++output_col) {
              data_col[output_col] = 0;
            }
          } else {
            int input_col = -pad.l + kernel_col * dilation_w;
            int output_col=0;
            const float * data_im_row;
            for (; input_col<0; ++output_col, input_col += stride_w) {
              data_col[output_col] = 0;
            }
            data_im_row = data_im+input_row * inputSize.w;
            if (1==stride_w) {
              int k = inputSize.w-input_col;
              memcpy(data_col+output_col, data_im_row+input_col, k*sizeof(*data_im));
              output_col += k;
              input_col += k;
            } else {
              for (; input_col<inputSize.w; ++output_col, input_col += stride_w) {
                data_col[output_col] = data_im_row[input_col];
              }
            }
            for (; output_col<output_w; ++output_col) {
              data_col[output_col] = 0;
            }
          }
          data_col += output_w;
          input_row += stride_h;
        }
      }
    }
  }
  return data_col0;
}
CC_INLINE void im2col_nd_core_cpu(const float* data_input, const bool im2col,
    int num_spatial_axes, const int* im_shape, const int* col_shape,
    const int* kernel_shape, const int* pad, const int* stride,
    const int* dilation, float* data_output) {
  int kernel_size, i, channels_col, c_col, d_i, incremented;
  int d_offset[MAX_DIM] = {0};
  int d_iter[MAX_DIM] = {0};
  if (!im2col) {
    int im_size = im_shape[0];
    for (i = 0; i < num_spatial_axes; ++i) {
      im_size *= im_shape[1 + i];
    }
    blas_sset(im_size, (float)(0), data_output);
  }
  kernel_size = 1;
  for (i = 0; i < num_spatial_axes; ++i) {
    kernel_size *= kernel_shape[i];
  }
  channels_col = col_shape[0];
  for (c_col = 0; c_col < channels_col; ++c_col) {
    // Loop over spatial axes in reverse order to compute a per-axis offset.
    int offset = c_col;
    for (d_i = num_spatial_axes - 1; d_i >= 0; --d_i) {
      if (d_i < num_spatial_axes - 1) {
        offset /= kernel_shape[d_i + 1];
      }
      d_offset[d_i] = offset % kernel_shape[d_i];
    }
    for (incremented = true; incremented; ) {
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
        ASSERT(d_iter[d_i] < d_max);
        if (d_iter[d_i] == d_max - 1) {
          d_iter[d_i] = 0;
        } else {  // d_iter[d_i] < d_max - 1
          ++d_iter[d_i];
          incremented = true;
          break;
        }
      }
    }  // while(incremented) {
  }  // for (c = 0; c < channels_col; ++c) {
}


void im2col_nd_cpu(const float* data_im, int num_spatial_axes,
    const int* im_shape, const int* col_shape,
    const int* kernel_shape, const int* pad, const int* stride,
    const int* dilation, float* data_col) {
  const bool kIm2Col = true;
  im2col_nd_core_cpu(data_im, kIm2Col, num_spatial_axes, im_shape, col_shape,
                  kernel_shape, pad, stride, dilation, data_col);
}

float* col2im_cpu(const float* data_col, float* data_im, DataSize inputSize,
    int kernel_h, int kernel_w, int stride_h, int stride_w, IRECT pad, int dilation_h, int dilation_w) {
  int output_h = (inputSize.h + (pad.t+pad.b) - (dilation_h * (kernel_h - 1) + 1)) / stride_h + 1;
  int output_w = (inputSize.w + (pad.l+pad.r) - (dilation_w * (kernel_w - 1) + 1)) / stride_w + 1;
  int channel_size = inputSize.h * inputSize.w;
  int channel, kernel_row, kernel_col, output_rows;
  float* data_im0 = data_im;
  blas_sset(Data_3DSize(inputSize), (0), data_im);
  for (channel = inputSize.c; channel--; data_im += channel_size) {
    for (kernel_row = 0; kernel_row < kernel_h; kernel_row++) {
      for (kernel_col = 0; kernel_col < kernel_w; kernel_col++) {
        int input_row = -pad.t + kernel_row * dilation_h;
        for (output_rows = output_h; output_rows; output_rows--) {
          if (!is_a_ge_zero_and_a_lt_b(input_row, inputSize.h)) {
            data_col += output_w;
          } else {
            int input_col = -pad.l + kernel_col * dilation_w;
#if 1
            int output_col = 0;
            float* data_im_row = data_im + input_row * inputSize.w;
            for (; input_col<0; ++output_col, input_col += stride_w) {
            }
            if (1==stride_w) {
              blas_saxpy(inputSize.w-input_col, 1, data_col+output_col, data_im_row+input_col);
            } else {
              for (; input_col < inputSize.w; ++output_col, input_col += stride_w) {
                data_im_row[input_col] += data_col[output_col];
              }
            }
            for (; output_col < output_w; ++output_col, input_col += stride_w) {
            }
            data_col+=output_w;
#else
            for (output_col = output_w; output_col; output_col--) {
              if (is_a_ge_zero_and_a_lt_b(input_col, inputSize.w)) {
                data_im[input_row * inputSize.w + input_col] += *data_col;
              }
              data_col++;
              input_col += stride_w;
            }
#endif
          }
          input_row += stride_h;
        }
      }
    }
  }
  return data_im0;
}

void col2im_nd_cpu(const float* data_col, int num_spatial_axes,
    const int* im_shape, const int* col_shape,
    const int* kernel_shape, const int* pad, const int* stride,
    const int* dilation, float* data_im) {
  const bool kIm2Col = false;
  im2col_nd_core_cpu(data_col, kIm2Col, num_spatial_axes, im_shape, col_shape,
                     kernel_shape, pad, stride, dilation, data_im);
}
static void substract_mean_normalize(int c, int h, int w, float* data, int cstep, const float* mean_vals, const float* norm_vals)
{
  int q, size = w * h;

  if (mean_vals && !norm_vals) {
    // substract mean only
    #pragma omp parallel for
    for (q = 0; q < c; q++) {
      float* ptr = data + cstep * q;
      const float mean = mean_vals[q];

#if __ARM_NEON
      int nn = size >> 2;
      int remain = size - (nn << 2);
#else
      int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
#if __aarch64__
      float32x4_t _mean = vdupq_n_f32(mean);

      for (; nn > 0; nn--) {
        float32x4_t _ptr = vld1q_f32(ptr);
        _ptr = vsubq_f32(_ptr, _mean);
        vst1q_f32(ptr, _ptr);
        ptr += 4;
      }

#else

      if (nn > 0) {
        asm volatile(
            "vdup.f32   q1, %4              \n"
            "0:                             \n"
            "pld        [%1, #128]          \n"
            "vld1.f32   {d0-d1}, [%1 :128]  \n"
            "vsub.f32   q0, q0, q1          \n"
            "subs       %0, #1              \n"
            "vst1.f32   {d0-d1}, [%1 :128]! \n"
            "bne        0b                  \n"
            : "=r"(nn),     // %0
            "=r"(ptr)     // %1
            : "0"(nn),
            "1"(ptr),
            "r"(mean)     // %4
            : "cc", "memory", "q0", "q1"
        );
      }

#endif // __aarch64__
#endif // __ARM_NEON

      for (; remain > 0; remain--) {
        *ptr -= mean;
        ptr++;
      }
    }
  }
  else if (!mean_vals && norm_vals) {
    // normalize only
    #pragma omp parallel for
    for (q = 0; q < c; q++) {
      float* ptr = data + cstep * q;
      const float norm = norm_vals[q];

#if __ARM_NEON
      int nn = size >> 2;
      int remain = size - (nn << 2);
#else
      int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
#if __aarch64__
      float32x4_t _norm = vdupq_n_f32(norm);

      for (; nn > 0; nn--) {
        float32x4_t _ptr = vld1q_f32(ptr);
        _ptr = vmulq_f32(_ptr, _norm);
        vst1q_f32(ptr, _ptr);
        ptr += 4;
      }

#else

      if (nn > 0) {
        asm volatile(
            "vdup.f32   q1, %4              \n"
            "0:                             \n"
            "pld        [%1, #128]          \n"
            "vld1.f32   {d0-d1}, [%1 :128]  \n"
            "vmul.f32   q0, q0, q1          \n"
            "subs       %0, #1              \n"
            "vst1.f32   {d0-d1}, [%1 :128]! \n"
            "bne        0b                  \n"
            : "=r"(nn),     // %0
            "=r"(ptr)     // %1
            : "0"(nn),
            "1"(ptr),
            "r"(norm)     // %4
            : "cc", "memory", "q0", "q1"
        );
      }

#endif // __aarch64__
#endif // __ARM_NEON

      for (; remain > 0; remain--) {
        *ptr *= norm;
        ptr++;
      }
    }
  }
  else if (mean_vals && norm_vals) {
    // substract mean and normalize
    #pragma omp parallel for
    for (q = 0; q < c; q++) {
      float* ptr = data + cstep * q;
      const float mean = mean_vals[q];
      const float norm = norm_vals[q];

#if __ARM_NEON
      int nn = size >> 2;
      int remain = size - (nn << 2);
#else
      int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
#if __aarch64__
      float32x4_t _mean = vdupq_n_f32(mean);
      float32x4_t _norm = vdupq_n_f32(norm);

      for (; nn > 0; nn--) {
        float32x4_t _ptr = vld1q_f32(ptr);
        _ptr = vsubq_f32(_ptr, _mean);
        _ptr = vmulq_f32(_ptr, _norm);
        vst1q_f32(ptr, _ptr);
        ptr += 4;
      }

#else

      if (nn > 0) {
        asm volatile(
            "vdup.f32   q1, %4              \n"
            "vdup.f32   q2, %5              \n"
            "0:                             \n"
            "pld        [%1, #128]          \n"
            "vld1.f32   {d0-d1}, [%1 :128]  \n"
            "vsub.f32   q0, q0, q1          \n"
            "vmul.f32   q0, q0, q2          \n"
            "subs       %0, #1              \n"
            "vst1.f32   {d0-d1}, [%1 :128]! \n"
            "bne        0b                  \n"
            : "=r"(nn),     // %0
            "=r"(ptr)     // %1
            : "0"(nn),
            "1"(ptr),
            "r"(mean),    // %4
            "r"(norm)     // %5
            : "cc", "memory", "q0", "q1", "q2"
        );
      }

#endif // __aarch64__
#endif // __ARM_NEON

      for (; remain > 0; remain--) {
        *ptr = (*ptr - mean) * norm;
        ptr++;
      }
    }
  }
}

// convert half precision floating point to float
static float half2float(unsigned short value)
{
  // 1 : 5 : 10
  unsigned short sign = (value & 0x8000) >> 15;
  unsigned short exponent = (value & 0x7c00) >> 10;
  unsigned short significand = value & 0x03FF;

  //     fprintf(stderr, "%d %d %d\n", sign, exponent, significand);

  // 1 : 8 : 23
  union {
    unsigned int u;
    float f;
  } tmp;

  if (exponent == 0) {
    if (significand == 0) {
      // zero
      tmp.u = (sign << 31);
    }
    else {
      // denormal
      exponent = 0;

      // find non-zero bit
      while ((significand & 0x200) == 0) {
        significand <<= 1;
        exponent++;
      }

      significand <<= 1;
      significand &= 0x3FF;
      tmp.u = (sign << 31) | ((-exponent + (-15 + 127)) << 23) | (significand << 13);
    }
  }
  else if (exponent == 0x1F) {
    // infinity or NaN
    tmp.u = (sign << 31) | (0xFF << 23) | (significand << 13);
  }
  else {
    // normalized
    tmp.u = (sign << 31) | ((exponent + (-15 + 127)) << 23) | (significand << 13);
  }

  return tmp.f;
}

static int from_float16(float* data32, const unsigned short* data16, int size)
{
  float* ptr = data32;

#if __ARM_NEON && (__ARM_FP & 2)
  int nn = cpu_support_arm_vfpv4() ? size >> 2 : 0;
  int remain = size - (nn << 2);
#else
  int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON && (__ARM_FP & 2)
#if __aarch64__

  if (nn > 0) {
    asm volatile(
        "0:                             \n"
        "ldr    d0, [%1], #8            \n"
        "fcvtl  v1.4s, v0.4h            \n"
        "subs   %w0, %w0, #1            \n"
        "str    q1, [%2], #16           \n"
        "bne    0b                      \n"
        : "=r"(nn),     // %0
        "=r"(data16),   // %1
        "=r"(ptr)     // %2
        : "0"(nn),
        "1"(data16),
        "2"(ptr)
        : "cc", "memory", "v0", "v1"
    );
  }

#else

  if (nn > 0) {
    asm volatile(
        "0:                             \n"
        "pld        [%1, #64]           \n"
        "vld1.s16   {d0}, [%1 :64]!     \n"
        "vcvt.f32.f16 q1, d0            \n"
        "subs       %0, #1              \n"
        "vst1.f32   {d2-d3}, [%2 :128]! \n"
        "bne        0b                  \n"
        : "=r"(nn),     // %0
        "=r"(data16),   // %1
        "=r"(ptr)     // %2
        : "0"(nn),
        "1"(data16),
        "2"(ptr)
        : "cc", "memory", "q0", "q1"
    );
  }

#endif // __aarch64__
#endif // __ARM_NEON

  for (; remain > 0; remain--) {
    *ptr = half2float(*data16);

    data16++;
    ptr++;
  }

  return 0;
}

// mat process
enum BORDER_TYPE {
  BORDER_CONSTANT = 0,
  BORDER_REPLICATE = 1,
};

static void copy_make_border_image(const float* src, float* dst, int src_w, int src_h, int dst_w, int dst_h, int top, int left, enum BORDER_TYPE type, float v)
{
  const float* ptr = (src);
  float* outptr = (dst);

  if (type == BORDER_CONSTANT) {
    int y = 0;

    // fill top
    for (; y < top; y++) {
      int x = 0;

      for (; x < dst_w; x++) {
        outptr[x] = v;
      }

      outptr += dst_w;
    }

    // fill center
    for (; y < (top + src_h); y++) {
      int x = 0;

      for (; x < left; x++) {
        outptr[x] = v;
      }

      for (; x < (left + src_w); x++) {
        outptr[x] = ptr[x - left];
      }

      for (; x < dst_w; x++) {
        outptr[x] = v;
      }

      ptr += src_w;
      outptr += dst_w;
    }

    // fill bottom
    for (; y < dst_h; y++) {
      int x = 0;

      for (; x < dst_w; x++) {
        outptr[x] = v;
      }

      outptr += dst_w;
    }
  }
  else if (type == BORDER_REPLICATE) {
    int y = 0;

    // fill top
    for (; y < top; y++) {
      int x = 0;

      for (; x < left; x++) {
        outptr[x] = ptr[0];
      }

      for (; x < (left + src_w); x++) {
        outptr[x] = ptr[x - left];
      }

      for (; x < dst_w; x++) {
        outptr[x] = ptr[src_w - 1];
      }

      outptr += dst_w;
    }

    // fill center
    for (; y < (top + src_h); y++) {
      int x = 0;

      for (; x < left; x++) {
        outptr[x] = ptr[0];
      }

      for (; x < (left + src_w); x++) {
        outptr[x] = ptr[x - left];
      }

      for (; x < dst_w; x++) {
        outptr[x] = ptr[src_w - 1];
      }

      ptr += src_w;
      outptr += dst_w;
    }

    // fill bottom
    ptr -= src_w;

    for (; y < dst_h; y++) {
      int x = 0;

      for (; x < left; x++) {
        outptr[x] = ptr[0];
      }

      for (; x < (left + src_w); x++) {
        outptr[x] = ptr[x - left];
      }

      for (; x < dst_w; x++) {
        outptr[x] = ptr[src_w - 1];
      }

      outptr += dst_w;
    }
  }
}

static float* copy_make_border(const float* src, float** pdst, DataSize srcSize, DataSize* pdstSize,
                               IRECT pad, enum BORDER_TYPE type, float v)
{
  DataSize dstSize = dataSize(srcSize.n, srcSize.c, srcSize.h + pad.t + pad.b, srcSize.w + pad.l + pad.r);
  int i;
  int srcCount = Data_2DSize(srcSize);
  int dstCount = Data_2DSize(dstSize);
  if (pdstSize) {
    *pdstSize = dstSize;
  }

  if (dstSize.w == srcSize.w && dstSize.h == srcSize.h) {
    clone(pdst, src, Data_total(dstSize));
    return *pdst;
  }

  resetArr(pdst, Data_total(dstSize));
  // unroll image channel
  #pragma omp parallel for
  for (i = 0; i < srcSize.c; i++) {
    copy_make_border_image(src + i*srcCount, *pdst + i*dstCount, srcSize.w, srcSize.h, dstSize.w, dstSize.h, pad.t, pad.l, type, v);
  }

  return *pdst;
}

static float* copy_cut_border_image(const float* src, float* dst, int src_w, int src_h, int dst_w, int dst_h, int top, int left)
{
  int x, y;

  const float* ptr = src + left + src_w * top;
  float* outptr = (dst);

  for (y = 0; y < dst_h; y++) {
    for (x = 0; x < dst_w; x++) {
      outptr[x] = ptr[x];
    }

    outptr += dst_w;
    ptr += src_w;
  }

  return dst;
}

static float* copy_cut_border(const float* src, float** pdst, DataSize srcSize, DataSize* pdstSize, int top, int bottom, int left, int right)
{
  DataSize dstSize = dataSize(srcSize.n, srcSize.c, srcSize.h - top - bottom, srcSize.w - left - right);
  int i, n = srcSize.n*srcSize.c;
  int srcCount = Data_2DSize(srcSize);
  int dstCount = Data_2DSize(dstSize);
  if (pdstSize) {
    *pdstSize = dstSize;
  }

  if (dstSize.w == srcSize.w && dstSize.h == srcSize.h) {
    clone(pdst, src, Data_total(srcSize));
    return *pdst;
  }
  resetArr(pdst, Data_total(dstSize));

  // unroll image channel
#pragma omp parallel for
  for (i = 0; i < n; i++) {
    copy_cut_border_image(src + i*srcCount, *pdst + i*dstCount, srcSize.w, srcSize.h, dstSize.w, dstSize.h, top, left);
  }
  
  return *pdst;
}

static float* resize_bilinear_image(const float* src, float* dst, int src_w, int src_h, int dst_w, int dst_h)
{
  int w = dst_w, h = dst_h;
  double scale_x = (double)src_w / w;
  double scale_y = (double)src_h / h;
  int* buf = MALLOC(int, w + h + w * 2 + h * 2);
  int* xofs = buf;//new int[w];
  int* yofs = buf + w;//new int[h];
  float* alpha = (float*)(buf + w + h);//new float[w * 2];
  float* beta = (float*)(buf + w + h + w * 2); //new float[h * 2];
  float fx, fy;
  float* _rowsbuf, *rows0, *rows1;
  int sx, sy, dy, dx;
  int prev_sy1;

  for (dx = 0; dx < w; dx++) {
    fx = (float)((dx + 0.5) * scale_x - 0.5);
    sx = fx;//cvFloor(fx);
    fx -= sx;

    if (sx >= src_w - 1) {
      sx = src_w - 2;
      fx = 1.f;
    }

    xofs[dx] = sx;

    alpha[dx * 2    ] = 1.f - fx;
    alpha[dx * 2 + 1] = fx;
  }

  for (dy = 0; dy < h; dy++) {
    fy = (float)((dy + 0.5) * scale_y - 0.5);
    sy = fy;//cvFloor(fy);
    fy -= sy;

    if (sy >= src_h - 1) {
      sy = src_h - 2;
      fy = 1.f;
    }

    yofs[dy] = sy;

    beta[dy * 2    ] = 1.f - fy;
    beta[dy * 2 + 1] = fy;
  }

  // loop body
  _rowsbuf = MALLOC(float, 2 * (w + 1));
  rows0 = _rowsbuf;
  rows1 = _rowsbuf + w + 1;

  prev_sy1 = -1;

  for (dy = 0; dy < h; dy++) {
    int sy = yofs[dy];

    if (sy == prev_sy1) {
      // hresize one row
      float* rows1p;
      int dx = 0;
      const float* S1 = src + (sy + 1) * src_w;
      const float* alphap = alpha;
      float* rows0_old = rows0;
      rows0 = rows1;
      rows1 = rows0_old;
      rows1p = rows1;
#if __ARM_NEON

      for (; dx + 1 < w; dx += 2) {
        int sx = xofs[dx];
        int sxn = xofs[dx + 1];
        const float* S1p = S1 + sx;
        const float* S1np = S1 + sxn;

        float32x4_t _a = vld1q_f32(alphap);
        float32x2_t _S1 = vld1_f32(S1p);
        float32x2_t _S1n = vld1_f32(S1np);

        float32x4_t _S1S1n = vcombine_f32(_S1, _S1n);
        float32x4_t _ms1 = vmulq_f32(_S1S1n, _a);
        float32x2_t _rows1 = vpadd_f32(vget_low_f32(_ms1), vget_high_f32(_ms1));

        vst1_f32(rows1p + dx, _rows1);

        alphap += 4;
      }

#endif // __ARM_NEON

      for (; dx < w; dx++) {
        int sx = xofs[dx];
        const float* S1p = S1 + sx;

        float a0 = alphap[0];
        float a1 = alphap[1];
        rows1p[dx] = S1p[0] * a0 + S1p[1] * a1;

        alphap += 2;
      }
    }
    else {
      // hresize two rows
      const float* S0 = src + sy * src_w;
      const float* S1 = src + (sy + 1) * src_w;

      const float* alphap = alpha;
      float* rows0p = rows0;
      float* rows1p = rows1;
      int dx = 0;
#if __ARM_NEON

      for (; dx + 1 < w; dx += 2) {
        int sx = xofs[dx];
        int sxn = xofs[dx + 1];
        const float* S0p = S0 + sx;
        const float* S1p = S1 + sx;
        const float* S0np = S0 + sxn;
        const float* S1np = S1 + sxn;

        float32x4_t _a = vld1q_f32(alphap);
        float32x2_t _S0 = vld1_f32(S0p);
        float32x2_t _S1 = vld1_f32(S1p);
        float32x2_t _S0n = vld1_f32(S0np);
        float32x2_t _S1n = vld1_f32(S1np);

        float32x4_t _S0S0n = vcombine_f32(_S0, _S0n);
        float32x4_t _S1S1n = vcombine_f32(_S1, _S1n);
        float32x4_t _ms0 = vmulq_f32(_S0S0n, _a);
        float32x4_t _ms1 = vmulq_f32(_S1S1n, _a);
        float32x2_t _rows0 = vpadd_f32(vget_low_f32(_ms0), vget_high_f32(_ms0));
        float32x2_t _rows1 = vpadd_f32(vget_low_f32(_ms1), vget_high_f32(_ms1));

        vst1_f32(rows0p + dx, _rows0);
        vst1_f32(rows1p + dx, _rows1);

        alphap += 4;
      }

#endif // __ARM_NEON

      for (; dx < w; dx++) {
        int sx = xofs[dx];
        const float* S0p = S0 + sx;
        const float* S1p = S1 + sx;

        float a0 = alphap[0];
        float a1 = alphap[1];
        rows0p[dx] = S0p[0] * a0 + S0p[1] * a1;
        rows1p[dx] = S1p[0] * a0 + S1p[1] * a1;

        alphap += 2;
      }
    }

    prev_sy1 = sy + 1;

    {
    // vresize
    float b0 = beta[0];
    float b1 = beta[1];

    float* rows0p = rows0;
    float* rows1p = rows1;
    float* Dp = dst + dy * dst_w;

#if __ARM_NEON
    int nn = w >> 3;
#else
    int nn = 0;
#endif
    int remain = w - (nn << 3);

#if __ARM_NEON
    float32x4_t _b0 = vdupq_n_f32(b0);
    float32x4_t _b1 = vdupq_n_f32(b1);

    for (; nn > 0; nn--) {
      float32x4_t _rows0 = vld1q_f32(rows0p);
      float32x4_t _rows1 = vld1q_f32(rows1p);

      float32x4_t _D = vmulq_f32(_rows0, _b0);
      _D = vmlaq_f32(_D, _rows1, _b1);

      vst1q_f32(Dp, _D);

      float32x4_t _rows0n = vld1q_f32(rows0p + 4);
      float32x4_t _rows1n = vld1q_f32(rows1p + 4);

      float32x4_t _Dn = vmulq_f32(_rows0n, _b0);
      _Dn = vmlaq_f32(_Dn, _rows1n, _b1);

      vst1q_f32(Dp + 4, _Dn);

      Dp += 8;
      rows0p += 8;
      rows1p += 8;
    }

#endif // __ARM_NEON

    for (; remain; --remain) {
      //             D[x] = rows0[x]*b0 + rows1[x]*b1;
      *Dp++ = *rows0p++ * b0 + *rows1p++ * b1;
    }


    }
    beta += 2;
  }

  FREE(_rowsbuf);
  FREE(buf);
  return dst;
}

static float* resize_bilinear(const float* src, float** pdst, DataSize srcSize, DataSize* pdstSize, int w, int h)
{
  DataSize dstSize = dataSize(srcSize.n, srcSize.c, w, h);
  int i, n = srcSize.n*srcSize.c;
  int srcCount = Data_2DSize(srcSize);
  int dstCount = Data_2DSize(dstSize);
  if (pdstSize) {
    *pdstSize = dstSize;
  }
  if (dstSize.w == srcSize.w && dstSize.h == srcSize.h) {
    clone(pdst, src, Data_total(dstSize));
    return *pdst;
  }
  
  resetArr(pdst, Data_total(dstSize));
  
  // unroll image channel
#pragma omp parallel for
  
  for (i = 0; i < n; i++) {
    resize_bilinear_image(src + i*srcCount, *pdst + i*dstCount, srcSize.w, srcSize.h, dstSize.w, dstSize.h);
  }
  
  return *pdst;
}

static void fill_PositiveUnitball(float* data, DataSize size) {
  int i, j, dim, count = Data_total(size);
  blas_rng_uniform(count, 0, 1, data);
  // We expect the filler to not be called very frequently, so we will
  // just use a simple implementation
  dim = count / size.n;
  for (i = 0; i < size.n; ++i) {
    float sum = 0;
    for (j = 0; j < dim; ++j) {
      sum += data[i * dim + j];
    }
    for (j = 0; j < dim; ++j) {
      data[i * dim + j] /= sum;
    }
  }
}

#if 0
static void fill_Xavier(float* data, DataSize size, Method variance_norm DEFAULT(Method_FAN_IN)) {
  int count = Data_total(im);
  int fan_in = count / size.n;
  int fan_out = count / size.c;
  float n = fan_in;  // default to fan_in
  if (variance_norm == Method_AVERAGE) {
    n = (fan_in + fan_out) / (float)(2);
  } else if (variance_norm == Method_FAN_OUT) {
    n = fan_out;
  }
  float scale = sqrt((float)(3) / n);
  blas_rng_uniform(count, -scale, scale, data);
}

static void fill_MSRA(float* data, DataSize size, Method variance_norm, float alpha) {
  int count = Data_total(im);
  int fan_in = count / size.n;
  int fan_out = count / size.c;
  float n = fan_out;  // default to fan_in
  if (variance_norm == Method_AVERAGE) {
    n = (fan_in + fan_out) / (float)(2);
  } else if (variance_norm == Method_FAN_IN) {
    n = fan_in;
  }
  float std = sqrt((float)(2) / n / (1 + alpha * alpha));
  blas_rng_gaussian(count, (0), std, data);
}
#endif

static void fill_Bilinear(float* data, DataSize size) {
  int count = Data_total(size);
  //CHECK_EQ(blob->num_axes(), 4) << "Blob must be 4 dim.";
  //CHECK_EQ(blob->input_w(), blob->input_h()) << "Filter must be square";
  int f = ceil(size.w / 2.);
  float c = (2 * f - 1 - f % 2) / (2. * f);
  int i;for (i = 0; i < count; ++i) {
    float x = i % size.w;
    float y = (i / size.w) % size.h;
    data[i] = (1 - fabs(x / f - c)) * (1 - fabs(y / f - c));
  }
}
static void fill_GaussianUnitBall(float* data, DataSize size) {
  int count = Data_total(size);
  int fan_in = count / size.n;
  int i, n = fan_in;  // default to fan_in
  float sum_sq;
  blas_rng_gaussian(count, (0), 1, data);
  for (i = 0; i < size.n; i++) {
    sum_sq = blas_sdot(n, data + i * n, data + i * n) + 1e-12;
    blas_sscale(n, (float)(1.0) / sqrt(sum_sq), data + i * n, data + i * n);
  }
}


void normal_distribution_init(float* data, int count, const float mean_value, const float standard_deviation)
{
  int i;
  for (i = 0; i < count; i++) {
    //data[i] = dist(engine);
    data[i] = (float)gaussrand() * standard_deviation + mean_value;
  }
}
static void uniform_distribution_init(float* data, int count, const float low_value, const float high_deviation)
{
  int i;
  float a = high_deviation - low_value;
  for (i = 0; i < count; i++) {
    //data[i] = dist(engine);
    data[i] = (float)uniform() * a + low_value;
  }
}
static void const_distribution_init(float* data, int count, float const_value)
{
  int i;
  for (i = 0; i < count; i++) {
    data[i] = const_value;
  }
}
static void xavier_init(float* data, int count, int fan_in, int fan_out)
{
  float weight_base = (float)sqrt(6.0f / (float)(fan_in + fan_out));
  uniform_distribution_init(data, count, -weight_base, weight_base);
}
static float moving_average(float avg, int acc_number, float value)
{
  avg -= avg / acc_number;
  avg += value / acc_number;
  return avg;
}
static void mul(const float* a, const float* b, float* c, int len)
{
  int i;
  for (i = 0; i < len; i++) {
    c[i] = a[i] * b[i];
  }
}
static void mul_inplace(float* a, const float* b, int len)
{
  int i;
  for (i = 0; i < len; i++) {
    a[i] *= b[i];
  }
}
//a /= b
static void div_inplace(float* a, const float b, int len)
{
  int i;
  float inv_b = 1./b;
  for (i = 0; i < len; i++) {
    a[i] *= inv_b;
  }
}
//f(x)=1/(1+e^(-x))
static void sigmoid(const float* X, float* y, int len)
{
  int i;
  for (i = 0; i < len; i++) {
    float x = X[i];
    y[i] = 1.0f / (1.0f + expf(-1.0f * x));
  }
}
//f'(x) = x(1-x)
static void df_sigmoid(const float* X, float* y, int len)
{
  int i;for (i = 0; i < len; i++) {
    float x = X[i];
    y[i] = x * (1.0f - x);
  }
}
//f(x)=(e^x-e^(-x))/(e^x+e^(-x))
static void tanh1(const float* X, float* Y, int len)
{
  int i;
  for (i = 0; i < len; i++) {
    const float x = X[i];
    const float ex = expf(x);
    const float efx = expf(-x);
    Y[i] = (ex - efx) / (ex + efx);
  }
}
//f'(x)=1-x^(1/2)
void df_tanh(const float* X, float* y, int len)
{
  int i;
  for (i = 0; i < len; i++) {
    const float x = X[i];
    y[i] = 1.0f - sqrtf(x);
  }
}
//f(x)=max(x,0)
static void relu(const float* X, float* y, int len)
{
  int i;
  for (i = 0; i < len; i++) {
    const float x = X[i];
#define MAX_OP(a,b) (a)>(b)?(a):(b)
    y[i] = MAX_OP(x, 0.0f);
#undef MAX_OP
  }
}
//f'(x)=0(x<=0),1(x>0)
static void df_relu(const float* X, float* y, int len)
{
  int i;
  for (i = 0; i < len; i++) {
    const float x = X[i];
    y[i] = x <= 0.0f ? 0.01f : 1.0f;
  }
}
//
static float get_base(float x)
{
  int i;
  for (i = 0; i >= -10; i--) {
    float base = powf(10.0f, (float)i) + 0.000001f;
    if (x > base) {
      return base;
    }
  }
  return 0.0f;
}

static int showmat(const char* name, const float* data, DataSize size) {
  int pn, pc;
  for (pn = 0; pn < size.n; pn++) {
    for (pc = 0; pc < size.c; pc++) {
      const float* imageData = data + Data_4DIndex(size, pn, pc, 0, 0);
      const int imageWidth = size.w;
      const int imageStride = imageWidth;
      const int imageHeight = size.h;
      const int imageChannel = 1;
      img_t image[1] = {0};
      char buf[256];
      cvSetMat(image, size.h, size.w, CC_32F, 1, NULL, CC_AUTOSTEP);
      _snprintf(buf, 256, "%s_n%d_c%d", name, pn, pc);
      ShowImagePal(buf, size.h, size.w, (const uchar*)imageData, size.w*sizeof(*data), sizeof(*data), PF_32bppGrayFloat, 0);
    }
  }
  return 0;
}

#include "layer/conv.inl"
#include "layer/inner_product_layer.inl"
