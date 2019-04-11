
#ifndef __CSTD_DNN_H__
#define __CSTD_DNN_H__

#include "cdnn.h"

struct IDnn {
	// cudaSuccess, cudaErrorInvalidDevicePointer
	virtual cdnnError_t Free(void *devPtr) = 0;
	// cudaSuccess, cdnnErrorInvalidValue, cdnnErrorMemoryAllocation
	virtual cdnnError_t Malloc(void **devPtr, size_t size) = 0;
	template <typename T>
	cdnnError_t Malloc(T **devPtr, size_t size) {
		return Malloc((void **)devPtr, size);
	}
	virtual cdnnError_t Memcpy(void *dst, const void *src, size_t count, enum cdnnMemcpyKind kind) = 0;
	virtual cdnnError_t Memset(void *devPtr, int value, size_t count) = 0;
	//

	virtual cdnnError_t SetDevice(int device) = 0;
	//
	virtual const char* GetErrorString(cdnnStatus_t) = 0;
	virtual cdnnStatus_t Create(cdnnHandle_t* handle) = 0;
	virtual	cdnnStatus_t Destroy(cdnnHandle_t handle) = 0;
	virtual cdnnStatus_t CreateTensorDescriptor(cdnnTensorDescriptor_t *tensorDesc) = 0;
	virtual cdnnStatus_t DestroyTensorDescriptor(cdnnTensorDescriptor_t tensorDesc) = 0;
	virtual cdnnStatus_t SetTensor4dDescriptor(cdnnTensorDescriptor_t tensorDesc, cdnnTensorFormat_t format, cdnnDataType_t dataType, int n, int c, int h, int w) = 0;
#if 0
	virtual cdnnStatus_t SetTensor4dDescriptorEx(cdnnTensorDescriptor_t tensorDesc, cdnnDataType_t dataType, int n, int c, int h, int w, int nStride, int cStride, int hStride, int wStride);
	virtual cdnnStatus_t GetTensor4dDescriptor(const cdnnTensorDescriptor_t tensorDesc, cdnnDataType_t *dataType, int *n, int *c, int *h, int *w, int *nStride, int *cStride, int *hStride, int *wStride);
	virtual cdnnStatus_t SetTensorNdDescriptor(cdnnTensorDescriptor_t tensorDesc, cdnnDataType_t dataType, int nbDims, const int dimA[], const int strideA[]);
	virtual cdnnStatus_t SetTensorNdDescriptorEx(cdnnTensorDescriptor_t tensorDesc, cdnnTensorFormat_t format, cdnnDataType_t dataType, int nbDims, const int dimA[]);
	virtual cdnnStatus_t GetTensorNdDescriptor(const cdnnTensorDescriptor_t tensorDesc, int nbDimsRequested, cdnnDataType_t *dataType, int *nbDims, int dimA[], int strideA[]);
	virtual cdnnStatus_t GetTensorSizeInBytes(const cdnnTensorDescriptor_t tensorDesc, size_t *size);
#endif

	//conv
	virtual cdnnStatus_t CreateFilterDescriptor(cdnnFilterDescriptor_t *filterDesc) = 0;
	virtual cdnnStatus_t SetFilter4dDescriptor(cdnnFilterDescriptor_t filterDesc, cdnnDataType_t dataType, cdnnTensorFormat_t format, int k, int c, int h, int w) = 0;
	virtual	cdnnStatus_t DestroyFilterDescriptor(cdnnFilterDescriptor_t filterDesc) = 0;
	virtual cdnnStatus_t CreateConvolutionDescriptor(cdnnConvolutionDescriptor_t *convDesc) = 0;
	virtual cdnnStatus_t SetConvolution2dDescriptor(cdnnConvolutionDescriptor_t convDesc, int pad_h, int pad_w, int u, int v, int dilation_h, int dilation_w, cdnnConvolutionMode_t mode, cdnnDataType_t computeType) = 0;
	virtual cdnnStatus_t GetConvolution2dForwardOutputDim(const cdnnConvolutionDescriptor_t convDesc, const cdnnTensorDescriptor_t inputTensorDesc, const cdnnFilterDescriptor_t filterDesc, int *n, int *c, int *h, int *w) = 0;
	virtual cdnnStatus_t GetConvolutionForwardAlgorithm(cdnnHandle_t handle, const cdnnTensorDescriptor_t xDesc, const cdnnFilterDescriptor_t wDesc, const cdnnConvolutionDescriptor_t convDesc, const cdnnTensorDescriptor_t yDesc, cdnnConvolutionFwdPreference_t preference, size_t memoryLimitInBytes, cdnnConvolutionFwdAlgo_t *algo) = 0;
	virtual cdnnStatus_t GetConvolutionForwardWorkspaceSize(cdnnHandle_t handle, const cdnnTensorDescriptor_t xDesc, const cdnnFilterDescriptor_t wDesc, const cdnnConvolutionDescriptor_t convDesc, const cdnnTensorDescriptor_t yDesc, cdnnConvolutionFwdAlgo_t algo, size_t *sizeInBytes) = 0;
	virtual cdnnStatus_t ConvolutionForward(cdnnHandle_t handle, const void *alpha, const cdnnTensorDescriptor_t xDesc, const void *x, const cdnnFilterDescriptor_t wDesc, const void *w, const cdnnConvolutionDescriptor_t convDesc, cdnnConvolutionFwdAlgo_t algo, void *workSpace, size_t workSpaceSizeInBytes, const void *beta, const cdnnTensorDescriptor_t yDesc, void *y) = 0;
	virtual cdnnStatus_t DestroyConvolutionDescriptor(cdnnConvolutionDescriptor_t convDesc) = 0;
	//
	virtual	cdnnStatus_t CreateActivationDescriptor(cdnnActivationDescriptor_t *activationDesc) = 0;
	virtual cdnnStatus_t SetActivationDescriptor(cdnnActivationDescriptor_t activationDesc, cdnnActivationMode_t mode, cdnnNanPropagation_t reluNanOpt, double coef) = 0;
	virtual	cdnnStatus_t ActivationForward(cdnnHandle_t handle, cdnnActivationDescriptor_t activationDesc, const void *alpha, const cdnnTensorDescriptor_t xDesc, const void *x, const void *beta, const cdnnTensorDescriptor_t yDesc, void *y) = 0;
	virtual cdnnStatus_t DestroyActivationDescriptor(cdnnActivationDescriptor_t activationDesc) = 0;
};

IDnn* GetDnnCpu();
IDnn* GetDnnCuda();


#endif // __CSTD_DNN_H__
