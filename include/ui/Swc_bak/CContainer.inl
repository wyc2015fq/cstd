
#define NORMAL 0
#define OVER   1
#define PRESS  2



typedef struct CTab
{
  TCHAR lpMsg[128];
  IRECT rect;
  UINT  nMenu;
} CTab;


enum Style {S3D = 0, FLAT = 1, SHADOW = 2};
enum AlingTab {ALN_TOP = 0, ALN_BOTTOM = 1};
enum TypeTab {TYPE_NORMAL = 0, TYPE_MDI = 1};

//*********************************************************
class CContainer
{
public:
  img_t*      m_img;
  CToolButton  m_CloseBtn;
  CToolButton  m_AutoHideBtn;
  TypeTab     m_TypeTab;
  BOOL      m_ActualTitle;
  CTab*     m_tab;
  int m_tab_count;
  ISIZE     m_sizeImag;
  int       m_Numtabs;
  int       m_iMinValRec;
  int       m_ActualVar;
  int       m_iSelectTab;
  IRECT     m_rectCliente;
  IRECT     m_rectTab;
  BOOL      m_style;
  //CWin*     pParent;
  AlingTab    m_alnTab;
  BOOL      m_InTab;
  BOOL      m_bDrawFrame; //if want to draw the mayor child window  or all windows
  IRECT     m_rcCloseBtn;
  IRECT     m_rcAutoHideBtn;
  int       m_stateBtn;
  int       m_stateAHBtn;
  BOOL      m_bCaption;
  BOOL      m_close;
  //Drag Variables
  BOOL      m_DragTab;
  BOOL      m_NCDragWindow;
  int       m_TabMove;
  BOOL        m_DragWindow;
  IPOINT      m_pointbase;
  //End Drag

  UINT      m_DOCKFLOAT;
  UINT      m_DOCKSITE;  //ultimo docksite de la ventana
public:
  
  int     GetCount() {
    return m_Numtabs;
  }
  
  void  OpcionDrawWindow(BOOL bDraw = TRUE) {
    m_bDrawFrame = bDraw;
  }


/////////////////////////////////////////////////////////////////////////////
// CContainer

#define  SPACE_TAB    5
#define  DEFAULT_FLAT 16
#define  HTPIN      323


CContainer::CContainer()
{
  iRectSetEmpty(&m_rectCliente);
  iRectSetEmpty(&m_rectTab);
  m_ActualVar = 0;
  m_Numtabs = 0;
  m_iSelectTab = 0;
  m_sizeImag = iSIZE(15, 15);
  m_iMinValRec = 0; //minimo valor requerido para mostrar toda las carpetas
  m_style = S3D;
  m_ActualTitle = TRUE;
  m_alnTab = ALN_BOTTOM;
  m_TypeTab = TYPE_NORMAL;
  m_bDrawFrame = FALSE;
  m_stateBtn = NORMAL;
  m_stateAHBtn = NORMAL;
  m_bCaption = TRUE;
  m_DragTab = FALSE;
  m_NCDragWindow = FALSE;
  m_TabMove = 0;
  m_DOCKSITE = BS_DOCKINGLEFT;
  m_DOCKFLOAT = 0;
  m_rcAutoHideBtn;
  m_rcCloseBtn;
  m_close = FALSE;
  m_pointbase = iPOINT(0, 0);
}

void CContainer::SetTypeTab(TypeTab Type)
{
  m_TypeTab = Type;
}
//****************************************************************************
BOOL CContainer::OnEraseBkgnd(img_t* im, const IRECT* pclip, IRECT m_rect)
{
  if (m_Numtabs == 0) {
    COLOR cbr;
    cbr = (CLR_BTNFACE);
    imdraw_FillRect(im, pclip, &m_rect, cbr);
  }

  return TRUE;
}

BOOL CContainer::OnPaint(img_t* im, const IRECT* pclip, IRECT rcClient1, const font_t* fo)
{
  IRECT rcClient = rcClient1;
  COLOR cbr;
  IRECT m_rectDraw;
  cbr = (GetRGBColorFace());
  imdraw_FillRect(im, pclip, &rcClient, cbr);

  m_rectTab = m_rectCliente;
  m_rectDraw = m_rectCliente;

  if (m_alnTab == ALN_BOTTOM) {
    if (m_Numtabs > 1) {
      m_rectDraw.b = rcClient.b - (m_sizeImag.h + SPACE_TAB + 2);
    }
  }
  else {
    m_rectDraw.t = rcClient.t + (m_sizeImag.h + SPACE_TAB + 2);
  }

  if (m_style != S3D) { //si son carpetas estilo excel o tabbed como devstudio 7
    if (m_alnTab == ALN_BOTTOM) {
      m_rectCliente.b = rcClient.b - (DEFAULT_FLAT);
      m_rectTab.t = rcClient.b + 2;
    }
    else {
      m_rectTab.b = rcClient.t + (DEFAULT_FLAT) + 3;
    }
  }

  int nVal = 0;

  if (m_alnTab != ALN_TOP) {
    imdraw_FillRect(im, pclip, &m_rectTab, cbr);
    //IRECT mrc = m_rectDraw;
  }
  else {
    IRECT rc = m_rectDraw;

    for (int i = 0; i < 2; i++) {
      imdraw_Draw3dRect(im, pclip, &rc, GetRGBColorFace(), GetRGBColorFace(), 0);
      iRectDeflate2(&rc, 1, 1);
    }

    iRectInflate2(&rc, 3, 3);

    if (m_bDrawFrame) {
      imdraw_Draw3dRect(im, pclip, &rc, _GetSysColor(CLR_BTNSHADOW), _GetSysColor(CLR_BTNSHADOW), 0);
    }
    else {
      IRECT mrc = m_rectDraw;
      mrc.t += nVal;
      imdraw_Draw3dRect(im, pclip, &mrc, _GetSysColor(CLR_BTNSHADOW), _GetSysColor(CLR_BTNSHADOW), 0);

    }

    rc = m_rectDraw;
    rc.b = rc.t + 3;
    imdraw_FillRect(im, pclip, &rc, cbr);

  }

  Drawtabs(im, pclip, fo);
  return TRUE;
}


#if 0
int CContainer::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
  if (!m_img.Create(IDB_GUI_DOCKBAR, 9, 11, _RGB(255, 0, 255))) {
    return -1;
  }

