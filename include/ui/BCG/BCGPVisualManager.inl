
// BCGPVisualManager.cpp: implementation of the CBCGPPVisualManager class.
//
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBCGPVisualManager::CBCGPVisualManager()
{
  m_bAutoDestroy = FALSE;

  m_bLook2000 = FALSE;
  m_bMenuFlatLook = FALSE;
  m_nMenuShadowDepth = 6;
  m_bShadowHighlightedImage = FALSE;
  m_bEmbossDisabledImage = TRUE;
  m_bFadeInactiveImage = FALSE;
  m_bEnableToolbarButtonFill = TRUE;

  m_nVertMargin = 12;
  m_nHorzMargin = 12;
  m_nGroupVertOffset = 15;
  m_nGroupCaptionHeight = 25;
  m_nGroupCaptionHorzOffset = 13;
  m_nGroupCaptionVertOffset = 7;
  m_nTasksHorzOffset = 12;
  m_nTasksIconHorzOffset = 5;
  m_nTasksIconVertOffset = 4;
  m_bActiveCaptions = TRUE;

  m_bOfficeXPStyleMenus = FALSE;
  m_nMenuBorderSize = 2;

  m_b3DTabWideBorder = TRUE;
  m_bAlwaysFillTab = FALSE;
  m_bFrameMenuCheckedItems = FALSE;
  m_clrMenuShadowBase = 0;  // Used in derived classes

  globalData.UpdateSysColors();


  m_bPlannerBackItemToday         = FALSE;
  m_bPlannerBackItemSelected      = FALSE;
  m_bPlannerCaptionBackItemHeader = FALSE;

  m_ptRibbonMainImageOffset = CPoint(0, 0);

  OnUpdateSystemColors();
}
//****
CBCGPVisualManager::~CBCGPVisualManager()
{
}
//****
void CBCGPVisualManager::OnUpdateSystemColors()
{
  m_clrPlannerWork    = _RGB(255, 255, 0);
  m_clrPalennerLine   = _RGB(128, 128, 128);
  m_clrReportGroupText  = globalData.clrHilite;
}
//****
void CBCGPVisualManager::OnDrawBarGripper(CDC* pDC, CRect rectGripper, BOOL bHorz,
    CBCGPBaseControlBar* pBar)
{
  ASSERT_VALID(pDC);

  const COLORREF clrBtnFace = globalData.clrBtnFace;
  const COLORREF clrHilite = pBar != NULL && pBar->IsDialogControl() ? globalData.clrBtnHilite : globalData.clrBarHilite;
  const COLORREF clrShadow = pBar != NULL && pBar->IsDialogControl() ? globalData.clrBtnShadow : globalData.clrBarShadow;

  const BOOL bSingleGripper = m_bLook2000;

  const int iGripperSize = 3;
  const int iGripperOffset = bSingleGripper ? 0 : 1;
  const int iLinesNum = bSingleGripper ? 1 : 2;

  if (bHorz) {
    //-----------------
    // Gripper at left:
    //-----------------
    rectGripper.DeflateRect(0, bSingleGripper ? 3 : 2);

    // ET: Center the grippers
    rectGripper.l = iGripperOffset + rectGripper.CenterPoint().x -
        (iLinesNum * iGripperSize + (iLinesNum - 1) * iGripperOffset) / 2;

    rectGripper.r = rectGripper.l + iGripperSize;


    for (int i = 0; i < iLinesNum; i ++) {
      pDC->Draw3dRect(rectGripper, clrHilite, clrShadow, clrBtnFace);

      // ET: not used for NewFlat look
      if (! bSingleGripper) {
        //-----------------------------------
        // To look same as MS Office Gripper!
        //-----------------------------------
        pDC->SetPixel(CPoint(rectGripper.l, rectGripper.b - 1), clrHilite);
      }

      rectGripper.OffsetRect(iGripperSize + 1, 0);
    }
  }
  else {
    //----------------
    // Gripper at top:
    //----------------
    rectGripper.t += iGripperOffset;
    rectGripper.DeflateRect(bSingleGripper ? 3 : 2, 0);

    // ET: Center the grippers
    rectGripper.t = iGripperOffset + rectGripper.CenterPoint().y -
        (iLinesNum * iGripperSize + (iLinesNum - 1)) / 2;

    rectGripper.b = rectGripper.t + iGripperSize;

    for (int i = 0; i < iLinesNum; i ++) {
      pDC->Draw3dRect(rectGripper, clrHilite, clrShadow, globalData.clrBtnFace);

      // ET: not used for NewFlat look
      if (! bSingleGripper) {
        //-----------------------------------
        // To look same as MS Office Gripper!
        //-----------------------------------
        pDC->SetPixel(CPoint(rectGripper.r - 1, rectGripper.t),
            clrHilite);
      }

      rectGripper.OffsetRect(0, iGripperSize + 1);
    }
  }
}


void CBCGPOutlookBarPane_OnEraseWorkArea(CDC* pDC, CRect rectWorkArea, CBCGPBaseControlBar* pBar)
{
  ASSERT_VALID(pDC);
  CBitmap* m_bmpBack = pBar->GetBackBitmap();

  if (m_bmpBack->GetCount() == 0) {
    COLOR m_clrBackColor = pBar->GetBackColor();
    FillRect(pDC, rectWorkArea, m_clrBackColor);
  }
  else {
    ASSERT(m_bmpBack->GetCount() == 1);
    CSize sizeBack = m_bmpBack->GetImageSize();

    for (int x = rectWorkArea.l; x < rectWorkArea.r; x += sizeBack.w) {
      for (int y = rectWorkArea.t; y < rectWorkArea.b; y += sizeBack.h) {
        DrawImage(pDC, m_bmpBack, x, y, 0);
      }
    }

  }
}

void CBCGPVisualManager::FillRebarPane(CDC* pDC, CBCGPBaseControlBar* pBar, CRect rectClient)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pBar);
  FillRect(pDC, &rectClient, globalData.brBarFace);
  return;
}

//****
void CBCGPVisualManager::OnFillBarBackground(CDC* pDC, CBCGPBaseControlBar* pBar,
    CRect rectClient, CRect rectClip,
    BOOL /*bNCArea*/)
{
  int cls = pBar->GetClass();
  ASSERT_VALID(pDC);
  ASSERT_VALID(pBar);

  if (pBar->IsOnGlass()) {
    pDC->FillSolidRect(rectClient, _RGB(0, 0, 0));
    return;
  }

  switch (cls) {
  case CLASS_REBAR :
    FillRebarPane(pDC, pBar, rectClient);
    return;

  case CLASS_OUTLOOKBAR :
    CBCGPOutlookBarPane_OnEraseWorkArea(pDC, rectClient, pBar);
    return;

  case CLASS_CAPTIONBAR :
    if (pBar->IsMessageBarMode()) {
      FillRect(pDC, rectClip, globalData.brBarFace);
    }
    else {
      COLOR m_clrBarBackground = pBar->GetBackColor();
      pDC->FillSolidRect(rectClip, m_clrBarBackground == -1 ?
          globalData.clrBarShadow : m_clrBarBackground);
    }

    return;

  case CLASS_POPUPMENUBAR:
    if (pBar->IsDropDownListMode()) {
      FillRect(pDC, rectClip, globalData.brWindow);
      return;
    }

  default:
    // By default, control bar background is filled by
    // the system 3d background color
    FillRect(pDC, rectClip.IsRectEmpty() ? rectClient : rectClip,
        pBar->IsDialogControl() ?
        globalData.brBtnFace : globalData.brBarFace);
    break;
  }

  return ;
}

