
// CGuiHeaderCtrl window

typedef struct HeaderItem {
  IRECT rc;
  int iImage;
  const char* text;
} HeaderItem;

class CHeaderCtrl
{
  HeaderItem* m_pItem;
  COLOR  m_clrFace;
  COLOR  m_clrShadow;
  COLOR  m_clrLight;
  img_t*  m_img;
  int     m_nNumbottomSelect;
  int     m_iItemCount;
  BOOL    m_bSentido;
  BOOL    m_bMouserOver;
  int     m_selected;
  font_t* m_cfontNormal;
  font_t* m_cfontBold;

public:
// CHeaderCtrl
void CHeaderCtrl::Init()
{
  m_clrFace = _GetSysColor(CLR_3DFACE);
  m_clrShadow = _GetSysColor(CLR_BTNSHADOW);
  m_clrLight = _GetSysColor(CLR_BTNHIGHLIGHT);
  m_nNumbottomSelect = -1;
  m_bSentido = TRUE;
  m_bMouserOver = FALSE;
  m_selected = -1;
}


// CHeaderCtrl message handlers

//********************************************************************************
void CHeaderCtrl::DrawArrow(img_t* im, const IRECT* pclip, IRECT rc, BOOL bUp)
{
  COLOR cp;
  COLOR cps;
  COLOR cpw;

  cp = (m_clrFace);
  cps = (m_clrShadow);
  cpw = (m_clrLight);

  rc.l = rc.r - 12;
  rc.r = rc.l + 8;
  rc.b = rc.t + 12;
  rc.t += 2;
  int m_mitad = rc.l + 4;

  if (bUp == TRUE) {
    //linea izquierda
    imdraw_line2(im, pclip, rc.l, rc.b, m_mitad, rc.t, cps, 1);
    //linea derecha
    imdraw_line2(im, pclip, rc.r, rc.b, m_mitad, rc.t, cpw, 1);
    //linea de abajo
    imdraw_line2(im, pclip, rc.l, rc.b, rc.r, rc.b, cpw, 1);
  }
  else {
    rc.b = rc.t + 12;
    rc.t += 4;
    //linea izquierda
    imdraw_line2(im, pclip, rc.l, rc.t, m_mitad, rc.b, cps, 1);
    //linea superior
    imdraw_line2(im, pclip, rc.l, rc.t, rc.r, rc.t, cps, 1);
    //linea derecha
    imdraw_line2(im, pclip, rc.r, rc.t, m_mitad, rc.b, cpw, 1);

  }

}

void CHeaderCtrl::DrawTextExt(img_t* im, const IRECT* pclip, LPCSTR Cad, IRECT* rc, const font_t* fo)
{
  TCHAR m_cadBreak[64];
  memset(m_cadBreak, 0x00, 64);
  strcpy(m_cadBreak, Cad);
  int contt;
  int cont = contt = strlen(m_cadBreak);
  ISIZE coor = font_text_size(fo, m_cadBreak, cont, 0);
  
  if (coor.w > RCW(rc)) {
    rc->l += 1;
    
    while (cont > 1) {
      TCHAR m_scadtemp[64];
      memset(m_scadtemp, 0x00, 64);
      strcpy(m_scadtemp, m_cadBreak);
      strcat(m_scadtemp, _T("..."));
      ISIZE coor = font_text_size(fo, m_scadtemp, -1, 0);
      
      if (coor.w > RCW(rc)) {
        *(m_cadBreak + cont) = NULL;
      }
      else {
        break;
      }
      
      cont--;
    }
    
    strcat(m_cadBreak, _T("..."));
    
    rc->r += 3;
  }
  
  imdraw_DrawText(im, pclip, m_cadBreak, *rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER, fo, Color_Black);
}

BOOL CHeaderCtrl::DrawItem(img_t* im, const IRECT* pclip, IRECT rcitem, int iOrder, int iImage, const char* pszText)
{
  ISIZE siImg = iSIZE(0, 0);

  if (m_nNumbottomSelect == iOrder) {
    DrawArrow(im, pclip, rcitem, m_bSentido);
    rcitem.r -= 12;
  }

  if (iImage > -1) {
    IRECT rcima;
    rcima = iRECT2(rcitem.l + 2, rcitem.t + 1, m_img->w, m_img->h);
    imdraw_imagelist(im, pclip, rcima, m_img, iImage, 0, 0, 0);
  }

  if (pszText) {
    IRECT m_rctext = rcitem;
    const font_t* fo = 0;
    m_rctext.l += siImg.w + 4;
    m_rctext.b -= 2;

    if (m_bMouserOver && m_selected == iOrder) {
      fo = (m_cfontBold);
    }
    else {
      fo = (m_cfontNormal);
    }

    DrawTextExt(im, pclip, pszText, &rcitem, fo);
  }

  return TRUE;
}

//************************************************************************
void CHeaderCtrl::BiselaBoton(img_t* im, const IRECT* pclip, IRECT rcWin)
{
  COLOR cp;
  COLOR cps;
  COLOR cpw;

  cp = (m_clrFace);
  cps = (GetRGBSemiShadow());
  cpw = (m_clrLight);
  /***************************************************
  imdraw_Draw3dRect(im, pclip, rcWin,m_clrLight,m_clrShadow);
  rcWin iRectDeflate2(&, 1,1);
  imdraw_Draw3dRect(im, pclip, rcWin,m_clrFace,m_clrFace);
  ***************************************************/

  int iNumItems = m_iItemCount;
  int iContx = 0;

  for (int i = 0; i < iNumItems; i++) {
    IRECT recItem;
    recItem = m_pItem[i].rc;
    iContx += RCW(&recItem);
    //quitamos todas las lineas
    iRectDeflate2(&recItem, 1, 1);
    imdraw_line2(im, pclip, iContx - 2, rcWin.t + 1, iContx - 2, rcWin.b - 1, cpw, 1);
    imdraw_line2(im, pclip, iContx - 1, rcWin.t + 1, iContx - 1, rcWin.b - 1, cps, 1);
    imdraw_line2(im, pclip, iContx, rcWin.t + 1, iContx, rcWin.b - 1, cpw, 1);
  }
}

void CHeaderCtrl::OnPaint(img_t* im, const IRECT* pclip, IRECT rc)
{
  IRECT   rcClient = rc;
  IRECT   rcWin = rc;
  COLOR    m_brush;

  m_brush = (CLR_3DFACE);
  imdraw_FillRect(im, pclip, &rcClient, m_brush); //evita el horrible color negro
  PrepareVertical(im, pclip, rcClient);

  BiselaBoton(im, pclip, rcWin);
  int iNumItems = m_iItemCount;

  for (int i = 0; i < iNumItems; i++) {
    DrawItem(im, pclip, m_pItem[i].rc, i, m_pItem[i].iImage, m_pItem[i].text);
  }

  imdraw_Draw3dRect(im, pclip, &rcClient, GetRGBSemiShadow(), GetRGBSemiShadow(), 0);
}

BOOL CHeaderCtrl::OnLButtonDown(UINT nFlags, int x, int y)
{
  int iNumItems = m_iItemCount;

  for (int i = 0; i < iNumItems; i++) {
    IRECT recItem;
    recItem = m_pItem[i].rc;

    if (iPtInRect(&recItem, x, y)) {
      if (m_nNumbottomSelect == i) {
        m_bSentido = !m_bSentido;
      }
      else {
        m_bSentido = TRUE;
      }

      m_nNumbottomSelect = i;
      break;
    }
  }

  return TRUE;
}
};
