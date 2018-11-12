

#include "caffe/layers/euclidean_loss_layer.hpp"


namespace
{

  template <typename Dtype>
  void EuclideanLossLayer::Reshape(
    const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    LossLayer::Reshape(bottom, top);
    CHECK_EQ(bottom[0]->count(1), bottom[1]->count(1))
        << "Inputs must have the same dimension.";
    diff_.ReshapeLike(*bottom[0]);
  }

  template <typename Dtype>
  void EuclideanLossLayer::Forward(CPUContext* context, const vector<Blob*> & bottom,
      const vector<Blob*> & top)
  {
    int count = bottom[0]->count();
    caffe_sub(
      count,
      bottom[0]->data(),
      bottom[1]->data(),
      diff_.mutable_data());
    Dtype dot = caffe_dot(count, diff_.data(), diff_.data());
    Dtype loss = dot / bottom[0]->num() / Dtype(2);
    top[0]->mutable_data()[0] = loss;
  }

  template <typename Dtype>
  void EuclideanLossLayer::Backward(CPUContext* context, const vector<Blob*> & top,
      const vector<Blob*> & bottom)
  {
    for (int i = 0; i < 2; ++i) {
      if (bottom[i]->propagate_down_) {
        const Dtype sign = (i == 0) ? 1 : -1;
        const Dtype alpha = sign * top[0]->diff()[0] / bottom[i]->num();
        caffe_axpby(
          bottom[i]->count(),              // count
          alpha,                              // alpha
          diff_.data(),                   // a
          Dtype(0),                           // beta
          bottom[i]->mutable_diff());  // b
      }
    }
  }

#ifdef CPU_ONLY
  STUB_GPU(EuclideanLossLayer);
#endif

  INSTANTIATE_CLASS(EuclideanLossLayer);
  REGISTER_LAYER_CLASS(EuclideanLoss);

}  // namespace
