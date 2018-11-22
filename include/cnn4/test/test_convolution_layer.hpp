

// Reference convolution for checking results:
// accumulate through explicit loops over input, output, and filters.

  void caffe_conv(Blob* in, Blob* out, Blob* weights, Blob* bias,
    int groups, int kernel_h, int kernel_w, int pad_h, int pad_w,
    int stride_h, int stride_w, int dilation_h, int dilation_w)
  {
    int num_axes = out->num_axes();
    const bool has_depth = (num_axes == 5);
    if (!has_depth) { CHECK_EQ(4, num_axes); }
    // Kernel size, stride, and pad
    int kernel_d, pad_d, stride_d, dilation_d;
    if (has_depth) {
      kernel_d = kernel_h;
      stride_d = stride_h;
      pad_d = pad_h;
      dilation_d = dilation_h;
    } else {
      kernel_d = stride_d = dilation_d = 1;
      pad_d = 0;
    }
    // Groups
    int o_g = out->shape(1) / groups;
    int k_g = in->shape(1) / groups;
    int o_head, k_head;
    // Convolution
    vector<int> weight_offset(4 + has_depth);
    vector<int> in_offset(4 + has_depth);
    vector<int> out_offset(4 + has_depth);
    Dtype* out_data = out->cpu_mdata();
    for (int n = 0; n < out->shape(0); n++) {
      for (int g = 0; g < groups; g++) {
        o_head = o_g * g;
        k_head = k_g * g;
        for (int o = 0; o < o_g; o++) {
          for (int k = 0; k < k_g; k++) {
            for (int z = 0; z < (has_depth ? out->shape(2) : 1); z++) {
              for (int y = 0; y < out->shape(2 + has_depth); y++) {
                for (int x = 0; x < out->shape(3 + has_depth); x++) {
                  for (int r = 0; r < kernel_d; r++) {
                    for (int p = 0; p < kernel_h; p++) {
                      for (int q = 0; q < kernel_w; q++) {
                        int in_z = z * stride_d - pad_d + r * dilation_d;
                        int in_y = y * stride_h - pad_h + p * dilation_h;
                        int in_x = x * stride_w - pad_w + q * dilation_w;
                        if (in_z >= 0 && in_z < (has_depth ? in->shape(2) : 1)
                            && in_y >= 0 && in_y < in->shape(2 + has_depth)
                            && in_x >= 0 && in_x < in->shape(3 + has_depth)) {
                          weight_offset[0] = o + o_head;
                          weight_offset[1] = k;
                          if (has_depth) { weight_offset[2] = r; }
                          weight_offset[2 + has_depth] = p;
                          weight_offset[3 + has_depth] = q;
                          in_offset[0] = n;
                          in_offset[1] = k + k_head;
                          if (has_depth) { in_offset[2] = in_z; }
                          in_offset[2 + has_depth] = in_y;
                          in_offset[3 + has_depth] = in_x;
                          out_offset[0] = n;
                          out_offset[1] = o + o_head;
                          if (has_depth) { out_offset[2] = z; }
                          out_offset[2 + has_depth] = y;
                          out_offset[3 + has_depth] = x;
                          out_data[out->offset(out_offset)] += in->data_at(in_offset) * weights->data_at(weight_offset);
                        }
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    // Bias
    if (bias) {
      const Dtype* bias_data = bias->cpu_data();
      for (int n = 0; n < out->shape(0); n++) {
        for (int o = 0; o < out->shape(1); o++) {
          for (int z = 0; z < (has_depth ? out->shape(2) : 1); z++) {
            for (int y = 0; y < out->shape(2 + has_depth); y++) {
              for (int x = 0; x < out->shape(3 + has_depth); x++) {
                out_offset[0] = n;
                out_offset[1] = o;
                if (has_depth) { out_offset[2] = z; }
                out_offset[2 + has_depth] = y;
                out_offset[3 + has_depth] = x;
                out_data[out->offset(out_offset)] += bias_data[o];
              }
            }
          }
        }
      }
    }
  }


  void caffe_conv(Blob* in, Blob* out, const ConvolutionLayer& layer) {
    caffe_conv(in, out, layer.blobs_[0], layer.blobs_[1], layer.group_,
      layer.kernel_.dim[0], layer.kernel_.dim[1],
      layer.pad_.dim[0], layer.pad_.dim[1],
      layer.stride_.dim[0], layer.stride_.dim[1],
      layer.dilation_.dim[0], layer.dilation_.dim[1]);
  }


  class ConvolutionLayerTest : public MultiDeviceTest
  {
    typedef typename TypeParam::Dtype Dtype;

  protected:
    ConvolutionLayerTest()
      : blob_bottom_(new Blob(2, 3, 6, 4)),
        blob_bottom_2_(new Blob(2, 3, 6, 4)),
      blob_top_(new Blob()),
      ref_blob_top_(new Blob()),
        blob_top_2_(new Blob()) {}
    virtual void SetUp() {
      // fill the values
      GaussianFiller(this->blob_bottom_);
      GaussianFiller(this->blob_bottom_2_);
      blob_bottom_vec_.push_back(blob_bottom_);
      blob_top_vec_.push_back(blob_top_);
    }

    virtual ~ConvolutionLayerTest() {
      delete blob_bottom_;
      delete blob_bottom_2_;
      delete blob_top_;
      delete blob_top_2_;
      delete ref_blob_top_;
    }

    virtual Blob* MakeReferenceTop(Blob* top) {
      this->ref_blob_top_->ReshapeLike(*top);
      return this->ref_blob_top_;
    }

    Blob* const blob_bottom_;
    Blob* const blob_bottom_2_;
    Blob* const blob_top_;
    Blob* const blob_top_2_;
    Blob* ref_blob_top_;
    vector<Blob*> blob_bottom_vec_;
    vector<Blob*> blob_top_vec_;
  };

  TYPED_TEST_CASE(ConvolutionLayerTest, TestDtypesAndDevices);

  TYPED_TEST(ConvolutionLayerTest, TestSetup)
  {
    typedef typename TypeParam::Dtype Dtype;
    ConvolutionLayer layer;
    //layer.set(3, 3, 0, 0, 2, 2, 1, 1, 4, 1, 1, false);
    layer.set_kernel_size(3);
    layer.set_stride(2);
    layer.num_output_ = (4);
    this->blob_bottom_vec_.push_back(this->blob_bottom_2_);
    this->blob_top_vec_.push_back(this->blob_top_2_);
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    EXPECT_EQ(this->blob_top_->num(), 2);
    EXPECT_EQ(this->blob_top_->channels(), 4);
    EXPECT_EQ(this->blob_top_->height(), 2);
    EXPECT_EQ(this->blob_top_->width(), 1);
    EXPECT_EQ(this->blob_top_2_->num(), 2);
    EXPECT_EQ(this->blob_top_2_->channels(), 4);
    EXPECT_EQ(this->blob_top_2_->height(), 2);
    EXPECT_EQ(this->blob_top_2_->width(), 1);
    // setting group should not change the shape
    layer.num_output_ = (3);
    layer.group_ = (3);
    
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    EXPECT_EQ(this->blob_top_->num(), 2);
    EXPECT_EQ(this->blob_top_->channels(), 3);
    EXPECT_EQ(this->blob_top_->height(), 2);
    EXPECT_EQ(this->blob_top_->width(), 1);
    EXPECT_EQ(this->blob_top_2_->num(), 2);
    EXPECT_EQ(this->blob_top_2_->channels(), 3);
    EXPECT_EQ(this->blob_top_2_->height(), 2);
    EXPECT_EQ(this->blob_top_2_->width(), 1);
  }

  TYPED_TEST(ConvolutionLayerTest, TestSimpleConvolution)
  {
    typedef typename TypeParam::Dtype Dtype;
    this->blob_bottom_vec_.push_back(this->blob_bottom_2_);
    this->blob_top_vec_.push_back(this->blob_top_2_);
    ConvolutionLayer layer;
    layer.set_kernel_size(3);
    layer.set_stride(2);
    layer.set_num_output(4);
    layer.weight_filler_.set_type("gaussian");
    layer.bias_filler_.set_type("constant");
    layer.bias_filler_.set_value(0.1);
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    // Check against reference convolution.
    const Dtype* top_data;
    const Dtype* ref_top_data;
    caffe_conv(this->blob_bottom_, this->MakeReferenceTop(this->blob_top_), layer);
    top_data = this->blob_top_->cpu_data();
    ref_top_data = this->ref_blob_top_->cpu_data();
    for (int i = 0; i < this->blob_top_->count(); ++i) {
      EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
    }
    caffe_conv(this->blob_bottom_2_, this->MakeReferenceTop(this->blob_top_2_), layer);
    top_data = this->blob_top_2_->cpu_data();
    ref_top_data = this->ref_blob_top_->cpu_data();
    for (int i = 0; i < this->blob_top_->count(); ++i) {
      EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
    }
  }

  TYPED_TEST0(ConvolutionLayerTest, TestDilatedConvolution)
  {
    typedef typename TypeParam::Dtype Dtype;
    vector<int> bottom_shape;
    bottom_shape.push_back(2);
    bottom_shape.push_back(3);
    bottom_shape.push_back(8);
    bottom_shape.push_back(7);
    this->blob_bottom_vec_.push_back(this->blob_bottom_2_);
    this->blob_top_vec_.push_back(this->blob_top_2_);
    for (int i = 0; i < this->blob_bottom_vec_.size(); ++i) {
      this->blob_bottom_vec_[i]->Reshape(bottom_shape);
    }
    ConvolutionLayer layer;
    layer.set_kernel_size(3);
    layer.set_dilation(2);
    layer.set_num_output(4);
    layer.weight_filler_.set_type("gaussian");
    layer.bias_filler_.set_type("constant");
    layer.bias_filler_.set_value(0.1);
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    // Check against reference convolution.
    const Dtype* top_data;
    const Dtype* ref_top_data;
    caffe_conv(this->blob_bottom_, this->MakeReferenceTop(this->blob_top_), layer);
    top_data = this->blob_top_->cpu_data();
    ref_top_data = this->ref_blob_top_->cpu_data();
    for (int i = 0; i < this->blob_top_->count(); ++i) {
      EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
    }
    caffe_conv(this->blob_bottom_2_, this->MakeReferenceTop(this->blob_top_2_), layer);
    top_data = this->blob_top_2_->cpu_data();
    ref_top_data = this->ref_blob_top_->cpu_data();
    for (int i = 0; i < this->blob_top_->count(); ++i) {
      EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
    }
  }

  TYPED_TEST(ConvolutionLayerTest, Test0DConvolution)
  {
    typedef typename TypeParam::Dtype Dtype;
    ConvolutionLayer layer;
    const int kNumOutput = 3;
    layer.set_num_output(kNumOutput);
    layer.set_axis(3);
    layer.weight_filler_.set_type("gaussian");
    layer.bias_filler_.set_type("gaussian");
    DataShape top_shape = this->blob_bottom_->shape();
    top_shape[3] = kNumOutput;
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    EXPECT_EQ(top_shape, this->blob_top_->shape());
    layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    // Check against reference convolution.
    vector<int> weight_offset(2);
    Blob* weight = layer.blobs_[0];
    Blob* bias = layer.blobs_[1];
    const int num = this->blob_top_->count(3);
    const int dim = this->blob_top_->shape(3);
    const int bottom_dim = this->blob_bottom_->shape(3);
    for (int n = 0; n < num; ++n) {
      for (int d = 0; d < dim; ++d) {
        weight_offset[0] = d;
        Dtype value = bias->cpu_data()[d];
        for (int bottom_d = 0; bottom_d < bottom_dim; ++bottom_d) {
          weight_offset[1] = bottom_d;
          value += weight->data_at(weight_offset) *
                   this->blob_bottom_->cpu_data()[n * bottom_dim + bottom_d];
        }
        EXPECT_NEAR(value, this->blob_top_->cpu_data()[n * dim + d], 1e-4);
      }
    }
  }

  TYPED_TEST(ConvolutionLayerTest, TestSimple3DConvolution)
  {
    typedef typename TypeParam::Dtype Dtype;
    this->blob_bottom_vec_.push_back(this->blob_bottom_2_);
    this->blob_top_vec_.push_back(this->blob_top_2_);
    vector<int> bottom_shape(5);
    bottom_shape[0] = this->blob_bottom_vec_[0]->shape(0);
    bottom_shape[1] = this->blob_bottom_vec_[0]->shape(1);
    bottom_shape[2] = 5;
    bottom_shape[3] = this->blob_bottom_vec_[0]->shape(2);
    bottom_shape[4] = this->blob_bottom_vec_[0]->shape(3);
    for (int i = 0; i < this->blob_bottom_vec_.size(); ++i) {
      this->blob_bottom_vec_[i]->Reshape(bottom_shape);
      GaussianFiller(this->blob_bottom_vec_[i]);
    }
    ConvolutionLayer layer;
    layer.set_kernel_size(3);
    layer.set_stride(2);
    layer.set_num_output(4);
    layer.weight_filler_.set_type("gaussian");
    layer.bias_filler_.set_type("gaussian");
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    // Check against reference convolution.
    const Dtype* top_data;
    const Dtype* ref_top_data;
    caffe_conv(this->blob_bottom_, this->MakeReferenceTop(this->blob_top_), layer);
    top_data = this->blob_top_->cpu_data();
    ref_top_data = this->ref_blob_top_->cpu_data();
    for (int i = 0; i < this->blob_top_->count(); ++i) {
      EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
    }
    caffe_conv(this->blob_bottom_2_, this->MakeReferenceTop(this->blob_top_2_), layer);
    top_data = this->blob_top_2_->cpu_data();
    ref_top_data = this->ref_blob_top_->cpu_data();
    for (int i = 0; i < this->blob_top_->count(); ++i) {
      EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
    }
  }

  TYPED_TEST(ConvolutionLayerTest, TestDilated3DConvolution)
  {
    typedef typename TypeParam::Dtype Dtype;
    this->blob_bottom_vec_.push_back(this->blob_bottom_2_);
    this->blob_top_vec_.push_back(this->blob_top_2_);
    vector<int> bottom_shape(5);
    bottom_shape[0] = this->blob_bottom_vec_[0]->shape(0);
    bottom_shape[1] = this->blob_bottom_vec_[0]->shape(1);
    bottom_shape[2] = 6;
    bottom_shape[3] = 7;
    bottom_shape[4] = 8;
    for (int i = 0; i < this->blob_bottom_vec_.size(); ++i) {
      this->blob_bottom_vec_[i]->Reshape(bottom_shape);
      GaussianFiller(this->blob_bottom_vec_[i]);
    }
    ConvolutionLayer layer;
    layer.set_kernel_size(3);
    layer.set_dilation(2);
    layer.set_num_output(4);
    layer.weight_filler_.set_type("gaussian");
    layer.bias_filler_.set_type("gaussian");
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    // Check against reference convolution.
    const Dtype* top_data;
    const Dtype* ref_top_data;
    caffe_conv(this->blob_bottom_, this->MakeReferenceTop(this->blob_top_), layer);
    top_data = this->blob_top_->cpu_data();
    ref_top_data = this->ref_blob_top_->cpu_data();
    for (int i = 0; i < this->blob_top_->count(); ++i) {
      EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
    }
    caffe_conv(this->blob_bottom_2_, this->MakeReferenceTop(this->blob_top_2_), layer);
    top_data = this->blob_top_2_->cpu_data();
    ref_top_data = this->ref_blob_top_->cpu_data();
    for (int i = 0; i < this->blob_top_->count(); ++i) {
      EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
    }
  }

  TYPED_TEST(ConvolutionLayerTest, Test1x1Convolution)
  {
    typedef typename TypeParam::Dtype Dtype;
    ConvolutionLayer layer;
    layer.set_kernel_size(1);
    layer.set_stride(1);
    layer.set_num_output(4);
    layer.weight_filler_.set_type("gaussian");
    layer.bias_filler_.set_type("constant");
    layer.bias_filler_.set_value(0.1);
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    // Check against reference convolution.
    const Dtype* top_data;
    const Dtype* ref_top_data;
    caffe_conv(this->blob_bottom_, this->MakeReferenceTop(this->blob_top_), layer);
    top_data = this->blob_top_->cpu_data();
    ref_top_data = this->ref_blob_top_->cpu_data();
    for (int i = 0; i < this->blob_top_->count(); ++i) {
      EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
    }
  }

  TYPED_TEST(ConvolutionLayerTest, TestSimpleConvolutionGroup)
  {
    typedef typename TypeParam::Dtype Dtype;
    ConvolutionLayer layer;
    layer.set_kernel_size(3);
    layer.set_stride(2);
    layer.set_num_output(3);
    layer.group_ = (3);
    layer.weight_filler_.set_type("gaussian");
    layer.bias_filler_.set_type("constant");
    layer.bias_filler_.set_value(0.1);
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    // Check against reference convolution.
    const Dtype* top_data;
    const Dtype* ref_top_data;
    caffe_conv(this->blob_bottom_, this->MakeReferenceTop(this->blob_top_), layer);
    top_data = this->blob_top_->cpu_data();
    ref_top_data = this->ref_blob_top_->cpu_data();
    for (int i = 0; i < this->blob_top_->count(); ++i) {
      EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
    }
  }

  TYPED_TEST(ConvolutionLayerTest, TestSobelConvolution)
  {
    // Test separable convolution by computing the Sobel operator
    // as a single filter then comparing the result
    // as the convolution of two rectangular filters.
    typedef typename TypeParam::Dtype Dtype;
    // Fill bottoms with identical Gaussian noise.;
    GaussianFiller(this->blob_bottom_);
    this->blob_bottom_2_->CopyFrom(this->blob_bottom_);
    // Compute Sobel G_x operator as 3 x 3 convolution.
    ConvolutionLayer layer;
    layer.set_kernel_size(3);
    layer.set_stride(2);
    layer.set_num_output(1);
    layer.set_bias_term(false);
    blobs_reset(layer.blobs_, 1);
    layer.blobs_[0]->Reshape(1, 3, 3, 3);
    Dtype* weights = layer.blobs_[0]->cpu_mdata();
    for (int c = 0; c < 3; ++c) {
      int i = c * 9;  // 3 x 3 filter
      weights[i +  0] = -1;
      weights[i +  1] =  0;
      weights[i +  2] =  1;
      weights[i +  3] = -2;
      weights[i +  4] =  0;
      weights[i +  5] =  2;
      weights[i +  6] = -1;
      weights[i +  7] =  0;
      weights[i +  8] =  1;
    }
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    // Compute Sobel G_x operator as separable 3 x 1 and 1 x 3 convolutions.
    // (1) the [1 2 1] column filter
    vector<Blob*> sep_blob_bottom_vec;
    vector<Blob*> sep_blob_top_vec;
    Blob* blob_sep = (new Blob());
    sep_blob_bottom_vec.push_back(this->blob_bottom_2_);
    sep_blob_top_vec.push_back(this->blob_top_2_);
    layer.set_kernel(3, 1);
    layer.set_stride(2, 1);
    layer.set_num_output(1);
    layer.set_bias_term(false);
    
    blobs_reset(layer.blobs_, 1);
    layer.blobs_[0]->Reshape(1, 3, 3, 1);
    Dtype* weights_1 = layer.blobs_[0]->cpu_mdata();
    for (int c = 0; c < 3; ++c) {
      int i = c * 3;  // 3 x 1 filter
      weights_1[i +  0] = 1;
      weights_1[i +  1] = 2;
      weights_1[i +  2] = 1;
    }
    layer.SetUp(sep_blob_bottom_vec, sep_blob_top_vec);
    layer.Forward(sep_blob_bottom_vec, sep_blob_top_vec);
    // (2) the [-1 0 1] row filter
    blob_sep->CopyFrom(this->blob_top_2_, false, true);
    sep_blob_bottom_vec.clear();
    sep_blob_bottom_vec.push_back(blob_sep);
    layer.set_kernel(1, 3);
    layer.set_stride(1, 2);
    layer.set_num_output(1);
    layer.set_bias_term(false);
    
    blobs_reset(layer.blobs_, 1);
    layer.blobs_[0]->Reshape(1, 1, 1, 3);
    Dtype* weights_2 = layer.blobs_[0]->cpu_mdata();
    weights_2[0] = -1;
    weights_2[1] =  0;
    weights_2[2] =  1;
    layer.SetUp(sep_blob_bottom_vec, sep_blob_top_vec);
    layer.Forward(sep_blob_bottom_vec, sep_blob_top_vec);
    // Test equivalence of full and separable filters.
    const Dtype* top_data = this->blob_top_->cpu_data();
    const Dtype* sep_top_data = this->blob_top_2_->cpu_data();
    for (int i = 0; i < this->blob_top_->count(); ++i) {
      EXPECT_NEAR(top_data[i], sep_top_data[i], 1e-4);
    }
  }

  TYPED_TEST(ConvolutionLayerTest, TestNDAgainst2D)
  {
    typedef typename TypeParam::Dtype Dtype;
    const int kernel_h = 11;
    const int kernel_w = 13;
    vector<int> bottom_shape(4);
    bottom_shape[0] = 15;
    bottom_shape[1] = 18;
    bottom_shape[2] = kernel_h * 2;
    bottom_shape[3] = kernel_w * 2;
    for (int i = 0; i < this->blob_bottom_vec_.size(); ++i) {
      this->blob_bottom_vec_[i]->Reshape(bottom_shape);
      GaussianFiller(this->blob_bottom_vec_[i]);
    }
    ConvolutionLayer layer;
    layer.set_num_output(12);
    layer.set_bias_term(false);
    layer.group_ = (6);
    layer.set_kernel(kernel_h, kernel_w);
    layer.weight_filler_.set_type("gaussian");
    Blob weights;
    Blob top_diff;
    // Shape and fill weights and top_diff.
    bool copy_diff;
    bool reshape;
    {
      
      layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
      top_diff.ReshapeLike(*this->blob_top_);
      GaussianFiller(&top_diff);
      ASSERT_EQ(1, layer.blobs_.size());
      copy_diff = false;
      reshape = true;
      weights.CopyFrom(layer.blobs_[0], copy_diff, reshape);
    }
    
    Blob result_2d;
    Blob backward_result_2d;
    Blob backward_weight_result_2d;
    // Test with 2D im2col
    {
      caffe_set(this->blob_top_->count(), Dtype(0),
                this->blob_top_->cpu_mdata());
      caffe_set(this->blob_bottom_->count(), Dtype(0),
                this->blob_bottom_->cpu_mdiff());
      caffe_set(weights.count(), Dtype(0), weights.cpu_mdiff());
      // Do SetUp and Forward; save Forward result in result_2d.
      layer.set_force_nd_im2col(false);
      ConvolutionLayer layer_2d(layer);
      layer_2d.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
      ASSERT_EQ(1, layer_2d.blobs_.size());
      copy_diff = false;
      reshape = false;
      layer_2d.blobs_[0]->CopyFrom(&weights, copy_diff, reshape);
      layer_2d.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
      copy_diff = false;
      reshape = true;
      result_2d.CopyFrom(this->blob_top_, copy_diff, reshape);
      // Copy pre-generated top diff into actual top diff;
      // do Backward and save result in backward_result_2d.
      ASSERT_EQ(this->blob_top_->shape(), top_diff.shape());
      caffe_copy(top_diff.count(), top_diff.cpu_data(),
                 this->blob_top_->cpu_mdiff());
      layer_2d.Backward(this->blob_top_vec_, this->blob_bottom_vec_);
      copy_diff = true;
      reshape = true;
      backward_result_2d.CopyFrom(this->blob_bottom_, copy_diff, reshape);
      backward_weight_result_2d.CopyFrom(&weights, copy_diff, reshape);
    }
    Blob result_nd;
    Blob backward_result_nd;
    Blob backward_weight_result_nd;
    // Test with ND im2col
    {
      caffe_set(this->blob_top_->count(), Dtype(0),
                this->blob_top_->cpu_mdata());
      caffe_set(this->blob_bottom_->count(), Dtype(0),
                this->blob_bottom_->cpu_mdiff());
      caffe_set(weights.count(), Dtype(0), weights.cpu_mdiff());
      // Do SetUp and Forward; save Forward result in result_nd.
      layer.set_force_nd_im2col(true);
      ConvolutionLayer layer_nd(layer);
      layer_nd.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
      ASSERT_EQ(1, layer_nd.blobs_.size());
      copy_diff = false;
      reshape = false;
      layer_nd.blobs_[0]->CopyFrom(&weights, copy_diff, reshape);
      layer_nd.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
      copy_diff = false;
      reshape = true;
      result_nd.CopyFrom(this->blob_top_, copy_diff, reshape);
      // Copy pre-generated top diff into actual top diff;
      // do Backward and save result in backward_result_nd.
      ASSERT_EQ(this->blob_top_->shape(), top_diff.shape());
      caffe_copy(top_diff.count(), top_diff.cpu_data(),
                 this->blob_top_->cpu_mdiff());
      layer_nd.Backward(this->blob_top_vec_, 
                        this->blob_bottom_vec_);
      copy_diff = true;
      reshape = true;
      backward_result_nd.CopyFrom(this->blob_bottom_, copy_diff, reshape);
      backward_weight_result_nd.CopyFrom(&weights, copy_diff, reshape);
    }
    ASSERT_EQ(result_nd.count(), result_2d.count());
    for (int i = 0; i < result_2d.count(); ++i)  {
      EXPECT_EQ(result_2d.cpu_data()[i], result_nd.cpu_data()[i]);
    }
    ASSERT_EQ(backward_result_nd.count(), backward_result_2d.count());
    for (int i = 0; i < backward_result_2d.count(); ++i) {
      EXPECT_EQ(backward_result_2d.cpu_diff()[i],
                backward_result_nd.cpu_diff()[i]);
    }
    ASSERT_EQ(backward_weight_result_nd.count(),
              backward_weight_result_2d.count());
    for (int i = 0; i < backward_weight_result_2d.count(); ++i) {
      EXPECT_EQ(backward_weight_result_2d.cpu_diff()[i],
                backward_weight_result_nd.cpu_diff()[i]);
    }
  }

  TYPED_TEST(ConvolutionLayerTest, TestGradient)
  {
    typedef typename TypeParam::Dtype Dtype;
    ConvolutionLayer layer;
    this->blob_bottom_vec_.push_back(this->blob_bottom_2_);
    this->blob_top_vec_.push_back(this->blob_top_2_);
    layer.set_kernel_size(3);
    layer.set_stride(2);
    layer.set_num_output(2);
    layer.weight_filler_.set_type("gaussian");
    layer.bias_filler_.set_type("gaussian");
    GradientChecker checker(1e-2, 1e-3);
    checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
                                    this->blob_top_vec_);
  }

  TYPED_TEST(ConvolutionLayerTest, TestDilatedGradient)
  {
    typedef typename TypeParam::Dtype Dtype;
    ConvolutionLayer layer;
    vector<int> bottom_shape;
    bottom_shape.push_back(2);
    bottom_shape.push_back(3);
    bottom_shape.push_back(5);
    bottom_shape.push_back(6);
    for (int i = 0; i < this->blob_bottom_vec_.size(); ++i) {
      this->blob_bottom_vec_[i]->Reshape(bottom_shape);
    }
    layer.set_kernel_size(3);
    layer.set_dilation(2);
    layer.set_num_output(2);
    layer.weight_filler_.set_type("gaussian");
    layer.bias_filler_.set_type("gaussian");
    
    GradientChecker checker(1e-2, 1e-3);
    checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
                                    this->blob_top_vec_);
  }

  TYPED_TEST(ConvolutionLayerTest, TestGradient3D)
  {
    typedef typename TypeParam::Dtype Dtype;
    ConvolutionLayer layer;
    vector<int> bottom_shape(5);
    bottom_shape[0] = this->blob_bottom_vec_[0]->shape(0);
    bottom_shape[1] = this->blob_bottom_vec_[0]->shape(1);
    bottom_shape[2] = 5;
    bottom_shape[3] = this->blob_bottom_vec_[0]->shape(2);
    bottom_shape[4] = this->blob_bottom_vec_[0]->shape(3);
    for (int i = 0; i < this->blob_bottom_vec_.size(); ++i) {
      this->blob_bottom_vec_[i]->Reshape(bottom_shape);
      GaussianFiller(this->blob_bottom_vec_[i]);
    }
    layer.set_kernel_size(3);
    layer.set_stride(2);
    layer.set_num_output(2);
    layer.weight_filler_.set_type("gaussian");
    layer.bias_filler_.set_type("gaussian");
    
    GradientChecker checker(1e-2, 1e-3);
    checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
                                    this->blob_top_vec_);
  }

  TYPED_TEST(ConvolutionLayerTest, Test1x1Gradient)
  {
    typedef typename TypeParam::Dtype Dtype;
    ConvolutionLayer layer;
    this->blob_bottom_vec_.push_back(this->blob_bottom_2_);
    this->blob_top_vec_.push_back(this->blob_top_2_);
    layer.set_kernel_size(1);
    layer.set_stride(1);
    layer.set_num_output(2);
    layer.weight_filler_.set_type("gaussian");
    layer.bias_filler_.set_type("gaussian");
    
    GradientChecker checker(1e-2, 1e-3);
    checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
                                    this->blob_top_vec_);
  }

  TYPED_TEST(ConvolutionLayerTest, TestGradientGroup)
  {
    typedef typename TypeParam::Dtype Dtype;
    ConvolutionLayer layer;
    layer.set_kernel_size(3);
    layer.set_stride(2);
    layer.set_num_output(3);
    layer.group_ = (3);
    layer.weight_filler_.set_type("gaussian");
    layer.bias_filler_.set_type("gaussian");
    
    GradientChecker checker(1e-2, 1e-3);
    checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
                                    this->blob_top_vec_);
  }

#ifdef USE_CUDNN

  template <typename Dtype>
  class CuDNNConvolutionLayerTest : public GPUDeviceTest
  {
  protected:
    CuDNNConvolutionLayerTest()
      : blob_bottom_(new Blob(2, 3, 6, 4)),
        blob_bottom_2_(new Blob(2, 3, 6, 4)),
        blob_top_(new Blob()),
        blob_top_2_(new Blob()) {}
    virtual void SetUp() {
      // fill the values
      FillerParameter filler_param;
      filler_param.set_value(1.);
      GaussianFiller filler(filler_param);
      filler.Fill(this->blob_bottom_);
      filler.Fill(this->blob_bottom_2_);
      blob_bottom_vec_.push_back(blob_bottom_);
      blob_top_vec_.push_back(blob_top_);
    }

    virtual ~CuDNNConvolutionLayerTest() {
      delete blob_bottom_;
      delete blob_bottom_2_;
      delete blob_top_;
      delete blob_top_2_;
    }

    virtual Blob* MakeReferenceTop(Blob* top) {
      this->ref_blob_top_.reset(new Blob());
      this->ref_blob_top_->ReshapeLike(*top);
      return this->ref_blob_top_;
    }

    Blob* const blob_bottom_;
    Blob* const blob_bottom_2_;
    Blob* const blob_top_;
    Blob* const blob_top_2_;
    SHARED_PTR<Blob > ref_blob_top_;
    vector<Blob*> blob_bottom_vec_;
    vector<Blob*> blob_top_vec_;
  };

  TYPED_TEST_CASE(CuDNNConvolutionLayerTest, TestDtypes);

  TYPED_TEST(CuDNNConvolutionLayerTest, TestSetupCuDNN)
  {
    this->blob_bottom_vec_.push_back(this->blob_bottom_2_);
    this->blob_top_vec_.push_back(this->blob_top_2_);
    ConvolutionLayer layer;
    ConvolutionParameter* convolution_param =
      layer.convolution_param();
    layer.set_kernel_size(3);
    layer.set_stride(2);
    layer.set_num_output(4);
    this->blob_bottom_vec_.push_back(this->blob_bottom_2_);
    this->blob_top_vec_.push_back(this->blob_top_2_);
    SHARED_PTR<Layer<TypeParam> > layer(
      new CuDNNConvolutionLayer<TypeParam>(layer));
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    EXPECT_EQ(this->blob_top_->num(), 2);
    EXPECT_EQ(this->blob_top_->channels(), 4);
    EXPECT_EQ(this->blob_top_->height(), 2);
    EXPECT_EQ(this->blob_top_->width(), 1);
    EXPECT_EQ(this->blob_top_2_->num(), 2);
    EXPECT_EQ(this->blob_top_2_->channels(), 4);
    EXPECT_EQ(this->blob_top_2_->height(), 2);
    EXPECT_EQ(this->blob_top_2_->width(), 1);
    // setting group should not change the shape
    layer.set_num_output(3);
    layer.group_ = (3);
    layer.reset(new CuDNNConvolutionLayer<TypeParam>(layer));
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    EXPECT_EQ(this->blob_top_->num(), 2);
    EXPECT_EQ(this->blob_top_->channels(), 3);
    EXPECT_EQ(this->blob_top_->height(), 2);
    EXPECT_EQ(this->blob_top_->width(), 1);
    EXPECT_EQ(this->blob_top_2_->num(), 2);
    EXPECT_EQ(this->blob_top_2_->channels(), 3);
    EXPECT_EQ(this->blob_top_2_->height(), 2);
    EXPECT_EQ(this->blob_top_2_->width(), 1);
  }

  TYPED_TEST(CuDNNConvolutionLayerTest, TestSimpleConvolutionCuDNN)
  {
    this->blob_bottom_vec_.push_back(this->blob_bottom_2_);
    this->blob_top_vec_.push_back(this->blob_top_2_);
    ConvolutionLayer layer;
    ConvolutionParameter* convolution_param =
      layer.convolution_param();
    layer.set_kernel_size(3);
    layer.set_stride(2);
    layer.set_num_output(4);
    layer.weight_filler_.set_type("gaussian");
    layer.bias_filler_.set_type("constant");
    layer.bias_filler_.set_value(0.1);
    SHARED_PTR<Layer<TypeParam> > layer(
      new CuDNNConvolutionLayer<TypeParam>(layer));
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    // Check against reference convolution.
    const TypeParam* top_data;
    const TypeParam* ref_top_data;
    caffe_conv(this->blob_bottom_, this->MakeReferenceTop(this->blob_top_), layer);
    top_data = this->blob_top_->cpu_data();
    ref_top_data = this->ref_blob_top_->cpu_data();
    for (int i = 0; i < this->blob_top_->count(); ++i) {
      EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
    }
    caffe_conv(this->blob_bottom_2_, this->MakeReferenceTop(this->blob_top_2_), layer);
    top_data = this->blob_top_2_->cpu_data();
    ref_top_data = this->ref_blob_top_->cpu_data();
    for (int i = 0; i < this->blob_top_->count(); ++i) {
      EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
    }
  }

  TYPED_TEST(CuDNNConvolutionLayerTest, TestSimpleConvolutionGroupCuDNN)
  {
    ConvolutionLayer layer;
    ConvolutionParameter* convolution_param =
      layer.convolution_param();
    layer.set_kernel_size(3);
    layer.set_stride(2);
    layer.set_num_output(3);
    layer.group_ = (3);
    layer.weight_filler_.set_type("gaussian");
    layer.bias_filler_.set_type("constant");
    layer.bias_filler_.set_value(0.1);
    SHARED_PTR<Layer<TypeParam> > layer(
      new CuDNNConvolutionLayer<TypeParam>(layer));
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    // Check against reference convolution.
    const TypeParam* top_data;
    const TypeParam* ref_top_data;
    caffe_conv(this->blob_bottom_, this->MakeReferenceTop(this->blob_top_), layer);
    top_data = this->blob_top_->cpu_data();
    ref_top_data = this->ref_blob_top_->cpu_data();
    for (int i = 0; i < this->blob_top_->count(); ++i) {
      EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
    }
  }

  TYPED_TEST(CuDNNConvolutionLayerTest, TestSobelConvolutionCuDNN)
  {
    // Test separable convolution by computing the Sobel operator
    // as a single filter then comparing the result
    // as the convolution of two rectangular filters.
    // Fill bottoms with identical Gaussian noise.
    SHARED_PTR<GaussianFiller<TypeParam> > filler;
    FillerParameter filler_param;
    filler_param.set_value(1.);
    filler.reset(new GaussianFiller<TypeParam>(filler_param));
    filler->Fill(this->blob_bottom_);
    this->blob_bottom_2_->CopyFrom(this->blob_bottom_);
    // Compute Sobel G_x operator as 3 x 3 convolution.
    ConvolutionLayer layer;
    ConvolutionParameter* convolution_param =
      layer.convolution_param();
    layer.set_kernel_size(3);
    layer.set_stride(2);
    layer.set_num_output(1);
    layer.set_bias_term(false);
    SHARED_PTR<Layer<TypeParam> > layer(
      new CuDNNConvolutionLayer<TypeParam>(layer));
    layer.blobs_.resize(1);
    layer.blobs_[0].reset(new Blob<TypeParam>(1, 3, 3, 3));
    TypeParam* weights = layer.blobs_[0]->cpu_mdata();
    for (int c = 0; c < 3; ++c) {
      int i = c * 9;  // 3 x 3 filter
      weights[i +  0] = -1;
      weights[i +  1] =  0;
      weights[i +  2] =  1;
      weights[i +  3] = -2;
      weights[i +  4] =  0;
      weights[i +  5] =  2;
      weights[i +  6] = -1;
      weights[i +  7] =  0;
      weights[i +  8] =  1;
    }
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    // Compute Sobel G_x operator as separable 3 x 1 and 1 x 3 convolutions.
    // (1) the [1 2 1] column filter
    vector<Blob<TypeParam>*> sep_blob_bottom_vec;
    vector<Blob<TypeParam>*> sep_blob_top_vec;
    SHARED_PTR<Blob<TypeParam> > blob_sep(new Blob<TypeParam>());
    sep_blob_bottom_vec.push_back(this->blob_bottom_2_);
    sep_blob_top_vec.push_back(this->blob_top_2_);
    layer.clear_kernel_size();
    layer.clear_stride();
    layer.set_kernel_h(3);
    layer.set_kernel_w(1);
    layer.set_stride_h(2);
    layer.set_stride_w(1);
    layer.set_num_output(1);
    layer.set_bias_term(false);
    layer.reset(new CuDNNConvolutionLayer<TypeParam>(layer));
    layer.blobs_.resize(1);
    layer.blobs_[0].reset(new Blob<TypeParam>(1, 3, 3, 1));
    TypeParam* weights_1 = layer.blobs_[0]->cpu_mdata();
    for (int c = 0; c < 3; ++c) {
      int i = c * 3;  // 3 x 1 filter
      weights_1[i +  0] = 1;
      weights_1[i +  1] = 2;
      weights_1[i +  2] = 1;
    }
    layer.SetUp(sep_blob_bottom_vec, sep_blob_top_vec);
    layer.Forward(sep_blob_bottom_vec, sep_blob_top_vec);
    // (2) the [-1 0 1] row filter
    blob_sep->CopyFrom(this->blob_top_2_, false, true);
    sep_blob_bottom_vec.clear();
    sep_blob_bottom_vec.push_back(blob_sep);
    layer.set_kernel_h(1);
    layer.set_kernel_w(3);
    layer.set_stride_h(1);
    layer.set_stride_w(2);
    layer.set_num_output(1);
    layer.set_bias_term(false);
    layer.reset(new CuDNNConvolutionLayer<TypeParam>(layer));
    layer.blobs_.resize(1);
    layer.blobs_[0].reset(new Blob<TypeParam>(1, 1, 1, 3));
    TypeParam* weights_2 = layer.blobs_[0]->cpu_mdata();
    weights_2[0] = -1;
    weights_2[1] =  0;
    weights_2[2] =  1;
    layer.SetUp(sep_blob_bottom_vec, sep_blob_top_vec);
    layer.Forward(sep_blob_bottom_vec, sep_blob_top_vec);
    // Test equivalence of full and separable filters.
    const TypeParam* top_data = this->blob_top_->cpu_data();
    const TypeParam* sep_top_data = this->blob_top_2_->cpu_data();
    for (int i = 0; i < this->blob_top_->count(); ++i) {
      EXPECT_NEAR(top_data[i], sep_top_data[i], 1e-4);
    }
  }

  TYPED_TEST(CuDNNConvolutionLayerTest, TestGradientCuDNN)
  {
    ConvolutionLayer layer;
    ConvolutionParameter* convolution_param =
      layer.convolution_param();
    this->blob_bottom_vec_.push_back(this->blob_bottom_2_);
    this->blob_top_vec_.push_back(this->blob_top_2_);
    layer.set_kernel_size(3);
    layer.set_stride(2);
    layer.set_num_output(2);
    layer.weight_filler_.set_type("gaussian");
    layer.bias_filler_.set_type("gaussian");
    CuDNNConvolutionLayer<TypeParam> layer(layer);
    GradientChecker<TypeParam> checker(1e-2, 1e-3);
    checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
                                    this->blob_top_vec_);
  }

  TYPED_TEST(CuDNNConvolutionLayerTest, TestGradientGroupCuDNN)
  {
    ConvolutionLayer layer;
    ConvolutionParameter* convolution_param =
      layer.convolution_param();
    layer.set_kernel_size(3);
    layer.set_stride(2);
    layer.set_num_output(3);
    layer.group_ = (3);
    layer.weight_filler_.set_type("gaussian");
    layer.bias_filler_.set_type("gaussian");
    CuDNNConvolutionLayer<TypeParam> layer(layer);
    GradientChecker<TypeParam> checker(1e-2, 1e-3);
    checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
                                    this->blob_top_vec_);
  }

#endif
