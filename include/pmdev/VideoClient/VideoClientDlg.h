// VideoClientDlg.h : header file
//

#if !defined(AFX_VIDEOCLIENTDLG_H__97E7BD51_FF34_4F0A_AECF_CCEE1DD5E8AB__INCLUDED_)
#define AFX_VIDEOCLIENTDLG_H__97E7BD51_FF34_4F0A_AECF_CCEE1DD5E8AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CVideoClientDlg dialog
#include "VideoSDK.h"

class CVideoClientDlg : public CDialog
{
// Construction
public:
	CVideoClientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CVideoClientDlg)
	enum { IDD = IDD_VIDEOCLIENT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoClientDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CVideoClientDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	VideoSDK m_objVideoSDK;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEOCLIENTDLG_H__97E7BD51_FF34_4F0A_AECF_CCEE1DD5E8AB__INCLUDED_)
