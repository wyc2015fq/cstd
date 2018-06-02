

#define SCROLLCLOSE 0x886

enum ExtTab {NORMAL_TAB = 0, COMBO_TAB = 1};
class  CTabbed : public CContainer
{
protected:
  CToolButton m_toolBtn[2];
  CToolButton m_toolBtnC;
  img_t*   m_img;
  ISIZE        m_sizeBtn;
  int      m_nDif;
  int     m_ExtTab;
  int m_Numtabs;
  int     m_count;
  int     m_doc;
  const font_t* m_cfontNormal;
  const font_t* m_cfontBold;
public:


void CTabbed::Init(void)
{
  m_nDif = 0;
  m_count = 0;
  m_doc = 1;
}

BOOL CTabbed::OnEraseBkgnd(img_t* im, const IRECT* pclip, IRECT m_rect)
{
  // TODO: Add your message handler code here and/or call default
  if (m_Numtabs == 0) {
    COLOR cbr;
    cbr = (CLR_BTNSHADOW);
    m_rect.t -= 22;
    imdraw_FillRect(im, pclip, &m_rect, cbr);
  }

  return TRUE;
}



ISIZE CTabbed::GetSizeButton()
{
  ISIZE szresult = iSIZE(0, 0);
  
  for (int i = 0; i < 3; i++) {
    ISIZE sztemp = m_toolBtn[i].GetSizeButton();
    szresult.w += sztemp.w;
    szresult.h += sztemp.h;
  }
  
  return szresult;
}

void CTabbed::Drawtabs(img_t* im, const IRECT* pclip, IRECT m_rect, const font_t* fo)
{
  if (m_Numtabs == 0) {
    COLOR cbr;
    cbr = (CLR_BTNSHADOW);
    imdraw_FillRect(im, pclip, &m_rect, cbr);
    return;
  }
  else {
    COLOR cbr;
    cbr = (CLR_BTNFACE);
    imdraw_FillRect(im, pclip, &m_rect, cbr);
  }

  int m_StrPos = 0;
  BOOL m_ViewAlltab = TRUE;
  COLOR Dark;

  Dark = (GetRGBColorShadow());
  COLOR hPen = (Dark);
  IRECT m_rcTabMain;
  ISIZE mszBtn = GetSizeButton();
  int m_iSizeButton = mszBtn.w;

  iRectDeflate2(&m_rectTab, 1, 1);
  m_rectTab.b += 1;

  IRECT rectT = m_rectTab;
  rectT.r -= m_iSizeButton + 2;
  int mSizeTotal = m_iMinValRec;
  
  if (mSizeTotal > RCW(&m_rectTab)) {
    //m_toolBtn[1].EnableWindow(TRUE);

    if (m_nDif < 0) {
      //m_toolBtn[0].EnableWindow(TRUE);
    }

    if ((mSizeTotal - (abs(m_nDif) - 20)) < RCW(&m_rectTab)) {
      //m_toolBtn[1].EnableWindow(FALSE);
    }
  }
  else {
    m_nDif = 0;
  }

  int m_right = m_nDif;

  for (int iCont = 0; iCont < m_Numtabs; iCont++) {
    IRECT mrt;
    mrt = m_tab[iCont].rect;
    int ancho = RCW(&mrt);
    mrt.l = m_right;
    mrt.r = mrt.l + ancho;

    //si es la carpeta seleccionada, se debe
    //eliminar las lineas oscuras de encima
    if ((iCont == m_iSelectTab) || (iCont == 0)) {

      IRECT  m_rectTabAux;
      m_rectTabAux = mrt;
      m_rcTabMain = m_rectTabAux;

      if (iCont == m_iSelectTab && iCont != 0) {
        int aaa[] = {
          m_rectTabAux.l + 3, m_rectTabAux.b + 2, RCW(&m_rectTabAux) - 2, RCH(&m_rectTabAux),
            m_rectTabAux.l + 4, m_rectTabAux.b + 1, RCW(&m_rectTabAux) - 2, RCH(&m_rectTabAux),
            m_rectTabAux.l - 12, m_rectTabAux.b, RCW(&m_rectTabAux) - 2, RCH(&m_rectTabAux),
            m_rectTabAux.l - 11, m_rectTabAux.b - 1, RCW(&m_rectTabAux) - 2, RCH(&m_rectTabAux),
            m_rectTabAux.l - 10, m_rectTabAux.b - 2, RCW(&m_rectTabAux) - 2, RCH(&m_rectTabAux),
            m_rectTabAux.l - 9, m_rectTabAux.b - 3, RCW(&m_rectTabAux) - 2, RCH(&m_rectTabAux),
            m_rectTabAux.l - 8, m_rectTabAux.b - 4, RCW(&m_rectTabAux) - 2, RCH(&m_rectTabAux),
            m_rectTabAux.l - 7, m_rectTabAux.b - 5, RCW(&m_rectTabAux) - 2, RCH(&m_rectTabAux),
            m_rectTabAux.l - 6, m_rectTabAux.b - 6, RCW(&m_rectTabAux) - 2, RCH(&m_rectTabAux),
            m_rectTabAux.l - 5, m_rectTabAux.b - 7, RCW(&m_rectTabAux) - 2, RCH(&m_rectTabAux),
            m_rectTabAux.l - 4, m_rectTabAux.b - 8, RCW(&m_rectTabAux) - 2, RCH(&m_rectTabAux),
            m_rectTabAux.l - 3, m_rectTabAux.b - 9, RCW(&m_rectTabAux) - 2, RCH(&m_rectTabAux),
            
            m_rectTabAux.l - 2, m_rectTabAux.b - 10, RCW(&m_rectTabAux) - 2, RCH(&m_rectTabAux),
            m_rectTabAux.l, m_rectTabAux.t + 7, RCW(&m_rectTabAux) - 2, RCH(&m_rectTabAux),
            m_rectTabAux.l - 1, m_rectTabAux.t + 8, RCW(&m_rectTabAux) - 2, RCH(&m_rectTabAux),
            m_rectTabAux.l - 2, m_rectTabAux.t + 9, RCW(&m_rectTabAux) - 2, RCH(&m_rectTabAux),
            
            m_rectTabAux.l + 2, m_rectTabAux.t + 6, RCW(&m_rectTabAux) - 2, RCH(&m_rectTabAux),
            m_rectTabAux.l + 4, m_rectTabAux.t + 5, RCW(&m_rectTabAux) - 2, RCH(&m_rectTabAux),
            m_rectTabAux.l + 5, m_rectTabAux.t + 4, RCW(&m_rectTabAux) - 2, RCH(&m_rectTabAux),
            m_rectTabAux.l + 6, m_rectTabAux.t + 3, RCW(&m_rectTabAux) - 4, RCH(&m_rectTabAux),
        };
        for (int i=0; i<countof(aaa); i+=4) {
          int* aa = aaa+i;
          IRECT rc1 = iRECT2(aa[0], aa[1], aa[2], aa[3]);
          PrepareTabs(im, pclip, rc1, GetRGBColorWhite(), GetRGBColorWhite());
        }
        imdraw_line2(im, pclip, m_rectTabAux.l - 15, m_rectTabAux.b + 1, m_rectTabAux.l + 2, m_rectTabAux.t + 5, Color_Black, 1);
        imdraw_line2(im, pclip, m_rectTabAux.l + 2, m_rectTabAux.t + 5, m_rectTabAux.l + 7, m_rectTabAux.t + 3, Color_Black, 1);
        imdraw_line2(im, pclip, m_rectTabAux.l + 7, m_rectTabAux.t + 3, m_rectTabAux.r + 2, m_rectTabAux.t + 3, Color_Black, 1);
      }
      else {
        if (iCont != m_iSelectTab) {
          PrepareTabs(im, pclip, m_rectTabAux, GetRGBColorWhite(), GetRGBSemiShadow());
        }
        else {
          int aaa[] = {
            m_rectTabAux.l + 4, m_rectTabAux.b + 2, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
              m_rectTabAux.l + 5, m_rectTabAux.b + 1, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
              m_rectTabAux.l + 6, m_rectTabAux.b, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
              m_rectTabAux.l + 7, m_rectTabAux.b - 1, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
              m_rectTabAux.l + 8, m_rectTabAux.b - 2, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
              m_rectTabAux.l + 9, m_rectTabAux.b - 3, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
              m_rectTabAux.l + 10, m_rectTabAux.b - 4, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
              m_rectTabAux.l + 11, m_rectTabAux.b - 5, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
              m_rectTabAux.l + 15, m_rectTabAux.b - 6, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
              m_rectTabAux.l + 13, m_rectTabAux.b - 7, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
              m_rectTabAux.l + 14, m_rectTabAux.b - 8, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
              m_rectTabAux.l + 15, m_rectTabAux.b - 9, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
              m_rectTabAux.l + 16, m_rectTabAux.b - 10, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
              m_rectTabAux.l + 17, m_rectTabAux.b - 11, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
              m_rectTabAux.l + 18, m_rectTabAux.b - 12, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
              m_rectTabAux.l + 19, m_rectTabAux.b - 13, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
              m_rectTabAux.l + 20, m_rectTabAux.b - 14, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
              m_rectTabAux.l + 21, m_rectTabAux.b - 15, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
              m_rectTabAux.l + 22, m_rectTabAux.b - 16, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
              m_rectTabAux.l + 23, m_rectTabAux.b - 17, RCW(&m_rectTabAux) - 20, RCH(&m_rectTabAux),
          m_rectTabAux.l + 27, m_rectTabAux.b - 18, RCW(&m_rectTabAux) - 26, RCH(&m_rectTabAux),
          };
          for (int i=0; i<countof(aaa); i+=4) {
            int* aa = aaa+i;
            IRECT rc1 = iRECT2(aa[0], aa[1], aa[2], aa[3]);
            PrepareTabs(im, pclip, rc1, GetRGBColorWhite(), GetRGBColorWhite());
          }
        }
        imdraw_line2(im, pclip, m_rectTabAux.l + 3, m_rectTabAux.b + 1, m_rectTabAux.l + 20, m_rectTabAux.t + 5, Color_Black, 1);
        imdraw_line2(im, pclip, m_rectTabAux.l + 20, m_rectTabAux.t + 5, m_rectTabAux.l + 27, m_rectTabAux.t + 3, Color_Black, 1);
        imdraw_line2(im, pclip, m_rectTabAux.l + 27, m_rectTabAux.t + 3, m_rectTabAux.r + 2, m_rectTabAux.t + 3, Color_Black, 1);
      }

      imdraw_line2(im, pclip, m_rectTabAux.r + 1, m_rectTabAux.t + 3, m_rectTabAux.r + 5, m_rectTabAux.t + 5, Color_Black, 1);
      imdraw_line2(im, pclip, m_rectTabAux.r + 4, m_rectTabAux.t + 5, m_rectTabAux.r + 4, m_rectTabAux.b + 3, Color_Black, 1);

      //linea desde el punto inferior derecho del tab hasta el final del area de tabs
      imdraw_line2(im, pclip, m_rectTabAux.r + 4, m_rectTabAux.b + 1, m_rectTab.r, m_rectTabAux.b + 1, Color_Black, 1);
      imdraw_line2(im, pclip, m_rectTab.l, m_rectTabAux.b + 1, m_rectTabAux.l - 14, m_rectTabAux.b + 1, Color_Black, 1);
    }
    else {
      IRECT  m_rectTabAux = mrt;
      m_rcTabMain = m_rectTabAux;
      //CGradient M(ISIZE(m_rectTabAux.Width()-4,m_rectTabAux.Height()));
      //M.PrepareTabs(dc,GetRGBColorWhite(),GetRGBColorFace());
      //M.Draw(dc,m_rectTabAux.l+6,m_rectTabAux.t+2,0,0,m_rectTabAux.Width(),m_rectTabAux.Height()-2,SRCCOPY);
      {
        imdraw_line2(im, pclip, m_rectTabAux.l + 4, m_rectTabAux.t + 6, m_rectTabAux.l + 8, m_rectTabAux.t + 2, Color_Black, 1);
        imdraw_line2(im, pclip, m_rectTabAux.l + 8, m_rectTabAux.t + 3, m_rectTabAux.r + 1, m_rectTabAux.t + 3, Color_Black, 1);
        imdraw_line2(im, pclip, m_rectTabAux.r + 1, m_rectTabAux.t + 3, m_rectTabAux.r + 4, m_rectTabAux.t + 5, Color_Black, 1);
        imdraw_line2(im, pclip, m_rectTabAux.r + 4, m_rectTabAux.t + 5, m_rectTabAux.r + 4, m_rectTabAux.b + 3, Color_Black, 1);
      }
    }

    IRECT m_rectTabAux = mrt;
    m_rectTabAux.t += 4;
    m_rectTabAux.l += 4;

    COLOR clrtext = _RGB(0, 0, 0);

    if (iCont != m_iSelectTab) {
      clrtext = (_RGB(0, 0, 0));
    }

    if (iCont == m_iSelectTab) {
    }

    IRECT rcW;
    rcW = m_rectTabAux;
    rcW.l += 5;
    imdraw_DrawText(im, pclip, m_tab[iCont].lpMsg, m_rectTabAux, DT_SINGLELINE | DT_CENTER | DT_VCENTER, fo, Color_Black);

    m_right = mrt.r;
    m_tab[iCont].rect = mrt;
  }

  imdraw_line2(im, pclip, m_rectTab.r - (m_iSizeButton + 2), m_rcTabMain.b + 2, m_rectTab.r, m_rcTabMain.b + 2, Color_Black, 1);
  IRECT rcCli;
  rcCli = m_rect;
  rcCli.t = m_rectTab.t;
  rcCli.b = m_rectTab.b;
  rcCli.l = rcCli.r - (m_iSizeButton);
  int nacum = 0;

  for (int i = 0; i < 3; i++) {
    ISIZE szBtn = m_toolBtn[i].GetSizeButton();
    //m_toolBtn[i].MoveWindow(rcCli.l + nacum - 1, rcCli.t + 2, szBtn.w, rcCli.Height() + 1);
    nacum += szBtn.w;
  }

  rcCli = m_rect;
  imdraw_Draw3dRect(im, pclip, &rcCli, GetRGBColorShadow(), GetRGBColorShadow(), 0);
}

};
