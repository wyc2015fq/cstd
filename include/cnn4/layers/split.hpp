#ifndef CAFFE_SPLIT_LAYER_HPP_
#define CAFFE_SPLIT_LAYER_HPP_



class SplitLayer : public Layer
{
public:
  int count_;

  virtual inline const char* type() const { return "Split"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int MinTopBlobs() const { return 1; }

  SplitLayer() {}
  virtual void init() {
  }
  virtual void fromJson(cjson* param) {
  }
  virtual void toJson(cjson* param) {
  }


  virtual void Reshape(const vector<Blob*> & bottom,
    const vector<Blob*> & top)
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

  virtual void Forward_(const vector<Blob*> & bottom,
    const vector<Blob*> & top)
  {
    for (int i = 0; i < top.size(); ++i) {
      top[i]->ShareData(bottom[0]);
    }
  }

  virtual void Backward_(const vector<Blob*> & top,
    const vector<Blob*> & bottom)
  {
    if (!bottom[0]->propagate_down_) { return; }
    if (top.size() == 1) {
      caffe_copy(count_, top[0]->diff(), bottom[0]->mdiff());
      return;
    }
    caffe_add(count_, top[0]->diff(), top[1]->diff(),
      bottom[0]->mdiff());
    // Add remaining top blob diffs.
    for (int i = 2; i < top.size(); ++i) {
      const Dtype* top_diff = top[i]->diff();
      Dtype* bottom_diff = bottom[0]->mdiff();
      caffe_axpy(count_, Dtype(1.), top_diff, bottom_diff);
    }
  }

};

INSTANTIATE_CLASS(Split, SplitLayer);


#endif  // CAFFE_SPLIT_LAYER_HPP_
