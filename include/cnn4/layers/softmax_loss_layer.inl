
int FUN(softmaxloss_forward)(const Dtype* prob_data, const Dtype* label,
  const int outer_num_, const int dim, const int inner_num_,
  const bool has_ignore_label_, const int ignore_label_,
  Dtype* out_loss) {
  int count = -1;
  Dtype loss = 0;
  for (int i = 0; i < outer_num_; ++i) {
    for (int j = 0; j < inner_num_; j++) {
      const int label_value = static_cast<int>(label[i * inner_num_ + j]);
      if (has_ignore_label_ && label_value == ignore_label_) {
        continue;
      }
      DCHECK_GE(label_value, 0);
      //DCHECK_LT(label_value, prob_.shape(softmax_axis_));
      loss -= log(std::max(prob_data[i * dim + label_value * inner_num_ + j], Dtype(FLT_MIN)));
      ++count;
    }
  }
  *out_loss = loss;
   // top[0]->mdata()[0] = loss / get_normalizer(bottom[0]->shape_, axis_, normalization_, valid_count);
  return count;
}

int FUN(softmaxloss_backward)(const Dtype* top_data,
  const Dtype* label, Dtype* bottom_diff, const int outer_num_, const int dim,
  const int inner_num_, const bool has_ignore_label_,
  const int ignore_label_) {
  int count = 0;
  const int channels = dim / inner_num_;
  for (int i = 0; i < outer_num_; ++i) {
    for (int j = 0; j < inner_num_; ++j) {
      const int label_value = static_cast<int>(label[i * inner_num_ + j]);
      if (has_ignore_label_ && label_value == ignore_label_) {
        for (int c = 0; c < channels; ++c) {
          bottom_diff[i * dim + c * inner_num_ + j] = 0;
        }
      }
      else {
        bottom_diff[i * dim + label_value * inner_num_ + j] -= 1;
        ++count;
      }
    }
  }
  return count;
}