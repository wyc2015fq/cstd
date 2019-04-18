# duilib——从XML文件中加载界面 - 深之JohnChen的专栏 - CSDN博客

2017年06月03日 18:22:35[byxdaz](https://me.csdn.net/byxdaz)阅读数：2250


Duilib支持xml界面布局文件，使得界面设计与逻辑处理相分离。

需要以下几个步骤：

1、创建并初始化CPaintManagerUI对象。

2、创建CDialogBuilder对象，调用CDialogBuilder对象的Create函数加载xml界面布局文件，该函数返回界面元素根节点。使用UIDesigner工具编写xml界面布局文件。

3、调用CPaintManagerUI的AttachDialog將界面元素根节点附加到CPaintManagerUI对象中。

实例：

//Helloworld.h

```cpp
#pragma once

#include "resource.h"

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


//Helloworld.cpp

```cpp
#include "Helloworld.h"

class CDuiFrameWnd : public CWindowWnd, public INotifyUI
{
public:
	CDuiFrameWnd()
	{
		m_pRoot = NULL;
		m_pHelloBtn = NULL;
	};
	virtual LPCTSTR GetWindowClassName() const { return _T("DUIMainFrame"); }
	UINT GetClassStyle() const { return UI_CLASSSTYLE_FRAME | CS_DBLCLKS; };
	void OnFinalMessage(HWND /*hWnd*/) { delete this; };

	virtual void    Notify(TNotifyUI & msg) 
	{
		if(msg.sType == _T("click"))
		{
			if (msg.pSender->GetName() == _T("btnHello"))
			{
				::MessageBox(NULL, _T("我是按钮"), _T("点击了按钮"), NULL);
			}
		}
	}

	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		LRESULT lRes = 0;

		if( uMsg == WM_CREATE ) 
		{
			m_PaintManager.Init(m_hWnd); 
			//从xml中加载界面
			CDialogBuilder builder;
			m_pRoot = builder.Create("main.xml",(UINT)0,NULL,&m_PaintManager);
			m_PaintManager.AttachDialog(m_pRoot); 
			m_PaintManager.AddNotifier(this);				//添加控件等消息响应，这样消息就会传达到duilib的消息循环
			m_pHelloBtn = m_PaintManager.FindControl(_T("btnHello"));
			if(m_pHelloBtn != NULL)
			{
				m_pHelloBtn->SetBorderSize(10);
				m_pHelloBtn->SetBkColor(0xff00ff00);
			}

			return lRes;
		}
		else if(uMsg == WM_PAINT)
		{

		}
		else if( uMsg == WM_DESTROY ) {
			::PostQuitMessage(0L);
		}
		else if( uMsg == WM_ERASEBKGND ) {
			return 1;
		}

		lRes = 0;
		if(m_PaintManager.MessageHandler(uMsg, wParam, lParam, lRes) ) 
			return lRes;

		return CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	}

protected:
	CPaintManagerUI m_PaintManager;
	CControlUI *m_pHelloBtn;
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

	CDuiFrameWnd* pFrame = new CDuiFrameWnd();
	if( pFrame == NULL ) 
		return 0;

	pFrame->Create(NULL, _T("简单测试demo-2"), UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
	pFrame->CenterWindow();
	pFrame->ShowWindow(true);
	CPaintManagerUI::MessageLoop();
	::CoUninitialize();
	return 0;
}
```



