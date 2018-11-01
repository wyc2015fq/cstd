

#include "caffe/layers/mvn_layer.hpp"


namespace
{

  template <typename Dtype>
  void MVNLayer<Dtype>::Reshape(const vector<Blob<Dtype>*> & bottom,
                                const vector<Blob<Dtype>*> & top)
  {
    top[0]->Reshape(bottom[0]->num(), bottom[0]->channels(),
                    bottom[0]->height(), bottom[0]->width());
    mean_.Reshape(bottom[0]->num(), bottom[0]->channels(),
                  1, 1);
    variance_.Reshape(bottom[0]->num(), bottom[0]->channels(),
                      1, 1);
    temp_.Reshape(bottom[0]->num(), bottom[0]->channels(),
                  bottom[0]->height(), bottom[0]->width());
    if ( this->param_->mvn_param().across_channels() ) {
      sum_multiplier_.Reshape(1, bottom[0]->channels(), bottom[0]->height(),
                              bottom[0]->width());
    } else {
      sum_multiplier_.Reshape(1, 1, bottom[0]->height(), bottom[0]->width());
    }
    Dtype* multiplier_data = sum_multiplier_.mutable_data<Context>();
    caffe_set(sum_multiplier_.count(), Dtype(1), multiplier_data);
    eps_ = this->param_->mvn_param().eps();
  }

  template <typename Dtype>
  void MVNLayer<Dtype>::Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
                                    const vector<Blob<Dtype>*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data<Context>();
    Dtype* top_data = top[0]->mutable_data<Context>();
    int num;
    if (this->param_->mvn_param().across_channels()) {
      num = bottom[0]->num();
    } else {
      num = bottom[0]->num() * bottom[0]->channels();
    }
    int dim = bottom[0]->count() / num;
    // subtract mean
    caffe_gemv<Dtype>(CblasNoTrans, num, dim, 1. / dim, bottom_data,
                          sum_multiplier_.data<Context>(), 0., mean_.mutable_data<Context>());  // EX
    caffe_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, dim, 1, -1.,
                          mean_.data<Context>(), sum_multiplier_.data<Context>(), 0.,
                          temp_.mutable_data<Context>());
    caffe_add(temp_.count(), bottom_data, temp_.data<Context>(), top_data);  // X-EX
    if (this->param_->mvn_param().normalize_variance()) {
      // compute variance using var(X) = E((X-EX)^2)
      caffe_powx(bottom[0]->count(), top_data, Dtype(2),
                 temp_.mutable_data<Context>());  // (X-EX)^2
      caffe_gemv<Dtype>(CblasNoTrans, num, dim, 1. / dim, temp_.data<Context>(),
                            sum_multiplier_.data<Context>(), 0.,
                            variance_.mutable_data<Context>());  // E((X-EX)^2)
      // normalize variance
      caffe_powx(variance_.count(), variance_.data<Context>(), Dtype(0.5),
                 variance_.mutable_data<Context>());
      caffe_add_scalar(variance_.count(), eps_, variance_.mutable_data<Context>());
      caffe_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, dim, 1, 1.,
                            variance_.data<Context>(), sum_multiplier_.data<Context>(), 0.,
                            temp_.mutable_data<Context>());
      caffe_div(temp_.count(), top_data, temp_.data<Context>(), top_data);
    }
  }

  template <typename Dtype>
  void MVNLayer<Dtype>::Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
                                     int*
                                     const vector<Blob<Dtype>*> & bottom)
  {
    const Dtype* top_diff = top[0]->diff<Context>();
    const Dtype* top_data = top[0]->data<Context>();
    const Dtype* bottom_data = bottom[0]->data<Context>();
    Dtype* bottom_diff = bottom[0]->mutable_diff<Context>();
    int num;
    if (this->param_->mvn_param().across_channels()) {
      num = bottom[0]->num();
    } else {
      num = bottom[0]->num() * bottom[0]->channels();
    }
    int dim = bottom[0]->count() / num;
    if (this->param_->mvn_param().normalize_variance()) {
      caffe_mul(temp_.count(), top_data, top_diff, bottom_diff);
      caffe_gemv<Dtype>(CblasNoTrans, num, dim, 1., bottom_diff,
                            sum_multiplier_.data<Context>(), 0., mean_.mutable_data<Context>());
      caffe_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, dim, 1, 1.,
                            mean_.data<Context>(), sum_multiplier_.data<Context>(), 0.,
                            bottom_diff);
      caffe_mul(temp_.count(), top_data, bottom_diff, bottom_diff);
      caffe_gemv<Dtype>(CblasNoTrans, num, dim, 1., top_diff,
                            sum_multiplier_.data<Context>(), 0., mean_.mutable_data<Context>());
      caffe_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, dim, 1, 1.,
                            mean_.data<Context>(), sum_multiplier_.data<Context>(), 1.,
                            bottom_diff);
      caffe_axpby(temp_.count(), Dtype(1), top_diff, Dtype(-1. / dim),
                      bottom_diff);
      // put the squares of bottom into temp_
      caffe_powx(temp_.count(), bottom_data, Dtype(2),
                 temp_.mutable_data<Context>());
      caffe_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, dim, 1, 1.,
                            variance_.data<Context>(), sum_multiplier_.data<Context>(), 0.,
                            temp_.mutable_data<Context>());
      caffe_div(temp_.count(), bottom_diff, temp_.data<Context>(), bottom_diff);
    } else {
      caffe_gemv<Dtype>(CblasNoTrans, num, dim, 1. / dim, top_diff,
                            sum_multiplier_.data<Context>(), 0., mean_.mutable_data<Context>());
      caffe_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, dim, 1, -1.,
                            mean_.data<Context>(), sum_multiplier_.data<Context>(), 0.,
                            temp_.mutable_data<Context>());
      caffe_add(temp_.count(), top_diff, temp_.data<Context>(), bottom_diff);
    }
  }


#ifdef CPU_ONLY
  STUB_GPU(MVNLayer);
#endif

  INSTANTIATE_CLASS(MVNLayer);
  REGISTER_LAYER_CLASS(MVN);

}  // namespace
