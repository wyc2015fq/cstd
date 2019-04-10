

#ifndef __CSTD_CDNN_H__
#define __CSTD_CDNN_H__

struct cdnnContext;
typedef struct cdnnContext *cdnnHandle_t;

enum cdnnError_t {
	cdnnSuccess = 0,
	cdnnErrorMissingConfiguration = 1,
	cdnnErrorMemoryAllocation = 2,
	cdnnErrorInitializationError = 3,
	cdnnErrorLaunchFailure = 4,
	cdnnErrorPriorLaunchFailure = 5,
	cdnnErrorLaunchTimeout = 6,
	cdnnErrorLaunchOutOfResources = 7,
	cdnnErrorInvalidDeviceFunction = 8,
	cdnnErrorInvalidConfiguration = 9,
	cdnnErrorInvalidDevice = 10,
	cdnnErrorInvalidValue = 11,
	cdnnErrorInvalidPitchValue = 12,
	cdnnErrorInvalidSymbol = 13,
	cdnnErrorMapBufferObjectFailed = 14,
	cdnnErrorUnmapBufferObjectFailed = 15,
	cdnnErrorInvalidHostPointer = 16,
	cdnnErrorInvalidDevicePointer = 17,
	cdnnErrorInvalidTexture = 18,
	cdnnErrorInvalidTextureBinding = 19,
	cdnnErrorInvalidChannelDescriptor = 20,
	cdnnErrorInvalidMemcpyDirection = 21,
	cdnnErrorAddressOfConstant = 22,
	cdnnErrorTextureFetchFailed = 23,
	cdnnErrorTextureNotBound = 24,
	cdnnErrorSynchronizationError = 25,
	cdnnErrorInvalidFilterSetting = 26,
	cdnnErrorInvalidNormSetting = 27,
	cdnnErrorMixedDeviceExecution = 28,
	cdnnErrorcdnnrtUnloading = 29,
	cdnnErrorUnknown = 30,
	cdnnErrorNotYetImplemented = 31,
	cdnnErrorMemoryValueTooLarge = 32,
	cdnnErrorInvalidResourceHandle = 33,
	cdnnErrorNotReady = 34,
	cdnnErrorInsufficientDriver = 35,
	cdnnErrorSetOnActiveProcess = 36,
	cdnnErrorInvalidSurface = 37,
	cdnnErrorNoDevice = 38,
	cdnnErrorECCUncorrectable = 39,
	cdnnErrorSharedObjectSymbolNotFound = 40,
	cdnnErrorSharedObjectInitFailed = 41,
	cdnnErrorUnsupportedLimit = 42,
	cdnnErrorDuplicateVariableName = 43,
	cdnnErrorDuplicateTextureName = 44,
	cdnnErrorDuplicateSurfaceName = 45,
	cdnnErrorDevicesUnavailable = 46,
	cdnnErrorInvalidKernelImage = 47,
	cdnnErrorNoKernelImageForDevice = 48,
	cdnnErrorIncompatibleDriverContext = 49,
	cdnnErrorPeerAccessAlreadyEnabled = 50,
	cdnnErrorPeerAccessNotEnabled = 51,
	cdnnErrorDeviceAlreadyInUse = 54,
	cdnnErrorProfilerDisabled = 55,
	cdnnErrorProfilerNotInitialized = 56,
	cdnnErrorProfilerAlreadyStarted = 57,
	cdnnErrorProfilerAlreadyStopped = 58,
	cdnnErrorAssert = 59,
	cdnnErrorTooManyPeers = 60,
	cdnnErrorHostMemoryAlreadyRegistered = 61,
	cdnnErrorHostMemoryNotRegistered = 62,
	cdnnErrorOperatingSystem = 63,
	cdnnErrorPeerAccessUnsupported = 64,
	cdnnErrorLaunchMaxDepthExceeded = 65,
	cdnnErrorLaunchFileScopedTex = 66,
	cdnnErrorLaunchFileScopedSurf = 67,
	cdnnErrorSyncDepthExceeded = 68,
	cdnnErrorLaunchPendingCountExceeded = 69,
	cdnnErrorNotPermitted = 70,
	cdnnErrorNotSupported = 71,
	cdnnErrorHardwareStackError = 72,
	cdnnErrorIllegalInstruction = 73,
	cdnnErrorMisalignedAddress = 74,
	cdnnErrorInvalidAddressSpace = 75,
	cdnnErrorInvalidPc = 76,
	cdnnErrorIllegalAddress = 77,
	cdnnErrorInvalidPtx = 78,
	cdnnErrorInvalidGraphicsContext = 79,
	cdnnErrorNvlinkUncorrectable = 80,
	cdnnErrorJitCompilerNotFound = 81,
	cdnnErrorCooperativeLaunchTooLarge = 82,
	cdnnErrorStartupFailure = 0x7f,
	cdnnErrorApiFailureBase = 10000
};

typedef enum
{
	CDNN_STATUS_SUCCESS = 0,
	CDNN_STATUS_NOT_INITIALIZED = 1,
	CDNN_STATUS_ALLOC_FAILED = 2,
	CDNN_STATUS_BAD_PARAM = 3,
	CDNN_STATUS_INTERNAL_ERROR = 4,
	CDNN_STATUS_INVALID_VALUE = 5,
	CDNN_STATUS_ARCH_MISMATCH = 6,
	CDNN_STATUS_MAPPING_ERROR = 7,
	CDNN_STATUS_EXECUTION_FAILED = 8,
	CDNN_STATUS_NOT_SUPPORTED = 9,
	CDNN_STATUS_LICENSE_ERROR = 10,
	CDNN_STATUS_RUNTIME_PREREQUISITE_MISSING = 11,
	CDNN_STATUS_RUNTIME_IN_PROGRESS = 12,
	CDNN_STATUS_RUNTIME_FP_OVERFLOW = 13,
} cdnnStatus_t;

