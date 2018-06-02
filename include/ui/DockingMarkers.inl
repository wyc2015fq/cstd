#include "wintools.inl"
#if 0
bool MarkerWnd::Create(UINT id, BOOL (__stdcall* pSetLayerWndAttr)(HWND, COLORREF, BYTE, DWORD), COLORREF clrMask)
{
  CString className = Tools::RegisterWndClass(_T("MarkerWnd"), CS_DBLCLKS, ::LoadCursor(NULL, IDC_ARROW), NULL, NULL);
  HWND hwnd;
  hwnd = CreateWindowEx(WS_EX_TOOLWINDOW | 0x00080000 /*WS_EX_LAYERED*/, className, _T(""), WS_POPUP, IRECT(0, 0, 0, 0), NULL, 0) == FALSE)
  return false;
  //
  CBitmap bmp;
  BITMAP bmpInfo;
  if (bmp.LoadBitmap(id) == FALSE || bmp.GetBitmap(&bmpInfo) == FALSE ||
        m_Imagelist.Create(bmpInfo.bmWidth / 3, bmpInfo.bmHeight, bmpInfo.bmBitsPixel, 3, 0) == FALSE ||
        m_Imagelist.Add(&bmp, _RGB(0, 0, 0)) == -1)
    return false;
    //
    m_pSetLayeredWindowAttributes = pSetLayerWndAttr;
    m_clrMask = clrMask;
    //
    ::MoveWindow(m_hWnd, 0, 0, bmpInfo.bmWidth / 3, bmpInfo.bmHeight, FALSE); // set size.
    //
    return true;
  }
