#ifndef CAFFE_CUDNN_CONV_LAYER_HPP_
#define CAFFE_CUDNN_CONV_LAYER_HPP_


// Set to three for the benefit of the backward pass, which
// can use separate streams for calculating the gradient w.r.t.
// bias, filter weights, and bottom data for each group independently
#define CUDNN_STREAMS_PER_GROUP 3

class CuDNNConvolutionLayer : public ConvolutionLayer {
public:

struct CuDNNConvolutionAlgo {
  cudnnConvolutionFwdAlgo_t fwd_algo_;
  cudnnConvolutionBwdFilterAlgo_t bwd_filter_algo_;
  cudnnConvolutionBwdDataAlgo_t bwd_data_algo_;
  size_t workspace_fwd_sizes_;
  size_t workspace_bwd_data_sizes_;
  size_t workspace_bwd_filter_sizes_;
  cudnnConvolutionDescriptor_t conv_descs_;
  cudnnTensorDescriptor_t bottom_descs_, top_descs_;
  void init() {
    createTensor4dDesc(&bottom_descs_);
    createTensor4dDesc(&top_descs_);
    createConvolutionDesc(&conv_descs_);
  }

  void  Forward(cudnnHandle_t handle,
    const void                         *x,
    const cudnnFilterDescriptor_t       wDesc,
    const void                         *w,
    void                               *workSpace,
    void                               *y) {
    CUDNN_CHECK(cudnnConvolutionForward(handle, gpu_get_one(), bottom_descs_, x, wDesc, w,
      conv_descs_, fwd_algo_, workSpace, workspace_fwd_sizes_, gpu_get_zero(), top_descs_, y));
  }
};

  bool handles_setup_;
  cudnnHandle_t* handle_;
  cudaStream_t*  stream_;

  // algorithms for forward and backwards convolutions
  CuDNNConvolutionAlgo* algo;
  int algo_size_;

