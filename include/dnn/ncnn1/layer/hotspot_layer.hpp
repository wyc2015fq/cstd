#ifndef CAFFE_XXX_LAYER_HPP_
#define CAFFE_XXX_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"




  /**
  * @brief Produce point heat maps by a series of the input 2d coordinates.
  *
  * TODO(dox): thorough documentation for Forward, Backward, and proto params.
  */
  
  class HotspotLayer : public Layer {
  public:
    explicit HotspotLayer(const LayerParameter& param)
      : Layer(param) {}

    virtual void LayerSetUp(const vector<Blob*>& bottom,
                            const vector<Blob*>& top);

    virtual void Reshape(const vector<Blob*>& bottom,
                         const vector<Blob*>& top);

    virtual inline const char* type() const { return "Hotspot"; }
    virtual inline int MinBottomBlobs() const { return 1; }
    virtual inline int ExactTopBlobs() const { return 1; }

  protected:
    virtual void Forward_cpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Forward_gpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Backward_cpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);
    virtual void Backward_gpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);

    int height_;
    int width_;
    Dtype gaussian_std_;
    int data_height_;
    int data_width_;
    bool mean_removed_;
  };



#endif  // CAFFE_XXX_LAYER_HPP_
