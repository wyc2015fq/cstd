#ifdef USE_CUDNN


#include "caffe/layers/cudnn_lcn_layer.hpp"

namespace {

template <typename Dtype>
void CuDNNLCNLayer::Forward(GPUContext* context, const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* bottom_data = bottom[0]->data();
  Dtype* top_data = top[0]->mutable_data();

  CUDNN_CHECK(cudnnDivisiveNormalizationForward(
        handle_, norm_desc_, CUDNN_DIVNORM_PRECOMPUTED_MEANS,
        cudnn::dataType<Dtype>::one,
        bottom_desc_, bottom_data,
        NULL,  // srcMeansData
        this->tempData1, this->tempData2,
        cudnn::dataType<Dtype>::zero,
        top_desc_, top_data) );
}

template <typename Dtype>
void CuDNNLCNLayer::Backward(GPUContext* context, const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  const Dtype* top_diff = top[0]->gpu_diff();
  const Dtype* top_data = top[0]->data();
  const Dtype* bottom_data = bottom[0]->data();
  Dtype* bottom_diff = bottom[0]->mutable_gpu_diff();

  CUDNN_CHECK(cudnnDivisiveNormalizationBackward(
        handle_, norm_desc_, CUDNN_DIVNORM_PRECOMPUTED_MEANS,
        cudnn::dataType<Dtype>::one,
        bottom_desc_, bottom_data,
        NULL, top_diff,  // NULL - srcMeansData
        this->tempData1, this->tempData2,
        cudnn::dataType<Dtype>::zero,
        bottom_desc_, bottom_diff,
        NULL) );
}

INSTANTIATE_LAYER_GPU_FUNCS(CuDNNLCNLayer);

}  // namespace
#endif
