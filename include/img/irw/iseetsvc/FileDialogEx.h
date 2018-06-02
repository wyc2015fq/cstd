/********************************************************************

	FileDialogEx.h

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
	本文件用途：	图像读写引擎－C语言版－VC编译器－测试软件保存
					对话框类定义文件。

	本文件编写人：	
					leye			louxiulin##263.net
					YZ				yzfree##sina.com

	本文件版本：	30528
	最后修改于：	2003-5-28

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
		地址收集软件。

	修正历史：

		2003-5		第一个测试版发布

********************************************************************/

#if !defined(AFX_FILEDIALOGEX_H__7C8C7557_C74F_11D3_B2C1_0020AF694846__INCLUDED_)
#define AFX_FILEDIALOGEX_H__7C8C7557_C74F_11D3_B2C1_0020AF694846__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __ISEE_IRW_INC__
#include "iseeirw.h"
#endif // __ISEE_IRW_INC__

/////////////////////////////////////////////////////////////////////////////
// 定义

#define DEFAULTEXT						("bmp")

#define ISD_COMBOBOXLIST_HEIGHT			100
#define ISD_CTLCNT						(ISD_ITEM_NUM+1)

/////////////////////////////////////////////////////////////////////////////
// 新控件ID定义
#define IDC_SAVEASCNT_STC				2012
#define IDC_SAVEASCNT_CON				(IDC_SAVEASCNT_STC+ISD_ITEM_NUM+1)


/////////////////////////////////////////////////////////////////////////////
// 有保存功能的插件信息结构
typedef struct _tagSavePluginInfo
{
	int				index;								// 插件索引
	int				piid;								// 插件ID
	char			str[64];							// 显示串
	char			ext[32];							// 缺省扩展名串
	IRW_SAVE_DESC	cont;								// 原始的保存功能描述
} SAVEPLUGININFO, *LPSAVEPLUGININFO;


/////////////////////////////////////////////////////////////////////////////
// CFileDialogEx - ‘另存为’对话框类

class CFileDialogEx : public CFileDialog
{
	DECLARE_DYNAMIC(CFileDialogEx)

public:
	CFileDialogEx(LPSPEC_DATAPACK);


	LPSPEC_DATAPACK	m_pPack;							// 待保存的图象包地址

	int				m_iRunMark;							// 初始化成功标记（0－成功）
	int				m_ctrlcnt;							// 当前所选插件的新增列表个数
	SAVESTR			m_save;								// 保存参数结构
	CString			m_ext;								// 当前缺省扩展名

	CStatic			*m_pStatic[ISD_CTLCNT];				// 标签类数组
	CComboBox		*m_pComboBox[ISD_CTLCNT];			// 列表类数组

	int				m_saveinfocnt;						// 具有保存功能的插件个数
	SAVEPLUGININFO	m_saveinfo[ISEEIRW_MAX_PLUGIN_CNT];	// 有保存功能插件的信息结构数组


	int  _GetPluginIndex(LPCTSTR sou);
	int  _GetPluginIndexFromFilterIndex(int idx);
	int  _SetFileType(int index);
	void _DispLastErrMess(int err);

	IRWE_CNVMARK _GetStdIDFromBitCnt(int bitcnt);

	BOOL _OK(void);

protected:
	//{{AFX_MSG(CFileDialogEx)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEDIALOGEX_H__7C8C7557_C74F_11D3_B2C1_0020AF694846__INCLUDED_)
