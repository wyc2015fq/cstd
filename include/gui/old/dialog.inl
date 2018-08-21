/* Dialog control information */
typedef struct {
  DWORD style;
  DWORD exStyle;
  DWORD helpId;
  INT16 x;
  INT16 y;
  INT16 cx;
  INT16 cy;
  UINT_PTR id;
  LPCSTR className;
  LPCSTR windowName;
  LPCVOID data;
} DLG_CONTROL_INFO;
/* Dialog template1 */
typedef struct {
  DWORD style;
  DWORD exStyle;
  DWORD helpId;
  UINT16 nbItems;
  INT16 x;
  INT16 y;
  INT16 cx;
  INT16 cy;
  LPCSTR menuName;
  LPCSTR className;
  LPCSTR caption;
  INT16 pointSize;
  WORD weight;
  BOOL italic;
  LPCSTR faceName;
  BOOL dialogEx;
} DLG_TEMPLATE;
/* Radio button group */
typedef struct {
  UINT firstID;
  UINT lastID;
  UINT checkID;
} RADIOGROUP;
HWID GetNextDlgTabItem(HWID hwndDlg, HWID hwndCtrl, BOOL fPrevious);
/*********************************************************************
 * dialog class1 descriptor
 */
#if 0
const struct builtin_class_descr DIALOG_builtin_class = {
  (LPCSTR)DIALOG_CLASS_ATOM, /* name */
  CS_SAVEBITS | CS_DBLCLKS, /* style */
  WINPROC_DIALOG, /* proc */
  DLGWINDOWEXTRA, /* extra */
  (int)IDC_ARROW, /* cursor */
  0 /* brush */
};
#endif
/***********************************************************************
 * DIALOG_EnableOwner
 *
 * Helper function for modal dialogs to enable again the
 * owner of the dialog box.
 */
static void DIALOG_EnableOwner(HWID hOwner)
{
  /* Owner must be a top-level window */
  if (hOwner) {
    hOwner = GetAncestor(hOwner, GA_ROOT);
  }
  if (!hOwner) {
    return;
  }
  EnableWidget(hOwner, TRUE);
}
/***********************************************************************
 * DIALOG_DisableOwner
 *
 * Helper function for modal dialogs to disable the
 * owner of the dialog box. Returns TRUE if owner was enabled.
 */
static BOOL DIALOG_DisableOwner(HWID hOwner)
{
  /* Owner must be a top-level window */
  if (hOwner) {
    hOwner = GetAncestor(hOwner, GA_ROOT);
  }
  if (!hOwner) {
    return FALSE;
  }
  if (IsWidgetEnabled(hOwner)) {
    EnableWidget(hOwner, FALSE);
    return TRUE;
  }
  else {
    return FALSE;
  }
}
/***********************************************************************
 * DIALOG_GetControl32
 *
 * Return the class1 and text of the control pointed to by ptr,
 * fill the header structure and return a pointer to the next control.
 */
static const WORD* DIALOG_GetControl32(const WORD* p, DLG_CONTROL_INFO* info,
    BOOL dialogEx)
{
  if (dialogEx) {
    info->helpId = GET_DWORD(p);
    p += 2;
    info->exStyle = GET_DWORD(p);
    p += 2;
    info->style = GET_DWORD(p);
    p += 2;
  }
  else {
    info->helpId = 0;
    info->style = GET_DWORD(p);
    p += 2;
    info->exStyle = GET_DWORD(p);
    p += 2;
  }
  info->x = GET_WORD(p);
  p++;
  info->y = GET_WORD(p);
  p++;
  info->cx = GET_WORD(p);
  p++;
  info->cy = GET_WORD(p);
  p++;
  if (dialogEx) {
    /* id is a DWORD for DIALOGEX */
    info->id = GET_DWORD(p);
    p += 2;
  }
  else {
    info->id = GET_WORD(p);
    p++;
  }
  if (GET_WORD(p) == 0xffff) {
    static const char class_names[6][10] = {
      "Button", /* 0x80 */
      "Edit", /* 0x81 */
      "Static", /* 0x82 */
      "ListBox", /* 0x83 */
      "ScrollBar", /* 0x84 */
      "ComboBox" /* 0x85 */
    };
    WORD id = GET_WORD(p + 1);
    /* Widgets treats dialog control class1 ids 0-5 same way as 0x80-0x85 */
    if ((id >= 0x80) && (id <= 0x85)) {
      id -= 0x80;
    }
    if (id <= 5) {
      info->className = class_names[id];
    }
    else {
      info->className = NULL;
      ERR("Unknown built-in class1 id %04x\n", id);
    }
    p += 2;
  }
  else {
    info->className = (char*)p;
    p += strlen(info->className) + 1;
  }
  if (GET_WORD(p) == 0xffff) { /* Is it an integer id? */
    info->windowName = MAKEINTRESOURCEA(GET_WORD(p + 1));
    p += 2;
  }
  else {
    info->windowName = (char*)p;
    p += strlen(info->windowName) + 1;
  }
#define debugstr_w(x) (x)
  TRACE(" %s %s %ld, %d, %d, %d, %d, %08x, %08x, %08x\n",
      debugstr_w(info->className), debugstr_w(info->windowName),
      info->id, info->x, info->y, info->cx, info->cy,
      info->style, info->exStyle, info->helpId);
  if (GET_WORD(p)) {
    //if (TRACE_ON(dialog))
#ifdef _DEBUG
    {
      WORD i, count = GET_WORD(p) / sizeof(WORD);
      TRACE(" BEGIN\n");
      TRACE(" ");
      for (i = 0; i < count; i++) {
        TRACE("%04x,", GET_WORD(p + i + 1));
      }
      TRACE("\n");
      TRACE(" END\n");
    }
#endif
    info->data = p + 1;
    p += GET_WORD(p) / sizeof(WORD);
  }
  else {
    info->data = NULL;
  }
  p++;
  /* Next control is on dword boundary */
  return (const WORD*)(((UINT_PTR)p + 3) & ~3);
}
/***********************************************************************
 * DIALOG_CreateControls32
 *
 * Create the control windows for a dialog.
 */
static BOOL DIALOG_CreateControls32(HWID hwnd, const WORD* ptemplate, const DLG_TEMPLATE* dlgTemplate)
{
  DLG_CONTROL_INFO info;
  HWID hwndCtrl, hwndDefButton = 0;
  INT items = dlgTemplate->nbItems;
  TRACE(" BEGIN\n");
  while (items--) {
    ptemplate = DIALOG_GetControl32((const WORD*)ptemplate, &info,
        dlgTemplate->dialogEx);
    info.style &= ~WS_POPUP;
    info.style |= WS_CHILD;
    if (info.style & WS_BORDER) {
      info.style &= ~WS_BORDER;
      info.exStyle |= WS_EX_CLIENTEDGE;
    }
    {
      hwndCtrl = CreateWidget(info.exStyle | WS_EX_NOPARENTNOTIFY,
          info.className, info.windowName,
          info.style | WS_CHILD,
          info.x,
          info.y,
          info.cx,
          info.cy,
          hwnd, (LPVOID)info.data);
    }
    if (!hwndCtrl) {
      WARN("control %s %s creation failed\n", debugstr_w(info.className),
          debugstr_w(info.windowName));
      if (dlgTemplate->style & DS_NOFAILCREATE) {
        continue;
      }
      return FALSE;
    }
    /* Send initialisation messages to the control */
    //if (dlgInfo->hUserFont) SendMessageW( hwndCtrl, WM_SETFONT, (WPARAM)dlgInfo->hUserFont, 0 );
    if (SendMessageW(hwndCtrl, WM_GETDLGCODE, 0, 0) & DLGC_DEFPUSHBUTTON) {
      /* If there's already a default push-button, set it back */
      /* to normal and use this one instead. */
      if (hwndDefButton) {
        SendMessageW(hwndDefButton, BM_SETSTYLE, BS_PUSHBUTTON, FALSE);
      }
      hwndDefButton = hwndCtrl;
      //dlgInfo->idResult = GetWidgetLong( hwndCtrl, GWLP_ID );
    }
  }
  TRACE(" END\n");
  return TRUE;
}
/***********************************************************************
 * DIALOG_ParseTemplate32
 *
 * Fill a DLG_TEMPLATE structure from the dialog ptemplate, and return
 * a pointer to the first control.
 */
