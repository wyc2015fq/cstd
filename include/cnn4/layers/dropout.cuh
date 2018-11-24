
__global__ void FUN(DropoutForward)(const int n, const Dtype* in, unsigned int* mask, const unsigned int threshold, const Dtype scale,
    Dtype* out) {
  CUDA_KERNEL_LOOP(index, n) {
    mask[index] = mask[index] > threshold;
    out[index] = in[index] * mask[index] * scale;
  }
}
void FUN(dropout_forward)(const int n, const Dtype* in, unsigned int* mask, const Stype dropout_ratio, const Stype scale, Dtype* out) {
  unsigned int uint_thres = (unsigned int)(UINT_MAX * dropout_ratio);
  FUN(caffe_rng_uniform_i)(n, mask);
  FUN(DropoutForward) << <CAFFE_GET_BLOCKS(n), CAFFE_CUDA_NUM_THREADS >> >( n, in, mask, uint_thres, scale, out);
}

__global__ void FUN(DropoutBackward)(const int n, const Dtype* in_diff, const unsigned int* mask, const Dtype scale,
    Dtype* out_diff) {
  CUDA_KERNEL_LOOP(index, n) {
    out_diff[index] = in_diff[index] * scale * mask[index];
  }
}

void FUN(dropout_backward)(const int n, const Dtype* in_diff, const unsigned int* mask, const Stype scale, Dtype* out_diff) {
  FUN(DropoutBackward) << <CAFFE_GET_BLOCKS(n), CAFFE_CUDA_NUM_THREADS >> >( n, in_diff, mask, scale, out_diff);
}
