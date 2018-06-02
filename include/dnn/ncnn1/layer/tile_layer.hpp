#ifndef CAFFE_TILE_LAYER_HPP_
#define CAFFE_TILE_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"




/**
 * @brief Copy a Blob along specified dimensions.
 */

class TileLayer : public Layer {
 public:
  explicit TileLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "Tile"; }
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

  unsigned int axis_, tiles_, outer_dim_, inner_dim_;
};



#endif  // CAFFE_TILE_LAYER_HPP_
