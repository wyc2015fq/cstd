/********************************************************************

	ShellTree.cpp

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
					类实现文件。

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

		2003-3		修改填充列表视的方式（引入ISee引擎接口调用）
		2003-1		第一个测试版发布

********************************************************************/

#include "stdafx.h"
#include "iseetsvc.h"

#include "ShellPidl.h"
#include "ShellTree.h"
#include "ShellList.h"
#include "ShellPath.h"

#include "Display.h"
#include "LeftView.h"
#include "RightView.h"

#include "MainFrm.h"
#include "IseetsvcDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


////////////////////////////////////////////////////////////////////////////////////////
// CShellTree类静态成员变量初始化

CWinThread			*CShellTree::g_pwt   = NULL;		// 延迟线程的对象指针
LPMALLOC			CShellTree::g_malloc = NULL;		// IMalloc接口

CShellList			*CShellTree::g_pList = 0;			// 列表控件指针
CShellPath			*CShellTree::g_pPath = 0;			// 上级容器指针




////////////////////////////////////////////////////////////////////////////////////////
// CShellTree类接口实现

CShellTree::CShellTree()
{
}

CShellTree::~CShellTree()
{
}


BEGIN_MESSAGE_MAP(CShellTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CShellTree)
	ON_MESSAGE(WM_TVN_DEFERFILL, OnDeferFill)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/****************************************************************************
*
*    函数: ShellTProc(LPVOID parm)
*
*    参数：parm - CShellTree类的this值
*
*    用途: 延迟线程，当用户用键盘修改了选择项时，CShellTree类将延迟填充子
*          文件夹信息
*
****************************************************************************/
UINT CShellTree::ShellTProc(LPVOID parm)
{
	CShellTree		*pst = (CShellTree*)parm;
	LPTREEMSGPACK	ptmp = 0;
	MSG				msg;
	MSG				msgtmp;
	BOOL			bRet;
	int				i;

	// 通知主线程，并创建本线程的消息队列
	pst->PostMessage(WM_TVN_PROCCREATE);

	// 消息循环
	while((bRet=::GetMessage(&msg, NULL, 0, 0)) != 0)
    { 
        if (bRet == -1)
        {
            continue;
        }
        else
        {
			if (msg.message == WM_TVN_QUIT)				// 退出消息
			{
				break;
			}
			else if (msg.message == WM_TVN_QUEUECLR)	// 清除消息
			{
				continue;
			}
			else if (msg.message == WM_TVN_DEFER)		// 延迟消息
			{
				// 提取命令参数包
				ptmp = (LPTREEMSGPACK)msg.lParam;

				if (!ptmp)
				{
					continue;
				}

				// 延迟指定的时间（SHELLTREE_DEFER * 10ms）
				for (i=0; i<SHELLTREE_DEFER; i++)
				{
					// 如果有新的命令到来，则废弃当前正在延迟的命令
					if (::PeekMessage(&msgtmp, NULL, WM_TVN_QUIT, WM_TVN_DEFER, PM_NOREMOVE))
					{
						ptmp->ppsf->Release();
						delete ptmp;
						ptmp = 0;
						break;
					}
					// 延迟一个时间单位（10ms）
					::Sleep(10);
				}

				if (i < SHELLTREE_DEFER)
				{
					continue;							// 直接返回，去获取新的命令
				}

				// 向主线程发送填充消息
				pst->PostMessage(WM_TVN_DEFERFILL, 0, (LPARAM)ptmp);

				ptmp = 0;
			}
			else
			{
				continue;
			}
        }
    } 
	
	// 退出线程
	::AfxEndThread(0, TRUE);

	return 0;
}


/****************************************************************************
*
*    函数: InitShellTree()
*
*    用途: 初始化CShellTree，并申请一些必要的资源
*
*    入口：psl - 对应的列表控制类的指针
*          psp - 容器类指针
*
*    返回：0     - 成功
*          -1	 - 失败，Shell内存分配接口申请失败
*          -2    - 失败，延迟线程创建失败
*
****************************************************************************/
int CShellTree::InitShellTree(CShellList *psl, CShellPath *psp)
{
	ISeeTsVCDebugStrRecorder().Record("开始初始化 ShellTree ...", 0, 0);

	// 申请Shell的内存分配接口
	if (FAILED(::SHGetMalloc(&g_malloc)))
	{
		return -1;										// 获取Shell分配接口失败
	}

	// 创建延迟线程
	if (!(g_pwt=::AfxBeginThread(ShellTProc, this, THREAD_PRIORITY_NORMAL, 0, 1, NULL)))
	{
		g_malloc->Release();
		g_malloc = 0;
		return -2;										// 创建延迟线程失败
	}

	ASSERT(psl&&psp);

	g_pList = psl;										// 初始化列表控制及容器类指针
	g_pPath = psp;

	::ResumeThread(g_pwt->m_hThread);					// 激活线程

	ISeeTsVCDebugStrRecorder().Record("ShellTree 初始化完毕。", 0, 0);

	return 0;
}


/****************************************************************************
*
*    函数: CloseShellTree()
*
*    用途: 释放CShellTree中的资源
*
****************************************************************************/
void CShellTree::CloseShellTree()
{
	DeleteAllItems();

	// 销毁延迟线程
	::PostThreadMessage(g_pwt->m_nThreadID, WM_TVN_QUIT, 0, 0);

	// 等待延迟线程退出
	::WaitForSingleObject(g_pwt->m_hThread ,INFINITE);

	g_pwt    = NULL;

	// 释放IMalloc接口
	g_malloc->Release();
	g_malloc = 0;
	
	g_pList  = 0;										// 列表控件指针
	g_pPath  = 0;										// 上级容器指针
	
	ISeeTsVCDebugStrRecorder().Record("ShellTree 关闭。", 0, 0);

	return;
}


