
#define MOVEABLE    "E:\\code\\c\\tab\\MultiPaneCtrl_src\\res\\"
#define IDB_BITMAP3             MOVEABLE"Sys.bmp"
#define IDB_BITMAP1             MOVEABLE"Tabs.bmp"
#define IDB_BITMAP2             MOVEABLE"TabsDisabled.bmp"
#define IDR_MAINFRAME           MOVEABLE"Toolbar.bmp"

#define IDB_BITMAP_LEFT         MOVEABLE"Markers A\\LeftPane.bmp"
#define IDB_BITMAP_TOP          MOVEABLE"Markers A\\TopPane.bmp"
#define IDB_BITMAP_RIGHT        MOVEABLE"Markers A\\RightPane.bmp"
#define IDB_BITMAP_BOTTOM       MOVEABLE"Markers A\\BottomPane.bmp"
#define IDB_BITMAP_TABS         MOVEABLE"Markers A\\TabsPane.bmp"

#define IDB_BITMAP_LEFT_2       MOVEABLE"Markers B\\LeftPane.bmp"
#define IDB_BITMAP_TOP_2        MOVEABLE"Markers B\\TopPane.bmp"
#define IDB_BITMAP_RIGHT_2      MOVEABLE"Markers B\\RightPane.bmp"
#define IDB_BITMAP_BOTTOM_2     MOVEABLE"Markers B\\BottomPane.bmp"
#define IDB_BITMAP_TABS_2       MOVEABLE"Markers B\\TabsPane.bmp"

#define IDB_BITMAP_LEFT_3       MOVEABLE"Markers C\\LeftPane.bmp"
#define IDB_BITMAP_TOP_3        MOVEABLE"Markers C\\TopPane.bmp"
#define IDB_BITMAP_RIGHT_3      MOVEABLE"Markers C\\RightPane.bmp"
#define IDB_BITMAP_BOTTOM_3     MOVEABLE"Markers C\\BottomPane.bmp"
#define IDB_BITMAP_TABS_3       MOVEABLE"Markers C\\TabsPane.bmp"
#if 0
typedef struct MarkerPane1 {
  IPOINT pos; // position of bitmap.
  IRECT area; // sensitive area (relative to this marker).
  img_t* img;  // bitmap in resources.
  COLOR m_clrMask;
} MarkerPane1;
typedef struct LayoutPane2 { // layout for pane's markers.
  int m_iWidthTotal, m_iHeightTotal; // total size of marker's group.
  COLOR m_clrMask;  // color of not visible pixels.
  union {
    struct {
      MarkerPane1 m_tabs, m_left, m_top, m_right, m_bottom;
    };
    MarkerPane panes[5];
  };
  int m_nDisable;
  int m_nHighlight;
  IRECT rect;
} LayoutPane1;
#endif
enum DOCKMARKER {
  // pane markers.
  DOCKMARKER_PANE_LEFT, DOCKMARKER_PANE_TOP, DOCKMARKER_PANE_RIGHT, DOCKMARKER_PANE_BOTTOM, DOCKMARKER_PANE_TABS,
  // frame markers.
  DOCKMARKER_FRAME_LEFT, DOCKMARKER_FRAME_TOP, DOCKMARKER_FRAME_RIGHT, DOCKMARKER_FRAME_BOTTOM
};

typedef struct MarkerPane {
  IPOINT pos; // position of bitmap.
  const char* id;  // ID of bitmap in resources.
  IRECT area; // sensitive area (relative to this marker).
} MarkerPane;
typedef struct LayoutPane { // layout for pane's markers.
  int m_iWidthTotal, m_iHeightTotal; // total size of marker's group.
  MarkerPane m_Pane[5];
  COLOR m_clrMask;  // color of not visible pixels.
} LayoutPane;
//
typedef struct MarkerFrame {
  const char* id;  // ID of bitmap in resources.
  IRECT area; // sensitive area (relative to this marker).
} MarkerFrame;

typedef struct LayoutFrame { // layout for pane's and frame's markers.
  int m_iWidthTotal, m_iHeightTotal; // total size of marker's group (for markers of pane).
  MarkerPane m_lPane, m_tPane, m_rPane, m_bPane, m_tabsPane;
  MarkerFrame m_lFrame, m_tFrame, m_rFrame, m_bFrame;
  COLOR m_clrMask;  // color of not visible pixels.
} LayoutFrame;
//
typedef struct Params {
  int transparence;  // 0...99% - transparence of markers.
  BOOL animation;  // enable/disable animation.
  int animationStep; // 1...100% - step of transparence changing during animation.
} Params ;

