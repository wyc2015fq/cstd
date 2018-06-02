

/**
 * @brief Generate the detection output based on bounding box regression and
 * confidence score.
 *
 * Intended for use with MultiBox detection method.
 *
 * NOTE: does not implement Backwards operation.
 */

class PredictBoxLayer : public Layer {
 public:
   explicit PredictBoxLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "PredictBox"; }

 protected:
  /**
   * @brief Get the bounding box output.
   *
   * @param bottom input Blob vector (exact 2)
   *   -# @f$ (1 \times 1 \times N \times 7) @f$
   *      N detection results.
   *   -# @f$ (1 \times 1 \times M \times 7) @f$
   *      M ground truth.
   * @param top Blob vector (length 1)
   *   -# @f$ (1 \times 1 \times N \times 4) @f$
   *      N is the number of detections, and each row is:
   *      [image_id, label, confidence, true_pos, false_pos]
   */
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
                           const vector<Blob*>& top);
  /// @brief Not implemented
  virtual void Backward_cpu(const vector<Blob*>& top,
                            const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
                            const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  int receptive_field_;
  int stride_;
  bool nms_;
  bool bounding_box_regression_;
  bool output_vector_;
  Dtype positive_thresh_;
  bool bounding_box_exp_;
  Blob counter_;
  bool use_stitch_;
};





void PredictBoxLayer::LayerSetUp(
      const vector<Blob*>& bottom, const vector<Blob*>& top) {
  PredictBoxParameter predict_box_param = this->layer_param_.predict_box_param();
  stride_ = predict_box_param.stride();
  receptive_field_ = predict_box_param.receptive_field();
  //nms_ = predict_box_param.nms();
  bounding_box_regression_ = (layer->i_size >= 2);
  nms_ = (layer->i_size == 3 && prev2->size.c == 2);
  use_stitch_ = (layer->i_size == 3 && prev2->size.c == 3);
  //output_vector_ = predict_box_param.output_vector();
  output_vector_ = (layer->o_size == 2);
  positive_thresh_ = predict_box_param.positive_thresh();
  bounding_box_exp_ = predict_box_param.bbreg_exp();
}


void PredictBoxLayer::Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  CHECK_EQ(prev->size.c, 2);
  if (bounding_box_regression_) {
    CHECK_EQ(prev->size.n, prev1->size.n);
    CHECK_EQ(prev->size.h, prev1->size.h);
    CHECK_EQ(prev->size.w, prev1->size.w);
    CHECK_EQ(prev1->size.c, 4);
  }
  
  if (nms_) {
    CHECK_EQ(prev->size.n, prev2->size.n);
    CHECK_EQ(prev->size.c, prev2->size.c);
    CHECK_EQ(prev->size.h, prev2->size.h);
    CHECK_EQ(prev->size.w, prev2->size.w);
  }

  if (use_stitch_) {
    CHECK_EQ(prev->size.n, prev2->size.n);
    CHECK_EQ(prev2->size.c, 3);
    CHECK_EQ(prev->size.h, prev2->size.h);
    CHECK_EQ(prev->size.w, prev2->size.w);
  }

  top[0]->Reshape({ prev->size.n, 5, prev->size.h, prev->size.w });
  if (output_vector_) {
    top[1]->Reshape({ prev->size.n, 1, 5 });//will be modified on the fly.
  }
  counter_.Reshape({ prev->size.n,1,prev->size.h,prev->size.w });
}


