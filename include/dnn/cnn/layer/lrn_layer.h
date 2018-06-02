
enum LRNParameter {
  LRNParameter_NormRegion_ACROSS_CHANNELS,
  LRNParameter_NormRegion_WITHIN_CHANNEL
};
static void LRN_CrossChannelForward(const float* prevData, float* nextData, Dtype* scaleBuff, DataSize size, float alpha, float beta, float k, int local_size) {
  // start with the constant value
  int count = Data_count(size);
  int num = size.n;
  int channels = size.c;
  int height = size.h;
  int width = size.w;
  int i, n, c;
  int pre_pad = (local_size - 1) / 2;
  DataSize padded_square;
  Dtype* padded_square_data = NULL;
  Dtype alpha_over_size;
  for (i = 0; i < count; ++i) {
    scaleBuff[i] = k;
  }
  padded_square = dataSize(1, channels + local_size - 1, height, width);
  reset(&padded_square_data, padded_square);
  blas_sset(Data_count(padded_square), (Dtype)(0), padded_square_data);
  alpha_over_size = alpha / local_size;
  // go through the images
  for (n = 0; n < num; ++n) {
    // compute the padded square
    blas_ssqr(channels * height * width, prevData + Data_4DIndex1(size, n), padded_square_data + Data_4DIndex2(padded_square, 0, pre_pad));
    // Create the first channel scale
    for (c = 0; c < local_size; ++c) {
      blas_saxpy(height * width, alpha_over_size,padded_square_data + Data_4DIndex2(padded_square, 0, c),scaleBuff + Data_4DIndex2(size, n, 0));
    }
    for (c = 1; c < channels; ++c) {
      // copy previous scale
      blas_scopy(height * width, scaleBuff + Data_4DIndex2(size, n, c - 1), scaleBuff + Data_4DIndex2(size, n, c));
      // add head
      blas_saxpy(height * width, alpha_over_size,padded_square_data + Data_4DIndex2(padded_square, 0, c + local_size - 1), scaleBuff + Data_4DIndex2(size, n, c));
      // subtract tail
      blas_saxpy(height * width, -alpha_over_size,padded_square_data + Data_4DIndex2(padded_square, 0, c - 1), scaleBuff + Data_4DIndex2(size, n, c));
    }
  }
  
  // In the end, compute output
  blas_spowx(count, scaleBuff, -beta, nextData);
  blas_smul(count, nextData, prevData, nextData);
  FREE(padded_square_data);
}
static void LRN_CrossChannelBackward(const float* prevData, Dtype* prevDiff, const float* nextData, const Dtype* nextDiff,
                              const Dtype* scaleBuff, DataSize size, float alpha, float beta, float k, int local_size) {
  int count = Data_count(size);
  int num = size.n;
  int channels = size.c;
  int height = size.h;
  int width = size.w;
  int n, c;
  int pre_pad = (local_size - 1) / 2;
  int inverse_pre_pad;
  DataSize padded_ratio = dataSize(1, channels + local_size - 1, height, width);
  DataSize accum_ratio = dataSize(1, 1, height, width);
  Dtype* padded_ratio_data = NULL;
  Dtype* accum_ratio_data = NULL;
  // We hack a little bit by using the diff() to store an additional result
  Dtype* accum_ratio_times_bottom = NULL;
  Dtype cache_ratio_value;
  resetZero(&padded_ratio_data, padded_ratio);
  resetZero(&accum_ratio_data, accum_ratio);
  resetZero(&accum_ratio_times_bottom, accum_ratio);
  blas_sset(Data_count(padded_ratio), (Dtype)(0), padded_ratio_data);
  cache_ratio_value = 2. * alpha * beta / local_size;

  blas_spowx(count, scaleBuff, -beta, prevDiff);
  blas_smul(count, nextDiff, prevDiff, prevDiff);

  // go through individual data
  inverse_pre_pad = local_size - (local_size + 1) / 2;
  for (n = 0; n < num; ++n) {
    int block_offset = Data_4DIndex1(size, n);
    // first, compute diff_i * y_i / s_i
    blas_smul(channels * height * width,nextDiff + block_offset, nextData + block_offset,padded_ratio_data + Data_4DIndex2(padded_ratio, 0, inverse_pre_pad));
    blas_sdiv(channels * height * width,padded_ratio_data + Data_4DIndex2(padded_ratio, 0, inverse_pre_pad),scaleBuff + block_offset,padded_ratio_data + Data_4DIndex2(padded_ratio, 0, inverse_pre_pad));
    // Now, compute the accumulated ratios and the bottom diff
    blas_sset(Data_count(accum_ratio), (Dtype)(0), accum_ratio_data);
    for (c = 0; c < local_size - 1; ++c) {
      blas_saxpy(height * width, 1.,padded_ratio_data + Data_4DIndex2(padded_ratio, 0, c), accum_ratio_data);
    }
    for (c = 0; c < channels; ++c) {
      blas_saxpy(height * width, 1.,padded_ratio_data + Data_4DIndex2(padded_ratio, 0, c + local_size - 1),accum_ratio_data);
      // compute bottom diff
      blas_smul(height * width,prevData + Data_4DIndex2(size, n, c),accum_ratio_data, accum_ratio_times_bottom);
      blas_saxpy(height * width, -cache_ratio_value,accum_ratio_times_bottom, prevDiff + Data_4DIndex2(size, n, c));
      blas_saxpy(height * width, -1.,padded_ratio_data + Data_4DIndex2(padded_ratio, 0, c), accum_ratio_data);
    }
  }
  FREE(accum_ratio_times_bottom);
  FREE(padded_ratio_data);
  FREE(accum_ratio_data);
}

