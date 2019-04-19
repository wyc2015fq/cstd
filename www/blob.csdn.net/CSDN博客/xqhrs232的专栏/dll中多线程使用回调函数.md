# dll中多线程使用回调函数 - xqhrs232的专栏 - CSDN博客
2014年01月04日 21:25:38[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1012
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.csdn.net/aizeng/article/details/6887642](http://blog.csdn.net/aizeng/article/details/6887642)
**[cpp]**[view
 plain](http://blog.csdn.net/aizeng/article/details/6887642#)[copy](http://blog.csdn.net/aizeng/article/details/6887642#)
- //dll头文件
- 
- #include <Windows.h>
- #include <stdio.h>
- #include <process.h>
- #define MYLIBAPI extern "C" __declspec(dllexport);
- typedefint (WINAPI *PFCALLBACK)();   
- MYLIBAPI void WINAPI TestCallBack(PFCALLBACK Func) ;  
**[cpp]**[view
 plain](http://blog.csdn.net/aizeng/article/details/6887642#)[copy](http://blog.csdn.net/aizeng/article/details/6887642#)
- //dll源文件
- 
- #include "stdafx.h"
- #include "CallbackDll.h"
- 
- PFCALLBACK gCallBack = NULL;  
- int WINAPI CBFunc(int Param1,int Param2);  
- ULONG WINAPI Thread1(LPVOID Param);  
- 
- void WINAPI TestCallBack(PFCALLBACK Func)   
- {   
- if(Func == NULL)  
- return;   
-     gCallBack = Func;   
- DWORD ThreadID = 0;   
- HANDLE hThread = CreateThread(NULL, NULL, Thread1, LPVOID(0), NULL, &ThreadID);   
- return;   
- }   
- 
- ULONG WINAPI Thread1(LPVOID Param)   
- {   
- int Step = 1;       
-     (*gCallBack)();  
- return 0;   
- }   
**[cpp]**[view
 plain](http://blog.csdn.net/aizeng/article/details/6887642#)[copy](http://blog.csdn.net/aizeng/article/details/6887642#)
- //.cpp文件
- #include "stdafx.h"
- #include <Windows.h>
- #include <stdio.h>
- #include <process.h>
- #include "../CallbackDll/CallbackDll.h"
- 
- 
- int WINAPI CBFunc();  
- int _tmain(int argc, _TCHAR* argv[])  
- {  
-     TestCallBack(CBFunc);  
-     Sleep(100);  
- return 0;  
- }  
- int WINAPI CBFunc()   
- {   
-     printf(_T("ddddddddddddddddddddddd\n"));  
- return 1;  
- }  