void PredictBoxLayer::Forward(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
  const Dtype* score_data = prevData;
  Dtype* bb_data = nextData;
  int num = prev->size.n;
  int output_height = prev->size.h;
  int output_width = prev->size.w;
  int count = 0;

  for (n = 0; n < num; n++) {
    for (x = 0; x < output_width; x++) {
      for (y = 0; y < output_height; y++) {
        if (((!nms_ && score_data[bottom[0]->offset(n, 1, y, x)] > positive_thresh_) ||
          (nms_ && score_data[bottom[0]->offset(n, 1, y, x)] > positive_thresh_ &&
           score_data[bottom[0]->offset(n, 1, y, x)] > getData(prev2)[bottom[2]->offset(n, 1, y, x)] - 1e-5))
            && !(use_stitch_ && getData(prev2)[bottom[2]->offset(n, 2, y, x)] == 0)) {
          Dtype bias_x = use_stitch_ ? getData(prev2)[bottom[2]->offset(n, 0, y, x)] : 0;
          Dtype bias_y = use_stitch_ ? getData(prev2)[bottom[2]->offset(n, 1, y, x)] : 0;
          Dtype real_receptive_field = use_stitch_ ? getData(prev2)[bottom[2]->offset(n, 2, y, x)] : receptive_field_;
          bb_data[top[0]->offset(n, 0, y, x)] = (Dtype(x * stride_) - bias_x) / (Dtype)(12) * real_receptive_field;
          bb_data[top[0]->offset(n, 1, y, x)] = (Dtype(y * stride_) - bias_y) / (Dtype)(12) * real_receptive_field;
          bb_data[top[0]->offset(n, 2, y, x)] = real_receptive_field;
          bb_data[top[0]->offset(n, 3, y, x)] = real_receptive_field;
          bb_data[top[0]->offset(n, 4, y, x)] = score_data[bottom[0]->offset(n, 1, y, x)];
          if (bounding_box_regression_) {
            if (bounding_box_exp_) {
              bb_data[top[0]->offset(n, 0, y, x)] += nextData[bottom[1]->offset(n, 0, y, x)] * real_receptive_field;
              bb_data[top[0]->offset(n, 1, y, x)] += nextData[bottom[1]->offset(n, 1, y, x)] * real_receptive_field;
              bb_data[top[0]->offset(n, 2, y, x)] *= exp(nextData[bottom[1]->offset(n, 2, y, x)]);
              bb_data[top[0]->offset(n, 3, y, x)] *= exp(nextData[bottom[1]->offset(n, 3, y, x)]);
            }
            else {
              bb_data[top[0]->offset(n, 0, y, x)] += nextData[bottom[1]->offset(n, 1, y, x)] * real_receptive_field;
              bb_data[top[0]->offset(n, 1, y, x)] += nextData[bottom[1]->offset(n, 0, y, x)] * real_receptive_field;
              bb_data[top[0]->offset(n, 2, y, x)] +=
                (nextData[bottom[1]->offset(n, 3, y, x)] - nextData[bottom[1]->offset(n, 1, y, x)]) * real_receptive_field;
              bb_data[top[0]->offset(n, 3, y, x)] +=
                (nextData[bottom[1]->offset(n, 2, y, x)] - nextData[bottom[1]->offset(n, 0, y, x)]) * real_receptive_field;
            }
          }
          count++;
        }
        else {
          bb_data[top[0]->offset(n, 0, y, x)] = (Dtype)(0);
          bb_data[top[0]->offset(n, 1, y, x)] = (Dtype)(0);
          bb_data[top[0]->offset(n, 2, y, x)] = (Dtype)(0);
          bb_data[top[0]->offset(n, 3, y, x)] = (Dtype)(0);
          bb_data[top[0]->offset(n, 4, y, x)] = (Dtype)(0);
        }
      }
    }
  }

  if (output_vector_) {
    if (num == 1 && count > 0) {
      top[1]->Reshape({ prev->size.n, count, 5 });
      int i = 0;
      for (x = 0; x < output_width; x++) {
        for (y = 0; y < output_height; y++) {
          if (bb_data[top[0]->offset(0, 4, y, x)] > positive_thresh_) {
            top[1]->mutable_data()[i * 5] = bb_data[top[0]->offset(0, 0, y, x)];
            top[1]->mutable_data()[i * 5 + 1] = bb_data[top[0]->offset(0, 1, y, x)];
            top[1]->mutable_data()[i * 5 + 2] = bb_data[top[0]->offset(0, 2, y, x)];
            top[1]->mutable_data()[i * 5 + 3] = bb_data[top[0]->offset(0, 3, y, x)];
            top[1]->mutable_data()[i * 5 + 4] = bb_data[top[0]->offset(0, 4, y, x)];
            i++;
          }
        }
      }
    }
    else {
      top[1]->Reshape({ prev->size.n, 1, 5 });
      blas_sset(top[1]->count(), 0, top[1]->mutable_data());
    }
  }
}


void PredictBoxLayer::Backward(const vector<Blob*>& top,
                                          const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  NOT_IMPLEMENTED;
}

#ifdef CPU_ONLY
STUB_GPU(PredictBoxLayer);
#endif

INSTANTIATE_CLASS(PredictBoxLayer);
REGISTER_LAYER_CLASS(PredictBox);


