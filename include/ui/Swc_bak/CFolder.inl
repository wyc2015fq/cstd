

//**********************************************************
typedef struct CFolderBar
{
  void* m_CWin;
  const char*  m_lpMsg;
  UINT  m_img;
  IRECT m_rect;
  BOOL  m_posUp;
} CFolderBar;

enum CFolderBarStyle {
  mNormal = 0,
    mPush = 1,
    mFlat = 2
};
enum TypeFolder {
  mOutlook = 0,
    mToolBox = 1
};

class  CFolder
{
public:
  CFolderBar*   m_FolderBar;
  int m_FolderBar_count;
  int m_NumFolders;   //total de folders
  int m_iSelected;
  int m_typel;  //tipo de look

public:
void CFolder::Init()
{
  m_typel = mOutlook; //by default
}

void CFolder::DrawFolder(img_t* im, const IRECT* pclip, const CFolderBar* cfb, int m_Style, const font_t* fo)
{
  IRECT rc = cfb->m_rect;
  COLOR cb;
  cb = (CLR_BTNFACE);

  imdraw_FillRect(im, pclip, &rc, cb);
  PrepareVertical(im, pclip, rc);
  char m_cadBreak[64];
  memset(m_cadBreak, 0x00, 64);
  strcpy(m_cadBreak, cfb->m_lpMsg);
  int contt;
  int cont = contt = strlen(m_cadBreak);
  ISIZE coor = font_text_size(fo, m_cadBreak, cont, 0);

  if (coor.w > RCW(&rc)) {
    rc.l += 1;

    while (cont > 1) {
      char m_scadtemp[64];
      memset(m_scadtemp, 0x00, 64);
      lstrcpy(m_scadtemp, m_cadBreak);
      lstrcat(m_scadtemp, _T("..."));
      ISIZE coor = font_text_size(fo, m_scadtemp, -1, 0);

      if (coor.w > RCW(&rc)) {
        *(m_cadBreak + cont) = NULL;
      }
      else {
        break;
      }

      cont--;
    }

    strcat(m_cadBreak, _T("..."));
    rc.r += 3;

    imdraw_DrawText(im, pclip, m_cadBreak, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER, fo, Color_Black);
  }
  else {
    if (m_typel != mOutlook) {
      imdraw_DrawText(im, pclip, cfb->m_lpMsg, rc, DT_SINGLELINE | DT_LEFT | DT_VCENTER, fo, Color_Black);
    }
    else {
      imdraw_DrawText(im, pclip, cfb->m_lpMsg, rc, DT_WORDBREAK | DT_SINGLELINE | DT_CENTER | DT_VCENTER, fo, Color_Black);
    }
  }
}


BOOL CFolder::OnPaint(img_t* im, const IRECT* pclip, const font_t* fo)
{
  int m_iNumFolder = m_FolderBar_count;

  for (int iCont = 0; iCont < m_iNumFolder; iCont++) {
    if (m_iSelected == iCont) {
      DrawFolder(im, pclip, m_FolderBar+iCont, mFlat, fo);
    }
    else {
      DrawFolder(im, pclip, m_FolderBar+iCont, mFlat, fo);
    }

  }

  return TRUE;
}

int CFolder::PointInFolder(int x, int y)
{
  for (int i = 0; i < m_FolderBar_count; i++) {
    if (iPtInRect(&m_FolderBar[i].m_rect, x, y)) {
      return i;
    }
  }

  return -1;
}


void CFolder::RecalLayout(IRECT rc)
{
  IRECT m_rect = rc;
  IRECT m_rectFolder;
  int m_iPosDown = 0; //a partir de donde se ubica el anterior folder
  int m_iposUp = 0; //a partir de donde se ubica el siguiente folder
  int m_iNumFolder = m_FolderBar_count;
  m_rectFolder = m_rect;
  m_iPosDown = m_rect.b;

  for (int iCont = 0; iCont < m_iNumFolder; iCont++) {
    CFolderBar* cfd = m_FolderBar+iCont;

    if (iCont == 0) { //inevitablemente arriba
      m_rectFolder.b = m_rect.t + 20;
      m_iposUp += RCH(&m_rectFolder) - 1;
      cfd->m_rect = m_rectFolder;
    }
    else {
      if (cfd->m_posUp) {
        m_rectFolder.t = m_iposUp;
        m_rectFolder.b = m_rectFolder.t + 20;
        m_iposUp += RCH(&m_rectFolder);
        cfd->m_rect = m_rectFolder;
      }
      else {

        for (int iRev = m_iNumFolder - 1; iRev >= iCont; iRev--) {
          CFolderBar* cfd = m_FolderBar+iRev;
          m_rectFolder.b = m_iPosDown;
          m_rectFolder.t = m_rectFolder.b - 20;
          m_iPosDown -= RCH(&m_rectFolder);
          cfd->m_rect = m_rectFolder;
        }

        break;
      }
    }
  }
}


BOOL CFolder::OnEraseBkgnd(img_t* im, const IRECT* pclip, IRECT rc)
{
  COLOR cbr;
  cbr = GetRGBColorFace();
  IRECT m_rect = rc;

  if (m_NumFolders <= 0) {
    imdraw_FillRect(im, pclip, &m_rect, cbr);
  }
  else {
    for (int i = 0; i < 2; i++) {
      imdraw_Draw3dRect(im, pclip, &m_rect, GetRGBColorFace(), GetRGBColorFace(), 0);
      iRectDeflate2(&m_rect, 1, 1);
    }
  }

  return TRUE;
}

};

