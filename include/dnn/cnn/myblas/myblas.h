
#include "std/stddef_c.h"
#include <math.h>
#include <string.h>
#include <assert.h>
typedef float Dtype;
//#include "math/rand.inl"

//#include <mkl.h>
//#include <Accelerate/Accelerate.h>

#ifdef _WIN32
#ifdef USE_CBLAS
#include <cblas.h>
#endif
#endif
//#include "blas/cblas.inl"
#define MYBLAS(NAME)  my_ ## NAME
#ifdef CBLAS_H
#pragma comment(lib,"libopenblas.lib")
#define BLAS(NAME)  cblas_ ## NAME
#else
#define BLAS(NAME)  my_ ## NAME
#endif
// x 	pad byte 	no value 	  	 
// c 	char 	string of length 1 	1 	 
// b 	signed char 	integer 	1 	(3)
// B 	unsigned char 	integer 	1 	(3)
// ? 	_Bool 	bool 	1 	(1)
// h 	short 	integer 	2 	(3)
// H 	unsigned short 	integer 	2 	(3)
// i 	int 	integer 	4 	(3)
// I 	unsigned int 	integer 	4 	(3)
// l 	long 	integer 	4 	(3)
// L 	unsigned long 	integer 	4 	(3)
// q 	long long 	integer 	8 	(2), (3)
// Q 	unsigned long long 	integer 	8 	(2), (3)
// f 	float 	float 	4 	(4)
// d 	double 	float 	8 	(4)
// s 	char[] 	string 	  	 
// p 	char[] 	string 	  	 
// P 	void * 	integer 	  	(5), (3)

#ifndef CBLAS_H
typedef enum CBLAS_ORDER     {CblasRowMajor=101, CblasColMajor=102} CBLAS_ORDER;
typedef enum CBLAS_TRANSPOSE {CblasNoTrans=111, CblasTrans=112, CblasConjTrans=113, CblasConjNoTrans=114} CBLAS_TRANSPOSE;
typedef enum CBLAS_UPLO      {CblasUpper=121, CblasLower=122} CBLAS_UPLO;
typedef enum CBLAS_DIAG      {CblasNonUnit=131, CblasUnit=132} CBLAS_DIAG;
typedef enum CBLAS_SIDE      {CblasLeft=141, CblasRight=142} CBLAS_SIDE;
#endif

#define fequal(a, b)  ((a)==(b))

#define SBLAS(NAME)  my_s ## NAME
#define DBLAS(NAME)  my_d ## NAME

#define blas_sset my_sset
#define blas_ssqr my_ssqr
#define blas_ssqrt my_ssqrt
#define blas_smul my_smul
#define blas_sdiv my_sdiv
#define blas_sadd my_sadd
#define blas_ssub my_ssub
#define blas_sabs my_sabs
#define blas_spowx my_spow

#include "sblas.h"
#define TBLAS  SBLAS
#define ISDOUBLE 0
#define FTYPE  float
#include "tblas.h"

#define TBLAS  DBLAS
#define ISDOUBLE 1
#define FTYPE  double
#include "tblas.h"

#define MAX_DIM 4
#ifndef MAX
#define MAX(a, b) ((a)>(b)?(a):(b))
#define MIN(a, b) ((a)<(b)?(a):(b))
#endif // MAX

// In addition, MKL comes with an additional function axpby that is not present
// in standard blas. We will simply use a two-step (inefficient, of course) way
// to mimic that.
CC_INLINE void blas_saxpby_inc(int N, float alpha, const float* X, int incX, float beta, float* Y, int incY) {
  BLAS(saxpby)(N, alpha, X, incX, beta, Y, incY);
}

// the branchless, type-safe version from
// http://stackoverflow.com/questions/1903954/is-there-a-standard-sign-function-signum-sgn-in-c-c
CC_INLINE int ssign(float val) {
  return ((float)(0) < val) - (val < (float)(0));
}
static void blas_ssign(int n, const float* x, float* y) {
  int i = 0;for (; i < n; ++i) {
    float val = x[i];
    y[i] = ((0) < val) - (val < (0));
  }
}

#if (defined _WIN32 || defined _WIN64) && (_MSC_VER == 1200)
#endif
static float ssignbit(float x) {
  ASSERT(0);
  return x;
}

