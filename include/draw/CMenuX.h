struct CMenuXItem {
public:
  // El menu esta activado
  BOOL _Enabled;
  // El menu es un separador
  BOOL _Separador;
  // Estado del menu
  int _Estado;
  // Icono del menu
  // Flag para saber si hay que mandar un WM_COMMAND o un WM_SYSCOMMAND
  BOOL _WM_SysCommand;
  int _Iko;
  // Texto del menu
  // Posicion del menu
  int _Pos;
  // ID del menu
  int _ID;
  std::string _Texto;
  // Submenus
  std::vector<CMenuXItem*> _Items;
  CMenuXItem() {
    _Texto = MENUTEXT;
    _Iko = NULL;
    _Separador = false;
    _Enabled = true;
    _Estado = -1;
  }
  inline const int TotalMenus(void) {
    return _Items.size();
  }
  inline CMenuXItem* GetSubMenuAt(int index) {
    return (0 <= index && index < TotalMenus()) ? _Items[ index ] : NULL;
  }
  // Funcion que retorna el texto de este menu
  inline const char* GetText(void) {
    return _Texto.c_str();
  }
  // Funcion para asignar el texto de este menu
  inline void SetText(const char* Txt) {
    _Texto = Txt;
  }
  CMenuXItem* AppendItem(const int ID, const char* Txt, const int IkoResource = NULL,
      const int Posicion = -1, const BOOL WM_SysCommand = false) {
    int Pos = Posicion;

    if (Pos = -1) {
      Pos = _Items.size();
    }

    CMenuXItem* TmpMenu = new CMenuXItem;
    TmpMenu->_Texto = Txt;
    TmpMenu->_Pos = Pos;
    TmpMenu->_Iko = IkoResource;
    TmpMenu->_ID = ID;
    //TmpMenu->_Parent = this;
    TmpMenu->_WM_SysCommand = WM_SysCommand;

    if (Pos > 0) {
      //TmpMenu->_Prev = _Items[ Pos - 1 ];
      //_Items[ Pos - 1 ] ->_Next = TmpMenu;
    }
    else {
      //TmpMenu->_Prev = NULL;
    }

    if (Pos < static_cast<int>(_Items.size())) {
      //TmpMenu->_Next = _Items[ Pos ];
      //_Items[ Pos ] ->_Prev = TmpMenu;
    }
    else {
      //TmpMenu->_Next = NULL;
    }

    _Items.insert(_Items.begin() + Pos, TmpMenu);
    return _Items[ Pos ];
  }
};
struct CMenuX : public CLabelUI {
public:
  CMenuXItem* _pMenuItems;
  // Variables
  // Padre del menu
  CMenuX* _Parent;
  // Ultimo menu
  //CMenuEx *_UMenu;
  // Item selecionado del submenu
  int _ItemSelecionado;
  // Destino de los mensajes WM_COMMAND
  //static HWND _hWndDestMsg;
  // ID que retorna el menu
  int _IDFinal;
  // El menu esta presionado
  int _Presionado;
  // Flag para saber si hay que desactivar el Alpha Blend
  BOOL _DesactivarBlend;
  // Objetos GDI que permaneceran en memoria mientras el menu este mostrado
  // Bitmap del BackBuffer
  //HBITMAP _MapaBits;
  // Libreria GDI 32
  // Funcion para el AlphaBlend
  TpGdiAlphaBlend GdiAlphaBlend;
  // Flag para determinar si las fuentes pertenecen a esta clase
  BOOL _FontsPropias;
  // Flag para determinar si los colores pertenecen a esta clase
  BOOL _ColoresPropios;
  // Colores del control
  ColoresCMenuEx* _Colores;
  // Fonts del control
  FontsCMenuEx* _Fonts;
  int rows, cols;
  int _ItemWidth;
  int _ItemHeight;
  CPopWnd* m_pWindow;
  CMenuXItem* m_pCurSelItem;
public:
  CMenuXItem* GetCurSel() const {
    return m_pCurSelItem; //_pMenuItems->GetSubMenuAt(_ItemSelecionado);
  }
  void SetCurSel(CMenuXItem* pItem) {
    m_pCurSelItem = pItem; //_pMenuItems->GetSubMenuAt(_ItemSelecionado);
  }
  CMenuX() {
    _Parent = NULL;
    m_pCurSelItem = NULL;
    m_pWindow = NULL;
    _pMenuItems = NULL;
    rows = 0;
    cols = 1;
    _ItemWidth = 70;
    _ItemHeight = 20;
    static CDllLibrary _Gdi32("gdi32.dll");
    GdiAlphaBlend = (TpGdiAlphaBlend) GetProcAddress((HMODULE) _Gdi32, "GdiAlphaBlend");
    //rows=3, cols=2;
    _ItemSelecionado = -1;
    Init(_RGB(192, 192, 192), _RGB(128, 128, 128), _RGB(255, 255, 255), _RGB(255, 255, 0), _RGB(192, 192, 192), _RGB(20, 20, 100), true);
  }
  void SetMenuItems(CMenuXItem* pMenuItems) {
    _pMenuItems = pMenuItems;
  }
  int GetItemWidth() const {
    return _ItemWidth;
  }
  int SetItemWidth(int nItemWidth) {
    return _ItemWidth = nItemWidth;
  }
  int GetItemHeight() const {
    return _ItemHeight;
  }
  int SetItemHeight(int nItemHeight) {
    return _ItemHeight = nItemHeight;
  }
  int SetItemRows(int nItemRows) {
    return rows = nItemRows;
  }
  int SetItemCols(int nItemCols) {
    return cols = nItemCols;
  }
  LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {}
  void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue) {
    if (_tcscmp(pstrName, _T("itemwidth")) == 0) {
      SetItemWidth(_ttoi(pstrValue));
    }
    else if (_tcscmp(pstrName, _T("itemheight")) == 0) {
      SetItemHeight(_ttoi(pstrValue));
    }
    else if (_tcscmp(pstrName, _T("itemdims")) == 0) {
      LPTSTR pstr = NULL;
      rows = _tcstol(pstrValue, &pstr, 10);
      ASSERT(pstr);
      cols = _tcstol(pstr + 1, &pstr, 10);
      ASSERT(pstr);
    }
    else {
      CLabelUI::SetAttribute(pstrName, pstrValue);
    }
  }
  const int Init(COLORREF Grad1, COLORREF Grad2, COLORREF TxtNormal, COLORREF TxtHilight,
      COLORREF TxtDesactivado, COLORREF FondoSel, const BOOL Esperar = true, const BOOL DesactivarBlend = false) {
    POINT Punto;
    //POINT Med;
    _IDFinal = -1;
    _ColoresPropios = true;
    _Colores = new ColoresCMenuEx;
    _Colores->Gradient1 = Grad1;
    _Colores->Gradient2 = Grad2;
    _Colores->TextoNormal = TxtNormal;
    _Colores->TextoHilight = TxtHilight;
    _Colores->TextoDesactivado = TxtDesactivado;
    _Colores->FondoSelecion = FondoSel;
    _Colores->Borde = _RGB(0, 0, 0);
    _FontsPropias = true;
    _Fonts = new FontsCMenuEx;
    _Fonts->FontNormal = CreateFont(13, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_ROMAN, "Tahoma");
    _Fonts->FontNegrita = CreateFont(13, 0, 0, 0, FW_BOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_ROMAN, "Tahoma");
    _Fonts->FontSubrayada = CreateFont(13, 0, 0, 0, FW_NORMAL, 0, true, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_ROMAN, "Tahoma");
    _DesactivarBlend = DesactivarBlend;
    GetCursorPos(&Punto);
    int TGetItemWidth = GetSystemMetrics(SM_CXFULLSCREEN);
    int FGetItemWidth = Punto.x;
    int TGetItemHeight = GetSystemMetrics(SM_CYFULLSCREEN);
    int FGetItemHeight = Punto.y;
    return _IDFinal;
  }
  // DeleteFontDC
  void DeleteFontDC(void) {
    if (_FontsPropias == true) {
      DeleteObject(_Fonts->FontNormal);
      DeleteObject(_Fonts->FontNegrita);
      DeleteObject(_Fonts->FontSubrayada);
      delete _Fonts;
      _FontsPropias = false;
    }

    if (_ColoresPropios == true) {
      delete _Colores;
      _ColoresPropios = false;
    }
  }
  void DrawBkgnd(HDC hDC, RECT RC) {
    //Transparency( 200 );
    // Bitmap viejo del BackBuffer
    int nWidth = RC.right - RC.left, nHeight = RC.bottom - RC.top;
    //RECT RC;
    HBRUSH BrochaBorde = CreateSolidBrush(_Colores->Borde);
    //GetClientRect( m_hWnd, &RC );
    TRIVERTEX GCVertex[ 2 ];
    GRADIENT_RECT tGRect;
    GCVertex[ 0 ].Red = (unsigned short)((_Colores->Gradient1 & 0xFF) * 0x100);
    GCVertex[ 0 ].Green = (unsigned short)(_Colores->Gradient1 & 0xFF00);
    GCVertex[ 0 ].Blue = (unsigned short)((_Colores->Gradient1 & 0xFF0000) / 0x100);
    GCVertex[ 0 ].x = RC.left;
    GCVertex[ 0 ].y = RC.top;
    GCVertex[ 1 ].Red = (unsigned short)((_Colores->Gradient2 & 0xFF) * 0x100);
    GCVertex[ 1 ].Green = (unsigned short)(_Colores->Gradient2 & 0xFF00);
    GCVertex[ 1 ].Blue = (unsigned short)((_Colores->Gradient2 & 0xFF0000) / 0x100);
    GCVertex[ 1 ].x = RC.right;
    GCVertex[ 1 ].y = RC.bottom;
    tGRect.UpperLeft = 0;
    tGRect.LowerRight = 1;
    GradientFill(hDC, GCVertex, 2, &tGRect, 1, GRADIENT_FILL_RECT_V);
    FrameRect(hDC, &RC, BrochaBorde);
    DeleteObject(BrochaBorde);
  }
  void DrawTrianguloSm(HDC hDC, int X, int Y, COLORREF Color, const BOOL Presionado /*= false */) {
    if (Presionado == true) {
      X++;
      Y++;
    }

    HRGN Region;
    POINT Puntos[ 3 ];
    Puntos[ 0 ].x = X;
    Puntos[ 0 ].y = Y + 2;
    Puntos[ 1 ].x = X + 9;
    Puntos[ 1 ].y = Y + 8;
    Puntos[ 2 ].x = X;
    Puntos[ 2 ].y = Y + 14;
    Region = CreatePolygonRgn(Puntos, 3, 1);
    HBRUSH Brocha = CreateSolidBrush(Color);
    FillRgn(hDC, Region, Brocha);
    FrameRgn(hDC, Region, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)), 1, 1);
    DeleteObject(Brocha);
    DeleteObject(Region);
  }
  void DrawSelecion(HDC hDC, RECT TmpR2) {
    TmpR2.right -= TmpR2.left;
    //TmpR2.bottom -= TmpR2.top;
    int An = TmpR2.right;
    int Al = TmpR2.bottom;
    HDC TmpBkhDC = CreateCompatibleDC(hDC);
    HBITMAP TmpBkBmp = CreateCompatibleBitmap(hDC, An, Al);
    HBITMAP TmpBkBmpV = static_cast<HBITMAP>(SelectObject(TmpBkhDC, TmpBkBmp));
    RECT TmpR = { 0, 0, An - 4, Al };
    HBRUSH BF2 = CreateSolidBrush(_Colores->FondoSelecion);
    FillRect(TmpBkhDC, &TmpR, BF2);
    FrameRect(TmpBkhDC, &TmpR, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));

    if (_DesactivarBlend == false) {
      BLENDFUNCTION BF;
      BF.AlphaFormat = 0;
      BF.BlendOp = NULL;
      BF.BlendFlags = NULL;
      BF.SourceConstantAlpha = 96;
      GdiAlphaBlend(hDC, TmpR2.left, TmpR2.top, TmpR2.right, TmpR2.bottom, TmpBkhDC, TmpR.left, TmpR.top, TmpR.right, TmpR.bottom, BF);
    }
    else {
      BitBlt(hDC, TmpR2.left, TmpR2.top, TmpR2.right, TmpR2.bottom, TmpBkhDC, TmpR.left, TmpR.top, SRCCOPY);
    }

    DeleteObject(BF2);
    SelectObject(TmpBkhDC, TmpBkBmpV);
    DeleteObject(TmpBkBmp);
    DeleteDC(TmpBkhDC);
  }
  void DrawItem(HDC hDC, RECT rcItem, CMenuXItem* pMenuItem, const int Estado) {
    int iItemHeight = rcItem.bottom - rcItem.top;
    int offsetY = rcItem.top;
    int offsetX = rcItem.left;
    pMenuItem ->_Estado = Estado;

    if (pMenuItem ->_Separador == true) {
      RECT RS = { offsetX + 5, offsetY + 26, offsetX + GetItemWidth() - 5, (offsetY + 28) };
      FillRect(hDC, &RS, static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
      return ;
    }

    HFONT VFont = NULL;
    HICON ResIko = NULL;
    //RECT TmpR2 = { 0, ( nItem * 20 ) + 17, GetItemWidth() - 4, 20 };
    RECT TmpR2 = { offsetX + 2, offsetY, offsetX + GetItemWidth(), iItemHeight };
    int X = offsetX + GetItemWidth() - 16;
    int Y = 19 + offsetY;

    switch (Estado) {
    case 0:                      // Normal CMenu
      VFont = static_cast<HFONT>(SelectObject(hDC, _Fonts->FontNormal));
      ::SetTextColor(hDC, _Colores->TextoNormal);

      if (pMenuItem ->TotalMenus() > 0) {
        DrawTrianguloSm(hDC, X, Y, _Colores->TextoNormal, false);
      }

      TextOut(hDC, offsetX + 24, offsetY + 2, pMenuItem ->GetText(), strlen(pMenuItem ->GetText()));

      if (pMenuItem ->_Iko != 0) {
        ResIko = static_cast<HICON>(LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(pMenuItem ->_Iko), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR));
        DrawIconEx(hDC, offsetX + 4, 19 + offsetY, ResIko, 16, 16, NULL, NULL, DI_NORMAL);
        DestroyIcon(ResIko);
      }

      break;

    case 1:                      // Selecionado
      VFont = static_cast<HFONT>(SelectObject(hDC, _Fonts->FontSubrayada));
      ::SetTextColor(hDC, _Colores->TextoHilight);
      DrawSelecion(hDC, TmpR2);

      if (pMenuItem ->TotalMenus() > 0) {
        DrawTrianguloSm(hDC, X, Y, _Colores->TextoHilight, false);
      }

      TextOut(hDC, offsetX + 24, offsetY + 2, pMenuItem ->GetText(), static_cast<int>(strlen(pMenuItem ->GetText())));

      if (pMenuItem ->_Iko != 0) {
        ResIko = static_cast<HICON>(LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(pMenuItem ->_Iko), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR));
        DrawIconEx(hDC, offsetX + 4, 19 + offsetY, ResIko, 16, 16, NULL, NULL, DI_NORMAL);
        DestroyIcon(ResIko);
      }

      break;

    case 2:                      // Presionado
      VFont = static_cast<HFONT>(SelectObject(hDC, _Fonts->FontSubrayada));
      ::SetTextColor(hDC, _Colores->TextoHilight);
      DrawSelecion(hDC, TmpR2);

      if (pMenuItem ->TotalMenus() > 0) {
        DrawTrianguloSm(hDC, X, Y, _Colores->TextoHilight, true);
      }

      TextOut(hDC, offsetX + 25, 21 + offsetY, pMenuItem ->GetText(), static_cast<int>(strlen(pMenuItem ->GetText())));

      if (pMenuItem ->_Iko != 0) {
        ResIko = static_cast<HICON>(LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(pMenuItem ->_Iko), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR));
        DrawIconEx(hDC, offsetX + 5, iItemHeight + offsetY, ResIko, 16, 16, NULL, NULL, DI_NORMAL);
        DestroyIcon(ResIko);
      }

      break;

    case 3:                      // Desactivado
      VFont = static_cast<HFONT>(SelectObject(hDC, _Fonts->FontNormal));
      ::SetTextColor(hDC, _Colores->TextoDesactivado);

      if (pMenuItem ->TotalMenus() > 0) {
        DrawTrianguloSm(hDC, X, Y, _Colores->TextoDesactivado, false);
      }

      TextOut(hDC, offsetX + 24, offsetY + 2, pMenuItem ->GetText(), static_cast<int>(strlen(pMenuItem ->GetText())));

      if (pMenuItem ->_Iko != 0) {
        ResIko = static_cast<HICON>(LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(pMenuItem ->_Iko), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR));
        DrawIconEx(hDC, offsetX + 4, 19 + offsetY, ResIko, 16, 16, NULL, NULL, DI_NORMAL);
        DestroyIcon(ResIko);
      }

      break;
    }

    SelectObject(hDC, VFont);
  };
  void DrawTitle(HDC TmphDC, const RECT& rcPaint) {
    HFONT VFont = static_cast<HFONT>(SelectObject(TmphDC, _Fonts->FontNegrita));
    ::SetTextColor(TmphDC, _Colores->TextoHilight);
    TextOut(TmphDC, rcPaint.left, rcPaint.top, GetText(), static_cast<int>(strlen(GetText())));
    SelectObject(TmphDC, VFont);
  }
  void DoPaint(HDC hDC, const RECT& rcPaint) {
    BOOL m_bDrawTitle = 0;
    int iTitleHeight = m_bDrawTitle ? _ItemHeight : 0;
    int nWidth = GetItemWidth() * cols;
    int nHeight = GetItemHeight() * rows;
    HDC _hDC = hDC;
    HDC TmphDC = CreateCompatibleDC(hDC);
    HBITMAP TmphDCBmp = CreateCompatibleBitmap(hDC, nWidth, nHeight);
    HBITMAP TmphDCBmpV = static_cast<HBITMAP>(SelectObject(TmphDC, TmphDCBmp));
    BitBlt(TmphDC, 0, 0, nWidth, nHeight, _hDC, m_rcItem.left, m_rcItem.top, SRCCOPY);
    SetBkMode(TmphDC, TRANSPARENT);
    CRect rcBk(0, 0, nWidth, nHeight);
    DrawBkgnd(TmphDC, rcBk);

    if (m_bDrawTitle) {
      DrawTitle(TmphDC, CRect(5, 3, 0, 0));
    }

    if (NULL != _pMenuItems) {
      for (unsigned int i = 0; i < _pMenuItems->TotalMenus(); i++) {
        CMenuXItem* pMenuItem = _pMenuItems->GetSubMenuAt(i);
        int offsetX = (i) % cols * _ItemWidth;
        int offsetY = (i) / cols * _ItemHeight;
        offsetY += iTitleHeight;
        RECT rc = iRECT(offsetX, offsetY, offsetX + _ItemWidth, offsetY + _ItemHeight);
        int nStage = 0;

        if (pMenuItem ->_Enabled == true) {
          if (_ItemSelecionado != i) {
            nStage = 0;
          }
          else {
            nStage = (_Presionado == i) ? 2 : 1;
          }
        }
        else {
          nStage = 3;
        }

        DrawItem(TmphDC, rc, pMenuItem, nStage);
      }
    }

    //BitBlt( hDC, 0, 0, nWidth, nHeight, TmphDC, 0, 0, SRCCOPY );
    BitBlt(hDC, m_rcItem.left, m_rcItem.top, nWidth, nHeight, TmphDC, 0, 0, SRCCOPY);
    SelectObject(TmphDC, TmphDCBmpV);
    DeleteObject(TmphDCBmp);
    DeleteDC(TmphDC);
  }
  void OnMouseLeave() {
    _ItemSelecionado = -1;
    Invalidate();
  }
  BOOL Activate(RECT rc, CMenuXItem* pMenuItems) {
    if (!CControlUI::Activate()) {
      return false;
    }

    //ASSERT( m_pWindow );
    if (NULL == m_pWindow) {
      m_pWindow = new CPopWnd();
      CMenuX* pMenu = new CMenuX();
      m_pWindow->Init(rc, &m_pWindow, this, pMenu);
      pMenu->SetMenuItems(pMenuItems);
      pMenu->_Parent = this;
      pMenu->SetItemCols(1);
      pMenu->SetItemRows(pMenuItems->TotalMenus());
    }
    else {
      m_pWindow->MoveWindow(&rc);
      CMenuX* pMenu = (CMenuX*)(m_pWindow->m_pLayout);
      pMenu->SetMenuItems(pMenuItems);
      m_pWindow->Invalidate();
    }

    Invalidate();
    return true;
  }
  void OnMouseMove(UINT nFlags, CPoint Pt) {
    RECT RC;
    int cY = Pt.y, cX = Pt.x;
    int aay, aax, aa;
    aax = static_cast<int>((cX + 3) / _ItemWidth);
    aay = static_cast<int>((cY + 3) / _ItemHeight);
    GetClientRect(&RC);

    if (PtInRect(&RC, Pt) == TRUE && aax < cols && aay < rows) {
      aa = aay * cols + aax;

      if (_ItemSelecionado == aa) {
        return ;
      }

      _ItemSelecionado = aa;

      if (aa < _pMenuItems->TotalMenus() && (rows > 1 || m_pWindow)) {
        CMenuXItem* pSubMenu = _pMenuItems->GetSubMenuAt(aa);
        int nSubMenuCount = pSubMenu->TotalMenus();

        if (nSubMenuCount > 0) {
          CRect rcSubMenu(m_rcItem.left + aax * _ItemWidth, m_rcItem.top + (aay + 1) * _ItemHeight,
              m_rcItem.left + (aax + 1) * _ItemWidth, m_rcItem.top + (aay + 1 + nSubMenuCount) * _ItemHeight);
          ::MapWindowRect(GetManager() ->GetPaintWindow(), HWND_DESKTOP, &rcSubMenu);
          Activate(rcSubMenu, pSubMenu);
        }
      }
    }
    else {
      POINT Punto;
      GetCursorPos(&Punto);
      HWND hh = WindowFromPoint(Punto);
      _ItemSelecionado = -1;

      if (SendMessage(hh, WM_ESMENU, 0, 0) == 33) {
        SetCapture(hh);
      }

      //return ;
    }

    Invalidate();
  }
  virtual LPCTSTR GetClass() const {
    return _T("MenuX");
  }
  void OnLButtonDown(UINT nFlags, CPoint Pt) {
    int aay, aax, aa;
    int cY = Pt.y, cX = Pt.x;
    aax = static_cast<int>((cX + 3) / _ItemWidth);
    aay = static_cast<int>((cY + 3) / _ItemHeight);
    aa = aay * cols + aax;

    if (aa < _pMenuItems->TotalMenus()) {
      CMenuXItem* pSubMenu = _pMenuItems->GetSubMenuAt(aa);
      int nSubMenuCount = pSubMenu->TotalMenus();

      if (nSubMenuCount > 0) {
        CRect rcSubMenu(m_rcItem.left + aax * _ItemWidth, m_rcItem.top + (aay + 1) * _ItemHeight,
            m_rcItem.left + (aax + 1) * _ItemWidth, m_rcItem.top + (aay + 1 + nSubMenuCount) * _ItemHeight);
        ::MapWindowRect(GetManager() ->GetPaintWindow(), HWND_DESKTOP, &rcSubMenu);
        Activate(rcSubMenu, pSubMenu);
        return ;
      }
    }

    CMenuX* pOwner = this;

    while (pOwner->_Parent) {
      pOwner = pOwner->_Parent;
    }

    pOwner->SetFocus();
    CMenuXItem* pCurSel = _pMenuItems->GetSubMenuAt(_ItemSelecionado);
    SetCurSel(pCurSel);
    pOwner->SetCurSel(pCurSel);
    pOwner->m_pManager->SendNotify(pOwner, _T("itemselect"));
    Invalidate();
  }
  BEGIN_MESSAGE_MAP()
    //ON_WM_MOUSEWHEEL()
    //ON_WM_KEYDOWN()
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONDOWN()
    ON_WM_MOUSELEAVE()
  END_MESSAGE_MAP(CLabelUI)
};
int test_CMenuX(CPaintManagerUI& m_pm)
{
  CMenuX* pMenu;
  static CMenuXItem MenuItem;
  CMenuXItem* pMenuItem = &MenuItem;
  pMenu = static_cast<CMenuX*>(m_pm.FindControl(_T("CMenuX1")));
  pMenu->SetMenuItems(pMenuItem);
  int i, j, k, id = 1;

  for (i = 0; i < 6; ++i) {
    char buf[ 256 ];
    _snprintf(buf, 256, "menu %d", i);
    CMenuXItem* pMenuItem2 = pMenuItem->AppendItem(id++, buf, 0);

    for (j = 0; j < 5; ++j) {
      _snprintf(buf, 256, "%d sub %d", i, j);
      CMenuXItem* pMenuItem3 = pMenuItem2->AppendItem(id++, buf, 0);

      for (k = 0; k < 5; ++k) {
        _snprintf(buf, 256, "%d sub %d sub %d", i, j, k);
        //CMenuXItem* pMenuItem4 = pMenuItem3->AppendItem( 2, buf, 0 );
      }
    }
  }

  return 0;
}
