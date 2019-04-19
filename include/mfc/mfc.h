

#ifndef __MFC_H__
#define __MFC_H__


#include "std/log_c.h"
#include "std/iconv_c.h"
#include <stdlib.h>
#include <io.h>

#include <tchar.h>


int afxTraceEnabled = 0;
#define _AFXCOLL_INLINE
#define ASSERT_KINDOF(a, b)
#define ASSERT_VALID(ptr) assert(ptr!=NULL)
#define AfxIsValidString(x)   (x)!=NULL
#define AfxIsValidAddress(a, b, c)  (a)!=NULL
#define _AFX_INLINE
#define DECLARE_SERIAL(a)
#define DECLARE_DYNAMIC(a)
struct __POSITION { };
typedef __POSITION* POSITION;
#define BEFORE_START_POSITION ((POSITION)-1L)
struct _AFX_DOUBLE { BYTE doubleBits[sizeof(double)]; };
struct _AFX_FLOAT { BYTE floatBits[sizeof(float)]; };

#define VERIFY(x)   (void)(x)
#define _strinc(x)  (x+1)
#define CSTR_CHARSET "gb2312"
/////////////////////////////////////////////////////////////////////////////
typedef char TCHAR;
typedef uchar _TUCHAR;
typedef TCHAR* LPTSTR;
typedef const TCHAR* LPCTSTR;
typedef const TCHAR* LPCSTR;
typedef const wchar_t* LPCWSTR;
typedef wchar_t* LPWSTR;
/////////////////////////////////////////////////////////////////////////////



enum mfcError_t {
	mfcSuccess = 0,
	mfcErrorMissingConfiguration = 1,
	mfcErrorMemoryAllocation = 2,
	mfcErrorInitializationError = 3,
	mfcErrorLaunchFailure = 4,
	mfcErrorPriorLaunchFailure = 5,
	mfcErrorLaunchTimeout = 6,
	mfcErrorLaunchOutOfResources = 7,
	mfcErrorInvalidDeviceFunction = 8,
	mfcErrorInvalidConfiguration = 9,
	mfcErrorInvalidDevice = 10,
	mfcErrorInvalidValue = 11,
	mfcErrorInvalidPitchValue = 12,
	mfcErrorInvalidSymbol = 13,
	mfcErrorMapBufferObjectFailed = 14,
	mfcErrorUnmapBufferObjectFailed = 15,
	mfcErrorInvalidHostPointer = 16,
	mfcErrorInvalidDevicePointer = 17,
	mfcErrorInvalidTexture = 18,
	mfcErrorInvalidTextureBinding = 19,
	mfcErrorInvalidChannelDescriptor = 20,
	mfcErrorInvalidMemcpyDirection = 21,
	mfcErrorAddressOfConstant = 22,
	mfcErrorTextureFetchFailed = 23,
	mfcErrorTextureNotBound = 24,
	mfcErrorSynchronizationError = 25,
	mfcErrorInvalidFilterSetting = 26,
	mfcErrorInvalidNormSetting = 27,
	mfcErrorMixedDeviceExecution = 28,
	mfcErrormfcrtUnloading = 29,
	mfcErrorUnknown = 30,
	mfcErrorNotYetImplemented = 31,
	mfcErrorMemoryValueTooLarge = 32,
	mfcErrorInvalidResourceHandle = 33,
	mfcErrorNotReady = 34,
	mfcErrorInsufficientDriver = 35,
	mfcErrorSetOnActiveProcess = 36,
	mfcErrorInvalidSurface = 37,
	mfcErrorNoDevice = 38,
	mfcErrorECCUncorrectable = 39,
	mfcErrorSharedObjectSymbolNotFound = 40,
	mfcErrorSharedObjectInitFailed = 41,
	mfcErrorUnsupportedLimit = 42,
	mfcErrorDuplicateVariableName = 43,
	mfcErrorDuplicateTextureName = 44,
	mfcErrorDuplicateSurfaceName = 45,
	mfcErrorDevicesUnavailable = 46,
	mfcErrorInvalidKernelImage = 47,
	mfcErrorNoKernelImageForDevice = 48,
	mfcErrorIncompatibleDriverContext = 49,
	mfcErrorPeerAccessAlreadyEnabled = 50,
	mfcErrorPeerAccessNotEnabled = 51,
	mfcErrorDeviceAlreadyInUse = 54,
	mfcErrorProfilerDisabled = 55,
	mfcErrorProfilerNotInitialized = 56,
	mfcErrorProfilerAlreadyStarted = 57,
	mfcErrorProfilerAlreadyStopped = 58,
	mfcErrorAssert = 59,
	mfcErrorTooManyPeers = 60,
	mfcErrorHostMemoryAlreadyRegistered = 61,
	mfcErrorHostMemoryNotRegistered = 62,
	mfcErrorOperatingSystem = 63,
	mfcErrorPeerAccessUnsupported = 64,
	mfcErrorLaunchMaxDepthExceeded = 65,
	mfcErrorLaunchFileScopedTex = 66,
	mfcErrorLaunchFileScopedSurf = 67,
	mfcErrorSyncDepthExceeded = 68,
	mfcErrorLaunchPendingCountExceeded = 69,
	mfcErrorNotPermitted = 70,
	mfcErrorNotSupported = 71,
	mfcErrorHardwareStackError = 72,
	mfcErrorIllegalInstruction = 73,
	mfcErrorMisalignedAddress = 74,
	mfcErrorInvalidAddressSpace = 75,
	mfcErrorInvalidPc = 76,
	mfcErrorIllegalAddress = 77,
	mfcErrorInvalidPtx = 78,
	mfcErrorInvalidGraphicsContext = 79,
	mfcErrorNvlinkUncorrectable = 80,
	mfcErrorJitCompilerNotFound = 81,
	mfcErrorCooperativeLaunchTooLarge = 82,
	mfcErrorStartupFailure = 0x7f,
	mfcErrorBadParam,

