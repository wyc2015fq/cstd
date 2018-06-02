#ifndef CAFFE_LARGEMARGIN_INNER_PRODUCT_LAYER_HPP_
#define CAFFE_LARGEMARGIN_INNER_PRODUCT_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"




/**
 * @brief Also known as a "LargeMargin fully-connected" layer, computes an LargeMargin inner product
 *        with a set of learned weights, and (optionally) adds biases.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */

class LargeMarginInnerProductLayer : public Layer {
 public:
  explicit LargeMarginInnerProductLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "LargeMarginInnerProduct"; }
  virtual inline int MinBottomBlobs() const { return 2; }
  virtual inline int MaxTopBlobs() const { return 2; }

 protected:
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  int M_;
  int K_;
  int N_;
  
  LargeMarginInnerProductParameter_LargeMarginType type_;

  // common variables
  Blob x_norm_;
  Blob w_norm_;
  Blob cos_theta_;
  Blob sign_0_; // sign_0 = sign(cos_theta)
  // for DOUBLE type
  Blob cos_theta_quadratic_;
  // for TRIPLE type
  Blob sign_1_; // sign_1 = sign(abs(cos_theta) - 0.5)
  Blob sign_2_; // sign_2 = sign_0 * (1 + sign_1) - 2
  Blob cos_theta_cubic_;
  // for QUADRA type
  Blob sign_3_; // sign_3 = sign_0 * sign(2 * cos_theta_quadratic_ - 1)
  Blob sign_4_; // sign_4 = 2 * sign_0 + sign_3 - 3
  Blob cos_theta_quartic_;

  int iter_;
  Dtype lambda_;

};



#endif  // CAFFE_LARGEMARGIN_INNER_PRODUCT_LAYER_HPP_
