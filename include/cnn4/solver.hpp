
#include "wstd/string.hpp"

struct Solver : SolverBase {
  CnnNet* net_;
  //Solver(CnnNet<Dtype>* net) {    init(net);  }
  void init(CnnNet* net) {
    SolverBase& base = *this;
    base = net->solver_param;
    net_ = net;
    smoothed_loss_ = 0;
    iter_ = 0;
  }

  int Test()
  {
    CnnNet* test_net = net_;
    double loss = 0;
    vector<double> test_score;
    size_t test_score_size = 0;
    for (int i = 0; i < net_->blobs_.size(); ++i) {
      Blob* blob = net_->blobs_[i];
      //auto layer = test_net->layers_[i];
      if (blob->loss_weight_ > 0 || blob->bottom_cnt_ == 0) {
        test_score_size += blob->count();
      }
    }
    test_score.assign(test_score_size, 0);
    for (int j = 0; j < test_iter_; ++j) {
      double iter_loss = test_net->Forward(TEST);
      int idx = 0;
      for (int i = 0; i < net_->blobs_.size(); ++i) {
        Blob* blob = net_->blobs_[i];
        //auto layer = test_net->layers_[i];
        if (blob->loss_weight_ > 0 || blob->bottom_cnt_ == 0) {
          for (int k = 0; k < blob->count(); ++k) {
            test_score[idx++] += blob->cpu_data()[k];
          }
        }
      }
      loss += iter_loss;
    }
    loss /= test_iter_;
    LOG(INFO) << "Test loss: " << loss;
    int idx = 0;
    for (int i = 0; i < net_->blobs_.size(); ++i) {
      Blob* blob = net_->blobs_[i];
      if (blob->loss_weight_ > 0 || blob->bottom_cnt_ == 0) {
        const double loss_weight = blob->loss_weight_;
        const string output_name = blob->name_;
        string loss_msg_stream;
        for (int k = 0; k < blob->count(); ++k) {
          const double mean_score = test_score[idx] / test_iter_;
          if (fabs(loss_weight) > 0) {
            loss_msg_stream = wstd::format(" (* %lf = %lf loss)", loss_weight, loss_weight* mean_score);
          }
          LOG(INFO) << "    Test net output #" << idx << ": " << output_name << " = "
            << mean_score << loss_msg_stream.c_str();
          ++idx;
        }
      }
    }
    return 0;
  }
  vector<double> losses_;
  double smoothed_loss_;
  void UpdateSmoothedLoss(double loss, int start_iter, int average_loss_)
  {
    if (losses_.size() < average_loss_) {
      losses_.push_back(loss);
      int size = (int)losses_.size();
      smoothed_loss_ = (smoothed_loss_ * (size - 1) + loss) / size;
    }
    else {
      int idx = (iter_ - start_iter) % average_loss_;
      smoothed_loss_ += (loss - losses_[idx]) / average_loss_;
      losses_[idx] = loss;
    }
  }

  int iter_;
  virtual void ApplyUpdate() = 0;

  vector<Blob*> learnable_params_;

