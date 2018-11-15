
inline int num_axes() const { return shape_.num_axes(); }

inline string shape_string() const {
  return DataShape_string(shape_);
}
inline int shape(int index) const {
  return shape_.dim[CanonicalAxisIndex(index)];
}
inline int CanonicalAxisIndex(int axis_index) const {
  return shape_.CanonicalAxisIndex(axis_index);
}

void shape(std::vector<int>& out) {
  out.assign(shape_.dim, shape_.dim + shape_.num_axes());
}
std::vector<int> shape_vec() {
  std::vector<int> out;
  out.assign(shape_.dim, shape_.dim + shape_.num_axes());
  return out;
}
/// @brief Deprecated legacy shape accessor num: use shape(0) instead.
inline int num() const { return LegacyShape(0); }
/// @brief Deprecated legacy shape accessor channels: use shape(1) instead.
inline int channels() const { return LegacyShape(1); }
/// @brief Deprecated legacy shape accessor height: use shape(2) instead.
inline int height() const { return LegacyShape(2); }
/// @brief Deprecated legacy shape accessor width: use shape(3) instead.
inline int width() const { return LegacyShape(3); }
inline int LegacyShape(int index) const {
  assert(num_axes() <= 4);
  CHECK_LE(num_axes(), 4)
    << "Cannot use legacy accessors on Blobs with > 4 axes.";
  CHECK_LT(index, 4);
  CHECK_GE(index, -4);
  if (index >= num_axes() || index < -num_axes()) {
    // Axis is out of range, but still in [0, 3] (or [-4, -1] for reverse
    // indexing) -- this special case simulates the one-padding used to fill
    // extraneous axes of legacy blobs.
    return 1;
  }
  return shape(index);
}

inline int count(int start_axis, int end_axis) const {
  return shape_.count(start_axis, end_axis);
}
inline int count(int start_axis) const {
  return count(start_axis, num_axes());
}
inline int offset(const int n, const int c = 0, const int h = 0,
  const int w = 0) const {
  CHECK_GE(n, 0);
  CHECK_LE(n, num());
  CHECK_GE(channels(), 0);
  CHECK_LE(c, channels());
  CHECK_GE(height(), 0);
  CHECK_LE(h, height());
  CHECK_GE(width(), 0);
  CHECK_LE(w, width());
  return ((n * channels() + c) * height() + h) * width() + w;
}

inline int offset(const vector<int> & indices) const {
  CHECK_LE(indices.size(), num_axes());
  int offset = 0;
  for (int i = 0; i < num_axes(); ++i) {
    offset *= shape(i);
    if (indices.size() > i) {
      CHECK_GE(indices[i], 0);
      CHECK_LT(indices[i], shape(i));
      offset += indices[i];
    }
  }
  return offset;
}

inline Dtype data_at(const int n, const int c, const int h,
  const int w) {
  return cpu_data()[offset(n, c, h, w)];
}

inline Dtype diff_at(const int n, const int c, const int h,
  const int w) {
  return cpu_diff()[offset(n, c, h, w)];
}

void Reshape(const vector<int> & shape) {
  int i;
  ASSERT(shape.size()<=kMaxBlobAxes);
  DataShape dshape;
  for (i = 0; i < shape.size(); ++i) {
    dshape.dim[i] = shape[i];
  }
  for (; i < 4; ++i) {
    dshape.dim[i] = 1;
  }
  Reshape(dshape);
}
void set_lr_mult(float lr_mult) {
  lr_mult_ = lr_mult;
}
void set_decay_mult(float decay_mult) {
  decay_mult_ = decay_mult;
}
int FromProto(CJSON* proto) {
  Blob* blob = this;
  DataShape shape;
  cJSON_GetObjectNumberArray(proto, "shape", shape.dim, 4, 1);
  blob->lr_mult_ = proto->getfloat("lr_mult", 1);
  blob->decay_mult_ = proto->getfloat("decay_mult", 1);
  int count = shape.count();
  int nbytes = count * sizeof(float);
  blob->Reshape(shape);
  if (proto->GetObjectItem("data")) {
    Dtype* data = blob->cpu_mdata();
    nbytes = blob->shape_.count();
    cJSON_GetObjectBinaryData(proto, "data", data, nbytes);
  }
  if (proto->GetObjectItem("diff")) {
    Dtype* data = blob->cpu_mdiff();
    cJSON_GetObjectBinaryData(proto, "diff", data, count);
  }
  return 0;
}

double amean_data() {
  return asum_data() / count();
}

double amean_diff() {
  return asum_diff() / count();
}

double asum_data() {
  int count_ = count();
  Dtype asum = caffe_asum(count_, data());
  return asum;
}
double asum_diff() {
  int count_ = count();
  Dtype asum = caffe_asum(count_, diff());
  return asum;
}
double sumsq_data() {
  int count_ = count();
  const Dtype* x = data();
  Dtype sumsq = caffe_dot(count_, x, x);
  return sumsq;
}
double sumsq_diff() {
  int count_ = count();
  const Dtype* x = diff();
  Dtype sumsq = caffe_dot(count_, x, x);
  return sumsq;
}
double Loss() {
  if (loss_weight_ > 0) {
    const int count = this->count();
    const Dtype* data = this->cpu_data();
    //const Dtype* loss_weights = top[top_id]->cpu_diff();
    Dtype* loss_multiplier = cpu_mdiff();
    cpu_caffe_set(count, loss_weight_, loss_multiplier);
    loss_ = cpu_caffe_dot(count, data, loss_multiplier);
  }
  return loss_;
}

void Update()
{
  const int count_ = this->count();
  caffe_axpy(count_, Dtype(-1), diff(), mdata());
}