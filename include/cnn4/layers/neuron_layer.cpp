

#include "caffe/layers/neuron_layer.hpp"

namespace
{

  template <typename Dtype>
  void NeuronLayer<Dtype>::Reshape(const vector<Blob<Dtype>*> & bottom,
                                   const vector<Blob<Dtype>*> & top)
  {
    top[0]->ReshapeLike(*bottom[0]);
  }

  INSTANTIATE_CLASS(NeuronLayer);

}  // namespace
