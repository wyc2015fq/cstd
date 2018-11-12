#ifndef _FILLER_INL_
#define _FILLER_INL_



int FUN(ConstantFiller)(DataShape shape, Dtype* data, CJSON* param) {
  const int count = shape.count();
  const Dtype value = (Dtype)param->getfloat("value", 0);
  CHECK(count);
  for (int i = 0; i < count; ++i) {
    data[i] = value;
  }
  CHECK_EQ(param->getfloat("sparse", -1), -1)
    << "Sparsity not supported by this Filler.";
  return 0;
}

/// @brief Fills a Blob with uniformly distributed values @f$ x\sim U(a, b) @f$.
int FUN(UniformFiller)(DataShape shape, Dtype* data, CJSON* param) {
  const int count = shape.count();
  FUN(caffe_rng_uniform)(count, param->getfloat("min", 0), param->getfloat("max", 1), data);
  CHECK_EQ(param->getfloat("sparse", -1), -1)
    << "Sparsity not supported by this Filler.";
  return 0;
}

/// @brief Fills a Blob with Gaussian-distributed values @f$ x = a @f$.
int FUN(GaussianFiller)(DataShape shape, Dtype* data, CJSON* param) {
  const int count = shape.count();
  CHECK(count);
  FUN(caffe_rng_gaussian)(count, Dtype(param->getfloat("mean", 0)),
    Dtype(param->getfloat("std", 1)), data);
  int sparse = (int)param->getfloat("sparse", -1);
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
    caffe_rng_bernoulli(count, non_zero_probability, mask);
    for (int i = 0; i < count; ++i) {
      data[i] *= mask[i];
    }
    free(mask);
  }
  return 0;
}

/** @brief Fills a Blob with values @f$ x \in [0, 1] @f$
 *         such that @f$ \forall i \sum_j x_{ij} = 1 @f$.
 */
int FUN(PositiveUnitballFiller)(DataShape shape, Dtype* data, CJSON* param) {
  const int count = shape.count();
  DCHECK(count);
  FUN(caffe_rng_uniform)(count, 0, 1, data);
  // We expect the filler to not be called very frequently, so we will
  // just use a simple implementation
  int dim = count / shape.num();
  CHECK(dim);
  for (int i = 0; i < shape.num(); ++i) {
    Dtype sum = 0;
    for (int j = 0; j < dim; ++j) {
      sum += data[i * dim + j];
    }
    for (int j = 0; j < dim; ++j) {
      data[i * dim + j] /= sum;
    }
  }
  CHECK_EQ(param->getfloat("sparse", -1), -1)
    << "Sparsity not supported by this Filler.";
  return 0;
}

/**
 * @brief Fills a Blob with values @f$ x \sim U(-a, +a) @f$ where @f$ a @f$ is
 *        set inversely proportional to number of incoming nodes, outgoing
 *        nodes, or their average.
 *
 * A Filler based on the paper [Bengio and Glorot 2010]: Understanding
 * the difficulty of training deep feedforward neuralnetworks.
 *
 * It fills the incoming matrix by randomly sampling uniform data from [-scale,
 * scale] where scale = sqrt(3 / n) where n is the fan_in, fan_out, or their
 * average, depending on the variance_norm option. You should make sure the
 * input blob has shape (num, a, b, c) where a * b * c = fan_in and num * b * c
 * = fan_out. Note that this is currently not the case for inner product layers.
 *
 * TODO(dox): make notation in above comment consistent with rest & use LaTeX.
 */
int FUN(XavierFiller)(DataShape shape, Dtype* data, CJSON* param) {
  const int count = shape.count();
  CHECK(count);
  int fan_in = count / shape.num();
  int fan_out = count / shape.channels();
  Dtype n = (Dtype)fan_in;  // default to fan_in
  VarianceNorm variance_norm = param->getenum("variance_norm", FAN_IN, VarianceNorm_Name, countof(VarianceNorm_Name));
  if (variance_norm == AVERAGE) {
    n = (fan_in + fan_out) / Dtype(2);
  }
  else if (variance_norm == FAN_OUT) {
    n = fan_out;
  }
  Dtype scale = sqrt(Dtype(3) / n);
  caffe_rng_uniform(count, -scale, scale,
    data);
  CHECK_EQ(param->getfloat("sparse", -1), -1)
    << "Sparsity not supported by this Filler.";
  return 0;
}

