// Eagleeye2_Viewer.h : main header file for the EAGLEEYE2_VIEWER application
//

#if !defined(AFX_EAGLEEYE2_VIEWER_H__BA674760_A435_4FDD_9C03_F13DE0552832__INCLUDED_)
#define AFX_EAGLEEYE2_VIEWER_H__BA674760_A435_4FDD_9C03_F13DE0552832__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CEagleeye2_ViewerApp:
// See Eagleeye2_Viewer.cpp for the implementation of this class
//

class CEagleeye2_ViewerApp : public CWinApp
{
public:
	CEagleeye2_ViewerApp();

public:
	DWORD	m_dwCpuFreq;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEagleeye2_ViewerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CEagleeye2_ViewerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EAGLEEYE2_VIEWER_H__BA674760_A435_4FDD_9C03_F13DE0552832__INCLUDED_)
