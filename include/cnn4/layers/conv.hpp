#ifndef CAFFE_CONV_LAYER_HPP_
#define CAFFE_CONV_LAYER_HPP_

#include "conv_gemm.h"

class ConvolutionLayer : public ConvolutionLayerBase
{
public:
  virtual inline const char* type() const { return "Convolution"; }
  virtual inline bool reverse_dimensions() { return false; }
  ConvolutionLayer() {
    ConvolutionLayerBase::init();
  }
  virtual void compute_output_shape(const int* input_shape)
  {
    const int* kernel_data = this->kernel_.dim;
    const int* stride_data = this->stride_.dim;
    const int* pad_data = this->pad_.dim;
    const int* dilation_data = this->dilation_.dim;
    //this->output_shape_.clear();
    int i = 0;
    this->output_shape_ = dataShape(1);
    for (; i < this->num_spatial_axes_; ++i) {
      // i + 1 to skip channel axis
      const int input_dim = input_shape[i + 1];
      const int kernel_extent = dilation_data[i] * (kernel_data[i] - 1) + 1;
      const int output_dim = (input_dim + 2 * pad_data[i] - kernel_extent) / stride_data[i] + 1;
      this->output_shape_.dim[i] = (output_dim);
    }
  }

  virtual void Forward_(const Dtype* in, Dtype* out, const Dtype* w, const Dtype* b, int i) {
    for (int n = 0; n < this->num_; ++n) {
      this->forward_gemm(in + n * this->bottom_dim_, w, out + n * this->top_dim_);
      if (b) {
        this->forward_bias(out + n * this->top_dim_, b);
      }
    }
  }
  virtual void Forward_(Blob* in, Blob* out, Blob* w, Blob* b, int i) {
    const Dtype* bottom_data = in->data();
    const Dtype* weight_data = w->data();
    Dtype* top_data = out->mdata();
    const Dtype* bias_data = (b) ? b->data() : NULL;
    Forward_(bottom_data, top_data, weight_data, bias_data, i);
  }
  virtual void Forward_(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    Blob* w = this->blobs_[0];
    Blob* b = (this->bias_term_ && blobs_.size() > 1) ? this->blobs_[1] : NULL;
    for (int i = 0; i < bottom.size(); ++i) {
      Forward_(bottom[i], top[i], w, b, i);
    }
  }
  virtual void Backward_(Blob* in, Blob* out, Blob* w, Blob* b, int i) {
    const Dtype* top_diff = out->diff();
    const Dtype* weight = w->data();
    Dtype* weight_diff = w->mdiff();
    // Bias gradient, if necessary.
    if (this->bias_term_ && b->propagate_down_) {
      Dtype* bias_diff = b->mdiff();
      for (int n = 0; n < this->num_; ++n) {
        this->backward_bias(bias_diff, top_diff + n * this->top_dim_);
      }
    }
    if (w->propagate_down_ || in->propagate_down_) {
      const Dtype* bottom_data = in->data();
      Dtype* bottom_diff = in->mdiff();
      for (int n = 0; n < this->num_; ++n) {
        // gradient w.r.t. weight. Note that we will accumulate diffs.
        if (w->propagate_down_) {
          this->weight_gemm(bottom_data + n * this->bottom_dim_,
            top_diff + n * this->top_dim_, weight_diff);
        }
        // gradient w.r.t. bottom data, if necessary.
        if (in->propagate_down_) {
          this->backward_gemm(top_diff + n * this->top_dim_, weight,
            bottom_diff + n * this->bottom_dim_);
        }
      }
    }
  }

  virtual void Backward_(const vector<Blob*> & top, const vector<Blob*> & bottom)
  {
    Blob* w = this->blobs_[0];
    Blob* b = (this->bias_term_ && blobs_.size() > 1) ? this->blobs_[1] : NULL;
    for (int i = 0; i < top.size(); ++i) {
      Backward_(bottom[i], top[i], w, b, i);
    }
  }
};


INSTANTIATE_CLASS(Convolution, ConvolutionLayer);

#endif  // CAFFE_CONV_LAYER_HPP_
