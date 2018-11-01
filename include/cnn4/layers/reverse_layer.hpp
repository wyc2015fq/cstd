#ifndef REVERSE_LAYER_HPP
#define REVERSE_LAYER_HPP







#include "caffe/layers/neuron_layer.hpp"

namespace
{

  /*
   * @brief Reverses the data of the input Blob into the output blob.
   *
   * Note: This is a useful layer if you want to reverse the time of
   * a recurrent layer.
   */

  template <typename Dtype>
  class ReverseLayer : public NeuronLayer<Dtype>
  {
  public:
    explicit ReverseLayer();

    virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom,
                            const vector<Blob<Dtype>*> & top);

    virtual inline const char* type() const { return "Reverse"; }

  public:
    virtual void Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);
    virtual void Forward(GPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);

    virtual void Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<Blob<Dtype>*> & bottom);
    virtual void Backward(GPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<Blob<Dtype>*> & bottom);

    int axis_;
  };

}  // namespace

#endif  // REVERSE_LAYER_HPP
