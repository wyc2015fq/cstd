/********************************************************************

	FileDialogEx.cpp

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
					对话框类实现文件。

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

#include "stdafx.h"
#include "iseetsvc.h"

#include "shellpidl.h"
#include "shelllist.h"
#include "shelltree.h"
#include "shellpath.h"

#include "Display.h"
#include "LeftView.h"
#include "RightView.h"

#include "MainFrm.h"
#include "iseetsvcDoc.h"
#include "FileDialogEx.h"

#include <dlgs.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileDialogEx

IMPLEMENT_DYNAMIC(CFileDialogEx, CFileDialog)

CFileDialogEx::CFileDialogEx(LPSPEC_DATAPACK pack) : CFileDialog(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, NULL, NULL)
{
	LPIRWMF_HEADER		pib;
	LPIRWMF_INFOBLOCK	pinfo;
	int					i;

	ASSERT(pack);

	// 等待包操作完成
	isirw_s_wait(pack, 0);
	
	m_pPack       = pack;
	m_saveinfocnt = 0;
	m_ctrlcnt     = 0;

	m_ext.Empty();

	memset((void*)m_saveinfo, 0, sizeof(SAVEPLUGININFO)*ISEEIRW_MAX_PLUGIN_CNT);
	memset((void*)m_pStatic, 0, sizeof(CStatic*)*ISD_CTLCNT);
	memset((void*)m_pComboBox, 0, sizeof(CComboBox*)*ISD_CTLCNT);
	memset((void*)m_save.para_value, 0, sizeof(SAVESTR));
	
	// 引擎不能处于锁定状态
	ASSERT(isirw_is_busy() != 3);
	
	// 等待引擎空闲下来
	while (isirw_is_busy())
	{
		::Sleep(0);
	}
	
	// 锁定图象读写引擎
	pib = isirw_lock();
	
	pinfo = pib->puh;
	
	// 将具有保存功能的插件信息提取出来，存放在信息数组中
	while (pinfo)
	{
		if (pinfo->ifo.irwp_function & IRWP_WRITE_SUPP)
		{
			m_saveinfo[m_saveinfocnt].index = m_saveinfocnt;
			m_saveinfo[m_saveinfocnt].piid  = pinfo->ifo.plug_id;
			strcpy((char*)m_saveinfo[m_saveinfocnt].str, (const char *)_strupr(pinfo->ifo.irwp_desc_info.idi_currency_name));
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)" 格式 (*.");
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)_strlwr(pinfo->ifo.irwp_desc_info.idi_currency_name));
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)")|");
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)"*.");
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)_strlwr(pinfo->ifo.irwp_desc_info.idi_currency_name));
			strcat((char*)m_saveinfo[m_saveinfocnt].str, (const char *)"|");
			memcpy((void*)&(m_saveinfo[m_saveinfocnt].cont), (const void *)&(pinfo->ifo.irwp_save), sizeof(IRW_SAVE_DESC));
			strcpy((char*)m_saveinfo[m_saveinfocnt].ext, (const char *)_strlwr(pinfo->ifo.irwp_desc_info.idi_currency_name));
			m_saveinfocnt++;
		}
		pinfo = pinfo->next;
	}
	
	isirw_unlock(pib);
	
	m_ofn.lpstrDefExt    = "";
	m_ofn.Flags			&= ~OFN_ENABLESIZING;
	m_ofn.Flags			&= ~OFN_OVERWRITEPROMPT;
	
	strcpy((char*)m_szFileName, "未命名");
	
	// 将MFC风格的过滤串转换为系统风格
	if (m_saveinfocnt)
	{
		for (i=0, m_strFilter.Empty(); i<m_saveinfocnt; i++)
		{
			m_strFilter += (LPCTSTR)m_saveinfo[i].str;
		}

		LPTSTR pch = m_strFilter.GetBuffer(0);

		// 用 '\0' 替换'|'符号
		while ((pch = _tcschr(pch, '|')) != NULL)
		{
			*pch++ = '\0';
		}

		m_ofn.lpstrFilter = m_strFilter;
	}

	m_iRunMark = 0;
}


BEGIN_MESSAGE_MAP(CFileDialogEx, CFileDialog)
	//{{AFX_MSG_MAP(CFileDialogEx)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CFileDialogEx::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	
	CWnd		*wndDlg = GetParent();
	CRect		Rect;
	int			i;

	// 此时包中必须含有图象的象素数据
	if (!(m_pPack->data_state&SDP_DATA_STATE_ORG_PIX))
	{
		m_iRunMark++;
	}

	for (i=0, Rect.SetRectEmpty(); i<ISD_CTLCNT; i++)
	{
		m_pStatic[i]   = new CStatic;
		m_pComboBox[i] = new CComboBox;
		
		// 创建新增控件窗口并设置字体
		if (m_pStatic[i])
		{
			m_pStatic[i]->Create((LPCTSTR)"", WS_CHILD|WS_TABSTOP|SS_LEFT|SS_LEFTNOWORDWRAP, Rect, wndDlg, IDC_SAVEASCNT_STC+i);
			m_pStatic[i]->SetFont(wndDlg->GetFont());
		}
		else
		{
			m_iRunMark++;
		}

		if (m_pComboBox[i])
		{
			m_pComboBox[i]->Create(WS_CHILD|CBS_DROPDOWN|WS_VSCROLL|WS_TABSTOP|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST|CBS_HASSTRINGS, Rect, wndDlg, IDC_SAVEASCNT_CON+i);
			m_pComboBox[i]->SetFont(wndDlg->GetFont());
		}
		else
		{
			m_iRunMark++;
		}
	}

	_SetFileType(0);
		
	return TRUE;
}

void CFileDialogEx::OnDestroy() 
{
	CFileDialog::OnDestroy();
	
	for (int i=0; i<ISD_CTLCNT; i++)
	{
		if (m_pStatic[i])
		{
			delete m_pStatic[i];
		}
		if (m_pComboBox[i])
		{
			delete m_pComboBox[i];
		}
	}
}


// 根据给定的字符串查找插件索引
int CFileDialogEx::_GetPluginIndex(LPCTSTR sou)
{
	for (int i=0; i<m_saveinfocnt; i++)
	{
		if (!memcmp((const void *)sou, (const void *)m_saveinfo[i].str, 12))
		{
			return i;
		}
	}
	return -1;
}


// 将过滤串索引转换为插件信息数组对应的索引
int CFileDialogEx::_GetPluginIndexFromFilterIndex(int idx)
{
	char	*p = (char*)m_ofn.lpstrFilter;
	int		i;

	ASSERT(p);

	for (i=1; i<idx; i++)
	{
		p += strlen((const char *)p)+1;					// 绕过过滤串对（2次）
		p += strlen((const char *)p)+1;
	}

	i = _GetPluginIndex((LPCTSTR)p);

	return i;
}


void CFileDialogEx::OnSize(UINT nType, int cx, int cy) 
{
	CFileDialog::OnSize(nType, cx, cy);
}


BOOL CFileDialogEx::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	OFNOTIFY			*pNotify = (OFNOTIFY*)lParam;
	
	switch(pNotify->hdr.code)
	{
	case CDN_TYPECHANGE:
		_SetFileType(pNotify->lpOFN->nFilterIndex);
		return TRUE;
	case CDN_FILEOK:
		*pResult = _OK();
		return TRUE;
	}

	return CFileDialog::OnNotify(wParam, lParam, pResult);
}


int CFileDialogEx::_SetFileType(int index)
{
	CWnd		*wndDlg = GetParent();
	int			i, j, k;
	DWORD		savebits;
	CString		stmp;

	// 如果初始化未成功，不执行下面的操作
	if (m_iRunMark)
	{
		return -1;
	}

	UpdateData(TRUE);

	static CRect	rcwnd(0,0,0,0);
	
	// 修正对话框尺寸
	if (rcwnd.IsRectEmpty())
	{
		wndDlg->GetWindowRect(rcwnd);
	}
	
	// 取得当前插件索引
	i = _GetPluginIndexFromFilterIndex(index);

	ASSERT(i >= 0);

	const UINT nControls = 5;	
	
	// 获取原始对话框相关控件窗口的尺寸数据
	UINT  Controls[nControls] = {stc3, stc2, edt1, cmb1, lst1};
	CRect rc[nControls];							
	
	// 修正原始窗口及控件矩形的坐标
	for (j=0; j<nControls; j++)
	{
		CWnd *wndCtrl = wndDlg->GetDlgItem(Controls[j]);
		wndCtrl->GetWindowRect(&rc[j]);
		wndDlg->ScreenToClient(&rc[j]);					// 转换为客户区坐标
	}

	// 新增一个控件窗口所需的高度
	int iExtraSize = rc[1].Height()+(rc[1].top-rc[0].bottom);
	
	// 设置位深度选择控制窗口标题
	m_pStatic[0]->SetWindowText("位深度选择:");
	// 清除以前的位数选项
	m_pComboBox[0]->ResetContent();

	// 当前插件支持的保存深度数据
	savebits = m_saveinfo[i].cont.bitcount;
	
	// 填充位深度项组合框
	for (j=0; j<ISEEIRW_MAX_BITCOUNT; j++)
	{
		if ((savebits>>j)&1)
		{
			stmp.Format("%d 位", j+1);
			k = m_pComboBox[0]->AddString((LPCTSTR)stmp);
			m_pComboBox[0]->SetItemData(k, j+1);
		}
	}

	// 缺省选择为第一项
	m_pComboBox[0]->SetCurSel(0);

	// 位深度控件窗口总是可见的
	m_pStatic[0]->SetWindowPos(NULL, rc[1].left, rc[1].top+iExtraSize, rc[1].Width(), rc[1].Height(), 0);
	m_pStatic[0]->ShowWindow(SW_SHOW);
	m_pComboBox[0]->SetWindowPos(NULL, rc[3].left, rc[3].top+iExtraSize, rc[3].Width(), rc[3].Height()+ISD_COMBOBOXLIST_HEIGHT, 0);
	m_pComboBox[0]->ShowWindow(SW_SHOW);

	// 先隐藏前次的控件窗口
	for (j=1; j<m_ctrlcnt+1; j++)
	{
		m_pStatic[j]->ShowWindow(SW_HIDE);
		m_pComboBox[j]->ShowWindow(SW_HIDE);
	}

	// 待新增的列表个数
	m_ctrlcnt = m_saveinfo[i].cont.count;

	// 生成新的控件窗口
	for(j=1; j<(m_ctrlcnt+1); j++)
	{
		CString strTxt;
		CRect   cr = rc[1];

		cr.OffsetRect(0, iExtraSize*(j+1));

		if(m_pStatic[j]->GetSafeHwnd())
		{
			m_pStatic[j]->SetWindowPos(NULL, cr.left, cr.top, cr.Width(), cr.Height(), 0);
			m_pStatic[j]->ShowWindow(SW_SHOW);
			strTxt = m_saveinfo[i].cont.para[j-1].desc;
			m_pStatic[j]->SetWindowText(strTxt+":");
		}
		else
		{
			strTxt = m_saveinfo[i].cont.para[j-1].desc;
			m_pStatic[j]->Create(strTxt+":", WS_CHILD|WS_VISIBLE, cr, wndDlg, IDC_SAVEASCNT_STC+j);
			m_pStatic[j]->SetFont(wndDlg->GetFont());
		}

		// 生成或者更新下拉控件
		cr = rc[3];

		cr.OffsetRect(0, iExtraSize*(j+1));
		cr.bottom += ISD_COMBOBOXLIST_HEIGHT;

		if(m_pComboBox[j]->GetSafeHwnd())
		{
			m_pComboBox[j]->SetWindowPos(NULL, cr.left, cr.top, cr.Width(), cr.Height(), 0);
			m_pComboBox[j]->ShowWindow(SW_SHOW);
		}
		else
		{
			m_pComboBox[j]->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|CBS_DROPDOWN|WS_TABSTOP|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST|CBS_HASSTRINGS, cr, wndDlg, IDC_SAVEASCNT_CON+j);
			m_pComboBox[j]->SetFont(wndDlg->GetFont());
		}

		//添加下拉控件的内容
		m_pComboBox[j]->ResetContent();

		// 加入选择项字串
		for (k=0; k<m_saveinfo[i].cont.para[j-1].count; k++)
		{
			stmp.Format("%s", (LPCTSTR)m_saveinfo[i].cont.para[j-1].value_desc[k]);
			m_pComboBox[j]->AddString((LPCTSTR)stmp);
		}

		m_pComboBox[j]->SetCurSel(0);
	}

	// 隐藏其余控件
	for(k=m_ctrlcnt+1; k<ISD_CTLCNT; k++)
	{
		if(m_pStatic[k]->GetSafeHwnd())
		{
			m_pStatic[k]->ShowWindow(SW_HIDE);
		}

		if(m_pComboBox[k]->GetSafeHwnd())
		{
			m_pComboBox[k]->ShowWindow(SW_HIDE);
		}
	}

	// 设置缺省扩展名
	m_ext = (LPCSTR)m_saveinfo[i].ext;

	// 修正对话框尺寸
	wndDlg->SetWindowPos(NULL, 0, 0, rcwnd.Width(), rcwnd.Height()+iExtraSize*(m_ctrlcnt), SWP_NOMOVE);
	
	UpdateData(FALSE);

	return 0;
}


// 保存文件
BOOL CFileDialogEx::_OK()
{
	// 此时包中必须含有图象的象素数据
	if (!(m_pPack->data_state&SDP_DATA_STATE_ORG_PIX))
	{
		m_iRunMark++;
	}
	
	UpdateData(TRUE);

	// 获取目标文件路径
	CString		strExt      = GetFileExt();
	CString		strFileName = GetFileName();
	CString		strPathName = GetPathName();
	CFileFind	fileFind;
	CString		strMsg;

	// 修正文件扩展名
	if (strExt.IsEmpty())
	{
		strPathName += ".";
		strPathName += m_ext;
		strFileName += ".";
		strFileName += m_ext;
	}
	else
	{
		if (_stricmp((const char *)strExt, (const char *)m_ext))
		{
			strPathName += ".";
			strPathName += m_ext;
			strFileName += ".";
			strFileName += m_ext;
		}
	}

	// 判断文件是否存在
	if(fileFind.FindFile(strPathName))
	{
		strMsg.Format("文件 %s 已经存在，要替换该文件吗？", strFileName);

		if(MessageBox(strMsg, "另存为", MB_ICONQUESTION|MB_YESNO) == IDNO)
		{
			UpdateData(FALSE);
			return TRUE;
		}
	}

	// 收集用户选择信息
	for(int i=0; i<m_ctrlcnt; i++)
	{
		m_save.para_value[i] = m_pComboBox[i+1]->GetCurSel();
	}

	// 获取目标图象为深度信息
	int	bitcount = (int)m_pComboBox[0]->GetItemData(m_pComboBox[0]->GetCurSel());

	// 保存原转换包数据
	IRWE_CNVMARK	cnvmode    = m_pPack->cnvmark;
	LPINFOSTR		oldcnvpack = m_pPack->cnv_img;

	m_pPack->cnv_img = 0;
	m_pPack->cnvmark = IRWE_CNVMARK_NOT;

	if (oldcnvpack)
	{
		m_pPack->data_state &= ~SDP_DATA_STATE_CNV;
	}

	// 将图象转换为用户指定的位深
	if (isirw_cnv(m_pPack, _GetStdIDFromBitCnt(bitcount), 0) >= 0)
	{
		// 保存图象
		if (isirw_s_write_img(ISEEIO_CONFER_LOC_FILE_SEQUENTIAL, (const char *)strPathName, 0, 0, m_pPack, &m_save, 0) == 0)
		{
			// 等待操作完成
			AfxGetApp()->DoWaitCursor(1);
			int retwait = isirw_s_wait(m_pPack, 0);
			AfxGetApp()->DoWaitCursor(0);
			
			switch (retwait)
			{
			case	ER_SUCCESS:
				break;
			case	ER_USERBREAK:
				_DispLastErrMess(ISIRW_LASTERR_USER);
				break;
			case	ER_MEMORYERR:
				_DispLastErrMess(ISIRW_LASTERR_MEM);
				break;
			case	ER_FILERWERR:
				_DispLastErrMess(ISIRW_LASTERR_RW);
				break;
			case	ER_SYSERR:
				_DispLastErrMess(ISIRW_LASTERR_OSERR);
				break;
			case	ER_NSIMGFOR:
				ASSERT(0);								// 插件设计问题
				break;
			default:
				ASSERT(0);								// 不可能是其它值
				break;
			}
		}
		else
		{
			_DispLastErrMess(isirw_s_get_last_err());
		}
	}
	else
	{
		_DispLastErrMess(ISIRW_LASTERR_MEM);
	}

	// 释放转换包
	if (m_pPack->cnv_img)
	{
		isirw_free_INFOSTR(m_pPack->cnv_img);
	}

	if (oldcnvpack)
	{
		m_pPack->data_state |= SDP_DATA_STATE_CNV;
	}
	
	// 恢复原状态
	m_pPack->cnv_img = oldcnvpack;
	m_pPack->cnvmark = cnvmode;

	UpdateData(FALSE);

	return FALSE;
}


// 将位深值转换为标准格式ID
IRWE_CNVMARK CFileDialogEx::_GetStdIDFromBitCnt(int bitcnt)
{
	IRWE_CNVMARK ret = IRWE_CNVMARK_32;

	switch (bitcnt)
	{
	case	1:
		ret = IRWE_CNVMARK_1;
		break;
	case	4:
		ret = IRWE_CNVMARK_4;
		break;
	case	8:
		ret = IRWE_CNVMARK_8;
		break;
	case	16:
		ret = IRWE_CNVMARK_16_555;						// 保存功能只支持555格式
		break;
	case	24:
		ret = IRWE_CNVMARK_24;
		break;
	case	32:
		ret = IRWE_CNVMARK_32;
		break;
	default:
		ASSERT(0);										// 插件保存功能提供的位深不可能超出标准格式之外
		break;
	}
	
	return ret;
}


// 显示错误信息
void CFileDialogEx::_DispLastErrMess(int err)
{
	CString		str;

	str.Empty();

	switch (err)
	{
	case	ISIRW_LASTERR_SUCCESS:				// 没有错误
		break;
	case	ISIRW_LASTERR_SYSCLOSE:				// 引擎系统未开启
		str = "ISee图象读写引擎系统未开启，无法保存图象！";
		break;
	case	ISIRW_LASTERR_MEM:					// 内存不足
		str = "内存不足，保存操作无法完成！";
		break;
	case	ISIRW_LASTERR_RW:					// 介质读写错误
		str = "发生磁盘读写错误，保存操作无法完成！";
		break;
	case	ISIRW_LASTERR_USER:					// 操作被用户中断
		str = "用户中断保存操作，该操作尚未完成。";
		break;
	case	ISIRW_LASTERR_IMGERR:				// 图象错误(受损或非图象)
		str = "图象错误(受损或非图象)。";
		break;
	case	ISIRW_LASTERR_OSERR:				// 操作系统错误
		str = "操作系统错误！保存操作无法完成！";
		break;
	default:
		ASSERT(0);
		str = "软件异常，建议退出程序！";
		break;
	}

	if (!str.IsEmpty())
	{
		AfxMessageBox((LPCTSTR)str);
	}
}

