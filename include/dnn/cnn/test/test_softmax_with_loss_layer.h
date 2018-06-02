
struct SoftmaxWithLossLayerTest_t {
  int blob_bottom_data_;
  int blob_bottom_label_;
  int blob_bottom_center_;
};

static int SoftmaxWithLossLayerTest(Net* net, struct SoftmaxWithLossLayerTest_t* s) {
  s->blob_bottom_data_ = net_addBlob(net, dataSize(10, 5, 2, 3), "test", "0");
  s->blob_bottom_label_ = net_addBlob(net, dataSize(10, 1, 2, 3), "test", "0");
    // fill the values
  srand(1701);
  net_init_gaussian(net, s->blob_bottom_data_, 0, 10);
  int* blob_bottom_label_ = (int*)setDataZero(BLOB(s->blob_bottom_label_));
  int i, count = Blob_count(BLOB(s->blob_bottom_label_));
  for (i = 0; i < count; ++i) {
    blob_bottom_label_[i] = rand() % 5;
  }
  return net_softmaxloss(net, s->blob_bottom_data_, s->blob_bottom_label_);
}

//TYPED_TEST_CASE(SoftmaxWithLossLayerTest, TestDtypesAndDevices);

TYPED_TEST(SoftmaxWithLossLayerTest, TestGradient) {
  Net net[1] = {0};
  struct SoftmaxWithLossLayerTest_t s[1];
  int top = SoftmaxWithLossLayerTest(net, s);
  net_gradient_check(net, top, 1e-2, 1e-2);
  net_free(net);
  return 1;
}

#if 0
TYPED_TEST(SoftmaxWithLossLayerTest, TestForwardIgnoreLabel) {
  typedef typename TypeParam::Dtype Dtype;
  LayerParameter layer_param;
  layer_param.mutable_loss_param()->set_normalize(false);
  // First, compute the loss with all labels
  scoped_ptr<SoftmaxWithLossLayer > layer(
      new SoftmaxWithLossLayer(layer_param));
  layer->SetUp(s->blob_bottom_vec_, s->blob_top_vec_);
  layer->Forward(s->blob_bottom_vec_, s->blob_top_vec_);
  Dtype full_loss = s->blob_top_loss_->cpu_data()[0];
  // Now, accumulate the loss, ignoring each label in {0, ..., 4} in turn.
  Dtype accum_loss = 0;
  for (int label = 0; label < 5; ++label) {
    layer_param.mutable_loss_param()->set_ignore_label(label);
    layer.reset(new SoftmaxWithLossLayer(layer_param));
    layer->SetUp(s->blob_bottom_vec_, s->blob_top_vec_);
    layer->Forward(s->blob_bottom_vec_, s->blob_top_vec_);
    accum_loss += s->blob_top_loss_->cpu_data()[0];
  }
  // Check that each label was included all but once.
  EXPECT_NEAR(4 * full_loss, accum_loss, 1e-4);
}

TYPED_TEST(SoftmaxWithLossLayerTest, TestGradientIgnoreLabel) {
  typedef typename TypeParam::Dtype Dtype;
  LayerParameter layer_param;
  // labels are in {0, ..., 4}, so we'll ignore about a fifth of them
  layer_param.mutable_loss_param()->set_ignore_label(0);
  SoftmaxWithLossLayer layer(layer_param);
  GradientChecker checker(1e-2, 1e-2, 1701);
  checker.CheckGradientExhaustive(&layer, s->blob_bottom_vec_,
      s->blob_top_vec_, 0);
}

TYPED_TEST(SoftmaxWithLossLayerTest, TestGradientUnnormalized) {
  typedef typename TypeParam::Dtype Dtype;
  LayerParameter layer_param;
  layer_param.mutable_loss_param()->set_normalize(false);
  SoftmaxWithLossLayer layer(layer_param);
  GradientChecker checker(1e-2, 1e-2, 1701);
  checker.CheckGradientExhaustive(&layer, s->blob_bottom_vec_,
      s->blob_top_vec_, 0);
}
#endif
