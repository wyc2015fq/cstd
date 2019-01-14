#ifndef CAFFE_POOLING_LAYER_HPP_
#define CAFFE_POOLING_LAYER_HPP_

/**
 * @brief Pools the input image by taking the max, average, etc. within regions.
 *
 * TODO(dox): thorough documentation for Forward_, Backward_, and proto params.
 */


template <typename T> T* clone(const T* p) {
  T* out = new T;
  *out = *p;
  return out;
}

int cjson_GetShape2D1(cjson* param, const char* name, const char* name_h, const char* name_w, int defint, int& pad_h_, int& pad_w_) {
  cjson* item = cjson_GetObjectItem(param, name);
  cjson* item_h = cjson_GetObjectItem(param, name_h);
  cjson* item_w = cjson_GetObjectItem(param, name_w);
  if (item_h || item_w) {
    pad_h_ = item_h ? item_h->valueint : defint;
    pad_w_ = item_h ? item_w->valueint : defint;
  }
  else {
    pad_h_ = pad_w_ = item ? item->valueint : defint;
  }
  return 0;
}

int cjson_GetShape2D(cjson* item, cjson* item_h, cjson* item_w, int defint, int& pad_h_, int& pad_w_) {
  if (item_h || item_w) {
    pad_h_ = item_h ? item_h->valueint : defint;
    pad_w_ = item_h ? item_w->valueint : defint;
  }
  else {
    pad_h_ = pad_w_ = item ? item->valueint : defint;
  }
  return 0;
}

#define PoolingParameter_DEF(DEF) \
DEF##Enum(pool, PoolMethod_MAX, PoolMethod) \
DEF##Int(pad, 0, 0) \
DEF##Int(pad_h, 0, 0) \
DEF##Int(pad_w, 0, 0) \
DEF##Int(kernel_size, 0, 0) \
DEF##Int(kernel_h, 2, 0) \
DEF##Int(kernel_w, 2, 0) \
DEF##Int(stride, 0, 0) \
DEF##Int(stride_h, 1, 0) \
DEF##Int(stride_w, 1, 0) \
DEF##Bool(global_pooling, false, 0) \

#if 0
struct pooling_layer_t {
  PoolingParameter_DEF(Def);
  DataShape out_shape;
  int channels_;
  int height_, width_;
  int pooled_height_, pooled_width_;
  void setup(DataShape in_shape) {
    pooling_layer_t* s = this;
    CHECK_EQ(4, in_shape.num_axes()) << "Input must have 4 axes, "
      << "corresponding to (num, channels, height, width)";
    s->channels_ = in_shape.channels();
    s->height_ = in_shape.height();
    s->width_ = in_shape.width();
    if (s->global_pooling_) {
      s->kernel_h_ = in_shape.height();
      s->kernel_w_ = in_shape.width();
    }
    s->pooled_height_ = static_cast<int>(ceil(static_cast<float>(s->height_ + 2 * s->pad_h_ - s->kernel_h_) / s->stride_h_)) + 1;
    s->pooled_width_ = static_cast<int>(ceil(static_cast<float>(s->width_ + 2 * s->pad_w_ - s->kernel_w_) / s->stride_w_)) + 1;
    if (s->pad_h_ || s->pad_w_) {
      // If we have padding, ensure that the last pooling starts strictly
      // inside the image (instead of at the padding); otherwise clip the last.
      if ((s->pooled_height_ - 1) * s->stride_h_ >= s->height_ + s->pad_h_) {
        --s->pooled_height_;
      }
      if ((s->pooled_width_ - 1) * s->stride_w_ >= s->width_ + pad_w_) {
        --s->pooled_width_;
      }
      CHECK_LT((pooled_height_ - 1) * stride_h_, height_ + pad_h_);
      CHECK_LT((pooled_width_ - 1) * stride_w_, width_ + pad_w_);
    }
    out_shape = dataShape(in_shape.num(), channels_, pooled_height_, pooled_width_);
    return;
  }
};
#endif


