

#include "caffe/layers/absval_layer.hpp"


namespace {

template <typename Dtype>
void AbsValLayer::Forward_gpu(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
  const int count = top[0]->count();
  Dtype* top_data = top[0]->mdata();
  caffe_gpu_abs(count, bottom[0]->data(), top_data);
}

template <typename Dtype>
void AbsValLayer::Backward(GPUContext* context, const vector<Blob*>& top,
    const vector<Blob*>& bottom) {
  const int count = top[0]->count();
  const Dtype* top_diff = top[0]->gpu_diff();
  if (bottom[0]->propagate_down_) {
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* bottom_diff = bottom[0]->gpu_mdiff();
    caffe_gpu_sign(count, bottom_data, bottom_diff);
    caffe_gpu_mul(count, bottom_diff, top_diff, bottom_diff);
  }
}

INSTANTIATE_LAYER_GPU_FUNCS(AbsValLayer);


}  // namespace
