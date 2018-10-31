

#include "caffe/layers/im2col_layer.hpp"
#include "caffe/util/im2col.hpp"

namespace {

template <typename Dtype>
void Im2colLayer<Dtype>::Forward(GPUContext* context, const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  const Dtype* bottom_data = bottom[0]->gpu_data();
  Dtype* top_data = top[0]->mutable_gpu_data();
  const int num_kernels = channels_ * top[0]->count(channel_axis_ + 1);
  for (int n = 0; n < num_; ++n) {
    if (!force_nd_im2col_ && num_spatial_axes_ == 2) {
      im2col_gpu(bottom_data + n * bottom_dim_, channels_,
          bottom[0]->shape(channel_axis_ + 1),
          bottom[0]->shape(channel_axis_ + 2),
          kernel_shape_.data<Context>()[0], kernel_shape_.data<Context>()[1],
          pad_.data<Context>()[0], pad_.data<Context>()[1],
          stride_.data<Context>()[0], stride_.data<Context>()[1],
          dilation_.data<Context>()[0], dilation_.data<Context>()[1],
          top_data + n * top_dim_);
    } else {
      im2col_nd_gpu(bottom_data + n * bottom_dim_, num_spatial_axes_,
          num_kernels, bottom[0]->gpu_shape() + channel_axis_,
          top[0]->gpu_shape() + channel_axis_,
          kernel_shape_.gpu_data(), pad_.gpu_data(), stride_.gpu_data(),
          dilation_.gpu_data(), top_data + n * top_dim_);
    }
  }
}

template <typename Dtype>
void Im2colLayer<Dtype>::Backward(GPUContext* context, const vector<Blob<Dtype>*>& top,
      const vector<bool>& propagate_down, const vector<Blob<Dtype>*>& bottom) {
  const Dtype* top_diff = top[0]->gpu_diff();
  Dtype* bottom_diff = bottom[0]->mutable_gpu_diff();
  for (int n = 0; n < num_; ++n) {
    if (!force_nd_im2col_ && num_spatial_axes_ == 2) {
      col2im_gpu(top_diff + n * top_dim_, channels_,
          bottom[0]->shape(channel_axis_ + 1),
          bottom[0]->shape(channel_axis_ + 2),
          kernel_shape_.data<Context>()[0], kernel_shape_.data<Context>()[1],
          pad_.data<Context>()[0], pad_.data<Context>()[1],
          stride_.data<Context>()[0], stride_.data<Context>()[1],
          dilation_.data<Context>()[0], dilation_.data<Context>()[1],
          bottom_diff + n * bottom_dim_);
    } else {
      col2im_nd_gpu(top_diff + n * top_dim_, num_spatial_axes_, bottom_dim_,
          bottom[0]->gpu_shape() + channel_axis_,
          top[0]->gpu_shape() + channel_axis_,
          kernel_shape_.gpu_data(), pad_.gpu_data(), stride_.gpu_data(),
          dilation_.gpu_data(), bottom_diff + n * bottom_dim_);
    }
  }
}


INSTANTIATE_LAYER_GPU_FUNCS(Im2colLayer);

}  // namespace
