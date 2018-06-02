
// * @brief A generalization of MultinomialLogisticLossLayer that takes an
//  *        "information gain" (infogain) matrix specifying the "value" of all label
// *        pairs.

static Dtype InfogainLossLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  /// prob stores the output probability predictions from the SoftmaxLayer.
  Blob prob_;
  /// bottom vector holder used in call to the underlying SoftmaxLayer::Forward
  vector<Blob*> softmax_bottom_vec_;
  /// top vector holder used in call to the underlying SoftmaxLayer::Forward
  vector<Blob*> softmax_top_vec_;

  Blob infogain_;
  Blob sum_rows_H_;  // cache the row sums of H.

  /// Whether to ignore instances with a certain label.
  bool has_ignore_label_;
  /// The label indicating that an instance should be ignored.
  int ignore_label_;
  /// How to normalize the output loss.
  LossParameter_NormalizationMode normalization_;

  int infogain_axis_, outer_num_, inner_num_, num_labels_;


#if 0
void InfogainLossLayer::LayerSetUp(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
  LossLayer::LayerSetUp(bottom, top);
  // internal softmax layer
  LayerParameter softmax_layer_param(this->layer_param_);
  SoftmaxParameter* softmax_param = softmax_layer_param.mutable_softmax_param();
  softmax_param->set_axis(this->layer_param_.infogain_loss_param().axis());
  softmax_layer_param.set_type("Softmax");
  softmax_layer_param.clear_loss_weight();
  softmax_layer_param.add_loss_weight(1);
  softmax_layer_ = LayerRegistry::CreateLayer(softmax_layer_param);
  softmax_bottom_vec_.clear();
  softmax_bottom_vec_.push_back(bottom[0]);
  softmax_top_vec_.clear();
  softmax_top_vec_.push_back(&prob_);
  softmax_layer_->SetUp(softmax_bottom_vec_, softmax_top_vec_);

  // ignore label
  has_ignore_label_ =
    this->layer_param_.loss_param().has_ignore_label();
  if (has_ignore_label_) {
    ignore_label_ = this->layer_param_.loss_param().ignore_label();
  }
  // normalization
  CHECK(!this->layer_param_.loss_param().has_normalize())
    << "normalize is deprecated. use \"normalization\"";
  normalization_ = this->layer_param_.loss_param().normalization();
  // matrix H
  if (layer->i_size < 3) {
    CHECK(this->layer_param_.infogain_loss_param().has_source())
        << "Infogain matrix source must be specified.";
    BlobProto blob_proto;
    ReadProtoFromBinaryFile(
      this->layer_param_.infogain_loss_param().source(), &blob_proto);
    infogain_.FromProto(blob_proto);
  }
}


void InfogainLossLayer::Reshape(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
  LossLayer::Reshape(bottom, top);
  softmax_layer_->Reshape(softmax_bottom_vec_, softmax_top_vec_);
  infogain_axis_ =
    bottom[0]->CanonicalAxisIndex(
      this->layer_param_.infogain_loss_param().axis());
  outer_num_ = bottom[0]->count(0, infogain_axis_);
  inner_num_ = bottom[0]->count(infogain_axis_ + 1);
  CHECK_EQ(outer_num_ * inner_num_, bottom[1]->count())
      << "Number of labels must match number of predictions; "
      << "e.g., if infogain axis == 1 and prediction shape is (N, C, H, W), "
      << "label count (number of labels) must be N*H*W, "
      << "with integer values in {0, 1, ..., C-1}.";
  num_labels_ = bottom[0]->shape(infogain_axis_);
  Blob* infogain = NULL;
  if (layer->i_size < 3) {
    infogain = &infogain_;
  } else {
    infogain = bottom[2];
  }
  CHECK_EQ(infogain->count(), num_labels_*num_labels_);
  sum_rows_H_.Reshape(vector<int>(1, num_labels_));
  if (layer->i_size == 2) {
    // H is provided as a parameter and will not change. sum rows once
    sum_rows_of_H(infogain);
  }
  if (layer->o_size >= 2) {
    // softmax output
    top[1]->ReshapeLike(*bottom[0]);
  }
}


