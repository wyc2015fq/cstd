
class BiasLayerTest : public MultiDeviceTest
{
  typedef typename TypeParam::Dtype Dtype;

protected:
  BiasLayerTest()
    : blob_bottom_(new Blob(2, 3, 4, 5)),
    blob_bottom_eltwise_(new Blob(2, 3, 4, 5)),
    blob_bottom_broadcast_0_(new Blob()),
    blob_bottom_broadcast_1_(new Blob()),
    blob_bottom_broadcast_2_(new Blob()),
    blob_bottom_bias_(new Blob(vector<int>())),
    blob_top_(new Blob()) {
    set_random_seed(1701);
    vector<int> broadcast_shape(2);
    broadcast_shape[0] = 2;
    broadcast_shape[1] = 3;
    this->blob_bottom_broadcast_0_->Reshape(broadcast_shape);
    broadcast_shape[0] = 3;
    broadcast_shape[1] = 4;
    this->blob_bottom_broadcast_1_->Reshape(broadcast_shape);
    broadcast_shape[0] = 4;
    broadcast_shape[1] = 5;
    this->blob_bottom_broadcast_2_->Reshape(broadcast_shape);
    Filler filler;
    filler.type_ = FillerMethod_uniform;
    filler.min_ = (1);
    filler.max_ = (10);
    filler.Fill(this->blob_bottom_);
    filler.Fill(this->blob_bottom_eltwise_);
    filler.Fill(this->blob_bottom_broadcast_0_);
    filler.Fill(this->blob_bottom_broadcast_1_);
    filler.Fill(this->blob_bottom_broadcast_2_);
    filler.Fill(this->blob_bottom_bias_);
    blob_bottom_vec_.push_back(blob_bottom_);
    blob_top_vec_.push_back(blob_top_);
  }
  virtual ~BiasLayerTest() {
    delete blob_bottom_;
    delete blob_bottom_eltwise_;
    delete blob_bottom_broadcast_0_;
    delete blob_bottom_broadcast_1_;
    delete blob_bottom_broadcast_2_;
    delete blob_bottom_bias_;
    delete blob_top_;
  }
  Blob* const blob_bottom_;
  Blob* const blob_bottom_eltwise_;
  Blob* const blob_bottom_broadcast_0_;
  Blob* const blob_bottom_broadcast_1_;
  Blob* const blob_bottom_broadcast_2_;
  Blob* const blob_bottom_bias_;
  Blob* const blob_top_;
  vector<Blob*> blob_bottom_vec_;
  vector<Blob*> blob_top_vec_;
};

TYPED_TEST_CASE(BiasLayerTest, TestDtypesAndDevices);

TYPED_TEST(BiasLayerTest, TestForwardEltwise)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_eltwise_);
  BiasLayer layer;
  layer.axis_ = (0);
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  ASSERT_EQ(this->blob_bottom_->shape(), this->blob_top_->shape());
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  const Dtype* data = this->blob_top_->cpu_data();
  const int count = this->blob_top_->count();
  const Dtype* in_data_a = this->blob_bottom_->cpu_data();
  const Dtype* in_data_b = this->blob_bottom_eltwise_->cpu_data();
  for (int i = 0; i < count; ++i) {
    EXPECT_NEAR(data[i], in_data_a[i] + in_data_b[i], 1e-5);
  }
}

TYPED_TEST(BiasLayerTest, TestForwardEltwiseInPlace)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_top_vec_[0] = this->blob_bottom_;  // in-place computation
  Blob orig_bottom(this->blob_bottom_->shape());
  orig_bottom.CopyFrom(this->blob_bottom_);
  this->blob_bottom_vec_.push_back(this->blob_bottom_eltwise_);
  BiasLayer layer;
  layer.axis_ = (0);
  
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  const Dtype* data = this->blob_bottom_->cpu_data();
  const int count = this->blob_bottom_->count();
  const Dtype* in_data_a = orig_bottom.cpu_data();
  const Dtype* in_data_b = this->blob_bottom_eltwise_->cpu_data();
  for (int i = 0; i < count; ++i) {
    EXPECT_NEAR(data[i], in_data_a[i] + in_data_b[i], 1e-5);
  }
}

