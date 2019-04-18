# VC++对话框程序打印及打印预览的实现（一） - L_Andy的专栏 - CSDN博客

2012年10月15日 09:47:34[卡哥](https://me.csdn.net/L_Andy)阅读数：4311


张明奇（卡哥）注释：成功版本

        MFC文档/视图的应用程序，向导给了打印及打印预览的标准支持。使这类应用程序打印及打印预览工作得以简化。另一类对话框程序却没有相应支持，从MFC打印及打印预览的标准支持入手，可以在对话框程序中，增加三个类以支持打印及打印预览，本文介绍了这三个类的实现。

        打印及打印预览是编写应用程序经常要解决的问题，为了理解VC++对话框程序的打印及打印预览实现,要先掌握基于文档/视图的应用程序打印及打印预览的基本原理。所以分为两部分介绍。

一、基于文档/视图的应用程序的打印及打印预览原理

        VC++基于文档/视图的应用程序中用MFC应用程序向导在步骤4对话框中选中Print and Print Preview选项，可以包含基本打印及打印预览的支持，应用程序文件菜单中会生成两个菜单项分别是打印（标识符ID_FILE_PRINT）和打印预览(标识符：ID_FILE_PRINT_PREVIEW)，展开程序源代码，可以发现，是Cview类提供标准打印和打印预览菜单命令的消息处理函数：

        设应用程序视图类为CMyView,展开MyView.cpp,其消息映象部分有如下两行：

> 
ON_COMMAND(ID_FILE_PRINT,CView::OnFilePrint)

ON_COMMAND(ID_FILE_PRINT_PREVIEW,CView::OnFilePrintPreview)

CView::OnFilePrint 和CView::OnFilePrintPreview函数都进行打印操作，但View::OnFilePrint将实际发送到打印机，而CView::OnFilePrintPreview则将输出发送到程序窗口上方显示的预览窗口显示一个或两个打印页面的复制外观。利用上面加入的缺省打印支持，只能打印或预览图形的一个打印页面，一个页中放不下的部分则放弃，为了加强程序，使它打印整个图形，一页中放不下的部分放在另一页，可以通过覆盖几个打印期间调用的虚拟函数来完成。下图说明了整个打印和打印预览的过程，显示了每个虚拟函数在程序中哪个部分调用。注意每打印一个页面就要经历图中的一个循环。

![](https://img-my.csdn.net/uploads/201210/22/1350896579_3426.jpg)

        下表列出了虚拟函数并介绍了各自完成的任务，由于MFC打印或打印预览文档时都调用虚拟函数，所以定义的覆盖函数既影响打印也影响打印预览。

二、对话框程序打印及打印预览的实现

        以上是基于文档/视图的应用程序的打印原理，可以根据需要从CView类派生出视图类覆盖打印及打印预览过程中的CView类的虚拟函数来定制文档/视图应用程序的打印及打印预览。在实际中，有很多基于对话框的应用程序，也需要提供打印及打印预览。但向导没有给基于对话框应用程序的基本支持。有了以上知识，可以构造出无文档的视图类，具体的实现时，增加三个类，用以支持打印及打印预览。以下用一具体实例说明。

|函数名()|覆盖函数可能完成的任务|
|----|----|
|CView::OnPreparePrinting()|调用CprintInfo成员函数（如CprintInfo::SetMaxPage设置文档长度）或设置CprintInfo数据成员以影响Print对话框或打印预览操作，然后调用DoPreparePrinting生成用于打印或打印预览的设备描述表（注意必须覆盖OnPreparePrinting并调用DoPreparePrinting）|
|CView::OnBeginPrinting()|分配专门用于打印的字体，画笔、画刷和其它对象，根据设备描述表计算并设置文档长，在设备描述表上存放所需的消息（这是第一个访问设备描述表的的虚拟函数）|
|CView::OnPrepareDC()|设置打印当前页面的文本或图形属性，修改视图原点，以打印当前页面，如果没有设置文档长度，在文档末尾终止打印循环（CprintInfo::m_bContinuePrinting赋值FALSE）|
|CView::OnPrint()|调用OnDraw进行输出；调用OnDraw前选择OnBeginPrinting分配的字体，调用OnDraw后取消对象，打印只出现在文档打印版中的页头和页脚，如果打印输出与屏幕输出的外观不同， 在这里打印，而不是调用OnDraw|
|CView::OnEndPrinting()|调用Cgdi::DeleteObject删除OnBeginPrinting分配的对象|

      1.用MFC应用向导创建对话框应用程序，设主对话框类为CPrintPreviewDlg，在主对话框上放一按钮，（标题：打印预览，ID：IDC_PRINTPREVIEW_BUTTON），用类向导增加其BN_CLICKED的消息响应函数OnPrintPreviewButton生成打印预览界面

> 
void CPrintPreviewDlg::OnPrintPreviewButton()

{

    CPrintFrame* pf = new CPrintFrame(this);

}

        2、增加新类：

用ClassWizard新建CPrintFrame类（基类CFrameWnd）,功能上相当于文档/视图的应用程序的框架窗口类。

用ClassWizard新建CPrintView类（基类CScrollView）,功能上相当于文档/视图的应用程序的视图类。

增加CPrintPreviewView类（基类CPreviewView,注意在头其定义头文件中加入包含afxpriv.h),用于打印预览界面的视图类。

        3、对新生成的各类修改如下：

CPrintFrame类：

①增加公有数据成员

> 
CPrintPreviewDlg*    m_pOldWnd;        // 用于保存主对话框对象；

CPrintView*              m_pView;              // 用于保存视图类对象；

②重载构造函数，保存对主对话框对象指针，创建窗口

> 
CPrintFrame::CPrintFrame(CPrintPreviewDlg* pOld)

{

    m_pOldWnd = pOld;

    if ( !Create(NULL, "打印预览", WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CRect(200, 200, 500, 500)))

        TRACE0("Failed to create view window! \n");

}

③修改析构函数，让主对话框显示

> 
CPrintFrame::~CPrintFrame()

{

    m_pOldWnd->ShowWindow(SW_SHOW);

}

④添加打印模式函数，设置A4纸纵向打印或横向打印(可以选择使用)

> 
#define   DMORIENT_PORTRAIT                           1       //纵向  

#define   DMORIENT_LANDSCAPE                        2       //横向

void SetLandscapeMode(int   PrintMode)  

{   

    PRINTDLG   pd;   

    pd.lStructSize=(DWORD)sizeof(PRINTDLG);   

    BOOL   bRet=AfxGetApp()->GetPrinterDeviceDefaults(&pd);   

    if(bRet)   

    {   

        //   protect   memory   handle   with   ::GlobalLock   and   ::GlobalUnlock  

        DEVMODE   FAR   *pDevMode=(DEVMODE   FAR   *)::GlobalLock(pd.hDevMode);

        pDevMode->dmPaperSize=DMPAPER_A4;   //将打印纸设置为A4   

        //   set   orientation   to   landscape   

        if(PrintMode==1)     //纵向打印   

            pDevMode->dmOrientation = DMORIENT_PORTRAIT;   

        else   if(PrintMode==2)    //横向打印   

            pDevMode->dmOrientation = DMORIENT_LANDSCAPE;       

        ::GlobalUnlock(pd.hDevMode);   

    }   

}   

⑤用ClassWizard增加WM_Create消息处理函数,关联CPrintView视图对象；调用其OnFilePrintPreview函数进行打印预览（若要直接打印，可直接向其发送消息）；隐藏主对话框。

> 
int CPrintFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)

