/********************************************************************

	iseetsvc.h

	----------------------------------------------------------------
    软件许可证 － GPL
	版权所有 (C) 2003 VCHelp coPathway ISee workgroup.
	----------------------------------------------------------------
	这一程序是自由软件，你可以遵照自由软件基金会出版的GNU 通用公共许
	可证条款来修改和重新发布这一程序。或者用许可证的第二版，或者（根
	据你的选择）用任何更新的版本。

    发布这一程序的目的是希望它有用，但没有任何担保。甚至没有适合特定
	目地的隐含的担保。更详细的情况请参阅GNU通用公共许可证。

    你应该已经和程序一起收到一份GNU通用公共许可证的副本（本目录
	GPL.txt文件）。如果还没有，写信给：
    The Free Software Foundation, Inc.,  675  Mass Ave,  Cambridge,
    MA02139,  USA
	----------------------------------------------------------------
	如果你在使用本软件时有什么问题或建议，请用以下地址与我们取得联系：

			http://isee.126.com
			http://cosoft.org.cn/projects/iseeexplorer
			
	或发信到：

			isee##vip.163.com
	----------------------------------------------------------------
	本文件用途：	图像读写引擎－C语言版－VC编译器－测试软件框架
					类定义文件。

	本文件编写人：	
					YZ				yzfree##sina.com

	本文件版本：	30318
	最后修改于：	2003-3-18

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
		地址收集软件。

	修正历史：

		2003-3		增加高精度计时器类
		2003-1		第一个测试版发布

********************************************************************/

#if !defined(AFX_ISEETSVC_H__4D09C7E7_8005_440E_B040_D14C2BA6F470__INCLUDED_)
#define AFX_ISEETSVC_H__4D09C7E7_8005_440E_B040_D14C2BA6F470__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

class CIseetsvcApp;

/////////////////////////////////////////////////////////////////////////////
// CTimeCounter - 高精度计时器类定义

class CTimeCounter
{
public:
	CTimeCounter();
	~CTimeCounter();

	void BeginCount(DWORD space = 0);
	BOOL IsEnd(void);
	DWORD GetTime(void);

protected:
	LARGE_INTEGER	m_liStart;
	LARGE_INTEGER	m_liEnd;
	LARGE_INTEGER	m_liFrequ;
	DWORD			m_dwSpace;
};


/////////////////////////////////////////////////////////////////////////////
// CLogRecorder - 操作记录类定义

class CLogRecorder : private CFile
{
public:
	CLogRecorder();
	~CLogRecorder();

	int	OpenRecorder(LPCSTR logname);
	int	CloseRecorder(void);

	int	Record(LPCSTR str);
	int	Record(LPCSTR str, int cont);
	int	Record(LPCSTR str, int cont, int ipar);
	int	Record(LPCSTR str, int cont, LPCSTR ppar);

protected:
	static char	clr_welcome[];
	static char	clr_spek[];
};


// 获取全局记录类地址
CLogRecorder & ISeeTsVCDebugStrRecorder(void);


/////////////////////////////////////////////////////////////////////////////
// CIseetsvcApp:
// See iseetsvc.cpp for the implementation of this class
//

class CIseetsvcApp : public CWinApp
{
public:
	CIseetsvcApp();

	DWORD GetDllVersion(LPCTSTR lpszDllName);

// Attrib
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CLogRecorder	m_clRecord;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIseetsvcApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL OnIdle(LONG lCount);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CIseetsvcApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISEETSVC_H__4D09C7E7_8005_440E_B040_D14C2BA6F470__INCLUDED_)
