# VC对话框child、popup模式之间切换 - 深之JohnChen的专栏 - CSDN博客

2018年11月12日 14:47:21[byxdaz](https://me.csdn.net/byxdaz)阅读数：215


       在WM_CREATE消息事件OnCreate(LPCREATESTRUCT lpCreateStruct)函数中判断窗口模式，进行切换窗口模式。

窗口对话框模式，先设置窗口资源属性，style设置成child、border设置成none。

窗口child模式，先设置窗口资源属性，style设置成popup、border设置成对话框外框。 

1、该类头文件中增加以下属性和方法

```
bool m_bIsChildWindow;            //是否为子窗口    
HWND m_hParentWnd;    
char m_szTitle[256];    
void SetChildWindow(bool bIsChildWindow,HWND hParentWnd,char *pTitle=NULL);
```

    该类实现文件中增加以下属性和方法

```
CXXXDialog::CXXXDialog()
	{
		m_bIsChildWindow = true;
		m_hParentWnd = NULL;
		memset(m_szTitle, 0, sizeof(m_szTitle));
	}
	
void CXXXDialog::SetChildWindow(bool bIsChildWindow, HWND hParentWnd,char *pTitle)
{
	m_bIsChildWindow = bIsChildWindow;
	m_hParentWnd = hParentWnd;
	if (pTitle != NULL)
	{
		strcpy_s(m_szTitle, pTitle);
	}
}
```

2、添加WM_CREATE消息事件

```
int CXXXDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	DWORD dwCurrentStyle = GetStyle();
	if ((dwCurrentStyle & WS_CHILDWINDOW) > 0)
	{
		if (!m_bIsChildWindow)
		{
			ModifyStyle(WS_CHILDWINDOW, WS_POPUPWINDOW | WS_DLGFRAME);
			SetParent(NULL);
			SetWindowText(m_szTitle);
		}
	}
	else
	{
		if (m_bIsChildWindow)
		{
			ModifyStyle(WS_POPUPWINDOW | WS_CAPTION, 0);
			ModifyStyleEx(WS_EX_DLGMODALFRAME, 0);
			CWnd * hWndParent = CWnd::FromHandle(m_hParentWnd);
			SetParent(hWndParent);
		}
		else
		{
			if (strlen(m_szTitle) > 0)
			{
				SetWindowText(m_szTitle);
			}
		}
	}

	return 0;
}
```

3、调用方法

1）、popup模式示例

```
CAttributeDialog dlg;
	dlg.SetChildWindow(false,NULL,"popup");
	dlg.DoModal();
```

2）、child模式示例

```
CRect rectTab;
	m_tabCtrl.GetClientRect(&rectTab);
	//添加控件按钮名称
	m_tabCtrl.InsertItem(0, _T("1"));
	m_tabCtrl.InsertItem(1, _T("2"));
	//对话框页面
	m_dlgAttribute.SetChildWindow(true, GetDlgItem(IDC_TAB1)->GetSafeHwnd());
	m_dlgAttribute.Create(IDD_DIALOG1, GetDlgItem(IDC_TAB1));
	//显示第一个页面
	m_dlgAttribute.ShowWindow(SW_SHOW);
	m_tabCtrl.SetCurSel(0);
	m_dlgAttribute.MoveWindow(rectTab.left, rectTab.top+30, rectTab.Width(), rectTab.Height()-30);
```

