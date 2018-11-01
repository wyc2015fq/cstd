#ifndef CAFFE_RELU_LAYER_HPP_
#define CAFFE_RELU_LAYER_HPP_

/**
 * @brief Rectified Linear Unit non-linearity @f$ y = \max(0, x) @f$.
 *        The simple max is fast to compute, and the function does not saturate.
 */
template <typename Dtype>
class ReLULayer : public NeuronLayer<Dtype>
{
public:
  virtual inline const char* type() const { return "ReLU"; }

public:
  /**
   * @param bottom input Blob vector (length 1)
   *   -# @f$ (N \times C \times H \times W) @f$
   *      the inputs @f$ x @f$
   * @param top output Blob vector (length 1)
   *   -# @f$ (N \times C \times H \times W) @f$
   *      the computed outputs @f$
   *        y = \max(0, x)
   *      @f$ by default.  If a non-zero negative_slope @f$ \nu @f$ is provided,
   *      the computed outputs are @f$ y = \max(0, x) + \nu \min(0, x) @f$.
   */
  virtual void Forward(Context* context, const vector<Blob<Dtype>*> & bottom,
    const vector<Blob<Dtype>*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data<Context>();
    Dtype* top_data = top[0]->mutable_data<Context>();
    const int count = bottom[0]->count();
    Dtype negative_slope = this->param_->GetObjectNumber("negative_slope", 0);
    relu_forward(context, count, bottom_data, top_data, negative_slope);
  }

  /**
   * @brief Computes the error gradient w.r.t. the ReLU inputs.
   *
   * @param top output Blob vector (length 1), providing the error gradient with
   *      respect to the outputs
   *   -# @f$ (N \times C \times H \times W) @f$
   *      containing error gradients @f$ \frac{\partial E}{\partial y} @f$
   *      with respect to computed outputs @f$ y @f$
   * @param propagate_down see Layer::Backward.
   * @param bottom input Blob vector (length 1)
   *   -# @f$ (N \times C \times H \times W) @f$
   *      the inputs @f$ x @f$; Backward fills their diff with
   *      gradients @f$
   *        \frac{\partial E}{\partial x} = \left\{
   *        \begin{array}{lr}
   *            0 & \mathrm{if} \; x \le 0 \\
   *            \frac{\partial E}{\partial y} & \mathrm{if} \; x > 0
   *        \end{array} \right.
   *      @f$ if top[0]->propagate_down_, by default.
   *      If a non-zero negative_slope @f$ \nu @f$ is provided,
   *      the computed gradients are @f$
   *        \frac{\partial E}{\partial x} = \left\{
   *        \begin{array}{lr}
   *            \nu \frac{\partial E}{\partial y} & \mathrm{if} \; x \le 0 \\
   *            \frac{\partial E}{\partial y} & \mathrm{if} \; x > 0
   *        \end{array} \right.
   *      @f$.
   */
  virtual void Backward(Context* context, const vector<Blob<Dtype>*> & top,
    const vector<Blob<Dtype>*> & bottom)
  {
    if (top[0]->propagate_down_) {
      const Dtype* bottom_data = bottom[0]->data<Context>();
      const Dtype* top_diff = top[0]->diff<Context>();
      Dtype* bottom_diff = bottom[0]->mutable_diff<Context>();
      const int count = bottom[0]->count();
      Dtype negative_slope = this->param_->GetObjectNumber("negative_slope", 0);
      relu_backward<Dtype>(context, count, top_diff, bottom_data, bottom_diff, negative_slope);
    }
  }
};

INSTANTIATE_CLASS(ReLU);

#endif  // CAFFE_RELU_LAYER_HPP_
