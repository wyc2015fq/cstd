#include <algorithm>
#include <cstring>
#include <vector>

//#include "gtest/gtest.h"
//#include "caffe/test/test_caffe_main.hpp"
#include "test_gradient_check_util.hpp"

class TransposeLayerTest : public MultiDeviceTest {
  typedef Blob TypeParam;
  typedef typename TypeParam::Dtype Dtype;
public:
  TransposeLayerTest()
    : blob_bottom_(new Blob(5, 2, 3, 4)),
    blob_top_(new Blob()) {
    // fill the values
    GaussianFiller(this->blob_bottom_);
    blob_bottom_vec_.push_back(blob_bottom_);
    blob_top_vec_.push_back(blob_top_);
  }
  virtual ~TransposeLayerTest() { delete blob_bottom_; delete blob_top_; }
  Blob* const blob_bottom_;
  Blob* const blob_top_;
  vector<Blob*> blob_bottom_vec_;
  vector<Blob*> blob_top_vec_;
};

TYPED_TEST(TransposeLayerTest, TestTopShape) {
  typedef typename TypeParam::Dtype Dtype;
  int dim[] = {0,2,3,1,};
  TransposeLayer layer;
  layer.dim_.assign(dim, dim+4);
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  DataShape bottom_shape = this->blob_bottom_->shape();
  DataShape top_shape = this->blob_top_->shape();
  EXPECT_EQ(top_shape.at(0), bottom_shape.at(0));
  EXPECT_EQ(top_shape.at(1), bottom_shape.at(2));
  EXPECT_EQ(top_shape.at(2), bottom_shape.at(3));
  EXPECT_EQ(top_shape.at(3), bottom_shape.at(1));
}

TYPED_TEST(TransposeLayerTest, TestForward) {
  typedef typename TypeParam::Dtype Dtype;
  int dim[] = { 0,2,3,1, };
  TransposeLayer layer;
  layer.dim_.assign(dim, dim + 4);
  layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  layer.Forward(this->blob_bottom_vec_, this->blob_top_vec_);
  const Dtype* bottom_data = this->blob_bottom_->cpu_data();
  const Dtype* top_data = this->blob_top_->cpu_data();
  for (int n = 0; n < this->blob_top_->shape(0); n++) {
    for (int c = 0; c < this->blob_top_->shape(1); c++) {
      for (int h = 0; h < this->blob_top_->shape(2); h++) {
        for (int w = 0; w < this->blob_top_->shape(3); w++) {
          EXPECT_EQ(*top_data, *(bottom_data + this->blob_bottom_->offset(n, w, c, h)));
          top_data += 1;
        }
      }
    }
  }
}

TYPED_TEST(TransposeLayerTest, TestGradient) {
  typedef typename TypeParam::Dtype Dtype;
  int dim[] = { 0,2,3,1, };
  TransposeLayer layer;
  layer.dim_.assign(dim, dim + 4);
  GradientChecker checker(1e-2, 1e-4);
  checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
    this->blob_top_vec_);
}
