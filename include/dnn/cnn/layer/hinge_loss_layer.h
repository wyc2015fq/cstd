


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

class HingeLossLayer : public LossLayer {
 public:
  explicit HingeLossLayer(const LayerParameter& param)
      : LossLayer(param) {}

  virtual inline const char* type() const { return "HingeLoss"; }

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
};




void HingeLossLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  Dtype* prevDiff = bottom[0]->mutable_diff();
  const Dtype* label = nextData;
  int num = prev->size.n;
  int count = Data_count(prev->size);
  int dim = count / num;

  blas_scopy(count, prevData, prevDiff);
  for (i = 0; i < num; ++i) {
    prevDiff[i * dim + (int)(label[i])] *= -1;
  }
  for (i = 0; i < num; ++i) {
    for (j = 0; j < dim; ++j) {
      prevDiff[i * dim + j] = MAX(
        (Dtype)(0), 1 + prevDiff[i * dim + j]);
    }
  }
  Dtype* loss = nextData;
  switch (this->layer_param_.hinge_loss_param().norm()) {
  case HingeLossParameter_Norm_L1:
    loss[0] = blas_sasum(count, prevDiff) / num;
    break;
  case HingeLossParameter_Norm_L2:
    loss[0] = blas_sdot(count, prevDiff, prevDiff) / num;
    break;
  default:
    LOG(FATAL) << "Unknown Norm";
  }
}


void HingeLossLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  if (propagate_down[1]) {
    LOG(FATAL) << this->type()
               << " Layer cannot backpropagate to label inputs.";
  }
  if (propagate_down[0]) {
    Dtype* prevDiff = bottom[0]->mutable_diff();
    const Dtype* label = nextData;
    int num = prev->size.n;
    int count = Data_count(prev->size);
    int dim = count / num;

    for (i = 0; i < num; ++i) {
      prevDiff[i * dim + (int)(label[i])] *= -1;
    }

    const Dtype loss_weight = nextDiff[0];
    switch (this->layer_param_.hinge_loss_param().norm()) {
    case HingeLossParameter_Norm_L1:
      blas_ssign(count, prevDiff, prevDiff);
      blas_sscal(count, loss_weight / num, prevDiff);
      break;
    case HingeLossParameter_Norm_L2:
      blas_sscal(count, loss_weight * 2 / num, prevDiff);
      break;
    default:
      LOG(FATAL) << "Unknown Norm";
    }
  }
}

INSTANTIATE_CLASS(HingeLossLayer);
REGISTER_LAYER_CLASS(HingeLoss);


