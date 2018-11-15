#include "caffe/layers/reverse_time_layer.hpp"



namespace
{

  template <typename Dtype>
  ReverseTimeLayer::ReverseTimeLayer()
    : Layer(param)
    , copy_remaining_(param.reverse_time_param().copy_remaining())
  {
  }

  template <typename Dtype>
  void ReverseTimeLayer::LayerSetUp(const vector<Blob*> & bottom,
      const vector<Blob*> & top)
  {
    Layer::LayerSetUp(bottom, top);
    CHECK_NE(top[0], bottom[0]) << this->type() << " Layer does not "
                                "allow in-place computation.";
    // copy to an expected length only
    CHECK_EQ(bottom[1]->num_axes(), 1) << "Only one axis allowed for sequence length";
    CHECK_EQ(bottom[1]->count(), bottom[0]->shape(1));
  }

  template <typename Dtype>
  void ReverseTimeLayer::Reshape(
    const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    top[0]->ReshapeLike(*bottom[0]);
  }

  template <typename Dtype>
  void ReverseTimeLayer::Forward_(_CONTEXT,
    const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    const Dtype* src = bottom[0]->data();
    Dtype* const dest = top[0]->mdata();
    // TODO: Remove these tests
    const Dtype* const src_max = src + bottom[0]->count();
    const Dtype* const dest_max = dest + top[0]->count();
    const int count = top[0]->count();
    const int copy_amount = top[0]->count(1);
    const int sub_iter_max = top[0]->shape(0);
    const Dtype* seq_length = bottom[1]->data();
    const int sub_axis_count = bottom[0]->shape(1);
    const int sub_copy_amount = copy_amount / sub_axis_count;
    for (int n = 0; n < sub_axis_count; ++n) {
      const Dtype* sub_src = src + n * sub_copy_amount;
      const int sub_seq_length = seq_length[n];
      Dtype* target = dest + copy_amount * sub_seq_length - copy_amount
                      + n * sub_copy_amount;
      // invert only until sub_seq_length, and copy rest:
      // 1. invert
      for (int t = 0; t < sub_seq_length; ++t) {
        // TODO: remove this test
        DCHECK_GE(sub_src, src);
        DCHECK_GE(target, dest);
        DCHECK_LE(sub_src + sub_copy_amount, src_max);
        DCHECK_LE(target + sub_copy_amount, dest_max);
        caffe_copy(sub_copy_amount, sub_src, target);
        sub_src += copy_amount;
        target -= copy_amount;
      }
      // 2. copy rest
      if (copy_remaining_) {
        Dtype* target = dest + (sub_seq_length) * copy_amount
                        + n * sub_copy_amount;
        for (int t = sub_seq_length; t < sub_iter_max; ++t) {
          // TODO: remove this test
          DCHECK_GE(sub_src, src);
          DCHECK_GE(target, dest);
          DCHECK_LE(sub_src + sub_copy_amount, src_max);
          DCHECK_LE(target + sub_copy_amount, dest_max);
          caffe_copy(sub_copy_amount, sub_src, target);
          sub_src += copy_amount;
          target += copy_amount;
        }
      }
    }
  }

  template <typename Dtype>
  void ReverseTimeLayer::Backward_(CPUContext* context, const vector<Blob*> & top,
      const vector<Blob*> & bottom)
  {
    if (!bottom[0]->propagate_down_) { return; }
    const Dtype* src = top[0]->diff();
    Dtype* const dest = bottom[0]->mdiff();
    // TODO: Remove these tests
    const Dtype* const src_max = src + top[0]->count();
    const Dtype* const dest_max = dest + bottom[0]->count();
    const int count = top[0]->count();
    const int copy_amount = top[0]->count(1);
    const int sub_iter_max = top[0]->shape(0);
    const Dtype* seq_length = bottom[1]->data();
    const int sub_axis_count = bottom[0]->shape(1);
    const int sub_copy_amount = copy_amount / sub_axis_count;
    for (int n = 0; n < sub_axis_count; ++n) {
      const Dtype* sub_src = src + n * sub_copy_amount;
      const int sub_seq_length = seq_length[n];
      Dtype* target = dest + copy_amount * sub_seq_length - copy_amount
                      + n * sub_copy_amount;
      // invert only until sub_seq_length, and copy rest:
      // 1. invert
      for (int t = 0; t < sub_seq_length; ++t) {
        // TODO: remove this test
        DCHECK_GE(sub_src, src);
        DCHECK_GE(target, dest);
        DCHECK_LE(sub_src + sub_copy_amount, src_max);
        DCHECK_LE(target + sub_copy_amount, dest_max);
        caffe_copy(sub_copy_amount, sub_src, target);
        sub_src += copy_amount;
        target -= copy_amount;
      }
      // 2. copy rest
      if (copy_remaining_) {
        Dtype* target = dest + (sub_seq_length) * copy_amount
                        + n * sub_copy_amount;
        for (int t = sub_seq_length; t < sub_iter_max; ++t) {
          // TODO: remove this test
          DCHECK_GE(sub_src, src);
          DCHECK_GE(target, dest);
          DCHECK_LE(sub_src + sub_copy_amount, src_max);
          DCHECK_LE(target + sub_copy_amount, dest_max);
          caffe_copy(sub_copy_amount, sub_src, target);
          sub_src += copy_amount;
          target += copy_amount;
        }
      }
    }
  }

#ifdef CPU_ONLY
  STUB_GPU(ReverseTimeLayer);
#endif

  INSTANTIATE_CLASS(ReverseTimeLayer);
  REGISTER_LAYER_CLASS(ReverseTime);

}  // namespace
