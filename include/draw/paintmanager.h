
typedef TCHAR __EXT_MFC_SAFE_TCHAR;
typedef const TCHAR __EXT_MFC_SAFE_CONST_TCHAR;
typedef LPTSTR __EXT_MFC_SAFE_LPTSTR;
typedef LPCTSTR __EXT_MFC_SAFE_LPCTSTR;
typedef TCHAR __prof_uis_used_wchar_t;
typedef TCHAR __prof_uis_converted_wchar_t;

#define __ExtMfc_WM_THEMECHANGED 0x031A
// Prof-UIS impl defines
#define __ExtMfc_MIN_SYS_COLOR_VAL  0L
#define __ExtMfc_MAX_SYS_COLOR_VAL  49L
#define __ExtMfc_COLOR_MAP_BASE   1000L
#define __ExtMfc_DISABLED_COLOR_SHIFT 1000L
#define __ExtMfc_XP_COLOR_SHIFT (__ExtMfc_DISABLED_COLOR_SHIFT+1000L)
#define __ExtMfc_2003_COLOR_SHIFT (__ExtMfc_XP_COLOR_SHIFT+1000L)
//#define __ExtMfc_MENU_GAP__ 4
#define __EXT_MENU_ICON_CX 16
#define __EXT_MENU_ICON_CY 16
#define __EXT_MENU_GAP 3
#define __EXT_MENU_MIN_HEIGHT (__EXT_MENU_ICON_CY+__EXT_MENU_GAP*2)
#define __EXT_MENU_SEPARATOR_HEIGHT 4
#define __EXT_MENU_POPUP_ARROW_AREA_DX 16
#define __EXT_MENU_IMG_RECT_OUT_GAP 2
#define __EXT_MENU_VERT_DISTANCE_NORM 1
#define __EXT_MENU_VERT_DISTANCE_TOOL 1
#define __EXT_MENU_TOOLBTN_GAP 2
#define __EXT_VERT_FONT_ESCAPEMENT_X__ (900)
#define __EXT_VERT_FONT_ESCAPEMENT__ (2700)
#define __DROPDOWN_ARROW_GAP 3
// uxtheme.dll defines
#define __EXT_UX_THEMEAPI(_name_)           HRESULT ( __stdcall * _name_ )
#define __EXT_UX_THEMEAPI_(_type_,_name_)   _type_ ( __stdcall * _name_ )
#define __EXT_UX_CALL                       __stdcall
#define __EXT_UX_DTT_GRAYED                 0x1
#define __EXT_UX_HTTB_BACKGROUNDSEG         0x0000
#define __EXT_UX_HTTB_FIXEDBORDER           0x0002
#define __EXT_UX_HTTB_CAPTION               0x0004
#define __EXT_UX_HTTB_RESIZINGBORDER_LEFT   0x0010
#define __EXT_UX_HTTB_RESIZINGBORDER_TOP    0x0020
#define __EXT_UX_HTTB_RESIZINGBORDER_RIGHT  0x0040
#define __EXT_UX_HTTB_RESIZINGBORDER_BOTTOM 0x0080
#define __EXT_UX_HTTB_RESIZINGBORDER \
  (__EXT_UX_HTTB_RESIZINGBORDER_LEFT \
      |__EXT_UX_HTTB_RESIZINGBORDER_TOP \
      |__EXT_UX_HTTB_RESIZINGBORDER_RIGHT \
      |__EXT_UX_HTTB_RESIZINGBORDER_BOTTOM)
#define __EXT_UX_HTTB_SIZINGTEMPLATE        0x0100
#define __EXT_UX_HTTB_SYSTEMSIZINGMARGINS   0x0200
#define __EXT_UX_MAX_INTLIST_COUNT          10
#define __EXT_UX_ETDT_DISABLE               0x00000001
#define __EXT_UX_ETDT_ENABLE                0x00000002
#define __EXT_UX_ETDT_USETABTEXTURE         0x00000004
#define __EXT_UX_ETDT_ENABLETAB \
  (__EXT_UX_ETDT_ENABLE| \
      __EXT_UX_ETDT_USETABTEXTURE)
