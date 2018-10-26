#ifndef CAFFE_NET_HPP_
#define CAFFE_NET_HPP_

#include <map>
#include <set>
#include <string>
#include <utility>
#include <vector>

#include "blob.hpp"
#include "common.hpp"
#include "layer.hpp"

/**
 * @brief Connects Layer%s together into a directed acyclic graph (DAG)
 *        specified by a NetParameter.
 *
 * TODO(dox): more thorough description.
 */
template <typename Dtype>
class Net
{
public:
  explicit Net(CJSON* param, const Net* root_net = NULL)
    : root_net_(root_net)
  {
    Init(param);
  }

  explicit Net(const string & param_file, Phase phase,
    const int level = 0, const vector<string>* stages = NULL,
    const Net* root_net = NULL)
    : root_net_(root_net)
  {
    NetParameter param;
    ReadNetParamsFromTextFileOrDie(param_file, &param);
    // Set phase, stages and level
    param.mutable_state()->set_phase(phase);
    if (stages != NULL) {
      for (int i = 0; i < stages->size(); i++) {
        param.mutable_state()->add_stage((*stages)[i]);
      }
    }
    param.mutable_state()->set_level(level);
    Init(param);
  }
  virtual ~Net() {}

  /// @brief Initialize a network with a NetParameter.
  void Init(CJSON* param) {
    CHECK(root_solver() || root_net_)
      << "root_net_ needs to be set for all non-root solvers";
    // Set phase from the state.
    phase_ = in_param.state().phase();
    // Filter layers based on their include/exclude rules and
    // the current NetState.
    NetParameter filtered_param;
    FilterNet(in_param, &filtered_param);
    LOG_IF(INFO, root_solver())
      << "Initializing net from parameters: " << std::endl
      << filtered_param.DebugString();
    // Create a copy of filtered_param with splits added where necessary.
    NetParameter param;
    InsertSplits(filtered_param, &param);
    // Basically, build all the layers and set up their connections.
    name_ = param.name();
    map<string, int> blob_name_to_idx;
    set<string> available_blobs;
    memory_used_ = 0;
    // For each layer, set up its input and output
    bottom_vecs_.resize(param.layer_size());
    top_vecs_.resize(param.layer_size());
    bottom_id_vecs_.resize(param.layer_size());
    param_id_vecs_.resize(param.layer_size());
    top_id_vecs_.resize(param.layer_size());
    bottom_need_backward_.resize(param.layer_size());
    for (int layer_id = 0; layer_id < param.layer_size(); ++layer_id) {
      // For non-root solvers, whether this layer is shared from root_net_.
      bool share_from_root = !root_solver()
        && root_net_->layers_[layer_id]->ShareInParallel();
      // Inherit phase from net if unset.
      if (!param.layer(layer_id).has_phase()) {
        param.mutable_layer(layer_id)->set_phase(phase_);
      }
      // Setup layer.
      CJSON* layer_param = param.layer(layer_id);
      if (layer_param.propagate_down_size() > 0) {
        CHECK_EQ(layer_param.propagate_down_size(),
          layer_param.bottom_size())
          << "propagate_down param must be specified "
          << "either 0 or bottom_size times ";
      }
      if (share_from_root) {
        LOG(INFO) << "Sharing layer " << layer_param.name() << " from root net";
        layers_.push_back(root_net_->layers_[layer_id]);
        layers_[layer_id]->SetShared(true);
      }
      else {
        layers_.push_back(LayerRegistry<Dtype>::CreateLayer(layer_param));
      }
      layer_names_.push_back(layer_param.name());
      LOG_IF(INFO, root_solver())
        << "Creating Layer " << layer_param.name();
      bool need_backward = false;
      // Figure out this layer's input and output
      for (int bottom_id = 0; bottom_id < layer_param.bottom_size();
        ++bottom_id) {
        const int blob_id = AppendBottom(param, layer_id, bottom_id,
          &available_blobs, &blob_name_to_idx);
        // If a blob needs backward, this layer should provide it.
        need_backward |= blob_need_backward_[blob_id];
      }
      int num_top = layer_param.top_size();
      for (int top_id = 0; top_id < num_top; ++top_id) {
        AppendTop(param, layer_id, top_id, &available_blobs, &blob_name_to_idx);
        // Collect Input layer tops as Net inputs.
        if (layer_param.type() == "Input") {
          const int blob_id = blobs_.size() - 1;
          net_input_blob_indices_.push_back(blob_id);
          net_input_blobs_.push_back(blobs_[blob_id].get());
        }
      }
      // If the layer specifies that AutoTopBlobs() -> true and the LayerParameter
      // specified fewer than the required number (as specified by
      // ExactNumTopBlobs() or MinTopBlobs()), allocate them here.
      Layer<Dtype>* layer = layers_[layer_id].get();
      if (layer->AutoTopBlobs()) {
        const int needed_num_top =
          std::max(layer->MinTopBlobs(), layer->ExactNumTopBlobs());
        for (; num_top < needed_num_top; ++num_top) {
          // Add "anonymous" top blobs -- do not modify available_blobs or
          // blob_name_to_idx as we don't want these blobs to be usable as input
          // to other layers.
          AppendTop(param, layer_id, num_top, NULL, NULL);
        }
      }
      // After this layer is connected, set it up.
      if (share_from_root) {
        // Set up size of top blobs using root_net_
        const vector<Blob<Dtype>*> & base_top = root_net_->top_vecs_[layer_id];
        const vector<Blob<Dtype>*> & this_top = this->top_vecs_[layer_id];
        for (int top_id = 0; top_id < base_top.size(); ++top_id) {
          this_top[top_id]->ReshapeLike(*base_top[top_id]);
          LOG(INFO) << "Created top blob " << top_id << " (shape: "
            << this_top[top_id]->shape_string() << ") for shared layer "
            << layer_param.name();
        }
      }
      else {
        layers_[layer_id]->SetUp(bottom_vecs_[layer_id], top_vecs_[layer_id]);
      }
      LOG_IF(INFO, root_solver())
        << "Setting up " << layer_names_[layer_id];
      for (int top_id = 0; top_id < top_vecs_[layer_id].size(); ++top_id) {
        if (blob_loss_weights_.size() <= top_id_vecs_[layer_id][top_id]) {
          blob_loss_weights_.resize(top_id_vecs_[layer_id][top_id] + 1, Dtype(0));
        }
        blob_loss_weights_[top_id_vecs_[layer_id][top_id]] = layer->loss(top_id);
        LOG_IF(INFO, root_solver())
          << "Top shape: " << top_vecs_[layer_id][top_id]->shape_string();
        if (layer->loss(top_id)) {
          LOG_IF(INFO, root_solver())
            << "    with loss weight " << layer->loss(top_id);
        }
        memory_used_ += top_vecs_[layer_id][top_id]->count();
      }
      LOG_IF(INFO, root_solver())
        << "Memory required for data: " << memory_used_ * sizeof(Dtype);
      const int param_size = layer_param.param_size();
      const int num_param_blobs = layers_[layer_id]->blobs().size();
      CHECK_LE(param_size, num_param_blobs)
        << "Too many params specified for layer " << layer_param.name();
      ParamSpec default_param_spec;
      for (int param_id = 0; param_id < num_param_blobs; ++param_id) {
        const ParamSpec* param_spec = (param_id < param_size) ?
          &layer_param.param(param_id) : &default_param_spec;
        const bool param_need_backward = param_spec->lr_mult() != 0;
        need_backward |= param_need_backward;
        layers_[layer_id]->set_param_propagate_down(param_id,
          param_need_backward);
      }
      for (int param_id = 0; param_id < num_param_blobs; ++param_id) {
        AppendParam(param, layer_id, param_id);
      }
      // Finally, set the backward flag
      layer_need_backward_.push_back(need_backward);
      if (need_backward) {
        for (int top_id = 0; top_id < top_id_vecs_[layer_id].size(); ++top_id) {
          blob_need_backward_[top_id_vecs_[layer_id][top_id]] = true;
        }
      }
    }
    // Go through the net backwards to determine which blobs contribute to the
    // loss.  We can skip backward computation for blobs that don't contribute
    // to the loss.
    // Also checks if all bottom blobs don't need backward computation (possible
    // because the skip_propagate_down param) and so we can skip bacward
    // computation for the entire layer
    set<string> blobs_under_loss;
    set<string> blobs_skip_backp;
    for (int layer_id = layers_.size() - 1; layer_id >= 0; --layer_id) {
      bool layer_contributes_loss = false;
      bool layer_skip_propagate_down = true;
      for (int top_id = 0; top_id < top_vecs_[layer_id].size(); ++top_id) {
        const string & blob_name = blob_names_[top_id_vecs_[layer_id][top_id]];
        if (layers_[layer_id]->loss(top_id) ||
          (blobs_under_loss.find(blob_name) != blobs_under_loss.end())) {
          layer_contributes_loss = true;
        }
        if (blobs_skip_backp.find(blob_name) == blobs_skip_backp.end()) {
          layer_skip_propagate_down = false;
        }
        if (layer_contributes_loss && !layer_skip_propagate_down) {
          break;
        }
      }
      // If this layer can skip backward computation, also all his bottom blobs
      // don't need backpropagation
      if (layer_need_backward_[layer_id] && layer_skip_propagate_down) {
        layer_need_backward_[layer_id] = false;
        for (int bottom_id = 0; bottom_id < bottom_vecs_[layer_id].size();
          ++bottom_id) {
          bottom_need_backward_[layer_id][bottom_id] = false;
        }
      }
      if (!layer_contributes_loss) { layer_need_backward_[layer_id] = false; }
      if (root_solver()) {
        if (layer_need_backward_[layer_id]) {
          LOG(INFO) << layer_names_[layer_id] << " needs backward computation.";
        }
        else {
          LOG(INFO) << layer_names_[layer_id]
            << " does not need backward computation.";
        }
      }
      for (int bottom_id = 0; bottom_id < bottom_vecs_[layer_id].size();
        ++bottom_id) {
        if (layer_contributes_loss) {
          const string & blob_name =
            blob_names_[bottom_id_vecs_[layer_id][bottom_id]];
          blobs_under_loss.insert(blob_name);
        }
        else {
          bottom_need_backward_[layer_id][bottom_id] = false;
        }
        if (!bottom_need_backward_[layer_id][bottom_id]) {
          const string & blob_name =
            blob_names_[bottom_id_vecs_[layer_id][bottom_id]];
          blobs_skip_backp.insert(blob_name);
        }
      }
    }
    // Handle force_backward if needed.
    if (param.force_backward()) {
      for (int layer_id = 0; layer_id < layers_.size(); ++layer_id) {
        layer_need_backward_[layer_id] = true;
        for (int bottom_id = 0;
          bottom_id < bottom_need_backward_[layer_id].size(); ++bottom_id) {
          bottom_need_backward_[layer_id][bottom_id] =
            bottom_need_backward_[layer_id][bottom_id] ||
            layers_[layer_id]->AllowForceBackward(bottom_id);
          blob_need_backward_[bottom_id_vecs_[layer_id][bottom_id]] =
            blob_need_backward_[bottom_id_vecs_[layer_id][bottom_id]] ||
            bottom_need_backward_[layer_id][bottom_id];
        }
        for (int param_id = 0; param_id < layers_[layer_id]->blobs().size();
          ++param_id) {
          layers_[layer_id]->set_param_propagate_down(param_id, true);
        }
      }
    }
    // In the end, all remaining blobs are considered output blobs.
    for (set<string>::iterator it = available_blobs.begin();
      it != available_blobs.end(); ++it) {
      LOG_IF(INFO, root_solver())
        << "This network produces output " << *it;
      net_output_blobs_.push_back(blobs_[blob_name_to_idx[*it]].get());
      net_output_blob_indices_.push_back(blob_name_to_idx[*it]);
    }
    for (size_t blob_id = 0; blob_id < blob_names_.size(); ++blob_id) {
      blob_names_index_[blob_names_[blob_id]] = blob_id;
    }
    for (size_t layer_id = 0; layer_id < layer_names_.size(); ++layer_id) {
      layer_names_index_[layer_names_[layer_id]] = layer_id;
    }
    ShareWeights();
    debug_info_ = param.debug_info();
    LOG_IF(INFO, root_solver()) << "Network initialization done.";
  }

