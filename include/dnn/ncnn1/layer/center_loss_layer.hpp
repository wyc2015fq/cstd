#ifndef CAFFE_CENTER_LOSS_LAYER_HPP_
#define CAFFE_CENTER_LOSS_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"


#include "loss_layer.hpp"



  
  class CenterLossLayer : public LossLayer {
  public:
    explicit CenterLossLayer(const LayerParameter& param)
      : LossLayer(param) {}
    virtual void LayerSetUp(const vector<Blob*>& bottom,
                            const vector<Blob*>& top);
    virtual void Reshape(const vector<Blob*>& bottom,
                         const vector<Blob*>& top);

    virtual inline const char* type() const { return "CenterLoss"; }
    virtual inline int ExactNumBottomBlobs() const { return 2; }
    virtual inline int ExactNumTopBlobs() const { return -1; }

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
    Blob distance_;
    Blob variation_sum_;
    Blob<int> count_;
    string distance_type_;
  };



#endif  // CAFFE_CENTER_LOSS_LAYER_HPP_