static LPCSTR DIALOG_ParseTemplate32(LPCSTR ptemplate, DLG_TEMPLATE* result)
{
  const WORD* p = (const WORD*)ptemplate;
  WORD signature;
  WORD dlgver;
  dlgver = GET_WORD(p);
  p++;
  signature = GET_WORD(p);
  p++;
  if (dlgver == 1 && signature == 0xffff) { /* DIALOGEX resource */
    result->dialogEx = TRUE;
    result->helpId = GET_DWORD(p);
    p += 2;
    result->exStyle = GET_DWORD(p);
    p += 2;
    result->style = GET_DWORD(p);
    p += 2;
  }
  else {
    result->style = GET_DWORD(p - 2);
    result->dialogEx = FALSE;
    result->helpId = 0;
    result->exStyle = GET_DWORD(p);
    p += 2;
  }
  result->nbItems = GET_WORD(p);
  p++;
  result->x = GET_WORD(p);
  p++;
  result->y = GET_WORD(p);
  p++;
  result->cx = GET_WORD(p);
  p++;
  result->cy = GET_WORD(p);
  p++;
  TRACE("DIALOG%s %d, %d, %d, %d, %d\n",
      result->dialogEx ? "EX" : "", result->x, result->y,
      result->cx, result->cy, result->helpId);
  TRACE(" STYLE 0x%08x\n", result->style);
  TRACE(" EXSTYLE 0x%08x\n", result->exStyle);
  /* Get the menu name */
  switch (GET_WORD(p)) {
  case 0x0000:
    result->menuName = NULL;
    p++;
    break;
  case 0xffff:
    result->menuName = MAKEINTRESOURCEA(GET_WORD(p + 1));
    p += 2;
    TRACE(" MENU %04x\n", LOWORD(result->menuName));
    break;
  default:
    result->menuName = (char*)p;
    TRACE(" MENU %s\n", debugstr_w(result->menuName));
    p += strlen(result->menuName) + 1;
    break;
  }
  /* Get the class1 name */
  switch (GET_WORD(p)) {
  case 0x0000:
    result->className = (LPCSTR)DIALOG_CLASS_ATOM;
    p++;
    break;
  case 0xffff:
    result->className = MAKEINTRESOURCEA(GET_WORD(p + 1));
    p += 2;
    TRACE(" CLASS %04x\n", LOWORD(result->className));
    break;
  default:
    result->className = (char*)p;
    TRACE(" CLASS %s\n", debugstr_w(result->className));
    p += strlen(result->className) + 1;
    break;
  }
  /* Get the window caption */
  result->caption = (char*)p;
  p += strlen(result->caption) + 1;
  TRACE(" CAPTION %s\n", debugstr_w(result->caption));
  /* Get the font name */
  result->pointSize = 0;
  result->faceName = NULL;
  result->weight = FW_DONTCARE;
  result->italic = FALSE;
  if (result->style & DS_SETFONT) {
    result->pointSize = GET_WORD(p);
    p++;
    /* If pointSize is 0x7fff, it means that we need to use the font
    * in NONCLIENTMETRICSW.lfMessageFont, and NOT read the weight,
    * italic, and facename from the dialog ptemplate.
    */
    if (result->pointSize == 0x7fff) {
      /* We could call SystemParametersInfo here, but then we'd have
      * to convert from pixel size to point size (which can be
      * imprecise).
      */
      TRACE(" FONT: Using message box font\n");
    }
    else {
      if (result->dialogEx) {
        result->weight = GET_WORD(p);
        p++;
        result->italic = LOBYTE(GET_WORD(p));
        p++;
      }
      result->faceName = (char*)p;
      p += strlen(result->faceName) + 1;
      TRACE(" FONT %d, %s, %d, %s\n",
          result->pointSize, debugstr_w(result->faceName),
          result->weight, result->italic ? "TRUE" : "FALSE");
    }
  }
  /* First control is on dword boundary */
  return (LPCSTR)(((UINT_PTR)p + 3) & ~3);
}
/***********************************************************************
 * DIALOG_CreateIndirect
 * Creates a dialog box window
 *
 * modal = TRUE if we are called from a modal dialog box.
 * (it's more compatible to do it here, as under Widgets the owner
 * is never disabled if the dialog fails because of an invalid ptemplate)
 */
