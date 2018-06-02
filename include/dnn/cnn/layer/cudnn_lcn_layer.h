#ifdef USE_CUDNN


#include "caffe/blob.hpp"
#include "caffe/layer.hpp"


#include "lrn_layer.hpp"
#include "power_layer.hpp"



class CuDNNLCNLayer : public LRNLayer {
 public:
  explicit CuDNNLCNLayer(const LayerParameter& param)
      : LRNLayer(param), handles_setup_(false), tempDataSize(0),
        tempData1(NULL), tempData2(NULL) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual ~CuDNNLCNLayer();

 protected:
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  bool handles_setup_;
  cudnnHandle_t             handle_;
  cudnnLRNDescriptor_t norm_desc_;
  cudnnTensorDescriptor_t bottom_desc_, top_desc_;

  int size_, pre_pad_;
  Dtype alpha_, beta_, k_;

  size_t tempDataSize;
  void *tempData1, *tempData2;
};


void CuDNNLCNLayer::LayerSetUp(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  LRNLayer::LayerSetUp(bottom, top);

  CUDNN_CHECK(cudnnCreate(&handle_));
  CUDNN_CHECK(cudnnCreateLRNDescriptor(&norm_desc_));
  cudnn::createTensor4dDesc(&bottom_desc_);
  cudnn::createTensor4dDesc(&top_desc_);

  // create a LRN handle
  handles_setup_ = true;

  size_ = this->layer_param().lrn_param().local_size();
  pre_pad_ = (size_ - 1) / 2;
  alpha_ = this->layer_param().lrn_param().alpha();
  beta_ = this->layer_param().lrn_param().beta();
  k_ = this->layer_param().lrn_param().k();
}


void CuDNNLCNLayer::Reshape(const vector<Blob*>& bottom,
    const vector<Blob*>& top) {
  LRNLayer::Reshape(bottom, top);
  cudnn::setTensor4dDesc(&bottom_desc_, prev->size.n,
      this->channels_, this->height_, this->width_);
  cudnn::setTensor4dDesc(&top_desc_, prev->size.n,
      this->channels_, this->height_, this->width_);
  CUDNN_CHECK(cudnnSetLRNDescriptor(norm_desc_, size_, alpha_, beta_, k_));

  // allocate / reallocate tempData buffers
  size_t totalSizeInBytes = sizeof(Dtype)*prev->size.n* \
                            this->channels_*this->height_*this->width_;

  if (totalSizeInBytes > tempDataSize) {
    tempDataSize = totalSizeInBytes;

    cudaFree(tempData1);
    cudaFree(tempData2);

    // allocate new buffers
    CUDA_CHECK(cudaMalloc(&tempData1, totalSizeInBytes));
    CUDA_CHECK(cudaMalloc(&tempData2, totalSizeInBytes));
  }
}


CuDNNLCNLayer::~CuDNNLCNLayer() {
  // Check that handles have been setup before destroying.
  if (!handles_setup_) { return; }

  cudnnDestroyTensorDescriptor(bottom_desc_);
  cudnnDestroyTensorDescriptor(top_desc_);

  // destroy LRN handle
  cudnnDestroy(handle_);

  // free temp buffers
  cudaFree(tempData1);
  cudaFree(tempData2);
}

INSTANTIATE_CLASS(CuDNNLCNLayer);

}   // namespace caffe
#endif
