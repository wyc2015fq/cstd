

// CCustomDrawToolTipCtrl


#ifndef TTS_BALLOON
#define TTS_BALLOON 0x40
#endif

#ifndef TTM_SETTITLE
#define TTM_SETTITLE (WM_USER + 32)
#endif

void set()
{

  m_bBalloonTooltip = FALSE;
  m_bDrawIcon = TRUE;
  m_bDrawDescription = TRUE;
  m_bBoldLabel = TRUE;
  m_bDrawSeparator = TRUE;
  m_bVislManagerTheme = FALSE;
  m_nMaxDescrWidth = 200;
  m_nGradientAngle = -1;

  m_bRoundedCorners = TRUE;
  m_clrFill = RGB_TOOLTIP_FILL_COLOR1;
  m_clrFillGradient = RGB_TOOLTIP_FILL_COLOR2;
  m_clrText = RGB_TOOLTIP_TEXT_COLOR;
  m_clrBorder = RGB_TOOLTIP_BORDER_COLOR;

  m_clrSeparator = (COLORREF) - 1;
  m_bDrawSupplementalDescription = FALSE;
  m_bDrawDescrIcon = FALSE;
  m_bDrawSupplementalImage = FALSE;
  m_bBoldDescription = FALSE;


  m_DisplayIDWParam = (-1);
  m_DisplayIDLParam = (-1);

  m_ptMargin          = cPOINT(0, 0);
  m_sizeImage         = iSIZE(0, 0);
  m_ptMargin          = cPOINT(0, 0);
  m_ptLocation        = cPOINT(-1, -1);

  m_sizeDescrImage      = iSIZE(0, 0);
  m_sizeSupplementalImage     = iSIZE(0, 0);
  m_nTextTabSize        = 4;
  m_nLabelHeight        = 0;

}

#define TIPTEXTBUFFER_SIZE  80

void SetText(NMHDR* pNMHDR, LPCTSTR lpcszText)
{
  // the szText can not exceed 80 characters.
  // See http://msdn.microsoft.com/en-us/library/bb760252(VS.85).aspx#tooltip_sample_multiline

  // handle both ANSI and UNICODE versions of the message
  LPTOOLTIPTEXTA pTTTA = reinterpret_cast<LPTOOLTIPTEXTA>(pNMHDR);
  LPTOOLTIPTEXTW pTTTW = reinterpret_cast<LPTOOLTIPTEXTW>(pNMHDR);

  size_t nStrLen = _tcslen(lpcszText);

  if (nStrLen >= TIPTEXTBUFFER_SIZE) {
    // the

    // Ansi environment
    if (pNMHDR->code == TTN_NEEDTEXTA) {
      str_setstr2(m_astrLongTipText, lpcszText);
      pTTTA->lpszText = (PSTR)m_astrLongTipText->s;
    }
  }
  else {
    // otherwise, just do the conversion & copy

    // Ansi environment
    if (pNMHDR->code == TTN_NEEDTEXTA) {
      lstrcpyn(pTTTA->szText, lpcszText, TIPTEXTBUFFER_SIZE);
    }
    else if (pNMHDR->code == TTN_NEEDTEXTW) {
      mbstowcs(pTTTW->szText, lpcszText, TIPTEXTBUFFER_SIZE);
    }

  }
}

BOOL CheckDisplayToolTip(WPARAM wParam, LPARAM lParam)
{
  if (m_DisplayIDWParam != wParam || m_DisplayIDLParam != lParam) {
    m_DisplayIDWParam = wParam;
    m_DisplayIDLParam = lParam;
    return TRUE;
  }

  return FALSE;
}

void SetFixedWidth(int nWidthRegular, int nWidthLargeImage)
{
#ifdef DERIVE_FROM_MFCTOOLTIPCTRL
  CCDToolTipCtrlBase::SetFixedWidth(nWidthRegular, nWidthLargeImage);
#else
  m_nFixedWidthRegular  = nWidthRegular;
  m_nFixedWidthWithImage  = nWidthLargeImage;
#endif // DERIVE_FROM_MFCTOOLTIPCTRL
}

int GetFixedWidth()
{
  return 0;
}

BOOL OnEraseBkgnd(HDDC hDC)
{
  RECT rect = m_rcClient;

  COLORREF clrDummy;
  OnFillBackground(hDC, rect, clrDummy, clrDummy);

  return TRUE;
}

