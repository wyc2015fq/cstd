

template <typename Dtype>
void softmax_forward(_CONTEXT, int count, int channels, int outer_num_, int inner_num_, const Dtype* bottom_data, Dtype* top_data, Dtype* scale_data) {
  int dim = count / outer_num_;
  BufData<Dtype, Context> sum_multiplier_(channels, Dtype(1));
  caffe_copy(context, count, bottom_data, top_data);
  // We need to subtract the max to avoid numerical issues, compute the exp,
  // and then normalize.
  for (int i = 0; i < outer_num_; ++i) {
    // initialize scale_data to the first plane
    caffe_copy(context, inner_num_, bottom_data + i * dim, scale_data);
    for (int j = 0; j < channels; j++) {
      for (int k = 0; k < inner_num_; k++) {
        scale_data[k] = std::max(scale_data[k],
          bottom_data[i * dim + j * inner_num_ + k]);
      }
    }
    // subtraction
    caffe_gemm<Dtype>(context, CblasNoTrans, CblasNoTrans, channels, inner_num_,
      1, -1., sum_multiplier_.get(), scale_data, 1., top_data);
    // exponentiation
    caffe_exp<Dtype>(context, dim, top_data, top_data);
    // sum after exp
    caffe_gemv<Dtype>(context, CblasTrans, channels, inner_num_, 1.,
      top_data, sum_multiplier_.get(), 0., scale_data);
    // division
    for (int j = 0; j < channels; j++) {
      caffe_div(context, inner_num_, top_data, scale_data, top_data);
      top_data += inner_num_;
    }
  }
}

template <typename Dtype>
void softmax_backward(_CONTEXT, int count, int channels, int outer_num_, int inner_num_, const Dtype* top_diff, const Dtype* top_data, Dtype* bottom_diff, Dtype* scale_data) {
  int dim = count / outer_num_;
  BufData<Dtype, Context> sum_multiplier_(channels, Dtype(1));
  caffe_copy(context, count, top_diff, bottom_diff);
  for (int i = 0; i < outer_num_; ++i) {
    // compute dot(top_diff, top_data) and subtract them from the bottom diff
    for (int k = 0; k < inner_num_; ++k) {
      scale_data[k] = caffe_strided_dot<Dtype>(context, channels,
        bottom_diff + i * dim + k, inner_num_,
        top_data + i * dim + k, inner_num_);
    }
    // subtraction
    caffe_gemm<Dtype>(context, CblasNoTrans, CblasNoTrans, channels, inner_num_, 1,
      -1., sum_multiplier_.get(), scale_data, 1., bottom_diff + i * dim);
  }
  // elementwise multiplication
  caffe_mul(context, count, bottom_diff, top_data, bottom_diff);
}
