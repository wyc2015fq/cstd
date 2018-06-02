int VisualManager_set(VisualManager* s)
{
  BCGPGLOBAL_DATA_set();
  s->m_bAutoDestroy = FALSE;
  s->m_bLook2000 = FALSE;
  s->m_bMenuFlatLook = FALSE;
  s->m_nMenuShadowDepth = 6;
  s->m_bShadowHighlightedImage = FALSE;
  s->m_bEmbossDisabledImage = TRUE;
  s->m_bFadeInactiveImage = FALSE;
  s->m_bEnableToolbarButtonFill = TRUE;
  s->m_nVertMargin = 12;
  s->m_nHorzMargin = 12;
  s->m_nGroupVertOffset = 15;
  s->m_nGroupCaptionHeight = 25;
  s->m_nGroupCaptionHorzOffset = 13;
  s->m_nGroupCaptionVertOffset = 7;
  s->m_nTasksHorzOffset = 12;
  s->m_nTasksIconHorzOffset = 5;
  s->m_nTasksIconVertOffset = 4;
  s->m_bActiveCaptions = TRUE;
  s->m_bOfficeXPStyleMenus = FALSE;
  s->m_nMenuBorderSize = 2;
  s->m_b3DTabWideBorder = TRUE;
  s->m_bAlwaysFillTab = FALSE;
  s->m_bFrameMenuCheckedItems = FALSE;
  s->m_clrMenuShadowBase = 0; // Used in derived classes
  s->m_bPlannerBackItemToday = FALSE;
  s->m_bPlannerBackItemSelected = FALSE;
  s->m_bPlannerCaptionBackItemHeader = FALSE;
  s->m_ptRibbonMainImageOffset = iPOINT(0, 0);
  s->m_clrPlannerWork = _RGB(255, 255, 0);
  s->m_clrPalennerLine = _RGB(128, 128, 128);
  s->m_clrReportGroupText = globalData.clrHilite;
  s->m_clrToolbarHighlightColor = globalData.clrBtnFace;
  s->m_clrToolbarDisabledTextColor = globalData.clrGrayedText;
  return 0;
}
VisualManager* VisualManager_get()
{
  static VisualManager s[1] = {0};
  static int inited = 0;
  if (0 == inited) {
    inited = 1;
    VisualManager_set(s);
  }
  return s;
}
void OnDrawMenuBorder(HDDC hDC, RECT rect)
{
  draw_3drect_inR(hDC, &rect, globalData.clrBtnLight, globalData.clrBtnDkShadow);
  DeflateRect(&rect, 1, 1);
  draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnShadow);
}
void OnDrawBarGripper(HDDC hDC, RECT rectGripper, BOOL bHorz)
{
  VisualManager* s = VisualManager_get();
  const COLORREF clrHilite = globalData.clrBtnHilite;
  const COLORREF clrShadow = globalData.clrBtnShadow;
  const BOOL bSingleGripper = s->m_bLook2000;
  const int iGripperSize = 3;
  const int iGripperOffset = bSingleGripper ? 0 : 1;
  const int iLinesNum = bSingleGripper ? 1 : 2;
  int i;
  if (bHorz) {
    //-----------------
    // Gripper at left:
    //-----------------
    DeflateRect(&rectGripper, 0, bSingleGripper ? 3 : 2);
    // ET: Center the grippers
    rectGripper.left = iGripperOffset + CenterPoint(&rectGripper).x -
        (iLinesNum * iGripperSize + (iLinesNum - 1) * iGripperOffset) / 2;
    rectGripper.right = rectGripper.left + iGripperSize;
    for (i = 0; i < iLinesNum; i ++) {
      draw_3drect_inR(hDC, &rectGripper,
          clrHilite,
          clrShadow);
      // ET: not used for NewFlat look
      if (! bSingleGripper) {
        //-----------------------------------
        // To look same as MS Office Gripper!
        //-----------------------------------
        _SetPixel(hDC, rectGripper.left, rectGripper.bottom - 1, clrHilite);
      }
      OffsetRect(&rectGripper, iGripperSize + 1, 0);
    }
  }
  else {
    //----------------
    // Gripper at top:
    //----------------
    rectGripper.top += iGripperOffset;
    DeflateRect(&rectGripper, bSingleGripper ? 3 : 2, 0);
    // ET: Center the grippers
    rectGripper.top = iGripperOffset + CenterPoint(&rectGripper).y -
        (iLinesNum * iGripperSize + (iLinesNum - 1)) / 2;
    rectGripper.bottom = rectGripper.top + iGripperSize;
    for (i = 0; i < iLinesNum; i ++) {
      draw_3drect_inR(hDC, &rectGripper,
          clrHilite,
          clrShadow);
      // ET: not used for NewFlat look
      if (! bSingleGripper) {
        //-----------------------------------
        // To look same as MS Office Gripper!
        //-----------------------------------
        _SetPixel(hDC, rectGripper.right - 1, rectGripper.top, clrHilite);
      }
      OffsetRect(&rectGripper, 0, iGripperSize + 1);
    }
  }
}
void OnDrawBarBorder(HDDC hDC, RECT rect, DWORD dwStyle, COLORREF clrBk, BOOL IsOnGlass, BOOL IsCanFloat)
{
  const COLORREF clrHilite = IsOnGlass ? _RGB(0, 0, 0) : globalData.clrBtnHilite;
  const COLORREF clrShadow = IsOnGlass ? _RGB(0, 0, 0) : globalData.clrBtnShadow;
  COLORREF clr = IsOnGlass ? _RGB(0, 0, 0) : clrHilite;
  if (!(dwStyle & CBRS_BORDER_ANY)) {
    return;
  }
  if (dwStyle & CBRS_BORDER_LEFT) {
    draw_fillrect(hDC, 0, 0, 1, RCH(&rect) - 1, clr);
  }
  if (dwStyle & CBRS_BORDER_TOP) {
    draw_fillrect(hDC, 0, 0, RCW(&rect) - 1 , 1, clr);
  }
  if (dwStyle & CBRS_BORDER_RIGHT)
    draw_fillrect(hDC, rect.right, 0/*RGL~:1*/, -1,
        RCH(&rect)/*RGL-: - 1*/, clrShadow);
  if (dwStyle & CBRS_BORDER_BOTTOM) {
    draw_fillrect(hDC, 0, rect.bottom, RCW(&rect) - 1, -1, clrShadow);
  }
  // if undockable toolbar at top of frame, apply special formatting to mesh
  // properly with frame menu
  if (!IsCanFloat) {
    draw_fillrect(hDC, 0, 0, RCW(&rect), 1, clrShadow);
    draw_fillrect(hDC, 0, 1, RCW(&rect), 1, clrHilite);
  }
  if (dwStyle & CBRS_BORDER_LEFT) {
    ++rect.left;
  }
  if (dwStyle & CBRS_BORDER_TOP) {
    ++rect.top;
  }
  if (dwStyle & CBRS_BORDER_RIGHT) {
    --rect.right;
  }
  if (dwStyle & CBRS_BORDER_BOTTOM) {
    --rect.bottom;
  }
}
void OnHighlightMenuItem(HDDC hDC, RECT rect, COLORREF clrText)
{
  draw_fillrectR(hDC, &rect, globalData.clrHilite);
}
COLORREF GetHighlightedMenuItemTextColor(UINT m_nStyle)
{
  if (m_nStyle & UIS_DISABLED) {
    return globalData.clrGrayedText;
  }
  return globalData.clrTextHilite;
}
void OnHighlightRarelyUsedMenuItems(HDDC hDC, RECT rectRarelyUsed)
{
  HighlightRect(hDC, rectRarelyUsed, -1, 0, 0, 0);
  draw_3drect_inR(hDC, &rectRarelyUsed, globalData.clrBtnShadow, globalData.clrBtnHilite);
}
void OnDrawMenuCheck(HDDC hDC, UINT m_nStyle, RECT rectCheck, BOOL bHighlight, BOOL bIsRadio)
{
  DrawMenuImages(hDC, bIsRadio ? IdRadio : IdCheck, rectCheck, (m_nStyle & UIS_DISABLED) ? ImageGray : ImageBlack);
}
void OnDrawMenuItemButton(HDDC hDC, RECT rectButton, BOOL bHighlight, BOOL bDisabled)
{
  RECT rect = rectButton;
  rect.right = rect.left + 1;
  rect.left--;
  DeflateRect(&rect, 0, bHighlight ? 1 : 4);
  draw_3drect_inR(hDC, &rect, globalData.clrBtnShadow, globalData.clrBtnHilite);
}
void OnDrawSeparator(HDDC hDC, RECT rect, BOOL bHorz)
{
  const COLORREF clrHilite = globalData.clrBtnHilite;
  const COLORREF clrShadow = globalData.clrBtnShadow;
  RECT rectSeparator = rect;
  if (bHorz) {
    rectSeparator.left += RCW(&rectSeparator) / 2 - 1;
    rectSeparator.right = rectSeparator.left + 2;
  }
  else {
    rectSeparator.top += RCH(&rectSeparator) / 2 - 1;
    rectSeparator.bottom = rectSeparator.top + 2;
  }
  if (globalData.m_bIsDrawOnGlass) {
    int x1, x2;
    int y1, y2;
    if (bHorz) {
      x1 = x2 = (rect.left + rect.right) / 2;
      y1 = rect.top;
      y2 = rect.bottom - 1;
    }
    else {
      y1 = y2 = (rect.top + rect.bottom) / 2;
      x1 = rect.left;
      x2 = rect.right;
    }
    draw_line(hDC, x1, y1, x2, y2, clrShadow);
  }
  else {
    draw_3drect_inR(hDC, &rectSeparator, clrShadow, clrHilite);
  }
}
COLORREF OnDrawMenuLabel(HDDC hDC, RECT rect)
{
  RECT rectSeparator = rect;
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
  rectSeparator.top = rectSeparator.bottom - 2;
  draw_3drect_inR(hDC, &rectSeparator, globalData.clrBtnShadow,
      globalData.clrBtnHilite);
  return globalData.clrBtnText;
}
COLORREF OnDrawControlBarCaption(HDDC hDC, BOOL bActive, RECT rectCaption, RECT rectButtons)
{
  draw_fillrectR(hDC, &rectCaption, bActive ? globalData.clrActiveCaption : globalData.clrInactiveCaption);
  // get the text color
  return bActive ? globalData.clrCaptionText : globalData.clrInactiveCaptionText;
}
#if 0
void OnDrawCaptionButton(HDDC hDC, RECT rc, BOOL bActive, BOOL bHorz, BOOL bMaximized, UINT uState, int nImageID /*= -1*/)
{
  int imageState;
  int id = nImageID;
  RECT rectImage = rc;
  BOOL bDisabled = uState & UIS_DISABLED;
  BOOL bPushed = uState & UIS_PUSHED;
  BOOL bFocused = uState & UIS_FOCUSED;
  if (bPushed) {
    OffsetRect(&rectImage, 1, 1);
  }
  if (bDisabled) {
    imageState = ImageGray;
  }
  else {
    imageState = bActive ? ImageWhite : ImageBlack;
  }
  DrawMenuImages(hDC, id, rectImage, imageState);
  if (!bDisabled) {
    if (bPushed && bFocused) {
      draw_3drect_inR(hDC, &rc, globalData.clrBtnDkShadow, globalData.clrBtnLight);
      DeflateRect(&rc, 1, 1);
      draw_3drect_inR(hDC, &rc, globalData.clrBtnDkShadow, globalData.clrBtnHilite);
    }
    else if (!s->m_bLook2000) {
      draw_3drect_inR(hDC, &rc, globalData.clrBtnLight, globalData.clrBtnDkShadow);
      DeflateRect(&rc, 1, 1);
      draw_3drect_inR(hDC, &rc, globalData.clrBtnHilite, globalData.clrBtnShadow);
    }
    else if (bFocused || bPushed) {
      draw_3drect_inR(hDC, &rc, globalData.clrBtnHilite, globalData.clrBtnShadow);
    }
  }
}
enum TabStyle {
  STYLE_3D = 0,
  STYLE_FLAT = 1,
  STYLE_FLAT_SHARED_HORZ_SCROLL = 2,
  STYLE_3D_SCROLLED = 3,
  STYLE_3D_ONENOTE = 4,
  STYLE_3D_VS2005 = 5,
  STYLE_3D_ROUNDED = 6,
  STYLE_3D_ROUNDED_SCROLL = 7,
};
enum ResizeMode {
  RESIZE_NO = 0,
  RESIZE_VERT = 1,
  RESIZE_HORIZ = 2,
};
int OnDrawTab(HDDC hDC, RECT rectTab, int iTab, int TabStyle, BOOL bIsActive, COLORREF clrTabBkColor, BOOL LOCATION_BOTTOM)
{
  int _x0, _y0;
  BOOL HasImage = FALSE;
  DWORD _clr = 0;
  DPOINT pts[1000];
  int len, i;
  COLORREF clrTab = clrTabBkColor;
  int nTabsHeight = FontHeight();
  const int nHalfHeight = nTabsHeight / 2;
  if (STYLE_FLAT == TabStyle) {
    // Draw tab edges:
    int a = LOCATION_BOTTOM ? rectTab.top : rectTab.bottom;
    int b = LOCATION_BOTTOM ? rectTab.bottom : rectTab.top;
    int c = nHalfHeight;
    int xLeft = rectTab.left, xRight = rectTab.right;
    i = 0;
    pts[i++] = dPOINT(xLeft, a);
    pts[i++] = dPOINT(xLeft + c, b);
    pts[i++] = dPOINT(xRight - c, b);
    pts[i++] = dPOINT(xRight, a);
    len = i;
    //drawaa_fillpoly_solid(hDC, pts, &len, 1, clrTab);
    draw_fillpoly(hDC, pts, len, clrTab);
    //draw_lines(hDC, pts, len, globalData.clrBtnShadow, 0);
  }
  else if (STYLE_3D_ROUNDED == TabStyle) {
    if (0) {
      int a = LOCATION_BOTTOM ? rectTab.top : rectTab.bottom;
      int b = LOCATION_BOTTOM ? rectTab.bottom : rectTab.top;
      int c = nTabsHeight;
      int k = LOCATION_BOTTOM ? 1 : -1;
      int xLeft = rectTab.left, xRight = rectTab.right;
      i = 0;
      pts[i++] = dPOINT(xLeft, a);
      pts[i++] = dPOINT(xLeft + (c - 5), b - 4 * k);
      pts[i++] = dPOINT(xLeft + (c - 3), b - 2 * k);
      pts[i++] = dPOINT(xLeft + (c + 1), b);
      pts[i++] = dPOINT(xRight - (c + 1), b);
      pts[i++] = dPOINT(xRight - (c - 3), b - 2 * k);
      pts[i++] = dPOINT(xRight - (c - 5), b - 4 * k);
      pts[i++] = dPOINT(xRight, a);
      len = i;
      draw_fillpoly(hDC, pts, len, clrTab);
      draw_lines(hDC, pts, len, globalData.clrBtnShadow, 0);
    }
    else {
      int y, xLeft, xRight, nTabLeft, nTabRight;
      int a = 0, b = countof(pts);
      pts[a++] = (dPOINT(rectTab.left, rectTab.top));
      pts[a++] = (dPOINT(rectTab.left, rectTab.top + 2));
      pts[--b] = (dPOINT(rectTab.right, rectTab.top));
      pts[--b] = (dPOINT(rectTab.right, rectTab.top + 2));
      xLeft = rectTab.left + 1;
      xRight = rectTab.right - 1;
      y = 0;
      for (y = rectTab.top + 2; y < rectTab.bottom - 4; y += 2) {
        pts[a++] = (dPOINT(xLeft, y));
        pts[a++] = (dPOINT(xLeft, y + 2));
        pts[--b] = (dPOINT(xRight, y));
        pts[--b] = (dPOINT(xRight, y + 2));
        xLeft++;
        xRight--;
      }
      if (!LOCATION_BOTTOM) {
        xLeft--;
        xRight++;
      }
      nTabLeft = xLeft - 1;
      nTabRight = xRight + 1;
      for (; y < rectTab.bottom - 1; y++) {
        pts[a++] = (dPOINT(xLeft, y));
        pts[a++] = (dPOINT(xLeft + 1, y + 1));
        pts[--b] = (dPOINT(xRight, y));
        pts[--b] = (dPOINT(xRight - 1, y + 1));
        if (y == rectTab.bottom - 2) {
          pts[a++] = (dPOINT(xLeft + 1, y + 1));
          pts[a++] = (dPOINT(xLeft + 3, y + 1));
          pts[--b] = (dPOINT(xRight, y + 1));
          pts[--b] = (dPOINT(xRight - 2, y + 1));
        }
        xLeft++;
        xRight--;
      }
      pts[a++] = (dPOINT(xLeft + 2, rectTab.bottom));
      pts[--b] = (dPOINT(xRight - 2, rectTab.bottom));
      for (; b < countof(pts);) {
        pts[a++] = pts[b++];
      }
      len = a;
      i = 0;
      for (i = 0; i < len; i++) {
        if (!LOCATION_BOTTOM) {
          pts[i].y = rectTab.top + rectTab.bottom - pts[i].y;
        }
      }
      draw_fillpoly(hDC, pts, len, clrTab);
      //draw_lines(hDC, pts, len, globalData.clrBtnShadow, 0);
      for (i = 0; i < len; i++) {
        _clr = globalData.clrBtnShadow;
        if ((i % 2) != 0) {
          int x1 = (int)pts [i - 1].x;
          int y1 = (int)pts [i - 1].y;
          int x2 = (int)pts [i].x;
          int y2 = (int)pts [i].y;
          if (x1 > CenterPoint(&rectTab).x && x2 > CenterPoint(&rectTab).x) {
            x1--;
            x2--;
          }
          if (y2 >= y1) {
            TMoveTo(hDC, x1, y1);
            _LineTo(hDC, x2, y2);
          }
          else {
            TMoveTo(hDC, x2, y2);
            _LineTo(hDC, x1, y1);
          }
        }
      }
      rectTab.left = nTabLeft;
      rectTab.right = nTabRight;
    }
  }
  else { // 3D Tab
    BOOL bIsCutted = FALSE;
    BOOL IsOneNoteStyle = STYLE_3D_ONENOTE == TabStyle;
    BOOL IsVS2005Style = STYLE_3D_VS2005 == TabStyle;
    const BOOL bIsOneNote = IsOneNoteStyle || IsVS2005Style;
    int nExtra, iVertOffset, iHorzOffset;
    BOOL bIs2005;
    RECT rectClip;
    HDDCGET_CLIP(hDC, &rectClip);
    nExtra = bIsOneNote ?
        ((iTab == 0 || bIsActive || IsVS2005Style) ? 0 : RCH(&rectTab)) : 0;
    if (rectTab.left + nExtra + 10 > rectClip.right ||
        rectTab.right - 10 <= rectClip.left) {
      return 0;
    }
    iVertOffset = 2;
    iHorzOffset = 2;
    bIs2005 = IsVS2005Style;
    if (!bIsActive || bIsOneNote || clrTab != 0 || s->m_bAlwaysFillTab) {
      if (clrTab != 0 || bIsOneNote || s->m_bAlwaysFillTab) {
        RECT rectFill = rectTab;
        if (bIsOneNote) {
          RECT rectFill = rectTab;
          const int nHeight = RCH(&rectFill);
          pts[0].x = rectFill.left;
          pts[0].y = rectFill.bottom;
          pts[1].x = rectFill.left;
          pts[1].y = rectFill.bottom;
          pts[2].x = rectFill.left + 2;
          pts[2].y = rectFill.bottom;
          pts[3].x = rectFill.left + nHeight;
          pts[3].y = rectFill.top + 2;
          pts[4].x = rectFill.left + nHeight + 4;
          pts[4].y = rectFill.top;
          pts[5].x = rectFill.right - 2;
          pts[5].y = rectFill.top;
          pts[6].x = rectFill.right;
          pts[6].y = rectFill.top + 2;
          pts[7].x = rectFill.right;
          pts[7].y = rectFill.bottom;
          for (i = 0; i < 8; i++) {
            if (pts[i].x > rectClip.right) {
              pts[i].x = rectClip.right;
              bIsCutted = TRUE;
            }
            if (LOCATION_BOTTOM) {
              pts[i].y = rectFill.bottom - pts[i].y + rectFill.top - 1;
            }
          }
        }
        else {
          DeflateRect(&rectFill, 1, 0);
          if (LOCATION_BOTTOM) {
            rectFill.bottom--;
          }
          else {
            rectFill.top++;
          }
          rectFill.right = MIN(rectFill.right, rectClip.right);
        }
        len = 8;
        draw_fillpoly(hDC, pts, len, clrTab);
        if (bIsOneNote) {
          RECT rectLeft = rectTab;
          rectLeft.right = rectClip.left - 1;
          if (iTab > 0 && !bIsActive) {
            RECT rectLeftTab = rectClip;
            const int nVertOffset = bIs2005 ? 2 : 1;
            rectLeftTab.right = rectFill.left + RCH(&rectFill) - 10;
            if (LOCATION_BOTTOM) {
              rectLeftTab.top -= nVertOffset;
            }
            else {
              rectLeftTab.bottom += nVertOffset;
            }
          }
          draw_lines(hDC, pts, len, globalData.clrBtnShadow, 1);
          _clr = globalData.clrBtnShadow;
          if (bIsCutted) {
            TMoveTo(hDC, rectClip.right, rectTab.top);
            _LineTo(hDC, rectClip.right, rectTab.bottom);
          }
        }
      }
    }
    {
      COLORREF penLight = (globalData.clrBtnHilite);
      COLORREF penShadow = (globalData.clrBtnShadow);
      COLORREF penDark = (globalData.clrBtnDkShadow);
      if (bIsOneNote) {
        _clr = (penLight);
        if (LOCATION_BOTTOM) {
          if (!bIsCutted) {
            int yTop = (int)(bIsActive ? pts[7].y - 1 : pts[7].y);
            TMoveTo(hDC, pts[6].x - 1, pts[6].y);
            _LineTo(hDC, pts[7].x - 1, yTop);
          }
        }
        else {
          TMoveTo(hDC, pts[2].x + 1, pts[2].y);
          _LineTo(hDC, pts[3].x + 1, pts[3].y);
          TMoveTo(hDC, pts[3].x + 1, pts[3].y);
          _LineTo(hDC, pts[3].x + 2, pts[3].y);
          TMoveTo(hDC, pts[3].x + 2, pts[3].y);
          _LineTo(hDC, pts[3].x + 3, pts[3].y);
          TMoveTo(hDC, pts[4].x - 1, pts[4].y + 1);
          _LineTo(hDC, pts[5].x + 1, pts[5].y + 1);
          if (!bIsActive && !bIsCutted && s->m_b3DTabWideBorder) {
            _clr = (penShadow);
            TMoveTo(hDC, pts[6].x - 2, pts[6].y - 1);
            _LineTo(hDC, pts[6].x - 1, pts[6].y - 1);
          }
          TMoveTo(hDC, pts[6].x - 1, pts[6].y);
          _LineTo(hDC, pts[7].x - 1, pts[7].y);
        }
      }
      else {
        if (rectTab.right > rectClip.right) {
          RECT rectTabClip = rectTab;
          rectTabClip.right = rectClip.right;
        }
        if (LOCATION_BOTTOM) {
          _clr = (penLight);
          if (!s->m_b3DTabWideBorder) {
            _clr = (penShadow);
          }
          TMoveTo(hDC, rectTab.left, rectTab.top);
          _LineTo(hDC, rectTab.left, rectTab.bottom - iVertOffset);
          if (s->m_b3DTabWideBorder) {
            _clr = (penDark);
          }
          _LineTo(hDC, rectTab.left + iHorzOffset, rectTab.bottom);
          _LineTo(hDC, rectTab.right - iHorzOffset, rectTab.bottom);
          _LineTo(hDC, rectTab.right, rectTab.bottom - iVertOffset);
          _LineTo(hDC, rectTab.right, rectTab.top - 1);
          _clr = (penShadow);
          if (s->m_b3DTabWideBorder) {
            TMoveTo(hDC, rectTab.left + iHorzOffset + 1, rectTab.bottom - 1);
            _LineTo(hDC, rectTab.right - iHorzOffset, rectTab.bottom - 1);
            _LineTo(hDC, rectTab.right - 1, rectTab.bottom - iVertOffset);
            _LineTo(hDC, rectTab.right - 1, rectTab.top - 1);
          }
        }
        else {
          _clr = (s->m_b3DTabWideBorder ? penDark : penShadow);
          TMoveTo(hDC, rectTab.right, bIsActive ? rectTab.bottom : rectTab.bottom - 1);
          _LineTo(hDC, rectTab.right, rectTab.top + iVertOffset);
          _LineTo(hDC, rectTab.right - iHorzOffset, rectTab.top);
          if (s->m_b3DTabWideBorder) {
            _clr = (penLight);
          }
          _LineTo(hDC, rectTab.left + iHorzOffset, rectTab.top);
          _LineTo(hDC, rectTab.left, rectTab.top + iVertOffset);
          _LineTo(hDC, rectTab.left, rectTab.bottom);
          if (s->m_b3DTabWideBorder) {
            _clr = (penShadow);
            TMoveTo(hDC, rectTab.right - 1, bIsActive ? rectTab.bottom : rectTab.bottom - 1);
            _LineTo(hDC, rectTab.right - 1, rectTab.top + iVertOffset - 1);
          }
        }
      }
    }
    if (bIsActive) {
      const int iBarHeight = 1;
      const int y = (LOCATION_BOTTOM) ?
          (rectTab.top - iBarHeight - 1) : (rectTab.bottom);
      RECT rectFill = iRECT(rectTab.left, y, rectTab.left + RCW(&rectTab), y + iBarHeight + 1);
      COLORREF clrActiveTab = clrTab;
      if (bIsOneNote) {
        if (bIs2005) {
          rectFill.left += 3;
        }
        else {
          OffsetRect(&rectFill, 1, 0);
          rectFill.left++;
        }
        if (clrActiveTab == 0) {
          clrActiveTab = globalData.clrWindow;
        }
      }
      if (clrActiveTab != 0) {
        draw_fillrectR(hDC, &rectFill, clrActiveTab);
      }
      else {
        draw_fillrectR(hDC, &rectFill, globalData.clrBtnFace);
      }
    }
    if (bIsOneNote) {
      int TAB_TEXT_MARGIN = 4;
      int TAB_IMAGE_MARGIN = 4;
      const int nLeftMargin = IsVS2005Style && bIsActive ? RCH(&rectTab) * 3 / 4 : RCH(&rectTab);
      const int nRightMargin = IsVS2005Style && bIsActive ? TAB_IMAGE_MARGIN * 3 / 4 : TAB_IMAGE_MARGIN;
      rectTab.left += nLeftMargin;
      rectTab.right -= nRightMargin;
      if (IsVS2005Style && bIsActive && HasImage) {
        OffsetRect(&rectTab, TAB_IMAGE_MARGIN, 0);
      }
    }
  }
  return 0;
}
void OnDrawTabCloseButton(HDDC hDC, RECT rect,
    BOOL bIsHighlighted,
    BOOL bIsPressed)
{
  if (bIsHighlighted) {
    draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
  }
  DrawMenuImages(hDC, IdClose, rect, ImageBlack);
  if (bIsHighlighted) {
    if (bIsPressed) {
      draw_3drect_inR(hDC, &rect, globalData.clrBtnDkShadow, globalData.clrBtnHilite);
    }
    else {
      draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnDkShadow);
    }
  }
}
int OnDrawTabsButtonBorder(HDDC hDC, RECT* rect, UINT uiState, BOOL IsPressed)
{
  if (IsPressed || (uiState & ODS_SELECTED)) {
    draw_3drect_inR(hDC, rect, globalData.clrBtnDkShadow, globalData.clrBtnHilite);
    rect->left += 2;
    rect->top += 2;
  }
  else {
    draw_3drect_inR(hDC, rect, globalData.clrBtnHilite, globalData.clrBtnDkShadow);
  }
  DeflateRect(rect, 2, 2);
  return 0;
}
void OnDrawTabResizeBar(HDDC hDC,
    BOOL bIsVert, RECT rect,
    COLORREF pbrFace, COLORREF pPen)
{
  int _x0, _y0;
  DWORD _clr;
  draw_fillrectR(hDC, &rect, pbrFace);
  _clr = (pPen);
  if (bIsVert) {
    TMoveTo(hDC, rect.left, rect.top);
    _LineTo(hDC, rect.left, rect.bottom);
  }
  else {
    TMoveTo(hDC, rect.left, rect.top);
    _LineTo(hDC, rect.right, rect.top);
  }
}
#define MENUIMAGESSIZECX 9
void OnDrawMenuArrowOnCustomizeList(HDDC hDC, RECT rectCommand, BOOL bSelected)
{
  RECT rectLine, rectTriangle = rectCommand;
  rectTriangle.left = rectTriangle.right - MENUIMAGESSIZECX;
  DrawMenuImages(hDC, IdArowRightLarge, rectTriangle,
      bSelected ? ImageWhite : ImageBlack);
  rectLine = rectCommand;
  rectLine.right = rectTriangle.left - 1;
  rectLine.left = rectLine.right - 2;
  DeflateRect(&rectLine, 0, 2);
  draw_3drect_inR(hDC, &rectLine, globalData.clrBtnShadow, globalData.clrBtnHilite);
}
void OnDrawTearOffCaption(HDDC hDC, RECT rect, BOOL bIsActive)
{
  const int iBorderSize = 2;
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
  DeflateRect(&rect, iBorderSize, 1);
  draw_fillrectR(hDC, &rect,
      bIsActive ?
      globalData.clrActiveCaption :
      globalData.clrInactiveCaption);
}
void OnDrawMenuResizeBar(HDDC hDC, RECT rect, int nResizeFlags)
{
  draw_fillrectR(hDC, &rect, globalData.clrInactiveCaption);
}
void OnDrawMenuScrollButton(HDDC hDC, RECT rect, BOOL bIsScrollDown,
    BOOL bIsHighlited, BOOL bIsPressed,
    BOOL bIsDisabled)
{
  RECT rectFill = rect;
  rectFill.top -= 2;
  draw_fillrectR(hDC, &rectFill, globalData.clrBtnFace);
  DrawMenuImages(hDC, bIsScrollDown ? IdArowDown : IdArowUp, rect, ImageBlack);
  if (bIsHighlited) {
    draw_3drect_inR(hDC, &rect,
        globalData.clrBtnHilite,
        globalData.clrBtnShadow);
  }
}
void OnDrawMenuSystemButton(HDDC hDC, RECT rect,
    UINT uiSystemCommand,
    UINT nStyle, BOOL bHighlight)
{
  UINT uiState = 0;
  switch (uiSystemCommand) {
  case SC_CLOSE:
    uiState |= DFCS_CAPTIONCLOSE;
    break;
  case SC_MINIMIZE:
    uiState |= DFCS_CAPTIONMIN;
    break;
  case SC_RESTORE:
    uiState |= DFCS_CAPTIONRESTORE;
    break;
  default:
    return;
  }
  if (nStyle & UIS_PUSHED) {
    uiState |= DFCS_PUSHED;
  }
  if (nStyle & UIS_DISABLED) { // Jan Vasina: Add support for disabled buttons
    uiState |= DFCS_INACTIVE;
  }
  draw_frame_ctrl(hDC, &rect, DFC_CAPTION, uiState);
}
void OnDrawComboDropButton(HDDC hDC, RECT rect,
    BOOL bDisabled,
    BOOL bIsDropped,
    BOOL bIsHighlighted, BOOL bIsDrawOnGlass)
{
  COLORREF clrText = globalData.clrWindowText;
  if (bIsDrawOnGlass) {
    DrawRect(hDC, rect, globalData.clrBtnFace, globalData.clrBtnHilite);
    if (bIsDropped) {
      OffsetRect(&rect, 1, 1);
      DrawRect(hDC, rect, 0, globalData.clrBtnShadow);
    }
    else if (bIsHighlighted) {
      DrawRect(hDC, rect, 0, globalData.clrBtnShadow);
    }
  }
  else {
    draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
    draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnHilite);
    if (bIsDropped) {
      OffsetRect(&rect, 1, 1);
      draw_3drect_inR(hDC, &rect, globalData.clrBtnShadow, globalData.clrBtnHilite);
    }
    else if (bIsHighlighted) {
      draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnShadow);
    }
  }
  DrawMenuImages(hDC, IdArowDown, rect,
      bDisabled ? ImageGray : ImageBlack);
}
void OnDrawComboBorder(HDDC hDC, RECT rect,
    BOOL bIsDropped,
    BOOL bIsHighlighted, BOOL bIsDrawOnGlass)
{
  if (bIsHighlighted || bIsDropped) {
    if (s->m_bMenuFlatLook) {
      DeflateRect(&rect, 1, 1);
    }
    if (bIsDrawOnGlass) {
      DrawRect(hDC, rect, 0, globalData.clrBtnDkShadow);
    }
    else {
      if (s->m_bMenuFlatLook) {
        draw_3drect_inR(hDC, &rect, globalData.clrBtnDkShadow, globalData.clrBtnDkShadow);
      }
      else {
        draw_3drect_inR(hDC, &rect, globalData.clrBtnShadow, globalData.clrBtnHilite);
      }
    }
  }
}
void OnDrawStatusBarPaneBorder(HDDC hDC,
    RECT rectPane, UINT nStyle)
{
  if (nStyle != BDS_NOBORDERS) {
    // draw the borders
    COLORREF clrHilite;
    COLORREF clrShadow;
    if (nStyle == BDS_UP) {
      // reverse colors
      clrHilite = globalData.clrBtnShadow;
      clrShadow = globalData.clrBtnHilite;
    }
    else {
      // normal colors
      clrHilite = globalData.clrBtnHilite;
      clrShadow = globalData.clrBtnShadow;
    }
    draw_3drect_inR(hDC, &rectPane, clrShadow, clrHilite);
  }
}
COLORREF OnFillMiniFrameCaption(HDDC hDC,
    RECT rectCaption, BOOL bActive)
{
  COLORREF br = (bActive ? globalData.clrActiveCaption : globalData.clrInactiveCaption);
  draw_fillrectR(hDC, &rectCaption, br);
  // get the text color
  return bActive ? globalData.clrCaptionText : globalData.clrInactiveCaptionText;
}
typedef enum BCGP_PREDOCK_STATE {
  BCGP_PDS_NOTHING,
  BCGP_PDS_DOCK_REGULAR,
  BCGP_PDS_DOCK_TO_TAB
};
void OnDrawMiniFrameBorder(
    HDDC hDC,
    RECT rectBorder, RECT rectBorderSize, int preDockState)
{
  if (preDockState == BCGP_PDS_DOCK_REGULAR) {
    // draw outer edge;
    draw_3drect_inR(hDC, &rectBorder, _RGB(127, 0, 0), globalData.clrBtnDkShadow);
    DeflateRect(&rectBorder, 1, 1);
    draw_3drect_inR(hDC, &rectBorder, globalData.clrBtnHilite, _RGB(127, 0, 0));
  }
  else if (preDockState == BCGP_PDS_DOCK_TO_TAB) {
    // draw outer edge;
    draw_3drect_inR(hDC, &rectBorder, _RGB(0, 0, 127), globalData.clrBtnDkShadow);
    DeflateRect(&rectBorder, 1, 1);
    draw_3drect_inR(hDC, &rectBorder, globalData.clrBtnHilite, _RGB(0, 0, 127));
  }
  else {
    // draw outer edge;
    draw_3drect_inR(hDC, &rectBorder, globalData.clrBtnFace, globalData.clrBtnDkShadow);
    DeflateRect(&rectBorder, 1, 1);
    draw_3drect_inR(hDC, &rectBorder, globalData.clrBtnHilite, globalData.clrBtnShadow);
  }
  // draw the inner egde
  DeflateRect(&rectBorder, rectBorderSize.right - 2, rectBorderSize.top - 2);
  draw_3drect_inR(hDC, &rectBorder, globalData.clrBtnFace, globalData.clrBtnFace);
  InflateRect(&rectBorder, 1, 1);
  draw_3drect_inR(hDC, &rectBorder, globalData.clrBtnFace, globalData.clrBtnFace);
}
void OnDrawFloatingToolbarBorder(
    HDDC hDC,
    RECT rectBorder)
{
  draw_3drect_inR(hDC, &rectBorder, globalData.clrBtnFace, globalData.clrBtnDkShadow);
  DeflateRect(&rectBorder, 1, 1);
  draw_3drect_inR(hDC, &rectBorder, globalData.clrBtnHilite, globalData.clrBtnShadow);
  DeflateRect(&rectBorder, 1, 1);
  draw_3drect_inR(hDC, &rectBorder, globalData.clrBtnFace, globalData.clrBtnFace);
}
void OnFillOutlookPageButton(HDDC hDC,
    RECT rect,
    COLORREF clrText)
{
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
  clrText = globalData.clrBtnText;
}
void OnDrawOutlookPageButtonBorder(HDDC hDC, RECT* rectBtn, BOOL bIsHighlighted, BOOL bIsPressed)
{
  if (bIsHighlighted && bIsPressed) {
    draw_3drect_inR(hDC, rectBtn, globalData.clrBtnDkShadow, globalData.clrBtnFace);
    DeflateRect(rectBtn, 1, 1);
    draw_3drect_inR(hDC, rectBtn, globalData.clrBtnShadow, globalData.clrBtnHilite);
  }
  else {
    if (bIsHighlighted || bIsPressed) {
      draw_3drect_inR(hDC, rectBtn, globalData.clrBtnFace, globalData.clrBtnDkShadow);
      DeflateRect(rectBtn, 1, 1);
    }
    draw_3drect_inR(hDC, rectBtn, globalData.clrBtnHilite, globalData.clrBtnShadow);
  }
  DeflateRect(rectBtn, 1, 1);
}
COLORREF GetCaptionBarTextColor()
{
  return globalData.clrWindow;
}
void OnDrawCaptionBarBorder(HDDC hDC,
    RECT rect, COLORREF clrBtnBorder, BOOL bFlatBorder)
{
  if (clrBtnBorder == 0) {
    draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
  }
  else {
    COLORREF brBorder = (clrBtnBorder);
    draw_fillrectR(hDC, &rect, brBorder);
  }
  if (!bFlatBorder) {
    draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnShadow);
  }
}
void OnDrawCaptionBarInfoArea(HDDC hDC, RECT rect)
{
  draw_fillrectR(hDC, &rect, _GetSysColor(COLOR_INFOBK));
  draw_3drect_inR(hDC, &rect, globalData.clrBtnShadow, globalData.clrBtnHilite);
  DeflateRect(&rect, 1, 1);
  draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnShadow);
}
COLORREF OnFillCaptionBarButton(HDDC hDC,
    RECT rect, BOOL bIsPressed, BOOL bIsHighlighted,
    BOOL bIsDisabled, BOOL bHasDropDownArrow,
    BOOL bIsSysButton)
{
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
  return bIsDisabled ? globalData.clrGrayedText : globalData.clrBtnText;
}
void OnDrawCaptionBarButtonBorder(HDDC hDC,
    RECT rect, BOOL bIsPressed, BOOL bIsHighlighted,
    BOOL bIsDisabled, BOOL bHasDropDownArrow,
    BOOL bIsSysButton, BOOL IsMessageBarMode)
{
  if (bIsPressed) {
    draw_3drect_inR(hDC, &rect, globalData.clrBtnDkShadow, globalData.clrBtnHilite);
  }
  else if (bIsHighlighted || IsMessageBarMode) {
    draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnDkShadow);
  }
}
void OnDrawStatusBarProgress(HDDC hDC,
    RECT rectProgress, int nProgressTotal, int nProgressCurr,
    COLORREF clrBtn, COLORREF clrProgressBarDest, COLORREF clrProgressText,
    BOOL bProgressText)
{
  RECT rectComplete = rectProgress;
  if (nProgressTotal == 0) {
    return;
  }
  rectComplete.right = rectComplete.left +
      nProgressCurr * RCW(&rectComplete) / nProgressTotal;
  if (clrProgressBarDest == 0) {
    // one-color bar
    COLORREF br = (clrBtn);
    draw_fillrectR(hDC, &rectComplete, br);
  }
  else {
    // gradient bar:
    FillGradient(hDC, rectComplete, clrBtn, clrProgressBarDest, FALSE);
  }
  if (bProgressText) {
    COLORREF clrText;
    char strText[256];
    _snprintf(strText, 256, _T("%d%%"), nProgressCurr * 100 / nProgressTotal);
    clrText = globalData.clrBtnText;
    draw_textR(hDC, strText, -1, &rectProgress, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX, clrText);
    clrText = clrProgressText == 0 ? globalData.clrTextHilite : clrProgressText;
    HDDC_BEGIN(hDC, &rectComplete);
    draw_textR(hDC, strText, -1, &rectProgress, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX, clrText);
    HDDC_END(hDC);
  }
}
void OnFillHeaderCtrlBackground(HDDC hDC, RECT rect)
{
  draw_fillrectR(hDC, &rect,
      globalData.clrBtnFace);
}
void OnDrawHeaderCtrlBorder(HDDC hDC, RECT rect, BOOL bIsPressed)
{
  if (bIsPressed) {
    draw_3drect_inR(hDC, &rect, globalData.clrBtnShadow, globalData.clrBtnShadow);
    rect.left++;
    rect.top++;
  }
  else {
    draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnShadow);
  }
}
void OnDrawStatusBarSizeBox(HDDC hDC,
    RECT rectSizeBox)
{
  RECT rc = rectSizeBox, rc2;
  int dx = MIN(RCH(&rc), RCW(&rc));
  RectCutLeft(&rc, -dx, 0, &rc2);
  draw_frame_ctrl(hDC, &rc2, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
}
void OnDrawEditBorder(HDDC hDC, RECT rect,
    BOOL bIsHighlighted)
{
  if (bIsHighlighted) {
    draw_edge(hDC, rect, EDGE_SUNKEN, BF_RECT);
  }
}
void OnFillTasksPaneBackground(HDDC hDC, RECT rectWorkArea)
{
  draw_fillrectR(hDC, &rectWorkArea, globalData.clrWindow);
}
void OnDrawExpandingBox(HDDC hDC, RECT rect, BOOL bIsOpened,
    COLORREF colorBox)
{
  int _x0, _y0;
  COLORREF _clr = (globalData.clrBtnText);
  POINT ptCenter;
  draw_3drect_inR(hDC, &rect, colorBox, colorBox);
  DeflateRect(&rect, 2, 2);
  ptCenter = CenterPoint(&rect);
  TMoveTo(hDC, rect.left, ptCenter.y);
  _LineTo(hDC, rect.right, ptCenter.y);
  if (!bIsOpened) {
    TMoveTo(hDC, ptCenter.x, rect.top);
    _LineTo(hDC, ptCenter.x, rect.bottom);
  }
}
void OnDrawTasksGroupIcon(HDDC hDC, RECT rect, img_t* hIcon, int nIconHOffset, int nGroupCaptionVertOffset)
{
  RECT rectImage;
  SIZE sizeIcon;
  int x, y, nTaskPaneVOffset = nGroupCaptionVertOffset;
  sizeIcon = imsize(hIcon);
  if (hIcon == NULL) {
    return;
  }
  rectImage = rect;
  rectImage.top += (nTaskPaneVOffset != -1 ? nTaskPaneVOffset : s->m_nGroupCaptionVertOffset);
  rectImage.right = rectImage.left + sizeIcon.w + nIconHOffset;
  x = MAX(0, (RCW(&rectImage) - sizeIcon.w) / 2);
  y = MAX(0, (RCH(&rectImage) - sizeIcon.h) / 2);
  draw_image(hDC, rectImage.left + x, rectImage.bottom - y - sizeIcon.h,
      rectImage.left + x + sizeIcon.w, rectImage.bottom - y, hIcon, 0, 0, 0, 0);
}
#define MENUIMAGESSIZECY MENUIMAGESSIZECX
void OnDrawTasksGroupCaption(HDDC hDC, RECT rectGroup, const char* strName,
    BOOL bIsHighlighted, BOOL bIsSelected, BOOL bCanCollapse, BOOL bIsCollapsed, BOOL bIsSpecial, img_t* hIcon)
{
  BOOL bShowIcon;
  BOOL bIsToolBox = FALSE;
  SIZE sizeIcon = imsize(hIcon);
  if (bIsToolBox) {
    RECT rectFill = rectGroup;
    COLORREF brFill = (globalData.clrBtnShadow);
    DeflateRect4(&rectFill, 1, 0, 1, 1);
    draw_fillrectR(hDC, &rectFill, brFill);
    if (bCanCollapse) {
      //--------------------
      // Draw expanding box:
      //--------------------
      int nBoxSize = 9;
      int nBoxOffset = 6;
      RECT rectButton;
      if (GetRibbonImageScale() != 1.) {
        nBoxSize = (int)(.5 + nBoxSize * GetRibbonImageScale());
      }
      rectButton = rectFill;
      rectButton.left += nBoxOffset;
      rectButton.right = rectButton.left + nBoxSize;
      rectButton.top = CenterPoint(&rectButton).y - nBoxSize / 2;
      rectButton.bottom = rectButton.top + nBoxSize;
      draw_fillrectR(hDC, &rectButton, globalData.clrBtnFace);
      OnDrawExpandingBox(hDC, rectButton, !bIsCollapsed,
          globalData.clrBtnText);
      rectGroup.left = rectButton.right + nBoxOffset;
      bCanCollapse = FALSE;
    }
  }
  else {
    // ---------------------------------
    // Draw caption background (Windows)
    // ---------------------------------
    draw_fillrectR(hDC, &rectGroup, (bIsSpecial ? globalData.clrHilite : globalData.clrBtnFace));
  }
  // ---------------------------
  // Draw an icon if it presents
  // ---------------------------
  bShowIcon = (hIcon != NULL && hIcon->w < RCW(&rectGroup) - RCH(&rectGroup));
  if (bShowIcon) {
    //OnDrawTasksGroupIcon(hDC, pGroup, 5, bIsHighlighted, bIsSelected, bCanCollapse);
  }
  // -----------------------
  // Draw group caption text
  // -----------------------
  {
    int nTaskPaneHOffset = 0;
    int nTaskPaneVOffset = 0;
    int nCaptionHOffset = (nTaskPaneHOffset != -1 ? nTaskPaneHOffset : s->m_nGroupCaptionHorzOffset);
    RECT rectText = rectGroup;
    COLORREF _clr;
    COLORREF clrText = rgb(0, 0, 0);
    COLORREF clrTextHot = rgb(0, 0, 0);
    if (bIsToolBox) {
      _clr = (globalData.clrWindow);
    }
    else {
      if (bCanCollapse && bIsHighlighted) {
        _clr = (clrTextHot == 0 ?
            (bIsSpecial ? globalData.clrWindow : globalData.clrWindowText) :
              clrTextHot);
      }
      else {
        _clr = (clrText == 0 ?
            (bIsSpecial ? globalData.clrWindow : globalData.clrWindowText) :
              clrText);
      }
    }
    rectText.left += (bShowIcon ? sizeIcon.w + 5 : nCaptionHOffset);
    rectText.top += (nTaskPaneVOffset != -1 ? nTaskPaneVOffset : s->m_nGroupCaptionVertOffset);
    rectText.right = MAX(rectText.left, rectText.right - (bCanCollapse ? RCH(&rectGroup) : nCaptionHOffset));
    draw_textR(hDC, strName, -1, &rectText, DT_SINGLELINE | DT_VCENTER, _clr);
    // -------------------------
    // Draw group caption button
    // -------------------------
    if (bCanCollapse && !strName && strlen(strName) > 0) {
      SIZE sizeButton = {MENUIMAGESSIZECX, MENUIMAGESSIZECY};
      RECT rectButton = rectGroup;
      rectButton.left = MAX(rectButton.left,
          rectButton.right - (RCH(&rectButton) + 1) / 2 - (sizeButton.w + 1) / 2);
      rectButton.top = MAX(rectButton.top,
          rectButton.bottom - (RCH(&rectButton) + 1) / 2 - (sizeButton.h + 1) / 2);
      rectButton.right = rectButton.left + sizeButton.w;
      rectButton.bottom = rectButton.top + sizeButton.h;
      if (rectButton.right <= rectGroup.right && rectButton.bottom <= rectGroup.bottom) {
        if (bIsHighlighted) {
          // Draw button frame
          draw_3drect_inR(hDC, &rectButton, globalData.clrWindow, globalData.clrBtnShadow);
        }
        if (bIsSpecial) {
          if (!bIsCollapsed) {
            DrawMenuImages(hDC, IdArowUp, rectButton, ImageBlack);
          }
          else {
            DrawMenuImages(hDC, IdArowDown, rectButton, ImageBlack);
          }
        }
        else {
          if (!bIsCollapsed) {
            DrawMenuImages(hDC, IdArowUp, rectButton, ImageBlack);
          }
          else {
            DrawMenuImages(hDC, IdArowDown, rectButton, ImageBlack);
          }
        }
      }
    }
  }
}
void OnDrawTasksGroupAreaBorder(HDDC hDC, RECT rect, BOOL bSpecial,
    BOOL bNoTitle)
{
  int _x0, _y0;
  DWORD _clr;
  // Draw caption background:
  _clr = (bSpecial ? globalData.clrHilite : globalData.clrBtnFace);
  TMoveTo(hDC, rect.left, rect.top);
  _LineTo(hDC, rect.left, rect.bottom - 1);
  _LineTo(hDC, rect.right - 1, rect.bottom - 1);
  _LineTo(hDC, rect.right - 1, rect.top);
  if (bNoTitle) {
    _LineTo(hDC, rect.left, rect.top);
  }
  else {
    _LineTo(hDC, rect.right - 1, rect.top - 1);
  }
}
void OnDrawTask(HDDC hDC, RECT rectText, img_t* pIcons, int nIcon,
    const char* strText, BOOL bIsHighlighted, BOOL bEnabled, BOOL bIsSeparator, BOOL bMultiline)
{
  SIZE sizeIcon = {0, 0};
  int _x0, _y0;
  COLORREF _clr;
  int nTasksIconHorzOffset = 0;
  int nTaskPaneOffset = nTasksIconHorzOffset;
  ASSERT(pIcons);
  if (bIsSeparator) {
    _clr = (globalData.clrBtnFace);
    TMoveTo(hDC, rectText.left, CenterPoint(&rectText).y);
    _LineTo(hDC, rectText.right, CenterPoint(&rectText).y);
    return;
  }
  // ---------
  // Draw icon
  // ---------
  if (nIcon >= 0) {
    draw_imageR(hDC, &rectText, pIcons, nIcon, 0, 0);
  }
  rectText.left += sizeIcon.w + (nTaskPaneOffset != -1 ? nTaskPaneOffset : s->m_nTasksIconHorzOffset);
  // ---------
  // Draw text
  // ---------
  _clr = globalData.clrWindowText;
  if (!bEnabled) {
    _clr = globalData.clrGrayedText;
  }
  else if (bIsHighlighted) {
    _clr = globalData.clrTextHot;
  }
  if (bMultiline) {
    draw_textR(hDC, strText, -1, &rectText, DT_WORDBREAK, _clr);
  }
  else {
    draw_textR(hDC, strText, -1, &rectText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS, _clr);
  }
}
void OnDrawScrollButtons(HDDC hDC, RECT rect, const int nBorderSize,
    int iImage, BOOL bHilited)
{
  RECT rectImage = {0, 0, MENUIMAGESSIZECX, MENUIMAGESSIZECY};
  RECT rectFill = rect;
  rectFill.top -= nBorderSize;
  draw_fillrectR(hDC, &rectFill, globalData.clrBtnFace);
  if (bHilited) {
    HighlightRect(hDC, rect, -1, 0, 0, 0);
    draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnDkShadow);
  }
  DrawMenuImages(hDC, iImage, rect, ImageBlack);
}
void OnDrawToolBoxFrame(HDDC hDC, RECT rect)
{
  draw_3drect_inR(hDC, &rect, globalData.clrBtnFace, globalData.clrBtnFace);
}
void OnDrawSlider(HDDC hDC, RECT rect, DWORD dwAlgn)
{
  RECT rectScreen = rect;
  RECT rectFill = rect;
  RECT rectBorder = rect;
  COLORREF clrBorder = globalData.clrBtnDkShadow;
  rectFill.left = MIN(rectFill.left, rectScreen.left);
  // draw outer edge;
  if (dwAlgn & CBRS_ALIGN_LEFT) {
    rectBorder.left = rectBorder.right;
  }
  else if (dwAlgn & CBRS_ALIGN_RIGHT) {
    rectBorder.right = rectBorder.left;
    clrBorder = globalData.clrBtnHilite;
  }
  else if (dwAlgn & CBRS_ALIGN_TOP) {
    rectBorder.top = rectBorder.bottom;
  }
  else if (dwAlgn & CBRS_ALIGN_BOTTOM) {
    rectBorder.bottom = rectBorder.top;
    clrBorder = globalData.clrBtnHilite;
  }
  else {
    ASSERT(FALSE);
    return;
  }
  draw_3drect_inR(hDC, &rectBorder, clrBorder, clrBorder);
}
void OnDrawSplitterBorder(HDDC hDC, RECT rect)
{
  draw_3drect_inR(hDC, &rect, globalData.clrBtnShadow, globalData.clrBtnHilite);
  InflateRect(&rect, -CX_BORDER, -CY_BORDER);
  draw_3drect_inR(hDC, &rect, globalData.clrBtnFace, globalData.clrBtnFace);
}
void OnDrawSplitterBox(HDDC hDC, RECT rect)
{
  draw_3drect_inR(hDC, &rect, globalData.clrBtnFace, globalData.clrBtnShadow);
}
void OnFillSplitterBackground(HDDC hDC, RECT rect)
{
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
}
void OnDrawSpinButtons(HDDC hDC, RECT rectSpin,
    int nState, BOOL bOrientation, BOOL bIsDrawOnGlass)
{
  RECT rect[2];
  DeflateRect(&rectSpin, 1, 1);
  rect[0] = rect[1] = rectSpin;
  if (!bOrientation) {
    DeflateRect4(&rect[0], 0, 0, 0, RCH(&rect[0]) / 2);
    rect[1].top = rect[0].bottom + 1;
  }
  else {
    DeflateRect4(&rect[0], 0, 0, RCW(&rect[0]) / 2, 0);
    rect[1].left = rect[0].right + 1;
  }
  if (bIsDrawOnGlass) {
    DrawRect(hDC, rectSpin, globalData.clrBtnFace, globalData.clrBtnHilite);
  }
  else {
    draw_fillrectR(hDC, &rectSpin, globalData.clrBtnFace);
    draw_3drect_inR(hDC, &rectSpin, globalData.clrBtnHilite, globalData.clrBtnHilite);
  }
  {
    int i;
    int id[2][2] = {{IdArowUp, IdArowDown}, {IdArowLeft, IdArowRight}};
    int idxPressed = !!(nState & UIS_PUSHED);
    BOOL bDisabled = nState & UIS_DISABLED;
    for (i = 0; i < 2; i ++) {
      if (bIsDrawOnGlass) {
        if (idxPressed == i) {
          DrawRect(hDC, rect[i], 0, globalData.clrBtnShadow);
        }
        else {
          DrawRect(hDC, rect[i], 0, globalData.clrBtnHilite);
        }
      }
      else {
        if (idxPressed == i) {
          draw_3drect_inR(hDC, &rect[i], globalData.clrBtnShadow, globalData.clrBtnHilite);
        }
        else {
          draw_3drect_inR(hDC, &rect[i], globalData.clrBtnHilite, globalData.clrBtnShadow);
        }
      }
      DrawMenuImages(hDC, id[bOrientation ? 1 : 0][i], rect[i], bDisabled ? ImageGray : ImageBlack);
    }
  }
}
COLORREF OnFillCalendarBarNavigationRow(HDDC hDC, RECT rect)
{
  COLORREF br = (globalData.clrInactiveCaption);
  draw_fillrectR(hDC, &rect, br);
  draw_3drect_inR(hDC, &rect, globalData.clrInactiveBorder, globalData.clrInactiveBorder);
  return globalData.clrInactiveCaptionText;
}
typedef struct CBCGPCalendarColors {
  COLORREF clrCaption;
  COLORREF clrCaptionText;
  COLORREF clrSelected;
  COLORREF clrSelectedText;
  COLORREF clrTodayBorder;
} CBCGPCalendarColors;
void GetCalendarColors(CBCGPCalendarColors* colors)
{
  colors->clrCaption = globalData.clrBtnFace;
  colors->clrCaptionText = globalData.clrBtnText;
  colors->clrSelected = globalData.clrBtnFace;
  colors->clrSelectedText = globalData.clrBtnText;
  colors->clrTodayBorder = _RGB(187, 85, 3);
}
void OnDrawControlBorder(HDDC hDC, RECT rect, BOOL bDrawOnGlass, BOOL IsPopup)
{
  if (bDrawOnGlass) {
    DrawRect(hDC, rect, 0, globalData.clrBtnShadow);
    DeflateRect(&rect, 1, 1);
    DrawRect(hDC, rect, 0, globalData.clrWindow);
  }
  else {
    if (IsPopup) {
      draw_3drect_inR(hDC, &rect, globalData.clrBtnShadow, globalData.clrBtnShadow);
    }
    else {
      draw_3drect_inR(hDC, &rect, globalData.clrBtnDkShadow, globalData.clrBtnHilite);
    }
    DeflateRect(&rect, 1, 1);
    draw_3drect_inR(hDC, &rect, globalData.clrWindow, globalData.clrWindow);
  }
}
void OnDrawShowAllMenuItems(HDDC hDC, RECT rect)
{
  DrawMenuImages(hDC, IdArowShowAll, rect, ImageBlack);
}
#define TEXT_MARGIN 3
#define TEXT_VMARGIN 2
#define MENU_IMAGE_MARGIN 2
int GetShowAllMenuItemsHeight()
{
  return MENUIMAGESSIZECY + 2 * TEXT_MARGIN;
}
#define GetTabFrameColors(clrDark,clrBlack,clrHighlight,clrFace,clrDarkShadow,clrLight,pbrFace,pbrBlack) \
  { \
    clrFace = globalData.clrBtnFace; \
    clrDark = globalData.clrBtnShadow; \
    clrBlack = globalData.clrBtnText; \
    clrHighlight = IsVS2005Style ? globalData.clrBtnShadow : globalData.clrBtnHilite; \
    clrDarkShadow = globalData.clrBtnDkShadow; \
    clrLight = globalData.clrBtnLight; \
    pbrFace = globalData.clrBtnFace; \
    pbrBlack = globalData.clrBlack; }