{

    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)

        return -1;

        // TODO: Add your specialized creation code here

        CCreateContext context;

        context.m_pNewViewClass = RUNTIME_CLASS(CPrintView);

        context.m_pCurrentFrame = this;

        context.m_pCurrentDoc = NULL;

        context.m_pLastView = NULL;

        m_pView = STATIC_DOWNCAST(CPrintView, CreateView(&context));

        if (m_pView != NULL)

        {

            m_pView->ShowWindow(SW_SHOW);

            SetActiveView(m_pView);

       //   SetLandscapeMode(DMORIENT_LANDSCAPE);

        }

        SetIcon(m_pOldWnd->GetIcon(FALSE), FALSE);

        SetIcon(m_pOldWnd->GetIcon(TRUE), TRUE);

        ShowWindow(SW_MAXIMIZE);

        CWinApp *pApp = AfxGetApp();

        pApp->m_pMainWnd = this;

        m_pView->OnFilePrintPreview();

        // m_pView->SendMessage(WM_COMMAND, ID_FILE_PRINT);  // 直接打印

        m_pOldWnd->ShowWindow(SW_HIDE);

        return 0;

}

⑥添加WM_Close消息处理函数

> 
void CPrintFrame::OnClose()

{

    // TODO: Add your message handler code here and/or call default

    CPrintFrame* pf=(CPrintFrame*)::AfxGetMainWnd(); 

    CWinApp *pApp=AfxGetApp();

    pApp->m_pMainWnd = pf->m_pOldWnd; 

    pf->DestroyWindow();

    // CFrameWnd::OnClose();

}

