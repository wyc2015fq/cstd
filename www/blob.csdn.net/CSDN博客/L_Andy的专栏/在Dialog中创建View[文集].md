# 在Dialog中创建View[文集] - L_Andy的专栏 - CSDN博客

2018年02月28日 18:24:04[卡哥](https://me.csdn.net/L_Andy)阅读数：362


共5篇文章： 

《略谈如何在对话框创建视图类画图》 

《An alternative to the often asked view-in-dialog problem using a dialog bar》 

《Creating a View in Dialog (An easy way). 》 

《如何在没有文档的情况下使用CView及其派生类》 

《浅谈：切换视时基于FormView的对话框属性设置与ASSERT报错的问题》 
**略谈如何在对话框创建视图类画图**

在对话框上画图可以通过添加控件来进行，但这种画图有一个严重弊端就是画图范围受控件范围控制。最近做一个程序，需要一个数据报告窗口，因为输出的内容比较多，格式不一致（涉及多个表，但每个表的数据要严格对齐），所以如ListControl等控件并不适合。所以我想到在对话框上创建视图类上进行数据显示。

一是视图窗口如何动态创建。首先是视图窗口的定位。一般的动态创建窗口定位窗口的位置不太容易。我从网上找到的一个好办法在对话框上加一个静态文本控件，然后把视图创建在该控件之上。这个问题想好，创建就基本完成了。

创建的具体步骤如下： 

1.     定义一个派生自CScrollView类的视图类CDrawView（至于为什么不是CView而是CScrollView，原因我会在下面谈）。
- 在对话框类上定义一个CDrawView类指针*m_pDrawView 

具体代码如何：

```
BOOL CStaticDataReport::OnInitDialog()
{
 CDialog::OnInitDialog();
 UINT TargetCtrlID = IDC_STATIC;
 CWnd *pWnd = this->GetDlgItem(TargetCtrlID);
 CRect RectTargetCtrl;
 pWnd->GetWindowRect(RectTargetCtrl);
 pWnd->DestroyWindow();
 this->ScreenToClient(RectTargetCtrl);
 //在目标位置动态创建CScrollView
 if (NULL==m_pDrawView)
 {
  return FALSE;
 }
m_pDrawView = (CDrawView*)RUNTIME_CLASS(CDrawView)->CreateObject();
 m_pDrawView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW|WS_VSCROLL|WS_HSCROLL, RectTargetCtrl, this, TargetCtrlID);
 m_pDrawView->ShowWindow(SW_SHOW);
 return TRUE;
}
```

二．前面我已经谈到了在对话框上绘图的一个弊端是绘图范围受控件范围所谓，一不小心就会出界，如下图所示：![这里写图片描述](https://img-blog.csdn.net/20180228175519022?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

使用视图类的好处是你可以使用滚动条扩大绘图范围，这也是我为何将自定义视图类继承CScrollView类的原因。

实际上滚动条的处理也是不太容易的，主要是滚动条添加后如何重绘新的显示范围比较麻烦。为此我重翻petzod的名著《Windows程序设计》的里面一节：建立更好的滚动。将里面的Win API代码改为MFC实现。因为我的程序是输出文字的，我就以如何在文字输出视图窗口创建滚动条。

首先要创建窗口的滚动条，你就必须在动态创建是指定两种窗口风格：WS_VSCROLL|WS_HSCROLL。

其次你需要指定窗口的滚动范围，具体就是滚动的最大高度和宽度。我的做法是在视图类定义两个变量：

```
int m_iMaxWidth;    // 滚动的的最大宽度
    int m_MaxNumLines;  // 滚动的最大高度
```

这个你可以自定义滚动范围。

其次你还要定义一些文字大小的相关变量：

```
int m_cxChar;
    int m_cxCaps;
    int m_cyChar;
```

在OnCreate函数（）（WM_CREATE消息映射函数）获取字体大小，代码如下：

```
int CDrawView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
 if (CScrollView::OnCreate(lpCreateStruct) == -1)
  return -1;
 // TODO: Add your specialized creation code here
 CDC *pDC = GetDC();
 TEXTMETRIC  tm ;
 pDC->GetTextMetrics(&tm) ;
 m_cxChar = tm.tmAveCharWidth ;
 m_cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * m_cxChar / 2 ;
 m_cyChar = tm.tmHeight + tm.tmExternalLeading ;
    ReleaseDC(pDC);
 return 0;
}
```

在OnSize函数（WM_SIZE消息映射函数）设置滚动范围：

```
void CDrawView::OnSize(UINT nType, int cx, int cy)
{
 SCROLLINFO  si ;
 si.cbSize     = sizeof (si) ;
 si.fMask      = SIF_RANGE | SIF_PAGE ;
 si.nMin       = 0 ;
 si.nMax       = m_MaxNumLines - 1 ;
 si.nPage      = cy/m_cyChar ;
 SetScrollInfo (SB_VERT, &si, TRUE) ;
 // Set horizontal scroll bar range and page size
 si.cbSize     = sizeof (si) ;
 si.fMask      = SIF_RANGE | SIF_PAGE ;
 si.nMin       = 0 ;
 si.nMax  = m_iMaxWidth;
 si.nPage      = cx/m_cxChar ;
 SetScrollInfo (SB_HORZ, &si, TRUE) ;
}
```

分别响应WM_VSCROLL消息和WM_HSCROLL，主要目的是设置滚定信息和决定是否要更新窗口：

```
void CDrawView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
 // TODO: Add your message handler code here and/or call default
 SCROLLINFO  si ;
 si.cbSize     = sizeof (si) ;
 si.fMask      = SIF_ALL ;
 GetScrollInfo (SB_VERT, &si) ;
 // Save the position for comparison later on
 int iVertPos = si.nPos ;
 switch (nSBCode)
 {
 case   SB_TOP:
  si.nPos       = si.nMin ;
  break ;
 case   SB_BOTTOM:
  si.nPos       = si.nMax ;
  break ;
 case SB_LINEUP:
  si.nPos -= 1 ;
  break ;
 case   SB_LINEDOWN:
  si.nPos += 1 ;
  break ;
 case   SB_PAGEUP:
  si.nPos -= si.nPage ;
  break ;
 case   SB_PAGEDOWN:
  si.nPos += si.nPage ;
  break ;
 case   SB_THUMBTRACK:
  si.nPos = si.nTrackPos ;
  break ;
 default:
  break ;      
 }
 // Set the position and then retrieve it.  Due to adjustments
 //  by Windows it may not be the same as the value set.
 si.fMask = SIF_POS ;
 SetScrollInfo(SB_VERT, &si, TRUE) ;
 GetScrollInfo(SB_VERT, &si) ;
 // If the position has changed, scroll the window and update it
 if (si.nPos != iVertPos)
 {                  
  ScrollWindow (0, m_cyChar * (iVertPos - si.nPos),NULL, NULL) ;
  UpdateWindow() ;
 }
```

```
CScrollView::OnVScroll(nSBCode, nPos, pScrollBar)void CDrawView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
 // TODO: Add your message handler code here and/or call default
 // Get the minimum and maximum scroll-bar positions.
 // Get all the vertical scroll bar information
 int minpos=0;
 int maxpos = 2 + m_iMaxWidth/m_cxChar ;
 SCROLLINFO  si ;
 si.cbSize = sizeof (si) ;
 si.fMask  = SIF_ALL ;
 // Save the position for comparison later on
 GetScrollInfo (SB_HORZ, &si) ;
 int iHorzPos = si.nPos ;
 switch (nSBCode)
 {
 case SB_LEFT:      // Scroll to far left.
  si.nPos = minpos;
  break;
 case SB_RIGHT:      // Scroll to far right.
  si.nPos = maxpos;
  break;
 case SB_ENDSCROLL:   // End scroll.
  break;
 case SB_LINELEFT:      // Scroll left.
  if (si.nPos > minpos)
   si.nPos--;
  break;
 case SB_LINERIGHT:   // Scroll right.
  if (si.nPos < maxpos)
   si.nPos++;
  break;
 case SB_PAGELEFT:    // Scroll one page left.
  {
   // Get the page size. 
   SCROLLINFO   info;
   GetScrollInfo (SIF_ALL, &info) ;
   //   pScrollBar->GetScrollInfo(&info, SIF_ALL);
   if (si.nPos > minpos)
    si.nPos = max(minpos, si.nPos - (int) info.nPage);
  }
  break;
 case SB_PAGERIGHT:      // Scroll one page right.
  {
   // Get the page size. 
   SCROLLINFO   info;
   GetScrollInfo (SIF_ALL, &info) ;
   //   pScrollBar->GetScrollInfo(&info, SIF_ALL);
   if (si.nPos < maxpos)
    si.nPos = min(maxpos, si.nPos + (int) info.nPage);
  }
  break;
 case SB_THUMBPOSITION: // Scroll to absolute position. nPos is the position
  si.nPos = nPos;      // of the scroll box at the end of the drag operation.
  break;
 case SB_THUMBTRACK:   // Drag scroll box to specified position. nPos is the
  si.nPos = nPos;     // position that the scroll box has been dragged to.
  break;
 }
 si.fMask = SIF_POS ;
 SetScrollInfo (SB_HORZ, &si, TRUE) ;
 GetScrollInfo (SB_HORZ, &si) ;
 // If the position has changed, scroll the window
 if (si.nPos != iHorzPos)
 {
ScrollWindow (m_cxChar * (iHorzPos - si.nPos), 0,NULL, NULL) ;
 }
 CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
}
```

然后在OnPaint函数（WM_PAINT消息响应函数）你就可以获取当前绘图范围了：

```
void CDrawView::OnPaint() 
{
 CPaintDC dc(this); // device context for painting
SCROLLINFO  si ;
 si.cbSize = sizeof (si) ;
 si.fMask  = SIF_POS ;
 GetScrollInfo(SB_VERT, &si) ;
 int iVertPos = si.nPos ;
 // Get horizontal scroll bar position
 GetScrollInfo (SB_HORZ, &si) ;
 int iHorzPos = si.nPos ;
 // Find painting limits
 int iPaintBeg = max(0,iVertPos + dc.m_ps.rcPaint.top /m_cyChar) ;
 int iPaintEnd = min(m_MaxNumLines - 1,iVertPos + dc.m_ps.rcPaint.bottom/m_cyChar);
 int x = 0;
 int y = 0;
 int i = 0;
 int j =0;
// iPaintBeg为绘图起始行，iPaintEnd为绘图结束行
 for (i = iPaintBeg ; i <= iPaintEnd;i++)
 {

……
}
}
```

效果图如下： 
![这里写图片描述](https://img-blog.csdn.net/20180228180034092?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

An alternative to the often asked view-in-dialog problem using a dialog bar 
[http://www.codeproject.com/KB/dialog/view_in_dialog.aspx](http://www.codeproject.com/KB/dialog/view_in_dialog.aspx)

[Download demo project - 27 Kb](https://www.codeproject.com/script/Membership/LogOn.aspx?download=true)
![这里写图片描述](https://img-blog.csdn.net/20180228180549422?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Introduction 

I tried to create a View on a Dialog, and thus looked up all the articles on the web. There are several attempts to do this, but more or less all of them produce errors and aren’t too useful in  my opinion. So I decided to go a different way. I created a new Window type (called CDialogWindow in my demo project) by deriving the class from CFrameWnd. In this CDialogWindow I created a View using Jorge Vigils code from [http://www.codeguru.com/doc_view/ReplacingView.shtml](http://www.codeguru.com/doc_view/ReplacingView.shtml) (I modified it of course, but am nonetheless grateful to Jorge). This View was a CScrollView which was my main intention, and perhaps this is also yours. Just imagine a task like displaying a bitmap on a dialog in full-size, which doesn’t fit in the Dialog form. My own task was to display a video image generated by an ActiveX-Control. Instead of really displaying this image in a dialog, I created the just mentioned view in the CDialogWindow and added a CDialogBar to this window. The result looks nearly like a dialog containing a view.

But unfortunately I have to deal with an unexpected disadvantage using this simple technique. As the CDialogBar isn’t connected with an own class but with the CDialogWindow Class, it is not possible to connect variables with the controls. This means of course, that all the advanced controls like spin buttons, scroll bars etc. which need to be represented by a variable of “Control” type, cannot be used. The functionality of the Window is reduced to input and output in text fields using the methods GetDlgItemInt(), GetDlgItemText(), SetDlgItemInt(), SetDlgItemText(). Perhaps someone can improve my project and tell me how to use all sort of controls in such a window.

Usage 

As you can see in the demo project, I have added my CDialogWindow to an MDI project, not using any of the classes created by the application wizard. So if you take my project as a start point, you still have all the functionality of an MDI-app. Mainly I haven’t used the generated view class but a class which I imported from another project, because this class was good for my purpose. Just to show you how the window works, I have replaced my video image by a simple text. What does this mean for you, if you want to copy some of my code:

Copy the CDialogWindow class to your project and create an object with the DoModeless() method as you can see it in the project. 

If you also want to display anything but not a document, also copy the CExtraView class and modify the OnDraw() method in order to make it display your stuff. 

If you want to create a view of a document or create a view from your own view class, just replace the class name in the RUNTIME_CLASS Macro in CDialogWindow::OnCreateClient() and don’t forget to declare CVideoWindow friend of your own view class. Of course you will have to do some other little modifications like including the header files and so on. 

Conclusion 

Pleas write me an email if you could improve my classes, especially, if you achieve to use all sort of controls in the dialog bar. I’m sorry, but I generated the demo project using the German version of the SDK. Thus the auto-generated comments are in German. This should not prevent you from understanding the code, as all pieces of my own code are commented in English.

License 

This article has no explicit license attached to it but may contain usage terms in the article text or the download files themselves. If in doubt please contact the author via the discussion board below.

A list of licenses authors might use can be found here

About the Author 

thom_as

Member  Thomas Schuster, studying Physics in Ulm / Germany and therefore programming Visual C++. 

Occupation: Web Developer 

Location:   Germany Germany

Creating a View in Dialog (An easy way).  
[http://www.codeguru.com/Cpp/W-D/dislog/article.php/c5009/](http://www.codeguru.com/Cpp/W-D/dislog/article.php/c5009/)

![这里写图片描述](https://img-blog.csdn.net/20180228181001809?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTF9BbmR5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

Environment developed: VC6 Win 2K]

To create a view you normally follow the Microsoft’s Document template model. (i.e) Single document template or multi doc template. You can pass three runtime classes to the constructors:

```
CSingleDocTemplate( UINT nIDResource,
CRuntimeClass* pDocClass,
CRuntimeClass* pFrameClass,
CRuntimeClass* pViewClass );
```

OR

```
CMultiDocTemplate( UINT nIDResource,
CRuntimeClass* pDocClass,
CRuntimeClass* pFrameClass,
CRuntimeClass* pViewClass );
```

When the document template is added, frame (child for MDI , main frame for SDI), document, and a view is created dynamically and added to the application’s document template list.

But some times you may need to create a view without using the default document template classes. For instance, if you are to create a view in dialog, you can not use the regular way of document template. In those cases you can use the following method:

```java
BOOL CVwindlgDlg::OnInitDialog()
{
...
CCreateContext pContext;
/**
* Note:CDialig derived pointer is converted to
* CWnd pointer (a common base class for CDialog and CFrameWnd).
* Thus casting it back to CFrameWnd is also easy.
*/
CWnd* pFrameWnd = this;
pContext.m_pCurrentDoc = new CMyDocument;
pContext.m_pNewViewClass = RUNTIME_CLASS(CMyVw);
CMyVw *pView =
(CMyVw *) ((CFrameWnd*)pFrameWnd)->CreateView(&pContext);
ASSERT(pView);
pView->ShowWindow(SW_NORMAL);
/**
* After a view is created, resize that to
* have the same size as the dialog.
*/
CRect rectWindow;
GetWindowRect(rectWindow);
/**
* Leave a little space for border and title...
*/
rectWindow.right += 15;
rectWindow.top   -= 10;
pView->MoveWindow(rectWindow);
CString str(AfxGetApp()->m_lpCmdLine);
/**
* Note: "CMyVw" is a CHTMLView derived class to add some
*       spice to the view, I have provided a HTML page
*       to which it navigates when the dialog is up.
*/
char strPath[255];
::GetCurrentDirectory(255,(LPSTR)(LPCSTR)strPath);
strcat(strPath,"\\defaultpage.html");
pView->Navigate(strPath);
....
return TRUE;  // return TRUE  unless you set the
// focus to a control
}
```

Downloads 
[Download demo project - 6 KB](https://www.codeguru.com/code/legacy/dialog/vwindlg_exe.zip)
[Download source - 38 KB](https://www.codeguru.com/code/legacy/dialog/vwindlg.zip)

【错误】 

ASSERT(  pParentFrame == pDesktopWnd || pDesktopWnd->IsChild(pParentFrame)); 

解决方法：重载OnMouseActive()，屏蔽掉找主框架的代码，就不会出错了。但是在debug模式下可以，但是在release版本下却还是会出错！！！据说代码还是在mfc42.dll里出错的。难道是MFC4.2的问题？ 

另一种解决办法见下篇文章：

**如何在没有文档的情况下使用CView及其派生类**

        在MFC中，CView及其派生类将显示及其相关的操作做的很好，比如在程序中 

如果我们的视图类继承CScrollView了那么我们不用写任何代码该视图就能够 

具有滚动的响应机制，能够自动根据设置的滚动范围和窗口实际大小确定是 

否显示滚动条（水平或者竖直的），CEditView，CRichEditView等也提供了 

相应的在不同实用范围内的一些基本操作处理。 

    通常在程序中，我们需要处理并且显示一些数据，将显示部分如果放到视图中 

的话，我们将可以忽略大部分的窗口交互的细节，将注意力关注在数据的显示 

上。

本文是在处理如下的情况中提出的：在数据采集和分析的时候，常常需要观看很 

多的数据。我们不可能在程序的主界面上显示。我们需要一个弹出窗口来显示， 

可以随时的关闭之，也就是说这样的窗口应该动态的生成，数量不定（这样限制 

了我们使用切分视图的选择）。对于这些弹出窗口，我们可以自己写一个CWnd的 

派生类，自然这是完全可以的，但是这样要求你可能处理滚动等等的一些列消息（我 

曾经在一个显示图像灰度直方图、以及二值图像投影等中遇到过要求这样显示的 

情况，当时就是自己从CWnd派生了一个类来显示数据，由于图像比较大，一屏显 

示不完整，所以要给窗口加滚动条，这样在处理这些消息上花费了我大量的时间， 

而真正显示数据的代码却很少）。因此我也在一直想使用MFC已经封装的非常好的 

CView类及其派生类来显示数据。由于我主要用到的就是CScrollView，因此我也 

以它为列来讲述怎么用视图。

基于对话框和SDI结构的实现上有不同细节， 必须分开了说：

1、如果你的程序是一个基于对话框的程序的话（指的是如利用MFC向导生成的对话 

框程序之类），那么事情就很简单： 

   首先，很自然的在Insert/New Class中插入一个类，选择CScrollView做基类 

假设你自己的视图类命名为CMyScrollView。 

   在对话框(不论是主界面的对话框还是新弹出的对话框，都一样)的OnInitDialog 

中加上如下的代码：

```
CRect rectWndClient;
 GetClientRect(&rectWndClient);

 CRuntimeClass *pViewRuntimeClass=RUNTIME_CLASS(CMyScrollView);

 CMyScrollView *pView=(CMyScrollView*)pViewRuntimeClass->CreateObject();
 pView->Create(NULL,NULL,WS_VISIBLE|WS_CHILD,
                          rectWndClient,
                           this,123,NULL);
  pView->OnInitialUpdate();
```

说明： 

          1)、由于CView及其派生类的构造函数是保护成员，所以采用RuntimeClass方式来构造 

      对对象，  

          2)、在Create函数中第一个参数必须有WS_CHILD属性，表明视图是一个子窗口 

          3)、第四个参数rectWndClient可以视情况，改变，因为你可能只想在对话框的某个区域 

      创建视图。 

          4)、第五个参数表明把对话框作为视图的父窗口。 

          5)、第六个参数是视图的ID可以任意指定一个整数

在创建之后，如果不掉用OnInitialUpdate的话会有错误，这是因为CScrollView类有个保护成员 

     m_nMapMode，它在CScrollView的构造函数中被置为 0，如下 

     m_nMapMode = MM_NONE;//在VIEWSCRL.CPP的115行，MM_NONE是宏， 定义为0

下面是错误的分析原因： 

      通常窗口是在响应WM_PAINT的时候绘制客户区。所有的视图都派生自CView，他们的的WM_PAINT响应函数如下

```
void CView::OnPaint() //在VIEWCORE.CPP 176行
  {
   // standard paint routine
   CPaintDC dc(this);
   OnPrepareDC(&dc);
   OnDraw(&dc);
  }
```

可见，在绘图之前虚函数OnPrepareDC会被调用，在CScroolView中重写了这个虚函数，这个虚函数的 

       开头部分如下

```
ASSERT_VALID(pDC);
#ifdef _DEBUG
```

```
if (m_nMapMode == MM_NONE)
   {
    TRACE0("Error: must call SetScrollSizes() or SetScaleToFitSize()");
    TRACE0("\tbefore painting scroll view.\n");
    ASSERT(FALSE);
    return;
   }
   #endif //_DEBUG
```

可见如果m_nMapMode是0（在构造函数中被默认置为0）的话将在调试程序的时候错误 

   所以通常应该在调用OnInitialUpdate的时候（注意：OnInitialUpdate被声明为CView的public成员函数，但是用MFC向导产生自己派生类的OnInitialUpdate函数的时候被声明成了protect，你可以将它改成public，然后调用OnInitialUpdate来初始化自己的视图，或者在构造函数中设置m_nMapMode为自己想要的映射模式）初始化一些东西，并且设置m_nMapMode的正确值，m_nMapMode的一个典型值就是 MM_TEXT（关于映射模式可以看有关文章或者书详细介绍）。

上面讲述了如何在对话框中创建视图和相关函数调用的原因。 有一点要讲的是如果你的对话框加上了 

   最大化按钮的话，你可能想要在最大话对话框的时候让视图也跟着变化，所以你要处理对话框的 

   WM_SIZE消息。在其中你依据当前对话框的实际大小来调整视图大小。这样你就需要视图的指针 

   因此你可能的程序应该是这样的

// 在对话框头文件中，声明一个指向你的视图的指针 

   CMyScrollView     * m_pView; 

   // 在构造函数中将指针赋值NULL 

   m_pView = NULL;

//在OnInitDialog创建视图

```
CRect rectWndClient;
  GetClientRect(&rectWndClient);

  CRuntimeClass *pViewRuntimeClass=RUNTIME_CLASS(CMyScrollView);

  m_pView=(CMyScrollView*)pViewRuntimeClass->CreateObject();
  m_pView->Create(NULL,NULL,WS_VISIBLE|WS_CHILD,
                               rectWndClient,
                               this,123,NULL);
  m_pView->OnInitialUpdat();
  // 在OnSize中改变视图大小
 if  (m_pView) //注意在第一次调用OnSize之前视图還沒有创建，这也是要在构造函数中将
 {            // m_pView)置NULL的原因
         m_pView->SetWindowPos(NULL,0,0,cx,cy,SWP_NOZORDER);
 }
```

// 注意我们在上面用new的方法【具体是在pViewRuntimeClass->CreateObject()的时候】产生了一个 

       CMyScrollView对象，我们的程序并没有调用delete的语句，这样会不会产生内存泄漏呢？

答案是否定的:窗口接受最后一个消息是WM_NCDESTROY，在OnNcDestroy()中CView调用了PostNcDestory这个虚函数 

      我们的类和CScrolView都没有重写PostNcDestory，因此他调用CView的PostNcDestory函数 在这个函数中调用了 

      delete this; CView自己删除了自己 所以不会有内存泄漏。

2、在SDI（单文档）程序中的对话框中使用CView

在SDI中的弹出对话框中创建视图，如果你用，上面的方法1做的话，只要你的鼠标不点击视图，就不会有问题（这样的要求 

      恐怕是没有人能够接受的^_^）。 

      我们先分析这样原因。在视图上点击鼠标会执行如下函数

```
int CView::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
    int nResult = CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
    if (nResult == MA_NOACTIVATE || nResult == MA_NOACTIVATEANDEAT)
            return nResult;   // frame does not want to activate

    CFrameWnd* pParentFrame = GetParentFrame(); // 这里  **********(A)
    if (pParentFrame != NULL)
    {
         // eat it if this will cause activation
         ASSERT(  pParentFrame == pDesktopWnd 
                   || pDesktopWnd->IsChild(pParentFrame)); //还有这里********(B)

        // 省略
             ……
      }
    return nResult;
}
```

上面的A B 两处是我们找到的问题所在，我们最后出错的地方是在B处，但原因是在A. 

    当是对话框程序的时候如前面1，返回值pParentFrame是0，不会执行B的断言，也就不会有错。 

   但是在上面SDI况下，A返回的pParentFrame是应用程序主框架窗口而pDesktopWnd是视图所在的对话框窗口， 

   我们分析B的代码，发现此时pParentFrame == pDesktopWnd 不成立。而且pParentFrame（此时是主窗口） 

   也不是pDesktopWnd（此时是对话框）的子窗口，所以ASSERT断言为FALSE ，产生了错误。

单看上面的代码，及其原因我们还是不能决定如何使用CScrollView。

但是观察A处GetParentFrame的实现，却为我们打开了希望之门：

GetParentFrame是从CWnd继承而来

```
CFrameWnd* CWnd::GetParentFrame() const
 {
    if (GetSafeHwnd() == NULL) // no Window attached
        return NULL;

    ASSERT_VALID(this);

    CWnd* pParentWnd = GetParent();  // start with one parent up
    while (pParentWnd != NULL)
    {
        if (pParentWnd->IsFrameWnd())
                 return (CFrameWnd*)pParentWnd;
            pParentWnd = pParentWnd->GetParent();
    }
     return NULL;
 }
```

分析这段代码，发现他的目的就是在他的祖先窗口中上溯，直到找到一个是FrameWnd类型的窗口之后, 返回这个窗口对象的指针(如果没有的话，返回NULL)，在SDI中这样的窗口是一定存在的(主框架窗口便是！)。 

 我们从前面的分析中知道，pDesktopWnd是对话框窗口，他一定不是一个框架窗口，所以断言的前一部分不能满足 只有考虑后一个，即pDesktopWnd->IsChild(pParentFrame)，先前已经说了主框架不可能是对话框的子窗口，所以要是 pDesktopWnd->IsChild(pParentFrame)满足，即要存在一个CFrameWnd类型的窗口是对话框的子窗口，也是视图的 父窗口(因为GetParentFrame是从视图开始上溯寻找的)，因此我们可以在对话框和视图中间嵌入一个CFrameWnd或者起派生类 的对象。 由于CFrameWnd本身不用什么操作，我们就不用派生一个自己的CMyFrameWnd了，直接用CFrameWnd， 因此 可能的代码是这样的

```
CRect rectWndClient;
GetClientRect(&rectWndClient);
```

```
CFrameWnd *pFrame= new CFrameWnd();
pFrame->Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rectWndClient,this);

CRuntimeClass *pViewClass=RUNTIME_CLASS(CMyScrollView);

CMyScrollView *pView=(CMyScrollView*)pViewClass->CreateObject();
pView->Create(NULL,NULL,WS_VISIBLE|WS_CHILD,
                        rectWndClient,
                        pFrame,123);
pView->OnInitialUpdate();
```

为了能够动态的随对话框的大小改变视图的大小，可以把pFrame和pView作为对话框的成员 

 我自己的某此使用过程中实现如下

// 在对话框头文件中，声明一个指向你的视图的指针

```
CFrameWnd * m_pFrame ;
 CMyScrollView    * m_pView;
```

// 在构造函数中将指针赋值NULL

```php
m_pFrame = NULL;
 m_pView = NULL;
```

//在OnInitDialog创建视图

```
CRect rectWndClient;
GetClientRect(&rectWndClient);
```

```
m_pFrame= new CFrameWnd();
m_pFrame->Create(NULL,NULL,WS_VISIBLE|WS_CHILD,rectWndClient,this); 
CRuntimeClass *pViewClass=RUNTIME_CLASS(CMyScrollView);
m_pView=(CMyScrollView*)pViewClass->CreateObject();
m_pView->Create(NULL,NULL,WS_VISIBLE|WS_CHILD,
                                rectWndClient,
                                m_pFrame,123);
m_pView->OnInitialUpdate();
```

// 在OnSize中改变视图大小

//注意在第一次调用OnSize之前视图還沒有创建，这也是要在构造函数中将 

 // m_pFrame，m_pView 置NULL的原因

```
if (m_pFrame && m_pView)
 {
  m_pFrame->SetWindowPos(NULL,0,0,cx,cy,SWP_NOZORDER);
  m_pView->SetWindowPos(NULL,0,0,cx,cy,SWP_NOZORDER);
 }
```

**《浅谈：切换视时基于FormView的对话框属性设置与ASSERT报错的问题》**

最近做的项目中用到了FormView切换视图，其主要原理是：先新建一些Dialog对话框，然后给这些对话框绑定对应的View，注意：这些View要基于FormView。 

 一开始还好好的，利用切换视的代码进行的很顺利(网上的相关代码很多，我就不赘述了)，但是，后来新加了两个Dialog，不知我怎么弄的，奇怪的事发生了：先前添加了Dialog都能正常的切换，但是，一切换新添加的Dialog，每次运行到：

```
BOOL CFormView::Create(LPCTSTR /*lpszClassName*/, LPCTSTR /*lpszWindowName*/,
 DWORD dwRequestedStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
 CCreateContext* pContext)
{
 ASSERT(pParentWnd != NULL);
 ASSERT(m_lpszTemplateName != NULL); 

 m_pCreateContext = pContext;    // save state for later OnCreate

 #ifdef _DEBUG
 // dialog template must exist and be invisible with WS_CHILD set  <——请注意此处
 if (!_AfxCheckDialogTemplate(m_lpszTemplateName, TRUE))
 {
  ASSERT(FALSE);          // invalid dialog template name    
  PostNcDestroy();        // cleanup if Create fails too soon
  return FALSE;
 }
#endif //_DEBUG
```

中的ASSERT时就报错，而在Release版本下却不会报错。我检查了一下这两个新加的Dialog与先前的Dialog属性有哪些不同，原来是自己把后来加的Dialog的Visible属性设置成了TRUE了, 根据代码的要求是：对话框模板必须存在，属性要设置成不可见和子窗口风格。SystemMenu和TitleBar属性最好都设为False，改完以后，一切正常~~ 

注：还有一种方法就是在添加Dialog资源时，在Dialog列表上点右键->添加资源，选择Dialog->IDD_FORMVIEW,再点“新建”按钮，这样新建出来的Dialog属性就会自动配好了

