# 通过修改EIP寄存器实现远程注入



功能：通过修改EIP寄存器实现32位程序的DLL注入（如果是64位，记得自己对应修改汇编代码部分）


原理： 
挂起目标进程，停止目标进程EIP的变换，在目标进程开启空间，然后把相关的指令机器码和数据拷贝到里面去，然后修改目标进程EIP使其强行跳转到我们拷贝进去的相关机器码位置，执行相关，然后跳转回来。下面的例子是实现DLL注入， 
但是和平时说的远程代码注入在注入的逻辑上不同，但是同时都是用到了一个重要的结论就是：很多系统dll的导出函数地址在不同进程中，是一样的。 
*/ 

/* 思路 
修改EID实现代码注入的汇编思路如下 
SuspendThread(); 
get eip 
push ad 
push fd 
push AddressDllFilePath 
call LoadLibrary 
pop fd 
pop ad 
jmp eip //这个是为了让程序执行完我们的代码之后自己跳转回去继续执行 
ResumeThread(); 
*/ 

/* 
注意两个问题 
1.call 如果直接是个地址的话要这么计算 
call RVA - call指令的下一条地址 RVA - (nowaddress + 5) //+5是因为 call dword 是长度5 1+4 
2.jmp 直接跳转地址也是同理 
jmp RVA - jmp指令的销一条地址 RVA - (nowaddress + 5) //+5是因为 jmp dword 长度是5 1+4 
Tip 
还有就是要知道，Kernel32.LoadLibraryW的地址不同进程是一样的，这样就可以直接得到相关RVA 
*/

 

附上代码：

```
  #include "stdafx.h"  
    #include <string>  
    #include <windows.h>  
    #include "AnalyzeAndRun.h"  
    using namespace std;  
      
      
    WCHAR pDllPath[] = L"C:\\TestDllMexxBoxX32.dll";              //被注入dll的路径（32位）  
    VOID Test(){  
        HWND hWnd=::FindWindow(NULL,L"AAA");                      //注入的线程对应窗体的title AAA，  
                                                                  //主要就是为了获得tid 和 pid 这个地方可以对应修改，通过别的姿势获取。  
        if(hWnd==NULL){  
            MessageBox(NULL ,L"未获取窗口句柄！",L"失败",MB_OK);  
            return;  
        }  
        DWORD pid,tid;  
        tid=GetWindowThreadProcessId(hWnd,&pid);  
        if(tid<=0){  
            MessageBox(NULL ,L"未获取线程ID",L"失败" ,MB_OK);  
            return;  
        }  
        if(pid<=0){  
            MessageBox(NULL ,L"未获取进程ID",L"失败" ,MB_OK);  
            return;  
        }  
        HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);  
        if(hProcess <= 0){  
            MessageBox(NULL ,L"未获取进程句柄",L"失败" ,MB_OK);  
            return;  
        }  
        HANDLE hThread=OpenThread(THREAD_ALL_ACCESS,FALSE,tid);  
        if(hThread <= 0){  
            MessageBox(NULL ,L"未获取线程ID",L"失败" ,MB_OK);  
            return;  
        }  
        SuspendThread(hThread);                         //挂起线程                       
      
      
        CONTEXT ct={0};  
        ct.ContextFlags = CONTEXT_CONTROL;  
        GetThreadContext(hThread,&ct);                 //获取，保存线程寄存器相关  
      
      
        DWORD dwSize = sizeof(WCHAR)*1024;             //0-0x100 写代码 之后写数据  
        BYTE *pProcessMem = (BYTE *)::VirtualAllocEx(hProcess,NULL,dwSize,MEM_COMMIT,PAGE_EXECUTE_READWRITE);  
        DWORD dwWrited = 0;  
        ::WriteProcessMemory(hProcess, (pProcessMem + 0x100), pDllPath,   //先把路径（数据）写到内存里，从0x100开始  
            (wcslen(pDllPath) + 1) * sizeof(WCHAR), &dwWrited);  
          
      
      
        FARPROC pLoadLibraryW = (FARPROC)::GetProcAddress(::GetModuleHandle(L"Kernel32"), "LoadLibraryW");  
        BYTE ShellCode[32] = { 0 };  
        DWORD *pdwAddr = NULL;  
      
      
        ShellCode[0] = 0x60; // pushad  
        ShellCode[1] = 0x9c; // pushfd  
        ShellCode[2] = 0x68; // push  
        pdwAddr = (DWORD *)&ShellCode[3]; // ShellCode[3/4/5/6]  
        *pdwAddr = (DWORD)(pProcessMem + 0x100);  
        ShellCode[7] = 0xe8;//call  
        pdwAddr = (DWORD *)&ShellCode[8]; // ShellCode[8/9/10/11]  
        *pdwAddr = (DWORD)pLoadLibraryW - ((DWORD)(pProcessMem + 7) + 5 );  // 因为直接call地址了，所以对应机器码需要转换，计算VA  
        ShellCode[12] = 0x9d; // popfd  
        ShellCode[13] = 0x61; // popad  
        ShellCode[14] = 0xe9; // jmp  
      
      
        pdwAddr = (DWORD *)&ShellCode[15]; // ShellCode[15/16/17/18]  
        *pdwAddr = ct.Eip - ((DWORD)(pProcessMem + 14) + 5);  //因为直接jmp地址了，所以对应机器码需要转换，计算VA  
        ::WriteProcessMemory(hProcess, pProcessMem, ShellCode, sizeof(ShellCode), &dwWrited);  
        ct.Eip = (DWORD)pProcessMem;  
        ::SetThreadContext(hThread, &ct);  
      
      
        ::ResumeThread(hThread);  
        ::CloseHandle(hProcess);  
        ::CloseHandle(hThread);  
      
      
    }  
    int _tmain(int argc, _TCHAR* argv[]){   
        Test();  
        return 0;  
    } 
```

 

文章作者：[lyshark](http://www.cnblogs.com/lyshark/)