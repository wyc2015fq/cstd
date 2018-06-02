/********************************************************************

	RightView.h

	----------------------------------------------------------------
    软件许可证 － GPL
	版权所有 (C) 2003 VCHelp coPathway ISee workgroup.
	----------------------------------------------------------------
	这一程序是自由软件，你可以遵照自由软件基金会出版的GNU 通用公共许
	可证条款来修改和重新发布这一程序。或者用许可证的第二版，或者（根
	据你的选择）用任何更新的版本。

    发布这一程序的目的是希望它有用，但没有任何担保。甚至没有适合特定
	目地的隐含的担保。更详细的情况请参阅GNU通用公共许可证。

    你应该已经和程序一起收到一份GNU通用公共许可证的副本（本目录
	GPL.txt文件）。如果还没有，写信给：
    The Free Software Foundation, Inc.,  675  Mass Ave,  Cambridge,
    MA02139,  USA
	----------------------------------------------------------------
	如果你在使用本软件时有什么问题或建议，请用以下地址与我们取得联系：

			http://isee.126.com
			http://cosoft.org.cn/projects/iseeexplorer
			
	或发信到：

			isee##vip.163.com
	----------------------------------------------------------------
	本文件用途：	图像读写引擎－C语言版－VC编译器－测试软件列表视
					类定义文件。

	本文件编写人：	
					YZ				yzfree##sina.com

	本文件版本：	30222
	最后修改于：	2003-2-22

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
		地址收集软件。

	修正历史：

		2003-2		第一个测试版发布

********************************************************************/

#if !defined(AFX_RIGHTVIEW_H__5FB6456A_1F62_4F47_9CE4_811EEC6FDC36__INCLUDED_)
#define AFX_RIGHTVIEW_H__5FB6456A_1F62_4F47_9CE4_811EEC6FDC36__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CShellList;


class CRightView : public CListView
{
protected: // create from serialization only
	CRightView();
	DECLARE_DYNCREATE(CRightView)

// Attributes
public:
	CIseetsvcDoc* GetDocument();

	CShellList		*m_pListCtrl;

	BOOL			m_bFrmMark;							// 窗口全部创建结束标志
	int				m_bFillMark;						// 列表项填充标志（0－开始填充、1－文件名填充完毕、2－图象信息填充完毕）

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRightView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRightView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRightView)
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg LRESULT OnFillBegin(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFillEnd(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAutoNext(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSubItemDataFill(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWndCreated(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteallitems(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnActive(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnStyleChanged(int nStyleType, LPSTYLESTRUCT lpStyleStruct);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RightView.cpp
inline CIseetsvcDoc* CRightView::GetDocument()
   { return (CIseetsvcDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIGHTVIEW_H__5FB6456A_1F62_4F47_9CE4_811EEC6FDC36__INCLUDED_)
