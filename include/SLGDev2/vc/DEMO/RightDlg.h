#if !defined(AFX_RIGHTDLG_H__7FB46F08_46DD_4707_A604_1AB500ADEF78__INCLUDED_)
#define AFX_RIGHTDLG_H__7FB46F08_46DD_4707_A604_1AB500ADEF78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RightDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRightDlg dialog
#include "SkinTreeCtrl.h"
class CRightDlg : public CDialog
{
// Construction
public:
	CRightDlg(CHCADOConnection& ado, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRightDlg)
	enum { IDD = IDD_DIALOG_RIGHT };
	CButton	m_checkHolidy;
	CButton	m_checkLock;
	CButton	m_checkOutPut;

	CListCtrl	m_listTime;
	CSkinTreeCtrl	m_treeRight;
	CString	m_name;
 
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRightDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRightDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonDown();
	afx_msg void OnSelRightTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelTimeGList( NMHDR * pNotifyStruct, LRESULT* result );
 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void LoadRight();
	void LoadTimeGroup();
	void LoadUser( const CString& sql );
	void Clear();
	void ShowRight(int ID );
private:
	HTREEITEM	m_hRootItem;
	HTREEITEM	m_hCurItem;
	CImageList m_treeImage;
private:
	CHCADOConnection& m_ado;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIGHTDLG_H__7FB46F08_46DD_4707_A604_1AB500ADEF78__INCLUDED_)
