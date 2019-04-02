# 剖析Windows消息处理机制-测试

2016年12月12日 23:31:59 [wangpengk7788](https://me.csdn.net/wangpengk7788) 阅读数：209



## 1.窗口

   Windows程序是由一系列的窗口构成的，每个窗口都有自己的窗口过程，窗口过程就是一个拥有有固定 Signature 的 C函数，具体格式如下：
```cpp
   LRESULT CALLBACK WindowProc(HWND hwnd,
       UINT uMsg,
       WPARAM wParam,
       LPARAM lParam
   );
```
   窗口类型：
   可重叠窗口(Overlapped Window)，
   弹出窗口(Pop-up Window)，
   子窗口(Child Window)

   窗口之间的关系: 父子关系，拥有关系，前后关系。

## 2.线程
    一个进程至少拥有一个线程，称为主线程，如果一个线程创建了窗口，拥有GUI资源，那么也称该线程为GUI线程,否则就为工作线程。窗口是由线程创建的，
 创建窗口的线程就拥有该窗口。这种线程拥有关系的概念对窗口有重要的意义：建立窗口的线程必须是为窗口处理所有消息的线程。为了使这个概念更加明
 确具体，可以想像一个线程建立了一个窗口，然后就结束了。在这种情况下，窗口不会收到一个WM_DESTROY或WM_NCDESTROY消息，因为线程已经结束，不可
 能被用来使窗口接收和处理这些消息。每个线程，如果它至少建立了一个窗口，都由系统对它分配一个消息队列。这个队列用于窗口消息的派送(dispatch)。
 为了使窗口接收这些消息，线程必须有它自己的消息循环，消息循环一般如下：
```cpp
 MSG msg;
 while( GetMessage(&msg, NULL, 0, 0) )
 {
  TranslateMessage (&msg);
  DispatchMessage (&msg);
 }
```
 应用程序不断的从消息队列中获取消息,然后系统通过DispatchMessage函数分派消息到相应窗口的窗口过程，使得消息得到处理。当获取到WM_QUIT消息时，
 GetMessage返回0，循环结束。

## 3.消息
 消息，就是指Windows发出的一个通知，告诉应用程序某个事情发生了。例如，单击鼠标、改变窗口尺寸、按下键盘上的一个键都会使Windows发送一个消息
 给应用程序，它被定义为：
```cpp
  typedef struct {
  HWND hwnd;    //窗口句柄, 发生在哪个窗口上
  UINT message;   //消息标识号 ( WM_MOUSEMOVE, WM_LBUTTONDOWN, ... )
  WPARAM wParam;   //消息参数1
  LPARAM lParam;   //消息参数2
  DWORD time;
  POINT pt;
 } MSG, *PMSG;
```
 一个消息结构体包含了该事件 所有完备信息，当应用程序收到该消息时，就可以做出相应处理了。

###  消息分类

####  <1>.队列消息和非队列消息

  从消息的发送途径上看，消息分两种：队列消息和非队列消息。
  队列消息送到系统消息队列，然后到线程消息队列；非队列消息直接送给目的窗口过程。

  这里，对消息队列阐述如下：
  Windows维护一个系统消息队列(System message queue)，每个GUI线程有一个线程消息队列(Thread message queue)。鼠标、键盘事件由鼠标或键盘驱动
  程序转换成输入消息并把消息放进系统消息队列，例如WM_MOUSEMOVE、WM_LBUTTONUP、WM_KEYDOWN、WM_CHAR等等。Windows每次从系统消息队列移走一个
  消息，确定它是送给哪个窗口的和这个窗口是由哪个线程创建的，然后，把它放进窗口创建线程的线程消息队列。线程消息队列接收送给该线程所创建窗口
  的消息。线程从消息队列取出消息，通过Windows把它送给适当的窗口过程来处理。

  除了键盘、鼠标消息以外，队列消息还有WM_PAINT、WM_TIMER和WM_QUIT。这些队列消息以外的绝大多数消息是非队列消息。

####  <2>.系统消息和应用程序消息

  从消息的来源来看，可以分为：系统定义的消息和应用程序定义的消息。

  系统消息ID的范围是从0到WM_USER-1，或0X80000到0XBFFFF；应用程序消息从WM_USER(0X0400)到0X7FFF，或0XC000到0XFFFF；WM_USER到0X7FFF范围的消息
  由应用程序自己使用；0XC000到0XFFFF范围的消息用来和其他应用程序通信，为了ID的唯一性，使用::RegisterWindowMessage来得到该范围的消息ID。

####  <3>.窗口消息，命令消息，控件通知消息

  根据处理过程的不同,可以分为三类：窗口消息，命令消息，控件通知消息。

  (1).窗口消息
   一般以WM_开头，如WM_CREATE, WM_SIZE, WM_MOUSEMOVE等标准的Windows消息, 用于窗口相关的事件通知，窗口消息将由系统分配到该窗口的窗口过程处理。
  (2).命令消息 (WM_COMMAND)
   一种特殊的窗口消息，它从一个窗口发送到另一个窗口以处理来自用户的请求，通常是从子窗口发送到父窗口，例如，点击按钮时，按钮的父窗口会收到
   WM_COMMAND消息，用以通知父窗口按钮被点击，经测试：子窗口向父窗口发送WM_COMMAND消息，或者称为父窗口会收到WM_COMMAND消息，操作系统并不是
   通过将WM_COMMAND消息放入到父窗口的消息队列中去，而是直接调用了父窗口的窗口过程，以 WM_COMMAND 为消息标识参数(UINT uMsg)，实现这个功能的
   API函数正是: LRESULT DispatchMessage(const MSG *lpmsg);
  (3).控件通知消息
   WM_NOTIFY消息，当用户与控件交互(Edit, Button...)时，通知消息会从控件窗口发送到父窗口，这种消息的目的不是为了处理用户命令，而是为了让父窗
   口能够适时的改变控件。

## 4.测试

###  <1>.测试代码:

  消息循环中，将从消息队列中取出的消息逐一打印出来，

```cpp
while(GetMessage(&msg, NULL, 0, 0))
  {
   char buf[1024];
   sprintf_s(buf, 1024, "hWnd:%d uMsg: %d WParam: %d  LParam: %d\n",
    msg.hwnd, msg.message, msg.wParam, msg.lParam);
   std::cout<<buf;
   TranslateMessage(&msg);
   DispatchMessage(&msg);
  }
```

在窗口过程中，如果收到 WM_COMMAND 消息，就在窗口上输入来。



```cpp
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
  {
   switch (message)
   {
   case WM_COMMAND:
    HDhdc = GetDC (hwnd) ;
    SelectObject (hdc, GetStockObject (SYSTEM_FIXED_FONT)) ;
    TextOut(hdc, 24 * cxChar, cyChar * (rect.bottom / cyChar - 1),
      szBuffer,
      wsprintf(szBuffer, szFormat,
      TEXT ("WM_COMMAND"),
      HIWORD(wParam), LOWORD(wParam),
      HIWORD(lParam), LOWORD(lParam))
      );
    ReleaseDC (hwnd, hdc) ;
    ValidateRect (hwnd, &rect) ;
    break ;
   }
   return DefWindowProc (hwnd, message, wParam, lParam) ;
 }
```

###    <2>.运行结果

​    一个窗口，窗口上有一个按钮子窗口，然后还有一个控制台，输出消息循环中的每条消息，当父窗口收到 WM_COMMAND 消息时，

​    会在屏幕上输出。



![img](https://img-blog.csdn.net/20161212232420394?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

#### (1). 当鼠标在父窗口上移动时：



![img](https://img-blog.csdn.net/20161212232459063?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





可见父窗口 hWnd: 461982，uMsg : 521 ( WM_MOUSEMOVE )，从线程消息队列中取出的。



#### (2). 当鼠标在按钮子窗口上移动时：

![img](https://img-blog.csdn.net/20161212232620458?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​          可见按钮 hWnd: 462118，uMsg : 521 ( WM_MOUSEMOVE)，从线程消息队列中取出的。



####       (3). 当鼠标在父窗口上点击时

![img](https://img-blog.csdn.net/20161212232800271?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



 父窗口收到了 513(WM_LBUTTONDOWN), 514(WM_LBUTTONUP) 消息，从线程消息队列中取出的。



####  (4). 当鼠标在按钮窗口上点击时：



![img](https://img-blog.csdn.net/20161212232858022?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



​      按钮窗口从线程的消息队列中取出了 513(WM_LBUTTONDOWN), 514(WM_LBUTTONUP) 消息，父窗口收到了WM_COMMAND
​         消息，TextOut 绘制出 WM_COMMAND 文本









我在 WM_COMMAND 的消息处理语句处打有断点，看下图:



![img](https://img-blog.csdn.net/20161212232959975?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



可见，窗口过程是被系统调用的，调用时系统传入的参数值为：
                        hwnd:          0x00070c9e，十进制就是461982，父窗口句柄；
                        message:      273 (WM_COMMAND)
                        wParam:      ...
                         lParam:       ...
          具体是WinMain中的哪一个函数中最后调用了 窗口过程 WndProc 呢，见下图：

![img](https://img-blog.csdn.net/20161212233031290?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ3BlbmdrNzc4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







原来是在 DispatchMessage 函数中，再看看参数的值：
            msg.hwnd:             0x00070d26，十进制是462118，是按钮窗口的句柄;
            msg.message:         514 (  WM_LBUTTONUP  )
            哦~~~~，原来是操作系统在从该线程的消息队列中取出按钮的 WM_LBUTTONUP (鼠标左键释放) 消息后，调用
            DispatchMessage 分派消息，DispatchMessage 会先将 WM_LBUTTONUP 消息分派到按钮的窗口过程(系统默认有)，
            这里的分派到按钮的窗口过程就是调用俺就的窗口过程，然后又以 按钮的父窗口的句柄为 窗口过程的第一个
            参数， WM_COMMAND 为窗口过程的第二个参数 调用了 父窗口的窗口过程，也就是将 WM_COMMAND
            消息分发到了父窗口，从而使父窗口得到了通知。这些，都是 Windows 来完成的，应用程序只需要在相应的窗口
            过程中处理相应的消息。

​            从上面，我们还可以看出，WM_COMMAND 是非队列消息，直接分派到目的窗口过程，而不是放入到消息队列中，
​            让消息循环去取。





##  总结:

​           简而言之， 标准Windows消息发送到产生窗口，通知消息(WM_COMMAND, WM_NOTIFY)发送到父窗口，这是Windows
​           的标准消息处理过程，MFC对 Window API 进行了封装，有自己的一套消息处理流程， 消息顺着一条路径流动，需要
​           处理的对象可以添加消息响应函数处理之，对于命令消息，它有 CView , CDocument,  CMainFram ,  CWinApp 一系列处理
​          节点，对于通知消息，MFC还加入一种很好的机制：消息反射，就是父窗口收到子窗口发出的通知消息后，会将此消息
​           发送给子窗口，先让子窗口处理，如果子窗口不处理，父窗口再处理之，这样有利于将所有消息处理代码都集成了子窗口
​           中，有利于控件的开发。MFC的消息处理，我不予详细讨论了，有兴趣的可以参考侯捷的<<深入浅出MFC>>。