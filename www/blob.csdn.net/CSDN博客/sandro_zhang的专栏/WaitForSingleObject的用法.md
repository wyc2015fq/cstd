# WaitForSingleObject的用法 - sandro_zhang的专栏 - CSDN博客
2011年09月13日 15:10:45[sandro_zhang](https://me.csdn.net/sandro_zhang)阅读数：305标签：[semaphore																[object																[winapi																[thread																[input](https://so.csdn.net/so/search/s.do?q=input&t=blog)](https://so.csdn.net/so/search/s.do?q=thread&t=blog)](https://so.csdn.net/so/search/s.do?q=winapi&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=semaphore&t=blog)
个人分类：[vc++](https://blog.csdn.net/sandro_zhang/article/category/884062)
Waits until the specified object is in the signaled state or the time-out interval elapses.
To enter an alertable wait state, use the [**WaitForSingleObjectEx**]() function. To wait for multiple objects, use the
[**WaitForMultipleObjects**]().
```cpp
DWORD WINAPI WaitForSingleObject(
  __in          HANDLE hHandle,
  __in          DWORD dwMilliseconds
);
```
参数*hHandle*是一个事件的句柄，第二个参数*dwMilliseconds*是时间间隔。如果时间是有信号状态返回WAIT_OBJECT_0，如果时间超过*dwMilliseconds*值但时间事件还是无信号状态则返回WAIT_TIMEOUT。
*hHandle*可以是下列对象的句柄：
- Change notification 
- Console input 
- Event 
- Memory resource notification 
- Mutex 
- Process 
- Semaphore 
- Thread 
- Waitable timer 
WaitForSingleObject函数用来检测*hHandle*事件的信号状态，当函数的执行时间超过*dwMilliseconds*就返回，但如果参数*dwMilliseconds*为*INFINITE*时函数将直到相应时间事件变成有信号状态才返回，否则就一直等待下去，直到WaitForSingleObject有返回直才执行后面的代码。在这里举个例子：
先创建一个全局Event对象g_event:
```cpp
CEvent g_event;
```
在程序中可以通过调用CEvent::SetEvent设置事件为有信号状态。
下面是一个线程函数MyThreadPro()
```cpp
UINT CFlushDlg::MyThreadProc( LPVOID pParam )
{
     WaitForSingleObject(g_event,INFINITE);
     For(;;)
        {
         ………….
        }
     return 0;
}
```
在这个线程函数中只有设置g_event为有信号状态时才执行下面的for循环，因为g_event是全局变量，所以我们可以在别的线程中通过g_event.
 SetEvent控制这个线程。
还有一种用法就是我们可以通过WaitForSingleObject函数来间隔的执行一个线程函数的函数体
```cpp
UINT CFlushDlg::MyThreadProc( LPVOID pParam )
{
     while(WaitForSingleObject(g_event,MT_INTERVAL)!=WAIT_OBJECT_0)
     {
         ………………
     }
     return 0;
}
```
在这个线程函数中可以可以通过设置MT_INTERVAL来控制这个线程的函数体多久执行一次，当事件为无信号状态时函数体隔MT_INTERVAL执行一次，当设置事件为有信号状态时，线程就执行完毕了。
借鉴链接：[http://blog.pfan.cn/embed/19089.html](http://blog.pfan.cn/embed/19089.html)
