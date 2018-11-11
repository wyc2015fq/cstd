#include <math.h>
#include <string>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
//#include <dirent.h>
//#include <boost/lexical_cast.hpp>
//#include <boost/filesystem.hpp>
//#include <boost/algorithm/string.hpp>
#include "wstd/string.hpp"

  using wstd::itos;

  template <typename Dtype>
  Dtype getZeroPaddedValue(bool isDiff, Blob<Dtype>* inputData, int n, int c, int h, int w)
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
  template <typename Dtype>
  void convolution_Fwd(Blob<Dtype>* input, Blob<Dtype>* output, Blob<Dtype>* filter, int N, int c_output, int c_input, int h_img, int w_img, int h_filter, int w_filter)
  {
    int outputShape[] = { N, c_output, h_img, w_img };
    vector<int> outputShapeVec(outputShape, outputShape + 4);
    output->Reshape(outputShapeVec);
    Dtype* outputPtr = output->mutable_data<Context>();
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
  template <typename Dtype>
  void convolution_Bwd(Blob<Dtype>* bottom, Blob<Dtype>* top, Blob<Dtype>* filter, int N, int c_output, int c_input, int h_img, int w_img, int h_filter, int w_filter)
  {
    Dtype* filterDiffPtr = filter->mutable_diff<Context>();
    Dtype* bottomDiffPtr = bottom->mutable_diff<Context>();
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

  template <typename Dtype>
  void ReLU_Fwd(Blob<Dtype>* bottom, Blob<Dtype>* top, int N, int C, int h_img, int w_img)
  {
    //Reshape top
    int topShapeArr[] = { N, C, h_img, w_img };
    vector<int> topShapeVec(topShapeArr, topShapeArr + 4);
    top->Reshape(topShapeVec);
    //ReLU Fwd
    Dtype* topPtr = top->mutable_data<Context>();
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

  template <typename Dtype>
  void ReLU_Bwd(Blob<Dtype>* bottom, Blob<Dtype>* top, int N, int C, int h_img, int w_img)
  {
    Dtype* bottomDiffPtr = bottom->mutable_diff<Context>();
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

  template <typename Dtype>
  Dtype getMean(Blob<Dtype>* A, int channelIdx)
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

  template <typename Dtype>
  Dtype getVar(Blob<Dtype>* A, int channelIdx)
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

  template <typename Dtype>
  void BN_inf_Fwd(Blob<Dtype>* input, Blob<Dtype>* output, int N, int C, int h_img, int w_img, Blob<Dtype>* globalMean, Blob<Dtype>* globalVar, Blob<Dtype>* scaler, Blob<Dtype>* bias, Blob<Dtype>* factor_b)
  {
    int channelShape[] = { 1, C, 1, 1 };
    vector<int> channelShapeVec(channelShape, channelShape + 4);
    Blob<Dtype>* localInf_Mean = new Blob<Dtype>(channelShapeVec);
    Blob<Dtype>* localInf_Var = new Blob<Dtype>(channelShapeVec);
    Dtype scale_factor = factor_b->data<Context>()[0] == 0 ? 0 : (1 / factor_b->data<Context>()[0]);
    caffe_scale(localInf_Mean->count(), scale_factor, globalMean->data<Context>(), localInf_Mean->mutable_data<Context>());
    caffe_scale(localInf_Var->count(), scale_factor, globalVar->data<Context>(), localInf_Var->mutable_data<Context>());
    //Reshape output
    int outputShape[] = { N, C, h_img, w_img };
    vector<int> outputShapeVec(outputShape, outputShape + 4);
    output->Reshape(outputShapeVec);
    //BN Fwd inf
    double epsilon = 1e-5;
    Dtype* outputPtr = output->mutable_data<Context>();
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

  template <typename Dtype>
  void BN_train_Fwd(Blob<Dtype>* bottom, Blob<Dtype>* top, Blob<Dtype>* output_xhat, Blob<Dtype>* globalMean, Blob<Dtype>* globalVar, Blob<Dtype>* batchMean, Blob<Dtype>* batchVar, Blob<Dtype>* scaler, Blob<Dtype>* bias, int N, int C, int h_img, int w_img, Dtype EMA_decay)
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
      Dtype* batchMean_mutable = batchMean->mutable_data<Context>();
      Dtype* batchVar_mutable = batchVar->mutable_data<Context>();
      batchMean_mutable[channelIdx] = getMean(bottom, channelIdx);
      batchVar_mutable[channelIdx] = (variance_adjust_m / (variance_adjust_m - 1.0)) * getVar(bottom, channelIdx);
      //global
      Dtype* globalMean_mutable = globalMean->mutable_data<Context>();
      Dtype* globalVar_mutable = globalVar->mutable_data<Context>();
      globalMean_mutable[channelIdx] = EMA_decay * globalMean->data_at(0, channelIdx, 0, 0) + batchMean->data_at(0, channelIdx, 0, 0);
      globalVar_mutable[channelIdx] = EMA_decay * globalVar->data_at(0, channelIdx, 0, 0) + batchVar->data_at(0, channelIdx, 0, 0);
    }
    //process data
    for (int n = 0; n < N; ++n) {
      for (int c = 0; c < C; ++c) {
        for (int h = 0; h < h_img; ++h) {
          for (int w = 0; w < w_img; ++w) {
            Dtype* xhat_mutable = output_xhat->mutable_data<Context>();
            xhat_mutable[output_xhat->offset(n, c, h, w)] = (bottom->data_at(n, c, h, w) - batchMean->data_at(0, c, 0, 0)) / sqrt(batchVar->data_at(0, c, 0, 0) + epsilon);
            Dtype* output_mutable = top->mutable_data<Context>();
            output_mutable[top->offset(n, c, h, w)] = (scaler->data_at(0, c, 0, 0)) * (output_xhat->data_at(n, c, h, w)) + bias->data_at(0, c, 0, 0);
          }
        }
      }
    }
  }

  template <typename Dtype>
  bool decide_channelDiffAllZero(Blob<Dtype>* B, int channelIdx, int N, int C, int H, int W)
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

  template <typename Dtype>
  void BN_train_Bwd(Blob<Dtype>* bottom, Blob<Dtype>* bottom_xhat, Blob<Dtype>* top, Blob<Dtype>* batchMean, Blob<Dtype>* batchVar, Blob<Dtype>* scaler, Blob<Dtype>* bias, int N, int C, int h_img, int w_img, bool betaOneData)
  {
    double epsilon = 1e-5;
    //bias and scaler grad
    Dtype* biasGrad = bias->mutable_diff<Context>();
    Dtype* scalerGrad = scaler->mutable_diff<Context>();
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
    Dtype* XhatGrad = bottom_xhat->mutable_diff<Context>();
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
    Dtype* varGrad = batchVar->mutable_diff<Context>();
    for (int c = 0; c < C; ++c) {
      for (int n = 0; n < N; ++n) {
        for (int h = 0; h < h_img; ++h) {
          for (int w = 0; w < w_img; ++w) {
            //varGrad[c] += bottom_xhat->diff_at(n,c,h,w) * (bottom->data_at(n,c,h,w)-batchMean->data_at(0,c,0,0)) * (-0.5) * pow(batchVar->data_at(0,c,0,0) + epsilon,-1.5);
            varGrad[c] += bottom_xhat->diff_at(n, c, h, w) * (bottom->data_at(n, c, h, w) - batchMean->data_at(0, c, 0, 0)) * (-0.5) * (1.0 / ((batchVar->data_at(0, c, 0, 0) + epsilon) * sqrt(batchVar->data_at(0, c, 0, 0) + epsilon)));
            //flag
            //if (decide_channelDiffAllZero<Dtype>(top,c,N,C,h_img,w_img)){
            //  std::cout<<varGrad[c]<<std::endl;
            //}
          }
        }
      }
    }
    //helper 3:
    double m = N * h_img * w_img;
    Dtype* meanGrad = batchMean->mutable_diff<Context>();
    for (int c = 0; c < C; ++c) {
      for (int n = 0; n < N; ++n) {
        for (int h = 0; h < h_img; ++h) {
          for (int w = 0; w < w_img; ++w) {
            meanGrad[c] += bottom_xhat->diff_at(n, c, h, w) * (-1.0 / sqrt(batchVar->data_at(0, c, 0, 0) + epsilon)) + batchVar->diff_at(0, c, 0, 0) * (-2.0) * (bottom->data_at(n, c, h, w) - batchMean->data_at(0, c, 0, 0)) / m;
            //if (decide_channelDiffAllZero<Dtype>(top,c,N,C,h_img,w_img)){
            //  std::cout<<varGrad[c]<<std::endl;
            //}
          }
        }
      }
    }
    //combine helpers
    Dtype* bottomDataGrad = bottom->mutable_diff<Context>();
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
            } else {
              bottomDataGrad[bottom->offset(n, c, h, w)] = term1 + term2 + term3;
            }
            //std::cout<<term1<<","<<term2<<","<<term3<<std::endl;
          }
        }
      }
    }
  }


  template <typename Dtype>
  void DenseBlockLayer<Dtype>::CPU_Initialization()
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
      this->batch_Mean[transitionIdx] = new Blob<Dtype>(channelShape);
      this->batch_Var[transitionIdx] = new Blob<Dtype>(channelShape);
      this->merged_conv[transitionIdx] = new Blob<Dtype>(mergeShape);
      this->BN_XhatVec[transitionIdx] = new Blob<Dtype>(mergeShape);
      this->postBN_blobVec[transitionIdx] = new Blob<Dtype>(mergeShape);
      this->postReLU_blobVec[transitionIdx] = new Blob<Dtype>(mergeShape);
      this->postConv_blobVec[transitionIdx] = new Blob<Dtype>(conv_y_Shape);
      if (useBC) {
        int quadGShapeArr[] = { N, 4 * growthRate, H, W };
        int quadChannelArr[] = { 1, 4 * growthRate, 1, 1 };
        vector<int> quadGShape(quadGShapeArr, quadGShapeArr + 4);
        vector<int> quadChannelShape(quadChannelArr, quadChannelArr + 4);
        this->BC_BN_XhatVec[transitionIdx] = new Blob<Dtype>(quadGShape);
        this->postBN_BCVec[transitionIdx] = new Blob<Dtype>(quadGShape);
        this->postReLU_BCVec[transitionIdx] = new Blob<Dtype>(quadGShape);
        this->postConv_BCVec[transitionIdx] = new Blob<Dtype>(quadGShape);
        batch_Mean4G[transitionIdx] = new Blob<Dtype>(quadChannelShape);
        batch_Var4G[transitionIdx] = new Blob<Dtype>(quadChannelShape);
      }
    }
    //the last element of merged_conv serve as output of forward
    int extraMergeOutputShapeArr[] = { this->N, this->initChannel + this->growthRate* this->numTransition, this->H, this->W };
    vector<int> extraMergeOutputShapeVector(extraMergeOutputShapeArr, extraMergeOutputShapeArr + 4);
    this->merged_conv[this->numTransition] = new Blob<Dtype>(extraMergeOutputShapeVector);
  }

  template <typename Dtype>
  void mergeChannelData(Blob<Dtype>* outputBlob, Blob<Dtype>* blobA, Blob<Dtype>* blobB)
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
              inData = blobA->data<Context>()[blobA->offset(n, c, h, w)];
            } else {
              int readC = c - frontC;
              inData = blobB->data<Context>()[blobB->offset(n, readC, h, w)];
            }
            outputBlob->mutable_data<Context>()[outputBlob->offset(n, c, h, w)] = inData;
          }
        }
      }
    }
  }

  template <typename Dtype>
  void distributeChannelDiff(Blob<Dtype>* inputBlob, Blob<Dtype>* blobA, Blob<Dtype>* blobB)
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
            Dtype readData = inputBlob->diff<Context>()[inputBlob->offset(n, c, h, w)];
            if (c < frontC) {
              blobA->mutable_diff<Context>()[blobA->offset(n, c, h, w)] = readData;
            } else {
              int writeC = c - frontC;
              blobB->mutable_diff<Context>()[blobB->offset(n, writeC, h, w)] = readData;
            }
          }
        }
      }
    }
  }

  template <typename Dtype>
  void BlobSetZero(Blob<Dtype>* B, int count)
  {
    Dtype* B_mutable_data = B->mutable_data<Context>();
    Dtype* B_mutable_diff = B->mutable_diff<Context>();
    for (int i = 0; i < count; ++i) {
      B_mutable_data[i] = 0;
      B_mutable_diff[i] = 0;
    }
  }

  template <typename Dtype>
  void DenseBlockLayer<Dtype>::LoopEndCleanup_cpu()
  {
    for (int transitionIdx = 0; transitionIdx < this->numTransition; ++transitionIdx) {
      int tensorCount = this->N * growthRate * this->H * this->W;
      int tensorMergeCount = this->N * (this->initChannel + this->growthRate * transitionIdx) * this->H * this->W;
      BlobSetZero<Dtype>(this->merged_conv[transitionIdx], tensorMergeCount);
      BlobSetZero<Dtype>(this->BN_XhatVec[transitionIdx], tensorMergeCount);
      BlobSetZero<Dtype>(this->postBN_blobVec[transitionIdx], tensorMergeCount);
      BlobSetZero<Dtype>(this->postReLU_blobVec[transitionIdx], tensorMergeCount);
      BlobSetZero<Dtype>(this->postConv_blobVec[transitionIdx], tensorCount);
    }
  }

  template <typename Dtype>
  void DenseBlockLayer<Dtype>::Forward(CPUContext* context, const vector<Blob<Dtype>*> & bottom,
      const vector<Blob<Dtype>*> & top)
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
    this->merged_conv[0]->CopyFrom(*(bottom[0]));
    //init CPU finish
    for (int transitionIdx = 0; transitionIdx < this->numTransition; ++transitionIdx) {
      //BN
      Blob<Dtype>* BN_bottom = this->merged_conv[transitionIdx];
      Blob<Dtype>* BN_top = this->postBN_blobVec[transitionIdx];
      Blob<Dtype>* Scaler = this->blobs_[numTransition + transitionIdx].get();
      Blob<Dtype>* Bias = this->blobs_[2 * numTransition + transitionIdx].get();
      int localChannels = this->initChannel + transitionIdx * this->growthRate;
      if (this->phase_ == TEST) {
        //std::cout<<"cpu BN test forward"<<std::endl;
        BN_inf_Fwd<Dtype>(BN_bottom, BN_top, this->N, localChannels, this->H, this->W, this->blobs_[3 * this->numTransition + transitionIdx].get(), this->blobs_[4 * this->numTransition + transitionIdx].get(), Scaler, Bias, this->blobs_[bnTimerIdx].get());
      } else {
        //std::cout<<"cpu BN train forward"<<std::endl;
        BN_train_Fwd<Dtype>(BN_bottom, BN_top, this->BN_XhatVec[transitionIdx], this->blobs_[3 * this->numTransition + transitionIdx].get(), this->blobs_[4 * this->numTransition + transitionIdx].get(), this->batch_Mean[transitionIdx], this->batch_Var[transitionIdx], Scaler, Bias, this->N, localChannels, this->H, this->W, this->EMA_decay);
      }
      //ReLU
      Blob<Dtype>* ReLU_top = this->postReLU_blobVec[transitionIdx];
      ReLU_Fwd<Dtype>(BN_top, ReLU_top, this->N, localChannels, this->H, this->W);
      //if useBC, Conv1*1-BN(BC)-ReLU(BC)
      if (useBC) {
        //BC Conv 1*1
        Blob<Dtype>* BC_filterBlob = this->blobs_[5 * numTransition + transitionIdx].get();
        Blob<Dtype>* BC_conv_x = postReLU_blobVec[transitionIdx];
        Blob<Dtype>* BC_conv_y = postConv_BCVec[transitionIdx];
        int BC_conv_inChannel = initChannel + growthRate * transitionIdx;
        int BC_conv_outChannel = 4 * growthRate;
        convolution_Fwd<Dtype>(BC_conv_x, BC_conv_y, BC_filterBlob, N, BC_conv_outChannel, BC_conv_inChannel, H, W, 1, 1);
        //BC BN
        Blob<Dtype>* BC_BN_x = postConv_BCVec[transitionIdx];
        Blob<Dtype>* BC_BN_y = postBN_BCVec[transitionIdx];
        Blob<Dtype>* BC_Scaler = this->blobs_[6 * numTransition + transitionIdx].get();
        Blob<Dtype>* BC_Bias = this->blobs_[7 * numTransition + transitionIdx].get();
        Blob<Dtype>* BC_Mean = this->blobs_[8 * numTransition + transitionIdx].get();
        Blob<Dtype>* BC_Var = this->blobs_[9 * numTransition + transitionIdx].get();
        if (this->phase_ == TEST) {
          BN_inf_Fwd<Dtype>(BC_BN_x, BC_BN_y, N, 4 * growthRate, H, W, BC_Mean, BC_Var, BC_Scaler, BC_Bias, this->blobs_[bnTimerIdx].get());
        } else {
          Blob<Dtype>* BC_xhat = BC_BN_XhatVec[transitionIdx];
          Blob<Dtype>* BC_batchMean = batch_Mean4G[transitionIdx];
          Blob<Dtype>* BC_batchVar = batch_Var4G[transitionIdx];
          BN_train_Fwd<Dtype>(BC_BN_x, BC_BN_y, BC_xhat, BC_Mean, BC_Var, BC_batchMean, BC_batchVar, BC_Scaler, BC_Bias, N, 4 * growthRate, H, W, EMA_decay);
        }
        //BC ReLU
        Blob<Dtype>* ReLU_x = postBN_BCVec[transitionIdx];
        Blob<Dtype>* ReLU_y = postReLU_BCVec[transitionIdx];
        ReLU_Fwd<Dtype>(ReLU_x, ReLU_y, N, 4 * growthRate, H, W);
      }
      //Conv
      Blob<Dtype>* filterBlob = this->blobs_[transitionIdx].get();
      Blob<Dtype>* conv_x = useBC ? postReLU_BCVec[transitionIdx] : postReLU_blobVec[transitionIdx];
      Blob<Dtype>* conv_y = this->postConv_blobVec[transitionIdx];
      int inConvChannel = useBC ? 4 * growthRate : initChannel + growthRate * transitionIdx;
      convolution_Fwd<Dtype>(conv_x, conv_y, filterBlob, N, growthRate, inConvChannel, H, W, 3, 3);
      //post Conv merge
      Blob<Dtype>* mergeOutput = merged_conv[transitionIdx + 1];
      Blob<Dtype>* mergeInputA = merged_conv[transitionIdx];
      Blob<Dtype>* mergeInputB = postConv_blobVec[transitionIdx];
      mergeChannelData(mergeOutput, mergeInputA, mergeInputB);
    }
    //deploy output data
    top[0]->CopyFrom(*(this->merged_conv[this->numTransition]));
    if (this->phase_ == TRAIN) {
      this->blobs_[bnTimerIdx]->mutable_data<Context>()[0] *= this->EMA_decay;
      this->blobs_[bnTimerIdx]->mutable_data<Context>()[0] += 1;
      this->trainCycleIdx += 1;
    }
    //logInternal_cpu("TC_TrueFwdlog");
  }

  template <typename Dtype>
  void DenseBlockLayer<Dtype>::Backward(CPUContext* context, const vector<Blob<Dtype>*> & top,
      int*
      const vector<Blob<Dtype>*> & bottom)
  {
    if (!this->cpuInited) {
      this->CPU_Initialization();
      this->cpuInited = true;
    }
    //deploy top diff
    this->merged_conv[this->numTransition]->CopyFrom(*(top[0]), true);
    for (int transitionIdx = this->numTransition - 1; transitionIdx >= 0; --transitionIdx) {
      //distribute diff
      distributeChannelDiff(this->merged_conv[transitionIdx + 1], this->merged_conv[transitionIdx], this->postConv_blobVec[transitionIdx]);
      //Conv Bwd
      Blob<Dtype>* conv_top = this->postConv_blobVec[transitionIdx];
      Blob<Dtype>* conv_bottom = useBC ? postReLU_BCVec[transitionIdx] : postReLU_blobVec[transitionIdx];
      Blob<Dtype>* filter = this->blobs_[transitionIdx].get();
      int c_input = useBC ? 4 * growthRate : initChannel + growthRate * transitionIdx;
      convolution_Bwd<Dtype>(conv_bottom, conv_top, filter, this->N, this->growthRate, c_input, this->H, this->W, 3, 3);
      //BC ReLU_BC_Bwd - BN_BC_Bwd - Conv1*1_BC_Bwd
      if (useBC) {
        //ReLU BC Bwd
        Blob<Dtype>* BC_ReLU_y = postReLU_BCVec[transitionIdx];
        Blob<Dtype>* BC_ReLU_x = postBN_BCVec[transitionIdx];
        ReLU_Bwd<Dtype>(BC_ReLU_x, BC_ReLU_y, N, 4 * growthRate, H, W);
        //BN BC Bwd
        Blob<Dtype>* BC_BN_y = postBN_BCVec[transitionIdx];
        Blob<Dtype>* BC_BN_x = postConv_BCVec[transitionIdx];
        Blob<Dtype>* BC_BN_xhat = BC_BN_XhatVec[transitionIdx];
        Blob<Dtype>* BC_Scaler = this->blobs_[6 * numTransition + transitionIdx].get();
        Blob<Dtype>* BC_Bias = this->blobs_[7 * numTransition + transitionIdx].get();
        Blob<Dtype>* BC_batchMean = batch_Mean4G[transitionIdx];
        Blob<Dtype>* BC_batchVar = batch_Var4G[transitionIdx];
        BN_train_Bwd<Dtype>(BC_BN_x, BC_BN_xhat, BC_BN_y, BC_batchMean, BC_batchVar, BC_Scaler, BC_Bias, N, 4 * growthRate, H, W, false);
        //Conv1*1 BC Bwd
        Blob<Dtype>* BC_conv_x = postReLU_blobVec[transitionIdx];
        Blob<Dtype>* BC_conv_y = postConv_BCVec[transitionIdx];
        Blob<Dtype>* BC_filter = this->blobs_[5 * numTransition + transitionIdx].get();
        int BC_c_input = initChannel + growthRate * transitionIdx;
        int BC_c_output = 4 * growthRate;
        convolution_Bwd<Dtype>(BC_conv_x, BC_conv_y, BC_filter, N, BC_c_output, BC_c_input, H, W, 1, 1);
      }
      //ReLU Bwd
      int localChannel = this->initChannel + this->growthRate * transitionIdx;
      ReLU_Bwd<Dtype>(postBN_blobVec[transitionIdx], postReLU_blobVec[transitionIdx], this->N, localChannel, this->H, this->W);
      //BN Bwd
      Blob<Dtype>* BN_bottom = this->merged_conv[transitionIdx];
      Blob<Dtype>* scaler = this->blobs_[this->numTransition + transitionIdx].get();
      Blob<Dtype>* bias = this->blobs_[2 * this->numTransition + transitionIdx].get();
      BN_train_Bwd<Dtype>(BN_bottom, this->BN_XhatVec[transitionIdx], this->postBN_blobVec[transitionIdx], this->batch_Mean[transitionIdx], this->batch_Var[transitionIdx], scaler, bias, this->N, localChannel, this->H, this->W, true);
    }
    bottom[0]->CopyFrom(*(this->merged_conv[0]), true);
    //logInternal_cpu("TC_TrueBwdlog");
    this->LoopEndCleanup_cpu();
  }

  template <typename Dtype>
  void DenseBlockLayer<Dtype>::Forward_cpu_public(const vector<Blob<Dtype>*> & bottom, const vector<Blob<Dtype>*> & top)
  {
    this->Forward(_CONTEXT,bottom, top);
  }

  template <typename Dtype>
  void DenseBlockLayer<Dtype>::Backward_cpu_public(const vector<Blob<Dtype>*> & top, const vector<Blob<Dtype>*> & bottom)
  {
    this->Backward_cpu(top, propagate_down, bottom);
  }


