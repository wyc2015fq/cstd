#ifndef CAFFE_CUDNN_SIGMOID_LAYER_HPP_
#define CAFFE_CUDNN_SIGMOID_LAYER_HPP_







#include "caffe/layers/neuron_layer.hpp"
#include "caffe/layers/sigmoid_layer.hpp"

namespace
{

#ifdef USE_CUDNN
  /**
   * @brief CuDNN acceleration of SigmoidLayer.
   */
  template <typename Dtype>
  class CuDNNSigmoidLayer : public SigmoidLayer<Dtype>
  {
  public:
    explicit CuDNNSigmoidLayer()
      : SigmoidLayer<Dtype>(param), handles_setup_(false) {}
    virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom,
                            const vector<Blob<Dtype>*> & top);
    virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
                         const vector<Blob<Dtype>*> & top);
    virtual ~CuDNNSigmoidLayer();

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

#endif  // CAFFE_CUDNN_SIGMOID_LAYER_HPP_
