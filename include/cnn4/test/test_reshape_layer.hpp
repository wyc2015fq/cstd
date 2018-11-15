
typedef Blob TypeParam;

  class ReshapeLayerTest : public MultiDeviceTest
  {
    typedef typename TypeParam::Dtype Dtype;
  protected:
    ReshapeLayerTest()
      : blob_bottom_(new Blob(2, 3, 6, 5)),
        blob_top_(new Blob()) {
      // fill the values
      GaussianFiller(this->blob_bottom_);
      blob_bottom_vec_.push_back(blob_bottom_);
      blob_top_vec_.push_back(blob_top_);
    }

    virtual ~ReshapeLayerTest() { delete blob_bottom_; delete blob_top_; }

    Blob* const blob_bottom_;
    Blob* const blob_top_;
    vector<Blob*> blob_bottom_vec_;
    vector<Blob*> blob_top_vec_;
  };

  TYPED_TEST_CASE(ReshapeLayerTest, TestDtypesAndDevices);

  TYPED_TEST(ReshapeLayerTest, TestFlattenOutputSizes)
  {
    typedef typename TypeParam::Dtype Dtype;
    int dim[] = {0, -1, 1, 1};
    ReshapeLayer layer;
    layer.shape_.assign(dim, dim+ countof(dim));
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    EXPECT_EQ(this->blob_top_->num(), 2);
    EXPECT_EQ(this->blob_top_->channels(), 3 * 6 * 5);
    EXPECT_EQ(this->blob_top_->height(), 1);
    EXPECT_EQ(this->blob_top_->width(), 1);
  }

  TYPED_TEST(ReshapeLayerTest, TestFlattenValues)
  {
    typedef typename TypeParam::Dtype Dtype;
    int dim[] = { 0, -1, 1, 1 };
    ReshapeLayer layer;
    layer.shape_.assign(dim, dim + countof(dim));
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    for (int c = 0; c < 3 * 6 * 5; ++c) {
      EXPECT_EQ(this->blob_top_->data_at(0, c, 0, 0),
                this->blob_bottom_->data_at(0, c / (6 * 5), (c / 5) % 6, c % 5));
      EXPECT_EQ(this->blob_top_->data_at(1, c, 0, 0),
                this->blob_bottom_->data_at(1, c / (6 * 5), (c / 5) % 6, c % 5));
    }
  }

// Test whether setting output dimensions to 0 either explicitly or implicitly
// copies the respective dimension of the input layer.
  TYPED_TEST(ReshapeLayerTest, TestCopyDimensions)
  {
    typedef typename TypeParam::Dtype Dtype;
    int dim[] = { 0, 0, 0, 0 };
    ReshapeLayer layer;
    layer.shape_.assign(dim, dim + countof(dim));
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    EXPECT_EQ(this->blob_top_->num(), 2);
    EXPECT_EQ(this->blob_top_->channels(), 3);
    EXPECT_EQ(this->blob_top_->height(), 6);
    EXPECT_EQ(this->blob_top_->width(), 5);
  }