TYPED_TEST(BiasLayerTest, TestBackwardEltwiseInPlace)
{
  typedef typename TypeParam::Dtype Dtype;
  Blob orig_bottom(this->blob_bottom_->shape());
  orig_bottom.CopyFrom(this->blob_bottom_);
  this->blob_bottom_vec_.push_back(this->blob_bottom_eltwise_);
  BiasLayer layer;
  layer.axis_ = (0);
  
  Blob top_diff(this->blob_bottom_->shape());
  GaussianFiller(&top_diff, 0, 1);
  // Run forward + backward without in-place computation;
  // save resulting bottom diffs.
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  caffe_copy(top_diff.count(), top_diff.cpu_data(),
    this->blob_top_->cpu_mdiff());
  layer.Backward(this->blob_top_vec_, this->blob_bottom_vec_);
  const bool kReshape = true;
  const bool kCopyDiff = true;
  Blob orig_bottom_diff;
  orig_bottom_diff.CopyFrom(this->blob_bottom_, kCopyDiff, kReshape);
  Blob orig_bias_diff;
  orig_bias_diff.CopyFrom(this->blob_bottom_eltwise_,
    kCopyDiff, kReshape);
  // Rerun forward + backward with in-place computation;
  // check that resulting bottom diffs are the same.
  this->blob_top_vec_[0] = this->blob_bottom_;  // in-place computation
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  caffe_copy(top_diff.count(), top_diff.cpu_data(),
    this->blob_bottom_->cpu_mdiff());
  layer.Backward(this->blob_top_vec_, this->blob_bottom_vec_);
  for (int i = 0; i < this->blob_bottom_->count(); ++i) {
    EXPECT_NEAR(orig_bottom_diff.cpu_diff()[i],
      this->blob_bottom_->cpu_diff()[i], 1e-5);
  }
  for (int i = 0; i < this->blob_bottom_eltwise_->count(); ++i) {
    EXPECT_NEAR(orig_bias_diff.cpu_diff()[i],
      this->blob_bottom_eltwise_->cpu_diff()[i], 1e-5);
  }
}

TYPED_TEST(BiasLayerTest, TestForwardEltwiseWithParam)
{
  typedef typename TypeParam::Dtype Dtype;
  BiasLayer layer;
  layer.axis_ = (0);
  layer.num_axes_ = (-1);
  layer.filler_.type_ = FillerMethod_gaussian;
  
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  ASSERT_EQ(this->blob_bottom_->shape(), this->blob_top_->shape());
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  const Dtype* data = this->blob_top_->cpu_data();
  const int count = this->blob_top_->count();
  const Dtype* in_data_a = this->blob_bottom_->cpu_data();
  const Dtype* in_data_b = layer.blobs_[0]->cpu_data();
  for (int i = 0; i < count; ++i) {
    EXPECT_NEAR(data[i], in_data_a[i] + in_data_b[i], 1e-5);
  }
}

TYPED_TEST(BiasLayerTest, TestForwardBroadcastBegin)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_broadcast_0_);
  BiasLayer layer;
  layer.axis_ = (0);
  
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  ASSERT_EQ(this->blob_bottom_->shape(), this->blob_top_->shape());
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  for (int n = 0; n < this->blob_bottom_->num(); ++n) {
    for (int c = 0; c < this->blob_bottom_->channels(); ++c) {
      for (int h = 0; h < this->blob_bottom_->height(); ++h) {
        for (int w = 0; w < this->blob_bottom_->width(); ++w) {
          EXPECT_NEAR(this->blob_top_->data_at(n, c, h, w),
            this->blob_bottom_->data_at(n, c, h, w) +
            this->blob_bottom_broadcast_0_->data_at(n, c, 0, 0),
            1e-5);
        }
      }
    }
  }
}

TYPED_TEST(BiasLayerTest, TestForwardBroadcastMiddle)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_broadcast_1_);
  BiasLayer layer;
  layer.axis_ = (1);
  
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  ASSERT_EQ(this->blob_bottom_->shape(), this->blob_top_->shape());
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  for (int n = 0; n < this->blob_bottom_->num(); ++n) {
    for (int c = 0; c < this->blob_bottom_->channels(); ++c) {
      for (int h = 0; h < this->blob_bottom_->height(); ++h) {
        for (int w = 0; w < this->blob_bottom_->width(); ++w) {
          EXPECT_NEAR(this->blob_top_->data_at(n, c, h, w),
            this->blob_bottom_->data_at(n, c, h, w) +
            this->blob_bottom_broadcast_1_->data_at(c, h, 0, 0),
            1e-5);
        }
      }
    }
  }
}

