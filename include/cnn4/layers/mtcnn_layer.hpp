






#include "caffe/layers/loss_layer.hpp"

namespace
{


  template <typename Dtype>
  class RegressionLayer : public LossLayer
  {
  public:
    explicit RegressionLayer()
      : LossLayer(param), diff_() {}
    virtual void Reshape(const vector<Blob*> & bottom,
      const vector<Blob*> & top) {
      LossLayer::Reshape(bottom, top);
      CHECK_EQ(bottom[0]->count(1), bottom[1]->count(1))
        << "Inputs must have the same dimension.";
      //if (bottom[0]->count(1) != bottom[1]->count(1)) {        printf("adfasdf");      }
      diff_.ReshapeLike(*bottom[0]);
    }

    virtual inline const char* type() const { return "Regression"; }
    /**
    * Unlike most loss layers, in the RegressionLayer we can backpropagate
    * to both inputs -- override to return true and always allow force_backward.
    */
    virtual inline bool AllowForceBackward(const int bottom_index) const {
      return true;
    }

  public:
    /// @copydoc RegressionLayer
    virtual void Forward(CPUContext* context, const vector<Blob*> & bottom,
      const vector<Blob*> & top) {
      int count = bottom[0]->count();
      //caffe_sub( count, bottom[0]->data(), bottom[1]->data(), diff_.mutable_data());
      //const Dtype* roi = bottom[1]->data();
      int num = bottom[1]->countL(1);
      int len = bottom[1]->countH(1);
      for (int i = 0; i < num; ++i) {
        const Dtype* bottom0_data = bottom[0]->data() + i*len;
        const Dtype* bottom1_data = bottom[1]->data() + i*len;
        Dtype* diff_data = diff_.mutable_data() + i*len;
        if (bottom1_data[0]>(0-0.0001)) {
          for (int j = 0; j < len; ++j) {
            diff_data[j] = bottom0_data[j] - bottom1_data[j];
          }
        }
        else {
          for (int j = 0; j < len; ++j) {
            diff_data[j] = Dtype(0);
          }
        }
      }
      Dtype dot = caffe_dot(count, diff_.data(), diff_.data());
      Dtype loss = dot / bottom[0]->num() / Dtype(2);
      top[0]->mutable_data()[0] = loss;
    }
    virtual void Forward(GPUContext* context, const vector<Blob*> & bottom,
      const vector<Blob*> & top);


    virtual void Backward(CPUContext* context, const vector<Blob*> & top,
      const vector<Blob*> & bottom) {
      for (int i = 0; i < 2; ++i) {
        if (bottom[i]->propagate_down_) {
          const Dtype sign = (i == 0) ? 1 : -1;
          const Dtype alpha = sign * top[0]->diff()[0] / bottom[i]->num();
          caffe_axpby(
            bottom[i]->count(),              // count
            alpha,                              // alpha
            diff_.data(),                   // a
            Dtype(0),                           // beta
            bottom[i]->mutable_diff());  // b
        }
      }
    }
    virtual void Backward(GPUContext* context, const vector<Blob*> & top,
      const vector<Blob*> & bottom);

    Blob diff_;
  };
#ifdef CPU_ONLY
  STUB_GPU(RegressionLayer);
#endif

  INSTANTIATE_CLASS(RegressionLayer);
  REGISTER_LAYER_CLASS(Regression);


  template <typename Dtype>
  class ClsFcLayer : public Layer
  {
  public:
    explicit ClsFcLayer()
      : Layer() {}
    virtual inline const char* type() const { return "ClsFcLayer"; }
    virtual inline bool AllowForceBackward(const int bottom_index) const {
      return true;
    }
    virtual void LayerSetUp(const vector<Blob*> & bottom,
      const vector<Blob*> & top) {
      CHECK_EQ(bottom.size(), 2) << ("Need 2 Inputs");
    }
    virtual void Reshape(const vector<Blob*> & bottom,
      const vector<Blob*> & top) {
      //label = bottom[1].data;
      //self.valid_index = np.where(label != -1)[0];
      //self.count = len(self.valid_index);
      int len = bottom[1]->count();
      top[0]->ReshapeLike(*bottom[0]);
      top[1]->ReshapeLike(*bottom[1]);
    }
    virtual void Forward(CPUContext* context, const vector<Blob*> & bottom,
      const vector<Blob*> & top) {
      int num = bottom[1]->countL(1);
      int len0 = bottom[0]->countH(1);
      int len1 = bottom[1]->countH(1);

      for (int i = 0; i < num; ++i) {
        const Dtype* bottom0_data = bottom[0]->data() + i*len0;
        const Dtype* bottom1_data = bottom[1]->data() + i*len1;
        Dtype* top0_data = top[0]->mutable_data() + i*len0;
        Dtype* top1_data = top[1]->mutable_data() + i*len1;
        if (bottom1_data[0] >= (0 - 0.00001)) {
          for (int j = 0; j < len0; ++j) {
            top0_data[j] = bottom0_data[j];
          }
          for (int j = 0; j < len1; ++j) {
            top1_data[j] = bottom1_data[j];
          }
        }
        else {
          for (int j = 0; j < len0; ++j) {
            top0_data[j] = Dtype(0);
          }
          for (int j = 0; j < len1; ++j) {
            top1_data[j] = Dtype(0);
          }
        }
      }
    }
    virtual void Backward(CPUContext* context, const vector<Blob*> & top,
      const vector<Blob*> & bottom) {
      int num = bottom[1]->countL(1);
      int len0 = bottom[0]->countH(1);
      int len1 = bottom[1]->countH(1);
      if (bottom[0]->propagate_down_) {
        for (int i = 0; i < num; ++i) {
          Dtype* bottom0_diff = bottom[0]->mutable_diff() + i*len0;
          const Dtype* bottom1_data = bottom[1]->data() + i*len1;
          const Dtype* top0_diff = top[0]->diff() + i*len0;
          if (bottom1_data[0] >= (0 - 0.00001)) {
            for (int j = 0; j < len0; ++j) {
              bottom0_diff[j] = top0_diff[j];
            }
          }
          else {
            for (int j = 0; j < len0; ++j) {
              bottom0_diff[j] = Dtype(0);
            }
          }
        }
      }
      if (bottom[1]->propagate_down_) {
      }
    }
};
  INSTANTIATE_CLASS(ClsFcLayer);
  REGISTER_LAYER_CLASS(ClsFc);

}