/****************************************************************************
*
*    函数: PopulateTree()
*
*    用途: 从系统Shell的根（桌面）组装目录树
*
*    入口：cszLastSel    - 指定当前的选择项（即路径串），可以为空（缺省选择）
*
*    返回：0     - 成功
*          -1    - 失败，未能获取桌面文件夹接口
*          -2    - 失败，未能获取桌面文件夹的PIDL
*          -3    - 失败，未能获取桌面文件夹显示名
*
****************************************************************************/
int CShellTree::PopulateTree(CString &cszLastSel)
{

	HTREEITEM		hti;
	TV_ITEM         tvi;
	TV_INSERTSTRUCT tvins;
	TV_SORTCB		tvscb;

	LPSHELLFOLDER	lpsf   = NULL;
	LPITEMIDLIST	lpi    = NULL;
	LPITEMIDLIST	lpifq  = NULL; 
	LPTVITEMDATA    lptvid = NULL;

	char			szBuff[MAX_PATH];

	ISeeTsVCDebugStrRecorder().Record("ShellTree 开始组装路径 <%s>。", 0, (LPCSTR)cszLastSel);

	// 获取桌面文件夹接口
	if (FAILED(::SHGetDesktopFolder(&lpsf)))
	{
		return -1;
	}

	// 删除以前树中所有的项
	DeleteAllItems();

	// 获取根元素的标识符列表（PIDL）
	if (FAILED(::SHGetSpecialFolderLocation(0, CSIDL_DESKTOP, &lpi)))
	{
		lpsf->Release();
		return -2;
	}

	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_CHILDREN;

	// 获取元素显示名
	if (GetName(lpsf, lpi, SHGDN_NORMAL, szBuff) == 0)
	{
		g_malloc->Free(lpi);
		lpsf->Release();
		return -3;
	}

	// 填写显示名及设置有子文件夹标志
	tvi.pszText    = szBuff;
	tvi.cchTextMax = MAX_PATH;
	tvi.cChildren  = 1;

	// 获取与根元素关联的图标索引
	GetNormalAndSelectedIcons(lpi, &tvi);

	// 私有数据定为空，因为根项是虚拟的（这是一个特例）
	tvi.lParam = (LPARAM)0;

	tvins.item         = tvi;
	tvins.hInsertAfter = 0;
	tvins.hParent      = 0;

	// 插入根元素
	hti = InsertItem(&tvins);

	// 填充树
	if (FillShellView(lpsf, lpi, hti, ISHTYPE_TREE) == 0)
	{
		tvscb.hParent     = hti;
		tvscb.lParam      = 0;
		tvscb.lpfnCompare = TreeViewCompareProc;
		
		// 将次根项按显示名排序
		SortChildrenCB(&tvscb);
		
		// 展开次根项
		Expand(hti, TVE_EXPAND);

		if (cszLastSel.GetLength() == 0)
		{
			// 获取‘我的文档’树项句柄
			hti = GetChildItem(hti);
		
			// 展开并选择‘我的文档’文件夹
			Expand(hti, TVE_EXPAND);
			Select(hti, TVGN_CARET);
		}
		else
		{
			TunnelTree(cszLastSel);
		}
	}
	else
	{
		Select(hti, TVGN_CARET);
	}

	// 释放IShellFolder接口及根项PIDL
	lpsf->Release();
	g_malloc->Free(lpi);

	ISeeTsVCDebugStrRecorder().Record("ShellTree 路径 <%s> 组装完毕。", 0, (LPCSTR)cszLastSel);

	return 0;
}


