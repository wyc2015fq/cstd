

#include "caffe/layers/sigmoid_cross_entropy_loss_layer.hpp"


namespace
{

  template <typename Dtype>
  void SigmoidCrossEntropyLossLayer<Dtype>::LayerSetUp(
    const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top)
  {
    LossLayer<Dtype>::LayerSetUp(bottom, top);
    sigmoid_bottom_vec_.clear();
    sigmoid_bottom_vec_.push_back(bottom[0]);
    sigmoid_top_vec_.clear();
    sigmoid_top_vec_.push_back(sigmoid_output_.get());
    sigmoid_layer_->SetUp(sigmoid_bottom_vec_, sigmoid_top_vec_);
  }

  template <typename Dtype>
  void SigmoidCrossEntropyLossLayer<Dtype>::Reshape(
    const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top)
  {
    LossLayer<Dtype>::Reshape(bottom, top);
    CHECK_EQ(bottom[0]->count(), bottom[1]->count()) <<
        "SIGMOID_CROSS_ENTROPY_LOSS layer inputs must have the same count.";
    sigmoid_layer_->Reshape(sigmoid_bottom_vec_, sigmoid_top_vec_);
  }

  template <typename Dtype>
  void SigmoidCrossEntropyLossLayer<Dtype>::Forward(_CONTEXT,
    const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top)
  {
    // The forward pass computes the sigmoid outputs.
    sigmoid_bottom_vec_[0] = bottom[0];
    sigmoid_layer_->Forward(sigmoid_bottom_vec_, sigmoid_top_vec_);
    // Compute the loss (negative log likelihood)
    const int count = bottom[0]->count();
    const int num = bottom[0]->num();
    // Stable version of loss computation from input data
    const Dtype* input_data = bottom[0]->data<Context>();
    const Dtype* target = bottom[1]->data<Context>();
    Dtype loss = 0;
    for (int i = 0; i < count; ++i) {
      loss -= input_data[i] * (target[i] - (input_data[i] >= 0)) -
              log(1 + exp(input_data[i] - 2 * input_data[i] * (input_data[i] >= 0)));
    }
    top[0]->mutable_data<Context>()[0] = loss / num;
  }

  template <typename Dtype>
  void SigmoidCrossEntropyLossLayer<Dtype>::Backward_cpu(
    const vector<Blob<Dtype>*> & top, int*
    const vector<Blob<Dtype>*> & bottom)
  {
    if (bottom[1]->propagate_down_) {
      LOG(FATAL) << this->type()
                 << " Layer cannot backpropagate to label inputs.";
    }
    if (bottom[0]->propagate_down_) {
      // First, compute the diff
      const int count = bottom[0]->count();
      const int num = bottom[0]->num();
      const Dtype* sigmoid_output_data = sigmoid_output_->data<Context>();
      const Dtype* target = bottom[1]->data<Context>();
      Dtype* bottom_diff = bottom[0]->mutable_diff<Context>();
      caffe_sub(count, sigmoid_output_data, target, bottom_diff);
      // Scale down gradient
      const Dtype loss_weight = top[0]->diff<Context>()[0];
      caffe_scal(count, loss_weight / num, bottom_diff);
    }
  }

#ifdef CPU_ONLY
  STUB_GPU_BACKWARD(SigmoidCrossEntropyLossLayer, Backward);
#endif

  INSTANTIATE_CLASS(SigmoidCrossEntropyLossLayer);
  REGISTER_LAYER_CLASS(SigmoidCrossEntropyLoss);

}  // namespace
