

#include "neuron_layer.hpp"



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

class AbsValLayer : public NeuronLayer {
 public:
  explicit AbsValLayer(const LayerParameter& param)
      : NeuronLayer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "AbsVal"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

 protected:
  /// @copydoc AbsValLayer
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

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
   *      @f$ if propagate_down[0]
   */
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
};







void AbsValLayer::LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  NeuronLayer::LayerSetUp(bottom, top);
  CHECK_NE(top[0], bottom[0]) << this->type() << " Layer does not "
    "allow in-place computation.";
}


void AbsValLayer::Forward_cpu(
    const vector<Blob*>& bottom, const vector<Blob*>& top) {
  const int count = top[0]->count();
  Dtype* top_data = top[0]->mutable_cpu_data();
  caffe_abs(count, pBox_datap(bottom_blob->data), top_data);
}


void AbsValLayer::Backward_cpu(const vector<Blob*>& top,
    const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  const int count = top[0]->count();
  const Dtype* top_diff = top[0]->cpu_diff();
  if (propagate_down[0]) {
    const Dtype* bottom_data = pBox_datap(bottom_blob->data);
    Dtype* bottom_diff = bottom[0]->mutable_cpu_diff();
    caffe_cpu_sign(count, bottom_data, bottom_diff);
    caffe_mul(count, bottom_diff, top_diff, bottom_diff);
  }
}

#ifdef CPU_ONLY
STUB_GPU(AbsValLayer);
#endif

INSTANTIATE_CLASS(AbsValLayer);
REGISTER_LAYER_CLASS(AbsVal);