/****************************************************************************
*
*  函数: FillShellView(LPSHELLFOLDER lpsf, LPITEMIDLIST lpifq, HTREEITEM hParent, int type)
*
*  用途: 填充一个文件夹下的子文件夹项。给定一个文件夹枚举其下的子文件夹，
*        然后填入树中。
*
*  入口:
*    lpsf         - 待填充文件夹的IShellFolder接口
*    lpifq        - 待填充文件夹的绝对PIDL
*    hParent      - 父项的节点
*    type         - 填充类型
*
****************************************************************************/
int CShellTree::FillShellView(LPSHELLFOLDER lpsf, LPITEMIDLIST lpifq, HTREEITEM hParent, int type)
{
    TV_ITEM         tvi;
	LV_ITEM			lvi;
    TV_INSERTSTRUCT tvins;

    HTREEITEM       hPrev = NULL;
    LPENUMIDLIST    lpe   = NULL;
    LPITEMIDLIST    lpi   = NULL;
	LPITEMIDLIST    lpiTemp       = NULL;
	LPITEMIDLIST    lpifqThisItem = NULL;
    LPTVITEMDATA    lptvid        = NULL;

    UINT            uCount = 0;
	UINT			uIcon  = 0;
	int				lcnt   = 0;
	int				lindx  = 0;
    ULONG           ulFetched;
	ULONG			ulAttrs;
    char            szBuff[MAX_PATH];
    HWND            hwnd;

	static			char phroot[MAX_PATH];


	hwnd = ::GetParent(m_hWnd);

	if (type&ISHTYPE_LIST)
	{
		AfxGetApp()->DoWaitCursor(1);

		if ((g_pList->GetStyle()&LVS_TYPEMASK) == LVS_ICON)
		{
			uIcon = SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_LARGEICON | SHGFI_ICON;
		}
		else
		{
			uIcon = SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_ICON;
		}

		phroot[0] = '\0';

		::SHGetPathFromIDList(lpifq, (LPSTR)phroot);

		if (strlen(phroot))
		{
			if (phroot[strlen(phroot)-1] != '\\')
			{
				strcat((char*)phroot, "\\");
			}
		}
		else
		{
			strcat((char*)phroot, "\\");
		}
		
		g_pList->SendMessage(WM_LVN_FILLBEGIN, (WPARAM)phroot);
	}

	DWORD	obj = 0;

	if (type&ISHTYPE_TREE)
	{
		obj |= SHCONTF_FOLDERS;
	}
	if (type&ISHTYPE_LIST)
	{
		obj |= SHCONTF_NONFOLDERS;
	}

    // 获取IEnumIDList接口，并枚举其中的指定元素（文件夹或/和非文件夹元素）
    if (SUCCEEDED(lpsf->EnumObjects(hwnd, obj, &lpe)))
    {
        // 枚举文件夹下的子文件夹和/或文件对象
        while (S_OK == lpe->Next(1, &lpi, &ulFetched))
        {
            ulAttrs = SFGAO_FOLDER | SFGAO_FILESYSANCESTOR | SFGAO_FILESYSTEM | SFGAO_HASSUBFOLDER;

            // 获取元素属性
            lpsf->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lpi, &ulAttrs);

			// 不包含或不是文件系统的对象将被忽略
			if (!(ulAttrs & (SFGAO_FILESYSANCESTOR | SFGAO_FILESYSTEM)))
			{
				g_malloc->Free(lpi);
				lpi = 0;
				continue;
			}

			// 文件夹处理
			if (ulAttrs & SFGAO_FOLDER)
			{
				if (type&ISHTYPE_TREE)
				{
					memset((void*)&tvi, 0, sizeof(TV_ITEM));
					
					tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;
					
					// 判断元素是否拥有子文件夹（视中该项左侧有+号），当用户第一次展开
					// 该项时我们再组装它。
					if (ulAttrs & SFGAO_HASSUBFOLDER)
					{
						tvi.cChildren = 1;
						tvi.mask |= TVIF_CHILDREN;
					}
					
					// 申请树项的私有数据结构（TVITEMDATA）
					if((lptvid=(LPTVITEMDATA)g_malloc->Alloc(sizeof(TVITEMDATA))) == NULL)
					{
						g_malloc->Free(lpi);
						lpi = 0;
						continue;
					}
					
					// 获取文件夹‘显示名’串
					if (GetName(lpsf, lpi, SHGDN_NORMAL, szBuff) == FALSE)
					{
						g_malloc->Free(lptvid);
						g_malloc->Free(lpi);
						lptvid = 0;
						lpi = 0;
						continue;
					}
					
					tvi.pszText    = szBuff;
					tvi.cchTextMax = MAX_PATH;
					
					// 链接两个PIDL，生成新的、绝对的PIDL
					lpifqThisItem = ConcatPidls(lpifq, lpi);
					
					// 复制一个‘相对’PIDL到私有数据结构
					lptvid->lpi = CopyITEMID(g_malloc, lpi);
					
					GetNormalAndSelectedIcons(lpifqThisItem, &tvi);
					
					// 保存父文件夹接口（COM引用累加）
					lptvid->lpsfParent = lpsf;
					lpsf->AddRef();
					
					// 将‘绝对’PIDL写入私有数据结构
					lptvid->lpifq = lpifqThisItem;
					lpifqThisItem = 0;
					
					tvi.lParam = (LPARAM)lptvid;
					
					// 插入一个树项
					tvins.item         = tvi;
					tvins.hInsertAfter = hPrev;
					tvins.hParent      = hParent;
					
					hPrev = InsertItem(&tvins);
				}
			}
			// 文件处理
			else if (ulAttrs & SFGAO_FILESYSTEM)
			{
				if (type&ISHTYPE_LIST)
				{
					memset((void*)&lvi, 0, sizeof(LV_ITEM));
					
					lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
					
					// 获取文件‘显示名’串 
					if (GetName(lpsf, lpi, SHGDN_INFOLDER|SHGDN_FORPARSING, szBuff) == FALSE)
					{
						g_malloc->Free(lpi);
						lpi = 0;
						continue;
					}
					
					// 判断是否需要将这个文件插入文件列表
					if (g_pList->IsDestFile((LPCTSTR)szBuff) == FALSE)
					{
						g_malloc->Free(lpi);
						lpi = 0;
						continue;
					}

					// 申请项的私有数据结构
					if ((lvi.lParam=(LPARAM) new LVITEMDATA) == 0)
					{
						g_malloc->Free(lpi);
						lpi = 0;
						continue;
					}
					
					// 初始化项私有数据结构
					memset((void*)lvi.lParam, 0, sizeof(LVITEMDATA));

					// 数据状态设为空
					((LPLVITEMDATA)lvi.lParam)->state = SPTS_EMPTY;
					((LPLVITEMDATA)lvi.lParam)->lpi   = CopyITEMID(g_malloc, lpi);
					((LPLVITEMDATA)lvi.lParam)->lpsfParent = lpsf;
					lpsf->AddRef();

					lvi.iItem      = lcnt++;
					lvi.pszText    = szBuff;
					lvi.cchTextMax = MAX_PATH;
					
					// 暂时采用未知类型图标
					lvi.iImage     = 0;
					
					// 插入一个列表项
					lindx = g_pList->InsertItem((const LVITEM *)&lvi);
				}
			}
            g_malloc->Free(lpi);
            lpi = 0;

        }
		lpe->Release();
		lpe = 0;
    }

	if (type&ISHTYPE_LIST)
	{
		g_pList->SendMessage(WM_LVN_FILLEND, (WPARAM)lcnt);
		
		AfxGetApp()->DoWaitCursor(-1);
	}
	
	return 0;
}


