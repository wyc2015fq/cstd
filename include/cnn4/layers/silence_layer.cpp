

#include "caffe/layers/silence_layer.hpp"


namespace
{

  template <typename Dtype>
  void SilenceLayer<Dtype>::Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
                                         const vector<Blob<Dtype>*> & bottom)
  {
    for (int i = 0; i < bottom.size(); ++i) {
      if (top[i]->propagate_down_) {
        caffe_set(bottom[i]->count(), Dtype(0),
                  bottom[i]->mutable_diff<Context>());
      }
    }
  }

#ifdef CPU_ONLY
  STUB_GPU(SilenceLayer);
#endif

  INSTANTIATE_CLASS(SilenceLayer);
  REGISTER_LAYER_CLASS(Silence);

}  // namespace
