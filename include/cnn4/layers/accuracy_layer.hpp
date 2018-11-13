#ifndef CAFFE_ACCURACY_LAYER_HPP_
#define CAFFE_ACCURACY_LAYER_HPP_

#include <xfunctional>

/**
 * @brief Computes the classification accuracy for a one-of-many
 *        classification task.
 */

#define AccuracyParameter_DEF(DEF) \
DEF##Int(top_k, 1, 0) \
DEF##Int(axis, 1, 0) \
DEF##Int(ignore_label, -1, 0) \

class AccuracyLayer : public Layer
{
public:
  AccuracyParameter_DEF(Def);
  int label_axis_, outer_num_, inner_num_;
  /// Whether to ignore instances with a certain label.
  bool has_ignore_label_;
  /// The label indicating that an instance should be ignored.
  /// Keeps counts of the number of samples per class.
  Blob nums_buffer_;


  AccuracyLayer() {
    AccuracyParameter_DEF(Set);
  }
  void init(CJSON* param) {
    AccuracyParameter_DEF(Get);
    has_ignore_label_ = param->has("ignore_label");
  }
  /**
   * @param param provides AccuracyParameter accuracy_param,
   *     with AccuracyLayer options:
   *   - top_k (\b optional, default 1).
   *     Sets the maximum rank @f$ k @f$ at which a prediction is considered
   *     correct.  For example, if @f$ k = 5 @f$, a prediction is counted
   *     correct if the correct label is among the top 5 predicted labels.
   */

  virtual inline const char* type() const { return "Accuracy"; }
  virtual inline int ExactNumBottomBlobs() const { return 2; }

  // If there are two top blobs, then the second blob will contain
  // accuracies per class.
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlos() const { return 2; }

  /// @brief Not implemented -- AccuracyLayer cannot be used as a loss.
  virtual void Backward(const vector<Blob*> & top,
    const vector<Blob*> & bottom) {
    for (int i = 0; i < top.size(); ++i) {
      if (bottom[i]->propagate_down_) { NOT_IMPLEMENTED; }
    }
  }

  virtual void LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
  }

  virtual void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    CHECK_LE(top_k_, bottom[0]->count() / bottom[1]->count())
      << "top_k must be less than or equal to the number of classes.";
    label_axis_ = bottom[0]->CanonicalAxisIndex(this->axis_);
    outer_num_ = bottom[0]->count(0, label_axis_);
    inner_num_ = bottom[0]->count(label_axis_ + 1);
    CHECK_EQ(outer_num_ * inner_num_, bottom[1]->count())
      << "Number of labels must match number of predictions; "
      << "e.g., if label axis == 1 and prediction shape is (N, C, H, W), "
      << "label count (number of labels) must be N*H*W, "
      << "with integer values in {0, 1, ..., C-1}.";
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

  virtual void Forward(const vector<Blob*> & bottom,
    const vector<Blob*> & top)
  {
    Dtype accuracy = 0;
    Dtype* top0_data = top[0]->cpu_mdata();
    Dtype* top1_data = NULL;
    Dtype* nums_buffer_data = nums_buffer_.cpu_mdata();
    const Dtype* bottom_data = bottom[0]->cpu_data();
    const Dtype* bottom_label = bottom[1]->cpu_data();
    const int dim = bottom[0]->count() / outer_num_;
    const int num_labels = bottom[0]->shape(label_axis_);
    vector<Dtype> maxval(top_k_ + 1);
    vector<int> max_id(top_k_ + 1);
    if (top.size() > 1) {
      top1_data = top[1]->cpu_mdata();
      cpu_caffe_set(nums_buffer_.count(), Dtype(0), nums_buffer_data);
      cpu_caffe_set(top[1]->count(), Dtype(0), top1_data);
    }
    int count = 0;
    for (int i = 0; i < outer_num_; ++i) {
      for (int j = 0; j < inner_num_; ++j) {
        const int label_value =
          static_cast<int>(bottom_label[i * inner_num_ + j]);
        if (has_ignore_label_ && label_value == ignore_label_) {
          continue;
        }
        if (top.size() > 1) { ++nums_buffer_data[label_value]; }
        DCHECK_GE(label_value, 0);
        DCHECK_LT(label_value, num_labels);
        // Top-k accuracy
        std::vector<std::pair<Dtype, int> > bottom_data_vector;
        for (int k = 0; k < num_labels; ++k) {
          bottom_data_vector.push_back(std::make_pair(
            bottom_data[i * dim + k * inner_num_ + j], k));
        }
        std::partial_sort(
          bottom_data_vector.begin(), bottom_data_vector.begin() + top_k_,
          bottom_data_vector.end(), std::greater<std::pair<Dtype, int> >());
        // check if true label is in top k predictions
        for (int k = 0; k < top_k_; k++) {
          if (bottom_data_vector[k].second == label_value) {
            ++accuracy;
            if (top.size() > 1) { ++top1_data[label_value]; }
            break;
          }
        }
        ++count;
      }
    }
    // LOG(INFO) << "Accuracy: " << accuracy;
    top0_data[0] = accuracy / count;
    if (top.size() > 1) {
      for (int i = 0; i < top[1]->count(); ++i) {
        top1_data[i] = nums_buffer_data[i] == 0 ? 0 : top1_data[i] / nums_buffer_data[i];
      }
    }
    // Accuracy layer should not be used as a loss function.
  }

};



/* MultiLabelAccuracyLayer
  Note: It is also a loss layer! Does not implement backwards step.
  Computes the accuracy and cross entropy loss with respect to b.
*/
template <typename Dtype>
class MultiLabelAccuracyLayer : public Layer
{
public:
  int label_axis_, outer_num_, inner_num_, label_num_;

  int top_k_;

  /// Whether to ignore instances with a certain label.
  bool has_ignore_label_;
  /// The label indicating that an instance should be ignored.
  int ignore_label_;
  /// Keeps counts of the number of samples per class.
  Blob nums_buffer_;


  virtual inline const char* type() const {
    return "MultiLabelAccuracy";
  }
  virtual inline int ExactNumBottomBlobs() const { return 2; }

  // If there are two top blobs, then the second blob will contain
  // accuracies per class.
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlos() const { return 2; }

  /// @brief Not implemented -- AccuracyLayer cannot be used as a loss.
  virtual void Backward(const vector<Blob*> & top,
    const vector<Blob*> & bottom) {
    for (int i = 0; i < propagate_down.size(); ++i) {
      if (bottom[i]->propagate_down_) { NOT_IMPLEMENTED; }
    }
  }

};

INSTANTIATE_CLASS(Accuracy);



#endif  // CAFFE_ACCURACY_LAYER_HPP_