/**
 * @brief Normalize the input in a local region across or within feature maps.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
*/
static Dtype LRNLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  int count = Data_count(prev->size);
  
  int local_size = 5;// 默认为5。如果是跨通道LRN，则表示求和的通道数；如果是在通道内LRN，则表示求和的正方形区域长度。
  float alpha = 1;// 默认为1，归一化公式中的参数。
  float beta = 5;// 默认为5，归一化公式中的参数。
  Dtype k = 1.;
  enum LRNParameter norm_region = LRNParameter_NormRegion_ACROSS_CHANNELS;// 默认为ACROSS_CHANNELS。有两个选择，ACROSS_CHANNELS表示在相邻的通道间求和归一化。WITHIN_CHANNEL表示在一个通道内部特定的区域内进行求和归一化。与前面的local_size参数对应。
  
  int pre_pad = (local_size - 1) / 2;
  ASSERT(local_size % 2 == 1 && "LRN only supports odd values for local_size");
  
#if 0
  // Fields used for normalization ACROSS_CHANNELS
  // scale stores the intermediate summing results
  Blob scale;

  // Fields used for normalization WITHIN_CHANNEL
  SplitLayer split_layer_;
  vector<Blob*> split_top_vec_;
  PowerLayer square_layer_;
  Blob square_input_;
  Blob square_output_;
  vector<Blob*> square_bottom_vec_;
  vector<Blob*> square_top_vec_;
  shared_ptr<PoolingLayer > pool_layer_;
  Blob pool_output_;
  vector<Blob*> pool_top_vec_;
  shared_ptr<PowerLayer > power_layer_;
  Blob power_output_;
  vector<Blob*> power_top_vec_;
  shared_ptr<EltwiseLayer > product_layer_;
  Blob product_input_;
  vector<Blob*> product_bottom_vec_;
#endif

  switch (op) {
  case Op_Forward:
    next->size = prev->size;
    {
      const float* prevData = getData(prev);
      float* nextData = setDataZero(next);
      float* scaleBuff = setBuffZero(next, 0);
      LRN_CrossChannelForward(prevData, nextData, scaleBuff, prev->size, alpha, beta, k, local_size);
    }
    break;
  case Op_Backward:
    {
      const float* nextData = getData(next);
      const float* prevData = getData(prev);
      const float* nextDiff = getDiff(next);
      const float* scaleBuff = getBuff(next, 0);
      float* prevDiff = setDiff(prev);
      LRN_CrossChannelBackward(prevData, prevDiff, nextData, nextDiff, scaleBuff, prev->size, alpha, beta, k, local_size);
      //double dd = Blob_diffDot(next);
      //printf("%f\n", dd);
    }
    break;
  default:
    break;
  }
  return 0;
}

static int net_lrn(struct Net* net, int input)
{
  const char* type = "lrn";
  struct Layer* layer = net_addLayer(net, 1, 1, 0, LRNLayer, type);
  LIBLOB(0) = input;
  return LOBLOB(0) = net_addBlob(net, IBLOB(0)->size, type, "o");
}

#if 0