  /**
   * @brief Run Forward and return the result.
   *
   */
  const vector<Blob<Dtype>*> & Forward(Dtype* loss = NULL);
  /// @brief DEPRECATED; use Forward() instead.
  const vector<Blob<Dtype>*> & ForwardPrefilled(Dtype* loss = NULL) {
    LOG_EVERY_N(WARNING, 1000) << "DEPRECATED: ForwardPrefilled() "
      << "will be removed in a future version. Use Forward().";
    return Forward(loss);
  }

  /**
   * The From and To variants of Forward and Backward operate on the
   * (topological) ordering by which the net is specified. For general DAG
   * networks, note that (1) computing from one layer to another might entail
   * extra computation on unrelated branches, and (2) computation starting in
   * the middle may be incorrect if all of the layers of a fan-in are not
   * included.
   */
  Dtype ForwardFromTo(int start, int end);
  Dtype ForwardFrom(int start);
  Dtype ForwardTo(int end);
  /// @brief DEPRECATED; set input blobs then use Forward() instead.
  const vector<Blob<Dtype>*> & Forward(const vector<Blob<Dtype>* > & bottom,
    Dtype* loss = NULL);

  /**
   * @brief Zeroes out the diffs of all net parameters.
   *        Should be run before Backward.
   */
  void ClearParamDiffs();

