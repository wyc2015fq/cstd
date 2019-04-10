#include <iomanip>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <stdio.h>

#include "cdnn.h"
//#include <cuda.h>
//#include <cdnn.h>
//#include <cdnn_v7.h>

CDnn* dnn = GetCDnn(CDNN_CPU);

#define CUDA_CALL(f) { \
  cdnnError_t err = (f); \
  if (err != cdnnSuccess) { \
    std::cout \
        << "    Error occurred: " << err << std::endl; \
    std::exit(1); \
  } \
}
 
#define CDNN_CALL(f) { \
  cdnnStatus_t err = (f); \
  if (err != CDNN_STATUS_SUCCESS) { \
    std::cout \
        << "    Error occurred: " << err << std::endl; \
    std::exit(1); \
  } \
}

#if 0
__global__ void dev_const(float *px, float k) {
  int tid = threadIdx.x + blockIdx.x * blockDim.x;
  px[tid] = k;
}
 
__global__ void dev_iota(float *px) {
  int tid = threadIdx.x + blockIdx.x * blockDim.x;
  px[tid] = tid;
}
#endif
 
void print(const float *data, int n, int c, int h, int w) {
  std::vector<float> buffer(1 << 20);
  CUDA_CALL(dnn->Memcpy(
        buffer.data(), data,
        n * c * h * w * sizeof(float),
        cdnnMemcpyDeviceToHost));
  int a = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < c; ++j) {
      std::cout << "n=" << i << ", c=" << j << ":" << std::endl;
      for (int k = 0; k < h; ++k) {
        for (int l = 0; l < w; ++l) {
          std::cout << std::setw(4) << std::right << buffer[a];
          ++a;
        }
        std::cout << std::endl;
      }
    }
  }
  std::cout << std::endl;
}
 
