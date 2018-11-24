

void FUN(relu_forward)(const int n, const Dtype* in, Dtype* out, Stype negative_slope) {
  CPU_KERNEL_LOOP(index, n) {
    out[index] = in[index] > 0 ? in[index] : in[index] * negative_slope;
  }
}


void FUN(relu_backward)(const int n, const Dtype* in_diff, const Dtype* in_data, Dtype* out_diff, Stype negative_slope) {
  CPU_KERNEL_LOOP(index, n) {
    out_diff[index] = in_diff[index] * ((in_data[index] > 0)
      + (in_data[index] <= 0) * negative_slope);
  }
}
