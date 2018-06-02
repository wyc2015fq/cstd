
// Blob(const int num, const int channels, const int height, const int width);
int SoftmaxLayerTest(Net* net, int* pbottom_) {
  int bottom_ = net_addBlob(net, dataSize(2, 10, 2, 3), "test", "0");
  int top_ = net_softmax(net, bottom_);
  net_init_gaussian(net, bottom_, 0, 1);
  //net_constant(blob_top_, 0.1);
  *pbottom_ = bottom_;
  return top_;
}

TYPED_TEST(SoftmaxLayerTest, TestForward) {
  Net net[1] = {0};
  int l, k, j, i;
  int bottom_=0;
  int top_ = SoftmaxLayerTest(net, &bottom_);
  net_forward(net);
  int channels = BLOB(bottom_)->c;
  // Test sum
  for (i = 0; i < BLOB(bottom_)->n; ++i) {
    for (k = 0; k < BLOB(bottom_)->h; ++k) {
      for (l = 0; l < BLOB(bottom_)->w; ++l) {
        Dtype sum = 0;
        for (j = 0; j < channels; ++j) {
          sum += *Blob_data_at(BLOB(top_), l, k, j, i);
        }
        EXPECT_GE(sum, 0.999);
        EXPECT_LE(sum, 1.001);
        // Test exact values
        Dtype scale = 0;
        for (j = 0; j < channels; ++j) {
          scale += exp(*Blob_data_at(BLOB(bottom_), l, k, j, i));
        }
        for (j = 0; j < channels; ++j) {
          EXPECT_GE(*Blob_data_at(BLOB(top_), l, k, j, i) + 1e-4, exp(*Blob_data_at(BLOB(bottom_), l, k, j, i)) / scale);
          EXPECT_LE(*Blob_data_at(BLOB(top_), l, k, j, i) - 1e-4, exp(*Blob_data_at(BLOB(bottom_), l, k, j, i)) / scale);
        }
      }
    }
  }
  net_free(net);
  return 1;
}

TYPED_TEST(SoftmaxLayerTest, TestGradient) {
  Net net[1] = {0};
  int bottom_=0;
  int top_ = SoftmaxLayerTest(net, &bottom_);
  ASSERT(net_check_gradient_single(net, bottom_, top_, 1e-2, 1e-3));
  net_free(net);
  return 1;
}

