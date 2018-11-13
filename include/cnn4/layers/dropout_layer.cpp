// TODO (sergeyk): effect should not be dependent on phase. wasted memcpy.



#include "caffe/layers/dropout_layer.hpp"


namespace
{

  template <typename Dtype>
  void DropoutLayer::LayerSetUp(const vector<Blob*> & bottom,
                                       const vector<Blob*> & top)
  {
    NeuronLayer::LayerSetUp(bottom, top);
    threshold_ = this->param_->dropout_param().dropout_ratio();
    DCHECK(threshold_ > 0.);
    DCHECK(threshold_ < 1.);
    scale_ = 1. / (1. - threshold_);
    uint_thres_ = static_cast<unsigned int>(UINT_MAX * threshold_);
  }

  template <typename Dtype>
  void DropoutLayer::Reshape(const vector<Blob*> & bottom,
                                    const vector<Blob*> & top)
  {
    NeuronLayer::Reshape(bottom, top);
    // Set up the cache for random number generation
    // ReshapeLike does not work because rand_vec_ is of Dtype uint
    rand_vec_.Reshape(bottom[0]->shape());
  }

  template <typename Dtype>
  void DropoutLayer::Forward(CPUContext* context, const vector<Blob*> & bottom,
                                        const vector<Blob*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* top_data = top[0]->mdata();
    unsigned int* mask = rand_vec_.mdata();
    const int count = bottom[0]->count();
    if (this->phase_ == TRAIN) {
      // Create random numbers
      caffe_rng_bernoulli(count, 1. - threshold_, mask);
      for (int i = 0; i < count; ++i) {
        top_data[i] = bottom_data[i] * mask[i] * scale_;
      }
    } else {
      caffe_copy(bottom[0]->count(), bottom_data, top_data);
    }
  }

  template <typename Dtype>
  void DropoutLayer::Backward(CPUContext* context, const vector<Blob*> & top,
                                         int*
                                         const vector<Blob*> & bottom)
  {
    if (bottom[0]->propagate_down_) {
      const Dtype* top_diff = top[0]->diff();
      Dtype* bottom_diff = bottom[0]->mdiff();
      if (this->phase_ == TRAIN) {
        const unsigned int* mask = rand_vec_.data();
        const int count = bottom[0]->count();
        for (int i = 0; i < count; ++i) {
          bottom_diff[i] = top_diff[i] * mask[i] * scale_;
        }
      } else {
        caffe_copy(top[0]->count(), top_diff, bottom_diff);
      }
    }
  }


#ifdef CPU_ONLY
  STUB_GPU(DropoutLayer);
#endif

  INSTANTIATE_CLASS(DropoutLayer);
  REGISTER_LAYER_CLASS(Dropout);

}  // namespace
