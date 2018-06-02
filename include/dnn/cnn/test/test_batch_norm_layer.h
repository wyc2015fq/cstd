
#define BATCH_SIZE 2
#define INPUT_DATA_SIZE 3

int BatchNormLayerTest(Net* net, int* pbottom) {
  DataSize size = dataSize(5, 2, 3, 4);
  const char* type = "test";
  if (pbottom) {
    *pbottom = net_addBlob(net, size, type, "0" );
    net_init_gaussian(net, *pbottom, 0, 1);
  }
  return 1;
}

TYPED_TEST(BatchNormLayerTest, TestForward) {
  Net net[1] = {0};
  int bottom, w, b, top;
  BatchNormLayerTest(net, &bottom);
  top = net_bn(net, bottom);
  net_forward(net);
  DataSize bottomSize = BLOB(bottom)->size;
  DataSize topSize = BLOB(top)->size;
  float* bottomData = BLOB(bottom)->data;
  float* topData = BLOB(top)->data;
  // Test mean

  for (int j = 0; j < topSize.c; ++j) {
    Dtype sum = 0, var = 0;
    for (int i = 0; i < topSize.n; ++i) {
      for ( int k = 0; k < topSize.h; ++k ) {
        for ( int l = 0; l < topSize.w; ++l ) {
          Dtype data = topData[Data_4DIndex(topSize, i, j, k, l)];
          sum += data;
          var += data * data;
        }
      }
    }
    sum /= topSize.h * topSize.w * topSize.n;
    var /= topSize.h * topSize.w * topSize.n;

    const Dtype kErrorBound = 0.001;
    // expect zero mean
    EXPECT_NEAR(0, sum, kErrorBound);
    // expect unit variance
    EXPECT_NEAR(1, var, kErrorBound);
  }
  net_free(net);
  return 1;
}
TYPED_TEST(BatchNormLayerTest, TestForwardInplace) {
  Net net[1] = {0};
  int bottom, w, b, top;
  DataSize size = dataSize(5, 2, 3, 4);
  bottom = net_addBlob(net, size, "test", "0" );
  net_init_gaussian(net, bottom, 0, 1);
  //BatchNormLayerTest(net, &bottom);
  top = net_bn(net, bottom);
  net_forward(net);


  // Test mean
  int num = size.n;
  int channels = size.c;
  int height = size.h;
  int width = size.w;
  float* topData = BLOB(top)->data;

  for (int j = 0; j < channels; ++j) {
    Dtype sum = 0, var = 0;
    for (int i = 0; i < num; ++i) {
      for ( int k = 0; k < height; ++k ) {
        for ( int l = 0; l < width; ++l ) {
          Dtype data = topData[Data_4DIndex(size, i, j, k, l)];
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
  net_free(net);
  return 1;
}

TYPED_TEST(BatchNormLayerTest, TestGradient) {
  Net net[1] = {0};
  int bottom, w, b, top;
  BatchNormLayerTest(net, &bottom);
  top = net_bn(net, bottom);
  net_loss(net, top, NULL, MSELoss);
  //net_gradient_check(net, 1e-2, 1e-4);
  net_gradient_check(net, 1e-2, 1e-2);
  net_free(net);
  return 1;
}