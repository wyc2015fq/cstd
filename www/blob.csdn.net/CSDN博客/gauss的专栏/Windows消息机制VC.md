# Windows消息机制VC - gauss的专栏 - CSDN博客
2013年01月04日 22:16:16[gauss](https://me.csdn.net/mathlmx)阅读数：269
个人分类：[Window编程](https://blog.csdn.net/mathlmx/article/category/944343)
在Windows中，用户或系统中所发生的任何活动被当作事件来处理，例如，用户按下了鼠标按钮，就产生一鼠标事件。对于所发生的每一个事件，Windows将其转换成消息的形式放在一个称为消息队列的内存区中，然后由Windows的消息发送程序选择适合的对象，将消息队列中的消息发送到欲接受消息的对象上。Windows的消息可分为四种类型：   
  　　（1）输入消息：对键盘和鼠标输入作反应。这类输入消息首先放在系统消息队列中，然后Windows将它们送入应用程序的消息队列，使消息得到处理。   
  　　（2）控制消息：用来与Windows的特殊控制对象，例如，对话框、列表框、按钮等进行双向通信。这类消息一般不通过应用程序的消息队列，而是直接发送到控制对象上。   
  　　（3）系统消息：对程式化的事件或系统时钟中断作出反应。有些系统消息，例如大部分DDE消息（程序间进行动态数据交换时所使用的消息）要通过Windows的系统消息队列。而有些系统消息，例如窗口的创建及删除等消息直接送入应用程序的消息队列。   
  　　（4）用户消息：这些消息是程序员创建的，通常，这些消息只从应用程序的某一部分进入到该应用程序的另一部分而被处理，不会离开应用程序。用户消息经常用来处理选单操作：一个用户消息与选单中的一选项相对应，当它在应用程序队列中出现时被处理。   
  　　Windows应用程序通过执行一段称为消息循环的代码来轮询应用程序的消息队列，从中检索出该程序要处理的消息，并立即将检索到的消息发送到有关的对象上。典型的Windows应用程序的消息循环的形式为：   
  　　MSG　　msg;   
  　　while   (GetMessage(&msg,   NULL,   0,   0L))   
  　　{   
  　　　　TranslateMessage(&msg);   
  　　　　DispatchMessage(&msg);   
  　　}   
  　　函数GetMessage从应用程序队列中检索出一条消息，并将它存于具有MSG类型的一个变量中，然后交由函数TranslateMessage对该消息进行翻译，紧接着，函数DispatchMessage将消息发送到适当的对象上。
关于自定义消息的参数
自定义消息如果光是消息那只能是一个通知。   
  你知道什么时候该干什么事了。但是有的情况下需要具体的数据。这样参数就起作用了。   
  比如说要在消息处理中填充一个结构。   
  如struct   mystruct{   
  int   i;   
  char   buf[255];   
  }   
  可以把这个结构的一个指针强制转换为long  ，即（lparam)&mystruct，然后做为消息参数进入消息处理函数。   其他结构可以此类推。
  消息处理函数只要将WParam   或是LParam   强制转换为mystruct   *   就可以只用这个结构指针。可以为这个结构中的成员附值。或是使用这个结构中的数据。
1.SendMessage函数功能描述:将指定的消息发送到一个或多个窗口。此函数为指定的窗口调用窗口过程，直到窗口过程处理完消息后才返回。
**.函数原型：**
LRESULT SendMessage(
HWND hWnd, // 目标窗口句柄
UINT Msg, // 被发送的消息
WPARAM wParam, // 第一个消息参数
LPARAM lParam // 第二个消息参数
);
**.参数：**
hWnd
窗口过程接收消息的窗口句柄。如果此参数为HWND_BROADCAST，则消息被送到系统的所有顶层窗口，包括无效或不可见的非自身拥有的窗口、被覆盖的窗口和弹出式窗口。消息不被送到子窗口。
Msg 
指定被发送的消息； 
wParam 
指定附加消息的特定信息； 
lParam 
指定附加消息的特定信息。
**.返回值：**
返回值返回消息处理的结果，其依赖于所发送的消息。
**.备注：**
需要以 HWND_BROADCAST 方式通信的应用程序应该使用 RegisterWindowMessage 函数来获得应用程序间通信的独特消息。
如果指定的窗口通过调用线程被创建，则窗口过程作为子程序被立即调用。如果指定的窗口通过调用不同线程被创建，则系统切换到该线程并调用适当的窗口过程。线程间的消息只有在接收线程执行消息检索代码时才被处理。发送线程将被阻塞到接收线程处理完消息为止。
Windows CE：Windows CE不支持Windows桌面平台所支持的所有消息。在使用SendMessage函数之前，应检查发送的消息是否被Windows CE所支持。
**.使用环境：**
Windows NT： 3.1及以上版本；
Windows：95及以上版本；
Windows CE：1.0及以上版本；
头文件：winuser.h；
输入库：user32.lib；
Unicode：在WindowsNT（Windows2000）环境下以Unicode和ANSI方式实现。
**.示例代码：**
case WM_KEYDOWN:
switch (wParam)
{
case VK_HOME:
SendMessage (hwnd, WM_VSCROLL, SB_TOP, 0) ;
break ;
case VK_END:
SendMessage (hwnd, WM_VSCROLL, SB_BOTTOM, 0) ;
break ;
case VK_PRIOR:
SendMessage (hwnd, WM_VSCROLL, SB_PAGEUP, 0) ;
break ;
...
}
return 0 ;
...
**.常见问题**
1）使用SendMessage来实现剪切、复制和粘贴
SendMessage(hwnd, WM_COPY, 0, 0);
SendMessage(hwnd, WM_CUT, 0, 0);
SendMessage(hwnd, WM_PASTE, 0, 0);
2）SendMessage与PostMessage的区别
PostMessage将消息放入消息队列后马上返回，而SendMessage直到窗口过程处理完消息后才返回
3）SendMessage发送WM_COPYDATA消息在进程间传送数据
WM_COPYDATA消息主要目的是允许在进程间传递少量只读数据。SDK文档推荐用户使用SendMessage()函数，接收方在数据复制完成前不返回，这样发送方就不可能删除和修改数据。
例如：
std:string strData = "VC知识库 VCKBASE.COM";
COPYDATASTRUCT cds;
cds.dwData = 0;
cds.cbData = strData.Length();
cds.lpData = strData.c_str();
::SendMessage(hwnd, WM_COPYDATA, NULL, (LPARAM)&cds); 
2.SendMessageCallback
　　函数功能：该函数将指定的消息发送到一个或多个窗口。此函数为指定的窗口调用窗口程序，并立即返回。当窗口程序处理完消息后，系统调用指定的回调函数，将消息处理的结果和一个应用程序定义的值传给回调函数。　　函数原型：BOOL SendMessageCallback（HWND hwnd，UINT Msg，WPARAM wParam，LPARAM IParam，　　SEhDASYNCPROC IpResultCallBack，DWORD
 dwData）；　　参数：　　hWnd：其窗口程序将接收消息的窗口的句柄。如果此参数为HWND_BROADCAST，则消息将被发送到系统中所有顶层窗口，包括无效或不可见的非自身拥有的窗口、被覆盖的窗口和弹出式窗口，但消息不被发送到子窗口。　　Msg：指定被发送的消息。　　wParam：指定附加的消息指定信息。　　IParam：指定附加的消息指定信息。　　IpResultCallBack：指向回收函数的指针，窗曰程序处理完消息后调用该回调函数。参见SendAsyncProc可得到合适的回调函数的信息。如果hwnd为HWND_BROADCAST，系统为每个顶层窗口调用一次SendASyncProc回调函数。　　dwData：一个应用程序定义的值，被传给由参数IPResultCallBack指向的回调函数。　　返回值：如果函数调用成功，返回非零值。如果函数调用失败，返回值是零。若想获得更多的错误信息，请调用GetLastError函数。　　备注：如果发送一个低于WM_USER范围的消息给异步消息函数（PostMessage，SendNotifyMesssge；SendMessageCallback），消息参数不能包含指针。否则，操作将会失败。函数将在接收线程处理消息之前返回，发送者将在内存被使用之前释放。　　需要以HWND_BROADCAST方式通信的应用程序应当用函数RegisterWindwosMessage来获得应用程序间通信的独特的消息。　　此回调函数仅当调用SendMessagecallback的线程调用GetMessage，PeekMessage或WaitMessage时调用。
例如：
case WM_LBUTTONDOWN:
  SendMessageCallback(hWnd,WM_LBUTTONUP,0,0,SendAsyncProc,0);
//具体的回调函数实现
VOID CALLBACK SendAsyncProc(HWND hwnd,UINT uMsg,DWORD dwData,LRESULT lResult)
{
  MessageBox(NULL,"Back From Main Window","SendMessageCallback",MB_OK);
  //下面可以进行更进一步的处理
}
还有这样一个例子：
在《windows高级编程指南》中说道SendMessageCallback()在发送后就立即返回。但是我在堵塞测试的时候发现，调用SendMessageCallback 
的线程也死掉了， 
下面是我的代码 
大概就是在用SendMessageCallback发送消息给对话框之前，创建一个Event，设置为无信号，然后在 
对话框的消息响应中等待Event变为有信号才继续执行，根据书上所说，SendMessageCallback（）在发送会就返回， 
那么发送消息的线程就应该不会因为对话框在处理消息时候因为等待Event而给堵塞住，这样发送线程就可以继续执行下面的SetEvent， 
这样对话框WaitForSingleObject（）得以返回，这样它也可以得以继续执行 
。 
void   CMainFrame::OnOpendialog()   
{ 
  //该函数创建一个无模式对话框 
    CWorkingDialog*   pdlg=new   CWorkingDialog; 
    ASSERT_VALID(pdlg);   
  //Create   the   modeless   dialog   .   represents   this   dialog. 
  BOOL   bResult   =   pdlg-> Create(IDD_DIALOG1); 
  CString   strMsg= "From   MainFrame "; 
  HANDLE   hEvent=CreateEvent(NULL,TRUE,FALSE, "wait ");//创建事件通知   ，手动，无信号 
  SendMessageCallback(pdlg-> GetSafeHwnd(),WM_MSG,(WPARAM)(LPCTSTR)strMsg,0,FunCallback,0); 
strMsg= "Yes "; 
  SetEvent(hEvent); 
} 
//响应wm_msg   
void   CWorkingDialog::OnTestMsg(WPARAM   wParam   ,LPARAM   lParam) 
{ 
HANDLE   hEvent   =   CreateEvent(NULL,TRUE,FALSE, "wait "); 
ASSERT(GetLastError()==ERROR_ALREADY_EXISTS); 
CString   str((LPCTSTR)wParam); 
                WaitForSingleObject(hEvent,INFINITE);//等到有信号才执行下面的代码 
MessageBox(str); 
} 
但是这段代码运行结果跟我上面的猜想不一致，是不是我对SendMessageCallback的“发送就返回”理解错误？ 
原因是对于SendMessageCallback()函数， 如果发送到当前线程中，则直接调用窗口过程，发送到其它线程中则立即返回。
3. SendNotifyMessage
　　函数功能：该函数将指定的消息发送到一个窗口。如果该窗口是由调用线程创建的；此函数为该窗口调用窗口程序，并等待窗口程序处理完消息后再返回。如果该窗口是由不同的线程创建的，此函数将消息传给该窗口程序，并立即返回，不等待窗口程序处理完消息。
　　函数原型：BOOL SendNotifyMessage（HWND hWnd，UINT Msg，WPARAM wParam，LPARAM IParam）；
　　参数：
　　hWnd：其窗口程序将接收消息的窗口的句柄。如果此参数为HWND_BROADCAST，则消息将被发送到系统中所有顶层窗口，包括无效或不可见的非自身拥有的窗口、被覆盖的窗口和弹出式窗口，但消息不被发送到子窗口。
　　Msg：指定被发送的消息。
　　wParam：指定附加的消息指定信息。
　　IParam：指定附加的消息指定信息。
　　返回值：如果函数调用成功，返回非零值；如果函数调用失败，返回值是零。若想获得更多的错误信息，请调用GetLastError函数。
　　备注：如果发送一个低于WM_USER范围的消息给异步消息函数（PostMessage，SendNotifyMessage，SendMesssgeCallback），消息参数不能包含指针。否则，操作将会失败。函数将在接收线程处理消息之前返回，发送者将在内存被使用之前释放。
　　需要以HWND_BROADCAST方式通信的应用程序应当用函数RegisterWindwosMessage来获得应用程序间通信的独特的消息。
例子：
case WM_RBUTTONDOWN:
  HANDLE hThread;
  DWORD ThreadID;
  //创建一个线程，把当前的窗口句柄作为参数传递给新线程
  hThread=CreateThread(NULL,0,Thread1,hWnd,NULL,&ThreadID);
  break;
case WM_LBUTTONDOWN:
  MessageBox(NULL,"Receiving Message：WM_LBUTTONDOWN","MESSAGE",MB_OK);
  break; 
//具体的线程函数实现
DWORD WINAPI  Thread1(LPVOID param)
{
  HWND hWnd=(HWND)param;
   SendNotifyMessage(hWnd,WM_LBUTTONDOWN,0,0);
  //一般情况下，这个消息框要比上面那个消息框早出现，因为函数并不等待消息
  //处理完成之后才返回
  MessageBox(NULL,"Return From SendNotifyMessage","SendNotifyMessage",MB_OK);
  return 1;
}
4. SendMessageTimeout
　　函数功能：该函数将指定的消息发送到一个或多个窗口。此函数为指定的窗口调用窗口程序，并且，如果指定的窗口属于不同的线程，直到窗口程序处理完消息或指定的超时周期结束函数才返回。如果接收消息的窗口和当前线程属于同一个队列，窗口程序立即调用，超时值无用。　　函数原型：LRESULT SendMessageTimeout（HWND hwnd，UINT Msg，WPARAM wParam，LPARAM IParam，UINTfuFlags，UIUT
 uTimeout，LPDWORD lpdwResultult）；　　参数：　　hWnd：其窗口程序将接收消息的窗口的句柄。如果此参数为HWND_BROADCAST，则消息将被发送到系统中所有顶层窗口，包括无效或不可见的非自身拥有的窗口。　　Msg：指定被发送的消息。　　wParam：指定附加的消息指定信息。　　IParam：指定附加的消息指定信息。　　fuFlags；指定如何发送消息。此参数可为下列值的组合：　　SMTO_ABORTIFHUNG：如果接收进程处于“hung”状态，不等待超时周期结束就返回。　　SMTO_BLOCK：阻止调用线程处理其他任何请求，直到函数返回。　　SMTO_NORMAL：调用线程等待函数返回时，不被阻止处理其他请求。　　SMTO_NOTIMEOUTIFNOTHUNG：Windows
 95及更高版本：如果接收线程没被挂起，当超时周期结束时不返回。　　uTimeout：为超时周期指定以毫秒为单位的持续时间。如果该消息是一个广播消息，每个窗口可使用全超时周期。例如，如果指定5秒的超时周期，有3个顶层窗回未能处理消息，可以有最多15秒的延迟。　　IpdwResult：指定消息处理的结果，依赖于所发送的消息。　　返回值：如果函数调用成功，返回非零值。如果函数调用失败，或超时，返回值是零。若想获得更多的错误信息，请调用GetLastError函数。如果GetLastError返回零，表明函数超时。如果使用HWND_BROADCAST，SenddMessaggTimeout不提供单个窗口超时信息。
DWORD WINAPI  Thread1(LPVOID param);
case WM_RBUTTONDOWN:
  HANDLE hThread;
  DWORD ThreadID;
  //创建一个线程，把当前的窗口句柄作为参数传递给新线程
  hThread=CreateThread(NULL,0,Thread1,hWnd,NULL,&ThreadID);
  break;
case WM_LBUTTONDOWN:
  Sleep(5000);//睡眠5秒
  MessageBox(NULL,"Receving Messageg：LBUTTONDOWN","MESSAGE",MB_OK);
  break;
DWORD WINAPI  Thread1(LPVOID param)
{
  HWND hWnd=(HWND)param;
  DWORD dwResult;
  LRESULT ret;
  //只等待三秒
  ret=SendMessageTimeout(hWnd,WM_LBUTTONDOWN,0,0,SMTO_BLOCK,3000,&dwResult);
  if(ret==0)
   MessageBox(NULL,"Fail To Process Message","SendMessageTimeout",MB_OK);
  else
   MessageBox(NULL,"Success To Process Message","SendMessageTimeout",MB_OK);
  return 1;
}
如果不 Sleep(5000);//睡眠5秒。先  MessageBox(NULL,"Receving Messageg：LBUTTONDOWN","MESSAGE",MB_OK);
后 if(ret==0)；但是Sleep(5000);先 if(ret==0)后MessageBox(NULL,"Receving Messageg：LBUTTONDOWN","MESSAGE",MB_OK);
5.PostThreadMessage
函数功能：该[函数](http://baike.baidu.com/view/15061.htm)将一个消息放入（寄送）到指定线程的消息队列里，不等待[线程](http://baike.baidu.com/view/1053.htm)处理消息就返回。
　　函数原型：BOOL PostThreadMessage（DWORD idThread，UINT Msg，WPARAM wParam，LPARAM IParam）；
　　参数
　　idThread：其消息将被寄送的线程的线程标识符。如果线程没有消息队列，此函数将失败。当线程第一次调用一个Win 32 USER或GDI函数时，系统创建线程的消息队列。要得到更多的信息，参见备注。
　　Msg：指定将被寄送的消息的类型。
　　wParam：指定附加的消息特定信息。
　　IParam：指定附加的消息特定信息。
　　返回值：如果函数调用成功，返回非零值。如果函数调用失败，返回值是零。若想获得更多的错误信息，请调用GetLastError函数。如果idThread不是一个有效的线程标识符或由idThread确定的线程没有消息队
　　列，GetLastError返回ERROR_INVALID_THREAD。
　　备注：消息将寄送到的线程必须创建消息队列，否则调用PostThreadMessage会失败。用下列方法之一来处理这种情况：
　　调用PostThreadMessage。如果失败，调用Sleep，再调用PostThreadMessage，反复执行，直到PostThreadMessage成功。
　　创建一个事件对象，再创建线程。在调用PostThreadMessage之前，用函数WaitForSingleObject来等特事件被设置为被告知状态。消息将寄送到的线程调用PeekMessage（&msg，NULL，WM_USER，WM_USER，PM_NOREMOVE）来强制系统创建消息队列。设置事件，表示线程已准备好接收寄送的消息。
　　消息将寄送到的线程通过调用GetMesssge或PeekMesssge来取得消息。返回的MSG结构中的hwnd成员为NULL。 
case WM_RBUTTONDOWN:
  HANDLE hThread;
  DWORD ThreadID,LocalThreadID;
  //将当前线程ID作为参数传递给新线程
  LocalThreadID=GetCurrentThreadId();
  hThread=CreateThread(NULL,0,Thread1,(LPVOID)LocalThreadID,NULL,&ThreadID);
  break;
//具体的线程函数实现
DWORD WINAPI  Thread1(LPVOID param)
{
  DWORD MainThreadID;
  MainThreadID=(DWORD)param;
  //向主线程发送一条消息
  PostThreadMessage(MainThreadID,WM_LBUTTONDOWN,0,0);
  return 1;
}
6.函数功能：该函数将一个消息放入（寄送）到与指定窗口创建的线程相联系消息队列里，不等待线程处理消息就返回，是异步消息模式。消息队列里的消息通过调用GetMessage和PeekMessage取得。　　函数原型：B00L PostMessage（HWND hWnd，UINT Msg，WPARAM wParam，LPARAM lParam）；　　参数　　hWnd：其窗口程序接收消息的窗口的句柄。可取有特定含义的两个值：　　HWND_BROADCAST：消息被寄送到系统的所有顶层窗口，包括无效或不可见的非自身拥有的窗口、被覆盖的窗口和弹出式窗口。消息不被寄送到子窗口。　　NULL：此函数的操作和调用参数dwThread设置为当前线程的标识符PostThreadMessage函数一样。　　Msg：指定被寄送的消息。　　wParam：指定附加的消息特定的信息。　　IParam：指定附加的消息特定的信息。　　返回值：如果函数调用成功，返回非零值：如果函数调用失败，返回值是零。若想获得更多的错误信息，请调用GetLastError函数。　　备注：需要以
 HWND_BROADCAST方式通信的应用程序应当用函数 RegisterwindwosMessage来获得应用程序间通信的独特的消息。　　如果发送一个低于WM_USER范围的消息给异步消息函数（PostMessage.SendNotifyMessage，SendMesssgeCallback），消息参数不能包含指针。否则，操作将会失败。函数将再接收线程处理消息之前返回，发送者将在内存被使用之前释放。　　速查：Windows NT： 3.1及以上版本；Windows：95及以上版本；Windows CE：1.0及以上版本；头文件：winuser.h；输入库：user32.lib；Unicode：在Windows
 NT环境下以Unicode和ANSI方式实现。　　P.S. 为什么 “如果发送一个低于WM_USER范围的消息给异步消息函数（PostMessage.SendNotifyMessage，SendMesssgeCallback），消息参数不能包含指针。否则，操作将会失败。
#define WM_SENDMESSAGE WM_USER+1
#define WM_POSTMESSAGE WM_USER+2
DWORD WINAPI  Thread1(LPVOID param)
case WM_SENDMESSAGE:
  MessageBox(NULL,"本对话框结束之后SendMessage才会返回!","SendMessage",MB_OK);
 break;
case WM_POSTMESSAGE:
  MessageBox(NULL,"本对话框结束之前PostMessage已经返回!","SendMessage",MB_OK);
  break;
case WM_RBUTTONDOWN:
  HANDLE hThread;
  DWORD ThreadID;
  //创建一个线程，将当前的窗口句柄作为参数传递给该新创建的线程
  hThread=CreateThread(NULL,0,Thread1,hWnd,NULL,&ThreadID);
  break;
//具体的线程函数实现
DWORD WINAPI  Thread1(LPVOID param)
{
  HWND hWnd;
  hWnd=(HWND)param;
  //可以比较下面两种发送消息的差别
  PostMessage(hWnd,WM_POSTMESSAGE,0,0);
// SendMessage(hWnd,WM_SENDMESSAGE,0,0);
  return 1;
}
7. PostQuitMessage
函数功能：该函数向系统表明有个线程有终止请求。通常用来响应WM_DESTROY消息。
　　函数原型：VOID PostQuitMessage（int nExitCode）；
　　参数：
　　nExitCode：指定应用程序退出代码。此值被用作消息WM_QUIT的wParam参数。
　　返回值：无。
　　备注：PostQuitMessage寄送一个WM_QUIT消息给线程的消息队列并立即返回；此函数向系统表明有个线程请求在随后的某一时间终止。
　　当线程从消息队列里取得WM_QUIT消息时，应当退出消息循环并将控制返回给系统。返回给系统的退出值必须是消息WM_QUIT的wParam参数。
case WM_LBUTTONDOWN:
   PostQuitMessage(1);
8.BroadcastSystemMessage
该函数发送消息给指定的接受者。接受者可以是一个应用程序、安装驱动器、网络驱动器、系统级设备驱动器或这些系统组件的组合。　　函数原型：long BroadcastSystemMessage（DWORD dwFIags，LPDWORD IpdwRecipients,UINT UiMessage，WPARAMwParam,LPARAM IParam）；　　参数： dwFlags：选项标志。可取下列值的组合：　　BSF_FLUSHDISK：接受者处理消息之后清洗磁盘。　　BSF_FORCEIFHUNG：继续广播消息，即使超时周期结束或一个接受者已挂起。
 BSF_IGNORECURRENTTASK：不发送消息给属于当前任务的窗口。这样，应用程序就不会接收自己的消息。　　BSF_NOHANG：强制挂起的应用程序超时。如果一个接受者超时，不再继续广播消息。 BSF_NOTIMEOUTIFNOTHUNG：只要接受者没挂起，一直等待对消息的响应。不会出现超时。 BSF_POSTMESSAGE：寄送消息。不能和BSF_QUERY组合使用。　　BSF_QUERY：每次发送消息给一个接受者，只有当前接受者返回TRUE后，才能发送给下一个接受者。 lpdwRecipients：指向变量的指针，该变量含有和接收消息接受者的信息。此变量可为下列值的组合：
 BSM_ALLCOMPONENTS：广播到所有的系统组件。　　BSM_ALLDESKTOPS：Windows NT下，广播到所有的桌面。要求SE_TCB_NAME特权。　　BSM_APPLICATIONS：广播到应用程序。　　BSM_INSTALLABLEDRIVERS：Windows 95下，广播到安装驱动器。　　BSM_INTDRIVER：Windows 95下，广播到网络驱动器。　　BSM_VXDS：Windows 95下，广播到所有系统级设备驱动器。　　当函数返回时，此变量接受上述值的组合，以确定真正接受消息的接受者。如果此参数为NULL，则将消息广播到所有的组件。　　uiMessage：系统消息标识符。　　WParam：32位消息特定值。　　IParam：32位消息特定值。　　返回值:如果函数调用成功，返回值是正数。如果函数不能广播消息，返回值是C1。如果参数dwFlags为BSF_QUERY且至少一个接受者返回BROADCAST_QUERY_DENY给相应的消息，返回值是零。若想获得更多的错误信息，请调用GetLastError函数。
 备注：如果BSF_QUERY没指定，函数发送指定的消息给所有请求的接受者，并忽略这些接受者返回的值。
RegisterWindowMessage中的BroadcastSystemMessage如何处理
我想用BroadcastSystemMessage来在两个进程之间通讯,我从一个进程发送了一个用 RegisterWindowMessage注册过的消息,但在目的进程中却没有收到该消息.
A:我认为你应该在两个进程的最高级窗口中都注册该消息.请看下例:
static UINT sBroadcastCommand = ::RegisterWindowMessage( _T("BroadcastCommand"));
BEGIN_MESSAGE_MAP( Gui_Top_Level_MainFrame, Gui_MainFrame )
ON_REGISTERED_MESSAGE( sBroadcastCommand, onBroadcastCommand )
END_MESSAGE_MAP()
LRESULT Gui_MainFrame :: onBroadcastCommand( UINT aMsg, LPARAM lParam )
{
your code...
}
然后发送进行应该包含:
While the sending process would contain:
static UINT sBroadcastCommand = ::RegisterWindowMessage( _T("BroadcastCommand"));
void Someclass :: someMethod( void )
{
::PostMessage( (HWND)HWND_BROADCAST,sBroadcastCommand, 0,yourMessageId );
}9.ReplyMessage函数功能：该函数用于应答由函数SendMessage发送的消息，不返回控制给调用SendMessage的函数。
　　函数原型：BOOL ReplyMessage（LRESULTIResult）；
　　参数：
　　IResult：指定消息处理的结果。可能的值由所发送的消息确定。
　　返回值：如果调用线程正处理从其他线程或进程发送的消息，返回非零值。如果调用线程不是正处理从其他线程或进程发送的消息，返回值是零。
　　备注：调用此函数，接收消息的窗口程序允许调用SendMessage的线程继续运行，尽管接收消息的线程已返回控制。调用ReplyMessage的线程也继续运行。
　　如果消息不是通过SendMessage发送的，或者消息由同一个线程发送，ReplyMessage不起作用。 case WM_LBUTTONDOWN:
  LRESULT result;
  result=1;
  //去掉下面语句，则SendMessage函数将等待5秒钟才会返回
  ReplyMessage(result);
  Sleep(5000);
  break;//具体的线程函数实现
DWORD WINAPI  Thread1(LPVOID param)
{
  HWND hWnd=(HWND)param;
  SendMessage(hWnd,WM_LBUTTONDOWN,0,0);
  MessageBox(NULL,"Come Back","SendMessage",MB_OK);
  return 1;
}while (WaitMessage())
{
  PeekMessage(&msg,NULL,0,0,PM_REMOVE);
  if(msg.message==WM_QUIT)
   break;
  //上面的几条语句相当于while(GetMessage(&msg,NULL,0,0)
  if(msg.hwnd==NULL)
   MessageBox(NULL,"Receive Message From PostThreadMessage",
              "PostThreadMessage",MB_OK);
  if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
  {
   TranslateMessage(&msg);
   DispatchMessage(&msg);
  }
 }10.
函数GetMessage 是 从调用线程的消息队列里取得一个消息并将其放于指定的结构。此函数可取得与指定窗口联系的消息和由PostThreadMesssge寄送的线程消息。此函数接收一定范围的消息值。GetMessage不接收属于其他线程或应用程序的消息。获取消息成功后，线程将从消息队列中删除该消息。函数会一直等待直到有消息到来才有返回值。
GetMessage
BOOL GetMessage（LPMSG lpMsg，HWND hWnd，UINT wMsgFilterMin，UINT wMsgFilterMax 
　　参数： 
　　lpMsg：指向MSG结构的指针，该结构从线程的消息队列里接收消息信息。 
　　hWnd：取得其消息的窗口的句柄。这是一个有特殊含义的值（NULL）。GetMessage为任何属于调用线程的窗口检索消息，线程消息通过PostThreadMessage寄送给调用线程。 
　　wMsgFilterMin：指定被检索的最小消息值的整数。 
　　wMsgFilterMax：指定被检索的最大消息值的整数。 
　　返回值：如果函数取得WM_QUIT之外的其他消息，返回非零值。如果函数取得WM_QUIT消息，返回值是零。如果出现了错误，返回值是-1。例如，当hWnd是无效的窗口句柄或lpMsg是无效的指针时。若想获得更多的错误信息，请调用GetLastError函数。 
　　应用程序通常用返回值来确定是否终止主[消息循环](http://baike.baidu.com/view/3068833.htm)并退出程序。 
　　GetMesssge只接收与参数hWnd标识的窗口或子窗口相联系的消息，子窗口由函数IsChild决定，消息值的范围由参数wMsgFilterMin和wMsgFilterMax给出。如果hWnd为NULL，则GetMessage接收属于调用线程的窗口的消息，线程消息由函数PostThreadMessage寄送给调用线程。GetMessage不接收属于其他线程或其他线程的窗口的消息，即使hWnd为NULL。由PostThreadMessage寄送的线程消息，其消息hWnd值为NULL。如果wMsgFilterMin和wMsgFilterMax都为零，GetMessage返回所有可得的消息（即，无范围过滤）。 
　　常数 WM_KEYFIRST和WM_KEYAST可作为过滤值取得与键盘输入相关的所有消息：常数WM_MOUSEFIRST和WM_MOUSELST可用来接收所有的鼠标消息。如果wMsgFilterMin和wMsgFilterMax都为零，GetMessage返回所有可得的消息（即，无范围过滤）。 
　　GetMessage不从队列里清除WM.PAINT消息。该消息将保留在队列里直到处理完毕。 
　　注意，此函数的返回值可非零、零或-1，应避免如下代码出现： 
　　while（GetMessage（IpMsg，hwnd，0，0））… 
　　－1返回值的可能性表示这样的代码会导致致命的应用程序错误。 
11.WaitMessage
函数功能：该函数产生对其他线程的控制，如果一个线程没有其他消息在其消息队列里。此函数中止线程，直到一个新消息被放入该线程的消息队列里，再返回。
　　函数原型；BOOL WaitMessage（VOID）
　　参数：无。
　　返回值：如果函数调用成功，返回非零值；如果函数调用失败，返回值是零。若想获得更多的错误信息，请调用GetLastError函数。
　　备注：在线程调用一个函数来检查队列后，如果有未经阅读的输入在消息队列里，WaitMessage不返回。这是因为PeekMessage，GetMessage，GetQueueStatus：WaitMessage，MsgWaitForMultipleObjects，MsgWaitForMulitpleObjectEx等函数检查队列后，改变队列的状态信息这样输入不再被认为是新的。如果连续调用WaitMessage，将等到指定类型的新输入到达后才返回。已存在的未读过的输入（在上次线程检查队列之前接收的）被忽略。 
12.TranslateAccelerator
函数功能：翻译加速键表。该函数处理菜单命令中的加速键。该函数将一个WM_KEYDOWN或WM_SYSKEYDOWN消息翻译成一个WM_COMMAND或WM_SYSCOMMAND消息（如果在给定的加速键表中有该键的入口），然后将WM_COMMAND或WM_SYSCOMMAND消息直接送到相应的窗口处理过程。
　　TranslateAccelerator直到窗口过程处理完消息后才返回。
　　函数原型：int TranslateAccelerator（HWND hWnd,HACCEL hAccTable，LPMSG IpMsg）; 　　：
　　hWnd:窗口句柄，该窗口的消息将被翻译。
　　hAccTable:加速键表句柄。加速键表必须由LoadAccelerators函数调用装入或由CreateAccd_eratorTable函数调用创建。
　　LpMsg:MSG结构指针，MSG结构中包含了从使用GetMessage或PeekMessage函数调用线程消息队列中得到的消息内容。
　　返回值：若函数调用成功，则返回非零值；若函数调用失败，则返回值为零。若要获得更多的错误信息，可调用GetLastError函数。 　　：为了将该函数发送的消息与菜单或控制发送的消息区别开来，使WM_COMMAND或WM_SYSCOMMAND消息的wParam参数的高位字值为1。用于从窗口菜单中选择菜单项的加速键组合被翻译成WM_SYSCOMMAND消息：所有其他的加速键组合被翻译成WM_COMMAND。若TransLateAccelerator返回非零值且消息已被翻译，应用程序就不能调用TranslateMessage函数对消息再做处理。每个加速键不一定都对应于菜单命令。若加速键命令对应于菜单项，则WM_INITMEMU和WM_INITMENUPOPUP消息将被发送到应用程序，就好像用户正试图显示该菜单。然而，如下的任一条件成立时，这些消息将不被发送：窗口被禁止，菜单项被禁止。
　　加速键组合无相应的窗口菜单项且窗口己被最小化。鼠标抓取有效。有关鼠标抓取消息，参看SetCapture函数。若指定的窗口为活动窗口且窗口无键盘焦点（当窗口最小化时一般是这种情况）， **TranslateAccelerator** 翻译WM_SYSDEYUP和WM_SYSKEYDOWN消息而不是WM_KEYUP和WM_KEYDOWN消息。
　　当按下相应于某菜单项的加速键，而包含该菜单的窗口又已被最小化时， **TranslateAccelerator** 不发送WM_COMMAND消息。但是，若按下与窗口菜单或某单项的任一项均不对应的加速键时， **TranslateAccelerator** 将发送一WM_COMMAND消息，即使窗口己被最小化。
　　Windows CE：所有的加速键消息被翻译成WM_COMMAND消息；Windows CE不支持WM_SYSCOMMAND消息。 
13.TranslateMessage
函数功能：该函数将虚拟键消息转换为字符消息。字符消息被寄送到调用线程的消息队列里，当下一次线程调用函数GetMessage或PeekMessage时被读出。
　　函数原型：BOOL TranslateMessage（CONST MSG*lpMsg）；
　　IpMsg：指向含有消息的MSG结构的指针，该结构里含有用函数GetMessage或PeekMessage从调用线程的消息队列里取得的消息信息。
　　返回值：如果消息被转换（即，字符消息被寄送到调用线程的消息队列里），返回非零值。如果消息是WM_KEYDOWN，WM_KEYUP WM_SYSKEYDOWN或WM_SYSKEYUP，返回非零值，不考虑转换。如果消息没被转换（即，字符消息没被寄送到调用线程的消息队列里），返回值是零。
　　备注：此函数不修改由参数IpMsg指向的消息。
　　WM_KEYDOWN和WM_KEYUP组合产生一个WM_CHAR或WM_DEADCHAR消息。
　　WM_SYSKEYDOWN和WM_SYSKEYUP组合产生一个WM_SYSCHAR或 WM_SYSDEADCHAR消息。TranslateMessage为那些由键盘驱动器映射为ASCll字符的键产生WM_CHAR消息。
　　如果应用程序为其他用途处理虚拟键消息，不应调用TranslateMessage。例如，如果件TranslateAccelerator返回一个非零值，应用程序不应调用TranslateMessage。
　　Windows CE：Windows CE不支持扫描码或扩展键标志，因此，不支持由TranslateMessage产生的WM_CHAR消息中的IKeyData参数（IParam）取值16-24。
　　TranslateMessage只能用于转换调用GetMessage或PeekMessage接收的消息。 
while (GetMessage(&msg, NULL, 0, 0)) 
{
  if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
  {
   TranslateMessage(&msg);
   MSG msg1;
   //从这里可以看出，该函数将WM_KEYDOWN消息转换为WM_CHAR等消息
   //在消息队列中只有WM_KEYDOWN消息的情况下就将其转换为WM_CHAR了
   if(PeekMessage(&msg1,NULL,WM_CHAR,WM_CHAR,PM_NOREMOVE))
    MessageBox(NULL,"Find WM_CHAR Message","WM_CHAR",MB_OK);
   DispatchMessage(&msg);
  }
 }
14.DispatchMessage
函数功能：该函数分发一个消息给窗口程序。通常消息从GetMessage函数获得。消息被分发到回调函数（过程函数)，作用是消息传递给操作系统，然后操作系统去调用我们的回调函数，也就是说我们在窗体的过程函数中处理消息
　　函数原型：LONG DispatchMessage（CONST MSG*lpmsg）；
　　参数：
　　lpmsg：指向含有消息的MSG结构的指针。
　　返回值：返回值是窗口程序返回的值。尽管返回值的含义依赖于被调度的消息，但返回值通常被忽略。
　　备注：MSG结构必须包含有效的消息值。如果参数lpmsg指向一个WM_TIMER消息，并且WM_TIMER消息的参数IParam不为NULL，则调用IParam指向的函数，而不是调用窗口程序。
　　速查：Windows NT：3.1及以上版本；Windows：95及以上版本；Windows CE：1.0及以上版本；头文件：winuser.h；输入库：user32.lib；Unicode：在Windows NT环境下以Unicode和ANSI方式实现。
**转载声明：** 本文转自 [http://www.360doc.com/content/10/1218/11/3972135_79208356.shtml](http://www.360doc.com/content/10/1218/11/3972135_79208356.shtml)