static LayoutPane MarkersLayoutA[1] = {
  107, 105,   // total size of marker's group.
  0, 35 , IDB_BITMAP_LEFT,  2, 2, 34, 33,   // left marker.
  36, 0 , IDB_BITMAP_TOP,  2, 2, 33, 34,   // top marker.
  71, 35 , IDB_BITMAP_RIGHT,  2, 2, 34, 33,  // right marker.
  36, 69 , IDB_BITMAP_BOTTOM,  2, 2, 33, 34,  // bottom marker.
  15, 15 , IDB_BITMAP_TABS,  2, 2, 75, 73,   // marker of tabs.
  _RGB(255, 0, 255)    // color of mask (pixels which don't show).
};
//
static LayoutPane MarkersLayoutB[1] = {
  88, 88,   // total size of marker's group.
  0, 29 , IDB_BITMAP_LEFT_2,  0, 0, 28, 29,   // left marker.
  29, 0 , IDB_BITMAP_TOP_2,  0, 0, 29, 28,    // top marker.
  61, 29 , IDB_BITMAP_RIGHT_2,  -1, 0, 27, 29,  // right marker.
  29, 60 , IDB_BITMAP_BOTTOM_2,  0, -1, 29, 28,  // bottom marker.
  0, 0 , IDB_BITMAP_TABS_2,  24, 24, 63, 63,   // marker of tabs.
  _RGB(255, 0, 255)    // color of mask (pixels which don't show).
};
//
static LayoutPane MarkersLayoutC[1] = {
  112, 112,   // total size of marker's group.
  4, 40 , IDB_BITMAP_LEFT_3,  0, 0, 32, 32,   // left marker.
  40, 4 , IDB_BITMAP_TOP_3,  0, 0, 32, 32,    // top marker.
  76, 40 , IDB_BITMAP_RIGHT_3,  0, 0, 32, 32,   // right marker.
  40, 76 , IDB_BITMAP_BOTTOM_3,  0, 0, 32, 32,  // bottom marker.
  0, 0 , IDB_BITMAP_TABS_3,  36, 36, 76, 76,   // marker of tabs.
  _RGB(255, 0, 255)    // color of mask (pixels which don't show).
};
#if 0
int load_MarkersLayout(sys_t* sys, LayoutPane* lp, int nStyle)
{
  // total size of marker's group.
  int xyz[] = {
    107, 105, _RGB(255, 0, 255),
    88, 88, _RGB(255, 0, 255),
    112, 112, _RGB(255, 0, 255),
  };
  int xy1[] = {
    15, 15, 2, 2, 75, 73,   // marker of tabs.
    0, 35, 2, 2, 34, 33,    // l marker.
    36,  0, 2, 2, 33, 34,   // t marker.
    71, 35, 2, 2, 34, 33,   // r marker.
    36, 69, 2, 2, 33, 34,   // b marker.

    0, 0, 24, 24, 63, 63,   // marker of tabs.
    0, 29, 0, 0, 28, 29,    // l marker.
    29, 0, 0, 0, 29, 28,    // t marker.
    61, 29, -1, 0, 27, 29,  // r marker.
    29, 60, 0, -1, 29, 28,  // b marker.

    0, 0, 36, 36, 76, 76,   // marker of tabs.
    4, 40, 0, 0, 32, 32,    // l marker.
    40, 4, 0, 0, 32, 32,    // t marker.
    76, 40, 0, 0, 32, 32,   // r marker.
    40, 76, 0, 0, 32, 32,   // b marker.

  };
  char* ss[] = {
    IDB_BITMAP_TABS      ,
    IDB_BITMAP_LEFT      ,
    IDB_BITMAP_TOP       ,
    IDB_BITMAP_RIGHT     ,
    IDB_BITMAP_BOTTOM    ,

    IDB_BITMAP_TABS_2    ,
    IDB_BITMAP_LEFT_2    ,
    IDB_BITMAP_TOP_2     ,
    IDB_BITMAP_RIGHT_2   ,
    IDB_BITMAP_BOTTOM_2  ,

    IDB_BITMAP_TABS_3    ,
    IDB_BITMAP_LEFT_3    ,
    IDB_BITMAP_TOP_3     ,
    IDB_BITMAP_RIGHT_3   ,
    IDB_BITMAP_BOTTOM_3  ,
  };
  int i;
  int* xx = xy1 + countof(xy1) * nStyle / 3;
  char** ps = ss + countof(ss) * nStyle / 3;
  lp->m_iWidthTotal = xyz[nStyle * 3];
  lp->m_iHeightTotal = xyz[nStyle * 3 + 1];
  lp->m_clrMask = xyz[nStyle * 3 + 2];

  for (i = 0; i < 5; ++i) {
    MarkerPane* mp = lp->panes + i;
    mp->pos = iPOINT(xx[i * 6 + 0], xx[i * 6 + 1]), mp->area = iRECT(xx[i * 6 + 2], xx[i * 6 + 3], xx[i * 6 + 4], xx[i * 6 + 5]);
    mp->img = sys_find_img(sys, ps[i]);
    mp->m_clrMask = lp->m_clrMask;
    //GetImageList1(, , lp->m_clrMask, 0, 3);
  }

  return 0;
}

