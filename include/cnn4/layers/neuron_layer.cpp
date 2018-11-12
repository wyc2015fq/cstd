

#include "caffe/layers/neuron_layer.hpp"

namespace
{

  template <typename Dtype>
  void NeuronLayer::Reshape(const vector<Blob*> & bottom,
                                   const vector<Blob*> & top)
  {
    top[0]->ReshapeLike(*bottom[0]);
  }

  INSTANTIATE_CLASS(NeuronLayer);

}  // namespace