  /**
   * The network backward should take no input and output, since it solely
   * computes the gradient w.r.t the parameters, and the data has already been
   * provided during the forward pass.
   */
  void Backward();
  void BackwardFromTo(int start, int end);
  void BackwardFrom(int start);
  void BackwardTo(int end);

  /**
   * @brief Reshape all layers from bottom to top.
   *
   * This is useful to propagate changes to layer sizes without running
   * a forward pass, e.g. to compute output feature size.
   */
  void Reshape();

  Dtype ForwardBackward() {
    Dtype loss;
    Forward(&loss);
    Backward();
    return loss;
  }

  /// @brief Updates the network weights based on the diff values computed.
  void Update();
  /**
   * @brief Shares weight data of owner blobs with shared blobs.
   *
   * Note: this is called by Net::Init, and thus should normally not be
   * called manually.
   */
  void ShareWeights();

  /**
   * @brief For an already initialized net, implicitly copies (i.e., using no
   *        additional memory) the pre-trained layers from another Net.
   */
  void ShareTrainedLayersWith(const Net* other);
  // For an already initialized net, CopyTrainedLayersFrom() copies the already
  // trained layers from another net parameter instance.
  /**
   * @brief For an already initialized net, copies the pre-trained layers from
   *        another Net.
   */
  void CopyTrainedLayersFrom(CJSON* param);
  void CopyTrainedLayersFrom(const string trained_filename);
  void CopyTrainedLayersFromBinaryProto(const string trained_filename);
  void CopyTrainedLayersFromHDF5(const string trained_filename);
  /// @brief Writes the net to a proto.
  void ToProto(CJSON* param, bool write_diff = false) const;
  /// @brief Writes the net to an HDF5 file.
  void ToHDF5(const string & filename, bool write_diff = false) const;