  cudnnTensorDescriptor_t    bias_desc_;
  cudnnFilterDescriptor_t    filter_desc_;
  int bottom_offset_, top_offset_, bias_offset_;

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
    algo_size_ = bottom.size();
    algo = new CuDNNConvolutionAlgo[algo_size_];
    memset(algo, 0, sizeof(CuDNNConvolutionAlgo)*bottom.size());
    // Initialize CUDA streams and cuDNN.
    stream_ = new cudaStream_t[this->group_ * CUDNN_STREAMS_PER_GROUP];
    handle_ = new cudnnHandle_t[this->group_ * CUDNN_STREAMS_PER_GROUP];
    // workspace data
    workspaceSizeInBytes = 0;
    workspaceData = NULL;
    workspace = new void*[this->group_ * CUDNN_STREAMS_PER_GROUP];

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
    const int* kernel_data = this->kernel_.dim;
    const int kernel_h = kernel_data[0];
    const int kernel_w = kernel_data[1];
    createFilterDesc(&filter_desc_, type,
      this->num_output_ / this->group_, this->channels_ / this->group_,
      kernel_h, kernel_w);
    // Create tensor descriptor(s) for data and corresponding convolution(s).
    for (int i = 0; i < bottom.size(); i++) {
      algo[i].init();
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
      setTensor4dDesc(algo[i].bottom_descs_, type,
        this->num_,
        this->channels_ / this->group_, height, width,
        this->channels_ * height * width,
        height * width, width, 1);
      setTensor4dDesc(algo[i].top_descs_, type,
        this->num_,
        this->num_output_ / this->group_, height_out, width_out,
        this->num_output_ * this->out_spatial_dim_,
        this->out_spatial_dim_, width_out, 1);
      setConvolutionDesc(algo[i].conv_descs_, type, algo[i].bottom_descs_,
        filter_desc_, pad_h, pad_w,
        stride_h, stride_w);
      // choose forward and backward algorithms + workspace(s)
      CUDNN_CHECK(cudnnGetConvolutionForwardAlgorithm(handle_[0],
        algo[i].bottom_descs_,
        filter_desc_,
        algo[i].conv_descs_,
        algo[i].top_descs_,
        CUDNN_CONVOLUTION_FWD_SPECIFY_WORKSPACE_LIMIT,
        workspace_limit_bytes,
        &algo[i].fwd_algo_));
      CUDNN_CHECK(cudnnGetConvolutionForwardWorkspaceSize(handle_[0],
        algo[i].bottom_descs_,
        filter_desc_,
        algo[i].conv_descs_,
        algo[i].top_descs_,
        algo[i].fwd_algo_,
        &(algo[i].workspace_fwd_sizes_)));
      // choose backward algorithm for filter
      CUDNN_CHECK(cudnnGetConvolutionBackwardFilterAlgorithm(handle_[0],
        algo[i].bottom_descs_, algo[i].top_descs_, algo[i].conv_descs_, filter_desc_,
        CUDNN_CONVOLUTION_BWD_FILTER_SPECIFY_WORKSPACE_LIMIT,
        workspace_limit_bytes, &algo[i].bwd_filter_algo_));
      // get workspace for backwards filter algorithm
      CUDNN_CHECK(cudnnGetConvolutionBackwardFilterWorkspaceSize(handle_[0],
        algo[i].bottom_descs_, algo[i].top_descs_, algo[i].conv_descs_, filter_desc_,
        algo[i].bwd_filter_algo_, &algo[i].workspace_bwd_filter_sizes_));
      // choose backward algo for data
      CUDNN_CHECK(cudnnGetConvolutionBackwardDataAlgorithm(handle_[0],
        filter_desc_, algo[i].top_descs_, algo[i].conv_descs_, algo[i].bottom_descs_,
        CUDNN_CONVOLUTION_BWD_DATA_SPECIFY_WORKSPACE_LIMIT,
        workspace_limit_bytes, &algo[i].bwd_data_algo_));
      // get workspace size
      CUDNN_CHECK(cudnnGetConvolutionBackwardDataWorkspaceSize(handle_[0],
        filter_desc_, algo[i].top_descs_, algo[i].conv_descs_, algo[i].bottom_descs_,
        algo[i].bwd_data_algo_, &algo[i].workspace_bwd_data_sizes_));
    }
    // reduce over all workspace sizes to get a maximum to allocate / reallocate
    size_t total_workspace_fwd = 0;
    size_t total_workspace_bwd_data = 0;
    size_t total_workspace_bwd_filter = 0;
    for (size_t i = 0; i < bottom.size(); i++) {
      total_workspace_fwd = std::max(total_workspace_fwd, algo[i].workspace_fwd_sizes_);
      total_workspace_bwd_data = std::max(total_workspace_bwd_data, algo[i].workspace_bwd_data_sizes_);
      total_workspace_bwd_filter = std::max(total_workspace_bwd_filter, algo[i].workspace_bwd_filter_sizes_);
    }
    // get max over all operations
    size_t max_workspace = std::max(total_workspace_fwd, total_workspace_bwd_data);
    max_workspace = std::max(max_workspace, total_workspace_bwd_filter);
    // ensure all groups have enough workspace
    size_t total_max_workspace = max_workspace * (this->group_ * CUDNN_STREAMS_PER_GROUP);
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
          algo[i].workspace_fwd_sizes_ = 0;
          algo[i].workspace_bwd_filter_sizes_ = 0;
          algo[i].workspace_bwd_data_sizes_ = 0;
          algo[i].fwd_algo_ = CUDNN_CONVOLUTION_FWD_ALGO_IMPLICIT_GEMM;
          algo[i].bwd_filter_algo_ = CUDNN_CONVOLUTION_BWD_FILTER_ALGO_0;
          algo[i].bwd_data_algo_ = CUDNN_CONVOLUTION_BWD_DATA_ALGO_0;
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
      setTensor4dDesc(bias_desc_, type, 1, this->num_output_ / this->group_, 1, 1);
    }
  }

  virtual ~CuDNNConvolutionLayer()
  {
    // Check that handles have been setup before destroying.
    if (!handles_setup_) { return; }
    for (int i = 0; i < algo_size_; i++) {
      cudnnDestroyTensorDescriptor(algo[i].bottom_descs_);
      cudnnDestroyTensorDescriptor(algo[i].top_descs_);
      cudnnDestroyConvolutionDescriptor(algo[i].conv_descs_);
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
    delete[] algo;
  }
  virtual void Forward_(int i, const Dtype* in, Dtype* out, const Dtype* w, const Dtype* b) {
    for (int g = 0; g < this->group_; g++) {
      // Filters.
      //algo[i].Forward(handle_[g], bottom_data + bottom_offset_ * g, filter_desc_, weight + this->weight_offset_ * g, workspace[g], top_data + top_offset_ * g);
      CUDNN_CHECK(cudnnConvolutionForward(handle_[g],
        gpu_get_one(),
        algo[i].bottom_descs_, in + bottom_offset_ * g,
        filter_desc_, w + this->weight_offset_ * g,
        algo[i].conv_descs_,
        algo[i].fwd_algo_, workspace[g], algo[i].workspace_fwd_sizes_,
        gpu_get_zero(),
        algo[i].top_descs_, out + top_offset_ * g));

      // Bias.
      if (b) {
        CUDNN_CHECK(cudnnAddTensor(handle_[g],
          gpu_get_one(),
          bias_desc_, b + bias_offset_ * g,
          gpu_get_one(),
          algo[i].top_descs_, out + top_offset_ * g));
      }
    }
  }

  virtual void Forward_1(const vector<Blob*>& bottom, const vector<Blob*>& top) {
    const Dtype* weight = this->blobs_[0]->data();
    for (int i = 0; i < bottom.size(); ++i) {
      const Dtype* bottom_data = bottom[i]->data();
      Dtype* top_data = top[i]->mdata();

      // Forward_ through cuDNN in parallel over groups.
      for (int g = 0; g < this->group_; g++) {
        // Filters.
        //algo[i].Forward(handle_[g], bottom_data + bottom_offset_ * g, filter_desc_, weight + this->weight_offset_ * g, workspace[g], top_data + top_offset_ * g);
        CUDNN_CHECK(cudnnConvolutionForward(handle_[g],
          gpu_get_one(),
          algo[i].bottom_descs_, bottom_data + bottom_offset_ * g,
          filter_desc_, weight + this->weight_offset_ * g,
          algo[i].conv_descs_,
          algo[i].fwd_algo_, workspace[g], algo[i].workspace_fwd_sizes_,
          gpu_get_zero(),
          algo[i].top_descs_, top_data + top_offset_ * g));

        // Bias.
        if (this->bias_term_) {
          const Dtype* bias_data = this->blobs_[1]->data();
          CUDNN_CHECK(cudnnAddTensor(handle_[g],
            gpu_get_one(),
            bias_desc_, bias_data + bias_offset_ * g,
            gpu_get_one(),
            algo[i].top_descs_, top_data + top_offset_ * g));
        }
      }

      // Synchronize the work across groups, each of which went into its own
      // stream, by launching an empty kernel into the default (null) stream.
      // NOLINT_NEXT_LINE(whitespace/operators)
      //sync_conv_groups << <1, 1 >> >();
    }
  }

  virtual void Backward_(const vector<Blob*>& top,  const vector<Blob*>& bottom) {
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
      // Backward_ through cuDNN in parallel over groups and gradients.
      for (int g = 0; g < this->group_; g++) {
        // Gradient w.r.t. bias.
        if (this->bias_term_ && this->blobs_[1]->propagate_down_) {
          CUDNN_CHECK(cudnnConvolutionBackwardBias(handle_[0 * this->group_ + g],
            gpu_get_one(),
            algo[i].top_descs_, top_diff + top_offset_ * g,
            gpu_get_one(),
            bias_desc_, bias_diff + bias_offset_ * g));
        }

        // Gradient w.r.t. weights.
        if (this->blobs_[0]->propagate_down_) {
          const Dtype* bottom_data = bottom[i]->data();
          CUDNN_CHECK(cudnnConvolutionBackwardFilter(
            handle_[1 * this->group_ + g],
            gpu_get_one(),
            algo[i].bottom_descs_, bottom_data + bottom_offset_ * g,
            algo[i].top_descs_, top_diff + top_offset_ * g,
            algo[i].conv_descs_,
            algo[i].bwd_filter_algo_, workspace[1 * this->group_ + g],
            algo[i].workspace_bwd_filter_sizes_,
            gpu_get_one(),
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
            gpu_get_one(),
            filter_desc_, weight + this->weight_offset_ * g,
            algo[i].top_descs_, top_diff + top_offset_ * g,
            algo[i].conv_descs_,
            algo[i].bwd_data_algo_, workspace[2 * this->group_ + g],
            algo[i].workspace_bwd_data_sizes_,
            gpu_get_zero(),
            algo[i].bottom_descs_, bottom_diff + bottom_offset_ * g));
        }
      }

      // Synchronize the work across groups, each of which went into its own
      // stream, by launching an empty kernel into the default (null) stream.
      // NOLINT_NEXT_LINE(whitespace/operators)
      //sync_conv_groups << <1, 1 >> >();
    }
  }
};

INSTANTIATE_CLASS(Convolution, CuDNNConvolutionLayer);

#endif  // CAFFE_CUDNN_CONV_LAYER_HPP_
