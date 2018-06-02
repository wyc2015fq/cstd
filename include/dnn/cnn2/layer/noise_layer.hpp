#ifndef CAFFE_NOISE_LAYER_HPP_
#define CAFFE_NOISE_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"


#include "neuron_layer.hpp"


  /**
  * @brief Add noise.
  */
  
  class NoiseLayer : public NeuronLayer {
  public:
    explicit NoiseLayer(const LayerParameter& param)
      : NeuronLayer(param) {}
    virtual void Reshape(const vector<Blob*>& bottom,
                         const vector<Blob*>& top);

    virtual inline const char* type() const { return "Noise"; }

  protected:
    virtual void Forward_cpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Forward_gpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Backward_cpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);
    virtual void Backward_gpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);

    Blob mask;
  };



#endif  // CAFFE_NOISE_LAYER_HPP_