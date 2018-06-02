

CCoolControlsManager()
{
  m_nState = dsNormal;
}


int DrawBorder(HDDC hDC, RECT rect)
{
  // Single line control looks better when this style is added
  if ((rect.bottom - rect.top) < 30) {
    m_nState |= dsAlternate;
  }
  else {
    m_nState &= ~dsAlternate;
  }

  if (m_nState & dsHover || m_nState & dsFocus)
    ; // If previous state is the same as current state, do nothing
  else {
    // Perform control-specific drawing routines
    DrawControl(hDC, rect);
  }

  return 0;
}

int DrawControl(HDDC hDC, const RECT& rc)
{
  if (m_nState & dsHoverMask) {
    Draw3dBorder(hDC, rc, COLOR_3DSHADOW, COLOR_3DHIGHLIGHT,
        COLOR_3DDKSHADOW, COLOR_3DLIGHT);
  }
  else {
    if (m_nState & dsDisabled)
      Draw3dBorder(hDC, rc, COLOR_3DSHADOW, COLOR_3DHIGHLIGHT,
          COLOR_3DFACE, COLOR_3DFACE);
    else
      Draw3dBorder(hDC, rc, COLOR_3DSHADOW, COLOR_3DHIGHLIGHT,
          m_nState & dsAlternate ? COLOR_3DHIGHLIGHT : COLOR_3DLIGHT, COLOR_3DLIGHT);
  }

  DrawScrollBars(hDC, rc);
  return 0;
}

int DrawScrollbarThumb(HDDC hDC, const RECT& rc)
{
  if (m_nState & dsHoverMask)
    Draw3dBorder(hDC, rc, COLOR_3DFACE, COLOR_3DDKSHADOW,
        COLOR_3DHIGHLIGHT, COLOR_3DSHADOW);
  else
    Draw3dBorder(hDC, rc, COLOR_3DHIGHLIGHT, COLOR_3DSHADOW,
        COLOR_3DFACE, COLOR_3DFACE);

  return 0;
}

int DrawScrollBars(HDDC hDC, const RECT& rect)
{
  const int nFrameSize  = GetSystemMetrics(SM_CXEDGE);
  const int nScrollSize = GetSystemMetrics(SM_CXHSCROLL);

  RECT rc;
  DWORD dwStyle = m_nStyle;

  if (dwStyle & WS_HSCROLL &&  dwStyle & WS_VSCROLL) {
    rc.left = rect.left + nFrameSize;
    rc.top = rect.bottom - nFrameSize - nScrollSize;
    rc.right = rect.right - nFrameSize - nScrollSize;
    rc.bottom = rect.bottom - nFrameSize;
    DrawScrollBar(hDC, rc, SB_HORZ);

    rc.left = rect.right - nFrameSize - nScrollSize;
    rc.top = rect.top + nFrameSize;
    rc.right = rect.right - nFrameSize;
    rc.bottom = rect.bottom - nFrameSize - nScrollSize;
    DrawScrollBar(hDC, rc, SB_VERT);
  }
  else if (dwStyle & WS_VSCROLL) {
    rc.left = rect.right - nFrameSize - nScrollSize;
    rc.top = rect.top + nFrameSize;
    rc.right = rect.right - nFrameSize;
    rc.bottom = rect.bottom - nFrameSize;
    DrawScrollBar(hDC, rc, SB_VERT);
  }
  else if (dwStyle & WS_HSCROLL) {
    rc.left = rect.left + nFrameSize;
    rc.top = rect.bottom - nFrameSize - nScrollSize;
    rc.right = rect.right - nFrameSize;
    rc.bottom = rect.bottom - nFrameSize;
    DrawScrollBar(hDC, rc, SB_HORZ);
  }

  return 0;
}

int DrawScrollBar(HDDC hDC, const RECT& rect,
    int nType, BOOL bScrollbarCtrl = FALSE)
{
  int nScrollSize = GetSystemMetrics(SM_CXHSCROLL);

  // The minimal thumb size depends on the system version
  // For Windows 98 minimal thumb size is a half of scrollbar size
  // and for Windows NT is always 8 pixels regardless of system metrics.
  // I really don't know why.
  int nMinThumbSize;

  if (GetVersion() & 0x80000000) { // Windows 98 code
    nMinThumbSize = nScrollSize / 2;
  }
  else {
    nMinThumbSize = 8;
  }

  // Calculate the arrow rectangles
  RECT rc1 = rect, rc2 = rect;

  if (nType == SB_HORZ) {
    if ((rect.right - rect.left) < 2 * nScrollSize) {
      nScrollSize = (rect.right - rect.left) / 2;
    }

    rc1.right = rect.left + nScrollSize;
    rc2.left = rect.right - nScrollSize;
  }
  else { // SB_VERT
    if ((rect.bottom - rect.top) < 2 * nScrollSize) {
      nScrollSize = (rect.bottom - rect.top) / 2;
    }

    rc1.bottom = rect.top + nScrollSize;
    rc2.top = rect.bottom - nScrollSize;
  }

  // Draw the scrollbar arrows
  DrawScrollbarThumb(hDC, rc1);
  DrawScrollbarThumb(hDC, rc2);

  // Disabled scrollbar never have a thumb
  if (bScrollbarCtrl == TRUE && m_nState & dsDisabled) {
    return 0;
  }

  SCROLLINFO si = {0};
  si.cbSize = sizeof(SCROLLINFO);
  si.fMask = SIF_ALL;
  //GetScrollInfo(m_hWnd, bScrollbarCtrl ? SB_CTL : nType, &si);

  // Calculate the size and position of the thumb
  int nRange = si.nMax - si.nMin + 1;

  if (nRange) {
    int nScrollArea = (nType == SB_VERT ? (rect.bottom - rect.top) : (rect.right - rect.left)) - 2 * nScrollSize;

    int nThumbSize;

    if (si.nPage == 0) { // If nPage is not set then thumb has default size
      nThumbSize = GetSystemMetrics(SM_CXHTHUMB);
    }
    else {
      nThumbSize = MAX(MulDiv(si.nPage , nScrollArea, nRange), nMinThumbSize);
    }

    if (nThumbSize >= nScrollArea) {
      nThumbSize = nScrollArea;

      if (bScrollbarCtrl == FALSE) {
        return 0;
      }
    }

    int nThumbPos;

    if (UINT(nRange) == si.nPage) {
      nThumbPos = 0;
      nThumbSize--;
    }
    else {
      nThumbPos = MulDiv(si.nPos - si.nMin, nScrollArea - nThumbSize, nRange - si.nPage);
    }

    if (nType == SB_VERT) {
      rc1.top += nScrollSize + nThumbPos;
      rc1.bottom = rc1.top + nThumbSize;
    }
    else { // SB_HORZ
      rc1.left += nScrollSize + nThumbPos;
      rc1.right = rc1.left + nThumbSize;
    }

    if (nThumbSize <= nScrollArea) { // Don't draw the thumb when it's larger than the scroll area
      DrawScrollbarThumb(hDC, rc1);
    }
  }

  return 0;
}


