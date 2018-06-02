#ifndef CAFFE_PERMUTE_LAYER_HPP_
#define CAFFE_PERMUTE_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"




  /**
  * @brief Permute the input blob by changing the memory order of the data.
  *
  * TODO(weiliu89): thorough documentation for Forward, Backward, and proto params.
  */

  // The main function which does the permute.
  
  void Permute(const int count, Dtype* bottom_data, const bool forward,
               const int* permute_order, const int* old_steps, const int* new_steps,
               const int num_axes, Dtype* top_data);

  
  class PermuteLayer : public Layer {
  public:
    explicit PermuteLayer(const LayerParameter& param)
      : Layer(param) {}
    virtual void LayerSetUp(const vector<Blob*>& bottom,
                            const vector<Blob*>& top);
    virtual void Reshape(const vector<Blob*>& bottom,
                         const vector<Blob*>& top);

    virtual inline const char* type() const { return "Permute"; }
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

    int num_axes_;
    bool need_permute_;

    // Use Blob because it is convenient to be accessible in .cu file.
    Blob<int> permute_order_;
    Blob<int> old_steps_;
    Blob<int> new_steps_;
  };



#endif  // CAFFE_PERMUTE_LAYER_HPP_