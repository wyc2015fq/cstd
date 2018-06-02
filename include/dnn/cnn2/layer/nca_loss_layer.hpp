#ifndef CAFFE_NCA_LOSS_LAYER_HPP_
#define CAFFE_NCA_LOSS_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"


#include "loss_layer.hpp"




class NCALossLayer : public LossLayer {
 public:
  explicit NCALossLayer(const LayerParameter& param)
      : LossLayer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
                          const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
                       const vector<Blob*>& top);
  virtual inline const char* type() const { return "NCALoss"; }
  virtual inline int ExactNumBottomBlobs() const { return -1; }
  virtual inline int ExactNumTopBlobs() const { return -1; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlobs() const { return 2; }

 protected:
  /// @copydoc NCALossLayer
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  bool min_negative_only_;
  Blob<int> min_negative_index_;
};


#endif  // CAFFE_NCA_LOSS_LAYER_HPP_
