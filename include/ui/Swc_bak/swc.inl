
#define BASE_STATUS_BAR     0x999111
#define BASE_TOOLTIPS_CLASS   0x9112
#define BASE_TOOLBAR_CLASS    0x9113
#define ID_MDI_FIRSTCHILD     0x5000
#define IDC_HSPLITBAR           30980       // splitter
#define IDC_VSPLITBAR           30981       // splitter
#define RT_TOOLBAR        MAKEINTRESOURCE(241)
#define DOCK_VERT       0x999922
#define DOCK_HORZ       0x999923
#define DOCK_LEFT       0x999924
#define DOCK_RIGHT        0x999925
#define DOCK_TOP        0x999926
#define DOCK_BOTTOM       0x999927

#define BS_FLOATING     0x00234
#define BS_DOCKING      0x00235
#define BS_DOCKINGTAB   0x00236
#define BS_DOCKINGLEFT    0x00237
#define BS_DOCKINGRIGHT   0x00238
#define BS_DOCKINGBOTTOM  0x00239

#define BS_ALIGN_LEFT 0x3100L
#define BS_ALIGN_BOTTOM 0x3101L
#define BS_ALIGN_RIGHT  0x3102L
#define BS_ALIGN_FLOAT  0x3103L

#define BS_ALIGN_VERT 0x3104L
#define BS_ALIGN_HORZ 0x3105L


#define BS_BORDER_LEFT  0x3106L
#define BS_BORDER_TOP 0x3107L
#define BS_BORDER_RIGHT 0x3108L

#define BS_SPLIT_VERT 0x3109L
#define BS_SPLIT_HORTZ  0x3110L

#define BS_AUTO_LEFT  0x3111L
#define BS_AUTO_BOTTOM  0x3112L
#define BS_AUTO_RIGHT 0x3113L
#define BS_AUTO_FLOAT 0x314L


#define WINNT     0x01
#define WIN2000   0x02
#define WINXP     0x03

#define GUISTYLE_XP   0x08001
#define GUISTYLE_2003 0x08002
#define GUISTYLE_2003MENUBTN 0x08021
#define WIN_NT      0x08003
#define WIN_2000  0x08004
#define WIN_XP    0x08005
#define WIN_95    0x08006
#define WIN_98    0x08007
#define WIN_32    0x08008
#define WIN_ME    0x08009
#define WIN_95OSR2  0x0800A
#define WIN_98_SE 0x0800B

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED           0x00080000
#define LWA_COLORKEY            0x00000001
#define LWA_ALPHA               0x00000002
#endif // ndef WS_EX_LAYERED

/*
 * CDrawlayer: This class is a wrapper of color and look for our future controls
 *
 **/

static COLOR DarkenColor(long lScale, COLOR lColor) {
  long R = MulDiv(GetRV(lColor), (255 - lScale), 255);
  long G = MulDiv(GetGV(lColor), (255 - lScale), 255);
  long B = MulDiv(GetBV(lColor), (255 - lScale), 255);
  
  return _RGB(R, G, B);
}


static COLOR GetRGBColorWhite() {
  return _RGB(255, 255, 255);
}



static COLOR GetRGBActiveBorder() {
  COLOR clrPress = _GetSysColor(CLR_INACTIVEBORDER);
  BYTE byRvalue = GetRV(clrPress);
  BYTE byGvalue = GetGV(clrPress);
  BYTE byBvalue = GetBV(clrPress);
  return _RGB(byRvalue, byGvalue, byBvalue);
}

static COLOR GetRGBColorXP() {
  COLOR clr3DFace = _GetSysColor(CLR_3DFACE);
  return _RGB(((3 * GetRV(clr3DFace) + 240) / 4) + 1,
    ((3 * GetGV(clr3DFace) + 240) / 4) + 1,
    ((3 * GetBV(clr3DFace) + 240) / 4) + 1);
}

static COLOR GetRGBColorTabs() {
  BYTE byRvalue = GetRV(GetRGBColorXP());
  BYTE byGvalue = GetGV(GetRGBColorXP());
  BYTE byBvalue = GetBV(GetRGBColorXP());
  int iMaximo = max(byRvalue, byGvalue);
  iMaximo = max(iMaximo, byBvalue);
  iMaximo = 255 - iMaximo;
  iMaximo = iMaximo - (int)(iMaximo * 0.51);
  return _RGB(byRvalue + iMaximo, byGvalue + iMaximo, byBvalue + iMaximo);
  
}

static COLOR GetRGBColorFace() {
  return _GetSysColor(CLR_BTNFACE);
}

static COLOR GetRGBColorShadow() {
  return _GetSysColor(CLR_BTNSHADOW);
}


static COLOR GetRGBSkinMenu() {
  BYTE byRvalue = GetRV(GetRGBColorFace());
  BYTE byGvalue = GetGV(GetRGBColorFace());
  BYTE byBvalue = GetBV(GetRGBColorFace());
  byRvalue = byRvalue >> 2;
  byGvalue = byGvalue >> 2;
  byBvalue = byBvalue >> 2;
  int iMaximo = max(byRvalue, byGvalue);
  iMaximo = max(iMaximo, byBvalue);
  iMaximo = 255 - iMaximo;
  iMaximo = iMaximo - (int)(iMaximo * 0.018);
  return _RGB(byRvalue + iMaximo, byGvalue + iMaximo, byBvalue + iMaximo);
  
}

static COLOR GetRGBColorBTNHigh() {
  return _GetSysColor(CLR_BTNHIGHLIGHT);
}



static COLOR GetRGBColorGrayText() {
  BYTE byRvalue = 0;
  BYTE byGvalue = 0;
  BYTE byBvalue = 0;
  byRvalue = byRvalue >> 1;
  byGvalue = byGvalue >> 1;
  byBvalue = byBvalue >> 1;
  int iMaximo = max(byRvalue, byGvalue);
  iMaximo = max(iMaximo, byBvalue);
  iMaximo = 255 - iMaximo;
  iMaximo = iMaximo - (int)(iMaximo * 0.60);
  return _RGB(byRvalue + iMaximo, byGvalue + iMaximo, byBvalue + iMaximo);
  
}


static COLOR GetRGBPressBXP() {
  
  BYTE byRvalue = GetRV(_GetSysColor(CLR_HIGHLIGHT));
  BYTE byGvalue = GetGV(_GetSysColor(CLR_HIGHLIGHT));
  BYTE byBvalue = GetBV(_GetSysColor(CLR_HIGHLIGHT));
  byRvalue = byRvalue >> 2;
  byGvalue = byGvalue >> 2;
  byBvalue = byBvalue >> 2;
  int iMaximo = max(byRvalue, byGvalue);
  iMaximo = max(iMaximo, byBvalue);
  iMaximo = 255 - iMaximo;
  iMaximo = iMaximo - (int)(iMaximo * 0.21);
  return _RGB(byRvalue + iMaximo, byGvalue + iMaximo, byBvalue + iMaximo);
  
}

static COLOR GetRGBMenu() {
  BYTE byRvalue = GetRV(_GetSysColor(CLR_HIGHLIGHT));
  BYTE byGvalue = GetGV(_GetSysColor(CLR_HIGHLIGHT));
  BYTE byBvalue = GetBV(_GetSysColor(CLR_HIGHLIGHT));
  byRvalue = byRvalue >> 3;
  byGvalue = byGvalue >> 3;
  byBvalue = byBvalue >> 3;
  int iMaximo = max(byRvalue, byGvalue);
  iMaximo = max(iMaximo, byBvalue);
  iMaximo = 255 - iMaximo;
  iMaximo = iMaximo - (int)(iMaximo * 0.47);
  return _RGB(byRvalue + iMaximo, byGvalue + iMaximo, byBvalue + iMaximo);
  
}

static COLOR GetRGBTitleMenu() {
  COLOR clrPress = _GetSysColor(CLR_BTNSHADOW);
  
  BYTE byRvalue = GetRV(_GetSysColor(clrPress));
  BYTE byGvalue = GetGV(_GetSysColor(clrPress));
  BYTE byBvalue = GetBV(_GetSysColor(clrPress));
  byRvalue = byRvalue >> 2;
  byGvalue = byGvalue >> 2;
  byBvalue = byBvalue >> 2;
  int iMaximo = max(byRvalue, byGvalue);
  iMaximo = max(iMaximo, byBvalue);
  iMaximo = 255 - iMaximo;
  iMaximo = iMaximo - (int)(iMaximo * 0.8);
  return _RGB(byRvalue + iMaximo, byGvalue + iMaximo, byBvalue + iMaximo);
  
}