void OnShow(HDDC hDC, LRESULT* pResult)
{
  *pResult = 0;

  POINT ptCursor;
  GetCursorPos(&ptCursor);

  m_sizeImage     = m_bDrawIcon ? imsize(m_hLabelIcon) : iSIZE(0, 0);
  m_ptMargin      = m_bRoundedCorners ? cPOINT(6, 4) : cPOINT(4, 2);
  m_sizeDescrImage  = m_bDrawDescrIcon ? GetDescrIconSize() : iSIZE(0, 0);
  m_sizeSupplementalImage = m_bDrawSupplementalImage ? GetSupplementalImageSize() : iSIZE(0, 0);

  //RECT rectMargin = m_rcMargin;
  RECT rectDraw = m_rcClient;

  RECT rectText(rectDraw);

  SIZE sizeText = OnDrawLabel(hDC, rectText, TRUE);

  int cx = sizeText.w;
  int cy = sizeText.h;

  SIZE sizeDescr = iSIZE(0, 0);

  m_nLabelHeight = cy;

  if (m_sizeImage.h > 0 && m_bDrawIcon) {
    m_nLabelHeight = MAX(cy, m_sizeImage.h);
  }

  cy = m_nLabelHeight;

  if (m_bDrawDescription && !(m_strDescription->l <= 0)) {
    sizeDescr = OnDrawDescription(hDC, rectText, TRUE);

    if ((m_sizeDescrImage.w != 0 && m_sizeDescrImage.h != 0) && m_bDrawDescrIcon) {
      sizeDescr.w += m_sizeDescrImage.w + m_ptMargin.x;
      sizeDescr.h = MAX(sizeDescr.h, m_sizeDescrImage.h);
    }

    if (sizeDescr.h <= 0) {
      cy += 2 * m_ptMargin.y;
    }
    else {
      cy += sizeDescr.h + 4 * m_ptMargin.y;
      cx = MAX(cx, sizeDescr.w);
    }
  }

  if (m_sizeImage.w > 0 && m_bDrawIcon) {
    cx += m_sizeImage.w + m_ptMargin.x;
  }

  SIZE sizeBody = OnDrawBody(hDC, rectDraw, TRUE);

  if (sizeBody.w && sizeBody.h) {
    cx = MAX(cx, sizeBody.w + 2 * m_ptMargin.x);
    cy += sizeBody.h + m_ptMargin.y;
  }

  cx += 2 * m_ptMargin.x;
  cy += 2 * m_ptMargin.y;

  const int nFixedWidth = GetFixedWidth();

  if (nFixedWidth > 0 && sizeDescr.w && sizeDescr.h) {
    cx = MAX(cx, nFixedWidth);
  }

  RECT rectWindow;

  int x = rectWindow.left;
  int y = rectWindow.top;

  if (m_ptLocation.x >= 0 && m_ptLocation.y >= 0) {
    x = m_ptLocation.x;
    y = m_ptLocation.y;

    *pResult = 1;
  }

  RECT rectScreen;

  MONITORINFO mi;
  mi.cbSize = sizeof(MONITORINFO);


  if (GetMonitorInfo(MonitorFromPoint(RCLT(&rectWindow), MONITOR_DEFAULTTONEAREST), &mi)) {
    rectScreen = mi.rcWork;
  }
  else {
    ::SystemParametersInfo(SPI_GETWORKAREA, 0, &rectScreen, 0);
  }

  int nBottom = MAX(ptCursor.y + cy + ::GetSystemMetrics(SM_CYCURSOR), y + cy + 2);

  if (nBottom > rectScreen.bottom) {
    y = ptCursor.y - cy - 1;

    *pResult = 1;
  }

  if (x + cx + 2 > rectScreen.right) {
    if ((*pResult) == 1) { // Y has been changed
      x = ptCursor.x - cx - 1;
    }
    else {
      x = rectScreen.right - cx - 1;
      *pResult = 1;
    }
  }

}

void OnPop(NMHDR* pNMHDR, LRESULT* pResult)
{
  str_free(m_strLabel);
  str_free(m_strSupplementalDescription);
  str_free(m_strDescription);
  m_ptLocation  = cPOINT(-1, -1);
  *pResult = 0;
}

