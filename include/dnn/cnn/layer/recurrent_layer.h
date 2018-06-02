

 class RecurrentLayer;

/**
 * @brief An abstract class for implementing recurrent behavior inside of an
 *        unrolled network.  This Layer type cannot be instantiated -- instead,
 *        you should use one of its implementations which defines the recurrent
 *        architecture, such as RNNLayer or LSTMLayer.
 */

class RecurrentLayer : public Layer {
 public:
  explicit RecurrentLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reset();

  virtual inline const char* type() const { return "Recurrent"; }
  virtual inline int MinBottomBlobs() const {
    int min_bottoms = 2;
    if (this->layer_param_.recurrent_param().expose_hidden()) {
      vector<string> inputs;
      this->RecurrentInputBlobNames(&inputs);
      min_bottoms += inputs.size();
    }
    return min_bottoms;
  }
  virtual inline int MaxBottomBlobs() const { return MinBottomBlobs() + 1; }
  virtual inline int ExactNumTopBlobs() const {
    int num_tops = 1;
    if (this->layer_param_.recurrent_param().expose_hidden()) {
      vector<string> outputs;
      this->RecurrentOutputBlobNames(&outputs);
      num_tops += outputs.size();
    }
    return num_tops;
  }

  virtual inline bool AllowForceBackward(const int bottom_index) const {
    // Can't propagate to sequence continuation indicators.
    return bottom_index != 1;
  }

 protected:
  /**
   * @brief Fills net_param with the recurrent network architecture.  Subclasses
   *        should define this -- see RNNLayer and LSTMLayer for examples.
   */
  virtual void FillUnrolledNet(NetParameter* net_param) const = 0;

  /**
   * @brief Fills names with the names of the 0th timestep recurrent input
   *        Blob&s.  Subclasses should define this -- see RNNLayer and LSTMLayer
   *        for examples.
   */
  virtual void RecurrentInputBlobNames(vector<string>* names) const = 0;

  /**
   * @brief Fills shapes with the shapes of the recurrent input Blob&s.
   *        Subclasses should define this -- see RNNLayer and LSTMLayer
   *        for examples.
   */
  virtual void RecurrentInputShapes(vector<BlobShape>* shapes) const = 0;

  /**
   * @brief Fills names with the names of the Tth timestep recurrent output
   *        Blob&s.  Subclasses should define this -- see RNNLayer and LSTMLayer
   *        for examples.
   */
  virtual void RecurrentOutputBlobNames(vector<string>* names) const = 0;

  /**
   * @brief Fills names with the names of the output blobs, concatenated across
   *        all timesteps.  Should return a name for each top Blob.
   *        Subclasses should define this -- see RNNLayer and LSTMLayer for
   *        examples.
   */
  virtual void OutputBlobNames(vector<string>* names) const = 0;

  /**
   * @param bottom input Blob vector (length 2-3)
   *
   *   -# @f$ (T \times N \times ...) @f$
   *      the time-varying input @f$ x @f$.  After the first two axes, whose
   *      dimensions must correspond to the number of timesteps @f$ T @f$ and
   *      the number of independent streams @f$ N @f$, respectively, its
   *      dimensions may be arbitrary.  Note that the ordering of dimensions --
   *      @f$ (T \times N \times ...) @f$, rather than
   *      @f$ (N \times T \times ...) @f$ -- means that the @f$ N @f$
   *      independent input streams must be "interleaved".
   *
   *   -# @f$ (T \times N) @f$
   *      the sequence continuation indicators @f$ \delta @f$.
   *      These inputs should be binary (0 or 1) indicators, where
   *      @f$ \delta_{t,n} = 0 @f$ means that timestep @f$ t @f$ of stream
   *      @f$ n @f$ is the beginning of a new sequence, and hence the previous
   *      hidden state @f$ h_{t-1} @f$ is multiplied by @f$ \delta_t = 0 @f$
   *      and has no effect on the cell's output at timestep @f$ t @f$, and
   *      a value of @f$ \delta_{t,n} = 1 @f$ means that timestep @f$ t @f$ of
   *      stream @f$ n @f$ is a continuation from the previous timestep
   *      @f$ t-1 @f$, and the previous hidden state @f$ h_{t-1} @f$ affects the
   *      updated hidden state and output.
   *
   *   -# @f$ (N \times ...) @f$ (optional)
   *      the static (non-time-varying) input @f$ x_{static} @f$.
   *      After the first axis, whose dimension must be the number of
   *      independent streams, its dimensions may be arbitrary.
   *      This is mathematically equivalent to using a time-varying input of
   *      @f$ x'_t = [x_t; x_{static}] @f$ -- i.e., tiling the static input
   *      across the @f$ T @f$ timesteps and concatenating with the time-varying
   *      input.  Note that if this input is used, all timesteps in a single
   *      batch within a particular one of the @f$ N @f$ streams must share the
   *      same static input, even if the sequence continuation indicators
   *      suggest that difference sequences are ending and beginning within a
   *      single batch.  This may require padding and/or truncation for uniform
   *      length.
   *
   * @param top output Blob vector (length 1)
   *   -# @f$ (T \times N \times D) @f$
   *      the time-varying output @f$ y @f$, where @f$ D @f$ is
   *      <code>recurrent_param.num_output()</code>.
   *      Refer to documentation for particular RecurrentLayer implementations
   *      (such as RNNLayer and LSTMLayer) for the definition of @f$ y @f$.
   */
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  /// @brief A Net to implement the Recurrent functionality.
  shared_ptr<Net > unrolled_net_;

