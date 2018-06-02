// VideoClient.h : main header file for the VIDEOCLIENT application
//

#if !defined(AFX_VIDEOCLIENT_H__36628848_B963_48D8_A952_0C050F8EDC47__INCLUDED_)
#define AFX_VIDEOCLIENT_H__36628848_B963_48D8_A952_0C050F8EDC47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CVideoClientApp:
// See VideoClient.cpp for the implementation of this class
//

class CVideoClientApp : public CWinApp
{
public:
	CVideoClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVideoClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVideoClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIDEOCLIENT_H__36628848_B963_48D8_A952_0C050F8EDC47__INCLUDED_)
