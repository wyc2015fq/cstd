# vc 获取服务列表、开始服务、停止服务 - 深之JohnChen的专栏 - CSDN博客

2018年09月07日 12:04:22[byxdaz](https://me.csdn.net/byxdaz)阅读数：380


//ServiceInfoCollect.h

```
#pragma once

#include <list>

//服务状态信息
typedef struct tagServiceInfo
{
	char szDisplayName[MAX_PATH];		//显示名称
	char szServiceName[MAX_PATH];		//服务名
	char szLoginUser[MAX_PATH];			//启动账户名
	DWORD dwCurrentState;				//服务状态
	DWORD dwServiceType;
	char szBinaryPathName[MAX_PATH];	//进程路径
	char szDescription[1024];			//描述
	DWORD dwProcessId;
}ServiceInfo;

class ServiceInfoCollect
{
public:
	ServiceInfoCollect();
	virtual ~ServiceInfoCollect();

	//枚举所有的服务
	BOOL	GetAllService(std::list<ServiceInfo> & lstServiceInfo);
	//启动一个服务
	BOOL	StartOneService(char *pServiceName);
	//停止一个服务
	BOOL	StopOneService(char *pServiceName);
};
```

//ServiceInfoCollect.cpp

```
#include "stdafx.h"
#include "ServiceInfoCollect.h"
#include <winsvc.h>

#define MAX_SERVICE_COUNT			1024			//最大服务数

//获取进程名称
BOOL	GetProcessNameByProcessId(DWORD lProcessID, char *pProcessPathName)
{
	BOOL bRet = FALSE;
	DWORD dwMajorVersion = 0;
	DWORD dwMinorVersion = 0;
	OSVERSIONINFOEX osver = { 0 };
	osver.dwOSVersionInfoSize = sizeof(osver);
	::GetVersionEx((OSVERSIONINFO*)&osver);
	dwMajorVersion = osver.dwMajorVersion;
	dwMinorVersion = osver.dwMinorVersion;
	//打开进程
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,//访问权限
		false,//是否允许得到的进程句柄被后创建的子进程继承
		lProcessID);//进程ID
	if (hProcess)
	{
		bRet = TRUE;
		//获取进程名
		char	szPathName[MAX_PATH] = { 0 };		//进程名
		if (dwMajorVersion < 5)		//< window2000
		{
			GetModuleBaseName(hProcess, NULL, szPathName, MAX_PATH - 1);
			strcpy(pProcessPathName, szPathName);
		}
		else if (dwMajorVersion == 5)  //xp或Windows Server2003
		{
			GetProcessImageFileName(hProcess, szPathName, sizeof(szPathName));
			strcpy(pProcessPathName, szPathName);
		}
		else if (osver.dwMajorVersion >= 6)		//win7或win7以上
		{
			DWORD dwPathNameSize = sizeof(szPathName);
			BOOL bSuccessProcess = TRUE;
			bSuccessProcess = QueryFullProcessImageName(hProcess, 0, szPathName, &dwPathNameSize);
			strcpy(pProcessPathName, szPathName);
			if (!bSuccessProcess)
			{
				bRet = FALSE;
			}
		}
		::CloseHandle(hProcess);
	}

	return bRet;
}

ServiceInfoCollect::ServiceInfoCollect()
{
}

ServiceInfoCollect::~ServiceInfoCollect()
{
}

//枚举所有的服务
BOOL	ServiceInfoCollect::GetAllService(std::list<ServiceInfo> & lstServiceInfo)
{
	lstServiceInfo.clear();
	char *pBuf = NULL;                  // 缓冲区指针
	DWORD dwBufSize = 0;                // 传入的缓冲长度
	DWORD dwBufNeed = 0;                // 需要的缓冲长度
	DWORD dwNumberOfService = 0;        // 返回的服务个数
	ENUM_SERVICE_STATUS_PROCESS *pServiceInfo = NULL;   // 服务信息
	BOOL bRet = FALSE;
	ServiceInfo info = { 0 };
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_CONNECT);
	}
	if (hSCM == NULL)
	{
		return FALSE;
	}
	// 获取需要的缓冲区大小
	EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL,
		NULL, dwBufSize, &dwBufNeed, &dwNumberOfService, NULL, NULL);

	// 多设置存放1个服务信息的长度
	dwBufSize = dwBufNeed + sizeof(ENUM_SERVICE_STATUS_PROCESS);
	pBuf = (char *)malloc(dwBufSize);
	if (NULL == pBuf)
	{
		//printf("malloc error.\n");
		return FALSE;
	}
	memset(pBuf, 0, dwBufSize);
	// 获取服务信息
	bRet = EnumServicesStatusEx(hSCM, SC_ENUM_PROCESS_INFO, SERVICE_WIN32, SERVICE_STATE_ALL,
		(LPBYTE)pBuf, dwBufSize, &dwBufNeed, &dwNumberOfService, NULL, NULL);
	if (bRet == FALSE)
	{
		//printf("EnumServicesStatusEx error.\n");
		::CloseServiceHandle(hSCM);
		free(pBuf);
		return FALSE;
	}
	pServiceInfo = (LPENUM_SERVICE_STATUS_PROCESS)pBuf;
		SC_HANDLE sh;
	char* szInfo[MAX_SERVICE_COUNT * 8] = { 0 };
	DWORD dwSize = MAX_SERVICE_COUNT * 8;
	for (unsigned int i = 0; i < dwNumberOfService; i++)
	{
		dwSize = MAX_SERVICE_COUNT * 8;
		ZeroMemory(szInfo, dwSize);
		memset(&info, 0, sizeof(ServiceInfo));
		info.dwProcessId = pServiceInfo[i].ServiceStatusProcess.dwProcessId;
		strcpy_s(info.szDisplayName, MAX_PATH-1, pServiceInfo[i].lpDisplayName);
		strcpy_s(info.szServiceName, MAX_PATH - 1, pServiceInfo[i].lpServiceName);
		info.dwCurrentState = pServiceInfo[i].ServiceStatusProcess.dwCurrentState;
		info.dwServiceType = pServiceInfo[i].ServiceStatusProcess.dwServiceType;
		//获取服务描述等信息
		sh = OpenService(hSCM, pServiceInfo[i].lpServiceName, SERVICE_ALL_ACCESS);
		if (sh == NULL)
		{
			sh = OpenService(hSCM, pServiceInfo[i].lpServiceName, SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS | SERVICE_START | SERVICE_STOP);
		}
		if (sh != NULL)
		{
			//得到服务描述信息
			bRet = QueryServiceConfig2(sh, SERVICE_CONFIG_DESCRIPTION, (LPBYTE)szInfo, dwSize, &dwSize);
			if (((LPSERVICE_DESCRIPTION)szInfo)->lpDescription != NULL)
			{
				if (strlen(((LPSERVICE_DESCRIPTION)szInfo)->lpDescription) >= 1024)
				{
					memcpy(info.szDescription, ((LPSERVICE_DESCRIPTION)szInfo)->lpDescription, 1023);
				}
				else
				{
					sprintf_s(info.szDescription, 1024 - 1, ((LPSERVICE_DESCRIPTION)szInfo)->lpDescription);
				}
			}
			//得到服务的启动账户名
			bRet = QueryServiceConfig(sh, (LPQUERY_SERVICE_CONFIG)szInfo, dwSize, &dwSize);
			if (bRet)
			{
				sprintf_s(info.szLoginUser, MAX_PATH - 1, ((LPQUERY_SERVICE_CONFIG)szInfo)->lpServiceStartName);
				sprintf_s(info.szBinaryPathName, MAX_PATH - 1, ((LPQUERY_SERVICE_CONFIG)szInfo)->lpBinaryPathName);
			}
			else
			{
				GetProcessNameByProcessId(info.dwProcessId, info.szBinaryPathName);
			}
			CloseServiceHandle(sh);
		}
		lstServiceInfo.push_back(info);
	}
	CloseServiceHandle(hSCM);
	free(pBuf);
	return TRUE;
}

//启动一个服务
BOOL	ServiceInfoCollect::StartOneService(char *pServiceName)
{
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_CONNECT);
	}
	if (hSCM == NULL)
	{
		return FALSE;
	}

	SC_HANDLE sh = OpenService(hSCM, pServiceName, SERVICE_ALL_ACCESS);
	if (sh == NULL)
	{
		sh = OpenService(hSCM, pServiceName, SERVICE_START | SERVICE_STOP | SERVICE_QUERY_STATUS);
	}
	if (sh == NULL)
	{
		CloseServiceHandle(hSCM);
		return FALSE;
	}

	//启动服务
	SERVICE_STATUS status;
	BOOL bSuccess = QueryServiceStatus(sh, &status);
	if (!bSuccess)
	{
		CloseServiceHandle(sh);
		CloseServiceHandle(hSCM);
		return FALSE;
	}

	if (status.dwCurrentState != SERVICE_RUNNING)
	{
		bSuccess = StartService(sh, 0, NULL);
		if (!bSuccess)
		{
			CloseServiceHandle(sh);
			CloseServiceHandle(hSCM);
			return FALSE;
		}
	}
	CloseServiceHandle(sh);
	CloseServiceHandle(hSCM);
	return TRUE;
}

//停止一个服务
BOOL	ServiceInfoCollect::StopOneService(char *pServiceName)
{
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL)
	{
		hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_CONNECT);
	}
	if (hSCM == NULL)
	{
		return FALSE;
	}

	SC_HANDLE sh = OpenService(hSCM, pServiceName, SERVICE_ALL_ACCESS);
	if (sh == NULL)
	{
		sh = OpenService(hSCM, pServiceName, SERVICE_START | SERVICE_STOP | SERVICE_QUERY_STATUS);
	}
	if (sh == NULL)
	{
		CloseServiceHandle(hSCM);
		return FALSE;
	}

	//启动服务
	SERVICE_STATUS status;
	BOOL bSuccess = QueryServiceStatus(sh, &status);
	if (!bSuccess)
	{
		CloseServiceHandle(sh);
		CloseServiceHandle(hSCM);
		return FALSE;
	}

	if (status.dwCurrentState != SERVICE_STOPPED)
	{
		bSuccess = ControlService(sh, SERVICE_STOPPED, &status);
		if (!bSuccess)
		{
			CloseServiceHandle(sh);
			CloseServiceHandle(hSCM);
			return FALSE;
		}
	}
	CloseServiceHandle(sh);
	CloseServiceHandle(hSCM);
	return TRUE;
}
```

服务启动、停止等需要管理员权限，否则容易失败。

参考资料：

https://blog.csdn.net/lanuage/article/details/51902075

https://blog.csdn.net/dgyanyong/article/details/40209069

