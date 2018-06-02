/********************************************************************

	ShellList.h

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
	本文件用途：	图像读写引擎－C语言版－VC编译器－测试软件ShellList
					控件类定义文件。

	本文件编写人：	
					YZ				yzfree##sina.com

	本文件版本：	30220
	最后修改于：	2003-2-20

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
		地址收集软件。

	修正历史：

		2003-2		第一个测试版发布

********************************************************************/

#if !defined(AFX_SHELLLIST_H__F535BD83_1ACB_4492_8ED1_CD1BF1171241__INCLUDED_)
#define AFX_SHELLLIST_H__F535BD83_1ACB_4492_8ED1_CD1BF1171241__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ShellList.h : header file
//
#include <shlobj.h>
#include "ShellPidl.h"

class CShellTree;
class CShellPath;


/////////////////////////////////////////////////////////////////////////////
// CShellList window

class CShellList : public CListCtrl, public CShellPidl
{
// Construction
public:
	CShellList();

	enum {MAX_SUBITEMCNT = 1};							// 填充线程发送消息量的上限
	enum {HITTEST_POS_X = 4, HITTEST_POS_Y = 4};		// 测试点
	enum {COLNUM = 4};									// 列表视的列数
	enum {APTIMEUNIT = 10};								// 自动播放延迟最小单位（毫秒）

	enum Cols											// 列标题索引
	{
		COL_NAME,
		COL_SIZE,
		COL_TYPE,
		COL_NOTE,
		COL_MAX
	};

	typedef struct _tagSibCol							// 列参数类型
	{
		enum Cols	index;
		char		name[64];
		int			ali;
		int			width;
	} SIBCOL, *LPSIBCOL;


// Attributes
public:
	int		InitShellList(CShellTree *pst, CShellPath *psp, CString filter);
	void	CloseShellList(void);
	
	void	EnableImages(void);

	LRESULT OnFillBegin(WPARAM wParam, LPARAM lParam);
	LRESULT OnFillEnd(WPARAM wParam, LPARAM lParam);
	LRESULT OnSubItemDataFill(WPARAM wParam, LPARAM lParam);
	LRESULT OnAutoNext(WPARAM wParam, LPARAM lParam);

	CString	&GetFilterString(void);
	void	SetFilterString(CString &nsp);
	BOOL	IsDestFile(LPCTSTR fname);

	int		GetAutoPlay(void);
	void	SetAutoPlay(int type);

// Operations
public:

private:
	static CWinThread	*m_pThread;						// 填充线程

	static int			m_iSN;							// 当前批次号
	static int			m_iNum;							// 当前批次的文件总个数
	static int			m_iCount;						// 未处理的填充命令个数
	static int			m_iCurrPt;						// 顺序方式的下一个可能的填充点

	static int			m_iAutoPlayTime;				// 自动播放图象的延迟时间（秒）
	static int			m_iAutoPlay;					// 自动播放标志（非0为自动播放）

	static UINT ShellLProc(LPVOID parm);

	static int  CALLBACK CFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);


// Overrides
	
// Overrides	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShellList)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShellList();

	// Generated message map functions
	//{{AFX_MSG(CShellList)
	afx_msg void OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeleteallitems(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	static CShellTree			*m_pTree;				// 树控件指针
	static CShellPath			*m_pPath;				// 容器指针
	static CString				m_sFilter;				// 列表项过滤串
	static CString				m_sCurrPath;			// 当前路径
	
	static SIBCOL				sc[COLNUM];

	friend class CShellTree;

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHELLLIST_H__F535BD83_1ACB_4492_8ED1_CD1BF1171241__INCLUDED_)