// Edit struct

int DrawEdit(HDDC hDC, const RECT& rc)
{
  RECT rect = rc;

  // Check if edit window has an associated up-down control.
  // If so draw a border around both controls
  // Up-down is found

  if (1) {
    DWORD dwStyle = m_nStyle;

    if ((dwStyle & UDS_ALIGNRIGHT || dwStyle & UDS_ALIGNLEFT)) {
      const int nEdge = GetSystemMetrics(SM_CXEDGE);

      if (dwStyle & UDS_ALIGNRIGHT) {
        rect.right += (rc.right - rc.left) - nEdge;
      }
      else { // UDS_ALIGNLEFT
        rect.left -= (rc.right - rc.left) - nEdge;
      }

      COLORREF clr = _GetSysColor(m_nState & dsHoverMask ? COLOR_3DDKSHADOW : COLOR_3DHIGHLIGHT);

      if (m_nState & dsDisabled) {
        clr = _GetSysColor(COLOR_3DFACE);
      }

      FillSolidRect(hDC, 1, 1, rc.right - rc.left - nEdge - 1, 1, clr);

      if (dwStyle & UDS_ALIGNLEFT) {
        FillSolidRect(hDC, 1, 1, 1, rc.bottom - rc.top - nEdge - 1, clr);
      }
    }
  }

  DrawControl(hDC, rect);
  return 0;
}


// ComboBox struct

int DrawComboBox(HDDC hDC, const RECT& rect)
{
  // First, draw borders around the control
  DrawControl(hDC, rect);

  // Now, we have to draw borders around the drop-down arrow
  RECT rc = rect;
  InflateRect(&rc, -2, -2);
  rc.left = rc.right - GetSystemMetrics(SM_CXHSCROLL);

  if (!(m_nState & dsDisabled)) {
    if (m_nState & dsHoverMask)
      Draw3dBorder(hDC, rc, COLOR_3DFACE, COLOR_3DDKSHADOW,
          COLOR_3DHIGHLIGHT, COLOR_3DSHADOW);
    else
      Draw3dBorder(hDC, rc, COLOR_3DHIGHLIGHT, COLOR_3DSHADOW,
          COLOR_3DFACE, COLOR_3DFACE);
  }
  else
    Draw3dBorder(hDC, rc, COLOR_3DFACE, COLOR_3DFACE,
        COLOR_3DFACE, COLOR_3DFACE);

  return 0;
}


// DateTime struct

int DrawDateTime(HDDC hDC, const RECT& rc)
{
  if (m_nStyle & DTS_UPDOWN) {
    DrawControl(hDC, rc);
  }
  else {
    DrawComboBox(hDC, rc);
  }

  return 0;
}


// PushButton struct

int DrawPushButton(HDDC hDC, const RECT& rc, int nCheck)
{
  BOOL bDefault = FALSE;
  // Unfortunately BS_DEFPUSHBUTTON is defined as 0x00000001L,
  // and BS_OWNERDRAW as 0x0000000BL (see winuser.h) so we cannot
  // determine the default button for owner-draw controls
  DWORD dwStyle = m_nStyle & BS_OWNERDRAW;

  if (dwStyle != BS_OWNERDRAW) {
    if (dwStyle == BS_DEFPUSHBUTTON && !(m_nState & dsDisabled)) {
      bDefault = TRUE;
    }
  }

  if (m_nState & dsHoverMask) {
    if (bDefault == TRUE) {
      Draw3dBorder(hDC, rc, COLOR_3DDKSHADOW, COLOR_3DDKSHADOW,
          COLOR_3DHIGHLIGHT, COLOR_3DDKSHADOW,
          COLOR_3DLIGHT, COLOR_3DSHADOW);
    }
    else {
      if (nCheck)
        Draw3dBorder(hDC, rc, COLOR_3DDKSHADOW, COLOR_3DHIGHLIGHT,
            COLOR_3DSHADOW, COLOR_3DLIGHT);
      else
        Draw3dBorder(hDC, rc, COLOR_3DHIGHLIGHT, COLOR_3DDKSHADOW,
            COLOR_3DLIGHT, COLOR_3DSHADOW,
            COLOR_3DFACE, COLOR_3DFACE);
    }
  }
  else {
    if (bDefault == TRUE) {
      Draw3dBorder(hDC, rc, COLOR_3DDKSHADOW, COLOR_3DDKSHADOW,
          COLOR_3DHIGHLIGHT, COLOR_3DSHADOW,
          COLOR_3DFACE, COLOR_3DFACE);
    }
    else {
      if (nCheck)
        Draw3dBorder(hDC, rc, COLOR_3DSHADOW, COLOR_3DHIGHLIGHT,
            COLOR_3DFACE, COLOR_3DFACE);
      else
        Draw3dBorder(hDC, rc, COLOR_3DHIGHLIGHT, COLOR_3DSHADOW,
            COLOR_3DFACE, COLOR_3DFACE,
            COLOR_3DFACE, COLOR_3DFACE);
    }
  }

  return 0;
}