  /// @brief returns the network name.
  inline const string & name() const { return name_; }
  /// @brief returns the layer names
  inline const vector<string> & layer_names() const { return layer_names_; }
  /// @brief returns the blob names
  inline const vector<string> & blob_names() const { return blob_names_; }
  /// @brief returns the blobs
  inline const vector<Blob<Dtype>* > & blobs() const {
    return blobs_;
  }
  /// @brief returns the layers
  inline const vector<Layer<Dtype>* > & layers() const {
    return layers_;
  }
  /// @brief returns the phase: TRAIN or TEST
  inline Phase phase() const { return phase_; }
  /**
   * @brief returns the bottom vecs for each layer -- usually you won't
   *        need this unless you do per-layer checks such as gradients.
   */
  inline const vector<vector<Blob<Dtype>*> > & bottom_vecs() const {
    return bottom_vecs_;
  }
  /**
   * @brief returns the top vecs for each layer -- usually you won't
   *        need this unless you do per-layer checks such as gradients.
   */
  inline const vector<vector<Blob<Dtype>*> > & top_vecs() const {
    return top_vecs_;
  }
  /// @brief returns the ids of the top blobs of layer i
  inline const vector<int> & top_ids(int i) const {
    CHECK_GE(i, 0) << "Invalid layer id";
    CHECK_LT(i, top_id_vecs_.size()) << "Invalid layer id";
    return top_id_vecs_[i];
  }
  /// @brief returns the ids of the bottom blobs of layer i
  inline const vector<int> & bottom_ids(int i) const {
    CHECK_GE(i, 0) << "Invalid layer id";
    CHECK_LT(i, bottom_id_vecs_.size()) << "Invalid layer id";
    return bottom_id_vecs_[i];
  }
  inline const vector<vector<bool> > & bottom_need_backward() const {
    return bottom_need_backward_;
  }
  inline const vector<Dtype> & blob_loss_weights() const {
    return blob_loss_weights_;
  }
  inline const vector<bool> & layer_need_backward() const {
    return layer_need_backward_;
  }
  /// @brief returns the parameters
  inline const vector<Blob<Dtype>* > & params() const {
    return params_;
  }
  inline const vector<Blob<Dtype>*> & learnable_params() const {
    return learnable_params_;
  }
  /// @brief returns the learnable parameter learning rate multipliers
  inline const vector<float> & params_lr() const { return params_lr_; }
  inline const vector<bool> & has_params_lr() const { return has_params_lr_; }
  /// @brief returns the learnable parameter decay multipliers
  inline const vector<float> & params_weight_decay() const {
    return params_weight_decay_;
  }
  inline const vector<bool> & has_params_decay() const {
    return has_params_decay_;
  }
  const map<string, int> & param_names_index() const {
    return param_names_index_;
  }
  int layer_index_by_name(const string & name) {
    int idx = -1;
    if (has_layer(name)) {
      idx = layer_names_index_[name];
    }
    return idx;
  }
  inline const vector<int> & param_owners() const { return param_owners_; }
  inline const vector<string> & param_display_names() const {
    return param_display_names_;
  }
  /// @brief Input and output blob numbers
  inline int num_inputs() const { return (int)net_input_blobs_.size(); }
  inline int num_outputs() const { return (int)net_output_blobs_.size(); }
  inline const vector<Blob<Dtype>*> & input_blobs() const {
    return net_input_blobs_;
  }
  inline const vector<Blob<Dtype>*> & output_blobs() const {
    return net_output_blobs_;
  }
  inline const vector<int> & input_blob_indices() const {
    return net_input_blob_indices_;
  }
  inline const vector<int> & output_blob_indices() const {
    return net_output_blob_indices_;
  }
  bool has_blob(const string & blob_name) const;
  const Blob<Dtype>* blob_by_name(const string & blob_name) const;
  bool has_layer(const string & layer_name) const;
  const Layer<Dtype>* layer_by_name(const string & layer_name) const;

