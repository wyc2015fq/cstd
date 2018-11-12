#ifndef CAFFE_BNLL_LAYER_HPP_
#define CAFFE_BNLL_LAYER_HPP_







#include "caffe/layers/neuron_layer.hpp"

namespace
{

  /**
   * @brief Computes @f$ y = x + \log(1 + \exp(-x)) @f$ if @f$ x > 0 @f$;
   *        @f$ y = \log(1 + \exp(x)) @f$ otherwise.
   *
   * @param bottom input Blob vector (length 1)
   *   -# @f$ (N \times C \times H \times W) @f$
   *      the inputs @f$ x @f$
   * @param top output Blob vector (length 1)
   *   -# @f$ (N \times C \times H \times W) @f$
   *      the computed outputs @f$
   *      y = \left\{
   *         \begin{array}{ll}
   *            x + \log(1 + \exp(-x)) & \mbox{if } x > 0 \\
   *            \log(1 + \exp(x)) & \mbox{otherwise}
   *         \end{array} \right.
   *      @f$
   */
  template <typename Dtype>
  class BNLLLayer : public NeuronLayer
  {
  public:
    explicit BNLLLayer()
      : NeuronLayer() {}

    virtual inline const char* type() const { return "BNLL"; }

  public:
    /// @copydoc BNLLLayer
    virtual void Forward(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Forward(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);

    /**
     * @brief Computes the error gradient w.r.t. the BNLL inputs.
     *
     * @param top output Blob vector (length 1), providing the error gradient with
     *      respect to the outputs
     *   -# @f$ (N \times C \times H \times W) @f$
     *      containing error gradients @f$ \frac{\partial E}{\partial y} @f$
     *      with respect to computed outputs @f$ y @f$
     * @param propagate_down see Layer::Backward.
     * @param bottom input Blob vector (length 2)
     *   -# @f$ (N \times C \times H \times W) @f$
     *      the inputs @f$ x @f$; Backward fills their diff with
     *      gradients @f$
     *        \frac{\partial E}{\partial x}
     *      @f$ if bottom[0]->propagate_down_
     */
    virtual void Backward(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Backward(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
  };

}  // namespace

#endif  // CAFFE_BNLL_LAYER_HPP_
