#include <algorithm>


#include "caffe/layers/bnll_layer.hpp"

namespace
{

  const float kBNLL_THRESHOLD = 50.;

  template <typename Dtype>
  void BNLLLayer<Dtype>::Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
                                     const vector<Blob<Dtype>*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data<Context>();
    Dtype* top_data = top[0]->mutable_data<Context>();
    const int count = bottom[0]->count();
    for (int i = 0; i < count; ++i) {
      top_data[i] = bottom_data[i] > 0 ?
                    bottom_data[i] + log(1. + exp(-bottom_data[i])) :
                    log(1. + exp(bottom_data[i]));
    }
  }

  template <typename Dtype>
  void BNLLLayer<Dtype>::Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
                                      int*
                                      const vector<Blob<Dtype>*> & bottom)
  {
    if (top[0]->propagate_down_) {
      const Dtype* bottom_data = bottom[0]->data<Context>();
      const Dtype* top_diff = top[0]->diff<Context>();
      Dtype* bottom_diff = bottom[0]->mutable_diff<Context>();
      const int count = bottom[0]->count();
      Dtype expval;
      for (int i = 0; i < count; ++i) {
        expval = exp(std::min(bottom_data[i], Dtype(kBNLL_THRESHOLD)));
        bottom_diff[i] = top_diff[i] * expval / (expval + 1.);
      }
    }
  }

#ifdef CPU_ONLY
  STUB_GPU(BNLLLayer);
#endif

  INSTANTIATE_CLASS(BNLLLayer);
  REGISTER_LAYER_CLASS(BNLL);

}  // namespace
