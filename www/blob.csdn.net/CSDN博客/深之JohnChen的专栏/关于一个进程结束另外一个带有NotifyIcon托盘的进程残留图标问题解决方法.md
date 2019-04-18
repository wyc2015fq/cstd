# 关于一个进程结束另外一个带有NotifyIcon托盘的进程残留图标问题解决方法 - 深之JohnChen的专栏 - CSDN博客

2018年06月15日 16:57:01[byxdaz](https://me.csdn.net/byxdaz)阅读数：1303


        进程A用Process 启动进程B，这是B在右下角有托盘图标，然后用Process关闭进程B，但是右下角的图标还在，只有鼠标放上去后才消失。
        网上大部分介绍的方法时通过刷新任务栏托盘区,清除异常关闭进程的残留图标。该方法在win10系统上不起作用。

         现在网上找到另外一种方法：如果进程带有界面，就通过发送WM_CLOSE关闭进程；如果进程不带界面，调用TerminateProcess结束进程。

```cpp
//关闭进程
int KillProcess(DWORD nProcessID)
{
	HANDLE hProcessHandle = NULL;  
	hProcessHandle = ::OpenProcess( PROCESS_TERMINATE, FALSE,nProcessID);
	if(hProcessHandle == NULL)
		return 0;

	//关闭子进程
	DWORD *pChildProcess = new DWORD[MAX_CHILD_PROCESS_COUNT];
	memset(pChildProcess,0,MAX_CHILD_PROCESS_COUNT);
	int nChildProcessNum = 0;
	BOOL bRet = GetChildProcessID(nProcessID,pChildProcess,nChildProcessNum);
	if(bRet)
	{
		for(int i = 0; i < nChildProcessNum;++i)
		{
			if(pChildProcess[i] > 0)
			{
				TerminateProcessEx(pChildProcess[i],200);
			}
		}
	}
	if(pChildProcess != NULL)
	{
		delete []pChildProcess;
		pChildProcess = NULL;
	}

	bRet = ::TerminateProcess(hProcessHandle,200);
	if(hProcessHandle != NULL)
		CloseHandle(hProcessHandle);

	return bRet;
}

/*
功能说明：关闭进程
参数说明：dwProcessID,表示进程ID;pMainWinTitle,表示主窗口名称
返回值：BOOL类型
*/
BOOL KillProcessEx(DWORD dwProcessID, char *pMainWinTitle)
{
	BOOL bSuccess = FALSE;
	if (pMainWinTitle != NULL)
	{
		HWND hFindWnd = ::FindWindow(NULL, pMainWinTitle);
		if (hFindWnd != NULL)
		{
			//发送消息关闭，直接关闭存储残留托盘图标
			::SendMessage(hFindWnd, WM_CLOSE, 0, 0);
			bSuccess = TRUE;
		}
    else
    {
 	     	if (1 == CommonFunction::KillProcess(dwProcessID))
				{
					bSuccess = TRUE;
				}
     }
	}
	else
	{
		if (1 == CommonFunction::KillProcess(dwProcessID))
		{
			bSuccess = TRUE;
		}
	}

	return bSuccess;
}
```

//FindWindow函数，存在多个相同名称窗口时，就无法删除指定的图标。使用EnumWindows函数遍历窗口并发送退出消息。代码如下：

```cpp
bool bFindMyselfWindow = false;
struct MyselfWindowAttribute
{
	DWORD dwProcessID;
	char szMainWinTitle[256];
	char szMainWinTitleEn[256];
};

BOOL CALLBACK  MyselfEnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	static TCHAR szTitleTemp[80] = { 0 };
	GetWindowText(hwnd, szTitleTemp, sizeof(szTitleTemp));
	MyselfWindowAttribute *pMyselfWindowAttribute = (MyselfWindowAttribute *)lParam;
	if (pMyselfWindowAttribute != NULL)
	{
		bool bFind = false;
		if (_tcscmp(szTitleTemp, pMyselfWindowAttribute->szMainWinTitle) != 0)
		{
			if (_tcscmp(szTitleTemp, pMyselfWindowAttribute->szMainWinTitleEn) == 0)
			{
				bFind = true;
			}
		}
		else
		{
			bFind = true;
		}
		if (bFind)
		{
			DWORD dwProcessId = 0;
			DWORD dwRet = GetWindowThreadProcessId(hwnd, &dwProcessId);
			if (dwProcessId == pMyselfWindowAttribute->dwProcessID)
			{
				bFindMyselfWindow = true;
				//发送消息关闭，直接关闭存储残留托盘图标
				::SendMessage(hwnd, WM_CLOSE, 0, 0);
			}
		}
	}

	return TRUE;
}

/*
功能说明：关闭进程
参数说明：dwProcessID,表示进程ID;pMainWinTitle,表示主窗口名称;pMainWinTitleEn,表示主窗口名称（英文）
返回值：BOOL类型
*/
BOOL KillProcessEx(DWORD dwProcessID, char *pMainWinTitle, char *pMainWinTitleEn)
{
	BOOL bSuccess = FALSE;
	if (pMainWinTitle != NULL)
	{
		bFindMyselfWindow = false;
		MyselfWindowAttribute mwaMyselfWindowAttribute = { 0 };
		mwaMyselfWindowAttribute.dwProcessID = dwProcessID;
		strcpy(mwaMyselfWindowAttribute.szMainWinTitle, pMainWinTitle);
		strcpy(mwaMyselfWindowAttribute.szMainWinTitleEn, pMainWinTitleEn);
		EnumWindows(MyselfEnumWindowsProc, (LPARAM)&mwaMyselfWindowAttribute);

		if(!bFindMyselfWindow)
		{
			if (1 == CommonFunction::KillProcess(dwProcessID))
			{
				bSuccess = TRUE;
			}
		}
                else
                {
			bSuccess = TRUE;
		}
	}
	else
	{
		if (1 == CommonFunction::KillProcess(dwProcessID))
		{
			bSuccess = TRUE;
		}
	}

	return bSuccess;
}
```

```cpp

```


