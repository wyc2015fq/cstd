# C++多线程实例(_beginThreadex创建多线程) - maopig的专栏 - CSDN博客
2011年09月13日 19:47:24[maopig](https://me.csdn.net/maopig)阅读数：43895
### C++多线程（二）(_beginThreadex创建多线程)
C/C++ Runtime 多线程函数
一 简单实例（来自codeprojct：[http://www.codeproject.com/useritems/MultithreadingTutorial.asp](http://www.codeproject.com/useritems/MultithreadingTutorial.asp)）
主线程创建2个线程t1和t2，创建时2个线程就被挂起，后来调用ResumeThread恢复2个线程，是其开始执行，调用WaitForSingleObject等待2个线程执行完，然后推出主线程即结束进程。
/*  file Main.cpp
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif) *
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif) *  This program is an adaptation of the code Rex Jaeschke showed in
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif) *  Listing 1 of his Oct 2005 C/C++ User's Journal article entitled
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif) *  "C++/CLI Threading: Part I".  I changed it from C++/CLI (managed)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif) *  code to standard C++.
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif) *
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif) *  One hassle is the fact that C++ must employ a free (C) function
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif) *  or a static class member function as the thread entry function.
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif) *
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif) *  This program must be compiled with a multi-threaded C run-time
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif) *  (/MT for LIBCMT.LIB in a release build or /MTd for LIBCMTD.LIB
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif) *  in a debug build).
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif) *
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif) *                                      John Kopplin  7/2006
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)*/
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <stdio.h>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <string>// for STL string class
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <windows.h>//for HANDLE
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <process.h>//for _beginthread()
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)usingnamespace std;
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)class ThreadX
{
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)private:
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)int loopStart;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)int loopEnd;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)int dispFrequency;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)public:
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)string threadName;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)  ThreadX( int startValue, int endValue, int frequency )
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    loopStart = startValue;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    loopEnd = endValue;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    dispFrequency = frequency;
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)  }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// In C++ you must employ a free (C) function or a static
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// class member function as the thread entry-point-function.
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// Furthermore, _beginthreadex() demands that the thread
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// entry function signature take a single (void*) and returned
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// an unsigned.
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)static unsigned __stdcall ThreadStaticEntryPoint(void* pThis)
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)      ThreadX * pthX = (ThreadX*)pThis;   // the tricky cast
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)      pthX->ThreadEntryPoint();           // now call the true entry-point-function
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// A thread terminates automatically if it completes execution,
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// or it can terminate itself with a call to _endthread().
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)return1;          // the thread exit code
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)  }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)void ThreadEntryPoint()
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// This is the desired entry-point-function but to get
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// here we have to use a 2 step procedure involving
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// the ThreadStaticEntryPoint() function.
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)for (int i = loopStart; i <= loopEnd; ++i)
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if (i % dispFrequency ==0)
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)          printf( "%s: i = %d\n", threadName.c_str(), i );
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)      }
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    printf( "%s thread terminating\n", threadName.c_str() );
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)  }
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)};
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)int main()
{
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// All processes get a primary thread automatically. This primary
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// thread can generate additional threads.  In this program the
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// primary thread creates 2 additional threads and all 3 threads
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// then run simultaneously without any synchronization.  No data
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// is shared between the threads.
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// We instantiate an object of the ThreadX class. Next we will
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// create a thread and specify that the thread is to begin executing
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// the function ThreadEntryPoint() on object o1. Once started,
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// this thread will execute until that function terminates or
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// until the overall process terminates.
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    ThreadX * o1 =new ThreadX( 0, 1, 2000 );
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// When developing a multithreaded WIN32-based application with
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// Visual C++, you need to use the CRT thread functions to create
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// any threads that call CRT functions. Hence to create and terminate
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// threads, use _beginthreadex() and _endthreadex() instead of
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// the Win32 APIs CreateThread() and EndThread().
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// The multithread library LIBCMT.LIB includes the _beginthread()
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// and _endthread() functions. The _beginthread() function performs
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// initialization without which many C run-time functions will fail.
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// You must use _beginthread() instead of CreateThread() in C programs
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// built with LIBCMT.LIB if you intend to call C run-time functions.
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// Unlike the thread handle returned by _beginthread(), the thread handle
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// returned by _beginthreadex() can be used with the synchronization APIs.
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    HANDLE   hth1;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    unsigned  uiThread1ID;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    hth1 = (HANDLE)_beginthreadex( NULL,         // security
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)0,            // stack size
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)                                   ThreadX::ThreadStaticEntryPoint,
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)                                   o1,           // arg list
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)CREATE_SUSPENDED,  // so we can later call ResumeThread()
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)&uiThread1ID );
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if ( hth1 ==0 )
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        printf("Failed to create thread 1\n");
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    DWORD   dwExitCode;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif) GetExitCodeThread( hth1, &dwExitCode );  // should be STILL_ACTIVE = 0x00000103 = 259
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    printf( "initial thread 1 exit code = %u\n", dwExitCode );
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// The System::Threading::Thread object in C++/CLI has a "Name" property.
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// To create the equivalent functionality in C++ I added a public data member
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// named threadName.
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    o1->threadName ="t1";
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    ThreadX * o2 =new ThreadX( -1000000, 0, 2000 );
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    HANDLE   hth2;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    unsigned  uiThread2ID;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    hth2 = (HANDLE)_beginthreadex( NULL,         // security
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)0,            // stack size
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)                                   ThreadX::ThreadStaticEntryPoint,
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)                                   o2,           // arg list
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)                                   CREATE_SUSPENDED,  //so we can later call ResumeThread()
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)&uiThread2ID );
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if ( hth2 ==0 )
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        printf("Failed to create thread 2\n");
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)GetExitCodeThread( hth2, &dwExitCode );  // should be STILL_ACTIVE = 0x00000103 = 259
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    printf( "initial thread 2 exit code = %u\n", dwExitCode );
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    o2->threadName="t2";
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// If we hadn't specified CREATE_SUSPENDED in the call to _beginthreadex()
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// we wouldn't now need to call ResumeThread().
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif) ResumeThread( hth1 );   // serves the purpose of Jaeschke's t1->Start()
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)ResumeThread( hth2 );//你需要恢复线程的句柄 使用该函数能够激活线程的运行
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// In C++/CLI the process continues until the last thread exits.
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// That is, the thread's have independent lifetimes. Hence
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// Jaeschke's original code was designed to show that the primary
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// thread could exit and not influence the other threads.
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// However in C++ the process terminates when the primary thread exits
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// and when the process terminates all its threads are then terminated.
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// Hence if you comment out the following waits, the non-primary
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// threads will never get a chance to run.
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)WaitForSingleObject( hth1, INFINITE );
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)WaitForSingleObject( hth2, INFINITE );
   //WaitForSingleObject函数用来检测 *hHandle*事件的信号状态，当函数的执行时间超过 *dwMilliseconds*就返回，
     //但如果参数 *dwMilliseconds*为 *INFINITE*时函数将直到相应时间事件变成有信号状态才返回，否则就一直等待下去
     //，直到 WaitForSingleObject有返回直才执行后面的代码
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)GetExitCodeThread( hth1, &dwExitCode );
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    printf( "thread 1 exited with code %u\n", dwExitCode );
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)GetExitCodeThread( hth2, &dwExitCode );
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    printf( "thread 2 exited with code %u\n", dwExitCode );
         ////GetExitCodeThread这个函数是获得线程的退出码，  第二个参数是一个 DWORD的指针，
