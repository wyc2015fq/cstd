

#include "caffe/layers/silence_layer.hpp"


namespace
{

  template <typename Dtype>
  void SilenceLayer::Backward(CPUContext* context, const vector<Blob*> & top,
                                         const vector<Blob*> & bottom)
  {
    for (int i = 0; i < bottom.size(); ++i) {
      if (bottom[i]->propagate_down_) {
        caffe_set(bottom[i]->count(), Dtype(0),
                  bottom[i]->mutable_diff());
      }
    }
  }

#ifdef CPU_ONLY
  STUB_GPU(SilenceLayer);
#endif

  INSTANTIATE_CLASS(SilenceLayer);
  REGISTER_LAYER_CLASS(Silence);

}  // namespace
