
#include <algorithm>
#include <cfloat>

__global__ void FUN(SoftmaxLossForwardGPU)(const int nthreads,
          const Dtype* prob_data, const Dtype* label, Dtype* loss,
          const int num, const int dim, const int spatial_dim,
          const bool has_ignore_label_, const int ignore_label_,
          Dtype* counts) {
  CUDA_KERNEL_LOOP(index, nthreads) {
    const int n = index / spatial_dim;
    const int s = index % spatial_dim;
    const int label_value = static_cast<int>(label[n * spatial_dim + s]);
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

__global__ void FUN(SoftmaxLossBackwardGPU)(const int nthreads, const Dtype* top,
          const Dtype* label, Dtype* bottom_diff, const int num, const int dim,
          const int spatial_dim, const bool has_ignore_label_,
          const int ignore_label_, Dtype* counts) {
  const int channels = dim / spatial_dim;

  CUDA_KERNEL_LOOP(index, nthreads) {
    const int n = index / spatial_dim;
    const int s = index % spatial_dim;
    const int label_value = static_cast<int>(label[n * spatial_dim + s]);

    if (has_ignore_label_ && label_value == ignore_label_) {
      for (int c = 0; c < channels; ++c) {
        bottom_diff[n * dim + c * spatial_dim + s] = 0;
      }
      counts[index] = 0;
    } else {
      bottom_diff[n * dim + label_value * spatial_dim + s] -= 1;
      counts[index] = 1;
    }
  }
}


int FUN(softmaxloss_forward)(const Dtype* prob_data, const Dtype* label,
  const int outer_num_, const int dim, const int inner_num_,
  const bool has_ignore_label_, const int ignore_label_, Dtype* out_loss) {
  const int nthreads = outer_num_ * inner_num_;
  BufData<Dtype> counts_buf(GPU, nthreads);
  BufData<Dtype> loss_buf(GPU, nthreads);
  Dtype* counts = counts_buf.get();
  Dtype* loss_data = loss_buf.get();
  FUN(SoftmaxLossForwardGPU) << <CAFFE_GET_BLOCKS(nthreads),
    CAFFE_CUDA_NUM_THREADS >> >(nthreads, prob_data, label, loss_data,
      outer_num_, dim, inner_num_, has_ignore_label_, ignore_label_, counts);
  FUN(caffe_asum2)(nthreads, loss_data, out_loss);
  Dtype valid_count = 0;
  // Only launch another CUDA kernel if we actually need the count of valid
  // outputs.
  FUN(caffe_asum2)(nthreads, counts, &valid_count);
  valid_count -= 1;
  return (int)valid_count;
}

int FUN(softmaxloss_backward)(const Dtype* top_data,
  const Dtype* label, Dtype* bottom_diff, const int outer_num_, const int dim,
  const int inner_num_, const bool has_ignore_label_,
  const int ignore_label_) {
  const int nthreads = outer_num_ * inner_num_;
  BufData<Dtype> counts_buf(GPU, nthreads);
  Dtype* counts = counts_buf.get();

  Dtype valid_count = 0;
  FUN(SoftmaxLossBackwardGPU) << <CAFFE_GET_BLOCKS(nthreads),
    CAFFE_CUDA_NUM_THREADS >> >(nthreads, top_data, label, bottom_diff,
      outer_num_, dim, inner_num_, has_ignore_label_, ignore_label_, counts);
  FUN(caffe_asum2)(nthreads, counts, &valid_count);
  valid_count -= 1;
  return (int)valid_count;
}
