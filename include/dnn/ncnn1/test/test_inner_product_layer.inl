


#ifndef CPU_ONLY
extern cudaDeviceProp CAFFE_TEST_CUDA_PROP;
#endif

int InnerProductLayerTest(Net* net, int* pbottom_, int* pbottom_nobatch_) {
  if (pbottom_) {
    *pbottom_ = net_addBlob(net, "bottom", 5, 4, 3, 2);
    net_rng_uniform(net, *pbottom_, 0, 1);
  }
  if (pbottom_nobatch_) {
    *pbottom_nobatch_ = net_addBlob(net, "bottom_nobatch", 4, 3, 2, 1);
    net_rng_uniform(net, *pbottom_nobatch_, 0, 1);
  }
  return 0;
}

TYPED_TEST(InnerProductLayerTest, TestSetUp) {
  Net net[1] = {0};
  int bottom_, w, b;
  InnerProductLayerTest(net, &bottom_, NULL);
  int top = net_matmul(net, bottom_, &w, &b, 10);
  img_t* top_data = BLOB(top)->data;
  EXPECT_EQ(top_data->f, 2);
  EXPECT_EQ(top_data->h, 1);
  EXPECT_EQ(top_data->w, 1);
  EXPECT_EQ(pBox_channels(top_data), 10);
  net_free(net);
  return 1;
}

/** @brief TestSetUp while toggling transpose flag
 */
TYPED_TEST(InnerProductLayerTest, TestSetUpTransposeFalse) {
  Net net[1] = {0};
  int bottom_, w, b;
  InnerProductLayerTest(net, &bottom_, NULL);
  int top = net_matmul(net, bottom_, &w, &b, 10, false);
  img_t* top_data = BLOB(top)->data;
  img_t* w_data = BLOB(w)->data;
  EXPECT_EQ(2, top_data->f);
  EXPECT_EQ(1, top_data->h);
  EXPECT_EQ(1, top_data->w);
  EXPECT_EQ(10, pBox_channels(top_data));
  EXPECT_EQ(10, w_data->h);
  EXPECT_EQ(60, w_data->w);
  net_free(net);
  return 1;
}

/** @brief TestSetUp while toggling transpose flag
 */
TYPED_TEST(InnerProductLayerTest, TestSetUpTransposeTrue) {
  Net net[1] = {0};
  int bottom_, w, b;
  InnerProductLayerTest(net, &bottom_, NULL);
  int top = net_matmul(net, bottom_, &w, &b, 10, true);
  img_t* top_data = BLOB(top)->data;
  img_t* w_data = BLOB(w)->data;
  EXPECT_EQ(2, top_data->f);
  EXPECT_EQ(1, top_data->h);
  EXPECT_EQ(1, top_data->w);
  EXPECT_EQ(10, pBox_channels(top_data));
  EXPECT_EQ(60, w_data->h);
  EXPECT_EQ(10, w_data->w);
  net_free(net);
  return 1;
}

TYPED_TEST(InnerProductLayerTest, TestForward) {
  Net net[1] = {0};
  int bottom_, w, b;
  InnerProductLayerTest(net, &bottom_, NULL);
  int top = net_matmul(net, bottom_, &w, &b, 10, false);
  net_rng_uniform(net, w, 0, 1);
  net_rng_uniform(net, b, 1, 2);
  net_Forward(net);
  const Dtype* data = pBox_pf(BLOB(top)->data);
  const int count = pBox_count(BLOB(top)->data);
  for (int i = 0; i < count; ++i) {
    EXPECT_GE(data[i], 1.);
  }
  net_free(net);
  return 1;
}

TYPED_TEST(InnerProductLayerTest, TestGradient) {
  Net net[1] = {0};
  int bottom_, top, w, b;
  InnerProductLayerTest(net, &bottom_, NULL);
  top = net_matmul(net, bottom_, &w, &b, 10, false);
  net_rng_gaussian(net, w, 0, 1);
  net_rng_gaussian(net, b, 1, 2);
  net_gradient_check(net, 1e-2, 1e-2);
  return 1;
}

TYPED_TEST(InnerProductLayerTest, TestGradientTranspose) {
  Net net[1] = {0};
  int bottom_, top, w, b;
  InnerProductLayerTest(net, &bottom_, NULL);
  top = net_matmul(net, bottom_, &w, &b, 10, true);
  net_rng_gaussian(net, w, 0, 1);
  net_rng_gaussian(net, b, 1, 2);
  net_gradient_check(net, 1e-2, 1e-3);
  return 1;
}

#if 0
/**
 * @brief Init. an IP layer without transpose + random weights,
 * run Forward, save the result.
 * Init. another IP layer with transpose.
 * manually copy and transpose the weights from the first IP layer,
 * then run Forward on the same input and check that the result is the same
 */