  m_CloseBtn.SetData(5, _T("Close"));
  m_CloseBtn.SetImageList(IDB_GUI_DOCKBAR, 9, 10, _RGB(255, 0, 255));
  m_AutoHideBtn.SetData(11, _T("Auto Hide"));
  m_AutoHideBtn.SetImageList(IDB_GUI_DOCKBAR, 9, 15, _RGB(255, 0, 255));
  // TODO: Add your specialized creation code here
  return 0;
}

BOOL CContainer::OnNcCalcSize(BOOL /*bCalcValidRects*/, LPNCCALCSIZE_PARAMS lpncsp)
{
  // adjust non-client area for border space

  if (m_bCaption && m_DOCKFLOAT != BS_FLOATING) {
    lpncsp->rgrc[0].t += 19;
  }

  return TRUE;
}
#endif

void CContainer::OnNcPaint(img_t* im, const IRECT* pclip, IRECT rcWindow, IRECT rcClient, const font_t* fo)
{
  // TODO: Add your message handler code here
  // Do not call CControlBar::OnNcPaint() for painting messages
  // Tomo la misma rutina que se desarrolla para la clase
  // CGuiToolBarWnd.
  //la idea es tomar el area de la ventana y area  cliente , luego debemos
  //igualar el area de coordenadas de ventana al cliente

  iRectOffset2(&rcClient, -rcWindow.l, -rcWindow.t);
  iRectOffset2(&rcWindow, -rcWindow.l, -rcWindow.t);

  COLOR cb;
  cb = (GetRGBColorFace());
  imdraw_FillRect(im, pclip, &rcWindow, cb);

  if (m_bCaption) {
    DrawGripper(im, pclip, rcWindow, fo);
  }
}

