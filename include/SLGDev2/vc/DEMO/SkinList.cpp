// MyListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "SkinList.h"
#include "MEMDC.h"
// CSkinList

IMPLEMENT_DYNAMIC(CSkinList, CListCtrl)

CSkinList::CSkinList() :
m_nItemHeight(0),
m_nItemWidth( 0 ),
m_crText( ::GetSysColor(COLOR_WINDOWTEXT) ),
m_crBackground( ::GetSysColor(COLOR_WINDOW) ),
m_crHighLight( ::GetSysColor(COLOR_HIGHLIGHT) ),
m_crHighLightText( ::GetSysColor(COLOR_HIGHLIGHTTEXT) ),
m_dwStyle( LVS_REPORT )
{

}

CSkinList::~CSkinList()
{
//	DeleteAllItems();
}


BEGIN_MESSAGE_MAP(CSkinList, CListCtrl)
	ON_NOTIFY_REFLECT_EX( NM_CLICK, CSkinList::OnLButtonDown )
	ON_WM_PAINT()
	ON_WM_MEASUREITEM_REFLECT()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CSkinList::OnPaint()
{
	CPaintDC dc(this);
	
	RECT rt = {0};
	GetClientRect( &rt );
	CDC* pdc = CDC::FromHandle( dc.m_hDC );
	pdc->FillSolidRect( &rt, m_crBackground );
	int nItems = CListCtrl::GetItemCount();

    if ( m_dwStyle == LVS_REPORT ){
		for ( int i = 0; i < nItems; ++i ){
			DrawReport( i, dc.m_hDC );
		}
	}
}

BOOL CSkinList::SetItemState( int nItem,UINT nState,UINT nMask )
{
	UpdateWindow();
	return CListCtrl::SetItemState( nItem, nState, nMask );
}

void CSkinList::SetItemHeight(int nItemHeight)
{
	m_nItemHeight = nItemHeight;
	RECT rt = {0};
	GetWindowRect(&rt);
	WINDOWPOS wp; 
    wp.hwnd = m_hWnd; 
	wp.cx = rt.right - rt.left; 
	wp.cy = rt.bottom - rt.top;
    wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
    SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );
}

void CSkinList::SetItemWidth(int nItemHeight)
{
	m_nItemWidth = nItemHeight;
}


void CSkinList::SetBkColor( COLORREF color )
{
	m_crBackground = color;
}

void CSkinList::SetTextColor( COLORREF color )
{
	m_crText = color;
}

void CSkinList::SetHighLightColor( COLORREF color )
{
	m_crHighLight = color;
}

void CSkinList::SetHighLightTextColor( COLORREF color )
{
	m_crHighLightText = color;
}


int CSkinList::InsertItem( int nItem, LPCTSTR lpszItem  )
{
	if (nItem < 0 || nItem > GetItemCount() ) return -1;
	int index = CListCtrl::InsertItem(nItem, lpszItem);

	LISTCTRLEXDATA* pLSD = new LISTCTRLEXDATA[ GetColumnCount() ];
	pLSD[0].m_strText = lpszItem;
	
	CListCtrl::SetItemData( index, (DWORD)pLSD );
	return index;
}
 

BOOL CSkinList::SetItemText( int nItem, int nSubItem,LPCTSTR lpszText )
{
	if (nItem < 0 || nItem >= GetItemCount() || nSubItem >= GetColumnCount() ) return FALSE;

	LISTCTRLEXDATA* pLSD = (LISTCTRLEXDATA*)( CListCtrl::GetItemData(nItem) );
	if ( NULL == pLSD ) return FALSE;
	pLSD[nSubItem].m_strText = lpszText;
	return TRUE;
}

BOOL CSkinList::SetItemData( int nItem, DWORD dwData )
{
	if (nItem < 0 || nItem >= GetItemCount() ) return FALSE;
	LISTCTRLEXDATA* pLSD = (LISTCTRLEXDATA*)( CListCtrl::GetItemData(nItem) );
	if ( NULL == pLSD ) return FALSE;
	pLSD[0].m_dwData = dwData;
	return TRUE;

}

