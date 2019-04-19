# C++多线程（一）  - maopig的专栏 - CSDN博客
2011年05月19日 23:16:00[maopig](https://me.csdn.net/maopig)阅读数：574标签：[多线程																[c++																[thread																[工作																[api																[dll](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=工作&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=多线程&t=blog)
## [C++多线程（一）](http://www.cppblog.com/mzty/archive/2007/07/24/28713.html)
WIN 多线程API
一 简单实例
比较简单的代码，创建10个线程，其中使第4个线程在一创建就挂起，等到其他的线程执行的差不多的时候再使第4个线程恢复执行。
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <stdio.h>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <stdlib.h>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#include <windows.h>
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)#define THREAD_NUM 10
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)DWORD WINAPI PrintThreads (LPVOID);
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)int main () 
![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    HANDLE hThread[THREAD_NUM]; 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    DWORD dwThreadID[THREAD_NUM]; 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)for (int i=0; i<THREAD_NUM; ++i) 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)int isStartImmediate =0;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if(3== i)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            isStartImmediate = CREATE_SUSPENDED;
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        hThread[i]=CreateThread(NULL,                // security attributes that should be applied to the new thread, 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// this is for NT. Use NULL to get the default security attributes. Use NULL for win95 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)0,                                            // default size of 1MB can be passed by passing zero. 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)                                PrintThreads,                     // function name:address of the function where the new thread starts.
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)                                (LPVOID)i,                         // parameter(void pointer): pointer to the 32 bit parameter that will be passed into the thread
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)                                isStartImmediate,             // flags to control the creation of the thread. Passing zero starts the thread immediately. 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// Passing CREATE_SUSPENDED suspends the thread until the ResumeThread( ) function is called.
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)&dwThreadID[i]            // pointer to a 32-bit variable that receives the thread identifier.
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)                                ); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if (hThread[i])
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){ 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            printf ("Thread launched successfully/n");                
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    printf("Start sleep 100, and let other thread excute/n");
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    Sleep (100);    
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    printf("Start sleep 100, and thread 3 excute/n");
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    ResumeThread(hThread[3]);
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)    Sleep(100);
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)for(int i =0; i<THREAD_NUM; ++i)
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)if (hThread[i])
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockStart.gif)![](http://www.cppblog.com/Images/OutliningIndicators/ContractedSubBlock.gif)![](http://www.cppblog.com/Images/dot.gif){            
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)            CloseHandle(hThread[i]);    // You need to use this to release kernel objects when you are done using them. 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// If a process exits without closing the thread handle, 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// the operating system drops the reference counts for those objects. 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// But if a process frequently creates threads without closing the handles, 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)// there could be hundreds of thread kernel objects lying around and these resource leaks can have a big hit on performance.
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)        }
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedSubBlockEnd.gif)    }
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)return (0); 
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)//function PrintThreads 
![](http://www.cppblog.com/Images/OutliningIndicators/None.gif)DWORD WINAPI PrintThreads (LPVOID num)
![](http://www.cppblog.com/Images/dot.gif){
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)for (int i=0; i<10; i++) 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)        printf ("Thread Number is %d%d%d/n", num,num,num); 
![](http://www.cppblog.com/Images/OutliningIndicators/InBlock.gif)return0;
![](http://www.cppblog.com/Images/OutliningIndicators/ExpandedBlockEnd.gif)}
二 其他基本API的说明
CreateThread() 调用成功返回句柄和一个id。
CloseHandle()  关闭一个打开的对象句柄，该对象句柄可以是线程句柄，也可以是进程、信号量等其他内核对象的句柄.
SuspendThread(HANDLE) 允许开发人员将HANDLE指定的线程挂起，如果要挂起的线程占有共享资源，则可能导致死锁。
ResumeThread(HANDLE)  恢复指定的线程。
TerminateThread() 立即终止线程的工作，不做任何清理工作。
ExitThread() 线程函数返回时回调用次函数，所以一般我们不去显示的调用。
ExitThread是推荐使用的结束一个线程的方法，当调用该函数时，当前线程的栈被释放，然后线程终止，相对于TerminateThread函数来说，这样做能够更好地完成附加在该线程上的DLL的清除工作. 但是ExitThread()会导致线程在清处构造器/自动变量之前就终止，所以我们最好不要显示的调用ExitThread()。
