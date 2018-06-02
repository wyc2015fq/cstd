#ifndef CAFFE_ELU_LAYER_HPP_
#define CAFFE_ELU_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"






/**
 * @brief Exponential Linear Unit non-linearity @f$
 *        y = \left\{
 *        \begin{array}{lr}
 *            x                  & \mathrm{if} \; x > 0 \\
 *            \alpha (\exp(x)-1) & \mathrm{if} \; x \le 0
 *        \end{array} \right.
 *      @f$.  
 */

class ELULayer {

};





void ELULayer::Forward(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  const Dtype* prevData = prevData;
  Dtype* nextData = nextData;
  const int count = Data_count(prev->size);
  Dtype alpha = this->layer_param_.elu_param().alpha();
  for (i = 0; i < count; ++i) {
    nextData[i] = MAX(prevData[i], (Dtype)(0))
        + alpha * (exp(std::min(prevData[i], (Dtype)(0))) - (Dtype)(1));
  }
}


void ELULayer::Backward(const vector<Blob*>& top,
    const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  if (propagate_down[0]) {
    const Dtype* prevData = prevData;
    const Dtype* nextData = top[0]->cpu_data();
    const Dtype* nextDiff = nextDiff;
    Dtype* prevDiff = bottom[0]->mutable_diff();
    const int count = Data_count(prev->size);
    Dtype alpha = this->layer_param_.elu_param().alpha();
    for (i = 0; i < count; ++i) {
      prevDiff[i] = nextDiff[i] * ((prevData[i] > 0)
          + (alpha + nextData[i]) * (prevData[i] <= 0));
    }
  }
}


#ifdef CPU_ONLY
STUB_GPU(ELULayer);
#endif

INSTANTIATE_CLASS(ELULayer);
REGISTER_LAYER_CLASS(ELU);


