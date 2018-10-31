#ifndef CAFFE_CUDNN_SOFTMAX_LAYER_HPP_
#define CAFFE_CUDNN_SOFTMAX_LAYER_HPP_







#include "caffe/layers/softmax_layer.hpp"

namespace
{

#ifdef USE_CUDNN
  /**
   * @brief cuDNN implementation of SoftmaxLayer.
   *        Fallback to SoftmaxLayer for CPU mode.
   */
  template <typename Dtype>
  class CuDNNSoftmaxLayer : public SoftmaxLayer<Dtype>
  {
  public:
    explicit CuDNNSoftmaxLayer()
      : SoftmaxLayer<Dtype>(param), handles_setup_(false) {}
    virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom,
                            const vector<Blob<Dtype>*> & top);
    virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
                         const vector<Blob<Dtype>*> & top);
    virtual ~CuDNNSoftmaxLayer();

  public:
    virtual void Forward(GPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);
    virtual void Backward(GPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom);

    bool handles_setup_;
    cudnnHandle_t             handle_;
    cudnnTensorDescriptor_t bottom_desc_;
    cudnnTensorDescriptor_t top_desc_;
  };
#endif

}  // namespace

#endif  // CAFFE_CUDNN_SOFTMAX_LAYER_HPP_
