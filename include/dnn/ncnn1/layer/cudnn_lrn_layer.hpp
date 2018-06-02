#ifndef CAFFE_CUDNN_LRN_LAYER_HPP_
#define CAFFE_CUDNN_LRN_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"


#include "lrn_layer.hpp"



#ifdef USE_CUDNN

class CuDNNLRNLayer : public LRNLayer {
 public:
  explicit CuDNNLRNLayer(const LayerParameter& param)
      : LRNLayer(param), handles_setup_(false) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual ~CuDNNLRNLayer();

 protected:
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  bool handles_setup_;
  cudnnHandle_t             handle_;
  cudnnLRNDescriptor_t norm_desc_;
  cudnnTensorDescriptor_t bottom_desc_, top_desc_;

  int size_;
  Dtype alpha_, beta_, k_;
};
#endif



#endif  // CAFFE_CUDNN_LRN_LAYER_HPP_
