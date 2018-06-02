


#include "bnll_layer.hpp"



const float kBNLL_THRESHOLD = 50.;


void BNLLLayer::Forward_cpu(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* bottom_data = pBox_datap(bottom_blob->data);
  Dtype* top_data = top[0]->mutable_cpu_data();
  const int count = bottom[0]->count();
  for (int i = 0; i < count; ++i) {
    top_data[i] = bottom_data[i] > 0 ?
        bottom_data[i] + log(1. + exp(-bottom_data[i])) :
        log(1. + exp(bottom_data[i]));
  }
}


void BNLLLayer::Backward_cpu(const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  if (propagate_down[0]) {
    const Dtype* bottom_data = pBox_datap(bottom_blob->data);
    const Dtype* top_diff = top[0]->cpu_diff();
    Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
    const int count = bottom[0]->count();
    Dtype expval;
    for (int i = 0; i < count; ++i) {
      expval = exp(std::min(bottom_data[i], Dtype(kBNLL_THRESHOLD)));
      bottom_diff[i] = top_diff[i] * expval / (expval + 1.);
    }
  }
}

#ifdef CPU_ONLY
STUB_GPU(BNLLLayer);
#endif

INSTANTIATE_CLASS(BNLLLayer);
REGISTER_LAYER_CLASS(BNLL);


