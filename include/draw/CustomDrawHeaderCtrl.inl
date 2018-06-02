

void header_ctrl_set()
{
  m_bIsMousePressed = (FALSE);
  m_nHighlightedItem = (-1);
  m_nSortColumn = (-1);
  m_bSortable = (TRUE);
}

void header_ctrl_drawitem(HDDC hDC, int iItem, RECT rect, BOOL bIsPressed, BOOL bIsHighlighted)
{
  hditem_t hdItem;

  if (m_bSortable) {
    if (m_bSortable && iItem >= 0 && iItem == m_nSortColumn) {
      if (!bIsPressed && !bIsHighlighted) {
        Fill4ColorsGradient(hDC, rect,
            RGB_HEADER_SORT_FILL_COLOR1, RGB_HEADER_SORT_FILL_COLOR2, RGB_HEADER_SORT_FILL_COLOR3, RGB_HEADER_SORT_FILL_COLOR4,
            TRUE, HEADER_FIRST_HALF_PERCENTAGE, 255);
      }

      POINT ptCenter = CenterPoint(&rect);
      RECT rectArrow;
      rectArrow.left = ptCenter.x - 3;
      rectArrow.right = ptCenter.x + 3;
      rectArrow.top = rect.top + 2;
      rectArrow.bottom = rectArrow.top + 3;
      DrawSortArrow(hDC, rectArrow, m_bAscending);
    }
  }
  else {
    CCustomDrawHelper drawer;
    drawer.m_bWndEnabled    = 1;
    drawer.m_bWndHasFocus   = 1;
    drawer.m_bFocusItem     = FALSE;
    drawer.m_bIsHotItem     = bIsHighlighted ? TRUE : FALSE;
    drawer.m_bIsPressed     = bIsPressed ? TRUE : FALSE;
    drawer.DrawThemeBackground(hDC, rect, CDTBT_HEADER);
  }

  // Draw border:
  {
    if (m_bSortable && iItem >= 0 && iItem == m_nSortColumn) {
      COLORREF clrBorder = _RGB(150, 217, 249);
      DrawRect(hDC, rect, 0, clrBorder);
    }
    else {

      if (CUSTOMDRAW_GRADIENT) {
        COLORREF clrBorder;

        if (bIsPressed || bIsHighlighted) {
          if (bIsPressed) {
            clrBorder = _RGB(112, 185, 223);
          }
          else {
            clrBorder = _RGB(147, 201, 227);
          }
        }
        else {
          clrBorder = _RGB(213, 213, 213);
        }

        COLORREF brBorder(clrBorder);
        DrawRect(hDC, rect, 0, clrBorder);
      }
      else {
        COLORREF clrHilite  = _GetSysColor(COLOR_3DHILIGHT);
        COLORREF clrShadow  = _GetSysColor(COLOR_3DSHADOW);

        if (bIsPressed) {
          Draw3dRect(hDC, &rect, clrShadow, clrShadow);

          rect.left++;
          rect.top++;
        }
        else {
          Draw3dRect(hDC, &rect, clrHilite, clrShadow);
        }

      }// CUSTOMDRAW_GRADIENT

    }
  }


  if (iItem < 0) {
    return;
  }

  hdItem = m_hdItem[iItem];

  // Draw bitmap and image:
  // The column has a image from imagelist
  if ((hdItem.fmt & HDF_IMAGE) && hdItem.iImage >= 0) {
    img_t* pImageList = NULL;

    if (pImageList != NULL) {
      SIZE sz = imsize(pImageList);
      RECT rc = rect;
      RectAlign(rect, sz.w, sz.h, DT_VCENTER, &rc);
      draw_imageR(hDC, &rc, pImageList, hdItem.iImage, 0, 0);
      rect.left += sz.w;
    }
  }

  if ((hdItem.fmt & (HDF_BITMAP | HDF_BITMAP_ON_RIGHT)) && hdItem.hbm != NULL) {
    img_t* pBmp = (img_t*)(hdItem.hbm);
    RECT rectBitmap = rect;

    if (hdItem.fmt & HDF_BITMAP_ON_RIGHT) {
      rectBitmap.right--;
      rect.right = rectBitmap.left = rectBitmap.right - pBmp->width;
    }
    else {
      rectBitmap.left++;
      rect.left = rectBitmap.right = rectBitmap.left + pBmp->width;
    }

    rectBitmap.top += MAX(0, (RCH(&rectBitmap) - pBmp->height) / 2);
    rectBitmap.bottom = rectBitmap.top + pBmp->height;

    //draw_imageR(hDC, rectBitmap.TopLeft(), rectBitmap.Size(), pBmp, DSS_NORMAL);
  }

  // Draw text:
  if ((hdItem.fmt & HDF_STRING) && hdItem.text->l > 0) {
    RECT rectLabel = rect;
    const int nTextMargin = 5;
    DeflateRect(&rectLabel, nTextMargin, 0);

    if (bIsPressed) {
      OffsetRect(&rectLabel, 1, 1); // push-like
    }

    UINT uiTextFlags = DT_VCENTER | DT_SINGLELINE | DT_END_ELLIPSIS | DT_NOPREFIX;

    if (hdItem.fmt & HDF_CENTER) {
      uiTextFlags |= DT_CENTER;
    }
    else if (hdItem.fmt & HDF_RIGHT) {
      uiTextFlags |= DT_RIGHT;
    }

    //hDC->DrawText(hdItem.pszText, rectLabel, uiTextFlags);
  }
}

