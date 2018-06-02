

#include "loss_layer.hpp"




void LossLayer::LayerSetUp(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
  // LossLayers have a non-zero (1) loss by default.
  if (this->layer_param_.loss_weight_size() == 0) {
    this->layer_param_.add_loss_weight(Dtype(1));
  }
}


void LossLayer::Reshape(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
  if (bottom.size() >= 2) {
    CHECK_EQ(bottom[0]->shape(0), bottom[1]->shape(0))
      << "The data and label should have the same first dimension.";
  }
  vector<int> loss_shape(0);  // Loss layers output a scalar; 0 axes.
  top[0]->Reshape(loss_shape);
}

INSTANTIATE_CLASS(LossLayer);