DWORD CSkinList::GetItemData(int nItem) const
{
	if (nItem < 0 || nItem >= GetItemCount() ) return 0;
	LISTCTRLEXDATA* pLSD = (LISTCTRLEXDATA*)( CListCtrl::GetItemData(nItem) );
	if ( NULL == pLSD ) return 0;
	return pLSD[0].m_dwData ;
}

int CSkinList::GetColumnCount() 
{
	return  ( ( GetHeaderCtrl()->GetItemCount() ) == 0 ) ? 1 : GetHeaderCtrl()->GetItemCount();
}



void CSkinList::DrawReport( int nItem, HDC hdc )
{

	RECT rrr = {0};
	GetClientRect( &rrr );
	
	COLORREF crText = m_crText;
	COLORREF crBk   = m_crBackground;
	if ( CListCtrl::GetItemState(nItem, LVIS_SELECTED) )
	{
		crText = m_crHighLightText;
		crBk =  m_crHighLight;
	}

	CDC cdc;
	cdc.Attach(hdc);
	CRect rt ;
	int n = GetColumnCount();

	LISTCTRLEXDATA* pLSD = (LISTCTRLEXDATA*)( CListCtrl::GetItemData(nItem) );
	if ( NULL == pLSD ) return;

	for ( int i = 0; i < n; ++i ){

		if ( 0 == i ){
			CListCtrl::GetItemRect( nItem, &rt, LVIR_BOUNDS );
			rt.right = GetColumnWidth( 0 );
		}
		else {
			CListCtrl::GetSubItemRect(nItem, i, LVIR_BOUNDS, rt);	
		}
		cdc.FillSolidRect(&rt, crBk);


		cdc.SetBkMode(TRANSPARENT);
		cdc.SetTextColor(crText);
		cdc.SetBkColor(crBk);

		CFont* ptf = GetHeaderCtrl()->GetFont(); 
		CFont* pOld = cdc.SelectObject( ptf );
		rt.left += 5;
		cdc.DrawText(pLSD[i].m_strText, &rt, DT_LEFT);	
		cdc.SelectObject( pOld );
	}
	cdc.Detach();
}



BOOL CSkinList::OnEraseBkgnd(CDC* pDC) 
{	
	ASSERT(pDC->GetSafeHdc());
	return false;
}

void CSkinList::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (m_nItemHeight > 0)
	{
		CRect rt;
		this->GetClientRect(&rt);
		lpMeasureItemStruct->itemWidth = rt.Width();
		lpMeasureItemStruct->itemHeight = m_nItemHeight;
	}
}


BOOL CSkinList::OnLButtonDown( NMHDR * pNotifyStruct, LRESULT* result ) 
{
	// TODO: Add your message handler code here and/or call default

	*result = CDRF_DODEFAULT;
	NMCLICK* pclick = (NMCLICK*)pNotifyStruct;

	if ( NULL == pclick ) return FALSE;
	if ( (pclick->dwItemSpec) < 0 || (pclick->dwItemSpec) >= GetItemCount() ) return FALSE;
	CListCtrl::SetItemState(pclick->dwItemSpec, LVIS_SELECTED, LVIS_SELECTED);
	UpdateWindow();
	return FALSE;
}

void CSkinList::PreSubclassWindow()
{
//	SetExtendedStyle( GetStyle() | LVS_EX_TRACKSELECT );
	
	CListCtrl::PreSubclassWindow();
}

BOOL CSkinList::DeleteItem( int nItem )
{
	LISTCTRLEXDATA* pLSCD = (LISTCTRLEXDATA*)(CListCtrl::GetItemData(nItem));
	if ( pLSCD ) delete [] pLSCD;
	BOOL ret = CListCtrl::DeleteItem(nItem);
	UpdateWindow();
	return ret;
}

BOOL CSkinList::DeleteAllItems()
{
	int nCount = CListCtrl::GetItemCount();
	for ( int i = 0; i< nCount; ++i ){
		LISTCTRLEXDATA* pLSCD = (LISTCTRLEXDATA*)(CListCtrl::GetItemData( i ));
		if ( pLSCD ) delete [] pLSCD;
		 
	}
	BOOL ret = CListCtrl::DeleteAllItems();
	UpdateWindow();
	return ret;
}

void CSkinList::UpdateWindow()
{
	RECT wrt = {0};
	this->GetClientRect( &wrt );
	InvalidateRect( &wrt );
}
