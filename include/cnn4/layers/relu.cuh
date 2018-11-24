
__global__ void FUN(ReLUForward)(const int n, const Dtype* in, Dtype* out,
  Stype negative_slope) {
  CUDA_KERNEL_LOOP(index, n) {
    out[index] = in[index] > 0 ? in[index] : in[index] * negative_slope;
  }
}

void FUN(relu_forward)(const int n, const Dtype* in, Dtype* out,
  Stype negative_slope) {
  FUN(ReLUForward) << <CAFFE_GET_BLOCKS(n), CAFFE_CUDA_NUM_THREADS >> >(n, in, out, negative_slope);
}

__global__ void FUN(ReLUBackward)(const int n, const Dtype* in_diff,
  const Dtype* in_data, Dtype* out_diff, Dtype negative_slope) {
  CUDA_KERNEL_LOOP(index, n) {
    out_diff[index] = in_diff[index] * ((in_data[index] > 0)
      + (in_data[index] <= 0) * negative_slope);
  }
}

void FUN(relu_backward)(const int n, const Dtype* in_diff, const Dtype* in_data, Dtype* out_diff, Stype negative_slope) {
  FUN(ReLUBackward) << <CAFFE_GET_BLOCKS(n), CAFFE_CUDA_NUM_THREADS >> >(n, in_diff, in_data, out_diff, negative_slope);
}
