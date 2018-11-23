
// Since ConvolutionLayerTest checks the shared conv/deconv code in detail,
// we'll just do a simple forward test and a gradient check.

class DeconvolutionLayerTest : public MultiDeviceTest
{
  typedef typename TypeParam::Dtype Dtype;

protected:
  DeconvolutionLayerTest()
    : blob_bottom_(new Blob(2, 3, 6, 4)),
    blob_bottom_2_(new Blob(2, 3, 6, 4)),
    blob_top_(new Blob()),
    blob_top_2_(new Blob()) {}
  virtual void SetUp() {
    // fill the values
    GaussianFiller(this->blob_bottom_);
    GaussianFiller(this->blob_bottom_2_);
    blob_bottom_vec_.push_back(blob_bottom_);
    blob_top_vec_.push_back(blob_top_);
  }

  virtual ~DeconvolutionLayerTest() {
    delete blob_bottom_;
    delete blob_bottom_2_;
    delete blob_top_;
    delete blob_top_2_;
  }

  Blob* const blob_bottom_;
  Blob* const blob_bottom_2_;
  Blob* const blob_top_;
  Blob* const blob_top_2_;
  vector<Blob*> blob_bottom_vec_;
  vector<Blob*> blob_top_vec_;
};

TYPED_TEST_CASE(DeconvolutionLayerTest, TestDtypesAndDevices);

TYPED_TEST(DeconvolutionLayerTest, TestSetup)
{
  typedef typename TypeParam::Dtype Dtype;
  DeconvolutionLayer layer;
  layer.set_kernel_size(3);
  layer.set_stride(2);
  layer.set_num_output(4);
  this->blob_bottom_vec_.push_back(this->blob_bottom_2_);
  this->blob_top_vec_.push_back(this->blob_top_2_);
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  EXPECT_EQ(this->blob_top_->num(), 2);
  EXPECT_EQ(this->blob_top_->channels(), 4);
  EXPECT_EQ(this->blob_top_->height(), 13);
  EXPECT_EQ(this->blob_top_->width(), 9);
  EXPECT_EQ(this->blob_top_2_->num(), 2);
  EXPECT_EQ(this->blob_top_2_->channels(), 4);
  EXPECT_EQ(this->blob_top_2_->height(), 13);
  EXPECT_EQ(this->blob_top_2_->width(), 9);
  // setting group should not change the shape
  blobs_reset(layer.blobs_, 0);
  layer.set_num_output(3);
  layer.set_group(3);
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  EXPECT_EQ(this->blob_top_->num(), 2);
  EXPECT_EQ(this->blob_top_->channels(), 3);
  EXPECT_EQ(this->blob_top_->height(), 13);
  EXPECT_EQ(this->blob_top_->width(), 9);
  EXPECT_EQ(this->blob_top_2_->num(), 2);
  EXPECT_EQ(this->blob_top_2_->channels(), 3);
  EXPECT_EQ(this->blob_top_2_->height(), 13);
  EXPECT_EQ(this->blob_top_2_->width(), 9);
}

TYPED_TEST(DeconvolutionLayerTest, TestSimpleDeconvolution)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_2_);
  this->blob_top_vec_.push_back(this->blob_top_2_);
  DeconvolutionLayer layer;
  layer.set_kernel_size(3);
  layer.set_stride(2);
  layer.set_num_output(4);
  layer.weight_filler_.set_type("constant");
  layer.weight_filler_.set_value(1);
  layer.bias_filler_.set_type("constant");
  layer.bias_filler_.set_value(0.1);
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  // constant-fill the bottom blobs
  ConstantFiller(this->blob_bottom_);
  ConstantFiller(this->blob_bottom_2_);
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  // simply check that accumulation works with overlapping filters
  const Dtype* top_data = this->blob_top_->cpu_data();
  for (int n = 0; n < this->blob_top_->num(); ++n) {
    for (int c = 0; c < this->blob_top_->channels(); ++c) {
      for (int h = 0; h < this->blob_top_->height(); ++h) {
        for (int w = 0; w < this->blob_top_->width(); ++w) {
          Dtype expected = Dtype(3.1);
          bool h_overlap = h % 2 == 0 && h > 0
            && h < this->blob_top_->height() - 1;
          bool w_overlap = w % 2 == 0 && w > 0
            && w < this->blob_top_->width() - 1;
          if (h_overlap && w_overlap) {
            expected += 9;
          }
          else if (h_overlap || w_overlap) {
            expected += 3;
          }
          EXPECT_NEAR(top_data[this->blob_top_->offset(n, c, h, w)],
            expected, 1e-4);
        }
      }
    }
  }
}

