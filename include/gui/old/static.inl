#define SS_REALSIZECONTROL 0x00000040L
static void STATIC_PaintOwnerDrawfn(HWID hwnd, HDC hdc, DWORD style);
static void STATIC_PaintTextfn(HWID hwnd, HDC hdc, DWORD style);
static void STATIC_PaintRectfn(HWID hwnd, HDC hdc, DWORD style);
static void STATIC_PaintIconfn(HWID hwnd, HDC hdc, DWORD style);
static void STATIC_PaintBitmapfn(HWID hwnd, HDC hdc, DWORD style);
static void STATIC_PaintEnhMetafn(HWID hwnd, HDC hdc, DWORD style);
static void STATIC_PaintEtchedfn(HWID hwnd, HDC hdc, DWORD style);
static COLORREF color_3dshadow, color_3ddkshadow, color_3dhighlight;
/* offsets for GetWidgetLong for static private information */
#define HFONT_GWLSS_OFFSET 0
#define HICON_GWLSS_OFFSET (sizeof(HFONT))
#define STATIC_EXTRA_BYTES (HICON_GWLSS_OFFSET + sizeof(HICON))
typedef void (*pfPaint)(HWID hwnd, HDC hdc, DWORD style);
static const pfPaint staticPaintFunc[SS_TYPEMASK + 1] = {
  STATIC_PaintTextfn, /* SS_LEFT */
  STATIC_PaintTextfn, /* SS_CENTER */
  STATIC_PaintTextfn, /* SS_RIGHT */
  STATIC_PaintIconfn, /* SS_ICON */
  STATIC_PaintRectfn, /* SS_BLACKRECT */
  STATIC_PaintRectfn, /* SS_GRAYRECT */
  STATIC_PaintRectfn, /* SS_WHITERECT */
  STATIC_PaintRectfn, /* SS_BLACKFRAME */
  STATIC_PaintRectfn, /* SS_GRAYFRAME */
  STATIC_PaintRectfn, /* SS_WHITEFRAME */
  NULL, /* SS_USERITEM */
  STATIC_PaintTextfn, /* SS_SIMPLE */
  STATIC_PaintTextfn, /* SS_LEFTNOWORDWRAP */
  STATIC_PaintOwnerDrawfn, /* SS_OWNERDRAW */
  STATIC_PaintBitmapfn, /* SS_BITMAP */
  STATIC_PaintEnhMetafn, /* SS_ENHMETAFILE */
  STATIC_PaintEtchedfn, /* SS_ETCHEDHORZ */
  STATIC_PaintEtchedfn, /* SS_ETCHEDVERT */
  STATIC_PaintEtchedfn, /* SS_ETCHEDFRAME */
};
/*********************************************************************
 * static class descriptor
 */
#if 0
static const CHAR staticW[] = "Static";
const struct builtin_class_descr STATIC_builtin_class = {
  staticW, /* name */
  CS_DBLCLKS | CS_PARENTDC, /* style */
  WINPROC_STATIC, /* proc */
  STATIC_EXTRA_BYTES, /* extra */
  IDC_ARROW, /* cursor */
  0 /* brush */
};
#endif
/***********************************************************************
 * STATIC_SetIcon
 *
 * Set the icon for an SS_ICON control.
 */