void OnPaint(HDDC hDC)
{
  RECT rect = m_rcClient;

  RECT rectMargin = m_rcMargin;

  RECT rectDraw(rect);

  InflateRectR(&rectDraw, &rectMargin);
  DeflateRect(&rectDraw, m_ptMargin.x, m_ptMargin.y);

  RECT rectText(rectDraw);

  COLORREF clrLine = m_clrBorder == (COLORREF) - 1 ? _GetSysColor(COLOR_INFOTEXT) : m_clrBorder;
  COLORREF clrText = m_clrText == (COLORREF) - 1 ? _GetSysColor(COLOR_INFOTEXT) : m_clrText;

  // Fill background:
  OnFillBackground(hDC, rect, clrText, clrLine);

  COLORREF penLine(clrLine);

  // Draw border:
  OnDrawBorder(hDC, rect, clrLine);

  int nIconHeight = 0;

  // Draw icon:
  if (m_sizeImage.w && m_sizeImage.h && m_bDrawIcon) {
    RECT rectImage = rectText;
    rectImage.right = rectImage.left + m_sizeImage.w;
    rectImage.bottom = rectImage.top + m_sizeImage.h;

    //OnDrawIcon(hDC, rectImage);

    rectText.left += m_sizeImage.w + m_ptMargin.x;

    nIconHeight = m_sizeImage.h;
  }


  clrText = (clrText);

  // Draw label:
  int nLabelHeight = OnDrawLabel(hDC, rectText, FALSE).h;

  // Draw body:
  rectDraw.top += MAX(nLabelHeight, nIconHeight);
  SIZE sizeBody = OnDrawBody(hDC, rectDraw, FALSE);

  // Draw separator + description:
  if (m_strDescription->l > 0 && m_bDrawDescription) {
    RECT rectDescr = rectDraw;

    if (sizeBody.w && sizeBody.h) {
      rectDescr.top += m_ptMargin.y;
    }

    rectDescr.top += sizeBody.h + 3 * m_ptMargin.y / 2;

    if (m_bDrawSeparator) {
      OnDrawSeparator(hDC, rectDescr.left, rectDescr.right, rectDescr.top - m_ptMargin.y / 2);
    }

    rectDescr.top += m_ptMargin.y;

    if (m_sizeDescrImage.w && m_sizeDescrImage.h && m_bDrawDescrIcon) {
      RECT rectImage = rectDescr;
      rectImage.right = rectImage.left + m_sizeDescrImage.w;
      rectImage.bottom = rectImage.top + m_sizeDescrImage.h;
      OnDrawDescriptionIcon(hDC, rectImage);
      rectDescr.left += m_sizeDescrImage.w + m_ptMargin.x;
    }

    OnDrawDescription(hDC, rectDescr, FALSE);
  }


}


void OnFillBackground(HDDC hDC, RECT rect, COLORREF& clrText, COLORREF& clrLine)
{
  if (m_clrFill == (COLORREF) - 1) {
    TFillRect(hDC, rect, _GetSysColor(COLOR_INFOBK));
  }
  else {
    if (m_clrFillGradient == (COLORREF) - 1) {
      TFillRect(hDC, rect, m_clrFill);
    }
    else {
      //int nGradientAngle = m_nGradientAngle == -1 ? 90 : m_nGradientAngle;
      FillGradient(hDC, rect, m_clrFill, m_clrFillGradient, TRUE, 0);
    }
  }

}

SIZE DrawText(HDDC hDC,
    const str_t& strText,
    RECT rect,
    BOOL bCalcOnly,
    BOOL bBold DEFAULT(FALSE),
    BOOL bVertCenter DEFAULT(FALSE),
    BOOL bWordBreak DEFAULT(FALSE)
             )
{


  SIZE sizeText = iSIZE(0, 0);

  DRAWTEXTPARAMS dtp = {sizeof(DRAWTEXTPARAMS)};
  dtp.iTabLength = m_nTextTabSize;

  UINT nFormat = DT_NOCLIP | DT_NOPREFIX | DT_TABSTOP | DT_EXPANDTABS;

  if (bWordBreak) {
    nFormat |= DT_WORDBREAK;
  }

  if (bCalcOnly) {
    nFormat |= DT_CALCRECT;
  }

  //sizeText.h = DrawTextEx(hDC, (LPTSTR)(LPCTSTR)strText, strText.GetLength(), rect, nFormat, &dtp);
  sizeText.w = RCW(&rect);

  return sizeText;
}

SIZE OnDrawLabel(HDDC hDC, RECT rect, BOOL bCalcOnly)
{
  return iSIZE(0, 0);//DrawText(hDC, strText, rect, bCalcOnly, bBoldText, TRUE);
}

