#ifndef CAFFE_BATCHNORM_LAYER_HPP_
#define CAFFE_BATCHNORM_LAYER_HPP_


#define BatchNormParameter_DEF(DEF) \
DEF##Float(moving_average_fraction, 0.999, 0) \
DEF##Float(eps, 1e-5, 0) \
DEF##Bool(use_global_stats, false, 0) \
DEF##Bool(update_global_stats, false, 0) \

class BatchNormLayer : public Layer
{
public:
  BatchNormParameter_DEF(Def);

  Blob mean_, variance_, temp_, x_norm_;
  int channels_;

  // extra temporarary variables is used to carry out sums/broadcasting
  // using BLAS
  Blob batch_sum_multiplier_;
  Blob num_by_chans_;
  Blob spatial_sum_multiplier_;

  virtual inline const char* type() const { return "BatchNorm"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

  BatchNormLayer() {
    BatchNormParameter_DEF(Set);
  }
  void init(cJSON* param) {
    BatchNormParameter_DEF(Get);
    use_global_stats_ = this->phase_ == TEST;
    if (param->has("use_global_stats")) {
      use_global_stats_ = param->getbool("use_global_stats", false);
    }
  }

  void BatchNormLayer::LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    if (bottom[0]->num_axes() == 1) {
      channels_ = 1;
    }
    else {
      channels_ = bottom[0]->shape(1);
    }
    if (this->blobs_.size() > 0) {
      LOG(INFO) << "Skipping parameter initialization";
    }
    else {
      blobs_reset(this->blobs_, 3);
      vector<int> sz;
      sz.push_back(channels_);
      this->blobs_[0]->Reshape(sz);
      this->blobs_[1]->Reshape(sz);
      sz[0] = 1;
      this->blobs_[2]->Reshape(sz);
      for (int i = 0; i < 3; ++i) {
        caffe_set(this->blobs_[i]->count(), Dtype(0),
          this->blobs_[i]->mdata());
      }
    }
  }


