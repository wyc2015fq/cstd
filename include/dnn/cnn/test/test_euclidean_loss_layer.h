
struct EuclideanLossLayerTest_t {
  int blob_bottom_data_;
  int blob_bottom_label_;
  int blob_bottom_center_;
  int blob_bottom_vec_[3];
};

void EuclideanLossLayerTest(Net* net, struct EuclideanLossLayerTest_t* s, LayerFun run) {
  s->blob_bottom_data_ = net_addBlob(net, dataSize(10, 5, 1, 1), "test", "0");
  s->blob_bottom_label_ = net_addBlob(net, dataSize(10, 5, 1, 1), "test", "0");
  //s->blob_bottom_label_ = net_addBlob(net, dataSize(10, 5, 1, 1), "test", "1");
    // fill the values
  Blob_init_uniform(BLOB(s->blob_bottom_data_), 0.1, 0.9);
  net_loss(net, s->blob_bottom_data_, s->blob_bottom_label_, run);
  Blob_init_uniform(BLOB(s->blob_bottom_label_), 0.1, 0.9);
  s->blob_bottom_vec_[0] = s->blob_bottom_data_;
  s->blob_bottom_vec_[1] = s->blob_bottom_label_;
}

//TYPED_TEST_CASE(EuclideanLossLayerTest, TestDtypesAndDevices);

TYPED_TEST(EuclideanLossLayerTest, TestForwardMSELoss) {
  EuclideanLossLayerTest_t s[1] = {0};
  Net net[1] = {0};
  EuclideanLossLayerTest(net, s, MSELoss);
  // Get the loss without a specified objective weight -- should be
  // equivalent to explicitly specifying a weight of 1.
  float loss = net_forward(net);
  
#if 0
  // Get the loss again with a different objective weight; check that it is
  // scaled appropriately.
  const Dtype kLossWeight = 3.7;
  layer_param.add_loss_weight(kLossWeight);
  EuclideanLossLayer layer_weight_2(layer_param);
  layer_weight_2.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  const Dtype loss_weight_2 = layer_weight_2.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  const Dtype kErrorMargin = 1e-5;
  EXPECT_NEAR(loss_weight_1 * kLossWeight, loss_weight_2, kErrorMargin);
  // Make sure the loss is non-trivial.
  const Dtype kNonTrivialAbsThresh = 1e-1;
  EXPECT_GE(fabs(loss_weight_1), kNonTrivialAbsThresh);
  
#endif
  net_free(net);
  return 1;
}
#if 0
TYPED_TEST(EuclideanLossLayerTest, TestGradient) {
  EuclideanLossLayerTest_t s[1] = {0};
  Net net[1] = {0};
  EuclideanLossLayerTest(net, s, MSELoss);
  net_check_gradient_single(net, s->blob_bottom_data_, -1, 1e-2, 1e-2);
  net_free(net);
  return 1;
}

TYPED_TEST(EuclideanLossLayerTest, TestGradientCrossEntropyLoss) {
  EuclideanLossLayerTest_t s[1] = {0};
  Net net[1] = {0};
  EuclideanLossLayerTest(net, s, CrossEntropyLoss);
  net_check_gradient_single(net, s->blob_bottom_data_, -1, 1e-2, 1e-2);
  net_free(net);
  return 1;
}

void EuclideanLabelLossLayerTest(Net* net, struct EuclideanLossLayerTest_t* s, LayerFun run) {
  s->blob_bottom_data_ = net_addBlob(net, dataSize(10, 5, 1, 1), "test", "0");
  //s->blob_bottom_label_ = net_addBlob(net, dataSize(10, 5, 1, 1), "test", "1");
    // fill the values
  Blob_init_uniform(BLOB(s->blob_bottom_data_), 0.1, 0.9);
  net_loss(net, s->blob_bottom_data_, &s->blob_bottom_label_, run);
  Blob_init_uniform_int(BLOB(s->blob_bottom_label_), 1, 5);
  s->blob_bottom_vec_[0] = s->blob_bottom_data_;
  s->blob_bottom_vec_[1] = s->blob_bottom_label_;
}
TYPED_TEST(EuclideanLabelLossLayerTest, TestGradientCrossEntropyLoss) {
  EuclideanLossLayerTest_t s[1] = {0};
  Net net[1] = {0};
  EuclideanLabelLossLayerTest(net, s, CrossEntropyLabelLoss);
  net_check_gradient_single(net, s->blob_bottom_data_, -1, 1e-2, 1e-2);
  net_free(net);
  return 1;
}

#endif
static int CenterLossLayerTest(Net* net, struct EuclideanLossLayerTest_t* s) {
  s->blob_bottom_data_ = net_addBlob(net, dataSize(100, 5, 2, 3), "test", "0");
  s->blob_bottom_label_ = net_addBlob(net, dataSize(100, 1, 1, 1), "test", "1");
    // fill the values
  srand(1701);
  net_centerloss(net, s->blob_bottom_data_, s->blob_bottom_label_, &s->blob_bottom_center_, 5, 1);
  //Blob_init_gaussian(net, s->blob_bottom_data_, 0, 10);
  Blob_init_uniform(BLOB(s->blob_bottom_data_), 0, 10);
  Blob_init_uniform_int(BLOB(s->blob_bottom_label_), 0, 5);
  Blob_init_const(BLOB(s->blob_bottom_center_), 0);
  return 0;
}
TYPED_TEST(CenterLossLayerTest, TestGradientCenterLoss) {
  EuclideanLossLayerTest_t s[1] = {0};
  Net net[1] = {0};
  CenterLossLayerTest(net, s);
  net_check_gradient_single(net, s->blob_bottom_data_, -1, 1e-2, 1e-2);
  net_free(net);
  return 1;
}