TYPED_TEST(InnerProductLayerTest, TestForwardTranspose) {
  typedef typename TypeParam::Dtype Dtype;
  blob_bottom_vec_.push_back(blob_bottom_);
  bool IS_VALID_CUDA = false;
#ifndef CPU_ONLY
  IS_VALID_CUDA = CAFFE_TEST_CUDA_PROP.major >= 2;
#endif
  if (Caffe::mode() == Caffe::CPU ||
      sizeof(Dtype) == 4 || IS_VALID_CUDA) {
    LayerParameter layer_param;
    InnerProductParameter* inner_product_param =
        layer_param.mutable_inner_product_param();
    inner_product_param->set_num_output(10);
    inner_product_param->mutable_weight_filler()->set_type("uniform");
    inner_product_param->mutable_bias_filler()->set_type("uniform");
    inner_product_param->mutable_bias_filler()->set_min(1);
    inner_product_param->mutable_bias_filler()->set_max(2);
    inner_product_param->set_transpose(false);
    shared_ptr<InnerProductLayer > layer(
        new InnerProductLayer(layer_param));
    layer->SetUp(blob_bottom_vec_, blob_top_vec_);
    layer->Forward(blob_bottom_vec_, blob_top_vec_);
    const int count = blob_top_->count();
    Blob* const top = new Blob();
    top->ReshapeLike(*blob_top_);
    caffe_copy(count, blob_top_->cpu_data(), top->mutable_cpu_data());
    blob_top_vec_.clear();
    blob_top_vec_.push_back(new Blob());
    inner_product_param->set_transpose(true);
    shared_ptr<InnerProductLayer > ip_t(
        new InnerProductLayer(layer_param));
    ip_t->SetUp(blob_bottom_vec_, blob_top_vec_);
    const int count_w = layer->blobs()[0]->count();
    EXPECT_EQ(count_w, ip_t->blobs()[0]->count());
    // manually copy and transpose the weights from 1st IP layer into 2nd
    const Dtype* w = layer->blobs()[0]->cpu_data();
    Dtype* w_t = ip_t->blobs()[0]->mutable_cpu_data();
    const int width = layer->blobs()[0]->shape(1);
    const int width_t = ip_t->blobs()[0]->shape(1);
    for (int i = 0; i < count_w; ++i) {
      int r = i / width;
      int c = i % width;
      w_t[c*width_t+r] = w[r*width+c];  // copy while transposing
    }
    // copy bias from 1st IP layer to 2nd IP layer
    ASSERT_EQ(layer->blobs()[1]->count(), ip_t->blobs()[1]->count());
    caffe_copy(layer->blobs()[1]->count(), layer->blobs()[1]->cpu_data(),
        ip_t->blobs()[1]->mutable_cpu_data());
    ip_t->Forward(blob_bottom_vec_, blob_top_vec_);
    EXPECT_EQ(count, blob_top_->count())
        << "Invalid count for top blob for IP with transpose.";
    Blob* const top_t = new Blob();\
    top_t->ReshapeLike(*blob_top_vec_[0]);
    caffe_copy(count,
      blob_top_vec_[0]->cpu_data(),
      top_t->mutable_cpu_data());
    const Dtype* data = top->cpu_data();
    const Dtype* data_t = top_t->cpu_data();
    for (int i = 0; i < count; ++i) {
      EXPECT_FLOAT_EQ(data[i], data_t[i]);
    }
  } else {
    LOG(ERROR) << "Skipping test due to old architecture.";
  }
}

TYPED_TEST(InnerProductLayerTest, TestForwardNoBatch) {
  typedef typename TypeParam::Dtype Dtype;
  blob_bottom_vec_.push_back(blob_bottom_nobatch_);
  bool IS_VALID_CUDA = false;
#ifndef CPU_ONLY
  IS_VALID_CUDA = CAFFE_TEST_CUDA_PROP.major >= 2;
#endif
  if (Caffe::mode() == Caffe::CPU ||
      sizeof(Dtype) == 4 || IS_VALID_CUDA) {
    LayerParameter layer_param;
    InnerProductParameter* inner_product_param =
        layer_param.mutable_inner_product_param();
    inner_product_param->set_num_output(10);
    inner_product_param->mutable_weight_filler()->set_type("uniform");
    inner_product_param->mutable_bias_filler()->set_type("uniform");
    inner_product_param->mutable_bias_filler()->set_min(1);
    inner_product_param->mutable_bias_filler()->set_max(2);
    shared_ptr<InnerProductLayer > layer(
        new InnerProductLayer(layer_param));
    layer->SetUp(blob_bottom_vec_, blob_top_vec_);
    layer->Forward(blob_bottom_vec_, blob_top_vec_);
    const Dtype* data = blob_top_->cpu_data();
    const int count = blob_top_->count();
    for (int i = 0; i < count; ++i) {
      EXPECT_GE(data[i], 1.);
    }
  } else {
    LOG(ERROR) << "Skipping test due to old architecture.";
  }
}

TYPED_TEST(InnerProductLayerTest, TestForward) {
  Net net[1] = {0};
  int bottom_, w, b;
  InnerProductLayerTest(net, &bottom_, NULL);
  int top = net_matmul(net, bottom_, &w, &b, 10, false);
  net_rng_uniform(net, w, 0, 1);
  net_rng_uniform(net, b, 1, 2);
  net_Forward(net);
  const Dtype* data = pBox_pf(BLOB(top)->data);
  const int count = pBox_count(BLOB(top)->data);
  for (int i = 0; i < count; ++i) {
    EXPECT_GE(data[i], 1.);
  }
  net_free(net);
  return 1;
}

