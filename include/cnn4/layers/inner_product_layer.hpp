#ifndef CAFFE_INNER_PRODUCT_LAYER_HPP_
#define CAFFE_INNER_PRODUCT_LAYER_HPP_

/**
 * @brief Also known as a "fully-connected" layer, computes an inner product
 *        with a set of learned weights, and (optionally) adds biases.
 *
 * TODO(dox): thorough documentation for Forward_, Backward_, and proto params.
 */
#define InnerProductParameter_DEF(DEF) \
DEF##Int(num_output, 0, 0) \
DEF##Int(axis, 1, 0) \
DEF##Bool(bias_term, true, 0) \
DEF##Bool(transpose, false, 0) \
DEF##Struct(weight_filler, 0, Filler) \
DEF##Struct(bias_filler, 0, Filler) \


struct InnerProductLayer : public Layer
{
  InnerProductParameter_DEF(Def);
  int M_;
  int K_;
  int N_;
  Blob bias_multiplier_;

  virtual inline const char* type() const { return "InnerProduct"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

  //bool has_data_;
  InnerProductLayer() {
    InnerProductParameter_DEF(Set);
  }
  void init(CJSON* param) {
    InnerProductParameter_DEF(Get);
    CJSON* blobs_json = param->get("blobs");
    //has_data_ = blobs_json && blobs_json->GetArraySize() > 0 && blobs_json->GetArrayItem(0)->has("data");
  }
  virtual void LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    N_ = num_output_;
    const int axis = bottom[0]->CanonicalAxisIndex(this->axis_);
    // Dimensions starting from "axis" are "flattened" into a single
    // length K_ vector. For example, if bottom[0]'s shape is (N, C, H, W),
    // and axis == 1, N inner products with dimension CHW are performed.
    K_ = bottom[0]->count(axis);
    // Check if we need to set up the weights
    if (blobs_.size()>0) {
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
      weight_filler_.Fill(this->blobs_[0]);
      // If necessary, intiialize and fill the bias term
      if (bias_term_) {
        DataShape bias_shape = dataShape(1, N_);
        this->blobs_[1]->Reshape(bias_shape);
        bias_filler_.Fill(this->blobs_[1]);
      }
    }  // parameter initialization
    //this->param_propagate_down_.resize(this->blobs_.size(), true);
  }
  
  virtual void Reshape(const vector<Blob*> & bottom,  const vector<Blob*> & top)
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
    // and replaced by a single axis with dimension num_output_ (N_).
    DataShape top_shape = bottom[0]->shape();
    top_shape.resize(axis + 1);
    top_shape.dim[axis] = N_;
    top[0]->Reshape(top_shape);
    // Set up the bias multiplier
    if (bias_term_) {
      DataShape bias_shape = dataShape(M_);
      bias_multiplier_.Reshape(bias_shape);
      caffe_set(M_, Dtype(1), bias_multiplier_.mdata());
    }
  }

  virtual void Forward_(const vector<Blob*> & bottom,  const vector<Blob*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* top_data = top[0]->mdata();
    const Dtype* weight = this->blobs_[0]->data();
    caffe_gemm(CblasNoTrans, transpose_ ? CblasNoTrans : CblasTrans,
      M_, N_, K_, (Dtype)1.,bottom_data, weight, (Dtype)0., top_data);
    if (bias_term_) {
      caffe_gemm(CblasNoTrans, CblasNoTrans, M_, N_, 1, (Dtype)1.,
        bias_multiplier_.data(),
        this->blobs_[1]->data(), (Dtype)1., top_data);
    }
  }


  virtual void Backward_(const vector<Blob*> & top,  const vector<Blob*> & bottom)
  {
    if (this->blobs_[0]->propagate_down_) {
      const Dtype* top_diff = top[0]->diff();
      const Dtype* bottom_data = bottom[0]->data();
      // Gradient with respect to weight
      if (transpose_) {
        caffe_gemm(CblasTrans, CblasNoTrans,
          K_, N_, M_,
          (Dtype)1., bottom_data, top_diff,
          (Dtype)1., this->blobs_[0]->mdiff());
      }
      else {
        caffe_gemm(CblasTrans, CblasNoTrans,
          N_, K_, M_,
          (Dtype)1., top_diff, bottom_data,
          (Dtype)1., this->blobs_[0]->mdiff());
      }
    }
    if (bias_term_ && this->blobs_[1]->propagate_down_) {
      const Dtype* top_diff = top[0]->diff();
      // Gradient with respect to bias
      caffe_gemv(CblasTrans, M_, N_, (Dtype)1., top_diff,
        bias_multiplier_.data(), (Dtype)1.,
        this->blobs_[1]->mdiff());
    }
    if (bottom[0]->propagate_down_) {
      const Dtype* top_diff = top[0]->diff();
      // Gradient with respect to bottom data
      if (transpose_) {
        caffe_gemm(CblasNoTrans, CblasTrans,
          M_, K_, N_,
          (Dtype)1., top_diff, this->blobs_[0]->data(),
          (Dtype)0., bottom[0]->mdiff());
      }
      else {
        caffe_gemm(CblasNoTrans, CblasNoTrans,
          M_, K_, N_,
          (Dtype)1., top_diff, this->blobs_[0]->data(),
          (Dtype)0., bottom[0]->mdiff());
      }
    }
  }
  
};

INSTANTIATE_CLASS(InnerProduct);



#endif  // CAFFE_INNER_PRODUCT_LAYER_HPP_
