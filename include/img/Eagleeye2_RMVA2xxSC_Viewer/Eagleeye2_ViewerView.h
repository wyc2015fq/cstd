// Eagleeye2_ViewerView.h : interface of the CEagleeye2_ViewerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_EAGLEEYE2_VIEWERVIEW_H__BA38D3AD_D91F_45F7_8334_45FF09337E34__INCLUDED_)
#define AFX_EAGLEEYE2_VIEWERVIEW_H__BA38D3AD_D91F_45F7_8334_45FF09337E34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEagleeye2_ViewerView : public CScrollView
{
protected: // create from serialization only
	CEagleeye2_ViewerView();
	DECLARE_DYNCREATE(CEagleeye2_ViewerView)

// Attributes
public:
	CEagleeye2_ViewerDoc* GetDocument();

// Operations
public:
	int			m_iClientW, m_iClientH;
	BOOL		m_gEraseBk;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEagleeye2_ViewerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEagleeye2_ViewerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CEagleeye2_ViewerView)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	afx_msg LRESULT OnRecvImageOK(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Eagleeye2_ViewerView.cpp
inline CEagleeye2_ViewerDoc* CEagleeye2_ViewerView::GetDocument()
   { return (CEagleeye2_ViewerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EAGLEEYE2_VIEWERVIEW_H__BA38D3AD_D91F_45F7_8334_45FF09337E34__INCLUDED_)