void CContainer::DrawGripper(img_t* im, const IRECT* pclip, IRECT rcWin, const font_t* fo)
{

  IRECT rc = rcWin;
  rc.b = rc.t + 18;

  COLOR cb;
  cb = (GetRGBPressBXP());
  imdraw_FillRect(im, pclip, &rc, cb);
  imdraw_Draw3dRect(im, pclip, &rc, GetRGBMenu(), GetRGBMenu(), 0);
  //CGradient M(iSIZE(rc.Width(),rc.Height()+4));
  //M.PrepareVertical(dc);
  //M.Draw(dc,rc.l,rc.t+1,0,0,rc.Width(),rc.Height()+4,SRCCOPY);

  //COLOR clrtext;

  //guardar espacio para la imagen

  CTab* ct = m_tab+m_iSelectTab;

  char m_cadBreak[126] = {0};
  strcpy(m_cadBreak, ct->lpMsg);
  int contt;
  int cont = contt = strlen(m_cadBreak);
  ISIZE coor = font_text_size(fo, m_cadBreak, cont, 0);
  rc.l += 3;

  if (coor.w > (RCW(&rc) - 35)) {
    rc.l += 1;

    while (cont > 1) {
      char m_scadtemp[64];
      memset(m_scadtemp, 0x00, 64);
      strcpy(m_scadtemp, m_cadBreak);
      strcat(m_scadtemp, ("..."));
      ISIZE coor = font_text_size(fo, m_scadtemp, -1, 0);

      if (coor.w > (RCW(&rc) - 35)) {
        *(m_cadBreak + cont) = 0;
      }
      else {
        break;
      }

      cont--;
    }

    lstrcat(m_cadBreak, _T("..."));
    rc.r += 3;


    imdraw_DrawText(im, pclip, m_cadBreak, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER, fo, Color_Black);
  }
  else {
    imdraw_DrawText(im, pclip, ct->lpMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER, fo, Color_Black);
  }

  //-------------------------
  //No Client Button
  //-------------------------
  rc = rcWin;
  rc.l = rcWin.r - 18;
  rc.b = rc.t + 18;
  rc.r -= 7;
  rc.t += 3;
  rc.b -= 3;

  if (m_DOCKFLOAT != BS_FLOATING) {
    m_rcAutoHideBtn = rc;
    m_rcAutoHideBtn.r -= 12;
    m_rcAutoHideBtn.l -= 12;
  }

  m_rcCloseBtn = rc;

  if (m_bCaption) {
    m_CloseBtn.CControlBarButton_Paint(im, pclip, m_stateBtn, m_rcCloseBtn, GetRGBPressBXP(), TRUE);
    //if (m_DOCKFLOAT !=BS_FLOATING)
    //  m_AutoHideBtn.Paint(dc,m_stateAHBtn,m_rcAutoHideBtn,GetRGBGray(),TRUE);
  }
}

#if 0
void CContainer::OnNcMouseMove(UINT nHitTest, IPOINT point)
{
  BOOL bDif = FALSE;

  if (m_NCDragWindow) {
    bDif = abs(m_pointbase.x - point.x) > 3 || abs(m_pointbase.y - point.y) > 3;
  }


  if (m_NCDragWindow) {
    if ((m_DOCKFLOAT != BS_FLOATING) && bDif) {

      if (!m_NCDragWindow) {
        return;
      }

      IRECT rcw;
      //GetWindowRect(rcw);
      //ClientToScreen(&point);
      ReleaseCapture();
      m_DOCKFLOAT = BS_FLOATING;
      m_DragTab = FALSE;
      m_NCDragWindow = FALSE;
      UINT uFirstTab = 0;
      int iCont = 0;

      for (iCont = 0; iCont < m_Numtabs; iCont++) {
        CTab* ct = m_tab[iCont];

        if (ct->rect.PtInRect(point) != 0) {

          if (iCont == 0) {
            uFirstTab = ((CDockPanelBase*)ct->pParent)->GetDlgCtrlID();
          }

          ((CDockPanelBase*)ct->pParent)->uAling = BS_FLOATING;
          ((CDockPanelBase*)ct->pParent)->iLasPosition = iCont;
          ((CDockPanelBase*)ct->pParent)->bIsPartOfTabbed = iCont == 0 ? FALSE : TRUE;
          ((CDockPanelBase*)ct->pParent)->bIsShow = TRUE;
          ((CDockPanelBase*)ct->pParent)->m_IdCtrlParent = iCont == 0 ? 0 : uFirstTab;
          rcw = ct->rect;

        }
      }

      theApp->m_WinMain->FloatWindow(this, point, rcw);

      DockManager* mDoc;

      if (m_DOCKSITE == BS_ALIGN_BOTTOM) {
        mDoc = (DockManager*)theApp->m_WinMain->m_DockBottom;
      }

      if (m_DOCKSITE == BS_ALIGN_LEFT) {
        mDoc = (DockManager*)theApp->m_WinMain->m_DockLeft;
      }

      if (m_DOCKSITE == BS_ALIGN_RIGHT) {
        mDoc = (DockManager*)theApp->m_WinMain->m_DockRight;
      }

      mDoc->DeleteWindow(
          mDoc->FindItem(GetSafeHwnd())
      );
      theApp->m_WinMain->UpdateFrame();

    }

    return ;
  }

  if (nHitTest == HTCLOSE)
    if (m_stateBtn != NORMAL) {
      return;
    }

  if (nHitTest == HTPIN)
    if (m_stateAHBtn != NORMAL) {
      return;
    }

  if (nHitTest == HTCLOSE) {
    m_stateBtn = OVER;
    SetTimer(1, 100, 0);
  }

  if (nHitTest == HTPIN) {
    m_stateAHBtn = OVER;
    SetTimer(2, 100, 0);
  }

  SendMessage(WM_NCPAINT, 0, 0);
  Default();

}

