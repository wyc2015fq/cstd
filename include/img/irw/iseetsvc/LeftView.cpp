/********************************************************************

	LeftView.cpp

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
	本文件用途：	图像读写引擎－C语言版－VC编译器－测试软件树视
					类实现文件。

	本文件编写人：	
					YZ				yzfree##sina.com

	本文件版本：	30218
	最后修改于：	2003-2-18

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

#include "iseetsvcDoc.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	//{{AFX_MSG_MAP(CLeftView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnDeleteitem)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDED, OnItemexpanded)
	ON_MESSAGE(WM_TVN_DEFERFILL, OnDeferFill)
	ON_MESSAGE(WM_TVN_SELCHANGED, OnSelectChange)
	ON_MESSAGE(WM_TVN_UPDATE, OnUpdateItem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CLeftView construction/destruction

CLeftView::CLeftView()
{
	m_pTreeCtrl = 0;
}


CLeftView::~CLeftView()
{
}


BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= TVS_HASBUTTONS|TVS_HASLINES|TVS_LINESATROOT;

	return CTreeView::PreCreateWindow(cs);
}



/////////////////////////////////////////////////////////////////////////////
// CLeftView drawing

void CLeftView::OnDraw(CDC* pDC)
{
	CIseetsvcDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}


void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
}



/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}


void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}


CIseetsvcDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CIseetsvcDoc)));
	return (CIseetsvcDoc*)m_pDocument;
}
#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers

int CLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 初始化树控件地址
	m_pTreeCtrl = (CShellTree*)	this;

	return 0;
}


// 项扩展响应
void CLeftView::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	m_pTreeCtrl->OnFolderExpanding(pNMHDR, pResult);
	
	*pResult = 0;
}


// 项删除响应
void CLeftView::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	m_pTreeCtrl->OnDeleteShellItem(pNMHDR, pResult);

	*pResult = 0;
}


// 项右点击响应
void CLeftView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_pTreeCtrl->GetContextMenu(pNMHDR, pResult);
	
	*pResult = 0;
}


// 项选择变更响应
void CLeftView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CString	path;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	m_pTreeCtrl->OnFolderSelected(pNMHDR, pResult, path);

	*pResult = 0;
}


void CLeftView::OnDestroy() 
{
	CTreeView::OnDestroy();
}


// 项扩展完毕响应
void CLeftView::OnItemexpanded(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	// 修正树视的边界外观
	((CSplitterWnd*)GetParent())->RecalcLayout();

	*pResult = 0;
}


// 延迟填充响应（由控件的延迟填充线程引发）
LRESULT CLeftView::OnDeferFill(WPARAM wParam, LPARAM lParam)
{
	return m_pTreeCtrl->OnDeferFillAtt(wParam, lParam);
}


// 选择变更响应（由树控件发送，与系统发送的选择变更消息不同）
LRESULT CLeftView::OnSelectChange(WPARAM wParam, LPARAM lParam)
{
	TCHAR	szBuff[MAX_PATH];
	
	lstrcpy((LPSTR)szBuff, "");
	
	// 设置程序标题
	if (m_pTreeCtrl->GetPathFromHTREEITEM((HTREEITEM)wParam, (LPTSTR)szBuff) == 0)
	{
		// 如果是文件系统的，则显示路径
		GetDocument()->SetTitle((LPCTSTR)szBuff);
	}
	else
	{
		// 否则显示树项的显示名
		GetDocument()->SetTitle((LPCTSTR)m_pTreeCtrl->GetItemText((HTREEITEM)wParam));
	}
	
	return 0;
}


// 当引擎被刷新之后调用，以更新列表内容
LRESULT CLeftView::OnUpdateItem(WPARAM wParam, LPARAM lParam)
{
	return m_pTreeCtrl->OnUpdateItem(wParam, lParam);
}