static HWID DIALOG_CreateIndirect(LPCVOID dlgTemplate, HWID owner, DLGPROC dlgProc, LPARAM param)
{
  HWID hwnd;
  RECT rect;
  POINT pos;
  SIZE size;
  DLG_TEMPLATE template1;
  DIALOGINFO* dlgInfo = NULL;
  DWORD units = GetDialogBaseUnits();
  BOOL ownerEnabled = TRUE;
  HMENU hMenu = 0;
  HFONT hUserFont = 0;
  UINT flags = 0;
  UINT xBaseUnit = LOWORD(units);
  UINT yBaseUnit = HIWORD(units);
  /* Parse dialog template1 */
  if (!dlgTemplate) {
    return 0;
  }
  dlgTemplate = DIALOG_ParseTemplate32((char*)dlgTemplate, &template1);
  /* Load menu */
  //if (template1.menuName) hMenu = LoadMenuW( hInst, template1.menuName );
  /* Create custom font if needed */
  if (template1.style & DS_SETFONT) {
    HDC dc = GetDC((HWND)0);
    if (template1.pointSize == 0x7fff) {
      /* We get the message font from the non-client metrics */
      NONCLIENTMETRICSW ncMetrics;
      ncMetrics.cbSize = sizeof(NONCLIENTMETRICSW);
      if (SystemParametersInfoW(SPI_GETNONCLIENTMETRICS,
          sizeof(NONCLIENTMETRICSW), &ncMetrics, 0)) {
        hUserFont = CreateFontIndirectW(&ncMetrics.lfMessageFont);
      }
    }
    else {
      /* We convert the size to pixels and then make it -ve. This works
      * for both +ve and -ve template1.pointSize */
      int pixels = MulDiv(template1.pointSize, GetDeviceCaps(dc , LOGPIXELSY), 72);
      hUserFont = CreateFontA(-pixels, 0, 0, 0, template1.weight,
          template1.italic, FALSE, FALSE, DEFAULT_CHARSET, 0, 0,
          PROOF_QUALITY, FF_DONTCARE,
          template1.faceName);
    }
    if (hUserFont) {
      SIZE charSize;
      HFONT hOldFont = (HFONT)SelectObject(dc, hUserFont);
      charSize.cx = GdiGetCharDimensions(dc, NULL, &charSize.cy);
      if (charSize.cx) {
        xBaseUnit = charSize.cx;
        yBaseUnit = charSize.cy;
      }
      SelectObject(dc, hOldFont);
    }
    ReleaseDC((HWND)0, dc);
    TRACE("units = %d,%d\n", xBaseUnit, yBaseUnit);
  }
  /* Create dialog main window */
  rect.left = rect.top = 0;
  rect.right = MulDiv(template1.cx, xBaseUnit, 4);
  rect.bottom = MulDiv(template1.cy, yBaseUnit, 8);
  if (template1.style & DS_CONTROL) {
    template1.style &= ~(WS_CAPTION | WS_SYSMENU);
  }
  template1.style |= DS_3DLOOK;
  if (template1.style & DS_MODALFRAME) {
    template1.exStyle |= WS_EX_DLGMODALFRAME;
  }
  if ((template1.style & DS_CONTROL) || !(template1.style & WS_CHILD)) {
    template1.exStyle |= WS_EX_CONTROLPARENT;
  }
  AdjustWindowRectEx(&rect, template1.style, (hMenu != 0), template1.exStyle);
  pos.x = rect.left;
  pos.y = rect.top;
  size.cx = rect.right - rect.left;
  size.cy = rect.bottom - rect.top;
  if (template1.x == (SHORT)0x8000 /*CW_USEDEFAULT16*/) {
    pos.x = pos.y = CW_USEDEFAULT;
  }
  else {
    HMONITOR monitor = 0;
    MONITORINFO mon_info;
    mon_info.cbSize = sizeof(mon_info);
    if (template1.style & DS_CENTER) {
      monitor = MonitorFromWindow(owner->handle->hwnd ? owner->handle->hwnd : GetActiveWindow(), MONITOR_DEFAULTTOPRIMARY);
      GetMonitorInfoW(monitor, &mon_info);
      pos.x = (mon_info.rcWork.left + mon_info.rcWork.right - size.cx) / 2;
      pos.y = (mon_info.rcWork.top + mon_info.rcWork.bottom - size.cy) / 2;
    }
    else if (template1.style & DS_CENTERMOUSE) {
      GetCursorPos(&pos);
      monitor = MonitorFromPoint(pos, MONITOR_DEFAULTTOPRIMARY);
      GetMonitorInfoW(monitor, &mon_info);
    }
    else {
      pos.x += MulDiv(template1.x, xBaseUnit, 4);
      pos.y += MulDiv(template1.y, yBaseUnit, 8);
      //if (!(template1.style & (WS_CHILD|DS_ABSALIGN))) ClientToScreen( owner, &pos );
    }
    if (!(template1.style & WS_CHILD)) {
      INT dX, dY;
      /* try to fit it into the desktop */
      if (!monitor) {
        SetRect(&rect, pos.x, pos.y, pos.x + size.cx, pos.y + size.cy);
        monitor = MonitorFromRect(&rect, MONITOR_DEFAULTTOPRIMARY);
        GetMonitorInfoW(monitor, &mon_info);
      }
      if ((dX = pos.x + size.cx + GetSystemMetrics(SM_CXDLGFRAME) - mon_info.rcWork.right) > 0) {
        pos.x -= dX;
      }
      if ((dY = pos.y + size.cy + GetSystemMetrics(SM_CYDLGFRAME) - mon_info.rcWork.bottom) > 0) {
        pos.y -= dY;
      }
      if (pos.x < mon_info.rcWork.left) {
        pos.x = mon_info.rcWork.left;
      }
      if (pos.y < mon_info.rcWork.top) {
        pos.y = mon_info.rcWork.top;
      }
    }
  }
  int modal = 0;
  if (modal) {
    ownerEnabled = DIALOG_DisableOwner(owner);
    if (ownerEnabled) {
      flags |= DF_OWNERENABLED;
    }
  }
  hwnd = CreateWidget(template1.exStyle, template1.className, template1.caption,
      template1.style & ~WS_VISIBLE, pos.x, pos.y, size.cx, size.cy, owner, NULL);
  if (!hwnd) {
    if (hUserFont) {
      DeleteObject(hUserFont);
    }
    if (hMenu) {
      DestroyMenu(hMenu);
    }
    if (modal && (flags & DF_OWNERENABLED)) {
      DIALOG_EnableOwner(owner);
    }
    return 0;
  }
  SetWidgetLongPtrW(hwnd, DWLP_DLGPROC, (ULONG_PTR)dlgProc);
  if (dlgProc && dlgInfo->hUserFont) {
    SendMessageW(hwnd, WM_SETFONT, (WPARAM)dlgInfo->hUserFont, 0);
  }
  /* Create controls */
  if (DIALOG_CreateControls32(hwnd, (const WORD*)dlgTemplate, &template1)) {
    /* Send initialisation messages and set focus */
    if (dlgProc) {
      HWID focus = GetNextDlgTabItem(hwnd, 0, FALSE);
      if (!focus) {
        focus = GetNextDlgGroupItem(hwnd, 0, FALSE);
      }
      if (SendMessageW(hwnd, WM_INITDIALOG, (WPARAM)focus, param) && IsWidget(hwnd) &&
          ((~template1.style & DS_CONTROL) || (template1.style & WS_VISIBLE))) {
        /* By returning TRUE, app has requested a default focus assignment.
        * WM_INITDIALOG may have changed the tab order, so find the first
        * tabstop control again. */
        //dlgInfo->hwndFocus = GetNextDlgTabItem( hwnd, 0, FALSE );
        //if (!dlgInfo->hwndFocus) dlgInfo->hwndFocus = GetNextDlgGroupItem( hwnd, 0, FALSE );
        //if( dlgInfo->hwndFocus )
        // SetFocus( dlgInfo->hwndFocus );
      }
    }
    if (template1.style & WS_VISIBLE && !(GetWidgetLongW(hwnd, GWL_STYLE) & WS_VISIBLE)) {
      ShowWidget(hwnd, SW_SHOWNORMAL); /* SW_SHOW doesn't always work */
    }
    return hwnd;
  }
  //if (modal && ownerEnabled) DIALOG_EnableOwner(owner);
  //if( IsWidget(hwnd) ) DestroyWidget( hwnd );
  return 0;
}
/***********************************************************************
 * DIALOG_DoDialogBox
 */
