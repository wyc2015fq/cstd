// testsdiView.h : interface of the CTestsdiView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TESTSDIVIEW_H__6E94A50E_F25F_4346_908F_B5DF2380B8AE__INCLUDED_)
#define AFX_TESTSDIVIEW_H__6E94A50E_F25F_4346_908F_B5DF2380B8AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTestsdiView : public CView
{
protected: // create from serialization only
	CTestsdiView();
	DECLARE_DYNCREATE(CTestsdiView)

// Attributes
public:
	CTestsdiDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestsdiView)
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
	virtual ~CTestsdiView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTestsdiView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in testsdiView.cpp
inline CTestsdiDoc* CTestsdiView::GetDocument()
   { return (CTestsdiDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTSDIVIEW_H__6E94A50E_F25F_4346_908F_B5DF2380B8AE__INCLUDED_)
