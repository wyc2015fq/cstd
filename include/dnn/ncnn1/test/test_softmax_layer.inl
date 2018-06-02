

#include "test_gradient_check_util.hpp"


// Blob(const int num, const int channels, const int height, const int width);
int SoftmaxLayerTest(Net* net, int* pbottom_) {
  int bottom_ = net_addBlob(net, "bottom", 3, 2, 10, 2);
  int top_ = net_softmax(net, bottom_);
  net_rng_gaussian(net, bottom_, 0, 1);
  //net_constant(blob_top_, 0.1);
  *pbottom_ = bottom_;
  return top_;
}


TYPED_TEST(SoftmaxLayerTest, TestForward) {
  Net net[1] = {0};
  int l, k, j, i;
  int bottom_=0;
  int top_ = SoftmaxLayerTest(net, &bottom_);
  net_Forward(net);
  img_t* bottom_data = BLOB(bottom_)->data;
  img_t* top_data = BLOB(top_)->data;
  int channels = pBox_channels(bottom_data);
  // Test sum
  for (i = 0; i < bottom_data->f; ++i) {
    for (k = 0; k < bottom_data->h; ++k) {
      for (l = 0; l < bottom_data->w; ++l) {
        Dtype sum = 0;
        for (j = 0; j < channels; ++j) {
          sum += *pBox_at(top_data, l, k, j, i);
        }
        EXPECT_GE(sum, 0.999);
        EXPECT_LE(sum, 1.001);
        // Test exact values
        Dtype scale = 0;
        for (j = 0; j < channels; ++j) {
          scale += exp(*pBox_at(bottom_data, l, k, j, i));
        }
        for (j = 0; j < channels; ++j) {
          EXPECT_GE(*pBox_at(top_data, l, k, j, i) + 1e-4, exp(*pBox_at(bottom_data, l, k, j, i)) / scale);
          EXPECT_LE(*pBox_at(top_data, l, k, j, i) - 1e-4, exp(*pBox_at(bottom_data, l, k, j, i)) / scale);
        }
      }
    }
  }
  net_free(net);
  return 1;
}

TYPED_TEST(SoftmaxLayerTest, TestGradient) {
  Net net[1] = {0};
  int l, k, j, i;
  int bottom_=0;
  int top_ = SoftmaxLayerTest(net, &bottom_);
  net_Forward(net);
  img_t* bottom_data = BLOB(bottom_)->data;
  img_t* top_data = BLOB(top_)->data;
  int channels = pBox_channels(bottom_data);
  ASSERT(net_gradient_check(net, 1e-2, 1e-3));
  net_free(net);
  return 1;
}

