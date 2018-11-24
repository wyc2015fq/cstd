#ifndef CAFFE_RELU_LAYER_HPP_
#define CAFFE_RELU_LAYER_HPP_

#define ReLUParameter_DEF(DEF) \
DEF##Float(negative_slope, 0, 0) \

class ReLULayer : public NeuronLayer
{
public:
  ReLUParameter_DEF(Def);
  virtual inline const char* type() const { return "ReLU"; }

  ReLULayer() {
    ReLUParameter_DEF(Set);
  }
  void init(CJSON* param) {
    ReLUParameter_DEF(Get);
  }
  virtual void Forward_(const vector<Blob*> & bottom,
    const vector<Blob*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* top_data = top[0]->mdata();
    const int count = bottom[0]->count();
    relu_forward(count, bottom_data, top_data, negative_slope_);
  }

  virtual void Backward_(const vector<Blob*> & top,
    const vector<Blob*> & bottom)
  {
    if (bottom[0]->propagate_down_) {
      const Dtype* bottom_data = bottom[0]->data();
      const Dtype* top_diff = top[0]->diff();
      Dtype* bottom_diff = bottom[0]->mdiff();
      const int count = bottom[0]->count();
      relu_backward(count, top_diff, bottom_data, bottom_diff, negative_slope_);
    }
  }
};
;

INSTANTIATE_CLASS(ReLU, ReLULayer);

#endif  // CAFFE_RELU_LAYER_HPP_
