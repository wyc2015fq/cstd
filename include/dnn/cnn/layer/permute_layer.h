


  /**
  * @brief Permute the input blob by changing the memory order of the data.
  *
  * TODO(weiliu89): thorough documentation for Forward, Backward, and proto params.
  */

  // The main function which does the permute.
  
  void Permute(const int count, Dtype* prevData, const bool forward,
               const int* permute_order, const int* old_steps, const int* new_steps,
               const int num_axes, Dtype* nextData);

  
  class PermuteLayer : public Layer {
  public:
    explicit PermuteLayer(const LayerParameter& param)
      : Layer(param) {}
    virtual void LayerSetUp(const vector<Blob*>& bottom,
                            const vector<Blob*>& top);
    virtual void Reshape(const vector<Blob*>& bottom,
                         const vector<Blob*>& top);

    virtual inline const char* type() const { return "Permute"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  protected:
    virtual void Forward_cpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Forward_gpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Backward_cpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);
    virtual void Backward_gpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);

    int num_axes_;
    bool need_permute_;

    // Use Blob because it is convenient to be accessible in .cu file.
    Blob<int> permute_order_;
    Blob<int> old_steps_;
    Blob<int> new_steps_;
  };


  
  void Permute(const int count, Dtype* prevData, const bool forward,
               const int* permute_order, const int* old_steps, const int* new_steps,
               const int num_axes, Dtype* nextData) {
    for (i = 0; i < count; ++i) {
      int old_idx = 0;
      int idx = i;
      for (j = 0; j < num_axes; ++j) {
        int order = permute_order[j];
        old_idx += (idx / new_steps[j]) * old_steps[order];
        idx %= new_steps[j];
      }
      if (forward) {
        nextData[i] = prevData[old_idx];
      }
      else {
        prevData[old_idx] = nextData[i];
      }
    }
  }

  
  void PermuteLayer::LayerSetUp(const vector<Blob*>& bottom,
                                       const vector<Blob*>& top) {
    PermuteParameter permute_param = this->layer_param_.permute_param();
    CHECK_EQ(layer->i_size, 1);
    num_axes_ = bottom[0]->num_axes();
    vector<int> orders;
    // Push the specified new orders.
    for (i = 0; i < permute_param.order_size(); ++i) {
      int order = permute_param.order(i);
      CHECK_LT(order, num_axes_)
        << "order should be less than the input dimension.";
      if (std::find(orders.begin(), orders.end(), order) != orders.end()) {
        LOG(FATAL) << "there are duplicate orders";
      }
      orders.push_back(order);
    }
    // Push the rest orders. And save original step sizes for each axis.
    for (i = 0; i < num_axes_; ++i) {
      if (std::find(orders.begin(), orders.end(), i) == orders.end()) {
        orders.push_back(i);
      }
    }
    CHECK_EQ(num_axes_, orders.size());
    // Check if we need to reorder the data or keep it.
    need_permute_ = false;
    for (i = 0; i < num_axes_; ++i) {
      if (orders[i] != i) {
        // As long as there is one order which is different from the natural order
        // of the data, we need to permute. Otherwise, we share the data and diff.
        need_permute_ = true;
        break;
      }
    }

    vector<int> top_shape(num_axes_, 1);
    permute_order_.Reshape(num_axes_, 1, 1, 1);
    old_steps_.Reshape(num_axes_, 1, 1, 1);
    new_steps_.Reshape(num_axes_, 1, 1, 1);
    for (i = 0; i < num_axes_; ++i) {
      permute_order_[i] = orders[i];
      top_shape[i] = bottom[0]->shape(orders[i]);
    }
    top[0]->Reshape(top_shape);
  }

  
  void PermuteLayer::Reshape(const vector<Blob*>& bottom,
                                    const vector<Blob*>& top) {
    vector<int> top_shape;
    for (i = 0; i < num_axes_; ++i) {
      if (i == num_axes_ - 1) {
        old_steps_[i] = 1;
      }
      else {
        old_steps_[i] = bottom[0]->count(i + 1);
      }
      top_shape.push_back(bottom[0]->shape(permute_order_.cpu_data()[i]));
    }
    top[0]->Reshape(top_shape);

    for (i = 0; i < num_axes_; ++i) {
      if (i == num_axes_ - 1) {
        new_steps_[i] = 1;
      }
      else {
        new_steps_[i] = top[0]->count(i + 1);
      }
    }
  }

  
  void PermuteLayer::Forward(const vector<Blob*>& bottom,
                                        const vector<Blob*>& top) {
    if (need_permute_) {
      Dtype* prevData = bottom[0]->mutable_data();
      Dtype* nextData = nextData;
      const int top_count = top[0]->count();
      const int* permute_order = permute_order_.cpu_data();
      const int* old_steps = old_steps_.cpu_data();
      const int* new_steps = new_steps_.cpu_data();
      bool forward = true;
      Permute(top_count, prevData, forward, permute_order, old_steps,
              new_steps, num_axes_, nextData);
    }
    else {
      // If there is no need to permute, we share data to save memory.
      top[0]->ShareData(*bottom[0]);
    }
  }

  
  void PermuteLayer::Backward(const vector<Blob*>& top,
                                         const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
    if (need_permute_) {
      Dtype* nextDiff = top[0]->mutable_diff();
      Dtype* prevDiff = bottom[0]->mutable_diff();
      const int top_count = top[0]->count();
      const int* permute_order = permute_order_.cpu_data();
      const int* old_steps = old_steps_.cpu_data();
      const int* new_steps = new_steps_.cpu_data();
      bool forward = false;
      Permute(top_count, prevDiff, forward, permute_order, old_steps,
              new_steps, num_axes_, nextDiff);
    }
    else {
      // If there is no need to permute, we share diff to save memory.
      bottom[0]->ShareDiff(*top[0]);
    }
  }

#ifdef CPU_ONLY
  STUB_GPU(PermuteLayer);
#endif

  INSTANTIATE_CLASS(PermuteLayer);
  REGISTER_LAYER_CLASS(Permute);

