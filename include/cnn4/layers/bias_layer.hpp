#ifndef CAFFE_BIAS_LAYER_HPP_
#define CAFFE_BIAS_LAYER_HPP_


/**
 * @brief Computes a sum of two input Blobs, with the shape of the
 *        latter Blob "broadcast" to match the shape of the former.
 *        Equivalent to tiling the latter Blob, then computing the elementwise
 *        sum.
 *
 * The second input may be omitted, in which case it's learned as a parameter
 * of the layer.
 */
template <typename Dtype>
struct BiasLayer : public Layer<Dtype>
{
public:
  virtual inline const char* type() const { return "Bias"; }
  virtual inline int MinBottomBlobs() const { return 1; }
  virtual inline int MaxBottomBlobs() const { return 2; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

  Blob<Dtype> bias_multiplier_;
  int outer_dim_, bias_dim_, inner_dim_, dim_;
  int axis_;
  int num_axes_;

  virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom,
    const vector<Blob<Dtype>*> & top)
  {
    axis_ = param_->GetObjectInt("axis", 0);
    num_axes_ = param_->GetObjectInt("num_axes", -1);

    if (bottom.size() == 1 && this->blobs_.size() > 0) {
      LOG(INFO) << "Skipping parameter initialization";
    }
    else if (bottom.size() == 1) {
      // bias is a learned parameter; initialize it
      const int axis = bottom[0]->CanonicalAxisIndex(axis_);
      const int num_axes = num_axes_;
      CHECK_GE(num_axes, -1) << "num_axes must be non-negative, "
        << "or -1 to extend to the end of bottom[0]";
      if (num_axes >= 0) {
        CHECK_GE(bottom[0]->num_axes(), axis + num_axes)
          << "bias blob's shape extends past bottom[0]'s shape when applied "
          << "starting with bottom[0] axis = " << axis;
      }
      this->blobs_.resize(1);
      const int* shape_start = bottom[0]->shape().dim + axis;
      const int* shape_end = (num_axes == -1) ? bottom[0]->shape().end() : (shape_start + num_axes);
      vector<int> bias_shape(shape_start, shape_end);
      this->blobs_[0]->Reshape(bias_shape);
      Filler<Dtype>(this->blobs_[0], param_);
    }
    //this->param_propagate_down_.resize(this->blobs_.size(), true);
  }

  virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
    const vector<Blob<Dtype>*> & top)
  {
    Blob<Dtype>* bias = (bottom.size() > 1) ? bottom[1] : this->blobs_[0];
    // Always set axis == 0 in special case where bias is a scalar
    // (num_axes == 0). Mathematically equivalent for any choice of axis, so the
    // actual setting can be safely ignored; and computation is most efficient
    // with axis == 0 and (therefore) outer_dim_ == 1.
    const int axis = bottom[0]->CanonicalAxisIndex(axis_);
    CHECK_GE(bottom[0]->num_axes(), axis + bias->num_axes())
      << "bias blob's shape extends past bottom[0]'s shape when applied "
      << "starting with bottom[0] axis = " << axis;
    for (int i = 0; i < bias->num_axes(); ++i) {
      CHECK_EQ(bottom[0]->shape(axis + i), bias->shape(i))
        << "dimension mismatch between bottom[0]->shape(" << axis + i
        << ") and bias->shape(" << i << ")";
    }
    outer_dim_ = bottom[0]->count(0, axis);
    bias_dim_ = bias->count();
    inner_dim_ = bottom[0]->count(axis + bias->num_axes());
    dim_ = bias_dim_ * inner_dim_;
    if (bottom[0] != top[0]) {
      top[0]->ReshapeLike(*bottom[0]);
    }
    bias_multiplier_.Reshape(vector<int>(1, inner_dim_));
    if (bias_multiplier_.data<Context>()[inner_dim_ - 1] != Dtype(1)) {
      caffe_set(inner_dim_, Dtype(1), bias_multiplier_.mutable_data<Context>());
    }
  }

  void Forward(Context* context, const vector<Blob<Dtype>*> & bottom,
    const vector<Blob<Dtype>*> & top)
  {
    const Dtype* bias_data =
      ((bottom.size() > 1) ? bottom[1] : this->blobs_[0])->data<Context>();
    Dtype* top_data = top[0]->mutable_data<Context>();
    if (bottom[0] != top[0]) {
      const Dtype* bottom_data = bottom[0]->data<Context>();
      caffe_copy(context,bottom[0]->count(), bottom_data, top_data);
    }
    for (int n = 0; n < outer_dim_; ++n) {
      caffe_gemm(context, CblasNoTrans, CblasNoTrans, bias_dim_,
        inner_dim_, 1, Dtype(1), bias_data,
        bias_multiplier_.data<Context>(), Dtype(1), top_data);
      top_data += dim_;
    }
  }

  void Backward(Context* context, const vector<Blob<Dtype>*> & top,
    const vector<Blob<Dtype>*> & bottom)
  {
    if (top[0]->propagate_down_ && bottom[0] != top[0]) {
      const Dtype* top_diff = top[0]->diff<Context>();
      Dtype* bottom_diff = bottom[0]->mutable_diff<Context>();
      caffe_copy(context,bottom[0]->count(), top_diff, bottom_diff);
    }
    // in-place, we don't need to do anything with the data diff
    const bool bias_param = (bottom.size() == 1);
      if ((!bias_param && top[1]->propagate_down_) ||
        (bias_param && this->blobs_[0]->propagate_down_)) {
      const Dtype* top_diff = top[0]->diff<Context>();
      Dtype* bias_diff = (bias_param ? this->blobs_[0] : bottom[1])
        ->mutable_diff<Context>();
      bool accum = bias_param;
      for (int n = 0; n < outer_dim_; ++n) {
        caffe_gemv(context, CblasNoTrans, bias_dim_, inner_dim_, Dtype(1),
          top_diff, bias_multiplier_.data<Context>(), Dtype(accum), bias_diff);
        top_diff += dim_;
        accum = true;
      }
    }
  }
};


INSTANTIATE_CLASS(Bias);


#endif  // CAFFE_BIAS_LAYER_HPP_
