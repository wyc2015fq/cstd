



/**
 * @brief Computes the multinomial logistic loss for a one-of-many
 *        classification task, directly taking a predicted probability
 *        distribution as input.
 *
 * When predictions are not already a probability distribution, you should
 * instead use the SoftmaxWithLossLayer, which maps predictions to a
 * distribution using the SoftmaxLayer, before computing the multinomial
 * logistic loss. The SoftmaxWithLossLayer should be preferred over separate
 * SoftmaxLayer + ConfidencePenaltyLossLayer
 * as its gradient computation is more numerically stable.
 *
 * @param bottom input Blob vector (length 2)
 *   -# @f$ (N \times C \times H \times W) @f$
 *      the predictions @f$ \hat{p} @f$, a Blob with values in
 *      @f$ [0, 1] @f$ indicating the predicted probability of each of the
 *      @f$ K = CHW @f$ classes.  Each prediction vector @f$ \hat{p}_n @f$
 *      should sum to 1 as in a probability distribution: @f$
 *      \forall n \sum\limits_{k=1}^K \hat{p}_{nk} = 1 @f$.
 *   -# @f$ (N \times 1 \times 1 \times 1) @f$
 *      the labels @f$ l @f$, an integer-valued Blob with values
 *      @f$ l_n \in [0, 1, 2, ..., K - 1] @f$
 *      indicating the correct class label among the @f$ K @f$ classes
 * @param top output Blob vector (length 1)
 *   -# @f$ (1 \times 1 \times 1 \times 1) @f$
 *      the computed multinomial logistic loss: @f$ E =
 *        \frac{-1}{N} \sum\limits_{n=1}^N \log(\hat{p}_{n,l_n})
 *      @f$
 */

class ConfidencePenaltyLossLayer : public LossLayer {
 public:
  explicit ConfidencePenaltyLossLayer(const LayerParameter& param)
      : LossLayer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
                          const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "ConfidencePenaltyLoss"; }

 protected:
  /// @copydoc ConfidencePenaltyLossLayer
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  /**
   * @brief Computes the multinomial logistic loss error gradient w.r.t. the
   *        predictions.
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
   *      the predictions @f$ \hat{p} @f$; Backward computes diff
   *      @f$ \frac{\partial E}{\partial \hat{p}} @f$
   *   -# @f$ (N \times 1 \times 1 \times 1) @f$
   *      the labels -- ignored as we can't compute their error gradients
   */
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  Dtype beta_;
};




  
  void ConfidencePenaltyLossLayer::LayerSetUp(const vector<Blob*>& bottom,
                                                const vector<Blob*>& top) {
    LossLayer::LayerSetUp(bottom, top);
    beta_ = this->layer_param_.confidence_penalty_loss_param().beta();
  }

  
  void ConfidencePenaltyLossLayer::Reshape(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
    LossLayer::Reshape(bottom, top);
    CHECK_EQ(prev1->size.c, 1);
    CHECK_EQ(prev1->size.h, 1);
    CHECK_EQ(prev1->size.w, 1);
  }

  
  void ConfidencePenaltyLossLayer::Forward(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
    const Dtype* prevData = prevData;
    const Dtype* bottom_label = nextData;
    int num = prev->size.n;
    int dim = Data_count(prev->size) / prev->size.n;
    Dtype loss = 0;
    for (i = 0; i < num; ++i) {
      for (j = 0; j < dim; ++j) {
        Dtype prob = MAX(
          prevData[i * dim + j], (Dtype)(kLOG_THRESHOLD));
        if (j == (int)(bottom_label[i])) {
          loss -= (Dtype(1.0) - beta_) * log(prob);
        }
        else {
          loss -= (beta_ / (num - (Dtype)(1.0))) * log(prob);
        }
      }
    }
    nextData[0] = loss / num;
  }

  
  void ConfidencePenaltyLossLayer::Backward(
    const vector<Blob*>& top, const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
    if (propagate_down[1]) {
      LOG(FATAL) << this->type()
        << " Layer cannot backpropagate to label inputs.";
    }
    if (propagate_down[0]) {
      const Dtype* prevData = prevData;
      const Dtype* bottom_label = nextData;
      Dtype* prevDiff = bottom[0]->mutable_diff();
      int num = prev->size.n;
      int dim = Data_count(prev->size) / prev->size.n;
      blas_sset(Data_count(prev->size), (Dtype)(0), prevDiff);
      const Dtype scale = -nextDiff[0] / num;
      for (i = 0; i < num; ++i) {
        for (j = 0; j < dim; ++j) {
          Dtype prob = MAX(prevData[i * dim + j], (Dtype)(kLOG_THRESHOLD));
          if (j == (int)(bottom_label[i])) {
            prevDiff[i * dim + j] = scale * (Dtype(1.0) - beta_) / prob;
          }
          else {
            prevDiff[i * dim + j] = scale * (beta_ / (num - (Dtype)(1.0))) / prob;
          }
        }
      }
    }
  }

  INSTANTIATE_CLASS(ConfidencePenaltyLossLayer);
  REGISTER_LAYER_CLASS(ConfidencePenaltyLoss);


