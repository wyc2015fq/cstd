#include <vector>

#include "layers/conv_layer.hpp"

namespace caffe
{

  template <typename Dtype>
  void ConvolutionLayer<Dtype>::compute_output_shape()
  {
    const int* kernel_shape_data = this->kernel_shape_.cpu_data();
    const int* stride_data = this->stride_.cpu_data();
    const int* pad_data = this->pad_.cpu_data();
    const int* dilation_data = this->dilation_.cpu_data();
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
  void ConvolutionLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*> & bottom,
      const vector<Blob<Dtype>*> & top)
  {
    const Dtype* weight = this->blobs_[0]->cpu_data();
    for (int i = 0; i < bottom.size(); ++i) {
      const Dtype* bottom_data = bottom[i]->cpu_data();
      Dtype* top_data = top[i]->mutable_cpu_data();
      for (int n = 0; n < this->num_; ++n) {
        this->forward_cpu_gemm(bottom_data + n * this->bottom_dim_, weight,
                               top_data + n * this->top_dim_);
        if (this->bias_term_) {
          const Dtype* bias = this->blobs_[1]->cpu_data();
          this->forward_cpu_bias(top_data + n * this->top_dim_, bias);
        }
      }
    }
  }

  template <typename Dtype>
  void ConvolutionLayer<Dtype>::Backward_cpu(const vector<Blob<Dtype>*> & top,
      const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom)
  {
    const Dtype* weight = this->blobs_[0]->cpu_data();
    Dtype* weight_diff = this->blobs_[0]->mutable_cpu_diff();
    for (int i = 0; i < top.size(); ++i) {
      const Dtype* top_diff = top[i]->cpu_diff();
      const Dtype* bottom_data = bottom[i]->cpu_data();
      Dtype* bottom_diff = bottom[i]->mutable_cpu_diff();
      // Bias gradient, if necessary.
      if (this->bias_term_ && this->param_propagate_down_[1]) {
        Dtype* bias_diff = this->blobs_[1]->mutable_cpu_diff();
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



  // Get convolution layer according to engine.
  template <typename Dtype>
  SHARED_PTR(Layer<Dtype>) GetConvolutionLayer(
    CJSON* param)
  {
    ConvolutionParameter conv_param = param.convolution_param();
    ConvolutionParameter_Engine engine = conv_param.engine();
#ifdef USE_CUDNN
    bool use_dilation = false;
    for (int i = 0; i < conv_param.dilation_size(); ++i) {
      if (conv_param.dilation(i) > 1) {
        use_dilation = true;
      }
    }
#endif
    if (engine == ConvolutionParameter_Engine_DEFAULT) {
      engine = ConvolutionParameter_Engine_CAFFE;
#ifdef USE_CUDNN
      if (!use_dilation) {
        engine = ConvolutionParameter_Engine_CUDNN;
      }
#endif
    }
    if (engine == ConvolutionParameter_Engine_CAFFE) {
      return SHARED_PTR(Layer<Dtype>)(new ConvolutionLayer<Dtype>(param));
#ifdef USE_CUDNN
    }
    else if (engine == ConvolutionParameter_Engine_CUDNN) {
      if (use_dilation) {
        LOG(FATAL) << "CuDNN doesn't support the dilated convolution at Layer "
          << param.name();
      }
      return SHARED_PTR(Layer<Dtype>)(new CuDNNConvolutionLayer<Dtype>(param));
#endif
    }
    else {
      LOG(FATAL) << "Layer " << param.name() << " has unknown engine.";
    }
    return NULL;
  }

  REGISTER_LAYER_CREATOR(Convolution, GetConvolutionLayer);

}  // namespace caffe