void LRNLayer::LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  if (this->layer_param_.lrn_param().norm_region() ==
      LRNParameter_NormRegion_WITHIN_CHANNEL) {
    // Set up split_layer_ to use inputs in the numerator and denominator.
    split_top_vec_.clear();
    split_top_vec_.push_back(&product_input_);
    split_top_vec_.push_back(&square_input_);
    LayerParameter split_param;
    split_layer_.reset(new SplitLayer(split_param));
    split_layer_->SetUp(bottom, split_top_vec_);
    // Set up square_layer_ to square the inputs.
    square_bottom_vec_.clear();
    square_top_vec_.clear();
    square_bottom_vec_.push_back(&square_input_);
    square_top_vec_.push_back(&square_output_);
    LayerParameter square_param;
    square_param.mutable_power_param()->set_power(Dtype(2));
    square_layer_.reset(new PowerLayer(square_param));
    square_layer_->SetUp(square_bottom_vec_, square_top_vec_);
    // Set up pool_layer_ to sum over square neighborhoods of the input.
    pool_top_vec_.clear();
    pool_top_vec_.push_back(&pool_output_);
    LayerParameter pool_param;
    pool_param.mutable_pooling_param()->set_pool(
        PoolingParameter_PoolMethod_AVE);
    pool_param.mutable_pooling_param()->set_pad(pre_pad);
    pool_param.mutable_pooling_param()->set_kernel_size(local_size);
    pool_layer_.reset(new PoolingLayer(pool_param));
    pool_layer_->SetUp(square_top_vec_, pool_top_vec_);
    // Set up power_layer_ to compute (1 + alpha/N^2 s)^-beta, where s is
    // the sum of a squared neighborhood (the output of pool_layer_).
    power_top_vec_.clear();
    power_top_vec_.push_back(&power_output_);
    LayerParameter power_param;
    power_param.mutable_power_param()->set_power(-beta);
    power_param.mutable_power_param()->set_scale(alpha);
    power_param.mutable_power_param()->set_shift(Dtype(1));
    power_layer_.reset(new PowerLayer(power_param));
    power_layer_->SetUp(pool_top_vec_, power_top_vec_);
    // Set up a product_layer_ to compute outputs by multiplying inputs by the
    // inverse demoninator computed by the power layer.
    product_bottom_vec_.clear();
    product_bottom_vec_.push_back(&product_input_);
    product_bottom_vec_.push_back(&power_output_);
    LayerParameter product_param;
    EltwiseParameter* eltwise_param = product_param.mutable_eltwise_param();
    eltwise_param->set_operation(EltwiseParameter_EltwiseOp_PROD);
    product_layer_.reset(new EltwiseLayer(product_param));
    product_layer_->SetUp(product_bottom_vec_, top);
  }
}


void LRNLayer::Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  CHECK_EQ(4, bottom[0]->num_axes()) << "Input must have 4 axes, "
      << "corresponding to (num, channels, height, width)";
  num = prev->size.n;
  channels = prev->size.c;
  height = prev->size.h;
  width = prev->size.w;
  switch (this->layer_param_.lrn_param().norm_region()) {
  case LRNParameter_NormRegion_ACROSS_CHANNELS:
    top[0]->Reshape(num, channels, height, width);
    scale.Reshape(num, channels, height, width);
    break;
  case LRNParameter_NormRegion_WITHIN_CHANNEL:
    split_layer_->Reshape(bottom, split_top_vec_);
    square_layer_->Reshape(square_bottom_vec_, square_top_vec_);
    pool_layer_->Reshape(square_top_vec_, pool_top_vec_);
    power_layer_->Reshape(pool_top_vec_, power_top_vec_);
    product_layer_->Reshape(product_bottom_vec_, top);
    break;
  }
}


void LRNLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  switch (this->layer_param_.lrn_param().norm_region()) {
  case LRNParameter_NormRegion_ACROSS_CHANNELS:
    CrossChannelForward(bottom, top);
    break;
  case LRNParameter_NormRegion_WITHIN_CHANNEL:
    WithinChannelForward(bottom, top);
    break;
  default:
    LOG(FATAL) << "Unknown normalization region.";
  }
}


void LRNLayer::WithinChannelForward(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
  split_layer_->Forward(bottom, split_top_vec_);
  square_layer_->Forward(square_bottom_vec_, square_top_vec_);
  pool_layer_->Forward(square_top_vec_, pool_top_vec_);
  power_layer_->Forward(pool_top_vec_, power_top_vec_);
  product_layer_->Forward(product_bottom_vec_, top);
}


void LRNLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  switch (this->layer_param_.lrn_param().norm_region()) {
  case LRNParameter_NormRegion_ACROSS_CHANNELS:
    CrossChannelBackward(top, propagate_down, bottom);
    break;
  case LRNParameter_NormRegion_WITHIN_CHANNEL:
    WithinChannelBackward(top, propagate_down, bottom);
    break;
  default:
    LOG(FATAL) << "Unknown normalization region.";
  }
}


void LRNLayer::WithinChannelBackward(
    const vector<Blob*>& top, const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  if (propagate_down[0]) {
    vector<bool> product_propagate_down(2, true);
    product_layer_->Backward(top, product_propagate_down, product_bottom_vec_);
    power_layer_->Backward(power_top_vec_, propagate_down, pool_top_vec_);
    pool_layer_->Backward(pool_top_vec_, propagate_down, square_top_vec_);
    square_layer_->Backward(square_top_vec_, propagate_down,
                            square_bottom_vec_);
    split_layer_->Backward(split_top_vec_, propagate_down, bottom);
  }
}

#endif
