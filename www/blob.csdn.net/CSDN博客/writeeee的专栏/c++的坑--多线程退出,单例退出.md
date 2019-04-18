# c++的坑--多线程退出,单例退出 - writeeee的专栏 - CSDN博客
2016年01月18日 23:34:00[writeeee](https://me.csdn.net/writeeee)阅读数：2618
代码说明 有个单例类，有个调用主函数 ,代码主要说明使用多线程时如何正确退出。
单例类代码
```
#pragma once
#include <concrt.h>
class AddData
{
public:
    static AddData* Instance() {
        if (!m_pInstance)
        {
            m_pInstance = new AddData();
        }
        return m_pInstance;
    };
public:
    AddData();
    ~AddData() {};
    void StartThread();
    DWORD KeepThreadRun();
private:
    static AddData* m_pInstance;
    // 同步线程
    HANDLE  m_hKeepExitEvent;
    HANDLE  m_hKeepInfoThread;
};
```
```
#include "stdafx.h"
#include "AddData.h"
#include <winbase.h>
#include <wtypes.h>
#include <afx.h>
// 同步线程
HANDLE  m_hKeepExitEvent =NULL;
HANDLE  m_hKeepInfoThread = NULL;
AddData::AddData()
{
    m_hKeepInfoThread = NULL;
}
void AddData::StartThread()
{
    if (m_hKeepInfoThread == NULL)
    {
        DWORD dwThreadId = 0;
        m_hKeepExitEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        m_hKeepInfoThread = CreateThread(NULL, 0, KeepThreadProc, this, 0, &dwThreadId);
    }
}
DWORD  WINAPI KeepThreadProc()
{
    AddData *pInstan = AddData::Instance();
    pInstan->KeepThreadRun();
}
DWORD AddData::KeepThreadRun()
{
    ULONG uIntervalTime = 5000;
    while (true)
    {
        int iRet= WaitForSingleObject(m_hKeepExitEvent,uIntervalTime);
        if (iRet == WAIT_OBJECT_0)
        {
            TRACE("KeepThreadRun 退出");
            //TRACE 如何使用
            //1.#include <afx.h>
            //2.project->setting, 选中use MFC in a shared DLL;
            //3.project->setting->c / c++, category中选code generation, 然后在use run - time library中选debug multithreaded dll
            break;
        }
        if (iRet<0)
        {
            continue;
        }
        TRACE("KeepThreadRun runing");
        //::EnterCriticalSection( &g_criticalSection );
        //SaveData();
        //::LeaveCriticalSection(&g_criticalSection);
    }
}
```
```
// ComTest.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include "AddData.h"
CRITICAL_SECTION        g_criticalSection;
int main()
{
    ::InitializeCriticalSection(&g_criticalSection);
    AddData *AddDataInstance =  AddData::Instance();
    AddDataInstance->KeepThreadRun();
    Sleep(3000);
    ::DeleteCriticalSection(&g_criticalSection);
    delete AddDataInstance;
    return 0;
}
```
主要说两点问题 
        1.单例中的线程在周期性存储 调用了 公共锁，如果退出时销毁公共锁，会导致存储时崩溃。因此要先销毁单例，在析构函数中将线程关闭。
```
if (m_hKeepInfoThread !=NULL)
    {
        SetEvent(m_hKeepExitEvent);
        ::WaitForSingleObject(m_hKeepInfoThread, INFINITE);
        ::CloseHandle(m_hKeepInfoThread);
        m_hKeepInfoThread = NULL;
        ::CloseHandle(m_hKeepExitEvent);
        m_hKeepExitEvent = NULL;
    }
```
```
2 单例的析构函数一定要将 m_instance 实例置为空，否则下一次创建时，检测m_instance 不为空，不会创建新的实例，导致调用失败。
  3 注意单例类的头文件中  ~AddData() {}; 就直接实现了析构，尽量不要这样写，因为你在多数情况下要进行清理工作。
```
