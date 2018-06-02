#if !defined(AFX_UDDLG_H__403859D2_C2C3_4E39_84EE_FF27D021E3D7__INCLUDED_)
#define AFX_UDDLG_H__403859D2_C2C3_4E39_84EE_FF27D021E3D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UDDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUDDlg dialog
#define OPER_TIMEGROUP  0x01
#define OPER_RIGHT		0x02
#define OPER_USERGROUP  0x03
class CUDDlg : public CDialog
{
// Construction
public:
	CUDDlg(CHCADOConnection& ado,const char* dev_addr, int flag, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUDDlg)
	enum { IDD = IDD_DIALOG_UDLOAD };
	CListCtrl	m_listTree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUDDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUDDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonUpload();
	afx_msg void OnButtonDownload();
	afx_msg void OnButtonDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
//	void Recv( const char* dev_addr, DEV_OPER_TYPE oper_type, DEV_OPER_FLAG oper_flag, void* oper_content, int serial_num, DEV_OPER_RESULT rt );
private:
// 	void TimeGroupInfo( const char* dev_addr, const DEV_TIMEGROUP_ARRAY& time_group );
// 	void RightInfo( const char* dev_addr, const DEV_RIGHT_ARRAY& right );
// 	void UserGroupInfo( const char* dev_addr, const DEV_USERGROUP_ARRAY& group );
private:
	void LoadTimegroup();
	void LoadRight();
	void LoadUserGroup();
	void UpLoadTimeGroup();
	void UpLoadRight();
	void UpUserGroup();
	void DownLoadTimeGroup();
	void DownLoadRight();
	void DownUserGroup();

	void DownDelTimeGroup();
	void DownDelRight();
	void DelUserGroup();
private:
	char m_IP[16];
	int  m_flag;
	CHCADOConnection& m_ado;
	int  m_nItems;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDDLG_H__403859D2_C2C3_4E39_84EE_FF27D021E3D7__INCLUDED_)
