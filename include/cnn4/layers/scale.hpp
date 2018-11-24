#ifndef CAFFE_SCALE_LAYER_HPP_
#define CAFFE_SCALE_LAYER_HPP_

#define ScaleParameter_DEF(DEF) \
DEF##Int(axis, 1, 0) \
DEF##Int(num_axes, 1, 0) \
DEF##Bool(bias_term, false, 0) \
DEF##Struct(filler, 0, Filler) \
DEF##Struct(bias_filler, 0, Filler) \

// top=alpha∗bottom+beta
struct ScaleLayer : public Layer
{
  BiasLayer* bias_layer_;
  //vector<Blob*> bias_bottom_vec_;
  //vector<bool> bias_propagate_down_;
  ScaleParameter_DEF(Def);
  int bias_param_id_;

  Blob sum_multiplier_;
  Blob sum_result_;
  Blob temp_;
  int outer_dim_, scale_dim_, inner_dim_;
public:
  virtual inline const char* type() const { return "Scale"; }
  // Scale
  virtual inline int MinBottomBlobs() const { return 1; }
  virtual inline int MaxBottomBlobs() const { return 2; }
  virtual inline int ExactNumTopBlobs() const { return 1; }
  
  ScaleLayer() {
    ScaleParameter_DEF(Set);
    bias_layer_ = NULL;
  }
  ~ScaleLayer() {
    if (bias_layer_) {
      delete bias_layer_;
    }
  }
  void init(cJSON* param) {
    ScaleParameter_DEF(Get);
    if (!param->has("filler")) {
      // Default to unit (1) filler_ for identity operation.
      //filler_param->set_type("constant");
      //filler_param->set_value(1);
      filler_.type_ = FillerMethod_constant;
      filler_.value_ = 1;
    }
  }

