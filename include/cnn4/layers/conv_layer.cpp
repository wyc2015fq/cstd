

#include "caffe/layers/conv_layer.hpp"

namespace
{

  template <typename Dtype>
  void ConvolutionLayer<Dtype>::compute_output_shape()
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
      const int output_dim = (input_dim + 2 * pad_data[i] - kernel_extent)
                             / stride_data[i] + 1;
      this->output_shape_.push_back(output_dim);
    }
  }

  template <typename Dtype>
  void ConvolutionLayer<Dtype>::Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
      const vector<Blob<Dtype>*> & top)
  {
    const Dtype* weight = this->blobs_[0]->data<Context>();
    for (int i = 0; i < bottom.size(); ++i) {
      const Dtype* bottom_data = bottom[i]->data<Context>();
      Dtype* top_data = top[i]->mutable_data<Context>();
      for (int n = 0; n < this->num_; ++n) {
        this->forward_cpu_gemm(bottom_data + n * this->bottom_dim_, weight,
                               top_data + n * this->top_dim_);
        if (this->bias_term_) {
          const Dtype* bias = this->blobs_[1]->data<Context>();
          this->forward_cpu_bias(top_data + n * this->top_dim_, bias);
        }
      }
    }
  }

  template <typename Dtype>
  void ConvolutionLayer<Dtype>::Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
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
          // gradient w.r.t. weight. Note that we will accumulate diffs.
          if (this->param_propagate_down_[0]) {
            this->weight_cpu_gemm(bottom_data + n * this->bottom_dim_,
                                  top_diff + n * this->top_dim_, weight_diff);
          }
          // gradient w.r.t. bottom data, if necessary.
          if (propagate_down[i]) {
            this->backward_cpu_gemm(top_diff + n * this->top_dim_, weight,
                                    bottom_diff + n * this->bottom_dim_);
          }
        }
      }
    }
  }

#ifdef CPU_ONLY
  STUB_GPU(ConvolutionLayer);
#endif

  INSTANTIATE_CLASS(ConvolutionLayer);

}  // namespace
