#include <algorithm>


#include "caffe/layers/elu_layer.hpp"

namespace
{

  template <typename Dtype>
  void ELULayer<Dtype>::Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
                                    const vector<Blob<Dtype>*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data<Context>();
    Dtype* top_data = top[0]->mutable_data<Context>();
    const int count = bottom[0]->count();
    Dtype alpha = this->param_->elu_param().alpha();
    for (int i = 0; i < count; ++i) {
      top_data[i] = std::max(bottom_data[i], Dtype(0))
                    + alpha * (exp(std::min(bottom_data[i], Dtype(0))) - Dtype(1));
    }
  }

  template <typename Dtype>
  void ELULayer<Dtype>::Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
                                     int*
                                     const vector<Blob<Dtype>*> & bottom)
  {
    if (bottom[0]->propagate_down_) {
      const Dtype* bottom_data = bottom[0]->data<Context>();
      const Dtype* top_data = top[0]->data<Context>();
      const Dtype* top_diff = top[0]->diff<Context>();
      Dtype* bottom_diff = bottom[0]->mutable_diff<Context>();
      const int count = bottom[0]->count();
      Dtype alpha = this->param_->elu_param().alpha();
      for (int i = 0; i < count; ++i) {
        bottom_diff[i] = top_diff[i] * ((bottom_data[i] > 0)
                                        + (alpha + top_data[i]) * (bottom_data[i] <= 0));
      }
    }
  }


#ifdef CPU_ONLY
  STUB_GPU(ELULayer);
#endif

  INSTANTIATE_CLASS(ELULayer);
  REGISTER_LAYER_CLASS(ELU);

}  // namespace
