
class DeconvolutionLayer : public BaseConvolutionLayer {
 public:
};





void DeconvolutionLayer::compute_output_shape() {
  const int* kernel_shape_data = this->kernel_shape_.cpu_data();
  const int* stride_data = this->stride_.cpu_data();
  const int* pad_data = this->pad_.cpu_data();
  const int* dilation_data = this->dilation_.cpu_data();
  this->output_shape_.clear();
  int shape_offset_size = this->layer_param_.convolution_param().shape_offset_size();
  if (shape_offset_size > 0) {
    CHECK_EQ(shape_offset_size, this->num_spatial_axes_);
  }
  for (i = 0; i < this->num_spatial_axes_; ++i) {
    // i + 1 to skip channel axis
    const int input_dim = this->input_shape(i + 1);
    const int kernel_extent = dilation_data[i] * (kernel_shape_data[i] - 1) + 1;
    const int output_dim = shape_offset_size == 0 ? stride_data[i] * (input_dim - 1)
      + kernel_extent - 2 * pad_data[i] : stride_data[i] * (input_dim - 1)
      + kernel_extent - 2 * pad_data[i] + this->layer_param_.convolution_param().
      shape_offset(i);
    this->output_shape_.push_back(output_dim);
  }
}


void DeconvolutionLayer::Forward(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  const Dtype* weight = this->blobs_[0]->cpu_data();
  for (i = 0; i < layer->i_size; ++i) {
    const Dtype* prevData = IBLOB(i)->cpu_data();
    Dtype* nextData = top[i]->mutable_data();
    for (n = 0; n < this->num_; ++n) {
      this->backward_gemm(prevData + n * this->bottom_dim_, weight,
          nextData + n * this->top_dim_);
      if (this->bias_term_) {
        const Dtype* bias = this->blobs_[1]->cpu_data();
        this->forward_bias(nextData + n * this->top_dim_, bias);
      }
    }
  }
}


void DeconvolutionLayer::Backward(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  const Dtype* weight = this->blobs_[0]->cpu_data();
  Dtype* weight_diff = this->blobs_[0]->mutable_diff();
  for (i = 0; i < layer->o_size; ++i) {
    const Dtype* nextDiff = top[i]->cpu_diff();
    const Dtype* prevData = IBLOB(i)->cpu_data();
    Dtype* prevDiff = IBLOB(i)->mutable_diff();
    // Bias gradient, if necessary.
    if (this->bias_term_ && this->param_propagate_down_[1]) {
      Dtype* bias_diff = this->blobs_[1]->mutable_diff();
      for (n = 0; n < this->num_; ++n) {
        this->backward_bias(bias_diff, nextDiff + n * this->top_dim_);
      }
    }
    if (this->param_propagate_down_[0] || propagate_down[i]) {
      for (n = 0; n < this->num_; ++n) {
        // Gradient w.r.t. weight. Note that we will accumulate diffs.
        if (this->param_propagate_down_[0]) {
          this->weight_gemm(nextDiff + n * this->top_dim_,
              prevData + n * this->bottom_dim_, weight_diff);
        }
        // Gradient w.r.t. bottom data, if necessary, reusing the column buffer
        // we might have just computed above.
        if (propagate_down[i]) {
          this->forward_gemm(nextDiff + n * this->top_dim_, weight,
              prevDiff + n * this->bottom_dim_,
              this->param_propagate_down_[0]);
        }
      }
    }
  }
}

#ifdef CPU_ONLY
STUB_GPU(DeconvolutionLayer);
#endif

INSTANTIATE_CLASS(DeconvolutionLayer);
REGISTER_LAYER_CLASS(Deconvolution);


