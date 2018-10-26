
template <typename Dtype>
struct Solver {
  CJSON* param_;
  Net<Dtype>* net_;
  //Solver(Net<Dtype>* net) {    init(net);  }
  void init(Net<Dtype>* net) {
    param_ = net->param_->GetObjectItem("solver");
    net_ = net;
  }

  int Test()
  {
    Net<Dtype>* test_net = net_;
    Dtype loss = 0;
    bool test_compute_loss = param_->GetObjectBool("test_compute_loss", false);
    int test_iter = param_->GetObjectInt("test_iter", 1);
    vector<Dtype> test_score;
    test_score.resize(net_->layers_.size());
    for (int i = 0; i < net_->layers_.size(); ++i) {
      test_score[i] = 0;
    }
    for (int j = 0; j < test_iter; ++j) {
      Dtype iter_loss = test_net->Forward();
      for (int i = 0; i < net_->layers_.size(); ++i) {
        auto layer = test_net->layers_[i];
        test_score[i] += iter_loss;
      }
      loss += iter_loss;
    }
    loss /= test_iter;
    LOG(INFO) << "Test loss: " << loss;
    for (int i = 0; i < net_->layers_.size(); ++i) {
      if (fabs(test_score[i]) > 0) {
        auto layer = test_net->layers_[i];
        const string output_name = layer->name;
        const Dtype loss_weight = layer->loss_weight_;
        ostringstream loss_msg_stream;
        const Dtype mean_score = test_score[i] / test_iter;
        if (loss_weight) {
          loss_msg_stream << " (* " << loss_weight
            << " = " << loss_weight* mean_score << " loss)";
        }
        LOG(INFO) << "    Test net output #" << i << ": " << output_name << " = "
          << mean_score << loss_msg_stream.str();
      }
    }
    return 0;
  }


int Solve()
{
  LOG(INFO) << "Solving " << param_->GetObjectString("name", "");
  double lr_policy = param_->GetObjectNumber("lr_policy", 0.001);
  LOG(INFO) << "Learning Rate Policy: " << lr_policy;
  int iter_ = 0;
  int start_iter = iter_;
  int max_iter = (int)param_->GetObjectNumber("max_iter", 1000);
  int test_interval = (int)param_->GetObjectNumber("test_interval", 0);
  int test_initialization = (int)param_->GetObjectNumber("test_initialization", 0);
  int iters = max_iter - iter_;

  const int stop_iter = max_iter;
  int average_loss = param_->GetObjectNumber("average_loss", 1);
  vector<float>  losses_;
  losses_.clear();
  double smoothed_loss_ = 0;
  while (iter_ < stop_iter) {
    // zero-init the params
    //ClearParamDiffs(net_);
    if (test_interval && iter_ % test_interval == 0 && (iter_ > 0 || test_initialization)) {
      Test();
    }
#if 0
    for (int i = 0; i < callbacks_.size(); ++i) {
      callbacks_[i]->on_start();
    }
    const bool display = param_.display() && iter_ % param_.display() == 0;
    net_->set_debug_info(display && param_.debug_info());
#ifdef _DEBUG
    //net_->set_debug_info(true);
#endif
      // accumulate the loss and gradient
    Dtype loss = 0;
    for (int i = 0; i < param_.iter_size(); ++i) {
      loss += net_->ForwardBackward();
    }
    loss /= param_.iter_size();
    // average the loss across iterations for smoothed reporting
    UpdateSmoothedLoss(loss, start_iter, average_loss);
    if (display) {
      LOG_IF(INFO, root_solver()) << "Iteration " << iter_
        << ", loss = " << smoothed_loss_;
      const vector<Blob<Dtype>*> & result = net_->output_blobs();
      int score_index = 0;
      for (int j = 0; j < result.size(); ++j) {
        const Dtype* result_vec = result[j]->cpu_data();
        const string & output_name =
          net_->blob_names()[net_->output_blob_indices()[j]];
        const Dtype loss_weight =
          net_->blob_loss_weights()[net_->output_blob_indices()[j]];
        if (result[j]->count() > 10) { //add by jxs tmp
          continue;
        }
        for (int k = 0; k < result[j]->count(); ++k) {
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
    for (int i = 0; i < callbacks_.size(); ++i) {
      callbacks_[i]->on_gradients_ready();
    }
    ApplyUpdate();
    // Increment the internal iter_ counter -- its value should always indicate
    // the number of times the weights have been updated.
    ++iter_;
    SolverAction::Enum request = GetRequestedAction();
    // Save a snapshot if needed.
    if ((param_.snapshot()
      && iter_ % param_.snapshot() == 0
      && root_solver()) ||
      (request == SolverAction::SNAPSHOT)) {
      Snapshot();
    }
#endif
  }
#if 0

  // If we haven't already, save a snapshot after optimization, unless
  // overridden by setting snapshot_after_train := false
  if (param_.snapshot_after_train()
    && (!param_.snapshot() || iter_ % param_.snapshot() != 0)) {
    Snapshot();
  }
  // After the optimization is done, run an additional train and test pass to
  // display the train and test loss/outputs if appropriate (based on the
  // display and test_interval settings, respectively).  Unlike in the rest of
  // training, for the train net we only run a forward pass as we've already
  // updated the parameters "max_iter" times -- this final pass is only done to
  // display the loss, which is computed in the forward pass.
  if (param_.display() && iter_ % param_.display() == 0) {
    int average_loss = this->param_.average_loss();
    Dtype loss;
    net_->Forward(&loss);
    UpdateSmoothedLoss(loss, start_iter, average_loss);
    LOG(INFO) << "Iteration " << iter_ << ", loss = " << smoothed_loss_;
  }
  if (param_.test_interval() && iter_ % param_.test_interval() == 0) {
    TestAll();
  }
#endif
  LOG(INFO) << "Optimization Done.";
  return 0;
}

};
