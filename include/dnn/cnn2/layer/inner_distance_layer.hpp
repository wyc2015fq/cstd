#ifndef CAFFE_INNER_PRODUCT_LAYER_HPP_
#define CAFFE_INNER_PRODUCT_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"




/**
 * @brief Use L1 or L2 distance to replace the dot product in InnerProduct
 * layer.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */

class InnerDistanceLayer : public Layer {
 public:
  explicit InnerDistanceLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "InnerDistance"; }
  virtual inline int MinNumBottomBlobs() const { return 1; }
  virtual inline int MaxNumBottomBlobs() const { return 3; }
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

  int M_;
  int K_;
  int N_;
  bool transpose_;  ///< if true, assume transposed weights
  std::string distance_type_;
  bool normalize_;
  Blob weight_norm_;
  bool update_center_only_;
};



#endif  // CAFFE_INNER_DISTANCE_LAYER_HPP_