int LayoutPane_hittest(LayoutPane* lp, int x, int y)
{
  IPOINT basePoint;
  int i;
  IRECT* rcScr = &lp->rect;
  basePoint.x = (rcScr->l + rcScr->r - lp->m_iWidthTotal) / 2;
  basePoint.y = (rcScr->t + rcScr->b - lp->m_iHeightTotal) / 2;

  // positioning and show new markers.
  for (i = 4; i >= 0; --i) {
    MarkerPane* mp = lp->panes + i;
    int iframe = ((lp->m_nDisable == i) ? ((lp->m_nHighlight == i) ? 0 : 1) : 2);
    //draw_MarkerPane( hDC, lp->panes+i, basePoint, iframe );
    IPOINT pt;
    ISIZE sz = imsize(mp->img);
    IRECT rc;
    rc = iRECT3(pt, sz);
    PTADD(pt, basePoint, mp->pos);

    if (iPtInRect(&rc, x, y)) {
      return i;
    }
  }

  return -1;
}
#endif
#if 0
int layoutpane_event(LayoutPane* s, const event* e)
{
  UIEVT(e);
  int hit;

  switch (e->msg) {
  case EVENT_INIT:
    s->m_nHighlight = -1;
    break;

  case WM_LBUTTONUP:
    hit = LayoutPane_hittest(s, pt);

    if (hit >= 0 && hit == s->m_nHighlight) {
      UISendEvent(e, s, e->toer, EVENT_LCLICK, hit, 0);
      force_redraw(e);
      return TRUE;
    }

    break;

  case WM_MOUSEMOVE:
    hit = LayoutPane_hittest(s, pt);

    if (hit != s->m_nHighlight) {
      s->m_nHighlight = hit;
      force_redraw(e);
    }

    break;
  }

  return 0;
}
#endif

int DrawSelectWindow(img_t* im, const IRECT* pclip, const IRECT* pRect, int hit)
{
  IRECT rc = *pRect;
  IPOINT pt = iPOINT((rc.l + rc.r) / 2, (rc.t + rc.b) / 2);
  DWORD _clr;
  _clr = _GetSysColor(CLR_HIGHLIGHT);
  _clr = _RGB_A(_clr, 80);

  if (1 == hit) {
    rc.r = pt.x;
  }
  else if (2 == hit) {
    rc.b = pt.y;
  }
  else if (3 == hit) {
    rc.l = pt.x;
  }
  else if (4 == hit) {
    rc.t = pt.y;
  }

  imdraw_fillrect(im, pclip, rc, _clr);
  return 0;
}
int imdraw_LayoutPane(img_t* im, const IRECT* pclip, IRECT rect, const LayoutPane* lp, int nHighlight, int nDisable, sys_t* sys)
{
  IPOINT basePoint;
  int i;
  const IRECT* rcScr = &rect;
  basePoint.x = (rcScr->l + rcScr->r - lp->m_iWidthTotal) / 2;
  basePoint.y = (rcScr->t + rcScr->b - lp->m_iHeightTotal) / 2;

  if (nHighlight >= 0) {
    DrawSelectWindow(im, pclip, rcScr, nHighlight);
  }

  // positioning and show new markers.
  for (i = 4; i >= 0; --i) {
    const MarkerPane* mp = lp->m_Pane + i;
    int iframe = ((nDisable & (1 << i)) ? 2 : ((nHighlight == i) ? 1 : 0));
    //draw_MarkerPane( hDC, lp->panes+i, basePoint, iframe );
    IPOINT pt;
    img_t* im1 = sys_find_img(sys, mp->id);
    img_t imglist[1] = {0};

    if (im1) {
      ISIZE sz = imsize(im1);
      IRECT rc, rc1;
      sz.w /= 3;
      PTADD(pt, basePoint, mp->pos);
      rc = iRECT3(pt, sz);
      rc1 = iRECT2(iframe * sz.w, 0, sz.w, sz.h);
      imdraw_imagelist(im, pclip, rc, im1, 0, &rc1, 0, lp->m_clrMask);
    }
  }

  return 0;
}

#undef IDB_BITMAP3
#undef IDB_BITMAP1
#undef IDB_BITMAP2
#undef IDR_MAINFRAME

#undef IDB_BITMAP_LEFT
#undef IDB_BITMAP_TOP
#undef IDB_BITMAP_RIGHT
#undef IDB_BITMAP_BOTTOM
#undef IDB_BITMAP_TABS

#undef IDB_BITMAP_LEFT_2
#undef IDB_BITMAP_TOP_2
#undef IDB_BITMAP_RIGHT_2
#undef IDB_BITMAP_BOTTOM_2
#undef IDB_BITMAP_TABS_2

#undef IDB_BITMAP_LEFT_3
#undef IDB_BITMAP_TOP_3
#undef IDB_BITMAP_RIGHT_3
#undef IDB_BITMAP_BOTTOM_3
#undef IDB_BITMAP_TABS_3


