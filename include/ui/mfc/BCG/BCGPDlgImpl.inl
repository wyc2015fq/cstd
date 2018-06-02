
//
// BCGPDlgImpl.cpp: implementation of the CBCGPDlgImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "multimon.h"
#include "bcgcbpro.h"

#ifndef _BCGSUITE_
#include "BCGPPopupMenu.h"
#include "BCGPToolbarMenuButton.h"
#else
#define FindDestBar	FindDestintationToolBar
#endif

#include "BCGPDialog.h"
#include "BCGPButton.h"
#include "BCGPPropertyPage.h"
#include "BCGPDlgImpl.h"
#include "BCGPSliderCtrl.h"
#include "BCGPProgressCtrl.h"
#include "BCGPGroup.h"
#include "BCGPStatic.h"
#include "BCGPEdit.h"
#include "BCGPComboBox.h"
#include "BCGPVisualManager.h"
#include "BCGPScrollBar.h"
#include "BCGPSpinButtonCtrl.h"

#ifndef _BCGSUITE_
#include "BCGPCaptionButton.h"
#include "BCGPFrameImpl.h"
#endif

#ifndef _BCGSUITE_
#define visualManagerMFC	CBCGPVisualManager::GetInstance ()
#define visualManager		CBCGPVisualManager::GetInstance ()
#else
#define visualManagerMFC	CMFCVisualManager::GetInstance ()
#define visualManager		CBCGPVisualManager::GetInstance ()
#endif

HHOOK CBCGPDlgImpl::m_hookMouse = NULL;
CBCGPDlgImpl* CBCGPDlgImpl::m_pMenuDlgImpl = NULL;

