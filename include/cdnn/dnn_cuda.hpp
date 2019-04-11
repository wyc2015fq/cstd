
#ifndef _CDNN_CUDA_HPP_
#define _CDNN_CUDA_HPP_


#include "dnn.h"
//#include <cuda.h>
#include <cudnn.h>
#include <stdlib.h>
#include <string.h>

struct DnnCuda : public IDnn {
	virtual cdnnError_t Free(void *devPtr) {
		return (cdnnError_t)cudaFree(devPtr);
	}
	virtual cdnnError_t Malloc(void **devPtr, size_t size) {
		return (cdnnError_t)cudaMalloc(devPtr, size);
	}
	virtual cdnnError_t Memcpy(void *dst, const void *src, size_t count, enum cdnnMemcpyKind kind) {
		return (cdnnError_t)cudaMemcpy(dst, src, count, (enum cudaMemcpyKind)kind);
	}
	virtual cdnnError_t Memset(void *devPtr, int value, size_t count) {
		return (cdnnError_t)cudaMemset(devPtr, value, count);
	}

	virtual cdnnError_t SetDevice(int device) {
		return (cdnnError_t)cudaSetDevice(device);
	}
	//
	virtual const char* GetErrorString(cdnnStatus_t status) {
		return cudnnGetErrorString((cudnnStatus_t)status);
	}
	virtual cdnnStatus_t Create(cdnnHandle_t* handle) {
		return (cdnnStatus_t)cudnnCreate((cudnnHandle_t*)handle);
	}
	virtual	cdnnStatus_t Destroy(cdnnHandle_t handle) {
		return (cdnnStatus_t)cudnnDestroy((cudnnHandle_t)handle);
	}
	virtual cdnnStatus_t CreateTensorDescriptor(cdnnTensorDescriptor_t *tensorDesc) {
		return (cdnnStatus_t)cudnnCreateTensorDescriptor((cudnnTensorDescriptor_t*)tensorDesc);
	}
	virtual cdnnStatus_t DestroyTensorDescriptor(cdnnTensorDescriptor_t tensorDesc) {
		return (cdnnStatus_t)cudnnDestroyTensorDescriptor((cudnnTensorDescriptor_t)tensorDesc);
	}
	virtual cdnnStatus_t SetTensor4dDescriptor(cdnnTensorDescriptor_t tensorDesc, cdnnTensorFormat_t format, cdnnDataType_t dataType, int n, int c, int h, int w) {
		return (cdnnStatus_t)cudnnSetTensor4dDescriptor((cudnnTensorDescriptor_t)tensorDesc, (cudnnTensorFormat_t)format, (cudnnDataType_t)dataType, n, c, h, w);
	}
#if 0
	virtual cdnnStatus_t SetTensor4dDescriptorEx(cdnnTensorDescriptor_t tensorDesc, cdnnDataType_t dataType, int n, int c, int h, int w, int nStride, int cStride, int hStride, int wStride);
	virtual cdnnStatus_t GetTensor4dDescriptor(const cdnnTensorDescriptor_t tensorDesc, cdnnDataType_t *dataType, int *n, int *c, int *h, int *w, int *nStride, int *cStride, int *hStride, int *wStride);
	virtual cdnnStatus_t SetTensorNdDescriptor(cdnnTensorDescriptor_t tensorDesc, cdnnDataType_t dataType, int nbDims, const int dimA[], const int strideA[]);
	virtual cdnnStatus_t SetTensorNdDescriptorEx(cdnnTensorDescriptor_t tensorDesc, cdnnTensorFormat_t format, cdnnDataType_t dataType, int nbDims, const int dimA[]);
	virtual cdnnStatus_t GetTensorNdDescriptor(const cdnnTensorDescriptor_t tensorDesc, int nbDimsRequested, cdnnDataType_t *dataType, int *nbDims, int dimA[], int strideA[]);
	virtual cdnnStatus_t GetTensorSizeInBytes(const cdnnTensorDescriptor_t tensorDesc, size_t *size);
#endif

