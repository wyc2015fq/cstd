#ifndef CAFFE_DENSEBLOCK_LAYER_HPP_
#define CAFFE_DENSEBLOCK_LAYER_HPP_


#include <string>
#include "cudnn.h"
#include "wstd/string.hpp"
using namespace wstd;

#define DenseBlockParameter_DEF(DEF) \
DEF##Struct(Filter_Filler, 0, Filler) \
DEF##Struct(BN_Scaler_Filler, 0, Filler) \
DEF##Struct(BN_Bias_Filler, 0, Filler) \

class DenseBlockLayer : public Layer
{
public:
  DenseBlockParameter_DEF(Def);
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

  Dtype* Mean_tmp;
  Dtype* Var_tmp;
  vector<Dtype*> ResultSaveMean_gpu;
  vector<Dtype*> ResultSaveInvVariance_gpu;

  int trainCycleIdx; //used in BN train phase for EMA Mean/Var estimation
                     //convolution Related
  int pad_h, pad_w, conv_verticalStride, conv_horizentalStride;
  int filter_H, filter_W;
  //Decay value used in EMA of BN
  Dtype EMA_decay;

  //end GPU specific data setion

  int initChannel, growthRate, numTransition;
  int N, H, W; //N,H,W of the input tensor, inited in reshape phase

  bool useDropout;
  float dropoutAmount;
  unsigned long long DB_randomSeed;
  bool useBC;
  bool BC_ultra_spaceEfficient;

  virtual inline const char* type() const { return "DenseBlock"; }

public:

