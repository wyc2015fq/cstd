static LRESULT MarkerWnd_OnPaint()
{
  //m_pDrawManager->DrawMarker( ( DockingMarkers * ) m_pDockingMarkers, &dc, &rc, m_Marker, &m_Imagelist, m_bHighlight, m_bDisable );
  return 0;
}
#if 0
//
static int DockingMarkers::TimerObj::OnTimer(Timer* timer, int id, int /*time*/)
{
  if (id == 1) { // appearance.
    m_iTransparence = max(m_iMinTransparence, m_iTransparence - m_iAnimationStep);
    if (m_iTransparence == m_iMinTransparence) {
      timer->KillTimer();
    }
  }
  else { // disappearance.
    m_iTransparence = min(100, m_iTransparence + m_iAnimationStep);
    if (m_iTransparence == 100) {
      timer->KillTimer();
      //
      for (i_lst_markWnds i = lst_markWnds.begin(), e = lst_markWnds.end(); i != e; ++i) {
        ShowWindow((*i) ->m_hWnd, SW_HIDE);
      }
    }
  }
  //
  for (i_lst_markWnds i = lst_markWnds.begin(), e = lst_markWnds.end(); i != e; ++i)
    if (id == 1 && (*i) ->IsHighlight() == TRUE && m_iTransparence == m_iMinTransparence) { // cursor above marker and it is finish of appearance.
      (*i) ->SetTransparence(0);
    }
    else {
      (*i) ->SetTransparence(m_iTransparence);
    }
}
//
static int DockingMarkers::TimerObj::Highlight(MarkerWnd* wnd, BOOL highlight)
{
  if (highlight == wnd->IsHighlight()) {
    return ;
  }
  wnd->Highlight(highlight);
  if (m_iMinTransparence > 0 && (m_bAnimation == FALSE || m_iTransparence <= m_iMinTransparence)) {
    wnd->SetTransparence(highlight == FALSE ? m_iMinTransparence : 0);
  }
  wnd->Invalidate(FALSE);
}
// DockingMarkers::Pane.
//
DockingMarkers::Pane::Pane(DockingMarkers* p) :
  left(p, DOCKMARKER_PANE_LEFT), top(p, DOCKMARKER_PANE_TOP),
  right(p, DOCKMARKER_PANE_RIGHT), bottom(p, DOCKMARKER_PANE_BOTTOM),
  tabs(p, DOCKMARKER_PANE_TABS) {}
// DockingMarkers::Frame::Frame.
//
DockingMarkers::Frame::Frame(DockingMarkers* p) :
  left(p, DOCKMARKER_FRAME_LEFT), top(p, DOCKMARKER_FRAME_TOP),
  right(p, DOCKMARKER_FRAME_RIGHT), bottom(p, DOCKMARKER_FRAME_BOTTOM) {}
// DockingMarkers::SelectWindow.
BEGIN_MESSAGE_MAP(DockingMarkers::SelectWindow, CWnd)
  ON_MESSAGE(WM_PAINT, DockingMarkers::SelectWindow::OnPaint)
