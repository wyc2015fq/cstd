# MFC与duilib之间控件相互调用 - 深之JohnChen的专栏 - CSDN博客

2017年06月04日 22:16:20[byxdaz](https://me.csdn.net/byxdaz)阅读数：2291


一、在MFC中使用duilib控件

将duilib的控件窗口的父窗口指定为MFC的窗口，同时在MFC中显示duilib的控件窗口。

duilib的控件窗口需要继承CWindowWnd, INotifyUI，实现GetWindowClassName、GetClassStyle、OnFinalMessage、Notify、HandleMessage等方法。

在MFC中以子窗口样式显示duilib的控件窗口。比如：

m_duiFrameWnd.Create(*this, NULL, UI_WNDSTYLE_CHILD, 0, 0, 0, 400, 300);  

m_duiFrameWnd.ShowWindow(TRUE); 

如何响应duilib控件事件，需要处理Notify函数。比如：

```cpp
//m_pDlgMain是MFC主窗口的指针。
void CDuiFrameWnd::Notify( TNotifyUI& msg )
{
    if (m_pDlgMain)
    {
        if( msg.sType == _T("click") ) 
        {
            if( msg.pSender->GetName() == _T("btnStart") ) 
            {
                m_pDlgMain->OnBnClickedButtonStart();
            }
            else if( msg.pSender->GetName() == _T("btnStop") ) 
            {
                m_pDlgMain->OnBnClickedButtonStop();
            }
        }
    }

    __super::Notify(msg);
}
```

二、在duilib中使用MFC控件

自定义一个duilib控件来容纳MFC控件，xml布局界面文件中使用该控件，使用非duilib控件来标记，比如：MyMfcWnd，非duilib控件需要自己在响应CreateControl创建，创建该控件时之后将MFC控件句柄绑定在一起（通过Attach）。

```
CControlUI* CDuiFrameWnd::CreateControl(LPCTSTR pstrClassName)
{
	if (_tcsicmp(pstrClassName, _T("MyWin32Wnd")) == 0)
	{
		//win32按钮
		CMfcWndUI *pUI   = new CMfcWndUI();            
		HWND    hWnd  = CreateWindow(_T("BUTTON"), _T("win32"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, 0, 0, 0, 0, m_PaintManager.GetPaintWindow(), NULL, NULL, NULL);
		pUI->Attach(hWnd);     
		return pUI;
	}
	else if (_tcsicmp(pstrClassName, _T("MyMfcWnd")) == 0)
	{
		//MFC按钮
		CMfcWndUI *pUI   = new CMfcWndUI();     
		m_pButton = new CButton();
		m_pButton->Create(_T("MFC"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON, CRect(0, 0, 0, 0), CWnd::FromHandle(m_PaintManager.GetPaintWindow()), MFC_BUTTON_ID);
		pUI->Attach(m_pButton->GetSafeHwnd());  
		return pUI;
	}

	return NULL;
}
```

//CMfcWndUI自定义MFC控件 

```
#pragma once

//自定义一个duilib控件来容纳MFC控件
class CMfcWndUI : public CControlUI
{
public:
	CMfcWndUI(void){m_hWnd = NULL;};
	~CMfcWndUI(void){};

	virtual void SetInternVisible(bool bVisible = true)
	{
		__super::SetInternVisible(bVisible);
		::ShowWindow(m_hWnd, bVisible);
	}

	virtual void SetPos(RECT rc)
	{
		__super::SetPos(rc);
		::SetWindowPos(m_hWnd, NULL, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER | SWP_NOACTIVATE);
	}

	BOOL Attach(HWND hWndNew)
	{
		if (! ::IsWindow(hWndNew))
		{
			return FALSE;
		}

		m_hWnd = hWndNew;
		return TRUE;
	}

	HWND Detach()
	{
		HWND hWnd = m_hWnd;
		m_hWnd = NULL;
		return hWnd;
	}

protected:
	HWND m_hWnd;
};
```

duilib中MFC控件的响应

win32是通过WM_COMMAND进行消息传递的，我们只需截获WM_COMMAND，而duilib的WM_COMMAND等windows消息可以在HandleMessage函数里截获，比如：



```cpp
LRESULT CDuiFrameWnd::HandleMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    switch (uMsg)
    {
    case WM_COMMAND:
        if (IDC_BUTTON_DUI_NEXT == wParam)
        {
            if (m_pDlgMain)
            {
                m_pDlgMain->OnBnClickedButtonSendMsg();
            }
        }
        break;
    default:
        break;
    }

    return __super::HandleMessage(uMsg, wParam, lParam);
}
```

![](https://img-blog.csdn.net/20170604221402812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYnl4ZGF6/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

完整实例代码：[http://download.csdn.net/detail/byxdaz/9860571](http://download.csdn.net/detail/byxdaz/9860571)