#define __EXT_UX_STAP_ALLOW_NONCLIENT       (1 << 0)
#define __EXT_UX_STAP_ALLOW_CONTROLS        (1 << 1)
#define __EXT_UX_STAP_ALLOW_WEBCONTENT      (1 << 2)
#define __EXT_UX_SZ_THDOCPROP_DISPLAYNAME   L"DisplayName"
#define __EXT_UX_SZ_THDOCPROP_CANONICALNAME L"ThemeName"
#define __EXT_UX_SZ_THDOCPROP_TOOLTIP       L"ToolTip"
#define __EXT_UX_SZ_THDOCPROP_AUTHOR        L"author"
#define __EXT_UX_DTBG_CLIPRECT              0x00000001
#define __EXT_UX_DTBG_DRAWSOLID             0x00000002
#define __EXT_UX_DTBG_OMITBORDER            0x00000004
#define __EXT_UX_DTBG_OMITCONTENT           0x00000008
#define __EXT_UX_DTBG_COMPUTINGREGION       0x00000010
#define __EXT_UX_DTBG_MIRRORDC              0x00000020
// paint managers
//struct __PROF_UIS_API CExtPaintManager: public CObject {
// color values container
typedef COLORREF* sys_brashes_container;
#define MAXCOLORS 10000
COLORREF m_colors[MAXCOLORS];
// system brushes container
// translated colors map
//typedef CMap < int, int, int, int > translate_color_map;
int m_mapColorTranslate[MAXCOLORS];

RGBQUAD m_PaletteWide[256];
int m_PaletteWideNum = 256;
COLORREF m_brushLightDefault, m_brushLightSystem, m_brushDarkDefault, m_brushDarkSystem, m_brushLighterDefault, m_brushLighterSystem, m_brushDarkerDefault, m_brushDarkerSystem, m_brushLightestDefault, m_brushLightestSystem, m_brushDarkestDefault, m_brushDarkestSystem;
// normal
HFONT m_FontNormal, m_FontNormalVert, m_FontNormalVertX;
HFONT m_FontBold, m_FontBoldVert, m_FontBoldVertX;
// bar caption
HFONT m_FontNormalBC, m_FontNormalVertBC, m_FontNormalVertXBC;
HFONT m_FontBoldBC, m_FontBoldVertBC, m_FontBoldVertXBC;
// marlett symbols
HFONT m_FontMarlett;
int m_DSS_DISABLED_style;
// global auto-pointer struct for CExtPaintManager

// init part of color table and system brushes table
BOOL m_bUseTahomaFont; // uses in SyncSysColors() to re-init font
#define GetMenuFrameFillColor()  GetColor(CLR_3DFACE_OUT)
// used always by default used translated colors
enum e_translated_colors_t {
  CLR_3DFACE_OUT = __ExtMfc_COLOR_MAP_BASE + 0, CLR_3DFACE_IN = __ExtMfc_COLOR_MAP_BASE + 1, CLR_3DFACE_DISABLED = CLR_3DFACE_OUT + __ExtMfc_DISABLED_COLOR_SHIFT, CLR_3DLIGHT_OUT = __ExtMfc_COLOR_MAP_BASE + 2, CLR_3DLIGHT_IN = __ExtMfc_COLOR_MAP_BASE + 3, CLR_3DLIGHT_DISABLED = CLR_3DLIGHT_OUT + __ExtMfc_DISABLED_COLOR_SHIFT, CLR_3DHILIGHT_OUT = __ExtMfc_COLOR_MAP_BASE + 4, CLR_3DHILIGHT_IN = __ExtMfc_COLOR_MAP_BASE + 5, CLR_3DHILIGHT_DISABLED = CLR_3DHILIGHT_OUT + __ExtMfc_DISABLED_COLOR_SHIFT, CLR_3DSHADOW_OUT = __ExtMfc_COLOR_MAP_BASE + 6, CLR_3DSHADOW_IN = __ExtMfc_COLOR_MAP_BASE + 7, CLR_3DSHADOW_DISABLED = CLR_3DSHADOW_OUT + __ExtMfc_DISABLED_COLOR_SHIFT, CLR_3DDKSHADOW_OUT = __ExtMfc_COLOR_MAP_BASE + 8, CLR_3DDKSHADOW_IN = __ExtMfc_COLOR_MAP_BASE + 9, CLR_3DDKSHADOW_DISABLED = CLR_3DDKSHADOW_OUT + __ExtMfc_DISABLED_COLOR_SHIFT, CLR_TEXT_OUT = __ExtMfc_COLOR_MAP_BASE + 10, CLR_TEXT_IN = __ExtMfc_COLOR_MAP_BASE + 11, CLR_TEXT_DISABLED = CLR_TEXT_OUT + __ExtMfc_DISABLED_COLOR_SHIFT,
};
// align types
enum e_align_t {
  __ALIGN_HORIZ_CENTER = 1, __ALIGN_HORIZ_RIGHT = 2, __ALIGN_VERT = 4
};
typedef struct _glyph_t {
  SIZE m_size; // size in pixels
  UINT m_nColors; // count of used indexes (including 0)
  // index 0 - transparent
  UINT* m_pColorIndexes;
} glyph_t; // struct glyph_t
Init_glyph1(glyph_t* This, SIZE _size, UINT nColors, const UINT* pColorIndexes)
{
  ASSERT(_size.cx > 0);
  ASSERT(_size.cy > 0);
  This->m_size.cx = _size.cx;
  This->m_size.cy = _size.cy;
  ASSERT(nColors > 0);
  This->m_nColors = nColors;
  ASSERT(pColorIndexes != NULL);
};
Init_glyph2(glyph_t* This, int cx, int cy, UINT nColors, const UINT* pColorIndexes)
{
  ASSERT(cx > 0);
  ASSERT(cy > 0);
  This->m_size.cx = cx;
  This->m_size.cy = cy;
  ASSERT(nColors > 0);
  This->m_nColors = nColors;
  ASSERT(pColorIndexes != NULL);
};