/****************************************************************************
*
*    函数: GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem)
*
*    用途:  为指定树项获取正常和选择状态下的、在系统图标表中的索引
*
*    入口:
*      lpifq    - 树项的绝对PIDL
*      lptvitem - 指向树项的TV_ITEM结构，返回的索引号将填写在该结构的iImage和
*                 iSelectedImage成员变量中。
*
****************************************************************************/
void CShellTree::GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem)
{
   // 在这我们不去检测GetItemIcon()的返回值是否正确，因为如果该函数失败，我们就会
   // 有大麻烦......
   // YZ 注：上面是sam的原话，下面两行代码确有改进的余地。

   lptvitem->iImage         = GetItemIcon(lpifq, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_ICON);
   lptvitem->iSelectedImage = GetItemIcon(lpifq, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON  | SHGFI_ICON);
   
   return;
}


/****************************************************************************
*
*    函数: TreeViewCompareProc(LPARAM, LPARAM, LPARAM)
*
*    用途:  排序功能的回调函数
*
****************************************************************************/
int CALLBACK CShellTree::TreeViewCompareProc(LPARAM lparam1, LPARAM lparam2, LPARAM lparamSort)
{
    LPTVITEMDATA lptvid1=(LPTVITEMDATA)lparam1;
    LPTVITEMDATA lptvid2=(LPTVITEMDATA)lparam2;
    HRESULT   hr;

    if (FAILED(hr=lptvid1->lpsfParent->CompareIDs(0,lptvid1->lpi,lptvid2->lpi)))
	{
		return 0;
	}

    return (short)SCODE_CODE(GetScode(hr));
}




/////////////////////////////////////////////////////////////////////////////
// CShellTree message handlers

/****************************************************************************
*
*	函数:	OnFolderExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
*
*	用途:	扩展指定的树项。本函数应在响应TVN_ITEMEXPANDING消息时调用。
*
*	注释:	
*
*	映射:	TVN_ITEMEXPANDING
*
****************************************************************************/
void CShellTree::OnFolderExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPTVITEMDATA   lptvid;
	LPSHELLFOLDER  lpsf2 = NULL;
	TV_SORTCB      tvscb;

	NM_TREEVIEW* pnmtv = (NM_TREEVIEW*)pNMHDR;

	// 如果该文件夹曾经被扩展过则无需进行下面的填充操作
    if ((pnmtv->itemNew.state & TVIS_EXPANDEDONCE))
	{
		*pResult = 0;
		return;
	}

	// 取出树项的私有数据结构
    lptvid = (LPTVITEMDATA)pnmtv->itemNew.lParam;

    if (lptvid)
	{
		// 获得待扩展文件夹的IShellFolder接口
		if (SUCCEEDED(lptvid->lpsfParent->BindToObject(lptvid->lpi, 0, IID_IShellFolder,(LPVOID *)&lpsf2)))
		{
			// 填充文件夹目录信息
			FillShellView(lpsf2, lptvid->lpifq, pnmtv->itemNew.hItem, ISHTYPE_TREE);

			lpsf2->Release();
		}

		tvscb.hParent     = pnmtv->itemNew.hItem;
		tvscb.lParam      = 0;
		tvscb.lpfnCompare = TreeViewCompareProc;

		// 按名称对子目录排序
		SortChildrenCB(&tvscb);
	}	
	
	*pResult = 0;
}


/****************************************************************************
*
*	函数:	GetContextMenu(NMHDR* pNMHDR, LRESULT* pResult) 
*
*	用途:	为一选定的树项弹出右键菜单。本函数应在响应NM_RCLICK通知时调用。
*
*	映射:	NM_RCLICK;
*
****************************************************************************/
void CShellTree::GetContextMenu(NMHDR* pNMHDR, LRESULT* pResult) 
{
	POINT			pt;
	LPTVITEMDATA	lptvid = NULL;
	LPSHELLFOLDER	lpsf2  = NULL;

	TV_HITTESTINFO	tvhti;
	TV_ITEM			tvi;

	static char		szBuff[MAX_PATH];
	

	// 获取鼠标当前坐标
	::GetCursorPos((LPPOINT)&pt);
	ScreenToClient(&pt);

	tvhti.pt = pt;
    HitTest(&tvhti);

    SelectItem(tvhti.hItem);

	// 只有当事件发生点在项的显示名及图标上时才弹出菜单
	if (tvhti.flags & (TVHT_ONITEMLABEL|TVHT_ONITEMICON))
	{
		ClientToScreen(&pt);

		tvi.mask  = TVIF_PARAM;
		tvi.hItem = tvhti.hItem;
		
        if (!GetItem(&tvi))
		{
			return;
		}
		
		// 取得私有数据结构
		lptvid = (LPTVITEMDATA)tvi.lParam;
		
		if (lptvid)
		{
			// 创建并显示右键菜单
			DoTheMenuThing(::GetParent(m_hWnd),	lptvid->lpsfParent, lptvid->lpi, &pt);
		}
	}
	
	*pResult = 0;
}


