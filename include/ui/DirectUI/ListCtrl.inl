
#include "ListTypes.h"
#include "ListCtrl.h"
#define UIListCtrl_SetPos CContainerUI_SetPos
#define UIListCtrl_SetAttribute CContainerUI_SetAttribute

#define GetListInfo(b)      ((this->cb) ? this->cb->f_GetList(this->pListData, 0, 0, &b, 0, 0) : (b.nColumnCount=0,b.nItemCount=0))
#define GetColumn(a, b)     this->cb->f_GetList(this->pListData, 0, a, 0, &b, 0)
#define GetSubItem(a, b, c) this->cb->f_GetList(this->pListData, a, b, 0, 0, &c)
#define GetFocusItem(a, b)
#define GetItemText(a, b, c, d)
#define IsSelected(a) (this->m_setSelectedItems ? this->m_setSelectedItems[a] : 0)
#define UIGetFocus(ww)  (ww->m_pFocus)

int UIListCtrl_CtrFree(CControlUI* ctr) {
  UIListCtrl* this = (UIListCtrl*)ctr;
  DEL(this->m_setSelectedItems);
  CContainerUI_CtrFree(ctr);
  return 0;
}
int UIListCtrl_CtrInit(CControlUI* ctr) {
  UIListCtrl* this = (UIListCtrl*)ctr;
  CContainerUI_CtrInit(ctr);
  this->m_bShowHeader = TRUE;
  this->m_bShowThemed = FALSE;
  this->m_bSortAscending = TRUE;
  this->m_bButtonDown = FALSE;
  this->m_bMouseOver = FALSE;
  this->m_bColumnSizing = FALSE;
  this->m_bBeginSelect = FALSE;
  this->m_bSingleSelect = FALSE;
  this->m_bFocusSubItem = FALSE;
  this->m_bGroupSelect = FALSE;
  this->m_bEnableHorizScroll = FALSE;
  this->m_bEnableVertScroll = FALSE;
  this->m_bShowHorizScroll = TRUE;
  this->m_bShowVertScroll = TRUE;
  this->m_bShowSort = TRUE;
  this->m_bResizeTimer = FALSE;
  this->m_bDragDrop = FALSE;
  this->m_bSmoothScroll = TRUE;
  this->m_bEditItem = FALSE;
  this->m_bScrolling = FALSE;
  this->m_bScrollDown = FALSE;
  this->m_bTileBackground = FALSE;
  this->m_nMouseWheelScroll = 3;
  this->m_nTotalWidth = 0;
  this->m_nHeaderHeight = 0;
  this->m_nItemHeight = 0;
  this->m_nFirstSelected = NULL_ITEM;
  this->m_nFocusItem = NULL_ITEM;
  this->m_nFocusSubItem = NULL_SUBITEM;
  this->m_nHotItem = NULL_ITEM;
  this->m_nHotSubItem = NULL_SUBITEM;
  this->m_nTitleTipItem = NULL_ITEM;
  this->m_nTitleTipSubItem = NULL_SUBITEM;
  this->m_nSortColumn = NULL_COLUMN;
  this->m_nHighlightColumn = NULL_COLUMN;
  this->m_nDragColumn = NULL_COLUMN;
  this->m_nHotColumn = NULL_COLUMN;
  this->m_nHotDivider = NULL_COLUMN;
  this->m_nColumnSizing = NULL_COLUMN;
  this->m_strSearchString = _T( "" );
  this->m_rgbBackground = GetSysColor( COLOR_WINDOW );
  this->m_rgbHeaderBackground = GetSysColor( COLOR_BTNFACE );
  this->m_rgbHeaderBorder = GetSysColor( COLOR_3DHIGHLIGHT );
  this->m_rgbHeaderShadow = GetSysColor( COLOR_3DSHADOW );
  this->m_rgbHeaderText = GetSysColor( COLOR_WINDOWTEXT );
  this->m_rgbHeaderHighlight = RGB( 130, 140, 180 );
  this->m_rgbSelectedItem = GetSysColor( COLOR_HIGHLIGHT );
  this->m_rgbSelectedText = GetSysColor( COLOR_HIGHLIGHTTEXT );
  this->m_rgbItemText = GetSysColor( COLOR_WINDOWTEXT );
  this->m_rgbSelectOuter = RGB( 170, 200, 245 );
  this->m_rgbSelectInner = RGB( 230, 250, 250 );
  this->m_rgbSelectTop = RGB( 210, 240, 250 );
  this->m_rgbSelectBottom = RGB( 185, 215, 250 );
  this->m_rgbNoFocusTop = RGB( 250, 250, 250 );
  this->m_rgbNoFocusBottom = RGB( 235, 235, 235 );
  this->m_rgbNoFocusOuter = RGB( 220, 220, 220 );
  this->m_rgbNoFocusInner = RGB( 245, 245, 245 );
  this->m_rgbFocusTop = RGB( 235, 245, 245 );
  this->m_rgbFocusBottom = RGB( 225, 235, 245 );
  this->m_rgbProgressTop = RGB( 170, 240, 170 );
  this->m_rgbProgressBottom = RGB( 45, 210, 50 );
  this->m_rgbItemFocus = RGB( 180, 190, 210 );
  this->m_rgbHyperLink = RGB( 0, 0, 200 );
  this->m_nHeaderClipboardFormat = RegisterClipboardFormat( _T( "HEADERCLIPBOARDFORMAT" ) );
  // get number of lines to scroll
#if 0
#define SPI_GETWHEELSCROLLLINES    0x0068
  SystemParametersInfo( SPI_GETWHEELSCROLLLINES, 0, &this->m_nMouseWheelScroll, 0 );
  // get system message font
  CLogFont logFont;
  logFont.SetMessageBoxFont();
  if ( !m_fntListFont.IsNull() )
    this->m_fntListFont.DeleteObject();
  if ( this->m_fntListFont.CreateFontIndirect( &logFont ) == NULL )
    return FALSE;
  // get system underline font
  logFont.lfUnderline = BYTE( TRUE );
  if ( !m_fntUnderlineFont.IsNull() )
    this->m_fntUnderlineFont.DeleteObject();
  if ( this->m_fntUnderlineFont.CreateFontIndirect( &logFont ) == NULL )
    return FALSE;
  CClientDC dcClient( M_HWND );
  HFONT hOldFont = dcClient.SelectFont( this->m_fntListFont );
  CSize sizeExtent;
  if ( !dcClient.GetTextExtent( _T( "Height" ), -1, &sizeExtent ) )
    return FALSE;
  dcClient.SelectFont( hOldFont );
  // has system font changed
  if ( this->m_nItemHeight != sizeExtent.cy + ITEM_HEIGHT_MARGIN ) {
    this->m_nItemHeight = sizeExtent.cy + ITEM_HEIGHT_MARGIN;
    this->m_nHeaderHeight = this->m_nItemHeight;
    // create drop arrows window
    //if ( !m_wndDropArrows->Create( M_HWND, this->m_nHeaderHeight, TRUE ) )
    //  return FALSE;
  }
  // create titletip window
  //if ( !m_wndTitleTip->Create( M_HWND, this->m_bShowThemed ) )
  //  return FALSE;
#endif
  return 0;
}
#define GetClientRect(rc)  ((rc) = ctr->m_rcItem)
bool GetBitmap( HBITMAP m_hBitmap, BITMAP* bm ) {
  return ( GetObject( m_hBitmap, sizeof( BITMAP ), &bm ) == sizeof( BITMAP ) );
}
bool GetBmpSize( HBITMAP m_hBitmap, SIZE* size ) {
  BITMAP bm;
  if ( !GetBitmap( m_hBitmap, &bm ) )
    return false;
  size->cx = bm.bmWidth;
  size->cy = bm.bmHeight;
  return true;
}
void UIListCtrl_DrawBkgnd( CWindowWnd* ww, CControlUI* ctr, HDC dcPaint, RECT rcClip ) {
  UIListCtrl* this = (UIListCtrl*)ctr;
  RECT rcClient;
  SetBkColor( dcPaint, this->m_rgbBackground );
  ExtTextOut( dcPaint, rcClip.left, rcClip.top, ETO_OPAQUE, &rcClip, _T( "" ), 0, NULL );
  GetClientRect( rcClient );
  rcClient.top = ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 );
  if ( this->m_bmpBackground!=NULL && rcClip.bottom > rcClient.top ) {
    SIZE sizBackground;
    HDC dcBackgroundImage;
    HBITMAP hOldBitmap;
    GetBmpSize( this->m_bmpBackground, &sizBackground );
    dcBackgroundImage=CreateCompatibleDC( dcPaint );
    hOldBitmap = SelectBitmap( dcBackgroundImage, this->m_bmpBackground );
    if ( this->m_bTileBackground ) {
      // calculate tile image maximum rows and columns
      div_t divRows = div( rcHeight(rcClient), sizBackground.cy );
      int nTileRows = divRows.rem > 0 ? divRows.quot + 1 : divRows.quot;
      div_t divColumns = div( rcWidth(rcClient), sizBackground.cx );
      int nTileColumns = divColumns.rem > 0 ? divColumns.quot + 1 : divColumns.quot;
      int nRow, nColumn;
      // draw tiled background image
      for ( nRow = 0; nRow <= nTileRows; nRow++ ) {
        for ( nColumn = 0; nColumn <= nTileColumns; nColumn++ )
          BitBlt( dcPaint, nColumn * sizBackground.cx, nRow * sizBackground.cy, sizBackground.cx, sizBackground.cy, dcBackgroundImage, 0, 0, SRCCOPY );
      }
    } else {
      RECT rcCentreImage = ( rcClient );
      // horizontally centre image if smaller than the client width
      if ( sizBackground.cx < rcWidth(rcClient) ) {
        rcCentreImage.left = ( rcWidth(rcClient) / 2 ) - ( int ) ( sizBackground.cx / 2 );
        rcCentreImage.right = rcCentreImage.left + sizBackground.cx;
      }
      // vertically centre image if smaller than the client height
      if ( sizBackground.cy + 16 < rcHeight(rcClient) ) {
        rcCentreImage.top = ( rcHeight(rcClient) / 2 ) - ( int ) ( ( sizBackground.cy + 16 ) / 2 );
        rcCentreImage.bottom = rcCentreImage.top + sizBackground.cy;
      }
      // draw centred background image
      BitBlt( dcPaint, rcCentreImage.left, rcCentreImage.top, rcWidth(rcCentreImage), rcHeight(rcCentreImage), dcBackgroundImage, 0, 0, SRCCOPY );
    }
    SelectBitmap( dcBackgroundImage, hOldBitmap );
    DeleteDC(dcBackgroundImage);
  }
}
int UIListCtrl_GetTotalWidth( CControlUI* ctr ) {
  UIListCtrl* this = (UIListCtrl*)ctr;
  int nColumn = 0;
  CListInfo listInfo = {0};
  this->m_nTotalWidth = 0;
  GetListInfo(listInfo);
  for ( ; nColumn < listInfo.nColumnCount; nColumn++ ) {
    CListColumn listColumn;
    GetColumn(nColumn, listColumn);
    this->m_nTotalWidth += listColumn.m_nWidth;
  }
  return this->m_nTotalWidth - 1;
}
int UIListCtrl_GetTopItem(CControlUI* ctr) {
  UIListCtrl* this = (UIListCtrl*)ctr;
  return ( int ) ( this->m_nItemHeight ? CContainerUI_GetScrollPos(ctr).cy / this->m_nItemHeight : -1 );
}
POINT cPoint(int x, int y) {POINT pt;pt.x=x,pt.y=y;return pt;}
#define RoundRect2( m_hDC, lpRect, x, y )  RoundRect( m_hDC, lpRect.left, lpRect.top, lpRect.right, lpRect.bottom, x, y )