  void set_debug_info(const bool value) { debug_info_ = value; }

  // Helpers for Init.
  /**
   * @brief Remove layers that the user specified should be excluded given the current
   *        phase, level, and stage.
   */
  static void FilterNet(CJSON* param,
    CJSON* param_filtered) {
    NetState net_state(param.state());
    param_filtered->CopyFrom(param);
    param_filtered->clear_layer();
    for (int i = 0; i < param.layer_size(); ++i) {
      CJSON* layer_param = param.layer(i);
      const string & layer_name = layer_param.name();
      CHECK(layer_param.include_size() == 0 || layer_param.exclude_size() == 0)
        << "Specify either include rules or exclude rules; not both.";
      // If no include rules are specified, the layer is included by default and
      // only excluded if it meets one of the exclude rules.
      bool layer_included = (layer_param.include_size() == 0);
      for (int j = 0; layer_included && j < layer_param.exclude_size(); ++j) {
        if (StateMeetsRule(net_state, layer_param.exclude(j), layer_name)) {
          layer_included = false;
        }
      }
      for (int j = 0; !layer_included && j < layer_param.include_size(); ++j) {
        if (StateMeetsRule(net_state, layer_param.include(j), layer_name)) {
          layer_included = true;
        }
      }
      if (layer_included) {
        param_filtered->add_layer()->CopyFrom(layer_param);
      }
    }
  }
  /// @brief return whether NetState state meets NetStateRule rule
  static bool StateMeetsRule(CJSON* state, CJSON* rule,
    const string & layer_name);

protected:
  // Helpers for Init.
  /// @brief Append a new top blob to the net.
  void AppendTop(CJSON* param, const int layer_id,
    const int top_id, set<string>* available_blobs,
    map<string, int>* blob_name_to_idx);
  /// @brief Append a new bottom blob to the net.
  int AppendBottom(CJSON* param, const int layer_id,
    const int bottom_id, set<string>* available_blobs,
    map<string, int>* blob_name_to_idx);
  /// @brief Append a new parameter blob to the net.
  void AppendParam(CJSON* param, const int layer_id,
    const int param_id);

