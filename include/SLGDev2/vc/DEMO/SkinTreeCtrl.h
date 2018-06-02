#if !defined(AFX_SKINTREECTRL_H__327561CB_3A1A_432D_AAC0_77D2E67CAE40__INCLUDED_)
#define AFX_SKINTREECTRL_H__327561CB_3A1A_432D_AAC0_77D2E67CAE40__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkinTreeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSkinTreeCtrl window

class CSkinTreeCtrl : public CTreeCtrl
{
// Construction
public:
	CSkinTreeCtrl();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinTreeCtrl)
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSkinTreeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSkinTreeCtrl)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKINTREECTRL_H__327561CB_3A1A_432D_AAC0_77D2E67CAE40__INCLUDED_)
