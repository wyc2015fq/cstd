
class AccuracyLayerTest : public CPUDeviceTest
{
protected:
  AccuracyLayerTest()
    : blob_bottom_data_(new Blob()),
    blob_bottom_label_(new Blob()),
    blob_top_(new Blob()),
    blob_top_per_class_(new Blob()),
    top_k_(3) {
    vector<int> shape(2);
    shape[0] = 100;
    shape[1] = 10;
    blob_bottom_data_->Reshape(shape);
    shape.resize(1);
    blob_bottom_label_->Reshape(shape);
    FillBottoms();
    blob_bottom_vec_.push_back(blob_bottom_data_);
    blob_bottom_vec_.push_back(blob_bottom_label_);
    blob_top_vec_.push_back(blob_top_);
    blob_top_per_class_vec_.push_back(blob_top_);
    blob_top_per_class_vec_.push_back(blob_top_per_class_);
  }

  virtual void FillBottoms() {
    // fill the probability values
    GaussianFiller(this->blob_bottom_data_);
    const unsigned int prefetch_rng_seed = caffe_rng_rand();
    Dtype* label_data = blob_bottom_label_->cpu_mdata();
    for (int i = 0; i < blob_bottom_label_->count(); ++i) {
      label_data[i] = rand() % 10;
    }
  }

  virtual ~AccuracyLayerTest() {
    delete blob_bottom_data_;
    delete blob_bottom_label_;
    delete blob_top_;
    delete blob_top_per_class_;
  }
  Blob* const blob_bottom_data_;
  Blob* const blob_bottom_label_;
  Blob* const blob_top_;
  Blob* const blob_top_per_class_;
  vector<Blob*> blob_bottom_vec_;
  vector<Blob*> blob_top_vec_;
  vector<Blob*> blob_top_per_class_vec_;
  int top_k_;
};

TYPED_TEST_CASE(AccuracyLayerTest, TestDtypes);

TYPED_TEST(AccuracyLayerTest, TestSetup)
{
  AccuracyLayer layer;

  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  EXPECT_EQ(this->blob_top_->num(), 1);
  EXPECT_EQ(this->blob_top_->channels(), 1);
  EXPECT_EQ(this->blob_top_->height(), 1);
  EXPECT_EQ(this->blob_top_->width(), 1);
}

TYPED_TEST(AccuracyLayerTest, TestSetupTopK)
{
  AccuracyLayer layer;
  layer.top_k_ = (5);

  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  EXPECT_EQ(this->blob_top_->num(), 1);
  EXPECT_EQ(this->blob_top_->channels(), 1);
  EXPECT_EQ(this->blob_top_->height(), 1);
  EXPECT_EQ(this->blob_top_->width(), 1);
}

TYPED_TEST(AccuracyLayerTest, TestSetupOutputPerClass)
{
  AccuracyLayer layer;

  layer.SetUp(this->blob_bottom_vec_, this->blob_top_per_class_vec_);
  EXPECT_EQ(this->blob_top_->num(), 1);
  EXPECT_EQ(this->blob_top_->channels(), 1);
  EXPECT_EQ(this->blob_top_->height(), 1);
  EXPECT_EQ(this->blob_top_->width(), 1);
  EXPECT_EQ(this->blob_top_per_class_->num(), 10);
  EXPECT_EQ(this->blob_top_per_class_->channels(), 1);
  EXPECT_EQ(this->blob_top_per_class_->height(), 1);
  EXPECT_EQ(this->blob_top_per_class_->width(), 1);
}

TYPED_TEST(AccuracyLayerTest, TestForwardCPU)
{
  AccuracyLayer layer;

  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  double max_value;
  int max_id;
  int num_correct_labels = 0;
  for (int i = 0; i < 100; ++i) {
    max_value = -FLT_MAX;
    max_id = 0;
    for (int j = 0; j < 10; ++j) {
      if (this->blob_bottom_data_->data_at(i, j, 0, 0) > max_value) {
        max_value = this->blob_bottom_data_->data_at(i, j, 0, 0);
        max_id = j;
      }
    }
    if (max_id == this->blob_bottom_label_->data_at(i, 0, 0, 0)) {
      ++num_correct_labels;
    }
  }
  EXPECT_NEAR(this->blob_top_->data_at(0, 0, 0, 0),
    num_correct_labels / 100.0, 1e-4);
}

