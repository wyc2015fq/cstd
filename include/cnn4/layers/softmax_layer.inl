

void FUN(softmax_forward)(int count, int channels, int outer_num_, int inner_num_, const Dtype* bottom_data, Dtype* top_data, Dtype* scale_data) {
  int dim = count / outer_num_;
  BufData<Dtype> sum_multiplier_buf(CPU, channels);
  Dtype* sum_multiplier_ = sum_multiplier_buf.get();
  icaffe_set<Dtype>(channels, Dtype(1), sum_multiplier_);
  FUN(caffe_copy)(count, bottom_data, top_data);
  // We need to subtract the max to avoid numerical issues, compute the exp,
  // and then normalize.
  for (int i = 0; i < outer_num_; ++i) {
    // initialize scale_data to the first plane
    caffe_copy(inner_num_, bottom_data + i * dim, scale_data);
    for (int j = 0; j < channels; j++) {
      for (int k = 0; k < inner_num_; k++) {
        scale_data[k] = std::max(scale_data[k],
          bottom_data[i * dim + j * inner_num_ + k]);
      }
    }
    // subtraction
    FUN(caffe_gemm)(CblasNoTrans, CblasNoTrans, channels, inner_num_,
      1, -1., sum_multiplier_, scale_data, 1., top_data);
    // exponentiation
    FUN(caffe_exp)(dim, top_data, top_data);
    // sum after exp
    FUN(caffe_gemv)(CblasTrans, channels, inner_num_, 1.,
      top_data, sum_multiplier_, 0., scale_data);
    // division
    for (int j = 0; j < channels; j++) {
      FUN(caffe_div)(inner_num_, top_data, scale_data, top_data);
      top_data += inner_num_;
    }
  }
}

void FUN(softmax_backward)(int count, int channels, int outer_num_, int inner_num_, const Dtype* top_diff, const Dtype* top_data, Dtype* bottom_diff, Dtype* scale_data) {
  int dim = count / outer_num_;
  BufData<Dtype> sum_multiplier_buf(CPU, channels);
  Dtype* sum_multiplier_ = sum_multiplier_buf.get();
  icaffe_set(channels, Dtype(1), sum_multiplier_);
  FUN(caffe_copy)(count, top_diff, bottom_diff);
  for (int i = 0; i < outer_num_; ++i) {
    // compute dot(top_diff, top_data) and subtract them from the bottom diff
    for (int k = 0; k < inner_num_; ++k) {
      scale_data[k] = FUN(caffe_strided_dot)(channels,
        bottom_diff + i * dim + k, inner_num_,
        top_data + i * dim + k, inner_num_);
    }
    // subtraction
    FUN(caffe_gemm)(CblasNoTrans, CblasNoTrans, channels, inner_num_, 1,
      -1., sum_multiplier_, scale_data, 1., bottom_diff + i * dim);
  }
  // elementwise multiplication
  FUN(caffe_mul)(count, bottom_diff, top_data, bottom_diff);
}
