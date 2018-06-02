

  /**
  * @brief Computes the classification accuracy for a one-of-many
  *        classification task.
  */
  
  class FCNAccuracyLayer : public Layer {
  public:
    /**
    * @param param provides AccuracyParameter accuracy_param,
    *     with AccuracyLayer options:
    *   - top_k (\b optional, default 1).
    *     Sets the maximum rank @f$ k @f$ at which a prediction is considered
    *     correct.  For example, if @f$ k = 5 @f$, a prediction is counted
    *     correct if the correct label is among the top 5 predicted labels.
    */
    explicit FCNAccuracyLayer(const LayerParameter& param)
      : Layer(param) {}
    virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
    virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

    virtual inline const char* type() const { return "FCNAccuracy"; }
    virtual inline int ExactNumBottomBlobs() const { return -1; }
    virtual inline int MinBottomBlobs() const { return 2; }
    virtual inline int MaxBottomBlobs() const { return 3; }

    // If there are two top blobs, then the second blob will contain
    // accuracies per class.
    virtual inline int MinTopBlobs() const { return 1; }
    virtual inline int MaxTopBlos() const { return 2; }

  protected:
    /**
    * @param bottom input Blob vector (length 2)
    *   -# @f$ (N \times C \times H \times W) @f$
    *      the predictions @f$ x @f$, a Blob with values in
    *      @f$ [-\infty, +\infty] @f$ indicating the predicted score for each of
    *      the @f$ K = CHW @f$ classes. Each @f$ x_n @f$ is mapped to a predicted
    *      label @f$ \hat{l}_n @f$ given by its maximal index:
    *      @f$ \hat{l}_n = \arg\max\limits_k x_{nk} @f$
    *   -# @f$ (N \times 1 \times 1 \times 1) @f$
    *      the labels @f$ l @f$, an integer-valued Blob with values
    *      @f$ l_n \in [0, 1, 2, ..., K - 1] @f$
    *      indicating the correct class label among the @f$ K @f$ classes
    * @param top output Blob vector (length 1)
    *   -# @f$ (1 \times 1 \times 1 \times 1) @f$
    *      the computed accuracy: @f$
    *        \frac{1}{N} \sum\limits_{n=1}^N \delta\{ \hat{l}_n = l_n \}
    *      @f$, where @f$
    *      \delta\{\mathrm{condition}\} = \left\{
    *         \begin{array}{lr}
    *            1 & \mbox{if condition} \\
    *            0 & \mbox{otherwise}
    *         \end{array} \right.
    *      @f$
    */
    virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);


    /// @brief Not implemented -- AccuracyLayer cannot be used as a loss.
    virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
      for (i = 0; i < propagate_down.size(); ++i) {
        if (propagate_down[i]) { NOT_IMPLEMENTED; }
      }
    }

    int label_axis_, outer_num_, inner_num_;

    int top_k_;

    /// Keeps counts of the number of samples per class.
    Blob nums_buffer_;
  };




  
  void FCNAccuracyLayer::LayerSetUp(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
    top_k_ = this->layer_param_.accuracy_param().top_k();
  }

  
  void FCNAccuracyLayer::Reshape(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
    CHECK_EQ(Data_count(prev->size), bottom[1]->count());
    if (layer->i_size == 3) {
      CHECK_EQ(bottom[1]->count(), bottom[2]->count())
        << "Number of loss weights must match number of label.";
    }
    vector<int> top_shape(0);  // Accuracy is a scalar; 0 axes.
    top[0]->Reshape(top_shape);
    if (layer->o_size > 1) {
      // Per-class accuracy is a vector; 1 axes.
      vector<int> top_shape_per_class(1);
      top_shape_per_class[0] = bottom[0]->shape(label_axis_);
      top[1]->Reshape(top_shape_per_class);
      nums_buffer_.Reshape(top_shape_per_class);
    }
  }

  
  void FCNAccuracyLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
    Dtype accuracy = 0;
    const Dtype* prevData = prevData;
    const Dtype* bottom_label = nextData;
    int count = 0;
    Dtype max_val;
    int max_n = 0, max_c = 0, max_h = 0, max_w = 0;
    for (n = 0; n < prev->size.n; n++) {
      if (n % top_k_ == 0) {
        max_val = -std::numeric_limits::max();
        max_h = 0;
        max_w = 0;
        max_n = 0;
      }
      for (c = 0; c < prev->size.c; c++) {
        for (h = 0; h < prev->size.h; h++) {
          for (w = 0; w < prev->size.w; w++) {
            if (prevData[bottom[0]->offset(n, c, h, w)] > max_val) {
              max_val = prevData[bottom[0]->offset(n, c, h, w)];
              max_h = h;
              max_w = w;
              max_c = c;
              max_n = n % top_k_;
            }
          }
        }
      }
      if (n % top_k_ == top_k_ - 1) {
        if (max_n == (top_k_ - 1) / 2 && bottom_label[bottom[1]->offset(max_n, max_c, max_h, max_w)] > 0) accuracy += (Dtype)1.0;
        count++;
      }
    }

    // LOG(INFO) << "Accuracy: " << accuracy;
    nextData[0] = accuracy / count;
    // Accuracy layer should not be used as a loss function.
  }

  INSTANTIATE_CLASS(FCNAccuracyLayer);
  REGISTER_LAYER_CLASS(FCNAccuracy);


