
#ifndef _CDNN_CPU_HPP_
#define _CDNN_CPU_HPP_


#include <stdlib.h>
#include <string.h>
#include "dnn.h"
#include "cdnn_def.h"
#include "myblas.h"
#define cpu_sgemm my_sgemm
#define cpu_dgemm my_dgemm

#define Stype double
#define _MATH_FUNCTIONS_TYPE_CPU_GPU_DEF(DEF)  \
DEF(cdnnStatus_t, ConvolutionForward, (cdnnHandle_t handle, const void *alpha, const cdnnTensorDescriptor_t xDesc, const void *x, const cdnnFilterDescriptor_t wDesc, const void *w, const cdnnConvolutionDescriptor_t convDesc, cdnnConvolutionFwdAlgo_t algo, void *workSpace, size_t workSpaceSizeInBytes, const void *beta, const cdnnTensorDescriptor_t yDesc, void *y))

#define FUN(fun)  cpu_s##fun
#define Dtype float
#include "cdnn.inl"
#undef Dtype
#undef FUN

#if 0
#define FUN(fun)  cpu_d##fun
#define Dtype double
#define Stype double
#include "cdnn.inl"
#undef Dtype
#undef FUN
#endif

#define DEF(RET, NAME, ARGS)  typedef RET (*NAME##_func_t)ARGS;
_MATH_FUNCTIONS_TYPE_CPU_GPU_DEF(DEF)
#undef DEF

struct dnn_cpu_func_table_t {
#define DEF(RET, NAME, ARGS)  NAME##_func_t NAME;
_MATH_FUNCTIONS_TYPE_CPU_GPU_DEF(DEF)
#undef DEF
};

dnn_cpu_func_table_t* dnn_cpu_func_get(cdnnDataType_t type) {
	static int inited = 0;
	static dnn_cpu_func_table_t func_table[8] = { 0 };
	if (0 == inited) {
		inited = 1;
#define DEF(RET, NAME, ARGS)  func_table[CDNN_DATA_FLOAT].NAME = (NAME##_func_t)cpu_s ## NAME;
		_MATH_FUNCTIONS_TYPE_CPU_GPU_DEF(DEF)
#undef DEF
	}
	return func_table+ type;
}

struct DnnCpu : public IDnn {
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
	virtual cdnnError_t Memset(void *devPtr, int value, size_t count) {
		if (devPtr == NULL) { return cdnnErrorInvalidValue; }
		memset(devPtr, value, count);
		return cdnnSuccess;
	}

