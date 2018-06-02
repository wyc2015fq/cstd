#ifndef CAFFE_BIAS_LAYER_HPP_
#define CAFFE_BIAS_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"




/**
 * @brief Computes a sum of two input Blobs, with the shape of the latter Blob
 *        "broadcast" to match the shape of the former. Equivalent to tiling
 *        the latter Blob, then computing the elementwise sum.
 *
 * The second input may be omitted, in which case it's learned as a parameter
 * of the layer. Note: in case bias and scaling are desired, both operations can
 * be handled by `ScaleLayer` configured with `bias_term: true`.
 */

class BiasLayer : public Layer {
 public:
  explicit BiasLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "Bias"; }
  virtual inline int MinBottomBlobs() const { return 1; }
  virtual inline int MaxBottomBlobs() const { return 2; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

 private:
  Blob bias_multiplier_;
  int outer_dim_, bias_dim_, inner_dim_, dim_;
};





#endif  // CAFFE_BIAS_LAYER_HPP_
