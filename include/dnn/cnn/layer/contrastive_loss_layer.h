

/**
 * @brief Computes the contrastive loss @f$
 *          E = \frac{1}{2N} \sum\limits_{n=1}^N \left(y\right) d^2 +
 *              \left(1-y\right) \max \left(margin-d, 0\right)^2
 *          @f$ where @f$
 *          d = \left| \left| a_n - b_n \right| \right|_2 @f$. This can be
 *          used to train siamese networks.
 *
 * @param bottom input Blob vector (length 3)
 *   -# @f$ (N \times C \times 1 \times 1) @f$
 *      the features @f$ a \in [-\infty, +\infty]@f$
 *   -# @f$ (N \times C \times 1 \times 1) @f$
 *      the features @f$ b \in [-\infty, +\infty]@f$
 *   -# @f$ (N \times 1 \times 1 \times 1) @f$
 *      the binary similarity @f$ s \in [0, 1]@f$
 * @param top output Blob vector (length 1)
 *   -# @f$ (1 \times 1 \times 1 \times 1) @f$
 *      the computed contrastive loss: @f$ E =
 *          \frac{1}{2N} \sum\limits_{n=1}^N \left(y\right) d^2 +
 *          \left(1-y\right) \max \left(margin-d, 0\right)^2
 *          @f$ where @f$
 *          d = \left| \left| a_n - b_n \right| \right|_2 @f$.
 * This can be used to train siamese networks.
 */

class ContrastiveLossLayer : public LossLayer {
 public:
  explicit ContrastiveLossLayer(const LayerParameter& param)
      : LossLayer(param), diff_() {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline int ExactNumBottomBlobs() const { return 4; }
  virtual inline const char* type() const { return "ContrastiveLoss"; }
  /**
   * Unlike most loss layers, in the ContrastiveLossLayer we can backpropagate
   * to the first two inputs.
   */
  virtual inline bool AllowForceBackward(const int bottom_index) const {
    return bottom_index != 2;
  }

 protected:
  /// @copydoc ContrastiveLossLayer
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  /**
   * @brief Computes the Contrastive error gradient w.r.t. the inputs.
   *
   * Computes the gradients with respect to the two input vectors (bottom[0] and
   * bottom[1]), but not the similarity label (bottom[2]).
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
   * @param bottom input Blob vector (length 2)
   *   -# @f$ (N \times C \times 1 \times 1) @f$
   *      the features @f$a@f$; Backward fills their diff with
   *      gradients if propagate_down[0]
   *   -# @f$ (N \times C \times 1 \times 1) @f$
   *      the features @f$b@f$; Backward fills their diff with gradients if
   *      propagate_down[1]
   */
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  Blob diff_;  // cached for backward pass
  Blob dist_sq_;  // cached for backward pass
  Blob diff_sq_;  // tmp storage for gpu forward pass
  Blob summer_vec_;  // tmp storage for gpu forward pass
};





void ContrastiveLossLayer::LayerSetUp(
  const vector<Blob*>& bottom, const vector<Blob*>& top) {
  LossLayer::LayerSetUp(bottom, top);
  CHECK_EQ(prev->size.c, prev1->size.c);
  CHECK_EQ(prev->size.h, 1);
  CHECK_EQ(prev->size.w, 1);
  CHECK_EQ(prev1->size.h, 1);
  CHECK_EQ(prev1->size.w, 1);
  CHECK_EQ(prev2->size.c, 1);
  CHECK_EQ(prev2->size.h, 1);
  CHECK_EQ(prev2->size.w, 1);
  diff_.Reshape(prev->size.n, prev->size.c, 1, 1);
  diff_sq_.Reshape(prev->size.n, prev->size.c, 1, 1);
  dist_sq_.Reshape(prev->size.n, 1, 1, 1);
  // vector of ones used to sum along channels
  summer_vec_.Reshape(prev->size.c, 1, 1, 1);
  for (i = 0; i < prev->size.c; ++i)
    summer_vec_[i] = (Dtype)(1);
}


void ContrastiveLossLayer::Forward(
    const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  int count = Data_count(prev->size);
  blas_ssub(
      count,
      prevData,  // a
      nextData,  // b
      diff_);  // a_i-b_i
  const int channels = prev->size.c;
  Dtype margin = this->layer_param_.contrastive_loss_param().margin();
  bool legacy_version =
      this->layer_param_.contrastive_loss_param().legacy_version();
  Dtype loss(0.0);
  for (i = 0; i < prev->size.n; ++i) {
    dist_sq_[i] = blas_sdot(channels,
        diff_.cpu_data() + (i*channels), diff_.cpu_data() + (i*channels));
    if ((int)(getData(prev2)[i]) == (int)(bottom[3]->cpu_data()[i])) {  // similar pairs
      loss += dist_sq_.cpu_data()[i];
    } else {  // dissimilar pairs
      if (legacy_version) {
        loss += MAX(margin - dist_sq_.cpu_data()[i], (Dtype)(0.0));
      } else {
        Dtype dist = MAX(margin - sqrt(dist_sq_.cpu_data()[i]),
          (Dtype)(0.0));
        loss += dist*dist;
      }
    }
  }
  loss = loss / static_cast(prev->size.n) / (Dtype)(2);
  nextData[0] = loss;
}


void ContrastiveLossLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  Dtype margin = this->layer_param_.contrastive_loss_param().margin();
  bool legacy_version =
      this->layer_param_.contrastive_loss_param().legacy_version();
  for (i = 0; i < 2; ++i) {
    if (propagate_down[i]) {
      const Dtype sign = (i == 0) ? 1 : -1;
      const Dtype alpha = sign * nextDiff[0] /
          static_cast(IBLOB(i)->size.n);
      int num = IBLOB(i)->size.n;
      int channels = IBLOB(i)->size.c;
      for (j = 0; j < num; ++j) {
        Dtype* bout = IBLOB(i)->mutable_diff();
        if ((int)(getData(prev2)[j]) == (int)(bottom[3]->cpu_data()[j])) {  // similar pairs
          blas_saxpby(
              channels,
              alpha,
              diff_.cpu_data() + (j*channels),
              (Dtype)(0.0),
              bout + (j*channels));
        } else {  // dissimilar pairs
          Dtype mdist(0.0);
          Dtype beta(0.0);
          if (legacy_version) {
            mdist = margin - dist_sq_.cpu_data()[j];
            beta = -alpha;
          } else {
            Dtype dist = sqrt(dist_sq_.cpu_data()[j]);
            mdist = margin - dist;
            beta = -alpha * mdist / (dist + (Dtype)(1e-4));
          }
          if (mdist > (Dtype)(0.0)) {
            blas_saxpby(
                channels,
                beta,
                diff_.cpu_data() + (j*channels),
                (Dtype)(0.0),
                bout + (j*channels));
          } else {
            blas_sset(channels, (Dtype)(0), bout + (j*channels));
          }
        }
      }
    }
  }
}

#ifdef CPU_ONLY
STUB_GPU(ContrastiveLossLayer);
#endif

INSTANTIATE_CLASS(ContrastiveLossLayer);
REGISTER_LAYER_CLASS(ContrastiveLoss);