// CheckBox struct
int DrawCheckBox(HDDC hDC, const RECT& rect)
{
  DWORD dwStyle = m_nStyle;

  int nState = m_nState;

  if (dwStyle & BS_PUSHLIKE) {
    DrawPushButton(hDC, rect, 0);
    return 0;
  }

  RECT rc;

  // Checkmark square size, hard coded here because I couldn't find any
  // method to obtain this value from the system.
  // Maybe someone else knows how to do it? If so, please let me know!
  const int nCheckSize = 13;

  if ((dwStyle & BS_VCENTER) == BS_VCENTER) {
    rc.top = rect.top + ((rect.bottom - rect.top) - nCheckSize) / 2;
  }
  else if (dwStyle & BS_TOP) {
    rc.top = rect.top + 1;
  }
  else if (dwStyle & BS_BOTTOM) {
    rc.top = rect.bottom - nCheckSize - 2;
  }
  else { // Default
    rc.top = rect.top + ((rect.bottom - rect.top) - nCheckSize) / 2;
  }

  if (dwStyle & BS_LEFTTEXT) {
    rc.left = rect.right - nCheckSize;
  }
  else {
    rc.left = rect.left;
  }

  rc.right = rc.left + nCheckSize;
  rc.bottom = rc.top + nCheckSize;

  if (m_nState & dsHoverMask) {
    Draw3dBorder(hDC, rc, COLOR_3DDKSHADOW, COLOR_3DHIGHLIGHT,
        COLOR_3DSHADOW, COLOR_3DFACE);
  }
  else {
    if (!(m_nState & dsDisabled)) {
      Draw3dBorder(hDC, rc, COLOR_3DSHADOW, COLOR_3DHIGHLIGHT,
          nState == 2 ? COLOR_3DHIGHLIGHT : COLOR_WINDOW,
          nState == 2 ? COLOR_3DHIGHLIGHT : COLOR_WINDOW);
    }
    else
      Draw3dBorder(hDC, rc, COLOR_3DSHADOW, COLOR_3DHIGHLIGHT,
          COLOR_3DFACE, COLOR_3DFACE);
  }

  return 0;
}


// RadioButton struct
int DrawRadioButtonFrame(POINT* ptArr, int nColor,
    HDDC hDC, int xOff, int yOff)
{
  for (int i = 0; ; i++) {
    if (!ptArr[i].x && !ptArr[i].y) {
      return 0;
    }

    _SetPixel(hDC, ptArr[i].x + xOff, ptArr[i].y + yOff, _GetSysColor(nColor));
  }

  return 0;
}

int DrawRadioButton(HDDC hDC, const RECT& rect)
{
  DWORD dwStyle = m_nStyle;

  if (dwStyle & BS_PUSHLIKE) {
    DrawPushButton(hDC, rect, 0);
    return 0;
  }

  const int nRadioSize = 12;
  RECT rc;

  if ((dwStyle & BS_VCENTER) == BS_VCENTER) {
    rc.top = rect.top + ((rect.bottom - rect.top) - nRadioSize - 1) / 2;
  }
  else if (dwStyle & BS_TOP) {
    rc.top = rect.top + 1;
  }
  else if (dwStyle & BS_BOTTOM) {
    rc.top = rect.bottom - nRadioSize - 3;
  }
  else { // Default
    rc.top = rect.top + ((rect.bottom - rect.top) - nRadioSize - 1) / 2;
  }

  if (dwStyle & BS_LEFTTEXT) {
    rc.left = rect.right - nRadioSize;
  }
  else {
    rc.left = rect.left + 1;
  }

  rc.right = rc.left + nRadioSize;
  rc.bottom = rc.top + nRadioSize;

  POINT pt1[] = {
    { 1, 9 }, { 1, 8 }, { 0, 7 }, { 0, 6 }, { 0, 5 }, { 0, 4 }, { 1, 3 }, { 1, 2 },
    { 2, 1 }, { 3, 1 }, { 4, 0 }, { 5, 0 }, { 6, 0 }, { 7, 0 }, { 8, 1 }, { 9, 1 },
    { 0, 0 }
  };

  POINT pt2[] = {
    { 2, 8 }, { 1, 7 }, { 1, 6 }, { 1, 5 }, { 1, 4 }, { 2, 3 }, { 2, 2 },
    { 3, 2 }, { 4, 1 }, { 5, 1 }, { 6, 1 }, { 7, 1 }, { 8, 2 }, { 9, 2 },
    { 0, 0 }
  };


  POINT pt3[] = {
    { 2, 9 }, { 3, 9 }, { 4, 10 }, { 5, 10 }, { 6, 10 }, { 7, 10 }, { 8, 9 }, { 9, 9 },
    { 9, 8 }, { 10, 7 }, { 10, 6 }, { 10, 5 }, { 10, 4 }, { 9, 3 }, { 0, 0 }
  };

  POINT pt4[] = {
    { 2, 10 }, { 3, 10 }, { 4, 11 }, { 5, 11 }, { 6, 11 }, { 7, 11 }, { 8, 10 }, { 9, 10 },
    { 10, 9 }, { 10, 8 }, { 11, 7 }, { 11, 6 }, { 11, 5 }, { 11, 4 }, { 10, 3 }, { 10, 2 },
    { 0, 0 }
  };

  if (m_nState & dsHoverMask) {
    DrawRadioButtonFrame(pt1, COLOR_3DSHADOW, hDC, rc.left, rc.top);
    DrawRadioButtonFrame(pt2, COLOR_3DDKSHADOW, hDC, rc.left, rc.top);
    DrawRadioButtonFrame(pt3, COLOR_3DFACE, hDC, rc.left, rc.top);
    DrawRadioButtonFrame(pt4, COLOR_WINDOW, hDC, rc.left, rc.top);
  }
  else {
    if (!(m_nState & dsDisabled)) {
      DrawRadioButtonFrame(pt1, COLOR_3DSHADOW, hDC, rc.left, rc.top);
      DrawRadioButtonFrame(pt2, COLOR_WINDOW, hDC, rc.left, rc.top);
      DrawRadioButtonFrame(pt3, COLOR_3DFACE, hDC, rc.left, rc.top);
      DrawRadioButtonFrame(pt4, COLOR_WINDOW, hDC, rc.left, rc.top);
    }
    else {
      DrawRadioButtonFrame(pt1, COLOR_3DSHADOW, hDC, rc.left, rc.top);
      DrawRadioButtonFrame(pt2, COLOR_3DFACE, hDC, rc.left, rc.top);
      DrawRadioButtonFrame(pt3, COLOR_3DFACE, hDC, rc.left, rc.top);
      DrawRadioButtonFrame(pt4, COLOR_3DHIGHLIGHT, hDC, rc.left, rc.top);
    }
  }

  return 0;
}

// UpDown struct
int DrawUpDownButton(HDDC hDC, const RECT& rc)
{
  // If associated edit control is disabled
  // draw the up-down as disabled too

  if (!(m_nState & dsDisabled) && m_nState & dsHoverMask)
    Draw3dBorder(hDC, rc, COLOR_3DFACE, COLOR_3DDKSHADOW,
        COLOR_3DHIGHLIGHT, COLOR_3DSHADOW);
  else
    Draw3dBorder(hDC, rc, COLOR_3DHIGHLIGHT, COLOR_3DSHADOW,
        COLOR_3DFACE, COLOR_3DFACE);

  return 0;
}

