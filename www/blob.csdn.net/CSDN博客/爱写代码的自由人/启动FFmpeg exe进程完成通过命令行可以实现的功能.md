# 启动FFmpeg.exe进程完成通过命令行可以实现的功能 - 爱写代码的自由人 - CSDN博客





2018年02月21日 19:46:42[zhoubotong2012](https://me.csdn.net/zhoubotong2012)阅读数：3226标签：[ffmpeg](https://so.csdn.net/so/search/s.do?q=ffmpeg&t=blog)
个人分类：[ffmpeg](https://blog.csdn.net/zhoubotong2012/article/category/7460027)









      FFmpeg的功能很强大，包含编码，解码，各种输入输出格式的处理，但是使用它的API来实现功能有点困难，有没有更简单的方式？通常API能做的功能FFmpeg的官方工具即ffmpeg.exe（开源）都实现了，我们为何不直接调用ffmpeg.exe进程来完成呢？不过ffmpeg.exe是通过命令行启动的，我们的程序如果启动ffmpeg.exe进程，就会显示一个CMD窗口，这样对用户的体验不好，所以我们需要把那个CMD窗口隐藏起来，让它在后台处理任务，并且要它处理任务之后能自然退出。那究竟怎么实现呢？

1. 如何隐藏FFmpeg的进程窗口

    首先启动FFmpeg进程可以用Windows  API: CreateProcess函数，这个函数带很多参数，其中有部分参数是跟窗口显示属性相关的，我们设置相关的参数属性就能达到隐藏窗口的目的。下面这段代码用来启动netsh.exe进程并传入命令参数来配置系统IP。

```cpp
int netIPConfig(int op,uint8_t* name,uint32_t ip,uint32_t mask)
{
	uint8_t cmd[1024];
	uint8_t sysdir[256];
	STARTUPINFO si = {sizeof(si)};
	PROCESS_INFORMATION pi;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_HIDE; //隐藏窗口

	memset(sysdir,0,256);
	GetSystemDirectory(sysdir,256);
//	uint8_t sys_dir[256];
//	memset(sys_dir,0,256);
//	GetSystemDirectory(cmd,256);

	if(op == 0) /*set current ip address*/
	{
		sprintf(cmd,"%s\\netsh.exe interface ip set address name=%s source=static addr=%u.%u.%u.%u mask=%u.%u.%u.%u",
			sysdir,name,NIPQUAD(ip),NIPQUAD(mask));
	}
	else if(op == 1)/*add ip address*/
	{
		sprintf(cmd,"%s\\netsh.exe interface ip add address name=%s addr=%u.%u.%u.%u mask=%u.%u.%u.%u",
			sysdir,name,NIPQUAD(ip),NIPQUAD(mask));
	}
	else if(op == 2) /*remove ip address*/
	{
		sprintf(cmd,"%s\\netsh.exe interface ip del address name=%s addr=%u.%u.%u.%u",
			sysdir,name,NIPQUAD(ip));
	}

	if(TRUE == CreateProcess(NULL,cmd,NULL,NULL,0,0,NULL,NULL,&si,&pi))
	{
		WaitForSingleObject(pi.hProcess,-1);
		CloseHandle(pi.hProcess);
	}

	return 0;
}
```

    另外还有一个属性CREATE_NO_WINDOW也可以隐藏窗口，看下面代码：

```cpp
STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &pi, sizeof(pi) );
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);

	//// Start the child process
	if(CreateProcess(szExePath, (LPTSTR)szCmdLine, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		
	}
	else 
	{
	    return NULL;
	}
```

2. 如何让FFmpeg进程完成任务后自然退出

     方法是设置进程运行的时间，通过-t参数，这是通过命令行参数传给FFmpeg的。后面跟的时间单位是秒，比如 -t 30就是进程运行30秒后就退出。

3. 启动FFmpeg.exe并传入命令行需要的参数

    下面的代码就演示了一个例子：启动ffmpeg.exe接收RTSP流，把视频和音频流写到一个MP4容器中，保存到本地一个文件。其中设置了-t参数，让其运行了指定的时长后退出。

```cpp
HANDLE  LoadProcess( LPCSTR szExePath, LPSTR szCmdLine)
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory( &pi, sizeof(pi) );
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);

	//// Start the child process
	if(CreateProcess(szExePath, (LPTSTR)szCmdLine, NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
	{
		
	}
	else 
	{
	    return NULL;
	} 

	Sleep(100);

	return pi.hProcess;
}

//函数作用：启动FFMPEG进程接收RTSP流并录制成文件
//rtspURL -- RTSP URL
//filePath -- 录制的文件路径
//duration -- 录制的时长（时间单位：秒）
//
BOOL  StartFFmpeg(string rtspUrl, string filePath,  int duration)
{
	HANDLE hChildApp = NULL;	
	char szProcessPath[MAX_PATH] = {0};

	//取得本地应用程序目录
   	TCHAR szLocalPath[MAX_PATH]; 
	GetModuleFileName( 0, szLocalPath, MAX_PATH );
	LPTSTR pos=_tcsrchr(szLocalPath, '\\');
	if (pos)
		*pos=0;

	_tcscat(szProcessPath, szLocalPath);
	_tcscat(szProcessPath, "\\bin\\ffmpeg.exe");

    char szCmdLine[1024];
    sprintf(szCmdLine, " -y -rtsp_transport tcp -i %s -t %d -vcodec copy -acodec copy  -f mp4 %s",
            rtspUrl.c_str(), duration, filePath.c_str());

	// Load the ffmpeg process
	if( (hChildApp = LoadProcess(szProcessPath, szCmdLine)) == NULL)
	{
		//启动进程失败
		AfxMessageBox("LoadProcess failed");
		return FALSE;
	}

	//阻塞等待进程退出（根据你的需求确定是否启用下面的代码）
#if 0
	DWORD tStartTick = GetTickCount();
	BOOL bExited = FALSE;

	while(GetTickCount() < tStartTick + (duration + 4)*1000) //等待进程退出
	{
		if(bExited)
			break;

		DWORD dwExitCode = 0;
		GetExitCodeProcess(hChildApp, &dwExitCode);
		if(dwExitCode == STILL_ACTIVE) //进程为活动状态
		{
			Sleep(300);
		}
		else
		{  //进程已退出
			CloseHandle(hChildApp);
			bExited = TRUE;
			TRACE("ffmpeg.exe进程已经退出....\n");
			break;
		}
	}//while

	//进程超过最大时间还没有退出，说明进程遇到了异常情况，我们强制把进程杀掉
	DWORD dwExitCode = 0;
	GetExitCodeProcess(hChildApp, &dwExitCode);
	if(dwExitCode == STILL_ACTIVE) //进程为活动状态
	{
	   ::TerminateProcess(hChildApp, 0);
	}
#else
	WaitForSingleObject(hChildApp,-1);
	CloseHandle(hChildApp);
#endif

	return TRUE;
}
```




