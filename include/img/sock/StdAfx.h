// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__CF537AD1_7A62_468E_86D5_39A068AB0717__INCLUDED_)
#define AFX_STDAFX_H__CF537AD1_7A62_468E_86D5_39A068AB0717__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#define _OUTPUT_INFO_

#ifdef _DEBUG
#define _OUTPUT_INFO_
#endif

#ifdef WIN32
#pragma warning( disable : 4786 )

// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include "DebugAssistant.h"
#include <windows.h>
#endif

// TODO: reference additional headers your program requires here

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__CF537AD1_7A62_468E_86D5_39A068AB0717__INCLUDED_)