CPrintView类：

①修改构造函数：将坐标射模式置为缺省模式。

> 
CPrintView::CPrintView()

{

    m_nMapMode = MM_TEXT;

}

②增加消息映射实现打印。

> 
ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)

③重载虚函数OnPreparePrinting，调用DoPreparePrinting生成用于打印或打印预览的设备描述表。

> 
BOOL CPrintView::OnPreparePrinting(CPrintInfo* pInfo)

{

    // TODO: call DoPreparePrinting to invoke the Print dialog box

    //pInfo->SetMaxPage(1);

    return DoPreparePrinting(pInfo);

    // return CLogScrollView::OnPreparePrinting(pInfo);

}

④增加公有函数OnFilePrintPreview，调用DoPrintPreview实现打印预览，该函数需要传入四个参数：打印预览工具条资源ID，执行打印及打印预览的视图对象指针，打印预览界面视图类的 CRuntimeClass指针，打印预览状态类CPrintPreviewState对象指针。

> 
void CPrintView::OnFilePrintPreview()

{

    CPrintPreviewState* pState = new CPrintPreviewState;

    pState->lpfnCloseProc = _AfxPrintPreviewCloseProc; //设置打印预览窗口关闭时的调用函数

    if (!DoPrintPreview(AFX_IDD_PREVIEW_TOOLBAR, this, RUNTIME_CLASS(CPrintPreviewView), pState))

    {

        TRACE0("Error, DoPrintPreview failed. \n");

        AfxMessageBox(AFX_IDP_COMMAND_FAILURE);

        delete pState;

    }

}

⑤一些其他函数的实现

> 
void CPrintView::OnInitialUpdate()

{

CScrollView::OnInitialUpdate();

 CSize sizeTotal;

 // TODO: 计算此视图的合计大小

 sizeTotal.cx = sizeTotal.cy = 100;

 SetScrollSizes(MM_TEXT, sizeTotal);

}

void CPrintView::OnDraw(CDC* pDC)

{

    CDocument* pDoc = GetDocument();

    // TODO: add draw code here

}

void CPrintView::OnPrint(CDC* pDC, CPrintInfo* pInfo)

{

    // m_rectPrint = pInfo->m_rectDraw; 

    //进行打印的内容代码等；

    CLogScrollView::OnPrint(pDC, pInfo);

}

⑥在PrintView.cpp文件中增加全局函数，_AfxMyPreviewCloseProc，当单击打印预览窗口关闭按钮时被调用。

> 
BOOL CALLBACK _AfxPrintPreviewCloseProc(CFrameWnd* pFrameWnd)

