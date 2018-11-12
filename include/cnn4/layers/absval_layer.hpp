#ifndef CAFFE_ABSVAL_LAYER_HPP_
#define CAFFE_ABSVAL_LAYER_HPP_







#include "caffe/layers/neuron_layer.hpp"

namespace
{

  /**
   * @brief Computes @f$ y = |x| @f$
   *
   * @param bottom input Blob vector (length 1)
   *   -# @f$ (N \times C \times H \times W) @f$
   *      the inputs @f$ x @f$
   * @param top output Blob vector (length 1)
   *   -# @f$ (N \times C \times H \times W) @f$
   *      the computed outputs @f$ y = |x| @f$
   */
  template <typename Dtype>
  class AbsValLayer : public NeuronLayer
  {
  public:
    explicit AbsValLayer()
      : NeuronLayer() {}
    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top);

    virtual inline const char* type() const { return "AbsVal"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
    /// @copydoc AbsValLayer
    virtual void Forward(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Forward(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);

    /**
     * @brief Computes the error gradient w.r.t. the absolute value inputs.
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
     *        \frac{\partial E}{\partial x} =
     *            \mathrm{sign}(x) \frac{\partial E}{\partial y}
     *      @f$ if bottom[0]->propagate_down_
     */
    virtual void Backward(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Backward(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
  };

}  // namespace

#endif  // CAFFE_ABSVAL_LAYER_HPP_
