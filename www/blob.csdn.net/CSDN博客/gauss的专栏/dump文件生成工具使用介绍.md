# dump文件生成工具使用介绍 - gauss的专栏 - CSDN博客
2013年01月04日 00:18:30[gauss](https://me.csdn.net/mathlmx)阅读数：329
个人分类：[开发工具/调试技巧](https://blog.csdn.net/mathlmx/article/category/1317877)
**场景：**
用户的机器上发生了死锁，没有调试环境，怎么快速定位？
用户的机器出现了Crash，你的程序没有提供自动捕获dump机制，或者捕获dump失败了？
遇到这类问题，开启远程调试成本又太高，最有的信息就是生成dump文件，然后通过dump文件查看callstack等信息定位。
**Dump文件生成原理：**
使用windbg调试器包含的dbghelp.dll库函数MiniDumpWriteDump生成dump文件。
代码示范：
typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(
                                                                                          IN HANDLE hProcess,
                                                                                          IN DWORD ProcessId,
                                                                                          IN HANDLE hFile,
                                                                                          IN MINIDUMP_TYPE DumpType,
                                                                                          IN CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam, OPTIONAL
                                                                                          IN CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam, OPTIONAL
                                                                                          IN CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam OPTIONAL
                                                                                          );
BOOL CreateDumpFile(LPCTSTR lpszDumpPath, DWORD dwProcessId,int miniDumpType)
{       
         BOOL bRet = FALSE;
         TCHAR szDbgHelpPath[_MAX_PATH] = {0};
         ::GetModuleFileName(NULL, szDbgHelpPath, _countof(szDbgHelpPath));
         LPTSTR p = _tcsrchr(szDbgHelpPath, _T('\'));
         *(p+1) = 0;
         _tcsncat(szDbgHelpPath, _T("dbghelp.dll"), _MAX_PATH - _tcslen(szDbgHelpPath) - 1);
static HMODULE hDll = NULL;
if (hDll == NULL)
                  hDll = ::LoadLibrary(szDbgHelpPath);
         assert(hDll);
if (hDll)
         {
                  MINIDUMPWRITEDUMP pWriteDumpFun = (MINIDUMPWRITEDUMP)::GetProcAddress(hDll,"MiniDumpWriteDump");
if (pWriteDumpFun)
                  {
// create the file
                           HANDLE hFile = ::CreateFile(lpszDumpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,
                                    FILE_ATTRIBUTE_NORMAL, NULL);
if (hFile != INVALID_HANDLE_VALUE)
                           {       
// write the dump
                                    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
                                    assert(hProcess);
                                    bRet = pWriteDumpFun(hProcess, dwProcessId, hFile, (MINIDUMP_TYPE)miniDumpType, 0, NULL, NULL);
                                    assert(bRet);
                                    ::CloseHandle(hProcess);
                                    ::CloseHandle(hFile);
                           }
                  }
         }
         assert(bRet);
return bRet;
}
调用上面的函数生成一个dump文件：
CreateDumpFile(strDumpFilePath, dwProcessId,
MiniDumpNormal|MiniDumpWithThreadInfo|MiniDumpWithHandleData|MiniDumpWithIndirectlyReferencedMemory);
为使用方便，我基于上面的代码实现了一个dump工具，支持MiniDump和FullDump两种模式。
