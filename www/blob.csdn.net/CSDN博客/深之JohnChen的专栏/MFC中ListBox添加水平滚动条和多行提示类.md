# MFC中ListBox添加水平滚动条和多行提示类 - 深之JohnChen的专栏 - CSDN博客

2018年12月04日 15:31:58[byxdaz](https://me.csdn.net/byxdaz)阅读数：256


//CExListBox.h

```
// CExListBox window

#pragma once

class CExListBox : public CListBox
{
// Construction
public:
	CExListBox();

	// 覆盖该方法以便添加水平滚动条
	int AddString(LPCTSTR lpszItem);
	int InsertString(int nIndex, LPCTSTR lpszItem);
	// 计算水平滚动条宽度
	void RefushHorizontalScrollBar(void);

// Attributes
public:
	int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
//	void PreSubclassWindow() ;
	
	BOOL OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExListBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CExListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CExListBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
	CString m_strToolTipText;
};
```

//CExListBox.cpp

```
// ExListBox.cpp : implementation file
//

#include "stdafx.h"

#include "ExListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExListBox

CExListBox::CExListBox()
{
	m_strToolTipText = "";
}

CExListBox::~CExListBox()
{
}

BEGIN_MESSAGE_MAP(CExListBox, CListBox)
	//{{AFX_MSG_MAP(CExListBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.

	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExListBox message handlers

void CExListBox::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	

	CListBox::PreSubclassWindow();
	EnableToolTips(TRUE);

}

int CExListBox::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
	int row;
	RECT cellrect;   // cellrect		- to hold the bounding rect
	BOOL tmp = FALSE;
	row  = ItemFromPoint(point,tmp);  //we call the ItemFromPoint function to determine the row,
	//note that in NT this function may fail  use the ItemFromPointNT member function

	if ( row == -1 ) 
		return -1;

	//set up the TOOLINFO structure. GetItemRect(row,&cellrect);
	GetItemRect(row,&cellrect);
	pTI->rect = cellrect;
	pTI->hwnd = m_hWnd;
	pTI->uId = (UINT)((row));   //The 憉Id?is assigned a value according to the row value.
	pTI->lpszText = LPSTR_TEXTCALLBACK;
	return pTI->uId;

}

//Define OnToolTipText(). This is the handler for the TTN_NEEDTEXT notification from 
//support ansi and unicode 
BOOL CExListBox::OnToolTipText( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
	// need to handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	UINT nID = pNMHDR->idFrom;

	AFX_MODULE_THREAD_STATE* pThreadState = AfxGetModuleThreadState();	
	CToolTipCtrl *pToolTip = pThreadState->m_pToolTip;	
	if (pToolTip != NULL)
	{
		pToolTip->SetMaxTipWidth(500);
	}
	
	GetText( nID , m_strToolTipText);

#ifdef _UNICODE
	char szMsg[512] = { 0 };
	strcpy(szMsg, m_strToolTipText.GetBuffer(m_strToolTipText.GetLength()));
	pTTTA->lpszText = szMsg;
	USES_CONVERSION;
	pTTTW->lpszText = A2W(m_strToolTipText);
	m_strToolTipText.ReleaseBuffer();
#else
	USES_CONVERSION;
	pTTTW->lpszText = A2W(m_strToolTipText);
	char szMsg[512] = { 0 };
	strcpy(szMsg, m_strToolTipText.GetBuffer(m_strToolTipText.GetLength()));
	pTTTA->lpszText = szMsg;
	m_strToolTipText.ReleaseBuffer();
#endif

	*pResult = 0;
	return TRUE;    
}

int CExListBox::AddString(LPCTSTR lpszItem)
{
	int nResult = CListBox::AddString(lpszItem);
	RefushHorizontalScrollBar();
	return nResult;
}
int CExListBox::InsertString(int nIndex, LPCTSTR lpszItem)
{
	int nResult = CListBox::InsertString(nIndex, lpszItem);
	RefushHorizontalScrollBar();
	return nResult;
}

void CExListBox::RefushHorizontalScrollBar(void)
{
	CDC *pDC = this->GetDC();
	if (NULL == pDC)
	{
		return;
	}
	int nCount = this->GetCount();
	if (nCount < 1)
	{
		this->SetHorizontalExtent(0);
		return;
	}
	int nMaxExtent = 0;
	CString szText;
	for (int i = 0; i < nCount; ++i)
	{
		this->GetText(i, szText);
		CSize &cs = pDC->GetTextExtent(szText);
		if (cs.cx > nMaxExtent)
		{
			nMaxExtent = cs.cx;
		}
	}
	this->SetHorizontalExtent(nMaxExtent);
}
```

使用步骤：

1、添加CListBox控件，设置属性，列表框的垂直滚动条、水平滚动条有效，取消其排序风格。并添加CListBox变量。

2、将CListBox变量，替换成CExListBox

