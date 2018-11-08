#ifndef CAFFE_INNER_PRODUCT_LAYER_HPP_
#define CAFFE_INNER_PRODUCT_LAYER_HPP_

/**
 * @brief Also known as a "fully-connected" layer, computes an inner product
 *        with a set of learned weights, and (optionally) adds biases.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */

template <typename Dtype>
struct InnerProductLayer : public Layer<Dtype>
{
  int M_;
  int K_;
  int N_;
  int axis_;
  bool bias_term_;
  Blob<Dtype> bias_multiplier_;
  bool transpose_;  ///< if true, assume transposed weights

  virtual inline const char* type() const { return "InnerProduct"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

  virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top)
  {
    const int num_output = this->param_->getint("num_output", 0);
    this->axis_ = this->param_->getint("axis", 1);
    bias_term_ = this->param_->getbool("bias_term", true);
    transpose_ = this->param_->getbool("transpose", false);
    N_ = num_output;
    const int axis = bottom[0]->CanonicalAxisIndex(this->axis_);
    CJSON* blobs_json = param_->get("blobs");
    bool has_data = blobs_json && blobs_json->GetArraySize() > 0 && blobs_json->GetArrayItem(0)->has("data");
    // Dimensions starting from "axis" are "flattened" into a single
    // length K_ vector. For example, if bottom[0]'s shape is (N, C, H, W),
    // and axis == 1, N inner products with dimension CHW are performed.
    K_ = bottom[0]->count(axis);
    // Check if we need to set up the weights
    if (has_data) {
      LOG(INFO) << "Skipping parameter initialization";
    }
    else {
      if (bias_term_) {
        blobs_reset(this->blobs_, 2);
      }
      else {
        blobs_reset(this->blobs_, 1);
      }
      // Initialize the weights
      DataShape weight_shape = transpose_ ? dataShape(K_, N_) : dataShape(N_, K_);
      this->blobs_[0]->Reshape(weight_shape);
      // fill the weights
      cJSON* weight_filler = param_->get("weight_filler");
      Filler(this->blobs_[0], weight_filler);
      // If necessary, intiialize and fill the bias term
      if (bias_term_) {
        cJSON* bias_filler = param_->get("bias_filler");
        DataShape bias_shape = dataShape(1, N_);
        this->blobs_[1]->Reshape(bias_shape);
        Filler(this->blobs_[1], bias_filler);
      }
    }  // parameter initialization
    //this->param_propagate_down_.resize(this->blobs_.size(), true);
  }
  
  virtual void Reshape(const vector<Blob<Dtype>*> & bottom,  const vector<Blob<Dtype>*> & top)
  {
    // Figure out the dimensions
    const int axis = bottom[0]->CanonicalAxisIndex(this->axis_);
    const int new_K = bottom[0]->count(axis);
    CHECK_EQ(K_, new_K)
      << "Input size incompatible with inner product parameters.";
    // The first "axis" dimensions are independent inner products; the total
    // number of these is M_, the product over these dimensions.
    M_ = bottom[0]->count(0, axis);
    // The top shape will be the bottom shape with the flattened axes dropped,
    // and replaced by a single axis with dimension num_output (N_).
    DataShape top_shape = bottom[0]->shape();
    top_shape.resize(axis + 1);
    top_shape.dim[axis] = N_;
    top[0]->Reshape(top_shape);
    // Set up the bias multiplier
    if (bias_term_) {
      DataShape bias_shape = dataShape(M_);
      bias_multiplier_.Reshape(bias_shape);
      caffe_set(CONTEXT, M_, Dtype(1), bias_multiplier_.mutable_data<Context>());
    }
  }

  virtual void Forward(Context* context, const vector<Blob<Dtype>*> & bottom,  const vector<Blob<Dtype>*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data<Context>();
    Dtype* top_data = top[0]->mutable_data<Context>();
    const Dtype* weight = this->blobs_[0]->data<Context>();
    caffe_gemm<Dtype>(context, CblasNoTrans, transpose_ ? CblasNoTrans : CblasTrans,
      M_, N_, K_, (Dtype)1.,bottom_data, weight, (Dtype)0., top_data);
    if (bias_term_) {
      caffe_gemm<Dtype>(context, CblasNoTrans, CblasNoTrans, M_, N_, 1, (Dtype)1.,
        bias_multiplier_.data<Context>(),
        this->blobs_[1]->data<Context>(), (Dtype)1., top_data);
    }
  }


  virtual void Backward(Context* context, const vector<Blob<Dtype>*> & top,  const vector<Blob<Dtype>*> & bottom)
  {
    if (this->blobs_[0]->propagate_down_) {
      const Dtype* top_diff = top[0]->diff<Context>();
      const Dtype* bottom_data = bottom[0]->data<Context>();
      // Gradient with respect to weight
      if (transpose_) {
        caffe_gemm<Dtype>(context, CblasTrans, CblasNoTrans,
          K_, N_, M_,
          (Dtype)1., bottom_data, top_diff,
          (Dtype)1., this->blobs_[0]->mutable_diff<Context>());
      }
      else {
        caffe_gemm<Dtype>(context, CblasTrans, CblasNoTrans,
          N_, K_, M_,
          (Dtype)1., top_diff, bottom_data,
          (Dtype)1., this->blobs_[0]->mutable_diff<Context>());
      }
    }
    if (bias_term_ && this->blobs_[1]->propagate_down_) {
      const Dtype* top_diff = top[0]->diff<Context>();
      // Gradient with respect to bias
      caffe_gemv<Dtype>(context, CblasTrans, M_, N_, (Dtype)1., top_diff,
        bias_multiplier_.data<Context>(), (Dtype)1.,
        this->blobs_[1]->mutable_diff<Context>());
    }
    if (bottom[0]->propagate_down_) {
      const Dtype* top_diff = top[0]->diff<Context>();
      // Gradient with respect to bottom data
      if (transpose_) {
        caffe_gemm<Dtype>(context, CblasNoTrans, CblasTrans,
          M_, K_, N_,
          (Dtype)1., top_diff, this->blobs_[0]->data<Context>(),
          (Dtype)0., bottom[0]->mutable_diff<Context>());
      }
      else {
        caffe_gemm<Dtype>(context, CblasNoTrans, CblasNoTrans,
          M_, K_, N_,
          (Dtype)1., top_diff, this->blobs_[0]->data<Context>(),
          (Dtype)0., bottom[0]->mutable_diff<Context>());
      }
    }
  }
  
};

INSTANTIATE_CLASS(InnerProduct);



#endif  // CAFFE_INNER_PRODUCT_LAYER_HPP_