//****
void CBCGPVisualManager::OnDrawBarBorder(CDC* pDC, CBCGPBaseControlBar* pBar, CRect& rect)
{
  ASSERT_VALID(pBar);
  ASSERT_VALID(pDC);

  if (pBar->IsFloating()) {
    return;
  }

  DWORD dwStyle = pBar->GetBarStyle();

  if (!(dwStyle & CBRS_BORDER_ANY)) {
    return;
  }

  COLORREF clrBckOld = pDC->GetBkColor(); // FillSolidRect changes it

  const COLORREF clrHilite = pBar->IsOnGlass() ? _RGB(0, 0, 0) : pBar->IsDialogControl() ?
      globalData.clrBtnHilite : globalData.clrBarHilite;
  const COLORREF clrShadow = pBar->IsOnGlass() ? _RGB(0, 0, 0) : pBar->IsDialogControl() ?
      globalData.clrBtnShadow : globalData.clrBarShadow;

  COLORREF clr = pBar->IsOnGlass() ? _RGB(0, 0, 0) : clrHilite;

  if (dwStyle & CBRS_BORDER_LEFT) {
    pDC->FillSolidRect(0, 0, 1, rect.Height() - 1, clr);
  }

  if (dwStyle & CBRS_BORDER_TOP) {
    pDC->FillSolidRect(0, 0, rect.Width() - 1 , 1, clr);
  }

  if (dwStyle & CBRS_BORDER_RIGHT)
    pDC->FillSolidRect(rect.r, 0/*RGL~:1*/, -1,
        rect.Height()/*RGL-: - 1*/, clrShadow);

  if (dwStyle & CBRS_BORDER_BOTTOM) {
    pDC->FillSolidRect(0, rect.b, rect.Width() - 1, -1, clrShadow);
  }

  // if undockable toolbar at top of frame, apply special formatting to mesh
  // properly with frame menu
  if (!pBar->CanFloat()) {
    pDC->FillSolidRect(0, 0, rect.Width(), 1, clrShadow);
    pDC->FillSolidRect(0, 1, rect.Width(), 1, clrHilite);
  }

  if (dwStyle & CBRS_BORDER_LEFT) {
    ++rect.l;
  }

  if (dwStyle & CBRS_BORDER_TOP) {
    ++rect.t;
  }

  if (dwStyle & CBRS_BORDER_RIGHT) {
    --rect.r;
  }

  if (dwStyle & CBRS_BORDER_BOTTOM) {
    --rect.b;
  }

  // Restore Bk color:
  pDC->SetBkColor(clrBckOld);
}
//****
void CBCGPVisualManager::OnDrawMenuBorder(CDC* pDC, CBCGPPopupMenu* /*pMenu*/, CRect rect)
{
  ASSERT_VALID(pDC);

  pDC->Draw3dRect(rect, globalData.clrBarLight, globalData.clrBarDkShadow, globalData.clrBtnFace);
  rect.DeflateRect(1, 1);
  pDC->Draw3dRect(rect, globalData.clrBarHilite, globalData.clrBarShadow, globalData.clrBtnFace);
}
//****
void CBCGPVisualManager::OnDrawMenuShadow(CDC* pPaintDC, const CRect& rectClient, const CRect& /*rectExclude*/,
    int nDepth,  int iMinBrightness,  int iMaxBrightness,
    CBitmap* pBmpSaveBottom,  CBitmap* pBmpSaveRight,
    BOOL bRTL)
{
  ASSERT_VALID(pPaintDC);
  ASSERT_VALID(pBmpSaveBottom);
  ASSERT_VALID(pBmpSaveRight);

  //------------------------------------------------------
  // Simple draw the shadow, ignore rectExclude parameter:
  //------------------------------------------------------
  DrawShadow(pPaintDC->im, &rectClient, rectClient, nDepth, iMinBrightness, iMaxBrightness, pBmpSaveBottom, pBmpSaveRight, 0, !bRTL);
}
//****
void CBCGPVisualManager::OnFillButtonInterior(CDC* pDC,
    CBCGPToolbarButton* pButton, CRect rect,
    BCGBUTTON_STATE state)
{
  int cls = pButton->GetClass();
  ASSERT_VALID(pDC);
  ASSERT_VALID(pButton);

  if (state == ButtonsIsHighlighted) {
    HighlightRect(pDC->im, &rect, rect);
  }

  return;

}
//****
COLORREF CBCGPVisualManager::GetToolbarHighlightColor()
{
  return globalData.clrBarFace;
}
//****
COLORREF CBCGPVisualManager::GetToolbarDisabledTextColor()
{
  return globalData.clrGrayedText;
}
//****
void CBCGPVisualManager::OnHighlightMenuItem(CDC* pDC, CBCGPToolbarMenuButton* /*pButton*/,
    CRect rect, COLORREF& /*clrText*/)
{
  ASSERT_VALID(pDC);
  FillRect(pDC, rect, globalData.brHilite);
}
//****
COLORREF CBCGPVisualManager::GetHighlightedMenuItemTextColor(CBCGPToolbarMenuButton* pButton)
{
  int m_nStyle = pButton->GetStyle();
  ASSERT_VALID(pButton);

  if (m_nStyle & TBBS_DISABLED) {
    return globalData.clrGrayedText;
  }

  return globalData.clrTextHilite;
}
//****
void CBCGPVisualManager::OnHighlightRarelyUsedMenuItems(CDC* pDC, CRect rectRarelyUsed)
{
  ASSERT_VALID(pDC);

  HighlightRect(pDC->im, 0, rectRarelyUsed);

  pDC->Draw3dRect(rectRarelyUsed, globalData.clrBarShadow, globalData.clrBarHilite, globalData.clrBtnFace);
}
//****
void CBCGPVisualManager::OnDrawMenuCheck(CDC* pDC, CBCGPToolbarMenuButton* pButton,
    CRect rectCheck, BOOL /*bHighlight*/, BOOL bIsRadio)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pButton);

  int iImage = bIsRadio ? IdRadio : IdCheck;
  int m_nStyle = pButton->GetStyle();
  imdraw_menuimg(pDC->im, 0, rectCheck, iImage, (m_nStyle & TBBS_DISABLED) ? ImageGray : ImageBlack);
}
//****
void CBCGPVisualManager::OnDrawMenuItemButton(CDC* pDC, CBCGPToolbarMenuButton* /*pButton*/,
    CRect rectButton, BOOL bHighlight, BOOL /*bDisabled*/)
{
  ASSERT_VALID(pDC);

  CRect rect = rectButton;
  rect.r = rect.l + 1;
  rect.l--;
  rect.DeflateRect(0, bHighlight ? 1 : 4);

  pDC->Draw3dRect(rect, globalData.clrBarShadow, globalData.clrBarHilite, globalData.clrBtnFace);
}
//****
void CBCGPVisualManager::OnDrawButtonBorder(CDC* pDC,
    CBCGPToolbarButton* pButton, CRect rect, BCGBUTTON_STATE state)
{
  int cls = pButton->GetClass();
  ASSERT_VALID(pDC);
  ASSERT_VALID(pButton);

  BOOL bIsOutlookButton = cls == CLASS_OutlookButton;
  COLORREF clrDark = bIsOutlookButton ?
      globalData.clrBarDkShadow : globalData.clrBarShadow;

  if (m_bIsDrawOnGlass) {
    if (state == ButtonsIsPressed || state == ButtonsIsHighlighted) {
      imdraw_DrawRectR(pDC->im, &rect, rect, 0, clrDark);
    }

    return;
  }

  switch (state) {
  case ButtonsIsPressed:
    pDC->Draw3dRect(&rect, clrDark, globalData.clrBarHilite, globalData.clrBtnFace);
    return;

  case ButtonsIsHighlighted:
    pDC->Draw3dRect(&rect, globalData.clrBarHilite, clrDark, globalData.clrBtnFace);
    return;
  }
}
//****
void CBCGPVisualManager::OnDrawButtonSeparator(CDC* pDC,
    CBCGPToolbarButton* pButton, CRect rect, BCGBUTTON_STATE state,
    BOOL /*bHorz*/)
{
  ASSERT_VALID(pButton);

  if (!m_bMenuFlatLook || !pButton->IsDroppedDown()) {
    OnDrawButtonBorder(pDC, pButton, rect, state);
  }
}
//****
void CBCGPVisualManager::OnDrawSeparator(CDC* pDC, CBCGPBaseControlBar* pBar,
    CRect rect, BOOL bHorz)
{
  ASSERT_VALID(pBar);
  ASSERT_VALID(pDC);

  CRect rectSeparator = rect;

  if (bHorz) {
    rectSeparator.l += rectSeparator.Width() / 2 - 1;
    rectSeparator.r = rectSeparator.l + 2;
  }
  else {
    rectSeparator.t += rectSeparator.Height() / 2 - 1;
    rectSeparator.b = rectSeparator.t + 2;
  }

  const COLORREF clrHilite = pBar->IsDialogControl() ?
      globalData.clrBtnHilite : globalData.clrBarHilite;
  const COLORREF clrShadow = pBar->IsDialogControl() ?
      globalData.clrBtnShadow : globalData.clrBarShadow;

  if (m_bIsDrawOnGlass) {
    int x1, x2;
    int y1, y2;

    if (bHorz) {
      x1 = x2 = (rect.l + rect.r) / 2;
      y1 = rect.t;
      y2 = rect.b - 1;
    }
    else {
      y1 = y2 = (rect.t + rect.b) / 2;
      x1 = rect.l;
      x2 = rect.r;
    }

    imdraw_DrawLine(pDC->im, 0, x1, y1, x2, y2, clrShadow);
  }
  else {
    pDC->Draw3dRect(rectSeparator, clrShadow, clrHilite, globalData.clrBtnFace);
  }
}
//****
COLORREF CBCGPVisualManager::OnDrawMenuLabel(CDC* pDC, CRect rect)
{
  ASSERT_VALID(pDC);

  FillRect(pDC, rect, globalData.brBtnFace);

  CRect rectSeparator = rect;
  rectSeparator.t = rectSeparator.b - 2;

  pDC->Draw3dRect(rectSeparator, globalData.clrBtnShadow, globalData.clrBtnHilite, globalData.clrBtnFace);

  return globalData.clrBtnText;
}
//****
COLORREF CBCGPVisualManager::OnDrawControlBarCaption(CDC* pDC, CBCGPDockingControlBar* /*pBar*/,
    BOOL bActive, CRect rectCaption, CRect /*rectButtons*/)
{
  ASSERT_VALID(pDC);

  COLOR clr = (bActive ? globalData.clrActiveCaption : globalData.clrInactiveCaption);
  FillRect(pDC, rectCaption, clr);

  // get the text color
  return bActive ? globalData.clrCaptionText : globalData.clrInactiveCaptionText;
}
//****
void CBCGPVisualManager::OnDrawCaptionButton(
    CDC* pDC, CBCGPCaptionButton* pButton, BOOL bActive,
    BOOL bHorz, BOOL bMaximized, BOOL bDisabled,
    int nImageID /*= -1*/)
{
  ASSERT_VALID(pDC);
  CRect rc = pButton->GetRect();

  int id = -1;

  if (nImageID != -1) {
    id = nImageID;
  }
  else {
    id = pButton->GetIconID(bHorz, bMaximized);
  }

  CRect rectImage = rc;

  if (pButton->IsPushed() && (pButton->IsFocused() || pButton->IsDroppedDown())) {
    rectImage.OffsetRect(1, 1);
  }

  IMAGE_STATE imageState;
  COLOR m_clrForeground = pButton->GetForegroundColor();

  if (bDisabled) {
    imageState = ImageGray;
  }
  else if (m_clrForeground == 0) {
    imageState = bActive ? ImageWhite : ImageBlack;
  }
  else {
    if (GetRV(m_clrForeground) > 192 &&
        GetGV(m_clrForeground) > 192 &&
        GetBV(m_clrForeground) > 192) {
      imageState = ImageWhite;
    }
    else {
      imageState = ImageBlack;
    }
  }

  CBCGPMenuImages_Draw(pDC, id, rectImage, imageState);

  if (!bDisabled) {
    if (pButton->IsPushed() && (pButton->IsFocused() || pButton->IsDroppedDown())) {
      pDC->Draw3dRect(rc, globalData.clrBarDkShadow, globalData.clrBarLight);
      rc.DeflateRect(1, 1);
      pDC->Draw3dRect(rc, globalData.clrBarDkShadow, globalData.clrBarHilite);
    }
    else if (!m_bLook2000) {
      pDC->Draw3dRect(rc, globalData.clrBarLight, globalData.clrBarDkShadow);
      rc.DeflateRect(1, 1);
      pDC->Draw3dRect(rc, globalData.clrBarHilite, globalData.clrBarShadow);
    }
    else if (pButton->IsFocused() || pButton->IsPushed() || pButton->IsDroppedDown()) {
      pDC->Draw3dRect(rc, globalData.clrBarHilite, globalData.clrBarShadow);
    }
  }
}
//****
void CBCGPVisualManager::OnEraseTabsArea(CDC* pDC, CRect rect,
    const CBCGPBaseTabWnd* /*pTabWnd*/)
{
  ASSERT_VALID(pDC);
  FillRect(pDC, rect, globalData.brBarFace);
}
//****
void CBCGPVisualManager::OnDrawTab(CDC* pDC, CRect rectTab,
    int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pTabWnd);
  ASSERT_VALID(pDC);

  COLORREF clrTab = pTabWnd->GetTabBkColor(iTab);

  CRect rectClip;
  pDC->GetClipBox(rectClip);

  if (pTabWnd->IsFlatTab()) {
    //----------------
    // Draw tab edges:
    //----------------
#define FLAT_POINTS_NUM 4
    IPOINT pts [FLAT_POINTS_NUM];

    const int nHalfHeight = pTabWnd->GetTabsHeight() / 2;

    if (pTabWnd->GetLocation() == CBCGPBaseTabWnd::LOCATION_BOTTOM) {
      rectTab.b --;

      pts [0].x = rectTab.l;
      pts [0].y = rectTab.t;

      pts [1].x = rectTab.l + nHalfHeight;
      pts [1].y = rectTab.b;

      pts [2].x = rectTab.r - nHalfHeight;
      pts [2].y = rectTab.b;

      pts [3].x = rectTab.r;
      pts [3].y = rectTab.t;
    }
    else {
      rectTab.t ++;

      pts [0].x = rectTab.l + nHalfHeight;
      pts [0].y = rectTab.t;

      pts [1].x = rectTab.l;
      pts [1].y = rectTab.b;

      pts [2].x = rectTab.r;
      pts [2].y = rectTab.b;

      pts [3].x = rectTab.r - nHalfHeight;
      pts [3].y = rectTab.t;

      rectTab.l += 2;
    }

    pDC->Polygon(pts, FLAT_POINTS_NUM, clrTab);
  }
  else if (pTabWnd->IsLeftRightRounded()) {
    CList<IPOINT> pts;

    POSITION posLeft = pts.AddHead(CPoint(rectTab.l, rectTab.t));
    posLeft = pts.InsertAfter(posLeft, CPoint(rectTab.l, rectTab.t + 2));

    POSITION posRight = pts.AddTail(CPoint(rectTab.r, rectTab.t));
    posRight = pts.InsertBefore(posRight, CPoint(rectTab.r, rectTab.t + 2));

    int xLeft = rectTab.l + 1;
    int xRight = rectTab.r - 1;

    int y = 0;

    for (y = rectTab.t + 2; y < rectTab.b - 4; y += 2) {
      posLeft = pts.InsertAfter(posLeft, CPoint(xLeft, y));
      posLeft = pts.InsertAfter(posLeft, CPoint(xLeft, y + 2));

      posRight = pts.InsertBefore(posRight, CPoint(xRight, y));
      posRight = pts.InsertBefore(posRight, CPoint(xRight, y + 2));

      xLeft++;
      xRight--;
    }

    if (pTabWnd->GetLocation() == CBCGPBaseTabWnd::LOCATION_TOP) {
      xLeft--;
      xRight++;
    }

    const int nTabLeft = xLeft - 1;
    const int nTabRight = xRight + 1;

    for (; y < rectTab.b - 1; y++) {
      posLeft = pts.InsertAfter(posLeft, CPoint(xLeft, y));
      posLeft = pts.InsertAfter(posLeft, CPoint(xLeft + 1, y + 1));

      posRight = pts.InsertBefore(posRight, CPoint(xRight, y));
      posRight = pts.InsertBefore(posRight, CPoint(xRight - 1, y + 1));

      if (y == rectTab.b - 2) {
        posLeft = pts.InsertAfter(posLeft, CPoint(xLeft + 1, y + 1));
        posLeft = pts.InsertAfter(posLeft, CPoint(xLeft + 3, y + 1));

        posRight = pts.InsertBefore(posRight, CPoint(xRight, y + 1));
        posRight = pts.InsertBefore(posRight, CPoint(xRight - 2, y + 1));
      }

      xLeft++;
      xRight--;
    }

    posLeft = pts.InsertAfter(posLeft, CPoint(xLeft + 2, rectTab.b));
    posRight = pts.InsertBefore(posRight, CPoint(xRight - 2, rectTab.b));

    IPOINT* points = new IPOINT [pts.GetCount()];

    int i = 0;

    for (POSITION pos = pts.GetHeadPosition(); pos != NULL; i++) {
      points [i] = pts.GetNext(pos);

      if (pTabWnd->GetLocation() == CBCGPBaseTabWnd::LOCATION_TOP) {
        points [i].y = rectTab.b - (points [i].y - rectTab.t);
      }
    }

    COLOR clr = (clrTab == 0 ? globalData.clrBtnFace : clrTab);
    OnFillTab(pDC, rectTab, clr, iTab, bIsActive, pTabWnd, points, (int) pts.GetCount());

    COLOR pen = (globalData.clrBarShadow);
    COLOR pOLdPen = pDC->SelectObject(pen);

    for (i = 0; i < pts.GetCount(); i++) {
      if ((i % 2) != 0) {
        int x1 = points [i - 1].x;
        int y1 = points [i - 1].y;

        int x2 = points [i].x;
        int y2 = points [i].y;

        if (x1 > rectTab.CenterPoint().x && x2 > rectTab.CenterPoint().x) {
          x1--;
          x2--;
        }

        if (y2 >= y1) {
          pDC->MoveTo(x1, y1);
          pDC->LineTo(x2, y2);
        }
        else {
          pDC->MoveTo(x2, y2);
          pDC->LineTo(x1, y1);
        }
      }
    }

    delete [] points;
    pDC->SelectObject(pOLdPen);

    rectTab.l = nTabLeft;
    rectTab.r = nTabRight;
  }
  else { // 3D Tab
    CRect rectClip;
    pTabWnd->GetTabsRect(rectClip);

    BOOL bIsCutted = FALSE;

    const BOOL bIsOneNote = pTabWnd->IsOneNoteStyle() || pTabWnd->IsVS2005Style();

    const int nExtra = bIsOneNote ?
        ((iTab == 0 || bIsActive || pTabWnd->IsVS2005Style()) ?
            0 : rectTab.Height()) : 0;

    if (rectTab.l + nExtra + 10 > rectClip.r ||
        rectTab.r - 10 <= rectClip.l) {
      return;
    }

    const int iVertOffset = 2;
    const int iHorzOffset = 2;
    const BOOL bIs2005 = pTabWnd->IsVS2005Style();

#define POINTS_NUM  8
    IPOINT pts [POINTS_NUM];

    if (!bIsActive || bIsOneNote || clrTab != 0 || m_bAlwaysFillTab) {
      if (clrTab != 0 || bIsOneNote || m_bAlwaysFillTab) {
        COLOR clr = (clrTab == 0 ? globalData.clrBtnFace : clrTab);

        IPOINT* pt = NULL;
        int npt = 0;
        CRect rectFill = rectTab;

        if (bIsOneNote) {
          CRect rectFill = rectTab;

          const int nHeight = rectFill.Height();

          pts [0].x = rectFill.l;
          pts [0].y = rectFill.b;

          pts [1].x = rectFill.l;
          pts [1].y = rectFill.b;

          pts [2].x = rectFill.l + 2;
          pts [2].y = rectFill.b;

          pts [3].x = rectFill.l + nHeight;
          pts [3].y = rectFill.t + 2;

          pts [4].x = rectFill.l + nHeight + 4;
          pts [4].y = rectFill.t;

          pts [5].x = rectFill.r - 2;
          pts [5].y = rectFill.t;

          pts [6].x = rectFill.r;
          pts [6].y = rectFill.t + 2;

          pts [7].x = rectFill.r;
          pts [7].y = rectFill.b;

          for (int i = 0; i < POINTS_NUM; i++) {
            if (pts [i].x > rectClip.r) {
              pts [i].x = rectClip.r;
              bIsCutted = TRUE;
            }

            if (pTabWnd->GetLocation() == CBCGPBaseTabWnd::LOCATION_BOTTOM) {
              pts [i].y = rectFill.b - pts [i].y + rectFill.t - 1;
            }
          }

          pt = pts, npt = POINTS_NUM;
        }
        else {
          rectFill.DeflateRect(1, 0);

          if (pTabWnd->GetLocation() == CBCGPBaseTabWnd::LOCATION_BOTTOM) {
            rectFill.b--;
          }
          else {
            rectFill.t++;
          }

          rectFill.r = min(rectFill.r, rectClip.r);
        }

        OnFillTab(pDC, rectFill, clr, iTab, bIsActive, pTabWnd, pt, npt);

        if (bIsOneNote) {
          CRect rectLeft;
          pTabWnd->GetClientRect(rectLeft);
          rectLeft.r = rectClip.l - 1;

          pDC->ExcludeClipRect(rectLeft);

          if (iTab > 0 && !bIsActive && iTab != pTabWnd->GetFirstVisibleTabNum()) {
            CRect rectLeftTab = rectClip;
            rectLeftTab.r = rectFill.l + rectFill.Height() - 10;

            const int nVertOffset = bIs2005 ? 2 : 1;

            if (pTabWnd->GetLocation() == CBCGPBaseTabWnd::LOCATION_BOTTOM) {
              rectLeftTab.t -= nVertOffset;
            }
            else {
              rectLeftTab.b += nVertOffset;
            }

            pDC->ExcludeClipRect(rectLeftTab);
          }

          pDC->Polyline(pts, POINTS_NUM);

          if (bIsCutted) {
            pDC->MoveTo(rectClip.r, rectTab.t);
            pDC->LineTo(rectClip.r, rectTab.b);
          }

          CRect rectRight = rectClip;
          rectRight.l = rectFill.r;

          pDC->ExcludeClipRect(rectRight);
        }
      }
    }

    COLOR penLight = (globalData.clrBarHilite);
    COLOR penShadow = (globalData.clrBarShadow);
    COLOR penDark = (globalData.clrBarDkShadow);

    COLOR pOldPen;

    if (bIsOneNote) {
      pOldPen = pDC->SelectObject(penLight);
      ASSERT(pOldPen != NULL);

      if (pTabWnd->GetLocation() == CBCGPBaseTabWnd::LOCATION_BOTTOM) {
        if (!bIsCutted) {
          int yTop = bIsActive ? pts [7].y - 1 : pts [7].y;

          pDC->MoveTo(pts [6].x - 1, pts [6].y);
          pDC->LineTo(pts [7].x - 1, yTop);
        }
      }
      else {
        pDC->MoveTo(pts [2].x + 1, pts [2].y);
        pDC->LineTo(pts [3].x + 1, pts [3].y);

        pDC->MoveTo(pts [3].x + 1, pts [3].y);
        pDC->LineTo(pts [3].x + 2, pts [3].y);

        pDC->MoveTo(pts [3].x + 2, pts [3].y);
        pDC->LineTo(pts [3].x + 3, pts [3].y);

        pDC->MoveTo(pts [4].x - 1, pts [4].y + 1);
        pDC->LineTo(pts [5].x + 1, pts [5].y + 1);

        if (!bIsActive && !bIsCutted && m_b3DTabWideBorder) {
          pDC->SelectObject(penShadow);

          pDC->MoveTo(pts [6].x - 2, pts [6].y - 1);
          pDC->LineTo(pts [6].x - 1, pts [6].y - 1);
        }

        pDC->MoveTo(pts [6].x - 1, pts [6].y);
        pDC->LineTo(pts [7].x - 1, pts [7].y);
      }
    }
    else {
      if (rectTab.r > rectClip.r) {
        CRect rectTabClip = rectTab;
        rectTabClip.r = rectClip.r;

        pDC->SetClipBox(&rectTabClip);
      }

      if (pTabWnd->GetLocation() == CBCGPBaseTabWnd::LOCATION_BOTTOM) {
        pOldPen = pDC->SelectObject(penLight);
        ASSERT(pOldPen != NULL);

        if (!m_b3DTabWideBorder) {
          pDC->SelectObject(penShadow);
        }

        pDC->MoveTo(rectTab.l, rectTab.t);
        pDC->LineTo(rectTab.l, rectTab.b - iVertOffset);

        if (m_b3DTabWideBorder) {
          pDC->SelectObject(penDark);
        }

        pDC->LineTo(rectTab.l + iHorzOffset, rectTab.b);
        pDC->LineTo(rectTab.r - iHorzOffset, rectTab.b);
        pDC->LineTo(rectTab.r, rectTab.b - iVertOffset);
        pDC->LineTo(rectTab.r, rectTab.t - 1);

        pDC->SelectObject(penShadow);

        if (m_b3DTabWideBorder) {
          pDC->MoveTo(rectTab.l + iHorzOffset + 1, rectTab.b - 1);
          pDC->LineTo(rectTab.r - iHorzOffset, rectTab.b - 1);
          pDC->LineTo(rectTab.r - 1, rectTab.b - iVertOffset);
          pDC->LineTo(rectTab.r - 1, rectTab.t - 1);
        }
      }
      else {
        pOldPen = pDC->SelectObject(m_b3DTabWideBorder ? penDark : penShadow);

        ASSERT(pOldPen != NULL);

        pDC->MoveTo(rectTab.r, bIsActive ? rectTab.b : rectTab.b - 1);
        pDC->LineTo(rectTab.r, rectTab.t + iVertOffset);
        pDC->LineTo(rectTab.r - iHorzOffset, rectTab.t);

        if (m_b3DTabWideBorder) {
          pDC->SelectObject(penLight);
        }

        pDC->LineTo(rectTab.l + iHorzOffset, rectTab.t);
        pDC->LineTo(rectTab.l, rectTab.t + iVertOffset);

        pDC->LineTo(rectTab.l, rectTab.b);

        if (m_b3DTabWideBorder) {
          pDC->SelectObject(penShadow);

          pDC->MoveTo(rectTab.r - 1, bIsActive ? rectTab.b : rectTab.b - 1);
          pDC->LineTo(rectTab.r - 1, rectTab.t + iVertOffset - 1);
        }
      }
    }

    if (bIsActive) {
      const int iBarHeight = 1;
      const int y = (pTabWnd->GetLocation() == CBCGPBaseTabWnd::LOCATION_BOTTOM) ?
          (rectTab.t - iBarHeight - 1) : (rectTab.b);

      CRect rectFill(CPoint(rectTab.l, y),
          CSize(rectTab.Width(), iBarHeight + 1));

      COLORREF clrActiveTab = pTabWnd->GetTabBkColor(iTab);

      if (bIsOneNote) {
        if (bIs2005) {
          rectFill.l += 3;
        }
        else {
          rectFill.OffsetRect(1, 0);
          rectFill.l++;
        }

        if (clrActiveTab == 0) {
          clrActiveTab = globalData.clrWindow;
        }
      }

      if (clrActiveTab != 0) {
        FillRect(pDC, rectFill, clrActiveTab);
      }
      else {
        FillRect(pDC, rectFill, globalData.brBarFace);
      }
    }

    pDC->SelectObject(pOldPen);

    if (bIsOneNote) {
      const int nLeftMargin = pTabWnd->IsVS2005Style() && bIsActive ?
          rectTab.Height() * 3 / 4 : rectTab.Height();

      const int nRightMargin = pTabWnd->IsVS2005Style() && bIsActive ?
          CBCGPBaseTabWnd::TAB_IMAGE_MARGIN * 3 / 4 : CBCGPBaseTabWnd::TAB_IMAGE_MARGIN;

      rectTab.l += nLeftMargin;
      rectTab.r -= nRightMargin;

      if (pTabWnd->IsVS2005Style() && bIsActive && pTabWnd->HasImage(iTab)) {
        rectTab.OffsetRect(CBCGPBaseTabWnd::TAB_IMAGE_MARGIN, 0);
      }
    }

    pDC->SelectClipRgn(NULL);
  }

  COLORREF clrText = pTabWnd->GetTabTextColor(iTab);

  COLORREF cltTextOld = 0;

  if (!bIsActive && clrText != 0) {
    cltTextOld = pDC->SetTextColor(clrText);
  }

  if (pTabWnd->IsOneNoteStyle() || pTabWnd->IsVS2005Style()) {
    CRect rectClip;
    pTabWnd->GetTabsRect(rectClip);

    rectTab.r = min(rectTab.r, rectClip.r - 2);
  }

  pDC->SetClipBox(&rectClip);

  OnDrawTabContent(pDC, rectTab, iTab, bIsActive, pTabWnd, 0);

  if (cltTextOld != 0) {
    pDC->SetTextColor(cltTextOld);
  }

  pDC->SelectClipRgn(NULL);
}
//****
void CBCGPVisualManager::OnFillTab(CDC* pDC, CRect rectFill, COLOR pbrFill, int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd, IPOINT* pt, int npt)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pbrFill);
  ASSERT_VALID(pTabWnd);

  if (bIsActive && !globalData.IsHighContastMode() &&
      (pTabWnd->IsOneNoteStyle() || pTabWnd->IsVS2005Style() || pTabWnd->IsLeftRightRounded()) &&
      pTabWnd->GetTabBkColor(iTab) == 0) {
    pbrFill = globalData.brWindow;
  }

  if (pt) {
    imdraw_poly(pDC->im, &rectFill, pt, npt, 1, pbrFill, 0, 0);
  }
  else {
    FillRect(pDC, rectFill, pbrFill);
  }
}
//****
BOOL CBCGPVisualManager::OnEraseTabsFrame(CDC* pDC, CRect rect,
    const CBCGPBaseTabWnd* pTabWnd)
{
  ASSERT_VALID(pTabWnd);
  ASSERT_VALID(pDC);

  COLORREF clrActiveTab = pTabWnd->GetTabBkColor(pTabWnd->GetActiveTab());

  if (clrActiveTab == 0) {
    return FALSE;
  }

  pDC->FillSolidRect(rect, clrActiveTab);
  return TRUE;
}
//****
void CBCGPVisualManager::OnDrawTabContent(CDC* pDC, CRect rectTab,
    int iTab, BOOL bIsActive, const CBCGPBaseTabWnd* pTabWnd,
    COLORREF clrText)
{
  ASSERT_VALID(pTabWnd);
  ASSERT_VALID(pDC);

  if (pTabWnd->IsActiveTabCloseButton() && bIsActive) {
    CRect rectClose = pTabWnd->GetTabCloseButton();
    rectTab.r = rectClose.l;

    OnDrawTabCloseButton(pDC, rectClose, pTabWnd,
        pTabWnd->IsTabCloseButtonHighlighted(),
        pTabWnd->IsTabCloseButtonPressed(),
        FALSE /* Disabled */);
  }

  CString strText;
  pTabWnd->GetTabLabel(iTab, strText);

  if (pTabWnd->IsFlatTab()) {
    //---------------
    // Draw tab text:
    //---------------
    UINT nFormat = DT_SINGLELINE | DT_CENTER | DT_VCENTER;

    if (pTabWnd->IsDrawNoPrefix()) {
      nFormat |= DT_NOPREFIX;
    }

    pDC->DrawText(strText, rectTab, nFormat);
  }
  else {
    CSize sizeImage = pTabWnd->GetImageSize();
    UINT uiIcon = pTabWnd->GetTabIcon(iTab);
    CBitmap* hIcon = pTabWnd->GetTabHicon(iTab);

    if (uiIcon == (UINT) - 1 && hIcon == NULL) {
      sizeImage.w = 0;
    }

    if (sizeImage.w + 2 * CBCGPBaseTabWnd::TAB_IMAGE_MARGIN <= rectTab.Width()) {
      if (hIcon != NULL) {
        //---------------------
        // Draw the tab's icon:
        //---------------------
        CRect rectImage = rectTab;

        rectImage.t += (rectTab.Height() - sizeImage.h) / 2;
        rectImage.b = rectImage.t + sizeImage.h;

        rectImage.l += IMAGE_MARGIN;
        rectImage.r = rectImage.l + sizeImage.w;

        DrawIconEx(pDC, rectImage.l, rectImage.t, hIcon,
            0, 0, 0, NULL, DI_NORMAL);
      }
      else {
        const CImageList* pImageList = pTabWnd->GetImageList();

        if (pImageList != NULL && uiIcon != (UINT) - 1) {
          //----------------------
          // Draw the tab's image:
          //----------------------
          CRect rectImage = rectTab;

          rectImage.t += (rectTab.Height() - sizeImage.h) / 2;
          rectImage.b = rectImage.t + sizeImage.h;

          rectImage.l += IMAGE_MARGIN;
          rectImage.r = rectImage.l + sizeImage.w;

          ASSERT_VALID(pImageList);
          CImageList_Draw(pDC, pImageList, uiIcon, rectImage.TopLeft(), ILD_TRANSPARENT);
        }
      }

      //------------------------------
      // Finally, draw the tab's text:
      //------------------------------
      CRect rcText = rectTab;
      rcText.l += sizeImage.w + 2 * TEXT_MARGIN;

      if (rcText.Width() < sizeImage.w * 2 &&
          !pTabWnd->IsLeftRightRounded()) {
        rcText.r -= TEXT_MARGIN;
      }

      if (clrText == 0) {
        clrText = GetTabTextColor(pTabWnd, iTab, bIsActive);
      }

      if (clrText != 0) {
        pDC->SetTextColor(clrText);
      }

      CString strText;
      pTabWnd->GetTabLabel(iTab, strText);

      UINT nFormat = DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS;

      if (pTabWnd->IsDrawNoPrefix()) {
        nFormat |= DT_NOPREFIX;
      }

      if (pTabWnd->IsOneNoteStyle() || pTabWnd->IsVS2005Style()) {
        nFormat |= DT_CENTER;
      }
      else {
        nFormat |= DT_LEFT;
      }

      pDC->DrawText(strText, rcText, nFormat);
    }
  }
}
//****
void CBCGPVisualManager::OnDrawTabCloseButton(CDC* pDC, CRect rect,
    const CBCGPBaseTabWnd* /*pTabWnd*/,
    BOOL bIsHighlighted,
    BOOL bIsPressed,
    BOOL /*bIsDisabled*/)
{
  if (bIsHighlighted) {
    FillRect(pDC, rect, globalData.brBarFace);
  }

  CBCGPMenuImages_Draw(pDC, IdClose, rect, ImageBlack);

  if (bIsHighlighted) {
    if (bIsPressed) {
      pDC->Draw3dRect(rect, globalData.clrBarDkShadow, globalData.clrBarHilite);
    }
    else {
      pDC->Draw3dRect(rect, globalData.clrBarHilite, globalData.clrBarDkShadow);
    }
  }
}
//****
void CBCGPVisualManager::OnEraseTabsButton(CDC* pDC, CRect rect,
    CBCGPButtonBase* /*pButton*/,
    CBCGPBaseTabWnd* /*pWndTab*/)
{
  ASSERT_VALID(pDC);
  FillRect(pDC, rect, globalData.brBarFace);
}
//****
void CBCGPVisualManager::OnDrawTabsButtonBorder(CDC* pDC, CRect& rect,
    CBCGPButtonBase* pButton, UINT uiState,
    CBCGPBaseTabWnd* /*pWndTab*/)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pButton);

  if (pButton->IsPressed() || (uiState & ODS_SELECTED)) {
    pDC->Draw3dRect(rect, globalData.clrBarDkShadow, globalData.clrBarHilite);

    rect.l += 2;
    rect.t += 2;
  }
  else {
    pDC->Draw3dRect(rect, globalData.clrBarHilite, globalData.clrBarDkShadow);
  }

  rect.DeflateRect(2, 2);
}
//****
void CBCGPVisualManager::OnDrawTabResizeBar(CDC* pDC, CBCGPBaseTabWnd* /*pWndTab*/,
    BOOL bIsVert, CRect rect,
    COLOR pbrFace, COLOR pPen)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pbrFace);
  ASSERT_VALID(pPen);

  FillRect(pDC, rect, pbrFace);

  COLOR pOldPen = pDC->SelectObject(pPen);
  ASSERT_VALID(pOldPen);

  if (bIsVert) {
    pDC->MoveTo(rect.l, rect.t);
    pDC->LineTo(rect.l, rect.b);
  }
  else {
    pDC->MoveTo(rect.l, rect.t);
    pDC->LineTo(rect.r, rect.t);
  }

  pDC->SelectObject(pOldPen);
}
//****
COLORREF CBCGPVisualManager::OnFillCommandsListBackground(CDC* pDC, CRect rect, BOOL bIsSelected)
{
  ASSERT_VALID(this);
  ASSERT_VALID(pDC);

  if (bIsSelected) {

    const int nFrameSize = 1;
    COLOR clr = globalData.brHilite;
    imdraw_rect(pDC->im, &rect, rect, clr, (~clr) | _RGB(0, 0, 0), 1, BDM_ALL);
#if 0
    FillRect(pDC, rect, globalData.brHilite);
    rect.DeflateRect(1, 1);
    rect.r--;
    rect.b--;
    pDC->PatBlt(rect.l, rect.t + nFrameSize, nFrameSize, rect.Height(), PATINVERT);
    pDC->PatBlt(rect.l, rect.t, rect.Width(), nFrameSize, PATINVERT);
    pDC->PatBlt(rect.r, rect.t, nFrameSize, rect.Height(), PATINVERT);
    pDC->PatBlt(rect.l + nFrameSize, rect.b, rect.Width(), nFrameSize, PATINVERT);
#endif
    return globalData.clrTextHilite;
  }

  FillRect(pDC, rect, globalData.brBarFace);

  return globalData.clrBarText;
}
//****
void CBCGPVisualManager::OnDrawMenuArrowOnCustomizeList(CDC* pDC,
    CRect rectCommand, BOOL bSelected)
{
  CRect rectTriangle = rectCommand;
  rectTriangle.l = rectTriangle.r - MENUIMAGES_W;

  CBCGPMenuImages_Draw(pDC, IdArowRightLarge, rectTriangle,
      bSelected ? ImageWhite : ImageBlack);

  CRect rectLine = rectCommand;
  rectLine.r = rectTriangle.l - 1;
  rectLine.l = rectLine.r - 2;
  rectLine.DeflateRect(0, 2);

  pDC->Draw3dRect(&rectLine, globalData.clrBtnShadow, globalData.clrBtnHilite);
}

