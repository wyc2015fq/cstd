# VC获取进程的cpu使用率、内存、线程数、句柄数等信息 - 深之JohnChen的专栏 - CSDN博客

2018年09月07日 11:50:43[byxdaz](https://me.csdn.net/byxdaz)阅读数：973


//ProcessInfoCollect.h

```
//进程信息采集

#pragma once

//枚举进程
typedef BOOL(_stdcall *ENUMPROCESS)(
	DWORD *pProcessIds,    //指向进程ID数组链
	DWORD cb,              //ID数组的大小，用字节计数
	DWORD *pBytesReturned  //返回的字节
	);

//枚举进程模块
typedef BOOL(_stdcall *ENUMPROCESSMODULES)(
	HANDLE  hProcess,   //进程句柄
	HMODULE *lphModule, //指向模块句柄数组链
	DWORD   cb,         //模块句柄数组大小，字节计数
	LPDWORD lpcbNeeded  //存储所有模块句柄所需的字节数
	);

//获得进程模块名
typedef DWORD(_stdcall *GETMODULEFILENAMEEX)(
	HANDLE  hProcess,   //进程句柄
	HMODULE hModule,    //进程句柄
	LPTSTR  lpFilename, //存放模块全路径名
	DWORD   nSize       //lpFilename缓冲区大小，字符计算
	);

//获得进程名
typedef DWORD(_stdcall *GETMODULEBASENAME)(
	HANDLE  hProcess,  //进程句柄
	HMODULE hModule,   //模块句柄
	LPTSTR  lpBaseName,//存放进程名
	DWORD   nSize      //lpBaseName缓冲区大小
	);

//进程信息结构
typedef struct tagProcessInfo 
{
	DWORD dwPID;//进程ID
	char  szFileName[MAX_PATH];//进程文件名
	char  szPathName[MAX_PATH];//进程路径名
}ProcessInfo;

class ProcessInfoCollect
{
public:
	ProcessInfoCollect();
	virtual ~ProcessInfoCollect();

	//提权
	BOOL	EnablePrivilege(HANDLE hToken, LPCSTR szPrivName);

	//枚举所有进程
	BOOL	EnumAllProcess(ProcessInfo *pAll, int & nTotal);
	//启动进程
	BOOL	CreateNewProcess(LPCSTR pszExeName, WORD wShowWindow, HANDLE &hToken, DWORD & dwProcessID);
	//关闭进程
	int		KillProcess(DWORD nProcessID);

	//获取指定进程的所有子进程
	BOOL	GetChildProcessID(DWORD dwParentPID, DWORD* pChildProcess, int & nChildProcessNum);
	//获取进程所属令牌
	HANDLE	GetProcessToken(DWORD dwPID);
	//根据程序名获取进程ID
	DWORD	GetProcessIDByName(const char* pAppFileName);	   

	//获取当前进程的cpu使用率
	BOOL    GetCPUUserRate(double & dCPUUserRate);
	//获取指定进程的cpu使用率
	BOOL    GetCPUUserRate(DWORD lProcessID, double & dCPUUserRate);

	//获取当前进程的IO计数
	int		GetIOBytes(ULONGLONG * read_bytes, ULONGLONG * write_bytes, ULONGLONG * wct, ULONGLONG * rct);
	//获取指定进程的IO计数
	int		GetIOBytes(DWORD lProcessID, ULONGLONG * read_bytes, ULONGLONG * write_bytes, ULONGLONG * wct, ULONGLONG * rct);

	//获取当前进程的内存
	BOOL	GetMemoryUsed(DWORD & dwPeakWorkingSetSize, DWORD & dwWorkingSetSize);
	//获取指定进程的内存
	BOOL	GetMemoryUsed(DWORD lProcessID, DWORD & dwPeakWorkingSetSize, DWORD & dwWorkingSetSize);

	//获取句柄数
	BOOL	GetHandleCount(DWORD &dwHandles);
	BOOL	GetHandleCount(DWORD lProcessID, DWORD &dwHandles);

	//获取线程数
	BOOL	GetThreadCount(DWORD &dwThreads);
	BOOL	GetThreadCount(DWORD lProcessID, DWORD &dwThreads);

#ifdef StartProcessBySysServiceEx_Flag
	//启动进程(通过服务启用进程)
	BOOL	StartProcessBySysServiceEx(const char* pAppFullFileName, const char* pRunParameter);
#endif

protected:
	//获取指定进程的cpu使用率
	BOOL    GetCPUUserRateEx(HANDLE hProccess, double & dCPUUserRate);

	//获取指定进程的IO计数
	int		GetIOBytesEx(HANDLE hProccess, ULONGLONG * read_bytes, ULONGLONG * write_bytes, ULONGLONG * wct, ULONGLONG * rct);

	//获取内存
	//参数：hProccess：进程句柄；dwPeakWorkingSetSize:使用内存高峰;dwWorkingSetSize:当前使用的内存;
	BOOL	GetMemoryUsedEx(HANDLE hProccess, DWORD & dwPeakWorkingSetSize, DWORD & dwWorkingSetSize);

	//获取句柄数
	BOOL	GetHandleCountEx(HANDLE hProccess, DWORD &dwHandles);

	//中断进程
	BOOL	TerminateProcessEx(DWORD   dwPID, DWORD  dwTimeout);
	//启动进程
	BOOL	StartProcess(LPCSTR pAppFullFileName, DWORD &dwProcessID, WORD wShowWindow);

	//启动进程(通过服务启用进程)
	HANDLE	StartProcessBySysService(const char* pAppFullFileName, const char* pRunParameter, HANDLE hToken);
};
```

//ProcessInfoCollect.cpp

```
#include "stdafx.h"
#include "ProcessInfoCollect.h"
#include <stdio.h>
#include <tchar.h>
#include <TLHELP32.H>
#include <winsvc.h>
#include <psapi.h>
#pragma comment(lib,"psapi.lib")

#ifdef StartProcessBySysServiceEx_Flag
#include <Userenv.h>
#include <WtsApi32.h>
#include <atlbase.h>
#pragma comment(lib, "WtsApi32.lib")
#pragma comment(lib,"Userenv.lib")
#endif

#define			MAX_ID							4096			//最大进程数
#define			MAX_CHILD_PROCESS_COUNT			256				//子进程数

//得到文件名(包含扩展名)  
const char* GetFileName(const char* pFile)
{
	if (NULL == pFile || 0 == strlen(pFile))
	{
		return "";
	}

	const char *pPos = strrchr(pFile, '\\');
	if (NULL == pPos)
	{
		pPos = strrchr(pFile, '/');

		if (NULL == pPos)
		{
			return "";
		}
	}

	return pPos + 1;
}

//得到文件名(不含扩展名)  
void GetFileNameWithoutExtendName(const char* pFile, char *pFileDest)
{
	if (NULL == pFile || 0 == strlen(pFile) || NULL == pFileDest)
		return;

	char *pPos = (char *)strrchr(pFile, '\\');
	if (NULL == pPos)
	{
		pPos = (char *)strrchr(pFile, '/');
	}
	if (NULL == pPos)
	{
		pPos = (char *)pFile;
	}
	if (pPos != NULL)
	{
		char *pSplitFlag = strrchr(pPos, '.');
		if (pSplitFlag != NULL)
		{
			int nLenTemp = pSplitFlag - pPos - 1;
			strncpy(pFileDest, pPos + 1, nLenTemp);
			pFileDest[nLenTemp] = '\0';
		}
		else
		{
			strcpy(pFileDest, pPos + 1);
		}
	}
	else
	{
		strcpy(pFileDest, pFile);
	}
}

#define TokenLinkedToken 19

#ifdef StartProcessBySysServiceEx_Flag
DWORD GetActiveSessionID()
{
	DWORD dwSessionId = 0;
	PWTS_SESSION_INFO pSessionInfo = NULL;
	DWORD dwCount = 0;
	WTSEnumerateSessions(WTS_CURRENT_SERVER_HANDLE, 0, 1, &pSessionInfo, &dwCount);
	for (DWORD i = 0; i < dwCount; i++)
	{
		WTS_SESSION_INFO si = pSessionInfo[i];
		if (WTSActive == si.State)
		{
			dwSessionId = si.SessionId;
			break;
		}
	}
	WTSFreeMemory(pSessionInfo);
	return dwSessionId;
}
#endif

ProcessInfoCollect::ProcessInfoCollect()
{
}

ProcessInfoCollect::~ProcessInfoCollect()
{
}

//获取cpu使用率
BOOL  ProcessInfoCollect::GetCPUUserRate(double & dCPUUserRate)
{
	HANDLE hProcess = ::GetCurrentProcess();
	return GetCPUUserRateEx(hProcess, dCPUUserRate);
}

//获取指定进程的cpu使用率
BOOL    ProcessInfoCollect::GetCPUUserRate(DWORD lProcessID, double & dCPUUserRate)
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, lProcessID);
	if (hProcess == NULL)
		return FALSE;

	BOOL bSuccess = GetCPUUserRateEx(hProcess, dCPUUserRate);
	CloseHandle(hProcess);
	return bSuccess;
}

int ProcessInfoCollect::GetIOBytes(ULONGLONG * read_bytes, ULONGLONG * write_bytes, ULONGLONG * wct, ULONGLONG * rct)
{
	HANDLE hProcess = GetCurrentProcess();//获取当前进程句柄
	int nRet = GetIOBytesEx(hProcess, read_bytes, write_bytes, wct, rct);
	return nRet;
}
//获取指定进程的IO计数
int	ProcessInfoCollect::GetIOBytes(DWORD lProcessID, ULONGLONG * read_bytes, ULONGLONG * write_bytes, ULONGLONG * wct, ULONGLONG * rct)
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, lProcessID);
	if (hProcess == NULL)
		return -1;

	int nRet = GetIOBytesEx(hProcess, read_bytes, write_bytes, wct, rct);
	CloseHandle(hProcess);
	return nRet;
}

//获取句柄数
BOOL	ProcessInfoCollect::GetHandleCount(DWORD &dwHandles)
{
	return GetHandleCountEx(GetCurrentProcess(),dwHandles);
}
BOOL	ProcessInfoCollect::GetHandleCount(DWORD lProcessID, DWORD &dwHandles)
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, lProcessID);
	if (hProcess == NULL)
		return FALSE;

	BOOL bSuccess = GetHandleCountEx(hProcess, dwHandles);
	CloseHandle(hProcess);
	return bSuccess;
}

//获取当前进程的内存
BOOL	ProcessInfoCollect::GetMemoryUsed(DWORD & dwPeakWorkingSetSize, DWORD & dwWorkingSetSize)
{
	HANDLE hProcess = GetCurrentProcess();//获取当前进程句柄
	return GetMemoryUsedEx(hProcess, dwPeakWorkingSetSize, dwWorkingSetSize);
}
//获取指定进程的内存
BOOL	ProcessInfoCollect::GetMemoryUsed(DWORD lProcessID, DWORD & dwPeakWorkingSetSize, DWORD & dwWorkingSetSize)
{
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, lProcessID);
	if (hProcess == NULL)
		return FALSE;

	BOOL bSuccess = GetMemoryUsedEx(hProcess, dwPeakWorkingSetSize, dwWorkingSetSize);
	CloseHandle(hProcess);
	return bSuccess;
}

//获取指定进程的cpu使用率
BOOL    ProcessInfoCollect::GetCPUUserRateEx(HANDLE hProccess, double & dCPUUserRate)
{
	static DWORD s_dwTickCountOld = 0;
	static LARGE_INTEGER s_lgProcessTimeOld = { 0 };
	static DWORD s_dwProcessorCoreNum = 0;
	if (!s_dwProcessorCoreNum)
	{
		SYSTEM_INFO sysInfo = { 0 };
		GetSystemInfo(&sysInfo);
		s_dwProcessorCoreNum = sysInfo.dwNumberOfProcessors;
	}
	double dbProcCpuPercent = 0;
	BOOL bRetCode = FALSE;
	FILETIME CreateTime, ExitTime, KernelTime, UserTime;
	LARGE_INTEGER lgKernelTime;
	LARGE_INTEGER lgUserTime;
	LARGE_INTEGER lgCurTime;
	bRetCode = GetProcessTimes(hProccess, &CreateTime, &ExitTime, &KernelTime, &UserTime);
	if (bRetCode)
	{
		lgKernelTime.HighPart = KernelTime.dwHighDateTime;
		lgKernelTime.LowPart = KernelTime.dwLowDateTime;
		lgUserTime.HighPart = UserTime.dwHighDateTime;
		lgUserTime.LowPart = UserTime.dwLowDateTime;
		lgCurTime.QuadPart = (lgKernelTime.QuadPart + lgUserTime.QuadPart);
		if (s_lgProcessTimeOld.QuadPart)
		{
			DWORD dwElepsedTime = ::GetTickCount() - s_dwTickCountOld;
			dbProcCpuPercent = (double)(((double)((lgCurTime.QuadPart - s_lgProcessTimeOld.QuadPart) * 100)) / dwElepsedTime) / 10000;
			dbProcCpuPercent = dbProcCpuPercent / s_dwProcessorCoreNum;
		}
		s_lgProcessTimeOld = lgCurTime;
		s_dwTickCountOld = ::GetTickCount();
	}
	dCPUUserRate = dbProcCpuPercent;
	return bRetCode;
}
//获取指定进程的IO计数
int		ProcessInfoCollect::GetIOBytesEx(HANDLE hProccess, ULONGLONG * read_bytes, ULONGLONG * write_bytes, ULONGLONG * wct, ULONGLONG * rct)
{
	IO_COUNTERS io_counter;
	if (GetProcessIoCounters(hProccess, &io_counter))
	{
		if (read_bytes) *read_bytes = io_counter.ReadTransferCount;//字节数
		if (write_bytes) *write_bytes = io_counter.WriteTransferCount;
		if (wct) *wct = io_counter.WriteOperationCount;//次数
		if (rct) *rct = io_counter.ReadOperationCount;
		return 0;
	}
	return -1;
}

//获取内存
//参数：hProccess：进程句柄；dwPeakWorkingSetSize:使用内存高峰;dwWorkingSetSize:当前使用的内存;
BOOL	ProcessInfoCollect::GetMemoryUsedEx(HANDLE hProccess, DWORD & dwPeakWorkingSetSize, DWORD & dwWorkingSetSize)
{
	//根据进程ID打开进程
	if (hProccess)
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		OSVERSIONINFO osvi;//定义OSVERSIONINFO数据结构对象
		memset(&osvi, 0, sizeof(OSVERSIONINFO));//开空间 
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);//定义大小 
		GetVersionEx(&osvi);//获得版本信息 
		if (osvi.dwMajorVersion < 6)
		{
			PROCESS_MEMORY_COUNTERS pmc;
			pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS);
			//获取这个进程的内存使用情况。
			if (::GetProcessMemoryInfo(hProccess, &pmc, sizeof(pmc)))
			{
				dwWorkingSetSize = pmc.PagefileUsage;//pmc.WorkingSetSize;
				dwPeakWorkingSetSize = pmc.PeakWorkingSetSize;
				//缺页中断次数:pmc.PageFaultCount
				//使用内存高峰:pmc.PeakWorkingSetSize
				//当前使用的内存: pmc.WorkingSetSize
				//使用页面缓存池高峰: pmc.QuotaPeakPagedPoolUsage
				//使用页面缓存池: pmc.QuotaPagedPoolUsage
				//使用非分页缓存池高峰: pmc.QuotaPeakNonPagedPoolUsage
				//使用非分页缓存池: pmc.QuotaNonPagedPoolUsage
				//使用分页文件:pmc.PagefileUsage
				//使用分页文件的高峰: pmc.PeakPagefileUsage
			}
		}
		else
		{
			DWORD dwMemProcess = 0;
			PSAPI_WORKING_SET_INFORMATION workSet;
			memset(&workSet, 0, sizeof(workSet));
			BOOL bOk = QueryWorkingSet(hProccess, &workSet, sizeof(workSet));
			if (bOk || (!bOk && GetLastError() == ERROR_BAD_LENGTH))
			{
				int nSize = sizeof(workSet.NumberOfEntries) + workSet.NumberOfEntries * sizeof(workSet.WorkingSetInfo);
				char* pBuf = new char[nSize];
				if (pBuf)
				{
					QueryWorkingSet(hProccess, pBuf, nSize);
					PSAPI_WORKING_SET_BLOCK* pFirst = (PSAPI_WORKING_SET_BLOCK*)(pBuf + sizeof(workSet.NumberOfEntries));
					DWORD dwMem = 0;
					for (ULONG_PTR nMemEntryCnt = 0; nMemEntryCnt < workSet.NumberOfEntries; nMemEntryCnt++, pFirst++)
					{
						if (pFirst->Shared == 0) dwMem += si.dwPageSize;
					}
					delete pBuf;
					if(workSet.NumberOfEntries > 0)
					{
						dwMemProcess = dwMem;
						dwWorkingSetSize = dwMemProcess;
						dwPeakWorkingSetSize = dwMemProcess;
					}
				}
			}
			else
			{
				return FALSE;
			}
		}
	}
	else
	{
		int ret = GetLastError();
		return FALSE;
	}
	return TRUE;
}

//获取句柄数
BOOL ProcessInfoCollect::GetHandleCountEx(HANDLE hProccess, DWORD &dwHandles)
{
	return GetProcessHandleCount(hProccess, &dwHandles);
}

//获取线程数
BOOL	ProcessInfoCollect::GetThreadCount(DWORD &dwThreads)
{
	return GetThreadCount(GetCurrentProcessId(), dwThreads);
}
BOOL ProcessInfoCollect::GetThreadCount(DWORD lProcessID, DWORD &dwThreads)
{
	//获取进程信息
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return FALSE;

	BOOL bFind = FALSE;
	char szFilePath[MAX_PATH] = { 0 };
	PROCESSENTRY32 stProcessEntry32 = { 0 };
	stProcessEntry32.dwSize = sizeof(stProcessEntry32);
	BOOL bSucceed = ::Process32First(hProcessSnap, &stProcessEntry32);;
	for (;;)
	{
		if (!bSucceed)
			break;

		if (stProcessEntry32.th32ProcessID == lProcessID)
		{
			dwThreads = stProcessEntry32.cntThreads;
			bFind = TRUE;
			break;
		}
		bSucceed = ::Process32Next(hProcessSnap, &stProcessEntry32);
	}
	::CloseHandle(hProcessSnap);
	return bFind;
}

BOOL ProcessInfoCollect::EnablePrivilege(HANDLE hToken, LPCSTR szPrivName)
{
	TOKEN_PRIVILEGES tkp;//访问令牌权限结构变量
	LUID  luid;//本地唯一标识符结构变量

			   //查询SE_DEBUG_NAME权限所对应的luid值
	if (!LookupPrivilegeValue(NULL, szPrivName, &luid))
	{
		//	TRACE("Lookup Privilege Value Failed...\nErrorCode:%d\n",GetLastError());
		return 0;
	}

	//填充Token_Privileges结构
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = luid;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	//提升权限
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL))
	{
		//		TRACE("Adjust Token Privileges Failed...\nErrorCode:%d\n",GetLastError());
		return 0;
	}

	return(GetLastError() == ERROR_SUCCESS);
}

BOOL ProcessInfoCollect::EnumAllProcess(ProcessInfo *pAll, int & nTotal)
{
	ENUMPROCESS         pEnumProcess;
	ENUMPROCESSMODULES  pEnumProcessModules;
	GETMODULEFILENAMEEX pGetModuleFileNameEx;
	GETMODULEBASENAME   pGetModuleBaseName;

	DWORD dwMajorVersion = 0;
	DWORD dwMinorVersion = 0;
	OSVERSIONINFOEX osver = { 0 };
	osver.dwOSVersionInfoSize = sizeof(osver);
	::GetVersionEx((OSVERSIONINFO*)&osver);
	dwMajorVersion = osver.dwMajorVersion;
	dwMinorVersion = osver.dwMinorVersion;

	HANDLE hToken = NULL;//访问令牌句柄

	DWORD dwPID[MAX_ID] = { 0 };//接授进程ID的数组
	DWORD cbReturn = 0;//返回的字节数
	DWORD dwPCount = 0;//进程数
	DWORD i;
	char  szFileName[MAX_PATH] = { 0 };//文件名
	char  szPathName[MAX_PATH] = { 0 };//路径名
	char  Id[] = "ID", Pid[] = "PID", Exe[] = "ProcessName", Path[] = "Path";

	HANDLE hProcess = NULL; //进程句柄
	HMODULE hModule = NULL;
	HINSTANCE hPsDll = NULL;//实例句柄

	hPsDll = LoadLibrary("PSAPI.DLL");//获得DLL的实例
	if (hPsDll == NULL)
	{
		//FreeLibrary(hPsDll);
		return FALSE;
	}

	//获得函数的地址
	pEnumProcess = (ENUMPROCESS)GetProcAddress(hPsDll, "EnumProcesses");
	pEnumProcessModules = (ENUMPROCESSMODULES)GetProcAddress(hPsDll, "EnumProcessModules");
	pGetModuleFileNameEx = (GETMODULEFILENAMEEX)GetProcAddress(hPsDll, "GetModuleFileNameExA");
	pGetModuleBaseName = (GETMODULEBASENAME)GetProcAddress(hPsDll, "GetModuleBaseNameA");

	if (!(pEnumProcess && pEnumProcessModules && pGetModuleFileNameEx &&pGetModuleBaseName))
	{
		FreeLibrary(hPsDll);
		return FALSE;
	}

	//打开当前进程访问令牌
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
	{
		if (EnablePrivilege(hToken, SE_DEBUG_NAME))
		{
			pEnumProcess(dwPID, sizeof(dwPID), &cbReturn);//枚举进程
			dwPCount = cbReturn / sizeof(DWORD);//计算进程/线程总数
			if (dwPCount > nTotal)
			{
				dwPCount = nTotal;
			}
			else
			{
				nTotal = dwPCount;
			}
			for (i = 0; i < dwPCount; i++)
			{
				//打开进程
				hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,//访问权限
					false,//是否允许得到的进程句柄被后创建的子进程继承
					dwPID[i]);//进程ID
				pAll[i].dwPID = dwPID[i];
				if (hProcess)
				{
					//枚举进程模块
					DWORD dwError = 0;
					BOOL bSuccessProcess = pEnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbReturn);
					if (!bSuccessProcess)
					{
						dwError = GetLastError();
					}
					memset(pAll[i].szFileName, 0, sizeof(char)*MAX_PATH);
					memset(pAll[i].szPathName, 0, sizeof(char)*MAX_PATH);
					memset(szFileName, 0, sizeof(szFileName));
					memset(szPathName, 0, sizeof(szPathName));
					if (hModule != NULL)
					{
						//获得进程模块文件名(包含路径)
						if (dwMajorVersion < 5)		//< window2000
						{
							//保存文件名
							//DWORD dwGetModuleBaseName = pGetModuleFileNameEx(hProcess, hModule, szPathName, sizeof(szPathName));
							DWORD dwGetModuleBaseName = GetModuleFileNameEx(hProcess, hModule, szPathName, sizeof(szPathName));
							GetFileNameWithoutExtendName(szPathName, pAll[i].szFileName);
						}
						if (dwMajorVersion == 5)  //xp或Windows Server2003
						{
							GetProcessImageFileName(hProcess, szPathName, sizeof(szPathName));
							GetFileNameWithoutExtendName(szPathName, pAll[i].szFileName);
						}
						else if (osver.dwMajorVersion >= 6)		//win7或win7以上
						{
							DWORD dwPathNameSize = sizeof(szPathName);
							bSuccessProcess = QueryFullProcessImageName(hProcess, 0, szPathName, &dwPathNameSize);
							GetFileNameWithoutExtendName(szPathName, pAll[i].szFileName);
						}
						if (!bSuccessProcess)
						{
							dwError = GetLastError();
						}
						strcpy(pAll[i].szPathName, szPathName);
					}
				}
				if (hProcess)
					CloseHandle(hProcess);
				//if(hModule)
				//	CloseHandle(hModule);
			}
		}
	}

	//关闭句柄和实例
	if (hToken)
		CloseHandle(hToken);
	FreeLibrary(hPsDll);

	return TRUE;
}

/*
功能说明：获取指定进程的所有子进程
参数说明：dwParentPID，父进程ID;m_pChildProcess,[out] 子进程列表；nChildProcessNum，[out]子进程数量
返回值：子进程ID列表
*/
BOOL ProcessInfoCollect::GetChildProcessID(DWORD dwParentPID, DWORD* pChildProcess, int & nChildProcessNum)
{
	//获取进程信息
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return FALSE;

	/*DWORD dwProcessID = 0;*/
	PROCESSENTRY32 stProcessEntry32 = { 0 };
	stProcessEntry32.dwSize = sizeof(stProcessEntry32);
	BOOL bSucceed = ::Process32First(hProcessSnap, &stProcessEntry32);;

	int  i = 0;
	for (;;)
	{
		if (!bSucceed)
			break;

		bSucceed = ::Process32Next(hProcessSnap, &stProcessEntry32);

		if (stProcessEntry32.th32ParentProcessID == dwParentPID)
		{
			if (i < MAX_CHILD_PROCESS_COUNT)
			{
				pChildProcess[i++] = stProcessEntry32.th32ProcessID;
			}
			else
			{
				break;
			}
		}
	}
	::CloseHandle(hProcessSnap);
	nChildProcessNum = i;

	return TRUE;
}

BOOL   CALLBACK   CloseWindowByPID(HWND  hwnd, LPARAM   lParam)
{
	BOOL bRet = FALSE;
	DWORD   dwID;

	GetWindowThreadProcessId(hwnd, &dwID);

	if (dwID == (DWORD)lParam)
	{
		if (PostMessage(hwnd, WM_CLOSE, 0, 0))
		{
			bRet = TRUE;
		}
	}

	return   TRUE;
}

//结束进程
BOOL ProcessInfoCollect::TerminateProcessEx(DWORD   dwPID, DWORD  dwTimeout)
{
	HANDLE       hProc;
	BOOL       bRet = TRUE;

	//   If   we   can 't   open   the   process   with   PROCESS_TERMINATE   rights,
	//   then   we   give   up   immediately.
	hProc = OpenProcess(SYNCHRONIZE | PROCESS_TERMINATE, FALSE,
		dwPID);

	if (hProc == NULL)
	{
		return   FALSE;
	}

	//   WindowEnumCallBack()   posts   WM_CLOSE   to   all   windows   whose   PID
	//   matches  dwPID.
	EnumWindows((WNDENUMPROC)CloseWindowByPID, (LPARAM)dwPID);

	//   Wait   on   the   handle.   If   it   signals,   great.   If   it   times   out,
	//   then   you   kill   it.
	if (WaitForSingleObject(hProc, dwTimeout) != WAIT_OBJECT_0)
	{
		bRet = TerminateProcess(hProc, 0);
		WaitForSingleObject(hProc, dwTimeout);
	}

	CloseHandle(hProc);
	return   bRet;
}

//获取进程所属令牌
HANDLE ProcessInfoCollect::GetProcessToken(DWORD dwPID)
{
	HANDLE       hProc;
	HANDLE       hToken = NULL;

	//   If   we   can 't   open   the   process   with   PROCESS_TERMINATE   rights,
	//   then   we   give   up   immediately. if(!OpenProcessToken(hProc,TOKEN_QUERY|TOKEN_QUERY_SOURCE, &hToken))
	hProc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE,
		dwPID);

	if (hProc != NULL)
	{
		if (!OpenProcessToken(hProc, TOKEN_ALL_ACCESS, &hToken))
		{
			DWORD dwRet = GetLastError();
		}
		CloseHandle(hProc);
	}

	return hToken;

}

//创建新进程
BOOL ProcessInfoCollect::CreateNewProcess(LPCSTR pszExeName, WORD wShowWindow, HANDLE &hToken, DWORD & dwProcessID)
{
	if (hToken == NULL)
	{
		BOOL bRet = StartProcess(pszExeName, dwProcessID, wShowWindow);
		if (bRet)
		{
			HANDLE hNewToken = GetProcessToken(dwProcessID);
			if (hToken != NULL)
				CloseHandle(hToken);

			hToken = hNewToken;
		}
		return bRet;
	}
	else
	{
		HANDLE hNewToken = StartProcessBySysService(pszExeName, "", hToken);
		if (hToken != NULL)
			CloseHandle(hToken);

		hToken = hNewToken;
		return TRUE;
	}
}

//根据程序名获取进程ID
DWORD ProcessInfoCollect::GetProcessIDByName(const char* pAppFileName)
{
	//获取进程信息
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
		return 0;

	DWORD dwProcessID = 0;
	PROCESSENTRY32 stProcessEntry32 = { 0 };
	stProcessEntry32.dwSize = sizeof(stProcessEntry32);
	BOOL bSucceed = ::Process32First(hProcessSnap, &stProcessEntry32);;
	for (;;)
	{
		if (!bSucceed)
			break;

		bSucceed = ::Process32Next(hProcessSnap, &stProcessEntry32);
		if (::_stricmp(stProcessEntry32.szExeFile, pAppFileName) == 0)
		{
			dwProcessID = stProcessEntry32.th32ProcessID;
			break;
		}
	}
	::CloseHandle(hProcessSnap);
	return dwProcessID;
}

//关闭进程
int ProcessInfoCollect::KillProcess(DWORD nProcessID)
{
	HANDLE hProcessHandle = NULL;
	hProcessHandle = ::OpenProcess(PROCESS_TERMINATE, FALSE, nProcessID);
	if (hProcessHandle == NULL)
		return 0;

	//关闭子进程
	DWORD *pChildProcess = new DWORD[MAX_CHILD_PROCESS_COUNT];
	memset(pChildProcess, 0, MAX_CHILD_PROCESS_COUNT);
	int nChildProcessNum = 0;
	BOOL bRet = GetChildProcessID(nProcessID, pChildProcess, nChildProcessNum);
	if (bRet)
	{
		for (int i = 0; i < nChildProcessNum; ++i)
		{
			if (pChildProcess[i] > 0)
			{
				TerminateProcessEx(pChildProcess[i], 200);
			}
		}
	}
	if (pChildProcess != NULL)
	{
		delete[]pChildProcess;
		pChildProcess = NULL;
	}

	bRet = ::TerminateProcess(hProcessHandle, 200);
	if (hProcessHandle != NULL)
		CloseHandle(hProcessHandle);

	return bRet;
}

//启动进程

BOOL ProcessInfoCollect::StartProcess(LPCSTR pAppFullFileName, DWORD &dwProcessID, WORD wShowWindow)
{
	BOOL bRet = FALSE;
	STARTUPINFO   stStartupInformation = { 0 };
	stStartupInformation.cb = sizeof(stStartupInformation);
	stStartupInformation.dwFlags = STARTF_USESHOWWINDOW;
	stStartupInformation.wShowWindow = wShowWindow;//SW_SHOW;

												   //stStartupInformation.lpDesktop="WinSta0\\Winlogon";

	stStartupInformation.lpDesktop = "winsta0\\default";

	PROCESS_INFORMATION   stProcessInformation = { 0 };
	//目录工作路径
	char szProgrammeWorkPath[MAX_PATH] = { 0 };
	::strcpy_s(szProgrammeWorkPath, MAX_PATH - 1, pAppFullFileName);//+1,从c:\..开始
	if (::strchr(szProgrammeWorkPath, ':') != NULL &&
		::strrchr(szProgrammeWorkPath, '\\') != NULL)
	{
		(_tcsrchr(szProgrammeWorkPath, '\\'))[1] = 0;
	}

	bRet = ::CreateProcess((LPTSTR)pAppFullFileName, NULL, NULL, NULL, FALSE, 0, NULL, szProgrammeWorkPath,
		&stStartupInformation, &stProcessInformation);

	dwProcessID = stProcessInformation.dwProcessId;
	if (stProcessInformation.hProcess != NULL)
		CloseHandle(stProcessInformation.hProcess);

	if (stProcessInformation.dwThreadId != NULL)
		CloseHandle(stProcessInformation.hThread);

	return bRet;
}

//启动进程(令牌)
HANDLE ProcessInfoCollect::StartProcessBySysService(const char* pAppFullFileName, const char* pRunParameter, HANDLE hToken)
{
	HANDLE hNewToken = NULL;

	char szSoftWare[1024] = { 0 };
	sprintf_s(szSoftWare, 1024, "\"%s\" %s", pAppFullFileName, pRunParameter);

	STARTUPINFO   stStartupInformation = { 0 };
	stStartupInformation.cb = sizeof(stStartupInformation);
	stStartupInformation.dwFlags = STARTF_USESHOWWINDOW;
	stStartupInformation.wShowWindow = SW_SHOW;
	stStartupInformation.lpDesktop = "winsta0\\default";

	PROCESS_INFORMATION   stProcessInformation = { 0 };

	//目录工作路径
	char szProgrammeWorkPath[MAX_PATH] = { 0 };
	::strcpy_s(szProgrammeWorkPath, MAX_PATH - 1, pAppFullFileName);//+1,从c:\..开始
	if (::strchr(szProgrammeWorkPath, ':') != NULL &&
		::strrchr(szProgrammeWorkPath, '\\') != NULL)
	{
		(_tcsrchr(szProgrammeWorkPath, '\\'))[1] = 0;
	}

	if (DuplicateTokenEx(hToken, MAXIMUM_ALLOWED, NULL,
		SecurityImpersonation, TokenPrimary, &hNewToken))
	{
		if (CreateProcessAsUser(hNewToken, NULL,
			szSoftWare, NULL, NULL, FALSE,
			0, NULL, szProgrammeWorkPath, &stStartupInformation, &stProcessInformation))
		{
			if (stProcessInformation.hProcess != NULL)
				CloseHandle(stProcessInformation.hProcess);
			if (stProcessInformation.dwThreadId != NULL)
				CloseHandle(stProcessInformation.hThread);
		}
	}
	return hNewToken;
}

#ifdef StartProcessBySysServiceEx_Flag
BOOL	ProcessInfoCollect::StartProcessBySysServiceEx(const char* pAppFullFileName, const char* pRunParameter)
{
	DWORD dwProcesses = 0;
	BOOL bResult = FALSE;
	char szSoftWare[1024] = { 0 };
	sprintf_s(szSoftWare, 1024, _T("\"%s\" %s"), pAppFullFileName, pRunParameter);

	DWORD dwSid = GetActiveSessionID();
	DWORD dwRet = 0;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	HANDLE hProcess = NULL, hPToken = NULL, hUserTokenDup = NULL;
	if (!WTSQueryUserToken(dwSid, &hPToken))
	{
		PROCESSENTRY32 procEntry;
		DWORD dwPid = 0;
		HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (hSnap == INVALID_HANDLE_VALUE)
		{
			return FALSE;
		}

		procEntry.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hSnap, &procEntry))
		{
			do
			{
				if (_tcsicmp(procEntry.szExeFile, _T("explorer.exe")) == 0)
				{
					DWORD exeSessionId = 0;
					if (ProcessIdToSessionId(procEntry.th32ProcessID, &exeSessionId) && exeSessionId == dwSid)
					{
						dwPid = procEntry.th32ProcessID;
						break;
					}
				}

			} while (Process32Next(hSnap, &procEntry));
		}
		CloseHandle(hSnap);

		// explorer进程不存在
		if (dwPid == 0)
		{
			return FALSE;
		}

		hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);
		if (hProcess == NULL)
		{
			return FALSE;
		}

		if (!::OpenProcessToken(hProcess, TOKEN_ALL_ACCESS_P, &hPToken))
		{
			CloseHandle(hProcess);
			return FALSE;
		}
	}

	if (hPToken == NULL)
		return FALSE;

	TOKEN_LINKED_TOKEN admin;
	bResult = GetTokenInformation(hPToken, (TOKEN_INFORMATION_CLASS)TokenLinkedToken, &admin, sizeof(TOKEN_LINKED_TOKEN), &dwRet);

	if (!bResult) // vista 以前版本不支持TokenLinkedToken
	{
		TOKEN_PRIVILEGES tp;
		LUID luid;
		if (LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &luid))
		{
			tp.PrivilegeCount = 1;
			tp.Privileges[0].Luid = luid;
			tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		}
		//复制token
		DuplicateTokenEx(hPToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hUserTokenDup);
	}
	else
	{
		hUserTokenDup = admin.LinkedToken;
	}

	LPVOID pEnv = NULL;
	DWORD dwCreationFlags = CREATE_PRESERVE_CODE_AUTHZ_LEVEL;

	// hUserTokenDup为当前登陆用户的令牌
	if (CreateEnvironmentBlock(&pEnv, hUserTokenDup, TRUE))
	{
		//如果传递了环境变量参数，CreateProcessAsUser的
		//dwCreationFlags参数需要加上CREATE_UNICODE_ENVIRONMENT，
		//这是MSDN明确说明的
		dwCreationFlags |= CREATE_UNICODE_ENVIRONMENT;
	}
	else
	{
		//环境变量创建失败仍然可以创建进程，
		//但会影响到后面的进程获取环境变量内容
		pEnv = NULL;
	}

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE;
	ZeroMemory(&pi, sizeof(pi));
	//USES_CONVERSION;
	bResult = CreateProcessAsUser(
		hUserTokenDup,                     // client's access token
		NULL,    // file to execute
		(LPTSTR)A2T(szSoftWare),                 // command line
		NULL,            // pointer to process SECURITY_ATTRIBUTES
		NULL,               // pointer to thread SECURITY_ATTRIBUTES
		FALSE,              // handles are not inheritable
		dwCreationFlags,     // creation flags
		pEnv,               // pointer to new environment block
		NULL,               // name of current directory
		&si,               // pointer to STARTUPINFO structure
		&pi                // receives information about new process
	);

	if (pi.hProcess)
	{
		CloseHandle(pi.hThread);
		CloseHandle(pi.hProcess);
	}

	if (hUserTokenDup != NULL)
		CloseHandle(hUserTokenDup);
	if (hProcess != NULL)
		CloseHandle(hProcess);
	if (hPToken != NULL)
		CloseHandle(hPToken);
	if (pEnv != NULL)
		DestroyEnvironmentBlock(pEnv);

	return TRUE;
}
#endif
```

//调用方法

```
ProcessInfoCollect picProcessInfoCollect;
int nRet = 0;    
DWORD             nMemoryUsed;                    //内存使用(Byte)    
DWORD            nVirtualMemoryUsed;                //虚拟内存使用(Byte)    
DWORD            nHandleNumber;                    //句柄数量
DWORD dwCurrentProcessThreadCount;        //线程数量    
ULONGLONG ullIo_read_bytes;                        //IO读字节数    
ULONGLONG ullIo_write_bytes;                    //IO写字节数    
ULONGLONG ullIo_wct;                            //IO写次数    
ULONGLONG ullIo_rct;                            //IO读次数        
double dCPUUserRate = 0;                        //CPU使用的百分比        
picProcessInfoCollect.GetCPUUserRate(dCPUUserRate);    
picProcessInfoCollect.GetMemoryUsed(nVirtualMemoryUsed, nMemoryUsed);    
nVirtualMemoryUsed = nVirtualMemoryUsed;    
nMemoryUsed = nMemoryUsed;    
picProcessInfoCollect.GetThreadCount(dwCurrentProcessThreadCount);    
picProcessInfoCollect.GetHandleCount(nHandleNumber);    
picProcessInfoCollect.GetIOBytes(&ullIo_read_bytes, &ullIo_write_bytes, &ullIo_wct, &ullIo_rct);
```

