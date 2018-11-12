

#include "caffe/layers/absval_layer.hpp"


namespace
{

  template <typename Dtype>
  void AbsValLayer::LayerSetUp(const vector<Blob*> & bottom,
                                      const vector<Blob*> & top)
  {
    NeuronLayer::LayerSetUp(bottom, top);
    CHECK_NE(top[0], bottom[0]) << this->type() << " Layer does not "
                                "allow in-place computation.";
  }

  template <typename Dtype>
  void AbsValLayer::Forward(_CONTEXT,
    const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    const int count = top[0]->count();
    Dtype* top_data = top[0]->mutable_data();
    caffe_abs(count, bottom[0]->data(), top_data);
  }

  template <typename Dtype>
  void AbsValLayer::Backward(CPUContext* context, const vector<Blob*> & top,
                                        const vector<Blob*> & bottom)
  {
    const int count = top[0]->count();
    const Dtype* top_diff = top[0]->diff();
    if (bottom[0]->propagate_down_) {
      const Dtype* bottom_data = bottom[0]->data();
      Dtype* bottom_diff = bottom[0]->mutable_diff();
      caffe_sign(count, bottom_data, bottom_diff);
      caffe_mul(count, bottom_diff, top_diff, bottom_diff);
    }
  }

#ifdef CPU_ONLY
  STUB_GPU(AbsValLayer);
#endif

  INSTANTIATE_CLASS(AbsValLayer);
  REGISTER_LAYER_CLASS(AbsVal);

}  // namespace
