# 多线程CreateThread函数的用法及注意事项 - 文章 - 伯乐在线
原文出处： [BeyondTechnology](http://www.cnblogs.com/BeyondTechnology/archive/2010/08/30/1813082.html)
当使用CreateProcess调用时，系统将创建一个进程和一个主线程。CreateThread将在主线程的基础上创建一个新线程，大致做如下步骤：
- 在内核对象中分配一个线程标识/句柄，可供管理，由CreateThread返回
- 把线程退出码置为STILL_ACTIVE，把线程挂起计数置1
- 分配context结构
- 分配两页的物理存储以准备栈，保护页设置为PAGE_READWRITE，第2页设为PAGE_GUARD
- lpStartAddr和lpvThread值被放在栈顶，使它们成为传送给StartOfThread的参数
- 把context结构的栈指针指向栈顶（第5步）指令指针指向startOfThread函数
语法：

C++
```
hThread = CreateThread(&security_attributes, dwStackSize, ThreadProc,pParam, dwFlags, &idThread) ;
HANDLE CreateThread(
  LPSECURITY_ATTRIBUTES lpThreadAttributes, 　　　　　// pointer to security attributes
  DWORD dwStackSize,　　　　　　　　　　　　　　　　　　// initial thread stack size
  LPTHREAD_START_ROUTINE lpStartAddress, 　　　　　　// pointer to thread function
  LPVOID lpParameter,　　　　　　　　　　　　　　　　　　　// argument for new thread
  DWORD dwCreationFlags,　　　　　　　　　　　　　　　　// creation flags
  LPDWORD lpThreadId　　　　　　　　　　　　　　　　　　// pointer to receive thread ID
);
```
第一个参数是指向SECURITY_ATTRIBUTES型态的结构的指针。在Windows 98中忽略该参数。在Windows NT中，它被设为NULL。
第二个参数是用于新线程的初始堆栈大小，默认值为0。在任何情况下，Windows根据需要动态延长堆栈的大小。
第三个参数是指向线程函数的指标。函数名称没有限制，但是必须以下列形式声明：

C++
```
DWORD WINAPI ThreadProc (PVOID pParam) ;
```
第四个参数为传递给ThreadProc的参数。这样主线程和从属线程就可以共享数据。
第五个参数通常为0，但当建立的线程不马上执行时为旗标CREATE_SUSPENDED。线程将暂停直到呼叫ResumeThread来恢复线程的执行为止。
第六个参数是一个指标，指向接受执行绪ID值的变量。
注意：临界区要在线程执行前初始化，因为线程一但被建立即开始运行（除非手工挂起），但线程建立后在初始化临界区可能出现问题
如：孙鑫MFC中的现象

C++
```
int main() 
{ 
InitializeCriticalSection(&g_cs); //先初始化临界区
HANDLE handle1=CreateThread(NULL,0,ThreadProc1,NULL,0,NULL); 
HANDLE handle2=CreateThread(NULL,0,ThreadProc2,NULL,0,NULL); 
CloseHandle(handle1); 
CloseHandle(handle2); 
Sleep(4000); 
DeleteCriticalSection(&g_cs); 
return 0; 
}
```
