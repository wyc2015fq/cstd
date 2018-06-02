/********************************************************************

	ShellPath.cpp

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
					类实现文件。

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

#include "stdafx.h"
#include "iseetsvc.h"

#include "ShellPidl.h"
#include "ShellTree.h"
#include "ShellList.h"
#include "ShellPath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CShellImageList

CShellImageList::CShellImageList()
{
	m_info.RemoveAll();

	m_cLIcon.m_hImageList = 0;
	m_cSIcon.m_hImageList = 0;
}


CShellImageList::~CShellImageList()
{
	m_info.RemoveAll();

	if (m_cLIcon.m_hImageList)
	{
		m_cLIcon.DeleteImageList();
	}

	if (m_cSIcon.m_hImageList)
	{
		m_cSIcon.DeleteImageList();
	}
}


// 初始化图象列表类
int CShellImageList::OpenImageList()
{
	ASSERT(m_cLIcon.m_hImageList == 0);

	m_info.RemoveAll();
	m_info.SetSize(0, CSIL_ICONGROW);

	m_iLIconCX = GetSystemMetrics(SM_CXICON);			// 获取系统大、小图标的尺寸
	m_iLIconCY = GetSystemMetrics(SM_CYICON);
	m_iSIconCX = GetSystemMetrics(SM_CXSMICON);
	m_iSIconCY = GetSystemMetrics(SM_CYSMICON);

	if (!m_cLIcon.Create(m_iLIconCX, m_iLIconCY, ILC_COLOR32|ILC_MASK, CSIL_MAXREVICONUM, CSIL_ICONGROW))
	{
		return -1;										// 大图标图象列表创建失败
	}

	if (!m_cSIcon.Create(m_iSIconCX, m_iSIconCY, ILC_COLOR32|ILC_MASK, CSIL_MAXREVICONUM, CSIL_ICONGROW))
	{
		m_cLIcon.DeleteImageList();
		m_cLIcon.m_hImageList = 0;
		return -2;										// 小图标图象列表创建失败
	}

	m_cLIcon.SetBkColor(CLR_NONE);
	m_cSIcon.SetBkColor(CLR_NONE);

	m_cLIcon.SetImageCount(CSIL_MAXREVICONUM);			// 预留CSIL_MAXREVICONUM个保留图标位置
	m_cSIcon.SetImageCount(CSIL_MAXREVICONUM);

	m_cLIcon.Replace(CSILIDX_VDOC, AfxGetApp()->LoadIcon(IDI_SHELLICON1));
	m_cLIcon.Replace(CSILIDX_VEXE, AfxGetApp()->LoadIcon(IDI_SHELLICON2));

	m_cSIcon.Replace(CSILIDX_VDOC, AfxGetApp()->LoadIcon(IDI_SHELLICON1));
	m_cSIcon.Replace(CSILIDX_VEXE, AfxGetApp()->LoadIcon(IDI_SHELLICON2));

	return 0;
}


// 关闭图象列表类
int CShellImageList::CloseImageList()
{
	m_info.RemoveAll();
	
	if (m_cLIcon.m_hImageList)
	{
		m_cLIcon.DeleteImageList();
		m_cLIcon.m_hImageList = 0;
	}
	
	if (m_cSIcon.m_hImageList)
	{
		m_cSIcon.DeleteImageList();
		m_cSIcon.m_hImageList = 0;
	}

	return 0;
}


// 获取指定对象在列表中的图标索引号
int CShellImageList::GetIconIndex(HWND hwnd, LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, LPCSTR filename)
{
	static int	iMdiPos = 0;							// 分类图标与个性图标的索引分界点

	ASSERT(lpsf&&lpi&&filename);

	IExtractIcon	*piei;
	
	char	szFileIcon[MAX_PATH];
	INT		fiIndex;
	UINT	fiComp;
	int		index = -3;

	// 获取对象的IExtractIcon接口
	if (FAILED(lpsf->GetUIObjectOf(hwnd, 1, (const struct _ITEMIDLIST **)&lpi, IID_IExtractIcon, 0, (LPVOID*)&piei)))
	{
		return -1;
	}

	// 获取对象图标所在的位置
	if (FAILED(piei->GetIconLocation(GIL_FORSHELL, (LPSTR)szFileIcon, MAX_PATH, &fiIndex, &fiComp)))
	{
		piei->Release();
		return -2;
	}

	HICON	hLIcon;
	HICON	hSIcon;
	SHELLICONINFO	cShellIcon;
	
	cShellIcon.index = -1;
	cShellIcon.szFileExt[0] = '\0';

	// 直接处理那些拥有自己图标的对象
	if (!(fiComp & GIL_PERCLASS))
	{
		// 搜索该类型对象的图标是否已被记录
		for (int i=iMdiPos; i<m_info.GetSize(); i++)
		{
			if (lstrcmpi((LPCSTR)m_info[i].szFileExt, (LPCSTR)filename) == 0)
			{
				piei->Release();
				return m_info[i].index;
			}
		}
		
		// 提取图标
		if (piei->Extract((LPCSTR)szFileIcon, fiIndex, &hLIcon, &hSIcon, (m_iLIconCX&0xffff)|((m_iSIconCX<<16)&0xffff0000)) != NOERROR)
		{
			piei->Release();
			return CShellImageList::CSILIDX_VEXE;
		}

		// 加入大图标列表
		if ((index=m_cLIcon.Add(hLIcon)) == -1)
		{
			piei->Release();
			DestroyIcon(hLIcon);
			DestroyIcon(hSIcon);
			return CShellImageList::CSILIDX_VEXE;
		}

		// 加入小图标列表
		if (index != m_cSIcon.Add(hSIcon))
		{
			m_cLIcon.Remove(index);
			piei->Release();
			DestroyIcon(hLIcon);
			DestroyIcon(hSIcon);
			return CShellImageList::CSILIDX_VEXE;
		}

		// 释放从系统获得的图标句柄
		DestroyIcon(hLIcon);
		DestroyIcon(hSIcon);

		cShellIcon.index = index;

		// 保存完整的对象名称
		lstrcpy((LPSTR)cShellIcon.szFileExt, (LPCSTR)filename);
		
		// 将图标的索引及对象名信息进行记录
		m_info.Add(cShellIcon);
	}
	else
	{
		// 定位对象扩展名的位置
		char *p = strrchr((const char *)filename, (int)(unsigned int)(uchar)'.');

		// 搜索该类型对象的图标是否已被记录
		for (int i=0; i<iMdiPos; i++)
		{
			if (lstrcmpi((LPCSTR)m_info[i].szFileExt, (LPCSTR)p) == 0)
			{
				piei->Release();
				return m_info[i].index;
			}
		}

		// 提取图标
		if (piei->Extract((LPCSTR)szFileIcon, fiIndex, &hLIcon, &hSIcon, (m_iLIconCX&0xffff)|((m_iSIconCX<<16)&0xffff0000)) != NOERROR)
		{
			piei->Release();
			return CShellImageList::CSILIDX_VDOC;
		}
		
		// 加入大图标列表
		if ((index=m_cLIcon.Add(hLIcon)) == -1)
		{
			piei->Release();
			DestroyIcon(hLIcon);
			DestroyIcon(hSIcon);
			return CShellImageList::CSILIDX_VDOC;
		}
		
		// 加入小图标列表
		if (index != m_cSIcon.Add(hSIcon))
		{
			m_cLIcon.Remove(index);
			piei->Release();
			DestroyIcon(hLIcon);
			DestroyIcon(hSIcon);
			return CShellImageList::CSILIDX_VDOC;
		}

		// 释放从系统获得的图标句柄
		DestroyIcon(hLIcon);
		DestroyIcon(hSIcon);
		
		cShellIcon.index = index;

		// 只保存分类图标对象的扩展名串
		lstrcpy((LPSTR)cShellIcon.szFileExt, (LPCSTR)p);
		
		// 将图标的索引及对象名信息进行记录
		m_info.InsertAt(0, cShellIcon);

		// 更新分界点(类图标与自有图标的分界点)
		iMdiPos++;
	}

	piei->Release();

	return index;
}



/////////////////////////////////////////////////////////////////////////////
// CShellPath

IMPLEMENT_DYNCREATE(CShellPath, CCmdTarget)

CShellPath::CShellPath()
{
	m_pTree = 0;
	m_pList = 0;
	m_pFwnd = 0;

	m_iCurrIdx      = -1;
	m_pCurrItemData = 0;
	
	a_bChangeTag = FALSE;
	m_sCurrPath.Empty();
	m_sCurrFile.Empty();
	a_cPath.Empty();

	a_bChangeTag = FALSE;								// 对象变更标志
	a_dParam1    = 0;									// 附加参数1
	a_dParam2    = 0;									// 附加参数2
	a_dNumber    = 0;									// 流水号
	a_dSN        = 0;									// 批次号

	a_cPath.Empty();									// 路径+文件名
	a_cName.Empty();									// 对象名称
}

CShellPath::~CShellPath()
{
	m_pTree = 0;
	m_pList = 0;
	m_pFwnd = 0;
}


BEGIN_MESSAGE_MAP(CShellPath, CCmdTarget)
	//{{AFX_MSG_MAP(CShellPath)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CShellPath message handlers

// 初始化ShellPath容器
int CShellPath::InitShellPath(CTreeCtrl *ptc, CListCtrl *plc, CFrameWnd *pfw, CString filter)
{
	int		ret = 0;

	ASSERT(ptc&&plc&&pfw);

	if (m_cImageList.OpenImageList())
	{
		return -1;
	}

	m_pTree   = (CShellTree*)ptc;						// 树控件指针
	m_pList   = (CShellList*)plc;						// 列表控件指针
	m_pFwnd   = (CFrameWnd*)pfw;						// 框架窗口指针
	
	// 初始化树控件
	if ((ret=m_pTree->InitShellTree(m_pList, this)) != 0)
	{
		return ret;
	}

	// 初始化列表控件
	if ((ret=m_pList->InitShellList(m_pTree, this, filter)) != 0)
	{
		return ret;
	}

	// 开启树控件图标
	m_pTree->EnableImages();

	// 开启列表控件图标
	m_pList->EnableImages();

	m_sCurrPath.Empty();
	m_sCurrFile.Empty();

	a_bChangeTag = FALSE;								// 对象变更标志
	a_dParam1    = 0;									// 附加参数1
	a_dParam2    = 0;									// 附加参数2
	a_dSN        = 0;									// 批次号
	
	a_cPath.Empty();									// 路径+文件名
	a_cName.Empty();									// 对象名称
	
	return ret;
}


// 关闭ShellPath容器
void CShellPath::CloseShellPath()
{
	m_pList->CloseShellList();
	m_pTree->CloseShellTree();

	m_pTree = 0;
	m_pList = 0;
	m_pFwnd = 0;

	m_cImageList.CloseImageList();
}


// 设置ShellPath类的初始路径
int CShellPath::SetInitPath(LPCSTR path)
{
	ASSERT(m_sCurrPath.IsEmpty());						// 该函数不能重复调用
	ASSERT(m_pTree);

	m_sCurrPath = path;

	// 组装树
	return m_pTree->PopulateTree(m_sCurrPath);
	//0     - 成功
	//-1    - 失败，未能获取桌面文件夹接口
	//-2    - 失败，未能获取桌面文件夹的PIDL
	//-3    - 失败，未能获取桌面文件夹显示名
}


// 获取当前路径
CString CShellPath::GetCurrPath()
{
	return m_sCurrPath;
}


// 获取树控件指针
inline CShellTree *CShellPath::GetShellTree()
{
	return m_pTree;
}


// 获取列表控件指针
inline CShellList *CShellPath::GetShellList()
{
	return m_pList;
}


// 获取当前的文件过滤串
CString	&CShellPath::GetFilterString()
{
	return m_pList->GetFilterString();
}


// 设置文件过滤串
void CShellPath::SetFilterString(CString &nsp)
{
	m_pList->SetFilterString(nsp);
}


// 路径或文件名更改
void CShellPath::ObjectChanged(LPCTSTR path, LPCTSTR fname, int idx)
{
	ASSERT(path||fname);

	// 合成变更数据（路径与文件名不能同时变更）
	if (path)
	{
		m_sCurrPath = path;
		m_sCurrFile = "";
		m_iCurrIdx  = -1;
			
		a_cName     = "";
		a_cPath     = "";
		a_dParam1   = 0;
		a_dParam2   = 0;
		a_dNumber   ++;
		a_dSN       = idx;								// 当前批次号

		m_pCurrItemData = 0;
	}
	else if (fname)
	{
		ASSERT(!m_sCurrPath.IsEmpty());
		m_sCurrFile = fname;
		m_iCurrIdx  = idx;

		a_cName     = m_sCurrFile;
		a_cPath     = m_sCurrPath + m_sCurrFile;
		a_dParam1   = 0;
		a_dParam2   = 0;
		a_dNumber   ++;

		// 取得该文件的列表项私有数据
		m_pCurrItemData = (LPLVITEMDATA)m_pList->GetItemData(idx);
	}

	a_bChangeTag= TRUE;

	// 延迟计时开始
	a_cTime.BeginCount(ISSHELLPATH_MAX_IDLTIM);
}

