
//
// BCGPFullScreenImpl.cpp: implementation of the BCGPFullScreenImpl class.
//
//////////////////////////////////////////////////////////////////////

#include "BCGPFullScreenImpl.h"
#include "BCGPMDIFrameWnd.h"
#include "BCGPMDIChildWnd.h"
#include "BCGPFrameWnd.h"
#include "BCGPWorkspace.h"
#include "BCGPToolBar.h"
#include "BCGPMiniFrameWnd.h"
#include "BCGPDockingControlBar.h"
#include "BCGPDropDown.h"
#include "BCGPBaseTabbedBar.h"
#include "BCGPDockBar.h"
#include "bcgprores.h"

#include "BCGPStatusBar.h"
#include "BCGPReBar.h"
//#include "multimon.h"
#include "BCGPRibbonBar.h"

extern CBCGPWorkspace* g_pWorkspace;

class CBCGFullScreenToolbar : public CBCGPToolBar
{
	virtual BOOL CanBeClosed() const
	{
		return FALSE;
	}
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPFullScreenImpl::CBCGPFullScreenImpl(CBCGPFrameImpl* pFrameImpl)
{
	m_pImpl				= pFrameImpl;
	m_pwndFullScreenBar = NULL;
	m_bFullScreen		= FALSE;
	m_bShowMenu			= TRUE;
	m_bTabsArea			= TRUE;
	m_uiFullScreenID	= (UINT)-1;
	m_strRegSection		= _T("");
}
//****
CBCGPFullScreenImpl::~CBCGPFullScreenImpl()
{

}
//****
void CBCGPFullScreenImpl::ShowFullScreen(CFrameWnd* pFrame)
{
	ASSERT (m_uiFullScreenID != -1);

	if (m_uiFullScreenID == -1)
	{
		return;
	}

	CBCGPMDIFrameWnd* pbcgMDIFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, pFrame);
	if (pbcgMDIFrame != NULL)
	{
		CBCGPMDIChildWnd* pChildWnd = DYNAMIC_DOWNCAST (CBCGPMDIChildWnd, pbcgMDIFrame->MDIGetActive());
		if (pChildWnd != NULL)
		{
			CWnd* pViewWnd = pChildWnd->GetActiveView();
			if (pViewWnd == NULL && !pChildWnd->IsTabbedControlBar())
			{
				return;
			}
		}
	}
	else
	{	
		CBCGPFrameWnd* pbcgFrame = DYNAMIC_DOWNCAST (CBCGPFrameWnd, pFrame);
		if (pbcgFrame == NULL)
		{
			return;
		}
	}

	CRect rectFrame, rectView, rectChild, rcScreen;

	pFrame->GetWindowRect (&rectFrame);
	m_rectFramePrev = rectFrame;

	MONITORINFO mi;
	mi.cbSize = sizeof (MONITORINFO);
	if (GetMonitorInfo (MonitorFromPoint (rectFrame.TopLeft(), MONITOR_DEFAULTTONEAREST),
		&mi))
	{
		rcScreen = mi.rcMonitor;

	}
	else
	{
		::SystemParametersInfo (SPI_GETWORKAREA, 0, &rcScreen, 0);
	}

	m_strRegSection = g_pWorkspace->GetRegistryBase(); 


	if (pbcgMDIFrame != NULL)
	{
		  g_pWorkspace->SaveState(pbcgMDIFrame, m_strRegSection);
		  g_pWorkspace->CleanState (_T("FullScreeenState"));	 
	}
	else
	{
		CBCGPFrameWnd* pbcgFrame = DYNAMIC_DOWNCAST (CBCGPFrameWnd, pFrame);
		if (pbcgFrame != NULL)
		{
			g_pWorkspace->SaveState (pbcgFrame, m_strRegSection);
			g_pWorkspace->CleanState (_T("FullScreeenState"));
		}
		else
		{
			return;
		}
	}

	CBCGPMenuBar* pMenuBar = NULL;
	if (pbcgMDIFrame != NULL)
	{
		pMenuBar = DYNAMIC_DOWNCAST (CBCGPMenuBar, pbcgMDIFrame->GetMenuBar());
	}
	else
	{
		CBCGPFrameWnd* pbcgFrame = DYNAMIC_DOWNCAST (CBCGPFrameWnd, pFrame);
		if (pbcgFrame != NULL)
		{
			pMenuBar = DYNAMIC_DOWNCAST (CBCGPMenuBar, pbcgFrame->GetMenuBar());
		}
	}

