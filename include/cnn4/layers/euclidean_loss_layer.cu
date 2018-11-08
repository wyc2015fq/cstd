

#include "caffe/layers/euclidean_loss_layer.hpp"


namespace {

template <typename Dtype>
void EuclideanLossLayer<Dtype>::Forward(GPUContext* context, const vector<Blob<Dtype>*>& bottom,
    const vector<Blob<Dtype>*>& top) {
  int count = bottom[0]->count();
  caffe_gpu_sub(
      count,
      bottom[0]->data<Context>(),
      bottom[1]->data<Context>(),
      diff_.mutable_data<Context>());
  Dtype dot;
  caffe_gpu_dot(count, diff_.data<Context>(), diff_.data<Context>(), &dot);
  Dtype loss = dot / bottom[0]->num() / Dtype(2);
  top[0]->mutable_data<Context>()[0] = loss;
}

template <typename Dtype>
void EuclideanLossLayer<Dtype>::Backward(GPUContext* context, const vector<Blob<Dtype>*>& top,
    const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {
  for (int i = 0; i < 2; ++i) {
    if (bottom[i]->propagate_down_) {
      const Dtype sign = Dtype((i == 0) ? 1 : -1);
      const Dtype alpha = sign * top[0]->diff<Context>()[0] / bottom[i]->num();
      caffe_gpu_axpby(
          bottom[i]->count(),              // count
          alpha,                              // alpha
          diff_.data<Context>(),                   // a
          Dtype(0),                           // beta
          bottom[i]->mutable_gpu_diff());  // b
    }
  }
}

INSTANTIATE_LAYER_GPU_FUNCS(EuclideanLossLayer);

}  // namespace