static COLOR GetRGBCaptionXP() {
  
  BYTE byRvalue = GetRV(_GetSysColor(CLR_HIGHLIGHT));
  BYTE byGvalue = GetGV(_GetSysColor(CLR_HIGHLIGHT));
  BYTE byBvalue = GetBV(_GetSysColor(CLR_HIGHLIGHT));
  return _RGB(byRvalue + 10, byGvalue + 10, byBvalue + 10);
}

static COLOR GetRGBFondoXP() {
  BYTE byRvalue = GetRV(_GetSysColor(CLR_HIGHLIGHT));
  BYTE byGvalue = GetGV(_GetSysColor(CLR_HIGHLIGHT));
  BYTE byBvalue = GetBV(_GetSysColor(CLR_HIGHLIGHT));
  byRvalue = byRvalue >> 2;
  byGvalue = byGvalue >> 2;
  byBvalue = byBvalue >> 2;
  int iMaximo = max(byRvalue, byGvalue);
  iMaximo = max(iMaximo, byBvalue);
  iMaximo = 255 - iMaximo;
  iMaximo = iMaximo - (int)(iMaximo * 0.14);
  return _RGB(byRvalue + iMaximo, byGvalue + iMaximo, byBvalue + iMaximo);
}


static COLOR GetRGBGray() {
  BYTE byRvalue = GetRV(_GetSysColor(CLR_BTNSHADOW));
  BYTE byGvalue = GetGV(_GetSysColor(CLR_BTNSHADOW));
  BYTE byBvalue = GetBV(_GetSysColor(CLR_BTNSHADOW));
  byRvalue = byRvalue >> 2;
  byGvalue = byGvalue >> 2;
  byBvalue = byBvalue >> 2;
  int iMaximo = max(byRvalue, byGvalue);
  iMaximo = max(iMaximo, byBvalue);
  iMaximo = 255 - iMaximo;
  iMaximo = iMaximo - (int)(iMaximo * 0.30);
  return _RGB(byRvalue + iMaximo, byGvalue + iMaximo, byBvalue + iMaximo);
  //return _RGB(182,179,171);
}

