# VC++对话框程序打印及打印预览的实现（二） - L_Andy的专栏 - CSDN博客

2012年10月22日 17:32:52[卡哥](https://me.csdn.net/L_Andy)阅读数：4465


这是接上面的具体实现能够成功的打印出数据

CPrintView.h的详细代码

#pragma once

#include "StatisticsCoverDlg.h"

// CPrintView 视图

#define PRINTMARGIN 2

BOOL CALLBACK _AfxMyPreviewCloseProc(CFrameWnd* pFrameWnd);

class CPrintView : public CScrollView

{

 DECLARE_DYNCREATE(CPrintView)

public:

 CPrintView();           // 动态创建所使用的受保护的构造函数

 virtual ~CPrintView();

 CCoverageStatisticsDlg*       m_pOldWnd;

public:

#ifdef _DEBUG

 virtual void AssertValid() const;

 virtual void Dump(CDumpContext& dc) const;

#endif

protected:

 virtual void OnDraw(CDC* pDC);      // 重写以绘制此视图

 virtual void OnInitialUpdate();     // 构造后的第一次

    virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

 virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);

    virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

 void PrintPageHeader(CDC* pDC, UINT nPageNumber, CPrintInfo* pInfo);;

 UINT m_cxWidth;

 UINT m_cxOffset;

 UINT m_nLinesPerPage;

 UINT m_cyPrinter;

 CFont m_ListFont;

 CFont m_fontPrinter;

 BOOL m_bPrintData;

 BOOL m_bPrintImage;

 BOOL m_bPrintHead;

 DECLARE_MESSAGE_MAP()

public:

 BOOL OnPreparePrinting(CPrintInfo* pInfo);

 void OnFilePrintPreview( );

};

//

#include "stdafx.h"

#include "RadioPlaning.h"

#include "PrintView.h"

#include "MyPreviewView.h"

// CPrintView

IMPLEMENT_DYNCREATE(CPrintView, CScrollView)

//修改构造函数；将坐标射模式置换成缺省模式。

CPrintView::CPrintView()

{

 m_nMapMode = MM_TEXT;

}

CPrintView::~CPrintView()

{

}

BEGIN_MESSAGE_MAP(CPrintView, CScrollView)

 //增加消息映射实现打印

 ON_COMMAND(ID_FILE_PRINT,CView::OnFilePrint)

END_MESSAGE_MAP()

// CPrintView 绘图

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

 // TODO: 在此添加绘制代码

}

// CPrintView 诊断

#ifdef _DEBUG

void CPrintView::AssertValid() const

{

 CScrollView::AssertValid();

}

void CPrintView::Dump(CDumpContext& dc) const

{

 CScrollView::Dump(dc);

}

#endif //_DEBUG

// CPrintView 消息处理程序

//重载虚函数OnPreatePrinting，调用DoPreparePrinting生成

//用于打印或打印预览的设备描述表

BOOL CPrintView::OnPreparePrinting(CPrintInfo* pInfo)

{

 return DoPreparePrinting(pInfo);

}

void CPrintView::OnFilePrintPreview( )

{

 CPrintPreviewState* pState = new CPrintPreviewState;

 //pState->lpfnCloseProc =_AfxMyPreviewCloseProc;//设置打印预览窗口关闭时的调用函数；

 if(!DoPrintPreview(AFX_IDD_PREVIEW_TOOLBAR, this,RUNTIME_CLASS(CMyPreviewView), pState))

 {

  TRACE0("Error: DoPrintPreview failed.\n");

  AfxMessageBox(AFX_IDP_COMMAND_FAILURE);

  delete pState;

 }

}

//当单机打印预览窗口关闭按钮时调用

BOOL CALLBACK _AfxMyPreviewCloseProc(CFrameWnd* pFrameWnd)

{

 ASSERT_VALID(pFrameWnd);

 CMyPreviewView* pView = (CMyPreviewView*) pFrameWnd->GetDlgItem(AFX_IDW_PANE_FIRST);

 ASSERT_KINDOF(CPreviewView, pView);

 pView->OnPreviewClose();

 return FALSE;

}

//打印页眉页脚

void CPrintView::PrintPageHeader(CDC* pDC, UINT nPageNumber, CPrintInfo* pInfo)

{

 CFont *pOldFont;

 CFont fontHeader;

 fontHeader.CreatePointFont(100,"FixedSys",pDC);

 pOldFont=(CFont *)(pDC->SelectObject(&fontHeader));

 int nPosY,nPosX;

 nPosY= 3 * m_cyPrinter;

 nPosX= m_cxOffset;

 pDC->TextOut(nPosX,nPosY,CString(_T("北京联基动力科技有限公司")));

 CString str;

 str.Format(_T("第 %d 页 / 共 %d 页"),nPageNumber,pInfo->GetMaxPage());

 CSize size = pDC->GetTextExtent(str);     //获取特定的字符串在屏幕上所占的宽度和高度

 nPosX = m_cxOffset+m_cxWidth-size.cx;

 pDC->TextOut(nPosX,nPosY,str);

 pDC->MoveTo(m_cxOffset,4*m_cyPrinter);

 pDC->LineTo(m_cxOffset+m_cxWidth,4*m_cyPrinter);

 pDC->SelectObject(pOldFont);

 fontHeader.DeleteObject();

}

