// TODO (sergeyk): effect should not be dependent on phase. wasted memcpy.



#include "caffe/layers/dropout_layer.hpp"


namespace
{

  template <typename Dtype>
  void DropoutLayer<Dtype>::LayerSetUp(const vector<Blob<Dtype>*> & bottom,
                                       const vector<Blob<Dtype>*> & top)
  {
    NeuronLayer<Dtype>::LayerSetUp(bottom, top);
    threshold_ = this->param_->dropout_param().dropout_ratio();
    DCHECK(threshold_ > 0.);
    DCHECK(threshold_ < 1.);
    scale_ = 1. / (1. - threshold_);
    uint_thres_ = static_cast<unsigned int>(UINT_MAX * threshold_);
  }

  template <typename Dtype>
  void DropoutLayer<Dtype>::Reshape(const vector<Blob<Dtype>*> & bottom,
                                    const vector<Blob<Dtype>*> & top)
  {
    NeuronLayer<Dtype>::Reshape(bottom, top);
    // Set up the cache for random number generation
    // ReshapeLike does not work because rand_vec_ is of Dtype uint
    rand_vec_.Reshape(bottom[0]->shape());
  }

  template <typename Dtype>
  void DropoutLayer<Dtype>::Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
                                        const vector<Blob<Dtype>*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data<Context>();
    Dtype* top_data = top[0]->mutable_data<Context>();
    unsigned int* mask = rand_vec_.mutable_data<Context>();
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
  void DropoutLayer<Dtype>::Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
                                         int*
                                         const vector<Blob<Dtype>*> & bottom)
  {
    if (top[0]->propagate_down_) {
      const Dtype* top_diff = top[0]->diff<Context>();
      Dtype* bottom_diff = bottom[0]->mutable_diff<Context>();
      if (this->phase_ == TRAIN) {
        const unsigned int* mask = rand_vec_.data<Context>();
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
