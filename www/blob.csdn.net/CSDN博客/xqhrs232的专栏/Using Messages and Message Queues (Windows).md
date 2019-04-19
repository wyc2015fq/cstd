# Using Messages and Message Queues (Windows) - xqhrs232的专栏 - CSDN博客
2012年11月29日 23:12:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：702
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644928(v=vs.85).aspx#examining_queue](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644928(v=vs.85).aspx#examining_queue)
31（共 130）对本文的评价是有帮助 - [评价此主题](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644928(v=vs.85).aspx#feedback)
The following code examples demonstrate how to perform the following tasks associated with Windows messages and message queues.
- [Creating a Message Loop](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644928(v=vs.85).aspx#creating_loop)
- [Examining a Message Queue](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644928(v=vs.85).aspx#examining_queue)
- [Posting a Message](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644928(v=vs.85).aspx#posting)
- [Sending a Message](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644928(v=vs.85).aspx#sending)
### Creating a Message Loop
The system does not automatically create a message queue for each thread. Instead, the system creates a message queue only for threads that perform operations which require a message queue. If the thread creates one or more windows, a message loop must be provided;
 this message loop retrieves messages from the thread's message queue and dispatches them to the appropriate window procedures.
Because the system directs messages to individual windows in an application, a thread must create at least one window before starting its message loop. Most applications contain a single thread that creates windows. A typical application registers the window
 class for its main window, creates and shows the main window, and then starts its message loop — all in the [**WinMain**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms633559(v=vs.85).aspx) function.
You create a message loop by using the [**GetMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644936(v=vs.85).aspx) and [**DispatchMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644934(v=vs.85).aspx) functions.
 If your application must obtain character input from the user, include the [**TranslateMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644955(v=vs.85).aspx) function
 in the loop.**TranslateMessage** translates virtual-key messages into character messages. The following example shows the message loop in the [**WinMain**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms633559(v=vs.85).aspx) function
 of a simple Windows-based application.

HINSTANCE hinst; 
HWND hwndMain; 
 
int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
    LPSTR lpszCmdLine, int nCmdShow) 
{ 
    MSG msg;
    BOOL bRet; 
    WNDCLASS wc; 
    UNREFERENCED_PARAMETER(lpszCmdLine); 
 
    // Register the window class for the main window. 
 
    if (!hPrevInstance) 
    { 
        wc.style = 0; 
        wc.lpfnWndProc = (WNDPROC) WndProc; 
        wc.cbClsExtra = 0; 
        wc.cbWndExtra = 0; 
        wc.hInstance = hInstance; 
        wc.hIcon = LoadIcon((HINSTANCE) NULL, 
            IDI_APPLICATION); 
        wc.hCursor = LoadCursor((HINSTANCE) NULL, 
            IDC_ARROW); 
        wc.hbrBackground = GetStockObject(WHITE_BRUSH); 
        wc.lpszMenuName =  "MainMenu"; 
        wc.lpszClassName = "MainWndClass"; 
 
        if (!RegisterClass(&wc)) 
            return FALSE; 
    } 
 
    hinst = hInstance;  // save instance handle 
 
    // Create the main window. 
 
    hwndMain = CreateWindow("MainWndClass", "Sample", 
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
        CW_USEDEFAULT, CW_USEDEFAULT, (HWND) NULL, 
        (HMENU) NULL, hinst, (LPVOID) NULL); 
 
    // If the main window cannot be created, terminate 
    // the application. 
 
    if (!hwndMain) 
        return FALSE; 
 
    // Show the window and paint its contents. 
 
    ShowWindow(hwndMain, nCmdShow); 
    UpdateWindow(hwndMain); 
 
    // Start the message loop. 
 
    while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
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
 
    // Return the exit code to the system. 
 
    return msg.wParam; 
} 
The following example shows a message loop for a thread that uses accelerators and displays a modeless dialog box. When [**TranslateAccelerator**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms646373(v=vs.85).aspx) or [**IsDialogMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms645498(v=vs.85).aspx) returns **TRUE** (indicating
 that the message has been processed), [**TranslateMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644955(v=vs.85).aspx) and [**DispatchMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644934(v=vs.85).aspx) are
 not called. The reason for this is that **TranslateAccelerator** and **IsDialogMessage** perform all necessary translating and dispatching of messages.

HWND hwndMain; 
HWND hwndDlgModeless = NULL; 
MSG msg;
BOOL bRet; 
HACCEL haccel; 
// 
// Perform initialization and create a main window. 
// 
 
while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
{ 
    if (bRet == -1)
    {
        // handle the error and possibly exit
    }
    else
    {
        if (hwndDlgModeless == (HWND) NULL || 
                !IsDialogMessage(hwndDlgModeless, &msg) && 
                !TranslateAccelerator(hwndMain, haccel, 
                    &msg)) 
        { 
            TranslateMessage(&msg); 
            DispatchMessage(&msg); 
        }
    } 
} 
### Examining a Message Queue
Occasionally, an application needs to examine the contents of a thread's message queue from outside the thread's message loop. For example, if an application's window procedure performs a lengthy drawing operation, you may want the user to be able to interrupt
 the operation. Unless your application periodically examines the message queue during the operation for mouse and keyboard messages, it will not respond to user input until after the operation has completed. The reason for this is that the[**DispatchMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644934(v=vs.85).aspx) function
 in the thread's message loop does not return until the window procedure finishes processing a message.
You can use the [**PeekMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644943(v=vs.85).aspx) function
 to examine a message queue during a lengthy operation.**PeekMessage** is similar to the [**GetMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644936(v=vs.85).aspx) function;
 both check a message queue for a message that matches the filter criteria and then copy the message to an [**MSG**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644958(v=vs.85).aspx) structure.
 The main difference between the two functions is that **GetMessage** does not return until a message matching the filter criteria is placed in the queue, whereas **PeekMessage** returns immediately regardless of whether a message
 is in the queue.
The following example shows how to use [**PeekMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644943(v=vs.85).aspx) to
 examine a message queue for mouse clicks and keyboard input during a lengthy operation.

HWND hwnd; 
BOOL fDone; 
MSG msg; 
 
// Begin the operation and continue until it is complete 
// or until the user clicks the mouse or presses a key. 
 
fDone = FALSE; 
while (!fDone) 
{ 
    fDone = DoLengthyOperation(); // application-defined function 
 
    // Remove any messages that may be in the queue. If the 
    // queue contains any mouse or keyboard 
    // messages, end the operation. 
 
    while (PeekMessage(&msg, hwnd,  0, 0, PM_REMOVE)) 
    { 
        switch(msg.message) 
        { 
            case WM_LBUTTONDOWN: 
            case WM_RBUTTONDOWN: 
            case WM_KEYDOWN: 
                // 
                // Perform any required cleanup. 
                // 
                fDone = TRUE; 
        } 
    } 
} 
Other functions, including [**GetQueueStatus**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644940(v=vs.85).aspx) and [**GetInputState**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644935(v=vs.85).aspx),
 also allow you to examine the contents of a thread's message queue. **GetQueueStatus** returns an array of flags that indicates the types of messages in the queue; using it is the fastest way to discover whether the queue contains any messages. **GetInputState** returns **TRUE** if
 the queue contains mouse or keyboard messages. Both of these functions can be used to determine whether the queue contains messages that need to be processed.
### Posting a Message
You can post a message to a message queue by using the [**PostMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644944(v=vs.85).aspx) function. **PostMessage** places
 a message at the end of a thread's message queue and returns immediately, without waiting for the thread to process the message. The function's parameters include a window handle, a message identifier, and two message parameters. The system copies these parameters
 to an [**MSG**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644958(v=vs.85).aspx) structure, fills the **time** and **pt** members
 of the structure, and places the structure in the message queue.
The system uses the window handle passed with the [**PostMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644944(v=vs.85).aspx) function
 to determine which thread message queue should receive the message. If the handle is **HWND_TOPMOST**, the system posts the message to the thread message queues of all top-level windows.
You can use the [**PostThreadMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644946(v=vs.85).aspx) function
 to post a message to a specific thread message queue.**PostThreadMessage** is similar to [**PostMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644944(v=vs.85).aspx),
 except the first parameter is a thread identifier rather than a window handle. You can retrieve the thread identifier by calling the [**GetCurrentThreadId**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms683183(v=vs.85).aspx) function.
Use the [**PostQuitMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644945(v=vs.85).aspx) function to
 exit a message loop. **PostQuitMessage** posts the [**WM_QUIT**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms632641(v=vs.85).aspx)message
 to the currently executing thread. The thread's message loop terminates and returns control to the system when it encounters the **WM_QUIT** message. An application usually calls **PostQuitMessage**in response to the [**WM_DESTROY**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms632620(v=vs.85).aspx) message,
 as shown in the following example.

case WM_DESTROY: 
 
    // Perform cleanup tasks. 
 
    PostQuitMessage(0); 
    break; 
### Sending a Message
The [**SendMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644950(v=vs.85).aspx) function is used to
 send a message directly to a window procedure. **SendMessage**calls a window procedure and waits for that procedure to process the message and return a result.
A message can be sent to any window in the system; all that is required is a window handle. The system uses the handle to determine which window procedure should receive the message.
Before processing a message that may have been sent from another thread, a window procedure should first call the [**InSendMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644941(v=vs.85).aspx) function.
 If this function returns **TRUE**, the window procedure should call[**ReplyMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms644948(v=vs.85).aspx) before
 any function that causes the thread to yield control, as shown in the following example.

case WM_USER + 5: 
    if (InSendMessage()) 
        ReplyMessage(TRUE); 
 
    DialogBox(hInst, "MyDialogBox", hwndMain, (DLGPROC) MyDlgProc); 
    break; 
A number of messages can be sent to controls in a dialog box. These control messages set the appearance, behavior, and content of controls or retrieve information about controls. For example, the[**CB_ADDSTRING**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/bb775828(v=vs.85).aspx) message
 can add a string to a combo box, and the [**BM_SETCHECK**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/bb775989(v=vs.85).aspx) message
 can set the check state of a check box or radio button.
Use the [**SendDlgItemMessage**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/ms645515(v=vs.85).aspx) function
 to send a message to a control, specifying the identifier of the control and the handle of the dialog box window that contains the control. The following example, taken from a dialog box procedure, copies a string from a combo box's edit control into its list
 box. The example uses **SendDlgItemMessage** to send a [**CB_ADDSTRING**](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/bb775828(v=vs.85).aspx) message
 to the combo box.

HWND hwndCombo; 
int cTxtLen; 
PSTR pszMem; 
 
switch (uMsg) 
{ 
    case WM_COMMAND: 
        switch (LOWORD(wParam)) 
        { 
            case IDD_ADDCBITEM: 
                // Get the handle of the combo box and the 
                // length of the string in the edit control 
                // of the combo box. 
 
                hwndCombo = GetDlgItem(hwndDlg, IDD_COMBO); 
                cTxtLen = GetWindowTextLength(hwndCombo); 
 
                // Allocate memory for the string and copy 
                // the string into the memory. 
 
                pszMem = (PSTR) VirtualAlloc((LPVOID) NULL, 
                    (DWORD) (cTxtLen + 1), MEM_COMMIT, 
                    PAGE_READWRITE); 
                GetWindowText(hwndCombo, pszMem, 
                    cTxtLen + 1); 
 
                // Add the string to the list box of the 
                // combo box and remove the string from the 
                // edit control of the combo box. 
 
                if (pszMem != NULL) 
                { 
                    SendDlgItemMessage(hwndDlg, IDD_COMBO, 
                        CB_ADDSTRING, 0, 
                        (DWORD) ((LPSTR) pszMem)); 
                    SetWindowText(hwndCombo, (LPSTR) NULL); 
                } 
 
                // Free the memory and return. 
 
                VirtualFree(pszMem, 0, MEM_RELEASE); 
                return TRUE; 
            // 
            // Process other dialog box commands. 
            // 
 
        } 
    // 
    // Process other dialog box messages. 
    // 
 
}

[Send
 comments about this topic to Microsoft](mailto:wsddocfb@microsoft.com?subject=Documentation%20feedback%20%5Bwinmsg%5Cwinmsg%5D:%20Using%20Messages%20and%20Message%20Queues%20%20RELEASE:%20(10/16/2012)&body=%0A%0APRIVACY%20STATEMENT%0A%0AThe%20doc%20team%20uses%20your%20feedback%20to%20improve%20the%20documentation.%20We%20don' rel=)
Build date: 10/16/2012
## 社区附加资源
[添加](http://msdn.microsoft.com/ZH-CN/library/windows/desktop/community/add/ms644928(v=vs.85).aspx)
### The "Correct" Message Loop
If the correct while condition is:
while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
Why does Visual Studio generate a default file like this for win32 apps:
while (GetMessage(&msg, NULL, 0, 0))
?
![](http://i1.social.s-msft.com/profile/u/avatar.jpg?displayname=timclancy&size=small)
[timclancy](http://social.msdn.microsoft.com/profile/timclancy/)
12/20/2011
### Hey
PeekMessage shouldn't be needed in modern, well-written applications.
![](http://i1.social.s-msft.com/profile/u/avatar.jpg?displayname=hayhay1982&size=small)
[Hayhay1982](http://social.msdn.microsoft.com/profile/hayhay1982/)
1/20/2011
### Why to use PeekMessage()
Actually, PeekMessage() is quite necessary in modern programs such as games which require the Direct3D device creation/rendering thread to be the same one the message loop runs on.  However, in a GUI based program, you could possibly have it in another thread. 
 Also, anything that uses threading is more complicated because any messages that the app depends on must be synchronized between the threads.  It is simpler to just use 1 thread for anything that deals with the window or its messages and use threads to do
 worker tasks that can be parallelized easily.
So you use it like this:
if(PeekMessage(...))
{
    ...
}
else
{
    RenderGame(); //Example
}
![](http://i1.social.s-msft.com/profile/u/avatar.jpg?displayname=caleb+everett&size=small)
[Caleb Everett](http://social.msdn.microsoft.com/profile/caleb%20everett/)
1/18/2011
### hPrevInstance
In the example code, checking hPrevInstance and conditionally registering the class is outdated. Win32 apps should not use the hPrevInstance member as it is always NULL.
![](http://i1.social.s-msft.com/profile/u/avatar.jpg?displayname=thomas+lee&size=small)
[Thomas Lee](http://social.msdn.microsoft.com/profile/thomas%20lee/)
9/26/2010
//===============================
备注::
### The "Correct" Message Loop
If the correct while condition is:
while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
Why does Visual Studio generate a default file like this for win32 apps:
while (GetMessage(&msg, NULL, 0, 0))
?
