#if !defined(AFX_DEVSTATISDLG_H__6E7563A2_B5AB_40CA_B0C7_5E540958FA1B__INCLUDED_)
#define AFX_DEVSTATISDLG_H__6E7563A2_B5AB_40CA_B0C7_5E540958FA1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DevStatisDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DevStatisDlg dialog

class DevStatisDlg : public CDialog
{
// Construction
public:
	DevStatisDlg(const char* ip , CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(DevStatisDlg)
	enum { IDD = IDD_DIALOG_DEVSTATIS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DevStatisDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DevStatisDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg LRESULT OnUpdate(WPARAM wparam, LPARAM lpparam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	//static void DevOperResult( const char* server_addr,  const char* dev_addr, NET_OPER_CMD oper_type, NET_OPER_RESULT eRt, void* oper_content, int serial_num, void* param );
    static BOOL CALLBACK DevOperResult( const char* cszDevAddr, DEV_OPER_TYPE eType, DEV_OPER_FLAG eFlag, void* pvContent, int nSeq, DEV_OPER_RESULT eResult, void* pvContext );
protected:
	void DevStatisInfoNotify( const char* dev_addr, const DEV_STATIS& dev_statis );
private:
	char m_serverIP[16];
	char m_IP[16];

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVSTATISDLG_H__6E7563A2_B5AB_40CA_B0C7_5E540958FA1B__INCLUDED_)
