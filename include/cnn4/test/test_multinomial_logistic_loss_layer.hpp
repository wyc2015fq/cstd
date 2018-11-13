#include <vector>

#include "gtest/gtest.h"

#include "caffe/blob.hpp"
#include "caffe/common.hpp"
#include "caffe/filler.hpp"
#include "caffe/layers/multinomial_logistic_loss_layer.hpp"

#include "caffe/test/test_caffe_main.hpp"
#include "caffe/test/test_gradient_check_util.hpp"

namespace caffe
{

  template <typename Dtype>
  class MultinomialLogisticLossLayerTest : public CPUDeviceTest
  {
  protected:
    MultinomialLogisticLossLayerTest()
      : blob_bottom_data_(new Blob(10, 5, 1, 1)),
        blob_bottom_label_(new Blob(10, 1, 1, 1)),
        blob_top_loss_(new Blob()) {
      Caffe::set_random_seed(1701);
      // fill the values
      FillerParameter filler_param;
      PositiveUnitballFiller filler(filler_param);
      filler.Fill(this->blob_bottom_data_);
      blob_bottom_vec_.push_back(blob_bottom_data_);
      for (int i = 0; i < blob_bottom_label_->count(); ++i) {
        blob_bottom_label_->mutable_cpu_data()[i] = caffe_rng_rand() % 5;
      }
      blob_bottom_vec_.push_back(blob_bottom_label_);
      blob_top_vec_.push_back(blob_top_loss_);
    }
    virtual ~MultinomialLogisticLossLayerTest() {
      delete blob_bottom_data_;
      delete blob_bottom_label_;
      delete blob_top_loss_;
    }
    Blob* const blob_bottom_data_;
    Blob* const blob_bottom_label_;
    Blob* const blob_top_loss_;
    vector<Blob*> blob_bottom_vec_;
    vector<Blob*> blob_top_vec_;
  };

  TYPED_TEST_CASE(MultinomialLogisticLossLayerTest, TestDtypes);


  TYPED_TEST(MultinomialLogisticLossLayerTest, TestGradientCPU)
  {
    LayerParameter layer_param;
    MultinomialLogisticLossLayer<TypeParam> layer(layer_param);
    layer.SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
    GradientChecker<TypeParam> checker(1e-2, 2 * 1e-2, 1701, 0, 0.05);
    checker.CheckGradientExhaustive(&layer, this->blob_bottom_vec_,
                                    this->blob_top_vec_, 0);
  }

}  // namespace caffe
