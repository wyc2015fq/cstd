

#include "caffe/layers/euclidean_loss_layer.hpp"


namespace
{

  template <typename Dtype>
  void EuclideanLossLayer<Dtype>::Reshape(
    const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top)
  {
    LossLayer<Dtype>::Reshape(bottom, top);
    CHECK_EQ(bottom[0]->count(1), bottom[1]->count(1))
        << "Inputs must have the same dimension.";
    diff_.ReshapeLike(*bottom[0]);
  }

  template <typename Dtype>
  void EuclideanLossLayer<Dtype>::Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
      const vector<Blob<Dtype>*> & top)
  {
    int count = bottom[0]->count();
    caffe_sub(
      count,
      bottom[0]->data<Context>(),
      bottom[1]->data<Context>(),
      diff_.mutable_data<Context>());
    Dtype dot = caffe_dot(count, diff_.data<Context>(), diff_.data<Context>());
    Dtype loss = dot / bottom[0]->num() / Dtype(2);
    top[0]->mutable_data<Context>()[0] = loss;
  }

  template <typename Dtype>
  void EuclideanLossLayer<Dtype>::Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
      const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom)
  {
    for (int i = 0; i < 2; ++i) {
      if (propagate_down[i]) {
        const Dtype sign = (i == 0) ? 1 : -1;
        const Dtype alpha = sign * top[0]->diff<Context>()[0] / bottom[i]->num();
        caffe_axpby(
          bottom[i]->count(),              // count
          alpha,                              // alpha
          diff_.data<Context>(),                   // a
          Dtype(0),                           // beta
          bottom[i]->mutable_diff<Context>());  // b
      }
    }
  }

#ifdef CPU_ONLY
  STUB_GPU(EuclideanLossLayer);
#endif

  INSTANTIATE_CLASS(EuclideanLossLayer);
  REGISTER_LAYER_CLASS(EuclideanLoss);

}  // namespace
