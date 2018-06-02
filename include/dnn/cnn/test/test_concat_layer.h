
struct ConcatLayerTest_t {
  int blob_bottom_vec_0_[2];
  int blob_bottom_vec_1_[2];
  int blob_bottom_0_;
  int blob_bottom_1_;
  int blob_bottom_2_;
};

static int ConcatLayerTest(Net* net, struct ConcatLayerTest_t* s) {
  const char* type = "test";
  s->blob_bottom_0_ = net_addBlob(net, dataSize(2, 3, 6, 5), type, "0");
  s->blob_bottom_1_ = net_addBlob(net, dataSize(2, 5, 6, 5), type, "1");
  s->blob_bottom_2_ = net_addBlob(net, dataSize(5, 3, 6, 5), type, "2");
  
  // fill the values
  net_init_const(net, s->blob_bottom_0_, 1);
  net_init_const(net, s->blob_bottom_1_, 2);
  net_init_const(net, s->blob_bottom_2_, 3);
  s->blob_bottom_vec_0_[0] = (s->blob_bottom_0_);
  s->blob_bottom_vec_0_[1] = (s->blob_bottom_1_);
  s->blob_bottom_vec_1_[0] = (s->blob_bottom_0_);
  s->blob_bottom_vec_1_[1] = (s->blob_bottom_2_);
  return 0;
};

//TYPED_TEST_CASE(ConcatLayerTest, TestDtypesAndDevices);

#define EXPECT_EQ(a, b)   ASSERT((a)==(b))

TYPED_TEST(ConcatLayerTest, TestSetupNum) {
  Net net[1] = {0};
  int blob_top_, axis = (0);
  struct ConcatLayerTest_t s[1];
  ConcatLayerTest(net, s);
  blob_top_ = net_concat(net, 2, s->blob_bottom_vec_1_, axis);
  Layer* layer = net->layers;
  EXPECT_EQ(BLOB(blob_top_)->n, BLOB(s->blob_bottom_0_)->n + BLOB(s->blob_bottom_2_)->n);
  EXPECT_EQ(BLOB(blob_top_)->c, BLOB(s->blob_bottom_0_)->c);
  EXPECT_EQ(BLOB(blob_top_)->h, BLOB(s->blob_bottom_0_)->h);
  EXPECT_EQ(BLOB(blob_top_)->w, BLOB(s->blob_bottom_0_)->w);
  net_free(net);
  return 1;
}

TYPED_TEST(ConcatLayerTest, TestSetupChannels) {
  Net net[1] = {0};
  int blob_top_, axis = (1);
  struct ConcatLayerTest_t s[1];
  ConcatLayerTest(net, s);
  blob_top_ = net_concat(net, 2, s->blob_bottom_vec_0_, axis);
  
  EXPECT_EQ(BLOB(blob_top_)->n, BLOB(s->blob_bottom_0_)->n);
  EXPECT_EQ(BLOB(blob_top_)->c, BLOB(s->blob_bottom_0_)->c + BLOB(s->blob_bottom_1_)->c);
  EXPECT_EQ(BLOB(blob_top_)->h, BLOB(s->blob_bottom_0_)->h);
  EXPECT_EQ(BLOB(blob_top_)->w, BLOB(s->blob_bottom_0_)->w);
  net_free(net);
  return 1;
}

TYPED_TEST(ConcatLayerTest, TestSetupChannelsNegativeIndexing) {
  Net net[1] = {0};
  int blob_top_, axis = (-3);
  struct ConcatLayerTest_t s[1];
  ConcatLayerTest(net, s);
  blob_top_ = net_concat(net, 2, s->blob_bottom_vec_0_, axis);
  // "channels" index is the third one from the end -- test negative indexing
  // by setting axis to -3 and checking that we get the same results as above in
  // TestSetupChannels.
  EXPECT_EQ(BLOB(blob_top_)->n, BLOB(s->blob_bottom_0_)->n);
  EXPECT_EQ(BLOB(blob_top_)->c, BLOB(s->blob_bottom_0_)->c + BLOB(s->blob_bottom_1_)->c);
  EXPECT_EQ(BLOB(blob_top_)->h, BLOB(s->blob_bottom_0_)->h);
  EXPECT_EQ(BLOB(blob_top_)->w, BLOB(s->blob_bottom_0_)->w);
  net_free(net);
  return 1;
}

TYPED_TEST(ConcatLayerTest, TestForwardTrivial) {
  Net net[1] = {0};
  int blob_top_, axis = (1);
  struct ConcatLayerTest_t s[1];
  ConcatLayerTest(net, s);
  blob_top_ = net_concat(net, 1, s->blob_bottom_vec_0_, axis);
  net_forward(net);
  int i;
  int count = Data_count(BLOB(s->blob_bottom_0_)->size);
  const float* a = getData(BLOB(s->blob_bottom_0_));
  const float* b = getData(BLOB(blob_top_));
  EXPECT_EQ(count, Data_count(BLOB(blob_top_)->size));
  for (i = 0; i < count; ++i) {
    EXPECT_EQ(a[i], b[i]);
  }
  net_free(net);
  return 1;
}

