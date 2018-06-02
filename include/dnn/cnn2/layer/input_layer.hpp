#ifndef CAFFE_INPUT_LAYER_HPP_
#define CAFFE_INPUT_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"




/**
 * @brief Provides data to the Net by assigning tops directly.
 *
 * This data layer is a container that merely holds the data assigned to it;
 * forward, backward, and reshape are all no-ops.
 */

class InputLayer : public Layer {
 public:
  explicit InputLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  // Data layers have no bottoms, so reshaping is trivial.
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {}

  virtual inline const char* type() const { return "Input"; }
  virtual inline int ExactNumBottomBlobs() const { return 0; }
  virtual inline int MinTopBlobs() const { return 1; }

 protected:
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {}
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom) {}
};



#endif  // CAFFE_INPUT_LAYER_HPP_
