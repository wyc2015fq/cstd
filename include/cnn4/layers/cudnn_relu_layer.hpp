#ifndef CAFFE_CUDNN_RELU_LAYER_HPP_
#define CAFFE_CUDNN_RELU_LAYER_HPP_







#include "caffe/layers/neuron_layer.hpp"
#include "caffe/layers/relu_layer.hpp"

namespace
{

#ifdef USE_CUDNN
  /**
   * @brief CuDNN acceleration of ReLULayer.
   */
  template <typename Dtype>
  class CuDNNReLULayer : public ReLULayer<Dtype>
  {
  public:
    explicit CuDNNReLULayer()
      : ReLULayer<Dtype>(param), handles_setup_(false) {}
    virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom,
                            const vector<Blob<Dtype>*> & top);
    virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
                         const vector<Blob<Dtype>*> & top);
    virtual ~CuDNNReLULayer();

  public:
    virtual void Forward(GPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);
    virtual void Backward(GPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom);

    bool handles_setup_;
    cudnnHandle_t             handle_;
    cudnnTensorDescriptor_t bottom_desc_;
    cudnnTensorDescriptor_t top_desc_;
    cudnnActivationDescriptor_t activ_desc_;
  };
#endif

}  // namespace

#endif  // CAFFE_CUDNN_RELU_LAYER_HPP_