  /// @brief The number of independent streams to process simultaneously.
  int N_;

  /**
   * @brief The number of timesteps in the layer's input, and the number of
   *        timesteps over which to backpropagate through time.
   */
  int T_;

  /// @brief Whether the layer has a "static" input copied across all timesteps.
  bool static_input_;

  /**
   * @brief The last layer to run in the network. (Any later layers are losses
   *        added to force the recurrent net to do backprop.)
   */
  int last_layer_index_;

  /**
   * @brief Whether the layer's hidden state at the first and last timesteps
   *        are layer inputs and outputs, respectively.
   */
  bool expose_hidden_;

  vector<Blob* > recur_input_blobs_;
  vector<Blob* > recur_output_blobs_;
  vector<Blob* > output_blobs_;
  Blob* x_input_blob_;
  Blob* x_static_input_blob_;
  Blob* cont_input_blob_;
};





void RecurrentLayer::LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  CHECK_GE(bottom[0]->num_axes(), 2)
      << "bottom[0] must have at least 2 axes -- (#timesteps, #streams, ...)";
  T_ = prevSize.n;
  N_ = prevSize.c;
  LOG(INFO) << "Initializing recurrent layer: assuming input batch contains "
            << T_ << " timesteps of " << N_ << " independent streams.";

  CHECK_EQ(bottom[1]->num_axes(), 2)
      << "bottom[1] must have exactly 2 axes -- (#timesteps, #streams)";
  CHECK_EQ(T_, prevSize1.n);
  CHECK_EQ(N_, prevSize1.c);

  // If expose_hidden is set, we take as input and produce as output
  // the hidden state blobs at the first and last timesteps.
  expose_hidden_ = this->layer_param_.recurrent_param().expose_hidden();

  // Get (recurrent) input/output names.
  vector<string> output_names;
  OutputBlobNames(&output_names);
  vector<string> recur_input_names;
  RecurrentInputBlobNames(&recur_input_names);
  vector<string> recur_output_names;
  RecurrentOutputBlobNames(&recur_output_names);
  const int num_recur_blobs = recur_input_names.size();
  CHECK_EQ(num_recur_blobs, recur_output_names.size());

  // If provided, bottom[2] is a static input to the recurrent net.
  const int num_hidden_exposed = expose_hidden_ * num_recur_blobs;
  static_input_ = (layer->i_size > 2 + num_hidden_exposed);
  if (static_input_) {
    CHECK_GE(bottom[2]->num_axes(), 1);
    CHECK_EQ(N_, bottom[2]->shape(0));
  }

  // Create a NetParameter; setup the inputs that aren't unique to particular
  // recurrent architectures.
  NetParameter net_param;

  LayerParameter* input_layer_param = net_param.add_layer();
  input_layer_param->set_type("Input");
  InputParameter* input_param = input_layer_param->mutable_input_param();
  input_layer_param->add_top("x");
  BlobShape input_shape;
  for (i = 0; i < bottom[0]->num_axes(); ++i) {
    input_shape.add_dim(bottom[0]->shape(i));
  }
  input_param->add_shape()->CopyFrom(input_shape);

  input_shape.Clear();
  for (i = 0; i < bottom[1]->num_axes(); ++i) {
    input_shape.add_dim(bottom[1]->shape(i));
  }
  input_layer_param->add_top("cont");
  input_param->add_shape()->CopyFrom(input_shape);

  if (static_input_) {
    input_shape.Clear();
    for (i = 0; i < bottom[2]->num_axes(); ++i) {
      input_shape.add_dim(bottom[2]->shape(i));
    }
    input_layer_param->add_top("x_static");
    input_param->add_shape()->CopyFrom(input_shape);
  }

  // Call the child's FillUnrolledNet implementation to specify the unrolled
  // recurrent architecture.
  this->FillUnrolledNet(&net_param);

  // Prepend this layer's name to the names of each layer in the unrolled net.
  const string& layer_name = this->layer_param_.name();
  if (layer_name.size()) {
    for (i = 0; i < net_param.layer_size(); ++i) {
      LayerParameter* layer = net_param.mutable_layer(i);
      layer->set_name(layer_name + "_" + layer->name());
    }
  }

  // Add "pseudo-losses" to all outputs to force backpropagation.
  // (Setting force_backward is too aggressive as we may not need to backprop to
  // all inputs, e.g., the sequence continuation indicators.)
  vector<string> pseudo_losses(output_names.size());
  for (i = 0; i < output_names.size(); ++i) {
    LayerParameter* layer = net_param.add_layer();
    pseudo_losses[i] = output_names[i] + "_pseudoloss";
    layer->set_name(pseudo_losses[i]);
    layer->set_type("Reduction");
    layer->add_bottom(output_names[i]);
    layer->add_top(pseudo_losses[i]);
    layer->add_loss_weight(1);
  }

  // Create the unrolled net.
  unrolled_net_.reset(new Net(net_param));
  unrolled_net_->set_debug_info(
      this->layer_param_.recurrent_param().debug_info());

  // Setup pointers to the inputs.
  x_input_blob_ = CHECK_NOTNULL(unrolled_net_->blob_by_name("x").get());
  cont_input_blob_ = CHECK_NOTNULL(unrolled_net_->blob_by_name("cont").get());
  if (static_input_) {
    x_static_input_blob_ =
        CHECK_NOTNULL(unrolled_net_->blob_by_name("x_static").get());
  }

  // Setup pointers to paired recurrent inputs/outputs.
  recur_input_blobs_.resize(num_recur_blobs);
  recur_output_blobs_.resize(num_recur_blobs);
  for (i = 0; i < recur_input_names.size(); ++i) {
    recur_input_blobs_[i] =
        CHECK_NOTNULL(unrolled_net_->blob_by_name(recur_input_names[i]).get());
    recur_output_blobs_[i] =
        CHECK_NOTNULL(unrolled_net_->blob_by_name(recur_output_names[i]).get());
  }

  // Setup pointers to outputs.
  CHECK_EQ(layer->o_size - num_hidden_exposed, output_names.size())
      << "OutputBlobNames must provide an output blob name for each top.";
  output_blobs_.resize(output_names.size());
  for (i = 0; i < output_names.size(); ++i) {
    output_blobs_[i] =
        CHECK_NOTNULL(unrolled_net_->blob_by_name(output_names[i]).get());
  }

  // We should have 2 inputs (x and cont), plus a number of recurrent inputs,
  // plus maybe a static input.
  CHECK_EQ(2 + num_recur_blobs + static_input_,
           unrolled_net_->input_blobs().size());

  // This layer's parameters are any parameters in the layers of the unrolled
  // net. We only want one copy of each parameter, so check that the parameter
  // is "owned" by the layer, rather than shared with another.
  this->blobs_.clear();
  for (i = 0; i < unrolled_net_->params().size(); ++i) {
    if (unrolled_net_->param_owners()[i] == -1) {
      LOG(INFO) << "Adding parameter " << i << ": "
                << unrolled_net_->param_display_names()[i];
      this->blobs_.push_back(unrolled_net_->params()[i]);
    }
  }
  // Check that param_propagate_down is set for all of the parameters in the
  // unrolled net; set param_propagate_down to true in this layer.
  for (i = 0; i < unrolled_net_->layers().size(); ++i) {
    for (j = 0; j < unrolled_net_->layers()[i]->blobs().size(); ++j) {
      CHECK(unrolled_net_->layers()[i]->param_propagate_down(j))
          << "param_propagate_down not set for layer " << i << ", param " << j;
    }
  }
  this->param_propagate_down_.clear();
  this->param_propagate_down_.resize(this->blobs_.size(), true);

  // Set the diffs of recurrent outputs to 0 -- we can't backpropagate across
  // batches.
  for (i = 0; i < recur_output_blobs_.size(); ++i) {
    blas_sset(recur_output_blobs_[i]->count(), (Dtype)(0),
              recur_output_blobs_[i]->mutable_diff());
  }

  // Check that the last output_names.size() layers are the pseudo-losses;
  // set last_layer_index so that we don't actually run these layers.
  const vector<string>& layer_names = unrolled_net_->layer_names();
  last_layer_index_ = layer_names.size() - 1 - pseudo_losses.size();
  for (i = last_layer_index_ + 1, j = 0; i < layer_names.size(); ++i, ++j) {
    CHECK_EQ(layer_names[i], pseudo_losses[j]);
  }
}