void CContainer::OnNcLButtonDown(UINT nHitTest, IPOINT point)
{
  if (m_NCDragWindow) {
    return;
  }

  if (nHitTest == HTCAPTION) {
    m_NCDragWindow = TRUE;
    //SetCapture();
    m_pointbase = point;
    SetFocus();
    return ;
  }
  else if (nHitTest == HTCLOSE) {
    m_stateBtn = PRESS;
    SendMessage(WM_NCPAINT, 0, 0);
    return;
  }
  else if (nHitTest == HTPIN) {
    m_stateAHBtn = PRESS;
    SendMessage(WM_NCPAINT, 0, 0);
    return;
  }

  //--------------------------

  Default();


}

BOOL CContainer::IsClosePane()
{
  return m_close;
}
void CContainer::OnNcLButtonUp(UINT nHitTest, IPOINT point)
{
  // TODO: Add your message handler code here and/or call default

  if (nHitTest == HTCAPTION) {

    if (nHitTest == HTCAPTION) {
      if (m_NCDragWindow) {
        ReleaseCapture();
        m_NCDragWindow = FALSE;
        return;
      }

    }

  }

  IRECT rc;
  //------------------ para el boton
  IRECT rcT = m_rcCloseBtn;
  ClientToScreen(rcT);
  point.y += 20;
  point.x += 1;

  if (rcT.PtInRect(point)) {
    if (m_stateBtn == PRESS) {
      if (IsWindowVisible()) {
        //if (m_Numtabs==1)
        ShowPane(FALSE);
        //else
        //  DeleteTab(m_iSelectTab);
      }
      else {
        ShowPane(TRUE);
      }

      if (m_DOCKSITE == BS_ALIGN_BOTTOM) {
        ((DockManager*)theApp->m_WinMain->m_DockBottom)->EveryWindowsInsert();
      }

      if (m_DOCKSITE == BS_ALIGN_LEFT) {
        ((DockManager*)theApp->m_WinMain->m_DockLeft)->EveryWindowsInsert();
      }

      if (m_DOCKSITE == BS_ALIGN_RIGHT) {
        ((DockManager*)theApp->m_WinMain->m_DockRight)->EveryWindowsInsert();
      }

      theApp->m_WinMain->UpdateFrame();
      //::SendMessage(GetParent(),WM_SIZE,0,0);
    }

  }

  if (m_stateBtn == PRESS) {
    m_stateBtn = NORMAL;
    KillTimer(1);
    SendMessage(WM_NCPAINT, 0, 0);
    return;

  }

  rcT = m_rcAutoHideBtn;
  ClientToScreen(rcT);

  if (rcT.PtInRect(point)) {

  }

  if (m_stateAHBtn == PRESS) {
    m_stateAHBtn = NORMAL;
    KillTimer(1);
    SendMessage(WM_NCPAINT, 0, 0);
    return;
  }


  Default();
}

UINT CContainer::OnNcHitTest(IPOINT cp)
{

  IRECT rcWindow;
  //no se convierte las coordenadas de pantalla porque el punto
  //entregado por esta función esta dado en el mismo sentido.
  GetWindowRect(rcWindow);
  rcWindow.b = rcWindow.t + 16;
  IRECT rcT = m_rcCloseBtn;
  IPOINT cpn;
  ClientToScreen(rcT);
  IPOINT pt = cp;
  pt.y += 20; //I don't know? but if you don't make this no check the point.
  pt.x += 1;

  if (rcT.PtInRect(pt)) {
    return HTCLOSE;
  }

  rcT = m_rcAutoHideBtn;

  ClientToScreen(rcT);
  pt = cp;
  pt.y += 20;
  pt.x += 1;

  if (rcT.PtInRect(pt)) {
    return HTPIN;
  }


  if (rcWindow.PtInRect(cp)) {
    SetCursor(::LoadCursor(NULL, IDC_ARROW));
    return HTCAPTION;
  }

  return Default();
}


