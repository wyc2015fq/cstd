#ifndef CAFFE_THRESHOLD_LAYER_HPP_
#define CAFFE_THRESHOLD_LAYER_HPP_







#include "caffe/layers/neuron_layer.hpp"

namespace
{

  /**
   * @brief Tests whether the input exceeds a threshold: outputs 1 for inputs
   *        above threshold; 0 otherwise.
   */
  template <typename Dtype>
  class ThresholdLayer : public NeuronLayer
  {
  public:
    /**
     * @param param provides ThresholdParameter threshold_param,
     *     with ThresholdLayer options:
     *   - threshold (\b optional, default 0).
     *     the threshold value @f$ t @f$ to which the input values are compared.
     */
    explicit ThresholdLayer()
      : NeuronLayer() {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);

    virtual inline const char* type() const { return "Threshold"; }

  public:
    /**
     * @param bottom input Blob vector (length 1)
     *   -# @f$ (N \times C \times H \times W) @f$
     *      the inputs @f$ x @f$
     * @param top output Blob vector (length 1)
     *   -# @f$ (N \times C \times H \times W) @f$
     *      the computed outputs @f$
     *       y = \left\{
     *       \begin{array}{lr}
     *         0 & \mathrm{if} \; x \le t \\
     *         1 & \mathrm{if} \; x > t
     *       \end{array} \right.
     *      @f$
     */
    virtual void Forward_(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Forward_(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    /// @brief Not implemented (non-differentiable function)
    virtual void Backward_(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom) {
      NOT_IMPLEMENTED;
    }

    Dtype threshold_;
  };

}  // namespace

#endif  // CAFFE_THRESHOLD_LAYER_HPP_
