#ifdef USE_CUDNN


#include "caffe/layers/cudnn_lrn_layer.hpp"

namespace {

template <typename Dtype>
void CuDNNLRNLayer::Forward_(GPUContext* context, const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* bottom_data = bottom[0]->data();
  Dtype* top_data = top[0]->mdata();

  CUDNN_CHECK(cudnnLRNCrossChannelForward(
        handle_, norm_desc_, CUDNN_LRN_CROSS_CHANNEL_DIM1,
        cudnn::dataType<Dtype>::one,
        bottom_desc_, bottom_data,
        cudnn::dataType<Dtype>::zero,
        top_desc_, top_data) );
}

template <typename Dtype>
void CuDNNLRNLayer::Backward_(GPUContext* context, const vector<Blob*>& top,
    const vector<Blob*>& bottom) {
  const Dtype* top_diff = top[0]->gpu_diff();
  const Dtype* top_data = top[0]->data();
  const Dtype* bottom_data = bottom[0]->data();
  Dtype* bottom_diff = bottom[0]->gpu_mdiff();

  CUDNN_CHECK(cudnnLRNCrossChannelBackward(
        handle_, norm_desc_, CUDNN_LRN_CROSS_CHANNEL_DIM1,
        cudnn::dataType<Dtype>::one,
        top_desc_, top_data,
        top_desc_, top_diff,
        bottom_desc_, bottom_data,
        cudnn::dataType<Dtype>::zero,
        bottom_desc_, bottom_diff) );
}

INSTANTIATE_LAYER_GPU_FUNCS(CuDNNLRNLayer);

};  // namespace

#endif
