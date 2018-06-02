

class NCALossLayer : public LossLayer {
 public:
  explicit NCALossLayer(const LayerParameter& param)
      : LossLayer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
                          const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
                       const vector<Blob*>& top);
  virtual inline const char* type() const { return "NCALoss"; }
  virtual inline int ExactNumBottomBlobs() const { return -1; }
  virtual inline int ExactNumTopBlobs() const { return -1; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlobs() const { return 2; }

 protected:
  /// @copydoc NCALossLayer
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  bool min_negative_only_;
  Blob<int> min_negative_index_;
};


void NCALossLayer::LayerSetUp(const vector<Blob*>& bottom,
                                               const vector<Blob*>& top) {
  LossLayer::LayerSetUp(bottom, top);
  min_negative_only_ = this->layer_param_.nca_param().min_negative_only();
  CHECK_GE(layer->i_size, 2);
}


void NCALossLayer::Reshape(const vector<Blob*>& bottom,
                                                 const vector<Blob*>& top) {
  LossLayer::Reshape(bottom, top);
  if (layer->o_size == 2) {
    // positive distance, negative distance.
    top[1]->Reshape({ 2 });
  }
  if (min_negative_only_) {
    min_negative_index_.Reshape({ prev->size.n });
  }
}


void NCALossLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  const Dtype* label = nextData;
  int* min_negative_index_data = NULL;
  if (min_negative_only_) min_negative_index_data = min_negative_index_;
  int num = prev->size.n;
  int count = Data_count(prev->size);
  int dim = count / num;
  Dtype positive_distance = (Dtype)(0);
  Dtype negative_distance = (Dtype)(0);
  Dtype* loss = nextData;

  for (i = 0; i < num; ++i) {
    if (min_negative_only_) min_negative_index_data[i] = 0;
    for (j = 0; j < dim; ++j) {
      if (j == (int)(label[i])) {
        positive_distance += prevData[i * dim + j];
        loss[0] += prevData[i * dim + j];
      }
      else {
        negative_distance += prevData[i * dim + j];
        if(!min_negative_only_) loss[0] += exp(-prevData[i * dim + j]);
        if (min_negative_only_ && prevData[i * dim + j] < prevData[i*dim + min_negative_index_data[i]]) {
          min_negative_index_data[i] = j;
        }
      }
    }
    if (min_negative_only_) {
      loss[0] += exp(-prevData[i * dim + min_negative_index_data[i]]);
    }
  }
  
  loss[0] /= num;
  if (layer->o_size >= 2) {
    Dtype* distances = top[1]->mutable_data();
    distances[0] = positive_distance / num;
    distances[1] = negative_distance / num / (dim - 1);
  }
}


void NCALossLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  if (propagate_down[1]) {
    LOG(FATAL) << this->type()
               << " Layer cannot backpropagate to label inputs.";
  }
  if (propagate_down[0]) {
    const Dtype* prevData = prevData;
    Dtype* prevDiff = bottom[0]->mutable_diff();
    const Dtype* label = nextData;
    const int* min_negative_index_data = NULL;
    if (min_negative_only_) min_negative_index_data = min_negative_index_.cpu_data();
    int num = prev->size.n;
    int count = Data_count(prev->size);
    int dim = count / num;

    Dtype negative_sum = (Dtype)(0);

    for (i = 0; i < num; ++i) {
      if (min_negative_only_) {
        prevDiff[i * dim + (int)(label[i])] = 1;
        prevDiff[i * dim + min_negative_index_data[i]] = -1 * exp(-prevData[i * dim + min_negative_index_data[i]]);
      }
      else {
        for (j = 0; j < dim; ++j) {
          if (j == (int)(label[i])) {
            prevDiff[i * dim + j] = 1;
          }
          else {
            prevDiff[i * dim + j] = -1 * exp(-prevData[i * dim + j]);
          }
        }
      }
    }

    const Dtype loss_weight = nextDiff[0];
    //Dtype weighted_count = num * (abs(positive_weight_) + (dim - 1)*abs(negative_weight_));
    if (layer->i_size == 3) {
      Dtype weight_sum = (Dtype)(0);
      for (i = 0; i < num; ++i) {
        weight_sum += getData(prev2)[i];
      }
      weight_sum /= num;
      for (i = 0; i < num; ++i) {
        for (j = 0; j < dim; ++j) {
          prevDiff[i * dim + j] *= getData(prev2)[i] / weight_sum;
        }
      }
    }
    
    blas_sscal(count, loss_weight / num, prevDiff);
  }
}

INSTANTIATE_CLASS(NCALossLayer);
REGISTER_LAYER_CLASS(NCALoss);