static void blas_ssignbit(int n, const float* x, float* y) {
  int i = 0;for (; i < n; ++i) {
    y[i] = ssignbit(x[i]);
  }
}
// y += a * x
static void blas_saxpy(int N, float alpha, const float* X, float* Y) {
  BLAS(saxpy)(N, alpha, X, 1, Y, 1);
}
static void blas_scopy(int count, const float* src, float* dst) {
  memcpy(dst, src, count * sizeof(float));
}
static void blas_dcopy(int count, const double* src, double* dst) {
  memcpy(dst, src, count * sizeof(double));
}
static void blas_saxpy_sign(int N, float alpha, const float* X, float* Y) {
  int i = 0;
  float map_alpha_[] = {-alpha, 0, alpha};
  float* map_alpha = map_alpha_+1;
  for (i=0; i<N; ++i) {
    int sign = ssign(X[i]);
    Y[i] += map_alpha[sign];
  }
}
static void blas_daxpy(int N, const double alpha, const double* X, double* Y) {
  BLAS(daxpy)(N, alpha, X, 1, Y, 1);
}
static void blas_iset(int N, int alpha, int* Y) {
  int i = 0;
  if (alpha == 0) {
    memset(Y, 0, sizeof(int) * N);  // NOLINT(caffe/alt_fn)
    return;
  }
  for (; i < N; ++i) {
    Y[i] = alpha;
  }
}
static void blas_sadd_scalar(int N, float alpha, float* Y) {
  int i = 0;for (; i < N; ++i) {
    Y[i] += alpha;
  }
}
static void blas_dadd_scalar(int N, const double alpha, double* Y) {
  int i = 0;for (; i < N; ++i) {
    Y[i] += alpha;
  }
}
static void blas_sscal(int N, float alpha, float *X) {
  BLAS(sscal)(N, alpha, X, 1);
}
static void blas_dscal(int N, const double alpha, double *X) {
  BLAS(dscal)(N, alpha, X, 1);
}
static void blas_saxpby(int N, float alpha, const float* X, float beta, float* Y) {
  BLAS(saxpby)(N, alpha, X, 1, beta, Y, 1);
}
static void blas_daxpby(int N, const double alpha, const double* X,
                             const double beta, double* Y) {
  BLAS(daxpby)(N, alpha, X, 1, beta, Y, 1);
}
static double blas_seuclidean(int n, const float* a, const float* b) {
  return MYBLAS(seuclidean)(n, a, b);
}
static double blas_deuclidean(int n, const double* a, const double* b) {
  return MYBLAS(deuclidean)(n, a, b);
}
static void blas_sgemm1(const CBLAS_TRANSPOSE TransA,
    const CBLAS_TRANSPOSE TransB, int M, int N, int K,
    float alpha, const float* A, const float* B, float beta,
    float* C) {
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  BLAS(sgemm)(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, N);
  return ;
}
// C = alpha*op( A )*op( B ) + beta*C
// M，矩阵A的行，矩阵C的行
// N，矩阵B的列，矩阵C的列
// K，矩阵A的列，矩阵B的行
static void blas_sgemm(const CBLAS_TRANSPOSE TransA, const CBLAS_TRANSPOSE TransB, int M, int N, int K,
                       float alpha, const float* A, const float* B, float beta, float* C) {
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
#if 0
  float* C1 = NULL;
  MYREALLOC(C1, M*N);
  MEMCPY(C1, C, M*N);
  BLAS(sgemm)(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, N);
  //my_sgemm
  MYBLAS(sgemm)(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C1, N);
  float err = blas_seuclidean(M*N, C, C1);
  if (err>0.001) {
    int adf=0;
  }
  FREE(C1);
#else
  BLAS(sgemm)(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, N);
#endif
}
static void blas_dgemm(const CBLAS_TRANSPOSE TransA, const CBLAS_TRANSPOSE TransB, int M, int N, int K,
                       const double alpha, const double* A, const double* B, const double beta, double* C) {
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  BLAS(dgemm)(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, N);
}
// y = alpha * A * x + beta * y
static void blas_sgemv(const CBLAS_TRANSPOSE TransA, int M, int N, float alpha,
                      const float* A, const float* x, float beta, float* y) {
  BLAS(sgemv)(CblasRowMajor, TransA, M, N, alpha, A, N, x, 1, beta, y, 1);
}
static void blas_dgemv(const CBLAS_TRANSPOSE TransA, int M, int N, const double alpha,
                      const double* A, const double* x, const double beta, double* y) {
  BLAS(dgemv)(CblasRowMajor, TransA, M, N, alpha, A, N, x, 1, beta, y, 1);
}

