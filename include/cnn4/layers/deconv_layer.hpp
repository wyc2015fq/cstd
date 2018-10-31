#ifndef CAFFE_DECONV_LAYER_HPP_
#define CAFFE_DECONV_LAYER_HPP_







#include "caffe/layers/base_conv_layer.hpp"

namespace
{

  /**
   * @brief Convolve the input with a bank of learned filters, and (optionally)
   *        add biases, treating filters and convolution parameters in the
   *        opposite sense as ConvolutionLayer.
   *
   *   ConvolutionLayer computes each output value by dotting an input window with
   *   a filter; DeconvolutionLayer multiplies each input value by a filter
   *   elementwise, and sums over the resulting output windows. In other words,
   *   DeconvolutionLayer is ConvolutionLayer with the forward and backward passes
   *   reversed. DeconvolutionLayer reuses ConvolutionParameter for its
   *   parameters, but they take the opposite sense as in ConvolutionLayer (so
   *   padding is removed from the output rather than added to the input, and
   *   stride results in upsampling rather than downsampling).
   */
  template <typename Dtype>
  class DeconvolutionLayer : public BaseConvolutionLayer<Dtype>
  {
  public:
    explicit DeconvolutionLayer()
      : BaseConvolutionLayer<Dtype>() {}

    virtual inline const char* type() const { return "Deconvolution"; }

  public:
    virtual void Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);
    virtual void Forward(GPUContext* context, const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top);
    virtual void Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom);
    virtual void Backward(GPUContext* context, const vector<Blob<Dtype>*> & top,
                              const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom);
    virtual inline bool reverse_dimensions() { return true; }
    virtual void compute_output_shape();
  };

}  // namespace

#endif  // CAFFE_DECONV_LAYER_HPP_
