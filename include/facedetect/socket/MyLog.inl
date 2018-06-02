#include "MyLog.h"
#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

#include "io.h"
#include "RongyiAsyncThread.h"
#include "time.h"
#include "CommonFunction.h"
#include "string"
using namespace std;

MyLog* MyLog::s_pThis = NULL;
MyLog::MyLog(const string& strPath
	, eMyLogLevel level
	, const string& strLogTag
	, unsigned long dwSingleFileSizeMB /* = SINGLE_LOG_FILE_SIZE_MB_DEFAULT */
	, unsigned long dwFileCount /* = LOG_FILES_COUNT_DEFAULT */)
{
	m_pFile = NULL;
	m_eLogLevel = level;
	m_strLogTag = strLogTag;
	m_strLogFolderPath = strPath + "\\" + m_strLogTag + "_Log";
	
	m_dwFileCount = dwFileCount;
	m_dwSingleFileSizeMBMax = dwSingleFileSizeMB;
	m_dwFileSizeCurrent = 0;
	m_strCurrentLogName = "";

	m_pWorkThread = new RongyiAsyncThread();
}

MyLog:: ~MyLog()
{
	if (m_pWorkThread)
	{
		m_pWorkThread->StopThreadSync();
		delete m_pWorkThread;
	}
	
	if (m_pFile){
		fclose(m_pFile);
		m_pFile = NULL;
	}
}

bool MyLog::Init(const string& strPath
	, eMyLogLevel level
	, const string& strLogTag
	, unsigned long dwSingleFileSizeMB /* = SINGLE_LOG_FILE_SIZE_MB_DEFAULT */
	, unsigned long dwFileCount /* = LOG_FILES_COUNT_DEFAULT */)
{
	if (!s_pThis) {
		s_pThis = new MyLog(strPath, level, strLogTag, dwSingleFileSizeMB, dwFileCount);
	}

	if (!s_pThis){
		return false;
	}

	if (!s_pThis->m_pFile)
	{
		// 0.creat log folder
		s_pThis->CreatePath(s_pThis->m_strLogFolderPath);

		// 1. read Config file
		s_pThis->CreateCurrentLogFile();
	}

	s_pThis->m_eLogLevel = level;

	if (s_pThis->m_pFile){
		LOGOUT_INFO("-------------------------------------------------------------------------------------------------------------------------------------------------------");
		return true;
	} else {
		return false;
	}
}

void MyLog::DeInit()
{
	if (s_pThis){
		delete s_pThis;
		s_pThis = NULL;
	}
}

void MyLog::CreatePath(const string& strPath)
{
	int iStartIndex = 0;
	int iTempIndex = -1;
	do 
	{
		iTempIndex = strPath.find('\\', iStartIndex);
		if (-1 != iTempIndex){
			iStartIndex = iTempIndex + 1;
			string strTempPath(strPath.c_str(), iTempIndex);
			if (0 != _access(strTempPath.c_str(), 0)){
				CreateDirectoryA(strTempPath.c_str(), NULL);
			}
		}
	} while(iTempIndex != -1);

	if (0 != _access(strPath.c_str(), 0)){
		CreateDirectoryA(strPath.c_str(), NULL);
	}
}

void MyLog::WorkThreadRun(void* pWnd, void* arg, const bool* pbCancelCurrentTask)
{
	if (true == *pbCancelCurrentTask){
		return;
	}

	if (!s_pThis->m_pFile){
		return;
	}

	fseek(s_pThis->m_pFile, 0, SEEK_END);
	unsigned long dwWriteLen = strlen((char*)arg);
	fwrite((byte*)arg, 1, dwWriteLen, s_pThis->m_pFile);
	fflush( s_pThis->m_pFile );
	s_pThis->m_dwFileSizeCurrent += dwWriteLen;
	if (s_pThis->m_dwFileSizeCurrent > s_pThis->m_dwSingleFileSizeMBMax * MILLIONBYTE)
	{
		// creat new log file
		// 1.close current file
		fclose(s_pThis->m_pFile);
		s_pThis->m_pFile = NULL;

		s_pThis->CreateCurrentLogFile(true);
	}
}

bool MyLog::SetLogLevel(eMyLogLevel level)
{
	if (!s_pThis){
		return false;
	}

	s_pThis->m_eLogLevel = level;
	return true;
}

