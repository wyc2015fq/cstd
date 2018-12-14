
int learnable_params(vector<Blob* >& out) {
  out.resize(0);
  for (int i = 0; i < layers_.size(); ++i) {
    for (int j = 0; j < layers_[i]->blobs_.size(); ++j) {
      out.push_back(layers_[i]->blobs_[j]);
    }
  }
  return (int)out.size();
}


int SetUp() {
  Net* net = this;
  for (int i = 0; i < layers_.size(); ++i) {
    // LOG(ERROR) << "Forwarding " << layer_names_[i];
    Layer* layer = net->layers_[i];
    layer->SetUp(layer->bottom_vecs_, layer->top_vecs_);
  }
  return 0;
}

void Reshape() {}
bool has_layer(const char* name) {
  return NULL != blob_by_name(name);
}
int layer_index_by_name(const char* name) {
  for (int i = 0; i < blobs_.size(); ++i) {
    if (strcmp(name, blobs_[i]->name_) == 0) {
      return i;
    }
  }
  return -1;
}
Blob* blob_by_name(const char* name) {
  for (int i = 0; i < blobs_.size(); ++i) {
    if (strcmp(name, blobs_[i]->name_) == 0) {
      return blobs_[i];
    }
  }
  return NULL;
}

#if 0
Blob* get_blob(const char* name) {
  Blob* out = blob_by_name(name);
  assert(out);
  return 0;
}
#endif
Blob* output_blobs(int k) {
  int n = 0;
  for (int i = 0; i < blobs_.size(); ++i) {
    if (blobs_[i]->bottom_cnt_ == 0) {
      if (n == k) {
        return blobs_[i];
      }
      ++n;
    }
  }
  return NULL;
}
Blob* input_blobs(int i) {
  for (int i = 0; i < layers_.size(); ++i) {
    if (0 == strcmp("Input", layers_[i]->type_)) {
      return layers_[i]->top_vecs_[i];
    }
  }
  return NULL;
}
int num_outputs() {
  int n = 0;
  for (int i = 0; i < blobs_.size(); ++i) {
    if (blobs_[i]->bottom_cnt_ == 0) {
      ++n;
    }
  }
  return n;
}
int num_inputs() {
  for (int i = 0; i < layers_.size(); ++i) {
    if (0 == strcmp("Input", layers_[i]->type_)) {
      return layers_[i]->top_vecs_.size();
    }
  }
  return 0;
}
int FromJsonFile(const char* fn) {
  int ret = 0;
  CJSON* root = cJSON_OpenFile(fn);
  if (root) {
    ret = FromProto(root);
    cJSON_Delete(root);
  }
  return ret;
}
int FromProto(CJSON* param) {
  Net* net = this;
  CJSON* layers_json = param->GetObjectItem("layers");
  int ret = 0;
  //net->param_ = param;
  solver_param.init(param);
  if (layers_json) {
    int layer_size = layers_json->GetArraySize();
    //net->reset(n);
    layers_.resize(layer_size);
    for (int i = 0; i < layer_size; ++i) {
      CJSON* layer_json = layers_json->GetArrayItem(i);
      const char* type = layer_json->getstring("type", NULL);
      const char* name = layer_json->getstring("name", NULL);
      CreateLayer(layer_json, net->layers_[i], type);
      Layer* layer = net->layers_[i];
      layer->FromProto(layer_json, blobs_);
      layer->SetUp(layer->bottom_vecs_, layer->top_vecs_);
    }
    ret = 1;
  }
  //SetUp();
  return ret;
}

Dtype Forward(Phase phase) {
  Dtype loss = ForwardFromTo(phase, 0, layers_.size() - 1);
  return loss;
}

double ForwardFromTo(Phase phase, int start, int end)
{
  Net* net = this;
  CHECK_GE(start, 0);
  CHECK_LT(end, net->layers_.size());
  double loss = 0;
  vector<Dtype> loss_weight_arr;
  for (int i = start; i <= end; ++i) {
    // LOG(ERROR) << "Forwarding " << layer_names_[i];
    Layer* layer = net->layers_[i];
    if (layer->phase_mask_ & (1 << phase)) {
      double layer_loss = layer->Forward(layer->bottom_vecs_, layer->top_vecs_);
      loss += layer_loss;
    }
  }
  return loss;
}

void BackwardFromTo(int start, int end)
{
  Net* net = this;
  CHECK_GE(end, 0);
  CHECK_LT(start, layers_.size());
  for (int i = start; i >= end; --i) {
    Layer* layer = net->layers_[i];
    if (layer->phase_mask_ & (1<<TRAIN)) {
      layers_[i]->Backward(layer->top_vecs_, layer->bottom_vecs_);
    }
  }
}

void Backward()
{
  BackwardFromTo(layers_.size() - 1, 0);
  if (debug_info_) {
    Dtype asum_data = 0, asum_diff = 0, sumsq_data = 0, sumsq_diff = 0;
    vector<Blob* > learnable_params_;
    learnable_params(learnable_params_);
    for (int i = 0; i < learnable_params_.size(); ++i) {
      asum_data += learnable_params_[i]->asum_data();
      asum_diff += learnable_params_[i]->asum_diff();
      sumsq_data += learnable_params_[i]->sumsq_data();
      sumsq_diff += learnable_params_[i]->sumsq_diff();
    }
    const Dtype l2norm_data = sqrt(sumsq_data);
    const Dtype l2norm_diff = sqrt(sumsq_diff);
    LOG(ERROR) << "    [Backward] All net params (data, diff): "
      << "L1 norm = (" << asum_data << ", " << asum_diff << "); "
      << "L2 norm = (" << l2norm_data << ", " << l2norm_diff << ")";
  }
}

Dtype ForwardBackward() {
  Dtype loss = Forward(TRAIN);
  Backward();
  return loss;
}

void Update()
{
  vector<Blob*>  learnable_params_;
  learnable_params(learnable_params_);
  for (int i = 0; i < learnable_params_.size(); ++i) {
    learnable_params_[i]->Update();
  }
}