enum cdnnMemoryType
{
	cdnnMemoryTypeHost = 1, /**< Host memory */
	cdnnMemoryTypeDevice = 2  /**< Device memory */
};

enum cdnnMemcpyKind
{
	cdnnMemcpyHostToHost = 0,      /**< Host   -> Host */
	cdnnMemcpyHostToDevice = 1,      /**< Host   -> Device */
	cdnnMemcpyDeviceToHost = 2,      /**< Device -> Host */
	cdnnMemcpyDeviceToDevice = 3,      /**< Device -> Device */
	cdnnMemcpyDefault = 4
};

typedef enum
{
	CDNN_TENSOR_NCHW = 0,          /* row major (wStride = 1, hStride = w) */
	CDNN_TENSOR_NHWC = 1,          /* feature maps interleaved ( cStride = 1 )*/
	CDNN_TENSOR_NCHW_VECT_C = 2    /* each image point is vector of element of C : the length of the vector is carried by the data type*/
} cdnnTensorFormat_t;

typedef enum
{
	CDNN_DATA_FLOAT = 0,
	CDNN_DATA_DOUBLE = 1,
	CDNN_DATA_HALF = 2,
	CDNN_DATA_INT8 = 3,
	CDNN_DATA_INT32 = 4,
	CDNN_DATA_INT8x4 = 5,
	CDNN_DATA_UINT8 = 6,
	CDNN_DATA_UINT8x4 = 7
} cdnnDataType_t;

typedef enum
{
	CDNN_CONVOLUTION = 0,
	CDNN_CROSS_CORRELATION = 1
} cdnnConvolutionMode_t;

typedef enum
{
	CDNN_CONVOLUTION_FWD_NO_WORKSPACE = 0,
	CDNN_CONVOLUTION_FWD_PREFER_FASTEST = 1,
	CDNN_CONVOLUTION_FWD_SPECIFY_WORKSPACE_LIMIT = 2,
} cdnnConvolutionFwdPreference_t;

typedef enum
{
	CDNN_CONVOLUTION_FWD_ALGO_IMPLICIT_GEMM = 0,
	CDNN_CONVOLUTION_FWD_ALGO_IMPLICIT_PRECOMP_GEMM = 1,
	CDNN_CONVOLUTION_FWD_ALGO_GEMM = 2,
	CDNN_CONVOLUTION_FWD_ALGO_DIRECT = 3,
	CDNN_CONVOLUTION_FWD_ALGO_FFT = 4,
	CDNN_CONVOLUTION_FWD_ALGO_FFT_TILING = 5,
	CDNN_CONVOLUTION_FWD_ALGO_WINOGRAD = 6,
	CDNN_CONVOLUTION_FWD_ALGO_WINOGRAD_NONFUSED = 7,
	CDNN_CONVOLUTION_FWD_ALGO_COUNT = 8
} cdnnConvolutionFwdAlgo_t;

#define CDNN_DIM_MAX 8

enum cdnnBrew_t { CDNN_CPU=0, CDNN_GPU=1 };

typedef struct cdnnTensorStruct*             cdnnTensorDescriptor_t;
typedef struct cdnnConvolutionStruct*        cdnnConvolutionDescriptor_t;
typedef struct cdnnPoolingStruct*            cdnnPoolingDescriptor_t;
typedef struct cdnnFilterStruct*             cdnnFilterDescriptor_t;
typedef struct cdnnLRNStruct*                cdnnLRNDescriptor_t;
typedef struct cdnnActivationStruct*         cdnnActivationDescriptor_t;
typedef struct cdnnSpatialTransformerStruct* cdnnSpatialTransformerDescriptor_t;
typedef struct cdnnOpTensorStruct*           cdnnOpTensorDescriptor_t;
typedef struct cdnnReduceTensorStruct*       cdnnReduceTensorDescriptor_t;
typedef struct cdnnCTCLossStruct*            cdnnCTCLossDescriptor_t;

struct CDnn {
	virtual cdnnBrew_t GetBrew() = 0;
	// cudaSuccess, cudaErrorInvalidDevicePointer
	virtual cdnnError_t Free(void *devPtr) = 0;
	// cudaSuccess, cdnnErrorInvalidValue, cdnnErrorMemoryAllocation
	virtual cdnnError_t Malloc(void **devPtr, size_t size) = 0;
	template <typename T>
	cdnnError_t Malloc(T **devPtr, size_t size) {
		return Malloc((void **)devPtr, size);
	}
	virtual cdnnError_t Memcpy(void *dst, const void *src, size_t count, enum cdnnMemcpyKind kind) = 0;

	virtual cdnnStatus_t Create(cdnnHandle_t* handle) = 0;
	virtual	cdnnStatus_t Destroy(cdnnHandle_t handle) = 0;
	virtual cdnnStatus_t CreateTensorDescriptor(cdnnTensorDescriptor_t *tensorDesc) = 0;
	virtual cdnnStatus_t DestroyTensorDescriptor(cdnnTensorDescriptor_t tensorDesc);
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
};

CDnn* GetCDnn(cdnnBrew_t brew);

#endif // __CSTD_CDNN_H__
