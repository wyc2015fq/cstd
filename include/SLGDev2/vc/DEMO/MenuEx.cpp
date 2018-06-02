// MenuEx.cpp: implementation of the CMenuEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MenuEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMenuEx::CMenuEx() :
m_bPopMenu(TRUE)
{

}

CMenuEx::~CMenuEx()
{

}

BOOL CMenuEx::CreatePopMenu( UINT nIDResource )
{
//	m_pParentMenu.LoadMenu( nIDResource );
//	m_hMenu = (m_pParentMenu.GetSubMenu(0))->m_hMenu;
	CreatePopupMenu();
	return TRUE;
}

BOOL CMenuEx::InserMenuItem( UINT nIDResource, LPCTSTR lpszNewItem )
{
	return CMenu::AppendMenu(MF_STRING, nIDResource, lpszNewItem);
}

void CMenuEx::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	if (lpDrawItemStruct->CtlType != ODT_MENU ) return;
	MENUITEM* item = (MENUITEM*)(lpDrawItemStruct->itemData);
	HDC hdc = lpDrawItemStruct->hDC;
	::SetBkColor(hdc, TRANSPARENT);
	if (lpDrawItemStruct->itemState&ODS_CHECKED)
	{
		int a = 0;
	}
	DrawBgColor(hdc, lpDrawItemStruct->rcItem);
	DrawSelected(hdc, lpDrawItemStruct->rcItem, lpDrawItemStruct->itemState);
	DrawMenuImage(hdc, lpDrawItemStruct->rcItem, lpDrawItemStruct->itemState);
	DrawText(hdc, lpDrawItemStruct->rcItem, item->strText, lpDrawItemStruct->itemState);
}

void CMenuEx::MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
	lpMeasureItemStruct->itemWidth = 130;
	lpMeasureItemStruct->itemHeight = 22;
}

BOOL CMenuEx::LoadMenu( UINT nIDResource )
{
	CMenu menu;
	if (!menu.LoadMenu(nIDResource)) return FALSE;
	
	if (!(CMenu::CreatePopupMenu())) return FALSE;
	
	LoadMenu(*menu.GetSubMenu(0));
	return TRUE;
}

BOOL CMenuEx::LoadMenu( CMenu& menu )
{
	for (UINT i = 0; i < menu.GetMenuItemCount(); ++i)
	{
		m_menuItem[i].nID = menu.GetMenuItemID(i);;
		m_menuItem[i].checkStatus = FALSE;
		menu.GetMenuString(i, m_menuItem[i].strText, MF_BYPOSITION);
		CMenu::AppendMenu(MF_OWNERDRAW, m_menuItem[i].nID, (LPCTSTR)(&m_menuItem[i]));
	}
	return TRUE;
}

BOOL CMenuEx::AppendMenu( UINT nFlags, UINT nIDNewItem /*= 0*/, LPCTSTR lpszNewItem /*= NULL */ )
{
	return TRUE;
}

void CMenuEx::DrawText( HDC hdc, RECT rect, CString strText, UINT  itemState )
{
	RECT rright = rect;
	rright.left = rect.left + 25;

	::SetBkColor(hdc, TRANSPARENT);

	COLORREF crBK = ::GetSysColor(COLOR_WINDOW);
	COLORREF crText = ::GetSysColor(COLOR_WINDOWTEXT);

	if (itemState&ODS_SELECTED)
	{
		crBK = ::GetSysColor(COLOR_HIGHLIGHT);
		crText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	}


	::SetTextColor(hdc, crText);
 	::SetBkColor(hdc, crBK);
	::DrawText(hdc, strText, -1,  &rright, DT_LEFT | DT_VCENTER | DT_SINGLELINE);	
}

void CMenuEx::DrawBgColor( HDC hdc, RECT rect )
{
	RECT rleft = rect;
	rleft.right = rleft.left + 20;
	RECT rright = rect;
	rright.left = rleft.right;
	HBRUSH hbrLeft  = ::CreateSolidBrush( RGB(236,233,216) );
	HBRUSH hbrRight = ::CreateSolidBrush(::GetSysColor(COLOR_WINDOW));
	::FillRect(hdc, &rleft, hbrLeft);
	::FillRect(hdc, &rright, hbrRight);
	::DeleteObject(hbrLeft);
	::DeleteObject(hbrRight);

}

void CMenuEx::DrawSelected( HDC hdc, RECT rect, UINT  itemState)
{
	if (itemState&ODS_SELECTED)
	{
		HBRUSH hbr = ::CreateSolidBrush(::GetSysColor(COLOR_HIGHLIGHT));
		::FillRect(hdc, &rect, hbr);
		::DeleteObject(hbr);
	}
}

void CMenuEx::DrawMenuImage( HDC hdc, RECT rect, UINT itemState )
{
	if (itemState&ODS_CHECKED)
	{
		RECT rleft = rect;
		rleft.right = rleft.left + 20;
		
		RECT ckbRect = rleft;
		ckbRect.top += 4;
		ckbRect.bottom -= 4;
		
		int w = rleft.right - rleft.left;
		int h = ckbRect.bottom - ckbRect.top;
		
		ckbRect.left = rect.left + w / 2 - h/2 ;
		ckbRect.right = ckbRect.left + h;

		{
			HBRUSH hbr = ::CreateSolidBrush( ::GetSysColor(COLOR_WINDOW) );
			::FillRect(hdc, &ckbRect, hbr);
			::DeleteObject(hbr);
		}

		{
			HBRUSH hbr = ::CreateSolidBrush(RGB(51,102,153));
			HGDIOBJ old = ::SelectObject(hdc, hbr);
			RECT showrect ;
			showrect.left = ckbRect.left;
			showrect.right = ckbRect.right;
			showrect.top  = ckbRect.top;
			showrect.bottom = ckbRect.bottom;
			int a =  ::FrameRect(hdc, &showrect, hbr);
			::SelectObject(hdc, old);
			::DeleteObject(hbr);
			::DeleteObject(old);
		}

		{
			HPEN blackpen = ::CreatePen(PS_SOLID, 1, RGB(51,153,51));
			HGDIOBJ oldpen = ::SelectObject(hdc, blackpen);
			
			
			
			int x = ckbRect.left + 9;
			ASSERT(x < ckbRect.right);
			int y = ckbRect.top + 3;
			int i;
			for (i = 0; i < 4; i++)
			{
				::MoveToEx(hdc, x, y, NULL);
				::LineTo(hdc, x, y+3);
				x--;
				y++;
			}
			for (i = 0; i < 3; i++)
			{
				::MoveToEx(hdc, x, y, NULL);
				::LineTo(hdc, x, y+3);
				x--;
				y--;
			}	
			
			::SelectObject(hdc, oldpen);
			::DeleteObject(blackpen);
			::DeleteObject(oldpen);
		}
	}
}