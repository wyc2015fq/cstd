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
  void SigmoidLayer<Dtype>::Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
                                        const vector<Blob<Dtype>*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data<Context>();
    Dtype* top_data = top[0]->mutable_data<Context>();
    const int count = bottom[0]->count();
    for (int i = 0; i < count; ++i) {
      top_data[i] = sigmoid(bottom_data[i]);
    }
  }

  template <typename Dtype>
  void SigmoidLayer<Dtype>::Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
                                         int*
                                         const vector<Blob<Dtype>*> & bottom)
  {
    if (top[0]->propagate_down_) {
      const Dtype* top_data = top[0]->data<Context>();
      const Dtype* top_diff = top[0]->diff<Context>();
      Dtype* bottom_diff = bottom[0]->mutable_diff<Context>();
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
