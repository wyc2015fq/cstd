// BCGPPropertySheet.cpp : implementation file
//

//#include "BCGCBPro.h"
#include "BCGPPropertyPage.h"
#include "BCGPPropertySheet.h"
#include "BCGPVisualManager.h"
#include "TrackMouse.h"
#include "BCGPDrawManager.h"
#include "BCGPButton.h"

#ifndef _BCGSUITE_
#include "BCGPPngImage.h"
#endif

const int idTree = 101;
const int idTab = 102;
const int idList = 103;

/////////////////////////////////////////////////////////////////////////////
// CBCGPPropSheetPane

BOOL CBCGPPropSheetPane::OnSendCommand (const CBCGPToolbarButton* pButton)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pParent);

	CWaitCursor wait;
	m_pParent->SetActivePage (ButtonToIndex (pButton));

	return TRUE;
}
//****
void CBCGPPropSheetPane::EnsureVisible (int iButton)
{
	ASSERT_VALID (this);

	CBCGPToolbarButton* pButton = GetButton (iButton);
	ASSERT_VALID (pButton);

	CRect rectButton = pButton->Rect();

	CRect rectWork;
	GetClientRect (rectWork);

	if (rectButton.Height() >= rectWork.Height())
	{
		// Work area is too small, nothing to do
		return;
	}

	if (rectButton.t >= rectWork.t && rectButton.b <= rectWork.b)
	{
		// Already visible
		return;
	}

	if (rectButton.t < rectWork.t)
	{
		while (pButton->Rect().t < rectWork.t)
		{
			int iScrollOffset = m_iScrollOffset;

			ScrollUp();

			if (iScrollOffset == m_iScrollOffset)
			{
				break;
			}
		}
	}
	else
	{
		while (pButton->Rect().b > rectWork.b)
		{
			int iScrollOffset = m_iScrollOffset;

			ScrollDown();

			if (iScrollOffset == m_iScrollOffset)
			{
				break;
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPPropSheetTab

CBCGPPropSheetTab::CBCGPPropSheetTab()
{
	m_bIsDlgControl = TRUE;
}
//****
BOOL CBCGPPropSheetTab::SetActiveTab (int iTab)
{
	ASSERT_VALID (this);
	ASSERT_VALID (m_pParent);

	CWaitCursor wait;

	if (m_pParent->GetActiveIndex() != iTab)
	{
		if (!m_pParent->SetActivePage (iTab))
		{
			return FALSE;
		}
	}

	CBCGPTabWnd::SetActiveTab (iTab);

	CRect rectWndArea = m_rectWndArea;
	MapWindowPoints (m_pParent, rectWndArea);

	CPropertyPage* pPage = m_pParent->GetPage (iTab);
	if (pPage != NULL)
	{
		pPage->SetWindowPos (NULL, rectWndArea.l, rectWndArea.t,
			rectWndArea.Width(), rectWndArea.Height(),
			SWP_NOACTIVATE | SWP_NOZORDER);
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
// CBCGPPropSheetCategory

IMPLEMENT_DYNAMIC(CBCGPPropSheetCategory, CObject)

CBCGPPropSheetCategory::CBCGPPropSheetCategory (LPCTSTR lpszName, int nIcon, 
											  int nSelectedIcon,
											  const CBCGPPropSheetCategory* pParentCategory,
											  CBCGPPropertySheet& propSheet) :
	m_strName (lpszName),
	m_nIcon (nIcon),
	m_nSelectedIcon (nSelectedIcon),
	m_pParentCategory ((CBCGPPropSheetCategory*) pParentCategory),
	m_propSheet (propSheet)
{
	m_hTreeItem = NULL;
	m_hLastSelectedItem = NULL;

	if (m_pParentCategory != NULL)
	{
		ASSERT_VALID (m_pParentCategory);
		m_pParentCategory->m_lstSubCategories.AddTail (this);
	}
}

CBCGPPropSheetCategory::~CBCGPPropSheetCategory()
{
	while (!m_lstSubCategories.IsEmpty())
	{
		delete m_lstSubCategories.RemoveHead();
	}

	if (m_propSheet.GetSafeHwnd() != NULL)
	{
		for (POSITION pos = m_lstPages.GetHeadPosition(); pos != NULL;)
		{
			CBCGPPropertyPage* pPage = m_lstPages.GetNext (pos);
			ASSERT_VALID (pPage);

			m_propSheet.RemovePage (pPage);
		}

		if (m_propSheet.m_wndTree.GetSafeHwnd() != NULL && m_hTreeItem != NULL)
		{
			m_propSheet.m_wndTree.DeleteItem (m_hTreeItem);
		}
	}

	if (m_pParentCategory != NULL)
	{
		ASSERT_VALID (m_pParentCategory);

		POSITION pos = m_pParentCategory->m_lstSubCategories.Find (this);
		if (pos != NULL)
		{
			m_pParentCategory->m_lstSubCategories.RemoveAt (pos);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBCGPPropertySheet

#define UM_AFTERACTIVATEPAGE	(WM_USER + 1001)
#define UM_ADJUSTBUTTONS		(WM_USER + 1002)

IMPLEMENT_DYNAMIC(CBCGPPropertySheet, CPropertySheet)

BOOL CBCGPPropertySheet::m_bUseOldLookInTreeMode = FALSE;

#pragma warning (disable : 4355)

CBCGPPropertySheet::CBCGPPropertySheet() :
	m_Impl (*this)
{
	CommonInit();
}

CBCGPPropertySheet::CBCGPPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage),
	m_Impl (*this)
{
	CommonInit();
}

CBCGPPropertySheet::CBCGPPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage),
	m_Impl (*this)
{
	CommonInit();
}

#pragma warning (default : 4355)

void CBCGPPropertySheet::SetLook (PropSheetLook look, int nNavBarWidth)
{
	ASSERT (GetSafeHwnd() == NULL);

	m_look = look;

	m_nBarWidth = nNavBarWidth;

	if (m_look == PropSheetLook_Wizard)
	{
		m_psh.dwFlags |= PSH_WIZARD;
	}

	if (m_look != PropSheetLook_Tabs)
	{
		EnableStackedTabs (FALSE);
	}
}

CBCGPPropertySheet::~CBCGPPropertySheet()
{
	while (!m_lstTreeCategories.IsEmpty())
	{
		delete m_lstTreeCategories.RemoveHead();
	}
}

void CBCGPPropertySheet::CommonInit()
{
	m_nBarWidth = 100;
	m_nActivePage = -1;
	m_look = PropSheetLook_Tabs;
	m_bIsInSelectTree = FALSE;
	m_bAlphaBlendIcons = FALSE;
	m_nHeaderHeight = 0;
	m_bWasMaximized = FALSE;
	m_bDrawPageFrame = FALSE;
}

BEGIN_MESSAGE_MAP(CBCGPPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CBCGPPropertySheet)
	ON_WM_SYSCOLORCHANGE()
	ON_WM_SETTINGCHANGE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_NCPAINT()
	ON_WM_NCMOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_WM_NCCALCSIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_ERASEBKGND()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
	ON_MESSAGE(UM_AFTERACTIVATEPAGE, OnAfterActivatePage)
	ON_MESSAGE(UM_ADJUSTBUTTONS, OnAdjustButtons)
	ON_NOTIFY(TVN_SELCHANGEDA, idTree, OnSelectTree)
	ON_NOTIFY(TVN_SELCHANGEDW, idTree, OnSelectTree)
	ON_NOTIFY(TVN_GETDISPINFOA, idTree, OnGetDispInfo)
	ON_NOTIFY(TVN_GETDISPINFOW, idTree, OnGetDispInfo)
	ON_LBN_SELCHANGE(idList, OnSelectList)
	ON_REGISTERED_MESSAGE(BCGM_CHANGEVISUALMANAGER, OnChangeVisualManager)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBCGPPropertySheet message handlers

void CBCGPPropertySheet::AddPage(CPropertyPage* pPage)
{
	CPropertySheet::AddPage (pPage);

	CBCGPPropertyPage* pBCGPage = DYNAMIC_DOWNCAST (CBCGPPropertyPage, pPage);
	if (pBCGPage != NULL)
	{
		ASSERT_VALID (pBCGPage);
		pBCGPage->EnableVisualManagerStyle (IsVisualManagerStyle());
	}

	if (GetSafeHwnd() == NULL || m_look == PropSheetLook_Tabs)
	{
		return;
	}

	CTabCtrl* pTab = GetTabControl();
	ASSERT_VALID (pTab);

	InternalAddPage (pTab->GetItemCount() - 1);
}
//****
void CBCGPPropertySheet::InternalAddPage (int nTab)
{
	CTabCtrl* pTab = GetTabControl();
	ASSERT_VALID (pTab);

	TCHAR szTab [256];

	TCITEM item;
	item.mask = TCIF_TEXT;
	item.cchTextMax = 255;
	item.pszText = szTab;

	pTab->GetItem (nTab, &item);

	if (m_wndPane1.GetSafeHwnd() != NULL)
	{
		CBitmap* hIcon = m_Icons.ExtractIcon (nTab);
		m_wndPane1.AddButton (hIcon, szTab, 0, -1, m_bAlphaBlendIcons);
		if (hIcon != NULL)
		{
			::DestroyIcon (hIcon);
		}	
	}

	if (m_wndTree.GetSafeHwnd() != NULL)
	{
		CBCGPPropertyPage* pPage = DYNAMIC_DOWNCAST (CBCGPPropertyPage, GetPage (nTab));
		if (pPage == NULL)
		{
			ASSERT (FALSE);
			return;
		}

		HTREEITEM hParent = NULL;
		if (pPage->m_pCategory != NULL)
		{
			ASSERT_VALID (pPage->m_pCategory);
			hParent = pPage->m_pCategory->m_hTreeItem;
		}

		HTREEITEM hTreeItem = m_wndTree.InsertItem (szTab, 
			I_IMAGECALLBACK, I_IMAGECALLBACK, hParent);
		m_wndTree.SetItemData (hTreeItem, (DWORD_PTR) pPage);
		pPage->m_hTreeNode = hTreeItem;
	}

	if (m_wndList.GetSafeHwnd() != NULL)
	{
		CBCGPPropertyPage* pPage = DYNAMIC_DOWNCAST (CBCGPPropertyPage, GetPage (nTab));
		if (pPage == NULL)
		{
			ASSERT (FALSE);
			return;
		}

		int nIndex = m_wndList.AddString (szTab);
		m_wndList.SetItemData (nIndex, (DWORD_PTR) pPage);
	}

	if (m_wndTab.GetSafeHwnd() != NULL)
	{
		CBCGPPropertyPage* pPage = DYNAMIC_DOWNCAST (CBCGPPropertyPage, GetPage (nTab));
		if (pPage == NULL)
		{
			ASSERT (FALSE);
			return;
		}

		UINT uiImage = m_Icons.GetSafeHandle() == NULL ? (UINT)-1 : nTab;

		m_wndTab.AddTab (pPage, szTab, uiImage, FALSE);
	}
}
//****
void CBCGPPropertySheet::RemovePage(CPropertyPage* pPage)
{
	int nPage = GetPageIndex (pPage);
	ASSERT (nPage >= 0);

	CPropertySheet::RemovePage (nPage);

	if (m_wndPane1.GetSafeHwnd() != NULL)
	{
		m_wndPane1.RemoveButton (nPage);
	}

	if (m_wndTree.GetSafeHwnd() != NULL)
	{
		if (!OnRemoveTreePage (pPage))
		{
			return;
		}
	}

	if (m_wndList.GetSafeHwnd() != NULL)
	{
		m_wndList.DeleteString (FindPageIndexInList (pPage));
	}
}
//****
void CBCGPPropertySheet::RemovePage(int nPage)
{
	if (m_wndTree.GetSafeHwnd() != NULL)
	{
		if (!OnRemoveTreePage (GetPage(nPage)))
		{
			return;
		}
	}
	
	if (m_wndList.GetSafeHwnd() != NULL)
	{
		m_wndList.DeleteString (FindPageIndexInList (GetPage (nPage)));
	}
	
	CPropertySheet::RemovePage (nPage);

	if (m_wndPane1.GetSafeHwnd() != NULL)
	{
		m_wndPane1.RemoveButton (nPage);
	}
}
//****
void CBCGPPropertySheet::RemoveCategory (CBCGPPropSheetCategory* pCategory)
{
	ASSERT_VALID (pCategory);

	POSITION pos = m_lstTreeCategories.Find (pCategory);
	if (pos != NULL)
	{
		m_lstTreeCategories.RemoveAt (pos);
	}

	delete pCategory;
}
//****
CBCGPPropSheetCategory* CBCGPPropertySheet::AddTreeCategory (LPCTSTR lpszLabel, 
	int nIconNum, int nSelectedIconNum, const CBCGPPropSheetCategory* pParentCategory)
{
	ASSERT_VALID (this);
	ASSERT (m_look == PropSheetLook_Tree);

	if (nSelectedIconNum == -1)
	{
		nSelectedIconNum = nIconNum;
	}

	CBCGPPropSheetCategory* pCategory = new CBCGPPropSheetCategory (
		lpszLabel, nIconNum, nSelectedIconNum,
		pParentCategory, *this);

	if (m_wndTree.GetSafeHwnd() != NULL)
	{
		HTREEITEM hParent = NULL;
		if (pParentCategory != NULL)
		{
			hParent = pParentCategory->m_hTreeItem;
		}

		pCategory->m_hTreeItem = m_wndTree.InsertItem (
			lpszLabel, I_IMAGECALLBACK, I_IMAGECALLBACK, hParent);
		m_wndTree.SetItemData (pCategory->m_hTreeItem, (DWORD_PTR) pCategory);
	}

	if (pParentCategory == NULL)
	{
		m_lstTreeCategories.AddTail (pCategory);
	}

	return pCategory;
}
//****
void CBCGPPropertySheet::AddPageToTree (CBCGPPropSheetCategory* pCategory, 
									   CBCGPPropertyPage* pPage, int nIconNum,
									   int nSelIconNum)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pPage);
	ASSERT (m_look == PropSheetLook_Tree);

	if (pCategory != NULL)
	{
		ASSERT_VALID (pCategory);
		pCategory->m_lstPages.AddTail (pPage);
	}

	pPage->m_pCategory = pCategory;
	pPage->m_nIcon = nIconNum;
	pPage->m_nSelIconNum = nSelIconNum;

	CPropertySheet::AddPage (pPage);

	CBCGPPropertyPage* pBCGPage = DYNAMIC_DOWNCAST (CBCGPPropertyPage, pPage);
	if (pBCGPage != NULL)
	{
		ASSERT_VALID (pBCGPage);
		pBCGPage->EnableVisualManagerStyle (IsVisualManagerStyle());
	}

	if (GetSafeHwnd() != NULL)
	{
		CTabCtrl* pTab = GetTabControl();
		ASSERT_VALID (pTab);

		InternalAddPage (pTab->GetItemCount() - 1);
	}
}
//****
BOOL CBCGPPropertySheet::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	m_Impl.m_bHasBorder = (GetStyle() & WS_BORDER) != 0;

	CWnd* pWndNavigator = InitNavigationControl();

	if (IsVisualManagerStyle())
	{
		m_Impl.EnableVisualManagerStyle (TRUE, IsVisualManagerNCArea());
	}

	if (m_wndTab.GetSafeHwnd() != NULL)
	{
		if (m_nHeaderHeight > 0)
		{
			CRect rectWindow;
			GetWindowRect (rectWindow);

			SetWindowPos (NULL, -1, -1, rectWindow.Width(),
				rectWindow.Height() + m_nHeaderHeight,
				SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
		}

		CTabCtrl* pTab = GetTabControl();
		ASSERT_VALID (pTab);

		CRect rectTab;
		pTab->GetWindowRect (rectTab);
		ScreenToClient (rectTab);

		rectTab.InflateRect (2, 0);

		rectTab.b += m_nHeaderHeight;

		m_wndTab.MoveWindow (rectTab);

		pTab->ModifyStyle (WS_TABSTOP, 0);
		pTab->ShowWindow (SW_HIDE);

		if (pTab->GetItemCount() > 0)
		{
			m_wndTab.SetActiveTab (GetActiveIndex());
		}

		if (m_nHeaderHeight > 0)
		{
			ReposButtons();
		}

		return bResult;
	}

	if (pWndNavigator != NULL)
	{
		CTabCtrl* pTab = GetTabControl();
		ASSERT_VALID (pTab);

		pTab->ModifyStyle (WS_TABSTOP, 0);

		CRect rectTabItem;
		pTab->GetItemRect (0, rectTabItem);
		pTab->MapWindowPoints (this, &rectTabItem);

		const int nVertMargin = 5;
		const int nTabsHeight = rectTabItem.Height() + nVertMargin;

		CRect rectClient;
		GetClientRect (rectClient);

		SetWindowPos (NULL, -1, -1, rectClient.Width() + m_nBarWidth,
			rectClient.Height() - nTabsHeight + 3 * nVertMargin + m_nHeaderHeight,
			SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);
		
		GetClientRect (rectClient);
		pTab->MoveWindow (m_nBarWidth, -nTabsHeight, rectClient.r, rectClient.b - 2 * nVertMargin);

		if (IsVisualManagerStyle())
		{
			pTab->ShowWindow (SW_HIDE);
			m_bDrawPageFrame = TRUE;
		}

		CRect rectTab;
		pTab->GetWindowRect (rectTab);
		ScreenToClient (rectTab);

		CRect rectNavigator = rectClient;
		rectNavigator.r = rectNavigator.l + m_nBarWidth;
		rectNavigator.b = rectTab.b;
		rectNavigator.DeflateRect (1, 1);

		if (m_look == PropSheetLook_List)
		{
			rectNavigator.b--;
		}

		if (m_look == PropSheetLook_Tree && IsVisualManagerStyle())
		{
			rectNavigator.b--;
			rectNavigator.DeflateRect (1, 1);
		}
		
		if (m_look == PropSheetLook_OutlookBar && IsVisualManagerStyle())
		{
			rectNavigator.b--;
		}
		
		pWndNavigator->SetWindowPos (&wndTop, 
							rectNavigator.l, rectNavigator.t,
							rectNavigator.Width(), 
							rectNavigator.Height(),
							SWP_NOACTIVATE);

		SetActivePage (GetActivePage());

		ReposButtons();
	}
	else if (m_nHeaderHeight > 0 && (m_psh.dwFlags & PSH_WIZARD) == 0)
	{
		CRect rectWindow;
		GetWindowRect (rectWindow);

		SetWindowPos (NULL, -1, -1, rectWindow.Width(),
			rectWindow.Height() + m_nHeaderHeight,
			SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);

		CTabCtrl* pTab = GetTabControl();
		ASSERT_VALID (pTab);

		CRect rectTab;
		pTab->GetWindowRect (rectTab);

		pTab->SetWindowPos (NULL, -1, -1, rectTab.Width(),
			rectTab.Height() + m_nHeaderHeight,
			SWP_NOZORDER | SWP_NOMOVE | SWP_NOACTIVATE);

		SetActivePage (GetActivePage());

		ReposButtons();
	}

	return bResult;
}
//****
void CBCGPPropertySheet::ReposButtons()
{
	const BOOL bIsRTL = (GetExStyle() & WS_EX_LAYOUTRTL);
	const int nHorzMargin = 5;
	const int nVertMargin = 5;

	CRect rectClient;
	GetClientRect (rectClient);

	int ids[] = { IDOK, IDCANCEL, ID_APPLY_NOW, IDHELP	};

	int nTotalButtonsWidth = 0;

	for (int iStep = 0; iStep < (bIsRTL ? 1 : 2); iStep++)
	{
		for (int i = 0; i < sizeof (ids) / sizeof (ids [0]); i++)
		{
			CWnd* pButton = GetDlgItem (ids[i]);

			if (pButton != NULL)
			{
				if (ids [i] == IDHELP && (m_psh.dwFlags & PSH_HASHELP) == 0)
				{
					continue;
				}

				if (ids [i] == ID_APPLY_NOW && (m_psh.dwFlags & PSH_NOAPPLYNOW))
				{
					continue;
				}

				CRect rectButton;
				pButton->GetWindowRect (rectButton);
				ScreenToClient (rectButton);

				if (iStep == 0)
				{
					// Align buttons at the bottom
					pButton->SetWindowPos (&wndTop, rectButton.l, 
						rectClient.b - rectButton.Height() - nVertMargin, 
						-1, -1, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);

					nTotalButtonsWidth = rectButton.r;
				}
				else
				{
					// Right align the buttons
					pButton->SetWindowPos (&wndTop, 
						rectButton.l + rectClient.r - nTotalButtonsWidth - nHorzMargin,
						rectButton.t,
						-1, -1, SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOZORDER);
				}
			}
		}
	}
}
//****
CWnd* CBCGPPropertySheet::InitNavigationControl()
{
	ASSERT_VALID (this);

	if (m_psh.dwFlags & PSH_WIZARD)
	{
		return NULL;
	}

	CTabCtrl* pTab = GetTabControl();
	ASSERT_VALID (pTab);

	if (m_look == PropSheetLook_OutlookBar)
	{
		DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_ALIGN_LEFT;
		DWORD dwBCGStyle = 0;
		m_wndOutlookBar.Create (_T(""), this, CRect (0, 0, 100, 100), 
			AFX_IDW_TOOLBAR, dwStyle, dwBCGStyle);

#ifndef _BCGSUITE_
		CBCGPBaseTabWnd* pWndTab = m_wndOutlookBar.GetUnderlinedWindow();
#else
		CBCGPBaseTabWnd* pWndTab = m_wndOutlookBar.GetUnderlyingWindow();
#endif

		ASSERT_VALID (pWndTab);

		pWndTab->HideSingleTab();

		m_wndPane1.Create (&m_wndOutlookBar, dwDefaultToolbarStyle, 1);
		m_wndPane1.m_pParent = this;
		m_wndOutlookBar.AddTab (&m_wndPane1);

		m_wndPane1.EnableTextLabels (TRUE);
		m_wndPane1.SetOwner (this);

		ASSERT (m_Icons.IsValid());
		ASSERT (m_Icons.GetImageCount() >= pTab->GetItemCount());

		for (int nTab = 0; nTab < pTab->GetItemCount(); nTab++)
		{
			InternalAddPage (nTab);
		}

		return &m_wndOutlookBar;
	}

	if (m_look == PropSheetLook_Tree)
	{
		CRect rectDummy (0, 0, 0, 0);
		
		DWORD dwTreeStyle =	WS_CHILD | WS_VISIBLE;
		
		if (!m_bUseOldLookInTreeMode)
		{
			dwTreeStyle |= 0x0200 /* TVS_TRACKSELECT */ | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | TVS_SHOWSELALWAYS;
		}

		m_wndTree.Create (dwTreeStyle, rectDummy, this, (UINT) idTree);

		if (!IsVisualManagerStyle())
		{
			m_wndTree.ModifyStyleEx (0, WS_EX_CLIENTEDGE);
		}

		if (m_Icons.IsValid())
		{
			m_wndTree.SetImageList (&m_Icons, TVSIL_NORMAL);
			m_wndTree.SetImageList (&m_Icons, TVSIL_STATE);
		}

		//----------------
		// Add categories:
		//----------------
		for (POSITION pos = m_lstTreeCategories.GetHeadPosition(); pos != NULL;)
		{
			AddCategoryToTree (m_lstTreeCategories.GetNext (pos));
		}

		//-----------
		// Add pages:
		//-----------
		for (int nTab = 0; nTab < pTab->GetItemCount(); nTab++)
		{
			InternalAddPage (nTab);
		}

		return &m_wndTree;
	}

	if (m_look == PropSheetLook_List)
	{
		CRect rectDummy (0, 0, 0, 0);
		const DWORD dwListStyle = 
			LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | LBS_NOINTEGRALHEIGHT | WS_CHILD | WS_VISIBLE | LBS_NOTIFY | WS_TABSTOP;
		m_wndList.Create (dwListStyle, rectDummy, this, (UINT) idList);
		m_wndList.m_bVisualManagerStyle = TRUE;

		m_wndList.ModifyStyleEx (0, WS_EX_CLIENTEDGE);

		//-----------
		// Add pages:
		//-----------
		for (int nTab = 0; nTab < pTab->GetItemCount(); nTab++)
		{
			InternalAddPage (nTab);
		}

		return &m_wndList;
	}

	if (m_look == PropSheetLook_OneNoteTabs || IsVisualManagerStyle())
	{
		const int nActiveTab = GetActiveIndex();

		CRect rectDummy (0, 0, 0, 0);

		m_wndTab.Create (
			m_look == PropSheetLook_OneNoteTabs ? CBCGPTabWnd::STYLE_3D_ONENOTE : CBCGPTabWnd::STYLE_3D,
			rectDummy, this, (UINT) idTab, CBCGPTabWnd::LOCATION_TOP, FALSE);

		m_wndTab.m_pParent = this;
		m_wndTab.EnableTabSwap (FALSE);
		m_wndTab.AutoDestroyWindow (FALSE);

		if (m_Icons.IsValid())
		{
			ASSERT (m_Icons.GetImageCount() >= pTab->GetItemCount());
			m_wndTab.SetImageList (m_Icons.GetSafeHandle());
		}

		for (int nTab = 0; nTab < pTab->GetItemCount(); nTab++)
		{
			InternalAddPage (nTab);
		}

		SetActivePage (nActiveTab);
		return &m_wndTab;
	}

	if (m_look == PropSheetLook_Tabs)
	{
		if (m_Icons.IsValid())
		{
			ASSERT (m_Icons.GetImageCount() >= pTab->GetItemCount());
			pTab->SetImageList(&m_Icons);

			TCITEM tci;
			::ZeroMemory(&tci, sizeof(tci));
			tci.mask = TCIF_IMAGE;

			for (int nTab = 0; nTab < pTab->GetItemCount(); nTab++)
			{
				tci.iImage = nTab;
				pTab->SetItem (nTab, &tci);
			}
		}
	}

	return NULL;
}
//****
void CBCGPPropertySheet::SetIconsList (HIMAGELIST hIcons)
{
	ASSERT_VALID(this);
	ASSERT (hIcons != NULL);
	ASSERT (m_Icons.GetSafeHandle() == NULL);

	m_Icons.Create (CImageList::FromHandle (hIcons));
}
//****
void CBCGPPropertySheet::AddCategoryToTree (CBCGPPropSheetCategory* pCategory)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pCategory);
	ASSERT (m_look == PropSheetLook_Tree);

	HTREEITEM hParent = NULL;
	if (pCategory->m_pParentCategory != NULL)
	{
		hParent = pCategory->m_pParentCategory->m_hTreeItem;
	}

	pCategory->m_hTreeItem = m_wndTree.InsertItem (pCategory->m_strName, 
		I_IMAGECALLBACK, I_IMAGECALLBACK, hParent);
	m_wndTree.SetItemData (pCategory->m_hTreeItem, (DWORD_PTR) pCategory);

	for (POSITION pos = pCategory->m_lstSubCategories.GetHeadPosition(); pos != NULL;)
	{
		AddCategoryToTree (pCategory->m_lstSubCategories.GetNext (pos));
	}
}
//****
BOOL CBCGPPropertySheet::SetIconsList (UINT uiImageListResID, int cx,
							  COLORREF clrTransparent)
{
	ASSERT_VALID(this);

	LPCTSTR lpszResourceName = MAKEINTRESOURCE (uiImageListResID);
	ASSERT(lpszResourceName != NULL);

	CBitmap* hbmp = NULL;

	//-----------------------------
	// Try to load PNG image first:
	//-----------------------------
	CBCGPPngImage pngImage;
	if (pngImage.Load (lpszResourceName))
	{
		hbmp = (CBitmap*) pngImage.Detach();
	}
	else
	{
		hbmp = (CBitmap*) ::LoadImage (
			AfxGetResourceHandle(),
			lpszResourceName,
			IMAGE_BITMAP,
			0, 0,
			LR_CREATEDIBSECTION);
	}

	if (hbmp == NULL)
	{
		TRACE(_T("Can't load image: %x\n"), uiImageListResID);
		return FALSE;
	}

	CImageList icons;
	m_bAlphaBlendIcons = FALSE;

	BITMAP bmpObj;
	::GetObject (hbmp, sizeof (BITMAP), &bmpObj);

	UINT nFlags = (clrTransparent == (COLORREF) -1) ? 0 : ILC_MASK;

	switch (bmpObj.bmBitsPixel)
	{
	case 4:
	default:
		nFlags |= ILC_COLOR4;
		break;

	case 8:
		nFlags |= ILC_COLOR8;
		break;

	case 16:
		nFlags |= ILC_COLOR16;
		break;

	case 24:
		nFlags |= ILC_COLOR24;
		break;

	case 32:
		nFlags |= ILC_COLOR32;
		m_bAlphaBlendIcons = TRUE;
		break;
	}

	icons.Create (cx, bmpObj.bmHeight, nFlags, 0, 0);
	icons.Add (CBitmap::FromHandle (hbmp), clrTransparent);

	SetIconsList (icons);

	::DeleteObject (hbmp);
	return TRUE;
}
//****
void CBCGPPropertySheet::OnActivatePage (CPropertyPage* pPage)
{
	ASSERT_VALID (this);
	ASSERT_VALID (pPage);

	if (m_wndPane1.GetSafeHwnd() != NULL)
	{
		int nPage = GetPageIndex (pPage);
		ASSERT (nPage >= 0);

		if (m_nActivePage >= 0)
		{
			m_wndPane1.SetButtonStyle (m_nActivePage, 0);
		}

		m_nActivePage = nPage;

		PostMessage (UM_AFTERACTIVATEPAGE);
	}

	if (m_wndTree.GetSafeHwnd() != NULL)
	{
		CBCGPPropertyPage* pBCGPropPage = DYNAMIC_DOWNCAST (CBCGPPropertyPage, pPage);
		if (pBCGPropPage != NULL)
		{
			if (!m_bIsInSelectTree)
			{
				m_wndTree.SelectItem (pBCGPropPage->m_hTreeNode);
			}

			m_wndTree.EnsureVisible (pBCGPropPage->m_hTreeNode);
		}
	}

	if (m_wndList.GetSafeHwnd() != NULL)
	{
		int nIdex = FindPageIndexInList (pPage);

		m_wndList.SetCurSel (nIdex);
		PostMessage (UM_AFTERACTIVATEPAGE);
	}

	if (m_wndTab.GetSafeHwnd() != NULL)
	{
		const int nTab = GetPageIndex (pPage);

		m_wndTab.SetActiveTab (nTab);
		m_wndTab.EnsureVisible (nTab);
	}

	if (m_psh.dwFlags & PSH_WIZARD)
	{
		AdjustButtons();
	}
}
//****
LRESULT CBCGPPropertySheet::OnAfterActivatePage(WPARAM,LPARAM)
{
	ASSERT_VALID (this);

	if (m_nActivePage >= 0)
	{
		if (m_wndPane1.GetSafeHwnd() != NULL)
		{
			m_wndPane1.SetButtonStyle (m_nActivePage, TBBS_CHECKED);
			m_wndPane1.EnsureVisible (m_nActivePage);
		}
	}

	if (m_wndList.GetSafeHwnd() != NULL)
	{
		m_wndList.RedrawWindow();
	}

	return 0;
}
//****
void CBCGPPropertySheet::OnSelectTree(NMHDR* /*pNMHDR*/, LRESULT* pResult) 
{
	*pResult = 0;

	HTREEITEM hTreeItem = m_wndTree.GetSelectedItem();
	if (hTreeItem == NULL)
	{
		return;
	}

	CBCGPPropSheetCategory* pNewCategory = NULL;
	CBCGPPropSheetCategory* pOldCategory = NULL;

	CBCGPPropertyPage* pCurrPage = DYNAMIC_DOWNCAST (CBCGPPropertyPage,
		GetActivePage());
	if (pCurrPage != NULL)
	{
		ASSERT_VALID (pCurrPage);
		pOldCategory = pCurrPage->m_pCategory;
	}

	m_bIsInSelectTree = TRUE;

	CBCGPPropertyPage* pPage = DYNAMIC_DOWNCAST (CBCGPPropertyPage,
		(CObject*) m_wndTree.GetItemData (hTreeItem));

	if (pPage == pCurrPage)
	{
		m_bIsInSelectTree = FALSE;
		return;
	}

	if (pPage != NULL)
	{
		CBCGPPropertyPage* pPrevPage = DYNAMIC_DOWNCAST (CBCGPPropertyPage, GetActivePage());

		ASSERT_VALID (pPage);
		if (!SetActivePage (pPage))
		{
		   m_wndTree.SendMessage (TVM_SELECTITEM, (WPARAM)TVGN_CARET,  (LPARAM)pCurrPage->m_hTreeNode);
		   m_bIsInSelectTree = FALSE;
		   return;
		}

		pNewCategory = pPage->m_pCategory;
		if (pNewCategory != NULL)
		{
			HTREEITEM hLastSelectedItem = hTreeItem;

			for (CBCGPPropSheetCategory* pCategory = pNewCategory;
				pCategory != NULL; 
				pCategory = pCategory->m_pParentCategory)
			{
				pCategory->m_hLastSelectedItem = hLastSelectedItem;
				hLastSelectedItem = pCategory->m_hTreeItem;
			}
		}

		if (pPrevPage != NULL)
		{
			ASSERT_VALID (pPrevPage);

			CRect rectItem;
			m_wndTree.GetItemRect (pPrevPage->m_hTreeNode, rectItem, FALSE);
			m_wndTree.InvalidateRect (rectItem);
		}
	}
	else
	{
		CBCGPPropSheetCategory* pCategory = DYNAMIC_DOWNCAST (CBCGPPropSheetCategory,
			(CObject*) m_wndTree.GetItemData (hTreeItem));
		if (pCategory != NULL)
		{
			ASSERT_VALID (pCategory);

			BOOL bIsPageSelected = FALSE;

			while (pCategory->m_hLastSelectedItem != NULL && !bIsPageSelected)
			{
				CBCGPPropSheetCategory* pChildCategory = DYNAMIC_DOWNCAST (CBCGPPropSheetCategory,
					(CObject*) m_wndTree.GetItemData (pCategory->m_hLastSelectedItem));
				if (pChildCategory == NULL)
				{
					CBCGPPropertyPage* pPage = DYNAMIC_DOWNCAST (CBCGPPropertyPage,
						(CObject*) m_wndTree.GetItemData (pCategory->m_hLastSelectedItem));
					if (pPage != NULL)
					{
						SetActivePage (pPage);

						CRect rectItem;
						m_wndTree.GetItemRect (pPage->m_hTreeNode, rectItem, FALSE);
						m_wndTree.InvalidateRect (rectItem);

						bIsPageSelected = TRUE;
					}
				}
				else
				{
					pCategory = pChildCategory;
				}
			}

			if (!bIsPageSelected)
			{
				while (!pCategory->m_lstSubCategories.IsEmpty())
				{
					pCategory = pCategory->m_lstSubCategories.GetHead();
					ASSERT_VALID (pCategory);
				}

				if (!pCategory->m_lstPages.IsEmpty())
				{
					pPage = pCategory->m_lstPages.GetHead();
					ASSERT_VALID (pPage);

					SetActivePage (pPage);

					CRect rectItem;
					m_wndTree.GetItemRect (pPage->m_hTreeNode, rectItem, FALSE);
					m_wndTree.InvalidateRect (rectItem);
				}
			}

			pNewCategory = pCategory;
		}
	}

	if (pNewCategory != pOldCategory)
	{
		if (pOldCategory != NULL && m_bUseOldLookInTreeMode)
		{
			ASSERT_VALID (pOldCategory);
			HTREEITEM hItem = pOldCategory->m_hTreeItem;

			do
			{
				m_wndTree.Expand (hItem, TVE_COLLAPSE);
				hItem = m_wndTree.GetParentItem (hItem);
			}
			while (hItem != NULL);
		}

		if (pNewCategory != NULL)
		{
			ASSERT_VALID (pNewCategory);
			HTREEITEM hItem = pNewCategory->m_hTreeItem;

			do
			{
				m_wndTree.Expand (hItem, TVE_EXPAND);
				hItem = m_wndTree.GetParentItem (hItem);
			}
			while (hItem != NULL);
		}
	}

	m_bIsInSelectTree = FALSE;
}
//****
void CBCGPPropertySheet::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTVDISPINFO lptvdi = (LPNMTVDISPINFO) pNMHDR;

	CBCGPPropertyPage* pPage = DYNAMIC_DOWNCAST (CBCGPPropertyPage,
		(CObject*) m_wndTree.GetItemData (lptvdi->item.hItem));
	if (pPage != NULL)
	{
		ASSERT_VALID (pPage);

		if (pPage == GetActivePage())
		{
			lptvdi->item.iImage = pPage->m_nSelIconNum;
			lptvdi->item.iSelectedImage = pPage->m_nSelIconNum;
		}
		else
		{
			lptvdi->item.iImage = pPage->m_nIcon;
			lptvdi->item.iSelectedImage = pPage->m_nIcon;
		}
	}

	CBCGPPropSheetCategory* pCategory = DYNAMIC_DOWNCAST (CBCGPPropSheetCategory,
		(CObject*) m_wndTree.GetItemData (lptvdi->item.hItem));
	if (pCategory != NULL)
	{
		ASSERT_VALID (pCategory);

		if (lptvdi->item.state & TVIS_EXPANDED)
		{
			lptvdi->item.iImage = pCategory->m_nSelectedIcon;
			lptvdi->item.iSelectedImage = pCategory->m_nSelectedIcon;
		}
		else
		{
			lptvdi->item.iImage = pCategory->m_nIcon;
			lptvdi->item.iSelectedImage = pCategory->m_nIcon;
		}
	}

	*pResult = 0;
}
//****
CBCGPTabWnd& CBCGPPropertySheet::GetTab() const
{
	ASSERT_VALID (this);
	ASSERT (m_look == PropSheetLook_OneNoteTabs);

	return (CBCGPTabWnd&) m_wndTab;
}
//****
BOOL CBCGPPropertySheet::PreTranslateMessage(MSG* pMsg) 
{
	if (m_Impl.PreTranslateMessage (pMsg))
	{
		return TRUE;
	}

	switch (pMsg->message)
	{
	case PSM_SETWIZBUTTONS:
	case PSM_SETFINISHTEXT:
		AdjustButtons();
		break;
	}

	return CPropertySheet::PreTranslateMessage(pMsg);
}
//****
BOOL CBCGPPropertySheet::OnRemoveTreePage(CPropertyPage* pPage)
{
	ASSERT (m_look == PropSheetLook_Tree);

	if (pPage == NULL)
	{
		return FALSE;
	}

	CBCGPPropertyPage* pDelPage = DYNAMIC_DOWNCAST (CBCGPPropertyPage, pPage);
	if (pDelPage == NULL)
	{
		ASSERT(!_T("DYNAMIC_DOWNCAST (CBCGPPropertyPage, pPage)"));
		return FALSE;
	}

	ASSERT (pDelPage->m_hTreeNode != NULL);

	BOOL bResult = m_wndTree.DeleteItem (pDelPage->m_hTreeNode);
	ASSERT (pDelPage->m_pCategory != NULL);

	POSITION pos = (pDelPage->m_pCategory->m_lstPages).Find(pDelPage);
	if (pos != NULL)
	{
		(pDelPage->m_pCategory->m_lstPages).RemoveAt(pos);
		bResult = TRUE;
	}

	return bResult;
}
//****
void CBCGPPropertySheet::OnSysColorChange() 
{
	CPropertySheet::OnSysColorChange();
	
	if (AfxGetMainWnd() == this)
	{
		globalData.UpdateSysColors();
	}
}
//****
void CBCGPPropertySheet::OnSettingChange(UINT uFlags, LPCTSTR lpszSection) 
{
	CPropertySheet::OnSettingChange(uFlags, lpszSection);
	
	if (AfxGetMainWnd() == this)
	{
		globalData.OnSettingChange();
	}
}
//****
int CBCGPPropertySheet::FindPageIndexInList (CPropertyPage* pPage)
{
	for (int i = 0; i < m_wndList.GetCount(); i++)
	{
		if ((CPropertyPage*) m_wndList.GetItemData (i) == pPage)
		{
			return i;
		}
	}

	return -1;
}
//****
void CBCGPPropertySheet::OnSelectList()
{
	int nCurSel = m_wndList.GetCurSel();

	if (nCurSel < 0)
	{
		return;
	}

	CPropertyPage* pPage = (CPropertyPage*) m_wndList.GetItemData (nCurSel);
	ASSERT_VALID (pPage);

	SetActivePage (pPage);
	m_wndList.RedrawWindow();
}
//****
void CBCGPPropertySheet::EnablePageHeader (int nHeaderHeight)
{
	ASSERT (GetSafeHwnd() == NULL);

	m_nHeaderHeight = nHeaderHeight;
}
//****
void CBCGPPropertySheet::OnDrawPageHeader (CDC* /*pDC*/, int /*nPage*/, CRect /*rectHeader*/)
{
}
//****
void CBCGPPropertySheet::EnableVisualManagerStyle (BOOL bEnable, BOOL bNCArea)
{
	ASSERT_VALID (this);

	m_Impl.EnableVisualManagerStyle (bEnable, bEnable && bNCArea);

	for (int i = 0; i < GetPageCount(); i++)
	{
		CBCGPPropertyPage* pBCGPage = DYNAMIC_DOWNCAST (CBCGPPropertyPage, GetPage (i));
		if (pBCGPage != NULL)
		{
			ASSERT_VALID (pBCGPage);
			pBCGPage->EnableVisualManagerStyle (bEnable);
		}
	}
}
//****
void CBCGPPropertySheet::OnDestroy() 
{
	m_Impl.OnDestroy();
	CPropertySheet::OnDestroy();
}
//****
void CBCGPPropertySheet::OnSize(UINT nType, int cx, int cy) 
{
	BOOL bIsMinimized = (nType == SIZE_MINIMIZED);

	if (m_Impl.IsOwnerDrawCaption())
	{
		CRect rectWindow;
		GetWindowRect (rectWindow);

		WINDOWPOS wndpos;
		wndpos.flags = SWP_FRAMECHANGED;
		wndpos.x     = rectWindow.l;
		wndpos.y     = rectWindow.t;
		wndpos.w    = rectWindow.Width();
		wndpos.h    = rectWindow.Height();

		m_Impl.OnWindowPosChanged (&wndpos);
	}

	m_Impl.UpdateCaption();

	if (!bIsMinimized && nType != SIZE_MAXIMIZED && !m_bWasMaximized)
	{
		return;
	}

	CPropertySheet::OnSize(nType, cx, cy);

	m_bWasMaximized = (nType == SIZE_MAXIMIZED);
}
//****
void CBCGPPropertySheet::OnNcPaint() 
{
	if (!m_Impl.OnNcPaint())
	{
		Default();
	}
}
//****
void CBCGPPropertySheet::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	m_Impl.OnNcMouseMove (nHitTest, point);
	CPropertySheet::OnNcMouseMove(nHitTest, point);
}
//****
void CBCGPPropertySheet::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_Impl.OnLButtonUp (point);
	CPropertySheet::OnLButtonUp(nFlags, point);
}
//****
void CBCGPPropertySheet::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_Impl.OnLButtonDown (point);
	CPropertySheet::OnLButtonDown(nFlags, point);
}
//****
BCGNcHitTestType CBCGPPropertySheet::OnNcHitTest(CPoint point) 
{
	BCGNcHitTestType nHit = m_Impl.OnNcHitTest (point);
	if (nHit != HTNOWHERE)
	{
		return nHit;
	}

	return CPropertySheet::OnNcHitTest(point);
}
//****
void CBCGPPropertySheet::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
	if (!m_Impl.OnNcCalcSize (bCalcValidRects, lpncsp))
	{
		CPropertySheet::OnNcCalcSize(bCalcValidRects, lpncsp);
	}
}
//****
void CBCGPPropertySheet::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_Impl.OnMouseMove (point);
	CPropertySheet::OnMouseMove(nFlags, point);
}
//****
void CBCGPPropertySheet::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	if ((lpwndpos->flags & SWP_FRAMECHANGED) == SWP_FRAMECHANGED)
	{
		m_Impl.OnWindowPosChanged (lpwndpos);
	}

	CPropertySheet::OnWindowPosChanged(lpwndpos);
}
//****
LRESULT CBCGPPropertySheet::OnChangeVisualManager (WPARAM, LPARAM)
{
	m_Impl.OnChangeVisualManager();
	return 0;
}
//****
BOOL CBCGPPropertySheet::OnEraseBkgnd(CDC* pDC) 
{
	CRect rectClient;
	GetClientRect (rectClient);

	if (!IsVisualManagerStyle() || 
		!CBCGPVisualManager::GetInstance()->OnFillDialog (pDC, this, rectClient))
	{
		return CPropertySheet::OnEraseBkgnd(pDC);
	}

	if (m_bDrawPageFrame)
	{
		CWnd* pPage = GetActivePage();
		if (pPage->GetSafeHwnd() != NULL)
		{
			CRect rectFrame;
			pPage->GetWindowRect (rectFrame);
			ScreenToClient (&rectFrame);

			CBCGPPenSelector pen (*pDC, globalData.clrBarShadow);

			pDC->MoveTo (rectClient.l, rectFrame.b + 1);
			pDC->LineTo (rectClient.r, rectFrame.b + 1);
		}

		if (m_look == PropSheetLook_Tree && m_wndTree.GetSafeHwnd() != NULL)
		{
			CRect rectTree;
			m_wndTree.GetWindowRect (rectTree);
			ScreenToClient (&rectTree);

			rectTree.InflateRect (1, 1);

			pDC->Draw3dRect (rectTree, globalData.clrBarShadow, globalData.clrBarShadow);
		}
	}

	return TRUE;
}
//****
void CBCGPPropertySheet::OnApply()
{
	AdjustButtons();
}
//****
LRESULT CBCGPPropertySheet::OnAdjustButtons(WPARAM,LPARAM)
{
	static int arButtons [] = 
	{
		ID_APPLY_NOW,
		ID_WIZBACK,
		ID_WIZNEXT,
		ID_WIZFINISH,
		IDOK,
		IDCANCEL,
		IDHELP
	};

	for (int i = 0; i < sizeof (arButtons) / sizeof (int); i++)
	{
		CBCGPButton* pBtn = DYNAMIC_DOWNCAST (CBCGPButton, GetDlgItem (arButtons [i]));
		if (pBtn->GetSafeHwnd() != NULL)
		{
			ASSERT_VALID (pBtn);

			pBtn->ModifyStyle (BS_DEFPUSHBUTTON, BS_OWNERDRAW);
			pBtn->RedrawWindow();
		}
	}

	RedrawWindow (NULL, NULL, RDW_FRAME | RDW_UPDATENOW);
	return 0;
}
//****
void CBCGPPropertySheet::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	m_Impl.OnGetMinMaxInfo (lpMMI);
	CPropertySheet::OnGetMinMaxInfo(lpMMI);
}
//****
LRESULT CBCGPPropertySheet::OnSetText(WPARAM, LPARAM) 
{
	LRESULT	lRes = Default();

	if (lRes && IsVisualManagerStyle() && IsVisualManagerNCArea())
	{
		RedrawWindow (NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW);
	}

	return lRes;
}
//****
void CBCGPPropertySheet::AdjustButtons()
{
	if (IsVisualManagerStyle())
	{
		PostMessage (UM_ADJUSTBUTTONS);
	}
}
