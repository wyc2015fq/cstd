# 深入浅出Win32多线程程序设计之线程控制 - 深之JohnChen的专栏 - CSDN博客

2006年03月31日 10:59:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：5423


**深入浅出Win32多线程程序设计之线程控制**

作者：宋宝华出处：天极开发责任编辑：[方舟](http://comments.yesky.com/t/%B7%BD%D6%DB/6,324/2235801.shtml)[ 2005-12-15 09:04 ]

WIN32线程控制主要实现线程的创建、终止、挂起和恢复等操作，这些操作都依赖于WIN32提供的一组API和具体编译器的C运行时库函数。

WIN32线程控制主要实现线程的创建、终止、挂起和[恢复](http://www.yesky.com/key/339/80339.html)等操作，这些操作都依赖于WIN32提供的一组API和具体编译器的C运行时[库函数](http://www.yesky.com/key/280/75280.html)。

　　1.线程函数

　　在启动一个线程之前，必须为线程编写一个全局的线程函数，这个线程函数接受一个32位的LPVOID作为参数，返回一个UINT，线程函数的结构为：

UINT ThreadFunction(LPVOID pParam)
{
　//线程处理代码
　return0;
}

　　在线程处理代码部分通常包括一个死循环，该循环中先等待某事情的发生，再处理相关的工作：

while(1)
{
　WaitForSingleObject(…,…);//或WaitForMultipleObjects(…)
　//Do something
}

　　一般来说，C++的类成员函数不能作为线程函数。这是因为在类中定义的成员函数，编译器会给其加上this指针。请看下列程序：

#include "windows.h"
#include <process.h>
class ExampleTask 
{ 
　public: 
　　void taskmain(LPVOID param); 
　　void StartTask(); 
}; 
void ExampleTask::taskmain(LPVOID param) 
{} 

void ExampleTask::StartTask() 
{ 
　_beginthread(taskmain,0,NULL);
} 

int main(int argc, char* argv[])
{
　ExampleTask realTimeTask;
　realTimeTask.StartTask();
　return 0;
}

　　程序编译时出现如下错误：

error C2664: '_beginthread' : cannot convert parameter 1 from 'void (void *)' to 'void (__cdecl *)(void *)'
None of the functions with this name in scope match the target type

　　再看下列程序：

#include "windows.h"
#include <process.h>
class ExampleTask 
{ 
　public: 
　　void taskmain(LPVOID param); 
}; 

void ExampleTask::taskmain(LPVOID param) 
{} 

int main(int argc, char* argv[])
{
　ExampleTask realTimeTask;
　_beginthread(ExampleTask::taskmain,0,NULL);
　return 0;
}

　　程序编译时会出错：

error C2664: '_beginthread' : cannot convert parameter 1 from 'void (void *)' to 'void (__cdecl *)(void *)'
None of the functions with this name in scope match the target type

　　如果一定要以类成员函数作为线程函数，通常有如下解决方案：

　　（1）将该成员函数声明为static类型，[去掉](http://www.yesky.com/key/1400/66400.html)this指针；

　　我们将上述二个程序改变为：

#include "windows.h"
#include <process.h>
class ExampleTask 
{ 
　public: 
　　void static taskmain(LPVOID param); 
　　void StartTask(); 
}; 

void ExampleTask::taskmain(LPVOID param) 
{} 

void ExampleTask::StartTask() 
{ 
　_beginthread(taskmain,0,NULL);
} 

int main(int argc, char* argv[])
{
　ExampleTask realTimeTask;
　realTimeTask.StartTask();
　return 0;
}
和
#include "windows.h"
#include <process.h>
class ExampleTask 
{ 
　public: 
　　void static taskmain(LPVOID param); 
}; 

void ExampleTask::taskmain(LPVOID param) 
{} 

int main(int argc, char* argv[])
{
　_beginthread(ExampleTask::taskmain,0,NULL);
　return 0;
}

　　均编译通过。

　　将成员函数声明为静态虽然可以解决作为线程函数的问题，但是它带来了新的问题，那就是static成员函数只能[访问](http://www.yesky.com/key/4934/84934.html)static成员。解决此问题的一种途径是可以在调用类静态成员函数（线程函数）时将this指针作为参数传入，并在改线程函数中用强制类型转换将this转换成指向该类的指针，通过该指针访问非静态成员。

　　（2）不定义类成员函数为线程函数，而将线程[函数定义](http://www.yesky.com/key/1907/81907.html)为类的友元函数。这样，线程函数也可以有类成员函数同等的权限； 

　　我们将程序修改为：

#include "windows.h"
#include <process.h>
class ExampleTask 
{ 
　public: 
　　friend void taskmain(LPVOID param); 
　　void StartTask(); 
}; 

void taskmain(LPVOID param) 
{ 
　ExampleTask * pTaskMain = (ExampleTask *) param; 
　//通过pTaskMain指针引用 
} 

void ExampleTask::StartTask() 
{ 
　_beginthread(taskmain,0,this);
}
int main(int argc, char* argv[])
{
　ExampleTask realTimeTask;
　realTimeTask.StartTask();
　return 0;
}

　　（3）可以对非静态成员函数实现回调，并访问非静态成员，此法涉及到一些高级技巧，在此不再详述。

　2.创建线程

　　进程的主线程由操作系统自动生成，Win32提供了CreateThread API来完成用户线程的创建，该API的原型为：

HANDLE CreateThread(
　LPSECURITY_ATTRIBUTES lpThreadAttributes,//Pointer to a SECURITY_ATTRIBUTES structure
　SIZE_T dwStackSize, //Initial size of the stack, in bytes.
　LPTHREAD_START_ROUTINE lpStartAddress,
　LPVOID lpParameter, //Pointer to a variable to be passed to the thread
　DWORD dwCreationFlags, //Flags that control the creation of the thread
　LPDWORD lpThreadId //Pointer to a variable that receives the thread identifier
);
　　如果使用C/C++语言编写多线程应用程序，一定不能使用操作系统提供的CreateThread API，而应该使用C/C++运行时库中的_beginthread（或_beginthreadex），其函数原型为：

uintptr_t _beginthread( 
　void( __cdecl *start_address )( void * ), //Start address of routine that begins execution of new thread
　unsigned stack_size, //Stack size for new thread or 0.
　void *arglist //Argument list to be passed to new thread or NULL
);
uintptr_t _beginthreadex( 
　void *security,//Pointer to a SECURITY_ATTRIBUTES structure
　unsigned stack_size,
　unsigned ( __stdcall *start_address )( void * ),
　void *arglist,
　unsigned initflag,//Initial state of new thread (0 for running or CREATE_SUSPENDED for suspended); 
　unsigned *thrdaddr 
);
　　_beginthread函数与Win32 API 中的CreateThread函数类似，但有如下差异： 

　　（1）通过_beginthread函数我们可以利用其参数列表arglist将多个参数传递到线程； 

　　（2）_beginthread 函数初始化某些 C 运行时库变量，在线程中若需要使用 C 运行时库。 

　　3.终止线程

　　线程的终止有如下四种方式：

　　（1）线程函数返回；

　　（2）线程自身调用ExitThread 函数即终止自己，其原型为：

VOID ExitThread(UINT fuExitCode ); 
　　它将参数fuExitCode设置为线程的退出码。

　　注意：如果使用C/C++编写代码，我们应该使用C/C++运行时库函数_endthread (_endthreadex)终止线程，决不能使用ExitThread！
_endthread 函数对于线程内的条件终止很有用。例如，专门用于通信处理的线程若无法获取对通信端口的控制，则会退出。

　　（3）同一进程或其他进程的线程调用TerminateThread函数，其原型为：

BOOL TerminateThread(HANDLE hThread,DWORD dwExitCode); 
　　该函数用来结束由hThread参数指定的线程，并把dwExitCode设成该线程的退出码。当某个线程不再响应时，我们可以用其他线程调用该函数来终止这个不响应的线程。

　　（4）包含线程的进程终止。

　　最好使用第1种方式终止线程，第2~4种方式都不宜采用。

　　4.挂起与恢复线程

　　当我们创建线程的时候，如果给其传入CREATE_SUSPENDED标志，则该线程创建后被挂起，我们应使用ResumeThread恢复它：

DWORD ResumeThread(HANDLE hThread); 
　　如果ResumeThread函数运行成功，它将返回线程的前一个暂停计数，否则返回0x FFFFFFFF。

　　对于没有被挂起的线程，程序员可以调用SuspendThread函数强行挂起之：

DWORD SuspendThread(HANDLE hThread);
　　一个线程可以被挂起多次。线程可以自行暂停运行，但是不能自行恢复运行。如果一个线程被挂起n次，则该线程也必须被恢复n次才可能得以执行。

5.设置线程优先级

　　当一个线程被首次创建时，它的优先级等同于它所属进程的优先级。在单个进程内可以通过调用SetThreadPriority函数改变线程的相对优先级。一个线程的优先级是相对于其所属进程的优先级而言的。

BOOL SetThreadPriority(HANDLE hThread, int nPriority); 
　　其中参数hThread是指向待修改优先级线程的句柄，线程与包含它的进程的优先级关系如下：

　　　线程优先级 = 进程类基本优先级 + 线程相对优先级

　　进程类的基本优先级包括：

　　（1）实时：REALTIME_PRIORITY_CLASS；

　　（2）高：HIGH _PRIORITY_CLASS；

　　（3）高于正常：ABOVE_NORMAL_PRIORITY_CLASS；

　　（4）正常：NORMAL _PRIORITY_CLASS；

　　（5）低于正常：BELOW_ NORMAL _PRIORITY_CLASS；

　　（6）空闲：IDLE_PRIORITY_CLASS。

　　我们从Win32任务管理器中可以直观的看到这六个进程类优先级，如下图：

![](http://dev.yesky.com/imagelist/05/12/7hdedy7210ey.jpg)

　　线程的相对优先级包括：

　　（1）空闲：THREAD_PRIORITY_IDLE；

　　（2）最低线程：THREAD_PRIORITY_LOWEST；

　　（3）低于正常线程：THREAD_PRIORITY_BELOW_NORMAL；

　　（4）正常线程：THREAD_PRIORITY_ NORMAL (缺省)；

　　（5）高于正常线程：THREAD_PRIORITY_ABOVE_NORMAL；

　　（6）最高线程：THREAD_PRIORITY_HIGHEST；

　　（7）关键时间：THREAD_PRIOTITY_CRITICAL。

　　下图给出了进程优先级和线程相对优先级的映射关系：

![](http://dev.yesky.com/imagelist/05/12/62v88f956u3z.jpg)

　　例如：

HANDLE hCurrentThread = GetCurrentThread();
//获得该线程句柄
SetThreadPriority(hCurrentThread, THREAD_PRIORITY_LOWEST); 
　　6.睡眠

VOID Sleep(DWORD dwMilliseconds);
　　该函数可使线程暂停自己的运行，直到dwMilliseconds毫秒过去为止。它告诉系统，自身不想在某个时间段内被调度。

　　7.其它重要API

　　获得线程优先级

　　一个线程被创建时，就会有一个默认的优先级，但是有时要动态地改变一个线程的优先级，有时需获得一个线程的优先级。

Int GetThreadPriority (HANDLE hThread);
　　如果函数执行发生错误，会返回THREAD_PRIORITY_ERROR_RETURN标志。如果函数成功地执行，会返回优先级标志。

　　获得线程退出码

BOOL WINAPI GetExitCodeThread(
　HANDLE hThread,
　LPDWORD lpExitCode
);
　　如果执行成功，GetExitCodeThread返回TRUE，退出码被lpExitCode指向内存记录；否则返回FALSE，我们可通过GetLastError()获知错误原因。如果线程尚未结束，lpExitCode带回来的将是STILL_ALIVE。

获得/设置线程上下文
BOOL WINAPI GetThreadContext(
　HANDLE hThread,
　LPCONTEXT lpContext
);
BOOL WINAPI SetThreadContext(
　HANDLE hThread,
　CONST CONTEXT *lpContext
);
　　由于GetThreadContext和SetThreadContext可以操作CPU内部的寄存器，因此在一些高级技巧的编程中有一定应用。譬如，调试器可利用GetThreadContext挂起被调试线程获取其上下文，并设置上下文中的标志寄存器中的陷阱标志位，最后通过SetThreadContext使设置生效来进行单步调试。

　　8.实例

　　以下程序使用CreateThread创建两个线程，在这两个线程中Sleep一段时间，主线程通过GetExitCodeThread来判断两个线程是否结束运行：

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>

DWORD WINAPI ThreadFunc(LPVOID);

int main()
{
　HANDLE hThrd1;
　HANDLE hThrd2;
　DWORD exitCode1 = 0;
　DWORD exitCode2 = 0;
　DWORD threadId;

　hThrd1 = CreateThread(NULL, 0, ThreadFunc, (LPVOID)1, 0, &threadId );
　if (hThrd1)
　　printf("Thread 1 launched/n");

　hThrd2 = CreateThread(NULL, 0, ThreadFunc, (LPVOID)2, 0, &threadId );
　if (hThrd2)
　　printf("Thread 2 launched/n");

　// Keep waiting until both calls to GetExitCodeThread succeed AND
　// neither of them returns STILL_ACTIVE.
　for (;;)
　{
　　printf("Press any key to exit../n");
　　getch();

　　GetExitCodeThread(hThrd1, &exitCode1);
　　GetExitCodeThread(hThrd2, &exitCode2);
　　if ( exitCode1 == STILL_ACTIVE )
　　　puts("Thread 1 is still running!");
　　if ( exitCode2 == STILL_ACTIVE )
　　　puts("Thread 2 is still running!");
　　if ( exitCode1 != STILL_ACTIVE && exitCode2 != STILL_ACTIVE )
　　　break;
　}

　CloseHandle(hThrd1);
　CloseHandle(hThrd2);

　printf("Thread 1 returned %d/n", exitCode1);
　printf("Thread 2 returned %d/n", exitCode2);

　return EXIT_SUCCESS;
}

/*
* Take the startup value, do some simple math on it,
* and return the calculated value.
*/
DWORD WINAPI ThreadFunc(LPVOID n)
{
　Sleep((DWORD)n*1000*2);
　return (DWORD)n * 10;
}
　　通过下面的程序我们可以看出多线程程序运行顺序的难以预料以及WINAPI的CreateThread函数与C运行时库的_beginthread的差别：

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

DWORD WINAPI ThreadFunc(LPVOID);

int main()
{
　HANDLE hThrd;
　DWORD threadId;
　int i;

　for (i = 0; i < 5; i++)
　{
　　hThrd = CreateThread(NULL, 0, ThreadFunc, (LPVOID)i, 0, &threadId);
　　if (hThrd)
　　{
　　　printf("Thread launched %d/n", i);
　　　CloseHandle(hThrd);
　　}
　}
　// Wait for the threads to complete.
　Sleep(2000);

　return EXIT_SUCCESS;
}

DWORD WINAPI ThreadFunc(LPVOID n)
{
　int i;
　for (i = 0; i < 10; i++)
　　printf("%d%d%d%d%d%d%d%d/n", n, n, n, n, n, n, n, n);
　return 0;
}
　　运行的输出具有很大的随机性，这里摘取了几次结果的一部分（几乎每一次都不同）：

![](http://dev.yesky.com/imagelist/05/12/2269bdvd5va1.jpg)![](http://dev.yesky.com/imagelist/05/12/94hfp586n83a.jpg)![](http://dev.yesky.com/imagelist/05/12/phqe9k2g8p2f.jpg)

　　如果我们使用标准C库函数而不是多线程版的运行时库，则程序可能输出"3333444444"这样的结果，而使用多线程运行时库后，则可避免这一问题。

　　下列程序在主线程中创建一个SecondThread，在SecondThread线程中通过自增对Counter计数到1000000，主线程一直等待其结束：

#include <Win32.h>
#include <stdio.h>
#include <process.h>

unsigned Counter;
unsigned __stdcall SecondThreadFunc(void *pArguments)
{
　printf("In second thread.../n");

　while (Counter < 1000000)
　　Counter++;

　_endthreadex(0);
　return 0;
}

int main()
{
　HANDLE hThread;
　unsigned threadID;

　printf("Creating second thread.../n");

　// Create the second thread.
　hThread = (HANDLE)_beginthreadex(NULL, 0, &SecondThreadFunc, NULL, 0, &threadID);

　// Wait until second thread terminates 
　WaitForSingleObject(hThread, INFINITE);
　printf("Counter should be 1000000; it is-> %d/n", Counter);
　// Destroy the thread object.
　CloseHandle(hThread);
}