//用户应该使用一个 DWORD 类型的变量去接收数据，返回的数据是线程的退出码，
//第一个参数是线程句柄，用 CreateThread 创建线程时获得到。
//通过线程退出码可以判断线程是否正在运行，还是已经退出。
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// The handle returned by _beginthreadex() has to be closed
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// by the caller of _beginthreadex().
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)CloseHandle( hth1 );
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)CloseHandle( hth2 );
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    delete o1;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    o1 = NULL;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    delete o2;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    o2 = NULL;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    printf("Primary thread terminating.\n");
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
二解释
1）如果你正在编写C/C++代码，决不应该调用CreateThread。相反，应该使用VisualC++运行期库函数_beginthreadex，推出也应该使用_endthreadex。如果不使用Microsoft的VisualC++编译器，你的编译器供应商有它自己的CreateThred替代函数。不管这个替代函数是什么，你都必须使用。
2）因为_beginthreadex和_endthreadex是CRT线程函数，所以必须注意编译选项runtimelibaray的选择，使用MT或MTD。
3) _beginthreadex函数的参数列表与CreateThread函数的参数列表是相同的，但是参数名和类型并不完全相同。这是因为Microsoft的C/C++运行期库的开发小组认为，C/C++运行期函数不应该对Windows数据类型有任何依赖。_beginthreadex函数也像CreateThread那样，返回新创建的线程的句柄。
下面是关于_beginthreadex的一些要点：
&8226;每个线程均获得由C/C++运行期库的堆栈分配的自己的tiddata内存结构。（tiddata结构位于Mtdll.h文件中的VisualC++源代码中）。
&8226;传递给_beginthreadex的线程函数的地址保存在tiddata内存块中。传递给该函数的参数也保存在该数据块中。
&8226;_beginthreadex确实从内部调用CreateThread，因为这是操作系统了解如何创建新线程的唯一方法。
&8226;当调用CreatetThread时，它被告知通过调用_threadstartex而不是pfnStartAddr来启动执行新线程。还有，传递给线程函数的参数是tiddata结构而不是pvParam的地址。
&8226;如果一切顺利，就会像CreateThread那样返回线程句柄。如果任何操作失败了，便返回NULL。
4) _endthreadex的一些要点：
&8226;C运行期库的_getptd函数内部调用操作系统的TlsGetValue函数，该函数负责检索调用线程的tiddata内存块的地址。
&8226;然后该数据块被释放，而操作系统的ExitThread函数被调用，以便真正撤消该线程。当然，退出代码要正确地设置和传递。
5)虽然也提供了简化版的的_beginthread和_endthread，但是可控制性太差，所以一般不使用。
6）线程handle因为是内核对象，所以需要在最后closehandle。
7）更多的API：
HANDLE GetCurrentProcess();
HANDLE GetCurrentThread();
DWORD GetCurrentProcessId();
DWORD GetCurrentThreadId()。
DWORD SetThreadIdealProcessor(HANDLE hThread,DWORD dwIdealProcessor);
BOOL SetThreadPriority(HANDLE hThread,int nPriority);
BOOL SetPriorityClass(GetCurrentProcess(),  IDLE_PRIORITY_CLASS);
BOOL GetThreadContext(HANDLE hThread,PCONTEXT pContext);BOOL SwitchToThread();
三注意
1）C++主线程的终止，同时也会终止所有主线程创建的子线程，不管子线程有没有执行完毕。所以上面的代码中如果不调用WaitForSingleObject，则2个子线程t1和t2可能并没有执行完毕或根本没有执行。
2）如果某线程挂起，然后有调用WaitForSingleObject等待该线程，就会导致死锁。所以上面的代码如果不调用resumethread，则会死锁。
## [为什么要用C运行时库的_beginthreadex代替操作系统的CreateThread来创建线程？](http://www.cnblogs.com/s5689412/archive/2006/12/05/582670.html)
**来源自自1999年7月MSJ杂志的《Win32 Q&A》栏目**
你也许会说我一直用CreateThread来创建线程，一直都工作得好好的，为什么要用_beginthreadex来代替CreateThread，下面让我来告诉你为什么。
回答一个问题可以有两种方式，一种是简单的，一种是复杂的。
如果你不愿意看下面的长篇大论，那我可以告诉你简单的答案：_beginthreadex在内部调用了CreateThread，在调用之前_beginthreadex做了很多的工作，从而使得它比CreateThread更安全。
