#ifdef USE_CUDNN

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"


#include "pooling_layer.hpp"


/*
 * @brief cuDNN implementation of PoolingLayer.
 *        Fallback to PoolingLayer for CPU mode.
*/

class CuDNNPoolingLayer : public PoolingLayer {
 public:
  explicit CuDNNPoolingLayer(const LayerParameter& param)
      : PoolingLayer(param), handles_setup_(false) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual ~CuDNNPoolingLayer();
  // Currently, cuDNN does not support the extra top blob.
  virtual inline int MinTopBlobs() const { return -1; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

 protected:
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  bool handles_setup_;
  cudnnHandle_t             handle_;
  cudnnTensorDescriptor_t bottom_desc_, top_desc_;
  cudnnPoolingDescriptor_t  pooling_desc_;
  cudnnPoolingMode_t        mode_;
};


void CuDNNPoolingLayer::LayerSetUp(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  PoolingLayer::LayerSetUp(bottom, top);
  CUDNN_CHECK(cudnnCreate(&handle_));
  cudnn::createTensor4dDesc(&bottom_desc_);
  cudnn::createTensor4dDesc(&top_desc_);
  cudnn::createPoolingDesc(&pooling_desc_,
      this->layer_param_.pooling_param().pool(), &mode_,
      this->kernel_h_, this->kernel_w_, this->pad_h_, this->pad_w_,
      this->stride_h_, this->stride_w_);
  handles_setup_ = true;
}


void CuDNNPoolingLayer::Reshape(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  PoolingLayer::Reshape(bottom, top);
  cudnn::setTensor4dDesc(&bottom_desc_, prev->size.n,
      this->channels_, this->height_, this->width_);
  cudnn::setTensor4dDesc(&top_desc_, prev->size.n,
      this->channels_, this->pooled_height_, this->pooled_width_);
}


CuDNNPoolingLayer::~CuDNNPoolingLayer() {
  // Check that handles have been setup before destroying.
  if (!handles_setup_) { return; }

  cudnnDestroyTensorDescriptor(bottom_desc_);
  cudnnDestroyTensorDescriptor(top_desc_);
  cudnnDestroyPoolingDescriptor(pooling_desc_);
  cudnnDestroy(handle_);
}

INSTANTIATE_CLASS(CuDNNPoolingLayer);

}   // namespace caffe
#endif
