
#define SCROLLLEFT  0x889
#define SCROLLRIGHT 0x890
//****************************************************************

class  CWorkTab 
{

  CScrollBar    m_spin;
  CNormalButton m_toolBtn[2];
  CScrollBar    m_scroll;
  BOOL      m_PresRight;
  BOOL      m_PresLeft;
  int       m_deplaza;
  CRect     m_btndesplaza;
  BOOL      bIniciaArrastre;
  CDrawLayer  m_wd;
  BOOL      m_EnableL;
  BOOL      m_EnableR;
  CImageCtrl    m_img;
protected:


#define ID_WTABSPIN   501
#define ID_SCROLLWINDOW 0x9666

#define  SPACE_TAB    4
#define  DEFAULT_FLAT 18

//////////////////////////////////////////////////////////////////////
// Construction/Destruction

#define MIN_SCROLL ::GetSystemMetrics(SM_CXHSCROLL)*2


CWorkTab::CWorkTab()
{
  SetStyle(CContainer::SHADOW);
  SetALingTabs(CContainer::ALN_BOTTOM);
  m_deplaza = 0;
  m_btndesplaza.SetRectEmpty();
  bIniciaArrastre = FALSE;
  m_EnableL = FALSE;
  m_EnableR = FALSE;
}

CWorkTab::~CWorkTab()
{

}



void CWorkTab::PintaSeparador(CGDI* dc)
{
  IRECT m_rClient = m_rectTab;

  // se pinta el boton de desplazamiento
  if (m_btndesplaza.IsRectNull()) {
    m_rClient.l    = m_rClient.Width() / 2;
    m_rClient.t    -= 3;
    m_rClient.l   -= 5;
    m_rClient.r   = m_rClient.l + 5;
    m_rClient.b -= 1;
    m_btndesplaza     = m_rClient;
    m_scroll.SetWindowPos(NULL, m_btndesplaza.r,
        m_rectTab.t, m_rectTab.r - (m_rClient.l + 5), m_rectTab.Height() - 1,
        SWP_NOACTIVATE | SWP_NOCOPYBITS);

    m_scroll.Invalidate();
    m_scroll.UpdateWindow();

    return;
  }
  else {
    IRECT rcCli;
    GetClientRect(&rcCli);

    if (m_btndesplaza.l + 40 > rcCli.r) {
      m_btndesplaza.l = rcCli.r - 40;
      m_btndesplaza.r = m_btndesplaza.l + 5;
    }

    m_btndesplaza.t = m_rectTab.t;
    m_btndesplaza.b = m_rectTab.b;
    m_rClient = m_btndesplaza;
  }

  if (!bIniciaArrastre) {
    dc->Draw3dRect(m_rClient, _GetSysColor(BLACK_PEN),
        _GetSysColor(CLR_BTNHIGHLIGHT));
    m_rClient iRectDeflate2(&, 1, 1);
    dc->Draw3dRect(m_rClient, _GetSysColor(CLR_BTNHIGHLIGHT),
        _GetSysColor(CLR_BTNSHADOW));
  }
}


void CWorkTab::OnSpiNiz(WPARAM wParam, LPARAM lParam, HWND hWnd)
{
  if (m_EnableL == TRUE) {
    m_PresLeft   = TRUE;
    m_PresRight  = FALSE;
    m_deplaza   += 10;
    CClientDC dc(this);
    Drawtabs(&dc);
  }
}
void CWorkTab::OnSpiNde(WPARAM wParam, LPARAM lParam, HWND hWnd)
{
  if (m_EnableR == TRUE) {
    m_PresLeft = FALSE;
    m_PresRight = TRUE;
    m_deplaza -= 10;
    CClientDC dc(this);
    Drawtabs(&dc);
    dc.DeleteDC();
  }
}


BOOL CWorkTab::OnPaint(HDC hDC)
{
  CPaintDC dc(this); // device context for painting
  COLOR cbr;
  IRECT m_rectDraw;
  cbr = (GetRGBColorXP());

  GetClientRect(m_rectCliente);
  m_rectTab = m_rectCliente;
  m_rectDraw = m_rectCliente;

  if (m_alnTab == ALN_BOTTOM) {
    m_rectDraw.b = m_rectCliente.b - (theApp->m_WinMain->m_sizeImagDocks.h + SPACE_TAB + 2);
  }

  IRECT rc = m_rectDraw;
  rc.t += 1;
  rc.b = rc.t + 1;
  dc.Draw3dRect(rc, GetRGBColorFace(), GetRGBColorFace());

  if (m_alnTab == ALN_BOTTOM) {
    m_rectCliente.b = m_rectCliente.b - (DEFAULT_FLAT);
    m_rectTab.t = m_rectCliente.b + 2;
  }

  cbr.DeleteObject();
  Drawtabs(&dc);
  dc.DeleteDC();
  return TRUE;
}

void CWorkTab::Drawtabs(CGDI* dc)
{
  int m_StrPos      = 0;
  BOOL m_ViewAlltab = TRUE;
  IRECT m_rClient;
  COLOR cbr;
  COLOR cp;
  cp = (_GetSysColor(CLR_BTNSHADOW));
  int m_ianchoScroll =::GetSystemMetrics(SM_CXHSCROLL) * 2;
  m_EnableR = TRUE;
  m_EnableL = TRUE;
  cbr = (m_wd.GetRGBColorXP());
  GetClientRect(m_rClient);
  IRECT rectT = m_rectTab;
  rectT.r = m_btndesplaza.l;
  rectT.r += 4;
  dc->FillRect(rectT, &cbr);
  HPEN hPen = dc->SelectObject(&cp);
  dc->MoveTo(m_rectTab.l, m_rectTab.t);
  dc->LineTo(m_btndesplaza.r + 1, m_rectTab.t);
  dc->Draw3dRect(m_rClient, _GetSysColor(CLR_BTNSHADOW),
      _GetSysColor(CLR_BTNHIGHLIGHT));

  if (!::IsWindow(m_toolBtn[0].GetSafeHwnd())) {
    IRECT r = m_rectTab;
    r.r = r.l + 35;
    r.t += 1;

    // r.b-=1;
    if (!m_img.Create(IDB_GUI_DOCKBAR, 9, 11, RGB(255, 0, 255))) {
      //TRACE0("error imagelist");
    }

    m_toolBtn[0].Create(GetSafeHwnd(), SCROLLLEFT, _T(""),
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
        IRECT(0, 0, 0, 0));
    m_toolBtn[0].SetToolTip(_T("Scroll Left"));
    m_toolBtn[0].SethIcon(m_img.ExtractIcon(4));
    m_toolBtn[0].ShowDark(FALSE);
    m_toolBtn[0].SetScrollButton();
    m_toolBtn[1].Create(GetSafeHwnd(), SCROLLRIGHT, _T(""),
        WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_OWNERDRAW,
        IRECT(0, 0, 0, 0));
    m_toolBtn[1].SetToolTip(_T("Scroll Right"));
    m_toolBtn[1].SethIcon(m_img.ExtractIcon(2));
    m_toolBtn[1].ShowDark(FALSE);
    m_toolBtn[1].SetScrollButton();
  }

  if (!::IsWindow(m_scroll.GetSafeHwnd())) {
    m_scroll.Create(GetSafeHwnd(), 0L, NULL, WS_CHILD | WS_VISIBLE | SBS_HORZ, m_rectTab);
  }

  CSize szBtn_1 = m_toolBtn[0].GetSizeButton();
  CSize szBtn_2 = m_toolBtn[0].GetSizeButton();

  IRECT rcSpin = IRECT(0, 0, szBtn_1.w + szBtn_2.w + 2, szBtn_1.h + szBtn_2.h);
  m_StrPos = rcSpin.Width();

  if ((m_btndesplaza.l < rcSpin.r) && ! m_btndesplaza.IsRectEmpty()) {
    m_btndesplaza.l = rcSpin.r + 5;
    m_btndesplaza.r = m_btndesplaza.l + 5;
  }

  PintaSeparador(dc);

  int m_Space = 8;
  int m_Pos   = 10;
  int m_right = m_StrPos;

  if (m_iMinValRec - 26 > m_btndesplaza.l) { // No existe suficioente espacio para los tabs
    m_ViewAlltab = FALSE;

    // m_spin.EnableScrollBar();
    if (m_PresLeft == TRUE) {
      m_right += m_deplaza;
    }

    if (m_PresRight == TRUE) {
      m_right += m_deplaza;
    }

    if (m_deplaza == 0) {
      m_EnableL = FALSE;
      m_EnableR = TRUE;
      //m_spin.EnableScrollBar(ESB_DISABLE_LTUP);
    }

    if ((m_iMinValRec + m_deplaza) - 26 <= m_btndesplaza.l) {
      //m_spin.EnableScrollBar(ESB_DISABLE_RTDN);
      m_EnableR = FALSE;
      m_EnableL = TRUE;
    }
  }
  else {
    m_EnableL = FALSE;
    m_EnableR = FALSE;
    // m_spin.EnableScrollBar(ESB_DISABLE_BOTH);
    m_ViewAlltab = TRUE;
    m_deplaza = 0;
  }

  IRECT rectScroll;
  IRECT rectRecorte;
  rectRecorte.l = m_rectTab.l + m_ianchoScroll;
  rectRecorte.t = m_rectTab.t;
  rectRecorte.b = m_rectTab.b;
  rectRecorte.r = m_btndesplaza.l;
  HRGN hRgn = CreateRectRgnIndirect(rectRecorte);
  dc->SelectClipRgn(hRgn);

  for (int iCont = 0; iCont < m_Numtabs; iCont++) {
    //;
    CTab* ctb = (CTab*)m_pArray[iCont];
    IRECT mrt = ctb->rect;
    int ancho = mrt.Width();
    mrt.l = m_right;
    mrt.r = mrt.l + ancho;
    IPOINT Coor[4];

    // dibujar la hoja seleccionada
    if (iCont == 0) {
      dc->MoveTo(mrt.l, mrt.t);
      dc->LineTo((mrt.l) + m_rectTab.Height() / 2, mrt.b - 1);
      dc->MoveTo((mrt.l) + m_rectTab.Height() / 2, mrt.b - 1);
      dc->LineTo((mrt.r) - m_rectTab.Height() / 2, mrt.b - 1);
      dc->MoveTo((mrt.r) - m_rectTab.Height() / 2, mrt.b - 1);

      if (m_iSelectTab == iCont) {
        dc->LineTo(mrt.r - 2, mrt.t + 2);
      }
      else {
        dc->LineTo((mrt.r) - 3, mrt.t + (m_rectTab.Height() / 2));
      }

      m_right = mrt.r;
    }
    else {
      // Se dibuja la hoja siguiente a la seleccionada
      // no se dibuja toda la diagonal y no es la ultima hoja

      if (iCont - 1 == m_iSelectTab) {
        dc->MoveTo(mrt.l + (m_rectTab.Height() / 4) + 1, mrt.t + m_rectTab.Height() / 2);
        dc->LineTo((mrt.l) + m_rectTab.Height() / 2, mrt.b - 1);
        dc->MoveTo((mrt.l) + m_rectTab.Height() / 2, mrt.b - 1);
        dc->LineTo((mrt.r) - m_rectTab.Height() / 2, mrt.b - 1);
        dc->MoveTo((mrt.r) - m_rectTab.Height() / 2, mrt.b - 1);

        if (iCont == m_Numtabs - 1) {
          dc->LineTo((mrt.r - 2), mrt.t + 2);
        }
        else {
          dc->LineTo((mrt.r) - 3, mrt.t + (m_rectTab.Height() / 2));
        }
      }
      else {
        dc->MoveTo(mrt.l, mrt.t);
        dc->LineTo((mrt.l) + m_rectTab.Height() / 2, mrt.b - 1);
        dc->MoveTo((mrt.l) + m_rectTab.Height() / 2, mrt.b - 1);
        dc->LineTo((mrt.r) - m_rectTab.Height() / 2, mrt.b - 1);
        dc->MoveTo((mrt.r) - m_rectTab.Height() / 2, mrt.b - 1);

        if (iCont == m_iSelectTab || iCont == m_Numtabs - 1) {
          dc->LineTo((mrt.r - 2), mrt.t + 2);
        }
        else {
          dc->LineTo((mrt.r) - 3, mrt.t + 3 + (m_rectTab.Height() / 2));
        }
      }

      m_right = mrt.r;
    }

    if (iCont == m_iSelectTab) {
      Coor[0].x = mrt.l;
      Coor[0].y = mrt.t;
      Coor[1].x = (mrt.l) + m_rectTab.Height() / 2;
      Coor[1].y = mrt.b - 1;
      Coor[2].x = (mrt.r) - m_rectTab.Height() / 2;
      Coor[2].y = mrt.b - 1;
      Coor[3].x = (mrt.r - 1);
      Coor[3].y = mrt.t + 3;
      dc->Polygon(Coor, 4);
      dc->SelectStockObject(WHITE_PEN);
      dc->MoveTo(mrt.l + 3, mrt.t + 3);
      dc->LineTo(mrt.r - 1, mrt.t + 3);
      dc->MoveTo(mrt.l + 2, mrt.t + 2);
      dc->LineTo(mrt.r, mrt.t + 2);
      dc->SelectObject(&cp);
    }

    HFONT hFont = dc->SelectObject(CFont((HFONT)GetStockObject(DEFAULT_GUI_FONT)));
    dc->SetBkMode(TRANSPARENT);
    IRECT mrtt = mrt;
    mrtt.t += 2;
    dc->DrawText(ctb->lpMsg, mrtt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    ctb->rect = mrt;
    m_right -= (m_rectTab.Height() / 2);

    if (!bIniciaArrastre) {
      IRECT m_rectCli;
      GetClientRect(m_rectCli);

      if (m_btndesplaza.l < m_rectCli.l + rcSpin.l + 1) {
        m_btndesplaza.l = m_rectCli.l + rcSpin.l + 5;
        m_btndesplaza.r = m_btndesplaza.l + 5;
        PintaSeparador(dc);
      }

      m_scroll.SetWindowPos(NULL,
          m_btndesplaza.r, m_rectTab.t, m_rectCli.r - (m_btndesplaza.l + 5),
          m_rectTab.Height() - 1, SWP_NOACTIVATE | SWP_NOCOPYBITS);
      m_scroll.Invalidate();
      m_scroll.UpdateWindow();
      CSize szBtn = m_toolBtn[0].GetSizeButton();
      m_toolBtn[0].MoveWindow(m_rectTab.l + 1,
          m_rectTab.t + 1, szBtn.w, m_rectTab.Height() - 2);
      CSize szBtn1 = m_toolBtn[1].GetSizeButton();
      m_toolBtn[1].MoveWindow(m_rectTab.l + szBtn.w + 1, m_rectTab.t + 1,
          szBtn1.w, m_rectTab.Height() - 2);
    }

  }

  cbr.DeleteObject();
  dc->SelectClipRgn(NULL);
  ::DeleteObject(hRgn);
  cp.DeleteObject();
  m_toolBtn[0].Invalidate();
  m_toolBtn[0].UpdateWindow();
  m_toolBtn[1].Invalidate();
  m_toolBtn[1].UpdateWindow();
}


void CWorkTab::OnHScroll(UINT nSBCode, UINT nPos, HWND hScrollBar)
{
  // TODO: Add your message handler code here and/or call default

  Default();

  if (hScrollBar == m_scroll.GetSafeHwnd()) {
    CTab* ct = (CTab*) m_pArray[m_iSelectTab];
    CWin* ViewAc = reinterpret_cast<CWin*>(ct->pParent);
    ViewAc->SendMessage(WM_HSCROLL, MAKEWPARAM(nSBCode, nPos), 0);
    SizeScroll();

    return;
  }

  CClientDC dc(this);
  Drawtabs(&dc);
  dc.DeleteDC();
}



BOOL CWorkTab::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message)
{
  IPOINT ptCurPos;
  GetCursorPos(&ptCurPos);
  ScreenToClient(&ptCurPos);

  if (m_btndesplaza.PtInRect(ptCurPos)) {
    SetCursor(LoadCursor(NULL, IDC_SIZEWE));
    return TRUE;
  }

  SetCursor(LoadCursor(NULL, IDC_ARROW));
  return TRUE;
}

BOOL CWorkTab::OnLButtonDown(UINT nFlags, IPOINT point)
{
  // TODO: Add your message handler code here and/or call default
  if (m_btndesplaza.PtInRect(point) && !bIniciaArrastre) {
    bIniciaArrastre = TRUE;
    SetCapture();
    InvalidateRect(m_btndesplaza, TRUE);
  }
  else {
    CContainer::OnLButtonDown(nFlags, point);
    ReleaseCapture();
    CTab* ct = (CTab*) m_pArray[m_iSelectTab];
    CWin* ViewAc = reinterpret_cast<CWin*>(ct->pParent);
    int rangmin, rangMax;
    ct->pParent->GetScrollRange(SB_HORZ, &rangmin, &rangMax);
    m_scroll.SetScrollRange(SB_HORZ, rangmin, rangMax);
    m_scroll.SetScrollPos(SB_HORZ, ViewAc->GetScrollPos(SB_HORZ));
  }

  //Default();
  return TRUE;
}


BOOL CWorkTab::OnLButtonUp(UINT nFlags, IPOINT point)
{
  if (bIniciaArrastre == TRUE) {
    bIniciaArrastre = FALSE;
    int iDiferencia = m_rectTab.r - 31;

    if (point.x < m_rectTab.l + 31) {
      point.x = m_rectTab.l + 31;
    }

    if (point.x > iDiferencia) {
      point.x = iDiferencia;
    }

    m_btndesplaza.l = point.x;
    m_btndesplaza.r = m_btndesplaza.l + 5;
    ReleaseCapture();
    Invalidate();
    UpdateWindow();
  }

  Default();
  return TRUE;
}

void CWorkTab::OnMouseMove(UINT nFlags, IPOINT point)
{
  IRECT m_rectCli;
  GetClientRect(m_rectCli);

  if (bIniciaArrastre == TRUE) {
    int iDiferencia = m_rectCli.r - 31;

    if (point.x < m_rectTab.l + 31) {
      point.x = m_rectTab.l + 31;
    }

    if (point.x > iDiferencia) {
      point.x = iDiferencia;
    }

    m_scroll.SetWindowPos(NULL, point.x,
        m_rectTab.t, m_rectCli.r - point.x, m_rectTab.Height() - 1,
        SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOCOPYBITS);
    m_scroll.Invalidate();
    m_scroll.UpdateWindow();
    m_btndesplaza.l = point.x;
    m_rectCli = m_btndesplaza;
    m_rectCli.l -= 5;
    InvalidateRect(m_btndesplaza, TRUE);
    UpdateWindow();
  }
  else {
    Default();
  }
}

BOOL CWorkTab::OnSysColorChange()
{
  Default();

  m_toolBtn[0].SetColor(GetRGBColorFace());
  m_toolBtn[1].SetColor(GetRGBColorFace());
  m_toolBtn[0].Invalidate();
  m_toolBtn[0].UpdateWindow();
  m_toolBtn[1].Invalidate();
  m_toolBtn[1].UpdateWindow();

  Invalidate();
  UpdateWindow();
  return TRUE;
}

void CWorkTab::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
  Default();
  CTab* ct  = (CTab*) m_pArray[m_iSelectTab];
  CWin* ViewAc = ct->pParent;
}

