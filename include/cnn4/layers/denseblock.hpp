#ifndef CAFFE_DENSEBLOCK_LAYER_HPP_
#define CAFFE_DENSEBLOCK_LAYER_HPP_

#include <vector>
#include <string>

#define DenseBlockParameter_DEF(DEF) \
DEF##Struct(Filter_Filler, 0, Filler) \
DEF##Struct(BN_Scaler_Filler, 0, Filler) \
DEF##Struct(BN_Bias_Filler, 0, Filler)


class DenseBlockLayer : public Layer {
 public:

   DenseBlockParameter_DEF(Def);

#define BLOB_NAME(NAME)  NAME.set_name(#NAME);
#define BLOB_DEF1(NAME)  Blob NAME;
#define BLOB_DEF(DEF) \
  DEF(postConv) \
  DEF(postConv_4G) \
  DEF(postReLU_4G) \
  DEF(postDropout) \
  DEF(postBN) \
  DEF(postBN_4G) \
  DEF(postReLU) \


   //BLOB_DEF(BLOB_DEF1);
#undef BLOB_DEF1

   virtual void init() {
     DenseBlockParameter_DEF(Init);
   }
   virtual void toJson(cjson* param) {
     DenseBlockParameter_DEF(Set);
   }
   virtual void fromJson(cjson* param) {
     DenseBlockParameter_DEF(Get);
     //BLOB_DEF(BLOB_NAME);

     this->numTransition = cjson_GetObjectInt(param, "numTransition", 40);
     this->growthRate = cjson_GetObjectInt(param, "growthrate", 12);
     this->EMA_decay = cjson_GetObjectFloat(param, "moving_average_fraction", 0.1);

     this->trainCycleIdx = 0; //initially, trainCycleIdx = 0
#ifndef CPU_ONLY
     this->workspace_size_bytes = cjson_GetObjectInt(param, "workspace_mb", 8) * 1024 * 1024;
     this->gpu_idx_ = cjson_GetObjectInt(param, "gpuidx", 0);
#endif
     this->useDropout = cjson_GetObjectBool(param, "use_dropout", false);
     this->dropoutAmount = cjson_GetObjectFloat(param, "dropout_amount", 0);
     this->DB_randomSeed = 124816;
     this->useBC = cjson_GetObjectBool(param, "use_bc", false);
     this->BC_ultra_spaceEfficient = cjson_GetObjectBool(param, "bc_ultra_space_efficient", false);
     //this->BC_ultra_spaceEfficient = dbParam.bc_ultra_space_efficient();
   }

  virtual void LayerSetUp(const vector<Blob*>& bottom, const vector<Blob*>& top);
  
  virtual void Reshape(const vector<Blob*>& bottom, const vector<Blob*>& top); 
  
  virtual inline const char* type() const { return "DenseBlock"; } 

  virtual void Forward_cpu_public(const vector<Blob*>& bottom, const vector<Blob*>& top);