	CSize sizeMenu = CSize (0,0);
	if (pMenuBar != NULL)
	{
		sizeMenu  = pMenuBar->CalcFixedLayout (FALSE, TRUE);
	}


	pFrame->SetRedraw (FALSE);
	UnDockAndHideControlBars (pFrame);

	CBCGPMDIChildWnd*	pChildWnd = NULL;
	if (pbcgMDIFrame != NULL)
	{
		pChildWnd = DYNAMIC_DOWNCAST (CBCGPMDIChildWnd, pbcgMDIFrame->MDIGetActive());
	}

	if (pbcgMDIFrame != NULL && pChildWnd != NULL)
	{
		CWnd* pViewWnd = NULL;
		if (pChildWnd->IsTabbedControlBar()) 
		{
			pViewWnd = pChildWnd->GetTabbedControlBar();
		}
		else
		{
			pViewWnd = pChildWnd->GetActiveView();
		}

		if (pViewWnd == NULL)
		{
			return;
		}

		pChildWnd->GetWindowRect (&rectView);
		pViewWnd->GetWindowRect (&rectChild);

		if (pbcgMDIFrame->AreMDITabs())
		{
			if (m_bTabsArea)
			{
				CBCGPDockManager* pDockMgr = m_pImpl->m_pDockManager;
				pDockMgr->AdjustDockingLayout();
				CRect rectClient; 
				pbcgMDIFrame->GetClientRect (&rectClient);
				pbcgMDIFrame->ClientToScreen (&rectClient);

				rectFrame.InflateRect (
					 (rectClient.l - rcScreen.l),
					 (rectClient.t - rcScreen.t) ,
					 (rcScreen.r - rectClient.r),
					 rcScreen.b - rectClient.b);
			}
			else
			{
				rectFrame.InflateRect (
					rectChild.l - rcScreen.l,
					rectChild.t - rcScreen.t,
					rcScreen.r - rectChild.r,
					rcScreen.b - rectChild.b);
			}			
		}
		else
		{
			rectFrame.InflateRect (
				rectChild.l - rcScreen.l,
				(rectChild.t - rcScreen.t) ,
				(rcScreen.r - rectChild.r),
				 rcScreen.b - rectChild.b);


				CBCGPTabWnd* wndTab = DYNAMIC_DOWNCAST (CBCGPTabWnd, pViewWnd->GetParent());
				if (wndTab != NULL && wndTab->GetLocation() == CBCGPTabWnd::LOCATION_TOP)
				{
					CBCGPMenuBar* pMenuBar = DYNAMIC_DOWNCAST (CBCGPMenuBar, pbcgMDIFrame->GetMenuBar());
					if (pMenuBar != NULL)
					{
						const int nSystemBorder = ::GetSystemMetrics (SM_CYSIZEFRAME);
						int nHeight = sizeMenu.h - nSystemBorder;
						rectFrame.t += nHeight; 
					}

				}
		}
	}
	else // Maybe SDI
	{
		CBCGPFrameWnd* pbcgFrame = DYNAMIC_DOWNCAST (CBCGPFrameWnd, pFrame);

		if (pbcgFrame != NULL)
		{
			CWnd* pViewWnd = pFrame->GetActiveView();
			if (pViewWnd == NULL)
			{
				CRect rectFrmClient;
				pbcgFrame->GetClientRect (&rectFrmClient);
				pbcgFrame->ClientToScreen (&rectFrmClient);

				rectFrame.InflateRect (
						rectFrmClient.l - rcScreen.l,
						rectFrmClient.t - rcScreen.t,
						rcScreen.r - rectFrmClient.r,
						rcScreen.b - rectFrmClient.b);
			}
			else
			{
				pViewWnd->GetWindowRect (&rectView);
				pbcgFrame->GetWindowRect (&rectFrame);
				rectFrame.InflateRect (
					rectView.l - rcScreen.l,
					rectView.t - rcScreen.t,
					rcScreen.r - rectView.r,
					rcScreen.b - rectView.b);
			}
		}
	}

