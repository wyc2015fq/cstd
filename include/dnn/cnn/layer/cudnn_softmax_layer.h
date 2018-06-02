#ifdef USE_CUDNN


#include "thrust/device_vector.h"

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"


#include "softmax_layer.hpp"


/**
 * @brief cuDNN implementation of SoftmaxLayer.
 *        Fallback to SoftmaxLayer for CPU mode.
 */

class CuDNNSoftmaxLayer : public SoftmaxLayer {
 public:
  explicit CuDNNSoftmaxLayer(const LayerParameter& param)
      : SoftmaxLayer(param), handles_setup_(false) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual ~CuDNNSoftmaxLayer();

 protected:
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_gpu(const vector<Blob*>& top,
     const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  bool handles_setup_;
  cudnnHandle_t             handle_;
  cudnnTensorDescriptor_t bottom_desc_;
  cudnnTensorDescriptor_t top_desc_;
};



void CuDNNSoftmaxLayer::LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  SoftmaxLayer::LayerSetUp(bottom, top);
  // Initialize CUDNN.
  CUDNN_CHECK(cudnnCreate(&handle_));
  cudnn::createTensor4dDesc(&bottom_desc_);
  cudnn::createTensor4dDesc(&top_desc_);
  handles_setup_ = true;
}


void CuDNNSoftmaxLayer::Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  SoftmaxLayer::Reshape(bottom, top);
  int N = this->outer_num_;
  int K = bottom[0]->shape(this->softmax_axis_);
  int H = this->inner_num_;
  int W = 1;
  cudnn::setTensor4dDesc(&bottom_desc_, N, K, H, W);
  cudnn::setTensor4dDesc(&top_desc_, N, K, H, W);
}


CuDNNSoftmaxLayer::~CuDNNSoftmaxLayer() {
  // Check that handles have been setup before destroying.
  if (!handles_setup_) { return; }

  cudnnDestroyTensorDescriptor(bottom_desc_);
  cudnnDestroyTensorDescriptor(top_desc_);
  cudnnDestroy(handle_);
}

INSTANTIATE_CLASS(CuDNNSoftmaxLayer);


#endif
