

// CSplitter

enum TYPEALING {ALINGHORZ = 0, ALINGVERT = 1};
  enum STYLESEPARATOR {STYLE3D = 0, STYLEFLAT = 1};
class  CSplitter : public CWin
{
public:
  COLORREF  clrBg;
  CWin*   pWndLeft;
  CWin*   pWndRight;
  TYPEALING TypeAling;
  CRect   m_rcSpplitter;
  int     m_nSizePix;
  CRect   m_rcSpplitterOld;
  BOOL    bIniciaArrastre;
  STYLESEPARATOR mStyleSeparator;
  COLORREF  mclrLeftTop;
  COLORREF  mclrBottomRight;
  CRect   m_rectLeft;
  CRect   m_rectRight;
  
  CWin* GetLeft() {
    if (HIWORD(pWndLeft)) {
      return pWndLeft;
    }
    
    return NULL;
  }
  CWin* GetRight() {
    if (HIWORD(pWndRight)) {
      return pWndRight;
    }
    
    return NULL;
  }
  void SetStyleSeparator(STYLESEPARATOR StyleSeparator = STYLEFLAT, COLORREF clrLeftTop = CDrawLayer::GetRGBColorShadow(),
    COLORREF clrBottomRight = CDrawLayer::GetRGBColorShadow());


// CSplitter



CSplitter::CSplitter()
{
  TypeAling = ALINGHORZ;
  m_nSizePix = 2;
  mStyleSeparator = STYLE3D;
  bIniciaArrastre = FALSE;
  clrBg = GetRGBColorFace();
  mclrLeftTop = GetRGBColorBTNHigh();
  mclrBottomRight = GetRGBColorShadow();
  m_rectLeft = IRECT(0, 0, 0, 0);
  m_rectRight = IRECT(0, 0, 0, 0);
}

CSplitter::~CSplitter()
{
}



void CSplitter::SeTypeALing(TYPEALING typeAling)
{
  TypeAling = typeAling;
  m_rcSpplitter = IRECT(0, 0, 0, 0);
  Invalidate();
  UpdateWindow();
}


// CSplitter message handlers
void CSplitter::SetColor(COLORREF m_clrface)
{
  clrBg = m_clrface;
}

int CSplitter::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  // TODO:  Add your specialized creation code here

  return 0;
}


BOOL CSplitter::Create(CWin* pParentWnd)
{
  // TODO: Add your specialized code here and/or call the base class
  LPTSTR lpclassname = RegisterDefaultClass(NULL, WS_CHILD | WS_VISIBLE);
  return CWin::Create(lpclassname, NULL, WS_VISIBLE | WS_CHILD, IRECT(0, 0, 0, 0), pParentWnd->GetSafeHwnd(), 0xffff);
}


BOOL CSplitter::OnPaint(HDC hDC)
{
  CPaintDC dc(GetSafeHwnd()); // device context for painting
  COLOR cbr;
  IRECT rcClient;
  GetClientRect(&rcClient);
  cbr = (clrBg);

  CGDI    MemDC;
  CBitmap   m_BitmapMemDC;
  MemDC.CreateCompatibleDC(dc.m_hDC);
  m_BitmapMemDC.CreateCompatibleBitmap(dc.m_hDC, rcClient.Width(), rcClient.Height());

  CBitmap* m_bitmapOld = new CBitmap(MemDC.SelectObject(&m_BitmapMemDC));

  MemDC.FillRect(&rcClient, &cbr);
  DrawSplitter(m_rcSpplitter, m_rectLeft, m_rectRight, &MemDC);

  dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), MemDC.m_hDC,
      rcClient.l, rcClient.t, SRCCOPY);
  MemDC.SelectObject(m_bitmapOld);
  m_BitmapMemDC.DeleteObject();
  MemDC.DeleteDC();
  cbr.DeleteObject();
  dc.DeleteDC();
  m_bitmapOld->DeleteObject();
  delete m_bitmapOld;
  return TRUE;
}

void CSplitter::SetPixelWidht(int nSizePix)
{
  m_nSizePix = nSizePix;
}

