

#include "caffe/layers/deconv_layer.hpp"

namespace
{

  template <typename Dtype>
  void DeconvolutionLayer<Dtype>::compute_output_shape()
  {
    const int* kernel_shape_data = this->kernel_shape_.data<Context>();
    const int* stride_data = this->stride_.data<Context>();
    const int* pad_data = this->pad_.data<Context>();
    const int* dilation_data = this->dilation_.data<Context>();
    this->output_shape_.clear();
    for (int i = 0; i < this->num_spatial_axes_; ++i) {
      // i + 1 to skip channel axis
      const int input_dim = this->input_shape(i + 1);
      const int kernel_extent = dilation_data[i] * (kernel_shape_data[i] - 1) + 1;
      const int output_dim = stride_data[i] * (input_dim - 1)
                             + kernel_extent - 2 * pad_data[i];
      this->output_shape_.push_back(output_dim);
    }
  }

  template <typename Dtype>
  void DeconvolutionLayer<Dtype>::Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
      const vector<Blob<Dtype>*> & top)
  {
    const Dtype* weight = this->blobs_[0]->data<Context>();
    for (int i = 0; i < bottom.size(); ++i) {
      const Dtype* bottom_data = bottom[i]->data<Context>();
      Dtype* top_data = top[i]->mutable_data<Context>();
      for (int n = 0; n < this->num_; ++n) {
        this->backward_cpu_gemm(bottom_data + n * this->bottom_dim_, weight,
                                top_data + n * this->top_dim_);
        if (this->bias_term_) {
          const Dtype* bias = this->blobs_[1]->data<Context>();
          this->forward_cpu_bias(top_data + n * this->top_dim_, bias);
        }
      }
    }
  }

  template <typename Dtype>
  void DeconvolutionLayer<Dtype>::Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
      const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom)
  {
    const Dtype* weight = this->blobs_[0]->data<Context>();
    Dtype* weight_diff = this->blobs_[0]->mutable_diff<Context>();
    for (int i = 0; i < top.size(); ++i) {
      const Dtype* top_diff = top[i]->diff<Context>();
      const Dtype* bottom_data = bottom[i]->data<Context>();
      Dtype* bottom_diff = bottom[i]->mutable_diff<Context>();
      // Bias gradient, if necessary.
      if (this->bias_term_ && this->param_propagate_down_[1]) {
        Dtype* bias_diff = this->blobs_[1]->mutable_diff<Context>();
        for (int n = 0; n < this->num_; ++n) {
          this->backward_cpu_bias(bias_diff, top_diff + n * this->top_dim_);
        }
      }
      if (this->param_propagate_down_[0] || propagate_down[i]) {
        for (int n = 0; n < this->num_; ++n) {
          // Gradient w.r.t. weight. Note that we will accumulate diffs.
          if (this->param_propagate_down_[0]) {
            this->weight_cpu_gemm(top_diff + n * this->top_dim_,
                                  bottom_data + n * this->bottom_dim_, weight_diff);
          }
          // Gradient w.r.t. bottom data, if necessary, reusing the column buffer
          // we might have just computed above.
          if (propagate_down[i]) {
            this->forward_cpu_gemm(top_diff + n * this->top_dim_, weight,
                                   bottom_diff + n * this->bottom_dim_,
                                   this->param_propagate_down_[0]);
          }
        }
      }
    }
  }

#ifdef CPU_ONLY
  STUB_GPU(DeconvolutionLayer);
#endif

  INSTANTIATE_CLASS(DeconvolutionLayer);
  REGISTER_LAYER_CLASS(Deconvolution);

}  // namespace
