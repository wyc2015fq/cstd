#include <algorithm>
#include <cfloat>


#include "caffe/layers/softmax_loss_layer_multi_label.hpp"


namespace {

template <typename Dtype>
__global__ void SoftmaxLossForwardGPU1(const int nthreads,
    const Dtype* prob_data, const Dtype* label, Dtype* loss,
    const int num, const int dim, const int spatial_dim,
    const bool has_ignore_label_, const int ignore_label_,
    Dtype* counts) {
  CUDA_KERNEL_LOOP(index, nthreads) {
    const int n = index / spatial_dim;//batch index
    const int s = index % spatial_dim;//label index
    const int label_value = static_cast<int>(label[index]);
    if (has_ignore_label_ && label_value == ignore_label_) {
      loss[index] = 0;
      counts[index] = 0;
    } else {
      loss[index] = -log(max(prob_data[n * dim + label_value * spatial_dim + s],
                      Dtype(FLT_MIN)));
      counts[index] = 1;
    }
  }
}

template <typename Dtype>
void SoftmaxWithLossMultiLabelLayer::Forward_gpu(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
  softmax_layer_->runForward(softmax_bottom_vec_, softmax_top_vec_);
  const Dtype* prob_data = prob_.data();
  const Dtype* label = bottom[1]->data();
  const int dim = prob_.count() / outer_num_;
  const int nthreads = outer_num_ * inner_num_;
  // Since this memory is not used for anything until it is overwritten
  // on the backward pass, we use it here to avoid having to allocate new GPU
  // memory to accumulate intermediate results in the kernel.
  Dtype* loss_data = bottom[0]->gpu_mdiff();
  // Similarly, this memory is never used elsewhere, and thus we can use it
  // to avoid having to allocate additional GPU memory.
  Dtype* counts = prob_.gpu_mdiff();
  // NOLINT_NEXT_LINE(whitespace/operators)
  SoftmaxLossForwardGPU<Dtype><<<CAFFE_GET_BLOCKS(nthreads),
      CAFFE_CUDA_NUM_THREADS>>>(nthreads, prob_data, label, loss_data,
      outer_num_, dim, inner_num_,
      has_ignore_label_, ignore_label_, counts);
  Dtype loss;
  caffe_gpu_asum(nthreads, loss_data, &loss);
  Dtype valid_count = -1;
  // Only launch another CUDA kernel if we actually need the count of valid
  // outputs.
  if (normalization_ == LossParameter_NormalizationMode_VALID &&
      has_ignore_label_) {
    caffe_gpu_asum(nthreads, counts, &valid_count);
  }
  top[0]->mdata()[0] = loss / get_normalizer(normalization_,
    (int)valid_count);
  if (top.size() == 2) {
    top[1]->ShareData(prob_);
  }
}

template <typename Dtype>
__global__ void SoftmaxLossBackwardGPU1(const int nthreads, const Dtype* top,
    const Dtype* label, Dtype* bottom_diff, const int num, const int dim,
    const int spatial_dim,
    const bool has_ignore_label_, const int ignore_label_, Dtype* counts) 
{
  const int channels = dim / spatial_dim;

  CUDA_KERNEL_LOOP(index, nthreads) {
    const int n = index / spatial_dim;
    const int s = index % spatial_dim;

    const int label_value = static_cast<int>(label[index]);

    if (has_ignore_label_ && label_value == ignore_label_) 
	{
		for (int c = 0; c < channels; ++c)
		{
			bottom_diff[n * dim + c * spatial_dim + s] = 0;
		}
		counts[index] = 0;
    } 
	else 
	{
		bottom_diff[n * dim + label_value * spatial_dim + s] -= 1;
		counts[index] = 1;
    }

  }
}

template <typename Dtype>
void SoftmaxWithLossMultiLabelLayer::Backward(GPUContext* context, const vector<Blob*>& top,
    const vector<Blob*>& bottom) {
  if (bottom[1]->propagate_down_) {
    LOG(FATAL) << this->type()
               << " Layer cannot backpropagate to label inputs.";
  }
  if (bottom[0]->propagate_down_) {
    Dtype* bottom_diff = bottom[0]->gpu_mdiff();
    const Dtype* prob_data = prob_.data();
    const Dtype* top_data = top[0]->data();
    caffe_gpu_memcpy(prob_.count() * sizeof(Dtype), prob_data, bottom_diff);
    const Dtype* label = bottom[1]->data();
    const int dim = prob_.count() / outer_num_;
    const int nthreads = outer_num_ * inner_num_;
    // Since this memory is never used for anything else,
    // we use to to avoid allocating new GPU memory.
    Dtype* counts = prob_.gpu_mdiff();
    // NOLINT_NEXT_LINE(whitespace/operators)
    SoftmaxLossBackwardGPU<Dtype><<<CAFFE_GET_BLOCKS(nthreads),
        CAFFE_CUDA_NUM_THREADS>>>(nthreads, top_data, label, bottom_diff,
        outer_num_, dim, inner_num_,
        has_ignore_label_, ignore_label_, counts);

    Dtype valid_count = -1;
    // Only launch another CUDA kernel if we actually need the count of valid
    // outputs.
    if (normalization_ == LossParameter_NormalizationMode_VALID &&
        has_ignore_label_) {
      caffe_gpu_asum(nthreads, counts, &valid_count);
    }
    const Dtype loss_weight = top[0]->diff()[0] /
                              get_normalizer(normalization_, (int)valid_count);
    caffe_gpu_scal(prob_.count(), loss_weight , bottom_diff);
  }
}

INSTANTIATE_LAYER_GPU_FUNCS(SoftmaxWithLossMultiLabelLayer);

}  // namespace