	mfcErrorFileNotFound,
	mfcErrorBadPath,
	mfcErrorTooManyOpenFiles,
	mfcErrorAccessDenied,
	mfcErrorInvalidFile,
	mfcErrorRemoveCurrentDir,
	mfcErrorDirectoryFull,
	mfcErrorBadSeek,
	mfcErrorHardIO,
	mfcErrorSharingViolation,
	mfcErrorLockViolation,
	mfcErrorDiskFull,
	mfcErrorEndOfFile,

	mfcErrorApiFailureBase = 10000
};

char* asprintf(const char* fmt, va_list arglist) {
	int len = strlen(fmt)*2;
	char* buf = (char*)malloc(len+10);
	int len1 = len;
	*buf = 0;
	while(1) {
		int len1 = vsnprintf(buf, len+10, fmt, arglist);
		buf[len1] = 0;
		if (len1 >= len) {
			len *= 2;
			buf = (char*)realloc(buf, len+10);
		}
	};
	return buf;
}

static mfcError_t if_fail_return(mfcError_t err, const char* fmt, ...)
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

struct CString;

struct DumpContext {
	virtual DumpContext* def(const char* name, CString& string) = 0;
	virtual DumpContext* def(const char* name, int& i32) = 0;
	virtual DumpContext* def(const char* name, long& i32) = 0;
	virtual DumpContext* def(const char* name, int64_t& i64) = 0;
	virtual DumpContext* def(const char* name, float& f) = 0;
	virtual DumpContext* def(const char* name, double& d) = 0;
	virtual DumpContext* beginStruct(const char* name) = 0;
	virtual DumpContext* endStruct() = 0;
	//template <typename T>	DumpContext* def(const char* name, T* x) {	  x	}

};



#endif // __MFC_H__
