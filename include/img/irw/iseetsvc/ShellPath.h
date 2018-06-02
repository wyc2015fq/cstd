/********************************************************************

	ShellPath.h

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
	本文件用途：	图像读写引擎－C语言版－VC编译器－测试软件shell path
					类定义文件。

	本文件编写人：	
					YZ				yzfree##sina.com

	本文件版本：	30309
	最后修改于：	2003-3-9

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
		地址收集软件。

	修正历史：

		2003-3		增加CShellImageList类，以加快文件图标的提取速度
		2003-2		第一个测试版发布

********************************************************************/

#if !defined(AFX_SHELLPATH_H__7549C0D3_70B0_49BE_9E8D_7B7DA9DE832D__INCLUDED_)
#define AFX_SHELLPATH_H__7549C0D3_70B0_49BE_9E8D_7B7DA9DE832D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ShellPath.h : header file

// 引用类声明
class CShellTree;
class CShellList;

/////////////////////////////////////////////////////////////////////////////
// CShellImageList

// 列表视所使用的图标列表类
class CShellImageList
{
public:
	enum {CSIL_MAXREVICONUM = 8, CSIL_ICONGROW = 256};
	enum {CSILIDX_VDOC = 0, CSILIDX_VEXE};

	CShellImageList();
	~CShellImageList();

	int OpenImageList(void);
	int CloseImageList(void);

	CImageList *GetLIconList(void){return &m_cLIcon;};
	CImageList *GetSIconList(void){return &m_cSIcon;};

	int GetIconIndex(HWND hwnd, LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, LPCSTR filename);

private:
	typedef struct _tagSHELLICONINFO
	{
		char	szFileExt[MAX_PATH];
		int		index;
	} SHELLICONINFO, *PSHELLICONINFO;

	int			m_iLIconCX;
	int			m_iLIconCY;
	int			m_iSIconCX;
	int			m_iSIconCY;

	CArray<SHELLICONINFO, SHELLICONINFO&>	m_info;		// 扩展名与图标索引对应关系表

	CImageList	m_cLIcon;								// 大图标列表
	CImageList	m_cSIcon;								// 小图标列表
};


/////////////////////////////////////////////////////////////////////////////
// CShellPath command target

enum {ISSHELLPATH_MAX_IDLTIM = 10};						// 文件选择变更的最大延迟时间


class CShellPath : public CCmdTarget
{
	DECLARE_DYNCREATE(CShellPath)

public:
	CShellPath();           // protected constructor used by dynamic creation
	virtual ~CShellPath();
	
// Attributes
public:
	CShellImageList	m_cImageList;						// 列表控件图标列表

	CShellTree		*m_pTree;							// 树控件指针   
	CShellList		*m_pList;							// 列表控件指针 
	CFrameWnd		*m_pFwnd;							// 框架窗口指针 

	int				m_iCurrIdx;							// 当前文件在列表视中的索引
	LPLVITEMDATA	m_pCurrItemData;					// 当前文件的列表项私有结构地址
	CString			m_sCurrPath;						// 当前路径
	CString			m_sCurrFile;						// 当前文件名

	BOOL					a_bChangeTag;				// 对象变更标志
	CString					a_cName;					// 对象名称
	CString					a_cPath;					// 路径+文件名
	DWORD					a_dParam1;					// 附加参数1
	DWORD					a_dParam2;					// 附加参数2
	CTimeCounter			a_cTime;					// 延迟计时
	DWORD					a_dNumber;					// 流水号
	DWORD					a_dSN;						// 批次号


// Operations
public:
	int  InitShellPath(CTreeCtrl *ptc, CListCtrl *plc, CFrameWnd *pfw, CString filter);
	void CloseShellPath(void);

	int         SetInitPath(LPCSTR path);
	CString     GetCurrPath(void);

	CShellTree *GetShellTree(void);
	CShellList *GetShellList(void);
	
	CString	   &GetFilterString(void);
	void        SetFilterString(CString &nsp);

	void		ObjectChanged(LPCTSTR path, LPCTSTR fname, int idx = -1);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShellPath)
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShellPath)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHELLPATH_H__7549C0D3_70B0_49BE_9E8D_7B7DA9DE832D__INCLUDED_)
