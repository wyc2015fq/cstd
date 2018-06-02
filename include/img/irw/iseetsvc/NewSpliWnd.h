/********************************************************************

	NewSpliWnd.h

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
	本文件用途：	图像读写引擎－C语言版－VC编译器－测试软件新分裂窗口
					类定义文件。

	本文件编写人：	
					YZ				yzfree##sina.com

	本文件版本：	30122
	最后修改于：	2003-1-22

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
		地址收集软件。

	修正历史：

		2003-1		第一个测试版发布

********************************************************************/

#if !defined(AFX_NEWSPLIWND_H__204CD775_5896_403B_B353_025F36CD77FF__INCLUDED_)
#define AFX_NEWSPLIWND_H__204CD775_5896_403B_B353_025F36CD77FF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewSpliWnd.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CNewSpliWnd frame with splitter

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#define CNSW_FILL_WIDTH_CURTAIL			(0)
#define CNSW_FILL_HEIGHT_CURTAIL		(-2)
#define CNSW_FILL_WIDTH_MINI			(4)
#define CNSW_FILL_HEIGHT_MINI			(16)

#define CNSW_LINE_CURTAIL				(6)


class CNewSpliWnd : public CSplitterWnd
{
	DECLARE_DYNCREATE(CNewSpliWnd)
public:
	CNewSpliWnd();           // protected constructor used by dynamic creation

// Attributes
protected:
	int		count;
	CRect	rc;

public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewSpliWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg);
	virtual void OnInvertTracker(const CRect& rect);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNewSpliWnd();

	// Generated message map functions
	//{{AFX_MSG(CNewSpliWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWSPLIWND_H__204CD775_5896_403B_B353_025F36CD77FF__INCLUDED_)
