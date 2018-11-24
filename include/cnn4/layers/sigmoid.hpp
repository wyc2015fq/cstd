#ifndef CAFFE_SIGMOID_LAYER_HPP_
#define CAFFE_SIGMOID_LAYER_HPP_







#include "caffe/layers/neuron_layer.hpp"

namespace
{

  /**
   * @brief Sigmoid function non-linearity @f$
   *         y = (1 + \exp(-x))^{-1}
   *     @f$, a classic choice in neural networks.
   *
   * Note that the gradient vanishes as the values move away from 0.
   * The ReLULayer is often a better choice for this reason.
   */
  template <typename Dtype>
  class SigmoidLayer : public NeuronLayer
  {
  public:
    explicit SigmoidLayer()
      : NeuronLayer() {}

    virtual inline const char* type() const { return "Sigmoid"; }

  public:
    /**
     * @param bottom input Blob vector (length 1)
     *   -# @f$ (N \times C \times H \times W) @f$
     *      the inputs @f$ x @f$
     * @param top output Blob vector (length 1)
     *   -# @f$ (N \times C \times H \times W) @f$
     *      the computed outputs @f$
     *        y = (1 + \exp(-x))^{-1}
     *      @f$
     */
    virtual void Forward_(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Forward_(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);

    /**
     * @brief Computes the error gradient w.r.t. the sigmoid inputs.
     *
     * @param top output Blob vector (length 1), providing the error gradient with
     *      respect to the outputs
     *   -# @f$ (N \times C \times H \times W) @f$
     *      containing error gradients @f$ \frac{\partial E}{\partial y} @f$
     *      with respect to computed outputs @f$ y @f$
     * @param propagate_down see Layer::Backward_.
     * @param bottom input Blob vector (length 1)
     *   -# @f$ (N \times C \times H \times W) @f$
     *      the inputs @f$ x @f$; Backward_ fills their diff with
     *      gradients @f$
     *        \frac{\partial E}{\partial x}
     *            = \frac{\partial E}{\partial y} y (1 - y)
     *      @f$ if bottom[0]->propagate_down_
     */
    virtual void Backward_(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Backward_(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
  };

}  // namespace

#endif  // CAFFE_SIGMOID_LAYER_HPP_
