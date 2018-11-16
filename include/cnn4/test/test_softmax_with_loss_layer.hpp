
class SoftmaxWithLossLayerTest : public MultiDeviceTest
{
  typedef typename TypeParam::Dtype Dtype;

protected:
  SoftmaxWithLossLayerTest()
    : blob_bottom_data_(new Blob(10, 5, 2, 3)),
    blob_bottom_label_(new Blob(10, 1, 2, 3)),
    blob_top_loss_(new Blob()) {
    // fill the values
    GaussianFiller(this->blob_bottom_data_, 0, 10);
    blob_bottom_vec_.push_back(blob_bottom_data_);
    for (int i = 0; i < blob_bottom_label_->count(); ++i) {
      blob_bottom_label_->cpu_mdata()[i] = caffe_rng_rand() % 5;
    }
    blob_bottom_vec_.push_back(blob_bottom_label_);
    blob_top_vec_.push_back(blob_top_loss_);
  }
  virtual ~SoftmaxWithLossLayerTest() {
    delete blob_bottom_data_;
    delete blob_bottom_label_;
    delete blob_top_loss_;
  }
  Blob* const blob_bottom_data_;
  Blob* const blob_bottom_label_;
  Blob* const blob_top_loss_;
  vector<Blob*> blob_bottom_vec_;
  vector<Blob*> blob_top_vec_;
};

TYPED_TEST_CASE(SoftmaxWithLossLayerTest, TestDtypesAndDevices);

void set_loss_weight(const vector<Blob*>& blobs, double x) {
  for (int i = 0; i < blobs.size(); ++i) {
    blobs[i]->loss_weight_ = x;
  }
}

TYPED_TEST(SoftmaxWithLossLayerTest, TestGradient)
{
  typedef typename TypeParam::Dtype Dtype;
  SoftmaxWithLossLayer layer;
  set_loss_weight(blob_top_vec_, 3);
  GradientChecker checker(1e-2, 1e-2, 1701);
  checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
    this->blob_top_vec_, 0);
}

TYPED_TEST(SoftmaxWithLossLayerTest, TestForwardIgnoreLabel)
{
  typedef typename TypeParam::Dtype Dtype;
  SoftmaxWithLossLayer layer;
  layer.normalize_ = (false);
  // First, compute the loss with all labels
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  Dtype full_loss = this->blob_top_loss_->cpu_data()[0];
  // Now, accumulate the loss, ignoring each label in {0, ..., 4} in turn.
  Dtype accum_loss = 0;
  for (int label = 0; label < 5; ++label) {
    SoftmaxWithLossLayer layer2;
    layer2.ignore_label_ = (label);
    layer2.normalize_ = (false);
    layer2.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    layer2.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    accum_loss += this->blob_top_loss_->cpu_data()[0];
  }
  // Check that each label was included all but once.
  EXPECT_NEAR(4 * full_loss, accum_loss, 1e-4);
}

TYPED_TEST(SoftmaxWithLossLayerTest, TestGradientIgnoreLabel)
{
  typedef typename TypeParam::Dtype Dtype;
  SoftmaxWithLossLayer layer;
  // labels are in {0, ..., 4}, so we'll ignore about a fifth of them
  layer.ignore_label_ = (0);
  
  GradientChecker checker(1e-2, 1e-2, 1701);
  checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
    this->blob_top_vec_, 0);
}

TYPED_TEST(SoftmaxWithLossLayerTest, TestGradientUnnormalized)
{
  typedef typename TypeParam::Dtype Dtype;
  SoftmaxWithLossLayer layer;
  layer.normalize_ = (false);
  
  GradientChecker checker(1e-2, 1e-2, 1701);
  checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
    this->blob_top_vec_, 0);
}
