#ifndef CAFFE_BASE_CONVOLUTION_LAYER_HPP_
#define CAFFE_BASE_CONVOLUTION_LAYER_HPP_

DataShape cJSON_GetShape(cJSON* conv_param, int kDefaultPad, const char* name, const char* name_h = NULL, const char* name_w = NULL) {
  DataShape pad = { 0 };
  int* pad_data = pad.dim;
  char buf[64];
  cJSON* item = conv_param->get(name);
  if (!name_h) {
    _snprintf(buf, 64, "%s_h", name);
    name_h = buf;
  }
  cJSON* h_json = conv_param->get(name_h);
  if (!name_w) {
    _snprintf(buf, 64, "%s_w", name);
    name_w = buf;
  }
  cJSON* w_json = conv_param->get(name_w);
  if (h_json || w_json) {
    CHECK_EQ(0, item->size())
      << "Either pad or pad_h/w should be specified; not both.";
    pad_data[0] = h_json ? h_json->valueint : 0;
    pad_data[1] = w_json ? w_json->valueint : 0;
  }
  else {
    const int num_pad_dims = item ? item->size() : 0;
    for (int i = 0; i < 4; ++i) {
      pad_data[i] = (num_pad_dims == 0) ? kDefaultPad :
        item->getint((num_pad_dims == 1) ? 0 : i, kDefaultPad);
    }
  }
  return pad;
}


/**
 * @brief Abstract base class that factors out the BLAS code common to
 *        ConvolutionLayer and DeconvolutionLayer.
 */
template <typename Dtype>
struct BaseConvolutionLayer : public Layer<Dtype>
{
public:

