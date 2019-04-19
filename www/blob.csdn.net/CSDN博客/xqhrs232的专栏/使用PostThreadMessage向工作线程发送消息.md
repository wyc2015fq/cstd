# 使用PostThreadMessage向工作线程发送消息 - xqhrs232的专栏 - CSDN博客
2014年01月22日 11:04:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：939
原文地址::[http://blog.csdn.net/liuliu20036/article/details/3879152](http://blog.csdn.net/liuliu20036/article/details/3879152)
相关文章
1、[多线程：PostThreadMessage 消息可能会丢失](http://blog.csdn.net/yuanmanzheng/article/details/5471487)----[http://blog.csdn.net/yuanmanzheng/article/details/5471487](http://blog.csdn.net/yuanmanzheng/article/details/5471487)
2、**使用PostThreadMessage在Win32线程间传递消息_深水蓝----[http://www.360doc.com/content/07/0622/12/198_573251.shtml](http://www.360doc.com/content/07/0622/12/198_573251.shtml)**
3\[使用PostThreadMessage在Win32线程间传递消息](http://www.cppblog.com/sandy/archive/2005/12/31/2320.html)----[http://www.cppblog.com/sandy/archive/2005/12/31/2320.html](http://www.cppblog.com/sandy/archive/2005/12/31/2320.html)
把window线程间传送消息仔细的看了一遍，觉得以前的理解很不深刻。说一说对PostThreadMessage的理解。
PostThreadMessage是一个线程体发送一个消息到指定的线程ID，其原型如下：
BOOL PostThreadMessage(         
                    DWORD idThread,
                    UINT Msg,  
                    WPARAM wParam,   
                    LPARAM lParam
);
       这个函数既可以发送消息给工作线程，也可以发送给UI线程。接受PostThreadMessage的线程必须已经有了一个message queue，否则调用PostThreadMessage会失败。因为此原因使用GetLastError会得到错误码为1444，这种情况经常出现，解决方法有如下两种：
1.         调用PostThreadMessage，如果失败，就Sleep一段时间再次调用PostThreadMessage直到调用成功；
2.         创建一个Event对象，让PostThreadMessage等待接受的线程创建一个message queue。可以通过调用PeekMessage强制系统创建一个message queue。示例代码如下：
假设mainAPP是发送线程ThreadA是接受线程
/*mainAPP.cpp*/
……
hStartEvent = ::CreateEvent(0,FALSE,FALSE,0); //create thread start event
if(hStartEvent == 0)
{
          printf("create start event failed,errno:%d/n",::GetLastError());
          return 1;
}
::WaitForSingleObject(hStartEvent,INFINITE);
CloseHandle(hStartEvent);
if(!PostThreadMessage(threadaID, WM_MESSAGE_A,0,0))
{
          _tprintf(_T("post error! %d/n"), GetLastError());
          return 1;
}
……
ThreadA是接受线程
/* ThreadA */
MSG msg;
PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
if(!SetEvent(hStartEvent))
{
          printf("set event error,%d/n",GetLastError());
          return 1;
}
while(true){
          if(GetMessage(&msg, 0,0,0)) {
                    switch(msg.message){
                              case WM_MESSAGE_A:
                               ……
                               break;
                              }
                    }
          }
}
PostThreadMessage传递的消息如果要包含信息，要注意在结束的时候释放消息中的信息。在消息中附加信息方法如下
/*构造信息如下*/
char* pInfo = new char[MAX_INFO_SIZE]; //create dynamic msg
sprintf(pInfo,"msg_%d",++count);
PostThreadMessage(nThreadID,MY_MSG,(WPARAM)pInfo,0)//post thread msg
/*解释信息如下*/
if(GetMessage(&msg,0,0,0)) //get msg from message queue
{
            switch(msg.message)
            {
            case MY_MSG:
            char * pInfo = (char *)msg.wParam;
            printf("recv %s/n",pInfo);
            delete[] pInfo; //这里释放了资源
            break;
            }
}
做了一个简单的消息通信实验，让主线程中等待用户输入，产生不同的消息，并把这些消息post给子线程，子线程根据产生的消息做出不同的反映。这些子线程可以是工作线程也可以是UI线程。
#include <windows.h>
#include <cstdio>
#include <process.h>
#define MY_MSG WM_USER+100
const int MAX_INFO_SIZE = 20;
HANDLE hStartEvent; // thread start event
// thread function
unsigned __stdcall fun(void *param)
{
    printf("thread fun start/n");
    MSG msg;
    PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    if(!SetEvent(hStartEvent)) //set thread start event
    {
        printf("set start event failed,errno:%d/n",::GetLastError());
        return 1;
    }
    while(true)
    {
        if(GetMessage(&msg,0,0,0)) //get msg from message queue
        {
            switch(msg.message)
            {
            case MY_MSG:
                char * pInfo = (char *)msg.wParam;
                printf("recv %s/n",pInfo);
                delete[] pInfo;
                break;
            }
        }
    };
    return 0;
}
int main()
{
    HANDLE hThread;
    unsigned nThreadID;
    hStartEvent = ::CreateEvent(0,FALSE,FALSE,0); //create thread start event
    if(hStartEvent == 0)
    {
        printf("create start event failed,errno:%d/n",::GetLastError());
        return 1;
    }
    //start thread
    hThread = (HANDLE)_beginthreadex( NULL, 0, &fun, NULL, 0, &nThreadID );
    if(hThread == 0)
    {
        printf("start thread failed,errno:%d/n",::GetLastError());
        CloseHandle(hStartEvent);
        return 1;
    }
    //wait thread start event to avoid PostThreadMessage return errno:1444
    ::WaitForSingleObject(hStartEvent,INFINITE);
    CloseHandle(hStartEvent);
    int count = 0;
    while(true)
    {
        char* pInfo = new char[MAX_INFO_SIZE]; //create dynamic msg
        sprintf(pInfo,"msg_%d",++count);
        if(!PostThreadMessage(nThreadID,MY_MSG,(WPARAM)pInfo,0))//post thread msg
        {
            printf("post message failed,errno:%d/n",::GetLastError());
            delete[] pInfo;
        }
        ::Sleep(1000);
    }
    CloseHandle(hThread);
    return 0;
}
要把SETTING 改为多线程的
Project->Settings->click C/C tab,
在Category 中选Code Generation, 然后在Use run-time libray 中选一个
Multithread 配置
