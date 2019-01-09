

struct DenseBlock_cudnn : public DenseBlockLayer {

  //start GPU specific data section
  //GPU ptr for efficient space usage only, these pointers not allocated when CPU_ONLY, these are not Blobs because Descriptor is not traditional
  //bool gpuInited;


  vector<void*> dropout_state_gpu;
  vector<size_t> dropout_stateSize;
  vector<void*> dropout_reserve_gpu;
  vector<size_t> dropout_reserveSize;

  //BC related parameters
  vector<Dtype*> postConv_4GVec; //used if not ultra space efficient mode
  cudnnTensorDescriptor_t quadG_tensorDesc;
  cudnnTensorDescriptor_t quadG_paramDesc;
  cudnnConvolutionDescriptor_t convBC_Descriptor;
  vector<Dtype*> BC_MeanInfVec;
  vector<Dtype*> BC_VarInfVec;
  vector<Dtype*> ResultSaveMean_BC;
  vector<Dtype*> ResultSaveInvVariance_BC;
  vector<cudnnFilterDescriptor_t> BC_filterDescriptorVec;
  //chosen Fwd,BwdFilter,BwdData algos for BC-Conv/Normal-Conv
  vector<cudnnConvolutionFwdAlgo_t> conv_FwdAlgoVec;
  vector<cudnnConvolutionFwdAlgo_t> BC_FwdAlgoVec;
  vector<cudnnConvolutionBwdFilterAlgo_t> conv_BwdFilterAlgoVec;
  vector<cudnnConvolutionBwdFilterAlgo_t> BC_BwdFilterAlgoVec;
  vector<cudnnConvolutionBwdDataAlgo_t> conv_BwdDataAlgoVec;
  vector<cudnnConvolutionBwdDataAlgo_t> BC_BwdDataAlgoVec;
  //BC_dropout

  struct Bottleneck {

  };


  //gpu handles and descriptors
  cudnnHandle_t cudnnHandlePtr;
  //cudaStream_t cudaPrimalStream;
  cudnnHandle_t extraHandles;
  cudaStream_t extraStreams;

  vector<cudnnTensorDescriptor_t> tensorDescriptorVec_conv_x; //local Conv X
  cudnnTensorDescriptor_t tensorDescriptor_conv_y; //local Conv Y
  vector<cudnnTensorDescriptor_t> tensorDescriptor_BN; //<channelwise>
                                                        //Dropout descriptor
  vector<cudnnDropoutDescriptor_t> dropoutDescriptorVec;
  //filter descriptor for conv
  vector<cudnnFilterDescriptor_t> filterDescriptorVec;
  //ReLU Activation Descriptor
  cudnnActivationDescriptor_t ReLUDesc;
  //conv descriptor for conv
  cudnnConvolutionDescriptor_t conv_Descriptor;


