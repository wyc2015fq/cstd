#ifndef CAFFE_CUDNN_POOLING_LAYER_HPP_
#define CAFFE_CUDNN_POOLING_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"


#include "pooling_layer.hpp"



#ifdef USE_CUDNN
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
#endif



#endif  // CAFFE_CUDNN_POOLING_LAYER_HPP_
