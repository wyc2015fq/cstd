# HDC与CDC相互转换 - xqhrs232的专栏 - CSDN博客
2010年08月27日 10:23:00[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：854标签：[图形																[mfc																[windows																[数据结构																[api																[工具](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=api&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=图形&t=blog)
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.csdn.net/loop_k/archive/2009/08/24/4476797.aspx](http://blog.csdn.net/loop_k/archive/2009/08/24/4476797.aspx)
首先说一下什么是DC（设备描述表）：Windows应用程序通过为指定设备（屏幕，打印机等）创建一个设备描述表（Device Context, DC）在DC表示的逻辑意义的“画布”上进行图形的绘制。DC是一种包含设备信息的数据结构，它包含了物理设备所需的各种状态信息 。Win32程序在绘制图形之前需要获取DC的句柄HDC，并在不继续使用时释放掉。
在c++ 编程中常会见到HDC,CDC,CClientDC,CPaintDC,CWindowDC这样的类
HDC是DC的句柄,API中的一个类似指针的数据类型.
CDC是MFC的DC的一个类
CDC等设备上下分类,都含有一个类的成员变量:m_nHdc;即HDC类型的句柄.
CDC及其派生类的继承视图:
CObject
public |------CDC
public |------|------CClientDC
public |------|------CPaintDC
public |------|------CWindowDC
public |------|------CMetaFileDC
(注意: 除CMetaFileDC以外的三个派生类用于图形绘制.)
CDC类定义了一个设备描述表相关的类，其对象提供成员函数操作设备描述表进行工作，如显示器，打印机，以及显示器描述表相关的窗口客户区域。
通过CDC的成员函数可进行一切绘图操作。CDC提供成员函数进行设备描述表的基本操作，使用绘图工具，选择类型安全的图形设备结构（GDI），以及色彩，调色板。除此之外还提供成员函数获取和设置绘图属性，映射，控制视口，窗体范围，转换坐标，区域操作，裁减，划线以及绘制简单图形（椭圆，多边形等）。成员函数也提供绘制文本，设置字体，打印机换码，滚动， 处理元文件。
其派生类:
1.PaintDC: 封装BeginPaint和EndPaint两个API的调用。
(1)用于响应窗口重绘消息（WM_PAINT）的绘图输出。
(2)CPaintDC在构造函数中调用BeginPaint()取得设备上下文，在析构函数中调用EndPaint()释放设备上下文。 EndPaint()除了释放设备上下文外，还负责从消息队列中清除WM_PAINT消息。因此，在处理窗口重画时，必须使用CPaintDC，否则 WM_PAINT消息无法从消息队列中清除，将引起不断的窗口重画。
(3)CPaintDC也只能用在WM_PAINT消息处理之中。
2.CClientDC（客户区设备上下文）: 处理显示器描述表的相关的窗体客户区域。
用于客户区的输出，与特定窗口关联，可以让开发者访问目标窗口中客户区，其构造函数中包含了GetDC,析构函数中包含了ReleaseDC。
3.CWindowDC: 处理显示器描述表相关的整个窗体区域，包括了框架和控 件（子窗体）。
(1)可在非客户区绘制图形，而CClientDC，CPaintDC只能在客户区绘制图形。
(2)坐标原点是在屏幕的左上角，CClientDC，CPaintDC下坐标原点是在客户区的左上角。
(3)关联一特定窗口，允许开发者在目标窗口的任何一部分进行绘图，包含边界与标题，这种DC同WM_NCPAINT消息一起发送。
4.CMetaFileDC: 与元文件相关的设备描述表关联。
CDC提供两个函数，GetLayout和SetLayout用于反转设备描述表的布局。用于方便阿拉伯，希伯来的书写文化习惯的设计，以及非欧洲表中的字体布局。
CDC包含两个设备描述表，m_hDC和m_hAttribDC对应于相同的设备，CDC为m_hDC指定所有的输出GDI调用，大多数的GDI属性调用由m_hAttribDC控制。（如,GetTextColor是属性调用，而SetTextColor是一种输出调用。）
下面用一些简单的代码看看如果使用这些类
HDC使用, 每次画线等操作都比MFC封装的类多了个HDC的参数
执行在哪个设备描述表操作
HDC hdc=::GetDC(m_hWnd);//m_hWnd == this->m_hWnd 即当前窗口句柄
MoveToEx(hdc,m_ptOrigin.x,m_ptOrigin.y,NULL);
LineTo(hdc,point.x,point.y);
::ReleaseDC(m_hWnd,hdc);//必须和GetDC配对
可以看到HDC的使用较麻烦, 而且如果::GetDC和::ReleaseDC不配对的话,会造成错误
CDC *pDC=GetDC();
pDC->MoveTo(m_ptOrigin);
pDC->LineTo(point);
ReleaseDC(pDC);
CClientDC dc(this);
dc.MoveTo(m_ptOrigin);
dc.LineTo(point);
CWindowDC dc(this);
CWindowDC dc2(GetDesktopWindow());//获得整个桌面的句柄, 一些桌面特效程序使用
dc.MoveTo(m_ptOrigin);
dc.LineTo(point);
CPaintDC dc(this);
dc.MoveTo(m_ptOrigin);
dc.LineTo(point);
可以看到 MFC 的类使用方便很多, 因为它们都在构造函数和析构函数调用了响应的函数进行DC的获取和释放.
下面说下一些细点的知识点
CClientDC,CWindowDC 区别不大, 可以说 CWindowDC包含了CClientDC 就拿记事本来说
CClientDC 就只是白白的我们可以编辑文字的那个区域，是客户区
CWindowDC 除了上面说的白白区域, 还包括菜单栏和工具栏等
CClientDC和CWindowDC 与 CPaintDC 的区别大点
在DC的获取方面 CClientDC和CWindowDC 使用的是并只能是 GetDC 和 ReleaseDC
CPaintDC 使用的是并只能是 BeginPaint 和 EndPaint
CPaintDC 只能用在响应 WM_PAINT 事件
CClientDC,CWindowDC 只能用在响应非WM_PAINT 事件
关于 WM_PAINT 事件
系统会在多个不同的时机发送WM_PAINT消息：当第一次创建一个窗口时，当改变窗口的大小时，当把窗口从另一个窗口背后移出时，当最大化或最小化窗口时，等等，这些动作都是由系统管理的，应用只是被动地接收该消息，在消息处理函数中进行绘制操作；大多数的时候应用也需要能够主动引发窗口中的绘制操作，比如当窗口显示的数据改变的时候，这一般是通过InvalidateRect和InvalidateRgn函数来完成的。InvalidateRect和 InvalidateRgn把指定的区域加到窗口的Update Region中，当应用的消息队列没有其他消息时，如果窗口的Update
 Region不为空时，系统就会自动产生WM_PAINT消息。
系统为什么不在调用Invalidate时发送WM_PAINT消息呢？又为什么非要等应用消息队列为空时才发送WM_PAINT消息呢？这是因为系统把在窗口中的绘制操作当作一种低优先级的操作，于是尽可能地推后做。不过这样也有利于提高绘制的效率：两个WM_PAINT消息之间通过 InvalidateRect和InvaliateRgn使之失效的区域就会被累加起来，然后在一个WM_PAINT消息中一次得到更新，不仅能避免多次重复地更新同一区域，也优化了应用的更新操作。像这种通过InvalidateRect和InvalidateRgn来使窗口区域无效，依赖于系统在合适的时机发送WM_PAINT消息的机制实际上是一种异步工作方式，也就是说，在无效化窗口区域和发送WM_PAINT消息之间是有延迟的；有时候这种延迟并不是我们希望的，这时我们当然可以在无效化窗口区域后利用SendMessage
 发送一条WM_PAINT消息来强制立即重画，但不如使用Windows GDI为我们提供的更方便和强大的函数：UpdateWindow和RedrawWindow。UpdateWindow会检查窗口的Update Region，当其不为空时才发送WM_PAINT消息；RedrawWindow则给我们更多的控制：是否重画非客户区和背景，是否总是发送 WM_PAINT消息而不管Update Region是否为空等。
相互转换：
方法一: 此方法在设备结束时不会销毁原来的资源(即:hDC,hBitmap)
CDC *pDC = CDC::FromHandle(hDC);
方法二: 此方法在设备结束时会销毁原来的资源(即:hDC,hBitmap)
CDC dc;
dc.Attach(hDC);
关于HDC CDC
首先：CDC 不可以释放. FromHandle 是通过 HDC 来创建了一个CDC 对象,以方便操作,释放 DC 的操作应该针对于HDC 而非此CDC , 如果释放了它   pDC->ReleaseDC,就会造成隐患. 
  HDC   hDC   =   GetDC(hWnd);  
  CDC   *pDC   =   CDC::FromHandle(hDC);   
这两者是指向的一个DC对象,只能释放一次,而这个释放应该针对 hDC 而非 pDC.   
然后：GetDC和ReleaseDC的调用匹配，CreateDC和DeleteDC的调用匹配。
GetDC是从窗口获取現有的DC，而CreateDC是创建DC，所以ReleaseDC和DeleteDC的作用一个是释放，一个是销毁。
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/loop_k/archive/2009/08/24/4476797.aspx](http://blog.csdn.net/loop_k/archive/2009/08/24/4476797.aspx)
