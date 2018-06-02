/********************************************************************

	RightView.cpp

	----------------------------------------------------------------
    软件许可证 － GPL
	版权所有 (C) 2003 VCHelp coPathway ISee workgroup.
	----------------------------------------------------------------
	这一程序是自由软件，你可以遵照自由软件基金会出版的GNU 通用公共许
	可证条款来修改和重新发布这一程序。或者用许可证的第二版，或者（根
	据你的选择）用任何更新的版本。

    发布这一程序的目的是希望它有用，但没有任何担保。甚至没有适合特定
	目地的隐含的担保。更详细的情况请参阅GNU通用公共许可证。

    你应该已经和程序一起收到一份GNU通用公共许可证的副本（本目录
	GPL.txt文件）。如果还没有，写信给：
    The Free Software Foundation, Inc.,  675  Mass Ave,  Cambridge,
    MA02139,  USA
	----------------------------------------------------------------
	如果你在使用本软件时有什么问题或建议，请用以下地址与我们取得联系：

			http://isee.126.com
			http://cosoft.org.cn/projects/iseeexplorer
			
	或发信到：

			isee##vip.163.com
	----------------------------------------------------------------
	本文件用途：	图像读写引擎－C语言版－VC编译器－测试软件列表视
					类实现文件。

	本文件编写人：	
					YZ				yzfree##sina.com

	本文件版本：	30222
	最后修改于：	2003-2-22

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
		地址收集软件。

	修正历史：

		2003-2		第一个测试版发布

********************************************************************/

#include "stdafx.h"
#include "iseetsvc.h"

#include "shellpidl.h"
#include "shelllist.h"
#include "shelltree.h"
#include "shellpath.h"

#include "Display.h"
#include "LeftView.h"
#include "RightView.h"

#include "MainFrm.h"
#include "iseetsvcDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CRightView

IMPLEMENT_DYNCREATE(CRightView, CListView)

CRightView::CRightView()
{
	m_bFrmMark  = FALSE;
	m_bFillMark = 2;			// 填充完毕标志
}

CRightView::~CRightView()
{
}


BEGIN_MESSAGE_MAP(CRightView, CListView)
	//{{AFX_MSG_MAP(CRightView)
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_MESSAGE(WM_LVN_FILLBEGIN, OnFillBegin)
	ON_MESSAGE(WM_LVN_FILLEND, OnFillEnd)
	ON_MESSAGE(WM_LVN_AUTONEXT, OnAutoNext)
	ON_MESSAGE(WM_LVN_SUBITEMDATA, OnSubItemDataFill)
	ON_MESSAGE(WM_RV_CREATED, OnWndCreated)
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnDeleteitem)
	ON_NOTIFY_REFLECT(LVN_DELETEALLITEMS, OnDeleteallitems)
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	ON_NOTIFY_REFLECT(LVN_ITEMACTIVATE, OnActive)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRightView construction/destruction

BOOL CRightView::PreCreateWindow(CREATESTRUCT& cs)
{
	UINT type = AfxGetApp()->GetProfileInt(ITSVCMAIN_RKEYSEC, ITSVCRIGHT_RKEY_LISTYPE, LVS_REPORT);

	cs.style |= LVS_SORTASCENDING | LVS_NOSORTHEADER | LVS_SHAREIMAGELISTS | LVS_SINGLESEL | LVS_AUTOARRANGE | type;

	return CListView::PreCreateWindow(cs);
}



/////////////////////////////////////////////////////////////////////////////
// CRightView drawing

void CRightView::OnDraw(CDC* pDC)
{
	CIseetsvcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CListCtrl& refCtrl = GetListCtrl();
}


void CRightView::OnInitialUpdate()
{
	GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_INFOTIP | LVS_EX_ONECLICKACTIVATE);

	CListView::OnInitialUpdate();
}



/////////////////////////////////////////////////////////////////////////////
// CRightView diagnostics

#ifdef _DEBUG
void CRightView::AssertValid() const
{
	CListView::AssertValid();
}


void CRightView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}


CIseetsvcDoc* CRightView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIseetsvcDoc)));
	return (CIseetsvcDoc*)m_pDocument;
}
#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CRightView message handlers

void CRightView::OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct)
{
	GetListCtrl().Arrange(LVA_DEFAULT);
}


void CRightView::OnDestroy() 
{
	DWORD dwStyle = GetStyle() & LVS_TYPEMASK;

	AfxGetApp()->WriteProfileInt(ITSVCMAIN_RKEYSEC, ITSVCRIGHT_RKEY_LISTYPE, dwStyle);

	CListView::OnDestroy();
}


