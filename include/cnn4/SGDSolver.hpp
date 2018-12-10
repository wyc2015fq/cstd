
struct SGDSolver : public Solver {
  int current_step_;
  vector<Blob*>  history_;
  vector<Blob*>  update_;
  vector<Blob*>  temp_;
  virtual void PreSolve()
  {
    // Initialize the history
    const vector<Blob*>& net_params = learnable_params_;
    int n = net_params.size();
    blobs_reset(history_, n);
    blobs_reset(update_, n);
    blobs_reset(temp_, n);
    for (int i = 0; i < n; ++i) {
      const DataShape & shape = net_params[i]->shape();
      history_[i]->Reshape(shape);
      update_[i]->Reshape(shape);
      temp_[i]->Reshape(shape);
    }
  }

  virtual double GetLearningRate()
  {
    double rate;
    string lr_policy = lr_policy_;
    if (lr_policy == "fixed") {
      rate = base_lr;
    }
    else if (lr_policy == "step") {
      this->current_step_ = this->iter_ / stepsize;
      rate = base_lr * pow(gamma, this->current_step_);
    }
    else if (lr_policy == "exp") {
      rate = base_lr * pow(gamma, this->iter_);
    }
    else if (lr_policy == "inv") {
      rate = base_lr * pow(Dtype(1) + gamma * this->iter_, -power);
    }
#if 0
    else if (lr_policy == "multistep") {
      CJSON* item = this->param_->GetObjectItem("stepvalue");
      if (item) {
        int stepvalue_size = item->GetArraySize();
        if (this->current_step_ < stepvalue_size && this->iter_ >= item->GetArrayItem(this->current_step_)->valueint) {
          this->current_step_++;
          LOG(INFO) << "MultiStep Status: Iteration " <<
            this->iter_ << ", step = " << this->current_step_;
        }
      }
      rate = base_lr * pow(gamma, this->current_step_);
    }
#endif
    else if (lr_policy == "poly") {
      rate = base_lr * pow(1. - (this->iter_ / max_iter), power);
    }
    else if (lr_policy == "sigmoid") {
      rate = base_lr * (1. / (1. + exp(-gamma * (this->iter_ - stepsize))));
    }
    else {
      LOG(FATAL) << "Unknown learning rate policy: " << lr_policy;
    }
    return rate;
  }

  virtual void ClipGradients()
  {
    if (clip_gradients < 0) { return; }
    Dtype sumsq_diff = 0;
    for (int i = 0; i < learnable_params_.size(); ++i) {
      sumsq_diff += learnable_params_[i]->sumsq_diff();
    }
    const Dtype l2norm_diff = std::sqrt(sumsq_diff);
    if (l2norm_diff > clip_gradients) {
      Dtype scale_factor = clip_gradients / l2norm_diff;
      //     LOG(INFO) << "Gradient clipping: scaling down gradients (L2 norm "
      //         << l2norm_diff << " > " << clip_gradients << ") "
      //         << "by scale factor " << scale_factor;
      for (int i = 0; i < learnable_params_.size(); ++i) {
        Blob* blob = learnable_params_[i];
        //learnable_params_[i]->scale_diff(scale_factor);
        Dtype* diff = blob->mdiff();
        int count_ = blob->count();
        caffe_scal(count_, scale_factor, diff);
      }
    }
  }

  virtual void Normalize(int param_id)
  {
    if (iter_size == 1) { return; }
    // Scale gradient to counterbalance accumulation.
    const Dtype accum_normalization = Dtype(1.) / iter_size;
    caffe_scal(learnable_params_[param_id]->count(), accum_normalization,
      learnable_params_[param_id]->mdiff());

  }


  virtual void Regularize(int param_id)
  {
    Dtype decay_mult = learnable_params_[param_id]->decay_mult_;
    Dtype local_decay = weight_decay * decay_mult;
    int count_ = learnable_params_[param_id]->count();
    const Dtype* learnable_params_data = learnable_params_[param_id]->data();
    Dtype* params_diff = learnable_params_[param_id]->mdiff();
    if (local_decay) {
      if (regularization_type == RegularizationType_L2) {
        // add weight decay
        caffe_axpy(count_, local_decay, learnable_params_data, params_diff);
      }
      else if (regularization_type == RegularizationType_L1) {
        Dtype* temp_data = temp_[param_id]->mdata();
        caffe_sign(count_, learnable_params_data, temp_data);
        caffe_axpy(count_, local_decay, temp_data, params_diff);
      }
      else {
        LOG(FATAL) << "Unknown regularization type: " << RegularizationType_Name[regularization_type];
      }
    }
  }
  
