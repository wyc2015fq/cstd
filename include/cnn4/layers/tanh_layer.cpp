// TanH neuron activation function layer.
// Adapted from ReLU layer code written by Yangqing Jia



#include "caffe/layers/tanh_layer.hpp"

namespace
{

  template <typename Dtype>
  void TanHLayer::Forward(CPUContext* context, const vector<Blob*> & bottom,
                                     const vector<Blob*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* top_data = top[0]->mutable_data();
    const int count = bottom[0]->count();
    for (int i = 0; i < count; ++i) {
      top_data[i] = tanh(bottom_data[i]);
    }
  }

  template <typename Dtype>
  void TanHLayer::Backward(CPUContext* context, const vector<Blob*> & top,
                                      int*
                                      const vector<Blob*> & bottom)
  {
    if (bottom[0]->propagate_down_) {
      const Dtype* top_data = top[0]->data();
      const Dtype* top_diff = top[0]->diff();
      Dtype* bottom_diff = bottom[0]->mutable_diff();
      const int count = bottom[0]->count();
      Dtype tanhx;
      for (int i = 0; i < count; ++i) {
        tanhx = top_data[i];
        bottom_diff[i] = top_diff[i] * (1 - tanhx * tanhx);
      }
    }
  }

#ifdef CPU_ONLY
  STUB_GPU(TanHLayer);
#endif

  INSTANTIATE_CLASS(TanHLayer);

}  // namespace
