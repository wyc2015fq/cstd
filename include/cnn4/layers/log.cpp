

#include "caffe/layers/log_layer.hpp"


namespace
{

  template <typename Dtype>
  void LogLayer::LayerSetUp(const vector<Blob*> & bottom,
                                   const vector<Blob*> & top)
  {
    NeuronLayer::LayerSetUp(bottom, top);
    const Dtype base = this->param_->log_param().base();
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
    base_scale_ = Dtype(1) / log_base;
    CHECK(!isnan(base_scale_))
        << "NaN result: 1/log(base) = 1/log(" << base << ") = " << base_scale_;
    CHECK(!isinf(base_scale_))
        << "Inf result: 1/log(base) = 1/log(" << base << ") = " << base_scale_;
    input_scale_ = this->param_->log_param().scale();
    input_shift_ = this->param_->log_param().shift();
    backward_num_scale_ = input_scale_ / log_base;
  }

  template <typename Dtype>
  void LogLayer::Forward_(CPUContext* context, const vector<Blob*> & bottom,
                                    const vector<Blob*> & top)
  {
    const int count = bottom[0]->count();
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* top_data = top[0]->mdata();
    if (input_scale_ == Dtype(1) && input_shift_ == Dtype(0)) {
      caffe_log(count, bottom_data, top_data);
    } else {
      caffe_copy(count, bottom_data, top_data);
      if (input_scale_ != Dtype(1)) {
        caffe_scal(count, input_scale_, top_data);
      }
      if (input_shift_ != Dtype(0)) {
        caffe_add_scalar(count, input_shift_, top_data);
      }
      caffe_log(count, top_data, top_data);
    }
    if (base_scale_ != Dtype(1)) {
      caffe_scal(count, base_scale_, top_data);
    }
  }

  template <typename Dtype>
  void LogLayer::Backward_(CPUContext* context, const vector<Blob*> & top,
                                     const vector<Blob*> & bottom)
  {
    if (!bottom[0]->propagate_down_) { return; }
    const int count = bottom[0]->count();
    const Dtype* bottom_data = bottom[0]->data();
    const Dtype* top_diff = top[0]->diff();
    Dtype* bottom_diff = bottom[0]->mdiff();
    caffe_copy(count, bottom_data, bottom_diff);
    if (input_scale_ != Dtype(1)) {
      caffe_scal(count, input_scale_, bottom_diff);
    }
    if (input_shift_ != Dtype(0)) {
      caffe_add_scalar(count, input_shift_, bottom_diff);
    }
    caffe_powx(count, bottom_diff, Dtype(-1), bottom_diff);
    if (backward_num_scale_ != Dtype(1)) {
      caffe_scal(count, backward_num_scale_, bottom_diff);
    }
    caffe_mul(count, top_diff, bottom_diff, bottom_diff);
  }

#ifdef CPU_ONLY
  STUB_GPU(LogLayer);
#endif

  INSTANTIATE_CLASS(LogLayer);
  REGISTER_LAYER_CLASS(Log);

}  // namespace
