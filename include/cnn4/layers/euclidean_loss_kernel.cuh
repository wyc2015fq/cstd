

#include "caffe/layers/euclidean_loss_layer.hpp"


namespace {

template <typename Dtype>
void EuclideanLossLayer::Forward_(GPUContext* context, const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  int count = bottom[0]->count();
  caffe_gpu_sub(
      count,
      bottom[0]->data(),
      bottom[1]->data(),
      diff_.mdata());
  Dtype dot;
  caffe_gpu_dot(count, diff_.data(), diff_.data(), &dot);
  Dtype loss = dot / bottom[0]->num() / Dtype(2);
  top[0]->mdata()[0] = loss;
}

template <typename Dtype>
void EuclideanLossLayer::Backward_(GPUContext* context, const vector<Blob*>& top,
    const vector<Blob*>& bottom) {
  for (int i = 0; i < 2; ++i) {
    if (bottom[i]->propagate_down_) {
      const Dtype sign = Dtype((i == 0) ? 1 : -1);
      const Dtype alpha = sign * top[0]->diff()[0] / bottom[i]->num();
      caffe_gpu_axpby(
          bottom[i]->count(),              // count
          alpha,                              // alpha
          diff_.data(),                   // a
          Dtype(0),                           // beta
          bottom[i]->gpu_mdiff());  // b
    }
  }
}

INSTANTIATE_LAYER_GPU_FUNCS(EuclideanLossLayer);

}  // namespace
