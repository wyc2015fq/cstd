# PeekMessage function (Windows) - xqhrs232的专栏 - CSDN博客
2012年11月29日 22:38:30[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：685
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644943(v=vs.85).aspx](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644943(v=vs.85).aspx)
16（共 33）对本文的评价是有帮助 - [评价此主题](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644943(v=vs.85).aspx#feedback)
Dispatches incoming sent messages, checks the thread message queue for a posted message, and retrieves the message (if any exist).
### Syntax
C++
BOOL WINAPI PeekMessage(
  _Out_     LPMSG lpMsg,
  _In_opt_  HWND hWnd,
  _In_      UINT wMsgFilterMin,
  _In_      UINT wMsgFilterMax,
  _In_      UINT wRemoveMsg
);
### Parameters
- *lpMsg* [out]
Type: **LPMSG**
A pointer to an [**MSG**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644958(v=vs.85).aspx) structure that
 receives message information.
- *hWnd* [in, optional]
Type: **HWND**
A handle to the window whose messages are to be retrieved. The window must belong to the current thread.
If *hWnd* is **NULL**, **PeekMessage** retrieves messages for any window that belongs to the current thread, and any messages on the current thread's message queue whose **hwnd** value is **NULL**(see
 the [**MSG**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644958(v=vs.85).aspx) structure). Therefore if hWnd
 is **NULL**, both window messages and thread messages are processed.
If *hWnd* is -1, **PeekMessage** retrieves only messages on the current thread's message queue whose **hwnd** value is **NULL**, that is, thread messages as posted by [**PostMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644944(v=vs.85).aspx) (when
 the *hWnd*parameter is **NULL**) or [**PostThreadMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644946(v=vs.85).aspx).
- *wMsgFilterMin* [in]
Type: **UINT**
The value of the first message in the range of messages to be examined. Use **WM_KEYFIRST**(0x0100) to specify the first keyboard message or **WM_MOUSEFIRST** (0x0200) to specify the first mouse message.
If *wMsgFilterMin* and *wMsgFilterMax* are both zero, **PeekMessage** returns all available messages (that is, no range filtering is performed).
- *wMsgFilterMax* [in]
Type: **UINT**
The value of the last message in the range of messages to be examined. Use **WM_KEYLAST** to specify the last keyboard message or **WM_MOUSELAST** to specify the last mouse message.
If *wMsgFilterMin* and *wMsgFilterMax* are both zero, **PeekMessage** returns all available messages (that is, no range filtering is performed).
- *wRemoveMsg* [in]
Type: **UINT**
Specifies how messages are to be handled. This parameter can be one or more of the following values.
|Value|Meaning|
|----|----|
|- **PM_NOREMOVE**- 0x0000|Messages are not removed from the queue after processing by**PeekMessage**.|
|- **PM_REMOVE**- 0x0001|Messages are removed from the queue after processing by **PeekMessage**.|
|- **PM_NOYIELD**- 0x0002|Prevents the system from releasing any thread that is waiting for the caller to go idle (see [**WaitForInputIdle**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms687022(v=vs.85).aspx)).Combine this value with either **PM_NOREMOVE** or **PM_REMOVE**.|
By default, all message types are processed. To specify that only certain message should be processed, specify one or more of the following values.
|Value|Meaning|
|----|----|
|- **PM_QS_INPUT**- (QS_INPUT << 16)|Process mouse and keyboard messages.|
|- **PM_QS_PAINT**- (QS_PAINT << 16)|Process paint messages.|
|- **PM_QS_POSTMESSAGE**- ((QS_POSTMESSAGE | QS_HOTKEY | QS_TIMER) << 16)|Process all posted messages, including timers and hotkeys.|
|- **PM_QS_SENDMESSAGE**- (QS_SENDMESSAGE << 16)|Process all sent messages.|
### Return value
Type:
**Type: **BOOL****
If a message is available, the return value is nonzero.
If no messages are available, the return value is zero.
### Remarks
**PeekMessage** retrieves messages associated with the window identified by the *hWnd* parameter or any of its children as specified by the [**IsChild**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms633524(v=vs.85).aspx) function,
 and within the range of message values given by the*wMsgFilterMin* and *wMsgFilterMax* parameters. Note that an application can only use the low word in the*wMsgFilterMin* and *wMsgFilterMax* parameters; the high word is reserved
 for the system.
Note that **PeekMessage** always retrieves [**WM_QUIT**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms632641(v=vs.85).aspx) messages,
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
The **PeekMessage** function normally does not remove [**WM_PAINT**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/dd145213(v=vs.85).aspx) messages
 from the queue. **WM_PAINT**messages remain in the queue until they are processed. However, if a **WM_PAINT** message has a **NULL**update region, **PeekMessage** does remove it from the queue.
If a top-level window stops responding to messages for more than several seconds, the system considers the window to be not responding and replaces it with a ghost window that has the same z-order, location, size, and visual attributes. This allows the user
 to move it, resize it, or even close the application. However, these are the only actions available because the application is actually not responding. When an application is being debugged, the system does not generate a ghost window.
### Examples
For an example, see [Examining a Message Queue](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644928(v=vs.85).aspx#examining_queue).
### Requirements
Unicode and ANSI names
**PeekMessageW** (Unicode) and **PeekMessageA** (ANSI)
### See also
- **Reference**
- [**GetMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644936(v=vs.85).aspx)
- [**IsChild**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms633524(v=vs.85).aspx)
- [**MSG**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644958(v=vs.85).aspx)
- [**WaitMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644956(v=vs.85).aspx)
- **Conceptual**
- [Messages and Message Queues](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms632590(v=vs.85).aspx)
- **Other Resources**
- [**WaitForInputIdle**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms687022(v=vs.85).aspx)
[Send
 comments about this topic to Microsoft](mailto:wsddocfb@microsoft.com?subject=Documentation%20feedback%20%5Bwinmsg%5Cwinmsg%5D:%20PeekMessage%20function%20%20RELEASE:%20(10/16/2012)&body=%0A%0APRIVACY%20STATEMENT%0A%0AThe%20doc%20team%20uses%20your%20feedback%20to%20improve%20the%20documentation.%20We%20don' rel=)
Build date: 10/16/2012