  virtual void ComputeUpdateValue(int param_id, Dtype rate)
  {
    //const vector<float> & net_params_lr = this->net_->params_lr();
    Dtype local_rate = rate;// net_params_lr[param_id];
#if 0
    int count_ = learnable_params_[param_id]->count();
    // Compute the update to history, then copy it to the parameter diff.
    Dtype* learnable_params_diff = learnable_params_[param_id]->mdiff();
    Dtype* history_data = history_[param_id]->mdata();
    caffe_axpby(count_, local_rate, learnable_params_diff, momentum, history_data);
    caffe_copy(count_, history_data, learnable_params_diff);
#else
    const vector<Blob*> & net_params = learnable_params_;
    sgd_update(net_params[param_id]->count(), net_params[param_id]->mdiff(), history_[param_id]->mdata(), momentum, local_rate);
#endif
  }

  virtual void ApplyUpdate()
  {
    CHECK(root_solver());
    Dtype rate = GetLearningRate();
    if (display && this->iter_ % display == 0) {
      LOG(INFO) << "Iteration " << this->iter_ << ", lr = " << rate;
    }
    ClipGradients();
    for (int param_id = 0; param_id < learnable_params_.size(); ++param_id) {
      Normalize(param_id);
      Regularize(param_id);
      ComputeUpdateValue(param_id, rate);
    }
    this->net_->Update();
  }

};

struct AdaDeltaSolver : public SGDSolver
{
  virtual void ComputeUpdateValue(int param_id, Dtype rate)
  {
    const vector<Blob*> & net_params = learnable_params_;
    //const vector<float> & net_params_lr = this->net_->params_lr();
    Dtype local_rate = rate * net_params[param_id]->lr_mult_;
    size_t update_history_offset = net_params.size();
    adadelta_update(net_params[param_id]->count(),
      net_params[param_id]->mdiff(),
      this->history_[param_id]->mdata(),
      this->history_[update_history_offset + param_id]->mdata(),
      momentum, delta, local_rate);
  }
};

struct AdaGradSolver : public SGDSolver
{
  virtual void ComputeUpdateValue(int param_id, Dtype rate)
  {
    CHECK(root_solver());
    const vector<Blob*> & net_params = learnable_params_;
    //const vector<float> & net_params_lr = this->net_->params_lr();
    //Dtype local_rate = rate * net_params_lr[param_id];
    Dtype local_rate = rate;
    adagrad_update(net_params[param_id]->count(),
      net_params[param_id]->mdiff(),
      this->history_[param_id]->mdata(), delta, local_rate);
  }

};

struct AdamSolver : public SGDSolver {
  virtual void ComputeUpdateValue(int param_id, Dtype rate)
  {
    const vector<Blob*> & net_params = learnable_params_;
    //const vector<float> & net_params_lr = this->net_->params_lr();
    Dtype local_rate = rate * net_params[param_id]->lr_mult_;
    const Dtype beta1 = momentum;
    const Dtype beta2 = momentum2;
    // we create aliases for convenience
    size_t update_history_offset = net_params.size();
    Blob* val_m = this->history_[param_id];
    Blob* val_v = this->history_[param_id + update_history_offset];
    Blob* val_t = this->temp_[param_id];
    const int t = this->iter_ + 1;
    const Dtype correction = std::sqrt(Dtype(1) - pow(beta2, t)) /
      (Dtype(1.) - pow(beta1, t));
    const int N = net_params[param_id]->count();
    const Dtype eps_hat = delta;

    adam_update(N, net_params[param_id]->mdiff(),
      val_m->mdata(), val_v->mdata(), beta1, beta2,
      eps_hat, local_rate * correction);
  }

};