static float blas_sstrided_dot(int n, const float* x, int incx, const float* y, int incy) {
  return BLAS(sdot)(n, x, incx, y, incy);
}
static double blas_dstrided_dot(int n, const double* x, int incx, const double* y, int incy) {
  return BLAS(ddot)(n, x, incx, y, incy);
}
static float blas_sdot(int n, const float* x, const float* y) {
  return blas_sstrided_dot(n, x, 1, y, 1);
}
static float blas_sasum(int n, const float* x) {
  return BLAS(sasum)(n, x, 1);
}
static double blas_dasum(int n, const double* x) {
  return BLAS(dasum)(n, x, 1);
}
static float blas_ssumsq(int n, const float* x) {
  return blas_sdot(n, x, x);
}
// y = alpha * x;
static void blas_sscale(int n, float alpha, const float *x, float* y) {
  memcpy(y, x, n*sizeof(*x));
  BLAS(sscal)(n, alpha, y, 1);
}
static void blas_dscale(int n, const double alpha, const double *x, double* y) {
  memcpy(y, x, n*sizeof(*x));
  BLAS(dscal)(n, alpha, y, 1);
}
// y[i]= max(a*x[i], b*y[i])
static void blas_seltwise_max(int N, const float alpha, const float* x, const float beta, float* y) {
  int i = 0;for (; i < N; ++i) {
    float a = alpha * x[i], b = beta * y[i];
    y[i] = MAX(a, b);
  }
}

// y[i]= min(a*x[i], b*y[i])
static void blas_seltwise_min(int N, const float alpha, const float* x,
                           const float beta, float* y) {
  int i = 0;for (; i < N; ++i) {
    float a = alpha * x[i], b = beta * y[i];
    y[i] = MIN(a, b);
  }
}

static void blas_sclamp(int n, float lower_bound, float upper_bound, const float* x, float* y) {
  int i = 0;for (; i < n; ++i) {
    y[i] = MIN(MAX(x[i], lower_bound), upper_bound);
  }
}

static void blas_slinspace(int n, float lower_bound, float upper_bound, float* y) {
  float s = (upper_bound - lower_bound)/(n-1);
  int i = 0;for (; i < n; ++i) {
    y[i] = lower_bound + s * i;
  }
}

static int blas_sargmax(int n, const float* a)// 返回向量最大数的序号
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

static float* sim2col1(const float* data_im, float* data_col, DataSize inputSize,
    DataSize kernelSize, DataSize strideSize, IRECT pad, DataSize dilationSize) {
  int output_h = (inputSize.h + (pad.t + pad.b) - (dilationSize.h * (kernelSize.h - 1) + 1)) / strideSize.h + 1;
  int output_w = (inputSize.w + (pad.l + pad.r) - (dilationSize.w * (kernelSize.w - 1) + 1)) / strideSize.w + 1;
  int channel_size = inputSize.h * inputSize.w;
  float* data_col0 = data_col;
  int channel, kernel_row, kernel_col, output_rows, output_cols, output_col;
  for (channel = inputSize.c; channel--; data_im += channel_size) {
    for (kernel_row = 0; kernel_row < kernelSize.h; kernel_row++) {
      for (kernel_col = 0; kernel_col < kernelSize.w; kernel_col++) {
        int input_row = -pad.t + kernel_row * dilationSize.h;
        for (output_rows = output_h; output_rows; output_rows--) {
          if (!is_a_ge_zero_and_a_lt_b(input_row, inputSize.h)) {
            for (output_cols = output_w; output_cols; output_cols--) {
              *data_col++ = 0;
            }
          } else {
            int input_col = -pad.l + kernel_col * dilationSize.w;
            for (output_col = output_w; output_col; output_col--) {
              if (is_a_ge_zero_and_a_lt_b(input_col, inputSize.w)) {
                *(data_col++) = data_im[input_row * inputSize.w + input_col];
              } else {
                *(data_col++) = 0;
              }
              input_col += strideSize.w;
            }
          }
          input_row += strideSize.h;
        }
      }
    }
  }
  return data_col0;
}