{

    ASSERT_VALID(pFrameWnd);

    CPrintPreviewView* pView = (CPrintPreviewView*)pFrameWnd->GetDlgItem(AFX_IDW_PANE_FIRST);

    ASSERT_KINDOF(CPreviewView, pView);

    pView->OnPreviewClose();

    return FALSE;

}

CPrintPreviewView类：

手动添加类CPrintPreviewView继承CPreviewView类，.h文件如下~

> 
#pragma once

#include "afxpriv.h"

#if !defined(AFX_MYPREVIEWVIEW_H__0AE8B670_B1AE_11DA_812E_00E04C39032F__INCLUDED_)

#define AFX_MYPREVIEWVIEW_H__0AE8B670_B1AE_11DA_812E_00E04C39032F__INCLUDED_

#if _MSC_VER > 1000

#pragma once

#endif // _MSC_VER > 1000

#include <afxpriv.h>

class CMyPreviewView : public CPreviewView 

{

 DECLARE_DYNCREATE(CMyPreviewView)

public:

 afx_msg void OnPreviewClose();

protected:

 CMyPreviewView();

 virtual ~CMyPreviewView();

 void OnDraw(CDC* pDC);

 void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);

 afx_msg void OnPreviewPrint( );

 DECLARE_MESSAGE_MAP( )

};

#endif // !defined(AFX_MYPREVIEWVIEW_H__0AE8B670_B1AE_11DA_812E_00E04C39032F__INCLUDED_)

增加工具栏按钮的消息响应函数OnPreviewClose()，OnPreviewPrint() .cpp如下详尽代码

> 
#include "stdafx.h"

#include ".\mypreviewview.h"

#include "StatisticsCoverDlg.h"

#include ".\statisticscoverdlg.h"

#include  "PrintFrame.h"

//#ifdef _DEBUG

//#undef THIS_FILE

//static char THIS_FILE［］=__FILE__;

//#define new DEBUG_NEW

//#endif

//////////////////////////////////////////

IMPLEMENT_DYNCREATE(CMyPreviewView, CPreviewView)

CMyPreviewView::CMyPreviewView()

{

}

CMyPreviewView::~CMyPreviewView()

{

}

BEGIN_MESSAGE_MAP(CMyPreviewView, CPreviewView)

 ON_COMMAND(AFX_ID_PREVIEW_CLOSE, OnPreviewClose)

 ON_COMMAND(AFX_ID_PREVIEW_PRINT, OnPreviewPrint)

END_MESSAGE_MAP()

void CMyPreviewView::OnDraw(CDC *pDC)

{

 CPreviewView::OnDraw(pDC);

 m_pToolBar->PostMessage(WM_IDLEUPDATECMDUI, (WPARAM)TRUE);// 控制条的命令状态更新

}

void CMyPreviewView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView)

{

 CPreviewView::OnEndPrintPreview(pDC, pInfo, point, pView);

}

void CMyPreviewView::OnPreviewClose()

{

 CPrintFrame* pf=(CPrintFrame*)::AfxGetMainWnd();

 CWinApp *pApp=AfxGetApp();    

 pApp->m_pMainWnd=pf->m_pOldWnd;

 pf->DestroyWindow();

}

void CMyPreviewView::OnPreviewPrint()

{

 m_pPrintView->SendMessage(WM_COMMAND, ID_FILE_PRINT);

}

        至此，基于对话框应用程序的具有打印及打印预览的基本支持已经生成，完全由新生成的三个类来支持。正如上一部分介绍的，可以在CMyView类中定义CView类的几个在打印过程中虚拟函数（CView::OnPreparePrinting，CView::OnBeginPrinting，CView::OnPrepareDC，CView::OnPrint，CView::OnEndPrinting，具体内容可参见前一部分）来定制其打印或打印预览的内容。也可以将实现打印或打印预览新增的三个类，生成MFC扩展动态链接库，以方便加入到程序中。

        打完收功~~

