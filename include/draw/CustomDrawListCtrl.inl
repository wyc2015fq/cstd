
// CCustomDrawListCtrl
BOOL IsFocus()
{
  return 0;
}
BOOL IsSortable()
{
  return 0;
}

int GetSubItemRect(hditem_t* lpLVItem, UINT nArea, RECT& rcItem)
{
  if (LVIR_BOUNDS == nArea) {
    rcItem = lpLVItem->rc;
  }
  else if (LVIR_ICON == nArea) {
    rcItem = lpLVItem->rc;
    rcItem.right = rcItem.left + imsize(m_SmallImageList).w;
  }
  else if (LVIR_LABEL == nArea) {
    rcItem = lpLVItem->rc;
    rcItem.left += imsize(m_SmallImageList).w;
  }

  return 0;
}


int set()
{
  m_nHotItem = (-1);
  m_bDrawHotItem = (TRUE);
  m_bMouseEventsTracked = (FALSE);
  m_bExplorerVisualStyle = (FALSE);
  m_nLockDrawCount = (0);
  m_bMarkSortedColumn = TRUE;
  m_clrSortedColumn = _RGB(225, 242, 249);
  return 0;
}
int list_ctrl_hitrect(RECT rc)
{
  int nRow, nCol;
  int nTextHeight = FontGetTextSize("H", 1).h + 4;
  RECT rect = rc, rcRow, rcCol;

  RectCutTop(&rect, nTextHeight, 0, &m_rcHeader);

  rect = rc;

  for (nRow = 0; nRow < m_nCount + 1; ++nRow) {
    RectCutTop(&rect, nTextHeight, 0, &rcRow);

    for (nCol = 0; nCol < m_nColCount; ++nCol) {
      RectCutLeft(&rcRow, m_hdItem[nCol].nWidth, 0, &rcCol);
      m_hdItem[(nRow)*m_nColCount + nCol].rc = rcCol;
    }
  }

  return 0;
}

void list_ctrl_paint(HDDC hDC)
{
  int nRow, nCol;

  header_ctrl_paint(hDC);

  for (nRow = 0; nRow < m_nCount; ++nRow) {
    for (nCol = 0; nCol < m_nColCount; ++nCol) {
      OnDrawCell(hDC, nRow, nCol);
    }
  }

  return ;
}

void OnDrawCell(HDDC hDC, int nItem, int nSubItem)
{
  RECT rcItem;
  // if it is full-row select, we need to invalidate thw whole row
  int nArea = m_bIsFullRowSelect ? LVIR_BOUNDS : LVIR_LABEL;
  hditem_t* lpLVItem = m_hdItem + (nItem + 1) * m_nColCount + nSubItem;
  rcItem = lpLVItem->rc;

  // Fill the background first
  if (m_bIsWindowEnabled || 1) {
    RECT rect = rcItem;

    UINT nItemState = lpLVItem->state;

    CCustomDrawHelper itemDrawer;
    itemDrawer.m_bWndHasFocus   = 0;//UIIsFocus(this);
    itemDrawer.m_bFocusItem     = ((nItemState & LVIS_FOCUSED) ? TRUE : FALSE);
    itemDrawer.m_bSelected      = ((nItemState & LVIS_SELECTED) ? TRUE : FALSE);
    itemDrawer.m_bIsHotItem     = m_bDrawHotItem && m_nHotItem == nItem;

    BOOL bFilledWithSortClr     = FALSE;

    if (IsSortable() && m_bMarkSortedColumn && m_nSortColumn == nSubItem
        && !itemDrawer.m_bSelected && !itemDrawer.m_bIsHotItem) {
      RECT rectColumn;
      GetSubItemRect(lpLVItem, LVIR_LABEL, rectColumn);
      TFillSolidRect(hDC, &rectColumn, m_clrSortedColumn);
      bFilledWithSortClr = TRUE;
    }

    if (0 == nSubItem || (bFilledWithSortClr && itemDrawer.m_bFocusItem)) {
      itemDrawer.m_bDrawBorderWhenFill  = 0 == nSubItem;
      itemDrawer.DrawItemBackground(hDC, rect, 255);
    }
  }

  // Draw the icon
  RECT rectIcon;
  rectIcon = lpLVItem->rc;
  rcItem.right = rcItem.left + imsize(m_SmallImageList).w;

  BOOL bHasIcon = FALSE;

  if (m_SmallImageList->height && lpLVItem->iImage >= 0) {
    RECT rect = rectIcon;
    SIZE sizeImage = iSIZE(0, 0);
    int ny = rect.top + (RCH(&rect) - sizeImage.h) / 2;

    // Unfortunately img_t::Draw(..ILD_TRANSPARENT) can't take care of transparent icon in
    // expected way, I see black outline on the bitmap, so here I use DrawIconEx instead
    //      POINT ptImage(rect.left, ny);
    //      pImageList->Draw(hDC, lpLVItem->iImage, ptImage, ILD_TRANSPARENT);

    bHasIcon = TRUE;
  }

  // Draw the text
  RECT rectLabel;
  GetSubItemRect(lpLVItem, LVIR_LABEL, rectLabel);

  {
    RECT rect = rectLabel;
    UINT nItemState = lpLVItem->state;

    str_t* strLabel = lpLVItem->text;
    COLORREF clrText = _RGB(0, 0, 0);

    if (strLabel->l > 0) {

      if (nItemState & LVIS_SELECTED && IsFocus() && (m_bIsFullRowSelect)) {
        clrText =  _GetSysColor(COLOR_HIGHLIGHTTEXT);
      }

      UINT nJustify = DT_LEFT;

      switch (lpLVItem->fmt & LVCFMT_JUSTIFYMASK) {
      case LVCFMT_RIGHT:
        nJustify = DT_RIGHT;
        break;

      case LVCFMT_CENTER:
        nJustify = DT_CENTER;
        break;

      default:
        break;
      }

      UINT nDTFormat = nJustify | DT_END_ELLIPSIS | DT_VCENTER | DT_SINGLELINE | DT_NOPREFIX;

      draw_textR(hDC, strLabel->s, strLabel->l, &rect, nDTFormat, clrText);

    }
  }
}

