# GetMessage function (Windows) - xqhrs232的专栏 - CSDN博客
2012年11月29日 22:30:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：640
原文地址::[http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644936(v=vs.85).aspx](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644936(v=vs.85).aspx)
41（共 68）对本文的评价是有帮助 - [评价此主题](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644936(v=vs.85).aspx#feedback)
Retrieves a message from the calling thread's message queue. The function dispatches incoming sent messages until a posted message is available for retrieval.
Unlike **GetMessage**, the [**PeekMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644943(v=vs.85).aspx) function
 does not wait for a message to be posted before returning.
### Syntax
C++
BOOL WINAPI GetMessage(
  _Out_     LPMSG lpMsg,
  _In_opt_  HWND hWnd,
  _In_      UINT wMsgFilterMin,
  _In_      UINT wMsgFilterMax
);
### Parameters
- *lpMsg* [out]
Type: **LPMSG**
A pointer to an [**MSG**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644958(v=vs.85).aspx) structure that
 receives message information from the thread's message queue.
- *hWnd* [in, optional]
Type: **HWND**
A handle to the window whose messages are to be retrieved. The window must belong to the current thread.
If *hWnd* is **NULL**, **GetMessage** retrieves messages for any window that belongs to the current thread, and any messages on the current thread's message queue whose **hwnd** value is **NULL**(see
 the [**MSG**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644958(v=vs.85).aspx) structure). Therefore if hWnd
 is **NULL**, both window messages and thread messages are processed.
If *hWnd* is -1, **GetMessage** retrieves only messages on the current thread's message queue whose **hwnd** value is **NULL**, that is, thread messages as posted by [**PostMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644944(v=vs.85).aspx) (when
 the *hWnd*parameter is **NULL**) or [**PostThreadMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644946(v=vs.85).aspx).
- *wMsgFilterMin* [in]
Type: **UINT**
The integer value of the lowest message value to be retrieved. Use **WM_KEYFIRST** (0x0100) to specify the first keyboard message or **WM_MOUSEFIRST** (0x0200) to specify the first mouse message.
Use [**WM_INPUT**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms645590(v=vs.85).aspx) here and in *wMsgFilterMax* to
 specify only the **WM_INPUT** messages.
If *wMsgFilterMin* and *wMsgFilterMax* are both zero, **GetMessage** returns all available messages (that is, no range filtering is performed).
- *wMsgFilterMax* [in]
Type: **UINT**
The integer value of the highest message value to be retrieved. Use **WM_KEYLAST** to specify the last keyboard message or **WM_MOUSELAST** to specify the last mouse message.
Use [**WM_INPUT**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms645590(v=vs.85).aspx) here and in *wMsgFilterMin* to
 specify only the **WM_INPUT** messages.
If *wMsgFilterMin* and *wMsgFilterMax* are both zero, **GetMessage** returns all available messages (that is, no range filtering is performed).
### Return value
Type:
**Type: **BOOL****
If the function retrieves a message other than [**WM_QUIT**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms632641(v=vs.85).aspx),
 the return value is nonzero.
If the function retrieves the [**WM_QUIT**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms632641(v=vs.85).aspx) message,
 the return value is zero.
If there is an error, the return value is -1. For example, the function fails if *hWnd* is an invalid window handle or *lpMsg* is an invalid pointer. To get extended error information, call [**GetLastError**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms679360(v=vs.85).aspx).
Because the return value can be nonzero, zero, or -1, avoid code like this:

while (GetMessage( lpMsg, hWnd, 0, 0)) ...
The possibility of a -1 return value means that such code can lead to fatal application errors. Instead, use code like this:

