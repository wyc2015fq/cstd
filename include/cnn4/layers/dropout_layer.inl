
void FUN(dropout_forward)(const int n, const Dtype* in, unsigned int* mask, const Stype dropout_ratio, const Stype scale, Dtype* out) {
  Dtype scale_ = scale;
  cpu_caffe_rng_bernoulli_u(n, 1. - dropout_ratio, mask);
  for (int i = 0; i < n; ++i) {
    out[i] = in[i] * mask[i] * scale_;
  }
}

void FUN(dropout_backward)(const int n, const Dtype* in_diff, const unsigned int* mask, const Stype scale, Dtype* out_diff) {
  Dtype scale_ = scale;
  for (int i = 0; i < n; ++i) {
    out_diff[i] = in_diff[i] * mask[i] * scale_;
  }
}
