#ifndef CAFFE_SPLIT_LAYER_HPP_
#define CAFFE_SPLIT_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"




/**
 * @brief Creates a "split" path in the network by copying the bottom Blob
 *        into multiple top Blob%s to be used by multiple consuming layers.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */

class SplitLayer : public Layer {
 public:
  explicit SplitLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "Split"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int MinTopBlobs() const { return 1; }

 protected:
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  int count_;
};



#endif  // CAFFE_SPLIT_LAYER_HPP_
