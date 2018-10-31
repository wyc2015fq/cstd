#ifndef CAFFE_CUDNN_TANH_LAYER_HPP_
#define CAFFE_CUDNN_TANH_LAYER_HPP_







#include "caffe/layers/neuron_layer.hpp"
#include "caffe/layers/tanh_layer.hpp"

namespace
{

#ifdef USE_CUDNN
  /**
   * @brief CuDNN acceleration of TanHLayer.
   */
  template <typename Dtype>
  class CuDNNTanHLayer : public TanHLayer<Dtype>
  {
  public:
    explicit CuDNNTanHLayer()
      : TanHLayer<Dtype>(param), handles_setup_(false) {}
    virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom,
                            const vector<Blob<Dtype>*> & top);
    virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
                         const vector<Blob<Dtype>*> & top);
    virtual ~CuDNNTanHLayer();

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

#endif  // CAFFE_CUDNN_TANH_LAYER_HPP_
