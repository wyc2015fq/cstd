
#if 0

  void SetPos( RECT rc ) {
    CContainerUI::SetPos( rc );
    {
      RECT r = this->m_rcItem;
      RECT b( 0, 0, GetTotalWidth(), GetTotalHeight() );
      ClientToScreen( b );
      int aa = this->m_pVerticalScrollbar->GetFixedWidth();
      r.right -= aa;
      this->m_pVerticalScrollbar->SetScrollRange( b.Height() - r.Height() );
      if ( b.Height() > r.Height() ) {
        this->m_pVerticalScrollbar->SetVisible( true );
      }
      CContainerUI::ProcessScrollbar( r, b.Width(), b.Height() );
    }
  }
  void SetAttribute( LPCTSTR pstrName, LPCTSTR pstrValue ) {
    Initialise();
    EnableScrollBar();
    CContainerUI::SetAttribute( pstrName, pstrValue );
  }
  BOOL Initialise() {
#define IDB_LISTITEMS "ListItems.bmp"
#define IDC_DIVIDER "divider.cur"
#define IDC_HYPERLINK "Hand.cur"
    // load list images
    if ( NULL == this->m_ilListItems.m_hImageList && !m_ilListItems.CreateFromImage( IDB_LISTITEMS, 16, 0, RGB( 255, 0, 255 ), IMAGE_BITMAP, LR_CREATEDIBSECTION ) )
      return FALSE;
    if ( NULL == this->m_curDivider.m_hCursor && this->m_curDivider.LoadCursor( IDC_DIVIDER ) == NULL )
      return FALSE;
    if ( NULL == this->m_curHyperLink.m_hCursor && this->m_curHyperLink.LoadCursor( IDC_HYPERLINK ) == NULL )
      return FALSE;
    // load interface settings
    if ( !LoadSettings() )
      return FALSE;
    // give control a static border
    //ModifyStyle( WS_BORDER, WS_CLIPCHILDREN );
    //ModifyStyleEx( WS_EX_CLIENTEDGE, WS_EX_STATICEDGE, SWP_FRAMECHANGED );
    // register drag drop
    this->m_oleDragDrop.Register( this );
    this->m_oleDragDrop.AddTargetFormat( this->m_nHeaderClipboardFormat );
    this->m_oleDragDrop.AddSourceFormat( this->m_nHeaderClipboardFormat );
    // create the tooltip
    //m_ttToolTip->SetMaxTipWidth( SHRT_MAX );
    return TRUE;
  }
  void ShowHeader( BOOL bShowHeader = TRUE ) {
    this->m_bShowHeader = bShowHeader;
    ResetScrollBars();
    Invalidate();
  }
  void ShowHeaderSort( BOOL bShowSort = TRUE ) {
    this->m_bShowSort = bShowSort;
    Invalidate();
  }
  void SetSingleSelect( BOOL bSingleSelect = TRUE ) {
    this->m_bSingleSelect = bSingleSelect;
    Invalidate();
  }
  void SetFocusSubItem( BOOL bFocusSubItem = TRUE ) {
    this->m_bFocusSubItem = bFocusSubItem;
    Invalidate();
  }
  void SetDragDrop( BOOL bDragDrop = TRUE ) {
    this->m_bDragDrop = bDragDrop;
  }
  void SetSmoothScroll( BOOL bSmoothScroll = TRUE ) {
    this->m_bSmoothScroll = bSmoothScroll;
  }
  void SetBackgroundImage( HBITMAP hBackgroundImage, BOOL bTileImage = FALSE ) {
    this->m_bmpBackground = hBackgroundImage;
    this->m_bTileBackground = bTileImage;
  }
  void SetImageList( CImageList& ilItemImages ) {
    this->m_ilItemImages = ilItemImages;
  }
  UINT ValidateFlags( UINT nFlags ) {
    if ( nFlags & ITEM_FLAGS_CENTRE )
      nFlags &= ~( ITEM_FLAGS_LEFT | ITEM_FLAGS_RIGHT );
    if ( nFlags & ITEM_FLAGS_RIGHT )
      nFlags &= ~ITEM_FLAGS_LEFT;
    if ( nFlags & ITEM_FLAGS_DATE_ONLY )
      nFlags &= ~ITEM_FLAGS_TIME_ONLY;
    if ( nFlags & ( ITEM_FLAGS_EDIT_NUMBER | ITEM_FLAGS_EDIT_FLOAT ) )
      nFlags &= ~ITEM_FLAGS_EDIT_UPPER;
    if ( !( nFlags & ( ITEM_FLAGS_EDIT_NUMBER | ITEM_FLAGS_EDIT_FLOAT ) ) )
      nFlags &= ~( ITEM_FLAGS_EDIT_NEGATIVE | ITEM_FLAGS_EDIT_OPERATOR );
    if ( nFlags & ITEM_FLAGS_COMBO_EDIT )
      nFlags &= ~( ITEM_FLAGS_DATE_ONLY | ITEM_FLAGS_TIME_ONLY | ITEM_FLAGS_DATETIME_NONE );
    return nFlags;
  }
  void AddColumn( CListColumn& listColumn ) {
    // minimum column width
    if ( listColumn.m_strText.IsEmpty() && listColumn.m_nImage != ITEM_IMAGE_NONE ) {
      CSize sizeIcon;
      this->m_ilListItems.GetIconSize( sizeIcon );
      listColumn.m_nWidth = sizeIcon.cx + 5;
      listColumn.m_nFlags |= ITEM_FLAGS_CENTRE;
    }
    // correct incompatible flag mask values
    listColumn.m_nFlags = ValidateFlags( listColumn.m_nFlags );
    // initial data index
    listColumn.m_nIndex = GetColumnCount();
    this->m_aColumns.Add( listColumn );
    ResetScrollBars();
    Invalidate();
  }
  void AddColumn( LPCTSTR lpszText, int nWidth = 0, int nImage = ITEM_IMAGE_NONE, BOOL bFixed = FALSE, UINT nFormat = ITEM_FORMAT_NONE, UINT nFlags = ITEM_FLAGS_NONE ) {
    CListColumn listColumn;
    listColumn.m_strText = lpszText;
    listColumn.m_nWidth = nWidth;
    listColumn.m_bFixed = bFixed;
    listColumn.m_nFormat = nFormat;
    listColumn.m_nFlags = nFlags;
    listColumn.m_nImage = nImage;
    AddColumn( listColumn );
  }
  int GetColumnCount() {
    return this->m_aColumns.GetSize();
  }
  BOOL GetColumn( int nColumn, CListColumn& listColumn ) {
    if ( nColumn < 0 || nColumn >= GetColumnCount() )
      return FALSE;
    listColumn = this->m_aColumns[ nColumn ];
    return TRUE;
  }
  int GetTotalWidth( BOOL bRecalc = FALSE ) {
    bRecalc = 1;
    if ( bRecalc ) {
      this->m_nTotalWidth = 0;
      for ( int nColumn = 0; nColumn < GetColumnCount(); nColumn++ )
        this->m_nTotalWidth += GetColumnWidth( nColumn );
    }
    return this->m_nTotalWidth - 1;
  }
  virtual int GetTotalHeight() {
    return max( ( this->GetItemCount() * this->m_nItemHeight ) + ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 ), 1 );
  }
  BOOL SetColumnWidth( int nColumn, int nWidth ) {
    if ( nColumn < 0 || nColumn >= GetColumnCount() )
      return FALSE;
    // set new column size if not fixed
    if ( !m_aColumns[ nColumn ].m_bFixed ) {
      this->m_aColumns[ nColumn ].m_nWidth = nWidth;
      ResetScrollBars();
      Invalidate();
    }
    return TRUE;
  }
  int GetColumnWidth( int nColumn ) {
    CListColumn listColumn;
    return GetColumn( nColumn, listColumn ) ? listColumn.m_nWidth : 0;
  }
  int GetColumnIndex( int nColumn ) {
    CListColumn listColumn;
    return GetColumn( nColumn, listColumn ) ? listColumn.m_nIndex : 0;
  }
  int IndexToOrder( int nIndex ) {
    for ( int nColumn = 0; nColumn < GetColumnCount(); nColumn++ ) {
      if ( GetColumnIndex( nColumn ) == nIndex )
        return nColumn;
    }
    return -1;
  }
  BOOL SetColumnFormat( int nColumn, UINT nFormat, UINT nFlags = ITEM_FLAGS_NONE ) {
    if ( nColumn < 0 || nColumn >= GetColumnCount() )
      return FALSE;
    this->m_aColumns[ nColumn ].m_nFormat = nFormat;
    this->m_aColumns[ nColumn ].m_nFlags = ValidateFlags( nFlags );
    return TRUE;
  }
  BOOL SetColumnFormat( int nColumn, UINT nFormat, UINT nFlags, CListArray < CStdString >& aComboList ) {
    if ( nColumn < 0 || nColumn >= GetColumnCount() )
      return FALSE;
    this->m_aColumns[ nColumn ].m_nFormat = nFormat;
    this->m_aColumns[ nColumn ].m_nFlags = ValidateFlags( nFlags );
    this->m_aColumns[ nColumn ].m_aComboList = aComboList;
    return TRUE;
  }
  UINT GetColumnFormat( int nColumn ) {
    CListColumn listColumn;
    return GetColumn( nColumn, listColumn ) ? listColumn.m_nFormat : ITEM_FORMAT_NONE;
  }
  UINT GetColumnFlags( int nColumn ) {
    CListColumn listColumn;
    return GetColumn( nColumn, listColumn ) ? listColumn.m_nFlags : ITEM_FLAGS_NONE;
  }
  BOOL GetColumnComboList( int nColumn, CListArray < CStdString >& aComboList ) {
    CListColumn listColumn;
    if ( !GetColumn( nColumn, listColumn ) )
      return FALSE;
    aComboList = listColumn.m_aComboList;
    return !aComboList.IsEmpty();
  }
  BOOL GetColumnRect( int nColumn, RECT& rcColumn ) {
    if ( nColumn < 0 || nColumn >= GetColumnCount() )
      return FALSE;
    GetClientRect( rcColumn );
    rcColumn.bottom = this->m_nHeaderHeight;
    for ( int nColumnOrder = 0; nColumnOrder < GetColumnCount(); nColumnOrder++ ) {
      int nWidth = GetColumnWidth( nColumnOrder );
      if ( nColumn == nColumnOrder ) {
        rcColumn.right = rcColumn.left + nWidth;
        break;
      }
      rcColumn.left += nWidth;
    }
    // offset column by scroll position
    rcColumn.OffsetRect( -GetScrollPos().cx, 0 );
    return TRUE;
  }
  BOOL AddItem() {
    ResetScrollBars();
    Invalidate();
    return TRUE;
  }
  BOOL DeleteItem( int nItem ) {
    this->m_setSelectedItems.erase( nItem );
    ResetScrollBars();
    return Invalidate();
  }
  BOOL DeleteAllItems() {
    this->m_setSelectedItems.clear();
    ResetScrollBars();
    return Invalidate();
  }
  // must be implemented in a derived class
  virtual int GetItemCount() const = 0; //   ATLASSERT( FALSE );
  virtual CStdString GetItemText( int nItem, int nSubItem ) {
    ATLASSERT( FALSE ); // must be implemented in a derived class
    return _T( "" );
  }
  BOOL GetItemDate( int nItem, int nSubItem, SYSTEMTIME& stItemDate ) {
    ZeroMemory( &stItemDate, sizeof( SYSTEMTIME ) );
    CStdString strItemText = this->GetItemText( nItem, nSubItem );
    if ( strItemText.IsEmpty() )
      return FALSE;
    // get date-time from item text: yyyymmddhhmmss
    stItemDate.wYear = _ttoi( strItemText.Left( 4 ) );
    stItemDate.wMonth = _ttoi( strItemText.Mid( 4, 2 ) );
    stItemDate.wDay = _ttoi( strItemText.Mid( 6, 2 ) );
    stItemDate.wHour = _ttoi( strItemText.Mid( 8, 2 ) );
    stItemDate.wMinute = _ttoi( strItemText.Mid( 10, 2 ) );
    stItemDate.wSecond = _ttoi( strItemText.Mid( 12, 2 ) );
    stItemDate.wMilliseconds = 0;
    return TRUE;
  }
  virtual int GetItemImage( int nItem, int nSubItem ) {
    return ITEM_IMAGE_NONE; // may be implemented in a derived class
  }
  UINT GetItemFormat( int nItem, int nSubItem ) {
    return GetColumnFormat( IndexToOrder( nSubItem ) ); // may be implemented in a derived class
  }
  UINT GetItemFlags( int nItem, int nSubItem ) {
    return GetColumnFlags( IndexToOrder( nSubItem ) ); // may be implemented in a derived class
  }
  BOOL GetItemComboList( int nItem, int nSubItem, CListArray < CStdString >& aComboList ) {
    return GetColumnComboList( IndexToOrder( nSubItem ), aComboList ); // may be implemented in a derived class
  }
  HFONT GetItemFont( int nItem, int nSubItem ) {
    return this->m_fntListFont; // may be implemented in a derived class
  }
  virtual BOOL GetItemColours( int nItem, int nSubItem, COLORREF& rgbBackground, COLORREF& rgbText ) {
    rgbBackground = this->m_rgbBackground;
    rgbText = this->m_rgbItemText;
    return TRUE;
  }
  virtual CStdString GetItemToolTip( int nItem, int nSubItem ) {
    return _T( "" ); // may be implemented in a derived class
  }
  virtual BOOL SetItemText( int nItem, int nSubItem, LPCTSTR lpszText ) {
    ATLASSERT( FALSE ); // must be implemented in a derived class
    return FALSE;
  }
  virtual BOOL SetItemComboIndex( int nItem, int nSubItem, int nIndex ) {
    ATLASSERT( FALSE ); // must be implemented in a derived class
    return FALSE;
  }
  BOOL SetItemDate( int nItem, int nSubItem, SYSTEMTIME& stItemDate ) {
    // set date-time in format (yyyymmddhhmmss)
    CStdString strFormatDate;
    strFormatDate.Format( _T( "%04d%02d%02d%02d%02d%02d" ), stItemDate.wYear, stItemDate.wMonth, stItemDate.wDay, stItemDate.wHour, stItemDate.wMinute, stItemDate.wSecond );
    return this->SetItemText( nItem, nSubItem, strFormatDate );
  }
  BOOL SetItemCheck( int nItem, int nSubItem, int nCheckValue ) {
    switch ( this->GetItemFormat( nItem, nSubItem ) ) {
    case ITEM_FORMAT_CHECKBOX:
      return this->SetItemText( nItem, nSubItem, nCheckValue > 0 ? _T( "1" ) : _T( "0" ) );
    case ITEM_FORMAT_CHECKBOX_3STATE:
      if ( nCheckValue < 0 )
        return this->SetItemText( nItem, nSubItem, _T( "-1" ) );
      if ( nCheckValue > 0 )
        return this->SetItemText( nItem, nSubItem, _T( "1" ) );
      return this->SetItemText( nItem, nSubItem, _T( "0" ) );
    }
    return FALSE;
  }
  virtual BOOL SetItemImage( int nItem, int nSubItem, int nImage ) {
    ATLASSERT( FALSE ); // must be implemented in a derived class
    return FALSE;
  }
  virtual BOOL SetItemFormat( int nItem, int nSubItem, UINT nFormat, UINT nFlags = ITEM_FLAGS_NONE ) {
    ATLASSERT( FALSE ); // must be implemented in a derived class
    return FALSE;
  }
  virtual BOOL SetItemFormat( int nItem, int nSubItem, UINT nFormat, UINT nFlags, CListArray < CStdString >& aComboList ) {
    ATLASSERT( FALSE ); // must be implemented in a derived class
    return FALSE;
  }
  virtual BOOL SetItemFont( int nItem, int nSubItem, HFONT hFont ) {
    ATLASSERT( FALSE ); // must be implemented in a derived class
    return FALSE;
  }
  virtual BOOL SetItemColours( int nItem, int nSubItem, COLORREF rgbBackground, COLORREF rgbText ) {
    ATLASSERT( FALSE ); // must be implemented in a derived class
    return FALSE;
  }
  virtual void ReverseItems() {
    ATLASSERT( FALSE ); // must be implemented in a derived class
  }
  virtual void SortItems( int nColumn, BOOL bAscending ) {
    ATLASSERT( FALSE ); // must be implemented in a derived class
  }
  BOOL GetItemRect( int nItem, int nSubItem, RECT& rcItem ) {
    int nTopItem = GetTopItem();
    if ( nItem < nTopItem || nItem >= this->GetItemCount() || nItem >= nTopItem + GetCountPerPage() )
      return FALSE;
    RECT rcClient;
    GetClientRect( rcClient );
    // calculate item rect based on scroll position
    rcItem = rcClient;
    rcItem.top = ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 ) + ( ( nItem - nTopItem ) * this->m_nItemHeight );
    rcItem.bottom = rcItem.top + this->m_nItemHeight;
    rcItem.right = min( rcClient.right, GetTotalWidth() );
    if ( nSubItem != NULL_SUBITEM ) {
      RECT rcColumn;
      if ( !GetColumnRect( nSubItem, rcColumn ) )
        return FALSE;
      rcItem.left = rcColumn.left;
      rcItem.right = rcColumn.right;
    }
    return TRUE;
  }
  BOOL GetItemRect( int nItem, RECT& rcItem ) {
    return GetItemRect( nItem, NULL_SUBITEM, rcItem );
  }
  BOOL InvalidateItem( int nItem, int nSubItem = NULL_SUBITEM ) {
    RECT rcItem;
    return GetItemRect( nItem, nSubItem, rcItem ) ? Invalidate() : FALSE;
  }
  BOOL InvalidateRect( RECT* x ) {
    Invalidate();
    return TRUE;
  }
  BOOL InvalidateHeader() {
    if ( !m_bShowHeader )
      return TRUE;
    RECT rcHeader;
    if ( !GetClientRect( rcHeader ) )
      return FALSE;
    rcHeader.bottom = this->m_nHeaderHeight;
    return InvalidateRect( rcHeader );
  }
  int GetCountPerPage( BOOL bPartial = TRUE ) {
    RECT rcClient;
    GetClientRect( rcClient );
    rcClient.top = ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 );
    // calculate number of items per control height (include partial item)
    div_t divHeight = div( rcClient.Height(), this->m_nItemHeight );
    // round up to nearest item count
    return max( bPartial && divHeight.rem > 0 ? divHeight.quot + 1 : divHeight.quot, 1 );
  }
  BOOL IsItemVisible( int nItem, int nSubItem = NULL_SUBITEM, BOOL bPartial = TRUE ) {
    self * pT = this;
    int nTopItem = GetTopItem();
    if ( nItem < nTopItem || nItem >= pT->GetItemCount() )
      return FALSE;
    // check whether item is visible
    if ( nItem < nTopItem || nItem >= nTopItem + GetCountPerPage( bPartial ) )
      return FALSE;
    // check whether subitem is visible
    if ( this->m_bFocusSubItem && nSubItem != NULL_SUBITEM ) {
      RECT rcColumn;
      if ( !GetColumnRect( nSubItem, rcColumn ) )
        return FALSE;
      RECT rcClient;
      GetClientRect( rcClient );
      if ( rcColumn.left < rcClient.left || rcColumn.right > rcClient.right )
        return FALSE;
    }
    return TRUE;
  }
  BOOL EnsureItemVisible( int nItem, int nSubItem = NULL_SUBITEM ) {
    if ( IsItemVisible( nItem, nSubItem, FALSE ) )
      return TRUE;
    HideTitleTip();
    RECT rcClient;
    GetClientRect( rcClient );
    rcClient.top = ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 );
    RECT rcItem;
    rcItem.top = ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 ) + ( ( nItem - GetTopItem() ) * this->m_nItemHeight );
    rcItem.bottom = rcItem.top + this->m_nItemHeight;
    if ( rcItem.top < rcClient.top || rcItem.bottom > rcClient.bottom ) {
      int nScrollItem = NULL_ITEM;
      // scroll list up/down to include item
      if ( rcItem.top < rcClient.top || rcItem.Height() > rcClient.Height() )
        nScrollItem = nItem;
      else if ( rcItem.bottom > rcClient.bottom )
        nScrollItem = nItem - ( GetCountPerPage( FALSE ) - 1 );
      if ( nScrollItem != NULL_ITEM )
        //SetScrollPos( SB_VERT, nScrollItem * this->m_nItemHeight );
        this->m_pVerticalScrollbar->SetScrollPos( nScrollItem * this->m_nItemHeight );
    }
    if ( this->m_bFocusSubItem && nSubItem != NULL_SUBITEM ) {
      RECT rcColumn;
      if ( !GetColumnRect( nSubItem, rcColumn ) )
        return FALSE;
      RECT rcClient;
      GetClientRect( rcClient );
      int nScrollPos = 0;
      // scroll list left/right to include subitem
      if ( rcColumn.Width() > rcClient.Width() || rcColumn.left < 0 )
        nScrollPos = rcColumn.left;
      else if ( rcColumn.right > rcClient.right )
        nScrollPos = rcColumn.right - rcClient.right;
      if ( nScrollPos != 0 )
        //SetScrollPos( SB_HORZ, GetScrollPos().cx + nScrollPos );
        this->m_pHorizontalScrollbar->SetScrollPos( GetScrollPos().cx + nScrollPos );
    }
    return Invalidate();
  }
  void ShowScrollBar( int nScrollBar, BOOL bShow = TRUE ) {
    switch ( nScrollBar ) {
    case SB_HORZ:
      this->m_bShowHorizScroll = bShow;
      break;
    case SB_VERT:
      this->m_bShowVertScroll = bShow;
      break;
    case SB_BOTH:
      this->m_bShowHorizScroll = bShow;
      this->m_bShowVertScroll = bShow;
      break;
    }
    ResetScrollBars();
    Invalidate();
  }
  void ResetScrollBars( int nScrollBar = SB_BOTH, int nScrollPos = -1, BOOL bRecalc = TRUE ) {
#if 0
    self * pT = this;
    RECT rcClient;
    GetClientRect( rcClient );
    SCROLLINFO infoScroll;
    infoScroll.cbSize = sizeof( SCROLLINFO );
    infoScroll.fMask = nScrollPos < 0 ? SIF_PAGE | SIF_RANGE : SIF_PAGE | SIF_RANGE | SIF_POS;
    infoScroll.nPos = nScrollPos;
    infoScroll.nMin = 0;
    if ( ( nScrollBar == SB_BOTH || nScrollBar == SB_VERT ) && this->m_bShowVertScroll ) {
      infoScroll.nMax = ( pT->GetItemCount() * this->m_nItemHeight ) + ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 );
      infoScroll.nPage = rcClient.Height() - ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 );
      // are we within client range?
      if ( ( UINT ) infoScroll.nMax <= infoScroll.nPage + ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 ) )
        infoScroll.nMax = 0;
      // set vertical scroll bar
      this->m_bEnableVertScroll = SetScrollInfo( SB_VERT, &infoScroll, TRUE ) ? ( infoScroll.nMax > 0 ) : FALSE;
    }
    if ( ( nScrollBar == SB_BOTH || nScrollBar == SB_HORZ ) && this->m_bShowHorizScroll ) {
      infoScroll.nMax = GetTotalWidth( bRecalc );
      infoScroll.nPage = rcClient.Width();
      // are we within client range?
      if ( infoScroll.nPage >= ( UINT ) infoScroll.nMax )
        infoScroll.nMax = 0;
      // set horizontal scroll bar
      this->m_bEnableHorizScroll = SetScrollInfo( SB_HORZ, &infoScroll, TRUE ) ? ( infoScroll.nMax > ( int ) infoScroll.nPage ) : FALSE;
    }