  virtual void GPU_Initialization() {
    cudnnDataType_t type = dataType<Dtype>::type;
    //std::cout<<"Pre DeviceSet"<<std::endl;
    //CUDA_CHECK(cudaSetDevice(1));
    //std::cout<<"Post DeviceSet"<<std::endl;
    //GPU intermediate ptrs
#if 1

#endif 

  //workspace
    CUDA_CHECK(cudaMalloc(&this->workspace, this->workspace_size_bytes));
    cudaMemset(this->workspace, 0, this->workspace_size_bytes);
    CUDA_CHECK(cudaMalloc(&this->workspace2, this->workspace_size_bytes));
    cudaMemset(this->workspace2, 0, this->workspace_size_bytes);
    //handles and descriptors
    //cudnn handle
    cudnnHandlePtr = 0;
    //cudaPrimalStream = NULL;
    CUDNN_CHECK(cudnnCreate(&cudnnHandlePtr));
    //CUDA_CHECK(cudaStreamCreate(&cudaPrimalStream));
    //CUDNN_CHECK(cudnnSetStream(cudnnHandlePtr,*cudaPrimalStream));
    //int extraHandle_num = 3;
    //for (int i = 0; i < extraHandle_num; ++i)
    {
      cudnnHandle_t localHandle = 0;
      cudaStream_t localStream = 0;
      CUDNN_CHECK(cudnnCreate(&localHandle));
      CUDA_CHECK(cudaStreamCreate(&localStream));
      CUDNN_CHECK(cudnnSetStream(localHandle, localStream));
      extraHandles = (localHandle);
      extraStreams = (localStream);
    }
    //ReLU Activation Descriptor
    this->ReLUDesc = NULL;
    createActivationDescriptor(&ReLUDesc, CUDNN_ACTIVATION_RELU);
    //conv_y global tensor descriptor
    this->tensorDescriptor_conv_y = NULL;
    createTensor4dDesc(&this->tensorDescriptor_conv_y);
#if 1
    setTensor4dDesc(this->tensorDescriptor_conv_y, type, this->N, this->growthRate, this->H, this->W, (this->numtransition_*this->growthRate + this->initChannel)*this->H*this->W, this->H*this->W, this->W, 1);
#endif
    //BC
    int quadG_numValues = 4 * N*growthRate*H*W;
    int quadG_numBytes = quadG_numValues * sizeof(Dtype);
    DataShape quadG_shape = dataShape(N, 4 * growthRate, H, W);
    if (useBC) {
      createTensor4dDesc(&quadG_tensorDesc);
#if 1
      setTensor4dDesc(quadG_tensorDesc, type, N, 4 * growthRate, H, W, 4 * growthRate*H*W, H*W, W, 1);
#endif
      quadG_paramDesc = NULL;
      createTensor4dDesc(&quadG_paramDesc);
      setTensor4dDesc(quadG_paramDesc, type, 1, 4 * growthRate, 1, 1, 4 * growthRate, 1, 1, 1);
      convBC_Descriptor = NULL;
      CUDNN_CHECK(cudnnCreateConvolutionDescriptor(&convBC_Descriptor));
      CUDNN_CHECK(cudnnSetConvolution2dDescriptor(convBC_Descriptor, 0, 0, 1, 1, 1, 1, CUDNN_CONVOLUTION, dataType<Dtype>::type));
    }
    //per transition variables
    for (int i = 0; i < this->numtransition_; ++i) {
      //conv_x descriptor
      int conv_x_channels = this->initChannel + this->growthRate * i;
      cudnnTensorDescriptor_t  wide_Desc_local_x = NULL;
      createTensor4dDesc(&wide_Desc_local_x);
#if 1
      setTensor4dDesc(wide_Desc_local_x, type, this->N, conv_x_channels, this->H, this->W,
        (this->numtransition_*this->growthRate + this->initChannel)*this->H*this->W, this->H*this->W, this->W, 1);
      this->tensorDescriptorVec_conv_x.push_back(wide_Desc_local_x);
#endif
      //filter Descriptor for Convolution
      if (!useBC) {
        cudnnFilterDescriptor_t  localFilterDesc = NULL;
        createFilterDesc(&localFilterDesc, type, growthRate, conv_x_channels, 3, 3);
        this->filterDescriptorVec.push_back(localFilterDesc);
      }
      else {
        //3*3 convolution filter desc
        cudnnFilterDescriptor_t  localFilterDesc = NULL;
        createFilterDesc(&localFilterDesc, type, growthRate, 4 * growthRate, 3, 3);
        this->filterDescriptorVec.push_back(localFilterDesc);
        //1*1 convolution filter desc
        cudnnFilterDescriptor_t  localBottleneckFilterDesc = NULL;
        createFilterDesc(&localBottleneckFilterDesc, type, 4 * growthRate, conv_x_channels, 1, 1);
        this->BC_filterDescriptorVec.push_back(localBottleneckFilterDesc);
      }
      //BN channel-wise Descriptor
      int channelsBefore_self = initChannel + growthRate*i;
      cudnnTensorDescriptor_t  BNparam = NULL;
      createTensor4dDesc(&BNparam);
      setTensor4dDesc(BNparam, type, 1, channelsBefore_self, 1, 1);
      this->tensorDescriptor_BN.push_back(BNparam);
      //Dropout Ptr and Descriptor
      if (useDropout) {
        size_t sizeState = 0;
        size_t sizeReserve = 0;
        CUDNN_CHECK(cudnnDropoutGetStatesSize((cudnnHandlePtr), &sizeState));
        CUDNN_CHECK(cudnnDropoutGetReserveSpaceSize(tensorDescriptor_conv_y, &sizeReserve));
        dropout_reserveSize.push_back(sizeReserve);
        dropout_stateSize.push_back(sizeState);
        void* localStatePtr;
        void* localReservePtr;
        CUDA_CHECK(cudaMalloc(&localStatePtr, sizeState));
        CUDA_CHECK(cudaMalloc(&localReservePtr, sizeReserve));
        dropout_state_gpu.push_back(localStatePtr);
        dropout_reserve_gpu.push_back(localReservePtr);
        cudnnDropoutDescriptor_t localDropoutDesc = NULL;
        cudnnCreateDropoutDescriptor(&localDropoutDesc);
        cudnnSetDropoutDescriptor(localDropoutDesc, cudnnHandlePtr, dropoutAmount, localStatePtr, sizeState, DB_randomSeed);
        dropoutDescriptorVec.push_back(localDropoutDesc);
        DB_randomSeed += 1;
      }
      //BC
      if (useBC && (!BC_ultra_spaceEfficient)) {
        Dtype* local_BC4G;
        CUDA_CHECK(cudaMalloc(&local_BC4G, quadG_numValues * sizeof(Dtype)));
        cudaMemset(local_BC4G, 0, quadG_numBytes);
        postConv_4GVec.push_back(local_BC4G);
      }
      if (useBC) {
        Dtype* BC_tmpMeanLocal;
        Dtype* BC_tmpVarLocal;
        int numChannel_BC = 4 * growthRate;
        int byteChannel_BC = numChannel_BC * sizeof(Dtype);
        CUDA_CHECK(cudaMalloc(&BC_tmpMeanLocal, numChannel_BC * sizeof(Dtype)));
        CUDA_CHECK(cudaMalloc(&BC_tmpVarLocal, numChannel_BC * sizeof(Dtype)));
        cudaMemset(BC_tmpMeanLocal, 0, byteChannel_BC);
        cudaMemset(BC_tmpVarLocal, 0, byteChannel_BC);
        BC_MeanInfVec.push_back(BC_tmpMeanLocal);
        BC_VarInfVec.push_back(BC_tmpVarLocal);
        Dtype* BC_localSaveMean;
        Dtype* BC_localSaveInvVar;
        CUDA_CHECK(cudaMalloc(&BC_localSaveMean, numChannel_BC * sizeof(Dtype)));
        CUDA_CHECK(cudaMalloc(&BC_localSaveInvVar, numChannel_BC * sizeof(Dtype)));
        cudaMemset(BC_localSaveMean, 0, byteChannel_BC);
        cudaMemset(BC_localSaveInvVar, 0, byteChannel_BC);
        ResultSaveMean_BC.push_back(BC_localSaveMean);
        ResultSaveInvVariance_BC.push_back(BC_localSaveInvVar);
      }
    }
    //Conv Descriptor
    this->conv_Descriptor = NULL;
    CUDNN_CHECK(cudnnCreateConvolutionDescriptor(&this->conv_Descriptor));
    CUDNN_CHECK(cudnnSetConvolution2dDescriptor(this->conv_Descriptor, 1, 1, 1, 1, 1, 1, CUDNN_CONVOLUTION, dataType<Dtype>::type));
    //CUDNN_CHECK(cudnnSetConvolution2dDescriptor(this->conv_Descriptor, 1, 1, 1, 1, 1, 1, CUDNN_CROSS_CORRELATION, dataType<Dtype>::type));


    //Convolution Algorithms
    for (int transitionIdx = 0; transitionIdx < numtransition_; ++transitionIdx) {
      cudnnTensorDescriptor_t conv_x_desc;
      cudnnTensorDescriptor_t conv_y_desc;
      cudnnFilterDescriptor_t conv_w_desc;
      cudnnTensorDescriptor_t BC_x_desc = NULL;
      cudnnTensorDescriptor_t BC_y_desc = NULL;
      cudnnFilterDescriptor_t BC_w_desc = NULL;
      if (useBC) {
        conv_x_desc = quadG_tensorDesc;
        conv_y_desc = tensorDescriptor_conv_y;
        conv_w_desc = filterDescriptorVec[transitionIdx];
        BC_x_desc = tensorDescriptorVec_conv_x[transitionIdx];
        BC_y_desc = quadG_tensorDesc;
        BC_w_desc = BC_filterDescriptorVec[transitionIdx];
      }
      else {
        conv_x_desc = (tensorDescriptorVec_conv_x[transitionIdx]);
        conv_y_desc = (tensorDescriptor_conv_y);
        conv_w_desc = (filterDescriptorVec[transitionIdx]);
      }
      //Conv Fwd Algo
      cudnnConvolutionFwdAlgo_t conv_FwdAlgo_local;
      CUDNN_CHECK(cudnnGetConvolutionForwardAlgorithm(
        cudnnHandlePtr,
        conv_x_desc, conv_w_desc, conv_Descriptor, conv_y_desc,
        CUDNN_CONVOLUTION_FWD_SPECIFY_WORKSPACE_LIMIT,
        workspace_size_bytes, &conv_FwdAlgo_local
      ));
      conv_FwdAlgoVec.push_back(conv_FwdAlgo_local);
      //Conv Bwd Filter Algo
      cudnnConvolutionBwdFilterAlgo_t conv_BwdFilter_local;
      CUDNN_CHECK(cudnnGetConvolutionBackwardFilterAlgorithm(
        cudnnHandlePtr,
        conv_x_desc, conv_y_desc, conv_Descriptor, conv_w_desc,
        CUDNN_CONVOLUTION_BWD_FILTER_SPECIFY_WORKSPACE_LIMIT,
        workspace_size_bytes, &conv_BwdFilter_local
      ));
      conv_BwdFilterAlgoVec.push_back(conv_BwdFilter_local);
      //Conv Bwd Data Algo
      cudnnConvolutionBwdDataAlgo_t conv_BwdData_local;
      CUDNN_CHECK(cudnnGetConvolutionBackwardDataAlgorithm(
        (this->extraHandles),
        conv_w_desc, conv_y_desc, conv_Descriptor, conv_x_desc,
        CUDNN_CONVOLUTION_BWD_DATA_SPECIFY_WORKSPACE_LIMIT,
        workspace_size_bytes, &conv_BwdData_local
      ));
      conv_BwdDataAlgoVec.push_back(conv_BwdData_local);
      //BC Convolution
      if (useBC) {
        //BC Fwd Algo
        cudnnConvolutionFwdAlgo_t BC_FwdAlgo_local;
        CUDNN_CHECK(cudnnGetConvolutionForwardAlgorithm(
          cudnnHandlePtr,
          BC_x_desc, BC_w_desc, convBC_Descriptor, BC_y_desc,
          CUDNN_CONVOLUTION_FWD_SPECIFY_WORKSPACE_LIMIT,
          workspace_size_bytes, &BC_FwdAlgo_local
        ));
        BC_FwdAlgoVec.push_back(BC_FwdAlgo_local);
        //BC Bwd Filter Algo
        cudnnConvolutionBwdFilterAlgo_t BC_BwdFilter_local;
        CUDNN_CHECK(cudnnGetConvolutionBackwardFilterAlgorithm(
          cudnnHandlePtr,
          BC_x_desc, BC_y_desc, convBC_Descriptor, BC_w_desc,
          CUDNN_CONVOLUTION_BWD_FILTER_SPECIFY_WORKSPACE_LIMIT,
          workspace_size_bytes, &BC_BwdFilter_local
        ));
        BC_BwdFilterAlgoVec.push_back(BC_BwdFilter_local);
        //BC Bwd Data Algo
        cudnnConvolutionBwdDataAlgo_t BC_BwdData_local;
        CUDNN_CHECK(cudnnGetConvolutionBackwardDataAlgorithm(
          (this->extraHandles),
          BC_w_desc, BC_y_desc, convBC_Descriptor, BC_x_desc,
          CUDNN_CONVOLUTION_BWD_DATA_SPECIFY_WORKSPACE_LIMIT,
          workspace_size_bytes, &BC_BwdData_local
        ));
        BC_BwdDataAlgoVec.push_back(BC_BwdData_local);
      }
    }
  }