	if (pbcgMDIFrame != NULL && pChildWnd == NULL)
	{
		CRect rectFrmClient;
		pbcgMDIFrame->GetClientRect (&rectFrmClient);
		pbcgMDIFrame->ClientToScreen (&rectFrmClient);

		rectFrame.InflateRect (
				rectFrmClient.l - rcScreen.l,
				rectFrmClient.t - rcScreen.t,
				rcScreen.r - rectFrmClient.r,
				rcScreen.b - rectFrmClient.b);
	}


	// Remember this for OnGetMinMaxInfo()
	m_rectFullScreenWindow = rectFrame;
		
	m_pwndFullScreenBar	= new CBCGFullScreenToolbar;
	
	if (!m_pwndFullScreenBar->Create (pFrame))
	{
		TRACE0("Failed to create toolbar\n");
		return;      // fail to create
	}
	
	CString strCaption;
	CString strLabel;
	{
		CBCGPLocalResource locaRes;
		strLabel.LoadString (IDS_BCGBARRES_FULLSCREEN_CLOSE);
		strCaption.LoadString (IDS_BCGBARRES_FULLSCREEN);
	}

	CBCGPToolbarButton	button (m_uiFullScreenID, -1, strLabel, FALSE, TRUE);
	m_pwndFullScreenBar->InsertButton (button);
	m_pwndFullScreenBar->EnableDocking (0);
	m_pwndFullScreenBar->SetWindowPos (0, 100, 100, 100, 100, SWP_NOZORDER | SWP_NOACTIVATE | SWP_SHOWWINDOW);
	m_pwndFullScreenBar->SetWindowText (strCaption);
	m_pwndFullScreenBar->FloatControlBar (CRect(rcScreen.l + 100, rcScreen.t + 100, 200, 200));
	m_bFullScreen = true;
			
	pFrame->SetWindowPos (NULL, rectFrame.l, rectFrame.t,
		rectFrame.Width(), rectFrame.Height(), SWP_NOZORDER);

	if (m_bShowMenu)
	{
		if (pbcgMDIFrame != NULL)
		{
			const CBCGPMenuBar* pMenuBar = pbcgMDIFrame->GetMenuBar();

			if (pMenuBar != NULL)
			{
				if (m_bTabsArea)
				{
					if (pMenuBar->CanFloat())
					{
						pbcgMDIFrame->DockControlBar ((CBCGPBaseControlBar*)pMenuBar);
					}
					pbcgMDIFrame->ShowControlBar ((CBCGPBaseControlBar*)pMenuBar, TRUE,FALSE, FALSE);
				}
				else
				{
					if (pMenuBar->CanFloat())
					{
						pbcgMDIFrame->DockControlBar ((CBCGPBaseControlBar*)pMenuBar);
						((CBCGPBaseControlBar*)pMenuBar)->FloatControlBar (CRect(300, 200, 500, 500));
					}
					pbcgMDIFrame->ShowControlBar ((CBCGPBaseControlBar*)pMenuBar, TRUE,FALSE, FALSE);
				}
			}	
		}
		else
		{
			CBCGPFrameWnd* pbcgFrame = DYNAMIC_DOWNCAST (CBCGPFrameWnd, pFrame);
			if (pbcgFrame != NULL)
			{
				const CBCGPMenuBar* pMenuBar = pbcgFrame->GetMenuBar();

				if (pMenuBar != NULL)
				{
					if (pMenuBar->CanFloat())
					{
						pbcgFrame->DockControlBar ((CBCGPBaseControlBar*)pMenuBar);
					}
					pbcgFrame->ShowControlBar ((CBCGPBaseControlBar*)pMenuBar, TRUE,FALSE, FALSE);
				}	
			}
		}
	}

