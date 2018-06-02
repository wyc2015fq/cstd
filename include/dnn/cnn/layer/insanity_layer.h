


#include "caffe/filler.hpp"
#include "caffe/layer.hpp"
#include "caffe/custom_layers.hpp"




void InsanityLayer::LayerSetUp(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  CHECK_GE(bottom[0]->num_axes(), 2)
      << "Number of axes of bottom blob must be >=2.";
  InsanityParameter insanity_param_ = this->layer_param().insanity_param();
  int channels = prev->size.c;
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


void InsanityLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  Dtype* nextData = nextData;
  const int count = Data_count(prev->size);
  const int dim = bottom[0]->count(2);
  const int channels = prev->size.c;
  Dtype* slope_data = alpha;

  // For in-place computation
  if (bottom[0] == top[0] && lb_ < 0) {
    blas_scopy(count, prevData, bottom_memory_);
  }

  if (this->phase_ == TRAIN) {
    blas_srng_uniform(count, lb_, ub_, slope_data);

    for (i = 0; i < count; ++i) {
      nextData[i] = MAX(prevData[i], (Dtype)(0))
          + std::min(prevData[i], (Dtype)(0)) / slope_data[i];
    }
  }
  else{
    for (i = 0; i < count; ++i) {
      nextData[i] = MAX(prevData[i], (Dtype)(0))
          + std::min(prevData[i], (Dtype)(0)) / mean_slope;
    }
  }
}


void InsanityLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  const Dtype* prevData = prevData;
  const Dtype* slope_data = alpha.cpu_data();
  const Dtype* nextDiff = nextDiff;
  const int count = Data_count(prev->size);
  const int dim = bottom[0]->count(2);
  const int channels = prev->size.c;

  // For in-place computation
  if (top[0] == bottom[0] && lb_ < 0) {
    prevData = bottom_memory_.cpu_data();
  }

  // Propagate to bottom
  if (propagate_down[0]) {
    Dtype* prevDiff = bottom[0]->mutable_diff();
    for (i = 0; i < count; ++i) {
      prevDiff[i] = nextDiff[i] * ((prevData[i] > 0)
          + (Dtype)(prevData[i] <= 0) / slope_data[i]);
    }
  }
}


#ifdef CPU_ONLY
STUB_GPU(InsanityLayer);
#endif

INSTANTIATE_CLASS(InsanityLayer);
REGISTER_LAYER_CLASS(Insanity);


