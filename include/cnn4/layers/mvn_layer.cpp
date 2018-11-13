

#include "caffe/layers/mvn_layer.hpp"


namespace
{

  template <typename Dtype>
  void MVNLayer::Reshape(const vector<Blob*> & bottom,
                                const vector<Blob*> & top)
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
    Dtype* multiplier_data = sum_multiplier_.mdata();
    caffe_set(sum_multiplier_.count(), Dtype(1), multiplier_data);
    eps_ = this->param_->mvn_param().eps();
  }

  template <typename Dtype>
  void MVNLayer::Forward(CPUContext* context, const vector<Blob*> & bottom,
                                    const vector<Blob*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* top_data = top[0]->mdata();
    int num;
    if (this->param_->mvn_param().across_channels()) {
      num = bottom[0]->num();
    } else {
      num = bottom[0]->num() * bottom[0]->channels();
    }
    int dim = bottom[0]->count() / num;
    // subtract mean
    caffe_gemv<Dtype>(CblasNoTrans, num, dim, 1. / dim, bottom_data,
                          sum_multiplier_.data(), 0., mean_.mdata());  // EX
    caffe_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, dim, 1, -1.,
                          mean_.data(), sum_multiplier_.data(), 0.,
                          temp_.mdata());
    caffe_add(temp_.count(), bottom_data, temp_.data(), top_data);  // X-EX
    if (this->param_->mvn_param().normalize_variance()) {
      // compute variance using var(X) = E((X-EX)^2)
      caffe_powx(bottom[0]->count(), top_data, Dtype(2),
                 temp_.mdata());  // (X-EX)^2
      caffe_gemv<Dtype>(CblasNoTrans, num, dim, 1. / dim, temp_.data(),
                            sum_multiplier_.data(), 0.,
                            variance_.mdata());  // E((X-EX)^2)
      // normalize variance
      caffe_powx(variance_.count(), variance_.data(), Dtype(0.5),
                 variance_.mdata());
      caffe_add_scalar(variance_.count(), eps_, variance_.mdata());
      caffe_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, dim, 1, 1.,
                            variance_.data(), sum_multiplier_.data(), 0.,
                            temp_.mdata());
      caffe_div(temp_.count(), top_data, temp_.data(), top_data);
    }
  }

  template <typename Dtype>
  void MVNLayer::Backward(CPUContext* context, const vector<Blob*> & top,
                                     int*
                                     const vector<Blob*> & bottom)
  {
    const Dtype* top_diff = top[0]->diff();
    const Dtype* top_data = top[0]->data();
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* bottom_diff = bottom[0]->mdiff();
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
                            sum_multiplier_.data(), 0., mean_.mdata());
      caffe_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, dim, 1, 1.,
                            mean_.data(), sum_multiplier_.data(), 0.,
                            bottom_diff);
      caffe_mul(temp_.count(), top_data, bottom_diff, bottom_diff);
      caffe_gemv<Dtype>(CblasNoTrans, num, dim, 1., top_diff,
                            sum_multiplier_.data(), 0., mean_.mdata());
      caffe_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, dim, 1, 1.,
                            mean_.data(), sum_multiplier_.data(), 1.,
                            bottom_diff);
      caffe_axpby(temp_.count(), Dtype(1), top_diff, Dtype(-1. / dim),
                      bottom_diff);
      // put the squares of bottom into temp_
      caffe_powx(temp_.count(), bottom_data, Dtype(2),
                 temp_.mdata());
      caffe_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, dim, 1, 1.,
                            variance_.data(), sum_multiplier_.data(), 0.,
                            temp_.mdata());
      caffe_div(temp_.count(), bottom_diff, temp_.data(), bottom_diff);
    } else {
      caffe_gemv<Dtype>(CblasNoTrans, num, dim, 1. / dim, top_diff,
                            sum_multiplier_.data(), 0., mean_.mdata());
      caffe_gemm<Dtype>(CblasNoTrans, CblasNoTrans, num, dim, 1, -1.,
                            mean_.data(), sum_multiplier_.data(), 0.,
                            temp_.mdata());
      caffe_add(temp_.count(), top_diff, temp_.data(), bottom_diff);
    }
  }


#ifdef CPU_ONLY
  STUB_GPU(MVNLayer);
#endif

  INSTANTIATE_CLASS(MVNLayer);
  REGISTER_LAYER_CLASS(MVN);

}  // namespace