//****
void CBCGPVisualManager::OnDrawTearOffCaption(CDC* pDC, CRect rect, BOOL bIsActive)
{
  const int iBorderSize = 2;

  ASSERT_VALID(pDC);

  FillRect(pDC, rect, globalData.brBarFace);

  rect.DeflateRect(iBorderSize, 1);

  pDC->FillSolidRect(rect,
      bIsActive ?
      globalData.clrActiveCaption :
      globalData.clrInactiveCaption);
}
//****
void CBCGPVisualManager::OnDrawMenuResizeBar(CDC* pDC, CRect rect, int /*nResizeFlags*/)
{
  ASSERT_VALID(pDC);

  pDC->FillSolidRect(rect, globalData.clrInactiveCaption);
}
//****
void CBCGPVisualManager::OnDrawMenuScrollButton(CDC* pDC, CRect rect, BOOL bIsScrollDown,
    BOOL bIsHighlited, BOOL /*bIsPressed*/,
    BOOL /*bIsDisabled*/)
{
  ASSERT_VALID(pDC);

  CRect rectFill = rect;
  rectFill.t -= 2;

  FillRect(pDC, rectFill, globalData.brBarFace);

  CBCGPMenuImages_Draw(pDC, bIsScrollDown ? IdArowDown : IdArowUp, rect);

  if (bIsHighlited) {
    pDC->Draw3dRect(rect,
        globalData.clrBarHilite,
        globalData.clrBarShadow);
  }
}
//****
void CBCGPVisualManager::OnDrawMenuSystemButton(CDC* pDC, CRect rect,
    UINT uiSystemCommand,
    UINT nStyle, BOOL /*bHighlight*/)
{
  ASSERT_VALID(pDC);

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

  if (nStyle & TBBS_PRESSED) {
    uiState |= DFCS_PUSHED;
  }

  if (nStyle & TBBS_DISABLED) { // Jan Vasina: Add support for disabled buttons
    uiState |= DFCS_INACTIVE;
  }

  pDC->DrawFrameControl(rect, DFC_CAPTION, uiState);
}
//****
void CBCGPVisualManager::OnDrawComboDropButton(CDC* pDC, CRect rect,
    BOOL bDisabled,
    BOOL bIsDropped,
    BOOL bIsHighlighted,
    CBCGPToolbarComboBoxButton* /*pButton*/)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(this);

  COLORREF clrText = pDC->GetTextColor();

  if (m_bIsDrawOnGlass) {
    imdraw_DrawRectR(pDC->im, 0, rect, globalData.clrBarFace, globalData.clrBarHilite);

    if (bIsDropped) {
      rect.OffsetRect(1, 1);
      imdraw_DrawRectR(pDC->im, 0, rect, 0, globalData.clrBarShadow);
    }
    else if (bIsHighlighted) {
      imdraw_DrawRectR(pDC->im, 0, rect, 0, globalData.clrBarShadow);
    }
  }
  else {
    FillRect(pDC, rect, globalData.brBarFace);
    pDC->Draw3dRect(rect, globalData.clrBarHilite, globalData.clrBarHilite);

    if (bIsDropped) {
      rect.OffsetRect(1, 1);
      pDC->Draw3dRect(&rect, globalData.clrBarShadow, globalData.clrBarHilite);
    }
    else if (bIsHighlighted) {
      pDC->Draw3dRect(&rect, globalData.clrBarHilite, globalData.clrBarShadow);
    }
  }

  CBCGPMenuImages_Draw(pDC, IdArowDown, rect,
      bDisabled ? ImageGray : ImageBlack);

  pDC->SetTextColor(clrText);
}
//****
void CBCGPVisualManager::OnDrawComboBorder(CDC* pDC, CRect rect,
    BOOL /*bDisabled*/,
    BOOL bIsDropped,
    BOOL bIsHighlighted,
    CBCGPToolbarComboBoxButton* /*pButton*/)
{
  ASSERT_VALID(pDC);

  if (bIsHighlighted || bIsDropped) {
    if (m_bMenuFlatLook) {
      rect.DeflateRect(1, 1);
    }

    if (m_bIsDrawOnGlass) {
      imdraw_DrawRectR(pDC->im, 0, rect, 0, globalData.clrBarDkShadow);
    }
    else {
      if (m_bMenuFlatLook) {
        pDC->Draw3dRect(&rect, globalData.clrBarDkShadow, globalData.clrBarDkShadow);
      }
      else {
        pDC->Draw3dRect(&rect, globalData.clrBarShadow, globalData.clrBarHilite);
      }
    }
  }
}
//****
void CBCGPVisualManager::OnDrawStatusBarPaneBorder(CDC* pDC, CBCGPStatusBar* /*pBar*/,
    CRect rectPane, UINT /*uiID*/, UINT nStyle)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(this);

  if (!(nStyle & SBPS_NOBORDERS)) {
    // draw the borders
    COLORREF clrHilite;
    COLORREF clrShadow;

    if (nStyle & SBPS_POPOUT) {
      // reverse colors
      clrHilite = globalData.clrBarShadow;
      clrShadow = globalData.clrBarHilite;
    }
    else {
      // normal colors
      clrHilite = globalData.clrBarHilite;
      clrShadow = globalData.clrBarShadow;
    }

    pDC->Draw3dRect(rectPane, clrShadow, clrHilite);
  }
}
//****
COLORREF CBCGPVisualManager::OnFillMiniFrameCaption(CDC* pDC,
    CRect rectCaption,
    CBCGPMiniFrameWnd* pFrameWnd, BOOL bActive)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pFrameWnd);

  COLOR clr = (bActive ? globalData.clrActiveCaption : globalData.clrInactiveCaption);
  FillRect(pDC, rectCaption, clr);

  // get the text color
  return bActive ? globalData.clrCaptionText : globalData.clrInactiveCaptionText;
}
//****
void CBCGPVisualManager::OnDrawMiniFrameBorder(
    CDC* pDC, CBCGPMiniFrameWnd* pFrameWnd,
    CRect rectBorder, CRect rectBorderSize)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pFrameWnd);

  int preDockState = pFrameWnd->GetPreDockState();

  if (preDockState == BCGP_PDS_DOCK_REGULAR) {
    // draw outer edge;
    pDC->Draw3dRect(rectBorder, _RGB(127, 0, 0), globalData.clrBarDkShadow);
    rectBorder.DeflateRect(1, 1);
    pDC->Draw3dRect(rectBorder, globalData.clrBarHilite, _RGB(127, 0, 0));
  }
  else if (preDockState == BCGP_PDS_DOCK_TO_TAB) {
    // draw outer edge;
    pDC->Draw3dRect(rectBorder, _RGB(0, 0, 127), globalData.clrBarDkShadow);
    rectBorder.DeflateRect(1, 1);
    pDC->Draw3dRect(rectBorder, globalData.clrBarHilite, _RGB(0, 0, 127));
  }
  else {
    // draw outer edge;
    pDC->Draw3dRect(rectBorder, globalData.clrBarFace, globalData.clrBarDkShadow);
    rectBorder.DeflateRect(1, 1);
    pDC->Draw3dRect(rectBorder, globalData.clrBarHilite, globalData.clrBarShadow);
  }

  // draw the inner egde
  rectBorder.DeflateRect(rectBorderSize.r - 2, rectBorderSize.t - 2);
  pDC->Draw3dRect(rectBorder, globalData.clrBarFace, globalData.clrBarFace);
  rectBorder.InflateRect(1, 1);
  pDC->Draw3dRect(rectBorder, globalData.clrBarFace, globalData.clrBarFace);
}
//****
void CBCGPVisualManager::OnDrawFloatingToolbarBorder(
    CDC* pDC, CBCGPBaseToolBar* /*pToolBar*/,
    CRect rectBorder, CRect /*rectBorderSize*/)
{
  ASSERT_VALID(pDC);

  pDC->Draw3dRect(rectBorder, globalData.clrBarFace, globalData.clrBarDkShadow);
  rectBorder.DeflateRect(1, 1);
  pDC->Draw3dRect(rectBorder, globalData.clrBarHilite, globalData.clrBarShadow);
  rectBorder.DeflateRect(1, 1);
  pDC->Draw3dRect(rectBorder, globalData.clrBarFace, globalData.clrBarFace);
}
//****
COLORREF GetHotTextColor()
{
  return globalData.clrBtnText;
}
//****
COLORREF CBCGPVisualManager::GetToolbarButtonTextColor(CBCGPToolbarButton* pButton,
    BCGBUTTON_STATE state)
{
  ASSERT_VALID(pButton);

  BOOL bDisabled = !pButton->IsEnabled();

  if (pButton->GetClass() == CLASS_OutlookButton) {
    if (globalData.IsHighContastMode()) {
      return bDisabled ? globalData.clrGrayedText : globalData.clrWindowText;
    }

    return bDisabled ? globalData.clrBtnFace : globalData.clrWindow;
  }

  return (bDisabled ? globalData.clrGrayedText :
      (state == ButtonsIsHighlighted) ?
      GetHotTextColor() : globalData.clrBarText);
}
//****
BOOL CBCGPVisualManager::IsToolBarButtonDefaultBackground(CBCGPToolbarButton* pButton,
    BCGBUTTON_STATE /*state*/)
{
  ASSERT_VALID(pButton);
  return !(pButton->GetStyle() & (TBBS_CHECKED | TBBS_INDETERMINATE));
}
//****
void CBCGPVisualManager::OnFillOutlookPageButton(CDC* pDC,
    const CRect& rect,
    BOOL /*bIsHighlighted*/, BOOL /*bIsPressed*/,
    COLORREF& clrText)
{
  ASSERT_VALID(pDC);

  FillRect(pDC, rect, globalData.brBarFace);
  clrText = globalData.clrBarText;
}
//****
void CBCGPVisualManager::OnDrawOutlookPageButtonBorder(
    CDC* pDC, CRect& rectBtn, BOOL bIsHighlighted, BOOL bIsPressed)
{
  ASSERT_VALID(pDC);

  if (bIsHighlighted && bIsPressed) {
    pDC->Draw3dRect(rectBtn, globalData.clrBarDkShadow, globalData.clrBarFace);
    rectBtn.DeflateRect(1, 1);
    pDC->Draw3dRect(rectBtn, globalData.clrBarShadow, globalData.clrBarHilite);
  }
  else {
    if (bIsHighlighted || bIsPressed) {
      pDC->Draw3dRect(rectBtn, globalData.clrBarFace, globalData.clrBarDkShadow);
      rectBtn.DeflateRect(1, 1);
    }

    pDC->Draw3dRect(rectBtn, globalData.clrBarHilite, globalData.clrBarShadow);
  }

  rectBtn.DeflateRect(1, 1);
}
//****
COLORREF CBCGPVisualManager::GetCaptionBarTextColor(CBCGPCaptionBar* pBar)
{
  ASSERT_VALID(pBar);

  return pBar->IsMessageBarMode() ? _GetSysColor(CLR_INFOTEXT) : globalData.clrWindow;
}
//****
void CBCGPVisualManager::OnDrawCaptionBarBorder(CDC* pDC,
    CBCGPCaptionBar* /*pBar*/, CRect rect, COLORREF clrBarBorder, BOOL bFlatBorder)
{
  ASSERT_VALID(pDC);

  if (clrBarBorder == (COLORREF) - 1) {
    FillRect(pDC, rect, globalData.brBarFace);
  }
  else {
    FillRect(pDC, rect, clrBarBorder);
  }

  if (!bFlatBorder) {
    pDC->Draw3dRect(rect, globalData.clrBarHilite, globalData.clrBarShadow);
  }
}
//****
void CBCGPVisualManager::OnDrawCaptionBarInfoArea(CDC* pDC, CBCGPCaptionBar* /*pBar*/, CRect rect)
{
  ASSERT_VALID(pDC);

  FillRect(pDC, rect, GetSysColorBrush(CLR_INFOBK));

  pDC->Draw3dRect(rect, globalData.clrBarShadow, globalData.clrBarHilite);
  rect.DeflateRect(1, 1);
  pDC->Draw3dRect(rect, globalData.clrBarHilite, globalData.clrBarShadow);
}
//****
COLORREF CBCGPVisualManager::OnFillCaptionBarButton(CDC* pDC, CBCGPCaptionBar* pBar,
    CRect rect, BOOL bIsPressed, BOOL bIsHighlighted,
    BOOL bIsDisabled, BOOL bHasDropDownArrow,
    BOOL bIsSysButton)
{
  UNREFERENCED_PARAMETER(bIsPressed);
  UNREFERENCED_PARAMETER(bIsHighlighted);
  UNREFERENCED_PARAMETER(bIsDisabled);
  UNREFERENCED_PARAMETER(bHasDropDownArrow);
  UNREFERENCED_PARAMETER(bIsSysButton);

  ASSERT_VALID(pBar);

  if (!pBar->IsMessageBarMode()) {
    return 0;
  }

  ASSERT_VALID(pDC);

  FillRect(pDC, rect, globalData.brBarFace);
  return bIsDisabled ? globalData.clrGrayedText : globalData.clrBarText;
}
//****
void CBCGPVisualManager::OnDrawCaptionBarButtonBorder(CDC* pDC, CBCGPCaptionBar* pBar,
    CRect rect, BOOL bIsPressed, BOOL bIsHighlighted,
    BOOL bIsDisabled, BOOL bHasDropDownArrow,
    BOOL bIsSysButton)
{
  UNREFERENCED_PARAMETER(bIsDisabled);
  UNREFERENCED_PARAMETER(bHasDropDownArrow);
  UNREFERENCED_PARAMETER(bIsSysButton);

  ASSERT_VALID(pDC);
  ASSERT_VALID(pBar);

  if (bIsPressed) {
    pDC->Draw3dRect(rect, globalData.clrBarDkShadow, globalData.clrBarHilite);
  }
  else if (bIsHighlighted || pBar->IsMessageBarMode()) {
    pDC->Draw3dRect(rect, globalData.clrBarHilite, globalData.clrBarDkShadow);
  }
}
//****
void CBCGPVisualManager::OnDrawStatusBarProgress(CDC* pDC, CBCGPStatusBar* /*pStatusBar*/,
    CRect rectProgress, int nProgressTotal, int nProgressCurr,
    COLORREF clrBar, COLORREF clrProgressBarDest, COLORREF clrProgressText,
    BOOL bProgressText)
{
  ASSERT_VALID(pDC);

  if (nProgressTotal == 0) {
    return;
  }

  CRect rectComplete = rectProgress;
  rectComplete.r = rectComplete.l +
      nProgressCurr * rectComplete.Width() / nProgressTotal;

  if (clrProgressBarDest == 0) {
    // one-color bar
    COLOR clr = (clrBar);
    FillRect(pDC, rectComplete, clr);
  }
  else {
    // gradient bar:
    FillGradient(pDC->im, 0, rectComplete, clrBar, clrProgressBarDest, FALSE);
  }

  if (bProgressText) {
    char strText[256];
    _snprintf(strText, 256, _T("%d%%"), nProgressCurr * 100 / nProgressTotal);

    COLORREF clrText = pDC->SetTextColor(globalData.clrBarText);

    pDC->DrawText(strText, rectProgress, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);

    pDC->SetClipBox(&rectComplete);
    pDC->SetTextColor(clrProgressText == 0 ?
        globalData.clrTextHilite : clrProgressText);
    pDC->DrawText(strText, rectProgress, DT_CENTER | DT_SINGLELINE | DT_VCENTER | DT_NOPREFIX);
    pDC->SetTextColor(clrText);
  }
}
//****
void CBCGPVisualManager::OnFillHeaderCtrlBackground(CBCGPHeaderCtrl* pCtrl,
    CDC* pDC, CRect rect)
{
  ASSERT_VALID(pDC);
  FillRect(pDC, &rect, pCtrl->IsDialogControl() ? globalData.brBtnFace : globalData.brBarFace);
}
//****
void CBCGPVisualManager::OnDrawHeaderCtrlBorder(CBCGPHeaderCtrl* pCtrl, CDC* pDC,
    CRect& rect, BOOL bIsPressed, BOOL /*bIsHighlighted*/)
{
  ASSERT_VALID(pDC);

  if (bIsPressed) {
    if (pCtrl->IsDialogControl()) {
      pDC->Draw3dRect(rect, globalData.clrBtnShadow, globalData.clrBtnShadow);
    }
    else {
      pDC->Draw3dRect(rect, globalData.clrBarShadow, globalData.clrBarShadow);
    }

    rect.l++;
    rect.t++;
  }
  else {
    if (pCtrl->IsDialogControl()) {
      pDC->Draw3dRect(rect, globalData.clrBtnHilite, globalData.clrBtnShadow);
    }
    else {
      pDC->Draw3dRect(rect, globalData.clrBarHilite, globalData.clrBarShadow);
    }
  }
}
//****
void CBCGPVisualManager::OnDrawHeaderCtrlSortArrow(CBCGPHeaderCtrl* pCtrl,
    CDC* pDC,
    CRect& rectArrow, BOOL bIsUp)
{
  DoDrawHeaderSortArrow(pDC, rectArrow, bIsUp, pCtrl != NULL && pCtrl->IsDialogControl());
}
//****
void CBCGPVisualManager::OnDrawStatusBarSizeBox(CDC* pDC, CBCGPStatusBar* /*pStatBar*/,
    CRect rectSizeBox)
{
  ASSERT_VALID(pDC);

  CFont* pOldFont = pDC->SelectObject(globalData.fontMarlett);
  ASSERT(pOldFont != NULL);

  const CString strSizeBox(_T("o"));  // Char of the sizing box in "Marlett" font

  UINT nTextAlign = pDC->SetTextAlign(TA_RIGHT | TA_BOTTOM);
  COLORREF clrText = pDC->SetTextColor(globalData.clrBarShadow);

  pDC->ExtTextOut(rectSizeBox.r, rectSizeBox.b, ETO_CLIPPED, &rectSizeBox, strSizeBox, NULL);

  pDC->SelectObject(pOldFont);
  pDC->SetTextColor(clrText);
  pDC->SetTextAlign(nTextAlign);
}
//****
void CBCGPVisualManager::OnDrawEditBorder(CDC* pDC, CRect rect,
    BOOL /*bDisabled*/,
    BOOL bIsHighlighted,
    CBCGPToolbarEditBoxButton* /*pButton*/)
{
  ASSERT_VALID(pDC);

  if (bIsHighlighted) {
    pDC->DrawEdge(rect, EDGE_SUNKEN, BF_RECT);
  }
}

#ifndef BCGP_EXCLUDE_TASK_PANE

