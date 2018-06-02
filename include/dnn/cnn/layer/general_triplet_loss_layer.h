

/**
 * @brief Computes the hinge loss for a one-of-many classification task.
 *
 * @param bottom input Blob vector (length 2)
 *   -# @f$ (N \times C \times H \times W) @f$
 *      the predictions @f$ t @f$, a Blob with values in
 *      @f$ [-\infty, +\infty] @f$ indicating the predicted score for each of
 *      the @f$ K = CHW @f$ classes. In an SVM, @f$ t @f$ is the result of
 *      taking the inner product @f$ X^T W @f$ of the D-dimensional features
 *      @f$ X \in \mathcal{R}^{D \times N} @f$ and the learned hyperplane
 *      parameters @f$ W \in \mathcal{R}^{D \times K} @f$, so a Net with just
 *      an InnerProductLayer (with num_output = D) providing predictions to a
 *      HingeLossLayer and no other learnable parameters or losses is
 *      equivalent to an SVM.
 *   -# @f$ (N \times 1 \times 1 \times 1) @f$
 *      the labels @f$ l @f$, an integer-valued Blob with values
 *      @f$ l_n \in [0, 1, 2, ..., K - 1] @f$
 *      indicating the correct class label among the @f$ K @f$ classes
 * @param top output Blob vector (length 1)
 *   -# @f$ (1 \times 1 \times 1 \times 1) @f$
 *      the computed hinge loss: @f$ E =
 *        \frac{1}{N} \sum\limits_{n=1}^N \sum\limits_{k=1}^K
 *        [\max(0, 1 - \delta\{l_n = k\} t_{nk})] ^ p
 *      @f$, for the @f$ L^p @f$ norm
 *      (defaults to @f$ p = 1 @f$, the L1 norm; L2 norm, as in L2-SVM,
 *      is also available), and @f$
 *      \delta\{\mathrm{condition}\} = \left\{
 *         \begin{array}{lr}
 *            1 & \mbox{if condition} \\
 *           -1 & \mbox{otherwise}
 *         \end{array} \right.
 *      @f$
 *
 * In an SVM, @f$ t \in \mathcal{R}^{N \times K} @f$ is the result of taking
 * the inner product @f$ X^T W @f$ of the features
 * @f$ X \in \mathcal{R}^{D \times N} @f$
 * and the learned hyperplane parameters
 * @f$ W \in \mathcal{R}^{D \times K} @f$. So, a Net with just an
 * InnerProductLayer (with num_output = @f$k@f$) providing predictions to a
 * HingeLossLayer is equivalent to an SVM (assuming it has no other learned
 * outside the InnerProductLayer and no other losses outside the
 * HingeLossLayer).
 */

class GeneralTripletLossLayer : public LossLayer {
 public:
  explicit GeneralTripletLossLayer(const LayerParameter& param)
      : LossLayer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
                          const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
                       const vector<Blob*>& top);
  virtual inline const char* type() const { return "GeneralTripletLoss"; }
  virtual inline int ExactNumTopBlobs() const { return -1; }
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlobs() const { return 2; }

 protected:
  /// @copydoc HingeLossLayer
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  /**
   * @brief Computes the hinge loss error gradient w.r.t. the predictions.
   *
   * Gradients cannot be computed with respect to the label inputs (bottom[1]),
   * so this method ignores bottom[1] and requires !propagate_down[1], crashing
   * if propagate_down[1] is set.
   *
   * @param top output Blob vector (length 1), providing the error gradient with
   *      respect to the outputs
   *   -# @f$ (1 \times 1 \times 1 \times 1) @f$
   *      This Blob's diff will simply contain the loss_weight* @f$ \lambda @f$,
   *      as @f$ \lambda @f$ is the coefficient of this layer's output
   *      @f$\ell_i@f$ in the overall Net loss
   *      @f$ E = \lambda_i \ell_i + \mbox{other loss terms}@f$; hence
   *      @f$ \frac{\partial E}{\partial \ell_i} = \lambda_i @f$.
   *      (*Assuming that this top Blob is not used as a bottom (input) by any
   *      other layer of the Net.)
   * @param propagate_down see Layer::Backward.
   *      propagate_down[1] must be false as we can't compute gradients with
   *      respect to the labels.
   * @param bottom input Blob vector (length 2)
   *   -# @f$ (N \times C \times H \times W) @f$
   *      the predictions @f$t@f$; Backward computes diff
   *      @f$ \frac{\partial E}{\partial t} @f$
   *   -# @f$ (N \times 1 \times 1 \times 1) @f$
   *      the labels -- ignored as we can't compute their error gradients
   */
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  Dtype margin_;
  bool add_center_loss_;
  bool hardest_only_;
  bool positive_first_;
  Dtype positive_upper_bound_;
  Dtype positive_weight_;
  Dtype negative_weight_;
  Blob<int> hardest_index_;
};





void GeneralTripletLossLayer::LayerSetUp(const vector<Blob*>& bottom,
                                               const vector<Blob*>& top) {
  LossLayer::LayerSetUp(bottom, top);
  CHECK_GE(layer->i_size, 2);
  margin_ = this->layer_param_.general_triplet_loss_param().margin();
  add_center_loss_ = this->layer_param_.general_triplet_loss_param().add_center_loss();
  hardest_only_ = this->layer_param_.general_triplet_loss_param().hardest_only();
  positive_weight_ = this->layer_param_.general_triplet_loss_param().positive_weight();
  negative_weight_ = this->layer_param_.general_triplet_loss_param().negative_weight();
  positive_first_ = this->layer_param_.general_triplet_loss_param().positive_first();
  positive_upper_bound_ = this->layer_param_.general_triplet_loss_param().positive_upper_bound();
}