bool MyLog::Write(const char* strLogConent, const char* strPosition, eMyLogLevel level)
{
	
	if (!s_pThis || !strLogConent || !strPosition){
		return false;
	}

	if (!s_pThis->m_pFile || level < s_pThis->m_eLogLevel){
		return false;
	}

	// log level
	string strLogLevel("");
	switch(level)
	{
	case MyLogLevel_Debug:
		strLogLevel = "[DEBUG] ";
		break;
	case MyLogLevel_Err:
		strLogLevel = "[ERROR] ";
		break;
	case MyLogLevel_Info:
		strLogLevel = "[INFOS] ";
		break;
	case MyLogLevel_Fatal:
		strLogLevel = "[FATAL] ";
		break;
	case MyLogLevel_Trace:
		strLogLevel = "[TRACE] ";
		break;
	default:
		strLogLevel = "[.UFO.] ";
		break;
	}

	// time
	SYSTEMTIME st = {0};
	GetLocalTime(&st);
	char strTimeInfo[50] = {0};
	_snprintf_s(strTimeInfo, 50, "[%02d%02d%02d-%02d%02d%02d][%010d][%08d] ",
		st.wYear % 100,// 1900
		st.wMonth, // 
		st.wDay,
		st.wHour,
		st.wMinute,
		st.wSecond,
		GetTickCount(),
		GetCurrentThreadId());

	string strLog =  strTimeInfo + strLogLevel + string(strPosition) + strLogConent + "\n";

	char* pLog = new char[strLog.length() + 1];
	if (!pLog){
		return false;
	}

	memcpy(pLog, strLog.c_str(), strLog.length());
	pLog[strLog.length()] = '\0';

	s_pThis->m_pWorkThread->PostRequest(pLog, s_pThis, &MyLog::WorkThreadRun, NULL, &MyLog::StaticFreeTaskMem);

	return true;
}

void MyLog::StaticFreeTaskMem(void* pWnd, void*& pTask)
{
	if (pTask){
		char* pTemp = (char*)pTask;
		delete []pTemp;
		pTemp = NULL;
		pTask = NULL;
	}
}

void MyLog::CreateCurrentLogFile(bool bClearExistFile /* = false */)
{
	unsigned long dwLogIndex = 1;
	string strConfigFileName = s_pThis->m_strLogFolderPath + "\\" + CURRENT_LOG_INDEX;
	if (0 == _access(strConfigFileName.c_str(), 0)){
		fopen_s(&m_pFile, strConfigFileName.c_str(), "r+");
		if (m_pFile){
			fread(&dwLogIndex, 1, sizeof(unsigned long), m_pFile);
			if (bClearExistFile)
			{
				++dwLogIndex;
				if (dwLogIndex > s_pThis->m_dwFileCount){
					dwLogIndex = 1;
				}
				fseek(m_pFile, 0, SEEK_SET);
				fwrite(&dwLogIndex, 1, sizeof(unsigned long), m_pFile);
			}
			fclose(m_pFile);
			m_pFile = NULL;
		}
	}else{
		fopen_s(&m_pFile, strConfigFileName.c_str(), "w+");
		fwrite(&dwLogIndex, 1, sizeof(unsigned long), m_pFile);
		fclose(m_pFile);
		m_pFile = NULL;
	}

	s_pThis->m_strCurrentLogName = s_pThis->m_strLogFolderPath + "\\" + s_pThis->m_strLogTag + "_Log_"+ s_pThis->GetLogIndex(dwLogIndex) + ".log";


	// 2. creat log file
	if(bClearExistFile || 0 != _access(m_strCurrentLogName.c_str(), 0)){
		fopen_s(&m_pFile, m_strCurrentLogName.c_str(), "w+");
		fclose(m_pFile);
		m_pFile = NULL;
	}

	m_pFile = _fsopen(m_strCurrentLogName.c_str(), "a+", _SH_DENYNO);
	if (m_pFile){
		fseek(m_pFile, 0, SEEK_END);
		m_dwFileSizeCurrent = ftell(m_pFile);
	}else{
		m_dwFileSizeCurrent = 0;
	}
}

string MyLog::GetLogIndex(const unsigned long& dwLogFileIndex)
{
	const unsigned long s_Size = 3;
	unsigned long dwLogIndexTemp = dwLogFileIndex;
	char strRet[s_Size + 1];
	memset(strRet, 0x00, (s_Size + 1) * sizeof(char));
	for (unsigned long dwIndex = 0; dwIndex < s_Size; ++dwIndex)
	{
		strRet[s_Size - 1 - dwIndex] = s_HexadecimalChar[dwLogIndexTemp%10];
		dwLogIndexTemp /= 10;
	}

	return strRet;
}

FunctionTraceLog::FunctionTraceLog(const char* strPosition, const string& strLogInfo)
{
	m_strLogInfo = strLogInfo;
	m_strPosition = strPosition;
	m_dwFuncInTimer = GetTickCount();
	string strLog = m_strLogInfo + " start";
	MyLog::Write(strLog.c_str(), m_strPosition.c_str(), MyLogLevel_Trace);
}

FunctionTraceLog::~FunctionTraceLog()
{
	char strTime[20] = {0};
	sprintf(strTime, " end [%d]", GetTickCount() - m_dwFuncInTimer);
	string strLog = m_strLogInfo + strTime;
	MyLog::Write(strLog.c_str(), m_strPosition.c_str(), MyLogLevel_Trace);
}