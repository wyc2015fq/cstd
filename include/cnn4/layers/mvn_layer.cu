

#include "caffe/layers/mvn_layer.hpp"


namespace {

template <typename Dtype>
void MVNLayer::Forward(GPUContext* context, const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* bottom_data = bottom[0]->data();
  Dtype* top_data = top[0]->mutable_data();
  int num;
  if (this->param_->mvn_param().across_channels())
    num = bottom[0]->num();
  else
    num = bottom[0]->num() * bottom[0]->channels();

  int dim = bottom[0]->count() / num;

  // subtract mean
  caffe_gpu_gemv<Dtype>(CblasNoTrans, num, dim, Dtype(1. / dim), bottom_data,
      sum_multiplier_.data(), Dtype(0.), mean_.mutable_data());  // EX
  caffe_gpu_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, dim, 1, -1.,
      mean_.data(), sum_multiplier_.data(), 0.,
      temp_.mutable_data());
  caffe_gpu_add(temp_.count(), bottom_data, temp_.data(),
      top_data);  // X-EX

  if (this->param_->mvn_param().normalize_variance()) {
    // compute variance using var(X) = E((X-EX)^2)
    caffe_gpu_powx(bottom[0]->count(), top_data, Dtype(2),
        temp_.mutable_data());  // (X-EX)^2
    caffe_gpu_gemv<Dtype>(CblasNoTrans, num, dim, Dtype(1. / dim), temp_.data(),
        sum_multiplier_.data(), Dtype(0.),
        variance_.mutable_data());  // E((X-EX)^2)

    // normalize variance
    caffe_gpu_powx(variance_.count(), variance_.data(), Dtype(0.5),
          variance_.mutable_data());

    caffe_gpu_add_scalar(variance_.count(), eps_, variance_.mutable_data());

    caffe_gpu_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, dim, 1, 1.,
          variance_.data(), sum_multiplier_.data(), 0.,
          temp_.mutable_data());

    caffe_gpu_div(temp_.count(), top_data, temp_.data(), top_data);
  }
}

template <typename Dtype>
void MVNLayer::Backward(GPUContext* context, const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  const Dtype* top_diff = top[0]->gpu_diff();
  const Dtype* top_data = top[0]->data();
  const Dtype* bottom_data = bottom[0]->data();
  Dtype* bottom_diff = bottom[0]->mutable_gpu_diff();

  int num;
  if (this->param_->mvn_param().across_channels())
    num = bottom[0]->num();
  else
    num = bottom[0]->num() * bottom[0]->channels();

  int dim = bottom[0]->count() / num;

  if (this->param_->mvn_param().normalize_variance()) {
    caffe_gpu_mul(temp_.count(), top_data, top_diff, bottom_diff);
    caffe_gpu_gemv<Dtype>(CblasNoTrans, num, dim, 1., bottom_diff,
          sum_multiplier_.data(), 0., mean_.mutable_data());
    caffe_gpu_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, dim, 1, 1.,
          mean_.data(), sum_multiplier_.data(), 0.,
          bottom_diff);
    caffe_gpu_mul(temp_.count(), top_data, bottom_diff, bottom_diff);

    caffe_gpu_gemv<Dtype>(CblasNoTrans, num, dim, 1., top_diff,
            sum_multiplier_.data(), 0., mean_.mutable_data());
    caffe_gpu_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, dim, 1, 1.,
            mean_.data(), sum_multiplier_.data(), 1.,
            bottom_diff);

    caffe_gpu_axpby(temp_.count(), Dtype(1), top_diff, Dtype(-1. / dim),
        bottom_diff);

    // put the squares of bottom into temp_
    caffe_gpu_powx(temp_.count(), bottom_data, Dtype(2),
        temp_.mutable_data());

    caffe_gpu_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, dim, 1, 1.,
        variance_.data(), sum_multiplier_.data(), 0.,
        temp_.mutable_data());

    caffe_gpu_div(temp_.count(), bottom_diff, temp_.data(), bottom_diff);
  } else {
    caffe_gpu_gemv<Dtype>(CblasNoTrans, num, dim, (Dtype)(1. / dim), top_diff,
            sum_multiplier_.data(), (Dtype)0., mean_.mutable_data());
    caffe_gpu_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, dim, 1, -1.,
            mean_.data(), sum_multiplier_.data(), 0.,
            temp_.mutable_data());
    caffe_gpu_add(temp_.count(), top_diff, temp_.data(), bottom_diff);
  }
}


INSTANTIATE_LAYER_GPU_FUNCS(MVNLayer);


}  // namespace
