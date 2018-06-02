
class CMiniDock;
class DockManager;


class CFrame
{
public:
  void*     m_ClientView;
  IPOINT m_oldPost;
  //CReBarCtrlEx* m_ReBar;
  //  CArray      m_dockBars;
  //CStatusCtrlEx m_statusCtrl;
  //HWND      m_SubMenuActual;
  //CMiniDock*    m_MiniFrame;
  //HWND hMDIClient;
  DockManager*  m_DockLeft;
  DockManager*  m_DockBottom;
  DockManager*  m_DockRight;
  img_t*    m_imgCtrl;
  ISIZE     m_sizeImagDocks;

public:

  // COOLMENU SUPPORT
  //CMenuSpawn  cSpawn;
  IRECT oldMenuBarRc;
  bool bMenuLooping;
  UINT uiTimerId;
  // END COOLMENU SUPPORT


public:

  CFrame() {
    m_ClientView = NULL;
    oldMenuBarRc = iRECT(0, 0, 0, 0);
    bMenuLooping = false;
    uiTimerId = 0;
    //m_ReBar = NULL;
    m_ClientView = NULL;
    m_DockLeft = NULL;
    m_DockBottom = NULL;
    m_DockRight = NULL;
    m_sizeImagDocks = iSIZE(0, 0);
  }

  void DrawIconDocks(img_t* im, const IRECT* pclip, int numIcon, IPOINT cp) {
    if (m_imgCtrl) {
      imdraw_imagelist2(im, pclip, cp.x, cp.y, m_imgCtrl, numIcon, 0);
    }
  }


};


//----------------------------------------
//Docking windows routines
//----------------------------------------
class CMiniDock: public CFrame
{
public:
  CMiniDock() {
    m_Drag = FALSE;
    bDrawNormal = FALSE;
    bDrawTab = FALSE;
    bDrawNormalDock = FALSE;
  }

protected:
  BOOL m_Drag;
  IRECT m_Oldrc; //Old Rect Normal float
  IRECT m_OldrcTab; //Old Rect Tab
  IRECT m_OldrcDock; //Old Rect Docking
  IRECT mWin;
  BOOL  bDrawNormal;
  BOOL  bDrawTab;
  BOOL  bDrawNormalDock;
public:



};


class CDockPanelBase
{
public:
  IRECT rcPosition;
  UINT  uAling;
  int   iLasPosition;
  BOOL  bIsPartOfTabbed;
  BOOL  bIsShow;
  UINT   m_IdCtrlParent;

public:
  CDockPanelBase() {
    rcPosition = iRECT(0, 0, 0, 0);
    bIsShow = TRUE;
    uAling = -1;
    iLasPosition = -1;
    bIsPartOfTabbed = FALSE;
    m_IdCtrlParent = 0;

  }

public:

};


struct CDockStruct
{
  IRECT m_rectPost;
  void* m_WinDock;
};


class CContainer;
class DockAutoHide
{
public:
  //CArray m_DockWindows;
  int    n_numBars;
  UINT    m_uID;
protected:

};

class DockManager
{
  CMiniDock* m_MiniFloat;
  //CArray m_DockWindows;
  //NewBValArray<IRECT> m_Splits;
  DWORD     m_Aling;    //Actual Aling
  DWORD     m_lastAling;  //before float where was the windows
  DWORD     m_Side;
  BOOL      m_Visible;    //Is the Windows visible
  CMiniDock*  m_MiniDock;
  IRECT     m_Splitter;
  IRECT     m_OldSplitter;
  ISIZE     m_size;
  //CWin*     m_pParent;
  int     m_NumWins;
  int     m_NumSep;
  BOOL      m_DragSplitter;
  BOOL      m_DragSplitterInside;
  IPOINT    m_ptStartPos;
  IPOINT    m_ptOldPos;
  int     m_SplitMove;
  BOOL      m_ShowMe;

  BOOL ShowMe() {
    return m_ShowMe;
  };

};

// CHookMenu
// if you want to learn more about it go to
//http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dnwui/html/msdn_hooks32.asp
//http://msdn.microsoft.com/library/default.asp?url=/library/en-us/winui/WinUI/WindowsUserInterface/Windowing/Hooks/UsingHooks.asp

int Menu_Print(img_t* im, const IRECT* pclip, IRECT rc)
{
  IRECT rect = rc;
  COLOR pen;
  //aqui debe utilizarse la brocha que define CDrawLayer, si no hacemos la siguiente
  //linea usted vera un horrible color negro, a cambio del dibujo.
  
  pen = GetRGBColorShadow();
  imdraw_rect(im, pclip, rect, 0, pen, 1, 0);
  iRectDeflate2(&rect, 1, 1);
  imdraw_Draw3dRect(im, pclip, &rect, GetRGBColorWhite(), GetRGBColorWhite(), 0);
  iRectDeflate2(&rect, 1, 1);
  imdraw_Draw3dRect(im, pclip, &rect, GetRGBColorWhite(), GetRGBColorWhite(), 0);
  iRectInflate2(&rect, 2, 2);
  return 0;
}
int Menu_NCPAINT(img_t* im, const IRECT* pclip, IRECT rc) {
  IRECT rect = rc;
  COLOR pen;
  IRECT rect1 = rect;
  pen = GetRGBColorShadow();
  imdraw_rect(im, pclip, rect, 0, pen, 1, 0);
  iRectDeflate2(&rect, 1, 1);
  imdraw_Draw3dRect(im, pclip, &rect, GetRGBColorWhite(), GetRGBColorWhite(), 0);
  iRectDeflate2(&rect, 1, 1);
  int bottom = rect.b;
  rect.b = rect.t + 1;
  imdraw_Draw3dRect(im, pclip, &rect, GetRGBColorWhite(), GetRGBColorWhite(), 0);
  rect.b = bottom;
  rect.l = rect.r - 1;
  imdraw_Draw3dRect(im, pclip, &rect, GetRGBColorWhite(), GetRGBColorWhite(), 0);
  
  if (1) { //;Opc==-4)
    rect1.l += 1;
    rect1.r = rect1.l + RCW(&rect) - 1;
    rect1.b = rect1.t;
    imdraw_Draw3dRect(im, pclip, &rect1, GetRGBColorXP(), GetRGBColorXP(), 0);
    
  }
  return 0;
}

