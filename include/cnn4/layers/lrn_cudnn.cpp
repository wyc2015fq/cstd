#ifdef USE_CUDNN


#include "caffe/layers/cudnn_lrn_layer.hpp"

namespace
{

  template <typename Dtype>
  void CuDNNLRNLayer::LayerSetUp(const vector<Blob*> & bottom,
                                        const vector<Blob*> & top)
  {
    LRNLayer::LayerSetUp(bottom, top);
    CUDNN_CHECK(cudnnCreate(&handle_));
    CUDNN_CHECK(cudnnCreateLRNDescriptor(&norm_desc_));
    cudnn::createTensor4dDesc<Dtype>(&bottom_desc_);
    cudnn::createTensor4dDesc<Dtype>(&top_desc_);
    // create a LRN handle
    handles_setup_ = true;
    size_ = this->layer_param().lrn_param().local_size();
    alpha_ = this->layer_param().lrn_param().alpha();
    beta_ = this->layer_param().lrn_param().beta();
    k_ = this->layer_param().lrn_param().k();
  }

  template <typename Dtype>
  void CuDNNLRNLayer::Reshape(const vector<Blob*> & bottom,
                                     const vector<Blob*> & top)
  {
    LRNLayer::Reshape(bottom, top);
    cudnn::setTensor4dDesc<Dtype>(&bottom_desc_, bottom[0]->num(),
                                  this->channels_, this->height_, this->width_);
    cudnn::setTensor4dDesc<Dtype>(&top_desc_, bottom[0]->num(),
                                  this->channels_, this->height_, this->width_);
    CUDNN_CHECK(cudnnSetLRNDescriptor(norm_desc_, size_, alpha_, beta_, k_));
  }

  template <typename Dtype>
  CuDNNLRNLayer::~CuDNNLRNLayer()
  {
    // Check that handles have been setup before destroying.
    if (!handles_setup_) { return; }
    cudnnDestroyTensorDescriptor(bottom_desc_);
    cudnnDestroyTensorDescriptor(top_desc_);
    // destroy LRN handle
    cudnnDestroy(handle_);
  }

  INSTANTIATE_CLASS(CuDNNLRNLayer);

}   // namespace
#endif
