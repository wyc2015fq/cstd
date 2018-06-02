#if !defined(AFX_DIALOGBAREX_H__D614B256_C5EC_11D2_B8C5_B41E04C10000__INCLUDED_)
#define AFX_DIALOGBAREX_H__D614B256_C5EC_11D2_B8C5_B41E04C10000__INCLUDED_

#if _MSC_VER >= 1000 // Defined as 1200 for Visual C++ 6.0
#pragma once
#endif

//////////////////////////////////////////////////////////////////////////////////////
//			Class written by Sharad Kelkar drssk@ad1.vsnl.net.in					//
//	This is freeware without any kind of restriction on usage and distribution.		//
//////////////////////////////////////////////////////////////////////////////////////
class CDialogBarEx : public CDialogBar 
{
public:
	CDialogBarEx();
	virtual ~CDialogBarEx();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual void OnInitDialogBar () ;

// Generated message map functions
protected:
	//{{AFX_MSG(CDialogBarEx)
	afx_msg void InitDialogBarHandler( WORD wParam , DWORD  lParam );
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGBAREX_H__D614B256_C5EC_11D2_B8C5_B41E04C10000__INCLUDED_)
