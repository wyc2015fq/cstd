
__global__ void FUN(ScaleForward)(const int n, const Dtype* in,
    const Dtype* scale, const int scale_dim, const int inner_dim,
    Dtype* out) {
  CUDA_KERNEL_LOOP(index, n) {
    const int scale_index = (index / inner_dim) % scale_dim;
    out[index] = in[index] * scale[scale_index];
  }
}

void FUN(scale_forward)(const int n, const Dtype* in, const Dtype* scale, const int scale_dim, const int inner_dim, Dtype* out) {
  FUN(ScaleForward) << <CAFFE_GET_BLOCKS(n), CAFFE_CUDA_NUM_THREADS >> >( n, in, scale, scale_dim, inner_dim, out);
}
__global__ void FUN(ScaleBiasForward)(const int n, const Dtype* in,
    const Dtype* scale, const Dtype* bias,
    const int scale_dim, const int inner_dim, Dtype* out) {
  CUDA_KERNEL_LOOP(index, n) {
    const int scale_index = (index / inner_dim) % scale_dim;
    out[index] = in[index] * scale[scale_index] + bias[scale_index];
  }
}
void FUN(scalebias_forward)(const int n, const Dtype* in,const Dtype* scale, const Dtype* bias, const int scale_dim, const int inner_dim, Dtype* out) {
  FUN(ScaleBiasForward) << <CAFFE_GET_BLOCKS(n), CAFFE_CUDA_NUM_THREADS >> >(n, in, scale, bias, scale_dim, inner_dim, out);
}
