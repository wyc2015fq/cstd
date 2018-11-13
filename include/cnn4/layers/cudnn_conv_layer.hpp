#ifndef CAFFE_CUDNN_CONV_LAYER_HPP_
#define CAFFE_CUDNN_CONV_LAYER_HPP_


// Set to three for the benefit of the backward pass, which
// can use separate streams for calculating the gradient w.r.t.
// bias, filter weights, and bottom data for each group independently
#define CUDNN_STREAMS_PER_GROUP 3

class CuDNNConvolutionLayer : public ConvolutionLayer
{
public:

  bool handles_setup_;
  cudnnHandle_t* handle_;
  cudaStream_t*  stream_;

  // algorithms for forward and backwards convolutions
  cudnnConvolutionFwdAlgo_t* fwd_algo_;
  cudnnConvolutionBwdFilterAlgo_t* bwd_filter_algo_;
  cudnnConvolutionBwdDataAlgo_t* bwd_data_algo_;

  vector<cudnnTensorDescriptor_t> bottom_descs_, top_descs_;
  cudnnTensorDescriptor_t    bias_desc_;
  cudnnFilterDescriptor_t      filter_desc_;
  vector<cudnnConvolutionDescriptor_t> conv_descs_;
  int bottom_offset_, top_offset_, bias_offset_;

  size_t* workspace_fwd_sizes_;
  size_t* workspace_bwd_data_sizes_;
  size_t* workspace_bwd_filter_sizes_;
  size_t workspaceSizeInBytes;  // size of underlying storage
  void* workspaceData;  // underlying storage
  void** workspace;  // aliases into workspaceData
public:


  /**
  * TODO(dox) explain cuDNN interface
  */
  