int DrawUpDown(HDDC hDC, const RECT& rect)
{
  RECT rc = rect;
  DWORD dwStyle = m_nStyle;

  if (dwStyle & UDS_ALIGNRIGHT || dwStyle & UDS_ALIGNLEFT) {
    if (dwStyle & UDS_ALIGNLEFT) {
      rc.left += 2;
    }
    else { // UDS_ALIGNRIGHT
      rc.right -= 2;
    }

    rc.top += 2;
    rc.bottom -= 2;

    if (!(m_nState & dsHoverMask)) {
      COLORREF clr1 = _GetSysColor(!(m_nState & dsDisabled) ? COLOR_3DHIGHLIGHT : COLOR_3DFACE);
      COLORREF clr2 = _GetSysColor(!(m_nState & dsDisabled) ? COLOR_3DLIGHT : COLOR_3DFACE);

      FillSolidRect(hDC, rc.left, rc.top - 1, rc.right, 1, clr1);
      FillSolidRect(hDC, rc.left, rc.bottom, rc.right, 1, clr2);

      if (dwStyle & UDS_ALIGNLEFT) {
        FillSolidRect(hDC, rc.left - 1, rc.top - 1, 1, rc.bottom, clr1);
      }
      else {
        FillSolidRect(hDC, rc.right, rc.top - 1, 1, rc.bottom, clr2);
      }
    }
  }

  RECT r = rc;

  if (dwStyle & UDS_HORZ) {
    r.right = r.left + (rc.right - rc.left) / 2;
    DrawUpDownButton(hDC, r);

    r.left = r.right + (rc.right - rc.left) % 2;
    r.right = rc.right;
    DrawUpDownButton(hDC, r);
  }
  else {
    r.bottom = r.top + (rc.bottom - rc.top) / 2;
    DrawUpDownButton(hDC, r);

    r.top = r.bottom + (rc.bottom - rc.top) % 2;
    r.bottom = rc.bottom;
    DrawUpDownButton(hDC, r);
  }

  return 0;
}

int DrawScrollBar(HDDC hDC, const RECT& rc)
{
  DrawScrollBar(hDC, rc,
      (m_nStyle & SBS_VERT) ? SB_VERT : SB_HORZ,
      TRUE);
}

// HeaderCtrl struct
int DrawHeaderCtrlButton(HDDC hDC, const RECT& rc, int nState)
{
  if (nState & dsHoverMask)
    Draw3dBorder(hDC, rc, COLOR_3DHIGHLIGHT, COLOR_3DDKSHADOW,
        COLOR_3DLIGHT, COLOR_3DSHADOW);
  else
    Draw3dBorder(hDC, rc, COLOR_3DHIGHLIGHT, COLOR_3DSHADOW,
        COLOR_3DFACE, COLOR_3DFACE);

  return 0;
}

int DrawHeaderCtrl(HDDC hDC, const RECT& rc, int nCount)
{
  POINT point;
  GetCursorPos(&point);

  // This code fails if we will have standalone header control but such cases are rare...
  // Test if mouse pointer is within the client area of the list control
  BOOL bInView = PtInRect(&rc, point);
  int nState;

  for (int i = 0; i < nCount; i++) {
    nState = 0;

    if (bInView & PtInRect(&rc, point)) {
      nState = dsHover;
    }

    DrawHeaderCtrlButton(hDC, rc, nState);
  }

  int l = rc.right;
  DrawHeaderCtrlButton(hDC, rc, 0);

  return 0;
}


// Trackbar struct

