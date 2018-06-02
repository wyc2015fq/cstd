

// Reference convolution for checking results:
// accumulate through explicit loops over input, output, and filters.

void caffe_conv(const pBox* in, pBox* out, Dtype* weights,
                int kernel_h, int kernel_w,
                int pad_h, int pad_w,
                int stride_h, int stride_w,
                int dilation_h, int dilation_w,
                int groups = 1,
                Dtype* bias_data = NULL
                ) {
  // Groups
  int o_c = pBox_channels(out);
  int o_g = pBox_channels(out) / groups;
  int k_g = pBox_channels(in) / groups;
  int o_head, k_head;
  // Convolution
  Dtype* out_data = pBox_pf(out);
  const Dtype* in_data = pBox_pf(in);
  for (int n = 0; n < out->f; n++) {
    for (int g = 0; g < groups; g++) {
      o_head = o_g * g;
      k_head = k_g * g;
      for (int o = 0; o < o_g; o++) {
        for (int k = 0; k < k_g; k++) {
          for (int y = 0; y < out->h; y++) {
            for (int x = 0; x < out->w; x++) {
              int out_offset = ((n*o_g+o + o_head)*out->h+y)*out->w+x;
              for (int p = 0; p < kernel_h; p++) {
                for (int q = 0; q < kernel_w; q++) {
                  int in_y = y * stride_h - pad_h + p * dilation_h;
                  int in_x = x * stride_w - pad_w + q * dilation_w;
                  if (is_a_ge_zero_and_a_lt_b(in_y, in->h) && is_a_ge_zero_and_a_lt_b(in_x, in->w)) {
                    int weight_offset = (((o + o_head)*k_g+k)*kernel_h+p)*kernel_w+q;
                    int in_offset = ((n*k_g+ k + k_head)*in->h+in_y)*in->w+in_x;
                    out_data[out_offset] += in_data[in_offset] * weights[weight_offset];
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
  if (bias_data) {
    for (int n = 0; n < out->f; n++) {
      for (int o = 0; o < o_c; o++) {
        for (int y = 0; y < out->h; y++) {
          for (int x = 0; x < out->w; x++) {
            int out_offset = ((n*o_c+o)*out->h+y)*out->w+x;
            out_data[out_offset] += bias_data[o];
          }
        }
      }
    }
  }
}

int ConvolutionLayerTest(Net* net, int* pbottom_, int* pbottom_2_) {
  if (pbottom_) {
    *pbottom_ = net_addBlob(net, "bottom", 4, 6, 3, 2);
    net_rng_gaussian(net, *pbottom_, 0, 1);
  }
  if (pbottom_2_) {
    *pbottom_2_ = net_addBlob(net, "bottom_2", 4, 6, 3, 2);
    net_rng_gaussian(net, *pbottom_2_, 0, 1);
  }
  return 0;
}

TYPED_TEST(ConvolutionLayerTest, TestSetup) {
  Net net[1] = {0};
  int bottom_, bottom_2_, w, b, top;
  ConvolutionLayerTest(net, &bottom_, &bottom_2_);
  top = net_conv2d(net, bottom_, &w, &b, 3, 4, 2, 0);
  img_t* top_data = BLOB(top)->data;
  EXPECT_EQ(top_data->f, 2);
  EXPECT_EQ(pBox_channels(top_data), 4);
  EXPECT_EQ(top_data->h, 2);
  EXPECT_EQ(top_data->w, 1);
  // setting group should not change the shape
  top = net_conv2d(net, bottom_, &w, &b, 3, 3, 2, 0, 3);
  top_data = BLOB(top)->data;
  EXPECT_EQ(top_data->f, 2);
  EXPECT_EQ(pBox_channels(top_data), 3);
  EXPECT_EQ(top_data->h, 2);
  EXPECT_EQ(top_data->w, 1);
  net_free(net);
  return 1;
}

TYPED_TEST(ConvolutionLayerTest, TestSimpleConvolution) {
  Net net[1] = {0};
  int bottom_, w, b, top;
  ConvolutionLayerTest(net, &bottom_, NULL);
  top = net_conv2d(net, bottom_, &w, &b, 3, 4, 2, 0);
  net_rng_gaussian(net, w, 0, 1);
  net_constant(net, b, 0.1);
  net_Forward(net);
  // Check against reference convolution.
  Layer* layer = net->layers;
  const img_t* top_data = BLOB(top)->data;
  const img_t* bottom_data = BLOB(bottom_)->data;
  const img_t* w_data = BLOB(w)->data;
  const img_t* b_data = BLOB(b)->data;
  img_t im[2] = {0};
  img_t* ref_top_data = im;
  pBox_ReshapeLike(ref_top_data, top_data);
  caffe_conv(bottom_data, ref_top_data, w_data->tt.f4, LARG(kernel_size), LARG(kernel_size),
    LARG(pad), LARG(pad), LARG(stride), LARG(stride), 1, 1, 1, b_data->tt.f4);
  for (int i = 0; i < pBox_count(top_data); ++i) {
    EXPECT_NEAR(top_data->tt.f4[i], ref_top_data->tt.f4[i], 1e-4);
  }
  imfrees2(im);
  net_free(net);
  return 1;
}

TYPED_TEST(ConvolutionLayerTest, TestGradient) {
  Net net[1] = {0};
  int bottom_, w, b, top;
  ConvolutionLayerTest(net, &bottom_, NULL);
  top = net_conv2d(net, bottom_, &w, &b, 3, 2, 2, 0);
  net_rng_gaussian(net, w, 0, 1);
  net_rng_gaussian(net, b, 0, 1);
  net_gradient_check(net, 1e-2, 1e-3);
  net_free(net);
  return 1;
}

#if 0

TYPED_TEST(ConvolutionLayerTest, TestDilatedConvolution) {
  Net net[1] = {0};
  int bottom_, w, b, top;
  ConvolutionLayerTest(net, &bottom_, NULL);
  // 7, 8, 3, 2
  top = net_conv2d(net, bottom_, &w, &b, 3, 4, 2, 0, 1, 2);



  convolution_param->mutable_weight_filler()->set_type("gaussian");
  convolution_param->mutable_bias_filler()->set_type("constant");
  convolution_param->mutable_bias_filler()->set_value(0.1);
  shared_ptr<Layer > layer(
      new ConvolutionLayer(layer_param));
  layer->SetUp(blob_bottom_vec_, blob_top_vec_);
  layer->Forward(blob_bottom_vec_, blob_top_vec_);
  // Check against reference convolution.
  const Dtype* top_data;
  const Dtype* ref_top_data;
  caffe_conv(blob_bottom_, convolution_param, layer->blobs(),
             MakeReferenceTop(blob_top_));
  top_data = blob_top_->cpu_data();
  ref_top_data = ref_blob_top_->cpu_data();
  for (int i = 0; i < blob_top_->count(); ++i) {
    EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
  }
  caffe_conv(blob_bottom_2_, convolution_param, layer->blobs(),
             MakeReferenceTop(blob_top_2_));
  top_data = blob_top_2_->cpu_data();
  ref_top_data = ref_blob_top_->cpu_data();
  for (int i = 0; i < blob_top_->count(); ++i) {
    EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
  }
}

TYPED_TEST(ConvolutionLayerTest, Test0DConvolution) {
  typedef typename TypeParam::Dtype Dtype;
  LayerParameter layer_param;
  ConvolutionParameter* convolution_param =
      layer_param.mutable_convolution_param();
  const int kNumOutput = 3;
  convolution_param->set_num_output(kNumOutput);
  convolution_param->set_axis(3);
  convolution_param->mutable_weight_filler()->set_type("gaussian");
  convolution_param->mutable_bias_filler()->set_type("gaussian");
  shared_ptr<Layer > layer(
      new ConvolutionLayer(layer_param));
  vector<int> top_shape = blob_bottom_->shape();
  top_shape[3] = kNumOutput;
  layer->SetUp(blob_bottom_vec_, blob_top_vec_);
  EXPECT_EQ(top_shape, blob_top_->shape());
  layer->Forward(blob_bottom_vec_, blob_top_vec_);
  // Check against reference convolution.
  vector<int> weight_offset(2);
  const Blob* weight = layer->blobs()[0].get();
  const Blob* bias = layer->blobs()[1].get();
  const int num = blob_top_->count(3);
  const int dim = blob_top_->shape(3);
  const int bottom_dim = blob_bottom_->shape(3);
  for (int n = 0; n < num; ++n) {
    for (int d = 0; d < dim; ++d) {
      weight_offset[0] = d;
      Dtype value = bias->cpu_data()[d];
      for (int bottom_d = 0; bottom_d < bottom_dim; ++bottom_d) {
        weight_offset[1] = bottom_d;
        value += weight->data_at(weight_offset) *
                 blob_bottom_->cpu_data()[n * bottom_dim + bottom_d];
      }
      EXPECT_NEAR(value, blob_top_->cpu_data()[n * dim + d], 1e-4);
    }
  }
}

TYPED_TEST(ConvolutionLayerTest, TestSimple3DConvolution) {
  typedef typename TypeParam::Dtype Dtype;
  blob_bottom_vec_.push_back(blob_bottom_2_);
  blob_top_vec_.push_back(blob_top_2_);
  vector<int> bottom_shape(5);
  bottom_shape[0] = blob_bottom_vec_[0]->shape(0);
  bottom_shape[1] = blob_bottom_vec_[0]->shape(1);
  bottom_shape[2] = 5;
  bottom_shape[3] = blob_bottom_vec_[0]->shape(2);
  bottom_shape[4] = blob_bottom_vec_[0]->shape(3);
  FillerParameter filler_param;
  GaussianFiller filler(filler_param);
  for (int i = 0; i < blob_bottom_vec_.size(); ++i) {
    blob_bottom_vec_[i]->Reshape(bottom_shape);
    filler.Fill(blob_bottom_vec_[i]);
  }
  LayerParameter layer_param;
  ConvolutionParameter* convolution_param =
      layer_param.mutable_convolution_param();
  convolution_param->add_kernel_size(3);
  convolution_param->add_stride(2);
  convolution_param->set_num_output(4);
  convolution_param->mutable_weight_filler()->set_type("gaussian");
  convolution_param->mutable_bias_filler()->set_type("gaussian");
  shared_ptr<Layer > layer(
      new ConvolutionLayer(layer_param));
  layer->SetUp(blob_bottom_vec_, blob_top_vec_);
  layer->Forward(blob_bottom_vec_, blob_top_vec_);
  // Check against reference convolution.
  const Dtype* top_data;
  const Dtype* ref_top_data;
  caffe_conv(blob_bottom_, convolution_param, layer->blobs(),
      MakeReferenceTop(blob_top_));
  top_data = blob_top_->cpu_data();
  ref_top_data = ref_blob_top_->cpu_data();
  for (int i = 0; i < blob_top_->count(); ++i) {
    EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
  }
  caffe_conv(blob_bottom_2_, convolution_param, layer->blobs(),
      MakeReferenceTop(blob_top_2_));
  top_data = blob_top_2_->cpu_data();
  ref_top_data = ref_blob_top_->cpu_data();
  for (int i = 0; i < blob_top_->count(); ++i) {
    EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
  }
}

TYPED_TEST(ConvolutionLayerTest, TestDilated3DConvolution) {
  typedef typename TypeParam::Dtype Dtype;
  blob_bottom_vec_.push_back(blob_bottom_2_);
  blob_top_vec_.push_back(blob_top_2_);
  vector<int> bottom_shape(5);
  bottom_shape[0] = blob_bottom_vec_[0]->shape(0);
  bottom_shape[1] = blob_bottom_vec_[0]->shape(1);
  bottom_shape[2] = 6;
  bottom_shape[3] = 7;
  bottom_shape[4] = 8;
  FillerParameter filler_param;
  GaussianFiller filler(filler_param);
  for (int i = 0; i < blob_bottom_vec_.size(); ++i) {
    blob_bottom_vec_[i]->Reshape(bottom_shape);
    filler.Fill(blob_bottom_vec_[i]);
  }
  LayerParameter layer_param;
  ConvolutionParameter* convolution_param =
      layer_param.mutable_convolution_param();
  convolution_param->add_kernel_size(3);
  convolution_param->add_dilation(2);
  convolution_param->set_num_output(4);
  convolution_param->mutable_weight_filler()->set_type("gaussian");
  convolution_param->mutable_bias_filler()->set_type("gaussian");
  shared_ptr<Layer > layer(
      new ConvolutionLayer(layer_param));
  layer->SetUp(blob_bottom_vec_, blob_top_vec_);
  layer->Forward(blob_bottom_vec_, blob_top_vec_);
  // Check against reference convolution.
  const Dtype* top_data;
  const Dtype* ref_top_data;
  caffe_conv(blob_bottom_, convolution_param, layer->blobs(),
             MakeReferenceTop(blob_top_));
  top_data = blob_top_->cpu_data();
  ref_top_data = ref_blob_top_->cpu_data();
  for (int i = 0; i < blob_top_->count(); ++i) {
    EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
  }
  caffe_conv(blob_bottom_2_, convolution_param, layer->blobs(),
             MakeReferenceTop(blob_top_2_));
  top_data = blob_top_2_->cpu_data();
  ref_top_data = ref_blob_top_->cpu_data();
  for (int i = 0; i < blob_top_->count(); ++i) {
    EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
  }
}
#endif


TYPED_TEST(ConvolutionLayerTest, Test1x1Convolution) {
  Net net[1] = {0};
  int bottom_, w, b, top;
  ConvolutionLayerTest(net, &bottom_, NULL);
  top = net_conv2d(net, bottom_, &w, &b, 1, 4, 1);
  net_rng_gaussian(net, w, 0, 1);
  net_constant(net, b, 0.1);
  net_Forward(net);
  
  // Check against reference convolution.
  Layer* layer = net->layers;
  const img_t* top_data = BLOB(top)->data;
  const img_t* bottom_data = BLOB(bottom_)->data;
  const img_t* w_data = BLOB(w)->data;
  const img_t* b_data = BLOB(b)->data;
  img_t im[2] = {0};
  img_t* ref_top_data = im;
  pBox_ReshapeLike(ref_top_data, top_data);
  caffe_conv(bottom_data, ref_top_data, w_data->tt.f4, LARG(kernel_size), LARG(kernel_size),
    LARG(pad), LARG(pad), LARG(stride), LARG(stride), 1, 1, 1, b_data->tt.f4);
  for (int i = 0; i < pBox_count(top_data); ++i) {
    EXPECT_NEAR(top_data->tt.f4[i], ref_top_data->tt.f4[i], 1e-4);
  }
  imfrees2(im);
  net_free(net);
  return 1;
}

#if 0
TYPED_TEST(ConvolutionLayerTest, TestSimpleConvolutionGroup) {
  typedef typename TypeParam::Dtype Dtype;
  LayerParameter layer_param;
  ConvolutionParameter* convolution_param =
      layer_param.mutable_convolution_param();
  convolution_param->add_kernel_size(3);
  convolution_param->add_stride(2);
  convolution_param->set_num_output(3);
  convolution_param->set_group(3);
  convolution_param->mutable_weight_filler()->set_type("gaussian");
  convolution_param->mutable_bias_filler()->set_type("constant");
  convolution_param->mutable_bias_filler()->set_value(0.1);
  shared_ptr<Layer > layer(
      new ConvolutionLayer(layer_param));
  layer->SetUp(blob_bottom_vec_, blob_top_vec_);
  layer->Forward(blob_bottom_vec_, blob_top_vec_);
  // Check against reference convolution.
  const Dtype* top_data;
  const Dtype* ref_top_data;
  caffe_conv(blob_bottom_, convolution_param, layer->blobs(),
      MakeReferenceTop(blob_top_));
  top_data = blob_top_->cpu_data();
  ref_top_data = ref_blob_top_->cpu_data();
  for (int i = 0; i < blob_top_->count(); ++i) {
    EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
  }
}

TYPED_TEST(ConvolutionLayerTest, TestSobelConvolution) {
  // Test separable convolution by computing the Sobel operator
  // as a single filter then comparing the result
  // as the convolution of two rectangular filters.
  typedef typename TypeParam::Dtype Dtype;
  // Fill bottoms with identical Gaussian noise.
  shared_ptr<GaussianFiller > filler;
  FillerParameter filler_param;
  filler_param.set_value(1.);
  filler.reset(new GaussianFiller(filler_param));
  filler->Fill(blob_bottom_);
  blob_bottom_2_->CopyFrom(*blob_bottom_);
  // Compute Sobel G_x operator as 3 x 3 convolution.
  LayerParameter layer_param;
  ConvolutionParameter* convolution_param =
      layer_param.mutable_convolution_param();
  convolution_param->add_kernel_size(3);
  convolution_param->add_stride(2);
  convolution_param->set_num_output(1);
  convolution_param->set_bias_term(false);
  shared_ptr<Layer > layer(
      new ConvolutionLayer(layer_param));
  layer->blobs().resize(1);
  layer->blobs()[0].reset(new Blob(1, 3, 3, 3));
  Dtype* weights = layer->blobs()[0]->mutable_cpu_data();
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
  layer->SetUp(blob_bottom_vec_, blob_top_vec_);
  layer->Forward(blob_bottom_vec_, blob_top_vec_);
  // Compute Sobel G_x operator as separable 3 x 1 and 1 x 3 convolutions.
  // (1) the [1 2 1] column filter
  vector<Blob*> sep_blob_bottom_vec;
  vector<Blob*> sep_blob_top_vec;
  shared_ptr<Blob > blob_sep(new Blob());
  sep_blob_bottom_vec.push_back(blob_bottom_2_);
  sep_blob_top_vec.push_back(blob_top_2_);
  convolution_param->clear_kernel_size();
  convolution_param->clear_stride();
  convolution_param->set_kernel_h(3);
  convolution_param->set_kernel_w(1);
  convolution_param->set_stride_h(2);
  convolution_param->set_stride_w(1);
  convolution_param->set_num_output(1);
  convolution_param->set_bias_term(false);
  layer.reset(new ConvolutionLayer(layer_param));
  layer->blobs().resize(1);
  layer->blobs()[0].reset(new Blob(1, 3, 3, 1));
  Dtype* weights_1 = layer->blobs()[0]->mutable_cpu_data();
  for (int c = 0; c < 3; ++c) {
    int i = c * 3;  // 3 x 1 filter
    weights_1[i +  0] = 1;
    weights_1[i +  1] = 2;
    weights_1[i +  2] = 1;
  }
  layer->SetUp(sep_blob_bottom_vec, sep_blob_top_vec);
  layer->Forward(sep_blob_bottom_vec, sep_blob_top_vec);
  // (2) the [-1 0 1] row filter
  blob_sep->CopyFrom(*blob_top_2_, false, true);
  sep_blob_bottom_vec.clear();
  sep_blob_bottom_vec.push_back(blob_sep.get());
  convolution_param->set_kernel_h(1);
  convolution_param->set_kernel_w(3);
  convolution_param->set_stride_h(1);
  convolution_param->set_stride_w(2);
  convolution_param->set_num_output(1);
  convolution_param->set_bias_term(false);
  layer.reset(new ConvolutionLayer(layer_param));
  layer->blobs().resize(1);
  layer->blobs()[0].reset(new Blob(1, 1, 1, 3));
  Dtype* weights_2 = layer->blobs()[0]->mutable_cpu_data();
  weights_2[0] = -1;
  weights_2[1] =  0;
  weights_2[2] =  1;
  layer->SetUp(sep_blob_bottom_vec, sep_blob_top_vec);
  layer->Forward(sep_blob_bottom_vec, sep_blob_top_vec);
  // Test equivalence of full and separable filters.
  const Dtype* top_data = blob_top_->cpu_data();
  const Dtype* sep_top_data = blob_top_2_->cpu_data();
  for (int i = 0; i < blob_top_->count(); ++i) {
    EXPECT_NEAR(top_data[i], sep_top_data[i], 1e-4);
  }
}

TYPED_TEST(ConvolutionLayerTest, TestNDAgainst2D) {
  typedef typename TypeParam::Dtype Dtype;
  const int kernel_h = 11;
  const int kernel_w = 13;
  vector<int> bottom_shape(4);
  bottom_shape[0] = 15;
  bottom_shape[1] = 18;
  bottom_shape[2] = kernel_h * 2;
  bottom_shape[3] = kernel_w * 2;
  FillerParameter filler_param;
  GaussianFiller filler(filler_param);
  for (int i = 0; i < blob_bottom_vec_.size(); ++i) {
    blob_bottom_vec_[i]->Reshape(bottom_shape);
    filler.Fill(blob_bottom_vec_[i]);
  }
  LayerParameter layer_param;
  ConvolutionParameter* convolution_param =
      layer_param.mutable_convolution_param();
  convolution_param->set_num_output(12);
  convolution_param->set_bias_term(false);
  convolution_param->set_group(6);
  convolution_param->set_kernel_h(kernel_h);
  convolution_param->set_kernel_w(kernel_w);
  convolution_param->mutable_weight_filler()->set_type("gaussian");
  Blob weights;
  Blob top_diff;
  // Shape and fill weights and top_diff.
  bool copy_diff;
  bool reshape;
  {
    ConvolutionLayer layer(layer_param);
    layer.SetUp(blob_bottom_vec_, blob_top_vec_);
    top_diff.ReshapeLike(*blob_top_);
    filler.Fill(&top_diff);
    ASSERT_EQ(1, layer.blobs().size());
    copy_diff = false; reshape = true;
    weights.CopyFrom(*layer.blobs()[0], copy_diff, reshape);
  }
  vector<bool> propagate_down(1, true);
  Blob result_2d;
  Blob backward_result_2d;
  Blob backward_weight_result_2d;
  // Test with 2D im2col
  {
    caffe_set(blob_top_->count(), Dtype(0),
              blob_top_->mutable_cpu_data());
    caffe_set(blob_bottom_->count(), Dtype(0),
              blob_bottom_->mutable_cpu_diff());
    caffe_set(weights.count(), Dtype(0), weights.mutable_cpu_diff());
    // Do SetUp and Forward; save Forward result in result_2d.
    convolution_param->set_force_nd_im2col(false);
    ConvolutionLayer layer_2d(layer_param);
    layer_2d.SetUp(blob_bottom_vec_, blob_top_vec_);
    ASSERT_EQ(1, layer_2d.blobs().size());
    copy_diff = false; reshape = false;
    layer_2d.blobs()[0]->CopyFrom(weights, copy_diff, reshape);
    layer_2d.Forward(blob_bottom_vec_, blob_top_vec_);
    copy_diff = false; reshape = true;
    result_2d.CopyFrom(*blob_top_, copy_diff, reshape);
    // Copy pre-generated top diff into actual top diff;
    // do Backward and save result in backward_result_2d.
    ASSERT_EQ(blob_top_->shape(), top_diff.shape());
    caffe_copy(top_diff.count(), top_diff.cpu_data(),
               blob_top_->mutable_cpu_diff());
    layer_2d.Backward(blob_top_vec_, propagate_down,
                      blob_bottom_vec_);
    copy_diff = true; reshape = true;
    backward_result_2d.CopyFrom(*blob_bottom_, copy_diff, reshape);
    backward_weight_result_2d.CopyFrom(weights, copy_diff, reshape);
  }
  Blob result_nd;
  Blob backward_result_nd;
  Blob backward_weight_result_nd;
  // Test with ND im2col
  {
    caffe_set(blob_top_->count(), Dtype(0),
              blob_top_->mutable_cpu_data());
    caffe_set(blob_bottom_->count(), Dtype(0),
              blob_bottom_->mutable_cpu_diff());
    caffe_set(weights.count(), Dtype(0), weights.mutable_cpu_diff());
    // Do SetUp and Forward; save Forward result in result_nd.
    convolution_param->set_force_nd_im2col(true);
    ConvolutionLayer layer_nd(layer_param);
    layer_nd.SetUp(blob_bottom_vec_, blob_top_vec_);
    ASSERT_EQ(1, layer_nd.blobs().size());
    copy_diff = false; reshape = false;
    layer_nd.blobs()[0]->CopyFrom(weights, copy_diff, reshape);
    layer_nd.Forward(blob_bottom_vec_, blob_top_vec_);
    copy_diff = false; reshape = true;
    result_nd.CopyFrom(*blob_top_, copy_diff, reshape);
    // Copy pre-generated top diff into actual top diff;
    // do Backward and save result in backward_result_nd.
    ASSERT_EQ(blob_top_->shape(), top_diff.shape());
    caffe_copy(top_diff.count(), top_diff.cpu_data(),
               blob_top_->mutable_cpu_diff());
    layer_nd.Backward(blob_top_vec_, propagate_down,
                      blob_bottom_vec_);
    copy_diff = true; reshape = true;
    backward_result_nd.CopyFrom(*blob_bottom_, copy_diff, reshape);
    backward_weight_result_nd.CopyFrom(weights, copy_diff, reshape);
  }
  ASSERT_EQ(result_nd.count(), result_2d.count());
  for (int i = 0; i < result_2d.count(); ++i)  {
    EXPECT_EQ(result_2d.cpu_data()[i], result_nd.cpu_data()[i]);
  }
  ASSERT_EQ(backward_result_nd.count(), backward_result_2d.count());
  for (int i = 0; i < backward_result_2d.count(); ++i) {
    EXPECT_FLOAT_EQ(backward_result_2d.cpu_diff()[i],
              backward_result_nd.cpu_diff()[i]);
  }
  ASSERT_EQ(backward_weight_result_nd.count(),
            backward_weight_result_2d.count());
  for (int i = 0; i < backward_weight_result_2d.count(); ++i) {
    EXPECT_EQ(backward_weight_result_2d.cpu_diff()[i],
              backward_weight_result_nd.cpu_diff()[i]);
  }
}

TYPED_TEST(ConvolutionLayerTest, TestDilatedGradient) {
  typedef typename TypeParam::Dtype Dtype;
  LayerParameter layer_param;
  ConvolutionParameter* convolution_param =
      layer_param.mutable_convolution_param();
  vector<int> bottom_shape;
  bottom_shape.push_back(2);
  bottom_shape.push_back(3);
  bottom_shape.push_back(5);
  bottom_shape.push_back(6);
  for (int i = 0; i < blob_bottom_vec_.size(); ++i) {
    blob_bottom_vec_[i]->Reshape(bottom_shape);
  }
  convolution_param->add_kernel_size(3);
  convolution_param->add_dilation(2);
  convolution_param->set_num_output(2);
  convolution_param->mutable_weight_filler()->set_type("gaussian");
  convolution_param->mutable_bias_filler()->set_type("gaussian");
  ConvolutionLayer layer(layer_param);
  GradientChecker checker(1e-2, 1e-3);
  checker.CheckGradientExhaustive(&layer, blob_bottom_vec_,
                                  blob_top_vec_);
}

TYPED_TEST(ConvolutionLayerTest, TestGradient3D) {
  typedef typename TypeParam::Dtype Dtype;
  LayerParameter layer_param;
  ConvolutionParameter* convolution_param =
      layer_param.mutable_convolution_param();
  vector<int> bottom_shape(5);
  bottom_shape[0] = blob_bottom_vec_[0]->shape(0);
  bottom_shape[1] = blob_bottom_vec_[0]->shape(1);
  bottom_shape[2] = 5;
  bottom_shape[3] = blob_bottom_vec_[0]->shape(2);
  bottom_shape[4] = blob_bottom_vec_[0]->shape(3);
  FillerParameter filler_param;
  GaussianFiller filler(filler_param);
  for (int i = 0; i < blob_bottom_vec_.size(); ++i) {
    blob_bottom_vec_[i]->Reshape(bottom_shape);
    filler.Fill(blob_bottom_vec_[i]);
  }
  convolution_param->add_kernel_size(3);
  convolution_param->add_stride(2);
  convolution_param->set_num_output(2);
  convolution_param->mutable_weight_filler()->set_type("gaussian");
  convolution_param->mutable_bias_filler()->set_type("gaussian");
  ConvolutionLayer layer(layer_param);
  GradientChecker checker(1e-2, 1e-3);
  checker.CheckGradientExhaustive(&layer, blob_bottom_vec_,
      blob_top_vec_);
}

TYPED_TEST(ConvolutionLayerTest, Test1x1Gradient) {
  typedef typename TypeParam::Dtype Dtype;
  LayerParameter layer_param;
  ConvolutionParameter* convolution_param =
      layer_param.mutable_convolution_param();
  blob_bottom_vec_.push_back(blob_bottom_2_);
  blob_top_vec_.push_back(blob_top_2_);
  convolution_param->add_kernel_size(1);
  convolution_param->add_stride(1);
  convolution_param->set_num_output(2);
  convolution_param->mutable_weight_filler()->set_type("gaussian");
  convolution_param->mutable_bias_filler()->set_type("gaussian");
  ConvolutionLayer layer(layer_param);
  GradientChecker checker(1e-2, 1e-3);
  checker.CheckGradientExhaustive(&layer, blob_bottom_vec_,
      blob_top_vec_);
}

TYPED_TEST(ConvolutionLayerTest, TestGradientGroup) {
  typedef typename TypeParam::Dtype Dtype;
  LayerParameter layer_param;
  ConvolutionParameter* convolution_param =
      layer_param.mutable_convolution_param();
  convolution_param->add_kernel_size(3);
  convolution_param->add_stride(2);
  convolution_param->set_num_output(3);
  convolution_param->set_group(3);
  convolution_param->mutable_weight_filler()->set_type("gaussian");
  convolution_param->mutable_bias_filler()->set_type("gaussian");
  ConvolutionLayer layer(layer_param);
  GradientChecker checker(1e-2, 1e-3);
  checker.CheckGradientExhaustive(&layer, blob_bottom_vec_,
      blob_top_vec_);
}

#ifdef USE_CUDNN


class CuDNNConvolutionLayerTest : public GPUDeviceTest {
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
    filler.Fill(blob_bottom_);
    filler.Fill(blob_bottom_2_);
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
    ref_blob_top_.reset(new Blob());
    ref_blob_top_->ReshapeLike(*top);
    return ref_blob_top_.get();
  }

  Blob* const blob_bottom_;
  Blob* const blob_bottom_2_;
  Blob* const blob_top_;
  Blob* const blob_top_2_;
  shared_ptr<Blob > ref_blob_top_;
  vector<Blob*> blob_bottom_vec_;
  vector<Blob*> blob_top_vec_;
};

TYPED_TEST_CASE(CuDNNConvolutionLayerTest, TestDtypes);

TYPED_TEST(CuDNNConvolutionLayerTest, TestSetupCuDNN) {
  blob_bottom_vec_.push_back(blob_bottom_2_);
  blob_top_vec_.push_back(blob_top_2_);
  LayerParameter layer_param;
  ConvolutionParameter* convolution_param =
      layer_param.mutable_convolution_param();
  convolution_param->add_kernel_size(3);
  convolution_param->add_stride(2);
  convolution_param->set_num_output(4);
  blob_bottom_vec_.push_back(blob_bottom_2_);
  blob_top_vec_.push_back(blob_top_2_);
  shared_ptr<Layer<TypeParam> > layer(
      new CuDNNConvolutionLayer<TypeParam>(layer_param));
  layer->SetUp(blob_bottom_vec_, blob_top_vec_);
  EXPECT_EQ(blob_top_->f, 2);
  EXPECT_EQ(blob_top_->channels(), 4);
  EXPECT_EQ(blob_top_->height(), 2);
  EXPECT_EQ(blob_top_->width(), 1);
  EXPECT_EQ(blob_top_2_->f, 2);
  EXPECT_EQ(blob_top_2_->channels(), 4);
  EXPECT_EQ(blob_top_2_->height(), 2);
  EXPECT_EQ(blob_top_2_->width(), 1);
  // setting group should not change the shape
  convolution_param->set_num_output(3);
  convolution_param->set_group(3);
  layer.reset(new CuDNNConvolutionLayer<TypeParam>(layer_param));
  layer->SetUp(blob_bottom_vec_, blob_top_vec_);
  EXPECT_EQ(blob_top_->f, 2);
  EXPECT_EQ(blob_top_->channels(), 3);
  EXPECT_EQ(blob_top_->height(), 2);
  EXPECT_EQ(blob_top_->width(), 1);
  EXPECT_EQ(blob_top_2_->f, 2);
  EXPECT_EQ(blob_top_2_->channels(), 3);
  EXPECT_EQ(blob_top_2_->height(), 2);
  EXPECT_EQ(blob_top_2_->width(), 1);
}

TYPED_TEST(CuDNNConvolutionLayerTest, TestSimpleConvolutionCuDNN) {
  blob_bottom_vec_.push_back(blob_bottom_2_);
  blob_top_vec_.push_back(blob_top_2_);
  LayerParameter layer_param;
  ConvolutionParameter* convolution_param =
      layer_param.mutable_convolution_param();
  convolution_param->add_kernel_size(3);
  convolution_param->add_stride(2);
  convolution_param->set_num_output(4);
  convolution_param->mutable_weight_filler()->set_type("gaussian");
  convolution_param->mutable_bias_filler()->set_type("constant");
  convolution_param->mutable_bias_filler()->set_value(0.1);
  shared_ptr<Layer<TypeParam> > layer(
      new CuDNNConvolutionLayer<TypeParam>(layer_param));
  layer->SetUp(blob_bottom_vec_, blob_top_vec_);
  layer->Forward(blob_bottom_vec_, blob_top_vec_);
  // Check against reference convolution.
  const TypeParam* top_data;
  const TypeParam* ref_top_data;
  caffe_conv(blob_bottom_, convolution_param, layer->blobs(),
      MakeReferenceTop(blob_top_));
  top_data = blob_top_->cpu_data();
  ref_top_data = ref_blob_top_->cpu_data();
  for (int i = 0; i < blob_top_->count(); ++i) {
    EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
  }
  caffe_conv(blob_bottom_2_, convolution_param, layer->blobs(),
      MakeReferenceTop(blob_top_2_));
  top_data = blob_top_2_->cpu_data();
  ref_top_data = ref_blob_top_->cpu_data();
  for (int i = 0; i < blob_top_->count(); ++i) {
    EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
  }
}

TYPED_TEST(CuDNNConvolutionLayerTest, TestSimpleConvolutionGroupCuDNN) {
  LayerParameter layer_param;
  ConvolutionParameter* convolution_param =
      layer_param.mutable_convolution_param();
  convolution_param->add_kernel_size(3);
  convolution_param->add_stride(2);
  convolution_param->set_num_output(3);
  convolution_param->set_group(3);
  convolution_param->mutable_weight_filler()->set_type("gaussian");
  convolution_param->mutable_bias_filler()->set_type("constant");
  convolution_param->mutable_bias_filler()->set_value(0.1);
  shared_ptr<Layer<TypeParam> > layer(
      new CuDNNConvolutionLayer<TypeParam>(layer_param));
  layer->SetUp(blob_bottom_vec_, blob_top_vec_);
  layer->Forward(blob_bottom_vec_, blob_top_vec_);
  // Check against reference convolution.
  const TypeParam* top_data;
  const TypeParam* ref_top_data;
  caffe_conv(blob_bottom_, convolution_param, layer->blobs(),
      MakeReferenceTop(blob_top_));
  top_data = blob_top_->cpu_data();
  ref_top_data = ref_blob_top_->cpu_data();
  for (int i = 0; i < blob_top_->count(); ++i) {
    EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
  }
}

TYPED_TEST(CuDNNConvolutionLayerTest, TestSobelConvolutionCuDNN) {
  // Test separable convolution by computing the Sobel operator
  // as a single filter then comparing the result
  // as the convolution of two rectangular filters.

  // Fill bottoms with identical Gaussian noise.
  shared_ptr<GaussianFiller<TypeParam> > filler;
  FillerParameter filler_param;
  filler_param.set_value(1.);
  filler.reset(new GaussianFiller<TypeParam>(filler_param));
  filler->Fill(blob_bottom_);
  blob_bottom_2_->CopyFrom(*blob_bottom_);
  // Compute Sobel G_x operator as 3 x 3 convolution.
  LayerParameter layer_param;
  ConvolutionParameter* convolution_param =
      layer_param.mutable_convolution_param();
  convolution_param->add_kernel_size(3);
  convolution_param->add_stride(2);
  convolution_param->set_num_output(1);
  convolution_param->set_bias_term(false);
  shared_ptr<Layer<TypeParam> > layer(
      new CuDNNConvolutionLayer<TypeParam>(layer_param));
  layer->blobs().resize(1);
  layer->blobs()[0].reset(new Blob<TypeParam>(1, 3, 3, 3));
  TypeParam* weights = layer->blobs()[0]->mutable_cpu_data();
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
  layer->SetUp(blob_bottom_vec_, blob_top_vec_);
  layer->Forward(blob_bottom_vec_, blob_top_vec_);
  // Compute Sobel G_x operator as separable 3 x 1 and 1 x 3 convolutions.
  // (1) the [1 2 1] column filter
  vector<Blob<TypeParam>*> sep_blob_bottom_vec;
  vector<Blob<TypeParam>*> sep_blob_top_vec;
  shared_ptr<Blob<TypeParam> > blob_sep(new Blob<TypeParam>());
  sep_blob_bottom_vec.push_back(blob_bottom_2_);
  sep_blob_top_vec.push_back(blob_top_2_);
  convolution_param->clear_kernel_size();
  convolution_param->clear_stride();
  convolution_param->set_kernel_h(3);
  convolution_param->set_kernel_w(1);
  convolution_param->set_stride_h(2);
  convolution_param->set_stride_w(1);
  convolution_param->set_num_output(1);
  convolution_param->set_bias_term(false);
  layer.reset(new CuDNNConvolutionLayer<TypeParam>(layer_param));
  layer->blobs().resize(1);
  layer->blobs()[0].reset(new Blob<TypeParam>(1, 3, 3, 1));
  TypeParam* weights_1 = layer->blobs()[0]->mutable_cpu_data();
  for (int c = 0; c < 3; ++c) {
    int i = c * 3;  // 3 x 1 filter
    weights_1[i +  0] = 1;
    weights_1[i +  1] = 2;
    weights_1[i +  2] = 1;
  }
  layer->SetUp(sep_blob_bottom_vec, sep_blob_top_vec);
  layer->Forward(sep_blob_bottom_vec, sep_blob_top_vec);
  // (2) the [-1 0 1] row filter
  blob_sep->CopyFrom(*blob_top_2_, false, true);
  sep_blob_bottom_vec.clear();
  sep_blob_bottom_vec.push_back(blob_sep.get());
  convolution_param->set_kernel_h(1);
  convolution_param->set_kernel_w(3);
  convolution_param->set_stride_h(1);
  convolution_param->set_stride_w(2);
  convolution_param->set_num_output(1);
  convolution_param->set_bias_term(false);
  layer.reset(new CuDNNConvolutionLayer<TypeParam>(layer_param));
  layer->blobs().resize(1);
  layer->blobs()[0].reset(new Blob<TypeParam>(1, 1, 1, 3));
  TypeParam* weights_2 = layer->blobs()[0]->mutable_cpu_data();
  weights_2[0] = -1;
  weights_2[1] =  0;
  weights_2[2] =  1;
  layer->SetUp(sep_blob_bottom_vec, sep_blob_top_vec);
  layer->Forward(sep_blob_bottom_vec, sep_blob_top_vec);
  // Test equivalence of full and separable filters.
  const TypeParam* top_data = blob_top_->cpu_data();
  const TypeParam* sep_top_data = blob_top_2_->cpu_data();
  for (int i = 0; i < blob_top_->count(); ++i) {
    EXPECT_NEAR(top_data[i], sep_top_data[i], 1e-4);
  }
}

TYPED_TEST(CuDNNConvolutionLayerTest, TestGradientCuDNN) {
  LayerParameter layer_param;
  ConvolutionParameter* convolution_param =
      layer_param.mutable_convolution_param();
  blob_bottom_vec_.push_back(blob_bottom_2_);
  blob_top_vec_.push_back(blob_top_2_);
  convolution_param->add_kernel_size(3);
  convolution_param->add_stride(2);
  convolution_param->set_num_output(2);
  convolution_param->mutable_weight_filler()->set_type("gaussian");
  convolution_param->mutable_bias_filler()->set_type("gaussian");
  CuDNNConvolutionLayer<TypeParam> layer(layer_param);
  GradientChecker<TypeParam> checker(1e-2, 1e-3);
  checker.CheckGradientExhaustive(&layer, blob_bottom_vec_,
      blob_top_vec_);
}

TYPED_TEST(CuDNNConvolutionLayerTest, TestGradientGroupCuDNN) {
  LayerParameter layer_param;
  ConvolutionParameter* convolution_param =
      layer_param.mutable_convolution_param();
  convolution_param->add_kernel_size(3);
  convolution_param->add_stride(2);
  convolution_param->set_num_output(3);
  convolution_param->set_group(3);
  convolution_param->mutable_weight_filler()->set_type("gaussian");
  convolution_param->mutable_bias_filler()->set_type("gaussian");
  CuDNNConvolutionLayer<TypeParam> layer(layer_param);
  GradientChecker<TypeParam> checker(1e-2, 1e-3);
  checker.CheckGradientExhaustive(&layer, blob_bottom_vec_,
      blob_top_vec_);
}

#endif

#endif