/**
 * @brief Fills a Blob with values @f$ x \sim N(0, \sigma^2) @f$ where
 *        @f$ \sigma^2 @f$ is set inversely proportional to number of incoming
 *        nodes, outgoing nodes, or their average.
 *
 * A Filler based on the paper [He, Zhang, Ren and Sun 2015]: Specifically
 * accounts for ReLU nonlinearities.
 *
 * Aside: for another perspective on the scaling factor, see the derivation of
 * [Saxe, McClelland, and Ganguli 2013 (v3)].
 *
 * It fills the incoming matrix by randomly sampling Gaussian data with std =
 * sqrt(2 / n) where n is the fan_in, fan_out, or their average, depending on
 * the variance_norm option. You should make sure the input blob has shape (num,
 * a, b, c) where a * b * c = fan_in and num * b * c = fan_out. Note that this
 * is currently not the case for inner product layers.
 */


int FUN(MSRAFiller)(DataShape shape, Dtype* data, CJSON* param) {
  const int count = shape.count();
  CHECK(count);
  int fan_in = count / shape.num();
  int fan_out = count / shape.channels();
  Dtype n = fan_in;  // default to fan_in
  const char* variance_norm = param->GetObjectString("variance_norm", "FAN_IN");
  if (0 == strcmp(variance_norm, "AVERAGE")) {
    n = (fan_in + fan_out) / Dtype(2);
  }
  else if(0 == strcmp(variance_norm, "FAN_OUT")) {
    n = fan_out;
  }
  Dtype std = sqrt(Dtype(2) / n);
  caffe_rng_gaussian(count, Dtype(0), std,
    data);
  CHECK_EQ(param->getfloat("sparse", -1), -1)
    << "Sparsity not supported by this Filler.";
  return 0;
}


/*!
@brief Fills a Blob with coefficients for bilinear interpolation.

A common use case is with the DeconvolutionLayer acting as upsampling.
You can upsample a feature map with shape of (B, C, H, W) by any integer factor
using the following proto.
\code
layer {
  name: "upsample", type: "Deconvolution"
  bottom: "{{bottom_name}}" top: "{{top_name}}"
  convolution_param {
    kernel_size: {{2 * factor - factor % 2}} stride: {{factor}}
    num_output: {{C}} group: {{C}}
    pad: {{ceil((factor - 1) / 2.)}}
    weight_filler: { type: "bilinear" } bias_term: false
  }
  param { lr_mult: 0 decay_mult: 0 }
}
\endcode
Please use this by replacing `{{}}` with your values. By specifying
`num_output: {{C}} group: {{C}}`, it behaves as
channel-wise convolution. The filter shape of this deconvolution layer will be
(C, 1, K, K) where K is `kernel_size`, and this filler will set a (K, K)
interpolation kernel for every channel of the filter identically. The resulting
shape of the top feature map will be (B, C, factor * H, factor * W).
Note that the learning rate and the
weight decay are set to 0 in order to keep coefficient values of bilinear
interpolation unchanged during training. If you apply this to an image, this
operation is equivalent to the following call in Python with Scikit.Image.
\code{.py}
out = skimage.transform.rescale(img, factor, mode='constant', cval=0)
\endcode
 */


int FUN(BilinearFiller)(DataShape shape, Dtype* data, CJSON* param) {
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
  CHECK_EQ(param->getfloat("sparse", -1), -1)
    << "Sparsity not supported by this Filler.";
  return 0;
}

/**
* @brief Get a specific filler from the specification given in FillerParameter.
*
* Ideally this would be replaced by a factory pattern, but we will leave it
* this way for now.
*/

int FUN(Filler)(DataShape shape, Dtype* data, CJSON* param) {
  const std::string & type = cJSON_GetObjectString(param, "type", "xavier");
  //Blob::Dtype* data = blob->mutable_cpu_data();
  if (type == "constant") {
    return FUN(ConstantFiller)(shape, data, param);
  }
  else if (type == "gaussian") {
    return FUN(GaussianFiller)(shape, data, param);
  }
  else if (type == "positive_unitball") {
    return FUN(PositiveUnitballFiller)(shape, data, param);
  }
  else if (type == "uniform") {
    return FUN(UniformFiller)(shape, data, param);
  }
  else if (type == "xavier") {
    return FUN(XavierFiller)(shape, data, param);
  }
  else if (type == "msra") {
    return FUN(MSRAFiller)(shape, data, param);
  }
  else if (type == "bilinear") {
    return FUN(BilinearFiller)(shape, data, param);
  }
  else {
    CHECK(false) << "Unknown filler name: " << param->GetObjectString("type", "constant");
  }
  return 1;
}


#endif // _FILLER_INL_
