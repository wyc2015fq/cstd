#ifdef USE_CUDNN

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"



#include "sigmoid_layer.hpp"



/**
 * @brief CuDNN acceleration of SigmoidLayer.
 */

class CuDNNSigmoidLayer : public SigmoidLayer {
 public:
  explicit CuDNNSigmoidLayer(const LayerParameter& param)
      : SigmoidLayer(param), handles_setup_(false) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual ~CuDNNSigmoidLayer();

 protected:
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  bool handles_setup_;
  cudnnHandle_t             handle_;
  cudnnTensorDescriptor_t bottom_desc_;
  cudnnTensorDescriptor_t top_desc_;
  cudnnActivationDescriptor_t activ_desc_;
};


void CuDNNSigmoidLayer::LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  SigmoidLayer::LayerSetUp(bottom, top);
  // initialize cuDNN
  CUDNN_CHECK(cudnnCreate(&handle_));
  cudnn::createTensor4dDesc(&bottom_desc_);
  cudnn::createTensor4dDesc(&top_desc_);
  cudnn::createActivationDescriptor(&activ_desc_,
      CUDNN_ACTIVATION_SIGMOID);
  handles_setup_ = true;
}


void CuDNNSigmoidLayer::Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  SigmoidLayer::Reshape(bottom, top);
  const int N = prev->size.n;
  const int K = prev->size.c;
  const int H = prev->size.h;
  const int W = prev->size.w;
  cudnn::setTensor4dDesc(&bottom_desc_, N, K, H, W);
  cudnn::setTensor4dDesc(&top_desc_, N, K, H, W);
}


CuDNNSigmoidLayer::~CuDNNSigmoidLayer() {
  // Check that handles have been setup before destroying.
  if (!handles_setup_) { return; }

  cudnnDestroyTensorDescriptor(this->bottom_desc_);
  cudnnDestroyTensorDescriptor(this->top_desc_);
  cudnnDestroy(this->handle_);
}

INSTANTIATE_CLASS(CuDNNSigmoidLayer);


#endif