	virtual cdnnError_t SetDevice(int device) {
		return cdnnSuccess;
	}
	//
	virtual const char* GetErrorString(cdnnStatus_t) { return ""; }
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
		desc->format = format;
		desc->data_type = dataType;
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
		desc->compute_type = computeType;
		desc->num_spatial_axes_ = 2;
		return CDNN_STATUS_SUCCESS;
	}
	virtual cdnnStatus_t GetConvolution2dForwardOutputDim(const cdnnConvolutionDescriptor_t convDesc, const cdnnTensorDescriptor_t inputTensorDesc, const cdnnFilterDescriptor_t filterDesc, int *n, int *c, int *h, int *w) {
		cdnnTensorDescriptorCpu* input_desc = (cdnnTensorDescriptorCpu*)inputTensorDesc;
		cdnnConvolutionDescriptorCpu* conv_desc = (cdnnConvolutionDescriptorCpu*)convDesc;
		cdnnFilterDescriptorCpu* filter_desc = (cdnnFilterDescriptorCpu*)filterDesc;
		cdnnDimCpu out = {0};
		const int* input_shape_data = input_desc->size.data;
		const int* kernel_shape_data = filter_desc->size.data;
		const int* stride_data = conv_desc->stride.data;
		const int* pad_data = conv_desc->pad.data;
		const int* dilation_data = conv_desc->dilation.data;
		if (input_shape_data[1] != kernel_shape_data[1]) return CDNN_STATUS_BAD_PARAM;
		out.data[0] = input_shape_data[0];
		out.data[1] = kernel_shape_data[0];
		// outputDim = 1 + ( inputDim + 2*pad - (((filterDim-1)*dilation)+1) )/convolutionStride;
		for (int i = 0; i < conv_desc->num_spatial_axes_; ++i) {
			// i + 1 to skip channel axis
			const int input_dim = input_shape_data[i+2];
			const int kernel_extent = dilation_data[i] * (kernel_shape_data[i+2] - 1) + 1;
			const int output_dim = (input_dim + 2 * pad_data[i] - kernel_extent) / stride_data[i] + 1;
			out.data[i+2] = (output_dim);
		}
		if (n) *n = out.data[0];
		if (c) *c = out.data[1];
		if (h) *h = out.data[2];
		if (w) *w = out.data[3];
		return CDNN_STATUS_SUCCESS;
	}
	virtual cdnnStatus_t GetConvolutionForwardAlgorithm(cdnnHandle_t handle, const cdnnTensorDescriptor_t xDesc, const cdnnFilterDescriptor_t wDesc, const cdnnConvolutionDescriptor_t convDesc, const cdnnTensorDescriptor_t yDesc, cdnnConvolutionFwdPreference_t preference, size_t memoryLimitInBytes, cdnnConvolutionFwdAlgo_t *algo) {
		*algo = CDNN_CONVOLUTION_FWD_ALGO_GEMM;
		return CDNN_STATUS_SUCCESS;
	}
	virtual cdnnStatus_t GetConvolutionForwardWorkspaceSize(cdnnHandle_t handle, const cdnnTensorDescriptor_t xDesc, const cdnnFilterDescriptor_t wDesc, const cdnnConvolutionDescriptor_t convDesc, const cdnnTensorDescriptor_t yDesc, cdnnConvolutionFwdAlgo_t algo, size_t *sizeInBytes) {
		*sizeInBytes = 100;
		return CDNN_STATUS_SUCCESS;
	}
	virtual cdnnStatus_t ConvolutionForward(cdnnHandle_t handle, const void *alpha, const cdnnTensorDescriptor_t xDesc, const void *x, const cdnnFilterDescriptor_t wDesc, const void *w, const cdnnConvolutionDescriptor_t convDesc, cdnnConvolutionFwdAlgo_t algo, void *workSpace, size_t workSpaceSizeInBytes, const void *beta, const cdnnTensorDescriptor_t yDesc, void *y) {
		//cdnnTensorDescriptorCpu* x_desc = (cdnnTensorDescriptorCpu*)xDesc;
		//cdnnTensorDescriptorCpu* y_desc = (cdnnTensorDescriptorCpu*)yDesc;
		//cdnnFilterDescriptorCpu* w_desc = (cdnnFilterDescriptorCpu*)wDesc;
		cdnnConvolutionDescriptorCpu* conv_desc = (cdnnConvolutionDescriptorCpu*)convDesc;
		conv_desc->num_spatial_axes_;
		dnn_cpu_func_get(conv_desc->compute_type)->ConvolutionForward(handle, alpha, xDesc, x, wDesc, w, convDesc, algo, workSpace, workSpaceSizeInBytes, beta, yDesc, y);
		return CDNN_STATUS_SUCCESS;
	}
	virtual cdnnStatus_t DestroyConvolutionDescriptor(cdnnConvolutionDescriptor_t convDesc) {
		free(convDesc);
		return CDNN_STATUS_SUCCESS;
	}
	//
	struct cdnnActivationDescriptorCpu {
		cdnnActivationMode_t mode;
		cdnnNanPropagation_t reluNanOpt;
		double coef;
	};
	virtual	cdnnStatus_t CreateActivationDescriptor(cdnnActivationDescriptor_t *activationDesc) {
		cdnnActivationDescriptorCpu* desc = (cdnnActivationDescriptorCpu*)malloc(sizeof(cdnnActivationDescriptorCpu));
		if (desc == NULL) return CDNN_STATUS_ALLOC_FAILED;
		memset(desc, 0, sizeof(cdnnActivationDescriptorCpu));
		*activationDesc = (cdnnActivationDescriptor_t)desc;
		return CDNN_STATUS_SUCCESS;
	}
	virtual cdnnStatus_t SetActivationDescriptor(cdnnActivationDescriptor_t activationDesc, cdnnActivationMode_t mode, cdnnNanPropagation_t reluNanOpt, double coef) {
		if (activationDesc == NULL) return CDNN_STATUS_ALLOC_FAILED;
		cdnnActivationDescriptorCpu* desc = (cdnnActivationDescriptorCpu*)activationDesc;
		return CDNN_STATUS_SUCCESS;
	}
	virtual	cdnnStatus_t ActivationForward(cdnnHandle_t handle, cdnnActivationDescriptor_t activationDesc, const void *alpha, const cdnnTensorDescriptor_t xDesc, const void *x, const void *beta, const cdnnTensorDescriptor_t yDesc, void *y) {
		return CDNN_STATUS_SUCCESS;
	}
	virtual cdnnStatus_t DestroyActivationDescriptor(cdnnActivationDescriptor_t activationDesc) {
		free(activationDesc);
		return CDNN_STATUS_SUCCESS;
	}
};


/////////////


IDnn* GetDnnCpu() {
	return new DnnCpu();
}



#endif // _CDNN_CPU_HPP_
