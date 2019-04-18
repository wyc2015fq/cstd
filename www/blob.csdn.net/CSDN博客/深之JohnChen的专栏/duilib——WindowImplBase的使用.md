# duilib——WindowImplBase的使用 - 深之JohnChen的专栏 - CSDN博客

2017年06月03日 20:04:10[byxdaz](https://me.csdn.net/byxdaz)阅读数：2140
个人分类：[duilib](https://blog.csdn.net/byxdaz/article/category/8858604)



在我们的程序中不再继承CWindowWnd和INotifyUI这两个类，而是直接继承WindowImplBase，重写父类的GetWindowClassName、GetClassStyle、GetSkinFile、Notify四个函数即可。

WindowImplBase类

```cpp
//通用窗口类WindowImplBase封装

#include "stdafx.h"

#include <string>
#if (defined(_MBCS) || defined(MBCS))			//多字节
typedef std::string tString;
#else											//unicode
typedef std::wstring tString;
#endif

class WindowImplBase : public CWindowWnd, public INotifyUI, public IMessageFilterUI, public IDialogBuilderCallback
{
public:
	WindowImplBase();
	virtual ~WindowImplBase();

	virtual void OnFinalMessage(HWND hWnd);

	virtual UINT GetClassStyle() const;

	virtual void Init();

	virtual CControlUI* CreateControl(LPCTSTR pstrClass);

	virtual LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, bool& bHandled);

	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

#if defined(WIN32) && !defined(UNDER_CE)
	virtual LRESULT OnNcActivate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnNcCalcSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnNcPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
#endif

	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);    

	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
	virtual tString GetSkinFolder();
	virtual tString GetSkinFile() = 0;

	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);

protected:
	CPaintManagerUI		m_PaintManager;
};
```

```cpp
#include "WindowImplBase.h"

WindowImplBase::WindowImplBase()
{}

WindowImplBase::~WindowImplBase()
{}

UINT WindowImplBase::GetClassStyle() const
{
    return CS_DBLCLKS;
}

CControlUI* WindowImplBase::CreateControl(LPCTSTR pstrClass)
{
    return NULL;
}

void WindowImplBase::OnFinalMessage(HWND /*hWnd*/)
{
    m_PaintManager.RemovePreMessageFilter(this);
    m_PaintManager.RemoveNotifier(this);
    m_PaintManager.ReapObjects(m_PaintManager.GetRoot());
}

void WindowImplBase::Init()
{}

LRESULT WindowImplBase::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = FALSE;
    return 0;
}

LRESULT WindowImplBase::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = FALSE;
    return 0;
}

#if defined(WIN32) && !defined(UNDER_CE)
LRESULT WindowImplBase::OnNcActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
    if( ::IsIconic(*this) ) bHandled = FALSE;
    return (wParam == 0) ? TRUE : FALSE;
}

LRESULT WindowImplBase::OnNcCalcSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return 0;
}

LRESULT WindowImplBase::OnNcPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    return 0;
}

LRESULT WindowImplBase::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    POINT pt; pt.x = GET_X_LPARAM(lParam); pt.y = GET_Y_LPARAM(lParam);
    ::ScreenToClient(*this, &pt);

    RECT rcClient;
    ::GetClientRect(*this, &rcClient);
    if( !::IsZoomed(*this) ) {
        RECT rcSizeBox = m_PaintManager.GetSizeBox();
        if( pt.y < rcClient.top + rcSizeBox.top ) {
            if( pt.x < rcClient.left + rcSizeBox.left ) return HTTOPLEFT;
            if( pt.x > rcClient.right - rcSizeBox.right ) return HTTOPRIGHT;
            return HTTOP;
        }
        else if( pt.y > rcClient.bottom - rcSizeBox.bottom ) {
            if( pt.x < rcClient.left + rcSizeBox.left ) return HTBOTTOMLEFT;
            if( pt.x > rcClient.right - rcSizeBox.right ) return HTBOTTOMRIGHT;
            return HTBOTTOM;
        }
        if( pt.x < rcClient.left + rcSizeBox.left ) return HTLEFT;
        if( pt.x > rcClient.right - rcSizeBox.right ) return HTRIGHT;
    }

    RECT rcCaption = m_PaintManager.GetCaptionRect();
    if( pt.x >= rcClient.left + rcCaption.left && pt.x < rcClient.right - rcCaption.right \
        && pt.y >= rcCaption.top && pt.y < rcCaption.bottom ) {
            CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
            if( pControl && _tcsicmp(pControl->GetClass(), _T("ButtonUI")) != 0 && 
                _tcsicmp(pControl->GetClass(), _T("OptionUI")) != 0 /*&&
                _tcsicmp(pControl->GetClass(), _T("TextUI")) != 0 */)
                return HTCAPTION;
    }

    return HTCLIENT;
}

LRESULT WindowImplBase::OnGetMinMaxInfo(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    MONITORINFO oMonitor = {};
    oMonitor.cbSize = sizeof(oMonitor);
    ::GetMonitorInfo(::MonitorFromWindow(*this, MONITOR_DEFAULTTOPRIMARY), &oMonitor);
    CDuiRect rcWork = oMonitor.rcWork;
    rcWork.Offset(-rcWork.left, -rcWork.top);

    LPMINMAXINFO lpMMI = (LPMINMAXINFO) lParam;
    lpMMI->ptMaxPosition.x  = rcWork.left;
    lpMMI->ptMaxPosition.y  = rcWork.top;
    lpMMI->ptMaxSize.x      = rcWork.right;
    lpMMI->ptMaxSize.y      = rcWork.bottom;

    bHandled = FALSE;
    return 0;
}

LRESULT WindowImplBase::OnMouseWheel(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = FALSE;
    return 0;
}
#endif

LRESULT WindowImplBase::OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = FALSE;
    return 0;
}

LRESULT WindowImplBase::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = FALSE;
    return 0;
}

LRESULT WindowImplBase::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = FALSE;
    return 0;
}

LRESULT WindowImplBase::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = FALSE;
    return 0;
}

LRESULT WindowImplBase::OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = FALSE;
    return 0;
}

LRESULT WindowImplBase::OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
    bHandled = FALSE;
    return 0;
}

LRESULT WindowImplBase::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    SIZE szRoundCorner = m_PaintManager.GetRoundCorner();
#if defined(WIN32) && !defined(UNDER_CE)
    if( !::IsIconic(*this) && (szRoundCorner.cx != 0 || szRoundCorner.cy != 0) ) {
        CDuiRect rcWnd;
        ::GetWindowRect(*this, &rcWnd);
        rcWnd.Offset(-rcWnd.left, -rcWnd.top);
        rcWnd.right++; rcWnd.bottom++;
        HRGN hRgn = ::CreateRoundRectRgn(rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom, szRoundCorner.cx, szRoundCorner.cy);
        ::SetWindowRgn(*this, hRgn, TRUE);
        ::DeleteObject(hRgn);
    }
#endif
    bHandled = FALSE;
    return 0;
}

LRESULT WindowImplBase::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if (wParam == SC_CLOSE){
        bHandled = TRUE;
        return 0;
    }
#if defined(WIN32) && !defined(UNDER_CE)
    BOOL bZoomed = ::IsZoomed(*this);
    LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    if( ::IsZoomed(*this) != bZoomed ){
		CControlUI* pbtnMax     = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("MaxBtn")));       // 最大化按钮
		CControlUI* pbtnRestore = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("RestoreBtn")));   // 还原按钮

		// 切换最大化按钮和还原按钮的状态
		if (pbtnMax && pbtnRestore)
		{
			pbtnMax->SetVisible(TRUE == bZoomed);       // 此处用表达式是为了避免编译器BOOL转换的警告
			pbtnRestore->SetVisible(FALSE == bZoomed);
		}

    }
#else
    LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
#endif
    return lRes;
}

tString WindowImplBase::GetSkinFolder()
{
    return tString(CPaintManagerUI::GetInstancePath()); //+ _T("skin\\");
}

LRESULT WindowImplBase::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    LONG styleValue = ::GetWindowLong(*this, GWL_STYLE);
    styleValue &= ~WS_CAPTION;
    ::SetWindowLong(*this, GWL_STYLE, styleValue | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
    RECT rcClient;
    ::GetClientRect(*this, &rcClient);
    ::SetWindowPos(*this, NULL, rcClient.left, rcClient.top, rcClient.right - rcClient.left, \
        rcClient.bottom - rcClient.top, SWP_FRAMECHANGED);

    m_PaintManager.Init(m_hWnd);
    m_PaintManager.AddPreMessageFilter(this);

    CDialogBuilder builder;
    CStdString strResourcePath = m_PaintManager.GetResourcePath();
    if (strResourcePath.IsEmpty())
    {
	strResourcePath = m_PaintManager.GetInstancePath();
	strResourcePath +=GetSkinFolder().data();
     }
     m_PaintManager.SetResourcePath(strResourcePath.GetData());
     tString tstrSkin = GetSkinFile();
     //builder.Create函数的第一个参数为：xml布局文件，函数里面处理时会加上GetResourcePath()路径,所以只传tstrSkin.c_str()就可以了，在这里加路径反而会报错。
    CControlUI* pRoot = builder.Create(tstrSkin.c_str(), (UINT)0, this, &m_PaintManager);
    m_PaintManager.AttachDialog(pRoot);
    m_PaintManager.AddNotifier(this);
    Init();
    return 0;
}

LRESULT WindowImplBase::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    return 0;
}

LRESULT WindowImplBase::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRes = 0;
    BOOL bHandled = TRUE;
    switch (uMsg)
    {
    case WM_CREATE:         lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
    case WM_CLOSE:          lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
    case WM_DESTROY:        lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
#if defined(WIN32) && !defined(UNDER_CE)
    case WM_NCACTIVATE:     lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
    case WM_NCCALCSIZE:     lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
    case WM_NCPAINT:        lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
    case WM_NCHITTEST:      lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
    case WM_GETMINMAXINFO:  lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
    case WM_MOUSEWHEEL:     lRes = OnMouseWheel(uMsg, wParam, lParam, bHandled); break;
#endif
    case WM_SIZE:           lRes = OnSize(uMsg, wParam, lParam, bHandled); break;   
    case WM_SYSCOMMAND:     lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
    case WM_KEYDOWN:        lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
    case WM_KILLFOCUS:      lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
    case WM_SETFOCUS:       lRes = OnSetFocus(uMsg, wParam, lParam, bHandled); break;
    case WM_LBUTTONUP:      lRes = OnLButtonUp(uMsg, wParam, lParam, bHandled); break;
    case WM_LBUTTONDOWN:    lRes = OnLButtonDown(uMsg, wParam, lParam, bHandled); break;
    case WM_MOUSEMOVE:      lRes = OnMouseMove(uMsg, wParam, lParam, bHandled); break;
    default:                bHandled = FALSE; break;
    }

    if (bHandled) return lRes;

    lRes = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
    if (bHandled) return lRes;

    if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT WindowImplBase::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/)
{
    if (uMsg == WM_KEYDOWN)
    {
        switch (wParam)
        {
        case VK_RETURN:
        case VK_ESCAPE:
			return FALSE;//拦截ESC退出界面
            //return ResponseDefaultKeyEvent(wParam);
        default:
            break;
        }
    }
    return FALSE;
}

LRESULT WindowImplBase::ResponseDefaultKeyEvent(WPARAM wParam)
{
    if (wParam == VK_RETURN)
    {
        return FALSE;
    }
    else if (wParam == VK_ESCAPE)
    {
        Close();
        return TRUE;
    }

    return FALSE;
}
```
	strResourcePath = m_PaintManager.GetInstancePath();
	strResourcePath +=GetSkinFolder().data();
     }
     m_PaintManager.SetResourcePath(strResourcePath.GetData());
     tString tstrSkin = GetSkinFile();
     //builder.Create函数的第一个参数为：xml布局文件，函数里面处理时会加上GetResourcePath()路径,所以只传tstrSkin.c_str()就可以了，在这里加路径反而会报错。
    CControlUI* pRoot = builder.Create(tstrSkin.c_str(), (UINT)0, this, &m_PaintManager);
    m_PaintManager.AttachDialog(pRoot);
    m_PaintManager.AddNotifier(this);
    Init();
    return 0;
}