void header_ctrl_paint(HDDC hDC)
{
  TFillSolidRect(hDC, &m_rcHeader, _GetSysColor(COLOR_WINDOW));

  COLORREF clrText;
  clrText = (_RGB(0x4C, 0x4C, 0x4C));

  RECT rectItem = m_rcHeader;

  int xMax = 0;

  for (int i = 0; i < m_nColCount; i++) {
    // Is item pressed?
    BOOL bIsHighlighted = m_hit == i;
    BOOL bIsPressed = m_bIsMousePressed && bIsHighlighted;
    header_ctrl_drawitem(hDC, i, m_hdItem[i].rc, bIsPressed, m_nHighlightedItem == i);
    xMax = MAX(xMax, m_hdItem[i].rc.right);
  }

  // Draw "tail border":
  if (m_nColCount == 0) {
    rectItem = m_rcHeader;
    rectItem.right++;
  }
  else {
    rectItem.left = xMax;
    rectItem.right = m_rcHeader.right + 1;
  }

  header_ctrl_drawitem(hDC, -1, rectItem, FALSE, FALSE);
}

void header_ctrl_mousemove(UINT nFlags, POINT point)
{
  if ((nFlags & MK_LBUTTON) == 0) {
    HDHITTESTINFO hdHitTestInfo;
    hdHitTestInfo.pt = point;

    int nPrevHighlightedItem = m_nHighlightedItem;
    m_nHighlightedItem = 0;//(int) SendMessage(HDM_HITTEST, 0, (LPARAM) &hdHitTestInfo);

    if ((hdHitTestInfo.flags & HHT_ONHEADER) == 0) {
      m_nHighlightedItem = -1;
    }

    if (nPrevHighlightedItem != m_nHighlightedItem) {
      RedrawWindow();
    }
  }
}

int InsertColumn(int nCol, LPCTSTR lpszColumnHeading, int nFormat = LVCFMT_LEFT, int nWidth = -1)
{
  hditem_t* pItem;
  SIZE sz = iSIZE(m_nColCount * sizeof(hditem_t), m_nCount + 1);
  m_hdItem = (hditem_t*)mat_inscol(m_hdItem, sz, nCol * sizeof(hditem_t), sizeof(hditem_t));
  pItem = m_hdItem + nCol;
  str_setstr2(pItem->text, lpszColumnHeading);
  pItem->fmt = nFormat;
  pItem->nWidth = nWidth;
  ++m_nColCount;
  return 0;
}

int InsertItem(int nItem, LPCTSTR lpszText, int nImage = -1)
{
  hditem_t* pItem;
  SIZE sz = iSIZE(m_nColCount * sizeof(hditem_t), m_nCount + 1);
  m_hdItem = (hditem_t*)mat_insrow(m_hdItem, sz, nItem + 1, 1);
  pItem = m_hdItem + (nItem + 1) * m_nColCount;
  str_setstr2(pItem->text, lpszText);
  pItem->iImage = nImage;
  pItem->nWidth = FontGetTextSize(lpszText, strlen(lpszText)).w + 10;
  ++m_nCount;
  return 0;
}

BOOL SetItemText(int nItem, int nSubItem, LPTSTR lpszText)
{
  hditem_t* pItem = m_hdItem + (nItem + 1) * m_nColCount + nSubItem;
  str_setstr2(pItem->text, lpszText);
  pItem->nWidth = FontGetTextSize(lpszText, strlen(lpszText)).w + 10;
  return TRUE;
}

BOOL SetColumnWidth(int nCol, int cx)
{
  int i;

  if (cx < 0) {
    for (i = 0; i < m_nCount; ++i) {
      hditem_t* pItem = m_hdItem + (i + 1) * m_nColCount + nCol;
      cx = MAX(cx, pItem->nWidth);
    }
  }

  m_hdItem[nCol].nWidth = cx;
  return 0;
}
