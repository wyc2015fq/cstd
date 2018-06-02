// test_venaMFC.h : main header file for the TEST_VENAMFC application
//

#if !defined(AFX_TEST_VENAMFC_H__A5EE73BF_83CA_41E8_9D58_83C5EDE81C53__INCLUDED_)
#define AFX_TEST_VENAMFC_H__A5EE73BF_83CA_41E8_9D58_83C5EDE81C53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTest_venaMFCApp:
// See test_venaMFC.cpp for the implementation of this class
//

class CTest_venaMFCApp : public CWinApp
{
public:
	CTest_venaMFCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTest_venaMFCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTest_venaMFCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_VENAMFC_H__A5EE73BF_83CA_41E8_9D58_83C5EDE81C53__INCLUDED_)