  virtual inline int MinBottomBlobs() const { return 1; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline bool EqualNumBottomTopBlobs() const { return true; }

  /// @brief The spatial dimensions of the input.
  inline int input_shape(int i) {
    return (*bottom_shape_).dim[channel_axis_ + i];
  }
  // reverse_dimensions should return true iff we are implementing deconv, so
  // that conv helpers know which dimensions are which.
  virtual bool reverse_dimensions() = 0;
  // Compute height_out_ and width_out_ from other parameters.
  virtual void compute_output_shape() = 0;

  /// @brief The spatial dimensions of a filter kernel.
  DataShape kernel_shape_;
  /// @brief The spatial dimensions of the stride.
  DataShape stride_;
  /// @brief The spatial dimensions of the padding.
  DataShape pad_;
  /// @brief The spatial dimensions of the dilation.
  DataShape dilation_;
  /// @brief The spatial dimensions of the convolution input.
  DataShape conv_input_shape_;
  /// @brief The spatial dimensions of the col_buffer.
  DataShape col_buffer_shape_;
  /// @brief The spatial dimensions of the output.
  DataShape output_shape_;
  const DataShape* bottom_shape_;

  int num_spatial_axes_;
  int bottom_dim_;
  int top_dim_;

  int channel_axis_;
  int num_;
  int channels_;
  int group_;
  int out_spatial_dim_;
  int weight_offset_;
  int num_output_;
  bool bias_term_;
  bool is_1x1_;
  bool force_nd_im2col_;

  int num_kernels_im2col_;
  int num_kernels_col2im_;
  int conv_out_channels_;
  int conv_in_channels_;
  int conv_out_spatial_dim_;
  int kernel_dim_;
  int col_offset_;
  int output_offset_;

  Blob<Dtype> col_buffer_;
  Blob<Dtype> bias_multiplier_;

  inline void conv_im2col(const Dtype* data, Dtype* col_buff) {
    if (!force_nd_im2col_ && num_spatial_axes_ == 2) {
      im2col(CONTEXT, data, conv_in_channels_,
        conv_input_shape_.dim[1], conv_input_shape_.dim[2],
        kernel_shape_.dim[0], kernel_shape_.dim[1],
        pad_.dim[0], pad_.dim[1],
        stride_.dim[0], stride_.dim[1],
        dilation_.dim[0], dilation_.dim[1], col_buff);
    }
    else {
      im2col_nd(CONTEXT, data, num_spatial_axes_, num_kernels_im2col_,
        conv_input_shape_, col_buffer_.shape_,
        kernel_shape_, pad_,
        stride_, dilation_, col_buff);
    }
  }
  inline void conv_col2im(const Dtype* col_buff, Dtype* data) {
    if (!force_nd_im2col_ && num_spatial_axes_ == 2) {
      col2im(CONTEXT, col_buff, conv_in_channels_,
        conv_input_shape_.dim[1], conv_input_shape_.dim[2],
        kernel_shape_.dim[0], kernel_shape_.dim[1],
        pad_.dim[0], pad_.dim[1],
        stride_.dim[0], stride_.dim[1],
        dilation_.dim[0], dilation_.dim[1], data);
    }
    else {
      col2im_nd(CONTEXT, col_buff, num_spatial_axes_, num_kernels_col2im_,
        conv_input_shape_, col_buffer_.shape_,
        kernel_shape_, pad_, stride_,
        dilation_, data);
    }
  }

  int axis_;
  virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom,
    const vector<Blob<Dtype>*> & top)
  {
    // Configure the kernel size, padding, stride, and inputs.
    cJSON* conv_param = this->param_;
    force_nd_im2col_ = conv_param->getbool("force_nd_im2col", false);
    axis_ = conv_param->getint("axis", 1);
    channel_axis_ = bottom[0]->CanonicalAxisIndex(axis_);
    const int first_spatial_axis = channel_axis_ + 1;
    const int num_axes = bottom[0]->num_axes();
    num_spatial_axes_ = num_axes - first_spatial_axis;
    CHECK_GE(num_spatial_axes_, 0);
    vector<int> bottom_dim_blob_shape(1, num_spatial_axes_ + 1);
    //vector<int> spatial_dim_blob_shape(1, std::max(num_spatial_axes_, 1));
    // Setup filter kernel dimensions (kernel_shape_).
    //kernel_shape_.Reshape(spatial_dim_blob_shape);
    kernel_shape_ = cJSON_GetShape(conv_param, 1, "kernel_size", "kernel_h", "kernel_w");
    for (int i = 0; i < num_spatial_axes_; ++i) {
      CHECK_GT(kernel_shape_.dim[i], 0) << "Filter dimensions must be nonzero.";
    }
    // Setup stride dimensions (stride_).
    //stride_.Reshape(spatial_dim_blob_shape);
    stride_ = cJSON_GetShape(conv_param, 1, "stride", "stride_h", "stride_w");
    // Setup pad dimensions (pad_).
    //pad_.Reshape(spatial_dim_blob_shape);
    pad_ = cJSON_GetShape(conv_param, 0, "pad", "pad_h", "pad_w");

    // Setup dilation dimensions (dilation_).
    dilation_ = cJSON_GetShape(conv_param, 0, "dilation", "dilation_h", "dilation_w");

    // Special case: im2col is the identity for 1x1 convolution with stride 1
    // and no padding, so flag for skipping the buffer and transformation.
    is_1x1_ = true;
    for (int i = 0; i < num_spatial_axes_; ++i) {
      is_1x1_ &=
        kernel_shape_.dim[i] == 1 && stride_.dim[i] == 1 && pad_.dim[i] == 0;
      if (!is_1x1_) { break; }
    }
    // Configure output channels and groups.
    channels_ = bottom[0]->shape(channel_axis_);
    num_output_ = this->param_->getint("num_output", 0);
    CHECK_GT(num_output_, 0);
    group_ = this->param_->getint("group", 1);
    CHECK_EQ(channels_ % group_, 0);
    CHECK_EQ(num_output_ % group_, 0)
      << "Number of output should be multiples of group.";
    if (reverse_dimensions()) {
      conv_out_channels_ = channels_;
      conv_in_channels_ = num_output_;
    }
    else {
      conv_out_channels_ = num_output_;
      conv_in_channels_ = channels_;
    }
    // Handle the parameters: weights and biases.
    // - blobs_[0] holds the filter weights
    // - blobs_[1] holds the biases (optional)
    DataShape weight_shape = {0};
    weight_shape.dim[0] = conv_out_channels_;
    weight_shape.dim[1] = conv_in_channels_ / group_;
    for (int i = 0; i < num_spatial_axes_; ++i) {
      weight_shape.dim[i+2] = (kernel_shape_.dim[i]);
    }
    bias_term_ = this->param_->getbool("bias_term", true);
    DataShape bias_shape;
    bias_shape.set(num_output_);
    if (this->blobs_.size() > 0) {
      CHECK_EQ(1 + bias_term_, this->blobs_.size())
        << "Incorrect number of weight blobs.";
      if (weight_shape != this->blobs_[0]->shape()) {
        LOG(FATAL) << "Incorrect weight shape: expected shape "
          << DataShape_string(weight_shape) << "; instead, shape was "
          << DataShape_string(this->blobs_[0]->shape_);
      }
      if (bias_term_ && bias_shape != this->blobs_[1]->shape()) {
        LOG(FATAL) << "Incorrect bias shape: expected shape "
          << DataShape_string(bias_shape) << "; instead, shape was "
          << DataShape_string(this->blobs_[1]->shape_);
      }
      LOG(INFO) << "Skipping parameter initialization";
    }
    else {
      if (bias_term_) {
        blobs_reset(this->blobs_, 2);
      }
      else {
        blobs_reset(this->blobs_, 1);
      }
      // Initialize and fill the weights:
      // output channels x input channels per-group x kernel height x kernel width
      cJSON* weight_filler = this->param_->get("weight_filler");
      this->blobs_[0]->Reshape(weight_shape);
      Filler(this->blobs_[0], weight_filler);
      // If necessary, initialize and fill the biases.
      if (bias_term_) {
        cJSON* bias_filler = this->param_->get("bias_filler");
        this->blobs_[1]->Reshape(bias_shape);
        Filler(this->blobs_[1], bias_filler);
      }
    }
    kernel_dim_ = this->blobs_[0]->count(1);
    weight_offset_ = conv_out_channels_ * kernel_dim_ / group_;
    // Propagate gradients to the parameters (as directed by backward pass).
    //this->param_propagate_down_.resize(this->blobs_.size(), true);
  }

  virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
    const vector<Blob<Dtype>*> & top)
  {
    const int first_spatial_axis = channel_axis_ + 1;
    CHECK_EQ(bottom[0]->num_axes(), first_spatial_axis + num_spatial_axes_)
      << "bottom num_axes may not change.";
    num_ = bottom[0]->count(0, channel_axis_);
    CHECK_EQ(bottom[0]->shape(channel_axis_), channels_)
      << "Input size incompatible with convolution kernel.";
    // TODO: generalize to handle inputs of different shapes.
    for (int bottom_id = 1; bottom_id < bottom.size(); ++bottom_id) {
      CHECK(bottom[0]->shape() == bottom[bottom_id]->shape())
        << "All inputs must have the same shape.";
    }
    // Shape the tops.
    bottom_shape_ = &bottom[0]->shape();
    compute_output_shape();
    vector<int> top_shape(bottom[0]->shape().begin(), bottom[0]->shape().begin() + channel_axis_);
    top_shape.push_back(num_output_);
    for (int i = 0; i < num_spatial_axes_; ++i) {
      top_shape.push_back(output_shape_.dim[i]);
    }
    for (int top_id = 0; top_id < top.size(); ++top_id) {
      top[top_id]->Reshape(top_shape);
    }
    if (reverse_dimensions()) {
      conv_out_spatial_dim_ = bottom[0]->count(first_spatial_axis);
    }
    else {
      conv_out_spatial_dim_ = top[0]->count(first_spatial_axis);
    }
    col_offset_ = kernel_dim_ * conv_out_spatial_dim_;
    output_offset_ = conv_out_channels_ * conv_out_spatial_dim_ / group_;
    // Setup input dimensions (conv_input_shape_).
    //vector<int> bottom_dim_blob_shape(1, num_spatial_axes_ + 1);
    //conv_input_shape_.Reshape(bottom_dim_blob_shape);
    int* conv_input_shape_data = conv_input_shape_.dim;
    int i = 0;
    for (; i < num_spatial_axes_ + 1; ++i) {
      if (reverse_dimensions()) {
        conv_input_shape_data[i] = top[0]->shape(channel_axis_ + i);
      }
      else {
        conv_input_shape_data[i] = bottom[0]->shape(channel_axis_ + i);
      }
    }
    // The im2col result buffer will only hold one image at a time to avoid
    // overly large memory usage. In the special case of 1x1 convolution
    // it goes lazily unused to save memory.
    //col_buffer_shape_.clear();
    i = 0;
    col_buffer_shape_.dim[0] = (kernel_dim_ * group_);
    for (i=0; i < num_spatial_axes_; ++i) {
      if (reverse_dimensions()) {
        col_buffer_shape_.dim[i + 1] = (input_shape(i + 1));
      }
      else {
        col_buffer_shape_.dim[i + 1] = (output_shape_.dim[i]);
      }
    }
    col_buffer_.Reshape(col_buffer_shape_);
    bottom_dim_ = bottom[0]->count(channel_axis_);
    top_dim_ = top[0]->count(channel_axis_);
    num_kernels_im2col_ = conv_in_channels_ * conv_out_spatial_dim_;
    num_kernels_col2im_ = reverse_dimensions() ? top_dim_ : bottom_dim_;
    // Set up the all ones "bias multiplier" for adding biases by BLAS
    out_spatial_dim_ = top[0]->count(first_spatial_axis);
    if (bias_term_) {
      vector<int> bias_multiplier_shape(1, out_spatial_dim_);
      bias_multiplier_.Reshape(bias_multiplier_shape);
      caffe_set(bias_multiplier_.count(), Dtype(1),
        bias_multiplier_.mutable_data<Context>());
    }
  }


  void forward_gemm(const Dtype* input, const Dtype* weights, Dtype* output, bool skip_im2col = false)
  {
    const Dtype* col_buff = input;
    if (!is_1x1_) {
      if (!skip_im2col) {
        conv_im2col(input, col_buffer_.mutable_data<Context>());
      }
      col_buff = col_buffer_.data<Context>();
    }
    for (int g = 0; g < group_; ++g) {
      caffe_gemm<Dtype>(CONTEXT, CblasNoTrans, CblasNoTrans, conv_out_channels_ /
        group_, conv_out_spatial_dim_, kernel_dim_,
        (Dtype)1., weights + weight_offset_ * g, col_buff + col_offset_ * g,
        (Dtype)0., output + output_offset_ * g);
    }
  }

  void forward_bias(Dtype* output,
    const Dtype* bias)
  {
    caffe_gemm<Dtype>(CONTEXT, CblasNoTrans, CblasNoTrans, num_output_,
      out_spatial_dim_, 1, (Dtype)1., bias, bias_multiplier_.data<Context>(),
      (Dtype)1., output);
  }

  void backward_gemm(const Dtype* output,
    const Dtype* weights, Dtype* input)
  {
    Dtype* col_buff = col_buffer_.mutable_data<Context>();
    if (is_1x1_) {
      col_buff = input;
    }
    for (int g = 0; g < group_; ++g) {
      caffe_gemm<Dtype>(CONTEXT, CblasTrans, CblasNoTrans, kernel_dim_,
        conv_out_spatial_dim_, conv_out_channels_ / group_,
        (Dtype)1., weights + weight_offset_ * g, output + output_offset_ * g,
        (Dtype)0., col_buff + col_offset_ * g);
    }
    if (!is_1x1_) {
      conv_col2im(col_buff, input);
    }
  }

  void weight_gemm(const Dtype* input, const Dtype* output, Dtype* weights)
  {
    const Dtype* col_buff = input;
    if (!is_1x1_) {
      conv_im2col(input, col_buffer_.mutable_data<Context>());
      col_buff = col_buffer_.data<Context>();
    }
    for (int g = 0; g < group_; ++g) {
      caffe_gemm<Dtype>(CONTEXT, CblasNoTrans, CblasTrans, conv_out_channels_ / group_,
        kernel_dim_, conv_out_spatial_dim_,
        (Dtype)1., output + output_offset_ * g, col_buff + col_offset_ * g,
        (Dtype)1., weights + weight_offset_ * g);
    }
  }

  void backward_bias(Dtype* bias,
    const Dtype* input)
  {
    caffe_gemv<Dtype>(CONTEXT, CblasNoTrans, num_output_, out_spatial_dim_, 1.,
      input, bias_multiplier_.data<Context>(), 1., bias);
  }



};



#endif  // CAFFE_BASE_CONVOLUTION_LAYER_HPP_
