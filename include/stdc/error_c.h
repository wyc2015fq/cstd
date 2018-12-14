
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
typedef enum CStatus {
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
    CC_OpenCLNoAMDBlasFft = -223
} CStatus;

#endif // _ERROR_C_H_
