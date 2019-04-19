# 【重学C/C++】DispatchMessage||TranslateMessage||PostThreadMessage||PeekMessage - 三少GG - CSDN博客
2011年10月13日 22:44:13[三少GG](https://me.csdn.net/scut1135)阅读数：4052
[Windows API一日一练(14) DispatchMessage函数](http://blog.csdn.net/caimouse/article/details/1704627) （牛人的网站！）
前面已经介绍从系统队列里获取一条消息，然后经过快捷键的函数检查，又通过字符消息函数的转换，最后要做的事情就是调用DispatchMessage函数，它的意思就是说要把这条消息发送到窗口里的消息处理函数WindowProc。
函数DispatchMessage声明如下：
WINUSERAPI
LRESULT
WINAPI
DispatchMessageA(
    __in CONST MSG *lpMsg);
WINUSERAPI
LRESULT
WINAPI
DispatchMessageW(
    __in CONST MSG *lpMsg);
#ifdef UNICODE
#define DispatchMessage DispatchMessageW
#else
#define DispatchMessage DispatchMessageA
#endif // !UNICODE
lpMsg是指向想向消息处理函数WindowProc发送的消息。
调用这个函数的例子如下：
#001 //主程序入口
#002 //
#003 // 蔡军生 2007/07/19
#004 // QQ: 9073204
#005 //
#006 int APIENTRY _tWinMain(HINSTANCE hInstance,
#007                       HINSTANCE hPrevInstance,
#008                       LPTSTR    lpCmdLine,
#009                       int       nCmdShow)
#010 {
#011  UNREFERENCED_PARAMETER(hPrevInstance);
#012  UNREFERENCED_PARAMETER(lpCmdLine);
#013 
#014   //
#015  MSG msg;
#016  HACCEL hAccelTable;
#017 
#018  // 加载全局字符串。
#019  LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
#020  LoadString(hInstance, IDC_TESTWIN, szWindowClass, MAX_LOADSTRING);
#021  MyRegisterClass(hInstance);
#022 
#023  // 应用程序初始化:
#024  if (!InitInstance (hInstance, nCmdShow))
#025  {
#026         return FALSE;
#027  }
#028 
#029  hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TESTWIN));
#030 
#031  // 消息循环:
#032  BOOL bRet;
#033  while ( (bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
#034  {
#035         if (bRet == -1)
#036         {
#037               //处理出错。
#038 
#039         }
#040         else if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
#041         {
#042               TranslateMessage(&msg);
#043               **DispatchMessage(&msg);**
#044         }
#045  }
#046 
#047  return (int) msg.wParam;
#048 }
#049 
第43行就是调用函数DispatchMessage发送消息。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
DispatchMessage
windows消息处理机制是这样的: 首先系统(也就是windows)把来自硬件(鼠标,键盘等消息)和来自应用程序的消息 放到一个系统消息队列中去. 
而应用程序需要有自己的消息队列,也就是线程消息队列。每一个线程有自己的消息队列,对于多线程的应用程序就有和线程数目相等的线程消息队列. 
winsows消息队列把得到的消息发送到线程消息队列, 线程消息队列**每次取出一条消息发送到指定窗口,不断循环直到程序退出.这个循环就是靠消息环(while(GetMessage()) TranslateMessage();DispatchMessage(); 实现的。**
**GetMessage()只是从线程消息中取出一条消息,而DispatchMessage 则把取出的消息发送到目的窗口。**
**如果收到WM_CLOSE消息则结束循环,发送postqiutmessage(0),处理WM_DESTROY销毁窗口!  **
其实问题的关键在于DispatchMessage到底干了什么 
如果只是去调用相应的窗口，那自己写个switch不就可以了 
DispatchMessage与switch不同之处在于DispatchMessage会 
先调用windows，进入管态（大概是range 0），然后再由windows调用 
窗口的函数。 
为什么这么麻烦？ 
因为这样windows就可以知道你的程序运行到什么情况了， 
windows来调用你的窗口，这样你的窗口返回的时候windows就知道 
你已经处理过一个消息了，如果没有新的消息进入消息队列 
windows就不再会给你的进程分配时间片 
如果是你自己写switch的话，windows就不可能这样灵活的分配时间 
资源利用率就会降低 
那么还要消息循环干什么，windows直接把消息发给窗口不就可以了吗？ 
因为你要在消息循环里把KEY_DOWN和KEY_UP组合成WM_CHAR， 
还可以直接屏蔽掉许多对你来说无用的消息，加快速度  
GetMessage:从线程的消息队列取出一个消息   
    TranslateMessage:     将msg结构传给Windows,进行一些转换,比如A键按下,转换成WM_CHAR消息等   
    DispatchMessage():再将msg结构传给Windows,Windows将该消息发给窗口过程,由窗口过程处理.
TranslateMessage是对一些键盘事件做预处理。
GetMessage是从系统为每个应用程序自动分配的消息对列的头部得到一个消息。   
    TranslateMessage是翻译需要翻译的消息   
DispatchMessage()则会把翻译好的消息发送到系统的消息处理函数中，而这个函数又会把这个消息传递到注册窗体时用户指定的消息处理函数中。
翻译消息不是简单的转换，一个消息被翻译后，可能会产生几个消息。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
TranslateMessage函数将虚拟键消息转换为字符消息。字符消息被寄送到调用线程的消息队列里，当下一次线程调用函数GetMessage或PeekMessage时被读出。
TranslateMessage是用来把虚拟键消息转换为字符消息。由于Windows对所有键盘编码都是采用虚拟键的定义，这样当按键按下时，并不得字符消息，需要键盘映射转换为字符的消息。
TranslateMessage函数用于将虚拟键消息转换为字符消息。字符消息被投递到调用线程的消息队列中，当下一次调用GetMessage函数时被取出。当我们敲击键盘上的某个字符键时，系统将产生WM_KEYDOWN和WM_KEYUP消息。这两个消息的附加参数（wParam和lParam）包含的是虚拟键代码和扫描码等信息，而我们在程序中往往需要得到某个字符的ASCII码，TranslateMessage这个函数就可以将WM_KEYDOWN和WM_ KEYUP消息的组合转换为一条WM_CHAR消息（该消息的wParam附加参数包含了字符的ASCII码），并将转换后的新消息投递到调用线程的消息队列中。注意，TranslateMessage函数并不会修改原有的消息，它只是产生新的消息并投递到消息队列中。
也就是说TranslateMessage会发现消息里是否有字符键的消息，如果有字符键的消息，就会产生WM_CHAR消息，如果没有就会产生什么消息。
### [消息循环中的TranslateMessage函数和DispatchMessage函数](http://blog.csdn.net/xxxluozhen/article/details/4123549)
**TranslateMessage函数**
函数功能描述:将虚拟键消息转换为字符消息。字符消息被送到调用线程的消息队列中，在下一次线程调用函数GetMessage或PeekMessage时被读出。
.函数原型：
    BOOL TranslateMessage(  CONST MSG *lpMsg );
.参数：
    lpMsg 
        指向一个含有用GetMessage或PeekMessage函数从调用线程的消息队列中取得消息信息的MSG结构的指针。
.返回值：
    如果消息被转换（即，字符消息被送到线程的消息队列中），返回非零值。
    如果消息是 WM_KEYDOWN, WM_KEYUP, WM_SYSKEYDOWN, 或 WM_SYSKEYUP，返回非零值，不考虑转换。
    如果消息没有转换（即，字符消息没被送到线程的消息队列中），返回值是零。
.备注：
    TranslateMessage函数不修改由参数lpMsg指向的消息。
    消息WM_KEYDOWN和WM_KEYUP组合产生一个WM_CHAR或WM_DEADCHAR消息。消息WM_SYSKEYDOWN和WM_SYSKEYUP组合产生一个WM_SYSCHAR或 WM_SYSDEADCHAR 消息。
    TtanslateMessage仅为那些由键盘驱动器映射为ASCII字符的键产生WM_CHAR消息。
    如果应用程序为其它用途而处理虚拟键消息，不应调用TranslateMessage函数。例如，如果TranslateAccelerator函数返回一个非零值，则应用程序将不调用TranslateMessage函数。
    Windows CE：Windows CE不支持扫描码或扩展键标志，因此，它不支持由TranslateMessage函数产生的WM_CHAR消息中的lKeyData参数（lParam）16-24的取值。
    TranslateMessage函数只能用于转换由GetMessage或PeekMessage函数接收到的消息。
**DispatchMessage函数**
函数功能：该函数调度一个消息给窗口程序。通常调度从GetMessage取得的消息。消息被调度到的窗口程序即是MainProc()函数。
　　函数原型：LONG DispatchMessage（CONST MSG*lpmsg）；
　　参数：
　　lpmsg：指向含有消息的MSG结构的指针。
　　返回值：返回值是窗口程序返回的值。尽管返回值的含义依赖于被调度的消息，但返回值通常被忽略。
　　备注：MSG结构必须包含有效的消息值。如果参数lpmsg指向一个WM_TIMER消息，并且WM_TIMER消息的参数IParam不为NULL，则调用IPa1ram指向的函数，而不是调用窗口程序。
　　速查：Windows NT：3.1及以上版本；Windows：95及以上版本；Windows CE：1.0及以上版本；头文件：winuser.h；输入库：user32.lib；Unicode：在Windows NT环境下以Unicode和ANSI方式实现。
总结:TranslateMessage函数将键盘消息转化,DispatchMessage函数将消息传给窗体函数去处理.
++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# [PostThreadMessage](http://my.oschina.net/wildcard/blog/2416?from=rss)
函数功能：该函数将一个消息放入 （寄送）到指定线程的消息队列里，不等待线程处理消息就返回。
函数原型：BOOL PostThreadMessage（DWORD idThread，UINT Msg，WPARAM wParam，LPARAM IParam）；
参数
idThread：其消息将被寄送的线程的 线程标识符。如果线程没有消息队列，此函数将失败。当线程第一次调用一个Win 32 USER或GDI函数时，系统创建线程的消息队列。要得到更多的信息，参见备注。
Msg：指定将被寄送的消息的类型。
wParam：指定附加的消息特定信息。
IParam：指定附加的消息特定信息。
返回值：如果函数调用成功，返回非零值。如 果函数调用失败，返回值是零。若想获得更多的错误信息，请调用GetLastError函数。如果idThread不是一个有效的线程标识符或由 idThread确定的线程没有消息队列，GetLastError返回 ERROR_INVALID_THREAD。
备注：消息将寄送到的线程必须创建消息队 列，否则调用PostThreadMessage会失败。用下列方法之一来处理这种情况：
调用PostThreadMessage。 如果失败，调用Sleep，再调用PostThreadMessage，反复执行，直到PostThreadMessage成功。
创建一个事件对象，再创建线程。在调用 PostThreadMessage之前，用函数WaitForSingleObject来等特事件被设置为被告知状态。消息将寄送到的线程调用 PeekMessage（￡msg，NULL，WM_USER，WM_USER，PM_NOREMOVE）来强制系统创建消息队列。设置事件，表示线程已 准备好接收寄送的消息。
消息将被寄送到的线程通过调用 GetMesssge或PeekMesssge来取得消息。返回的MSG结构中的hwnd成员为NULL。
速查：Windows NT：3.1及以上版本；Windows：95及以上版本；Windows CE：1.0及以上版本：头文件：winuser.h；输入库：user32.lib；Unicode：在Windows NT环境下以Unicode和ANSI方式实现。
---------------------------------------------------------------------------------------
把window线程间传送消息仔细的看了一遍，觉得以前的理解 很不深刻。说一说对PostThreadMessage的 理解。
PostThreadMessage是一个线程体发送一个消息 到指定的线程ID，其 原型如下：
BOOL PostThreadMessage(                                 DWORD idThread,                       UINT Msg,                          WPARAM wParam,                           LPARAM lParam);
**  这个函数既可以发送消息给工 作线程，也可以发送给UI线 程。接受PostThreadMessage的 线程必须已经有了一个message queue， 否则调用PostThreadMessage会 失败。**因为此原因使用GetLastError会 得到错误码为1444，
 这种情况经常出现，解决方法有如下两种：
1.         调 用PostThreadMessage， 如果失败，就Sleep一 段时间再次调用PostThreadMessage直 到调用成功；
2.         创 建一个Event对 象，让PostThreadMessage等 待接受的线程创建一个message queue。 可以通过调用PeekMessage强 制系统创建一个message queue。 示例代码如下：
假设mainAPP是发送线程ThreadA是接受线程
/*mainAPP.cpp*/……hStartEvent = ::CreateEvent(0,FALSE,FALSE,0); //create thread start eventif(hStartEvent == 0){             printf("create start event failed,errno:%d\n",::GetLastError());             return 1;}::WaitForSingleObject(hStartEvent,INFINITE);CloseHandle(hStartEvent);if(!PostThreadMessage(threadaID, WM_MESSAGE_A,0,0)){             _tprintf(_T("post error! %d\n"), GetLastError());             return 1;}……
ThreadA是接受线程
/* ThreadA */MSG msg;PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);if(!SetEvent(hStartEvent)){             printf("set event error,%d\n",GetLastError());             return 1;}while(true){             if(GetMessage(&msg, 0,0,0)) {                       switch(msg.message){                                       case WM_MESSAGE_A:                                  ……                                  break;                                    }                       }             }}
**PostThreadMessage传递的消息如果要包含信 息，要注意在结束的时候释放消息中的信息。（2.如果是post动态分配的memory给另外一个thread,要注意内存的正确释放。）**
在消息中附加信息方法如下
/*构造信息如下*/char* pInfo = new char[MAX_INFO_SIZE]; //create dynamic msgsprintf(pInfo,"msg_%d",++count);PostThreadMessage(nThreadID,MY_MSG,(WPARAM)pInfo,0)//post thread msg/*解释信息如下*/if(GetMessage(&msg,0,0,0)) //get msg from message queue{               switch(msg.message)               {               case MY_MSG:               char * pInfo = (char *)msg.wParam;               printf("recv %s\n",pInfo);            delete[] pInfo; //这里释放了资源               break;               }}
做了一个简单的消息通信实验，让主 线程中等待用户输入，产生不同的消息，并把这些消息post给 子线程，子线程根据产生的消息做出不同的反映。这些子线程可以是工作线程也可以是UI线程。
#include <windows.h>
#include <cstdio>
#include <process.h>
#define MY_MSG WM_USER+100
const int MAX_INFO_SIZE = 20;
HANDLE hStartEvent; // thread start event
// thread function
unsigned __stdcall fun(void *param)
{
     printf("thread fun start![](http://www.cppblog.com/Images/dot.gif)\n");
     MSG msg;
     PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
    if(!SetEvent(hStartEvent)) //set thread start event 
     {
         printf("set start event failed,errno:%d\n",::GetLastError());
        return 1;
     }
    while(true)
     {
        if(GetMessage(&msg,0,0,0)) //get msg from message queue
         {
            switch(msg.message)
             {
            case MY_MSG:
                char * pInfo = (char *)msg.wParam;
                 printf("recv %s\n",pInfo);
                 delete[] pInfo;
                break;
             }
         }
     };
    return 0;
}
int main()
{
     HANDLE hThread;
     unsigned nThreadID;
     hStartEvent = ::CreateEvent(0,FALSE,FALSE,0); //create thread start event
    if(hStartEvent == 0)
     {
         printf("create start event failed,errno:%d\n",::GetLastError());
        return 1;
     }
    //start thread
     hThread = (HANDLE)_beginthreadex( NULL, 0, &fun, NULL, 0, &nThreadID );
    if(hThread == 0)
     {
         printf("start thread failed,errno:%d\n",::GetLastError());
         CloseHandle(hStartEvent);
        return 1;
     }
    //wait thread start event to avoid PostThreadMessage return errno:1444
     ::WaitForSingleObject(hStartEvent,INFINITE);
     CloseHandle(hStartEvent);
    int count = 0;
    while(true)
     {
        char* pInfo = new char[MAX_INFO_SIZE]; //create dynamic msg
         sprintf(pInfo,"msg_%d",++count);
        if(!PostThreadMessage(nThreadID,MY_MSG,(WPARAM)pInfo,0))//post thread msg
         {
             printf("post message failed,errno:%d\n",::GetLastError());
             delete[] pInfo;
         }
         ::Sleep(1000);
     }
     CloseHandle(hThread);
    return 0;
}
要把SETTING 改为多线程的 
Project->Settings->click C/C tab, 在Category 中选Code Generation, 然后在Use run-time libray 中选一个 Multithread 配置
++++++++++++++++++++++++++++++++++++++++++///////////////////////////+++++++++++++++++++++++++++++++++++++
**PostThreadMessage**的原型是这样的
BOOL PostThreadMessage( DWORD idThread,
    UINT Msg,
    WPARAM wParam,
    LPARAM lParam
);
**PostThreadMessage可以用于线程之间的异步通讯，因为它不用等待调用者返回，这也许是线程通讯中最简单的一种方法了。**
但是要注意以下问题
1 .PostThreadMessage有时会失败，报1444错误(Invalid thread identifier. )其实这不一定是线程不存在的原因，也有可能是线程不存在消息队列(message queue)造成的。事实上，并不是每个thread都有message queue,那如何让thread具有呢？答案是，至少调用message相关的function一次，比如GetMessage,PeekMessage。
2.如果是post动态分配的memory给另外一个thread,要注意内存的正确释放。
3.PostThreadMessage不能够post WM_COPYDATE之类的同步消息，否则会报错
4.最好不要使用PostThreadMessage post message给一个窗口,使用PostMessage替代。
下面是我写的一个比较严整的例子，仅供参考。
#include <windows.h>
#include <cstdio>
#include <process.h>
#define MY_MSG WM_USER+100
constint MAX_INFO_SIZE =20;
HANDLE hStartEvent; // thread start event
// thread function
unsigned __stdcall fun(void*param)
{
    printf("thread fun start![](http://www.cppblog.com/Images/dot.gif)\n");
    MSG msg;
    PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
if(!SetEvent(hStartEvent)) //set thread start event 
    {
        printf("set start event failed,errno:%d\n",::GetLastError());
return1;
    }
while(true)
    {
if(GetMessage(&msg,0,0,0)) //get msg from message queue
        {
switch(msg.message)
            {
case MY_MSG:
char* pInfo = (char*)msg.wParam;
                printf("recv %s\n",pInfo);
                delete[] pInfo;
break;
            }
        }
    };
return0;
}
int main()
{
    HANDLE hThread;
    unsigned nThreadID;
    hStartEvent = ::CreateEvent(0,FALSE,FALSE,0); //create thread start event
if(hStartEvent ==0)
    {
        printf("create start event failed,errno:%d\n",::GetLastError());
return1;
    }
//start thread
    hThread = (HANDLE)_beginthreadex( NULL, 0, &fun, NULL, 0, &nThreadID );
if(hThread ==0)
    {
        printf("start thread failed,errno:%d\n",::GetLastError());
        CloseHandle(hStartEvent);
return1;
    }
//wait thread start event to avoid PostThreadMessage return errno:1444
    ::WaitForSingleObject(hStartEvent,INFINITE);
    CloseHandle(hStartEvent);
int count =0;
while(true)
    {
char* pInfo =newchar[MAX_INFO_SIZE]; //create dynamic msg
        sprintf(pInfo,"msg_%d",++count);
if(!PostThreadMessage(nThreadID,MY_MSG,(WPARAM)pInfo,0))//post thread msg
        {
            printf("post message failed,errno:%d\n",::GetLastError());
            delete[] pInfo;
        }
        ::Sleep(1000);
    }
    CloseHandle(hThread);
return0;
}
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# [PeekMessage](http://msdn.microsoft.com/zh-cn/library/ms911928)
**Windows CE 5.0**
This function checks a thread message queue for a message and places the message (if any) in the specified structure.

BOOL PeekMessage(
  LPMSG lpMsg, 
  HWND hWnd, 
  UINT wMsgFilterMin, 
  UINT wMsgFilterMax, 
  UINT wRemoveMsg 
); 
#### Parameters
- *lpMsg*
[in] Pointer to an [MSG](http://msdn.microsoft.com/zh-cn/library/ms931459) structure that receives message information.- *hWnd*
[in] Handle to the window whose messages are to be examined.- *wMsgFilterMin*
[in] Specifies the value of the first message in the range of messages to be examined.- *wMsgFilterMax*
[in] Specifies the value of the last message in the range of messages to be examined.- *wRemoveMsg*
[in] Specifies how messages are handled. This parameter can be one of the following values.
|Value|Description|
|----|----|
|PM_NOREMOVE|Messages are not removed from the queue after processing by **PeekMessage**.|
|PM_REMOVE|Messages are removed from the queue after processing by **PeekMessage**. An exception is WM_PAINT messages, which are not removed from the queue.|
You can optionally combine the value PM_NOYIELD with either PM_NOREMOVE or PM_REMOVE. This flag prevents the system from releasing any thread that is waiting for the caller to go idle.
#### Return Values
Nonzero indicates success. Zero indicates failure.
#### Remarks
Unlike the [GetMessage](http://msdn.microsoft.com/zh-cn/library/aa453135) function, the **PeekMessage** function does not wait for a message to be placed in the queue before returning.
**PeekMessage** retrieves only messages associated with the window identified by the *hWnd* parameter or any of its children as specified by the [IsChild](http://msdn.microsoft.com/zh-cn/library/ms909862) function,
 and within the range of message values given by the *wMsgFilterMin* and *wMsgFilterMax* parameters. If *hWnd* is NULL, **PeekMessage** retrieves messages for any window that belongs to the current thread making the call. (**PeekMessage** does
 not retrieve messages for windows that belong to other threads.) If *hWnd* is –1, **PeekMessage** only returns messages with a *hWnd* value of NULL, as posted by the[PostThreadMessage](http://msdn.microsoft.com/zh-cn/library/ms911939) function.
 If *wMsgFilterMin* and *wMsgFilterMax* are both zero, **PeekMessage** returns all available messages (that is, no range filtering is performed).
The WM_KEYFIRST and WM_KEYLAST constants can be used as filter values to retrieve all keyboard messages; the WM_MOUSEFIRST and WM_MOUSELAST constants can be used to retrieve all mouse messages.
The** PeekMessage** function does not remove WM_PAINT messages from the queue. WM_PAINT messages remain in the queue until they are processed.
#### Requirements
**OS Versions:** Windows CE 1.0 and later.
**Header:** Winuser.h.
**Link Library:** Coredll.lib.
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
# [PeekMessage&GetMessage](http://www.cnblogs.com/faceang/archive/2010/05/25/1743757.html)
# PeekMessage与GetMessage的对比
# 相同点：
PeekMessage函数与GetMessage函数都用于查看应用程序消息队列，有消息时将队列中的消息派发出去。
不同点：
无论应用程序消息队列是否有消息，PeekMessage函数都立即返回，程序得以继续执行后面的语句（无消息则执行其它指令，有消息时一般要将消息派发出去，再执行其它指令）。
GetMessage函数只有在消息队列中有消息时返回，队列中无消息就会一直等，直至下一个消息出现时才返回。在等的这段时间，应用程序不能执行任何指令。 
**（从他们的不同点上来看，PeekMessage函数有点像“乞丐行乞”，有你就施舍点，没有也不强求。GetMessage函数有点像“强盗打劫”，有你得给，没有我就等你什么时候有了再给，这段时间我什么都不干，我就等你。）**
