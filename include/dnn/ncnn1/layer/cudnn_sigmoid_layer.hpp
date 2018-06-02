#ifndef CAFFE_CUDNN_SIGMOID_LAYER_HPP_
#define CAFFE_CUDNN_SIGMOID_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"


#include "neuron_layer.hpp"
#include "sigmoid_layer.hpp"



#ifdef USE_CUDNN
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
#endif



#endif  // CAFFE_CUDNN_SIGMOID_LAYER_HPP_