void GeneralTripletLossLayer::Reshape(const vector<Blob*>& bottom,
                                                 const vector<Blob*>& top) {
  LossLayer::Reshape(bottom, top);
  if (layer->o_size >= 2) {
    // positive distance, negative distance.
    top[1]->Reshape({ 2 });
  }
  if (hardest_only_) {
    hardest_index_.Reshape({ prev->size.n });
  }
}


void GeneralTripletLossLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  Dtype* prevDiff = bottom[0]->mutable_diff();
  const Dtype* label = nextData;
  int* hardest_index_data = NULL;
  if (hardest_only_) hardest_index_data = hardest_index_;
  int num = prev->size.n;
  int count = Data_count(prev->size);
  int dim = count / num;
  Dtype positive_distance = (Dtype)(0);
  Dtype negative_distance = (Dtype)(0);

  for (i = 0; i < num; ++i) {
    Dtype same_distance = prevData[i * dim + (int)(label[i])];
    positive_distance += same_distance;
    if (hardest_only_) hardest_index_data[i] = -1;
    for (j = 0; j < dim; ++j) {
      if (j != (int)(label[i])) {
        prevDiff[i * dim + j] = MAX(
          (Dtype)(0), same_distance + margin_ - prevData[i * dim + j]) * negative_weight_;
        negative_distance += prevData[i * dim + j];
        if (hardest_only_ && prevDiff[i * dim + j] > 0 &&
          (hardest_index_data[i] < 0 || prevDiff[i * dim + j] > prevDiff[i * dim + hardest_index_data[i]])) {
          hardest_index_data[i] = j;
        }
      }
      else {
        prevDiff[i * dim + j] = (Dtype)(0);
      }
    }
    if (hardest_only_ && hardest_index_data[i] >= 0) {
      for (j = 0; j < dim; ++j) {
        if (j != (int)(label[i]) && j != hardest_index_data[i]) {
          prevDiff[i * dim + j] = (Dtype)(0);
        }
      }
    }
    if (positive_first_ && same_distance > positive_upper_bound_) {
      for (j = 0; j < dim; ++j) {
        if (j != (int)(label[i])) {
          prevDiff[i * dim + j] = (Dtype)(0);
        }
      }
    }
  }
  Dtype* loss = nextData;
  loss[0] = blas_sasum(count, prevDiff) / num;
  if (layer->o_size >= 2) {
    Dtype* distances = top[1]->mutable_data();
    distances[0] = positive_distance / num;
    distances[1] = negative_distance / num / (dim - 1);
  }
}


void GeneralTripletLossLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  if (propagate_down[1]) {
    LOG(FATAL) << this->type()
               << " Layer cannot backpropagate to label inputs.";
  }
  if (propagate_down[0]) {
    const Dtype* prevData = prevData;
    Dtype* prevDiff = bottom[0]->mutable_diff();
    const Dtype* label = nextData;
    int* hardest_index_data = NULL;
    if (hardest_only_) hardest_index_data = hardest_index_;
    int num = prev->size.n;
    int count = Data_count(prev->size);
    int dim = count / num;  

    for (i = 0; i < num; ++i) {
      Dtype same_distance = prevData[i * dim + (int)(label[i])];
      if (positive_first_ && same_distance > positive_upper_bound_) {
        prevDiff[i * dim + (int)(label[i])] = positive_weight_;
        for (j = 0; j < dim; ++j) {
          if (j != (int)(label[i])) {
            prevDiff[i * dim + j] = (Dtype)(0);
          }
        }
        continue;
      }
      int negative_sum = 0;
      if (hardest_only_) {
        if (hardest_index_data[i] >= 0 && prevData[i * dim + hardest_index_data[i]]) {
          prevDiff[i * dim + hardest_index_data[i]] = -negative_weight_;
          negative_sum += 1;
        }
      }
      else {
        for (j = 0; j < dim; ++j) {
          if (j != (int)(label[i])) {
            if (same_distance + margin_ > prevData[i * dim + j]) {
              prevDiff[i * dim + j] = -negative_weight_;
              negative_sum += 1;
            }
            else {
              prevDiff[i * dim + j] = (Dtype)(0);
            }
          }
        }
      }
      
      if (add_center_loss_ || negative_sum > 0) {
        prevDiff[i * dim + (int)(label[i])] = positive_weight_;
      }
    } 

    const Dtype loss_weight = nextDiff[0];
    //Dtype weighted_count = num * (abs(positive_weight_) + (dim - 1)*abs(negative_weight_));
    if (layer->i_size == 3) {
      for (i = 0; i < num; ++i) {
        for (j = 0; j < dim; ++j) {
          prevDiff[i * dim + j] *= getData(prev2)[i];
        }
      }
    }
    blas_sscal(count, loss_weight / num, prevDiff);
  }
}

INSTANTIATE_CLASS(GeneralTripletLossLayer);
REGISTER_LAYER_CLASS(GeneralTripletLoss);


