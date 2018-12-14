
int FromProto(CJSON* param, vector<Blob*>& net_blobs_) {
  Layer* layer = this;
  CJSON* blobs_json = param->GetObjectItem("blobs");
  //layer->param_ = param;
  strncpy(layer->name_, param->getstring("name", ""), MAX_NAME);
  //layer->loss_weight_ = param->GetObjectNumber("loss_weight", 0);
  if (param->has("phase")) {
    Phase phase = (Phase)param->getenum("phase", TEST, Phase_Name, countof(Phase_Name));
    layer->phase_mask_ = 1 << phase;
  }
  else {
  }
  if (blobs_json) {
    int blob_size = blobs_json->GetArraySize();
    layer->reset(blob_size);
    for (int j = 0; j < blob_size; ++j) {
      CJSON* blob_json = blobs_json->GetArrayItem(j);
      layer->blobs_[j]->FromProto(blob_json);
    }
  }
  AppendName(param, layer, false, net_blobs_);
  AppendName(param, layer, true, net_blobs_);
  {
    cJSON* item = cJSON_GetObjectItem(param, "loss_weight");
    int loss_weight_size = item ? cJSON_GetArraySize(item) : 0;

    if (loss_weight_size == layer->top_vecs_.size()) {
      for (int j = 0; j < top_vecs_.size(); ++j) {
        top_vecs_[j]->loss_weight_ = cJSON_GetArrayItem(item, j)->valuedouble;
      }
    }
  }
  {
    //cJSON* item = cJSON_GetObjectItem(layer->param_, "propagate_down");
  }
  return 0;
}
#if 0
inline void SetLossWeights(const vector<Blob*> & top, vector<Dtype>& lossvec) {
  vector<double> loss_weight_arr;
  cJSON_GetObjectNumberArray(param_, "loss_weight", loss_weight_arr);
  if (loss_weight_arr.size()) {
    CHECK_EQ(top.size(), loss_weight_arr.size()) << "loss_weight must be "
      "unspecified or specified once per top blob->";
    for (int top_id = 0; top_id < top.size(); ++top_id) {
      const Dtype loss_weight = loss_weight_arr[top_id];
      if (loss_weight == Dtype(0)) { continue; }
      this->set_loss(top_id, loss_weight);
      const int count = top[top_id]->count();
      Dtype* loss_multiplier = top[top_id]->cpu_mdiff();
      caffe_set(count, loss_weight, loss_multiplier);
    }
  }
}
#endif

void CheckBlobCounts(const vector<Blob*> & bottom, const vector<Blob*> & top) {
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

void SetUp(const vector<Blob*> & bottom, const vector<Blob*> & top) {
  InitMutex();
  CheckBlobCounts(bottom, top);
  LayerSetUp(bottom, top);
  Reshape(bottom, top);
  //SetLossWeights(layer, top);
}

double Forward(const vector<Blob*> & bottom, const vector<Blob*> & top )
{
  // Lock during forward to ensure sequential forward
  Lock();
  Dtype loss = 0;
  utime_start(a);
  Reshape(bottom, top);
  Forward_(bottom, top);
  double t = utime_elapsed(a);
#if 0
  if (bottom.size() > 0 && top.size() > 0) {
    Dtype* bottom_data = bottom[0]->cpu_mdata();
    Dtype* top_data = top[0]->cpu_mdata();
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
  for (int top_id = 0; top_id < top.size(); ++top_id) {
    //if (!this->loss(top_id)) { continue; }
    loss += top[top_id]->Loss();
  }

  Unlock();
#ifdef _DEBUG
  //debug_info_ = 2;
#endif
  if (debug_info_) {
    char buf[256];
    LOG_IF(INFO, root_solver()) << "  [Forward] " << type_ << "Layer " << name_ << "(" << t << ")";
    if (debug_info_ > 1) {
      for (int i = 0; i < top.size(); ++i) {
        LOG_IF(INFO, root_solver()) << "    top blob " << i << " " << top[i]->to_debug_string(buf, 256);
      }
      if (debug_info_ > 2) {
        for (int i = 0; i < blobs_.size(); ++i) {
          LOG_IF(INFO, root_solver()) << "    param blob " << i << " " << blobs_[i]->to_debug_string(buf, 256);
        }
      }
    }
  }

  return loss;
}


inline void Backward(const vector<Blob*> & top, const vector<Blob*> & bottom)
{
  Backward_(top, bottom);

  if (debug_info_) {
    char buf[256];
    LOG_IF(INFO, root_solver()) << "  [Backward] " << type_ << "Layer " << name_;
    for (int i = 0; i < bottom.size(); ++i) {
      LOG_IF(INFO, root_solver()) << "    bottom blob " << i << " " << bottom[i]->to_debug_string(buf, 256);
    }
    for (int i = 0; i < blobs_.size(); ++i) {
      LOG_IF(INFO, root_solver()) << "    param blob " << i << " " << blobs_[i]->to_debug_string(buf, 256);
    }
  }

}
