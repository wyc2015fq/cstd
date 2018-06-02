
#include "loss_layer.hpp"



/**
 * @brief Computes the classification accuracy for a one-of-many
 *        classification task.
 */

class AccuracyLayer : public Layer {
 public:
  /**
   * @param param provides AccuracyParameter accuracy_param,
   *     with AccuracyLayer options:
   *   - top_k (\b optional, default 1).
   *     Sets the maximum rank @f$ k @f$ at which a prediction is considered
   *     correct.  For example, if @f$ k = 5 @f$, a prediction is counted
   *     correct if the correct label is among the top 5 predicted labels.
   */
  explicit AccuracyLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "Accuracy"; }
  virtual inline int ExactNumBottomBlobs() const { return -1; }
  virtual inline int MinBottomBlobs() const { return 2; }
  virtual inline int MaxBottomBlobs() const { return 3; }

  // If there are two top blobs, then the second blob will contain
  // accuracies per class.
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlobs() const { return 2; }

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
    for (int i = 0; i < propagate_down.size(); ++i) {
      if (propagate_down[i]) { NOT_IMPLEMENTED; }
    }
  }

  int label_axis_, outer_num_, inner_num_;

  int top_k_;

  /// Whether to ignore instances with a certain label.
  bool has_ignore_label_;
  /// The label indicating that an instance should be ignored.
  int ignore_label_;
  /// Keeps counts of the number of samples per class.
  Blob nums_buffer_;
  //inverse the sort order.
  bool min_is_better_;
};






void AccuracyLayer::LayerSetUp(
  const vector<Blob*>& bottom, const vector<Blob*>& top) {
  top_k_ = this->layer_param_.accuracy_param().top_k();

  has_ignore_label_ =
    this->layer_param_.accuracy_param().has_ignore_label();
  if (has_ignore_label_) {
    ignore_label_ = this->layer_param_.accuracy_param().ignore_label();
  }
  min_is_better_ = this->layer_param_.accuracy_param().min_is_better();
}


void AccuracyLayer::Reshape(
  const vector<Blob*>& bottom, const vector<Blob*>& top) {
  CHECK_LE(top_k_, bottom[0]->count() / bottom[1]->count())
      << "top_k must be less than or equal to the number of classes.";
  label_axis_ =
      bottom[0]->CanonicalAxisIndex(this->layer_param_.accuracy_param().axis());
  outer_num_ = bottom[0]->count(0, label_axis_);
  inner_num_ = bottom[0]->count(label_axis_ + 1);
  CHECK_EQ(outer_num_ * inner_num_, bottom[1]->count())
      << "Number of labels must match number of predictions; "
      << "e.g., if label axis == 1 and prediction shape is (N, C, H, W), "
      << "label count (number of labels) must be N*H*W, "
      << "with integer values in {0, 1, ..., C-1}.";
  if (bottom.size() == 3) {
    CHECK_EQ(outer_num_ * inner_num_, bottom[2]->count())
      << "Number of loss weights must match number of label.";
  }
  vector<int> top_shape(0);  // Accuracy is a scalar; 0 axes.
  top[0]->Reshape(top_shape);
  if (top.size() > 1) {
    // Per-class accuracy is a vector; 1 axes.
    vector<int> top_shape_per_class(1);
    top_shape_per_class[0] = bottom[0]->shape(label_axis_);
    top[1]->Reshape(top_shape_per_class);
    nums_buffer_.Reshape(top_shape_per_class);
  }
}


void AccuracyLayer::Forward_cpu(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  Dtype accuracy = 0;
  const Dtype* bottom_data = Blob_datap(bottom_blob->data);
  const Dtype* bottom_label = Blob_datap(top_blob->data);
  const int dim = bottom[0]->count() / outer_num_;
  const int num_labels = bottom[0]->shape(label_axis_);
  vector maxval(top_k_+1);
  vector<int> max_id(top_k_+1);
  if (top.size() > 1) {
    caffe_set(nums_buffer_.count(), Dtype(0), nums_buffer_.mutable_cpu_data());
    caffe_set(top[1]->count(), Dtype(0), top[1]->mutable_cpu_data());
  }
  int count = 0;
  for (int i = 0; i < outer_num_; ++i) {
    for (int j = 0; j < inner_num_; ++j) {
      const int label_value =
          static_cast<int>(bottom_label[i * inner_num_ + j]);
      if (bottom.size() == 3 && bottom[2]->cpu_data()[i * inner_num_ + j] == 0) {
        continue;
      }
      if (has_ignore_label_ && label_value == ignore_label_) {
        continue;
      }
      if (top.size() > 1) ++nums_buffer_.mutable_cpu_data()[label_value];
      DCHECK_GE(label_value, 0);
      DCHECK_LT(label_value, num_labels);
      // Top-k accuracy
      std::vector<std::pair<Dtype, int> > bottom_data_vector;
      for (int k = 0; k < num_labels; ++k) {
        bottom_data_vector.push_back(std::make_pair(
            bottom_data[i * dim + k * inner_num_ + j], k));
      }
      if (min_is_better_) {
        std::partial_sort(
          bottom_data_vector.begin(), bottom_data_vector.begin() + top_k_,
          bottom_data_vector.end(), std::less<std::pair<Dtype, int> >());
      }
      else {
        std::partial_sort(
          bottom_data_vector.begin(), bottom_data_vector.begin() + top_k_,
          bottom_data_vector.end(), std::greater<std::pair<Dtype, int> >());
      }
      
      // check if true label is in top k predictions
      for (int k = 0; k < top_k_; k++) {
        if (bottom_data_vector[k].second == label_value) {
          ++accuracy;
          if (top.size() > 1) ++top[1]->mutable_cpu_data()[label_value];
          break;
        }
      }
      ++count;
    }
  }

  // LOG(INFO) << "Accuracy: " << accuracy;
  top[0]->mutable_cpu_data()[0] = accuracy / count;
  if (top.size() > 1) {
    for (int i = 0; i < top[1]->count(); ++i) {
      top[1]->mutable_cpu_data()[i] =
          nums_buffer_.cpu_data()[i] == 0 ? 0
          : top[1]->cpu_data()[i] / nums_buffer_.cpu_data()[i];
    }
  }
  // Accuracy layer should not be used as a loss function.
}

INSTANTIATE_CLASS(AccuracyLayer);
REGISTER_LAYER_CLASS(Accuracy);