void OnDrawBorder(HDDC hDC, RECT rect, COLORREF clrLine)
{

  if (m_bRoundedCorners) {
    DrawSimpleRoundRectBorder(hDC, rect, clrLine, FALSE);
  }
  else {
    Draw3dRect(hDC, &rect, clrLine, clrLine);
  }

}

SIZE OnDrawBody(HDDC hDC, RECT rect, BOOL bCalcOnly)
{
  RECT rectDraw(rect);
  DeflateRect(&rectDraw, m_ptMargin.x, m_ptMargin.y);

  SIZE sizeBody = iSIZE(0, 0);

  if (m_sizeSupplementalImage.w && m_sizeSupplementalImage.h && m_bDrawSupplementalImage) {
    if (!bCalcOnly) {
      RECT rectImage = rectDraw;
      rectImage.right = rectImage.left + m_sizeSupplementalImage.w;
      rectImage.bottom = rectImage.top + m_sizeSupplementalImage.h;

      OnDrawSupplementalImage(hDC, rectImage);
    }

    sizeBody = m_sizeSupplementalImage;
  }

  // Draw separator + supplemental description:
  if (m_strSupplementalDescription->l && m_bDrawSupplementalDescription) {
    RECT rectDescr = rectDraw;
    int cx = 0;

    if (sizeBody.w > 0) {
      cx = m_ptMargin.x;
      rectDescr.left += sizeBody.w + cx;
    }

    SIZE sizeDescr = OnDrawSupplementalDescription(hDC, rectDescr, bCalcOnly);
    sizeBody.w += sizeDescr.w + cx;
    sizeBody.h = MAX(sizeBody.h, sizeDescr.h);
  }

  return sizeBody;
}

SIZE GetDescrIconSize()
{
  if (m_hDescrIcon) {
    SIZE sizeIcon = imsize(m_hDescrIcon);
    return sizeIcon;
  }

  return iSIZE(0, 0);
}

SIZE GetSupplementalImageSize()
{
  if (m_hSupplementalBmp) {
    SIZE sizeIcon = imsize(m_hSupplementalBmp);
    return sizeIcon;
  }

  return iSIZE(0, 0);
}

BOOL OnDrawSupplementalImage(HDDC hDC, RECT rect)
{
  return FALSE;
}

SIZE OnDrawSupplementalDescription(HDDC hDC, RECT rect, BOOL bCalcOnly)
{
  //DrawText(hDC, m_strSupplementalDescription, rect, bCalcOnly, FALSE, FALSE, bSingleLineText);
  return iSIZE(0, 0);
}

void SetLocation(POINT pt)
{
  m_ptLocation = pt;
}

void SetTextTabSize(int nTabSize DEFAULT(4))
{
  m_nTextTabSize = nTabSize;
}

BOOL OnDrawDescriptionIcon(HDDC hDC, RECT rect)
{
  if (m_hDescrIcon) {
    //return ::DrawIconEx(hDC, rect.left, rect.top, m_hDescrIcon, m_sizeDescrImage.w, m_sizeDescrImage.h, 0, NULL, DI_NORMAL);
  }

  return FALSE;
}

SIZE OnDrawDescription(HDDC hDC, RECT rect, BOOL bCalcOnly)
{
  if (!m_bDrawDescription) {
    return iSIZE(0, 0);
  }

  int nFixedWidth = GetFixedWidth();

  if (nFixedWidth > 0 && m_sizeImage.w <= 32) {
    rect.right = rect.left + nFixedWidth;

    if (m_sizeImage.w > 0 && m_bDrawIcon) {
      rect.right -= m_sizeImage.w + m_ptMargin.x;
    }
  }
  else if (m_nMaxDescrWidth > 0) {
    rect.right = rect.left + m_nMaxDescrWidth;
  }

  return iSIZE(0, 0);//DrawText(hDC, m_strDescription, rect, bCalcOnly, m_bBoldDescription, TRUE, TRUE);
}

void OnDrawSeparator(HDDC hDC, int x1, int x2, int y)
{
  COLORREF clrSeparator = m_clrSeparator == (COLORREF) - 1 ? _RGB(158, 187, 221) : m_clrBorder;
  COLORREF _clr = clrSeparator;
  int _x0, _y0;
  TMoveTo(hDC, x1, y);
  _LineTo(hDC, x2, y);

  _clr = (_RGB(255, 255, 255));
  TMoveTo(hDC, x1, y + 1);
  _LineTo(hDC, x2, y + 1);


}
