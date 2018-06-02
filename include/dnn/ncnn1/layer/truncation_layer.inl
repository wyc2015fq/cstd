


#include "truncation_layer.hpp"



  
  void TruncationLayer::Forward_cpu(const vector<Blob*>& bottom,
                                           const vector<Blob*>& top) {
    const Dtype* bottom_data = pBox_datap(bottom_blob->data);
    Dtype* top_data = top[0]->mutable_cpu_data();
    const int count = bottom[0]->count();
    Dtype lower_bound = this->layer_param_.truncation_param().lower_bound();
    Dtype upper_bound = this->layer_param_.truncation_param().upper_bound();
    for (int i = 0; i < count; ++i) {
      top_data[i] = std::min(std::max(bottom_data[i], lower_bound), upper_bound);
    }
  }

  
  void TruncationLayer::Backward_cpu(const vector<Blob*>& top,
                                            const vector<bool>& propagate_down,
                                            const vector<Blob*>& bottom) {
    if (propagate_down[0]) {
      const Dtype* bottom_data = pBox_datap(bottom_blob->data);
      const Dtype* top_diff = top[0]->cpu_diff();
      Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
      const int count = bottom[0]->count();
      Dtype lower_bound = this->layer_param_.truncation_param().lower_bound();
      Dtype upper_bound = this->layer_param_.truncation_param().upper_bound();

      for (int i = 0; i < count; ++i) {
        bottom_diff[i] = (bottom_data[i] > lower_bound && bottom_data[i] < upper_bound) ? top_diff[i] : 0;
      }
    }
  }


#ifdef CPU_ONLY
  STUB_GPU(TruncationLayer);
#endif

  INSTANTIATE_CLASS(TruncationLayer);
  REGISTER_LAYER_CLASS(Truncation);