struct PoolingLayer : public Layer
{
  PoolingParameter_DEF(Def);
  DataShape out_shape;
  int channels_;
  int height_, width_;
  int pooled_height_, pooled_width_;
  Blob rand_idx_;
  Blob max_idx_;
  virtual inline const char* type() const { return "Pooling"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int MinTopBlobs() const { return 1; }
  // MAX POOL layers can output an extra top blob for the mask;
  // others can only output the pooled inputs.
  virtual inline int MaxTopBlobs() const {
    return (pool_ == PoolMethod_MAX) ? 2 : 1;
  }
  PoolingLayer() {
    init();
  }
  virtual void init() {
    PoolingParameter_DEF(Init);
  }
  virtual void toJson(cjson* param) {
    PoolingParameter_DEF(Set);
  }
  void fromJson(cjson* param) {
    PoolingParameter_DEF(Get);
    cjson* kernel_size_json = cjson_GetObjectItem(param, "kernel_size");
    cjson* kernel_h_json = cjson_GetObjectItem(param, "kernel_h");
    cjson* kernel_w_json = cjson_GetObjectItem(param, "kernel_w");
    cjson* pad_json = cjson_GetObjectItem(param, "pad");
    cjson* pad_h_json = cjson_GetObjectItem(param, "pad_h");
    cjson* pad_w_json = cjson_GetObjectItem(param, "pad_w");
    cjson* stride_json = cjson_GetObjectItem(param, "stride");
    cjson* stride_h_json = cjson_GetObjectItem(param, "stride_h");
    cjson* stride_w_json = cjson_GetObjectItem(param, "stride_w");
    if (global_pooling_) {
      CHECK(!(kernel_size_json || kernel_h_json || kernel_w_json))
        << "With Global_pooling: true Filter size cannot specified";
    }
    else {
      CHECK(!kernel_size_json != !(kernel_h_json && kernel_w_json))
        << "Filter size is kernel_size OR kernel_h and kernel_w; not both";
      CHECK(kernel_size_json || (kernel_h_json && kernel_w_json))
        << "For non-square filters both kernel_h and kernel_w are required.";
    }
    CHECK((!pad_json && pad_h_json && pad_w_json) || (!pad_h_json && !pad_w_json)) << "pad is pad OR pad_h and pad_w are required.";
    //CHECK((!stride_json && stride_h_json && stride_w_json) || (!stride_h_json && !stride_w_json)) << "Stride is stride OR stride_h and stride_w are required.";
    if (global_pooling_) {
    }
    else {
      cjson_GetShape2D(kernel_size_json, kernel_h_json, kernel_w_json, 0, kernel_h_, kernel_w_);
    }
    cjson_GetShape2D(pad_json, pad_h_json, pad_w_json, 0, pad_h_, pad_w_);
    cjson_GetShape2D(stride_json, stride_h_json, stride_w_json, 0, stride_h_, stride_w_);
  }
  
  virtual void LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    if (kernel_size_) {
      kernel_h_ = kernel_w_ = kernel_size_;
    }
    if (pad_) {
      pad_h_ = pad_w_ = pad_;
    }
    if (stride_) {
      stride_h_ = stride_w_ = stride_;
    }
    if (global_pooling_) {
      kernel_h_ = bottom[0]->height();
      kernel_w_ = bottom[0]->width();
    }
    CHECK_GT(kernel_h_, 0) << "Filter dimensions cannot be zero.";
    CHECK_GT(kernel_w_, 0) << "Filter dimensions cannot be zero.";
    if (global_pooling_) {
      CHECK(pad_h_ == 0 && pad_w_ == 0 && stride_h_ == 1 && stride_w_ == 1)
        << "With Global_pooling: true; only pad = 0 and stride = 1";
    }
    if (pad_h_ != 0 || pad_w_ != 0) {
      CHECK(this->pool_ == PoolMethod_AVE || this->pool_ == PoolMethod_MAX)
        << "Padding implemented only for average and max pooling.";
      CHECK_LT(pad_h_, kernel_h_);
      CHECK_LT(pad_w_, kernel_w_);
    }
  }

