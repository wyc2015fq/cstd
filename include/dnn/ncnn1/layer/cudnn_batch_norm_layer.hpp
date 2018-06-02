#ifndef CAFFE_CUDNN_BATCH_NORM_LAYER_HPP_
#define CAFFE_CUDNN_BATCH_NORM_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"


#include "batch_norm_layer.hpp"



#ifdef USE_CUDNN

class CuDNNBatchNormLayer : public BatchNormLayer {
 public:
  explicit CuDNNBatchNormLayer(const LayerParameter& param)
      : BatchNormLayer(param), handles_setup_(false) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual ~CuDNNBatchNormLayer();

 protected:
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  // cuDNN descriptors / handles
  cudnnTensorDescriptor_t bottom_desc_, top_desc_;
  cudnnTensorDescriptor_t scale_bias_mean_var_desc_;
  cudnnBatchNormMode_t mode_;

  Blob save_mean_, save_inv_var_;
  bool handles_setup_;
};
#endif



#endif  // CAFFE_CUDNN_BATCH_NORM_LAYER_HPP_
