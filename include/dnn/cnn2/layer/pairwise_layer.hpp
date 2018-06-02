#ifndef CAFFE_PAIRWISE_LAYER_HPP_
#define CAFFE_PAIRWISE_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"




/**
 * @brief Compute elementwise operations, such as product and sum,
 *        along multiple input Blobs.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */

class PairwiseLayer : public Layer {
 public:
  explicit PairwiseLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "Pairwise"; }
  virtual inline int ExactBottomBlobs() const { return 2; }
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

  PairwiseParameter_PairwiseOp op_;
  vector coeffs_;
  Blob<int> max_idx_;

  int M_, N_, K_;
};



#endif  // CAFFE_PAIRWISE_LAYER_HPP_