	pFrame->SetRedraw (TRUE);
	pFrame->RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | 
							RDW_ERASE | RDW_ALLCHILDREN);

	if (pbcgMDIFrame != NULL && pbcgMDIFrame->IsMDITabbedGroup())
	{
		pbcgMDIFrame->AdjustClientArea();
	}
}
//****
void CBCGPFullScreenImpl::RestoreState(CFrameWnd* pFrame) 
{
	ASSERT (m_uiFullScreenID != -1);

	if (m_uiFullScreenID == -1)
	{
		return;
	}

	// Destroy the toolbar
	CWnd*	pWnd	= m_pwndFullScreenBar->GetParentMiniFrame();
	if (pWnd == NULL)
		pWnd	= m_pwndFullScreenBar;
	VERIFY (pWnd->DestroyWindow());

	delete m_pwndFullScreenBar; 
		
	m_pwndFullScreenBar = NULL;
	m_bFullScreen = false;

	CBCGPMDIFrameWnd* pbcgMDIFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, pFrame);
		
	//Restore window pos
	pFrame->SetWindowPos (NULL, m_rectFramePrev.l, m_rectFramePrev.t,
		m_rectFramePrev.Width(), m_rectFramePrev.Height(), SWP_NOZORDER);

	//restore layout
	pFrame->SetRedraw (FALSE);

	g_pWorkspace->m_bLoadUserToolbars = FALSE;

	if (pbcgMDIFrame != NULL)
	{
		g_pWorkspace->LoadState (pbcgMDIFrame, m_strRegSection);
	}
	else
	{
		CBCGPFrameWnd* pbcgFrame = DYNAMIC_DOWNCAST (CBCGPFrameWnd, pFrame);
		if (pbcgFrame != NULL)
		{
			g_pWorkspace->LoadState (pbcgFrame, m_strRegSection);
		}
	}

	pFrame->SetRedraw (TRUE);
	pFrame->RedrawWindow (NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW | 
								RDW_ERASE | RDW_ALLCHILDREN);

	if (!m_bShowMenu)
	{
		if (pbcgMDIFrame != NULL)
		{
			const CBCGPMenuBar* pMenuBar = pbcgMDIFrame->GetMenuBar();

			if (pMenuBar != NULL)
			{
				pbcgMDIFrame->ShowControlBar ((CBCGPBaseControlBar*)pMenuBar, TRUE,FALSE, FALSE);
				if (pMenuBar->IsFloating())
				{
					pbcgMDIFrame->SetFocus();
				}
			}
		}
		else
		{
			CBCGPFrameWnd* pbcgFrame = DYNAMIC_DOWNCAST (CBCGPFrameWnd, pFrame);
			if (pbcgFrame != NULL)
			{
				const CBCGPMenuBar* pMenuBar = pbcgFrame->GetMenuBar();
				if (pMenuBar != NULL)
				{
					pbcgFrame->ShowControlBar ((CBCGPBaseControlBar*)pMenuBar, TRUE,FALSE, FALSE);
					if (pMenuBar->IsFloating())
					{
						pbcgFrame->SetFocus();
					}
				}
			}
		}	
	}

	if (pbcgMDIFrame != NULL && pbcgMDIFrame->IsMDITabbedGroup())
	{
		pbcgMDIFrame->AdjustClientArea();
	}

#ifndef BCGP_EXCLUDE_RIBBON
	if (m_pImpl != NULL &&
		m_pImpl->m_pRibbonBar != NULL &&
		m_pImpl->m_pRibbonBar->IsWindowVisible() &&
		m_pImpl->m_pRibbonBar->IsReplaceFrameCaption())
	{
		m_pImpl->OnChangeVisualManager();
	}
