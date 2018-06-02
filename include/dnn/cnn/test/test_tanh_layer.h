
double tanh_naive(double x) {
  if (x < -40) {
    // avoid negative overflow
    return -1;
  } else if (x > 40) {
    // avoid positive overflow
    return 1;
  } else {
    // exact expression for tanh, which is unstable for large x
    double exp2x = exp(2 * x);
    return (exp2x - 1.0) / (exp2x + 1.0);
  }
}

static void TanHLayerTest(Net* net, int* pbottom_) {
  if (pbottom_) {
    *pbottom_ = net_addBlob(net, "bottom", 5, 4, 3, 2);
    net_rng_gaussian(net, *pbottom_, 0, 1);
  }
}

void TestForward(Dtype filler_std) {
  int bottom_, top;
  Net net[1] = {0};
  TanHLayerTest(net, &bottom_);
  net_rng_gaussian(net, bottom_, 0, filler_std);
  top = net_tanh(net, bottom_);
  net_Forward(net);
  // Now, check values
  const Dtype* bottom_data = this->blob_bottom_->cpu_data();
  const Dtype* top_data = this->blob_top_->cpu_data();
  const Dtype min_precision = 1e-5;
  for (int i = 0; i < this->blob_bottom_->count(); ++i) {
    Dtype expected_value = tanh_naive(bottom_data[i]);
    Dtype precision = std::max(
      Dtype(std::abs(expected_value * Dtype(1e-4))), min_precision);
    EXPECT_NEAR(expected_value, top_data[i], precision);
  }
  net_free(net);
}

#if 0

  void TestBackward(Dtype filler_std) {
    FillerParameter filler_param;
    filler_param.set_std(filler_std);
    GaussianFiller filler(filler_param);
    filler.Fill(this->blob_bottom_);

    LayerParameter layer_param;
    TanHLayer layer(layer_param);
    GradientChecker checker(1e-2, 1e-2, 1701);
    checker.CheckGradientEltwise(&layer, this->blob_bottom_vec_,
        this->blob_top_vec_);
  }


TYPED_TEST_CASE(TanHLayerTest, TestDtypesAndDevices);

TYPED_TEST(TanHLayerTest, TestTanH) {
  this->TestForward(1.0);
}

TYPED_TEST(TanHLayerTest, TestTanHOverflow) {
  // this will fail if tanh overflow is not properly handled
  this->TestForward(10000.0);
}

TYPED_TEST(TanHLayerTest, TestTanHGradient) {
  this->TestBackward(1.0);
}

#endif
