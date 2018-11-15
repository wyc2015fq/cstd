

bool IS_VALID_CUDA = true;

#ifndef CPU_ONLY
//extern cudaDeviceProp CAFFE_TEST_CUDA_PROP;
#endif

class InnerProductLayerTest : public MultiDeviceTest
{
  typedef typename TypeParam::Dtype Dtype;
protected:
  InnerProductLayerTest()
    : blob_bottom_(new Blob(2, 3, 4, 5)),
    blob_bottom_nobatch_(new Blob(1, 2, 3, 4)),
    blob_top_(new Blob()) {
    // fill the values
    UniformFiller(this->blob_bottom_);
    blob_top_vec_.push_back(blob_top_);
  }
  virtual ~InnerProductLayerTest() {
    delete blob_bottom_;
    delete blob_bottom_nobatch_;
    delete blob_top_;
  }
  Blob* const blob_bottom_;
  Blob* const blob_bottom_nobatch_;
  Blob* const blob_top_;
  vector<Blob*> blob_bottom_vec_;
  vector<Blob*> blob_top_vec_;
};

TYPED_TEST_CASE(InnerProductLayerTest, TestDtypesAndDevices);

TYPED_TEST(InnerProductLayerTest, TestSetUp)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_);
  InnerProductLayer layer_;
  InnerProductLayer* layer = &layer_;
  layer->num_output_ = 10;
  layer->SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  EXPECT_EQ(this->blob_top_->num(), 2);
  EXPECT_EQ(this->blob_top_->height(), 1);
  EXPECT_EQ(this->blob_top_->width(), 1);
  EXPECT_EQ(this->blob_top_->channels(), 10);
}

/** @brief TestSetUp while toggling tranpose flag
 */
TYPED_TEST(InnerProductLayerTest, TestSetUpTranposeFalse)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_);
  InnerProductLayer layer_;
  InnerProductLayer* layer = &layer_;
  layer->num_output_ = 10;
  layer->transpose_ = (false);
  layer->SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  EXPECT_EQ(2, this->blob_top_->num());
  EXPECT_EQ(1, this->blob_top_->height());
  EXPECT_EQ(1, this->blob_top_->width());
  EXPECT_EQ(10, this->blob_top_->channels());
  EXPECT_EQ(2, layer->blobs_[0]->num_axes());
  EXPECT_EQ(10, layer->blobs_[0]->shape(0));
  EXPECT_EQ(60, layer->blobs_[0]->shape(1));
}

/** @brief TestSetUp while toggling tranpose flag
 */
TYPED_TEST(InnerProductLayerTest, TestSetUpTranposeTrue)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_);
  InnerProductLayer layer_;
  InnerProductLayer* layer = &layer_;
  layer->num_output_ = 10;
  layer->transpose_ = (true);
  layer->SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  EXPECT_EQ(2, this->blob_top_->num());
  EXPECT_EQ(1, this->blob_top_->height());
  EXPECT_EQ(1, this->blob_top_->width());
  EXPECT_EQ(10, this->blob_top_->channels());
  EXPECT_EQ(2, layer->blobs_[0]->num_axes());
  EXPECT_EQ(60, layer->blobs_[0]->shape(0));
  EXPECT_EQ(10, layer->blobs_[0]->shape(1));
}

TYPED_TEST(InnerProductLayerTest, TestForward)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_);
  if (mode() == CPU || sizeof(Dtype) == 4 || IS_VALID_CUDA) {
    InnerProductLayer layer_;
    InnerProductLayer* layer = &layer_;
    layer->num_output_ = 10;
    layer->weight_filler_.type_ = FillerMethod_uniform;
    layer->bias_filler_.type_ = FillerMethod_uniform;
    layer->bias_filler_.min_ = (1);
    layer->bias_filler_.max_ = (2);
    layer->SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    layer->Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    const Dtype* data = this->blob_top_->cpu_data();
    const int count = this->blob_top_->count();
    for (int i = 0; i < count; ++i) {
      EXPECT_GE(data[i], 1.);
    }
  }
  else {
    LOG(ERROR) << "Skipping test due to old architecture.";
  }
}

/**
 * @brief Init. an IP layer without transpose + random weights,
 * run Forward, save the result.
 * Init. another IP layer with transpose.
 * manually copy and transpose the weights from the first IP layer,
 * then run Forward on the same input and check that the result is the same
 */