  virtual void Backward_cpu_public(const vector<Blob*>& top, const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  void Forward_gpu_public(const vector<Blob*>& bottom, const vector<Blob*>& top);

  void Backward_gpu_public(const vector<Blob*>& top, const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  
  virtual void syncBlobs(DenseBlockLayer* originLayer);

  virtual void setLogId(int uid);

  virtual void logInternal_cpu(string dir);

  void logInternal_gpu(string dir,int transitionIdx,bool logDynamic,bool logDiff);

  int initChannel, growthRate, numTransition; 
  int N,H,W; //N,H,W of the input tensor, inited in reshape phase
  
  bool useDropout;
  float dropoutAmount;
  unsigned long long DB_randomSeed;
  bool useBC;
  bool BC_ultra_spaceEfficient;
  
 protected:
  
  virtual void CPU_Initialization();

  void GPU_Initialization();
  void reshape_gpu_data(int oldh, int oldw, int oldn, int h, int w, int newn);

  virtual void LoopEndCleanup_cpu();

  void LoopEndCleanup_gpu();

  void resetDropoutDesc(); 

  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  
  //start logging specific data: for debugging
  int logId;
  //end logging specific data

  //performance related parameter
  int gpu_idx_;

  //common Blobs for both CPU & GPU mode
  //in this->blobs_, containing all filters for Convolution, scalers and bias for BN
  
  //start CPU specific data section
  bool cpuInited;
  //at T has shape (1,initC+T*growth,1,1)
  vector<Blob*> batch_Mean; 
  vector<Blob*> batch_Var;
  vector<Blob*> batch_Mean4G;
  vector<Blob*> batch_Var4G;

  vector<Blob*> merged_conv;//at T has shape (N,initC+T*growth,H,W), but this vector has T+1 elements

  vector<Blob*> BN_XhatVec;//at T has shape (N,initC+T*growth,H,W)
  vector<Blob*> postBN_blobVec;
  vector<Blob*> postReLU_blobVec;
  vector<Blob*> postConv_blobVec;//at T has shape(N,growth,H,W)
  //BC related CPU 
  vector<Blob*> BC_BN_XhatVec;//at T has shape(N,4*growthRate,H,W)
  vector<Blob*> postBN_BCVec;
  vector<Blob*> postReLU_BCVec;
  vector<Blob*> postConv_BCVec; 
  //end CPU specific data section

  int trainCycleIdx; //used in BN train phase for EMA Mean/Var estimation
					 //convolution Related
  int pad_h, pad_w, conv_verticalStride, conv_horizentalStride;
  int filter_H, filter_W;
  //Decay value used in EMA of BN
  Dtype EMA_decay;

#ifndef CPU_ONLY
  //start GPU specific data section
  //GPU ptr for efficient space usage only, these pointers not allocated when CPU_ONLY, these are not Blobs because Descriptor is not traditional 
  //bool gpuInited;
  Dtype* postConv_data_gpu;
  Dtype* postConv_grad_gpu;
  Dtype* postDropout_data_gpu;
  Dtype* postDropout_grad_gpu;
  Dtype* postBN_data_gpu;
  Dtype* postBN_grad_gpu;
  Dtype* postReLU_data_gpu;
  Dtype* postReLU_grad_gpu;
  Dtype* workspace;
  Dtype* workspace2;
  //gpu workspace size
  int workspace_size_bytes;

  vector<Dtype*> ResultSaveMean_gpu;
  vector<Dtype*> ResultSaveInvVariance_gpu;
  vector<void*> dropout_state_gpu;
  vector<size_t> dropout_stateSize;
  vector<void*> dropout_reserve_gpu;
  vector<size_t> dropout_reserveSize;
  Dtype* Mean_tmp;//used in BN inf
  Dtype* Var_tmp;//used in BN inf
  
  //BC related parameters 
  vector<Dtype*> postConv_4GVec; //used if not ultra space efficient mode
  Dtype* postConv_4G; //used if ultra space efficient mode
  Dtype* postBN_4G;
  Dtype* postReLU_4G;  
  Dtype* postConv_4G_grad;
  Dtype* postBN_4G_grad;
  Dtype* postReLU_4G_grad;
  cudnnTensorDescriptor_t * quadG_tensorDesc;
  cudnnTensorDescriptor_t * quadG_paramDesc;
  cudnnConvolutionDescriptor_t* convBC_Descriptor;
   vector<Dtype*> BC_MeanInfVec;
  vector<Dtype*> BC_VarInfVec;
  vector<Dtype*> ResultSaveMean_BC;
  vector<Dtype*> ResultSaveInvVariance_BC;
   vector<cudnnFilterDescriptor_t *> BC_filterDescriptorVec;
  //chosen Fwd,BwdFilter,BwdData algos for BC-Conv/Normal-Conv
  vector<cudnnConvolutionFwdAlgo_t *> conv_FwdAlgoVec;
  vector<cudnnConvolutionFwdAlgo_t *> BC_FwdAlgoVec;
  vector<cudnnConvolutionBwdFilterAlgo_t *> conv_BwdFilterAlgoVec;
  vector<cudnnConvolutionBwdFilterAlgo_t *> BC_BwdFilterAlgoVec;
  vector<cudnnConvolutionBwdDataAlgo_t *> conv_BwdDataAlgoVec;
  vector<cudnnConvolutionBwdDataAlgo_t *> BC_BwdDataAlgoVec; 
   //BC_dropout
  //vector<void*> BC_dropout_state;
  //vector<void*> BC_dropout_reserve;
  //vector<size_t> BC_dropout_stateSize;
  //vector<size_t> BC_dropout_reserveSize;
  //Dtype* postDropout_4G;
  //Dtype* postDropout_4G_grad;
  
 
   //gpu handles and descriptors
  cudnnHandle_t* cudnnHandlePtr;
  cudaStream_t* cudaPrimalStream;
  vector<cudnnHandle_t*> extraHandles;
  vector<cudaStream_t*> extraStreams;

  vector<cudnnTensorDescriptor_t *> tensorDescriptorVec_conv_x;//local Conv X
  cudnnTensorDescriptor_t * tensorDescriptor_conv_y;//local Conv Y
  vector<cudnnTensorDescriptor_t *> tensorDescriptor_BN;//<channelwise>
  //Dropout descriptor 
  vector<cudnnDropoutDescriptor_t *> dropoutDescriptorVec;
  //filter descriptor for conv
  vector<cudnnFilterDescriptor_t *> filterDescriptorVec;
  //ReLU Activation Descriptor  
  cudnnActivationDescriptor_t* ReLUDesc;
  //conv descriptor for conv
  cudnnConvolutionDescriptor_t* conv_Descriptor;
#endif
  //end GPU specific data setion
};

void DenseBlockLayer::LayerSetUp(const vector<Blob*>& bottom, const vector<Blob*>& top)
{
  this->cpuInited = false;
  // #ifndef CPU_ONLY
  // 		this->gpuInited = false;
  // #endif

  this->N = bottom[0]->shape()[0];
  this->H = bottom[0]->shape()[2];
  this->W = bottom[0]->shape()[3];

  this->initChannel = bottom[0]->channels();
  //modified by jxs
  //Parameter Blobs
  //for transition i, 
  //blobs_[i] is its filter blob
  //blobs_[numTransition + i] is its scaler blob
  //blobs_[2*numTransition + i] is its bias blob
  //blobs_[3*numTransition + i] is its globalMean
  //blobs_[4*numTransition + i] is its globalVar
  if (useBC) {
    this->blobs_.resize(10 * this->numTransition + 1);
  }
  else {
    this->blobs_.resize(5 * this->numTransition + 1);
  }
  for (int transitionIdx = 0; transitionIdx < this->numTransition; ++transitionIdx) {
    //filter
    //No BC case
    if (!useBC) {
      int inChannels = initChannel + transitionIdx * growthRate;
      int filterShape_Arr[] = { growthRate,inChannels,3,3 };
      vector<int> filterShape(filterShape_Arr, filterShape_Arr + 4);
      this->blobs_[transitionIdx]->Reshape(filterShape);
      this->blobs_[transitionIdx]->Fill(&Filter_Filler_);
    }
    else {
      //3*3 kernel
      int filter_33_shapeArr[] = { growthRate,4 * growthRate,3,3 };
      vector<int> filter33Shape(filter_33_shapeArr, filter_33_shapeArr + 4);
      this->blobs_[transitionIdx]->Reshape((filter33Shape));
      this->blobs_[transitionIdx]->Fill(&Filter_Filler_);

      //1*1 kernel
      int inChannels = initChannel + transitionIdx * growthRate;
      int filter_11_shapeArr[] = { 4 * growthRate,inChannels,1,1 };
      vector<int> filter11Shape(filter_11_shapeArr, filter_11_shapeArr + 4);
      this->blobs_[5 * numTransition + transitionIdx]->Reshape((filter11Shape));
      this->blobs_[5 * numTransition + transitionIdx]->Fill(&Filter_Filler_);
    }
    //scaler & bias
    int inChannels = initChannel + transitionIdx * growthRate;
    int BNparamShape_Arr[] = { 1,inChannels,1,1 };
    vector<int> BNparamShape(BNparamShape_Arr, BNparamShape_Arr + 4);
    //scaler
    this->blobs_[numTransition + transitionIdx]->Reshape((BNparamShape));
    this->blobs_[numTransition + transitionIdx]->Fill(&BN_Scaler_Filler_);

    int BN_4G_Shape[] = { 1,4 * growthRate,1,1 };
    vector<int> BN_4Gparam_ShapeVec(BN_4G_Shape, BN_4G_Shape + 4);
    //scaler BC
    if (useBC) {
      this->blobs_[6 * numTransition + transitionIdx]->Reshape((BN_4Gparam_ShapeVec));
      this->blobs_[6 * numTransition + transitionIdx]->Fill(&BN_Scaler_Filler_);
    }
    //bias
    this->blobs_[2 * numTransition + transitionIdx]->Reshape((BNparamShape));
    this->blobs_[2 * numTransition + transitionIdx]->Fill(&BN_Bias_Filler_);
    //bias BC
    if (useBC) {
      this->blobs_[7 * numTransition + transitionIdx]->Reshape((BN_4Gparam_ShapeVec));
      this->blobs_[7 * numTransition + transitionIdx]->Fill(&BN_Bias_Filler_);
    }
    //globalMean
    this->blobs_[3 * numTransition + transitionIdx]->Reshape((BNparamShape));
    for (int blobIdx = 0; blobIdx < inChannels; ++blobIdx) {
      Blob* localB = this->blobs_[3 * numTransition + transitionIdx];
      localB->cpu_mdata()[localB->offset(0, blobIdx, 0, 0)] = 0;
    }
    //globalMean BC
    if (useBC) {
      this->blobs_[8 * numTransition + transitionIdx]->Reshape((BN_4Gparam_ShapeVec));
      Blob* localB = this->blobs_[8 * numTransition + transitionIdx];
      for (int blobIdx = 0; blobIdx < 4 * growthRate; ++blobIdx) {
        localB->cpu_mdata()[localB->offset(0, blobIdx, 0, 0)] = 0;
      }
    }
    //globalVar
    this->blobs_[4 * numTransition + transitionIdx]->Reshape((BNparamShape));
    for (int blobIdx = 0; blobIdx < inChannels; ++blobIdx) {
      Blob* localB = this->blobs_[4 * numTransition + transitionIdx];
      localB->cpu_mdata()[localB->offset(0, blobIdx, 0, 0)] = 1;
    }
    //globalVar BC
    if (useBC) {
      this->blobs_[9 * numTransition + transitionIdx]->Reshape((BN_4Gparam_ShapeVec));
      Blob* localB = this->blobs_[9 * numTransition + transitionIdx];
      for (int blobIdx = 0; blobIdx < 4 * growthRate; ++blobIdx) {
        localB->cpu_mdata()[localB->offset(0, blobIdx, 0, 0)] = 1;
      }
    }
  }
  //final parameter for the equivalent of blobs_[2] in Caffe-BN
  vector<int> singletonShapeVec;
  singletonShapeVec.push_back(1);
  int singletonIdx = useBC ? 10 * numTransition : 5 * numTransition;
  this->blobs_[singletonIdx]->Reshape((singletonShapeVec));
  this->blobs_[singletonIdx]->cpu_mdata()[0] = Dtype(0);
  //parameter specification: globalMean/Var weight decay and lr is 0
  if (!useBC) {
    for (int i = 0; i < this->blobs_.size(); ++i) {
      //if (this->layer_param_.param_size() != i) {					CHECK_EQ(0, 1) << "Nope";				}
      //ParamSpec* fixed_param_spec = this->layer_param_.add_param();
      Blob* fixed_param_spec = blobs_[i];
      //global Mean/Var
      if (i >= 3 * this->numTransition) {
        fixed_param_spec->set_lr_mult(0.f);
        fixed_param_spec->set_decay_mult(0.f);
      }
      //BN Scaler and Bias
      else if (i >= this->numTransition) {
        fixed_param_spec->set_lr_mult(1.f);
        fixed_param_spec->set_decay_mult(1.f);
      }
      else {
        fixed_param_spec->set_lr_mult(1.f);
        fixed_param_spec->set_decay_mult(1.f);
      }
    }
  }
  else {
    for (int i = 0; i < this->blobs_.size(); ++i) {
      //if (this->layer_param_.param_size() != i) {					CHECK_EQ(0, 1)						<< "Nope";				}
      //ParamSpec* fixed_param_spec = this->layer_param_.add_param();
      Blob* fixed_param_spec = blobs_[i];
      if ((i >= 3 * numTransition) && (i < 5 * numTransition)) {
        fixed_param_spec->set_lr_mult(0.f);
        fixed_param_spec->set_decay_mult(0.f);
      }
      else if (i >= 8 * numTransition) {
        fixed_param_spec->set_lr_mult(0.f);
        fixed_param_spec->set_decay_mult(0.f);
      }
      else {
        fixed_param_spec->set_lr_mult(1.f);
        fixed_param_spec->set_decay_mult(1.f);
      }
    }
  }

#ifndef CPU_ONLY
  GPU_Initialization();
#endif
}

void DenseBlockLayer::Reshape(const vector<Blob*>& bottom, const vector<Blob*>& top) {
  int batch_size = bottom[0]->shape()[0];
  int h = bottom[0]->shape()[2];
  int w = bottom[0]->shape()[3];

#ifndef CPU_ONLY
  reshape_gpu_data(this->H, this->W, this->N, h, w, batch_size);
#endif
  this->N = batch_size;
  this->H = h;
  this->W = w;
  int topShapeArr[] = { this->N, this->initChannel + this->numTransition*this->growthRate,this->H,this->W };
  vector<int> topShape(topShapeArr, topShapeArr + 4);
  top[0]->Reshape(topShape);
}

void DenseBlockLayer::syncBlobs(DenseBlockLayer* originLayer) {
  vector<Blob* >& originBlobs = originLayer->blobs_;
  for (int blobIdx = 0; blobIdx < originBlobs.size(); ++blobIdx) {
    Blob* localBlob = originBlobs[blobIdx];
    Blob* newBlob = this->blobs_[blobIdx];
    newBlob->CopyFrom(localBlob, false);
  }
}


void DenseBlockLayer::setLogId(int uid) {
  this->logId = uid;
}

void DenseBlockLayer::logInternal_cpu(string dir) {
  string localDir = dir + "/cpu_" + itos(this->logId) + "/";
  //batch_Mean
  for (int i = 0; i < this->batch_Mean.size(); ++i) {
    string blobStr = localDir + "batch_Mean_" + itos(i);
    logBlob(this->batch_Mean[i], blobStr);
  }
  //batch_Var
  for (int i = 0; i < this->batch_Var.size(); ++i) {
    string blobStr = localDir + "batch_Var_" + itos(i);
    logBlob(this->batch_Var[i], blobStr);
  }
  if (useBC) {
    //batch_Mean
    for (int i = 0; i < this->batch_Mean4G.size(); ++i) {
      string blobStr = localDir + "batch_Mean_BC_" + itos(i);
      logBlob(this->batch_Mean4G[i], blobStr);
    }
    //batch_Var
    for (int i = 0; i < this->batch_Var4G.size(); ++i) {
      string blobStr = localDir + "batch_Var_BC_" + itos(i);
      logBlob(this->batch_Var4G[i], blobStr);
    }
  }
  //merged_conv
  for (int i = 0; i < this->merged_conv.size(); ++i) {
    string blobStr = localDir + "merged_conv_" + itos(i);
    logBlob(this->merged_conv[i], blobStr);
  }
  //BN_XhatVec
  for (int i = 0; i < this->BN_XhatVec.size(); ++i) {
    string blobStr = localDir + "BN_XhatVec_" + itos(i);
    logBlob(this->BN_XhatVec[i], blobStr);
  }
  //postBN_blobVec
  for (int i = 0; i < this->postBN_blobVec.size(); ++i) {
    string blobStr = localDir + "postBN_blobVec_" + itos(i);
    logBlob(this->postBN_blobVec[i], blobStr);
  }
  //postReLU_blobVec
  for (int i = 0; i < this->postReLU_blobVec.size(); ++i) {
    string blobStr = localDir + "postReLU_blobVec_" + itos(i);
    logBlob(this->postReLU_blobVec[i], blobStr);
  }
  //postConv_blobVec
  for (int i = 0; i < this->postConv_blobVec.size(); ++i) {
    string blobStr = localDir + "postConv_blobVec_" + itos(i);
    logBlob(this->postConv_blobVec[i], blobStr);
  }
  if (useBC) {
    //BC_BN_XhatVec
    for (int i = 0; i < this->BC_BN_XhatVec.size(); ++i) {
      string blobStr = localDir + "BC_BN_XhatVec_" + itos(i);
      logBlob(this->BC_BN_XhatVec[i], blobStr);
    }
    //postBN_BCVec
    for (int i = 0; i < this->postBN_BCVec.size(); ++i) {
      string blobStr = localDir + "postBN_BCVec_" + itos(i);
      logBlob(this->postBN_BCVec[i], blobStr);
    }
    //postReLU_BCVec
    for (int i = 0; i < this->postReLU_BCVec.size(); ++i) {
      string blobStr = localDir + "postReLU_BCVec_" + itos(i);
      logBlob(this->postReLU_BCVec[i], blobStr);
    }
    //postConv_BCVec
    for (int i = 0; i < this->postConv_BCVec.size(); ++i) {
      string blobStr = localDir + "postConv_BCVec_" + itos(i);
      logBlob(this->postConv_BCVec[i], blobStr);
    }
  }
  //filter
  for (int i = 0; i < this->numTransition; ++i) {
    string blobStr = localDir + "filter_" + itos(i);
    logBlob(this->blobs_[i], blobStr);
  }
  //scaler 
  for (int i = 0; i < this->numTransition; ++i) {
    string blobStr = localDir + "scaler_" + itos(i);
    logBlob(this->blobs_[this->numTransition + i], blobStr);
  }
  //bias
  for (int i = 0; i < this->numTransition; ++i) {
    string blobStr = localDir + "bias_" + itos(i);
    logBlob(this->blobs_[this->numTransition * 2 + i], blobStr);
  }
  if (useBC) {
    //filter
    for (int i = 0; i < this->numTransition; ++i) {
      string blobStr = localDir + "filter_BC_" + itos(i);
      logBlob(this->blobs_[5 * numTransition + i], blobStr);
    }
    //scaler 
    for (int i = 0; i < this->numTransition; ++i) {
      string blobStr = localDir + "scaler_BC_" + itos(i);
      logBlob(this->blobs_[6 * numTransition + i], blobStr);
    }
    //bias
    for (int i = 0; i < this->numTransition; ++i) {
      string blobStr = localDir + "bias_BC_" + itos(i);
      logBlob(this->blobs_[7 * numTransition + i], blobStr);
    }
    //Mean
    for (int i = 0; i < this->numTransition; ++i) {
      string blobStr = localDir + "Mean_BC_" + itos(i);
      logBlob(this->blobs_[8 * numTransition + i], blobStr);
    }
    //Var
    for (int i = 0; i < this->numTransition; ++i) {
      string blobStr = localDir + "Var_BC_" + itos(i);
      logBlob(this->blobs_[9 * numTransition + i], blobStr);
    }
  }
}


Dtype getZeroPaddedValue(bool isDiff, Blob* inputData, int n, int c, int h, int w) {
  int n_blob = inputData->shape(0);
  int c_blob = inputData->shape(1);
  int h_blob = inputData->shape(2);
  int w_blob = inputData->shape(3);
  if ((n < 0) || (n >= n_blob)) return 0;
  if ((c < 0) || (c >= c_blob)) return 0;
  if ((h < 0) || (h >= h_blob)) return 0;
  if ((w < 0) || (w >= w_blob)) return 0;
  if (isDiff) return inputData->diff_at(n, c, h, w);
  else return inputData->data_at(n, c, h, w);
}

//Assumption, h_filter and w_filter must be 3 for now
//naivest possible implementation of convolution, CPU forward and backward should not be used in production.
//CPU version of convolution assume img H,W does not change after convolution, which corresponds to denseBlock without BC
//input of size N*c_input*h_img*w_img

void convolution_Fwd(Blob* input, Blob* output, Blob* filter, int N, int c_output, int c_input, int h_img, int w_img, int h_filter, int w_filter) {
  int outputShape[] = { N,c_output,h_img,w_img };
  vector<int> outputShapeVec(outputShape, outputShape + 4);
  output->Reshape(outputShapeVec);
  Dtype * outputPtr = output->cpu_mdata();
  for (int n = 0; n < N; ++n) {
    for (int c_outIdx = 0; c_outIdx < c_output; ++c_outIdx) {
      for (int hIdx = 0; hIdx < h_img; ++hIdx) {
        for (int wIdx = 0; wIdx < w_img; ++wIdx) {
          outputPtr[output->offset(n, c_outIdx, hIdx, wIdx)] = 0;
          for (int c_inIdx = 0; c_inIdx < c_input; ++c_inIdx) {
            for (int filter_x = 0; filter_x < h_filter; ++filter_x) {
              for (int filter_y = 0; filter_y < w_filter; ++filter_y) {
                int localX = hIdx + (h_filter / 2) - filter_x;
                int localY = wIdx + (w_filter / 2) - filter_y;
                outputPtr[output->offset(n, c_outIdx, hIdx, wIdx)] += (filter->data_at(c_outIdx, c_inIdx, filter_x, filter_y) * getZeroPaddedValue(false, input, n, c_inIdx, localX, localY));
              }
            }
          }
        }
      }
    }
  }
}

//beta = 1 Convolution for bottomDiff

void convolution_Bwd(Blob* bottom, Blob* top, Blob* filter, int N, int c_output, int c_input, int h_img, int w_img, int h_filter, int w_filter) {
  Dtype * filterDiffPtr = filter->cpu_mdiff();
  Dtype * bottomDiffPtr = bottom->cpu_mdiff();
  //compute FilterGrad
  for (int coutIdx = 0; coutIdx < c_output; ++coutIdx) {
    for (int cinIdx = 0; cinIdx < c_input; ++cinIdx) {
      for (int filter_x = 0; filter_x < h_filter; ++filter_x) {
        for (int filter_y = 0; filter_y < w_filter; ++filter_y) {
          Dtype localGradSum = 0;
          for (int n = 0; n < N; ++n) {
            for (int i_img = 0; i_img < h_img; ++i_img) {
              for (int j_img = 0; j_img < w_img; ++j_img) {
                int localX = i_img + (h_filter / 2) - filter_x;
                int localY = j_img + (w_filter / 2) - filter_y;
                localGradSum += top->diff_at(n, coutIdx, i_img, j_img) * getZeroPaddedValue(false, bottom, n, cinIdx, localX, localY);
              }
            }
          }
          filterDiffPtr[filter->offset(coutIdx, cinIdx, filter_x, filter_y)] = localGradSum;
        }
      }
    }
  }
  //compute BottomGrad
  for (int n = 0; n < N; ++n) {
    for (int cinIdx = 0; cinIdx < c_input; ++cinIdx) {
      for (int i_img = 0; i_img < h_img; ++i_img) {
        for (int j_img = 0; j_img < w_img; ++j_img) {
          Dtype localGradSum = 0;
          for (int coutIdx = 0; coutIdx < c_output; ++coutIdx) {
            for (int x_img = 0; x_img < h_img; ++x_img) {
              for (int y_img = 0; y_img < w_img; ++y_img) {
                int localX = x_img - i_img + (h_filter / 2);
                int localY = y_img - j_img + (w_filter / 2);
                localGradSum += top->diff_at(n, coutIdx, x_img, y_img) * getZeroPaddedValue(false, filter, coutIdx, cinIdx, localX, localY);
              }
            }
          }
          bottomDiffPtr[bottom->offset(n, cinIdx, i_img, j_img)] = localGradSum;
        }
      }
    }
  }
}


void ReLU_Fwd(Blob* bottom, Blob* top, int N, int C, int h_img, int w_img) {
  //Reshape top
  int topShapeArr[] = { N,C,h_img,w_img };
  vector<int> topShapeVec(topShapeArr, topShapeArr + 4);
  top->Reshape(topShapeVec);
  //ReLU Fwd
  Dtype* topPtr = top->cpu_mdata();
  for (int n = 0; n < N; ++n) {
    for (int cIdx = 0; cIdx < C; ++cIdx) {
      for (int hIdx = 0; hIdx < h_img; ++hIdx) {
        for (int wIdx = 0; wIdx < w_img; ++wIdx) {
          Dtype bottomData = bottom->data_at(n, cIdx, hIdx, wIdx);
          topPtr[top->offset(n, cIdx, hIdx, wIdx)] = bottomData >= 0 ? bottomData : 0;
        }
      }
    }
  }
}


void ReLU_Bwd(Blob* bottom, Blob* top, int N, int C, int h_img, int w_img) {
  Dtype* bottomDiffPtr = bottom->cpu_mdiff();
  for (int n = 0; n < N; ++n) {
    for (int cIdx = 0; cIdx < C; ++cIdx) {
      for (int hIdx = 0; hIdx < h_img; ++hIdx) {
        for (int wIdx = 0; wIdx < w_img; ++wIdx) {
          bottomDiffPtr[bottom->offset(n, cIdx, hIdx, wIdx)] = bottom->data_at(n, cIdx, hIdx, wIdx) >= 0 ? top->diff_at(n, cIdx, hIdx, wIdx) : 0;
        }
      }
    }
  }
}


Dtype getMean(Blob* A, int channelIdx) {
  int N = A->shape(0);
  int H = A->shape(2);
  int W = A->shape(3);
  int totalCount = N*H*W;

  Dtype sum = 0;
  for (int n = 0; n < N; ++n) {
    for (int h = 0; h < H; ++h) {
      for (int w = 0; w < W; ++w) {
        sum += A->data_at(n, channelIdx, h, w);
      }
    }
  }
  return sum / totalCount;
}


Dtype getVar(Blob* A, int channelIdx) {
  int N = A->shape(0);
  int H = A->shape(2);
  int W = A->shape(3);
  int totalCount = N*H*W;
  Dtype mean = getMean(A, channelIdx);

  Dtype sum = 0;
  for (int n = 0; n < N; ++n) {
    for (int h = 0; h < H; ++h) {
      for (int w = 0; w < W; ++w) {
        sum += (A->data_at(n, channelIdx, h, w) - mean) * (A->data_at(n, channelIdx, h, w) - mean);
      }
    }
  }
  return sum / totalCount;
}


void BN_inf_Fwd(Blob* input, Blob* output, int N, int C, int h_img, int w_img, Blob* globalMean, Blob* globalVar, Blob* scaler, Blob* bias, Blob* factor_b) {
  int channelShape[] = { 1,C,1,1 };
  vector<int> channelShapeVec(channelShape, channelShape + 4);
  Blob* localInf_Mean = new Blob(channelShapeVec);
  Blob* localInf_Var = new Blob(channelShapeVec);
  Dtype scale_factor = factor_b->cpu_data()[0] == 0 ? 0 : (1 / factor_b->cpu_data()[0]);
  cpu_caffe_scale(localInf_Mean->count(), scale_factor, globalMean->cpu_data(), localInf_Mean->cpu_mdata());
  cpu_caffe_scale(localInf_Var->count(), scale_factor, globalVar->cpu_data(), localInf_Var->cpu_mdata());
  //Reshape output
  int outputShape[] = { N,C,h_img,w_img };
  vector<int> outputShapeVec(outputShape, outputShape + 4);
  output->Reshape(outputShapeVec);
  //BN Fwd inf
  double epsilon = 1e-5;
  Dtype* outputPtr = output->cpu_mdata();

  for (int n = 0; n < N; ++n) {
    for (int cIdx = 0; cIdx < C; ++cIdx) {
      Dtype denom = 1.0 / sqrt(localInf_Var->data_at(0, cIdx, 0, 0) + epsilon);
      for (int hIdx = 0; hIdx < h_img; ++hIdx) {
        for (int wIdx = 0; wIdx < w_img; ++wIdx) {
          outputPtr[output->offset(n, cIdx, hIdx, wIdx)] = scaler->data_at(0, cIdx, 0, 0) * (denom * (input->data_at(n, cIdx, hIdx, wIdx) - localInf_Mean->data_at(0, cIdx, 0, 0))) + bias->data_at(0, cIdx, 0, 0);
        }
      }
    }
  }
}


void BN_train_Fwd(Blob* bottom, Blob* top, Blob* output_xhat, Blob* globalMean, Blob* globalVar, Blob* batchMean, Blob* batchVar, Blob* scaler, Blob* bias, int N, int C, int h_img, int w_img, Dtype EMA_decay) {
  //reshape output
  int outputShape[] = { N,C,h_img,w_img };
  vector<int> outputShapeVec(outputShape, outputShape + 4);
  top->Reshape(outputShapeVec);
  output_xhat->Reshape(outputShapeVec);
  //BN Fwd train
  double epsilon = 1e-5;
  //get batch/global Mean/Var
  for (int channelIdx = 0; channelIdx < C; ++channelIdx) {
    int variance_adjust_m = N*h_img*w_img;
    //batch
    Dtype* batchMean_mutable = batchMean->cpu_mdata();
    Dtype* batchVar_mutable = batchVar->cpu_mdata();
    batchMean_mutable[channelIdx] = getMean(bottom, channelIdx);
    batchVar_mutable[channelIdx] = (variance_adjust_m / (variance_adjust_m - 1.0)) * getVar(bottom, channelIdx);
    //global
    Dtype* globalMean_mutable = globalMean->cpu_mdata();
    Dtype* globalVar_mutable = globalVar->cpu_mdata();
    globalMean_mutable[channelIdx] = EMA_decay * globalMean->data_at(0, channelIdx, 0, 0) + batchMean->data_at(0, channelIdx, 0, 0);
    globalVar_mutable[channelIdx] = EMA_decay * globalVar->data_at(0, channelIdx, 0, 0) + batchVar->data_at(0, channelIdx, 0, 0);
  }
  //process data
  for (int n = 0; n < N; ++n) {
    for (int c = 0; c < C; ++c) {
      for (int h = 0; h < h_img; ++h) {
        for (int w = 0; w < w_img; ++w) {
          Dtype* xhat_mutable = output_xhat->cpu_mdata();
          xhat_mutable[output_xhat->offset(n, c, h, w)] = (bottom->data_at(n, c, h, w) - batchMean->data_at(0, c, 0, 0)) / sqrt(batchVar->data_at(0, c, 0, 0) + epsilon);
          Dtype* output_mutable = top->cpu_mdata();
          output_mutable[top->offset(n, c, h, w)] = (scaler->data_at(0, c, 0, 0)) * (output_xhat->data_at(n, c, h, w)) + bias->data_at(0, c, 0, 0);
        }
      }
    }
  }
}


bool decide_channelDiffAllZero(Blob* B, int channelIdx, int N, int C, int H, int W) {
  bool output = true;
  for (int n = 0; n < N; ++n) {
    for (int h = 0; h < H; ++h) {
      for (int w = 0; w < W; ++w) {
        output = output && (B->diff_at(n, channelIdx, h, w) < 0.001) && (B->diff_at(n, channelIdx, h, w) > -0.001);
      }
    }
  }
  return output;
}


void BN_train_Bwd(Blob* bottom, Blob* bottom_xhat, Blob* top, Blob* batchMean, Blob* batchVar, Blob* scaler, Blob* bias, int N, int C, int h_img, int w_img, bool betaOneData) {
  double epsilon = 1e-5;
  //bias and scaler grad
  Dtype* biasGrad = bias->cpu_mdiff();
  Dtype* scalerGrad = scaler->cpu_mdiff();
  for (int channelIdx = 0; channelIdx < C; ++channelIdx) {
    biasGrad[channelIdx] = 0;
    scalerGrad[channelIdx] = 0;
    for (int n = 0; n < N; ++n) {
      for (int hIdx = 0; hIdx < h_img; ++hIdx) {
        for (int wIdx = 0; wIdx < w_img; ++wIdx) {
          biasGrad[channelIdx] += top->diff_at(n, channelIdx, hIdx, wIdx);
          scalerGrad[channelIdx] += top->diff_at(n, channelIdx, hIdx, wIdx) * bottom_xhat->data_at(n, channelIdx, hIdx, wIdx);
        }
      }
    }
  }
  //bottom data grad
  //helper 1:
  Dtype* XhatGrad = bottom_xhat->cpu_mdiff();
  for (int n = 0; n < N; ++n) {
    for (int c = 0; c < C; ++c) {
      for (int h = 0; h < h_img; ++h) {
        for (int w = 0; w < w_img; ++w) {
          XhatGrad[bottom_xhat->offset(n, c, h, w)] = top->diff_at(n, c, h, w) * scaler->data_at(0, c, 0, 0);
        }
      }
    }
  }
  //helper 2:
  Dtype* varGrad = batchVar->cpu_mdiff();
  for (int c = 0; c < C; ++c) {
    for (int n = 0; n < N; ++n) {
      for (int h = 0; h < h_img; ++h) {
        for (int w = 0; w < w_img; ++w) {
          //varGrad[c] += bottom_xhat->diff_at(n,c,h,w) * (bottom->data_at(n,c,h,w)-batchMean->data_at(0,c,0,0)) * (-0.5) * pow(batchVar->data_at(0,c,0,0) + epsilon,-1.5);
          varGrad[c] += bottom_xhat->diff_at(n, c, h, w) * (bottom->data_at(n, c, h, w) - batchMean->data_at(0, c, 0, 0)) * (-0.5) * (1.0 / ((batchVar->data_at(0, c, 0, 0) + epsilon) * sqrt(batchVar->data_at(0, c, 0, 0) + epsilon)));
          //flag
          //if (decide_channelDiffAllZero(top,c,N,C,h_img,w_img)){
          //  std::cout<<varGrad[c]<<std::endl;
          //}

        }
      }
    }
  }

  //helper 3:
  double m = N * h_img * w_img;
  Dtype* meanGrad = batchMean->cpu_mdiff();
  for (int c = 0; c < C; ++c) {
    for (int n = 0; n < N; ++n) {
      for (int h = 0; h < h_img; ++h) {
        for (int w = 0; w < w_img; ++w) {
          meanGrad[c] += bottom_xhat->diff_at(n, c, h, w) * (-1.0 / sqrt(batchVar->data_at(0, c, 0, 0) + epsilon)) + batchVar->diff_at(0, c, 0, 0) * (-2.0) * (bottom->data_at(n, c, h, w) - batchMean->data_at(0, c, 0, 0)) / m;
          //if (decide_channelDiffAllZero(top,c,N,C,h_img,w_img)){
          //  std::cout<<varGrad[c]<<std::endl;
          //}

        }
      }
    }
  }

  //combine helpers
  Dtype* bottomDataGrad = bottom->cpu_mdiff();
  for (int n = 0; n < N; ++n) {
    for (int c = 0; c < C; ++c) {
      for (int h = 0; h < h_img; ++h) {
        for (int w = 0; w < w_img; ++w) {
          //Dtype term1=bottom_xhat->diff_at(n,c,h,w)*pow(batchVar->data_at(0,c,0,0)+epsilon,-0.5);
          Dtype term1 = bottom_xhat->diff_at(n, c, h, w) / (sqrt(batchVar->data_at(0, c, 0, 0) + epsilon));
          Dtype term2 = batchVar->diff_at(0, c, 0, 0)*2.0*(bottom->data_at(n, c, h, w) - batchMean->data_at(0, c, 0, 0)) / m;
          Dtype term3 = batchMean->diff_at(0, c, 0, 0) / m;
          if (betaOneData) {
            bottomDataGrad[bottom->offset(n, c, h, w)] += term1 + term2 + term3;
          }
          else {
            bottomDataGrad[bottom->offset(n, c, h, w)] = term1 + term2 + term3;
          }
          //std::cout<<term1<<","<<term2<<","<<term3<<std::endl;
        }
      }
    }
  }

}



void DenseBlockLayer::CPU_Initialization() {
  this->batch_Mean.resize(this->numTransition);
  this->batch_Var.resize(this->numTransition);

  this->merged_conv.resize(this->numTransition + 1);
  this->BN_XhatVec.resize(this->numTransition);
  this->postBN_blobVec.resize(this->numTransition);
  this->postReLU_blobVec.resize(this->numTransition);
  this->postConv_blobVec.resize(this->numTransition);
  if (useBC) {
    BC_BN_XhatVec.resize(this->numTransition);
    postBN_BCVec.resize(this->numTransition);
    postReLU_BCVec.resize(this->numTransition);
    postConv_BCVec.resize(this->numTransition);
    batch_Mean4G.resize(numTransition);
    batch_Var4G.resize(numTransition);
  }
  for (int transitionIdx = 0; transitionIdx < this->numTransition; ++transitionIdx) {
    int conv_y_Channels = this->growthRate;
    int mergeChannels = this->initChannel + this->growthRate * transitionIdx;
    int channelShapeArr[] = { 1,mergeChannels,1,1 };
    int conv_y_ShapeArr[] = { this->N,conv_y_Channels,this->H,this->W };
    int mergeShapeArr[] = { this->N,mergeChannels,this->H,this->W };
    vector<int> channelShape(channelShapeArr, channelShapeArr + 4);
    vector<int> conv_y_Shape(conv_y_ShapeArr, conv_y_ShapeArr + 4);
    vector<int> mergeShape(mergeShapeArr, mergeShapeArr + 4);

    this->batch_Mean[transitionIdx] = new Blob(channelShape);
    this->batch_Var[transitionIdx] = new Blob(channelShape);

    this->merged_conv[transitionIdx] = new Blob(mergeShape);
    this->BN_XhatVec[transitionIdx] = new Blob(mergeShape);
    this->postBN_blobVec[transitionIdx] = new Blob(mergeShape);
    this->postReLU_blobVec[transitionIdx] = new Blob(mergeShape);
    this->postConv_blobVec[transitionIdx] = new Blob(conv_y_Shape);
    if (useBC) {
      int quadGShapeArr[] = { N,4 * growthRate,H,W };
      int quadChannelArr[] = { 1,4 * growthRate,1,1 };
      vector<int> quadGShape(quadGShapeArr, quadGShapeArr + 4);
      vector<int> quadChannelShape(quadChannelArr, quadChannelArr + 4);
      this->BC_BN_XhatVec[transitionIdx] = new Blob(quadGShape);
      this->postBN_BCVec[transitionIdx] = new Blob(quadGShape);
      this->postReLU_BCVec[transitionIdx] = new Blob(quadGShape);
      this->postConv_BCVec[transitionIdx] = new Blob(quadGShape);
      batch_Mean4G[transitionIdx] = new Blob(quadChannelShape);
      batch_Var4G[transitionIdx] = new Blob(quadChannelShape);
    }
  }
  //the last element of merged_conv serve as output of forward
  int extraMergeOutputShapeArr[] = { this->N,this->initChannel + this->growthRate*this->numTransition,this->H,this->W };
  vector<int> extraMergeOutputShapeVector(extraMergeOutputShapeArr, extraMergeOutputShapeArr + 4);
  this->merged_conv[this->numTransition] = new Blob(extraMergeOutputShapeVector);
}


void mergeChannelData(Blob* outputBlob, Blob* blobA, Blob* blobB) {
  int N = blobA->shape(0);
  int frontC = blobA->shape(1); int backC = blobB->shape(1);
  int H = blobA->shape(2);
  int W = blobA->shape(3);

  for (int n = 0; n < N; ++n) {
    for (int c = 0; c < frontC + backC; ++c) {
      for (int h = 0; h < H; ++h) {
        for (int w = 0; w < W; ++w) {
          Dtype inData;
          if (c < frontC) {
            inData = blobA->cpu_data()[blobA->offset(n, c, h, w)];
          }
          else {
            int readC = c - frontC;
            inData = blobB->cpu_data()[blobB->offset(n, readC, h, w)];
          }
          outputBlob->cpu_mdata()[outputBlob->offset(n, c, h, w)] = inData;
        }
      }
    }
  }
}


void distributeChannelDiff(Blob* inputBlob, Blob* blobA, Blob* blobB) {
  int N = blobA->shape(0);
  int frontC = blobA->shape(1); int backC = blobB->shape(1);
  int H = blobA->shape(2);
  int W = blobA->shape(3);

  for (int n = 0; n < N; ++n) {
    for (int c = 0; c < frontC + backC; ++c) {
      for (int h = 0; h < H; ++h) {
        for (int w = 0; w < W; ++w) {
          Dtype readData = inputBlob->cpu_diff()[inputBlob->offset(n, c, h, w)];
          if (c < frontC) {
            blobA->cpu_mdiff()[blobA->offset(n, c, h, w)] = readData;
          }
          else {
            int writeC = c - frontC;
            blobB->cpu_mdiff()[blobB->offset(n, writeC, h, w)] = readData;
          }
        }
      }
    }
  }
}


void BlobSetZero(Blob* B, int count) {
  Dtype* B_mutable_data = B->cpu_mdata();
  Dtype* B_mutable_diff = B->cpu_mdiff();
  for (int i = 0; i < count; ++i) {
    B_mutable_data[i] = 0;
    B_mutable_diff[i] = 0;
  }
}


void DenseBlockLayer::LoopEndCleanup_cpu() {
  for (int transitionIdx = 0; transitionIdx < this->numTransition; ++transitionIdx) {
    int tensorCount = this->N * growthRate * this->H * this->W;
    int tensorMergeCount = this->N * (this->initChannel + this->growthRate * transitionIdx) * this->H * this->W;
    BlobSetZero(this->merged_conv[transitionIdx], tensorMergeCount);
    BlobSetZero(this->BN_XhatVec[transitionIdx], tensorMergeCount);
    BlobSetZero(this->postBN_blobVec[transitionIdx], tensorMergeCount);
    BlobSetZero(this->postReLU_blobVec[transitionIdx], tensorMergeCount);
    BlobSetZero(this->postConv_blobVec[transitionIdx], tensorCount);
  }
}


void DenseBlockLayer::Forward_cpu(const vector<Blob*>& bottom,
  const vector<Blob*>& top)
{
  //init CPU
  if (!this->cpuInited) {
    //std::cout<<"fwd cpu init"<<std::endl;
    this->CPU_Initialization();
    this->cpuInited = true;
    //std::cout<<"fwd cpu init done"<<std::endl;
  }
  int bnTimerIdx = useBC ? 10 * numTransition : 5 * numTransition;
  //deploy init data
  this->merged_conv[0]->CopyFrom(bottom[0]);
  //init CPU finish
  for (int transitionIdx = 0; transitionIdx < this->numTransition; ++transitionIdx) {
    //BN
    Blob* BN_bottom = this->merged_conv[transitionIdx];
    Blob* BN_top = this->postBN_blobVec[transitionIdx];
    Blob* Scaler = this->blobs_[numTransition + transitionIdx];
    Blob* Bias = this->blobs_[2 * numTransition + transitionIdx];
    int localChannels = this->initChannel + transitionIdx*this->growthRate;
    if (this->phase_cur_ == TEST) {
      //std::cout<<"cpu BN test forward"<<std::endl;
      BN_inf_Fwd(BN_bottom, BN_top, this->N, localChannels, this->H, this->W, this->blobs_[3 * this->numTransition + transitionIdx], this->blobs_[4 * this->numTransition + transitionIdx], Scaler, Bias, this->blobs_[bnTimerIdx]);
    }
    else {
      //std::cout<<"cpu BN train forward"<<std::endl;
      BN_train_Fwd(BN_bottom, BN_top, this->BN_XhatVec[transitionIdx], this->blobs_[3 * this->numTransition + transitionIdx], this->blobs_[4 * this->numTransition + transitionIdx], this->batch_Mean[transitionIdx], this->batch_Var[transitionIdx], Scaler, Bias, this->N, localChannels, this->H, this->W, this->EMA_decay);
    }
    //ReLU
    Blob* ReLU_top = this->postReLU_blobVec[transitionIdx];
    ReLU_Fwd(BN_top, ReLU_top, this->N, localChannels, this->H, this->W);
    //if useBC, Conv1*1-BN(BC)-ReLU(BC)
    if (useBC) {
      //BC Conv 1*1
      Blob* BC_filterBlob = this->blobs_[5 * numTransition + transitionIdx];
      Blob* BC_conv_x = postReLU_blobVec[transitionIdx];
      Blob* BC_conv_y = postConv_BCVec[transitionIdx];
      int BC_conv_inChannel = initChannel + growthRate*transitionIdx;
      int BC_conv_outChannel = 4 * growthRate;
      convolution_Fwd(BC_conv_x, BC_conv_y, BC_filterBlob, N, BC_conv_outChannel, BC_conv_inChannel, H, W, 1, 1);
      //BC BN 
      Blob* BC_BN_x = postConv_BCVec[transitionIdx];
      Blob* BC_BN_y = postBN_BCVec[transitionIdx];
      Blob* BC_Scaler = this->blobs_[6 * numTransition + transitionIdx];
      Blob* BC_Bias = this->blobs_[7 * numTransition + transitionIdx];
      Blob* BC_Mean = this->blobs_[8 * numTransition + transitionIdx];
      Blob* BC_Var = this->blobs_[9 * numTransition + transitionIdx];
      if (this->phase_cur_ == TEST) {
        BN_inf_Fwd(BC_BN_x, BC_BN_y, N, 4 * growthRate, H, W, BC_Mean, BC_Var, BC_Scaler, BC_Bias, this->blobs_[bnTimerIdx]);
      }
      else {
        Blob* BC_xhat = BC_BN_XhatVec[transitionIdx];
        Blob* BC_batchMean = batch_Mean4G[transitionIdx];
        Blob* BC_batchVar = batch_Var4G[transitionIdx];
        BN_train_Fwd(BC_BN_x, BC_BN_y, BC_xhat, BC_Mean, BC_Var, BC_batchMean, BC_batchVar, BC_Scaler, BC_Bias, N, 4 * growthRate, H, W, EMA_decay);
      }
      //BC ReLU 
      Blob* ReLU_x = postBN_BCVec[transitionIdx];
      Blob* ReLU_y = postReLU_BCVec[transitionIdx];
      ReLU_Fwd(ReLU_x, ReLU_y, N, 4 * growthRate, H, W);
    }
    //Conv
    Blob* filterBlob = this->blobs_[transitionIdx];
    Blob* conv_x = useBC ? postReLU_BCVec[transitionIdx] : postReLU_blobVec[transitionIdx];
    Blob* conv_y = this->postConv_blobVec[transitionIdx];
    int inConvChannel = useBC ? 4 * growthRate : initChannel + growthRate*transitionIdx;
    convolution_Fwd(conv_x, conv_y, filterBlob, N, growthRate, inConvChannel, H, W, 3, 3);
    //post Conv merge
    Blob* mergeOutput = merged_conv[transitionIdx + 1];
    Blob* mergeInputA = merged_conv[transitionIdx];
    Blob* mergeInputB = postConv_blobVec[transitionIdx];
    mergeChannelData(mergeOutput, mergeInputA, mergeInputB);
  }
  //deploy output data
  top[0]->CopyFrom((this->merged_conv[this->numTransition]));
  if (this->phase_cur_ == TRAIN) {
    this->blobs_[bnTimerIdx]->cpu_mdata()[0] *= this->EMA_decay;
    this->blobs_[bnTimerIdx]->cpu_mdata()[0] += 1;
    this->trainCycleIdx += 1;
  }
  //logInternal_cpu("TC_TrueFwdlog");
}



void DenseBlockLayer::Backward_cpu(const vector<Blob*>& top,
  const vector<bool>& propagate_down,
  const vector<Blob*>& bottom)
{
  if (!this->cpuInited) {
    this->CPU_Initialization();
    this->cpuInited = true;
  }
  //deploy top diff
  this->merged_conv[this->numTransition]->CopyFrom((top[0]), true);
  for (int transitionIdx = this->numTransition - 1; transitionIdx >= 0; --transitionIdx) {
    //distribute diff
    distributeChannelDiff(this->merged_conv[transitionIdx + 1], this->merged_conv[transitionIdx], this->postConv_blobVec[transitionIdx]);
    //Conv Bwd
    Blob* conv_top = this->postConv_blobVec[transitionIdx];
    Blob* conv_bottom = useBC ? postReLU_BCVec[transitionIdx] : postReLU_blobVec[transitionIdx];
    Blob* filter = this->blobs_[transitionIdx];
    int c_input = useBC ? 4 * growthRate : initChannel + growthRate*transitionIdx;
    convolution_Bwd(conv_bottom, conv_top, filter, this->N, this->growthRate, c_input, this->H, this->W, 3, 3);
    //BC ReLU_BC_Bwd - BN_BC_Bwd - Conv1*1_BC_Bwd
    if (useBC) {
      //ReLU BC Bwd
      Blob* BC_ReLU_y = postReLU_BCVec[transitionIdx];
      Blob* BC_ReLU_x = postBN_BCVec[transitionIdx];
      ReLU_Bwd(BC_ReLU_x, BC_ReLU_y, N, 4 * growthRate, H, W);
      //BN BC Bwd
      Blob* BC_BN_y = postBN_BCVec[transitionIdx];
      Blob* BC_BN_x = postConv_BCVec[transitionIdx];
      Blob* BC_BN_xhat = BC_BN_XhatVec[transitionIdx];
      Blob* BC_Scaler = this->blobs_[6 * numTransition + transitionIdx];
      Blob* BC_Bias = this->blobs_[7 * numTransition + transitionIdx];
      Blob* BC_batchMean = batch_Mean4G[transitionIdx];
      Blob* BC_batchVar = batch_Var4G[transitionIdx];
      BN_train_Bwd(BC_BN_x, BC_BN_xhat, BC_BN_y, BC_batchMean, BC_batchVar, BC_Scaler, BC_Bias, N, 4 * growthRate, H, W, false);
      //Conv1*1 BC Bwd
      Blob* BC_conv_x = postReLU_blobVec[transitionIdx];
      Blob* BC_conv_y = postConv_BCVec[transitionIdx];
      Blob* BC_filter = this->blobs_[5 * numTransition + transitionIdx];
      int BC_c_input = initChannel + growthRate*transitionIdx;
      int BC_c_output = 4 * growthRate;
      convolution_Bwd(BC_conv_x, BC_conv_y, BC_filter, N, BC_c_output, BC_c_input, H, W, 1, 1);
    }
    //ReLU Bwd
    int localChannel = this->initChannel + this->growthRate*transitionIdx;
    ReLU_Bwd(postBN_blobVec[transitionIdx], postReLU_blobVec[transitionIdx], this->N, localChannel, this->H, this->W);
    //BN Bwd
    Blob* BN_bottom = this->merged_conv[transitionIdx];
    Blob* scaler = this->blobs_[this->numTransition + transitionIdx];
    Blob* bias = this->blobs_[2 * this->numTransition + transitionIdx];
    BN_train_Bwd(BN_bottom, this->BN_XhatVec[transitionIdx], this->postBN_blobVec[transitionIdx], this->batch_Mean[transitionIdx], this->batch_Var[transitionIdx], scaler, bias, this->N, localChannel, this->H, this->W, true);
  }
  bottom[0]->CopyFrom((this->merged_conv[0]), true);
  //logInternal_cpu("TC_TrueBwdlog");
  this->LoopEndCleanup_cpu();
}


void DenseBlockLayer::Forward_cpu_public(const vector<Blob*>& bottom, const vector<Blob*>& top) {
  this->Forward_cpu(bottom, top);
}


void DenseBlockLayer::Backward_cpu_public(const vector<Blob*>& top, const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  this->Backward_cpu(top, propagate_down, bottom);
}


INSTANTIATE_CLASS(DenseBlock, DenseBlockLayer);

#endif  // CAFFE_DENSEBLOCK_LAYER_HPP_

