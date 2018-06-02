#if !defined(AFX_SELECTFILEDLG_H__4A93386C_2009_4DCC_BD14_519FDA9FDCEF__INCLUDED_)
#define AFX_SELECTFILEDLG_H__4A93386C_2009_4DCC_BD14_519FDA9FDCEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectFileDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectFileDlg dialog

class CSelectFileDlg : public CFileDialog
{
// Construction
public:
	CSelectFileDlg(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

// Dialog Data
	//{{AFX_DATA(CSelectFileDlg)
	enum { IDD = IDD_SELECTFILE_PREVIEW };
	CStatic	m_Preview;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectFileDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSelectFileDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTFILEDLG_H__4A93386C_2009_4DCC_BD14_519FDA9FDCEF__INCLUDED_)
