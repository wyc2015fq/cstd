


#include "caffe/filler.hpp"
#include "caffe/layer.hpp"
#include "caffe/custom_layers.hpp"




void InsanityLayer::LayerSetUp(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  CHECK_GE(bottom[0]->num_axes(), 2)
      << "Number of axes of bottom blob must be >=2.";
  InsanityParameter insanity_param_ = this->layer_param().insanity_param();
  int channels = bottom[0]->channels();
  lb_ = insanity_param_.lb();
  ub_ = insanity_param_.ub();
  CHECK_GT(ub_, lb_) << "upper bound must > lower bound.";
  CHECK_NE(lb_ * ub_, 0) <<"lower & upper bound must not be 0.";
  mean_slope = (ub_ - lb_) / (log(abs(ub_)) - log(abs(lb_)));
  alpha.ReshapeLike(*bottom[0]);
}


void InsanityLayer::Reshape(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  CHECK_GE(bottom[0]->num_axes(), 2)
      << "Number of axes of bottom blob must be >=2.";
  top[0]->ReshapeLike(*bottom[0]);
  if (bottom[0] == top[0] && lb_ < 0) {
    // For in-place computation
    bottom_memory_.ReshapeLike(*bottom[0]);
  }
}


void InsanityLayer::Forward_cpu(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* bottom_data = pBox_datap(bottom_blob->data);
  Dtype* top_data = top[0]->mutable_cpu_data();
  const int count = bottom[0]->count();
  const int dim = bottom[0]->count(2);
  const int channels = bottom[0]->channels();
  Dtype* slope_data = alpha.mutable_cpu_data();

  // For in-place computation
  if (bottom[0] == top[0] && lb_ < 0) {
    caffe_copy(count, bottom_data, bottom_memory_.mutable_cpu_data());
  }

  if (this->phase_ == TRAIN) {
    caffe_rng_uniform(count, lb_, ub_, slope_data);

    for (int i = 0; i < count; ++i) {
      top_data[i] = std::max(bottom_data[i], Dtype(0))
          + std::min(bottom_data[i], Dtype(0)) / slope_data[i];
    }
  }
  else{
    for (int i = 0; i < count; ++i) {
      top_data[i] = std::max(bottom_data[i], Dtype(0))
          + std::min(bottom_data[i], Dtype(0)) / mean_slope;
    }
  }
}


void InsanityLayer::Backward_cpu(const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  const Dtype* bottom_data = pBox_datap(bottom_blob->data);
  const Dtype* slope_data = alpha.cpu_data();
  const Dtype* top_diff = top[0]->cpu_diff();
  const int count = bottom[0]->count();
  const int dim = bottom[0]->count(2);
  const int channels = bottom[0]->channels();

  // For in-place computation
  if (top[0] == bottom[0] && lb_ < 0) {
    bottom_data = bottom_memory_.cpu_data();
  }

  // Propagate to bottom
  if (propagate_down[0]) {
    Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
    for (int i = 0; i < count; ++i) {
      bottom_diff[i] = top_diff[i] * ((bottom_data[i] > 0)
          + (Dtype)(bottom_data[i] <= 0) / slope_data[i]);
    }
  }
}


#ifdef CPU_ONLY
STUB_GPU(InsanityLayer);
#endif

INSTANTIATE_CLASS(InsanityLayer);
REGISTER_LAYER_CLASS(Insanity);


