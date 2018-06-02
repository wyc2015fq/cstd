// PluginSet.cpp : implementation file
//

#include "stdafx.h"
#include "iseetsvc.h"
#include "PluginSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPluginSet dialog


CPluginSet::CPluginSet(CWnd* pParent /*=NULL*/)
	: CDialog(CPluginSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPluginSet)
	m_sPluginPath = _T("");
	m_sDBG = _T("");
	m_sEngVer = _T("");
	m_sPliVer = _T("");
	//}}AFX_DATA_INIT

	pirwhead = 0;
	plicnt   = 0;
	selcnt   = 0;
	
	memset((void*)plda, 0, sizeof(LDATA)*MAX_PLUGIN_NUM);
}


void CPluginSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPluginSet)
	DDX_Control(pDX, IDC_BTN_UPDATE, m_btnUpdate);
	DDX_Control(pDX, IDC_BTN_SELALL, m_btnAll);
	DDX_Control(pDX, IDC_BTN_SEL, m_btnUse);
	DDX_Control(pDX, IDC_BTN_FREE, m_btnFree);
	DDX_Control(pDX, IDC_LIST_PLUGIN, m_cList);
	DDX_Text(pDX, IDC_PLUGIN_PATH, m_sPluginPath);
	DDV_MaxChars(pDX, m_sPluginPath, 260);
	DDX_Text(pDX, IDC_STA_DBG, m_sDBG);
	DDV_MaxChars(pDX, m_sDBG, 64);
	DDX_Text(pDX, IDC_STA_ENGVER, m_sEngVer);
	DDV_MaxChars(pDX, m_sEngVer, 32);
	DDX_Text(pDX, IDC_STA_PLIVER, m_sPliVer);
	DDV_MaxChars(pDX, m_sPliVer, 32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPluginSet, CDialog)
	//{{AFX_MSG_MAP(CPluginSet)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PLUGIN, OnItemchangedListPlugin)
	ON_BN_CLICKED(IDC_BTN_FREE, OnBtnFree)
	ON_BN_CLICKED(IDC_BTN_SEL, OnBtnSel)
	ON_BN_CLICKED(IDC_BTN_SELALL, OnBtnSelall)
	ON_BN_CLICKED(IDC_BTN_UPDATE, OnBtnUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPluginSet message handlers

BOOL CPluginSet::OnInitDialog() 
{
	int					ev, pv, dbg;

	isirw_get_eng_info(&ev, &pv, &dbg, (char*)m_sPluginPath.GetBuffer(MAX_PATH));

	m_sEngVer.Format("%d.%d版", ev/10, ev%10);
	m_sPliVer.Format("%d.%d版", pv/10, pv%10);

	if (dbg == 0)
	{
		m_sDBG = "调试";
	}
	else
	{
		m_sDBG = "发布";
	}
	
	CDialog::OnInitDialog();

	// 调用者必须已调用了SetData()函数
	ASSERT(pirwhead);

	m_cList.InsertColumn(0, (LPCTSTR)"名称", LVCFMT_CENTER, 100);
	m_cList.InsertColumn(1, (LPCTSTR)"版本", LVCFMT_CENTER, 36);
	m_cList.InsertColumn(2, (LPCTSTR)"状态", LVCFMT_CENTER, 72);
	m_cList.InsertColumn(3, (LPCTSTR)"优先级", LVCFMT_CENTER, 100);
	m_cList.InsertColumn(4, (LPCTSTR)"调试", LVCFMT_CENTER, 36);
	m_cList.InsertColumn(5, (LPCTSTR)"插件ID", LVCFMT_CENTER, 48);
	m_cList.InsertColumn(6, (LPCTSTR)"开发人", LVCFMT_CENTER, 300);

	memset((void*)plda, 0, sizeof(LDATA)*MAX_PLUGIN_NUM);

	plicnt = 0;
	selcnt = 0;
	
	// 初始化列表视
	_init_list();

	return TRUE;
}


// 调用者设定引擎内部信息表映射地址
void CPluginSet::SetData(LPIRWMF_HEADER pih)
{
	ASSERT(pih);
	ASSERT(isirw_is_ok() == 0);							// 引擎必须已成功建立
	ASSERT(isirw_is_busy() == 3);						// 引擎当前必须是已被锁定的
	pirwhead = pih;
}


void CPluginSet::OnItemchangedListPlugin(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if (pNMListView->uChanged == LVIF_STATE)
	{
		int			slda[MAX_PLUGIN_NUM];
		POSITION	pos = m_cList.GetFirstSelectedItemPosition();
		int			ibuse = 0, ibfree = 0, i;
		
		memset((void*)slda, 0, sizeof(int)*MAX_PLUGIN_NUM);
		
		selcnt = 0;

		// 获取选择项的个数
		while (pos)
		{
			slda[selcnt++] = (int)m_cList.GetItemData(m_cList.GetNextSelectedItem(pos));
		}

		// 搜索选择项中录用插件和未用插件的数目
		for (i=0; i<selcnt; i++)
		{
			if (plda[slda[i]].inst == PLINSTATUS_NOUSE)
			{
				ibfree++;
			}
			else if ((plda[slda[i]].inst == PLINSTATUS_USE)||(plda[slda[i]].inst == PLINSTATUS_ACT))
			{
				ibuse++;
			}
			else
			{
				ASSERT(0);
			}
		}

		// 只有存在已录用插件时，禁用按钮才有效
		if (ibuse)
		{
			m_btnFree.EnableWindow();
		}
		else
		{
			m_btnFree.EnableWindow(FALSE);
		}

		// 只有存在未录用插件时，录用按钮才有效
		if (ibfree)
		{
			m_btnUse.EnableWindow();
		}
		else
		{
			m_btnUse.EnableWindow(FALSE);
		}
	}

	*pResult = 0;
}


// 禁用选择的插件
void CPluginSet::OnBtnFree() 
{
	if (selcnt == 0)
	{
		return;
	}

	int	item = -1, index;

	// 搜索所有选择项
	while ((item=m_cList.GetNextItem(item,LVNI_ALL|LVNI_SELECTED)) != -1)
	{
		index = m_cList.GetItemData(item);

		// 将已录用插件禁用
		if ((plda[index].inst == PLINSTATUS_USE)||(plda[index].inst == PLINSTATUS_ACT))
		{
			plda[index].inst = PLINSTATUS_NOUSE;
			m_cList.SetItemText(item, 2, "未录用");
		}
	}

	// 修正按钮状态
	m_btnUse.EnableWindow();
	m_btnFree.EnableWindow(FALSE);

	m_cList.SetFocus();
}


// 录用选择的插件
void CPluginSet::OnBtnSel() 
{
	if (selcnt == 0)
	{
		return;
	}
	
	int	item = -1, index;
	
	// 搜索所有选择项
	while ((item=m_cList.GetNextItem(item,LVNI_ALL|LVNI_SELECTED)) != -1)
	{
		index = m_cList.GetItemData(item);
		
		// 启用未录用的插件
		if (plda[index].inst == PLINSTATUS_NOUSE)
		{
			if (plda[index].iost == PLINSTATUS_ACT)
			{
				plda[index].inst = PLINSTATUS_ACT;
				m_cList.SetItemText(item, 2, "调入");
			}
			else
			{
				plda[index].inst = PLINSTATUS_USE;
				m_cList.SetItemText(item, 2, "录用未调入");
			}
		}
	}
	
	// 修正按钮状态
	m_btnUse.EnableWindow(FALSE);
	m_btnFree.EnableWindow();

	m_cList.SetFocus();
}


// 启用所有插件
void CPluginSet::OnBtnSelall() 
{
	int	item, index;

	// 启用所有插件
	for (item=0; item<m_cList.GetItemCount(); item++)
	{
		index = m_cList.GetItemData(item);
		
		// 只处理未录用的插件
		if (plda[index].inst == PLINSTATUS_NOUSE)
		{
			if (plda[index].iost == PLINSTATUS_ACT)
			{
				plda[index].inst = PLINSTATUS_ACT;
				m_cList.SetItemText(item, 2, "调入");
			}
			else
			{
				plda[index].inst = PLINSTATUS_USE;
				m_cList.SetItemText(item, 2, "录用未调入");
			}
		}
	}

	POSITION pos = m_cList.GetFirstSelectedItemPosition();

	// 更新按钮状态
	if (pos)
	{
		m_btnFree.EnableWindow();
	}
	else
	{
		m_btnFree.EnableWindow(FALSE);
	}

	m_btnUse.EnableWindow(FALSE);

	m_cList.SetFocus();
}


// 刷新插件信息表
void CPluginSet::OnBtnUpdate() 
{
	ASSERT(isirw_is_busy() == 3);						// 引擎当前必须是已被锁定的

	int		ret;
	char	mess[MAX_PATH];

	// 显示等待光标、锁定列表视重绘
	AfxGetApp()->DoWaitCursor(1);
	m_cList.LockWindowUpdate();

	// 删除所有列表项
	m_cList.DeleteAllItems();

	memset((void*)plda, 0, sizeof(LDATA)*MAX_PLUGIN_NUM);
	
	plicnt = 0;
	selcnt = 0;

	UPDATAINFO	udp;

	// 刷新插件信息表
	switch (ret=isirw_update(pirwhead, IRWE_UPDTYPE_DISMISSAL, &udp))
	{
	case	0:			/* 0: 刷新成功 */
		_init_list();	
		break;
	case	1:			/* 1: 无可用插件 */
		strcpy((char*)mess, "引擎中已没有任何图象读写插件（引擎将无法继续工作），建议关闭软件并检查插件目录！");
		break;
	case	2:			/* 2: 读写错误 */
		strcpy((char*)mess, "引擎在读插件文件时出现错误，请检查插件文件是否遭到破坏。建议先关闭本软件！");
		break;
	case	3:			/* 3: 内存不足 */
		strcpy((char*)mess, "计算机内存出现不足，建议关闭一个大型软件。");
		break;
	case	4:			/* 4: 系统异常 */
	default:
		strcpy((char*)mess, "操作系统异常，建议关闭本软件后重启系统！");
		break;
	}
	
	if (ret)
	{
		// 报告错误信息
		AfxMessageBox((LPCTSTR)mess);
	}
	else
	{
		// 报告刷新结果 
		sprintf((char*)mess, "刷新结果：\n---------------------------------------------------------\n * 新增插件 %d 个。\n\n * 注销无效插件 %d 个。\n\n * 升级插件 %d 个。", udp.newcnt, udp.delcnt, udp.updcnt);
		AfxMessageBox((LPCTSTR)mess, MB_ICONINFORMATION|MB_OK);
	}

	UnlockWindowUpdate();
	AfxGetApp()->DoWaitCursor(0);

	m_cList.SetFocus();

	return;
}


// 初始化插件列表时控件
void CPluginSet::_init_list()
{
	int					i, j, k, l, uc, nc;
	LPIRWMF_INFOBLOCK	pib;
	LVITEM				lvi;
	char				buf[MAX_PATH];
	
	uc = pirwhead->uc;
	nc = pirwhead->nc;
	
	// 插入录用插件信息项
	for (i=0, pib=pirwhead->puh; i<uc; i++, pib=pib->next)
	{
		plda[i].id   = pib->ifo.plug_id;
		plda[i].pol  = pib;
		plda[i].iost = (pib->act) ? PLINSTATUS_ACT : PLINSTATUS_USE;
		plda[i].inst = plda[i].iost;
		
		memset((void*)&lvi, 0, sizeof(LVITEM));

		// 插件名称
		lvi.mask     = LVIF_TEXT | LVIF_PARAM;
		lvi.iItem    = i;
		lvi.iSubItem = 0;
		lvi.pszText  = (LPTSTR)pib->ifo.irwp_name;
		lvi.lParam   = i;
		l = m_cList.InsertItem(&lvi);

		// 版本
		sprintf((char*)buf, "%d.%d", pib->ifo.irwp_version/10, pib->ifo.irwp_version%10);
		m_cList.SetItemText(l, 1, buf);

		// 状态
		m_cList.SetItemText(l, 2, (pib->act) ? "调入" : "录用未调入");
		
		// 优先级
		sprintf((char*)buf, "%d", pib->pr);
		m_cList.SetItemText(l, 3, buf);

		// 调试
		m_cList.SetItemText(l, 4, (pib->ifo.irwp_build_set) ? "发布" : "调试");
		
		// 插件ID
		sprintf((char*)buf, "%02X", (int)pib->ifo.plug_id);
		m_cList.SetItemText(l, 5, buf);
		
		// 开发人
		buf[0] = '\0';
		for (k=0; k<(int)pib->ifo.irwp_author_count; k++)
		{
			strcat((char*)buf, (const char *)pib->ifo.irwp_author[k].ai_name);
			if ((k+1)<(int)pib->ifo.irwp_author_count)
			{
				strcat((char*)buf, " and ");
			}
			else
			{
				strcat((char*)buf, ";");
			}
		}
		m_cList.SetItemText(l, 6, buf);
	}
	
	// 插入未用插件信息项
	for (j=0, pib=pirwhead->pnh; j<nc; j++, pib=pib->next)
	{
		plda[i+j].id   = pib->ifo.plug_id;
		plda[i+j].pol  = pib;
		plda[i+j].iost = PLINSTATUS_NOUSE;
		plda[i+j].inst = plda[i+j].iost;
		
		memset((void*)&lvi, 0, sizeof(LVITEM));
		
		// 插件名称
		lvi.mask     = LVIF_TEXT | LVIF_PARAM;
		lvi.iItem    = i+j;
		lvi.iSubItem = 0;
		lvi.pszText  = (LPTSTR)pib->ifo.irwp_name;
		lvi.lParam   = i+j;
		l = m_cList.InsertItem(&lvi);
		
		// 版本
		sprintf((char*)buf, "%d.%d", pib->ifo.irwp_version/10, pib->ifo.irwp_version%10);
		m_cList.SetItemText(l, 1, buf);
		
		// 状态
		m_cList.SetItemText(l, 2, "未录用");
		
		// 优先级
		sprintf((char*)buf, "%d", pib->pr);
		m_cList.SetItemText(l, 3, buf);
		
		// 调试
		m_cList.SetItemText(l, 4, (pib->ifo.irwp_build_set) ? "发布" : "调试");
		
		// 插件ID
		sprintf((char*)buf, "%02X", (int)pib->ifo.plug_id);
		m_cList.SetItemText(l, 5, buf);
		
		// 开发人
		buf[0] = '\0';
		for (k=0; k<(int)pib->ifo.irwp_author_count; k++)
		{
			strcat((char*)buf, (const char *)pib->ifo.irwp_author[k].ai_name);
			if ((k+1)<(int)pib->ifo.irwp_author_count)
			{
				strcat((char*)buf, " and ");
			}
			else
			{
				strcat((char*)buf, ";");
			}
		}
		m_cList.SetItemText(l, 6, buf);
	}

	plicnt = pirwhead->plugin_num;
}

