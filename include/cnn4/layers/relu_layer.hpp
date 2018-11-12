#ifndef CAFFE_RELU_LAYER_HPP_
#define CAFFE_RELU_LAYER_HPP_

/**
 * @brief Rectified Linear Unit non-linearity @f$ y = \max(0, x) @f$.
 *        The simple max is fast to compute, and the function does not saturate.
 */
class ReLULayer : public NeuronLayer
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
  virtual void Forward(const vector<Blob*> & bottom,
    const vector<Blob*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* top_data = top[0]->mutable_data();
    const int count = bottom[0]->count();
    Dtype negative_slope = this->param_->GetObjectNumber("negative_slope", 0);
    relu_forward(count, bottom_data, top_data, negative_slope);
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
   *      @f$ if bottom[0]->propagate_down_, by default.
   *      If a non-zero negative_slope @f$ \nu @f$ is provided,
   *      the computed gradients are @f$
   *        \frac{\partial E}{\partial x} = \left\{
   *        \begin{array}{lr}
   *            \nu \frac{\partial E}{\partial y} & \mathrm{if} \; x \le 0 \\
   *            \frac{\partial E}{\partial y} & \mathrm{if} \; x > 0
   *        \end{array} \right.
   *      @f$.
   */
  virtual void Backward(const vector<Blob*> & top,
    const vector<Blob*> & bottom)
  {
    if (bottom[0]->propagate_down_) {
      const Dtype* bottom_data = bottom[0]->data();
      const Dtype* top_diff = top[0]->diff();
      Dtype* bottom_diff = bottom[0]->mutable_diff();
      const int count = bottom[0]->count();
      Dtype negative_slope = this->param_->GetObjectNumber("negative_slope", 0);
      relu_backward(count, top_diff, bottom_data, bottom_diff, negative_slope);
    }
  }
};

INSTANTIATE_CLASS(ReLU);

#endif  // CAFFE_RELU_LAYER_HPP_