  /// @brief Helper for displaying debug info in Forward.
  void ForwardDebugInfo(const int layer_id);
  /// @brief Helper for displaying debug info in Backward.
  void BackwardDebugInfo(const int layer_id);
  /// @brief Helper for displaying debug info in Update.
  void UpdateDebugInfo(const int param_id);

  /// @brief The network name
  string name_;
  /// @brief The phase: TRAIN or TEST
  Phase phase_;
  /// @brief Individual layers in the net
  vector<Layer<Dtype>* > layers_;
  vector<string> layer_names_;
  map<string, int> layer_names_index_;
  vector<bool> layer_need_backward_;
  /// @brief the blobs storing intermediate results between the layer.
  vector<Blob<Dtype>* > blobs_;
  vector<string> blob_names_;
  map<string, int> blob_names_index_;
  vector<bool> blob_need_backward_;
  /// bottom_vecs stores the vectors containing the input for each layer.
  /// They don't actually host the blobs (blobs_ does), so we simply store
  /// pointers.
  vector<vector<Blob<Dtype>*> > bottom_vecs_;
  vector<vector<int> > bottom_id_vecs_;
  vector<vector<bool> > bottom_need_backward_;
  /// top_vecs stores the vectors containing the output for each layer
  vector<vector<Blob<Dtype>*> > top_vecs_;
  vector<vector<int> > top_id_vecs_;
  /// Vector of weight in the loss (or objective) function of each net blob,
  /// indexed by blob_id.
  vector<Dtype> blob_loss_weights_;
  vector<vector<int> > param_id_vecs_;
  vector<int> param_owners_;
  vector<string> param_display_names_;
  vector<pair<int, int> > param_layer_indices_;
  map<string, int> param_names_index_;
  /// blob indices for the input and the output of the net
  vector<int> net_input_blob_indices_;
  vector<int> net_output_blob_indices_;
  vector<Blob<Dtype>*> net_input_blobs_;
  vector<Blob<Dtype>*> net_output_blobs_;
  /// The parameters in the network.
  vector<Blob<Dtype>* > params_;
  vector<Blob<Dtype>*> learnable_params_;
  /**
   * The mapping from params_ -> learnable_params_: we have
   * learnable_param_ids_.size() == params_.size(),
   * and learnable_params_[learnable_param_ids_[i]] == params_[i].get()
   * if and only if params_[i] is an "owner"; otherwise, params_[i] is a sharer
   * and learnable_params_[learnable_param_ids_[i]] gives its owner.
   */
  vector<int> learnable_param_ids_;
  /// the learning rate multipliers for learnable_params_
  vector<float> params_lr_;
  vector<bool> has_params_lr_;
  /// the weight decay multipliers for learnable_params_
  vector<float> params_weight_decay_;
  vector<bool> has_params_decay_;
  /// The bytes of memory used by this net
  size_t memory_used_;
  /// Whether to compute and display debug info for the net.
  bool debug_info_;
  /// The root net that actually holds the shared layers in data parallelism
  const Net* const root_net_;
  DISABLE_COPY_AND_ASSIGN(Net);
};




#endif  // CAFFE_NET_HPP_