int DrawTrackbarThumb(HDDC hDC, const RECT& rc)
{
  DWORD dwStyle = m_nStyle;

  if (dwStyle & TBS_BOTH) {
    FillSolidRect(hDC, rc, _GetSysColor(COLOR_3DFACE));

    if (m_nState & dsHoverMask)
      Draw3dBorder(hDC, rc, COLOR_3DHIGHLIGHT, COLOR_3DDKSHADOW,
          COLOR_3DLIGHT, COLOR_3DSHADOW);
    else
      Draw3dBorder(hDC, rc, COLOR_3DHIGHLIGHT, COLOR_3DSHADOW,
          COLOR_3DFACE, COLOR_3DFACE);

    return 0;
  }

  COLORREF penHighlight = (_GetSysColor(COLOR_3DHIGHLIGHT));
  COLORREF penLight = (_GetSysColor(m_nState & dsHoverMask ? COLOR_3DLIGHT : COLOR_3DFACE));
  COLORREF penDkShadow = (_GetSysColor(m_nState & dsHoverMask ? COLOR_3DDKSHADOW : COLOR_3DSHADOW));
  COLORREF penShadow = (_GetSysColor(m_nState & dsHoverMask ? COLOR_3DSHADOW : COLOR_3DFACE));

  int n;
  int _x0, _y0;

  if (dwStyle & TBS_VERT) {
    if (dwStyle & TBS_LEFT) {
      n = (rc.bottom - rc.top) / 2 + 1;

      FillSolidRect(hDC, rc, _GetSysColor(COLOR_3DFACE));

      COLORREF _clr = (penHighlight);
      TMoveTo(hDC, rc.right - 2, rc.top);
      _LineTo(hDC, rc.left + n - 1, rc.top);
      _LineTo(hDC, rc.left, rc.top + n - 1);

      _clr = (penDkShadow);
      _LineTo(hDC, rc.left + n - 1, rc.bottom - 1);
      _LineTo(hDC, rc.right - 1, rc.bottom - 1);
      _LineTo(hDC, rc.right - 1, rc.top - 1);

      _clr = (penLight);
      TMoveTo(hDC, rc.right - 3, rc.top + 1);
      _LineTo(hDC, rc.left + n - 1, rc.top + 1);
      _LineTo(hDC, rc.left + 1, rc.top + n - 1);

      _clr = (penShadow);
      _LineTo(hDC, rc.left + n - 1, rc.bottom - 2);
      _LineTo(hDC, rc.right - 2, rc.bottom - 2);
      _LineTo(hDC, rc.right - 2, rc.top);

    }
    else { // TBS_RIGHT
      n = (rc.bottom - rc.top) / 2 + 1;

      FillSolidRect(hDC, rc, _GetSysColor(COLOR_3DFACE));

      COLORREF _clr = (COLORREF)(penHighlight);
      TMoveTo(hDC, rc.left, rc.bottom - 2);
      _LineTo(hDC, rc.left, rc.top);
      _LineTo(hDC, rc.right - n, rc.top);
      _LineTo(hDC, rc.right - 1, rc.top + n - 1);

      _clr = (penDkShadow);
      TMoveTo(hDC, rc.left, rc.bottom - 1);
      _LineTo(hDC, rc.right - n, rc.bottom - 1);
      _LineTo(hDC, rc.right, rc.top + n - 2);

      _clr = (penLight);
      TMoveTo(hDC, rc.left + 1, rc.bottom - 3);
      _LineTo(hDC, rc.left + 1, rc.top + 1);
      _LineTo(hDC, rc.right - n, rc.top + 1);
      _LineTo(hDC, rc.right - 2, rc.top + n - 1);

      _clr = (penShadow);
      TMoveTo(hDC, rc.left + 1, rc.bottom - 2);
      _LineTo(hDC, rc.right - n, rc.bottom - 2);
      _LineTo(hDC, rc.right - 1, rc.top + n - 2);

    }
  }
  else {
    if (dwStyle & TBS_TOP) {
      n = (rc.right - rc.left) / 2 + 1;

      FillSolidRect(hDC, rc, _GetSysColor(COLOR_3DFACE));

      COLORREF _clr = (COLORREF)(penHighlight);
      TMoveTo(hDC, rc.left + n - 2, rc.top + 1);
      _LineTo(hDC, rc.left, rc.top + n - 1);
      _LineTo(hDC, rc.left, rc.bottom - 1);

      _clr = (penDkShadow);
      _LineTo(hDC, rc.right - 1, rc.bottom - 1);
      _LineTo(hDC, rc.right - 1, rc.top + n - 1);
      _LineTo(hDC, rc.left + n - 2, rc.top - 1);

      _clr = (penLight);
      TMoveTo(hDC, rc.left + n - 2, rc.top + 2);
      _LineTo(hDC, rc.left + 1, rc.top + n - 1);
      _LineTo(hDC, rc.left + 1, rc.bottom - 2);

      _clr = (penShadow);
      _LineTo(hDC, rc.right - 2, rc.bottom - 2);
      _LineTo(hDC, rc.right - 2, rc.top + n - 1);
      _LineTo(hDC, rc.left + n - 2, rc.top);

    }
    else { // TBS_BOTTOM
      n = (rc.right - rc.left) / 2 + 1;

      FillSolidRect(hDC, rc, _GetSysColor(COLOR_3DFACE));

      COLORREF _clr = (penHighlight);
      TMoveTo(hDC, rc.left + n - 2, rc.bottom - 2);
      _LineTo(hDC, rc.left, rc.bottom - n);
      _LineTo(hDC, rc.left, rc.top);
      _LineTo(hDC, rc.right - 1, rc.top);

      _clr = (penDkShadow);
      _LineTo(hDC, rc.right - 1, rc.bottom - n);
      _LineTo(hDC, rc.left + n - 2, rc.bottom);

      _clr = (penLight);
      TMoveTo(hDC, rc.left + n - 2, rc.bottom - 3);
      _LineTo(hDC, rc.left + 1, rc.bottom - n);
      _LineTo(hDC, rc.left + 1, rc.top + 1);
      _LineTo(hDC, rc.right - 2, rc.top + 1);

      _clr = (penShadow);
      _LineTo(hDC, rc.right - 2, rc.bottom - n);
      _LineTo(hDC, rc.left + n - 2, rc.bottom - 1);

    }
  }

  return 0;
}

int DrawTrackbar(HDDC hDC, const RECT& rect)
{
  DWORD dwStyle = m_nStyle;

  RECT rc = rect;

  // BUG!: Windows incorrectly calculates the channel rectangle for
  // sliders with TBS_VERT style, so we have to calculate the rectangle
  // in different manner. This bug appears on all Windows platforms!
  if (dwStyle & TBS_VERT) {
    int w = (rc.right - rc.left);
    int h = (rc.bottom - rc.top);
    rc.top = rc.left;
    rc.bottom = rc.left + w;

    RECT r = {0};
    //SendMessage(m_hWnd, TBM_GETTHUMBRECT, 0, (LPARAM)&r);

    rc.left = r.left + ((r.right - r.left) / 2 + 1) - h / 2;

    if (dwStyle & TBS_LEFT)
      ;
    else if (dwStyle & TBS_BOTH) {
      rc.left -= 1;
    }
    else {
      rc.left -= 2;
    }

    rc.right = rc.left + h;
  }

  // Draw the channel rect
  if (m_nState & dsHoverMask) {
    Draw3dBorder(hDC, rc, COLOR_3DSHADOW, COLOR_3DHIGHLIGHT, COLOR_3DDKSHADOW, COLOR_3DLIGHT);
  }
  else {
    Draw3dBorder(hDC, rc, COLOR_3DSHADOW, COLOR_3DHIGHLIGHT, COLOR_3DFACE, COLOR_3DFACE);
  }

  // Draw the slider thumb
  if (!(dwStyle & TBS_NOTHUMB)) {
    SetRectEmpty(&rc);
    //SendMessage(m_hWnd, TBM_GETTHUMBRECT, 0, (LPARAM)&rc);
    DrawTrackbarThumb(hDC, rc);
  }

  return 0;
}


// Toolbar struct

int DrawToolbarButton(HDDC hDC, const RECT& rc, int nState)
{
  if (nState & bsChecked) {
    if (nState & bsHover) {
      Draw3dBorder(hDC, rc, COLOR_3DDKSHADOW, COLOR_3DHIGHLIGHT, COLOR_3DSHADOW, COLOR_3DSHADOW);
    }
    else {
      Draw3dBorder(hDC, rc, COLOR_3DSHADOW, COLOR_3DHIGHLIGHT, COLOR_3DFACE, COLOR_3DFACE);
    }
  }
  else {
    if (nState & bsHover)
      Draw3dBorder(hDC, rc,
          COLOR_3DHIGHLIGHT, COLOR_3DDKSHADOW,
          COLOR_3DLIGHT, COLOR_3DSHADOW);
    else
      Draw3dBorder(hDC, rc,
          COLOR_3DHIGHLIGHT, COLOR_3DSHADOW,
          COLOR_3DFACE, COLOR_3DFACE);
  }
}

