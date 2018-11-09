#ifndef CAFFE_SOFTMAX_LAYER_HPP_
#define CAFFE_SOFTMAX_LAYER_HPP_


/**
 * @brief Computes the softmax function.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */
template <typename Dtype>
class SoftmaxLayer : public Layer<Dtype>
{
public:
  int outer_num_;
  int inner_num_;
  int softmax_axis_;
  /// sum_multiplier is used to carry out sum using BLAS
  //Blob<Dtype> sum_multiplier_;
  /// scale is an intermediate Blob to hold temporary results.
  Blob<Dtype> scale_;

  virtual inline const char* type() const { return "Softmax"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

  void Reshape(const vector<Blob<Dtype>*> & bottom,
    const vector<Blob<Dtype>*> & top)
  {
    int axis_ = param_->getint("axis", 1);
    softmax_axis_ = bottom[0]->CanonicalAxisIndex(axis_);
    top[0]->ReshapeLike(*bottom[0]);
    //vector<int> mult_dims(1, bottom[0]->shape(softmax_axis_));
    //sum_multiplier_.Reshape(mult_dims);
    outer_num_ = bottom[0]->count(0, softmax_axis_);
    inner_num_ = bottom[0]->count(softmax_axis_ + 1);
    DataShape scale_dims = bottom[0]->shape();
    scale_dims.dim[softmax_axis_] = 1;
    scale_.Reshape(scale_dims);
  }

  void Forward(Context* context, const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top)
  {
    int count = bottom[0]->count();
    int channels = bottom[0]->shape(softmax_axis_);
    int channels1 = top[0]->shape(softmax_axis_);
    const Dtype* bottom_data = bottom[0]->data<Context>();
    Dtype* top_data = top[0]->mutable_data<Context>();
    Dtype* scale_data = scale_.mutable_data<Context>();
    softmax_forward<Dtype>(context, count, channels, outer_num_, inner_num_, bottom_data, top_data, scale_data);
    return ;
  }

  
  void Backward(Context* context, const vector<Blob<Dtype>*> & top,  const vector<Blob<Dtype>*> & bottom)
  {
    const Dtype* top_diff = top[0]->diff<Context>();
    const Dtype* top_data = top[0]->data<Context>();
    Dtype* bottom_diff = bottom[0]->mutable_diff<Context>();
    Dtype* scale_data = scale_.mutable_data<Context>();
    int count = top[0]->count();
    int channels = top[0]->shape(softmax_axis_);
    softmax_backward<Dtype>(context, count, channels, outer_num_, inner_num_, top_diff, top_data, bottom_diff, scale_data);
  }
};

INSTANTIATE_CLASS(Softmax);

#endif  // CAFFE_SOFTMAX_LAYER_HPP_