  virtual void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    CHECK_EQ(4, bottom[0]->num_axes()) << "Input must have 4 axes, "
      << "corresponding to (num, channels, height, width)";
    channels_ = bottom[0]->channels();
    height_ = bottom[0]->height();
    width_ = bottom[0]->width();
    if (global_pooling_) {
      kernel_h_ = bottom[0]->height();
      kernel_w_ = bottom[0]->width();
    }
    pooled_height_ = static_cast<int>(ceil(static_cast<float>(height_ + 2 * pad_h_ - kernel_h_) / stride_h_)) + 1;
    pooled_width_ = static_cast<int>(ceil(static_cast<float>(width_ + 2 * pad_w_ - kernel_w_) / stride_w_)) + 1;
    if (pad_h_ || pad_w_) {
      // If we have padding, ensure that the last pooling starts strictly
      // inside the image (instead of at the padding); otherwise clip the last.
      if ((pooled_height_ - 1) * stride_h_ >= height_ + pad_h_) {
        --pooled_height_;
      }
      if ((pooled_width_ - 1) * stride_w_ >= width_ + pad_w_) {
        --pooled_width_;
      }
      CHECK_LT((pooled_height_ - 1) * stride_h_, height_ + pad_h_);
      CHECK_LT((pooled_width_ - 1) * stride_w_, width_ + pad_w_);
    }
    top[0]->Reshape(bottom[0]->num(), channels_, pooled_height_,
      pooled_width_);
    if (top.size() > 1) {
      top[1]->ReshapeLike(*top[0]);
    }
    // If max pooling, we will initialize the vector index part.
    if (this->pool_ == PoolMethod_MAX && top.size() == 1) {
      max_idx_.Reshape(bottom[0]->num(), channels_, pooled_height_,
        pooled_width_);
    }
    // If stochastic pooling, we will initialize the random index part.
    if (this->pool_ == PoolMethod_STOCHASTIC) {
      rand_idx_.Reshape(bottom[0]->num(), channels_, pooled_height_, pooled_width_);
    }
  }

  // TODO(Yangqing): Is there a faster way to do pooling in the channel-first
  // case?
  
  virtual void Forward_(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    int top_count = top[0]->count();
    int num = top[0]->num();
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* top_data = top[0]->mdata();
    //const int top_count = top[0]->count();
    // We'll output the mask to top[1] if it's of size >1.
    const bool use_top_mask = top.size() > 1;
    int* mask = NULL;  // suppress warnings about uninitalized variables
    Dtype* top_mask = NULL;
    Dtype* rand_idx = NULL;
    // Different pooling methods. We explicitly do the switch outside the for
    // loop to save time, although this results in more code.
      // Initialize
    if (use_top_mask) {
      top_mask = top[1]->mdata();
    }
    else {
      mask = (int*)max_idx_.mdata();
    }
    pooling_forward(pool_, phase_cur_, bottom_data,
      num, channels_, height_, width_, pooled_height_, pooled_width_,
      kernel_h_, kernel_w_, stride_h_, stride_w_, pad_h_, pad_w_,
      rand_idx, top_data, mask, top_mask);
    return;
  }

  
  virtual void Backward_(const vector<Blob*> & top,
    const vector<Blob*> & bottom)
  {
    //int count = bottom[0]->count();
    int num = top[0]->num();
    if (!bottom[0]->propagate_down_) {
      return;
    }
    const Dtype* top_diff = top[0]->diff();
    Dtype* bottom_diff = bottom[0]->mdiff();
    // Different pooling methods. We explicitly do the switch outside the for
    // loop to save time, although this results in more codes.
    //caffe_set(bottom[0]->count(), Dtype(0), bottom_diff);
    // We'll output the mask to top[1] if it's of size >1.
    const bool use_top_mask = top.size() > 1;
    const int* mask = NULL;  // suppress warnings about uninitialized variables
    const Dtype* top_mask = NULL;
    // The main loop
    if (use_top_mask) {
      top_mask = top[1]->data();
    }
    else {
      mask = (int*)max_idx_.data();
    }
    pooling_backward(pool_, NULL, top_diff, mask, top_mask, num, channels_, height_, width_,
      pooled_height_, pooled_width_, kernel_h_, kernel_w_, stride_h_, stride_w_, pad_h_, pad_w_, bottom_diff);
  }

};

INSTANTIATE_CLASS(Pooling, PoolingLayer);

#endif  // CAFFE_POOLING_LAYER_HPP_