void CContainer::RecalLayout(IRECT m_rectCliente)
{
  if (m_style == S3D) {
    if (m_Numtabs > 1) {
      m_rectCliente.b = m_rectCliente.b - (m_sizeImag.h + SPACE_TAB - 1);
    }
  }
  else {
    if (m_alnTab == ALN_BOTTOM) {
      m_rectCliente.b = m_rectCliente.b - (DEFAULT_FLAT + 1);
    }
    else {
      m_rectCliente.t += DEFAULT_FLAT;
      m_rectCliente.b = m_rectCliente.b - (DEFAULT_FLAT + 4);

    }
  }

  for (int iCont = 0; iCont < m_Numtabs; iCont++) {
    CTab* ct = m_tab[iCont];
    CWin* m_tempWin = ct->pParent;
    ASSERT(m_tempWin);
    IRECT mrc;

    if (m_style == S3D) {
      m_tempWin->SetWindowPos(NULL, m_rectCliente.l + (SPACE_TAB - 4),
          m_rectCliente.t + (SPACE_TAB / 2), m_rectCliente.r - (SPACE_TAB - 4),
          m_rectCliente.b - ((SPACE_TAB)), SWP_NOACTIVATE |
          SWP_NOZORDER);
    }
    else {
      if (m_alnTab == ALN_BOTTOM) {

        m_tempWin->SetWindowPos(NULL, m_rectCliente.l + (SPACE_TAB - 3),
            m_rectCliente.t + 1, m_rectCliente.r - 1,
            m_rectCliente.b - 1, SWP_NOACTIVATE | SWP_NOZORDER);
      }
      else {
        m_tempWin->SetWindowPos(NULL, m_rectCliente.l + (SPACE_TAB - 4),
            m_rectCliente.t + 7, m_rectCliente.r - 4,
            m_rectCliente.b - 4, SWP_NOACTIVATE | SWP_NOZORDER);
      }
    }
  }

  AjustTabs();
  Invalidate();
  UpdateWindow();
}


void CContainer::AjustTabs()
{
  int m_iSizeAct = 0;
  m_iMinValRec = 4;

  if (m_Numtabs <= 1 && m_alnTab == ALN_BOTTOM) {
    return;
  }

  CClientDC dc(GetSafeHwnd());
  GetClientRect(m_rectCliente);

  if (m_alnTab == ALN_BOTTOM) {
    m_rectCliente.b = m_rectCliente.b - (m_sizeImag.h + SPACE_TAB + 2);
  }
  else {
    m_rectCliente.b = m_rectCliente.t + (m_sizeImag.h + SPACE_TAB + 2);
  }

  HFONT hFont = dc.SelectObject(CFont((HFONT)GetStockObject(DEFAULT_GUI_FONT)));

  for (int iCont = 0; iCont < m_Numtabs; iCont++) {
    CTab* ct = m_tab[iCont];
    IRECT m_Arect;
    ISIZE m_sChar = dc.GetTextExtent(ct->lpMsg, lstrlen(ct->lpMsg));
    m_Arect.l = m_iMinValRec + 1;
    ISIZE m_sz;
    m_sz = theApp->m_WinMain->m_sizeImagDocks;
    m_iMinValRec += m_alnTab != ALN_TOP ? 16 + 2 + m_sChar.w + 6 : m_sChar.w + 6;

    if (m_alnTab == ALN_BOTTOM) {
      m_Arect.t = m_rectCliente.b;
      m_Arect.r = m_iMinValRec;
      m_Arect.b = m_rectCliente.b + 6 + m_sizeImag.h;
    }
    else {
      m_Arect.t = m_rectCliente.t;
      m_Arect.r = m_iMinValRec + (iCont == m_iSelectTab || iCont == 0 ? 30 : 0);
      m_Arect.b = m_rectCliente.t + 6 + m_sizeImag.h;
    }

    ct->rect = m_Arect;

  }

  //si el espacio requerido es mayor que el disponible
  //se debe proporcionar cada tab solo si es 3d
  if (m_style == S3D) {
    int m_NewResize = 4;

    if (m_iMinValRec > m_rectCliente.Width() - 4) {
      m_iSizeAct = (m_rectCliente.Width() - 12) / m_Numtabs;

      for (int iCont = 0; iCont < m_Numtabs; iCont++) {
        CTab* ct = m_tab[iCont];
        IRECT m_Arect;
        m_Arect.l = m_NewResize;
        m_Arect.r = m_NewResize + m_iSizeAct;
        m_Arect.t = m_rectCliente.b;
        m_Arect.b = m_rectCliente.b + 6 + m_sizeImag.h;
        m_NewResize += m_iSizeAct + 1;
        ct->rect = m_Arect;
      }
    }
  }

  dc.DeleteDC();
}
#endif

