
#define SCLOSE  0x9998
class  CMMedia : public CFolder
{

  img_t*  m_img;
  CToolButton m_toolBtnC;
public:


void CMMedia::DrawFolder(img_t* im, const IRECT* pclip, CFolderBar* cfb, int m_Style, const font_t* fo)
{
  IRECT rc;
  IRECT rcWin;
  rc = cfb->m_rect;
  rcWin = cfb->m_rect;
  int nNuImage = cfb->m_img;
  PrepareReverseVert(im, pclip, rc, GUISTYLE_XP);

  IRECT rct;
  rct = rc;
  rct.t = rct.b - 1;
  imdraw_Draw3dRect(im, pclip, &rct, GetRGBColorShadow(), GetRGBColorShadow(), 0);
  rct = rc;
  rct.t += (RCH(&rct) / 2) - 6;
  rct.b = rct.t + 9;
  rct.l += 4;
  rct.r = rct.l + 9;
  IRECT rctemp;
  rctemp = rct;
  imdraw_Draw3dRect(im, pclip, &rct, GetRGBColorShadow(), GetRGBColorShadow(), 0);
  iRectDeflate2(&rct, 1, 1);
  PrepareVertical(im, pclip, rct, GUISTYLE_XP);

  if (m_iSelected == 1) {
    rct = rctemp;
    rct.t += RCH(&rct) / 2;
    rct.b = rct.t + 1;
    rct.l += 2;
    rct.r -= 2;
    imdraw_Draw3dRect(im, pclip, &rct, GetRGBColorShadow(), GetRGBColorShadow(), 0);
  }
  else {
    rct = rctemp;
    rct.t += RCH(&rct) / 2;
    rct.b = rct.t + 1;
    rct.l += 2;
    rct.r -= 2;
    imdraw_Draw3dRect(im, pclip, &rct, GetRGBColorShadow(), GetRGBColorShadow(), 0);
    rct = rctemp;
    rct.t += 2;
    rct.b -= 2;
    rct.l += (RCW(&rct) / 2);
    rct.r = rct.l + 1;
    imdraw_Draw3dRect(im, pclip, &rct, GetRGBColorShadow(), GetRGBColorShadow(), 0);
  }


  COLOR crTextColor;
  crTextColor = (_GetSysColor(CLR_MENUTEXT));

  //guardar espacio para la imagen
  char m_cadBreak[64];
  memset(m_cadBreak, 0x00, 64);
  strcpy(m_cadBreak, cfb->m_lpMsg);
  int contt;
  int cont = contt = strlen(m_cadBreak);
  ISIZE coor = font_text_size(fo, m_cadBreak, cont, 0);
  rc.l += 16;

  if (coor.w > RCW(&rc)) {
    rc.l += 1;

    while (cont > 1) {
      char m_scadtemp[64];
      memset(m_scadtemp, 0x00, 64);
      strcpy(m_scadtemp, m_cadBreak);
      strcat(m_scadtemp, _T("..."));
      ISIZE coor = font_text_size(fo, m_scadtemp, -1, 0);

      if (coor.w > RCW(&rc)) {
        *(m_cadBreak + cont) = NULL;
      }
      else {
        break;
      }

      cont--;
    }

    lstrcat(m_cadBreak, _T("..."));
    rc.r += 3;


    imdraw_DrawText(im, pclip, m_cadBreak, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER, fo, crTextColor);
  }
  else {
    imdraw_DrawText(im, pclip, cfb->m_lpMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER, fo, crTextColor);
  }
}

void CMMedia::DrawGripper(img_t* im, const IRECT* pclip, IRECT rcWin, BOOL bHorz)
{
  if (bHorz) {
    IRECT rcBlack;
    rcWin.t += 7;
    rcWin.l += 3;
    rcWin.r = rcWin.l + 2;
    rcWin.b -= 4;

    for (int i = 0; i < RCH(&rcWin); i += 4) {
      IRECT rcWindow;
      COLOR cb;
      cb = (_GetSysColor(CLR_BTNHIGHLIGHT));
      rcWindow = rcWin;
      rcWindow.t = rcWin.t + i;
      rcWindow.b = rcWindow.t + 2;
      imdraw_FillRect(im, pclip, &rcWindow, cb);
      rcBlack = rcWindow;
      rcBlack.l -= 1;
      rcBlack.t = (rcWin.t + i) - 1;
      rcBlack.b = rcBlack.t + 2;
      rcBlack.r = rcBlack.l + 2;
      cb = (GetRGBColorShadow());
      imdraw_FillRect(im, pclip, &rcBlack, cb);
    }
  }
  else {
    rcWin.t += 3;
    rcWin.l += 3;
    rcWin.r -= 2;
    rcWin.b = rcWin.t + 2;
    IRECT rcBlack;

    for (int i = 0; i < RCW(&rcWin); i += 4) {
      IRECT rcWindow;
      COLOR cb;
      cb = (_GetSysColor(CLR_BTNHIGHLIGHT));
      rcWindow = rcWin;
      rcWindow.l = rcWindow.l + i;
      rcWindow.r = rcWindow.l + 2;
      imdraw_FillRect(im, pclip, &rcWindow, cb);
      rcBlack = rcWindow;
      rcBlack.t -= 1;
      rcBlack.l -= 1;
      rcBlack.b = rcBlack.t + 2;
      rcBlack.r = rcBlack.l + 2;
      cb = (GetRGBColorShadow());
      imdraw_FillRect(im, pclip, &rcBlack, cb);
    }
  }
}

};

