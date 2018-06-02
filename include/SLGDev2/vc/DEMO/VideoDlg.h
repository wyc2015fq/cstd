#if !defined(AFX_VIDEODLG_H__4AD23BF1_D70A_4E77_84A0_5FDD035D3CB8__INCLUDED_)
#define AFX_VIDEODLG_H__4AD23BF1_D70A_4E77_84A0_5FDD035D3CB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// VideoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// VideoDlg dialog

class VideoDlg : public CDialog
{
// Construction
public:
	VideoDlg(CWnd* pParent = NULL);   // standard constructor
	void Init( const CStringArray& IPArray );
// Dialog Data
	//{{AFX_DATA(VideoDlg)
	enum { IDD = IDD_DIALOG_VIDEO };
		// NOTE: the ClassWizard will add data members here
	CListBox m_IPList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(VideoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy( );
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(VideoDlg)
		// NOTE: the ClassWizard will add member functions here
	afx_msg void OnBtnVideo1();
	afx_msg void OnBtnVideo2();
	afx_msg void OnBtnVideo3();
	afx_msg void OnBtnVideo4();
	afx_msg void OnBtnVideo5();
	afx_msg void OnBtnVideo6();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CStringArray m_IPArray;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEODLG_H__4AD23BF1_D70A_4E77_84A0_5FDD035D3CB8__INCLUDED_)