  virtual void LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    axis_ = bottom[0]->CanonicalAxisIndex(axis_);
    if (bottom.size() == 1 && this->blobs_.size() > 0) {
      LOG(INFO) << "Skipping parameter initialization";
    }
    else if (bottom.size() == 1) {
      // scale is a learned parameter; initialize it
      CHECK_GE(num_axes_, -1) << "num_axes_ must be non-negative, "
        << "or -1 to extend to the end of bottom[0]";
      if (num_axes_ >= 0) {
        CHECK_GE(bottom[0]->num_axes(), axis_ + num_axes_)
          << "scale blob's shape extends past bottom[0]'s shape when applied "
          << "starting with bottom[0] axis = " << axis_;
      }
      blobs_reset(this->blobs_, 1);
      const int* shape_start = bottom[0]->shape().begin() + axis_;
      const int* shape_end = (num_axes_ == -1) ? bottom[0]->shape().end() : (shape_start + num_axes_);
      vector<int> scale_shape(shape_start, shape_end);
      this->blobs_[0]->Reshape((scale_shape));
      Fill(this->blobs_[0], &filler_);
    }
    if (bias_term_) {
      bias_layer_ = new BiasLayer();
      bias_layer_->axis_ = axis_;
      if (bottom.size() > 1) {
        bias_layer_->num_axes_ = (bottom[1]->num_axes());
      }
      else {
        bias_layer_->num_axes_ = num_axes_;
      }
      bias_layer_->filler_ = bias_filler_;
      bias_param_id_ = this->blobs_.size();
      blobs_reset(this->blobs_, bias_param_id_ + 1);
      bias_layer_->bottom_vecs_.resize(2);
      bias_layer_->bottom_vecs_[0] = bottom[0];
      bias_layer_->bottom_vecs_[1] = this->blobs_[bias_param_id_];
      Blob* bias = this->blobs_[bias_param_id_];
      bias_layer_->LayerSetUp(bottom[0], top[0], bias);
      bias_layer_->Reshape(bottom[0], top[0], bias);
      //Fill(bias, &bias_filler_);
      //bias_propagate_down_.resize(1, false);
    }
    //this->param_propagate_down_.resize(this->blobs_.size(), true);
  }

  virtual void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    Blob* scale = (bottom.size() > 1) ? bottom[1] : this->blobs_[0];
    // Always set axis_ == 0 in special case where scale is a scalar
    // (num_axes_ == 0). Mathematically equivalent for any choice of axis_, so the
    // actual setting can be safely ignored; and computation is most efficient
    // with axis_ == 0 and (therefore) outer_dim_ == 1. (Setting axis_ to
    // bottom[0]->num_axes() - 1, giving inner_dim_ == 1, would be equally
    // performant.)
    axis_ = (scale->num_axes() == 0) ? 0 : bottom[0]->CanonicalAxisIndex(axis_);
    CHECK_GE(bottom[0]->num_axes(), axis_ + scale->num_axes())
      << "scale blob's shape extends past bottom[0]'s shape when applied "
      << "starting with bottom[0] axis = " << axis_;
    for (int i = 0; i < scale->num_axes(); ++i) {
      CHECK_EQ(bottom[0]->shape(axis_ + i), scale->shape(i))
        << "dimension mismatch between bottom[0]->shape(" << axis_ + i
        << ") and scale->shape(" << i << ")";
    }
    outer_dim_ = bottom[0]->count(0, axis_);
    scale_dim_ = scale->count();
    inner_dim_ = bottom[0]->count(axis_ + scale->num_axes());
    if (bottom[0] == top[0]) {  // in-place computation
      temp_.ReshapeLike(*bottom[0]);
    }
    else {
      top[0]->ReshapeLike(*bottom[0]);
    }
    sum_result_.Reshape(vector<int>(1, outer_dim_ * scale_dim_));
    const int sum_mult_size = std::max(outer_dim_, inner_dim_);
    sum_multiplier_.Reshape(vector<int>(1, sum_mult_size));
    if (sum_multiplier_.cpu_data()[sum_mult_size - 1] != Dtype(1)) {
      caffe_set(sum_mult_size, Dtype(1), sum_multiplier_.mdata());
    }
    if (bias_layer_) {
      bias_layer_->bottom_vecs_[0] = top[0];
      bias_layer_->Reshape(bias_layer_->bottom_vecs_, top);
    }
  }


  virtual void Forward_(const vector<Blob*>& bottom, const vector<Blob*>& top) {
    const int count = top[0]->count();
    const Dtype* bottom_data = bottom[0]->data();
    if (bottom[0] == top[0]) {
      // in-place computation; need to store bottom data before overwriting it.
      // Note that this is only necessary for Backward_; we could skip this if not
      // doing Backward_, but Caffe currently provides no way of knowing whether
      // we'll need to do Backward_ at the time of the Forward_ call.
      caffe_copy(bottom[0]->count(), bottom[0]->data(),
        temp_.mdata());
    }
    const Dtype* scale_data =
      ((bottom.size() > 1) ? bottom[1] : this->blobs_[0])->data();
    // LOG(INFO) <<  " scale value=" << scale_data[0];
    Dtype* top_data = top[0]->mdata();
    if (bias_term_) {
      const Dtype* bias_data = this->blobs_[bias_param_id_]->data();
      scalebias_forward(count, bottom_data, scale_data, bias_data, scale_dim_, inner_dim_, top_data);
    }
    else {
      scale_forward(count, bottom_data, scale_data, scale_dim_, inner_dim_, top_data);
    }
  }

  virtual void Backward_(const vector<Blob*>& top, const vector<Blob*>& bottom) {
    if (bias_term_) {
      bias_layer_->Backward(top, bias_layer_->bottom_vecs_);
    }
    const bool scale_param = (bottom.size() == 1);
    Blob* scale = scale_param ? this->blobs_[0] : bottom[1];
    if ((!scale_param && bottom[1]->propagate_down_) ||
      (scale_param && this->blobs_[0]->propagate_down_)) {
      const Dtype* top_diff = top[0]->diff();
      const bool in_place = (bottom[0] == top[0]);
      const Dtype* bottom_data = (in_place ? &temp_ : bottom[0])->data();
      // Hack: store big eltwise product in bottom[0] diff, except in the special
      // case where this layer itself does the eltwise product, in which case we
      // can store it directly in the scale diff, and we're done.
      // If we're computing in-place (and not doing eltwise computation), this
      // hack doesn't work and we store the product in temp_.
      const bool is_eltwise = (bottom[0]->count() == scale->count());
      Dtype* product = (is_eltwise ? scale->mdiff() :
        (in_place ? temp_.mdata() : bottom[0]->mdiff()));
      caffe_mul(top[0]->count(), top_diff, bottom_data, product);
      if (!is_eltwise) {
        Dtype* sum_result = NULL;
        if (inner_dim_ == 1) {
          sum_result = product;
        }
        else if (sum_result_.count() == 1) {
          const Dtype* sum_mult = sum_multiplier_.data();
          Dtype* scale_diff = scale->cpu_mdiff();
          Dtype result = caffe_dot(inner_dim_, product, sum_mult);
          if (scale_param) {
            *scale_diff += result;
          }
          else {
            *scale_diff = result;
          }
        }
        else {
          const Dtype* sum_mult = sum_multiplier_.data();
          sum_result = (outer_dim_ == 1) ? scale->mdiff() : sum_result_.mdata();
          caffe_gemv(CblasNoTrans, sum_result_.count(), inner_dim_,
            Dtype(1), product, sum_mult, Dtype(0), sum_result);
        }
        if (outer_dim_ != 1) {
          const Dtype* sum_mult = sum_multiplier_.data();
          if (scale_dim_ == 1) {
            Dtype* scale_diff = scale->cpu_mdiff();
            if (scale_param) {
              Dtype result = caffe_dot(outer_dim_, sum_mult, sum_result);
              *scale_diff += result;
            }
            else {
              *scale_diff = caffe_dot(outer_dim_, sum_mult, sum_result);
            }
          }
          else {
            Dtype* scale_diff = scale->mdiff();
            caffe_gemv(CblasTrans, outer_dim_, scale_dim_,
              Dtype(1), sum_result, sum_mult, Dtype(scale_param),
              scale_diff);
          }
        }
      }
    }
    if (bottom[0]->propagate_down_) {
      const int count = top[0]->count();
      const Dtype* top_diff = top[0]->diff();
      const Dtype* scale_data = scale->data();
      Dtype* bottom_diff = bottom[0]->mdiff();
      scale_forward(count, top_diff, scale_data, scale_dim_, inner_dim_, bottom_diff);
    }
  }
};


INSTANTIATE_CLASS(Scale, ScaleLayer);

#endif  // CAFFE_SCALE_LAYER_HPP_