UINT GetColorPixel(glyph_t* This, int x, int y)
{
  int nOffset;
  UINT nValue;
  ASSERT(x >= 0 && x < This->m_size.cx);
  ASSERT(y >= 0 && y < This->m_size.cy);
  ASSERT(This->m_size.cx > 0);
  ASSERT(This->m_size.cy > 0);
  ASSERT(This->m_nColors > 0);
  ASSERT(This->m_pColorIndexes != NULL);
  nOffset = y * This->m_size.cx + x;
  nValue = *(This->m_pColorIndexes + nOffset);
  return nValue;
};
glyph_t* _CopyFromOther(const glyph_t* other, char* buf, int buflen)
{
  glyph_t* This = 0;

  if (other->m_pColorIndexes != NULL) {
    int nBufferSize = other->m_size.cx * other->m_size.cy;
    int n = sizeof(glyph_t) + nBufferSize * sizeof(UINT);
    ASSERT(buflen >= n);
    This = (glyph_t*)buf;
    *This = *other;
    This->m_pColorIndexes = (UINT*)(This + 1);
    ASSERT(nBufferSize > 0);
    memcpy((void*)This->m_pColorIndexes, other->m_pColorIndexes, nBufferSize * sizeof(UINT));
    This->m_size.cx = other->m_size.cx;
    This->m_size.cy = other->m_size.cy;
    This->m_nColors = other->m_nColors;
  }

  return This;
};
glyph_t* glyph_Rotate(const glyph_t* other, UINT nRotateAngle, char* buf, int buflen)
{
  glyph_t* This;
  int nBufferSize, x, y, t;
  UINT* pNew, *p;
  const UINT* pOld;
  This = _CopyFromOther(other, buf, buflen);
  ASSERT(nRotateAngle == 0 || nRotateAngle == 90 || nRotateAngle == 180 || nRotateAngle == 270);
  ASSERT(This->m_size.cx > 0);
  ASSERT(This->m_size.cy > 0);
  ASSERT(This->m_nColors > 0);
  ASSERT(This->m_pColorIndexes != NULL);

  if (nRotateAngle == 0) {
    return This;
  }

  nBufferSize = This->m_size.cx * This->m_size.cy;
  ASSERT(nBufferSize > 0);
  pNew = This->m_pColorIndexes;
  pOld = (const UINT*)other->m_pColorIndexes;

  switch (nRotateAngle) {
  case 90: {
    for (y = 0; y < This->m_size.cy; y++) {
      for (x = 0; x < This->m_size.cx; x++, pOld++) {
        int nNewOffset = x * This->m_size.cy + (This->m_size.cy - y - 1);
        ASSERT(nNewOffset >= 0 && nNewOffset < nBufferSize);
        p = pNew + nNewOffset;
        *p =  *pOld;
      }

      // for( x = 0; x<m_size.cx; x++, pOld++ )
    }

    // for( y = 0; y<m_size.cy; y++ )
    SWAP(This->m_size.cy, This->m_size.cx, t);
  }
  break; // case 90

  case 180: {
    for (y = 0; y < This->m_size.cy; y++) {
      for (x = 0; x < This->m_size.cx; x++, pOld++) {
        int nNewOffset = (This->m_size.cy - y - 1) * This->m_size.cx + (This->m_size.cx - x - 1);
        ASSERT(nNewOffset >= 0 && nNewOffset < nBufferSize);
        p = pNew + nNewOffset;
        *p =  *pOld;
      }

      // for( x = 0; x<m_size.cx; x++, pOld++ )
    }

    // for( y = 0; y<m_size.cy; y++ )
  }
  break; // case 180

  case 270: {
    for (y = 0; y < This->m_size.cy; y++) {
      for (x = 0; x < This->m_size.cx; x++, pOld++) {
        int nNewOffset = (This->m_size.cx - x - 1) * This->m_size.cy + y;
        ASSERT(nNewOffset >= 0 && nNewOffset < nBufferSize);
        p = pNew + nNewOffset;
        *p =  *pOld;
      }

      // for( x = 0; x<m_size.cx; x++, pOld++ )
    }

    // for( y = 0; y<m_size.cy; y++ )
    SWAP(This->m_size.cy, This->m_size.cx, t);
  }
  break; // case 270
  } // switch( nRotateAngle )

  return This;
}