/****************************************************************************
*
*	函数:	OnFolderSelected(NMHDR* pNMHDR, LRESULT* pResult, CString &szFolderPath) 
*
*	用途:	当用户选择了一个文件夹时应调用本函数（TVN_SELCHANGED）。如果该文件夹
*           是文件系统的一部分，则本函数将返回它的路径。
*
*	映射:	TVN_SELCHANGED
*
****************************************************************************/
BOOL CShellTree::OnFolderSelected(NMHDR* pNMHDR, LRESULT* pResult, CString &szFolderPath) 
{
	HRESULT			hr;
	LPTVITEMDATA	lptvid;

	LPSHELLFOLDER	lpsf2  = NULL;
	HTREEITEM		hItem  = NULL;
	BOOL			bRet   = FALSE;
	TV_SORTCB		tvscb;
	int				obj;

	static char		szBuff[MAX_PATH];

	NM_TREEVIEW* pnmtv = (NM_TREEVIEW*)pNMHDR;

	if((hItem = GetSelectedItem()))
	{
		// 取得选择项的私有数据结构
		lptvid = (LPTVITEMDATA)GetItemData(hItem);

		// 如果该项没有私有数据则不再进行进一步的处理（比如根项）
		if (lptvid && lptvid->lpsfParent && lptvid->lpi)
		{
			// 获取选择项的IShellFolder接口
			hr = lptvid->lpsfParent->BindToObject(lptvid->lpi, 0, IID_IShellFolder, (LPVOID *)&lpsf2);

			if (SUCCEEDED(hr))
			{
				ULONG ulAttrs = SFGAO_FILESYSTEM;

				// 判断选择项是否属于文件系统
				lptvid->lpsfParent->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lptvid->lpi, &ulAttrs);

				// 如果属于则填写路径信息
				if (ulAttrs & (SFGAO_FILESYSTEM))
				{
					if(::SHGetPathFromIDList(lptvid->lpifq, szBuff))
					{
						szFolderPath = szBuff;
						bRet = TRUE;					// 返回值是TRUE
					}
				}

				// 如果是通过键盘修改的选择状态则延迟处理
				if ((pnmtv->action == TVC_BYKEYBOARD)||(pnmtv->action == TVC_UNKNOWN))
				{
					LPTREEMSGPACK	ptmp = (LPTREEMSGPACK) new TREEMSGPACK;

					if (ptmp)
					{
						ptmp->ppsf   = lpsf2;
						ptmp->pidlfq = lptvid->lpifq;
						memcpy((void*)&ptmp->item, (const void *)&(pnmtv->itemNew), sizeof(TVITEM));

						// 发送延迟处理消息
						::PostThreadMessage(g_pwt->m_nThreadID, WM_TVN_DEFER, 0, (LPARAM)ptmp);

						// lpsf2 和 ptmp 将由延迟线程或延迟填充函数负责释放
						lpsf2 = NULL;
						ptmp  = NULL;
					}
				}
				else
				{
					// 如果延迟线程中有未处理的命令，则废弃它们
					::PostThreadMessage(g_pwt->m_nThreadID, WM_TVN_QUEUECLR, 0, 0);

					obj  = ((pnmtv->itemNew.cChildren == 1)&&(!(pnmtv->itemNew.state & TVIS_EXPANDEDONCE))) ? ISHTYPE_TREE : 0;
					obj |= ISHTYPE_LIST;

					if (bRet)
						ISeeTsVCDebugStrRecorder().Record("ShellTree 正在填充路径 <%s> 下的子目录数据。", 1, szBuff);
					else
						ISeeTsVCDebugStrRecorder().Record("ShellTree 正在填充一个非文件系统路径。", 1, 0);

					// 填充信息
					if (FillShellView(lpsf2, lptvid->lpifq, pnmtv->itemNew.hItem, obj) == 0)
					{
						if (obj&ISHTYPE_TREE)
						{
							tvscb.hParent     = pnmtv->itemNew.hItem;
							tvscb.lParam      = 0;
							tvscb.lpfnCompare = TreeViewCompareProc;
							SortChildrenCB(&tvscb);
							
							// 设置‘已被扩展过’标志，在真正扩展时将加快速度
							pnmtv->itemNew.state     |= TVIS_EXPANDEDONCE;
							pnmtv->itemNew.stateMask |= TVIS_EXPANDEDONCE;
							pnmtv->itemNew.mask      |= TVIF_STATE;
							SetItem(&pnmtv->itemNew);
						}
					}

					PostMessage(WM_TVN_SELCHANGED, (WPARAM)pnmtv->itemNew.hItem, 0);
				}
			}
			else
			{
				::PostThreadMessage(g_pwt->m_nThreadID, WM_TVN_QUEUECLR, 0, 0);

				PostMessage(WM_TVN_SELCHANGED, (WPARAM)pnmtv->itemNew.hItem, 0);
			}
		}
		else
		{
			::PostThreadMessage(g_pwt->m_nThreadID, WM_TVN_QUEUECLR, 0, 0);

			PostMessage(WM_TVN_SELCHANGED, (WPARAM)pnmtv->itemNew.hItem, 0);
		}

		if(lpsf2)
		{
			lpsf2->Release();
		}
	}	

	*pResult = 0;

	return bRet;
}


