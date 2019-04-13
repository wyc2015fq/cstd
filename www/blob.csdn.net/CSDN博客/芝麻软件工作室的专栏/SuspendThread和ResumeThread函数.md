
# SuspendThread和ResumeThread函数 -  芝麻软件工作室的专栏 - CSDN博客


2012年05月07日 10:53:41[seven-soft](https://me.csdn.net/softn)阅读数：334


操作系统对线程有几种状态的变化：执行，挂起和恢复执行。
当线程做完任务或者现在想暂停线程运行，就需要使用SuspendThread来暂停线程的执行，当然恢复线程的执行就是使用ResumeThread函数了。这两个函数使用很简单的，下面就来看看例子是怎么样使用的。
函数SuspendThread和ResumeThread声明如下：
WINBASEAPI
DWORD
WINAPI
SuspendThread(
__in HANDLE hThread
);
WINBASEAPI
DWORD
WINAPI
ResumeThread(
__in HANDLE hThread
);
**hThread**是线程的句柄。
调用函数的例子如下：
\#001  //线程的暂停和恢复。
\#002  //
\#003  void ThreadSuspendResume(void)
\#004  {
\#005         ::SuspendThread(m_hThread);
\#006
\#007         Sleep(10);
\#008         ::ResumeThread(m_hThread);
\#009  }
\#010
第5行是暂停线程执行。
第8行是继续线程执行