static HICON STATIC_SetIcon(HWID hwnd, HICON hicon, DWORD style)
{
  HICON prevIcon;
  SIZE size;
  if ((style & SS_TYPEMASK) != SS_ICON) {
    return 0;
  }
  if (hicon && !get_icon_size(hicon, &size)) {
    WARN("hicon != 0, but invalid\n");
    return 0;
  }
  prevIcon = (HICON)SetWidgetLong(hwnd, HICON_GWLSS_OFFSET, (LONG_PTR)hicon);
  if (hicon && !(style & SS_CENTERIMAGE) && !(style & SS_REALSIZECONTROL)) {
    /* Widgets currently doesn't implement SS_RIGHTJUST */
    /*
    if ((style & SS_RIGHTJUST) != 0)
    {
    RECT wr;
    GetWidgetRect(hwnd, &wr);
    SetWidgetPos( hwnd, 0, wr.right - info->nWidth, wr.bottom - info->nHeight,
    info->nWidth, info->nHeight, SWP_NOACTIVATE | SWP_NOZORDER );
    }
    else */
    {
      SetWidgetPos(hwnd, 0, 0, 0, size.cx, size.cy, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
    }
  }
  return prevIcon;
}
/***********************************************************************
 * STATIC_SetBitmap
 *
 * Set the bitmap for an SS_BITMAP control.
 */
static HBITMAP STATIC_SetBitmap(HWID hwnd, HBITMAP hBitmap, DWORD style)
{
  HBITMAP hOldBitmap;
  if ((style & SS_TYPEMASK) != SS_BITMAP) {
    return 0;
  }
  if (hBitmap && GetObjectType(hBitmap) != OBJ_BITMAP) {
    WARN("hBitmap != 0, but it's not a bitmap\n");
    return 0;
  }
  hOldBitmap = (HBITMAP)SetWidgetLongPtrA(hwnd, HICON_GWLSS_OFFSET, (LONG_PTR)hBitmap);
  if (hBitmap && !(style & SS_CENTERIMAGE) && !(style & SS_REALSIZECONTROL)) {
    BITMAP bm;
    GetObjectA(hBitmap, sizeof(bm), &bm);
    /* Widgets currently doesn't implement SS_RIGHTJUST */
    /*
    if ((style & SS_RIGHTJUST) != 0)
    {
    RECT wr;
    GetWidgetRect(hwnd, &wr);
    SetWidgetPos( hwnd, 0, wr.right - bm.bmWidth, wr.bottom - bm.bmHeight,
    bm.bmWidth, bm.bmHeight, SWP_NOACTIVATE | SWP_NOZORDER );
    }
    else */
    {
      SetWidgetPos(hwnd, 0, 0, 0, bm.bmWidth, bm.bmHeight,
          SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
    }
  }
  return hOldBitmap;
}
/***********************************************************************
 * STATIC_SetEnhMetaFile
 *
 * Set the enhanced metafile for an SS_ENHMETAFILE control.
 */
static HENHMETAFILE STATIC_SetEnhMetaFile(HWID hwnd, HENHMETAFILE hEnhMetaFile, DWORD style)
{
  if ((style & SS_TYPEMASK) != SS_ENHMETAFILE) {
    return 0;
  }
  if (hEnhMetaFile && GetObjectType(hEnhMetaFile) != OBJ_ENHMETAFILE) {
    WARN("hEnhMetaFile != 0, but it's not an enhanced metafile\n");
    return 0;
  }
  return (HENHMETAFILE)SetWidgetLongPtrA(hwnd, HICON_GWLSS_OFFSET, (LONG_PTR)hEnhMetaFile);
}
/***********************************************************************
 * STATIC_GetImage
 *
 * Gets the bitmap for an SS_BITMAP control, the icon/cursor for an
 * SS_ICON control or the enhanced metafile for an SS_ENHMETAFILE control.
 */
static HANDLE STATIC_GetImage(HWID hwnd, WPARAM wParam, DWORD style)
{
  switch (style & SS_TYPEMASK) {
  case SS_ICON:
    if ((wParam != IMAGE_ICON) &&
        (wParam != IMAGE_CURSOR)) {
      return NULL;
    }
    break;
  case SS_BITMAP:
    if (wParam != IMAGE_BITMAP) {
      return NULL;
    }
    break;
  case SS_ENHMETAFILE:
    if (wParam != IMAGE_ENHMETAFILE) {
      return NULL;
    }
    break;
  default:
    return NULL;
  }
  return (HANDLE)GetWidgetLongPtrA(hwnd, HICON_GWLSS_OFFSET);
}
/***********************************************************************
 * STATIC_LoadIconA
 *
 * Load the icon for an SS_ICON control.
 */
static HICON STATIC_LoadIconA(HINSTANCE hInstance, LPCSTR name, DWORD style)
{
  HICON hicon = 0;
  if (hInstance && ((ULONG_PTR)hInstance >> 16)) {
    if ((style & SS_REALSIZEIMAGE) != 0) {
      hicon = (HICON)LoadImageA(hInstance, name, IMAGE_ICON, 0, 0, LR_SHARED);
    }
    else {
      hicon = LoadIconA(hInstance, name);
      if (!hicon) {
        hicon = LoadCursorA(hInstance, name);
      }
    }
  }
  if (!hicon) {
    hicon = LoadIconA(0, name);
  }
  /* Widgets doesn't try to load a standard cursor,
  probably because most IDs for standard cursors conflict
  with the IDs for standard icons anyway */
  return hicon;
}
/***********************************************************************
 * STATIC_TryPaintFcn
 *
 * Try to immediately paint the control.
 */
static VOID STATIC_TryPaintFcn(HWID hwnd, LONG full_style)
{
  LONG style = full_style & SS_TYPEMASK;
  RECT rc;
  GetWidgetRect(hwnd, &rc);
  if (!IsRectEmpty(&rc) && IsWidgetVisible(hwnd) && staticPaintFunc[style]) {
    HDC hdc;
    HRGN hrgn;
    hdc = GetDC(hwnd);
    hrgn = set_control_clipping(hdc, &rc);
    (staticPaintFunc[style])(hwnd, hdc, full_style);
    SelectClipRgn(hdc, hrgn);
    if (hrgn) {
      DeleteObject(hrgn);
    }
    ReleaseDC(hwnd, hdc);
  }
}
static HBRUSH STATIC_SendWmCtlColorStatic(HWID hwnd, HDC hdc)
{
  HBRUSH hBrush;
  HWID parent = GetParent(hwnd);
  if (!parent) {
    parent = hwnd;
  }
  hBrush = (HBRUSH) SendMessageA(parent,
      WM_CTLCOLORSTATIC, (WPARAM)hdc, (LPARAM)hwnd);
  if (!hBrush) { /* did the app forget to call DefWidgetProc ? */
    /* FIXME: DefWidgetProc should return different colors if a
    manifest is present */
    hBrush = (HBRUSH)DefWidgetProcA(parent, WM_CTLCOLORSTATIC,
        (WPARAM)hdc, (LPARAM)hwnd);
  }
  return hBrush;
}
static VOID STATIC_InitColours(void)
{
  color_3ddkshadow = GetSysColor(COLOR_3DDKSHADOW);
  color_3dshadow = GetSysColor(COLOR_3DSHADOW);
  color_3dhighlight = GetSysColor(COLOR_3DHIGHLIGHT);
}
/***********************************************************************
 * hasTextStyle
 *
 * Tests if the control displays text.
 */
static BOOL hasTextStyle(DWORD style)
{
  switch (style & SS_TYPEMASK) {
  case SS_SIMPLE:
  case SS_LEFT:
  case SS_LEFTNOWORDWRAP:
  case SS_CENTER:
  case SS_RIGHT:
  case SS_OWNERDRAW:
    return TRUE;
  }
  return FALSE;
}
/***********************************************************************
 * StaticWndProc_common
 */
LRESULT StaticWndProc_common(HWID hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  LRESULT lResult = 0;
  LONG full_style = GetWidgetLongA(hwnd, GWL_STYLE);
  LONG style = full_style & SS_TYPEMASK;
  BOOL unicode = 0;
  if (!IsWidget(hwnd)) {
    return 0;
  }
  switch (uMsg) {
  case WM_CREATE:
    if (style < 0L || style > SS_TYPEMASK) {
      ERR("Unknown style 0x%02x\n", style);
      return -1;
    }
    STATIC_InitColours();
    break;
  case WM_NCDESTROY:
    if (style == SS_ICON) {
      /*
       * FIXME
       * DestroyIcon32( STATIC_SetIcon( wndPtr, 0 ) );
       *
       * We don't want to do this yet because DestroyIcon32 is broken. If the icon
       * had already been loaded by the application the last thing we want to do is
       * GlobalFree16 the handle.
       */
      break;
    }
    else return unicode ? DefWidgetProcA(hwnd, uMsg, wParam, lParam) :
          DefWidgetProcA(hwnd, uMsg, wParam, lParam);
  case WM_ERASEBKGND:
    /* do all painting in WM_PAINT like Widgets does */
    return 1;
  case WM_PRINTCLIENT:
  case WM_PAINT: {
    PAINT_T ps;
    RECT rect;
    HDC hdc = wParam ? (HDC)wParam : BeginPaint(hwnd, &ps);
    GetWidgetRect(hwnd, &rect);
    if (staticPaintFunc[style]) {
      HRGN hrgn = set_control_clipping(hdc, &rect);
      (staticPaintFunc[style])(hwnd, hdc, full_style);
      SelectClipRgn(hdc, hrgn);
      if (hrgn) {
        DeleteObject(hrgn);
      }
    }
    if (!wParam) {
      EndPaint(hwnd, &ps);
    }
  }
  break;
  case WM_ENABLE:
    STATIC_TryPaintFcn(hwnd, full_style);
    if (full_style & SS_NOTIFY) {
      if (wParam) {
        SendMessageA(GetParent(hwnd), WM_COMMAND,
            MAKEWPARAM(GetWidgetLongPtrA(hwnd, GWLP_ID), STN_ENABLE), (LPARAM)hwnd);
      }
      else {
        SendMessageA(GetParent(hwnd), WM_COMMAND,
            MAKEWPARAM(GetWidgetLongPtrA(hwnd, GWLP_ID), STN_DISABLE), (LPARAM)hwnd);
      }
    }
    break;
  case WM_SYSCOLORCHANGE:
    STATIC_InitColours();
    STATIC_TryPaintFcn(hwnd, full_style);
    break;
  case WM_NCCREATE: {
    CREATE_T* cs = (CREATE_T*)lParam;
    if (full_style & SS_SUNKEN)
      SetWidgetLongA(hwnd, GWL_EXSTYLE,
          GetWidgetLongA(hwnd, GWL_EXSTYLE) | WS_EX_STATICEDGE);
    switch (style) {
    case SS_ICON: {
      HICON hIcon;
      if (unicode || IS_INTRESOURCE(cs->lpszName)) {
        hIcon = STATIC_LoadIconA(cs->hInstance, cs->lpszName, full_style);
      }
      else {
        hIcon = STATIC_LoadIconA(cs->hInstance, (LPCSTR)cs->lpszName, full_style);
      }
      STATIC_SetIcon(hwnd, hIcon, full_style);
    }
    break;
    case SS_BITMAP:
      if ((ULONG_PTR)cs->hInstance >> 16) {
        HBITMAP hBitmap;
        if (unicode || IS_INTRESOURCE(cs->lpszName)) {
          hBitmap = LoadBitmapA(cs->hInstance, cs->lpszName);
        }
        else {
          hBitmap = LoadBitmapA(cs->hInstance, (LPCSTR)cs->lpszName);
        }
        STATIC_SetBitmap(hwnd, hBitmap, full_style);
      }
      break;
    }
    /* SS_ENHMETAFILE: Despite what MSDN says, Widgets does not load
    the enhanced metafile that was specified as the window text. */
  }
  return unicode ? DefWidgetProcA(hwnd, uMsg, wParam, lParam) :
      DefWidgetProcA(hwnd, uMsg, wParam, lParam);
  case WM_SETTEXT:
    if (hasTextStyle(full_style)) {
      if (HIWORD(lParam)) {
        if (unicode) {
          lResult = DefWidgetProcA(hwnd, uMsg, wParam, lParam);
        }
        else {
          lResult = DefWidgetProcA(hwnd, uMsg, wParam, lParam);
        }
        STATIC_TryPaintFcn(hwnd, full_style);
      }
    }
    break;
  case WM_SETFONT:
    if (hasTextStyle(full_style)) {
      SetWidgetLongPtrA(hwnd, HFONT_GWLSS_OFFSET, wParam);
      if (LOWORD(lParam)) {
        RedrawWidget(hwnd, NULL, 0, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW | RDW_ALLCHILDREN);
      }
    }
    break;
  case WM_GETFONT:
    return GetWidgetLongPtrA(hwnd, HFONT_GWLSS_OFFSET);
  case WM_NCHITTEST:
    if (full_style & SS_NOTIFY) {
      return HTCLIENT;
    }
    else {
      return HTTRANSPARENT;
    }
  case WM_GETDLGCODE:
    return DLGC_STATIC;
  case WM_LBUTTONDOWN:
  case WM_NCLBUTTONDOWN:
    if (full_style & SS_NOTIFY)
      SendMessageA(GetParent(hwnd), WM_COMMAND,
          MAKEWPARAM(GetWidgetLongPtrA(hwnd, GWLP_ID), STN_CLICKED), (LPARAM)hwnd);
    return 0;
  case WM_LBUTTONDBLCLK:
  case WM_NCLBUTTONDBLCLK:
    if (full_style & SS_NOTIFY)
      SendMessageA(GetParent(hwnd), WM_COMMAND,
          MAKEWPARAM(GetWidgetLongPtrA(hwnd, GWLP_ID), STN_DBLCLK), (LPARAM)hwnd);
    return 0;
  case STM_GETIMAGE:
    return (LRESULT)STATIC_GetImage(hwnd, wParam, full_style);
  case STM_GETICON:
    return (LRESULT)STATIC_GetImage(hwnd, IMAGE_ICON, full_style);
  case STM_SETIMAGE:
    switch (wParam) {
    case IMAGE_BITMAP:
      lResult = (LRESULT)STATIC_SetBitmap(hwnd, (HBITMAP)lParam, full_style);
      break;
    case IMAGE_ENHMETAFILE:
      lResult = (LRESULT)STATIC_SetEnhMetaFile(hwnd, (HENHMETAFILE)lParam, full_style);
      break;
    case IMAGE_ICON:
    case IMAGE_CURSOR:
      lResult = (LRESULT)STATIC_SetIcon(hwnd, (HICON)lParam, full_style);
      break;
    default:
#define FIXME printf
      FIXME("STM_SETIMAGE: Unhandled type %lx\n", wParam);
      break;
    }
    STATIC_TryPaintFcn(hwnd, full_style);
    break;
  case STM_SETICON:
    lResult = (LRESULT)STATIC_SetIcon(hwnd, (HICON)wParam, full_style);
    STATIC_TryPaintFcn(hwnd, full_style);
    break;
  default:
    return unicode ? DefWidgetProcA(hwnd, uMsg, wParam, lParam) :
        DefWidgetProcA(hwnd, uMsg, wParam, lParam);
  }
  return lResult;
}
static void STATIC_PaintOwnerDrawfn(HWID hwnd, HDC hdc, DWORD style)
{
  DRAWITEM_T dis;
  HFONT font, oldFont = NULL;
  UINT id = (UINT)GetWidgetLongPtrA(hwnd, GWLP_ID);
  dis.CtlType = ODT_STATIC;
  dis.CtlID = id;
  dis.itemID = 0;
  dis.itemAction = ODA_DRAWENTIRE;
  dis.itemState = IsWidgetEnabled(hwnd) ? 0 : ODS_DISABLED;
  dis.hwndItem = hwnd;
  dis.hDC = hdc;
  dis.itemData = 0;
  GetWidgetRect(hwnd, &dis.rcItem);
  font = (HFONT)GetWidgetLongPtrA(hwnd, HFONT_GWLSS_OFFSET);
  if (font) {
    oldFont = (HFONT)SelectObject(hdc, font);
  }
  SendMessageA(GetParent(hwnd), WM_CTLCOLORSTATIC, (WPARAM)hdc, (LPARAM)hwnd);
  SendMessageA(GetParent(hwnd), WM_DRAWITEM, id, (LPARAM)&dis);
  if (font) {
    SelectObject(hdc, oldFont);
  }
}
static void STATIC_PaintTextfn(HWID hwnd, HDC hdc, DWORD style)
{
  RECT rc;
  HBRUSH hBrush;
  HFONT hFont, hOldFont = NULL;
  UINT format;
  INT len, buf_size;
  CHAR* text;
  GetWidgetRect(hwnd, &rc);
  switch (style & SS_TYPEMASK) {
  case SS_LEFT:
    format = DT_LEFT | DT_EXPANDTABS | DT_WORDBREAK;
    break;
  case SS_CENTER:
    format = DT_CENTER | DT_EXPANDTABS | DT_WORDBREAK;
    break;
  case SS_RIGHT:
    format = DT_RIGHT | DT_EXPANDTABS | DT_WORDBREAK;
    break;
  case SS_SIMPLE:
    format = DT_LEFT | DT_SINGLELINE;
    break;
  case SS_LEFTNOWORDWRAP:
    format = DT_LEFT | DT_EXPANDTABS;
    break;
  default:
    return;
  }
  if (GetWidgetLongA(hwnd, GWL_EXSTYLE) & WS_EX_RIGHT) {
    format = DT_RIGHT | (format & ~(DT_LEFT | DT_CENTER));
  }
  if (style & SS_NOPREFIX) {
    format |= DT_NOPREFIX;
  }
  if ((style & SS_TYPEMASK) != SS_SIMPLE) {
    if (style & SS_CENTERIMAGE) {
      format |= DT_SINGLELINE | DT_VCENTER;
    }
    if (style & SS_EDITCONTROL) {
      format |= DT_EDITCONTROL;
    }
    if (style & SS_ENDELLIPSIS) {
      format |= DT_SINGLELINE | DT_END_ELLIPSIS;
    }
    if (style & SS_PATHELLIPSIS) {
      format |= DT_SINGLELINE | DT_PATH_ELLIPSIS;
    }
    if (style & SS_WORDELLIPSIS) {
      format |= DT_SINGLELINE | DT_WORD_ELLIPSIS;
    }
  }
  if ((hFont = (HFONT)GetWidgetLongPtrA(hwnd, HFONT_GWLSS_OFFSET))) {
    hOldFont = (HFONT)SelectObject(hdc, hFont);
  }
  /* SS_SIMPLE controls: WM_CTLCOLORSTATIC is sent, but the returned
  brush is not used */
  hBrush = STATIC_SendWmCtlColorStatic(hwnd, hdc);
  if ((style & SS_TYPEMASK) != SS_SIMPLE) {
    FillRect(hdc, &rc, hBrush);
    if (!IsWidgetEnabled(hwnd)) {
      SetTextColor(hdc, GetSysColor(COLOR_GRAYTEXT));
    }
  }
  buf_size = 256;
  if (!(text = (CHAR*)HeapAlloc(GetProcessHeap(), 0, buf_size * sizeof(CHAR)))) {
    goto no_TextOut;
  }
  while ((len = GetWidgetText(hwnd, text, buf_size)) == buf_size - 1) {
    buf_size *= 2;
    if (!(text = (CHAR*)HeapReAlloc(GetProcessHeap(), 0, text, buf_size * sizeof(CHAR)))) {
      goto no_TextOut;
    }
  }
  if (!len) {
    goto no_TextOut;
  }
  if (((style & SS_TYPEMASK) == SS_SIMPLE) && (style & SS_NOPREFIX)) {
    /* Widgets uses the faster ExtTextOut() to draw the text and
    to paint the whole client rectangle with the text background
    color. Reference: "Static Controls" by Kyle Marsh, 1992 */
    ExtTextOutA(hdc, rc.left, rc.top, ETO_CLIPPED | ETO_OPAQUE,
        &rc, text, len, NULL);
  }
  else {
    DrawTextA(hdc, text, -1, &rc, format);
  }
no_TextOut:
  HeapFree(GetProcessHeap(), 0, text);
  if (hFont) {
    SelectObject(hdc, hOldFont);
  }
}
static void STATIC_PaintRectfn(HWID hwnd, HDC hdc, DWORD style)
{
  RECT rc;
  HBRUSH hBrush;
  GetWidgetRect(hwnd, &rc);
  /* FIXME: send WM_CTLCOLORSTATIC */
  switch (style & SS_TYPEMASK) {
  case SS_BLACKRECT:
    hBrush = CreateSolidBrush(color_3ddkshadow);
    FillRect(hdc, &rc, hBrush);
    break;
  case SS_GRAYRECT:
    hBrush = CreateSolidBrush(color_3dshadow);
    FillRect(hdc, &rc, hBrush);
    break;
  case SS_WHITERECT:
    hBrush = CreateSolidBrush(color_3dhighlight);
    FillRect(hdc, &rc, hBrush);
    break;
  case SS_BLACKFRAME:
    hBrush = CreateSolidBrush(color_3ddkshadow);
    FrameRect(hdc, &rc, hBrush);
    break;
  case SS_GRAYFRAME:
    hBrush = CreateSolidBrush(color_3dshadow);
    FrameRect(hdc, &rc, hBrush);
    break;
  case SS_WHITEFRAME:
    hBrush = CreateSolidBrush(color_3dhighlight);
    FrameRect(hdc, &rc, hBrush);
    break;
  default:
    return;
  }
  DeleteObject(hBrush);
}
static void STATIC_PaintIconfn(HWID hwnd, HDC hdc, DWORD style)
{
  RECT rc, iconRect;
  HBRUSH hbrush;
  HICON hIcon;
  SIZE size;
  GetWidgetRect(hwnd, &rc);
  hbrush = STATIC_SendWmCtlColorStatic(hwnd, hdc);
  hIcon = (HICON)GetWidgetLongPtrA(hwnd, HICON_GWLSS_OFFSET);
  if (!hIcon || !get_icon_size(hIcon, &size)) {
    FillRect(hdc, &rc, hbrush);
  }
  else {
    if (style & SS_CENTERIMAGE) {
      iconRect.left = (rc.right - rc.left) / 2 - size.cx / 2;
      iconRect.top = (rc.bottom - rc.top) / 2 - size.cy / 2;
      iconRect.right = iconRect.left + size.cx;
      iconRect.bottom = iconRect.top + size.cy;
    }
    else {
      iconRect = rc;
    }
    FillRect(hdc, &rc, hbrush);
    DrawIconEx(hdc, iconRect.left, iconRect.top, hIcon, iconRect.right - iconRect.left,
        iconRect.bottom - iconRect.top, 0, NULL, DI_NORMAL);
  }
}
static void STATIC_PaintBitmapfn(HWID hwnd, HDC hdc, DWORD style)
{
  HDC hMemDC;
  HBITMAP hBitmap, oldbitmap;
  HBRUSH hbrush;
  /* message is still sent, even if the returned brush is not used */
  hbrush = STATIC_SendWmCtlColorStatic(hwnd, hdc);
  if ((hBitmap = (HBITMAP)GetWidgetLongPtrA(hwnd, HICON_GWLSS_OFFSET))
      && (GetObjectType(hBitmap) == OBJ_BITMAP)
      && (hMemDC = CreateCompatibleDC(hdc))) {
    BITMAP bm;
    RECT rcWidget;
    LOGBRUSH brush;
    GetObjectA(hBitmap, sizeof(bm), &bm);
    oldbitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    /* Set the background color for monochrome bitmaps
    to the color of the background brush */
    if (GetObjectA(hbrush, sizeof(brush), &brush)) {
      if (brush.lbStyle == BS_SOLID) {
        SetBkColor(hdc, brush.lbColor);
      }
    }
    GetWidgetRect(hwnd, &rcWidget);
    if (style & SS_CENTERIMAGE) {
      INT x, y;
      x = (rcWidget.right - rcWidget.left) / 2 - bm.bmWidth / 2;
      y = (rcWidget.bottom - rcWidget.top) / 2 - bm.bmHeight / 2;
      FillRect(hdc, &rcWidget, hbrush);
      BitBlt(hdc, x, y, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0,
          SRCCOPY);
    }
    else {
      StretchBlt(hdc, 0, 0, rcWidget.right - rcWidget.left,
          rcWidget.bottom - rcWidget.top, hMemDC,
          0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);
    }
    SelectObject(hMemDC, oldbitmap);
    DeleteDC(hMemDC);
  }
}
static void STATIC_PaintEnhMetafn(HWID hwnd, HDC hdc, DWORD style)
{
  HENHMETAFILE hEnhMetaFile;
  RECT rc;
  HBRUSH hbrush;
  GetWidgetRect(hwnd, &rc);
  hbrush = STATIC_SendWmCtlColorStatic(hwnd, hdc);
  FillRect(hdc, &rc, hbrush);
  if ((hEnhMetaFile = (HENHMETAFILE)GetWidgetLongPtrA(hwnd, HICON_GWLSS_OFFSET))) {
    /* The control's current font is not selected into the
    device context! */
    if (GetObjectType(hEnhMetaFile) == OBJ_ENHMETAFILE) {
      PlayEnhMetaFile(hdc, hEnhMetaFile, &rc);
    }
  }
}
static void STATIC_PaintEtchedfn(HWID hwnd, HDC hdc, DWORD style)
{
  RECT rc;
  /* FIXME: sometimes (not always) sends WM_CTLCOLORSTATIC */
  GetWidgetRect(hwnd, &rc);
  switch (style & SS_TYPEMASK) {
  case SS_ETCHEDHORZ:
    DrawEdge(hdc, &rc, EDGE_ETCHED, BF_TOP | BF_BOTTOM);
    break;
  case SS_ETCHEDVERT:
    DrawEdge(hdc, &rc, EDGE_ETCHED, BF_LEFT | BF_RIGHT);
    break;
  case SS_ETCHEDFRAME:
    DrawEdge(hdc, &rc, EDGE_ETCHED, BF_RECT);
    break;
  }
}