void CBCGPVisualManager::OnFillTasksPaneBackground(CDC* pDC, CRect rectWorkArea)
{
  ASSERT_VALID(pDC);

  FillRect(pDC, rectWorkArea, globalData.brWindow);
}
//****
void CBCGPVisualManager::OnDrawTasksGroupCaption(CDC* pDC, CBCGPTasksGroup* pGroup,
    BOOL bIsHighlighted, BOOL bIsSelected, BOOL bCanCollapse)
{
  ASSERT_VALID(pDC);
  ASSERT(pGroup != NULL);

  BOOL bIsToolBox = FALSE;
  CRect rectGroup = pGroup->GetRect();

  if (bIsToolBox) {
    CRect rectFill = rectGroup;
    rectFill.DeflateRect(1, 0, 1, 1);

    FillRect(pDC, rectFill, globalData.clrBarShadow);

    if (bCanCollapse) {
      //--------------------
      // Draw expanding box:
      //--------------------
      int nBoxSize = 9;
      int nBoxOffset = 6;

      if (globalData.GetRibbonImageScale() != 1.) {
        nBoxSize = (int)(.5 + nBoxSize * globalData.GetRibbonImageScale());
      }

      CRect rectButton = rectFill;

      rectButton.l += nBoxOffset;
      rectButton.r = rectButton.l + nBoxSize;
      rectButton.t = rectButton.CenterPoint().y - nBoxSize / 2;
      rectButton.b = rectButton.t + nBoxSize;

      FillRect(pDC, rectButton, globalData.brBarFace);

      OnDrawExpandingBox(pDC, rectButton, !pGroup->IsCollapsed(),
          globalData.clrBarText);

      rectGroup.l = rectButton.r + nBoxOffset;
      bCanCollapse = FALSE;
    }
  }
  else {
    // ---------------------------------
    // Draw caption background (Windows)
    // ---------------------------------
    COLORREF clrBckOld = pDC->GetBkColor();
    pDC->FillSolidRect(rectGroup,
        (pGroup->IsSpecial() ? globalData.clrHilite : globalData.clrBarFace));
    pDC->SetBkColor(clrBckOld);
  }

  // ---------------------------
  // Draw an icon if it presents
  // ---------------------------
  img_t* m_hIcon = pGroup->GetIcon();
  BOOL bShowIcon = (m_hIcon != NULL
      && m_hIcon->w < rectGroup.Width() - rectGroup.Height());

  if (bShowIcon) {
    OnDrawTasksGroupIcon(pDC, pGroup, 5, bIsHighlighted, bIsSelected, bCanCollapse);
  }

  // -----------------------
  // Draw group caption text
  // -----------------------
  CFont* pFontOld = pDC->SelectObject(globalData.fontBold);
  COLORREF clrTextOld = pDC->GetTextColor();

  if (bIsToolBox) {
    pDC->SetTextColor(globalData.clrWindow);

  }
  else {
    if (bCanCollapse && bIsHighlighted) {
      clrTextOld = pDC->SetTextColor(pGroup->GetTextHotColor() == 0 ?
          (pGroup->IsSpecial() ? globalData.clrWindow : globalData.clrWindowText) :
            pGroup->GetTextHotColor());
    }
    else {
      clrTextOld = pDC->SetTextColor(pGroup->GetTextColor() == 0 ?
          (pGroup->IsSpecial() ? globalData.clrWindow : globalData.clrWindowText) :
            pGroup->GetTextColor());
    }
  }

  int nBkModeOld = pDC->SetBkMode(TRANSPARENT);

  int nTaskPaneHOffset = pGroup->GetGroupCaptionHorzOffset();
  int nTaskPaneVOffset = pGroup->GetGroupCaptionVertOffset();
  int nCaptionHOffset = (nTaskPaneHOffset != -1 ? nTaskPaneHOffset : m_nGroupCaptionHorzOffset);

  CRect rectText = rectGroup;
  rectText.l += (bShowIcon ? m_hIcon->w + 5 : nCaptionHOffset);
  rectText.t += (nTaskPaneVOffset != -1 ? nTaskPaneVOffset : m_nGroupCaptionVertOffset);
  rectText.r = max(rectText.l,
      rectText.r - (bCanCollapse ? rectGroup.Height() : nCaptionHOffset));

  pDC->DrawText(pGroup->GetName(), rectText, DT_SINGLELINE | DT_VCENTER);

  pDC->SetBkMode(nBkModeOld);
  pDC->SelectObject(pFontOld);
  pDC->SetTextColor(clrTextOld);

  CString m_strName = pGroup->GetStrName();

  // -------------------------
  // Draw group caption button
  // -------------------------
  if (bCanCollapse && !m_strName.IsEmpty()) {
    CSize sizeButton = CSize(MENUIMAGES_W, MENUIMAGES_H);
    CRect rectButton = rectGroup;
    rectButton.l = max(rectButton.l,
        rectButton.r - (rectButton.Height() + 1) / 2 - (sizeButton.w + 1) / 2);
    rectButton.t = max(rectButton.t,
        rectButton.b - (rectButton.Height() + 1) / 2 - (sizeButton.h + 1) / 2);
    rectButton.r = rectButton.l + sizeButton.w;
    rectButton.b = rectButton.t + sizeButton.h;

    if (rectButton.r <= rectGroup.r && rectButton.b <= rectGroup.b) {
      if (bIsHighlighted) {
        // Draw button frame
        pDC->Draw3dRect(&rectButton, globalData.clrWindow, globalData.clrBarShadow, globalData.brBarFace);
      }

      if (pGroup->IsSpecial()) {
        if (!pGroup->IsCollapsed()) {
          CBCGPMenuImages_Draw(pDC, IdArowUp, rectButton.TopLeft());
        }
        else {
          CBCGPMenuImages_Draw(pDC, IdArowDown, rectButton.TopLeft());
        }
      }
      else {
        if (!pGroup->IsCollapsed()) {
          CBCGPMenuImages_Draw(pDC, IdArowUp, rectButton.TopLeft());
        }
        else {
          CBCGPMenuImages_Draw(pDC, IdArowDown, rectButton.TopLeft());
        }
      }
    }
  }
}
//****
void CBCGPVisualManager::OnDrawTasksGroupIcon(CDC* pDC, CBCGPTasksGroup* pGroup,
    int nIconHOffset, BOOL /*bIsHighlighted*/, BOOL /*bIsSelected*/, BOOL /*bCanCollapse*/)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pGroup);
  img_t* m_hIcon = pGroup->GetIcon();

  if (m_hIcon == NULL) {
    return;
  }

  int nTaskPaneVOffset = pGroup->GetGroupCaptionVertOffset();

  CRect rectImage = pGroup->GetRect();
  rectImage.t += (nTaskPaneVOffset != -1 ? nTaskPaneVOffset : m_nGroupCaptionVertOffset);
  rectImage.r = rectImage.l + m_hIcon->w + nIconHOffset;

  int x = max(0, (rectImage.Width() - m_hIcon->w) / 2);
  int y = max(0, (rectImage.Height() - m_hIcon->h) / 2);

  ::DrawIconEx(pDC,
      rectImage.l + x, rectImage.b - y - m_hIcon->h,
      m_hIcon, m_hIcon->w, m_hIcon->h,
      0, NULL, DI_NORMAL);
}
//****
void CBCGPVisualManager::OnFillTasksGroupInterior(CDC* /*pDC*/, CRect /*rect*/, BOOL /*bSpecial*/)
{
}
//****
void CBCGPVisualManager::OnDrawTasksGroupAreaBorder(CDC* pDC, CRect rect, BOOL bSpecial,
    BOOL bNoTitle)
{
  ASSERT_VALID(pDC);

  // Draw caption background:
  COLOR pPenOld = (COLOR) pDC->SelectObject(bSpecial ? globalData.penHilite : globalData.penBarFace);

  pDC->MoveTo(rect.l, rect.t);
  pDC->LineTo(rect.l, rect.b - 1);
  pDC->LineTo(rect.r - 1, rect.b - 1);
  pDC->LineTo(rect.r - 1, rect.t);

  if (bNoTitle) {
    pDC->LineTo(rect.l, rect.t);
  }
  else {
    pDC->LineTo(rect.r - 1, rect.t - 1);
  }

  pDC->SelectObject(pPenOld);

}
//****
void CBCGPVisualManager::OnDrawTask(CDC* pDC, CBCGPTask* pTask, CImageList* pIcons,
    BOOL bIsHighlighted, BOOL /*bIsSelected*/)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pIcons);
  ASSERT(pTask != NULL);

  CRect rectText = pTask->GetRect();

  if (pTask->IsSeparator()) {
    COLOR pPenOld = pDC->SelectObject(globalData.penBarFace);

    pDC->MoveTo(rectText.l, rectText.CenterPoint().y);
    pDC->LineTo(rectText.r, rectText.CenterPoint().y);

    pDC->SelectObject(pPenOld);
    return;
  }

  // ---------
  // Draw icon
  // ---------
  CSize sizeIcon(0, 0);
  ImageList_GetIconSize(pIcons, (int*) &sizeIcon.w, (int*) &sizeIcon.h);
  int m_nIcon = pTask->GetIconIndex();

  if (m_nIcon >= 0 && sizeIcon.w > 0) {
    CImageList_Draw(pDC, pIcons, m_nIcon, rectText.TopLeft(), ILD_TRANSPARENT);
  }

  int nTaskPaneOffset = pTask->GetTasksIconHorzOffset();
  rectText.l += sizeIcon.w + (nTaskPaneOffset != -1 ? nTaskPaneOffset : m_nTasksIconHorzOffset);

  // ---------
  // Draw text
  // ---------
  BOOL bIsLabel = (pTask->GetCommandID() == 0);

  CFont* pFontOld = NULL;
  COLORREF clrTextOld = pDC->GetTextColor();

  if (bIsLabel) {
    pFontOld = pDC->SelectObject(
        pTask->IsBold() ? globalData.fontBold : globalData.fontRegular);
    pDC->SetTextColor(pTask->GetTextColor() == 0 ?
        globalData.clrWindowText : pTask->GetTextColor());
  }
  else if (!pTask->IsEnabled()) {
    pDC->SetTextColor(globalData.clrGrayedText);
    pFontOld = pDC->SelectObject(globalData.fontRegular);
  }
  else if (bIsHighlighted) {
    pFontOld = pDC->SelectObject(globalData.fontUnderline);
    pDC->SetTextColor(pTask->GetTextHotColor() == 0 ?
        globalData.clrWindowText : pTask->GetTextHotColor());
  }
  else {
    pFontOld = pDC->SelectObject(globalData.fontRegular);
    pDC->SetTextColor(pTask->GetTextColor() == 0 ?
        globalData.clrWindowText : pTask->GetTextColor());
  }

  int nBkModeOld = pDC->SetBkMode(TRANSPARENT);

  CBCGPTasksPane* pTaskPane = pTask->GetTaskPane();
  ASSERT_VALID(pTaskPane);

  BOOL bMultiline = bIsLabel ?
      pTaskPane->IsWrapLabelsEnabled() : pTaskPane->IsWrapTasksEnabled();

  if (bMultiline) {
    pDC->DrawText(pTask->GetName(), rectText, DT_WORDBREAK);
  }
  else {
    CString strText = pTask->GetName();
    strText.Remove(_T('\n'));
    strText.Remove(_T('\r'));
    pDC->DrawText(strText, rectText, DT_SINGLELINE | DT_VCENTER | DT_END_ELLIPSIS);
  }

  pDC->SetBkMode(nBkModeOld);
  pDC->SelectObject(pFontOld);
  pDC->SetTextColor(clrTextOld);
}
//****
void CBCGPVisualManager::OnDrawScrollButtons(CDC* pDC, const CRect& rect, const int nBorderSize,
    int iImage, BOOL bHilited)
{
  ASSERT_VALID(pDC);
  CRect rectImage(CPoint(0, 0), CBCGPMenuImages_Size());

  CRect rectFill = rect;
  rectFill.t -= nBorderSize;

  FillRect(pDC, rectFill, globalData.brBarFace);

  if (bHilited) {
    HighlightRect(pDC->im, 0, rect);

    pDC->Draw3dRect(rect,
        globalData.clrBarHilite,
        globalData.clrBarDkShadow);
  }

  CBCGPMenuImages_Draw(pDC, (IMAGES_IDS) iImage, rect);
}
//****
void CBCGPVisualManager::OnDrawToolBoxFrame(CDC* pDC, const CRect& rect)
{
  ASSERT_VALID(pDC);
  pDC->Draw3dRect(rect, globalData.clrBarFace, globalData.clrBarFace);
}

#endif // BCGP_EXCLUDE_TASK_PANE

