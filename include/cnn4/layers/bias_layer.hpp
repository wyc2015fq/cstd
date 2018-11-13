#ifndef CAFFE_BIAS_LAYER_HPP_
#define CAFFE_BIAS_LAYER_HPP_

#define BiasParameter_DEF(DEF) \
DEF##Int(axis, 1, 0) \
DEF##Int(num_axes, 1, 0) \
DEF##Struct(filler, 1, Filler)

struct BiasLayer : public Layer
{
public:
  virtual inline const char* type() const { return "Bias"; }
  virtual inline int MinBottomBlobs() const { return 1; }
  virtual inline int MaxBottomBlobs() const { return 2; }
  virtual inline int ExactNumTopBlobs() const { return 1; }
  BiasParameter_DEF(Def);
  Blob bias_multiplier_;
  int outer_dim_, bias_dim_, inner_dim_, dim_;

  BiasLayer() {
    BiasParameter_DEF(Set);
  }

  void init(CJSON* param) {
    BiasParameter_DEF(Get);
  }

  virtual void LayerSetUp(const vector<Blob*> & bottom,
    const vector<Blob*> & top)
  {
    if (bottom.size() == 1 && this->blobs_.size() > 0) {
      LOG(INFO) << "Skipping parameter initialization";
    }
    else if (bottom.size() == 1) {
      // bias is a learned parameter; initialize it
      axis_ = bottom[0]->CanonicalAxisIndex(axis_);
      CHECK_GE(num_axes_, -1) << "num_axes_ must be non-negative, "
        << "or -1 to extend to the end of bottom[0]";
      if (num_axes_ >= 0) {
        CHECK_GE(bottom[0]->num_axes(), axis_ + num_axes_)
          << "bias blob's shape extends past bottom[0]'s shape when applied "
          << "starting with bottom[0] axis_ = " << axis_;
      }
      blobs_reset(this->blobs_, 1);
      const int* shape_start = bottom[0]->shape().dim + axis_;
      const int* shape_end = (num_axes_ == -1) ? bottom[0]->shape().end() : (shape_start + num_axes_);
      vector<int> bias_shape(shape_start, shape_end);
      this->blobs_[0]->Reshape(bias_shape);
      filler_.Fill(this->blobs_[0]);
    }
    //this->param_propagate_down_.resize(this->blobs_.size(), true);
  }

  virtual void Reshape(const vector<Blob*> & bottom,
    const vector<Blob*> & top)
  {
    Blob* bias = (bottom.size() > 1) ? bottom[1] : this->blobs_[0];
    // Always set axis_ == 0 in special case where bias is a scalar
    // (num_axes_ == 0). Mathematically equivalent for any choice of axis_, so the
    // actual setting can be safely ignored; and computation is most efficient
    // with axis_ == 0 and (therefore) outer_dim_ == 1.
    axis_ = bottom[0]->CanonicalAxisIndex(axis_);
    CHECK_GE(bottom[0]->num_axes(), axis_ + bias->num_axes())
      << "bias blob's shape extends past bottom[0]'s shape when applied "
      << "starting with bottom[0] axis_ = " << axis_;
    for (int i = 0; i < bias->num_axes(); ++i) {
      CHECK_EQ(bottom[0]->shape(axis_ + i), bias->shape(i))
        << "dimension mismatch between bottom[0]->shape(" << axis_ + i
        << ") and bias->shape(" << i << ")";
    }
    outer_dim_ = bottom[0]->count(0, axis_);
    bias_dim_ = bias->count();
    inner_dim_ = bottom[0]->count(axis_ + bias->num_axes());
    dim_ = bias_dim_ * inner_dim_;
    if (bottom[0] != top[0]) {
      top[0]->ReshapeLike(*bottom[0]);
    }
    bias_multiplier_.Reshape(vector<int>(1, inner_dim_));
    if (bias_multiplier_.cpu_data()[inner_dim_ - 1] != Dtype(1)) {
      caffe_set(inner_dim_, Dtype(1), bias_multiplier_.mdata());
    }
  }

  void Forward(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    const Dtype* bias_data =
      ((bottom.size() > 1) ? bottom[1] : this->blobs_[0])->data();
    Dtype* top_data = top[0]->mdata();
    if (bottom[0] != top[0]) {
      const Dtype* bottom_data = bottom[0]->data();
      caffe_copy(bottom[0]->count(), bottom_data, top_data);
    }
    for (int n = 0; n < outer_dim_; ++n) {
      caffe_gemm(CblasNoTrans, CblasNoTrans, bias_dim_,
        inner_dim_, 1, Dtype(1), bias_data,
        bias_multiplier_.data(), Dtype(1), top_data);
      top_data += dim_;
    }
  }

  void Backward(const vector<Blob*> & top, const vector<Blob*> & bottom)
  {
    if (bottom[0]->propagate_down_ && bottom[0] != top[0]) {
      const Dtype* top_diff = top[0]->diff();
      Dtype* bottom_diff = bottom[0]->mdiff();
      caffe_copy(bottom[0]->count(), top_diff, bottom_diff);
    }
    // in-place, we don't need to do anything with the data diff
    const bool bias_param = (bottom.size() == 1);
      if ((!bias_param && bottom[1]->propagate_down_) ||
        (bias_param && this->blobs_[0]->propagate_down_)) {
      const Dtype* top_diff = top[0]->diff();
      Dtype* bias_diff = (bias_param ? this->blobs_[0] : bottom[1])
        ->mdiff();
      bool accum = bias_param;
      for (int n = 0; n < outer_dim_; ++n) {
        caffe_gemv(CblasNoTrans, bias_dim_, inner_dim_, Dtype(1),
          top_diff, bias_multiplier_.data(), Dtype(accum), bias_diff);
        top_diff += dim_;
        accum = true;
      }
    }
  }
};


INSTANTIATE_CLASS(Bias);


#endif  // CAFFE_BIAS_LAYER_HPP_
