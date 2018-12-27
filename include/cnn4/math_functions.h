
#ifndef _MATH_FUNCTIONS_H_
#define _MATH_FUNCTIONS_H_

#include "config.h"
#include "myblas/myblas.h"
#include "types.h"
#include "cpu.hpp"
struct cjson;


//struct CPUContext;
//struct GPUContext;
#define BN_MIN_EPSILON  (1e-5)

#define _MATH_FUNCTIONS_TYPE_CPU_GPU_DEF(DEF, Dtype, Stype)  \
DEF(const Dtype*, get_ones, (int n)) \
DEF(const Dtype*, get_one, ()) \
DEF(const Dtype*, get_zero, ()) \
DEF(void, caffe_copy, (const int N, const Dtype* X, Dtype* Y)) \
DEF(void, caffe_gemm, (const CBLAS_TRANSPOSE TransA, const CBLAS_TRANSPOSE TransB, const int M, const int N, const int K, \
const Stype alpha, const Dtype* A, const Dtype* B, const Stype beta, Dtype* C)) \
DEF(void, caffe_gemv, (const CBLAS_TRANSPOSE TransA, const int M, const int N, const Stype alpha, const Dtype* A, const Dtype* x, const Stype beta,Dtype* y)) \
DEF(void, caffe_axpy, (const int N, const Stype alpha, const Dtype* X,Dtype* Y)) \
DEF(void, caffe_axpby, (const int N, const Stype alpha, const Dtype* X,const Stype beta, Dtype* Y)) \
DEF(void, caffe_set, (const int N, const Stype alpha, Dtype* X)) \
DEF(void, caffe_add_scalar, (const int N, const Stype alpha, Dtype* X)) \
DEF(void, caffe_scal, (const int N, const Stype alpha, Dtype* X)) \
DEF(void, caffe_sqr, (const int N, const Dtype* a, Dtype* y)) \
DEF(void, caffe_add, (const int N, const Dtype* a, const Dtype* b, Dtype* y)) \
DEF(void, caffe_sub, (const int N, const Dtype* a, const Dtype* b, Dtype* y)) \
DEF(void, caffe_mul, (const int N, const Dtype* a, const Dtype* b, Dtype* y)) \
DEF(void, caffe_div, (const int N, const Dtype* a, const Dtype* b, Dtype* y)) \
DEF(void, caffe_abs, (const int n, const Dtype* a, Dtype* y)) \
DEF(void, caffe_exp, (const int n, const Dtype* a, Dtype* y)) \
DEF(void, caffe_log, (const int n, const Dtype* a, Dtype* y)) \
DEF(void, caffe_powx, (const int n, const Dtype* a, const Stype b, Dtype* y)) \
DEF(void, caffe_bound, (const int n, const Dtype* a, const Stype min, const Stype max, Dtype* y)) \
DEF(void, caffe_rng_uniform, (const int n, const Stype a, const Stype b, Dtype* r)) \
DEF(void, caffe_rng_gaussian, (const int n, const Stype mu, const Stype sigma, Dtype* r)) \
DEF(Stype, caffe_dot, (const int n, const Dtype* x, const Dtype* y)) \
DEF(Stype, caffe_strided_dot, (const int n, const Dtype* x, const int incx, const Dtype* y, const int incy)) \
DEF(Stype, caffe_asum, (const int n, const Dtype* x)) \
DEF(void, caffe_asum2, (const int n, const Dtype* x, Dtype* y)) \
DEF(void, caffe_sign, (const int n, const Dtype* x, Dtype* y)) \
DEF(void, caffe_sgnbit, (const int n, const Dtype* x, Dtype* y)) \
DEF(void, caffe_fabs, (const int n, const Dtype* x, Dtype* y)) \
DEF(void, caffe_scale, (const int n, const Stype alpha, const Dtype* x, Dtype* y)) \
DEF(void, sgd_update, (int N, Dtype* g, Dtype* h, Stype momentum, Stype local_rate)) \
DEF(void, adadelta_update, (int N, Dtype* g, Dtype* h, Dtype* h2, Stype momentum, Stype delta, Stype local_rate)) \
DEF(void, adagrad_update, (int N, Dtype* g, Dtype* h, Stype delta, Stype local_rate)) \
DEF(void, adam_update, (int N, Dtype* g, Dtype* m, Dtype* v, Stype beta1, Stype beta2, Stype eps_hat, Stype corrected_local_rate)) \
DEF(void, im2col, (const Dtype* data_im, const int channels,const int height, const int width, const int kernel_h, const int kernel_w,\
const int pad_h, const int pad_w, const int stride_h,const int stride_w, const int dilation_h, const int dilation_w,Dtype* data_col)) \
DEF(void, im2col_nd, (const Dtype* data_im, const int num_spatial_axes, const int num_kernels, const DataShape im_shape, const DataShape col_shape, \
const DataShape kernel_shape, const DataShape pad, const DataShape stride, const DataShape dilation, Dtype* data_col)) \
DEF(void, col2im, (const Dtype* data_col, const int channels, const int height, const int width, const int kernel_h, const int kernel_w, \
const int pad_h, const int pad_w, const int stride_h, const int stride_w, const int dilation_h, const int dilation_w, Dtype* data_im)) \
DEF(void, col2im_nd, (const Dtype* data_col, const int num_spatial_axes, const int im_size, const DataShape im_shape, const DataShape col_shape, \
const DataShape kernel_shape, const DataShape pad, const DataShape stride, const DataShape dilation, Dtype* data_im)) \
DEF(void, relu_forward, (const int n, const Dtype* in, Dtype* out, Stype negative_slope)) \
DEF(void, relu_backward, (const int n, const Dtype* in_diff, const Dtype* in_data, Dtype* out_diff, Stype negative_slope)) \
DEF(void, pooling_forward, (PoolMethod pool, Phase phase, const Dtype* bottom_data,const int num, const int channels,const int height, const int width, \
const int pooled_height, const int pooled_width, const int kernel_h, const int kernel_w, const int stride_h, const int stride_w, \
const int pad_h, const int pad_w, Dtype* rand_idx, Dtype* top_data, int* mask, Dtype* top_mask)) \
DEF(void, pooling_backward, (PoolMethod pool, const Dtype* const rand_idx, const Dtype* top_diff, const int* mask, const Dtype* top_mask, const int num, \
const int channels, const int height, const int width, const int pooled_height, const int pooled_width, const int kernel_h, const int kernel_w, \
const int stride_h, const int stride_w, const int pad_h, const int pad_w, Dtype* bottom_diff)) \
DEF(void, softmax_forward, (int count, int channels, int outer_num_, int inner_num_, \
const Dtype* bottom_data, Dtype* top_data, Dtype* scale_data)) \
DEF(void, softmax_backward, (int count, int channels, int outer_num_, int inner_num_, \
const Dtype* top_diff, const Dtype* top_data, Dtype* bottom_diff, Dtype* scale_data)) \
DEF(int, softmaxloss_forward, (const Dtype* prob_data, const Dtype* label, const int num, \
const int dim, const int spatial_dim, const bool has_ignore_label_, const int ignore_label_, Dtype* out_loss)) \
DEF(int, softmaxloss_backward, (const Dtype* top_data,const Dtype* label, Dtype* bottom_diff, \
const int outer_num_, const int dim,const int spatial_dim, const bool has_ignore_label_,const int ignore_label_)) \
DEF(void, scale_forward, (const int n, const Dtype* in, const Dtype* scale, const int scale_dim, const int inner_dim, Dtype* out)) \
DEF(void, scalebias_forward, (const int n, const Dtype* in,const Dtype* scale, const Dtype* bias, const int scale_dim, const int inner_dim, Dtype* out)) \
DEF(void, dropout_forward, (const int n, const Dtype* in, unsigned int* mask, const Stype dropout_ratio, const Stype scale, Dtype* out)) \
DEF(void, dropout_backward, (const int n, const Dtype* in_diff, const unsigned int* mask, const Stype scale, Dtype* out_diff)) \
DEF(void, transpose, (const int nthreads, const Dtype* from_data, Dtype* to_data, const DataShape from_counts, const DataShape to_counts, const DataShape map, const int num_axes)) \
DEF(void, BatchNormalizationForwardInference, (int num, int channels, int inner_num_, const Dtype *x, Dtype *y, const Dtype *bnScale, const Dtype *bnBias, const Dtype *estimatedMean, const Dtype *estimatedVariance, Stype epsilon)) \
DEF(void, BatchNormalizationForwardTraining, (int N, int C, int M, Dtype* X, Dtype* Y, const Dtype* scaler, const Dtype* bias, Stype factor, Dtype* runningMean, Dtype* runningVar, Stype epsilon, Dtype* batchMean, Dtype* batchVar)) \
//
#define _MATH_FUNCTIONS_TYPE_CPU_DEF(DEF, Dtype, Stype)  \
DEF(void, caffe_rng_bernoulli, (const int n, const Stype p, int* r)) \
DEF(void, caffe_rng_bernoulli_u, (const int n, const Stype p, unsigned int* r)) \
DEF(int, ConstantFiller, (DataShape shape, Dtype* data, const Stype value)) \
DEF(int, Filler, (DataShape shape, Dtype* data, cjson* param)) \
_MATH_FUNCTIONS_TYPE_CPU_GPU_DEF(DEF, Dtype, Stype)