  virtual void resetDropoutDesc() {
    for (int transitionIdx = 0; transitionIdx < numtransition_; ++transitionIdx) {
      std::cout << &(dropout_state_gpu[transitionIdx]) << "," << dropout_stateSize[transitionIdx] << std::endl;
      CUDNN_CHECK(cudnnSetDropoutDescriptor(
        (dropoutDescriptorVec[transitionIdx]),
        cudnnHandlePtr,
        dropoutAmount,
        dropout_state_gpu[transitionIdx],
        dropout_stateSize[transitionIdx],
        DB_randomSeed
      ));
      DB_randomSeed++;
    }
  }

  virtual void bottleneck_Forward_(int transitionIdx) {
    //BN Fwd 
    Dtype* BN_x_ptr;
    if (this->phase_ == TRAIN && useDropout) {
      BN_x_ptr = this->postDropout.gpu_mdata();
    }
    else {
      BN_x_ptr = this->postConv.gpu_mdata();
    }
    Dtype* BN_y_ptr = this->postBN.gpu_mdata();
    Dtype* BN_globalMean = this->blobs_[3 * this->numtransition_ + transitionIdx]->gpu_mdata();
    Dtype* BN_globalVar = this->blobs_[4 * this->numtransition_ + transitionIdx]->gpu_mdata();
    //Dtype* BC_filter = this->blobs_[5 * numtransition_ + transitionIdx]->gpu_mdata();
    cudnnTensorDescriptor_t BN_paramDesc = tensorDescriptor_BN[transitionIdx];
    int numChannels = initChannel + growthRate*transitionIdx;
    Dtype* local_MeanInf = this->Mean_tmp;
    Dtype* local_VarInf = this->Var_tmp;

    const Dtype* bnScale = this->blobs_[this->numtransition_ + transitionIdx]->gpu_data();
    const Dtype *bnBias = this->blobs_[2 * this->numtransition_ + transitionIdx]->gpu_data();
    int localChannels = this->initChannel + transitionIdx * this->growthRate;
    int inner_num_ = this->H * this->W;
    if (this->phase_ == TEST) {
      if (1) {
        CUDNN_CHECK(cudnnBatchNormalizationForwardInference(
          cudnnHandlePtr, CUDNN_BATCHNORM_SPATIAL,
          cuda_get_one(), cuda_get_zero(),
          (this->tensorDescriptorVec_conv_x[transitionIdx]), BN_x_ptr,
          (this->tensorDescriptorVec_conv_x[transitionIdx]), BN_y_ptr,
          BN_paramDesc,
          bnScale,
          this->blobs_[2 * this->numtransition_ + transitionIdx]->gpu_data(),
          BN_globalMean, BN_globalVar, CUDNN_BN_MIN_EPSILON)
        );
        //log_blob(this->postBN);
      }
      if (0) {
        BatchNormalizationForwardInference(this->N, localChannels, inner_num_, BN_x_ptr, BN_y_ptr, bnScale, bnBias, BN_globalMean, BN_globalVar, BN_MIN_EPSILON);
      }
      //log_blob(this->postBN);
      //log_blob(this->postConv);
    }
    else {
      Dtype* batchMean = this->ResultSaveMean_gpu[transitionIdx];
      Dtype* batchInvVar = this->ResultSaveInvVariance_gpu[transitionIdx];
      if (1) {
        CUDNN_CHECK(cudnnBatchNormalizationForwardTraining(
          cudnnHandlePtr, CUDNN_BATCHNORM_SPATIAL,
          cuda_get_one(), cuda_get_zero(),
          (this->tensorDescriptorVec_conv_x[transitionIdx]), BN_x_ptr,
          (this->tensorDescriptorVec_conv_x[transitionIdx]), BN_y_ptr,
          BN_paramDesc,
          bnScale,
          bnBias,
          Dtype(1), local_MeanInf, local_VarInf, CUDNN_BN_MIN_EPSILON,
          batchMean, batchInvVar)
        );
        //update global Mean/Var manually
        //Mean:
        cuda_caffe_axpby(numChannels, EMA_decay, local_MeanInf, Dtype(1.0 - EMA_decay), BN_globalMean);
        //Var:
        cuda_caffe_axpby(numChannels, EMA_decay, local_VarInf, Dtype(1.0 - EMA_decay), BN_globalVar);
#define DEBUG_cudnnBatchNormalizationForwardTraining \
      LOG(INFO) << caffe_asum(numChannels, local_MeanInf); \
      LOG(INFO) << caffe_asum(numChannels, batchMean); \
      LOG(INFO) << caffe_asum(numChannels, local_VarInf); \
      LOG(INFO) << caffe_asum(numChannels, batchInvVar); \
      LOG(INFO) << caffe_asum(numChannels, BN_globalMean); \
      LOG(INFO) << caffe_asum(numChannels, BN_globalVar);
        //DEBUG_cudnnBatchNormalizationForwardTraining;
      }
      if (0) {
        BatchNormalizationForwardTraining(this->N, localChannels, inner_num_, BN_x_ptr, BN_y_ptr, bnScale, bnBias, Dtype(1), local_MeanInf, local_VarInf, CUDNN_BN_MIN_EPSILON, batchMean, batchInvVar);
        //update global Mean/Var manually
        //Mean:
        cuda_caffe_axpby(numChannels, EMA_decay, local_MeanInf, Dtype(1.0 - EMA_decay), BN_globalMean);
        //Var:
        cuda_caffe_axpby(numChannels, EMA_decay, local_VarInf, Dtype(1.0 - EMA_decay), BN_globalVar);
        //DEBUG_cudnnBatchNormalizationForwardTraining;
      }
      if (0) {
        if (this->phase_ == TRAIN && useDropout) {
          BN_x_ptr = this->postDropout.cpu_mdata();
        }
        else {
          BN_x_ptr = this->postConv.cpu_mdata();
        }
        Dtype* BN_y_ptr = this->postBN.cpu_mdata();
        Dtype* BN_globalMean = this->blobs_[3 * this->numtransition_ + transitionIdx]->cpu_mdata();
        Dtype* BN_globalVar = this->blobs_[4 * this->numtransition_ + transitionIdx]->cpu_mdata();
        const Dtype* bnScale = this->blobs_[this->numtransition_ + transitionIdx]->cpu_data();
        const Dtype *bnBias = this->blobs_[2 * this->numtransition_ + transitionIdx]->cpu_data();
        Dtype local_MeanInf[64] = { 0 };
        Dtype local_VarInf[64] = { 0 };
        Dtype batchMean[64] = { 0 };
        Dtype batchInvVar[64] = { 0 };
        cpu_BatchNormalizationForwardTraining(this->N, localChannels, inner_num_, BN_x_ptr, BN_y_ptr, bnScale, bnBias, Dtype(1), local_MeanInf, local_VarInf, CUDNN_BN_MIN_EPSILON, batchMean, batchInvVar);
        //update global Mean/Var manually
        //Mean:
        //cpu_caffe_axpby(numChannels, EMA_decay, local_MeanInf, Dtype(1.0 - EMA_decay), BN_globalMean);
        //Var:
        //cpu_caffe_axpby(numChannels, EMA_decay, local_VarInf, Dtype(1.0 - EMA_decay), BN_globalVar);
        //DEBUG_cudnnBatchNormalizationForwardTraining;
      }
      if (1) {}

    }
    //ReLU
    Dtype* ReLU_x_ptr = this->postBN.gpu_mdata();
    Dtype* ReLU_y_ptr = this->postReLU.gpu_mdata();
    if (0) {
      CUDNN_CHECK(cudnnActivationForward(cudnnHandlePtr, ReLUDesc,
        cuda_get_one(),
        (this->tensorDescriptorVec_conv_x[transitionIdx]), ReLU_x_ptr,
        cuda_get_zero(),
        (this->tensorDescriptorVec_conv_x[transitionIdx]), ReLU_y_ptr)
      );
    }
    if (1) {
      int count = this->postBN.count();
      relu_forward(count, ReLU_x_ptr, ReLU_y_ptr, 0);
    }
    //log_blob(this->postReLU);
    if (useBC) {
      //Convolution 1*1 kernel 
      Dtype* conv_x_4G = postReLU.gpu_mdata();
      Dtype* conv_y_4G;
      if (BC_ultra_spaceEfficient) {
        conv_y_4G = postConv_4G.gpu_mdata();
      }
      else {
        conv_y_4G = postConv_4GVec[transitionIdx];
      }
      //CONV_ALGO
      CUDNN_CHECK(cudnnConvolutionForward(cudnnHandlePtr,
        cuda_get_one(),
        this->tensorDescriptorVec_conv_x[transitionIdx], conv_x_4G,
        this->BC_filterDescriptorVec[transitionIdx],
        this->blobs_[5 * numtransition_ + transitionIdx]->gpu_data(),
        convBC_Descriptor, BC_FwdAlgoVec[transitionIdx],
        workspace, workspace_size_bytes, cuda_get_zero(),
        quadG_tensorDesc, conv_y_4G
      ));
      //std::cout<<"BC Fwd Conv Done"<<std::endl;
      //BN 4G Fwd
      Dtype* BN_x_4G = BC_ultra_spaceEfficient ? postConv_4G.gpu_mdata() : postConv_4GVec[transitionIdx];
      Dtype* BN_y_4G = postBN_4G.gpu_mdata();
      Dtype* BN_BC_globalMean = this->blobs_[8 * numtransition_ + transitionIdx]->gpu_mdata();
      Dtype* BN_BC_globalVar = this->blobs_[9 * numtransition_ + transitionIdx]->gpu_mdata();
      Dtype* localBC_MeanInf = BC_MeanInfVec[transitionIdx];
      Dtype* localBC_VarInf = BC_VarInfVec[transitionIdx];
      //std::cout<<"BC Fwd BN Prepared"<<std::endl;
      if (this->phase_ == TEST) {
        CUDNN_CHECK(cudnnBatchNormalizationForwardInference(
          cudnnHandlePtr, CUDNN_BATCHNORM_SPATIAL,
          cuda_get_one(), cuda_get_zero(),
          quadG_tensorDesc, BN_x_4G,
          quadG_tensorDesc, BN_y_4G,
          quadG_paramDesc,
          this->blobs_[6 * numtransition_ + transitionIdx]->gpu_data(),
          this->blobs_[7 * numtransition_ + transitionIdx]->gpu_data(),
          BN_BC_globalMean, BN_BC_globalVar, CUDNN_BN_MIN_EPSILON)
        );
      }
      else {
        Dtype* BC_batchMean = ResultSaveMean_BC[transitionIdx];
        Dtype* BC_batchInvVar = ResultSaveInvVariance_BC[transitionIdx];
        CUDNN_CHECK(cudnnBatchNormalizationForwardTraining(
          cudnnHandlePtr, CUDNN_BATCHNORM_SPATIAL,
          cuda_get_one(), cuda_get_zero(),
          quadG_tensorDesc, BN_x_4G,
          quadG_tensorDesc, BN_y_4G,
          quadG_paramDesc,
          this->blobs_[6 * numtransition_ + transitionIdx]->gpu_mdata(),
          this->blobs_[7 * numtransition_ + transitionIdx]->gpu_mdata(),
          Dtype(1), localBC_MeanInf, localBC_VarInf, CUDNN_BN_MIN_EPSILON,
          BC_batchMean, BC_batchInvVar
        ));
        cuda_caffe_axpby(4 * growthRate, EMA_decay, localBC_MeanInf, Dtype(1.0 - EMA_decay), BN_BC_globalMean);
        cuda_caffe_axpby(4 * growthRate, EMA_decay, localBC_VarInf, Dtype(1.0 - EMA_decay), BN_BC_globalVar);
      }
      //std::cout<<"BC Fwd BN Done"<<std::endl;
      //ReLU 4G Fwd
      Dtype* ReLU_BC_x = postBN_4G.gpu_mdata();
      Dtype* ReLU_BC_y = postReLU_4G.gpu_mdata();
      CUDNN_CHECK(cudnnActivationForward(cudnnHandlePtr, ReLUDesc,
        cuda_get_one(),
        quadG_tensorDesc, ReLU_BC_x,
        cuda_get_zero(),
        quadG_tensorDesc, ReLU_BC_y
      ));
      //std::cout<<"BC Fwd ReLU Done"<<std::endl;
    }
    //Convolution
    int delayChannel = this->initChannel + this->growthRate * transitionIdx;
    Dtype* conv_x_local;
    cudnnTensorDescriptor_t conv_x_localDesc;
    Blob* conv_x = NULL;
    if (useBC) {
      conv_x = &postReLU_4G;
      conv_x_local = postReLU_4G.gpu_mdata();
      conv_x_localDesc = quadG_tensorDesc;
    }
    else {
      conv_x = &postReLU;
      conv_x_local = postReLU.gpu_mdata();
      conv_x_localDesc = tensorDescriptorVec_conv_x[transitionIdx];
    }
    Dtype* conv_y_local = this->postConv.gpu_mdata() + delayChannel * this->H * this->W;
    //CONV_ALGO
    if (1) {
      //uutime a;
      CUDNN_CHECK(cudnnConvolutionForward(cudnnHandlePtr,
        cuda_get_one(),
        conv_x_localDesc, conv_x_local,
        filterDescriptorVec[transitionIdx],
        this->blobs_[transitionIdx]->gpu_data(),
        conv_Descriptor, conv_FwdAlgoVec[transitionIdx],
        workspace, workspace_size_bytes, cuda_get_zero(),
        tensorDescriptor_conv_y, conv_y_local
      )
      );
      //LOG(INFO) << a.elapsed();
      //log_blob(this->postConv);
    }
    //  void cudnn_conv2d(const Dtype* inData, float* outData, const Dtype* weights, const Dtype* biasData,
      //DataShape inSize, DataShape outSize, int kernel_h, int kernel_w, int stride_h, int stride_w,
      //int dilation_h, int dilation_w, int pad_h, int pad_w, int group_, bool cross_correlation)
#if 0
    if (1) {
      uutime a;

      void cudnn_conv2d(const Dtype* inData, float* outData, const Dtype* weights, const Dtype* biasData,
        DataShape inSize, DataShape outSize, int kernel_h, int kernel_w, int stride_h, int stride_w,
        int dilation_h, int dilation_w, int pad_h, int pad_w, int group_, bool cross_correlation);
      //     static void FUN(conv2d)(const Dtype* inData, float* outData, const Dtype* weights, const Dtype* biasData,
      // DataShape inSize, DataShape outSize, int kernel_h, int kernel_w, int stride_h, int stride_w,
      // int dilation_h, int dilation_w, int pad_h, int pad_w, int groups)
      int conv_x_channels = this->initChannel + this->growthRate * transitionIdx;
      Blob* conv_w = this->blobs_[transitionIdx];
      DataShape conv_x_shape = conv_x->shape_;
      conv_x_shape.c = conv_x_channels;
      DataShape conv_y_shape = dataShape(this->N, this->growthRate, this->H, this->W);
      Dtype* conv_x_local = postReLU.gpu_mdata();
      Dtype* conv_y_local = this->postConv.gpu_mdata() + delayChannel * this->H * this->W;
      const Dtype* w = conv_w->gpu_data();
      //cuda_caffe_set(conv_y_shape.count(), 0, conv_y_local);
      cudnn_conv2d(conv_x_local, conv_y_local, w, NULL, conv_x_shape, conv_y_shape, conv_w->shape_.h, conv_w->shape_.w, 1, 1, 1, 1, 1, 1, 1, false);
      LOG(INFO) << a.elapsed();
      log_blob(this->postConv);
      1 == 1;
    }
#endif
    if (0) {
      //uutime a;
      //     static void FUN(conv2d)(const Dtype* inData, float* outData, const Dtype* weights, const Dtype* biasData,
      // DataShape inSize, DataShape outSize, int kernel_h, int kernel_w, int stride_h, int stride_w,
      // int dilation_h, int dilation_w, int pad_h, int pad_w, int groups)
      int conv_x_channels = this->initChannel + this->growthRate * transitionIdx;
      Blob* conv_w = this->blobs_[transitionIdx];
      DataShape conv_x_shape = conv_x->shape_;
      conv_x_shape.c = conv_x_channels;
      DataShape conv_y_shape = dataShape(this->N, this->growthRate, this->H, this->W);
      Dtype* conv_x_local = postReLU.cpu_mdata();
      Dtype* conv_y_local = this->postConv.cpu_mdata() + delayChannel * this->H * this->W;
      const Dtype* w = conv_w->cpu_data();
      cpu_caffe_set(conv_y_shape.count(), 0, conv_y_local);
      cpu_conv2d(conv_x_local, conv_y_local, w, NULL, conv_x_shape, conv_y_shape, conv_w->shape_.h, conv_w->shape_.w, 1, 1, 1, 1, 1, 1, 1, false);
      //LOG(INFO) << a.elapsed();
      log_blob(this->postConv);
      1 == 1;
    }
    if (0) {
      int conv_x_channels = this->initChannel + this->growthRate * transitionIdx;
      Blob* conv_w = this->blobs_[transitionIdx];
      DataShape conv_x_shape = conv_x->shape_;
      conv_x_shape.c = conv_x_channels;
      conv_FwdAlgo.set(conv_w->shape_.h, conv_w->shape_.w, 1, 1, 1, 1, 1, 1, this->growthRate, 1, 1, false);
      conv_FwdAlgo.setup(conv_x_shape, conv_w, NULL, false);
      conv_FwdAlgo.Reshape(conv_x_shape);
      conv_FwdAlgo.Forward_(conv_x_local, conv_y_local, conv_w->gpu_data(), NULL, 0);
      log_blob(this->postConv);
    }
    //Dropout
    if ((this->phase_ == TRAIN) && useDropout) {
      Dtype* dropout_x_local = postConv.gpu_mdata() + delayChannel*H*W;
      Dtype* dropout_y_local = postDropout.gpu_mdata() + delayChannel*H*W;
      CUDNN_CHECK(cudnnDropoutForward(cudnnHandlePtr,
        dropoutDescriptorVec[transitionIdx],
        tensorDescriptor_conv_y, dropout_x_local,
        tensorDescriptor_conv_y, dropout_y_local,
        dropout_reserve_gpu[transitionIdx], dropout_reserveSize[transitionIdx]
      ));
    }
    //this->logInternal_gpu("TClogFwd",transitionIdx,true,false);
  }

