#ifndef CAFFE_DENSEBLOCK_LAYER_HPP_
#define CAFFE_DENSEBLOCK_LAYER_HPP_


#include <string>
#include "wstd/string.hpp"
using namespace wstd;

#define DenseBlockParameter_DEF(DEF) \
DEF##Struct(Filter_Filler, 0, Filler) \
DEF##Struct(BN_Scaler_Filler, 0, Filler) \
DEF##Struct(BN_Bias_Filler, 0, Filler) \
DEF##Int(numtransition, 40, 0) \

class DenseBlockLayer : public Layer
{
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


  BLOB_DEF(BLOB_DEF1);
#undef BLOB_DEF1


  virtual void init() {
    DenseBlockParameter_DEF(Init);
  }
  virtual void toJson(cjson* param) {
    DenseBlockParameter_DEF(Set);
  }
  virtual void fromJson(cjson* param) {
    DenseBlockParameter_DEF(Get);
    BLOB_DEF(BLOB_NAME);

    this->growthRate = cjson_GetObjectInt(param, "growthrate", 12);
    this->EMA_decay = cjson_GetObjectFloat(param, "moving_average_fraction", 0.1);

    this->workspace_size_bytes = cjson_GetObjectInt(param, "workspace_mb", 8) * 1024 * 1024;
    this->gpu_idx_ = cjson_GetObjectInt(param, "gpuidx", 0);
    this->useDropout = cjson_GetObjectBool(param, "use_dropout", false);
    this->dropoutAmount = cjson_GetObjectFloat(param, "dropout_amount", 0);
    this->DB_randomSeed = 124816;
    this->useBC = cjson_GetObjectBool(param, "use_bc", false);
    this->BC_ultra_spaceEfficient = cjson_GetObjectBool(param, "bc_ultra_space_efficient", false);
  }

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
  ConvolutionLayer conv_FwdAlgo;

  Dtype* Mean_tmp;
  Dtype* Var_tmp;
  vector<Dtype*> ResultSaveMean_gpu;
  vector<Dtype*> ResultSaveInvVariance_gpu;

  int trainCycleIdx; //used in BN train phase for EMA Mean/Var estimation
                     //convolution Related
  int pad_h, pad_w, conv_verticalStride, conv_horizentalStride;
  int filter_H, filter_W;
  //Decay value used in EMA of BN
  double EMA_decay;

  //end GPU specific data setion

  int initChannel, growthRate;
  int N, H, W; //N,H,W of the input tensor, inited in reshape phase

  bool useDropout;
  double dropoutAmount;
  uint64 DB_randomSeed;
  bool useBC;
  bool BC_ultra_spaceEfficient;

  Dtype* workspace;
  Dtype* workspace2;
  //gpu workspace size
  int workspace_size_bytes;

  virtual inline const char* type() const { return "DenseBlock"; }

public:

  DenseBlockLayer() {
    DenseBlockParameter_DEF(Init);
  }

