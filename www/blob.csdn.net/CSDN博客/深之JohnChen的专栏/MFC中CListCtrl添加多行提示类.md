# MFC中CListCtrl添加多行提示类 - 深之JohnChen的专栏 - CSDN博客

2017年05月05日 13:18:30[byxdaz](https://me.csdn.net/byxdaz)阅读数：715


一、CListCtrl添加多行提示类

//ToolTipListCtrl.h

```cpp
#pragma once

// CToolTipListCtrl

class CToolTipListCtrl : public CListCtrl
{
	DECLARE_DYNAMIC(CToolTipListCtrl)

public:
	CToolTipListCtrl();
	virtual ~CToolTipListCtrl();

	virtual int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;

protected:
	virtual afx_msg BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT *  pResult );
	virtual void PreSubclassWindow();
	DECLARE_MESSAGE_MAP()
private:
	CString m_strToolTipText;//Item提示信息
};
```

//ToolTipListCtrl.cpp

```cpp
#include "stdafx.h"
#include "ToolTipListCtrl.h"
#include <atlconv.h>

// CToolTipListCtrl

IMPLEMENT_DYNAMIC(CToolTipListCtrl, CListCtrl)
CToolTipListCtrl::CToolTipListCtrl()
{
}

CToolTipListCtrl::~CToolTipListCtrl()
{
}

BEGIN_MESSAGE_MAP(CToolTipListCtrl, CListCtrl)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

// CToolTipListCtrl 消息处理程序

void CToolTipListCtrl::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

	// Disable the CToolTipCtrl of CListCtrl so it won't disturb our own tooltip-ctrl
	GetToolTips()->Activate(FALSE);
}

int CToolTipListCtrl::OnToolHitTest(CPoint point, TOOLINFO * pTI) const {

	//See if the point falls onto a list item
	//UINT nFlags = 0;

	LVHITTESTINFO lvhitTestInfo;

	lvhitTestInfo.pt = point;

	int nItem = ListView_SubItemHitTest(
		this->m_hWnd,
		&lvhitTestInfo);
	int nSubItem = lvhitTestInfo.iSubItem;

	UINT nFlags =   lvhitTestInfo.flags;

	//nFlags is 0 if the SubItemHitTest fails
	//Therefore, 0 & <anything> will equal false
	if (nFlags & LVHT_ONITEMLABEL){
		//If it did fall on a list item,
		//and it was also hit one of the
		//item specific sub-areas we wish to show tool tips for

		//Get the client (area occupied by this control
		RECT rcClient;
		GetClientRect( &rcClient );

		//Fill in the TOOLINFO structure
		pTI->hwnd = m_hWnd;
		pTI->uId = (UINT) (nItem * 100 + nSubItem);
		pTI->lpszText = LPSTR_TEXTCALLBACK;
		pTI->rect = rcClient;

		return pTI->uId; //By returning a unique value per listItem,
		//we ensure that when the mouse moves over another list item,
		//the tooltip will change
	}else{
		//Otherwise, we aren't interested, so let the message propagate
		return -1;
	}
}

BOOL CToolTipListCtrl::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult ){

	//VC6.0则用下面这句
	//_AFX_THREAD_STATE* pThreadState = AfxGetThreadState();

	//VC2003则用这句
	AFX_MODULE_THREAD_STATE* pThreadState = AfxGetModuleThreadState();
	CToolTipCtrl *pToolTip = pThreadState->m_pToolTip;
	pToolTip->SetMaxTipWidth(500);

	//Handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	if( (pNMHDR->idFrom == (UINT)m_hWnd) &&
		( ((pNMHDR->code == TTN_NEEDTEXTA) && (pTTTA->uFlags & TTF_IDISHWND)) ||
		((pNMHDR->code == TTN_NEEDTEXTW) && (pTTTW->uFlags & TTF_IDISHWND)) ) ){
			return FALSE;
	}

	*pResult = 0;

	//Get the mouse position
	const MSG* pMessage;
	pMessage = GetCurrentMessage();
	ASSERT ( pMessage );
	CPoint pt;
	pt = pMessage->pt; //Get the point from the message
	ScreenToClient( &pt );

	LVHITTESTINFO lvhitTestInfo;

	lvhitTestInfo.pt = pt;

	int nItem = SubItemHitTest(&lvhitTestInfo);
	int nSubItem = lvhitTestInfo.iSubItem;

	UINT nFlags =   lvhitTestInfo.flags;

	if( nFlags & LVHT_ONITEMLABEL ){

		//设置提示信息显示的内容
		m_strToolTipText = GetItemText(nItem,nSubItem);
		//pTTTA->lpszText = (LPSTR)(LPCSTR)m_strToolTipText;	//releas 会出现乱码，用下面方式替换
		char szMsg[512] = {0};
		strcpy(szMsg,m_strToolTipText.GetBuffer());
		pTTTA->lpszText = szMsg;
		m_strToolTipText.ReleaseBuffer();

		USES_CONVERSION;
		pTTTW->lpszText = A2W(m_strToolTipText);

		return FALSE; 
	}

	return FALSE;
}
```

二、CListCtrl内嵌ToolTipCtrl自定义提示类
//ListCtrl_OwnToolTipCtrl.h

```cpp
#pragma once

#include "ListCtrl_ToolTip.h"

class CListCtrl_OwnToolTipCtrl : public CListCtrl_ToolTip
{
	int m_LastToolTipCol;
	int m_LastToolTipRow;

	CToolTipCtrl m_OwnToolTipCtrl;

	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP();

public:
	CListCtrl_OwnToolTipCtrl()
		:m_LastToolTipCol(-1)
		,m_LastToolTipRow(-1)
	{}
};
```

//ListCtrl_OwnToolTipCtrl.cpp

```cpp
#include "stdafx.h"
#include "ListCtrl_OwnToolTipCtrl.h"

// Maintain our own CToolTipCtrl
BEGIN_MESSAGE_MAP(CListCtrl_OwnToolTipCtrl, CListCtrl_OwnToolTipCtrl)
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_EX(TTN_NEEDTEXTA, 0, OnToolNeedText)
	ON_NOTIFY_EX(TTN_NEEDTEXTW, 0, OnToolNeedText)
END_MESSAGE_MAP()

void CListCtrl_OwnToolTipCtrl::PreSubclassWindow()
{
	CListCtrl_ToolTip::PreSubclassWindow();

	// Disable the CToolTipCtrl of CListCtrl so it won't disturb our own tooltip-ctrl
	GetToolTips()->Activate(FALSE);

	// Enable our own tooltip-ctrl and make it show tooltip even if not having focus
	VERIFY( m_OwnToolTipCtrl.Create(this, TTS_ALWAYSTIP) );
	m_OwnToolTipCtrl.Activate(TRUE);
}

BOOL CListCtrl_OwnToolTipCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (m_OwnToolTipCtrl.m_hWnd)
		m_OwnToolTipCtrl.RelayEvent(pMsg);
	return CListCtrl::PreTranslateMessage(pMsg);
}

void CListCtrl_OwnToolTipCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);

	// Find the subitem
	LVHITTESTINFO hitinfo = {0};
	hitinfo.flags = nFlags;
	hitinfo.pt = pt;
	SubItemHitTest(&hitinfo);

	if (m_LastToolTipCol!=hitinfo.iSubItem || m_LastToolTipRow!=hitinfo.iItem)
	{
		// Mouse moved over a new cell
		m_LastToolTipCol = hitinfo.iSubItem;
		m_LastToolTipRow = hitinfo.iItem;

		// Remove the old tooltip (if available)
		if (m_OwnToolTipCtrl.GetToolCount()>0)
		{
			m_OwnToolTipCtrl.DelTool(this);
			m_OwnToolTipCtrl.Activate(FALSE);
		}

		// Add the new tooltip (if available)
		if (m_LastToolTipRow!=-1 && m_LastToolTipRow!=-1)
		{
			// Not using CToolTipCtrl::AddTool() because it redirects the messages to CListCtrl parent
			TOOLINFO ti = {0};
			ti.cbSize = sizeof(TOOLINFO);
			ti.uFlags = TTF_IDISHWND;	// Indicate that uId is handle to a control
			ti.uId = (UINT_PTR)m_hWnd;	// Handle to the control
			ti.hwnd = m_hWnd;			// Handle to window to receive the tooltip-messages
			ti.hinst = AfxGetInstanceHandle();
			ti.lpszText = LPSTR_TEXTCALLBACK;
			m_OwnToolTipCtrl.SendMessage(TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
			m_OwnToolTipCtrl.Activate(TRUE);
		}
	}

	CListCtrl::OnMouseMove(nFlags, point);
}

BOOL CListCtrl_OwnToolTipCtrl::OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	return CListCtrl_ToolTip::OnToolNeedText(id, pNMHDR, pResult);
}

//ListCtrl_ToolTip.h
#pragma once

class CListCtrl_ToolTip : public CListCtrl
{
public:
	afx_msg BOOL OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);

	virtual void PreSubclassWindow();

	void CellHitTest(const CPoint& pt, int& nRow, int& nCol) const;
	bool ShowToolTip(const CPoint& pt) const;
	CString GetToolTipText(int nRow, int nCol);
};

//ListCtrl_ToolTip.cpp
#include "stdafx.h"
#include "CListCtrl_ToolTip.h"

void CListCtrl_ToolTip::CellHitTest(const CPoint& pt, int& nRow, int& nCol) const
{
	nRow = -1;
	nCol = -1;

	LVHITTESTINFO lvhti = {0};
	lvhti.pt = pt;
	nRow = ListView_SubItemHitTest(m_hWnd, &lvhti);	// SubItemHitTest is non-const
	nCol = lvhti.iSubItem;
	if (!(lvhti.flags & LVHT_ONITEMLABEL))
		nRow = -1;
}

BOOL CListCtrl_ToolTip::OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	CPoint pt(GetMessagePos());
	ScreenToClient(&pt);

	int nRow, nCol;
	CellHitTest(pt, nRow, nCol);

	CString tooltip = GetToolTipText(nRow, nCol);
	if (tooltip.IsEmpty())
		return FALSE;

	// Non-unicode applications can receive requests for tooltip-text in unicode
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
#ifndef _UNICODE
	if (pNMHDR->code == TTN_NEEDTEXTA)
		lstrcpyn(pTTTA->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTA->szText));
	else
		_mbstowcs(pTTTW->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTW->szText)/sizeof(WCHAR));
#else
	if (pNMHDR->code == TTN_NEEDTEXTA)
		_wcstombsz(pTTTA->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTA->szText));
	else
		lstrcpyn(pTTTW->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTW->szText)/sizeof(WCHAR));
#endif
	// If wanting to display a tooltip which is longer than 80 characters,
	// then one must allocate the needed text-buffer instead of using szText,
	// and point the TOOLTIPTEXT::lpszText to this text-buffer.
	// When doing this, then one is required to release this text-buffer again
	return TRUE;
}

bool CListCtrl_ToolTip::ShowToolTip(const CPoint& pt) const
{
	// Lookup up the cell
	int nRow, nCol;
	CellHitTest(pt, nRow, nCol);

	if (nRow!=-1 && nCol!=-1)
		return true;
	else
		return false;
}

CString CListCtrl_ToolTip::GetToolTipText(int nRow, int nCol)
{
	if (nRow!=-1 && nCol!=-1)
		return GetItemText(nRow, nCol);	// Cell-ToolTip
	else
		return CString("");
}

namespace {
	LRESULT EnableWindowTheme(HWND hwnd, LPCWSTR classList, LPCWSTR subApp, LPCWSTR idlist)
	{
		LRESULT lResult = S_FALSE;
		HMODULE hinstDll;
		BOOL (WINAPI *pIsThemeActive)();
		HRESULT (WINAPI *pSetWindowTheme)(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);
		HANDLE (WINAPI *pOpenThemeData)(HWND hwnd, LPCWSTR pszClassList);
		HRESULT (WINAPI *pCloseThemeData)(HANDLE hTheme);

		// Check if running on Windows XP or newer
		hinstDll = ::LoadLibrary(_T("UxTheme.dll"));
		if (hinstDll)
		{
			// Check if theme service is running
			(FARPROC&)pIsThemeActive = ::GetProcAddress( hinstDll, "IsThemeActive" );
			if( pIsThemeActive && pIsThemeActive() )
			{
				(FARPROC&)pOpenThemeData = ::GetProcAddress(hinstDll, "OpenThemeData");
				(FARPROC&)pCloseThemeData = ::GetProcAddress(hinstDll, "CloseThemeData");
				(FARPROC&)pSetWindowTheme = ::GetProcAddress(hinstDll, "SetWindowTheme");
				if (pSetWindowTheme && pOpenThemeData && pCloseThemeData)			
				{
					// Check is themes is available for the application
					HANDLE hTheme = pOpenThemeData(hwnd,classList);
					if (hTheme!=NULL)
					{
						VERIFY(pCloseThemeData(hTheme)==S_OK);
						// Enable Windows Theme Style
						lResult = pSetWindowTheme(hwnd, subApp, idlist);
					}
				}
			}
			::FreeLibrary(hinstDll);
		}
		return lResult;
	}
}

void CListCtrl_ToolTip::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

	// Focus retangle is not painted properly without double-buffering
#if (_WIN32_WINNT >= 0x501)
	SetExtendedStyle(LVS_EX_DOUBLEBUFFER | GetExtendedStyle());
#endif
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_HEADERDRAGDROP);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_GRIDLINES);

	// Enable Vista-look if possible
	EnableWindowTheme(m_hWnd, L"ListView", L"Explorer", NULL);
}
```
三、CListCtrl添加提示类

//MyTooltipListCtrl.h

```cpp
#pragma once
#include "afxcmn.h"
class CMyTooltipListCtrl : public CListCtrl
{
public:
	CMyTooltipListCtrl();
	~CMyTooltipListCtrl();

protected:
	CToolTipCtrl m_toolTip;        //文本提示类  
	int m_nSubItem;                //存放行号  
	int m_nItem;                //存放列号  
	BOOL m_bEnableTips;            //是否开启文本提示 
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
```

//MyTooltipListCtrl.cpp

```cpp
#include "stdafx.h"
#include "MyTooltipListCtrl.h"

CMyTooltipListCtrl::CMyTooltipListCtrl()
{
	m_bEnableTips = TRUE;
	m_toolTip.Create(this, TTS_ALWAYSTIP);
	m_toolTip.SetDelayTime(TTDT_AUTOPOP, 10000);//10 seconds
	m_toolTip.SetMaxTipWidth(10);
}

CMyTooltipListCtrl::~CMyTooltipListCtrl()
{
}

BOOL CMyTooltipListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_toolTip.GetSafeHwnd())
	{
		m_toolTip.RelayEvent(pMsg);
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}
BEGIN_MESSAGE_MAP(CMyTooltipListCtrl, CListCtrl)
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CMyTooltipListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//如果开启文本提示  
	if (m_bEnableTips)
	{
		CString str;
		LVHITTESTINFO lvhti;

		// 判断鼠标当前所在的位置(行, 列)  
		lvhti.pt = point;
		SubItemHitTest(&lvhti);

		//如果鼠标移动到另一个单元格内, 则进行处理; 否则, 不做处理  
		if ((lvhti.iItem != m_nItem) || (lvhti.iSubItem != m_nSubItem))
		{
			// 保存当前鼠标所在的(行,列)  
			m_nItem = lvhti.iItem;
			m_nSubItem = lvhti.iSubItem;

			// 如果鼠标移动到一个合法的单元格内,则显示新的提示信息  
			// 否则, 不显示提示  
			if ((m_nItem != -1) && (m_nSubItem != -1))
			{
				// 在这里修改要显示的提示信息  
				// 这里仅仅是一个例子---获得当前单元格的文字信息, 并设置为新的提示信息  
				str = GetItemText(m_nItem, m_nSubItem);
				m_toolTip.AddTool(this, str);
				// 显示提示框  
				m_toolTip.Pop();
			}
			else
			{
				m_toolTip.AddTool(this, "");
				m_toolTip.Pop();
			}
		}
	}

	CListCtrl::OnMouseMove(nFlags, point);
}
```




