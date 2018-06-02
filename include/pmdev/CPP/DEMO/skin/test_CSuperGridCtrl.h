#ifndef IDB_FOLDERS
#define IDB_FOLDERS 1
#endif
static HIMAGELIST CImageList_Create( UINT nBitmapID, int cx, int nGrow, COLORREF crMask ) {
  ASSERT( HIWORD( nBitmapID ) == 0 );
  HINSTANCE hInst = ( HINSTANCE ) GetModuleHandle( NULL );
  //HINSTANCE hInst = AfxFindResourceHandle((LPCTSTR)nBitmapID, RT_BITMAP);
  ASSERT( hInst != NULL );
  return ( ImageList_LoadBitmap( hInst, ( LPCTSTR ) nBitmapID, cx, nGrow, crMask ) );
}
static HIMAGELIST CListCtrl_SetImageList( HWND m_hWnd, HIMAGELIST pImageList, int nImageList ) {
  ASSERT( ::IsWindow( m_hWnd ) );
  return ( ( HIMAGELIST ) ::SendMessage( m_hWnd, LVM_SETIMAGELIST, nImageList, ( LPARAM ) pImageList ) );
}
static HIMAGELIST CListCtrl_GetImageList( HWND m_hWnd, int nImageListType ) {
  ASSERT( ::IsWindow( m_hWnd ) );
  return ( ( HIMAGELIST ) ::SendMessage( m_hWnd, LVM_GETIMAGELIST, nImageListType, 0L ) );
}
static DWORD CListCtrl_SetExtendedStyle( HWND m_hWnd, DWORD dwNewStyle ) {
  ASSERT( ::IsWindow( m_hWnd ) );
  return ( DWORD ) ::SendMessage( m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, ( LPARAM ) dwNewStyle );
}
static int CListCtrl_InsertColumn( HWND m_hWnd, int nCol, const LVCOLUMN* pColumn ) {
  ASSERT( ::IsWindow( m_hWnd ) );
  return ( int ) ::SendMessage( m_hWnd, LVM_INSERTCOLUMN, nCol, ( LPARAM ) pColumn );
}
static HTREEITEM CTreeCtrl_InsertItem( HWND m_hWnd, UINT nMask, LPCTSTR lpszItem, int nImage,
                                       int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam,
                                       HTREEITEM hParent, HTREEITEM hInsertAfter ) {
  ASSERT( ::IsWindow( m_hWnd ) );
  TVINSERTSTRUCT tvis;
  tvis.hParent = hParent;
  tvis.hInsertAfter = hInsertAfter;
  tvis.item.mask = nMask;
  tvis.item.pszText = ( LPTSTR ) lpszItem;
  tvis.item.iImage = nImage;
  tvis.item.iSelectedImage = nSelectedImage;
  tvis.item.state = nState;
  tvis.item.stateMask = nStateMask;
  tvis.item.lParam = lParam;
  return ( HTREEITEM ) ::SendMessage( m_hWnd, TVM_INSERTITEM, 0, ( LPARAM ) & tvis );
}
static HTREEITEM CTreeCtrl_InsertItem( HWND m_hWnd, LPTVINSERTSTRUCT lpInsertStruct ) {
  ASSERT( ::IsWindow( m_hWnd ) );
  return ( HTREEITEM ) ::SendMessage( m_hWnd, TVM_INSERTITEM, 0, ( LPARAM ) lpInsertStruct );
}
static HTREEITEM CTreeCtrl_InsertItem( HWND m_hWnd, LPCTSTR lpszItem, int nImage,
                                       int nSelectedImage, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST, LPARAM lParam=0 ) {
  ASSERT( ::IsWindow( m_hWnd ) );
  return CTreeCtrl_InsertItem( m_hWnd, TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE, lpszItem, nImage, nSelectedImage, 0, 0, lParam, hParent, hInsertAfter );
}
static HTREEITEM CTreeCtrl_InsertItem( HWND m_hWnd, LPCTSTR lpszItem, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST, LPARAM lParam=0  ) {
  ASSERT( ::IsWindow( m_hWnd ) );
  return CTreeCtrl_InsertItem( m_hWnd, TVIF_TEXT, lpszItem, 0, 0, 0, 0, lParam, hParent, hInsertAfter );
}
static BOOL CListCtrl_SetCheck( HWND m_hWnd, int nItem, BOOL fCheck = TRUE ) {
  ASSERT( ::IsWindow( m_hWnd ) );
  LVITEM lvi;
  lvi.stateMask = LVIS_STATEIMAGEMASK;
  /*
  Since state images are one-based, 1 in this macro turns the check off, and
  2 turns it on.
  */
  lvi.state = INDEXTOSTATEIMAGEMASK( ( fCheck ? 2 : 1 ) );
  return ( BOOL ) ::SendMessage( m_hWnd, LVM_SETITEMSTATE, nItem, ( LPARAM ) & lvi );
}
static BOOL CTreeCtrl_SetItem( HWND m_hWnd, TVITEM* pItem ) {
  ASSERT( ::IsWindow( m_hWnd ) );
  return ( BOOL ) ::SendMessage( m_hWnd, TVM_SETITEM, 0, ( LPARAM ) pItem );
}
static BOOL CTreeList_SetItem( HWND m_hWnd, int nItem, int nSubItem, LPCTSTR lpszItem, LPARAM lParam = 0 ) {
  ASSERT( ::IsWindow( m_hWnd ) );
  LV_ITEM lvi = {0};
  lvi.mask = LVIF_TEXT | LVIF_PARAM;
  lvi.lParam = lParam;
  lvi.mask = LVIF_TEXT;
  lvi.iItem = nItem;
  lvi.iSubItem = nSubItem;
  lvi.pszText = ( LPTSTR ) lpszItem;
  return CTreeCtrl_SetItem( m_hWnd, ( TVITEM* ) & lvi );
}
static BOOL CTreeCtrl_Expand( HWND m_hWnd, HTREEITEM hItem, UINT nCode ) {
  ASSERT( ::IsWindow( m_hWnd ) );
  return ( BOOL ) ::SendMessage( m_hWnd, TVM_EXPAND, nCode, ( LPARAM ) hItem );
}
static BOOL CTreeCtrl_DeleteItem(HWND m_hWnd, int nItem)
	{ ASSERT(::IsWindow(m_hWnd)); return (BOOL) ::SendMessage(m_hWnd, TVM_DELETEITEM, nItem, 0L); }