TYPED_TEST(InnerProductLayerTest, TestForwardTranspose)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_);
  if (mode() == CPU || sizeof(Dtype) == 4 || IS_VALID_CUDA) {
    InnerProductLayer layer_;
    InnerProductLayer* layer = &layer_;
    layer->num_output_ = 10;
    layer->weight_filler_.type_ = FillerMethod_uniform;
    layer->bias_filler_.type_ = FillerMethod_uniform;
    layer->bias_filler_.min_ = (1);
    layer->bias_filler_.max_ = (2);
    layer->transpose_ = (false);
    layer->SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    layer->Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    const int count = this->blob_top_->count();
    Blob* const top = new Blob();
    top->ReshapeLike(*this->blob_top_);
    caffe_copy(count, this->blob_top_->cpu_data(), top->cpu_mdata());
    this->blob_top_vec_.clear();
    this->blob_top_vec_.push_back(new Blob());
    InnerProductLayer ip_t_;
    InnerProductLayer* ip_t = &ip_t_;
    ip_t->num_output_ = 10;
    ip_t->weight_filler_.type_ = FillerMethod_uniform;
    ip_t->bias_filler_.type_ = FillerMethod_uniform;
    ip_t->bias_filler_.min_ = (1);
    ip_t->bias_filler_.max_ = (2);
    ip_t->transpose_ = (true);
    ip_t->SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    const int count_w = layer->blobs_[0]->count();
    EXPECT_EQ(count_w, ip_t->blobs_[0]->count());
    // manually copy and transpose the weights from 1st IP layer into 2nd
    const Dtype* w = layer->blobs_[0]->cpu_data();
    Dtype* w_t = ip_t->blobs_[0]->cpu_mdata();
    const int width = layer->blobs_[0]->shape(1);
    const int width_t = ip_t->blobs_[0]->shape(1);
    for (int i = 0; i < count_w; ++i) {
      int r = i / width;
      int c = i % width;
      w_t[c * width_t + r] = w[r * width + c]; // copy while transposing
    }
    // copy bias from 1st IP layer to 2nd IP layer
    ASSERT_EQ(layer->blobs_[1]->count(), ip_t->blobs_[1]->count());
    caffe_copy(layer->blobs_[1]->count(), layer->blobs_[1]->cpu_data(),
      ip_t->blobs_[1]->cpu_mdata());
    ip_t->Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    EXPECT_EQ(count, this->blob_top_->count())
      << "Invalid count for top blob for IP with transpose.";
    Blob* const top_t = new Blob();
    top_t->ReshapeLike(*this->blob_top_vec_[0]);
    caffe_copy(count, this->blob_top_vec_[0]->cpu_data(), top_t->cpu_mdata());
    const Dtype* data = top->cpu_data();
    const Dtype* data_t = top_t->cpu_data();
    for (int i = 0; i < count; ++i) {
      EXPECT_FLOAT_EQ(data[i], data_t[i]);
    }
  }
  else {
    LOG(ERROR) << "Skipping test due to old architecture.";
  }
}

TYPED_TEST(InnerProductLayerTest, TestForwardNoBatch)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_nobatch_);
  if (mode() == CPU || sizeof(Dtype) == 4 || IS_VALID_CUDA) {
    InnerProductLayer layer_;
    InnerProductLayer* layer = &layer_;
    layer->num_output_ = 10;
    layer->weight_filler_.type_ = (FillerMethod_uniform);
    layer->bias_filler_.type_ = (FillerMethod_uniform);
    layer->bias_filler_.min_ = (1);
    layer->bias_filler_.max_ = (2);
    layer->SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    layer->Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    const Dtype* data = this->blob_top_->cpu_data();
    const int count = this->blob_top_->count();
    for (int i = 0; i < count; ++i) {
      EXPECT_GE(data[i], 1.);
    }
  }
  else {
    LOG(ERROR) << "Skipping test due to old architecture.";
  }
}

TYPED_TEST(InnerProductLayerTest, TestGradient)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_);
  if (mode() == CPU || sizeof(Dtype) == 4 || IS_VALID_CUDA) {
    InnerProductLayer layer_;
    InnerProductLayer* layer = &layer_;
    layer->num_output_ = 10;
    layer->weight_filler_.type_ = (FillerMethod_gaussian);
    layer->bias_filler_.type_ = (FillerMethod_gaussian);
    layer->bias_filler_.min_ = (1);
    layer->bias_filler_.max_ = (2);
    GradientChecker checker(1e-2, 1e-3);
    checker.CheckGradientExhaustive(layer, this->blob_bottom_vec_,
      this->blob_top_vec_);
  }
  else {
    LOG(ERROR) << "Skipping test due to old architecture.";
  }
}

TYPED_TEST(InnerProductLayerTest, TestGradientTranspose)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_);
  if (mode() == CPU || sizeof(Dtype) == 4 || IS_VALID_CUDA) {
    InnerProductLayer layer_;
    InnerProductLayer* layer = &layer_;
    layer->num_output_ = 11;
    layer->weight_filler_.type_ = (FillerMethod_gaussian);
    layer->bias_filler_.type_ = (FillerMethod_gaussian);
    layer->bias_filler_.min_ = (1);
    layer->bias_filler_.max_ = (2);
    layer->transpose_ = (true);
    GradientChecker checker(1e-2, 1e-3);
    checker.CheckGradientExhaustive(layer, this->blob_bottom_vec_,
      this->blob_top_vec_);
  }
  else {
    LOG(ERROR) << "Skipping test due to old architecture.";
  }
}