  virtual void LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top) {
    this->cpuInited = false;
    // #ifndef CPU_ONLY
    // 		this->gpuInited = false;
    // #endif
    this->N = bottom[0]->shape(0);
    this->H = bottom[0]->shape(2);
    this->W = bottom[0]->shape(3);
    //this->initChannel = param->initchannel();
    this->initChannel = bottom[0]->channels();//modified by jxs
    this->trainCycleIdx = 0; //initially, trainCycleIdx = 0
    //Parameter Blobs
    //for transition i,
    //blobs_[i] is its filter blob
    //blobs_[numtransition_ + i] is its scaler blob
    //blobs_[2*numtransition_ + i] is its bias blob
    //blobs_[3*numtransition_ + i] is its globalMean
    //blobs_[4*numtransition_ + i] is its globalVar
    int blobs_size = 0;
    if (useBC) {
      blobs_size = 10 * this->numtransition_ + 1;
    }
    else {
      blobs_size = 5 * this->numtransition_ + 1;
    }
    if (blobs_.size() < blobs_size) {
      blobs_reset(this->blobs_, blobs_size);
      Blob* localB;
      for (int transitionIdx = 0; transitionIdx < this->numtransition_; ++transitionIdx) {
        //filter
        //No BC case
        if (!useBC) {
          int inChannels = initChannel + transitionIdx * growthRate;
          int filterShape_Arr[] = { growthRate, inChannels, 3, 3 };
          vector<int> filterShape(filterShape_Arr, filterShape_Arr + 4);
          this->blobs_[transitionIdx]->Reshape((filterShape));
          this->blobs_[transitionIdx]->Fill(&Filter_Filler_);
        }
        else {
          //3*3 kernel
          int filter_33_shapeArr[] = { growthRate, 4 * growthRate, 3, 3 };
          vector<int> filter33Shape(filter_33_shapeArr, filter_33_shapeArr + 4);
          this->blobs_[transitionIdx]->Reshape((filter33Shape));
          this->blobs_[transitionIdx]->Fill(&Filter_Filler_);
          //1*1 kernel
          int inChannels = initChannel + transitionIdx * growthRate;
          int filter_11_shapeArr[] = { 4 * growthRate, inChannels, 1, 1 };
          vector<int> filter11Shape(filter_11_shapeArr, filter_11_shapeArr + 4);
          this->blobs_[5 * numtransition_ + transitionIdx]->Reshape((filter11Shape));
          this->blobs_[5 * numtransition_ + transitionIdx]->Fill(&Filter_Filler_);
        }
        //scaler & bias
        int inChannels = initChannel + transitionIdx * growthRate;
        int BNparamShape_Arr[] = { 1, inChannels, 1, 1 };
        vector<int> BNparamShape(BNparamShape_Arr, BNparamShape_Arr + 4);
        //scaler
        this->blobs_[numtransition_ + transitionIdx]->Reshape((BNparamShape));
        this->blobs_[numtransition_ + transitionIdx]->Fill(&BN_Scaler_Filler_);
        int BN_4G_Shape[] = { 1, 4 * growthRate, 1, 1 };
        vector<int> BN_4Gparam_ShapeVec(BN_4G_Shape, BN_4G_Shape + 4);
        //scaler BC
        if (useBC) {
          localB = this->blobs_[6 * numtransition_ + transitionIdx];
          localB->Reshape((BN_4Gparam_ShapeVec));
          localB->Fill(&BN_Scaler_Filler_);
        }
        //bias
        localB = this->blobs_[2 * numtransition_ + transitionIdx];
        localB->Reshape((BNparamShape));
        localB->Fill(&BN_Bias_Filler_);
        //bias BC
        if (useBC) {
          localB = this->blobs_[7 * numtransition_ + transitionIdx];
          localB->Reshape((BN_4Gparam_ShapeVec));
          localB->Fill(&BN_Bias_Filler_);
        }
        //globalMean
        localB = this->blobs_[3 * numtransition_ + transitionIdx];
        localB->Reshape((BNparamShape));
        localB->FillConstant(0);
        //globalMean BC
        if (useBC) {
          localB = this->blobs_[8 * numtransition_ + transitionIdx];
          localB->Reshape((BN_4Gparam_ShapeVec));
          localB->FillConstant(0);
        }
        //globalVar
        localB = this->blobs_[4 * numtransition_ + transitionIdx];
        localB->Reshape((BNparamShape));
        localB->FillConstant(1);
        //globalVar BC
        if (useBC) {
          localB  = this->blobs_[9 * numtransition_ + transitionIdx];
          localB->Reshape((BN_4Gparam_ShapeVec));
          localB->FillConstant(1);
        }
      }
      //final parameter for the equivalent of blobs_[2] in Caffe-BN
      vector<int> singletonShapeVec;
      singletonShapeVec.push_back(1);
      int singletonIdx = useBC ? 10 * numtransition_ : 5 * numtransition_;
      this->blobs_[singletonIdx]->Reshape((singletonShapeVec));
      this->blobs_[singletonIdx]->cpu_mdata()[0] = Dtype(0);
      //parameter specification: globalMean/Var weight decay and lr is 0
      if (!useBC) {
        for (int i = 0; i < this->blobs_.size(); ++i) {
          Blob* blob = blobs_[i];
          //global Mean/Var
          if (i >= 3 * this->numtransition_) {
            blob->set_lr_mult(0.f);
            blob->set_decay_mult(0.f);
          }
          //BN Scaler and Bias
          else if (i >= this->numtransition_) {
            blob->set_lr_mult(1.f);
            blob->set_decay_mult(1.f);
          }
          else {
            blob->set_lr_mult(1.f);
            blob->set_decay_mult(1.f);
          }
        }
      }
      else {
        for (int i = 0; i < this->blobs_.size(); ++i) {
          Blob* blob = blobs_[i];
          if ((i >= 3 * numtransition_) && (i < 5 * numtransition_)) {
            blob->set_lr_mult(0.f);
            blob->set_decay_mult(0.f);
          }
          else if (i >= 8 * numtransition_) {
            blob->set_lr_mult(0.f);
            blob->set_decay_mult(0.f);
          }
          else {
            blob->set_lr_mult(1.f);
            blob->set_decay_mult(1.f);
          }
        }
      }
    }
    CPU_Initialization();
    GPU_Initialization();
  }
  virtual void GPU_Initialization() {}
  virtual void reshape_data(int oldh, int oldw, int oldn, int h, int w, int newn) {
  }

  virtual void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    int batch_size = bottom[0]->shape(0);
    int h = bottom[0]->shape(2);
    int w = bottom[0]->shape(3);
    reshape_data(this->H, this->W, this->N, h, w, batch_size);
    this->N = batch_size;
    this->H = h;
    this->W = w;
    int topShapeArr[] = { this->N, this->initChannel + this->numtransition_* this->growthRate, this->H, this->W };
    vector<int> topShape(topShapeArr, topShapeArr + 4);
    top[0]->Reshape(topShape);
  }

  virtual void syncBlobs(DenseBlockLayer* originLayer)
  {
    vector<Blob*> & originBlobs = originLayer->blobs_;
    blobs_reset(blobs_, originBlobs.size());
    for (int blobIdx = 0; blobIdx < originBlobs.size(); ++blobIdx) {
      Blob* localBlob = originBlobs[blobIdx];
      Blob* newBlob = this->blobs_[blobIdx];
      newBlob->CopyFrom(localBlob, false);
    }
  }

  virtual void setLogId(int uid)
  {
    this->logId = uid;
  }


  virtual void logInternal_cpu(string dir)
  {
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
    for (int i = 0; i < this->numtransition_; ++i) {
      string blobStr = localDir + "filter_" + itos(i);
      logBlob(this->blobs_[i], blobStr);
    }
    //scaler
    for (int i = 0; i < this->numtransition_; ++i) {
      string blobStr = localDir + "scaler_" + itos(i);
      logBlob(this->blobs_[this->numtransition_ + i], blobStr);
    }
    //bias
    for (int i = 0; i < this->numtransition_; ++i) {
      string blobStr = localDir + "bias_" + itos(i);
      logBlob(this->blobs_[this->numtransition_ * 2 + i], blobStr);
    }
    if (useBC) {
      //filter
      for (int i = 0; i < this->numtransition_; ++i) {
        string blobStr = localDir + "filter_BC_" + itos(i);
        logBlob(this->blobs_[5 * numtransition_ + i], blobStr);
      }
      //scaler
      for (int i = 0; i < this->numtransition_; ++i) {
        string blobStr = localDir + "scaler_BC_" + itos(i);
        logBlob(this->blobs_[6 * numtransition_ + i], blobStr);
      }
      //bias
      for (int i = 0; i < this->numtransition_; ++i) {
        string blobStr = localDir + "bias_BC_" + itos(i);
        logBlob(this->blobs_[7 * numtransition_ + i], blobStr);
      }
      //Mean
      for (int i = 0; i < this->numtransition_; ++i) {
        string blobStr = localDir + "Mean_BC_" + itos(i);
        logBlob(this->blobs_[8 * numtransition_ + i], blobStr);
      }
      //Var
      for (int i = 0; i < this->numtransition_; ++i) {
        string blobStr = localDir + "Var_BC_" + itos(i);
        logBlob(this->blobs_[9 * numtransition_ + i], blobStr);
      }
    }
  }