void CBCGPVisualManager::OnDrawSlider(CDC* pDC, CBCGPSlider* pSlider, CRect rect, BOOL bAutoHideMode)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pSlider);

  CRect rectScreen = globalData.m_rectVirtual;
  pSlider->ScreenToClient(&rectScreen);

  CRect rectFill = rect;
  rectFill.l = min(rectFill.l, rectScreen.l);

  OnFillBarBackground(pDC, pSlider, rectFill, rect);

  if (bAutoHideMode) {
    // draw outer edge;

    DWORD dwAlgn = pSlider->GetCurrentAlignment();
    CRect rectBorder = rect;

    COLORREF clrBorder = globalData.clrBarDkShadow;

    if (dwAlgn & CBRS_ALIGN_LEFT) {
      rectBorder.l = rectBorder.r;
    }
    else if (dwAlgn & CBRS_ALIGN_RIGHT) {
      rectBorder.r = rectBorder.l;
      clrBorder = globalData.clrBarHilite;
    }
    else if (dwAlgn & CBRS_ALIGN_TOP) {
      rectBorder.t = rectBorder.b;
    }
    else if (dwAlgn & CBRS_ALIGN_BOTTOM) {
      rectBorder.b = rectBorder.t;
      clrBorder = globalData.clrBarHilite;
    }
    else {
      ASSERT(FALSE);
      return;
    }

    pDC->Draw3dRect(rectBorder, clrBorder, clrBorder);
  }
}
//****
void CBCGPVisualManager::OnDrawSplitterBorder(CDC* pDC, CBCGPSplitterWnd* /*pSplitterWnd*/, CRect rect)
{
  ASSERT_VALID(pDC);

  pDC->Draw3dRect(rect, globalData.clrBarShadow, globalData.clrBarHilite);
  rect.InflateRect(-CX_BORDER, -CY_BORDER);
  pDC->Draw3dRect(rect, globalData.clrBarFace, globalData.clrBarFace);
}
//****
void CBCGPVisualManager::OnDrawSplitterBox(CDC* pDC, CBCGPSplitterWnd* /*pSplitterWnd*/, CRect& rect)
{
  ASSERT_VALID(pDC);
  pDC->Draw3dRect(rect, globalData.clrBarFace, globalData.clrBarShadow);
}
//****
void CBCGPVisualManager::OnFillSplitterBackground(CDC* pDC, CBCGPSplitterWnd* /*pSplitterWnd*/, CRect rect)
{
  ASSERT_VALID(pDC);
  pDC->FillSolidRect(rect, globalData.clrBarFace);
}
//****
void CBCGPVisualManager::OnDrawCheckBox(CDC* pDC, CRect rect,
    BOOL bHighlighted,
    BOOL bChecked,
    BOOL bEnabled)
{
  OnDrawCheckBoxEx(pDC, rect, bChecked ? 1 : 0, bHighlighted, FALSE, bEnabled);
}
//****
void CBCGPVisualManager::OnDrawCheckBoxEx(CDC* pDC, CRect rect,
    int nState,
    BOOL bHighlighted,
    BOOL /*bPressed*/,
    BOOL bEnabled)
{
  if (m_bIsDrawOnGlass) {
    rect.DeflateRect(1, 1);

    imdraw_DrawRectR(pDC->im, 0, rect,
        bEnabled ? globalData.clrWindow : globalData.clrBarFace,
        globalData.clrBarShadow);

    if (nState == 1) {
      CBCGPMenuImages_Draw(pDC, IdCheck, rect,
          ImageBlack);
    }

    return;
  }

  if (bHighlighted) {
    pDC->DrawFocusRect(rect);
  }

  rect.DeflateRect(1, 1);
  pDC->FillSolidRect(&rect, bEnabled ? globalData.clrWindow :
      globalData.clrBarFace);

  pDC->Draw3dRect(&rect,
      globalData.clrBarDkShadow,
      globalData.clrBarHilite);

  rect.DeflateRect(1, 1);
  pDC->Draw3dRect(&rect,
      globalData.clrBarShadow,
      globalData.clrBarLight);

  if (nState == 1) {
    CBCGPMenuImages_Draw(pDC, IdCheck, rect,
        ImageBlack);
  }
  else if (nState == 2) {
    rect.DeflateRect(1, 1);

    WORD HatchBits [8] = { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 };

    CBitmap bmp;
    bmp.CreateBitmap(8, 8, 1, 1, HatchBits);

    //CBrush br;
    //br.CreatePatternBrush (&bmp);

    imdraw_image_pattern(pDC->im, 0, rect, &bmp, 0);
  }
}
//****
void CBCGPVisualManager::OnDrawRadioButton(CDC* pDC, CRect rect,
    BOOL bOn,
    BOOL bHighlighted,
    BOOL /*bPressed*/,
    BOOL bEnabled)
{
  rect.DeflateRect(1, 1);

  imdraw_EllipseR(pDC->im, 0, rect,
      bEnabled ? globalData.clrBarHilite : globalData.clrBarFace,
      bHighlighted && bEnabled ? globalData.clrBarDkShadow : globalData.clrBarShadow, 1);

  if (bOn) {
    rect.DeflateRect(rect.Width() / 3, rect.Width() / 3);

    imdraw_EllipseR(pDC->im, 0, rect,
        bHighlighted && bEnabled ? globalData.clrBarDkShadow : globalData.clrBarShadow,
        0, 0);
  }
}
//****
void CBCGPVisualManager::OnDrawSpinButtons(CDC* pDC, CRect rectSpin,
    int nState, BOOL bOrientation, CBCGPSpinButtonCtrl* /*pSpinCtrl*/)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(this);

  rectSpin.DeflateRect(1, 1);

  CRect rect[2];

  rect[0] = rect[1] = rectSpin;

  if (!bOrientation) {
    rect[0].DeflateRect(0, 0, 0, rect[0].Height() / 2);
    rect[1].t = rect[0].b + 1;
  }
  else {
    rect[0].DeflateRect(0, 0, rect[0].Width() / 2, 0);
    rect[1].l = rect[0].r + 1;
  }

  if (m_bIsDrawOnGlass) {
    imdraw_DrawRectR(pDC->im, 0, rectSpin, globalData.clrBarFace, globalData.clrBarHilite);
  }
  else {
    FillRect(pDC, rectSpin, globalData.brBarFace);
    pDC->Draw3dRect(rectSpin, globalData.clrBarHilite, globalData.clrBarHilite);
  }

  IMAGES_IDS id[2][2] = {{IdArowUp, IdArowDown}, {IdArowLeft, IdArowRight}};

  int idxPressed = (nState & (SPIN_PRESSEDUP | SPIN_PRESSEDDOWN)) - 1;
  BOOL bDisabled = nState & SPIN_DISABLED;

  for (int i = 0; i < 2; i ++) {
    if (m_bIsDrawOnGlass) {
      if (idxPressed == i) {
        imdraw_DrawRectR(pDC->im, 0, rect[i], 0, globalData.clrBarShadow);
      }
      else {
        imdraw_DrawRectR(pDC->im, 0, rect[i], 0, globalData.clrBarHilite);
      }
    }
    else {
      if (idxPressed == i) {
        pDC->Draw3dRect(&rect[i], globalData.clrBarShadow, globalData.clrBarHilite);
      }
      else {
        pDC->Draw3dRect(&rect[i], globalData.clrBarHilite, globalData.clrBarShadow);
      }
    }

    CBCGPMenuImages_Draw(pDC, id[bOrientation ? 1 : 0][i], rect[i],
        bDisabled ? ImageGray : ImageBlack);
  }
}
//****
void CBCGPVisualManager::OnDrawExpandingBox(CDC* pDC, CRect rect, BOOL bIsOpened,
    COLORREF colorBox)
{
  ASSERT_VALID(pDC);

  pDC->Draw3dRect(rect, colorBox, colorBox);

  rect.DeflateRect(2, 2);

  COLOR penLine = (globalData.clrBarText);
  COLOR pOldPen = pDC->SelectObject(penLine);

  CPoint ptCenter = rect.CenterPoint();

  pDC->MoveTo(rect.l, ptCenter.y);
  pDC->LineTo(rect.r, ptCenter.y);

  if (!bIsOpened) {
    pDC->MoveTo(ptCenter.x, rect.t);
    pDC->LineTo(ptCenter.x, rect.b);
  }

  pDC->SelectObject(pOldPen);
}
//****
COLORREF CBCGPVisualManager::OnFillCalendarBarNavigationRow(CDC* pDC, CRect rect)
{
  ASSERT_VALID(pDC);

  COLOR clr = (globalData.clrInactiveCaption);
  FillRect(pDC, rect, clr);

  pDC->Draw3dRect(rect, globalData.clrInactiveBorder, globalData.clrInactiveBorder);
  return globalData.clrInactiveCaptionText;
}
//****
void CBCGPVisualManager::GetCalendarColors(const CBCGPCalendar* /*pCalendar*/,
    CBCGPCalendarColors& colors)
{
  colors.clrCaption = globalData.clrBtnFace;
  colors.clrCaptionText = globalData.clrBtnText;
  colors.clrSelected = globalData.clrBtnFace;
  colors.clrSelectedText = globalData.clrBtnText;
  colors.clrTodayBorder = _RGB(187, 85, 3);
}
//****
#if 0
void CBCGPVisualManager::OnDrawControlBorder(CWnd* pWndCtrl)
{
  ASSERT_VALID(pWndCtrl);

  CRect rect;
  pWndCtrl->GetWindowRect(rect);

  rect.b -= rect.t;
  rect.r -= rect.l;
  rect.l = rect.t = 0;

  OnDrawControlBorder(&dc, rect, pWndCtrl, m_bIsDrawOnGlass);
}
#endif
//****
void CBCGPVisualManager::OnDrawControlBorder(CDC* pDC, CRect rect, CBCGPBaseControl* pWndCtrl, BOOL bDrawOnGlass)
{
  ASSERT_VALID(pDC);

  if (bDrawOnGlass) {
    imdraw_DrawRectR(pDC->im, 0, rect, 0, globalData.clrBarShadow);
    rect.DeflateRect(1, 1);
    imdraw_DrawRectR(pDC->im, 0, rect, 0, globalData.clrWindow);
  }
  else {
    if ((pWndCtrl->GetStyle() & WS_POPUP)) {
      pDC->Draw3dRect(rect, globalData.clrBarShadow, globalData.clrBarShadow);
    }
    else {
      pDC->Draw3dRect(rect, globalData.clrBarDkShadow, globalData.clrBarHilite);
    }

    rect.DeflateRect(1, 1);
    pDC->Draw3dRect(rect, globalData.clrWindow, globalData.clrWindow);
  }
}
//****
void CBCGPVisualManager::OnDrawShowAllMenuItems(CDC* pDC, CRect rect,
    BCGBUTTON_STATE /*state*/)
{
  ASSERT_VALID(pDC);
  CBCGPMenuImages_Draw(pDC, IdArowShowAll, rect);
}
//****
int CBCGPVisualManager::GetShowAllMenuItemsHeight(CDC* /*pDC*/, const CSize& /*sizeDefault*/)
{
  return CBCGPMenuImages_Size().h + 2 * TEXT_MARGIN;
}
//****
void CBCGPVisualManager::GetTabFrameColors(const CBCGPBaseTabWnd* pTabWnd,
    COLORREF& clrDark,
    COLORREF& clrBlack,
    COLORREF& clrHighlight,
    COLORREF& clrFace,
    COLORREF& clrDarkShadow,
    COLORREF& clrLight,
    COLOR& pbrFace,
    COLOR& pbrBlack)
{
  ASSERT_VALID(pTabWnd);

  COLORREF clrActiveTab = pTabWnd->GetTabBkColor(pTabWnd->GetActiveTab());

  if (pTabWnd->IsOneNoteStyle() && clrActiveTab != 0) {
    clrFace = clrActiveTab;
  }
  else if (pTabWnd->IsDialogControl()) {
    clrFace = globalData.clrBtnFace;
  }
  else {
    clrFace = globalData.clrBarFace;
  }

  if (pTabWnd->IsDialogControl() && !pTabWnd->IsVisualManagerStyle()) {
    clrDark = globalData.clrBtnShadow;
    clrBlack = globalData.clrBtnText;
    clrHighlight = pTabWnd->IsVS2005Style() ? globalData.clrBtnShadow : globalData.clrBtnHilite;
    clrDarkShadow = globalData.clrBtnDkShadow;
    clrLight = globalData.clrBtnLight;

    pbrFace = globalData.brBtnFace;
  }
  else {
    clrDark = globalData.clrBarShadow;
    clrBlack = globalData.clrBarText;
    clrHighlight = pTabWnd->IsVS2005Style() ? globalData.clrBarShadow : globalData.clrBarHilite;
    clrDarkShadow = globalData.clrBarDkShadow;
    clrLight = globalData.clrBarLight;

    pbrFace = globalData.brBarFace;
  }

  pbrBlack = globalData.brBlack;
}
//****
void CBCGPVisualManager::OnFillAutoHideButtonBackground(CDC* pDC, CRect rect, CBCGPAutoHideButton* /*pButton*/)
{
  ASSERT_VALID(pDC);
  FillRect(pDC, rect, globalData.brBarFace);
}
//****
void CBCGPVisualManager::OnDrawAutoHideButtonBorder(CDC* pDC, CRect rectBounds, CRect rectBorderSize, CBCGPAutoHideButton* /*pButton*/)
{
  ASSERT_VALID(pDC);

  COLORREF clr = globalData.clrBarShadow;

  COLORREF clrText = pDC->GetTextColor();

  if (rectBorderSize.l > 0) {
    pDC->FillSolidRect(rectBounds.l, rectBounds.t,
        rectBounds.l + rectBorderSize.l,
        rectBounds.b, clr);
  }

  if (rectBorderSize.t > 0) {
    pDC->FillSolidRect(rectBounds.l, rectBounds.t,
        rectBounds.r,
        rectBounds.t + rectBorderSize.t, clr);
  }

  if (rectBorderSize.r > 0) {
    pDC->FillSolidRect(rectBounds.r - rectBorderSize.r, rectBounds.t,
        rectBounds.r,
        rectBounds.b, clr);
  }

  if (rectBorderSize.b > 0) {
    pDC->FillSolidRect(rectBounds.l, rectBounds.b - rectBorderSize.b,
        rectBounds.r,
        rectBounds.b, clr);
  }

  pDC->SetTextColor(clrText);
}
//****
COLORREF CBCGPVisualManager::GetAutoHideButtonTextColor(CBCGPAutoHideButton* /*pButton*/)
{
  return globalData.clrBarText;
}
//****
void CBCGPVisualManager::OnDrawOutlookBarSplitter(CDC* pDC, CRect rectSplitter)
{
  ASSERT_VALID(pDC);

  FillRect(pDC, rectSplitter, globalData.brBarFace);
  pDC->Draw3dRect(rectSplitter, globalData.clrBarHilite, globalData.clrBarShadow);
}
//****
void CBCGPVisualManager::OnFillOutlookBarCaption(CDC* pDC, CRect rectCaption,
    COLORREF& clrText)
{

  pDC->FillSolidRect(rectCaption, globalData.clrBarShadow);
  clrText = globalData.clrBarHilite;
}
//****
BOOL CBCGPVisualManager::OnDrawCalculatorButton(CDC* pDC,
    CRect rect, CBCGPToolbarButton* /*pButton*/,
    BCGBUTTON_STATE state,
    int /*cmd*/ /* CBCGPCalculator::CalculatorCommands */,
    CBCGPCalculator* /*pCalculator*/)
{
  ASSERT_VALID(pDC);

  switch (state) {
  case ButtonsIsPressed:
    FillRect(pDC, rect, globalData.brLight);
    pDC->Draw3dRect(&rect, globalData.clrBarShadow, globalData.clrBarHilite);
    break;

  case ButtonsIsHighlighted:
    FillRect(pDC, rect, globalData.brLight);

  default:
    pDC->Draw3dRect(&rect, globalData.clrBarHilite, globalData.clrBarShadow);
  }

  return TRUE;
}
//****
BOOL CBCGPVisualManager::OnDrawCalculatorDisplay(CDC* pDC, CRect rect,
    const CString& /*strText*/, BOOL /*bMem*/,
    CBCGPCalculator* /*pCalculator*/)
{
  ASSERT_VALID(pDC);

  FillRect(pDC, rect, globalData.brWindow);
  pDC->Draw3dRect(rect, globalData.clrBarShadow, globalData.clrBarHilite);

  return TRUE;
}
//****
BOOL CBCGPVisualManager::OnDrawBrowseButton(CDC* pDC, CRect rect,
    CBCGPEdit* /*pEdit*/,
    BCGBUTTON_STATE state,
    COLORREF& /*clrText*/)
{
  ASSERT_VALID(pDC);

  FillRect(pDC, &rect, globalData.brBtnFace);

  CRect rectFrame = rect;
  rectFrame.InflateRect(0, 1, 1, 1);

  pDC->Draw3dRect(rectFrame, globalData.clrBtnDkShadow, globalData.clrBtnDkShadow);

  rectFrame.DeflateRect(1, 1);
  pDC->DrawEdge(rectFrame, state == ButtonsIsPressed ? BDR_SUNKENINNER : BDR_RAISEDINNER, BF_RECT);

  return TRUE;
}
//****
void CBCGPVisualManager::OnDrawAppBarBorder(CDC* pDC, CBCGPAppBarWnd* /*pAppBarWnd*/,
    CRect rectBorder, CRect rectBorderSize)
{
  ASSERT_VALID(pDC);
  ASSERT(pDC);

  imdraw_rect_border(pDC->im, 0, rectBorder, 0, globalData.brBtnFace, rectBorderSize);

  rectBorderSize.DeflateRect(2, 2);
  rectBorder.DeflateRect(2, 2);

  imdraw_rect_border(pDC->im, 0, rectBorder, 0, globalData.brLight, rectBorderSize);

}
//****
void CBCGPVisualManager::OnDrawAppBarCaption(CDC* pDC, CBCGPAppBarWnd* /*pAppBarWnd*/,
    CRect rectCaption, CString strCaption)
{
  ASSERT_VALID(pDC);

  FillRect(pDC, rectCaption, globalData.brBarFace);

  // Paint caption text:
  int nOldMode = pDC->SetBkMode(TRANSPARENT);
  COLORREF clrOldText = pDC->SetTextColor(globalData.clrBarText);
  CFont* pOldFont = pDC->SelectObject(globalData.fontBold);
  ASSERT_VALID(pOldFont);

  CRect rectText = rectCaption;
  rectText.DeflateRect(2, 0);
  pDC->DrawText(strCaption, rectText, DT_LEFT | DT_SINGLELINE | DT_VCENTER);

  pDC->SelectObject(pOldFont);
  pDC->SetBkMode(nOldMode);
  pDC->SetTextColor(clrOldText);
}
//****
void CBCGPVisualManager::GetSmartDockingBaseMarkerColors(
    COLORREF& clrBaseGroupBackground,
    COLORREF& clrBaseGroupBorder)
{
  clrBaseGroupBackground = globalData.clrBarFace;
  clrBaseGroupBorder = globalData.clrBarShadow;
}
//****
COLORREF CBCGPVisualManager::GetSmartDockingMarkerToneColor()
{
  return globalData.clrActiveCaption;
}

#ifndef BCGP_EXCLUDE_TOOLBOX

BOOL CBCGPVisualManager::OnEraseToolBoxButton(CDC* pDC, CRect rect,
    CBCGPToolBoxButton* pButton)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pButton);

  if (pButton->GetCheck()) {
    rect.DeflateRect(1, 1);
    FillDitheredRect(pDC->im, rect);
  }

  return TRUE;
}
//****
BOOL CBCGPVisualManager::OnDrawToolBoxButtonBorder(CDC* pDC, CRect& rect,
    CBCGPToolBoxButton* pButton, UINT /*uiState*/)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pButton);

  if (pButton->GetCheck()) {
    pDC->Draw3dRect(&rect, globalData.clrBarShadow, globalData.clrBarHilite);
  }
  else if (pButton->IsHighlighted()) {
    pDC->Draw3dRect(&rect, globalData.clrBarHilite, globalData.clrBarShadow);
  }

  return TRUE;
}
//****
COLORREF CBCGPVisualManager::GetToolBoxButtonTextColor(CBCGPToolBoxButton* /*pButton*/)
{
  return globalData.clrWindowText;
}

#endif // BCGP_EXCLUDE_TOOLBOX

#ifndef BCGP_EXCLUDE_POPUP_WINDOW

void CBCGPVisualManager::OnFillPopupWindowBackground(CDC* pDC, CRect rect)
{
  ASSERT_VALID(pDC);
  FillRect(pDC, rect, globalData.brBarFace);
}
//****
void CBCGPVisualManager::OnDrawPopupWindowBorder(CDC* pDC, CRect rect)
{
  ASSERT_VALID(pDC);

  pDC->Draw3dRect(rect, globalData.clrBarLight, globalData.clrBarDkShadow);
  rect.DeflateRect(1, 1);
  pDC->Draw3dRect(rect, globalData.clrBarHilite, globalData.clrBarShadow);
}
//****
COLORREF  CBCGPVisualManager::OnDrawPopupWindowCaption(CDC* pDC, CRect rectCaption, CBCGPPopupWindow* /*pPopupWnd*/)
{
  ASSERT_VALID(pDC);

  BOOL bActive = TRUE;  // TODO

  COLOR clr = (bActive ? globalData.clrActiveCaption : globalData.clrInactiveCaption);
  FillRect(pDC, rectCaption, clr);

  // get the text color
  return bActive ? globalData.clrCaptionText : globalData.clrInactiveCaptionText;
}
//****
void CBCGPVisualManager::OnErasePopupWindowButton(CDC* pDC, CRect rect, CBCGPPopupWndButton* pButton)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pButton);

  if (pButton->IsCaptionButton()) {
    FillRect(pDC, rect, globalData.brBtnFace);
    return;
  }

#if 0
  CRect rectParent;
  pButton->GetParent()->GetClientRect(rectParent);

  pButton->GetParent()->MapWindowPoints(pButton, rectParent);
  OnFillPopupWindowBackground(pDC, rectParent);
#endif
}
//****
void CBCGPVisualManager::OnDrawPopupWindowButtonBorder(CDC* pDC, CRect rect, CBCGPPopupWndButton* pButton)
{
  if (pButton->IsPressed()) {
    pDC->Draw3dRect(rect, globalData.clrBarDkShadow, globalData.clrBarLight);
    rect.DeflateRect(1, 1);
    pDC->Draw3dRect(rect, globalData.clrBarShadow, globalData.clrBarHilite);
  }
  else {
    pDC->Draw3dRect(rect, globalData.clrBarLight, globalData.clrBarDkShadow);
    rect.DeflateRect(1, 1);
    pDC->Draw3dRect(rect, globalData.clrBarHilite, globalData.clrBarShadow);
  }
}

#endif // BCGP_EXCLUDE_POPUP_WINDOW

#ifndef BCGP_EXCLUDE_PLANNER

//****
COLORREF CBCGPVisualManager::OnFillPlannerCaption(CDC* pDC,
    CBCGPPlannerView* pView, CRect rect, BOOL bIsToday, BOOL bIsSelected,
    BOOL bMonth, BOOL bNoBorder/* = FALSE*/, BOOL /*bHorz = TRUE*/)
{
  ASSERT_VALID(pDC);

  COLORREF clrText = globalData.clrBtnText;

  //const BOOL bMonth = DYNAMIC_DOWNCAST(CBCGPPlannerViewMonth, pView) != NULL;
  if (bMonth && !bIsToday && !bIsSelected) {
    return clrText;
  }

  rect.DeflateRect(1, 1);

  if (bIsToday) {
    FillRect(pDC, rect, globalData.brBtnFace);
    FillDitheredRect(pDC->im, rect);
  }
  else if (bIsSelected) {
    FillRect(pDC, rect, globalData.brHilite);
    clrText = globalData.clrTextHilite;
  }
  else {
    FillRect(pDC, rect, globalData.brBtnFace);
  }

  if (bIsToday || !bNoBorder) {
    COLOR clr = (bIsToday ? globalData.clrBarShadow : globalData.clrBtnShadow);
    COLOR pOldPen = pDC->SelectObject(clr);

    pDC->MoveTo(rect.l, rect.b);
    pDC->LineTo(rect.r, rect.b);

    pDC->SelectObject(pOldPen);
  }

  return clrText;
}
//****
void CBCGPVisualManager::OnDrawPlannerCaptionText(CDC* pDC,
    CBCGPPlannerView* /*pView*/, CRect rect, const CString& strText,
    COLORREF clrText, int nAlign, BOOL bHighlight)
{
  const int nTextMargin = 2;

  if (bHighlight) {
    const int nTextLen = min(rect.Width(),
        pDC->GetTextExtent(strText).w + 2 * nTextMargin);

    CRect rectHighlight = rect;
    rectHighlight.DeflateRect(1, 1);

    switch (nAlign) {
    case DT_LEFT:
      rectHighlight.r = rectHighlight.l + nTextLen;
      break;

    case DT_RIGHT:
      rectHighlight.l = rectHighlight.r - nTextLen;
      break;

    case DT_CENTER:
      rectHighlight.l = rectHighlight.CenterPoint().x - nTextLen / 2;
      rectHighlight.r = rectHighlight.l + nTextLen;
      break;
    }

    FillRect(pDC, rectHighlight, globalData.brHilite);

    clrText = globalData.clrTextHilite;
  }

  rect.DeflateRect(nTextMargin, 0);

  COLORREF clrOld = pDC->SetTextColor(clrText);
  pDC->DrawText(strText, rect, DT_SINGLELINE | DT_VCENTER | nAlign);

  pDC->SetTextColor(clrOld);
}
//****
void CBCGPVisualManager::GetPlannerAppointmentColors(CBCGPPlannerView* pView,
    BOOL bSelected, BOOL bSimple, DWORD dwDrawFlags,
    COLORREF& clrBack1, COLORREF& clrBack2,
    COLORREF& clrFrame1, COLORREF& clrFrame2, COLORREF& clrText)
{
  int clr = pView->GetClass();
  ASSERT_VALID(pView);

  const BOOL bIsGradientFill =
      dwDrawFlags & BCGP_PLANNER_DRAW_APP_GRADIENT_FILL &&
      globalData.m_nBitsPerPixel > 8 &&
      !(globalData.m_bIsBlackHighContrast || globalData.m_bIsWhiteHighContrast);
  const BOOL bIsOverrideSelection = dwDrawFlags & BCGP_PLANNER_DRAW_APP_OVERRIDE_SELECTION;

  const BOOL bSelect = bSelected && !bIsOverrideSelection;

  const BOOL bDayView = clr == CLASS_PlannerViewDay;

  if (bSelect) {
    if (bDayView) {
      if (!bSimple) {
        clrBack1 = globalData.clrBtnFace;
        clrText  = globalData.clrBtnText;
      }
    }
    else {
      clrBack1 = globalData.clrHilite;
      clrText  = globalData.clrTextHilite;
    }
  }

  BOOL bDefault = FALSE;

  if (clrBack1 == CLR_DEFAULT) {
    if (bIsGradientFill) {
      clrBack1 = pView->GetPlanner()->GetBackgroundColor();

      if (clrBack1 == CLR_DEFAULT) {
        bDefault = TRUE;
        clrBack1 = m_clrPlannerWork;
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
        clrBack2 = PixelAlpha(clrBack1, _RGB(255, 255, 255), 25);
      }
    }

    clrFrame1 = PixelAlpha(clrBack1, _RGB(0, 0, 0), 75);

    if (!bSelected) {
      clrFrame2 = clrFrame1;
    }
  }
}
//****
COLORREF CBCGPVisualManager::GetPlannerAppointmentTimeColor(CBCGPPlannerView* /*pView*/,
    BOOL /*bSelected*/, BOOL /*bSimple*/, DWORD /*dwDrawFlags*/)
{
  return CLR_DEFAULT;
}
//****
COLORREF CBCGPVisualManager::GetPlannerHourLineColor(CBCGPPlannerView* /*pView*/,
    BOOL /*bWorkingHours*/, BOOL /*bHour*/)
{
  return m_clrPalennerLine;
}
//****
COLORREF CBCGPVisualManager::GetPlannerSelectionColor(CBCGPPlannerView* /*pView*/)
{
  return globalData.clrHilite;
}
//****
COLORREF CBCGPVisualManager::GetPlannerSeparatorColor(CBCGPPlannerView* /*pView*/)
{
  return globalData.clrWindowFrame;
}
//****
void CBCGPVisualManager::OnFillPlanner(CDC* pDC, CBCGPPlannerView* pView,
    CRect rect, BOOL bWorkingArea)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pView);


  COLORREF colorFill = globalData.IsHighContastMode() ?
      CLR_DEFAULT : pView->GetPlanner()->GetBackgroundColor();
  BOOL bIsDefaultColor = colorFill == CLR_DEFAULT;

  if (bIsDefaultColor) {
    colorFill = m_clrPlannerWork; // Use default color
  }

  switch (pView->GetPlanner()->GetType()) {
  case BCGP_PLANNER_TYPE_DAY:
  case BCGP_PLANNER_TYPE_WORK_WEEK: {
    COLORREF clrTextOld = pDC->SetTextColor(colorFill);

    COLORREF clrBkOld = pDC->SetBkColor(bWorkingArea ?
        _RGB(255, 255, 255) : _RGB(128, 128, 128));

    //FillRect(pDC, rect, m_brPlanner);
    {
      WORD HatchBits [8] = { 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55, 0xAA, 0x55 };

      CBitmap bmp;
      bmp.CreateBitmap(8, 8, 1, 1, HatchBits);

      imdraw_image_pattern(pDC->im, 0, rect, &bmp, 0);
    }

    pDC->SetTextColor(clrTextOld);
    pDC->SetBkColor(clrBkOld);
  }
  break;

  default:
    if (bIsDefaultColor || !bWorkingArea) {
      FillRect(pDC, rect, bWorkingArea ?
          globalData.brWindow : globalData.brBtnFace);
    }
    else {
      COLOR clr = (colorFill);
      FillRect(pDC, rect, clr);
    }
  }
}
//****
COLORREF CBCGPVisualManager::OnFillPlannerTimeBar(CDC* pDC, CBCGPPlannerView* /*pView*/,
    CRect rect, COLORREF& clrLine)
{
  ASSERT_VALID(pDC);

  FillRect(pDC, rect, globalData.brBtnFace);

  clrLine = globalData.clrBtnShadow;

  return globalData.clrBtnText;
}
//****
void CBCGPVisualManager::OnDrawPlannerTimeLine(CDC* pDC, CBCGPPlannerView* /*pView*/, CRect rect)
{
  ASSERT_VALID(pDC);

  FillRect(pDC, rect, globalData.brActiveCaption);

  COLOR pOldPen = pDC->SelectObject(globalData.penBarShadow);

  pDC->MoveTo(rect.l, rect.b);
  pDC->LineTo(rect.r, rect.b);

  pDC->SelectObject(pOldPen);
}
//****
void CBCGPVisualManager::OnFillPlannerWeekBar(CDC* pDC, CBCGPPlannerView* /*pView*/, CRect rect)
{
  ASSERT_VALID(pDC);

  FillRect(pDC, rect, globalData.brBtnFace);
}
//****
void CBCGPVisualManager::OnDrawPlannerHeader(CDC* pDC,
    CBCGPPlannerView* /*pView*/, CRect rect)
{
  ASSERT_VALID(pDC);

  FillRect(pDC, rect, globalData.brBtnFace);
  pDC->Draw3dRect(rect, globalData.clrBtnHilite, globalData.clrBtnShadow);
}
//****
void CBCGPVisualManager::OnDrawPlannerHeaderPane(CDC* pDC,
    CBCGPPlannerView* /*pView*/, CRect rect)
{
  ASSERT_VALID(pDC);
  pDC->Draw3dRect(rect, globalData.clrBtnShadow, globalData.clrBtnHilite);
}
//****
void CBCGPVisualManager::OnFillPlannerHeaderAllDay(CDC* pDC,
    CBCGPPlannerView* /*pView*/, CRect rect)
{
  ASSERT_VALID(pDC);

  COLOR clr = (globalData.clrBtnShadow);
  FillRect(pDC, rect, clr);
}
//****
void CBCGPVisualManager::OnDrawPlannerHeaderAllDayItem(CDC* pDC,
    CBCGPPlannerView* /*pView*/, CRect rect, BOOL /*bIsToday*/, BOOL bIsSelected)
{
  if (bIsSelected) {
    FillRect(pDC, rect, globalData.brWindow);
  }
}
//****
void CBCGPVisualManager::PreparePlannerBackItem(BOOL /*bIsToday*/, BOOL /*bIsSelected*/)
{
  m_bPlannerBackItemToday    = FALSE;
  m_bPlannerBackItemSelected = FALSE;
}
//****
void CBCGPVisualManager::PreparePlannerCaptionBackItem(BOOL bIsHeader)
{
  m_bPlannerCaptionBackItemHeader = bIsHeader;
}
#endif // BCGP_EXCLUDE_PLANNER

