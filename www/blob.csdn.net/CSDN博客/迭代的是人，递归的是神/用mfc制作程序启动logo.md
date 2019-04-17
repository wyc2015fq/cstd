# 用mfc制作程序启动logo - 迭代的是人，递归的是神 - CSDN博客





2012年03月06日 09:52:40[yang_xian521](https://me.csdn.net/yang_xian521)阅读数：5830








完了，这次土鳖了，介绍个非常过时的东西，其实就是自己收藏一下，怕以后想用的时候自己忘了。

直接进入正题，首先用ps等工具做一个自己喜欢的程序启动的logo位图，我这里制作的logo大小为500*313，比例为黄金比例，看上去很舒服。

新建mfc工程，把制作好的logo位图添加的资源中去，资源ID号设置为IDB_SPLASH。

新建类CSplashWnd，



```cpp
#pragma once
#include "afxwin.h"

class CSplashWnd :
	public CWnd
{
public:
	CSplashWnd(void);
	~CSplashWnd(void);
	CBitmap m_bitmap;
	static void ShowSplashScreen(CWnd* pParentWnd = NULL);
protected:
	BOOL Create(CWnd* pParentWnd = NULL);	
	static CSplashWnd* c_pSplashWnd;
public:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
```


为各成员函数添加代码：



```cpp
#include "StdAfx.h"
#include "SplashWnd.h"
#include "resource.h"

CSplashWnd* CSplashWnd::c_pSplashWnd;

BEGIN_MESSAGE_MAP(CSplashWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

CSplashWnd::CSplashWnd(void)
/*: c_pSplashWnd(NULL)*/
{
}

CSplashWnd::~CSplashWnd(void)
{
}

void CSplashWnd::ShowSplashScreen(CWnd* pParentWnd)
{
	c_pSplashWnd = new CSplashWnd;
	if (!c_pSplashWnd->Create(pParentWnd))
		delete c_pSplashWnd;
	else
		c_pSplashWnd->UpdateWindow();
}

BOOL CSplashWnd::Create(CWnd* pParentWnd)
{
	if (!m_bitmap.LoadBitmap(IDB_SPLASH))	/*加载位图资源，需在资源文件中添加*/
		return FALSE;

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	return CreateEx(0,
		AfxRegisterWndClass(0, AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		NULL, WS_POPUP | WS_VISIBLE, 0, 0, bm.bmWidth, bm.bmHeight, pParentWnd->GetSafeHwnd(), NULL);
	return 0;
}

int CSplashWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	// Center the window.
	CenterWindow();

	// Set a timer to destroy the splash screen.
	SetTimer(1, 1000, NULL);				/*设置消隐时间*/

	return 0;
}

void CSplashWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	CDC dcImage;
	if (!dcImage.CreateCompatibleDC(&dc))
		return;

	BITMAP bm;
	m_bitmap.GetBitmap(&bm);

	// Paint the image.
	CBitmap* pOldBitmap = dcImage.SelectObject(&m_bitmap);
	dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &dcImage, 0, 0, SRCCOPY);
	dcImage.SelectObject(pOldBitmap);
}

void CSplashWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DestroyWindow();
	AfxGetMainWnd()->UpdateWindow();

	CWnd::OnTimer(nIDEvent);
}
```


其中消隐时间的设置为你喜欢的数值。

接下来，只需在MFC工程中的CMainFrame类中添加函数OnCreate，并在函数定义中添加语句CSplashWnd::ShowSplashScreen(this);



```cpp
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CSplashWnd::ShowSplashScreen(this);

	return 0;
}
```


好了，你的程序启动logo做好了，编译运行吧，少年！~~
            


