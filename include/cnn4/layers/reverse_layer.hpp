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
  class ReverseLayer : public NeuronLayer
  {
  public:
    explicit ReverseLayer();

    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);

    virtual inline const char* type() const { return "Reverse"; }

  public:
    virtual void Forward_(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Forward_(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);

    virtual void Backward_(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Backward_(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);

    int axis_;
  };

}  // namespace

#endif  // REVERSE_LAYER_HPP
