/********************************************************************

	iseetsvcDoc.cpp

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
	本文件用途：	图像读写引擎－C语言版－VC编译器－测试软件文档管理
					类实现文件。

	本文件编写人：	
					YZ				yzfree##sina.com

	本文件版本：	30118
	最后修改于：	2003-1-18

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
		地址收集软件。

	修正历史：

		2003-1		第一个测试版发布

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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CDisplay		*CIseetsvcDoc::m_pView = 0;				// 视类
HWND			CIseetsvcDoc::m_hView  = 0;				// 视窗口句柄



/////////////////////////////////////////////////////////////////////////////
// CIseetsvcDoc

IMPLEMENT_DYNCREATE(CIseetsvcDoc, CDocument)

BEGIN_MESSAGE_MAP(CIseetsvcDoc, CDocument)
	//{{AFX_MSG_MAP(CIseetsvcDoc)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_AS, OnUpdateFileSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CIseetsvcDoc construction/destruction

CIseetsvcDoc::CIseetsvcDoc()
{
	m_pPack = 0;
	m_bPkst = FALSE;
	memset(&m_dInfo, 0, sizeof(BITMAPINFO_EX));
}


CIseetsvcDoc::~CIseetsvcDoc()
{
	if (m_pPack)
	{
		isirw_free_SPEC_DATAPACK(m_pPack);
	}
}


BOOL CIseetsvcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// 获取上一次运行本软件时最后的路径
	CString	csInitPath = AfxGetApp()->GetProfileString(ITSVCMAIN_RKEYSEC, ITSVCDOC_RKEY_LASTPATH, "");

	// 组装树及列表控件
	m_spath.SetInitPath((LPCSTR)csInitPath);
	
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CIseetsvcDoc serialization

void CIseetsvcDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}



/////////////////////////////////////////////////////////////////////////////
// CIseetsvcDoc diagnostics

#ifdef _DEBUG
void CIseetsvcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CIseetsvcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIseetsvcDoc commands

void CIseetsvcDoc::OnCloseDocument() 
{
	// 保存本次运行期的最后路径
	AfxGetApp()->WriteProfileString(ITSVCMAIN_RKEYSEC, ITSVCDOC_RKEY_LASTPATH, (LPCTSTR)m_spath.GetCurrPath());

	CDocument::OnCloseDocument();
}


int __cdecl CIseetsvcDoc::_cbfun(LPSPEC_DATAPACK psdp, PFUN_COMM comm, int imgnum, int sl, int lcnt, int rw)
{
	static int	img = 0;								// 当前处理的图象帧号（主：0...）

	// 回调函数将只处理读函数通知
	if (rw == 1)
	{
		return 0;
	}

	// 分类发送通知
	switch (comm)
	{
	case	PFC_INTO_PLUG:					/* 开始执行命令 */
		::PostMessage(m_hView, WM_DISP_INTOPLUG, (WPARAM)psdp, (LPARAM)0);
		break;
	case	PFC_BUILD_MAST_IMAGE:			/* 创立主图象 */
		img = imgnum;
		::PostMessage(m_hView, WM_DISP_BUILDMI, (WPARAM)psdp, (LPARAM)0);
		break;
	case	PFC_BUILD_SERT_IMAGE:			/* 创立新的子图象 */
		img = imgnum;
		::PostMessage(m_hView, WM_DISP_BUILDSI, (WPARAM)psdp, (LPARAM)imgnum);
		break;
	case	PFC_PROCESS:					/* 步进 */
		::PostMessage(m_hView, WM_DISP_PROCESS, (WPARAM)psdp, (LPARAM)sl);
		break;
	case	PFC_OVER_SIR:					/* 结束一副图象的操作（主图象或子图象）*/
		::PostMessage(m_hView, WM_DISP_OVER, (WPARAM)psdp, (LPARAM)img);
		break;
	case	PFC_OK:							/* 命令被成功执行 */
		::PostMessage(m_hView, WM_DISP_OK, (WPARAM)psdp, (LPARAM)0);
		break;
	case	PFC_BREAK:						/* 命令被中断 */
		::PostMessage(m_hView, WM_DISP_FAILD, (WPARAM)psdp, (LPARAM)0);
		break;
	default:
		break;
	}

	return 0;
}


BOOL CIseetsvcDoc::OnIdleProc(LONG count)
{
	if (m_spath.a_bChangeTag)
	{
		if (m_spath.a_cTime.IsEnd() == TRUE)
		{
			m_spath.a_bChangeTag = FALSE;

			m_bPkst = FALSE;

			// 销毁旧图象包
			if (m_pPack)
			{
				// 如果旧图象包没有读取完毕，则中断它的操作
				if (m_pPack->exec_state != IRWE_SDPSTATE_IDLE)
				{
					isirw_s_wait(m_pPack, -1);
				}

				// 中断当前的动画播放（如果是动画文件的话）
				m_pView->m_cAniThread->PostThreadMessage(WM_DANI_DIABLE, 0, 0);

				// 等待动画线程的回应
				while (m_pView->m_bDiableMark == FALSE)
				{
					Sleep(0);
				}

				m_pView->m_bDiableMark = FALSE;
				m_pView->m_iCurrFr     = 0;

				// 释放包
				isirw_free_SPEC_DATAPACK(m_pPack);
				m_pPack = 0;
				
				// 清理显示位置的数据
				m_pView->ClearDispPosInfo();
			}
			
			if (!m_spath.a_cPath.IsEmpty())
			{
				// 读取新图象
				if (isirw_s_read_img_c(m_spath.a_eMedium, (LPCSTR)m_spath.a_cPath, m_spath.a_dParam1, m_spath.a_dParam2, m_spath.a_dSN, IRWE_CNVMARK_32, (void*)_cbfun, &m_pPack) != ISIRW_LASTERR_SUCCESS)
				{
					ASSERT(!m_pPack);
					m_pView->Invalidate();
					m_pView->UpdateWindow();
				}
			}
			else
			{
				CMainFrame		*cmf  = (CMainFrame*)AfxGetMainWnd();
				CString			stime;

				// 更新状态条中的图象信息串
				m_pView->m_sImgInfo.Format("0×0×0－0");
				
				if (cmf)
				{
					cmf->m_wndStatusBar.SetPaneText(cmf->m_wndStatusBar.CommandToIndex(IDS_IMGINFO), (LPCTSTR)m_pView->m_sImgInfo);
				}

				// 更新读时间窗格
				stime.Format("0 毫秒");

				if (cmf)
				{
					cmf->m_wndStatusBar.SetPaneText(cmf->m_wndStatusBar.CommandToIndex(IDS_TIMECNT), (LPCTSTR)stime);
				}
					
				// 切换目录时进行清屏
				m_pView->Invalidate();
				m_pView->UpdateWindow();
			}
			
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}

	return FALSE;
}



void CIseetsvcDoc::OnFileSaveAs() 
{
	CFileDialogEx	dlg(m_pPack);
	
	dlg.DoModal();
	
	return;
}

void CIseetsvcDoc::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	if (m_bPkst)
	{
		pCmdUI->Enable();
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}
