
void FUN(scale_forward)(const int n, const Dtype* in, const Dtype* scale, const int scale_dim, const int inner_dim, Dtype* out) {
  CPU_KERNEL_LOOP(index, n) {
    const int scale_index = (index / inner_dim) % scale_dim;
    out[index] = in[index] * scale[scale_index];
  }
}

void FUN(scalebias_forward)(const int n, const Dtype* in, const Dtype* scale, const Dtype* bias, const int scale_dim, const int inner_dim, Dtype* out) {
  CPU_KERNEL_LOOP(index, n) {
    const int scale_index = (index / inner_dim) % scale_dim;
    out[index] = in[index] * scale[scale_index] + bias[scale_index];
  }
}
