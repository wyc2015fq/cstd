

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

class SoftContrastiveLossLayer : public LossLayer {
 public:
  explicit SoftContrastiveLossLayer(const LayerParameter& param)
      : LossLayer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
                          const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
                       const vector<Blob*>& top);
  virtual inline const char* type() const { return "SoftContrastiveLoss"; }
  virtual inline int ExactNumBottomBlobs() const { return -1; }
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

  Dtype positive_margin_;
  Dtype negative_margin_;
  Dtype positive_weight_;
  Dtype negative_weight_;
  Dtype exponent_scale_;
  Dtype positive_outlier_thresh_;
  Blob sum_exp_;
  bool square_;
};




void SoftContrastiveLossLayer::LayerSetUp(const vector<Blob*>& bottom,
                                               const vector<Blob*>& top) {
  LossLayer::LayerSetUp(bottom, top);
  CHECK_GE(layer->i_size, 2);
  positive_margin_ = this->layer_param_.soft_contrastive_loss_param().positive_margin();
  negative_margin_ = this->layer_param_.soft_contrastive_loss_param().negative_margin();
  positive_weight_ = this->layer_param_.soft_contrastive_loss_param().positive_weight();
  negative_weight_ = this->layer_param_.soft_contrastive_loss_param().negative_weight();
  positive_outlier_thresh_ = this->layer_param_.general_contrastive_loss_param().positive_outlier_thresh();
  exponent_scale_ = this->layer_param_.soft_contrastive_loss_param().negative_weight();
  square_ = this->layer_param_.soft_contrastive_loss_param().square();
}


void SoftContrastiveLossLayer::Reshape(const vector<Blob*>& bottom,
                                                 const vector<Blob*>& top) {
  LossLayer::Reshape(bottom, top);
  if (layer->o_size >= 2) {
    // positive distance, negative distance.
    top[1]->Reshape({ 2 });
  }
  sum_exp_.Reshape({ prev->size.n, 1 });
}


void SoftContrastiveLossLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  Dtype* prevDiff = bottom[0]->mutable_diff();
  const Dtype* label = nextData;
  Dtype* sum_exp_data = sum_exp_;
  int num = prev->size.n;
  int count = Data_count(prev->size);
  int dim = count / num;
  Dtype weighted_count = num * (abs(positive_weight_) + (dim - 1)*abs(negative_weight_));
  Dtype positive_distance = (Dtype)(0);
  Dtype negative_distance = (Dtype)(0);
  Dtype* loss = nextData;

  for (i = 0; i < num; ++i) {
    sum_exp_data[i] = (Dtype)(0);
    for (j = 0; j < dim; ++j) {
      if (j != (int)(label[i])) {
        if (prevData[i * dim + j] < negative_margin_) {
          if (square_) {
            prevDiff[i * dim + j] = (negative_margin_ - prevData[i * dim + j]) * (negative_margin_ - prevData[i * dim + j]) * negative_weight_;
            sum_exp_data[i] += exp((negative_margin_ - prevData[i * dim + j]) * (negative_margin_ - prevData[i * dim + j]) * exponent_scale_);
            negative_distance += negative_margin_ - prevData[i * dim + j];
          }
          else {
            prevDiff[i * dim + j] = (negative_margin_ - prevData[i * dim + j]) * negative_weight_;//exp(negative_margin_ - prevData[i * dim + j]);
            sum_exp_data[i] += exp((negative_margin_ - prevData[i * dim + j]) * exponent_scale_);
            negative_distance += negative_margin_ - prevData[i * dim + j];
          }
        }
        else {
          prevDiff[i * dim + j] = 0;
        }
      }
      else {
        if (square_) {
          Dtype truncated_value = std::min(positive_outlier_thresh_, MAX(Dtype(0), prevData[i * dim + j] - positive_margin_));
          positive_distance += truncated_value;
          truncated_value *= truncated_value;
          prevDiff[i * dim + j] = truncated_value * positive_weight_;
          loss[0] += prevDiff[i * dim + j];
        }
        else {
          Dtype truncated_value = std::min(positive_outlier_thresh_, MAX(Dtype(0), prevData[i * dim + j] - positive_margin_));
          prevDiff[i * dim + j] = truncated_value * positive_weight_;
          positive_distance += truncated_value;
          loss[0] += prevDiff[i * dim + j];
        }
      }
    }
    if(sum_exp_data[i] > 0) loss[0] += log(sum_exp_data[i]);
  }
  
  loss[0] /= num;
  if (layer->o_size >= 2) {
    Dtype* distances = top[1]->mutable_data();
    distances[0] = positive_distance / num;
    distances[1] = negative_distance / num / (dim - 1);
  }
}


void SoftContrastiveLossLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  if (propagate_down[1]) {
    LOG(FATAL) << this->type()
               << " Layer cannot backpropagate to label inputs.";
  }
  if (propagate_down[0]) {
    const Dtype* prevData = prevData;
    Dtype* prevDiff = bottom[0]->mutable_diff();
    const Dtype* label = nextData;
    const Dtype* sum_exp_data = sum_exp_.cpu_data();
    int num = prev->size.n;
    int count = Data_count(prev->size);
    int dim = count / num;

    Dtype negative_sum = (Dtype)(0);

    for (i = 0; i < num; ++i) {
      Dtype min_negative_distance = FLT_MAX;
      int min_negative_index = 0;
      for (j = 0; j < dim; ++j) {
        if (j == (int)(label[i])) {
          if (prevData[i * dim + j] > positive_margin_ && prevData[i * dim + j] < positive_outlier_thresh_) {
            if (square_) {
              prevDiff[i * dim + j] = prevData[i * dim + j] * positive_weight_;
            }
            else {
              prevDiff[i * dim + j] = positive_weight_;
            }
          }
          else {
            prevDiff[i * dim + j] = 0;
          }
        }
        else {
          if (prevData[i * dim + j] < negative_margin_) {
            if (square_) {
              prevDiff[i * dim + j] = (prevData[i * dim + j] - negative_margin_) * negative_weight_ *
                exp((negative_margin_ - prevData[i * dim + j]) * (negative_margin_ - prevData[i * dim + j]) * exponent_scale_) / sum_exp_data[i];
            }
            else {
              prevDiff[i * dim + j] = -negative_weight_ * exp((negative_margin_ - prevData[i * dim + j]) * exponent_scale_) / sum_exp_data[i];
            }
          }
          else {
            prevDiff[i * dim + j] = 0;
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

INSTANTIATE_CLASS(SoftContrastiveLossLayer);
REGISTER_LAYER_CLASS(SoftContrastiveLoss);


