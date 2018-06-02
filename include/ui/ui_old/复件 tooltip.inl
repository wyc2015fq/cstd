

//Tooltip工具提示控件的使用

//如何为一个控件创建一个工具提示控件How to Create a Tooltip for a Control

// 创建控件的Tooltip
HWND CreateToolTip(HWND hwndTool, HWND hwnd, PTSTR pszText)
{
  TOOLINFO toolInfo = {0};
  HWND hwndTip = CreateWindowEx(0,
      TOOLTIPS_CLASS,
      NULL,
      WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      CW_USEDEFAULT,
      hwnd,
      NULL,
      0,
      NULL);

  if (!hwndTool || !hwndTip) {
    return (HWND)NULL;
  }

  toolInfo.cbSize = sizeof(TOOLINFO);
  toolInfo.hwnd = hwnd;
  toolInfo.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
  toolInfo.uId = (UINT_PTR)hwndTool;
  toolInfo.lpszText = pszText;
  SendMessage(hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo);

  return hwndTip;
}

//3.在对话框OnInitDialog()函数添加代码：
//1
//      CreateToolTip(IDOK, hwnd, _T("这是按钮的ToolTip语句"));
//4.编译运行，鼠标移到对话框按钮上，暂停鼠标移动，截图如下：

//如何创建一个矩形区域的工具提示控件 How to Create a Tooltip for a Rectangular Area

//1.创建一个基于对话框的工程TestToolTip；
//2.在对话框实现文件写入以下代码：

// 创建矩形区域ToolTip
void CreateToolTipForRect(HWND hwnd)
{
  TOOLINFO ti = {0};
  HWND hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hwnd,
      NULL,
      0,
      NULL);

  SetWindowPos(hwndTT, HWND_TOPMOST, 0, 0, 0, 0,
      SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

  ti.cbSize = sizeof(TOOLINFO);
  ti.uFlags = TTF_SUBCLASS;
  ti.hwnd = hwnd;
  ti.hinst = 0;
  ti.lpszText = _T("这是你的ToolTip语句");

  GetWindowRect(hwnd, &ti.rect);
  //GetClientRect(hwnd, &ti.rect);

  SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
}

// 代码中，第9行的设置WS_POPUP风格，当CreateWindowEx一个Tooltip时，总是具有WS_POPUP和WS_EX_TOOLWINDOW风格，
// 不管在创建的时候你是否指定。TTS_NOPREFIX指示当光标在一个工具上时显示工具提示，不管工具提示的属主窗口是否是处于活动状态。
// 没有这个风格，则只有当工具的属主窗口是活动的时候才会显示工具提示控件，否则不显示。
// TTS_ALWAYSTIP这个风格禁止系统将&字符从一个字符串中去掉。如果一个工具提示控件没有TTS_NOPREFIX风格，
// 则系统自动去掉&字符，让应用程序用同一个字符串作为菜单项和工具提示控件中的文本。
// 3.在对话框OnInitDialog()函数添加代码：
//
//      CreateToolTipForRect(hwnd);    //对话框提示
//     CreateToolTipForRect(GetDlgItem(IDOK)->hwnd);    //“确定”按钮提示
//
// 4.编译运行，鼠标移到对话框上，暂停鼠标移动，截图如下：
//
// 如何实现跟踪工具提示控件 How to Implement Tracking Tooltips
// 1.创建一个基于对话框的工程TestToolTip；
// 2.在对话框实现文件写入以下代码：

TOOLINFO g_toolItem = {0};
int g_TrackingMouse = 0;
HWND g_hwndTrackingTT = 0;

HWND CreateTrackingToolTip(HWND hwnd)
{
  HWND hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL,
      WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, CW_USEDEFAULT, CW_USEDEFAULT,
      CW_USEDEFAULT, CW_USEDEFAULT, 0, NULL, 0,  NULL);

  if (!hwndTT) {
    return NULL;
  }

  g_hwndTrackingTT = hwndTT;

  g_toolItem.cbSize = sizeof(TOOLINFO);
  g_toolItem.uFlags = TTF_TRACK | TTF_ABSOLUTE | TTF_SUBCLASS;
  g_toolItem.hwnd = 0;
  g_toolItem.hinst = 0;
  g_toolItem.lpszText = " ";
  g_toolItem.uId = (UINT_PTR)0;

  GetClientRect(hwnd, &g_toolItem.rect);
  g_toolItem.rect.right /= 2;
  g_toolItem.rect.bottom /= 2;
  SendMessage(g_hwndTrackingTT, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&g_toolItem);

  //SendMessage(hwndTT, TTM_SETMAXTIPWIDTH, 0, 300);//情况3
  return hwndTT;
}

// 3.在对话框OnInitDialog()函数添加代码：
// 1
//      g_hwndTrackingTT = CreateTrackingToolTip(IDOK, hwnd, _T("ToolTip"));
// 4.添加对话框的WM_MOUSELEAVE事件，代码如下：
//  Description: 鼠标移出窗口，停用ToolTip
// void CTestToolTipDlg::OnMouseLeave()
// {
//     ::SendMessage(g_hwndTrackingTT, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&g_toolItem);
//     g_TrackingMouse = FALSE;
//     CDialog::OnMouseLeave();
// }
// 5.添加对话框的WM_MOUSEMOVE事件，代码如下：
//  Description: 鼠标移动，ToolTip更新信息
int tooltip_mouse(HWND hwnd, UINT msg, POINT pt)
{
  static int oldX, oldY;

  if (WM_MOUSELEAVE == msg) {
    SendMessage(g_hwndTrackingTT, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&g_toolItem);
    g_TrackingMouse = 0;
    return 0;
  }
  else if (WM_MOUSEMOVE == msg) {
    if (!g_TrackingMouse) {
      TRACKMOUSEEVENT tme = {sizeof(TRACKMOUSEEVENT)};
      tme.hwndTrack = hwnd;
      tme.dwFlags = TME_LEAVE;
      TrackMouseEvent(&tme);
      g_TrackingMouse = TRUE;
    }

    if (PtInRect(&g_toolItem.rect, pt)) {
      SendMessage(g_hwndTrackingTT, TTM_TRACKACTIVATE, (WPARAM)TRUE, (LPARAM)&g_toolItem);
    }
    else {
      SendMessage(g_hwndTrackingTT, TTM_TRACKACTIVATE, (WPARAM)FALSE, (LPARAM)&g_toolItem);
    }

    if ((pt.x != oldX) || (pt.y != oldY)) {
      char coords[12];
      oldX = pt.x, oldY = pt.y;
      _snprintf(coords, 12, _T("%d, %d"), pt.x, pt.y);
      g_toolItem.lpszText = coords;
      SendMessage(g_hwndTrackingTT, TTM_SETTOOLINFO, 0, (LPARAM)&g_toolItem);
      ClientToScreen(hwnd, &pt);
      SendMessage(g_hwndTrackingTT, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(pt.x + 10, pt.y - 20));
    }
  }

  return 0;
}
