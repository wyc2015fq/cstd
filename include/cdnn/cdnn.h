

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

typedef enum {
	CDNN_DEFAULT_MATH = 0,
	CDNN_TENSOR_OP_MATH = 1,
} cdnnMathType_t;

typedef enum {
	CDNN_NOT_PROPAGATE_NAN = 0,
	CDNN_PROPAGATE_NAN = 1,
} cdnnNanPropagation_t;

typedef enum
{
	CDNN_NON_DETERMINISTIC = 0,
	CDNN_DETERMINISTIC = 1,
} cdnnDeterminism_t;

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

/* helper function to provide the convolution algo that fit best the requirement */
typedef enum
{
	CDNN_CONVOLUTION_BWD_FILTER_NO_WORKSPACE = 0,
	CDNN_CONVOLUTION_BWD_FILTER_PREFER_FASTEST = 1,
	CDNN_CONVOLUTION_BWD_FILTER_SPECIFY_WORKSPACE_LIMIT = 2,
} cdnnConvolutionBwdFilterPreference_t;

typedef enum
{
	CDNN_CONVOLUTION_BWD_FILTER_ALGO_0 = 0,  /* non-deterministic */
	CDNN_CONVOLUTION_BWD_FILTER_ALGO_1 = 1,
	CDNN_CONVOLUTION_BWD_FILTER_ALGO_FFT = 2,
	CDNN_CONVOLUTION_BWD_FILTER_ALGO_3 = 3,  /* non-deterministic */
	CDNN_CONVOLUTION_BWD_FILTER_ALGO_WINOGRAD = 4,  /* not implemented */
	CDNN_CONVOLUTION_BWD_FILTER_ALGO_WINOGRAD_NONFUSED = 5,
	CDNN_CONVOLUTION_BWD_FILTER_ALGO_FFT_TILING = 6,
	CDNN_CONVOLUTION_BWD_FILTER_ALGO_COUNT = 7
} cdnnConvolutionBwdFilterAlgo_t;

typedef enum
{
	CDNN_ACTIVATION_SIGMOID = 0,
	CDNN_ACTIVATION_RELU = 1,
	CDNN_ACTIVATION_TANH = 2,
	CDNN_ACTIVATION_CLIPPED_RELU = 3,
	CDNN_ACTIVATION_ELU = 4,
	CDNN_ACTIVATION_IDENTITY = 5
} cdnnActivationMode_t;

#define CDNN_DIM_MAX 8

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



#endif // __CSTD_CDNN_H__