void CContainer::Drawtabs(img_t* im, const IRECT* pclip, const font_t* fo)
{
  COLOR light;
  COLOR Dark;
  COLOR Black;
  COLOR Face;

  if (m_Numtabs <= 1 && m_alnTab == ALN_BOTTOM) {
    return;
  }

  light = (_GetSysColor(CLR_BTNHIGHLIGHT));
  Dark = (_GetSysColor(CLR_BTNSHADOW));
  Black = (_GetSysColor(BLACK_PEN));
  Face = (GetRGBColorFace());
  //IRECT rectText;
  COLOR clr = (Dark);
  LPCSTR szStrPoints = _T("...");

  for (int iCont = 0; iCont < m_Numtabs; iCont++) {
    CTab* ct = m_tab + iCont;

    if (iCont == m_iSelectTab) {
      PrepareTabs(im, pclip, ct->rect, GetRGBColorWhite(), GetRGBColorWhite());
      clr = (Dark);
      imdraw_line2(im, pclip, ct->rect.l, ct->rect.t, ct->rect.l, ct->rect.b - 2, clr, 1);
      imdraw_line2(im, pclip, ct->rect.l + 1, ct->rect.b - 2, ct->rect.l + 2, ct->rect.b - 1, clr, 1);

      //corner left corrections begin
      clr = (Face);
      imdraw_line2(im, pclip, ct->rect.l, ct->rect.b - 2, ct->rect.l, ct->rect.b - 1, clr, 1);
      imdraw_line2(im, pclip, ct->rect.l, ct->rect.b - 1, ct->rect.l + 2, ct->rect.b - 1, clr, 1);
      //---corner corrections end

      clr = (Dark);
      imdraw_line2(im, pclip, m_rectTab.l, ct->rect.t, ct->rect.l, ct->rect.t, clr, 1);
      imdraw_line2(im, pclip, ct->rect.r, ct->rect.t, m_rectTab.r, ct->rect.t, clr, 1);

      //line bottom
      clr = (Dark);
      imdraw_line2(im, pclip, ct->rect.l + 2, ct->rect.b - 1, ct->rect.r - 3, ct->rect.b - 1, clr, 1);
      clr = (Face);
      imdraw_line2(im, pclip, ct->rect.l + 2, ct->rect.b - 2, ct->rect.r - 3, ct->rect.b - 2, clr, 1);

      //right corner
      clr = (Dark);
      imdraw_line2(im, pclip, ct->rect.r - 3, ct->rect.b - 1, ct->rect.r, ct->rect.b - 4, clr, 1);

      clr = (Face);
      imdraw_line2(im, pclip, ct->rect.r - 3, ct->rect.b - 2, ct->rect.r - 1, ct->rect.b - 5, clr, 1);

      // corner right corrections
      imdraw_line2(im, pclip, ct->rect.r - 1, ct->rect.b - 2, ct->rect.r, ct->rect.b - 2, clr, 1);
      imdraw_line2(im, pclip, ct->rect.r - 2, ct->rect.b - 1, ct->rect.r, ct->rect.b - 1, clr, 1);

      // end corner right corrections
      //right vertical line
      clr = (Dark);
      imdraw_line2(im, pclip, ct->rect.r - 1, ct->rect.t, ct->rect.r - 1, ct->rect.b - 3, clr, 1);
      clr = (Face);
      imdraw_line2(im, pclip, ct->rect.r - 2, ct->rect.t + 1, ct->rect.r - 2, ct->rect.b - 3, clr, 1);
    }
    else {
      if (iCont != 0 && ((iCont - 1) != m_iSelectTab)) {
        clr = (Dark);
        imdraw_line2(im, pclip, ct->rect.l, ct->rect.t + 3, ct->rect.l, ct->rect.b - 3, clr, 1);
      }
    }

#if 0
    //se dibuja el icono
    ISIZE m_sChar = font_text_size(fo, ct->lpMsg, -1, 0);
    COLOR clrtext = _RGB(0, 0, 0);

    if (iCont != m_iSelectTab) {
      clrtext = (GetRGBColorGrayText());
    }

    if (theApp->m_WinMain->m_sizeImagDocks.w + m_sChar.w < ct->rect.Width() - 6) {
      //Image.Draw(ct->uIcon,dc->m_hDC,iPOINT(ct->rect.l+5,ct->rect.t+2),ILD_TRANSPARENT);
      theApp->m_WinMain->DrawIconDocks(ct->pParent->GetNumIcon(), iPOINT(ct->rect.l + 4, ct->rect.t + 2), dc);
      rectText = ct->rect;
      rectText.l += theApp->m_WinMain->m_sizeImagDocks.w + 6;
      rectText.r -= 2;
      dc->DrawText(ct->lpMsg, rectText, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
    }
    else {
      //verificamos si se puede poner algun texto o
      //por lo memos el icono
      if (theApp->m_WinMain->m_sizeImagDocks.w < ct->rect.Width()) {
        //Image.Draw(ct->uIcon,dc->m_hDC,iPOINT(ct->rect.l+4,ct->rect.t+2),ILD_TRANSPARENT);
        theApp->m_WinMain->DrawIconDocks(ct->pParent->GetNumIcon(), iPOINT(ct->rect.l + 4, ct->rect.t + 2), dc);
        rectText = ct->rect;
        rectText.l += theApp->m_WinMain->m_sizeImagDocks.w + 6;
        rectText.r -= 5;
        LPCSTR m_cadBreak = ct->lpMsg;
        int c = 0;
        dc->DrawTextAjust(rectText, ct->lpMsg, DT_SINGLELINE | DT_LEFT | DT_VCENTER, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
        rectText.r += 3;
      }
    }
#endif
  }

}

CTab* CContainer::GetTab(int nTab)
{
  if (nTab > m_Numtabs) {
    return NULL;
  }
  else {
    if (m_tab_count < 1) {
      return NULL;
    }

    return m_tab+ nTab;
  }

  return NULL;
}

#if 0
void CContainer::OnMouseMove(UINT nHitTest, IPOINT point)
{
  if (m_NCDragWindow) {
    if (((nHitTest != HTCAPTION) && (m_DOCKFLOAT != BS_FLOATING)) && m_TypeTab != TYPE_MDI) {

      if (!m_NCDragWindow) {
        return;
      }

      IRECT rcw;
      GetWindowRect(rcw);
      ClientToScreen(&point);
      ReleaseCapture();
      m_DOCKFLOAT = BS_FLOATING;
      m_DragTab = FALSE;
      m_NCDragWindow = FALSE;
      UINT uFirstTab = 0;
      int iCont = 0;

      for (iCont = 0; iCont < m_Numtabs; iCont++) {
        CTab* ct = m_tab[iCont];

        if (ct->rect.PtInRect(point) != 0) {

          if (iCont == 0) {
            uFirstTab = ((CDockPanelBase*)ct->pParent)->GetDlgCtrlID();
          }

          ((CDockPanelBase*)ct->pParent)->uAling = BS_FLOATING;
          ((CDockPanelBase*)ct->pParent)->iLasPosition = iCont;
          ((CDockPanelBase*)ct->pParent)->bIsPartOfTabbed = iCont == 0 ? FALSE : TRUE;
          ((CDockPanelBase*)ct->pParent)->bIsShow = TRUE;
          ((CDockPanelBase*)ct->pParent)->m_IdCtrlParent = iCont == 0 ? 0 : uFirstTab;

        }
      }

      theApp->m_WinMain->FloatWindow(this, point, rcw);

      DockManager* mDoc;

      if (m_DOCKSITE == BS_ALIGN_BOTTOM) {
        mDoc = (DockManager*)theApp->m_WinMain->m_DockBottom;
      }

      if (m_DOCKSITE == BS_ALIGN_LEFT) {
        mDoc = (DockManager*)theApp->m_WinMain->m_DockLeft;
      }

      if (m_DOCKSITE == BS_ALIGN_RIGHT) {
        mDoc = (DockManager*)theApp->m_WinMain->m_DockRight;
      }

      mDoc->DeleteWindow(
          mDoc->FindItem(GetSafeHwnd())
      );
      theApp->m_WinMain->UpdateFrame();

    }

    return ;
  }

  if (m_DragTab) {
    CTab* ct = m_tab[m_TabMove];

    if (ct->rect.PtInRect(point) != 0) {
      // we are in the same tab so don't do nothing
      return;
    }
    else {
      int iCont;

      for (iCont = 0; iCont < m_Numtabs; iCont++) {
        CTab* ct = m_tab[iCont];

        if (ct->rect.PtInRect(point) != 0) {
          m_InTab = TRUE;
          m_DragTab = TRUE; //DragTab
          //umm interchange the pointers
          NewBValArray<CTab*> m_pTemp;
          CTab* Temp = m_tab[m_TabMove];
          m_pArray.SetAt(m_TabMove, ct);
          m_pArray.SetAt(iCont, Temp);
          SetCurtab(iCont);
          m_TabMove = iCont; //This is moved

          ReleaseCapture();
          break;
        }

      }

      if ((iCont >= m_Numtabs) && (m_TypeTab != TYPE_MDI)) {
        IRECT rcClient;
        GetClientRect(&rcClient);

        CContainer* ctn = new CContainer;
        CTab* ct = m_tab[m_TabMove];

        rcClient.t = ct->rect.t;
        rcClient.b = ct->rect.b;

        if (rcClient.PtInRect(point)) {
          return;
        }

        if (!ctn->Create(WS_CHILD | WS_VISIBLE, IRECT(0, 0, 0, 0), this, ct->pParent->GetDlgCtrlID())) {
          return;
        }

        ctn->Addtab(ct->pParent, ct->lpMsg, ct->nMenu);
        ct->pParent->SetParent(ctn);

        //ctn->SetImageList(Image);
        ClientToScreen(&point);
        ctn->m_DOCKFLOAT = BS_FLOATING;
        ctn->m_DOCKSITE = GetWindowLong(GetSafeHwnd(), GWL_ID);
        HWND hwnd = (HWND)GetWindowLong(ct->pParent->GetSafeHwnd(), GWL_HWNDPARENT);
        HWND hWnd1 = ctn->GetSafeHwnd();
        IRECT rcw;
        GetWindowRect(rcw);
        theApp->m_WinMain->FloatWindow(ctn, point, rcw);
        m_DragTab = FALSE;
        DeleteTab(m_TabMove);

        if (m_pArray.GetSize() > 0) {
          ct = m_tab[0];
          ct->pParent->ShowWindow(SW_SHOW);
          ct->pParent->SetFocus();
        }

        m_TabMove = -1;
        ReleaseCapture();
      }

    }
  }
}

BOOL CContainer::OnLButtonDown(UINT nFlags, IPOINT point)
{
  // TODO: Add your message handler code here and/or call default
  for (int iCont = 0; iCont < m_Numtabs; iCont++) {
    CTab* ct = m_tab[iCont];

    if (ct->rect.PtInRect(point) != 0) {
      SetCurtab(iCont);
      m_InTab = TRUE;
      m_DragTab = TRUE; //DragTab
      m_TabMove = iCont; //This is moved
      SetCapture();
      break;
    }

  }

  return TRUE;

}

void CContainer::OnNcRButtonDown(UINT nHitTest, IPOINT point)
{
  ShowMenu(point);
}



void CContainer::ShowMenu(IPOINT point)
{
  CMenu m_menu;
  IRECT rc;
  GetWindowRect(rc);

  if (((CTab*)m_pArray[m_iSelectTab])->nMenu > 0) {
    CMenu m_menu;
    m_menu.LoadMenu((LPCTSTR)((CTab*)m_pArray[m_iSelectTab])->nMenu);
    CMenu m_SubMenu = m_menu.GetSubMenu(0);
    IRECT rc = ((CTab*)m_pArray[m_iSelectTab])->rect;
    m_SubMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL,
        point.x, point.y, theApp->GetMainWindows()->GetSafeHwnd(), &rc);

  }

}

BOOL CContainer::OnRButtonDown(UINT nFlags, IPOINT point)
{
  ClientToScreen(&point);
  ShowMenu(point);
  return TRUE;
}


void CContainer::SetStyle(Style estyle)
{
  m_style = estyle;
}

#endif

};