int test_conv() {
  cdnnHandle_t cdnn;
  CDNN_CALL(dnn->Create(&cdnn));
 
  // input
  const int in_n = 1;
  const int in_c = 1;
  const int in_h = 5;
  const int in_w = 5;
  std::cout << "in_n: " << in_n << std::endl;
  std::cout << "in_c: " << in_c << std::endl;
  std::cout << "in_h: " << in_h << std::endl;
  std::cout << "in_w: " << in_w << std::endl;
  std::cout << std::endl;
 
  cdnnTensorDescriptor_t in_desc;
  CDNN_CALL(dnn->CreateTensorDescriptor(&in_desc));
  CDNN_CALL(dnn->SetTensor4dDescriptor(
        in_desc, CDNN_TENSOR_NCHW, CDNN_DATA_FLOAT,
        in_n, in_c, in_h, in_w));
 
  float *in_data;
  CUDA_CALL(dnn->Malloc(
        &in_data, in_n * in_c * in_h * in_w * sizeof(float)));
 
  // filter
  const int filt_k = 1;
  const int filt_c = 1;
  const int filt_h = 2;
  const int filt_w = 2;
  std::cout << "filt_k: " << filt_k << std::endl;
  std::cout << "filt_c: " << filt_c << std::endl;
  std::cout << "filt_h: " << filt_h << std::endl;
  std::cout << "filt_w: " << filt_w << std::endl;
  std::cout << std::endl;
 
  cdnnFilterDescriptor_t filt_desc;
  CDNN_CALL(dnn->CreateFilterDescriptor(&filt_desc));
  CDNN_CALL(dnn->SetFilter4dDescriptor(
        filt_desc, CDNN_DATA_FLOAT, CDNN_TENSOR_NCHW,
        filt_k, filt_c, filt_h, filt_w));
 
  float *filt_data;
  CUDA_CALL(dnn->Malloc(
      &filt_data, filt_k * filt_c * filt_h * filt_w * sizeof(float)));
 
  // convolution
  const int pad_h = 1;
  const int pad_w = 1;
  const int str_h = 1;
  const int str_w = 1;
  const int dil_h = 1;
  const int dil_w = 1;
  std::cout << "pad_h: " << pad_h << std::endl;
  std::cout << "pad_w: " << pad_w << std::endl;
  std::cout << "str_h: " << str_h << std::endl;
  std::cout << "str_w: " << str_w << std::endl;
  std::cout << "dil_h: " << dil_h << std::endl;
  std::cout << "dil_w: " << dil_w << std::endl;
  std::cout << std::endl;
 
  cdnnConvolutionDescriptor_t conv_desc;
  CDNN_CALL(dnn->CreateConvolutionDescriptor(&conv_desc));
  CDNN_CALL(dnn->SetConvolution2dDescriptor(
        conv_desc,
        pad_h, pad_w, str_h, str_w, dil_h, dil_w,
        CDNN_CONVOLUTION, CDNN_DATA_FLOAT));
 
  // output
  int out_n;
  int out_c;
  int out_h;
  int out_w;
   
  CDNN_CALL(dnn->GetConvolution2dForwardOutputDim(
        conv_desc, in_desc, filt_desc,
        &out_n, &out_c, &out_h, &out_w));
 
  std::cout << "out_n: " << out_n << std::endl;
  std::cout << "out_c: " << out_c << std::endl;
  std::cout << "out_h: " << out_h << std::endl;
  std::cout << "out_w: " << out_w << std::endl;
  std::cout << std::endl;
 
  cdnnTensorDescriptor_t out_desc;
  CDNN_CALL(dnn->CreateTensorDescriptor(&out_desc));
  CDNN_CALL(dnn->SetTensor4dDescriptor(
        out_desc, CDNN_TENSOR_NCHW, CDNN_DATA_FLOAT,
        out_n, out_c, out_h, out_w));
 
  float *out_data;
  CUDA_CALL(dnn->Malloc(
        &out_data, out_n * out_c * out_h * out_w * sizeof(float)));
 
  // algorithm
  cdnnConvolutionFwdAlgo_t algo;
  CDNN_CALL(dnn->GetConvolutionForwardAlgorithm(
        cdnn,
        in_desc, filt_desc, conv_desc, out_desc,
        CDNN_CONVOLUTION_FWD_PREFER_FASTEST, 0, &algo));
 
  std::cout << "Convolution algorithm: " << algo << std::endl;
  std::cout << std::endl;
 
  // workspace
  size_t ws_size;
  CDNN_CALL(dnn->GetConvolutionForwardWorkspaceSize(
        cdnn, in_desc, filt_desc, conv_desc, out_desc, algo, &ws_size));
 
  float *ws_data;
  CUDA_CALL(dnn->Malloc(&ws_data, ws_size));
 
  std::cout << "Workspace size: " << ws_size << std::endl;
  std::cout << std::endl;
 
  // perform
  float alpha = 1.f;
  float beta = 0.f;
  dev_iota<<<in_w * in_h, in_n * in_c>>>(in_data);
  dev_const<<<filt_w * filt_h, filt_k * filt_c>>>(filt_data, 1.f);
  CDNN_CALL(dnn->ConvolutionForward(
      cdnn,
      &alpha, in_desc, in_data, filt_desc, filt_data,
      conv_desc, algo, ws_data, ws_size,
      &beta, out_desc, out_data));
 
  // results
  std::cout << "in_data:" << std::endl;
  print(in_data, in_n, in_c, in_h, in_w);
   
  std::cout << "filt_data:" << std::endl;
  print(filt_data, filt_k, filt_c, filt_h, filt_w);
   
  std::cout << "out_data:" << std::endl;
  print(out_data, out_n, out_c, out_h, out_w);
 
  // finalizing
  CUDA_CALL(dnn->Free(ws_data));
  CUDA_CALL(dnn->Free(out_data));
  CDNN_CALL(dnn->DestroyTensorDescriptor(out_desc));
  CDNN_CALL(dnn->DestroyConvolutionDescriptor(conv_desc));
  CUDA_CALL(dnn->Free(filt_data));
  CDNN_CALL(dnn->DestroyFilterDescriptor(filt_desc));
  CUDA_CALL(dnn->Free(in_data));
  CDNN_CALL(dnn->DestroyTensorDescriptor(in_desc));
  CDNN_CALL(dnn->Destroy(cdnn));
  return 0;
}

#if 0
#pragma comment(lib,"cudart.lib")
#pragma comment(lib,"cublas.lib")
#pragma comment(lib,"curand.lib")
#pragma comment(lib,"cudnn.lib")
#endif