BOOL bRet;
while( (bRet = GetMessage( &msg, hWnd, 0, 0 )) != 0)
{ 
    if (bRet == -1)
    {
        // handle the error and possibly exit
    }
    else
    {
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }
}
### Remarks
An application typically uses the return value to determine whether to end the main message loop and exit the program.
The **GetMessage** function retrieves messages associated with the window identified by the *hWnd*parameter or any of its children, as specified by the [**IsChild**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms633524(v=vs.85).aspx) function,
 and within the range of message values given by the *wMsgFilterMin* and *wMsgFilterMax* parameters. Note that an application can only use the low word in the *wMsgFilterMin* and *wMsgFilterMax* parameters; the high word is reserved
 for the system.
Note that **GetMessage** always retrieves [**WM_QUIT**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms632641(v=vs.85).aspx) messages,
 no matter which values you specify for*wMsgFilterMin* and *wMsgFilterMax*.
During this call, the system delivers pending, nonqueued messages, that is, messages sent to windows owned by the calling thread using the [**SendMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644950(v=vs.85).aspx), [**SendMessageCallback**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644951(v=vs.85).aspx), [**SendMessageTimeout**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644952(v=vs.85).aspx),
 or[**SendNotifyMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644953(v=vs.85).aspx) function. Then
 the first queued message that matches the specified filter is retrieved. The system may also process internal events. If no filter is specified, messages are processed in the following order:
- Sent messages
- Posted messages
- Input (hardware) messages and system internal events
- Sent messages (again)
- [**WM_PAINT**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/dd145213(v=vs.85).aspx) messages
- [**WM_TIMER**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644902(v=vs.85).aspx) messages
To retrieve input messages before posted messages, use the *wMsgFilterMin* and *wMsgFilterMax*parameters.
**GetMessage** does not remove [**WM_PAINT**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/dd145213(v=vs.85).aspx) messages
 from the queue. The messages remain in the queue until processed.
If a top-level window stops responding to messages for more than several seconds, the system considers the window to be not responding and replaces it with a ghost window that has the same z-order, location, size, and visual attributes. This allows the user
 to move it, resize it, or even close the application. However, these are the only actions available because the application is actually not responding. When in the debugger mode, the system does not generate a ghost window.
### Examples
For an example, see [Creating a Message Loop](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644928(v=vs.85).aspx#creating_loop).
### Requirements
Unicode and ANSI names
**GetMessageW** (Unicode) and **GetMessageA** (ANSI)
### See also
- **Reference**
- [**IsChild**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms633524(v=vs.85).aspx)
- [**MSG**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644958(v=vs.85).aspx)
- [**PeekMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644943(v=vs.85).aspx)
- [**PostMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644944(v=vs.85).aspx)
- [**PostThreadMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644946(v=vs.85).aspx)
- [**WaitMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644956(v=vs.85).aspx)
- **Conceptual**
- [Messages and Message Queues](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms632590(v=vs.85).aspx)
[Send
 comments about this topic to Microsoft](mailto:wsddocfb@microsoft.com?subject=Documentation%20feedback%20%5Bwinmsg%5Cwinmsg%5D:%20GetMessage%20function%20%20RELEASE:%20(10/16/2012)&body=%0A%0APRIVACY%20STATEMENT%0A%0AThe%20doc%20team%20uses%20your%20feedback%20to%20improve%20the%20documentation.%20We%20don' rel=)
Build date: 10/16/2012
//=================================
备注::
1>看来很多WINDOWS API函数都别有洞天啊！不看不知道，一细看吓一跳，原来自己对WINDOWS下基本的API了解的也是这样的肤浅，
实在是惭愧啊！
2>对东西都把握不准，谈何把它用好用准呢？对WINDOWS下基本的API都只知道个模糊的概念，看来我的WINDOWS编程还没有入门啊！
3>
The possibility of a -1 return value means that such code can lead to fatal application errors. Instead, use code like this:

BOOL bRet;
while( (bRet = GetMessage( &msg, hWnd, 0, 0 )) != 0)
{ 
    if (bRet == -1)
    {
        // handle the error and possibly exit
    }
    else
    {
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }
}

