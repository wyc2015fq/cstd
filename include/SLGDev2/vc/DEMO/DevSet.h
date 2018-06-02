#if !defined(AFX_DEVSET_H__82096225_7C37_4B38_B819_156015EAECA3__INCLUDED_)
#define AFX_DEVSET_H__82096225_7C37_4B38_B819_156015EAECA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DevSetDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DevSetDlg dialog

class DevSetDlg : public CDialog
{
// Construction
public:
	DevSetDlg(CHCADOConnection& ado, const char* ip, CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(DevSetDlg)
	enum { IDD = IDD_DIALOG_DEVSET };
		// NOTE: the ClassWizard will add data members here
	CComboBox m_cmbValide;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DevSetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DevSetDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonDown();
	afx_msg void OnClose();
	afx_msg void OnBNValide();
	afx_msg LRESULT OnUpdate(WPARAM wparam, LPARAM lpparam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	//static void DevOperResult( const char* server_addr, const char* dev_addr, NET_OPER_CMD oper_type, NET_OPER_RESULT eRt, void* oper_content, int serial_num, void* param );
    static BOOL CALLBACK DevOperResult( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult, void* pvContext );
protected:
	void DevWorkAttInfoNotify( const char* dev_addr, const DEV_WORKATT& dev_att );
private:
	void InitCtrl();
	char m_IP[64];
	char m_ServerIP[64];
	CHCADOConnection& m_ado;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVSET_H__82096225_7C37_4B38_B819_156015EAECA3__INCLUDED_)
