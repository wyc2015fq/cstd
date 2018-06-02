
/**
 * @brief A helper for image operations that rearranges image regions into
 *        column vectors.  Used by ConvolutionLayer to perform convolution
 *        by matrix multiplication.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */

class Im2colLayer : public Layer {
 public:
  explicit Im2colLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "Im2col"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

 protected:
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  /// @brief The spatial dimensions of a filter kernel.
  Blob<int> kernel_shape_;
  /// @brief The spatial dimensions of the stride.
  Blob<int> stride_;
  /// @brief The spatial dimensions of the padding.
  Blob<int> pad_;
  /// @brief The spatial dimensions of the dilation.
  Blob<int> dilation_;

  int num_spatial_axes_;
  int bottom_dim_;
  int top_dim_;

  int channel_axis_;
  int num_;
  int channels_;

  bool force_nd_im2col_;
};




void Im2colLayer::LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  ConvolutionParameter conv_param = this->layer_param_.convolution_param();
  force_nd_im2col_ = conv_param.force_nd_im2col();
  const int input_num_dims = bottom[0]->shape().size();
  channel_axis_ = bottom[0]->CanonicalAxisIndex(conv_param.axis());
  const int first_spatial_dim = channel_axis_ + 1;
  num_spatial_axes_ = input_num_dims - first_spatial_dim;
  CHECK_GE(num_spatial_axes_, 1);
  vector<int> dim_blob_shape(1, num_spatial_axes_);
  // Setup filter kernel dimensions (kernel_shape_).
  kernel_shape_.Reshape(dim_blob_shape);
  int* kernel_shape_data = kernel_shape_;
  if (conv_param.has_kernel_h() || conv_param.has_kernel_w()) {
    CHECK_EQ(num_spatial_axes_, 2)
        << "kernel_h & kernel_w can only be used for 2D convolution.";
    CHECK_EQ(0, conv_param.kernel_size_size())
        << "Either kernel_size or kernel_h/w should be specified; not both.";
    kernel_shape_data[0] = conv_param.kernel_h();
    kernel_shape_data[1] = conv_param.kernel_w();
  } else {
    const int num_kernel_dims = conv_param.kernel_size_size();
    CHECK(num_kernel_dims == 1 || num_kernel_dims == num_spatial_axes_)
        << "kernel_size must be specified once, or once per spatial dimension "
        << "(kernel_size specified " << num_kernel_dims << " times; "
        << num_spatial_axes_ << " spatial dims);";
      for (i = 0; i < num_spatial_axes_; ++i) {
        kernel_shape_data[i] =
            conv_param.kernel_size((num_kernel_dims == 1) ? 0 : i);
      }
  }
  for (i = 0; i < num_spatial_axes_; ++i) {
    CHECK_GT(kernel_shape_data[i], 0) << "Filter dimensions must be nonzero.";
  }
  // Setup stride dimensions (stride_).
  stride_.Reshape(dim_blob_shape);
  int* stride_data = stride_;
  if (conv_param.has_stride_h() || conv_param.has_stride_w()) {
    CHECK_EQ(num_spatial_axes_, 2)
        << "stride_h & stride_w can only be used for 2D convolution.";
    CHECK_EQ(0, conv_param.stride_size())
        << "Either stride or stride_h/w should be specified; not both.";
    stride_data[0] = conv_param.stride_h();
    stride_data[1] = conv_param.stride_w();
  } else {
    const int num_stride_dims = conv_param.stride_size();
    CHECK(num_stride_dims == 0 || num_stride_dims == 1 ||
          num_stride_dims == num_spatial_axes_)
        << "stride must be specified once, or once per spatial dimension "
        << "(stride specified " << num_stride_dims << " times; "
        << num_spatial_axes_ << " spatial dims);";
    const int kDefaultStride = 1;
    for (i = 0; i < num_spatial_axes_; ++i) {
      stride_data[i] = (num_stride_dims == 0) ? kDefaultStride :
          conv_param.stride((num_stride_dims == 1) ? 0 : i);
      CHECK_GT(stride_data[i], 0) << "Stride dimensions must be nonzero.";
    }
  }
  // Setup pad dimensions (pad_).
  pad_.Reshape(dim_blob_shape);
  int* pad_data = pad_;
  if (conv_param.has_pad_h() || conv_param.has_pad_w()) {
    CHECK_EQ(num_spatial_axes_, 2)
        << "pad_h & pad_w can only be used for 2D convolution.";
    CHECK_EQ(0, conv_param.pad_size())
        << "Either pad or pad_h/w should be specified; not both.";
    pad_data[0] = conv_param.pad_h();
    pad_data[1] = conv_param.pad_w();
  } else {
    const int num_pad_dims = conv_param.pad_size();
    CHECK(num_pad_dims == 0 || num_pad_dims == 1 ||
          num_pad_dims == num_spatial_axes_)
        << "pad must be specified once, or once per spatial dimension "
        << "(pad specified " << num_pad_dims << " times; "
        << num_spatial_axes_ << " spatial dims);";
    const int kDefaultPad = 0;
    for (i = 0; i < num_spatial_axes_; ++i) {
      pad_data[i] = (num_pad_dims == 0) ? kDefaultPad :
          conv_param.pad((num_pad_dims == 1) ? 0 : i);
    }
  }
  // Setup dilation dimensions (dilation_).
  dilation_.Reshape(dim_blob_shape);
  int* dilation_data = dilation_;
  const int num_dilation_dims = conv_param.dilation_size();
  CHECK(num_dilation_dims == 0 || num_dilation_dims == 1 ||
        num_dilation_dims == num_spatial_axes_)
      << "dilation must be specified once, or once per spatial dimension "
      << "(dilation specified " << num_dilation_dims << " times; "
      << num_spatial_axes_ << " spatial dims).";
  const int kDefaultDilation = 1;
  for (i = 0; i < num_spatial_axes_; ++i) {
    dilation_data[i] = (num_dilation_dims == 0) ? kDefaultDilation :
                       conv_param.dilation((num_dilation_dims == 1) ? 0 : i);
  }
}


