/********************************************************************

	ShellTree.h

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
	本文件用途：	图像读写引擎－C语言版－VC编译器－测试软件shell tree
					类定义文件。

	本文件编写人：	
					Selom Ofori		none
					YZ				yzfree##sina.com

	本文件版本：	30119
	最后修改于：	2003-1-19

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
		地址收集软件。
	----------------------------------------------------------------
	引用注释：

	本文件的原始作者为 Selom Ofori，源码没有许可限制。后由YZ改写，并入
	ISee项目。许可证设为GPL。因 Selom Ofori 没有留下联系地址，所以未能
	征求他的意见。

	修正历史：

		2003-1		第一个测试版发布

********************************************************************/

#if !defined(AFX_SHELLTREE_H__6B1818E3_8ADA_11D1_B10E_40F603C10000__INCLUDED_)
#define AFX_SHELLTREE_H__6B1818E3_8ADA_11D1_B10E_40F603C10000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <shlobj.h>
#include "ShellPidl.h"

class CShellList;
class CShellPath;

#define	SHELLTREE_DEFER			60						// 延迟时间（10毫秒单位）

class CShellTree : public CTreeCtrl, public CShellPidl
{
public:
	enum FindAttribs{type_drive,type_folder,type_path};


protected:
	typedef struct _tagTREEMSGPACK
	{
		LPSHELLFOLDER	ppsf;							// 当前命令的IShellFolder接口
		LPITEMIDLIST	pidlfq;							// 当前命令的绝对PIDL
		TVITEM			item;							// 当前命令的TVITEM结构数据
	} TREEMSGPACK, *LPTREEMSGPACK;


// Construction
public:
	CShellTree();

	int		InitShellTree(CShellList *psl, CShellPath *psp);
	void	CloseShellTree(void);
	
// Attributes
public:
	void	GetContextMenu(NMHDR* pNMHDR, LRESULT* pResult);
	void	EnableImages();
	BOOL	GetSelectedFolderPath(CString &szFolderPath);

	LPSHELLFOLDER	GetParentShellFolder(HTREEITEM folderNode);
	LPITEMIDLIST	GetRelativeIDLIST(HTREEITEM folderNode);
	LPITEMIDLIST	GetFullyQualifiedIDLIST(HTREEITEM folderNode);
	int				GetPathFromHTREEITEM(HTREEITEM folderNode, LPTSTR szBuff);

// Operations
public:

	int 	PopulateTree(CString &);

	bool	SearchTree(HTREEITEM treeNode, CString szSearchName, FindAttribs attr);
	void	TunnelTree(CString szFindPath);

	void	OnFolderExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	BOOL	OnFolderSelected(NMHDR* pNMHDR, LRESULT* pResult, CString &szFolderPath);
	void	OnDeleteShellItem(NMHDR* pNMHDR, LRESULT* pResult);
	LRESULT OnDeferFillAtt(WPARAM wParam, LPARAM lParam);
	int		OnUpdateItem(WPARAM wParam, LPARAM lParam);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShellTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShellTree();

// Generated message map functions
protected:
	const enum ISHFILLTYPE{ ISHTYPE_TREE = 1, ISHTYPE_LIST = 2	};

	int 		FillShellView(LPSHELLFOLDER lpsf, LPITEMIDLIST lpifq, HTREEITEM hParent, int list);
	void		GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem);
	HTREEITEM	SearchByText(HTREEITEM hti, CString szFindText);

 	static  int CALLBACK TreeViewCompareProc(LPARAM, LPARAM, LPARAM);

	
private:
	static UINT ShellTProc(LPVOID parm);

	static CWinThread			*g_pwt;
	static LPMALLOC				g_malloc;
	
	static CShellList			*g_pList;
	static CShellPath			*g_pPath;

protected:
	//{{AFX_MSG(CShellTree)
	afx_msg LRESULT OnDeferFill(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	friend class CShellList;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHELLTREE_H__6B1818E3_8ADA_11D1_B10E_40F603C10000__INCLUDED_)
