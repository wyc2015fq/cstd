#ifndef CAFFE_NEURON_LAYER_HPP_
#define CAFFE_NEURON_LAYER_HPP_

/**
 * @brief An interface for layers that take one blob as input (@f$ x @f$)
 *        and produce one equally-sized blob as output (@f$ y @f$), where
 *        each element of the output depends only on the corresponding input
 *        element.
 */
struct NeuronLayer : public Layer
{
  virtual void Reshape(const vector<Blob*> & bottom,  const vector<Blob*> & top) {
    top[0]->ReshapeLike(*bottom[0]);
  }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int ExactNumTopBlobs() const { return 1; }
};


#endif  // CAFFE_NEURON_LAYER_HPP_
