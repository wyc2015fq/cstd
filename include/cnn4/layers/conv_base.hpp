#ifndef CAFFE_BASE_CONVOLUTION_LAYER_HPP_
#define CAFFE_BASE_CONVOLUTION_LAYER_HPP_

DataShape cJSON_GetShape(cJSON* param, int kDefaultPad, const char* name, const char* name_h = NULL, const char* name_w = NULL) {
  DataShape pad;
  int* pad_data = pad.dim;
  char buf[64];
  cJSON* item = param->get(name);
  if (!name_h) {
    _snprintf(buf, 64, "%s_h", name);
    name_h = buf;
  }
  cJSON* h_json = param->get(name_h);
  if (!name_w) {
    _snprintf(buf, 64, "%s_w", name);
    name_w = buf;
  }
  cJSON* w_json = param->get(name_w);
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

#define ConvolutionParameter_DEF(DEF) \
DEF##Bool(force_nd_im2col, false, 0) \
DEF##Bool(bias_term, true, 0) \
DEF##Int(axis, 1, 0) \
DEF##Int(num_output, 0, 0) \
DEF##Int(group, 1, 0) \
DEF##Struct(weight_filler, 0, Filler) \
DEF##Struct(bias_filler, 0, Filler) \


struct ConvolutionLayerBase : public Layer
{
public:
  ConvolutionParameter_DEF(Def);
  virtual inline int MinBottomBlobs() const { return 1; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline bool EqualNumBottomTopBlobs() const { return true; }

  /// @brief The spatial dimensions of the input.
  // reverse_dimensions should return true iff we are implementing deconv, so
  // that conv helpers know which dimensions are which.
  virtual bool reverse_dimensions() = 0;
  // Compute height_out_ and width_out_ from other parameters.
  virtual void compute_output_shape(const int* input_shape) = 0;

  /// @brief The spatial dimensions of a filter kernel.
  DataShape kernel_;
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

  int num_spatial_axes_;
  int bottom_dim_;
  int top_dim_;

  int channel_axis_;
  int num_;
  int channels_;
  int out_spatial_dim_;
  int weight_offset_;
  bool is_1x1_;

  int num_kernels_im2col_;
  int num_kernels_col2im_;
  int conv_out_channels_;
  int conv_in_channels_;
  int conv_out_spatial_dim_;
  int kernel_dim_;
  int col_offset_;
  int output_offset_;

  Blob col_buffer_;
  ConvolutionLayerBase() {
    init();
  }
  //Blob bias_multiplier_;
  inline void conv_im2col(const Dtype* data, Dtype* col_buff) {
    if (!force_nd_im2col_ && num_spatial_axes_ == 2) {
      im2col(data, conv_in_channels_,
        conv_input_shape_.dim[1], conv_input_shape_.dim[2],
        kernel_.dim[0], kernel_.dim[1],
        pad_.dim[0], pad_.dim[1],
        stride_.dim[0], stride_.dim[1],
        dilation_.dim[0], dilation_.dim[1], col_buff);
    }
    else {
      im2col_nd(data, num_spatial_axes_, num_kernels_im2col_,
        conv_input_shape_, col_buffer_.shape_,
        kernel_, pad_,
        stride_, dilation_, col_buff);
    }
  }
  inline void conv_col2im(const Dtype* col_buff, Dtype* data) {
    if (!force_nd_im2col_ && num_spatial_axes_ == 2) {
      col2im(col_buff, conv_in_channels_,
        conv_input_shape_.dim[1], conv_input_shape_.dim[2],
        kernel_.dim[0], kernel_.dim[1],
        pad_.dim[0], pad_.dim[1],
        stride_.dim[0], stride_.dim[1],
        dilation_.dim[0], dilation_.dim[1], data);
    }
    else {
      col2im_nd(col_buff, num_spatial_axes_, num_kernels_col2im_,
        conv_input_shape_, col_buffer_.shape_,
        kernel_, pad_, stride_,
        dilation_, data);
    }
  }
  void init() {
    ConvolutionParameter_DEF(Set);
    set_kernel_size(1);
    set_pad(0);
    set_stride(1);
    set_dilation(1);
  }

  void init(CJSON* param) {
    ConvolutionParameter_DEF(Get);
    kernel_ = cJSON_GetShape(param, 1, "kernel_size", "kernel_h", "kernel_w");
    // Setup stride dimensions (stride_).
    //stride_.Reshape(spatial_dim_blob_shape);
    stride_ = cJSON_GetShape(param, 1, "stride", "stride_h", "stride_w");
    // Setup pad dimensions (pad_).
    //pad_.Reshape(spatial_dim_blob_shape);
    pad_ = cJSON_GetShape(param, 0, "pad", "pad_h", "pad_w");

    // Setup dilation dimensions (dilation_).
    dilation_ = cJSON_GetShape(param, 1, "dilation", "dilation_h", "dilation_w");
    CJSON* blobs_json = param->get("blobs");
  }
  void set_group(int group) {  group_ = group;  }
  void set_force_nd_im2col(bool force_nd_im2col) { force_nd_im2col_ = force_nd_im2col; }
  void set_bias_term(bool bias_term) { bias_term_ = bias_term; }
  void set_axis(int axis) {    axis_ = axis;  }
  void set_kernel_size(int kernel_size) {    kernel_ = dataShape(kernel_size, kernel_size, kernel_size);  }
  void set_kernel(int kernel_h, int kernel_w) {    kernel_ = dataShape(kernel_h, kernel_w);  }
  void set_stride(int stride) {    stride_ = dataShape(stride, stride, stride);  }
  void set_stride(int stride_h, int stride_w) {    stride_ = dataShape(stride_h, stride_w);  }
  void set_pad(int pad) {    pad_ = dataShape(pad, pad, pad);  }
  void set_pad(int pad_h, int pad_w) {    pad_ = dataShape(pad_h, pad_w);  }
  void set_dilation(int dilation) {    dilation_ = dataShape(dilation, dilation, dilation);  }
  void set_dilation(int dilation_h, int dilation_w) {    dilation_ = dataShape(dilation_h, dilation_w);  }
  void set_num_output(int num_output) {  num_output_ = num_output;}
  virtual void set(int kernel_h, int kernel_w, int pad_h, int pad_w, int stride_h, int stride_w, int dilation_h, int dilation_w, int num_output, int group = 1, int axis = 1, bool bias_term = true) {
    kernel_ = dataShape(kernel_h, kernel_w);
    pad_ = dataShape(pad_h, pad_w);
    stride_ = dataShape(stride_h, stride_w);
    dilation_ = dataShape(dilation_h, dilation_w);

    num_output_ = num_output;
    group_ = group;
    bias_term_ = bias_term;
    axis_ = axis;
  }
  virtual void setup(const DataShape& in_shape, Blob* w, Blob* b, bool create_w) {
    // Configure the kernel size, padding, stride, and inputs.
    channel_axis_ = in_shape.CanonicalAxisIndex(axis_);
    const int first_spatial_axis = channel_axis_ + 1;
    const int num_axes = in_shape.num_axes();
    num_spatial_axes_ = num_axes - first_spatial_axis;
    CHECK_GE(num_spatial_axes_, 0);
    vector<int> bottom_dim_blob_shape(1, num_spatial_axes_ + 1);
    //vector<int> spatial_dim_blob_shape(1, std::max(num_spatial_axes_, 1));
    // Setup filter kernel dimensions (kernel_).
    //kernel_.Reshape(spatial_dim_blob_shape);
    for (int i = 0; i < num_spatial_axes_; ++i) {
      CHECK_GT(kernel_.dim[i], 0) << "Filter dimensions must be nonzero.";
    }

    // Special case: im2col is the identity for 1x1 convolution with stride 1
    // and no padding, so flag for skipping the buffer and transformation.
    is_1x1_ = true;
    for (int i = 0; i < num_spatial_axes_; ++i) {
      is_1x1_ &=
        kernel_.dim[i] == 1 && stride_.dim[i] == 1 && pad_.dim[i] == 0;
      if (!is_1x1_) { break; }
    }
    // Configure output channels and groups.
    channels_ = in_shape.shape(channel_axis_);
    CHECK_GT(num_output_, 0);
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
    DataShape weight_shape = dataShape(conv_out_channels_, conv_in_channels_ / group_);
    for (int i = 0; i < num_spatial_axes_; ++i) {
      weight_shape.dim[i + 2] = (kernel_.dim[i]);
    }
    DataShape bias_shape = dataShape(num_output_);

    if (!create_w) {
      if (weight_shape != w->shape()) {
        LOG(FATAL) << "Incorrect weight shape: expected shape "
          << DataShape_string(weight_shape) << "; instead, shape was "
          << DataShape_string(w->shape_);
      }
      if (bias_term_) {
        if (bias_shape != b->shape()) {
          LOG(FATAL) << "Incorrect bias shape: expected shape "
            << DataShape_string(bias_shape) << "; instead, shape was "
            << DataShape_string(b->shape_);
        }
      }
    }
    else {
      // Initialize and fill the weights:
      // output channels x input channels per-group x kernel height x kernel width
      w->Reshape(weight_shape);
      weight_filler_.Fill(w);
      // If necessary, initialize and fill the biases.
      if (b) {
        b->Reshape(bias_shape);
        bias_filler_.Fill(b);
      }
    }
    kernel_dim_ = w->count(1);
    weight_offset_ = conv_out_channels_ * kernel_dim_ / group_;
    // Propagate gradients to the parameters (as directed by backward pass).
    //parampropagate_down_.resize(this->blobs_.size(), true);
  }

  virtual void LayerSetUp(const vector<Blob*>& bottom,  const vector<Blob*> & top)
  {
    int blobs_size = bias_term_ ? 2 : 1;
    bool create_w = blobs_.size() != blobs_size;
    if (create_w) {
      create_w = true;
      blobs_reset(this->blobs_, blobs_size);
      //LOG(INFO) << "Skipping parameter initialization";
    }
    setup(bottom[0]->shape_, blobs_[0], bias_term_ ? blobs_[1] : NULL, create_w);
    CHECK_EQ(blobs_size, this->blobs_.size()) << "Incorrect number of weight blobs.";
  }
  virtual DataShape Reshape(const DataShape& in_shape) {
    const int first_spatial_axis = channel_axis_ + 1;
    CHECK_EQ(in_shape.num_axes(), first_spatial_axis + num_spatial_axes_)
      << "bottom num_axes may not change.";
    num_ = in_shape.count(0, channel_axis_);
    CHECK_EQ(in_shape.shape(channel_axis_), channels_) << "Input size incompatible with convolution kernel.";
    // TODO: generalize to handle inputs of different shapes.
    // Shape the tops.
    const int* bottom_shape_ = in_shape.dim + channel_axis_;
    compute_output_shape(bottom_shape_);
    DataShape top_shape;
    top_shape.set(in_shape.begin(), channel_axis_);
    top_shape.push_back(num_output_);
    for (int i = 0; i < num_spatial_axes_; ++i) {
      top_shape.push_back(output_shape_.dim[i]);
    }
    DataShape out;
    out.set(top_shape.begin(), top_shape.size());
    if (reverse_dimensions()) {
      conv_out_spatial_dim_ = in_shape.count(first_spatial_axis);
    }
    else {
      conv_out_spatial_dim_ = out.count(first_spatial_axis);
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
        conv_input_shape_data[i] = out.shape(channel_axis_ + i);
      }
      else {
        conv_input_shape_data[i] = in_shape.shape(channel_axis_ + i);
      }
    }
    // The im2col result buffer will only hold one image at a time to avoid
    // overly large memory usage. In the special case of 1x1 convolution
    // it goes lazily unused to save memory.
    //col_buffer_shape_.clear();
    i = 0;
    col_buffer_shape_ = dataShape(kernel_dim_ * group_);
    for (i = 0; i < num_spatial_axes_; ++i) {
      if (reverse_dimensions()) {
        col_buffer_shape_.dim[i + 1] = bottom_shape_[i + 1];
      }
      else {
        col_buffer_shape_.dim[i + 1] = (output_shape_.dim[i]);
      }
    }
    col_buffer_.Reshape(col_buffer_shape_);
    bottom_dim_ = in_shape.count(channel_axis_);
    top_dim_ = out.count(channel_axis_);
    num_kernels_im2col_ = conv_in_channels_ * conv_out_spatial_dim_;
    num_kernels_col2im_ = reverse_dimensions() ? top_dim_ : bottom_dim_;
    // Set up the all ones "bias multiplier" for adding biases by BLAS
    out_spatial_dim_ = out.count(first_spatial_axis);
    if (bias_term_) {
      //vector<int> bias_multiplier_shape(1, out_spatial_dim_);
      //bias_multiplier_.Reshape(bias_multiplier_shape);
      //caffe_set(bias_multiplier_.count(), Dtype(1), bias_multiplier_.mdata());
    }
    return out;
  }

  virtual void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    Blob* in = bottom[0];
    Blob* out = top[0];
    DataShape dshape = Reshape(in->shape_);
    for (int i = 1; i < bottom.size(); ++i) {
      CHECK(in->shape() == bottom[i]->shape())
        << "All inputs must have the same shape.";
    }
    for (int i = 0; i < top.size(); ++i) {
      top[i]->Reshape(dshape);
    }
  }

  void forward_gemm(const Dtype* input, const Dtype* weights, Dtype* output, bool skip_im2col = false)
  {
    const Dtype* col_buff = input;
    if (!is_1x1_) {
      if (!skip_im2col) {
        conv_im2col(input, col_buffer_.mdata());
      }
      col_buff = col_buffer_.data();
    }
    for (int g = 0; g < group_; ++g) {
      caffe_gemm(CblasNoTrans, CblasNoTrans, conv_out_channels_ /
        group_, conv_out_spatial_dim_, kernel_dim_,
        (Dtype)1., weights + weight_offset_ * g, col_buff + col_offset_ * g,
        (Dtype)0., output + output_offset_ * g);
    }
  }

  void forward_bias(Dtype* output, const Dtype* bias)
  {
    const Dtype* ones = (const Dtype*)get_ones(out_spatial_dim_);
    caffe_gemm(CblasNoTrans, CblasNoTrans, num_output_, out_spatial_dim_, 1, (Dtype)1., bias, ones, (Dtype)1., output);
  }

  void backward_gemm(const Dtype* output, const Dtype* weights, Dtype* input)
  {
    Dtype* col_buff = col_buffer_.mdata();
    if (is_1x1_) {
      col_buff = input;
    }
    for (int g = 0; g < group_; ++g) {
      caffe_gemm(CblasTrans, CblasNoTrans, kernel_dim_,
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
      conv_im2col(input, col_buffer_.mdata());
      col_buff = col_buffer_.data();
    }
    for (int g = 0; g < group_; ++g) {
      caffe_gemm(CblasNoTrans, CblasTrans, conv_out_channels_ / group_,
        kernel_dim_, conv_out_spatial_dim_,
        (Dtype)1., output + output_offset_ * g, col_buff + col_offset_ * g,
        (Dtype)1., weights + weight_offset_ * g);
    }
  }

  void backward_bias(Dtype* bias, const Dtype* input)
  {
    //const Data* ones = bias_multiplier_.data();
    const Dtype* ones = (const Dtype*)get_ones(out_spatial_dim_);
    caffe_gemv(CblasNoTrans, num_output_, out_spatial_dim_, 1., input, ones, 1., bias);
  }

};



#endif  // CAFFE_BASE_CONVOLUTION_LAYER_HPP_