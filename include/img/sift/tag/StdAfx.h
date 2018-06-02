// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__73245384_05C8_4F72_A998_B6C017EC9B0F__INCLUDED_)
#define AFX_STDAFX_H__73245384_05C8_4F72_A998_B6C017EC9B0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

// #include <afxwin.h>         // MFC core and standard components
// #include <afxext.h>         // MFC extensions
// #include <afxdisp.h>        // MFC Automation classes
// #include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
// #ifndef _AFX_NO_AFXCMN_SUPPORT
// #include <afxcmn.h>			// MFC support for Windows Common Controls
// #endif // _AFX_NO_AFXCMN_SUPPORT
#include <windows.h>  
#include "cv.h"
#include "cxcore.h" 
#include "cvaux.h" 
#include "highgui.h"

#define  dst_width  100	//图像缩放尺寸的宽
#define  dst_height	100//缩放尺寸的高
#define  siftratio  0.75//sift匹配选择阈值参数（越大标准越严格，但匹配点越少）
#define BYTE unsigned char
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__73245384_05C8_4F72_A998_B6C017EC9B0F__INCLUDED_)