INT DIALOG_DoDialogBox(HWID hwid, HWID owner)
{
  DIALOGINFO* dlgInfo = (DIALOGINFO*)(hwid + 1);
  MSG msg;
  INT retval;
  HWID ownerMsg = GetAncestor(owner, GA_ROOT);
  BOOL bFirstEmpty;
  HWND hwnd = hwid->handle->hwnd;
  bFirstEmpty = TRUE;
  if (!(dlgInfo->flags & DF_END)) { /* was EndDialog called in WM_INITDIALOG ? */
    for (;;) {
      if (!PeekMessageW(&msg, 0, 0, 0, PM_REMOVE)) {
        if (bFirstEmpty) {
          /* ShowWidget the first time the queue goes empty */
          ShowWindow(hwnd, SW_SHOWNORMAL);
          bFirstEmpty = FALSE;
        }
        if (!(GetWindowLongW(hwnd, GWL_STYLE) & DS_NOIDLEMSG)) {
          /* No message present -> send ENTERIDLE and wait */
          SendMessageW(ownerMsg, WM_ENTERIDLE, MSGF_DIALOGBOX, (LPARAM)hwnd);
        }
        GetMessageW(&msg, 0, 0, 0);
      }
      if (msg.message == WM_QUIT) {
        PostQuitMessage(msg.wParam);
        if (!IsWindow(hwnd)) {
          return 0;
        }
        break;
      }
      if (!IsWindow(hwnd)) {
        return 0;
      }
      if (!(dlgInfo->flags & DF_END) && !IsDialogMessageW(hwnd, &msg)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
      }
      if (!IsWindow(hwnd)) {
        return 0;
      }
      if (dlgInfo->flags & DF_END) {
        break;
      }
      if (bFirstEmpty && msg.message == WM_TIMER) {
        ShowWindow(hwnd, SW_SHOWNORMAL);
        bFirstEmpty = FALSE;
      }
    }
  }
  if (dlgInfo->flags & DF_OWNERENABLED) {
    DIALOG_EnableOwner(owner);
  }
  retval = dlgInfo->idResult;
  DestroyWindow(hwnd);
  return retval;
}
/***********************************************************************
 * EndDialog (USER32.@)
 */
BOOL WINAPI EndDialog(HWID hwid, INT_PTR retval)
{
  BOOL wasEnabled = TRUE;
  DIALOGINFO* dlgInfo = (DIALOGINFO*)(hwid + 1);
  HWID owner = NULL;
  HWND hwnd = hwid->handle->hwnd;
  TRACE("%p %ld\n", hwnd, retval);
  dlgInfo->idResult = retval;
  dlgInfo->flags |= DF_END;
  wasEnabled = (dlgInfo->flags & DF_OWNERENABLED);
  owner = GetWidget(hwid, GW_OWNER);
  if (wasEnabled && owner) {
    DIALOG_EnableOwner(owner);
  }
  /* Widgets sets the focus to the dialog itself in EndDialog */
  if (IsChild(hwnd, GetFocus())) {
    SetFocus(hwnd);
  }
  /* Don't have to send a ShowWidget(SW_HIDE), just do SetWindowPos
  SetWidgetPos with SWP_HIDEWINDOW as done in Widgets */
  SetWindowPos(hwnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE
      | SWP_NOZORDER | SWP_NOACTIVATE | SWP_HIDEWINDOW);
  if (hwnd == GetActiveWindow()) {
    /* If this dialog was given an owner then set the focus to that owner
    even when the owner is disabled (normally when a window closes any
    disabled windows cannot receive the focus). */
    if (owner && owner->handle->hwnd) {
      SetForegroundWindow(owner->handle->hwnd);
    }
    //else
    // WINPOS_ActivateOtherWidget( hwnd );
  }
  /* unblock dialog loop */
  PostMessageA(hwnd, WM_NULL, 0, 0);
  return TRUE;
}
#if 0
/***********************************************************************
 * DIALOG_IsAccelerator
 */
static BOOL DIALOG_IsAccelerator(HWID hwnd, HWID hwndDlg, WPARAM wParam)
{
  HWID hwndControl = hwnd;
  HWID hwndNext;
  INT dlgCode;
  char buffer[128];
  do {
    DWORD style = GetWidgetLongW(hwndControl, GWL_STYLE);
    if ((style & (WS_VISIBLE | WS_DISABLED)) == WS_VISIBLE) {
      dlgCode = SendMessageW(hwndControl, WM_GETDLGCODE, 0, 0);
      if ((dlgCode & (DLGC_BUTTON | DLGC_STATIC)) &&
          GetWidgetText(hwndControl, buffer, sizeof(buffer) / sizeof(char))) {
        /* find the accelerator key */
        LPSTR p = buffer - 2;
        do {
          p = strchr(p + 2, '&');
        }
        while (p != NULL && p[1] == '&');
        /* and check if it's the one we're looking for */
        if (p != NULL && toupper(p[1]) == toupper(wParam)) {
          if ((dlgCode & DLGC_STATIC) || (style & 0x0f) == BS_GROUPBOX) {
            /* set focus to the control */
            SendMessageW(hwndDlg, WM_NEXTDLGCTL, (WPARAM)hwndControl, 1);
            /* and bump it on to next */
            SendMessageW(hwndDlg, WM_NEXTDLGCTL, 0, 0);
          }
          else if (dlgCode & DLGC_BUTTON) {
            /* send BM_CLICK message to the control */
            SendMessageW(hwndControl, BM_CLICK, 0, 0);
          }
          return TRUE;
        }
      }
      hwndNext = GetWidget(hwndControl, GW_CHILD);
    }
    else {
      hwndNext = 0;
    }
    if (!hwndNext) {
      hwndNext = GetWidget(hwndControl, GW_HWNDNEXT);
    }
    while (!hwndNext && hwndControl) {
      hwndControl = GetParent(hwndControl);
      if (hwndControl == hwndDlg) {
        if (hwnd == hwndDlg) { /* prevent endless loop */
          hwndNext = hwnd;
          break;
        }
        hwndNext = GetWidget(hwndDlg, GW_CHILD);
      }
      else {
        hwndNext = GetWidget(hwndControl, GW_HWNDNEXT);
      }
    }
    hwndControl = hwndNext;
  }
  while (hwndControl && (hwndControl != hwnd));
  return FALSE;
}
/***********************************************************************
 * DIALOG_FindMsgDestination
 *
 * The messages that IsDialogMessage sends may not go to the dialog
 * calling IsDialogMessage if that dialog is a child, and it has the
 * DS_CONTROL style set.
 * We propagate up until we hit one that does not have DS_CONTROL, or
 * whose parent is not a dialog.
 *
 * This is undocumented behaviour.
 */
static HWID DIALOG_FindMsgDestination(HWID hwndDlg)
{
  while (GetWidgetLong(hwndDlg, GWL_STYLE) & DS_CONTROL) {
    WND* pParent;
    HWID hParent = GetParent(hwndDlg);
    if (!hParent) {
      break;
    }
    pParent = WIN_GetPtr(hParent);
    if (!pParent || pParent == WND_OTHER_PROCESS || pParent == WND_DESKTOP) {
      break;
    }
    if (!pParent->dlgInfo) {
      WIN_ReleasePtr(pParent);
      break;
    }
    WIN_ReleasePtr(pParent);
    hwndDlg = hParent;
  }
  return hwndDlg;
}
/***********************************************************************
 * DIALOG_FixOneChildOnChangeFocus
 *
 * Callback helper for DIALOG_FixChildrenOnChangeFocus
 */
static BOOL CALLBACK DIALOG_FixOneChildOnChangeFocus(HWID hwndChild,
    LPARAM lParam)
{
  /* If a default pushbutton then no longer default */
  if (DLGC_DEFPUSHBUTTON & SendMessageW(hwndChild, WM_GETDLGCODE, 0, 0)) {
    SendMessageW(hwndChild, BM_SETSTYLE, BS_PUSHBUTTON, TRUE);
  }
  return TRUE;
}
/***********************************************************************
 * DIALOG_FixChildrenOnChangeFocus
 *
 * Following the change of focus that occurs for example after handling
 * a WM_KEYDOWN VK_TAB in IsDialogMessage, some tidying of the dialog's
 * children may be required.
 */
