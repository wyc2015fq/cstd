#ifndef CAFFE_CUDNN_SOFTMAX_LAYER_HPP_
#define CAFFE_CUDNN_SOFTMAX_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"


#include "softmax_layer.hpp"



#ifdef USE_CUDNN
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
#endif



#endif  // CAFFE_CUDNN_SOFTMAX_LAYER_HPP_
