
#ifndef _CDNN_CPU_HPP_
#define _CDNN_CPU_HPP_


#include <stdlib.h>
#include <string.h>
#include "cdnn.h"

struct cdnnContextCpu {
	int x;
};
struct cdnnDimCpu {
	int data[CDNN_DIM_MAX];
};
struct cdnnTensorDescriptorCpu {
	cdnnDimCpu size;
	cdnnDimCpu stride;
};

struct CDnnCpu : public CDnn {
	virtual cdnnBrew_t GetBrew() { return CDNN_CPU; }
	virtual cdnnError_t Free(void *devPtr) {
		if (devPtr == NULL) { return cdnnErrorInvalidDevicePointer; }
		free(devPtr);
		return cdnnSuccess;
	}
	virtual cdnnError_t Malloc(void **devPtr, size_t size) {
		if (devPtr == NULL) { return cdnnErrorInvalidValue; }
		if (size == 0) { return cdnnErrorInvalidValue; }
		*devPtr = malloc(size);
		if (*devPtr == 0) { return cdnnErrorMemoryAllocation; }
		return cdnnSuccess;
	}
	virtual cdnnError_t Memcpy(void *dst, const void *src, size_t count, enum cdnnMemcpyKind kind) {
		if (dst == NULL) { return cdnnErrorInvalidValue; }
		memcpy(dst, src, count);
		return cdnnSuccess;
	}

	virtual cdnnStatus_t Create(cdnnHandle_t* handle) {
		*handle = (cdnnHandle_t)malloc(sizeof(cdnnContextCpu));
		if (*handle == NULL) { return CDNN_STATUS_ALLOC_FAILED; }
		return CDNN_STATUS_SUCCESS;
	}
	virtual	cdnnStatus_t Destroy(cdnnHandle_t handle) {
		if (handle == NULL) return CDNN_STATUS_BAD_PARAM;
		free(handle);
		return CDNN_STATUS_SUCCESS;
	}
	virtual cdnnStatus_t CreateTensorDescriptor(cdnnTensorDescriptor_t *tensorDesc) {
		if (tensorDesc == NULL) return CDNN_STATUS_BAD_PARAM;
		cdnnTensorDescriptorCpu* desc = (cdnnTensorDescriptorCpu*)malloc(sizeof(cdnnTensorDescriptorCpu));
		if (desc == NULL) return CDNN_STATUS_BAD_PARAM;
		memset(desc, 0, sizeof(cdnnTensorDescriptorCpu));
		*tensorDesc = (cdnnTensorDescriptor_t)(desc);
		return CDNN_STATUS_SUCCESS;
	}
	virtual cdnnStatus_t DestroyTensorDescriptor(cdnnTensorDescriptor_t tensorDesc) {
		//if (tensorDesc == NULL) return CDNN_STATUS_BAD_PARAM;
		if (tensorDesc) {
			free(tensorDesc);
		}
		return CDNN_STATUS_SUCCESS;
	}
	virtual cdnnStatus_t SetTensor4dDescriptor(cdnnTensorDescriptor_t tensorDesc, cdnnTensorFormat_t format, cdnnDataType_t dataType, int n, int c, int h, int w) {
		cdnnTensorDescriptorCpu* desc = (cdnnTensorDescriptorCpu*)tensorDesc;
		if (n < 0 || c < 0 || h < 0 || w < 0 ) return CDNN_STATUS_BAD_PARAM;
		size_t size = n*c*h*w;
		if (size>INT_MAX) return CDNN_STATUS_NOT_SUPPORTED;
		memset(desc, 0, sizeof(cdnnTensorDescriptorCpu));
		desc->size.data[0] = n;
		desc->size.data[1] = c;
		desc->size.data[2] = h;
		desc->size.data[3] = w;
		return CDNN_STATUS_SUCCESS;
	}
#if 0
	virtual cdnnStatus_t SetTensor4dDescriptorEx(cdnnTensorDescriptor_t tensorDesc, cdnnDataType_t dataType, int n, int c, int h, int w, int nStride, int cStride, int hStride, int wStride);
	virtual cdnnStatus_t GetTensor4dDescriptor(const cdnnTensorDescriptor_t tensorDesc, cdnnDataType_t *dataType, int *n, int *c, int *h, int *w, int *nStride, int *cStride, int *hStride, int *wStride);
	virtual cdnnStatus_t SetTensorNdDescriptor(cdnnTensorDescriptor_t tensorDesc, cdnnDataType_t dataType, int nbDims, const int dimA[], const int strideA[]);
	virtual cdnnStatus_t SetTensorNdDescriptorEx(cdnnTensorDescriptor_t tensorDesc, cdnnTensorFormat_t format, cdnnDataType_t dataType, int nbDims, const int dimA[]);
	virtual cdnnStatus_t GetTensorNdDescriptor(const cdnnTensorDescriptor_t tensorDesc, int nbDimsRequested, cdnnDataType_t *dataType, int *nbDims, int dimA[], int strideA[]);
	virtual cdnnStatus_t GetTensorSizeInBytes(const cdnnTensorDescriptor_t tensorDesc, size_t *size);
#endif