static void DIALOG_FixChildrenOnChangeFocus(HWID hwndDlg, HWID hwndNext)
{
  INT dlgcode_next = SendMessageW(hwndNext, WM_GETDLGCODE, 0, 0);
  /* INT dlgcode_dlg = SendMessageW (hwndDlg, WM_GETDLGCODE, 0, 0); */
  /* Widgets does ask for this. I don't know why yet */
  EnumChildWidgets(hwndDlg, DIALOG_FixOneChildOnChangeFocus, 0);
  /* If the button that is getting the focus WAS flagged as the default
  * pushbutton then ask the dialog what it thinks the default is and
  * set that in the default style.
  */
  if (dlgcode_next & DLGC_DEFPUSHBUTTON) {
    DWORD def_id = SendMessageW(hwndDlg, DM_GETDEFID, 0, 0);
    if (HIWORD(def_id) == DC_HASDEFID) {
      HWID hwndDef;
      def_id = LOWORD(def_id);
      hwndDef = GetDlgItem(hwndDlg, def_id);
      if (hwndDef) {
        INT dlgcode_def = SendMessageW(hwndDef, WM_GETDLGCODE, 0, 0);
        /* I know that if it is a button then it should already be a
        * UNDEFPUSHBUTTON, since we have just told the buttons to
        * change style. But maybe they ignored our request
        */
        if ((dlgcode_def & DLGC_BUTTON) &&
            (dlgcode_def & DLGC_UNDEFPUSHBUTTON)) {
          SendMessageW(hwndDef, BM_SETSTYLE, BS_DEFPUSHBUTTON, TRUE);
        }
      }
    }
  }
  else if ((dlgcode_next & DLGC_BUTTON) && (dlgcode_next & DLGC_UNDEFPUSHBUTTON)) {
    SendMessageW(hwndNext, BM_SETSTYLE, BS_DEFPUSHBUTTON, TRUE);
    /* I wonder why it doesn't send a DM_SETDEFID */
  }
}
/***********************************************************************
 * DIALOG_IdToHwnd
 *
 * A recursive version of GetDlgItem
 *
 * RETURNS
 * The HWID for a Child ID.
 */
static HWID DIALOG_IdToHwnd(HWID hwndDlg, INT id)
{
  int i;
  HWID* list = WIN_ListChildren(hwndDlg);
  HWID ret = 0;
  if (!list) {
    return 0;
  }
  for (i = 0; list[i]; i++) {
    if (GetWidgetLongPtrW(list[i], GWLP_ID) == id) {
      ret = list[i];
      break;
    }
    /* Recurse into every child */
    if ((ret = DIALOG_IdToHwnd(list[i], id))) {
      break;
    }
  }
  HeapFree(GetProcessHeap(), 0, list);
  return ret;
}
/***********************************************************************
 * IsDialogMessageW (USER32.@)
 */
BOOL WINAPI IsDialogMessageW(HWID hwndDlg, LPMSG msg)
{
  INT dlgCode = 0;
  if (CallMsgFilterW(msg, MSGF_DIALOGBOX)) {
    return TRUE;
  }
  hwndDlg = WIN_GetFullHandle(hwndDlg);
  if (is_desktop_window(hwndDlg)) {
    return FALSE;
  }
  if ((hwndDlg != msg->hwnd) && !IsChild(hwndDlg, msg->hwnd)) {
    return FALSE;
  }
  hwndDlg = DIALOG_FindMsgDestination(hwndDlg);
  switch (msg->message) {
  case WM_KEYDOWN:
    dlgCode = SendMessageW(msg->hwnd, WM_GETDLGCODE, msg->wParam, (LPARAM)msg);
    if (dlgCode & (DLGC_WANTMESSAGE)) {
      break;
    }
    switch (msg->wParam) {
    case VK_TAB:
      if (!(dlgCode & DLGC_WANTTAB)) {
        BOOL fIsDialog = TRUE;
        WND* pWnd = WIN_GetPtr(hwndDlg);
        if (pWnd && pWnd != WND_OTHER_PROCESS) {
          fIsDialog = (pWnd->dlgInfo != NULL);
          WIN_ReleasePtr(pWnd);
        }
        /* I am not sure under which circumstances the TAB is handled
        * each way. All I do know is that it does not always simply
        * send WM_NEXTDLGCTL. (Personally I have never yet seen it
        * do so but I presume someone has)
        */
        if (fIsDialog) {
          SendMessageW(hwndDlg, WM_NEXTDLGCTL, (GetKeyState(VK_SHIFT) & 0x8000), 0);
        }
        else {
          /* It would appear that GetNextDlgTabItem can handle being
          * passed hwndDlg rather than NULL but that is undocumented
          * so let's do it properly
          */
          HWID hwndFocus = GetFocus();
          HWID hwndNext = GetNextDlgTabItem(hwndDlg,
              hwndFocus == hwndDlg ? NULL : hwndFocus,
              GetKeyState(VK_SHIFT) & 0x8000);
          if (hwndNext) {
            dlgCode = SendMessageW(hwndNext, WM_GETDLGCODE,
                msg->wParam, (LPARAM)msg);
            if (dlgCode & DLGC_HASSETSEL) {
              INT maxlen = 1 + SendMessageW(hwndNext, WM_GETTEXTLENGTH, 0, 0);
              char* buffer = HeapAlloc(GetProcessHeap(), 0, maxlen * sizeof(char));
              if (buffer) {
                INT length;
                SendMessageW(hwndNext, WM_GETTEXT, maxlen, (LPARAM) buffer);
                length = strlen(buffer);
                HeapFree(GetProcessHeap(), 0, buffer);
                SendMessageW(hwndNext, EM_SETSEL, 0, length);
              }
            }
            SetFocus(hwndNext);
            DIALOG_FixChildrenOnChangeFocus(hwndDlg, hwndNext);
          }
          else {
            return FALSE;
          }
        }
        return TRUE;
      }
      break;
    case VK_RIGHT:
    case VK_DOWN:
    case VK_LEFT:
    case VK_UP:
      if (!(dlgCode & DLGC_WANTARROWS)) {
        BOOL fPrevious = (msg->wParam == VK_LEFT || msg->wParam == VK_UP);
        HWID hwndNext = GetNextDlgGroupItem(hwndDlg, GetFocus(), fPrevious);
        SendMessageW(hwndDlg, WM_NEXTDLGCTL, (WPARAM)hwndNext, 1);
        return TRUE;
      }
      break;
    case VK_CANCEL:
    case VK_ESCAPE:
      SendMessageW(hwndDlg, WM_COMMAND, IDCANCEL, (LPARAM)GetDlgItem(hwndDlg, IDCANCEL));
      return TRUE;
    case VK_EXECUTE:
    case VK_RETURN: {
      DWORD dw;
      if ((GetFocus() == msg->hwnd) &&
          (SendMessageW(msg->hwnd, WM_GETDLGCODE, 0, 0) & DLGC_DEFPUSHBUTTON)) {
        SendMessageW(hwndDlg, WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(msg->hwnd), BN_CLICKED), (LPARAM)msg->hwnd);
      }
      else if (DC_HASDEFID == HIWORD(dw = SendMessageW(hwndDlg, DM_GETDEFID, 0, 0))) {
        HWID hwndDef = DIALOG_IdToHwnd(hwndDlg, LOWORD(dw));
        if (!hwndDef || IsWidgetEnabled(hwndDef)) {
          SendMessageW(hwndDlg, WM_COMMAND, MAKEWPARAM(LOWORD(dw), BN_CLICKED), (LPARAM)hwndDef);
        }
      }
      else {
        SendMessageW(hwndDlg, WM_COMMAND, IDOK, (LPARAM)GetDlgItem(hwndDlg, IDOK));
      }
    }
    return TRUE;
    }
    break;
  case WM_CHAR:
    /* FIXME Under what circumstances does WM_GETDLGCODE get sent?
    * It does NOT get sent in the test program I have
    */
    dlgCode = SendMessageW(msg->hwnd, WM_GETDLGCODE, msg->wParam, (LPARAM)msg);
    if (dlgCode & (DLGC_WANTCHARS | DLGC_WANTMESSAGE)) {
      break;
    }
    if (msg->wParam == '\t' && (dlgCode & DLGC_WANTTAB)) {
      break;
    }
    /* drop through */
  case WM_SYSCHAR:
    if (DIALOG_IsAccelerator(WIN_GetFullHandle(msg->hwnd), hwndDlg, msg->wParam)) {
      /* don't translate or dispatch */
      return TRUE;
    }
    break;
  }
  TranslateMessage(msg);
  DispatchMessageW(msg);
  return TRUE;
}
/***********************************************************************
 * GetDlgCtrlID (USER32.@)
 */
