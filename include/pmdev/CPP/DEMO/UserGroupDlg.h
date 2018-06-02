#if !defined(AFX_USERGROUPDLG_H__F5D35246_41CA_413D_BD3D_44106A0C8428__INCLUDED_)
#define AFX_USERGROUPDLG_H__F5D35246_41CA_413D_BD3D_44106A0C8428__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UserGroupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// UserGroupDlg dialog

class UserGroupDlg : public CDialog
{
// Construction
public:
	UserGroupDlg(CHCADOConnection& ado,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(UserGroupDlg)
	enum { IDD = IDD_DIALOG_USERGROUP };
	CListCtrl	m_listSelUser;
	CListCtrl	m_listUser;
	CTreeCtrl	m_treeUG;
	int		m_nNormal;
	int		m_nForce;
	CString	m_Name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(UserGroupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(UserGroupDlg)
	afx_msg void OnButtonNew();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonR();
	afx_msg void OnButtonL();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelRightTree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void Clear();
	void ShowUG( int ID );
	void LoadUser( const CString& sql );
	void LoadGroup();
	HTREEITEM	m_hRootItem;
	HTREEITEM	m_hCurItem;
	CImageList m_treeImage;
private:
	CHCADOConnection& m_ado;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERGROUPDLG_H__F5D35246_41CA_413D_BD3D_44106A0C8428__INCLUDED_)