// TabControl struct
int DrawTabControlTab(HDDC hDC, const RECT& rect,
    int nItem, int nState)
{
  RECT rc = rect;
  int nCurSel = 0;//SendMessage(m_hWnd, TCM_GETCURSEL, 0, 0L);

  if (nCurSel == -1) {
    nCurSel = -2;
  }

  switch (GetOrientation()) {
  case tabLeft:
    if (nState & bsChecked) {
      rc.top -= 2;
      rc.bottom += 2;
      rc.left -= 2;
      rc.right += 1;
    }

    if (nState & bsHover) {
      if (nCurSel != nItem - 1) {
        FillSolidRect(hDC, rc.left + 2, rc.top, rc.right - rc.left - 2, 1, _GetSysColor(COLOR_3DHIGHLIGHT));
        FillSolidRect(hDC, rc.left + 2, rc.top + 1, rc.right - rc.left - 2, 1, _GetSysColor(COLOR_3DLIGHT));
        _SetPixel(hDC, rc.left + 1, rc.top + 1, _GetSysColor(COLOR_3DHIGHLIGHT));
      }

      FillSolidRect(hDC, rc.left, rc.top + 2, 1, rc.bottom - rc.top - 4, _GetSysColor(COLOR_3DHIGHLIGHT));
      FillSolidRect(hDC, rc.left + 1, rc.top + 2, 1, rc.bottom - rc.top - 4, _GetSysColor(COLOR_3DLIGHT));

      if (nCurSel != nItem + 1) {
        FillSolidRect(hDC, rc.left + 2, rc.bottom - 1, rc.right - rc.left - 2, 1, _GetSysColor(COLOR_3DDKSHADOW));
        FillSolidRect(hDC, rc.left + 2, rc.bottom - 2, rc.right - rc.left - 2, 1, _GetSysColor(COLOR_3DSHADOW));
        _SetPixel(hDC, rc.left + 1, rc.bottom - 2, _GetSysColor(COLOR_3DDKSHADOW));
      }
    }
    else {
      if (nCurSel != nItem - 1) {
        FillSolidRect(hDC, rc.left + 2, rc.top, rc.right - rc.left - 2, 1, _GetSysColor(COLOR_3DHIGHLIGHT));
        FillSolidRect(hDC, rc.left + 2, rc.top + 1, rc.right - rc.left - 2, 1, _GetSysColor(COLOR_3DFACE));
        _SetPixel(hDC, rc.left + 1, rc.top + 1, _GetSysColor(COLOR_3DHIGHLIGHT));
      }

      FillSolidRect(hDC, rc.left, rc.top + 2, 1, rc.bottom - rc.top - 4, _GetSysColor(COLOR_3DHIGHLIGHT));
      FillSolidRect(hDC, rc.left + 1, rc.top + 2, 1, rc.bottom - rc.top - 4, _GetSysColor(COLOR_3DFACE));

      if (nCurSel != nItem + 1) {
        FillSolidRect(hDC, rc.left + 2, rc.bottom - 1, rc.right - rc.left - 2, 1, _GetSysColor(COLOR_3DSHADOW));
        FillSolidRect(hDC, rc.left + 2, rc.bottom - 2, rc.right - rc.left - 2, 1, _GetSysColor(COLOR_3DFACE));
        _SetPixel(hDC, rc.left + 1, rc.bottom - 2, _GetSysColor(COLOR_3DSHADOW));
      }
    }

    break;

  case tabTop:
    if (nState & bsChecked) {
      rc.top -= 2;
      rc.bottom += 1;
      rc.left -= 2;
      rc.right += 2;
    }

    if (nState & bsHover) {
      if (nCurSel != nItem - 1) {
        FillSolidRect(hDC, rc.left, rc.top + 2, 1, rc.bottom - rc.top - 2, _GetSysColor(COLOR_3DHIGHLIGHT));
        FillSolidRect(hDC, rc.left + 1, rc.top + 2, 1, rc.bottom - rc.top - 2, _GetSysColor(COLOR_3DLIGHT));
        _SetPixel(hDC, rc.left + 1, rc.top + 1, _GetSysColor(COLOR_3DHIGHLIGHT));
      }

      FillSolidRect(hDC, rc.left + 2, rc.top, rc.right - rc.left - 4, 1, _GetSysColor(COLOR_3DHIGHLIGHT));
      FillSolidRect(hDC, rc.left + 2, rc.top + 1, rc.right - rc.left - 4, 1, _GetSysColor(COLOR_3DLIGHT));

      if (nCurSel != nItem + 1) {
        FillSolidRect(hDC, rc.right - 1, rc.top + 2, 1, rc.bottom - rc.top - 2, _GetSysColor(COLOR_3DDKSHADOW));
        FillSolidRect(hDC, rc.right - 2, rc.top + 2, 1, rc.bottom - rc.top - 2, _GetSysColor(COLOR_3DSHADOW));
        _SetPixel(hDC, rc.right - 2, rc.top + 1, _GetSysColor(COLOR_3DDKSHADOW));
      }
    }
    else {
      if (nCurSel != nItem - 1) {
        FillSolidRect(hDC, rc.left, rc.top + 2, 1, rc.bottom - rc.top - 2, _GetSysColor(COLOR_3DHIGHLIGHT));
        FillSolidRect(hDC, rc.left + 1, rc.top + 2, 1, rc.bottom - rc.top - 2, _GetSysColor(COLOR_3DFACE));
        _SetPixel(hDC, rc.left + 1, rc.top + 1, _GetSysColor(COLOR_3DHIGHLIGHT));
      }

      FillSolidRect(hDC, rc.left + 2, rc.top, rc.right - rc.left - 4, 1, _GetSysColor(COLOR_3DHIGHLIGHT));
      FillSolidRect(hDC, rc.left + 2, rc.top + 1, rc.right - rc.left - 4, 1, _GetSysColor(COLOR_3DFACE));

      if (nCurSel != nItem + 1) {
        FillSolidRect(hDC, rc.right - 1, rc.top + 2, 1, rc.bottom - rc.top - 2, _GetSysColor(COLOR_3DSHADOW));
        FillSolidRect(hDC, rc.right - 2, rc.top + 2, 1, rc.bottom - rc.top - 2, _GetSysColor(COLOR_3DFACE));
        _SetPixel(hDC, rc.right - 2, rc.top + 1, _GetSysColor(COLOR_3DSHADOW));
      }
    }

    break;

  case tabRight:
    if (nState & bsChecked) {
      rc.top -= 2;
      rc.bottom += 2;
      rc.right += 2;
      rc.left -= 1;
    }

    if (nState & bsHover) {
      if (nCurSel != nItem - 1) {
        FillSolidRect(hDC, rc.left, rc.top, rc.right - rc.left - 2, 1, _GetSysColor(COLOR_3DHIGHLIGHT));
        FillSolidRect(hDC, rc.left, rc.top + 1, rc.right - rc.left - 2, 1, _GetSysColor(COLOR_3DLIGHT));
        _SetPixel(hDC, rc.right - 2, rc.top + 1, _GetSysColor(COLOR_3DHIGHLIGHT));
      }

      FillSolidRect(hDC, rc.right - 1, rc.top + 2, 1, rc.bottom - rc.top - 4, _GetSysColor(COLOR_3DDKSHADOW));
      FillSolidRect(hDC, rc.right - 2, rc.top + 2, 1, rc.bottom - rc.top - 4, _GetSysColor(COLOR_3DSHADOW));

      if (nCurSel != nItem + 1) {
        FillSolidRect(hDC, rc.left, rc.bottom - 2, rc.right - rc.left - 2, 1, _GetSysColor(COLOR_3DSHADOW));
        FillSolidRect(hDC, rc.left, rc.bottom - 1, rc.right - rc.left - 2, 1, _GetSysColor(COLOR_3DDKSHADOW));
        _SetPixel(hDC, rc.right - 2, rc.bottom - 2, _GetSysColor(COLOR_3DDKSHADOW));
      }
    }
    else {
      if (nCurSel != nItem - 1) {
        FillSolidRect(hDC, rc.left, rc.top, rc.right - rc.left - 2, 1, _GetSysColor(COLOR_3DHIGHLIGHT));
        FillSolidRect(hDC, rc.left, rc.top + 1, rc.right - rc.left - 2, 1, _GetSysColor(COLOR_3DFACE));
        _SetPixel(hDC, rc.right - 2, rc.top + 1, _GetSysColor(COLOR_3DHIGHLIGHT));
      }

      FillSolidRect(hDC, rc.right - 1, rc.top + 2, 1, rc.bottom - rc.top - 4, _GetSysColor(COLOR_3DSHADOW));
      FillSolidRect(hDC, rc.right - 2, rc.top + 2, 1, rc.bottom - rc.top - 4, _GetSysColor(COLOR_3DFACE));

      if (nCurSel != nItem + 1) {
        FillSolidRect(hDC, rc.left, rc.bottom - 1, rc.right - rc.left - 2, 1, _GetSysColor(COLOR_3DSHADOW));
        FillSolidRect(hDC, rc.left, rc.bottom - 2, rc.right - rc.left - 2, 1, _GetSysColor(COLOR_3DFACE));
        _SetPixel(hDC, rc.right - 2, rc.bottom - 2, _GetSysColor(COLOR_3DSHADOW));
      }
    }

    break;

  case tabBottom:
    if (nState & bsChecked) {
      rc.bottom += 2;
      rc.left -= 2;
      rc.right += 2;
      rc.top -= 1;
    }

    if (nState & bsHover) {
      if (nCurSel != nItem - 1) {
        FillSolidRect(hDC, rc.left, rc.top, 1, rc.bottom - rc.top - 2, _GetSysColor(COLOR_3DHIGHLIGHT));
        FillSolidRect(hDC, rc.left + 1, rc.top, 1, rc.bottom - rc.top - 2, _GetSysColor(COLOR_3DLIGHT));
        _SetPixel(hDC, rc.left + 1, rc.bottom - 2, _GetSysColor(COLOR_3DHIGHLIGHT));
      }

      FillSolidRect(hDC, rc.left + 2, rc.bottom - 1, rc.right - rc.left - 4, 1, _GetSysColor(COLOR_3DDKSHADOW));
      FillSolidRect(hDC, rc.left + 2, rc.bottom - 2, rc.right - rc.left - 4, 1, _GetSysColor(COLOR_3DSHADOW));

      if (nCurSel != nItem + 1) {
        FillSolidRect(hDC, rc.right - 1, rc.top, 1, rc.bottom - rc.top - 2, _GetSysColor(COLOR_3DDKSHADOW));
        FillSolidRect(hDC, rc.right - 2, rc.top, 1, rc.bottom - rc.top - 2, _GetSysColor(COLOR_3DSHADOW));
        _SetPixel(hDC, rc.right - 2, rc.bottom - 2, _GetSysColor(COLOR_3DDKSHADOW));
      }
    }
    else {
      if (nCurSel != nItem - 1) {
        FillSolidRect(hDC, rc.left, rc.top, 1, rc.bottom - rc.top - 2, _GetSysColor(COLOR_3DHIGHLIGHT));
        FillSolidRect(hDC, rc.left + 1, rc.top, 1, rc.bottom - rc.top - 2, _GetSysColor(COLOR_3DFACE));
        _SetPixel(hDC, rc.left + 1, rc.bottom - 2, _GetSysColor(COLOR_3DHIGHLIGHT));
      }

      FillSolidRect(hDC, rc.left + 2, rc.bottom - 1, rc.right - rc.left - 4, 1, _GetSysColor(COLOR_3DSHADOW));
      FillSolidRect(hDC, rc.left + 2, rc.bottom - 2, rc.right - rc.left - 4, 1, _GetSysColor(COLOR_3DFACE));

      if (nCurSel != nItem + 1) {
        FillSolidRect(hDC, rc.right - 1, rc.top, 1, rc.bottom - rc.top - 2, _GetSysColor(COLOR_3DSHADOW));
        FillSolidRect(hDC, rc.right - 2, rc.top, 1, rc.bottom - rc.top - 2, _GetSysColor(COLOR_3DFACE));
        _SetPixel(hDC, rc.right - 2, rc.bottom - 2, _GetSysColor(COLOR_3DSHADOW));
      }
    }

    break;
  }

  return 0;
}

