#if !defined(AFX_GROUPTIMEDLG_H__D6A4D525_0BB2_4263_B7A1_C16D6CB1F76A__INCLUDED_)
#define AFX_GROUPTIMEDLG_H__D6A4D525_0BB2_4263_B7A1_C16D6CB1F76A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GroupTimeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGroupTimeDlg dialog
#include "SkinTreeCtrl.h"
class CGroupTimeDlg : public CDialog
{
// Construction
public:
	CGroupTimeDlg(CHCADOConnection& ado, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGroupTimeDlg)
	enum { IDD = IDD_DIALOG_TIME_GROUP };
	CSkinTreeCtrl	m_treeWeek;
	CSkinTreeCtrl	m_treeTG;
	CButton	m_checkW;
	CButton	m_checkY;
	CButton	m_checkM;
	CButton	m_checkD;
	CButton m_checkT;
	CButton m_checkH;
	CTime	m_start;
	CTime	m_end;
	CString	m_name;
	long	m_s_hour;
	long	m_s_mini;
	long	m_s_second;
	long	m_e_hour;
	long	m_e_mini;
	long	m_e_second;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGroupTimeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGroupTimeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonNew();
	afx_msg void OnSelTimeGroupTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonSave();
	afx_msg void OnWeekSelChk();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonUpload();
	afx_msg void OnButtonCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void LoadTimeGroup();
	void LoadWeekDay();
	void ClearSkin();
	void ShowTimeGroup(int ID );
private:
	HTREEITEM	m_hRootItem;
	HTREEITEM	m_hCurItem;
	HTREEITEM	m_hWeekFirst;
	CImageList	m_ImageTimeGroupTree;
	CImageList	m_ImageWeekCheckTree;
private:
	CHCADOConnection& m_ado;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GROUPTIMEDLG_H__D6A4D525_0BB2_4263_B7A1_C16D6CB1F76A__INCLUDED_)