int CRightView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 初始化列表控件地址
	m_pListCtrl = (CShellList*)this;
	
	return 0;
}


// 开始一个新的填充过程（由树控件引发）
LRESULT CRightView::OnFillBegin(WPARAM wParam, LPARAM lParam)
{
	// 锁定窗口刷新，避免闪烁
	LockWindowUpdate();

	m_bFillMark = 0;

	// 填充列表控件
	m_pListCtrl->OnFillBegin(wParam, lParam);

	return 0;
}


// 结束一个填充过程（由树控件引发）
LRESULT CRightView::OnFillEnd(WPARAM wParam, LPARAM lParam)
{
	// 通知列表控件结束填充
	m_pListCtrl->OnFillEnd(wParam, lParam);

	// 文件名填充完毕
	m_bFillMark = 1;

	// 恢复窗口刷新
	UnlockWindowUpdate();

	return 0;
}


// 自动选择下一幅图象（自动播放时调用）
LRESULT CRightView::OnAutoNext(WPARAM wParam, LPARAM lParam)
{
	return m_pListCtrl->OnAutoNext(wParam, lParam);
}


// 填充列表项的详细数据（由填充线程引发）
LRESULT CRightView::OnSubItemDataFill(WPARAM wParam, LPARAM lParam)
{
	return m_pListCtrl->OnSubItemDataFill(wParam, lParam);
}


void CRightView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);

	int		item = -1;

	// 使选择项保持可见
	if ((item=GetListCtrl().GetNextItem(item, LVNI_SELECTED)) != -1)
	{
		GetListCtrl().EnsureVisible(item, 1);
	}
	
	// 修正状态条中信息窗格的尺寸
	if (m_bFrmMark)
	{
		CStatusBar	*cbr = &(((CMainFrame*)AfxGetMainWnd())->m_wndStatusBar);
		UINT		idx = cbr->CommandToIndex(IDS_PROGRESS);

		WINDOWPLACEMENT	wpt;

		wpt.length  = sizeof(WINDOWPLACEMENT);
		wpt.showCmd	= 0;

		if (!AfxGetMainWnd()->GetWindowPlacement(&wpt))
		{
			return;
		}

		if (wpt.showCmd == SW_SHOWMAXIMIZED)
		{
			cx = ((cx-2) < 0) ? 0 : cx-2;
		}
		else
		{
			cx = ((cx-GetSystemMetrics(SM_CXHSCROLL)-4) < 0) ? 0 : cx-GetSystemMetrics(SM_CXHSCROLL)-4;
		}

		if (GetStyle()&WS_VSCROLL)
		{
			cx += GetSystemMetrics(SM_CXVSCROLL);
		}

		cbr->SetPaneInfo(idx, IDS_PROGRESS, cbr->GetPaneStyle(idx), cx);
	}

	return;
}


// 当框架窗口创建完所有的窗口后，将发送WM_RV_CREATED消息，本函数将响应此消息
LRESULT CRightView::OnWndCreated(WPARAM wParam, LPARAM lParam)
{
	m_bFrmMark = TRUE;

	return 0;
}


void CRightView::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_pListCtrl->OnDeleteitem(pNMHDR, pResult);
}


void CRightView::OnDeleteallitems(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_pListCtrl->OnDeleteallitems(pNMHDR, pResult);
}


void CRightView::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMITEMACTIVATE	lpnmitem = (LPNMITEMACTIVATE)pNMHDR;

	int	item = m_pListCtrl->GetNextItem(-1, LVNI_ALL|LVNI_SELECTED);

	


	*pResult = 0;
}


void CRightView::OnActive(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult = 0;
}



void CRightView::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMLISTVIEW	pnmv = (LPNMLISTVIEW)pNMHDR;

	if ((pnmv->iItem != -1)&&(pnmv->uChanged == LVIF_STATE)&&(pnmv->uNewState&LVIS_SELECTED))
	{
		char	buf[MAX_PATH];

		m_pListCtrl->GetItemText(pnmv->iItem, 0, (LPTSTR)buf, MAX_PATH);

		m_pListCtrl->m_pPath->ObjectChanged((LPCTSTR)0, (LPCTSTR)buf, pnmv->iItem);

		ISeeTsVCDebugStrRecorder().Record("ShellList 当前选择文件为 <%s>。", 1, (LPCTSTR)buf);
	}
	
	*pResult = 0;
}