TYPED_TEST(AccuracyLayerTest, TestForwardWithSpatialAxes)
{
  this->blob_bottom_data_->Reshape(2, 10, 4, 5);
  vector<int> label_shape(3);
  label_shape[0] = 2;
  label_shape[1] = 4;
  label_shape[2] = 5;
  this->blob_bottom_label_->Reshape(label_shape);
  this->FillBottoms();
  AccuracyLayer layer;
  layer.axis_ = (1);

  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  double max_value;
  const int num_labels = this->blob_bottom_label_->count();
  int max_id;
  int num_correct_labels = 0;
  vector<int> label_offset(3);
  for (int n = 0; n < this->blob_bottom_data_->num(); ++n) {
    for (int h = 0; h < this->blob_bottom_data_->height(); ++h) {
      for (int w = 0; w < this->blob_bottom_data_->width(); ++w) {
        max_value = -FLT_MAX;
        max_id = 0;
        for (int c = 0; c < this->blob_bottom_data_->channels(); ++c) {
          const Dtype pred_value =
            this->blob_bottom_data_->data_at(n, c, h, w);
          if (pred_value > max_value) {
            max_value = pred_value;
            max_id = c;
          }
        }
        label_offset[0] = n;
        label_offset[1] = h;
        label_offset[2] = w;
        const int correct_label =
          static_cast<int>(this->blob_bottom_label_->data_at(label_offset));
        if (max_id == correct_label) {
          ++num_correct_labels;
        }
      }
    }
  }
  EXPECT_NEAR(this->blob_top_->data_at(0, 0, 0, 0),
    num_correct_labels / Dtype(num_labels), 1e-4);
}

TYPED_TEST(AccuracyLayerTest, TestForwardIgnoreLabel)
{
  AccuracyLayer layer;
  const Dtype kIgnoreLabelValue = -1;
  layer.ignore_label_ = (kIgnoreLabelValue);

  // Manually set some labels to the ignore label value (-1).
  this->blob_bottom_label_->cpu_mdata()[2] = kIgnoreLabelValue;
  this->blob_bottom_label_->cpu_mdata()[5] = kIgnoreLabelValue;
  this->blob_bottom_label_->cpu_mdata()[32] = kIgnoreLabelValue;
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  double max_value;
  int max_id;
  int num_correct_labels = 0;
  int count = 0;
  for (int i = 0; i < 100; ++i) {
    if (kIgnoreLabelValue == this->blob_bottom_label_->data_at(i, 0, 0, 0)) {
      continue;
    }
    ++count;
    max_value = -FLT_MAX;
    max_id = 0;
    for (int j = 0; j < 10; ++j) {
      if (this->blob_bottom_data_->data_at(i, j, 0, 0) > max_value) {
        max_value = this->blob_bottom_data_->data_at(i, j, 0, 0);
        max_id = j;
      }
    }
    if (max_id == this->blob_bottom_label_->data_at(i, 0, 0, 0)) {
      ++num_correct_labels;
    }
  }
  EXPECT_EQ(count, 97);  // We set 3 out of 100 labels to kIgnoreLabelValue.
  EXPECT_NEAR(this->blob_top_->data_at(0, 0, 0, 0),
    num_correct_labels / Dtype(count), 1e-4);
}

TYPED_TEST(AccuracyLayerTest, TestForwardCPUTopK)
{
  AccuracyLayer layer;
  layer.top_k_ = (this->top_k_);

  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  Dtype current_value;
  int current_rank;
  int num_correct_labels = 0;
  for (int i = 0; i < 100; ++i) {
    for (int j = 0; j < 10; ++j) {
      current_value = this->blob_bottom_data_->data_at(i, j, 0, 0);
      current_rank = 0;
      for (int k = 0; k < 10; ++k) {
        if (this->blob_bottom_data_->data_at(i, k, 0, 0) > current_value) {
          ++current_rank;
        }
      }
      if (current_rank < this->top_k_ &&
        j == this->blob_bottom_label_->data_at(i, 0, 0, 0)) {
        ++num_correct_labels;
      }
    }
  }
  EXPECT_NEAR(this->blob_top_->data_at(0, 0, 0, 0),
    num_correct_labels / 100.0, 1e-4);
}

