# MFC CListControl 点击列头排序的实现 - 深之JohnChen的专栏 - CSDN博客

2017年07月31日 18:18:15[byxdaz](https://me.csdn.net/byxdaz)阅读数：1324


       SetItemData可以为每一行绑定一个DWORD类型的变量。用GetItemData可以获得这个变量。
举个例子，假设CListCtrl中你需要显示某个数据表中的记录，该表有个流水号主键ID，一般这个ID值本身没有什么意义，用户也不需要看，因此在CListCtrl的可见列中，你不需要显示。但往往做具体查询等操作时，你又需要用这个ID来完成。这时，用SetItemData将其绑定到每一行，将非常方便，用户操作哪一行，则用GetItemData可以得到对应记录的ID，直接用来做操作。

      SetItemData未绑定对象，可直接绑定行号进行排序。

      排序类

      ListCtrl_Sortable.h

```cpp
#pragma once

class CListCtrl_Sortable : public CListCtrl
{
public:
    bool m_Ascending;
	int  m_SortCol;

	DECLARE_MESSAGE_MAP();

	afx_msg BOOL OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult);
	void PreSubclassWindow();

public:
	CListCtrl_Sortable()
		:m_Ascending(false)
		,m_SortCol(-1)
	{}
	int GetColumnData(int col) const;
	void SetSortArrow(int col, bool ascending);
	bool IsAscending() const { return m_Ascending; }
	void ResetSortOrder();
	virtual bool SortColumn(int columnIndex, bool ascending);
	static int g_columnIndex;
	static bool g_ascending;
};
```

      ListCtrl_Sortable.cpp

```cpp
#include "stdafx.h"

#include "CListCtrl_Sortable.h"
#include "Resource.h"

#include <shlwapi.h>

int CListCtrl_Sortable::g_columnIndex = 0;
bool CListCtrl_Sortable::g_ascending = false;

BEGIN_MESSAGE_MAP(CListCtrl_Sortable, CListCtrl)
	ON_NOTIFY_REFLECT_EX(LVN_COLUMNCLICK, OnHeaderClick)	// Column Click
END_MESSAGE_MAP()

namespace {
	bool IsThemeEnabled()
	{
		HMODULE hinstDll;
		bool XPStyle = false;
		bool (__stdcall *pIsAppThemed)();
		bool (__stdcall *pIsThemeActive)();

		// Test if operating system has themes enabled
		hinstDll = ::LoadLibrary("UxTheme.dll");
		if (hinstDll)
		{
			(FARPROC&)pIsAppThemed = ::GetProcAddress(hinstDll, "IsAppThemed");
			(FARPROC&)pIsThemeActive = ::GetProcAddress(hinstDll,"IsThemeActive");
			::FreeLibrary(hinstDll);
			if (pIsAppThemed != NULL && pIsThemeActive != NULL)
			{
				if (pIsAppThemed() && pIsThemeActive())
				{
					// Test if application has themes enabled by loading the proper DLL
					hinstDll = LoadLibrary("comctl32.dll");
					if (hinstDll)
					{
						DLLGETVERSIONPROC pDllGetVersion = (DLLGETVERSIONPROC)::GetProcAddress(hinstDll, "DllGetVersion");
						::FreeLibrary(hinstDll);
						if (pDllGetVersion != NULL)
						{
							DLLVERSIONINFO dvi;
							ZeroMemory(&dvi, sizeof(dvi));
							dvi.cbSize = sizeof(dvi);
							HRESULT hRes = pDllGetVersion ((DLLVERSIONINFO *) &dvi);
							if (SUCCEEDED(hRes))
                                XPStyle = dvi.dwMajorVersion >= 6;
						}
					}
				}
			}
		}
		return XPStyle;
	}

	LRESULT EnableWindowTheme(HWND hwnd, LPCWSTR app, LPCWSTR idlist)
	{
		HMODULE hinstDll;
		HRESULT (__stdcall *pSetWindowTheme)(HWND hwnd, LPCWSTR pszSubAppName, LPCWSTR pszSubIdList);
		HANDLE (__stdcall *pOpenThemeData)(HWND hwnd, LPCWSTR pszClassList);
		HRESULT (__stdcall *pCloseThemeData)(HANDLE hTheme);

		hinstDll = ::LoadLibrary("UxTheme.dll");
		if (hinstDll)
		{
			(FARPROC&)pOpenThemeData = ::GetProcAddress(hinstDll, TEXT("OpenThemeData"));
			(FARPROC&)pCloseThemeData = ::GetProcAddress(hinstDll, TEXT("CloseThemeData"));
			(FARPROC&)pSetWindowTheme = ::GetProcAddress(hinstDll, TEXT("SetWindowTheme"));
			::FreeLibrary(hinstDll);
			if (pSetWindowTheme && pOpenThemeData && pCloseThemeData)
			{
				HANDLE theme = pOpenThemeData(hwnd,L"ListView");
				if (theme!=NULL)
				{
					VERIFY(pCloseThemeData(theme)==S_OK);
					return pSetWindowTheme(hwnd, app, idlist);
				}
			}
		}
		return S_FALSE;
	}
}

BOOL CListCtrl_Sortable::OnHeaderClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* pLV = reinterpret_cast<NMLISTVIEW*>(pNMHDR);

	SetFocus();	// Ensure other controls gets kill-focus

	int colIndex = pLV->iSubItem;

	if (m_SortCol==colIndex)
	{
		m_Ascending = !m_Ascending;
	}
	else
	{
		m_SortCol = colIndex;
		m_Ascending = true;
	}

	if (SortColumn(m_SortCol, m_Ascending))
		SetSortArrow(m_SortCol, m_Ascending);

	return FALSE;	// Let parent-dialog get chance
}

void CListCtrl_Sortable::SetSortArrow(int colIndex, bool ascending)
{
	if (IsThemeEnabled())
	{
#if (_WIN32_WINNT >= 0x501)
		for(int i = 0; i < GetHeaderCtrl()->GetItemCount(); ++i)
		{
			HDITEM hditem = {0};
			hditem.mask = HDI_FORMAT;
			VERIFY( GetHeaderCtrl()->GetItem( i, &hditem ) );
			hditem.fmt &= ~(HDF_SORTDOWN|HDF_SORTUP);
			if (i == colIndex)
			{
				hditem.fmt |= ascending ? HDF_SORTDOWN : HDF_SORTUP;
			}
			VERIFY( CListCtrl::GetHeaderCtrl()->SetItem( i, &hditem ) );
		}
#endif
	}
	else
	{
		UINT bitmapID = m_Ascending ? IDB_DOWNARROW : IDB_UPARROW;			//IDB_DOWNARROW，表示向下箭头IDB位图；IDB_UPARROW，表示向上箭头IDB位图
		for(int i = 0; i < GetHeaderCtrl()->GetItemCount(); ++i)
		{
			HDITEM hditem = {0};
			hditem.mask = HDI_BITMAP | HDI_FORMAT;
			VERIFY( GetHeaderCtrl()->GetItem( i, &hditem ) );
			if (hditem.fmt & HDF_BITMAP && hditem.fmt & HDF_BITMAP_ON_RIGHT)
			{
				if (hditem.hbm)
				{
					DeleteObject(hditem.hbm);
					hditem.hbm = NULL;
				}
				hditem.fmt &= ~(HDF_BITMAP|HDF_BITMAP_ON_RIGHT);
				VERIFY( CListCtrl::GetHeaderCtrl()->SetItem( i, &hditem ) );
			}
			if (i == colIndex)
			{
				hditem.fmt |= HDF_BITMAP|HDF_BITMAP_ON_RIGHT;
				hditem.hbm = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(bitmapID), IMAGE_BITMAP, 0,0, LR_LOADMAP3DCOLORS); 
				VERIFY( hditem.hbm!=NULL );
				VERIFY( CListCtrl::GetHeaderCtrl()->SetItem( i, &hditem ) );
			}
		}
	}
}

void CListCtrl_Sortable::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

	// Focus retangle is not painted properly without double-buffering
#if (_WIN32_WINNT >= 0x501)
	SetExtendedStyle(LVS_EX_DOUBLEBUFFER | GetExtendedStyle());
#endif
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_HEADERDRAGDROP);

	EnableWindowTheme(GetSafeHwnd(), L"Explorer", NULL);
}

void CListCtrl_Sortable::ResetSortOrder()
{
	m_Ascending = true;
	m_SortCol = -1;
	SetSortArrow(m_SortCol, m_Ascending);
}

// The column version of GetItemData(), one can specify an unique
// identifier when using InsertColumn()
int CListCtrl_Sortable::GetColumnData(int col) const
{
	LVCOLUMN lvc = {0};
	lvc.mask = LVCF_SUBITEM;
	VERIFY( GetColumn(col, &lvc) );
	return lvc.iSubItem;
}

// 比较函数
static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// 从参数中提取所需比较lc的两行数据
	int row1 = (int) lParam1;
	int row2 = (int) lParam2;
	CListCtrl* lc = (CListCtrl*)lParamSort;
	CString lp1 = lc->GetItemText(row1,CListCtrl_Sortable::g_columnIndex);
	CString lp2 = lc->GetItemText(row2,CListCtrl_Sortable::g_columnIndex);
	// 文字型比较
	if (CListCtrl_Sortable::g_ascending)
		return lp1.CompareNoCase(lp2);
	else
		return lp2.CompareNoCase(lp1);

	return 0;
}

bool CListCtrl_Sortable::SortColumn(int columnIndex, bool ascending)
{
	//排序
	g_columnIndex = columnIndex;
	g_ascending = ascending;
	int count = GetItemCount();
	for (int i=0;i<count;i++)
	{
		  SetItemData(i,i); // 每行的比较关键字，此处为列序号（点击的列号），可以设置为其他 比较函数的第一二个参数
	}
	SortItems(MyCompareProc, (DWORD_PTR)this);
	return true;
}
```

//IDB_DOWNARROW，表示向下箭头IDB位图；IDB_UPARROW，表示向上箭头IDB位图。

      SetItemData绑定对象排序。

      如果SetItemData绑定的数据是一个对象时，需要在SortColumn函数的比较函数中做处理。通过GetItemData读取对象，遍历CListCtrl所有行，与对象比较，找到某两行。然后再获取这两行对应列的文本，进行比较排序。

     ListCtrl_Sortable_ItemObject.h

```cpp
#pragma once
#include "listctrl_sortable.h"

class CListCtrl_Sortable_ItemObject : public CListCtrl_Sortable
{
public:
	CListCtrl_Sortable_ItemObject(void);
	virtual ~CListCtrl_Sortable_ItemObject(void);

	virtual bool SortColumn(int columnIndex, bool ascending);
};
```

     ListCtrl_Sortable_ItemObject.cpp

```cpp
#include "StdAfx.h"
#include "ListCtrl_Sortable_ItemObject.h"

CListCtrl_Sortable_ItemObject::CListCtrl_Sortable_ItemObject(void)
{
}

CListCtrl_Sortable_ItemObject::~CListCtrl_Sortable_ItemObject(void)
{
}

//比较函数
static int CALLBACK ItemObjectCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// 从参数中提取所需比较lc的两行数据
	void * pRow1 = (void *) lParam1;
	void * pRow2 = (void *) lParam2;
	if(pRow1 == NULL || pRow2 == NULL)
		return 0;

	CListCtrl* lc = (CListCtrl*)lParamSort;
	void * pTempObject = NULL;
	int nItemCount = lc->GetItemCount();
	int nn = 0,nRow1 = -1,nRow2 = -1;
	for(nn=0; nn<nItemCount; nn++)
	{
		pTempObject = (void *)lc->GetItemData(nn);
		if(pTempObject == pRow1)
		{
			nRow1 = nn;
		}
		else if(pTempObject == pRow2)
		{
			nRow2 = nn;
		}

		if(nRow1 != -1 && nRow2 != -1)
		{
			break;
		}
	}

	if(nRow1 == -1 || nRow2 == -1)
		return 0;

	CString lp1 = lc->GetItemText(nRow1,CListCtrl_Sortable_ItemObject::g_columnIndex);
	CString lp2 = lc->GetItemText(nRow2,CListCtrl_Sortable_ItemObject::g_columnIndex);
	// 文字型比较
	if (CListCtrl_Sortable_ItemObject::g_ascending)
		return lp1.CompareNoCase(lp2);
	else
		return lp2.CompareNoCase(lp1);

	return 0;
}

bool CListCtrl_Sortable_ItemObject::SortColumn(int columnIndex, bool ascending)
{
	//排序
	g_columnIndex = columnIndex;
	g_ascending = ascending;
	SortItems(ItemObjectCompareProc, (DWORD_PTR)this);
	return true;
}
```