LRESULT WindowImplBase::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    return 0;
}

LRESULT WindowImplBase::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lRes = 0;
    BOOL bHandled = TRUE;
    switch (uMsg)
    {
    case WM_CREATE:         lRes = OnCreate(uMsg, wParam, lParam, bHandled); break;
    case WM_CLOSE:          lRes = OnClose(uMsg, wParam, lParam, bHandled); break;
    case WM_DESTROY:        lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
#if defined(WIN32) && !defined(UNDER_CE)
    case WM_NCACTIVATE:     lRes = OnNcActivate(uMsg, wParam, lParam, bHandled); break;
    case WM_NCCALCSIZE:     lRes = OnNcCalcSize(uMsg, wParam, lParam, bHandled); break;
    case WM_NCPAINT:        lRes = OnNcPaint(uMsg, wParam, lParam, bHandled); break;
    case WM_NCHITTEST:      lRes = OnNcHitTest(uMsg, wParam, lParam, bHandled); break;
    case WM_GETMINMAXINFO:  lRes = OnGetMinMaxInfo(uMsg, wParam, lParam, bHandled); break;
    case WM_MOUSEWHEEL:     lRes = OnMouseWheel(uMsg, wParam, lParam, bHandled); break;
#endif
    case WM_SIZE:           lRes = OnSize(uMsg, wParam, lParam, bHandled); break;   
    case WM_SYSCOMMAND:     lRes = OnSysCommand(uMsg, wParam, lParam, bHandled); break;
    case WM_KEYDOWN:        lRes = OnKeyDown(uMsg, wParam, lParam, bHandled); break;
    case WM_KILLFOCUS:      lRes = OnKillFocus(uMsg, wParam, lParam, bHandled); break;
    case WM_SETFOCUS:       lRes = OnSetFocus(uMsg, wParam, lParam, bHandled); break;
    case WM_LBUTTONUP:      lRes = OnLButtonUp(uMsg, wParam, lParam, bHandled); break;
    case WM_LBUTTONDOWN:    lRes = OnLButtonDown(uMsg, wParam, lParam, bHandled); break;
    case WM_MOUSEMOVE:      lRes = OnMouseMove(uMsg, wParam, lParam, bHandled); break;
    default:                bHandled = FALSE; break;
    }

    if (bHandled) return lRes;

    lRes = HandleCustomMessage(uMsg, wParam, lParam, bHandled);
    if (bHandled) return lRes;

    if (m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes)) return lRes;
    return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
}

