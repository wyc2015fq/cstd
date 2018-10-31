
int AppendName(Layer<Dtype>* layer, bool is_top, vector<Blob<Dtype>*>& net_blobs_) {
  vector<string> vec;
  const char* name = is_top ? "top" : "bottom";
  cJSON_GetObjectStringArray(layer->param_, name, vec);
  Blob<Dtype>* bi;
  vector<Blob<Dtype>*>& blobvec = is_top ? layer->top_vecs_ : layer->bottom_vecs_;
  for (int i = 0; i < vec.size(); ++i) {
    if (blobs_count(net_blobs_, vec[i]) == 0) {
      bi = blobs_add(net_blobs_, vec[i]);
    }
    else {
      bi = blobs_get(net_blobs_, vec[i]);
    }
    if (is_top) {
      bi->top_cnt_++;
    }
    else {
      bi->bottom_cnt_++;
    }
    blobvec.push_back(bi);
  }
  return 0;
}

int FromProto(CJSON* param, vector<Blob<Dtype>*>& net_blobs_) {
  auto layer = this;
  CJSON* blobs_json = param->GetObjectItem("blobs");
  layer->param_ = param;
  //layer->loss_weight_ = param->GetObjectNumber("loss_weight", 0);
  this->phase_ = CJSON_GETOBJECTENUM(param_, "phase", TRAINorTEST, Phase);
  if (blobs_json) {
    int blob_size = blobs_json->GetArraySize();
    layer->reset(blob_size);
    for (int j = 0; j < blob_size; ++j) {
      CJSON* blob_json = blobs_json->GetArrayItem(j);
      layer->blobs_[j]->FromProto(blob_json);
    }
  }
  AppendName(layer, false, net_blobs_);
  AppendName(layer, true, net_blobs_);
  {
    cJSON* item = cJSON_GetObjectItem(layer->param_, "loss_weight");
    int loss_weight_size = item ? cJSON_GetArraySize(item) : 0;
    has_loss_weights_ = 0;
    if (loss_weight_size == layer->top_vecs_.size()) {
      for (int j = 0; j < top_vecs_.size(); ++j) {
        blobs_[j]->loss_weight_ = cJSON_GetArrayItem(item, j)->valuedouble;
        if (blobs_[j]->loss_weight_ > 0) {
          ++has_loss_weights_;
        }
      }
    }
    else {
      for (int j = 0; j < layer->top_vecs_.size(); ++j) {
        blobs_[j]->loss_weight_ = (layer->top_vecs_[j]->bottom_cnt_ == 0) ? 1 : 0;
        if (blobs_[j]->loss_weight_ > 0) {
          ++has_loss_weights_;
        }
      }
    }
  }
  {
    cJSON* item = cJSON_GetObjectItem(layer->param_, "propagate_down");
  }
  return 0;
}

inline void SetLossWeights(const vector<Blob<Dtype>*> & top, vector<Dtype>& lossvec) {
  vector<double> loss_weight_arr;
  cJSON_GetObjectNumberArray(param_, "loss_weight", loss_weight_arr);
  if (loss_weight_arr.size()) {
    CHECK_EQ(top.size(), loss_weight_arr.size()) << "loss_weight must be "
      "unspecified or specified once per top blob.";
    for (int top_id = 0; top_id < top.size(); ++top_id) {
      const Dtype loss_weight = loss_weight_arr[top_id];
      if (loss_weight == Dtype(0)) { continue; }
      this->set_loss(top_id, loss_weight);
      const int count = top[top_id]->count();
      Dtype* loss_multiplier = top[top_id]->mutable_cpu_diff();
      caffe_set(count, loss_weight, loss_multiplier);
    }
  }
}

