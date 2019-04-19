# WM_ERASEBKGND和WM_PAINT的深刻理解 - xqhrs232的专栏 - CSDN博客
2012年06月21日 11:19:05[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：866标签：[windows																[callback																[command																[mfc																[测试																[menu](https://so.csdn.net/so/search/s.do?q=menu&t=blog)](https://so.csdn.net/so/search/s.do?q=测试&t=blog)](https://so.csdn.net/so/search/s.do?q=mfc&t=blog)](https://so.csdn.net/so/search/s.do?q=command&t=blog)](https://so.csdn.net/so/search/s.do?q=callback&t=blog)](https://so.csdn.net/so/search/s.do?q=windows&t=blog)
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.csdn.net/oraclej/article/details/7576388](http://blog.csdn.net/oraclej/article/details/7576388)
一直以来，对于WM_PAINT和WM_ERASEBKGND消息不是很清楚，从书上和网上找了很多资料，大体上有以下几点说法：
1>WM_PAINT先产生，WM_ERASEBKGND后产生
2.WM_PAINT产生后，在调用BeginPaint时
hdc = BeginPaint(hWnd, &ps); 
如果ps.fErase为true，则BeginPaint会产生WM_ERASEBKGND消息
3.BeginPaint函数用来擦除窗口背景
4.WM_ERASEBKGND用来绘制背景
经过调试、分析，发现上面的说法并不正确。以下是一些测试代码，代码后面附上一些分析。最后总结出几点，可以解释程序中出现的所有关于窗口重绘的问题。
如有不正确的地方，大家可以指正。
为了说明问题，在此不说WM_NCPAINT消息（非客户区消息），只说WM_ERASEBKGND消息和客户区的WM_PAINT消息
//此段代码摘自vc6应用程序向导自动生成的代码，并添加了一些测试代码
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 int wmId, wmEvent;
 PAINTSTRUCT ps;
 HDC hdc;
 TCHAR szHello[MAX_LOADSTRING];
 LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);
 switch (message) 
 {
 case WM_COMMAND:
  wmId    = LOWORD(wParam); 
  wmEvent = HIWORD(wParam); 
  // Parse the menu selections:
  switch (wmId)
  {
  case IDM_ABOUT:
   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
   break;
  case IDM_EXIT:
   DestroyWindow(hWnd);
   break;
  default:
   return DefWindowProc(hWnd, message, wParam, lParam);
  }
  break;
  case WM_ERASEBKGND: //如果处理了这个消息，则默认消息处理函数不会调用，背景就不会绘制
   {
    static int iCount=0;
    char ch[MAX_PATH];
    sprintf(ch,"%d ---------WM_ERASEBKGND\n",iCount); //这个函数需要包含#include<stdio.h>
    OutputDebugString(ch); //调试时便于观察
    iCount++;
   break;
   }
  case WM_PAINT:
   {
    OutputDebugString("  -------------WM_PAINT\n");
    hdc = BeginPaint(hWnd, &ps); //使无效区域变得有效，并填充ps结构
    // TODO: Add any drawing code here...
    //绘制一个蓝色椭圆，ps.rcPaint保存了客户区矩形
    HBRUSH hbrush=::CreateSolidBrush(RGB(0,0,255)); 
    ::SelectObject(hdc,hbrush);
    ::Ellipse(hdc,ps.rcPaint.left,ps.rcPaint.top,ps.rcPaint.right,ps.rcPaint.bottom);
    ::DeleteObject(hbrush);
    EndPaint(hWnd, &ps);
    break;
   }
  case WM_LBUTTONDOWN: //调用DefWindowProc擦除客户区背景
   {
    HDC hdc;
    hdc=::GetDC(hWnd);
    WPARAM w=(WPARAM)hdc;
    LPARAM l=0;
    DefWindowProc(hWnd, WM_ERASEBKGND, w, l);
   }
   break;
  case WM_DESTROY:
   PostQuitMessage(0);
   break;
  default:
   return DefWindowProc(hWnd, message, wParam, lParam);
 }
 return 0;
}
先说一下程序运行时发现的一些现象：
1.
上面的代码：如果添加了WM_ERASEBKGND消息，里面什么也不做，如下
case WM_ERASEBKGND:
  break;
则当程序运行时，如果收到WM_ERASEBKGND消息，则这个switch-case结构中就不会执行默认消息处理函数DefWindowProc，运行时发现，窗口的背景就没有了，即背景为空。
这说明了窗口背景仅仅是由默认的消息处理函数DefWindowProc绘制的。
（注：注册窗口类时，背景设置为白色wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);）
2.
如上面的代码，因为有WM_ERASEBKGND消息，则程序运行时窗口背景为NULL,
但是如果添加了WM_LBUTTONDOWN消息，从里面调用默认窗口消息处理函数，如下
case WM_LBUTTONDOWN:
  {
   HDC hdc;
   hdc=::GetDC(hWnd);
   WPARAM w=(WPARAM)hdc; //变量w作为WM_ERASEBKGND消息的wParam参数，保存了设备环境句柄
   LPARAM l=0;
   DefWindowProc(hWnd, WM_ERASEBKGND, w, l); //调用默认消息处理函数DefWindowProc
  }
  break;
程序运行时，如果用鼠标单击一下窗口客户区，则窗口的背景就会显示！这进一步说明了窗口的背景色是由默认消息处理函数DefWindowProc绘制的。
3.
以上代码，因为添加了WM_ERASEBKGND消息，所以窗口背景是空。
虽然在WM_PAINT消息中有
hdc = BeginPaint(hWnd, &ps);函数的调用，但是窗口背景仍然是空，这说明了BeginPaint函数并不会擦除背景（即用默认画刷绘制窗口背景）。
BeginPaint函数只做了两件事情：
1》使窗口无效区域变得有效，从而使Windows不再发送WM_PAINT消息（直到窗口大小改变等，使窗口再次变得无效）。
（如果窗口一直无效，则Windows会不停地发送WM_PAINT消息）
1》填充PAINTSTRUCT结构。填充这个结构的目的，是让程序员可以根据ps变量中的标志值进行某些操作
4.
调试的时候，发现：当窗口改变大小，或者其它操作使窗口变得无效时，WM_ERASEBKGND消息总是先于WM_PAINT消息发出，而且如果产生WM_ERASEBKGND消息，
则最后一个WM_ERASEBKGND的下一条消息一定是WM_PAINT消息（WM_ERASEBKGND可能会连续产生几次）。WM_ERASEBKGND消息和WM_PAINT消息之间没有其它消息
--------------------------------------------
以下是一些总结
1.窗口背景的擦除（即绘制）
窗口的背景色是由默认的消息处理函数DefWindowProc擦除的（即这个函数使用注册窗口类时使用的背景刷擦除窗口背景）。
什么时候绘制？在窗口函数收到WM_ERASEBKGND消息，DefWindowProc函数以WM_ERASEBKGND为参数，才会绘制窗口背景
（注：当WM_ERASEBKGND消息产生后，窗口一定有一部分变得无效）
2.窗口的无效：
当拖动窗口的一个顶点改变了窗口的大小、窗口由最小化恢复到最大化、窗口的一部分被其它窗口遮住又重新显示、调用MoveWindow函数改变了窗口大小、窗口移动到桌面之外的
部分被拖回重新显示时，窗口就会变得无效。 无效区域是整个客户区，因此默认窗口处理函数DefWindowProc会擦除整个客户区。
（注：拖动窗口标题栏移动窗口，只要窗口没有移动到屏幕之外，那么这两个消息都不产生）
当窗口无效时，Windows会给窗口发出WM_ERASEBKGND消息和WM_PAINT消息，而且WM_ERASEBKGND先发出一次或者几次，紧接着是WM_PAINT
例外:InvalidateRect函数的调用会使窗口变得无效，并产生WM_ERASEBKGND消息和WM_PAINT消息，而WM_ERASEBKGND是否产生取决于参数bErase
void InvalidateRect ( 
LPCRECT lpRect, 
BOOL bErase = TRUE ); 
当参数bErase为true时，WM_ERASEBKGND消息产生，当bErase为false时WM_ERASEBKGND消息不产生
3.消息的处理过程
当窗口无效时，
先发出WM_ERASEBKGND消息若干次-----------再发出WM_PAINT消息，WM_ERASEBKGND和WM_PAINT之间没有其它消息
WM_ERASEBKGND消息的后面一定是WM_PAINT
1》WM_ERASEBKGND消息的处理：
上面的代码，如果没有添加WM_ERASEBKGND,则默认的消息处理函数DefWindowProc会被调用，此时的DefWindowProc会擦除窗口背景（即绘制背景），并且ps.fErase会为FALSE
如果添加了WM_ERASEBKGND消息，DefWindowProc就不会被调用，则无法擦除窗口背景,并且ps.fErase会为true
2》WM_PAINT的处理
在这个消息中如果调用了hdc = BeginPaint(hWnd, &ps);函数，则此函数只做了两件事：填充ps结构、使窗口重新变得有效
另外DefWindowProc函数也会使窗口变得有效
关于ps.fErase;
这个参数和窗口函数WndProc的返回值有关：
当窗口函数WndProc返回true;则产生WM_PAINT消息时，ps.fErase就为false;表明系统擦除了背景
当窗口函数WndProc返回false;则产生WM_PAINT消息时，ps.fErase就为true;表明系统没有擦除背景
设想一下，当上面的代码中添加了WM_ERASEBKGND消息并在其中直接返回true（这表明系统已经绘制了窗口背景），则ps.fErase就为false
case WM_ERASEBKGND: 
return true; //窗口函数WndProc返回true;
注意返回的真或者假只是让程序员可以看见ps.fErase，并作出自己的代码，与窗口的显示即背景没有关系
有些人说当ps.fErase==true，BeginPaint函数会发送一个WM_ERASEBKGND消息，其实BeginPaint并未发出WM_ERASEBKGND消息
4.自己绘制背景或者系统绘制背景。
如果程序员不想系统擦除背景，而自己想绘制背景，怎么办呢？方法是在WM_ERASEBKGND消息处理中添加自己的绘制代码。
对于WM_ERASEBKGND消息，wParam参数保存了用于绘制的设备环境，lParam不使用。
如上面的示例代码，当添加了WM_ERASEBKGND消息，则switch---case中就不会调用DefWindowProc函数绘制背景。这时，程序员自己就可以添加绘制代码
而在基于MFC的程序中，是这样处理自绘代码的：
BOOL CCeDlg::OnEraseBkgnd(CDC* pDC) //这个函数就是WM_ERASEBKGND的消息处理函数
{
 // TODO: Add your message handler code here and/or call default
//添加自绘代码
...
return TRUE; //返回真，代表着窗口函数的返回值。以便于程序员在WM_PAINT消息中作出相应处理（如果需要）。这里返回时就不会调用下面的默认处理
//下面将调用系统默认的消息处理函数DefWindowProc进行背景的默认绘制。
 return CDialog::OnEraseBkgnd(pDC); //不执行自动生成的这个函数
}
执行这个函数时，提示用户绘制背景，如果用户没有绘制背景，则return CDialog::OnEraseBkgnd(pDC);调用默认的窗口处理函数进行背景的擦除
BOOL CCeDlg::OnEraseBkgnd(CDC* pDC) 
{
 // TODO: Add your message handler code here and/or call default
//添加自绘背景代码
 CBitmap m_bitmap;
 BITMAP m_bmInfo;
 m_bitmap.LoadBitmap(IDB_BITMAP1);
 m_bitmap.GetObject(sizeof(m_bmInfo),&m_bmInfo);
 CDC memDC;
 memDC.CreateCompatibleDC(pDC);
 memDC.SelectObject(&m_bitmap);
 GetClientRect(m_rect);
 pDC->StretchBlt(0,0,m_rect.Width(),m_rect.Height(),
&memDC,0,0,m_bmInfo.bmWidth,m_bmInfo.bmHeight,SRCCOPY); //内存拷贝函数。绘制背景
 memDC.DeleteDC();
 return true;//返回真，代表着窗口函数的返回值。以便于程序员在WM_PAINT消息中作出相应处理（如果需要）。这里返回时就不会调用下面的默认处理
//下面将调用系统默认的消息处理函数DefWindowProc进行背景的默认绘制。
 return CDialog::OnEraseBkgnd(pDC); //不执行自动生成的这个函数
}
5.WM_ERASEBKGND消息和WM_PAINT消息的另外一种含义：背景色与前景色
可以这样理解WM_ERASEBKGND消息和WM_PAINT消息：
WM_ERASEBKGND消息用于通知系统或者程序员绘制背景色
WM_PAINT消息用于通知程序员绘制前景色，比如在WM_PAINT中调用TextOut函数输出文本
