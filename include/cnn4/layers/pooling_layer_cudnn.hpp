#ifndef CAFFE_CUDNN_POOLING_LAYER_HPP_
#define CAFFE_CUDNN_POOLING_LAYER_HPP_

class CuDNNPoolingLayer : public PoolingLayer
{
public:
  bool handles_setup_;
  cudnnHandle_t           handle_;
  cudnnTensorDescriptor_t bottom_desc_, top_desc_;
  cudnnPoolingDescriptor_t  pooling_desc_;
  // Currently, cuDNN does not support the extra top blob.
  virtual inline int MinTopBlobs() const { return -1; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

public:

  virtual ~CuDNNPoolingLayer()
  {
    // Check that handles have been setup before destroying.
    if (!handles_setup_) { return; }
    cudnnDestroyTensorDescriptor(bottom_desc_);
    cudnnDestroyTensorDescriptor(top_desc_);
    cudnnDestroyPoolingDescriptor(pooling_desc_);
    cudnnDestroy(handle_);
  }

  virtual void LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    PoolingLayer::LayerSetUp(bottom, top);
    CUDNN_CHECK(cudnnCreate(&handle_));
    createTensor4dDesc(&bottom_desc_);
    createTensor4dDesc(&top_desc_);
    createPoolingDesc(&pooling_desc_, this->pool_, 
      this->kernel_h_, this->kernel_w_, this->pad_h_, this->pad_w_,
      this->stride_h_, this->stride_w_);
    handles_setup_ = true;
  }

  virtual void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    cudnnDataType_t type = dataType<Dtype>::type;
    PoolingLayer::Reshape(bottom, top);
    setTensor4dDesc(bottom_desc_, type, bottom[0]->num(), this->channels_, this->height_, this->width_);
    setTensor4dDesc(top_desc_, type, bottom[0]->num(), this->channels_, this->pooled_height_, this->pooled_width_);
  }

  virtual void Forward_(const vector<Blob*>& bottom, const vector<Blob*>& top) {
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* top_data = top[0]->mdata();
    CUDNN_CHECK(cudnnPoolingForward(handle_, pooling_desc_,
      gpu_get_one(),
      bottom_desc_, bottom_data,
      gpu_get_zero(),
      top_desc_, top_data));
  }

  virtual void Backward_(const vector<Blob*>& top, const vector<Blob*>& bottom) {
    if (!bottom[0]->propagate_down_) {
      return;
    }
    const Dtype* top_diff = top[0]->gpu_diff();
    const Dtype* top_data = top[0]->data();
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* bottom_diff = bottom[0]->gpu_mdiff();
    CUDNN_CHECK(cudnnPoolingBackward(handle_, pooling_desc_,
      gpu_get_one(),
      top_desc_, top_data, top_desc_, top_diff,
      bottom_desc_, bottom_data,
      gpu_get_zero(),
      bottom_desc_, bottom_diff));
  }

};

INSTANTIATE_CLASS2(Pooling, CuDNNPooling);


#endif  // CAFFE_CUDNN_POOLING_LAYER_HPP_