TYPED_TEST(InnerProductLayerTest, TestBackwardTranspose) {
  typedef typename TypeParam::Dtype Dtype;
  blob_bottom_vec_.push_back(blob_bottom_);
  bool IS_VALID_CUDA = false;
#ifndef CPU_ONLY
  IS_VALID_CUDA = CAFFE_TEST_CUDA_PROP.major >= 2;
#endif
  if (Caffe::mode() == Caffe::CPU ||
      sizeof(Dtype) == 4 || IS_VALID_CUDA) {
    LayerParameter layer_param;
    InnerProductParameter* inner_product_param =
        layer_param.mutable_inner_product_param();
    inner_product_param->set_num_output(10);
    inner_product_param->mutable_weight_filler()->set_type("uniform");
    inner_product_param->mutable_bias_filler()->set_type("uniform");
    inner_product_param->mutable_bias_filler()->set_min(1);
    inner_product_param->mutable_bias_filler()->set_max(2);
    inner_product_param->set_transpose(false);
    shared_ptr<InnerProductLayer > layer(
        new InnerProductLayer(layer_param));
    layer->SetUp(blob_bottom_vec_, blob_top_vec_);
    layer->Forward(blob_bottom_vec_, blob_top_vec_);
    // copy top blob
    Blob* const top = new Blob();
    top->CopyFrom(*blob_top_, false, true);
    // fake top diff
    Blob* const diff = new Blob();
    diff->ReshapeLike(*blob_top_);
    {
      FillerParameter filler_param;
      UniformFiller filler(filler_param);
      filler.Fill(diff);
    }
    caffe_copy(blob_top_vec_[0]->count(),
      diff->cpu_data(),
      blob_top_vec_[0]->mutable_cpu_diff());
    vector<bool> propagate_down(1, true);
    layer->Backward(blob_top_vec_,
        propagate_down,
        blob_bottom_vec_);
    // copy first ip's weights and their diffs
    Blob* const w = new Blob();
    w->CopyFrom(*layer->blobs()[0], false, true);
    w->CopyFrom(*layer->blobs()[0], true, true);
    // copy bottom diffs
    Blob* const bottom_diff = new Blob();
    bottom_diff->CopyFrom(*blob_bottom_vec_[0], true, true);
    // repeat original top with transposed ip
    blob_top_vec_.clear();
    blob_top_vec_.push_back(new Blob());
    inner_product_param->set_transpose(true);
    shared_ptr<InnerProductLayer > ip_t(
        new InnerProductLayer(layer_param));
    ip_t->SetUp(blob_bottom_vec_, blob_top_vec_);
    // manually copy and transpose the weights from 1st IP layer into 2nd
    {
      const Dtype* w_src = w->cpu_data();
      Dtype* w_t = ip_t->blobs()[0]->mutable_cpu_data();
      const int width = layer->blobs()[0]->shape(1);
      const int width_t = ip_t->blobs()[0]->shape(1);
      for (int i = 0; i < layer->blobs()[0]->count(); ++i) {
        int r = i / width;
        int c = i % width;
        w_t[c*width_t+r] = w_src[r*width+c];  // copy while transposing
      }
      // copy bias from 1st IP layer to 2nd IP layer
      ASSERT_EQ(layer->blobs()[1]->count(), ip_t->blobs()[1]->count());
      caffe_copy(layer->blobs()[1]->count(), layer->blobs()[1]->cpu_data(),
          ip_t->blobs()[1]->mutable_cpu_data());
    }
    ip_t->Forward(blob_bottom_vec_, blob_top_vec_);
    caffe_copy(blob_top_vec_[0]->count(),
      diff->cpu_data(),
      blob_top_vec_[0]->mutable_cpu_diff());
    ip_t->Backward(blob_top_vec_, propagate_down, blob_bottom_vec_);
    const Dtype* data = w->cpu_diff();
    const Dtype* data_t = ip_t->blobs()[0]->cpu_diff();
    const int WIDTH = layer->blobs()[0]->shape(1);
    const int WIDTH_T = ip_t->blobs()[0]->shape(1);
    for (int i = 0; i < layer->blobs()[0]->count(); ++i) {
      int r = i / WIDTH;
      int c = i % WIDTH;
      EXPECT_NE(Dtype(0.), data[r*WIDTH+c]);
      EXPECT_FLOAT_EQ(data[r*WIDTH+c], data_t[c*WIDTH_T+r]);
    }
    data = bottom_diff->cpu_diff();
    data_t = blob_bottom_vec_[0]->cpu_diff();
    for (int i = 0; i < blob_bottom_vec_[0]->count(); ++i) {
      EXPECT_NE(Dtype(0.), data[i]);
      EXPECT_FLOAT_EQ(data[i], data_t[i]);
    }
  } else {
    LOG(ERROR) << "Skipping test due to old architecture.";
  }
}
#endif

