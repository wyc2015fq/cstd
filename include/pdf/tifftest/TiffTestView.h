// TiffTestView.h : interface of the CTiffTestView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TIFFTESTVIEW_H__AE9B82B2_3533_4087_A443_B5F5C0E7F736__INCLUDED_)
#define AFX_TIFFTESTVIEW_H__AE9B82B2_3533_4087_A443_B5F5C0E7F736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTiffTestView : public CView
{
protected: // create from serialization only
	CTiffTestView();
	DECLARE_DYNCREATE(CTiffTestView)

// Attributes
public:
	CTiffTestDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTiffTestView)
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
	virtual ~CTiffTestView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTiffTestView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in TiffTestView.cpp
inline CTiffTestDoc* CTiffTestView::GetDocument()
   { return (CTiffTestDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIFFTESTVIEW_H__AE9B82B2_3533_4087_A443_B5F5C0E7F736__INCLUDED_)
