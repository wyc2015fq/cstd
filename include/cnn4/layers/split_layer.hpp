#ifndef CAFFE_SPLIT_LAYER_HPP_
#define CAFFE_SPLIT_LAYER_HPP_

/**
 * @brief Creates a "split" path in the network by copying the bottom Blob
 *        into multiple top Blob%s to be used by multiple consuming layers.
 *
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */
template <typename Dtype>
class SplitLayer : public Layer<Dtype>
{
public:
  int count_;

  virtual inline const char* type() const { return "Split"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int MinTopBlobs() const { return 1; }

  virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
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

  virtual void Forward(Context* context, const vector<Blob<Dtype>*> & bottom,
    const vector<Blob<Dtype>*> & top)
  {
    for (int i = 0; i < top.size(); ++i) {
      top[i]->ShareData(*bottom[0]);
    }
  }

  virtual void Backward(Context* context, const vector<Blob<Dtype>*> & top,
    const vector<Blob<Dtype>*> & bottom)
  {
    if (!bottom[0]->propagate_down_) { return; }
    if (top.size() == 1) {
      caffe_copy(context, count_, top[0]->diff<Context>(), bottom[0]->mutable_diff<Context>());
      return;
    }
    caffe_add(context, count_, top[0]->diff<Context>(), top[1]->diff<Context>(),
      bottom[0]->mutable_diff<Context>());
    // Add remaining top blob diffs.
    for (int i = 2; i < top.size(); ++i) {
      const Dtype* top_diff = top[i]->diff<Context>();
      Dtype* bottom_diff = bottom[0]->mutable_diff<Context>();
      caffe_axpy(context, count_, Dtype(1.), top_diff, bottom_diff);
    }
  }

};

INSTANTIATE_CLASS(Split);


#endif  // CAFFE_SPLIT_LAYER_HPP_
