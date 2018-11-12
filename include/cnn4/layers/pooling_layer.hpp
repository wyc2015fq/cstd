#ifndef CAFFE_POOLING_LAYER_HPP_
#define CAFFE_POOLING_LAYER_HPP_

/**
 * @brief Pools the input image by taking the max, average, etc. within regions.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */


int cJSON_GetShape2D(cJSON* pool_param, const char* name, const char* name_h, const char* name_w, int defint, int& pad_h_, int& pad_w_) {
  cJSON* item = pool_param->get(name);
  cJSON* item_h = pool_param->get(name_h);
  cJSON* item_w = pool_param->get(name_w);
  if (item_h || item_w) {
    pad_h_ = item_h ? item_h->valueint : defint;
    pad_w_ = item_h ? item_w->valueint : defint;
  }
  else {
    pad_h_ = pad_w_ = item ? item->valueint : defint;
  }
  return 0;
}

struct PoolingLayer : public Layer
{
  int kernel_h_, kernel_w_;
  int stride_h_, stride_w_;
  int pad_h_, pad_w_;
  int channels_;
  int height_, width_;
  int pooled_height_, pooled_width_;
  bool global_pooling_;
  Blob rand_idx_;
  Blob max_idx_;
  PoolMethod pool_;
  virtual inline const char* type() const { return "Pooling"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int MinTopBlobs() const { return 1; }
  // MAX POOL layers can output an extra top blob for the mask;
  // others can only output the pooled inputs.
  virtual inline int MaxTopBlobs() const {
    return (pool_ == PoolMethod_MAX) ? 2 : 1;
  }

  
  virtual void LayerSetUp(const vector<Blob*> & bottom,
    const vector<Blob*> & top)
  {
    CJSON* pool_param = this->param_;
    pool_ = (PoolMethod)this->param_->getenum("pool", 1, PoolMethod_Name, countof(PoolMethod_Name));
    global_pooling_ = pool_param->getbool("global_pooling", false);
    if (global_pooling_) {
      CHECK(!(pool_param->has("kernel_size") ||
        pool_param->has("kernel_h") || pool_param->has("kernel_w")))
        << "With Global_pooling: true Filter size cannot specified";
    }
    else {
      CHECK(!pool_param->has("kernel_size") !=
        !(pool_param->has("kernel_h") && pool_param->has("kernel_w")))
        << "Filter size is kernel_size OR kernel_h and kernel_w; not both";
      CHECK(pool_param->has("kernel_size") ||
        (pool_param->has("kernel_h") && pool_param->has("kernel_w")))
        << "For non-square filters both kernel_h and kernel_w are required.";
    }
    CHECK((!pool_param->has("pad") && pool_param->has("pad_h")
      && pool_param->has("pad_w"))
      || (!pool_param->has("pad_h") && !pool_param->has("pad_w")))
      << "pad is pad OR pad_h and pad_w are required.";
    CHECK((!pool_param->has("stride") && pool_param->has("stride_h")
      && pool_param->has("stride_w"))
      || (!pool_param->has("stride_h") && !pool_param->has("stride_w")))
      << "Stride is stride OR stride_h and stride_w are required.";
    if (global_pooling_) {
      kernel_h_ = bottom[0]->height();
      kernel_w_ = bottom[0]->width();
    }
    else {
      cJSON_GetShape2D(pool_param, "kernel_size", "kernel_h", "kernel_w", 0, kernel_h_, kernel_w_);
    }
    CHECK_GT(kernel_h_, 0) << "Filter dimensions cannot be zero.";
    CHECK_GT(kernel_w_, 0) << "Filter dimensions cannot be zero.";
    cJSON_GetShape2D(pool_param, "pad", "pad_h", "pad_w", 0, pad_h_, pad_w_);
    cJSON_GetShape2D(pool_param, "stride", "stride_h", "stride_w", 0, stride_h_, stride_w_);


    if (global_pooling_) {
      CHECK(pad_h_ == 0 && pad_w_ == 0 && stride_h_ == 1 && stride_w_ == 1)
        << "With Global_pooling: true; only pad = 0 and stride = 1";
    }
    if (pad_h_ != 0 || pad_w_ != 0) {
      CHECK(this->pool_
        == PoolMethod_AVE
        || this->pool_
        == PoolMethod_MAX)
        << "Padding implemented only for average and max pooling.";
      CHECK_LT(pad_h_, kernel_h_);
      CHECK_LT(pad_w_, kernel_w_);
    }
  }

  
  virtual void Reshape(const vector<Blob*> & bottom,
    const vector<Blob*> & top)
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
    pooled_height_ = static_cast<int>(ceil(static_cast<float>(
      height_ + 2 * pad_h_ - kernel_h_) / stride_h_)) + 1;
    pooled_width_ = static_cast<int>(ceil(static_cast<float>(
      width_ + 2 * pad_w_ - kernel_w_) / stride_w_)) + 1;
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
      rand_idx_.Reshape(bottom[0]->num(), channels_, pooled_height_,
        pooled_width_);
    }
  }

  // TODO(Yangqing): Is there a faster way to do pooling in the channel-first
  // case?
  
  virtual void Forward(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    int top_count = top[0]->count();
    int num = top[0]->num();
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* top_data = top[0]->mutable_data();
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
      top_mask = top[1]->mutable_data();
    }
    else {
      mask = (int*)max_idx_.mutable_data();
    }
    pooling_forward(pool_, phase_, bottom_data,
      num, channels_, height_, width_, pooled_height_, pooled_width_,
      kernel_h_, kernel_w_, stride_h_, stride_w_, pad_h_, pad_w_,
      rand_idx, top_data, mask, top_mask);
    return;
  }

  
  virtual void Backward(const vector<Blob*> & top,
    const vector<Blob*> & bottom)
  {
    //int count = bottom[0]->count();
    int num = top[0]->num();
    if (!bottom[0]->propagate_down_) {
      return;
    }
    const Dtype* top_diff = top[0]->diff();
    Dtype* bottom_diff = bottom[0]->mutable_diff();
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

INSTANTIATE_CLASS(Pooling);

#endif  // CAFFE_POOLING_LAYER_HPP_