#endif
}
//****
void CBCGPFullScreenImpl::ShowFullScreen()
{
	if (!m_bFullScreen)
	{
		ShowFullScreen (m_pImpl->m_pFrame);
	}
	else
	{
		RestoreState (m_pImpl->m_pFrame);
	}
}
//****
void CBCGPFullScreenImpl::UnDockAndHideControlBars(CFrameWnd* pFrame)
{

	CBCGPMDIFrameWnd* pbcgMDIFrame = DYNAMIC_DOWNCAST (CBCGPMDIFrameWnd, pFrame);
	CBCGPFrameWnd* pbcgFrame = DYNAMIC_DOWNCAST (CBCGPFrameWnd, pFrame);

	CBCGPDockManager* pDockMgr = m_pImpl->m_pDockManager;

	if	(pDockMgr == NULL)
	{
		return;
	}

	CObList list;
	pDockMgr->GetControlBarList (list, TRUE);

	//------------------------------------
	// UnDock and hide DockingControlBars
	//-------------------------------------

	POSITION pos;
	for (pos = list.GetHeadPosition(); pos != NULL;)
	{
		CBCGPBaseControlBar* pBarNext = (CBCGPBaseControlBar*) list.GetNext (pos);

		if (!::IsWindow(pBarNext->m_hWnd))
		{
			continue;
		}

		CBCGPDockingControlBar* pBar = DYNAMIC_DOWNCAST(CBCGPDockingControlBar, pBarNext);
		if (pBar != NULL) 
		{
			if (pBar->IsAutoHideMode())
			{
				pBar->SetAutoHideMode (FALSE, CBRS_ALIGN_ANY);
			}

			if (pBar->IsMDITabbed())
			{
				continue;
			}

			if (pBar->IsTabbed())
			{
				CBCGPBaseTabWnd* pTabWnd = (CBCGPBaseTabWnd*) pBar->GetParent();
				CBCGPBaseTabbedBar* pTabBar = (CBCGPBaseTabbedBar*) pTabWnd->GetParent();
				pTabBar->DetachControlBar (pBar);
			}

			if(pBar->CanFloat())
			{
				pBar->FloatControlBar (CRect (300, 200, 500, 500));
			}

			if (pbcgMDIFrame != NULL)
			{
				pbcgMDIFrame->ShowControlBar (pBar, FALSE,FALSE, FALSE);

			}
			else if (pbcgFrame != NULL)
			{
				pbcgFrame->ShowControlBar (pBar, FALSE,FALSE, FALSE);
			}

		}else
		{
			CBCGPControlBar* pControlBar =  DYNAMIC_DOWNCAST (CBCGPControlBar, pBarNext);

			if (pControlBar != NULL)
			{
				if (pbcgMDIFrame != NULL)
				{
					pbcgMDIFrame->ShowControlBar (pControlBar, FALSE,FALSE, FALSE);
				}
				else if (pbcgFrame != NULL)
				{
					pbcgFrame->ShowControlBar (pControlBar, FALSE,FALSE, FALSE);
				}

				continue;
			}
		}
	}

	//------------------------------------
	// UnDock and hide all Toolbars
	//-------------------------------------
	const CObList& gAllToolbars = CBCGPToolBar::GetAllToolbars();
	
	for (pos = gAllToolbars.GetHeadPosition(); pos != NULL;)
	{
		CBCGPToolBar* pToolBar = (CBCGPToolBar*) gAllToolbars.GetNext (pos);
		ASSERT (pToolBar != NULL);

		if (CWnd::FromHandlePermanent (pToolBar->m_hWnd) != NULL)
		{
			ASSERT_VALID(pToolBar);

			//------------------------------
			// Don't touch dropdown toolbars!
			//------------------------------
			if (!pToolBar->IsKindOf (RUNTIME_CLASS (CBCGPDropDownToolBar)))
			{
				if (pToolBar->IsKindOf (RUNTIME_CLASS (CBCGPMenuBar)))
				{
					if(pToolBar->CanFloat())
					{
						pToolBar->FloatControlBar (CRect (0, -1024, 0, -1024));

						if (pbcgMDIFrame != NULL)
						{
							pbcgMDIFrame->ShowControlBar (pToolBar, FALSE,FALSE, FALSE);
						}
						else if (pbcgFrame != NULL)
						{
							pbcgFrame->ShowControlBar (pToolBar, FALSE,FALSE, FALSE);
						}
					}
					continue;
				}

				//------------------------------------------------------
				// Don't touch toolbars resids on the DockingControlBars
				//------------------------------------------------------

				CWnd* pWnd = pToolBar->GetParent();
				if (pWnd->IsKindOf (RUNTIME_CLASS (CBCGPDockBar)) ||
				   pWnd->IsKindOf (RUNTIME_CLASS (CBCGPMiniFrameWnd)))
				{
					if (pbcgMDIFrame != NULL)
					{
						if (pToolBar->CanFloat())
						{
							pToolBar->FloatControlBar (CRect (300, 200, 500, 500));
						}

						pbcgMDIFrame->ShowControlBar (pToolBar, FALSE,FALSE, FALSE);
					}
					else if (pbcgFrame != NULL)
					{
					
						if (pToolBar->CanFloat())
						{
							pToolBar->FloatControlBar (CRect (300, 200, 500, 500));
						}
						pbcgFrame->ShowControlBar (pToolBar, FALSE,FALSE, FALSE);
					}
				}
			}
		}
	}
}
//****
void CBCGPFullScreenImpl::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
{
	if (m_bFullScreen)
	{
		lpMMI->ptMaxSize.x		=
		lpMMI->ptMaxTrackSize.x	= m_rectFullScreenWindow.Width();

		lpMMI->ptMaxSize.y		=
		lpMMI->ptMaxTrackSize.y	= m_rectFullScreenWindow.Height();
	}
}