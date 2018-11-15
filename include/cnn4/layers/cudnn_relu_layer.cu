#ifdef USE_CUDNN


#include "caffe/layers/cudnn_relu_layer.hpp"

namespace {

template <typename Dtype>
void CuDNNReLULayer::Forward_(GPUContext* context, const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  // Fallback to standard Caffe for leaky ReLU.
  if (ReLULayer::param_->relu_param().negative_slope() != 0) {
    return ReLULayer::Forward_gpu(bottom, top);
  }

  const Dtype* bottom_data = bottom[0]->data();
  Dtype* top_data = top[0]->mdata();
#if CUDNN_VERSION_MIN(5, 0, 0)
  CUDNN_CHECK(cudnnActivationForward(this->handle_,
        activ_desc_,
        cudnn::dataType<Dtype>::one,
        this->bottom_desc_, bottom_data,
        cudnn::dataType<Dtype>::zero,
        this->top_desc_, top_data));
#else
  CUDNN_CHECK(cudnnActivationForward_v4(this->handle_,
        activ_desc_,
        cudnn::dataType<Dtype>::one,
        this->bottom_desc_, bottom_data,
        cudnn::dataType<Dtype>::zero,
        this->top_desc_, top_data));
#endif
}

template <typename Dtype>
void CuDNNReLULayer::Backward_(GPUContext* context, const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  if (!bottom[0]->propagate_down_) {
    return;
  }

  // Fallback to standard Caffe for leaky ReLU.
  if (ReLULayer::param_->relu_param().negative_slope() != 0) {
    return ReLULayer::Backward_gpu(top, propagate_down, bottom);
  }

  const Dtype* top_data = top[0]->data();
  const Dtype* top_diff = top[0]->gpu_diff();
  const Dtype* bottom_data = bottom[0]->data();
  Dtype* bottom_diff = bottom[0]->gpu_mdiff();
#if CUDNN_VERSION_MIN(5, 0, 0)
  CUDNN_CHECK(cudnnActivationBackward(this->handle_,
        activ_desc_,
        cudnn::dataType<Dtype>::one,
        this->top_desc_, top_data, this->top_desc_, top_diff,
        this->bottom_desc_, bottom_data,
        cudnn::dataType<Dtype>::zero,
        this->bottom_desc_, bottom_diff));
#else
  CUDNN_CHECK(cudnnActivationBackward_v4(this->handle_,
        activ_desc_,
        cudnn::dataType<Dtype>::one,
        this->top_desc_, top_data, this->top_desc_, top_diff,
        this->bottom_desc_, bottom_data,
        cudnn::dataType<Dtype>::zero,
        this->bottom_desc_, bottom_diff));
#endif
}

INSTANTIATE_LAYER_GPU_FUNCS(CuDNNReLULayer);

}  // namespace
#endif
