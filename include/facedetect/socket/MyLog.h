#ifndef RY_MYLOG_H
#define RY_MYLOG_H

#include "windows.h"
#include "string"
using namespace std;



//#define TRACE_OPEN

#define MILLIONBYTE (1024*1024)
const unsigned long SINGLE_LOG_FILE_SIZE_MB_DEFAULT = 5;
const unsigned long SINGLE_LOG_FILE_SIZE_MB_MAX = 100;

#ifdef _DEBUG
const unsigned long LOG_POSISTION_SIZE_MAX = 1000;
#else
const unsigned long LOG_POSISTION_SIZE_MAX = 200;
#endif

const unsigned long LOG_FILES_COUNT_DEFAULT = 5;
const unsigned long LOG_FILES_COUNT_MAX = 50;
const unsigned long SINGLE_LOG_SIZE_MAX = 1024*2 + 1;

enum eMyLogLevel
{
	MyLogLevel_Debug = 0,
	MyLogLevel_Info = 1,
	MyLogLevel_Err = 2,
	MyLogLevel_Fatal = 3,
	MyLogLevel_Trace = 4,
};


const char CURRENT_LOG_INDEX[]	= "CURRENT_LOG_INDEX";

#ifdef TRACE_OPEN
#define	_TRACE(printf_exp, ...)		\
	printf(printf_exp, __VA_ARGS__);	\
	printf("\n");
#else
#define	_TRACE(printf_exp, ...)
#endif


inline char* stringf(const char* format, ...)
{
	va_list arg_list;
	va_start(arg_list, format);

	// SUSv2 version doesn't work for buf NULL/size 0, so try printing
	// into a small buffer that avoids the double-rendering and alloca path too...
	char* pBuffer = new char[SINGLE_LOG_SIZE_MAX];
	if(pBuffer){ 
		memset(pBuffer, 0x00, SINGLE_LOG_SIZE_MAX);	
		vsnprintf(pBuffer, SINGLE_LOG_SIZE_MAX - 1, format, arg_list);
	} 
	va_end(arg_list);
	return pBuffer;
}

inline char* stringfBigData(const char* format, ...)
{
	va_list arg_list;
	va_start(arg_list, format);

	// SUSv2 version doesn't work for buf NULL/size 0, so try printing
	// into a small buffer that avoids the double-rendering and alloca path too...
	char* pBuffer = new char[SINGLE_LOG_SIZE_MAX * 5];
	if(pBuffer){ 
		memset(pBuffer, 0x00, SINGLE_LOG_SIZE_MAX * 5);	
		vsnprintf(pBuffer, SINGLE_LOG_SIZE_MAX * 5 - 1, format, arg_list);
	} 

	return pBuffer;
}

#define LOGOUT_DEBUG(printf_exp, ...)	{ \
	_TRACE(printf_exp, __VA_ARGS__)	\
	char* pBuffer = stringf(printf_exp, __VA_ARGS__); \
	if(pBuffer){ \
		char pPosition[LOG_POSISTION_SIZE_MAX] = {0}; \
		sprintf(pPosition, "%s(%04d) %s ", __FILE__, __LINE__, __FUNCTION__); \
		MyLog::Write(pBuffer, pPosition, MyLogLevel_Debug); \
		delete []pBuffer; \
	} \
}


#define LOGOUT_INFO(printf_exp, ...)	{ \
	_TRACE(printf_exp, __VA_ARGS__)	\
	char* pBuffer = stringf(printf_exp, __VA_ARGS__); \
	if(pBuffer){ \
		char pPosition[LOG_POSISTION_SIZE_MAX] = {0}; \
		sprintf(pPosition, "%s(%04d) %s ", __FILE__, __LINE__, __FUNCTION__); \
		MyLog::Write(pBuffer, pPosition, MyLogLevel_Info); \
		delete []pBuffer; \
	} \
}

