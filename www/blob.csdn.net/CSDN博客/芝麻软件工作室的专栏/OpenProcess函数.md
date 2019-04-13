
# OpenProcess函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月05日 11:11:47[seven-soft](https://me.csdn.net/softn)阅读数：186标签：[query																](https://so.csdn.net/so/search/s.do?q=query&t=blog)[工具																](https://so.csdn.net/so/search/s.do?q=工具&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=query&t=blog)个人分类：[WINDOWS API																](https://blog.csdn.net/softn/article/category/1130113)



这一年来流氓软件特别多，面对这种非常恶心的软件，让大家非常痛苦。正是在这种环境之下，众多客户需要强大查杀这种流氓软件的工具。如果让你来开发一个查杀这种病毒的软件，你会怎么做呢？当然是先把电脑里所有进程遍历出来，然后把每个进程的详细信息显示给用户，让用户决定自己那些进程可以运行，那些不可以运行。或者根据当前进程的信息，再跟根据病毒库里的特征码进行比较，就可以标识那些是可疑的病毒了。下面就来演示用函数OpenProcess怎么打开进程并获取进程的名称。
函数OpenProcess声明如下：
WINBASEAPI
__out
HANDLE
WINAPI
OpenProcess(
__in DWORD dwDesiredAccess,
__in BOOL bInheritHandle,
__in DWORD dwProcessId
);
**dwDesiredAccess**是访问进程的权限。
**bInheritHandle**是句柄是否继承进程属性。
**dwProcessId**是进程ＩＤ。
调用函数的例子如下：
\#001 //获取进程的信息。
\#002  //
\#003  void TestOpenProcesses(void)
\#004  {
\#005         //
\#006         const int nBufSize = 512;
\#007         TCHAR chBuf[nBufSize];
\#008         ZeroMemory(chBuf,nBufSize);
\#009
\#010         //
\#011         DWORD dwProcs[1024];
\#012         DWORD dwNeeded;
\#013
\#014         //枚举所有进程ID。
\#015         if ( !EnumProcesses( dwProcs, sizeof(dwProcs), &dwNeeded ) )
\#016         {
\#017               //输出出错信息。
\#018               wsprintf(chBuf,_T("EnumProcesses failed (%d)./n"), GetLastError() );
\#019               OutputDebugString(chBuf);
\#020
\#021               return;
\#022         }
\#023
\#024         //计算有多少个进程ID。
\#025         DWORD dwProcCount = dwNeeded / sizeof(DWORD);
\#026
\#027         wsprintf(chBuf,_T("EnumProcesses Count(%d)./n"), dwProcCount );
\#028         OutputDebugString(chBuf);
\#029
\#030         //遍历所有进程ID，打开进程。
\#031         for (DWORD i = 0; i < dwProcCount; i++)
\#032         {
\#033               wsprintf(chBuf,_T("EnumProcesses (%d)./r/n"), dwProcs[i] );
\#034             OutputDebugString(chBuf);
\#035
\#036               //根据进程ID打开进程。
**\#037              HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |**
**\#038                   PROCESS_VM_READ,**
**\#039                   FALSE, dwProcs[i] );**
\#040
\#041               if (hProcess)
\#042               {
\#043                    HMODULE hMod;
\#044                    DWORD cbNeeded;
\#045
\#046                    //获取进程第一个模块的句柄。
\#047                    if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod),
\#048                          &cbNeeded) )
\#049                    {
\#050                          //
\#051                          ZeroMemory(chBuf,nBufSize);
\#052
\#053                          //获取进程第一个模块的名称。
\#054                        if (::GetModuleBaseName(hProcess,hMod,chBuf,nBufSize))
\#055                          {
\#056                               //
\#057                               OutputDebugString(chBuf);
\#058                               OutputDebugString(_T("/r/n"));
\#059                          }
\#060                    }
\#061               }
\#062         }
\#063
\#064  }