int GetOrientation() const
{
  DWORD dwStyle = m_nStyle;

  if (dwStyle & TCS_BOTTOM) {
    return (dwStyle & TCS_VERTICAL) ? tabRight : tabBottom;
  }
  else {
    return (dwStyle & TCS_VERTICAL) ? tabLeft : tabTop;
  }

  return 0;
}

int DrawTabControl(HDDC hDC, const RECT& rect)
{
  DWORD dwStyle = m_nStyle;

  if (dwStyle & TCS_BUTTONS) { // Skip button-like tab controls
    return 0;
  }

  RECT rc = rect;
  InflateRect(&rc, 4, 4);

  RECT rcSel;
  int nCurSel = 0;//SendMessage(m_hWnd, TCM_GETCURSEL, 0, 0L);
  //SendMessage(m_hWnd, TCM_GETITEMRECT, nCurSel, (LPARAM)&rcSel);

  switch (GetOrientation()) {
  case tabLeft:
    rc.left += 2;
    FillSolidRect(hDC, rc.left, rc.bottom, rc.right - rc.left, -1, _GetSysColor(COLOR_3DSHADOW));
    FillSolidRect(hDC, rc.right, rc.top, -1, rc.bottom - rc.top, _GetSysColor(COLOR_3DSHADOW));
    break;

  case tabTop:
    rc.top += 2;
    FillSolidRect(hDC, rc.left, rc.bottom, rc.right - rc.left, -1, _GetSysColor(COLOR_3DSHADOW));
    FillSolidRect(hDC, rc.right, rc.top, -1, rc.bottom - rc.top, _GetSysColor(COLOR_3DSHADOW));
    break;

  case tabRight:
    rc.right -= 2;
    FillSolidRect(hDC, rc.left, rc.bottom, rc.right - rc.left, -1, _GetSysColor(COLOR_3DSHADOW));
    FillSolidRect(hDC, rc.right, rc.top, -1, rcSel.top - rc.top, _GetSysColor(COLOR_3DSHADOW));
    FillSolidRect(hDC, rc.right, rcSel.bottom, -1, rc.bottom - rcSel.bottom, _GetSysColor(COLOR_3DSHADOW));
    break;

  case tabBottom:
    rc.bottom -= 2;
    FillSolidRect(hDC, rc.left, rc.bottom, rcSel.left - rc.left, -1, _GetSysColor(COLOR_3DSHADOW));
    FillSolidRect(hDC, rcSel.right, rc.bottom, rc.right - rcSel.right, -1, _GetSysColor(COLOR_3DSHADOW));
    FillSolidRect(hDC, rc.right, rc.top, -1, rc.bottom - rc.top, _GetSysColor(COLOR_3DSHADOW));
    break;
  }

  Draw3dRect(hDC, rc.left + 1, rc.top + 1, rc.right - rc.left - 2, rc.bottom - rc.top - 2,
      _GetSysColor(COLOR_3DFACE), _GetSysColor(COLOR_3DFACE));

  int nState = 0;
  POINT point;
  GetCursorPos(&point);
  //ScreenToClient(m_hWnd, &point);
  int nCount = 0;//SendMessage(m_hWnd, TCM_GETITEMCOUNT, 0, 0L);

  for (int i = 0; i < nCount; i++) {
    //SendMessage(m_hWnd, TCM_GETITEMRECT, i, (LPARAM)&rc);

    nState = bsNormal;

    if (nCurSel != i) {
      if (PtInRect(&rc, point) == TRUE) {
        nState |= bsHover;
      }

      DrawTabControlTab(hDC, rc, i, nState);
    }
  }

  nState = bsChecked;

  if (PtInRect(&rcSel, point) == TRUE) {
    nState |= bsHover;
  }

  DrawTabControlTab(hDC, rcSel, nCurSel, nState);

  if (nCurSel != 0)
    switch (GetOrientation()) {
    case tabTop:
      FillSolidRect(hDC, rect.left, rect.top, 2, rcSel.bottom - rcSel.top + 2, _GetSysColor(COLOR_3DFACE));
      break;

    case tabBottom:
      FillSolidRect(hDC, rect.left, rect.bottom, 2, -rcSel.bottom + rcSel.top - 2, _GetSysColor(COLOR_3DFACE));
      break;
    }

  return 0;
}