//****
void CBCGPVisualManager::DoDrawHeaderSortArrow(CDC* pDC, CRect rectArrow, BOOL bIsUp, BOOL /*bDlgCtrl*/)
{
  ASSERT_VALID(pDC);

  CBCGPMenuImages_Draw(pDC,
      bIsUp ? IdArowUpLarge : IdArowDownLarge,
      rectArrow);
}

#ifndef BCGP_EXCLUDE_GRID_CTRL

void CBCGPVisualManager::OnDrawGridSortArrow(CBCGPGridCtrl* /*pCtrl*/, CDC* pDC,
    CRect rectArrow, BOOL bIsUp)
{
  DoDrawHeaderSortArrow(pDC, rectArrow, bIsUp, TRUE/*!pCtrl->IsControlBarColors()*/);
}
//****
COLORREF CBCGPVisualManager::OnFillGridGroupByBoxBackground(CDC* pDC, CRect rect)
{
  ASSERT_VALID(pDC);

  COLOR clr = (globalData.clrBtnShadow);
  FillRect(pDC, rect, clr);

  return globalData.clrBarText;
}
//****
COLORREF CBCGPVisualManager::OnFillGridGroupByBoxTitleBackground(CDC* pDC, CRect rect)
{
  ASSERT_VALID(pDC);

  FillRect(pDC, rect, globalData.brBtnFace);
  return globalData.clrBtnShadow;
}
//****
COLORREF CBCGPVisualManager::GetGridGroupByBoxLineColor() const
{
  return globalData.clrBarText;
}
//****
void CBCGPVisualManager::OnDrawGridGroupByBoxItemBorder(CBCGPGridCtrl* /*pCtrl*/,
    CDC* pDC, CRect rect)
{
  ASSERT_VALID(pDC);
  FillRect(pDC, rect, globalData.brBtnFace);

  pDC->Draw3dRect(rect, globalData.clrBarWindow, globalData.clrBtnText);
  rect.DeflateRect(0, 0, 1, 1);
  pDC->Draw3dRect(rect, globalData.clrBarWindow, globalData.clrBtnShadow);
}
//****
COLORREF CBCGPVisualManager::GetGridLeftOffsetColor(CBCGPGridCtrl* pCtrl)
{
  ASSERT_VALID(pCtrl);

  COLORREF clrGray = 0;

  if (globalData.m_nBitsPerPixel <= 8) {
    clrGray = pCtrl->IsControlBarColors() ?
        globalData.clrBarShadow : globalData.clrBtnShadow;
  }
  else {
    clrGray = PixelAlpha(
        pCtrl->IsControlBarColors() ? globalData.clrBarFace : globalData.clrBtnFace, 94);
  }

  return clrGray;
}
//****
COLORREF CBCGPVisualManager::GetGridItemSortedColor(CBCGPGridCtrl* pCtrl)
{
  ASSERT_VALID(pCtrl);

  COLORREF clrSortedColumn = 0;

  if (globalData.m_nBitsPerPixel <= 8) {
    clrSortedColumn = pCtrl->GetBkColor();
  }
  else {
    clrSortedColumn = PixelAlpha(
        pCtrl->GetBkColor(), .97, .97, .97);
  }

  return clrSortedColumn;
}
//****
void CBCGPVisualManager::OnFillGridGroupBackground(CBCGPGridCtrl* /*pCtrl*/, CDC* pDC,
    CRect rectFill)
{
  ASSERT_VALID(pDC);
  FillRect(pDC, rectFill, globalData.brWindow);
}
//****
void CBCGPVisualManager::OnDrawGridGroupUnderline(CBCGPGridCtrl* pCtrl, CDC* pDC,
    CRect rectFill)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pCtrl);

  COLORREF clrOld = pDC->GetBkColor();
  pDC->FillSolidRect(rectFill,
      pCtrl->IsControlBarColors() ? globalData.clrBarShadow : globalData.clrBtnShadow);
  pDC->SetBkColor(clrOld);
}
//****
COLORREF CBCGPVisualManager::OnFillGridRowBackground(CBCGPGridCtrl* pCtrl,
    CDC* pDC, CRect rectFill, BOOL bSelected)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pCtrl);

  // Fill area:
  if (!pCtrl->IsFocused()) {
    FillRect(pDC, rectFill,
        pCtrl->IsControlBarColors() ? globalData.brBarFace : globalData.brBtnFace);
  }
  else {
    FillRect(pDC, rectFill, globalData.brHilite);
  }

  // Return text color:
  if (!pCtrl->IsHighlightGroups() && bSelected) {
    return (!pCtrl->IsFocused()) ? globalData.clrHotText : globalData.clrTextHilite;
  }

  return pCtrl->IsHighlightGroups() ?
      (pCtrl->IsControlBarColors() ? globalData.clrBarShadow : globalData.clrBtnShadow) :
        globalData.clrWindowText;
}
//****
COLORREF CBCGPVisualManager::OnFillGridItem(CBCGPGridCtrl* pCtrl,
    CDC* pDC, CRect rectFill,
    BOOL bSelected, BOOL bActiveItem, BOOL bSortedColumn)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pCtrl);

  // Fill area:
  if (bSelected && !bActiveItem) {
    if (!pCtrl->IsFocused()) {
      FillRect(pDC, rectFill,
          pCtrl->IsControlBarColors() ? globalData.brBarFace : globalData.brBtnFace);
      return globalData.clrBtnText;
    }
    else {
      FillRect(pDC, rectFill, globalData.brHilite);
      return globalData.clrTextHilite;
    }
  }
  else {
    if (bActiveItem) {
      FillRect(pDC, rectFill, globalData.brWindow);
    }
    else if (bSortedColumn) {
      COLOR clr = (pCtrl->GetSortedColor());
      FillRect(pDC, rectFill, clr);
    }
    else {
      // no painting
    }
  }

  return 0;
}
//****
void CBCGPVisualManager::OnDrawGridHeaderMenuButton(CBCGPGridCtrl* /*pCtrl*/, CDC* pDC, CRect rect,
    BOOL bHighlighted, BOOL /*bPressed*/, BOOL /*bDisabled*/)
{
  ASSERT_VALID(pDC);

  rect.DeflateRect(1, 1);

  if (bHighlighted) {
    pDC->Draw3dRect(rect, globalData.clrBtnHilite, globalData.clrBtnDkShadow);
  }
}
//****
void CBCGPVisualManager::OnDrawGridSelectionBorder(CBCGPGridCtrl* /*pCtrl*/, CDC* pDC, CRect rect)
{
  ASSERT_VALID(pDC);

  pDC->Draw3dRect(rect, globalData.clrBtnDkShadow, globalData.clrBtnDkShadow);
  rect.DeflateRect(1, 1);
  pDC->Draw3dRect(rect, globalData.clrBtnDkShadow, globalData.clrBtnDkShadow);
}
//****
BOOL CBCGPVisualManager::OnSetGridColorTheme(CBCGPGridCtrl* /*pCtrl*/, BCGP_GRID_COLOR_DATA& /*theme*/)
{
  return TRUE;
}
//****
COLORREF CBCGPVisualManager::OnFillReportCtrlRowBackground(CBCGPGridCtrl* pCtrl,
    CDC* pDC, CRect rectFill,
    BOOL bSelected, BOOL bGroup)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pCtrl);

  // Fill area:
  COLORREF clrText = 0;

  if (bSelected) {
    if (!pCtrl->IsFocused()) {
      FillRect(pDC, rectFill,
          pCtrl->IsControlBarColors() ? globalData.brBarFace : globalData.brBtnFace);

      clrText = m_clrReportGroupText;
    }
    else {
      FillRect(pDC, rectFill, globalData.brHilite);
      clrText = globalData.clrTextHilite;
    }
  }
  else {
    if (bGroup) {
      // no painting
      clrText = m_clrReportGroupText;
    }
  }

  // Return text color:
  return clrText;
}
//****
COLORREF CBCGPVisualManager::GetReportCtrlGroupBackgoundColor()
{
  return globalData.clrBtnLight;
}
//****
void CBCGPVisualManager::OnFillGridBackground(CDC* pDC, CRect rect)
{
  ASSERT_VALID(pDC);

  FillRect(pDC, rect, globalData.brWindow);
}
//****
void CBCGPVisualManager::OnDrawGridExpandingBox(CDC* pDC, CRect rect, BOOL bIsOpened, COLORREF colorBox)
{
  OnDrawExpandingBox(pDC, rect, bIsOpened, colorBox);
}
//****
void CBCGPVisualManager::OnFillGridHeaderBackground(CBCGPGridCtrl* /*pCtrl*/, CDC* pDC, CRect rect)
{
  ASSERT_VALID(pDC);

  FillRect(pDC, rect, globalData.brBtnFace);
}
//****
BOOL CBCGPVisualManager::OnDrawGridHeaderItemBorder(CBCGPGridCtrl* /*pCtrl*/, CDC* pDC, CRect rect, BOOL /*bPressed*/)
{
  ASSERT_VALID(pDC);

  pDC->Draw3dRect(rect, globalData.clrBtnHilite, globalData.clrBtnDkShadow);
  return TRUE;
}
//****
void CBCGPVisualManager::OnFillGridRowHeaderBackground(CBCGPGridCtrl* /*pCtrl*/, CDC* pDC, CRect rect)
{
  ASSERT_VALID(pDC);

  FillRect(pDC, rect, globalData.brBtnFace);
}
//****
BOOL CBCGPVisualManager::OnDrawGridRowHeaderItemBorder(CBCGPGridCtrl* /*pCtrl*/, CDC* pDC, CRect rect, BOOL /*bPressed*/)
{
  ASSERT_VALID(pDC);

  pDC->Draw3dRect(rect, globalData.clrBtnHilite, globalData.clrBtnDkShadow);
  return TRUE;
}
//****
void CBCGPVisualManager::OnFillGridSelectAllAreaBackground(CBCGPGridCtrl* /*pCtrl*/, CDC* pDC, CRect rect, BOOL /*bPressed*/)
{
  ASSERT_VALID(pDC);

  FillRect(pDC, rect, globalData.brBtnFace);
}
//****
BOOL CBCGPVisualManager::OnDrawGridSelectAllAreaBorder(CBCGPGridCtrl* /*pCtrl*/, CDC* pDC, CRect rect, BOOL /*bPressed*/)
{
  ASSERT_VALID(pDC);

  pDC->Draw3dRect(rect, globalData.clrBtnHilite, globalData.clrBtnDkShadow);
  return TRUE;
}

#endif // BCGP_EXCLUDE_GRID_CTRL

//****

#if !defined (BCGP_EXCLUDE_GRID_CTRL) && !defined (BCGP_EXCLUDE_GANTT)

void CBCGPGanttChart_PrepareColorScheme(COLORREF clrMain, BCGP_GANTT_CHART_COLORS& colors)  // static
{
  double H, S, L;
  RGBtoHSL(clrMain, &H, &S, &L);
  double L1 = max(L, 0.85);
  clrMain = HLStoRGB_ONE(H, L1, S);

  colors.clrRowBackground  = clrMain;
  colors.clrRowDayOff      = HLStoRGB_ONE(H, L1 - 0.05, S);
  colors.clrBarFill        = HLStoRGB_ONE(H, L1 - 0.50, S);
  colors.clrBarComplete    = HLStoRGB_ONE(H, L1 - 0.20, S);
  colors.clrConnectorLines = MixColors(clrMain, _RGB(0, 0, 0),       0.85f);
  colors.clrGridLine0      = MixColors(clrMain, _RGB(192, 192, 192), 0.75f);
  colors.clrGridLine1      = MixColors(clrMain, _RGB(64, 64, 64),    0.75f);
  colors.clrShadows        = MixColors(clrMain, _RGB(32, 32, 32),    0.8f);
}

//****
void CBCGPVisualManager::GetGanttColors(const CBCGPGanttChart* /*pChart*/, BCGP_GANTT_CHART_COLORS& colors, COLORREF clrBack) const
{
  if (clrBack == CLR_DEFAULT) {
    clrBack = globalData.clrWindow;
  }

  CBCGPGanttChart_PrepareColorScheme(clrBack, colors);

  colors.clrBackground      = globalData.clrWindow;
  colors.clrShadows         = m_clrMenuShadowBase;

  colors.clrRowBackground   = colors.clrBackground;
  colors.clrGridLine0       = globalData.clrBarShadow;
  colors.clrGridLine1       = globalData.clrBarLight;
  colors.clrSelection       = globalData.clrHilite;
  colors.clrSelectionBorder = globalData.clrHilite;

  colors.clrBarFill         = _RGB(0, 0, 255);
  colors.clrBarComplete     = _RGB(0, 255, 0);
}
//****
void CBCGPVisualManager::DrawGanttChartBackground(const CBCGPGanttChart*, CDC& dc, const CRect& rectChart, COLORREF clrFill)
{
  dc.FillSolidRect(rectChart, clrFill);
}
//****
void CBCGPVisualManager::DrawGanttItemBackgroundCell(const CBCGPGanttChart*, CDC& dc, const CRect& /*rectItem*/, const CRect& rectClip, const BCGP_GANTT_CHART_COLORS& colors, BOOL bDayOff)
{
  dc.FillSolidRect(rectClip, (bDayOff) ? colors.clrRowDayOff : colors.clrRowBackground);
}
//****
void CBCGPVisualManager::DrawGanttHeaderCell(const CBCGPGanttChart*, CDC& dc, const BCGP_GANTT_CHART_HEADER_CELL_INFO& cellInfo, BOOL /*bHilite*/)
{
  dc.FillSolidRect(cellInfo.rectCell, globalData.clrBarFace);
  dc.Draw3dRect(cellInfo.rectCell, globalData.clrBtnHilite, globalData.clrBtnDkShadow);
}
//****
void CBCGPVisualManager::DrawGanttHeaderText(const CBCGPGanttChart*, CDC& dc, const BCGP_GANTT_CHART_HEADER_CELL_INFO& cellInfo, const CString& sCellText, BOOL bHilite) const
{
  CRect rcCell = cellInfo.rectCell;
  rcCell.DeflateRect(1, 2, 2, 2);  // text padding

  CRect rcVisible;
  rcVisible.IntersectRect(&cellInfo.rectClip, rcCell);

  if (rcVisible.IsRectEmpty()) {
    return;
  }

  CFont* pOldFont = dc.SelectObject(globalData.fontRegular);

  dc.SetBkMode(TRANSPARENT);
  dc.SetTextColor(GetGanttHeaderTextColor(bHilite));

  ASSERT(cellInfo.pHeaderInfo != NULL);

  DWORD dwFlags = DT_VCENTER | DT_NOPREFIX | cellInfo.pHeaderInfo->dwAlignment;
  dc.DrawText(sCellText, rcCell, dwFlags);

  dc.SelectObject(pOldFont);
}
//****
COLORREF CBCGPVisualManager::GetGanttHeaderTextColor(BOOL bHilite) const
{
  return bHilite ? globalData.clrHotText : globalData.clrWindowText;
}
//****
void CBCGPVisualManager::FillGanttBar(const CBCGPGanttItem* /*pItem*/, CDC& dc, const CRect& rectFill, COLORREF color, double /*dGlowLine*/)
{
  dc.FillSolidRect(rectFill, color);
}
//****

#endif // !defined (BCGP_EXCLUDE_GRID_CTRL) && !defined (BCGP_EXCLUDE_GANTT)

//****

#ifndef BCGP_EXCLUDE_PROP_LIST

COLORREF CBCGPVisualManager::GetPropListGroupColor(CBCGPPropList* pPropList)
{
  ASSERT_VALID(pPropList);

  return pPropList->DrawControlBarColors() ?
      globalData.clrBarFace : globalData.clrBtnFace;
}
//****
COLORREF CBCGPVisualManager::GetPropListGroupTextColor(CBCGPPropList* pPropList)
{
  ASSERT_VALID(pPropList);

  return pPropList->DrawControlBarColors() ?
      globalData.clrBarDkShadow : globalData.clrBtnDkShadow;
}
//****
COLORREF CBCGPVisualManager::GetPropListCommandTextColor(CBCGPPropList* /*pPropList*/)
{
  return globalData.clrHotLinkText;
}

#endif // BCGP_EXCLUDE_PROP_LIST

COLORREF CBCGPVisualManager::GetMenuItemTextColor(
    CBCGPToolbarMenuButton* /*pButton*/, BOOL bHighlighted, BOOL bDisabled)
{
  if (bHighlighted) {
    return bDisabled ? globalData.clrBtnFace : globalData.clrTextHilite;
  }

  return bDisabled ? globalData.clrGrayedText : globalData.clrWindowText;
}
//****
int CBCGPVisualManager::GetMenuDownArrowState(CBCGPToolbarMenuButton* /*pButton*/, BOOL /*bHightlight*/, BOOL /*bPressed*/, BOOL /*bDisabled*/)
{
  return (int) ImageBlack;
}
//****
COLORREF CBCGPVisualManager::GetStatusBarPaneTextColor(CBCGPStatusBar* /*pStatusBar*/,
    CBCGStatusBarPaneInfo* pPane)
{
  ASSERT(pPane != NULL);

  return (pPane->GetStyle() & SBPS_DISABLED) ? globalData.clrGrayedText :
      pPane->GetTextColor() == 0 ? globalData.clrBtnText : pPane->GetTextColor();
}

#ifndef BCGP_EXCLUDE_RIBBON