/****************************************************************************
*
*	函数:	OnDeleteShellItem(NMHDR* pNMHDR, LRESULT* pResult)
*
*	用途:	在响应删除树项时调用本函数（TVN_DELETEITEM），它主要用于释放
*           项的私有数据结构
*
*	映射:	TVN_DELETEITEM
*
****************************************************************************/
void CShellTree::OnDeleteShellItem(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPTVITEMDATA	lptvid = NULL;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	if ((lptvid=(LPTVITEMDATA)pNMTreeView->itemOld.lParam) != NULL)
	{
		ASSERT(g_malloc);

		lptvid->lpsfParent->Release();

		g_malloc->Free(lptvid->lpi);
		g_malloc->Free(lptvid->lpifq);
		g_malloc->Free(lptvid);
	}
}


/****************************************************************************
*
*	函数:	OnDeferFillAtt()
*
*	用途:	主类在相应延迟填充消息时调用（WM_TVN_DEFERFILL）。它是OnDeferFill()
*           函数的一个包装。
*
*	映射:	WM_TVN_DEFERFILL
*
****************************************************************************/
LRESULT CShellTree::OnDeferFillAtt(WPARAM wParam, LPARAM lParam)
{
	return OnDeferFill(wParam, lParam);
}


/****************************************************************************
*
*	函数:	OnDeferFill()
*
*	用途:	类在响应延迟填充消息时调用。WM_TVN_DEFERFILL消息为本类的自定义消息，
*           它主要为完成类的延迟填充特性。
*
*	映射:	WM_TVN_DEFERFILL
*
****************************************************************************/
LRESULT CShellTree::OnDeferFill(WPARAM wParam, LPARAM lParam)
{
	LPTREEMSGPACK	ptmp = (LPTREEMSGPACK)lParam;
	int				obj;
	TV_SORTCB		tvscb;
	
	ASSERT(ptmp);

	if (ptmp->ppsf)
	{
		// 判断待填充项是否已被填充过了，如果是则不再进行子目录的填充
		obj  = ((!(GetItemState(ptmp->item.hItem, TVIS_EXPANDEDONCE) & TVIS_EXPANDEDONCE))&&(ptmp->item.cChildren == 1)) ? ISHTYPE_TREE : 0;
		obj |= ISHTYPE_LIST;

		ISeeTsVCDebugStrRecorder().Record("ShellTree 正在延迟填充一个路径的子目录信息。", 1, 0);

		// 填充
		if (FillShellView(ptmp->ppsf, ptmp->pidlfq, ptmp->item.hItem, obj) == 0)
		{
			if (obj&ISHTYPE_TREE)
			{
				tvscb.hParent     = ptmp->item.hItem;
				tvscb.lParam      = 0;
				tvscb.lpfnCompare = TreeViewCompareProc;
				SortChildrenCB(&tvscb);
				
				// 设置已被填充标志
				ptmp->item.state     |= TVIS_EXPANDEDONCE;
				ptmp->item.stateMask |= TVIS_EXPANDEDONCE;
				ptmp->item.mask      |= TVIF_STATE;
				SetItem(&ptmp->item);
			}
		}
		// 发送选择修改通知
		PostMessage(WM_TVN_SELCHANGED, (WPARAM)ptmp->item.hItem, 1);
		
		// 释放IShellFolder接口
		ptmp->ppsf->Release();

		delete ptmp;
	}

	return 0;
}


/****************************************************************************
*
*	函数:	EnableImages()
*
*	用途:	将操作系统的图标列表粘贴到树中（注：因为是系统的图标列表，所以
*           绝不要去释放它）。
*
****************************************************************************/
void CShellTree::EnableImages()
{
    HIMAGELIST	hImageList;
    SHFILEINFO	sfi;

    hImageList = (HIMAGELIST)SHGetFileInfo((LPCSTR)"C:\\", 
                                           0,
                                           &sfi, 
                                           sizeof(SHFILEINFO), 
                                           SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

    // 粘贴到树中
    if (hImageList)
	{
		::SendMessage(m_hWnd, TVM_SETIMAGELIST, (WPARAM) TVSIL_NORMAL, (LPARAM)hImageList);
	}
}


/****************************************************************************
*
*	函数:	GetSelectedFolderPath(CString &szFolderPath)
*
*	用途:	获取当前选择项的路径串。如果当前选择项不在文件系统中（如‘我的电脑’）
*           则本函数返回FALSE。
*
****************************************************************************/
BOOL CShellTree::GetSelectedFolderPath(CString &szFolderPath)
{
	LPTVITEMDATA	lptvid;
	HRESULT			hr;
	BOOL			bRet  = FALSE;
	LPSHELLFOLDER	lpsf2 = NULL;
	HTREEITEM		hItem = NULL;

	static char		szBuff[MAX_PATH];
	
	// 获取当前选择项
	if((hItem = GetSelectedItem()))
	{
		// 得到该项的私有数据结构
		lptvid=(LPTVITEMDATA)GetItemData(hItem);

		if (lptvid && lptvid->lpsfParent && lptvid->lpi)
		{
			hr = lptvid->lpsfParent->BindToObject(lptvid->lpi, 0, IID_IShellFolder, (LPVOID *)&lpsf2);

			if (SUCCEEDED(hr))
			{
				ULONG ulAttrs = SFGAO_FILESYSTEM;

				// 判断该文件夹是否处于文件系统
				lptvid->lpsfParent->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lptvid->lpi, &ulAttrs);

				if (ulAttrs & (SFGAO_FILESYSTEM))
				{
					// 将绝对PIDL转换为文件系统的路径串，并保存在szBuff缓冲区中
					if(SHGetPathFromIDList(lptvid->lpifq, szBuff))
					{						
						szFolderPath = szBuff;
						bRet = TRUE;					// 获取到路径串返回TRUE
					}
				}

				lpsf2->Release();
			}
		}
	}

	return bRet;
}


