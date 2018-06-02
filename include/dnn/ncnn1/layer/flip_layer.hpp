#ifndef CAFFE_FLIP_LAYER_HPP_
#define CAFFE_FLIP_LAYER_HPP_




#include "caffe/blob.hpp"
#include "caffe/layer.hpp"




/**
 * @brief Takes a Blob and flip it along the "width" side.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */


class FlipLayer : public Layer {
 public:
  explicit FlipLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "Flip"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

 protected:
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  bool flip_width_;
  bool flip_height_;
};


#endif  // CAFFE_FLIP_LAYER_HPP_
