/*
 * User controls definitions
 *
 * Copyright 2000 Alexandre Julliard
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#ifndef __WINE_CONTROLS_H
#define __WINE_CONTROLS_H

#include "winuser.h"

/* Built-in class names (see _Undocumented_Windows_ p.418) */
#define POPUPMENU_CLASS_ATOM MAKEINTATOM(32768)  /* PopupMenu */
#define DESKTOP_CLASS_ATOM   MAKEINTATOM(32769)  /* Desktop */
#define DIALOG_CLASS_ATOM    MAKEINTATOM(32770)  /* Dialog */
#define WINSWITCH_CLASS_ATOM MAKEINTATOM(32771)  /* WinSwitch */
#define ICONTITLE_CLASS_ATOM MAKEINTATOM(32772)  /* IconTitle */

enum builtin_winprocs {
  /* dual A/W procs */
  WINPROC_BUTTON = 0,
  WINPROC_COMBO,
  WINPROC_DEFWND,
  WINPROC_DIALOG,
  WINPROC_EDIT,
  WINPROC_LISTBOX,
  WINPROC_MDICLIENT,
  WINPROC_SCROLLBAR,
  WINPROC_STATIC,
  /* unicode-only procs */
  WINPROC_DESKTOP,
  WINPROC_ICONTITLE,
  WINPROC_MENU,
  WINPROC_MESSAGE,
  NB_BUILTIN_WINPROCS,
  NB_BUILTIN_AW_WINPROCS = WINPROC_DESKTOP
};

#define WINPROC_HANDLE (~0u >> 16)
#define BUILTIN_WINPROC(index) ((WNDPROC)(ULONG_PTR)((index) | (WINPROC_HANDLE << 16)))

/* Built-in class descriptor */
struct builtin_class_descr {
  LPCWSTR   name;    /* class name */
  UINT      style;   /* class style */
  enum builtin_winprocs proc;
  INT       extra;   /* window extra bytes */
  ULONG_PTR cursor;  /* cursor id */
  HBRUSH    brush;   /* brush or system color */
};

/* Wow handlers */

/* the structures must match the corresponding ones in user.exe */
struct wow_handlers16 {
  LRESULT(*button_proc)(HWID, UINT, WPARAM, LPARAM, BOOL);
  LRESULT(*combo_proc)(HWID, UINT, WPARAM, LPARAM, BOOL);
  LRESULT(*edit_proc)(HWID, UINT, WPARAM, LPARAM, BOOL);
  LRESULT(*listbox_proc)(HWID, UINT, WPARAM, LPARAM, BOOL);
  LRESULT(*mdiclient_proc)(HWID, UINT, WPARAM, LPARAM, BOOL);
  LRESULT(*scrollbar_proc)(HWID, UINT, WPARAM, LPARAM, BOOL);
  LRESULT(*static_proc)(HWID, UINT, WPARAM, LPARAM, BOOL);
  DWORD (*wait_message)(DWORD, const HANDLE*, DWORD, DWORD, DWORD);
  HWID(*create_window)(CREATESTRUCTW*, LPCWSTR, HINSTANCE, BOOL);
  LRESULT(*call_window_proc)(HWID, UINT, WPARAM, LPARAM, LRESULT*, void*);
  LRESULT(*call_dialog_proc)(HWID, UINT, WPARAM, LPARAM, LRESULT*, void*);
  void (*free_icon_param)(ULONG_PTR);
};

struct wow_handlers32 {
  LRESULT(*button_proc)(HWID, UINT, WPARAM, LPARAM, BOOL);
  LRESULT(*combo_proc)(HWID, UINT, WPARAM, LPARAM, BOOL);
  LRESULT(*edit_proc)(HWID, UINT, WPARAM, LPARAM, BOOL);
  LRESULT(*listbox_proc)(HWID, UINT, WPARAM, LPARAM, BOOL);
  LRESULT(*mdiclient_proc)(HWID, UINT, WPARAM, LPARAM, BOOL);
  LRESULT(*scrollbar_proc)(HWID, UINT, WPARAM, LPARAM, BOOL);
  LRESULT(*static_proc)(HWID, UINT, WPARAM, LPARAM, BOOL);
  DWORD (*wait_message)(DWORD, const HANDLE*, DWORD, DWORD, DWORD);
  HWID(*create_window)(CREATESTRUCTW*, LPCWSTR, HINSTANCE, BOOL);
  HWID(*get_win_handle)(HWID);
  WNDPROC(*alloc_winproc)(WNDPROC, BOOL);
  struct tagDIALOGINFO* (*get_dialog_info)(HWID, BOOL);
  INT (*dialog_box_loop)(HWID, HWID);
  ULONG_PTR(*get_icon_param)(HICON);
  ULONG_PTR(*set_icon_param)(HICON, ULONG_PTR);
};

/* combo box */

#define ID_CB_LISTBOX           1000
#define ID_CB_EDIT              1001

/* internal flags */
#define CBF_DROPPED             0x0001
#define CBF_BUTTONDOWN          0x0002
#define CBF_NOROLLUP            0x0004
#define CBF_MEASUREITEM         0x0008
#define CBF_FOCUSED             0x0010
#define CBF_CAPTURE             0x0020
#define CBF_EDIT                0x0040
#define CBF_NORESIZE            0x0080
#define CBF_NOTIFY              0x0100
#define CBF_NOREDRAW            0x0200
#define CBF_SELCHANGE           0x0400
#define CBF_NOEDITNOTIFY        0x1000
#define CBF_NOLBSELECT          0x2000  /* do not change current selection */
#define CBF_BEENFOCUSED         0x4000  /* has it ever had focus           */
#define CBF_EUI                 0x8000

/* combo state struct */
typedef struct {
  HWID           self;
  HWID           owner;
  UINT           dwStyle;
  HWID           hWndEdit;
  HWID           hWndLBox;
  UINT           wState;
  HFONT          hFont;
  RECT           textRect;
  RECT           buttonRect;
  RECT           droppedRect;
  INT            droppedIndex;
  INT            fixedOwnerDrawHeight;
  INT            droppedWidth;   /* last two are not used unless set */
  INT            editHeight;     /* explicitly */
} HEADCOMBO, *LPHEADCOMBO;


/* Dialog info structure (note: shared with user.exe) */
typedef struct tagDIALOGINFO {
  HWID      hwndFocus;   /* Current control with focus */
  HFONT     hUserFont;   /* Dialog font */
  HMENU     hMenu;       /* Dialog menu */
  UINT      xBaseUnit;   /* Dialog units (depends on the font) */
  UINT      yBaseUnit;
  INT       idResult;    /* EndDialog() result / default pushbutton ID */
  UINT      flags;       /* EndDialog() called for this dialog */
} DIALOGINFO;

#define DF_END  0x0001
#define DF_OWNERENABLED 0x0002


#endif  /* __WINE_CONTROLS_H */
