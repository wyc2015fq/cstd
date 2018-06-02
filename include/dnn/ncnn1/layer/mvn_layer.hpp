#ifndef CAFFE_MVN_LAYER_HPP_
#define CAFFE_MVN_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"




/**
 * @brief Normalizes the input to have 0-mean and/or unit (1) variance.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */

class MVNLayer : public Layer {
 public:
  explicit MVNLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "MVN"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

 protected:
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
     const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  Blob mean_, variance_, temp_;

  /// sum_multiplier is used to carry out sum using BLAS
  Blob sum_multiplier_;
  Dtype eps_;
};



#endif  // CAFFE_MVN_LAYER_HPP_