public:

  void CPU_Initialization()
  {
    this->batch_Mean.resize(this->numtransition_);
    this->batch_Var.resize(this->numtransition_);
    this->merged_conv.resize(this->numtransition_ + 1);
    this->BN_XhatVec.resize(this->numtransition_);
    this->postBN_blobVec.resize(this->numtransition_);
    this->postReLU_blobVec.resize(this->numtransition_);
    this->postConv_blobVec.resize(this->numtransition_);
    if (useBC) {
      BC_BN_XhatVec.resize(this->numtransition_);
      postBN_BCVec.resize(this->numtransition_);
      postReLU_BCVec.resize(this->numtransition_);
      postConv_BCVec.resize(this->numtransition_);
      batch_Mean4G.resize(numtransition_);
      batch_Var4G.resize(numtransition_);
    }
    for (int transitionIdx = 0; transitionIdx < this->numtransition_; ++transitionIdx) {
      int conv_y_Channels = this->growthRate;
      int mergeChannels = this->initChannel + this->growthRate * transitionIdx;
      int channelShapeArr[] = { 1, mergeChannels, 1, 1 };
      int conv_y_ShapeArr[] = { this->N, conv_y_Channels, this->H, this->W };
      int mergeShapeArr[] = { this->N, mergeChannels, this->H, this->W };
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
        int quadGShapeArr[] = { N, 4 * growthRate, H, W };
        int quadChannelArr[] = { 1, 4 * growthRate, 1, 1 };
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
    int extraMergeOutputShapeArr[] = { this->N, this->initChannel + this->growthRate* this->numtransition_, this->H, this->W };
    vector<int> extraMergeOutputShapeVector(extraMergeOutputShapeArr, extraMergeOutputShapeArr + 4);
    this->merged_conv[this->numtransition_] = new Blob(extraMergeOutputShapeVector);



    DataShape shape = dataShape(N, (this->initChannel + this->growthRate*this->numtransition_), H, W);
    postConv.Reshape(shape);
    if (useDropout) {
      postDropout.Reshape(shape);
    }
    postBN.Reshape(shape);
    postReLU.Reshape(shape);

    DataShape quadG_shape = dataShape(N, 4 * growthRate, H, W);
    if (useBC) {
#if 1
      postBN_4G.Reshape(quadG_shape);
      postReLU_4G.Reshape(quadG_shape);
      postReLU_4G.Reshape(quadG_shape);
#endif
    }
    for (int i = 0; i < this->numtransition_; ++i) {
      //Result Running/Saving Mean/Variance/InvVariance
      int localChannel = this->initChannel + i * this->growthRate;
      Dtype* local_SaveMean;
      Dtype* local_SaveInvVar;

      caffe_Malloc(&local_SaveMean, localChannel * sizeof(Dtype));
      caffe_Malloc(&local_SaveInvVar, localChannel * sizeof(Dtype));

      caffe_Memset(local_SaveMean, 0, localChannel * sizeof(Dtype));
      caffe_Memset(local_SaveInvVar, 0, localChannel * sizeof(Dtype));

      this->ResultSaveMean_gpu.push_back(local_SaveMean);
      this->ResultSaveInvVariance_gpu.push_back(local_SaveInvVar);
    }

    //Mean and Var tmp
    int totalNumChannel = this->initChannel + this->growthRate * this->numtransition_;
    caffe_Malloc(&this->Mean_tmp, totalNumChannel * sizeof(Dtype));
    caffe_Malloc(&this->Var_tmp, totalNumChannel * sizeof(Dtype));
    cleanupBuffer(this->Mean_tmp, totalNumChannel);
    cleanupBuffer(this->Var_tmp, totalNumChannel);

  }




  void cleanupBuffer(Dtype* ptr_gpu, int count) {
    caffe_set(count, 0, ptr_gpu);
  }

  void copy_one_to_many(const Dtype* inPtr, Dtype* outPtr, int numChunks, int chunkSize_input, int chunkStride_output) {
    for (int chunkIdx = 0; chunkIdx < numChunks; ++chunkIdx) {
      const Dtype* inPtr_local = inPtr + chunkIdx*chunkSize_input;
      Dtype* outPtr_local = outPtr + chunkIdx*chunkStride_output;

      //printf("inpointer %p\n",inPtr);
      //printf("outpointer %p\n",outPtr);
      //CUDA_CHECK(cudaMemcpy(outPtr_local, inPtr_local, , cudaMemcpyDeviceToDevice));
      caffe_memcpy(chunkSize_input * sizeof(Dtype), inPtr_local, outPtr_local);
    }
  }

  void copy_many_to_one(const Dtype* inPtr, Dtype* outPtr, int numChunks, int chunkSize_output, int chunkStride_input) {
    for (int chunkIdx = 0; chunkIdx < numChunks; ++chunkIdx) {
      const Dtype* inPtr_local = inPtr + chunkIdx*chunkStride_input;
      Dtype* outPtr_local = outPtr + chunkIdx*chunkSize_output;
      //CUDA_CHECK(cudaMemcpy(outPtr_local, inPtr_local, chunkSize_output * sizeof(Dtype), cudaMemcpyDeviceToDevice));
      caffe_memcpy(chunkSize_output * sizeof(Dtype), inPtr_local, outPtr_local);
    }
  }

  //__global__ void sync_streams() {}
  virtual void Forward_(const vector<Blob*>& bottom, const vector<Blob*>& top) {
#if 0
    if (!this->gpuInited) {
      //std::cout<<"Initializing GPU local"<<std::endl;
      this->GPU_Initialization();
      this->gpuInited = true;
      //std::cout<< "GPUInited"<< std::endl;
    }
#endif
    //clock_t begin_fwd = std::clock();//timer
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* top_data = top[0]->mdata();
    const int count = bottom[0]->count();
    //copy to bottom_data to buffer with stride
    int chunkSize_copy_init = this->initChannel * this->H * this->W;
    int chunkStride_copy = (this->initChannel + this->growthRate * this->numtransition_) * this->H * this->W;
    if ((this->phase_cur_ == TRAIN) && useDropout) {
      copy_one_to_many(bottom_data, this->postDropout.mdata(), this->N, chunkSize_copy_init, chunkStride_copy);
    }
    else {
      copy_one_to_many(bottom_data, this->postConv.mdata(), this->N, chunkSize_copy_init, chunkStride_copy);
    }
    int work_n = this->N * (this->initChannel + this->numtransition_ * this->growthRate) * this->H * this->W;
    //work in the buffer, transition by transition
    for (int transitionIdx = 0; transitionIdx < this->numtransition_; ++transitionIdx) {
      bottleneck_Forward_(transitionIdx);
    }
    //deploy top data
    if ((this->phase_cur_ == TRAIN) && useDropout) {
      //cudaMemcpy(top[0]->gpu_mdata(), postDropout.gpu_mdata(), work_n * sizeof(Dtype), cudaMemcpyDeviceToDevice);
      caffe_memcpy(work_n * sizeof(Dtype), postDropout.data(), top[0]->mdata());
    }
    else {
      //cudaMemcpy(top[0]->gpu_mdata(), postConv.gpu_mdata(), work_n * sizeof(Dtype), cudaMemcpyDeviceToDevice);
      caffe_memcpy(work_n * sizeof(Dtype), postConv.data(), top[0]->mdata());
    }
    //clock_t end_fwd = std::clock();
    //double elapsed_fwd = double(end_fwd - begin_fwd) / CLOCKS_PER_SEC;
    //std::cout<<"elapsed fwd gpu:"<<elapsed_fwd<<std::endl;
    //this->logInternal("TClogFwd",-1,false,false);
  }

  virtual void LoopEndCleanup_gpu() {
    int valsBuffer = this->N * (this->initChannel + this->growthRate * this->numtransition_) * this->H * this->W;
    cleanupBuffer(this->postConv.mdata(), valsBuffer);
    cleanupBuffer(this->postConv.mdiff(), valsBuffer);
    if (useDropout) {
      cleanupBuffer(this->postDropout.mdata(), valsBuffer);
      cleanupBuffer(this->postDropout.mdiff(), valsBuffer);
    }
    cleanupBuffer(this->postBN.mdata(), valsBuffer);
    cleanupBuffer(this->postBN.mdiff(), valsBuffer);
    cleanupBuffer(this->postReLU.mdata(), valsBuffer);
    cleanupBuffer(this->postReLU.mdiff(), valsBuffer);
    int vals4G = N * 4 * growthRate*H*W;
    if (useBC) {
      cleanupBuffer(postConv_4G.mdiff(), vals4G);
      cleanupBuffer(postBN_4G.mdiff(), vals4G);
      cleanupBuffer(postReLU_4G.mdiff(), vals4G);
    }
  }





static Dtype getZeroPaddedValue(bool isDiff, Blob* inputData, int n, int c, int h, int w)
{
  int n_blob = inputData->shape(0);
  int c_blob = inputData->shape(1);
  int h_blob = inputData->shape(2);
  int w_blob = inputData->shape(3);
  if ((n < 0) || (n >= n_blob)) { return 0; }
  if ((c < 0) || (c >= c_blob)) { return 0; }
  if ((h < 0) || (h >= h_blob)) { return 0; }
  if ((w < 0) || (w >= w_blob)) { return 0; }
  if (isDiff) { return inputData->diff_at(n, c, h, w); }
  else { return inputData->data_at(n, c, h, w); }
}

//Assumption, h_filter and w_filter must be 3 for now
//naivest possible implementation of convolution, CPU forward and backward should not be used in production.
//CPU version of convolution assume img H,W does not change after convolution, which corresponds to denseBlock without BC
//input of size N*c_input*h_img*w_img

void convolution_Fwd(Blob* input, Blob* output, Blob* filter, int N, int c_output, int c_input, int h_img, int w_img, int h_filter, int w_filter)
{
  int outputShape[] = { N, c_output, h_img, w_img };
  vector<int> outputShapeVec(outputShape, outputShape + 4);
  output->Reshape(outputShapeVec);
  Dtype* outputPtr = output->mdata();
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

void convolution_Bwd(Blob* bottom, Blob* top, Blob* filter, int N, int c_output, int c_input, int h_img, int w_img, int h_filter, int w_filter)
{
  Dtype* filterDiffPtr = filter->mdiff();
  Dtype* bottomDiffPtr = bottom->mdiff();
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

void ReLU_Fwd(Blob* bottom, Blob* top, int N, int C, int h_img, int w_img)
{
  //Reshape top
  int topShapeArr[] = { N, C, h_img, w_img };
  vector<int> topShapeVec(topShapeArr, topShapeArr + 4);
  top->Reshape(topShapeVec);
  //ReLU Fwd
  Dtype* topPtr = top->mdata();
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

void ReLU_Bwd(Blob* bottom, Blob* top, int N, int C, int h_img, int w_img)
{
  Dtype* bottomDiffPtr = bottom->mdiff();
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

Dtype getMean(Blob* A, int channelIdx)
{
  int N = A->shape(0);
  int H = A->shape(2);
  int W = A->shape(3);
  int totalCount = N * H * W;
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

Dtype getVar(Blob* A, int channelIdx)
{
  int N = A->shape(0);
  int H = A->shape(2);
  int W = A->shape(3);
  int totalCount = N * H * W;
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

void BN_inf_Fwd(Blob* input, Blob* output, int N, int C, int h_img, int w_img, Blob* globalMean, Blob* globalVar, Blob* scaler, Blob* bias, Blob* factor_b)
{
  int channelShape[] = { 1, C, 1, 1 };
  vector<int> channelShapeVec(channelShape, channelShape + 4);
  Blob* localInf_Mean = new Blob(channelShapeVec);
  Blob* localInf_Var = new Blob(channelShapeVec);
  Dtype scale_factor = factor_b->data()[0] == 0 ? 0 : (1 / factor_b->data()[0]);
  caffe_scale(localInf_Mean->count(), scale_factor, globalMean->data(), localInf_Mean->mdata());
  caffe_scale(localInf_Var->count(), scale_factor, globalVar->data(), localInf_Var->mdata());
  //Reshape output
  int outputShape[] = { N, C, h_img, w_img };
  vector<int> outputShapeVec(outputShape, outputShape + 4);
  output->Reshape(outputShapeVec);
  //BN Fwd inf
  double epsilon = 1e-5;
  Dtype* outputPtr = output->mdata();
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

void BN_train_Fwd(Blob* bottom, Blob* top, Blob* output_xhat, Blob* globalMean, Blob* globalVar, Blob* batchMean, Blob* batchVar, Blob* scaler, Blob* bias, int N, int C, int h_img, int w_img, Dtype EMA_decay)
{
  //reshape output
  int outputShape[] = { N, C, h_img, w_img };
  vector<int> outputShapeVec(outputShape, outputShape + 4);
  top->Reshape(outputShapeVec);
  output_xhat->Reshape(outputShapeVec);
  //BN Fwd train
  double epsilon = 1e-5;
  //get batch/global Mean/Var
  for (int channelIdx = 0; channelIdx < C; ++channelIdx) {
    int variance_adjust_m = N * h_img * w_img;
    //batch
    Dtype* batchMean_mutable = batchMean->mdata();
    Dtype* batchVar_mutable = batchVar->mdata();
    batchMean_mutable[channelIdx] = getMean(bottom, channelIdx);
    batchVar_mutable[channelIdx] = (variance_adjust_m / (variance_adjust_m - 1.0)) * getVar(bottom, channelIdx);
    //global
    Dtype* globalMean_mutable = globalMean->mdata();
    Dtype* globalVar_mutable = globalVar->mdata();
    globalMean_mutable[channelIdx] = EMA_decay * globalMean->data_at(0, channelIdx, 0, 0) + batchMean->data_at(0, channelIdx, 0, 0);
    globalVar_mutable[channelIdx] = EMA_decay * globalVar->data_at(0, channelIdx, 0, 0) + batchVar->data_at(0, channelIdx, 0, 0);
  }
  //process data
  for (int n = 0; n < N; ++n) {
    for (int c = 0; c < C; ++c) {
      for (int h = 0; h < h_img; ++h) {
        for (int w = 0; w < w_img; ++w) {
          Dtype* xhat_mutable = output_xhat->mdata();
          xhat_mutable[output_xhat->offset(n, c, h, w)] = (bottom->data_at(n, c, h, w) - batchMean->data_at(0, c, 0, 0)) / sqrt(batchVar->data_at(0, c, 0, 0) + epsilon);
          Dtype* output_mutable = top->mdata();
          output_mutable[top->offset(n, c, h, w)] = (scaler->data_at(0, c, 0, 0)) * (output_xhat->data_at(n, c, h, w)) + bias->data_at(0, c, 0, 0);
        }
      }
    }
  }
}

bool decide_channelDiffAllZero(Blob* B, int channelIdx, int N, int C, int H, int W)
{
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

void BN_train_Bwd(Blob* bottom, Blob* bottom_xhat, Blob* top, Blob* batchMean, Blob* batchVar, Blob* scaler, Blob* bias, int N, int C, int h_img, int w_img, bool betaOneData)
{
  double epsilon = 1e-5;
  //bias and scaler grad
  Dtype* biasGrad = bias->mdiff();
  Dtype* scalerGrad = scaler->mdiff();
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
  Dtype* XhatGrad = bottom_xhat->mdiff();
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
  Dtype* varGrad = batchVar->mdiff();
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
  Dtype* meanGrad = batchMean->mdiff();
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
  Dtype* bottomDataGrad = bottom->mdiff();
  for (int n = 0; n < N; ++n) {
    for (int c = 0; c < C; ++c) {
      for (int h = 0; h < h_img; ++h) {
        for (int w = 0; w < w_img; ++w) {
          //Dtype term1=bottom_xhat->diff_at(n,c,h,w)*pow(batchVar->data_at(0,c,0,0)+epsilon,-0.5);
          Dtype term1 = bottom_xhat->diff_at(n, c, h, w) / (sqrt(batchVar->data_at(0, c, 0, 0) + epsilon));
          Dtype term2 = batchVar->diff_at(0, c, 0, 0) * 2.0 * (bottom->data_at(n, c, h, w) - batchMean->data_at(0, c, 0, 0)) / m;
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

void mergeChannelData(Blob* outputBlob, Blob* blobA, Blob* blobB)
{
  int N = blobA->shape(0);
  int frontC = blobA->shape(1);
  int backC = blobB->shape(1);
  int H = blobA->shape(2);
  int W = blobA->shape(3);
  for (int n = 0; n < N; ++n) {
    for (int c = 0; c < frontC + backC; ++c) {
      for (int h = 0; h < H; ++h) {
        for (int w = 0; w < W; ++w) {
          Dtype inData;
          if (c < frontC) {
            inData = blobA->data()[blobA->offset(n, c, h, w)];
          }
          else {
            int readC = c - frontC;
            inData = blobB->data()[blobB->offset(n, readC, h, w)];
          }
          outputBlob->mdata()[outputBlob->offset(n, c, h, w)] = inData;
        }
      }
    }
  }
}

void distributeChannelDiff(Blob* inputBlob, Blob* blobA, Blob* blobB)
{
  int N = blobA->shape(0);
  int frontC = blobA->shape(1);
  int backC = blobB->shape(1);
  int H = blobA->shape(2);
  int W = blobA->shape(3);
  for (int n = 0; n < N; ++n) {
    for (int c = 0; c < frontC + backC; ++c) {
      for (int h = 0; h < H; ++h) {
        for (int w = 0; w < W; ++w) {
          Dtype readData = inputBlob->diff()[inputBlob->offset(n, c, h, w)];
          if (c < frontC) {
            blobA->mdiff()[blobA->offset(n, c, h, w)] = readData;
          }
          else {
            int writeC = c - frontC;
            blobB->mdiff()[blobB->offset(n, writeC, h, w)] = readData;
          }
        }
      }
    }
  }
}

void BlobSetZero(Blob* B, int count)
{
  Dtype* B_mdata = B->mdata();
  Dtype* B_mdiff = B->mdiff();
  for (int i = 0; i < count; ++i) {
    B_mdata[i] = 0;
    B_mdiff[i] = 0;
  }
}

void LoopEndCleanup_cpu()
{
  for (int transitionIdx = 0; transitionIdx < this->numtransition_; ++transitionIdx) {
    int tensorCount = this->N * growthRate * this->H * this->W;
    int tensorMergeCount = this->N * (this->initChannel + this->growthRate * transitionIdx) * this->H * this->W;
    BlobSetZero(this->merged_conv[transitionIdx], tensorMergeCount);
    BlobSetZero(this->BN_XhatVec[transitionIdx], tensorMergeCount);
    BlobSetZero(this->postBN_blobVec[transitionIdx], tensorMergeCount);
    BlobSetZero(this->postReLU_blobVec[transitionIdx], tensorMergeCount);
    BlobSetZero(this->postConv_blobVec[transitionIdx], tensorCount);
  }
}


virtual void bottleneck_Forward_(int transitionIdx) {
  //BN Fwd
  Dtype* BN_x_ptr;
  if (this->phase_cur_ == TRAIN && useDropout) {
    BN_x_ptr = this->postDropout.mdata();
  }
  else {
    BN_x_ptr = this->postConv.mdata();
  }
  Dtype* BN_y_ptr = this->postBN.mdata();
  Dtype* BN_globalMean = this->blobs_[3 * this->numtransition_ + transitionIdx]->mdata();
  Dtype* BN_globalVar = this->blobs_[4 * this->numtransition_ + transitionIdx]->mdata();
  //Dtype* BC_filter = this->blobs_[5 * numtransition_ + transitionIdx]->mdata();
  int numChannels = initChannel + growthRate*transitionIdx;
  Dtype* local_MeanInf = this->Mean_tmp;
  Dtype* local_VarInf = this->Var_tmp;

  const Dtype* bnScale = this->blobs_[this->numtransition_ + transitionIdx]->data();
  const Dtype *bnBias = this->blobs_[2 * this->numtransition_ + transitionIdx]->data();
  int localChannels = this->initChannel + transitionIdx * this->growthRate;
  int inner_num_ = this->H * this->W;
  if (this->phase_cur_ == TEST) {
    int count = this->N*localChannels*inner_num_;
    BatchNormalizationForwardInference(this->N, localChannels, inner_num_, BN_x_ptr, BN_y_ptr, bnScale, bnBias, BN_globalMean, BN_globalVar, BN_MIN_EPSILON);
    //log_blob(this->postBN);
    //log_blob(this->postConv);
  }
  else {
    Dtype* batchMean = this->ResultSaveMean_gpu[transitionIdx];
    Dtype* batchInvVar = this->ResultSaveInvVariance_gpu[transitionIdx];
    BatchNormalizationForwardTraining(this->N, localChannels, inner_num_, BN_x_ptr, BN_y_ptr, bnScale, bnBias, Dtype(1), local_MeanInf, local_VarInf, BN_MIN_EPSILON, batchMean, batchInvVar);
    //update global Mean/Var manually
    //Mean:
    caffe_axpby(numChannels, EMA_decay, local_MeanInf, Dtype(1.0 - EMA_decay), BN_globalMean);
    //Var:
    caffe_axpby(numChannels, EMA_decay, local_VarInf, Dtype(1.0 - EMA_decay), BN_globalVar);
  }
  Dtype* ReLU_x_ptr = this->postBN.mdata();
  Dtype* ReLU_y_ptr = this->postReLU.mdata();
  if (1) {
    int count = this->postBN.count();
    relu_forward(count, ReLU_x_ptr, ReLU_y_ptr, 0);
  }
  //log_blob(this->postReLU);
  if (useBC) {
    assert(0);
#if 0
    //Convolution 1*1 kernel
    Dtype* conv_x_4G = postReLU.mdata();
    Dtype* conv_y_4G;
    if (BC_ultra_spaceEfficient) {
      conv_y_4G = postConv_4G.mdata();
    }
    else {
      conv_y_4G = postConv_4GVec[transitionIdx];
    }
    //CONV_ALGO
    CUDNN_CHECK(cudnnConvolutionForward(cudnnHandlePtr,
      get_one(),
      this->tensorDescriptorVec_conv_x[transitionIdx], conv_x_4G,
      this->BC_filterDescriptorVec[transitionIdx],
      this->blobs_[5 * numtransition_ + transitionIdx]->data(),
      convBC_Descriptor, BC_FwdAlgoVec[transitionIdx],
      workspace, workspace_size_bytes, get_zero(),
      quadG_tensorDesc, conv_y_4G
    ));
    //std::cout<<"BC Fwd Conv Done"<<std::endl;
    //BN 4G Fwd
    Dtype* BN_x_4G = BC_ultra_spaceEfficient ? postConv_4G.mdata() : postConv_4GVec[transitionIdx];
    Dtype* BN_y_4G = postBN_4G.mdata();
    Dtype* BN_BC_globalMean = this->blobs_[8 * numtransition_ + transitionIdx]->mdata();
    Dtype* BN_BC_globalVar = this->blobs_[9 * numtransition_ + transitionIdx]->mdata();
    Dtype* localBC_MeanInf = BC_MeanInfVec[transitionIdx];
    Dtype* localBC_VarInf = BC_VarInfVec[transitionIdx];
    //std::cout<<"BC Fwd BN Prepared"<<std::endl;
    if (this->phase_cur_ == TEST) {
      CUDNN_CHECK(cudnnBatchNormalizationForwardInference(
        cudnnHandlePtr, CUDNN_BATCHNORM_SPATIAL,
        get_one(), get_zero(),
        quadG_tensorDesc, BN_x_4G,
        quadG_tensorDesc, BN_y_4G,
        quadG_paramDesc,
        this->blobs_[6 * numtransition_ + transitionIdx]->data(),
        this->blobs_[7 * numtransition_ + transitionIdx]->data(),
        BN_BC_globalMean, BN_BC_globalVar, CUDNN_BN_MIN_EPSILON)
      );
    }
    else {
      Dtype* BC_batchMean = ResultSaveMean_BC[transitionIdx];
      Dtype* BC_batchInvVar = ResultSaveInvVariance_BC[transitionIdx];
      CUDNN_CHECK(cudnnBatchNormalizationForwardTraining(
        cudnnHandlePtr, CUDNN_BATCHNORM_SPATIAL,
        get_one(), get_zero(),
        quadG_tensorDesc, BN_x_4G,
        quadG_tensorDesc, BN_y_4G,
        quadG_paramDesc,
        this->blobs_[6 * numtransition_ + transitionIdx]->mdata(),
        this->blobs_[7 * numtransition_ + transitionIdx]->mdata(),
        Dtype(1), localBC_MeanInf, localBC_VarInf, CUDNN_BN_MIN_EPSILON,
        BC_batchMean, BC_batchInvVar
      ));
      caffe_axpby(4 * growthRate, EMA_decay, localBC_MeanInf, Dtype(1.0 - EMA_decay), BN_BC_globalMean);
      caffe_axpby(4 * growthRate, EMA_decay, localBC_VarInf, Dtype(1.0 - EMA_decay), BN_BC_globalVar);
    }
    //std::cout<<"BC Fwd BN Done"<<std::endl;
    //ReLU 4G Fwd
    Dtype* ReLU_BC_x = postBN_4G.mdata();
    Dtype* ReLU_BC_y = postReLU_4G.mdata();
    CUDNN_CHECK(cudnnActivationForward(cudnnHandlePtr, ReLUDesc,
      get_one(),
      quadG_tensorDesc, ReLU_BC_x,
      get_zero(),
      quadG_tensorDesc, ReLU_BC_y
    ));
    //std::cout<<"BC Fwd ReLU Done"<<std::endl;
#endif
  }
  //Convolution
  int delayChannel = this->initChannel + this->growthRate * transitionIdx;
  Dtype* conv_x_local;
  Blob* conv_x = NULL;
  if (useBC) {
    conv_x = &postReLU_4G;
    conv_x_local = postReLU_4G.mdata();
  }
  else {
    conv_x = &postReLU;
    conv_x_local = postReLU.mdata();
  }
  Dtype* conv_y_local = this->postConv.mdata() + delayChannel * this->H * this->W;
  //CONV_ALGO
  if (1) {
    //utime_start(a);
    int conv_x_channels = this->initChannel + this->growthRate * transitionIdx;
    Blob* conv_w = this->blobs_[transitionIdx];
    DataShape conv_x_shape = conv_x->shape_;
    conv_x_shape.c = conv_x_channels;
    DataShape conv_y_shape = dataShape(this->N, this->growthRate, this->H, this->W);
    Dtype* conv_x_local = postReLU.cpu_mdata();
    Dtype* conv_y_local = this->postConv.cpu_mdata() + delayChannel * this->H * this->W;
    const Dtype* w = conv_w->cpu_data();
    cpu_caffe_set(conv_y_shape.count(), 0, conv_y_local);
    //cpu_conv2d(conv_x_local, conv_y_local, w, NULL, conv_x_shape, conv_y_shape, conv_w->shape_.h, conv_w->shape_.w, 1, 1, 1, 1, 1, 1, 1, false);
    conv2d_fast(conv_x_local, conv_y_local, w, NULL, conv_x_shape, conv_y_shape, conv_w->shape_.h, conv_w->shape_.w, 1, 1, 1, 1, 1, 1, 1, false);
    //LOG(INFO) << utime_elapsed(a);
    //log_blob(this->postConv);
  }
#if 0
  //Dropout
  if ((this->phase_cur_ == TRAIN) && useDropout) {
    Dtype* dropout_x_local = postConv.mdata() + delayChannel*H*W;
    Dtype* dropout_y_local = postDropout.mdata() + delayChannel*H*W;
    CUDNN_CHECK(cudnnDropoutForward(cudnnHandlePtr,
      dropoutDescriptorVec[transitionIdx],
      tensorDescriptor_conv_y, dropout_x_local,
      tensorDescriptor_conv_y, dropout_y_local,
      dropout_reserve_gpu[transitionIdx], dropout_reserveSize[transitionIdx]
    ));
  }
#endif
  //this->logInternal_gpu("TClogFwd",transitionIdx,true,false);
}


void Forward_1(const vector<Blob*> & bottom, const vector<Blob*> & top)
{
  //init CPU
  if (!this->cpuInited) {
    //std::cout<<"fwd cpu init"<<std::endl;
    this->CPU_Initialization();
    this->cpuInited = true;
    //std::cout<<"fwd cpu init done"<<std::endl;
  }
  int bnTimerIdx = useBC ? 10 * numtransition_ : 5 * numtransition_;
  //deploy init data
  this->merged_conv[0]->CopyFrom(bottom[0]);
  //init CPU finish
  for (int transitionIdx = 0; transitionIdx < this->numtransition_; ++transitionIdx) {
    //BN
    Blob* BN_bottom = this->merged_conv[transitionIdx];
    Blob* BN_top = this->postBN_blobVec[transitionIdx];
    Blob* Scaler = this->blobs_[numtransition_ + transitionIdx];
    Blob* Bias = this->blobs_[2 * numtransition_ + transitionIdx];
    int localChannels = this->initChannel + transitionIdx * this->growthRate;
    if (this->phase_cur_ == TEST) {
      //std::cout<<"cpu BN test forward"<<std::endl;
      BN_inf_Fwd(BN_bottom, BN_top, this->N, localChannels, this->H, this->W, this->blobs_[3 * this->numtransition_ + transitionIdx], this->blobs_[4 * this->numtransition_ + transitionIdx], Scaler, Bias, this->blobs_[bnTimerIdx]);
    }
    else {
      //std::cout<<"cpu BN train forward"<<std::endl;
      BN_train_Fwd(BN_bottom, BN_top, this->BN_XhatVec[transitionIdx], this->blobs_[3 * this->numtransition_ + transitionIdx], this->blobs_[4 * this->numtransition_ + transitionIdx], this->batch_Mean[transitionIdx], this->batch_Var[transitionIdx], Scaler, Bias, this->N, localChannels, this->H, this->W, this->EMA_decay);
    }
    //ReLU
    Blob* ReLU_top = this->postReLU_blobVec[transitionIdx];
    ReLU_Fwd(BN_top, ReLU_top, this->N, localChannels, this->H, this->W);
    //if useBC, Conv1*1-BN(BC)-ReLU(BC)
    if (useBC) {
      //BC Conv 1*1
      Blob* BC_filterBlob = this->blobs_[5 * numtransition_ + transitionIdx];
      Blob* BC_conv_x = postReLU_blobVec[transitionIdx];
      Blob* BC_conv_y = postConv_BCVec[transitionIdx];
      int BC_conv_inChannel = initChannel + growthRate * transitionIdx;
      int BC_conv_outChannel = 4 * growthRate;
      convolution_Fwd(BC_conv_x, BC_conv_y, BC_filterBlob, N, BC_conv_outChannel, BC_conv_inChannel, H, W, 1, 1);
      //BC BN
      Blob* BC_BN_x = postConv_BCVec[transitionIdx];
      Blob* BC_BN_y = postBN_BCVec[transitionIdx];
      Blob* BC_Scaler = this->blobs_[6 * numtransition_ + transitionIdx];
      Blob* BC_Bias = this->blobs_[7 * numtransition_ + transitionIdx];
      Blob* BC_Mean = this->blobs_[8 * numtransition_ + transitionIdx];
      Blob* BC_Var = this->blobs_[9 * numtransition_ + transitionIdx];
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
    int inConvChannel = useBC ? 4 * growthRate : initChannel + growthRate * transitionIdx;
    convolution_Fwd(conv_x, conv_y, filterBlob, N, growthRate, inConvChannel, H, W, 3, 3);
    //post Conv merge
    Blob* mergeOutput = merged_conv[transitionIdx + 1];
    Blob* mergeInputA = merged_conv[transitionIdx];
    Blob* mergeInputB = postConv_blobVec[transitionIdx];
    mergeChannelData(mergeOutput, mergeInputA, mergeInputB);
  }
  //deploy output data
  top[0]->CopyFrom((this->merged_conv[this->numtransition_]));
  if (this->phase_cur_ == TRAIN) {
    this->blobs_[bnTimerIdx]->mdata()[0] *= this->EMA_decay;
    this->blobs_[bnTimerIdx]->mdata()[0] += 1;
    this->trainCycleIdx += 1;
  }
  //logInternal_cpu("TC_TrueFwdlog");
}

void Backward_(const vector<Blob*> & top, const vector<Blob*> & bottom)
{
  if (!this->cpuInited) {
    this->CPU_Initialization();
    this->cpuInited = true;
  }
  //deploy top diff
  this->merged_conv[this->numtransition_]->CopyFrom((top[0]), true);
  for (int transitionIdx = this->numtransition_ - 1; transitionIdx >= 0; --transitionIdx) {
    //distribute diff
    distributeChannelDiff(this->merged_conv[transitionIdx + 1], this->merged_conv[transitionIdx], this->postConv_blobVec[transitionIdx]);
    //Conv Bwd
    Blob* conv_top = this->postConv_blobVec[transitionIdx];
    Blob* conv_bottom = useBC ? postReLU_BCVec[transitionIdx] : postReLU_blobVec[transitionIdx];
    Blob* filter = this->blobs_[transitionIdx];
    int c_input = useBC ? 4 * growthRate : initChannel + growthRate * transitionIdx;
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
      Blob* BC_Scaler = this->blobs_[6 * numtransition_ + transitionIdx];
      Blob* BC_Bias = this->blobs_[7 * numtransition_ + transitionIdx];
      Blob* BC_batchMean = batch_Mean4G[transitionIdx];
      Blob* BC_batchVar = batch_Var4G[transitionIdx];
      BN_train_Bwd(BC_BN_x, BC_BN_xhat, BC_BN_y, BC_batchMean, BC_batchVar, BC_Scaler, BC_Bias, N, 4 * growthRate, H, W, false);
      //Conv1*1 BC Bwd
      Blob* BC_conv_x = postReLU_blobVec[transitionIdx];
      Blob* BC_conv_y = postConv_BCVec[transitionIdx];
      Blob* BC_filter = this->blobs_[5 * numtransition_ + transitionIdx];
      int BC_c_input = initChannel + growthRate * transitionIdx;
      int BC_c_output = 4 * growthRate;
      convolution_Bwd(BC_conv_x, BC_conv_y, BC_filter, N, BC_c_output, BC_c_input, H, W, 1, 1);
    }
    //ReLU Bwd
    int localChannel = this->initChannel + this->growthRate * transitionIdx;
    ReLU_Bwd(postBN_blobVec[transitionIdx], postReLU_blobVec[transitionIdx], this->N, localChannel, this->H, this->W);
    //BN Bwd
    Blob* BN_bottom = this->merged_conv[transitionIdx];
    Blob* scaler = this->blobs_[this->numtransition_ + transitionIdx];
    Blob* bias = this->blobs_[2 * this->numtransition_ + transitionIdx];
    BN_train_Bwd(BN_bottom, this->BN_XhatVec[transitionIdx], this->postBN_blobVec[transitionIdx], this->batch_Mean[transitionIdx], this->batch_Var[transitionIdx], scaler, bias, this->N, localChannel, this->H, this->W, true);
  }
  bottom[0]->CopyFrom((this->merged_conv[0]), true);
  //logInternal_cpu("TC_TrueBwdlog");
  this->LoopEndCleanup_cpu();
}

};


INSTANTIATE_CLASS(DenseBlock, DenseBlockLayer);


#endif  // CAFFE_DENSEBLOCK_LAYER_HPP_

