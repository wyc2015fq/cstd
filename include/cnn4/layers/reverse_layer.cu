

#include "caffe/layers/reverse_layer.hpp"


namespace {

template <typename Dtype>
void ReverseLayer::Forward(GPUContext* context, const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* src = bottom[0]->data();

  const int count = top[0]->count();
  const int axis_count = top[0]->count(axis_);
  const int copy_amount
        = (axis_ + 1 == top[0]->num_axes()) ? 1 : top[0]->count(axis_ + 1);
  const int num_fix = (axis_ > 0) ? count / axis_count : 1;
  const int sub_iter_max = top[0]->shape(axis_);

  for (int fix = 0; fix < num_fix; ++fix) {
    Dtype* target = top[0]->mutable_data()
            + (fix + 1) * copy_amount * sub_iter_max - copy_amount;
    for (int i = 0; i < sub_iter_max; ++i) {
      caffe_copy(copy_amount, src, target);
      src += copy_amount;     // normal order
      target -= copy_amount;  // reverse order
    }
  }
}

template <typename Dtype>
void ReverseLayer::Backward(GPUContext* context, const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  if (!bottom[0]->propagate_down_) { return; }

  Dtype* target = bottom[0]->mutable_gpu_diff();

  const int count = top[0]->count();
  const int axis_count = top[0]->count(axis_);
  const int copy_amount
        = (axis_ + 1 == top[0]->num_axes()) ? 1 : top[0]->count(axis_ + 1);
  const int num_fix = (axis_ > 0) ? count / axis_count : 1;
  const int sub_iter_max = top[0]->shape(axis_);

  for (int fix = 0; fix < num_fix; ++fix) {
    const Dtype* src = top[0]->gpu_diff()
            + (fix + 1) * copy_amount * sub_iter_max - copy_amount;
    for (int i = 0; i < sub_iter_max; ++i) {
      caffe_copy(copy_amount, src, target);
      target += copy_amount;  // normal order
      src -= copy_amount;     // reverse order
    }
  }
}

INSTANTIATE_LAYER_GPU_FUNCS(ReverseLayer);


}  // namespace
