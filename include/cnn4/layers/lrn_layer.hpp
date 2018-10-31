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
   * TODO(dox): thorough documentation for Forward, Backward, and proto params.
   */
  template <typename Dtype>
  class LRNLayer : public Layer<Dtype>
  {
  public:
    explicit LRNLayer()
      : Layer<Dtype>() {}
    virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom,
                            const vector<Blob<Dtype>*> & top);
    virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
                         const vector<Blob<Dtype>*> & top);

    virtual inline const char* type() const { return "LRN"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
    virtual void Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);
    virtual void Forward(GPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);
    virtual void Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom);
    virtual void Backward(GPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom);

    virtual void CrossChannelForward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
                                         const vector<Blob<Dtype>*> & top);
    virtual void CrossChannelForward(GPUContext* context, const vector<Blob<Dtype>*> & bottom,
                                         const vector<Blob<Dtype>*> & top);
    virtual void WithinChannelForward(const vector<Blob<Dtype>*> & bottom,
                                      const vector<Blob<Dtype>*> & top);
    virtual void CrossChannelBackward(CPUContext* context, const vector<Blob<Dtype>*> & top,
                                          const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom);
    virtual void CrossChannelBackward(GPUContext* context, const vector<Blob<Dtype>*> & top,
                                          const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom);
    virtual void WithinChannelBackward(const vector<Blob<Dtype>*> & top,
                                       const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom);

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
    Blob<Dtype> scale_;

    // Fields used for normalization WITHIN_CHANNEL
    SHARED_PTR<SplitLayer<Dtype> > split_layer_;
    vector<Blob<Dtype>*> split_top_vec_;
    SHARED_PTR<PowerLayer<Dtype> > square_layer_;
    Blob<Dtype> square_input_;
    Blob<Dtype> square_output_;
    vector<Blob<Dtype>*> square_bottom_vec_;
    vector<Blob<Dtype>*> square_top_vec_;
    SHARED_PTR<PoolingLayer<Dtype> > pool_layer_;
    Blob<Dtype> pool_output_;
    vector<Blob<Dtype>*> pool_top_vec_;
    SHARED_PTR<PowerLayer<Dtype> > power_layer_;
    Blob<Dtype> power_output_;
    vector<Blob<Dtype>*> power_top_vec_;
    SHARED_PTR<EltwiseLayer<Dtype> > product_layer_;
    Blob<Dtype> product_input_;
    vector<Blob<Dtype>*> product_bottom_vec_;
  };

}  // namespace

#endif  // CAFFE_LRN_LAYER_HPP_
