// SkinTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SkinTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinTreeCtrl

CSkinTreeCtrl::CSkinTreeCtrl()
{
}

CSkinTreeCtrl::~CSkinTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CSkinTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CSkinTreeCtrl)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnNMCustomdraw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinTreeCtrl message handlers


void CSkinTreeCtrl::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)pNMHDR;
	HTREEITEM parentitem = NULL;
	if( lplvcd != NULL )
	{
		INT nState;
		
		switch(lplvcd->nmcd.dwDrawStage)
		{
		case CDDS_PREPAINT: 
			* pResult= CDRF_NOTIFYITEMDRAW; 
			break;

		case CDDS_ITEMPREPAINT:
			//parentitem=GetParentItem((HTREEITEM)lplvcd->nmcd.dwItemSpec); 
			//if( parentitem != NULL )
			{
				nState = lplvcd->nmcd.uItemState;
				if( nState&CDIS_SELECTED || nState&CDIS_FOCUS != CDIS_FOCUS  )
				{
					DEBUG_INFO1( _T("%s"), _T("nState&CDIS_SELECTED") );
					lplvcd->clrTextBk = RGB(49,106,197);
					lplvcd->clrText = RGB(255,255,255);
					* pResult= CDRF_DODEFAULT;
					break;
				}
				else
				{
					//lplvcd->clrTextBk = RGB(255,255,255);
					//lplvcd->clrText = RGB(0,0,0);
					* pResult= CDRF_DODEFAULT;
					break;
				}	
			}


			
		default:
			* pResult = 0;
			break;  
		}

	}

	

	/*
	LPNMHDR pNmhdr = (LPNMHDR)pNMHDR;          
	
	// To make sure that the notification is coming
	// from the right control (GGH)
	// CLeftView *pView = GetLeftPane();
		
	if( m_hWnd != pNmhdr->hwndFrom)
		return;
		
		LPNMTVCUSTOMDRAW pCustomDraw = (LPNMTVCUSTOMDRAW)pNMHDR;
		switch (pCustomDraw->nmcd.dwDrawStage)
		{
		case CDDS_PREPAINT:
			// Need to process this case and set pResult to CDRF_NOTIFYITEMDRAW, 
			// otherwise parent will never receive CDDS_ITEMPREPAINT notification. (GGH) 
			*pResult = CDRF_NOTIFYITEMDRAW;
			return;
			
		case CDDS_ITEMPREPAINT:
			if(this->IsWindowEnabled()==1)
			{
				if ( (pCustomDraw->nmcd.uItemState & (CDIS_FOCUS))==0 &&
					(pCustomDraw->nmcd.uItemState & (CDIS_SELECTED))==CDIS_SELECTED) // selected
				{ 
					pCustomDraw->clrTextBk=RGB(255, 255, 255);
					pCustomDraw->clrText = RGB(0, 0, 0);
				}
				*pResult = CDRF_NOTIFYPOSTPAINT;
				return;
			}
			else{
				*pResult = CDRF_DODEFAULT ;
				return;
			}
		case CDDS_ITEMPOSTPAINT:
			if(this->IsWindowEnabled()==1)
			{
				if ( (pCustomDraw->nmcd.uItemState & (CDIS_FOCUS))==0 &&
					(pCustomDraw->nmcd.uItemState & (CDIS_SELECTED))==CDIS_SELECTED) // selected
				{
					CRect   rcText;  
					HTREEITEM hItem=(HTREEITEM) pCustomDraw->nmcd.dwItemSpec;
					GetItemRect(hItem,   &rcText,   true); 
					CPen penBlue(PS_SOLID ,1,RGB(0, 0, 255));
					CDC* pDC=CDC::FromHandle(pCustomDraw->nmcd.hdc); 
					CBrush* pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
					CBrush* pOldBrush = pDC->SelectObject(pBrush);
					CPen* pOldPen = pDC->SelectObject(&penBlue);
					pDC->Rectangle(&rcText);
					pDC->SelectObject(pOldBrush);
					pDC->SelectObject(pOldPen);
				}
				*pResult = CDRF_SKIPDEFAULT;
				return;
			}
			else{
				*pResult = CDRF_DODEFAULT ;
				return;
			}
			
		}

  */
		


	
}

BOOL CSkinTreeCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CTreeCtrl::OnNotify(wParam, lParam, pResult);
}
