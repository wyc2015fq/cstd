


/**
 * @brief Compute elementwise operations, such as product and sum,
 *        along multiple input Blobs.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */

class PairwiseLayer : public Layer {
 public:
  explicit PairwiseLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "Pairwise"; }
  virtual inline int ExactBottomBlobs() const { return 2; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

 protected:
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  PairwiseParameter_PairwiseOp op_;
  vector coeffs_;
  Blob<int> max_idx_;

  int M_, N_, K_;
};




void PairwiseLayer::LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  CHECK(this->layer_param().pairwise_param().coeff_size() == 0
      || this->layer_param().pairwise_param().coeff_size() == layer->i_size) <<
      "Pairwise Layer takes one coefficient per bottom blob.";
  CHECK(!(this->layer_param().pairwise_param().operation()
      == PairwiseParameter_PairwiseOp_PROD
      && this->layer_param().pairwise_param().coeff_size())) <<
      "Pairwise layer only takes coefficients for summation.";
  op_ = this->layer_param_.pairwise_param().operation();
  // Blob-wise coefficients for the elementwise operation.
  coeffs_ = vector(layer->i_size, 1);
  if (this->layer_param().pairwise_param().coeff_size()) {
    for (i = 0; i < layer->i_size; ++i) {
      coeffs_[i] = this->layer_param().pairwise_param().coeff(i);
    }
  }
}


void PairwiseLayer::Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  CHECK_EQ(prev->size.c, prev1->size.c);
  M_ = prev->size.n;
  N_ = prev1->size.n;
  K_ = prev->size.c;
  top[0]->Reshape({ M_, N_, K_ });
  // If max operation, we will initialize the vector index part.
  if (this->layer_param_.pairwise_param().operation() ==
      PairwiseParameter_PairwiseOp_MAX && layer->o_size == 1) {
    max_idx_.Reshape(top[0]->shape());
  }
}


void PairwiseLayer::Forward(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
  int* mask = NULL;
  const Dtype* bottom_data_a = prevData;
  const Dtype* bottom_data_b = nextData;
  const int count = top[0]->count();
  Dtype* nextData = nextData;
  switch (op_) {
  case PairwiseParameter_PairwiseOp_PROD:
    for (m = 0; m < M_; m++) {
      for (n = 0; n < N_; n++) {
        blas_smul(K_, bottom_data_a + m*K_, bottom_data_b + n*K_, nextData + (m*N_ + n)*K_);
      }
    }
    break;
  case PairwiseParameter_PairwiseOp_SUM:
    blas_sset(count, (Dtype)(0), nextData);
    for (m = 0; m < M_; m++) {
      for (n = 0; n < N_; n++) {
        blas_saxpy(K_, coeffs_[0], bottom_data_a + m*K_, nextData + (m*N_ + n)*K_);
        blas_saxpy(K_, coeffs_[1], bottom_data_b + n*K_, nextData + (m*N_ + n)*K_);
        //for (k = 0; k < K_; k++) {
        //  nextData[(m*N_ + n)*K_ + k] = coeffs_[0] * bottom_data_a[m*K_ + k] + coeffs_[1] * bottom_data_b[n*K_ + k];
        //}
      }
    }
    break;
  case PairwiseParameter_PairwiseOp_MAX:
    // Initialize
    mask = max_idx_;
    for (m = 0; m < M_; m++) {
      for (n = 0; n < N_; n++) {
        for (k = 0; k < K_; k++) {
          if (bottom_data_a[m*K_ + k] > bottom_data_b[n*K_ + k]) {
            nextData[(m*N_ + n)*K_ + k] = bottom_data_a[m*K_ + k];
            mask[(m*N_ + n)*K_ + k] = 0;
          }
          else {
            nextData[(m*N_ + n)*K_ + k] = bottom_data_b[n*K_ + k];
            mask[(m*N_ + n)*K_ + k] = 1;
          }
        }
      }
    }
    break;
  default:
    LOG(FATAL) << "Unknown pairwise operation.";
  }
}


void PairwiseLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  const int* mask = NULL;
  const int count = top[0]->count();
  const Dtype* nextData = top[0]->cpu_data();
  const Dtype* nextDiff = nextDiff;
  const Dtype* bottom_data_a = prevData;
  const Dtype* bottom_data_b = nextData;
  Dtype* bottom_diff_a = bottom[0]->mutable_diff();
  Dtype* bottom_diff_b = bottom[1]->mutable_diff();
  if (propagate_down[0]) {
    switch (op_) {
    case PairwiseParameter_PairwiseOp_PROD:
      blas_sset(Data_count(prev->size), (Dtype)(0), bottom_diff_a);
      for (m = 0; m < M_; m++) {
        for (n = 0; n < N_; n++) {
          for (k = 0; k < K_; k++) {
            bottom_diff_a[m*K_ + k] += nextDiff[(m*N_ + n)*K_ + k] * bottom_data_b[n*K_ + k];
          }
        }
      }
      break;
    case PairwiseParameter_PairwiseOp_SUM:
      blas_sset(Data_count(prev->size), (Dtype)(0), bottom_diff_a);
      for (m = 0; m < M_; m++) {
        for (n = 0; n < N_; n++) {
          for (k = 0; k < K_; k++) {
            bottom_diff_a[m*K_ + k] += nextDiff[(m*N_ + n)*K_ + k] * coeffs_[0];
          }
        }
      }
      break;
    case PairwiseParameter_PairwiseOp_MAX:
      mask = max_idx_.cpu_data();
      for (m = 0; m < M_; m++) {
        for (n = 0; n < N_; n++) {
          for (k = 0; k < K_; k++) {
            if (mask[(m*N_ + n)*K_ + k] == 0) {
              bottom_diff_a[m*K_ + k] += nextDiff[(m*N_ + n)*K_ + k];
            }
          }
        }
      }
      break;
    default:
      LOG(FATAL) << "Unknown pairwise operation.";
    }
  }
  if (propagate_down[1]) {
    switch (op_) {
    case PairwiseParameter_PairwiseOp_PROD:
      blas_sset(bottom[1]->count(), (Dtype)(0), bottom_diff_b);
      for (m = 0; m < M_; m++) {
        for (n = 0; n < N_; n++) {
          for (k = 0; k < K_; k++) {
            bottom_diff_b[n*K_ + k] += nextDiff[(m*N_ + n)*K_ + k] * bottom_data_a[m*K_ + k];
          }
        }
      }
      break;
    case PairwiseParameter_PairwiseOp_SUM:
      blas_sset(bottom[1]->count(), (Dtype)(0), bottom_diff_b);
      for (m = 0; m < M_; m++) {
        for (n = 0; n < N_; n++) {
          for (k = 0; k < K_; k++) {
            bottom_diff_b[n*K_ + k] += nextDiff[(m*N_ + n)*K_ + k] * coeffs_[1];
          }
        }
      }
      break;
    case PairwiseParameter_PairwiseOp_MAX:
      blas_sset(bottom[1]->count(), (Dtype)(0), bottom_diff_b);
      mask = max_idx_.cpu_data();
      for (m = 0; m < M_; m++) {
        for (n = 0; n < N_; n++) {
          for (k = 0; k < K_; k++) {
            if (mask[(m*N_ + n)*K_ + k] == 1) {
              bottom_diff_b[n*K_ + k] += nextDiff[(m*N_ + n)*K_ + k];
            }
          }
        }
      }
      break;
    default:
      LOG(FATAL) << "Unknown pairwise operation.";
    }
  }
}

#ifdef CPU_ONLY
STUB_GPU(PairwiseLayer);
#endif

INSTANTIATE_CLASS(PairwiseLayer);
REGISTER_LAYER_CLASS(Pairwise);