//开始打印设置字体等

void CPrintView::OnBeginPrinting(CDC* pDC,CPrintInfo* pInfo)

{

 CScrollView::OnBeginPrinting(pDC,pInfo);

 //CDC* pDC   = (CDC*)wParam;

 //CPrintInfo* pInfo = (CPrintInfo *)lParam;

 if(m_fontPrinter.m_hObject==NULL)

  m_fontPrinter.CreatePointFont(120,"FixedSys",pDC);

 TEXTMETRIC tm;

 CFont* pOldFont = pDC->SelectObject (&m_fontPrinter);

 pDC->GetTextMetrics (&tm);                                           //获取当前字体的信息

 m_cyPrinter = tm.tmHeight + tm.tmExternalLeading;

 CSize size = pDC->GetTextExtent (_T ("---------1---------2---------" \

  "3---------4---------5---------6---------7---------8------"), 81);

 pDC->SelectObject (pOldFont);

 m_cxWidth = size.cx;

 //////////////////////////////////////////////////////////////////////////

 m_nLinesPerPage = (pDC->GetDeviceCaps (VERTRES) -

  (m_cyPrinter * (3 + (2 * PRINTMARGIN)))) / m_cyPrinter;

 //pInfo->SetMaxPage (1);

 pInfo->SetMaxPage(pInfo->m_nCurPage);

 m_cxOffset = (pDC->GetDeviceCaps (HORZRES) - size.cx) / 2;

 return ;

}

//结束打印，释放字体等

void CPrintView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)

{

 CScrollView::OnEndPrinting(pDC,pInfo);

 if(m_fontPrinter.m_hObject!=NULL)

  m_fontPrinter.DeleteObject();

 return;

}

//打印数据

void CPrintView::OnPrint(CDC* pDC,CPrintInfo* pInfo)

{//Andy

 CScrollView::OnPrint(pDC,pInfo);

 UINT gl_uNumOfPoints = 52;

 //CDC* pDC   = (CDC*)wParam;

 //CPrintInfo* pInfo = (CPrintInfo *)lParam;

 int nPageNumber = pInfo->m_nCurPage;

 int i,j;

 CFont *pOldFont;

 CFont DataFont;

 DataFont.CreatePointFont(120,_T("宋体"),pDC);

 TCHAR *pszTitle[4]={"台站名称","门限","面积","人口"};

 pOldFont = pDC->SelectObject(&DataFont);

 pDC->Rectangle(m_cxOffset,(int)((2.5 + PRINTMARGIN)*m_cyPrinter),m_cxOffset+m_cxWidth,(m_nLinesPerPage+3 + PRINTMARGIN)*m_cyPrinter+20);

 for(i=1;i<4;i++)

 {

  pDC->MoveTo(m_cxOffset+i*m_cxWidth/4,(int)((2.5 + PRINTMARGIN)*m_cyPrinter));

  pDC->LineTo(m_cxOffset+i*m_cxWidth/4,(3 +m_nLinesPerPage+PRINTMARGIN)*m_cyPrinter+20);

 }

 pDC->MoveTo(m_cxOffset,(4 + PRINTMARGIN)*m_cyPrinter);

 pDC->LineTo(m_cxOffset+m_cxWidth,(4 + PRINTMARGIN)*m_cyPrinter);

 for(i=0;i<4;i++)

 {

  pDC->TextOut(m_cxOffset+50+i*m_cxWidth/4,(int)((2.8 + PRINTMARGIN)*m_cyPrinter),CString(pszTitle[i]));

 }

 //////////////////////////////////////Andy//////////////////////////////预览数据

 for(int i = 0; i < g_p2DView->m_StationManage.m_StationEntityArray.GetSize(); i++)

 {

  CStationEntity* pEntity = g_p2DView->m_StationManage.m_StationEntityArray.GetAt(i);

  double nCoverArea = g_p2DView->m_StationManage.GetCoverageArea(pEntity->m_nObjID,g_pMain->m_sysParameters.fThreshold);

  Station station = (g_p2DView->m_StationManage.m_StationEntityArray.GetAt(i))->m_Station;

  CString str[4];

  int nIndex = (nPageNumber-1)*(m_nLinesPerPage-1)+i;

  if(nIndex<(int)gl_uNumOfPoints)

  {

   str[0].Format("%s",station.szName);

   str[1].Format("%.3f",g_pMain->m_sysParameters.fThreshold);

   str[2].Format("%.3f",nCoverArea);

   str[3].Format("%.f",0);

   for(j=0;j<4;j++)

   {

    pDC->TextOut(m_cxOffset+50+j*m_cxWidth/4,(4 + PRINTMARGIN+i)*m_cyPrinter+5,str[j]);

   }

  }

 }

 pDC->SelectObject (pOldFont);

 PrintPageHeader(pDC,pInfo->m_nCurPage,  pInfo);

 return ;

}

