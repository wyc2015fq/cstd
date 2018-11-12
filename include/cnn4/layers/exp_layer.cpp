

#include "caffe/layers/exp_layer.hpp"


namespace
{

  template <typename Dtype>
  void ExpLayer::LayerSetUp(const vector<Blob*> & bottom,
                                   const vector<Blob*> & top)
  {
    NeuronLayer::LayerSetUp(bottom, top);
    const Dtype base = this->param_->exp_param().base();
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
    const Dtype input_scale = this->param_->exp_param().scale();
    const Dtype input_shift = this->param_->exp_param().shift();
    inner_scale_ = log_base * input_scale;
    outer_scale_ = (input_shift == Dtype(0)) ? Dtype(1) :
                   ( (base != Dtype(-1)) ? pow(base, input_shift) : exp(input_shift) );
  }

  template <typename Dtype>
  void ExpLayer::Forward(CPUContext* context, const vector<Blob*> & bottom,
                                    const vector<Blob*> & top)
  {
    const int count = bottom[0]->count();
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* top_data = top[0]->mutable_data();
    if (inner_scale_ == Dtype(1)) {
      caffe_exp(count, bottom_data, top_data);
    } else {
      caffe_scale(count, inner_scale_, bottom_data, top_data);
      caffe_exp(count, top_data, top_data);
    }
    if (outer_scale_ != Dtype(1)) {
      caffe_scal(count, outer_scale_, top_data);
    }
  }

  template <typename Dtype>
  void ExpLayer::Backward(CPUContext* context, const vector<Blob*> & top,
                                     const vector<Blob*> & bottom)
  {
    if (!bottom[0]->propagate_down_) { return; }
    const int count = bottom[0]->count();
    const Dtype* top_data = top[0]->data();
    const Dtype* top_diff = top[0]->diff();
    Dtype* bottom_diff = bottom[0]->mutable_diff();
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

}  // namespace
