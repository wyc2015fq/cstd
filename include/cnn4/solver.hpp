

struct Solver : SolverBase {
  Net* net_;
  typedef Net::Dtype Dtype;
  //Solver(Net<Dtype>* net) {    init(net);  }
  void init(Net* net) {
    SolverBase& base = *this;
    base = net->solver_param;
    net_ = net;
    smoothed_loss_ = 0;
  }

  int Test()
  {
    Net* test_net = net_;
    Dtype loss = 0;
    vector<Dtype> test_score;
    test_score.assign(net_->blobs_.size(), 0);
    debug_info_ = 0;
    for (int j = 0; j < test_iter; ++j) {
      Dtype iter_loss = test_net->Forward(TEST);
      for (int i = 0; i < net_->blobs_.size(); ++i) {
        Blob* blob = net_->blobs_[i];
        //auto layer = test_net->layers_[i];
        if (blob->loss_weight_ > 0 || blob->bottom_cnt_ == 0) {
          test_score[i] += blob->amean_data();
        }
      }
      loss += iter_loss;
    }
    loss /= test_iter;
    LOG(INFO) << "Test loss: " << loss;
    int idx = 0;
    for (int i = 0; i < net_->blobs_.size(); ++i) {
      Blob* blob = net_->blobs_[i];
      if (blob->loss_weight_ > 0 || blob->bottom_cnt_==0) {
        const Dtype loss_weight = blob->loss_weight_;
        const string output_name = blob->name_;
        ostringstream loss_msg_stream;
        const Dtype mean_score = test_score[i] / test_iter;
        if (fabs(loss_weight) > 0) {
          loss_msg_stream << " (* " << loss_weight
            << " = " << loss_weight* mean_score << " loss)";
        }
        LOG(INFO) << "    Test net output #" << idx << ": " << output_name << " = "
          << mean_score << loss_msg_stream.str();
        ++idx;
      }
    }
    return 0;
  }
  vector<double> losses_;
  Dtype smoothed_loss_;
  void UpdateSmoothedLoss(Dtype loss, int start_iter,  int average_loss)
  {
    if (losses_.size() < average_loss) {
      losses_.push_back(loss);
      int size = losses_.size();
      smoothed_loss_ = (smoothed_loss_ * (size - 1) + loss) / size;
    }
    else {
      int idx = (iter_ - start_iter) % average_loss;
      smoothed_loss_ += (loss - losses_[idx]) / average_loss;
      losses_[idx] = loss;
    }
  }

  int iter_ = 0;
  virtual void ApplyUpdate() = 0;

  vector<Blob*> learnable_params_;

  virtual void PreSolve() = 0;
  int Solve()
  {
    LOG(INFO) << "Learning Rate Policy: " << lr_policy;
    int start_iter = iter_;
    int iters = max_iter - iter_;
    this->net_->learnable_params(learnable_params_);

    const int stop_iter = max_iter;
    vector<float>  losses_;
    losses_.clear();
    PreSolve();
    Snapshot();
    double smoothed_loss_ = 0;
    while (iter_ < stop_iter) {
      // zero-init the params
      //ClearParamDiffs(net_);
      if (test_interval && iter_ % test_interval == 0 && (iter_ > 0 || test_initialization)) {
        Test();
      }
      int debug_info_ = display && iter_ % display == 0;
      // accumulate the loss and gradient
      Dtype loss = 0;
      for (int i = 0; i < iter_size; ++i) {
        loss += net_->ForwardBackward();
      }
      loss /= iter_size;
      // average the loss across iterations for smoothed reporting
      UpdateSmoothedLoss(loss, start_iter, average_loss);
      if (debug_info_) {
        LOG_IF(INFO, root_solver()) << "Iteration " << iter_
          << ", loss = " << smoothed_loss_;
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
      // Save a snapshot if needed.
      if (snapshot && iter_ % snapshot == 0 && root_solver()) {
        Snapshot();
      }
    }

    if (display && iter_ % display == 0) {
      Dtype loss = net_->Forward(TRAIN);
      UpdateSmoothedLoss(loss, start_iter, average_loss);
      LOG(INFO) << "Iteration " << iter_ << ", loss = " << smoothed_loss_;
    }
    if (test_interval && iter_ % test_interval == 0) {
      Test();
    }
    LOG(INFO) << "Optimization Done.";
    return 0;
  }

  void Snapshot() {
    char model_filename[256];
    _snprintf(model_filename, 256, "%s_iter_%d.json", snapshot_prefix_, iter_);
    //cJSON_SaveFile(model_filename, net_->param_);
  }
};
