
enum LossParameter_NormalizationMode {
  LossParameter_NormalizationMode_NONE,
  LossParameter_NormalizationMode_FULL,
  LossParameter_NormalizationMode_VALID,
  LossParameter_NormalizationMode_BATCH_SIZE,
};

Dtype get_normalizer(LossParameter_NormalizationMode normalization_mode, Dtype valid_count, Dtype outer_num_, Dtype inner_num_) {
  Dtype normalizer;
  switch (normalization_mode) {
    case LossParameter_NormalizationMode_FULL:
      normalizer = (Dtype)(outer_num_ * inner_num_);
      break;
    case LossParameter_NormalizationMode_VALID:
      if (valid_count == -1) {
        normalizer = (Dtype)(outer_num_ * inner_num_);
      } else {
        normalizer = valid_count;
      }
      break;
    case LossParameter_NormalizationMode_BATCH_SIZE:
      normalizer = (Dtype)(outer_num_);
      break;
    case LossParameter_NormalizationMode_NONE:
      normalizer = (Dtype)(1);
      break;
    default:
      printf("Unknown normalization mode\n");
      //printf("Unknown normalization mode: %s\n", LossParameter_NormalizationMode_Name(normalization_mode));
      break;
  }
  // Some users will have no labels for some examples in order to 'turn off' a
  // particular loss in a multi-task setup. The max prevents NaNs in that case.
  return MAX((Dtype)(1.0), normalizer);
}

static Dtype SoftmaxWithLossLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* label = IBLOB(1);
  struct Blob* next = OBLOB(0);
  struct Blob* class_weight_ = layer->i_size==3 ? IBLOB(3) : NULL;
  Dtype loss = 0;
  /// prob stores the output probability predictions from the SoftmaxLayer.
  //Blob prob_;
  /// prev vector holder used in call to the underlying SoftmaxLayer::Forward
  //vector<Blob*> softmax_bottom_vec_;
  /// top vector holder used in call to the underlying SoftmaxLayer::Forward
  //vector<Blob*> softmax_top_vec_;
  /// Whether to ignore instances with a certain label.
  //bool has_ignore_label_;
  /// The label indicating that an instance should be ignored.
  //int ignore_label_;
  bool has_class_weight_ = class_weight_!=NULL;
  /// How to normalize the output loss.
  LossParameter_NormalizationMode normalization_ = LossParameter_NormalizationMode_FULL;
  enum Method normalize_type_ = M_Softmax;
  int softmax_axis_, outer_num_, inner_num_;

  normalize_type_ = MARG(distance_type);

#if 0
void SoftmaxWithLossLayer::LayerSetUp(
    const vector<Blob*>& prev, const vector<Blob*>& top) {
  LossLayer::LayerSetUp(prev, top);
  normalize_type_ = this->layer_param_.softmax_param().normalize_type();
  if (normalize_type_ == "Softmax") {
    LayerParameter softmax_param(this->layer_param_);
    softmax_param.set_type("Softmax");
    softmax_layer_ = LayerRegistry::CreateLayer(softmax_param);
    softmax_bottom_vec_.clear();
    softmax_bottom_vec_.push_back(prev[0]);
    softmax_top_vec_.clear();
    softmax_top_vec_.push_back(&prob_);
    softmax_layer_->SetUp(softmax_bottom_vec_, softmax_top_vec_);
  }
  else if(normalize_type_ == "L2" || normalize_type_ == "L1") {
    LayerParameter normalize_param(this->layer_param_);
    normalize_param.set_type("Normalize");
    softmax_layer_ = LayerRegistry::CreateLayer(normalize_param);
    softmax_bottom_vec_.clear();
    softmax_bottom_vec_.push_back(prev[0]);
    softmax_top_vec_.clear();
    softmax_top_vec_.push_back(&prob_);
    softmax_layer_->SetUp(softmax_bottom_vec_, softmax_top_vec_);
  }
  else {
    NOT_IMPLEMENTED;
  }

  has_ignore_label_ =
    this->layer_param_.loss_param().has_ignore_label();
  if (has_ignore_label_) {
    ignore_label_ = this->layer_param_.loss_param().ignore_label();
  }
  has_class_weight_ = (this->layer_param_.softmax_param().class_weight_size() != 0);
  softmax_axis_ =
    prev[0]->CanonicalAxisIndex(this->layer_param_.softmax_param().axis());
  if (has_class_weight_) {
    class_weight_.Reshape({ prev[0]->shape(softmax_axis_) });
    CHECK_EQ(this->layer_param_.softmax_param().class_weight().size(), prev[0]->shape(softmax_axis_));
    for (i = 0; i < prev[0]->shape(softmax_axis_); i++) {
      class_weight_[i] = (Dtype)this->layer_param_.softmax_param().class_weight(i);
    }
  }
  else {
    if (layer->i_size == 3) {
      class_weight_.Reshape({ prev[0]->shape(softmax_axis_) });
      for (i = 0; i < prev[0]->shape(softmax_axis_); i++) {
        class_weight_[i] = (Dtype)1.0;
      }
    }
  }
  if (!this->layer_param_.loss_param().has_normalization() &&
      this->layer_param_.loss_param().has_normalize()) {
    normalization_ = this->layer_param_.loss_param().normalize() ?
                     LossParameter_NormalizationMode_VALID :
                     LossParameter_NormalizationMode_BATCH_SIZE;
  } else {
    normalization_ = this->layer_param_.loss_param().normalization();
  }
}


