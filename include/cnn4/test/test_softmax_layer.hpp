

class SoftmaxLayerTest : public MultiDeviceTest
{
  typedef typename TypeParam::Dtype Dtype;
protected:
  SoftmaxLayerTest()
    : blob_bottom_(new Blob(2, 10, 2, 3)),
    blob_top_(new Blob()) {
    // fill the values
    GaussianFiller(this->blob_bottom_);
    blob_bottom_vec_.push_back(blob_bottom_);
    blob_top_vec_.push_back(blob_top_);
  }
  virtual ~SoftmaxLayerTest() { delete blob_bottom_; delete blob_top_; }
  Blob* const blob_bottom_;
  Blob* const blob_top_;
  vector<Blob*> blob_bottom_vec_;
  vector<Blob*> blob_top_vec_;
};

TYPED_TEST_CASE(SoftmaxLayerTest, TestDtypesAndDevices);

TYPED_TEST(SoftmaxLayerTest, TestForward)
{
  typedef typename TypeParam::Dtype Dtype;
  SoftmaxLayer layer;
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  // Test sum
  for (int i = 0; i < this->blob_bottom_->num(); ++i) {
    for (int k = 0; k < this->blob_bottom_->height(); ++k) {
      for (int l = 0; l < this->blob_bottom_->width(); ++l) {
        Dtype sum = 0;
        for (int j = 0; j < this->blob_top_->channels(); ++j) {
          sum += this->blob_top_->data_at(i, j, k, l);
        }
        EXPECT_GE(sum, 0.999);
        EXPECT_LE(sum, 1.001);
        // Test exact values
        Dtype scale = 0;
        for (int j = 0; j < this->blob_bottom_->channels(); ++j) {
          scale += exp(this->blob_bottom_->data_at(i, j, k, l));
        }
        for (int j = 0; j < this->blob_bottom_->channels(); ++j) {
          EXPECT_GE(this->blob_top_->data_at(i, j, k, l) + 1e-4,
            exp(this->blob_bottom_->data_at(i, j, k, l)) / scale)
            << "debug: " << i << " " << j;
          EXPECT_LE(this->blob_top_->data_at(i, j, k, l) - 1e-4,
            exp(this->blob_bottom_->data_at(i, j, k, l)) / scale)
            << "debug: " << i << " " << j;
        }
      }
    }
  }
}

TYPED_TEST(SoftmaxLayerTest, TestGradient)
{
  typedef typename TypeParam::Dtype Dtype;
  SoftmaxLayer layer;
  
  GradientChecker checker(1e-2, 1e-3);
  checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
    this->blob_top_vec_);
}

#ifdef USE_CUDNN
template <typename Dtype>
class CuDNNSoftmaxLayerTest : public GPUDeviceTest
{
protected:
  CuDNNSoftmaxLayerTest()
    : blob_bottom_(new Blob(2, 10, 2, 3)),
    blob_top_(new Blob()) {
    // fill the values
    FillerParameter filler_param;
    GaussianFiller filler(filler_param);
    filler.Fill(this->blob_bottom_);
    blob_bottom_vec_.push_back(blob_bottom_);
    blob_top_vec_.push_back(blob_top_);
  }
  virtual ~CuDNNSoftmaxLayerTest() { delete blob_bottom_; delete blob_top_; }
  Blob* const blob_bottom_;
  Blob* const blob_top_;
  vector<Blob*> blob_bottom_vec_;
  vector<Blob*> blob_top_vec_;
};

TYPED_TEST_CASE(CuDNNSoftmaxLayerTest, TestDtypes);

TYPED_TEST(CuDNNSoftmaxLayerTest, TestForwardCuDNN)
{
  SoftmaxLayer layer;
  CuDNNSoftmaxLayer<TypeParam> layer(layer);
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  // Test sum
  for (int i = 0; i < this->blob_bottom_->num(); ++i) {
    for (int k = 0; k < this->blob_bottom_->height(); ++k) {
      for (int l = 0; l < this->blob_bottom_->width(); ++l) {
        TypeParam sum = 0;
        for (int j = 0; j < this->blob_top_->channels(); ++j) {
          sum += this->blob_top_->data_at(i, j, k, l);
        }
        EXPECT_GE(sum, 0.999);
        EXPECT_LE(sum, 1.001);
        // Test exact values
        TypeParam scale = 0;
        for (int j = 0; j < this->blob_bottom_->channels(); ++j) {
          scale += exp(this->blob_bottom_->data_at(i, j, k, l));
        }
        for (int j = 0; j < this->blob_bottom_->channels(); ++j) {
          EXPECT_GE(this->blob_top_->data_at(i, j, k, l) + 1e-4,
            exp(this->blob_bottom_->data_at(i, j, k, l)) / scale)
            << "debug: " << i << " " << j;
          EXPECT_LE(this->blob_top_->data_at(i, j, k, l) - 1e-4,
            exp(this->blob_bottom_->data_at(i, j, k, l)) / scale)
            << "debug: " << i << " " << j;
        }
      }
    }
  }
}

TYPED_TEST(CuDNNSoftmaxLayerTest, TestGradientCuDNN)
{
  SoftmaxLayer layer;
  CuDNNSoftmaxLayer<TypeParam> layer(layer);
  GradientChecker<TypeParam> checker(1e-2, 1e-3);
  checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
    this->blob_top_vec_);
}

#endif