  virtual void LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    ConvolutionLayer::LayerSetUp(bottom, top);
    // Initialize CUDA streams and cuDNN.
    stream_ = new cudaStream_t[this->group_ * CUDNN_STREAMS_PER_GROUP];
    handle_ = new cudnnHandle_t[this->group_ * CUDNN_STREAMS_PER_GROUP];
    // Initialize algorithm arrays
    fwd_algo_ = new cudnnConvolutionFwdAlgo_t[bottom.size()];
    bwd_filter_algo_ = new cudnnConvolutionBwdFilterAlgo_t[bottom.size()];
    bwd_data_algo_ = new cudnnConvolutionBwdDataAlgo_t[bottom.size()];
    // initialize size arrays
    workspace_fwd_sizes_ = new size_t[bottom.size()];
    workspace_bwd_filter_sizes_ = new size_t[bottom.size()];
    workspace_bwd_data_sizes_ = new size_t[bottom.size()];
    // workspace data
    workspaceSizeInBytes = 0;
    workspaceData = NULL;
    workspace = new void*[this->group_ * CUDNN_STREAMS_PER_GROUP];
    for (size_t i = 0; i < bottom.size(); ++i) {
      // initialize all to default algorithms
      fwd_algo_[i] = (cudnnConvolutionFwdAlgo_t)0;
      bwd_filter_algo_[i] = (cudnnConvolutionBwdFilterAlgo_t)0;
      bwd_data_algo_[i] = (cudnnConvolutionBwdDataAlgo_t)0;
      // default algorithms don't require workspace
      workspace_fwd_sizes_[i] = 0;
      workspace_bwd_data_sizes_[i] = 0;
      workspace_bwd_filter_sizes_[i] = 0;
    }
    for (int g = 0; g < this->group_ * CUDNN_STREAMS_PER_GROUP; g++) {
      CUDA_CHECK(cudaStreamCreate(&stream_[g]));
      CUDNN_CHECK(cudnnCreate(&handle_[g]));
      CUDNN_CHECK(cudnnSetStream(handle_[g], stream_[g]));
      workspace[g] = NULL;
    }
    cudnnDataType_t type = dataType<Dtype>::type;
    // Set the indexing parameters.
    bias_offset_ = (this->num_output_ / this->group_);
    // Create filter descriptor.
    const int* kernel_shape_data = this->kernel_shape_.dim;
    const int kernel_h = kernel_shape_data[0];
    const int kernel_w = kernel_shape_data[1];
    createFilterDesc(&filter_desc_, type,
      this->num_output_ / this->group_, this->channels_ / this->group_,
      kernel_h, kernel_w);
    // Create tensor descriptor(s) for data and corresponding convolution(s).
    for (int i = 0; i < bottom.size(); i++) {
      cudnnTensorDescriptor_t bottom_desc;
      createTensor4dDesc(&bottom_desc);
      bottom_descs_.push_back(bottom_desc);
      cudnnTensorDescriptor_t top_desc;
      createTensor4dDesc(&top_desc);
      top_descs_.push_back(top_desc);
      cudnnConvolutionDescriptor_t conv_desc;
      createConvolutionDesc(&conv_desc);
      conv_descs_.push_back(conv_desc);
    }
    // Tensor descriptor for bias.
    if (this->bias_term_) {
      createTensor4dDesc(&bias_desc_);
    }
    handles_setup_ = true;
  }

  virtual void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    cudnnDataType_t type = dataType<Dtype>::type;
    ConvolutionLayer::Reshape(bottom, top);
    CHECK_EQ(2, this->num_spatial_axes_)
      << "CuDNNConvolution input must have 2 spatial axes "
      << "(e.g., height and width). "
      << "Use 'engine: CAFFE' for general ND convolution.";
    bottom_offset_ = this->bottom_dim_ / this->group_;
    top_offset_ = this->top_dim_ / this->group_;
    const int height = bottom[0]->shape(this->channel_axis_ + 1);
    const int width = bottom[0]->shape(this->channel_axis_ + 2);
    const int height_out = top[0]->shape(this->channel_axis_ + 1);
    const int width_out = top[0]->shape(this->channel_axis_ + 2);
    const int* pad_data = this->pad_.dim;
    const int pad_h = pad_data[0];
    const int pad_w = pad_data[1];
    const int* stride_data = this->stride_.dim;
    const int stride_h = stride_data[0];
    const int stride_w = stride_data[1];
    // Specify workspace limit for kernels directly until we have a
    // planning strategy and a rewrite of Caffe's GPU memory mangagement
    size_t workspace_limit_bytes = 8 * 1024 * 1024;
    for (int i = 0; i < bottom.size(); i++) {
      setTensor4dDesc(&bottom_descs_[i], type,
        this->num_,
        this->channels_ / this->group_, height, width,
        this->channels_ * height * width,
        height * width, width, 1);
      setTensor4dDesc(&top_descs_[i], type,
        this->num_,
        this->num_output_ / this->group_, height_out, width_out,
        this->num_output_ * this->out_spatial_dim_,
        this->out_spatial_dim_, width_out, 1);
      setConvolutionDesc(&conv_descs_[i], type, bottom_descs_[i],
        filter_desc_, pad_h, pad_w,
        stride_h, stride_w);
      // choose forward and backward algorithms + workspace(s)
      CUDNN_CHECK(cudnnGetConvolutionForwardAlgorithm(handle_[0],
        bottom_descs_[i],
        filter_desc_,
        conv_descs_[i],
        top_descs_[i],
        CUDNN_CONVOLUTION_FWD_SPECIFY_WORKSPACE_LIMIT,
        workspace_limit_bytes,
        &fwd_algo_[i]));
      CUDNN_CHECK(cudnnGetConvolutionForwardWorkspaceSize(handle_[0],
        bottom_descs_[i],
        filter_desc_,
        conv_descs_[i],
        top_descs_[i],
        fwd_algo_[i],
        &(workspace_fwd_sizes_[i])));
      // choose backward algorithm for filter
      CUDNN_CHECK(cudnnGetConvolutionBackwardFilterAlgorithm(handle_[0],
        bottom_descs_[i], top_descs_[i], conv_descs_[i], filter_desc_,
        CUDNN_CONVOLUTION_BWD_FILTER_SPECIFY_WORKSPACE_LIMIT,
        workspace_limit_bytes, &bwd_filter_algo_[i]));
      // get workspace for backwards filter algorithm
      CUDNN_CHECK(cudnnGetConvolutionBackwardFilterWorkspaceSize(handle_[0],
        bottom_descs_[i], top_descs_[i], conv_descs_[i], filter_desc_,
        bwd_filter_algo_[i], &workspace_bwd_filter_sizes_[i]));
      // choose backward algo for data
      CUDNN_CHECK(cudnnGetConvolutionBackwardDataAlgorithm(handle_[0],
        filter_desc_, top_descs_[i], conv_descs_[i], bottom_descs_[i],
        CUDNN_CONVOLUTION_BWD_DATA_SPECIFY_WORKSPACE_LIMIT,
        workspace_limit_bytes, &bwd_data_algo_[i]));
      // get workspace size
      CUDNN_CHECK(cudnnGetConvolutionBackwardDataWorkspaceSize(handle_[0],
        filter_desc_, top_descs_[i], conv_descs_[i], bottom_descs_[i],
        bwd_data_algo_[i], &workspace_bwd_data_sizes_[i]));
    }
    // reduce over all workspace sizes to get a maximum to allocate / reallocate
    size_t total_workspace_fwd = 0;
    size_t total_workspace_bwd_data = 0;
    size_t total_workspace_bwd_filter = 0;
    for (size_t i = 0; i < bottom.size(); i++) {
      total_workspace_fwd = std::max(total_workspace_fwd,
        workspace_fwd_sizes_[i]);
      total_workspace_bwd_data = std::max(total_workspace_bwd_data,
        workspace_bwd_data_sizes_[i]);
      total_workspace_bwd_filter = std::max(total_workspace_bwd_filter,
        workspace_bwd_filter_sizes_[i]);
    }
    // get max over all operations
    size_t max_workspace = std::max(total_workspace_fwd,
      total_workspace_bwd_data);
    max_workspace = std::max(max_workspace, total_workspace_bwd_filter);
    // ensure all groups have enough workspace
    size_t total_max_workspace = max_workspace *
      (this->group_ * CUDNN_STREAMS_PER_GROUP);
    // this is the total amount of storage needed over all groups + streams
    if (total_max_workspace > workspaceSizeInBytes) {
      DLOG(INFO) << "Reallocating workspace storage: " << total_max_workspace;
      workspaceSizeInBytes = total_max_workspace;
      // free the existing workspace and allocate a new (larger) one
      cudaFree(this->workspaceData);
      cudaError_t err = cudaMalloc(&(this->workspaceData), workspaceSizeInBytes);
      if (err != cudaSuccess) {
        // force zero memory path
        for (int i = 0; i < bottom.size(); i++) {
          workspace_fwd_sizes_[i] = 0;
          workspace_bwd_filter_sizes_[i] = 0;
          workspace_bwd_data_sizes_[i] = 0;
          fwd_algo_[i] = CUDNN_CONVOLUTION_FWD_ALGO_IMPLICIT_GEMM;
          bwd_filter_algo_[i] = CUDNN_CONVOLUTION_BWD_FILTER_ALGO_0;
          bwd_data_algo_[i] = CUDNN_CONVOLUTION_BWD_DATA_ALGO_0;
        }
        // NULL out all workspace pointers
        for (int g = 0; g < (this->group_ * CUDNN_STREAMS_PER_GROUP); g++) {
          workspace[g] = NULL;
        }
        // NULL out underlying data
        workspaceData = NULL;
        workspaceSizeInBytes = 0;
      }
      // if we succeed in the allocation, set pointer aliases for workspaces
      for (int g = 0; g < (this->group_ * CUDNN_STREAMS_PER_GROUP); g++) {
        workspace[g] = reinterpret_cast<char*>(workspaceData) + g * max_workspace;
      }
    }
    // Tensor descriptor for bias.
    if (this->bias_term_) {
      setTensor4dDesc(&bias_desc_, type,
        1, this->num_output_ / this->group_, 1, 1);
    }
  }

  
  virtual ~CuDNNConvolutionLayer()
  {
    // Check that handles have been setup before destroying.
    if (!handles_setup_) { return; }
    for (int i = 0; i < bottom_descs_.size(); i++) {
      cudnnDestroyTensorDescriptor(bottom_descs_[i]);
      cudnnDestroyTensorDescriptor(top_descs_[i]);
      cudnnDestroyConvolutionDescriptor(conv_descs_[i]);
    }
    if (this->bias_term_) {
      cudnnDestroyTensorDescriptor(bias_desc_);
    }
    cudnnDestroyFilterDescriptor(filter_desc_);
    for (int g = 0; g < this->group_ * CUDNN_STREAMS_PER_GROUP; g++) {
      cudaStreamDestroy(stream_[g]);
      cudnnDestroy(handle_[g]);
    }
    cudaFree(workspaceData);
    delete[] stream_;
    delete[] handle_;
    delete[] fwd_algo_;
    delete[] bwd_filter_algo_;
    delete[] bwd_data_algo_;
    delete[] workspace_fwd_sizes_;
    delete[] workspace_bwd_data_sizes_;
    delete[] workspace_bwd_filter_sizes_;
  }

  virtual void Forward(const vector<Blob*>& bottom, const vector<Blob*>& top) {
    const Dtype* weight = this->blobs_[0]->data();
    for (int i = 0; i < bottom.size(); ++i) {
      const Dtype* bottom_data = bottom[i]->data();
      Dtype* top_data = top[i]->mdata();

      // Forward through cuDNN in parallel over groups.
      for (int g = 0; g < this->group_; g++) {
        // Filters.
        CUDNN_CHECK(cudnnConvolutionForward(handle_[g],
          dataType<Dtype>::get_one(),
          bottom_descs_[i], bottom_data + bottom_offset_ * g,
          filter_desc_, weight + this->weight_offset_ * g,
          conv_descs_[i],
          fwd_algo_[i], workspace[g], workspace_fwd_sizes_[i],
          dataType<Dtype>::get_zero(),
          top_descs_[i], top_data + top_offset_ * g));

        // Bias.
        if (this->bias_term_) {
          const Dtype* bias_data = this->blobs_[1]->data();
          CUDNN_CHECK(cudnnAddTensor(handle_[g],
            dataType<Dtype>::get_one(),
            bias_desc_, bias_data + bias_offset_ * g,
            dataType<Dtype>::get_one(),
            top_descs_[i], top_data + top_offset_ * g));
        }
      }

      // Synchronize the work across groups, each of which went into its own
      // stream, by launching an empty kernel into the default (null) stream.
      // NOLINT_NEXT_LINE(whitespace/operators)
      //sync_conv_groups << <1, 1 >> >();
    }
  }

  virtual void Backward(const vector<Blob*>& top,  const vector<Blob*>& bottom) {
    const Dtype* weight = NULL;
    Dtype* weight_diff = NULL;
    if (this->blobs_[0]->propagate_down_) {
      weight = this->blobs_[0]->data();
      weight_diff = this->blobs_[0]->gpu_mdiff();
    }
    Dtype* bias_diff = NULL;
    if (this->bias_term_ && this->blobs_[1]->propagate_down_) {
      bias_diff = this->blobs_[1]->gpu_mdiff();
    }
    for (int i = 0; i < top.size(); ++i) {
      const Dtype* top_diff = top[i]->gpu_diff();
      // Backward through cuDNN in parallel over groups and gradients.
      for (int g = 0; g < this->group_; g++) {
        // Gradient w.r.t. bias.
        if (this->bias_term_ && this->blobs_[1]->propagate_down_) {
          CUDNN_CHECK(cudnnConvolutionBackwardBias(handle_[0 * this->group_ + g],
            dataType<Dtype>::get_one(),
            top_descs_[i], top_diff + top_offset_ * g,
            dataType<Dtype>::get_one(),
            bias_desc_, bias_diff + bias_offset_ * g));
        }

        // Gradient w.r.t. weights.
        if (this->blobs_[0]->propagate_down_) {
          const Dtype* bottom_data = bottom[i]->data();
          CUDNN_CHECK(cudnnConvolutionBackwardFilter(
            handle_[1 * this->group_ + g],
            dataType<Dtype>::get_one(),
            bottom_descs_[i], bottom_data + bottom_offset_ * g,
            top_descs_[i], top_diff + top_offset_ * g,
            conv_descs_[i],
            bwd_filter_algo_[i], workspace[1 * this->group_ + g],
            workspace_bwd_filter_sizes_[i],
            dataType<Dtype>::get_one(),
            filter_desc_, weight_diff + this->weight_offset_ * g));
        }

        // Gradient w.r.t. bottom data.
        if (bottom[i]->propagate_down_) {
          if (weight == NULL) {
            weight = this->blobs_[0]->data();
          }
          Dtype* bottom_diff = bottom[i]->gpu_mdiff();
          CUDNN_CHECK(cudnnConvolutionBackwardData(
            handle_[2 * this->group_ + g],
            dataType<Dtype>::get_one(),
            filter_desc_, weight + this->weight_offset_ * g,
            top_descs_[i], top_diff + top_offset_ * g,
            conv_descs_[i],
            bwd_data_algo_[i], workspace[2 * this->group_ + g],
            workspace_bwd_data_sizes_[i],
            dataType<Dtype>::get_zero(),
            bottom_descs_[i], bottom_diff + bottom_offset_ * g));
        }
      }

      // Synchronize the work across groups, each of which went into its own
      // stream, by launching an empty kernel into the default (null) stream.
      // NOLINT_NEXT_LINE(whitespace/operators)
      //sync_conv_groups << <1, 1 >> >();
    }
  }
};



INSTANTIATE_CLASS2(Convolution, CuDNNConvolution);


#endif  // CAFFE_CUDNN_CONV_LAYER_HPP_