  virtual void PreSolve() = 0;
  int Solve()
  {
    LOG(INFO) << "Learning Rate Policy: " << lr_policy_;
    int start_iter = iter_;
    int iters = max_iter_ - iter_;
    this->net_->learnable_params(learnable_params_);

    const int stop_iter = max_iter_;
    vector<float>  losses_;
    losses_.clear();
    PreSolve();
    Snapshot();
    smoothed_loss_ = 0;
    while (iter_ < stop_iter) {
      // zero-init the params
      //ClearParamDiffs(net_);
      if (test_interval_ && iter_ % test_interval_ == 0 && (iter_ > 0 || test_initialization_)) {
        Test();
      }
      int debug_info_ = display_ && iter_ % display_ == 0;
      // accumulate the loss and gradient
      Dtype loss = 0;
      for (int i = 0; i < iter_size_; ++i) {
        loss += net_->ForwardBackward();
      }
      loss /= iter_size_;
      // average the loss across iterations for smoothed reporting
      UpdateSmoothedLoss(loss, start_iter, average_loss_);
      if (debug_info_) {
        LOG_IF(INFO, root_solver()) << "Iteration " << iter_ << ", loss = " << smoothed_loss_;
        int score_index = 0;
        for (int j = 0; j < net_->blobs_.size(); ++j) {
          Blob* blob = net_->blobs_[j];
          if (blob->loss_weight_>0) {
            const Dtype* result_vec = blob->cpu_data();
            const string output_name = blob->name_;
            const Dtype loss_weight = blob->loss_weight_;

            for (int k = 0; k < blob->count(); ++k) {
              ostringstream loss_msg_stream;
              if (loss_weight) {
                loss_msg_stream << " (* " << loss_weight
                  << " = " << loss_weight* result_vec[k] << " loss)";
              }
              LOG_IF(INFO, root_solver()) << "    Train net output #"
                << score_index++ << ": " << output_name << " = "
                << result_vec[k] << loss_msg_stream.str();
            }
          }
        }
      }
      ApplyUpdate();
      // Increment the internal iter_ counter -- its value should always indicate
      // the number of times the weights have been updated.
      ++iter_;
      //SolverAction::Enum request = GetRequestedAction();
      // Save a snapshot_ if needed.
      if (snapshot_ && iter_ % snapshot_ == 0 && root_solver()) {
        Snapshot();
      }
    }

    if (display_ && iter_ % display_ == 0) {
      double loss = net_->Forward(TRAIN);
      UpdateSmoothedLoss(loss, start_iter, average_loss_);
      LOG(INFO) << "Iteration " << iter_ << ", loss = " << smoothed_loss_;
    }
    if (test_interval_ && iter_ % test_interval_ == 0) {
      Test();
    }
    LOG(INFO) << "Optimization Done.";
    return 0;
  }

  int saveJson(CnnNet* net, const char* fn) {
    cjson* root = net->ToJson();
    if (root) {
      cjson_SaveFile(fn, root);
      cjson_Delete(root);
    }
    return 1;
  }

  void Snapshot() {
    char model_filename[256];
    _snprintf(model_filename, 256, "%s_iter_%d.json", snapshot_prefix_, iter_);
    //cJSON_SaveFile(model_filename, net_->param_);
    LOG(INFO) << "saved: " << model_filename;
    saveJson(net_, model_filename);
  }
};



struct SGDSolver : public Solver {
  int current_step_;
  vector<Blob*>  history_;
  vector<Blob*>  update_;
  vector<Blob*>  temp_;
  virtual void PreSolve()
  {
    // Initialize the history
    const vector<Blob*>& net_params = learnable_params_;
    int n = (int)net_params.size();
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
    switch(lr_policy_) {
    case LrPolicy_fixed:
      rate = base_lr_;
      break;
    case LrPolicy_step:
      this->current_step_ = this->iter_ / stepsize_;
      rate = base_lr_ * pow(gamma_, this->current_step_);
      break;
    case LrPolicy_exp:
      rate = base_lr_ * pow(gamma_, this->iter_);
      break;
    case LrPolicy_inv:
      rate = base_lr_ * pow(Dtype(1) + gamma_ * this->iter_, -power_);
      break;
    case LrPolicy_multistep:
    {
      int stepvalue_size = stepvalue.size();
        if (this->current_step_ < stepvalue_size && this->iter_ >= this->stepvalue[current_step_]) {
          this->current_step_++;
          LOG(INFO) << "MultiStep Status: Iteration " << this->iter_ << ", step = " << this->current_step_;
        }
        rate = base_lr_ * pow(gamma_, this->current_step_);
    }
    break;
    case LrPolicy_poly:
      rate = base_lr_ * pow(1. - (this->iter_ / max_iter_), power_);
      break;
    case LrPolicy_sigmoid:
      rate = base_lr_ * (1. / (1. + exp(-gamma_ * (this->iter_ - stepsize_))));
      break;
    default:
      LOG(FATAL) << "Unknown learning rate policy: " << LrPolicy_Name[lr_policy_];
      ASSERT(0);
      break;
    }
    return rate;
  }

