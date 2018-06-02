


/**
 * @brief Also known as a "marginal fully-connected" layer, computes an marginal inner product
 *        with a set of learned weights, and (optionally) adds biases.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */

class MarginInnerProductLayer : public Layer {
 public:
  explicit MarginInnerProductLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "MarginInnerProduct"; }
  virtual inline int ExactNumBottomBlobs() const { return 2; }
  virtual inline int MaxTopBlobs() const { return 2; }

 protected:
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  int M_;
  int K_;
  int N_;
  
  MarginInnerProductParameter_MarginType type_;

  // common variables
  Blob x_norm_;
  Blob cos_theta_;
  Blob sign_0_; // sign_0 = sign(cos_theta)
  // for DOUBLE type
  Blob cos_theta_quadratic_;
  // for TRIPLE type
  Blob sign_1_; // sign_1 = sign(abs(cos_theta) - 0.5)
  Blob sign_2_; // sign_2 = sign_0 * (1 + sign_1) - 2
  Blob cos_theta_cubic_;
  // for QUADRA type
  Blob sign_3_; // sign_3 = sign_0 * sign(2 * cos_theta_quadratic_ - 1)
  Blob sign_4_; // sign_4 = 2 * sign_0 + sign_3 - 3
  Blob cos_theta_quartic_;

  int iter_;
  Dtype lambda_;

};




void MarginInnerProductLayer::LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  CHECK_EQ(prev->size.n, prev1->size.n)
      << "Number of labels must match number of output; "
      << "DO NOT support multi-label this version."
      << "e.g., if prediction shape is (M X N), "
      << "label count (number of labels) must be M, "
      << "with integer values in {0, 1, ..., N-1}.";

  type_ = this->layer_param_.margin_inner_product_param().type();
  iter_ = this->layer_param_.margin_inner_product_param().iteration();
  lambda_ = (Dtype)0.;

  const int num_output = this->layer_param_.margin_inner_product_param().num_output();
  N_ = num_output;
  const int axis = bottom[0]->CanonicalAxisIndex(
      this->layer_param_.margin_inner_product_param().axis());
  // Dimensions starting from "axis" are "flattened" into a single
  // length K_ vector. For example, if bottom[0]'s shape is (N, C, H, W),
  // and axis == 1, N inner products with dimension CHW are performed.
  K_ = bottom[0]->count(axis);
  // Check if we need to set up the weights
  if (this->blobs_.size() > 0) {
    LOG(INFO) << "Skipping parameter initialization";
  } else {
    this->blobs_.resize(1);
    // Intialize the weight
    vector<int> weight_shape(2);
    weight_shape[0] = N_;
    weight_shape[1] = K_;
    this->blobs_[0].reset(new Blob(weight_shape));
    // fill the weights
    shared_ptr<Filler > weight_filler(GetFiller(
        this->layer_param_.margin_inner_product_param().weight_filler()));
    weight_filler->Fill(this->blobs_[0].get());
  }  // parameter initialization
  this->param_propagate_down_.resize(this->blobs_.size(), true);
}


