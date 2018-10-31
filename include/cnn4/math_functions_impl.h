

void caffe_memset(_CONTEXT, const size_t N, const int alpha, void* X);

template <typename Dtype>
void caffe_copy(_CONTEXT, const int N, const Dtype* X, Dtype* Y);

// Decaf gpu gemm provides an interface that is almost the same as the cpu
// gemm function - following the c convention and calling the fortran-order
// gpu code under the hood.
template <typename Dtype>
void caffe_gemm(_CONTEXT,const CBLAS_TRANSPOSE TransA,
  const CBLAS_TRANSPOSE TransB, const int M, const int N, const int K,
  const Dtype alpha, const Dtype* A, const Dtype* B, const Dtype beta,
  Dtype* C);

template <typename Dtype>
void caffe_gemv(_CONTEXT,const CBLAS_TRANSPOSE TransA, const int M, const int N,
  const Dtype alpha, const Dtype* A, const Dtype* x, const Dtype beta,
  Dtype* y);

template <typename Dtype>
void caffe_axpy(_CONTEXT,const int N, const Dtype alpha, const Dtype* X,
  Dtype* Y);

template <typename Dtype>
void caffe_axpby(_CONTEXT,const int N, const Dtype alpha, const Dtype* X,
  const Dtype beta, Dtype* Y);

void caffe_memcpy(_CONTEXT,const size_t N, const void* X, void* Y);

template <typename Dtype>
void caffe_set(_CONTEXT,const int N, const Dtype alpha, Dtype* X);

template <typename Dtype>
void caffe_add_scalar(_CONTEXT,const int N, const Dtype alpha, Dtype* X);

template <typename Dtype>
void caffe_scal(_CONTEXT,const int N, const Dtype alpha, Dtype* X);

template <typename Dtype>
void caffe_sqr(_CONTEXT,const int N, const Dtype* a, Dtype* y);

template <typename Dtype>
void caffe_add(_CONTEXT,const int N, const Dtype* a, const Dtype* b, Dtype* y);

template <typename Dtype>
void caffe_sub(_CONTEXT,const int N, const Dtype* a, const Dtype* b, Dtype* y);

template <typename Dtype>
void caffe_mul(_CONTEXT,const int N, const Dtype* a, const Dtype* b, Dtype* y);

template <typename Dtype>
void caffe_div(_CONTEXT,const int N, const Dtype* a, const Dtype* b, Dtype* y);

template <typename Dtype>
void caffe_abs(_CONTEXT,const int n, const Dtype* a, Dtype* y);

template <typename Dtype>
void caffe_exp(_CONTEXT,const int n, const Dtype* a, Dtype* y);

template <typename Dtype>
void caffe_log(_CONTEXT,const int n, const Dtype* a, Dtype* y);

template <typename Dtype>
void caffe_powx(_CONTEXT,const int n, const Dtype* a, const Dtype b, Dtype* y);

template <typename Dtype>
void caffe_bound(_CONTEXT,const int n, const Dtype* a, const Dtype min, const Dtype max, Dtype* y);

// caffe_rng_uniform with two arguments generates integers in the range
// [0, UINT_MAX].
void caffe_rng_uniform(_CONTEXT,const int n, unsigned int* r);

// caffe_rng_uniform with four arguments generates floats in the range
// (a, b] (strictly greater than a, less than or equal to b) due to the
// specification of curandGenerateUniform.  With a = 0, b = 1, just calls
// curandGenerateUniform; with other limits will shift and scale the outputs
// appropriately after calling curandGenerateUniform.
template <typename Dtype>
void caffe_rng_uniform(_CONTEXT,const int n, const Dtype a, const Dtype b, Dtype* r);

template <typename Dtype>
void caffe_rng_gaussian(_CONTEXT,const int n, const Dtype mu, const Dtype sigma,
  Dtype* r);

template <typename Dtype>
void caffe_rng_bernoulli(_CONTEXT,const int n, const Dtype p, int* r);

template <typename Dtype>
Dtype caffe_dot(_CONTEXT, const int n, const Dtype* x, const Dtype* y);

template <typename Dtype>
void caffe_dot(_CONTEXT, const int n, const Dtype* x, const Dtype* y, Dtype* out);

template <typename Dtype>
Dtype caffe_strided_dot(_CONTEXT,const int n, const Dtype* x, const int incx, const Dtype* y, const int incy);

template <typename Dtype>
Dtype caffe_asum(_CONTEXT,const int n, const Dtype* x);

template <typename Dtype>
void caffe_asum(_CONTEXT,const int n, const Dtype* x, Dtype* y);

template<typename Dtype>
void caffe_sign(_CONTEXT,const int n, const Dtype* x, Dtype* y);

template<typename Dtype>
void caffe_sgnbit(_CONTEXT,const int n, const Dtype* x, Dtype* y);

template <typename Dtype>
void caffe_fabs(_CONTEXT,const int n, const Dtype* x, Dtype* y);

template <typename Dtype>
void caffe_scale(_CONTEXT,const int n, const Dtype alpha, const Dtype* x, Dtype* y);


template <typename Dtype>
void sgd_update(_CONTEXT, int N, Dtype* g, Dtype* h, Dtype momentum,
  Dtype local_rate);

template <typename Dtype>
void adadelta_update(_CONTEXT, int N, Dtype* g, Dtype* h, Dtype* h2, Dtype momentum,
  Dtype delta, Dtype local_rate);

template <typename Dtype>
void adagrad_update(_CONTEXT, int N, Dtype* g, Dtype* h, Dtype delta, Dtype local_rate);

template <typename Dtype>
void adam_update(_CONTEXT, int N, Dtype* g, Dtype* m, Dtype* v, Dtype beta1,
  Dtype beta2, Dtype eps_hat, Dtype corrected_local_rate);

template <typename Dtype>
void relu_forward(_CONTEXT,const int n, const Dtype* in, Dtype* out, Dtype negative_slope);

template <typename Dtype>
void relu_backward(_CONTEXT,const int n, const Dtype* in_diff, const Dtype* in_data, Dtype* out_diff, Dtype negative_slope);
