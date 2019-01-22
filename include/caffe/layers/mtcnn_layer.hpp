
#include <vector>

#include "caffe/blob.hpp"
#include "caffe/layer.hpp"
#include "caffe/proto/caffe.pb.h"

#include "caffe/layers/loss_layer.hpp"

namespace caffe
{


  template <typename Dtype>
  class RegressionLayer : public LossLayer<Dtype>
  {
  public:
    explicit RegressionLayer(const LayerParameter & param)
      : LossLayer<Dtype>(param), diff_() {}
    virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
      const vector<Blob<Dtype>*> & top) {
      LossLayer<Dtype>::Reshape(bottom, top);
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

  protected:
    /// @copydoc RegressionLayer
    virtual void Forward_cpu(const vector<Blob<Dtype>*> & bottom,
      const vector<Blob<Dtype>*> & top) {
      int count = bottom[0]->count();
      //caffe_sub( count, bottom[0]->cpu_data(), bottom[1]->cpu_data(), diff_.mutable_cpu_data());
      //const Dtype* roi = bottom[1]->cpu_data();
      int num = bottom[1]->countL(1);
      int len = bottom[1]->countH(1);
      for (int i = 0; i < num; ++i) {
        const Dtype* bottom0_data = bottom[0]->cpu_data() + i*len;
        const Dtype* bottom1_data = bottom[1]->cpu_data() + i*len;
        Dtype* diff_data = diff_.mutable_cpu_data() + i*len;
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
      Dtype dot = caffe_cpu_dot(count, diff_.cpu_data(), diff_.cpu_data());
      Dtype loss = dot / bottom[0]->num() / Dtype(2);
      top[0]->mutable_cpu_data()[0] = loss;
    }
    virtual void Forward_gpu(const vector<Blob<Dtype>*> & bottom,
      const vector<Blob<Dtype>*> & top);


    virtual void Backward_cpu(const vector<Blob<Dtype>*> & top,
      const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom) {
      for (int i = 0; i < 2; ++i) {
        if (propagate_down[i]) {
          const Dtype sign = (i == 0) ? 1 : -1;
          const Dtype alpha = sign * top[0]->cpu_diff()[0] / bottom[i]->num();
          caffe_cpu_axpby(
            bottom[i]->count(),              // count
            alpha,                              // alpha
            diff_.cpu_data(),                   // a
            Dtype(0),                           // beta
            bottom[i]->mutable_cpu_diff());  // b
        }
      }
    }
    virtual void Backward_gpu(const vector<Blob<Dtype>*> & top,
      const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom);

    Blob<Dtype> diff_;
  };
#ifdef CPU_ONLY
  STUB_GPU(RegressionLayer);
#endif

  INSTANTIATE_CLASS(RegressionLayer);
  REGISTER_LAYER_CLASS(Regression);


  template <typename Dtype>
  class ClsFcLayer : public Layer<Dtype>
  {
  public:
    explicit ClsFcLayer(const LayerParameter & param)
      : Layer<Dtype>(param) {}
    virtual inline const char* type() const { return "ClsFcLayer"; }
    virtual inline bool AllowForceBackward(const int bottom_index) const {
      return true;
    }
    virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom,
      const vector<Blob<Dtype>*> & top) {
      CHECK_EQ(bottom.size(), 2) << ("Need 2 Inputs");
    }
    virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
      const vector<Blob<Dtype>*> & top) {
      //label = bottom[1].data;
      //self.valid_index = np.where(label != -1)[0];
      //self.count = len(self.valid_index);
      int len = bottom[1]->count();
      top[0]->ReshapeLike(*bottom[0]);
      top[1]->ReshapeLike(*bottom[1]);
    }
    virtual void Forward_cpu(const vector<Blob<Dtype>*> & bottom,
      const vector<Blob<Dtype>*> & top) {
      int num = bottom[1]->countL(1);
      int len0 = bottom[0]->countH(1);
      int len1 = bottom[1]->countH(1);

      for (int i = 0; i < num; ++i) {
        const Dtype* bottom0_data = bottom[0]->cpu_data() + i*len0;
        const Dtype* bottom1_data = bottom[1]->cpu_data() + i*len1;
        Dtype* top0_data = top[0]->mutable_cpu_data() + i*len0;
        Dtype* top1_data = top[1]->mutable_cpu_data() + i*len1;
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
    virtual void Backward_cpu(const vector<Blob<Dtype>*> & top,
      const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom) {
      int num = bottom[1]->countL(1);
      int len0 = bottom[0]->countH(1);
      int len1 = bottom[1]->countH(1);
      if (propagate_down[0]) {
        for (int i = 0; i < num; ++i) {
          Dtype* bottom0_diff = bottom[0]->mutable_cpu_diff() + i*len0;
          const Dtype* bottom1_data = bottom[1]->cpu_data() + i*len1;
          const Dtype* top0_diff = top[0]->cpu_diff() + i*len0;
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
      if (propagate_down[1]) {
      }
    }
};
  INSTANTIATE_CLASS(ClsFcLayer);
  REGISTER_LAYER_CLASS(ClsFc);

}