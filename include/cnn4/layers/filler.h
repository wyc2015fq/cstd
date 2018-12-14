#ifndef _FILLER_INL_
#define _FILLER_INL_

int FUN(ConstantFiller)(DataShape shape, Dtype* data, const Stype value) {
  const int count = shape.count();
  CHECK(count);
  for (int i = 0; i < count; ++i) {
    data[i] = value;
  }
  return 0;
}

/// @brief Fills a Blob with uniformly distributed values @f$ x\sim U(a, b) @f$.
int FUN(UniformFiller)(DataShape shape, Dtype* data, const Stype a, const Stype b) {
  const int count = shape.count();
  cpu_caffe_rng_uniform(count, a, b, data);
  return 0;
}

/// @brief Fills a Blob with Gaussian-distributed values @f$ x = a @f$.
int FUN(GaussianFiller)(DataShape shape, Dtype* data, const Stype mu, const Stype st, int sparse) {
  const int count = shape.count();
  CHECK(count);
  FUN(caffe_rng_gaussian)(count, mu, st, data);
  CHECK_GE(sparse, -1);
  if (sparse >= 0) {
    // Sparse initialization is implemented for "weight" blobs; i.e. matrices.
    // These have num == channels == 1; width is number of inputs; height is
    // number of outputs.  The 'sparse' variable specifies the mean number
    // of non-zero input weights for a given output.
    CHECK_GE(shape.num_axes(), 1);
    const int num_outputs = shape.shape(0);
    Dtype non_zero_probability = Dtype(sparse) / Dtype(num_outputs);
    int* mask = reinterpret_cast<int*>(malloc(count * sizeof(int)));
    cpu_caffe_rng_bernoulli(count, non_zero_probability, mask);
    for (int i = 0; i < count; ++i) {
      data[i] *= mask[i];
    }
    free(mask);
  }
  return 0;
}

int FUN(PositiveUnitballFiller)(DataShape shape, Dtype* data) {
  const int count = shape.count();
  DCHECK(count);
  FUN(caffe_rng_uniform)(count, 0, 1, data);
  // We expect the filler to not be called very frequently, so we will
  // just use a simple implementation
  int i, j, dim = count / shape.num();
  CHECK(dim);
  for (i = 0; i < shape.num(); ++i) {
    Dtype sum = 0;
    for (j = 0; j < dim; ++j) {
      sum += data[i * dim + j];
    }
    for (j = 0; j < dim; ++j) {
      data[i * dim + j] /= sum;
    }
  }
  return 0;
}

int FUN(XavierFiller)(DataShape shape, Dtype* data, VarianceNorm variance_norm) {
  const int count = shape.count();
  CHECK(count);
  int fan_in = count / shape.num();
  int fan_out = count / shape.channels();
  Dtype n = (Dtype)fan_in;  // default to fan_in
  if (variance_norm == AVERAGE) {
    n = (fan_in + fan_out) / Dtype(2);
  }
  else if (variance_norm == FAN_OUT) {
    n = fan_out;
  }
  Dtype scale = sqrt(Dtype(3) / n);
  cpu_caffe_rng_uniform(count, -scale, scale, data);
  return 0;
}

int FUN(MSRAFiller)(DataShape shape, Dtype* data, VarianceNorm variance_norm) {
  const int count = shape.count();
  CHECK(count);
  int fan_in = count / shape.num();
  int fan_out = count / shape.channels();
  Dtype n = fan_in;  // default to fan_in
  if (variance_norm == AVERAGE) {
    n = (fan_in + fan_out) / Dtype(2);
  }
  else if(variance_norm == FAN_OUT) {
    n = fan_out;
  }
  Dtype std = sqrt(Dtype(2) / n);
  cpu_caffe_rng_gaussian(count, Dtype(0), std, data);
  return 0;
}

int FUN(BilinearFiller)(DataShape shape, Dtype* data) {
  const int count = shape.count();
  CHECK_EQ(shape.num_axes(), 4) << "Blob must be 4 dim.";
  CHECK_EQ(shape.width(), shape.height()) << "Filter must be square";
  int f = ceil(shape.width() / 2.);
  float c = (2 * f - 1 - f % 2) / (2. * f);
  for (int i = 0; i < count; ++i) {
    float x = i % shape.width();
    float y = (i / shape.width()) % shape.height();
    data[i] = (1 - fabs(x / f - c)) * (1 - fabs(y / f - c));
  }
  return 0;
}



#endif // _FILLER_INL_
