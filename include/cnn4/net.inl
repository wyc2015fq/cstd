

int learnable_params(vector<Blob<Dtype>* >& out) {
  out.resize(0);
  for (int i = 0; i < layers_.size(); ++i) {
    for (int j = 0; j < layers_[i]->blobs_.size(); ++j) {
      out.push_back(layers_[i]->blobs_[j]);
    }
  }
  return out.size();
}


int SetUp() {
  Net<Dtype>* net = this;
  for (int i = 0; i < layers_.size(); ++i) {
    // LOG(ERROR) << "Forwarding " << layer_names_[i];
    Layer<Dtype>* layer = net->layers_[i];
    layer->SetUp(layer->bottom_vecs_, layer->top_vecs_);
  }
  return 0;
}

int FromProto(CJSON* param) {
  Net<Dtype>* net = this;
  CJSON* layers_json = param->GetObjectItem("layers");
  int ret = 0;
  net->param_ = param;
  if (layers_json) {
    int layer_size = layers_json->GetArraySize();
    //net->reset(n);
    layers_.resize(layer_size);
    for (int i = 0; i < layer_size; ++i) {
      CJSON* layer_json = layers_json->GetArrayItem(i);
      const char* type = layer_json->GetObjectString("type", NULL);
      CreateLayer(net->layers_[i], type);
      net->layers_[i]->FromProto(layer_json, blobs_);
    }
    ret = 1;
  }
  SetUp();
  return ret;
}

Dtype Forward(Phase phase) {
  Dtype loss = ForwardFromTo(phase, 0, layers_.size() - 1);
  return loss;
}

double ForwardFromTo(Phase phase, int start, int end)
{
  Net<Dtype>* net = this;
  CHECK_GE(start, 0);
  CHECK_LT(end, net->layers_.size());
  double loss = 0;
  vector<Dtype> loss_weight_arr;
  for (int i = start; i <= end; ++i) {
    // LOG(ERROR) << "Forwarding " << layer_names_[i];
    Layer<Dtype>* layer = net->layers_[i];
    if (layer->phase_ == TRAINorTEST || layer->phase_ == phase) {
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
    Layer<Dtype>* layer = net->layers_[i];
    if (layer->phase_ == TRAINorTEST || layer->phase_ == TRAIN) {
      layers_[i]->Backward(layer->top_vecs_, layer->bottom_vecs_);
    }
  }
}

void Backward()
{
  BackwardFromTo(layers_.size() - 1, 0);
  if (debug_info_) {
    Dtype asum_data = 0, asum_diff = 0, sumsq_data = 0, sumsq_diff = 0;
    vector<Blob<Dtype>* > learnable_params_;
    learnable_params(learnable_params_);
    for (int i = 0; i < learnable_params_.size(); ++i) {
      asum_data += learnable_params_[i]->asum_data();
      asum_diff += learnable_params_[i]->asum_diff();
      sumsq_data += learnable_params_[i]->sumsq_data();
      sumsq_diff += learnable_params_[i]->sumsq_diff();
    }
    const Dtype l2norm_data = std::sqrt(sumsq_data);
    const Dtype l2norm_diff = std::sqrt(sumsq_diff);
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
  vector<Blob<Dtype>*>  learnable_params_;
  learnable_params(learnable_params_);
  for (int i = 0; i < learnable_params_.size(); ++i) {
    learnable_params_[i]->Update();
  }
}