void CBCGPVisualManager::OnDrawRibbonCaption(CDC* pDC, CBCGPRibbonBar* pBar,
    CRect rect, CRect rectText)
{
  ASSERT_VALID(pBar);

  const BOOL bGlass    = pBar->IsTransparentCaption();
  const DWORD dwStyleEx  = pBar->GetExStyle();
  const BOOL bIsRTL      = (dwStyleEx & WS_EX_LAYOUTRTL) == WS_EX_LAYOUTRTL;
  BOOL bTextCenter = TRUE;

  ASSERT_VALID(pDC);

  CRect rectQAT = pBar->GetQuickAccessToolbarLocation();
  BOOL bHide  = (pBar->GetHideFlags() & BCGPRIBBONBAR_HIDE_ALL) != 0;
  BOOL bExtra = !bHide && pBar->IsQuickAccessToolbarOnTop() &&
      rectQAT.l < rectQAT.r && !pBar->IsQATEmpty();

  if ((bHide && !bExtra) || pBar->IsScenicLook()) {
    CBitmap* hIcon = pBar->GetWndIcon();

    if (hIcon != NULL) {
      CSize szIcon(::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));

      long x = rect.l + 2;
      long y = rect.t  + max(0, (rect.Height() - szIcon.h) / 2);

      if (bGlass) {
        globalData.DrawIconOnGlass(0, pDC, hIcon, CRect(x, y, x + szIcon.w, y + szIcon.h));
      }
      else {
        ::DrawIconEx(pDC, x, y, hIcon, szIcon.w, szIcon.h,
            0, NULL, DI_NORMAL);
      }

      if (rectText.l < (x + szIcon.w + 4)) {
        rectText.l = x + szIcon.w + 4;
      }

    }

    bTextCenter = TRUE;
  }

  CFont* pOldFont = pDC->SelectObject(globalData.fontBold);
  ASSERT(pOldFont != NULL);

  int nOldMode = pDC->SetBkMode(TRANSPARENT);

  CString strCaption;
  pBar->GetWindowText(strCaption);

  DWORD dwTextStyle = DT_END_ELLIPSIS | DT_SINGLELINE | DT_VCENTER |
      (bIsRTL ? DT_RTLREADING | DT_RIGHT : 0);

  COLORREF clrText = _RGB(0, 0, 0);

  int widthFull = rectText.Width();
  int width = pDC->GetTextExtent(strCaption).w;

  if (bTextCenter && width < widthFull) {
    rectText.l += (widthFull - width) / 2;
  }

  rectText.r = min(rectText.l + width, rectText.r);

  if (rectText.r > rectText.l) {
    if (bGlass) {
      DrawTextOnGlass(pDC, strCaption, rectText, dwTextStyle, 10);
    }
    else {
      COLORREF clrOldText = pDC->SetTextColor(clrText);
      pDC->DrawText(strCaption, rectText, dwTextStyle);
      pDC->SetTextColor(clrOldText);
    }
  }

  pDC->SetBkMode(nOldMode);
  pDC->SelectObject(pOldFont);
}
//****
void CBCGPVisualManager::OnDrawRibbonCaptionButton(
    CDC* pDC, CBCGPRibbonCaptionButton* pButton)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pButton);

  OnFillRibbonButton(pDC, pButton);

  IMAGES_IDS imageID;

  switch (pButton->GetID()) {
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

  CBCGPMenuImages_Draw(pDC, imageID, pButton->GetRect(),
      pButton->IsDisabled() ?
      ImageGray : ImageBlack);

  OnDrawRibbonButtonBorder(pDC, pButton);
}
//****
COLORREF CBCGPVisualManager::OnDrawRibbonButtonsGroup(
    CDC* /*pDC*/, CBCGPRibbonButtonsGroup* /*pGroup*/,
    CRect /*rectGroup*/)
{
  return 0;
}
//****
void CBCGPVisualManager::OnDrawDefaultRibbonImage(
    CDC* pDC, CRect rectImage,
    BOOL bIsDisabled,
    BOOL /*bIsPressed*/,
    BOOL /*bIsHighlighted*/)
{
  ASSERT_VALID(pDC);

  CRect rectBullet(rectImage.CenterPoint(), CSize(1, 1));
  rectBullet.InflateRect(5, 5);

  if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode()) {
    COLOR clr = (bIsDisabled ? globalData.clrGrayedText : _RGB(0, 127, 0));

    pDC->Ellipse(rectBullet, clr);
  }
  else {
    imdraw_EllipseR(pDC->im, 0, rectBullet,
        bIsDisabled ? globalData.clrGrayedText : _RGB(160, 208, 128),
        bIsDisabled ? globalData.clrBtnShadow : _RGB(71, 117, 44), 1);
  }
}
//****
void CBCGPVisualManager::OnDrawRibbonMainButton(
    CDC* pDC,
    CBCGPRibbonButton* pButton)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pButton);

  const BOOL bIsHighlighted = pButton->IsHighlighted() || pButton->IsFocused();
  const BOOL bIsPressed = pButton->IsPressed() || pButton->IsDroppedDown();

  CRect rect = pButton->GetRect();
  rect.DeflateRect(2, 2);

  imdraw_EllipseR(pDC->im, 0, rect,
      bIsPressed ? globalData.clrBarLight : globalData.clrBarFace,
      bIsHighlighted ? globalData.clrBarDkShadow : globalData.clrBarShadow, 1);
}
//****
COLORREF CBCGPVisualManager::OnDrawRibbonTabsFrame(
    CDC* pDC,
    CBCGPRibbonBar* /*pWndRibbonBar*/,
    CRect rectTab)
{
  ASSERT_VALID(pDC);

  COLOR pen = (globalData.clrBarShadow);
  COLOR pOldPen = pDC->SelectObject(pen);
  ASSERT(pOldPen != NULL);

  pDC->MoveTo(rectTab.l, rectTab.t);
  pDC->LineTo(rectTab.r, rectTab.t);

  pDC->SelectObject(pOldPen);

  return 0;
}
//****
void CBCGPVisualManager::OnDrawRibbonCategory(
    CDC* pDC,
    CBCGPRibbonCategory* pCategory,
    CRect rectCategory)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pCategory);

  const int nShadowSize = 2;

  rectCategory.r -= nShadowSize;
  rectCategory.b -= nShadowSize;

  FillRect(pDC, rectCategory, globalData.brBarFace);

  CRect rectActiveTab = pCategory->GetTabRect();

  COLOR pen = (globalData.clrBarShadow);
  COLOR pOldPen = pDC->SelectObject(pen);
  ASSERT(pOldPen != NULL);

  pDC->MoveTo(rectCategory.l, rectCategory.t);
  pDC->LineTo(rectActiveTab.l + 1, rectCategory.t);

  pDC->MoveTo(rectActiveTab.r - 2, rectCategory.t);
  pDC->LineTo(rectCategory.r, rectCategory.t);
  pDC->LineTo(rectCategory.r, rectCategory.b);
  pDC->LineTo(rectCategory.l, rectCategory.b);
  pDC->LineTo(rectCategory.l, rectCategory.t);

  pDC->SelectObject(pOldPen);

  DrawShadow(pDC->im, 0, rectCategory, nShadowSize, 100, 75, NULL, NULL,
      m_clrMenuShadowBase);
}
//****
void CBCGPVisualManager::OnDrawRibbonCategoryScroll(
    CDC* pDC,
    CBCGPRibbonCategoryScroll* pScroll)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pScroll);

  CRect rect = pScroll->GetRect();
  rect.b--;

  FillRect(pDC, rect, globalData.brBarFace);

  if (pScroll->IsHighlighted()) {
    HighlightRect(pDC->im, 0, rect);
  }

  BOOL bIsLeft = pScroll->IsLeftScroll();

  if (globalData.m_bIsRTL) {
    bIsLeft = !bIsLeft;
  }

  CBCGPMenuImages_Draw(pDC,
      bIsLeft ? IdArowLeftLarge : IdArowRightLarge,
      rect);

  pDC->Draw3dRect(rect, globalData.clrBarShadow, globalData.clrBarShadow);
}
//****
COLORREF CBCGPVisualManager::OnDrawRibbonCategoryTab(
    CDC* pDC,
    CBCGPRibbonTab* pTab,
    BOOL bIsActive)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pTab);

  CBCGPRibbonCategory* pCategory = pTab->GetParentCategory();
  ASSERT_VALID(pCategory);
  CBCGPRibbonBar* pBar = pCategory->GetParentRibbonBar();
  ASSERT_VALID(pBar);

  bIsActive = bIsActive &&
      ((pBar->GetHideFlags() & BCGPRIBBONBAR_HIDE_ELEMENTS) == 0 || pTab->GetDroppedDown() != NULL);

  const BOOL bIsFocused = pTab->IsFocused() && (pBar->GetHideFlags() & BCGPRIBBONBAR_HIDE_ELEMENTS);
  const BOOL bIsHighlighted = (pTab->IsHighlighted() || bIsFocused) && !pTab->IsDroppedDown();

  COLOR pen = (globalData.clrBarShadow);
  COLOR pOldPen = pDC->SelectObject(pen);
  ASSERT(pOldPen != NULL);

  CRect rectTab = pTab->GetRect();

  rectTab.t += 3;

  const int nTrancateRatio =
      pTab->GetParentCategory()->GetParentRibbonBar()->GetTabTrancateRatio();

  if (nTrancateRatio > 0) {
    const int nPercent = max(10, 100 - nTrancateRatio / 2);

    COLORREF color = PixelAlpha(globalData.clrBarFace, nPercent);
    COLOR pOldPen = pDC->SelectObject(color);

    pDC->MoveTo(rectTab.r - 1, rectTab.t);
    pDC->LineTo(rectTab.r - 1, rectTab.b);

    pDC->SelectObject(pOldPen);
  }

  if (!bIsActive && !bIsHighlighted) {
    return globalData.clrBarText;
  }

  rectTab.r -= 2;

#define POINTS_NUM  8
  IPOINT pts [POINTS_NUM];

  pts [0] = CPoint(rectTab.l, rectTab.b);
  pts [1] = CPoint(rectTab.l + 1, rectTab.b - 1);
  pts [2] = CPoint(rectTab.l + 1, rectTab.t + 2);
  pts [3] = CPoint(rectTab.l + 3, rectTab.t);
  pts [4] = CPoint(rectTab.r - 3, rectTab.t);
  pts [5] = CPoint(rectTab.r - 1, rectTab.t + 2);
  pts [6] = CPoint(rectTab.r - 1, rectTab.b - 1);
  pts [7] = CPoint(rectTab.r, rectTab.b);

  if (bIsActive) {
    COLORREF clrFill = pTab->IsSelected() ? globalData.clrBarHilite :
        RibbonCategoryColorToRGB(pTab->GetParentCategory()->GetTabColor());

    if (clrFill == 0) {
      clrFill = bIsHighlighted ? globalData.brWindow : globalData.brBarFace;
    }

    imdraw_poly(pDC->im, &rectTab, pts, POINTS_NUM, 1, clrFill, 0, 0);
  }

  pDC->Polyline(pts, POINTS_NUM);

  pDC->SelectObject(pOldPen);

  return globalData.clrBarText;
}
//****
COLORREF CBCGPVisualManager::OnDrawRibbonPanel(
    CDC* pDC,
    CBCGPRibbonPanel* pPanel,
    CRect rectPanel,
    CRect /*rectCaption*/)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pPanel);

  COLORREF clrText = globalData.clrBarText;

  if (pPanel->IsCollapsed() && pPanel->GetDefaultButton()->IsFocused()) {
    FillRect(pDC, rectPanel, globalData.brHilite);
    clrText = globalData.clrTextHilite;
  }
  else if (pPanel->IsHighlighted()) {
    HighlightRect(pDC->im, 0, rectPanel);
  }

  pDC->Draw3dRect(rectPanel, globalData.clrBarHilite, globalData.clrBarHilite);
  rectPanel.OffsetRect(-1, -1);
  pDC->Draw3dRect(rectPanel, globalData.clrBarShadow, globalData.clrBarShadow);

  return clrText;
}
//****
void CBCGPVisualManager::OnDrawRibbonPanelCaption(
    CDC* pDC,
    CBCGPRibbonPanel* pPanel,
    CRect rectCaption)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pPanel);

  rectCaption.DeflateRect(1, 1);
  rectCaption.r -= 2;

  COLORREF clrText = OnFillRibbonPanelCaption(pDC, pPanel, rectCaption);
  COLORREF clrTextOld = pDC->SetTextColor(clrText);

  CString str = pPanel->GetName();

  if (pPanel->GetLaunchButton()->GetID() > 0) {
    rectCaption.r = pPanel->GetLaunchButton()->GetRect().l;
  }

  pDC->DrawText(str, rectCaption,
      DT_SINGLELINE | DT_CENTER | DT_VCENTER | DT_END_ELLIPSIS | DT_NOPREFIX);

  pDC->SetTextColor(clrTextOld);
}
//****
COLORREF CBCGPVisualManager::OnFillRibbonPanelCaption(CDC* pDC, CBCGPRibbonPanel* pPanel, CRect rectCaption)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pPanel);

  COLOR clr = (pPanel->IsHighlighted() ?
      globalData.clrActiveCaption : globalData.clrInactiveCaption);
  FillRect(pDC, rectCaption, clr);

  return pPanel->IsHighlighted() ? globalData.clrCaptionText : globalData.clrInactiveCaptionText;
}
//****
void CBCGPVisualManager::OnDrawRibbonLaunchButton(
    CDC* pDC,
    CBCGPRibbonLaunchButton* pButton,
    CBCGPRibbonPanel* pPanel)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pButton);
  ASSERT_VALID(pPanel);

  OnFillRibbonButton(pDC, pButton);

  COLORREF clrText = pPanel->IsHighlighted() ?
      globalData.clrCaptionText : globalData.clrInactiveCaptionText;

  IMAGE_STATE imageState = ImageBlack;

  if (pButton->IsDisabled()) {
    imageState = ImageGray;
  }
  else if (!pButton->IsHighlighted()) {
    if (GetRV(clrText) > 192 &&
        GetGV(clrText) > 192 &&
        GetBV(clrText) > 192) {
      imageState = ImageWhite;
    }
    else {
      imageState = ImageBlack;
    }
  }

  CBCGPMenuImages_Draw(pDC, IdLaunchArrow,
      pButton->GetRect(), imageState);

  OnDrawRibbonButtonBorder(pDC, pButton);
}
//****
void CBCGPVisualManager::OnDrawRibbonDefaultPaneButton(
    CDC* pDC,
    CBCGPRibbonButton* pButton)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pButton);

  if (pButton->IsQATMode()) {
    OnFillRibbonButton(pDC, pButton);
    OnDrawRibbonDefaultPaneButtonContext(pDC, pButton);
    OnDrawRibbonButtonBorder(pDC, pButton);
  }
  else {
    OnDrawRibbonDefaultPaneButtonContext(pDC, pButton);
  }
}
//****
void CBCGPVisualManager::OnDrawRibbonDefaultPaneButtonContext(
    CDC* pDC,
    CBCGPRibbonButton* pButton)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pButton);

  CRect rectMenuArrow = pButton->GetRect();;

  if (pButton->IsQATMode()) {
    pButton->DrawImage(pDC, RibbonImageSmall, pButton->GetRect());
    return;
  }

  CRect rectImage = pButton->GetRect();
  rectImage.t += 10;
  rectImage.b = rectImage.t + pButton->GetImageSize(RibbonImageSmall).h;

  pButton->DrawImage(pDC, RibbonImageSmall, rectImage);

  // Draw text:
  pButton->DrawBottomText(pDC, FALSE);
}
//****
void CBCGPVisualManager::OnDrawRibbonDefaultPaneButtonIndicator(
    CDC* pDC,
    CBCGPRibbonButton* /*pButton*/,
    CRect rect,
    BOOL /*bIsSelected*/,
    BOOL /*bHighlighted*/)
{
  ASSERT_VALID(pDC);

  rect.l = rect.r - rect.Height();
  rect.DeflateRect(1, 1);

  FillRect(pDC, rect, globalData.brBarFace);
  pDC->Draw3dRect(rect, globalData.clrBarShadow, globalData.clrBarShadow);

  CRect rectWhite = rect;
  rectWhite.OffsetRect(0, 1);

  CBCGPMenuImages_Draw(pDC, IdArowDown, rectWhite,
      ImageWhite);

  CBCGPMenuImages_Draw(pDC, IdArowDown, rect,
      ImageBlack);
}
//****
COLORREF CBCGPVisualManager::OnFillRibbonButton(
    CDC* pDC,
    CBCGPRibbonButton* pButton)
{
  int cls = pButton->GetClass();
  ASSERT_VALID(pDC);
  ASSERT_VALID(pButton);

  if (CLASS_RibbonEdit == cls) {
    COLORREF clrBorder = globalData.clrBarShadow;
    CRect rectCommand = pButton->GetCommandRect();

    CRect rect = pButton->GetRect();
    rect.l = rectCommand.l;

    if (m_bIsDrawOnGlass) {
      imdraw_DrawRectR(pDC->im, 0, rect, globalData.clrWindow, clrBorder);
    }
    else {
      if (pButton->IsDroppedDown() || pButton->IsHighlighted()) {
        FillRect(pDC, rectCommand, globalData.brWindow);
      }
      else {
        HighlightRect(pDC->im, 0, rectCommand);
      }

      pDC->Draw3dRect(rect, clrBorder, clrBorder);
    }

    return 0;
  }

  if (pButton->IsMenuMode() && !pButton->IsPaletteIcon()) {
    if (pButton->IsHighlighted()) {
      FillRect(pDC, &pButton->GetRect(), globalData.brHilite);
      return globalData.clrTextHilite;
    }
  }
  else {
    if (pButton->IsChecked() && !pButton->IsHighlighted()) {
      if (m_bIsDrawOnGlass) {
        imdraw_DrawRectR(pDC->im, 0, pButton->GetRect(), globalData.clrWindow, 0);
      }
      else {
        FillDitheredRect(pDC->im, pButton->GetRect());
      }
    }
  }

  return 0;
}
//****
COLORREF CBCGPVisualManager::OnFillRibbonMainPanelButton(
    CDC* pDC,
    CBCGPRibbonButton* pButton)
{
  return OnFillRibbonButton(pDC, pButton);
}
//****
void CBCGPVisualManager::OnDrawRibbonMainPanelButtonBorder(
    CDC* pDC, CBCGPRibbonButton* pButton)
{
  OnDrawRibbonButtonBorder(pDC, pButton);
}
//****
void CBCGPVisualManager::OnFillRibbonEdit(
    CDC* pDC,
    CBCGPRibbonEditCtrl* /*pEdit*/,
    CRect rect,
    BOOL bIsHighlighted,
    BOOL /*bIsPaneHighlighted*/,
    BOOL bIsDisabled,
    COLORREF& /*clrText*/, COLORREF& /*clrSelBackground*/, COLORREF& /*clrSelText*/)
{
  ASSERT_VALID(pDC);

  if (bIsHighlighted && !bIsDisabled) {
    if (m_bIsDrawOnGlass) {
      imdraw_DrawRectR(pDC->im, 0, rect, globalData.clrWindow, 0);
    }
    else {
      FillRect(pDC, rect, globalData.brWindow);
    }
  }
  else {
    if (m_bIsDrawOnGlass) {
      imdraw_DrawRectR(pDC->im, 0, rect, globalData.clrBarFace, 0);
    }
    else {
      FillRect(pDC, rect, globalData.brBarFace);
      HighlightRect(pDC->im, 0, rect);
    }
  }
}
//****
COLORREF CBCGPVisualManager::GetRibbonEditBackgroundColor(
    CBCGPRibbonEditCtrl* /*pEdit*/,
    BOOL bIsHighlighted,
    BOOL /*bIsPaneHighlighted*/,
    BOOL bIsDisabled)
{
  return (bIsHighlighted && !bIsDisabled) ? globalData.clrWindow : globalData.clrBarFace;
}
//****
void CBCGPVisualManager::OnDrawRibbonButtonBorder(
    CDC* pDC,
    CBCGPRibbonButton* pButton)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pButton);

  if (CLASS_RibbonEdit == pButton->GetClass()) {
    return;
  }

  CRect rect = pButton->GetRect();

  if (pButton->IsMenuMode() &&
      pButton->IsChecked() && !pButton->IsHighlighted()) {
    return;
  }

  if (pButton->IsHighlighted() || pButton->IsChecked() ||
      pButton->IsDroppedDown() || pButton->IsFocused()) {
    if (m_bIsDrawOnGlass) {
      imdraw_DrawRectR(pDC->im, 0, rect, 0, globalData.clrBarShadow);
    }
    else {
      if (pButton->IsPressed() || pButton->IsChecked() ||
          pButton->IsDroppedDown()) {
        pDC->Draw3dRect(rect, globalData.clrBarShadow, globalData.clrBarHilite);
      }
      else {
        pDC->Draw3dRect(rect, globalData.clrBarHilite, globalData.clrBarShadow);
      }
    }

    CRect rectMenu = pButton->GetMenuRect();

    if (!rectMenu.IsRectEmpty()) {
      if (m_bIsDrawOnGlass) {
        if (pButton->IsMenuOnBottom()) {
          imdraw_DrawLine(pDC->im, 0, rectMenu.l, rectMenu.t, rectMenu.r, rectMenu.t, globalData.clrBarShadow);
        }
        else {
          imdraw_DrawLine(pDC->im, 0, rectMenu.l, rectMenu.t, rectMenu.l, rectMenu.b, globalData.clrBarShadow);
        }
      }
      else {
        COLOR pOldPen = pDC->SelectObject(globalData.penBarShadow);
        ASSERT(pOldPen != NULL);

        if (pButton->IsMenuOnBottom()) {
          pDC->MoveTo(rectMenu.l, rectMenu.t);
          pDC->LineTo(rectMenu.r, rectMenu.t);
        }
        else {
          pDC->MoveTo(rectMenu.l, rectMenu.t);
          pDC->LineTo(rectMenu.l, rectMenu.b);
        }

        pDC->SelectObject(pOldPen);
      }
    }
  }
}
//****
void CBCGPVisualManager::OnDrawRibbonMenuCheckFrame(
    CDC* pDC,
    CBCGPRibbonButton* /*pButton*/,
    CRect rect)
{
  ASSERT_VALID(pDC);

  FillRect(pDC, rect, globalData.brBtnFace);

  pDC->Draw3dRect(rect, globalData.clrBtnShadow,
      globalData.clrBtnHilite);
}
//****
void CBCGPVisualManager::OnDrawRibbonMainPanelFrame(
    CDC* pDC,
    CBCGPRibbonMainPanel* /*pPanel*/,
    CRect rect)
{
  ASSERT_VALID(pDC);

  pDC->Draw3dRect(rect, globalData.clrBarShadow, globalData.clrBarShadow);
  rect.InflateRect(1, 1);
  pDC->Draw3dRect(rect, globalData.clrBarHilite, globalData.clrBarHilite);
}
//****
void CBCGPVisualManager::OnFillRibbonMenuFrame(
    CDC* pDC,
    CBCGPRibbonMainPanel* /*pPanel*/,
    CRect rect)
{
  ASSERT_VALID(pDC);
  FillRect(pDC, rect, globalData.brWindow);
}
//****
void CBCGPVisualManager::OnDrawRibbonRecentFilesFrame(
    CDC* pDC,
    CBCGPRibbonMainPanel* /*pPanel*/,
    CRect rect)
{
  ASSERT_VALID(pDC);

  FillRect(pDC, rect, globalData.brBtnFace);

  CRect rectSeparator = rect;
  rectSeparator.r = rectSeparator.l + 2;

  pDC->Draw3dRect(rectSeparator, globalData.clrBtnShadow,
      globalData.clrBtnHilite);
}
//****
void CBCGPVisualManager::OnDrawRibbonLabel(
    CDC* /*pDC*/,
    CBCGPRibbonLabel* /*pLabel*/,
    CRect /*rect*/)
{
}
//****
void CBCGPVisualManager::OnDrawRibbonPaletteButton(
    CDC* pDC,
    CBCGPRibbonPaletteIcon* pButton)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pButton);

  OnFillRibbonButton(pDC, pButton);
  OnDrawRibbonButtonBorder(pDC, pButton);
}
//****
void CBCGPVisualManager::OnDrawRibbonPaletteBorder(
    CDC* pDC,
    CBCGPRibbonPaletteButton* /*pButton*/,
    CRect rectBorder)
{
  ASSERT_VALID(pDC);
  pDC->Draw3dRect(rectBorder, globalData.clrBarShadow, globalData.clrBarShadow);
}
//****
COLORREF CBCGPVisualManager::RibbonCategoryColorToRGB(BCGPRibbonCategoryColor color)
{
  if (globalData.m_nBitsPerPixel <= 8 || globalData.IsHighContastMode()) {
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
//****
COLORREF CBCGPVisualManager::OnDrawRibbonCategoryCaption(
    CDC* pDC,
    CBCGPRibbonContextCaption* pContextCaption)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pContextCaption);

  COLORREF clrFill = RibbonCategoryColorToRGB(pContextCaption->GetColor());
  CRect rect = pContextCaption->GetRect();

  if (clrFill != 0) {
    if (m_bIsDrawOnGlass) {

      rect.DeflateRect(0, 1);
      imdraw_DrawRectR(pDC->im, 0, rect, clrFill, 0);
    }
    else {
      COLOR clr = (clrFill);
      FillRect(pDC, rect, clr);
    }
  }

  return globalData.clrBarText;
}
//****
COLORREF CBCGPVisualManager::OnDrawRibbonStatusBarPane(CDC* pDC, CBCGPRibbonStatusBar* /*pBar*/,
    CBCGPRibbonStatusBarPane* pPane)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pPane);

  CRect rect = pPane->GetRect();

  if (pPane->IsHighlighted()) {
    CRect rectButton = rect;
    rectButton.DeflateRect(1, 1);

    pDC->Draw3dRect(rectButton,
        pPane->IsPressed() ? globalData.clrBarShadow : globalData.clrBarHilite,
        pPane->IsPressed() ? globalData.clrBarHilite : globalData.clrBarShadow);
  }

  return 0;
}
//****
void CBCGPVisualManager::GetRibbonSliderColors(CBCGPRibbonSlider* /*pSlider*/,
    BOOL /*bIsHighlighted*/,
    BOOL /*bIsPressed*/,
    BOOL /*bIsDisabled*/,
    COLORREF& /*clrLine*/,
    COLORREF& /*clrFill*/)
{
  /*  clrLine = bIsDisabled ? globalData.clrBarShadow : globalData.clrBarDkShadow;

    clrFill = bIsPressed && bIsHighlighted ?
      globalData.clrBarShadow :
      bIsHighlighted ? globalData.clrBarHilite : globalData.clrBarFace;*/
}
//****
void CBCGPVisualManager::OnDrawRibbonSliderZoomButton(
    CDC* pDC, CBCGPRibbonSlider* /*pSlider*/,
    CRect rect, BOOL bIsZoomOut,
    BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled)
{
  ASSERT_VALID(pDC);

  COLORREF clrLine = bIsDisabled ? globalData.clrBarShadow : globalData.clrBarDkShadow;

  COLORREF clrFill = bIsPressed && bIsHighlighted ?
      globalData.clrBarShadow :
      bIsHighlighted ? globalData.clrBarHilite : globalData.clrBarFace;

  CPoint ptCenter = rect.CenterPoint();

  CRect rectCircle(CPoint(ptCenter.x - 7, ptCenter.y - 7), CSize(15, 15));



  imdraw_EllipseR(pDC->im, 0, rectCircle, clrFill, clrLine, 1);

  // Draw +/- sign:
  CRect rectSign(CPoint(ptCenter.x - 3, ptCenter.y - 3), CSize(7, 7));

  if (m_bIsDrawOnGlass) {
    imdraw_DrawLine(pDC->im, 0, rectSign.l, ptCenter.y, rectSign.r, ptCenter.y, clrLine);

    if (!bIsZoomOut) {
      imdraw_DrawLine(pDC->im, 0, ptCenter.x, rectSign.t, ptCenter.x, rectSign.b, clrLine);
    }
  }
  else {
    COLOR pOldPen = pDC->SelectObject(clrLine);

    pDC->MoveTo(rectSign.l, ptCenter.y);
    pDC->LineTo(rectSign.r, ptCenter.y);

    if (!bIsZoomOut) {
      pDC->MoveTo(ptCenter.x, rectSign.t);
      pDC->LineTo(ptCenter.x, rectSign.b);
    }

    pDC->SelectObject(pOldPen);
  }
}
//****
void CBCGPVisualManager::OnDrawRibbonSliderChannel(
    CDC* pDC, CBCGPRibbonSlider* pSlider, CRect rect)
{
  BOOL bIsVert = FALSE;

  if (pSlider != NULL) {
    ASSERT_VALID(pSlider);
    bIsVert = pSlider->IsVert();
  }

  OnDrawSliderChannel(pDC, NULL, bIsVert, rect, m_bIsDrawOnGlass);
}
//****
void CBCGPVisualManager::OnDrawRibbonSliderThumb(
    CDC* pDC, CBCGPRibbonSlider* pSlider,
    CRect rect, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled)
{
  BOOL bIsVert = FALSE;
  BOOL bLeftTop = FALSE;
  BOOL bRightBottom = FALSE;

  if (pSlider != NULL) {
    ASSERT_VALID(pSlider);

    bIsVert = pSlider->IsVert();
    bLeftTop = pSlider->IsThumbLeftTop();
    bRightBottom = pSlider->IsThumbRightBottom();
  }

  OnDrawSliderThumb(pDC, NULL, rect, bIsHighlighted, bIsPressed, bIsDisabled,
      bIsVert, bLeftTop, bRightBottom, m_bIsDrawOnGlass);
}
//****
void CBCGPVisualManager::OnDrawRibbonProgressBar(CDC* pDC,
    CBCGPRibbonProgressBar* /*pProgress*/,
    CRect rectProgress, CRect rectChunk,
    BOOL /*bInfiniteMode*/)
{
  ASSERT_VALID(pDC);

  if (m_bIsDrawOnGlass) {


    if (!rectChunk.IsRectEmpty()) {
      imdraw_DrawRectR(pDC->im, 0, rectChunk, globalData.clrHilite, 0);
    }

    imdraw_DrawRectR(pDC->im, 0, rectProgress, 0, globalData.clrBarShadow);
  }
  else {
    if (!rectChunk.IsRectEmpty()) {
      FillRect(pDC, rectChunk, globalData.brHilite);
    }

    pDC->Draw3dRect(rectProgress, globalData.clrBarShadow, globalData.clrBarHilite);
  }
}
//****
void CBCGPVisualManager::OnFillRibbonQATPopup(
    CDC* pDC, CBCGPRibbonPanelMenuBar* /*pMenuBar*/, CRect rect)
{
  ASSERT_VALID(pDC);
  FillRect(pDC, rect, globalData.brBarFace);
}
//****
void CBCGPVisualManager::OnDrawRibbonQATSeparator(CDC* pDC,
    CBCGPRibbonSeparator* /*pSeparator*/, CRect rect)
{
  ASSERT_VALID(pDC);

  if (m_bIsDrawOnGlass) {

    imdraw_DrawRectR(pDC->im, 0, rect, 0, globalData.clrBtnShadow);
  }
  else {
    pDC->Draw3dRect(rect, globalData.clrBarShadow, globalData.clrBarHilite);
  }
}
//****
void CBCGPVisualManager::OnDrawRibbonKeyTip(CDC* pDC, CBCGPBaseRibbonElement* pElement,
    CRect rect, CString str)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pElement);

  COLORREF clrText = _GetSysColor(CLR_INFOTEXT);
  COLORREF clrBorder = clrText;

  {
    ::FillRect(pDC, rect, GetSysColorBrush(CLR_INFOBK));
  }

  str.MakeUpper();

  COLORREF clrTextOld = pDC->SetTextColor(pElement->IsDisabled() ? globalData.clrGrayedText : clrText);

  pDC->DrawText(str, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

  pDC->SetTextColor(clrTextOld);

  pDC->Draw3dRect(rect, clrBorder, clrBorder);
}
//****
void CBCGPVisualManager::OnDrawRibbonRadioButtonOnList(CDC* pDC, CBCGPRibbonRadioButton* /*pRadioButton*/,
    CRect rect, BOOL /*bIsSelected*/, BOOL /*bHighlighted*/)
{
  ASSERT_VALID(pDC);

  rect.OffsetRect(1, 1);
  CBCGPMenuImages_Draw(pDC, IdRadio, rect, ImageWhite);

  rect.OffsetRect(-1, -1);
  CBCGPMenuImages_Draw(pDC, IdRadio, rect, ImageBlack);
}
//****
void CBCGPVisualManager::OnDrawRibbonCheckBoxOnList(CDC* pDC, CBCGPRibbonCheckBox* /*pCheckBox*/,
    CRect rect, BOOL /*bIsSelected*/, BOOL /*bHighlighted*/)
{
  ASSERT_VALID(pDC);

  rect.OffsetRect(1, 1);
  CBCGPMenuImages_Draw(pDC, IdCheck, rect, ImageWhite);

  rect.OffsetRect(-1, -1);
  CBCGPMenuImages_Draw(pDC, IdCheck, rect, ImageBlack);
}
//****
COLORREF CBCGPVisualManager::GetRibbonHyperlinkTextColor(CBCGPRibbonHyperlink* pHyperLink)
{
  ASSERT_VALID(pHyperLink);

  if (pHyperLink->IsDisabled()) {
    return GetToolbarDisabledTextColor();
  }

  return pHyperLink->IsHighlighted() ? globalData.clrHotLinkText : globalData.clrHotText;
}
//****
COLORREF CBCGPVisualManager::GetRibbonStatusBarTextColor(CBCGPRibbonStatusBar* /*pStatusBar*/)
{
  return globalData.clrBarText;
}
//****
void CBCGPVisualManager::OnDrawRibbonColorPaletteBox(CDC* pDC, CBCGPRibbonColorButton* /*pColorButton*/,
    CBCGPRibbonPaletteIcon* /*pIcon*/,
    COLORREF color, CRect rect, BOOL bDrawTopEdge, BOOL bDrawBottomEdge,
    BOOL bIsHighlighted, BOOL bIsChecked, BOOL /*bIsDisabled*/)
{
  ASSERT_VALID(pDC);

  CRect rectFill = rect;
  rectFill.DeflateRect(1, 0);

  if (bIsHighlighted || bIsChecked) {
    FillDitheredRect(pDC->im, rect);
    rectFill.DeflateRect(1, 2);
  }

  if (color != 0) {
    COLOR clr = (color);
    FillRect(pDC, rectFill, clr);
  }

  COLORREF clrBorder = globalData.clrBtnShadow;

  if (bDrawTopEdge && bDrawBottomEdge) {
    pDC->Draw3dRect(rect, clrBorder, clrBorder);
  }
  else {
    COLOR pOldPen = pDC->SelectObject(clrBorder);
    ASSERT(pOldPen != NULL);

    pDC->MoveTo(rect.l, rect.t);
    pDC->LineTo(rect.l, rect.b);

    pDC->MoveTo(rect.r - 1, rect.t);
    pDC->LineTo(rect.r - 1, rect.b);

    if (bDrawTopEdge) {
      pDC->MoveTo(rect.l, rect.t);
      pDC->LineTo(rect.r, rect.t);
    }

    if (bDrawBottomEdge) {
      pDC->MoveTo(rect.l, rect.b - 1);
      pDC->LineTo(rect.r, rect.b - 1);
    }

    pDC->SelectObject(pOldPen);
  }

  if (bIsHighlighted) {
    pDC->Draw3dRect(&rect, globalData.clrBarHilite, globalData.clrBarShadow);
  }
  else if (bIsChecked) {
    pDC->Draw3dRect(&rect, globalData.clrBarShadow, globalData.clrBarHilite);
  }
}

