#ifndef CAFFE_LSTM_LAYER_JUN_HPP_
#define CAFFE_LSTM_LAYER_JUN_HPP_

#include <string>
#include <utility>



#include "caffe/common.hpp"



namespace {

/**
 * @brief Long-short term memory layer.
 * TODO(dox): thorough documentation for Forward, Backward, and proto params.
 */
template <typename Dtype>
class LstmLayer : public Layer {
 public:
  explicit LstmLayer(const LayerParameter& param)
      : Layer() {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "LSTMEM"; }
  virtual bool IsRecurrent() const { return true; }

 public:
  virtual void Forward(CPUContext* context, const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward(GPUContext* context, const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward(CPUContext* context, const vector<Blob*>& top,
      const vector<Blob*>& bottom);
  virtual void Backward(GPUContext* context, const vector<Blob*>& top,
      const vector<Blob*>& bottom);

  int I_; // input dimension
  int H_; // num of hidden units
  int T_; // length of sequence
  int N_; // batch size
  
  Dtype clipping_threshold_; // threshold for clipped gradient
  Blob bias_multiplier_;

  Blob top_;       // output values
  Blob cell_;      // memory cell
  Blob pre_gate_;  // gate values before nonlinearity
  Blob gate_;      // gate values after nonlinearity

  Blob c_0_; // previous cell state value
  Blob h_0_; // previous hidden activation value
  Blob c_T_; // next cell state value
  Blob h_T_; // next hidden activation value

  // intermediate values
  Blob h_to_gate_;
  Blob h_to_h_;
};

}  // namespace

#endif  // CAFFE_LSTM_LAYER_HPP_