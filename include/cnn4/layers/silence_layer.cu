

#include "caffe/layers/silence_layer.hpp"


namespace {

template <typename Dtype>
void SilenceLayer::Forward(GPUContext* context, const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  // Do nothing.
}

template <typename Dtype>
void SilenceLayer::Backward(GPUContext* context, const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  for (int i = 0; i < bottom.size(); ++i) {
    if (bottom[i]->propagate_down_) {
      caffe_gpu_set(bottom[i]->count(), Dtype(0),
                    bottom[i]->mutable_gpu_diff());
    }
  }
}

INSTANTIATE_LAYER_GPU_FUNCS(SilenceLayer);

}  // namespace