/****************************************************************************
*
*	函数:	GetParentShellFolder(HTREEITEM folderNode)
*
*	用途:	获取指定文件夹的父文件夹接口（IShellFolder）
*
****************************************************************************/
LPSHELLFOLDER CShellTree::GetParentShellFolder(HTREEITEM folderNode)
{
	LPTVITEMDATA	lptvid;

	// 获得指定项（文件夹）的私有数据结构
	lptvid = (LPTVITEMDATA)GetItemData(folderNode);

	if(lptvid)
		return lptvid->lpsfParent;						// 返回父文件夹接口
	else
		return NULL;									// 如果没有私有数据，则返回NULL
}


/****************************************************************************
*
*	函数:	GetRelativeIDLIST(HTREEITEM folderNode)
*
*	用途:	获取指定文件夹的‘相对’PIDL（相对于它的父文件夹）
*
****************************************************************************/
LPITEMIDLIST CShellTree::GetRelativeIDLIST(HTREEITEM folderNode)
{
	LPTVITEMDATA	lptvid;

	// 获得指定项（文件夹）的私有数据结构
	lptvid = (LPTVITEMDATA)GetItemData(folderNode);

	if(lptvid)
		return lptvid->lpifq;							// 返回指定文件夹的相对PIDL
	else
		return NULL;									// 无私有数据结构则返回NULL
}


/****************************************************************************
*
*	函数:	GetFullyQualifiedIDLIST(HTREEITEM folderNode)
*
*	用途:	获取指定文件夹的‘绝对’PIDL（从根开始）
*
****************************************************************************/
LPITEMIDLIST CShellTree::GetFullyQualifiedIDLIST(HTREEITEM folderNode)
{
	LPTVITEMDATA	lptvid;

	// 获得指定项（文件夹）的私有数据结构
	lptvid = (LPTVITEMDATA)GetItemData(folderNode);

	if(lptvid)
		return lptvid->lpifq;							// 返回指定文件夹的绝对PIDL
	else
		return NULL;
}


/****************************************************************************
*
*	函数:	SearchTree(	HTREEITEM treeNode, CString szSearchName, FindAttribs attr)
*
*	用途:	搜索文件夹下是否有指定的子文件夹（只搜索一级），并选择它
*
*	注释:	该函数只能工作于文件系统，并且函数代码实现比较粗糙，有待改进
*
****************************************************************************/
bool CShellTree::SearchTree(HTREEITEM treeNode, CString szSearchName, FindAttribs attr)
{
	LPTVITEMDATA	lptvid;
	HRESULT			hr;
	bool			bRet  = FALSE;
	LPSHELLFOLDER	lpsf2 = NULL;

	char	drive[_MAX_DRIVE];
	char	dir[_MAX_DIR];
	char	fname[_MAX_FNAME];
	char	ext[_MAX_EXT];

	CString	szCompare;

	// 将搜索名转换为大写
	szSearchName.MakeUpper();

	while(treeNode && bRet==FALSE)
	{
		// 获取项的私有数据结构
		lptvid = (LPTVITEMDATA)GetItemData(treeNode);

		if (lptvid && lptvid->lpsfParent && lptvid->lpi)
		{
			// 获取IShellFolder接口
			hr = lptvid->lpsfParent->BindToObject(lptvid->lpi, 0, IID_IShellFolder, (LPVOID *)&lpsf2);

			if (SUCCEEDED(hr))
			{
				ULONG ulAttrs = SFGAO_FILESYSTEM;

				lptvid->lpsfParent->GetAttributesOf(1, (const struct _ITEMIDLIST **)&lptvid->lpi, &ulAttrs);

				// 只有当项处于文件系统时才进行进一步的比较操作
				if (ulAttrs & (SFGAO_FILESYSTEM))
				{
					// 获取项的全路径
					if(SHGetPathFromIDList(lptvid->lpifq, szCompare.GetBuffer(MAX_PATH)))
					{
						switch(attr)
						{
						case type_drive:
							// 解析路径串，提取其中的驱动器符
							_splitpath(szCompare, drive, dir, fname, ext);
							szCompare = drive;
							break;
						case type_folder:
							// 只提取项的显示名
							szCompare = GetItemText(treeNode);
							break;
						case	type_path:
							// 保留
							break;
						default:
							ASSERT(0);
							break;
						}

						// 转换为大写，并于调用者提供的串进行比较
						szCompare.MakeUpper();

						if(szCompare == szSearchName)
						{
							// 如果相同，确保该项可见，并选择它
							EnsureVisible(treeNode);
							SelectItem(treeNode);
							bRet = TRUE;
						}
					}
				}
				lpsf2->Release();
			}
		}
		// 取得下一个兄弟项句柄
		treeNode = GetNextSiblingItem(treeNode);
	}

	return bRet;
}


