
#include <algorithm>
#include <cfloat>
#include "thrust/device_vector.h"


__global__ void FUN(kernel_channel_max)(const int num, const int channels, const int spatial_dim, const Dtype* data, Dtype* out) {
  CUDA_KERNEL_LOOP(index, num * spatial_dim) {
    int n = index / spatial_dim;
    int s = index % spatial_dim;
    Dtype maxval = -FLT_MAX;
    for (int c = 0; c < channels; ++c) {
      maxval = max(data[(n * channels + c) * spatial_dim + s], maxval);
    }
    out[index] = maxval;
  }
}

__global__ void FUN(kernel_channel_subtract)(const int count, const int num, const int channels,
    const int spatial_dim, const Dtype* channel_max, Dtype* data) {
  CUDA_KERNEL_LOOP(index, count) {
    int n = index / channels / spatial_dim;
    int s = index % spatial_dim;
    data[index] -= channel_max[n * spatial_dim + s];
  }
}

__global__ void FUN(kernel_exp)(const int count, const Dtype* data, Dtype* out) {
  CUDA_KERNEL_LOOP(index, count) {
    out[index] = exp(data[index]);
  }
}

__global__ void FUN(kernel_channel_sum)(const int num, const int channels, const int spatial_dim, const Dtype* data, Dtype* channel_sum) {
  CUDA_KERNEL_LOOP(index, num * spatial_dim) {
    int n = index / spatial_dim;
    int s = index % spatial_dim;
    Dtype sum = 0;
    for (int c = 0; c < channels; ++c) {
      sum += data[(n * channels + c) * spatial_dim + s];
    }
    channel_sum[index] = sum;
  }
}

__global__ void FUN(kernel_channel_div)(const int count,  const int num, const int channels,
  const int spatial_dim, const Dtype* channel_sum, Dtype* data) {
  CUDA_KERNEL_LOOP(index, count) {
    int n = index / channels / spatial_dim;
    int s = index % spatial_dim;
    data[index] /= channel_sum[n * spatial_dim + s];
  }
}

__global__ void FUN(kernel_channel_dot)(const int num, const int channels,
    const int spatial_dim, const Dtype* data_1, const Dtype* data_2,
    Dtype* channel_dot) {
  CUDA_KERNEL_LOOP(index, num * spatial_dim) {
    int n = index / spatial_dim;
    int s = index % spatial_dim;
    Dtype dot = 0;
    for (int c = 0; c < channels; ++c) {
      dot += (data_1[(n * channels + c) * spatial_dim + s] * data_2[(n * channels + c) * spatial_dim + s]);
    }
    channel_dot[index] = dot;
  }
}

void FUN(softmax_forward)(int count, int channels, int outer_num_, int inner_num_, const Dtype* bottom_data, Dtype* top_data, Dtype* scale_data) {
  //const Dtype* bottom_data = bottom[0]->data();
  //Dtype* top_data = top[0]->mdata();
  //Dtype* scale_data = scale_.mdata();
  //int count = bottom[0]->count();
  //int channels = top[0]->shape(softmax_axis_);
  //caffe_set(inner_num_, 0, scale_data);
  FUN(caffe_copy)(count, bottom_data, top_data);
  // We need to subtract the max to avoid numerical issues, compute the exp,
  // and then normalize.
  // compute max
  // NOLINT_NEXT_LINE(whitespace/operators)
  FUN(kernel_channel_max) <<<CAFFE_GET_BLOCKS(outer_num_ * inner_num_), CAFFE_CUDA_NUM_THREADS>>>(outer_num_, channels, inner_num_, top_data, scale_data);
  // subtract
  // NOLINT_NEXT_LINE(whitespace/operators)
  FUN(kernel_channel_subtract)<<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS>>>(count, outer_num_, channels, inner_num_, scale_data, top_data);
  // exponentiate
  // NOLINT_NEXT_LINE(whitespace/operators)
  FUN(kernel_exp)<<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS>>>(count, top_data, top_data);
  // sum after exp
  // NOLINT_NEXT_LINE(whitespace/operators)
  FUN(kernel_channel_sum)<<<CAFFE_GET_BLOCKS(outer_num_ * inner_num_), CAFFE_CUDA_NUM_THREADS>>>(outer_num_, channels, inner_num_, top_data, scale_data);
  // divide
  // NOLINT_NEXT_LINE(whitespace/operators)
  FUN(kernel_channel_div)<<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS>>>(count, outer_num_, channels, inner_num_, scale_data, top_data);
}

void FUN(softmax_backward)(int count, int channels, int outer_num_, int inner_num_,
  const Dtype* top_diff, const Dtype* top_data, Dtype* bottom_diff, Dtype* scale_data) {
  //const Dtype* top_diff = top[0]->gpu_diff();
  //const Dtype* top_data = top[0]->data();
  //Dtype* bottom_diff = bottom[0]->gpu_mdiff();
  //Dtype* scale_data = scale_.mdata();
  //int count = top[0]->count();
  //int channels = top[0]->shape(softmax_axis_);
  FUN(caffe_copy)(count, top_diff, bottom_diff);
  // Compute inner1d(top_diff, top_data) and subtract them from the bottom diff.
  // NOLINT_NEXT_LINE(whitespace/operators)
  FUN(kernel_channel_dot)<<<CAFFE_GET_BLOCKS(outer_num_ * inner_num_), CAFFE_CUDA_NUM_THREADS>>>(outer_num_, channels, inner_num_, top_diff, top_data, scale_data);
  // NOLINT_NEXT_LINE(whitespace/operators)
  FUN(kernel_channel_subtract)<<<CAFFE_GET_BLOCKS(count), CAFFE_CUDA_NUM_THREADS>>>(count, outer_num_, channels, inner_num_, scale_data, bottom_diff);
  // elementwise multiplication
  FUN(caffe_mul)(count, bottom_diff, top_data, bottom_diff);
}

