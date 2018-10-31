
inline int num_axes() const { return (int)4; }

inline string shape_string() const {
  char buf[256];
  const int* p = shape_.dim;
  _snprintf(buf, 256, "(%d %d %d %d)", p[0], p[1], p[2], p[3]);
  return buf;
}
inline int shape(int index) const {
  return shape_.dim[CanonicalAxisIndex(index)];
}
inline int CanonicalAxisIndex(int axis_index) const {
  CHECK_GE(axis_index, -num_axes())
    << "axis " << axis_index << " out of range for " << num_axes()
    << "-D Blob with shape " << shape_string();
  CHECK_LT(axis_index, num_axes())
    << "axis " << axis_index << " out of range for " << num_axes()
    << "-D Blob with shape " << shape_string();
  if (axis_index < 0) {
    return axis_index + num_axes();
  }
  return axis_index;
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
  if (end_axis > num_axes()) {
    int asdf = 0;
  }
  start_axis = min(start_axis, num_axes());
  end_axis = min(end_axis, num_axes());
  CHECK_LE(start_axis, end_axis);
  CHECK_GE(start_axis, 0);
  CHECK_GE(end_axis, 0);
  CHECK_LE(start_axis, num_axes());
  CHECK_LE(end_axis, num_axes());
  int count = 1;
  for (int i = start_axis; i < end_axis; ++i) {
    count *= shape(i);
  }
  return count;
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

void Reshape(const vector<int> & shape) {
  int i;
  ASSERT(shape.size()<=4);
  DataShape dshape;
  for (i = 0; i < shape.size(); ++i) {
    dshape.dim[i] = shape[i];
  }
  for (; i < 4; ++i) {
    dshape.dim[i] = 1;
  }
  Reshape(dshape);
}
int FromProto(CJSON* proto) {
  Blob<Dtype>* blob = this;
  DataShape shape;
  cJSON_GetObjectNumberArray(proto, "shape", shape.dim, 4, 1);
  int count = shape.count();
  int nbytes = count * sizeof(float);
  blob->Reshape(shape);
  if (proto->GetObjectItem("data")) {
    Dtype* data = blob->mutable_data<Context>();
    nbytes = blob->shape_.count();
    cJSON_GetObjectBinaryData(proto, "data", data, nbytes);
  }
  if (proto->GetObjectItem("diff")) {
    float* data = blob->mutable_diff<Context>();
    cJSON_GetObjectBinaryData(proto, "diff", data, count);
  }
  return 0;
}

double asum_data() const {
  Dtype asum;
  int count_ = count();
  caffe_asum(CONTEXT, count_, data<Context>(), &asum);
  return asum;
}
double asum_diff() const {
  Dtype asum;
  int count_ = count();
  caffe_asum(CONTEXT, count_, diff<Context>(), &asum);
  return asum;
}
double sumsq_data() const {
  Dtype sumsq;
  int count_ = count();
  const Dtype* x = data<Context>();
  sumsq = caffe_dot(CONTEXT, count_, x, x);
  return sumsq;
}
double sumsq_diff() const {
  Dtype sumsq;
  int count_ = count();
  const Dtype* x = data<Context>();
  sumsq = caffe_dot(CONTEXT, count_, x, x);
  return sumsq;
}
double Loss() {
  const int count = this->count();
  const Dtype* data = this->data<CPUContext>();
  //const Dtype* loss_weights = top[top_id]->cpu_diff();
  Dtype* loss_multiplier = mutable_diff<CPUContext>();
  caffe_set(count, loss_weight_, loss_multiplier);
  loss_ = caffe_dot<Dtype>(CPUCONTEXT, count, data, loss_multiplier);
  return loss_;
}

void Update()
{
  const int count_ = this->count();
  caffe_axpy<Dtype>(CONTEXT, count_, Dtype(-1), diff<Context>(), mutable_data<Context>());
}
