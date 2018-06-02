/********************************************************************

	ShellList.cpp

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
					控件类实现文件。

	本文件编写人：	
					YZ				yzfree##sina.com

	本文件版本：	30315
	最后修改于：	2003-3-15

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
		地址收集软件。

	修正历史：

		2003-3		增加项私有数据结构的设置
		2003-2		第一个测试版发布

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


/////////////////////////////////////////////////////////////////////////////
// CShellList

CShellTree	*CShellList::m_pTree   = 0;
CShellPath	*CShellList::m_pPath   = 0;

CWinThread	*CShellList::m_pThread = 0;

#pragma pack(push)
#pragma pack(4)
int			CShellList::m_iCount   = 0;
int			CShellList::m_iSN      = 0;
int			CShellList::m_iCurrPt  = 0;
int			CShellList::m_iNum     = 0;
#pragma pack(pop)

CString		CShellList::m_sFilter  = "*.*";
CString		CShellList::m_sCurrPath;

int			CShellList::m_iAutoPlayTime = 4;
int			CShellList::m_iAutoPlay     = 1;



CShellList::SIBCOL	CShellList::sc[COLNUM] = 
{
	{COL_NAME, "名称", LVCFMT_LEFT, 160},
	{COL_SIZE, "大小", LVCFMT_RIGHT, 50},
	{COL_TYPE, "类型", LVCFMT_LEFT, 100},
	{COL_NOTE, "备注", LVCFMT_LEFT, 120}
};


CShellList::CShellList()
{
}

CShellList::~CShellList()
{
}


BEGIN_MESSAGE_MAP(CShellList, CListCtrl)
	//{{AFX_MSG_MAP(CShellList)
	ON_NOTIFY_REFLECT(LVN_DELETEITEM, OnDeleteitem)
	ON_NOTIFY_REFLECT(LVN_DELETEALLITEMS, OnDeleteallitems)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CShellList message handlers

// 列表控件填充线程
UINT CShellList::ShellLProc(LPVOID parm)
{
	CShellList		*pList = (CShellList*)parm;
	MSG				msg;
	MSG				msgtmp;
	BOOL			bRet;
	int				sn, count, i, apc;
	
	// 通知主线程，并创建本线程的消息队列
	pList->PostMessage(WM_LVN_PROCCREATE);
	
	// 消息循环
	while((bRet=::GetMessage(&msg, NULL, 0, 0)) != 0)
    { 
        if (bRet == -1)
        {
            continue;
        }
        else
        {
			if (msg.message == WM_LVN_QUIT)				// 退出消息
			{
				break;
			}
			else if (msg.message == WM_LVN_AUTOSEL)		// 自动播放消息
			{
				apc = msg.wParam;

				while (m_iAutoPlay)
				{
					// 延迟一定的时间
					for (i=m_iAutoPlayTime*1000; i>0; i-=APTIMEUNIT)
					{
						// 如果有新的命令到来，则废弃当前的播放过程
						if (::PeekMessage(&msgtmp, NULL, WM_LVN_NEWCOMM, WM_LVN_QUIT, PM_NOREMOVE))
						{
							break;
						}
					
						// 延迟一个最小时间单位
						::Sleep(APTIMEUNIT);
					}

					if ((i <= 0)&&(m_iAutoPlay))
					{
						// 时间到，向主线程发送选择下一幅图象的消息
						pList->PostMessage(WM_LVN_AUTONEXT, (WPARAM)apc, (LPARAM)0);
					}
					else
					{
						// 有新的“指示”，处理下一个消息:)
						break;
					}
				}
			}
			else if (msg.message == WM_LVN_NEWCOMM)		// 新命令消息
			{
				// 提取命令参数
				sn    = msg.wParam;
				count = msg.lParam;
				
				if (count == 0)
				{
					continue;
				}
				
				// 去除可能残留在消息队列中的 WM_LVN_AUTOSEL 消息
				while (::PeekMessage(&msgtmp, NULL, WM_LVN_AUTOSEL, WM_LVN_AUTOSEL, PM_REMOVE)){;}

				// 发送子项填充消息
				for (i=0; i<count; i++)
				{
					// 如果有新的命令到来，则废弃当前的命令
					if (::PeekMessage(&msgtmp, NULL, WM_LVN_NEWCOMM, WM_LVN_QUIT, PM_NOREMOVE))
					{
						break;
					}
					
					if (m_iCount < MAX_SUBITEMCNT)
					{
						// 向主线程发送填充消息
						pList->PostMessage(WM_LVN_SUBITEMDATA, (WPARAM)sn, (LPARAM)i);
						::InterlockedIncrement((LPLONG)&m_iCount);
						::Sleep(0);						// 为主线程留出处理其它消息的时间
					}
					else
					{
						i--;
						::Sleep(0);						// 有太多发出的命令没有被处理，暂停发送
					}
				}
				
				if (i < count)
				{
					continue;							// 直接返回，去获取新的命令
				}

				// 如果设置了自动播放，则触发播放过程
				if (m_iAutoPlay)
				{
					::PostThreadMessage(pList->m_pThread->m_nThreadID, WM_LVN_AUTOSEL, count, 0);
				}
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


// 列表控件排序回调函数
int CALLBACK CShellList::CFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CListCtrl* pListCtrl = (CListCtrl*) lParamSort;
	CString    strItem1  = pListCtrl->GetItemText(lParam1, 0);
	CString    strItem2  = pListCtrl->GetItemText(lParam2, 0);
	
	return strcmp(strItem2, strItem1);
}


// 初始化列表控件
int CShellList::InitShellList(CShellTree *pst, CShellPath *psp, CString filter)
{
	ISeeTsVCDebugStrRecorder().Record("开始初始化 ShellList ...", 0, 0);

	m_pTree = pst;
	m_pPath = psp;
	m_sCurrPath = "C:\\";
	m_sFilter   = filter;

	// 创建填充线程
	if (!(m_pThread=::AfxBeginThread(ShellLProc, (LPVOID)this)))
	{
		return -1;
	}

	for (int i=0; i<COLNUM; i++)
	{
		InsertColumn(sc[i].index, (LPCTSTR)sc[i].name, sc[i].ali, sc[i].width, -1);
	}
	
	// 启用系统图象列表
	EnableImages();

	// 设定自动播放标志
	m_iAutoPlay = AfxGetApp()->GetProfileInt(ITSVCMAIN_RKEYSEC, ITSVCLIST_RKEY_AUTOPLAY, 0);

	ISeeTsVCDebugStrRecorder().Record("ShellList 初始化完毕。", 0, 0);
	
	return 0;
}


// 关闭列表控件
void CShellList::CloseShellList()
{
	// 退出前销毁所有的列表项
	DeleteAllItems();

	// 设定自动播放标志
	AfxGetApp()->WriteProfileInt(ITSVCMAIN_RKEYSEC, ITSVCLIST_RKEY_AUTOPLAY, m_iAutoPlay);

	// 销毁填充线程
	::PostThreadMessage(m_pThread->m_nThreadID, WM_LVN_QUIT, 0, 0);
	
	::WaitForSingleObject(m_pThread->m_hThread, 1000);

	m_sFilter.Empty();
	m_sCurrPath.Empty();

	m_pThread = 0;

	m_pTree = 0;
	m_pPath = 0;

	ISeeTsVCDebugStrRecorder().Record("ShellList 关闭。", 0, 0);
}


// 项删除响应
void CShellList::OnDeleteitem(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW		*pNMListView = (NM_LISTVIEW*)pNMHDR;
	LPLVITEMDATA	plvid = (LPLVITEMDATA)GetItemData(pNMListView->iItem);

	if (plvid)
	{
		// 释放文件的相对PIDL
		if (plvid->lpi)
		{
			m_pTree->g_malloc->Free(plvid->lpi);
		}

		// 释放父文件夹接口
		if (plvid->lpsfParent)
		{
			plvid->lpsfParent->Release();
		}

		delete plvid;
	}

	*pResult = 0;
}


// 所有项删除响应
void CShellList::OnDeleteallitems(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	*pResult = FALSE;
}


// 为列表控件启用图标列表
void CShellList::EnableImages()
{
	SetImageList(m_pPath->m_cImageList.GetLIconList(), LVSIL_NORMAL);
	SetImageList(m_pPath->m_cImageList.GetSIconList(), LVSIL_SMALL);
}


// 开始一个新的填充过程（由视控件引发）
LRESULT CShellList::OnFillBegin(WPARAM wParam, LPARAM lParam)
{
	// 删除旧项
	DeleteAllItems();

	// 改变当前路径
	m_sCurrPath = (LPCTSTR)(const char *)wParam;

	m_iNum = 0;

	return 0;
}


// 结束一个填充过程
LRESULT CShellList::OnFillEnd(WPARAM wParam, LPARAM lParam)
{
	// 排序项
	SortItems(CFunc, (DWORD)(LPVOID)this);

	Arrange(LVA_DEFAULT);

	// 批次号更改
	m_iSN++;
	m_iNum = wParam;
	
	// 填充点初始化
	m_iCurrPt = 0;
	
	// 修正当前路径及文件
	m_pPath->ObjectChanged((LPCTSTR)m_sCurrPath, (LPCTSTR)0, m_iSN);

	// 通知填充线程开始填充子项信息
	::PostThreadMessage(m_pThread->m_nThreadID, WM_LVN_NEWCOMM, (WPARAM)m_iSN, (LPARAM)wParam);

	ISeeTsVCDebugStrRecorder().Record("ShellList 的当前路径被改变为 <%s>。", 1, (LPCSTR)m_sCurrPath);
	ISeeTsVCDebugStrRecorder().Record("ShellList 当前路径下的文件项个数为 <%d>。", 1, m_iNum);
	
	return 0;
}


// 自动选择下一幅图象（自动播放时调用）
LRESULT CShellList::OnAutoNext(WPARAM wParam, LPARAM lParam)
{
	POSITION	pos  = GetFirstSelectedItemPosition();
	int			nCnt = GetItemCount();
	int			nItem;
	UINT		nState;

	// 获取当前选择项的索引
	if (pos == NULL)
	{
		// 搜索未被选中但拥有焦点的项
		if ((nItem=GetNextItem(-1, LVNI_ALL|LVNI_FOCUSED)) >= 0)
		{
			nState = LVIS_FOCUSED;
			SetItemState(nItem, 0, LVIS_FOCUSED);
		}
		else
		{
			nItem  = -1;
			nState = 0;
		}
	}
	else
	{
		// 获取选择项索引
		nItem = GetNextSelectedItem(pos);

		// 判断原项是否拥有焦点
		nState = GetItemState(nItem, LVIS_FOCUSED);

		// 去除当前项的选择状态及焦点
		SetItemState(nItem, 0, LVIS_SELECTED);

		if (nState == LVIS_FOCUSED)
		{
			SetItemState(nItem, 0, LVIS_FOCUSED);
		}
	}

	// 回绕功能
	if (++nItem >= nCnt)
	{
		nItem = 0;
	}

	// 设置新项的焦点及选择状态
	if (nState == LVIS_FOCUSED)
	{
		SetItemState(nItem, LVIS_FOCUSED, LVIS_FOCUSED);
	}

	SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);

	return 0;
}


// 获取自动播放标志
int CShellList::GetAutoPlay()
{
	return m_iAutoPlay;
}


// 设置自动播放
void CShellList::SetAutoPlay(int type)
{
	// type含义：非0 开启自动播放，0 关闭自动播放
	if (type)
	{
		// 如果当前状态为关闭
		if (m_iAutoPlay == 0)
		{
			m_iAutoPlay = 1;

			// 如果当前目录已被填充完毕，则手动发送自动播放消息
			if (m_iNum)
			{
				::PostThreadMessage(m_pThread->m_nThreadID, WM_LVN_AUTOSEL, m_iNum, 0);
			}
		}
	}
	else
	{
		m_iAutoPlay = 0;
	}
}


// 填充列表项的详细数据（由填充线程引发）
LRESULT CShellList::OnSubItemDataFill(WPARAM wParam, LPARAM lParam)
{
	static	WIN32_FILE_ATTRIBUTE_DATA	wfa;
	static	CString	spath;
	static	CString	cs;
	static	char	buff[MAX_PATH];
	
	// 如果是过期命令，则直接返回（根据批次号判别）
	if (wParam != (WPARAM)m_iSN)
	{
		::InterlockedDecrement((LPLONG)&m_iCount);
		return 0;
	}

	CRect			rc;
	UINT			flags = 0, style = 0;
	int				index, numperpage, itemcount, i, lastidx;
	DWORD			dwl;
	LPSPEC_DATAPACK	psdp;
	LPLVITEMDATA	pdata;
	
	itemcount  = GetItemCount();						// 获取列表视中所有项的个数
	numperpage = GetCountPerPage();						// 获取视的每一页中可显示项的个数
	style      = GetStyle();							// 获取视的风格

	// 搜索列表视、报表视风格下当前可视范围里是否存在未填充子项数据的项
	if (((style&LVS_TYPEMASK) == LVS_REPORT)||((style&LVS_TYPEMASK) == LVS_LIST))
	{
		index      = GetTopIndex();						// 获取第一个可视项的索引
		lastidx    = min((itemcount-index), numperpage);

		ASSERT(index != -1);

		for (i=0; i<lastidx; i++)						// 查找可视范围内的未填充项
		{
			pdata = (LPLVITEMDATA)GetItemData(index+i);
			ASSERT(pdata);

			if (pdata->state == SPTS_EMPTY)				
			{
				index = index+i;
				break;
			}
		}

		index = (i == numperpage) ? -1 : index;			// 如果没有找到，返回-1
	}
	else
	{
		index = -1;
	}

	CMainFrame	*cmf = (CMainFrame*)AfxGetMainWnd();
	CString		prs;
	BOOL		ismark = FALSE;

	if (index != -1)
	{
		// 获取文件名
		GetItemText(index, 0, (LPTSTR)buff, MAX_PATH);
		
		// 合成全路径串
		spath = m_sCurrPath;
		spath += (LPCTSTR)buff;

		// 设置文件对应的图标
		i = m_pPath->m_cImageList.GetIconIndex(AfxGetMainWnd()->GetSafeHwnd(), pdata->lpsfParent, pdata->lpi, (LPCSTR)spath);
		SetItem(index, 0, LVIF_IMAGE, (LPCTSTR)0, i, 0, 0, 0);

		// 获取文件属性信息
		if (GetFileAttributesEx((LPCSTR)spath, GetFileExInfoStandard, (LPVOID)&(pdata->wfad)))
		{
			pdata->state |= SPTS_FATT;					// 已拥有文件属性信息

			dwl = (pdata->wfad.nFileSizeLow+1023)/1024;
			if (dwl > 1024)
				cs.Format("%.1fMB", (float)dwl/1024);
			else
				cs.Format("%dKB", dwl);
			
			SetItem(index, 1, LVIF_TEXT, (LPCTSTR)cs, 0, 0, 0, 0);
		}
		
		// 读取图象概略信息
		if ((psdp=isirw_s_get_img_info(ISEEIO_CONFER_LOC_FILE, (LPCTSTR)spath, 0, 0, m_iSN)) != 0)
		{
			if (isirw_s_wait(psdp, 0) == ER_SUCCESS)
			{
				// 提取图象信息到项的私有数据结构
				pdata->width    = (DWORD)psdp->org_img->width;
				pdata->height   = (DWORD)psdp->org_img->height;
				pdata->bitcount = (DWORD)psdp->org_img->bitcount;
				
				pdata->state   |= SPTS_IMGINF;

				// 显示图象的宽、高等数据
				cs.Format("%d×%d×%d", psdp->org_img->width, psdp->org_img->height, psdp->org_img->bitcount);
				SetItem(index, 2, LVIF_TEXT, (LPCTSTR)cs, 0, 0, 0, 0);

				ismark = TRUE;
			}
			isirw_free_SPEC_DATAPACK(psdp);
		}

		if (ismark == FALSE)
		{
			cs.Format("受损的图象文件！");
			SetItem(index, 3, LVIF_TEXT, (LPCTSTR)cs, 0, 0, 0, 0);
		}

		::InterlockedDecrement((LPLONG)&m_iCount);

		if (lParam == (m_iNum-1))
		{
			((CRightView*)this)->m_bFillMark = 2;		// 填充完毕

			prs.Format("共%d张图片 - 100%%", m_iNum);
		}
		else
		{
			prs.Format("共%d张图片 - %d%%", m_iNum, (lParam*100)/m_iNum);
		}

		// 更新状态条中的相关信息
		cmf->m_wndStatusBar.SetPaneText(cmf->m_wndStatusBar.CommandToIndex(IDS_PROGRESS), (LPCTSTR)prs);
		
		return 1;
	}
	else if (m_iCurrPt < itemcount)
	{
		index = m_iCurrPt;

		while (TRUE)
		{
			m_iCurrPt++;

			pdata = (LPLVITEMDATA)GetItemData(index);

			if (pdata->state == SPTS_EMPTY)
			{
				GetItemText(index, 0, (LPTSTR)buff, MAX_PATH);
				
				spath = m_sCurrPath;
				spath += (LPCTSTR)buff;
				
				// 设置文件对应的图标
				i = m_pPath->m_cImageList.GetIconIndex(AfxGetMainWnd()->GetSafeHwnd(), pdata->lpsfParent, pdata->lpi, (LPCSTR)spath);
				SetItem(index, 0, LVIF_IMAGE, (LPCTSTR)0, i, 0, 0, 0);
				
				// 获取文件属性信息
				if (GetFileAttributesEx((LPCSTR)spath, GetFileExInfoStandard, (LPVOID)&(pdata->wfad)))
				{
					pdata->state |= SPTS_FATT;					// 已拥有文件属性信息
					
					dwl = (pdata->wfad.nFileSizeLow+1023)/1024;
					if (dwl > 1024)
						cs.Format("%.1fMB", (float)dwl/1024);
					else
						cs.Format("%dKB", dwl);
					
					SetItem(index, 1, LVIF_TEXT, (LPCTSTR)cs, 0, 0, 0, 0);
				}
		
				// 读取图象概略信息
				if ((psdp=isirw_s_get_img_info(ISEEIO_CONFER_LOC_FILE, (LPCTSTR)spath, 0, 0, m_iSN)) != 0)
				{
					if (isirw_s_wait(psdp, 0) == ER_SUCCESS)
					{
						// 提取图象信息到项的私有数据结构
						pdata->width    = (DWORD)psdp->org_img->width;
						pdata->height   = (DWORD)psdp->org_img->height;
						pdata->bitcount = (DWORD)psdp->org_img->bitcount;
						
						pdata->state   |= SPTS_IMGINF;
						
						// 显示图象的宽、高等数据
						cs.Format("%d×%d×%d", psdp->org_img->width, psdp->org_img->height, psdp->org_img->bitcount);
						SetItem(index, 2, LVIF_TEXT, (LPCTSTR)cs, 0, 0, 0, 0);

						ismark = TRUE;
					}
					isirw_free_SPEC_DATAPACK(psdp);
				}

				if (ismark == FALSE)
				{
					cs.Format("受损的图象文件！");
					SetItem(index, 3, LVIF_TEXT, (LPCTSTR)cs, 0, 0, 0, 0);
				}

				::InterlockedDecrement((LPLONG)&m_iCount);
				
				if (lParam == (m_iNum-1))
				{
					((CRightView*)this)->m_bFillMark = 2;		// 填充完毕

					prs.Format("共%d张图片 - 100%%", m_iNum);
				}
				else
				{
					prs.Format("共%d张图片 - %d%%", m_iNum, (lParam*100)/m_iNum);
				}

				cmf->m_wndStatusBar.SetPaneText(cmf->m_wndStatusBar.CommandToIndex(IDS_PROGRESS), (LPCTSTR)prs);

				return 1;
			}
			
			index = m_iCurrPt;

			if (index >= itemcount)
			{
				break;
			}
		}
	}
	
	ASSERT(m_iCount == 0);

	return 0;
}


// 获取文件过滤串
CString	&CShellList::GetFilterString()
{
	return m_sFilter;
}


// 设置文件过滤串
void CShellList::SetFilterString(CString &nsp)
{
	m_sFilter = nsp;
}


// 判断指定文件是否能被列表控件接受（根据文件过滤串）
BOOL CShellList::IsDestFile(LPCTSTR fname)
{
	if (m_sFilter.Find("*.*",0) != -1)
	{
		return TRUE;									// "*.*"表示任何文件均接受
	}

	CString	stmp = fname;
	int		dot  = 0;
	
	// 过滤串为小写，所以需要先转换
	stmp.MakeLower();

	// 判断是否存在扩展名
	if ((dot=stmp.ReverseFind('.')) == -1)
	{
		return FALSE;									// 无扩展名文件将被视为非目标对象
	}

	// 提取扩展名串（包括'.'符号）
	CString	stmp2 = stmp.Right(stmp.GetLength()-dot);

	// 粗略的虑掉一些不符合规定的文件类型
	if (stmp2.GetLength() == 1)
	{
		return FALSE;									// 无扩展名的文件
	}

	// 判断该类型是否符合过滤条件
	if ((dot=m_sFilter.Find((LPCTSTR)stmp2,0)) == -1)
	{
		return FALSE;									// 不支持的文件类型
	}

	LPCTSTR		ps = (LPCSTR)m_sFilter;

	// 更严格的判断
	if ((ps[dot+stmp2.GetLength()] != ',')&&(ps[dot+stmp2.GetLength()] != '\0'))
	{
		return FALSE;
	}

	// 符合返回TRUE
	return TRUE;
}

