/*
int _x0, _y0;
COLORREF _clr;
*/
#define IsExist(s, hTab) (0<=hTab && s->m_vtrtabsLen>hTab)
#define CmpTabs(s, hTab1, hTab2) ((hTab1==hTab2)?0:(hTab1>hTab2 ? 1:-1))
#define IsSystemButton(s, hTab) ((hTab)>SYSTEMBUTTON)
#define GetTabBorderColor2(s, hover) s->m_TabBorderColor2[!!(hover)]
#define SetTabBorderColor2(s, a, b) GetTabBorderColor2(s, TRUE) = a, GetTabBorderColor2(s, FALSE) = b
COLORREF _GetTabBorderColor2(const TabCtrl* s, BOOL hover)
{
  return _GetSysColor(COLOR_BTNSHADOW);
}
COLORREF GetTabBorderColor3(const TabCtrl* s, BOOL active, BOOL disable)
{
  switch (s->m_nStyle) {
    // it is approximate color (VS2003 uses some another way).
  case TabCtrlStyle_VS2008_client_classic: {
    return _GetSysColor(COLOR_BTNSHADOW);
  }

  case TabCtrlStyle_VS2008_client_blue: {
    if (active == TRUE) {
      return _RGB(105, 161, 191);
    }

    if (disable == TRUE) {
      return _RGB(145, 150, 162);
    }

    return _RGB(145, 150, 162);
  }

  case TabCtrlStyle_VS2008_client_silver: {
    if (active == TRUE) {
      return _RGB(147, 145, 176);
    }

    if (disable == TRUE) {
      return _RGB(157, 157, 161);
    }

    return _RGB(157, 157, 161);
  }

  case TabCtrlStyle_VS2008_client_olive: {
    if (active == TRUE) {
      return _RGB(147, 160, 112);
    }

    if (disable == TRUE) {
      return _RGB(172, 168, 153);
    }

    return _RGB(172, 168, 153);
  }
  };

  return _GetSysColor(COLOR_BTNSHADOW);
}
RECT GetCtrlAreaPadding(const TabCtrl* s)
{
  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2003_client:
  case TabCtrlStyle_VS2003_client_custom1:
    return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(5, 2, 3, 0) : iRECT(5, 0, 3, 2));

  case TabCtrlStyle_VS2003_bars:
    return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(5, 2, 3, 2) : iRECT(5, 2, 3, 2));

  case TabCtrlStyle_VS2008_client_base:
  case TabCtrlStyle_VS2008_client_classic:
  case TabCtrlStyle_VS2008_client_blue:
  case TabCtrlStyle_VS2008_client_silver:
  case TabCtrlStyle_VS2008_client_olive:
    return iRECT(0, 0, 3, 0);

  case TabCtrlStyle_VS2008_bars_base:
  case TabCtrlStyle_VS2008_bars_classic:
  case TabCtrlStyle_VS2008_bars_classic_custom1:
  case TabCtrlStyle_VS2008_bars_blue:
  case TabCtrlStyle_VS2008_bars_blue_custom1:
  case TabCtrlStyle_VS2008_bars_silver :
  case TabCtrlStyle_VS2008_bars_silver_custom1:
  case TabCtrlStyle_VS2008_bars_olive:
  case TabCtrlStyle_VS2008_bars_olive_custom1:
    return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(0, 1, 0, 2) : iRECT(0, 2, 0, 1));

  case TabCtrlStyle_VS2008_bars_custom1_base:
    return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(0, 1, 0, 0) : iRECT(0, 0, 0, 1));

  case TabCtrlStyle_VS2010_client:
  case TabCtrlStyle_VS2010_client_custom1:
  case TabCtrlStyle_VS2010_client_custom2:
    return iRECT(0, 0, 3, 0);

  case TabCtrlStyle_VS2003_bars_custom1:
    return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(4, 1, 2, 0) : iRECT(4, 0, 2, 1));

  case TabCtrlStyle_VS2010_bars:
    return iRECT(0, 0, 3, 0);
  };

  return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(3, 1, 3, 2) : iRECT(3, 2, 3, 1));
}
RECT GetTabHorzMargin(const TabCtrl* s)
{
  return iRECT(0, 0, 0, 0);
}
RECT GetTabPadding(const TabCtrl* s)
{
  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2003_client:
  case TabCtrlStyle_VS2003_client_custom1: {
    return (s->m_Layout == TAB_LAYOUT_TOP ?
        iRECT(6, 1/*border*/ + 2, 6, 2 + 1/*line*/) : iRECT(6, 1/*line*/ + 2, 6, 2 + 1/*border*/));
  }

  case TabCtrlStyle_VS2003_bars:
  case TabCtrlStyle_VS2003_bars_custom1: {
    return (s->m_Layout == TAB_LAYOUT_TOP ?
        iRECT(6, 1/*border*/ + 1, 6, 1 + 1/*line*/) : iRECT(6, 1/*line*/ + 1, 6, 1 + 1/*border*/));
  }

  case TabCtrlStyle_VS2008_client_base:
  case TabCtrlStyle_VS2008_client_classic:
  case TabCtrlStyle_VS2008_client_blue:
  case TabCtrlStyle_VS2008_client_silver:
  case TabCtrlStyle_VS2008_client_olive: {
    return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(6, 1/*border*/ + 2, 6, 3 + 2/*indent*/) : iRECT(6, 2/*indent*/ + 2, 6, 3 + 1/*border*/));
  }

  case TabCtrlStyle_VS2008_bars_base:
  case TabCtrlStyle_VS2008_bars_custom1_base:
  case TabCtrlStyle_VS2008_bars_classic:
  case TabCtrlStyle_VS2008_bars_classic_custom1:
  case TabCtrlStyle_VS2008_bars_blue:
  case TabCtrlStyle_VS2008_bars_blue_custom1:
  case TabCtrlStyle_VS2008_bars_silver :
  case TabCtrlStyle_VS2008_bars_silver_custom1:
  case TabCtrlStyle_VS2008_bars_olive:
  case TabCtrlStyle_VS2008_bars_olive_custom1: {
    return (s->m_Layout == TAB_LAYOUT_TOP ?
        iRECT(1/*border*/ + 2, 1 + 1/*border*/ + 2/*indent*/, 3, 1/*border*/ + 1) :
        iRECT(1/*border*/ + 2, 1/*border*/ + 1, 3, 1 + 1/*border*/ + 2/*indent*/));
  }

  case TabCtrlStyle_VS2010_client:
  case TabCtrlStyle_VS2010_client_custom1:
  case TabCtrlStyle_VS2010_client_custom2: {
    return (s->m_Layout == TAB_LAYOUT_TOP ?
        iRECT(5, 1/*border*/ + 3, 5, 3) : iRECT(5, 3, 5, 3 + 1/*border*/));
  }

  case TabCtrlStyle_VS2010_bars: {
    return (s->m_Layout == TAB_LAYOUT_TOP ?
        iRECT(5, 1/*border*/ + 1, 5, 2) : iRECT(5, 2, 5, 1 + 1/*border*/));
  }
  }

  return iRECT(5, 2 + 1/*border*/, 5, 2 + 1/*border*/);
}
int GetSlantWidth(const TabCtrl* s)
{
  return RCH(&s->m_rcTabs);
}
int GetTabExtraWidth(const TabCtrl* s, int hTab)
{
  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2008_client_base:
  case TabCtrlStyle_VS2008_client_classic:
  case TabCtrlStyle_VS2008_client_blue:
  case TabCtrlStyle_VS2008_client_silver:
  case TabCtrlStyle_VS2008_client_olive: {
    if (hTab != 0) {
      return 0; // it isn't first tab.
    }

    return GetSlantWidth(s) - 6;
  }
  }

  return 0;
}
BOOL IsStraightOrder(const TabCtrl* s)
{
  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2008_client_base:
  case TabCtrlStyle_VS2008_client_classic:
  case TabCtrlStyle_VS2008_client_blue:
  case TabCtrlStyle_VS2008_client_silver:
  case TabCtrlStyle_VS2008_client_olive: {
    return FALSE;
  }
  }

  return TRUE;
}
#define GetIconSize(im, psz) ((psz)->w=(im)->width, (psz)->h=(im)->height)
int TabCtrlStyle_base_GetTabMinWidth(const TabCtrl* s)
{
  RECT rcTabPadding = GetTabPadding(s);
  SIZE szImage, szImageDisabled;
  GetIconSize(s->m_ImageList, &szImage);
  GetIconSize(s->m_ImageListDisabled, &szImageDisabled);
  return rcTabPadding.left + MAX(szImage.w, szImageDisabled.w) + rcTabPadding.right;
}
COLORREF GetTabOutlineColor(const TabCtrl* s, BOOL active, BOOL hover, BOOL disable, BOOL left)
{
  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2008_client_classic: {
    if (active == TRUE) {
      return _GetSysColor(COLOR_WINDOW);
    }

    if (hover == TRUE && disable == FALSE) {
      return _GetSysColor(COLOR_WINDOW);
    }

    if (disable == TRUE) {
      return _GetSysColor(COLOR_BTNFACE);
    }

    return _GetSysColor(COLOR_BTNFACE);
  }

  case TabCtrlStyle_VS2008_client_blue: {
    if (active == TRUE) {
      return _RGB(255, 255, 255);
    }

    if (hover == TRUE && disable == FALSE) {
      return _RGB(255, 255, 255);
    }

    if (disable == TRUE) {
      return _RGB(140, 171, 204);
    }

    return (left == TRUE ? _RGB(242, 250, 255) : _RGB(140, 171, 204));
  }

  case TabCtrlStyle_VS2008_client_silver: {
    if (active == TRUE) {
      return _RGB(255, 255, 255);
    }

    if (hover == TRUE && disable == FALSE) {
      return _RGB(255, 255, 255);
    }

    if (disable == TRUE) {
      return _RGB(172, 171, 196);
    }

    return (left == TRUE ? _RGB(255, 255, 255) : _RGB(172, 171, 196));
  }

  case TabCtrlStyle_VS2008_client_olive: {
    if (active == TRUE) {
      return _RGB(255, 255, 255);
    }

    if (hover == TRUE && disable == FALSE) {
      return _RGB(255, 255, 255);
    }

    if (disable == TRUE) {
      return _RGB(165, 179, 133);
    }

    return (left == TRUE ? _RGB(255, 255, 255) : _RGB(165, 179, 133));
  }
  };

  //ASSERT(0);
  return 0;
}
int DrawCtrlAreaBack(const TabCtrl* s, HDDC pDC, const RECT* pRect)
{
  int _x0, _y0;

  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2003_client:
  case TabCtrlStyle_VS2003_client_custom1: {
    FillSolidRectRC(pDC, pRect, s->m_CtrlAreaBackColor);

    if (s->m_Layout == TAB_LAYOUT_TOP) {
      draw_line(pDC, pRect->left, pRect->bottom - 1, pRect->right, pRect->bottom - 1, _GetSysColor(COLOR_BTNHIGHLIGHT));
    }
    else {
      draw_line(pDC, pRect->left, pRect->top, pRect->right, pRect->top, _GetSysColor(COLOR_BTNHIGHLIGHT));
    }

    draw_line(pDC, pRect->left, pRect->top, pRect->left, pRect->bottom, s->m_WndsAreaBackColor);
    draw_line(pDC, pRect->right - 1, pRect->top, pRect->right - 1, pRect->bottom, s->m_WndsAreaBackColor);
    return 0;
  }

  case TabCtrlStyle_VS2003_bars: {
    RECT rc = (*pRect);
    COLORREF clrWndsAreaBack = s->m_WndsAreaBackColor;

    if (s->m_Layout == TAB_LAYOUT_TOP) {
      draw_line(pDC, rc.left + 1, rc.top, rc.right - 1, rc.top, clrWndsAreaBack);
      ++rc.top;
      rc.bottom -= 3;
      FillSolidRectRC(pDC, &rc, s->m_CtrlAreaBackColor);
      rc.top = rc.bottom;
      rc.bottom = pRect->bottom;
      draw_line(pDC, rc.left + 1, rc.top, rc.right - 1, rc.top, _GetSysColor(COLOR_BTNTEXT));
      ++rc.top;
      FillSolidRectRC(pDC, &rc, clrWndsAreaBack);
    }
    else {
      rc.top += 3;
      --rc.bottom;
      draw_line(pDC, rc.left + 1, rc.bottom, rc.right - 1, rc.bottom, clrWndsAreaBack);
      FillSolidRectRC(pDC, &rc, s->m_CtrlAreaBackColor);
      --rc.top;
      draw_line(pDC, rc.left + 1, rc.top, rc.right - 1, rc.top, _GetSysColor(COLOR_BTNTEXT));
      rc.bottom = rc.top;
      rc.top = pRect->top;
      FillSolidRectRC(pDC, &rc, clrWndsAreaBack);
    }

    draw_line(pDC, pRect->left, pRect->top, pRect->left, pRect->bottom, clrWndsAreaBack);
    draw_line(pDC, pRect->right - 1, pRect->top, pRect->right - 1, pRect->bottom, clrWndsAreaBack);
    return 0;
  }

  case TabCtrlStyle_VS2003_bars_custom1: {
    RECT rc = (*pRect);
    COLORREF clrBorder = s->m_TabBorderColor;
    COLORREF clrCtrlAreaBack = s->m_CtrlAreaBackColor;

    if (s->m_Layout == TAB_LAYOUT_TOP) {
      --rc.bottom;
      FillSolidRectRC(pDC, &rc, clrCtrlAreaBack);
      draw_line(pDC, rc.left, rc.bottom, rc.right, rc.bottom, clrBorder);
    }
    else {
      draw_line(pDC, rc.left, rc.top, rc.right, rc.top, clrBorder);
      ++rc.top;
      FillSolidRectRC(pDC, &rc, clrCtrlAreaBack);
    }

    return 0;
  }

  case TabCtrlStyle_VS2008_client_base:
  case TabCtrlStyle_VS2008_client_classic:
  case TabCtrlStyle_VS2008_client_blue:
  case TabCtrlStyle_VS2008_client_silver:
  case TabCtrlStyle_VS2008_client_olive: {
    COLORREF penBorder, penOutline, _clr;
    BOOL allDisabled = (s->m_hCurTab < 0); // all tabs are disabled.
    FillSolidRectRC(pDC, pRect, s->m_CtrlAreaBackColor);
    penBorder = (GetTabBorderColor3(s, TRUE, allDisabled));
    penOutline = (GetTabOutlineColor(s, TRUE, FALSE, allDisabled, TRUE));
    _clr = (penBorder);

    if (s->m_Layout == TAB_LAYOUT_TOP) {
      if (allDisabled == FALSE) {
        // draw border.
        TMoveTo(pDC, pRect->left + 1, pRect->bottom - 1);
        _LineTo(pDC, pRect->left + 2, pRect->bottom - 2);
        _LineTo(pDC, pRect->right - 3, pRect->bottom - 2);
        _LineTo(pDC, pRect->right - 1, pRect->bottom);
        // draw outline.
        _clr = (penOutline);
        TMoveTo(pDC, pRect->left + 2, pRect->bottom - 1);
        _LineTo(pDC, pRect->right - 2, pRect->bottom - 1);
      }
      else {
        // draw border.
        TMoveTo(pDC, pRect->left, pRect->bottom - 2);
        _LineTo(pDC, pRect->right, pRect->bottom - 2);
        // draw outline.
        _clr = (penOutline);
        TMoveTo(pDC, pRect->left, pRect->bottom - 1);
        _LineTo(pDC, pRect->right, pRect->bottom - 1);
      }
    }
    else {
      if (allDisabled == FALSE) {
        // draw border.
        TMoveTo(pDC, pRect->left + 1, pRect->top);
        _LineTo(pDC, pRect->left + 2, pRect->top + 1);
        _LineTo(pDC, pRect->right - 3, pRect->top + 1);
        _LineTo(pDC, pRect->right - 1, pRect->top - 1);
        // draw outline.
        _clr = (penOutline);
        TMoveTo(pDC, pRect->left + 2, pRect->top);
        _LineTo(pDC, pRect->right - 2, pRect->top);
      }
      else {
        // draw border.
        TMoveTo(pDC, pRect->left, pRect->top + 1);
        _LineTo(pDC, pRect->right, pRect->top + 1);
        // draw outline.
        _clr = (penOutline);
        TMoveTo(pDC, pRect->left, pRect->top);
        _LineTo(pDC, pRect->right, pRect->top);
      }
    }

    //_clr = (pOldPen);
    return 0;
  }

  case TabCtrlStyle_VS2008_bars_base:
  case TabCtrlStyle_VS2008_bars_classic:
  case TabCtrlStyle_VS2008_bars_classic_custom1:
  case TabCtrlStyle_VS2008_bars_blue:
  case TabCtrlStyle_VS2008_bars_blue_custom1:
  case TabCtrlStyle_VS2008_bars_silver :
  case TabCtrlStyle_VS2008_bars_silver_custom1:
  case TabCtrlStyle_VS2008_bars_olive:
  case TabCtrlStyle_VS2008_bars_olive_custom1: {
    COLORREF clrBorder;
    FillSolidRectRC(pDC, pRect, s->m_CtrlAreaBackColor);
    clrBorder = GetTabBorderColor2(s, FALSE);

    if (s->m_Layout == TAB_LAYOUT_TOP) {
      draw_line(pDC, pRect->left, pRect->bottom - 3, pRect->right, pRect->bottom - 3, clrBorder);
    }
    else {
      draw_line(pDC, pRect->left, pRect->top + 2, pRect->right, pRect->top + 2, clrBorder);
    }

    return 0;
  }

  case TabCtrlStyle_VS2008_bars_custom1_base: {
    COLORREF clrBorder;
    FillSolidRectRC(pDC, pRect, s->m_CtrlAreaBackColor);
    clrBorder = GetTabBorderColor2(s, FALSE);

    if (s->m_Layout == TAB_LAYOUT_TOP) {
      draw_line(pDC, pRect->left, pRect->bottom - 1, pRect->right, pRect->bottom - 1, clrBorder);
    }
    else {
      draw_line(pDC, pRect->left, pRect->top, pRect->right, pRect->top, clrBorder);
    }

    return 0;
  }

  case TabCtrlStyle_VS2010_client: {
    FillSolidRectRC(pDC, pRect, s->m_CtrlAreaBackColor);
    return 0;
  }

  case TabCtrlStyle_VS2010_bars: {
    FillSolidRectRC(pDC, pRect, s->m_CtrlAreaBackColor);
    return 0;
  }
  };

  {
    RECT rc = (*pRect);

    if (s->m_Layout == TAB_LAYOUT_TOP) {
      // draw background for tabs area.
      rc.bottom -= GetCtrlAreaPadding(s).bottom;
      FillSolidRectRC(pDC, &rc, s->m_CtrlAreaBackColor);
      // draw separator.
      draw_line(pDC, pRect->left, rc.bottom - 1, pRect->right, rc.bottom - 1, s->m_TabBorderColor);
      // draw background for windows area.
      rc.top = rc.bottom;
      rc.bottom = pRect->bottom;

      if (IsRectEmpty(&rc) == 0) {
        FillSolidRectRC(pDC, &rc, s->m_ChildWndBackColor);
      }
    }
    else {
      // draw background for windows area.
      rc.bottom = rc.top + GetCtrlAreaPadding(s).top;

      if (IsRectEmpty(&rc) == 0) {
        FillSolidRectRC(pDC, &rc, s->m_ChildWndBackColor);
      }

      // draw separator.
      draw_line(pDC, pRect->left, rc.bottom, pRect->right, rc.bottom, s->m_TabBorderColor);
      // draw background for tabs area.
      rc.top = rc.bottom + 1;
      rc.bottom = pRect->bottom;
      FillSolidRectRC(pDC, &rc, s->m_CtrlAreaBackColor);
    }

    return 0;
  }
}
int TabCtrlStyle_VS2010_client_DrawTabBack(const TabCtrl* s, HDDC pDC, const RECT* pRect, BOOL active, BOOL select, BOOL hover)
{
  int _x0, _y0;
  RECT rc = (*pRect);

  if (s->m_Layout == TAB_LAYOUT_TOP) {
    if (select == TRUE) {
      if (active == TRUE) {
        rc.top = (rc.top + rc.bottom) / 2;
        FillSolidRectRC(pDC, &rc, _RGB(255, 232, 166)); // bottom path.
        rc.bottom = rc.top;
        rc.top = pRect->top;
        DrawGradient(pDC, &rc, FALSE, _RGB(255, 252, 242), _RGB(255, 243, 207)); // top path.
        // left corner.
        _SetPixel(pDC, rc.left, rc.top, _RGB(94, 109, 133));
        _SetPixel(pDC, rc.left + 1, rc.top, _RGB(195, 198, 199));
        _SetPixel(pDC, rc.left, rc.top + 1, _RGB(195, 197, 197));
        // right corner.
        _SetPixel(pDC, rc.right - 1, rc.top, _RGB(94, 109, 133));
        _SetPixel(pDC, rc.right - 2, rc.top, _RGB(195, 198, 199));
        _SetPixel(pDC, rc.right - 1, rc.top + 1, _RGB(195, 197, 197));
      }
      else {
        rc.top = (rc.top + rc.bottom) / 2;
        FillSolidRectRC(pDC, &rc, _RGB(206, 212, 223)); // bottom path.
        rc.bottom = rc.top;
        rc.top = pRect->top;
        DrawGradient(pDC, &rc, FALSE, _RGB(251, 252, 252), _RGB(215, 220, 228)); // top path.
        // left corner.
        _SetPixel(pDC, rc.left, rc.top, _RGB(93, 109, 135));
        _SetPixel(pDC, rc.left + 1, rc.top, _RGB(192, 198, 206));
        _SetPixel(pDC, rc.left, rc.top + 1, _RGB(190, 196, 204));
        // right corner.
        _SetPixel(pDC, rc.right - 1, rc.top, _RGB(93, 109, 135));
        _SetPixel(pDC, rc.right - 2, rc.top, _RGB(192, 198, 206));
        _SetPixel(pDC, rc.right - 1, rc.top + 1, _RGB(190, 196, 204));
      }
    }
    else if (hover == TRUE) {
      // draw border.
      COLORREF _clr = (_RGB(155, 167, 183));
      TMoveTo(pDC, rc.left, rc.bottom);
      _LineTo(pDC, rc.left, rc.top);
      _LineTo(pDC, rc.right - 1, rc.top);
      _LineTo(pDC, rc.right - 1, rc.bottom);
      // draw back.
      DeflateRect4(&rc, 1, 1, 1, 0);
      DrawGradient(pDC, &rc, FALSE, _RGB(111, 119, 118), _RGB(79, 95, 116));
      InflateRect4(&rc, 1, 1, 1, 0);
      // left corner.
      _SetPixel(pDC, rc.left, rc.top, _RGB(50, 65, 93));
      _SetPixel(pDC, rc.left + 1, rc.top, _RGB(120, 135, 157));
      _SetPixel(pDC, rc.left, rc.top + 1, _RGB(117, 130, 150));
      _SetPixel(pDC, rc.left + 1, rc.top + 1, _RGB(115, 126, 139));
      // right corner.
      _SetPixel(pDC, rc.right - 1, rc.top, _RGB(50, 65, 93));
      _SetPixel(pDC, rc.right - 2, rc.top, _RGB(120, 135, 157));
      _SetPixel(pDC, rc.right - 1, rc.top + 1, _RGB(117, 130, 150));
      _SetPixel(pDC, rc.right - 2, rc.top + 1, _RGB(115, 126, 139));
      //DeleteObject(pen);
    }
  }
  else { // bottom.
    if (select == TRUE) {
      if (active == TRUE) {
        rc.bottom = (rc.top + rc.bottom) / 2;
        FillSolidRectRC(pDC, &rc, _RGB(255, 232, 166)); // top path.
        rc.top = rc.bottom;
        rc.bottom = pRect->bottom;
        DrawGradient(pDC, &rc, FALSE, _RGB(255, 243, 207), _RGB(255, 252, 242)); // bottom path.
        // left corner.
        _SetPixel(pDC, rc.left, rc.bottom - 1, _RGB(94, 109, 133));
        _SetPixel(pDC, rc.left + 1, rc.bottom - 1, _RGB(195, 198, 199));
        _SetPixel(pDC, rc.left, rc.bottom - 2, _RGB(195, 197, 197));
        // right corner.
        _SetPixel(pDC, rc.right - 1, rc.bottom - 1, _RGB(94, 109, 133));
        _SetPixel(pDC, rc.right - 2, rc.bottom - 1, _RGB(195, 198, 199));
        _SetPixel(pDC, rc.right - 1, rc.bottom - 2, _RGB(195, 197, 197));
      }
      else {
        rc.bottom = (rc.top + rc.bottom) / 2;
        FillSolidRectRC(pDC, &rc, _RGB(206, 212, 223)); // bottom path.
        rc.top = rc.bottom;
        rc.bottom = pRect->bottom;
        DrawGradient(pDC, &rc, FALSE, _RGB(215, 220, 228), _RGB(251, 252, 252)); // top path.
        // left corner.
        _SetPixel(pDC, rc.left, rc.bottom - 1, _RGB(93, 109, 135));
        _SetPixel(pDC, rc.left + 1, rc.bottom - 1, _RGB(192, 198, 206));
        _SetPixel(pDC, rc.left, rc.bottom - 2, _RGB(190, 196, 204));
        // right corner.
        _SetPixel(pDC, rc.right - 1, rc.bottom - 1, _RGB(93, 109, 135));
        _SetPixel(pDC, rc.right - 2, rc.bottom - 1, _RGB(192, 198, 206));
        _SetPixel(pDC, rc.right - 1, rc.bottom - 2, _RGB(190, 196, 204));
      }
    }
    else if (hover == TRUE) {
      // draw border.
      COLORREF pen = (_RGB(155, 167, 183));
      COLORREF _clr = (pen);
      TMoveTo(pDC, rc.left, rc.top);
      _LineTo(pDC, rc.left, rc.bottom - 1);
      _LineTo(pDC, rc.right - 1, rc.bottom - 1);
      _LineTo(pDC, rc.right - 1, rc.top);
      //_clr = (pOldPen);
      // draw back.
      DeflateRect4(&rc, 1, 0, 1, 1);
      DrawGradient(pDC, &rc, FALSE, _RGB(79, 95, 116), _RGB(111, 119, 118));
      InflateRect4(&rc, 1, 0, 1, 1);
      // left corner.
      _SetPixel(pDC, rc.left, rc.bottom - 1, _RGB(50, 65, 93));
      _SetPixel(pDC, rc.left + 1, rc.bottom - 1, _RGB(120, 135, 157));
      _SetPixel(pDC, rc.left, rc.bottom - 2, _RGB(117, 130, 150));
      _SetPixel(pDC, rc.left + 1, rc.bottom - 2, _RGB(115, 126, 139));
      // right corner.
      _SetPixel(pDC, rc.right - 1, rc.bottom - 1, _RGB(50, 65, 93));
      _SetPixel(pDC, rc.right - 2, rc.bottom - 1, _RGB(120, 135, 157));
      _SetPixel(pDC, rc.right - 1, rc.bottom - 2, _RGB(117, 130, 150));
      _SetPixel(pDC, rc.right - 2, rc.bottom - 2, _RGB(115, 126, 139));
      //DeleteObject(pen);
    }
  }

  return 0;
}
COLORREF GetTabGradientLightColor4(const TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2008_client_classic: {
    if (active == TRUE) {
      return _GetSysColor(COLOR_BTNHIGHLIGHT);
    }

    if (hover == TRUE && disable == FALSE) {
      return _GetSysColor(COLOR_BTNHIGHLIGHT);
    }

    return _GetSysColor(COLOR_BTNFACE);
  }

  case TabCtrlStyle_VS2008_client_blue: {
    if (active == TRUE) {
      return _RGB(252, 253, 254);
    }

    if (hover == TRUE && disable == FALSE) {
      return _RGB(247, 252, 254);
    }

    if (disable == TRUE) {
      return _RGB(207, 223, 237);
    }

    return _RGB(236, 245, 252);
  }

  case TabCtrlStyle_VS2008_client_silver: {
    if (active == TRUE) {
      return _RGB(247, 247, 253);
    }

    if (hover == TRUE && disable == FALSE) {
      return _RGB(247, 247, 253);
    }

    if (disable == TRUE) {
      return _RGB(203, 205, 217);
    }

    return _RGB(234, 235, 240);
  }

  case TabCtrlStyle_VS2008_client_olive: {
    if (active == TRUE) {
      return _RGB(250, 251, 247);
    }

    if (hover == TRUE && disable == FALSE) {
      return _RGB(245, 247, 240);
    }

    if (disable == TRUE) {
      return _RGB(208, 217, 181);
    }

    return _RGB(241, 244, 233);
  }

  case TabCtrlStyle_VS2008_bars_classic: {
    return _GetSysColor(COLOR_WINDOW);
  }
  }

  return _GetSysColor(COLOR_BTNFACE);
}
COLORREF GetTabGradientLightColor(const TabCtrl* s, BOOL hover, BOOL disable)
{
  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2008_bars_blue: {
    return ((hover == TRUE && disable == FALSE) ? _RGB(250, 253, 254) : _RGB(252, 252, 252));
  }

  case TabCtrlStyle_VS2008_bars_silver: {
    return ((hover == TRUE && disable == FALSE) ? _RGB(247, 247, 253) : _RGB(252, 252, 252));
  }

  case TabCtrlStyle_VS2008_bars_olive: {
    return ((hover == TRUE && disable == FALSE) ? _RGB(250, 251, 247) : _RGB(252, 252, 252));
  }

  case TabCtrlStyle_VS2008_bars_classic_custom1: {
    return _GetSysColor(COLOR_WINDOW);
  }

  case TabCtrlStyle_VS2008_bars_blue_custom1: {
    return ((hover == TRUE && disable == FALSE) ? _RGB(250, 253, 254) : _RGB(252, 252, 252));
  }

  case TabCtrlStyle_VS2008_bars_silver_custom1: {
    return ((hover == TRUE && disable == FALSE) ? _RGB(247, 247, 253) : _RGB(252, 252, 252));
  }

  case TabCtrlStyle_VS2008_bars_olive_custom1: {
    return ((hover == TRUE && disable == FALSE) ? _RGB(250, 251, 247) : _RGB(252, 252, 252));
  }
  }

  return _GetSysColor(COLOR_BTNFACE);
}
COLORREF GetTabGradientDarkColor4(const TabCtrl* s, BOOL active, BOOL hover, BOOL disable)
{
  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2008_client_classic: {
    if (active == TRUE) {
      return _GetSysColor(COLOR_BTNFACE);
    }

    if (hover == TRUE && disable == FALSE) {
      return _GetSysColor(COLOR_BTNHIGHLIGHT);
    }

    return _GetSysColor(COLOR_BTNFACE);
  }

  case TabCtrlStyle_VS2008_client_blue: {
    if (active == TRUE) {
      return _RGB(210, 230, 250);
    }

    if (hover == TRUE && disable == FALSE) {
      return _RGB(129, 208, 241);
    }

    if (disable == TRUE) {
      return _RGB(207, 223, 237);
    }

    return _RGB(152, 180, 210);
  }

  case TabCtrlStyle_VS2008_client_silver: {
    if (active == TRUE) {
      return _RGB(225, 226, 236);
    }

    if (hover == TRUE && disable == FALSE) {
      return _RGB(182, 185, 201);
    }

    if (disable == TRUE) {
      return _RGB(203, 205, 217);
    }

    return _RGB(172, 171, 196);
  }

  case TabCtrlStyle_VS2008_client_olive: {
    if (active == TRUE) {
      return _RGB(173, 190, 126);
    }

    if (hover == TRUE && disable == FALSE) {
      return _RGB(197, 210, 165);
    }

    if (disable == TRUE) {
      return _RGB(208, 217, 181);
    }

    return _RGB(165, 179, 133);
  }
  }

  return _RGB(165, 179, 133);
}
COLORREF GetTabGradientDarkColor(const TabCtrl* s, BOOL hover, BOOL disable)
{
  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2008_bars_classic: {
    return ((hover == TRUE && disable == FALSE) ?
        PixelAlpha1(_GetSysColor(COLOR_BTNSHADOW), _RGB(255, 255, 255), 75) :
        PixelAlpha1(_GetSysColor(COLOR_BTNSHADOW), _RGB(255, 255, 255), 55));
  }

  case TabCtrlStyle_VS2008_bars_blue: {
    return ((hover == TRUE && disable == FALSE) ? _RGB(167, 217, 245) : _RGB(207, 207, 207));
  }

  case TabCtrlStyle_VS2008_bars_silver: {
    return ((hover == TRUE && disable == FALSE) ? _RGB(180, 179, 202) : _RGB(207, 207, 207));
  }

  case TabCtrlStyle_VS2008_bars_olive: {
    return ((hover == TRUE && disable == FALSE) ? _RGB(182, 198, 141) : _RGB(207, 207, 207));
  }

  case TabCtrlStyle_VS2008_bars_classic_custom1: {
    return ((hover == TRUE && disable == FALSE) ?
        PixelAlpha1(_GetSysColor(COLOR_BTNSHADOW), _RGB(255, 255, 255), 75) :
        PixelAlpha1(_GetSysColor(COLOR_BTNSHADOW), _RGB(255, 255, 255), 55));
  }

  case TabCtrlStyle_VS2008_bars_blue_custom1: {
    return ((hover == TRUE && disable == FALSE) ? _RGB(167, 217, 245) : _RGB(207, 207, 207));
  }

  case TabCtrlStyle_VS2008_bars_silver_custom1: {
    return ((hover == TRUE && disable == FALSE) ? _RGB(180, 179, 202) : _RGB(207, 207, 207));
  }

  case TabCtrlStyle_VS2008_bars_olive_custom1: {
    return ((hover == TRUE && disable == FALSE) ? _RGB(182, 198, 141) : _RGB(207, 207, 207));
  }
  }

  return _GetSysColor(COLOR_BTNFACE);
}
#define DrawEdge(a, b, c, d) draw_3drectR(a, b, 0)
int DrawButtonFrame(const TabCtrl* s, HDDC pDC, const RECT* pRect, BOOL bHover, BOOL bPushed)
{
  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2003_base:
  case TabCtrlStyle_VS2003_client:
  case TabCtrlStyle_VS2003_bars: {
    if (bHover == TRUE && bPushed == FALSE) {
      DrawEdge(pDC, (RECT*)pRect, BDR_RAISEDOUTER, BF_RECT);
    }
    else if (bHover == TRUE && bPushed == TRUE) {
      DrawEdge(pDC, (RECT*)pRect, BDR_SUNKENINNER, BF_RECT);
    }
  }

  case TabCtrlStyle_VS2003_client_custom1: {
    break;
  }

  case TabCtrlStyle_VS2003_bars_custom1: {
    break;
  }

  case TabCtrlStyle_VS2008_client_silver: {
    if (bHover != bPushed) {
      FillSolidRectRC(pDC, pRect, _RGB(255, 227, 173));
      DrawRectRC(pDC, pRect, _RGB(74, 73, 107));
    }
    else if (bHover == TRUE && bPushed == TRUE) {
      FillSolidRectRC(pDC, pRect, _RGB(255, 182, 115));
      DrawRectRC(pDC, pRect, _RGB(74, 73, 107));
    }

    return 0;
  }

  case TabCtrlStyle_VS2008_client_olive: {
    if (bHover != bPushed) {
      FillSolidRectRC(pDC, pRect, _RGB(255, 227, 173));
      DrawRectRC(pDC, pRect, _RGB(118, 128, 95));
    }
    else if (bHover == TRUE && bPushed == TRUE) {
      FillSolidRectRC(pDC, pRect, _RGB(255, 182, 115));
      DrawRectRC(pDC, pRect, _RGB(118, 128, 95));
    }

    return 0;
  }

  case TabCtrlStyle_VS2008_bars_silver: {
    if (bHover != bPushed) {
      FillSolidRectRC(pDC, pRect, _RGB(255, 227, 173));
      DrawRectRC(pDC, pRect, _RGB(74, 73, 107));
    }
    else if (bHover == TRUE && bPushed == TRUE) {
      FillSolidRectRC(pDC, pRect, _RGB(255, 182, 115));
      DrawRectRC(pDC, pRect, _RGB(74, 73, 107));
    }

    return 0;
  }

  case TabCtrlStyle_VS2008_bars_olive: {
    if (bHover != bPushed) {
      FillSolidRectRC(pDC, pRect, _RGB(255, 227, 173));
      DrawRectRC(pDC, pRect, _RGB(118, 128, 95));
    }
    else if (bHover == TRUE && bPushed == TRUE) {
      FillSolidRectRC(pDC, pRect, _RGB(255, 182, 115));
      DrawRectRC(pDC, pRect, _RGB(118, 128, 95));
    }

    return 0;
  }

  case TabCtrlStyle_VS2008_bars_silver_custom1: {
    if (bHover != bPushed) {
      FillSolidRectRC(pDC, pRect, _RGB(255, 227, 173));
      DrawRectRC(pDC, pRect, _RGB(74, 73, 107));
    }
    else if (bHover == TRUE && bPushed == TRUE) {
      FillSolidRectRC(pDC, pRect, _RGB(255, 182, 115));
      DrawRectRC(pDC, pRect, _RGB(74, 73, 107));
    }

    return 0;
  }

  case TabCtrlStyle_VS2008_bars_olive_custom1: {
    if (bHover != bPushed) {
      FillSolidRectRC(pDC, pRect, _RGB(255, 227, 173));
      DrawRectRC(pDC, pRect, _RGB(118, 128, 95));
    }
    else if (bHover == TRUE && bPushed == TRUE) {
      FillSolidRectRC(pDC, pRect, _RGB(255, 182, 115));
      DrawRectRC(pDC, pRect, _RGB(118, 128, 95));
    }

    return 0;
  }

  case TabCtrlStyle_VS2010_client: {
    if (bHover != bPushed || (bHover == TRUE && bPushed == TRUE)) {
      RECT rc = (*pRect);
      DrawRectRC(pDC, &rc, _RGB(229, 195, 101));
      DeflateRect(&rc, 1, 1);

      if (bHover == TRUE && bPushed == TRUE) {
        FillSolidRectRC(pDC, &rc, _RGB(255, 232, 166));
      }
      else {
        FillSolidRectRC(pDC, &rc, _RGB(255, 252, 244));
      }
    }

    return 0;
  }

  case TabCtrlStyle_VS2010_bars: {
    if (bHover != bPushed || (bHover == TRUE && bPushed == TRUE)) {
      RECT rc = (*pRect);
      DrawRectRC(pDC, &rc, _RGB(229, 195, 101));
      DeflateRect(&rc, 1, 1);

      if (bHover == TRUE && bPushed == TRUE) {
        FillSolidRectRC(pDC, &rc, _RGB(255, 232, 166));
      }
      else {
        FillSolidRectRC(pDC, &rc, _RGB(255, 252, 244));
      }
    }

    return 0;
  }
  }

  if (bHover != bPushed) {
    FillSolidRectRC(pDC, pRect, PixelAlpha1(_GetSysColor(COLOR_HIGHLIGHT), _GetSysColor(COLOR_WINDOW), 30)); // it is approximate color (VisualStudio uses some another way).
    DrawRectRC(pDC, pRect, _GetSysColor(COLOR_HIGHLIGHT));
  }
  else if (bHover == TRUE && bPushed == TRUE) {
    FillSolidRectRC(pDC, pRect, PixelAlpha1(_GetSysColor(COLOR_HIGHLIGHT), _GetSysColor(COLOR_WINDOW), 22)); // it is approximate color (VisualStudio uses some another way).
    DrawRectRC(pDC, pRect, _GetSysColor(COLOR_HIGHLIGHT));
  }

  return 0;
}
COLORREF GetButtonMarkerColor(const TabCtrl* s, BOOL bHover, BOOL bPushed)
{
  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2003_base: {
    if (s->m_hCurTab >= 0) {
      return PixelAlpha1(_GetSysColor(COLOR_BTNSHADOW), _RGB(0, 0, 0), 55);
    }
  }
  break;

  case TabCtrlStyle_VS2010_client: {
    return ((bHover == TRUE || bPushed == TRUE) ? _RGB(0, 0, 0) : _RGB(206, 212, 221));
  }

  case TabCtrlStyle_VS2010_bars: {
    return ((bHover == TRUE || bPushed == TRUE) ? _RGB(0, 0, 0) : _RGB(206, 212, 221));
  }
  }

  return (s->m_hCurTab >= 0 ? _GetSysColor(COLOR_BTNTEXT) : _GetSysColor(COLOR_GRAYTEXT));
}
int DrawButtonClose(const TabCtrl* s, HDDC pDC, const RECT* pRect, BOOL bHover, BOOL bPushed)
{
  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2003_base:
  case TabCtrlStyle_VS2003_client:
  case TabCtrlStyle_VS2003_bars: {
    RECT rc = (*pRect);
    const img_t* pImageList = s->m_SysImageList;
    DrawButtonFrame(s, pDC, pRect, bHover, bPushed);

    if (bHover == TRUE && bPushed == TRUE) {
      OffsetRect(&rc, 1, 1);
    }

    DrawMarkerI(pDC, &rc, pImageList, IMAGE_CLOSE, GetButtonMarkerColor(s, bHover, bPushed));
    return 0;
  }

  case TabCtrlStyle_VS2003_client_custom1: {
    break;
  }

  case TabCtrlStyle_VS2003_bars_custom1: {
    break;
  }
  }

  {
    const img_t* pImageList = s->m_SysImageList;
    DrawButtonFrame(s, pDC, pRect, bHover, bPushed);
    DrawMarkerI(pDC, pRect, pImageList, IMAGE_CLOSE, GetButtonMarkerColor(s, bHover, bPushed));
  }

  return 0;
}
int DrawButtonMenu(const TabCtrl* s, HDDC pDC, const RECT* pRect, BOOL bHover, BOOL bPushed, BOOL bPartialView)
{
  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2003_base: {
    RECT rc = (*pRect);
    const img_t* pImageList = s->m_SysImageList;
    DrawButtonFrame(s, pDC, pRect, bHover, bPushed);

    if (bHover == TRUE && bPushed == TRUE) {
      OffsetRect(&rc, 1, 1);
    }

    DrawMarkerI(pDC, &rc, pImageList,
        (bPartialView == FALSE ? IMAGE_MENU : IMAGE_MENU_PARTIAL),
        GetButtonMarkerColor(s, bHover, bPushed));
    return 0;
  }

  case TabCtrlStyle_VS2003_client_custom1: {
    break;
  }

  case TabCtrlStyle_VS2003_bars_custom1: {
    break;
  }
  }

  {
    const img_t* pImageList = s->m_SysImageList;
    DrawButtonFrame(s, pDC, pRect, bHover, bPushed);
    DrawMarkerI(pDC, pRect, pImageList, (bPartialView == FALSE ? IMAGE_MENU : IMAGE_MENU_PARTIAL), GetButtonMarkerColor(s, bHover, bPushed));
    return 0;
  }
}
int DrawButtonScrollLeft(const TabCtrl* s, HDDC pDC, const RECT* pRect, BOOL bHover, BOOL bPushed, BOOL bPartialView)
{
  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2003_base:
  case TabCtrlStyle_VS2003_client:
  case TabCtrlStyle_VS2003_bars: {
    RECT rc = (*pRect);
    const img_t* pImageList = s->m_SysImageList;
    DrawButtonFrame(s, pDC, pRect, bHover, bPushed);

    if (bHover == TRUE && bPushed == TRUE) {
      OffsetRect(&rc, 1, 1);
    }

    DrawMarkerI(pDC, &rc, pImageList,
        (bPartialView == FALSE ? IMAGE_ARROW_LEFT_DISABLE : IMAGE_ARROW_LEFT),
        GetButtonMarkerColor(s, bHover, bPushed));
    return 0;
  }

  case TabCtrlStyle_VS2003_client_custom1: {
    break;
  }

  case TabCtrlStyle_VS2003_bars_custom1: {
    break;
  }
  }

  {
    const img_t* pImageList = s->m_SysImageList;
    DrawButtonFrame(s, pDC, pRect, bHover, bPushed);
    DrawMarkerI(pDC, pRect, pImageList, (bPartialView == FALSE ? IMAGE_ARROW_LEFT_DISABLE : IMAGE_ARROW_LEFT),
        GetButtonMarkerColor(s, bHover, bPushed));
    return 0;
  }
}
int DrawButtonScrollRight(const TabCtrl* s, HDDC pDC, const RECT* pRect, BOOL bHover, BOOL bPushed, BOOL bPartialView)
{
  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2003_base:
  case TabCtrlStyle_VS2003_client:
  case TabCtrlStyle_VS2003_bars: {
    RECT rc = (*pRect);
    const img_t* pImageList = s->m_SysImageList;
    DrawButtonFrame(s, pDC, pRect, bHover, bPushed);

    if (bHover == TRUE && bPushed == TRUE) {
      OffsetRect(&rc, 1, 1);
    }

    DrawMarkerI(pDC, &rc, pImageList, (bPartialView == FALSE ? IMAGE_ARROW_RIGHT_DISABLE : IMAGE_ARROW_RIGHT),
        GetButtonMarkerColor(s, bHover, bPushed));
    return 0;
  }

  case TabCtrlStyle_VS2003_client_custom1: {
    break;
  }

  case TabCtrlStyle_VS2003_bars_custom1: {
    break;
  }
  }

  {
    const img_t* pImageList = s->m_SysImageList;
    DrawButtonFrame(s, pDC, pRect, bHover, bPushed);
    DrawMarkerI(pDC, pRect, pImageList,
        (bPartialView == FALSE ? IMAGE_ARROW_RIGHT_DISABLE : IMAGE_ARROW_RIGHT),
        GetButtonMarkerColor(s, bHover, bPushed));
    return 0;
  }
}
int GetTabOutline(const TabCtrl* s, int hTab, const RECT* rect, BOOL top, DPOINT* pts, RECT* rcFill)
{
  BOOL first = (hTab == 0);
  BOOL selected = (s->m_hCurTab == hTab);
  int iSlantWidth = GetSlantWidth(s);
  RECT rc = (*rect);

  if (selected == FALSE) {
    rc.top += 2;
    rc.bottom -= 2;
    iSlantWidth -= 4;
  }

  if (first == FALSE) {
    rc.left -= (iSlantWidth - 6);
  }
  else if (selected == FALSE) {
    rc.left += 4;
  }

  if (top == TRUE) {
    if (selected == FALSE) {
      pts[0].x = rc.left;
      pts[0].y = rc.bottom;
      pts[1].x = rc.left + 1;
      pts[1].y = rc.bottom - 1;
    }
    else {
      pts[0].x = rc.left + 2;
      pts[0].y = rc.bottom;
      pts[1].x = rc.left + 2;
      pts[1].y = rc.bottom - 2;
    }

    pts[2].x = rc.left + iSlantWidth - 2;
    pts[2].y = rc.top + 2;
    pts[3].x = rc.left + iSlantWidth + 3;
    pts[3].y = rc.top;
    pts[4].x = rc.right - 3;
    pts[4].y = rc.top;
    pts[5].x = rc.right - 1;
    pts[5].y = rc.top + 2;

    if (selected == FALSE) {
      pts[6].x = rc.right - 1;
      pts[6].y = rc.bottom;
    }
    else {
      pts[6].x = rc.right - 1;
      pts[6].y = rc.bottom - 1;
    }

    pts[7].x = rc.right - 1;
    pts[7].y = rc.bottom;
  }
  else {
    if (selected == FALSE) {
      pts[0].x = rc.left;
      pts[0].y = rc.top;
      pts[1].x = rc.left + 1;
      pts[1].y = rc.top;
    }
    else {
      pts[0].x = rc.left + 2;
      pts[0].y = rc.top;
      pts[1].x = rc.left + 2;
      pts[1].y = rc.top + 1;
    }

    pts[2].x = rc.left + iSlantWidth - 2;
    pts[2].y = rc.bottom - 3;
    pts[3].x = rc.left + iSlantWidth + 3;
    pts[3].y = rc.bottom - 1;
    pts[4].x = rc.right - 3;
    pts[4].y = rc.bottom - 1;
    pts[5].x = rc.right - 1;
    pts[5].y = rc.bottom - 3;

    if (selected == FALSE) {
      pts[6].x = rc.right - 1;
      pts[6].y = rc.top - 1;
    }
    else {
      pts[6].x = rc.right - 1;
      pts[6].y = rc.top;
    }

    pts[7].x = rc.right - 1;
    pts[7].y = rc.top;
  }

  if (rcFill != NULL) {
    *rcFill = rc;

    if (selected == FALSE && first == FALSE) {
      rcFill->left = rect->left;
    }
  }

  return 8;
}
int DrawTabBack(const TabCtrl* s, HDDC pDC, int hTab, const RECT* pRect)
{
  int _x0, _y0;
  COLORREF _clr;

  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2003_client:
  case TabCtrlStyle_VS2003_client_custom1: {
    BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
    BOOL select = (s->m_hCurTab == hTab);
    RECT rc = (*pRect);

    if (select == TRUE) {
      COLORREF penBorder = (s->m_TabBorderColor);
      COLORREF _clr = (penBorder);

      if (top == TRUE) {
        FillSolidRectRC(pDC, &rc, s->m_WndsAreaBackColor); // fill background.
        // draw left-top border.
        TMoveTo(pDC, rc.left, rc.bottom - 1);
        _LineTo(pDC, rc.left, rc.top);
        _LineTo(pDC, rc.right, rc.top);
        // draw right border.
        draw_line(pDC, rc.right - 1, rc.top + 1, rc.right - 1, rc.bottom, _GetSysColor(COLOR_BTNTEXT));
      }
      else {
        FillSolidRectRC(pDC, &rc, s->m_WndsAreaBackColor); // fill background.
        // draw left-bottom border.
        TMoveTo(pDC, rc.left, rc.top);
        _LineTo(pDC, rc.left, rc.bottom - 1);
        _LineTo(pDC, rc.right, rc.bottom - 1);
        // draw right border.
        draw_line(pDC, rc.right - 1, rc.top, rc.right - 1, rc.bottom - 1, _GetSysColor(COLOR_BTNTEXT));
      }

      //_clr = (pOldPen);
      //DeleteObject(penBorder);
    }
    else {
      RECT rcTabPadding = GetTabPadding(s);
      DeflateRect4(&rc, 0, rcTabPadding.top - 1, 0, rcTabPadding.bottom - 1);
      draw_line(pDC, rc.right, rc.top, rc.right, rc.bottom, _GetSysColor(COLOR_BTNSHADOW)); // draw right separator.
    }

    return 0;
  }

  case TabCtrlStyle_VS2003_bars:
  case TabCtrlStyle_VS2003_bars_custom1: {
    BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
    BOOL select = (s->m_hCurTab == hTab);
    RECT rc = (*pRect);

    if (select == TRUE) {
      COLORREF penBorder = (s->m_TabBorderColor);
      COLORREF _clr = (penBorder);

      if (top == TRUE) {
        DeflateRect4(&rc, 1, 1, 1, 0);
        FillSolidRectRC(pDC, &rc, s->m_WndsAreaBackColor); // fill background.
        // draw left-top border.
        InflateRect4(&rc, 1, 1, 1, 0);
        TMoveTo(pDC, rc.left + 1, rc.top);
        _LineTo(pDC, rc.right - 1, rc.top);
        _LineTo(pDC, rc.right - 1, rc.bottom);
        // draw right border.
        draw_line(pDC, rc.left, rc.top, rc.left, rc.bottom - 1, _GetSysColor(COLOR_BTNHIGHLIGHT));
      }
      else {
        DeflateRect4(&rc, 1, 0, 1, 1);
        FillSolidRectRC(pDC, &rc, s->m_WndsAreaBackColor); // fill background.
        // draw left-bottom border.
        InflateRect4(&rc, 1, 0, 1, 1);
        TMoveTo(pDC, rc.left + 1, rc.bottom - 1);
        _LineTo(pDC, rc.right - 1, rc.bottom - 1);
        _LineTo(pDC, rc.right - 1, rc.top - 1);
        // draw right border.
        draw_line(pDC, rc.left, rc.top + 1, rc.left, rc.bottom, _GetSysColor(COLOR_BTNHIGHLIGHT));
      }

      //_clr = (pOldPen);
      //DeleteObject(penBorder);
    }
    else {
      RECT rcTabPadding = GetTabPadding(s);

      if (top == TRUE) {
        DeflateRect4(&rc, 0, rcTabPadding.top, 0, rcTabPadding.bottom + 1);
      }
      else {
        DeflateRect4(&rc, 0, rcTabPadding.top + 1, 0, rcTabPadding.bottom);
      }

      draw_line(pDC, rc.right, rc.top, rc.right, rc.bottom, _GetSysColor(COLOR_BTNSHADOW)); // draw right separator.
    }

    return 0;
  }

  case TabCtrlStyle_VS2008_client_base:
  case TabCtrlStyle_VS2008_client_classic:
  case TabCtrlStyle_VS2008_client_blue:
  case TabCtrlStyle_VS2008_client_silver:
  case TabCtrlStyle_VS2008_client_olive: {
    BOOL selected = (s->m_hCurTab == hTab);
    BOOL hover = (s->m_hHoverTab == hTab) && (s->m_hPushedTab < 0);
    BOOL disabled = s->m_vtrtabs[hTab].disable;
    BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
    COLORREF clrBorder = GetTabBorderColor3(s, selected, disabled);
    COLORREF clrBackLight = GetTabGradientLightColor4(s, selected, hover, disabled);
    COLORREF clrBackDark = GetTabGradientDarkColor4(s, selected, hover, disabled);
    COLORREF clrOutlineLeft = GetTabOutlineColor(s, selected, hover, disabled, TRUE);
    COLORREF clrOutlineRight = GetTabOutlineColor(s, selected, hover, disabled, FALSE);
    COLORREF penBorder = (clrBorder);
    COLORREF penOutlineLeft = (clrOutlineLeft);
    COLORREF penOutlineRight = (clrOutlineRight);
    DPOINT pts[8];
    RECT rcFill;
    GetTabOutline(s, hTab, pRect, top, pts, &rcFill);
    {
      DWORD clrs[2] = {clrBackLight, clrBackDark};
      // draw back.
      //DrawGradient(pDC, &rcFill, FALSE, clrs[!top], clrs[!!top]);
      DoPaintGradientPloy(pDC, rcFill, pts, 8, clrs[!top], clrs[!!top], 1);
    }
    // left outline.
    _clr = (penOutlineLeft);
    TMoveTo(pDC, pts[1].x + 1, pts[1].y);
    _LineTo(pDC, pts[2].x + 1, pts[2].y);
    _LineTo(pDC, pts[3].x + 1, pts[3].y);
    // right outline.
    _clr = (penOutlineRight);
    TMoveTo(pDC, pts[4].x - 1, pts[4].y);
    _LineTo(pDC, pts[5].x - 1, pts[5].y);
    _LineTo(pDC, pts[6].x - 1, pts[6].y);
    // draw border.
    _clr = (penBorder);
    TMoveTo(pDC, pts[1].x, pts[1].y);
    _LineTo(pDC, pts[2].x, pts[2].y);
    _LineTo(pDC, pts[3].x, pts[3].y);
    _LineTo(pDC, pts[4].x, pts[4].y);
    _LineTo(pDC, pts[5].x, pts[5].y);
    _LineTo(pDC, pts[6].x, pts[6].y);
    //_clr = (pOldPen);
    //DeleteObject(penBorder);
    //DeleteObject(penOutlineLeft);
    //DeleteObject(penOutlineRight);
    //DeleteObject(rgn);
    //DeleteObject(rgnRes);
    return 0;
  }

  case TabCtrlStyle_VS2008_bars_base:
  case TabCtrlStyle_VS2008_bars_custom1_base:
  case TabCtrlStyle_VS2008_bars_classic:
  case TabCtrlStyle_VS2008_bars_classic_custom1:
  case TabCtrlStyle_VS2008_bars_blue:
  case TabCtrlStyle_VS2008_bars_blue_custom1:
  case TabCtrlStyle_VS2008_bars_silver :
  case TabCtrlStyle_VS2008_bars_silver_custom1:
  case TabCtrlStyle_VS2008_bars_olive:
  case TabCtrlStyle_VS2008_bars_olive_custom1: {
    BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
    BOOL select = (s->m_hCurTab == hTab);
    int count = s->m_vtrtabsLen;
    COLORREF clrBorder = GetTabBorderColor2(s, FALSE);
    COLORREF penBorder = (clrBorder);
    COLORREF _clr = (penBorder);

    if (select == TRUE) {
      BOOL firstTab = ((0) == hTab);
      COLORREF clrBackSelected = s->m_TabSelectedBackColor;
      // draw back.
      FillSolidRectRC(pDC, pRect, clrBackSelected); // fill background.

      // draw tab border.
      if (top == TRUE) {
        TMoveTo(pDC, pRect->left, pRect->bottom - (firstTab == TRUE ? 1 : 2));
        _LineTo(pDC, pRect->left, pRect->top);
        _LineTo(pDC, pRect->right - 1, pRect->top);
        _LineTo(pDC, pRect->right - 1, pRect->bottom - 1);
      }
      else {
        TMoveTo(pDC, pRect->left, pRect->top + (firstTab == TRUE ? 0 : 1));
        _LineTo(pDC, pRect->left, pRect->bottom - 1);
        _LineTo(pDC, pRect->right - 1, pRect->bottom - 1);
        _LineTo(pDC, pRect->right - 1, pRect->top);
      }
    }
    else { // tab isn't selected.
      BOOL hover = (s->m_hHoverTab == hTab) && (s->m_hPushedTab < 0);
      BOOL disabled = s->m_vtrtabs[hTab].disable;
      //
      COLORREF clrBorderHover = GetTabBorderColor2(s, TRUE);
      COLORREF clrBackLight = GetTabGradientLightColor(s, hover, disabled);
      COLORREF clrBackDark = GetTabGradientDarkColor(s, hover, disabled);
      int tabSel = s->m_hCurTab;
      int cmpRes = (tabSel >= 0 ? CmpTabs(s, hTab, tabSel) : -1);
      RECT rc = (*pRect);

      if (top == TRUE) {
        DeflateRect4(&rc, 0, 2, 0, 1);
        // draw back.
        DrawGradient(pDC, &rc, FALSE, clrBackLight, clrBackDark);

        if (cmpRes < 0) { // before selected tab or there isn't selected tab.
          // draw tab border.
          if (hover == TRUE && disabled == FALSE) {
            draw_line(pDC, rc.left, rc.bottom - 1, rc.left, rc.top, clrBorderHover);
            draw_line(pDC, rc.left, rc.top, rc.right, rc.top, clrBorderHover);
          }
          else {
            TMoveTo(pDC, rc.left, rc.bottom - 1);
            _LineTo(pDC, rc.left, rc.top);
            _LineTo(pDC, rc.right, rc.top);
          }

          // draw outline.
          draw_line(pDC, rc.left + 1, rc.top + 1, rc.left + 1, rc.bottom, clrBackLight);
        }
        else { // after selected tab.
          // draw tab border.
          if (hover == TRUE && disabled == FALSE) {
            draw_line(pDC, rc.left, rc.top, rc.right - 1, rc.top, clrBorderHover);
            draw_line(pDC, rc.right - 1, rc.top, rc.right - 1, rc.bottom, clrBorderHover);
          }
          else {
            TMoveTo(pDC, rc.left, rc.top);
            _LineTo(pDC, rc.right - 1, rc.top);
            _LineTo(pDC, rc.right - 1, rc.bottom);
          }

          // draw outline.
          draw_line(pDC, rc.right - 2, rc.top + 1, rc.right - 2, rc.bottom, clrBackLight);
        }
      }
      else { // bottom.
        DeflateRect4(&rc, 0, 1, 0, 2);
        // draw back.
        DrawGradient(pDC, &rc, FALSE, clrBackDark, clrBackLight);

        if (cmpRes < 0) { // before selected tab or there isn't selected tab.
          // draw tab border.
          if (hover == TRUE && disabled == FALSE) {
            draw_line(pDC, rc.left, rc.top, rc.left, rc.bottom - 1, clrBorderHover);
            draw_line(pDC, rc.left, rc.bottom - 1, rc.right, rc.bottom - 1, clrBorderHover);
          }
          else {
            TMoveTo(pDC, rc.left, rc.top);
            _LineTo(pDC, rc.left, rc.bottom - 1);
            _LineTo(pDC, rc.right, rc.bottom - 1);
          }

          // draw outline.
          draw_line(pDC, rc.left + 1, rc.top, rc.left + 1, rc.bottom - 1, clrBackLight);
        }
        else { // after selected tab.
          // draw tab border.
          if (hover == TRUE && disabled == FALSE) {
            draw_line(pDC, rc.left, rc.bottom - 1, rc.right - 1, rc.bottom - 1, clrBorderHover);
            draw_line(pDC, rc.right - 1, rc.bottom - 1, rc.right - 1, rc.top - 1, clrBorderHover);
          }
          else {
            TMoveTo(pDC, rc.left, rc.bottom - 1);
            _LineTo(pDC, rc.right - 1, rc.bottom - 1);
            _LineTo(pDC, rc.right - 1, rc.top - 1);
          }

          // draw outline.
          draw_line(pDC, rc.right - 2, rc.top, rc.right - 2, rc.bottom - 1, clrBackLight);
        }
      }

      // draw closing line.
      if (cmpRes < 0) { // before selected tab or there isn't selected tab.
        BOOL lastTab = ((count - 1) == hTab);

        if (lastTab == TRUE) {
          draw_line(pDC, rc.right - 1, rc.top, rc.right - 1, rc.bottom, _clr);
        }
      }
    }

    //_clr = (pOldPen);
    //DeleteObject(penBorder);
    return 0;
  }

  case TabCtrlStyle_VS2010_client: {
    if (s->m_vtrtabs[hTab].disable == FALSE) {
      BOOL active = s->m_bActive;
      BOOL select = (s->m_hCurTab == hTab);
      BOOL hover = (s->m_hHoverTab == hTab);
      TabCtrlStyle_VS2010_client_DrawTabBack(s, pDC, pRect, active, select, hover);
    }

    return 0;
  }

  case TabCtrlStyle_VS2010_client_custom1: {
    if (s->m_vtrtabs[hTab].disable == FALSE) {
      BOOL select = (s->m_hCurTab == hTab);
      BOOL hover = (s->m_hHoverTab == hTab);
      TabCtrlStyle_VS2010_client_DrawTabBack(s, pDC, pRect, TRUE, select, hover);
    }

    return 0;
  }

  case TabCtrlStyle_VS2010_client_custom2: {
    if (s->m_vtrtabs[hTab].disable == FALSE) {
      BOOL select = (s->m_hCurTab == hTab);
      BOOL hover = (s->m_hHoverTab == hTab);
      TabCtrlStyle_VS2010_client_DrawTabBack(s, pDC, pRect, FALSE, select, hover);
    }

    return 0;
  }

  case TabCtrlStyle_VS2010_bars: {
    if (s->m_vtrtabs[hTab].disable == FALSE) {
      BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
      BOOL select = (s->m_hCurTab == hTab);
      BOOL hover = (s->m_hHoverTab == hTab);
      RECT rc = (*pRect);

      if (top == TRUE) {
        if (select == TRUE) {
          FillSolidRectRC(pDC, &rc, _RGB(255, 255, 255)); // back.
          // left corner.
          _SetPixel(pDC, rc.left, rc.top, _RGB(103, 116, 138));
          _SetPixel(pDC, rc.left + 1, rc.top, _RGB(221, 224, 228));
          _SetPixel(pDC, rc.left, rc.top + 1, _RGB(197, 202, 210));
          // right corner.
          _SetPixel(pDC, rc.right - 1, rc.top, _RGB(103, 116, 138));
          _SetPixel(pDC, rc.right - 2, rc.top, _RGB(221, 224, 228));
          _SetPixel(pDC, rc.right - 1, rc.top + 1, _RGB(197, 202, 210));
        }
        else if (hover == TRUE) {
          // draw border.
          COLORREF _clr = (_RGB(155, 167, 183));
          TMoveTo(pDC, rc.left, rc.top);
          _LineTo(pDC, rc.left, rc.bottom - 1);
          _LineTo(pDC, rc.right - 1, rc.bottom - 1);
          _LineTo(pDC, rc.right - 1, rc.top);
          _LineTo(pDC, rc.left, rc.top);
          //_clr = (pOldPen);
          // draw back.
          DeflateRect(&rc, 1, 1);
          DrawGradient(pDC, &rc, FALSE, _RGB(76, 92, 116), _RGB(111, 119, 118));
          InflateRect(&rc, 1, 1);
          // left corner.
          _SetPixel(pDC, rc.left, rc.top, _RGB(50, 65, 93));
          _SetPixel(pDC, rc.left + 1, rc.top, _RGB(120, 135, 157));
          _SetPixel(pDC, rc.left, rc.top + 1, _RGB(117, 130, 150));
          _SetPixel(pDC, rc.left + 1, rc.top + 1, _RGB(115, 126, 139));
          // right corner.
          _SetPixel(pDC, rc.right - 1, rc.top, _RGB(50, 65, 93));
          _SetPixel(pDC, rc.right - 2, rc.top, _RGB(120, 135, 157));
          _SetPixel(pDC, rc.right - 1, rc.top + 1, _RGB(117, 130, 150));
          _SetPixel(pDC, rc.right - 2, rc.top + 1, _RGB(115, 126, 139));
          //DeleteObject(pen);
        }
      }
      else { // bottom.
        if (select == TRUE) {
          FillSolidRectRC(pDC, &rc, _RGB(255, 255, 255)); // back.
          // left corner.
          _SetPixel(pDC, rc.left, rc.bottom - 1, _RGB(103, 116, 138));
          _SetPixel(pDC, rc.left + 1, rc.bottom - 1, _RGB(221, 224, 228));
          _SetPixel(pDC, rc.left, rc.bottom - 2, _RGB(197, 202, 210));
          // right corner.
          _SetPixel(pDC, rc.right - 1, rc.bottom - 1, _RGB(103, 116, 138));
          _SetPixel(pDC, rc.right - 2, rc.bottom - 1, _RGB(221, 224, 228));
          _SetPixel(pDC, rc.right - 1, rc.bottom - 2, _RGB(197, 202, 210));
        }
        else if (hover == TRUE) {
          // draw border.
          COLORREF pen = (_RGB(155, 167, 183));
          COLORREF _clr = (pen);
          TMoveTo(pDC, rc.left, rc.top);
          _LineTo(pDC, rc.left, rc.bottom - 1);
          _LineTo(pDC, rc.right - 1, rc.bottom - 1);
          _LineTo(pDC, rc.right - 1, rc.top);
          _LineTo(pDC, rc.left, rc.top);
          //_clr = (pOldPen);
          // draw back.
          DeflateRect(&rc, 1, 1);
          DrawGradient(pDC, &rc, FALSE, _RGB(111, 119, 118), _RGB(76, 92, 116));
          InflateRect(&rc, 1, 1);
          // left corner.
          _SetPixel(pDC, rc.left, rc.bottom - 1, _RGB(50, 65, 93));
          _SetPixel(pDC, rc.left + 1, rc.bottom - 1, _RGB(120, 135, 157));
          _SetPixel(pDC, rc.left, rc.bottom - 2, _RGB(117, 130, 150));
          _SetPixel(pDC, rc.left + 1, rc.bottom - 2, _RGB(115, 126, 139));
          // right corner.
          _SetPixel(pDC, rc.right - 1, rc.bottom - 1, _RGB(50, 65, 93));
          _SetPixel(pDC, rc.right - 2, rc.bottom - 1, _RGB(120, 135, 157));
          _SetPixel(pDC, rc.right - 1, rc.bottom - 2, _RGB(117, 130, 150));
          _SetPixel(pDC, rc.right - 2, rc.bottom - 2, _RGB(115, 126, 139));
          //DeleteObject(pen);
        }
      }
    }

    return 0;
  }
  }

  {
    COLORREF clrBorder = s->m_TabBorderColor;
    COLORREF clrBack = s->m_TabSelectedBackColor;
    RECT rcTabPadding;
    int topMargin;
    int bottomMargin;

    if (s->m_hCurTab != hTab) {
      if (s->m_hHoverTab == hTab && s->m_hPushedTab < 0 && s->m_vtrtabs[hTab].disable == FALSE) { // highlight tab.
        RECT rc = (*pRect);

        if (s->m_Layout == TAB_LAYOUT_TOP) {
          DeflateRect4(&rc, 2, 1, 1, 1);
          DrawHalfRoundFrame(pDC, &rc, SIDE_TOP, 1, clrBorder, clrBack);
          DeflateRect4(&rc, 1, RCH(&rc) / 2, 1, 0);
          DrawGradient(pDC, &rc, FALSE, clrBack, clrBorder);
        }
        else {
          DeflateRect4(&rc, 2, 1, 1, 1);
          DrawHalfRoundFrame(pDC, &rc, SIDE_BOTTOM, 1, clrBorder, clrBack);
        }
      }

      rcTabPadding = GetTabPadding(s);
      topMargin = rcTabPadding.top + 1;
      bottomMargin = rcTabPadding.bottom + 1;

      if (s->m_Layout == TAB_LAYOUT_TOP) {
        bottomMargin += 1; /*separator*/
      }
      else {
        topMargin += 1; /*separator*/
      }

      if (hTab == (0)) { // this is first tab.
        draw_line(pDC, pRect->left, pRect->top + topMargin, pRect->left, pRect->bottom - bottomMargin, clrBorder);
      }

      draw_line(pDC, pRect->right, pRect->top + topMargin, pRect->right, pRect->bottom - bottomMargin, clrBorder);
    }
    else {
      DrawHalfRoundFrame(pDC, pRect, (s->m_Layout == TAB_LAYOUT_TOP ? SIDE_TOP : SIDE_BOTTOM), 2, clrBorder, clrBack);
    }

    return 0;
  }
}
COLORREF GetTabTextColor(const TabCtrl* s, int hTab)
{
  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2003_base:
  case TabCtrlStyle_VS2003_client:
  case TabCtrlStyle_VS2003_client_custom1:
  case TabCtrlStyle_VS2003_bars:
  case TabCtrlStyle_VS2003_bars_custom1: {
    if (s->m_hCurTab == hTab)
      if (s->m_bWatchActivityCtrl == FALSE || s->m_bActive == TRUE) {
        return _GetSysColor(COLOR_BTNTEXT);
      }
      else {
        return PixelAlpha1(_GetSysColor(COLOR_BTNSHADOW), _RGB(0, 0, 0), 55);
      }

    if (s->m_vtrtabs[hTab].disable == FALSE) {
      return PixelAlpha1(_GetSysColor(COLOR_BTNSHADOW), _RGB(0, 0, 0), 55); // it is approximate color (VS2003 uses some another way).
    }

    //TabCtrlStyle_base::GetTabTextColor(s,hTab));
  }
  break;

  case TabCtrlStyle_VS2008_bars_base:
  case TabCtrlStyle_VS2008_bars_custom1_base:
  case TabCtrlStyle_VS2008_bars_classic:
  case TabCtrlStyle_VS2008_bars_classic_custom1:
  case TabCtrlStyle_VS2008_bars_blue:
  case TabCtrlStyle_VS2008_bars_blue_custom1:
  case TabCtrlStyle_VS2008_bars_silver :
  case TabCtrlStyle_VS2008_bars_silver_custom1:
  case TabCtrlStyle_VS2008_bars_olive:
  case TabCtrlStyle_VS2008_bars_olive_custom1: {
    if (s->m_vtrtabs[hTab].disable == TRUE) { // disabled.
      //return TabCtrlStyle_base::GetTabTextColor(s,hTab);
      break;
    }
    else if (s->m_hCurTab == hTab) { // selected.
      return _GetSysColor(COLOR_BTNTEXT);
    }
    else {
      return PixelAlpha1(_GetSysColor(COLOR_BTNSHADOW), _RGB(0, 0, 0), 55);
    }
  }

  case TabCtrlStyle_VS2010_client:
  case TabCtrlStyle_VS2010_client_custom1:
  case TabCtrlStyle_VS2010_client_custom2: {
    return (s->m_hCurTab == hTab ? _RGB(13, 0, 5) : _RGB(248, 255, 255));
  }

  case TabCtrlStyle_VS2010_bars: {
    return (s->m_hCurTab == hTab ? _RGB(13, 0, 5) : _RGB(248, 255, 255));
  }
  };

  return (s->m_vtrtabs[hTab].disable == FALSE ? _GetSysColor(COLOR_BTNTEXT) : _GetSysColor(COLOR_GRAYTEXT));
}
RECT GetWndsAreaPadding(const TabCtrl* s)
{
  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2003_client:
  case TabCtrlStyle_VS2003_client_custom1: {
    BOOL bHideSingleTab = (s->m_vtrtabsLen == 1 && s->m_bHideSingleTab == TRUE);

    if (bHideSingleTab == FALSE) {
      return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(2, 3, 2, 2) : iRECT(2, 2, 2, 3));
    }
    else {
      return iRECT(2, 2, 2, 2);
    }
  }

  case TabCtrlStyle_VS2003_bars_custom1: {
    BOOL bHideSingleTab = (s->m_vtrtabsLen == 1 && s->m_bHideSingleTab == TRUE);

    if (bHideSingleTab == FALSE) {
      return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(1, 0, 1, 1) : iRECT(1, 1, 1, 0));
    }
    else {
      return iRECT(1, 1, 1, 1);
    }
  }

  case TabCtrlStyle_VS2008_client_base:
  case TabCtrlStyle_VS2008_client_classic:
  case TabCtrlStyle_VS2008_client_blue:
  case TabCtrlStyle_VS2008_client_silver:
  case TabCtrlStyle_VS2008_client_olive: {
    BOOL bHideSingleTab = (s->m_vtrtabsLen == 1 && s->m_bHideSingleTab == TRUE);

    if (bHideSingleTab == FALSE) {
      return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(5, 3, 5, 5) : iRECT(5, 5, 5, 3));
    }
    else {
      return iRECT(5, 5, 5, 5);
    }
  }

  case TabCtrlStyle_VS2008_bars_custom1_base: {
    BOOL bHideSingleTab = (s->m_vtrtabsLen == 1 && s->m_bHideSingleTab == TRUE);

    if (bHideSingleTab == FALSE) {
      return (s->m_Layout == TAB_LAYOUT_TOP ? iRECT(1, 0, 1, 1) : iRECT(1, 1, 1, 0));
    }
    else {
      return iRECT(1, 1, 1, 1);
    }
  }

  case TabCtrlStyle_VS2010_client: {
    return iRECT(0, 4, 0, 4);
  }
  }

  return iRECT(0, 0, 0, 0);
}
int TabCtrlStyle_VS2010_client_DrawWndsAreaBack(const TabCtrl* s, HDDC pDC, const RECT* pRect, BOOL top, BOOL active)
{
  RECT rc;
  int firstTab = (s->m_vtrtabsLen > 0 ? (0) : -1);
  BOOL selectFirstTab = (s->m_hCurTab == firstTab);
  BOOL scaleMode = (s->m_Behavior == TAB_BEHAVIOR_SCALE);
  COLORREF clr = (active == TRUE ? _RGB(255, 232, 166) : _RGB(206, 212, 223));
  // draw top.
  SetRect(&rc, pRect->left, pRect->top, pRect->right, pRect->top + 4);
  FillSolidRectRC(pDC, &rc, clr);

  if (top == FALSE || scaleMode == TRUE) {
    // left corner.
    if (top == FALSE || selectFirstTab == FALSE || (s->m_vtrtabsLen == 1 && s->m_bHideSingleTab == TRUE)) {
      if (top == TRUE && s->m_hHoverTab == firstTab) { // tab is highlight.
        if (active == TRUE) {
          _SetPixel(pDC, rc.left, rc.top, _RGB(175, 180, 180));
          _SetPixel(pDC, rc.left + 1, rc.top, _RGB(205, 194, 152));
          _SetPixel(pDC, rc.left, rc.top + 1, _RGB(227, 214, 171));
        }
        else {
          _SetPixel(pDC, rc.left, rc.top, _RGB(165, 176, 191));
          _SetPixel(pDC, rc.left + 1, rc.top, _RGB(170, 179, 193));
          _SetPixel(pDC, rc.left, rc.top + 1, _RGB(192, 199, 212));
        }
      }// tab isn't highlight.
      else if (active == TRUE) {
        _SetPixel(pDC, rc.left, rc.top, _RGB(86, 95, 105));
        _SetPixel(pDC, rc.left + 1, rc.top, _RGB(198, 188, 149));
        _SetPixel(pDC, rc.left, rc.top + 1, _RGB(195, 184, 144));
      }
      else {
        _SetPixel(pDC, rc.left, rc.top, _RGB(76, 91, 116));
        _SetPixel(pDC, rc.left + 1, rc.top, _RGB(163, 173, 190));
        _SetPixel(pDC, rc.left, rc.top + 1, _RGB(160, 169, 185));
      }
    }

    // right corner.
    if (active == TRUE) {
      _SetPixel(pDC, rc.right - 1, rc.top, _RGB(100, 106, 109));
      _SetPixel(pDC, rc.right - 2, rc.top, _RGB(205, 192, 148));
      _SetPixel(pDC, rc.right - 1, rc.top + 1, _RGB(195, 184, 144));
    }
    else {
      _SetPixel(pDC, rc.right - 1, rc.top, _RGB(87, 100, 124));
      _SetPixel(pDC, rc.right - 2, rc.top, _RGB(168, 176, 192));
      _SetPixel(pDC, rc.right - 1, rc.top + 1, _RGB(160, 169, 185));
    }
  }

  // draw bottom.
  SetRect(&rc, pRect->left, pRect->bottom - 4, pRect->right, pRect->bottom);
  FillSolidRectRC(pDC, &rc, clr);

  if (top == TRUE || scaleMode == TRUE) {
    // left corner.
    if (top == TRUE || selectFirstTab == FALSE || (s->m_vtrtabsLen == 1 && s->m_bHideSingleTab == TRUE)) {
      if (top == FALSE && s->m_hHoverTab == firstTab) { // tab is highlight.
        if (active == TRUE) {
          _SetPixel(pDC, rc.left, rc.bottom - 1, _RGB(175, 180, 180));
          _SetPixel(pDC, rc.left + 1, rc.bottom - 1, _RGB(205, 194, 152));
          _SetPixel(pDC, rc.left, rc.bottom - 2, _RGB(227, 214, 171));
        }
        else {
          _SetPixel(pDC, rc.left, rc.bottom - 1, _RGB(165, 176, 191));
          _SetPixel(pDC, rc.left + 1, rc.bottom - 1, _RGB(170, 179, 193));
          _SetPixel(pDC, rc.left, rc.bottom - 2, _RGB(192, 199, 212));
        }
      }// tab isn't highlight.
      else if (active == TRUE) {
        _SetPixel(pDC, rc.left, rc.bottom - 1, _RGB(86, 95, 105));
        _SetPixel(pDC, rc.left + 1, rc.bottom - 1, _RGB(198, 188, 149));
        _SetPixel(pDC, rc.left, rc.bottom - 2, _RGB(195, 184, 144));
      }
      else {
        _SetPixel(pDC, rc.left, rc.bottom - 1, _RGB(76, 91, 116));
        _SetPixel(pDC, rc.left + 1, rc.bottom - 1, _RGB(163, 173, 190));
        _SetPixel(pDC, rc.left, rc.bottom - 2, _RGB(160, 169, 185));
      }
    }

    // right corner.
    if (active == TRUE) {
      _SetPixel(pDC, rc.right - 1, rc.bottom - 1, _RGB(100, 106, 109));
      _SetPixel(pDC, rc.right - 2, rc.bottom - 1, _RGB(205, 192, 148));
      _SetPixel(pDC, rc.right - 1, rc.bottom - 2, _RGB(195, 184, 144));
    }
    else {
      _SetPixel(pDC, rc.right - 1, rc.bottom - 1, _RGB(87, 100, 124));
      _SetPixel(pDC, rc.right - 2, rc.bottom - 1, _RGB(168, 176, 192));
      _SetPixel(pDC, rc.right - 1, rc.bottom - 2, _RGB(160, 169, 185));
    }
  }

  return 0;
}
int DrawWndsAreaBack(const TabCtrl* s, HDDC pDC, const RECT* pRect)
{
  int _x0, _y0;

  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2003_client:
  case TabCtrlStyle_VS2003_client_custom1: {
    if (s->m_vtrtabsLen == 0 || s->m_hCurTab < 0) {
      break;
    }
    else {
      RECT rc = (*pRect), rc2;
      FillSolidRectRC(pDC, &rc, s->m_WndsAreaBackColor);
      rc2 = GetWndsAreaPadding(s);
      DeflateRectR(&rc, &rc2);
      InflateRect(&rc, 1, 1);

      if (IsRectEmpty(&rc) == 0) {
        COLORREF color = s->m_BorderColor;
        DrawRectRC(pDC, &rc, color); // rect around child window.
      }
    }

    return 0;
  }

  case TabCtrlStyle_VS2003_bars_custom1: {
    int count = s->m_vtrtabsLen;

    if (count == 0 || s->m_hCurTab < 0) {
      break;
    }
    else {
      COLORREF clrBorder = s->m_TabBorderColor;
      BOOL bHideSingleTab = (count == 1 && s->m_bHideSingleTab == TRUE);

      if (bHideSingleTab == TRUE) {
        DrawRectRC(pDC, pRect, clrBorder);
      }
      else {
        BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
        COLORREF penBorder = (clrBorder);
        COLORREF _clr = (penBorder);

        if (top == TRUE) {
          TMoveTo(pDC, pRect->left, pRect->top);
          _LineTo(pDC, pRect->left, pRect->bottom - 1);
          _LineTo(pDC, pRect->right - 1, pRect->bottom - 1);
          _LineTo(pDC, pRect->right - 1, pRect->top - 1);
        }
        else {
          TMoveTo(pDC, pRect->left, pRect->bottom - 1);
          _LineTo(pDC, pRect->left, pRect->top);
          _LineTo(pDC, pRect->right - 1, pRect->top);
          _LineTo(pDC, pRect->right - 1, pRect->bottom);
        }

        //_clr = (pOldPen);
        //DeleteObject(penBorder);
      }
    }

    return 0;
  }

  case TabCtrlStyle_VS2008_client_base:
  case TabCtrlStyle_VS2008_client_classic:
  case TabCtrlStyle_VS2008_client_blue:
  case TabCtrlStyle_VS2008_client_silver:
  case TabCtrlStyle_VS2008_client_olive: {
    int count = s->m_vtrtabsLen;

    if (count == 0 || s->m_hCurTab < 0) {
      break;
    }
    else {
      BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
      BOOL bHideSingleTab = (count == 1 && s->m_bHideSingleTab == TRUE);
      COLORREF clrBorder = GetTabBorderColor3(s, TRUE, FALSE);
      COLORREF clrOutline = GetTabOutlineColor(s, TRUE, FALSE, FALSE, TRUE);
      COLORREF clrBack = GetTabGradientDarkColor4(s, TRUE, FALSE, FALSE);
      COLORREF clrBackOut = s->m_WndsAreaBackColor;
      COLORREF penBorder = (clrBorder);
      COLORREF penOutline = (clrOutline);
      COLORREF penBack = (clrBack);
      RECT rc = (*pRect);
      COLORREF _clr = (penBorder);

      if (bHideSingleTab == TRUE) {
        // outside border.
        DrawBeveledRect(pDC, &rc, 2);
        // left-top corner.
        _SetPixel(pDC, rc.left, rc.top, clrBackOut);
        _SetPixel(pDC, rc.left + 1, rc.top, clrBackOut);
        _SetPixel(pDC, rc.left, rc.top + 1, clrBackOut);
        // right-top corner.
        _SetPixel(pDC, rc.right - 1, rc.top, clrBackOut);
        _SetPixel(pDC, rc.right - 2, rc.top, clrBackOut);
        _SetPixel(pDC, rc.right - 1, rc.top + 1, clrBackOut);
        // left-bottom corner.
        _SetPixel(pDC, rc.left, rc.bottom - 1, clrBackOut);
        _SetPixel(pDC, rc.left + 1, rc.bottom - 1, clrBackOut);
        _SetPixel(pDC, rc.left, rc.bottom - 2, clrBackOut);
        // right-bottom corner.
        _SetPixel(pDC, rc.right - 1, rc.bottom - 1, clrBackOut);
        _SetPixel(pDC, rc.right - 2, rc.bottom - 1, clrBackOut);
        _SetPixel(pDC, rc.right - 1, rc.bottom - 2, clrBackOut);
        // inside border.
        DeflateRect(&rc, 4, 4);

        if (IsRectEmpty(&rc) == 0) {
          DrawRectR(pDC, &rc);
        }
      }
      else if (top == TRUE) {
        // outside border.
        TMoveTo(pDC, rc.left, rc.top);
        _LineTo(pDC, rc.left, rc.bottom - 3);
        _LineTo(pDC, rc.left + 2, rc.bottom - 1);
        _LineTo(pDC, rc.right - 3, rc.bottom - 1);
        _LineTo(pDC, rc.right - 1, rc.bottom - 3);
        _LineTo(pDC, rc.right - 1, rc.top - 1);
        // left-bottom corner.
        _SetPixel(pDC, rc.left, rc.bottom - 1, clrBackOut);
        _SetPixel(pDC, rc.left + 1, rc.bottom - 1, clrBackOut);
        _SetPixel(pDC, rc.left, rc.bottom - 2, clrBackOut);
        // right-bottom corner.
        _SetPixel(pDC, rc.right - 1, rc.bottom - 1, clrBackOut);
        _SetPixel(pDC, rc.right - 2, rc.bottom - 1, clrBackOut);
        _SetPixel(pDC, rc.right - 1, rc.bottom - 2, clrBackOut);
        // inside border.
        DeflateRect4(&rc, 4, 2, 4, 4);

        if (IsRectEmpty(&rc) == 0) {
          DrawRectR(pDC, &rc);
        }
      }
      else {
        // outside border.
        TMoveTo(pDC, rc.left, rc.bottom - 1);
        _LineTo(pDC, rc.left, rc.top + 2);
        _LineTo(pDC, rc.left + 2, rc.top);
        _LineTo(pDC, rc.right - 3, rc.top);
        _LineTo(pDC, rc.right - 1, rc.top + 2);
        _LineTo(pDC, rc.right - 1, rc.bottom);
        // left-bottom corner.
        _SetPixel(pDC, rc.left, rc.top, clrBackOut);
        _SetPixel(pDC, rc.left + 1, rc.top, clrBackOut);
        _SetPixel(pDC, rc.left, rc.top + 1, clrBackOut);
        // right-bottom corner.
        _SetPixel(pDC, rc.right - 1, rc.top, clrBackOut);
        _SetPixel(pDC, rc.right - 2, rc.top, clrBackOut);
        _SetPixel(pDC, rc.right - 1, rc.top + 1, clrBackOut);
        // inside border.
        DeflateRect4(&rc, 4, 4, 4, 2);

        if (IsRectEmpty(&rc) == 0) {
          DrawRectR(pDC, &rc);
        }
      }

      // draw back.
      _clr = (penBack);
      InflateRect(&rc, 1, 1);

      if (IsRectEmpty(&rc) == 0) {
        DrawRectR(pDC, &rc);
      }

      InflateRect(&rc, 1, 1);

      if (IsRectEmpty(&rc) == 0) {
        DrawRectR(pDC, &rc);
      }

      // draw outline;
      _clr = (penOutline);

      if (bHideSingleTab == TRUE) {
        InflateRect(&rc, 1, 1);

        if (IsRectEmpty(&rc) == 0) {
          TMoveTo(pDC, rc.left, rc.top + 1);
          _LineTo(pDC, rc.left, rc.bottom - 1);
          TMoveTo(pDC, rc.left + 1, rc.bottom - 1);
          _LineTo(pDC, rc.right - 1, rc.bottom - 1);
          TMoveTo(pDC, rc.right - 1, rc.bottom - 2);
          _LineTo(pDC, rc.right - 1, rc.top);
          TMoveTo(pDC, rc.right - 2, rc.top);
          _LineTo(pDC, rc.left, rc.top);
        }
      }
      else if (top == TRUE) {
        InflateRect4(&rc, 1, 0, 1, 1);

        if (IsRectEmpty(&rc) == 0) {
          TMoveTo(pDC, rc.left, rc.top);
          _LineTo(pDC, rc.left, rc.bottom - 1);
          TMoveTo(pDC, rc.left + 1, rc.bottom - 1);
          _LineTo(pDC, rc.right - 1, rc.bottom - 1);
          TMoveTo(pDC, rc.right - 1, rc.bottom - 2);
          _LineTo(pDC, rc.right - 1, rc.top - 1);
        }
      }
      else {
        InflateRect4(&rc, 1, 1, 1, 0);

        if (IsRectEmpty(&rc) == 0) {
          TMoveTo(pDC, rc.left, rc.bottom - 1);
          _LineTo(pDC, rc.left, rc.top);
          TMoveTo(pDC, rc.left + 1, rc.top);
          _LineTo(pDC, rc.right - 1, rc.top);
          TMoveTo(pDC, rc.right - 1, rc.top + 1);
          _LineTo(pDC, rc.right - 1, rc.bottom);
        }
      }

      //_clr = (pOldPen);
      //DeleteObject(penBorder);
      //DeleteObject(penOutline);
      //DeleteObject(penBack);
    }

    return 0;
  }

  case TabCtrlStyle_VS2008_bars_custom1_base: {
    int count = s->m_vtrtabsLen;

    if (count == 0 || s->m_hCurTab < 0) {
      break;
    }
    else {
      COLORREF clrBorder = GetTabBorderColor2(s, FALSE);
      BOOL bHideSingleTab = (count == 1 && s->m_bHideSingleTab == TRUE);

      if (bHideSingleTab == TRUE) {
        DrawRectRC(pDC, pRect, clrBorder);
      }
      else {
        BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
        COLORREF penBorder = (clrBorder);
        COLORREF _clr = (penBorder);

        if (top == TRUE) {
          TMoveTo(pDC, pRect->left, pRect->top);
          _LineTo(pDC, pRect->left, pRect->bottom - 1);
          _LineTo(pDC, pRect->right - 1, pRect->bottom - 1);
          _LineTo(pDC, pRect->right - 1, pRect->top - 1);
        }
        else {
          TMoveTo(pDC, pRect->left, pRect->bottom - 1);
          _LineTo(pDC, pRect->left, pRect->top);
          _LineTo(pDC, pRect->right - 1, pRect->top);
          _LineTo(pDC, pRect->right - 1, pRect->bottom);
        }

        //_clr = (pOldPen);
        //DeleteObject(penBorder);
      }
    }

    return 0;
  }

  case TabCtrlStyle_VS2010_client: {
    if (s->m_vtrtabsLen == 0 || s->m_hCurTab < 0) {
      break;
    }
    else {
      BOOL active = s->m_bActive;
      BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
      TabCtrlStyle_VS2010_client_DrawWndsAreaBack(s, pDC, pRect, top, active);
    }

    return 0;
  }

  case TabCtrlStyle_VS2010_client_custom1: {
    if (s->m_vtrtabsLen == 0 || s->m_hCurTab < 0) {
      break;
    }
    else {
      BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
      TabCtrlStyle_VS2010_client_DrawWndsAreaBack(s, pDC, pRect, top, TRUE);
    }

    return 0;
  }

  case TabCtrlStyle_VS2010_client_custom2: {
    if (s->m_vtrtabsLen == 0 || s->m_hCurTab < 0) {
      break;
    }
    else {
      BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
      TabCtrlStyle_VS2010_client_DrawWndsAreaBack(s, pDC, pRect, top, FALSE);
    }

    return 0;
  }
  }

  {
    if (s->m_vtrtabsLen == 0 || s->m_hCurTab < 0) {
      FillSolidRectRC(pDC, pRect, s->m_EmptyWndsAreaBackColor);
    }
    else {
      FillSolidRectRC(pDC, pRect, s->m_TabSelectedBackColor);
    }

    return 0;
  }
}
int DrawTabText(const TabCtrl* s, HDDC pDC, int hTab, const RECT* pRect)
{
  RECT rc = *pRect;
  LPCTSTR text = s->m_vtrtabs[hTab].text;
  COLORREF _clr = GetTabTextColor(s, hTab);
  draw_textR(pDC, text, strlen(text), &rc, DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS, _clr);
  return 0;
}
int TabCtrlStyle_base_DrawTabContext(const TabCtrl* s, HDDC pDC, int hTab, const RECT* pRect)
{
  RECT rcTabPadding = GetTabPadding(s);
  RECT rc = (*pRect);
  BOOL disable = s->m_vtrtabs[hTab].disable;
  LPCTSTR text = s->m_vtrtabs[hTab].text;
  int textWidth;
  const img_t* pImageList;
  DeflateRectR(&rc, &rcTabPadding);
  textWidth = FontGetTextSize(text, strlen(text)).w;
  pImageList = (disable == FALSE ? s->m_ImageList : s->m_ImageListDisabled);

  // draw image.
  if ((s->m_vtrtabs[hTab].image) > -1 && pImageList != NULL) {
    SIZE szImage;
    int iContentWidth;
    GetIconSize(disable ? s->m_ImageListDisabled : s->m_ImageList, &szImage);
    iContentWidth = szImage.w + s->m_nTabImageTextGap + textWidth;
    rc.left += (iContentWidth < RCW(&rc) ? (RCW(&rc) - iContentWidth) / 2 : 0);
    //DrawTabImage(s, pDC, hTab, &rc);
    {
      RECT* pRect = &rc;
      SIZE szImage;
      const img_t* pImageList = (s->m_vtrtabs[hTab].disable) ? s->m_ImageListDisabled : s->m_ImageList;
      GetIconSize(pImageList, &szImage);
      //ImageList_Draw(pImageList, s->m_vtrtabs[hTab].image, pDC, pRect->left, (pRect->top + pRect->bottom - szImage.h + 1) / 2, ILD_TRANSPARENT);
      DoPaintImageList(pDC, pImageList, s->m_vtrtabs[hTab].image, pRect->left, (pRect->top + pRect->bottom - szImage.h + 1) / 2);
    }
    rc.left += szImage.w + s->m_nTabImageTextGap;
  }
  else {
    rc.left += (textWidth < RCW(&rc) ? (RCW(&rc) - textWidth) / 2 : 0);
  }

  // draw text.
  if (IsRectEmpty(&rc) == FALSE) {
    DrawTabText(s, pDC, hTab, &rc);
  }

  return 0;
}
int DrawTabContext(const TabCtrl* s, HDDC pDC, int hTab, const RECT* pRect)
{
  RECT rc = (*pRect);

  switch (s->m_nStyle) {
  case TabCtrlStyle_VS2008_client_base:
  case TabCtrlStyle_VS2008_client_classic:
  case TabCtrlStyle_VS2008_client_blue:
  case TabCtrlStyle_VS2008_client_silver:
  case TabCtrlStyle_VS2008_client_olive: {
    if (s->m_hCurTab != hTab) {
      if (s->m_Layout == TAB_LAYOUT_TOP) {
        rc.top += 2;
      }
      else {
        rc.bottom -= 2;
      }
    }

    rc.left += GetTabExtraWidth(s, hTab);
    break;
  }

  case TabCtrlStyle_VS2008_bars_base:
  case TabCtrlStyle_VS2008_bars_custom1_base:
  case TabCtrlStyle_VS2008_bars_classic:
  case TabCtrlStyle_VS2008_bars_classic_custom1:
  case TabCtrlStyle_VS2008_bars_blue:
  case TabCtrlStyle_VS2008_bars_blue_custom1:
  case TabCtrlStyle_VS2008_bars_silver :
  case TabCtrlStyle_VS2008_bars_silver_custom1:
  case TabCtrlStyle_VS2008_bars_olive:
  case TabCtrlStyle_VS2008_bars_olive_custom1: {
    RECT rc = (*pRect);

    if (s->m_hCurTab == hTab) {
      if (s->m_Layout == TAB_LAYOUT_TOP) {
        rc.top -= 2;
      }
      else {
        rc.bottom += 2;
      }
    }

    break;
  }
  }

  return TabCtrlStyle_base_DrawTabContext(s, pDC, hTab, &rc);
}
BOOL TabCtrlStyle_VS2008_client_base_HitTest(const TabCtrl* s, int hTab, BOOL top, POINT point)
{
  RECT rc = s->m_vtrtabs[hTab].rc;
  DPOINT pts[8];
  GetTabOutline(s, hTab, &rc, top, pts, NULL);
  return point_in_poly(pts, 8, point.x, point.y);
}
int TabCtrl_HitTest0(const TabCtrl* s, POINT point) // get tab in the given point.
{
  switch (s->m_nStyle) {
    // get tab in the given point.
  case TabCtrlStyle_VS2008_client_base:
  case TabCtrlStyle_VS2008_client_classic:
  case TabCtrlStyle_VS2008_client_blue:
  case TabCtrlStyle_VS2008_client_silver:
  case TabCtrlStyle_VS2008_client_olive: {
    if (PtInRect(&s->m_rcTabs, point) != 0) {
      BOOL top = (s->m_Layout == TAB_LAYOUT_TOP);
      int i;
      int hTabSel = s->m_hCurTab;

      if (hTabSel >= 0 && TabCtrlStyle_VS2008_client_base_HitTest(s, hTabSel, top, point) == TRUE) {
        return hTabSel;
      }

      for (i = 0; i < s->m_vtrtabsLen; ++i) {
        int hTab = (i);

        if (hTab != hTabSel && TabCtrlStyle_VS2008_client_base_HitTest(s, hTab, top, point) == TRUE) {
          return hTab;
        }
      }
    }

    return -1;
  }
  }

  {
    RECT rc = s->m_rcTabs;

    if (PtInRect(&rc, point) != 0) {
      int i;

      for (i = 0; i < s->m_vtrtabsLen; ++i) {
        if (PtInRect(&s->m_vtrtabs[i].rc, point) != 0) {
          return i;
        }
      }
    }
  }

  return -1;
}

