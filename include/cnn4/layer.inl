
struct Layer {
  Phase phase_cur_;
  Phase phase_def_;
  char name_[MAX_NAME];
  char type_[MAX_NAME];
  //cJSON* param_;
  vector<Blob*> blobs_;
  vector<Blob*>* net_blobs_;
  vector<Blob*> bottom_vecs_;
  vector<Blob*> top_vecs_;

  virtual void fromJson(cjson* param) = 0;
  virtual void toJson(cjson* param) = 0;
  virtual inline const char* type() const { return ""; }
  virtual inline int ExactNumBottomBlobs() const { return -1; }
  virtual inline int MinBottomBlobs() const { return -1; }
  virtual inline int MaxBottomBlobs() const { return -1; }
  virtual inline int ExactNumTopBlobs() const { return -1; }
  virtual inline int MinTopBlobs() const { return -1; }
  virtual inline int MaxTopBlobs() const { return -1; }
  virtual inline bool EqualNumBottomTopBlobs() const { return false; }
  virtual inline bool AutoTopBlobs() const { return false; }
  virtual void LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top) { }
  virtual void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top) { }
  // bottom -> top
  virtual void Forward_(const vector<Blob*> & bottom, const vector<Blob*> & top) { assert(0); }
  // top -> bottom
  virtual void Backward_(const vector<Blob*> & top, const vector<Blob*> & bottom) { assert(0); }

  Layer() {
    init();
  }
  //Vtbl* vtbl;
  void init() {
    //memset(this, 0, sizeof(Layer));
    *name_ = 0;
    *type_ = 0;
    //vtbl = NULL;
  }
  void Free() {
    reset(0);
  }
  void reset(int blob_size) {
    blobs_reset(blobs_, blob_size);
  }
  typedef Layer* (*fun_type)();

  static fun_type reg(fun_type fun, const char* type) {
    static map<string, fun_type>  fmap;
    if (fun) {
      fmap[type] = fun;
    }
    if (type) {
      fun = fmap[type];
    }
    return fun;
  }
  int AppendName(cjson* param, bool is_top) {
    Layer* layer = this;
    vector<string> vec;
    const char* name = is_top ? "top" : "bottom";
    cjson_GetObjectStringArray(param, name, vec);
    Blob* bi;
    vector<Blob*>& blobvec = is_top ? layer->top_vecs_ : layer->bottom_vecs_;
    for (int i = 0; i < vec.size(); ++i) {
      bi = blobs_aget(*net_blobs_, vec[i].c_str());
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

  cjson* AddBlobNamesToJson(cjson* param, const char* name, const vector<Blob*>& blobs) {
    if (blobs.size() > 0) {
      cjson* top_json = cjson_AddArrayToObject(param, name);
      for (int i = 0; i < blobs.size(); ++i) {
        cjson_AddStringToArray(top_json, blobs[i]->name_);
      }
    }
    return NULL;
  }

  cjson* ToJson() {
    Layer* layer = this;
    cjson* param = cjson_CreateObject();
    cjson_AddStringToObject(param, "name", layer->name_);
    cjson_AddStringToObject(param, "type", layer->type_);
    AddBlobNamesToJson(param, "top", layer->top_vecs_);
    AddBlobNamesToJson(param, "bottom", layer->bottom_vecs_);
    if (phase_def_ !=TRAINorTEST) {
      cjson_AddObjectString(param, "phase", Phase_Name[phase_def_]);
    }
    toJson(param);
    int blob_size = (int)layer->blobs_.size();
    if (blob_size > 0) {
      cjson* blobs_json = cjson_AddObjectArray(param, "blobs");
      for (int j = 0; j < blob_size; ++j) {
        cjson* blob_json = layer->blobs_[j]->ToJson();
        cjson_AddItemToArray(blobs_json, blob_json);
      }
    }
    return param;
  }

  int FromJson(cjson* param) {
    Layer* layer = this;
    layer->fromJson(param);
    layer->AppendName(param, false);
    layer->AppendName(param, true);

    cjson* blobs_json = cjson_GetObjectItem(param, "blobs");
    cjson* phase_json = cjson_GetObjectItem(param, "phase");
    //layer->param_ = param;
    strncpy(layer->name_, cjson_GetObjectString(param, "name", ""), MAX_NAME);
    //layer->loss_weight_ = param->GetObjectNumber("loss_weight", 0);
    phase_def_ = TRAINorTEST;
    if (phase_json) {
      phase_def_ = (Phase)cjson_GetEnum(phase_json, TRAINorTEST, Phase_Name, countof(Phase_Name));
    }
    layer->SetUp(layer->bottom_vecs_, layer->top_vecs_);
    if (blobs_json) {
      int blob_size = cjson_GetArraySize(blobs_json);
      blob_size = MIN(blob_size, layer->blobs_.size());
      //layer->reset(blob_size);
      for (int j = 0; j < blob_size; ++j) {
        cjson* blob_json = cjson_GetArrayItem(blobs_json, j);
        layer->blobs_[j]->FromJson(blob_json);
      }
    }
    else {
      int blob_size = layer->blobs_.size();
      for (int j = 0; j < blob_size; ++j) {
        layer->blobs_[j]->Fill();
      }
    }
    {
      cjson* item = cjson_GetObjectItem(param, "loss_weight");
      int loss_weight_size = item ? cjson_GetArraySize(item) : 0;

      if (loss_weight_size == layer->top_vecs_.size()) {
        for (int j = 0; j < top_vecs_.size(); ++j) {
          top_vecs_[j]->loss_weight_ = cjson_GetArrayItem(item, j)->valuedouble;
        }
      }
    }
    {
      //cjson* item = cjson_GetObjectItem(layer->param_, "propagate_down");
    }
    return 0;
  }
#if 0
  inline void SetLossWeights(const vector<Blob*> & top, vector<Dtype>& lossvec) {
    vector<double> loss_weight_arr;
    cjson_GetObjectNumberArray(param_, "loss_weight", loss_weight_arr);
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

  double Forward(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    // Lock during forward to ensure sequential forward
    Lock();
    Dtype loss = 0;
    utime_start(a);
    char buf[256];
    for (int i = 0; i < blobs_.size(); ++i) {
      //LOG_IF(INFO, root_solver()) << "    param blob " << i << " " << blobs_[i]->to_debug_string(buf, 256);
    }
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

};
