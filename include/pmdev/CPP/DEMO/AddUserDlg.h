#if !defined(AFX_ADDUSERDLG_H__ED463DC0_5633_437B_98A1_D49A6D027897__INCLUDED_)
#define AFX_ADDUSERDLG_H__ED463DC0_5633_437B_98A1_D49A6D027897__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddUserDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// AddUserDlg dialog

class AddUserDlg : public CDialog
{
// Construction
public:
	AddUserDlg(CHCADOConnection& ado,char* ID=NULL, CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(AddUserDlg)
	enum { IDD = IDD_DIALOG_USER };
	CComboBox	m_cmbRight;
	CComboBox	m_cmbUserGroup;
	CString	m_ID;
	CString	m_Card;
	CString	m_Dep;
	CString	m_Name;
	CString	m_invalideTime;
	int     m_userFlag;
	int		m_index;
	int     m_Right;
	int     m_UserType;
	int     m_UserGroup;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AddUserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(AddUserDlg)
	afx_msg void OnButtonSave();
	afx_msg void OnButtonCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboRight();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CHCADOConnection& m_ado;

	char* m_pUserID;
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDUSERDLG_H__ED463DC0_5633_437B_98A1_D49A6D027897__INCLUDED_)
