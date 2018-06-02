/********************************************************************

	MainFrm.cpp

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
	本文件用途：	图像读写引擎－C语言版－VC编译器－测试软件框架窗口
					类实现文件。

	本文件编写人：	
					YZ				yzfree##sina.com

	本文件版本：	30603
	最后修改于：	2003-6-3

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
		地址收集软件。

	修正历史：

		2003-6		修正测试软件无法在Win98系统下运行的BUG
					（该BUG由注册表访问函数内部在不同系统下有不同动作而引起的）
		2003-1		第一个测试版发布

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

#include "IseetsvcDoc.h"
#include "MainFrm.h"
#include "PluginSet.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_VAT_CENTER, OnVatCenter)
	ON_UPDATE_COMMAND_UI(ID_VAT_CENTER, OnUpdateVatCenter)
	ON_COMMAND(ID_VAT_LD, OnVatLd)
	ON_UPDATE_COMMAND_UI(ID_VAT_LD, OnUpdateVatLd)
	ON_COMMAND(ID_VAT_LT, OnVatLt)
	ON_UPDATE_COMMAND_UI(ID_VAT_LT, OnUpdateVatLt)
	ON_COMMAND(ID_VAT_RD, OnVatRd)
	ON_UPDATE_COMMAND_UI(ID_VAT_RD, OnUpdateVatRd)
	ON_COMMAND(ID_VAT_RT, OnVatRt)
	ON_UPDATE_COMMAND_UI(ID_VAT_RT, OnUpdateVatRt)
	ON_COMMAND(ID_VIEW_PLAYTYPE, OnViewPlaytype)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PLAYTYPE, OnUpdateViewPlaytype)
	ON_COMMAND(ID_VRT_AUTO, OnVrtAuto)
	ON_UPDATE_COMMAND_UI(ID_VRT_AUTO, OnUpdateVrtAuto)
	ON_COMMAND(ID_VRT_L200, OnVrtL200)
	ON_UPDATE_COMMAND_UI(ID_VRT_L200, OnUpdateVrtL200)
	ON_COMMAND(ID_VRT_L300, OnVrtL300)
	ON_UPDATE_COMMAND_UI(ID_VRT_L300, OnUpdateVrtL300)
	ON_COMMAND(ID_VRT_L400, OnVrtL400)
	ON_UPDATE_COMMAND_UI(ID_VRT_L400, OnUpdateVrtL400)
	ON_COMMAND(ID_VRT_ORG, OnVrtOrg)
	ON_UPDATE_COMMAND_UI(ID_VRT_ORG, OnUpdateVrtOrg)
	ON_COMMAND(ID_VRT_S20, OnVrtS20)
	ON_UPDATE_COMMAND_UI(ID_VRT_S20, OnUpdateVrtS20)
	ON_COMMAND(ID_VRT_S40, OnVrtS40)
	ON_UPDATE_COMMAND_UI(ID_VRT_S40, OnUpdateVrtS40)
	ON_COMMAND(ID_VRT_S60, OnVrtS60)
	ON_UPDATE_COMMAND_UI(ID_VRT_S60, OnUpdateVrtS60)
	ON_COMMAND(ID_VRT_S80, OnVrtS80)
	ON_UPDATE_COMMAND_UI(ID_VRT_S80, OnUpdateVrtS80)
	ON_COMMAND(ID_VIEW_PLUGININFO, OnViewPlugininfo)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PLUGININFO, OnUpdateViewPlugininfo)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnUpdateViewStyles)
	ON_COMMAND_RANGE(AFX_ID_VIEW_MINIMUM, AFX_ID_VIEW_MAXIMUM, OnViewStyle)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	IDS_IMGINFO,
	IDS_TIMECNT,
	IDS_PROGRESS, 
};



/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}


CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	WINDOWPLACEMENT	*wp;
	UINT			swp;
	
	// 读取上一次窗口的位置信息
	AfxGetApp()->GetProfileBinary(ITSVCMAIN_RKEYSEC, ITSVCMAIN_RKEY_WINPOS, (LPBYTE*)&wp, &swp);
	
	// 设置窗口位置
	if (swp == sizeof(WINDOWPLACEMENT))
	{
		SetWindowPlacement(wp);
		delete []wp;
	}

	ISeeTsVCDebugStrRecorder().Record("主框架窗口创建完毕，下面将创建状态条。", 0, 0);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;
	}

	ISeeTsVCDebugStrRecorder().Record("状态条创建完毕。", 0, 0);

	UINT	idx = m_wndStatusBar.CommandToIndex(ID_SEPARATOR);

	// 设置提示窗格为浮动宽度
	m_wndStatusBar.SetPaneStyle(idx, m_wndStatusBar.GetPaneStyle(idx)|SBPS_STRETCH);

	// 设置时间提示窗格宽度
	idx = m_wndStatusBar.CommandToIndex(IDS_TIMECNT);
	m_wndStatusBar.SetPaneInfo(idx, IDS_TIMECNT, m_wndStatusBar.GetPaneStyle(idx), 50);

	// 设置图象信息窗格宽度
	idx = m_wndStatusBar.CommandToIndex(IDS_IMGINFO);
	m_wndStatusBar.SetPaneInfo(idx, IDS_IMGINFO, m_wndStatusBar.GetPaneStyle(idx), 120);

	EnableDocking(CBRS_ALIGN_ANY);

	// 通知列表视，所有窗口已创建完毕
	GetRightPane()->SendMessage(WM_RV_CREATED);

	return 0;
}


BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	ISeeTsVCDebugStrRecorder().Record("开始创建分裂窗口及树视、列表视、及图视。", 0, 0);

	// 显示欢迎画面
	CSplashWnd::ShowSplashScreen(this);
	
	// 创建A分裂窗口
	if (!m_spa.CreateStatic(this, 1, 2))
	{
		return FALSE;
	}

	LPRECT	rc  = 0;
	UINT	src = 0;
	
	// 恢复上一次运行时树视、列表视窗口的尺寸
	if (!AfxGetApp()->GetProfileBinary(ITSVCMAIN_RKEYSEC, ITSVCMAIN_RKEY_LEFTWINPOS, (LPBYTE*)&rc, &src))
	{
		rc  = new CRect(0,0,200,400);
		src = sizeof(CRect);
	}

	// 创建树视
	if (!m_spa.CreateView(0, 0, pContext->m_pNewViewClass, CSize(rc->right-rc->left, rc->bottom-rc->top), pContext))
	{
		if (rc)
		{
			delete rc;
		}

		m_spa.DestroyWindow();
		return FALSE;
	}
	
	if (rc)
	{
		delete rc;
	}

	// 创建B分裂窗口
	if (!m_spb.CreateStatic(&m_spa, 1, 2, WS_CHILD|WS_VISIBLE|WS_BORDER, m_spa.IdFromRowCol(0,1)))
	{
		m_spa.DestroyWindow();
		return FALSE;
	}

	if (!AfxGetApp()->GetProfileBinary(ITSVCMAIN_RKEYSEC, ITSVCMAIN_RKEY_DISPWINPOS, (LPBYTE*)&rc, &src))
	{
		rc  = new CRect(0,0,200,400);
		src = sizeof(CRect);
	}
	
	// 创建图视和列表视
	if ((!m_spb.CreateView(0, 0, RUNTIME_CLASS(CDisplay), CSize(rc->right-rc->left, rc->bottom-rc->top), pContext))||
		(!m_spb.CreateView(0, 1, RUNTIME_CLASS(CRightView), CSize(0, 0), pContext)))
	{
		if (rc)
		{
			delete rc;
		}

		m_spb.DestroyWindow();
		m_spa.DestroyWindow();
		return FALSE;
	}
	
	if (rc)
	{
		delete rc;
	}

	ISeeTsVCDebugStrRecorder().Record("树视、列表视、图视创建完毕。", 0, 0);

	// 将树视设为当前窗口
	m_spa.SetActivePane(0, 0);

	CIseetsvcDoc *pid = (CIseetsvcDoc*)GetActiveDocument();

	// 初始化画面视指针
	pid->m_pView = (CDisplay*)m_spb.GetPane(0,0);
	pid->m_hView = pid->m_pView->GetSafeHwnd();

	ISeeTsVCDebugStrRecorder().Record("开始组装树视...", 0, 0);
	// 初始化ShellPath容器
	pid->m_spath.InitShellPath((CTreeCtrl*)&(((CTreeView*)m_spa.GetPane(0,0))->GetTreeCtrl()),
		(CListCtrl*)&(((CListView*)m_spb.GetPane(0,1))->GetListCtrl()),
		(CFrameWnd*)this,
		CString((LPCTSTR)isirw_get_shellstr()));
	ISeeTsVCDebugStrRecorder().Record("树视组装完毕。", 0, 0);

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

// 获取列表视指针
CRightView* CMainFrame::GetRightPane()
{
	CWnd* pWnd = m_spb.GetPane(0, 1);
	CRightView* pView = DYNAMIC_DOWNCAST(CRightView, pWnd);
	return pView;
}


// 获取树视指针
CLeftView* CMainFrame::GetLeftPane()
{
	CWnd* pWnd = m_spa.GetPane(0, 0);
	CLeftView* pView = DYNAMIC_DOWNCAST(CLeftView, pWnd);
	return pView;
}


// 获取显示视指针
CDisplay* CMainFrame::GetDisplayPane()
{
	CWnd* pWnd = m_spb.GetPane(0, 0);
	CDisplay* pView = DYNAMIC_DOWNCAST(CDisplay, pWnd);
	return pView;
}


// 列表视风格选择工具
void CMainFrame::OnUpdateViewStyles(CCmdUI* pCmdUI)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.

	CRightView* pView = GetRightPane(); 

	// if the right-hand pane hasn't been created or isn't a view,
	// disable commands in our range

	if (pView == NULL)
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		DWORD dwStyle = pView->GetStyle() & LVS_TYPEMASK;

		// if the command is ID_VIEW_LINEUP, only enable command
		// when we're in LVS_ICON or LVS_SMALLICON mode

		if (pCmdUI->m_nID == ID_VIEW_LINEUP)
		{
			if (dwStyle == LVS_ICON || dwStyle == LVS_SMALLICON)
				pCmdUI->Enable();
			else
				pCmdUI->Enable(FALSE);
		}
		else
		{
			// otherwise, use dots to reflect the style of the view
			pCmdUI->Enable();
			BOOL bChecked = FALSE;

			switch (pCmdUI->m_nID)
			{
			case ID_VIEW_DETAILS:
				bChecked = (dwStyle == LVS_REPORT);
				break;

			case ID_VIEW_SMALLICON:
				bChecked = (dwStyle == LVS_SMALLICON);
				break;

			case ID_VIEW_LARGEICON:
				bChecked = (dwStyle == LVS_ICON);
				break;

			case ID_VIEW_LIST:
				bChecked = (dwStyle == LVS_LIST);
				break;

			default:
				bChecked = FALSE;
				break;
			}

			pCmdUI->SetCheck(bChecked ? 1 : 0);
		}
	}
}


// 修改列表视风格
void CMainFrame::OnViewStyle(UINT nCommandID)
{
	// TODO: customize or extend this code to handle choices on the
	// View menu.
	CRightView* pView = GetRightPane();

	// if the right-hand pane has been created and is a CRightView,
	// process the menu commands...
	if (pView != NULL)
	{
		DWORD dwStyle = -1;

		switch (nCommandID)
		{
		case ID_VIEW_LINEUP:
			{
				// ask the list control to snap to grid
				CListCtrl& refListCtrl = pView->GetListCtrl();
				refListCtrl.Arrange(LVA_SNAPTOGRID);
			}
			break;
		case ID_VIEW_DETAILS:
			dwStyle = LVS_REPORT;
			break;
		case ID_VIEW_SMALLICON:
			dwStyle = LVS_SMALLICON;
			break;
		case ID_VIEW_LARGEICON:
			dwStyle = LVS_ICON;
			break;
		case ID_VIEW_LIST:
			dwStyle = LVS_LIST;
			break;
		}

		// change the style; window will repaint automatically
		if (dwStyle != -1)
		{
			switch (dwStyle)
			{
			case LVS_LIST:
				ISeeTsVCDebugStrRecorder().Record("修改列表视风格为 <列表> 风格。", 0, 0);
				break;
			case LVS_ICON:
				ISeeTsVCDebugStrRecorder().Record("修改列表视风格为 <大图标> 风格。", 0, 0);
				break;
			case LVS_SMALLICON:
				ISeeTsVCDebugStrRecorder().Record("修改列表视风格为 <小图标> 风格。", 0, 0);
				break;
			case ID_VIEW_DETAILS:
				ISeeTsVCDebugStrRecorder().Record("修改列表视风格为 <详细列表> 风格。", 0, 0);
				break;
			default:
				ISeeTsVCDebugStrRecorder().Record("！！！未知的列表视风格被指定。", 0, 0);
			}

			pView->ModifyStyle(LVS_TYPEMASK, dwStyle);
		}
	}
}


void CMainFrame::OnClose() 
{
	CIseetsvcDoc	*pid = (CIseetsvcDoc*)GetActiveDocument();

	// 关闭ShellPath容器
	pid->m_spath.CloseShellPath();

	ISeeTsVCDebugStrRecorder().Record("关闭框架窗口。", 0, 0);

	CWnd		*pWnd = m_spa.GetPane(0, 0);
	CLeftView	*pLView = DYNAMIC_DOWNCAST(CLeftView, pWnd);
	RECT		rc;

	// 保存树视、列表视、显示窗口尺寸
	pLView->GetWindowRect(&rc);
	AfxGetApp()->WriteProfileBinary(ITSVCMAIN_RKEYSEC, ITSVCMAIN_RKEY_LEFTWINPOS, (LPBYTE)&rc, sizeof(RECT));

	pWnd = m_spb.GetPane(0, 1);
	CRightView* pRView = DYNAMIC_DOWNCAST(CRightView, pWnd);

	pRView->GetWindowRect(&rc);
	AfxGetApp()->WriteProfileBinary(ITSVCMAIN_RKEYSEC, ITSVCMAIN_RKEY_RIGHTWINPOS, (LPBYTE)&rc, sizeof(RECT));

	pWnd = m_spb.GetPane(0, 0);
	CDisplay* pDView = DYNAMIC_DOWNCAST(CDisplay, pWnd);

	pDView->GetWindowRect(&rc);
	AfxGetApp()->WriteProfileBinary(ITSVCMAIN_RKEYSEC, ITSVCMAIN_RKEY_DISPWINPOS, (LPBYTE)&rc, sizeof(RECT));
	

	WINDOWPLACEMENT	wp;
	GetWindowPlacement(&wp);

	// 保存当前窗口的位置信息
	AfxGetApp()->WriteProfileBinary(ITSVCMAIN_RKEYSEC, ITSVCMAIN_RKEY_WINPOS, (LPBYTE)&wp, sizeof(WINDOWPLACEMENT));

	CFrameWnd::OnClose();
}


BOOL CMainFrame::OnIdleProc(LONG count)
{
	return FALSE;
}


void CMainFrame::OnVatCenter() 
{
	CDisplay *pDisp = GetDisplayPane();
	pDisp->m_iInitPos = CDisplay::DPT_CENTER;
}

void CMainFrame::OnUpdateVatCenter(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();

	if (pDisp->m_iInitPos != CDisplay::DPT_CENTER)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
}

void CMainFrame::OnVatLd() 
{
	CDisplay *pDisp = GetDisplayPane();
	pDisp->m_iInitPos = CDisplay::DPT_LEFTBOTTOM;
}

void CMainFrame::OnUpdateVatLd(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if (pDisp->m_iInitPos != CDisplay::DPT_LEFTBOTTOM)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
}

void CMainFrame::OnVatLt() 
{
	CDisplay *pDisp = GetDisplayPane();
	pDisp->m_iInitPos = CDisplay::DPT_LEFTTOP;
}

void CMainFrame::OnUpdateVatLt(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if (pDisp->m_iInitPos != CDisplay::DPT_LEFTTOP)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
}

void CMainFrame::OnVatRd() 
{
	CDisplay *pDisp = GetDisplayPane();
	pDisp->m_iInitPos = CDisplay::DPT_RIGHTBOTTOM;
}

void CMainFrame::OnUpdateVatRd(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if (pDisp->m_iInitPos != CDisplay::DPT_RIGHTBOTTOM)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
}

void CMainFrame::OnVatRt() 
{
	CDisplay *pDisp = GetDisplayPane();
	pDisp->m_iInitPos = CDisplay::DPT_RIGHTTOP;
}

void CMainFrame::OnUpdateVatRt(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if (pDisp->m_iInitPos != CDisplay::DPT_RIGHTTOP)
	{
		pCmdUI->SetCheck(0);
	}
	else
	{
		pCmdUI->SetCheck(1);
	}
}

void CMainFrame::OnViewPlaytype() 
{
	CShellList* pList = (CShellList*)GetRightPane();
	
	pList->SetAutoPlay((pList->GetAutoPlay()) ? 0 : 1);
}

void CMainFrame::OnUpdateViewPlaytype(CCmdUI* pCmdUI) 
{
	CShellList* pList = (CShellList*)GetRightPane();
	
	if (pList->GetAutoPlay())
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnVrtAuto() 
{
	CDisplay *pDisp = GetDisplayPane();

	pDisp->m_iTalAutResize = TRUE;

	pDisp->SetDispLocInfo();
	
	// 更新缩放比例
	pDisp->SetPictLocInfo(TRUE);
	pDisp->SetFrmeLocInfo(TRUE);
	
	// 修正图象的显示位置
	pDisp->SetFrmeLocInfo();
	
	// 刷新显示
	pDisp->Invalidate(TRUE);
	pDisp->UpdateWindow();
}

void CMainFrame::OnUpdateVrtAuto(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();

	if (pDisp->m_iTalAutResize)
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnVrtL200() 
{
	CDisplay *pDisp = GetDisplayPane();
	
	pDisp->m_iTalAutResize = FALSE;
	pDisp->m_fTalRatio     = (float)2.0;
	
	pDisp->SetDispLocInfo();
	
	// 更新缩放比例
	pDisp->SetPictLocInfo(TRUE);
	pDisp->SetFrmeLocInfo(TRUE);
	
	// 修正图象的显示位置
	pDisp->SetFrmeLocInfo();
	
	// 刷新显示
	pDisp->Invalidate(TRUE);
	pDisp->UpdateWindow();
}

void CMainFrame::OnUpdateVrtL200(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if ((pDisp->m_fTalRatio == (float)2.0)&&(pDisp->m_iTalAutResize == FALSE))
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnVrtL300() 
{
	CDisplay *pDisp = GetDisplayPane();
	
	pDisp->m_iTalAutResize = FALSE;
	pDisp->m_fTalRatio     = (float)3.0;
	
	pDisp->SetDispLocInfo();
	
	// 更新缩放比例
	pDisp->SetPictLocInfo(TRUE);
	pDisp->SetFrmeLocInfo(TRUE);
	
	// 修正图象的显示位置
	pDisp->SetFrmeLocInfo();
	
	// 刷新显示
	pDisp->Invalidate(TRUE);
	pDisp->UpdateWindow();
}

void CMainFrame::OnUpdateVrtL300(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();

	if ((pDisp->m_fTalRatio == (float)3.0)&&(pDisp->m_iTalAutResize == FALSE))
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnVrtL400() 
{
	CDisplay *pDisp = GetDisplayPane();
	
	pDisp->m_iTalAutResize = FALSE;
	pDisp->m_fTalRatio     = (float)4.0;
	
	pDisp->SetDispLocInfo();
	
	// 更新缩放比例
	pDisp->SetPictLocInfo(TRUE);
	pDisp->SetFrmeLocInfo(TRUE);
	
	// 修正图象的显示位置
	pDisp->SetFrmeLocInfo();
	
	// 刷新显示
	pDisp->Invalidate(TRUE);
	pDisp->UpdateWindow();
}

void CMainFrame::OnUpdateVrtL400(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if ((pDisp->m_fTalRatio == (float)4.0)&&(pDisp->m_iTalAutResize == FALSE))
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnVrtOrg() 
{
	CDisplay *pDisp = GetDisplayPane();
	
	pDisp->m_iTalAutResize = FALSE;
	pDisp->m_fTalRatio     = (float)1.0;
	
	pDisp->SetDispLocInfo();
	
	// 更新缩放比例
	pDisp->SetPictLocInfo(TRUE);
	pDisp->SetFrmeLocInfo(TRUE);
	
	// 修正图象的显示位置
	pDisp->SetFrmeLocInfo();
	
	// 刷新显示
	pDisp->Invalidate(TRUE);
	pDisp->UpdateWindow();
}

void CMainFrame::OnUpdateVrtOrg(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if ((pDisp->m_fTalRatio == (float)1.0)&&(pDisp->m_iTalAutResize == FALSE))
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnVrtS20() 
{
	CDisplay *pDisp = GetDisplayPane();
	
	pDisp->m_iTalAutResize = FALSE;
	pDisp->m_fTalRatio     = (float)0.2;
	
	pDisp->SetDispLocInfo();
	
	// 更新缩放比例
	pDisp->SetPictLocInfo(TRUE);
	pDisp->SetFrmeLocInfo(TRUE);
	
	// 修正图象的显示位置
	pDisp->SetFrmeLocInfo();
	
	// 刷新显示
	pDisp->Invalidate(TRUE);
	pDisp->UpdateWindow();
}

void CMainFrame::OnUpdateVrtS20(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if ((pDisp->m_fTalRatio == (float)0.2)&&(pDisp->m_iTalAutResize == FALSE))
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnVrtS40() 
{
	CDisplay *pDisp = GetDisplayPane();
	
	pDisp->m_iTalAutResize = FALSE;
	pDisp->m_fTalRatio     = (float)0.4;
	
	pDisp->SetDispLocInfo();
	
	// 更新缩放比例
	pDisp->SetPictLocInfo(TRUE);
	pDisp->SetFrmeLocInfo(TRUE);
	
	// 修正图象的显示位置
	pDisp->SetFrmeLocInfo();
	
	// 刷新显示
	pDisp->Invalidate(TRUE);
	pDisp->UpdateWindow();
}

void CMainFrame::OnUpdateVrtS40(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if ((pDisp->m_fTalRatio == (float)0.4)&&(pDisp->m_iTalAutResize == FALSE))
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnVrtS60() 
{
	CDisplay *pDisp = GetDisplayPane();
	
	pDisp->m_iTalAutResize = FALSE;
	pDisp->m_fTalRatio     = (float)0.6;
	
	pDisp->SetDispLocInfo();
	
	// 更新缩放比例
	pDisp->SetPictLocInfo(TRUE);
	pDisp->SetFrmeLocInfo(TRUE);
	
	// 修正图象的显示位置
	pDisp->SetFrmeLocInfo();
	
	// 刷新显示
	pDisp->Invalidate(TRUE);
	pDisp->UpdateWindow();
}

void CMainFrame::OnUpdateVrtS60(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if ((pDisp->m_fTalRatio == (float)0.6)&&(pDisp->m_iTalAutResize == FALSE))
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnVrtS80() 
{
	CDisplay *pDisp = GetDisplayPane();
	
	pDisp->m_iTalAutResize = FALSE;
	pDisp->m_fTalRatio     = (float)0.8;
	
	pDisp->SetDispLocInfo();
	
	// 更新缩放比例
	pDisp->SetPictLocInfo(TRUE);
	pDisp->SetFrmeLocInfo(TRUE);
	
	// 修正图象的显示位置
	pDisp->SetFrmeLocInfo();
	
	// 刷新显示
	pDisp->Invalidate(TRUE);
	pDisp->UpdateWindow();
}

void CMainFrame::OnUpdateVrtS80(CCmdUI* pCmdUI) 
{
	CDisplay *pDisp = GetDisplayPane();
	
	if ((pDisp->m_fTalRatio == (float)0.8)&&(pDisp->m_iTalAutResize == FALSE))
	{
		pCmdUI->SetCheck(1);
	}
	else
	{
		pCmdUI->SetCheck(0);
	}
}

void CMainFrame::OnViewPlugininfo() 
{
	CPluginSet		dlg;
	LPIRWMF_HEADER	pib;

	// 等待引擎空闲下来
	while (isirw_is_busy())
	{
		::Sleep(0);
	}

	// 锁定图象读写引擎
	pib = isirw_lock();

	if (!pib)
	{
		AfxMessageBox("ISee图象读写引擎无法锁定，软件处于不稳定状态，建议关闭本软件！");
		return;
	}

	// 设置对话框的插件信息表地址
	dlg.SetData(pib);

	// 打开设置对话框
	if (dlg.DoModal() == IDOK)
	{
		// 如果用户进行了插件调整，则修改插件信息表
		for (int i=0; i<dlg.plicnt; i++)
		{
			if (dlg.plda[i].inst != dlg.plda[i].iost)
			{
				if (dlg.plda[i].inst == CPluginSet::PLINSTATUS_NOUSE)
				{
					isirw_dismissal(dlg.plda[i].id);	// 解雇插件
				}
				else
				{
					isirw_employ(dlg.plda[i].id);		// 录用插件
				}
			}
		}

		CIseetsvcDoc	*pid = (CIseetsvcDoc*)GetActiveDocument();
		CString			filt((LPCTSTR)isirw_get_shellstr());

		// 更新文件过滤串
		pid->m_spath.SetFilterString(filt);
		
		// 刷新列表项
		GetLeftPane()->PostMessage(WM_TVN_UPDATE);
	}

	// 引擎解锁
	isirw_unlock(pib);
}

void CMainFrame::OnUpdateViewPlugininfo(CCmdUI* pCmdUI) 
{
	if (GetRightPane()->m_bFillMark < 2)
	{
		pCmdUI->Enable(FALSE);							// 在填充目录项期间，不能打开插件设置对话框
	}
	else
	{
		pCmdUI->Enable();
	}
}

void CMainFrame::OnFileSaveAs() 
{
	return;
}

void CMainFrame::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	CIseetsvcDoc	*pid = (CIseetsvcDoc*)GetActiveDocument();
	
	if (pid->m_bPkst)
	{
		pCmdUI->Enable();
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}