void CheckBlobCounts(const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top) {
#if 0
  if (ExactNumBottomBlobs() >= 0) {
    CHECK_EQ(ExactNumBottomBlobs(), bottom.size())
      << type() << " Layer takes " << ExactNumBottomBlobs()
      << " bottom blob(s) as input.";
  }
  if (MinBottomBlobs() >= 0) {
    CHECK_LE(MinBottomBlobs(), bottom.size())
      << type() << " Layer takes at least " << MinBottomBlobs()
      << " bottom blob(s) as input.";
  }
  if (MaxBottomBlobs() >= 0) {
    CHECK_GE(MaxBottomBlobs(), bottom.size())
      << type() << " Layer takes at most " << MaxBottomBlobs()
      << " bottom blob(s) as input.";
  }
  if (ExactNumTopBlobs() >= 0) {
    CHECK_EQ(ExactNumTopBlobs(), top.size())
      << type() << " Layer produces " << ExactNumTopBlobs()
      << " top blob(s) as output.";
  }
  if (MinTopBlobs() >= 0) {
    CHECK_LE(MinTopBlobs(), top.size())
      << type() << " Layer produces at least " << MinTopBlobs()
      << " top blob(s) as output.";
  }
  if (MaxTopBlobs() >= 0) {
    CHECK_GE(MaxTopBlobs(), top.size())
      << type() << " Layer produces at most " << MaxTopBlobs()
      << " top blob(s) as output.";
  }
  if (EqualNumBottomTopBlobs()) {
    CHECK_EQ(bottom.size(), top.size())
      << type() << " Layer produces one top blob as output for each "
      << "bottom blob input.";
  }
#endif
}

void SetUp(const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top) {
  InitMutex();
  CheckBlobCounts(bottom, top);
  LayerSetUp(bottom, top);
  Reshape(bottom, top);
  //SetLossWeights(layer, top);
}

double Forward(const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top )
{
  // Lock during forward to ensure sequential forward
  Lock();
  Dtype loss = 0;
  Reshape(bottom, top);
  Forward(CONTEXT,bottom, top);
#if 0
  if (bottom.size() > 0 && top.size() > 0) {
    Dtype* bottom_data = bottom[0]->mutable_cpu_data();
    Dtype* top_data = top[0]->mutable_cpu_data();
    printf("############## %s ###########\n", this->layer_param().name().c_str());
    for (int i = 0; i < 10; i++) {
      printf("%f ", bottom_data[i]);
    }
    printf("\n");
    for (int i = 0; i < 10; i++) {
      printf("%f ", top_data[i]);
    }
    printf("\n");
  }
#endif
  if (has_loss_weights_) {
    for (int top_id = 0; top_id < top.size(); ++top_id) {
      //if (!this->loss(top_id)) { continue; }
      loss += top[top_id]->Loss();
    }
  }

  Unlock();

  if (debug_info_) {
    for (int top_id = 0; top_id < top.size(); ++top_id) {
      const Blob<Dtype> & blob = *top[top_id];
      const string & blob_name = blob.name;
      const Dtype data_abs_val_mean = blob.asum_data() / blob.count();
      LOG_IF(INFO, root_solver())
        << "    [Forward] "
        << "Layer " << name
        << ", top blob " << blob_name
        << " data: " << data_abs_val_mean;
    }
    for (int param_id = 0; param_id < blobs_.size();
      ++param_id) {
      const Blob<Dtype> & blob = *blobs_[param_id];
      const string & blob_name = blob.name;
      const Dtype data_abs_val_mean = blob.asum_data() / blob.count();
      LOG_IF(INFO, root_solver())
        << "    [Forward] "
        << "Layer " << name
        << ", param blob " << blob_name
        << " data: " << data_abs_val_mean;
    }
  }

  return loss;
}


inline void Backward(const vector<Blob<Dtype>*> & top,
  const vector<bool> & propagate_down,
  const vector<Blob<Dtype>*> & bottom)
{
  Backward(CONTEXT, top, propagate_down, bottom);

  if (debug_info_) {
    for (int top_id = 0; top_id < top.size(); ++top_id) {
      const Blob<Dtype> & blob = *top[top_id];
      const string & blob_name = top[top_id]->name;
      const Dtype data_abs_val_mean = blob.asum_data() / blob.count();
      LOG_IF(INFO, root_solver())
        << "    [Forward] "
        << "Layer " << name
        << ", top blob " << blob_name
        << " data: " << data_abs_val_mean;
    }
    for (int param_id = 0; param_id < blobs_.size(); ++param_id) {
      const Blob<Dtype> & blob = *blobs_[param_id];
      const string & blob_name = blob.name;
      const Dtype data_abs_val_mean = blob.asum_data() / blob.count();
      LOG_IF(INFO, root_solver())
        << "    [Forward] "
        << "Layer " << name
        << ", param blob " << blob_name
        << " data: " << data_abs_val_mean;
    }
  }

}
