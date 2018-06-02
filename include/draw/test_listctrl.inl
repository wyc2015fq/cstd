
typedef struct test_listctrl_t {
  uictrl base;
  UI_engine ui[1];
  list_ctrl m_list;
  //CMyFileListCtrl      m_fileListCtrl;
  //CMyFileListBox      m_fileListBox;
  //CCustomDrawComboBox    m_simpleCombo;
  //CMyFileComboBox      m_fileCombo1;  // dynamically create
  //CMyFileComboBox      m_fileCombo2;  // "subclass" create
  //CMyCustomDrawTreeCtrl  m_tree;
  //CFilterEdit        m_FilterEditCtrl;

  //CImageList        m_imageList;

  //CVComboBox        m_vComboBox;  // for registering the window struct only
  // otherwise this dialog won't show up when DERIVE_FROM_VCOMBO is turned on

  //CCustomDrawButton    m_btn1;
  //CCustomDrawButton    m_btn2;
} test_listctrl_t;

BOOL test_listctrl_init(test_listctrl_t* s)
{
  int ii;
  char strItem[256];
  s->m_list.InsertColumn(0, _T("Column1"));
  s->m_list.InsertColumn(1, _T("Column2"));

  for (ii = 0; ii < 10; ++ii) {
    _snprintf(strItem, 256, _T("Item %d"), ii);
    s->m_list.InsertItem(ii, strItem);

    _snprintf(strItem, 256, _T("Sub Item %d"), ii);
    s->m_list.SetItemText(ii, 1, strItem);

    //m_simpleCombo.AddString(strItem);
  }

  s->m_list.SetColumnWidth(0, LVSCW_AUTOSIZE);
  s->m_list.SetColumnWidth(1, LVSCW_AUTOSIZE);
  s->m_list.list_ctrl_hitrect(s->base.rc);

#if 0
  s->m_fileCombo1.CreateFromCtrl(this, IDC_VIRTUAL_FILECOMBO1, VCBS_DEFAULT_DROPDOWNLIST);

  s->m_fileListBox.CreateFromCtrl(this, IDC_FILELISTBOX, LBS_NODATA);

  DWORD dwExtStyle = s->m_list.GetExtendedStyle();
  //dwExtStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
  dwExtStyle |= LVS_EX_GRIDLINES;
  s->m_list.SetExtendedStyle(dwExtStyle);

  //   dwExtStyle = s->m_fileListCtrl.GetExtendedStyle();
  //   dwExtStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
  //   s->m_fileListCtrl.SetExtendedStyle(dwExtStyle);

  s->m_list.EnableCustomDraw();

  UINT nImageListFlags = ILC_COLOR32 | ILC_MASK;
  s->m_imageList.Create(16, 15, nImageListFlags, 0, 8);
  s->m_imageList.Add(m_hIcon);
  s->m_imageList.Add(m_hIcon);
  //m_list.SetImageList(&m_imageList, LVSIL_SMALL);

  HTREEITEM hRoot = s->m_tree.InsertItem(_T("C:\\Windows"));
  HTREEITEM h1 = s->m_tree.InsertItem(_T("Calc.exe"), hRoot);
  HTREEITEM h2 = s->m_tree.InsertItem(_T("System32"), hRoot);
  HTREEITEM h21 = s->m_tree.InsertItem(_T("kernel32.dll"), h2);
  HTREEITEM h22 = s->m_tree.InsertItem(_T("user32.dll"), h2);
  HTREEITEM h3 = s->m_tree.InsertItem(_T("explorer.exe"), hRoot);

  HTREEITEM hItem = s->m_tree.InsertItem(_T("A very very very very very very very very "
      "long tree item just to make the horizontal scrollbar appear"
                                           ), hRoot);

  for (ii = 0; ii < 20; ++ii) {
    CString strText;
    strText.Format(_T("KB%u.log"), 955000 + ii);
    hItem = s->m_tree.InsertItem(strText, hRoot);
  }

  s->m_tree.Expand(h2, TVE_EXPAND);
  s->m_tree.Expand(hRoot, TVE_EXPAND);
  //m_tree.SetItemHeight(31);

  s->m_fileCombo1.SetDroppedVisibleItemCount(1);

  s->m_fileListCtrl.SetFullRowSelect();
  s->m_fileListCtrl.SetDoubleBuffered();
  s->m_fileListCtrl.SetHeaderDragDrop();

#define STR_FILE_INCLUDE_PATTERN  _T("*.txt;*.bmp")
  //#define STR_FILE_INCLUDE_PATTERN  _T("*.*")

  TCHAR szPath[MAX_PATH] = {0};
  //GetCurrentDirectory(MAX_PATH, szPath);
  GetWindowsDirectory(szPath, MAX_PATH);
  //_tcscpy(szPath, _T("D:\\"));

  static EnumerateFolderArray folders;
  folders.push_back(EnumerateFolder(szPath, STR_FILE_INCLUDE_PATTERN));

  s->m_FileInfoHolder.SetEnumerateFolderArray(&folders);

  s->m_fileListCtrl.RefreshObjInfoList();

  s->m_controlAnchor.Init(this, 0);
  //m_controlAnchor.Add(m_fileListCtrl,      CtrlAnchorAll);
  //m_controlAnchor.Add(m_tree,          CtrlAnchorAllButLeft);
  s->m_controlAnchor.Add(m_fileListCtrl,      CtrlAnchorAllButRight);
  s->m_controlAnchor.Add(m_tree,          CtrlAnchorAll);
  s->m_controlAnchor.Add(m_FilterEditCtrl,    CtrlAnchorAllButTop);
  s->m_controlAnchor.Add(m_simpleCombo,      CtrlAnchorBottomRight);
  s->m_controlAnchor.Add(m_fileCombo1,      CtrlAnchorBottomRight);
  s->m_controlAnchor.Add(m_fileCombo2,      CtrlAnchorBottomRight);
  s->m_controlAnchor.Add(m_fileListBox,      CtrlAnchorAllButTop);
  s->m_controlAnchor.Add(m_list,          CtrlAnchorBottomRight);
  s->m_controlAnchor.Add(m_btn1,          CtrlAnchorBottomRight);
  s->m_controlAnchor.Add(m_btn2,          CtrlAnchorBottomRight);
  s->m_controlAnchor.Add(IDC_BUTTON3,      CtrlAnchorBottomRight);

#ifdef TEST_TOOLTIP_TRACKING
  // Initialize the tooltip
  s->m_tooltips.Create(this, TTS_NOPREFIX | TTS_ALWAYSTIP);
  s->m_tooltips.ModifyStyleEx(0, WS_EX_TOPMOST);
#ifdef MSVC_NEW_VER
  s->m_toolinfo.cbSize = sizeof(TOOLINFO);
#else
  // We have to set this value unless we use manifest file, otherwise the tooltip won't show at all
  s->m_toolinfo.cbSize = TTTOOLINFO_V2_SIZE;
#endif // MSVC_NEW_VER
  s->m_toolinfo.uFlags = TTF_IDISHWND | TTF_TRACK | TTF_ABSOLUTE;
  s->m_toolinfo.hwnd = s->m_hWnd;
  s->m_toolinfo.hinst = AfxGetInstanceHandle();
  s->m_toolinfo.lpszText = _T("");
  s->m_toolinfo.uId = (UINT_PTR)m_hWnd;
  GetClientRect(&m_toolinfo.rect);
  s->m_tooltips.SendMessage(TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&m_toolinfo);
#endif // TEST_TOOLTIP_TRACKING

#ifdef DO_DLG_ANIMATION
  CenterWindow();
  ORandomAnimateWindow(m_hWnd, 200);

  // comment out this line and you will see that the listctrl's header does not get paint well.
  RedrawWindow();
#endif // DO_DLG_ANIMATION

  s->m_FilterEditCtrl.SetPromptText(_T("enter search strings (e.g., ""sub string -excludeme"") (type \\ to search full path)"));

#endif
  //   ::SetDialogFocus(GetSafeHwnd(), s->m_FilterEditCtrl.GetSafeHwnd());
  //   return FALSE;  // return false since we want to set the focus to the edit control
  return TRUE;
}

