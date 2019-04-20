
#ifndef _STDC_ERROR_C_H_
#define _STDC_ERROR_C_H_

/* Fast search data structures  */
struct CvFeatureTree;
struct CvLSH;
struct CvLSHOperations;
#include <stdio.h>
#ifdef __GNUC__
#define CC_Error( code, err ) printf("%s:%d: error: (%d) %s in function %s\n", __FILE__, __LINE__, code, err, __func__)
#else
#define CC_Error( code, err ) printf("%s:%d: error: (%d) %s\n", __FILE__, __LINE__, code, err)
#endif
#define CC_Error_( code, args ) CC_Error(code, _format args)
#define CC_Assert( expr ) { if(!(expr)) CC_Error(CC_StsAssert, #expr); }
#define CC_StaticAssert(condition, reason)    {int _a[condition]={0}; const char* _s=reason;}
#ifdef _DEBUG
#define CC_DbgAssert(expr) CC_Assert(expr)
#else
#define CC_DbgAssert(expr)
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum CError {
  CC_BADMEMBLOCK_ERR = -113,
  CC_INPLACE_NOT_SUPPORTED_ERR = -112,
  CC_UNMATCHED_ROI_ERR = -111,
  CC_NOTFOUND_ERR = -110,
  CC_BADCONVERGENCE_ERR = -109,
  CC_BADDEPTH_ERR = -107,
  CC_BADROI_ERR = -106,
  CC_BADHEADER_ERR = -105,
  CC_UNMATCHED_FORMATS_ERR = -104,
  CC_UNSUPPORTED_COI_ERR = -103,
  CC_UNSUPPORTED_CHANNELS_ERR = -102,
  CC_UNSUPPORTED_DEPTH_ERR = -101,
  CC_UNSUPPORTED_FORMAT_ERR = -100,
  CC_BADARG_ERR = -49,  //ipp comp
  CC_NOTDEFINED_ERR = -48,  //ipp comp
  CC_BADCHANNELS_ERR = -47,  //ipp comp
  CC_BADRANGE_ERR = -44,  //ipp comp
  CC_BADSTEP_ERR = -29,  //ipp comp
  CC_BADFLAG_ERR = -12,
  CC_DIV_BY_ZERO_ERR = -11, //ipp comp
  CC_BADCOEF_ERR = -10,
  CC_BADFACTOR_ERR = -7,
  CC_BADPOINT_ERR = -6,
  CC_BADSCALE_ERR = -4,
  CC_OUTOFMEM_ERR = -3,
  CC_NULLPTR_ERR = -2,
  CC_BADSIZE_ERR = -1,
  CC_FAIL = -1,
  CC_NO_ERR = 0,
  CC_OK = CC_NO_ERR,

  CC_StsOk = 0,  //!< everithing is ok
  CC_StsBackTrace = -1,  //!< pseudo error for back trace
  CC_StsError = -2,  //!< unknown /unspecified error
  CC_StsInternal = -3,  //!< internal error (bad state)
  CC_StsNoMem = -4,  //!< insufficient memory
  CC_StsBadArg = -5,  //!< function arg/param is bad
  CC_StsBadFunc = -6,  //!< unsupported function
  CC_StsNoConv = -7,  //!< iter. didn't converge
  CC_StsAutoTrace = -8,  //!< tracing
  CC_HeaderIsNull = -9,  //!< image header is NULL
  CC_BadImageSize = -10,  //!< image size is invalid
  CC_BadOffset = -11,  //!< offset is invalid
  CC_BadDataPtr = -12,  //!<
  CC_BadStep = -13,  //!<
  CC_BadModelOrChSeq = -14,  //!<
  CC_BadNumChannels = -15,  //!<
  CC_BadNumChannel1U = -16,  //!<
  CC_BadDepth = -17,  //!<
  CC_BadAlphaChannel = -18,  //!<
  CC_BadOrder = -19,  //!<
  CC_BadOrigin = -20,  //!<
  CC_BadAlign = -21,  //!<
  CC_BadCallBack = -22,  //!<
  CC_BadTileSize = -23,  //!<
  CC_BadCOI = -24,  //!<
  CC_BadROISize = -25,  //!<
  CC_MaskIsTiled = -26,  //!<
  CC_StsNullPtr = -27,  //!< null pointer
  CC_StsVecLengthErr = -28,  //!< incorrect vector length
  CC_StsFilterStructContentErr = -29,  //!< incorr. filter structure content
  CC_StsKernelStructContentErr = -30,  //!< incorr. transform kernel content
  CC_StsFilterOffsetErr = -31,  //!< incorrect filter ofset value
  CC_StsBadSize = -201, //!< the input/output structure size is incorrect
  CC_StsDivByZero = -202, //!< division by zero
  CC_StsInplaceNotSupported = -203, //!< in-place operation is not supported
  CC_StsObjectNotFound = -204, //!< request can't be completed
  CC_StsUnmatchedFormats = -205, //!< formats of input/output arrays differ
  CC_StsBadFlag = -206, //!< flag is wrong or not supported
  CC_StsBadPoint = -207, //!< bad CPoint
  CC_StsBadMask = -208, //!< bad format of mask (neither 8uC1 nor 8sC1)
  CC_StsUnmatchedSizes = -209, //!< sizes of input/output structures do not match
  CC_StsUnsupportedFormat = -210, //!< the data format/type is not supported by the function
  CC_StsOutOfRange = -211, //!< some of parameters are out of range
  CC_StsParseError = -212, //!< invalid syntax/structure of the parsed file
  CC_StsNotImplemented = -213, //!< the requested function/feature is not implemented
  CC_StsBadMemBlock = -214, //!< an allocated block has been corrupted
  CC_StsAssert = -215, //!< assertion failed
  CC_GpuNotSupported = -216,
  CC_GpuApiCallError = -217,
  CC_OpenGlNotSupported = -218,
  CC_OpenGlApiCallError = -219,
  CC_OpenCLApiCallError = -220,
  CC_OpenCLDoubleNotSupported = -221,
  CC_OpenCLInitError = -222,
  CC_OpenCLNoAMDBlasFft = -223,


  stdOk = 0,
  stdOK = 0,
  stdSuccess = 0,
  errMissingConfiguration = 1,
  errMemoryAllocation = 2,
  errInitializationError = 3,
  errLaunchFailure = 4,
  errPriorLaunchFailure = 5,
  errLaunchTimeout = 6,
  errLaunchOutOfResources = 7,
  errInvalidDeviceFunction = 8,
  errInvalidConfiguration = 9,
  errInvalidDevice = 10,
  errInvalidValue = 11,
  errInvalidPitchValue = 12,
  errInvalidSymbol = 13,
  errMapBufferObjectFailed = 14,
  errUnmapBufferObjectFailed = 15,
  errInvalidHostPointer = 16,
  errInvalidDevicePointer = 17,
  errInvalidTexture = 18,
  errInvalidTextureBinding = 19,
  errInvalidChannelDescriptor = 20,
  errInvalidMemcpyDirection = 21,
  errAddressOfConstant = 22,
  errTextureFetchFailed = 23,
  errTextureNotBound = 24,
  errSynchronizationError = 25,
  errInvalidFilterSetting = 26,
  errInvalidNormSetting = 27,
  errMixedDeviceExecution = 28,
  errmfcrtUnloading = 29,
  errUnknown = 30,
  errNotYetImplemented = 31,
  errMemoryValueTooLarge = 32,
  errInvalidResourceHandle = 33,
  errNotReady = 34,
  errInsufficientDriver = 35,
  errSetOnActiveProcess = 36,
  errInvalidSurface = 37,
  errNoDevice = 38,
  errECCUncorrectable = 39,
  errSharedObjectSymbolNotFound = 40,
  errSharedObjectInitFailed = 41,
  errUnsupportedLimit = 42,
  errDuplicateVariableName = 43,
  errDuplicateTextureName = 44,
  errDuplicateSurfaceName = 45,
  errDevicesUnavailable = 46,
  errInvalidKernelImage = 47,
  errNoKernelImageForDevice = 48,
  errIncompatibleDriverContext = 49,
  errPeerAccessAlreadyEnabled = 50,
  errPeerAccessNotEnabled = 51,
  errDeviceAlreadyInUse = 54,
  errProfilerDisabled = 55,
  errProfilerNotInitialized = 56,
  errProfilerAlreadyStarted = 57,
  errProfilerAlreadyStopped = 58,
  errAssert = 59,
  errTooManyPeers = 60,
  errHostMemoryAlreadyRegistered = 61,
  errHostMemoryNotRegistered = 62,
  errOperatingSystem = 63,
  errPeerAccessUnsupported = 64,
  errLaunchMaxDepthExceeded = 65,
  errLaunchFileScopedTex = 66,
  errLaunchFileScopedSurf = 67,
  errSyncDepthExceeded = 68,
  errLaunchPendingCountExceeded = 69,
  errNotPermitted = 70,
  errNotSupported = 71,
  errHardwareStackError = 72,
  errIllegalInstruction = 73,
  errMisalignedAddress = 74,
  errInvalidAddressSpace = 75,
  errInvalidPc = 76,
  errIllegalAddress = 77,
  errInvalidPtx = 78,
  errInvalidGraphicsContext = 79,
  errNvlinkUncorrectable = 80,
  errJitCompilerNotFound = 81,
  errCooperativeLaunchTooLarge = 82,
  errStartupFailure = 0x7f,
  errBadParam,

  errFileNotFound,
  errBadPath,
  errTooManyOpenFiles,
  errAccessDenied,
  errInvalidFile,
  errRemoveCurrentDir,
  errDirectoryFull,
  errBadSeek,
  errHardIO,
  errSharingViolation,
  errLockViolation,
  errDiskFull,
  errEndOfFile,

	  Ok = 0,
	  GenericError = 1,
	  InvalidParameter = 2,
	  OutOfMemory = 3,
	  ObjectBusy = 4,
	  InsufficientBuffer = 5,
	  NotImplemented = 6,
	  Win32Error = 7,
	  WrongState = 8,
	  Aborted = 9,
	  FileNotFound = 10,
	  ValueOverflow = 11,
	  AccessDenied = 12,
	  UnknownImageFormat = 13,
	  FontFamilyNotFound = 14,
	  FontStyleNotFound = 15,
	  NotTrueTypeFont = 16,
	  UnsupportedGdiplusVersion = 17,
	  GdiplusNotInitialized = 18,
	  PropertyNotFound = 19,
	  PropertyNotSupported = 20,
	  ProfileNotFound = 21,

  errApiFailureBase = 10000
} CError;



#if 0

enum mfcError_t {
};
#endif


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

static char* asprintf(const char* fmt, va_list arglist) {
	int len = strlen(fmt) * 2;
	char* buf = (char*)malloc(len + 10);
	int len1 = len;
	*buf = 0;
	while (1) {
		int len1 = vsnprintf(buf, len + 10, fmt, arglist);
		buf[len1] = 0;
		if (len1 >= len) {
			len *= 2;
			buf = (char*)realloc(buf, len + 10);
		}
	};
	return buf;
}

static CError if_fail_return(CError err, const char* fmt, ...)
{
	//static char _time_buffer[ 16 ];
	va_list arglist;
	va_start(arglist, fmt);
	char* p = asprintf(fmt, arglist);
	va_end(arglist);
	puts(p);
	free(p);
	return err;
}



#endif // _ERROR_C_H_