INT WINAPI GetDlgCtrlID(HWID hwnd)
{
  return GetWidgetLongPtrW(hwnd, GWLP_ID);
}
/***********************************************************************
 * GetDlgItem (USER32.@)
 */
HWID WINAPI GetDlgItem(HWID hwndDlg, INT id)
{
  int i;
  HWID* list = WIN_ListChildren(hwndDlg);
  HWID ret = 0;
  if (!list) {
    return 0;
  }
  for (i = 0; list[i]; i++) if (GetWidgetLongPtrW(list[i], GWLP_ID) == id) {
      break;
    }
  ret = list[i];
  HeapFree(GetProcessHeap(), 0, list);
  return ret;
}
/*******************************************************************
 * SendDlgItemMessageA (USER32.@)
 */
LRESULT WINAPI SendDlgItemMessageA(HWID hwnd, INT id, UINT msg,
    WPARAM wParam, LPARAM lParam)
{
  HWID hwndCtrl = GetDlgItem(hwnd, id);
  if (hwndCtrl) {
    return SendMessageA(hwndCtrl, msg, wParam, lParam);
  }
  else {
    return 0;
  }
}
/*******************************************************************
 * SendDlgItemMessageW (USER32.@)
 */
LRESULT WINAPI SendDlgItemMessageW(HWID hwnd, INT id, UINT msg,
    WPARAM wParam, LPARAM lParam)
{
  HWID hwndCtrl = GetDlgItem(hwnd, id);
  if (hwndCtrl) {
    return SendMessageW(hwndCtrl, msg, wParam, lParam);
  }
  else {
    return 0;
  }
}
/*******************************************************************
 * SetDlgItemTextA (USER32.@)
 */
BOOL WINAPI SetDlgItemTextA(HWID hwnd, INT id, LPCSTR lpString)
{
  return SendDlgItemMessageA(hwnd, id, WM_SETTEXT, 0, (LPARAM)lpString);
}
/*******************************************************************
 * SetDlgItemTextW (USER32.@)
 */
BOOL WINAPI SetDlgItemTextW(HWID hwnd, INT id, LPCSTR lpString)
{
  return SendDlgItemMessageW(hwnd, id, WM_SETTEXT, 0, (LPARAM)lpString);
}
/***********************************************************************
 * GetDlgItemTextA (USER32.@)
 */
UINT WINAPI GetDlgItemTextA(HWID hwnd, INT id, LPSTR str, INT len)
{
  if (str && (len > 0)) {
    str[0] = '\0';
  }
  return (UINT)SendDlgItemMessageA(hwnd, id, WM_GETTEXT,
      len, (LPARAM)str);
}
/***********************************************************************
 * GetDlgItemTextW (USER32.@)
 */
UINT WINAPI GetDlgItemTextW(HWID hwnd, INT id, LPSTR str, INT len)
{
  if (str && (len > 0)) {
    str[0] = '\0';
  }
  return (UINT)SendDlgItemMessageW(hwnd, id, WM_GETTEXT,
      len, (LPARAM)str);
}
/*******************************************************************
 * SetDlgItemInt (USER32.@)
 */
BOOL WINAPI SetDlgItemInt(HWID hwnd, INT id, UINT value,
    BOOL fSigned)
{
  char str[20];
  if (fSigned) {
    sprintf(str, "%d", (INT)value);
  }
  else {
    sprintf(str, "%u", value);
  }
  SendDlgItemMessageA(hwnd, id, WM_SETTEXT, 0, (LPARAM)str);
  return TRUE;
}
/***********************************************************************
 * GetDlgItemInt (USER32.@)
 */
UINT WINAPI GetDlgItemInt(HWID hwnd, INT id, BOOL* translated,
    BOOL fSigned)
{
  char str[30];
  char* endptr;
  LONG_PTR result = 0;
  if (translated) {
    *translated = FALSE;
  }
  if (!SendDlgItemMessageA(hwnd, id, WM_GETTEXT, sizeof(str), (LPARAM)str)) {
    return 0;
  }
  if (fSigned) {
    result = strtol(str, &endptr, 10);
    if (!endptr || (endptr == str)) { /* Conversion was unsuccessful */
      return 0;
    }
    if (((result == LONG_MIN) || (result == LONG_MAX)) && (errno == ERANGE)) {
      return 0;
    }
  }
  else {
    result = strtoul(str, &endptr, 10);
    if (!endptr || (endptr == str)) { /* Conversion was unsuccessful */
      return 0;
    }
    if ((result == ULONG_MAX) && (errno == ERANGE)) {
      return 0;
    }
  }
  if (translated) {
    *translated = TRUE;
  }
  return (UINT)result;
}
/***********************************************************************
 * CheckDlgButton (USER32.@)
 */
BOOL WINAPI CheckDlgButton(HWID hwnd, INT id, UINT check)
{
  SendDlgItemMessageW(hwnd, id, BM_SETCHECK, check, 0);
  return TRUE;
}
/***********************************************************************
 * IsDlgButtonChecked (USER32.@)
 */
UINT WINAPI IsDlgButtonChecked(HWID hwnd, int id)
{
  return (UINT)SendDlgItemMessageW(hwnd, id, BM_GETCHECK, 0, 0);
}
/***********************************************************************
 * CheckRB
 *
 * Callback function used to check/uncheck radio buttons that fall
 * within a specific range of IDs.
 */
static BOOL CALLBACK CheckRB(HWID hwndChild, LPARAM lParam)
{
  LONG lChildID = GetWidgetLongPtrW(hwndChild, GWLP_ID);
  RADIOGROUP* lpRadioGroup = (RADIOGROUP*) lParam;
  if ((lChildID >= lpRadioGroup->firstID) &&
      (lChildID <= lpRadioGroup->lastID)) {
    if (lChildID == lpRadioGroup->checkID) {
      SendMessageW(hwndChild, BM_SETCHECK, BST_CHECKED, 0);
    }
    else {
      SendMessageW(hwndChild, BM_SETCHECK, BST_UNCHECKED, 0);
    }
  }
  return TRUE;
}
/***********************************************************************
 * CheckRadioButton (USER32.@)
 */
