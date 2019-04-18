# 获取进程全路径方法（支持xp、win7、win10系统） - 深之JohnChen的专栏 - CSDN博客

2018年06月14日 13:01:17[byxdaz](https://me.csdn.net/byxdaz)阅读数：978


**获取进程全路径方法（支持xp、win7、win10系统）**

获取进程的全路径的函数包括GetModuleFileNameEx、GetProcessImageFileName、QueryFullProcessImageName。
这三个函数的原型：

DWORD GetModuleFileNameEx(HANDLE hProcess,HMODULE hModule,LPTSTR lpFilename,DWORD nSize)
      hProcess是目标进程的句柄、hModule是目标模块的句柄(当此参数为NULL时函数返回的是进程可执行文件的路径)、lpFilename是存放路径的字符串缓冲区、nSize表示缓冲区的大小。函数调用失败将返回0。注：进程的句柄须有PROCESS_QUERY_INFORMATION和PROCESS_VM_READ权限。

DWORD GetProcessImageFileName(HANDLE hProcess,LPTSTR lpImageFileName,DWORD nSize)
      hProcess是目标进程的句柄、lpImageFileName是存放路径的字符串缓冲区、nSize表示缓冲区的大小。函数失败将返回0。注：进程句柄需要有PROCESS_QUERY_INFORMATION的权限。

BOOL QueryFullProcessImageName(HANDLE hProcess,DWORD dwFlags,LPTSTR lpExeName,PDWORD lpdwSize)
      hProcess是目标进程的句柄、dwFlags一般设为0(表示返回的路径是Win32的路径格式，如"C:/..."，如将其设为PROCESS_NAME_NATIVE将返回"/Device/HarddiskVolume1/..."这样的格式路径)、lpExeName是存放路径的字符串缓冲区、lpdwSize表示缓冲区的大小。函数失败将返回FALSE。注：调用此函数的句柄须有PROCESS_QUERY_INFORMATION或这是PROCESS_QUERY_LIMITED_INFORMATION的权限，并且只能在Vista或更高版本的系统中使用。

      调用GetModuleFileNameEx和GetProcessImageFileName需要包含Psapi.h头文件，并链接到Psapi.lib
#include <Psapi.h>
#pragma comment (lib,"Psapi.lib")

在win10 64位系统上调用GetModuleFileNameEx()遍历获取64位进程的全路径失败，得到的路径都为空。

根据官方的说法:
For the best results use the following table to convert paths.

Windows 2000 = GetModuleFileNameEx()
Windows XP/Server2003 = GetProcessImageFileName()
Windows Vista/7/8/10/Server2008/Server2012 = QueryFullProcessImageName()

如果在win10系统中调用GetModuleFileNameEx 返回错误299 GetLastError()；需要调用QueryFullProcessImageName()函数。

实例：

```cpp
DWORD dwMajorVersion = 0;
	DWORD dwMinorVersion = 0;
	OSVERSIONINFOEX osver = { 0 };
	osver.dwOSVersionInfoSize = sizeof(osver);
	::GetVersionEx((OSVERSIONINFO*)&osver);
	dwMajorVersion = osver.dwMajorVersion;
	dwMinorVersion = osver.dwMinorVersion;
	
	//打开进程
	//dwPID,进程ID
	char  szFileName[MAX_PATH] = {0};//文件名
	char  szPathName[MAX_PATH] = {0};//路径名
	HANDLE hProcess = NULL;
	HMODULE hModule = NULL;
	hProcess=OpenProcess(PROCESS_QUERY_INFORMATION|PROCESS_VM_READ,//访问权限，win7以上系统需要提升权限EnablePrivilege
		false,//是否允许得到的进程句柄被后创建的子进程继承
		dwPID);//进程ID
  	DWORD dwError = 0;
		//BOOL bSuccessProcess = pEnumProcessModules(hProcess,&hModule,sizeof(hModule),&cbReturn);
		BOOL bSuccessProcess = EnumProcessModules(hProcess, &hModule, sizeof(hModule), &cbReturn);
		if (!bSuccessProcess)
		{
			dwError = GetLastError();
		}
		memset(szFileName, 0, sizeof(szFileName));
		memset(szPathName, 0, sizeof(szPathName));
		if (hModule != NULL)
		{
			//获得进程模块文件名(包含路径)
			if (dwMajorVersion < 5)		//2000
			{
				//保存文件名
				DWORD dwGetModuleBaseName = GetModuleFileNameEx(hProcess, hModule, szPathName, sizeof(szPathName));
				GetFileNameWithoutExtendName(szPathName, pAll[i].szFileName);
			}
			if (dwMajorVersion == 5)  //x或Windows Server2003
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
		if(hProcess)
			CloseHandle(hProcess);
				

//得到文件名(不含扩展名)  
void GetFileNameWithoutExtendName(const char* pFile,char *pFileDest)
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

//提权函数
BOOL EnablePrivilege(HANDLE hToken,LPCSTR szPrivName)
{
	TOKEN_PRIVILEGES tkp;//访问令牌权限结构变量
	LUID  luid;//本地唯一标识符结构变量

	//查询SE_DEBUG_NAME权限所对应的luid值
	if(!LookupPrivilegeValue(NULL,szPrivName,&luid))
	{
	//	TRACE("Lookup Privilege Value Failed...\nErrorCode:%d\n",GetLastError());
		return 0;
	}

	//填充Token_Privileges结构
	tkp.PrivilegeCount=1;
	tkp.Privileges[0].Luid=luid;
	tkp.Privileges[0].Attributes=SE_PRIVILEGE_ENABLED;

	//提升权限
	if(!AdjustTokenPrivileges(hToken,FALSE,&tkp,sizeof(tkp),NULL,NULL))
	{
//		TRACE("Adjust Token Privileges Failed...\nErrorCode:%d\n",GetLastError());
		return 0;
	}

	return(GetLastError()==ERROR_SUCCESS);
}
```

