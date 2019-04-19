# VC下线程安全退出的方法----百度文章 - xqhrs232的专栏 - CSDN博客
2013年12月28日 11:35:48[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1450
原文地址::[http://wenku.baidu.com/link?url=9Ab59gzee2LZYGZrwx74i2-wOp5KRNgHFY2-SvCt8_3TV0jJTIUXif_nFi-Qfm0wqorlK2vFVp26ITbbcvvOAEmT8XIxl6EmtyVFrVzQ0AC](http://wenku.baidu.com/link?url=9Ab59gzee2LZYGZrwx74i2-wOp5KRNgHFY2-SvCt8_3TV0jJTIUXif_nFi-Qfm0wqorlK2vFVp26ITbbcvvOAEmT8XIxl6EmtyVFrVzQ0AC)
相关文章
1、线程安全退出【转载】----[http://wsyjwps1983.blog.163.com/blog/static/6800900120122155112865/](http://wsyjwps1983.blog.163.com/blog/static/6800900120122155112865/)
若要终止线程的运行，可以使用下面的方法：
1.线程函数返回（最好使用这种方法）。
2.通过调用ExitThread函数，线程将自行撤消（最好不要使用这种方法）。
3.同一个进程或另一个进程中的线程调用TerminateThread函数（应该避免使用这种方法）。
4.包含线程的进程终止运行，（应该避免使用这种方法）。
如果使用方法1，就可以确保下列事项的实现：
1.在线程函数中创建的所有C + +对象均将通过它们的撤消函数正确地撤消。
2.操作系统将正确地释放线程堆栈使用的内存。
3.系统将线程的退出代码（在线程的内核对象中维护）设置为线程函数的返回值。
4.系统将递减线程内核对象的使用计数。
示例:源代码中threadtest中。
UINT ThreadProc(LPVOID pParam) 
{
TRACE("ThreadProc:in ...\n"); 
while(1)
{
if(WaitForSingleObject(g_exitevent.m_hObject, 0) == WAIT_OBJECT_0)
{
break;
}
Sleep(1000);
}
TRACE("ThreadProc:exit ...\n");
return 100;  //这个值可以用GetExitCodeThread获得
}
BOOL CThreadtestDlg::StopThread() 
{ 
if (m_pThread == NULL)
{ 
       TRACE("StopThread:thread already exit...\n");
       return FALSE; 
} 
g_exitevent.SetEvent();
        //等待线程结束，只等待2秒，
int nTimes=100;   
while(1)
{   
DWORD dwExitCode;
        // 检索线程的退出代码前要求线程对象还没有退出
        //如果m_pThread->m_bAutoDelete没有设置成FALSE,就不能获得dwExitCode
//并且GetExitCodeThread会返回FALSE
::GetExitCodeThread(m_pThread->m_hThread, &dwExitCode);
if( dwExitCode != STILL_ACTIVE)
{ 
                       TRACE("StopThread:thread exitcode %d...\n",dwExitCode);
break;
}
if(nTimes--==0)
{
TRACE("StopThread:stop thread error...\n");
TerminateThread(m_pThread->m_hThread,0);  //强行结束，不推荐
break;
}
Sleep(20);
}
   delete m_pThread;  //因为m_pThread->m_bAutoDelete=FALSE，所以要自行删除线程对象
   m_pThread = NULL; 
   return TRUE;
}

void CThreadtestDlg::OnOK() 
{
// TODO: Add extra validation here
//启动线程，初始为挂起状态,类成员变量 CWinThread* m_pThread
m_pThread = AfxBeginThread(ThreadProc, NULL ,THREAD_PRIORITY_ABOVE_NORMAL, 0, CREATE_SUSPENDED);
//线程结束时不自动删除
m_pThread->m_bAutoDelete = FALSE; 
//恢复线程运行 
m_pThread->ResumeThread(); 
}
//=========================================================================================================
备注::
1》最好使用这种方法----线程函数返回
2》可以去等待一个线程的句柄的有无状态来决定一个线程是否真的退出