#endif
  }
  BOOL IsScrollBarVisible( int nScrollBar ) {
    switch ( nScrollBar ) {
    case SB_HORZ:
      return this->m_bEnableHorizScroll;
    case SB_VERT:
      return this->m_bEnableVertScroll;
    case SB_BOTH:
      return ( this->m_bEnableHorizScroll && this->m_bEnableVertScroll );
    default:
      return FALSE;
    }
  }
  BOOL ResetSelected() {
    this->m_setSelectedItems.clear();
    this->m_nFocusItem = NULL_ITEM;
    this->m_nFocusSubItem = NULL_SUBITEM;
    this->m_nFirstSelected = NULL_ITEM;
    return Invalidate();
  }
  BOOL SelectItem( int nItem, int nSubItem = NULL_SUBITEM, UINT nFlags = 0 ) {
    self * pT = this;
    if ( nItem < 0 || nItem >= pT->GetItemCount() )
      return FALSE;
    BOOL bSelectItem = TRUE;
    BOOL bSelectRange = !m_bSingleSelect && ( nFlags & MK_SHIFT );
    BOOL bNewSelect = !( bSelectRange || ( nFlags & MK_CONTROL ) );
    BOOL bEnsureVisible = FALSE;
    // are we starting a new select sequence?
    if ( bNewSelect || bSelectRange ) {
      // are we simply reselecting the same item?
      if ( this->m_setSelectedItems.size() == 1 && *m_setSelectedItems.begin() == nItem ) {
        bSelectItem = FALSE;
        this->m_nFirstSelected = nItem;
        this->m_nFocusItem = nItem;
        this->m_nFocusSubItem = nSubItem;
      } else
        this->m_setSelectedItems.clear();
    } else // we adding to or removing from select sequence
    {
      if ( this->m_bSingleSelect )
        this->m_setSelectedItems.clear();
      set
        < int >::iterator posSelectedItem = this->m_setSelectedItems.find( nItem );
      // is this item already selected?
      if ( posSelectedItem != this->m_setSelectedItems.end() ) {
        bSelectItem = FALSE;
        this->m_setSelectedItems.erase( posSelectedItem );
        this->m_nFirstSelected = nItem;
        this->m_nFocusItem = nItem;
        this->m_nFocusSubItem = this->m_setSelectedItems.size() > 1 ? NULL_SUBITEM : nSubItem;
      }
    }
    // are we adding this item to the select sequence?
    if ( bSelectItem ) {
      bEnsureVisible = TRUE;
      if ( bSelectRange ) {
        if ( this->m_nFirstSelected == NULL_ITEM )
          this->m_nFirstSelected = nItem;
        for ( int nSelectedItem = min( this->m_nFirstSelected, nItem ); nSelectedItem <= max( this->m_nFirstSelected, nItem ); nSelectedItem++ )
          this->m_setSelectedItems.insert( nSelectedItem );
      } else {
        this->m_nFirstSelected = nItem;
        this->m_setSelectedItems.insert( nItem );
      }
      this->m_nFocusItem = nItem;
      this->m_nFocusSubItem = this->m_setSelectedItems.size() > 1 ? NULL_SUBITEM : nSubItem;
      // notify parent of selected item
      NotifyParent( this->m_nFocusItem, this->m_nFocusSubItem, LCN_SELECTED );
    }
    // start visible timer (scrolls list to partially hidden item)
    if ( !IsItemVisible( nItem, this->m_setSelectedItems.size() > 1 ? NULL_SUBITEM : nSubItem, FALSE ) )
      GetManager() ->SetTimer( this, ITEM_VISIBLE_TIMER, ITEM_VISIBLE_PERIOD );
    else if ( this->m_nFocusItem != NULL_ITEM && this->m_nFocusSubItem != NULL_SUBITEM )
      EditItem( this->m_nFocusItem, this->m_nFocusSubItem );
    return Invalidate();
  }
  BOOL IsSelected( int nItem ) {
    set
      < int >::iterator posSelectedItem = this->m_setSelectedItems.find( nItem );
    return ( posSelectedItem != this->m_setSelectedItems.end() );
  }
  BOOL GetSelectedItems( CListArray < int >& aSelectedItems ) {
    aSelectedItems.RemoveAll();
    for ( set
            < int >::iterator posSelectedItem = this->m_setSelectedItems.begin(); posSelectedItem != this->m_setSelectedItems.end(); ++posSelectedItem )
        aSelectedItems.Add( *posSelectedItem );
    return !aSelectedItems.IsEmpty();
  }
  BOOL SetFocusItem( int nItem, int nSubItem = NULL_SUBITEM ) {
    this->m_nFocusItem = nItem;
    this->m_nFocusSubItem = nSubItem;
    return EnsureItemVisible( this->m_nFocusItem, this->m_nFocusSubItem );
  }
  BOOL GetFocusItem( int& nItem, int& nSubItem ) {
    nItem = IsSelected( this->m_nFocusItem ) ? this->m_nFocusItem : ( this->m_setSelectedItems.empty() ? NULL_ITEM : *m_setSelectedItems.begin() );
    nSubItem = !m_bFocusSubItem || nItem == NULL_ITEM ? NULL_SUBITEM : this->m_nFocusSubItem;
    return ( nItem != NULL_ITEM );
  }
  int GetFocusItem() {
    return IsSelected( this->m_nFocusItem ) ? this->m_nFocusItem : ( this->m_setSelectedItems.empty() ? NULL_ITEM : *m_setSelectedItems.begin() );
  }
  BOOL HitTestHeader( CPoint point, int& nColumn, UINT& nFlags ) {
    // reset hittest flags
    nFlags = HITTEST_FLAG_NONE;
    if ( !m_bShowHeader )
      return FALSE;
    RECT rcClient;
    if ( !GetClientRect( rcClient ) )
      return FALSE;
    // are we over the header?
    if ( point.y < rcClient.top || point.y > this->m_nHeaderHeight )
      return FALSE;
    int nDividerPos = 0;
    int nColumnCount = GetColumnCount();
    // get hit-test subitem
    for ( nColumn = 0; nColumn < nColumnCount; nColumn++ ) {
      int nColumnWidth = GetColumnWidth( nColumn );
      nDividerPos += nColumnWidth;
      // offset divider position with current scroll position
      int nRelativePos = nDividerPos - GetScrollPos().cx;
      // are we over the divider zone?
      if ( point.x >= nRelativePos - DRAG_HEADER_OFFSET - 1 && point.x <= nRelativePos + DRAG_HEADER_OFFSET ) {
        nFlags |= HITTEST_FLAG_HEADER_DIVIDER;
        // are we to the left of the divider (or over last column divider)?
        if ( ( point.x >= nRelativePos - DRAG_HEADER_OFFSET - 1 && point.x < nRelativePos ) || nColumn + 1 >= nColumnCount - 1 ) {
          nFlags |= HITTEST_FLAG_HEADER_LEFT;
          return TRUE;
        }
        // find last zero-length column after this column
        for ( int nNextColumn = nColumn + 1; nNextColumn < nColumnCount; nNextColumn++ ) {
          if ( GetColumnWidth( nNextColumn ) > 0 )
            break;
          nColumn = nNextColumn;
        }
        nFlags |= HITTEST_FLAG_HEADER_RIGHT;
        return TRUE;
      }
      // are we over a column?
      if ( point.x > nRelativePos - nColumnWidth && point.x < nRelativePos )
        return TRUE;
    }
    return FALSE;
  }
  BOOL HitTest( CPoint point, int& nItem, int& nSubItem ) {
    self * pT = this;
    // are we over the header?
    if ( point.y < ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 ) )
      return FALSE;
    // calculate hit test item
    if ( this->m_nItemHeight > 0 ) {
      nItem = GetTopItem() + ( int ) ( ( point.y - ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 ) ) / this->m_nItemHeight );
    } else {
      nItem = -1;
    }
    if ( nItem < 0 || nItem >= pT->GetItemCount() )
      return FALSE;
    int nTotalWidth = 0;
    int nColumnCount = GetColumnCount();
    // get hit-test subitem
    for ( nSubItem = 0; nSubItem < nColumnCount; nSubItem++ ) {
      int nColumnWidth = GetColumnWidth( nSubItem );
      nTotalWidth += nColumnWidth;
      // offset position with current scroll position
      int nRelativePos = nTotalWidth - GetScrollPos().cx;
      // are we over a subitem?
      if ( point.x > nRelativePos - nColumnWidth && point.x < nRelativePos )
        return TRUE;
    }
    return FALSE;
  }
  BOOL AutoSizeColumn( int nColumn ) {
    self * pT = this;
    CListColumn listColumn;
    if ( !GetColumn( nColumn, listColumn ) || listColumn.m_bFixed )
      return FALSE;
    CClientDC dcClient( M_HWND );
    HFONT hOldFont = dcClient.SelectFont( this->m_fntListFont );
    // set to column text width if zero-length
    CSize sizeExtent;
    if ( !dcClient.GetTextExtent( listColumn.m_strText, -1, &sizeExtent ) )
      return FALSE;
    int nMaxWidth = sizeExtent.cx + ITEM_WIDTH_MARGIN;
    CSize sizeIcon( 0, 0 );
    if ( !m_ilItemImages.IsNull() )
      this->m_ilItemImages.GetIconSize( sizeIcon );
    // calculate maximum column width required
    for ( int nItem = 0; nItem < pT->GetItemCount(); nItem++ ) {
      if ( !dcClient.GetTextExtent( pT->GetItemText( nItem, listColumn.m_nIndex ), -1, &sizeExtent ) )
        return FALSE;
      if ( !m_ilItemImages.IsNull() && pT->GetItemImage( nItem, listColumn.m_nIndex ) != ITEM_IMAGE_NONE )
        sizeExtent.cx += sizeIcon.cx;
      nMaxWidth = max( nMaxWidth, ( int ) sizeExtent.cx + ITEM_WIDTH_MARGIN );
    }
    dcClient.SelectFont( hOldFont );
    return SetColumnWidth( nColumn, nMaxWidth );
  }
  void ResizeColumn( BOOL bColumnScroll = FALSE ) {
    HideTitleTip();
    int nCurrentPos = GET_X_LPARAM( GetMessagePos() );
    RECT rcClient;
    GetClientRect( rcClient );
    int nScrollLimit = GetTotalWidth() - rcClient.Width();
    if ( bColumnScroll ) {
      // have we finished scrolling list to accommodate new column size?
      if ( !m_bColumnSizing || !m_bEnableHorizScroll || nCurrentPos - this->m_nStartScrollPos > 0 ) {
        GetManager() ->KillTimer( this, RESIZE_COLUMN_TIMER );
        // reset resize start point
        this->m_nStartPos = nCurrentPos;
        this->m_bResizeTimer = FALSE;
      } else if ( nCurrentPos < this->m_nStartPos && GetScrollPos().cx >= nScrollLimit ) {
        // reset start column size
        this->m_nStartSize = max( GetColumnWidth( this->m_nColumnSizing ) + ( nCurrentPos - this->m_nStartScrollPos ), 0 );
        // resize column
        SetColumnWidth( this->m_nColumnSizing, this->m_nStartSize );
      }
    } else {
      int nColumnSize = max( this->m_nStartSize + ( nCurrentPos - this->m_nStartPos ), 0 );
      // are we scrolled fully to the right and wanting to reduce the size of a column?
      if ( this->m_bEnableHorizScroll && GetScrollPos().cx >= nScrollLimit && nColumnSize < GetColumnWidth( this->m_nColumnSizing ) ) {
        if ( !m_bResizeTimer ) {
          // only start the scroll timer once
          this->m_bResizeTimer = TRUE;
          // set new start scroll position
          this->m_nStartScrollPos = nCurrentPos;
          // start column resize / scroll timer
          GetManager() ->SetTimer( this, RESIZE_COLUMN_TIMER, RESIZE_COLUMN_PERIOD );
        }
      } else {
        // resizing is done in scroll timer (if started)
        if ( !m_bResizeTimer )
          SetColumnWidth( this->m_nColumnSizing, nColumnSize );
      }
    }
  }
  void DragColumn() {
    HideTitleTip();
    RECT rcColumn;
    if ( !GetColumnRect( this->m_nHighlightColumn, rcColumn ) )
      return ;
    RECT rcHeaderItem( rcColumn );
    rcHeaderItem.MoveToXY( 0, 0 );
    CListColumn listColumn;
    if ( !GetColumn( this->m_nHighlightColumn, listColumn ) )
      return ;
    // store drag column
    this->m_nDragColumn = this->m_nHighlightColumn;
    CClientDC dcClient( M_HWND );
    CDC dcHeader;
    dcHeader.CreateCompatibleDC( dcClient );
    int nContextState = dcHeader.SaveDC();
    // create drag header bitmap
    CBitmapHandle bmpHeader;
    bmpHeader.CreateCompatibleBitmap( dcClient, rcHeaderItem.Width(), rcHeaderItem.Height() );
    dcHeader.SelectBitmap( bmpHeader );
    {
      dcHeader.SetBkColor( this->m_rgbHeaderBackground );
      dcHeader.ExtTextOut( rcHeaderItem.left, rcHeaderItem.top, ETO_OPAQUE, rcHeaderItem, _T( "" ), 0, NULL );
      dcHeader.Draw3dRect( rcHeaderItem, this->m_rgbHeaderBorder, this->m_rgbHeaderShadow );
    }
    RECT rcHeaderText( rcHeaderItem );
    rcHeaderText.left += this->m_nHighlightColumn == 0 ? 4 : 3;
    rcHeaderText.OffsetRect( 0, 1 );
    // margin header text
    rcHeaderText.DeflateRect( 4, 0, 5, 0 );
    // has this header item an associated image?
    if ( listColumn.m_nImage != ITEM_IMAGE_NONE ) {
      CSize sizeIcon;
      this->m_ilListItems.GetIconSize( sizeIcon );
      RECT rcHeaderImage;
      rcHeaderImage.left = listColumn.m_strText.IsEmpty() ? ( ( rcHeaderText.left + rcHeaderText.right ) / 2 ) - ( sizeIcon.cx / 2 ) - ( ( !m_bShowThemed ) ? 0 : 1 ) : rcHeaderText.left;
      rcHeaderImage.right = min( rcHeaderImage.left + sizeIcon.cx, rcHeaderItem.right );
      rcHeaderImage.top = ( ( rcHeaderItem.top + rcHeaderItem.bottom ) / 2 ) - ( sizeIcon.cy / 2 );
      rcHeaderImage.bottom = min( rcHeaderImage.top + sizeIcon.cy, rcHeaderItem.bottom );
      this->m_ilListItems.DrawEx( listColumn.m_nImage, dcHeader, rcHeaderImage, CLR_DEFAULT, CLR_DEFAULT, ILD_TRANSPARENT );
      // offset header text (for image)
      rcHeaderText.left += sizeIcon.cx + 4;
    }
    dcHeader.SelectFont( this->m_fntListFont );
    dcHeader.SetTextColor( this->m_rgbHeaderText );
    dcHeader.SetBkMode( TRANSPARENT );
    UINT nFormat = DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS;
    if ( listColumn.m_nFlags & ITEM_FLAGS_CENTRE )
      nFormat |= DT_CENTER;
    else if ( listColumn.m_nFlags & ITEM_FLAGS_RIGHT )
      nFormat |= DT_RIGHT;
    else
      nFormat |= DT_LEFT;
    // draw header text
    if ( !listColumn.m_strText.IsEmpty() )
      dcHeader.DrawText( listColumn.m_strText, listColumn.m_strText.GetLength(), rcHeaderText, nFormat );
    dcHeader.RestoreDC( nContextState );
    SHDRAGIMAGE shDragImage;
    ZeroMemory( &shDragImage, sizeof( SHDRAGIMAGE ) );
    shDragImage.sizeDragImage.cx = rcHeaderItem.Width();
    shDragImage.sizeDragImage.cy = rcHeaderItem.Height();
    shDragImage.ptOffset.x = rcColumn.Width() / 2;
    shDragImage.ptOffset.y = rcColumn.Height() / 2;
    shDragImage.hbmpDragImage = bmpHeader;
    shDragImage.crColorKey = this->m_rgbBackground;
    // start header drag operation
    this->m_oleDragDrop.DoDragDrop( &shDragImage, DROPEFFECT_MOVE );
    // hide drop arrows after moving column
    this->m_wndDropArrows->Hide();
    if ( this->m_bButtonDown ) {
      ReleaseCapture();
      this->m_bButtonDown = FALSE;
      this->m_bBeginSelect = FALSE;
      this->m_ptDownPoint = 0;
      this->m_ptSelectPoint = 0;
    }
    // finish moving a column
    if ( this->m_nHighlightColumn != NULL_COLUMN ) {
      this->m_nHighlightColumn = NULL_COLUMN;
      InvalidateHeader();
    }
    this->m_nDragColumn = NULL_COLUMN;
  }
  BOOL DropColumn( CPoint point ) {
    if ( !m_bShowHeader )
      return FALSE;
    this->m_nHotDivider = NULL_COLUMN;
    this->m_nHotColumn = NULL_COLUMN;
    UINT nHeaderFlags = HITTEST_FLAG_NONE;
    // are we over the header?
    if ( HitTestHeader( point, this->m_nHotColumn, nHeaderFlags ) ) {
      RECT rcColumn;
      if ( !GetColumnRect( this->m_nHotColumn, rcColumn ) )
        return FALSE;
      this->m_nHotDivider = point.x < ( ( rcColumn.left + rcColumn.right ) / 2 ) ? this->m_nHotColumn : this->m_nHotColumn + 1;
      if ( this->m_nHotDivider == this->m_nDragColumn || this->m_nHotDivider == this->m_nDragColumn + 1 )
        this->m_nHotDivider = NULL_COLUMN;
    }
    if ( this->m_nHotDivider != NULL_COLUMN ) {
      RECT rcHeader;
      GetClientRect( rcHeader );
      rcHeader.bottom = this->m_nHeaderHeight;
      CPoint ptDivider( 0, rcHeader.Height() / 2 );
      RECT rcColumn;
      int nColumnCount = GetColumnCount();
      // set closest divider position
      if ( GetColumnRect( this->m_nHotDivider < nColumnCount ? this->m_nHotDivider : nColumnCount - 1, rcColumn ) )
        ptDivider.x = this->m_nHotDivider < nColumnCount ? rcColumn.left : rcColumn.right;
      ClientToScreen( &ptDivider );
      // track drop window
      this->m_wndDropArrows->Show( ptDivider );
      return TRUE;
    }
    this->m_wndDropArrows->Hide();
    return FALSE;
  }
  BOOL SortColumn( int nColumn ) {
    self * pT = this;
    if ( !m_bShowHeader || !m_bShowSort )
      return FALSE;
    BOOL bReverseSort = FALSE;
    int nSortIndex = GetColumnIndex( nColumn );
    CWaitCursor curWait;
    if ( nSortIndex != this->m_nSortColumn ) {
      // sort by new column
      this->m_bSortAscending = TRUE;
      this->m_nSortColumn = nSortIndex;
      pT->SortItems( this->m_nSortColumn, this->m_bSortAscending );
    } else {
      // toggle sort order if sorting same column
      this->m_bSortAscending = !m_bSortAscending;
      pT->ReverseItems();
    }
    return ResetSelected();
  }
  BOOL GetSortColumn( int& nColumn, BOOL& bAscending ) {
    if ( !m_bShowHeader || !m_bShowSort || this->m_nSortColumn == NULL_COLUMN )
      return FALSE;
    nColumn = this->m_nSortColumn;
    bAscending = this->m_bSortAscending;
    return TRUE;
  }
  BOOL DragItem() {
    ATLASSERT( FALSE ); // must be implemented in a derived class
    return FALSE;
  }
  BOOL GroupSelect( CPoint point ) {
    HideTitleTip();
    int nHorzScroll = GetScrollPos().cx;
    int nVertScroll = GetScrollPos().cy;
    this->m_rcGroupSelect.left = min( this->m_ptSelectPoint.x, point.x + nHorzScroll );
    this->m_rcGroupSelect.right = max( this->m_ptSelectPoint.x, point.x + nHorzScroll );
    this->m_rcGroupSelect.top = min( this->m_ptSelectPoint.y, point.y + nVertScroll );
    this->m_rcGroupSelect.bottom = max( this->m_ptSelectPoint.y, point.y + nVertScroll );
    if ( this->m_rcGroupSelect.IsRectEmpty() )
      return FALSE;
    // select items in group
    AutoSelect( point );
    // start auto scroll timer
    GetManager() ->SetTimer( this, ITEM_AUTOSCROLL_TIMER, ITEM_SCROLL_PERIOD );
    DWORD dwCurrentTick = GetTickCount();
    // timer messages are a low priority, therefore we need to simulate the timer when moving the mouse
    if ( ( dwCurrentTick - this->m_dwScrollTick ) > ITEM_SCROLL_PERIOD - 10 ) {
      if ( AutoScroll( point ) )
        this->m_dwScrollTick = dwCurrentTick;
    }
    // redraw list immediately
    return RedrawWindow();
  }
  BOOL RedrawWindow() {
    return Invalidate();
  }
  void AutoSelect( CPoint point ) {
    this->m_setSelectedItems.clear();
    if ( this->m_rcGroupSelect.left < GetTotalWidth() ) {
      int nHorzScroll = GetScrollPos().cx;
      int nVertScroll = GetScrollPos().cy;
      RECT rcGroupSelect( this->m_rcGroupSelect );
      rcGroupSelect.OffsetRect( -nHorzScroll, -nVertScroll );
      int nTopItem = GetTopItem();
      int nLastItem = nTopItem + ( ( rcGroupSelect.bottom - ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 ) ) / this->m_nItemHeight );
      nTopItem += ( ( rcGroupSelect.top - ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 ) ) / this->m_nItemHeight ) - ( ( rcGroupSelect.top < 0 ) ? 1 : 0 );
      for ( int nItem = nTopItem; nItem <= nLastItem; nItem++ ) {
        if ( this->m_setSelectedItems.empty() )
          this->m_nFirstSelected = nItem;
        this->m_setSelectedItems.insert( nItem );
        this->m_nFocusItem = nItem;
        this->m_nFocusSubItem = NULL_SUBITEM;
      }
    }
    if ( this->m_setSelectedItems.empty() )
      this->m_nFirstSelected = NULL_ITEM;
  }
  BOOL AutoScroll( CPoint point ) {
    RECT rcClient;
    GetClientRect( rcClient );
    rcClient.top = ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 );
    int nHorzScroll = GetScrollPos().cx;
    int nVertScroll = GetScrollPos().cy;
    BOOL bAutoScroll = FALSE;
    if ( point.y < rcClient.top ) {
      //SendMessage( WM_VSCROLL, MAKEWPARAM( SB_LINEUP, 0 ) );
      LineUp();
      int nAutoScroll = GetScrollPos().cy;
      if ( nVertScroll != nAutoScroll ) {
        this->m_rcGroupSelect.top = rcClient.top + nAutoScroll - 1;
        this->m_rcGroupSelect.bottom = max( this->m_ptSelectPoint.y, point.y + nAutoScroll - 1 );
        bAutoScroll = TRUE;
      }
    }
    if ( point.y > rcClient.bottom ) {
      //SendMessage( WM_VSCROLL, MAKEWPARAM( SB_LINEDOWN, 0 ) );
      LineDown();
      int nAutoScroll = GetScrollPos().cy;
      if ( nVertScroll != nAutoScroll ) {
        this->m_rcGroupSelect.top = min( this->m_ptSelectPoint.y, point.y + nAutoScroll + 1 );
        this->m_rcGroupSelect.bottom = rcClient.bottom + nAutoScroll + 1;
        bAutoScroll = TRUE;
      }
    }
    if ( point.x < rcClient.left ) {
      //SendMessage( WM_HSCROLL, MAKEWPARAM( SB_LINELEFT, 0 ) );
      LineLeft();
      int nAutoScroll = GetScrollPos().cx;
      if ( nHorzScroll != nAutoScroll ) {
        this->m_rcGroupSelect.left = rcClient.left + nAutoScroll - 1;
        this->m_rcGroupSelect.right = max( this->m_ptSelectPoint.x, point.x + nAutoScroll - 1 );
        bAutoScroll = TRUE;
      }
    }
    if ( point.x > rcClient.right ) {
      //SendMessage( WM_HSCROLL, MAKEWPARAM( SB_LINERIGHT, 0 ) );
      LineRight();
      int nAutoScroll = GetScrollPos().cx;
      if ( nHorzScroll != nAutoScroll ) {
        this->m_rcGroupSelect.left = min( this->m_ptSelectPoint.x, point.x + nAutoScroll + 1 );
        this->m_rcGroupSelect.right = rcClient.right + nAutoScroll + 1;
        bAutoScroll = TRUE;
      }
    }
    // was scrolling performed?
    return bAutoScroll;
  }
  BOOL BeginScroll( int nBeginItem, int nEndItem ) {
    self * pT = this;
    // any scroll required?
    if ( nBeginItem == nEndItem )
      return FALSE;
    // calculate scroll offset
    this->m_nScrollOffset = abs( nEndItem - nBeginItem ) * this->m_nItemHeight;
    this->m_nScrollUnit = min( max( this->m_nScrollOffset / this->m_nItemHeight, ITEM_SCROLL_UNIT_MIN ), ITEM_SCROLL_UNIT_MAX );
    this->m_nScrollDelta = ( this->m_nScrollOffset - this->m_nScrollUnit ) / this->m_nScrollUnit;
    this->m_bScrollDown = ( nBeginItem < nEndItem );
    CClientDC dcClient( M_HWND );
    CDC dcScrollList;
    dcScrollList.CreateCompatibleDC( dcClient );
    int nContextState = dcScrollList.SaveDC();
    RECT rcScrollList;
    GetClientRect( rcScrollList );
    rcScrollList.bottom = ( GetCountPerPage() + abs( nEndItem - nBeginItem ) ) * this->m_nItemHeight;
    if ( !m_bmpScrollList.IsNull() )
      this->m_bmpScrollList.DeleteObject();
    this->m_bmpScrollList.CreateCompatibleBitmap( dcClient, rcScrollList.Width(), rcScrollList.Height() );
    dcScrollList.SelectBitmap( this->m_bmpScrollList );
    pT->DrawBkgnd( dcScrollList.m_hDC );
    RECT rcItem;
    rcItem.left = -GetScrollPos().cx;
    rcItem.right = GetTotalWidth();
    rcItem.top = 0;
    rcItem.bottom = rcItem.top;
    // draw all visible items into bitmap
    for ( int nItem = min( nBeginItem, nEndItem ); nItem < pT->GetItemCount(); rcItem.top = rcItem.bottom, nItem++ ) {
      rcItem.bottom = rcItem.top + this->m_nItemHeight;
      if ( rcItem.top > rcScrollList.bottom )
        break;
      // may be implemented in a derived class
      pT->DrawItem( dcScrollList.m_hDC, nItem, rcItem );
    }
    dcScrollList.RestoreDC( nContextState );
    ScrollList();
    // start scrolling timer
    GetManager() ->SetTimer( this, ITEM_SCROLL_TIMER, ITEM_SCROLL_PERIOD );
    return TRUE;
  }
  BOOL EndScroll() {
    GetManager() ->KillTimer( this, ITEM_SCROLL_TIMER );
    if ( !m_bmpScrollList.IsNull() )
      this->m_bmpScrollList.DeleteObject();
    return Invalidate();
  }
  BOOL ScrollList() {
    if ( this->m_nScrollOffset <= this->m_nScrollUnit )
      this->m_nScrollOffset--;
    else {
      this->m_nScrollOffset -= this->m_nScrollDelta;
      if ( this->m_nScrollOffset < this->m_nScrollDelta )
        this->m_nScrollOffset = this->m_nScrollUnit;
    }
    if ( this->m_bmpScrollList.IsNull() || this->m_nScrollOffset < 0 )
      return FALSE;
    CClientDC dcClient( M_HWND );
    CDC dcScrollList;
    dcScrollList.CreateCompatibleDC( dcClient );
    RECT rcClient;
    GetClientRect( rcClient );
    rcClient.top = ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 );
    HBITMAP hOldBitmap = dcScrollList.SelectBitmap( this->m_bmpScrollList );
    CSize sizScrollBitmap;
    this->m_bmpScrollList.GetSize( sizScrollBitmap );
    // draw scrolled list
    dcClient.BitBlt( 0, rcClient.top, rcClient.Width(), rcClient.Height(), dcScrollList, 0, this->m_bScrollDown ? ( sizScrollBitmap.cy - ( GetCountPerPage() * this->m_nItemHeight ) - this->m_nScrollOffset ) : this->m_nScrollOffset, SRCCOPY );
    dcScrollList.SelectBitmap( hOldBitmap );
    return TRUE;
  }
  BOOL EditItem( int nItem, int nSubItem = NULL_SUBITEM ) {
#if 0
    self * pT = this;
    if ( !EnsureItemVisible( nItem, nSubItem ) )
      return FALSE;
    if ( GetFocus() != M_HWND )
      return FALSE;
    RECT rcSubItem;
    if ( !GetItemRect( nItem, nSubItem, rcSubItem ) )
      return FALSE;
    int nIndex = GetColumnIndex( nSubItem );
    if ( pT->GetItemFlags( nItem, nIndex ) & ITEM_FLAGS_READ_ONLY )
      return TRUE;
    switch ( pT->GetItemFormat( nItem, nIndex ) ) {
    case ITEM_FORMAT_EDIT:
      this->m_bEditItem = TRUE;
      if ( !RedrawWindow() )
        return FALSE;
      if ( !m_wndItemEdit->Create( M_HWND, nItem, nSubItem, rcSubItem, pT->GetItemFlags( nItem, nIndex ), pT->GetItemText( nItem, nIndex ) ) )
        return FALSE;
      break;
    case ITEM_FORMAT_DATETIME: {
        this->m_bEditItem = TRUE;
        if ( !RedrawWindow() )
          return FALSE;
        SYSTEMTIME stItemDate;
        GetItemDate( nItem, nIndex, stItemDate );
        if ( !m_wndItemDate.Create( M_HWND, nItem, nSubItem, rcSubItem, pT->GetItemFlags( nItem, nIndex ), stItemDate ) )
          return FALSE;
      }
      break;
    case ITEM_FORMAT_COMBO: {
        this->m_bEditItem = TRUE;
        if ( !RedrawWindow() )
          return FALSE;
        CListArray < CStdString > aComboList;
        if ( !pT->GetItemComboList( nItem, nIndex, aComboList ) )
          return FALSE;
        if ( !m_wndItemCombo.Create( M_HWND, nItem, nSubItem, rcSubItem, pT->GetItemFlags( nItem, nIndex ), pT->GetItemText( nItem, nIndex ), this->m_bShowThemed, aComboList ) )
          return FALSE;
      }
      break;
    }
#endif
    return TRUE;
  }
  CStdString FormatDate( SYSTEMTIME& stFormatDate ) {
    if ( stFormatDate.wYear == 0 )
      return _T( "" );
    // format date to local format
    TCHAR szDateFormat[ DATE_STRING ];
    return GetDateFormat( LOCALE_USER_DEFAULT, DATE_SHORTDATE, &stFormatDate, NULL, szDateFormat, DATE_STRING ) == 0 ? _T( "" ) : szDateFormat;
  }
  CStdString FormatTime( SYSTEMTIME& stFormatDate ) {
    SYSTEMTIME stFormatTime = stFormatDate;
    stFormatTime.wYear = 0;
    stFormatTime.wMonth = 0;
    stFormatTime.wDay = 0;
    // format time to local format
    TCHAR szTimeFormat[ DATE_STRING ];
    return GetTimeFormat( LOCALE_USER_DEFAULT, 0, &stFormatTime, NULL, szTimeFormat, DATE_STRING ) == 0 ? _T( "" ) : szTimeFormat;
  }
  void NotifyParent( int nItem, int nSubItem, int nMessage ) {
#if 0
    self * pT = this;
    CListNotify listNotify;
    listNotify.m_hdrNotify.hwndFrom = pT->M_HWND;
    listNotify.m_hdrNotify.idFrom = pT->GetDlgCtrlID();
    listNotify.m_hdrNotify.code = nMessage;
    listNotify.m_nItem = nItem;
    listNotify.m_nSubItem = GetColumnIndex( nSubItem );
    listNotify.m_nExitChar = 0;
    listNotify.m_lpszItemText = NULL;
    listNotify.m_lpItemDate = NULL;
    // forward notification to parent
    FORWARD_WM_NOTIFY( pT->GetParent(), listNotify.m_hdrNotify.idFrom, &listNotify.m_hdrNotify, ::SendMessage );
#endif
  }
  BOOL ShowTitleTip( CPoint point, int nItem, int nSubItem ) {
    self * pT = this;
    // do not show titletip if editing
    if ( this->m_bEditItem )
      return FALSE;
    // is titletip already shown for this item?
    if ( nItem == this->m_nTitleTipItem && nSubItem == this->m_nTitleTipSubItem )
      return FALSE;
    RECT rcSubItem;
    if ( !GetItemRect( nItem, nSubItem, rcSubItem ) ) {
      HideTitleTip();
      return FALSE;
    }
    int nIndex = GetColumnIndex( nSubItem );
    RECT rcItemText( rcSubItem );
    // margin item text
    rcItemText.left += nSubItem == 0 ? 4 : 3;
    rcItemText.DeflateRect( 4, 0 );
    // offset item text (for image)
    if ( !m_ilItemImages.IsNull() && pT->GetItemImage( nItem, nIndex ) != ITEM_IMAGE_NONE ) {
      CSize sizeIcon;
      this->m_ilItemImages.GetIconSize( sizeIcon );
      rcItemText.left += sizeIcon.cx + 4;
    }
    // is current cursor position over item text (not over an icon)?
    if ( !rcItemText.PtInRect( point ) )
      return FALSE;
    CStdString strItemText;
    switch ( pT->GetItemFormat( nItem, nIndex ) ) {
    case ITEM_FORMAT_CHECKBOX:
    case ITEM_FORMAT_CHECKBOX_3STATE:
    case ITEM_FORMAT_PROGRESS:
      break; // no titletip for checkboxes or progress
    case ITEM_FORMAT_DATETIME: {
        SYSTEMTIME stItemDate;
        if ( !GetItemDate( nItem, nIndex, stItemDate ) )
          break;
        UINT nItemFlags = pT->GetItemFlags( nItem, nIndex );
        if ( nItemFlags & ITEM_FLAGS_DATE_ONLY )
          strItemText = FormatDate( stItemDate );
        else if ( nItemFlags & ITEM_FLAGS_TIME_ONLY )
          strItemText = FormatTime( stItemDate );
        else
          strItemText = FormatDate( stItemDate ) + _T( " " ) + FormatTime( stItemDate );
      }
      break;
    default:
      strItemText = pT->GetItemText( nItem, nIndex );
      break;
    }
    if ( strItemText.IsEmpty() ) {
      HideTitleTip();
      return FALSE;
    }
    ClientToScreen( rcItemText );
    if ( this->m_wndTitleTip && !m_wndTitleTip->Show( rcItemText, strItemText, pT->GetItemToolTip( nItem, nIndex ) ) ) {
      HideTitleTip();
      return FALSE;
    }
    this->m_nTitleTipItem = nItem;
    this->m_nTitleTipSubItem = nSubItem;
    return TRUE;
  }
  BOOL ShowTitleTip( RECT& rcRect, LPCTSTR lpszItemText, LPCTSTR lpszToolTip ) {
    return 0;
  }
  BOOL HideTitleTip( BOOL bResetItem = TRUE ) {
    if ( bResetItem ) {
      this->m_nTitleTipItem = NULL_ITEM;
      this->m_nTitleTipSubItem = NULL_SUBITEM;
    }
    return this->m_wndTitleTip ? this->m_wndTitleTip->Hide() : FALSE;
  }
  void OnDestroy() {
    this->m_oleDragDrop.Revoke();
  }
  void OnSetFocus() {
    Invalidate();
  }
  void OnKillFocus() {
    Invalidate();
  }
  UINT OnGetDlgCode( LPMSG lpMessage ) {
    return DLGC_WANTARROWS | DLGC_WANTTAB | DLGC_WANTCHARS;
  }
  void OnSize( UINT nType, CSize size ) {
    // stop any pending scroll
    EndScroll();
    // end any pending edit
    if ( this->m_bEditItem )
      SetFocus();
    ResetScrollBars( SB_BOTH, -1, FALSE );
    Invalidate();
  }
  void OnHScroll( int nSBCode, short nPos, HWND hScrollBar ) {
    // stop any pending scroll
    EndScroll();
    // end any pending edit
    if ( this->m_bEditItem )
      SetFocus();
    HideTitleTip();
    RECT rcClient;
    GetClientRect( rcClient );
    int nScrollPos = GetScrollPos().cx;
    switch ( nSBCode ) {
    case SB_LEFT:
      nScrollPos = 0;
      break;
    case SB_LINELEFT:
      nScrollPos = max( nScrollPos - ITEM_SCROLL_OFFSET, 0 );
      break;
    case SB_PAGELEFT:
      nScrollPos = max( nScrollPos - rcClient.Width(), 0 );
      break;
    case SB_RIGHT:
      nScrollPos = rcClient.Width();
      break;
    case SB_LINERIGHT:
      nScrollPos = min( nScrollPos + ITEM_SCROLL_OFFSET, GetTotalWidth() );
      break;
    case SB_PAGERIGHT:
      nScrollPos = min( nScrollPos + rcClient.Width(), GetTotalWidth() );
      break;
    case SB_THUMBPOSITION:
    case SB_THUMBTRACK: {
        SCROLLINFO infoScroll;
        ZeroMemory( &infoScroll, sizeof( SCROLLINFO ) );
        infoScroll.cbSize = sizeof( SCROLLINFO );
        infoScroll.fMask = SIF_TRACKPOS;
        // get 32-bit scroll position
        //if ( !GetScrollInfo( SB_HORZ, &infoScroll ) )
        //  return ;
        // has scroll position changed?
        nScrollPos = infoScroll.nTrackPos;
      }
      break;
    default:
      return ;
    }
    ResetScrollBars( SB_HORZ, nScrollPos, FALSE );
    Invalidate();
  }
  void OnVScroll( int nSBCode, short nPos, HWND hScrollBar ) {
    self * pT = this;
    // end any pending edit
    if ( this->m_bEditItem )
      SetFocus();
    HideTitleTip();
    RECT rcClient;
    GetClientRect( rcClient );
    rcClient.top = ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 );
    int nScrollPos = GetScrollPos().cy;
    BOOL bScrollList = this->m_bSmoothScroll;
    switch ( nSBCode ) {
    case SB_TOP:
      nScrollPos = 0;
      bScrollList = FALSE;
      break;
    case SB_LINEUP:
      nScrollPos = max( nScrollPos - this->m_nItemHeight, 0 );
      break;
    case SB_PAGEUP:
      nScrollPos = max( nScrollPos - rcClient.Height(), 0 );
      break;
    case SB_BOTTOM:
      nScrollPos = pT->GetItemCount() * this->m_nItemHeight;
      bScrollList = FALSE;
      break;
    case SB_LINEDOWN:
      nScrollPos += this->m_nItemHeight;
      break;
    case SB_PAGEDOWN:
      nScrollPos += rcClient.Height();
      break;
    case SB_THUMBTRACK:
    case SB_THUMBPOSITION: {
        SCROLLINFO infoScroll;
        ZeroMemory( &infoScroll, sizeof( SCROLLINFO ) );
        infoScroll.cbSize = sizeof( SCROLLINFO );
        infoScroll.fMask = SIF_TRACKPOS;
        // get 32-bit scroll position
        //if ( !GetScrollInfo( SB_VERT, &infoScroll ) )
        //  return ;
        // has scroll position changed?
        //if ( infoScroll.nTrackPos == nScrollPos )
        //  return ;
        nScrollPos = infoScroll.nTrackPos;
        bScrollList = FALSE;
      }
      break;
    case SB_ENDSCROLL:
      this->m_bScrolling = FALSE;
    default:
      return ;
    }
    // store original top item before scrolling
    int nTopItem = GetTopItem();
    ResetScrollBars( SB_VERT, nScrollPos, FALSE );
    if ( bScrollList && !m_bScrolling )
      this->m_bScrolling = BeginScroll( nTopItem, GetTopItem() );
    else
      EndScroll();
  }
  void OnCancelMode() {
    if ( this->m_bButtonDown )
      ReleaseCapture();
    HideTitleTip();
    this->m_wndDropArrows->Hide();
    this->m_nDragColumn = NULL_COLUMN;
    this->m_nHighlightColumn = NULL_COLUMN;
  }
  void OnLButtonDown( UINT nFlags, CPoint point ) {
    self * pT = this;
    HideTitleTip( FALSE );
    this->m_bButtonDown = TRUE;
    this->m_ptDownPoint = point;
    SIZE sz = GetScrollPos();
    this->m_ptSelectPoint = CPoint( point.x + sz.cx, point.y + sz.cy );
    // stop any pending scroll
    EndScroll();
    SetFocus();
    // capture all mouse input
    //SetCapture();
    int nColumn = NULL_COLUMN;
    UINT nHeaderFlags = HITTEST_FLAG_NONE;
    // are we over the header?
    if ( HitTestHeader( point, nColumn, nHeaderFlags ) ) {
      CListColumn listColumn;
      if ( !GetColumn( nColumn, listColumn ) )
        return ;
      if ( !listColumn.m_bFixed && ( nHeaderFlags & HITTEST_FLAG_HEADER_DIVIDER ) ) {
        SetCursor( this->m_curDivider );
        // begin column resizing
        this->m_bColumnSizing = TRUE;
        this->m_nColumnSizing = nColumn;
        this->m_nStartSize = listColumn.m_nWidth;
        this->m_nStartPos = GET_X_LPARAM( GetMessagePos() );
      } else {
        this->m_nHighlightColumn = nColumn;
        InvalidateHeader();
      }
      return ;
    }
    int nItem = NULL_ITEM;
    int nSubItem = NULL_SUBITEM;
    if ( !HitTest( point, nItem, nSubItem ) ) {
      this->m_nFirstSelected = NULL_ITEM;
      this->m_bBeginSelect = TRUE;
    } else {
      // do not begin group select from first columns
      if ( !( nFlags & MK_SHIFT ) && !( nFlags & MK_CONTROL ) && nSubItem != 0 ) {
        this->m_bBeginSelect = TRUE;
        this->m_nFirstSelected = nItem;
      }
      // only select item if not already selected
      if ( ( nFlags & MK_SHIFT ) || ( nFlags & MK_CONTROL ) || !IsSelected( nItem ) || this->m_setSelectedItems.size() <= 1 )
        SelectItem( nItem, nSubItem, nFlags );
      int nIndex = GetColumnIndex( nSubItem );
      if ( !( pT->GetItemFlags( nItem, nIndex ) & ITEM_FLAGS_READ_ONLY ) ) {
        switch ( pT->GetItemFormat( nItem, nIndex ) ) {
        case ITEM_FORMAT_CHECKBOX:
          this->m_bBeginSelect = FALSE;
          pT->SetItemText( nItem, nIndex, _ttoi( pT->GetItemText( nItem, nIndex ) ) > 0 ? _T( "0" ) : _T( "1" ) );
          NotifyParent( nItem, nSubItem, LCN_MODIFIED );
          InvalidateItem( nItem );
          break;
        case ITEM_FORMAT_CHECKBOX_3STATE: {
            this->m_bBeginSelect = FALSE;
            int nCheckImage = _ttoi( pT->GetItemText( nItem, nIndex ) );
            if ( nCheckImage < 0 )
              pT->SetItemText( nItem, nIndex, _T( "0" ) );
            else if ( nCheckImage > 0 )
              pT->SetItemText( nItem, nIndex, _T( "-1" ) );
            else
              pT->SetItemText( nItem, nIndex, _T( "1" ) );
            NotifyParent( nItem, nSubItem, LCN_MODIFIED );
            InvalidateItem( nItem );
          }
          break;
        case ITEM_FORMAT_HYPERLINK:
          this->m_bBeginSelect = FALSE;
          SetCursor( this->m_curHyperLink );
          NotifyParent( nItem, nSubItem, LCN_HYPERLINK );
          break;
        }
      }
    }
  }
  void OnLButtonUp( UINT nFlags, CPoint point ) {
    if ( this->m_bButtonDown )
      ReleaseCapture();
    // finish resizing or selecting a column
    if ( this->m_bColumnSizing || this->m_nHighlightColumn != NULL_COLUMN ) {
      // are we changing the sort order?
      if ( !m_bColumnSizing && this->m_nHighlightColumn != NULL_COLUMN )
        SortColumn( this->m_nHighlightColumn );
      this->m_bColumnSizing = FALSE;
      this->m_nColumnSizing = NULL_COLUMN;
      this->m_nHighlightColumn = NULL_COLUMN;
      this->m_nStartSize = 0;
      this->m_nStartPos = 0;
      InvalidateHeader();
    }
    this->m_bBeginSelect = FALSE;
    this->m_bButtonDown = FALSE;
    this->m_ptDownPoint = 0;
    this->m_ptSelectPoint = 0;
    // have we finished a group select?
    if ( this->m_bGroupSelect ) {
      this->m_bGroupSelect = FALSE;
      Invalidate();
    } else {
      int nItem = NULL_ITEM;
      int nSubItem = NULL_SUBITEM;
      // de-select item if current item is selected
      if ( HitTest( point, nItem, nSubItem ) && IsSelected( nItem ) && this->m_setSelectedItems.size() > 1 && !( nFlags & MK_SHIFT ) && !( nFlags & MK_CONTROL ) )
        SelectItem( nItem, nSubItem, nFlags );
      // notify parent of left-click item
      NotifyParent( nItem, nSubItem, LCN_LEFTCLICK );
    }
  }
  void OnLButtonDblClk( UINT nFlags, CPoint point ) {
    HideTitleTip( FALSE );
    // handle double-clicks (for drawing)
    //SendMessage( WM_LBUTTONDOWN, 0, MAKELPARAM( point.x, point.y ) );
    int nColumn = NULL_COLUMN;
    UINT nHeaderFlags = HITTEST_FLAG_NONE;
    // resize column if double-click on a divider
    if ( HitTestHeader( point, nColumn, nHeaderFlags ) && ( nHeaderFlags & HITTEST_FLAG_HEADER_DIVIDER ) )
      AutoSizeColumn( nColumn );
    int nItem = NULL_ITEM;
    int nSubItem = NULL_SUBITEM;
    HitTest( point, nItem, nSubItem );
    // notify parent of double-clicked item
    NotifyParent( nItem, nSubItem, LCN_DBLCLICK );
  }
  void OnRButtonDown( UINT nFlags, CPoint point ) {
    // stop any pending scroll
    EndScroll();
    SetFocus();
    HideTitleTip( FALSE );
    int nItem = NULL_ITEM;
    int nSubItem = NULL_SUBITEM;
    // only select item if not already selected (de-select in OnLButtonUp)
    if ( HitTest( point, nItem, nSubItem ) && !IsSelected( nItem ) )
      SelectItem( nItem, nSubItem, nFlags );
  }
  void OnRButtonUp( UINT nFlags, CPoint point ) {
    int nItem = NULL_ITEM;
    int nSubItem = NULL_SUBITEM;
    if ( !HitTest( point, nItem, nSubItem ) )
      ResetSelected();
    // notify parent of right-click item
    NotifyParent( nItem, nSubItem, LCN_RIGHTCLICK );
  }
  void OnMouseMove( UINT nFlags, CPoint point ) {
    self * pT = this;
    if ( !( nFlags & MK_LBUTTON ) ) {
      if ( this->m_bButtonDown )
        ReleaseCapture();
      //m_bButtonDown = FALSE;
    }
    if ( !m_bMouseOver ) {
      this->m_bMouseOver = TRUE;
      TRACKMOUSEEVENT trkMouse;
      trkMouse.cbSize = sizeof( TRACKMOUSEEVENT );
      trkMouse.dwFlags = TME_LEAVE;
      trkMouse.hwndTrack = M_HWND;
      // notify when the mouse leaves button
      _TrackMouseEvent( &trkMouse );
    }
    if ( this->m_bButtonDown ) {
      // are we resizing a column?
      if ( this->m_bColumnSizing ) {
        ResizeColumn();
        return ;
      }
      // are we beginning to drag a column?
      if ( this->m_nHighlightColumn != NULL_COLUMN && ( point.x < this->m_ptDownPoint.x - DRAG_HEADER_OFFSET || point.x > this->m_ptDownPoint.x + DRAG_HEADER_OFFSET || point.y < this->m_ptDownPoint.y - DRAG_HEADER_OFFSET || point.y > this->m_ptDownPoint.y + DRAG_HEADER_OFFSET ) ) {
        DragColumn();
        return ;
      }
      // are we beginning a group select or dragging an item?
      if ( point.x < this->m_ptDownPoint.x - DRAG_ITEM_OFFSET || point.x > this->m_ptDownPoint.x + DRAG_ITEM_OFFSET || point.y < this->m_ptDownPoint.y - DRAG_ITEM_OFFSET || point.y > this->m_ptDownPoint.y + DRAG_ITEM_OFFSET ) {
        if ( this->m_bBeginSelect || !m_bDragDrop )
          this->m_bGroupSelect = ( !m_bSingleSelect && !m_bEditItem );
        else {
          int nItem = NULL_ITEM;
          int nSubItem = NULL_SUBITEM;
          if ( HitTest( point, nItem, nSubItem ) ) {
            // select the drag item (if not already selected)
            if ( !IsSelected( nItem ) )
              SelectItem( nItem, nSubItem, nFlags );
            // begin drag item operation
            pT->DragItem();
          }
        }
      }
      if ( this->m_bGroupSelect ) {
        GroupSelect( point );
        return ;
      }
    } else {
      int nColumn = NULL_COLUMN;
      UINT nHeaderFlags = HITTEST_FLAG_NONE;
      // are we over the header?
      BOOL bHitTestHeader = HitTestHeader( point, nColumn, nHeaderFlags );
      if ( bHitTestHeader ) {
        HideTitleTip();
        CListColumn listColumn;
        if ( GetColumn( nColumn, listColumn ) && !listColumn.m_bFixed && ( nHeaderFlags & HITTEST_FLAG_HEADER_DIVIDER ) )
          SetCursor( this->m_curDivider );
        return ;
      }
      int nItem = NULL_ITEM;
      int nSubItem = NULL_SUBITEM;
      if ( !HitTest( point, nItem, nSubItem ) ) {
        if ( this->m_nHotItem != NULL_ITEM && this->m_nHotSubItem != NULL_SUBITEM ) {
          // redraw old hot item
          int nIndex = GetColumnIndex( this->m_nHotSubItem );
          if ( pT->GetItemFormat( this->m_nHotItem, nIndex ) == ITEM_FORMAT_HYPERLINK && !( pT->GetItemFlags( this->m_nHotItem, nIndex ) & ITEM_FLAGS_READ_ONLY ) )
            InvalidateItem( this->m_nHotItem, this->m_nHotSubItem );
        }
        ActivateToolTip( FALSE );
        DelToolTip( M_HWND, TOOLTIP_TOOL_ID );
        this->m_nHotItem = NULL_ITEM;
        this->m_nHotSubItem = NULL_SUBITEM;
        HideTitleTip();
      } else {
        // has the hot item changed?
        if ( nItem != this->m_nHotItem || nSubItem != this->m_nHotSubItem ) {
          // redraw old hot item
          int nIndex = GetColumnIndex( this->m_nHotSubItem );
          if ( pT->GetItemFormat( this->m_nHotItem, nIndex ) == ITEM_FORMAT_HYPERLINK && !( pT->GetItemFlags( this->m_nHotItem, nIndex ) & ITEM_FLAGS_READ_ONLY ) )
            InvalidateItem( this->m_nHotItem, this->m_nHotSubItem );
          this->m_nHotItem = nItem;
          this->m_nHotSubItem = nSubItem;
        }
        int nIndex = GetColumnIndex( this->m_nHotSubItem );
        UINT nItemFormat = pT->GetItemFormat( this->m_nHotItem, nIndex );
        UINT nItemFlags = pT->GetItemFlags( this->m_nHotItem, nIndex );
        // draw new hot hyperlink item
        if ( nItemFormat == ITEM_FORMAT_HYPERLINK && !( nItemFlags & ITEM_FLAGS_READ_ONLY ) ) {
          InvalidateItem( this->m_nHotItem, this->m_nHotSubItem );
          SetCursor( this->m_curHyperLink );
        }
        // get tooltip for this item
        CStdString strToolTip = pT->GetItemToolTip( this->m_nHotItem, nIndex );
        RECT rcSubItem;
        if ( !strToolTip.IsEmpty() && GetItemRect( this->m_nHotItem, rcSubItem ) ) {
          ActivateToolTip( TRUE );
          AddToolTip( M_HWND, ( LPCTSTR ) strToolTip.Left( SHRT_MAX ), rcSubItem, TOOLTIP_TOOL_ID );
        } else {
          ActivateToolTip( FALSE );
          DelToolTip( M_HWND, TOOLTIP_TOOL_ID );
        }
        // show titletips for this item
        ShowTitleTip( point, this->m_nHotItem, this->m_nHotSubItem );
      }
    }
  }
  BOOL AddToolTip( HWND hWnd, _U_STRINGorID text = LPSTR_TEXTCALLBACK, LPCRECT lpRectTool = NULL, UINT nIDTool = 0 ) {
    return 0;
  }
  void DelToolTip( HWND hWnd, UINT nIDTool = 0 ) {}
  void ActivateToolTip( BOOL bActivate ) {
    //m_ttToolTip->Activate( TRUE );
  }
  void OnMouseLeave() {
    this->m_bMouseOver = FALSE;
    if ( this->m_nHotColumn != NULL_COLUMN ) {
      this->m_nHotColumn = NULL_COLUMN;
      InvalidateHeader();
    }
    if ( this->m_nHotItem != NULL_ITEM || this->m_nHotSubItem != NULL_SUBITEM ) {
      this->m_nHotItem = NULL_ITEM;
      this->m_nHotSubItem = NULL_SUBITEM;
      Invalidate();
    }
  }
  BOOL OnMouseWheel( UINT nFlags, short nDelta, CPoint point ) {
    HideTitleTip();
    // end any pending edit
    if ( this->m_bEditItem )
      SetFocus();
    int nRowsScrolled = this->m_nMouseWheelScroll * nDelta / 120;
    int nScrollPos = GetScrollPos().cy;
    if ( nRowsScrolled > 0 )
      nScrollPos = max( nScrollPos - ( nRowsScrolled * this->m_nItemHeight ), 0 );
    else
      nScrollPos += ( -nRowsScrolled * this->m_nItemHeight );
    ResetScrollBars( SB_VERT, nScrollPos, FALSE );
    Invalidate();
    return TRUE;
  }
  void OnTimer( UINT nIDEvent, TIMERPROC timer ) {
    switch ( nIDEvent ) {
    case RESIZE_COLUMN_TIMER:
      ResizeColumn( TRUE );
      break;
    case ITEM_VISIBLE_TIMER: {
        GetManager() ->KillTimer( this, ITEM_VISIBLE_TIMER );
        int nFocusItem = NULL_ITEM;
        int nFocusSubItem = NULL_SUBITEM;
        // get current focus item
        if ( !GetFocusItem( nFocusItem, nFocusSubItem ) )
          break;
        // make sure current focus item is visible before editing
        if ( !EditItem( nFocusItem, nFocusSubItem ) )
          break;
      }
      break;
    case ITEM_AUTOSCROLL_TIMER:
      if ( !m_bGroupSelect )
        GetManager() ->KillTimer( this, ITEM_AUTOSCROLL_TIMER );
      else {
        DWORD dwPoint = GetMessagePos();
        CPoint ptMouse( GET_X_LPARAM( dwPoint ), GET_Y_LPARAM( dwPoint ) );
        ScreenToClient( &ptMouse );
        // automatically scroll when group selecting
        AutoScroll( ptMouse );
        AutoSelect( ptMouse );
      }
      break;
    case ITEM_SCROLL_TIMER:
      if ( !ScrollList() )
        EndScroll();
      break;
    }
  }
  void OnKeyDown( TCHAR nChar, UINT nRepCnt, UINT nFlags ) {
    self * pT = this;
    // stop any pending scroll
    EndScroll();
    BOOL bCtrlKey = ( ( GetKeyState( VK_CONTROL ) & 0x8000 ) != 0 );
    BOOL bShiftKey = ( ( GetKeyState( VK_SHIFT ) & 0x8000 ) != 0 );
    RECT rcClient;
    GetClientRect( rcClient );
    int nFocusItem = NULL_ITEM;
    int nFocusSubItem = NULL_SUBITEM;
    GetFocusItem( nFocusItem, nFocusSubItem );
    switch ( nChar ) {
    case VK_DOWN:
      SetFocusItem( min( nFocusItem + 1, pT->GetItemCount() - 1 ), nFocusSubItem );
      break;
    case VK_UP:
      SetFocusItem( max( nFocusItem - 1, 0 ), nFocusSubItem );
      break;
    case VK_NEXT:
      SetFocusItem( min( nFocusItem + GetCountPerPage( FALSE ) - 1, pT->GetItemCount() - 1 ), nFocusSubItem );
      break;
    case VK_PRIOR:
      SetFocusItem( max( nFocusItem - GetCountPerPage( FALSE ) + 1, 0 ), nFocusSubItem );
      break;
    case VK_HOME:
      SetFocusItem( 0, nFocusSubItem );
      break;
    case VK_END:
      SetFocusItem( pT->GetItemCount() - 1, nFocusSubItem );
      break;
    case VK_LEFT:
      if ( this->m_bFocusSubItem )
        SetFocusItem( nFocusItem, max( nFocusSubItem - 1, 0 ) );
      else
        LineDown();
      //SetScrollPos( SB_HORZ, max( GetScrollPos().cx - ( bCtrlKey ? ITEM_SCROLL_OFFSET * 10 : ITEM_SCROLL_OFFSET ), 0 ) );
      break;
    case VK_RIGHT:
      if ( this->m_bFocusSubItem )
        SetFocusItem( nFocusItem, min( nFocusSubItem + 1, GetColumnCount() - 1 ) );
      else
        LineRight();
      //SetScrollPos( SB_HORZ, min( GetScrollPos().cx + ( bCtrlKey ? ITEM_SCROLL_OFFSET * 10 : ITEM_SCROLL_OFFSET ), rcClient.Width() ) );
      break;
    case VK_TAB:
      if ( !bCtrlKey && this->m_bFocusSubItem )
        SetFocusItem( nFocusItem, bShiftKey ? max( nFocusSubItem - 1, 0 ) : min( nFocusSubItem + 1, GetColumnCount() - 1 ) );
      break;
    default:
      if ( nChar == VK_SPACE ) {
        int nIndex = GetColumnIndex( nFocusSubItem );
        if ( !( pT->GetItemFlags( nFocusItem, nIndex ) & ITEM_FLAGS_READ_ONLY ) ) {
          switch ( pT->GetItemFormat( nFocusItem, nIndex ) ) {
          case ITEM_FORMAT_CHECKBOX:
            pT->SetItemText( nFocusItem, nIndex, _ttoi( pT->GetItemText( nFocusItem, nIndex ) ) > 0 ? _T( "0" ) : _T( "1" ) );
            NotifyParent( nFocusItem, nFocusSubItem, LCN_MODIFIED );
            InvalidateItem( nFocusItem );
            return ;
          case ITEM_FORMAT_CHECKBOX_3STATE: {
              int nCheckImage = _ttoi( pT->GetItemText( nFocusItem, nIndex ) );
              if ( nCheckImage < 0 )
                pT->SetItemText( nFocusItem, nIndex, _T( "0" ) );
              else if ( nCheckImage > 0 )
                pT->SetItemText( nFocusItem, nIndex, _T( "-1" ) );
              else
                pT->SetItemText( nFocusItem, nIndex, _T( "1" ) );
              NotifyParent( nFocusItem, nFocusSubItem, LCN_MODIFIED );
              InvalidateItem( nFocusItem );
            }
            return ;
          }
        }
      }
      if ( bCtrlKey && nChar == _T( 'A' ) && !m_bSingleSelect ) {
        this->m_setSelectedItems.clear();
        for ( int nItem = 0; nItem < pT->GetItemCount(); nItem++ )
          this->m_setSelectedItems.insert( nItem );
        Invalidate();
        return ;
      }
      if ( !bCtrlKey && iswprint( nChar ) && iswupper( nChar ) ) {
        int nSortIndex = GetColumnIndex( this->m_nSortColumn );
        int nStartItem = nFocusItem + 1;
        DWORD dwCurrentTick = GetTickCount();
        CStdString strStart;
        strStart += nChar;
        // has there been another keypress since last search period?
        if ( ( dwCurrentTick - this->m_dwSearchTick ) < SEARCH_PERIOD ) {
          if ( this->m_strSearchString.Left( 1 ) != strStart )
            this->m_strSearchString += nChar;
          CStdString strFocusText = pT->GetItemText( nFocusItem, nSortIndex );
          // are we continuing to type characters under current focus item?
          if ( this->m_strSearchString.GetLength() > 1 && this->m_strSearchString.CompareNoCase( strFocusText.Left( this->m_strSearchString.GetLength() ) ) == 0 ) {
            this->m_dwSearchTick = GetTickCount();
            return ;
          }
        } else {
          if ( this->m_strSearchString.Left( 1 ) != strStart )
            nStartItem = 0;
          this->m_strSearchString = strStart;
        }
        this->m_dwSearchTick = GetTickCount();
        // scan for next search string
        for ( int nFirst = nStartItem; nFirst < pT->GetItemCount(); nFirst++ ) {
          CStdString strItemText = pT->GetItemText( nFirst, nSortIndex );
          if ( this->m_strSearchString.CompareNoCase( strItemText.Left( this->m_strSearchString.GetLength() ) ) == 0 ) {
            SelectItem( nFirst, nFocusSubItem, TRUE );
            EnsureItemVisible( nFirst, nFocusSubItem );
            return ;
          }
        }
        // re-scan from top if not found search string
        for ( int nSecond = 0; nSecond < pT->GetItemCount(); nSecond++ ) {
          CStdString strItemText = pT->GetItemText( nSecond, nSortIndex );
          if ( this->m_strSearchString.CompareNoCase( strItemText.Left( this->m_strSearchString.GetLength() ) ) == 0 ) {
            SelectItem( nSecond, nFocusSubItem, TRUE );
            EnsureItemVisible( nSecond, nFocusSubItem );
            return ;
          }
        }
      }
      return ;
    }
    if ( !bCtrlKey )
      SelectItem( this->m_nFocusItem, this->m_nFocusSubItem, bShiftKey ? MK_SHIFT : 0 );
  }
  void OnSysKeyDown( TCHAR nChar, UINT nRepCnt, UINT nFlags ) {
    HideTitleTip( FALSE );
  }
  void OnSettingsChange( UINT nFlags, LPCTSTR lpszSection ) {
    OnSettingsChange();
  }
  void OnSettingsChange() {
    LoadSettings();
    ResetScrollBars();
    Invalidate();
  }
  LRESULT OnEndEdit( LPNMHDR lpNMHDR ) {
    self * pT = this;
    CListNotify *pListNotify = reinterpret_cast<CListNotify *>( lpNMHDR );
    this->m_bEditItem = FALSE;
    int nIndex = GetColumnIndex( pListNotify->m_nSubItem );
    switch ( pListNotify->m_nExitChar ) {
    case VK_ESCAPE:
      break; // do nothing
    case VK_DELETE:
      pT->SetItemText( pListNotify->m_nItem, nIndex, _T( "" ) );
      NotifyParent( pListNotify->m_nItem, pListNotify->m_nSubItem, LCN_MODIFIED );
      break;
    default:
      if ( pListNotify->m_lpItemDate == NULL )
        pT->SetItemText( pListNotify->m_nItem, nIndex, pListNotify->m_lpszItemText );
      else {
        if ( _ttoi( pListNotify->m_lpszItemText ) == 0 )
          pT->SetItemText( pListNotify->m_nItem, nIndex, _T( "" ) );
        else
          pT->SetItemDate( pListNotify->m_nItem, nIndex, *pListNotify->m_lpItemDate );
      }
      //if ( pListNotify->m_nExitChar == VK_TAB )
      //  PostMessage( WM_KEYDOWN, ( WPARAM ) VK_TAB );
      NotifyParent( pListNotify->m_nItem, pListNotify->m_nSubItem, LCN_MODIFIED );
      break;
    }
    InvalidateItem( pListNotify->m_nItem );
    return 0;
  }
  DWORD OnDragEnter( FORMATETC& FormatEtc, STGMEDIUM& StgMedium, DWORD dwKeyState, CPoint point ) {
    DWORD dwEffect = DROPEFFECT_NONE;
    if ( FormatEtc.cfFormat == this->m_nHeaderClipboardFormat ) {
      LPBYTE lpDragHeader = ( LPBYTE ) GlobalLock( StgMedium.hGlobal );
      if ( lpDragHeader == NULL )
        return DROPEFFECT_NONE;
      // dragged column must originate from this control
      if ( *( ( HWND* ) lpDragHeader ) == M_HWND )
        dwEffect = DropColumn( point ) ? DROPEFFECT_MOVE : DROPEFFECT_NONE;
      GlobalUnlock( StgMedium.hGlobal );
    }
    return dwEffect;
  }
  DWORD OnDragOver( FORMATETC& FormatEtc, STGMEDIUM& StgMedium, DWORD dwKeyState, CPoint point ) {
    DWORD dwEffect = DROPEFFECT_NONE;
    if ( FormatEtc.cfFormat == this->m_nHeaderClipboardFormat ) {
      LPBYTE lpDragHeader = ( LPBYTE ) GlobalLock( StgMedium.hGlobal );
      if ( lpDragHeader == NULL )
        return DROPEFFECT_NONE;
      // dragged column must originate from this control
      if ( *( ( HWND* ) lpDragHeader ) == M_HWND )
        dwEffect = DropColumn( point ) ? DROPEFFECT_MOVE : DROPEFFECT_NONE;
      GlobalUnlock( StgMedium.hGlobal );
    }
    return dwEffect;
  }
  BOOL OnDrop( FORMATETC& FormatEtc, STGMEDIUM& StgMedium, DWORD dwEffect, CPoint point ) {
    if ( FormatEtc.cfFormat == this->m_nHeaderClipboardFormat ) {
      if ( this->m_nDragColumn != NULL_COLUMN && this->m_nHotDivider != NULL_COLUMN ) {
        CListColumn listColumn;
        if ( !GetColumn( this->m_nDragColumn, listColumn ) )
          return FALSE;
        // move column to new position
        this->m_aColumns.RemoveAt( this->m_nDragColumn );
        this->m_aColumns.InsertAt( ( this->m_nDragColumn < this->m_nHotColumn ? ( this->m_nHotDivider == 0 ? 0 : this->m_nHotDivider - 1 ) : this->m_nHotDivider ), listColumn );
        Invalidate();
      }
      return TRUE;
    }
    // not supported
    return FALSE;
  }
  BOOL OnRenderData( FORMATETC& FormatEtc, STGMEDIUM *pStgMedium, BOOL bDropComplete ) {
    if ( FormatEtc.cfFormat == this->m_nHeaderClipboardFormat ) {
      pStgMedium->tymed = TYMED_HGLOBAL;
      pStgMedium->hGlobal = GlobalAlloc( GMEM_MOVEABLE, sizeof( HWND ) );
      if ( pStgMedium->hGlobal == NULL )
        return FALSE;
      LPBYTE lpDragHeader = ( LPBYTE ) GlobalLock( pStgMedium->hGlobal );
      if ( lpDragHeader == NULL )
        return FALSE;
      // store this window handle
      *( ( HWND* ) lpDragHeader ) = M_HWND;
      GlobalUnlock( pStgMedium->hGlobal );
      return TRUE;
    }
    return FALSE;
  }
};
struct CSubItem {
  CStdString this->m_strText;
  int this->m_nImage;
  UINT this->m_nFormat;
  UINT this->m_nFlags;
  CListArray < CStdString > this->m_aComboList;
  HFONT this->m_hFont;
  COLORREF this->m_rgbBackground;
  COLORREF this->m_rgbText;
};
template < class TData = DWORD >
struct CListItem {
  typedef CListItem<TData> self;
  CListArray < CSubItem > this->m_aSubItems;
  CStdString this->m_strToolTip;
  TData this->m_tData;
};
template < class TData >
class CListCtrlData : public CListImpl {
public:
  typedef CListImpl base;
  DECLARE_WND_CLASS( _T( "ListCtrlData" ) )
protected:
  CListArray < CListItem< TData > > this->m_aItems;
public:
  int AddItem( CListItem< TData >& listItem ) {
    CListItem< TData > listItem1 = listItem;
    if ( !m_aItems.Add( listItem1 ) )
      return -1;
    return base::AddItem() ? GetItemCount() - 1 : -1;
  }
  int AddItem( LPCTSTR lpszText, int nImage = ITEM_IMAGE_NONE, UINT nFormat = ITEM_FORMAT_NONE, UINT nFlags = ITEM_FLAGS_NONE ) {
    CSubItem listSubItem;
    listSubItem.m_nImage = ITEM_IMAGE_NONE;
    listSubItem.m_nFormat = nFormat;
    listSubItem.m_nFlags = ValidateFlags( nFlags );
    listSubItem.m_hFont = NULL;
    listSubItem.m_rgbBackground = this->m_rgbBackground;
    listSubItem.m_rgbText = this->m_rgbItemText;
    CListItem< TData > listItem;
    for ( int nSubItem = 0; nSubItem < GetColumnCount(); nSubItem++ )
      listItem.m_aSubItems.Add( listSubItem );
    // set item details for first subitem
    listItem.m_aSubItems[ 0 ].m_strText = lpszText;
    listItem.m_aSubItems[ 0 ].m_nImage = nImage;
    return AddItem( listItem );
  }
  BOOL DeleteItem( int nItem ) {
    if ( nItem < 0 || nItem >= GetItemCount() )
      return FALSE;
    return this->m_aItems.RemoveAt( nItem ) ? base::DeleteItem( nItem ) : FALSE;
  }
  BOOL DeleteAllItems() {
    this->m_aItems.RemoveAll();
    return base::DeleteAllItems();
  }
  virtual int GetItemCount() const {
    return this->m_aItems.GetSize();
  }
  BOOL GetItem( int nItem, CListItem< TData >& listItem ) {
    if ( nItem < 0 || nItem >= GetItemCount() )
      return FALSE;
    listItem = this->m_aItems[ nItem ];
    return TRUE;
  }
  virtual BOOL GetSubItem( int nItem, int nSubItem, CSubItem& listSubItem ) {
    CListItem< TData > listItem;
    if ( !GetItem( nItem, listItem ) )
      return FALSE;
    if ( nSubItem < 0 || nSubItem >= ( int ) listItem.m_aSubItems.GetSize() )
      return FALSE;
    listSubItem = listItem.m_aSubItems[ nSubItem ];
    return TRUE;
  }
  virtual CStdString GetItemText( int nItem, int nSubItem ) {
    CSubItem listSubItem;
    return GetSubItem( nItem, nSubItem, listSubItem ) ? listSubItem.m_strText : _T( "" );
  }
  int GetItemImage( int nItem, int nSubItem ) {
    CSubItem listSubItem;
    return GetSubItem( nItem, nSubItem, listSubItem ) ? listSubItem.m_nImage : ITEM_IMAGE_NONE;
  }
  virtual UINT GetItemFormat( int nItem, int nSubItem ) {
    CSubItem listSubItem;
    if ( !GetSubItem( nItem, nSubItem, listSubItem ) )
      return FALSE;
    return listSubItem.m_nFormat == ITEM_FORMAT_NONE ? GetColumnFormat( IndexToOrder( nSubItem ) ) : listSubItem.m_nFormat;
  }
  UINT GetItemFlags( int nItem, int nSubItem ) {
    CSubItem listSubItem;
    if ( !GetSubItem( nItem, nSubItem, listSubItem ) )
      return FALSE;
    return listSubItem.m_nFlags == ITEM_FLAGS_NONE ? GetColumnFlags( IndexToOrder( nSubItem ) ) : listSubItem.m_nFlags;
  }
  BOOL GetItemComboList( int nItem, int nSubItem, CListArray < CStdString >& aComboList ) {
    CSubItem listSubItem;
    if ( !GetSubItem( nItem, nSubItem, listSubItem ) )
      return FALSE;
    aComboList = listSubItem.m_aComboList;
    return aComboList.IsEmpty() ? GetColumnComboList( IndexToOrder( nSubItem ), aComboList ) : !aComboList.IsEmpty();
  }
  HFONT GetItemFont( int nItem, int nSubItem ) {
    CSubItem listSubItem;
    if ( !GetSubItem( nItem, nSubItem, listSubItem ) )
      return FALSE;
    return listSubItem.m_hFont == NULL ? base::GetItemFont( nItem, nSubItem ) : listSubItem.m_hFont;
  }
  BOOL GetItemColours( int nItem, int nSubItem, COLORREF& rgbBackground, COLORREF& rgbText ) {
    CSubItem listSubItem;
    if ( !GetSubItem( nItem, nSubItem, listSubItem ) )
      return FALSE;
    rgbBackground = listSubItem.m_rgbBackground;
    rgbText = listSubItem.m_rgbText;
    return TRUE;
  }
  CStdString GetItemToolTip( int nItem, int nSubItem ) {
    CListItem< TData > listItem;
    return GetItem( nItem, listItem ) ? listItem.m_strToolTip : _T( "" );
  }
  BOOL GetItemData( int nItem, TData& tData ) {
    CListItem< TData > listItem;
    if ( !GetItem( nItem, listItem ) )
      return FALSE;
    tData = listItem.m_tData;
    return TRUE;
  }
  virtual BOOL SetItemText( int nItem, int nSubItem, LPCTSTR lpszText ) {
    if ( nItem < 0 || nItem >= GetItemCount() )
      return FALSE;
    if ( nSubItem < 0 || nSubItem >= ( int ) this->m_aItems[ nItem ].m_aSubItems.GetSize() )
      return FALSE;
    this->m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_strText = lpszText;
    return TRUE;
  }
  BOOL SetItemComboIndex( int nItem, int nSubItem, int nIndex ) {
    CListArray < CStdString > aComboList;
    if ( !GetItemComboList( nItem, nSubItem, aComboList ) )
      return FALSE;
    return SetItemText( nItem, nSubItem, nIndex < 0 || nIndex >= aComboList.GetSize() ? _T( "" ) : aComboList[ nIndex ] );
  }
  BOOL SetItemImage( int nItem, int nSubItem, int nImage ) {
    if ( nItem < 0 || nItem >= GetItemCount() )
      return FALSE;
    if ( nSubItem < 0 || nSubItem >= ( int ) this->m_aItems[ nItem ].m_aSubItems.GetSize() )
      return FALSE;
    this->m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_nImage = nImage;
    return TRUE;
  }
  BOOL SetItemFormat( int nItem, int nSubItem, UINT nFormat, UINT nFlags = ITEM_FLAGS_NONE ) {
    if ( nItem < 0 || nItem >= GetItemCount() )
      return FALSE;
    if ( nSubItem < 0 || nSubItem >= ( int ) this->m_aItems[ nItem ].m_aSubItems.GetSize() )
      return FALSE;
    this->m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_nFormat = nFormat;
    this->m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_nFlags = nFlags;
    return TRUE;
  }
  BOOL SetItemFormat( int nItem, int nSubItem, UINT nFormat, UINT nFlags, CListArray < CStdString >& aComboList ) {
    if ( nItem < 0 || nItem >= GetItemCount() )
      return FALSE;
    if ( nSubItem < 0 || nSubItem >= ( int ) this->m_aItems[ nItem ].m_aSubItems.GetSize() )
      return FALSE;
    this->m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_nFormat = nFormat;
    this->m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_nFlags = nFlags;
    this->m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_aComboList = aComboList;
    return TRUE;
  }
  BOOL SetItemFont( int nItem, int nSubItem, HFONT hFont ) {
    if ( nItem < 0 || nItem >= GetItemCount() )
      return FALSE;
    if ( nSubItem < 0 || nSubItem >= ( int ) this->m_aItems[ nItem ].m_aSubItems.GetSize() )
      return FALSE;
    this->m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_hFont = hFont;
    return TRUE;
  }
  BOOL SetItemColours( int nItem, int nSubItem, COLORREF rgbBackground, COLORREF rgbText ) {
    if ( nItem < 0 || nItem >= GetItemCount() )
      return FALSE;
    if ( nSubItem < 0 || nSubItem >= ( int ) this->m_aItems[ nItem ].m_aSubItems.GetSize() )
      return FALSE;
    this->m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_rgbBackground = rgbBackground;
    this->m_aItems[ nItem ].m_aSubItems[ nSubItem ].m_rgbText = rgbText;
    return TRUE;
  }
  void ReverseItems() {
    this->m_aItems.Reverse();
  }
  class CompareItem {
  public:
    CompareItem( int nColumn ) : this->m_nColumn( nColumn ) {}
    inline bool operator() ( const CListItem< TData >& listItem1, const CListItem< TData >& listItem2 ) {
      return ( listItem1.m_aSubItems[ this->m_nColumn ].m_strText.Compare( listItem2.m_aSubItems[ this->m_nColumn ].m_strText ) < 0 );
    }
  protected:
    int this->m_nColumn;
  };
  void SortItems( int nColumn, BOOL bAscending ) {
    this->m_aItems.Sort( CompareItem( nColumn ) );
  }
  BOOL SetItemToolTip( int nItem, LPCTSTR lpszToolTip ) {
    if ( nItem < 0 || nItem >= GetItemCount() )
      return FALSE;
    this->m_aItems[ nItem ].m_strToolTip = lpszToolTip;
    return TRUE;
  }
  BOOL SetItemData( int nItem, TData& tData ) {
    if ( nItem < 0 || nItem >= GetItemCount() )
      return FALSE;
    this->m_aItems[ nItem ].m_tData = tData;
    return TRUE;
  }

typedef CListCtrlData< DWORD > CListCtrl;
int test_CListCtrl( CPaintManagerUI& this->m_pm ) {
  CListCtrl * pListCtrl = static_cast<CListCtrl*>( this->m_pm.FindControl( _T( "ListCtrl" ) ) );
  CFont this->m_fntCustomFont1;
  CFont this->m_fntCustomFont2;
  LOGFONT logFont = { 0 };
  logFont.lfCharSet = DEFAULT_CHARSET;
  logFont.lfHeight = 90;
  lstrcpy( logFont.lfFaceName, _T( "New Times Roman" ) );
  logFont.lfWeight = FW_BOLD;
  logFont.lfItalic = ( BYTE ) TRUE;
  this->m_fntCustomFont1.CreatePointFontIndirect( &logFont );
  logFont.lfHeight = 100;
  lstrcpy( logFont.lfFaceName, _T( "Arial" ) );
  logFont.lfUnderline = ( BYTE ) TRUE;
  this->m_fntCustomFont2.CreatePointFontIndirect( &logFont );
  CImageList this->m_ilItemImages;
#define IDB_EXAMPLE "ExampleItems.bmp"
  this->m_ilItemImages.CreateFromImage( IDB_EXAMPLE, 16, 0, RGB( 255, 0, 255 ), IMAGE_BITMAP, LR_CREATEDIBSECTION );
  pListCtrl->SetImageList( this->m_ilItemImages );
  pListCtrl->SetFocusSubItem( TRUE );
  pListCtrl->AddColumn( _T( "Column 1" ), 150 );
  pListCtrl->AddColumn( _T( "" ), 0, ITEM_IMAGE_3STATE, TRUE, ITEM_FORMAT_CHECKBOX_3STATE );
  pListCtrl->AddColumn( _T( "Column 2" ), 130 );
  pListCtrl->AddColumn( _T( "Column 3" ), 80, -1, FALSE, ITEM_FORMAT_NONE, ITEM_FLAGS_RIGHT );
  pListCtrl->AddColumn( _T( "Column 4" ), 120 );
  pListCtrl->AddColumn( _T( "Column 5" ), 170, ITEM_IMAGE_ATTACHMENT, FALSE, ITEM_FORMAT_PROGRESS );
  srand( 12345 );
  for ( int nItem = 1; nItem <= 1000; nItem++ ) {
    char sItemText[ 256 ];
    _snprintf( sItemText, 256, _T( "Item %d" ), nItem );
    int nNewItem = pListCtrl->AddItem( sItemText, rand() % 6 );
    pListCtrl->SetItemCheck( nNewItem, 1, ( rand() % 3 ) - 1 );
    TCHAR nSortChar = _T( 'A' ) + ( rand() % 26 );
    _snprintf( sItemText, 256, _T( "%c Random Text" ), nSortChar );
    pListCtrl->SetItemText( nNewItem, 2, sItemText );
    pListCtrl->SetItemImage( nNewItem, 2, rand() % 6 );
    _snprintf( sItemText, 256, _T( "%d" ), nItem );
    pListCtrl->SetItemText( nNewItem, 3, sItemText );
    _snprintf( sItemText, 256, _T( "%d" ), ( rand() % 101 ) );
    pListCtrl->SetItemText( nNewItem, 5, sItemText );
  }
  CListArray < CStdString > aComboList;
  aComboList.Add( CStdString( _T( "Item 1" ) ) );
  aComboList.Add( CStdString( _T( "Item 2" ) ) );
  aComboList.Add( CStdString( _T( "Item 3" ) ) );
  aComboList.Add( CStdString( _T( "Item 4" ) ) );
  aComboList.Add( CStdString( _T( "Item 5" ) ) );
  pListCtrl->SetItemText( 0, 4, _T( "Combo (Edit):" ) );
  pListCtrl->SetItemFormat( 0, 5, ITEM_FORMAT_COMBO, ITEM_FLAGS_COMBO_EDIT, aComboList );
  pListCtrl->SetItemComboIndex( 0, 5, 0 ); // "Item 1"
  pListCtrl->SetItemText( 1, 4, _T( "Combo (List):" ) );
  pListCtrl->SetItemFormat( 1, 5, ITEM_FORMAT_COMBO, ITEM_FLAGS_NONE, aComboList );
  pListCtrl->SetItemText( 1, 5, _T( "Item 1" ) );
  pListCtrl->SetItemText( 2, 4, _T( "Edit:" ) );
  pListCtrl->SetItemFormat( 2, 5, ITEM_FORMAT_EDIT );
  pListCtrl->SetItemText( 2, 5, _T( "<alpha-numeric>" ) );
  pListCtrl->SetItemText( 3, 4, _T( "Edit (Numeric):" ) );
  pListCtrl->SetItemFormat( 3, 5, ITEM_FORMAT_EDIT, ITEM_FLAGS_EDIT_NUMBER );
  pListCtrl->SetItemText( 3, 5, _T( "123" ) );
  CTime tmDateTime = CTime::GetCurrentTime();
  SYSTEMTIME stDateTime;
  tmDateTime.GetAsSystemTime( stDateTime );
  pListCtrl->SetItemText( 4, 4, _T( "Date (inc Time):" ) );
  pListCtrl->SetItemFormat( 4, 5, ITEM_FORMAT_DATETIME, ITEM_FLAGS_DATETIME_NONE );
  pListCtrl->SetItemDate( 4, 5, stDateTime );
  pListCtrl->SetItemText( 5, 4, _T( "Date (Date only):" ) );
  pListCtrl->SetItemFormat( 5, 5, ITEM_FORMAT_DATETIME, ITEM_FLAGS_DATE_ONLY );
  pListCtrl->SetItemDate( 5, 5, stDateTime );
  pListCtrl->SetItemText( 6, 4, _T( "Solid progress:" ) );
  pListCtrl->SetItemFormat( 6, 5, ITEM_FORMAT_PROGRESS, ITEM_FLAGS_PROGRESS_SOLID );
  pListCtrl->SetItemText( 7, 4, _T( "Normal progress:" ) );
  pListCtrl->SetItemText( 8, 4, _T( "Checkbox:" ) );
  pListCtrl->SetItemFormat( 8, 5, ITEM_FORMAT_CHECKBOX );
  pListCtrl->SetItemText( 9, 4, _T( "Checkbox (3 State):" ) );
  pListCtrl->SetItemFormat( 9, 5, ITEM_FORMAT_CHECKBOX_3STATE );
  pListCtrl->SetItemText( 10, 4, _T( "Checkbox (read-only):" ) );
  pListCtrl->SetItemFormat( 10, 5, ITEM_FORMAT_CHECKBOX, ITEM_FLAGS_READ_ONLY );
  pListCtrl->SetItemText( 11, 4, _T( "Hyperlink" ) );
  pListCtrl->SetItemFormat( 11, 4, ITEM_FORMAT_HYPERLINK );
  pListCtrl->SetItemText( 12, 4, _T( "Some very long text that should trigger titletip" ) );
  pListCtrl->SetItemText( 13, 4, _T( "Built-in tooltip support" ) );
  pListCtrl->SetItemToolTip( 13, _T( "This is a\nmultiline tooltip" ) );
  pListCtrl->SetItemText( 14, 4, _T( "Font Support" ) );
  pListCtrl->SetItemFont( 14, 4, this->m_fntCustomFont1 );
  pListCtrl->SetItemText( 15, 4, _T( "Font Support" ) );
  pListCtrl->SetItemFont( 15, 4, this->m_fntCustomFont2 );
  pListCtrl->SetItemText( 16, 4, _T( "Colour Support" ) );
  pListCtrl->SetItemColours( 16, 4, RGB( 128, 128, 64 ), RGB( 0, 255, 0 ) );
  pListCtrl->SetItemText( 17, 4, _T( "Colour Support" ) );
  pListCtrl->SetItemColours( 17, 4, RGB( 128, 0, 128 ), RGB( 255, 255, 128 ) );
  return 0;
}
#endif
