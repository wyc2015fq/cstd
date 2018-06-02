#ifndef CAFFE_SOFT_TRUNCATION_LAYER_HPP_
#define CAFFE_SOFT_TRUNCATION_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"


#include "neuron_layer.hpp"




class SoftTruncationLayer : public NeuronLayer {
 public:
  explicit SoftTruncationLayer(const LayerParameter& param)
      : NeuronLayer(param) {}

  virtual inline const char* type() const { return "SoftTruncation"; }

 protected:
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
};



#endif  // CAFFE_SOFT_TRUNCATION_LAYER_HPP_