  DenseBlockLayer() {
    DenseBlockParameter_DEF(Set);
  }
  void init(CJSON* param) {
    DenseBlockParameter_DEF(Get);
    BLOB_DEF(BLOB_NAME);

    this->numTransition = param->getint("numtransition", 40);
    this->growthRate = param->getint("growthrate", 12);
    this->EMA_decay = param->getfloat("moving_average_fraction", 0.1);
#ifndef CPU_ONLY
    this->workspace_size_bytes = param->getint("workspace_mb", 8) * 1024 * 1024;
    this->gpu_idx_ = param->getint("gpuidx", 0);
#endif
    this->useDropout = param->getbool("use_dropout", false);
    this->dropoutAmount = param->getfloat("dropout_amount", 0);
    this->DB_randomSeed = 124816;
    this->useBC = param->getbool("use_bc", false);
    this->BC_ultra_spaceEfficient = param->getbool("bc_ultra_space_efficient", false);
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
    //blobs_[numTransition + i] is its scaler blob
    //blobs_[2*numTransition + i] is its bias blob
    //blobs_[3*numTransition + i] is its globalMean
    //blobs_[4*numTransition + i] is its globalVar
    int blobs_size = 0;
    if (useBC) {
      blobs_size = 10 * this->numTransition + 1;
    }
    else {
      blobs_size = 5 * this->numTransition + 1;
    }
    if (blobs_.size() < blobs_size) {
      blobs_reset(this->blobs_, blobs_size);
      for (int transitionIdx = 0; transitionIdx < this->numTransition; ++transitionIdx) {
        //filter
        //No BC case
        if (!useBC) {
          int inChannels = initChannel + transitionIdx * growthRate;
          int filterShape_Arr[] = { growthRate, inChannels, 3, 3 };
          vector<int> filterShape(filterShape_Arr, filterShape_Arr + 4);
          this->blobs_[transitionIdx]->Reshape((filterShape));
          Fill(this->blobs_[transitionIdx], &Filter_Filler_);
        }
        else {
          //3*3 kernel
          int filter_33_shapeArr[] = { growthRate, 4 * growthRate, 3, 3 };
          vector<int> filter33Shape(filter_33_shapeArr, filter_33_shapeArr + 4);
          this->blobs_[transitionIdx]->Reshape((filter33Shape));
          Fill(this->blobs_[transitionIdx], &Filter_Filler_);
          //1*1 kernel
          int inChannels = initChannel + transitionIdx * growthRate;
          int filter_11_shapeArr[] = { 4 * growthRate, inChannels, 1, 1 };
          vector<int> filter11Shape(filter_11_shapeArr, filter_11_shapeArr + 4);
          this->blobs_[5 * numTransition + transitionIdx]->Reshape((filter11Shape));
          Fill(this->blobs_[5 * numTransition + transitionIdx], &Filter_Filler_);
        }
        //scaler & bias
        int inChannels = initChannel + transitionIdx * growthRate;
        int BNparamShape_Arr[] = { 1, inChannels, 1, 1 };
        vector<int> BNparamShape(BNparamShape_Arr, BNparamShape_Arr + 4);
        //scaler
        this->blobs_[numTransition + transitionIdx]->Reshape((BNparamShape));
        Fill(this->blobs_[numTransition + transitionIdx], &BN_Scaler_Filler_);
        int BN_4G_Shape[] = { 1, 4 * growthRate, 1, 1 };
        vector<int> BN_4Gparam_ShapeVec(BN_4G_Shape, BN_4G_Shape + 4);
        //scaler BC
        if (useBC) {
          this->blobs_[6 * numTransition + transitionIdx]->Reshape((BN_4Gparam_ShapeVec));
          Fill(this->blobs_[6 * numTransition + transitionIdx], &BN_Scaler_Filler_);
        }
        //bias
        this->blobs_[2 * numTransition + transitionIdx]->Reshape((BNparamShape));
        Fill(this->blobs_[2 * numTransition + transitionIdx], &BN_Bias_Filler_);
        //bias BC
        if (useBC) {
          this->blobs_[7 * numTransition + transitionIdx]->Reshape((BN_4Gparam_ShapeVec));
          Fill(this->blobs_[7 * numTransition + transitionIdx], &BN_Bias_Filler_);
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
          Blob* blob = blobs_[i];
          //global Mean/Var
          if (i >= 3 * this->numTransition) {
            blob->set_lr_mult(0.f);
            blob->set_decay_mult(0.f);
          }
          //BN Scaler and Bias
          else if (i >= this->numTransition) {
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
          if ((i >= 3 * numTransition) && (i < 5 * numTransition)) {
            blob->set_lr_mult(0.f);
            blob->set_decay_mult(0.f);
          }
          else if (i >= 8 * numTransition) {
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
#ifndef CPU_ONLY
    GPU_Initialization();
#else
    CPU_Initialization();
#endif
  }

  virtual void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    int batch_size = bottom[0]->shape(0);
    int h = bottom[0]->shape(2);
    int w = bottom[0]->shape(3);
#ifndef CPU_ONLY
    reshape_data(this->H, this->W, this->N, h, w, batch_size);
#endif
    this->N = batch_size;
    this->H = h;
    this->W = w;
    int topShapeArr[] = { this->N, this->initChannel + this->numTransition* this->growthRate, this->H, this->W };
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


public:
  #define log_blob(blob)    LOG_IF(INFO, root_solver()) << " denseblock blob " << (blob).to_debug_string()

  void CPU_Initialization()
  {
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
    int extraMergeOutputShapeArr[] = { this->N, this->initChannel + this->growthRate* this->numTransition, this->H, this->W };
    vector<int> extraMergeOutputShapeVector(extraMergeOutputShapeArr, extraMergeOutputShapeArr + 4);
    this->merged_conv[this->numTransition] = new Blob(extraMergeOutputShapeVector);



    DataShape shape = dataShape(N, (this->initChannel + this->growthRate*this->numTransition), H, W);
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
    for (int i = 0; i < this->numTransition; ++i) {
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
    int totalNumChannel = this->initChannel + this->growthRate * this->numTransition;
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
    int chunkStride_copy = (this->initChannel + this->growthRate * this->numTransition) * this->H * this->W;
    if ((this->phase_ == TRAIN) && useDropout) {
      copy_one_to_many(bottom_data, this->postDropout.mdata(), this->N, chunkSize_copy_init, chunkStride_copy);
    }
    else {
      copy_one_to_many(bottom_data, this->postConv.mdata(), this->N, chunkSize_copy_init, chunkStride_copy);
    }
    int work_n = this->N * (this->initChannel + this->numTransition * this->growthRate) * this->H * this->W;
    //work in the buffer, transition by transition
    for (int transitionIdx = 0; transitionIdx < this->numTransition; ++transitionIdx) {
      bottleneck_Forward_(transitionIdx);
    }
    //deploy top data
    if ((this->phase_ == TRAIN) && useDropout) {
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
    int valsBuffer = this->N * (this->initChannel + this->growthRate * this->numTransition) * this->H * this->W;
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

#ifndef CPU_ONLY
#include "DenseBlock_layer.cuh"
#else
#include "DenseBlock_layer.inl"
#endif


};


INSTANTIATE_CLASS(DenseBlock);

#undef log_blob

#endif  // CAFFE_DENSEBLOCK_LAYER_HPP_