void RecurrentLayer::Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  CHECK_GE(bottom[0]->num_axes(), 2)
      << "bottom[0] must have at least 2 axes -- (#timesteps, #streams, ...)";
  CHECK_EQ(T_, prevSize.n) << "input number of timesteps changed";
  N_ = prevSize.c;
  CHECK_EQ(bottom[1]->num_axes(), 2)
      << "bottom[1] must have exactly 2 axes -- (#timesteps, #streams)";
  CHECK_EQ(T_, prevSize1.n);
  CHECK_EQ(N_, prevSize1.c);
  x_input_blob_->ReshapeLike(*bottom[0]);
  vector<int> cont_shape = bottom[1]->shape();
  cont_input_blob_->Reshape(cont_shape);
  if (static_input_) {
    x_static_input_blob_->ReshapeLike(*bottom[2]);
  }
  vector<BlobShape> recur_input_shapes;
  RecurrentInputShapes(&recur_input_shapes);
  CHECK_EQ(recur_input_shapes.size(), recur_input_blobs_.size());
  for (i = 0; i < recur_input_shapes.size(); ++i) {
    recur_input_blobs_[i]->Reshape(recur_input_shapes[i]);
  }
  unrolled_net_->Reshape();
  x_input_blob_->ShareData(*bottom[0]);
  x_input_blob_->ShareDiff(*bottom[0]);
  cont_input_blob_->ShareData(*bottom[1]);
  if (static_input_) {
    x_static_input_blob_->ShareData(*bottom[2]);
    x_static_input_blob_->ShareDiff(*bottom[2]);
  }
  if (expose_hidden_) {
    const int bottom_offset = 2 + static_input_;
    for (i = bottom_offset, j = 0; i < layer->i_size; ++i, ++j) {
      CHECK(recur_input_blobs_[j]->shape() == IBLOB(i)->shape())
          << "bottom[" << i << "] shape must match hidden state input shape: "
          << recur_input_blobs_[j]->shape_string();
      recur_input_blobs_[j]->ShareData(*IBLOB(i));
    }
  }
  for (i = 0; i < output_blobs_.size(); ++i) {
    top[i]->ReshapeLike(*output_blobs_[i]);
    top[i]->ShareData(*output_blobs_[i]);
    top[i]->ShareDiff(*output_blobs_[i]);
  }
  if (expose_hidden_) {
    const int top_offset = output_blobs_.size();
    for (i = top_offset, j = 0; i < layer->o_size; ++i, ++j) {
      top[i]->ReshapeLike(*recur_output_blobs_[j]);
    }
  }
}