BOOL CWorkTab::OnSize(UINT nType, int cx, int cy)
{
  CContainer::OnSize(nType, cx, cy);
  SizeScroll();
  return TRUE;
}


void CWorkTab::SizeScroll()
{
  SCROLLINFO sCrollInfo;
  ASSERT(this);

  if (!m_pArray.GetSize()) {
    return;
  }

  CTab* ct  = (CTab*) m_pArray[m_iSelectTab];
  CWin* ViewAc = reinterpret_cast<CWin*>(ct->pParent);

  memset(&sCrollInfo, 0, sizeof(SCROLLINFO));
  sCrollInfo.cbSize = sizeof(SCROLLINFO);
  sCrollInfo.fMask  = SIF_ALL;

  ViewAc->GetScrollInfo(SB_HORZ, &sCrollInfo);

  if (::IsWindow(m_scroll.GetSafeHwnd())) {
    m_scroll.SetScrollInfo(SB_HORZ, &sCrollInfo);
  }

  ViewAc->ShowScrollBar(SB_HORZ, FALSE);
  ViewAc->ModifyStyle(WS_HSCROLL, 0, SWP_DRAWFRAME);
}

int CWorkTab::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (CContainer::OnCreate(lpCreateStruct) == -1) {
    return -1;
  }

  return 0;
}
};
