


/**
 * @brief Takes a Blob and flip it along the "width" side.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */


class FlipLayer : public Layer {
 public:
  explicit FlipLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "Flip"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

 protected:
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  bool flip_width_;
  bool flip_height_;
};





void FlipLayer::LayerSetUp(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  flip_width_ = this->layer_param_.flip_param().flip_width();
  flip_height_ = this->layer_param_.flip_param().flip_height();
}


void FlipLayer::Reshape(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  top[0]->ReshapeLike(*bottom[0]);
}


void FlipLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  Dtype* nextData = nextData;
  int num = prev->size.n;
  int channels = prev->size.c;
  int width = prev->size.w;
  int height = prev->size.h;

  for (n = 0; n < num; n++) {
    for (c = 0; c < channels; c++) {
      for (h = 0; h < height; h++) {
        for (w = 0; w < width; w++) {
          nextData[(((n * channels + c) * height + h) * width) + w] = 
            prevData[(((n * channels + c) * height + (flip_height_ ? (height - 1 - h) : h)) * width) + (flip_width_ ? (width - 1 - w) : w)];
        }
      }
    }
  }
}


void FlipLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  const Dtype* nextDiff = nextDiff;
  Dtype* prevDiff = bottom[0]->mutable_diff();
  int num = prev->size.n;
  int channels = prev->size.c;
  int width = prev->size.w;
  int height = prev->size.h;

  if (propagate_down[0]) {
    for (n = 0; n < num; n++) {
      for (c = 0; c < channels; c++) {
        for (h = 0; h < height; h++) {
          for (w = 0; w < width; w++) {
            prevDiff[(((n * channels + c) * height + h) * width) + w] = 
              nextDiff[(((n * channels + c) * height + (flip_height_ ? (height - 1 - h) : h)) * width) + (flip_width_ ? (width - 1 - w) : w)];
          }
        }
      }
    }
  }
}

#ifdef CPU_ONLY
STUB_GPU(FlipLayer);
#endif

INSTANTIATE_CLASS(FlipLayer);
REGISTER_LAYER_CLASS(Flip);


