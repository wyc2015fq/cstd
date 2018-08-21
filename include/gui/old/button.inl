/* File: button.c -- Button type widgets
 *
 * Copyright (C) 1993 Johannes Ruscheinski
 * Copyright (C) 1993 David Metcalfe
 * Copyright (C) 1994 Alexandre Julliard
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 *
 * NOTES
 *
 * This code was audited for completeness against the documented features
 * of Comctl32.dll version 6.0 on Oct. 3, 2004, by Dimitrie O. Paun.
 *
 * Unless otherwise noted, we believe this code to be complete, as per
 * the specification mentioned above.
 * If you discover missing features, or bugs, please note them below.
 *
 * TODO
 * Styles
 * - BS_NOTIFY: is it complete?
 * - BS_RIGHTBUTTON: same as BS_LEFTTEXT
 *
 * Messages
 * - WM_CHAR: Checks a (manual or automatic) check box on '+' or '=', clears it on '-' key.
 * - WM_SETFOCUS: For (manual or automatic) radio buttons, send the parent window BN_CLICKED
 * - WM_NCCREATE: Turns any BS_OWNERDRAW button into a BS_PUSHBUTTON button.
 * - WM_SYSKEYUP
 * - BCM_GETIDEALSIZE
 * - BCM_GETIMAGELIST
 * - BCM_GETTEXTMARGIN
 * - BCM_SETIMAGELIST
 * - BCM_SETTEXTMARGIN
 *
 * Notifications
 * - BCN_HOTITEMCHANGE
 * - BN_DISABLE
 * - BN_PUSHED/BN_HILITE
 * + BN_KILLFOCUS: is it OK?
 * - BN_PAINT
 * + BN_SETFOCUS: is it OK?
 * - BN_UNPUSHED/BN_UNHILITE
 * - NM_CUSTOMDRAW
 *
 * Structures/Macros/Definitions
 * - BUTTON_IMAGELIST
 * - NMBCHOTITEM
 * - Button_GetIdealSize
 * - Button_GetImageList
 * - Button_GetTextMargin
 * - Button_SetImageList
 * - Button_SetTextMargin
 */
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#define OEMRESOURCE
#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
//#include "controls.h"
//#include "win.h"
//#include "user_private.h"
//#include "wine/debug.h"
//WINE_DEFAULT_DEBUG_CHANNEL(button);
/* GetWidgetLong offsets for window extra information */
#define STATE_GWLBN_OFFSET 0
#define HFONT_GWLBN_OFFSET (sizeof(LONG))
#define HIMAGE_GWLBN_OFFSET (HFONT_GWLBN_OFFSET+sizeof(HFONT))
#define NB_EXTRA_BYTES (HIMAGE_GWLBN_OFFSET+sizeof(HANDLE))
/* undocumented flags */
#define BUTTON_NSTATES 0x0F
#define BUTTON_BTNPRESSED 0x40
#define BUTTON_UNKNOWN2 0x20
#define BUTTON_UNKNOWN3 0x10
#define BUTTON_NOTIFY_PARENT(hWid, code) \
  do { /* Notify parent which has created this button control */ \
    TRACE("notification " #code " sent to hWid=%p\n", GetParent(hWid)); \
    SendMessage(GetParent(hWid), WM_COMMAND, \
        MAKEWPARAM(GetWidgetLongPtrW((hWid),GWLP_ID), (code)), \
        (LPARAM)(hWid)); \
  } while(0)
static UINT BUTTON_CalcLabelRect(HWID hWid, HDC hdc, RECT* rc);
static void PB_Paint(HWID hWid, HDC hDC, DWORD action);
static void CB_Paint(HWID hWid, HDC hDC, DWORD action);
static void GB_Paint(HWID hWid, HDC hDC, DWORD action);
static void UB_Paint(HWID hWid, HDC hDC, DWORD action);
static void OB_Paint(HWID hWid, HDC hDC, DWORD action);
static void BUTTON_CheckAutoRadioButton(HWID hWid);
#define MAX_BTN_TYPE 16
static const WORD maxCheckState[MAX_BTN_TYPE] = {
  BST_UNCHECKED, /* BS_PUSHBUTTON */
  BST_UNCHECKED, /* BS_DEFPUSHBUTTON */
  BST_CHECKED, /* BS_CHECKBOX */
  BST_CHECKED, /* BS_AUTOCHECKBOX */
  BST_CHECKED, /* BS_RADIOBUTTON */
  BST_INDETERMINATE, /* BS_3STATE */
  BST_INDETERMINATE, /* BS_AUTO3STATE */
  BST_UNCHECKED, /* BS_GROUPBOX */
  BST_UNCHECKED, /* BS_USERBUTTON */
  BST_CHECKED, /* BS_AUTORADIOBUTTON */
  BST_UNCHECKED, /* BS_PUSHBOX */
  BST_UNCHECKED /* BS_OWNERDRAW */
};
static const pfPaint btnPaintFunc[MAX_BTN_TYPE] = {
  PB_Paint, /* BS_PUSHBUTTON */
  PB_Paint, /* BS_DEFPUSHBUTTON */
  CB_Paint, /* BS_CHECKBOX */
  CB_Paint, /* BS_AUTOCHECKBOX */
  CB_Paint, /* BS_RADIOBUTTON */
  CB_Paint, /* BS_3STATE */
  CB_Paint, /* BS_AUTO3STATE */
  GB_Paint, /* BS_GROUPBOX */
  UB_Paint, /* BS_USERBUTTON */
  CB_Paint, /* BS_AUTORADIOBUTTON */
  NULL, /* BS_PUSHBOX */
  OB_Paint /* BS_OWNERDRAW */
};
static HBITMAP hbitmapCheckBoxes = 0;
static WORD checkBoxWidth = 0, checkBoxHeight = 0;
static inline LONG get_button_state(HWID hWid)
{
  return GetWidgetLong(hWid, STATE_GWLBN_OFFSET);
}
static inline void set_button_state(HWID hWid, LONG state)
{
  SetWidgetLong(hWid, STATE_GWLBN_OFFSET, state);
}
static inline HFONT get_button_font(HWID hWid)
{
  return (HFONT)GetWidgetLong(hWid, HFONT_GWLBN_OFFSET);
}
static inline void set_button_font(HWID hWid, HFONT font)
{
  SetWidgetLong(hWid, HFONT_GWLBN_OFFSET, (LONG_PTR)font);
}
static inline UINT get_button_type(LONG window_style)
{
  return (window_style & BS_TYPEMASK);
}
/* paint a button of any type */
static inline void paint_button(HWID hWid, LONG style, DWORD action)
{
  if (btnPaintFunc[style] && IsVisible(hWid)) {
    //HDC hdc = GetDC( hWid );
    //btnPaintFunc[style]( hWid, hdc, action );
    //ReleaseDC( hWid, hdc );
    InvalidateRect(hWid, NULL, TRUE);
  }
}
/* retrieve the button text; returned buffer must be freed by caller */
static inline char* get_button_text(HWID hWid)
{
  INT len = 512;
  char* buffer = (char*)HeapAlloc(GetProcessHeap(), 0, (len + 1) * sizeof(char));
  if (buffer) {
    GetWidgetText(hWid, buffer, len + 1);
  }
  return buffer;
}
/***********************************************************************
 * ButtonWndProc_common
 */
LRESULT CALLBACK ButtonWndProc_common(HWID hWid, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  RECT rect;
  POINT pt;
  LONG style = GetWidgetLong(hWid, GWL_STYLE);
  UINT btn_type = get_button_type(style);
  LONG state;
  HANDLE oldHbitmap;
  BOOL unicode = 0;
  if (!IsWidget(hWid)) {
    return 0;
  }
  pt.x = (short)LOWORD(lParam);
  pt.y = (short)HIWORD(lParam);
  switch (uMsg) {
  case WM_GETDLGCODE:
    switch (btn_type) {
    case BS_USERBUTTON:
    case BS_PUSHBUTTON:
      return DLGC_BUTTON | DLGC_UNDEFPUSHBUTTON;
    case BS_DEFPUSHBUTTON:
      return DLGC_BUTTON | DLGC_DEFPUSHBUTTON;
    case BS_RADIOBUTTON:
    case BS_AUTORADIOBUTTON:
      return DLGC_BUTTON | DLGC_RADIOBUTTON;
    case BS_GROUPBOX:
      return DLGC_STATIC;
    default:
      return DLGC_BUTTON;
    }
  case WM_ENABLE:
    paint_button(hWid, btn_type, ODA_DRAWENTIRE);
    break;
  case WM_CREATE:
    if (!hbitmapCheckBoxes) {
      BITMAP bmp;
      hbitmapCheckBoxes = LoadXpm(obm_checkboxes_xpm);
      GetObjectW(hbitmapCheckBoxes, sizeof(bmp), &bmp);
      checkBoxWidth = bmp.bmWidth / 4;
      checkBoxHeight = bmp.bmHeight / 3;
    }
    if (btn_type >= MAX_BTN_TYPE) {
      return -1; /* abort */
    }
    /* XP turns a BS_USERBUTTON into BS_PUSHBUTTON */
    if (btn_type == BS_USERBUTTON) {
      style = (style & ~BS_TYPEMASK) | BS_PUSHBUTTON;
      SetStyle(hWid, style, BS_TYPEMASK & ~style);
    }
    set_button_state(hWid, BST_UNCHECKED);
    return 0;
  case WM_ERASEBKGND:
    if (btn_type == BS_OWNERDRAW) {
      HDC hdc = (HDC)wParam;
      RECT rc;
      HBRUSH hBrush;
      HWID parent = GetParent(hWid);
      if (!parent) {
        parent = hWid;
      }
      hBrush = (HBRUSH)SendMessage(parent, WM_CTLCOLORBTN, (WPARAM)hdc, (LPARAM)hWid);
      if (!hBrush) { /* did the app forget to call defwindowproc ? */
        hBrush = (HBRUSH)DefWidgetProc(parent, WM_CTLCOLORBTN, (WPARAM)hdc, (LPARAM)hWid);
      }
      GetWidgetRect(hWid, &rc);
      FillRect(hdc, &rc, hBrush);
    }
    return 1;
  case WM_PRINTCLIENT:
  case WM_PAINT:
    if (btnPaintFunc[btn_type]) {
      PAINTSTRUCT ps;
      HDC hdc = (HDC)BeginPaint(hWid, &ps);
      int nOldMode = SetBkMode(hdc, OPAQUE);
      (btnPaintFunc[btn_type])(hWid, hdc, ODA_DRAWENTIRE);
      SetBkMode(hdc, nOldMode); /* reset painting mode */
      EndPaint(hWid, &ps) ;
    }
    break;
  case WM_KEYDOWN:
    if (wParam == VK_SPACE) {
      SendMessage(hWid, BM_SETSTATE, TRUE, 0);
      set_button_state(hWid, get_button_state(hWid) | BUTTON_BTNPRESSED);
      WidgetSetCapture(hWid);
    }
    break;
  case WM_LBUTTONDBLCLK:
    if (style & BS_NOTIFY ||
        btn_type == BS_RADIOBUTTON ||
        btn_type == BS_USERBUTTON ||
        btn_type == BS_OWNERDRAW) {
      BUTTON_NOTIFY_PARENT(hWid, BN_DOUBLECLICKED);
      break;
    }
    /* fall through */
  case WM_LBUTTONDOWN:
    WidgetSetCapture(hWid);
    WidgetSetFocus(hWid);
    set_button_state(hWid, get_button_state(hWid) | BUTTON_BTNPRESSED);
    SendMessageW(hWid, BM_SETSTATE, TRUE, 0);
    state = get_button_state(hWid);
    InvalidateRect(hWid, NULL, TRUE);
    break;
  case WM_KEYUP:
    if (wParam != VK_SPACE) {
      break;
    }
    /* fall through */
  case WM_LBUTTONUP:
    state = get_button_state(hWid);
    if (!(state & BUTTON_BTNPRESSED)) {
      break;
    }
    state &= BUTTON_NSTATES;
    set_button_state(hWid, state);
    if (!(state & BST_PUSHED)) {
      ReleaseCapture();
      break;
    }
    WidgetSendMessageW(hWid, BM_SETSTATE, FALSE, 0);
    ReleaseCapture();
    GetWidgetRect(hWid, &rect);
    if (PtInRect(&rect, pt)) {
      state = get_button_state(hWid);
      switch (btn_type) {
      case BS_AUTOCHECKBOX:
        WidgetSendMessageW(hWid, BM_SETCHECK, !(state & BST_CHECKED), 0);
        break;
      case BS_AUTORADIOBUTTON:
        WidgetSendMessageW(hWid, BM_SETCHECK, TRUE, 0);
        break;
      case BS_AUTO3STATE:
        WidgetSendMessageW(hWid, BM_SETCHECK, (state & BST_INDETERMINATE) ? 0 : ((state & 3) + 1), 0);
        break;
      }
      BUTTON_NOTIFY_PARENT(hWid, BN_CLICKED);
    }
    break;
  case WM_CAPTURECHANGED:
    if (0) {
      TRACE("WM_CAPTURECHANGED %p\n", hWid);
      state = get_button_state(hWid);
      if (state & BUTTON_BTNPRESSED) {
        state &= BUTTON_NSTATES;
        set_button_state(hWid, state);
        if (state & BST_PUSHED) {
          WidgetSendMessageW(hWid, BM_SETSTATE, FALSE, 0);
        }
      }
    }
    break;
  case WM_MOUSEMOVE:
    if ((wParam & MK_LBUTTON) && WidgetGetCapture() == hWid) {
      //GetWidgetRect( hWid, &rect );
      //SendMessageW( hWid, BM_SETSTATE, PtInRect(&rect, pt), 0 );
    }
    break;
  case WM_SETTEXT: {
    /* Clear an old text here as Widgets does */
    HDC hdc = GetDC(hWid);
    HBRUSH hbrush;
    RECT client, rc;
    HWID parent = GetParent(hWid);
    if (!parent) {
      parent = hWid;
    }
    hbrush = (HBRUSH)WidgetSendMessageW(parent, WM_CTLCOLORSTATIC, (WPARAM)hdc, (LPARAM)hWid);
    if (!hbrush) { /* did the app forget to call DefWidgetProc ? */
      hbrush = (HBRUSH)DefWidgetProcW(parent, WM_CTLCOLORSTATIC, (WPARAM)hdc, (LPARAM)hWid);
    }
    GetWidgetRect(hWid, &client);
    rc = client;
    BUTTON_CalcLabelRect(hWid, hdc, &rc);
    /* Clip by client rect bounds */
    if (rc.right > client.right) {
      rc.right = client.right;
    }
    if (rc.bottom > client.bottom) {
      rc.bottom = client.bottom;
    }
    FillRect(hdc, &rc, hbrush);
    ReleaseDC(hWid, hdc);
    if (unicode) {
      DefWidgetProcW(hWid, WM_SETTEXT, wParam, lParam);
    }
    else {
      DefWidgetProcA(hWid, WM_SETTEXT, wParam, lParam);
    }
    InvalidateRect(hWid, NULL, TRUE);
    return 1; /* success. FIXME: check text length */
  }
  case WM_SETFONT:
    set_button_font(hWid, (HFONT)wParam);
    if (lParam) {
      InvalidateRect(hWid, NULL, TRUE);
    }
    break;
  case WM_GETFONT:
    return (LRESULT)get_button_font(hWid);
  case WM_SETFOCUS:
    TRACE("WM_SETFOCUS %p\n", hWid);
    set_button_state(hWid, get_button_state(hWid) | BST_FOCUS);
    paint_button(hWid, btn_type, ODA_FOCUS);
    //InvalidateRect( hWid, TRUE );
    if (style & BS_NOTIFY) {
      BUTTON_NOTIFY_PARENT(hWid, BN_SETFOCUS);
    }
    break;
  case WM_KILLFOCUS:
    TRACE("WM_KILLFOCUS %p\n", hWid);
    state = get_button_state(hWid);
    set_button_state(hWid, state & ~BST_FOCUS);
    paint_button(hWid, btn_type, ODA_FOCUS);
    //InvalidateRect( hWid, TRUE );
    if ((state & BUTTON_BTNPRESSED) && WidgetGetCapture() == hWid) {
      ReleaseCapture();
    }
    if (style & BS_NOTIFY) {
      BUTTON_NOTIFY_PARENT(hWid, BN_KILLFOCUS);
    }
    InvalidateRect(hWid, NULL, FALSE);
    break;
  case WM_SYSCOLORCHANGE:
    InvalidateRect(hWid, NULL, FALSE);
    break;
  case BM_SETSTYLE:
    if ((wParam & BS_TYPEMASK) >= MAX_BTN_TYPE) {
      break;
    }
    btn_type = wParam & BS_TYPEMASK;
    style = (style & ~BS_TYPEMASK) | btn_type;
    SetStyle(hWid, style, BS_TYPEMASK & ~style);
    /* Only redraw if lParam flag is set.*/
    if (lParam) {
      InvalidateRect(hWid, NULL, TRUE);
    }
    break;
  case BM_CLICK:
    WidgetSendMessageW(hWid, WM_LBUTTONDOWN, 0, 0);
    WidgetSendMessageW(hWid, WM_LBUTTONUP, 0, 0);
    break;
  case BM_SETIMAGE:
    /* Check that image format matches button style */
    switch (style & (BS_BITMAP | BS_ICON)) {
    case BS_BITMAP:
      if (wParam != IMAGE_BITMAP) {
        return 0;
      }
      break;
    case BS_ICON:
      if (wParam != IMAGE_ICON) {
        return 0;
      }
      break;
    default:
      return 0;
    }
    oldHbitmap = (HBITMAP)SetWidgetLongPtrW(hWid, HIMAGE_GWLBN_OFFSET, lParam);
    InvalidateRect(hWid, NULL, FALSE);
    return (LRESULT)oldHbitmap;
  case BM_GETIMAGE:
    return GetWidgetLongPtrW(hWid, HIMAGE_GWLBN_OFFSET);
  case BM_GETCHECK:
    return get_button_state(hWid) & 3;
  case BM_SETCHECK:
    if (wParam > maxCheckState[btn_type]) {
      wParam = maxCheckState[btn_type];
    }
    state = get_button_state(hWid);
    if ((btn_type == BS_RADIOBUTTON) || (btn_type == BS_AUTORADIOBUTTON)) {
      if (wParam) {
        SetStyle(hWid, WS_TABSTOP, 0);
      }
      else {
        SetStyle(hWid, 0, WS_TABSTOP);
      }
    }
    if ((state & 3) != wParam) {
      set_button_state(hWid, (state & ~3) | wParam);
      paint_button(hWid, btn_type, ODA_SELECT);
    }
    if ((btn_type == BS_AUTORADIOBUTTON) && (wParam == BST_CHECKED) && (style & WS_CHILD)) {
      BUTTON_CheckAutoRadioButton(hWid);
    }
    break;
  case BM_GETSTATE:
    return get_button_state(hWid);
  case BM_SETSTATE:
    state = get_button_state(hWid);
    if ((!(state & BST_PUSHED)) != (!wParam)) {
      if (wParam) {
        set_button_state(hWid, state | BST_PUSHED);
      }
      else {
        set_button_state(hWid, state & ~BST_PUSHED);
      }
      paint_button(hWid, btn_type, ODA_SELECT);
    }
    break;
  case WM_NCHITTEST:
    if (btn_type == BS_GROUPBOX) {
      return HTTRANSPARENT;
    }
    /* fall through */
  default:
    return DefWidgetProc(hWid, uMsg, wParam, lParam);
  }
  return 0;
}
/**********************************************************************
 * Convert button styles to flags used by DrawText.
 */
static UINT BUTTON_BStoDT(DWORD style, DWORD ex_style)
{
  UINT dtStyle = DT_NOCLIP; /* We use SelectClipRgn to limit output */
  /* "Convert" pushlike buttons to pushbuttons */
  if (style & BS_PUSHLIKE) {
    style &= ~BS_TYPEMASK;
  }
  if (!(style & BS_MULTILINE)) {
    dtStyle |= DT_SINGLELINE;
  }
  else {
    dtStyle |= DT_WORDBREAK;
  }
  switch (style & BS_CENTER) {
  case BS_LEFT: /* DT_LEFT is 0 */
    break;
  case BS_RIGHT:
    dtStyle |= DT_RIGHT;
    break;
  case BS_CENTER:
    dtStyle |= DT_CENTER;
    break;
  default:
    /* Pushbutton's text is centered by default */
    if (get_button_type(style) <= BS_DEFPUSHBUTTON) {
      dtStyle |= DT_CENTER;
    }
    /* all other flavours have left aligned text */
  }
  if (ex_style & WS_EX_RIGHT) {
    dtStyle = DT_RIGHT | (dtStyle & ~(DT_LEFT | DT_CENTER));
  }
  /* DrawText ignores vertical alignment for multiline text,
  * but we use these flags to align label manually.
  */
  if (get_button_type(style) != BS_GROUPBOX) {
    switch (style & BS_VCENTER) {
    case BS_TOP: /* DT_TOP is 0 */
      break;
    case BS_BOTTOM:
      dtStyle |= DT_BOTTOM;
      break;
    case BS_VCENTER: /* fall through */
    default:
      dtStyle |= DT_VCENTER;
      break;
    }
  }
  else
    /* GroupBox's text is always single line and is top aligned. */
  {
    dtStyle |= DT_SINGLELINE;
  }
  return dtStyle;
}
/**********************************************************************
 * BUTTON_CalcLabelRect
 *
 * Calculates label's rectangle depending on button style.
 *
 * Returns flags to be passed to DrawText.
 * Calculated rectangle doesn't take into account button state
 * (pushed, etc.). If there is nothing to draw (no text/image) output
 * rectangle is empty, and return value is (UINT)-1.
 */
static UINT BUTTON_CalcLabelRect(HWID hWid, HDC hdc, RECT* rc)
{
  LONG style = GetWidgetLong(hWid, GWL_STYLE);
  LONG ex_style = GetWidgetLong(hWid, GWL_EXSTYLE);
  char* text;
  ICONINFO iconInfo;
  BITMAP bm;
  UINT dtStyle = BUTTON_BStoDT(style, ex_style);
  RECT r = *rc;
  INT n;
  /* Calculate label rectangle according to label type */
  switch (style & (BS_ICON | BS_BITMAP)) {
  case BS_TEXT:
    if (!(text = get_button_text(hWid))) {
      goto empty_rect;
    }
    if (!text[0]) {
      HeapFree(GetProcessHeap(), 0, text);
      goto empty_rect;
    }
    DrawTextA(hdc, text, -1, &r, dtStyle | DT_CALCRECT);
    HeapFree(GetProcessHeap(), 0, text);
    break;
  case BS_ICON:
    if (!GetIconInfo((HICON)GetWidgetLongPtrW(hWid, HIMAGE_GWLBN_OFFSET), &iconInfo)) {
      goto empty_rect;
    }
    GetObjectW(iconInfo.hbmColor, sizeof(BITMAP), &bm);
    r.right = r.left + bm.bmWidth;
    r.bottom = r.top + bm.bmHeight;
    DeleteObject(iconInfo.hbmColor);
    DeleteObject(iconInfo.hbmMask);
    break;
  case BS_BITMAP:
    if (!GetObjectW((HANDLE)GetWidgetLongPtrW(hWid, HIMAGE_GWLBN_OFFSET), sizeof(BITMAP), &bm)) {
      goto empty_rect;
    }
    r.right = r.left + bm.bmWidth;
    r.bottom = r.top + bm.bmHeight;
    break;
  default:
empty_rect:
    rc->right = r.left;
    rc->bottom = r.top;
    return (UINT) - 1;
  }
  /* Position label inside bounding rectangle according to
  * alignment flags. (calculated rect is always left-top aligned).
  * If label is aligned to any side - shift label in opposite
  * direction to leave extra space for focus rectangle.
  */
  switch (dtStyle & (DT_CENTER | DT_RIGHT)) {
  case DT_LEFT:
    r.left++;
    r.right++;
    break;
  case DT_CENTER:
    n = r.right - r.left;
    r.left = rc->left + ((rc->right - rc->left) - n) / 2;
    r.right = r.left + n;
    break;
  case DT_RIGHT:
    n = r.right - r.left;
    r.right = rc->right - 1;
    r.left = r.right - n;
    break;
  }
  switch (dtStyle & (DT_VCENTER | DT_BOTTOM)) {
  case DT_TOP:
    r.top++;
    r.bottom++;
    break;
  case DT_VCENTER:
    n = r.bottom - r.top;
    r.top = rc->top + ((rc->bottom - rc->top) - n) / 2;
    r.bottom = r.top + n;
    break;
  case DT_BOTTOM:
    n = r.bottom - r.top;
    r.bottom = rc->bottom - 1;
    r.top = r.bottom - n;
    break;
  }
  *rc = r;
  return dtStyle;
}
/**********************************************************************
 * BUTTON_DrawTextCallback
 *
 * Callback function used by DrawStateW function.
 */
static BOOL CALLBACK BUTTON_DrawTextCallback(HDC hdc, LPARAM lp, WPARAM wp, int cx, int cy)
{
  RECT rc;
  rc.left = 0;
  rc.top = 0;
  rc.right = cx;
  rc.bottom = cy;
  DrawTextA(hdc, (LPCSTR)lp, -1, &rc, (UINT)wp);
  return TRUE;
}
/**********************************************************************
 * BUTTON_DrawLabel
 *
 * Common function for drawing button label.
 */
static void BUTTON_DrawLabel(HWID hWid, HDC hdc, UINT dtFlags, const RECT* rc)
{
  DRAWSTATEPROC lpOutputProc = NULL;
  LPARAM lp;
  WPARAM wp = 0;
  HBRUSH hbr = 0;
  UINT flags = IsWidgetEnabled(hWid) ? DSS_NORMAL : DSS_DISABLED;
  LONG state = get_button_state(hWid);
  LONG style = GetWidgetLong(hWid, GWL_STYLE);
  char* text = NULL;
  /* FIXME: To draw disabled label in Win31 look-and-feel, we probably
  * must use DSS_MONO flag and COLOR_GRAYTEXT brush (or maybe DSS_UNION).
  * I don't have Win31 on hand to verify that, so I leave it as is.
  */
  if ((style & BS_PUSHLIKE) && (state & BST_INDETERMINATE)) {
    hbr = GetSysColorBrush(COLOR_GRAYTEXT);
    flags |= DSS_MONO;
  }
  switch (style & (BS_ICON | BS_BITMAP)) {
  case BS_TEXT:
    /* DST_COMPLEX -- is 0 */
    lpOutputProc = BUTTON_DrawTextCallback;
    if (!(text = get_button_text(hWid))) {
      return;
    }
    lp = (LPARAM)text;
    wp = dtFlags;
    break;
  case BS_ICON:
    flags |= DST_ICON;
    lp = GetWidgetLongPtrW(hWid, HIMAGE_GWLBN_OFFSET);
    break;
  case BS_BITMAP:
    flags |= DST_BITMAP;
    lp = GetWidgetLongPtrW(hWid, HIMAGE_GWLBN_OFFSET);
    break;
  default:
    return;
  }
  DrawStateA(hdc, hbr, lpOutputProc, lp, wp, rc->left, rc->top,
      rc->right - rc->left, rc->bottom - rc->top, flags);
  HeapFree(GetProcessHeap(), 0, text);
}
/**********************************************************************
 * Push Button Functions
 */
static void PB_Paint(HWID hWid, HDC hDC, DWORD action)
{
  RECT rc, r;
  UINT dtFlags, uState;
  HPEN hOldPen;
  HBRUSH hOldBrush;
  INT oldBkMode;
  COLORREF oldTxtColor;
  HFONT hFont;
  LONG state = get_button_state(hWid);
  LONG style = GetWidgetLong(hWid, GWL_STYLE);
  BOOL pushedState = (state & BST_PUSHED);
  HWID parent;
  HRGN hrgn;
  //printf("PB_Paint state=%08x\n", state);
  GetWidgetRect(hWid, &rc);
  /* Send WM_CTLCOLOR to allow changing the font (the colors are fixed) */
  if ((hFont = get_button_font(hWid))) {
    SelectObject(hDC, hFont);
  }
  parent = GetParent(hWid);
  if (!parent) {
    parent = hWid;
  }
  WidgetSendMessageW(parent, WM_CTLCOLORBTN, (WPARAM)hDC, (LPARAM)hWid);
  hrgn = set_control_clipping(hDC, &rc);
  hOldPen = (HPEN)SelectObject(hDC, SYSCOLOR_GetPen(COLOR_WINDOWFRAME));
  hOldBrush = (HBRUSH)SelectObject(hDC, GetSysColorBrush(COLOR_BTNFACE));
  oldBkMode = SetBkMode(hDC, TRANSPARENT);
  if (get_button_type(style) == BS_DEFPUSHBUTTON) {
    if (action != ODA_FOCUS) {
      Rectangle(hDC, rc.left, rc.top, rc.right, rc.bottom);
    }
    InflateRect(&rc, -1, -1);
  }
  /* completely skip the drawing if only focus has changed */
  if (action == ODA_FOCUS) {
    goto draw_focus;
  }
  uState = DFCS_BUTTONPUSH;
  if (style & BS_FLAT) {
    uState |= DFCS_MONO;
  }
  else if (pushedState) {
    if (get_button_type(style) == BS_DEFPUSHBUTTON) {
      uState |= DFCS_FLAT;
    }
    else {
      uState |= DFCS_PUSHED;
    }
  }
  if (state & (BST_CHECKED | BST_INDETERMINATE)) {
    uState |= DFCS_CHECKED;
  }
  DrawFrameControl(hDC, &rc, DFC_BUTTON, uState);
  /* draw button label */
  r = rc;
  dtFlags = BUTTON_CalcLabelRect(hWid, hDC, &r);
  if (dtFlags == (UINT) - 1L) {
    goto cleanup;
  }
  if (pushedState) {
    OffsetRect(&r, 1, 1);
  }
  oldTxtColor = SetTextColor(hDC, GetSysColor(COLOR_BTNTEXT));
  BUTTON_DrawLabel(hWid, hDC, dtFlags, &r);
  SetTextColor(hDC, oldTxtColor);
draw_focus:
  if (action == ODA_FOCUS || (state & BST_FOCUS)) {
    InflateRect(&rc, -2, -2);
    DrawFocusRect(hDC, &rc);
  }
cleanup:
  SelectObject(hDC, hOldPen);
  SelectObject(hDC, hOldBrush);
  SetBkMode(hDC, oldBkMode);
  SelectClipRgn(hDC, hrgn);
  if (hrgn) {
    DeleteObject(hrgn);
  }
}
/**********************************************************************
 * Check Box & Radio Button Functions
 */
static void CB_Paint(HWID hWid, HDC hDC, DWORD action)
{
  RECT rbox, rtext, client;
  HBRUSH hBrush;
  int delta;
  UINT dtFlags;
  HFONT hFont;
  LONG state = get_button_state(hWid);
  LONG style = GetWidgetLong(hWid, GWL_STYLE);
  HWID parent;
  HRGN hrgn;
  if (style & BS_PUSHLIKE) {
    PB_Paint(hWid, hDC, action);
    return;
  }
  GetWidgetRect(hWid, &client);
  rbox = rtext = client;
  if ((hFont = get_button_font(hWid))) {
    SelectObject(hDC, hFont);
  }
  parent = GetParent(hWid);
  if (!parent) {
    parent = hWid;
  }
  hBrush = (HBRUSH)WidgetSendMessageW(parent, WM_CTLCOLORSTATIC,
      (WPARAM)hDC, (LPARAM)hWid);
  if (!hBrush) /* did the app forget to call defwindowproc ? */
    hBrush = (HBRUSH)DefWidgetProcW(parent, WM_CTLCOLORSTATIC,
        (WPARAM)hDC, (LPARAM)hWid);
  hrgn = set_control_clipping(hDC, &client);
  if (style & BS_LEFTTEXT) {
    /* magic +4 is what CTL3D expects */
    rtext.right -= checkBoxWidth + 4;
    rbox.left = rbox.right - checkBoxWidth;
  }
  else {
    rtext.left += checkBoxWidth + 4;
    rbox.right = rbox.left + checkBoxWidth;
  }
  /* Since WM_ERASEBKGND does nothing, first prepare background */
  if (action == ODA_SELECT) {
    FillRect(hDC, &rbox, hBrush);
  }
  if (action == ODA_DRAWENTIRE) {
    FillRect(hDC, &client, hBrush);
  }
  /* Draw label */
  client = rtext;
  dtFlags = BUTTON_CalcLabelRect(hWid, hDC, &rtext);
  /* Only adjust rbox when rtext is valid */
  if (dtFlags != (UINT) - 1L) {
    rbox.top = rtext.top;
    rbox.bottom = rtext.bottom;
  }
  /* Draw the check-box bitmap */
  if (action == ODA_DRAWENTIRE || action == ODA_SELECT) {
    UINT flags;
    if ((get_button_type(style) == BS_RADIOBUTTON) ||
        (get_button_type(style) == BS_AUTORADIOBUTTON)) {
      flags = DFCS_BUTTONRADIO;
    }
    else if (state & BST_INDETERMINATE) {
      flags = DFCS_BUTTON3STATE;
    }
    else {
      flags = DFCS_BUTTONCHECK;
    }
    if (state & (BST_CHECKED | BST_INDETERMINATE)) {
      flags |= DFCS_CHECKED;
    }
    if (state & BST_PUSHED) {
      flags |= DFCS_PUSHED;
    }
    if (style & WS_DISABLED) {
      flags |= DFCS_INACTIVE;
    }
    /* rbox must have the correct height */
    delta = rbox.bottom - rbox.top - checkBoxHeight;
    if (style & BS_TOP) {
      if (delta > 0) {
        rbox.bottom = rbox.top + checkBoxHeight;
      }
      else {
        rbox.top -= -delta / 2 + 1;
        rbox.bottom = rbox.top + checkBoxHeight;
      }
    }
    else if (style & BS_BOTTOM) {
      if (delta > 0) {
        rbox.top = rbox.bottom - checkBoxHeight;
      }
      else {
        rbox.bottom += -delta / 2 + 1;
        rbox.top = rbox.bottom - checkBoxHeight;
      }
    }
    else { /* Default */
      if (delta > 0) {
        int ofs = (delta / 2);
        rbox.bottom -= ofs + 1;
        rbox.top = rbox.bottom - checkBoxHeight;
      }
      else if (delta < 0) {
        int ofs = (-delta / 2);
        rbox.top -= ofs + 1;
        rbox.bottom = rbox.top + checkBoxHeight;
      }
    }
    DrawFrameControl(hDC, &rbox, DFC_BUTTON, flags);
  }
  if (dtFlags == (UINT) - 1L) { /* Noting to draw */
    return;
  }
  if (action == ODA_DRAWENTIRE) {
    BUTTON_DrawLabel(hWid, hDC, dtFlags, &rtext);
  }
  /* ... and focus */
  if (action == ODA_FOCUS || (state & BST_FOCUS)) {
    rtext.left--;
    rtext.right++;
    IntersectRect(&rtext, &rtext, &client);
    DrawFocusRect(hDC, &rtext);
  }
  SelectClipRgn(hDC, hrgn);
  if (hrgn) {
    DeleteObject(hrgn);
  }
}
/**********************************************************************
 * BUTTON_CheckAutoRadioButton
 *
 * hWid is checked, uncheck every other auto radio button in group
 */
static void BUTTON_CheckAutoRadioButton(HWID hWid)
{
  HWID parent, sibling, start;
  parent = GetParent(hWid);
  /* make sure that starting control is not disabled or invisible */
  start = sibling = GetNextDlgGroupItem(parent, hWid, TRUE);
  do {
    if (!sibling) {
      break;
    }
    if ((hWid != sibling) &&
        ((GetWidgetLong(sibling, GWL_STYLE) & BS_TYPEMASK) == BS_AUTORADIOBUTTON)) {
      WidgetSendMessageW(sibling, BM_SETCHECK, BST_UNCHECKED, 0);
    }
    sibling = GetNextDlgGroupItem(parent, sibling, FALSE);
  }
  while (sibling != start);
}
/**********************************************************************
 * Group Box Functions
 */
static void GB_Paint(HWID hWid, HDC hDC, DWORD action)
{
  RECT rc, rcFrame;
  HBRUSH hbr;
  HFONT hFont;
  UINT dtFlags;
  TEXTMETRICW tm;
  LONG style = GetWidgetLong(hWid, GWL_STYLE);
  HWID parent;
  HRGN hrgn;
  if ((hFont = get_button_font(hWid))) {
    SelectObject(hDC, hFont);
  }
  /* GroupBox acts like static control, so it sends CTLCOLORSTATIC */
  parent = GetParent(hWid);
  if (!parent) {
    parent = hWid;
  }
  hbr = (HBRUSH)WidgetSendMessageW(parent, WM_CTLCOLORSTATIC, (WPARAM)hDC, (LPARAM)hWid);
  if (!hbr) /* did the app forget to call defwindowproc ? */
    hbr = (HBRUSH)DefWidgetProcW(parent, WM_CTLCOLORSTATIC,
        (WPARAM)hDC, (LPARAM)hWid);
  GetWidgetRect(hWid, &rc);
  rcFrame = rc;
  hrgn = set_control_clipping(hDC, &rc);
  GetTextMetricsW(hDC, &tm);
  rcFrame.top += (tm.tmHeight / 2) - 1;
  DrawEdge(hDC, &rcFrame, EDGE_ETCHED, BF_RECT | ((style & BS_FLAT) ? BF_FLAT : 0));
  InflateRect(&rc, -7, 1);
  dtFlags = BUTTON_CalcLabelRect(hWid, hDC, &rc);
  if (dtFlags != (UINT) - 1) {
    /* Because buttons have CS_PARENTDC class style, there is a chance
    * that label will be drawn out of client rect.
    * But Widgets doesn't clip label's rect, so do I.
    */
    /* There is 1-pixel margin at the left, right, and bottom */
    rc.left--;
    rc.right++;
    rc.bottom++;
    FillRect(hDC, &rc, hbr);
    rc.left++;
    rc.right--;
    rc.bottom--;
    BUTTON_DrawLabel(hWid, hDC, dtFlags, &rc);
  }
  SelectClipRgn(hDC, hrgn);
  if (hrgn) {
    DeleteObject(hrgn);
  }
}
/**********************************************************************
 * User Button Functions
 */
static void UB_Paint(HWID hWid, HDC hDC, DWORD action)
{
  RECT rc;
  HBRUSH hBrush;
  HFONT hFont;
  LONG state = get_button_state(hWid);
  HWID parent;
  GetWidgetRect(hWid, &rc);
  if ((hFont = get_button_font(hWid))) {
    SelectObject(hDC, hFont);
  }
  parent = GetParent(hWid);
  if (!parent) {
    parent = hWid;
  }
  hBrush = (HBRUSH)WidgetSendMessageW(parent, WM_CTLCOLORBTN, (WPARAM)hDC, (LPARAM)hWid);
  if (!hBrush) /* did the app forget to call defwindowproc ? */
    hBrush = (HBRUSH)DefWidgetProcW(parent, WM_CTLCOLORBTN,
        (WPARAM)hDC, (LPARAM)hWid);
  FillRect(hDC, &rc, hBrush);
  if (action == ODA_FOCUS || (state & BST_FOCUS)) {
    DrawFocusRect(hDC, &rc);
  }
  switch (action) {
  case ODA_FOCUS:
    BUTTON_NOTIFY_PARENT(hWid, (state & BST_FOCUS) ? BN_SETFOCUS : BN_KILLFOCUS);
    break;
  case ODA_SELECT:
    BUTTON_NOTIFY_PARENT(hWid, (state & BST_PUSHED) ? BN_HILITE : BN_UNHILITE);
    break;
  default:
    BUTTON_NOTIFY_PARENT(hWid, BN_PAINT);
    break;
  }
}
/**********************************************************************
 * Ownerdrawn Button Functions
 */
static void OB_Paint(HWID hWid, HDC hDC, DWORD action)
{
  LONG state = get_button_state(hWid);
  DRAWITEMSTRUCT dis;
  LONG_PTR id = GetWidgetLongPtrW(hWid, GWLP_ID);
  HWID parent;
  HFONT hFont, hPrevFont = 0;
  HRGN hrgn;
  dis.CtlType = ODT_BUTTON;
  dis.CtlID = id;
  dis.itemID = 0;
  dis.itemAction = action;
  dis.itemState = ((state & BST_FOCUS) ? ODS_FOCUS : 0) |
      ((state & BST_PUSHED) ? ODS_SELECTED : 0) |
      (IsWidgetEnabled(hWid) ? 0 : ODS_DISABLED);
  //dis.hwndItem = hWid;
  dis.hDC = hDC;
  dis.itemData = 0;
  GetWidgetRect(hWid, &dis.rcItem);
  if ((hFont = get_button_font(hWid))) {
    hPrevFont = (HFONT)SelectObject(hDC, hFont);
  }
  parent = GetParent(hWid);
  if (!parent) {
    parent = hWid;
  }
  WidgetSendMessageW(parent, WM_CTLCOLORBTN, (WPARAM)hDC, (LPARAM)hWid);
  hrgn = set_control_clipping(hDC, &dis.rcItem);
  WidgetSendMessageW(GetParent(hWid), WM_DRAWITEM, id, (LPARAM)&dis);
  if (hPrevFont) {
    SelectObject(hDC, hPrevFont);
  }
  SelectClipRgn(hDC, hrgn);
  if (hrgn) {
    DeleteObject(hrgn);
  }
}