TYPED_TEST(BiasLayerTest, TestForwardBroadcastMiddleInPlace)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_top_vec_[0] = this->blob_bottom_;  // in-place computation
  Blob orig_bottom(this->blob_bottom_->shape());
  orig_bottom.CopyFrom(this->blob_bottom_);
  this->blob_bottom_vec_.push_back(this->blob_bottom_broadcast_1_);
  BiasLayer layer;
  layer.axis_ = (1);
  
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  for (int n = 0; n < this->blob_bottom_->num(); ++n) {
    for (int c = 0; c < this->blob_bottom_->channels(); ++c) {
      for (int h = 0; h < this->blob_bottom_->height(); ++h) {
        for (int w = 0; w < this->blob_bottom_->width(); ++w) {
          EXPECT_NEAR(this->blob_bottom_->data_at(n, c, h, w),
            orig_bottom.data_at(n, c, h, w) +
            this->blob_bottom_broadcast_1_->data_at(c, h, 0, 0),
            1e-5);
        }
      }
    }
  }
}

TYPED_TEST(BiasLayerTest, TestBackwardBroadcastMiddleInPlace)
{
  typedef typename TypeParam::Dtype Dtype;
  Blob orig_bottom(this->blob_bottom_->shape());
  orig_bottom.CopyFrom(this->blob_bottom_);
  this->blob_bottom_vec_.push_back(this->blob_bottom_broadcast_1_);
  BiasLayer layer;
  layer.axis_ = (1);
  
  Blob top_diff(this->blob_bottom_->shape());
  GaussianFiller(&top_diff);
  // Run forward + backward without in-place computation;
  // save resulting bottom diffs.
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  caffe_copy(top_diff.count(), top_diff.cpu_data(),
    this->blob_top_->cpu_mdiff());
  layer.Backward(this->blob_top_vec_, this->blob_bottom_vec_);
  const bool kReshape = true;
  const bool kCopyDiff = true;
  Blob orig_bottom_diff;
  orig_bottom_diff.CopyFrom(this->blob_bottom_, kCopyDiff, kReshape);
  Blob orig_bias_diff;
  orig_bias_diff.CopyFrom(this->blob_bottom_broadcast_1_,
    kCopyDiff, kReshape);
  // Rerun forward + backward with in-place computation;
  // check that resulting bottom diffs are the same.
  this->blob_top_vec_[0] = this->blob_bottom_;  // in-place computation
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  caffe_copy(top_diff.count(), top_diff.cpu_data(),
    this->blob_bottom_->cpu_mdiff());
  layer.Backward(this->blob_top_vec_, this->blob_bottom_vec_);
  for (int i = 0; i < this->blob_bottom_->count(); ++i) {
    EXPECT_NEAR(orig_bottom_diff.cpu_diff()[i],
      this->blob_bottom_->cpu_diff()[i], 1e-5);
  }
  for (int i = 0; i < this->blob_bottom_broadcast_1_->count(); ++i) {
    EXPECT_NEAR(orig_bias_diff.cpu_diff()[i],
      this->blob_bottom_broadcast_1_->cpu_diff()[i], 1e-5);
  }
}

TYPED_TEST(BiasLayerTest, TestForwardBroadcastMiddleWithParam)
{
  typedef typename TypeParam::Dtype Dtype;
  BiasLayer layer;
  layer.axis_ = (1);
  layer.num_axes_ = (2);
  layer.filler_.type_ = FillerMethod_gaussian;
  
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  ASSERT_EQ(this->blob_bottom_->shape(), this->blob_top_->shape());
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  for (int n = 0; n < this->blob_bottom_->num(); ++n) {
    for (int c = 0; c < this->blob_bottom_->channels(); ++c) {
      for (int h = 0; h < this->blob_bottom_->height(); ++h) {
        for (int w = 0; w < this->blob_bottom_->width(); ++w) {
          EXPECT_NEAR(this->blob_top_->data_at(n, c, h, w),
            this->blob_bottom_->data_at(n, c, h, w) +
            layer.blobs_[0]->data_at(c, h, 0, 0), 1e-5);
        }
      }
    }
  }
}