#endif
enum TYPE { TYPE_FIRST, TYPE_SECOND };
enum LAYOUT { LAYOUT_PANE, LAYOUT_FRAME };
typedef struct SelectWindow {
  struct DockingMarkers* m_pDockingMarkers;
  COLORREF m_clrMask;
  BOOL m_bTab, m_bTop;
  int m_iTabHeight;
} SelectWindow;
typedef struct DockingMarkers {
  ctrl_t c[1];
  LayoutPane* m_Layout;
  //
  BOOL m_bTab, m_bTop;
  int m_iTabHeight;
  int m_iHighlight;
  //
  int m_ShownPane; // TYPE_FIRST, TYPE_SECOND
  int m_LayoutType; // LAYOUT_PANE, LAYOUT_FRAME
  SelectWindow m_SelectWnd;
  int (*DrawMarker)(struct DockingMarkers* pObj, img_t* im, const IRECT* pclip, IRECT const* pRect, int marker, const char* pImageList, BOOL bHighlight, BOOL bDisable);
  int (*DrawSelectWindow)(struct DockingMarkers* pObj, img_t* im, const IRECT* pclip, IRECT const* pRect, BOOL bTab, BOOL bTop, int iTabHeight);
  int (*DrawSelectFrame)(struct DockingMarkers* pObj, img_t* im, const IRECT* pclip, IRECT const* pRectNew, IRECT const* pRectOld, BOOL bTab, BOOL bTop, int iTabHeight); // uses when WasCreated()==false -> DockingMarkers wasn't created.
  int (*DrawSelectFrame_Impl)(struct DockingMarkers* pObj, img_t* im, const IRECT* pclip, IRECT const* pRect);
  int (*GetSelectBarBorderWidth)(struct DockingMarkers* pObj);
  int (*GetSelectFrameWidth)(struct DockingMarkers* pObj);
  COLOR(*GetSelectBarInsideColor)(struct DockingMarkers* pObj);
  COLOR(*GetSelectBarBorderColor)(struct DockingMarkers* pObj);
  int (*DrawSelectBarBorder)(struct DockingMarkers* pObj, img_t* im, const IRECT* pclip, IRECT const* pRect);
  int (*DrawSelectBarInside)(struct DockingMarkers* pObj, img_t* im, const IRECT* pclip, IRECT const* pRect);
  int (*GetOuterOutline)(struct DockingMarkers* pObj, IRECT const* rect, BOOL top, int tabHeight, int width, POINT pts[ 8 ] /*out*/);
  int (*GetInnerOutline)(struct DockingMarkers* pObj, IRECT const* rect, BOOL top, int tabHeight, int width, POINT pts[ 8 ] /*out*/);
} DockingMarkers;
#include "ddd.inl"
static int DockingMarkers_HitTest(DockingMarkers* s, int x, int y)
{
  int i;
  for (i = 0; i < 5; ++i) {
    IRECT rc1 = s->m_Layout->m_Pane[i].area;
    iRectOffsetXY(&rc1, s->m_Layout->m_Pane[i].pos.x, s->m_Layout->m_Pane[i].pos.y);
    if (iPtInRect(&rc1, x, y)) {
      return i;
    }
  }
  return -1;
}
static int DockingMarkers_OnMouseMove(DockingMarkers* s, int x, int y)
{
  int hit;
  ctrl_t* c = s->c;
  //HWND hwnd = c->hwnd;
  hit = DockingMarkers_HitTest(s, x, y);
  if (hit != s->m_iHighlight) {
    s->m_iHighlight = hit;
    force_redraw(c);
  }
  return 0;
}
static LRESULT DockingMarkers_proc(ctrl_t* sender, ctrl_t* c, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  DockingMarkers* s = (DockingMarkers*)c->x;
  IRECT rc = c->rc;
  int i;
  int x = LOWORD(lParam), y = HIWORD(lParam);
  ctrl_t* sub = 0;
  if (WM_CTRLFREE == uMsg) {
    c->ctrlproc = 0;
    return 0;
  }
  if (WM_CTRLINIT == uMsg) {
    //SetWindowPos( hwnd, 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW | SWP_NOACTIVATE );
    DockingMarkersDrawBase_set(s);
    strcpy(c->sys->imagepaths, "E:/code/c/tab/MultiPaneCtrl_src");
    s->m_iHighlight = -1;
    return 0;
  }
  switch (uMsg) {
  case WM_GETMINMAXINFO:
    if (s && s->m_Layout) {
      MINMAXINFO* mminfo = (PMINMAXINFO)lParam;
      mminfo->ptMinTrackSize.x = s->m_Layout->m_iWidthTotal;
      mminfo->ptMinTrackSize.y = s->m_Layout->m_iHeightTotal;
    }
    break;
  case WM_SIZE:
    if (sub) {
      sub->rc = c->rc;
      sub->redraw = 1;
      sub->ctrlproc(c, sub, uMsg, wParam, lParam);
    }
    break;
  case WM_LBUTTONUP:
    if (sub) {
    }
    break;
  case WM_COMMAND:
    if (sub) {
    }
    break;
  case WM_LBUTTONDBLCLK:
  case WM_LBUTTONDOWN:
    if (iPtInRect(&rc, x, y)) {
    }
    break;
  case WM_MOUSEMOVE:
    if (s) {
      DockingMarkers_OnMouseMove(s, x, y);
    }
    break;
  case WM_PAINT:
    if (sub && sub->redraw) {
      c->redraw = 1;
    }
    if (c->redraw) {
      img_t* im = (img_t*)wParam;
      //SetLayered(hwnd, 1);
      //SetWindowTransparence(hwnd, s->m_Layout->m_clrMask & 0x00ffffff, 50);
      imdraw_FillSolidRect(im, &rc, &rc, s->m_Layout->m_clrMask);
      for (i = 5; --i >= 0;) {
        IRECT rc1 = s->m_Layout->m_Pane[i].area;
        iRectOffsetXY(&rc1, s->m_Layout->m_Pane[i].pos.x, s->m_Layout->m_Pane[i].pos.y);
        //printf("w = %d\n", RCW(&rc1));
        s->DrawMarker(s, im, &rc, &rc1, 0, s->m_Layout->m_Pane[i].id, i == s->m_iHighlight, 0);
      }
      c->redraw = 0;
    }
    if (sub) {
      sub->ctrlproc(c, sub, uMsg, wParam, lParam);
    }
    break;
  }
  return 0;
}