  virtual void ClipGradients()
  {
    if (clip_gradients_ < 0) { return; }
    double sumsq_diff = 0;
    for (int i = 0; i < learnable_params_.size(); ++i) {
      sumsq_diff += learnable_params_[i]->sumsq_diff();
    }
    const double l2norm_diff = sqrt(sumsq_diff);
    if (l2norm_diff > clip_gradients_) {
      double scale_factor = clip_gradients_ / l2norm_diff;
      //     LOG(INFO) << "Gradient clipping: scaling down gradients (L2 norm "
      //         << l2norm_diff << " > " << clip_gradients_ << ") "
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
    if (iter_size_ == 1) { return; }
    // Scale gradient to counterbalance accumulation.
    const Dtype accum_normalization = Dtype(1.) / iter_size_;
    caffe_scal(learnable_params_[param_id]->count(), accum_normalization,
      learnable_params_[param_id]->mdiff());

  }


  virtual void Regularize(int param_id)
  {
    double decay_mult = learnable_params_[param_id]->decay_mult_;
    double local_decay = weight_decay_ * decay_mult;
    int count_ = learnable_params_[param_id]->count();
    const Dtype* learnable_params_data = learnable_params_[param_id]->data();
    Dtype* params_diff = learnable_params_[param_id]->mdiff();
    if (local_decay) {
      if (regularization_type_ == RegularizationType_L2) {
        // add weight decay
        caffe_axpy(count_, local_decay, learnable_params_data, params_diff);
      }
      else if (regularization_type_ == RegularizationType_L1) {
        Dtype* temp_data = temp_[param_id]->mdata();
        caffe_sign(count_, learnable_params_data, temp_data);
        caffe_axpy(count_, local_decay, temp_data, params_diff);
      }
      else {
        LOG(FATAL) << "Unknown regularization type: " << RegularizationType_Name[regularization_type_];
      }
    }
  }

  virtual void ComputeUpdateValue(int param_id, double rate)
  {
    //const vector<float> & net_params_lr = this->net_->params_lr();
    double local_rate = rate;// net_params_lr[param_id];
#if 0
    int count_ = learnable_params_[param_id]->count();
    // Compute the update to history, then copy it to the parameter diff.
    Dtype* learnable_params_diff = learnable_params_[param_id]->mdiff();
    Dtype* history_data = history_[param_id]->mdata();
    caffe_axpby(count_, local_rate, learnable_params_diff, momentum_, history_data);
    caffe_copy(count_, history_data, learnable_params_diff);
#else
    const vector<Blob*> & net_params = learnable_params_;
    sgd_update(net_params[param_id]->count(), net_params[param_id]->mdiff(), history_[param_id]->mdata(), momentum_, local_rate);
#endif
  }

  virtual void ApplyUpdate()
  {
    CHECK(root_solver());
    double rate = GetLearningRate();
    if (display_ && this->iter_ % display_ == 0) {
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
      momentum_, delta_, local_rate);
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
      this->history_[param_id]->mdata(), delta_, local_rate);
  }

};

struct AdamSolver : public SGDSolver {
  virtual void ComputeUpdateValue(int param_id, Dtype rate)
  {
    const vector<Blob*> & net_params = learnable_params_;
    //const vector<float> & net_params_lr = this->net_->params_lr();
    double local_rate = rate * net_params[param_id]->lr_mult_;
    const double beta1 = momentum_;
    const double beta2 = momentum2_;
    // we create aliases for convenience
    size_t update_history_offset = net_params.size();
    Blob* val_m = this->history_[param_id];
    Blob* val_v = this->history_[param_id + update_history_offset];
    Blob* val_t = this->temp_[param_id];
    const int t = this->iter_ + 1;
    const double correction = sqrt(double(1) - pow(beta2, t)) /
      (double(1.) - pow(beta1, t));
    const int N = net_params[param_id]->count();
    const double eps_hat = delta_;

    adam_update(N, net_params[param_id]->mdiff(),
      val_m->mdata(), val_v->mdata(), beta1, beta2,
      eps_hat, local_rate * correction);
  }

};
