// Zheng_DFA.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CZheng_DFAApp:
// See Zheng_DFA.cpp for the implementation of this class
//

class CZheng_DFAApp : public CWinApp
{
public:
	CZheng_DFAApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CZheng_DFAApp theApp;