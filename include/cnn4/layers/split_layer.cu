

#include "caffe/layers/split_layer.hpp"


namespace {

template <typename Dtype>
void SplitLayer::Forward(GPUContext* context, const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  for (int i = 0; i < top.size(); ++i) {
    top[i]->ShareData(*bottom[0]);
  }
}

template <typename Dtype>
void SplitLayer::Backward(GPUContext* context, const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  if (!bottom[0]->propagate_down_) { return; }
  if (top.size() == 1) {
    caffe_copy(count_, top[0]->gpu_diff(), bottom[0]->mutable_gpu_diff());
    return;
  }
  caffe_gpu_add(count_, top[0]->gpu_diff(), top[1]->gpu_diff(),
                bottom[0]->mutable_gpu_diff());
  // Add remaining top blob diffs.
  for (int i = 2; i < top.size(); ++i) {
    const Dtype* top_diff = top[i]->gpu_diff();
    Dtype* bottom_diff = bottom[0]->mutable_gpu_diff();
    caffe_gpu_axpy(count_, Dtype(1.), top_diff, bottom_diff);
  }
}


INSTANTIATE_LAYER_GPU_FUNCS(SplitLayer);

}  // namespace