END_MESSAGE_MAP()
//
DockingMarkers::SelectWindow::SelectWindow(DockingMarkers* p) :
  m_pDockingMarkers(p)
{
  m_pDrawManager = NULL;
}
//
DockingMarkers::SelectWindow::~SelectWindow()
{
  DestroyWindow(); // for delete warning: 'calling DestroyWindow in CWnd::~CWnd; OnDestroy or PostNcDestroy in derived class will not be called'.
}
static int DockingMarkers::SelectWindow::SetParams(BOOL bTab, BOOL bTop, int iTabHeight)
{
  m_bTab = bTab;
  m_bTop = bTop;
  m_iTabHeight = iTabHeight;
}
//
static int DockingMarkers::SelectWindow::Show(HWND hWndAfter, IRECT const* rcScr)
{
  SetWindowPos(m_hWnd, hWndAfter, rcScr->l, rcScr->t, rcScr RCW(), rcScr RCH(),
      SWP_SHOWWINDOW | SWP_NOACTIVATE | SWP_NOCOPYBITS);
  RedrawWindow(m_hWnd, NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
}
//
static int DockingMarkers::SelectWindow::Hide()
{
  ShowWindow(m_hWnd, SW_HIDE);
}
//
LRESULT DockingMarkers::SelectWindow::OnPaint(WPARAM /*wp*/, LPARAM /*lp*/)
{
  CPaintDC dc(this);
  VirtualWindow virtwnd(&dc, this);
  //
  IRECT rc;
  GetClientRect(&rc);
  //
  imdraw_FillSolidRect(&virtwnd, &rc, m_clrMask);
  if (m_pDrawManager != NULL) {
    m_pDrawManager->DrawSelectWindow((DockingMarkers*) m_pDockingMarkers, &virtwnd, &rc, m_bTab, m_bTop, m_iTabHeight);
  }
  return 0;
}
//
static int DockingMarkers::ShowPaneMarkers(IRECT const* rcScr)
{
  _ASSERTE(WasCreated() == TRUE);
  //
  m_ShownPane = (m_ShownPane == Pane::TYPE_FIRST ? Pane::TYPE_SECOND : Pane::TYPE_FIRST); // switch pane.
  Pane* p = GetWorkingPane();
  //
  // set enable states for markers.
  p->l.Disable(!m_pAbilityManager->IsMarkerEnable(this, DOCKMARKER_PANE_LEFT));
  p->t.Disable(!m_pAbilityManager->IsMarkerEnable(this, DOCKMARKER_PANE_TOP));
  p->r.Disable(!m_pAbilityManager->IsMarkerEnable(this, DOCKMARKER_PANE_RIGHT));
  p->b.Disable(!m_pAbilityManager->IsMarkerEnable(this, DOCKMARKER_PANE_BOTTOM));
  p->tabs.Disable(!m_pAbilityManager->IsMarkerEnable(this, DOCKMARKER_PANE_TABS));
  //
  // calculate left-top point.
  p->basePoint.x = (rcScr->l + rcScr->r - m_Layout.m_iWidthTotal) / 2;
  p->basePoint.y = (rcScr->t + rcScr->b - m_Layout.m_iHeightTotal) / 2;
  // positioning and show new markers.
  p->tabs.SetPos(p->basePoint + m_Layout.m_tabsPane.pos);
  p->l.SetPos(p->basePoint + m_Layout.m_leftPane.pos);
  p->t.SetPos(p->basePoint + m_Layout.m_topPane.pos);
  p->r.SetPos(p->basePoint + m_Layout.m_rightPane.pos);
  p->b.SetPos(p->basePoint + m_Layout.m_bottomPane.pos);
  p->timer.Show();
  // hide old markers.
  p = GetNotWorkingPane();
  p->timer.Hide();
}
//
static int DockingMarkers::ShowFrameMarkers(IRECT const* rcScr)
{
  _ASSERTE(WasCreated() == TRUE);
  _ASSERTE(m_LayoutType == LAYOUT_FRAME);
  //
  // set enable states for markers.
  frame.left.Disable(!m_pAbilityManager->IsMarkerEnable(this, DOCKMARKER_FRAME_LEFT));
  frame.top.Disable(!m_pAbilityManager->IsMarkerEnable(this, DOCKMARKER_FRAME_TOP));
  frame.right.Disable(!m_pAbilityManager->IsMarkerEnable(this, DOCKMARKER_FRAME_RIGHT));
  frame.b.Disable(!m_pAbilityManager->IsMarkerEnable(this, DOCKMARKER_FRAME_BOTTOM));
  //
  IRECT rc(rcScr);
  rc.DeflateRect(12, 12);
  //
  // calculate base points.
  frame.baseLeft = CPoint(rc.left, (rcScr->t + rcScr->b - frame.left.GetSize().cy) / 2);
  frame.baseTop = CPoint((rcScr->l + rcScr->r - frame.top.GetSize().cx) / 2, rc.top);
  frame.baseRight = CPoint(rc.right - frame.right.GetSize().cx, (rcScr->t + rcScr->b - frame.right.GetSize().cy) / 2);
  frame.baseBottom = CPoint((rcScr->l + rcScr->r - frame.b.GetSize().cx) / 2, rc.b - frame.b.GetSize().cy);
  // positioning and show markers.
  frame.left.SetPos(frame.baseLeft);
  frame.top.SetPos(frame.baseTop);
  frame.right.SetPos(frame.baseRight);
  frame.b.SetPos(frame.baseBottom);
  frame.timer.Show();
}
//
static int DockingMarkers::HidePaneMarkers()
{
  _ASSERTE(WasCreated() == TRUE);
  //
  GetWorkingPane() ->timer.Hide();
}
//
static int DockingMarkers::HideFrameMarkers()
{
  _ASSERTE(m_LayoutType == LAYOUT_FRAME);
  _ASSERTE(WasCreated() == TRUE);
  //
  frame.timer.Hide();
}
//
static int DockingMarkers::ShowSelectBar(CWnd* pParentWnd, IRECT const* rcScr, BOOL bTab, BOOL bTop, int iTabHeight)
{
  if (WasCreated() == TRUE) {
    m_SelectWnd.SetParams(bTab, bTop, iTabHeight);
    //
    HWND hWndAfter =
        (IsWindowVisible(frame.left.m_hWnd) != 0 ? frame.left.m_hWnd :
            (IsWindowVisible(GetWorkingPane() ->tabs.m_hWnd) != 0 ? GetWorkingPane() ->tabs.m_hWnd : HWND_TOPMOST));
    m_SelectWnd.Show(hWndAfter, rcScr);
  }
  else if (GetDrawManager() != NULL) {
    m_bTab = bTab;
    m_bTop = bTop;
    m_iTabHeight = iTabHeight;
    //
    IRECT rc(rcScr);
    pParentWnd->ScreenToClient(&rc);
    DrawSelectFrame(pParentWnd, &rc);
  }
}
//
static int DockingMarkers::HideSelectBar(CWnd* pParentWnd)
{
  if (WasCreated() == TRUE) {
    m_SelectWnd.Hide();
  }
  else if (GetDrawManager() != NULL) {
    DrawSelectFrame(pParentWnd, NULL);
  }
}
//
static int DockingMarkers::DrawSelectFrame(CWnd* pParentWnd, IRECT const* pRect)
{
  static IRECT rcOld(0, 0, 0, 0);
  //
  if (pRect != NULL || rcOld.IsRectNull() == 0) {
    img_t* im = pParentWnd->GetDCEx(NULL, DCX_CACHE | DCX_LOCKWINDOWUPDATE);
    if (im != NULL) {
      GetDrawManager() ->DrawSelectFrame(this, im, pRect, (rcOld.IsRectNull() == 0 ? &rcOld : NULL),
          m_bTab, m_bTop, m_iTabHeight);
      pParentWnd->ReleaseDC(im);
    }
  }
  //
  rcOld = (pRect != NULL ? *pRect : IRECT(0, 0, 0, 0));
}
//
static int DockingMarkers::RelayMouseMove(CPoint pt, BOOL testPaneMarkers, BOOL testFrameMarkers)
{
  _ASSERTE(WasCreated() == TRUE);
  //
  int marker = HitTest(pt, testPaneMarkers, testFrameMarkers);
  //
  if (marker != DOCKMARKER_NONE)
    if (m_pAbilityManager->IsMarkerEnable(this, marker) == FALSE) {
      marker = DOCKMARKER_NONE;
    }
  //
  Pane* p = GetWorkingPane();
  //
  switch (marker) {
  case DOCKMARKER_PANE_LEFT:
    p->timer.Highlight(&p->l, TRUE);
    break;
  case DOCKMARKER_PANE_TOP:
    p->timer.Highlight(&p->t, TRUE);
    break;
  case DOCKMARKER_PANE_RIGHT:
    p->timer.Highlight(&p->r, TRUE);
    break;
  case DOCKMARKER_PANE_BOTTOM:
    p->timer.Highlight(&p->b, TRUE);
    break;
  case DOCKMARKER_PANE_TABS:
    p->timer.Highlight(&p->tabs, TRUE);
    break;
    //
  case DOCKMARKER_FRAME_LEFT:
    frame.timer.Highlight(&frame.left, TRUE);
    break;
  case DOCKMARKER_FRAME_TOP:
    frame.timer.Highlight(&frame.top, TRUE);
    break;
  case DOCKMARKER_FRAME_RIGHT:
    frame.timer.Highlight(&frame.right, TRUE);
    break;
  case DOCKMARKER_FRAME_BOTTOM:
    frame.timer.Highlight(&frame.b, TRUE);
    break;
  }
  //
  if (marker != DOCKMARKER_PANE_LEFT) {
    p->timer.Highlight(&p->l, FALSE);
  }
  if (marker != DOCKMARKER_PANE_TOP) {
    p->timer.Highlight(&p->t, FALSE);
  }
  if (marker != DOCKMARKER_PANE_RIGHT) {
    p->timer.Highlight(&p->r, FALSE);
  }
  if (marker != DOCKMARKER_PANE_BOTTOM) {
    p->timer.Highlight(&p->b, FALSE);
  }
  if (marker != DOCKMARKER_PANE_TABS) {
    p->timer.Highlight(&p->tabs, FALSE);
  }
  //
  if (m_LayoutType == LAYOUT_FRAME) {
    if (marker != DOCKMARKER_FRAME_LEFT) {
      frame.timer.Highlight(&frame.left, FALSE);
    }
    if (marker != DOCKMARKER_FRAME_TOP) {
      frame.timer.Highlight(&frame.top, FALSE);
    }
    if (marker != DOCKMARKER_FRAME_RIGHT) {
      frame.timer.Highlight(&frame.right, FALSE);
    }
    if (marker != DOCKMARKER_FRAME_BOTTOM) {
      frame.timer.Highlight(&frame.b, FALSE);
    }
  }
  //
  return marker;
}
//
static int DockingMarkers::HitTest(CPoint pt, BOOL testPaneMarkers, BOOL testFrameMarkers)
{
  Pane* p = GetWorkingPane();
  //
  if (testPaneMarkers == TRUE)
    if ((m_Layout.m_leftPane.area + p->basePoint + m_Layout.m_leftPane.pos).PtInRect(pt) != FALSE) {
      return DOCKMARKER_PANE_LEFT;
    }
    else if ((m_Layout.m_topPane.area + p->basePoint + m_Layout.m_topPane.pos).PtInRect(pt) != FALSE) {
      return DOCKMARKER_PANE_TOP;
    }
    else if ((m_Layout.m_rightPane.area + p->basePoint + m_Layout.m_rightPane.pos).PtInRect(pt) != FALSE) {
      return DOCKMARKER_PANE_RIGHT;
    }
    else if ((m_Layout.m_bottomPane.area + p->basePoint + m_Layout.m_bottomPane.pos).PtInRect(pt) != FALSE) {
      return DOCKMARKER_PANE_BOTTOM;
    }
    else if ((m_Layout.m_tabsPane.area + p->basePoint + m_Layout.m_tabsPane.pos).PtInRect(pt) != FALSE) {
      return DOCKMARKER_PANE_TABS;
    }
  //
  if (testFrameMarkers == TRUE)
    if (m_LayoutType == LAYOUT_FRAME)
      if ((m_Layout.m_leftFrame.area + frame.baseLeft).PtInRect(pt) != FALSE) {
        return DOCKMARKER_FRAME_LEFT;
      }
      else if ((m_Layout.m_topFrame.area + frame.baseTop).PtInRect(pt) != FALSE) {
        return DOCKMARKER_FRAME_TOP;
      }
      else if ((m_Layout.m_rightFrame.area + frame.baseRight).PtInRect(pt) != FALSE) {
        return DOCKMARKER_FRAME_RIGHT;
      }
      else if ((m_Layout.m_bottomFrame.area + frame.baseBottom).PtInRect(pt) != FALSE) {
        return DOCKMARKER_FRAME_BOTTOM;
      }
  //
  return DOCKMARKER_NONE;
}
//
static int DockingMarkers::SetDrawManager(DockingMarkersDraw* p)
{
  pane1.left.SetDrawManager(p);
  pane1.top.SetDrawManager(p);
  pane1.right.SetDrawManager(p);
  pane1.b.SetDrawManager(p);
  pane1.tabs.SetDrawManager(p);
  pane2.left.SetDrawManager(p);
  pane2.top.SetDrawManager(p);
  pane2.right.SetDrawManager(p);
  pane2.b.SetDrawManager(p);
  pane2.tabs.SetDrawManager(p);
  //
  frame.left.SetDrawManager(p);
  frame.top.SetDrawManager(p);
  frame.right.SetDrawManager(p);
  frame.b.SetDrawManager(p);
  //
  m_SelectWnd.SetDrawManager(p);
}
// DockingMarkersDrawBase.
#endif
//
static int DockingMarkersDrawBase_GetSelectBarBorderWidth(DockingMarkers* pObj)
{
  return 3;
}
//
static int DockingMarkersDrawBase_GetSelectFrameWidth(DockingMarkers* pObj)
{
  return 4;
}
//
static COLOR DockingMarkersDrawBase_GetSelectBarInsideColor(DockingMarkers* pObj)
{
  return _GetSysColor(CLR_HIGHLIGHT);
}
//
static COLOR DockingMarkersDrawBase_GetSelectBarBorderColor(DockingMarkers* pObj)
{
  return _GetSysColor(CLR_ACTIVEBORDER);
}
//
static int DockingMarkersDrawBase_DrawMarker(DockingMarkers* pObj, img_t* im, const IRECT* pclip, IRECT const* pRect, int marker, const char* pImageList, BOOL bHighlight, BOOL bDisable)
{
  //pImageList->Draw( im, ( bDisable == FALSE ? ( bHighlight == FALSE ? 0 : 1 ) : 2 ), CPoint( 0, 0 ), ILD_NORMAL );
  int i = (bDisable == FALSE ? (bHighlight == FALSE ? 0 : 1) : 2);
  ctrl_t* c = pObj->c;
  img_t* im1 = sys_find_img(c->sys, pImageList);
  if (im1) {
    return imdraw_image_grid(im, pclip, *pRect, im1, i, 0, 3, 1, 0, pObj->m_Layout->m_clrMask);
  }
  return 0;
}
//
static int DockingMarkersDrawBase_DrawSelectBarInside(DockingMarkers* pObj, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  imdraw_FillSolidRect(im, pclip, pRect, pObj->GetSelectBarInsideColor(pObj));
  return 0;
}
//
static int DockingMarkersDrawBase_DrawSelectBarBorder(DockingMarkers* pObj, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  imdraw_FillSolidRect(im, pclip, pRect, pObj->GetSelectBarBorderColor(pObj));
  return 0;
}
//
static int DockingMarkersDrawBase_DrawSelectFrame_Impl(DockingMarkers* pObj, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  //CBrush * pOldBrush = im->SelectObject( img_t::GetHalftoneBrush() );
  //im->PatBlt( pRect->l, pRect->t, pRect RCW(), pRect RCH(), PATINVERT );
  //im->SelectObject( pOldBrush );
  return 0;
}
//
static int DockingMarkersDrawBase_DrawSelectWindow(DockingMarkers* pObj, img_t* im, const IRECT* pclip, IRECT const* pRect, BOOL bTab, BOOL bTop, int iTabHeight)
{
  IRECT rc = *pRect;
  int width = pObj->GetSelectBarBorderWidth(pObj);
  //
  if (bTab == FALSE || RCW(pRect) < 75 || RCH(pRect) < 2 * iTabHeight) {
    iRectDeflate2(&rc, width, width);
    //
    //im->ExcludeClipRect( &rc );
    pObj->DrawSelectBarBorder(pObj, im, pclip, pRect);
    //im->SelectClipRgn( NULL, RGN_COPY );
    pObj->DrawSelectBarInside(pObj, im, pclip, &rc);
  }
  else {
    POINT pts1[ 8 ], pts2[ 8 ];
    pObj->GetOuterOutline(pObj, &rc, bTop, iTabHeight, width, pts1);
    pObj->GetInnerOutline(pObj, &rc, bTop, iTabHeight, width, pts2);
    //
    //CRgn rgn1, rgn2;
    //rgn1.CreatePolygonRgn( pts1, sizeof( pts1 ) / sizeof( POINT ), WINDING );
    //rgn2.CreatePolygonRgn( pts2, sizeof( pts2 ) / sizeof( POINT ), WINDING );
    //
    //im->SelectClipRgn( &rgn1 );
    //im->SelectClipRgn( &rgn2, RGN_DIFF );
    //DrawSelectBarBorder( pObj, im, &rc );
    //im->SelectClipRgn( &rgn2, RGN_COPY );
    //DrawSelectBarInside( pObj, im, &rc );
    //im->SelectClipRgn( NULL, RGN_COPY );
  }
  return 0;
}
//
static int DockingMarkersDrawBase_DrawSelectFrame(DockingMarkers* pObj, img_t* im, const IRECT* pclip, IRECT const* pRectNew, IRECT const* pRectOld, BOOL bTab, BOOL bTop, int iTabHeight)
{
  int width = pObj->GetSelectFrameWidth(pObj);
  IRECT rc = (pRectNew != NULL ? *pRectNew : *pRectOld);
  //
  if (bTab == FALSE || RCW(&rc) < 75 || RCH(&rc) < 2 * iTabHeight) {
    IRECT rcClip = rc;
    iRectDeflate2(&rcClip, width, width);
    //im->ExcludeClipRect( &rcClip );
    pObj->DrawSelectFrame_Impl(pObj, im, pclip, &rc);
  }
  else {
    POINT pts1[ 8 ], pts2[ 8 ];
    pObj->GetOuterOutline(pObj, &rc, bTop, iTabHeight, width, pts1);
    pObj->GetInnerOutline(pObj, &rc, bTop, iTabHeight, width, pts2);
    //
    //CRgn rgn1, rgn2;
    //rgn1.CreatePolygonRgn( pts1, sizeof( pts1 ) / sizeof( POINT ), WINDING );
    //rgn2.CreatePolygonRgn( pts2, sizeof( pts2 ) / sizeof( POINT ), WINDING );
    //
    //im->SelectClipRgn( &rgn1 );
    //im->SelectClipRgn( &rgn2, RGN_DIFF );
    pObj->DrawSelectFrame_Impl(pObj, im, pclip, &rc);
  }
  //
  //im->SelectClipRgn( NULL, RGN_COPY );
  return 0;
}
//
static int DockingMarkersDrawBase_GetOuterOutline(DockingMarkers* pObj, IRECT const* rect, BOOL top, int tabHeight, int width, POINT pts[ 8 ] /*out*/)
{
  int w = (width != 1 ? width / 2 : 1);
  //
  if (top == TRUE) {
    pts[ 0 ].x = rect->l;
    pts[ 0 ].y = rect->b;
    pts[ 1 ].x = rect->l;
    pts[ 1 ].y = rect->t + tabHeight - w;
    pts[ 2 ].x = rect->l + 15;
    pts[ 2 ].y = rect->t + tabHeight - w;
    pts[ 3 ].x = rect->l + 15;
    pts[ 3 ].y = rect->t;
    pts[ 4 ].x = rect->l + 60;
    pts[ 4 ].y = rect->t;
    pts[ 5 ].x = rect->l + 60;
    pts[ 5 ].y = rect->t + tabHeight - w;
    pts[ 6 ].x = rect->r;
    pts[ 6 ].y = rect->t + tabHeight - w;
    pts[ 7 ].x = rect->r;
    pts[ 7 ].y = rect->b;
  }
  else {
    pts[ 0 ].x = rect->l;
    pts[ 0 ].y = rect->t;
    pts[ 1 ].x = rect->l;
    pts[ 1 ].y = rect->b - tabHeight + w;
    pts[ 2 ].x = rect->l + 15;
    pts[ 2 ].y = rect->b - tabHeight + w;
    pts[ 3 ].x = rect->l + 15;
    pts[ 3 ].y = rect->b;
    pts[ 4 ].x = rect->l + 60;
    pts[ 4 ].y = rect->b;
    pts[ 5 ].x = rect->l + 60;
    pts[ 5 ].y = rect->b - tabHeight + w;
    pts[ 6 ].x = rect->r;
    pts[ 6 ].y = rect->b - tabHeight + w;
    pts[ 7 ].x = rect->r;
    pts[ 7 ].y = rect->t;
  }
  return 0;
}
//
static int DockingMarkersDrawBase_GetInnerOutline(DockingMarkers* pObj, IRECT const* rect, BOOL top, int tabHeight, int width, POINT pts[ 8 ] /*out*/)
{
  int w = width / 2;
  //
  if (top == TRUE) {
    pts[ 0 ].x = rect->l + width;
    pts[ 0 ].y = rect->b - width;
    pts[ 1 ].x = rect->l + width;
    pts[ 1 ].y = rect->t + tabHeight + w;
    pts[ 2 ].x = rect->l + 15 + width;
    pts[ 2 ].y = rect->t + tabHeight + w;
    pts[ 3 ].x = rect->l + 15 + width;
    pts[ 3 ].y = rect->t + width;
    pts[ 4 ].x = rect->l + 60 - width;
    pts[ 4 ].y = rect->t + width;
    pts[ 5 ].x = rect->l + 60 - width;
    pts[ 5 ].y = rect->t + tabHeight + w;
    pts[ 6 ].x = rect->r - width;
    pts[ 6 ].y = rect->t + tabHeight + w;
    pts[ 7 ].x = rect->r - width;
    pts[ 7 ].y = rect->b - width;
  }
  else {
    pts[ 0 ].x = rect->l + width;
    pts[ 0 ].y = rect->t + width;
    pts[ 1 ].x = rect->l + width;
    pts[ 1 ].y = rect->b - tabHeight - w;
    pts[ 2 ].x = rect->l + 15 + width;
    pts[ 2 ].y = rect->b - tabHeight - w;
    pts[ 3 ].x = rect->l + 15 + width;
    pts[ 3 ].y = rect->b - width;
    pts[ 4 ].x = rect->l + 60 - width;
    pts[ 4 ].y = rect->b - width;
    pts[ 5 ].x = rect->l + 60 - width;
    pts[ 5 ].y = rect->b - tabHeight - w;
    pts[ 6 ].x = rect->r - width;
    pts[ 6 ].y = rect->b - tabHeight - w;
    pts[ 7 ].x = rect->r - width;
    pts[ 7 ].y = rect->t + width;
  }
  return 0;
}
static int DockingMarkersDrawBase_set(DockingMarkers* s)
{
  CLASSFUN_INIT(s, DockingMarkersDrawBase, DrawMarker);
  CLASSFUN_INIT(s, DockingMarkersDrawBase, DrawSelectWindow);
  CLASSFUN_INIT(s, DockingMarkersDrawBase, DrawSelectFrame);
  CLASSFUN_INIT(s, DockingMarkersDrawBase, GetSelectBarBorderWidth);
  CLASSFUN_INIT(s, DockingMarkersDrawBase, GetSelectFrameWidth);
  CLASSFUN_INIT(s, DockingMarkersDrawBase, GetSelectBarInsideColor);
  CLASSFUN_INIT(s, DockingMarkersDrawBase, GetSelectBarBorderColor);
  CLASSFUN_INIT(s, DockingMarkersDrawBase, DrawSelectBarBorder);
  CLASSFUN_INIT(s, DockingMarkersDrawBase, DrawSelectBarInside);
  CLASSFUN_INIT(s, DockingMarkersDrawBase, GetOuterOutline);
  CLASSFUN_INIT(s, DockingMarkersDrawBase, GetInnerOutline);
  CLASSFUN_INIT(s, DockingMarkersDrawBase, DrawSelectFrame_Impl);
  return 0;
}
// DockingMarkersDraw_custom1.
//
static int DockingMarkersDraw_custom1_GetSelectBarBorderWidth(DockingMarkers* pObj)
{
  return 1;
}
//
static int DockingMarkersDraw_custom1_GetSelectFrameWidth(DockingMarkers* pObj)
{
  return 2;
}
//
static int DockingMarkersDraw_custom1_DrawSelectBarInside(DockingMarkers* pObj, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  //CBrush brush;
  //brush.CreateHatchBrush( HS_BDIAGONAL, _RGB( 110, 110, 110 ) );
  //im->SetBkColor( _RGB( 255, 128, 0 ) );
  //im->FillRect( pRect, &brush );
  imdraw_FillSolidRect(im, pclip, pRect, _RGB(110, 110, 110));
  return 0;
}
//
static int DockingMarkersDraw_custom1_DrawSelectFrame_Impl(DockingMarkers* pObj, img_t* im, const IRECT* pclip, IRECT const* pRect)
{
  //CBrush br( HS_BDIAGONAL, _GetSysColor( CLR_WINDOWTEXT ) );
  //
  //CBrush *pOldBrush = im->SelectObject( &br );
  //im->PatBlt( pRect->l, pRect->t, pRect RCW(), pRect RCH(), PATINVERT );
  //im->SelectObject( pOldBrush );
  imdraw_FillSolidRect(im, pclip, pRect, CLR_WINDOWTEXT);
  return 0;
}
//
static COLOR DockingMarkersDraw_custom1_GetSelectBarBorderColor(DockingMarkers* pObj)
{
  return _RGB(60, 60, 60);
}
static int DockingMarkersDraw_custom1_set(DockingMarkers* s)
{
  DockingMarkersDrawBase_set(s);
  CLASSFUN_INIT(s, DockingMarkersDraw_custom1, GetSelectBarBorderWidth);
  CLASSFUN_INIT(s, DockingMarkersDraw_custom1, GetSelectFrameWidth);
  CLASSFUN_INIT(s, DockingMarkersDraw_custom1, DrawSelectBarInside);
  CLASSFUN_INIT(s, DockingMarkersDraw_custom1, DrawSelectFrame_Impl);
  CLASSFUN_INIT(s, DockingMarkersDraw_custom1, GetSelectBarBorderColor);
  return 0;
}
// DockingMarkersDraw_VS2003.
//
static int DockingMarkersDraw_VS2003_GetSelectFrameWidth(DockingMarkers* pObj)
{
  return 8;
}
static int DockingMarkersDraw_VS2003_set(DockingMarkers* s)
{
  DockingMarkersDrawBase_set(s);
  CLASSFUN_INIT(s, DockingMarkersDraw_VS2003, GetSelectFrameWidth);
  return 0;
}
// DockingMarkersDraw_VS2005.
//
static int DockingMarkersDraw_VS2005_GetSelectBarBorderWidth(DockingMarkers* pObj)
{
  return 2;
}
static int DockingMarkersDraw_VS2005_set(DockingMarkers* s)
{
  DockingMarkersDrawBase_set(s);
  CLASSFUN_INIT(s, DockingMarkersDraw_VS2005, GetSelectBarBorderWidth);
  return 0;
}
// DockingMarkersDraw_VS2010.
//
static int DockingMarkersDraw_VS2010_GetSelectBarBorderWidth(DockingMarkers* pObj)
{
  return 5;
}
//
static COLOR DockingMarkersDraw_VS2010_GetSelectBarInsideColor(DockingMarkers* pObj)
{
  return PixelAlpha_100(_GetSysColor(CLR_HIGHLIGHT), _RGB(255, 255, 255), 80);
}
//
static int DockingMarkersDraw_VS2010_GetOuterOutline(DockingMarkers* pObj, IRECT const* rect, BOOL top, int tabHeight, int width, POINT pts[ 8 ] /*out*/)
{
  if (top == TRUE) {
    pts[ 0 ].x = rect->l;
    pts[ 0 ].y = rect->b;
    pts[ 1 ].x = rect->l;
    pts[ 1 ].y = rect->t;
    pts[ 2 ].x = rect->l;
    pts[ 2 ].y = rect->t; // repeat.
    pts[ 3 ].x = rect->l;
    pts[ 3 ].y = rect->t; // repeat.
    pts[ 4 ].x = rect->l + 60;
    pts[ 4 ].y = rect->t;
    pts[ 5 ].x = rect->l + 60;
    pts[ 5 ].y = rect->t + tabHeight;
    pts[ 6 ].x = rect->r;
    pts[ 6 ].y = rect->t + tabHeight;
    pts[ 7 ].x = rect->r;
    pts[ 7 ].y = rect->b;
  }
  else {
    pts[ 0 ].x = rect->l;
    pts[ 0 ].y = rect->t;
    pts[ 1 ].x = rect->l;
    pts[ 1 ].y = rect->b;
    pts[ 2 ].x = rect->l;
    pts[ 2 ].y = rect->b; // repeat.
    pts[ 3 ].x = rect->l;
    pts[ 3 ].y = rect->b; // repeat.
    pts[ 4 ].x = rect->l + 60;
    pts[ 4 ].y = rect->b;
    pts[ 5 ].x = rect->l + 60;
    pts[ 5 ].y = rect->b - tabHeight;
    pts[ 6 ].x = rect->r;
    pts[ 6 ].y = rect->b - tabHeight;
    pts[ 7 ].x = rect->r;
    pts[ 7 ].y = rect->t;
  }
  return 0;
}
//
static int DockingMarkersDraw_VS2010_GetInnerOutline(DockingMarkers* pObj, IRECT const* rect, BOOL top, int tabHeight, int width, POINT pts[ 8 ] /*out*/)
{
  if (top == TRUE) {
    pts[ 0 ].x = rect->l + width;
    pts[ 0 ].y = rect->b - width;
    pts[ 1 ].x = rect->l + width;
    pts[ 1 ].y = rect->t + width;
    pts[ 2 ].x = rect->l + width;
    pts[ 2 ].y = rect->t + width; // repeat.
    pts[ 3 ].x = rect->l + width;
    pts[ 3 ].y = rect->t + width; // repeat.
    pts[ 4 ].x = rect->l + 60 - width;
    pts[ 4 ].y = rect->t + width;
    pts[ 5 ].x = rect->l + 60 - width;
    pts[ 5 ].y = rect->t + tabHeight + width;
    pts[ 6 ].x = rect->r - width;
    pts[ 6 ].y = rect->t + tabHeight + width;
    pts[ 7 ].x = rect->r - width;
    pts[ 7 ].y = rect->b - width;
  }
  else {
    pts[ 0 ].x = rect->l + width;
    pts[ 0 ].y = rect->t + width;
    pts[ 1 ].x = rect->l + width;
    pts[ 1 ].y = rect->b - width;
    pts[ 2 ].x = rect->l + width;
    pts[ 2 ].y = rect->b - width; // repeat.
    pts[ 3 ].x = rect->l + width;
    pts[ 3 ].y = rect->b - width; // repeat.
    pts[ 4 ].x = rect->l + 60 - width;
    pts[ 4 ].y = rect->b - width;
    pts[ 5 ].x = rect->l + 60 - width;
    pts[ 5 ].y = rect->b - tabHeight - width;
    pts[ 6 ].x = rect->r - width;
    pts[ 6 ].y = rect->b - tabHeight - width;
    pts[ 7 ].x = rect->r - width;
    pts[ 7 ].y = rect->t + width;
  }
  return 0;
}
static int DockingMarkersDraw_VS2010_set(DockingMarkers* s)
{
  DockingMarkersDrawBase_set(s);
  CLASSFUN_INIT(s, DockingMarkersDraw_VS2010, GetSelectBarBorderWidth);
  CLASSFUN_INIT(s, DockingMarkersDraw_VS2010, GetSelectBarInsideColor);
  CLASSFUN_INIT(s, DockingMarkersDraw_VS2010, GetOuterOutline);
  CLASSFUN_INIT(s, DockingMarkersDraw_VS2010, GetInnerOutline);
  return 0;
}

