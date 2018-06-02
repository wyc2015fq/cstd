// testsdi.h : main header file for the TESTSDI application
//

#if !defined(AFX_TESTSDI_H__363889B2_B71C_4CAF_9F21_1DE34E7B2AC9__INCLUDED_)
#define AFX_TESTSDI_H__363889B2_B71C_4CAF_9F21_1DE34E7B2AC9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestsdiApp:
// See testsdi.cpp for the implementation of this class
//

class CTestsdiApp : public CWinApp
{
public:
	CTestsdiApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestsdiApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTestsdiApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTSDI_H__363889B2_B71C_4CAF_9F21_1DE34E7B2AC9__INCLUDED_)