#define LOGOUT_ERR(printf_exp, ...)	{ \
	_TRACE(printf_exp, __VA_ARGS__)	\
	char* pBuffer = stringf(printf_exp, __VA_ARGS__); \
	if(pBuffer){ \
		char pPosition[LOG_POSISTION_SIZE_MAX] = {0}; \
		sprintf(pPosition, "%s(%04d) %s ", __FILE__, __LINE__, __FUNCTION__); \
		MyLog::Write(pBuffer, pPosition, MyLogLevel_Err); \
		delete []pBuffer; \
	} \
}

#define LOGOUT_FATAL(printf_exp, ...)	{ \
	_TRACE(printf_exp, __VA_ARGS__)	\
	char* pBuffer = stringf(printf_exp, __VA_ARGS__); \
	if(pBuffer){ \
		char pPosition[LOG_POSISTION_SIZE_MAX] = {0}; \
		sprintf(pPosition, "%s(%04d) %s ", __FILE__, __LINE__, __FUNCTION__); \
		MyLog::Write(pBuffer, pPosition, MyLogLevel_Fatal); \
		delete []pBuffer; \
	} \
}

#define LOGOUT_BIGDATA(printf_exp, ...)	{ \
	_TRACE(printf_exp, __VA_ARGS__)	\
	char* pBuffer = stringfBigData(printf_exp, __VA_ARGS__); \
	if(pBuffer){ \
	char pPosition[LOG_POSISTION_SIZE_MAX] = {0}; \
	sprintf(pPosition, "%s(%04d) %s ", __FILE__, __LINE__, __FUNCTION__); \
	MyLog::Write(pBuffer, pPosition, MyLogLevel_Info); \
	delete []pBuffer; \
	} \
}

class FunctionTraceLog
{
public:
	FunctionTraceLog(const char* strPosition, const string& strLogInfo);

	~FunctionTraceLog();

private:
	string m_strLogInfo;
	string m_strPosition;
	unsigned long m_dwFuncInTimer;
};

#define LOGOUT_TRACE(logInfo)	\
	char pPosition[LOG_POSISTION_SIZE_MAX] = {0}; \
	sprintf(pPosition, "%s(%04d) %s ", __FILE__, __LINE__, __FUNCTION__); \
	FunctionTraceLog traceLog(pPosition, logInfo);



class RongyiAsyncThread;
class MyLog
{
public:
	virtual ~MyLog();
	static bool Init(const string& strPath, 
		eMyLogLevel level, 
		const string& strLogTag, 
		unsigned long dwSingleFileSizeMB = SINGLE_LOG_FILE_SIZE_MB_DEFAULT, 
		unsigned long dwFileCount = LOG_FILES_COUNT_DEFAULT);
	static void DeInit();
	static bool SetLogLevel(eMyLogLevel level);
	static bool Write(const char* strLogConent, const char* strPosition, eMyLogLevel level);
	static void WorkThreadRun(void* pWnd, void* arg, const bool* pbCancelCurrentTask);

	static void StaticFreeTaskMem(void* pWnd, void*& pTask);
	void FreeTaskMem(void*& pTask);
private:
	
	MyLog(const string& strPath, 
		eMyLogLevel level, 
		const string& strLogTag, 
		unsigned long dwSingleFileSizeMB = SINGLE_LOG_FILE_SIZE_MB_DEFAULT, 
		unsigned long dwFileCount = LOG_FILES_COUNT_DEFAULT);

	static MyLog* s_pThis;
	RongyiAsyncThread* m_pWorkThread;

	eMyLogLevel m_eLogLevel;
	string m_strLogFolderPath;
	string m_strCurrentLogName;
	string m_strLogTag;
	FILE* m_pFile;

	// file control
	unsigned long m_dwFileCount;
	unsigned long m_dwSingleFileSizeMBMax;
	unsigned long m_dwFileSizeCurrent;

	void CreatePath(const string& strPath);
	string GetLogIndex(const unsigned long& dwLogFileIndex);
	void CreateCurrentLogFile(bool bClearExistFile = false);
};

#endif //RY_MYLOG_H