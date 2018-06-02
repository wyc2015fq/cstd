#ifndef CAFFE_NORMALIZE_LAYER_HPP_
#define CAFFE_NORMALIZE_LAYER_HPP_




#include "caffe/blob.hpp"
#include "caffe/layer.hpp"



  /**
  * @brief Normalizes input.
  */
  
  class NormalizeLayer : public Layer {
  public:
    explicit NormalizeLayer(const LayerParameter& param)
      : Layer(param) {}
    virtual void LayerSetUp(const vector<Blob*>& bottom,
                            const vector<Blob*>& top);
    virtual void Reshape(const vector<Blob*>& bottom,
                         const vector<Blob*>& top);

    virtual inline const char* type() const { return "Normalize"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
    virtual inline int MinTopBlobs() const { return 1; }
    virtual inline int MaxTopBlobs() const { return 2; }

  protected:
    virtual void Forward_cpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Forward_gpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Backward_cpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);
    virtual void Backward_gpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);

    Blob sum_multiplier_, squared_, norm_;
    std::string normalize_type_;
    bool fix_gradient_;
    bool bp_norm_;
  };

}

#endif  // CAFFE_NORMALIZE_LAYER_HPP_