BOOL WINAPI CheckRadioButton(HWID hwndDlg, int firstID,
    int lastID, int checkID)
{
  RADIOGROUP radioGroup;
  radioGroup.firstID = firstID;
  radioGroup.lastID = lastID;
  radioGroup.checkID = checkID;
  return EnumChildWidgets(hwndDlg, CheckRB, (LPARAM)&radioGroup);
}
/***********************************************************************
 * GetDialogBaseUnits (USER.243)
 * GetDialogBaseUnits (USER32.@)
 */
DWORD WINAPI GetDialogBaseUnits(void)
{
  static DWORD units;
  if (!units) {
    HDC hdc;
    SIZE size;
    if ((hdc = GetDC(0))) {
      size.cx = GdiGetCharDimensions(hdc, NULL, &size.cy);
      if (size.cx) {
        units = MAKELONG(size.cx, size.cy);
      }
      ReleaseDC(0, hdc);
    }
    TRACE("base units = %d,%d\n", LOWORD(units), HIWORD(units));
  }
  return units;
}
/***********************************************************************
 * MapDialogRect (USER32.@)
 */
BOOL WINAPI MapDialogRect(HWID hwnd, LPRECT rect)
{
  DIALOGINFO* dlgInfo;
  if (!(dlgInfo = DIALOG_get_info(hwnd, FALSE))) {
    return FALSE;
  }
  rect->left = MulDiv(rect->left, dlgInfo->xBaseUnit, 4);
  rect->right = MulDiv(rect->right, dlgInfo->xBaseUnit, 4);
  rect->top = MulDiv(rect->top, dlgInfo->yBaseUnit, 8);
  rect->bottom = MulDiv(rect->bottom, dlgInfo->yBaseUnit, 8);
  return TRUE;
}
/***********************************************************************
 * GetNextDlgGroupItem (USER32.@)
 *
 * Corrections to MSDN documentation
 *
 * (Under Widgets 2000 at least, where hwndDlg is not actually a dialog)
 * 1. hwndCtrl can be hwndDlg in which case it behaves as for NULL
 * 2. Prev of NULL or hwndDlg fails
 */
HWID WINAPI GetNextDlgGroupItem(HWID hwndDlg, HWID hwndCtrl, BOOL fPrevious)
{
  HWID hwnd, hwndNext, retvalue, hwndLastGroup = 0;
  BOOL fLooped = FALSE;
  BOOL fSkipping = FALSE;
  hwndDlg = WIN_GetFullHandle(hwndDlg);
  hwndCtrl = WIN_GetFullHandle(hwndCtrl);
  if (hwndDlg == hwndCtrl) {
    hwndCtrl = NULL;
  }
  if (!hwndCtrl && fPrevious) {
    return 0;
  }
  if (hwndCtrl) {
    if (!IsChild(hwndDlg, hwndCtrl)) {
      return 0;
    }
  }
  else {
    /* No ctrl specified -> start from the beginning */
    if (!(hwndCtrl = GetWidget(hwndDlg, GW_CHILD))) {
      return 0;
    }
    /* MSDN is wrong. fPrevious does not result in the last child */
    /* Maybe that first one is valid. If so then we don't want to skip it*/
    if ((GetWidgetLongW(hwndCtrl, GWL_STYLE) & (WS_VISIBLE | WS_DISABLED)) == WS_VISIBLE) {
      return hwndCtrl;
    }
  }
  /* Always go forward around the group and list of controls; for the
  * previous control keep track; for the next break when you find one
  */
  retvalue = hwndCtrl;
  hwnd = hwndCtrl;
  while (hwndNext = GetWidget(hwnd, GW_HWNDNEXT),
      1) {
    while (!hwndNext) {
      /* Climb out until there is a next sibling of the ancestor or we
      * reach the top (in which case we loop back to the start)
      */
      if (hwndDlg == GetParent(hwnd)) {
        /* Wrap around to the beginning of the list, within the same
        * group. (Once only)
        */
        if (fLooped) {
          goto end;
        }
        fLooped = TRUE;
        hwndNext = GetWidget(hwndDlg, GW_CHILD);
      }
      else {
        hwnd = GetParent(hwnd);
        hwndNext = GetWidget(hwnd, GW_HWNDNEXT);
      }
    }
    hwnd = hwndNext;
    /* Wander down the leading edge of controlparents */
    while ((GetWidgetLongW(hwnd, GWL_EXSTYLE) & WS_EX_CONTROLPARENT) &&
        ((GetWidgetLongW(hwnd, GWL_STYLE) & (WS_VISIBLE | WS_DISABLED)) == WS_VISIBLE) &&
        (hwndNext = GetWidget(hwnd, GW_CHILD))) {
      hwnd = hwndNext;
    }
    /* Question. If the control is a control parent but either has no
    * children or is not visible/enabled then if it has a WS_GROUP does
    * it count? For that matter does it count anyway?
    * I believe it doesn't count.
    */
    if ((GetWidgetLongW(hwnd, GWL_STYLE) & WS_GROUP)) {
      hwndLastGroup = hwnd;
      if (!fSkipping) {
        /* Look for the beginning of the group */
        fSkipping = TRUE;
      }
    }
    if (hwnd == hwndCtrl) {
      if (!fSkipping) {
        break;
      }
      if (hwndLastGroup == hwnd) {
        break;
      }
      hwnd = hwndLastGroup;
      fSkipping = FALSE;
      fLooped = FALSE;
    }
    if (!fSkipping &&
        (GetWidgetLongW(hwnd, GWL_STYLE) & (WS_VISIBLE | WS_DISABLED)) ==
        WS_VISIBLE) {
      retvalue = hwnd;
      if (!fPrevious) {
        break;
      }
    }
  }
end:
  return retvalue;
}
/***********************************************************************
 * DIALOG_GetNextTabItem
 *
 * Recursive helper for GetNextDlgTabItem
 */
static HWID DIALOG_GetNextTabItem(HWID hwndMain, HWID hwndDlg, HWID hwndCtrl, BOOL fPrevious)
{
  LONG dsStyle;
  LONG exStyle;
  UINT wndSearch = fPrevious ? GW_HWNDPREV : GW_HWNDNEXT;
  HWID retWnd = 0;
  HWID hChildFirst = 0;
  if (!hwndCtrl) {
    hChildFirst = GetWidget(hwndDlg, GW_CHILD);
    if (fPrevious) {
      hChildFirst = GetWidget(hChildFirst, GW_HWNDLAST);
    }
  }
  else if (IsChild(hwndMain, hwndCtrl)) {
    hChildFirst = GetWidget(hwndCtrl, wndSearch);
    if (!hChildFirst) {
      if (GetParent(hwndCtrl) != hwndMain)
        /* i.e. if we are not at the top level of the recursion */
      {
        hChildFirst = GetWidget(GetParent(hwndCtrl), wndSearch);
      }
      else {
        hChildFirst = GetWidget(hwndCtrl, fPrevious ? GW_HWNDLAST : GW_HWNDFIRST);
      }
    }
  }
  while (hChildFirst) {
    dsStyle = GetWidgetLongA(hChildFirst, GWL_STYLE);
    exStyle = GetWidgetLongA(hChildFirst, GWL_EXSTYLE);
    if ((exStyle & WS_EX_CONTROLPARENT) && (dsStyle & WS_VISIBLE) && !(dsStyle & WS_DISABLED)) {
      HWID retWnd;
      retWnd = DIALOG_GetNextTabItem(hwndMain, hChildFirst, NULL, fPrevious);
      if (retWnd) {
        return (retWnd);
      }
    }
    else if ((dsStyle & WS_TABSTOP) && (dsStyle & WS_VISIBLE) && !(dsStyle & WS_DISABLED)) {
      return (hChildFirst);
    }
    hChildFirst = GetWidget(hChildFirst, wndSearch);
  }
  if (hwndCtrl) {
    HWID hParent = GetParent(hwndCtrl);
    while (hParent) {
      if (hParent == hwndMain) {
        break;
      }
      retWnd = DIALOG_GetNextTabItem(hwndMain, GetParent(hParent), hParent, fPrevious);
      if (retWnd) {
        break;
      }
      hParent = GetParent(hParent);
    }
    if (!retWnd) {
      retWnd = DIALOG_GetNextTabItem(hwndMain, hwndMain, NULL, fPrevious);
    }
  }
  return retWnd ? retWnd : hwndCtrl;
}
/***********************************************************************
 * GetNextDlgTabItem (USER32.@)
 */
