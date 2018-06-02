/********************************************************************

	MainFrm.h

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
	本文件用途：	图像读写引擎－C语言版－VC编译器－测试软件框架窗口
					类定义文件。

	本文件编写人：	
					YZ				yzfree##sina.com

	本文件版本：	30112
	最后修改于：	2003-1-12

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
		地址收集软件。

	修正历史：

		2003-1		第一个测试版发布

********************************************************************/

#if !defined(AFX_MAINFRM_H__93899ABB_8D39_435E_935E_B7EBD5994DA0__INCLUDED_)
#define AFX_MAINFRM_H__93899ABB_8D39_435E_935E_B7EBD5994DA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "newspliwnd.h"
#include "shellpath.h"


class CRightView;


class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CNewSpliWnd		m_spa;								// 分裂窗口A
	CNewSpliWnd		m_spb;								// 分裂窗口B

// Operations
public:
	BOOL OnIdleProc(LONG count);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
	CRightView* GetRightPane();
	CLeftView*	GetLeftPane();
	CDisplay*	GetDisplayPane();


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CStatusBar		m_wndStatusBar;
	CProgressCtrl	m_wndProgress;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnVatCenter();
	afx_msg void OnUpdateVatCenter(CCmdUI* pCmdUI);
	afx_msg void OnVatLd();
	afx_msg void OnUpdateVatLd(CCmdUI* pCmdUI);
	afx_msg void OnVatLt();
	afx_msg void OnUpdateVatLt(CCmdUI* pCmdUI);
	afx_msg void OnVatRd();
	afx_msg void OnUpdateVatRd(CCmdUI* pCmdUI);
	afx_msg void OnVatRt();
	afx_msg void OnUpdateVatRt(CCmdUI* pCmdUI);
	afx_msg void OnViewPlaytype();
	afx_msg void OnUpdateViewPlaytype(CCmdUI* pCmdUI);
	afx_msg void OnVrtAuto();
	afx_msg void OnUpdateVrtAuto(CCmdUI* pCmdUI);
	afx_msg void OnVrtL200();
	afx_msg void OnUpdateVrtL200(CCmdUI* pCmdUI);
	afx_msg void OnVrtL300();
	afx_msg void OnUpdateVrtL300(CCmdUI* pCmdUI);
	afx_msg void OnVrtL400();
	afx_msg void OnUpdateVrtL400(CCmdUI* pCmdUI);
	afx_msg void OnVrtOrg();
	afx_msg void OnUpdateVrtOrg(CCmdUI* pCmdUI);
	afx_msg void OnVrtS20();
	afx_msg void OnUpdateVrtS20(CCmdUI* pCmdUI);
	afx_msg void OnVrtS40();
	afx_msg void OnUpdateVrtS40(CCmdUI* pCmdUI);
	afx_msg void OnVrtS60();
	afx_msg void OnUpdateVrtS60(CCmdUI* pCmdUI);
	afx_msg void OnVrtS80();
	afx_msg void OnUpdateVrtS80(CCmdUI* pCmdUI);
	afx_msg void OnViewPlugininfo();
	afx_msg void OnUpdateViewPlugininfo(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnUpdateViewStyles(CCmdUI* pCmdUI);
	afx_msg void OnViewStyle(UINT nCommandID);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__93899ABB_8D39_435E_935E_B7EBD5994DA0__INCLUDED_)
