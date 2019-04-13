
# Windows消息机制 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[Windows消息机制](https://www.cnblogs.com/watsonyin/archive/2005/12/12/295536.html)
Windows消息机制
Windows操作系统最大的特点就是其图形化的操作界面，其图形化界面是建立在其消息处理机制这个基础之上的。如果不理解Windows消息处理机制，肯定无法深入的理解Windows编程。可惜很多程序员对Windows消息只是略有所闻，对其使用知之甚少，更不了解其内部实现原理，本文试着一步一步向大家披露我理解的Windows消息机制。可以说，掌握了这一部分知识，就是掌握了Windows编程中的神兵利器，灵活运用它，将会极大的提高我们的编程能力。
一、消息概述
Windows窗体是怎样展现在屏幕上的呢？众所周知，是通过API绘制实现的。Windows操作系统提供了一系列的API函数来实现界面的绘制功能，例如：
²DrawText绘制文字
²DrawEdge绘制边框
²DrawIcon绘制图标
²BitBlt绘制位图
²Rectangle绘制矩形
²…
再复杂的程序界面都是通过这个函数来实现的。
那什么时候调用这些函数呢？显然我们需要一个控制中心，用来进行“发号施令”，我们还需要一个命令传达机制，将命令即时的传达到目的地。这个控制中心，就是一个动力源，就像一颗心脏，源源不断地将血液送往各处。这个命令传达机制就是Windows消息机制，Windows消息就好比是身体中的血液，它是命令传达的使者。
Windows消息控制中心一般是三层结构，其顶端就是Windows内核。Windows内核维护着一个消息队列，第二级控制中心从这个消息队列中获取属于自己管辖的消息，后做出处理，有些消息直接处理掉，有些还要发送给下一级窗体(Window)或控件（Control）。第二级控制中心一般是各Windows应用程序的Application对象。第三级控制中心就是Windows窗体对象，每一个窗体都有一个默认的窗体过程，这个过程负责处理各种接收到的消息。如下图所示：
![](https://images.cnblogs.com/cnblogs_com/watsonyin/winmsg_1.jpg)
（注：windows指windows操作系统；窗口：即windows窗口；窗体：包括窗口，以及有句柄的控件；control指控件，控件本身也可能是一个window，也可能不是；Application即应用程序，应用程序也可能不会用到Windows消息机制，这里我们专门讨论有消息循环的应用程序）
图1包含了Windows机制的大部分内容，下面所讲的所有内容实际上都是对张图的解释或扩充。
消息是以固定的结构传送给应用程序的，结构如下：
Public Type MSG
hwnd As Long
message As Long
wParam As Long
lParam As Long
time As Long
pt As POINTAPI
End Type
其中hwnd是窗体的句柄，message是一个消息常量，用来表示消息的类型，wParam和lParam都是32位的附加信息，具体表示什么内容，要视消息的类型而定，time是消息发送的时间，pt是消息发送时鼠标所在的位置。
Windows操作系统中包括以下几种消息：
1、标准Windows消息：
这种消息以WM_打头。
2、通知消息
通知消息是针对标准Windows控件的消息。这些控个包括：按钮(Button)、组合框(ComboBox)、编辑框(TextBox)、列表框（ListBox）、ListView控件、Treeview控件、工具条(Toolbar)、菜单（Menu）等。每种消息以不同的字符串打头。
3、自定义消息
编程人员还可以自定义消息。
二、关于Windows句柄
不是每个控件都能接收消息，转发消息和绘制自身，只有具有句柄（handle）的控件才能做到。有句柄的控件本质上都是一个窗体(window)，它们可以独立存在，可以作为其它控件的容器，而没有句柄的控件，如Label，是不能独立存在的，只能作为窗口控件的子控件，它不能绘制自身，只能依靠父窗体将它绘制来。
句柄的本质是一个系统自动维护的32位的数值，在整个操作系统的任一时刻，这个数值是唯一的。但该句柄代表的窗体释放后，句柄也会被释放，这个数值又可能被其它窗体使用。也就是说，句柄的数值是动态的，它本身只是一个唯一性标识，操作系统通过句柄来识别和查找它所代表的对象。
然而，并非所有的句柄都是窗体的句柄，Windows系统中还中很多其它类型的句柄，如画布（hdc）句柄，画笔句柄，画刷句柄，应用程序句柄(hInstance)等。这种句柄是不能接收消息的。但不管是哪种句柄，都是系统中对象的唯一标识。本文只讨论窗体句柄。
那为什么句柄使窗口具有了如此独特的特性呢？实际是都是由于消息的原因。由于有了句柄，窗体能够接收消息，也就知道了该什么时候绘制自己，绘制子控件，知道了鼠标在什么时候点击了窗口的哪个部分，从而作出相应的处理。句柄就好像是一个人的身份证，有了它，你就可以从事各种社会活动；否则的话，你要么是一个社会看不到的黑户，要么跟在别人后面，通过别人来证明你的存在。
三、消息的传送
1、从消息队列获取消息：
可以通过PeekMessage或GetMessage函数从Windows消息队列中获取消息。Windows保存的消息队列是以线程(Thread)来分组的，也就是说每个线程都有自己的消息队列。
2、发送消息
发送消息到指定窗体一般通过以下两个函数完成：SendMessage和PostMessage。两个函数的区别在于：PostMessage函数只是向线程消息队列中添加消息，如果添加成功，则返回True,否则返回False，消息是否被处理，或处理的结果，就不知道了。而SendMessage则有些不同，它并不是把消息加入到队列里，而是直接翻译消息和调用消息处理，直到消息处理完成后才返回。所以，如果我们希望发送的消息立即被执行，就应该调用SendMessage。
还有一点，就是SendMessage发送的消息由于不会被加入到消息队列中，所以通过PeekMessage或GetMessage是不能获取到由SendMessage发送的消息。
另外，有些消息用PostMessage不会成功，比如wm_settext。所以不是所有的消息都能够用PostMessage的。
还有一些其它的发送消息API函数，如PostThreadMessage，SendMessageCallback，SendMessageTimeout，SendNotifyMessage等。
四、消息循环与窗体过程
消息循环是应用程序能够持续存在的根本原因。如果循环退出，则应用程序就结束了。
我们来看一看Delphi中封装的消息循环是怎样的：
第一步：程序开始运行(Run)
Application.Initialize;//初始化
Application.CreateForm(TForm1, Form1);//创建主窗体
Application.Run;//开始运行，准备进行消息循环
如果不创建主窗体，应用程序同样可以存在和运行。
第二步：开始调用消息循环(HandleMessage)
procedure TApplication.Run;
begin
FRunning := True;
try
AddExitProc(DoneApplication);
if FMainForm <> nil then
begin
case CmdShow of
SW_SHOWMINNOACTIVE: FMainForm.FWindowState := wsMinimized;
SW_SHOWMAXIMIZED: MainForm.WindowState := wsMaximized;
end;
if FShowMainForm then
if FMainForm.FWindowState = wsMinimized then
Minimize else
FMainForm.Visible := True;
Repeat//注：循环开始
try
HandleMessage;
except
HandleException(Self);
end;
until Terminated;//循环结束条件
end;
finally
FRunning := False;
end;
end;
第三步：消息循环中对消息的处理。
procedure TApplication.HandleMessage;
var
Msg: TMsg;
begin
if not ProcessMessage(Msg) then Idle(Msg);
end;
function TApplication.ProcessMessage(var Msg: TMsg): Boolean;
var
Handled: Boolean;
begin
Result := False;
if PeekMessage(Msg, 0, 0, 0, PM_REMOVE) then
begin
Result := True;
if Msg.Message <> WM_QUIT then
begin
Handled := False;
if Assigned(FOnMessage) then FOnMessage(Msg, Handled);
if not IsHintMsg(Msg) and not Handled and not IsMDIMsg(Msg) and
not IsKeyMsg(Msg) and not IsDlgMsg(Msg) then
begin
TranslateMessage(Msg);
DispatchMessage(Msg);
end;
end
else
FTerminate := True;
end;
end;
窗体过程实际上是一个回调函数。所谓的回调函数，实际上就是由Windows操作系统或外部程序调用的函数。回调函数一般都有规定的参数格式，以地址方式传递给调用者。窗口过程中是Windows操作系统调用了，在一个窗口创建的时候，在分配窗体句柄的时候就需要传入回调函数地址。那为什么我们平时编程看不到这个回调函数呢？这是由于我们的编程工具已经为我们生成了默认的窗体过程，这个过程的要做的事情就是判断不同的消息类型，然后做出不同的处理。例如可以为键盘或鼠标输入生成事件等。
五、消息与事件
事件本质上是对消息的封装，是IDE编程环境为了简化编程而提供的有用的工具。这个封装是在窗体过程中实现的。每种IDE封装了许多Windows的消息，例如：
事件
消息
|
OnActivate
WM_ACTIVATE
|
OnClick
WM_XBUTTONDOWN
|
OnCreate
WM_CREATE
|
OnDblClick
WM_XBUTTONDBLCLICK
|
OnKeyDown
WM_KEYDOWN
|
OnKeyPress
WM_CHAR
|
OnKeyUp
WIN_KEYUP
|
OnPaint
WM_PAINT
|
OnResize
WM_SIZE
|
OnTimer
WM_TIMER
|
了解了这一点后，我们完成可以封装自己的事件。
通过上面的介绍，相信各位已经对Windows消息机制有了一定的理解了。通过Windows消息编程，我们不但可以实现很多常规功能，而且可以实现很多IDE类库没有提供的功能；另外，我们还可以通过消息钩子，对消息进行截获，改变其默认的处理函数，从而突破平台或软件功能的限制，极大的扩展程序的功能；我们还可以修改默认的窗体过程，按自己的要求来响应消息；或者自定义消息，实现程序之间的即时通讯等等。通过更加深入的学习，我们还会接触到更多与Windows消息机制相关其它Windows相对比较底层的知识，如果能够这样，蓦然回首，你会发现自己原来离“高手”不远了。




posted on2005-12-12 13:58[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=295536)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