void OnFillAutoHideButtonBackground(HDDC hDC, RECT rect)
{
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
}
void OnDrawAutoHideButtonBorder(HDDC hDC, RECT rectBounds, RECT rectBorderSize)
{
  COLORREF clr = globalData.clrBtnShadow;
  COLORREF clrText = globalData.clrWindowText;
  if (rectBorderSize.left > 0) {
    draw_fillrect(hDC, rectBounds.left, rectBounds.top,
        rectBounds.left + rectBorderSize.left,
        rectBounds.bottom, clr);
  }
  if (rectBorderSize.top > 0) {
    draw_fillrect(hDC, rectBounds.left, rectBounds.top,
        rectBounds.right,
        rectBounds.top + rectBorderSize.top, clr);
  }
  if (rectBorderSize.right > 0) {
    draw_fillrect(hDC, rectBounds.right - rectBorderSize.right, rectBounds.top,
        rectBounds.right,
        rectBounds.bottom, clr);
  }
  if (rectBorderSize.bottom > 0) {
    draw_fillrect(hDC, rectBounds.left, rectBounds.bottom - rectBorderSize.bottom,
        rectBounds.right,
        rectBounds.bottom, clr);
  }
}
COLORREF GetAutoHideButtonTextColor()
{
  return globalData.clrBtnText;
}
void OnDrawOutlookBarSplitter(HDDC hDC, RECT rectSplitter)
{
  draw_fillrectR(hDC, &rectSplitter, globalData.clrBtnFace);
  draw_3drect_inR(hDC, &rectSplitter, globalData.clrBtnHilite, globalData.clrBtnShadow);
}
void OnFillOutlookBarCaption(HDDC hDC, RECT rectCaption,
    COLORREF clrText)
{
  draw_fillrectR(hDC, &rectCaption, globalData.clrBtnShadow);
  clrText = globalData.clrBtnHilite;
}
enum BCGBUTTON_STATE {
  ButtonsIsRegular,
  ButtonsIsPressed,
  ButtonsIsHighlighted,
};
BOOL OnDrawCalculatorButton(HDDC hDC,
    RECT rect, int state, int cmd)
{
  switch (state) {
  case ButtonsIsPressed:
    draw_fillrectR(hDC, &rect, globalData.clrLight);
    draw_3drect_inR(hDC, &rect, globalData.clrBtnShadow, globalData.clrBtnHilite);
    break;
  case ButtonsIsHighlighted:
    draw_fillrectR(hDC, &rect, globalData.clrLight);
  default:
    draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnShadow);
  }
  return TRUE;
}
BOOL OnDrawCalculatorDisplay(HDDC hDC, RECT rect)
{
  draw_fillrectR(hDC, &rect, globalData.clrWindow);
  draw_3drect_inR(hDC, &rect, globalData.clrBtnShadow, globalData.clrBtnHilite);
  return TRUE;
}
BOOL OnDrawBrowseButton(HDDC hDC, RECT rect,
    int state,
    BOOL IsPressed)
{
  RECT rectFrame = rect;
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
  InflateRect4(&rectFrame, 0, 1, 1, 1);
  draw_3drect_inR(hDC, &rectFrame, globalData.clrBtnDkShadow, globalData.clrBtnDkShadow);
  DeflateRect(&rectFrame, 1, 1);
  draw_edge(hDC, rectFrame, state == IsPressed ? BDR_SUNKENINNER : BDR_RAISEDINNER, BF_RECT);
  return TRUE;
}
void OnDrawAppBarBorder(HDDC hDC,
    RECT rectBorder, RECT rectBorderSize)
{
  COLORREF _clr = (globalData.clrBtnFace);
#define TPatBlt(hDC, x, y, cx, cy, aa) draw_fillrect(hDC, x, y, (x)+cx, (y)+cy, _clr)
  TPatBlt(hDC, rectBorder.left, rectBorder.top, rectBorderSize.left, RCH(&rectBorder), PATCOPY);
  TPatBlt(hDC, rectBorder.left, rectBorder.top, RCW(&rectBorder), rectBorderSize.top, PATCOPY);
  TPatBlt(hDC, rectBorder.right - rectBorderSize.right, rectBorder.top, rectBorderSize.right, RCH(&rectBorder), PATCOPY);
  TPatBlt(hDC, rectBorder.left, rectBorder.bottom - rectBorderSize.bottom, RCW(&rectBorder), rectBorderSize.bottom, PATCOPY);
  DeflateRect(&rectBorderSize, 2, 2);
  DeflateRect(&rectBorder, 2, 2);
  _clr = (globalData.clrLight);
  TPatBlt(hDC, rectBorder.left, rectBorder.top + 1, rectBorderSize.left, RCH(&rectBorder) - 2, PATCOPY);
  TPatBlt(hDC, rectBorder.left + 1, rectBorder.top, RCW(&rectBorder) - 2, rectBorderSize.top, PATCOPY);
  TPatBlt(hDC, rectBorder.right - rectBorderSize.right, rectBorder.top + 1, rectBorderSize.right, RCH(&rectBorder) - 2, PATCOPY);
  TPatBlt(hDC, rectBorder.left + 1, rectBorder.bottom - rectBorderSize.bottom, RCW(&rectBorder) - 2, rectBorderSize.bottom, PATCOPY);
}
void OnDrawAppBarCaption(HDDC hDC, RECT rectCaption, const char* strCaption)
{
  RECT rectText;
  draw_fillrectR(hDC, &rectCaption, globalData.clrBtnFace);
  // Paint caption text:
  rectText = rectCaption;
  DeflateRect(&rectText, 2, 0);
  draw_textR(hDC, strCaption, -1, &rectText, DT_LEFT | DT_SINGLELINE | DT_VCENTER, globalData.clrBtnText);
}
void GetSmartDockingBaseMarkerColors(
    COLORREF clrBaseGroupBackground,
    COLORREF clrBaseGroupBorder)
{
  clrBaseGroupBackground = globalData.clrBtnFace;
  clrBaseGroupBorder = globalData.clrBtnShadow;
}
COLORREF GetSmartDockingMarkerToneColor()
{
  return globalData.clrActiveCaption;
}
BOOL OnEraseToolBoxButton(HDDC hDC, RECT rect, BOOL IsCheck)
{
  if (IsCheck) {
    DeflateRect(&rect, 1, 1);
    FillDitheredRect(hDC, rect);
  }
  return TRUE;
}
BOOL OnDrawToolBoxButtonBorder(HDDC hDC, RECT rect, BOOL IsCheck, BOOL IsHighlighted)
{
  if (IsCheck) {
    draw_3drect_inR(hDC, &rect, globalData.clrBtnShadow, globalData.clrBtnHilite);
  }
  else if (IsHighlighted) {
    draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnShadow);
  }
  return TRUE;
}
COLORREF GetToolBoxButtonTextColor()
{
  return globalData.clrWindowText;
}
void OnFillPopupWindowBackground(HDDC hDC, RECT rect)
{
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
}
void OnDrawPopupWindowBorder(HDDC hDC, RECT rect)
{
  draw_3drect_inR(hDC, &rect, globalData.clrBtnLight, globalData.clrBtnDkShadow);
  DeflateRect(&rect, 1, 1);
  draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnShadow);
}
COLORREF OnDrawPopupWindowCaption(HDDC hDC, RECT rectCaption)
{
  BOOL bActive = TRUE; // TODO
  COLORREF br = (bActive ? globalData.clrActiveCaption : globalData.clrInactiveCaption);
  draw_fillrectR(hDC, &rectCaption, br);
  // get the text color
  return bActive ? globalData.clrCaptionText : globalData.clrInactiveCaptionText;
}
void OnErasePopupWindowButton(HDDC hDC, RECT rect, BOOL IsCaptionButton)
{
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
}
void OnDrawPopupWindowButtonBorder(HDDC hDC, RECT rect, BOOL IsPressed)
{
  if (IsPressed) {
    draw_3drect_inR(hDC, &rect, globalData.clrBtnDkShadow, globalData.clrBtnLight);
    DeflateRect(&rect, 1, 1);
    draw_3drect_inR(hDC, &rect, globalData.clrBtnShadow, globalData.clrBtnHilite);
  }
  else {
    draw_3drect_inR(hDC, &rect, globalData.clrBtnLight, globalData.clrBtnDkShadow);
    DeflateRect(&rect, 1, 1);
    draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnShadow);
  }
}
COLORREF OnFillPlannerCaption(HDDC hDC,
    RECT rect, BOOL bIsToday, BOOL bIsSelected, BOOL bNoBorder)
{
  COLORREF clrText = globalData.clrBtnText;
  const BOOL bMonth = TRUE;
  if (bMonth && !bIsToday && !bIsSelected) {
    return clrText;
  }
  DeflateRect(&rect, 1, 1);
  if (bIsToday) {
    draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
    FillDitheredRect(hDC, rect);
  }
  else if (bIsSelected) {
    draw_fillrectR(hDC, &rect, globalData.clrHilite);
    clrText = globalData.clrTextHilite;
  }
  else {
    draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
  }
  if (bIsToday || !bNoBorder) {
    int _x0, _y0;
    COLORREF _clr = (globalData.clrBtnShadow);
    TMoveTo(hDC, rect.left, rect.bottom);
    _LineTo(hDC, rect.right, rect.bottom);
  }
  return clrText;
}
void OnDrawPlannerCaptionText(HDDC hDC,
    RECT rect, const char* strText,
    COLORREF clrText, int nAlign, BOOL bHighlight)
{
  const int nTextMargin = 2;
  if (bHighlight) {
    const int nTextLen = MIN(RCW(&rect),
        FontGetTextExtent(strText).w + 2 * nTextMargin);
    RECT rectHighlight = rect;
    DeflateRect(&rectHighlight, 1, 1);
    switch (nAlign) {
    case DT_LEFT:
      rectHighlight.right = rectHighlight.left + nTextLen;
      break;
    case DT_RIGHT:
      rectHighlight.left = rectHighlight.right - nTextLen;
      break;
    case DT_CENTER:
      rectHighlight.left = CenterPoint(&rectHighlight).x - nTextLen / 2;
      rectHighlight.right = rectHighlight.left + nTextLen;
      break;
    }
    draw_fillrectR(hDC, &rectHighlight, globalData.clrHilite);
    clrText = globalData.clrTextHilite;
  }
  DeflateRect(&rect, nTextMargin, 0);
  draw_textR(hDC, strText, -1, &rect, DT_SINGLELINE | DT_VCENTER | nAlign, clrText);
}
#define BCGP_PLANNER_RULE_DAILY 1
#define BCGP_PLANNER_RULE_WEEKLY 2
#define BCGP_PLANNER_RULE_MONTHLY 3
#define BCGP_PLANNER_RULE_YEARLY 4
#define BCGP_PLANNER_DRAW_APP_GRADIENT_FILL 0x0001
#define BCGP_PLANNER_DRAW_APP_ROUNDED_CORNERS 0x0002
#define BCGP_PLANNER_DRAW_APP_OVERRIDE_SELECTION 0x0004
#define BCGP_PLANNER_DRAW_APP_NO_MULTIDAY_CLOCKS 0x0008
#define BCGP_PLANNER_DRAW_APP_DURATION_SHAPE 0x0010
#define BCGP_PLANNER_DRAW_APP_NO_DURATION 0x0020
#define BCGP_PLANNER_DRAW_APP_NO_IMAGES 0x0040
#define BCGP_PLANNER_DRAW_APP_NO_RECURRENCE_IMAGE 0x0080
#define BCGP_PLANNER_DRAW_VIEW_NO_DURATION 0x0100
#define BCGP_PLANNER_DRAW_VIEW_WEEK_BAR 0x0200
#define BCGP_PLANNER_DRAW_VIEW_CAPTION_DAY_BOLD 0x0400
#define BCGP_PLANNER_DRAW_VIEW_CAPTION_DAY_COMPACT 0x0800
void GetPlannerAppointmentColors(
    BOOL bSelected, BOOL bSimple, DWORD dwDrawFlags,
    COLORREF clrBack1, COLORREF clrBack2,
    COLORREF clrFrame1, COLORREF clrFrame2, COLORREF clrText, COLORREF clrBackgroundColor)
{
  const BOOL bIsGradientFill =
      dwDrawFlags & BCGP_PLANNER_DRAW_APP_GRADIENT_FILL &&
      !(globalData.m_bIsBlackHighContrast || globalData.m_bIsWhiteHighContrast);
  const BOOL bIsOverrideSelection = dwDrawFlags & BCGP_PLANNER_DRAW_APP_OVERRIDE_SELECTION;
  const BOOL bSelect = bSelected && !bIsOverrideSelection;
  const BOOL bDayView = TRUE;
  BOOL bDefault = FALSE;
  if (bSelect) {
    if (bDayView) {
      if (!bSimple) {
        clrBack1 = globalData.clrBtnFace;
        clrText = globalData.clrBtnText;
      }
    }
    else {
      clrBack1 = globalData.clrHilite;
      clrText = globalData.clrTextHilite;
    }
  }
  if (clrBack1 == CLR_DEFAULT) {
    if (bIsGradientFill) {
      clrBack1 = clrBackgroundColor;
      if (clrBack1 == CLR_DEFAULT) {
        bDefault = TRUE;
        clrBack1 = s->m_clrPlannerWork;
      }
    }
    else {
      clrBack1 = (bDayView || !bSimple)
          ? globalData.clrWindow
          : CLR_DEFAULT;
    }
  }
  clrBack2 = clrBack1;
  if (clrText == CLR_DEFAULT) {
    clrText = globalData.clrWindowText;
  }
  clrFrame1 = globalData.clrWindowFrame;
  clrFrame2 = clrFrame1;
  if (bIsGradientFill && clrBack1 != CLR_DEFAULT) {
    if (!bSelect) {
      if (bDefault) {
        clrBack2 = _RGB(255, 255, 255);
      }
      else {
        clrBack2 = PixelAlpha3(clrBack1, _RGB(255, 255, 255), 25);
      }
    }
    clrFrame1 = PixelAlpha3(clrBack1, _RGB(0, 0, 0), 75);
    if (!bSelected) {
      clrFrame2 = clrFrame1;
    }
  }
}
COLORREF GetPlannerAppointmentTimeColor()
{
  return CLR_DEFAULT;
}
COLORREF GetPlannerHourLineColor()
{
  return s->m_clrPalennerLine;
}
COLORREF GetPlannerSelectionColor()
{
  return globalData.clrHilite;
}
COLORREF GetPlannerSeparatorColor()
{
  return globalData.clrWindowFrame;
}
void OnFillPlanner(HDDC hDC, RECT rect, BOOL bWorkingArea, COLORREF clrBackgroundColor)
{
}
COLORREF OnFillPlannerTimeBar(HDDC hDC,
    RECT rect, COLORREF clrLine)
{
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
  clrLine = globalData.clrBtnShadow;
  return globalData.clrBtnText;
}
void OnDrawPlannerTimeLine(HDDC hDC, RECT rect)
{
  int _x0, _y0;
  COLORREF _clr = (globalData.clrBtnShadow);
  draw_fillrectR(hDC, &rect, globalData.clrActiveCaption);
  TMoveTo(hDC, rect.left, rect.bottom);
  _LineTo(hDC, rect.right, rect.bottom);
}
void OnFillPlannerWeekBar(HDDC hDC, RECT rect)
{
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
}
void OnDrawPlannerHeader(HDDC hDC,
    RECT rect)
{
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
  draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnShadow);
}
void OnDrawPlannerHeaderPane(HDDC hDC,
    RECT rect)
{
  draw_3drect_inR(hDC, &rect, globalData.clrBtnShadow, globalData.clrBtnHilite);
}
void OnFillPlannerHeaderAllDay(HDDC hDC, RECT rect)
{
  draw_fillrectR(hDC, &rect, globalData.clrBtnShadow);
}
void OnDrawPlannerHeaderAllDayItem(HDDC hDC, RECT rect, BOOL bIsSelected)
{
  if (bIsSelected) {
    draw_fillrectR(hDC, &rect, globalData.clrWindow);
  }
}
void PreparePlannerBackItem()
{
  s->m_bPlannerBackItemToday = FALSE;
  s->m_bPlannerBackItemSelected = FALSE;
}
void PreparePlannerCaptionBackItem(BOOL bIsHeader)
{
  s->m_bPlannerCaptionBackItemHeader = bIsHeader;
}
void DoDrawHeaderSortArrow(HDDC hDC, RECT rectArrow, BOOL bIsUp)
{
  DrawMenuImages(hDC, bIsUp ? IdArowUpLarge : IdArowDownLarge, rectArrow, ImageBlack);
}
void OnDrawGridSortArrow(HDDC hDC, RECT rectArrow, BOOL bIsUp)
{
  DoDrawHeaderSortArrow(hDC, rectArrow, bIsUp);
}
COLORREF OnFillGridGroupByBoxBackground(HDDC hDC, RECT rect)
{
  draw_fillrectR(hDC, &rect, globalData.clrBtnShadow);
  return globalData.clrBtnText;
}
COLORREF OnFillGridGroupByBoxTitleBackground(HDDC hDC, RECT rect)
{
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
  return globalData.clrBtnShadow;
}
COLORREF GetGridGroupByBoxLineColor()
{
  return globalData.clrBtnText;
}
void OnDrawGridGroupByBoxItemBorder(
    HDDC hDC, RECT rect)
{
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
  draw_3drect_inR(hDC, &rect, globalData.clrBtnWindow, globalData.clrBtnText);
  DeflateRect4(&rect, 0, 0, 1, 1);
  draw_3drect_inR(hDC, &rect, globalData.clrBtnWindow, globalData.clrBtnShadow);
}
COLORREF GetGridLeftOffsetColor()
{
  COLORREF clrGray = 0;
  clrGray = PixelAlpha2(globalData.clrBtnFace, 94);
  return clrGray;
}
COLORREF GetGridItemSortedColor(COLORREF clrBkColor)
{
  COLORREF clrSortedColumn = 0;
  clrSortedColumn = PixelAlpha(clrBkColor, .97, .97, .97);
  return clrSortedColumn;
}
void OnFillGridGroupBackground(HDDC hDC, RECT rectFill)
{
  draw_fillrectR(hDC, &rectFill, globalData.clrWindow);
}
void OnDrawGridGroupUnderline(HDDC hDC, RECT rectFill, COLORREF clrBkColor)
{
  draw_fillrectR(hDC, &rectFill,
      globalData.clrBtnShadow);
}
COLORREF OnFillGridRowBackground(HDDC hDC, RECT rectFill, BOOL bSelected, BOOL IsFocused, BOOL IsHighlightGroups)
{
  // Fill area:
  if (!IsFocused) {
    draw_fillrectR(hDC, &rectFill,
        globalData.clrBtnFace);
  }
  else {
    draw_fillrectR(hDC, &rectFill, globalData.clrHilite);
  }
  // Return text color:
  if (!IsHighlightGroups && bSelected) {
    return (!IsFocused) ? globalData.clrHotText : globalData.clrTextHilite;
  }
  return IsHighlightGroups ?
      (globalData.clrBtnShadow) :
      globalData.clrWindowText;
}
COLORREF OnFillGridItem(
    HDDC hDC, RECT rectFill,
    BOOL bSelected, BOOL bActiveItem, BOOL bSortedColumn, BOOL IsFocused, COLORREF clrSortedColor)
{
  // Fill area:
  if (bSelected && !bActiveItem) {
    if (!IsFocused) {
      draw_fillrectR(hDC, &rectFill,
          globalData.clrBtnFace);
      return globalData.clrBtnText;
    }
    else {
      draw_fillrectR(hDC, &rectFill, globalData.clrHilite);
      return globalData.clrTextHilite;
    }
  }
  else {
    if (bActiveItem) {
      draw_fillrectR(hDC, &rectFill, globalData.clrWindow);
    }
    else if (bSortedColumn) {
      draw_fillrectR(hDC, &rectFill, clrSortedColor);
    }
    else {
      // no painting
    }
  }
  return 0;
}
void OnDrawGridHeaderMenuButton(HDDC hDC, RECT rect,
    BOOL bHighlighted)
{
  DeflateRect(&rect, 1, 1);
  if (bHighlighted) {
    draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnDkShadow);
  }
}
void OnDrawGridSelectionBorder(HDDC hDC, RECT rect)
{
  draw_3drect_inR(hDC, &rect, globalData.clrBtnDkShadow, globalData.clrBtnDkShadow);
  DeflateRect(&rect, 1, 1);
  draw_3drect_inR(hDC, &rect, globalData.clrBtnDkShadow, globalData.clrBtnDkShadow);
}
BOOL OnSetGridColorTheme()
{
  return TRUE;
}
COLORREF OnFillReportCtrlRowBackground(
    HDDC hDC, RECT rectFill,
    BOOL bSelected, BOOL bGroup, BOOL IsFocused)
{
  COLORREF clrText = 0;
  // Fill area:
  if (bSelected) {
    if (!IsFocused) {
      draw_fillrectR(hDC, &rectFill,
          globalData.clrBtnFace);
      clrText = s->m_clrReportGroupText;
    }
    else {
      draw_fillrectR(hDC, &rectFill, globalData.clrHilite);
      clrText = globalData.clrTextHilite;
    }
  }
  else {
    if (bGroup) {
      // no painting
      clrText = s->m_clrReportGroupText;
    }
  }
  // Return text color:
  return clrText;
}
COLORREF GetReportCtrlGroupBackgoundColor()
{
  return globalData.clrBtnLight;
}
void OnFillGridBackground(HDDC hDC, RECT rect)
{
  draw_fillrectR(hDC, &rect, globalData.clrWindow);
}
void OnDrawGridExpandingBox(HDDC hDC, RECT rect, BOOL bIsOpened, COLORREF colorBox)
{
  OnDrawExpandingBox(hDC, rect, bIsOpened, colorBox);
}
void OnFillGridHeaderBackground(HDDC hDC, RECT rect)
{
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
}
BOOL OnDrawGridHeaderItemBorder(HDDC hDC, RECT rect)
{
  draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnDkShadow);
  return TRUE;
}
void OnFillGridRowHeaderBackground(HDDC hDC, RECT rect)
{
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
}
BOOL OnDrawGridRowHeaderItemBorder(HDDC hDC, RECT rect)
{
  draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnDkShadow);
  return TRUE;
}
void OnFillGridSelectAllAreaBackground(HDDC hDC, RECT rect)
{
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
}
BOOL OnDrawGridSelectAllAreaBorder(HDDC hDC, RECT rect)
{
  draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnDkShadow);
  return TRUE;
}
typedef struct BCGP_GANTT_CHART_COLORS {
  COLORREF clrBackground;
  COLORREF clrRowBackground;
  COLORREF clrRowDayOff;
  COLORREF clrBtnFill;
  COLORREF clrGroupBarFill;
  COLORREF clrBtnComplete;
  COLORREF clrGridLine0;
  COLORREF clrGridLine1;
  COLORREF clrConnectorLines;
  COLORREF clrShadows;
  COLORREF clrSelection;
  COLORREF clrSelectionBorder;
} BCGP_GANTT_CHART_COLORS;
void PrepareColorScheme(COLORREF clrMain, BCGP_GANTT_CHART_COLORS* colors) // static
{
  double H, S, L, L1;
  RGBtoHSL(clrMain, &H, &S, &L);
  L1 = MAX(L, 0.85);
  clrMain = HLStoRGB_ONE(H, L1, S);
  colors->clrRowBackground = clrMain;
  colors->clrRowDayOff = HLStoRGB_ONE(H, L1 - 0.05, S);
  colors->clrBtnFill = HLStoRGB_ONE(H, L1 - 0.50, S);
  colors->clrBtnComplete = HLStoRGB_ONE(H, L1 - 0.20, S);
  colors->clrConnectorLines = MixColors(clrMain, _RGB(0, 0, 0), 0.85f);
  colors->clrGridLine0 = MixColors(clrMain, _RGB(192, 192, 192), 0.75f);
  colors->clrGridLine1 = MixColors(clrMain, _RGB(64, 64, 64), 0.75f);
  colors->clrShadows = MixColors(clrMain, _RGB(32, 32, 32), 0.8f);
}
void GetGanttColors(BCGP_GANTT_CHART_COLORS* colors, COLORREF clrBack)
{
  if (clrBack == CLR_DEFAULT) {
    clrBack = globalData.clrWindow;
  }
  PrepareColorScheme(clrBack, colors);
  colors->clrBackground = globalData.clrWindow;
  colors->clrShadows = s->m_clrMenuShadowBase;
  colors->clrRowBackground = colors->clrBackground;
  colors->clrGridLine0 = globalData.clrBtnShadow;
  colors->clrGridLine1 = globalData.clrBtnLight;
  colors->clrSelection = globalData.clrHilite;
  colors->clrSelectionBorder = globalData.clrHilite;
  colors->clrBtnFill = _RGB(0, 0, 255);
  colors->clrBtnComplete = _RGB(0, 255, 0);
}
void DrawGanttChartBackground(HDDC hDC, RECT rectChart, COLORREF clrFill)
{
  draw_fillrectR(hDC, &rectChart, clrFill);
}
void DrawGanttItemBackgroundCell(HDDC hDC, RECT rectClip, const BCGP_GANTT_CHART_COLORS* colors, BOOL bDayOff)
{
  draw_fillrectR(hDC, &rectClip, (bDayOff) ? colors->clrRowDayOff : colors->clrRowBackground);
}
void DrawGanttHeaderCell(HDDC hDC, RECT rectCell)
{
  draw_fillrectR(hDC, &rectCell, globalData.clrBtnFace);
  draw_3drect_inR(hDC, &rectCell, globalData.clrBtnHilite, globalData.clrBtnDkShadow);
}
COLORREF GetGanttHeaderTextColor(BOOL bHilite)
{
  return bHilite ? globalData.clrHotText : globalData.clrWindowText;
}
void DrawGanttHeaderText(HDDC hDC, RECT rectCell, RECT rectClip, const char* sCellText, BOOL bHilite, DWORD dwAlignment)
{
  RECT rcCell = rectCell;
  RECT rcVisible;
  COLORREF _clr;
  DWORD dwFlags = DT_VCENTER | DT_NOPREFIX | dwAlignment;
  DeflateRect4(&rcCell, 1, 2, 2, 2); // text padding
  IntersectRect(&rcVisible, &rectClip, &rcCell);
  if (IsRectEmpty(&rcVisible)) {
    return;
  }
  _clr = (GetGanttHeaderTextColor(bHilite));
  draw_textR2(hDC, sCellText, rcCell, dwFlags);
}
void FillGanttBar(HDDC hDC, RECT rectFill, COLORREF color)
{
  draw_fillrectR(hDC, &rectFill, color);
}
COLORREF GetPropListGroupColor()
{
  return globalData.clrBtnFace;
}
COLORREF GetPropListGroupTextColor()
{
  return globalData.clrBtnDkShadow;
}
COLORREF GetPropListCommandTextColor()
{
  return globalData.clrHotLinkText;
}
COLORREF GetMenuItemTextColor(
    BOOL bHighlighted, BOOL bDisabled)
{
  if (bHighlighted) {
    return bDisabled ? globalData.clrBtnFace : globalData.clrTextHilite;
  }
  return bDisabled ? globalData.clrGrayedText : globalData.clrWindowText;
}
int GetMenuDownArrowState()
{
  return (int) ImageBlack;
}
COLORREF GetStatusBarPaneTextColor(UINT nStyle)
{
  return (nStyle & UIS_DISABLED) ? globalData.clrGrayedText : globalData.clrBtnText;
}
void OnDrawRibbonCaption(HDDC hDC, RECT rect, RECT rectText, BOOL bExtra, BOOL bHide, BOOL IsScenicLook, img_t* hIcon, const char* strCaption)
{
  const DWORD dwStyleEx = TRUE;
  const BOOL bIsRTL = TRUE;
  BOOL bTextCenter = TRUE;
  if ((bHide && !bExtra) || IsScenicLook) {
    BOOL bDestroyIcon = FALSE;
    if (hIcon != NULL) {
      SIZE szIcon = iSIZE(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON));
      long x = rect.left + 2;
      long y = rect.top + MAX(0, (RCH(&rect) - szIcon.h) / 2);
      draw_image(hDC, x, y, x + szIcon.w, y + szIcon.h, hIcon, 0, 0, 0, 0);
      if (rectText.left < (x + szIcon.w + 4)) {
        rectText.left = x + szIcon.w + 4;
      }
    }
    bTextCenter = TRUE;
  }
  {
    DWORD dwTextStyle = DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER | (bIsRTL ? DT_RTLREADING | DT_RIGHT : 0);
    COLORREF clrText = _RGB(0, 0, 0);
    int widthFull = RCW(&rectText);
    int width = FontGetTextExtent(strCaption).w;
    if (bTextCenter && width < widthFull) {
      rectText.left += (widthFull - width) / 2;
    }
    rectText.right = MIN(rectText.left + width, rectText.right);
    if (rectText.right > rectText.left) {
      COLORREF _clr = (clrText);
      draw_textR2(hDC, strCaption, rectText, dwTextStyle);
    }
  }
}
BOOL IsMenuMode, IsDisabled, IsRibbonEdit, IsChecked, IsHighlighted, IsFocused, IsDroppedDown, bIsDrawOnGlass, IsPressed, IsMenuOnBottom;
COLORREF OnFillRibbonButton(HDDC hDC, RECT rect, RECT rectCommand)
{
  if (IsRibbonEdit) {
    COLORREF clrBorder = globalData.clrBtnShadow;
    rect.left = rectCommand.left;
    if (bIsDrawOnGlass) {
      DrawRect(hDC, rect, globalData.clrWindow, clrBorder);
    }
    else {
      if (IsDroppedDown || IsHighlighted) {
        draw_fillrectR(hDC, &rectCommand, globalData.clrWindow);
      }
      else {
        HighlightRect1(hDC, rectCommand);
      }
      draw_3drect_inR(hDC, &rect, clrBorder, clrBorder);
    }
    return 0;
  }
  if (IsMenuMode) {
    if (IsHighlighted) {
      draw_fillrectR(hDC, &rect, globalData.clrHilite);
      return globalData.clrTextHilite;
    }
  }
  else {
    if (IsChecked && !IsHighlighted) {
      if (bIsDrawOnGlass) {
        DrawRect(hDC, rect, globalData.clrWindow, 0);
      }
      else {
        FillDitheredRect(hDC, rect);
      }
    }
  }
  return 0;
}
void OnDrawRibbonButtonBorder(HDDC hDC, RECT rect, RECT rectMenu)
{
  if (IsRibbonEdit) {
    return;
  }
  if (IsMenuMode &&
      IsChecked && !IsHighlighted) {
    return;
  }
  if (IsHighlighted || IsChecked ||
      IsDroppedDown || IsFocused) {
    if (bIsDrawOnGlass) {
      DrawRect(hDC, rect, 0, globalData.clrBtnShadow);
    }
    else {
      if (IsPressed || IsChecked ||
          IsDroppedDown) {
        draw_3drect_inR(hDC, &rect, globalData.clrBtnShadow, globalData.clrBtnHilite);
      }
      else {
        draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnShadow);
      }
    }
    if (!IsRectEmpty(&rectMenu)) {
      if (bIsDrawOnGlass) {
        if (IsMenuOnBottom) {
          draw_line(hDC, rectMenu.left, rectMenu.top, rectMenu.right, rectMenu.top, globalData.clrBtnShadow);
        }
        else {
          draw_line(hDC, rectMenu.left, rectMenu.top, rectMenu.left, rectMenu.bottom, globalData.clrBtnShadow);
        }
      }
      else {
        int _x0, _y0;
        COLORREF _clr = (globalData.clrBtnShadow);
        if (IsMenuOnBottom) {
          TMoveTo(hDC, rectMenu.left, rectMenu.top);
          _LineTo(hDC, rectMenu.right, rectMenu.top);
        }
        else {
          TMoveTo(hDC, rectMenu.left, rectMenu.top);
          _LineTo(hDC, rectMenu.left, rectMenu.bottom);
        }
      }
    }
  }
}
void OnDrawRibbonCaptionButton(HDDC hDC, int nID, RECT rect, RECT rectCommand)
{
  int imageID;
  OnFillRibbonButton(hDC, rect, rectCommand);
  switch (nID) {
  case SC_CLOSE:
    imageID = IdClose;
    break;
  case SC_MINIMIZE:
    imageID = IdMinimize;
    break;
  case SC_MAXIMIZE:
    imageID = IdMaximize;
    break;
  case SC_RESTORE:
    imageID = IdRestore;
    break;
  default:
    return;
  }
  DrawMenuImages(hDC, imageID, rect, IsDisabled ? ImageGray : ImageBlack);
  OnDrawRibbonButtonBorder(hDC, rect, rect);
}
COLORREF OnDrawRibbonButtonsGroup()
{
  return 0;
}
RECT RectPS1(int x, int y, int cx, int cy)
{
  RECT rc = iRECT(x, y, x + cx, y + cy);
  return rc;
}
RECT RectPS(POINT pt, SIZE sz)
{
  RECT rc = iRECT(pt.x, pt.y, pt.x + sz.w, pt.y + sz.h);
  return rc;
}
void OnDrawDefaultRibbonImage(
    HDDC hDC, RECT rectImage,
    BOOL bIsDisabled)
{
  RECT rectBullet = RectPS(CenterPoint(&rectImage), iSIZE(1, 1));
  InflateRect(&rectBullet, 5, 5);
  DrawEllipseR(hDC, rectBullet,
      bIsDisabled ? globalData.clrGrayedText : _RGB(160, 208, 128),
      bIsDisabled ? globalData.clrBtnShadow : _RGB(71, 117, 44));
}
void OnDrawRibbonMainButton(HDDC hDC, RECT rect)
{
  const BOOL bIsHighlighted = IsHighlighted || IsFocused;
  const BOOL bIsPressed = IsPressed || IsDroppedDown;
  DeflateRect(&rect, 2, 2);
  DrawEllipseR(hDC, rect,
      bIsPressed ? globalData.clrBtnLight : globalData.clrBtnFace,
      bIsHighlighted ? globalData.clrBtnDkShadow : globalData.clrBtnShadow);
}
COLORREF OnDrawRibbonTabsFrame(
    HDDC hDC,
    RECT rectTab)
{
  int _x0, _y0;
  COLORREF _clr = (globalData.clrBtnShadow);
  TMoveTo(hDC, rectTab.left, rectTab.top);
  _LineTo(hDC, rectTab.right, rectTab.top);
  return 0;
}
void OnDrawRibbonCategory(HDDC hDC, RECT rectActiveTab, RECT rectCategory)
{
  int _x0, _y0;
  const int nShadowSize = 2;
  COLORREF _clr;
  rectCategory.right -= nShadowSize;
  rectCategory.bottom -= nShadowSize;
  draw_fillrectR(hDC, &rectCategory, globalData.clrBtnFace);
  _clr = (globalData.clrBtnShadow);
  TMoveTo(hDC, rectCategory.left, rectCategory.top);
  _LineTo(hDC, rectActiveTab.left + 1, rectCategory.top);
  TMoveTo(hDC, rectActiveTab.right - 2, rectCategory.top);
  _LineTo(hDC, rectCategory.right, rectCategory.top);
  _LineTo(hDC, rectCategory.right, rectCategory.bottom);
  _LineTo(hDC, rectCategory.left, rectCategory.bottom);
  _LineTo(hDC, rectCategory.left, rectCategory.top);
  DrawShadow(hDC, rectCategory, nShadowSize, 100, 75, NULL, NULL, s->m_clrMenuShadowBase, TRUE);
}
void OnDrawRibbonCategoryScroll(HDDC hDC, RECT rect, BOOL bIsLeft)
{
  rect.bottom--;
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
  if (IsHighlighted) {
    HighlightRect1(hDC, rect);
  }
  if (globalData.m_bIsRTL) {
    bIsLeft = !bIsLeft;
  }
  DrawMenuImages(hDC, bIsLeft ? IdArowLeftLarge : IdArowRightLarge, rect, ImageBlack);
  draw_3drect_inR(hDC, &rect, globalData.clrBtnShadow, globalData.clrBtnShadow);
}
enum BCGPRibbonCategoryColor {
  BCGPCategoryColor_None,
  BCGPCategoryColor_Red,
  BCGPCategoryColor_Orange,
  BCGPCategoryColor_Yellow,
  BCGPCategoryColor_Green,
  BCGPCategoryColor_Blue,
  BCGPCategoryColor_Indigo,
  BCGPCategoryColor_Violet
};
COLORREF RibbonCategoryColorToRGB(int color)
{
  if (IsHighContastMode()) {
    switch (color) {
    case BCGPCategoryColor_Red:
      return _RGB(255, 0, 0);
    case BCGPCategoryColor_Orange:
      return _RGB(255, 128, 0);
    case BCGPCategoryColor_Yellow:
      return _RGB(255, 255, 0);
    case BCGPCategoryColor_Green:
      return _RGB(0, 255, 0);
    case BCGPCategoryColor_Blue:
      return _RGB(0, 0, 255);
    case BCGPCategoryColor_Indigo:
      return _RGB(0, 0, 128);
    case BCGPCategoryColor_Violet:
      return _RGB(255, 0, 255);
    }
    return 0;
  }
  switch (color) {
  case BCGPCategoryColor_Red:
    return _RGB(255, 160, 160);
  case BCGPCategoryColor_Orange:
    return _RGB(239, 189, 55);
  case BCGPCategoryColor_Yellow:
    return _RGB(253, 229, 27);
  case BCGPCategoryColor_Green:
    return _RGB(113, 190, 89);
  case BCGPCategoryColor_Blue:
    return _RGB(128, 181, 196);
  case BCGPCategoryColor_Indigo:
    return _RGB(114, 163, 224);
  case BCGPCategoryColor_Violet:
    return _RGB(214, 178, 209);
  }
  return 0;
}
COLORREF OnDrawRibbonCategoryTab(HDDC hDC, RECT rectTab, int clrTab, BOOL bIsActive, BOOL bIsFocused, BOOL bIsHighlighted, BOOL IsSelected, int nTrancateRatio)
{
  DPOINT pts[8];
  int _x0, _y0;
  int len;
  rectTab.top += 3;
  if (nTrancateRatio > 0) {
    const int nPercent = MAX(10, 100 - nTrancateRatio / 2);
    COLORREF _clr = PixelAlpha2(globalData.clrBtnFace, nPercent);
    TMoveTo(hDC, rectTab.right - 1, rectTab.top);
    _LineTo(hDC, rectTab.right - 1, rectTab.bottom);
  }
  if (!bIsActive && !bIsHighlighted) {
    return globalData.clrBtnText;
  }
  rectTab.right -= 2;
  pts[0] = dPOINT(rectTab.left, rectTab.bottom);
  pts[1] = dPOINT(rectTab.left + 1, rectTab.bottom - 1);
  pts[2] = dPOINT(rectTab.left + 1, rectTab.top + 2);
  pts[3] = dPOINT(rectTab.left + 3, rectTab.top);
  pts[4] = dPOINT(rectTab.right - 3, rectTab.top);
  pts[5] = dPOINT(rectTab.right - 1, rectTab.top + 2);
  pts[6] = dPOINT(rectTab.right - 1, rectTab.bottom - 1);
  pts[7] = dPOINT(rectTab.right, rectTab.bottom);
  len = 8;
  if (bIsActive) {
    COLORREF clrFill = IsSelected ? globalData.clrBtnHilite : RibbonCategoryColorToRGB(clrTab);
    if (clrFill != 0) {
      draw_fillpoly(hDC, pts, len, clrFill);
    }
    else {
      draw_fillpoly(hDC, pts, len, bIsHighlighted ? globalData.clrWindow : globalData.clrBtnFace);
    }
  }
  draw_lines(hDC, pts, len, globalData.clrBtnShadow, 1);
  return globalData.clrBtnText;
}
COLORREF OnDrawRibbonPanel(HDDC hDC, RECT rectPanel, BOOL IsHighlighted, BOOL IsCollapsed)
{
  COLORREF clrText = globalData.clrBtnText;
  if (IsCollapsed) {
    draw_fillrectR(hDC, &rectPanel, globalData.clrHilite);
    clrText = globalData.clrTextHilite;
  }
  else if (IsHighlighted) {
    HighlightRect1(hDC, rectPanel);
  }
  draw_3drect_inR(hDC, &rectPanel, globalData.clrBtnHilite, globalData.clrBtnHilite);
  OffsetRect(&rectPanel, -1, -1);
  draw_3drect_inR(hDC, &rectPanel, globalData.clrBtnShadow, globalData.clrBtnShadow);
  return clrText;
}
COLORREF OnFillRibbonPanelCaption(HDDC hDC, RECT rectCaption)
{
  COLORREF br = (IsHighlighted ? globalData.clrActiveCaption : globalData.clrInactiveCaption);
  draw_fillrectR(hDC, &rectCaption, br);
  return IsHighlighted ? globalData.clrCaptionText : globalData.clrInactiveCaptionText;
}
void OnDrawRibbonPanelCaption(HDDC hDC, RECT rectCaption, const char* szName)
{
  const char* str = szName;
  COLORREF _clr, clrText;
  DeflateRect(&rectCaption, 1, 1);
  rectCaption.right -= 2;
  clrText = OnFillRibbonPanelCaption(hDC, rectCaption);
  _clr = (clrText);
  draw_textR2(hDC, str, rectCaption, DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX);
}
void OnDrawRibbonLaunchButton(HDDC hDC, RECT rect, RECT rectCommand, RECT rectMenu)
{
  COLORREF clrText;
  int imageState = ImageBlack;
  OnFillRibbonButton(hDC, rect, rectCommand);
  clrText = IsHighlighted ?
      globalData.clrCaptionText : globalData.clrInactiveCaptionText;
  if (IsDisabled) {
    imageState = ImageGray;
  }
  else if (!IsHighlighted) {
    if (GetRValue(clrText) > 192 &&
        GetGValue(clrText) > 192 &&
        GetBValue(clrText) > 192) {
      imageState = ImageWhite;
    }
    else {
      imageState = ImageBlack;
    }
  }
  DrawMenuImages(hDC, IdLaunchArrow, rect, imageState);
  OnDrawRibbonButtonBorder(hDC, rect, rectMenu);
}
enum RibbonElementLocation {
  RibbonElementNotInGroup,
  RibbonElementSingleInGroup,
  RibbonElementFirstInGroup,
  RibbonElementLastInGroup,
  RibbonElementMiddleInGroup,
};
enum RibbonImageType {
  RibbonImageLarge,
  RibbonImageSmall
};
void OnDrawRibbonDefaultPaneButtonContext(HDDC hDC, RECT rect, BOOL IsQATMode)
{
  RECT rectMenuArrow = rect;
  if (IsQATMode) {
    pButton->DrawImage(hDC, RibbonImageSmall, rect);
    return;
  }
  RECT rectImage = rect;
  rectImage.top += 10;
  rectImage.bottom = rectImage.top + pButton->GetImageSize(RibbonImageSmall).h;
  pButton->DrawImage(hDC, RibbonImageSmall, rectImage);
  // Draw text:
  pButton->DrawBottomText(hDC, FALSE);
}
void OnDrawRibbonDefaultPaneButton(HDDC hDC, BOOL IsQATMode)
{
  RECT rect = {0}, rectCommand = {0}, rectMenu = {0};
  if (IsQATMode) {
    OnFillRibbonButton(hDC, rect, rectCommand);
    OnDrawRibbonDefaultPaneButtonContext(hDC);
    OnDrawRibbonButtonBorder(hDC, rect, rectMenu);
  }
  else {
    OnDrawRibbonDefaultPaneButtonContext(hDC, pButton);
  }
}
void OnDrawRibbonDefaultPaneButtonIndicator(
    HDDC hDC,
    RECT rect,
    BOOL /*bIsSelected*/,
    BOOL /*bHighlighted*/)
{
  rect.left = rect.right - RCH(&rect);
  DeflateRect(&rect, 1, 1);
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
  draw_3drect_inR(hDC, &rect, globalData.clrBtnShadow, globalData.clrBtnShadow);
  RECT rectWhite = rect;
  rectWhite OffsetRect(&, 0, 1);
  DrawMenuImages(hDC, IdArowDown, rectWhite,
      ImageWhite);
  DrawMenuImages(hDC, IdArowDown, rect,
      ImageBlack);
}
COLORREF OnFillRibbonMainPanelButton(
    HDDC hDC,
)
{
  return OnFillRibbonButton(hDC, pButton);
}
void OnDrawRibbonMainPanelButtonBorder(
    HDDC hDC)
{
  OnDrawRibbonButtonBorder(hDC, pButton);
}
void OnFillRibbonEdit(
    HDDC hDC,
    CBCGPRibbonEditCtrl* /*pEdit*/,
    RECT rect,
    BOOL bIsHighlighted,
    BOOL /*bIsPaneHighlighted*/,
    BOOL bIsDisabled,
    COLORREF /*clrText*/, COLORREF /*clrSelBackground*/, COLORREF /*clrSelText*/)
{
  if (bIsHighlighted && !bIsDisabled) {
    if (bIsDrawOnGlass) {
      DrawRect(hDC, rect, globalData.clrWindow, 0);
    }
    else {
      draw_fillrectR(hDC, &rect, globalData.clrWindow);
    }
  }
  else {
    if (bIsDrawOnGlass) {
      DrawRect(hDC, rect, globalData.clrBtnFace, 0);
    }
    else {
      draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
      HighlightRect1(hDC, rect);
    }
  }
}
COLORREF GetRibbonEditBackgroundColor(
    BOOL bIsHighlighted,
    BOOL bIsDisabled)
{
  return (bIsHighlighted && !bIsDisabled) ? globalData.clrWindow : globalData.clrBtnFace;
}
void OnDrawRibbonMenuCheckFrame(
    HDDC hDC,
    RECT rect)
{
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
  draw_3drect_inR(hDC, &rect, globalData.clrBtnShadow,
      globalData.clrBtnHilite);
}
void OnDrawRibbonMainPanelFrame(
    HDDC hDC,
    RECT rect)
{
  draw_3drect_inR(hDC, &rect, globalData.clrBtnShadow, globalData.clrBtnShadow);
  InflateRect(&rect, 1, 1);
  draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnHilite);
}
void OnFillRibbonMenuFrame(
    HDDC hDC,
    RECT rect)
{
  draw_fillrectR(hDC, &rect, globalData.clrWindow);
}
void OnDrawRibbonRecentFilesFrame(
    HDDC hDC,
    RECT rect)
{
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
  RECT rectSeparator = rect;
  rectSeparator.right = rectSeparator.left + 2;
  draw_3drect_inR(hDC, &rectSeparator, globalData.clrBtnShadow,
      globalData.clrBtnHilite);
}
void OnDrawRibbonLabel(
    HDDC /*hDC*/,
    RECT /*rect*/)
{
}
void OnDrawRibbonPaletteButton(
    HDDC hDC,
    CBCGPRibbonPaletteIcon* pButton)
{
  OnFillRibbonButton(hDC, pButton);
  OnDrawRibbonButtonBorder(hDC, pButton);
}
void OnDrawRibbonPaletteBorder(
    HDDC hDC,
    CBCGPRibbonPaletteButton* /*pButton*/,
    RECT rectBorder)
{
  draw_3drect_inR(hDC, &rectBorder, globalData.clrBtnShadow, globalData.clrBtnShadow);
}
COLORREF OnDrawRibbonCategoryCaption(
    HDDC hDC,
    CBCGPRibbonContextCaption* pContextCaption)
{
  ASSERT(pContextCaption);
  COLORREF clrFill = RibbonCategoryColorToRGB(pContextCaption->GetColor());
  RECT rect = pContextCaption->GetRect();
  if (clrFill != 0) {
    if (bIsDrawOnGlass) {
      DeflateRect(&rect, 0, 1);
      DrawRect(hDC, rect, clrFill, 0);
    }
    else {
      COLORREF br = (clrFill);
      draw_fillrectR(hDC, &rect, br);
    }
  }
  return globalData.clrBtnText;
}
COLORREF OnDrawRibbonStatusBarPane(HDDC hDC, CBCGPRibbonStatusBar* /*pBar*/,
    CBCGPRibbonStatusBarPane* pPane)
{
  ASSERT(pPane);
  RECT rect = pPane->GetRect();
  if (pPane->IsHighlighted()) {
    RECT rectButton = rect;
    rectButton DeflateRect(&, 1, 1);
    draw_3drect_inR(hDC, &rectButton,
        pPane->IsPressed() ? globalData.clrBtnShadow : globalData.clrBtnHilite,
        pPane->IsPressed() ? globalData.clrBtnHilite : globalData.clrBtnShadow);
  }
  return 0;
}
void GetRibbonSliderColors(CBCGPRibbonSlider* /*pSlider*/,
    BOOL /*bIsHighlighted*/,
    BOOL /*bIsPressed*/,
    BOOL /*bIsDisabled*/,
    COLORREF /*clrLine*/,
    COLORREF /*clrFill*/)
{
  /* clrLine = bIsDisabled ? globalData.clrBtnShadow : globalData.clrBtnDkShadow;
  clrFill = bIsPressed && bIsHighlighted ?
  globalData.clrBtnShadow :
  bIsHighlighted ? globalData.clrBtnHilite : globalData.clrBtnFace;*/
}
void OnDrawRibbonSliderZoomButton(
    HDDC hDC, CBCGPRibbonSlider* /*pSlider*/,
    RECT rect, BOOL bIsZoomOut,
    BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled)
{
  COLORREF clrLine = bIsDisabled ? globalData.clrBtnShadow : globalData.clrBtnDkShadow;
  COLORREF clrFill = bIsPressed && bIsHighlighted ?
      globalData.clrBtnShadow :
      bIsHighlighted ? globalData.clrBtnHilite : globalData.clrBtnFace;
  POINT ptCenter = rect CenterPoint(&);
  RECT rectCircle(iPOINT(ptCenter.x - 7, ptCenter.y - 7), SIZE(15, 15));
  DrawEllipseR(hDC, rectCircle, clrFill, clrLine);
  // Draw +/- sign:
  RECT rectSign(iPOINT(ptCenter.x - 3, ptCenter.y - 3), SIZE(7, 7));
  if (bIsDrawOnGlass) {
    draw_line(hDC, rectSign.left, ptCenter.y, rectSign.right, ptCenter.y, clrLine);
    if (!bIsZoomOut) {
      draw_line(hDC, ptCenter.x, rectSign.top, ptCenter.x, rectSign.bottom, clrLine);
    }
  }
  else {
    COLORREF _clr = (clrLine);
    TMoveTo(hDC, rectSign.left, ptCenter.y);
    _LineTo(hDC, rectSign.right, ptCenter.y);
    if (!bIsZoomOut) {
      TMoveTo(hDC, ptCenter.x, rectSign.top);
      _LineTo(hDC, ptCenter.x, rectSign.bottom);
    }
  }
}
void OnDrawRibbonSliderChannel(
    HDDC hDC, CBCGPRibbonSlider* pSlider, RECT rect)
{
  BOOL bIsVert = FALSE;
  if (pSlider != NULL) {
    ASSERT(pSlider);
    bIsVert = pSlider->IsVert();
  }
  OnDrawSliderChannel(hDC, NULL, bIsVert, rect, bIsDrawOnGlass);
}
void OnDrawRibbonSliderThumb(
    HDDC hDC, CBCGPRibbonSlider* pSlider,
    RECT rect, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled)
{
  BOOL bIsVert = FALSE;
  BOOL bLeftTop = FALSE;
  BOOL bRightBottom = FALSE;
  if (pSlider != NULL) {
    ASSERT(pSlider);
    bIsVert = pSlider->IsVert();
    bLeftTop = pSlider->IsThumbLeftTop();
    bRightBottom = pSlider->IsThumbRightBottom();
  }
  OnDrawSliderThumb(hDC, NULL, rect, bIsHighlighted, bIsPressed, bIsDisabled,
      bIsVert, bLeftTop, bRightBottom, bIsDrawOnGlass);
}
void OnDrawRibbonProgressBar(HDDC hDC,
    CBCGPRibbonProgressBar* /*pProgress*/,
    RECT rectProgress, RECT rectChunk,
    BOOL /*bInfiniteMode*/)
{
  if (bIsDrawOnGlass) {
    if (!rectChunk.IsRectEmpty()) {
      DrawRect(hDC, rectChunk, globalData.clrHilite, 0);
    }
    DrawRect(hDC, rectProgress, 0, globalData.clrBtnShadow);
  }
  else {
    if (!rectChunk.IsRectEmpty()) {
      draw_fillrectR(hDC, &rectChunk, globalData.clrHilite);
    }
    draw_3drect_inR(hDC, &rectProgress, globalData.clrBtnShadow, globalData.clrBtnHilite);
  }
}
void OnFillRibbonQATPopup(
    HDDC hDC, RECT rect)
{
  draw_fillrectR(hDC, &rect, globalData.clrBtnFace);
}
void OnDrawRibbonQATSeparator(HDDC hDC,
    CBCGPRibbonSeparator* /*pSeparator*/, RECT rect)
{
  if (bIsDrawOnGlass) {
    DrawRect(hDC, rect, 0, globalData.clrBtnShadow);
  }
  else {
    draw_3drect_inR(hDC, &rect, globalData.clrBtnShadow, globalData.clrBtnHilite);
  }
}
void OnDrawRibbonKeyTip(HDDC hDC, CBCGPBaseRibbonElement* pElement,
    RECT rect, const char* str)
{
  ASSERT(pElement);
  COLORREF clrText = _GetSysColor(COLOR_INFOTEXT);
  COLORREF clrBorder = clrText;
  draw_fillrectR(hDC, &rect, _GetSysColor(COLOR_INFOBK));
  str.MakeUpper();
  COLORREF clrTextOld = hDC->SetTextColor(pElement->IsDisabled() ? globalData.clrGrayedText : clrText);
  draw_textR(hDC, str, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
  draw_3drect_inR(hDC, &rect, clrBorder, clrBorder);
}
void OnDrawRibbonRadioButtonOnList(HDDC hDC, CBCGPRibbonRadioButton* /*pRadioButton*/,
    RECT rect, BOOL /*bIsSelected*/, BOOL /*bHighlighted*/)
{
  OffsetRect(&rect, 1, 1);
  DrawMenuImages(hDC, IdRadio, rect, ImageWhite);
  OffsetRect(&rect, -1, -1);
  DrawMenuImages(hDC, IdRadio, rect, ImageBlack);
}
void OnDrawRibbonCheckBoxOnList(HDDC hDC, CBCGPRibbonCheckBox* /*pCheckBox*/,
    RECT rect, BOOL /*bIsSelected*/, BOOL /*bHighlighted*/)
{
  OffsetRect(&rect, 1, 1);
  DrawMenuImages(hDC, IdCheck, rect, ImageWhite);
  OffsetRect(&rect, -1, -1);
  DrawMenuImages(hDC, IdCheck, rect, ImageBlack);
}
COLORREF GetRibbonHyperlinkTextColor(CBCGPRibbonHyperlink* pHyperLink)
{
  ASSERT(pHyperLink);
  if (pHyperLink->IsDisabled()) {
    return GetToolbarDisabledTextColor();
  }
  return pHyperLink->IsHighlighted() ? globalData.clrHotLinkText : globalData.clrHotText;
}
COLORREF GetRibbonStatusBarTextColor(CBCGPRibbonStatusBar* /*pStatusBar*/)
{
  return globalData.clrBtnText;
}
void OnDrawRibbonColorPaletteBox(HDDC hDC, CBCGPRibbonColorButton* /*pColorButton*/,
    CBCGPRibbonPaletteIcon* /*pIcon*/,
    COLORREF color, RECT rect, BOOL bDrawTopEdge, BOOL bDrawBottomEdge,
    BOOL bIsHighlighted, BOOL bIsChecked, BOOL /*bIsDisabled*/)
{
  RECT rectFill = rect;
  rectFill DeflateRect(&, 1, 0);
  if (bIsHighlighted || bIsChecked) {
    FillDitheredRect(hDC, rect);
    rectFill DeflateRect(&, 1, 2);
  }
  if (color != 0) {
    COLORREF br = (color);
    draw_fillrectR(hDC, &rectFill, br);
  }
  COLORREF clrBorder = globalData.clrBtnShadow;
  if (bDrawTopEdge && bDrawBottomEdge) {
    draw_3drect_inR(hDC, &rect, clrBorder, clrBorder);
  }
  else {
    COLORREF penBorder = (clrBorder);
    COLORREF _clr = (penBorder);
    TMoveTo(hDC, rect.left, rect.top);
    _LineTo(hDC, rect.left, rect.bottom);
    TMoveTo(hDC, rect.right - 1, rect.top);
    _LineTo(hDC, rect.right - 1, rect.bottom);
    if (bDrawTopEdge) {
      TMoveTo(hDC, rect.left, rect.top);
      _LineTo(hDC, rect.right, rect.top);
    }
    if (bDrawBottomEdge) {
      TMoveTo(hDC, rect.left, rect.bottom - 1);
      _LineTo(hDC, rect.right, rect.bottom - 1);
    }
  }
  if (bIsHighlighted) {
    draw_3drect_inR(hDC, &rect, globalData.clrBtnHilite, globalData.clrBtnShadow);
  }
  else if (bIsChecked) {
    draw_3drect_inR(hDC, &rect, globalData.clrBtnShadow, globalData.clrBtnHilite);
  }
}
BOOL OnSetWindowRegion(CWnd* pWnd, SIZE sizeWindow)
{
  if (globalData.DwmIsCompositionEnabled()) {
    return FALSE;
  }
  ASSERT(pWnd);
  CBCGPRibbonBar* pRibbonBar = NULL;
  if (pWnd->IsKindOf(RUNTIME_CLASS(CBCGPFrameWnd))) {
    pRibbonBar = ((CBCGPFrameWnd*) pWnd)->GetRibbonBar();
  }
  else if (pWnd->IsKindOf(RUNTIME_CLASS(CBCGPMDIFrameWnd))) {
    pRibbonBar = ((CBCGPMDIFrameWnd*) pWnd)->GetRibbonBar();
  }
  if (pRibbonBar == NULL ||
      !pRibbonBar->IsWindowVisible() ||
      !pRibbonBar->IsReplaceFrameCaption()) {
    return FALSE;
  }
  const int nLeftRadius = 11;
  const int nRightRadius = 11;
  CRgn rgnWnd;
  rgnWnd.CreateRectRgn(0, 0, sizeWindow.w, sizeWindow.h);
  CRgn rgnTemp;
  rgnTemp.CreateRectRgn(0, 0, nLeftRadius / 2, nLeftRadius / 2);
  rgnWnd.CombineRgn(&rgnTemp, &rgnWnd, RGN_XOR);
  rgnTemp.DeleteObject();
  rgnTemp.CreateEllipticRgn(0, 0, nLeftRadius, nLeftRadius);
  rgnWnd.CombineRgn(&rgnTemp, &rgnWnd, RGN_OR);
  rgnTemp.DeleteObject();
  rgnTemp.CreateRectRgn(sizeWindow.w - nRightRadius / 2, 0, sizeWindow.w, nRightRadius / 2);
  rgnWnd.CombineRgn(&rgnTemp, &rgnWnd, RGN_XOR);
  rgnTemp.DeleteObject();
  rgnTemp.CreateEllipticRgn(sizeWindow.w - nRightRadius + 1, 0, sizeWindow.w + 1, nRightRadius);
  rgnWnd.CombineRgn(&rgnTemp, &rgnWnd, RGN_OR);
  pWnd->SetWindowRgn((HRGN)rgnWnd.Detach(), TRUE);
  return TRUE;
}
BOOL OnNcPaint(CWnd* /*pWnd*/, const CObList& /*lstSysButtons*/, RECT /*rectRedraw*/)
{
  return FALSE;
}
void OnActivateApp(CWnd* /*pWnd*/, BOOL /*bActive*/)
{
}
BOOL OnNcActivate(CWnd* /*pWnd*/, BOOL /*bActive*/)
{
  return FALSE;
}
SIZE GetNcBtnSize(BOOL /*bSmall*/)
{
  return SIZE(0, 0);
}
BOOL OnUpdateNcButtons(CWnd* /*pWnd*/, const CObList& /*lstSysButtons*/,)
{
  return FALSE;
}
BOOL OnEraseMDIClientArea(HDDC /*hDC*/, RECT /*rectClient*/)
{
  return FALSE;
}
BOOL GetToolTipParams(CBCGPToolTipParams& params,
    UINT /*nType*/ /*= (UINT)(-1)*/)
{
  CBCGPToolTipParams dummy;
  params = dummy;
  return TRUE;
}
void OnFillToolTip(HDDC hDC, CBCGPToolTipCtrl* /*pToolTip*/, RECT rect,
    COLORREF clrText, COLORREF clrLine)
{
  draw_fillrectR(hDC, &rect, _GetSysColor(COLOR_INFOBK));
}
BOOL DrawTextOnGlass(HDDC hDC, const char* strText, RECT rect,
    DWORD dwFlags, int nGlowSize, COLORREF clrText)
{
  COLORREF clrOldText = globalData.clrWindowText;
  hDC->SetTextColor(_RGB(0, 0, 0)); // TODO
  BOOL bRes = globalData.DrawTextOnGlass(s->m_hThemeButton, hDC, 0, 0, strText, rect,
      dwFlags, nGlowSize, clrText);
  hDC->SetTextColor(clrOldText);
  return bRes;
}
COLORREF OnFillListBoxItem(HDDC hDC, CBCGPListBox* /*pListBox*/, int /*nItem*/, RECT rect, BOOL bIsHighlihted, BOOL bIsSelected)
{
  COLORREF clrText = 0;
  if (bIsSelected) {
    draw_fillrectR(hDC, &rect, globalData.clrHilite);
    clrText = globalData.clrTextHilite;
  }
  if (bIsHighlihted) {
    hDC->draw_focus_rectR(rect);
  }
  return clrText;
}
void OnDrawGroup(HDDC hDC, CBCGPGroup* pGroup, RECT rect, const char* strName)
{
  ASSERT(pGroup);
  SIZE sizeText = FontGetTextExtent(strName);
  RECT rectFrame = rect;
  rectFrame.top += sizeText.h / 2;
  if (sizeText == SIZE(0, 0)) {
    rectFrame.top += FontGetTextExtent(_T("A")).h / 2;
  }
  int xMargin = sizeText.h / 2;
  RECT rectText = rect;
  rectText.left += xMargin;
  rectText.right = rectText.left + sizeText.w + xMargin;
  rectText.bottom = rectText.top + sizeText.h;
  if (!strName.IsEmpty()) {
    hDC->ExcludeClipRect(rectText);
  }
  {
    DrawRect(hDC, rectFrame, 0, globalData.clrBtnDkShadow);
  }
  hDC->SelectClipRgn(NULL);
  if (strName.IsEmpty()) {
    return;
  }
  DWORD dwTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_NOCLIP;
  if (bOnGlass) {
    DrawTextOnGlass(hDC, strName, rectText, dwTextStyle, 10, globalData.clrBtnText);
  }
  else {
    const char* strCaption = strName;
    draw_textR(hDC, strCaption, rectText, dwTextStyle);
  }
}
BOOL OnFillDialog(HDDC hDC, CWnd* pDlg, RECT rect)
{
  draw_fillrectR(hDC, &rect, &GetDlgBackBrush(pDlg));
  return TRUE;
}
COLORREF GetDlgBackBrush(CWnd* /*pDlg*/)
{
  return globalData.clrBtnFace;
}
COLORREF GetURLLinkColor(CBCGPURLLinkButton* /*pButton*/, BOOL bHover)
{
  return bHover ? globalData.clrHotLinkText : globalData.clrHotText;
}
BOOL CBCGPWinXPThemeManager::DrawComboBorder(HDDC hDC, RECT rect,
    BOOL bDisabled,
    BOOL bIsDropped,
    BOOL bIsHighlighted)
{
  if (s->m_hThemeWindow == NULL) {
    return FALSE;
  }
  if (bIsHighlighted || bIsDropped) {
    DeflateRect(&rect, 1, 1);
    draw_3drect_inR(hDC, &rect, globalData.clrHilite, globalData.clrHilite);
  }
  return TRUE;
}
#endif
int DrawSeparator(HDDC hDC, RECT rect, COLORREF pen1, COLORREF pen2, BOOL bHorz)
{
  RECT rect1 = (rect);
  RECT rect2;
  BOOL m_bIsDrawOnGlass = 0;
  if (bHorz) {
    rect1.top += RCH(&rect) / 2 - 1;
    rect1.bottom = rect1.top;
    rect2 = rect1;
    OffsetRect(&rect2, 0, 1);
  }
  else {
    rect1.left += RCW(&rect) / 2 - 1;
    rect1.right = rect1.left;
    rect2 = rect1;
    OffsetRect(&rect2, 1, 0);
  }
  if (m_bIsDrawOnGlass) {
    draw_line(hDC, rect1.left, rect1.top, rect1.right, rect1.bottom, pen1);
    draw_line(hDC, rect2.left, rect2.top, rect2.right, rect2.bottom, pen2);
  }
  else {
    DWORD _clr = pen1;
    int _x0, _y0;
    TMoveTo(hDC, rect1.left, rect1.top);
    _LineTo(hDC, rect2.right, rect2.bottom);
    _clr = pen2;
    TMoveTo(hDC, rect1.left, rect1.top);
    _LineTo(hDC, rect2.right, rect2.bottom);
  }
  return 0;
}
// 模仿DrawFrameControl
// lpRect ：矩形
// nType ：控件类型
// nState ：控件状态
int draw_frame_ctrl(HDDC hDC, const RECT* lpRect, UINT nType, UINT nState)
{
  char sss[1 << 16] = {0};
  struct {
    UINT a, b;
    const char* s;
    DWORD clrs[10];
  } aaa[] = {0};
  const char* s = 0;
  DWORD clr[10] = {COLOR_BTNTEXT};
  if (RCH(lpRect) <= 0 || RCW(lpRect) <= 0) {
    return 0;
  }
#define AAADEF(a, b, c) ((b)==(0xff&nState))? (c) :
  clr[0] = _GetSysColor(COLOR_BTNTEXT);
  if (DFC_SCROLL == nType) {
    s = AAADEF(DFC_SCROLL, DFCS_SCROLLUP, "5")
        AAADEF(DFC_SCROLL, DFCS_SCROLLDOWN, "6")
        AAADEF(DFC_SCROLL, DFCS_SCROLLLEFT, "3")
        AAADEF(DFC_SCROLL, DFCS_SCROLLRIGHT, "4")
        AAADEF(DFC_SCROLL, DFCS_SCROLLCOMBOBOX, "u")
        AAADEF(DFC_SCROLL, DFCS_SCROLLSIZEGRIP, "o")
        AAADEF(DFC_SCROLL, DFCS_SCROLLSIZEGRIPRIGHT, "x") 0;
  }
  if (DFC_MENU == nType) {
    s = AAADEF(DFC_MENU, DFCS_MENUARROW, "8")
        AAADEF(DFC_MENU, DFCS_MENUCHECK, "b")
        AAADEF(DFC_MENU, DFCS_MENUBULLET, "h")
        AAADEF(DFC_MENU, DFCS_MENUARROWRIGHT, "w") 0;
  }
  if (DFC_CAPTION == nType) {
    s = AAADEF(DFC_CAPTION, DFCS_CAPTIONCLOSE, "r")
        AAADEF(DFC_CAPTION, DFCS_CAPTIONMIN, "0")
        AAADEF(DFC_CAPTION, DFCS_CAPTIONMAX, "1")
        AAADEF(DFC_CAPTION, DFCS_CAPTIONRESTORE, "2")
        AAADEF(DFC_CAPTION, DFCS_CAPTIONHELP, "s") 0;
  }
  if (DFC_BUTTON == nType) {
    s = AAADEF(DFC_BUTTON, DFCS_BUTTONCHECK, "c")
        AAADEF(DFC_BUTTON, DFCS_BUTTONRADIOIMAGE, "0")
        AAADEF(DFC_BUTTON, DFCS_BUTTONRADIOMASK, "0")
        AAADEF(DFC_BUTTON, DFCS_BUTTON3STATE, "0")
        AAADEF(DFC_BUTTON, DFCS_BUTTONPUSH, "0") 0;
    if (DFCS_BUTTONRADIO == (0xff & nState)) {
      s = DFCS_CHECKED & nState ? "njklmi" : "njklm";
      ARRINIT6(clr, _GetSysColor(COLOR_WINDOW), _GetSysColor(COLOR_3DSHADOW),
          _GetSysColor(COLOR_3DHIGHLIGHT), _GetSysColor(COLOR_3DDKSHADOW),
          _GetSysColor(COLOR_3DLIGHT), _GetSysColor(COLOR_BTNTEXT));
    }
    if (DFCS_BUTTONCHECK == (0xff & nState)) {
      s = DFCS_CHECKED & nState ? "gcdefa" : "gcdef";
      ARRINIT6(clr, _GetSysColor(COLOR_WINDOW), _GetSysColor(COLOR_3DSHADOW),
          _GetSysColor(COLOR_3DHIGHLIGHT), _GetSysColor(COLOR_3DDKSHADOW),
          _GetSysColor(COLOR_3DLIGHT), _GetSysColor(COLOR_BTNTEXT));
    }
  }
#undef AAADEF
  if (s) {
    int i;
    RECT rc = *lpRect;
    int oldfont = SelectFontToMemDC("Marlett", RCH(lpRect), 0);
    HDDCSET_CLIP(hDC, &rc);
    for (i = 0; s[i]; ++i) {
      draw_text_impl(hDC, s + i, 1, lpRect->left, lpRect->top, clr[i], 0, 0, 0);
    }
    HDDCSET_CLIP(hDC, 0);
    TSelectFont(oldfont);
    return 1;
  }
  return 0;
}
int test_draw_frame_ctrl(HDDC hDC, const RECT rc)
{
  struct {
    int a, b;
    const char* sa;
    const char* sb;
  } aaa[] = {
#define AAADEF(a, b) {a, b, #a, #b}
    AAADEF(DFC_CAPTION, DFCS_CAPTIONCLOSE),
    AAADEF(DFC_CAPTION, DFCS_CAPTIONMIN),
    AAADEF(DFC_CAPTION, DFCS_CAPTIONMAX),
    AAADEF(DFC_CAPTION, DFCS_CAPTIONRESTORE),
    AAADEF(DFC_CAPTION, DFCS_CAPTIONHELP),
    AAADEF(DFC_MENU, DFCS_MENUARROW),
    AAADEF(DFC_MENU, DFCS_MENUCHECK),
    AAADEF(DFC_MENU, DFCS_MENUBULLET),
    AAADEF(DFC_MENU, DFCS_MENUARROWRIGHT),
    AAADEF(DFC_SCROLL, DFCS_SCROLLUP),
    AAADEF(DFC_SCROLL, DFCS_SCROLLDOWN),
    AAADEF(DFC_SCROLL, DFCS_SCROLLLEFT),
    AAADEF(DFC_SCROLL, DFCS_SCROLLRIGHT),
    AAADEF(DFC_SCROLL, DFCS_SCROLLCOMBOBOX),
    AAADEF(DFC_SCROLL, DFCS_SCROLLSIZEGRIP),
    AAADEF(DFC_SCROLL, DFCS_SCROLLSIZEGRIPRIGHT),
    AAADEF(DFC_BUTTON, DFCS_BUTTONCHECK),
    AAADEF(DFC_BUTTON, DFCS_BUTTONCHECK | DFCS_CHECKED),
    AAADEF(DFC_BUTTON, DFCS_BUTTONRADIOIMAGE),
    AAADEF(DFC_BUTTON, DFCS_BUTTONRADIOMASK),
    AAADEF(DFC_BUTTON, DFCS_BUTTONRADIO),
    AAADEF(DFC_BUTTON, DFCS_BUTTONRADIO | DFCS_CHECKED),
    AAADEF(DFC_BUTTON, DFCS_BUTTON3STATE),
    AAADEF(DFC_BUTTON, DFCS_BUTTONPUSH),
#undef AAADEF
  };
  RECT rc1[countof(aaa)];
  RECT rc2[3];
  int i;
  RectMatrix(rc, (countof(rc1) + 7) / 8, 8, 10, 10, countof(rc1), rc1);
  for (i = 0; i < countof(aaa); ++i) {
    RectCutTop(&rc1[i], -20, 0, rc2);
    RectCutTop(&rc1[i], -20, 0, rc2 + 1);
    rc1[i] = RectToSquare(rc1[i]); // 正方形
    draw_frame_ctrl(hDC, &rc1[i], aaa[i].a, aaa[i].b);
    draw_textR(hDC, aaa[i].sa, -1, &rc2[0], DT_CENTER, rgb(0, 0, 0));
    draw_textR(hDC, aaa[i].sb, -1, &rc2[1], DT_CENTER, rgb(0, 0, 0));
  }
  return 0;
}
#if 0
int LinearGradient(HDDC hDC, LPCRECT rect, COLORREF clrFrom, COLORREF clrTo, BOOL bHorizontal)
{
  const int NCOLORSHADES = 255;
  int cxCap = rect->right - rect->left;
  int cyCap = rect->bottom - rect->top;
  // Get the intensity values for the ending color
  int r1 = GetRV(clrTo); // red
  int g1 = GetGV(clrTo); // green
  int b1 = GetBV(clrTo); // blue
  // Get the intensity values for the begining color
  int r2 = GetRV(clrFrom); // red
  int g2 = GetGV(clrFrom); // green
  int b2 = GetBV(clrFrom); // blue
  int r, g, b;
  if (bHorizontal) {
    //paint horizontal rect;
    int x = cxCap;
    int w = x; // width of area to shade
    int xDelta = MAX(w / NCOLORSHADES, 1); // width of one shade band
    while (x >= xDelta) {
      x -= xDelta;
      if (r1 > r2) {
        r = r1 - (r1 - r2) * (w - x) / w;
      }
      else {
        r = r1 + (r2 - r1) * (w - x) / w;
      }
      if (g1 > g2) {
        g = g1 - (g1 - g2) * (w - x) / w;
      }
      else {
        g = g1 + (g2 - g1) * (w - x) / w;
      }
      if (b1 > b2) {
        b = b1 - (b1 - b2) * (w - x) / w;
      }
      else {
        b = b1 + (b2 - b1) * (w - x) / w;
      }
      PaintRect(hDC, rect->left + x, rect->top, xDelta, cyCap, _RGB(r, g, b));
    }
  }
  else {
    //paint vertical rect;
    int y = cyCap;
    int w = y; // height of area to shade
    int yDelta = MAX(w / NCOLORSHADES, 1); // height of one shade band
    //while (y >= yDelta) {
    while (y > 0) {
      y -= yDelta;
      if (r1 > r2) {
        r = r1 - (r1 - r2) * (w - y) / w;
      }
      else {
        r = r1 + (r2 - r1) * (w - y) / w;
      }
      if (g1 > g2) {
        g = g1 - (g1 - g2) * (w - y) / w;
      }
      else {
        g = g1 + (g2 - g1) * (w - y) / w;
      }
      if (b1 > b2) {
        b = b1 - (b1 - b2) * (w - y) / w;
      }
      else {
        b = b1 + (b2 - b1) * (w - y) / w;
      }
      PaintRect(hDC, rect->left, rect->top + y, cxCap, yDelta, _RGB(r, g, b));
    }
  }
  return 0;
}
int DrawElectricity(HDDC hDC, LPCRECT rect)
{
  static int on1, on2, oon1, oon2;
  int nPivot = RCWIDTH(rect);
  int n1, n2, nRandElement;
  n1 = rand() * nPivot / RAND_MAX - nPivot / 2;
  n2 = rand() * nPivot / RAND_MAX - nPivot / 2;
  //
  // Add extra random element: (2/10 chance of making some wild stuff on one of the curves)
  //
  nRandElement = rand() * 10 / RAND_MAX;
  if (nRandElement < 2) {
    n1 = rand() * (nPivot * 2) / RAND_MAX - nPivot;
  }
  else if (rand() < RAND_MAX / 10) {
    n2 = rand() * (nPivot * 2) / RAND_MAX - nPivot;
  }
  {
    POINT TopRight = {rect->right, rect->top};
    POINT TopLeft = {rect->left, rect->top};
    POINT BottomLeft = {rect->left, rect->bottom};
    POINT BottomRight = {rect->right, rect->bottom};
    int nMidHorizontal = rect->left + ((rect->right - rect->left) / 2);
    int nMidVertical = rect->top + ((rect->bottom - rect->top) / 2);
    //
    // Top Right -> Bottom Left
    //
    POINT points1[4] = {
      TopRight.x, TopRight.y, nMidHorizontal + n1, nMidVertical, nMidHorizontal, nMidVertical - n1, BottomLeft.x, BottomLeft.y
    };
    // Top Left -> Bottom Right
    POINT points2[4] = {
      TopLeft.x, TopLeft.y, nMidHorizontal - n2, nMidVertical, nMidHorizontal, nMidVertical - n2, BottomRight.x, BottomRight.y,
    };
    // Top Right -> BottomLeft
    POINT opoints1[4] = {
      TopRight.x, TopRight.y, nMidHorizontal + on1, nMidVertical, nMidHorizontal, nMidVertical - on1, BottomLeft.x, BottomLeft.y,
    };
    // Top Left -> Bottom Right
    POINT opoints2[4] = {
      TopLeft.x, TopLeft.y, nMidHorizontal - on2, nMidVertical, nMidHorizontal, nMidVertical - on2, BottomRight.x, BottomRight.y,
    };
    // Top Right -> BottomLeft
    POINT oopoints1[4] = {
      TopRight.x, TopRight.y, nMidHorizontal + oon1, nMidVertical, nMidHorizontal, nMidVertical - oon1, BottomLeft.x, BottomLeft.y,
    };
    // Top Left -> Bottom Right
    POINT oopoints2[4] = {
      TopLeft.x, TopLeft.y, nMidHorizontal - oon2, nMidVertical, nMidHorizontal, nMidVertical - oon2, BottomRight.x, BottomRight.y,
    };
    COLORREF wpen = _RGB(150, 220, 255);
    COLORREF pen = _RGB(50, 100, 255);
    COLORREF open = _RGB(50, 100, 255);
    COLORREF oopen = _RGB(0, 30, 150);
    TDrawBezier(hDC, oopoints1, 4, oopen);
    TDrawBezier(hDC, oopoints2, 4, oopen);
    TDrawBezier(hDC, opoints1, 4, open);
    TDrawBezier(hDC, opoints2, 4, open);
    TDrawBezier(hDC, points1, 4, pen);
    TDrawBezier(hDC, points2, 4, pen);
    TDrawBezier(hDC, points1, 4, wpen);
    TDrawBezier(hDC, points2, 4, wpen);
    oon1 = on1;
    oon2 = on2;
    on1 = n1;
    on2 = n2;
  }
  return 0;
}
// dwCycles is the number of full waves
int DrawSineWave(HDDC hDC, LPCRECT rect, double dwCycles, COLORREF crPen)
{
  COLORREF Marker = (crPen);
  int nWidth = RCHEIGHT(rect);
  double dwStep = (2 * PI * dwCycles) / ((double)RCWIDTH(rect));
  double dwAmplitude = RCHEIGHT(rect) / 2;
  BOOL bFirstDot = TRUE;
  BOOL bDotsOnly = FALSE;
  int x;
  int _x0, _y0;
  ASSERT(dwCycles > 0);
  for (x = 0; x < RCWIDTH(rect); x++) {
    int nOffset = (rect->top) + RCHEIGHT(rect) / 2;
    int yPos = (int)(sin(x * dwStep) * (dwAmplitude)) + nOffset;
    int xPos = x + rect->left;
    if (bFirstDot && !bDotsOnly) {
      TMoveTo(hDC, xPos, yPos);
      bFirstDot = FALSE;
    }
    if (bDotsOnly) {
      _SetPixel(hDC, xPos, yPos, crPen);
    }
    else {
      TLineTo(hDC, xPos, yPos, crPen);
    }
  }
  return 0;
}
int test_drawUtils(HDDC hDC)
{
  int m_nSampleNumber;
  double m_dDegrees;
  int x = 0, y = 0;
  RECT rc = iRECT(x, y, hDC->w, y + hDC->h);
  RECT m_recClient = rc;
  RECT m_DisplayRect[20];
  RECT Border;
  int nIndex = 0;
  m_nSampleNumber = 0;
  m_dDegrees = 0;
  m_DisplayRect[3] = iRECT(10, 10, 80, 80);
  m_DisplayRect[4] = iRECT(110, 10, 80, 80);
  m_DisplayRect[5] = iRECT(210, 10, 80, 80);
  m_DisplayRect[6] = iRECT(310, 10, 80, 80);
  m_DisplayRect[7] = iRECT(410, 10, 80, 80);
  m_DisplayRect[8] = iRECT(510, 10, 80, 80);
  m_DisplayRect[9] = iRECT(10, 110, 180, 80);
  m_DisplayRect[10] = iRECT(210, 110, 80, 80);
  m_DisplayRect[11] = iRECT(310, 110, 80, 80);
  m_DisplayRect[12] = iRECT(10, 210, 150, 150);
  m_DisplayRect[13] = iRECT(210, 210, 180, 80);
  m_DisplayRect[14] = iRECT(410, 210, 180, 80);
  if (1) {
    RECT TileRect = (m_recClient);
    int nCentre = (RCHEIGHT(&m_recClient) / 2);
    TileRect.top = nCentre - (RCHEIGHT(&m_recClient) / 4);
    TileRect.bottom = nCentre + (RCHEIGHT(&m_recClient) / 4);
    //TileBitmap( hDC, &TileRect, IDB_BITMAP_TILED );
    Draw3DRect(hDC, &TileRect, FALSE, - 1, _RGB(0, 0, 255), _RGB(0, 0, 255));
  }
  if (1) {
    char* s1 = "phttp://www.greystonefx.com";
    char* s2 = "Courier New";
    char* s3 = "Arial";
    SelectFontToMemDC("Tahoma", 22, 0, 0);
    TTextOut(hDC, 10, m_recClient.bottom - 150, s1, _RGB(255, 215, 215));
    SelectFontToMemDC("Courier New", 32, 0, 0);
    TTextOut(hDC, 10, m_recClient.bottom - 100, s2, _RGB(255, 255, 255));
    SelectFontToMemDC("Arial", 32, 0, 0);
    TTextOut(hDC, 10, m_recClient.bottom - 50, s3, _RGB(215, 255, 215));
  }
  if (1) {
    Border = (m_DisplayRect[nIndex++]);
    Draw3DRect(hDC, &Border, FALSE, _GetSysColor(COLOR_BTNFACE), - 1, - 1);
  }
  if (1) {
    Border = (m_DisplayRect[nIndex++]);
    Draw3DRect(hDC, &Border, TRUE, - 1, - 1, - 1);
    LinearGradient(hDC, &Border, _RGB(0, 0, 50), _RGB(0, 0, 200), TRUE);
  }
  if (1) {
    Border = (m_DisplayRect[nIndex++]);
    Draw3DRect(hDC, &Border, TRUE, _RGB(0, 0, 100), - 1, - 1);
    DrawElectricity(hDC, &Border);
  }
  if (1) {
    Border = (m_DisplayRect[nIndex++]);
    Draw3DRect(hDC, &Border, TRUE, - 1, - 1, - 1);
    DrawElectricity(hDC, &Border);
  }
  if (1) {
    Border = (m_DisplayRect[nIndex++]);
    Draw3DRect(hDC, &Border, TRUE, - 1, - 1, - 1);
    RadialGradient(hDC, &Border, _RGB(0, 0, 50), _RGB(0, 0, 200));
  }
  // SINE WAVE:
  if (1) {
    Border = (m_DisplayRect[nIndex++]);
    Draw3DRect(hDC, &Border, TRUE, _GetSysColor(COLOR_BTNFACE), - 1, - 1);
    DrawSineWave(hDC, &Border, 2 /*Cycles*/, _RGB(0, 0, 50));
  }
  if (1) {
    Border = (m_DisplayRect[nIndex++]);
    Draw3DRect(hDC, &Border, TRUE, _RGB(0, 0, 0), - 1, - 1);
  }
  // GLASS 1
  if (1) {
    Border = (m_DisplayRect[nIndex++]);
    Draw3DRect(hDC, &Border, TRUE, - 1, - 1, - 1);
  }
  if (1) {
    Border = (m_DisplayRect[nIndex++]);
    Draw3DRect(hDC, &Border, TRUE, - 1, - 1, - 1);
    //PaintBitmap( hDC, this, Border, IDB_BITMAP_TEST_OVERLAY, TRUE, TRUE );
  }
  if (1) {
    Border = (m_DisplayRect[nIndex++]);
    Draw3DRect(hDC, &Border, TRUE, - 1, - 1, - 1);
    //PaintBitmap( hDC, this, Border, IDB_QUAKE_BG, FALSE, FALSE );
    //BlitBitmap( hDC, &Border, IDB_QUAKE_MODEL, _RGB(0,0,0), m_dDegrees /* Degrees */ );
  }
  return 0;
}
int TGradientFillY(HDDC hDC, LPCRECT lpRect, COLORREF clr1, COLORREF clr2)
{
  int i, alpha, h, w, al;
  RECT rc = *lpRect;
  DWORD* pix, clr;
  if (clr1 == clr2) {
    return TFillRect(hDC, lpRect, clr1);
  }
  TClipRect(hDC, &rc);
  h = RCHEIGHT(&rc), w = RCWIDTH(&rc), al = (hDC->bw >> 2);
  pix = &_Pixel(hDC, rc.left, rc.top);
  for (i = 0; i < h; ++i, pix += al) {
    alpha = i * 256 / (h - 1);
    alpha = i * i * 256 / ((h - 1) * (h - 1));
    clr = ALPHACOLOR(clr1, clr2, alpha);
    Fill1D(w, pix, clr);
  }
  return 0;
}
//绘制小球(运动部件)
//(x,y) 左上角位置
//nRadius 半径
//设置扩散区域和高光区域的逼近精度
//nDifPrecision-----漫反射区域绘制次数
//nSpePrecision-----高光区域绘制次数
//二者可以决定漫反射区域和高光区域所占有的比例
int DrawSphere(HDDC hDC, int x, int y, int nRadius, int nDifPrecision, int nSpePrecision, DWORD clr)
{
  int i;
  //半径
  int m_nRadius;
  //主体颜色
  BYTE m_byRed, m_byGreen, m_byBlue;
  //精细程度
  //扩散色区域的逼近次数
  int m_nDifPrecision;
  //高光区域的逼近次数
  int m_nSpePrecision;
  m_nRadius = nRadius;
  if (m_nRadius < 1) {
    m_nRadius = 1;
  }
  m_byRed = 192;
  m_byGreen = 192;
  m_byBlue = 192;
  m_nDifPrecision = 8;
  m_nSpePrecision = 4;
  m_byRed = GetRV(clr);
  m_byGreen = GetGV(clr);
  m_byBlue = GetBV(clr);
  m_nDifPrecision = nDifPrecision;
  m_nSpePrecision = nSpePrecision;
  if (m_nDifPrecision < 1) {
    m_nDifPrecision = 1;
  }
  if (m_nSpePrecision < 1) {
    m_nDifPrecision = 1;
  }
  {
    int nSpeTimes;
    //漫反射光逼近次数
    int nDifTimes = MIN(m_nDifPrecision, m_nRadius);
    //分母
    int nDenominator = (7 * nDifTimes);
    //绘制漫反射区域
    for (i = 0; i < nDifTimes; i++) {
      //颜色
      BYTE byRed = (BYTE)((i * m_byRed * 4) / nDenominator + 3 * (int)m_byRed / 7);
      BYTE byGreen = (BYTE)((i * m_byGreen * 4) / nDenominator + 3 * (int)m_byGreen / 7);
      BYTE byBlue = (BYTE)((i * m_byBlue * 4) / nDenominator + 3 * (int)m_byBlue / 7);
      //画刷
      DWORD brush = _RGB(byRed, byGreen, byBlue);
      //大小
      int nSize = m_nRadius - (5 * i) / 4 + 1;
      int r = nSize / 2;
      //绘制
      TDrawEllipse(hDC, x + i + r, y + (i * 2) / 5 + r, r, r, brush);
    }
    //绘制高光区域
    //高光逼近次数
    nSpeTimes = MIN(m_nSpePrecision, (m_nRadius - nDifTimes));
    if (nSpeTimes < 1) {
      return 0;
    }
    //暂时变量,分母
    nDenominator = (2 * nSpeTimes);
    //绘制高光区域
    for (i = 0; i < nSpeTimes; i++) {
      int nIndex = i + nDifTimes;
      //颜色
      BYTE byRed = (BYTE)(m_byRed + ((255 - m_byRed) * i) / nDenominator);
      BYTE byGreen = (BYTE)(m_byGreen + ((255 - m_byGreen) * i) / nDenominator);
      BYTE byBlue = (BYTE)(m_byBlue + ((255 - m_byBlue) * i) / nDenominator);
      //画刷
      DWORD brush = (_RGB(byRed, byGreen, byBlue));
      //左上角坐标
      int xLT = x + nIndex;
      int yLT = y + (nIndex * 2) / 5;
      //大小
      int nSize = m_nRadius - (5 * nIndex) / 4 + 1;
      int xRB = xLT + nSize;
      int yRB = yLT + nSize;
      //绘制
      FillEllipse(hDC, (xLT + xRB) / 2, (yLT + yRB) / 2, abs(xLT - xRB) / 2, abs(yLT - yRB) / 2, brush, 1);
    }
  }
  return 0;
}
int test_DrawSphere(HDDC hDC)
{
  int nClientWidth = hDC->w;
  int nClientHeight = hDC->h;
  //小球运动坐标, 运动速度
  static int x = 9, y = 30, m_nSpeedX = 9, m_nSpeedY = 1;
  x += m_nSpeedX;
  //控制 x 坐标
  if (x > nClientWidth - 25) {
    x = nClientWidth - 25;
    m_nSpeedX = - m_nSpeedX;
  }
  else if (x < 9) {
    x = 9;
    m_nSpeedX = - m_nSpeedX;
  }
  //控制 y 坐标
  y += m_nSpeedY;
  m_nSpeedY++;
  if (y > (nClientHeight - 26)) {
    y = 2 * (nClientHeight - 26) - y;
    m_nSpeedY = - m_nSpeedY + 4;
  }
  else if (y < 30) {
    y = 30;
  }
  //TFillRect(hDC, 0, 0);
  DrawSphere(hDC, x, y, 36, 8, 4, _RGB(255, 255, 255));
  //DrawSphere(hDC, 100, 100, 36, 8, 4, _RGB(255, 255, 255));
  return 0;
}
#define STATE_REDRAW (1<<31)
#define GEDIT_SHOWOUTERFRAME 0x0001 //外边框
#define GEDIT_SHOWCURSOR 0x0002 //光标
#define GEDIT_NUMONLY 0x0004 //纯数字
#define GetStage(_ctr, _S) ((_ctr)->nState&(_S))
#define SetStage1(_ctr, _S) if (!((_ctr)->nState&(_S))) {(_ctr)->nState|=(_S);(_ctr)->nState|=STATE_REDRAW;}
#define SetStage0(_ctr, _S) if ((_ctr)->nState&(_S)) {(_ctr)->nState&=~(_S);if (STATE_REDRAW!=(_S)) (_ctr)->nState|=STATE_REDRAW;}
int TDrawButton(HDDC hDC, LPCRECT lpRect, UINT nState, const char* lpText, int tLen)
{
  POINT pt = {1, 1};
  RECT rc = *lpRect;
  COLORREF color1, color2;
  DWORD crFace = _GetSysColor(COLOR_BTNFACE);
  DWORD crLight = _GetSysColor(COLOR_BTNHIGHLIGHT);
  DWORD crShadow = _GetSysColor(COLOR_BTNSHADOW);
  DWORD crText = _GetSysColor(COLOR_BTNTEXT);
  DWORD cr3DShadow = _GetSysColor(COLOR_3DDKSHADOW);
  DWORD crBlack = _RGB(0, 0, 0);
  color1 = crFace;
  color2 = crFace;
  if (nState & (ODS_OVER)) {
    color1 = color2 = ALPHACOLOR(crFace, 0xffffff, 53);
  }
  if (nState & (ODS_SELECTED)) {
    color1 = color2 = ALPHACOLOR(crFace, 0xffffff, 153);
    RCOFFSET(&rc, 1, 1, 1, 1);
  }
  //绘制按钮的底色
  TGradientFillY(hDC, lpRect, color1, color2);
  if (1) {
    RECT rc2 = *lpRect;
    //绘制按钮的内边框
    if (nState & (ODS_SELECTED)) {
      TDraw3DRect(hDC, lpRect, crBlack, crBlack);
      RCOFFSET(&rc2, 1, 1, - 1, - 1);
      TDraw3DRect(hDC, &rc2, crShadow, crShadow);
    }
    else {
      RCOFFSET(&rc2, 0, 0, - 1, - 1);
      TDraw3DRect(hDC, &rc2, crLight, crShadow);
      TDraw3DRect(hDC, lpRect, crLight, cr3DShadow);
    }
  }
  //绘制按钮的外边框
  if (nState & (ODS_SELECTED)) {
    //TRoundRect( hDC, lpRect, pt, _RGB(0, 0, 0) );
  }
  //TDrawText(hDC, lpString, strlen(lpString), &rc, DT_CENTER, _GetSysColor(COLOR_BTNTEXT));
  TDrawText2(hDC, lpText, tLen, &rc, DT_CENTER, _RGB(0, 0, 0));
  return 0;
}
int TDrawEdit(HDDC hDC, LPCRECT lpRect, UINT nState, int nTextPosX, const char* lpText, int tLen, int nCursorPos, int nSelBegin, int nSelEnd)
{
  COLORREF clrBorder = _RGB(0, 0, 0);
  COLORREF clrText = _GetSysColor(COLOR_BTNTEXT);
  COLORREF clrTextSel = _RGB(255, 255, 255);
  COLORREF clrBg = _RGB(255, 255, 255);
  COLORREF clrBgSel = _RGB(0, 0, 255);
  int nEditW, nEditH;
  RECT rc2 = *lpRect;
  RCOFFSET(&rc2, 1, 1, - 1, - 1);
  nEditW = RCWIDTH(&rc2), nEditH = RCHEIGHT(&rc2);
  nTextPosX += lpRect->left;
  // 保存编辑框区域
  //SetRect(&m_rcWorld,x,y,x+m_nEditW,y+m_nEditH);
  // 画背景
  TFillRect(hDC, lpRect, clrBg);
  // 画文字
  {
    int text_x = nTextPosX;
    TDrawText(hDC, lpText, &rc2, DT_VCENTER);
    if (nSelBegin > nSelEnd) {
      int t = nSelBegin;
      nSelBegin = nSelEnd;
      nSelEnd = t;
    }
    if (nSelBegin < nSelEnd) {
      RECT rc3 = rc2;
      RCOFFSET(&rc3, 1, 1, - 1, - 1);
      TFillRect(hDC, &rc3, clrBgSel);
      TDrawText2(hDC, lpText + nSelBegin, nSelEnd - nSelBegin, &rc3, DT_VCENTER, clrTextSel);
      //printf("%d %d\n", nSelBegin, nSelEnd);
    }
  }
  // 画外框
  if (GEDIT_SHOWOUTERFRAME & nState) {
    TDrawRect(hDC, lpRect, clrBorder);
  }
  // 画光标
  if (GEDIT_SHOWCURSOR & nState) {
    HDC hMemDC = GetMemDC();
    int nCursorWidth = 1;
    RECT rc = *lpRect;
    int char_x = nTextPosX;
    int t;
    TEXTMETRIC tm;
    GetTextMetrics(hMemDC, &tm);
    t = (RCHEIGHT(&rc) - tm.tmHeight) / 2;
    if (nCursorPos > 0) {
      SIZE szTmp = FontGetTextSize(lpText, nCursorPos);
      char_x = nTextPosX + szTmp.w;
    }
    RCOFFSET(&rc, 0, t, 0, - t);
    rc.left = char_x - nCursorWidth / 2;
    rc.left = BOUND(char_x - nCursorWidth, rc2.left, rc2.right);
    rc.right = rc.left + nCursorWidth;
    TFillRect(hDC, &rc, clrText);
  }
  //m_pHGE->Gfx_SetClipping();
  return 0;
}
typedef struct _POINTV1 {
  union {
    POINT p;
    struct {
      int x, y;
    };
  };
  DWORD c;
  float z;
} POINTV1;
int GradientFillTri(HDDC hDC, POINT a, POINT b, POINT c, DWORD clr1, DWORD clr2, DWORD clr3)
{
  int x12, n, i1;
  DWORD c12;
  POINTV1 pt3[3], t;
  pt3[0].p = a, pt3[1].p = b, pt3[2].p = c;
  pt3[0].c = clr1, pt3[1].c = clr2, pt3[2].c = clr3;
  TriSort(pt3, t);
  if (pt3[1].y == pt3[0].y) {
    GradientFillTiXin(hDC, pt3[0].x, pt3[1].x, pt3[0].y, pt3[2].x, pt3[2].x, pt3[2].y, pt3[0].c, pt3[1].c, pt3[2].c, pt3[2].c);
    return 0;
  }
  if (pt3[1].y == pt3[2].y) {
    GradientFillTiXin(hDC, pt3[0].x, pt3[0].x, pt3[0].y, pt3[1].x, pt3[2].x, pt3[2].y, pt3[0].c, pt3[0].c, pt3[1].c, pt3[2].c);
    return 0;
  }
  x12 = GET_LINE_X(pt3[0].x, pt3[0].y, pt3[2].x, pt3[2].y, pt3[1].y);
  n = pt3[2].y - pt3[0].y;
  i1 = pt3[1].y - pt3[0].y;
  c12 = GRADIENTRGB(n, i1, pt3[2].c, pt3[0].c);
  GradientFillTiXin(hDC, pt3[0].x, pt3[0].x, pt3[0].y, pt3[1].x, x12, pt3[1].y, pt3[0].c, pt3[0].c, pt3[1].c, c12);
  GradientFillTiXin(hDC, pt3[1].x, x12, pt3[1].y, pt3[2].x, pt3[2].x, pt3[2].y, pt3[1].c, c12, pt3[2].c, pt3[2].c);
  return 0;
}
int test_FillTiXin(HDDC hDC)
{
  enum {
    MAX_points = 40
  };
  int len = 4;
  POINT pt2[MAX_points] = {
    100, 10, 10, 200, 100, 50, 300, 300
  };
  POINT pt3[MAX_points] = {
    100, 210, 10, 100, 100, 150, 300, 10
  };
  POINT pt4[MAX_points] = {
    100, 20, 300, 300, 300, 10, 20, 100
  };
  RandPoints(pt3, MAX_points, hDC->w, hDC->h);
  len = MAX_points;
  RandPoints(pt4, MAX_points, hDC->w, hDC->h);
  len = MAX_points;
  if (0) {
    utime_start(_start_time);
    int mi01 = MIN(pt4[0].x, pt4[1].x);
    int mx01 = MAX(pt4[0].x, pt4[1].x);
    int mi23 = MIN(pt4[2].x, pt4[3].x);
    int mx23 = MAX(pt4[2].x, pt4[3].x);
    FillTiXin(hDC, mi01, mx01, pt4[1].y, mi23, mx23, pt4[3].y, _RGB(255, 0, 0));
    FillTiXin(hDC, mi01, mi01, pt4[1].y, mi23, mx23, pt4[3].y, _RGB(0, 255, 0));
    FillTiXin(hDC, mi01, mx01, pt4[1].y, mx23, mx23, pt4[3].y, _RGB(0, 0, 255));
    printf("%f\n", utime_elapsed(_start_time));
  }
  if (1) {
    utime_start(_start_time);
    int mi01 = MIN(pt4[0].x, pt4[1].x);
    int mx01 = MAX(pt4[0].x, pt4[1].x);
    int mi23 = MIN(pt4[2].x, pt4[3].x);
    int mx23 = MAX(pt4[2].x, pt4[3].x);
    GradientFillTri(hDC, pt4[0], pt4[1], pt4[2], _RGB(255, 0, 0), _RGB(0, 255, 0), _RGB(0, 0, 255));
    GradientFillTri(hDC, pt4[0], pt4[1], pt4[2], 0x008a8a14, 0x008a8a14, 0x008a8a14);
    //GradientFillTiXin2(hDC, mx01, mi01, pt4[1].y, mi23, mx23, pt4[3].y, _RGB(255, 0, 0), _RGB(0, 255, 0), _RGB(0, 0, 255), _RGB(0, 0, 0));
    printf("%f\n", utime_elapsed(_start_time));
  }
  return 0;
}
#define RGB_BUTTON_BLACK (_GetSysColor(COLOR_WINDOWFRAME))
#define RGB_BUTTON_WHITE (_GetSysColor(COLOR_BTNHIGHLIGHT))
#define RGB_BUTTON_LIGHT (_GetSysColor(COLOR_BTNFACE))
#define RGB_BUTTON_DARK (_GetSysColor(COLOR_BTNSHADOW))
//Colors : Standard colors for QcQp
#define BLACK 0
#define WHITE 1
#define RED 2
#define GREEN 3
#define BLUE 4
#define YELLOW 5
#define MAGENTA 6
#define CYAN 7
#define GRAY 8
#define LIGHTGRAY 9
#define DARKRED 10
#define DARKGREEN 11
#define DARKBLUE 12
#define LIGHTBROWN 13
#define DARKMAGENTA 14
#define DARKCYAN 15
void Draw3DFrame(HDDC pDC, const RECT* rc, COLORREF colBottomRight, COLORREF colTopLeft)
{
  int _x0, _y0;
  RECT rcBox = *rc;
  TMoveTo(pDC, rcBox.right - 1, rcBox.top);
  TLineTo(pDC, rcBox.right - 1, rcBox.bottom - 1, colBottomRight);
  TLineTo(pDC, rcBox.left - 1, rcBox.bottom - 1, colBottomRight);
  TMoveTo(pDC, rcBox.left, rcBox.bottom - 2);
  TLineTo(pDC, rcBox.left, rcBox.top, colTopLeft);
  TLineTo(pDC, rcBox.right - 1, rcBox.top, colTopLeft);
}
#define FrameRect TDrawRect
void Draw3DButtonFrame(HDDC pDC, const RECT* rc, BOOL fFocus)
{
  DWORD GrayBrush = (RGB_BUTTON_LIGHT);
  DWORD BlackBrush = (RGB_BUTTON_BLACK);
  DWORD _clr;
  int _x0, _y0;
  RECT rcButton = *rc;
  _clr = RGB_BUTTON_BLACK;
  // Draw gray outside
  FrameRect(pDC, &rcButton, GrayBrush);
  InflateRect(&rcButton, -1, -1);
  if (fFocus) {
    // Draw inside of border
    FrameRect(pDC, &rcButton, BlackBrush);
    // Draw curved border on outside;
    InflateRect(&rcButton, 1, 1);
  }
  else {
    // Prepare inside border
    FrameRect(pDC, &rcButton, GrayBrush);
  }
  TMoveTo(pDC, rcButton.left + 1, rcButton.top);
  _LineTo(pDC, rcButton.right - 1, rcButton.top);
  TMoveTo(pDC, rcButton.left + 1, rcButton.bottom - 1);
  _LineTo(pDC, rcButton.right - 1, rcButton.bottom - 1);
  TMoveTo(pDC, rcButton.left, rcButton.top + 1);
  _LineTo(pDC, rcButton.left, rcButton.bottom - 1);
  TMoveTo(pDC, rcButton.right - 1, rcButton.top + 1);
  _LineTo(pDC, rcButton.right - 1, rcButton.bottom - 1);
}
static COLORREF DarkenColor(long lScale, COLORREF lColor)
{
  long R = MulDiv(GetRV(lColor), (255 - lScale), 255);
  long G = MulDiv(GetGV(lColor), (255 - lScale), 255);
  long B = MulDiv(GetBV(lColor), (255 - lScale), 255);
  return _RGB(R, G, B);
}
static COLORREF GetRGBColorWhite()
{
  return _RGB(255, 255, 255);
}
static COLORREF GetRGBActiveBorder()
{
  COLORREF clrPress = _GetSysColor(COLOR_INACTIVEBORDER);
  BYTE byRvalue = GetRV(clrPress);
  BYTE byGvalue = GetGV(clrPress);
  BYTE byBvalue = GetBV(clrPress);
  return _RGB(byRvalue, byGvalue, byBvalue);
}
static COLORREF GetRGBColorXP()
{
  COLORREF clr3DFace = _GetSysColor(COLOR_3DFACE);
  return _RGB(((3 * GetRV(clr3DFace) + 240) / 4) + 1,
      ((3 * GetGV(clr3DFace) + 240) / 4) + 1,
      ((3 * GetBV(clr3DFace) + 240) / 4) + 1);
}
static COLORREF GetRGBColorTabs()
{
  BYTE byRvalue = GetRV(GetRGBColorXP());
  BYTE byGvalue = GetGV(GetRGBColorXP());
  BYTE byBvalue = GetBV(GetRGBColorXP());
  int iMaximo = MAX(byRvalue, byGvalue);
  iMaximo = MAX(iMaximo, byBvalue);
  iMaximo = 255 - iMaximo;
  iMaximo = iMaximo - (int)(iMaximo * 0.51);
  return _RGB(byRvalue + iMaximo, byGvalue + iMaximo, byBvalue + iMaximo);
}
static COLORREF GetRGBColorFace()
{
  return _GetSysColor(COLOR_BTNFACE);
}
static COLORREF GetRGBColorShadow()
{
  return _GetSysColor(COLOR_BTNSHADOW);
}
static COLORREF GetRGBSkinMenu()
{
  int iMaximo;
  BYTE byRvalue = GetRV(GetRGBColorFace());
  BYTE byGvalue = GetGV(GetRGBColorFace());
  BYTE byBvalue = GetBV(GetRGBColorFace());
  byRvalue = byRvalue >> 2;
  byGvalue = byGvalue >> 2;
  byBvalue = byBvalue >> 2;
  iMaximo = MAX(byRvalue, byGvalue);
  iMaximo = MAX(iMaximo, byBvalue);
  iMaximo = 255 - iMaximo;
  iMaximo = iMaximo - (int)(iMaximo * 0.018);
  return _RGB(byRvalue + iMaximo, byGvalue + iMaximo, byBvalue + iMaximo);
}
static COLORREF GetRGBColorBTNHigh()
{
  return _GetSysColor(COLOR_BTNHIGHLIGHT);
}
static COLORREF GetRGBColorGrayText()
{
  int iMaximo;
  BYTE byRvalue = 0;
  BYTE byGvalue = 0;
  BYTE byBvalue = 0;
  byRvalue = byRvalue >> 1;
  byGvalue = byGvalue >> 1;
  byBvalue = byBvalue >> 1;
  iMaximo = MAX(byRvalue, byGvalue);
  iMaximo = MAX(iMaximo, byBvalue);
  iMaximo = 255 - iMaximo;
  iMaximo = iMaximo - (int)(iMaximo * 0.60);
  return _RGB(byRvalue + iMaximo, byGvalue + iMaximo, byBvalue + iMaximo);
}
static COLORREF GetRGBPressBXP()
{
  int iMaximo;
  DWORD crH = _GetSysColor(COLOR_HIGHLIGHT);
  BYTE byRvalue = GetRV(crH);
  BYTE byGvalue = GetGV(crH);
  BYTE byBvalue = GetBV(crH);
  byRvalue = byRvalue >> 2;
  byGvalue = byGvalue >> 2;
  byBvalue = byBvalue >> 2;
  iMaximo = MAX(byRvalue, byGvalue);
  iMaximo = MAX(iMaximo, byBvalue);
  iMaximo = 255 - iMaximo;
  iMaximo = iMaximo - (int)(iMaximo * 0.21);
  return _RGB(byRvalue + iMaximo, byGvalue + iMaximo, byBvalue + iMaximo);
}
static COLORREF GetRGBMenu()
{
  int iMaximo;
  DWORD crH = _GetSysColor(COLOR_HIGHLIGHT);
  BYTE byRvalue = GetRV(crH);
  BYTE byGvalue = GetGV(crH);
  BYTE byBvalue = GetBV(crH);
  byRvalue = byRvalue >> 3;
  byGvalue = byGvalue >> 3;
  byBvalue = byBvalue >> 3;
  iMaximo = MAX(byRvalue, byGvalue);
  iMaximo = MAX(iMaximo, byBvalue);
  iMaximo = 255 - iMaximo;
  iMaximo = iMaximo - (int)(iMaximo * 0.47);
  return _RGB(byRvalue + iMaximo, byGvalue + iMaximo, byBvalue + iMaximo);
}
static COLORREF GetRGBTitleMenu()
{
  int iMaximo;
  COLORREF clrPress = _GetSysColor(COLOR_BTNSHADOW);
  BYTE byRvalue = GetRV(_GetSysColor(clrPress));
  BYTE byGvalue = GetGV(_GetSysColor(clrPress));
  BYTE byBvalue = GetBV(_GetSysColor(clrPress));
  byRvalue = byRvalue >> 2;
  byGvalue = byGvalue >> 2;
  byBvalue = byBvalue >> 2;
  iMaximo = MAX(byRvalue, byGvalue);
  iMaximo = MAX(iMaximo, byBvalue);
  iMaximo = 255 - iMaximo;
  iMaximo = iMaximo - (int)(iMaximo * 0.8);
  return _RGB(byRvalue + iMaximo, byGvalue + iMaximo, byBvalue + iMaximo);
}
static COLORREF GetRGBCaptionXP()
{
  BYTE byRvalue = GetRV(_GetSysColor(COLOR_HIGHLIGHT));
  BYTE byGvalue = GetGV(_GetSysColor(COLOR_HIGHLIGHT));
  BYTE byBvalue = GetBV(_GetSysColor(COLOR_HIGHLIGHT));
  return _RGB(byRvalue + 10, byGvalue + 10, byBvalue + 10);
}
static COLORREF GetRGBFondoXP()
{
  int iMaximo;
  BYTE byRvalue = GetRV(_GetSysColor(COLOR_HIGHLIGHT));
  BYTE byGvalue = GetGV(_GetSysColor(COLOR_HIGHLIGHT));
  BYTE byBvalue = GetBV(_GetSysColor(COLOR_HIGHLIGHT));
  byRvalue = byRvalue >> 2;
  byGvalue = byGvalue >> 2;
  byBvalue = byBvalue >> 2;
  iMaximo = MAX(byRvalue, byGvalue);
  iMaximo = MAX(iMaximo, byBvalue);
  iMaximo = 255 - iMaximo;
  iMaximo = iMaximo - (int)(iMaximo * 0.14);
  return _RGB(byRvalue + iMaximo, byGvalue + iMaximo, byBvalue + iMaximo);
}
static COLORREF GetRGBGray()
{
  int iMaximo;
  BYTE byRvalue = GetRV(_GetSysColor(COLOR_BTNSHADOW));
  BYTE byGvalue = GetGV(_GetSysColor(COLOR_BTNSHADOW));
  BYTE byBvalue = GetBV(_GetSysColor(COLOR_BTNSHADOW));
  byRvalue = byRvalue >> 2;
  byGvalue = byGvalue >> 2;
  byBvalue = byBvalue >> 2;
  iMaximo = MAX(byRvalue, byGvalue);
  iMaximo = MAX(iMaximo, byBvalue);
  iMaximo = 255 - iMaximo;
  iMaximo = iMaximo - (int)(iMaximo * 0.30);
  return _RGB(byRvalue + iMaximo, byGvalue + iMaximo, byBvalue + iMaximo);
  //return _RGB(182,179,171);
}
static BOOL IsGrayed()
{
  BYTE byRvalue = GetRV(_GetSysColor(COLOR_3DFACE));
  BYTE byGvalue = GetGV(_GetSysColor(COLOR_3DFACE));
  BYTE byBvalue = GetBV(_GetSysColor(COLOR_3DFACE));
  if ((byRvalue == byGvalue) && (byGvalue == byBvalue) &&
      (byBvalue > 190 && byBvalue < 193) || (byBvalue == 216)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}
static COLORREF GetRGBSemiShadow()
{
  int iMaximo;
  COLORREF clr3DFace = _GetSysColor(COLOR_3DFACE);
  BYTE byRvalue = GetRV(clr3DFace);
  BYTE byGvalue = GetGV(clr3DFace);
  BYTE byBvalue = GetBV(clr3DFace);
  byRvalue = byRvalue >> 2;
  byGvalue = byGvalue >> 2;
  byBvalue = byBvalue >> 2;
  iMaximo = MAX(byRvalue, byGvalue);
  iMaximo = MAX(iMaximo, byBvalue);
  iMaximo = 255 - iMaximo;
  iMaximo = iMaximo - (int)(iMaximo * 0.25);
  return _RGB(byRvalue + iMaximo, byGvalue + iMaximo, byBvalue + iMaximo);
}
int GetDropDownWidth()
{
  HFONT hFont;
  int iSize;
  HDC hDC = GetDC(NULL);
  ASSERT(hDC != NULL);
  if ((hFont = CreateFont(GetSystemMetrics(SM_CYMENUCHECK), 0, 0, 0,
      FW_NORMAL, 0, 0, 0, SYMBOL_CHARSET, 0, 0, 0, 0, _T("Marlett"))) != NULL) {
    hFont = (HFONT) SelectObject(hDC, hFont);
  }
  GetCharWidth(hDC, '6', '6', &iSize);
  if (hFont != NULL) {
    SelectObject(hDC, hFont);
    DeleteObject(hFont);
  }
  ReleaseDC(NULL, hDC);
  ASSERT(iSize != -1);
  return iSize;
}
static int SWCFindChar(TCHAR Buffer[], TCHAR ch)
{
  int i = 0;
  for (i = 0; i < lstrlen(Buffer); i++) {
    if (Buffer[ i ] == ch) {
      break;
    }
  }
  if (i == lstrlen(Buffer)) {
    return -1;
  }
  return i;
}
static void SWCCopyRight(TCHAR BufferSrc[], int pos, TCHAR BufferTarget[])
{
  lstrcpyn(BufferTarget, BufferSrc + pos, lstrlen(BufferSrc) - (pos - 1));
}
static void SWCCopyLeft(TCHAR BufferSrc[], int pos, TCHAR BufferTarget[])
{
  lstrcpyn(BufferTarget, BufferSrc, pos);
}
//function of Bruno Podetti
//#define TRANSPARENTROP 0xb8074a
int DrawShade2(RECT Rect, POINT screen, HDDC pDC, HDDC hDcDsk)
{
  // if(!SystemParametersInfo(SPI_GETDROPSHADOW,0,&bEnabled,0))
  // return;
  // Get the size of the menu...
  long winW = RCWIDTH(&Rect);
  long winH = RCHEIGHT(&Rect);
  long xOrg = screen.x;
  long yOrg = screen.y;
  // Get the desktop hDC...
  int X, Y;
  // Simulate a shadow on right edge...
  for (X = 1; X <= 2 ; X++) {
    for (Y = 0; Y < 2 ; Y++) {
      _SetPixel(pDC, winW - X, Y, _GetPixel(hDcDsk, xOrg + winW - X, yOrg + Y));
    }
    for (Y = 4; Y < 8 ; Y++) {
      COLORREF c = _GetPixel(hDcDsk, xOrg + winW - X, yOrg + Y) ;
      _SetPixel(pDC, winW - X, Y, DarkenColor(3 * X * (Y - 3), c)) ;
    }
    for (Y = 8; Y <= (winH - 5) ; Y++) {
      COLORREF c = _GetPixel(hDcDsk, xOrg + winW - X, yOrg + Y) ;
      _SetPixel(pDC, winW - X, Y, DarkenColor(15 * X, c));
    }
    for (Y = (winH - 4); Y <= (winH - 1) ; Y++) {
      COLORREF c = _GetPixel(hDcDsk, xOrg + winW - X, yOrg + Y) ;
      _SetPixel(pDC, winW - X, Y, DarkenColor(3 * X * -(Y - winH), c)) ;
    }
  }
  // Simulate a shadow on the bottom edge...
  for (Y = 1; Y <= 2 ; Y++) {
    for (X = 0; X <= 2 ; X++) {
      _SetPixel(pDC, X, winH - Y, _GetPixel(hDcDsk, xOrg + X, yOrg + winH - Y)) ;
    }
    for (X = 4; X <= 7 ; X++) {
      COLORREF c = _GetPixel(hDcDsk, xOrg + X, yOrg + winH - Y) ;
      _SetPixel(pDC, X, winH - Y, DarkenColor(3 * (X - 3) * Y, c)) ;
    }
    for (X = 8; X <= (winW - 5) ; X++) {
      COLORREF c = _GetPixel(hDcDsk, xOrg + X, yOrg + winH - Y);
      _SetPixel(pDC, X, winH - Y, DarkenColor(15 * Y, c)) ;
    }
  }
  return 0;
}
int DrawCheck(HDDC pDC, RECT m_rcTemp)
{
  int iMediaAltura = (RCHEIGHT(&m_rcTemp) / 2) - 2;
  int iMedioBox = RCWIDTH(&m_rcTemp) / 2;
  DWORD _clr;
  int _x0, _y0;
  _clr = GetRGBCaptionXP();
  TMoveTo(pDC, m_rcTemp.left + 1, m_rcTemp.top + iMediaAltura + 3);
  _LineTo(pDC, m_rcTemp.left + 3, m_rcTemp.bottom - 2);
  TMoveTo(pDC, m_rcTemp.left + 3, m_rcTemp.bottom - 2);
  _LineTo(pDC, m_rcTemp.right - 1, m_rcTemp.top + 2);
  TMoveTo(pDC, m_rcTemp.left + 1, m_rcTemp.top + iMediaAltura + 2);
  _LineTo(pDC, m_rcTemp.left + 3, m_rcTemp.bottom - 3);
  TMoveTo(pDC, m_rcTemp.left + 3, m_rcTemp.bottom - 3);
  _LineTo(pDC, m_rcTemp.right - 1, m_rcTemp.top + 1);
  TMoveTo(pDC, m_rcTemp.left + 1, m_rcTemp.top + iMediaAltura + 1);
  _LineTo(pDC, m_rcTemp.left + 3, m_rcTemp.bottom - 4);
  TMoveTo(pDC, m_rcTemp.left + 3, m_rcTemp.bottom - 4);
  _LineTo(pDC, m_rcTemp.right - 1, m_rcTemp.top);
  return 0;
}
int DrawGripper(HDDC hDC, RECT rcWin, BOOL bHorz/* = TRUE*/)
{
  int i;
  RECT rcBlack, rcWindow;
  DWORD cb;
  if (bHorz) {
    rcWin.top += 6;
    rcWin.left += 5;
    rcWin.right = rcWin.left + 2;
    rcWin.bottom -= 2;
    for (i = 0; i < RCHEIGHT(&rcWin); i += 4) {
      cb = (_GetSysColor(COLOR_BTNHIGHLIGHT));
      rcWindow = rcWin;
      rcWindow.top = rcWin.top + i;
      rcWindow.bottom = rcWindow.top + 2;
      TFillRect(hDC, &rcWindow, cb);
      rcBlack = rcWindow;
      rcBlack.left -= 1;
      rcBlack.top = (rcWin.top + i) - 1;
      rcBlack.bottom = rcBlack.top + 2;
      rcBlack.right = rcBlack.left + 2;
      cb = GetRGBColorShadow();
      TFillRect(hDC, &rcBlack, cb);
    }
  }
  else {
    rcWin.top += 3;
    rcWin.left += 4;
    rcWin.right -= 2;
    rcWin.bottom = rcWin.top + 2;
    for (i = 0; i < RCWIDTH(&rcWin); i += 4) {
      cb = _GetSysColor(COLOR_BTNHIGHLIGHT);
      rcWindow = rcWin;
      rcWindow.left = rcWindow.left + i;
      rcWindow.right = rcWindow.left + 2;
      TFillRect(hDC, &rcWindow, cb);
      rcBlack = rcWindow;
      rcBlack.top -= 1;
      rcBlack.left -= 1;
      rcBlack.bottom = rcBlack.top + 2;
      rcBlack.right = rcBlack.left + 2;
      cb = GetRGBColorShadow();
      TFillRect(hDC, &rcBlack, cb);
    }
  }
  return 0;
}
#define GUISTYLE_XP 0x08001
#define GUISTYLE_2003 0x08002
#define GUISTYLE_2003MENUBTN 0x08021
#define WIN_NT 0x08003
#define WIN_2000 0x08004
#define WIN_XP 0x08005
#define WIN_95 0x08006
#define WIN_98 0x08007
#define WIN_32 0x08008
#define WIN_ME 0x08009
#define WIN_95OSR2 0x0800A
#define WIN_98_SE 0x0800B
void PrepareVertical(HDDC pDC, UINT RTop, UINT GTop, UINT BTop, UINT RBot, UINT GBot, UINT BBot)
{
  //Here we will create a memory bitmap and draw the colored bitmap on it
  //using my pretty Algorithm for generating colors.
  //if we have already a drawn DC then delete it so we will not have a
  //memory leak
  //create the Memory Bitmap
  ////////////////////////////////////////////////////////////////
  ////Drawing The Gradient in a MemDC
  ////////////////////////////////////////////////////////////////
  //ALGORITHM:
  //We will have the _RGB values of the color at which we will start
  //and the _RGB values of the color at which we will end
  //we will fill 256 rectangles using all colors that lie between
  //these two colors. this is done by moving the R,G, and B values slowly
  //from the Starting color to the Ending Color.
  //For example: if we have RTop=100 and RBot=150 then we will draw 256
  //rectangles colored with the R-values of 100,100+(150-100)/256,
  //100+2*(150-100)/256,100+3*(150-100)/256,...
  //and the same will happen to G and B values.
  //rStep,gStep, and bStep are variables that will be used
  //to hold the values at which R,G,B will be changed respectivily
  //For example: if we have RTop=100 and RBot=150 then
  //rStep=(150-100)/256 so when we start at R=100 and draw 256 rectangles
  //we will end at R=150 when we finish drawing these rectangles
  SIZE m_Size = {pDC->w, pDC->h};
  double rStep, gStep, bStep;
  double rCount, gCount, bCount;
  double RectHeight = m_Size.h / 256.0;
  const int NUM_COLORS = 256;
  int ColorCount;
  //We will start counting from TopColor to BottomColor
  //So we give an initial value of the TopColor
  rCount = RTop;
  gCount = GTop;
  bCount = BTop;
  //Calcualte the step of R,G,B values
  rStep = -((double) RTop - RBot) / NUM_COLORS;
  gStep = -((double) GTop - GBot) / NUM_COLORS;
  bStep = -((double) BTop - BBot) / NUM_COLORS;
  for (ColorCount = 0; ColorCount < NUM_COLORS; ColorCount++) {
    //Draw using current _RGB values and Change _RGB values
    //to represent the next color in the chain
    RECT rc = iRECT(0, (int)(ColorCount * RectHeight), m_Size.w, (int)((ColorCount + 1) * RectHeight));
    TFillRect(pDC, &rc, _RGB(rCount, gCount, bCount));
    rCount += rStep;
    gCount += gStep;
    bCount += bStep;
  }
}
int PrepareHorizontal(HDDC pDC, UINT RLeft, UINT GLeft, UINT BLeft, UINT RRight, UINT GRight, UINT BRight)
{
  ////Drawing The Gradient in a MemDC
  SIZE m_Size = {pDC->w, pDC->h};
  double rStep, gStep, bStep;
  double rCount, gCount, bCount;
  double RectWidth = m_Size.w / 256.0;
  const int NUM_COLORS = 256;
  int ColorCount;
  rCount = RRight;
  gCount = GRight;
  bCount = BRight;
  rStep = -((double) RRight - RLeft) / NUM_COLORS;
  gStep = -((double) GRight - GLeft) / NUM_COLORS;
  bStep = -((double) BRight - BLeft) / NUM_COLORS;
  for (ColorCount = 0; ColorCount < NUM_COLORS; ColorCount++) {
    RECT rc = iRECT((int)(ColorCount * RectWidth), 0, (int)((ColorCount + 1) * RectWidth), m_Size.h);
    TFillRect(pDC, &rc, _RGB(rCount, gCount, bCount));
    rCount += rStep;
    gCount += gStep;
    bCount += bStep;
  }
  return 0;
}
void PrepareVertical2(HDDC pDC, DWORD m_StyleDisplay/* = GUISTYLE_XP*/, COLORREF m_clr/* = GetRGBColorTabs()*/, int nRate/* = 40 */)
{
  BYTE byRvalue = GetRV(m_clr); //+(nRate==40 ?10:(15));
  BYTE byGvalue = GetGV(m_clr); //+(nRate==40 ?10:(15));
  BYTE byBvalue = GetBV(m_clr); //+(nRate==40 ?10:(15));
  /* if (m_StyleDisplay!= GUISTYLE_XP )
  {
  COLORREF clrNear=(GetRGBCaptionXP());
  if (clrNear==13595707)
  PrepareVertical(pDC,222,234,253,141,178,223);
  else if(clrNear==8039069)
  PrepareVertical(pDC,243,251,221,179,194,138);
  else if(clrNear==13221564)
  PrepareVertical(pDC,248,248,250,165,160,184);
  }
  else*/
  PrepareVertical(pDC, 255, 255, 255, byRvalue - 10, byGvalue - 10, byBvalue - 10);
}
void PrepareCaption(HDDC pDC, DWORD m_StyleDisplay/* = GUISTYLE_XP*/)
{
  COLORREF clrNear = (GetRGBCaptionXP());
  if (clrNear == 13595707) {
    PrepareVertical(pDC, 222, 234, 253, 141, 178, 223);
  }
  else if (clrNear == 8039069) {
    PrepareVertical(pDC, 243, 251, 221, 179, 194, 138);
  }
  else if (clrNear == 13221564) {
    PrepareVertical(pDC, 248, 248, 250, 165, 160, 184);
  }
  else {
    PrepareVertical2(pDC, GUISTYLE_XP, GetRGBColorTabs(), 40);
  }
}
/**/
void PrepareCaptionVert(HDDC pDC, DWORD m_StyleDisplay/* = GUISTYLE_XP*/)
{
  BYTE byRvalue ;
  BYTE byGvalue ;
  BYTE byBvalue ;
  if (m_StyleDisplay != GUISTYLE_XP) {
    PrepareHorizontal(pDC, 249, 200, 102, 248, 248, 205);
    return ;
  }
  else if (m_StyleDisplay == GUISTYLE_2003) {
    byRvalue = GetRV(GetRGBPressBXP());
    byGvalue = GetGV(GetRGBPressBXP());
    byBvalue = GetBV(GetRGBPressBXP());
  }
  else {
    byRvalue = GetRV(GetRGBColorShadow());
    byGvalue = GetGV(GetRGBColorShadow());
    byBvalue = GetBV(GetRGBColorShadow());
  }
  PrepareHorizontal(pDC, byRvalue, byGvalue, byBvalue, byRvalue - 40, byGvalue - 40, byBvalue - 40);
}
void PrepareTabs(HDDC pDC, COLORREF m_clrL, COLORREF m_clrH)
{
  BYTE byRvalueL = GetRV(m_clrL);
  BYTE byGvalueL = GetGV(m_clrL);
  BYTE byBvalueL = GetBV(m_clrL);
  BYTE byRvalueH = GetRV(m_clrH);
  BYTE byGvalueH = GetGV(m_clrH);
  BYTE byBvalueH = GetBV(m_clrH);
  PrepareVertical(pDC, byRvalueL, byGvalueL, byBvalueL, byRvalueH, byGvalueH, byBvalueH);
}
void PrepareReverseVert(HDDC pDC, DWORD m_StyleDisplay/* = GUISTYLE_XP*/, COLORREF m_clr/* = GetRGBColorTabs()*/)
{
  BYTE byRvalue = GetRV(GetRGBColorFace());
  BYTE byGvalue = GetGV(GetRGBColorFace());
  BYTE byBvalue = GetBV(GetRGBColorFace());
  /* if (m_StyleDisplay!= GUISTYLE_XP)
  {
  COLORREF clrNear=(GetRGBCaptionXP());
  if (clrNear==13595707)
  PrepareVertical(pDC,141,178,223,222,234,253);
  else if(clrNear==8039069)
  PrepareVertical(pDC,179,194,138,243,251,221);
  else if(clrNear==13221564)
  PrepareVertical(pDC,165,160,184,248,248,250);
  }
  else*/
  PrepareVertical(pDC, byRvalue - 40, byGvalue - 40, byBvalue - 40, 255, 255, 255);
  //PrepareVertical(pDC,byRvalue-2,byGvalue-2,byBvalue-2,byRvalue,byGvalue,byBvalue);
}
void PrepareReverseVertTab(HDDC pDC, DWORD m_StyleDisplay/* = GUISTYLE_XP*/, COLORREF m_clr/* = GetRGBColorTabs()*/)
{
  BYTE byRvalue = GetRV(m_clr);
  BYTE byGvalue = GetGV(m_clr);
  BYTE byBvalue = GetBV(m_clr);
  if (m_StyleDisplay != GUISTYLE_XP) {
    COLORREF clrNear = (GetRGBCaptionXP());
    if (clrNear == 13595707) {
      PrepareVertical(pDC, 216, 228, 243, 255, 255, 255);
    }
    else if (clrNear == 8039069) {
      PrepareVertical(pDC, 233, 237, 220, 255, 255, 255);
    }
    else if (clrNear == 13221564) {
      PrepareVertical(pDC, 234, 233, 239, 255, 255, 255);
    }
  }
  else {
    PrepareVertical(pDC, byRvalue - 10, byGvalue - 10, byBvalue - 10, byRvalue, byGvalue, byBvalue);
  }
}
void PrepareReverseColorTab(HDDC pDC, DWORD m_StyleDisplay/* = GUISTYLE_XP*/, COLORREF m_clrL/* = GetRGBColorTabs()*/, COLORREF m_clrH/* = GetRGBColorTabs()*/)
{
  BYTE byRvalueL = GetRV(m_clrL);
  BYTE byGvalueL = GetGV(m_clrL);
  BYTE byBvalueL = GetBV(m_clrL);
  BYTE byRvalueH = GetRV(m_clrH);
  BYTE byGvalueH = GetGV(m_clrH);
  BYTE byBvalueH = GetBV(m_clrH);
  PrepareVertical(pDC, byRvalueL, byGvalueL, byBvalueL, byRvalueH, byGvalueH, byBvalueH);
}
void PrepareReverseHor(HDDC pDC, COLORREF m_clr/* = GetRGBColorFace()*/, COLORREF m_clr1/* = GetRGBColorShadow()*/)
{
  BYTE byRvalue = GetRV(m_clr);
  BYTE byGvalue = GetGV(m_clr);
  BYTE byBvalue = GetBV(m_clr);
  BYTE byRvalue1 = GetRV(m_clr1);
  BYTE byGvalue1 = GetGV(m_clr1);
  BYTE byBvalue1 = GetBV(m_clr1);
  /*if (m_StyleDisplay!= GUISTYLE_XP)
  {
  COLORREF clrNear=(GetRGBCaptionXP());
  if (clrNear==13595707)
  PrepareHorizontal(pDC,222,234,253,141,178,223);
  else if(clrNear==8039069)
  PrepareHorizontal(pDC,243,251,221,179,194,138);
  else if(clrNear==13221564)
  PrepareHorizontal(pDC,248,248,250,165,160,184);
  }
  else*/
  PrepareHorizontal(pDC, byRvalue1, byGvalue1, byBvalue1, byRvalue, byGvalue, byBvalue);
  //PrepareHorizontal(pDC,byRvalue,byGvalue,byBvalue,byRvalue-40,byGvalue-40,byBvalue-40);
}
void PrepareHorizontal2(HDDC pDC, COLORREF m_clrBase/* = GetRGBColorTabs()*/, COLORREF m_clr/* = GetRGBColorFace()*/)
{
  BYTE byRvalue = GetRV(m_clrBase) + 10;
  BYTE byGvalue = GetGV(m_clrBase) + 10;
  BYTE byBvalue = GetBV(m_clrBase) + 10;
  BYTE byRvalue1 = GetRV(m_clr);
  BYTE byGvalue1 = GetGV(m_clr);
  BYTE byBvalue1 = GetBV(m_clr);
  PrepareHorizontal(pDC, byRvalue1 - 10, byGvalue1 - 10, byBvalue1 - 10, 255, 255, 255);
}
int DrawToolBar(HDDC hDC0, RECT rect)
{
  //aqui debe utilizarse la brocha que define CDrawLayer, si no hacemos la siguiente
  //linea usted vera un horrible color negro, a cambio del dibujo.
  DWORD cb = _RGB(255, 255, 255);
  DWORD cb2 = GetRGBColorTabs();
  TDrawDC hDC[1];
  DPOINT pt[100];
  int n;
  //cb2 = _RGB(GetRV(cb2)-10, GetRV(cb2)-10, GetRV(cb2)-10);
  HDDCSET_RECT(hDC, hDC0, &rect);
  n = round_rect_vertex1(rect.left, rect.top, rect.right, rect.bottom, 2, pt);
  FillPolyGradient(hDC, pt, &n, 1, rect.left, rect.top, rect.right, rect.top, rect.left, rect.bottom, cb, cb, cb2);
  //FillPolyCell1(hDC0, pt, n, cb2);
  //PrepareVertical2( hDC, GUISTYLE_XP, GetRGBPressBXP(), 40 );
  DrawGripper(hDC, rect, TRUE);
  return 0;
}
#endif