static float* sim2col(const float* data_im, float* data_col, DataSize inputSize,
    DataSize kernelSize, DataSize strideSize, IRECT pad, DataSize dilationSize) {
  int output_h = (inputSize.h + (pad.t + pad.b) - (dilationSize.h * (kernelSize.h - 1) + 1)) / strideSize.h + 1;
  int output_w = (inputSize.w + (pad.l + pad.r) - (dilationSize.w * (kernelSize.w - 1) + 1)) / strideSize.w + 1;
  int channel_size = inputSize.h * inputSize.w;
  float* data_col0 = data_col;
  int channel, kernel_row, kernel_col, output_rows;
  for (channel = inputSize.c; channel--; data_im += channel_size) {
    for (kernel_row = 0; kernel_row < kernelSize.h; kernel_row++) {
      for (kernel_col = 0; kernel_col < kernelSize.w; kernel_col++) {
        int input_row = -pad.t + kernel_row * dilationSize.h;
        for (output_rows = output_h; output_rows; output_rows--) {
          if (!is_a_ge_zero_and_a_lt_b(input_row, inputSize.h)) {
            int output_col=0;
            for (; output_col<output_w; ++output_col) {
              data_col[output_col] = 0;
            }
          } else {
            int input_col = -pad.l + kernel_col * dilationSize.w;
            int output_col=0;
            const float * data_im_row;
            for (; input_col<0; ++output_col, input_col += strideSize.w) {
              data_col[output_col] = 0;
            }
            data_im_row = data_im+input_row * inputSize.w;
            if (1==strideSize.w) {
              int k = inputSize.w-input_col;
              memcpy(data_col+output_col, data_im_row+input_col, k*sizeof(*data_im));
              output_col += k;
              input_col += k;
            } else {
              for (; input_col<inputSize.w; ++output_col, input_col += strideSize.w) {
                data_col[output_col] = data_im_row[input_col];
              }
            }
            for (; output_col<output_w; ++output_col) {
              data_col[output_col] = 0;
            }
          }
          data_col += output_w;
          input_row += strideSize.h;
        }
      }
    }
  }
  return data_col0;
}
CC_INLINE void sim2col_nd_core(const float* data_input, const bool im2col,
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


static void sim2col_nd(const float* data_im, int num_spatial_axes,
    const int* im_shape, const int* col_shape,
    const int* kernel_shape, const int* pad, const int* stride,
    const int* dilation, float* data_col) {
  const bool kIm2Col = true;
  sim2col_nd_core(data_im, kIm2Col, num_spatial_axes, im_shape, col_shape,
                  kernel_shape, pad, stride, dilation, data_col);
}

static float* scol2im(const float* data_col, float* data_im, DataSize inputSize,
    DataSize kernelSize, DataSize strideSize, IRECT pad, DataSize dilationSize) {
  int output_h = (inputSize.h + (pad.t+pad.b) - (dilationSize.h * (kernelSize.h - 1) + 1)) / strideSize.h + 1;
  int output_w = (inputSize.w + (pad.l+pad.r) - (dilationSize.w * (kernelSize.w - 1) + 1)) / strideSize.w + 1;
  int channel_size = inputSize.h * inputSize.w;
  int channel, kernel_row, kernel_col, output_rows;
  float* data_im0 = data_im;
  blas_sset(Data_3DSize(inputSize), (0), data_im);
  for (channel = inputSize.c; channel--; data_im += channel_size) {
    for (kernel_row = 0; kernel_row < kernelSize.h; kernel_row++) {
      for (kernel_col = 0; kernel_col < kernelSize.w; kernel_col++) {
        int input_row = -pad.t + kernel_row * dilationSize.h;
        for (output_rows = output_h; output_rows; output_rows--) {
          if (!is_a_ge_zero_and_a_lt_b(input_row, inputSize.h)) {
            data_col += output_w;
          } else {
            int output_col = 0;
            int input_col = -pad.l + kernel_col * dilationSize.w;
#if 0
            float* data_im_row = data_im + input_row * inputSize.w;
            for (; input_col<0; ++output_col, input_col += strideSize.w) {
            }
            if (1==strideSize.w) {
              blas_saxpy(inputSize.w-input_col, 1, data_col+output_col, data_im_row+input_col);
            } else {
              for (; input_col < inputSize.w; ++output_col, input_col += strideSize.w) {
                data_im_row[input_col] += data_col[output_col];
              }
            }
            for (; output_col < output_w; ++output_col, input_col += strideSize.w) {
            }
            data_col+=output_w;
#else
            for (output_col = output_w; output_col; output_col--) {
              if (is_a_ge_zero_and_a_lt_b(input_col, inputSize.w)) {
                data_im[input_row * inputSize.w + input_col] += *data_col;
              }
              data_col++;
              input_col += strideSize.w;
            }
#endif
          }
          input_row += strideSize.h;
        }
      }
    }
  }
  return data_im0;
}

static void scol2im_nd(const float* data_col, int num_spatial_axes,
    const int* im_shape, const int* col_shape,
    const int* kernel_shape, const int* pad, const int* stride,
    const int* dilation, float* data_im) {
  const bool kIm2Col = false;
  sim2col_nd_core(data_col, kIm2Col, num_spatial_axes, im_shape, col_shape,
                     kernel_shape, pad, stride, dilation, data_im);
}
static void ssubstract_mean_normalize(int c, int h, int w, float* data, int cstep, const float* mean_vals, const float* norm_vals)
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

enum CBorderTypes {
	CC_BORDER_CONSTANT = 0, //!< `iiiiii|abcdefgh|iiiiiii`  with some specified `i`
	CC_BORDER_REPLICATE = 1, //!< `aaaaaa|abcdefgh|hhhhhhh`
	CC_BORDER_REFLECT = 2, //!< `fedcba|abcdefgh|hgfedcb`
	CC_BORDER_WRAP = 3, //!< `cdefgh|abcdefgh|abcdefg`
	CC_BORDER_REFLECT_101 = 4, //!< `gfedcb|abcdefgh|gfedcba`
	CC_BORDER_TRANSPARENT = 5, //!< `uvwxyz|absdefgh|ijklmno`

	CC_BORDER_REFLECT101 = CC_BORDER_REFLECT_101, //!< same as BORDER_REFLECT_101
	CC_BORDER_DEFAULT = CC_BORDER_REFLECT_101, //!< same as BORDER_REFLECT_101
	CC_BORDER_ISOLATED = 16 //!< do not look outside of ROI
};


// mat process
static void copy_make_border_image(const float* src, float* dst, int src_w, int src_h, int dst_w, int dst_h, int top, int left, enum CBorderTypes type, float v)
{
  const float* ptr = (src);
  float* outptr = (dst);

  if (type == CC_BORDER_CONSTANT) {
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
  else if (type == CC_BORDER_REPLICATE) {
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

static float* resize_bilinear_image(const float* srcData, float* dstData, DataSize srcSize, DataSize dstSize)
{
  int src_w = srcSize.w, src_h = srcSize.h, dst_w = dstSize.w, dst_h = dstSize.h;
  int i, n = srcSize.n*srcSize.c;
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

  for (i=0; i<n; ++i) {
    const float* src = srcData + i*Data_2DSize(srcSize);
    float* dst = dstData + i*Data_2DSize(dstSize);
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
  }

  FREE(_rowsbuf);
  FREE(buf);
  return dstData;
}

static float moving_average(float avg, int acc_number, float value)
{
  avg -= avg / acc_number;
  avg += value / acc_number;
  return avg;
}
static void mul(const float* a, const float* b, float* c, int len)
{
  int i = 0;
  for (; i < len; i++) {
    c[i] = a[i] * b[i];
  }
}
static void mul_inplace(float* a, const float* b, int len)
{
  int i = 0;
  for (; i < len; i++) {
    a[i] *= b[i];
  }
}
//a /= b
static void div_inplace(float* a, const float b, int len)
{
  int i = 0;
  float inv_b = 1./b;
  for (; i < len; i++) {
    a[i] *= inv_b;
  }
}
//f(x)=1/(1+e^(-x))
static void blas_ssigmoid(const float* X, float* y, int len)
{
  int i = 0;
  for (; i < len; i++) {
    float x = X[i];
    y[i] = 1.0f / (1.0f + expf(-1.0f * x));
  }
}
//f'(x) = x(1-x)
static void df_sigmoid(const float* X, float* y, int len)
{
  int i = 0;
  for (; i < len; i++) {
    float x = X[i];
    y[i] = x * (1.0f - x);
  }
}
//f(x)=(e^x-e^(-x))/(e^x+e^(-x))
static void blas_stanh(const float* X, float* Y, int len)
{
  int i = 0;
  for (; i < len; i++) {
    const float x = X[i];
    const float ex = expf(x);
    const float efx = expf(-x);
    Y[i] = (ex - efx) / (ex + efx);
  }
}
//f'(x)=1-x^(1/2)
static void df_tanh(const float* X, float* y, int len)
{
  int i = 0;
  for (; i < len; i++) {
    y[i] = 1.0f - sqrtf(X[i]);
  }
}
//
static float get_base(float x)
{
  int i = 0;
  for (; i >= -10; i--) {
    float base = powf(10.0f, (float)i) + 0.000001f;
    if (x > base) {
      return base;
    }
  }
  return 0.0f;
}

static void minmax_norm_inplace(float* a, int len)
{
  int i = 0;
  float _min, _max, b, inv_b;
  MYBLAS(sminmax)(len, a, &_min, &_max);
  b = MAX(fabs(_min), fabs(_max));
  inv_b = 0.1/(b+0.00001);
  for (; i < len; i++) {
    a[i] *= inv_b;
  }
}

