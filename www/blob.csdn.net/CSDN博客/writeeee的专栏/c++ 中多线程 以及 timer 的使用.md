# c++ 中多线程 以及 timer 的使用 - writeeee的专栏 - CSDN博客
2015年08月19日 19:39:51[writeeee](https://me.csdn.net/writeeee)阅读数：566
RunCheckTaskThread() 开辟新的线程，在开辟新线程的时候为他创建了两种事件，一个是timer，一个是停止 
CheckTask是新线程执行函数
.cpp文件
```
//任务检查线程
void Task::RunCheckTaskThread()
{
    //创建两个事件
    m_CheckTaskTimer = ::CreateWaitableTimer(NULL, FALSE, NULL);
    if (NULL==m_CheckTaskTimer)
    {
        MyException::ErrorMessage("[RunCheckTaskThread] CreateWaitableTimer Failed",REPORTERROR_POINTER_NULL);
    }
    m_CheckTaskStop = ::CreateEvent(NULL, TRUE, FALSE, NULL);
    if (NULL==m_CheckTaskStop)
    {
        MyException::ErrorMessage("[RunCheckTaskThread] CreateEvent Failed",REPORTERROR_POINTER_NULL);
    }
    //创建线程
    m_CheckTaskThread=(HANDLE)::_beginthreadex(NULL,0,(unsigned (__stdcall*)(void*))CheckTask,(LPVOID)this,0,NULL);
    if (NULL==m_CheckTaskThread)
    {
        MyException::ErrorMessage("_beginthreadex create Thread Failed",REPORTERROR_POINTER_NULL);
    }
}
//检查是否有打印任务
DWORD WINAPI CheckTask(PVOID lp)
{
    Task* myTask=Task::GetTaskInstance();
    //设置事件列表
    const int eventLen=2;
    HANDLE hEvent[eventLen] = {0};
    hEvent[0]=myTask->m_CheckTaskTimer;
    hEvent[1]=myTask->m_CheckTaskTimer;
    //设置timer
    LARGE_INTEGER dueTime;
    dueTime.QuadPart = 0;
    long lPeriod = 1000;
    ::SetWaitableTimer(myTask->m_CheckTaskTimer, &dueTime, lPeriod, NULL, NULL, 0);
    //进入timer循环
    while(1)
    {
        DWORD nRet = ::WaitForMultipleObjects(eventLen, hEvent, FALSE, INFINITE);
        if(WAIT_OBJECT_0 == nRet)
        {
            break;
        }
        else if(WAIT_OBJECT_0 + 1 == nRet)
        {   
            time_t curTime = time(NULL);
            //循环检查一遍是否有任务计划要执行
            for (TaskVec::iterator iter=g_PlanInfo.begin();iter!=g_PlanInfo.end();iter++)
            {
                //检查1s内的需要打印的计划
                if ((*iter).NextExcuteTime>curTime-1&&(*iter).NextExcuteTime<=curTime)
                {
                    myTask->Lock();
                    g_TaskQue.push(*iter);
                    myTask->UnLock();
                }
            }
        }
        ::CancelWaitableTimer(myTask->m_CheckTaskTimer);
        ::CloseHandle(myTask->m_CheckTaskTimer);
        myTask->m_CheckTaskTimer = NULL;
        ::CloseHandle(myTask->m_CheckTaskStop);
        myTask->m_CheckTaskStop = NULL;
    }
    return 0;
}
```
注意 下边的这两个函数都是静态的。 
DWORD WINAPI  CheckTask(PVOID lp); 
Task::GetTaskInstance() 