void DrawRoundRect( HDC dcPaint, RECT rcRect, COLORREF rgbOuter, COLORREF rgbInner, COLORREF rgbBackground ) {
  RECT rcRoundRect=( rcRect );
  HPEN penBorder, penInnerBorder;
  HBRUSH bshInterior;
  penBorder=CreatePen( PS_SOLID, 1, rgbOuter );
  bshInterior = CreateSolidBrush( rgbBackground );
  SelectPen( dcPaint, penBorder );
  SelectBrush( dcPaint, bshInterior );
  RoundRect2( dcPaint, rcRoundRect, 5, 5 );
  DeflateRect( &rcRoundRect, 1, 1 );
  penInnerBorder = CreatePen( PS_SOLID, 1, rgbInner );
  SelectPen( dcPaint, penInnerBorder );
  RoundRect2( dcPaint, rcRoundRect, 2, 2 );
  DeleteObject(penInnerBorder);
  DeleteObject(penBorder);
  DeleteObject(bshInterior);
}
void DrawGradient2( HDC dcPaint, RECT rcRect, COLORREF rgbTop, COLORREF rgbBottom ) {
  GRADIENT_RECT grdRect = { 0, 1 };
  TRIVERTEX triVertext[ 2 ] = {
    rcRect.left,
      rcRect.top,
      GetRValue( rgbTop ) << 8,
      GetGValue( rgbTop ) << 8,
      GetBValue( rgbTop ) << 8,
      0x0000,
      rcRect.right,
      rcRect.bottom,
      GetRValue( rgbBottom ) << 8,
      GetGValue( rgbBottom ) << 8,
      GetBValue( rgbBottom ) << 8,
      0x0000
  };
  GradientFill( dcPaint, triVertext, 2, &grdRect, 1, GRADIENT_FILL_RECT_V );
}
BOOL GetIconSize( HIMAGELIST m_hImageList, SIZE* size ) {
  return ImageList_GetIconSize( m_hImageList, ( int* ) & size->cx, ( int* ) & size->cy );
}
BOOL DrawEx2( HIMAGELIST m_hImageList, int nImage, HDC hDC, int x, int y, int dx, int dy, COLORREF rgbBk, COLORREF rgbFg, UINT fStyle ) {
  return ImageList_DrawEx( m_hImageList, nImage, hDC, x, y, dx, dy, rgbBk, rgbFg, fStyle );
}
BOOL DrawEx( HIMAGELIST m_hImageList, int nImage, HDC hDC, RECT* rect, COLORREF rgbBk, COLORREF rgbFg, UINT fStyle ) {
  return ImageList_DrawEx( m_hImageList, nImage, hDC, rect->left, rect->top, rect->right - rect->left, rect->bottom - rect->top, rgbBk, rgbFg, fStyle );
}
void DrawItem( CWindowWnd* ww, CControlUI* ctr, HDC dcPaint, int nItem, RECT rcItem ) {
  UIListCtrl* this = (UIListCtrl*)ctr;
  int nFocusItem, nFocusSubItem, nSubItem, nColumnCount;
  BOOL bSelectedItem, bControlFocus;
  RECT rcClip, rcSubItem, rcItemText;
  BOOL bFocusSubItem;
  CListInfo listInfo;
  COLORREF rgbBackground;
  COLORREF rgbText;
  COLORREF rgbOuter, rgbInner, rgbTop, rgbBottom;
  UIResource* res = ww->m_pResource;
  DWORD dwTextColor = UIResource_GetDefaultFontColor(res);
  if ( GetClipBox( dcPaint, &rcClip ) == ERROR )
    return ;
  nFocusItem = NULL_ITEM;
  nFocusSubItem = NULL_SUBITEM;
  GetFocusItem( nFocusItem, nFocusSubItem );
  bSelectedItem = IsSelected( nItem );
  bControlFocus = ( UIGetFocus(ww) == ctr || this->m_bEditItem );
  // draw selected background
  if ( bSelectedItem ) {
    if ( this->m_bShowThemed ) {
      RECT rcSelect=( rcItem );
      // draw select border
      rgbOuter = bControlFocus ? this->m_rgbSelectOuter : this->m_rgbNoFocusOuter;
      rgbInner = bControlFocus ? this->m_rgbSelectInner : this->m_rgbNoFocusInner;
      DrawRoundRect( dcPaint, rcItem, rgbOuter, rgbInner, this->m_rgbBackground );
      DeflateRect( &rcSelect, 2, 2 );
      // fill selected area
      rgbTop=bControlFocus ? this->m_rgbSelectTop : this->m_rgbNoFocusTop;
      rgbBottom = bControlFocus ? this->m_rgbSelectBottom : this->m_rgbNoFocusBottom;
      DrawGradient2( dcPaint, rcSelect, rgbTop, rgbBottom );
    } else {
      SetBkColor( dcPaint, this->m_rgbSelectedItem );
      ExtTextOut( dcPaint, rcItem.left, rcItem.top, ETO_OPAQUE, &rcItem, _T( "" ), 0, NULL );
    }
  }
  rcSubItem = rcItem;
  rcSubItem.right = rcSubItem.left;
  GetListInfo(listInfo);
  nSubItem = 0, nColumnCount = listInfo.nColumnCount;
  for ( ; nSubItem < nColumnCount; rcSubItem.left = rcSubItem.right + 1, nSubItem++ ) {
    CListColumn listColumn;
    CSubItem subItem;
    TCHAR* strItemText;
    int nItemImage;
    UINT nItemFlags;
    UINT nFormat;
    GetColumn( nSubItem, listColumn );
    rcSubItem.right = rcSubItem.left + listColumn.m_nWidth - 1;
    if ( rcSubItem.right < rcClip.left || rcWidth(rcSubItem) == 0 )
      continue;
    if ( rcSubItem.left > rcClip.right )
      break;
    GetSubItem( nItem, nSubItem, subItem );
    strItemText = subItem.m_strText;
    nItemImage = subItem.m_nItemImage;
    nItemFlags = subItem.m_nItemFlags;
    rgbBackground = subItem.m_rgbBackground;
    rgbText = subItem.m_rgbText;
    // custom draw subitem format
    bFocusSubItem = ( this->m_bFocusSubItem && nFocusItem == nItem && nFocusSubItem == nSubItem );
    rgbBackground = this->m_rgbBackground;
    rgbText = this->m_rgbItemText;
    if ( bFocusSubItem ) {
      if ( this->m_bShowThemed ) {
        // only draw subitem focus if control has focus
        if ( bControlFocus ) {
          RECT rcSelect;
          rgbOuter = bControlFocus ? this->m_rgbSelectOuter : this->m_rgbNoFocusOuter;
          rgbInner = bControlFocus ? this->m_rgbSelectInner : this->m_rgbNoFocusInner;
          // draw select border
          DrawRoundRect( dcPaint, rcSubItem, rgbOuter, rgbInner, this->m_rgbBackground );
          rcSelect =( rcSubItem );
          DeflateRect( &rcSelect, 2, 2 );
          // fill selected area
          if ( !this->m_bEditItem ) {
            DrawGradient2( dcPaint, rcSelect, this->m_rgbFocusTop, this->m_rgbFocusBottom );
          } else {
            SetBkColor( dcPaint, this->m_rgbBackground );
            ExtTextOut( dcPaint, rcSelect.left, rcSelect.top, ETO_OPAQUE, &rcSelect, _T( "" ), 0, NULL );
          }
        }
      } else {
        SetBkColor(dcPaint, this->m_bEditItem ? this->m_rgbBackground : this->m_rgbItemFocus );
        ExtTextOut(dcPaint, rcSubItem.left, rcSubItem.top, ETO_OPAQUE, &rcSubItem, _T( "" ), 0, NULL );
        if ( this->m_bEditItem ) {
          HBRUSH bshSelectFrame;
          bshSelectFrame=CreateSolidBrush( this->m_rgbItemFocus );
          FrameRect( dcPaint, &rcSubItem, bshSelectFrame );
          DeleteObject(bshSelectFrame);
        }
      }
    } else if ( rgbBackground != this->m_rgbBackground ) {
      HPEN penBorder;
      HBRUSH bshInterior;
      penBorder=CreatePen( PS_SOLID, 1, rgbBackground );
      bshInterior=CreateSolidBrush( rgbBackground );
      SelectPen(dcPaint, penBorder );
      SelectBrush( dcPaint, bshInterior );
      RoundRect2( dcPaint, rcSubItem, 3, 3 );
      DeleteObject(bshInterior);
      DeleteObject(penBorder);
    }
    rcItemText=( rcSubItem );
    // margin item text
    rcItemText.left += nSubItem == 0 ? 4 : 3;
    DeflateRect( &rcItemText, 4, 0 );
    // draw subitem image if supplied
    if ( this->m_ilItemImages!=NULL && nItemImage != ITEM_IMAGE_NONE && ( !this->m_bEditItem || ( this->m_bEditItem && !bFocusSubItem ) ) ) {
      SIZE sizeIcon;
      RECT rcItemImage;
      GetIconSize( this->m_ilItemImages, &sizeIcon );
      rcItemImage.left = IsEmpty(strItemText) ? ( ( rcItemText.left + rcItemText.right ) / 2 ) - ( sizeIcon.cx / 2 ) - ( ( !this->m_bShowThemed ) ? 0 : 1 ) : rcItemText.left;
      rcItemImage.right = min( rcItemImage.left + sizeIcon.cx, rcSubItem.right );
      rcItemImage.top = ( ( rcSubItem.top + rcSubItem.bottom ) / 2 ) - ( sizeIcon.cy / 2 );
      rcItemImage.bottom = min( rcItemImage.top + sizeIcon.cy, rcSubItem.bottom );
      DrawEx( this->m_ilItemImages, nItemImage, dcPaint, &rcItemImage, CLR_DEFAULT, CLR_DEFAULT, ILD_TRANSPARENT );
      // offset item text (for image)
      rcItemText.left += sizeIcon.cx + 4;
    }
    if ( IsRectEmpty(&rcItemText) )
      continue;
    SelectFont( dcPaint, subItem.m_hFont );
    dwTextColor = ( ( ( !this->m_bShowThemed ) && bSelectedItem && !bFocusSubItem ) ? this->m_rgbSelectedText : rgbText );
    SetBkMode( dcPaint, TRANSPARENT );
    nFormat = DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS;
    if ( nItemFlags & ITEM_FLAGS_CENTRE )
      nFormat |= DT_CENTER;
    else if ( nItemFlags & ITEM_FLAGS_RIGHT )
      nFormat |= DT_RIGHT;
    else
      nFormat |= DT_LEFT;
    switch ( subItem.m_nItemFormat ) {
    case ITEM_FORMAT_DATETIME:
      if ( !IsEmpty(strItemText) ) {
#if 0
        SYSTEMTIME stItemDate;
        if ( !GetItemDate( nItem, nSubItem, stItemDate ) )
          break;
        CStdString strItemDate;
        if ( nItemFlags & ITEM_FLAGS_DATE_ONLY )
          strItemDate = FormatDate( stItemDate );
        else if ( nItemFlags & ITEM_FLAGS_TIME_ONLY )
          strItemDate = FormatTime( stItemDate );
        else
          strItemDate = FormatDate( stItemDate ) + _T( " " ) + FormatTime( stItemDate );
        dcPaint.DrawText( strItemDate, strItemDate.GetLength(), rcItemText, nFormat );
#endif
      }
      break;
    case ITEM_FORMAT_CHECKBOX:
    case ITEM_FORMAT_CHECKBOX_3STATE: 
      {
#if 0
        SIZE sizeIcon;
        this->m_ilListItems.GetIconSize( sizeIcon );
        RECT rcCheckBox;
        rcCheckBox.left = ( ( rcItemText.left + rcItemText.right ) / 2 ) - ( sizeIcon.cx / 2 ) - 1;
        rcCheckBox.right = min( rcCheckBox.left + sizeIcon.cx, rcSubItem.right );
        rcCheckBox.top = ( ( rcSubItem.top + rcSubItem.bottom ) / 2 ) - ( sizeIcon.cy / 2 );
        rcCheckBox.bottom = min( rcCheckBox.top + sizeIcon.cy, rcSubItem.bottom );
        int nCheckValue = _ttoi( strItemText );
        if ( nItemFormat == ITEM_FORMAT_CHECKBOX )
          this->m_ilListItems.DrawEx( nCheckValue > 0 ? ITEM_IMAGE_CHECK_ON : ITEM_IMAGE_CHECK_OFF, dcPaint, rcCheckBox, CLR_DEFAULT, CLR_DEFAULT, ILD_TRANSPARENT );
        else {
          int nCheckImage = ITEM_IMAGE_3STATE_UNDEF;
          if ( nCheckValue < 0 )
            nCheckImage = ITEM_IMAGE_3STATE_OFF;
          else if ( nCheckValue > 0 )
            nCheckImage = ITEM_IMAGE_3STATE_ON;
          this->m_ilListItems.DrawEx( nCheckImage, dcPaint, rcCheckBox, CLR_DEFAULT, CLR_DEFAULT, ILD_TRANSPARENT );
        }
#endif
      }
      break;
    case ITEM_FORMAT_PROGRESS: 
      {
      RECT rcProgress=( rcSubItem );
      DeflateRect( &rcProgress, 3, 2 );
      // draw progress border
      DrawRoundRect( dcPaint, rcProgress, this->m_rgbHeaderShadow, this->m_rgbHeaderBackground, this->m_rgbBackground );
      // fill progress bar area
      DeflateRect( &rcProgress, 3, 3 );
#define _tstof(x) _tcstod(x, NULL)
      rcProgress.right = rcProgress.left + ( int ) ( ( double ) rcWidth(rcProgress) * ( ( max( min( _tstof( strItemText ), 100 ), 0 ) ) / 100.0 ) );
      DrawGradient2( dcPaint, rcProgress, this->m_rgbProgressTop, this->m_rgbProgressBottom );
      }
      break;
    case ITEM_FORMAT_HYPERLINK:
      if ( nItem == this->m_nHotItem && nSubItem == this->m_nHotSubItem && !( nItemFlags & ITEM_FLAGS_READ_ONLY ) ) {
        SelectFont( dcPaint, this->m_fntUnderlineFont );
        SetTextColor( dcPaint, this->m_rgbHyperLink );
      }
    default:                              // draw item text
      if ( !IsEmpty(strItemText) ) {
        //DrawText( dcPaint, strItemText, strlen(strItemText), &rcItemText, nFormat );
        //UIDrawText( dcPaint, strItemText, strlen(strItemText), &rcItemText, nFormat );
        int iFont=0;
        UIDrawText( dcPaint, res, rcItemText, strItemText, dwTextColor, iFont, nFormat );
      }
      break;
    }
  }
}
void UIListCtrl_DrawList( CWindowWnd* ww, CControlUI* ctr, HDC dcPaint ) {
  UIListCtrl* this = (UIListCtrl*)ctr;
  RECT rcClip, rcItem;
  CListInfo listInfo;
  int nItem, nItemCount;
  if ( GetClipBox( dcPaint, &rcClip ) == ERROR )
    return ;
  GetClientRect( rcItem );
  rcItem.left = -CContainerUI_GetScrollPos(ctr).cx;
  rcItem.right = UIListCtrl_GetTotalWidth(ctr);
  rcItem.top += ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 );
  rcItem.bottom = rcItem.top;
  //ClientToScreen( rcItem );
  // draw all visible items
  GetListInfo(listInfo);
  nItem = UIListCtrl_GetTopItem(ctr);
  nItemCount = listInfo.nItemCount;
  for ( ; nItem < nItemCount; rcItem.top = rcItem.bottom, nItem++ ) {
    rcItem.bottom = rcItem.top + this->m_nItemHeight;
    if ( rcItem.bottom < rcClip.top || rcItem.right < rcClip.left )
      continue;
    if ( rcItem.top > rcClip.bottom || rcItem.left > rcClip.right )
      break;
    // may be implemented in a derived class
    DrawItem( ww, ctr, dcPaint, nItem, rcItem );
  }
}
void Draw3dRect( HDC m_hDC, int x, int y, int cx, int cy, COLORREF clrTopLeft, COLORREF clrBottomRight ) {
  FillSolidRect( m_hDC, x, y, cx - 1, 1, clrTopLeft );
  FillSolidRect( m_hDC, x, y, 1, cy - 1, clrTopLeft );
  FillSolidRect( m_hDC, x + cx, y, -1, cy, clrBottomRight );
  FillSolidRect( m_hDC, x, y + cy, cx, -1, clrBottomRight );
}
void UIListCtrl_DrawHeader( CWindowWnd* ww, CControlUI* ctr, HDC dcPaint ) {
  UIListCtrl* this = (UIListCtrl*)ctr;
  RECT rcClip, rcHeader, rcHeaderItem;
  HPEN penHighlight, penShadow;
  CListInfo listInfo;
  int nHeaderWidth, nColumn, nColumnCount;
  if ( !this->m_bShowHeader )
    return ;
  if ( GetClipBox( dcPaint, &rcClip ) == ERROR )
    return ;
  //ScreenToClient(rcClip);
  GetClientRect( rcHeader );
  //rcHeader.bottom = this->m_nHeaderHeight;
  //GetWindowRect( rcHeader );
  rcHeader = ctr->m_rcItem;
  rcHeader.bottom = rcHeader.top + this->m_nHeaderHeight;
  if ( rcClip.top > rcHeader.bottom )
    return ;
  if ( !this->m_bShowThemed ) {
    SetBkColor( dcPaint, this->m_rgbHeaderBackground );
    ExtTextOut( dcPaint, rcHeader.left, rcHeader.top, ETO_OPAQUE, &rcHeader, _T( "" ), 0, NULL );
  }
  penHighlight=CreatePen( PS_SOLID, 1, this->m_rgbHeaderBorder );
  penShadow=CreatePen( PS_SOLID, 1, this->m_rgbHeaderShadow );
  rcHeaderItem=( rcHeader );
  OffsetRect( &rcHeaderItem, -CContainerUI_GetScrollPos(ctr).cx, 0 );
  nHeaderWidth = 0;
  GetListInfo(listInfo);
  nColumn = 0, nColumnCount = listInfo.nColumnCount;
  for ( ; nColumn < nColumnCount; rcHeaderItem.left = rcHeaderItem.right, nColumn++ ) {
    CListColumn listColumn;
    RECT rcHeaderText;
    BOOL bShowArrow;
    UINT nFormat;
    int nSubItem = nColumn;
    GetColumn( nColumn, listColumn );
    rcHeaderItem.right = rcHeaderItem.left + listColumn.m_nWidth;
    nHeaderWidth += rcWidth(rcHeaderItem);
    if ( rcHeaderItem.right < rcClip.left )
      continue;
    if ( rcHeaderItem.left > rcClip.right )
      break;
    // draw header and divider
    {
      if ( nColumn == this->m_nHighlightColumn ) {
        SetBkColor( dcPaint, this->m_rgbHeaderHighlight );
        ExtTextOut( dcPaint, rcHeaderItem.left, rcHeaderItem.top, ETO_OPAQUE, &rcHeaderItem, _T( "" ), 0, NULL );
      }
      SelectPen( dcPaint, penShadow );
      MoveTo( dcPaint, rcHeaderItem.right - 1, rcHeaderItem.top + 4 );
      LineTo( dcPaint, rcHeaderItem.right - 1, rcHeaderItem.top + this->m_nHeaderHeight - 4 );
      SelectPen( dcPaint, penHighlight );
      MoveTo( dcPaint, rcHeaderItem.right, rcHeaderItem.top + 4 );
      LineTo( dcPaint, rcHeaderItem.right, rcHeaderItem.top + this->m_nHeaderHeight - 4 );
    }
    rcHeaderText =( rcHeaderItem );
    rcHeaderText.left += nColumn == 0 ? 4 : 3;
    OffsetRect( &rcHeaderText, 0, 1 );
    bShowArrow = this->m_bShowSort && ( rcWidth(rcHeaderItem) > 15 );
    // offset text bounding rectangle to account for sorting arrow
    if ( bShowArrow && !listColumn.m_bFixed && nSubItem == this->m_nSortColumn )
      rcHeaderText.right -= 15;
    // margin header text
    DeflateRect4( &rcHeaderText, 4, 0, 5, 0 );
    // has this header item an associated image?
    if ( this->m_ilListItems && listColumn.m_nImage != ITEM_IMAGE_NONE ) {
      SIZE sizeIcon;
      RECT rcHeaderImage;
      GetIconSize( this->m_ilListItems, &sizeIcon );
      rcHeaderImage.left = IsEmpty(listColumn.m_strText) ? ( ( rcHeaderText.left + rcHeaderText.right ) / 2 ) - ( sizeIcon.cx / 2 ) - ( ( !this->m_bShowThemed ) ? 0 : 1 ) : rcHeaderText.left;
      rcHeaderImage.right = min( rcHeaderImage.left + sizeIcon.cx, rcHeaderItem.right - 2 );
      rcHeaderImage.top = ( ( rcHeaderItem.top + rcHeaderItem.bottom ) / 2 ) - ( sizeIcon.cy / 2 );
      rcHeaderImage.bottom = min( rcHeaderImage.top + sizeIcon.cy, rcHeaderItem.bottom );
      DrawEx( this->m_ilListItems, listColumn.m_nImage, dcPaint, &rcHeaderImage, CLR_DEFAULT, CLR_DEFAULT, ILD_TRANSPARENT );
      // offset header text (for image)
      rcHeaderText.left += sizeIcon.cx + 4;
    }
    SelectFont( dcPaint, this->m_fntListFont );
    SetTextColor( dcPaint, this->m_rgbHeaderText );
    SetBkMode( dcPaint, TRANSPARENT );
    nFormat = DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS;
    if ( listColumn.m_nFlags & ITEM_FLAGS_CENTRE )
      nFormat |= DT_CENTER;
    else if ( listColumn.m_nFlags & ITEM_FLAGS_RIGHT )
      nFormat |= DT_RIGHT;
    else
      nFormat |= DT_LEFT;
    // draw header text
    if ( !IsRectEmpty(&rcHeaderText) && !IsEmpty(listColumn.m_strText) )
      DrawText( dcPaint, listColumn.m_strText, strlen(listColumn.m_strText), &rcHeaderText, nFormat );
    // draw sorting arrow
    if ( bShowArrow && !listColumn.m_bFixed && nSubItem == this->m_nSortColumn ) {
      SIZE sizeIcon;
      RECT rcSortArrow;
      GetIconSize( this->m_ilListItems, &sizeIcon );
      rcSortArrow.left = rcHeaderText.right + 4;
      rcSortArrow.right = min( rcSortArrow.left + sizeIcon.cx, rcHeaderItem.right );
      rcSortArrow.top = rcHeight(rcHeaderItem) / 2 - 3;
      rcSortArrow.bottom = min( rcSortArrow.top + sizeIcon.cy, rcHeaderItem.bottom );
      DrawEx( this->m_ilListItems, this->m_bSortAscending ? ITEM_IMAGE_UP : ITEM_IMAGE_DOWN, dcPaint, &rcSortArrow, CLR_DEFAULT, CLR_DEFAULT, ILD_TRANSPARENT );
    }
  }
  // draw remaining blank header
  // draw a frame around all header columns
  if ( ( !this->m_bShowThemed ) && nHeaderWidth > 0 ) {
    Draw3dRect( dcPaint, rcHeader.left, rcHeader.top, rcWidth(rcHeader) + 2, rcHeight(rcHeader), this->m_rgbHeaderBorder, this->m_rgbHeaderShadow );
  }
  DeleteObject(penHighlight);
  DeleteObject(penShadow);
}
int UIListCtrl_GetItemCount(CControlUI* ctr) {
  UIListCtrl* this = (UIListCtrl*)ctr;
  CListInfo listInfo = {0};
  GetListInfo(listInfo);
  return listInfo.nItemCount;
}
int UIListCtrl_GetTotalHeight(CControlUI* ctr) {
  UIListCtrl* this = (UIListCtrl*)ctr;
  return max( ( UIListCtrl_GetItemCount(ctr) * this->m_nItemHeight ) + ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 ), 1 );
}
void UIListCtrl_DrawSelect( CWindowWnd* ww, CControlUI* ctr, HDC dcPaint ) {
  UIListCtrl* this = (UIListCtrl*)ctr;
  int nHorzScroll, nVertScroll;
  SIZE sScroll;
  RECT rcGroupSelect;
  RECT rcClient;
  RECT rcSelectArea;
  RECT rcTmp;
  HDC dcBackground;
  HDC dcGroupSelect;
  HBRUSH bshSelectFrame;
  HBITMAP bmpBackground;
  HBITMAP bmpGroupSelect;
  int nBackgroundContext;
  int nGroupSelectContext;
  BLENDFUNCTION blendFunction;
  if ( !this->m_bGroupSelect )
    return ;
  sScroll = CContainerUI_GetScrollPos(ctr);
  nHorzScroll = sScroll.cx;
  nVertScroll = sScroll.cy;
  rcGroupSelect = ( this->m_rcGroupSelect );
  OffsetRect( &rcGroupSelect, -nHorzScroll, -nVertScroll );
  GetClientRect( rcClient );
  rcClient.top = ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 );
  // limit box to list client area if scrolled to limits
  if ( nHorzScroll > ( UIListCtrl_GetTotalWidth(ctr) - rcWidth(rcClient) ) )
    rcGroupSelect.right = min( rcClient.right, rcGroupSelect.right );
  if ( nHorzScroll == 0 )
    rcGroupSelect.left = max( rcClient.left, rcGroupSelect.left );
  if ( nVertScroll > ( UIListCtrl_GetTotalHeight(ctr) - rcHeight(rcClient) ) )
    rcGroupSelect.bottom = min( rcClient.bottom, rcGroupSelect.bottom );
  if ( nVertScroll == 0 )
    rcGroupSelect.top = max( rcClient.top, rcGroupSelect.top );
  // limit bitmap to client area
  rcSelectArea = ( rcGroupSelect );
  IntersectRect( &rcSelectArea, &rcSelectArea, &rcClient );
  dcBackground=CreateCompatibleDC( dcPaint );
  nBackgroundContext = SaveDC(dcBackground);
  bmpBackground=CreateCompatibleBitmap( dcPaint, rcWidth(rcSelectArea), rcHeight(rcSelectArea) );
  SelectBitmap( dcBackground, bmpBackground );
  // take a copy of existing backgroud
  BitBlt( dcBackground, 0, 0, rcWidth(rcSelectArea), rcHeight(rcSelectArea), dcPaint, rcSelectArea.left, rcSelectArea.top, SRCCOPY );
  
  dcGroupSelect=CreateCompatibleDC( dcPaint );
  nGroupSelectContext = SaveDC(dcGroupSelect);
  bmpGroupSelect=CreateCompatibleBitmap( dcPaint, rcWidth(rcSelectArea), rcHeight(rcSelectArea) );
  SelectBitmap( dcGroupSelect, bmpGroupSelect );
  // draw group select box
  SetBkColor( dcGroupSelect, ( !this->m_bShowThemed ) ? this->m_rgbItemFocus : this->m_rgbSelectedItem );
  rcTmp = iRECT( 0, 0, rcWidth(rcSelectArea), rcHeight(rcSelectArea) );
  ExtTextOut( dcGroupSelect, 0, 0, ETO_OPAQUE, &rcTmp, _T( "" ), 0, NULL );
  blendFunction.BlendOp = AC_SRC_OVER;
  blendFunction.BlendFlags = 0;
  blendFunction.SourceConstantAlpha = 180;
  blendFunction.AlphaFormat = 0;
  // blend existing background with selection box
  AlphaBlend( dcGroupSelect, 0, 0, rcWidth(rcSelectArea), rcHeight(rcSelectArea), dcBackground, 0, 0, rcWidth(rcSelectArea), rcHeight(rcSelectArea), blendFunction );
  // draw blended selection box
  BitBlt( dcPaint, rcSelectArea.left, rcSelectArea.top, rcWidth(rcSelectArea), rcHeight(rcSelectArea), dcGroupSelect, 0, 0, SRCCOPY );
  // draw selection box frame
  bshSelectFrame=CreateSolidBrush( ( !this->m_bShowThemed ) ? this->m_rgbItemText : this->m_rgbSelectedItem );
  FrameRect( dcPaint, &rcGroupSelect, bshSelectFrame );
  DeleteObject(bshSelectFrame);
  DeleteObject(bmpBackground);
  DeleteObject(bmpGroupSelect);
  RestoreDC( dcBackground, nBackgroundContext );
  RestoreDC( dcGroupSelect, nGroupSelectContext );
}
int UIListCtrl_DoPaint( CWindowWnd* ww, CControlUI* ctr, HDC hDC, RECT rcPaint ) {
  UIListCtrl* this = (UIListCtrl*)ctr;
  HDC dcPaint = hDC;
  //CContainerUI_DoPaint( ww, ctr, hDC, rcPaint );
  UIListCtrl_DrawBkgnd( ww, ctr, dcPaint, ctr->m_rcItem );
  UIListCtrl_DrawList( ww, ctr, dcPaint );
  UIListCtrl_DrawSelect( ww, ctr, dcPaint );
  UIListCtrl_DrawHeader( ww, ctr, dcPaint );
  return 0;
}

