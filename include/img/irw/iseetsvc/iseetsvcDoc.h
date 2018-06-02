/********************************************************************

	iseetsvcDoc.h

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
	本文件用途：	图像读写引擎－C语言版－VC编译器－测试软件文档管理
					类定义文件。

	本文件编写人：	
					YZ				yzfree##sina.com

	本文件版本：	30118
	最后修改于：	2003-1-18

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
		地址收集软件。

	修正历史：

		2003-1		第一个测试版发布

********************************************************************/

#if !defined(AFX_ISEETSVCDOC_H__68859D6C_A067_4883_8E30_0761C83E4C50__INCLUDED_)
#define AFX_ISEETSVCDOC_H__68859D6C_A067_4883_8E30_0761C83E4C50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CIseetsvcDoc : public CDocument
{
protected: // create from serialization only
	CIseetsvcDoc();
	DECLARE_DYNCREATE(CIseetsvcDoc)

// Attributes
public:
	CShellPath		m_spath;							// ShellPath容器

	LPSPEC_DATAPACK	m_pPack;							// 当前专业型数据包
	BOOL			m_bPkst;							// 当前数据包可操作状态
	BITMAPINFO_EX	m_dInfo;							// 显示用信息

// Operations
public:
	BOOL OnIdleProc(LONG count);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIseetsvcDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CIseetsvcDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	static CDisplay		*m_pView;						// 视类
	static HWND			m_hView;						// 视窗口句柄

	static int __cdecl _cbfun(LPSPEC_DATAPACK psdp, PFUN_COMM comm, int imgnum, int sl, int lcnt, int rw);
	

// Generated message map functions
protected:
	//{{AFX_MSG(CIseetsvcDoc)
	afx_msg void OnFileSaveAs();
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ISEETSVCDOC_H__68859D6C_A067_4883_8E30_0761C83E4C50__INCLUDED_)
