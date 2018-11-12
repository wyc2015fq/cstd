#include <cmath>


#include "caffe/layers/sigmoid_layer.hpp"

namespace
{

  template <typename Dtype>
  inline Dtype sigmoid1(Dtype x)
  {
    return 1. / (1. + exp(-x));
  }

  template <typename Dtype>
  void SigmoidLayer::Forward(CPUContext* context, const vector<Blob*> & bottom,
                                        const vector<Blob*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* top_data = top[0]->mutable_data();
    const int count = bottom[0]->count();
    for (int i = 0; i < count; ++i) {
      top_data[i] = sigmoid(bottom_data[i]);
    }
  }

  template <typename Dtype>
  void SigmoidLayer::Backward(CPUContext* context, const vector<Blob*> & top,
                                         int*
                                         const vector<Blob*> & bottom)
  {
    if (bottom[0]->propagate_down_) {
      const Dtype* top_data = top[0]->data();
      const Dtype* top_diff = top[0]->diff();
      Dtype* bottom_diff = bottom[0]->mutable_diff();
      const int count = bottom[0]->count();
      for (int i = 0; i < count; ++i) {
        const Dtype sigmoid_x = top_data[i];
        bottom_diff[i] = top_diff[i] * sigmoid_x * (1. - sigmoid_x);
      }
    }
  }

#ifdef CPU_ONLY
  STUB_GPU(SigmoidLayer);
#endif

  INSTANTIATE_CLASS(SigmoidLayer);


}  // namespace