#endif

  softmax_axis_ = IARG(axis);
  outer_num_ = Blob_count_range(prev, 0, softmax_axis_);
  inner_num_ = Blob_count_range(prev, softmax_axis_ + 1, 4);
  ASSERT(outer_num_ * inner_num_ == Blob_count(label) &&
       "Number of labels must match number of predictions; "
       "e.g., if softmax axis == 1 and prediction shape is (N, C, H, W), "
       "label count (number of labels) must be N*H*W, "
       "with integer values in {0, 1, ..., C-1}." );
  if (layer->i_size == 3) {
    ASSERT(outer_num_ * inner_num_ == Blob_count(IBLOB(2)) && "Number of loss weights must match number of label." );
  }
  if (has_class_weight_) {
    ASSERT(Blob_count(class_weight_) == prev->size.c);
  }
  int i, j;

  switch (op) {
  case Op_Forward:
    next->size = prev->size;
    {
      // The forward pass computes the softmax prob values.
      const Dtype* prevData = getData(prev);
      Dtype* nextData = setDataZero(next);
      softmax_forward(prevData, nextData, prev->size, softmax_axis_);
      if (layer->i_size>=2) {
        const Dtype* probData = nextData;
        const int* labelData = (int*)getData(label);
        int dim = Data_count(next->size) / outer_num_;
        Dtype count = 0;
        if (layer->i_size == 2) {
          for (i = 0; i < outer_num_; ++i) {
            for (j = 0; j < inner_num_; j++) {
              const int label_value = (int)(labelData[i * inner_num_ + j]);
              //DCHECK_GE(label_value, 0);
              //DCHECK_LT(label_value, prob_.shape(softmax_axis_));
              loss -= log(MAX(probData[i * dim + label_value * inner_num_ + j], (Dtype)(FLT_MIN)));
              count += 1;
            }
          }
        }
#if 0
        else if(layer->i_size == 3) {
          const Dtype* weights = getData(class_weight_);
          for (i = 0; i < outer_num_; ++i) {
            for (j = 0; j < inner_num_; j++) {
              const int label_value = (int)(labelData[i * inner_num_ + j]);
              const Dtype weight_value = weights[i * inner_num_ + j] * (has_class_weight_? class_weight_.getData()[label_value] : 1.0);
              if (weight_value == 0) continue;
              if (has_ignore_label_ && label_value == ignore_label_) {
                continue;
              }
              DCHECK_GE(label_value, 0);
              DCHECK_LT(label_value, prob_.shape(softmax_axis_));
              loss -= weight_value * log(MAX(probData[i * dim + label_value * inner_num_ + j],
                (Dtype)(FLT_MIN)));
              count += weight_value;
            }
          }
        }
#endif
        loss /= get_normalizer(normalization_, count, outer_num_, inner_num_);
      }
    }
    break;
  case Op_Backward:
    {
      Blob* prob_ = next;
      const Dtype* prevData = getData(prev);
      const Dtype* nextData = getData(next);
      const Dtype* nextDiff = getDiff(next);
      Dtype* prevDiff = setDiffZero(prev);
      const Dtype* probData = nextData;
      blas_scopy(Blob_count(next), probData, prevDiff);
      const int* labelData = (int*)getData(label);
      int dim = Blob_count(prob_) / outer_num_;
      Dtype count = 0;
      if (layer->i_size >= 2) {
        if (layer->i_size == 2) {
          for (i = 0; i < outer_num_; ++i) {
            for (j = 0; j < inner_num_; ++j) {
              const int label_value = (int)(labelData[i * inner_num_ + j]);
              prevDiff[i * dim + label_value * inner_num_ + j] -= 1;
              count += 1;
            }
          }
        }
#if 0
        else if (layer->i_size == 3) {
          const Dtype* weights = getData(prev2);
          for (i = 0; i < outer_num_; ++i) {
            for (j = 0; j < inner_num_; ++j) {
              const int label_value = (int)(labelData[i * inner_num_ + j]);
              const Dtype weight_value = weights[i * inner_num_ + j];
              if (has_ignore_label_ && label_value == ignore_label_) {
                for (c = 0; c < prev[0]->shape(softmax_axis_); ++c) {
                  prevDiff[i * dim + c * inner_num_ + j] = 0;
                }
              }
              else {
                prevDiff[i * dim + label_value * inner_num_ + j] -= 1;
                for (c = 0; c < prev[0]->shape(softmax_axis_); ++c) {
                  prevDiff[i * dim + c * inner_num_ + j] *= weight_value * (has_class_weight_ ? class_weight_.getData()[label_value] : 1.0);
                }
                if(weight_value != 0) count += weight_value;
              }
            }
          }
        }
#endif
        // Scale gradient
        Dtype loss_weight = nextDiff[0] / get_normalizer(normalization_, count, outer_num_, inner_num_);
        blas_sscal(Blob_count(prob_), loss_weight, prevDiff);
      }
      //softmax_backward(prevData, nextData, nextDiff, prevDiff, prev->size, softmax_axis_);
    }
    break;
  default:
    break;
  }

  return loss;
}

static int net_softmaxloss(struct Net* net, int input, int index)
{
  const char* type = "softmaxloss";
  struct Layer* layer = net_addLayer(net, 2, 1, 0, SoftmaxWithLossLayer, type);
  LIBLOB(0) = input;
  LIBLOB(1) = index;
  IARG(axis) = 1;
  return LOBLOB(0) = net_addBlob(net, IBLOB(0)->size, type, "o");
}