// When a dimension is set to -1, we should infer its value from the other
// dimensions (including those that get copied from below).
  TYPED_TEST(ReshapeLayerTest, TestInferenceOfUnspecified)
  {
    typedef typename TypeParam::Dtype Dtype;
    int dim[] = { 0, 3, 10, -1 };
    ReshapeLayer layer;
    layer.shape_.assign(dim, dim + countof(dim));
    // Count is 180, thus height should be 180 / (2*3*10) = 3.
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    EXPECT_EQ(this->blob_top_->num(), 2);
    EXPECT_EQ(this->blob_top_->channels(), 3);
    EXPECT_EQ(this->blob_top_->height(), 10);
    EXPECT_EQ(this->blob_top_->width(), 3);
  }

  TYPED_TEST(ReshapeLayerTest, TestInferenceOfUnspecifiedWithStartAxis)
  {
    typedef typename TypeParam::Dtype Dtype;
    int dim[] = { 3, 10, -1 };
    ReshapeLayer layer;
    layer.shape_.assign(dim, dim + countof(dim));
    layer.axis_ = 1;
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    ASSERT_EQ(this->blob_top_->num_axes(), 4);
    EXPECT_EQ(this->blob_top_->num(), 2);
    EXPECT_EQ(this->blob_top_->channels(), 3);
    EXPECT_EQ(this->blob_top_->height(), 10);
    EXPECT_EQ(this->blob_top_->width(), 3);
  }

  TYPED_TEST(ReshapeLayerTest, TestInsertSingletonAxesStart)
  {
    typedef typename TypeParam::Dtype Dtype;
    int dim[] = { 1,1,1 };
    ReshapeLayer layer;
    layer.shape_.assign(dim, dim + countof(dim));
    layer.axis_ = (0);
    layer.num_axes_ = (0);
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    ASSERT_EQ(this->blob_top_->num_axes(), 7);
    EXPECT_EQ(this->blob_top_->shape(0), 1);
    EXPECT_EQ(this->blob_top_->shape(1), 1);
    EXPECT_EQ(this->blob_top_->shape(2), 1);
    EXPECT_EQ(this->blob_top_->shape(3), 2);
    EXPECT_EQ(this->blob_top_->shape(4), 3);
    EXPECT_EQ(this->blob_top_->shape(5), 6);
    EXPECT_EQ(this->blob_top_->shape(6), 5);
  }
  TYPED_TEST(ReshapeLayerTest, TestInsertSingletonAxesMiddle)
  {
    typedef typename TypeParam::Dtype Dtype;
    ReshapeLayer layer;
    layer.axis_=(2);
    layer.num_axes_ = (0);
    vector<int>* blob_shape = &layer.shape_;
    blob_shape->push_back(1);
    blob_shape->push_back(1);
    blob_shape->push_back(1);
    
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    ASSERT_EQ(this->blob_top_->num_axes(), 7);
    EXPECT_EQ(this->blob_top_->shape(0), 2);
    EXPECT_EQ(this->blob_top_->shape(1), 3);
    EXPECT_EQ(this->blob_top_->shape(2), 1);
    EXPECT_EQ(this->blob_top_->shape(3), 1);
    EXPECT_EQ(this->blob_top_->shape(4), 1);
    EXPECT_EQ(this->blob_top_->shape(5), 6);
    EXPECT_EQ(this->blob_top_->shape(6), 5);
  }

  TYPED_TEST(ReshapeLayerTest, TestInsertSingletonAxesEnd)
  {
    typedef typename TypeParam::Dtype Dtype;
    ReshapeLayer layer;
    layer.axis_=(-1);
    layer.num_axes_ = (0);
    vector<int>* blob_shape = &layer.shape_;
    blob_shape->push_back(1);
    blob_shape->push_back(1);
    blob_shape->push_back(1);
    
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    ASSERT_EQ(this->blob_top_->num_axes(), 7);
    EXPECT_EQ(this->blob_top_->shape(0), 2);
    EXPECT_EQ(this->blob_top_->shape(1), 3);
    EXPECT_EQ(this->blob_top_->shape(2), 6);
    EXPECT_EQ(this->blob_top_->shape(3), 5);
    EXPECT_EQ(this->blob_top_->shape(4), 1);
    EXPECT_EQ(this->blob_top_->shape(5), 1);
    EXPECT_EQ(this->blob_top_->shape(6), 1);
  }

  TYPED_TEST(ReshapeLayerTest, TestFlattenMiddle)
  {
    typedef typename TypeParam::Dtype Dtype;
    ReshapeLayer layer;
    layer.axis_=(1);
    layer.num_axes_ = (2);
    vector<int>* blob_shape = &layer.shape_;
    blob_shape->push_back(-1);
    
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    ASSERT_EQ(this->blob_top_->num_axes(), 3);
    EXPECT_EQ(this->blob_top_->shape(0), 2);
    EXPECT_EQ(this->blob_top_->shape(1), 3 * 6);
    EXPECT_EQ(this->blob_top_->shape(2), 5);
  }

  TYPED_TEST(ReshapeLayerTest, TestForward)
  {
    typedef typename TypeParam::Dtype Dtype;
    ReshapeLayer layer;
    vector<int>* shape = &layer.shape_;
    shape->push_back(6);
    shape->push_back(2);
    shape->push_back(3);
    shape->push_back(5);
    
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    for (int i = 0; i < this->blob_bottom_->count(); ++i) {
      EXPECT_EQ(this->blob_top_->cpu_data()[i],
                this->blob_bottom_->cpu_data()[i]);
    }
  }

  TYPED_TEST(ReshapeLayerTest, TestForwardAfterReshape)
  {
    typedef typename TypeParam::Dtype Dtype;
    ReshapeLayer layer;
    vector<int>* shape = &layer.shape_;
    shape->push_back(6);
    shape->push_back(2);
    shape->push_back(3);
    shape->push_back(5);
    
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    // We know the above produced the correct result from TestForward.
    // Reshape the bottom and call layer.Reshape, then try again.
    vector<int> new_bottom_shape(1, 2 * 3 * 6 * 5);
    this->blob_bottom_->Reshape(new_bottom_shape);
    layer.Reshape(this->blob_bottom_vec_, this->blob_top_vec_);
    GaussianFiller(this->blob_bottom_);
    layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
    for (int i = 0; i < this->blob_bottom_->count(); ++i) {
      EXPECT_EQ(this->blob_top_->cpu_data()[i],
                this->blob_bottom_->cpu_data()[i]);
    }
  }

  TYPED_TEST(ReshapeLayerTest, TestGradient)
  {
    typedef typename TypeParam::Dtype Dtype;
    ReshapeLayer layer;
    vector<int>* shape = &layer.shape_;
    shape->push_back(6);
    shape->push_back(2);
    shape->push_back(3);
    shape->push_back(5);
    
    GradientChecker checker(1e-2, 1e-2);
    checker.CheckGradientEltwise(&layer, this->blob_bottom_vec_,
                                 this->blob_top_vec_);
  }
