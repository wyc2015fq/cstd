#ifdef USE_CUDNN


#include "caffe/layers/cudnn_pooling_layer.hpp"

namespace {

template <typename Dtype>
void CuDNNPoolingLayer::Forward_(GPUContext* context, const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* bottom_data = bottom[0]->data();
  Dtype* top_data = top[0]->mdata();
  CUDNN_CHECK(cudnnPoolingForward(handle_, pooling_desc_,
        cudnn::dataType<Dtype>::one,
        bottom_desc_, bottom_data,
        cudnn::dataType<Dtype>::zero,
        top_desc_, top_data));
}

template <typename Dtype>
void CuDNNPoolingLayer::Backward_(GPUContext* context, const vector<Blob*>& top,
    const vector<Blob*>& bottom) {
  if (!bottom[0]->propagate_down_) {
    return;
  }
  const Dtype* top_diff = top[0]->gpu_diff();
  const Dtype* top_data = top[0]->data();
  const Dtype* bottom_data = bottom[0]->data();
  Dtype* bottom_diff = bottom[0]->gpu_mdiff();
  CUDNN_CHECK(cudnnPoolingBackward(handle_, pooling_desc_,
        cudnn::dataType<Dtype>::one,
        top_desc_, top_data, top_desc_, top_diff,
        bottom_desc_, bottom_data,
        cudnn::dataType<Dtype>::zero,
        bottom_desc_, bottom_diff));
}

INSTANTIATE_LAYER_GPU_FUNCS(CuDNNPoolingLayer);

}  // namespace
#endif