LRESULT WindowImplBase::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, bool& /*bHandled*/)
{
    if (uMsg == WM_KEYDOWN)
    {
        switch (wParam)
        {
        case VK_RETURN:
        case VK_ESCAPE:
			return FALSE;//拦截ESC退出界面
            //return ResponseDefaultKeyEvent(wParam);
        default:
            break;
        }
    }
    return FALSE;
}

LRESULT WindowImplBase::ResponseDefaultKeyEvent(WPARAM wParam)
{
    if (wParam == VK_RETURN)
    {
        return FALSE;
    }
    else if (wParam == VK_ESCAPE)
    {
        Close();
        return TRUE;
    }

    return FALSE;
}

//stdafx.h中引用duilib头文件

```cpp
#include "COMUTIL.H"
#include <UIlib.h>
using namespace DuiLib;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_ud.lib")
#   else
#       pragma comment(lib, "DuiLib_d.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "DuiLib_u.lib")
#   else
#       pragma comment(lib, "DuiLib.lib")
#   endif
#endif
```

//调用实例

```cpp
#include "stdafx.h"
#include "WindowImplBase.h"

class CMyDuiFrameWnd : public WindowImplBase
{
public:
	CMyDuiFrameWnd()
	{
		m_pRoot = NULL;
	};

	void OnFinalMessage(HWND /*hWnd*/)
	{
		delete this;
	};
	LPCTSTR GetWindowClassName() const
	{
		return "MyDuiFrameWnd";
	}

	UINT GetClassStyle() const{
		return UI_CLASSSTYLE_FRAME|CS_DBLCLKS;
	}

	tString GetSkinFolder()
	{
		return "";
	}

	tString GetSkinFile(){
		return "main.xml";		//布局文件
	}
	void Notify(TNotifyUI& msg)
	{
		if(msg.sType == "click")
		{
			if(msg.pSender->GetName() == "btnHello")
			{
				::MessageBox(NULL,_T("我是按钮"),_T("点击了按钮"),NULL);
			}
			else if(msg.pSender->GetName() == "MinBtn")
			{
				::SendMessage(m_hWnd,WM_SYSCOMMAND, SC_MINIMIZE, 0);
			}
			else if(msg.pSender->GetName() == "MaxBtn")
			{
				::SendMessage(m_hWnd,WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			}
			else if(msg.pSender->GetName() == "CloseBtn")
			{
				::PostQuitMessage(0);
			}
		}
	} 

protected:
	CControlUI *m_pRoot;
};

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	HRESULT Hr = ::CoInitialize(NULL);
	if(FAILED(Hr)) 
		return 0;

	CMyDuiFrameWnd* pFrame = new CMyDuiFrameWnd();
	if( pFrame == NULL ) 
		return 0;

	pFrame->Create(NULL, _T("简单测试demo"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame->CenterWindow();
	pFrame->ShowWindow(true);
	CPaintManagerUI::MessageLoop();
	::CoUninitialize();

	return 0;
}
```
	return "";
	}

	tString GetSkinFile(){
		return "main.xml";		//布局文件
	}
	void Notify(TNotifyUI& msg)
	{
		if(msg.sType == "click")
		{
			if(msg.pSender->GetName() == "btnHello")
			{
				::MessageBox(NULL,_T("我是按钮"),_T("点击了按钮"),NULL);
			}
			else if(msg.pSender->GetName() == "MinBtn")
			{
				::SendMessage(m_hWnd,WM_SYSCOMMAND, SC_MINIMIZE, 0);
			}
			else if(msg.pSender->GetName() == "MaxBtn")
			{
				::SendMessage(m_hWnd,WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			}
			else if(msg.pSender->GetName() == "CloseBtn")
			{
				::PostQuitMessage(0);
			}
		}
	} 

protected:
	CControlUI *m_pRoot;
};

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());
	HRESULT Hr = ::CoInitialize(NULL);
	if(FAILED(Hr)) 
		return 0;

	CMyDuiFrameWnd* pFrame = new CMyDuiFrameWnd();
	if( pFrame == NULL ) 
		return 0;

	pFrame->Create(NULL, _T("简单测试demo"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame->CenterWindow();
	pFrame->ShowWindow(true);
	CPaintManagerUI::MessageLoop();
	::CoUninitialize();

	return 0;
}