// Helper functions for drawing 3D frames (borrowed from CDC struct)

int FillSolidRect(HDDC hDC, int x, int y, int cx, int cy, COLORREF clr)
{
  draw_fillrect(hDC, x, y, x + cx, y + cy, clr);
  return 0;
}

int FillSolidRect(HDDC hDC, const RECT& rect, COLORREF clr)
{
  draw_fillrectR(hDC, &rect, clr);
  return 0;
}

int Draw3dRect(HDDC hDC, int x, int y, int cx, int cy,
    COLORREF clrTopLeft, COLORREF clrBottomRight)
{
  FillSolidRect(hDC, x, y, cx - 1, 1, clrTopLeft);
  FillSolidRect(hDC, x, y, 1, cy - 1, clrTopLeft);
  FillSolidRect(hDC, x + cx, y, -1, cy, clrBottomRight);
  FillSolidRect(hDC, x, y + cy, cx, -1, clrBottomRight);
  return 0;
}

int Draw3dRect(HDDC hDC, const RECT& rect,
    COLORREF clrTopLeft, COLORREF clrBottomRight)
{
  Draw3dRect(hDC, rect.left, rect.top, rect.right - rect.left,
      rect.bottom - rect.top, clrTopLeft, clrBottomRight);
  return 0;
}

int Draw3dBorder(HDDC hDC, const RECT& rc,
    int nColor1, int nColor2,
    int nColor3, int nColor4)
{
  Draw3dRect(hDC, rc, _GetSysColor(nColor1), _GetSysColor(nColor2));

  Draw3dRect(hDC, rc.left + 1, rc.top + 1, rc.right - rc.left - 2, rc.bottom - rc.top - 2,
      _GetSysColor(nColor3), _GetSysColor(nColor4));
  return 0;
}

int Draw3dBorder(HDDC hDC, const RECT& rc,
    int nColor1, int nColor2,
    int nColor3, int nColor4,
    int nColor5, int nColor6)
{
  Draw3dRect(hDC, rc, _GetSysColor(nColor1), _GetSysColor(nColor2));

  Draw3dRect(hDC, rc.left + 1, rc.top + 1, rc.right - rc.left - 2, rc.bottom - rc.top - 2,
      _GetSysColor(nColor3), _GetSysColor(nColor4));
  Draw3dRect(hDC, rc.left + 2, rc.top + 2, rc.right - rc.left - 4, rc.bottom - rc.top - 4,
      _GetSysColor(nColor5), _GetSysColor(nColor6));
  return 0;
}
