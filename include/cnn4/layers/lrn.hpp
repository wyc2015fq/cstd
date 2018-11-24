#ifndef CAFFE_LRN_LAYER_HPP_
#define CAFFE_LRN_LAYER_HPP_







#include "caffe/layers/eltwise_layer.hpp"
#include "caffe/layers/pooling_layer.hpp"
#include "caffe/layers/power_layer.hpp"
#include "caffe/layers/split_layer.hpp"

namespace
{

  /**
   * @brief Normalize the input in a local region across or within feature maps.
   *
   * TODO(dox): thorough documentation for Forward_, Backward_, and proto params.
   */
  template <typename Dtype>
  class LRNLayer : public Layer
  {
  public:
    explicit LRNLayer()
      : Layer() {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top);

    virtual inline const char* type() const { return "LRN"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
    virtual void Forward_(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Forward_(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Backward_(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Backward_(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);

    virtual void CrossChannelForward(CPUContext* context, const vector<Blob*> & bottom,
                                         const vector<Blob*> & top);
    virtual void CrossChannelForward(GPUContext* context, const vector<Blob*> & bottom,
                                         const vector<Blob*> & top);
    virtual void WithinChannelForward(const vector<Blob*> & bottom,
                                      const vector<Blob*> & top);
    virtual void CrossChannelBackward(CPUContext* context, const vector<Blob*> & top,
                                          const vector<Blob*> & bottom);
    virtual void CrossChannelBackward(GPUContext* context, const vector<Blob*> & top,
                                          const vector<Blob*> & bottom);
    virtual void WithinChannelBackward(const vector<Blob*> & top,
                                       const vector<Blob*> & bottom);

    int size_;
    int pre_pad_;
    Dtype alpha_;
    Dtype beta_;
    Dtype k_;
    int num_;
    int channels_;
    int height_;
    int width_;

    // Fields used for normalization ACROSS_CHANNELS
    // scale_ stores the intermediate summing results
    Blob scale_;

    // Fields used for normalization WITHIN_CHANNEL
    SHARED_PTR<SplitLayer > split_layer_;
    vector<Blob*> split_top_vec_;
    SHARED_PTR<PowerLayer > square_layer_;
    Blob square_input_;
    Blob square_output_;
    vector<Blob*> square_bottom_vec_;
    vector<Blob*> square_top_vec_;
    SHARED_PTR<PoolingLayer > pool_layer_;
    Blob pool_output_;
    vector<Blob*> pool_top_vec_;
    SHARED_PTR<PowerLayer > power_layer_;
    Blob power_output_;
    vector<Blob*> power_top_vec_;
    SHARED_PTR<EltwiseLayer > product_layer_;
    Blob product_input_;
    vector<Blob*> product_bottom_vec_;
  };

}  // namespace

#endif  // CAFFE_LRN_LAYER_HPP_
