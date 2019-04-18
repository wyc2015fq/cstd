# Receiving and Dispatching Messages - ljx0305的专栏 - CSDN博客
2010年06月30日 16:47:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：719标签：[application																[windows																[hibernate																[processing																[dialog																[function](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=dialog&t=blog)](https://so.csdn.net/so/search/s.do?q=processing&t=blog)](https://so.csdn.net/so/search/s.do?q=hibernate&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)
个人分类：[VC																[MFC](https://blog.csdn.net/ljx0305/article/category/403913)](https://blog.csdn.net/ljx0305/article/category/401790)
Receiving and Dispatching Messages
To receive messages, call the **GetMessage** function. Typically, an application calls **GetMessage** inside the window message loop.
The following code example shows a message loop that calls **GetMessage**.
Copy
while (GetMessage(&msg, NULL, 0, 0)) 
{
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}
When a thread calls **GetMessage**, Windows CE examines the thread message queue for incoming messages. Windows CE processes messages in the following order: 
- Windows CE checks for messages placed on the queue by the **SendMessage** function. After the system removes the message from the queue, it dispatches the message to the appropriate window procedure from within the **GetMessage** function. This guarantees that the sending and receiving message queues remain synchronized. The receiver must call **GetMessage** for the sent messages to be processed. 
- If no sent message is found, Windows CE checks for messages placed on the queue by a call to the **PostMessage** function. 
- If no posted message is found, Windows CE checks the queue for messages posted by the user-input system.
By processing user-input messages at a lower priority, the system guarantees that each input message and all posted messages that result from it are processed completely before moving on to the next input message. 
- If no posted input messages are found, Windows CE checks the queue for WM_QUIT messages placed on the queue by a call to the **PostQuitMessage** function. 
- If no posted quit messages are found, Windows CE checks the queue for WM_PAINT messages that were placed on the queue by the windowing system. 
- If no paint messages are found, Windows CE checks the queue for WM_TIMER messages placed on the queue by the timer system.
When **GetMessage** receives any of the previous messages, it returns the message content. The thread must call the **DispatchMessage** function to dispatch the message to the correct window procedure. If the message is a WM_QUIT message, the return value of **GetMessage** is zero, which causes the thread to end its message loop.
The system dispatches messages in the **GetMessage** call of the message loop, and the application dispatches messages by calling the **DispatchMessage** function in the message loop.
You might need to process messages that you receive from **GetMessage** before you send them out by using **DispatchMessage**. The most common processing functions are the **TranslateMessage**, **TranslateAccelerator**, and **IsDialogMessage** functions. Some of these functions can dispatch messages internally, because the application no longer needs to call **DispatchMessage** in the main message loop.
You usually call **TranslateMessage** before **DispatchMessage**. **TranslateMessage** determines which characters go with keyboard messages. **TranslateMessage** also posts the characters to the message queue to be picked up on the next pass of the message loop.
To intercept keyboard messages and generate menu commands, call the **TranslateAccelerator** function. Call the **IsDialogMessage** function to ensure the proper operation of modeless dialog boxes.
The following code example shows a message loop that uses **DispatchMessage**, **TranslateMessage**, **TranslateAccelerator**, and **IsDialogMessage** in addition to **GetMessage**. The call to **IsDialogMessage** in the code example uses the **HWND** for the modeless dialog box that is currently active. The window handle of the modeless dialog box that is currently active should be tracked in a global variable by your application if you are using multiple modeless dialog boxes in your application.
Copy
while (GetMessage(&msg, NULL, 0, 0)) 
{
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
    {
        if (hwndDLGCurrent != NULL || !IsDialogMessage(hwndDLGCurrent, 
                                                       &msg)) 
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
}
You can remove a message from its queue by using the **GetMessage** function. Call the **PeekMessage** function to examine a message without removing it from its queue. This function fills an **MSG** structure with information about the message. Use **PeekMessage** carefully, because it does not block the waiting for a message event, which enables an application to continue running regardless of messages in the queue. In a Windows CE–based application, if an application does not block the waiting for a message or some other event, the kernel cannot shift the CPU into low-power mode, which can drain the device batteries quickly.
When processing messages, Windows CE supports both system-defined messages and application-defined messages. System-defined messages have message identifiers ranging from 0 through 0x3ff. Messages with message identifiers ranging from 0x400 through 0x7fff are available for application-defined messages. Windows CE defines the message number 0x400 as WM_USER. If you want to define a custom message for your application, define the message as an offset from WM_USER. The following code example shows how to define a custom message.
Copy
#define WM_MYNEWMESSAGE       (WM_USER 999)
There are two types of system-defined messages: general window messages, which are used for all windows, and special-purpose messages, which apply to a particular class of windows. General window messages cover a wide range of information and requests, including messages for input device and keyboard input, as well as window creation and management.
The prefix of the symbolic constant for the message generally identifies the category to which the message belongs. For example, all general window messages start with WM, whereas messages that apply only to button controls start with BM.
The following table shows Windows CE message types.
|Message type|Description|
|----|----|
|BM|Button message|
|BN|Button notification|
|CB|Combo box message|
|CBN|Combo box notification|
|CDM|Common dialog box message|
|CDN|Common dialog box notification|
|CPL|Control panel message|
|DB|Object store message|
|DM|Dialog box default command button message|
|DTM|Date and time picker and Hypertext Markup Language (HTML) viewer messages|
|DTN|Date and time picker notification|
|EM|Edit control message|
|EN|Edit control notification|
|HDM|Header control message|
|HDN|Header control notification|
|IMN|Input context message|
|LB|List box control message|
|LBN|List box notification|
|LINE|Line device message|
|LVM|List view message|
|LVN|List view notification|
|MCM|Month calendar message|
|MCN|Month calendar notification|
|NM|Messages sent by a variety of controls|
|PBM|Progress bar message|
|PSM|Property sheet message|
|PSN|Property sheet notification|
|RB|Rebar message|
|RBN|Rebar notification|
|SB|Status bar window message|
|SBM|Scroll bar message|
|STM|Static bar message|
|STN|Static bar notification|
|TB|Toolbar message|
|TBM|Trackbar message|
|TBN|Trackbar notification|
|TCM|Tab control message|
|TCN|Tab control notification|
|TVM|Tree-view message|
|TVN|Tree-view notification|
|UDM|Up-down control message|
|UDN|Up-down control notification|
|WM|General window messages|
You can define messages for use by the window of your application. If you create messages, be sure that the window procedure that receives them interprets and processes them correctly. The OS does not interpret application-defined messages.
Occasionally, you need to use messages to communicate with windows that are controlled by other processes. In such a situation, call the **RegisterWindowMessage** function to register a message identifier. The message number that is returned is guaranteed to be unique throughout the system. By using this function, you prevent the conflicts that can arise if different applications use the same message identifier for different purposes.
Windows CE does not support hooking messages, because the extra processing that is required by hooks can impair the performance of Windows CE–based devices.
When handling messages in your application, be aware of the WM_HIBERNATE message. Windows CE defines a WM_HIBERNATE message to notify an application when system resources run low. When an application receives this message, it should attempt to release as many resources as possible. The system checks memory status at five-second intervals. Every Windows CE–based application that uses even moderate amounts of system resources should implement a handler for the WM_HIBERNATE message. If an application window is not visible, the application cannot receive a WM_HIBERNATE message. The application cannot receive the WM_HIBERNATE message because the WM_HIBERNATE message is sent only to applications that have a button on the taskbar, which is the case for visible windows only. A hidden window will not get this message, even if it is a top-level, overlapped window.
 Last updated on Tuesday, July 13, 2004
© 1992-2000 Microsoft Corporation. All rights reserved.
引用:http://msdn.microsoft.com/en-us/library/ms927938.aspx
