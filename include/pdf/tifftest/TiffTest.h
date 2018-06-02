// TiffTest.h : main header file for the TIFFTEST application
//

#if !defined(AFX_TIFFTEST_H__6D2DD42A_12E8_49FF_9B5A_1FF144E0DEBA__INCLUDED_)
#define AFX_TIFFTEST_H__6D2DD42A_12E8_49FF_9B5A_1FF144E0DEBA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTiffTestApp:
// See TiffTest.cpp for the implementation of this class
//

class CTiffTestApp : public CWinApp
{
public:
	CTiffTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTiffTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CTiffTestApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIFFTEST_H__6D2DD42A_12E8_49FF_9B5A_1FF144E0DEBA__INCLUDED_)
