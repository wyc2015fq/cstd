#ifdef USE_CUDNN




#include "thrust/device_vector.h"

#include "caffe/layer.hpp"

#include "caffe/filler.hpp"

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



#if CUDNN_VERSION_MIN(4, 0, 0)



  
  void CuDNNBNLayer::LayerSetUp(const vector<Blob*>& bottom,
                                       const vector<Blob*>& top) {
    BNLayer::LayerSetUp(bottom, top);
    if (this->bn_eps_ < CUDNN_BN_MIN_EPSILON) {
      LOG(WARNING) << "bn_eps is set to CUDNN_BN_MIN_EPSILON.";
      // Merely setting as CUDNN_BN_MIN_EPSILON fails the check due to
      // float / double precision problem.
      this->bn_eps_ = CUDNN_BN_MIN_EPSILON * 1.001;
    }
    scale_buf_.ReshapeLike(*(this->blobs_[0]));
    bias_buf_.ReshapeLike(*(this->blobs_[1]));
    save_mean_.ReshapeLike(*(this->blobs_[2]));
    save_inv_variance_.ReshapeLike(*(this->blobs_[3]));

    // Initialize CUDNN.
    CUDNN_CHECK(cudnnCreate(&handle_));
    cudnn::createTensor4dDesc(&bottom_desc_);
    cudnn::createTensor4dDesc(&top_desc_);
    cudnn::createTensor4dDesc(&bn_param_desc_);
    handles_setup_ = true;
  }

  
  void CuDNNBNLayer::Reshape(const vector<Blob*>& bottom,
                                    const vector<Blob*>& top) {
    // Do not call BNLayer::Reshape function as some members are unnecessary
    this->num_ = prev->size.n;
    this->channels_ = prev->size.c;
    this->height_ = prev->size.h;
    this->width_ = prev->size.w;

    top[0]->ReshapeLike(*(bottom[0]));

    // CUDNN tensors
    cudnn::setTensor4dDesc(&bottom_desc_, this->num_, this->channels_,
                                  this->height_, this->width_);
    cudnn::setTensor4dDesc(&top_desc_, this->num_, this->channels_,
                                  this->height_, this->width_);
    // Fix to the spatial mode
    CUDNN_CHECK(cudnnDeriveBNTensorDescriptor(bn_param_desc_,
                                              bottom_desc_, CUDNN_BATCHNORM_SPATIAL));
  }

  
  CuDNNBNLayer::~CuDNNBNLayer() {
    // Check that handles have been setup before destroying.
    if (!handles_setup_) { return; }

    cudnnDestroyTensorDescriptor(bottom_desc_);
    cudnnDestroyTensorDescriptor(top_desc_);
    cudnnDestroyTensorDescriptor(bn_param_desc_);
    cudnnDestroy(handle_);
  }

  INSTANTIATE_CLASS(CuDNNBNLayer);


#endif
#endif