void Im2colLayer::Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  vector<int> top_shape = bottom[0]->shape();
  const int* kernel_shape_data = kernel_shape_.cpu_data();
  const int* stride_data = stride_.cpu_data();
  const int* pad_data = pad_.cpu_data();
  const int* dilation_data = dilation_.cpu_data();
  for (i = 0; i < num_spatial_axes_; ++i) {
    top_shape[channel_axis_] *= kernel_shape_data[i];
    const int input_dim = bottom[0]->shape(channel_axis_ + i + 1);
    const int kernel_extent = dilation_data[i] * (kernel_shape_data[i] - 1) + 1;
    const int output_dim = (input_dim + 2 * pad_data[i] - kernel_extent)
        / stride_data[i] + 1;
    top_shape[channel_axis_ + i + 1] = output_dim;
  }
  top[0]->Reshape(top_shape);
  num_ = bottom[0]->count(0, channel_axis_);
  bottom_dim_ = bottom[0]->count(channel_axis_);
  top_dim_ = top[0]->count(channel_axis_);

  channels_ = bottom[0]->shape(channel_axis_);
}


void Im2colLayer::Forward(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  Dtype* nextData = nextData;
  for (n = 0; n < num_; ++n) {
    DCHECK_EQ(bottom[0]->shape().size() - channel_axis_, num_spatial_axes_ + 1);
    DCHECK_EQ(top[0]->shape().size() - channel_axis_, num_spatial_axes_ + 1);
    DCHECK_EQ(kernel_shape_.count(), num_spatial_axes_);
    DCHECK_EQ(pad_.count(), num_spatial_axes_);
    DCHECK_EQ(stride_.count(), num_spatial_axes_);
    DCHECK_EQ(dilation_.count(), num_spatial_axes_);
    if (!force_nd_im2col_ && num_spatial_axes_ == 2) {
      im2col(prevData + n * bottom_dim_, channels_,
          bottom[0]->shape(channel_axis_ + 1),
          bottom[0]->shape(channel_axis_ + 2),
          kernel_shape_.cpu_data()[0], kernel_shape_.cpu_data()[1],
          pad_.cpu_data()[0], pad_.cpu_data()[1],
          stride_.cpu_data()[0], stride_.cpu_data()[1],
          dilation_.cpu_data()[0], dilation_.cpu_data()[1],
          nextData + n * top_dim_);
    } else {
      im2col_nd(prevData + n * bottom_dim_, num_spatial_axes_,
          bottom[0]->shape().data() + channel_axis_,
          top[0]->shape().data() + channel_axis_,
          kernel_shape_.cpu_data(), pad_.cpu_data(), stride_.cpu_data(),
          dilation_.cpu_data(), nextData + n * top_dim_);
    }
  }
}


void Im2colLayer::Backward(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  const Dtype* nextDiff = nextDiff;
  Dtype* prevDiff = bottom[0]->mutable_diff();
  for (n = 0; n < num_; ++n) {
    if (!force_nd_im2col_ && num_spatial_axes_ == 2) {
      col2im(nextDiff + n * top_dim_, channels_,
          bottom[0]->shape(channel_axis_ + 1),
          bottom[0]->shape(channel_axis_ + 2),
          kernel_shape_.cpu_data()[0], kernel_shape_.cpu_data()[1],
          pad_.cpu_data()[0], pad_.cpu_data()[1],
          stride_.cpu_data()[0], stride_.cpu_data()[1],
          dilation_.cpu_data()[0], dilation_.cpu_data()[1],
          prevDiff + n * bottom_dim_);
    } else {
      col2im_nd(nextDiff + n * top_dim_, num_spatial_axes_,
          bottom[0]->shape().data() + channel_axis_,
          top[0]->shape().data() + channel_axis_,
          kernel_shape_.cpu_data(), pad_.cpu_data(), stride_.cpu_data(),
          dilation_.cpu_data(), prevDiff + n * bottom_dim_);
    }
  }
}

#ifdef CPU_ONLY
STUB_GPU(Im2colLayer);
#endif

INSTANTIATE_CLASS(Im2colLayer);
REGISTER_LAYER_CLASS(Im2col);


