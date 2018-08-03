

#include "exp_layer.hpp"





void ExpLayer::LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  NeuronLayer::LayerSetUp(bottom, top);
  const Dtype base = this->layer_param_.exp_param().base();
  if (base != Dtype(-1)) {
    CHECK_GT(base, 0) << "base must be strictly positive.";
  }
  // If base == -1, interpret the base as e and set log_base = 1 exactly.
  // Otherwise, calculate its log explicitly.
  const Dtype log_base = (base == Dtype(-1)) ? Dtype(1) : log(base);
  CHECK(!isnan(log_base))
      << "NaN result: log(base) = log(" << base << ") = " << log_base;
  CHECK(!isinf(log_base))
      << "Inf result: log(base) = log(" << base << ") = " << log_base;
  const Dtype input_scale = this->layer_param_.exp_param().scale();
  const Dtype input_shift = this->layer_param_.exp_param().shift();
  inner_scale_ = log_base * input_scale;
  outer_scale_ = (input_shift == Dtype(0)) ? Dtype(1) :
     ( (base != Dtype(-1)) ? pow(base, input_shift) : exp(input_shift) );
}


void ExpLayer::Forward_cpu(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const int count = bottom[0]->count();
  const Dtype* bottom_data = Blob_datap(bottom_blob->data);
  Dtype* top_data = top[0]->mutable_cpu_data();
  if (inner_scale_ == Dtype(1)) {
    caffe_exp(count, bottom_data, top_data);
  } else {
    caffe_cpu_scale(count, inner_scale_, bottom_data, top_data);
    caffe_exp(count, top_data, top_data);
  }
  if (outer_scale_ != Dtype(1)) {
    caffe_scal(count, outer_scale_, top_data);
  }
}


void ExpLayer::Backward_cpu(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  if (!propagate_down[0]) { return; }
  const int count = bottom[0]->count();
  const Dtype* top_data = top[0]->cpu_data();
  const Dtype* top_diff = top[0]->cpu_diff();
  Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
  caffe_mul(count, top_data, top_diff, bottom_diff);
  if (inner_scale_ != Dtype(1)) {
    caffe_scal(count, inner_scale_, bottom_diff);
  }
}

#ifdef CPU_ONLY
STUB_GPU(ExpLayer);
#endif

INSTANTIATE_CLASS(ExpLayer);
REGISTER_LAYER_CLASS(Exp);

