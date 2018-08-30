// testA20.h : main header file for the TESTA20 application
//

#if !defined(AFX_TESTA20_H__D4884D70_B81B_4C4F_B1A1_C07AAC301A4E__INCLUDED_)
#define AFX_TESTA20_H__D4884D70_B81B_4C4F_B1A1_C07AAC301A4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestA20App:
// See testA20.cpp for the implementation of this class
//

class CTestA20App : public CWinApp
{
public:
	CTestA20App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestA20App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestA20App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTA20_H__D4884D70_B81B_4C4F_B1A1_C07AAC301A4E__INCLUDED_)