TYPED_TEST(ConcatLayerTest, TestForwardNum) {
  Net net[1] = {0};
  int blob_top_, axis = 0;
  struct ConcatLayerTest_t s[1];
  ConcatLayerTest(net, s);
  blob_top_ = net_concat(net, 2, s->blob_bottom_vec_1_, axis);
  net_forward(net);
  int n, c, h, w;
  const float* a = getData(BLOB(blob_top_));
  const float* v0 = getData(BLOB(s->blob_bottom_vec_1_[0]));
  const float* v1 = getData(BLOB(s->blob_bottom_vec_1_[1]));
  for (n = 0; n < BLOB(s->blob_bottom_vec_1_[0])->n; ++n) {
    for (c = 0; c < BLOB(blob_top_)->c; ++c) {
      for (h = 0; h < BLOB(blob_top_)->h; ++h) {
        for (w = 0; w < BLOB(blob_top_)->w; ++w) {
          int k = Data_4DIndex(BLOB(blob_top_)->size, n, c, h, w);
          int k1 = Data_4DIndex(BLOB(s->blob_bottom_vec_1_[0])->size, n, c, h, w);
          EXPECT_EQ(a[k], v0[k1]);
        }
      }
    }
  }
  for (n = 0; n < BLOB(s->blob_bottom_vec_1_[1])->n; ++n) {
    for (c = 0; c < BLOB(blob_top_)->c; ++c) {
      for (h = 0; h < BLOB(blob_top_)->h; ++h) {
        for (w = 0; w < BLOB(blob_top_)->w; ++w) {
          int k = Data_4DIndex(BLOB(blob_top_)->size, n+2, c, h, w);
          int k1 = Data_4DIndex(BLOB(s->blob_bottom_vec_1_[1])->size, n, c, h, w);
          EXPECT_EQ(a[k], v1[k1]);
        }
      }
    }
  }
  net_free(net);
  return 1;
}

TYPED_TEST(ConcatLayerTest, TestForwardChannels) {
  Net net[1] = {0};
  int blob_top_, axis = 1;
  struct ConcatLayerTest_t s[1];
  ConcatLayerTest(net, s);
  blob_top_ = net_concat(net, 2, s->blob_bottom_vec_0_, axis);
  net_forward(net);
  int n, c, h, w;

  const float* a = getData(BLOB(blob_top_));
  const float* v0 = getData(BLOB(s->blob_bottom_vec_0_[0]));
  const float* v1 = getData(BLOB(s->blob_bottom_vec_0_[1]));

  for (n = 0; n < BLOB(blob_top_)->n; ++n) {
    for (c = 0; c < BLOB(s->blob_bottom_0_)->c; ++c) {
      for (h = 0; h < BLOB(blob_top_)->h; ++h) {
        for (w = 0; w < BLOB(blob_top_)->w; ++w) {
          int k = Data_4DIndex(BLOB(blob_top_)->size, n, c, h, w);
          int k1 = Data_4DIndex(BLOB(s->blob_bottom_vec_0_[0])->size, n, c, h, w);
          EXPECT_EQ(a[k], v0[k1]);
        }
      }
    }
    for (c = 0; c < BLOB(s->blob_bottom_1_)->c; ++c) {
      for (h = 0; h < BLOB(blob_top_)->h; ++h) {
        for (w = 0; w < BLOB(blob_top_)->w; ++w) {
          int k = Data_4DIndex(BLOB(blob_top_)->size, n, c+3, h, w);
          int k1 = Data_4DIndex(BLOB(s->blob_bottom_vec_0_[1])->size, n, c, h, w);
          EXPECT_EQ(a[k], v1[k1]);
        }
      }
    }
  }
  net_free(net);
  return 1;
}

TYPED_TEST(ConcatLayerTest, TestGradientTrivial) {
  Net net[1] = {0};
  int blob_top_, axis = 1;
  struct ConcatLayerTest_t s[1];
  ConcatLayerTest(net, s);
  blob_top_ = net_concat(net, 1, s->blob_bottom_vec_0_, axis);
  // CheckGradientEltwise
  net_check_gradient(net, 1, s->blob_bottom_vec_0_, blob_top_, 1e-2, 1e-2);
  net_free(net);
  return 1;
}

TYPED_TEST(ConcatLayerTest, TestGradientNum) {
  Net net[1] = {0};
  int blob_top_, axis = 0;
  struct ConcatLayerTest_t s[1];
  ConcatLayerTest(net, s);
  blob_top_ = net_concat(net, 2, s->blob_bottom_vec_1_, axis);
  //CheckGradient
  net_check_gradient(net, 2, s->blob_bottom_vec_1_, blob_top_, 1e-2, 1e-2);
  net_free(net);
  return 1;
}


TYPED_TEST(ConcatLayerTest, TestGradientChannels) {
  Net net[1] = {0};
  int blob_top_, axis = 1;
  struct ConcatLayerTest_t s[1];
  ConcatLayerTest(net, s);
  blob_top_ = net_concat(net, 2, s->blob_bottom_vec_0_, axis);
  //CheckGradient
  net_check_gradient(net, 2, s->blob_bottom_vec_0_, blob_top_, 1e-2, 1e-2);
  net_free(net);
  return 1;
}

#if 0
TYPED_TEST(ConcatLayerTest, TestGradientChannelsBottomOneOnly) {
  typedef typename TypeParam::Dtype Dtype;
  LayerParameter layer_param;
  ConcatLayer layer(layer_param);
  GradientChecker checker(1e-2, 1e-2);
  checker.CheckGradient(&layer, s->blob_bottom_vec_0_,
    BLOB(blob_top_)vec_, 1);
}
#endif

