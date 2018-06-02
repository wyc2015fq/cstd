


/**
 * @brief Reshapes the input Blob into flat vectors.
 *
 * Note: because this layer does not change the input values -- merely the
 * dimensions -- it can simply copy the input. The copy happens "virtually"
 * (thus taking effectively 0 real time) by setting, in Forward, the data
 * pointer of the top Blob to that of the bottom Blob (see Blob::ShareData),
 * and in Backward, the diff pointer of the bottom Blob to that of the top Blob
 * (see Blob::ShareDiff).
 */

class FlattenLayer : public Layer {
 public:
  explicit FlattenLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "Flatten"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

 protected:
  /**
   * @param bottom input Blob vector (length 2+)
   *   -# @f$ (N \times C \times H \times W) @f$
   *      the inputs
   * @param top output Blob vector (length 1)
   *   -# @f$ (N \times CHW \times 1 \times 1) @f$
   *      the outputs -- i.e., the (virtually) copied, flattened inputs
   */
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  /**
   * @brief Computes the error gradient w.r.t. the concatenate inputs.
   *
   * @param top output Blob vector (length 1), providing the error gradient with
   *        respect to the outputs
   * @param propagate_down see Layer::Backward.
   * @param bottom input Blob vector (length K), into which the top error
   *        gradient is (virtually) copied
   */
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
};




void FlattenLayer::Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  CHECK_NE(top[0], bottom[0]) << this->type() << " Layer does not "
      "allow in-place computation.";
  const int start_axis = bottom[0]->CanonicalAxisIndex(
      this->layer_param_.flatten_param().axis());
  const int end_axis = bottom[0]->CanonicalAxisIndex(
      this->layer_param_.flatten_param().end_axis());
  vector<int> top_shape;
  for (i = 0; i < start_axis; ++i) {
    top_shape.push_back(bottom[0]->shape(i));
  }
  const int flattened_dim = bottom[0]->count(start_axis, end_axis + 1);
  top_shape.push_back(flattened_dim);
  for (i = end_axis + 1; i < bottom[0]->num_axes(); ++i) {
    top_shape.push_back(bottom[0]->shape(i));
  }
  top[0]->Reshape(top_shape);
  CHECK_EQ(top[0]->count(), Data_count(prev->size));
}


void FlattenLayer::Forward(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  top[0]->ShareData(*bottom[0]);
}


void FlattenLayer::Backward(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  bottom[0]->ShareDiff(*top[0]);
}

INSTANTIATE_CLASS(FlattenLayer);
REGISTER_LAYER_CLASS(Flatten);


