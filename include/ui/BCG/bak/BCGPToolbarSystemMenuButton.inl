
// BCGToolbarSystemMenuButton.cpp: implementation of the CBCGPToolbarSystemMenuButton class.
//
//////////////////////////////////////////////////////////////////////

#include <afxpriv.h>
#include "BCGPToolBar.h"
#include "BCGPToolbarSystemMenuButton.h"

IMPLEMENT_SERIAL(CBCGPToolbarSystemMenuButton, CBCGPToolbarMenuButton, VERSIONABLE_SCHEMA | 1)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPToolbarSystemMenuButton::CBCGPToolbarSystemMenuButton()
{
	m_hSysMenuIcon = NULL;
	m_hSystemMenu = NULL;
}
//****
CBCGPToolbarSystemMenuButton::CBCGPToolbarSystemMenuButton (HMENU hSystemMenu, CBitmap* hSystemIcon) :
	CBCGPToolbarMenuButton (0, hSystemMenu, -1)
{
	m_hSysMenuIcon = hSystemIcon;
	m_hSystemMenu = hSystemMenu;
}
//****
CBCGPToolbarSystemMenuButton::~CBCGPToolbarSystemMenuButton()
{
}
//****
void CBCGPToolbarSystemMenuButton::CopyFrom (const CBCGPToolbarButton& s)
{
	CBCGPToolbarMenuButton::CopyFrom (s);

	const CBCGPToolbarSystemMenuButton& src = (const CBCGPToolbarSystemMenuButton&) s;

	m_hSysMenuIcon = src.m_hSysMenuIcon;
	m_hSystemMenu = src.m_hSystemMenu;
}
//****
ISIZE CBCGPToolbarSystemMenuButton::OnCalculateSize (CDC* /*pDC*/, const CSize& sizeDefault, 
													BOOL /*bHorz*/)
{
	return CSize (::GetSystemMetrics (SM_CXMENUSIZE), sizeDefault.h);
}
//****
void CBCGPToolbarSystemMenuButton::OnDraw (CDC* pDC, const CRect& rect, 
					CBCGPToolBarImages* /*pImages*/,
					BOOL /*bHorz*/, BOOL /*bCustomizeMode*/,
					BOOL /*bHighlight*/,
					BOOL /*bDrawBorder*/, BOOL /*bGrayDisabledButtons*/)
{
	if (m_hSysMenuIcon != NULL)
	{

		CSize size (min (::GetSystemMetrics (SM_CXSMICON), ::GetSystemMetrics (SM_CXMENUSIZE)),
					min (::GetSystemMetrics (SM_CYSMICON), ::GetSystemMetrics (SM_CYMENUSIZE)));
		
		int iOffset = (rect.Height() - size.h) / 2;
		::DrawIconEx (*pDC, rect.l, rect.t + iOffset, m_hSysMenuIcon,
			size.w, size.h,
			0, NULL, DI_NORMAL);
	}
}
//****
void CBCGPToolbarSystemMenuButton::OnDblClick (CWnd* pWnd)
{
	if (CBCGPToolBar::IsCustomizeMode())
	{
		return;
	}

	ASSERT (pWnd != NULL);

	//////////////////////////////////////////////
	// Make sure to close the popup menu and
	// find the MDI frame correctly.
	//--------------------------------------------
	OnCancelMode();

	CFrameWnd* pParentFrame = BCGPGetParentFrame (pWnd);
	if(pParentFrame != NULL && pParentFrame->IsKindOf (RUNTIME_CLASS (CMiniDockFrameWnd)))
	{
		pParentFrame = (CFrameWnd*) pParentFrame->GetParent();
	}

	CMDIFrameWnd* pMDIFrame = 
		DYNAMIC_DOWNCAST (CMDIFrameWnd, pParentFrame);

	if (pMDIFrame != NULL)
	{
		CMDIChildWnd* pChild = pMDIFrame->MDIGetActive();
		ASSERT_VALID (pChild);

		BOOL bCloseIsDisabled = FALSE;

		CMenu* pSysMenu = pChild->GetSystemMenu (FALSE);
		if (pSysMenu != NULL)
		{
			MENUITEMINFO menuInfo;
			ZeroMemory(&menuInfo,sizeof(MENUITEMINFO));
			menuInfo.cbSize = sizeof(MENUITEMINFO);
			menuInfo.fMask = MIIM_STATE;

			pSysMenu->GetMenuItemInfo (SC_CLOSE, &menuInfo);
			bCloseIsDisabled =	((menuInfo.fState & MFS_GRAYED) || 
								(menuInfo.fState & MFS_DISABLED));
		}

		if (!bCloseIsDisabled)
		{
			pChild->SendMessage (WM_SYSCOMMAND, SC_CLOSE);
		}
	}
	//--------------------------------------------
	//////////////////////////////////////////////
}
//****
void CBCGPToolbarSystemMenuButton::CreateFromMenu (HMENU hMenu)
{
	m_hSystemMenu = hMenu;
}
//****
HMENU CBCGPToolbarSystemMenuButton::CreateMenu() const
{
	ASSERT (m_hSystemMenu != NULL);

	HMENU hMenu = CBCGPToolbarMenuButton::CreateMenu();
	if (hMenu == NULL)
	{
		return NULL;
	}

	//---------------------------------------------------------------------
	// System menu don't produce updating command statuses via the
	// standard MFC idle command targeting. So, we should enable/disable
	// system menu items according to the standard system menu status:
	//---------------------------------------------------------------------
	CMenu* pMenu = CMenu::FromHandle (hMenu);
	ASSERT_VALID (pMenu);

	CMenu* pSysMenu = CMenu::FromHandle (m_hSystemMenu);
	ASSERT_VALID (pSysMenu);

	int iCount = (int) pSysMenu->GetMenuItemCount();
	for (int i = 0; i < iCount; i ++)
	{
		UINT uiState = pSysMenu->GetMenuState (i, MF_BYPOSITION);
		UINT uiCmd = pSysMenu->GetMenuItemID (i);

		if (uiState & MF_CHECKED)
		{
			pMenu->CheckMenuItem (uiCmd, MF_CHECKED);
		}

		if (uiState & MF_DISABLED)
		{
			pMenu->EnableMenuItem (uiCmd, MF_DISABLED);
		}

		if (uiState & MF_GRAYED)
		{
			pMenu->EnableMenuItem (uiCmd, MF_GRAYED);
		}
	}

	return hMenu;
}
//****
void CBCGPToolbarSystemMenuButton::OnCancelMode()
{
	if (m_pPopupMenu != NULL && ::IsWindow (m_pPopupMenu->m_hWnd))
	{
		if (m_pPopupMenu->InCommand())
		{
			return;
		}

		m_pPopupMenu->SaveState();
		m_pPopupMenu->m_bAutoDestroyParent = FALSE;
		m_pPopupMenu->CloseMenu();
	}

	m_pPopupMenu = NULL;
	m_bToBeClosed = FALSE;
}
//****
void CBCGPToolbarSystemMenuButton::OnAfterCreatePopupMenu()
{
	if (m_pPopupMenu != NULL && ::IsWindow (m_pPopupMenu->m_hWnd))
	{
		CFrameWnd* pParentFrame = BCGCBProGetTopLevelFrame (m_pPopupMenu);
		if(pParentFrame != NULL && pParentFrame->IsKindOf (RUNTIME_CLASS (CMiniDockFrameWnd)))
		{
			pParentFrame = (CFrameWnd*) pParentFrame->GetParent();
		}

		CMDIFrameWnd* pMDIFrame = 
			DYNAMIC_DOWNCAST (CMDIFrameWnd, pParentFrame);

		if (pMDIFrame != NULL)
		{
			CMDIChildWnd* pChild = pMDIFrame->MDIGetActive();
			ASSERT_VALID (pChild);
		
			m_pPopupMenu->SetMessageWnd (pChild);
		}
	}
}
