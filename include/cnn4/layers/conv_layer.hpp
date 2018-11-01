#ifndef CAFFE_CONV_LAYER_HPP_
#define CAFFE_CONV_LAYER_HPP_

/**
 * @brief Convolves the input image with a bank of learned filters,
 *        and (optionally) adds biases.
 *
 *   Caffe convolves by reduction to matrix multiplication. This achieves
 *   high-throughput and generality of input and filter dimensions but comes at
 *   the cost of memory for matrices. This makes use of efficiency in BLAS.
 *
 *   The input is "im2col" transformed to a channel K' x H x W data matrix
 *   for multiplication with the N x K' x H x W filter matrix to yield a
 *   N' x H x W output matrix that is then "col2im" restored. K' is the
 *   input channel * kernel height * kernel width dimension of the unrolled
 *   inputs so that the im2col matrix has a column for each input region to
 *   be filtered. col2im restores the output spatial structure by rolling up
 *   the output channel N' columns of the output matrix.
 */
template <typename Dtype>
class ConvolutionLayer : public BaseConvolutionLayer<Dtype>
{
public:
  /**
   * @param param provides ConvolutionParameter convolution_param,
   *    with ConvolutionLayer options:
   *  - num_output. The number of filters.
   *  - kernel_size / kernel_h / kernel_w. The filter dimensions, given by
   *  kernel_size for square filters or kernel_h and kernel_w for rectangular
   *  filters.
   *  - stride / stride_h / stride_w (\b optional, default 1). The filter
   *  stride, given by stride_size for equal dimensions or stride_h and stride_w
   *  for different strides. By default the convolution is dense with stride 1.
   *  - pad / pad_h / pad_w (\b optional, default 0). The zero-padding for
   *  convolution, given by pad for equal dimensions or pad_h and pad_w for
   *  different padding. Input padding is computed implicitly instead of
   *  actually padding.
   *  - dilation (\b optional, default 1). The filter
   *  dilation, given by dilation_size for equal dimensions for different
   *  dilation. By default the convolution has dilation 1.
   *  - group (\b optional, default 1). The number of filter groups. Group
   *  convolution is a method for reducing parameterization by selectively
   *  connecting input and output channels. The input and output channel dimensions must be divisible
   *  by the number of groups. For group @f$ \geq 1 @f$, the
   *  convolutional filters' input and output channels are separated s.t. each
   *  group takes 1 / group of the input channels and makes 1 / group of the
   *  output channels. Concretely 4 input channels, 8 output channels, and
   *  2 groups separate input channels 1-2 and output channels 1-4 into the
   *  first group and input channels 3-4 and output channels 5-8 into the second
   *  group.
   *  - bias_term (\b optional, default true). Whether to have a bias.
   *  - engine: convolution has CAFFE (matrix multiplication) and CUDNN (library
   *    kernels + stream parallelism) engines.
   */


  virtual inline const char* type() const { return "Convolution"; }


  virtual inline bool reverse_dimensions() { return false; }
  virtual void compute_output_shape()
  {
    const int* kernel_shape_data = this->kernel_shape_.dim;
    const int* stride_data = this->stride_.dim;
    const int* pad_data = this->pad_.dim;
    const int* dilation_data = this->dilation_.dim;
    //this->output_shape_.clear();
    int i = 0;
    for (; i < this->num_spatial_axes_; ++i) {
      // i + 1 to skip channel axis
      const int input_dim = this->input_shape(i + 1);
      const int kernel_extent = dilation_data[i] * (kernel_shape_data[i] - 1) + 1;
      const int output_dim = (input_dim + 2 * pad_data[i] - kernel_extent) / stride_data[i] + 1;
      this->output_shape_.dim[i] = (output_dim);
    }
    for (; i < MAX_DIM; ++i) { this->output_shape_.dim[i] = 1; }
  }

  virtual void Forward(Context* context, const vector<Blob<Dtype>*> & bottom,
    const vector<Blob<Dtype>*> & top)
  {
    const Dtype* weight = this->blobs_[0]->data<Context>();
    for (int i = 0; i < bottom.size(); ++i) {
      const Dtype* bottom_data = bottom[i]->data<Context>();
      Dtype* top_data = top[i]->mutable_data<Context>();
      for (int n = 0; n < this->num_; ++n) {
        this->forward_gemm(bottom_data + n * this->bottom_dim_, weight,
          top_data + n * this->top_dim_);
        if (this->bias_term_) {
          const Dtype* bias = this->blobs_[1]->data<Context>();
          this->forward_bias(top_data + n * this->top_dim_, bias);
        }
      }
    }
  }

  virtual void Backward(Context* context, const vector<Blob<Dtype>*> & top,
    const vector<Blob<Dtype>*> & bottom)
  {
    const Dtype* weight = this->blobs_[0]->data<Context>();
    Dtype* weight_diff = this->blobs_[0]->mutable_diff<Context>();
    for (int i = 0; i < top.size(); ++i) {
      const Dtype* top_diff = top[i]->diff<Context>();
      // Bias gradient, if necessary.
      if (this->bias_term_ && this->blobs_[1]->propagate_down_) {
        Dtype* bias_diff = this->blobs_[1]->mutable_diff<Context>();
        for (int n = 0; n < this->num_; ++n) {
          this->backward_bias(bias_diff, top_diff + n * this->top_dim_);
        }
      }
      if (this->blobs_[0]->propagate_down_ || top[i]->propagate_down_) {
        const Dtype* bottom_data = bottom[i]->data<Context>();
        Dtype* bottom_diff = bottom[i]->mutable_diff<Context>();
        for (int n = 0; n < this->num_; ++n) {
          // gradient w.r.t. weight. Note that we will accumulate diffs.
          if (this->blobs_[0]->propagate_down_) {
            this->weight_gemm(bottom_data + n * this->bottom_dim_,
              top_diff + n * this->top_dim_, weight_diff);
          }
          // gradient w.r.t. bottom data, if necessary.
          if (top[i]->propagate_down_) {
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
