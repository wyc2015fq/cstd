# Invalidate 窗口区无效 InvalidateRect - xqhrs232的专栏 - CSDN博客
2012年09月18日 14:43:53[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1483
个人分类：[C/C++/VC++/VS](https://blog.csdn.net/xqhrs232/article/category/906934)
原文地址::[http://blog.sina.com.cn/s/blog_4ad042e50102e3a3.html](http://blog.sina.com.cn/s/blog_4ad042e50102e3a3.html)
窗口区无效是什么意思?
无效就是被其它程序盖住了，盖它的程序关了，它又要显示出来，所以这个函数，可以发消息给被盖住的程序再重新绘制自己。
通俗的说就是擦除你在窗口上画的图，让窗口重新调用OnDraw函数。
InvalidateRect只是增加重绘区域，在下次WM_PAINT的时候才生效。
InvalidateRect函数中的参数TRUE表示系统会在你画之前用背景色将所选区域覆盖一次，默认背景色为白色，可以通过设置BRUSH来改变背景色。
Invalidate()之后：
...OnPaint()->OnPrepareDC()->OnDraw()
所以只是刷新在OnPaint()和OnDraw()函数中的绘图语句。其它地方没有影响。
Invalidate标记一个需要重绘的无效区域，并不意味着调用该函数后就立刻进行重绘。类似于PostMessage(WM_PAINT)，需要处理到WM_PAINT消息时才真正重绘。以为您Invalidate之后还有其他的语句正在执行，程序没有机会去处理WM_PAINT消息，但当函数执行完毕后，消息处理才得以进行。
Invalidate只是放一个WM_PAINT消息在队列里，不做别的，所以只有当当前函数返回后，进入消息循环，取出WM_PAINT，才执行PAINT，所以不管Invalidate放哪里，都是最后的。
具体例子给你举个吧：
我做过一个游戏，要在屏幕上不断刷新图片。比如任务移动，每一下键盘按下就要判断，然后重绘地图。是就是用到Invalidate()，然后我在OnPaint（）中实现了重绘，而且Invalidate()我放到了键盘处理函数中，每有键盘按下，就会调用Invalidate()，接着调用OnPaint（），实现重绘地图。懂了不？再告诉你一点，你最好要用双缓冲，这样不会出现屏幕闪烁。呵呵！
___________________________________________________________________
网上列子说，在重载对话框onPaint()进行绘图时，需要在绘图的那些函数之前写
Invalidate();
UpdateWindow();
这里我搞不懂：onPaint()的调用，是Invalidate();的结果，为何还需要再次调用Invalidate();？
还有一个问题：我发现，子控件（比如位图图片框）的无效，并不会引起父窗体的无效，所以调用子控件Invalidate();并不会引起重绘。我说的对么？
问题补充：
必须在父窗口的onpaint中指定子控件的无效区域，并立即重绘，才能使得子控件得以重绘。说半天也没人看懂，写代码吧：父窗口：onpaint(){picbox.Invalidate(); picbox.UpdateWindow();picbox.GetDC->bitblt(......).......(均为在图片框上绘图的函数)}如果不加picbox.Invalidate(); picbox.UpdateWindow();强制重绘是无效果的。（试验已证实）回应蔡文碧：WS_CLIPCHILDREN 是说：当在父窗口内绘图时,排除子窗口区域。而我这是在子窗口上绘图。不过我依然在父窗口的PreCreateWindow（）函数中使用了cs.style
 &= ~WS_CLIPCHILDREN; 可是无论是在子窗口或是父窗口上绘图，在子窗口区域，都无法绘显示，（在父窗口上绘图，非子窗口区域可以显示）。我就纳闷了。。。我猜测，可能在onPaint()中，所有子窗口是最后才绘制上去的。我十分想知道，onpaint的详细的，具体的绘制流程。另外补充一下：对话框的Clip siblings和Clip children都没有的勾上的，也就是说,实际上不存在WS_CLIPCHILDREN的问题.可能是我搞错了？能说下"在对话框的位图图片框(子窗口)中绘图"的具体的过程么？
onPaint就是响应消息WM_PAINT,
不要在onPaint里面调用Invalidate, Invalidate是强制刷新显示.
网上的资料可能是说的, 如果需要刷新时, 在其他地方调用Invalidate, 这样OnPaint函数就会被执
子窗口的绘制代码应该放在子窗口自己的OnPaint过程里面，干啥要放在父窗口过程里面？----不知道你的图片框是不是CStatic，如果是要这么画：
1）给static控件增加SS_OWNERDRAW属性
2）给父窗口增加ON_WM_DRAWITEM消息映射函数
3）在OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)消息映射函数里面绘制控件
例如：下面给对话框里的IDC_STATIC_DRAW控件画个蓝色的背景
void CMyDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
 if(nIDCtl == IDC_STATIC_DRAW)
{ CDC *pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
pDC->FillSolidRect(10, 10, lpDrawItemStruct->rcItem.right - 20, lpDrawItemStruct->rcItem.bottom - 20, RGB(0, 0, 255));
return;
}
CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
另外，还可以这么画从CStatic继承一个新类，然后增加WM_PAINT消息处理，在OnPaint里面画
例如：class CMyStatic : public CStatic
{...afx_msg void OnPaint();};
void CMyStatic::OnPaint()
{CPaintDC dc(this); // device context for painting
CRect rc;
GetClientRect(rc);
dc.FillSolidRect(rc, RGB(0, 0, 255));
}
__________________________________________________________________________
1.
Invalidate在消息队列中加入一条WM_PAINT消息，其无效区为整个客户区。
2.
UpdateWindow直接发送一个WM_PAINT消息，其无效区范围就是消息队列中WM_PAINT消息（最多只有一条）的无效区。
3.
效果很明显，当调用Invalidate之后，屏幕不一定马上更新，因为WM_PAINT消息不一定在队列头部，而调用UpdateWindow会使WM_PAINT消息马上执行的，绕过了消息队列。
4.
如果调用Invalidate之后想马上更新屏幕，那就加上UpdateWindow()这条语句。
MSDN的解释
UpdateWindow
The UpdateWindow function updates the client area of the specified window by sending a WM_PAINT
message to the window if the window's update region is not empty. The function sends a WM_PAINT
 message directly to the window procedure of the specified window, bypassing the application queue.
If the update region is empty, no message is sent. 
InvalidateRect
The system sends a WM_PAINT message to a window whenever its update region is not empty and
 there are no other messages in the application queue for that window. 
翻译成中文大概的解释如下：
  UpdateWindow：如果有无效区，则马上sending a WM_PAINT message到窗口处理过程，不进消息队列进行排队等待，立即刷新窗口，否则，什么都不做。
 InvalidateRect：设置无效区，如果为NULL参数，则设置整个窗口为无效区。当应用程序的那个窗口的消息队列为空时，则sending a WM_PAINT message(即使更新区域为空).在sending a WM_PAINT message的所有InvalidateRect的更新区域会累加。
 1:设置无效区
 InvalidateRect
 2：立即刷新
 UpdateWindow();
如果不调用 InvalidateRect就调用 UpdateWindow，那么UpdateWindow什么都不做。 ??????
如果调用 InvalidateRect 后不调用UpdateWindow，则系统会自动在窗口消息队列为空的时候，系统自动发送一WM_PAINT消息。
调用UpdateWindow()时将会发送一个WM_PAINT消息，而应用程序在接收到WM_PAINT消息后，将自动地调用Invalidate(),所以，在程序代码中，不一定要出现Invalidate()!
UpdateWindow()就是立即发送WM_PAINT消息,只对声明无效的区域起作用，  
  Invalidate()则是声明无效的方式之一。
Invalidate()表示客户区域无效，在下次WM_PAINT发生时重绘。而WM_PAINT是由系统进行维护的，每当CWnd的更新区域不为空，并且在应用程序的窗口消息队列中没有其它消息时，Windows就发送一条WM_PAINT消息。  
  Invalidate里面有个bool型的参数，用来标识重绘的时候是否用背景色填充。是不是用SetBkcolor函数？下去继续研究。
 updateWindow则是要求系统对区域进行立即重绘。
 看到有人在网上提出问题，他在Invalidate后面又写了绘图的函数但是没有执行，因为invalidate执行过以后转到PAINT命令了。所以后面的都没有显示。
 也终于想通我绘的图一直在闪啊闪，因为我在PAINT里面用到Invalidate()函数，所以他不停的自嵌套，倒是绘的图不停的闪。
Invalidate让客户区处于可以重画的状态，而UpdateWindow开始重画，但是它先判断客户区是否为空，不空UpdateWindow不执行，为空才执行重画。
Invalidat最后也是调用InvalidatRect,在windows API里只有InvalidatRect的
___________________________________________________________________
BOOL InvalidateRect(
  HWND hWnd，           // 窗口句柄
  CONST RECT* lpRect,   // 矩形区域
  BOOL bErase            //是否擦除背景
);
InvalidateRect只是增加重绘区域，在下次WM_PAINT的时候才生效。InvalidateRect函数中的参数TRUE表示系统会在你画之前用背景色将所选区域覆盖一次，默认背景色为白色，可以通过设置BRUSH来改变背景色。
void Invalidate( BOOL bErase )执行之后调用函数的次序为：
...OnPaint()->OnPrepareDC()->OnDraw()
所以只是刷新在OnPaint()和OnDraw()函数中的绘图语句。其它地方没有影响。
Invalidate()标记一个需要重绘的无效区域，并不意味着调用该函数后就立刻进行重绘。类似于PostMessage(WM_PAINT)，需要处理到WM_PAINT消息时才真正重绘。如果您Invalidate()之后还有其他的语句正在执行，程序没有机会去处理WM_PAINT消息，只有当函数执行完毕后，消息处理才得以进行。
Invalidate()只是放一个WM_PAINT消息在队列里，不做别的，所以只有当当前函数返回后，进入消息循环，取出WM_PAINT，才执行OnPaint()，所以不管Invalidate放哪里，都是最后执行的。
InvalidateRect(hWnd,&rect,TRUE);向hWnd窗体发出WM_PAINT的消息，强制客户区域重绘制。rect是你指定要刷新的区域，此区域外的客户区域不被重绘，这样防止客户区域的一个局部的改动，而导致整个客户区域重绘而导致闪烁，如果最后的参数为TRUE，则还向窗体发送WM_ERASEBKGND消息，在客户区域重绘之前先重绘背景。
UpdateWindow只向窗体发送WM_PAINT消息，在发送之前判断GetUpdateRect(hWnd,NULL,TRUE)看有无可绘制的客户区域，如果没有，则不发送WM_PAINT。
如果希望立即刷新无效区域，可以在调用InvalidateRect之后调用UpdateWindow，如果客户区的任一部分无效，则UpdateWindow将导致Windows用WM_PAINT消息调用窗口过程（如果整个客户区有效，则不调用窗口过程）。这一WM_PAINT消息不进入消息队列，直接由WINDOWS调用窗口过程。窗口过程完成刷新以后立刻退出，WINDOWS将控制返回给程序中UpdateWindow调用之后的语句。
UpdateData()这个函数不是刷新界面用的。
UpdateData();参数为FALSE时，将界面上控件绑定的变量的数据导到控件内，参数为TRUE时，导入方向则相反。
