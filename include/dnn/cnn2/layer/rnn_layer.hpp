#ifndef CAFFE_RNN_LAYER_HPP_
#define CAFFE_RNN_LAYER_HPP_





#include "caffe/blob.hpp"
#include "caffe/common.hpp"
#include "caffe/layer.hpp"
#include "recurrent_layer.hpp"
#include "caffe/net.hpp"




 class RecurrentLayer;

/**
 * @brief Processes time-varying inputs using a simple recurrent neural network
 *        (RNN). Implemented as a network unrolling the RNN computation in time.
 *
 * Given time-varying inputs @f$ x_t @f$, computes hidden state @f$
 *     h_t := \tanh[ W_{hh} h_{t_1} + W_{xh} x_t + b_h ]
 * @f$, and outputs @f$
 *     o_t := \tanh[ W_{ho} h_t + b_o ]
 * @f$.
 */

class RNNLayer : public RecurrentLayer {
 public:
  explicit RNNLayer(const LayerParameter& param)
      : RecurrentLayer(param) {}

  virtual inline const char* type() const { return "RNN"; }

 protected:
  virtual void FillUnrolledNet(NetParameter* net_param) const;
  virtual void RecurrentInputBlobNames(vector<string>* names) const;
  virtual void RecurrentOutputBlobNames(vector<string>* names) const;
  virtual void RecurrentInputShapes(vector<BlobShape>* shapes) const;
  virtual void OutputBlobNames(vector<string>* names) const;
};



#endif  // CAFFE_RNN_LAYER_HPP_