static UINT stat_pixels_glyph_btn_expand_bottom[] = {
  1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0,
};
static UINT stat_pixels_glyph_btn_expand_bottom_2003[] = {
  1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0,
};
const glyph_t g_glyph_btn_expand_bottom = {5, 3, 2, stat_pixels_glyph_btn_expand_bottom};
const glyph_t g_glyph_btn_expand_bottom_2003 = {5, 6, 2, stat_pixels_glyph_btn_expand_bottom_2003};
static UINT stat_pixels_glyph_btn_expand_right2[] = {
  1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0,
};
static UINT stat_pixels_glyph_btn_expand_right2_2003[] = {
  0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
};
const glyph_t g_glyph_btn_expand_right2 = {8, 5, 2, stat_pixels_glyph_btn_expand_right2};
const glyph_t g_glyph_btn_expand_right2_2003 = {6, 5, 2, stat_pixels_glyph_btn_expand_right2_2003};

static char g_glyph_btn_expand_left_buf[sizeof(g_glyph_btn_expand_bottom) + 64 * sizeof(UINT)];
static char g_glyph_btn_expand_left_2003_buf[sizeof(g_glyph_btn_expand_bottom) + 64 * sizeof(UINT)];
static char g_glyph_btn_expand_right_buf[sizeof(g_glyph_btn_expand_bottom) + 64 * sizeof(UINT)];
static char g_glyph_btn_expand_right_2003_buf[sizeof(g_glyph_btn_expand_bottom) + 64 * sizeof(UINT)];
static char g_glyph_btn_expand_bottom2_buf[sizeof(g_glyph_btn_expand_bottom) + 64 * sizeof(UINT)];
static char g_glyph_btn_expand_bottom2_2003_buf[sizeof(g_glyph_btn_expand_bottom) + 64 * sizeof(UINT)];
#define GLYPH_ROTATE(_a, _b, _c) (*glyph_Rotate(&(_a), _b, _c, sizeof(_c)))
#define g_glyph_btn_expand_left GLYPH_ROTATE(g_glyph_btn_expand_bottom, 90, g_glyph_btn_expand_left_buf)
#define g_glyph_btn_expand_left_2003 GLYPH_ROTATE(g_glyph_btn_expand_bottom_2003, 90, g_glyph_btn_expand_left_2003_buf)
#define g_glyph_btn_expand_right GLYPH_ROTATE(g_glyph_btn_expand_bottom, 270, g_glyph_btn_expand_left_2003_buf)
#define g_glyph_btn_expand_right_2003 GLYPH_ROTATE(g_glyph_btn_expand_bottom_2003, 270, g_glyph_btn_expand_left_2003_buf)
#define g_glyph_btn_expand_bottom2 GLYPH_ROTATE(g_glyph_btn_expand_right2, 90, g_glyph_btn_expand_bottom2_buf)
#define g_glyph_btn_expand_bottom2_2003 GLYPH_ROTATE(g_glyph_btn_expand_right2_2003, 90, g_glyph_btn_expand_bottom2_2003_buf)
static UINT stat_pixels_glyph_customize_popup_src[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
const glyph_t g_glyph_customize_popup_src = {12, 16, 2, stat_pixels_glyph_customize_popup_src};
static UINT stat_pixels_glyph_customize_dropdown_src[] = {
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
const glyph_t g_glyph_customize_dropdown_src = {12, 16, 2, stat_pixels_glyph_customize_dropdown_src};


void _SetColorPixel(glyph_t* This, int x, int y, UINT nValue)
{
  int nOffset;
  UINT* ptr;
  ASSERT(x >= 0 && x < This->m_size.cx);
  ASSERT(y >= 0 && y < This->m_size.cy);
  ASSERT(This->m_size.cx > 0);
  ASSERT(This->m_size.cy > 0);
  ASSERT(This->m_nColors > 0);
  ASSERT(This->m_pColorIndexes != NULL);
  nOffset = y * This->m_size.cx + x;
  ptr = (UINT*)This->m_pColorIndexes + nOffset;
  *ptr = nValue;
};

typedef struct _monitor_parms_t {
  RECT m_rcMonitor, m_rcWorkArea;
  BOOL m_bPrimaryMonitor;
  //monitor_parms_t(): m_rcMonitor(0, 0, 0, 0), m_rcWorkArea(0, 0, 0, 0), m_bPrimaryMonitor(TRUE){}
} monitor_parms_t; // struct monitor_parms_t

typedef enum _e_docking_caption_button_t {
  __DCBT_EMPTY = 0, __DCBT_CLOSE = 1, __DCBT_ARROW_UP = 2, __DCBT_ARROW_DOWN = 3, __DCBT_ARROW_LEFT = 4, __DCBT_ARROW_RIGHT = 5, __DCBT_WND_MINIMIZE = 6, __DCBT_WND_RESTORE = 7, __DCBT_WND_MAXIMIZE = 8, __DCBT_WND_CONTEXTHELP = 9, __DCBT_WND_KEEP = 10, __DCBT_WND_UNKEEP = 11, __DCBT_ARROW_UP_L = 12, __DCBT_ARROW_DOWN_L = 13, __DCBT_ARROW_LEFT_L = 14, __DCBT_ARROW_RIGHT_L = 15, __DCBT_XP_THIN_CLOSE = 16, __DCBT_HOLLOW_UP = 17, __DCBT_HOLLOW_DOWN = 18, __DCBT_HOLLOW_LEFT = 19, __DCBT_HOLLOW_RIGHT = 20, __DCBT_CLOSE_DC2K = 21, __DCBT_HOLLOW_PLUS = 22, __DCBT_HOLLOW_MINUS = 23, __DCBT_TREE_PLUS = 24, __DCBT_TREE_MINUS = 25, __DCBT_CIRCLE_PLUS = 26, __DCBT_CIRCLE_MINUS = 27, __DCBT_ARROW_PT_UP = 28, __DCBT_ARROW_PT_DOWN = 29, __DCBT_ARROW_PT_LEFT = 30, __DCBT_ARROW_PT_RIGHT = 31, __DCBT_ARROW_PT_LT = 32, __DCBT_ARROW_PT_RT = 33, __DCBT_ARROW_PT_RB = 34, __DCBT_ARROW_PT_LB = 35, __DCBT_ARROW_SMALL_UP = 36, __DCBT_ARROW_SMALL_DOWN = 37, __DCBT_ARROW_SMALL_LEFT = 38, __DCBT_ARROW_SMALL_RIGHT = 39, __DCBT_ELLIPSIS = 40, __DCBT_ARROW_SORT_UP = 41, __DCBT_ARROW_SORT_DOWN = 42, __DCBT_ARROW_SORT_LEFT = 43, __DCBT_ARROW_SORT_RIGHT = 44, __DCBT_VALUE_MIN = 0, __DCBT_VALUE_MAX = 44,
} e_docking_caption_button_t;
#define __PROF_UIS_API
static glyph_t* g_DockingCaptionGlyphs[__DCBT_VALUE_MAX + 1];
typedef struct __PROF_UIS_API _PAINTDOCKINGCAPTIONBUTTONDATA {
  void* m_pHelperSrc;
  LPARAM m_lParam;
  RECT m_rcClient;
  int m_eType; // CExtPaintManager::e_docking_caption_button_t
  BOOL m_bHover: 1;
  BOOL m_bPushed: 1;
  BOOL m_bEnabled: 1;
  BOOL m_bSideBar: 1;
  BOOL m_bBarWndActive: 1;
  BOOL m_bFloating: 1;
} PAINTDOCKINGCAPTIONBUTTONDATA; // struct PAINTDOCKINGCAPTIONBUTTONDATA
typedef struct __PROF_UIS_API _PAINTPUSHBUTTONDATA {
  void* m_pHelperSrc;
  LPARAM m_lParam;
  RECT m_rcClient;
  __EXT_MFC_SAFE_LPCTSTR m_sText;
  int m_pIcon;
  int m_eAlign;
  HFONT m_hFont;
  UINT m_nHelperSysCmdID;
  BOOL m_bHorz: 1;
  BOOL m_bFlat: 1;
  BOOL m_bHover: 1;
  BOOL m_bPushed: 1;
  BOOL m_bIndeterminate: 1;
  BOOL m_bEnabled: 1;
  BOOL m_bDrawBorder: 1;
  BOOL m_bDrawFocusRect: 1;
  BOOL m_bDefaultPushButton: 1;
  BOOL m_bDropDown: 1;
  BOOL m_bTransparentBackground: 1;
  BOOL m_bSeparatedDropDown: 1;
  BOOL m_bPushedDropDown: 1;
  // custom colors
  COLORREF m_clrForceTextDisabled, m_clrForceTextNormal, m_clrForceTextHover, m_clrForceTextPressed;
  UINT m_nDrawTextFlagsH;
  BOOL m_bForceUseDrawTextApiH: 1;
} PAINTPUSHBUTTONDATA; // struct PAINTPUSHBUTTONDATA
typedef void CObject;
typedef struct __PROF_UIS_API _PAINTMENUITEMDATA {
  CObject* m_pHelperSrc;
  LPARAM m_lParam;
  RECT m_rcItem;
  INT m_nIconAreaWidth;
  __EXT_MFC_SAFE_LPCTSTR m_sText, m_sAccelText;
  int m_pIcon;
  UINT m_nHelperSysCmdID; // for painting system menu glyphs
  BOOL m_bPopup: 1;
  BOOL m_bSelected: 1;
  BOOL m_bPushedCheck: 1;
  BOOL m_bPushedRadio: 1;
  BOOL m_bIndeterminate: 1;
  BOOL m_bEnabled: 1;
  BOOL m_bRarelyUsed: 1;
  BOOL m_bRarelyUsedPrev: 1;
  BOOL m_bRarelyUsedNext: 1;
  BOOL m_bForceNoLeftGradient: 1;
  BOOL m_bCustomizeListMode: 1;
  BOOL m_bSeparatedDropDown: 1;
  BOOL m_bXtraMark: 1;
  BOOL m_bXtraChecked: 1;
  RECT m_rcInPlaceWnd;
  TCHAR* m_strInPlaceText;
  BOOL m_bHelperPopupDropped: 1;
} PAINTMENUITEMDATA; // struct PAINTMENUITEMDATA
struct __PROF_UIS_API _PAINTGRIPPERDATA {
  CObject* m_pHelperSrc;
  LPARAM m_lParam;
  const RECT m_rcGripper, m_rcText;
  BOOL m_bActive: 1;
  BOOL m_bFloating: 1;
  BOOL m_bHorz: 1;
  BOOL m_bSideBar: 1;
  __EXT_MFC_SAFE_LPCTSTR m_sCaption;
  BOOL m_bHelperNoRect: 1;
  BOOL m_bHelperNoFill: 1;
} PAINTGRIPPERDATA; // struct PAINTGRIPPERDATA
typedef struct __PROF_UIS_API _PAINTDOCKINGFRAMEDATA {
  CObject* m_pHelperSrc;
  LPARAM m_lParam;
  RECT m_rcWindow, m_rcClient;
  BOOL m_bFloating: 1;
  BOOL m_bExtBar: 1;
  BOOL m_bSideBar: 1;
} PAINTDOCKINGFRAMEDATA; // struct PAINTDOCKINGFRAMEDATA
typedef struct __PROF_UIS_API _PAINTCONTROLFRAMEDATA {
  CObject* m_pHelperSrc;
  LPARAM m_lParam;
  RECT m_rcClient;
  BOOL m_bFlat: 1;
  BOOL m_bHover: 1;
  BOOL m_bEnabled: 1;
  BOOL m_bFocus: 1;
} PAINTCONTROLFRAMEDATA; // struct PAINTCONTROLFRAMEDATA
typedef struct __PROF_UIS_API _PAINTCOMBOFRAMEDATA {
  CObject* m_pHelperSrc;
  LPARAM m_lParam;
  RECT m_rcClient;
  BOOL m_bHover: 1;
  BOOL m_bPushed: 1;
  BOOL m_bEnabled: 1;
  COLORREF m_clrForceNABorder, m_clrForceNAContent, m_clrForceActiveOuterBorder;
  BOOL m_bNoTopOuterBorder: 1;
} PAINTCOMBOFRAMEDATA; // struct PAINTCOMBOFRAMEDATA
enum e_control_bar_border_type_t {
  __CB_OUTER_DOCKBAR = 0, __CB_OUTER_STATUSBAR = 1, __CB_INNER_STATUSBAR_ITEM = 2,
};
typedef struct __PROF_UIS_API _PAINTCONTROLBARBORDERSDATA {
  CObject* m_pHelperSrc;
  LPARAM m_lParam;
  int m_eCBBT; // CExtPaintManager::e_control_bar_border_type_t
  DWORD m_dwStyle;
  RECT m_rc;
} PAINTCONTROLBARBORDERSDATA; // struct PAINTCONTROLBARBORDERSDATA
#if (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)
typedef struct __PROF_UIS_API _PAINTSHORTCUTLISTITEMSDATA {
  CObject* m_pHelperSrc;
  LPARAM m_lParam;
  int m_pIconBig,  *m_pIconSmall;
  LPARAM m_lParamItem;
  RECT m_rcBoundsAll, m_rcBoundsIcon, m_rcBoundsText, m_rcCtrlClient;
  TCHAR* m_sText;
  BOOL m_bHover: 1, m_bPressed: 1, m_bVisible: 1, m_bChecked: 1, m_bExpandMode: 1, m_bHorzLayout: 1, m_bBigIconView: 1, m_bEraseQuery: 1;
  DWORD m_dwShortcutListStyle;
  HFONT m_hItemTextFont;
} PAINTSHORTCUTLISTITEMSDATA; // struct PAINTSHORTCUTLISTITEMSDATA
#endif // (!defined __EXT_MFC_NO_SHORTCUTLIST_CTRL)
typedef struct __PROF_UIS_API _PAINTTOOLBARTEXTFIELDDATA {
  CObject* m_pHelperSrc;
  LPARAM m_lParam;
  UINT m_nDrawTextFlags;
  TCHAR* m_sText;
  RECT m_rcButton, m_rcTextField;
  BOOL m_bComboField: 1;
  BOOL m_bHover: 1;
  BOOL m_bPushed: 1;
  BOOL m_bEnabled: 1;
} PAINTTOOLBARTEXTFIELDDATA; // struct PAINTTOOLBARTEXTFIELDDATA
//}; // struct CExtPaintManager
//struct __PROF_UIS_API CExtPaintManagerXP: public CExtPaintManager {

BOOL m_bEnabledHoverIconShadows;
BOOL m_bHelperXpStyle8BitBarRect;
BOOL m_bExpBtnSwapVGlyphs;
const glyph_t* m_pGlyphTbEpBtnH0;
const glyph_t* m_pGlyphTbEpBtnH1;
const glyph_t* m_pGlyphTbEpBtnV0;
const glyph_t* m_pGlyphTbEpBtnV1;
int m_nIdxClrMlaNormLeft, m_nIdxClrMlaNormRight, m_nIdxClrMlaRarelyLeft, m_nIdxClrMlaRarelyRight, m_nIdxClrBtnNormLeft, m_nIdxClrBtnNormRight, m_nIdxClrBtnHoverLeft, m_nIdxClrBtnHoverRight, m_nIdxClrBtnPressedLeft, m_nIdxClrBtnPressedRight, m_nIdxClrBtnHovPresLeft, m_nIdxClrBtnHovPresRight;
enum e_xp_colors_t {
  XPCLR_3DFACE_DARK = __ExtMfc_XP_COLOR_SHIFT + 0, XPCLR_3DFACE_NORMAL = __ExtMfc_XP_COLOR_SHIFT + 1, XPCLR_SEPARATOR = __ExtMfc_XP_COLOR_SHIFT + 2, XPCLR_HILIGHT = __ExtMfc_XP_COLOR_SHIFT + 3, XPCLR_HILIGHT_BORDER = __ExtMfc_XP_COLOR_SHIFT + 4, XPCLR_RARELY_BORDER = __ExtMfc_XP_COLOR_SHIFT + 5, XPCLR_PUSHEDHOVERTEXT = __ExtMfc_XP_COLOR_SHIFT + 6, XPCLR_MENU_BORDER = __ExtMfc_XP_COLOR_SHIFT + 7, XPCLR_3DFACE_FLOAT_R = __ExtMfc_XP_COLOR_SHIFT + 8, XPCLR_3DFACE_FLOAT_F = __ExtMfc_XP_COLOR_SHIFT + 9,
};
#define GetMenuFrameFillColorXP() GetColor(XPCLR_3DFACE_NORMAL)
//}; // struct CExtPaintManagerXP
//struct __PROF_UIS_API CExtPaintManagerOffice2003: public CExtPaintManagerXP {
int m_bmpCtBarGradientHorz, *m_bmpCtBarGradientVert, *m_bmpDockerGradient;
SIZE m_sizeCtBarGradientHorz, m_sizeCtBarGradientVert, m_sizeDockerGradient, m_sizeMenuExpBtnCircle;
POINT m_rgnMenuExpBtnCircle[] = {4, 0, 11, 0, 15, 4, 15, 11, 11, 15, 4, 15, 0, 11, 0, 4,};
int m_nIdxClrTbFillMargin;
SIZE m_sizeToolBarRgnRounders;
enum e_2003_colors_t {
  _2003CLR_GRADIENT_LIGHT = __ExtMfc_2003_COLOR_SHIFT + 0, _2003CLR_GRADIENT_DARK = __ExtMfc_2003_COLOR_SHIFT + 1, _2003CLR_GRIPPER_DOT_LIGHT = __ExtMfc_2003_COLOR_SHIFT + 2, _2003CLR_GRIPPER_DOT_DARK = __ExtMfc_2003_COLOR_SHIFT + 3, _2003CLR_EXPBTN_LIGHT = __ExtMfc_2003_COLOR_SHIFT + 4, _2003CLR_EXPBTN_DARK = __ExtMfc_2003_COLOR_SHIFT + 5, _2003CLR_EXPBTN_HOVER_LIGHT = __ExtMfc_2003_COLOR_SHIFT + 6, _2003CLR_EXPBTN_HOVER_DARK = __ExtMfc_2003_COLOR_SHIFT + 7, _2003CLR_EXPBTN_PRESSED_LIGHT = __ExtMfc_2003_COLOR_SHIFT + 8, _2003CLR_EXPBTN_PRESSED_DARK = __ExtMfc_2003_COLOR_SHIFT + 9, _2003CLR_SEPARATOR_LIGHT = __ExtMfc_2003_COLOR_SHIFT + 10, _2003CLR_SEPARATOR_DARK = __ExtMfc_2003_COLOR_SHIFT + 11, _2003CLR_EXPGLYPH_LIGHT = __ExtMfc_2003_COLOR_SHIFT + 12, _2003CLR_EXPGLYPH_DARK = __ExtMfc_2003_COLOR_SHIFT + 13, _2003CLR_STATUSBAR_ITEM = __ExtMfc_2003_COLOR_SHIFT + 14, _2003CLR_MLA_NORM_LEFT = __ExtMfc_2003_COLOR_SHIFT + 15, _2003CLR_MLA_NORM_RIGHT = __ExtMfc_2003_COLOR_SHIFT + 16, _2003CLR_MLA_RARELY_LEFT = __ExtMfc_2003_COLOR_SHIFT + 17, _2003CLR_MLA_RARELY_RIGHT = __ExtMfc_2003_COLOR_SHIFT + 18, _2003CLR_BTN_HOVER_LEFT = __ExtMfc_2003_COLOR_SHIFT + 19, _2003CLR_BTN_HOVER_RIGHT = __ExtMfc_2003_COLOR_SHIFT + 20, _2003CLR_BTN_PRESSED_LEFT = __ExtMfc_2003_COLOR_SHIFT + 21, _2003CLR_BTN_PRESSED_RIGHT = __ExtMfc_2003_COLOR_SHIFT + 22, _2003CLR_BTN_HP_LEFT = __ExtMfc_2003_COLOR_SHIFT + 23, _2003CLR_BTN_HP_RIGHT = __ExtMfc_2003_COLOR_SHIFT + 24, _2003CLR_MENUEXPBALOON = __ExtMfc_2003_COLOR_SHIFT + 25,
};
//}; // struct CExtPaintManagerOffice2003
int g_PaintManager = 1;

