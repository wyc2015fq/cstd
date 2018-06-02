



/**
 * @brief Takes two+ Blobs, interprets last Blob as a selector and
 *  filter remaining Blobs accordingly with selector data (0 means that
 * the corresponding item has to be filtered, non-zero means that corresponding
 * item needs to stay).
 */

class FilterLayer : public Layer {
 public:
  explicit FilterLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "Filter"; }
  virtual inline int MinBottomBlobs() const { return 2; }
  virtual inline int MinTopBlobs() const { return 1; }

 protected:
  /**
   * @param bottom input Blob vector (length 2+)
   *   -# @f$ (N \times C \times H \times W) @f$
   *      the inputs to be filtered @f$ x_1 @f$
   *   -# ...
   *   -# @f$ (N \times C \times H \times W) @f$
   *      the inputs to be filtered @f$ x_K @f$
   *   -# @f$ (N \times 1 \times 1 \times 1) @f$
   *      the selector blob
   * @param top output Blob vector (length 1+)
   *   -# @f$ (S \times C \times H \times W) @f$ ()
   *        the filtered output @f$ x_1 @f$
   *        where S is the number of items
   *        that haven't been filtered
   *      @f$ (S \times C \times H \times W) @f$
   *        the filtered output @f$ x_K @f$
   *        where S is the number of items
   *        that haven't been filtered
   */
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
    const vector<Blob*>& top);

  /**
   * @brief Computes the error gradient w.r.t. the forwarded inputs.
   *
   * @param top output Blob vector (length 1+), providing the error gradient with
   *        respect to the outputs
   * @param propagate_down see Layer::Backward.
   * @param bottom input Blob vector (length 2+), into which the top error
   *        gradient is copied
   */
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  bool first_reshape_;
  vector<int> indices_to_forward_;
};





void FilterLayer::LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  CHECK_EQ(layer->o_size, layer->i_size - 1);
  first_reshape_ = true;
}


void FilterLayer::Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  // bottom[0...k-1] are the blobs to filter
  // bottom[last] is the "selector_blob"
  int selector_index = layer->i_size - 1;
  for (i = 1; i < bottom[selector_index]->num_axes(); ++i) {
    CHECK_EQ(bottom[selector_index]->shape(i), 1)
        << "Selector blob dimensions must be singletons (1), except the first";
  }
  for (i = 0; i < layer->i_size - 1; ++i) {
    CHECK_EQ(bottom[selector_index]->shape(0), IBLOB(i)->shape(0)) <<
        "Each bottom should have the same 0th dimension as the selector blob";
  }

  const Dtype* bottom_data_selector = bottom[selector_index]->cpu_data();
  indices_to_forward_.clear();

  // look for non-zero elements in bottom[0]. Items of each bottom that
  // have the same index as the items in bottom[0] with value == non-zero
  // will be forwarded
  for (item_id = 0; item_id < bottom[selector_index]->shape(0); ++item_id) {
    // we don't need an offset because item size == 1
    const Dtype* tmp_data_selector = bottom_data_selector + item_id;
    if (*tmp_data_selector) {
      indices_to_forward_.push_back(item_id);
    }
  }
  // only filtered items will be forwarded
  int new_tops_num = indices_to_forward_.size();
  // init
  if (first_reshape_) {
    new_tops_num = prevSize.n;
    first_reshape_ = false;
  }
  for (t = 0; t < layer->o_size; ++t) {
    int num_axes = bottom[t]->num_axes();
    vector<int> shape_top(num_axes);
    shape_top[0] = new_tops_num;
    for (ts = 1; ts < num_axes; ++ts)
      shape_top[ts] = bottom[t]->shape(ts);
    top[t]->Reshape(shape_top);
  }
}


void FilterLayer::Forward(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  int new_tops_num = indices_to_forward_.size();
  // forward all filtered items for all bottoms but the Selector (bottom[last])
  for (t = 0; t < layer->o_size; ++t) {
    const Dtype* prevData = bottom[t]->cpu_data();
    Dtype* nextData = top[t]->mutable_data();
    int dim = bottom[t]->count() / bottom[t]->shape(0);
    for (n = 0; n < new_tops_num; ++n) {
      int data_offset_top = n * dim;
      int data_offset_bottom = indices_to_forward_[n] * bottom[t]->count(1);
      blas_scopy(dim, prevData + data_offset_bottom,
          nextData + data_offset_top);
    }
  }
}


void FilterLayer::Backward(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  if (propagate_down[layer->i_size - 1]) {
    LOG(FATAL) << this->type()
               << "Layer cannot backpropagate to filter index inputs";
  }
  for (i = 0; i < layer->o_size; i++) {
    // bottom[last] is the selector and never needs backpropagation
    // so we can iterate over top vector because layer->o_size == layer->i_size -1
    if (propagate_down[i]) {
      const int dim = top[i]->count() / top[i]->shape(0);
      int next_to_backward_offset = 0;
      int batch_offset = 0;
      int data_offset_bottom = 0;
      int data_offset_top = 0;
      for (n = 0; n < IBLOB(i)->shape(0); n++) {
        data_offset_bottom = n * dim;
        if (next_to_backward_offset >= indices_to_forward_.size()) {
          // we already visited all items that were been forwarded, so
          // just set to zero remaining ones
          blas_sset(dim, (Dtype)(0),
              IBLOB(i)->mutable_diff() + data_offset_bottom);
        } else {
          batch_offset = indices_to_forward_[next_to_backward_offset];
          if (n != batch_offset) {  // this data was not been forwarded
            blas_sset(dim, (Dtype)(0),
                IBLOB(i)->mutable_diff() + data_offset_bottom);
          } else {  // this data was been forwarded
            data_offset_top = next_to_backward_offset * dim;
            next_to_backward_offset++;  // point to next forwarded item index
            blas_scopy(dim, top[i]->mutable_diff() + data_offset_top,
                IBLOB(i)->mutable_diff() + data_offset_bottom);
          }
        }
      }
    }
  }
}

#ifdef CPU_ONLY
STUB_GPU(FilterLayer);
#endif

INSTANTIATE_CLASS(FilterLayer);
REGISTER_LAYER_CLASS(Filter);