TYPED_TEST(DeconvolutionLayerTest, TestGradient)
{
  typedef typename TypeParam::Dtype Dtype;
  DeconvolutionLayer layer;
  this->blob_bottom_vec_.push_back(this->blob_bottom_2_);
  this->blob_top_vec_.push_back(this->blob_top_2_);
  layer.set_kernel_size(2);
  layer.set_stride(1);
  layer.set_num_output(1);
  layer.weight_filler_.set_type("gaussian");
  layer.bias_filler_.set_type("gaussian");

  GradientChecker checker(1e-2, 1e-3);
  checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
    this->blob_top_vec_);
}

TYPED_TEST(DeconvolutionLayerTest, TestNDAgainst2D)
{
  typedef typename TypeParam::Dtype Dtype;
  const int kernel_h = 11;
  const int kernel_w = 13;
  vector<int> bottom_shape(4);
  bottom_shape[0] = 15;
  bottom_shape[1] = 12;
  bottom_shape[2] = kernel_h * 2;
  bottom_shape[3] = kernel_w * 2;
  for (int i = 0; i < this->blob_bottom_vec_.size(); ++i) {
    this->blob_bottom_vec_[i]->Reshape(bottom_shape);
    GaussianFiller(this->blob_bottom_vec_[i]);
  }
  DeconvolutionLayer layer;
  layer.set_num_output(18);
  layer.set_bias_term(false);
  layer.set_group(6);
  layer.set_kernel(kernel_h, kernel_w);
  layer.weight_filler_.set_type("gaussian");
  Blob weights;
  Blob top_diff;
  // Shape and fill weights and top_diff.
  bool copy_diff;
  bool reshape;
  {
    DeconvolutionLayer layer(layer);
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    top_diff.ReshapeLike(*this->blob_top_);
    GaussianFiller(&top_diff);
    ASSERT_EQ(1, layer.blobs_.size());
    copy_diff = false;
    reshape = true;
    weights.CopyFrom(layer.blobs_[0], copy_diff, reshape);
  }
  vector<bool> propagate_down(1, true);
  Blob result_2d;
  Blob backward_result_2d;
  Blob backward_weight_result_2d;
  // Test with 2D im2col
  {
    caffe_set(this->blob_top_->count(), Dtype(0), this->blob_top_->mdata());
    caffe_set(this->blob_bottom_->count(), Dtype(0), this->blob_bottom_->mdiff());
    caffe_set(weights.count(), Dtype(0), weights.mdiff());
    // Do SetUp and Forward; save Forward result in result_2d.
    layer.set_force_nd_im2col(false);
    DeconvolutionLayer layer_2d(layer);
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
    caffe_copy(top_diff.count(), top_diff.data(),
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
    caffe_set(weights.count(), Dtype(0), weights.mdiff());
    // Do SetUp and Forward; save Forward result in result_nd.
    layer.set_force_nd_im2col(true);
    DeconvolutionLayer layer_nd(layer);
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
    caffe_copy(top_diff.count(), top_diff.data(),
      this->blob_top_->cpu_mdiff());
    layer_nd.Backward(this->blob_top_vec_, this->blob_bottom_vec_);
    copy_diff = true;
    reshape = true;
    backward_result_nd.CopyFrom(this->blob_bottom_, copy_diff, reshape);
    backward_weight_result_nd.CopyFrom(&weights, copy_diff, reshape);
  }
  ASSERT_EQ(result_nd.count(), result_2d.count());
  for (int i = 0; i < result_2d.count(); ++i) {
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

TYPED_TEST(DeconvolutionLayerTest, TestGradient3D)
{
  typedef typename TypeParam::Dtype Dtype;
  vector<int> bottom_shape(5);
  bottom_shape[0] = this->blob_bottom_vec_[0]->shape(0);
  bottom_shape[1] = this->blob_bottom_vec_[0]->shape(1);
  bottom_shape[2] = 2;
  bottom_shape[3] = 3;
  bottom_shape[4] = 2;
  for (int i = 0; i < this->blob_bottom_vec_.size(); ++i) {
    this->blob_bottom_vec_[i]->Reshape(bottom_shape);
    GaussianFiller(this->blob_bottom_vec_[i]);
  }
  DeconvolutionLayer layer;
  layer.set_kernel_size(2);
  layer.set_stride(2);
  layer.set_pad(1);
  layer.set_num_output(2);
  layer.weight_filler_.set_type("gaussian");
  layer.bias_filler_.set_type("gaussian");
  GradientChecker checker(1e-2, 1e-3);
  checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
    this->blob_top_vec_);
}