Dtype InfogainLossLayer::get_normalizer(
    LossParameter_NormalizationMode normalization_mode, int valid_count) {
  Dtype normalizer;
  switch (normalization_mode) {
    case LossParameter_NormalizationMode_FULL:
      normalizer = (Dtype)(outer_num_ * inner_num_);
      break;
    case LossParameter_NormalizationMode_VALID:
      if (valid_count == -1) {
        normalizer = (Dtype)(outer_num_ * inner_num_);
      } else {
        normalizer = (Dtype)(valid_count);
      }
      break;
    case LossParameter_NormalizationMode_BATCH_SIZE:
      normalizer = (Dtype)(outer_num_);
      break;
    case LossParameter_NormalizationMode_NONE:
      normalizer = (Dtype)(1);
      break;
    default:
      LOG(FATAL) << "Unknown normalization mode: "
          << LossParameter_NormalizationMode_Name(normalization_mode);
  }
  // Some users will have no labels for some examples in order to 'turn off' a
  // particular loss in a multi-task setup. The max prevents NaNs in that case.
  return MAX(Dtype(1.0), normalizer);
}

#endif

void InfogainLossLayer::sum_rows_of_H(const Blob* H) {
  CHECK_EQ(H->count(), num_labels_*num_labels_)
    << "H must be " << num_labels_ << "x" << num_labels_;
  const Dtype* infogain_mat = H->cpu_data();
  Dtype* sum = sum_rows_H_;
  for ( int row = 0; row < num_labels_ ; row++ ) {
    sum[row] = 0;
    for ( int col = 0; col < num_labels_ ; col++ ) {
      sum[row] += infogain_mat[row*num_labels_+col];
    }
  }
}

void InfogainLossLayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  // The forward pass computes the softmax prob values.
  softmax_layer_->Forward(softmax_bottom_vec_, softmax_top_vec_);
  const Dtype* prob_data = prob_.cpu_data();
  const Dtype* bottom_label = nextData;
  const Dtype* infogain_mat = NULL;
  if (layer->i_size < 3) {
    infogain_mat = infogain_.cpu_data();
  } else {
    infogain_mat = getData(prev2);
  }
  int count = 0;
  Dtype loss = 0;
  for (i = 0; i < outer_num_; ++i) {
    for (j = 0; j < inner_num_; j++) {
      const int label_value =
        (int)(bottom_label[i * inner_num_ + j]);
      if (has_ignore_label_ && label_value == ignore_label_) {
        continue;
      }
      DCHECK_GE(label_value, 0);
      DCHECK_LT(label_value, num_labels_);
      for (l = 0; l < num_labels_; l++) {
        loss -= infogain_mat[label_value * num_labels_ + l] *
          log(MAX(
                prob_data[i * inner_num_*num_labels_ + l * inner_num_ + j],
                (Dtype)(kLOG_THRESHOLD)));
      }
      ++count;
    }
  }
  nextData[0] = loss / get_normalizer(normalization_, count);
  if (layer->o_size == 2) {
    top[1]->ShareData(prob_);
  }
}


void InfogainLossLayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  if (propagate_down[1]) {
    LOG(FATAL) << this->type()
               << " Layer cannot backpropagate to label inputs.";
  }
  if (propagate_down.size() > 2 && propagate_down[2]) {
    LOG(FATAL) << this->type()
               << " Layer cannot backpropagate to infogain inputs.";
  }
  if (propagate_down[0]) {
    const Dtype* prob_data = prob_.cpu_data();
    const Dtype* bottom_label = nextData;
    const Dtype* infogain_mat = NULL;
    if (layer->i_size < 3) {
      infogain_mat = infogain_.cpu_data();
    } else {
      infogain_mat = getData(prev2);
      // H is provided as a "bottom" and might change. sum rows every time.
      sum_rows_of_H(bottom[2]);
    }
    const Dtype* sum_rows_H = sum_rows_H_.cpu_data();
    Dtype* prevDiff = bottom[0]->mutable_diff();
    const int dim = Data_count(prev->size) / outer_num_;
    int count = 0;
    for (i = 0; i < outer_num_; ++i) {
      for (j = 0; j < inner_num_; ++j) {
        const int label_value =
          (int)(bottom_label[i * inner_num_ + j]);
        DCHECK_GE(label_value, 0);
        DCHECK_LT(label_value, num_labels_);
        if (has_ignore_label_ && label_value == ignore_label_) {
          for (l = 0; l < num_labels_; ++l) {
            prevDiff[i * dim + l * inner_num_ + j] = 0;
          }
        } else {
          for (l = 0; l < num_labels_; ++l) {
            prevDiff[i * dim + l * inner_num_ + j] =
               prob_data[i*dim + l*inner_num_ + j]*sum_rows_H[label_value]
               - infogain_mat[label_value * num_labels_ + l];
          }
          ++count;
        }
      }
    }
    // Scale gradient
    Dtype loss_weight = nextDiff[0] /
                        get_normalizer(normalization_, count);
    blas_sscal(Data_count(prev->size), loss_weight, prevDiff);
  }
}

};