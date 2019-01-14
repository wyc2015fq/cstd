#ifndef CAFFE_DROPOUT_LAYER_HPP_
#define CAFFE_DROPOUT_LAYER_HPP_


#define DropoutParameter_DEF(DEF) \
DEF##Float(dropout_ratio, 0.5, 0)

class DropoutLayer : public NeuronLayer
{
public:
  DropoutParameter_DEF(Def);
  /// when divided by UINT_MAX, the randomly generated values @f$u\sim U(0,1)@f$
  Blob rand_vec_;
  /// the probability @f$ p @f$ of dropping any input
  //Dtype dropout_ratio_;
  /// the scale for undropped inputs at train time @f$ 1 / (1 - p) @f$

  virtual inline const char* type() const { return "Dropout"; }
public:
  DropoutLayer() {
    DropoutParameter_DEF(Init);
  }
  virtual void init() {
    DropoutParameter_DEF(Init);
  }
  virtual void fromJson(cjson* param) {
    DropoutParameter_DEF(Get);
  }
  virtual void toJson(cjson* param) {
    DropoutParameter_DEF(Set);
  }


  virtual void LayerSetUp(const vector<Blob*> & bottom,
    const vector<Blob*> & top)
  {
    NeuronLayer::LayerSetUp(bottom, top);
    DCHECK(dropout_ratio_ > 0.);
    DCHECK(dropout_ratio_ < 1.);
  }

  virtual void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    NeuronLayer::Reshape(bottom, top);
    // Set up the cache for random number generation
    // ReshapeLike does not work because rand_vec_ is of Dtype uint
    rand_vec_.Reshape(bottom[0]->shape());
  }

  virtual void Forward_(const vector<Blob*>& bottom, const vector<Blob*>& top) {
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* top_data = top[0]->mdata();
    const int count = bottom[0]->count();
    Dtype scale_ = 1. / (1. - dropout_ratio_);
    if (this->phase_cur_ == TRAIN) {
      unsigned int* mask = (unsigned int*)(rand_vec_.mdata());
      // set thresholds
      // NOLINT_NEXT_LINE(whitespace/operators)
      dropout_forward(count, bottom_data, mask, dropout_ratio_, Dtype(scale_), top_data);
      //CUDA_POST_KERNEL_CHECK;
    }
    else {
      caffe_copy(count, bottom_data, top_data);
    }
  }

  virtual void Backward_(const vector<Blob*>& top, const vector<Blob*>& bottom) {
    if (bottom[0]->propagate_down_) {
      const Dtype* top_diff = top[0]->diff();
      Dtype* bottom_diff = bottom[0]->mdiff();
      Dtype scale_ = 1. / (1. - dropout_ratio_);
      if (this->phase_cur_ == TRAIN) {
        const unsigned int* mask = (const unsigned int*)(rand_vec_.data());
        const int count = bottom[0]->count();
        // NOLINT_NEXT_LINE(whitespace/operators)
        dropout_backward(count, top_diff, mask, Dtype(scale_), bottom_diff);
        //CUDA_POST_KERNEL_CHECK;
      }
      else {
        caffe_copy(top[0]->count(), top_diff, bottom_diff);
      }
    }
  }

};

INSTANTIATE_CLASS(Dropout, DropoutLayer);

#endif  // CAFFE_DROPOUT_LAYER_HPP_