#endif // BCGP_EXCLUDE_RIBBON

BOOL CBCGPVisualManager::OnSetWindowRegion(CWnd* pWnd, CSize sizeWindow)
{
#if 0
#ifdef BCGP_EXCLUDE_RIBBON
  UNREFERENCED_PARAMETER(pWnd);
  UNREFERENCED_PARAMETER(sizeWindow);
  return FALSE;
#else

  if (globalData.DwmIsCompositionEnabled()) {
    return FALSE;
  }

  ASSERT_VALID(pWnd);

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

  const int nLeftRadius  = 11;
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
#endif
#endif
  return TRUE;
}
//****
BOOL CBCGPVisualManager::OnNcPaint(CWnd* /*pWnd*/, const CObList& /*lstSysButtons*/, CRect /*rectRedraw*/)
{
  return FALSE;
}
//****
void CBCGPVisualManager::OnActivateApp(CWnd* /*pWnd*/, BOOL /*bActive*/)
{
}
//****
BOOL CBCGPVisualManager::OnNcActivate(CWnd* /*pWnd*/, BOOL /*bActive*/)
{
  return FALSE;
}
//****
CSize CBCGPVisualManager::GetNcBtnSize(BOOL /*bSmall*/) const
{
  return CSize(0, 0);
}
//****
BOOL CBCGPVisualManager::OnUpdateNcButtons(CWnd* /*pWnd*/, const CObList& /*lstSysButtons*/, CRect /*rectCaption*/)
{
  return FALSE;
}
//****
BOOL CBCGPVisualManager::OnEraseMDIClientArea(CDC* /*pDC*/, CRect /*rectClient*/)
{
  return FALSE;
}
//****
BOOL CBCGPVisualManager::GetToolTipParams(CBCGPToolTipParams& params,
    UINT /*nType*/ /*= (UINT)(-1)*/)
{
  CBCGPToolTipParams dummy;
  params = dummy;

  return TRUE;
}
//****
void CBCGPVisualManager::OnFillToolTip(CDC* pDC, CBCGPToolTipCtrl* /*pToolTip*/, CRect rect,
    COLORREF& clrText, COLORREF& clrLine)
{
  {
    ::FillRect(pDC, rect, GetSysColorBrush(CLR_INFOBK));
  }
}
//****
BOOL CBCGPVisualManager::DrawTextOnGlass(CDC* pDC, CString strText, CRect rect,
    DWORD dwFlags, int nGlowSize, COLORREF clrText)
{
  ASSERT_VALID(pDC);

  //---- bits used in dwFlags of DTTOPTS ----
#define BCGP_DTT_TEXTCOLOR       (1UL << 0)      // crText has been specified
#define BCGP_DTT_BORDERCOLOR     (1UL << 1)      // crBorder has been specified
#define BCGP_DTT_SHADOWCOLOR     (1UL << 2)      // crShadow has been specified
#define BCGP_DTT_SHADOWTYPE      (1UL << 3)      // iTextShadowType has been specified
#define BCGP_DTT_SHADOWOFFSET    (1UL << 4)      // ptShadowOffset has been specified
#define BCGP_DTT_BORDERSIZE      (1UL << 5)      // iBorderSize has been specified
#define BCGP_DTT_FONTPROP        (1UL << 6)      // iFontPropId has been specified
#define BCGP_DTT_COLORPROP       (1UL << 7)      // iColorPropId has been specified
#define BCGP_DTT_STATEID         (1UL << 8)      // IStateId has been specified
#define BCGP_DTT_CALCRECT        (1UL << 9)      // Use pRect as and in/out parameter
#define BCGP_DTT_APPLYOVERLAY    (1UL << 10)     // fApplyOverlay has been specified
#define BCGP_DTT_GLOWSIZE        (1UL << 11)     // iGlowSize has been specified
#define BCGP_DTT_CALLBACK        (1UL << 12)     // pfnDrawTextCallback has been specified
#define BCGP_DTT_COMPOSITED      (1UL << 13)     // Draws text with antialiased alpha (needs a DIB section)


  pDC->DrawText(strText, rect, dwFlags);

  return TRUE;
}
//****
COLORREF CBCGPVisualManager::OnFillListBoxItem(CDC* pDC, CBCGPListBox* /*pListBox*/, int /*nItem*/, CRect rect, BOOL bIsHighlihted, BOOL bIsSelected)
{
  ASSERT_VALID(pDC);

  COLORREF clrText = 0;

  if (bIsSelected) {
    FillRect(pDC, rect, globalData.brHilite);
    clrText = globalData.clrTextHilite;
  }

  if (bIsHighlihted) {
    pDC->DrawFocusRect(rect);
  }

  return clrText;
}
//****
void CBCGPVisualManager::OnDrawGroup(CDC* pDC, CBCGPGroup* pGroup, CRect rect, const CString& strName)
{
  ASSERT_VALID(pDC);
  ASSERT_VALID(pGroup);

  CSize sizeText = pDC->GetTextExtent(strName);

  CRect rectFrame = rect;
  rectFrame.t += sizeText.h / 2;

  if (sizeText == CSize(0, 0)) {
    rectFrame.t += pDC->GetTextExtent(_T("A")).h / 2;
  }

  int xMargin = sizeText.h / 2;

  CRect rectText = rect;
  rectText.l += xMargin;
  rectText.r = rectText.l + sizeText.w + xMargin;
  rectText.b = rectText.t + sizeText.h;

  if (!strName.IsEmpty()) {
    pDC->ExcludeClipRect(rectText);
  }

  {

    imdraw_DrawRectR(pDC->im, 0, rectFrame, 0, globalData.clrBarDkShadow);
  }

  pDC->SelectClipRgn(NULL);

  if (strName.IsEmpty()) {
    return;
  }

  DWORD dwTextStyle = DT_SINGLELINE | DT_VCENTER | DT_CENTER | DT_NOCLIP;

  if (pGroup->IsOnGlass()) {
    DrawTextOnGlass(pDC, strName, rectText, dwTextStyle, 10, globalData.clrBarText);
  }
  else {
    CString strCaption = strName;
    pDC->DrawText(strCaption, rectText, dwTextStyle);
  }
}
//****
BOOL CBCGPVisualManager::OnFillDialog(CDC* pDC, CRect rect)
{
  ASSERT_VALID(pDC);
  FillRect(pDC, rect, GetDlgBackBrush());

  return TRUE;
}
//****
COLOR CBCGPVisualManager::GetDlgBackBrush()
{
  return globalData.brBarFace;
}
//****
void CBCGPVisualManager::OnDrawSliderChannel(CDC* pDC, CBCGPSliderCtrl* /*pSlider*/, BOOL bVert, CRect rect, BOOL bDrawOnGlass)
{
  ASSERT_VALID(pDC);

  if (bVert) {
    if (rect.Width() < 3) {
      rect.r++;
    }
  }
  else {
    if (rect.Height() < 3) {
      rect.b++;
    }
  }

  if (bDrawOnGlass) {

    imdraw_DrawRectR(pDC->im, 0, rect, 0, globalData.clrBarShadow);
  }
  else {
    pDC->Draw3dRect(rect, globalData.clrBarShadow, globalData.clrBarHilite);
  }
}
//****
void CBCGPVisualManager::OnDrawSliderThumb(CDC* pDC, CBCGPSliderCtrl* /*pSlider*/,
    CRect rect, BOOL bIsHighlighted, BOOL bIsPressed, BOOL bIsDisabled,
    BOOL bVert, BOOL bLeftTop, BOOL bRightBottom, BOOL /*bDrawOnGlass*/)
{
  ASSERT_VALID(pDC);

  COLORREF clrLine = bIsDisabled ? globalData.clrBarShadow : globalData.clrBarDkShadow;
  COLORREF clrFill = !bIsDisabled && (bIsHighlighted || bIsPressed) ?
      globalData.clrBarHilite : globalData.clrBarFace;

  if (bVert) {
    rect.DeflateRect(2, 1);

    rect.l = rect.CenterPoint().x - rect.Height();
    rect.r = rect.l + 2 * rect.Height();
  }
  else {
    rect.DeflateRect(1, 2);

    rect.t = rect.CenterPoint().y - rect.Width();
    rect.b = rect.t + 2 * rect.Width();
  }

  imdraw_DrawRectR(pDC->im, 0, rect, clrFill, clrLine);
}
//****
COLORREF CBCGPVisualManager::GetURLLinkColor(CBCGPURLLinkButton* /*pButton*/, BOOL bHover)
{
  return bHover ? globalData.clrHotLinkText : globalData.clrHotText;
}
//****
BOOL CBCGPVisualManager::OnFillParentBarBackground(CWnd* pWnd, CDC* pDC, IRECT* rectClip)
{
#if 0
  ASSERT_VALID(pDC);
  ASSERT_VALID(pWnd);

  CWnd* pParent = pWnd->GetParent();
  ASSERT_VALID(pParent);

  CBCGPBaseControlBar* pParentBar = DYNAMIC_DOWNCAST(CBCGPBaseControlBar, pParent);

  if (pParentBar == NULL) {
    return FALSE;
  }

  CBCGPDialogBar* pDialogBar = DYNAMIC_DOWNCAST(CBCGPDialogBar, pParentBar);

  if (pDialogBar != NULL && pDialogBar->IsVisualManagerStyle()) {
    return FALSE;
  }

  CRgn rgn;

  if (rectClip != NULL) {
    rgn.CreateRectRgnIndirect(rectClip);
    pDC->SelectClipRgn(&rgn);
  }

  CPoint pt(0, 0);
  pWnd->MapWindowPoints(pParent, &pt, 1);
  pt = pDC->OffsetWindowOrg(pt.x, pt.y);

  CRect rectBar;
  pParentBar->GetClientRect(rectBar);

  OnFillBarBackground(pDC, pParentBar, rectBar, rectBar);

  pDC->SetWindowOrg(pt.x, pt.y);

  pDC->SelectClipRgn(NULL);

#endif
  return TRUE;
}

#undef POINTS_NUM
