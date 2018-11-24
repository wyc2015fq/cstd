

#include "caffe/layers/exp_layer.hpp"


namespace {

template <typename Dtype>
void ExpLayer::Forward_(GPUContext* context, const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const int count = bottom[0]->count();
  const Dtype* bottom_data = bottom[0]->data();
  Dtype* top_data = top[0]->mdata();
  if (inner_scale_ == Dtype(1)) {
    caffe_gpu_exp(count, bottom_data, top_data);
  } else {
    caffe_gpu_scale(count, inner_scale_, bottom_data, top_data);
    caffe_gpu_exp(count, top_data, top_data);
  }
  if (outer_scale_ != Dtype(1)) {
    caffe_gpu_scal(count, outer_scale_, top_data);
  }
}

template <typename Dtype>
void ExpLayer::Backward_(GPUContext* context, const vector<Blob*>& top,
    const vector<Blob*>& bottom) {
  if (!bottom[0]->propagate_down_) { return; }
  const int count = bottom[0]->count();
  const Dtype* top_data = top[0]->data();
  const Dtype* top_diff = top[0]->gpu_diff();
  Dtype* bottom_diff = bottom[0]->gpu_mdiff();
  caffe_gpu_mul(count, top_data, top_diff, bottom_diff);
  if (inner_scale_ != Dtype(1)) {
    caffe_gpu_scal(count, inner_scale_, bottom_diff);
  }
}

INSTANTIATE_LAYER_GPU_FUNCS(ExpLayer);


}  // namespace