TYPED_TEST(AccuracyLayerTest, TestForwardCPUPerClass)
{
  AccuracyLayer layer;

  layer.SetUp(this->blob_bottom_vec_, this->blob_top_per_class_vec_);
  layer.Forward(this->blob_bottom_vec_, this->blob_top_per_class_vec_);
  double max_value;
  int max_id;
  int num_correct_labels = 0;
  const int num_class = this->blob_top_per_class_->num();
  vector<int> correct_per_class(num_class, 0);
  vector<int> num_per_class(num_class, 0);
  for (int i = 0; i < 100; ++i) {
    max_value = -FLT_MAX;
    max_id = 0;
    for (int j = 0; j < 10; ++j) {
      if (this->blob_bottom_data_->data_at(i, j, 0, 0) > max_value) {
        max_value = this->blob_bottom_data_->data_at(i, j, 0, 0);
        max_id = j;
      }
    }
    ++num_per_class[this->blob_bottom_label_->data_at(i, 0, 0, 0)];
    if (max_id == this->blob_bottom_label_->data_at(i, 0, 0, 0)) {
      ++num_correct_labels;
      ++correct_per_class[max_id];
    }
  }
  EXPECT_NEAR(this->blob_top_->data_at(0, 0, 0, 0),
    num_correct_labels / 100.0, 1e-4);
  for (int i = 0; i < num_class; ++i) {
    Dtype accuracy_per_class = (num_per_class[i] > 0 ? (Dtype)(correct_per_class[i]) / num_per_class[i] : 0);
    EXPECT_NEAR(this->blob_top_per_class_->data_at(i, 0, 0, 0),
      accuracy_per_class, 1e-4);
  }
}


TYPED_TEST(AccuracyLayerTest, TestForwardCPUPerClassWithIgnoreLabel)
{
  AccuracyLayer layer;
  const Dtype kIgnoreLabelValue = -1;
  layer.ignore_label_ = (kIgnoreLabelValue);

  // Manually set some labels to the ignore label value (-1).
  this->blob_bottom_label_->cpu_mdata()[2] = kIgnoreLabelValue;
  this->blob_bottom_label_->cpu_mdata()[5] = kIgnoreLabelValue;
  this->blob_bottom_label_->cpu_mdata()[32] = kIgnoreLabelValue;
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_per_class_vec_);
  layer.Forward(this->blob_bottom_vec_, this->blob_top_per_class_vec_);
  double max_value;
  int max_id;
  int num_correct_labels = 0;
  const int num_class = this->blob_top_per_class_->num();
  vector<int> correct_per_class(num_class, 0);
  vector<int> num_per_class(num_class, 0);
  int count = 0;
  for (int i = 0; i < 100; ++i) {
    if (kIgnoreLabelValue == this->blob_bottom_label_->data_at(i, 0, 0, 0)) {
      continue;
    }
    ++count;
    max_value = -FLT_MAX;
    max_id = 0;
    for (int j = 0; j < 10; ++j) {
      if (this->blob_bottom_data_->data_at(i, j, 0, 0) > max_value) {
        max_value = this->blob_bottom_data_->data_at(i, j, 0, 0);
        max_id = j;
      }
    }
    ++num_per_class[this->blob_bottom_label_->data_at(i, 0, 0, 0)];
    if (max_id == this->blob_bottom_label_->data_at(i, 0, 0, 0)) {
      ++num_correct_labels;
      ++correct_per_class[max_id];
    }
  }
  EXPECT_EQ(count, 97);
  EXPECT_NEAR(this->blob_top_->data_at(0, 0, 0, 0),
    num_correct_labels / Dtype(count), 1e-4);
  for (int i = 0; i < 10; ++i) {
    Dtype accuracy_per_class = (num_per_class[i] > 0 ?
      (Dtype)(correct_per_class[i]) / num_per_class[i] : 0);
    EXPECT_NEAR(this->blob_top_per_class_->data_at(i, 0, 0, 0),
      accuracy_per_class, 1e-4);
  }
}