TYPED_TEST(InnerProductLayerTest, TestBackwardTranspose)
{
  typedef typename TypeParam::Dtype Dtype;
  this->blob_bottom_vec_.push_back(this->blob_bottom_);
  if (mode() == CPU || sizeof(Dtype) == 4 || IS_VALID_CUDA) {
    InnerProductLayer layer_;
    InnerProductLayer* layer = &layer_;
    layer->num_output_ = 10;
    layer->weight_filler_.type_ = (FillerMethod_uniform);
    layer->bias_filler_.type_ = (FillerMethod_uniform);
    layer->bias_filler_.min_ = (1);
    layer->bias_filler_.max_ = (2);
    layer->transpose_ = (false);
    layer->SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    layer->Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    // copy top blob
    Blob* const top = new Blob();
    top->CopyFrom(this->blob_top_, false, true);
    // fake top diff
    Blob* const diff = new Blob();
    diff->ReshapeLike(*this->blob_top_);
    UniformFiller(diff);
    caffe_copy(this->blob_top_vec_[0]->count(),
      diff->cpu_data(),
      this->blob_top_vec_[0]->cpu_mdiff());
    vector<bool> propagate_down(1, true);
    layer->Backward(this->blob_top_vec_, this->blob_bottom_vec_);
    // copy first ip's weights and their diffs
    Blob* const w = new Blob();
    w->CopyFrom(layer->blobs_[0], false, true);
    w->CopyFrom(layer->blobs_[0], true, true);
    // copy bottom diffs
    Blob* const bottom_diff = new Blob();
    bottom_diff->CopyFrom(this->blob_bottom_vec_[0], true, true);
    // repeat original top with tranposed ip
    this->blob_top_vec_.clear();
    this->blob_top_vec_.push_back(new Blob());
    layer->transpose_ = (true);
    InnerProductLayer ip_t_;
    InnerProductLayer* ip_t = &ip_t_;
    ip_t->num_output_ = 10;
    ip_t->weight_filler_.type_ = FillerMethod_uniform;
    ip_t->bias_filler_.type_ = FillerMethod_uniform;
    ip_t->bias_filler_.min_ = (1);
    ip_t->bias_filler_.max_ = (2);
    ip_t->SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    // manually copy and transpose the weights from 1st IP layer into 2nd
    {
      const Dtype* w_src = w->cpu_data();
      Dtype* w_t = ip_t->blobs_[0]->cpu_mdata();
      const int width = layer->blobs_[0]->shape(1);
      const int width_t = ip_t->blobs_[0]->shape(1);
      for (int i = 0; i < layer->blobs_[0]->count(); ++i) {
        int r = i / width;
        int c = i % width;
        w_t[c * width_t + r] = w_src[r * width + c]; // copy while transposing
      }
      // copy bias from 1st IP layer to 2nd IP layer
      ASSERT_EQ(layer->blobs_[1]->count(), ip_t->blobs_[1]->count());
      caffe_copy(layer->blobs_[1]->count(), layer->blobs_[1]->cpu_data(),
        ip_t->blobs_[1]->cpu_mdata());
    }
    ip_t->Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    caffe_copy(this->blob_top_vec_[0]->count(),
      diff->cpu_data(),
      this->blob_top_vec_[0]->cpu_mdiff());
    ip_t->Backward(this->blob_top_vec_, this->blob_bottom_vec_);
    const Dtype* data = w->cpu_diff();
    const Dtype* data_t = ip_t->blobs_[0]->cpu_diff();
    const int WIDTH = layer->blobs_[0]->shape(1);
    const int WIDTH_T = ip_t->blobs_[0]->shape(1);
    for (int i = 0; i < layer->blobs_[0]->count(); ++i) {
      int r = i / WIDTH;
      int c = i % WIDTH;
      EXPECT_NE(Dtype(0.), data[r * WIDTH + c]);
      EXPECT_FLOAT_EQ(data[r * WIDTH + c], data_t[c * WIDTH_T + r]);
    }
    data = bottom_diff->cpu_diff();
    data_t = this->blob_bottom_vec_[0]->cpu_diff();
    for (int i = 0; i < this->blob_bottom_vec_[0]->count(); ++i) {
      EXPECT_NE(Dtype(0.), data[i]);
      EXPECT_FLOAT_EQ(data[i], data_t[i]);
    }
  }
  else {
    LOG(ERROR) << "Skipping test due to old architecture.";
  }
}
