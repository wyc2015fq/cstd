#ifndef CAFFE_COS_LAYER_HPP_
#define CAFFE_COS_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"


#include "neuron_layer.hpp"




class CosineLayer : public NeuronLayer {
 public:

  explicit CosineLayer(const LayerParameter& param)
      : NeuronLayer(param) {}

  virtual inline const char* type() const { return "Cosine"; }

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



#endif  // CAFFE_COS_LAYER_HPP_
