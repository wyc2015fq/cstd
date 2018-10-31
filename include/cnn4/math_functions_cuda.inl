
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
void sgd_update(_CONTEXT, int N, Dtype* g, Dtype* h, Dtype momentum, Dtype local_rate) {
  SGDUpdate<Dtype>  // NOLINT_NEXT_LINE(whitespace/operators)
    << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
      N, g, h, momentum, local_rate);
  CUDA_POST_KERNEL_CHECK;
}

template <>
void adadelta_update(_CONTEXT, int N, Dtype* g, Dtype* h, Dtype* h2, Dtype momentum,
  Dtype delta, Dtype local_rate) {
  AdaDeltaUpdate<Dtype>  // NOLINT_NEXT_LINE(whitespace/operators)
    << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
      N, g, h, h2, momentum, delta, local_rate);
  CUDA_POST_KERNEL_CHECK;
}
template <>
void adagrad_update(_CONTEXT, int N, Dtype* g, Dtype* h, Dtype delta, Dtype local_rate) {
  AdaGradUpdate<Dtype>  // NOLINT_NEXT_LINE(whitespace/operators)
    << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
      N, g, h, delta, local_rate);
  CUDA_POST_KERNEL_CHECK;
}

template <>
void adam_update(_CONTEXT, int N, Dtype* g, Dtype* m, Dtype* v, Dtype beta1,
  Dtype beta2, Dtype eps_hat, Dtype corrected_local_rate) {
  AdamUpdate<Dtype>  // NOLINT_NEXT_LINE(whitespace/operators)
    << <CAFFE_GET_BLOCKS(N), CAFFE_CUDA_NUM_THREADS >> >(
      N, g, m, v, beta1, beta2, eps_hat, corrected_local_rate);
  CUDA_POST_KERNEL_CHECK;
}

template <>
void relu_forward(_CONTEXT,const int n, const Dtype* in, Dtype* out,
  Dtype negative_slope) {
  ReLUForward<Dtype> << <CAFFE_GET_BLOCKS(n), CAFFE_CUDA_NUM_THREADS >> >( n, in, out, negative_slope);
}

template <>
void relu_backward(_CONTEXT, const int n, const Dtype* in_diff, const Dtype* in_data, Dtype* out_diff, Dtype negative_slope) {
  ReLUBackward<Dtype> << <CAFFE_GET_BLOCKS(n), CAFFE_CUDA_NUM_THREADS >> >( n, in_diff, in_data, out_diff, negative_slope);
}