static BOOL CTreeCtrl_DeleteItem(HWND m_hWnd, HTREEITEM hItem)
	{ ASSERT(::IsWindow(m_hWnd)); return (BOOL)::SendMessage(m_hWnd, TVM_DELETEITEM, -1, (LPARAM)hItem); }
static BOOL CTreeCtrl_DeleteAllItems(HWND m_hWnd)
	{ ASSERT(::IsWindow(m_hWnd)); return (BOOL)::SendMessage(m_hWnd, TVM_DELETEITEM, 0, (LPARAM)TVI_ROOT); }
int test_CSuperGridCtrl( CSuperGridCtrl* This ) {
  int i, j;
  int col = 5, row = 9;
  HIMAGELIST hIml = CImageList_Create( IDB_FOLDERS, 16, 1, RGB( 0, 255, 255 ) );
  if ( !( hIml ) )
    return -1;
  CListCtrl_SetImageList( This->m_hWnd, hIml, LVSIL_SMALL );
  HIMAGELIST pImageList = CListCtrl_GetImageList( This->m_hWnd, LVSIL_SMALL );
  if ( pImageList ) {
    ImageList_GetIconSize( pImageList, &This->m_cxImage, &This->m_cyImage );
  } else {
    return -1;
  }
  //CItemInfo lpp;
  CListCtrl_SetExtendedStyle( This->m_hWnd, LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES );
  LV_COLUMN lvColumn;
  //initialize the columns
  lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
  lvColumn.fmt = LVCFMT_LEFT;
  lvColumn.cx = 100;
  for ( i = 0; i < col; i++ ) {
    char buf[ 64 ];
    _snprintf( buf, 64, "Column %d", i );
    //make the secondary columns smaller
    lvColumn.pszText = buf;
    CListCtrl_InsertColumn( This->m_hWnd, i, &lvColumn );
  }
  HTREEITEM fas[100];
  HTREEITEM fa1 = TVI_ROOT, fa = TVI_ROOT;
  TLCONTROLINFO info[] = {
    {TLCT_TEXT, RGB(255, 0,0), ""},
    {TLCT_TEXT, RGB(0, 255,0), ""},
    {TLCT_TEXT, RGB(0, 0,255), ""},
    {TLCT_EDIT, RGB(255, 255,0), ""},
    {TLCT_COMBOBOX, RGB(0, 255,255), "1\n2\n3"}
  };
  for ( i = 0; i < row; ++i ) {
    char buf[ 64 ];
    j = 0;
    _snprintf( buf, 64, "%d, %d", i, j );
    if ( 0 == i % 3 ) {
      fa = TVI_ROOT;
    } else {
      fa = fa1;
      fa = TVI_ROOT;
    }
    fa1 = CTreeCtrl_InsertItem( This->m_hWnd, buf, fa, TVI_LAST, 0 );
    fas[i] = fa1;
    if ( 1 == i % 3 ) {
      CListCtrl_SetCheck( This->m_hWnd, i, TRUE );
    }
    for ( j = 1; j < col; ++j ) {
      _snprintf( buf, 64, "%d, %d", i, j );
      CTreeList_SetItem( This->m_hWnd, i, j, buf, 0 );
    }
    for ( j = 1; j < col; ++j ) {
      LPARAM lp=(LPARAM)(info+j);
      //CTreeList_SetItem( This->m_hWnd, i, j, 0, lp );
    }
  }
  for ( i = 0; i < row; ++i ) {
    fa1 = ( HTREEITEM ) This->m_Items[ i ];
    CTreeCtrl_Expand( This->m_hWnd, fa1, 0 );
  }
  CTreeCtrl_DeleteItem(This->m_hWnd, fas[2]);
  CTreeCtrl_DeleteItem(This->m_hWnd, 2);
  CTreeCtrl_DeleteAllItems(This->m_hWnd);
  return 0;
}
