#ifndef CAFFE_CUDNN_LRN_LAYER_HPP_
#define CAFFE_CUDNN_LRN_LAYER_HPP_







#include "caffe/layers/lrn_layer.hpp"

namespace
{

#ifdef USE_CUDNN
  template <typename Dtype>
  class CuDNNLRNLayer : public LRNLayer
  {
  public:
    explicit CuDNNLRNLayer()
      : LRNLayer(param), handles_setup_(false) {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top);
    virtual ~CuDNNLRNLayer();

  public:
    virtual void Forward(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Backward(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);

    bool handles_setup_;
    cudnnHandle_t             handle_;
    cudnnLRNDescriptor_t norm_desc_;
    cudnnTensorDescriptor_t bottom_desc_, top_desc_;

    int size_;
    Dtype alpha_, beta_, k_;
  };
#endif

}  // namespace

#endif  // CAFFE_CUDNN_LRN_LAYER_HPP_
