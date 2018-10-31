

#include "caffe/layers/split_layer.hpp"


namespace
{

  template <typename Dtype>
  void SplitLayer<Dtype>::Reshape(const vector<Blob<Dtype>*> & bottom,
                                  const vector<Blob<Dtype>*> & top)
  {
    count_ = bottom[0]->count();
    for (int i = 0; i < top.size(); ++i) {
      // Do not allow in-place computation in the SplitLayer.  Instead, share data
      // by reference in the forward pass, and keep separate diff allocations in
      // the backward pass.  (Technically, it should be possible to share the diff
      // blob of the first split output with the input, but this seems to cause
      // some strange effects in practice...)
      CHECK_NE(top[i], bottom[0]) << this->type() << " Layer does not "
                                  "allow in-place computation.";
      top[i]->ReshapeLike(*bottom[0]);
      CHECK_EQ(count_, top[i]->count());
    }
  }

  template <typename Dtype>
  void SplitLayer<Dtype>::Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
                                      const vector<Blob<Dtype>*> & top)
  {
    for (int i = 0; i < top.size(); ++i) {
      top[i]->ShareData(*bottom[0]);
    }
  }

  template <typename Dtype>
  void SplitLayer<Dtype>::Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
                                       const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom)
  {
    if (!propagate_down[0]) { return; }
    if (top.size() == 1) {
      caffe_copy(count_, top[0]->diff<Context>(), bottom[0]->mutable_diff<Context>());
      return;
    }
    caffe_add(count_, top[0]->diff<Context>(), top[1]->diff<Context>(),
              bottom[0]->mutable_diff<Context>());
    // Add remaining top blob diffs.
    for (int i = 2; i < top.size(); ++i) {
      const Dtype* top_diff = top[i]->diff<Context>();
      Dtype* bottom_diff = bottom[0]->mutable_diff<Context>();
      caffe_axpy(count_, Dtype(1.), top_diff, bottom_diff);
    }
  }


#ifdef CPU_ONLY
  STUB_GPU(SplitLayer);
#endif

  INSTANTIATE_CLASS(SplitLayer);
  REGISTER_LAYER_CLASS(Split);

}  // namespace