#define _MATH_FUNCTIONS_MEM_CPU_GPU_DEF(DEF)  \
DEF(void*, caffe_malloc, (const int N)) \
DEF(void, caffe_free, (void* p)) \
DEF(void, caffe_memset, (const size_t N, const int alpha, void* X)) \
DEF(void, caffe_memcpy, (const size_t N, const void* X, void* Y))

//
#define _MATH_FUNCTIONS_TYPE_GPU_DEF(DEF, Dtype, Stype)  \
_MATH_FUNCTIONS_TYPE_CPU_GPU_DEF(DEF, Dtype, Stype)

//
#define DEF(RET, NAME, ARGS)  RET cpu_ ## NAME ARGS;
_MATH_FUNCTIONS_MEM_CPU_GPU_DEF(DEF)
_MATH_FUNCTIONS_TYPE_CPU_DEF(DEF, float, double)
#undef DEF

#ifndef CPU_ONLY
#define DEF(RET, NAME, ARGS)  RET gpu_ ## NAME ARGS;
_MATH_FUNCTIONS_MEM_CPU_GPU_DEF(DEF)
_MATH_FUNCTIONS_TYPE_CPU_DEF(DEF, float, double)
#undef DEF
#define DEF(RET, NAME, ARGS)  typedef RET (*NAME ## t) ARGS;static NAME ## t NAME = (NAME ## t)gpu_ ## NAME;
_MATH_FUNCTIONS_MEM_CPU_GPU_DEF(DEF)
_MATH_FUNCTIONS_TYPE_CPU_GPU_DEF(DEF, void, double)
#undef DEF
#else
#define DEF(RET, NAME, ARGS)  typedef RET (*NAME ## t) ARGS;static NAME ## t NAME = (NAME ## t)cpu_ ## NAME;
_MATH_FUNCTIONS_TYPE_CPU_GPU_DEF(DEF, void, double)
_MATH_FUNCTIONS_MEM_CPU_GPU_DEF(DEF)
#undef DEF
#endif

template <typename Dtype>
void icaffe_set(const int N, const Dtype alpha, Dtype* Y)
{
  if (alpha == 0) {
    memset(Y, 0, sizeof(Dtype) * N);  // NOLINT(caffe/alt_fn)
    return;
  }
  for (int i = 0; i < N; ++i) {
    Y[i] = alpha;
  }
}

template <typename Dtype>
struct BufData : public Buffer {
  ~BufData() { Free(this); }
  BufData(Brew brew, int count) {
    memset(this, 0, sizeof(*this));
    Buffer* ptr = this;
    ReAlloc(brew, ptr, count * sizeof(Dtype));
  }
  Dtype* get() {
    return (Dtype*)data;
  }
};
template <typename T>
void caffe_Malloc(T** p, int n) {
  *p = (T*)caffe_malloc(n);
}
template <typename T>
void caffe_Free(T** p) {
  caffe_free(*p);
  *p = NULL;
}
static void caffe_Memcpy(void* y, const void* x, int n) { caffe_memcpy(n, x, y); }
static void caffe_Memset(void* y, const int a, int n) { caffe_memset(n, a, y); }


#endif // _MATH_FUNCTIONS_H_
