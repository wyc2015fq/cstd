#ifndef CAFFE_CONV_LAYER_HPP_
#define CAFFE_CONV_LAYER_HPP_

class ConvolutionLayer : public BaseConvolutionLayer
{
public:
  virtual inline const char* type() const { return "Convolution"; }
  virtual inline bool reverse_dimensions() { return false; }
  ConvolutionLayer() {
    BaseConvolutionLayer::init();
  }
  virtual void compute_output_shape()
  {
    const int* kernel_shape_data = this->kernel_shape_.dim;
    const int* stride_data = this->stride_.dim;
    const int* pad_data = this->pad_.dim;
    const int* dilation_data = this->dilation_.dim;
    //this->output_shape_.clear();
    int i = 0;
    this->output_shape_ = dataShape(1);
    for (; i < this->num_spatial_axes_; ++i) {
      // i + 1 to skip channel axis
      const int input_dim = this->input_shape(i + 1);
      const int kernel_extent = dilation_data[i] * (kernel_shape_data[i] - 1) + 1;
      const int output_dim = (input_dim + 2 * pad_data[i] - kernel_extent) / stride_data[i] + 1;
      this->output_shape_.dim[i] = (output_dim);
    }
  }

  virtual void Forward_(const vector<Blob*> & bottom,
    const vector<Blob*> & top)
  {
    const Dtype* weight = this->blobs_[0]->data();
    for (int i = 0; i < bottom.size(); ++i) {
      const Dtype* bottom_data = bottom[i]->data();
      Dtype* top_data = top[i]->mdata();
      for (int n = 0; n < this->num_; ++n) {
        this->forward_gemm(bottom_data + n * this->bottom_dim_, weight,
          top_data + n * this->top_dim_);
        if (this->bias_term_) {
          const Dtype* bias = this->blobs_[1]->data();
          this->forward_bias(top_data + n * this->top_dim_, bias);
        }
      }
    }
  }

  virtual void Backward_(const vector<Blob*> & top,
    const vector<Blob*> & bottom)
  {
    const Dtype* weight = this->blobs_[0]->data();
    Dtype* weight_diff = this->blobs_[0]->mdiff();
    for (int i = 0; i < top.size(); ++i) {
      const Dtype* top_diff = top[i]->diff();
      // Bias gradient, if necessary.
      if (this->bias_term_ && this->blobs_[1]->propagate_down_) {
        Dtype* bias_diff = this->blobs_[1]->mdiff();
        for (int n = 0; n < this->num_; ++n) {
          this->backward_bias(bias_diff, top_diff + n * this->top_dim_);
        }
      }
      if (this->blobs_[0]->propagate_down_ || bottom[i]->propagate_down_) {
        const Dtype* bottom_data = bottom[i]->data();
        Dtype* bottom_diff = bottom[i]->mdiff();
        for (int n = 0; n < this->num_; ++n) {
          // gradient w.r.t. weight. Note that we will accumulate diffs.
          if (this->blobs_[0]->propagate_down_) {
            this->weight_gemm(bottom_data + n * this->bottom_dim_,
              top_diff + n * this->top_dim_, weight_diff);
          }
          // gradient w.r.t. bottom data, if necessary.
          if (bottom[i]->propagate_down_) {
            this->backward_gemm(top_diff + n * this->top_dim_, weight,
              bottom_diff + n * this->bottom_dim_);
          }
        }
      }
    }
  }
};


INSTANTIATE_CLASS(Convolution);

#endif  // CAFFE_CONV_LAYER_HPP_
