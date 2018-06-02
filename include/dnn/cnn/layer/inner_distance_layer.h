
#include "inner_distance_layer.hpp"




/**
 * @brief Use L1 or L2 distance to replace the dot product in InnerProduct
 * layer.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */

class InnerDistanceLayer : public Layer {
 public:
  explicit InnerDistanceLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "InnerDistance"; }
  virtual inline int MinNumBottomBlobs() const { return 1; }
  virtual inline int MaxNumBottomBlobs() const { return 3; }
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

  int M_;
  int K_;
  int N_;
  bool transpose_;  ///< if true, assume transposed weights
  std::string distance_type_;
  bool normalize_;
  Blob weight_norm_;
  bool update_center_only_;
};




#define sign(x) (Dtype(0) < (x)) - ((x) < (Dtype)(0))


void InnerDistanceLayer::LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  const int num_output = this->layer_param_.inner_distance_param().num_output();
  transpose_ = this->layer_param_.inner_distance_param().transpose();
  distance_type_ = this->layer_param_.inner_distance_param().distance_type();
  normalize_ = this->layer_param_.inner_distance_param().normalize();
  update_center_only_ = this->layer_param_.inner_distance_param().update_center_only();
  
  const int axis = bottom[0]->CanonicalAxisIndex(
      this->layer_param_.inner_distance_param().axis());
  // Dimensions starting from "axis" are "flattened" into a single
  // length K_ vector. For example, if bottom[0]'s shape is (N, C, H, W),
  // and axis == 1, N inner products with dimension CHW are performed.
  K_ = bottom[0]->count(axis);
  if (layer->i_size == 1) N_ = num_output;
  else N_ = prev1->size.n;
  // Check if we need to set up the weights
  if (this->blobs_.size() > 0 || (!update_center_only_ && layer->i_size > 1)
      || (update_center_only_ && layer->i_size > 2)) {
    LOG(INFO) << "Skipping parameter initialization";
  } else {
    this->blobs_.resize(1);
    // Initialize the weights
    vector<int> weight_shape(2);
    if (transpose_) {
      weight_shape[0] = K_;
      weight_shape[1] = N_;
    } else {
      weight_shape[0] = N_;
      weight_shape[1] = K_;
    }
    this->blobs_[0].reset(new Blob(weight_shape));
    // fill the weights
    shared_ptr<Filler > weight_filler(GetFiller(
        this->layer_param_.inner_distance_param().weight_filler()));
    weight_filler->Fill(this->blobs_[0].get());
  }  // parameter initialization
  if(layer->i_size == 1) this->param_propagate_down_.resize(this->blobs_.size(), true);
}


void InnerDistanceLayer::Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  // Figure out the dimensions
  const int axis = bottom[0]->CanonicalAxisIndex(
      this->layer_param_.inner_distance_param().axis());
  const int new_K = bottom[0]->count(axis);
  CHECK_EQ(K_, new_K)
      << "Input size incompatible with inner product parameters.";
  // The first "axis" dimensions are independent inner products; the total
  // number of these is M_, the product over these dimensions.
  M_ = bottom[0]->count(0, axis);
  if (layer->i_size >= 2) N_ = prev1->size.n;
  // The top shape will be the bottom shape with the flattened axes dropped,
  // and replaced by a single axis with dimension num_output (N_).
  vector<int> top_shape = bottom[0]->shape();
  top_shape.resize(axis + 1);
  top_shape[axis] = N_;
  top[0]->Reshape(top_shape);
  if (layer->i_size == 1) {
    if (normalize_) {
      vector<int> weight_norm_shape(1, N_);
      weight_norm_.Reshape(weight_norm_shape);
      blas_sset(N_, (Dtype)(0), weight_norm_);
    }
  }
}


void InnerDistanceLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  Dtype* nextData = nextData;
  const Dtype* weight = layer->i_size >= 2 ? nextData : this->blobs_[0]->cpu_data();

  if (normalize_ && layer->i_size == 1) {
    Dtype* mutable_weight = this->blobs_[0]->mutable_data();
    Dtype sum_sq;
    for (n = 0; n < N_; n++) {
      sum_sq = blas_sdot(K_, weight + n*K_, weight + n*K_) + 1e-6;
      blas_sscal(K_, (Dtype)(1) / sqrt(sum_sq), mutable_weight + n*K_);
    }
  }

  if (distance_type_ == "L2") {//tanspose = false, TODO: transpose = true
    for (m = 0; m < M_; m++) {
      for (n = 0; n < N_; n++) {
        nextData[m * N_ + n] = (Dtype)(0);
        for (k = 0; k < K_; k++) {
          nextData[m * N_ + n] += (prevData[m * K_ + k] - weight[n * K_ + k]) * (prevData[m * K_ + k] - weight[n * K_ + k]);
        }
      }
    }
  }
  else if (distance_type_ == "L1") {
    for (m = 0; m < M_; m++) {
      for (n = 0; n < N_; n++) {
        nextData[m * N_ + n] = (Dtype)(0);
        for (k = 0; k < K_; k++) {
          nextData[m * N_ + n] += abs(prevData[m * K_ + k] - weight[n * K_ + k]);
        }
      }
    }
  }
  else {
    NOT_IMPLEMENTED;
  }
}


void InnerDistanceLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  const Dtype* nextDiff = nextDiff;
  const Dtype* prevData = prevData;
  const Dtype* weight = layer->i_size >= 2 ? nextData : this->blobs_[0]->cpu_data();
  if ((layer->i_size == 1 && this->param_propagate_down_[0]) ||
    (layer->i_size >= 2 && propagate_down[1])) {
    Dtype* weight_diff = layer->i_size >= 2 ? bottom[1]->mutable_diff() : this->blobs_[0]->mutable_diff();
    if (layer->i_size >= 2) {
      blas_sset(bottom[1]->count(), (Dtype)(0), weight_diff);
    }
    const Dtype* label_data = NULL;
    if (update_center_only_) {
      label_data = bottom[layer->i_size - 1]->cpu_data();
    }
    // Gradient with respect to weight
    if (distance_type_ == "L2") {
      for (n = 0; n < N_; n++) {
        for (k = 0; k < K_; k++) {
          for (m = 0; m < M_; m++) {
            if (update_center_only_ && n != (int)(label_data[m])) continue;
            weight_diff[n * K_ + k] += nextDiff[m * N_ + n] * (weight[n * K_ + k] - prevData[m * K_ + k]) * (Dtype)(2);
          }
        }
      }
    }
    else if (distance_type_ == "L1") {
      for (n = 0; n < N_; n++) {
        for (k = 0; k < K_; k++) {
          for (m = 0; m < M_; m++) {
            if (update_center_only_ && n != (int)(label_data[m])) continue;
            weight_diff[n * K_ + k] += nextDiff[m * N_ + n] * sign(weight[n * K_ + k] - prevData[m * K_ + k]);
          }
        }
      }
    }
    else {
      NOT_IMPLEMENTED;
    }
  }
  if (propagate_down[0]) {
    Dtype* prevDiff = bottom[0]->mutable_diff();
    blas_sset(M_ * K_, 0, prevDiff);
    if (distance_type_ == "L2") {
      for (m = 0; m < M_; m++) {
        for (k = 0; k < K_; k++) {
          for (n = 0; n < N_; n++) {
            prevDiff[m * K_ + k] += nextDiff[m * N_ + n] * (prevData[m * K_ + k] - weight[n * K_ + k]) * (Dtype)(2);
          }
        }
      }
    }
    else if (distance_type_ == "L1") {
      for (m = 0; m < M_; m++) {
        for (k = 0; k < K_; k++) {
          for (n = 0; n < N_; n++) {
            prevDiff[m * K_ + k] += nextDiff[m * N_ + n] * sign(prevData[m * K_ + k] - weight[n * K_ + k]);
          }
        }
      }
    }
    else {
      NOT_IMPLEMENTED;
    }
  }
}

#ifdef CPU_ONLY
STUB_GPU(InnerDistanceLayer);
#endif

INSTANTIATE_CLASS(InnerDistanceLayer);
REGISTER_LAYER_CLASS(InnerDistance);


