// testmdiView.h : interface of the CTestmdiView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTMDIVIEW_H__D9986DB5_75DE_4E6A_8326_EC628E97233B__INCLUDED_)
#define AFX_TESTMDIVIEW_H__D9986DB5_75DE_4E6A_8326_EC628E97233B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTestmdiView : public CView
{
	IMPLEMENT_DYNCREATE(CTestmdiView, CView)
protected: // create from serialization only
	CTestmdiView();

// Attributes
public:
	CTestmdiDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestmdiView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTestmdiView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTestmdiView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in testmdiView.cpp
inline CTestmdiDoc* CTestmdiView::GetDocument()
   { return (CTestmdiDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTMDIVIEW_H__D9986DB5_75DE_4E6A_8326_EC628E97233B__INCLUDED_)