int test_listctrl_paint_proc(test_listctrl_t* s, const event* e)
{
  UIHDC(e);
  draw_clear(hDC, _RGB(0, 0, 0));
  draw_clear(hDC, _RGB(255, 255, 255));
  //draw_uimdi(s->mdi, e);

  if (1) {
    //CCustomDrawHeaderCtrl hd;
    //hd.nColCount = 5;
    s->m_list.list_ctrl_paint(hDC);
  }

  //test_draweffects(hDC, s->base.rc);
  return 0;
}
int test_listctrl_frame_proc(test_listctrl_t* s, const event* e)
{
  UIEVT(e);

  switch (e->msg) {
  case WM_MOUSEMOVE:
  case EVENT_LCLICK:
    if (e->sender == s) {
    }

    break;

  case WM_SIZE:
    if (1) {
      s->m_list.list_ctrl_hitrect(s->base.rc);
    }

    break;

  case EVENT_EXIT:
    if (e->sender == s) {
      printf("EVENT_EXIT %x\n", e->sender);
      PostQuitMessage(0);
    }

    break;

  case EVENT_INIT:
    if (e->sender == s) {
      Win_MoveCenter(s->ui->hwnd, 800, 600);
    }

    break;
  }

  return 0;
}


int test_listctrl()
{
  test_listctrl_t s[1] = {0};

  UISETCALL(s, test_listctrl_paint_proc, test_listctrl_frame_proc);
  UI_set(s->ui, "test_listctrl", &s->base, 0);

#define RESDEF(type, id, str)  resload(#type, id, #str);
#include "test_listctrl_resdef.txt"
#undef RESDEF

  test_listctrl_init(s);
  waitkey(s->ui, -1);
  res_free();
  return 0;
}
