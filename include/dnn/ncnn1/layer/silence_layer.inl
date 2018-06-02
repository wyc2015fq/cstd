

#include "silence_layer.hpp"





void SilenceLayer::Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  for (int i = 0; i < bottom.size(); ++i) {
    if (propagate_down[i]) {
      caffe_set(bottom[i]->count(), Dtype(0),
                bottom[i]->mutable_cpu_diff());
    }
  }
}

#ifdef CPU_ONLY
STUB_GPU(SilenceLayer);
#endif

INSTANTIATE_CLASS(SilenceLayer);
REGISTER_LAYER_CLASS(Silence);