  void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    if (bottom[0]->num_axes() >= 1) {
      CHECK_EQ(bottom[0]->shape(1), channels_);
    }
    top[0]->ReshapeLike(*bottom[0]);
    vector<int> sz;
    sz.push_back(channels_);
    mean_.Reshape(sz);
    variance_.Reshape(sz);
    temp_.ReshapeLike(*bottom[0]);
    x_norm_.ReshapeLike(*bottom[0]);
    sz[0] = bottom[0]->shape(0);
    batch_sum_multiplier_.Reshape(sz);
    int spatial_dim = bottom[0]->count() / (channels_ * bottom[0]->shape(0));
    if (spatial_sum_multiplier_.num_axes() == 0 || spatial_sum_multiplier_.shape(0) != spatial_dim) {
      sz[0] = spatial_dim;
      spatial_sum_multiplier_.Reshape(sz);
    }
    caffe_set(spatial_sum_multiplier_.count(), Dtype(1), spatial_sum_multiplier_.mdata());
    int numbychans = channels_ * bottom[0]->shape(0);
    if (num_by_chans_.num_axes() == 0 || num_by_chans_.shape(0) != numbychans) {
      sz[0] = numbychans;
      num_by_chans_.Reshape(sz);
    }
    caffe_set(batch_sum_multiplier_.count(), Dtype(1), batch_sum_multiplier_.mdata());
  }


  void BatchNormLayer::Forward_(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data();
    const Dtype* batch_sum_multiplier_data = batch_sum_multiplier_.data();
    const Dtype* spatial_sum_multiplier_data = spatial_sum_multiplier_.data();
    Dtype* num_by_chans_data = num_by_chans_.mdata();
    Dtype* top_data = top[0]->mdata();
    int num = bottom[0]->shape(0);
    int spatial_dim = bottom[0]->count() / (bottom[0]->shape(0) * channels_);
    if (bottom[0] != top[0]) {
      caffe_copy(bottom[0]->count(), bottom_data, top_data);
    }
    if (use_global_stats_) {
      // use the stored mean/variance estimates.
      const Dtype scale_factor = this->blobs_[2]->data()[0] == 0 ? 0 : 1 / this->blobs_[2]->data()[0];
      caffe_scale(variance_.count(), scale_factor, this->blobs_[0]->data(), mean_.mdata());
      caffe_scale(variance_.count(), scale_factor, this->blobs_[1]->data(), variance_.mdata());
    }
    else {
      // compute mean
      caffe_gemv(CblasNoTrans, channels_ * num, spatial_dim, 1. / (num * spatial_dim), bottom_data, spatial_sum_multiplier_data, 0., num_by_chans_data);
      caffe_gemv(CblasTrans, num, channels_, 1., num_by_chans_data, batch_sum_multiplier_data, 0., mean_.mdata());
    }
    // subtract mean
    caffe_gemm(CblasNoTrans, CblasNoTrans, num, channels_, 1, 1, batch_sum_multiplier_data, mean_.data(), 0., num_by_chans_data);
    caffe_gemm(CblasNoTrans, CblasNoTrans, channels_ * num, spatial_dim, 1, -1, num_by_chans_data, spatial_sum_multiplier_data, 1., top_data);
    if (!use_global_stats_) {
      // compute variance using var(X) = E((X-EX)^2)
      caffe_powx(top[0]->count(), top_data, Dtype(2), temp_.mdata());  // (X-EX)^2
      caffe_gemv(CblasNoTrans, channels_ * num, spatial_dim, 1. / (num * spatial_dim), temp_.data(),
        spatial_sum_multiplier_data, 0., num_by_chans_data);
      caffe_gemv(CblasTrans, num, channels_, 1., num_by_chans_data, batch_sum_multiplier_data, 0., variance_.mdata());  // E((X_EX)^2)
                                // compute and save moving average
      this->blobs_[2]->cpu_mdata()[0] *= moving_average_fraction_;
      this->blobs_[2]->cpu_mdata()[0] += 1;
      caffe_axpby(mean_.count(), Dtype(1), mean_.data(), moving_average_fraction_, this->blobs_[0]->mdata());
      int m = bottom[0]->count() / channels_;
      Dtype bias_correction_factor = m > 1 ? Dtype(m) / (m - 1) : 1;
      caffe_axpby(variance_.count(), bias_correction_factor, variance_.data(), moving_average_fraction_, this->blobs_[1]->mdata());
    }
    // normalize variance
    caffe_add_scalar(variance_.count(), eps_, variance_.mdata());
    caffe_powx(variance_.count(), variance_.data(), Dtype(0.5), variance_.mdata());
    // replicate variance to input size
    caffe_gemm(CblasNoTrans, CblasNoTrans, num, channels_, 1, 1, batch_sum_multiplier_data, variance_.data(), 0., num_by_chans_data);
    caffe_gemm(CblasNoTrans, CblasNoTrans, channels_ * num, spatial_dim, 1, 1., num_by_chans_data, spatial_sum_multiplier_data, 0., temp_.mdata());
    caffe_div(temp_.count(), top_data, temp_.data(), top_data);
    // TODO(cdoersch): The caching is only needed because later in-place layers
    //                 might clobber the data.  Can we skip this if they won't?
    caffe_copy(x_norm_.count(), top_data, x_norm_.mdata());
  }

  virtual void Backward_(const vector<Blob*>& top, const vector<Blob*>& bottom)
  {
    const Dtype* top_diff;
    const Dtype* batch_sum_multiplier_data = batch_sum_multiplier_.data();
    const Dtype* spatial_sum_multiplier_data = spatial_sum_multiplier_.data();
    Dtype* num_by_chans_data = num_by_chans_.mdata();
    if (bottom[0] != top[0]) {
      top_diff = top[0]->diff();
    }
    else {
      caffe_copy(x_norm_.count(), top[0]->diff(), x_norm_.mdiff());
      top_diff = x_norm_.diff();
    }
    Dtype* bottom_diff = bottom[0]->mdiff();
    if (use_global_stats_) {
      caffe_div(temp_.count(), top_diff, temp_.data(), bottom_diff);
      return;
    }
    const Dtype* top_data = x_norm_.data();
    int num = bottom[0]->shape(0);
    int spatial_dim = bottom[0]->count() / (bottom[0]->shape(0) * channels_);
    // if Y = (X-mean(X))/(sqrt(var(X)+eps)), then
    //
    // dE(Y)/dX =
    //   (dE/dY - mean(dE/dY) - mean(dE/dY \cdot Y) \cdot Y)
    //     ./ sqrt(var(X) + eps)
    //
    // where \cdot and ./ are hadamard product and elementwise division,
    // respectively, dE/dY is the top diff, and mean/var/sum are all computed
    // along all dimensions except the channels dimension.  In the above
    // equation, the operations allow for expansion (i.e. broadcast) along all
    // dimensions except the channels dimension where required.
    // sum(dE/dY \cdot Y)
    caffe_mul(temp_.count(), top_data, top_diff, bottom_diff);
    caffe_gemv(CblasNoTrans, channels_ * num, spatial_dim, 1.,
      bottom_diff, spatial_sum_multiplier_data, 0.,
      num_by_chans_data);
    caffe_gemv(CblasTrans, num, channels_, 1.,
      num_by_chans_data, batch_sum_multiplier_data, 0.,
      mean_.mdata());
    // reshape (broadcast) the above
    caffe_gemm(CblasNoTrans, CblasNoTrans, num, channels_, 1, 1,
      batch_sum_multiplier_data, mean_.data(), 0.,
      num_by_chans_data);
    caffe_gemm(CblasNoTrans, CblasNoTrans, channels_ * num,
      spatial_dim, 1, 1., num_by_chans_data,
      spatial_sum_multiplier_data, 0., bottom_diff);
    // sum(dE/dY \cdot Y) \cdot Y
    caffe_mul(temp_.count(), top_data, bottom_diff, bottom_diff);
    // sum(dE/dY)-sum(dE/dY \cdot Y) \cdot Y
    caffe_gemv(CblasNoTrans, channels_ * num, spatial_dim, 1.,
      top_diff, spatial_sum_multiplier_data, 0.,
      num_by_chans_data);
    caffe_gemv(CblasTrans, num, channels_, 1.,
      num_by_chans_data, batch_sum_multiplier_data, 0.,
      mean_.mdata());
    // reshape (broadcast) the above to make
    // sum(dE/dY)-sum(dE/dY \cdot Y) \cdot Y
    caffe_gemm(CblasNoTrans, CblasNoTrans, num, channels_, 1, 1,
      batch_sum_multiplier_data, mean_.data(), 0.,
      num_by_chans_data);
    caffe_gemm(CblasNoTrans, CblasNoTrans, num * channels_,
      spatial_dim, 1, 1., num_by_chans_data,
      spatial_sum_multiplier_data, 1., bottom_diff);
    // dE/dY - mean(dE/dY)-mean(dE/dY \cdot Y) \cdot Y
    caffe_axpby(temp_.count(), Dtype(1), top_diff,
      Dtype(-1. / (num * spatial_dim)), bottom_diff);
    // note: temp_ still contains sqrt(var(X)+eps), computed during the forward
    // pass.
    caffe_div(temp_.count(), bottom_diff, temp_.data(), bottom_diff);
  }


};

INSTANTIATE_CLASS(BatchNorm);


#endif  // CAFFE_BATCHNORM_LAYER_HPP_
