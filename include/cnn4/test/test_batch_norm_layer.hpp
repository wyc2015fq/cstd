
#define BATCH_SIZE 2
#define INPUT_DATA_SIZE 3

class BatchNormLayerTest : public MultiDeviceTest
{
  typedef typename TypeParam::Dtype Dtype;
protected:
  BatchNormLayerTest()
    : blob_bottom_(new Blob(5, 2, 3, 4)),
    blob_top_(new Blob()) {
    // fill the values
    GaussianFiller(this->blob_bottom_);
    blob_bottom_vec_.push_back(blob_bottom_);
    blob_top_vec_.push_back(blob_top_);
  }
  virtual ~BatchNormLayerTest() { delete blob_bottom_; delete blob_top_; }
  Blob* const blob_bottom_;
  Blob* const blob_top_;
  vector<Blob*> blob_bottom_vec_;
  vector<Blob*> blob_top_vec_;
};

TYPED_TEST_CASE(BatchNormLayerTest, TestDtypesAndDevices);

TYPED_TEST(BatchNormLayerTest, TestForward)
{
  typedef typename TypeParam::Dtype Dtype;
  BatchNormLayer layer;
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  // Test mean
  int num = this->blob_bottom_->num();
  int channels = this->blob_bottom_->channels();
  int height = this->blob_bottom_->height();
  int width = this->blob_bottom_->width();
  for (int j = 0; j < channels; ++j) {
    Dtype sum = 0, var = 0;
    for (int i = 0; i < num; ++i) {
      for (int k = 0; k < height; ++k) {
        for (int l = 0; l < width; ++l) {
          Dtype data = this->blob_top_->data_at(i, j, k, l);
          sum += data;
          var += data * data;
        }
      }
    }
    sum /= height * width * num;
    var /= height * width * num;
    const Dtype kErrorBound = 0.001;
    // expect zero mean
    EXPECT_NEAR(0, sum, kErrorBound);
    // expect unit variance
    EXPECT_NEAR(1, var, kErrorBound);
  }
}

TYPED_TEST(BatchNormLayerTest, TestForwardInplace)
{
  typedef typename TypeParam::Dtype Dtype;
  Blob blob_inplace(5, 2, 3, 4);
  vector<Blob*> blob_bottom_vec;
  vector<Blob*> blob_top_vec;
  GaussianFiller(&blob_inplace);
  blob_bottom_vec.push_back(&blob_inplace);
  blob_top_vec.push_back(&blob_inplace);
  BatchNormLayer layer;
  layer.SetUp(blob_bottom_vec, blob_top_vec);
  layer.Forward(blob_bottom_vec, blob_top_vec);
  // Test mean
  int num = blob_inplace.num();
  int channels = blob_inplace.channels();
  int height = blob_inplace.height();
  int width = blob_inplace.width();
  for (int j = 0; j < channels; ++j) {
    Dtype sum = 0, var = 0;
    for (int i = 0; i < num; ++i) {
      for (int k = 0; k < height; ++k) {
        for (int l = 0; l < width; ++l) {
          Dtype data = blob_inplace.data_at(i, j, k, l);
          sum += data;
          var += data * data;
        }
      }
    }
    sum /= height * width * num;
    var /= height * width * num;
    const Dtype kErrorBound = 0.001;
    // expect zero mean
    EXPECT_NEAR(0, sum, kErrorBound);
    // expect unit variance
    EXPECT_NEAR(1, var, kErrorBound);
  }
}

TYPED_TEST(BatchNormLayerTest, TestGradient)
{
  typedef typename TypeParam::Dtype Dtype;
  BatchNormLayer layer;
  GradientChecker checker(1e-2, 1e-4);
  checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
    this->blob_top_vec_);
}