TYPED_TEST(BiasLayerTest, TestForwardBroadcastEnd)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_broadcast_2_);
  BiasLayer layer;
  layer.axis_ = (2);
  
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  ASSERT_EQ(this->blob_bottom_->shape(), this->blob_top_->shape());
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  for (int n = 0; n < this->blob_bottom_->num(); ++n) {
    for (int c = 0; c < this->blob_bottom_->channels(); ++c) {
      for (int h = 0; h < this->blob_bottom_->height(); ++h) {
        for (int w = 0; w < this->blob_bottom_->width(); ++w) {
          EXPECT_NEAR(this->blob_top_->data_at(n, c, h, w),
            this->blob_bottom_->data_at(n, c, h, w) +
            this->blob_bottom_broadcast_2_->data_at(h, w, 0, 0),
            1e-5);
        }
      }
    }
  }
}

TYPED_TEST(BiasLayerTest, TestForwardBias)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_bias_);
  BiasLayer layer;
  
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  ASSERT_EQ(this->blob_bottom_->shape(), this->blob_top_->shape());
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  const Dtype* data = this->blob_top_->cpu_data();
  const int count = this->blob_top_->count();
  const Dtype* in_data = this->blob_bottom_->cpu_data();
  const Dtype bias = *this->blob_bottom_bias_->cpu_data();
  for (int i = 0; i < count; ++i) {
    EXPECT_NEAR(data[i], in_data[i] + bias, 1e-5);
  }
}

TYPED_TEST(BiasLayerTest, TestForwardBiasAxis2)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_bias_);
  BiasLayer layer;
  layer.axis_ = (2);
  
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  ASSERT_EQ(this->blob_bottom_->shape(), this->blob_top_->shape());
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  const Dtype* data = this->blob_top_->cpu_data();
  const int count = this->blob_top_->count();
  const Dtype* in_data = this->blob_bottom_->cpu_data();
  const Dtype bias = *this->blob_bottom_bias_->cpu_data();
  for (int i = 0; i < count; ++i) {
    EXPECT_NEAR(data[i], in_data[i] + bias, 1e-5);
  }
}

TYPED_TEST(BiasLayerTest, TestGradientEltwise)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_eltwise_);
  BiasLayer layer;
  layer.axis_ = (0);
  GradientChecker checker(1e-2, 1e-3);
  checker.CheckGradientEltwise(&layer, this->blob_bottom_vec_,
    this->blob_top_vec_);
}

TYPED_TEST(BiasLayerTest, TestGradientEltwiseWithParam)
{
  typedef typename TypeParam::Dtype Dtype;
  BiasLayer layer;
  layer.axis_ = (0);
  layer.num_axes_ = (-1);
  layer.filler_.type_ = FillerMethod_gaussian;
  GradientChecker checker(1e-2, 1e-3);
  checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
    this->blob_top_vec_);
}

TYPED_TEST(BiasLayerTest, TestGradientBroadcastBegin)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_broadcast_0_);
  BiasLayer layer;
  layer.axis_ = (0);
  GradientChecker checker(1e-2, 1e-3);
  checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
    this->blob_top_vec_);
}

TYPED_TEST(BiasLayerTest, TestGradientBroadcastMiddle)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_broadcast_1_);
  BiasLayer layer;
  layer.axis_ = (1);
  GradientChecker checker(1e-2, 1e-3);
  checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
    this->blob_top_vec_);
}

TYPED_TEST(BiasLayerTest, TestGradientBroadcastMiddleWithParam)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_broadcast_1_);
  BiasLayer layer;
  layer.axis_ = (1);
  layer.num_axes_ = (2);
  layer.filler_.type_ = FillerMethod_gaussian;
  GradientChecker checker(1e-2, 1e-3);
  checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
    this->blob_top_vec_);
}

TYPED_TEST(BiasLayerTest, TestGradientBroadcastEnd)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_broadcast_2_);
  BiasLayer layer;
  layer.axis_ = (2);
  GradientChecker checker(1e-2, 1e-3);
  checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
    this->blob_top_vec_);
}

TYPED_TEST(BiasLayerTest, TestGradientBias)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_bias_);
  BiasLayer layer;
  GradientChecker checker(1e-2, 1e-3);
  checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
    this->blob_top_vec_);
}

TYPED_TEST(BiasLayerTest, TestGradientBiasAxis2)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_bias_);
  BiasLayer layer;
  layer.axis_ = (2);
  GradientChecker checker(1e-2, 1e-3);
  checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
    this->blob_top_vec_);
}
