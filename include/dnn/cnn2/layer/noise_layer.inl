// TODO (sergeyk): effect should not be dependent on phase. wasted memcpy.



#include "noise_layer.hpp"




  
  void NoiseLayer::Reshape(const vector<Blob*>& bottom,
                                      const vector<Blob*>& top) {
    mask.ReshapeLike(*bottom[0]);
  }


void NoiseLayer::Forward_cpu(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* bottom_data = Blob_datap(bottom_blob->data);
  Dtype* top_data = top[0]->mutable_cpu_data();
  const int count = bottom[0]->count();
  if (this->phase_ == TRAIN) {
    Dtype data_magnitude = sqrt(bottom[0]->sumsq_data() / Dtype(bottom[0]->count()));
    if (this->layer_param_.noise_param().has_gaussian_std()) {
      caffe_rng_gaussian(count, this->layer_param_.noise_param().bias(),
                                data_magnitude * this->layer_param_.noise_param().gaussian_std(), mask.mutable_cpu_data());
    }
    else if (this->layer_param_.noise_param().has_uniform_range()) {
      caffe_rng_uniform(count, this->layer_param_.noise_param().bias() - this->layer_param_.noise_param().uniform_range(),
                        this->layer_param_.noise_param().bias() + this->layer_param_.noise_param().uniform_range(), mask.mutable_cpu_data());
    }
    caffe_add(count, bottom_data, mask.cpu_data(), top_data);
  } else {
    caffe_copy(count, bottom_data, top_data);
  }
}


void NoiseLayer::Backward_cpu(const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  if (propagate_down[0]) {
    bottom[0]->ShareDiff(*top[0]);
  }
}


#ifdef CPU_ONLY
STUB_GPU(NoiseLayer);
#endif

INSTANTIATE_CLASS(NoiseLayer);
REGISTER_LAYER_CLASS(Noise);