  virtual void Backward_(const vector<Blob*>& top, const vector<Blob*>& bottom) {
#if 0
    if (!this->gpuInited) {
      this->GPU_Initialization();
      this->gpuInited = true;
    }
#endif
    //clock_t begin_bwd = std::clock();
    //assuming buffers store already computed value, always propagate down
    Dtype* bottom_diff = bottom[0]->gpu_mdiff();
    int work_n = N * (initChannel + growthRate*numtransition_) * H * W;
    //deploy top diff
    if (useDropout) {
      cudaMemcpy(postDropout.gpu_mdiff(), top[0]->gpu_mdiff(), work_n * sizeof(Dtype), cudaMemcpyDeviceToDevice);
    }
    else {
      cudaMemcpy(postConv.gpu_mdiff(), top[0]->gpu_mdiff(), work_n * sizeof(Dtype), cudaMemcpyDeviceToDevice);
    }
    //Backward_, transition by transition
    for (int transitionIdx = this->numtransition_ - 1; transitionIdx >= 0; --transitionIdx) {
      int channelsBefore_self = this->initChannel + transitionIdx * this->growthRate;
      //Using BN & ReLU Fwd to generate corresponding postBN,postReLU data for this transition 
      //BN Fwd
      Dtype* BN_x_ptr;
      if (useDropout) {
        BN_x_ptr = postDropout.gpu_mdata();
      }
      else {
        BN_x_ptr = postConv.gpu_mdata();
      }
      Dtype* BN_y_ptr = postBN.gpu_mdata();
      Dtype* BN_globalMean = this->blobs_[3 * this->numtransition_ + transitionIdx]->gpu_mdata();
      Dtype* BN_globalVar = this->blobs_[4 * this->numtransition_ + transitionIdx]->gpu_mdata();
      cudnnTensorDescriptor_t BN_paramDesc = tensorDescriptor_BN[transitionIdx];
      Dtype* local_MeanInf = Mean_tmp;
      Dtype* local_VarInf = Var_tmp;
      Dtype* batchMean = this->ResultSaveMean_gpu[transitionIdx];
      Dtype* batchInvVar = this->ResultSaveInvVariance_gpu[transitionIdx];
      CUDNN_CHECK(cudnnBatchNormalizationForwardTraining(
        cudnnHandlePtr, CUDNN_BATCHNORM_SPATIAL,
        cuda_get_one(), cuda_get_zero(),
        (this->tensorDescriptorVec_conv_x[transitionIdx]), BN_x_ptr,
        (this->tensorDescriptorVec_conv_x[transitionIdx]), BN_y_ptr,
        BN_paramDesc,
        this->blobs_[this->numtransition_ + transitionIdx]->gpu_mdata(),
        this->blobs_[2 * this->numtransition_ + transitionIdx]->gpu_mdata(),
        Dtype(1), local_MeanInf, local_VarInf, CUDNN_BN_MIN_EPSILON,
        batchMean, batchInvVar)
      );

      /*CUDNN_CHECK(cudnnBatchNormalizationForwardInference(
      cudnnHandlePtr,CUDNN_BATCHNORM_SPATIAL,
      cuda_get_one(),cuda_get_zero(),
      *(this->tensorDescriptorVec_conv_x[transitionIdx]),BN_x_ptr,
      *(this->tensorDescriptorVec_conv_x[transitionIdx]),BN_y_ptr,
      BN_paramDesc,
      this->blobs_[this->numtransition_+transitionIdx]->gpu_data(),
        this->blobs_[2*this->numtransition_+transitionIdx]->gpu_data(),
      local_MeanInf,local_VarInf,CUDNN_BN_MIN_EPSILON)
    );*/
    //ReLU Fwd
      Dtype* ReLU_x_ptr = this->postBN.gpu_mdata();
      Dtype* ReLU_y_ptr = this->postReLU.gpu_mdata();
      CUDNN_CHECK(cudnnActivationForward(cudnnHandlePtr, ReLUDesc,
        cuda_get_one(),
        (this->tensorDescriptorVec_conv_x[transitionIdx]), ReLU_x_ptr,
        cuda_get_zero(),
        (this->tensorDescriptorVec_conv_x[transitionIdx]), ReLU_y_ptr)
      );
      if (useBC) {
        //Fwd phase 
        //If BC Ultra SpaceEfficient, then need convolution Fwd 1*1
          //CONV_ALGO
        if (BC_ultra_spaceEfficient) {
          Dtype* conv_x_4G = postReLU.gpu_mdata();
          Dtype* conv_y_4G = postConv_4G.gpu_mdata();
          CUDNN_CHECK(cudnnConvolutionForward(cudnnHandlePtr,
            cuda_get_one(),
            this->tensorDescriptorVec_conv_x[transitionIdx], conv_x_4G,
            (BC_filterDescriptorVec[transitionIdx]),
            this->blobs_[5 * numtransition_ + transitionIdx]->gpu_data(),
            (convBC_Descriptor), BC_FwdAlgoVec[transitionIdx],
            workspace, workspace_size_bytes, cuda_get_zero(),
            quadG_tensorDesc, conv_y_4G
          ));
        }
        //cudnnHandle_t localFwdHandle = BC_ultra_spaceEfficient?cudnnHandlePtr:extraHandles[0];//TODO
        cudnnHandle_t localFwdHandle = cudnnHandlePtr;
        //BC BN Fwd reconstruction
        Dtype* BN_x_4G = BC_ultra_spaceEfficient ? postConv_4G.gpu_mdata() : postConv_4GVec[transitionIdx];
        Dtype* BN_y_4G = postBN_4G.gpu_mdata();
        Dtype* localBC_MeanInf = BC_MeanInfVec[transitionIdx];
        Dtype* localBC_VarInf = BC_VarInfVec[transitionIdx];
        Dtype* BC_batchMean = ResultSaveMean_BC[transitionIdx];
        Dtype* BC_batchInvVar = ResultSaveInvVariance_BC[transitionIdx];
        CUDNN_CHECK(cudnnBatchNormalizationForwardTraining(
          cudnnHandlePtr, CUDNN_BATCHNORM_SPATIAL,
          cuda_get_one(), cuda_get_zero(),
          quadG_tensorDesc, BN_x_4G,
          quadG_tensorDesc, BN_y_4G,
          quadG_paramDesc,
          this->blobs_[6 * numtransition_ + transitionIdx]->gpu_mdata(),
          this->blobs_[7 * numtransition_ + transitionIdx]->gpu_mdata(),
          Dtype(1), localBC_MeanInf, localBC_VarInf, CUDNN_BN_MIN_EPSILON,
          BC_batchMean, BC_batchInvVar
        ));
        /*CUDNN_CHECK(cudnnBatchNormalizationForwardInference(
          localFwdHandle,CUDNN_BATCHNORM_SPATIAL,
          cuda_get_one(),cuda_get_zero(),
          quadG_tensorDesc,BN_x_4G,
          quadG_tensorDesc,BN_y_4G,
          quadG_paramDesc,
          this->blobs_[6*numtransition_+transitionIdx]->gpu_data(),
          this->blobs_[7*numtransition_+transitionIdx]->gpu_data(),
          localBC_MeanInf,localBC_VarInf,CUDNN_BN_MIN_EPSILON
        ));*/
        //BC ReLU Fwd reconstruction
        Dtype* ReLU_BC_x = postBN_4G.gpu_mdata();
        Dtype* ReLU_BC_y = postReLU_4G.gpu_mdata();
        CUDNN_CHECK(cudnnActivationForward(localFwdHandle, ReLUDesc,
          cuda_get_one(),
          quadG_tensorDesc, ReLU_BC_x,
          cuda_get_zero(),
          quadG_tensorDesc, ReLU_BC_y
        ));
      }
      //CUDA_CHECK(cudaStreamSynchronize(*(extraStreams[0])));
      //sync_streams<<<1, 1>>>();	
        //Now do Bwd
        //Dropout
      if (useDropout) {
        Dtype* dropout_dy_ptr = postDropout.gpu_mdiff() + channelsBefore_self*H*W;
        Dtype* dropout_dx_ptr = postConv.gpu_mdiff() + channelsBefore_self*H*W;
        CUDNN_CHECK(cudnnDropoutBackward(cudnnHandlePtr,
          (dropoutDescriptorVec[transitionIdx]),
          tensorDescriptor_conv_y, dropout_dy_ptr,
          tensorDescriptor_conv_y, dropout_dx_ptr,
          dropout_reserve_gpu[transitionIdx], dropout_reserveSize[transitionIdx]
        ));
      }
      //Conv
      Dtype* filterGrad_local = this->blobs_[transitionIdx]->gpu_mdiff();
      Dtype* filterData_local = this->blobs_[transitionIdx]->gpu_mdata();
      Dtype* conv_x_local = useBC ? postReLU_4G.gpu_mdata() : postReLU.gpu_mdata();
      Dtype* conv_dy_local = postConv.gpu_mdiff() + channelsBefore_self * this->H * this->W;
      Dtype* conv_dx_local = useBC ? postReLU_4G.gpu_mdiff() : postReLU.gpu_mdiff();
      cudnnTensorDescriptor_t conv_x_localDesc = useBC ? quadG_tensorDesc : tensorDescriptorVec_conv_x[transitionIdx];
      //Conv w.r.t. filter
      //CONV_ALGO
      CUDNN_CHECK(cudnnConvolutionBackwardFilter(cudnnHandlePtr,
        cuda_get_one(),
        conv_x_localDesc, conv_x_local,
        (this->tensorDescriptor_conv_y), conv_dy_local,
        (this->conv_Descriptor), conv_BwdFilterAlgoVec[transitionIdx],
        this->workspace, this->workspace_size_bytes,
        cuda_get_one(),
        (this->filterDescriptorVec[transitionIdx]), filterGrad_local
      )
      );
      //Conv w.r.t. x
        //CONV_ALGO
      CUDNN_CHECK(cudnnConvolutionBackwardData(this->extraHandles,
        cuda_get_one(),
        this->filterDescriptorVec[transitionIdx], filterData_local,
        this->tensorDescriptor_conv_y, conv_dy_local,
        this->conv_Descriptor, conv_BwdDataAlgoVec[transitionIdx],
        this->workspace2, this->workspace_size_bytes,
        cuda_get_zero(),
        conv_x_localDesc, conv_dx_local
      )
      );
      //sync_streams << <1, 1 >> > ();
      if (useBC) {
        //BC ReLU Bwd
        Dtype* BC_ReLU_y_local = postReLU_4G.gpu_mdata();
        Dtype* BC_ReLU_dy_local = postReLU_4G.gpu_mdiff();
        Dtype* BC_ReLU_x_local = postBN_4G.gpu_mdata();
        Dtype* BC_ReLU_dx_local = postBN_4G.gpu_mdiff();
        CUDNN_CHECK(cudnnActivationBackward(cudnnHandlePtr, ReLUDesc,
          cuda_get_one(),
          quadG_tensorDesc, BC_ReLU_y_local,
          quadG_tensorDesc, BC_ReLU_dy_local,
          quadG_tensorDesc, BC_ReLU_x_local,
          cuda_get_zero(),
          quadG_tensorDesc, BC_ReLU_dx_local
        ));
        //BC BN Bwd
        Dtype* BC_BN_x_local = BC_ultra_spaceEfficient ? postConv_4G.gpu_mdata() : postConv_4GVec[transitionIdx];
        Dtype* BC_BN_dx_local = postConv_4G.gpu_mdiff();
        Dtype* BC_BN_dy_local = postBN_4G.gpu_mdiff();
        Dtype* BC_saveMean_local = ResultSaveMean_BC[transitionIdx];
        Dtype* BC_saveInvVar_local = ResultSaveInvVariance_BC[transitionIdx];
        // CUDNN_CHECK(
        cudnnStatus_t sta =
          cudnnBatchNormalizationBackward(
            cudnnHandlePtr,
            CUDNN_BATCHNORM_SPATIAL,
            cuda_get_one(),
            cuda_get_zero(),
#if CUDNN_VERSION >= 4005
            cuda_get_one(),
            cuda_get_one(),
#endif
            quadG_tensorDesc,
            BC_BN_x_local,
            quadG_tensorDesc,
            BC_BN_dy_local,
            quadG_tensorDesc,
            BC_BN_dx_local,
            quadG_paramDesc,
            this->blobs_[6 * numtransition_ + transitionIdx]->gpu_data(),
            this->blobs_[6 * numtransition_ + transitionIdx]->gpu_mdiff(),
            this->blobs_[7 * numtransition_ + transitionIdx]->gpu_mdiff(),
            CUDNN_BN_MIN_EPSILON,
            BC_saveMean_local,
            BC_saveInvVar_local
          );
        //);	

      //BC Conv 1*1 Bwd
        Dtype* BC_filterGrad = this->blobs_[5 * numtransition_ + transitionIdx]->gpu_mdiff();
        Dtype* BC_filterData = this->blobs_[5 * numtransition_ + transitionIdx]->gpu_mdata();
        Dtype* BC_conv_x_local = postReLU.gpu_mdata();
        Dtype* BC_conv_dy_local = postConv_4G.gpu_mdiff();
        Dtype* BC_conv_dx_local = postReLU.gpu_mdiff();
        //Conv Bwd w.r.t. filter
        //CONV_ALGO
        CUDNN_CHECK(cudnnConvolutionBackwardFilter(cudnnHandlePtr,
          cuda_get_one(),
          tensorDescriptorVec_conv_x[transitionIdx], BC_conv_x_local,
          quadG_tensorDesc, BC_conv_dy_local,
          convBC_Descriptor, BC_BwdFilterAlgoVec[transitionIdx],
          workspace, workspace_size_bytes,
          cuda_get_one(),
          BC_filterDescriptorVec[transitionIdx], BC_filterGrad
        ));
        //Conv Bwd w.r.t. data
        //CONV_ALGO
        CUDNN_CHECK(cudnnConvolutionBackwardData(extraHandles,
          cuda_get_one(),
          BC_filterDescriptorVec[transitionIdx], BC_filterData,
          quadG_tensorDesc, BC_conv_dy_local,
          convBC_Descriptor, BC_BwdDataAlgoVec[transitionIdx],
          workspace2, workspace_size_bytes,
          cuda_get_zero(),
          tensorDescriptorVec_conv_x[transitionIdx], BC_conv_dx_local
        ));
        //sync_streams << <1, 1 >> > ();
      }
      //ReLU Bwd
      Dtype* ReLU_y_local = postReLU.gpu_mdata();
      Dtype* ReLU_x_local = postBN.gpu_mdata();
      Dtype* ReLU_dy_local = postReLU.gpu_mdiff();
      Dtype* ReLU_dx_local = postBN.gpu_mdiff();
      CUDNN_CHECK(cudnnActivationBackward(cudnnHandlePtr, ReLUDesc,
        cuda_get_one(),
        (this->tensorDescriptorVec_conv_x[transitionIdx]), ReLU_y_local,
        (this->tensorDescriptorVec_conv_x[transitionIdx]), ReLU_dy_local,
        (this->tensorDescriptorVec_conv_x[transitionIdx]), ReLU_x_local,
        cuda_get_zero(),
        (this->tensorDescriptorVec_conv_x[transitionIdx]), ReLU_dx_local)
      );
      //BN Bwd
      Dtype* BN_x_local;
      Dtype* BN_dx_local;
      if (useDropout) {
        BN_x_local = this->postDropout.gpu_mdata();
        BN_dx_local = this->postDropout.gpu_mdiff();
      }
      else {
        BN_x_local = this->postConv.gpu_mdata();
        BN_dx_local = this->postConv.gpu_mdiff();
      }
      Dtype* BN_dy_local = this->postBN.gpu_mdiff();
      Dtype* saveMean_local = this->ResultSaveMean_gpu[transitionIdx];
      Dtype* saveInvVar_local = this->ResultSaveInvVariance_gpu[transitionIdx];
      //CUDNN_CHECK(
      cudnnBatchNormalizationBackward(cudnnHandlePtr,
        CUDNN_BATCHNORM_SPATIAL,
        cuda_get_one(), cuda_get_one(),
#if CUDNN_VERSION >= 4005
        cuda_get_one(), cuda_get_one(),
#endif	  
        (this->tensorDescriptorVec_conv_x[transitionIdx]), BN_x_local,
        (this->tensorDescriptorVec_conv_x[transitionIdx]), BN_dy_local,
        (this->tensorDescriptorVec_conv_x[transitionIdx]), BN_dx_local,
        BN_paramDesc,
        this->blobs_[this->numtransition_ + transitionIdx]->gpu_data(),
        this->blobs_[this->numtransition_ + transitionIdx]->gpu_mdiff(),
        this->blobs_[2 * this->numtransition_ + transitionIdx]->gpu_mdiff(),
        CUDNN_BN_MIN_EPSILON, saveMean_local, saveInvVar_local
      );
      //);
        //this->logInternal_gpu("TClogBwd",transitionIdx,true,false);
        //this->logInternal_gpu("TClogBwd",transitionIdx,true,true);
    }
    //deploy buffer to bottom diff
    //this->logInternal_gpu("TClogBwd",-1,false,false);
    int chunkSize_copy_init = this->initChannel * this->H * this->W;
    int chunkStride_copy = (this->initChannel + this->numtransition_ * this->growthRate) * this->H * this->W;
    if (useDropout) {
      copy_many_to_one(postDropout.mdiff(), bottom_diff, this->N, chunkSize_copy_init, chunkStride_copy);
      //this->resetDropoutDesc();
    }
    else {
      copy_many_to_one(postConv.mdiff(), bottom_diff, this->N, chunkSize_copy_init, chunkStride_copy);
    }
    int numTotalChannels = initChannel + growthRate*numtransition_;
    cleanupBuffer(this->Mean_tmp, numTotalChannels);
    cleanupBuffer(this->Var_tmp, numTotalChannels);
    this->LoopEndCleanup_gpu();
    //clock_t end_bwd = std::clock();
    //double elapsed_bwd = double(end_bwd - begin_bwd) / CLOCKS_PER_SEC;
    //std::cout<<"elapsed bwd time:"<<elapsed_bwd<<std::endl;
  }

