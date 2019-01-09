#ifndef WARP_CTC_LOSS_LAYER_HPP
#define WARP_CTC_LOSS_LAYER_HPP

#include <list>

#define WarpCTCLossParameter_DEF(DEF) \
DEF##Int(blank_index, 0, 0) \



class WarpCTCLossLayer : public LossLayer
{
public:
  int T_;
  int N_;
  int C_;

  // blank index of input sequence, set to -1 for last
  // if set to 0, the 'real' labels must start at 1

  vector<int> flat_labels_;
  vector<int> label_lengths_;
  vector<int> input_lengths_;

  DevMem workspace_[1];

public:
  explicit WarpCTCLossLayer() {
    T_ = N_ = C_ = 0;
    workspace_->init();
    init();
  }

  virtual ~WarpCTCLossLayer() {
    workspace_->free();
  }

  WarpCTCLossParameter_DEF(Def);
  virtual void init() {
    WarpCTCLossParameter_DEF(Init);
  }
  virtual void fromJson(cjson* param) {
    WarpCTCLossParameter_DEF(Get);
  }
  virtual void toJson(cjson* param) {
    WarpCTCLossParameter_DEF(Set);
  }

  virtual void LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    LossLayer::LayerSetUp(bottom, top);
    const Blob* probs = bottom[0];//TxNxC
    T_ = probs->num();
    N_ = probs->channels();
    C_ = probs->height();
    CHECK_EQ(probs->width(), 1);
    if (bottom.size() == 3) {
      const Blob* seq_ind = bottom[1];
      const Blob* label_seq = bottom[2];
      CHECK_EQ(T_, seq_ind->num());
      CHECK_EQ(N_, seq_ind->channels());
      CHECK_EQ(N_, label_seq->channels());
    }
    else if (bottom.size() == 4) {
      const Blob* seq_len_blob = bottom[1];
      const Blob* lab_len_blob = bottom[2];
      const Blob* label_seq_blob = bottom[3];
      CHECK_EQ(N_, seq_len_blob->count());
      CHECK_EQ(N_, lab_len_blob->count());
      CHECK_EQ(N_, label_seq_blob->channels());
    }
    else if (bottom.size() == 2) { //input seq + labels
      const Blob* label_seq = bottom[1];
      CHECK_EQ(N_, label_seq->num());
    }
    else {
      LOG(FATAL) << "Unsupported blobs shape";
    }
    label_lengths_.resize(N_);
    input_lengths_.resize(N_);
  }

  virtual void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    vector<int> loss_shape(0);  // Loss layers output a scalar; 0 axes.
    top[0]->Reshape(loss_shape);
  }

  virtual inline const char* type() const { return "WarpCTCLoss"; }

  // probabilities, sequence indicators, target sequence
  virtual inline int MinBottomBlobs() const { return 2; }
  virtual inline int MaxBottomBlobs() const { return 4; }
  virtual inline int ExactNumBottomBlobs() const { return -1; }

  // loss
  virtual inline int ExactNumTopBlobs() const { return 1; }

public:
  /**
   * @brief Computes the loss and the error gradients for the input data
   *        in one step (due to optimization isses)
   *
   * @param bottom input Blob vector (length 3)
   *   -# @f$ (T \times N \times C) @f$
   *      the inputs @f$ x @f$
   *   -# @f$ (T \times N) @f$
   *      the sequence indicators for the data
   *      (must be 0 at @f$ t = 0 @f$ and 1 during a sequence)
   *   -# @f$ (T \times N) @f$
   *      the target sequence
   *      (must start at @f$ t = 0 @f$ and filled with -1 if the sequence has ended)
   * @param top output Blob vector (length 1)
   *   -# @f$ (1) @f$
   *      the computed loss
   */

  virtual void Forward_(const vector<Blob*> & bottom, const vector<Blob*> & top) {
    int count = bottom[1]->count();
    const Dtype* bottom0_data = bottom[0]->data();
    Dtype* bottom0_mdiff = bottom[0]->mdiff();
    int bottom_size = bottom.size();
    const Dtype* bottom1_data = bottom_size>1 ? bottom[1]->cpu_data() : NULL;
    const Dtype* bottom2_data = bottom_size>2 ? bottom[2]->cpu_data() : NULL;
    const Dtype* bottom3_data = bottom_size>3 ? bottom[3]->cpu_data() : NULL;
    Dtype* top_data = top[0]->cpu_mdata();
    warp_ctc_loss_fwd(T_, N_, C_, count, blank_index_, bottom0_data, bottom0_mdiff, bottom1_data, bottom2_data, bottom3_data, top_data);
    return;
  }

  /**
   * @brief Unused. Gradient calculation is done in Forward_cpu
   */
  virtual void Backward_(const vector<Blob*> & top, const vector<Blob*> & bottom) {
    CHECK_EQ(bottom[0]->propagate_down_, true) << "Required to propagate to probabilities";
    if (bottom.size() >= 3) {
      CHECK_EQ(bottom[1]->propagate_down_, false) << "Cannot propagate to sequence indicators";
      CHECK_EQ(bottom[2]->propagate_down_, false) << "Cannot propagate to target label sequence";
    }
    else if (bottom.size() == 2) {
      //CHECK_EQ(bottom[1]->propagate_down_, false) << "Cannot propagate to target label sequence";
    }
  }

};


INSTANTIATE_CLASS(WarpCTCLoss, WarpCTCLossLayer);


#endif // WARP_CTC_LOSS_LAYER_HPP