void MarginInnerProductLayer::Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  // Figure out the dimensions
  const int axis = bottom[0]->CanonicalAxisIndex(
      this->layer_param_.margin_inner_product_param().axis());
  const int new_K = bottom[0]->count(axis);
  CHECK_EQ(K_, new_K)
      << "Input size incompatible with inner product parameters.";
  // The first "axis" dimensions are independent inner products; the total
  // number of these is M_, the product over these dimensions.
  M_ = bottom[0]->count(0, axis);
  // The top shape will be the bottom shape with the flattened axes dropped,
  // and replaced by a single axis with dimension num_output (N_).
  vector<int> top_shape = bottom[0]->shape();
  top_shape.resize(axis + 1);
  top_shape[axis] = N_;
  top[0]->Reshape(top_shape);

  // if needed, reshape top[1] to output lambda
  if (layer->o_size == 2) {
    vector<int> lambda_shape(1, 1);
    top[1]->Reshape(lambda_shape);
  }
  
  // common temp variables
  vector<int> shape_1_X_M(1, M_);
  x_norm_.Reshape(shape_1_X_M);
  sign_0_.Reshape(top_shape);
  cos_theta_.Reshape(top_shape);

  // optional temp variables
  switch (type_) {
  case MarginInnerProductParameter_MarginType_SINGLE:
    break;
  case MarginInnerProductParameter_MarginType_DOUBLE:
    cos_theta_quadratic_.Reshape(top_shape);
    break;
  case MarginInnerProductParameter_MarginType_TRIPLE:
    cos_theta_quadratic_.Reshape(top_shape);
    cos_theta_cubic_.Reshape(top_shape);
    sign_1_.Reshape(top_shape);
    sign_2_.Reshape(top_shape);
    break;
  case MarginInnerProductParameter_MarginType_QUADRUPLE:
    cos_theta_quadratic_.Reshape(top_shape);
    cos_theta_cubic_.Reshape(top_shape);
    cos_theta_quartic_.Reshape(top_shape);
    sign_3_.Reshape(top_shape);
    sign_4_.Reshape(top_shape);
    break;
  default:
    LOG(FATAL) << "Unknown margin type.";
  }
}


void MarginInnerProductLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  iter_ += (Dtype)1.;
  Dtype base_ = this->layer_param_.margin_inner_product_param().base();
  Dtype gamma_ = this->layer_param_.margin_inner_product_param().gamma();
  Dtype power_ = this->layer_param_.margin_inner_product_param().power();
  Dtype lambda_min_ = this->layer_param_.margin_inner_product_param().lambda_min();
  lambda_ = base_ * pow(((Dtype)1. + gamma_ * iter_), -power_);
  lambda_ = MAX(lambda_, lambda_min_);
  top[1]->mutable_data()[0] = lambda_;

  /************************* normalize weight *************************/
  Dtype* norm_weight = this->blobs_[0]->mutable_data();
  Dtype temp_norm = (Dtype)0.;
  for (i = 0; i < N_; i++) {
  	temp_norm = blas_sdot(K_, norm_weight + i * K_, norm_weight + i * K_);
  	temp_norm = (Dtype)1./sqrt(temp_norm);
  	blas_sscal(K_, temp_norm, norm_weight + i * K_);
  }

  /************************* common variables *************************/
  // x_norm_ = |x|
  const Dtype* prevData = prevData;
  const Dtype* weight = this->blobs_[0]->cpu_data();
  Dtype* mutable_x_norm_data = x_norm_;
  for (i = 0; i < M_; i++) {
    mutable_x_norm_data[i] = sqrt(blas_sdot(K_, prevData + i * K_, prevData + i * K_));
  }
  // cos_theta = x'w/|x|
  Dtype* mutable_cos_theta_data = cos_theta_;
  blas_sgemm(CblasNoTrans, CblasTrans, M_, N_, K_, (Dtype)1.,
      prevData, weight, (Dtype)0., mutable_cos_theta_data);
  for (i = 0; i < M_; i++) {
    blas_sscal(N_, (Dtype)1./mutable_x_norm_data[i], mutable_cos_theta_data + i * N_);
  }
  // sign_0 = sign(cos_theta)
  blas_ssign(M_ * N_, cos_theta_.cpu_data(), sign_0_);

  /************************* optional variables *************************/
  switch (type_) {
  case MarginInnerProductParameter_MarginType_SINGLE:
    break;
  case MarginInnerProductParameter_MarginType_DOUBLE:
    // cos_theta_quadratic
    blas_spowx(M_ * N_, cos_theta_.cpu_data(), (Dtype)2., cos_theta_quadratic_);
    break;
  case MarginInnerProductParameter_MarginType_TRIPLE:
    // cos_theta_quadratic && cos_theta_cubic
    blas_spowx(M_ * N_, cos_theta_.cpu_data(), (Dtype)2., cos_theta_quadratic_);
    blas_spowx(M_ * N_, cos_theta_.cpu_data(), (Dtype)3., cos_theta_cubic_);
    // sign_1 = sign(abs(cos_theta) - 0.5)
    blas_sabs(M_ * N_, cos_theta_.cpu_data(), sign_1_);
    blas_sadd_scalar(M_ * N_, -(Dtype)0.5, sign_1_);
    blas_ssign(M_ * N_, sign_1_.cpu_data(), sign_1_);
    // sign_2 = sign_0 * (1 + sign_1) - 2
    blas_scopy(M_ * N_, sign_1_.cpu_data(), sign_2_);
    blas_sadd_scalar(M_ * N_, (Dtype)1., sign_2_);
    blas_smul(M_ * N_, sign_0_.cpu_data(), sign_2_.cpu_data(), sign_2_);
    blas_sadd_scalar(M_ * N_, - (Dtype)2., sign_2_);
    break;
  case MarginInnerProductParameter_MarginType_QUADRUPLE:
    // cos_theta_quadratic && cos_theta_cubic && cos_theta_quartic
    blas_spowx(M_ * N_, cos_theta_.cpu_data(), (Dtype)2., cos_theta_quadratic_);
    blas_spowx(M_ * N_, cos_theta_.cpu_data(), (Dtype)3., cos_theta_cubic_);
    blas_spowx(M_ * N_, cos_theta_.cpu_data(), (Dtype)4., cos_theta_quartic_);
    // sign_3 = sign_0 * sign(2 * cos_theta_quadratic_ - 1)
    blas_scopy(M_ * N_, cos_theta_quadratic_.cpu_data(), sign_3_);
    blas_sscal(M_ * N_, (Dtype)2., sign_3_);
    blas_sadd_scalar(M_ * N_, (Dtype)-1., sign_3_);
    blas_ssign(M_ * N_, sign_3_.cpu_data(), sign_3_);
    blas_smul(M_ * N_, sign_0_.cpu_data(), sign_3_.cpu_data(), sign_3_);
    // sign_4 = 2 * sign_0 + sign_3 - 3
    blas_scopy(M_ * N_, sign_0_.cpu_data(), sign_4_);
    blas_sscal(M_ * N_, (Dtype)2., sign_4_);
    blas_sadd(M_ * N_, sign_4_.cpu_data(), sign_3_.cpu_data(), sign_4_);
    blas_sadd_scalar(M_ * N_, - (Dtype)3., sign_4_);
    break;
  default:
    LOG(FATAL) << "Unknown margin type.";
  }

  /************************* Forward *************************/
  Dtype* nextData = nextData;
  blas_sgemm(CblasNoTrans, CblasTrans, M_, N_, K_, (Dtype)1.,
      prevData, weight, (Dtype)0., nextData);
  const Dtype* label = nextData;
  const Dtype* x_norm_data = x_norm_.cpu_data();
    switch (type_) {
  case MarginInnerProductParameter_MarginType_SINGLE: {
    break;
  }
  case MarginInnerProductParameter_MarginType_DOUBLE: {
  	const Dtype* sign_0_data = sign_0_.cpu_data();
  	const Dtype* cos_theta_quadratic_data = cos_theta_quadratic_.cpu_data();
    // the label[i]_th nextData
    for (i = 0; i < M_; i++) {
      const int label_value = (int)(label[i]);
      // |x| * (2 * sign_0 * cos_theta_quadratic - 1)
      nextData[i * N_ + label_value] = x_norm_data[i] * ((Dtype)2. * sign_0_data[i * N_ + label_value] * 
                                       cos_theta_quadratic_data[i * N_ + label_value] - (Dtype)1.);
    }
    // + lambda * x'w
    blas_sgemm(CblasNoTrans, CblasTrans, M_, N_, K_, lambda_,
      prevData, weight, (Dtype)1., nextData);
    // * 1 / (1 + lambda)
    blas_sscal(M_ * N_, (Dtype)1./((Dtype)1. + lambda_), nextData);
    break;
  }
  case MarginInnerProductParameter_MarginType_TRIPLE: {
  	const Dtype* sign_1_data = sign_1_.cpu_data();
    const Dtype* sign_2_data = sign_2_.cpu_data();
    const Dtype* cos_theta_data = cos_theta_.cpu_data();
    const Dtype* cos_theta_cubic_data = cos_theta_cubic_.cpu_data();
    // the label[i]_th output
    for (i = 0; i < M_; i++) {
      const int label_value = (int)(label[i]);
      // |x| * (sign_1 * (4 * cos_theta_cubic - 3 * cos_theta) + sign_2)
      nextData[i * N_ + label_value] = x_norm_data[i] * (sign_1_data[i * N_ + label_value] * 
                                      ((Dtype)4. * cos_theta_cubic_data[i * N_ + label_value] - 
                                       (Dtype)3. * cos_theta_data[i * N_ + label_value]) + 
                                       sign_2_data[i * N_ + label_value]);
    }
    // + lambda * x'w
    blas_sgemm(CblasNoTrans, CblasTrans, M_, N_, K_, lambda_,
      prevData, weight, (Dtype)1., nextData);
    // / (1 + lambda)
    blas_sscal(M_ * N_, (Dtype)1./((Dtype)1. + lambda_), nextData);
    break;
  }
  case MarginInnerProductParameter_MarginType_QUADRUPLE: {
  	const Dtype* sign_3_data = sign_3_.cpu_data();
    const Dtype* sign_4_data = sign_4_.cpu_data();
    const Dtype* cos_theta_quadratic_data = cos_theta_quadratic_.cpu_data();
    const Dtype* cos_theta_quartic_data = cos_theta_quartic_.cpu_data();
    // the label[i]_th output
    for (i = 0; i < M_; i++) {
      const int label_value = (int)(label[i]);
      // // |x| * (sign_3 * (8 * cos_theta_quartic - 8 * cos_theta_quadratic + 1) + sign_4)
      nextData[i * N_ + label_value] = x_norm_data[i] * (sign_3_data[i * N_ + label_value] * 
                                       ((Dtype)8. * cos_theta_quartic_data[i * N_ + label_value] - 
                                        (Dtype)8. * cos_theta_quadratic_data[i * N_ + label_value] + 
                                        (Dtype)1.) + sign_4_data[i * N_ + label_value]);
    }
    // + lambda * x'w
    blas_sgemm(CblasNoTrans, CblasTrans, M_, N_, K_, lambda_,
      prevData, weight, (Dtype)1., nextData);
    // / (1 + lambda)
    blas_sscal(M_ * N_, (Dtype)1./((Dtype)1. + lambda_), nextData);
    break;
  }
  default: {
    LOG(FATAL) << "Unknown margin type.";
  }
  }
}


void MarginInnerProductLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {

  const Dtype* nextDiff = nextDiff;
  const Dtype* prevData = prevData;
  const Dtype* label = nextData;
  const Dtype* weight = this->blobs_[0]->cpu_data();
 
  // Gradient with respect to weight
  if (this->param_propagate_down_[0]) {
    blas_sgemm(CblasTrans, CblasNoTrans, N_, K_, M_, (Dtype)1.,
        nextDiff, prevData, (Dtype)1., this->blobs_[0]->mutable_diff());
  }
  
  // Gradient with respect to bottom data
  if (propagate_down[0]) {
    Dtype* prevDiff = bottom[0]->mutable_diff();
    const Dtype* x_norm_data = x_norm_.cpu_data();
    blas_sset(M_ * K_, (Dtype)(0), prevDiff);
    switch (type_) {
    case MarginInnerProductParameter_MarginType_SINGLE: {
      blas_sgemm(CblasNoTrans, CblasNoTrans, M_, K_, N_, (Dtype)1.,
        nextDiff, this->blobs_[0]->cpu_data(), (Dtype)0.,
        bottom[0]->mutable_diff());
      break;
    }
    case MarginInnerProductParameter_MarginType_DOUBLE: {
      const Dtype* sign_0_data = sign_0_.cpu_data();
      const Dtype* cos_theta_data = cos_theta_.cpu_data();
      const Dtype* cos_theta_quadratic_data = cos_theta_quadratic_.cpu_data();
      for (i = 0; i < M_; i++) {
        const int label_value = (int)(label[i]);
        for (j = 0; j < N_; j++) {
          if (label_value != j) {
            // 1 / (1 + lambda) * w
            blas_saxpby(K_, (Dtype)1. / ((Dtype)1. + lambda_) * nextDiff[i * N_ + j], 
                            weight + j * K_, (Dtype)1., prevDiff + i * K_);
          } else {
            // 4 * sign_0 * cos_theta * w
            Dtype coeff_w = (Dtype)4. * sign_0_data[i * N_ + j] * cos_theta_data[i * N_ + j];
            // 1 / (-|x|) * (2 * sign_0 * cos_theta_quadratic + 1) * x
            Dtype coeff_x = (Dtype)1. / (-x_norm_data[i]) * ((Dtype)2. * 
                            sign_0_data[i * N_ + j] * cos_theta_quadratic_data[i * N_ + j] + (Dtype)1.);
            Dtype coeff_norm = sqrt(coeff_w * coeff_w + coeff_x * coeff_x);
            coeff_w = coeff_w / coeff_norm;
            coeff_x = coeff_x / coeff_norm;
            blas_saxpby(K_, (Dtype)1. / ((Dtype)1. + lambda_) * nextDiff[i * N_ + j] * coeff_w, 
                            weight + j * K_, (Dtype)1., prevDiff + i * K_);
            blas_saxpby(K_, (Dtype)1. / ((Dtype)1. + lambda_) * nextDiff[i * N_ + j] * coeff_x, 
                            prevData + i * K_, (Dtype)1., prevDiff + i * K_);
          }
        }
      }
      // + lambda/(1 + lambda) * w
      blas_sgemm(CblasNoTrans, CblasNoTrans, M_, K_, N_, lambda_/((Dtype)1. + lambda_),
        nextDiff, this->blobs_[0]->cpu_data(), (Dtype)1.,
        bottom[0]->mutable_diff());
      break;
    }
    case MarginInnerProductParameter_MarginType_TRIPLE: {
      const Dtype* sign_1_data = sign_1_.cpu_data();
      const Dtype* sign_2_data = sign_2_.cpu_data();
      const Dtype* cos_theta_quadratic_data = cos_theta_quadratic_.cpu_data();
      const Dtype* cos_theta_cubic_data = cos_theta_cubic_.cpu_data();
      for (i = 0; i < M_; i++) {
        const int label_value = (int)(label[i]);
        for (j = 0; j < N_; j++) {
          if (label_value != j) {
            blas_saxpby(K_, (Dtype)1. / ((Dtype)1. + lambda_) * nextDiff[i * N_ + j], 
                            weight + j * K_, (Dtype)1., prevDiff + i * K_);
          } else {
            // sign_1 * (12 * cos_theta_quadratic - 3) * w
            Dtype coeff_w = sign_1_data[i * N_ + j] * ((Dtype)12. * 
                            cos_theta_quadratic_data[i * N_ + j] - (Dtype)3.);
            // 1 / (-|x|) * (8 * sign_1 * cos_theta_cubic - sign_2) * x
            Dtype coeff_x = (Dtype)1. / (-x_norm_data[i]) * ((Dtype)8. * sign_1_data[i * N_ + j] * 
                              cos_theta_cubic_data[i * N_ + j] - sign_2_data[i * N_ +j]);
            Dtype coeff_norm = sqrt(coeff_w * coeff_w + coeff_x * coeff_x);
            coeff_w = coeff_w / coeff_norm;
            coeff_x = coeff_x / coeff_norm;
            blas_saxpby(K_, (Dtype)1. / ((Dtype)1. + lambda_) * nextDiff[i * N_ + j] * coeff_w, 
                            weight + j * K_, (Dtype)1., prevDiff + i * K_);
            blas_saxpby(K_, (Dtype)1. / ((Dtype)1. + lambda_) * nextDiff[i * N_ + j] * coeff_x, 
                            prevData + i * K_, (Dtype)1., prevDiff + i * K_);
          }
        }
      }
      // + lambda/(1 + lambda) * w
      blas_sgemm(CblasNoTrans, CblasNoTrans, M_, K_, N_, lambda_/((Dtype)1. + lambda_),
        nextDiff, this->blobs_[0]->cpu_data(), (Dtype)1.,
        bottom[0]->mutable_diff());
      break;
    }
    case MarginInnerProductParameter_MarginType_QUADRUPLE: {
      const Dtype* sign_3_data = sign_3_.cpu_data();
      const Dtype* sign_4_data = sign_4_.cpu_data();
      const Dtype* cos_theta_data = cos_theta_.cpu_data();
      const Dtype* cos_theta_quadratic_data = cos_theta_quadratic_.cpu_data();
      const Dtype* cos_theta_cubic_data = cos_theta_cubic_.cpu_data();
      const Dtype* cos_theta_quartic_data = cos_theta_quartic_.cpu_data();
      for (i = 0; i < M_; i++) {
        const int label_value = (int)(label[i]);
        for (j = 0; j < N_; j++) {
          if (label_value != j) {
            blas_saxpby(K_, (Dtype)1. / ((Dtype)1. + lambda_) * nextDiff[i * N_ + j], 
                            weight + j * K_, (Dtype)1., prevDiff + i * K_);
          } else {
            // 1 / (1 + lambda) * sign_3 * (32 * cos_theta_cubic - 16 * cos_theta) * w
            Dtype coeff_w = sign_3_data[i * N_ + j] * ((Dtype)32. * cos_theta_cubic_data[i * N_ + j] -
                                (Dtype)16. * cos_theta_data[i * N_ + j]);
            // 1 / (-|x|) * (sign_3 * (24 * cos_theta_quartic - 8 * cos_theta_quadratic - 1) + 
            //                        sign_4) * x
            Dtype coeff_x = (Dtype)1. / (-x_norm_data[i]) * (sign_3_data[i * N_ + j] * 
                            ((Dtype)24. * cos_theta_quartic_data[i * N_ + j] - 
                            (Dtype)8. * cos_theta_quadratic_data[i * N_ + j] - (Dtype)1.) - 
                             sign_4_data[i * N_ + j]);
            Dtype coeff_norm = sqrt(coeff_w * coeff_w + coeff_x * coeff_x);
            coeff_w = coeff_w / coeff_norm;
            coeff_x = coeff_x / coeff_norm;
            blas_saxpby(K_, (Dtype)1. / ((Dtype)1. + lambda_) * nextDiff[i * N_ + j] * coeff_w, 
                            weight + j * K_, (Dtype)1., prevDiff + i * K_);
            blas_saxpby(K_, (Dtype)1. / ((Dtype)1. + lambda_) * nextDiff[i * N_ + j] * coeff_x, 
                            prevData + i * K_, (Dtype)1., prevDiff + i * K_);
          }
        }
      }
      // + lambda/(1 + lambda) * w
      blas_sgemm(CblasNoTrans, CblasNoTrans, M_, K_, N_, lambda_/((Dtype)1. + lambda_),
        nextDiff, this->blobs_[0]->cpu_data(), (Dtype)1.,
        bottom[0]->mutable_diff());
      break;
    }
    default: {
      LOG(FATAL) << "Unknown margin type.";
    }
    }
  }
}

#ifdef CPU_ONLY
STUB_GPU(MarginInnerProductLayer);
#endif

INSTANTIATE_CLASS(MarginInnerProductLayer);
REGISTER_LAYER_CLASS(MarginInnerProduct);


