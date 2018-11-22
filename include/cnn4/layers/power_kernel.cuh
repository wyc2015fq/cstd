

#include "caffe/layers/power_layer.hpp"


namespace {

template <typename Dtype>
void PowerLayer::Forward_(GPUContext* context, const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  Dtype* top_data = top[0]->mdata();
  const int count = bottom[0]->count();
  // Special case where we can ignore the input: scale or power is 0.
  if (diff_scale_ == Dtype(0)) {
    Dtype value = (power_ == 0) ? Dtype(1) : pow(shift_, power_);
    caffe_gpu_set(count, value, top_data);
    return;
  }
  const Dtype* bottom_data = bottom[0]->data();
  caffe_copy(count, bottom_data, top_data);
  if (scale_ != Dtype(1)) {
    caffe_gpu_scal(count, scale_, top_data);
  }
  if (shift_ != Dtype(0)) {
    caffe_gpu_add_scalar(count, shift_, top_data);
  }
  if (power_ != Dtype(1)) {
    caffe_gpu_powx(count, top_data, power_, top_data);
  }
}

template <typename Dtype>
void PowerLayer::Backward_(GPUContext* context, const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  if (bottom[0]->propagate_down_) {
    Dtype* bottom_diff = bottom[0]->gpu_mdiff();
    const int count = bottom[0]->count();
    const Dtype* top_diff = top[0]->gpu_diff();
    if (diff_scale_ == Dtype(0) || power_ == Dtype(1)) {
      caffe_gpu_set(count, diff_scale_, bottom_diff);
    } else {
      const Dtype* bottom_data = bottom[0]->data();
      // Compute dy/dx = scale * power * (shift + scale * x)^(power - 1)
      //               = diff_scale * y / (shift + scale * x)
      if (power_ == Dtype(2)) {
        // Special case for y = (shift + scale * x)^2
        //     -> dy/dx = 2 * scale * (shift + scale * x)
        //              = diff_scale * shift + diff_scale * scale * x
        caffe_gpu_axpby(count, diff_scale_ * scale_, bottom_data,
            Dtype(0), bottom_diff);
        if (shift_ != Dtype(0)) {
          caffe_gpu_add_scalar(count, diff_scale_ * shift_, bottom_diff);
        }
      } else if (shift_ == Dtype(0)) {
        // Special case for y = (scale * x)^power
        //     -> dy/dx = scale * power * (scale * x)^(power - 1)
        //              = scale * power * (scale * x)^power * (scale * x)^(-1)
        //              = power * y / x
        const Dtype* top_data = top[0]->data();
        caffe_gpu_div(count, top_data, bottom_data, bottom_diff);
        caffe_gpu_scal(count, power_, bottom_diff);
      } else {
        caffe_copy(count, bottom_data, bottom_diff);
        if (scale_ != Dtype(1)) {
          caffe_gpu_scal(count, scale_, bottom_diff);
        }
        if (shift_ != Dtype(0)) {
          caffe_gpu_add_scalar(count, shift_, bottom_diff);
        }
        const Dtype* top_data = top[0]->data();
        caffe_gpu_div<Dtype>(count, top_data, bottom_diff, bottom_diff);
        if (diff_scale_ != Dtype(1)) {
          caffe_gpu_scal(count, diff_scale_, bottom_diff);
        }
      }
    }
    caffe_gpu_mul(count, top_diff, bottom_diff, bottom_diff);
  }
}

INSTANTIATE_LAYER_GPU_FUNCS(PowerLayer);


}  // namespace