UINT BCGM_ONSETCONTROLAERO = ::RegisterWindowMessage (_T("BCGM_ONSETCONTROLAERO"));
UINT BCGM_ONSETCONTROLVMMODE = ::RegisterWindowMessage (_T("BCGM_ONSETCONTROLVMMODE"));

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPDlgImpl::CBCGPDlgImpl(CWnd& dlg) :
	m_Dlg (dlg),
	m_bVisualManagerStyle (FALSE),
	m_bTransparentStaticCtrls (TRUE),
	m_bVisualManagerNCArea (FALSE),
	m_nHotSysButton (HTNOWHERE),
	m_nHitSysButton (HTNOWHERE),
	m_bWindowPosChanging (FALSE),
	m_bHasBorder (FALSE),
	m_bIsWindowRgn (FALSE)
{
	m_AeroMargins.cxLeftWidth = 0;
	m_AeroMargins.cxRightWidth = 0;
	m_AeroMargins.cyTopHeight = 0;
	m_AeroMargins.cyBottomHeight = 0;

	m_rectRedraw.SetRectEmpty ();
}
//****
CBCGPDlgImpl::~CBCGPDlgImpl()
{
	//------------------------------
	// Clear caption system buttons:
	//------------------------------
	while (!m_lstCaptionSysButtons.IsEmpty ())
	{
		delete m_lstCaptionSysButtons.RemoveHead ();
	}
}
//****
BOOL CBCGPDlgImpl::ProcessMouseClick (POINT pt)
{
	if (!CBCGPToolBar::IsCustomizeMode () &&
		CBCGPPopupMenu::GetActiveMenu() != NULL &&
		::IsWindow (CBCGPPopupMenu::GetActiveMenu()->m_hWnd))
	{
		CBCGPPopupMenu::MENUAREA_TYPE clickArea = CBCGPPopupMenu::GetActiveMenu()->CheckArea (pt);

		if (clickArea == CBCGPPopupMenu::OUTSIDE)
		{
			// Click outside of menu

			//--------------------------------------------
			// Maybe secondary click on the parent button?
			//--------------------------------------------
			CBCGPToolbarMenuButton* pParentButton = 
				CBCGPPopupMenu::GetActiveMenu()->GetParentButton ();
			if (pParentButton != NULL)
			{
				CWnd* pWndParent = pParentButton->GetParentWnd ();
				if (pWndParent != NULL)
				{
					CBCGPPopupMenuBar* pWndParentPopupMenuBar = 
						DYNAMIC_DOWNCAST (CBCGPPopupMenuBar, pWndParent);

					CPoint ptClient = pt;
					pWndParent->ScreenToClient (&ptClient);

					if (pParentButton->Rect ().PtInRect (ptClient))
					{
						//-------------------------------------------------------
						// If user clicks second time on the parent button,
						// we should close an active menu on the toolbar/menubar
						// and leave it on the popup menu:
						//-------------------------------------------------------
						if (pWndParentPopupMenuBar == NULL &&
							!CBCGPPopupMenu::GetActiveMenu()->InCommand ())
						{
							//----------------------------------------
							// Toolbar/menu bar: close an active menu!
							//----------------------------------------
							CBCGPPopupMenu::GetActiveMenu()->SendMessage (WM_CLOSE);
						}

						return TRUE;
					}

					if (pWndParentPopupMenuBar != NULL)
					{
						pWndParentPopupMenuBar->CloseDelayedSubMenu ();
						
						CBCGPPopupMenu* pWndParentPopupMenu = 
							DYNAMIC_DOWNCAST (CBCGPPopupMenu, 
							pWndParentPopupMenuBar->GetParent ());

						if (pWndParentPopupMenu != NULL)
						{
							CBCGPPopupMenu::MENUAREA_TYPE clickAreaParent = 
								pWndParentPopupMenu->CheckArea (pt);

							switch (clickAreaParent)
							{
							case CBCGPPopupMenu::MENU:
							case CBCGPPopupMenu::TEAROFF_CAPTION:
							case CBCGPPopupMenu::LOGO:
								return FALSE;

							case CBCGPPopupMenu::SHADOW_RIGHT:
							case CBCGPPopupMenu::SHADOW_BOTTOM:
								pWndParentPopupMenu->SendMessage (WM_CLOSE);
								m_Dlg.SetFocus ();

								return TRUE;
							}
						}
					}
				}
			}

			if (!CBCGPPopupMenu::GetActiveMenu()->InCommand ())
			{
				CBCGPPopupMenu::GetActiveMenu()->SendMessage (WM_CLOSE);

				CWnd* pWndFocus = CWnd::GetFocus ();
				if (pWndFocus != NULL && pWndFocus->IsKindOf (RUNTIME_CLASS (CBCGPToolBar)))
				{
					m_Dlg.SetFocus ();
				}

				if (clickArea != CBCGPPopupMenu::OUTSIDE)	// Click on shadow
				{
					return TRUE;
				}
			}
		}
		else if (clickArea == CBCGPPopupMenu::SHADOW_RIGHT ||
				clickArea == CBCGPPopupMenu::SHADOW_BOTTOM)
		{
			CBCGPPopupMenu::GetActiveMenu()->SendMessage (WM_CLOSE);
			m_Dlg.SetFocus ();

			return TRUE;
		}
	}

	return FALSE;
}
//****
BOOL CBCGPDlgImpl::ProcessMouseMove (POINT pt)
{
	if (!CBCGPToolBar::IsCustomizeMode () &&
		CBCGPPopupMenu::GetActiveMenu() != NULL)
	{
		CRect rectMenu;
		CBCGPPopupMenu::GetActiveMenu()->GetWindowRect (rectMenu);

		if (rectMenu.PtInRect (pt) ||
			CBCGPPopupMenu::GetActiveMenu()->GetMenuBar ()->FindDestBar (pt) != NULL)
		{
			return FALSE;	// Default processing
		}

		return TRUE;		// Active menu "capturing"
	}

	return FALSE;	// Default processing
}
//****
BOOL CBCGPDlgImpl::PreTranslateMessage(MSG* pMsg) 
{
	switch (pMsg->message)
	{
	case WM_SYSKEYDOWN:
	case WM_CONTEXTMENU:
		if (CBCGPPopupMenu::GetActiveMenu() != NULL &&
			::IsWindow (CBCGPPopupMenu::GetActiveMenu()->m_hWnd) &&
			pMsg->wParam == VK_MENU)
		{
			CBCGPPopupMenu::GetActiveMenu()->SendMessage (WM_CLOSE);
			return TRUE;
		}
		break;

	case WM_SYSKEYUP:
		if (CBCGPPopupMenu::GetActiveMenu() != NULL &&
			::IsWindow (CBCGPPopupMenu::GetActiveMenu()->m_hWnd))
		{
			return TRUE;	// To prevent system menu opening
		}
		break;

	case WM_KEYDOWN:
		//-----------------------------------------
		// Pass keyboard action to the active menu:
		//-----------------------------------------
		if (CBCGPPopupMenu::GetActiveMenu() != NULL &&
			::IsWindow (CBCGPPopupMenu::GetActiveMenu()->m_hWnd))
		{
			CBCGPPopupMenu::GetActiveMenu()->SendMessage (WM_KEYDOWN, (int) pMsg->wParam);
			return TRUE;
		}
		break;

	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
		{
			CPoint pt (BCG_GET_X_LPARAM(pMsg->lParam), BCG_GET_Y_LPARAM(pMsg->lParam));
			CWnd* pWnd = CWnd::FromHandle (pMsg->hwnd);

			if (pWnd != NULL && ::IsWindow (pMsg->hwnd))
			{
				pWnd->ClientToScreen (&pt);
			}

			if (ProcessMouseClick (pt))
			{
				return TRUE;
			}

			if (!::IsWindow (pMsg->hwnd))
			{
				return TRUE;
			}
		}
		break;

	case WM_NCLBUTTONDOWN:
	case WM_NCLBUTTONUP:
	case WM_NCRBUTTONDOWN:
	case WM_NCRBUTTONUP:
	case WM_NCMBUTTONDOWN:
	case WM_NCMBUTTONUP:
		if (ProcessMouseClick (CPoint (BCG_GET_X_LPARAM(pMsg->lParam), BCG_GET_Y_LPARAM(pMsg->lParam))))
		{
			return TRUE;
		}

		if (pMsg->message == WM_NCRBUTTONUP && pMsg->hwnd == m_Dlg.GetSafeHwnd () && IsOwnerDrawCaption ())
		{
			CPoint pt (BCG_GET_X_LPARAM(pMsg->lParam), BCG_GET_Y_LPARAM(pMsg->lParam));

			UINT nHit = OnNcHitTest (pt);

			if (nHit == HTCAPTION || nHit == HTSYSMENU)
			{
				CMenu* pMenu = m_Dlg.GetSystemMenu (FALSE);
				if (pMenu->GetSafeHmenu () != NULL && ::IsMenu (pMenu->GetSafeHmenu ()))
				{
					UINT uiRes = ::TrackPopupMenu (pMenu->GetSafeHmenu(), TPM_LEFTBUTTON | TPM_RETURNCMD, 
						pt.x, pt.y, 0, m_Dlg.GetSafeHwnd (), NULL);

					if (uiRes != 0)
					{
						m_Dlg.SendMessage (WM_SYSCOMMAND, uiRes);
						return TRUE;
					}
				}

			}
		}
		break;

	case WM_MOUSEWHEEL:
		if (CBCGPPopupMenu::GetActiveMenu() != NULL &&
			::IsWindow (CBCGPPopupMenu::GetActiveMenu()->m_hWnd) &&
			CBCGPPopupMenu::GetActiveMenu()->IsScrollable ())
		{
			CBCGPPopupMenu::GetActiveMenu()->SendMessage (WM_MOUSEWHEEL,
				pMsg->wParam, pMsg->lParam);

		}
		break;

	case WM_MOUSEMOVE:
		{
			CPoint pt (BCG_GET_X_LPARAM(pMsg->lParam), BCG_GET_Y_LPARAM(pMsg->lParam));
			CWnd* pWnd = CWnd::FromHandle (pMsg->hwnd);

			if (pWnd != NULL)
			{
				pWnd->ClientToScreen (&pt);
			}

			if (ProcessMouseMove (pt))
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}
//****
LRESULT CALLBACK CBCGPDlgImpl::BCGDlgMouseProc (int nCode, WPARAM wParam, LPARAM lParam)
{
	MOUSEHOOKSTRUCT* lpMS = (MOUSEHOOKSTRUCT*) lParam;
	ASSERT (lpMS != NULL);

	if (m_pMenuDlgImpl != NULL)
	{
		switch (wParam)
		{
		case WM_LBUTTONDOWN:
		case WM_RBUTTONDOWN:
		case WM_MBUTTONDOWN:
		case WM_NCLBUTTONDOWN:
		case WM_NCRBUTTONDOWN:
		case WM_NCMBUTTONDOWN:
			{
				CPoint ptCursor;
				::GetCursorPos (&ptCursor);

				CRect rectWindow;
				m_pMenuDlgImpl->m_Dlg.GetWindowRect (rectWindow);

				if (!rectWindow.PtInRect (ptCursor))
				{
					m_pMenuDlgImpl->ProcessMouseClick (ptCursor);
				}
			}
		}
	}

	return CallNextHookEx (m_hookMouse, nCode, wParam, lParam);
}
//****
void CBCGPDlgImpl::SetActiveMenu (CBCGPPopupMenu* pMenu)
{
#ifndef _BCGSUITE_
	CBCGPPopupMenu::m_pActivePopupMenu = pMenu;
#else
	class CBCGPPopupMenuDummy : public CBCGPPopupMenu
	{
		friend class CBCGPDlgImpl;
	};

	CBCGPPopupMenuDummy::m_pActivePopupMenu = pMenu;

#endif

	if (pMenu != NULL)
	{
		if (m_hookMouse == NULL)
		{
			m_hookMouse = ::SetWindowsHookEx (WH_MOUSE, BCGDlgMouseProc, 
				0, GetCurrentThreadId ());
		}

		m_pMenuDlgImpl = this;
	}
	else 
	{
		if (m_hookMouse != NULL)
		{
			::UnhookWindowsHookEx (m_hookMouse);
			m_hookMouse = NULL;
		}

		m_pMenuDlgImpl = NULL;
	}

}
//****
void CBCGPDlgImpl::OnDestroy ()
{
	for (int i = 0; i < m_arSubclassedCtrls.GetSize (); i++)
	{
		delete m_arSubclassedCtrls [i];
	}

	m_arSubclassedCtrls.RemoveAll ();
	if (m_pMenuDlgImpl != NULL &&
		m_pMenuDlgImpl->m_Dlg.GetSafeHwnd () == m_Dlg.GetSafeHwnd ())
	{
		m_pMenuDlgImpl = NULL;
	}
}
//****
BOOL CBCGPDlgImpl::OnCommand (WPARAM wParam, LPARAM /*lParam*/)
{
	if (HIWORD (wParam) == 1)
	{
		UINT uiCmd = LOWORD (wParam);

		CBCGPToolBar::AddCommandUsage (uiCmd);

		//---------------------------
		// Simmulate ESC keystroke...
		//---------------------------
		if (CBCGPPopupMenu::GetActiveMenu() != NULL &&
			::IsWindow (CBCGPPopupMenu::GetActiveMenu()->m_hWnd))
		{
			CBCGPPopupMenu::GetActiveMenu()->SendMessage (WM_KEYDOWN, VK_ESCAPE);
			return TRUE;
		}

		if (g_pUserToolsManager != NULL &&
			g_pUserToolsManager->InvokeTool (uiCmd))
		{
			return TRUE;
		}
	}

	return FALSE;
}
//****
void CBCGPDlgImpl::OnNcActivate (BOOL& bActive)
{
	//----------------------------------------
	// Stay active if WF_STAYACTIVE bit is on:
	//----------------------------------------
	if (m_Dlg.m_nFlags & WF_STAYACTIVE)
	{
		bActive = TRUE;
	}

	//--------------------------------------------------
	// But do not stay active if the window is disabled:
	//--------------------------------------------------
	if (!m_Dlg.IsWindowEnabled ())
	{
		bActive = FALSE;
	}

	if (IsOwnerDrawCaption ())
	{
		visualManagerMFC->OnNcActivate (&m_Dlg, bActive);
		m_Dlg.RedrawWindow (CRect (0, 0, 0, 0), NULL,
			RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOCHILDREN);
	}
}
//****
void CBCGPDlgImpl::OnActivate(UINT nState, CWnd* pWndOther)
{
	m_Dlg.m_nFlags &= ~WF_STAYACTIVE;

	//--------------------------------------------------
	// Determine if this window should be active or not:
	//--------------------------------------------------
	CWnd* pWndActive = (nState == WA_INACTIVE) ? pWndOther : &m_Dlg;
	if (pWndActive != NULL)
	{
		BOOL bStayActive = (pWndActive->GetSafeHwnd () == m_Dlg.GetSafeHwnd () ||
			pWndActive->SendMessage (WM_FLOATSTATUS, FS_SYNCACTIVE));

		if (bStayActive)
		{
			m_Dlg.m_nFlags |= WF_STAYACTIVE;
		}
	}
	else 
	{
		//------------------------------------------
		// Force painting on our non-client area....
		//------------------------------------------
		m_Dlg.SendMessage (WM_NCPAINT, 1);
	}
}
//****
void CBCGPDlgImpl::EnableVisualManagerStyle (BOOL bEnable, BOOL bNCArea, const CList<UINT,UINT>* plstNonSubclassedItems)
{
	m_bVisualManagerStyle = bEnable;
	m_bVisualManagerNCArea = bNCArea;

	if (m_Dlg.GetSafeHwnd () == NULL)
	{
		return;
	}

	CWnd* pWndChild = m_Dlg.GetWindow (GW_CHILD);

	while (pWndChild != NULL)
	{
		ASSERT_VALID (pWndChild);

		if (plstNonSubclassedItems != NULL && plstNonSubclassedItems->Find (pWndChild->GetDlgCtrlID ()) != NULL)
		{
			pWndChild = pWndChild->GetNextWindow ();
			continue;
		}

		CBCGPButton* pButton = DYNAMIC_DOWNCAST(CBCGPButton, pWndChild);
		if (pButton != NULL)
		{
			ASSERT_VALID (pButton);
			pButton->m_bVisualManagerStyle = m_bVisualManagerStyle;
		}

		if (m_bVisualManagerStyle &&
			CWnd::FromHandlePermanent (pWndChild->GetSafeHwnd ()) == NULL)
		{
			#define MAX_CLASS_NAME		255
			#define STATIC_CLASS		_T("Static")
			#define BUTTON_CLASS		_T("Button")
			#define EDIT_CLASS			_T("Edit")
			#define	COMBOBOX_CLASS		_T("ComboBox")
			#define SCROLLBAR_CLASS		_T("ScrollBar")

			TCHAR lpszClassName [MAX_CLASS_NAME + 1];

			::GetClassName (pWndChild->GetSafeHwnd (), lpszClassName, MAX_CLASS_NAME);
			CString strClass = lpszClassName;

			CWnd* pWndSubclassedCtrl = NULL;

			if (strClass == STATIC_CLASS)
			{
				pWndSubclassedCtrl = new CBCGPStatic;
			}
			else if (strClass == BUTTON_CLASS)
			{
				if ((pWndChild->GetStyle () & 0xF) == BS_GROUPBOX)
				{
					pWndSubclassedCtrl = new CBCGPGroup;
				}
				else
				{
					pWndSubclassedCtrl = new CBCGPButton;
				}
			}
			else if (strClass == PROGRESS_CLASS)
			{
				pWndSubclassedCtrl = new CBCGPProgressCtrl;
			}
			else if (strClass == TRACKBAR_CLASS)
			{
				pWndSubclassedCtrl = new CBCGPSliderCtrl;
			}
			else if (strClass == EDIT_CLASS)
			{
				pWndSubclassedCtrl = new CBCGPEdit;
			}
			else if (strClass == COMBOBOX_CLASS)
			{
				pWndSubclassedCtrl = new CBCGPComboBox;
			}
			else if (strClass == SCROLLBAR_CLASS)
			{
				pWndSubclassedCtrl = new CBCGPScrollBar;
			}
			else if (strClass == UPDOWN_CLASS)
			{
				pWndSubclassedCtrl = new CBCGPSpinButtonCtrl;
			}

			if (pWndSubclassedCtrl != NULL)
			{
				m_arSubclassedCtrls.Add (pWndSubclassedCtrl);
				pWndSubclassedCtrl->SubclassWindow (pWndChild->GetSafeHwnd ());
			}
		}

		pWndChild->SendMessage (BCGM_ONSETCONTROLVMMODE, (WPARAM) bEnable);
		pWndChild = pWndChild->GetNextWindow ();
	}

	OnChangeVisualManager ();

	if (m_Dlg.IsWindowVisible ())
	{
		m_Dlg.RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	}
}
//****
BOOL CBCGPDlgImpl::EnableAero (BCGPMARGINS& margins)
{
	m_AeroMargins = margins;

	if (HasAeroMargins () && !m_bVisualManagerStyle)
	{
		EnableVisualManagerStyle (TRUE);
	}

	if (m_Dlg.GetSafeHwnd () == NULL)
	{
		return TRUE;
	}

	BOOL bRes = globalData.DwmExtendFrameIntoClientArea (m_Dlg.GetSafeHwnd (), &m_AeroMargins);
	BOOL bIsAeroEnabled = globalData.DwmIsCompositionEnabled ();

	CRect rectClient;
	m_Dlg.GetClientRect (rectClient);

	CWnd* pWndChild = m_Dlg.GetWindow (GW_CHILD);

	while (pWndChild != NULL)
	{
		ASSERT_VALID (pWndChild);

		BOOL bGlass = FALSE;

		if (bIsAeroEnabled && HasAeroMargins ())
		{
			CRect rectChild;
			pWndChild->GetWindowRect (&rectChild);
			m_Dlg.ScreenToClient (&rectChild);

			CRect rectInter;

			if (m_AeroMargins.cxLeftWidth != 0)
			{
				CRect rectAero = rectClient;
				rectAero.right = rectAero.left + m_AeroMargins.cxLeftWidth;

				if (rectInter.IntersectRect (rectAero, rectChild))
				{
					bGlass = TRUE;
				}
			}

			if (!bGlass && m_AeroMargins.cxRightWidth != 0)
			{
				CRect rectAero = rectClient;
				rectAero.left = rectAero.right - m_AeroMargins.cxRightWidth;

				if (rectInter.IntersectRect (rectAero, rectChild))
				{
					bGlass = TRUE;
				}
			}

			if (!bGlass && m_AeroMargins.cyTopHeight != 0)
			{
				CRect rectAero = rectClient;
				rectAero.bottom = rectAero.top + m_AeroMargins.cyTopHeight;

				if (rectInter.IntersectRect (rectAero, rectChild))
				{
					bGlass = TRUE;
				}
			}

			if (!bGlass && m_AeroMargins.cyBottomHeight != 0)
			{
				CRect rectAero = rectClient;
				rectAero.top = rectAero.bottom - m_AeroMargins.cyBottomHeight;

				if (rectInter.IntersectRect (rectAero, rectChild))
				{
					bGlass = TRUE;
				}
			}
		}

		pWndChild->SendMessage (BCGM_ONSETCONTROLAERO, (WPARAM) bGlass);
		pWndChild = pWndChild->GetNextWindow ();
	}

	if (m_Dlg.IsWindowVisible ())
	{
		m_Dlg.RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
	}

	return bRes;
}
//****
void CBCGPDlgImpl::GetAeroMargins (BCGPMARGINS& margins) const
{
	margins = m_AeroMargins;
}
//****
BOOL CBCGPDlgImpl::HasAeroMargins () const
{
	return 	m_AeroMargins.cxLeftWidth != 0 ||
			m_AeroMargins.cxRightWidth != 0 ||
			m_AeroMargins.cyTopHeight != 0 ||
			m_AeroMargins.cyBottomHeight != 0;
}
//****
void CBCGPDlgImpl::ClearAeroAreas (CDC* pDC)
{
	if (!HasAeroMargins () || m_Dlg.GetSafeHwnd () == NULL ||
		!globalData.DwmIsCompositionEnabled ())
	{
		return;
	}

	CRect rectClient;
	m_Dlg.GetClientRect (rectClient);

	if (m_AeroMargins.cxLeftWidth != 0)
	{
		CRect rectAero = rectClient;
		rectAero.right = rectAero.left + m_AeroMargins.cxLeftWidth;

		pDC->FillSolidRect (rectAero, RGB (0, 0, 0));
	}

	if (m_AeroMargins.cxRightWidth != 0)
	{
		CRect rectAero = rectClient;
		rectAero.left = rectAero.right - m_AeroMargins.cxRightWidth;

		pDC->FillSolidRect (rectAero, RGB (0, 0, 0));
	}

	if (m_AeroMargins.cyTopHeight != 0)
	{
		CRect rectAero = rectClient;
		rectAero.bottom = rectAero.top + m_AeroMargins.cyTopHeight;

		pDC->FillSolidRect (rectAero, RGB (0, 0, 0));
	}

	if (m_AeroMargins.cyBottomHeight != 0)
	{
		CRect rectAero = rectClient;
		rectAero.top = rectAero.bottom - m_AeroMargins.cyBottomHeight;

		pDC->FillSolidRect (rectAero, RGB (0, 0, 0));
	}
}
//****
void CBCGPDlgImpl::OnDWMCompositionChanged ()
{
	if (!HasAeroMargins () || m_Dlg.GetSafeHwnd () == NULL)
	{
		return;
	}

	EnableAero (m_AeroMargins);
	OnChangeVisualManager ();
	m_Dlg.RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
}
//****
HBRUSH CBCGPDlgImpl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	#define MAX_CLASS_NAME	255
	#define STATIC_CLASS	_T("Static")
	#define BUTTON_CLASS	_T("Button")

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		TCHAR lpszClassName [MAX_CLASS_NAME + 1];

		::GetClassName (pWnd->GetSafeHwnd (), lpszClassName, MAX_CLASS_NAME);
		CString strClass = lpszClassName;

		if (strClass == STATIC_CLASS)
		{
			pDC->SetBkMode(TRANSPARENT);

			if (m_bVisualManagerStyle)
			{
				pDC->SetTextColor (globalData.clrBarText);
			}

			if (m_bTransparentStaticCtrls && (pWnd->GetStyle () & SS_ICON) != SS_ICON)
			{
				return (HBRUSH) ::GetStockObject (HOLLOW_BRUSH);
			}

			return (HBRUSH) visualManager->GetDlgBackBrush (&m_Dlg).GetSafeHandle ();
		}

		if (strClass == BUTTON_CLASS)
		{
			DWORD dwStyle = pWnd->GetStyle ();

			if (dwStyle & BS_GROUPBOX)
			{
				if (m_bVisualManagerStyle)
				{
					pDC->SetTextColor (globalData.clrBarText);
					pDC->SetBkMode(TRANSPARENT);
					return (HBRUSH) visualManager->GetDlgBackBrush (&m_Dlg).GetSafeHandle ();
				}
			}

			if ((dwStyle & BS_CHECKBOX) == 0)
			{
				pDC->SetBkMode(TRANSPARENT);
			}

			return (HBRUSH) ::GetStockObject (HOLLOW_BRUSH);
		}
	}

	return NULL;
}
//****
BOOL CBCGPDlgImpl::OnNcPaint ()
{
#ifndef _BCGSUITE_
	if (globalData.m_bInSettingsChange || !IsOwnerDrawCaption ())
	{
		return FALSE;
	}
#else
	if (!IsOwnerDrawCaption ())
	{
		return FALSE;
	}
#endif

	return visualManagerMFC->OnNcPaint (&m_Dlg,
		m_lstCaptionSysButtons, m_rectRedraw);
}
//****
BOOL CBCGPDlgImpl::OnNcCalcSize(BOOL /*bCalcValidRects*/, NCCALCSIZE_PARAMS FAR* lpncsp)
{
	ASSERT (lpncsp != NULL);

	if ((m_Dlg.GetStyle () & WS_CAPTION) == 0)
	{
		m_bVisualManagerNCArea = FALSE;
	}

	if (IsOwnerDrawCaption ())
	{
		lpncsp->rgrc[0].top += GetCaptionHeight ();
	}

	return (m_Dlg.GetStyle () & WS_MAXIMIZE) == WS_MAXIMIZE && IsOwnerDrawCaption ();
}
//****
CRect CBCGPDlgImpl::GetCaptionRect ()
{
	CSize szSystemBorder (::GetSystemMetrics (SM_CXSIZEFRAME), ::GetSystemMetrics (SM_CYSIZEFRAME));

	if (m_Dlg.IsIconic () || 
		(m_Dlg.GetStyle () & WS_MAXIMIZE) == WS_MAXIMIZE)
	{
		szSystemBorder = CSize (0, 0);
	}
	else if ((m_Dlg.GetStyle () & WS_THICKFRAME) == 0)
	{
		szSystemBorder = CSize (::GetSystemMetrics (SM_CXFIXEDFRAME), ::GetSystemMetrics (SM_CXFIXEDFRAME));
	}
	

	CRect rectWnd;
	m_Dlg.GetWindowRect (&rectWnd);

	m_Dlg.ScreenToClient (&rectWnd);

	int cyOffset = szSystemBorder.cy;
	if (!m_Dlg.IsIconic ())
	{
		cyOffset += GetCaptionHeight ();
	}

	rectWnd.OffsetRect (szSystemBorder.cx, cyOffset);

	CRect rectCaption (	rectWnd.left + szSystemBorder.cx, 
						rectWnd.top + szSystemBorder.cy, 
						rectWnd.right - szSystemBorder.cx, 
						rectWnd.top + szSystemBorder.cy + GetCaptionHeight ());

	if (m_Dlg.IsIconic ())
	{
		rectCaption.top += ::GetSystemMetrics (SM_CYSIZEFRAME);
		rectCaption.right -= ::GetSystemMetrics (SM_CXSIZEFRAME);
	}

	return rectCaption;
}
//****
void CBCGPDlgImpl::UpdateCaption ()
{
	if (!IsOwnerDrawCaption ())
	{
		return;
	}

	if (m_lstCaptionSysButtons.IsEmpty ())
	{
		//------------------------
		// Create caption buttons:
		//------------------------
		const DWORD dwStyle = m_Dlg.GetStyle ();
		HMENU hSysMenu = NULL;
		CMenu* pSysMenu = m_Dlg.GetSystemMenu (FALSE);

		if (pSysMenu != NULL && ::IsMenu (pSysMenu->m_hMenu))
		{
			hSysMenu = pSysMenu->GetSafeHmenu ();
			if (!::IsMenu (hSysMenu) || (m_Dlg.GetStyle () & WS_SYSMENU) == 0)
			{
				hSysMenu = NULL;
			}
		}

		if (hSysMenu != NULL)
		{
			m_lstCaptionSysButtons.AddTail (new CBCGPFrameCaptionButton (HTCLOSE_BCG));

			if ((dwStyle & WS_MAXIMIZEBOX) == WS_MAXIMIZEBOX)
			{
				m_lstCaptionSysButtons.AddTail (new CBCGPFrameCaptionButton (HTMAXBUTTON_BCG));
			}

			if ((dwStyle & WS_MINIMIZEBOX) == WS_MINIMIZEBOX)
			{
				m_lstCaptionSysButtons.AddTail (new CBCGPFrameCaptionButton (HTMINBUTTON_BCG));
			}

#ifndef _BCGSUITE_
			if ((dwStyle & DS_CONTEXTHELP) == DS_CONTEXTHELP)
			{
				if (!m_Dlg.IsKindOf (RUNTIME_CLASS (CPropertySheet)))
				{
					m_lstCaptionSysButtons.AddTail (new CBCGPFrameCaptionButton (HTHELPBUTTON_BCG));
				}
			}
#endif
		}
	}

	CRect rectCaption = GetCaptionRect ();

#ifndef _BCGSUITE_
    if (!visualManagerMFC->OnUpdateNcButtons(&m_Dlg, m_lstCaptionSysButtons, rectCaption))
#endif
    {
	    CSize sizeButton = visualManagerMFC->GetNcBtnSize (FALSE);
	    sizeButton.cy = min (sizeButton.cy, rectCaption.Height () - 2);

	    int x = rectCaption.right - sizeButton.cx;
	    int y = rectCaption.top + max (0, (rectCaption.Height () - sizeButton.cy) / 2);

	    for (POSITION pos = m_lstCaptionSysButtons.GetHeadPosition (); pos != NULL;)
	    {
		    CBCGPFrameCaptionButton* pButton = (CBCGPFrameCaptionButton*)
			    m_lstCaptionSysButtons.GetNext (pos);
		    ASSERT_VALID (pButton);

		    pButton->SetRect (CRect (CPoint (x, y), sizeButton));

		    x -= sizeButton.cx;
	    }
    }

#ifndef _BCGSUITE_
    m_Dlg.SendMessage (BCGM_ONAFTERUPDATECAPTION);
#endif

	m_Dlg.RedrawWindow (NULL, NULL,
		RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOCHILDREN);
}
//****
void CBCGPDlgImpl::UpdateCaptionButtons()
{
	if ((m_Dlg.GetStyle () & WS_SYSMENU) == 0)
	{
		return;
	}

	CMenu* pSysMenu = m_Dlg.GetSystemMenu (FALSE);

	if (pSysMenu == NULL || !::IsMenu (pSysMenu->m_hMenu))
	{
		return;
	}

	for (POSITION pos = m_lstCaptionSysButtons.GetHeadPosition (); pos != NULL;)
	{
		CBCGPFrameCaptionButton* pButton = (CBCGPFrameCaptionButton*)m_lstCaptionSysButtons.GetNext (pos);
		ASSERT_VALID (pButton);

		if (pButton->GetHit () == HTCLOSE_BCG)
		{
			BOOL bGrayed = pSysMenu->GetMenuState (SC_CLOSE, MF_BYCOMMAND) & MF_GRAYED;
			pButton->m_bEnabled = bGrayed ? FALSE : TRUE;
		}

		if (pButton->GetHit () == HTMAXBUTTON_BCG)
		{
			BOOL bGrayed = pSysMenu->GetMenuState (SC_MAXIMIZE, MF_BYCOMMAND) & MF_GRAYED;
			pButton->m_bEnabled = bGrayed ? FALSE : TRUE;
		}

		if (pButton->GetHit () == HTMINBUTTON_BCG)
		{
			BOOL bGrayed = pSysMenu->GetMenuState (SC_MINIMIZE, MF_BYCOMMAND) & MF_GRAYED;
			pButton->m_bEnabled = bGrayed ? FALSE : TRUE;
		}
	}

#ifndef _BCGSUITE_
	m_Dlg.SendMessage (BCGM_ONAFTERUPDATECAPTION);
#endif

	m_Dlg.RedrawWindow (NULL, NULL, RDW_FRAME | RDW_INVALIDATE | RDW_UPDATENOW | RDW_NOCHILDREN);
}
//****
UINT CBCGPDlgImpl::OnNcHitTest (CPoint point)
{
	if (!IsOwnerDrawCaption ())
	{
		return HTNOWHERE;
	}

	m_Dlg.ScreenToClient (&point);

	const CSize szSystemBorder(::GetSystemMetrics (SM_CXSIZEFRAME), ::GetSystemMetrics (SM_CYSIZEFRAME));

	int cyOffset = szSystemBorder.cy;
	if (!m_Dlg.IsIconic ())
	{
		cyOffset += GetCaptionHeight ();
	}

	point.Offset (szSystemBorder.cx, cyOffset);

	for (POSITION pos = m_lstCaptionSysButtons.GetHeadPosition (); pos != NULL;)
	{
		CBCGPFrameCaptionButton* pButton = (CBCGPFrameCaptionButton*)
			m_lstCaptionSysButtons.GetNext (pos);
		ASSERT_VALID (pButton);

		if (pButton->GetRect ().PtInRect (point))
		{
			return pButton->m_nHit;
		}
	}

	CRect rectCaption = GetCaptionRect ();
	if (rectCaption.PtInRect (point))
	{
		if ((m_Dlg.GetExStyle () & WS_EX_TOOLWINDOW) == 0)
		{
			CRect rectSysMenu = rectCaption;
			rectSysMenu.right = rectSysMenu.left + ::GetSystemMetrics (SM_CXSMICON) + 2 +
				(m_Dlg.IsZoomed () ? szSystemBorder.cx : 0);

			return rectSysMenu.PtInRect (point) ? HTSYSMENU : HTCAPTION;
		}

		return HTCAPTION;
	}

	return HTNOWHERE;
}
//****
void CBCGPDlgImpl::OnNcMouseMove(UINT /*nHitTest*/, CPoint point)
{
	if (!IsOwnerDrawCaption ())
	{
		return;
	}

	OnTrackCaptionButtons (point);
}
//****
void CBCGPDlgImpl::OnLButtonDown(CPoint /*point*/)
{
	if (m_nHotSysButton == HTNOWHERE)
	{
		return;
	}

	CBCGPFrameCaptionButton* pBtn = GetSysButton (m_nHotSysButton);
	if (pBtn != NULL)
	{
		m_nHitSysButton = m_nHotSysButton;
		pBtn->m_bPushed = TRUE;
		RedrawCaptionButton (pBtn);
	}
}
//****
void CBCGPDlgImpl::OnLButtonUp(CPoint /*point*/)
{
	if (!IsOwnerDrawCaption ())
	{
		return;
	}

	switch (m_nHitSysButton)
	{
	case HTCLOSE_BCG:
	case HTMAXBUTTON_BCG:
	case HTMINBUTTON_BCG:
#ifndef _BCGSUITE_
	case HTHELPBUTTON_BCG:
#endif
		{
			UINT nHot = m_nHotSysButton;
			UINT nHit = m_nHitSysButton;

			StopCaptionButtonsTracking ();

			if (nHot == nHit)
			{
				UINT nSysCmd = 0;

				switch (nHot)
				{
				case HTCLOSE_BCG:
					nSysCmd = SC_CLOSE;
					break;

				case HTMAXBUTTON_BCG:
					nSysCmd = 
						(m_Dlg.GetStyle () & WS_MAXIMIZE) == WS_MAXIMIZE ? SC_RESTORE : SC_MAXIMIZE;
					break;

				case HTMINBUTTON_BCG:
					nSysCmd = m_Dlg.IsIconic () ? SC_RESTORE : SC_MINIMIZE;
					break;

#ifndef _BCGSUITE_
				case HTHELPBUTTON_BCG:
					nSysCmd = SC_CONTEXTHELP;
					break;
#endif
				}

				m_Dlg.PostMessage (WM_SYSCOMMAND, nSysCmd);
			}
		}
	}
}
//****
void CBCGPDlgImpl::OnMouseMove(CPoint point)
{
	if (!IsOwnerDrawCaption ())
	{
		return;
	}

	CPoint ptScreen = point;
	m_Dlg.ClientToScreen (&ptScreen);

	OnTrackCaptionButtons (ptScreen);
}
//****
CBCGPFrameCaptionButton* CBCGPDlgImpl::GetSysButton (UINT nHit)
{
	for (POSITION pos = m_lstCaptionSysButtons.GetHeadPosition (); pos != NULL;)
	{
		CBCGPFrameCaptionButton* pButton = (CBCGPFrameCaptionButton*)
			m_lstCaptionSysButtons.GetNext (pos);
		ASSERT_VALID (pButton);

		if (pButton->m_nHit == nHit)
		{
			return pButton;
		}
	}

	return NULL;
}
//****
void CBCGPDlgImpl::SetHighlightedSysButton (UINT nHit)
{
	BOOL bRedraw = FALSE;

	for (POSITION pos = m_lstCaptionSysButtons.GetHeadPosition (); pos != NULL;)
	{
		CBCGPFrameCaptionButton* pButton = (CBCGPFrameCaptionButton*)
			m_lstCaptionSysButtons.GetNext (pos);
		ASSERT_VALID (pButton);

		if (pButton->m_nHit == nHit)
		{
			if (pButton->m_bFocused)
			{
				return;
			}

			pButton->m_bFocused = TRUE;
			bRedraw = TRUE;
		}
	}
}
//****
void CBCGPDlgImpl::OnTrackCaptionButtons (CPoint point)
{
	if (CBCGPPopupMenu::GetActiveMenu () != NULL)
	{
		return;
	}

	UINT nHot = m_nHotSysButton;
	CBCGPFrameCaptionButton* pBtn = GetSysButton (OnNcHitTest (point));

	if (pBtn != NULL && pBtn->m_bEnabled)
	{
		m_nHotSysButton = pBtn->GetHit ();
		pBtn->m_bFocused = TRUE;
	}
	else
	{
		m_nHotSysButton = HTNOWHERE;
	}

	if (m_nHotSysButton != nHot)
	{
		RedrawCaptionButton (pBtn);

		CBCGPFrameCaptionButton* pBtnOld = GetSysButton (nHot);
		if (pBtnOld != NULL)
		{
			pBtnOld->m_bFocused = FALSE;
			RedrawCaptionButton (pBtnOld);
		}
	}

	if (m_nHitSysButton == HTNOWHERE)
	{
		if (nHot != HTNOWHERE && m_nHotSysButton == HTNOWHERE)
		{
			::ReleaseCapture();
		}
		else if (nHot == HTNOWHERE && m_nHotSysButton != HTNOWHERE)
		{
			m_Dlg.SetCapture ();
		}
	}
}
//****
void CBCGPDlgImpl::StopCaptionButtonsTracking ()
{
	if (m_nHitSysButton != HTNOWHERE)
	{
		CBCGPFrameCaptionButton* pBtn = GetSysButton (m_nHitSysButton);
		m_nHitSysButton = HTNOWHERE;

		ReleaseCapture ();
		if (pBtn != NULL)
		{
			pBtn->m_bPushed = FALSE;
			RedrawCaptionButton (pBtn);
		}
	}

	if (m_nHotSysButton != HTNOWHERE)
	{
		CBCGPFrameCaptionButton* pBtn = GetSysButton (m_nHotSysButton);
		m_nHotSysButton = HTNOWHERE;

		ReleaseCapture ();
		if (pBtn != NULL)
		{
			pBtn->m_bFocused = FALSE;
			RedrawCaptionButton (pBtn);
		}
	}
}
//****
void CBCGPDlgImpl::RedrawCaptionButton (CBCGPFrameCaptionButton* pBtn)
{
	if (pBtn ==	NULL)
	{
		return;
	}

	ASSERT_VALID (pBtn);

	m_rectRedraw = pBtn->GetRect ();
	m_Dlg.SendMessage (WM_NCPAINT);
	m_rectRedraw.SetRectEmpty ();

	m_Dlg.UpdateWindow ();
}
//****
void CBCGPDlgImpl::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos)
{
	if (m_bWindowPosChanging)
	{
		return;
	}

	if (((lpwndpos->flags & SWP_NOSIZE) == 0 || (lpwndpos->flags & SWP_FRAMECHANGED)) && 
		(IsOwnerDrawCaption ()))
	{
		m_bWindowPosChanging = TRUE;

		m_bIsWindowRgn = visualManagerMFC->OnSetWindowRegion (
			&m_Dlg, CSize (lpwndpos->cx, lpwndpos->cy));

		m_bWindowPosChanging = FALSE;
	}
}
//****
void CBCGPDlgImpl::OnChangeVisualManager ()
{
	if (m_Dlg.GetSafeHwnd () == NULL)
	{
		return;
	}

	CRect rectWindow;
	m_Dlg.GetWindowRect (rectWindow);

	BOOL bZoomed = m_Dlg.IsZoomed ();

	if (IsOwnerDrawCaption ())
	{
		BOOL bChangeBorder = FALSE;

		if ((m_Dlg.GetStyle () & WS_BORDER) == WS_BORDER && m_bHasBorder)
		{
			bChangeBorder = TRUE;
			m_bWindowPosChanging = TRUE;
			m_Dlg.ModifyStyle (WS_BORDER, 0, SWP_FRAMECHANGED);
			m_bWindowPosChanging = FALSE;
		}

		m_bIsWindowRgn = visualManagerMFC->OnSetWindowRegion (
			&m_Dlg, rectWindow.Size ());

		if (bZoomed && bChangeBorder)
		{
			m_Dlg.ShowWindow (SW_MINIMIZE);
			m_Dlg.ShowWindow (SW_MAXIMIZE);
		}
	}
	else
	{
		BOOL bChangeBorder = FALSE;

		if ((m_Dlg.GetStyle () & WS_BORDER) == 0 && m_bHasBorder)
		{
			bChangeBorder = TRUE;
			m_bWindowPosChanging = TRUE;
			m_Dlg.ModifyStyle (0, WS_BORDER, SWP_FRAMECHANGED);
			m_bWindowPosChanging = FALSE;
		}

		if (m_bIsWindowRgn)
		{
			m_bIsWindowRgn = FALSE;
			m_Dlg.SetWindowRgn (NULL, TRUE);
		}

		if (bZoomed && bChangeBorder)
		{
			NCCALCSIZE_PARAMS params;
			ZeroMemory(&params, sizeof (NCCALCSIZE_PARAMS));
			params.rgrc[0].left   = rectWindow.left;
			params.rgrc[0].top    = rectWindow.top;
			params.rgrc[0].right  = rectWindow.right;
			params.rgrc[0].bottom = rectWindow.bottom;

			m_Dlg.CalcWindowRect (&params.rgrc[0], CFrameWnd::adjustBorder);

			if ((m_Dlg.GetStyle () & WS_CAPTION) == WS_CAPTION)
			{
				params.rgrc[0].top += GetCaptionHeight ();
			}

			m_Dlg.SetWindowPos (NULL, params.rgrc[0].left, params.rgrc[0].top, 
				params.rgrc[0].right - params.rgrc[0].left, params.rgrc[0].bottom - params.rgrc[0].top,
				SWP_NOACTIVATE | SWP_NOZORDER);
		}
		else
		{
			m_Dlg.SetWindowPos (NULL, -1, -1, rectWindow.Width () + 1, rectWindow.Height () + 1,
				SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
			m_Dlg.SetWindowPos (NULL, -1, -1, rectWindow.Width (), rectWindow.Height (),
				SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
		}
	}

	UpdateCaption ();
	UpdateCaptionButtons();
}
//****
int CBCGPDlgImpl::GetCaptionHeight ()
{
	if (m_Dlg.GetSafeHwnd () == NULL)
	{
		return 0;
	}

	if (m_Dlg.GetExStyle () & WS_EX_TOOLWINDOW)
	{
		return ::GetSystemMetrics (SM_CYSMCAPTION);
	}
	
	return ::GetSystemMetrics (SM_CYCAPTION);
}
//****
void CBCGPDlgImpl::OnGetMinMaxInfo (MINMAXINFO FAR* lpMMI)
{
	ASSERT (lpMMI != NULL);

	if ((m_Dlg.GetStyle () & WS_CAPTION) == 0 ||
		(m_Dlg.GetStyle () & WS_BORDER) == 0)
	{
		CRect rectWindow;
		m_Dlg.GetWindowRect (&rectWindow);

		if (m_Dlg.IsIconic ())
		{
			WINDOWPLACEMENT wp;
			wp.length = sizeof (WINDOWPLACEMENT);

			m_Dlg.GetWindowPlacement (&wp);

			rectWindow = wp.rcNormalPosition;
		}

		CRect rect (0, 0, 0, 0);

		MONITORINFO mi;
		mi.cbSize = sizeof (MONITORINFO);

		if (GetMonitorInfo (MonitorFromPoint (rectWindow.CenterPoint (), MONITOR_DEFAULTTONEAREST),
			&mi))
		{
			CRect rectWork = mi.rcWork;
			CRect rectScreen = mi.rcMonitor;

			rect.left = rectWork.left - rectScreen.left;
			rect.top = rectWork.top - rectScreen.top;

			rect.right = rect.left + rectWork.Width ();
			rect.bottom = rect.top + rectWork.Height ();
		}
		else
		{
			::SystemParametersInfo (SPI_GETWORKAREA, 0, &rect, 0);
		}

		if (globalData.m_nShellAutohideBars & BCGP_AUTOHIDE_BOTTOM)
		{
			rect.bottom -= 2;
		}

		if (globalData.m_nShellAutohideBars & BCGP_AUTOHIDE_TOP)
		{
			rect.top += 2;
		}

		if (globalData.m_nShellAutohideBars & BCGP_AUTOHIDE_RIGHT)
		{
			rect.right -= 2;
		}

		if (globalData.m_nShellAutohideBars & BCGP_AUTOHIDE_LEFT)
		{
			rect.left += 2;
		}

		lpMMI->ptMaxPosition.x = rect.left;
		lpMMI->ptMaxPosition.y = rect.top;
		lpMMI->ptMaxSize.x = rect.Width ();
		lpMMI->ptMaxSize.y = rect.Height ();
	}
}
