#ifndef CAFFE_DECONV_LAYER_HPP_
#define CAFFE_DECONV_LAYER_HPP_

struct DeconvolutionLayer : public ConvolutionLayerBase
{
  virtual inline const char* type() const { return "Deconvolution"; }

public:
  virtual inline bool reverse_dimensions() {
    return true;
  }

  virtual void compute_output_shape(const int* input_shape)
  {
    const int* kernel_shape_data = this->kernel_.dim;
    const int* stride_data = this->stride_.dim;
    const int* pad_data = this->pad_.dim;
    const int* dilation_data = this->dilation_.dim;
    this->output_shape_ = dataShape(1);
    for (int i = 0; i < this->num_spatial_axes_; ++i) {
      // i + 1 to skip channel axis
      const int input_dim = input_shape[i + 1];
      const int kernel_extent = dilation_data[i] * (kernel_shape_data[i] - 1) + 1;
      const int output_dim = stride_data[i] * (input_dim - 1) + kernel_extent - 2 * pad_data[i];
      this->output_shape_.dim[i] = (output_dim);
    }
  }

  virtual void Forward_(const vector<Blob*> & bottom,  const vector<Blob*> & top)
  {
    const Dtype* weight = this->blobs_[0]->data();
    for (int i = 0; i < bottom.size(); ++i) {
      const Dtype* bottom_data = bottom[i]->data();
      Dtype* top_data = top[i]->mdata();
      for (int n = 0; n < this->num_; ++n) {
        this->backward_gemm(bottom_data + n * this->bottom_dim_, weight, top_data + n * this->top_dim_);
        if (this->bias_term_) {
          const Dtype* bias = this->blobs_[1]->data();
          this->forward_bias(top_data + n * this->top_dim_, bias);
        }
      }
    }
  }

  virtual void Backward_(const vector<Blob*> & top, const vector<Blob*> & bottom)
  {
    const Dtype* weight = this->blobs_[0]->data();
    Dtype* weight_diff = this->blobs_[0]->mdiff();
    for (int i = 0; i < top.size(); ++i) {
      const Dtype* top_diff = top[i]->diff();
      const Dtype* bottom_data = bottom[i]->data();
      Dtype* bottom_diff = bottom[i]->mdiff();
      // Bias gradient, if necessary.
      if (this->bias_term_ && this->blobs_[1]->propagate_down_) {
        Dtype* bias_diff = this->blobs_[1]->mdiff();
        for (int n = 0; n < this->num_; ++n) {
          this->backward_bias(bias_diff, top_diff + n * this->top_dim_);
        }
      }
      if (this->blobs_[0]->propagate_down_ || bottom[i]->propagate_down_) {
        for (int n = 0; n < this->num_; ++n) {
          // Gradient w.r.t. weight. Note that we will accumulate diffs.
          if (this->blobs_[0]->propagate_down_) {
            this->weight_gemm(top_diff + n * this->top_dim_,
              bottom_data + n * this->bottom_dim_, weight_diff);
          }
          // Gradient w.r.t. bottom data, if necessary, reusing the column buffer
          // we might have just computed above.
          if (bottom[i]->propagate_down_) {
            this->forward_gemm(top_diff + n * this->top_dim_, weight,
              bottom_diff + n * this->bottom_dim_,
              this->blobs_[0]->propagate_down_);
          }
        }
      }
    }
  }

};

INSTANTIATE_CLASS(Deconvolution, DeconvolutionLayer);

#endif  // CAFFE_DECONV_LAYER_HPP_
