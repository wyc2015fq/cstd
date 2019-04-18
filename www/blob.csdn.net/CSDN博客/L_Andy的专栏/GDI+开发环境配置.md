# GDI+开发环境配置 - L_Andy的专栏 - CSDN博客

2013年05月24日 09:15:07[卡哥](https://me.csdn.net/L_Andy)阅读数：1043
个人分类：[VC/MFC编程](https://blog.csdn.net/L_Andy/article/category/1099539)



## 下载GDI+ SDK for Visual C++6.0

下载地址：[http://groups.google.com/group/tim-files/files?upload=1](http://groups.google.com/group/tim-files/files?upload=1)     名为GDIPlus.zip

## 配置Visual C++6.0环境

### 将GDI+下的Include文件夹添加到Visual C++6.0

Tools->Options->Directorys下的的Include files下面添加GDI+的Include文件夹的路径，见下图：

![](http://lh5.ggpht.com/_awgsDXknlhw/SU8c5lZnqiI/AAAAAAAAAQM/7ptwgpieCBo/s400/55.jpg)

### 将GDI+下的Lib文件夹添加到Visual C++6.0

Tools->Options->Directorys下的的Lib files下面添加GDI+的Lib文件夹的路径，见下图：

![](http://lh3.ggpht.com/_awgsDXknlhw/SU8c6EE_ozI/AAAAAAAAAQU/l9ERO-PIdR8/s400/66.jpg)

## 配置Visual C++6.0的工程环境

### 在StdAfx.h中添加代码

添加的代码如下：

#define ULONG_PTR ULONG

#include <gdiplus.h>

using namespace Gdiplus;

#pragma comment(lib, "gdiplus.lib") 

### 编译、连接工程

至于如何编译和连接工程，我想就不废话了吧，按两下按钮就OK了。

将GDI+解压出来的文件下的gdiplus.dll拷贝到工程Debug或Release目录下。

### 在CWinApp派生类中添加成员变量

如果你的工程名字是test，那么CWinApp的派生类的名字就为CtestApp。在CtestApp的头文件（也就是test.h）中，添加如下代码：

private:

  GdiplusStartupInput m_gdiplusStartupInput;

  ULONG_PTR m_pGdiToken;

### 在CWinApp派生类中的InitialInstance函数中添加初始化代码

添加的代码如下：

GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);

注意：一定要添加在 m_pMainWnd->ShowWindow(SW_SHOW);之前。

### 为CWinApp派生类添加Virtual Function

添加的虚函数的名字为ExitInstance，并在ExitInstance中添加如下退出GDI+的代码：

GdiplusShutdown(m_pGdiToken);

## 测试GDI+环境是否配置成功

新建一个单文档工程，然后在工程的OnDraw函数中添加如下代码：

Graphics graphics(pDC->m_hDC);

Pen pen(Color(255, 0, 255));

graphics.DrawLine(&pen, 0, 0, 200, 100);

然后编译，运行工程，如果可以显示出一条线，就证明已经配置成功了。

## 特别注意

如果你建立的是一个基于对话框的程序，可能无法使用GDI+。因为我尝试过在对话框程序下面配置GDI+，总是出现如下的错误：

f:/includes/gdiplusenums.h(29) : error C2146: syntax error : missing ';' before identifier 'GraphicsState'

f:/includes/gdiplusenums.h(29) : fatal error C1004: unexpected end of file found

即使你在#include <gdiplus.h>之前加上#include <afxdtctl.h>，那么在编译阶段是不会有错误的，但是连接的时候就会出现同样的错误，而且不能通过。

但是，单文档的应用程序，按照上面的配置就不会出现编译或链接的错误的。

但是，不知道为什么，后来我建立了一个基于对话框的程序，就没有上面的编译的问题。只是不能绘制出曲线。

如果是基于对话框的程序，那么需要相应WM_PAINT消息，然后再OnPaint函数中添加如下代码：

void CDlgGDIDlg::OnPaint() 

{

  CPaintDC dc(this); // device context for painting

  CDC* pDC=GetDC();

  Graphics graphics(pDC->GetSafeHdc());

    //添加自己的绘制代码

  ReleaseDC(pDC);

}

可是仍不能绘制出曲线，原因是如果你建立的是基于对话框的应用程序，那么初始化InitialInstance中的初始化GDI+的代码必须放在DoModal函数之前。然而，即便如此，我发现在OnPait函数中的GDI+代码可以画出来，可是在按钮下面的仍然是编译通过，但是画不出来。

不过有一种解决方法就是在按钮下面调用OnPaint函数，然后再调用RedrawWindow函数就可以了。比如下面的程序：

void CGDIDlg::OnPaint() 

{

 CPaintDC dc(this); // device context for painting

 // TODO: Add your message handler code here

 // Do not call CDialog::OnPaint() for painting messages

 using namespace Gdiplus;

    Graphics graphics( dc.m_hDC );

 if(m_filename!="")

 {

  Image image(m_filename.AllocSysString());

  graphics.DrawImage(&image,0,0);

  UpdateWindow();

 }

}

void CGDIDlg::OnFileOpen() 

{

 // TODO: Add your command handler code here

 //利用系统的打开对话框

 static char szFilter[]="位图文件(*.bmp;*.dib)|*.bmp;*.dib|JPG文件(*.jpg)|*.jpg|All Files(*.*)|*.*||";

  CFileDialog dlg(true,"*.bmp",NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,szFilter);

  if(dlg.DoModal()==IDOK)

  {

   UpdateData(true);

   m_filename=dlg.GetPathName();

  m_pImage=Image::FromFile(m_filename.AllocSysString());

  }

  else

  {

   return;

  }

 OnPaint();

 RedrawWindow(); 

}

如果是单文档或多文档的，要在OnDraw函数中添加如下代码：

void CExampleView::OnDraw(CDC* pDC)

{

    CExampleDoc* pDoc=GetDocument();

    ASSERT_VALID(pDoc);

    if(!pDoc) return;

    //添加内容

    Graphics graph(pDC->GetSafeHdc());

    //添加自己的绘制代码

}

