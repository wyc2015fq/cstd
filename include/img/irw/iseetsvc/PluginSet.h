#if !defined(AFX_PLUGINSET_H__903A6D4E_2BEF_4B30_823B_74679356E9CF__INCLUDED_)
#define AFX_PLUGINSET_H__903A6D4E_2BEF_4B30_823B_74679356E9CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PluginSet.h : header file
//

#define MAX_PLUGIN_NUM		256

/////////////////////////////////////////////////////////////////////////////
// CPluginSet dialog

class CPluginSet : public CDialog
{
// struct
public:
	typedef struct _tagLData
	{
		ulong		id;							// 插件ID
		LPIRWMF_INFOBLOCK	pol;						// 插件信息块原地址
		int					iost;						// 插件原状态
		int					inst;						// 插件新状态
	} LDATA, *PLDATA;

	enum {PLINSTATUS_NUL, PLINSTATUS_NOUSE, PLINSTATUS_USE, PLINSTATUS_ACT};

// Construction
public:
	CPluginSet(CWnd* pParent = NULL);   // standard constructor

	void SetData(LPIRWMF_HEADER pih);

// Dialog Data
	//{{AFX_DATA(CPluginSet)
	enum { IDD = IDD_DLG_PINFO };
	CButton	m_btnUpdate;
	CButton	m_btnAll;
	CButton	m_btnUse;
	CButton	m_btnFree;
	CListCtrl	m_cList;
	CString	m_sPluginPath;
	CString	m_sDBG;
	CString	m_sEngVer;
	CString	m_sPliVer;
	//}}AFX_DATA

	LPIRWMF_HEADER	pirwhead;
	LDATA			plda[MAX_PLUGIN_NUM];
	int				plicnt;
	int				selcnt;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPluginSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void _init_list(void);

	// Generated message map functions
	//{{AFX_MSG(CPluginSet)
	virtual BOOL OnInitDialog();
	afx_msg void OnItemchangedListPlugin(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnFree();
	afx_msg void OnBtnSel();
	afx_msg void OnBtnSelall();
	afx_msg void OnBtnUpdate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLUGINSET_H__903A6D4E_2BEF_4B30_823B_74679356E9CF__INCLUDED_)
