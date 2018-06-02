// testmdi.h : main header file for the TESTMDI application
//

#if !defined(AFX_TESTMDI_H__5DEDEE3A_ACEC_427B_9A03_461E0E68CFB4__INCLUDED_)
#define AFX_TESTMDI_H__5DEDEE3A_ACEC_427B_9A03_461E0E68CFB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestmdiApp:
// See testmdi.cpp for the implementation of this class
//

class CTestmdiApp : public CWinApp
{
public:
	CTestmdiApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestmdiApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTestmdiApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTMDI_H__5DEDEE3A_ACEC_427B_9A03_461E0E68CFB4__INCLUDED_)
