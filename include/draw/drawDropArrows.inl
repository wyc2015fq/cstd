#define NULL_COLUMN -1
#define NULL_ITEM -1
#define NULL_SUBITEM -1
#define ITEM_HEIGHT_MARGIN 6
#define ITEM_WIDTH_MARGIN 20
#define ITEM_SCROLL_OFFSET 5
#define DRAG_HEADER_OFFSET 4
#define DRAG_ITEM_OFFSET 3
#define ITEM_EDIT_MARGIN 5
#define TOOLTIP_TOOL_ID 1000
#define RESIZE_COLUMN_TIMER 1001
#define RESIZE_COLUMN_PERIOD 20
#define ITEM_VISIBLE_TIMER 1002
#define ITEM_VISIBLE_PERIOD 250
#define SEARCH_PERIOD 800
#define ITEM_AUTOSCROLL_TIMER 1003
#define ITEM_SCROLL_TIMER 1004
#define ITEM_SCROLL_PERIOD 20
#define ITEM_SCROLL_UNIT_MAX 8
#define ITEM_SCROLL_UNIT_MIN 4
#define ITEM_FORMAT_NONE 0
#define ITEM_FORMAT_EDIT 1
#define ITEM_FORMAT_DATETIME 2
#define ITEM_FORMAT_COMBO 3
#define ITEM_FORMAT_CHECKBOX 4
#define ITEM_FORMAT_CHECKBOX_3STATE 5
#define ITEM_FORMAT_HYPERLINK 6
#define ITEM_FORMAT_PROGRESS 7
#define ITEM_FORMAT_CUSTOM 8
#define ITEM_FLAGS_NONE 0x0000
#define ITEM_FLAGS_LEFT 0x0001
#define ITEM_FLAGS_RIGHT 0x0002
#define ITEM_FLAGS_CENTRE 0x0004
#define ITEM_FLAGS_READ_ONLY 0x0008
#define ITEM_FLAGS_EDIT_UPPER 0x0010
#define ITEM_FLAGS_EDIT_NUMBER 0x0020
#define ITEM_FLAGS_EDIT_FLOAT 0x0040
#define ITEM_FLAGS_EDIT_NEGATIVE 0x0080
#define ITEM_FLAGS_EDIT_OPERATOR 0x0100
#define ITEM_FLAGS_COMBO_EDIT 0x0200
#define ITEM_FLAGS_DATE_ONLY 0x0400
#define ITEM_FLAGS_TIME_ONLY 0x0800
#define ITEM_FLAGS_DATETIME_NONE 0x1000
#define ITEM_FLAGS_PROGRESS_SOLID 0x2000
#define ITEM_IMAGE_NONE -1
#define ITEM_IMAGE_DOWN 0
#define ITEM_IMAGE_UP 1
#define ITEM_IMAGE_CHECK_OFF 2
#define ITEM_IMAGE_CHECK_ON 3
#define ITEM_IMAGE_3STATE_UNDEF ITEM_IMAGE_CHECK_OFF
#define ITEM_IMAGE_3STATE_ON 4
#define ITEM_IMAGE_3STATE_OFF 5
#define ITEM_IMAGE_LOCK 6
#define ITEM_IMAGE_ATTACHMENT 7
#define ITEM_IMAGE_3STATE 8
#define ITEM_IMAGE_CHECKBOX 9
#define HITTEST_FLAG_NONE 0x0000
#define HITTEST_FLAG_HEADER_DIVIDER 0x0001
#define HITTEST_FLAG_HEADER_LEFT 0x0002
#define HITTEST_FLAG_HEADER_RIGHT 0x0004
#define LCN_FIRST ( 0U - 1500U )
#define LCN_LAST ( 0U - 1600U )
#define LCN_SELECTED ( LCN_FIRST - 1 )
#define LCN_LEFTCLICK ( LCN_FIRST - 2 )
#define LCN_RIGHTCLICK ( LCN_FIRST - 3 )
#define LCN_DBLCLICK ( LCN_FIRST - 4 )
#define LCN_ENDEDIT ( LCN_FIRST - 5 )
#define LCN_MODIFIED ( LCN_FIRST - 6 )
#define LCN_HYPERLINK ( LCN_FIRST - 7 )
int DrawDropArrows(HDDC hDC, int nSpanLength, BOOL bVertical, COLORREF clr)
{
  POINT ptArrow[ 7 ];
  ptArrow[0].x = bVertical ? 8 : 0;
  ptArrow[0].y = bVertical ? 0 : 9;
  ptArrow[1].x = bVertical ? 8 : 4;
  ptArrow[1].y = bVertical ? 4 : 9;
  ptArrow[2].x = bVertical ? 11 : 4;
  ptArrow[2].y = bVertical ? 4 : 12;
  ptArrow[3].x = bVertical ? 6 : 10;
  ptArrow[3].y = bVertical ? 9 : 6;
  ptArrow[4].x = bVertical ? 1 : 4;
  ptArrow[4].y = bVertical ? 4 : 0;
  ptArrow[5].x = bVertical ? 4 : 4;
  ptArrow[5].y = bVertical ? 4 : 4;
  ptArrow[6].x = bVertical ? 4 : 0;
  ptArrow[6].y = bVertical ? 0 : 4;
  ScanFill(hDC, ptArrow, 7, clr);
  ptArrow[0].x = bVertical ? 4 : nSpanLength;
  ptArrow[0].y = bVertical ? nSpanLength : 4;
  ptArrow[1].x = bVertical ? 4 : nSpanLength - 4;
  ptArrow[1].y = bVertical ? nSpanLength - 4 : 4;
  ptArrow[2].x = bVertical ? 0 : nSpanLength - 4;
  ptArrow[2].y = bVertical ? nSpanLength - 4 : 0;
  ptArrow[3].x = bVertical ? 6 : nSpanLength - 10;
  ptArrow[3].y = bVertical ? nSpanLength - 10 : 6;
  ptArrow[4].x = bVertical ? 12 : nSpanLength - 4;
  ptArrow[4].y = bVertical ? nSpanLength - 4 : 12;
  ptArrow[5].x = bVertical ? 8 : nSpanLength - 4;
  ptArrow[5].y = bVertical ? nSpanLength - 4 : 9;
  ptArrow[6].x = bVertical ? 8 : nSpanLength;
  ptArrow[6].y = bVertical ? nSpanLength : 9;
  ScanFill(hDC, ptArrow, 7, clr);
  return 0;
}
BOOL m_bShowHeader;
BOOL m_bShowSort;
BOOL m_bSortAscending;
BOOL m_bButtonDown;
BOOL m_bMouseOver;
BOOL m_bColumnSizing;
BOOL m_bBeginSelect;
BOOL m_bSingleSelect;
BOOL m_bFocusSubItem;
BOOL m_bGroupSelect;
BOOL m_bShowHorizScroll;
BOOL m_bShowVertScroll;
BOOL m_bEnableHorizScroll;
BOOL m_bEnableVertScroll;
BOOL m_bResizeTimer;
BOOL m_bDragDrop;
BOOL m_bSmoothScroll;
BOOL m_bEditItem;
BOOL m_bScrolling;
BOOL m_bScrollDown;
BOOL m_bTileBackground;
POINT m_ptDownPoint;
POINT m_ptSelectPoint;
RECT m_rcGroupSelect;
int m_nItemHeight;
int m_nHeaderHeight;
int m_nFirstSelected;
int m_nFocusItem;
int m_nFocusSubItem;
int m_nHotItem;
int m_nHotSubItem;
int m_nTitleTipItem;
int m_nTitleTipSubItem;
int m_nMouseWheelScroll;
int m_nTotalWidth;
int m_nSortColumn;
int m_nDragColumn;
int m_nHighlightColumn;
int m_nHotColumn;
int m_nHotDivider;
int m_nColumnSizing;
int m_nScrollOffset;
int m_nScrollDelta;
int m_nScrollUnit;
int m_nStartScrollPos;
int m_nStartSize;
int m_nStartPos;
DWORD m_dwSearchTick;
DWORD m_dwScrollTick;
TCHAR* m_strSearchString;
Image* m_bmpScrollList;
Image* m_bmpBackground;
uint m_nHeaderClipboardFormat;
COLORREF m_rgbBackground;
COLORREF m_rgbHeaderBackground;
COLORREF m_rgbHeaderBorder;
COLORREF m_rgbHeaderShadow;
COLORREF m_rgbHeaderText;
COLORREF m_rgbHeaderHighlight;
COLORREF m_rgbSelectedItem;
COLORREF m_rgbSelectedText;
COLORREF m_rgbItemText;
COLORREF m_rgbSelectOuter;
COLORREF m_rgbSelectInner;
COLORREF m_rgbSelectTop;
COLORREF m_rgbSelectBottom;
COLORREF m_rgbNoFocusTop;
COLORREF m_rgbNoFocusBottom;
COLORREF m_rgbNoFocusOuter;
COLORREF m_rgbNoFocusInner;
COLORREF m_rgbFocusTop;
COLORREF m_rgbFocusBottom;
COLORREF m_rgbProgressTop;
COLORREF m_rgbProgressBottom;
COLORREF m_rgbItemFocus;
COLORREF m_rgbHyperLink;
HFONT m_fntListFont;
HFONT m_fntUnderlineFont;
Image m_curDivider[1] = {0};
Image m_curHyperLink[1] = {0};
Image m_ilListItems[1] = {0};
Image m_ilItemImages[1] = {0};
CListColumn* m_aColumns;
int m_setSelectedItems[100];
SIZE GetTextExtent(LPCTSTR lpszString, int nCount)
{
  HDC m_hAttribDC = GetMemDC();
  SIZE size;
  VERIFY(GetTextExtentPoint32(m_hAttribDC, lpszString, nCount, &size));
  return size;
}
#define SETTRANSPARENTCOLOR(_P, _A) ((_P)|=((_A)<<24))
int SetTransparentColor(Image* im, COLORREF clr)
{
  int n = im->h * im->w, i;
  DWORD* p = (DWORD*)(im + 1);

  for (i = 0; i < n; ++i) {
    if (GetRGBV(p[i]) == GetRGBV(clr)) {
      SETTRANSPARENTCOLOR(p[i], 0xFF);
    }
  }

  return 0;
}
BOOL LoadSettings()
{
  SIZE sizeExtent;
  m_rgbBackground = _GetSysColor(COLOR_WINDOW);
  m_rgbHeaderBackground = _GetSysColor(COLOR_BTNFACE);
  m_rgbHeaderBorder = _GetSysColor(COLOR_3DHIGHLIGHT);
  m_rgbHeaderShadow = _GetSysColor(COLOR_3DSHADOW);
  m_rgbHeaderText = _GetSysColor(COLOR_WINDOWTEXT);
  m_rgbHeaderHighlight = _RGB(130, 140, 180);
  m_rgbSelectedItem = _GetSysColor(COLOR_HIGHLIGHT);
  m_rgbSelectedText = _GetSysColor(COLOR_HIGHLIGHTTEXT);
  m_rgbItemText = _GetSysColor(COLOR_WINDOWTEXT);
  m_rgbSelectOuter = _RGB(170, 200, 245);
  m_rgbSelectInner = _RGB(230, 250, 250);
  m_rgbSelectTop = _RGB(210, 240, 250);
  m_rgbSelectBottom = _RGB(185, 215, 250);
  m_rgbNoFocusTop = _RGB(250, 250, 250);
  m_rgbNoFocusBottom = _RGB(235, 235, 235);
  m_rgbNoFocusOuter = _RGB(220, 220, 220);
  m_rgbNoFocusInner = _RGB(245, 245, 245);
  m_rgbFocusTop = _RGB(235, 245, 245);
  m_rgbFocusBottom = _RGB(225, 235, 245);
  m_rgbProgressTop = _RGB(170, 240, 170);
  m_rgbProgressBottom = _RGB(45, 210, 50);
  m_rgbItemFocus = _RGB(180, 190, 210);
  m_rgbHyperLink = _RGB(0, 0, 200);
  m_nHeaderClipboardFormat = RegisterClipboardFormat(_T("HEADERCLIPBOARDFORMAT"));
  // get number of lines to scroll
  //SystemParametersInfo( SPI_GETWHEELSCROLLLINES, 0, &m_nMouseWheelScroll, 0 );
  sizeExtent = GetTextExtent(_T("Height"), 6);

  // has system font changed
  if (m_nItemHeight != sizeExtent.h + ITEM_HEIGHT_MARGIN) {
    m_nItemHeight = sizeExtent.h + ITEM_HEIGHT_MARGIN;
    m_nHeaderHeight = m_nItemHeight;
  }

  return TRUE;
}
void InitCListImpl()
{
  m_bShowHeader = TRUE;
  m_bSortAscending = TRUE;
  m_bButtonDown = FALSE;
  m_bMouseOver = FALSE;
  m_bColumnSizing = FALSE;
  m_bBeginSelect = FALSE;
  m_bSingleSelect = FALSE;
  m_bFocusSubItem = FALSE;
  m_bGroupSelect = FALSE;
  m_bEnableHorizScroll = FALSE;
  m_bEnableVertScroll = FALSE;
  m_bShowHorizScroll = TRUE;
  m_bShowVertScroll = TRUE;
  m_bShowSort = TRUE;
  m_bResizeTimer = FALSE;
  m_bDragDrop = FALSE;
  m_bSmoothScroll = TRUE;
  m_bEditItem = FALSE;
  m_bScrolling = FALSE;
  m_bScrollDown = FALSE;
  m_bTileBackground = FALSE;
  m_nMouseWheelScroll = 3;
  m_nTotalWidth = 0;
  m_nHeaderHeight = 20;
  m_nItemHeight = 0;
  m_nFirstSelected = NULL_ITEM;
  m_nFocusItem = NULL_ITEM;
  m_nFocusSubItem = NULL_SUBITEM;
  m_nHotItem = NULL_ITEM;
  m_nHotSubItem = NULL_SUBITEM;
  m_nTitleTipItem = NULL_ITEM;
  m_nTitleTipSubItem = NULL_SUBITEM;
  m_nSortColumn = NULL_COLUMN;
  m_nHighlightColumn = NULL_COLUMN;
  m_nDragColumn = NULL_COLUMN;
  m_nHotColumn = NULL_COLUMN;
  m_nHotDivider = NULL_COLUMN;
  m_nColumnSizing = NULL_COLUMN;
  m_nScrollOffset = 0;
  m_nScrollDelta = 0;
  m_nScrollUnit = 0;
  m_nStartScrollPos = 0;
  m_nStartSize = 0;
  m_nStartPos = 0;
  m_ptDownPoint = iPOINT(0, 0);
  m_ptSelectPoint = m_ptDownPoint;
  //m_rcGroupSelect = 0;
  m_dwSearchTick = 0;
  m_dwScrollTick = 0;
  m_strSearchString = _T("");
  SelectFontToMemDC("ËÎÌו", 12, 0, 0);
  // load interface settings
  LoadSettings();
}
int DrawHeader(HDDC dc, CListColumn* listCol, int nColumnCount)
{
  int nColumn, nHeaderWidth;
  RECT rcHeader = {0, 0, dc->w, m_nHeaderHeight};
  RECT rcClip = rcHeader;
  RECT rcHeaderItem;
  COLORREF penHighlight;
  COLORREF penShadow;
  BOOL bShowArrow;
  int _x0, _y0;
  uint nFormat;
  rcHeader.bottom = m_nHeaderHeight;

  if (rcClip.top > rcHeader.bottom) {
    return 0;
  }

  TFillRect(dc, &rcHeader, m_rgbHeaderBackground);
  penHighlight = (m_rgbHeaderBorder);
  penShadow = (m_rgbHeaderShadow);
  rcHeaderItem = rcHeader;
  nHeaderWidth = 0;

  for (nColumn = 0; nColumn < nColumnCount; rcHeaderItem.left = rcHeaderItem.right, nColumn++) {
    RECT rcHeaderText;
    CListColumn* listColumn = listCol + nColumn;
    rcHeaderItem.right = rcHeaderItem.left + listColumn->m_nWidth;
    nHeaderWidth += RCWIDTH(&rcHeaderItem);

    if (rcHeaderItem.right < rcClip.left) {
      continue;
    }

    if (rcHeaderItem.left > rcClip.right) {
      break;
    }

    // draw header and divider
    {
      if (nColumn == m_nHighlightColumn) {
        TFillRect(dc, &rcHeaderItem, m_rgbHeaderHighlight);
      }

      TMoveTo(dc, rcHeaderItem.right - 1, rcHeaderItem.top + 4);
      TLineTo(dc, rcHeaderItem.right - 1, m_nHeaderHeight - 4, penShadow);
      TMoveTo(dc, rcHeaderItem.right, rcHeaderItem.top + 4);
      TLineTo(dc, rcHeaderItem.right, m_nHeaderHeight - 4, penHighlight);
    }
    rcHeaderText = (rcHeaderItem);
    rcHeaderText.left += nColumn == 0 ? 4 : 3;
    OffsetRect(&rcHeaderText, 0, 1);
    bShowArrow = m_bShowSort && (RCWIDTH(&rcHeaderItem) > 15);

    // offset text bounding rectangle to account for sorting arrow
    if (bShowArrow && !listColumn->m_bFixed && nColumn == m_nSortColumn) {
      rcHeaderText.right -= 15;
    }

    // margin header text
    DeflateRect4(&rcHeaderText, 4, 0, 5, 0);

    // has this header item an associated image?
    if (listColumn->m_nImage != ITEM_IMAGE_NONE && m_ilListItems) {
      SIZE sizeIcon = iSIZE(m_ilListItems->h, m_ilListItems->h);
      RECT rcHeaderImage;
      rcHeaderImage.left = !listColumn->m_strText ? ((rcHeaderText.left + rcHeaderText.right) / 2) - (sizeIcon.w / 2) : rcHeaderText.left;
      rcHeaderImage.right = MIN(rcHeaderImage.left + sizeIcon.w, rcHeaderItem.right - 2);
      rcHeaderImage.top = ((rcHeaderItem.top + rcHeaderItem.bottom) / 2) - (sizeIcon.h / 2);
      rcHeaderImage.bottom = MIN(rcHeaderImage.top + sizeIcon.h, rcHeaderItem.bottom);
      TDrawImageList(dc, rcHeaderImage.left, rcHeaderImage.top, m_ilListItems, listColumn->m_nImage);
      // offset header text (for image)
      rcHeaderText.left += sizeIcon.w + 4;
    }

    //SelectFont( dc, m_fntListFont );
    //SetTextColor( dc, m_rgbHeaderText );
    nFormat = DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS;

    if (listColumn->m_nFlags & ITEM_FLAGS_CENTRE) {
      nFormat |= DT_CENTER;
    }
    else if (listColumn->m_nFlags & ITEM_FLAGS_RIGHT) {
      nFormat |= DT_RIGHT;
    }
    else {
      nFormat |= DT_LEFT;
    }

    // draw header text
    if (!IsRectEmpty(&rcHeaderText) && listColumn->m_strText) {
      TDrawText(dc, listColumn->m_strText, &rcHeaderText, nFormat);
    }

    // draw sorting arrow
    if (bShowArrow && !listColumn->m_bFixed && nColumn == m_nSortColumn) {
      SIZE sizeIcon = iSIZE(m_ilListItems->h, m_ilListItems->h);
      RECT rcSortArrow;
      rcSortArrow.left = rcHeaderText.right + 4;
      rcSortArrow.right = MIN(rcSortArrow.left + sizeIcon.w, rcHeaderItem.right);
      rcSortArrow.top = RCHEIGHT(&rcHeaderItem) / 2 - 3;
      rcSortArrow.bottom = MIN(rcSortArrow.top + sizeIcon.h, rcHeaderItem.bottom);
      TDrawImageList(dc, rcSortArrow.left, rcSortArrow.top, m_ilListItems, m_bSortAscending ? ITEM_IMAGE_UP : ITEM_IMAGE_DOWN);
    }
  }

  // draw a frame around all header columns
  if (nHeaderWidth > 0) {
    RECT rc = iRECT(rcHeader.left, rcHeader.top, rcHeader.right + 2, rcHeader.bottom);
    TDraw3DRect(dc, &rc, m_rgbHeaderBorder, m_rgbHeaderShadow);
  }

  return 0;
}
void DrawItem(HDDC dc, int nItem, RECT* rcItem, CListColumn* listCol, int nColumnCount, CSubItem* items)
{
  RECT rcClip = {0, 0, dc->w, dc->h};
  int nFocusItem = NULL_ITEM;
  int nFocusSubItem = NULL_SUBITEM;
  //GetFocusItem( nFocusItem, nFocusSubItem );
  int nSubItem;
  BOOL bSelectedItem = 0;
  BOOL bControlFocus = 1;
  RECT rcSubItem;

  // draw selected background
  if (bSelectedItem) {
    if (0) {
      // draw select border
      //DrawRoundRect( dc, rcItem, bControlFocus ? m_rgbSelectOuter : m_rgbNoFocusOuter, bControlFocus ? m_rgbSelectInner : m_rgbNoFocusInner );
      RECT rcSelect = (*rcItem);
      DeflateRect(&rcSelect, 2, 2);
      // fill selected area
      //DrawGradient( dc, rcSelect, bControlFocus ? m_rgbSelectTop : m_rgbNoFocusTop, bControlFocus ? m_rgbSelectBottom : m_rgbNoFocusBottom );
      DrawGradient(dc, &rcSelect, bControlFocus ? m_rgbSelectTop : m_rgbNoFocusTop, bControlFocus ? m_rgbSelectBottom : m_rgbNoFocusBottom, GRADIENT_VERTICAL, TRANSFORMATION_NONE);
    }
    else {
      TFillRect(dc, rcItem, m_rgbSelectedItem);
    }
  }

  if (bControlFocus) {
    TFillRect(dc, rcItem, m_rgbItemFocus);
  }

  rcSubItem = (*rcItem);
  rcSubItem.right = rcSubItem.left;

  for (nSubItem = 0; nSubItem < nColumnCount; rcSubItem.left = rcSubItem.right + 1, nSubItem++) {
    CListColumn* listColumn = listCol + nSubItem;
    CSubItem* pT = items + nSubItem;
    LPCTSTR strItemText = pT->m_strText;
    int nItemImage = pT->m_nImage;
    uint nItemFormat = pT->m_nFormat;
    uint nItemFlags = pT->m_nFlags;
    BOOL bFocusSubItem;
    COLORREF rgbBackground, rgbText;
    RECT rcItemText;
    uint nFormat;
    rcSubItem.right = rcSubItem.left + listColumn->m_nWidth - 1;

    if (rcSubItem.right < rcClip.left || RCWIDTH(&rcSubItem) == 0) {
      continue;
    }

    if (rcSubItem.left > rcClip.right) {
      break;
    }

    // custom draw subitem format
    if (nItemFormat == ITEM_FORMAT_CUSTOM) {
      ASSERT(FALSE); // must be implemented in a derived struct
      //pT->DrawCustomItem( dc, nItem, nSubItem, rcSubItem );
      return;
    }

    bFocusSubItem = (m_bFocusSubItem && nFocusItem == nItem && nFocusSubItem == nSubItem);
    rgbBackground = m_rgbBackground;
    rgbText = m_rgbItemText;

    if (bFocusSubItem) {
      if (0) {
        // only draw subitem focus if control has focus
        if (bControlFocus) {
          RECT rcSelect;
          // draw select border
          //TRoundRect( dc, &rcSubItem, 0, 0, bControlFocus ? m_rgbSelectOuter : m_rgbNoFocusOuter, bControlFocus ? m_rgbSelectInner : m_rgbNoFocusInner );
          TRoundRect(dc, &rcSubItem, 0, 0, bControlFocus ? m_rgbSelectOuter : m_rgbNoFocusOuter);
          rcSelect = (rcSubItem);
          DeflateRect(&rcSelect, 2, 2);

          // fill selected area
          if (!m_bEditItem) {
            DrawGradient(dc, &rcSelect, m_rgbFocusTop, m_rgbFocusBottom, GRADIENT_VERTICAL, TRANSFORMATION_NONE);
          }
          else {
            TFillRect(dc, &rcSelect, m_rgbBackground);
          }
        }
      }
      else {
        TFillRect(dc, &rcSubItem, m_bEditItem ? m_rgbBackground : m_rgbItemFocus);

        if (m_bEditItem) {
          TFillRect(dc, &rcSubItem, m_rgbItemFocus);
        }
      }
    }
    else if (pT->m_rgbBackground != m_rgbBackground) {
      //TRoundRect( dc, &rcSubItem, 3, 3, pT->m_rgbBackground );
      TDrawRect(dc, &rcSubItem, pT->m_rgbBackground);
    }

    rcItemText = (rcSubItem);
    // margin item text
    rcItemText.left += nSubItem == 0 ? 4 : 3;
    DeflateRect(&rcItemText, 4, 0);

    // draw subitem image if supplied
    if (m_ilItemImages && nItemImage != ITEM_IMAGE_NONE && (!m_bEditItem || (m_bEditItem && !bFocusSubItem))) {
      SIZE sizeIcon = iSIZE(m_ilItemImages->h, m_ilItemImages->h);
      RECT rcItemImage;
      rcItemImage.left = !strItemText ? ((rcItemText.left + rcItemText.right) / 2) - (sizeIcon.w / 2) : rcItemText.left;
      rcItemImage.right = MIN(rcItemImage.left + sizeIcon.w, rcSubItem.right);
      rcItemImage.top = ((rcSubItem.top + rcSubItem.bottom) / 2) - (sizeIcon.h / 2);
      rcItemImage.bottom = MIN(rcItemImage.top + sizeIcon.h, rcSubItem.bottom);
      TDrawImageList(dc, rcItemImage.left, rcItemImage.top, m_ilItemImages, nItemImage);
      // offset item text (for image)
      rcItemText.left += sizeIcon.w + 4;
    }

    if (IsRectEmpty(&rcItemText)) {
      continue;
    }

    rgbText = ((bSelectedItem && !bFocusSubItem) ? m_rgbSelectedText : rgbText);
    nFormat = DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS;

    if (nItemFlags & ITEM_FLAGS_CENTRE) {
      nFormat |= DT_CENTER;
    }
    else if (nItemFlags & ITEM_FLAGS_RIGHT) {
      nFormat |= DT_RIGHT;
    }
    else {
      nFormat |= DT_LEFT;
    }

    switch (nItemFormat) {
    case ITEM_FORMAT_DATETIME:
      if (strItemText) {
        TCHAR strItemDate[256];
        SYSTEMTIME stItemDate = pT->m_stItemDate;

        if (nItemFlags & ITEM_FLAGS_DATE_ONLY) {
          GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &stItemDate, NULL, strItemDate, 256);
        }
        else if (nItemFlags & ITEM_FLAGS_TIME_ONLY) {
          stItemDate.wYear = 0;
          stItemDate.wMonth = 0;
          stItemDate.wDay = 0;
          GetDateFormat(LOCALE_USER_DEFAULT, 0, &stItemDate, NULL, strItemDate, 256);
        }
        else {
          GetDateFormat(LOCALE_USER_DEFAULT, 0, &stItemDate, NULL, strItemDate, 256);
        }

        TDrawText(dc, strItemDate, &rcItemText, nFormat);
      }

      break;

    case ITEM_FORMAT_CHECKBOX:
    case ITEM_FORMAT_CHECKBOX_3STATE: {
      SIZE sizeIcon = iSIZE(m_ilListItems->h, m_ilListItems->h);
      int nCheckValue = pT->m_nCheckValue;
      RECT rcCheckBox;
      rcCheckBox.left = ((rcItemText.left + rcItemText.right) / 2) - (sizeIcon.w / 2) - 1;
      rcCheckBox.right = MIN(rcCheckBox.left + sizeIcon.w, rcSubItem.right);
      rcCheckBox.top = ((rcSubItem.top + rcSubItem.bottom) / 2) - (sizeIcon.h / 2);
      rcCheckBox.bottom = MIN(rcCheckBox.top + sizeIcon.h, rcSubItem.bottom);

      if (nItemFormat == ITEM_FORMAT_CHECKBOX) {
        TDrawImageList(dc, rcCheckBox.left, rcCheckBox.top, m_ilListItems, nCheckValue > 0 ? ITEM_IMAGE_CHECK_ON : ITEM_IMAGE_CHECK_OFF);
      }
      else {
        int nCheckImage = ITEM_IMAGE_3STATE_UNDEF;

        if (nCheckValue < 0) {
          nCheckImage = ITEM_IMAGE_3STATE_OFF;
        }
        else if (nCheckValue > 0) {
          nCheckImage = ITEM_IMAGE_3STATE_ON;
        }

        TDrawImageList(dc, rcCheckBox.left, rcCheckBox.top, m_ilListItems, nCheckImage);
      }
    }
    break;

    case ITEM_FORMAT_PROGRESS: {
      int iProgress = pT->m_nProgress;
      RECT rcProgress = (rcSubItem);
      DeflateRect(&rcProgress, 3, 2);
      // draw progress border
      //TDrawRoundRect( dc, &rcProgress, m_rgbHeaderShadow, m_rgbHeaderBackground );
      //TRoundRect( dc, &rcProgress, 0, 0, m_rgbHeaderBackground );
      TFillRect(dc, &rcProgress, m_rgbHeaderBackground);
      // fill progress bar area
      DeflateRect(&rcProgress, 3, 3);
      rcProgress.right = rcProgress.left + (int)((double)RCWIDTH(&rcProgress) * ((MAX(MIN(iProgress, 100), 0)) / 100.0));
      DrawGradient(dc, &rcProgress, m_rgbProgressTop, m_rgbProgressBottom, GRADIENT_VERTICAL, TRANSFORMATION_NONE);
    }
    break;

    case ITEM_FORMAT_HYPERLINK:
      if (nItem == m_nHotItem && nSubItem == m_nHotSubItem && !(nItemFlags & ITEM_FLAGS_READ_ONLY)) {
        //dc.SelectFont( m_fntUnderlineFont );
        //dc.SetTextColor( m_rgbHyperLink );
      }

    default:

      // draw item text
      if (strItemText) {
        TDrawText(dc, strItemText, &rcItemText, nFormat);
      }

      break;
    }
  }
}
int DrawList(HDDC dc, CListColumn* listCol, int nColumnCount, CSubItem* items, int nItemCount)
{
  RECT rcClip = {0, 0, dc->w, dc->h};
  RECT rcItem;
  int nTopItem = 0;
  int nItem;
  rcItem.left = 0;
  rcItem.right = dc->w;
  rcItem.top = (m_bShowHeader ? m_nHeaderHeight : 0);
  rcItem.bottom = rcItem.top;
  TFillRect(dc, &rcClip, m_rgbBackground);

  if (m_bShowHeader) {
    DrawHeader(dc, listCol, nColumnCount);
  }

  // draw all visible items
  for (nItem = nTopItem; nItem < nItemCount; rcItem.top = rcItem.bottom, nItem++) {
    rcItem.bottom = rcItem.top + m_nItemHeight;

    if (rcItem.bottom < rcClip.top || rcItem.right < rcClip.left) {
      continue;
    }

    if (rcItem.top > rcClip.bottom || rcItem.left > rcClip.right) {
      break;
    }

    // may be implemented in a derived struct
    DrawItem(dc, nItem, &rcItem, listCol, nColumnCount, items);
    items += nColumnCount;
  }

  return 0;
}
void SetColumn(CListColumn* c, LPCTSTR lpszText, int nWidth, int nImage, BOOL bFixed, uint nFormat, uint nFlags)
{
  c->m_strText = lpszText;
  c->m_nWidth = nWidth;
  c->m_bFixed = bFixed;
  c->m_nFormat = nFormat;
  c->m_nFlags = nFlags;
  c->m_nImage = nImage;
}
uint ValidateFlags(uint nFlags)
{
  if (nFlags & ITEM_FLAGS_CENTRE) {
    nFlags &= ~(ITEM_FLAGS_LEFT | ITEM_FLAGS_RIGHT);
  }

  if (nFlags & ITEM_FLAGS_RIGHT) {
    nFlags &= ~ITEM_FLAGS_LEFT;
  }

  if (nFlags & ITEM_FLAGS_DATE_ONLY) {
    nFlags &= ~ITEM_FLAGS_TIME_ONLY;
  }

  if (nFlags & (ITEM_FLAGS_EDIT_NUMBER | ITEM_FLAGS_EDIT_FLOAT)) {
    nFlags &= ~ITEM_FLAGS_EDIT_UPPER;
  }

  if (!(nFlags & (ITEM_FLAGS_EDIT_NUMBER | ITEM_FLAGS_EDIT_FLOAT))) {
    nFlags &= ~(ITEM_FLAGS_EDIT_NEGATIVE | ITEM_FLAGS_EDIT_OPERATOR);
  }

  if (nFlags & ITEM_FLAGS_COMBO_EDIT) {
    nFlags &= ~(ITEM_FLAGS_DATE_ONLY | ITEM_FLAGS_TIME_ONLY | ITEM_FLAGS_DATETIME_NONE);
  }

  return nFlags;
}
int SetItem(CSubItem* listSubItem, LPCTSTR lpszText, int nImage, uint nFormat, uint nFlags)
{
  listSubItem->m_nImage = ITEM_IMAGE_NONE;
  listSubItem->m_nFormat = nFormat;
  listSubItem->m_nFlags = ValidateFlags(nFlags);
  //listSubItem->m_hFont = NULL;
  listSubItem->m_rgbBackground = m_rgbBackground;
  listSubItem->m_rgbText = m_rgbItemText;
  // set item details for first subitem
  listSubItem->m_strText = lpszText;
  listSubItem->m_nImage = nImage;
  return 0;
}
int test_DrawHeader(HDDC hDC)
{
  CListColumn listCol[10];
  int nCols = 0;
  CSubItem items0[1000];
  char buf[102400];
  char* p = buf;
  int i = 0, j = 0;
  CSubItem* items = items0;
  InitCListImpl();
  SetColumn(listCol + i++, _T("Column 1"), 150, -1, 0, 0, 0);
  SetColumn(listCol + i++, _T(""), 30, ITEM_IMAGE_3STATE, TRUE, ITEM_FORMAT_CHECKBOX_3STATE, 0);
  SetColumn(listCol + i++, _T("Column 2"), 130, -1, FALSE, 0, 0);
  SetColumn(listCol + i++, _T("Column 3"), 80, -1, FALSE, 0, ITEM_FLAGS_RIGHT);
  SetColumn(listCol + i++, _T("Column 4"), 120, -1, FALSE, 0, 0);
  SetColumn(listCol + i++, _T("Column 5"), 170, ITEM_IMAGE_ATTACHMENT, FALSE, ITEM_FORMAT_PROGRESS, 0);
  nCols = i;
  srand(12345);

  for (j = 0; j < 100; j++) {
    i = 0;
    _snprintf(p, 256, _T("Item %d"), j + 1);
    SetItem(items + i, p, rand() % 6, listCol[i].m_nFormat, listCol[i].m_nFlags);
    ++i;
    items[i].m_nCheckValue = (rand() % 3) - 1;
    SetItem(items + i, p, -1, listCol[i].m_nFormat, listCol[i].m_nFlags);
    ++i;
    p += strlen(p) + 1;
    _snprintf(p, 256, _T("%c Random Text"), _T('A') + (rand() % 26));
    SetItem(items + i, p, rand() % 6, listCol[i].m_nFormat, listCol[i].m_nFlags);
    ++i;
    p += strlen(p) + 1;
    _snprintf(p, 256, _T("%d"), j);
    SetItem(items + i, p, -1, listCol[i].m_nFormat, listCol[i].m_nFlags);
    ++i;
    SetItem(items + i, 0, -1, listCol[i].m_nFormat, listCol[i].m_nFlags);
    ++i;
    items[i].m_nProgress = (rand() % 101);
    SetItem(items + i, 0, -1, listCol[i].m_nFormat, listCol[i].m_nFlags);
    ++i;
    ASSERT(i == nCols);
    items += nCols;
  }

  m_bShowHeader = 1;
  //_RGB( 255, 0, 255 )
#define IDB_EXAMPLE "D:/code/c/list/ListCtrlDemo/res/ExampleItems.bmp"
#define IDB_LISTITEMS "D:/code/c/list/ListCtrlDemo/res/ListItems.bmp"
#define IDC_DIVIDER "D:/code/c/list/ListCtrlDemo/res/divider.cur"
#define IDC_HYPERLINK "D:/code/c/list/ListCtrlDemo/res/hand.cur"
#define TRANSCOLOR _RGB( 255, 0, 255 )
  // load list images
  loadbmpfile(IDB_LISTITEMS, 32, 1, m_ilListItems);
  loadbmpfile(IDB_EXAMPLE, 32, 1, m_ilItemImages);
  //m_curDivider = loadbmpfile(IDC_DIVIDER, 32, 1);
  //m_curHyperLink = loadbmpfile(IDC_HYPERLINK, 32, 1);
  SetTransparentColor(m_ilListItems, TRANSCOLOR);
  SetTransparentColor(m_ilItemImages, TRANSCOLOR);
  DrawList(hDC, listCol, nCols, items0, 1000);
  free(m_ilListItems);
  free(m_ilItemImages);
  return 0;
}

