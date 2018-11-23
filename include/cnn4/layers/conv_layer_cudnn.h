
typedef Blob::Dtype Dtype;

void cudnn_conv2d(const Dtype* inData, float* outData, const Dtype* weights, const Dtype* biasData,
  DataShape inSize, DataShape outSize, int kernel_h, int kernel_w, int stride_h, int stride_w,
  int dilation_h, int dilation_w, int pad_h, int pad_w, int group_, bool cross_correlation) {
  typedef Blob::Dtype Dtype;
  cudnnConvolutionFwdAlgo_t fwd_algo_;
  cudnnConvolutionBwdFilterAlgo_t bwd_filter_algo_;
  cudnnConvolutionBwdDataAlgo_t bwd_data_algo_;

  cudnnTensorDescriptor_t bottom_descs_ = NULL, top_descs_ = NULL;
  cudnnTensorDescriptor_t    bias_desc_ = NULL;
  cudnnFilterDescriptor_t      filter_desc_ = NULL;
  cudnnConvolutionDescriptor_t conv_descs_ = NULL;

  size_t workspace_fwd_sizes_ = 0;
  size_t workspace_bwd_data_sizes_ = 0;
  size_t workspace_bwd_filter_sizes_ = 0;
  size_t workspaceSizeInBytes = 0;  // size of underlying storage
  void* workspaceData = NULL;  // underlying storage
  void** workspace = NULL;  // aliases into workspaceData

  cudnnHandle_t handle_;
  cudaStream_t stream_;
  CUDA_CHECK(cudaStreamCreate(&stream_));
  CUDNN_CHECK(cudnnCreate(&handle_));
  CUDNN_CHECK(cudnnSetStream(handle_, stream_));

  int channel_axis_ = 1;
  int channels_ = inSize.shape(channel_axis_);
  int num_output_ = outSize.c;
  int num_ = inSize.n;

  cudnnDataType_t type = dataType<Dtype>::type;
  createFilterDesc(&filter_desc_, type, num_output_ / group_, channels_ / group_, kernel_h, kernel_w);
  // Create tensor descriptor(s) for data and corresponding convolution(s).

  createTensor4dDesc(&bottom_descs_);
  createTensor4dDesc(&top_descs_);
  createConvolutionDesc(&conv_descs_);
  bool bias_term_ = biasData != NULL;
  // Tensor descriptor for bias.
  if (bias_term_) {
    createTensor4dDesc(&bias_desc_);
  }
  size_t workspace_limit_bytes = 8 * 1024 * 1024;
  setTensor4dDesc(bottom_descs_, type, inSize);
  setTensor4dDesc(top_descs_, type, outSize);
  setConvolutionDesc(conv_descs_, type, bottom_descs_, filter_desc_, pad_h, pad_w, stride_h, stride_w);
  // choose forward and backward algorithms + workspace(s)
  CUDNN_CHECK(cudnnGetConvolutionForwardAlgorithm(handle_, bottom_descs_, filter_desc_, conv_descs_, top_descs_, CUDNN_CONVOLUTION_FWD_SPECIFY_WORKSPACE_LIMIT, workspace_limit_bytes, &fwd_algo_));
  CUDNN_CHECK(cudnnGetConvolutionForwardWorkspaceSize(handle_, bottom_descs_, filter_desc_, conv_descs_, top_descs_, fwd_algo_, &(workspace_fwd_sizes_)));
  // choose backward algorithm for filter
  CUDNN_CHECK(cudnnGetConvolutionBackwardFilterAlgorithm(handle_, bottom_descs_, top_descs_, conv_descs_, filter_desc_, CUDNN_CONVOLUTION_BWD_FILTER_SPECIFY_WORKSPACE_LIMIT, workspace_limit_bytes, &bwd_filter_algo_));
  // get workspace for backwards filter algorithm
  CUDNN_CHECK(cudnnGetConvolutionBackwardFilterWorkspaceSize(handle_, bottom_descs_, top_descs_, conv_descs_, filter_desc_, bwd_filter_algo_, &workspace_bwd_filter_sizes_));
  // choose backward algo for data
  CUDNN_CHECK(cudnnGetConvolutionBackwardDataAlgorithm(handle_, filter_desc_, top_descs_, conv_descs_, bottom_descs_, CUDNN_CONVOLUTION_BWD_DATA_SPECIFY_WORKSPACE_LIMIT, workspace_limit_bytes, &bwd_data_algo_));
  // get workspace size
  CUDNN_CHECK(cudnnGetConvolutionBackwardDataWorkspaceSize(handle_, filter_desc_, top_descs_, conv_descs_, bottom_descs_, bwd_data_algo_, &workspace_bwd_data_sizes_));

  size_t total_workspace_fwd = 0;
  size_t total_workspace_bwd_data = 0;
  size_t total_workspace_bwd_filter = 0;
  total_workspace_fwd = MAX(total_workspace_fwd, workspace_fwd_sizes_);
  total_workspace_bwd_data = MAX(total_workspace_bwd_data, workspace_bwd_data_sizes_);
  total_workspace_bwd_filter = MAX(total_workspace_bwd_filter, workspace_bwd_filter_sizes_);

  cudaError_t err = cudaMalloc(&workspaceData, workspaceSizeInBytes);
  if (err != cudaSuccess) {
    // force zero memory path
    fwd_algo_ = CUDNN_CONVOLUTION_FWD_ALGO_IMPLICIT_GEMM;
    bwd_filter_algo_ = CUDNN_CONVOLUTION_BWD_FILTER_ALGO_0;
    bwd_data_algo_ = CUDNN_CONVOLUTION_BWD_DATA_ALGO_0;
  }

  size_t max_workspace = MAX(total_workspace_fwd, total_workspace_bwd_data);
  max_workspace = MAX(max_workspace, total_workspace_bwd_filter);
  // ensure all groups have enough workspace
  size_t total_max_workspace = max_workspace * (group_ * CUDNN_STREAMS_PER_GROUP);
  int bottom_offset_ = inSize.count(1) / group_;
  int top_offset_ = outSize.count(1) / group_;
  int bias_offset_ = (num_output_ / group_);

  /////

  //void cudnn_conv2d(const Dtype* inData, float* outData, const Dtype* weights, const Dtype* biasData,
//    DataShape inSize, DataShape outSize, int kernel_h, int kernel_w, int stride_h, int stride_w,
    //int dilation_h, int dilation_w, int pad_h, int pad_w, int group_, bool cross_correlation);
  //virtual void Forward_(const vector<Blob*>& bottom, const vector<Blob*>& top)
  {
    int kernel_dim_ = kernel_h * kernel_w;
    int weight_offset_ = outSize.c * kernel_dim_ / group_;
    const Dtype* weight = weights;
    
    {
      const Dtype* bottom_data = inData;
      Dtype* top_data = outData;

      // Forward_ through cuDNN in parallel over groups.
      for (int g = 0; g < group_; g++) {
        // Filters.
        CUDNN_CHECK(cudnnConvolutionForward(handle_,
          gpu_get_one(),
          bottom_descs_, bottom_data + bottom_offset_ * g,
          filter_desc_, weight + weight_offset_ * g,
          conv_descs_,
          fwd_algo_, workspace, workspace_fwd_sizes_,
          gpu_get_zero(),
          top_descs_, top_data + top_offset_ * g));

        // Bias.
        if (biasData) {
          const Dtype* bias_data = biasData;
          CUDNN_CHECK(cudnnAddTensor(handle_,
            gpu_get_one(),
            bias_desc_, bias_data + bias_offset_ * g,
            gpu_get_one(),
            top_descs_, top_data + top_offset_ * g));
        }
      }

      // Synchronize the work across groups, each of which went into its own
      // stream, by launching an empty kernel into the default (null) stream.
      // NOLINT_NEXT_LINE(whitespace/operators)
      //sync_conv_groups << <1, 1 >> >();
    }
  }

  /////
  cudnnDestroyTensorDescriptor(bottom_descs_);
  cudnnDestroyTensorDescriptor(top_descs_);
  cudnnDestroyConvolutionDescriptor(conv_descs_);

  if (bias_term_) {
    cudnnDestroyTensorDescriptor(bias_desc_);
  }
  cudnnDestroyFilterDescriptor(filter_desc_);
  //for (int g = 0; g < group_ * CUDNN_STREAMS_PER_GROUP; g++)
  {
    cudaStreamDestroy(stream_);
    cudnnDestroy(handle_);
  }
  cudaFree(workspaceData);
  return;
}