	virtual cdnnStatus_t CreateFilterDescriptor(cdnnFilterDescriptor_t *filterDesc) {
		return (cdnnStatus_t)cudnnCreateFilterDescriptor((cudnnFilterDescriptor_t*)filterDesc);
	}
	virtual cdnnStatus_t SetFilter4dDescriptor(cdnnFilterDescriptor_t filterDesc, cdnnDataType_t dataType, cdnnTensorFormat_t format, int k, int c, int h, int w) {
		return (cdnnStatus_t)cudnnSetFilter4dDescriptor((cudnnFilterDescriptor_t)filterDesc, (cudnnDataType_t)dataType, (cudnnTensorFormat_t)format, k, c, h, w);
	}
	virtual	cdnnStatus_t DestroyFilterDescriptor(cdnnFilterDescriptor_t filterDesc) {
		return (cdnnStatus_t)cudnnDestroyFilterDescriptor((cudnnFilterDescriptor_t)filterDesc);
	}
	//conv
	virtual cdnnStatus_t CreateConvolutionDescriptor(cdnnConvolutionDescriptor_t *convDesc) {
		return (cdnnStatus_t)cudnnCreateConvolutionDescriptor((cudnnConvolutionDescriptor_t*)convDesc);
	}
	virtual cdnnStatus_t SetConvolution2dDescriptor(cdnnConvolutionDescriptor_t convDesc, int pad_h, int pad_w, int u, int v, int dilation_h, int dilation_w, cdnnConvolutionMode_t mode, cdnnDataType_t computeType) {
		return (cdnnStatus_t)cudnnSetConvolution2dDescriptor((cudnnConvolutionDescriptor_t)convDesc, pad_h, pad_w, u, v, dilation_h, dilation_w, (cudnnConvolutionMode_t)mode, (cudnnDataType_t)computeType);
	}
	virtual cdnnStatus_t GetConvolution2dForwardOutputDim(const cdnnConvolutionDescriptor_t convDesc, const cdnnTensorDescriptor_t inputTensorDesc, const cdnnFilterDescriptor_t filterDesc, int *n, int *c, int *h, int *w) {
		return (cdnnStatus_t)cudnnGetConvolution2dForwardOutputDim((const cudnnConvolutionDescriptor_t)convDesc, (const cudnnTensorDescriptor_t)inputTensorDesc, (const cudnnFilterDescriptor_t)filterDesc, n, c, h, w);
	}
	virtual cdnnStatus_t GetConvolutionForwardAlgorithm(cdnnHandle_t handle, const cdnnTensorDescriptor_t xDesc, const cdnnFilterDescriptor_t wDesc, const cdnnConvolutionDescriptor_t convDesc, const cdnnTensorDescriptor_t yDesc, cdnnConvolutionFwdPreference_t preference, size_t memoryLimitInBytes, cdnnConvolutionFwdAlgo_t *algo) {
		return (cdnnStatus_t)cudnnGetConvolutionForwardAlgorithm((cudnnHandle_t)handle, (const cudnnTensorDescriptor_t)xDesc, (const cudnnFilterDescriptor_t)wDesc, (const cudnnConvolutionDescriptor_t)convDesc, (const cudnnTensorDescriptor_t)yDesc, (cudnnConvolutionFwdPreference_t)preference, memoryLimitInBytes, (cudnnConvolutionFwdAlgo_t*)algo);
	}
	virtual cdnnStatus_t GetConvolutionForwardWorkspaceSize(cdnnHandle_t handle, const cdnnTensorDescriptor_t xDesc, const cdnnFilterDescriptor_t wDesc, const cdnnConvolutionDescriptor_t convDesc, const cdnnTensorDescriptor_t yDesc, cdnnConvolutionFwdAlgo_t algo, size_t *sizeInBytes) {
		return (cdnnStatus_t)cudnnGetConvolutionForwardWorkspaceSize((cudnnHandle_t)handle, (const cudnnTensorDescriptor_t)xDesc, (const cudnnFilterDescriptor_t)wDesc, (const cudnnConvolutionDescriptor_t)convDesc, (const cudnnTensorDescriptor_t)yDesc, (cudnnConvolutionFwdAlgo_t)algo, sizeInBytes);
	}
	virtual cdnnStatus_t ConvolutionForward(cdnnHandle_t handle, const void *alpha, const cdnnTensorDescriptor_t xDesc, const void *x, const cdnnFilterDescriptor_t wDesc, const void *w, const cdnnConvolutionDescriptor_t convDesc, cdnnConvolutionFwdAlgo_t algo, void *workSpace, size_t workSpaceSizeInBytes, const void *beta, const cdnnTensorDescriptor_t yDesc, void *y) {
		return (cdnnStatus_t)cudnnConvolutionForward((cudnnHandle_t)handle, alpha, (const cudnnTensorDescriptor_t)xDesc, x, (const cudnnFilterDescriptor_t)wDesc, w, (const cudnnConvolutionDescriptor_t)convDesc, (cudnnConvolutionFwdAlgo_t)algo, workSpace, workSpaceSizeInBytes, beta, (const cudnnTensorDescriptor_t)yDesc, y);
	}
	virtual cdnnStatus_t DestroyConvolutionDescriptor(cdnnConvolutionDescriptor_t convDesc) {
		return (cdnnStatus_t)cudnnDestroyConvolutionDescriptor((cudnnConvolutionDescriptor_t)convDesc);
	}
	//
	virtual	cdnnStatus_t CreateActivationDescriptor(cdnnActivationDescriptor_t *activationDesc) {
		return (cdnnStatus_t)cudnnCreateActivationDescriptor((cudnnActivationDescriptor_t*)activationDesc);
	}
	virtual cdnnStatus_t SetActivationDescriptor(cdnnActivationDescriptor_t activationDesc, cdnnActivationMode_t mode, cdnnNanPropagation_t reluNanOpt, double coef) {
		return (cdnnStatus_t)cudnnSetActivationDescriptor((cudnnActivationDescriptor_t)activationDesc, (cudnnActivationMode_t)mode, (cudnnNanPropagation_t)reluNanOpt, coef);
	}
	virtual	cdnnStatus_t ActivationForward(cdnnHandle_t handle, cdnnActivationDescriptor_t activationDesc, const void *alpha, const cdnnTensorDescriptor_t xDesc, const void *x, const void *beta, const cdnnTensorDescriptor_t yDesc, void *y) {
		return (cdnnStatus_t)cudnnActivationForward((cudnnHandle_t)handle, (cudnnActivationDescriptor_t)activationDesc, alpha, (const cudnnTensorDescriptor_t)xDesc, x, beta, (const cudnnTensorDescriptor_t)yDesc, y);
	}
	virtual cdnnStatus_t ActivationBackward(cdnnHandle_t handle, cdnnActivationDescriptor_t activationDesc, const void *alpha, const cdnnTensorDescriptor_t yDesc, const void *y, const cdnnTensorDescriptor_t dyDesc, const void *dy, const cdnnTensorDescriptor_t xDesc, const void *x, const void *beta, const cdnnTensorDescriptor_t dxDesc, void *dx) {
		return (cdnnStatus_t)cudnnActivationBackward((cudnnHandle_t)handle, (cudnnActivationDescriptor_t)activationDesc, alpha, (const cudnnTensorDescriptor_t)yDesc, y, (const cudnnTensorDescriptor_t)dyDesc, dy, (const cudnnTensorDescriptor_t)xDesc, x, beta, (const cudnnTensorDescriptor_t)dxDesc, dx);
	}
	virtual cdnnStatus_t DestroyActivationDescriptor(cdnnActivationDescriptor_t activationDesc) {
		return (cdnnStatus_t)cudnnDestroyActivationDescriptor((cudnnActivationDescriptor_t)activationDesc);
	}
};


/////////////

IDnn* GetDnnCuda() {
	static DnnCuda dnn_cuda;
	return &dnn_cuda;
}





#endif // _CDNN_CUDA_HPP_