	//Filter
	struct cdnnFilterDescriptorCpu {
		cdnnDimCpu size;
	};
	virtual cdnnStatus_t CreateFilterDescriptor(cdnnFilterDescriptor_t *filterDesc) {
		if (filterDesc == NULL) return CDNN_STATUS_BAD_PARAM;
		cdnnFilterDescriptorCpu* desc = (cdnnFilterDescriptorCpu*)malloc(sizeof(cdnnFilterDescriptorCpu));
		if (desc == NULL) return CDNN_STATUS_BAD_PARAM;
		memset(desc, 0, sizeof(cdnnFilterDescriptorCpu));
		*filterDesc = (cdnnFilterDescriptor_t)(desc);
		return CDNN_STATUS_SUCCESS;
	}
	virtual cdnnStatus_t SetFilter4dDescriptor(cdnnFilterDescriptor_t filterDesc, cdnnDataType_t dataType, cdnnTensorFormat_t format, int k, int c, int h, int w) {
		cdnnFilterDescriptorCpu* desc = (cdnnFilterDescriptorCpu*)filterDesc;
		if (k < 0 || c < 0 || h < 0 || w < 0) return CDNN_STATUS_BAD_PARAM;
		size_t size = k*c*h*w;
		if (size>INT_MAX) return CDNN_STATUS_NOT_SUPPORTED;
		memset(desc, 0, sizeof(cdnnFilterDescriptorCpu));
		desc->size.data[0] = k;
		desc->size.data[1] = c;
		desc->size.data[2] = h;
		desc->size.data[3] = w;
		return CDNN_STATUS_SUCCESS;
	}
	virtual	cdnnStatus_t DestroyFilterDescriptor(cdnnFilterDescriptor_t filterDesc) {
		if (filterDesc) {
			free(filterDesc);
		}
		return CDNN_STATUS_SUCCESS;
	}
	//conv
	struct cdnnConvolutionDescriptorCpu {
		cdnnDimCpu pad;
		cdnnDimCpu stride;
		cdnnDimCpu dilation;
		cdnnConvolutionMode_t mode;
		cdnnDataType_t type;
	};
	virtual cdnnStatus_t CreateConvolutionDescriptor(cdnnConvolutionDescriptor_t *convDesc) {
		cdnnConvolutionDescriptorCpu* desc = (cdnnConvolutionDescriptorCpu*)malloc(sizeof(cdnnConvolutionDescriptor_t));
		if (desc == NULL) return CDNN_STATUS_ALLOC_FAILED;
		memset(desc, 0, sizeof(cdnnConvolutionDescriptorCpu));
		*convDesc = (cdnnConvolutionDescriptor_t)desc;
		return CDNN_STATUS_SUCCESS;
	}
	virtual cdnnStatus_t SetConvolution2dDescriptor(cdnnConvolutionDescriptor_t convDesc, int pad_h, int pad_w, int u, int v, int dilation_h, int dilation_w, cdnnConvolutionMode_t mode, cdnnDataType_t computeType) {
		cdnnConvolutionDescriptorCpu* desc = (cdnnConvolutionDescriptorCpu*)convDesc;
		memset(desc, 0, sizeof(cdnnConvolutionDescriptorCpu));
		desc->pad.data[0] = pad_h;
		desc->pad.data[1] = pad_w;
		desc->stride.data[0] = u;
		desc->stride.data[1] = v;
		desc->dilation.data[0] = dilation_h;
		desc->dilation.data[1] = dilation_w;
		desc->mode = mode;
		desc->type = computeType;
		return CDNN_STATUS_SUCCESS;
	}
	virtual cdnnStatus_t GetConvolution2dForwardOutputDim(const cdnnConvolutionDescriptor_t convDesc, const cdnnTensorDescriptor_t inputTensorDesc, const cdnnFilterDescriptor_t filterDesc, int *n, int *c, int *h, int *w) {

	}
	virtual cdnnStatus_t GetConvolutionForwardAlgorithm(cdnnHandle_t handle, const cdnnTensorDescriptor_t xDesc, const cdnnFilterDescriptor_t wDesc, const cdnnConvolutionDescriptor_t convDesc, const cdnnTensorDescriptor_t yDesc, cdnnConvolutionFwdPreference_t preference, size_t memoryLimitInBytes, cdnnConvolutionFwdAlgo_t *algo) = 0;
	virtual cdnnStatus_t GetConvolutionForwardWorkspaceSize(cdnnHandle_t handle, const cdnnTensorDescriptor_t xDesc, const cdnnFilterDescriptor_t wDesc, const cdnnConvolutionDescriptor_t convDesc, const cdnnTensorDescriptor_t yDesc, cdnnConvolutionFwdAlgo_t algo, size_t *sizeInBytes) = 0;
	virtual cdnnStatus_t ConvolutionForward(cdnnHandle_t handle, const void *alpha, const cdnnTensorDescriptor_t xDesc, const void *x, const cdnnFilterDescriptor_t wDesc, const void *w, const cdnnConvolutionDescriptor_t convDesc, cdnnConvolutionFwdAlgo_t algo, void *workSpace, size_t workSpaceSizeInBytes, const void *beta, const cdnnTensorDescriptor_t yDesc, void *y) = 0;
	virtual cdnnStatus_t DestroyConvolutionDescriptor(cdnnConvolutionDescriptor_t convDesc) = 0;
};


#endif // _CDNN_CPU_HPP_
