/********************************************************************

	ShellPidl.h

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
	本文件用途：	图像读写引擎－C语言版－VC编译器－测试软件shell pidl
					类定义文件。

	本文件编写人：	
					Selom Ofori		none
					YZ				yzfree##sina.com

	本文件版本：	30315
	最后修改于：	2003-3-15

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
		地址收集软件。
	----------------------------------------------------------------
	引用注释：

	本文件的原始作者为 Selom Ofori，源码没有许可限制。后由YZ改写，并入
	ISee项目。许可证设为GPL。因 Selom Ofori 没有留下联系地址，所以未能
	征求他的意见。

	修正历史：

		2003-3		修改列表视项的私有数据结构（引入ISee数据包变量）
		2003-1		第一个测试版发布

********************************************************************/

#if !defined(AFX_SHELLPIDL_H__INCLUDED_)
#define AFX_SHELLPIDL_H__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include <shlobj.h>

// 列表项数据结构中的数据状态
enum {SPTS_EMPTY = 0, SPTS_FATT = 1, SPTS_IMGINF = 2};


// 列表项私有数据结构
typedef struct tagLVID
{
	int							type;				// 项类型（保留）

	WIN32_FILE_ATTRIBUTE_DATA	wfad;				// 项的文件属性

	DWORD						width;				// 图象的宽、高、位深度数
	DWORD						height;
	DWORD						bitcount;

	LPSHELLFOLDER				lpsfParent;			// 父文件夹IShellFolder接口
	LPITEMIDLIST				lpi;				// 文件的相对PIDL

	UINT						state;				// 项的数据状态(enum SPTS_XXX)

	char						rev[1016];			// 保留，可能将包含缩略图数据
} LVITEMDATA, *LPLVITEMDATA;



// 树项私有数据结构
typedef struct tagID
{
	int				type;							// 项类型（保留，用于未来扩展）
	LPSHELLFOLDER	lpsfParent;						// 父文件夹IShellFolder接口
	LPITEMIDLIST	lpi;							// 相对于父文件夹的‘相对’PIDL
	LPITEMIDLIST	lpifq;							// 相对于根文件夹的‘绝对’PIDL
} TVITEMDATA, *LPTVITEMDATA;


class CShellPidl
{
public:

public:

    LPITEMIDLIST	ConcatPidls(LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2);
    LPITEMIDLIST	GetFullyQualPidl(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi);
    LPITEMIDLIST	CopyITEMID(LPMALLOC lpMalloc, LPITEMIDLIST lpi);
    BOOL			GetName(LPSHELLFOLDER lpsf, LPITEMIDLIST  lpi, DWORD dwFlags, LPSTR lpFriendlyName);
    LPITEMIDLIST	CreatePidl(UINT cbSize);
    UINT			GetSize(LPCITEMIDLIST pidl);
    LPITEMIDLIST	Next(LPCITEMIDLIST pidl);

	BOOL	DoTheMenuThing(HWND hwnd, LPSHELLFOLDER lpsfParent, LPITEMIDLIST  lpi, LPPOINT lppt);
	int		GetItemIcon(LPITEMIDLIST lpi, UINT uFlags);
};

#endif // !defined(AFX_SHELLTREE_H__INCLUDED_)
