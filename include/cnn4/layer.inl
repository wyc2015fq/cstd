
int FromProto(CJSON* param) {
  auto layer = this;
  CJSON* blobs_json = param->GetObjectItem("blobs");
  layer->param_ = param;
  layer->loss_weight_ = param->GetObjectNumber("loss_weight", 0);
  this->phase_ = CJSON_GETOBJECTENUM(param_, "phase", TRAINorTEST, Phase);
  if (blobs_json) {
    int blob_size = blobs_json->GetArraySize();
    layer->reset(blob_size);
    for (int j = 0; j < blob_size; ++j) {
      CJSON* blob_json = blobs_json->GetArrayItem(j);
      layer->blobs_[j]->FromProto(blob_json);
    }
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

double Forward(const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top, const Dtype* loss_weights )
{
  // Lock during forward to ensure sequential forward
  Lock();
  Dtype loss = 0;
  Reshape(bottom, top);
  switch (mode()) {
  case CPU: {
    Forward_cpu(bottom, top);
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
    if (loss_weights) {
      for (int top_id = 0; top_id < top.size(); ++top_id) {
        //if (!this->loss(top_id)) { continue; }
        const int count = top[top_id]->count();
        const Dtype* data = top[top_id]->cpu_data();
        //const Dtype* loss_weights = top[top_id]->cpu_diff();
        Dtype* loss_multiplier = top[top_id]->mutable_cpu_diff();
        caffe_set(count, loss_weights[top_id], loss_multiplier);
        loss += caffe_dot<Dtype>(CPUCONTEXT, count, data, loss_multiplier);
      }
      loss_ = loss;
    }
  }
            break;
  case GPU:
    Forward_gpu(bottom, top);
#ifndef CPU_ONLY
    if (loss_weights) {
      for (int top_id = 0; top_id < top.size(); ++top_id) {
        //if (!this->loss(top_id)) { continue; }
        const int count = top[top_id]->count();
        const Dtype* data = top[top_id]->gpu_data();
        //const Dtype* loss_weights = top[top_id]->gpu_diff();
        Dtype* loss_multiplier = top[top_id]->mutable_cpu_diff();
        caffe_set(count, loss_weights[top_id], loss_multiplier);
        Dtype blob_loss = 0;
        caffe_dot<Dtype>(GPUCONTEXT, count, data, loss_multiplier, &blob_loss);
        loss += blob_loss;
      }
    }
#endif
    break;
  default:
    LOG(FATAL) << "Unknown caffe mode.";
  }
  Unlock();

  if (debug_info_) {
#if 0
    for (int top_id = 0; top_id < top.size(); ++top_id) {
      const Blob<Dtype> & blob = *top[top_id];
      const string & blob_name = blob.name;
      const Dtype data_abs_val_mean = blob.asum_data() / blob.count();
      LOG_IF(INFO, root_solver())
        << "    [Forward] "
        << "Layer " << layer_names_[layer_id]
        << ", top blob " << blob_name
        << " data: " << data_abs_val_mean;
    }
    for (int param_id = 0; param_id < layers_[layer_id]->blobs().size();
      ++param_id) {
      const Blob<Dtype> & blob = *layers_[layer_id]->blobs()[param_id];
      const int net_param_id = param_id_vecs_[layer_id][param_id];
      const string & blob_name = param_display_names_[net_param_id];
      const Dtype data_abs_val_mean = blob.asum_data() / blob.count();
      LOG_IF(INFO, root_solver())
        << "    [Forward] "
        << "Layer " << layer_names_[layer_id]
        << ", param blob " << blob_name
        << " data: " << data_abs_val_mean;
    }
#endif
  }

  return loss;
}

