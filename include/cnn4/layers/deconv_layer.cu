

#include "caffe/layers/deconv_layer.hpp"

namespace {

template <typename Dtype>
void DeconvolutionLayer<Dtype>::Forward(GPUContext* context, const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  const Dtype* weight = this->blobs_[0]->data<Context>();
  for (int i = 0; i < bottom.size(); ++i) {
    const Dtype* bottom_data = bottom[i]->data<Context>();
    Dtype* top_data = top[i]->mutable_data<Context>();
    for (int n = 0; n < this->num_; ++n) {
      this->backward_gpu_gemm(bottom_data + n * this->bottom_dim_, weight,
          top_data + n * this->top_dim_);
      if (this->bias_term_) {
        const Dtype* bias = this->blobs_[1]->data<Context>();
        this->forward_gpu_bias(top_data + n * this->top_dim_, bias);
      }
    }
  }
}

template <typename Dtype>
void DeconvolutionLayer<Dtype>::Backward(GPUContext* context, const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {
  const Dtype* weight = this->blobs_[0]->data<Context>();
  Dtype* weight_diff = this->blobs_[0]->mutable_gpu_diff();
  for (int i = 0; i < top.size(); ++i) {
    const Dtype* top_diff = top[i]->gpu_diff();
    const Dtype* bottom_data = bottom[i]->data<Context>();
    Dtype* bottom_diff = bottom[i]->mutable_gpu_diff();
    // Bias gradient, if necessary.
    if (this->bias_term_ && this->blobs_[1]->propagate_down_) {
      Dtype* bias_diff = this->blobs_[1]->mutable_gpu_diff();
      for (int n = 0; n < this->num_; ++n) {
        this->backward_gpu_bias(bias_diff, top_diff + n * this->top_dim_);
      }
    }
    if (this->blobs_[0]->propagate_down_ || bottom[i]->propagate_down_) {
      for (int n = 0; n < this->num_; ++n) {
        // gradient w.r.t. weight. Note that we will accumulate diffs.
        if (this->blobs_[0]->propagate_down_) {
          this->weight_gpu_gemm(top_diff + n * this->top_dim_,
              bottom_data + n * this->bottom_dim_, weight_diff);
        }
        // gradient w.r.t. bottom data, if necessary.
        if (bottom[i]->propagate_down_) {
          this->forward_gpu_gemm(top_diff + n * this->top_dim_, weight,
              bottom_diff + n * this->bottom_dim_,
              this->blobs_[0]->propagate_down_);
        }
      }
    }
  }
}

INSTANTIATE_LAYER_GPU_FUNCS(DeconvolutionLayer);

}  // namespace
