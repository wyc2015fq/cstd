#ifndef CAFFE_CUDNN_RELU_LAYER_HPP_
#define CAFFE_CUDNN_RELU_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"


#include "neuron_layer.hpp"
#include "relu_layer.hpp"



#ifdef USE_CUDNN
/**
 * @brief CuDNN acceleration of ReLULayer.
 */

class CuDNNReLULayer : public ReLULayer {
 public:
  explicit CuDNNReLULayer(const LayerParameter& param)
      : ReLULayer(param), handles_setup_(false) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual ~CuDNNReLULayer();

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
#endif



#endif  // CAFFE_CUDNN_RELU_LAYER_HPP_