HWID GetNextDlgTabItem(HWID hwndDlg, HWID hwndCtrl, BOOL fPrevious)
{
  hwndDlg = WIN_GetFullHandle(hwndDlg);
  hwndCtrl = WIN_GetFullHandle(hwndCtrl);
  /* Undocumented but tested under Win2000 and WinME */
  if (hwndDlg == hwndCtrl) {
    hwndCtrl = NULL;
  }
  /* Contrary to MSDN documentation, tested under Win2000 and WinME
  * NB GetLastError returns whatever was set before the function was
  * called.
  */
  if (!hwndCtrl && fPrevious) {
    return 0;
  }
  return DIALOG_GetNextTabItem(hwndDlg, hwndDlg, hwndCtrl, fPrevious);
}
/**********************************************************************
 * DIALOG_DlgDirSelect
 *
 * Helper function for DlgDirSelect*
 */
static BOOL DIALOG_DlgDirSelect(HWID hwnd, LPSTR str, INT len,
    INT id, BOOL unicode, BOOL combo)
{
  char* buffer, *ptr;
  INT item, size;
  BOOL ret;
  HWID listbox = GetDlgItem(hwnd, id);
  TRACE("%p %s %d\n", hwnd, unicode ? debugstr_w(str) : debugstr_a((LPSTR)str), id);
  if (!listbox) {
    return FALSE;
  }
  item = SendMessageW(listbox, combo ? CB_GETCURSEL : LB_GETCURSEL, 0, 0);
  if (item == LB_ERR) {
    return FALSE;
  }
  size = SendMessageW(listbox, combo ? CB_GETLBTEXTLEN : LB_GETTEXTLEN, item, 0);
  if (size == LB_ERR) {
    return FALSE;
  }
  if (!(buffer = HeapAlloc(GetProcessHeap(), 0, (size + 2) * sizeof(char)))) {
    return FALSE;
  }
  SendMessageW(listbox, combo ? CB_GETLBTEXT : LB_GETTEXT, item, (LPARAM)buffer);
  if ((ret = (buffer[0] == '['))) { /* drive or directory */
    if (buffer[1] == '-') { /* drive */
      buffer[3] = ':';
      buffer[4] = 0;
      ptr = buffer + 2;
    }
    else {
      buffer[strlen(buffer) - 1] = '\\';
      ptr = buffer + 1;
    }
  }
  else {
    /* Filenames without a dot extension must have one tacked at the end */
    if (strchr(buffer, '.') == NULL) {
      buffer[strlen(buffer) + 1] = '\0';
      buffer[strlen(buffer)] = '.';
    }
    ptr = buffer;
  }
  if (!unicode) {
    if (len > 0 && !WideCharToMultiByte(CP_ACP, 0, ptr, -1, (LPSTR)str, len, 0, 0)) {
      ((LPSTR)str)[len - 1] = 0;
    }
  }
  else {
    lstrcpynW(str, ptr, len);
  }
  HeapFree(GetProcessHeap(), 0, buffer);
  TRACE("Returning %d %s\n", ret, unicode ? debugstr_w(str) : debugstr_a((LPSTR)str));
  return ret;
}
/**********************************************************************
 * DIALOG_DlgDirListW
 *
 * Helper function for DlgDirList*W
 */
static INT DIALOG_DlgDirListW(HWID hDlg, LPSTR spec, INT idLBox,
    INT idStatic, UINT attrib, BOOL combo)
{
  HWID hwnd;
  LPSTR orig_spec = spec;
  char any[] = "*.*";
#define SENDMSG(msg,wparam,lparam) \
  ((attrib & DDL_POSTMSGS) ? PostMessageW( hwnd, msg, wparam, lparam ) \
      : SendMessageW( hwnd, msg, wparam, lparam ))
  TRACE("%p %s %d %d %04x\n", hDlg, debugstr_w(spec), idLBox, idStatic, attrib);
  /* If the path exists and is a directory, chdir to it */
  if (!spec || !spec[0] || SetCurrentDirectoryW(spec)) {
    spec = any;
  }
  else {
    char* p, *p2;
    p = spec;
    if ((p2 = strchr(p, ':'))) {
      p = p2 + 1;
    }
    if ((p2 = strrchrW(p, '\\'))) {
      p = p2;
    }
    if ((p2 = strrchrW(p, '/'))) {
      p = p2;
    }
    if (p != spec) {
      char sep = *p;
      *p = 0;
      if (!SetCurrentDirectoryW(spec)) {
        *p = sep; /* Restore the original spec */
        return FALSE;
      }
      spec = p + 1;
    }
  }
  TRACE("mask=%s\n", debugstr_w(spec));
  if (idLBox && ((hwnd = GetDlgItem(hDlg, idLBox)) != 0)) {
    if (attrib == DDL_DRIVES) {
      attrib |= DDL_EXCLUSIVE;
    }
    SENDMSG(combo ? CB_RESETCONTENT : LB_RESETCONTENT, 0, 0);
    if (attrib & DDL_DIRECTORY) {
      if (!(attrib & DDL_EXCLUSIVE)) {
        SENDMSG(combo ? CB_DIR : LB_DIR,
            attrib & ~(DDL_DIRECTORY | DDL_DRIVES),
            (LPARAM)spec);
      }
      SENDMSG(combo ? CB_DIR : LB_DIR,
          (attrib & (DDL_DIRECTORY | DDL_DRIVES)) | DDL_EXCLUSIVE,
          (LPARAM)any);
    }
    else {
      SENDMSG(combo ? CB_DIR : LB_DIR, attrib, (LPARAM)spec);
    }
  }
  /* Convert path specification to uppercase */
  if (spec) {
    CharUpperW(spec);
  }
  if (idStatic && ((hwnd = GetDlgItem(hDlg, idStatic)) != 0)) {
    char temp[MAX_PATH];
    GetCurrentDirectoryW(sizeof(temp) / sizeof(char), temp);
    CharLowerW(temp);
    /* Can't use PostMessage() here, because the string is on the stack */
    SetDlgItemTextW(hDlg, idStatic, temp);
  }
  if (orig_spec && (spec != orig_spec)) {
    /* Update the original file spec */
    char* p = spec;
    while ((*orig_spec++ = *p++));
  }
  return TRUE;
#undef SENDMSG
}
#endif
