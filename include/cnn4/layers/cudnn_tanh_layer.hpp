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
  class CuDNNTanHLayer : public TanHLayer
  {
  public:
    explicit CuDNNTanHLayer()
      : TanHLayer(param), handles_setup_(false) {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top);
    virtual ~CuDNNTanHLayer();

  public:
    virtual void Forward_(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Backward_(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);

    bool handles_setup_;
    cudnnHandle_t             handle_;
    cudnnTensorDescriptor_t bottom_desc_;
    cudnnTensorDescriptor_t top_desc_;
    cudnnActivationDescriptor_t activ_desc_;
  };
#endif

}  // namespace

#endif  // CAFFE_CUDNN_TANH_LAYER_HPP_