static BOOL IsGrayed() {
  BYTE byRvalue = GetRV(_GetSysColor(CLR_3DFACE));
  BYTE byGvalue = GetGV(_GetSysColor(CLR_3DFACE));
  BYTE byBvalue = GetBV(_GetSysColor(CLR_3DFACE));
  
  if ((byRvalue == byGvalue) && (byGvalue == byBvalue) &&
    (byBvalue > 190 && byBvalue < 193) || (byBvalue == 216)) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

static COLOR GetRGBSemiShadow() {
  
  
  COLOR clr3DFace = _GetSysColor(CLR_3DFACE);
  BYTE byRvalue = GetRV(clr3DFace);
  BYTE byGvalue = GetGV(clr3DFace);
  BYTE byBvalue = GetBV(clr3DFace);
  byRvalue = byRvalue >> 2;
  byGvalue = byGvalue >> 2;
  byBvalue = byBvalue >> 2;
  int iMaximo = max(byRvalue, byGvalue);
  iMaximo = max(iMaximo, byBvalue);
  iMaximo = 255 - iMaximo;
  iMaximo = iMaximo - (int)(iMaximo * 0.25);
  return _RGB(byRvalue + iMaximo, byGvalue + iMaximo, byBvalue + iMaximo);
}
int GetDropDownWidth() {
  int iSize;
  HDC hDC = GetDC(NULL);
  ASSERT(hDC != NULL);
  HFONT hFont;
  
  if ((hFont = CreateFont(GetSystemMetrics(SM_CYMENUCHECK), 0, 0, 0,
    FW_NORMAL, 0, 0, 0, SYMBOL_CHARSET, 0, 0, 0, 0, _T("Marlett"))) != NULL) {
    hFont = (HFONT)SelectObject(hDC, hFont);
  }
  
  GetCharWidth(hDC, '6', '6', &iSize);
  
  if (hFont != NULL) {
    SelectObject(hDC, hFont);
    DeleteObject(hFont);
  }
  
  ReleaseDC(NULL, hDC);
  ASSERT(iSize != -1);
  return iSize;
}


static int SWCFindChar(TCHAR Buffer[], TCHAR ch) {
  int i = 0;
  
  for (i = 0; i < lstrlen(Buffer); i++) {
    if (Buffer[i] == ch) {
      break;
    }
  }
  
  if (i == lstrlen(Buffer)) {
    return -1;
  }
  
  return i;
}


static void  SWCCopyRight(TCHAR BufferSrc[], int pos, char BufferTarget[]) {
  lstrcpynA(BufferTarget, BufferSrc + pos, lstrlen(BufferSrc) - (pos - 1));
  
}

static void  SWCCopyLeft(TCHAR BufferSrc[], int pos, char BufferTarget[]) {
  lstrcpynA(BufferTarget, BufferSrc, pos);
}

//function of Bruno Podetti
void DrawCheck(img_t* im, const IRECT* pclip, IRECT m_rcTemp)
{
  int iMediaAltura = (RCH(&m_rcTemp) / 2) - 2;
  int iMedioBox = RCW(&m_rcTemp) / 2;
  COLOR cp = GetRGBCaptionXP();
  imdraw_line2(im, pclip, m_rcTemp.l + 1, m_rcTemp.t + iMediaAltura + 3, m_rcTemp.l + 3, m_rcTemp.b - 2, cp, 1);
  imdraw_line2(im, pclip, m_rcTemp.l + 3, m_rcTemp.b - 2, m_rcTemp.r - 1, m_rcTemp.t + 2, cp, 1);
  
  imdraw_line2(im, pclip, m_rcTemp.l + 1, m_rcTemp.t + iMediaAltura + 2, m_rcTemp.l + 3, m_rcTemp.b - 3, cp, 1);
  imdraw_line2(im, pclip, m_rcTemp.l + 3, m_rcTemp.b - 3, m_rcTemp.r - 1, m_rcTemp.t + 1, cp, 1);
  
  imdraw_line2(im, pclip, m_rcTemp.l + 1, m_rcTemp.t + iMediaAltura + 1, m_rcTemp.l + 3, m_rcTemp.b - 4, cp, 1);
  imdraw_line2(im, pclip, m_rcTemp.l + 3, m_rcTemp.b - 4, m_rcTemp.r - 1, m_rcTemp.t, cp, 1);
  return ;
}


//#define TRANSPARENTROP 0xb8074a

void DrawShade(img_t* im, const IRECT* pclip, IRECT Rect, IPOINT screen)
{
  //  if(!SystemParametersInfo(SPI_GETDROPSHADOW,0,&bEnabled,0))
  //  return;
  
  // Get the size of the menu...
  long winW = RCW(&Rect);
  long winH = RCH(&Rect);
  long xOrg = screen.x;
  long yOrg = screen.y;
  
  // Get the desktop hDC...
  
  int X, Y;
  
  // Simulate a shadow on right edge...
  for (X = 1; X <= 2 ; X++) {
    for (Y = 0; Y < 2 ; Y++) {
      imdraw_SetPixel(im, pclip, winW - X, Y, imdraw_GetPixel(im, pclip, xOrg + winW - X, yOrg + Y));
    }
    
    for (Y = 4; Y < 8 ; Y++) {
      COLOR c = imdraw_GetPixel(im, pclip, xOrg + winW - X, yOrg + Y) ;
      imdraw_SetPixel(im, pclip, winW - X, Y, DarkenColor(3 * X * (Y - 3), c)) ;
    }
    
    for (Y = 8; Y <= (winH - 5) ; Y++) {
      COLOR c = imdraw_GetPixel(im, pclip, xOrg + winW - X, yOrg + Y) ;
      imdraw_SetPixel(im, pclip, winW - X, Y, DarkenColor(15 * X, c));
    }
    
    for (Y = (winH - 4); Y <= (winH - 1) ; Y++) {
      COLOR c = imdraw_GetPixel(im, pclip, xOrg + winW - X, yOrg + Y) ;
      imdraw_SetPixel(im, pclip, winW - X, Y, DarkenColor(3 * X * -(Y - winH), c)) ;
    }
  }
  
  // Simulate a shadow on the rc->b edge...
  for (Y = 1; Y <= 2 ; Y++) {
    for (X = 0; X <= 2 ; X++) {
      imdraw_SetPixel(im, pclip, X, winH - Y, imdraw_GetPixel(im, pclip, xOrg + X, yOrg + winH - Y)) ;
    }
    
    for (X = 4; X <= 7 ; X++) {
      COLOR c = imdraw_GetPixel(im, pclip, xOrg + X, yOrg + winH - Y) ;
      imdraw_SetPixel(im, pclip, X, winH - Y, DarkenColor(3 * (X - 3) * Y, c)) ;
    }
    
    for (X = 8; X <= (winW - 5) ; X++) {
      COLOR  c = imdraw_GetPixel(im, pclip, xOrg + X, yOrg + winH - Y);
      imdraw_SetPixel(im, pclip, X, winH - Y, DarkenColor(15 * Y, c)) ;
    }
  }
  
  // Release the desktop hDC...
  return ;
}

static int iSIZE_cmp(ISIZE sz1, ISIZE sz2) {
  return sz1.h!=sz2.h ? CV_CMP(sz1.h, sz2.h) : CV_CMP(sz1.w, sz2.w);
}

static ISIZE iSIZE_add(ISIZE sz1, ISIZE sz2) {
  sz1.w += sz2.w;
  sz1.h += sz2.h;
  return sz1;
}
static ISIZE iSIZE_sub(ISIZE sz1, ISIZE sz2) {
  sz1.w -= sz2.w;
  sz1.h -= sz2.h;
  return sz1;
}

static ISIZE iSIZE_addpt(ISIZE sz1, IPOINT pt2) {
  sz1.w += pt2.x;
  sz1.h += pt2.y;
  return sz1;
}
static ISIZE iSIZE_subpt(ISIZE sz1, IPOINT pt2) {
  sz1.w -= pt2.x;
  sz1.h -= pt2.y;
  return sz1;
}

static int iPOINT_cmp_(IPOINT pt1, IPOINT pt2) {
  return pt1.y!=pt2.y ? CV_CMP(pt1.y, pt2.y) : CV_CMP(pt1.x, pt2.x);
}

static IPOINT iPOINT_add(IPOINT pt1, IPOINT pt2) {
  pt1.x += pt2.x;
  pt1.y += pt2.y;
  return pt1;
}
static IPOINT iPOINT_sub(IPOINT pt1, IPOINT pt2) {
  pt1.x -= pt2.x;
  pt1.y -= pt2.y;
  return pt1;
}
static IPOINT iPOINT_addsz(IPOINT pt1, ISIZE sz2) {
  pt1.x += sz2.w;
  pt1.y += sz2.h;
  return pt1;
}
static IPOINT iPOINT_subsz(IPOINT pt1, ISIZE sz2) {
  pt1.x -= sz2.w;
  pt1.y -= sz2.h;
  return pt1;
}

static IRECT iRECT_ptsz(IPOINT point, ISIZE size) {
    IRECT rc;
    rc.r = (rc.l = point.x) + size.w;
    rc.b = (rc.t = point.y) + size.h;
    return rc;
}
static IRECT iRECT_pt2(IPOINT topLeft, IPOINT bottomRight) {
    IRECT rc;
    rc.l = topLeft.x;
    rc.t = topLeft.y;
    rc.r = bottomRight.x;
    rc.b = bottomRight.y;
    return rc;
  }
  static int  iRECT_W(const IRECT* rc) {
    return (int)rc->r - rc->l;
  }

  static int iRECT_H(const IRECT* rc) {
    return (int)rc->b - rc->t;
  }

  static ISIZE iRECT_Size(const IRECT* rc)       {
    return iSIZE(rc->r - rc->l, rc->b - rc->t);
  }
#if 0
  static IPOINT  iRECT_TL(const IRECT* rc)      {
    return iPOINT(rc->t, rc->l);
  }
  static IPOINT  iRECT_BR(const IRECT* rc)    {
    return iPOINT(rc->b, rc->r);
  }

  static IPOINT  iRECT_CenterPoint(const IRECT* rc)  {
    return iPOINT((rc->l + rc->r) / 2, (rc->t + rc->b) / 2);
  }
  static BOOL  iRECT_IsEmpty(const IRECT* rc)  {
    return 0;
  }
  static BOOL  iRECT_IsRectNull(const IRECT* rc)   {
    return (rc->l == 0 && rc->r == 0 && rc->t == 0 && rc->b == 0);
  }
  static BOOL  PtInRect(const IRECT* rc, IPOINT point)    {
    return ::PtInRect(this, point);
  }
  static void  SetRect(const IRECT* rc, int x1, int y1, int x2, int y2)  {
    ::SetRect(this, x1, y1, x2, y2);
  }
  static void  SetRect(const IRECT* rc, IPOINT topLeft, IPOINT bottomRight)  {
    ::SetRect(this, topLeft.x, topLeft.y, bottomRight.x, bottomRight.y);
  }
  static void  SetRectEmpty(const IRECT* rc)   {
    ::SetRectEmpty(this);
  }
  static void  CopyRect(const IRECT* rc, const IRECT* lpSrcRect)    {
    ::CopyRect(this, lpSrcRect);
  }
  static BOOL  EqualRect(const IRECT* rc, const IRECT* lpRect)  {
    return ::EqualRect(this, lpRect);
  }
  static void  InflateRect(const IRECT* rc, int x, int y)      {
    ::InflateRect(this, x, y);
  }
  static void  InflateRect(const IRECT* rc, ISIZE size)     {
    ::InflateRect(this, size.w, size.h);
  }
  static void  DeflateRect(const IRECT* rc, int x, int y)      {
    ::InflateRect(this, -x, -y);
  }
  static void  DeflateRect(const IRECT* rc, ISIZE size)     {
    ::InflateRect(this, -size.w, -size.h);
  }
  static void  OffsetRect(const IRECT* rc, int x, int y)     {
    ::OffsetRect(this, x, y);
  }
  static void  OffsetRect(const IRECT* rc, IPOINT point)      {
    ::OffsetRect(this, point.x, point.y);
  }
  static void  OffsetRect(const IRECT* rc, ISIZE size)        {
    ::OffsetRect(this, size.w, size.h);
  }
  static BOOL  IntersectRect(const IRECT* rc, const IRECT* lpRect1, const IRECT* lpRect2)  {
    return ::IntersectRect(this, lpRect1, lpRect2);
  }
  static BOOL  UnionRect(const IRECT* rc, const IRECT* lpRect1, const IRECT* lpRect2)    {
    return ::UnionRect(this, lpRect1, lpRect2);
  }
  static BOOL  operator==(const IRECT* rc, IRECT& rect)         {
    return ::EqualRect(this, &rect);
  }
  static BOOL  operator!=(const IRECT* rc, IRECT& rect)         {
    return !::EqualRect(this, &rect);
  }
  static void  operator+=(const IRECT* rc, IPOINT point)              {
    ::OffsetRect(this, point.x, point.y);
  }
  static void  operator+=(const IRECT* rc, ISIZE size)                {
    ::OffsetRect(this, size.w, size.h);
  }
  static void  operator+=(const IRECT* rc, const IRECT* lpRect)           {
    InflateRect(lpRect);
  }
  static void  operator-=(const IRECT* rc, IPOINT point)              {
    ::OffsetRect(this, -point.x, -point.y);
  }
  static void  operator-=(const IRECT* rc, ISIZE size)                {
    ::OffsetRect(this, -size.w, -size.h);
  }
  static void  operator-=(const IRECT* rc, const IRECT* lpRect)           {
    DeflateRect(lpRect);
  }
  static void  operator&=(const IRECT* rc, IRECT& rect)           {
    ::IntersectRect(this, this, &rect);
  }
  static void  operator|=(const IRECT* rc, IRECT& rect)           {
    ::UnionRect(this, this, &rect);
  }
  static IRECT  operator+(const IRECT* rc, IPOINT pt)   {
    IRECT rect(*this);
    ::OffsetRect(&rect, pt.x, pt.y);
    return rect;
  }
  static IRECT  operator-(const IRECT* rc, IPOINT pt)   {
    IRECT rect(*this);
    ::OffsetRect(&rect, -pt.x, -pt.y);
    return rect;
  }
  static IRECT  operator+(const IRECT* rc, ISIZE size)  {
    IRECT rect(*this);
    ::OffsetRect(&rect, size.w, size.h);
    return rect;
  }
  static IRECT  operator-(const IRECT* rc, ISIZE size)  {
    IRECT rect(*this);
    ::OffsetRect(&rect, -size.w, -size.h);
    return rect;
  }
  static BOOL  SubtractRect(const IRECT* rc, const IRECT* lpRectSrc1, const IRECT* lpRectSrc2) {
    return ::SubtractRect(this, lpRectSrc1, lpRectSrc2);
  }
  static IRECT operator&(const IRECT* rc, const IRECT& rc) {
    IRECT rect;
    ::IntersectRect(&rect, this, &rc);
    return rect;
  }
  static IRECT operator|(const IRECT* rc, const IRECT& rc)  {
    IRECT rect;
    ::UnionRect(&rect, this, &rc);
  }

  static void  InflateRect(const IRECT* rc, const IRECT* lpRect) {
    rc->l -= lpRect->l;
    rc->t -= lpRect->t;
    rc->r += lpRect->r;
    rc->b += lpRect->b;
  }

  static void  InflateRect(const IRECT* rc, int l, int t, int r, int b) {
    rc->l -= l;
    rc->t -= t;
    rc->r += r;
    rc->b += b;
  }

  static void  DeflateRect(const IRECT* rc, const IRECT* lpRect) {
    rc->l += lpRect->l;
    rc->t += lpRect->t;
    rc->r -= lpRect->r;
    rc->b -= lpRect->b;
  }

  static void  DeflateRect(const IRECT* rc, int l, int t, int r, int b) {
    rc->l += l;
    rc->t += t;
    rc->r -= r;
    rc->b -= b;
  }

  static IRECT  MulDiv(const IRECT* rc, int nMultiplier, int nDivisor) {
    return iRECT(
        MulDiv(rc->l, nMultiplier, nDivisor),
        MulDiv(rc->t, nMultiplier, nDivisor),
        MulDiv(rc->r, nMultiplier, nDivisor),
        MulDiv(rc->b, nMultiplier, nDivisor));
  }
#endif



/*
 *  CGradient Class, draw a gradient background and special color
 *
 **/

static void PrepareVertical(img_t* im, const IRECT* pclip, IRECT rc, UINT RTop, UINT GTop, UINT BTop, UINT RBot, UINT GBot, UINT BBot) {
  COLOR clr1 = _RGB(RTop, GTop, BTop);
  COLOR clr2 = _RGB(RBot, GBot, BBot);
  imdraw_rect_gradient(im, pclip, rc, clr1, clr2, 0);
  return ;
}

void PrepareHorizontal(img_t* im, const IRECT* pclip, IRECT rc, UINT RLeft, UINT GLeft, UINT BLeft, UINT RRight, UINT GRight, UINT BRight) {
  COLOR clr1 = _RGB(RLeft, GLeft, BLeft);
  COLOR clr2 = _RGB(RRight, GRight, BRight);
  imdraw_rect_gradient(im, pclip, rc, clr1, clr2, 1);
  return ;
}


void PrepareVertical(img_t* im, const IRECT* pclip, IRECT rc, DWORD m_StyleDisplay = GUISTYLE_XP, COLOR m_clr = GetRGBColorTabs(), int nRate = 40) {
  BYTE byRvalue = GetRV(m_clr); //+(nRate==40 ?10:(15));
  BYTE byGvalue = GetGV(m_clr); //+(nRate==40 ?10:(15));
  BYTE byBvalue = GetBV(m_clr); //+(nRate==40 ?10:(15));
                                /*  if (m_StyleDisplay!= GUISTYLE_XP )
                                {
                                COLOR clrNear=DCGetNearestColor(GetRGBCaptionXP());
                                if (clrNear==13595707)
                                PrepareVertical(pDC,222,234,253,141,178,223);
                                else if(clrNear==8039069)
                                PrepareVertical(pDC,243,251,221,179,194,138);
                                else if(clrNear==13221564)
                                PrepareVertical(pDC,248,248,250,165,160,184);
                                }
  else*/
  PrepareVertical(im, pclip, rc, 255, 255, 255, byRvalue - 10, byGvalue - 10, byBvalue - 10);
}

#define DCGetNearestColor(x)  (x)

void PrepareCaption(img_t* im, const IRECT* pclip, IRECT rc, DWORD m_StyleDisplay = GUISTYLE_XP) {
  
  COLOR clrNear = DCGetNearestColor(GetRGBCaptionXP());
  
  if (clrNear == 13595707) {
    PrepareVertical(im, pclip, rc, 222, 234, 253, 141, 178, 223);
  }
  else if (clrNear == 8039069) {
    PrepareVertical(im, pclip, rc, 243, 251, 221, 179, 194, 138);
  }
  else if (clrNear == 13221564) {
    PrepareVertical(im, pclip, rc, 248, 248, 250, 165, 160, 184);
  }
  else {
    PrepareVertical(im, pclip, rc);
  }
}

void PrepareCaptionVert(img_t* im, const IRECT* pclip, IRECT rc, DWORD m_StyleDisplay = GUISTYLE_XP) {
  BYTE byRvalue ;
  BYTE byGvalue ;
  BYTE byBvalue ;
  
  if (m_StyleDisplay != GUISTYLE_XP) {
    PrepareHorizontal(im, pclip, rc, 249, 200, 102, 248, 248, 205);
    return;
  }
  else if (m_StyleDisplay == GUISTYLE_2003) {
    byRvalue = GetRV(GetRGBPressBXP());
    byGvalue = GetGV(GetRGBPressBXP());
    byBvalue = GetBV(GetRGBPressBXP());
  }
  else {
    byRvalue = GetRV(GetRGBColorShadow());
    byGvalue = GetGV(GetRGBColorShadow());
    byBvalue = GetBV(GetRGBColorShadow());
  }
  PrepareHorizontal(im, pclip, rc, byRvalue, byGvalue, byBvalue, byRvalue - 40, byGvalue - 40, byBvalue - 40);
}

void PrepareTabs(img_t* im, const IRECT* pclip, IRECT rc, COLOR m_clrL, COLOR m_clrH) {
  
  BYTE byRvalueL = GetRV(m_clrL);
  BYTE byGvalueL = GetGV(m_clrL);
  BYTE byBvalueL = GetBV(m_clrL);
  
  BYTE byRvalueH = GetRV(m_clrH);
  BYTE byGvalueH = GetGV(m_clrH);
  BYTE byBvalueH = GetBV(m_clrH);
  PrepareVertical(im, pclip, rc, byRvalueL, byGvalueL, byBvalueL, byRvalueH, byGvalueH, byBvalueH);
}

void PrepareReverseVert(img_t* im, const IRECT* pclip, IRECT rc, DWORD m_StyleDisplay = GUISTYLE_XP, COLOR m_clr = GetRGBColorTabs()) {
  
  BYTE byRvalue = GetRV(GetRGBColorFace());
  BYTE byGvalue = GetGV(GetRGBColorFace());
  BYTE byBvalue = GetBV(GetRGBColorFace());
  /*  if (m_StyleDisplay!= GUISTYLE_XP)
  {
  COLOR clrNear=DCGetNearestColor(GetRGBCaptionXP());
  if (clrNear==13595707)
  PrepareVertical(pDC,141,178,223,222,234,253);
  else if(clrNear==8039069)
  PrepareVertical(pDC,179,194,138,243,251,221);
  else if(clrNear==13221564)
  PrepareVertical(pDC,165,160,184,248,248,250);
  }
  else*/
  PrepareVertical(im, pclip, rc, byRvalue - 40, byGvalue - 40, byBvalue - 40, 255, 255, 255);
  //PrepareVertical(pDC,byRvalue-2,byGvalue-2,byBvalue-2,byRvalue,byGvalue,byBvalue);
}

void PrepareReverseVertTab(img_t* im, const IRECT* pclip, IRECT rc, DWORD m_StyleDisplay = GUISTYLE_XP, COLOR m_clr = GetRGBColorTabs()) {
  
  BYTE byRvalue = GetRV(m_clr);
  BYTE byGvalue = GetGV(m_clr);
  BYTE byBvalue = GetBV(m_clr);
  
  if (m_StyleDisplay != GUISTYLE_XP) {
    COLOR clrNear = DCGetNearestColor(GetRGBCaptionXP());
    
    if (clrNear == 13595707) {
      PrepareVertical(im, pclip, rc, 216, 228, 243, 255, 255, 255);
    }
    else if (clrNear == 8039069) {
      PrepareVertical(im, pclip, rc, 233, 237, 220, 255, 255, 255);
    }
    else if (clrNear == 13221564) {
      PrepareVertical(im, pclip, rc, 234, 233, 239, 255, 255, 255);
    }
  }
  else {
    PrepareVertical(im, pclip, rc, byRvalue - 10, byGvalue - 10, byBvalue - 10, byRvalue, byGvalue, byBvalue);
  }
}

void PrepareReverseColorTab(img_t* im, const IRECT* pclip, IRECT rc, DWORD m_StyleDisplay = GUISTYLE_XP, COLOR m_clrL = GetRGBColorTabs(), COLOR m_clrH = GetRGBColorTabs()) {
  
  BYTE byRvalueL = GetRV(m_clrL);
  BYTE byGvalueL = GetGV(m_clrL);
  BYTE byBvalueL = GetBV(m_clrL);
  
  BYTE byRvalueH = GetRV(m_clrH);
  BYTE byGvalueH = GetGV(m_clrH);
  BYTE byBvalueH = GetBV(m_clrH);
  
  PrepareVertical(im, pclip, rc, byRvalueL, byGvalueL, byBvalueL, byRvalueH, byGvalueH, byBvalueH);
}

void PrepareReverseHor(img_t* im, const IRECT* pclip, IRECT rc, COLOR m_clr = GetRGBColorFace(), COLOR m_clr1 = GetRGBColorShadow()) {
  BYTE byRvalue = GetRV(m_clr);
  BYTE byGvalue = GetGV(m_clr);
  BYTE byBvalue = GetBV(m_clr);
  BYTE byRvalue1 = GetRV(m_clr1);
  BYTE byGvalue1 = GetGV(m_clr1);
  BYTE byBvalue1 = GetBV(m_clr1);
  /*if (m_StyleDisplay!= GUISTYLE_XP)
  {
  COLOR clrNear=DCGetNearestColor(GetRGBCaptionXP());
  if (clrNear==13595707)
  PrepareHorizontal(pDC,222,234,253,141,178,223);
  else if(clrNear==8039069)
  PrepareHorizontal(pDC,243,251,221,179,194,138);
  else if(clrNear==13221564)
  PrepareHorizontal(pDC,248,248,250,165,160,184);
  }
  else*/
  PrepareHorizontal(im, pclip, rc, byRvalue1, byGvalue1, byBvalue1, byRvalue, byGvalue, byBvalue);
  //PrepareHorizontal(pDC,byRvalue,byGvalue,byBvalue,byRvalue-40,byGvalue-40,byBvalue-40);
  
}

void PrepareHorizontal(img_t* im, const IRECT* pclip, IRECT rc, COLOR m_clrBase = GetRGBColorTabs(), COLOR m_clr = GetRGBColorFace()) {
  BYTE byRvalue = GetRV(m_clrBase) + 10;
  BYTE byGvalue = GetGV(m_clrBase) + 10;
  BYTE byBvalue = GetBV(m_clrBase) + 10;
  BYTE byRvalue1 = GetRV(m_clr);
  BYTE byGvalue1 = GetGV(m_clr);
  BYTE byBvalue1 = GetBV(m_clr);
  PrepareHorizontal(im, pclip, rc, byRvalue1 - 10, byGvalue1 - 10, byBvalue1 - 10, 255, 255, 255);
}


// CToolBarCtrlEx
void imdraw_ToolBarButton(img_t* im, const IRECT* pclip, IRECT rcBtn, int fsState, int fsStyle, const char* text, img_t* imagelist, int iIndexImage, IPOINT pt, const font_t* fo) {
  COLOR cb = GetRGBColorFace();
  UINT uItemState = fsState;
  bool bSelected = (uItemState & CDIS_SELECTED || uItemState & CDIS_HOT || uItemState & CDIS_CHECKED) != 0;
  BOOL Over = FALSE;

  if (bSelected)
    if (fsStyle & TBSTYLE_SEP) {
      if (!(fsStyle&TBSTATE_WRAP)) {
        IRECT m_Separ;
        m_Separ = rcBtn;

        int nhalf = (RCW(&m_Separ) / 2) - 1;
        m_Separ.t -= 1;
        m_Separ.l += nhalf;
        m_Separ.r = m_Separ.l + 1;
        m_Separ.b += 1;
        COLOR cbs;
        cbs = _RGB(167, 167, 167);
        imdraw_rect(im, pclip, m_Separ, cbs, 0, 0, 0);
      }
    }
    else if (fsState & TBSTATE_ENABLED) {
      COLOR cblu;

      if (!iPtInRect(&rcBtn, pt.x, pt.y)) {
        if (fsState & TBSTATE_CHECKED) {
          cblu = (GetRGBFondoXP());
          imdraw_Draw3dRect(im, pclip, &rcBtn, GetRGBMenu(), GetRGBMenu(), cblu);
        }

      }
      else {
        Over = TRUE;
        imdraw_Draw3dRect(im, pclip, &rcBtn, GetRGBMenu(), GetRGBMenu(), 0);
        iRectDeflate2(&rcBtn, 1, 1);

        if (fsState & TBSTATE_CHECKED) {
          cblu = (GetRGBFondoXP());
          imdraw_FillRect(im, pclip, &rcBtn, cblu);
        }
        else {
          if (fsState & TBSTATE_PRESSED) {
            cblu = (GetRGBPressBXP());
            imdraw_FillRect(im, pclip, &rcBtn, cblu);
          }
          else {
            cblu = (fsState & TBSTATE_PRESSED ? GetRGBPressBXP() : GetRGBFondoXP());
            imdraw_FillRect(im, pclip, &rcBtn, cblu);
          }

          if (fsState & TBSTYLE_DROPDOWN) {
            IRECT rcCli = rcBtn;
            int dif = rcBtn.r - 8;
            rcCli.l = dif + 1;

            if (!(fsState & TBSTATE_PRESSED)) {
              COLOR Cp;
              Cp = (GetRGBCaptionXP());
              imdraw_line2(im, pclip, dif, rcBtn.t, dif, rcBtn.b, Cp, 1);
            }
          }
        }

        iRectInflate2(&rcBtn, 1, 1);
      }
    }


  if (!(fsStyle & TBSTYLE_SEP)) {
    const img_t* hiImg = imagelist;
    int cxIcon = hiImg->w, cyIcon = hiImg->h;
    //ISIZE sizeButton = iRECT_Size(&rcBtn);
    ISIZE siImg = iSIZE(cxIcon, cyIcon);
    IPOINT pti = iPOINT(rcBtn.l + 4, rcBtn.t + 4);
    IRECT rc1 = iRECT_ptsz(pti, siImg);
    const char* mszText = text;
    //HFONT m_fontOld=dc.SelectFont(fo);
    COLOR cbShadow;
    cbShadow = (GetRGBColorShadow());

    //if (fsState & TBSTATE_ENABLED)
    {
      imdraw_imagelist(im, pclip, rc1, hiImg, iIndexImage, 0, 0, 0);

      if (strlen(mszText) > 0) {
        IRECT m_rctext = rcBtn;
        m_rctext.l += siImg.w + 4;
        m_rctext.b -= 1;
        rc1 = iRECT_ptsz(pti, siImg);
        imdraw_text(im, pclip, m_rctext, text, -1, fo, Color_Black, 0, DT_VCENTER|DT_CENTER);
      }
    }
  }
}

/*void DrawArrow(CGDI* pDC,IRECT m_rc)
{
  int difh =m_rc.Height()-mHeight.y;
  difh/=2;

  m_rc.l=m_rc.r-GetDropDownWidth();
  m_imgArrow.Draw(pDC,0,iPOINT(m_rc.l+2,m_rc.t+difh),ILD_TRANSPARENT);

}*/
  
DWORD imdraw_ToolBarCtrl(img_t* im, const IRECT* pclip, IRECT rc, int n) {
  /*if (rc1.Width() > lpSize.w)
  {
  rc1.l= (rc1.r- (rc1.Width() - lpSize.w))+5;
  rc.r=rc1.l;
  }*/
  PrepareVertical(im, pclip, rc);
  int i;
  for (i=0; i<n; ++i) {
    //imdraw_ToolBarButton(im, pclip, )
  }
  return 0;
}

/*
 *
 *  CStatusCtrlEx
 *
 ****/

enum StatusCtrl_Style {DEVSTUDIO = 0, OFFICE = 1};

  //int m_Style; = DEVSTUDIO;

void DrawGripper(img_t* im, const IRECT* pclip, IRECT rcWin, BOOL bHorz = TRUE) {
  int i;
  if (bHorz) {
    rcWin.t += 7;
    rcWin.l += 5;
    rcWin.r = rcWin.l + 2;
    rcWin.b -= 4;
    IRECT rcBlack;
    COLOR cb;
    
    for (i = 0; i < RCH(&rcWin); i += 4) {
      IRECT rcWindow;
      cb = (GetRGBColorShadow());
      rcWindow = rcWin;
      rcWindow.t = rcWin.t + i;
      rcWindow.b = rcWindow.t + 2;
      imdraw_FillRect(im, pclip, &rcWindow, cb);
      rcBlack = rcWindow;
      rcBlack.l -= 1;
      rcBlack.t = (rcWin.t + i) - 1;
      rcBlack.b = rcBlack.t + 2;
      rcBlack.r = rcBlack.l + 2;
      cb = (_GetSysColor(CLR_BTNHIGHLIGHT));
      imdraw_FillRect(im, pclip, &rcBlack, cb);
    }
    
  }
  else {
    rcWin.t += 3;
    rcWin.l += 4;
    rcWin.r -= 2;
    rcWin.b = rcWin.t + 2;
    IRECT rcBlack;
    COLOR cb;
    
    for (i = 0; i < RCW(&rcWin); i += 4) {
      IRECT rcWindow;
      cb = (GetRGBColorShadow());
      rcWindow = rcWin;
      rcWindow.l = rcWindow.l + i;
      rcWindow.r = rcWindow.l + 2;
      imdraw_FillRect(im, pclip, &rcWindow, cb);
      rcBlack = rcWindow;
      rcBlack.t -= 1;
      rcBlack.l -= 1;
      rcBlack.b = rcBlack.t + 2;
      rcBlack.r = rcBlack.l + 2;

      cb = (_GetSysColor(CLR_BTNHIGHLIGHT));
      imdraw_FillRect(im, pclip, &rcBlack, cb);
    }
  }
}

typedef struct status_panel_t {
  const char* text;
  IRECT rc;
} status_panel_t;


#if 0
void StatusCtrl_Size() {
  SendMessage(WM_SIZE, 0L, 0);
  IRECT rc;
  GetClientRect(rc);
  int width = rc.Width();
  int iSW[10];
  int iCont = GetNumPanels();
  int iAcum = 20;
  
  for (int i = iCont - 1; i > -1; i--) {
    int val = width - iAcum;
    iSW[i] = val;
    iAcum += 20;
  }
  
  SetIndicators(iSW, iCont);
}
#endif

BOOL imdraw_StatusCtrl(img_t* im, const IRECT* pclip, IRECT rc, int iNumPanels, const status_panel_t* sp, int m_Style, const font_t* fo) {
  IRECT rcClient = rc, rcTemp;
  COLOR cb;
  cb = (GetRGBColorFace());
  rcTemp = rcClient;
  imdraw_FillRect(im, pclip, &rcClient, cb);

  /*CGradient M(iSIZE(rcClient.Width(),rcClient.Height()));
  M.PrepareVertical(&dc);
  M.Draw(&MemDC,0,0,0,0,rcClient.Width(),rcClient.Height(),SRCCOPY);
  */
  
  int iCont = iNumPanels;
  IRECT rcCli, rcCliente;
  int i;
  //ISIZE m_sChar = font_text_size(fo, "H", -1, 0);

  for (i = 0; i < iCont; i++) {
    const char* lpsText = sp[i].text;
    rcCli = sp[i].rc;
    ISIZE m_sChar = iSIZE(strlen(lpsText), 0);

    if (RCW(&rcCli) < m_sChar.w) {
      int dif = m_sChar.w - RCW(&rcCli);
      rcCli.r = rcCli.l + m_sChar.w - dif;
    }

    /*CGradient M(iSIZE(rcCli.Width(),rcCli.Height()));
    M.PrepareReverseVert(pDC);
    M.Draw(pDC,rcCli.l,rcCli.t,0,0,rcCli.Width(),rcCli.Height(),SRCCOPY);

    */
    if (m_Style == DEVSTUDIO) {
      imdraw_Draw3dRect(im, pclip, &rcCli, GetRGBColorShadow(), GetRGBColorShadow(), 0);
    }
    else {
      imdraw_Draw3dRect(im, pclip, &rcCli, GetRGBColorTabs(), GetRGBColorTabs(), 0);
    }

    if (i == iCont - 1) {
      rcCli.l = rcCli.r;
      rcCli.r = rcCli.l + 2;
      imdraw_Draw3dRect(im, pclip, &rcCli, GetRGBColorFace(), GetRGBColorFace(), 0);
      rcCli.r += 1;
      imdraw_Draw3dRect(im, pclip, &rcCli, GetRGBColorFace(), GetRGBColorFace(), 0);
      rcCli.r += 1;
      imdraw_Draw3dRect(im, pclip, &rcCli, GetRGBColorFace(), GetRGBColorFace(), 0);

    }

    IRECT rc1 = rcCli;
    rc1.l +=4;
    imdraw_text(im, pclip, rc1, lpsText, -1, fo, Color_Black, 0, DT_VCENTER);
  }

  rcCliente = rc;

  if ((rcCliente.r - rcCli.r) != 0) {
    COLOR cb = (GetRGBColorFace());
    rcCliente.l = rcCli.r;
    rcCliente.b = rcCliente.t + 6;
    //imdraw_FillRect(im, pclip, rcCliente,&cb);
  }

  //se dibuja la esquina que permite hacer sizing de la ventana
  IRECT rc1;
  rc1 = rc;
  rc.l = rc.r - 15;
  rc.t = rc.b - 5;
  DrawGripper(im, pclip, rc, FALSE);
  rc.l = rc.r - 11;
  rc.t = rc.b - 10;
  DrawGripper(im, pclip, rc, FALSE);
  rc.l = rc.r - 7;
  rc.t = rc.b - 15;
  DrawGripper(im, pclip, rc, FALSE);
  return TRUE;
}

// CRebarCtrlEx
void DrawGripper1(img_t* im, const IRECT* pclip, IRECT rcWin, BOOL bHorz = TRUE) {
  if (bHorz) {
    rcWin.t += 6;
    rcWin.l += 5;
    rcWin.r = rcWin.l + 2;
    rcWin.b -= 2;
    IRECT rcBlack;
    COLOR cb;
    
    for (int i = 0; i < RCH(&rcWin); i += 4) {
      IRECT rcWindow;
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
    rcWin.l += 4;
    rcWin.r -= 2;
    rcWin.b = rcWin.t + 2;
    IRECT rcBlack;
    COLOR cb;
    
    for (int i = 0; i < RCW(&rcWin); i += 4) {
      IRECT rcWindow;
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

int imdraw_ReBarCtrl(img_t* im, const IRECT* pclip, IRECT rc, int iBandCount) {
  IRECT rect = rc;
  //aqui debe utilizarse la brocha que define CDrawLayer, si no hacemos la siguiente
  //linea usted vera un horrible color negro, a cambio del dibujo.
  
  /*CGradient M(iSIZE(rect.Width(),rect.Height()));
  M.PrepareVertical(&m_dc);
  M.Draw(&m_dc,0,0,0,0,rect.Width(),rect.Height(),SRCCOPY); */
  COLOR cb;
  cb = (CLR_BTNFACE);
  //rc.t+=1;
  IRECT rc1 = rect;
  imdraw_FillRect(im, pclip, &rc1, cb);
  int nCount = iBandCount;
  
  for (int i = 0; i < nCount; i++) {
    //rebarbandinfo_t rbBand = bandinfo[i];
    IRECT rct = { 0, 0, 0, 0 };
    //ISIZE lpSize;
    //rbBand.w += rct.l + rct.r;
    IRECT rc1 = rct;
    rc1.l += 1;
    //rc1.r=rc1.l+lpSize.w+13;
    IRECT rc2 = rct;
    rc2.l = rc2.r - 6;
    PrepareVertical(im, pclip, rc2, GUISTYLE_XP, GetRGBPressBXP());
    PrepareVertical(im, pclip, rc1); //,GUISTYLE_XP,GetRGBPressBXP());
    DrawGripper1(im, pclip, rct);
    //round border efects
    rc1 = rct;
    rc1.l += 1;
    rc1.r = rc1.l + 1;
    rc1.t = rc1.b - 1;
    imdraw_FillRect(im, pclip, &rc1, cb);
    rc1 = rct;
    rc1.l += 1;
    rc1.r = rc1.l + 2;
    rc1.b = rc1.t + 1;
    imdraw_FillRect(im, pclip, &rc1, cb);
    rc1 = rct;
    rc1.l += 1;
    rc1.r = rc1.l + 1;
    rc1.b = rc1.t + 2;
    imdraw_FillRect(im, pclip, &rc1, cb);
  }
  return 0;
}




#define ID_SEPARATOR -3

typedef struct ToolBarData {
  // struct for toolbar resource; guess you already know it ;)
  WORD wVersion;
  WORD wWidth;
  WORD wHeight;
  WORD wItemCount;
} ToolBarData;

typedef struct SpawnItem {
  int   iImageIdx;
  int   iCmd;
  char  cText[128];
} SpawnItem;

typedef struct ImageItem {
  int   iImageIdx;
  int   iCmd;
}ImageItem;

typedef struct FlotMenu {
  int iCmd;
  int Width;
  int iSubMenu;
}FlotMenu;

bool CMenuSpawn_DrawItem(img_t* im, const IRECT* pclip, IRECT rcItem, int itemID, int itemState, SpawnItem* itemData, const img_t* ilList, const img_t* bmpBack, const font_t* fo)
{
  bool res = false;
  ISIZE szImage = iSIZE(20, 20);
  FlotMenu  fltMenu = {0};

  COLOR crMenuText, crMenuTextSel;
  COLOR cr3dFace, crMenu, crHighlight, cr3dHilight, cr3dShadow, crGrayText;
  COLOR m_clrBtnFace, m_clrBtnHilight, m_clrBtnShadow;
  
  {
    crMenuText = RGB(0, 0, 0);
    crMenuTextSel = crMenuText;
    
    cr3dFace = GetSysColor(CLR_3DFACE);
    crMenu = GetRGBSkinMenu();
    crHighlight = GetRGBCaptionXP();
    cr3dHilight = GetSysColor(CLR_3DHILIGHT);
    cr3dShadow = GetSysColor(CLR_3DSHADOW);
    crGrayText = GetSysColor(CLR_GRAYTEXT);
    
    m_clrBtnFace = GetSysColor(CLR_BTNFACE);
    m_clrBtnHilight = GetRGBCaptionXP();
    m_clrBtnShadow = GetSysColor(CLR_BTNSHADOW);
    
  }

  if (bmpBack) {
    szImage.h = bmpBack->h;
    szImage.w = bmpBack->w;
  }

  //pDC1.Draw3dRect(rcClient,GetRGBColorFace(),GetRGBColorShadow());
  {
    UINT id = itemID;
    UINT state = itemState;
    bool bEnab = !(state & ODS_DISABLED);
    bool bSelect = (state & ODS_SELECTED) ? true : false;
    bool bChecked = (state & ODS_CHECKED) ? true : false;
    bool bHotLight = (state & ODS_HOTLIGHT) ? true : false;
    SpawnItem* pItem = itemData;

    if (pItem) {
      const font_t*  pft = fo;
      const font_t*  of = fo;
      IRECT rc = rcItem;
      IRECT rcImage=(rc), rcText=(rc);
      rcImage.r = rcImage.l + RCH(&rc);
      rcImage.b = rc.b;

      COLOR brush;
      brush = (GetRGBColorFace());

      IRECT rcImg = rcImage;
      rcImg.r += 1;
      imdraw_FillRect(im, pclip, &rcImg, brush);

      if (pItem->iCmd != -4) {
        PrepareHorizontal(im, pclip, rcImg, GetRGBColorWhite(), GetRGBColorFace());
      }

      if (bmpBack) {
        imdraw_FillSolidRect(im, pclip, &rcText, crMenu);
        imdraw_image(im, pclip, &rcText, bmpBack, &rcText);
      }

      if (pItem->iCmd == -3) { // is a separator
        COLOR brush;
        brush = (GetRGBSkinMenu());
        rc.l += RCW(&rcImage) + 15;
        imdraw_FillRect(im, pclip, &rc, brush);
        rc.t += RCH(&rc) >> 1;
        rc.l += 5;
        imdraw_edge(im, pclip, rc, EDGE_ETCHED, BF_TOP);
        rcImg.r = rc.l - 4;
        PrepareHorizontal(im, pclip, rcImg);
        fltMenu.iCmd = -1;

      }
      else if (pItem->iCmd == -4) { // is a title item
        //CString cs(pItem->cText), cs1;
        //IRECT rcBdr = (rcText);

        if (pItem->iCmd == -4) {
          fltMenu.iCmd = -4;
          fltMenu.Width = RCW(&rcText);
        }

        if (bSelect && bEnab) {
          COLOR Dark;
          Dark = (GetSysColor(CLR_BTNSHADOW));
          rcText.t += 1;
          rcText.b += 1;
          PrepareVertical(im, pclip, rcText);
          rcText.b -= 1;
          imdraw_line2(im, pclip, rcText.l, rcText.b - 1, rcText.l, rcText.t, Dark, 1);
          imdraw_line2(im, pclip, rcText.l, rcText.t, rcText.r - 1, rcText.t, Dark, 1);
          imdraw_line2(im, pclip, rcText.r - 1, rcText.t, rcText.r - 1, rcText.b, Dark, 1);

          COLOR cb;
          cb = (GetRGBSkinMenu());
          //rc.t+=1;
          iRectDeflate2(&rcText, 1, 1);
          rcText.b += 1;
          imdraw_FillRect(im, pclip, &rcText, cb);
          //pDC.Draw3dRect(rcText,GetRGBColorShadow(),GetRGBColorShadow());
          rcText.t -= 1;
        }
        else if (bHotLight && bEnab) {
          rcText.t += 1;
          imdraw_FillSolidRect(im, pclip, &rcText, GetRGBFondoXP());
          imdraw_Draw3dRect(im, pclip, &rcText, GetRGBMenu(), GetRGBMenu(), 0);
          rcText.t -= 1;
          fltMenu.iCmd = -1;
          fltMenu.Width = -1;
        }
        else {
          imdraw_FillRect(im, pclip, &rcText, brush);
        }

        imdraw_text(im, pclip, rcText, pItem->cText, -1, fo, Color_Black, 0, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
      }
      else {
        rcText.l += rcImage.r + 1;
        COLOR crTextColor = Color_Black;

        if (bSelect) {
          if (pItem->iImageIdx >= 0 || (state & ODS_CHECKED)) {
            imdraw_FillSolidRect(im, pclip, &rc, GetRGBFondoXP());
          }
          else {
            imdraw_FillSolidRect(im, pclip, &rc, GetRGBFondoXP());
          }

          imdraw_Draw3dRect(im, pclip, &rc, GetRGBMenu(), GetRGBMenu(), 0);
          crTextColor = (crMenuTextSel);
        }
        else {
          if (!bmpBack) {
            if (pItem->iImageIdx >= 0 || (state & ODS_CHECKED)) {
              imdraw_FillSolidRect(im, pclip, &rcText, crMenu);
            }
            else {
              imdraw_FillSolidRect(im, pclip, &rcText/*rcText*/, crMenu);
            }
          }

          crTextColor = (crMenuText);
        }

        if (pItem->iImageIdx >= 0) {
          int ay = (RCH(&rcImage) - szImage.h) / 2;
          int ax = (RCW(&rcImage) - szImage.w) / 2;
          IRECT p = iRECT2(rcImage.l + ax, rcImage.t + ay, szImage.w, szImage.h);

          if (bEnab) {
            if (bSelect) {
              imdraw_imagelist(im, pclip, p, ilList, pItem->iImageIdx, 0, 0, 0);
            }
            else {
              imdraw_imagelist(im, pclip, p, ilList, pItem->iImageIdx, 0, 0, 0);
            }
          }
          else {
            imdraw_imagelist(im, pclip, p, ilList, pItem->iImageIdx, 0, 0, 0);
          }
        }
        else {
          if (bChecked) {
            int ay = (RCH(&rcImage) - szImage.h) / 2;
            int ax = (RCW(&rcImage) - szImage.w) / 2;
            //ilOther.Draw(0, pDC.m_hDC, iPOINT(rcImage.l + ax, rcImage.t + ay - 2), ILD_NORMAL);
          }
        }

        char BufferLeft[128];
        char BufferRight[128];
        //BufferLeft[0]=0;
        //BufferRight[0]=0;
        ISIZE sz;
        sz = font_text_size(fo, pItem->cText, -1, 0);
        int ay1 = (RCH(&rcText) - sz.h) / 2;
        rcText.t += ay1;
        rcText.l += 2;
        rcText.r -= 15;
        rcText.l += 4;
        int tf = SWCFindChar(pItem->cText, '\t');

        if (tf >= 0) {

          SWCCopyRight(pItem->cText, tf + 1, BufferRight);
          SWCCopyLeft(pItem->cText, tf + 1, BufferLeft);

          if (!bEnab) {
            if (!bSelect) {
              IRECT rcText1 = (rcText);
              crTextColor = (cr3dHilight);
              imdraw_DrawText(im, pclip, BufferLeft, rcText1, DT_VCENTER | DT_LEFT, fo, crTextColor);
              imdraw_DrawText(im, pclip, BufferRight, rcText1, DT_VCENTER | DT_RIGHT, fo, crTextColor);
              crTextColor = (crGrayText);
              imdraw_DrawText(im, pclip, BufferLeft, rcText, DT_VCENTER | DT_LEFT, fo, crTextColor);
              imdraw_DrawText(im, pclip, BufferRight, rcText, DT_VCENTER | DT_RIGHT, fo, crTextColor);
            }
            else {
              crTextColor = (crMenu);
              imdraw_DrawText(im, pclip, BufferLeft, rcText, DT_VCENTER | DT_LEFT, fo, crTextColor);
              imdraw_DrawText(im, pclip, BufferRight, rcText, DT_VCENTER | DT_RIGHT, fo, crTextColor);
            }
          }
          else {
            imdraw_DrawText(im, pclip, BufferLeft, rcText, DT_VCENTER | DT_LEFT, fo, crTextColor);
            imdraw_DrawText(im, pclip, BufferRight, rcText, DT_VCENTER | DT_RIGHT, fo, crTextColor);
          }
        }
        else {
          if (!bEnab) {

            if (!bSelect) {
              IRECT rcText1 = (rcText);
              iRectInflate2(&rcText1, -1, -1);
              crTextColor = (cr3dHilight);
              imdraw_DrawText(im, pclip, pItem->cText, rcText1, DT_VCENTER | DT_LEFT | DT_EXPANDTABS, fo, crTextColor);
              crTextColor = (crGrayText);
              imdraw_DrawText(im, pclip, pItem->cText, rcText, DT_VCENTER | DT_LEFT | DT_EXPANDTABS, fo, crTextColor);
            }
            else {
              crTextColor = (crMenu);
              imdraw_DrawText(im, pclip, pItem->cText, rcText, DT_VCENTER | DT_LEFT | DT_EXPANDTABS, fo, crTextColor);
            }
          }
          else {
            imdraw_DrawText(im, pclip, pItem->cText, rcText, DT_VCENTER | DT_LEFT | DT_EXPANDTABS, fo, crTextColor);
          }
        }

        //crTextColor = (ocr);
      }

    }

    res = true;
  }

  return res;
}

static ISIZE MenuSpawn_MeasureItem(int itemID, const SpawnItem* itemData, const img_t* bmpBack, const font_t* fo)
{
  ISIZE sz = {0};
  UINT id = itemID;
  const SpawnItem* pItem = itemData;
  ISIZE szImage = {0};
  if (bmpBack) {
    szImage.h = bmpBack->h;
    szImage.w = bmpBack->w;
  }
  
  if (pItem) {
    if (pItem->iCmd == -3) { // is a separator
      sz.w = 10;
      sz.h = 6;
    }
    else {
      if (_tcslen(pItem->cText) > 0) {
        ISIZE osz = font_text_size(fo, pItem->cText, -1, 0);
        
        //  ISIZE osz(LOWORD(dSize), HIWORD(dSize) );
        if (pItem->iCmd == -4) {
          sz = font_text_size(fo, pItem->cText, -1, DT_TOP | DT_VCENTER | DT_SINGLELINE);
        }
        else {
          sz.h = szImage.h + 5;
          
          if (osz.h > sz.h) {
            sz.h = (int) osz.h;
          }
          
          sz.w  = osz.w + 2 + 15;
          sz.w += sz.h > szImage.w ? sz.h : szImage.w;
        }
        
      }
      else {
        sz.h = szImage.h + 5;
        sz.w  = 100;
      }
    }
  }

  return sz;
}

enum enTypeShow {CM_NORMAL = 0, CM_ONFOCUS, CM_SELECCIONADO};

// CComboBoxExt window

static void DrawArrow(img_t* im, const IRECT* pclip, IRECT m_rc)
{
  IPOINT      mHeight;
  mHeight = iPOINT(9, 10);
  int difh = RCH(&m_rc) - mHeight.y;
  difh /= 2;
  
  //m_rc.l = m_rc.r - m_iWidthDrowDown;
  //m_imgArrow.Draw(0, pDC->m_hDC, iPOINT(m_rc.l + 3, m_rc.t + difh), ILD_TRANSPARENT);
}


void DrawComboBox(img_t* im, const IRECT* pclip, IRECT rc, int enShow)
{
  IRECT m_rcClient = rc;
  BOOL    m_bOverCombo;
  int     m_iWidthDrowDown;
  COLORREF  m_clrBtnFace;
  COLORREF  m_clrBtnLight;
  COLORREF  m_clrBtnDark;
  BOOL    m_bPress;
  BOOL        m_bFondoXp;
  BOOL    m_IsCallMiniTool;
  BOOL    bhistory;
  // Generated message map functions
  BOOL      bColor;
  //img_t*    m_imgArrow;
  
  {
    m_clrBtnFace = GetRGBColorFace();
    m_clrBtnLight =_GetSysColor(CLR_3DHILIGHT);
    m_clrBtnDark =_GetSysColor(CLR_3DSHADOW);
    m_iWidthDrowDown = GetSystemMetrics(SM_CXHTHUMB);
    m_bOverCombo = FALSE;
    m_bPress = FALSE;
    //m_imgArrow.Create(IDB_GUI_DOCKBAR, 9, 10, RGB(255, 0, 255));
    m_bFondoXp = FALSE;
    m_IsCallMiniTool = FALSE;
    bColor = TRUE;
    bhistory = FALSE;
  }
  m_clrBtnFace = GetRGBColorFace();
  //IRECT m_rcDropDown;
  PrepareVertical(im, pclip, m_rcClient);
  m_rcClient.b -= 2;
  iRectDeflate2(&m_rcClient, 1, 1);
  
  if (enShow == CM_NORMAL) {
    imdraw_Draw3dRect(im, pclip, &m_rcClient, GetRGBColorWhite(), GetRGBColorWhite(), 0);
  }
  else { // && !XP
    imdraw_Draw3dRect(im, pclip, &m_rcClient, GetRGBMenu(), GetRGBMenu(), 0);
  }
  
  //debemos pintar el borde del boton drawDwon
  iRectDeflate2(&m_rcClient, 1, 1);
  m_rcClient.l = m_rcClient.r - m_iWidthDrowDown;
  iRectDeflate2(&m_rcClient, 1, 1);
  
  if (enShow != CM_NORMAL) {
    imdraw_Draw3dRect(im, pclip, &m_rcClient, GetRGBFondoXP(), GetRGBFondoXP(), 0);
  }
  
  iRectInflate2(&m_rcClient, 0, 1);
  
  if (enShow == CM_NORMAL) {
    //el boton no esta seleccionado
    m_rcClient.l += 2;
    m_rcClient.r += 2;
    DrawArrow(im, pclip, m_rcClient);
  }
  else
  {
    iRectInflate2(&m_rcClient, 1, 1);
    COLOR cblu;
    cblu = (enShow == CM_ONFOCUS ? GetRGBFondoXP() : GetRGBPressBXP());
    imdraw_FillRect(im, pclip, &m_rcClient, cblu);
    m_rcClient.r += 1;
    imdraw_Draw3dRect(im, pclip, &m_rcClient, GetRGBMenu(), GetRGBMenu(), 0);
    DrawArrow(im, pclip, m_rcClient);
  }
  
}

//AutoHide Routines

static int _cyHorzFont, _cyMenuOnBar, _cyTextMargin;
const int CXTEXTMARGIN = 5;

void DockAutoHide_DrawTextVert(img_t* im, const IRECT* pclip, IRECT m_rect, const char* m_Caption, const font_t* fo)
{
  COLOR clr = _GetSysColor(CLR_MENUTEXT);
  COLOR crTextColor = (clr);
  
  ISIZE m_sizeHorz;
  ISIZE m_sizeCad = font_text_size(fo, m_Caption, -1, 0);
  m_sizeHorz.w = (m_sizeCad.w + CXTEXTMARGIN * 2) + 8;
  m_sizeHorz.h = (_cyHorzFont + _cyTextMargin * 2) + 1;
  IRECT rcString = iRECT(m_rect.r - _cyTextMargin, m_rect.t + CXTEXTMARGIN, m_sizeHorz.w, m_sizeHorz.h);
  
  imdraw_DrawText(im, pclip, m_Caption, rcString, DT_SINGLELINE | DT_NOCLIP | DT_NOPREFIX, fo, crTextColor);// don't forget DT_NOCLIP
}
BOOL DockAutoHide_OnPaint(img_t* im, const IRECT* pclip, IRECT m_rect)
{
  COLOR cbr;
  cbr = (GetRGBColorFace());
  imdraw_FillRect(im, pclip, &m_rect, cbr);
  return TRUE;
}
BOOL DockAutoHide_OnEraseBkgnd(img_t* im, const IRECT* pclip, IRECT m_rect)
{
  COLOR cbr;
  cbr = (CLR_BTNFACE);
  imdraw_FillRect(im, pclip, &m_rect, cbr);
  return TRUE;
}


#include "CToolButton.inl"
#include "CContainer.inl"
#include "CFrame.inl"
#include "CFolder.inl"
#include "CHeaderCtrl.inl"
#include "CMMedia.inl"
#include "CTabbed.inl"