BOOL UIListCtrl_HitTest( CControlUI* ctr, POINT point, int* nItem, int* nSubItem ) {
  UIListCtrl* this = (UIListCtrl*)ctr;
  int nTotalWidth, nColumnCount, nRelativePos, nHorzScroll;
  CListInfo listInfo = {0};
  GetListInfo(listInfo);
  point.y-=ctr->m_rcItem.top;
  point.x-=ctr->m_rcItem.left;
  // are we over the header?
  if ( point.y < ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 ) )
    return FALSE;
  // calculate hit test item
  if ( this->m_nItemHeight > 0 ) {
    *nItem = UIListCtrl_GetTopItem(ctr) + ( int ) ( ( point.y - ( this->m_bShowHeader ? this->m_nHeaderHeight : 0 ) ) / this->m_nItemHeight );
  } else {
    *nItem = -1;
  }
  if ( *nItem < 0 || *nItem >= UIListCtrl_GetItemCount(ctr) )
    return FALSE;
  nTotalWidth = 0;
  nColumnCount = listInfo.nColumnCount;
  nHorzScroll = CContainerUI_GetScrollPos(ctr).cx;
  // get hit-test subitem
  for ( *nSubItem = 0; *nSubItem < nColumnCount; (*nSubItem)++ ) {
    int nColumnWidth;
    CListColumn listColumn;
    GetColumn( *nSubItem, listColumn );
    nColumnWidth = listColumn.m_nWidth;
    nTotalWidth += nColumnWidth;
    // offset position with current scroll position
    nRelativePos = nTotalWidth - nHorzScroll;
    // are we over a subitem?
    if ( point.x > nRelativePos - nColumnWidth && point.x < nRelativePos )
      return TRUE;
  }
  return FALSE;
}
BOOL UIListCtrl_HitTestHeader( CControlUI* ctr, POINT point, int* nColumn, UINT* nFlags ) {
  UIListCtrl* this = (UIListCtrl*)ctr;
  RECT rcClient;
  int nDividerPos, nColumnCount;
  // reset hittest flags
  nFlags = HITTEST_FLAG_NONE;
  if ( !this->m_bShowHeader )
    return FALSE;
  GetClientRect( rcClient );

  // are we over the header
  if ( point.y < rcClient.top || point.y > this->m_nHeaderHeight )
    return FALSE;
  nDividerPos = 0;
  nColumnCount = GetColumnCount();
  *nFlags=0;
  // get hit-test subitem
  for ( *nColumn = 0; *nColumn < nColumnCount; (*nColumn)++ ) {
    int nRelativePos, nColumnWidth, nNextColumn;
    nColumnWidth = GetColumnWidth( nColumn );
    nDividerPos += nColumnWidth;
    // offset divider position with current scroll position
    nRelativePos = nDividerPos - CContainerUI_GetScrollPos(ctr).cx;
    // are we over the divider zone?
    if ( point.x >= nRelativePos - DRAG_HEADER_OFFSET - 1 && point.x <= nRelativePos + DRAG_HEADER_OFFSET ) {
      *nFlags |= HITTEST_FLAG_HEADER_DIVIDER;
      // are we to the left of the divider (or over last column divider)?
      if ( ( point.x >= nRelativePos - DRAG_HEADER_OFFSET - 1 && point.x < nRelativePos ) || *nColumn + 1 >= nColumnCount - 1 ) {
        *nFlags |= HITTEST_FLAG_HEADER_LEFT;
        return TRUE;
      }
      // find last zero-length column after this column
      for ( nNextColumn = *nColumn + 1; nNextColumn < nColumnCount; nNextColumn++ ) {
        if ( GetColumnWidth( nNextColumn ) > 0 )
          break;
        *nColumn = nNextColumn;
      }
      *nFlags |= HITTEST_FLAG_HEADER_RIGHT;
      return TRUE;
    }
    // are we over a column?
    if ( point.x > nRelativePos - nColumnWidth && point.x < nRelativePos )
      return TRUE;
  }
  return FALSE;
}
int GetSelectedItemsCount(CControlUI* ctr) {
  UIListCtrl* this = (UIListCtrl*)ctr;
  int i, n=0;
  if (this->m_setSelectedItems) {
    for (i=0; i<this->m_setSelectedItemsSize; ++i) {
      n+=!!this->m_setSelectedItems[i];
    }
  }
  return n;
}
BOOL UIListCtrl_SelectItem( CControlUI* ctr, int nItem, int nSubItem, UINT nFlags ) {
  UIListCtrl* this = (UIListCtrl*)ctr;
  BOOL bSelectItem = TRUE;
  BOOL bSelectRange;
  BOOL bNewSelect;
  BOOL bEnsureVisible = FALSE;
  int nItemCount = UIListCtrl_GetItemCount(ctr);
  if ( nItem < 0 || nItem >= UIListCtrl_GetItemCount(ctr) )
    return FALSE;
  bSelectRange = !this->m_bSingleSelect && ( nFlags & MK_SHIFT );
  bNewSelect = !( bSelectRange || ( nFlags & MK_CONTROL ) );
  if (nItemCount!=this->m_setSelectedItemsSize) {
    DEL(this->m_setSelectedItemsSize);
    this->m_setSelectedItems = NEW(char, nItemCount);
    memset(this->m_setSelectedItems, 0, nItemCount);
    this->m_setSelectedItemsSize = nItemCount;
  }
  // are we starting a new select sequence?
  if ( bNewSelect || bSelectRange ) {
    // are we simply reselecting the same item?
    memset(this->m_setSelectedItems, 0, this->m_setSelectedItemsSize);
    if ( bSelectRange ) {
      if ( this->m_nFirstSelected == NULL_ITEM ) {
        this->m_nFirstSelected = nItem;
      }
      memset(this->m_setSelectedItems+this->m_nFirstSelected, 1, nItem-this->m_nFirstSelected+1);
    } else {
      this->m_nFirstSelected = nItem;
      this->m_setSelectedItems[nItem] = 1;
    }
  } else // we adding to or removing from select sequence
  {
    this->m_setSelectedItems[nItem] = !this->m_setSelectedItems[nItem];
  }
  // are we adding this item to the select sequence?
  if ( bSelectItem ) {
    bEnsureVisible = TRUE;
    // notify parent of selected item
    //NotifyParent( m_nFocusItem, m_nFocusSubItem, LCN_SELECTED );
  }
  // start visible timer (scrolls list to partially hidden item)
  //else if ( m_nFocusItem != NULL_ITEM && m_nFocusSubItem != NULL_SUBITEM )
  //  EditItem( m_nFocusItem, m_nFocusSubItem );
  return 0;
}
void UIListCtrl_OnLButtonDown( CControlUI* ctr, UINT nFlags, POINT point ) {
  UIListCtrl* this = (UIListCtrl*)ctr;
  int nColumn;
  int nItem, nSubItem;
  UINT nHeaderFlags;
  SIZE sz;
  this->m_bButtonDown = TRUE;
  this->m_ptDownPoint = point;
  sz = CContainerUI_GetScrollPos(ctr);
  this->m_ptSelectPoint = cPoint( point.x + sz.cx, point.y + sz.cy );
  // stop any pending scroll
  // capture all mouse input
  //SetCapture();
  nColumn = NULL_COLUMN;
  nHeaderFlags = HITTEST_FLAG_NONE;
  // are we over the header?
  if ( UIListCtrl_HitTestHeader( ctr, point, &nColumn, &nHeaderFlags ) ) {
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
      //InvalidateHeader();
    }
    return ;
  }
  nItem = NULL_ITEM;
  nSubItem = NULL_SUBITEM;
  if ( !UIListCtrl_HitTest( ctr, point, &nItem, &nSubItem ) ) {
    this->m_nFirstSelected = NULL_ITEM;
    this->m_bBeginSelect = TRUE;
  } else {
    int nIndex;
    CSubItem subItem;
    // do not begin group select from first columns
    if ( !( nFlags & MK_SHIFT ) && !( nFlags & MK_CONTROL ) && nSubItem != 0 ) {
      this->m_bBeginSelect = TRUE;
      this->m_nFirstSelected = nItem;
    }
    // only select item if not already selected
    if ( ( nFlags & MK_SHIFT ) || ( nFlags & MK_CONTROL ) || !IsSelected( nItem )// || this->m_setSelectedItems.size() <= 1 
      ) {
    }
    UIListCtrl_SelectItem( ctr, nItem, nSubItem, nFlags );
    nIndex = ( nSubItem );
    GetSubItem( nItem, nIndex, subItem );
    if ( !( subItem.m_nFlags & ITEM_FLAGS_READ_ONLY ) ) {
      int nCheckImage;
      switch ( subItem.m_nFormat ) {
      case ITEM_FORMAT_CHECKBOX:
        this->m_bBeginSelect = FALSE;
        //SetItemText( nItem, nIndex, _ttoi( pT->GetItemText( nItem, nIndex ) ) > 0 ? _T( "0" ) : _T( "1" ) );
        //NotifyParent( nItem, nSubItem, LCN_MODIFIED );
        //InvalidateItem( nItem );
        break;
      case ITEM_FORMAT_CHECKBOX_3STATE: 
        {
          this->m_bBeginSelect = FALSE;
          nCheckImage = _ttoi( subItem.m_strText );
          //if ( nCheckImage < 0 )
          //  SetItemText( nItem, nIndex, _T( "0" ) );
          //else if ( nCheckImage > 0 )
          //  SetItemText( nItem, nIndex, _T( "-1" ) );
          //else
          //  SetItemText( nItem, nIndex, _T( "1" ) );
          //NotifyParent( nItem, nSubItem, LCN_MODIFIED );
          //InvalidateItem( nItem );
        }
        break;
      case ITEM_FORMAT_HYPERLINK:
        this->m_bBeginSelect = FALSE;
        SetCursor( this->m_curHyperLink );
        //NotifyParent( nItem, nSubItem, LCN_HYPERLINK );
        break;
      }
    }
  }
}
int UIListCtrl_Event( CWindowWnd* ww, CControlUI* ctr, TEventUI* event ) {
  UIListCtrl* this = (UIListCtrl*)ctr;
  if ( 
    event->Type == UIEVENT_SETFOCUS||
    event->Type == UIEVENT_KILLFOCUS||
    event->Type == UIEVENT_BUTTONUP||
    event->Type == UIEVENT_BUTTONDOWN||
    event->Type == UIEVENT_KILLFOCUS||
    0
    ) {
    //Invalidate(ww->m_hWnd);
  }
  if ( event->Type == UIEVENT_BUTTONDOWN ) {
    UIListCtrl_OnLButtonDown(ctr, event->wParam, event->ptMouse);
  }
  CContainerUI_Event( ww, ctr, event );
  return 0;
}
