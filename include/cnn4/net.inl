struct blob_info {
  int idx_;
  int top_cnt_;
  int bottom_cnt_;
  blob_info() {}
  blob_info(int idx) { idx_ = idx; bottom_cnt_ = top_cnt_ = 0; }
};

map<string, blob_info> name2int;

int AppendName(CJSON* layer_json, bool is_top, int& k) {
  vector<string> vec;
  const char* name = is_top ? "top" : "bottom";
  cJSON_GetObjectStringArray(layer_json, name, vec);
  for (int i = 0; i < vec.size(); ++i) {
    if (name2int.count(vec[i]) == 0) {
      name2int[vec[i]] = blob_info(k++);
    }
    blob_info& bi = name2int[vec[i]];
    if (is_top) {
      bi.top_cnt_++;
    }
    else {
      bi.bottom_cnt_++;
    }
  }
  return k;
}
int GetBlobs(CJSON* layer_json, bool is_top, vector<Blob<Dtype>*>& out) {
  vector<string> vec;
  const char* name = is_top ? "top" : "bottom";
  cJSON_GetObjectStringArray(layer_json, name, vec);
  out.resize(vec.size());
  for (int i = 0; i < vec.size(); ++i) {
    int idx = name2int[vec[i]].idx_;
    out[i] = blobs_[i];
  }
  return 0;
}
int FromProto(CJSON* param) {
  Net<Dtype>* net = this;
  CJSON* layers_json = param->GetObjectItem("layers");
  int ret = 0, k = 0;
  net->param_ = param;
  if (layers_json) {
    int n = layers_json->GetArraySize();
    net->reset(n);
    for (int i = 0; i < n; ++i) {
      CJSON* layer_json = layers_json->GetArrayItem(i);
      AppendName(layer_json, false, k);
      AppendName(layer_json, true, k);
      net->layers_[i]->FromProto(layer_json);
    }
    ret = 1;
  }
  blob_reset(blobs_, k);
  return ret;
}

Dtype Forward() {
  Dtype loss = ForwardFromTo(0, layers_.size() - 1);
  return loss;
}

double ForwardFromTo(int start, int end)
{
  Net<Dtype>* net = this;
  CHECK_GE(start, 0);
  CHECK_LT(end, net->layers_.size());
  double loss = 0;
  vector<Blob<Dtype>*> bottom_vecs_;
  vector<Blob<Dtype>*> top_vecs_;
  vector<Dtype> loss_weight_arr;
  for (int i = start; i <= end; ++i) {
    // LOG(ERROR) << "Forwarding " << layer_names_[i];
    Layer<Dtype>* layer = net->layers_[i];
    GetBlobs(layer->param_, false, bottom_vecs_);
    GetBlobs(layer->param_, true, top_vecs_);
    Dtype* loss_weights = 0;
    if (1) {
      cJSON_GetObjectNumberArray(layer->param_, "loss_weight", loss_weight_arr);
      if (loss_weight_arr.size()== top_vecs_.size()) {
        loss_weights = loss_weight_arr.data();
      }
      else {
        loss_weight_arr.resize(top_vecs_.size());
        for (int j = 0; j < top_vecs_.size(); ++j) {
          if (name2int[top_vecs_[j]->name].bottom_cnt_ == 0) {
            loss_weight_arr[j] = 1;
            loss_weights = loss_weight_arr.data();
          }
          else {
            loss_weight_arr[j] = 0;
          }
        }
      }
    }
    double layer_loss = layer->Forward(bottom_vecs_, top_vecs_, loss_weights);
    loss += layer_loss;
  }
  return loss;
}
