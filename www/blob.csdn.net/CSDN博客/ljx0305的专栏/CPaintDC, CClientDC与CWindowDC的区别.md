# CPaintDC, CClientDC与CWindowDC的区别 - ljx0305的专栏 - CSDN博客
2009年10月31日 12:55:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：493标签：[mfc																[windows																[constructor																[destructor																[application																[variables](https://so.csdn.net/so/search/s.do?q=variables&t=blog)](https://so.csdn.net/so/search/s.do?q=application&t=blog)](https://so.csdn.net/so/search/s.do?q=destructor&t=blog)](https://so.csdn.net/so/search/s.do?q=constructor&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)
个人分类：[MFC																[VC](https://blog.csdn.net/ljx0305/article/category/401790)](https://blog.csdn.net/ljx0305/article/category/403913)
**CPaintDC, CClientDC与CWindowDC的区别**     CClientDC（客户区设备上下文）用于客户区的输出，它在构造函数中封装了GetDC()，在析构函数中封装了ReleaseDC()函数。一般在响应非窗口重画消息（如键盘输入时绘制文本、鼠标绘图）绘图时要用到它。用法是：
CClientDC dc(this);//this一般指向本窗口或当前活动视图
dc.TextOut(10,10,str,str.GetLength());
//利用dc输出文本，如果是在CScrollView中使用，还要注意调
//用OnPrepareDC(&dc)调整设备上下文的坐标。
CPaintDC用于响应窗口重绘消息（WM_PAINT）是的绘图输出。CPaintDC在构造函数中调用BeginPaint()取得设备上下文，在析构函数中调用EndPaint()释放设备上下文。EndPaint()除了释放设备上下文外，还负责从消息队列中清除WM_PAINT消息。因此，在处理窗口重画时，必须使用CPaintDC，否则WM_PAINT消息无法从消息队列中清除，将引起不断的窗口重画。**CPaintDC也只能用在WM_PAINT消息处理之中。**
使用CPaintDC、CClientDC、CWindowDC的方法 
首先，定义一个这些类的实例变量，通常在栈中定义。然后，使用它。
例如，MFC中CView对WM_PAINT消息的实现方法如下：
void CView::OnPaint()
{
// standard paint routine
CPaintDC dc(this);
OnPrepareDC(&dc);
OnDraw(&dc);
}
在栈中定义了CPaintDC类型的变量dc，随着构造函数的调用获取了设备描述表；设备描述表使用完毕，超出其有效范围就被自动地清除，随着析构函数的调用，其获取的设备描述表被释放。
如果希望在堆中创建，例如
CPaintDC *pDC；
pDC = new CPaintDC(this)
则在使用完毕时，用delete删除pDC:
delete pDC;
直接使用CDC 
需要注意的是：在生成CDC对象的时候，并不像它的派生类那样，在构造函数里获取相应的Windows设备描述表。最好不要使用::GetDC等函数来获取一个设备描述表，而是创建一个设备描述表。其构造函数如下：
CDC::CDC()
{
m_hDC = NULL;
m_hAttribDC = NULL;
m_bPrinting = FALSE;
}
其析构函数如下：
CDC::~CDC()
{
if (m_hDC != NULL)
::DeleteDC(Detach());
}
在CDC析构函数中，如果设备描述表句柄不空，则调用DeleteDC删除它。这是直接使用CDC时最好创建Windows设备描述表的理由。如果设备描述表不是创建的，则应该在析构函数被调用前分离出设备描述表句柄并用::RealeaseDC释放它，释放后m_hDC为空，则在析构函数调用时不会执行::DeleteDC。当然，不用担心CDC的派生类的析构函数调用CDC的析构函数，因为CDC::~CDC()不是虚拟析构函数。
直接使用CDC的例子是内存设备上下文，例如：
CDC dcMem; //声明一个CDC对象
dcMem.CreateCompatibleDC(&dc); //创建设备描述表
pbmOld = dcMem.SelectObject(&m_bmBall);//更改设备描述表属性
…//作一些绘制操作
dcMem.SelectObject(pbmOld);//恢复设备描述表的属性
dcMem.DeleteDC(); //可以不调用，而让析构函数去删除设备描述表
## 系统何时发送WM_PAINT消息？
(来自：[http://dev.csdn.net/article/74/74935.shtm](http://dev.csdn.net/article/74/74935.shtm))
系统会在多个不同的时机发送WM_PAINT消息：当第一次创建一个窗口时，当改变窗口的大小时，当把窗口从另一个窗口背后移出时，当最大化或最小化窗口时，等等，这些动作都是由 系统管理的，应用只是被动地接收该消息，在消息处理函数中进行绘制操作；大多数的时候应用也需要能够主动引发窗口中的绘制操作，比如当窗口显示的数据改变的时候，这一般是通过InvalidateRect和 InvalidateRgn函数来完成的。InvalidateRect和InvalidateRgn把指定的区域加到窗口的Update Region中，当应用的消息队列没有其他消息时，如果窗口的Update Region不为空时，系统就会自动产生WM_PAINT消息。
系统为什么不在调用Invalidate时发送WM_PAINT消息呢？又为什么非要等应用消息队列为空时才发送WM_PAINT消息呢？这是因为系统把在窗口中的绘制操作当作一种低优先级的操作，于是尽 可能地推后做。不过这样也有利于提高绘制的效率：两个WM_PAINT消息之间通过InvalidateRect和InvaliateRgn使之失效的区域就会被累加起来，然后在一个WM_PAINT消息中一次得到 更新，不仅能避免多次重复地更新同一区域，也优化了应用的更新操作。像这种通过InvalidateRect和InvalidateRgn来使窗口区域无效，依赖于系统在合适的时机发送WM_PAINT消息的机 制实际上是一种异步工作方式，也就是说，在无效化窗口区域和发送WM_PAINT消息之间是有延迟的；有时候这种延迟并不是我们希望的，这时我们当然可以在无效化窗口区域后利用SendMessage 发送一条WM_PAINT消息来强制立即重画，但不如使用Windows GDI为我们提供的更方便和强大的函数：UpdateWindow和RedrawWindow。UpdateWindow会检查窗口的Update Region，当其不为空时才发送WM_PAINT消息；RedrawWindow则给我们更多的控制：是否重画非客户区和背景，是否总是发送WM_PAINT消息而不管Update Region是否为空等。
**Learn more:**
The CPaintDC Class
MFC's CPaintDC class lets you paint in a window's client area in response to WM_PAINT messages. You should use it only in OnPaint handlers and never anywhere else. WM_PAINT messages are different from all other Windows messages in one very important respect: If the handler fails to call the Windows ::BeginPaint and ::EndPaint functions (or the MFC equivalents, CWnd::BeginPaint and CWnd::EndPaint), the message will not be removed from the message queue no matter how much drawing you do. Consequently, the application will get stuck processing the same WM_PAINT message over and over. CPaintDC virtually ensures that this won't happen by calling ::BeginPaint and ::EndPaint from its constructor and destructor, respectively. 
The CClientDC and CWindowDC Classes
Windows programs don't always limit their painting to OnPaint. If you write an application that draws a circle on the screen whenever a mouse button is clicked, you'll probably want to paint the circle immediately—when you receive the button-click message—rather than wait for the next WM_PAINT message. 
That's what MFC's CClientDC class is for. CClientDC creates a client-area device context that can be used outside OnPaint. The following message handler uses CClientDC and two CDC member functions to draw an X connecting the corners of the window's client area when the left mouse button is clicked: 
void CMainWindow::OnLButtonDown (UINT nFlags, CPoint point)
{
    CRect rect;
    GetClientRect (&rect);
    CClientDC dc (this);
    dc.MoveTo (rect.left, rect.top);
    dc.LineTo (rect.right, rect.bottom);
    dc.MoveTo (rect.right, rect.top);
    dc.LineTo (rect.left, rect.bottom);
}
left, right, top, and bottom are public member variables defined in MFC's CRect class. They store the coordinates of the rectangle's four sides. MoveTo and LineTo are line-drawing functions that CClientDC inherits from CDC. You'll learn more about these two functions in a moment. 
For the rare occasions on which you'd like to paint not only the window's client area but also the nonclient area (the title bar, the window border, and so on), MFC provides the CWindowDC class. CWindowDC is similar to CClientDC, but the device context it represents encompasses everything within the window's borders. Programmers sometimes use CWindowDC for unusual effects such as custom-drawn title bars and windows with rounded corners. In general, you won't need CWindowDC very often. If you do want to do your own painting in a window's nonclient area, you can trap WM_NCPAINT messages with an OnNcPaint handler to determine when the nonclient area needs to be painted. Unlike OnPaint, an OnNcPaint handler need not (and should not) call BeginPaint and EndPaint. 
For the even rarer occasions on which a program requires access to the entire screen, you can create a CClientDC or CWindowDC object and pass its constructor a NULL pointer. The statements 
CClientDC dc (NULL);
dc.Ellipse (0, 0, 100, 100);
draw a circle in the upper left corner of the screen. Screen capture programs frequently use full-screen DCs to access the whole screen. Needless to say, drawing outside your own window is a very unfriendly thing to do unless you have a specific reason for doing so.
引用：[http://imcc.blogbus.com/logs/1962781.html](http://imcc.blogbus.com/logs/1962781.html)
