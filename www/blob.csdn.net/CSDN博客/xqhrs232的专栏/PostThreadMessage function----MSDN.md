# PostThreadMessage function----MSDN - xqhrs232的专栏 - CSDN博客
2014年01月25日 15:19:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1017
原文地址::[http://msdn.microsoft.com/en-us/library/ms644946(VS.85).aspx](http://msdn.microsoft.com/en-us/library/ms644946(VS.85).aspx)
相关文章
1、[使用PostThreadMessage在Win32线程间传递消息](http://www.cppblog.com/sandy/archive/2005/12/31/2320.html)----[http://www.cppblog.com/sandy/archive/2005/12/31/2320.html](http://www.cppblog.com/sandy/archive/2005/12/31/2320.html)
2、[多线程：PostThreadMessage 消息可能会丢失](http://blog.csdn.net/yuanmanzheng/article/details/5471487)----[http://blog.csdn.net/yuanmanzheng/article/details/5471487](http://blog.csdn.net/yuanmanzheng/article/details/5471487)

Posts a message to the message queue of the specified thread. It returns without waiting for the thread to process the message.
## Syntax
C++
BOOL WINAPI PostThreadMessage(
  _In_  DWORD idThread,
  _In_  UINT Msg,
  _In_  WPARAM wParam,
  _In_  LPARAM lParam
);
## Parameters
- *idThread* [in]
Type: **DWORD**
The identifier of the thread to which the message is to be posted.
The function fails if the specified thread does not have a message queue. The system creates a thread's message queue when the thread makes its first call to one of the User or GDI functions. For more information, see the Remarks section.
Message posting is subject to UIPI. The thread of a process can post messages only to posted-message queues of threads in processes of lesser or equal integrity level.
This thread must have the **SE_TCB_NAME** privilege to post a message to a thread that belongs to a process with the same locally unique identifier (LUID) but is in a different desktop. Otherwise, the function fails and returns **ERROR_INVALID_THREAD_ID**.
This thread must either belong to the same desktop as the calling thread or to a process with the same LUID. Otherwise, the function fails and returns **ERROR_INVALID_THREAD_ID**.
- *Msg* [in]
Type: **UINT**
The type of message to be posted.
- *wParam* [in]
Type: **WPARAM**
Additional message-specific information.
- *lParam* [in]
Type: **LPARAM**
Additional message-specific information.
## Return value
Type:
**Type: **BOOL****
If the function succeeds, the return value is nonzero.
If the function fails, the return value is zero. To get extended error information, call [**GetLastError**](http://msdn.microsoft.com/en-us/library/ms679360(v=vs.85).aspx). **GetLastError** returns**ERROR_INVALID_THREAD_ID** if *idThread* is
 not a valid thread identifier, or if the thread specified by *idThread* does not have a message queue. **GetLastError** returns **ERROR_NOT_ENOUGH_QUOTA** when the message limit is hit.
## Remarks
When a message is blocked by UIPI the last error, retrieved with [**GetLastError**](http://msdn.microsoft.com/en-us/library/ms679360(v=vs.85).aspx),
 is set to 5 (access denied).
The thread to which the message is posted must have created a message queue, or else the call to **PostThreadMessage** fails. Use the following method to handle this situation.
- 
Create an event object, then create the thread.
- 
Use the [**WaitForSingleObject**](http://msdn.microsoft.com/en-us/library/ms687032(v=vs.85).aspx) function to wait for the event to be set
 to the signaled state before calling **PostThreadMessage**.
- 
In the thread to which the message will be posted, call [**PeekMessage**](http://msdn.microsoft.com/en-us/library/ms644943(v=vs.85).aspx) as
 shown here to force the system to create the message queue.
`PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE)`
- 
Set the event, to indicate that the thread is ready to receive posted messages.
The thread to which the message is posted retrieves the message by calling the [**GetMessage**](http://msdn.microsoft.com/en-us/library/ms644936(v=vs.85).aspx) or [**PeekMessage**](http://msdn.microsoft.com/en-us/library/ms644943(v=vs.85).aspx) function.
 The **hwnd**member of the returned [**MSG**](http://msdn.microsoft.com/en-us/library/ms644958(v=vs.85).aspx) structure is **NULL**.
Messages sent by **PostThreadMessage** are not associated with a window. As a general rule, messages that are not associated with a window cannot be dispatched by the [**DispatchMessage**](http://msdn.microsoft.com/en-us/library/ms644934(v=vs.85).aspx) function.
 Therefore, if the recipient thread is in a modal loop (as used by[**MessageBox**](http://msdn.microsoft.com/en-us/library/ms645505(v=vs.85).aspx) or [**DialogBox**](http://msdn.microsoft.com/en-us/library/ms645452(v=vs.85).aspx)),
 the messages will be lost. To intercept thread messages while in a modal loop, use a thread-specific hook.
The system only does marshalling for system messages (those in the range 0 to ([**WM_USER**](http://msdn.microsoft.com/en-us/library/ms644931(v=vs.85).aspx)-1)).
 To send other messages (those >=**WM_USER**) to another process, you must do custom marshalling.
There is a limit of 10,000 posted messages per message queue. This limit should be sufficiently large. If your application exceeds the limit, it should be redesigned to avoid consuming so many system resources. To adjust this limit, modify the following registry
 key.
**HKEY_LOCAL_MACHINE   SOFTWARE      Microsoft         Windows NT            CurrentVersion               Windows                  USERPostMessageLimit**
The minimum acceptable value is 4000.
## Requirements
Unicode and ANSI names
**PostThreadMessageW** (Unicode) and **PostThreadMessageA** (ANSI)
## See also
- **Reference**
- [**GetMessage**](http://msdn.microsoft.com/en-us/library/ms644936(v=vs.85).aspx)
- [**GetWindowThreadProcessId**](http://msdn.microsoft.com/en-us/library/ms633522(v=vs.85).aspx)
- [**MSG**](http://msdn.microsoft.com/en-us/library/ms644958(v=vs.85).aspx)
- [**PeekMessage**](http://msdn.microsoft.com/en-us/library/ms644943(v=vs.85).aspx)
- [**PostMessage**](http://msdn.microsoft.com/en-us/library/ms644944(v=vs.85).aspx)
- **Conceptual**
- [Messages and Message Queues](http://msdn.microsoft.com/en-us/library/ms632590(v=vs.85).aspx)
- **Other Resources**
- [**GetCurrentThreadId**](http://msdn.microsoft.com/en-us/library/ms683183(v=vs.85).aspx)
- [**Sleep**](http://msdn.microsoft.com/en-us/library/ms686298(v=vs.85).aspx)
- [**WaitForSingleObject**](http://msdn.microsoft.com/en-us/library/ms687032(v=vs.85).aspx)
Build date: 11/16/2013
//=======================================================================================================================
1>不是每个线程都能有消息队列，怎么样确保一个线程能有消息队列？注意Remarks部分的介绍