void  CSplitter::AddLeftCtrl(CWin* pWnd, long MinSize)
{
  ASSERT(pWnd);
  pWndLeft = pWnd;
}

void  CSplitter::AddRightCtrl(CWin* pWnd, long MinSize)
{
  ASSERT(pWnd);
  pWndRight = pWnd;
  RecalLayout();
}

void CSplitter::OnLButtonDown(UINT nFlags, IPOINT point)
{
  // TODO: Add your message handler code here and/or call default
  if (m_rcSpplitter.PtInRect(point) && !bIniciaArrastre) {
    bIniciaArrastre = TRUE;
    SetCapture();
    InvalidateRect(m_rcSpplitter, TRUE);
  }
}

void CSplitter::OnLButtonUp(UINT nFlags, IPOINT pt)
{
  // TODO: Add your message handler code here and/or call default
  if (bIniciaArrastre == TRUE) {
    bIniciaArrastre = FALSE;
    RecalLayout();
    ReleaseCapture();
    Invalidate();
  }
}



void CSplitter::OnMouseMove(UINT nFlags, IPOINT point)
{
  // TODO: Add your message handler code here and/or call default
  IRECT m_rectCli;
  GetClientRect(m_rectCli);

  if (bIniciaArrastre == TRUE) {
    m_rcSpplitterOld = m_rcSpplitter;

    if (TypeAling == ALINGHORZ) {
      if (point.x < m_rectCli.l) {
        m_rcSpplitter.l = m_rectCli.l + m_nSizePix;
      }
      else if (point.x > m_rectCli.r) {
        m_rcSpplitter.l = m_rectCli.r - m_nSizePix;
      }
      else {
        m_rcSpplitter.l = point.x;
        m_rcSpplitter.r = point.x + m_nSizePix;
      }
    }
    else {
      if (point.y < m_rectCli.t) {
        m_rcSpplitter.t = m_rectCli.t + m_nSizePix;
      }
      else if (point.y > m_rectCli.b) {
        m_rcSpplitter.b = m_rectCli.t - m_nSizePix;
      }
      else {
        m_rcSpplitter.t = point.y;
        m_rcSpplitter.b = point.y + m_nSizePix;
      }
    }

    CClientDC dc(GetSafeHwnd());
    InvalidateRect(m_rcSpplitterOld);
    RecalLayout();
    dc.DeleteDC();
  }
}

BOOL CSplitter::OnSize(UINT nType, int cx, int cy)
{
  RecalLayout();
  return TRUE;
  // TODO: Add your message handler code here
}

