#ifndef CAFFE_CUDNN_BN_LAYER_HPP_
#define CAFFE_CUDNN_BN_LAYER_HPP_



#include "caffe/blob.hpp"
#include "caffe/layer.hpp"


#include "bn_layer.hpp"



#ifdef USE_CUDNN
#if CUDNN_VERSION_MIN(4, 0, 0)
  /*
  * @brief cuDNN implementation of ConvolutionLayer.
  *        Fallback to ConvolutionLayer for CPU mode.
  *
  * cuDNN accelerates convolution through forward kernels for filtering and bias
  * plus backward kernels for the gradient w.r.t. the filters, biases, and
  * inputs. Caffe + cuDNN further speeds up the computation through forward
  * parallelism across groups and backward parallelism across gradients.
  *
  * The CUDNN engine does not have memory overhead for matrix buffers. For many
  * input and filter regimes the CUDNN engine is faster than the CAFFE engine,
  * but for fully-convolutional models and large inputs the CAFFE engine can be
  * faster as long as it fits in memory.
  */
  
  class CuDNNBNLayer : public BNLayer {
  public:
    explicit CuDNNBNLayer(const LayerParameter& param)
      : BNLayer(param), handles_setup_(false) {}
    virtual void LayerSetUp(const vector<Blob*>& bottom,
                            const vector<Blob*>& top);
    virtual void Reshape(const vector<Blob*>& bottom,
                         const vector<Blob*>& top);
    virtual ~CuDNNBNLayer();

    virtual inline const char* type() const { return "BN"; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  protected:
    virtual void Forward_gpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Backward_gpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);

    bool handles_setup_;
    cudnnHandle_t handle_;
    cudnnTensorDescriptor_t bottom_desc_;
    cudnnTensorDescriptor_t top_desc_;
    cudnnTensorDescriptor_t bn_param_desc_;

    Blob scale_buf_;
    Blob bias_buf_;
    Blob save_mean_;
    Blob save_inv_variance_;
  };

#endif
#endif


#endif  // CAFFE_CUDNN_BN_LAYER_HPP_
