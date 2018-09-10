windows/linux下实现线程的方式是完全不一样的。于是我们先定义一个线程类。

[cpp] view plaincopy在CODE上查看代码片派生到我的代码片

#ifndef _OSAPI_THREAD_H  
#define _OSAPI_THREAD_H  
  
/* presented by 阿发你好 */  
class OS_Thread  
{  
public:  
    OS_Thread();  
    virtual ~OS_Thread();  
  
    // 创建并启动  
    virtual int Run();  
  
    // 等待和收回资源  
    static void Join(OS_Thread* thrd);  
  
    // Sleep函数  
    static void Msleep(int ms);  
    static void Sleep(int s);  
  
public:  
    virtual int Routine() = 0;  
  
private:  
    void* m_Priv;  
};  
  
  
#endif  

 

也就是说，无论你是XXX平台还是YYY平台，总该是有线程支持的。所以麻烦你先按照上述接口实现一个线程类吧。我在业务代码里要使用线程的时候，就使用线程接口，完全不使用平台相关的接口了。

[cpp] view plaincopy在CODE上查看代码片派生到我的代码片

#include "../osapi/Thread.h"  
  
  
class MyThread : public OS_Thread  
{  
public:  
    int Create()  
    {  
        m_quitflag = false;  
        run();  
        return 0;  
    }  
      
    void Destroy()  
    {  
        m_quitflag = true;  
        Join(this);  
    }  
  
private:  
    /* 线程函数 */  
    virtual int Routine()  
    {  
        while(!m_quitflag)  
        {  
            printf("helloafa!\n");  
            OS_Thread::msleep(500);  
        }  
        return 0;  
    }  
      
private:  
    bool m_quitflag;  
};  

在Windows上，我们写一份实现Thread_Win32.cpp


 

[cpp] view plaincopy在CODE上查看代码片派生到我的代码片

#ifdef _WIN32  
  
#include <windows.h>  
#include <process.h>   
  
#include "Thread.h"  
  
struct OS_Thread_Priv  
{  
    HANDLE hThread;  
};  
  
OS_Thread::OS_Thread()   
: m_Priv(NULL)  
{  
}  
  
OS_Thread::~OS_Thread()  
{  
    if(m_Priv)  
    {  
        OS_Thread_Priv* priv = (OS_Thread_Priv*) m_Priv;  
        delete priv;  
    }  
}  
  
static DWORD WINAPI OS_Thread_Proc_Win32_1(LPVOID param)  
{  
    OS_Thread* thrd = (OS_Thread*) param;  
    thrd->Routine();  
  
    return 0;  
}  
  
static void OS_Thread_Proc_Win32_2(void* param)  
{  
    OS_Thread* thrd = (OS_Thread*) param;  
    thrd->Routine();  
}  
  
static unsigned int WINAPI OS_Thread_Proc_Win32_3(void* param)  
{  
    OS_Thread* thrd = (OS_Thread*) param;  
    thrd->Routine();  
    return 0;  
}  
  
int OS_Thread::Run()  
{  
    // 创建私有结构  
    OS_Thread_Priv* priv = new OS_Thread_Priv;  
    if(!priv) return -1;  
  
    m_Priv = priv;  
  
    // 创建线程  
//  DWORD nTheadId;  
//  priv->hThread = _beginthreadex(NULL, NULL, OS_Thread_Proc_Win32, this,  0, &nTheadId);  
//  priv->hThread = (HANDLE) _beginthread(OS_Thread_Proc_Win32, 0, this);  
    unsigned int thrdaddr ;  
    priv->hThread = (HANDLE) _beginthreadex(NULL, 0, OS_Thread_Proc_Win32_3, this, 0, &thrdaddr);  
    if(priv->hThread == NULL)  
    {  
        delete priv;  
        m_Priv = NULL;  
        return -1;  
    }  
  
    return 0;  
}  
  
void OS_Thread::Join(OS_Thread* thrd)  
{  
    OS_Thread_Priv* priv = (OS_Thread_Priv*) thrd->m_Priv;  
    if(priv)  
    {  
        WaitForSingleObject(priv->hThread, INFINITE);  
//      CloseHandle(priv->hThread);  
        // 删除资源  
        delete priv;  
        thrd->m_Priv = NULL;  
    }  
}  
  
void OS_Thread::Msleep(int ms)  
{  
    ::Sleep(ms);  
}  
  
void OS_Thread::Sleep(int s)  
{  
    ::Sleep(s * 1000);  
}  
  
int OS_Thread::Routine()  
{  
    return 0;  
}  
  
#endif  //_WIN32  

在Linux上的实现Thread_Linux.cpp （注间Thread_Linux.cpp和Thread_Win32.cpp是同时存在于你的项目里的，上面已经用#ifndef _WIN32给隔开了，不影响你的编译）

[cpp] view plaincopy在CODE上查看代码片派生到我的代码片

#ifndef _WIN32  
//#if 1  
#include <pthread.h>  
#include <unistd.h>  
#include <time.h>  
#include "Thread.h"  
  
  
struct OS_Thread_Priv  
{  
    pthread_t hThread;  
};  
  
OS_Thread::OS_Thread()   
: m_Priv(NULL)  
{  
}  
  
OS_Thread::~OS_Thread()  
{  
    if(m_Priv)  
    {  
        OS_Thread_Priv* priv = (OS_Thread_Priv*) m_Priv;  
        delete priv;  
    }  
}  
  
static void* OS_Thread_Proc_Linux(void* param)  
{  
    OS_Thread* thrd = (OS_Thread*) param;  
    thrd->Routine();  
    return NULL;  
}  
  
int OS_Thread::Run()  
{  
    // 创建私有结构  
    OS_Thread_Priv* priv = new OS_Thread_Priv;  
    if(!priv) return -1;  
  
    m_Priv = priv;  
  
    // 创建线程  
    if(pthread_create(&priv->hThread, NULL, OS_Thread_Proc_Linux, this) < 0)  
    {  
        delete priv;  
        m_Priv = NULL;  
        return -1;  
    }  
  
    return 0;  
}  
  
void OS_Thread::Join(OS_Thread* thrd)  
{  
    OS_Thread_Priv* priv = (OS_Thread_Priv*) thrd->m_Priv;  
    if(priv)  
    {  
        pthread_join(priv->hThread, NULL);  
  
        // 删除资源  
        delete priv;  
        thrd->m_Priv = NULL;  
    }  
}  
  
void OS_Thread::Msleep(int ms)  
{  
    //::usleep(ms * 1000);  
    // 好像使用nanosleep更好  
  
    timespec ts;  
    ts.tv_sec = ms / 1000;  
    ts.tv_nsec = (ms % 1000) * 1000000;  
    nanosleep(&ts, NULL);  
}  
  
void OS_Thread::Sleep(int s)  
{  
    ::sleep(s);  
}  
  
int OS_Thread::Routine()  
{  
    return 0;  
}  
  
  
  
  
#endif // ! _WIN32  
