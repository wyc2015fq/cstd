#include <thread>
#include <vector>

#include "blob.hpp"
#include "data_transformer.hpp"
#include "internal_thread.hpp"
#include "layer.hpp"
#include "layers/base_data_layer.hpp"
#include "proto.h"
#include "blocking_queue.hpp"

namespace caffe
{

  template <typename Dtype>
  BaseDataLayer<Dtype>::BaseDataLayer(CJSON* param)
    : Layer<Dtype>(param),
      transform_param_(param.transform_param())
  {
  }

  template <typename Dtype>
  void BaseDataLayer<Dtype>::LayerSetUp(const vector<Blob<Dtype>*> & bottom,
                                        const vector<Blob<Dtype>*> & top)
  {
    data_transformer_.reset(
      new DataTransformer<Dtype>(transform_param_, this->phase_));
    data_transformer_->InitRand();
    // The subclasses should setup the size of bottom and top
    DataLayerSetUp(bottom, top);
  }

  template <typename Dtype>
  BasePrefetchingDataLayer<Dtype>::BasePrefetchingDataLayer(
    CJSON* param)
    : BaseDataLayer<Dtype>(param),
      prefetch_(param.data_param().prefetch()),
      prefetch_free_(), prefetch_full_(), prefetch_current_()
  {
    for (int i = 0; i < prefetch_.size(); ++i) {
      prefetch_[i].reset(new Batch<Dtype>());
      prefetch_free_.push(prefetch_[i].get());
    }
  }

  template <typename Dtype>
  void BasePrefetchingDataLayer<Dtype>::LayerSetUp(
    const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top)
  {
    BaseDataLayer<Dtype>::LayerSetUp(bottom, top);
    // Before starting the prefetch thread, we make cpu_data and gpu_data
    // calls so that the prefetch thread does not accidentally make simultaneous
    // cudaMalloc calls when the main thread is running. In some GPUs this
    // seems to cause failures if we do not so.
    for (int i = 0; i < prefetch_.size(); ++i) {
      for (int j = 0; j < top.size(); ++j) {
        prefetch_[i]->data_[j].mutable_cpu_data();
      }
    }
#ifndef CPU_ONLY
    if (mode() == GPU) {
      for (int i = 0; i < prefetch_.size(); ++i) {
      	for (int j=0; j< top.size(); ++j) {
          prefetch_[i]->data_[j].mutable_gpu_data();
        }
      }
    }
#endif
    DLOG(INFO) << "Initializing prefetch";
    this->data_transformer_->InitRand();
    StartInternalThread();
    DLOG(INFO) << "Prefetch initialized.";
  }

  template <typename Dtype>
  void BasePrefetchingDataLayer<Dtype>::InternalThreadEntry()
  {
#ifndef CPU_ONLY
    cudaStream_t stream = 0;
    if (mode() == GPU) {
      CUDA_CHECK(cudaStreamCreateWithFlags(&stream, cudaStreamNonBlocking));
    }
#endif
    try {
      while (!must_stop()) {
        Batch<Dtype>* batch = prefetch_free_.pop();
        load_batch(batch);
#ifndef CPU_ONLY
        if (mode() == GPU) {
        	for (int j=0; j<batch->data_size; ++j) {
            SyncedMemory* ptr = batch->data_[j].data().get();
            if (ptr) ptr->async_gpu_push(stream);
          }
          CUDA_CHECK(cudaStreamSynchronize(stream));
        }
#endif
        prefetch_full_.push(batch);
      }
    } catch (std::exception & e) {
      LOG(ERROR) << "exception: " << e.what();
      // Interrupted exception is expected on shutdown
    }
#ifndef CPU_ONLY
    if (mode() == GPU) {
      CUDA_CHECK(cudaStreamDestroy(stream));
    }
#endif
  }

  template <typename Dtype>
  void BasePrefetchingDataLayer<Dtype>::Forward_cpu(
    const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top)
  {
    if (prefetch_current_) {
      prefetch_free_.push(prefetch_current_);
    }
    prefetch_current_ = prefetch_full_.pop("Waiting for data");
    // Reshape to loaded data.
    LOG_IF(INFO, top.size() > top.size()) << " top.size()> datum.blob_size()";
    for (int j = 0; j<top.size(); ++j) {
	    top[j]->ReshapeLike(prefetch_current_->data_[j]);
	    top[j]->set_cpu_data(prefetch_current_->data_[j].mutable_cpu_data());
      //Dtype* ptr = (Dtype*)prefetch_current_->data_[j].mutable_cpu_data();
      //int count = top[j]->count();
      //printf("%f ", ptr[count-1]);
    }
    //printf("\n");
  }

#ifdef CPU_ONLY
  STUB_GPU_FORWARD(BasePrefetchingDataLayer, Forward);
#endif

  INSTANTIATE_CLASS(BaseDataLayer);
  INSTANTIATE_CLASS(BasePrefetchingDataLayer);

}  // namespace caffe