/****************************************************************************
*
*	函数:	TunnelTree(CString szFindPath)
*
*	用途:	使指定的文件夹项处于选择状态并可见，必要时会进行展开动作
*
*	注释:	该函数比较粗糙，只能工作于文件系统的文件夹，并且树的根项必须
*           是桌面。
*
****************************************************************************/
void CShellTree::TunnelTree(CString szFindPath)
{
	HTREEITEM		subNode = GetRootItem();
	CString			szPathHop;
	LPSHELLFOLDER	lpsf   = NULL;
	LPITEMIDLIST	lpi    = NULL;
	
	char			drive[_MAX_DRIVE];
	char			dir[_MAX_DIR];
	char			fname[_MAX_FNAME];
	char			ext[_MAX_EXT];
	char			delimiter[]="\\";

	char			szBuff[MAX_PATH];

	if (szFindPath.IsEmpty())
	{
		return;
	}

	if(szFindPath.ReverseFind('\\') != szFindPath.GetLength()-1)
	{
		szFindPath += "\\";
	}

	// 解析路径串
	_splitpath(szFindPath, drive, dir, fname, ext);

	// 先搜索驱动器
	szPathHop = drive;

	// 获取桌面文件夹接口
	if (FAILED(SHGetDesktopFolder(&lpsf)))
	{
		return;
	}
	
	// 获取‘我的电脑’元素的标识符列表（PIDL）
	if (FAILED(SHGetSpecialFolderLocation(0, CSIDL_DRIVES, &lpi)))
	{
		lpsf->Release();
		return;
	}

	// 获取元素显示名
	if (GetName(lpsf, lpi, SHGDN_NORMAL, szBuff) == 0)
	{
		g_malloc->Free(lpi);
		lpsf->Release();
		return;
	}
	
	// 释放资源
	g_malloc->Free(lpi);
	lpsf->Release();

	// 获取‘我的电脑’文件夹的树项句柄
	if ((subNode=SearchByText(subNode, CString(szBuff))) == NULL)
	{
		return;
	}

	// 文件夹填充
	Expand(subNode, TVE_EXPAND);

	// 获取第一个驱动器树项句柄
	if((subNode=GetChildItem(subNode)) != NULL)
	{
		// 从驱动器树项开始逐级选择、搜索
		if(SearchTree(subNode, szPathHop, CShellTree::type_drive))
		{
			char *p = strtok(dir, delimiter);

			while(p)
			{
				// 获取选择项
				subNode = GetSelectedItem();

				// 填充
				Expand(subNode, TVE_EXPAND);

				// 获取第一个子文件夹项
				subNode = GetChildItem(subNode);

				if(SearchTree(subNode, p, CShellTree::type_folder))
				{
					// 找到一级后，将p定位到下一级文件夹
					p = strtok(NULL, delimiter);
				}
				else
				{
					p = NULL;
				}
			}
		}
	}
}


/****************************************************************************
*
*	函数:	SearchByText(HTREEITEM hti, CString szFindText)
*
*	用途:	搜索下一级文件夹中是否有指定显示名的子文件夹，如果有则返回它的项句柄
*
****************************************************************************/
HTREEITEM CShellTree::SearchByText(HTREEITEM hti, CString szFindText)
{
	HTREEITEM	hRet = NULL;
	HTREEITEM	hChildItem;
	
	// 判断是否有子项
	if (ItemHasChildren(hti))
	{
		hChildItem = GetChildItem(hti);
		
		while (hChildItem != NULL)
		{
			// 比较项文本（显示名）
			if (GetItemText(hChildItem) == szFindText)
			{
				hRet = hChildItem;
				break;
			}

			hChildItem = GetNextItem(hChildItem, TVGN_NEXT);
		}
	}

	return hRet;
}


/****************************************************************************
*
*	函数:	GetPathFromHTREEITEM(HTREEITEM folderNode, LPTSTR szBuff)
*
*	用途:	根据指定树项，返回它的全路径。szBuff的尺寸被假设为MAX_PATH
*
****************************************************************************/
int CShellTree::GetPathFromHTREEITEM(HTREEITEM folderNode, LPTSTR szBuff)
{
	// 获取树项私有数据结构中的绝对PIDL（无需释放）
	LPITEMIDLIST	pidlfq = GetFullyQualifiedIDLIST(folderNode);

	if (pidlfq == NULL)
	{
		return -1;										// 无效的树项句柄（或根）
	}

	if (SHGetPathFromIDList(pidlfq, szBuff) == FALSE)
	{
		return -2;										// 转换失败
	}

	return 0;
}


/****************************************************************************
*
*	函数:	OnUpdateItem(WPARAM wParam, LPARAM lParam)
*
*	用途:	当引擎被刷新之后调用，以更新列表视内容
*
****************************************************************************/
int CShellTree::OnUpdateItem(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM		hItem = GetSelectedItem();
	LPTVITEMDATA	pda;

	if (hItem)
	{
		pda = (LPTVITEMDATA)GetItemData(hItem);			// 取得树项附加数据

		if (pda)
		{
			LPSHELLFOLDER  lpsf2 = NULL;
			
			// 获取选择项Shell接口
			if (SUCCEEDED(pda->lpsfParent->BindToObject(pda->lpi, 0, IID_IShellFolder, (LPVOID *)&lpsf2)))
			{
				// 填充文件夹信息
				FillShellView(lpsf2, pda->lpifq, 0, ISHTYPE_LIST);
				lpsf2->Release();
			}
		}
	}

	return 0;
}