void RecurrentLayer::Reset() {
  // "Reset" the hidden state of the net by zeroing out all recurrent outputs.
  for (i = 0; i < recur_output_blobs_.size(); ++i) {
    blas_sset(recur_output_blobs_[i]->count(), (Dtype)(0),
              recur_output_blobs_[i]->mutable_data());
  }
}


void RecurrentLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  // Hacky fix for test time: reshare all the internal shared blobs, which may
  // currently point to a stale owner blob that was dropped when Solver::Test
  // called test_net->ShareTrainedLayersWith(net_.get()).
  // TODO: somehow make this work non-hackily.
  if (this->phase_ == TEST) {
    unrolled_net_->ShareWeights();
  }

  DCHECK_EQ(recur_input_blobs_.size(), recur_output_blobs_.size());
  if (!expose_hidden_) {
    for (i = 0; i < recur_input_blobs_.size(); ++i) {
      const int count = recur_input_blobs_[i]->count();
      DCHECK_EQ(count, recur_output_blobs_[i]->count());
      const Dtype* timestep_T_data = recur_output_blobs_[i]->cpu_data();
      Dtype* timestep_0_data = recur_input_blobs_[i]->mutable_data();
      blas_scopy(count, timestep_T_data, timestep_0_data);
    }
  }

  unrolled_net_->ForwardTo(last_layer_index_);

  if (expose_hidden_) {
    const int top_offset = output_blobs_.size();
    for (i = top_offset, j = 0; i < layer->o_size; ++i, ++j) {
      top[i]->ShareData(*recur_output_blobs_[j]);
    }
  }
}


void RecurrentLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  CHECK(!propagate_down[1]) << "Cannot backpropagate to sequence indicators.";

  // TODO: skip backpropagation to inputs and parameters inside the unrolled
  // net according to propagate_down[0] and propagate_down[2]. For now just
  // backprop to inputs and parameters unconditionally, as either the inputs or
  // the parameters do need backward (or Net would have set
  // layer_needs_backward_[i] == false for this layer).
  unrolled_net_->BackwardFrom(last_layer_index_);
}

#ifdef CPU_ONLY
STUB_GPU_FORWARD(RecurrentLayer, Forward);
#endif

INSTANTIATE_CLASS(RecurrentLayer);


