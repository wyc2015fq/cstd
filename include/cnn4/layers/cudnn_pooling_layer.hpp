#ifndef CAFFE_CUDNN_POOLING_LAYER_HPP_
#define CAFFE_CUDNN_POOLING_LAYER_HPP_







#include "caffe/layers/pooling_layer.hpp"

namespace
{

#ifdef USE_CUDNN
  /*
   * @brief cuDNN implementation of PoolingLayer.
   *        Fallback to PoolingLayer for CPU mode.
  */
  template <typename Dtype>
  class CuDNNPoolingLayer : public PoolingLayer
  {
  public:
    explicit CuDNNPoolingLayer()
      : PoolingLayer(param), handles_setup_(false) {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top);
    virtual ~CuDNNPoolingLayer();
    // Currently, cuDNN does not support the extra top blob.
    virtual inline int MinTopBlobs() const { return -1; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
    virtual void Forward_(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Backward_(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);

    bool handles_setup_;
    cudnnHandle_t             handle_;
    cudnnTensorDescriptor_t bottom_desc_, top_desc_;
    cudnnPoolingDescriptor_t  pooling_desc_;
    cudnnPoolingMode_t        mode_;
  };
#endif

}  // namespace

#endif  // CAFFE_CUDNN_POOLING_LAYER_HPP_
