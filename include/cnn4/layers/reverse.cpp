#include "caffe/layers/reverse_layer.hpp"



namespace
{

  template <typename Dtype>
  ReverseLayer::ReverseLayer()
    : NeuronLayer(param)
    , axis_(param.reverse_param().axis())
  {
    CHECK_GE(axis_, 0);
  }

  template <typename Dtype>
  void ReverseLayer::LayerSetUp(const vector<Blob*> & bottom,
                                       const vector<Blob*> & top)
  {
    NeuronLayer::LayerSetUp(bottom, top);
    CHECK_NE(top[0], bottom[0]) << this->type() << " Layer does not "
                                "allow in-place computation.";
    CHECK_LT(axis_, bottom[0]->num_axes())
        << "Axis must be less than the number of axis for reversing";
  }

  template <typename Dtype>
  void ReverseLayer::Forward_(_CONTEXT,
    const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    const Dtype* src = bottom[0]->data();
    const int count = top[0]->count();
    const int axis_count = top[0]->count(axis_);
    const int copy_amount
      = (axis_ + 1 == top[0]->num_axes()) ? 1 : top[0]->count(axis_ + 1);
    const int num_fix = (axis_ > 0) ? count / axis_count : 1;
    const int sub_iter_max = top[0]->shape(axis_);
    for (int fix = 0; fix < num_fix; ++fix) {
      Dtype* target = top[0]->mdata()
                      + (fix + 1) * copy_amount * sub_iter_max - copy_amount;
      for (int i = 0; i < sub_iter_max; ++i) {
        caffe_copy(copy_amount, src, target);
        src += copy_amount;     // normal order
        target -= copy_amount;
      }
    }
  }

  template <typename Dtype>
  void ReverseLayer::Backward_(CPUContext* context, const vector<Blob*> & top,
                                         const vector<Blob*> & bottom)
  {
    if (!bottom[0]->propagate_down_) { return; }
    Dtype* target = bottom[0]->mdiff();
    const int count = top[0]->count();
    const int axis_count = top[0]->count(axis_);
    const int copy_amount =
      (axis_ + 1 == top[0]->num_axes()) ? 1 : top[0]->count(axis_ + 1);
    const int num_fix = (axis_ > 0) ? count / axis_count : 1;
    const int sub_iter_max = top[0]->shape(axis_);
    for (int fix = 0; fix < num_fix; ++fix) {
      const Dtype* src
        = top[0]->diff() + (fix + 1) * copy_amount * sub_iter_max
          - copy_amount;
      for (int i = 0; i < sub_iter_max; ++i) {
        caffe_copy(copy_amount, src, target);
        target += copy_amount;  // normal order
        src -= copy_amount;     // reverse order
      }
    }
  }

#ifdef CPU_ONLY
  STUB_GPU(ReverseLayer);
#endif

  INSTANTIATE_CLASS(ReverseLayer);
  REGISTER_LAYER_CLASS(Reverse);

}  // namespace
