#ifndef CAFFE_RESIZE_LAYER_HPP_
#define CAFFE_RESIZE_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"


#include "neuron_layer.hpp"



  /**
  * @brief ResizeLayer resize blob using bilinear interpolation
  *
  * 	   In layer configuration, <code>is_pyramid_test</code> specifiy whether
  * 	   the output height and width is fixed. if <code> is_pyramid_test == false </code>,
  * 	   you should give the <code>height</code> and <code>width</code> for output.
  * 	   Otherwise, you should give both <code>out_height_scale</code> and
  * 	   <code>out_width_scale</code>
  */
  
  class ResizeLayer : public Layer {
  public:
    explicit ResizeLayer(const LayerParameter& param)
      : Layer(param) {}
    virtual void LayerSetUp(const vector<Blob*>& bottom,
                            const vector<Blob*>& top);
    virtual void Reshape(const vector<Blob*>& bottom,
                         const vector<Blob*>& top);

    virtual inline const char* type() const { return "Resize"; }

    virtual inline int ExactNumTopBlobs() const { return 1; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }

  protected:
    virtual void Forward_cpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Backward_cpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);
    virtual void Forward_gpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Backward_gpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);

    vector<Blob*> locs_;
    int out_height_;
    int out_width_;
    int out_channels_;
    int out_num_;

  };


#endif  // CAFFE_RESIZE_LAYER_HPP_