void  CSplitter::RecalLayout()
{
  IRECT m_rectCli;
  IRECT m_rctemp;
  GetClientRect(m_rectCli);

  if (m_rectCli.IsRectEmpty()) {
    return;
  }

  if ((pWndLeft == NULL || pWndRight == NULL) ||
      (pWndLeft->IsWindowVisible() == FALSE || pWndRight->IsWindowVisible() == FALSE)) {
    if (pWndLeft == NULL || pWndRight == NULL) {
      if (pWndLeft == NULL && pWndRight == NULL) {
        return;
      }
      else if (pWndLeft == NULL) {
        if (pWndLeft->IsWindowVisible()) {
          pWndLeft->MoveWindow(m_rectCli);
        }
      }
      else {
        if (pWndRight->IsWindowVisible()) {
          pWndRight->MoveWindow(m_rectCli);
        }
      }
    }
    else if (pWndLeft->IsWindowVisible() == FALSE || pWndRight->IsWindowVisible() == FALSE) {
      if (pWndLeft->IsWindowVisible() == FALSE && pWndRight->IsWindowVisible() == FALSE) {
        return;
      }
      else if (pWndLeft->IsWindowVisible()) {
        if (pWndLeft != NULL) {
          pWndLeft->MoveWindow(m_rectCli);
        }
      }
      else {
        if (pWndRight != NULL) {
          pWndRight->MoveWindow(m_rectCli);
        }
      }
    }

    ::SendMessage(GetParent(), WM_SIZE, 0, 0);
    return ;
  }

  if (m_rcSpplitter.l < m_rectCli.l) {
    m_rcSpplitter.l = m_rectCli.l + 1;
  }

  if (m_rcSpplitter.r > m_rectCli.r) {
    m_rcSpplitter.r = m_rectCli.r - 1;
  }

  if (m_rcSpplitter.b > m_rectCli.b) {
    m_rcSpplitter.b = m_rectCli.b - 1;
  }

  if (m_rcSpplitter.t < m_rectCli.t) {
    m_rcSpplitter.t = m_rectCli.t + 1;
  }

  if (m_rcSpplitter.IsRectEmpty() || m_rcSpplitter.b >= 5000) {
    m_rcSpplitter = m_rectCli;

    if (TypeAling == ALINGHORZ) {
      m_rcSpplitter.r = m_rectCli.Width() / 2;
      m_rcSpplitter.l = m_rcSpplitter.r - m_nSizePix;
    }
    else {
      m_rcSpplitter.b = m_rectCli.Height() / 2;
      m_rcSpplitter.t = m_rcSpplitter.b - m_nSizePix;
    }
  }

  if (TypeAling == ALINGHORZ) {
    m_rcSpplitter.r = m_rcSpplitter.l + m_nSizePix;
    m_rcSpplitter.b = m_rectCli.b;
  }
  else {
    m_rcSpplitter.b = m_rcSpplitter.t + m_nSizePix;
    m_rcSpplitter.r = m_rectCli.r;
  }

  m_rctemp = m_rectCli;

  if (TypeAling == ALINGHORZ) {
    m_rctemp.r = m_rcSpplitter.l - 1;
  }
  else {
    m_rctemp.b = m_rcSpplitter.t - 1;
  }

  m_rectLeft = m_rctemp;
  m_rctemp iRectDeflate2(&, 1, 1);
  pWndLeft->MoveWindow(m_rctemp);
  pWndLeft->Invalidate();
  pWndLeft->UpdateWindow();
  m_rctemp = m_rectCli;

  if (TypeAling == ALINGHORZ) {
    m_rctemp.l = m_rcSpplitter.r + 1;
  }
  else {
    m_rctemp.t = m_rcSpplitter.b + 1;
  }

  m_rectRight = m_rctemp;
  m_rctemp iRectDeflate2(&, 1, 1);
  pWndRight->MoveWindow(m_rctemp);
  pWndRight->Invalidate();
  pWndRight->UpdateWindow();


}

BOOL CSplitter::OnSysColorChange()
{
  // TODO: Add your message handler code here
  return TRUE;
}

BOOL CSplitter::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message)
{
  // TODO: Add your message handler code here and/or call default
  IPOINT ptCurPos;
  GetCursorPos(&ptCurPos);
  ScreenToClient(&ptCurPos);

  if (m_rcSpplitter.PtInRect(ptCurPos)) {
    if (TypeAling == ALINGHORZ) {
      SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
    }
    else if (TypeAling == ALINGVERT) {
      SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
    }

    return TRUE;
  }

  SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
  return TRUE;
}

void CSplitter::SetStyleSeparator(STYLESEPARATOR StyleSeparator, COLORREF clrLeftTop, COLORREF clrBottomRight)

{
  mStyleSeparator = StyleSeparator;
  mclrLeftTop = clrLeftTop;
  mclrBottomRight = clrBottomRight;
}

void CSplitter::DrawSplitter(IRECT rcSeparator, IRECT rcLeft, IRECT rcRight, CGDI* pDC)
{
  if (mStyleSeparator == STYLE3D) {
    imdraw_Draw3dRect(im, pclip, rcLeft, mclrLeftTop, mclrBottomRight);
    imdraw_Draw3dRect(im, pclip, rcRight, mclrLeftTop, mclrBottomRight);
  }
  else {
    imdraw_Draw3dRect(im, pclip, rcLeft, GetRGBColorShadow(), GetRGBColorShadow());
    imdraw_Draw3dRect(im, pclip, rcRight, GetRGBColorShadow(), GetRGBColorShadow());
  }


}

};

