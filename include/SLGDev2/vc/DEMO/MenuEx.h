// MenuEx.h: interface for the CMenuEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUEX_H__58EAE0CA_DDD5_415E_A4F9_D3865E490606__INCLUDED_)
#define AFX_MENUEX_H__58EAE0CA_DDD5_415E_A4F9_D3865E490606__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct tagMENUITEM 
{
	UINT nID;
	CString strText;
	BOOL checkStatus;
}MENUITEM, *LPMENUITE;

class CMenuEx : public CMenu  
{
public:
	CMenuEx();
	virtual ~CMenuEx();
public:
	BOOL CreatePopMenu( UINT nIDResource );
	BOOL InserMenuItem( UINT nIDResource, LPCTSTR lpszNewItem );
	BOOL LoadMenu(UINT nIDResource);
	BOOL LoadMenu(CMenu& menu);
	BOOL AppendMenu( UINT nFlags, UINT nIDNewItem = 0, LPCTSTR lpszNewItem = NULL );
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
private:
	void DrawText(HDC hdc, RECT rect, CString strText, UINT  itemState);
	void DrawBgColor(HDC hdc,  RECT rect);
	void DrawSelected(HDC hdc, RECT rect, UINT  itemState);
	void DrawMenuImage(HDC hdc, RECT rect, UINT  itemState);
private:
	BOOL m_bPopMenu;
	CMenu m_pParentMenu;
	MENUITEM m_menuItem[20];
};

#endif // !defined(AFX_MENUEX_H__58EAE0CA_DDD5_415E_A4F9_D3865E490606__INCLUDED_)