  void ReallocCudaMem(Dtype** p, int size)
  {
    cudaFree(*p); *p = 0;
    CUDA_CHECK(cudaMalloc(p, size));
    cudaMemset(*p, 0, size);
  }

  virtual void reshape_data(int oldh, int oldw, int oldn, int h, int w, int newn)
  {
    cudnnDataType_t type = dataType<Dtype>::type;
    int bufferSize_byte_old = oldn*(this->initChannel + this->growthRate*this->numtransition_)*oldh*oldw * sizeof(Dtype);
    int bufferSize_byte_new = newn*(this->initChannel + this->growthRate*this->numtransition_)*h*w * sizeof(Dtype);
    DataShape shape_new = dataShape(newn, (this->initChannel + this->growthRate*this->numtransition_), h, w);
    if (bufferSize_byte_new > bufferSize_byte_old)
    {
      int bufferSize_byte = bufferSize_byte_new;

      postConv.Reshape(shape_new);

      if (useDropout) {
        postDropout.Reshape(shape_new);
      }
      postBN.Reshape(shape_new);
      postReLU.Reshape(shape_new);
    }

    setTensor4dDesc(this->tensorDescriptor_conv_y, type, newn, this->growthRate, h, w, (this->numtransition_*this->growthRate + this->initChannel)*h*w, h*w, w, 1);

    int quadG_numValues_old = 4 * newn*growthRate*oldh*oldw;
    int quadG_numValues = 4 * newn*growthRate*h*w;
    int quadG_numBytes = quadG_numValues * sizeof(Dtype);
    if (quadG_numValues > quadG_numValues_old)
    {
      DataShape quadG_shape = dataShape(newn, 4 * growthRate, oldh, oldw);
      if (useBC)
      {
        postBN_4G.Reshape(quadG_shape);
        postReLU_4G.Reshape(quadG_shape);
        postConv_4G.Reshape(quadG_shape);

        if (BC_ultra_spaceEfficient) {
          //ReallocCudaMem(&postConv_4G.gpu_mdata(), quadG_numBytes);
        }
      }
    }
    if (useBC)
    {
      setTensor4dDesc(quadG_tensorDesc, type, newn, 4 * growthRate, h, w, 4 * growthRate*h*w, h*w, w, 1);
    }

    for (int i = 0; i < this->numtransition_; ++i)
    {
      int conv_x_channels = this->initChannel + this->growthRate * i;
      setTensor4dDesc(this->tensorDescriptorVec_conv_x[i], type, newn, conv_x_channels, h, w, (this->numtransition_*this->growthRate + this->initChannel)*h*w, h*w, w, 1);
    }
  }

};


INSTANTIATE_CLASS(DenseBlock, DenseBlock_cudnn);
