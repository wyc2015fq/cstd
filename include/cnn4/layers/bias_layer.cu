

#include "caffe/filler.hpp"
#include "caffe/layers/bias_layer.hpp"


namespace {

template <typename Dtype>
__global__ void BiasForward(const int n, const Dtype* in,
    const Dtype* bias, const int bias_dim, const int inner_dim,
    Dtype* out) {
  CUDA_KERNEL_LOOP(index, n) {
    const int bias_index = (index / inner_dim) % bias_dim;
    out[index] = in[index] + bias[bias_index];
  }
}

template <typename Dtype>
void BiasLayer::Forward(GPUContext* context, const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  const int count = top[0]->count();
  const Dtype* bottom_data = bottom[0]->data();
  const Dtype* bias_data =
      ((bottom.size() > 1) ? bottom[1] : this->blobs_[0].get())->data();
  Dtype* top_data = top[0]->mdata();
  BiasForward<Dtype>  // NOLINT_NEXT_LINE(whitespace/operators)
      <<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS>>>(
      count, bottom_data, bias_data, bias_dim_, inner_dim_, top_data);
}

template <typename Dtype>
void BiasLayer::Backward(GPUContext* context, const vector<Blob*>& top,
      const vector<Blob*>& bottom) {
  if (bottom[0]->propagate_down_ && bottom[0] != top[0]) {
    const Dtype* top_diff = top[0]->gpu_diff();
    Dtype* bottom_diff = bottom[0]->gpu_mdiff();
    caffe_copy(bottom[0]->count(), top_diff, bottom_diff);
  }
  // in-place, we don't need to do anything with the data diff
  const bool bias_param = (bottom.size() == 1);
  if ((!bias_param && bottom[1]->propagate_down_) ||
      (bias_param && this->blobs_[0]->propagate_down_)) {
    const Dtype* top_diff = top[0]->gpu_diff();
    Dtype* bias_diff = (bias_param ? this->blobs_[0].get() : bottom[1])
        ->gpu_mdiff();
    bool accum = bias_param;
    for (int n = 0; n < outer_dim_; ++n) {
      caffe_gpu_gemv(CblasNoTrans, bias_dim_, inner_dim_, Dtype(1),
          top_diff, bias_multiplier_.data(), Dtype(accum), bias_diff);
      top_diff += dim_;
      accum = true;
    }
  }
}

INSTANTIATE_LAYER_GPU_FUNCS(BiasLayer);

}  // namespace
