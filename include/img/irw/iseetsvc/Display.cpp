/********************************************************************

	Display.cpp

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
	本文件用途：	图像读写引擎－C语言版－VC编译器－测试软件图视
					类实现文件。

	本文件编写人：	
					YZ				yzfree##sina.com

	本文件版本：	30627
	最后修改于：	2003-6-27

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
		地址收集软件。

	修正历史：

		2003-6		压制DrawDibDraw中使用高度的一个问题(梦晖 mssoft##sina.com)
		2003-5		实现自动缩放、预定显示位置功能
		2003-4		增加增加缩放、拖动功能，改善图象显示速度
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CDisplay

IMPLEMENT_DYNCREATE(CDisplay, CView)

CDisplay::CDisplay()
{
	m_hDrawDib	= DrawDibOpen();						// 打开DrawDib库
	m_sImgInfo.Empty();

	m_iViewModl     = ISDISP_NOL;						// 缺省为正常模式
	m_iTalAutResize = TRUE;								// 全局自动缩放标志(缺省为不自动缩放)
	m_fTalRatio		= DPTRS_DEFAULT;					// 全局缩放倍数(缺省为真实模式)
	m_iAutoResize	= m_iTalAutResize;
	m_fRatio		= m_fTalRatio;
	m_iPassMark		= FALSE;

	m_iInitPos		= DPT_CENTER;						// 缺省为中点对齐
	m_iSwitch		= ISDISP_SWH_FAST;					// 缺省为快速图象切换模式
	
	m_rcPict.SetRectEmpty();							// 框中图象尺寸（包括缩放）
	m_rcBmpRc.SetRectEmpty();							// 图象原始尺寸
	m_rcFTRc.SetRectEmpty();
	m_rcPTRc.SetRectEmpty();

	m_rcHeader.SetRectEmpty();
	m_rcImage.SetRectEmpty();
	m_rcFrme.SetRectEmpty();							// 包围图片的修饰框
	m_rcClient.SetRectEmpty();

	m_cpPrePt.x = m_cpPrePt.y = 0;

	m_bMoveing     = FALSE;
	m_pLineBuf     = 0;
	m_pDispBuf     = 0;
	m_cCurrPackCnt = 0;
	m_icbTrueState = 0;
	m_icbResize    = 0;
	m_lBuffSize    = 0;

	m_hcHSd = ::AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	m_hcHFr = ::AfxGetApp()->LoadCursor(IDC_CUR_HANDFREE);
	m_hcHOd = ::AfxGetApp()->LoadCursor(IDC_CUR_HANDFOLD);
	m_hcHPs = ::AfxGetApp()->LoadCursor(IDC_CUR_PASS);

	m_iCurrFr     = 0;
	m_cAniThread  = 0;
	m_bDiableMark = FALSE;
	m_iInfo       = 0;
}


CDisplay::~CDisplay()
{
	if (m_hDrawDib)
	{
		DrawDibClose(m_hDrawDib);						// 关闭DrawDib库
	}
}


BEGIN_MESSAGE_MAP(CDisplay, CView)
	//{{AFX_MSG_MAP(CDisplay)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_DISP_INTOPLUG, OnDispIntoPlug)
	ON_MESSAGE(WM_DISP_BUILDMI, OnDispBuildMi)
	ON_MESSAGE(WM_DISP_BUILDSI, OnDispBuildSi)
	ON_MESSAGE(WM_DISP_PROCESS, OnDispProcess)
	ON_MESSAGE(WM_DISP_OVER, OnDispOver)
	ON_MESSAGE(WM_DISP_OK, OnDispOK)
	ON_MESSAGE(WM_DISP_FAILD, OnDispFaild)
	ON_MESSAGE(WM_DANI_NEXT, OnDispAniNext)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ISDBTNID_DISPTYPE, OnBtn1)
	ON_BN_CLICKED(ISDBTNID_RS, OnBtn2)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// 动画播放线程
UINT CDisplay::AniProc(LPVOID parm)
{
	CDisplay		*pDisp = (CDisplay*)parm;
	LPSPEC_DATAPACK	pPack  = 0;

	MSG				msg;
	MSG				msgtmp;
	BOOL			bRet, bAbi, bNew, bTnl;
	int				idx, count, i, l, k, tm;
	
	// 通知主线程动画线程初始化完毕
	pDisp->PostMessage(WM_DANI_INIT);
	
	// 消息循环
	while((bRet=::GetMessage(&msg, NULL, 0, 0)) != 0)
    { 
        if (bRet == -1)
        {
            continue;
        }
        else
        {
			if (msg.message == WM_DANI_QUIT)			// 退出消息
			{
				break;
			}
			else if (msg.message == WM_DANI_NEWCOMM)	// 新命令消息
			{
				// 提取专业图象包及循环播放标志
				pPack = (LPSPEC_DATAPACK)msg.wParam;
				bAbi  = (BOOL)msg.lParam;
				
				if (pPack == 0)
				{
					ASSERT(0);
					continue;
				}
				
				// 如果是静态图象（单帧），则忽略它
				if (pPack->org_img->imgnumbers <= 1)
				{
					continue;
				}

				bNew                 = FALSE;
				pDisp->m_bDiableMark = FALSE;

				// 播放动画图象
				while (bNew == FALSE)
				{
					// 如果存在播放表，则按播放表播放
					count = (pPack->org_img->play_order != 0) ? pPack->org_img->play_number : pPack->org_img->imgnumbers;

					for (i=0; i<count; i++)
					{
						idx = (pPack->org_img->play_order != 0) ? (int)pPack->org_img->play_order[i] : i;

						// 发送显示指定子图象的通知消息
						pDisp->PostMessage(WM_DANI_NEXT, (WPARAM)pPack, (LPARAM)idx);

						// 获取延迟时间（毫秒）
						if (idx == 0)
						{
							tm = (int)pPack->org_img->time;
						}
						else
						{
							tm  = isirw_s_get_subimg(pPack, idx, 0)->time;
						}

						// 太大或太小的延迟值将被修正
						tm = min(tm, ISDISP_MAXANI_TIME);
						tm = max(tm, ISDISP_MINANI_TIME);
						
						k    = tm/ISDISP_MINANI_TIME;
						bTnl = FALSE;

						// 延迟，以使图象停留指定时间
						for (l=0; l<k; l++)
						{
							::Sleep(ISDISP_MINANI_TIME);

							// 如果有新的命令到来，则废弃当前的命令
							if (::PeekMessage(&msgtmp, NULL, WM_DANI_NEWCOMM, WM_DANI_QUIT, PM_NOREMOVE))
							{
								bTnl = TRUE;
								bNew = TRUE;
								break;
							}
						}

						if (bTnl == TRUE)
						{
							break;
						}
					}

					// 如果未设循环标志，则播放一遍后退出
					if (bAbi == FALSE) 
					{
						bNew = TRUE;
					}
					else
					{
						::Sleep(0);
					}
				}
			}
			else if (msg.message == WM_DANI_DIABLE)		// 废弃当前命令
			{
				pDisp->m_bDiableMark = TRUE;
				continue;
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


/////////////////////////////////////////////////////////////////////////////
// CDisplay drawing

void CDisplay::OnDraw(CDC* pDC)
{
	CIseetsvcDoc* pDoc = (CIseetsvcDoc*)GetDocument();

	CTimeCounter	ctcr;
	ctcr.BeginCount();
	
	// 绘制标题部分
	DrawHeader(pDC, pDoc->m_spath.m_iCurrIdx);

	// 绘制图象
	DrawImage(pDC);

	DWORD ttt = ctcr.GetTime();

	return;
}



/////////////////////////////////////////////////////////////////////////////
// CDisplay diagnostics

#ifdef _DEBUG
void CDisplay::AssertValid() const
{
	CView::AssertValid();
}

void CDisplay::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CDisplay message handlers

int CDisplay::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	m_cbBackupBmp.LoadBitmap(IDB_BITMAP7);

	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 创建动画延时线程
	if (!(m_cAniThread=::AfxBeginThread(AniProc, (LPVOID)this)))
	{
		return -1;
	}
	
	// 申请图象翻转缓冲区
	if (!(m_pLineBuf=(char*)isirw_malloc(ISDISP_LINEBUFFSIZE)))
	{
		// 销毁动画延时线程
		m_cAniThread->PostThreadMessage(WM_DANI_QUIT, 0, 0);
		::WaitForSingleObject(m_cAniThread->m_hThread, 1000);
		return -1;
	}

	m_lBuffSize = ISDISP_MAXBUFFSIZE;
	
	// 申请显示缓冲区
	if (!(m_pDispBuf=(char*)isirw_malloc(m_lBuffSize)))
	{
		isirw_free(m_pLineBuf);
		m_pLineBuf = 0;

		// 销毁动画延时线程
		m_cAniThread->PostThreadMessage(WM_DANI_QUIT, 0, 0);
		::WaitForSingleObject(m_cAniThread->m_hThread, 1000);
		return -1;
	}

	CRect	rc;

	rc.SetRect(ISDISP_MINBTN_WIDTH, ISDISP_TOP+ISDISP_HEADER_H-2-ISDISP_BTN_H, ISDISP_MINBTN_WIDTH+ISDISP_BTN_W, ISDISP_TOP+ISDISP_HEADER_H-2);
	m_cbTrue.Create("", BS_PUSHBUTTON|WS_VISIBLE|WS_CHILDWINDOW, rc, this, ISDBTNID_DISPTYPE);
	m_cbTrue.SetBitmaps(IDB_BITMAP3, 0);
	m_cbTrue.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(255,255,255), 0);
	m_cbTrue.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255,255,255), 0);
	m_cbTrue.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(0x7b, 0xd7, 0xff), 0);
	m_cbTrue.SetTooltipText("自动缩放和原始尺寸切换");

	rc.SetRect(ISDISP_MINBTN_WIDTH+ISDISP_BTN_W+ISDISP_BTN_SPEC, ISDISP_TOP+ISDISP_HEADER_H-2-ISDISP_BTN_H, ISDISP_MINBTN_WIDTH+ISDISP_BTN_W*2+ISDISP_BTN_SPEC, ISDISP_TOP+ISDISP_HEADER_H-2);
	m_cbResize.Create("", BS_PUSHBUTTON|WS_VISIBLE|WS_CHILDWINDOW, rc, this, ISDBTNID_RS);
	m_cbResize.SetBitmaps(IDB_BITMAP1, 0);
	m_cbResize.SetColor(CButtonST::BTNST_COLOR_BK_OUT, RGB(255,255,255), 0);
	m_cbResize.SetColor(CButtonST::BTNST_COLOR_BK_IN, RGB(255,255,255), 0);
	m_cbResize.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, RGB(0x7b, 0xd7, 0xff), 0);
	m_cbResize.SetTooltipText("放大与缩小");
	
	return 0;
}


void CDisplay::OnSize(UINT nType, int cx, int cy) 
{
	// 修正显示区尺寸数据
	SetDispLocInfo();

	// 在自动缩放模式下，更新缩放比例
	if (m_iAutoResize)
	{
		SetPictLocInfo();
		SetFrmeLocInfo(TRUE);
	}

	// 修正图象的显示位置
	SetFrmeLocInfo();

	// 修正按钮位置
	if ((cx > (ISDISP_LEFT+ISDISP_HEADER_W+ISDISP_MINBTN_WIDTH+ISDISP_BTN_W+ISDISP_BTN_SPEC+ISDISP_BTN_W+ISDISP_RIGHT))&&(cy > (ISDISP_TOP+ISDISP_HEADER_H+ISDISP_HISPEC)))
	{
		int x = max((ISDISP_LEFT+ISDISP_HEADER_W+ISDISP_MINBTN_WIDTH), (cx-ISDISP_RIGHT-ISDISP_BTN_W-ISDISP_BTN_SPEC-ISDISP_BTN_W));
		int y = ISDISP_TOP+ISDISP_HEADER_H-2-ISDISP_BTN_H;

		if (m_icbTrueState == 0)
		{
			m_icbTrueState = 1;
			m_cbTrue.ShowWindow(SW_SHOW);
		}
		m_cbTrue.MoveWindow(CRect(x,y,x+ISDISP_BTN_W,y+ISDISP_BTN_H));

		if (m_icbResize == 0)
		{
			m_icbResize = 1;
			m_cbResize.ShowWindow(SW_SHOW);
		}
		x += ISDISP_BTN_W+ISDISP_BTN_SPEC;
		m_cbResize.MoveWindow(CRect(x,y,x+ISDISP_BTN_W,y+ISDISP_BTN_H));
	}
	else
	{
		// 如果标题块与按钮间距过小，则隐藏按钮
		m_cbTrue.ShowWindow(SW_HIDE);
		m_icbTrueState = 0;
		m_cbResize.ShowWindow(SW_HIDE);
		m_icbResize = 0;
	}
	
	CView::OnSize(nType, cx, cy);
}


// 绘制显示区上方的标签
void CDisplay::DrawHeader(CDC *pDC, int ord)
{
	if (m_rcHeader.IsRectEmpty())
	{
		return;
	}

	COLORREF oldbkc = pDC->GetBkColor();

	// 设置索引号背景色
	pDC->SetBkColor((COLORREF)(DWORD)ISCOL_HEADER_BG);
	
	// 绘制索引号显示背景框
	pDC->FillSolidRect(m_rcHeader.left, m_rcHeader.top, ISDISP_HEADER_W, ISDISP_HEADER_H, (COLORREF)(DWORD)ISCOL_HEADER_BG);

	CPen	pen;
	CPen	*oldpen;
	LOGPEN	logpen;

	logpen.lopnStyle   = PS_SOLID;
	logpen.lopnWidth.x = 0;
	logpen.lopnWidth.y = 0;
	logpen.lopnColor   = (COLORREF)(DWORD)ISCOL_HEADER_BG;

	pen.CreatePenIndirect(&logpen);

	oldpen = (CPen*)pDC->SelectObject(&pen);

	// 绘制标题基线
	pDC->MoveTo(m_rcHeader.left, m_rcHeader.bottom-1);
	pDC->LineTo(m_rcHeader.right, m_rcHeader.bottom-1);

	pDC->SelectObject(oldpen);

	COLORREF	oldtxtcol = pDC->GetTextColor();
	CString		sbuf;

	pDC->SetTextColor(RGB(255,255,255));

	// 绘制索引号码
	if (ord == -1)
	{
		sbuf = TEXT("");
	}
	else
	{
		sbuf.Format("%d", ord);
	}

	// 计算索引号字符显示位置
	CSize		fntsize = pDC->GetOutputTabbedTextExtent(sbuf, 0, NULL);
	CRect		fntrect((POINT)CPoint(ISDISP_LEFT+(ISDISP_HEADER_W-fntsize.cx)/2, ISDISP_TOP+(ISDISP_HEADER_H-fntsize.cy)/2), fntsize);

	// 显示索引号
	pDC->DrawText(sbuf, fntrect, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	// 恢复DC设置
	pDC->SetTextColor(oldtxtcol);
	pDC->SetBkColor(oldbkc);
	
	return;
}


// 绘制图象及周围的画框
void CDisplay::DrawImage(CDC *pDC)
{
	CIseetsvcDoc*	pDoc = (CIseetsvcDoc*)GetDocument();
	ASSERT(pDoc);
	
	LPINFOSTR		pinfo;
	long n;
				
	if ((!m_rcImage.IsRectEmpty())&&(pDoc->m_pPack)&&(pDoc->m_pPack->org_img->p_bit_data))
	{
		pinfo = ((pDoc->m_pPack->exec_state == IRWE_SDPSTATE_IDLE)) ? pDoc->m_pPack->cnv_img : pDoc->m_pPack->org_img;

		// 排除不需要刷新的区域
		pDC->ExcludeClipRect(0, 0, ISDISP_LEFT, m_rcClient.bottom);
		pDC->ExcludeClipRect(0, 0, m_rcClient.right, m_rcHeader.bottom+ISDISP_HISPEC);
		pDC->ExcludeClipRect(m_rcClient.right-ISDISP_RIGHT, 0, m_rcClient.right, m_rcClient.bottom);
		pDC->ExcludeClipRect(0, m_rcClient.bottom-ISDISP_BOTTOM, m_rcClient.right, m_rcClient.bottom);

		// 显示图象
		if (pDoc->m_pPack->exec_state != IRWE_SDPSTATE_IDLE)
		{
			if (m_iSwitch == ISDISP_SWH_FAST)
			{
				// 在读取的过程中采用StretchDIBits()函数显示图象（因为无法断言原始图象的正反方向）
				StretchDIBits(pDC->GetSafeHdc(), m_rcPTRc.left, m_rcPTRc.top, 
							m_rcPTRc.Width(), m_rcPTRc.Height(), 
							m_rcBmpRc.left, m_rcBmpRc.top, 
							m_rcBmpRc.Width(), m_rcBmpRc.Height(), 
							pinfo->p_bit_data, 
							(wBITMAPINFO*)&pDoc->m_dInfo, 
							DIB_RGB_COLORS, SRCCOPY);
			}
			else
			{
				;
			}
		}
		else
		{
			n = pDoc->m_dInfo.info.biHeight;
			if(n < 0)
				pDoc->m_dInfo.info.biHeight = -n;

			DrawDibDraw(m_hDrawDib, pDC->GetSafeHdc(), m_rcPTRc.left, m_rcPTRc.top, 
					m_rcPTRc.Width(), m_rcPTRc.Height(), 
					(wBITMAPINFOHEADER*)&pDoc->m_dInfo, 
					(LPVOID)m_pDispBuf, 
					0, 0, pinfo->width, pinfo->height, 
					DDF_HALFTONE);

			pDoc->m_dInfo.info.biHeight = n;
		}

		// 绘制附加图样
		if (!m_rcFrme.IsRectEmpty())
		{
			CPen	pen(PS_SOLID, 1, RGB(128,128,128));
			CPen	penlt(PS_SOLID, 1, RGB(225,225,225));
			CPen	penrb(PS_SOLID, 1, RGB(128,128,128));
			CPen	penrb1(PS_SOLID, 1, RGB(168,168,168));
			CPen	penrb2(PS_SOLID, 1, RGB(216,216,216));
			CPen	penrs(PS_SOLID, 1, RGB(173, 211, 255));
			CPen	*oldpen;
			int		x, y;

			oldpen = (CPen*)pDC->SelectObject(&penlt);

			// 绘制画框
			pDC->MoveTo(m_rcFTRc.left, m_rcFTRc.top);
			pDC->LineTo(m_rcFTRc.right-2, m_rcFTRc.top);
			pDC->MoveTo(m_rcFTRc.left, m_rcFTRc.top);
			pDC->LineTo(m_rcFTRc.left, m_rcFTRc.bottom-2);
			pDC->SelectObject(&penrb);
			pDC->MoveTo(m_rcFTRc.right-2, m_rcFTRc.top);
			pDC->LineTo(m_rcFTRc.right-2, m_rcFTRc.bottom-1);
			pDC->MoveTo(m_rcFTRc.left+1, m_rcFTRc.bottom-2);
			pDC->LineTo(m_rcFTRc.right-2, m_rcFTRc.bottom-2);
			pDC->SelectObject(&penrb1);
			pDC->MoveTo(m_rcFTRc.right-1, m_rcFTRc.top+1);
			pDC->LineTo(m_rcFTRc.right-1, m_rcFTRc.bottom);
			pDC->MoveTo(m_rcFTRc.left+2, m_rcFTRc.bottom-1);
			pDC->LineTo(m_rcFTRc.right-1, m_rcFTRc.bottom-1);
			pDC->SelectObject(&penrb2);
			pDC->MoveTo(m_rcFTRc.right, m_rcFTRc.top+2);
			pDC->LineTo(m_rcFTRc.right, m_rcFTRc.bottom+1);
			pDC->MoveTo(m_rcFTRc.left+3, m_rcFTRc.bottom);
			pDC->LineTo(m_rcFTRc.right, m_rcFTRc.bottom);

			if (m_fRatio != DPTRS_REAL)
			{
				pDC->SelectObject(&penrs);

				y = m_rcFTRc.bottom - ISDISP_FRMSPEC + ((ISDISP_FRMSPEC-2)/2-3);
				x = m_rcFTRc.CenterPoint().x;

				// 绘制缩放标记
				pDC->MoveTo(x-3, y);
				pDC->LineTo(x, y);
				pDC->MoveTo(x+2, y);
				pDC->LineTo(x+5, y);

				pDC->MoveTo(x-5, y+1);
				pDC->LineTo(x, y+1);
				pDC->MoveTo(x+2, y+1);
				pDC->LineTo(x+7, y+1);

				pDC->MoveTo(x-7, y+2);
				pDC->LineTo(x+9, y+2);
				pDC->MoveTo(x-7, y+3);
				pDC->LineTo(x+9, y+3);

				pDC->MoveTo(x-5, y+4);
				pDC->LineTo(x, y+4);
				pDC->MoveTo(x+2, y+4);
				pDC->LineTo(x+7, y+4);
				
				pDC->MoveTo(x-3, y+5);
				pDC->LineTo(x, y+5);
				pDC->MoveTo(x+2, y+5);
				pDC->LineTo(x+5, y+5);
			}
			
			pDC->SelectObject(oldpen);
		}
	}
	else
	{
		if (pDoc->m_pPack)
		{
			return;
		}

		// 没有选择图象时，直接返回
		if (pDoc->m_spath.m_iCurrIdx == -1)
		{
			return;
		}

		// 如果当前没有可显示的图象，则显示缺省背景图象
		pDC->ExcludeClipRect(0, 0, ISDISP_LEFT, m_rcClient.bottom);
		pDC->ExcludeClipRect(0, 0, m_rcClient.right, m_rcHeader.bottom+ISDISP_HISPEC);
		pDC->ExcludeClipRect(m_rcClient.right-ISDISP_RIGHT, 0, m_rcClient.right, m_rcClient.bottom);
		pDC->ExcludeClipRect(0, m_rcClient.bottom-ISDISP_BOTTOM, m_rcClient.right, m_rcClient.bottom);

		CDC			cmdc;
		CBitmap		*oldbmp;
		CRect		rct;
		CRect		rcbmp;
		BITMAP		bim;

		memset((void*)&bim, 0, sizeof(BITMAP));
		m_cbBackupBmp.GetBitmap(&bim);

		rcbmp.SetRect(0, 0, bim.bmWidth, bim.bmHeight);

		if (cmdc.CreateCompatibleDC(pDC))
		{
			oldbmp = (CBitmap*)cmdc.SelectObject(&m_cbBackupBmp);

			rct.SetRect(m_rcImage.CenterPoint().x-rcbmp.CenterPoint().x, 
						m_rcImage.CenterPoint().y-rcbmp.CenterPoint().y, 
						(m_rcImage.CenterPoint().x-rcbmp.CenterPoint().x)+rcbmp.Width(), 
						(m_rcImage.CenterPoint().y-rcbmp.CenterPoint().y)+rcbmp.Height());

			pDC->BitBlt(rct.left, rct.top, rct.Width(), rct.Height(), &cmdc, 0, 0, SRCCOPY);

			cmdc.SelectObject(oldbmp);
		}
	}
}


// 更新图片原始尺寸、缩放尺寸及画框尺寸（当更换图象对象时调用）
void CDisplay::SetPictLocInfo(BOOL init)
{
	CIseetsvcDoc*	pDoc = (CIseetsvcDoc*)GetDocument();
	LPSPEC_DATAPACK pdsp = pDoc->m_pPack;

	// 如果没有图象对象，则简单的返回
	if (pdsp == 0)
	{
		return;
	}

	if (init == TRUE)
	{
		// 设置图象真实（原始）尺寸结构
		m_rcBmpRc.SetRect(0, 0, pdsp->org_img->width, pdsp->org_img->height);

		// 从全局标签中获取缩放参数
		m_iAutoResize = m_iTalAutResize;
		m_fRatio      = m_fTalRatio;
	}

	// 如果是自动缩放
	if (m_iAutoResize)
	{
		int		w, h;
		float	wr, hr;

		w = m_rcImage.Width() - ISDISP_FRMSPEC*2;
		h = m_rcImage.Height() - ISDISP_FRMSPEC*2;

		w = max(1, w);
		h = max(1, h);

		wr = (float)w/(float)m_rcBmpRc.Width();			// 计算宽、高的缩放比例
		hr = (float)h/(float)m_rcBmpRc.Height();
			
		if ((wr >= 1.0)&&(hr >= 1.0))
		{
			m_fRatio = DPTRS_REAL;						// 显示区尺寸比图象大，采用真实尺寸
		}
		else
		{
			m_fRatio = max((min(wr,hr)), DPTRS_MIN);	// 否则缩小尺寸(限度修正)
		}
	}
	else
	{
		ASSERT(m_fRatio >= DPTRS_MIN);
		ASSERT(m_fRatio <= DPTRS_MAX);
	}

	m_rcFrme.left = 0;
	m_rcFrme.top  = 0;
	
	// 设置图象显示尺寸（包括缩放）、及边框显示尺寸
	if (m_fRatio == DPTRS_REAL)							// 实际尺寸
	{
		m_rcPict = m_rcBmpRc;

		if ((m_rcPict.Width() < ISDISP_MIN_WIDTH)&&(m_rcPict.Height() < ISDISP_MIN_HEIGHT))
		{
			m_rcFrme.right  = ISDISP_MIN_WIDTH+ISDISP_FRMSPEC*2;
			m_rcFrme.bottom = ISDISP_MIN_HEIGHT+ISDISP_FRMSPEC*2;
		}
		else
		{
			m_rcFrme.right  = m_rcPict.Width()+ISDISP_FRMSPEC*2;
			m_rcFrme.bottom = m_rcPict.Height()+ISDISP_FRMSPEC*2;
		}
	}
	else if (m_fRatio > DPTRS_REAL)						// 放大
	{
		ASSERT(m_fRatio <= DPTRS_MAX);

		m_rcPict.SetRect(0, 0, (int)((float)m_rcBmpRc.Width()*m_fRatio+0.5), (int)((float)m_rcBmpRc.Height()*m_fRatio+0.5));

		if ((m_rcPict.Width() < ISDISP_MIN_WIDTH)&&(m_rcPict.Height() < ISDISP_MIN_HEIGHT))
		{
			m_rcFrme.right  = ISDISP_MIN_WIDTH+ISDISP_FRMSPEC*2;
			m_rcFrme.bottom = ISDISP_MIN_HEIGHT+ISDISP_FRMSPEC*2;
		}
		else
		{
			m_rcFrme.right  = m_rcPict.Width()+ISDISP_FRMSPEC*2;
			m_rcFrme.bottom = m_rcPict.Height()+ISDISP_FRMSPEC*2;
		}
	}
	else												// 缩小
	{
		ASSERT(m_fRatio >= DPTRS_MIN);

		m_rcPict.SetRect(0, 0, max(((int)((float)m_rcBmpRc.Width()*m_fRatio+0.5)),1), max(((int)((float)m_rcBmpRc.Height()*m_fRatio+0.5)),1));

		if ((m_rcPict.Width() < ISDISP_MIN_WIDTH)&&(m_rcPict.Height() < ISDISP_MIN_HEIGHT))
		{
			m_rcFrme.right  = ISDISP_MIN_WIDTH+ISDISP_FRMSPEC*2;
			m_rcFrme.bottom = ISDISP_MIN_HEIGHT+ISDISP_FRMSPEC*2;
		}
		else
		{
			m_rcFrme.right  = m_rcPict.Width()+ISDISP_FRMSPEC*2;
			m_rcFrme.bottom = m_rcPict.Height()+ISDISP_FRMSPEC*2;
		}
	}
}


// 更新标签框及图象框的尺寸
void CDisplay::SetDispLocInfo()
{
	// 更新客户区尺寸
	GetClientRect(m_rcClient);

	// 更新标签矩形（索引号显示部位）及图象框
	if ((m_rcClient.Width() <= (ISDISP_LEFT+ISDISP_RIGHT))||(m_rcClient.Height() <= (ISDISP_TOP+ISDISP_HEADER_H)))
	{
		m_rcHeader.SetRectEmpty();
		m_rcImage.SetRectEmpty();
	}
	else
	{
		m_rcHeader.SetRect(ISDISP_LEFT, ISDISP_TOP, m_rcClient.Width()-ISDISP_RIGHT, ISDISP_TOP+ISDISP_HEADER_H);
		
		if (m_rcClient.Height() <= (ISDISP_TOP+ISDISP_HEADER_H+ISDISP_HISPEC))
		{
			m_rcImage.SetRectEmpty();
		}
		else
		{
			m_rcImage.SetRect(ISDISP_LEFT, ISDISP_TOP+ISDISP_HEADER_H+ISDISP_HISPEC, m_rcClient.Width()-ISDISP_RIGHT, m_rcClient.Height()-ISDISP_BOTTOM);
		}
	}
}


// 更新图象的显示坐标参数
void CDisplay::SetFrmeLocInfo(BOOL init)
{
	CIseetsvcDoc*	pDoc = (CIseetsvcDoc*)GetDocument();
	LPSPEC_DATAPACK pdsp = pDoc->m_pPack;

	// 当没有图片对象时只简单的返回
	if (pdsp == 0)
	{
		return;
	}

	// 更新图象显示坐标矩形
	if (init == FALSE)									// （当窗体改变尺寸时）
	{
		int off;

		// 修正X轴位置，已使图象能自动跟踪窗体尺寸的变化
		if (m_rcImage.Width() >= m_rcFTRc.Width())
		{
			m_rcFTRc.left  = m_rcImage.CenterPoint().x - m_rcFTRc.Width()/2;
			m_rcFTRc.right = m_rcFTRc.left+m_rcFrme.Width();
			m_rcPTRc.left  = m_rcImage.CenterPoint().x - m_rcPTRc.Width()/2;
			m_rcPTRc.right = m_rcPTRc.left+m_rcPict.Width();
		}
		else
		{
			if (m_rcImage.right > m_rcFTRc.right)
			{
				off = m_rcImage.right-m_rcFTRc.right;

				m_rcFTRc.OffsetRect(off, 0);
				m_rcPTRc.OffsetRect(off, 0);
			}
			else
			{
				if (m_rcFTRc.left > m_rcImage.left)
				{
					off = m_rcImage.left-m_rcFTRc.left;

					m_rcFTRc.OffsetRect(off, 0);
					m_rcPTRc.OffsetRect(off, 0);
				}
			}
		}

		// 修正Y轴位置
		if (m_rcImage.Height() >= m_rcFTRc.Height())
		{
			m_rcFTRc.top    = m_rcImage.CenterPoint().y - m_rcFTRc.Height()/2;
			m_rcFTRc.bottom	= m_rcFTRc.top+m_rcFrme.Height();
			m_rcPTRc.top    = m_rcImage.CenterPoint().y - m_rcPTRc.Height()/2;
			m_rcPTRc.bottom = m_rcPTRc.top+m_rcPict.Height();
		}
		else
		{
			if (m_rcImage.bottom > m_rcFTRc.bottom)
			{
				off = m_rcImage.bottom-m_rcFTRc.bottom;

				m_rcFTRc.OffsetRect(0, off);
				m_rcPTRc.OffsetRect(0, off);
			}
			else
			{
				if (m_rcFTRc.top > m_rcImage.top)
				{
					off = m_rcImage.top-m_rcFTRc.top;

					m_rcFTRc.OffsetRect(0, off);
					m_rcPTRc.OffsetRect(0, off);
				}
			}
		}

		return;
	}
	else												// （当更换图象对象时调用）
	{
		// 如果图象显示区太小，则不进行进一步的计算
		if (m_rcImage.IsRectEmpty())
		{
			return;
		}

		ASSERT(!m_rcFrme.IsRectEmpty());				// 图象的尺寸数据必不为空
		ASSERT(!m_rcPict.IsRectEmpty());

		CPoint	cpt = m_rcImage.CenterPoint();
		int		xf, yf, xb, yb, x1, y1, x2, y2;
		
		// 根据对齐方式，设定图象初始显示位置
		switch (m_iInitPos)
		{
		case	DPT_LEFTTOP:							// 左上角对齐
			xf = m_rcImage.CenterPoint().x-m_rcFrme.Width()/2;
			yf = m_rcImage.CenterPoint().y-m_rcFrme.Height()/2;
			xb = m_rcImage.CenterPoint().x-m_rcPict.Width()/2;
			yb = m_rcImage.CenterPoint().y-m_rcPict.Height()/2;
			
			x1 = max(xf, m_rcImage.left);
			y1 = max(yf, m_rcImage.top);
			x2 = x1+m_rcFrme.Width();
			y2 = y1+m_rcFrme.Height();
			m_rcFTRc.SetRect(x1, y1, x2, y2);

			x1 = max(xb, m_rcImage.left+ISDISP_FRMSPEC);
			y1 = max(yb, m_rcImage.top+ISDISP_FRMSPEC);
			x2 = x1+m_rcPict.Width();
			y2 = y1+m_rcPict.Height();
			m_rcPTRc.SetRect(x1, y1, x2, y2);
			break;
		case	DPT_RIGHTTOP:							// 右上角对齐
			xf = m_rcImage.CenterPoint().x+m_rcFrme.Width()/2;
			yf = m_rcImage.CenterPoint().y-m_rcFrme.Height()/2;
			xb = m_rcImage.CenterPoint().x+m_rcPict.Width()/2;
			yb = m_rcImage.CenterPoint().y-m_rcPict.Height()/2;
			
			x2 = min(xf, m_rcImage.right);
			y1 = max(yf, m_rcImage.top);
			x1 = x2-m_rcFrme.Width();
			y2 = y1+m_rcFrme.Height();
			m_rcFTRc.SetRect(x1, y1, x2, y2);
			
			x2 = min(xb, m_rcImage.right-ISDISP_FRMSPEC);
			y1 = max(yb, m_rcImage.top+ISDISP_FRMSPEC);
			x1 = x2-m_rcPict.Width();
			y2 = y1+m_rcPict.Height();
			m_rcPTRc.SetRect(x1, y1, x2, y2);
			break;
		case	DPT_LEFTBOTTOM:							// 左下角对齐
			xf = m_rcImage.CenterPoint().x-m_rcFrme.Width()/2;
			yf = m_rcImage.CenterPoint().y+m_rcFrme.Height()/2;
			xb = m_rcImage.CenterPoint().x-m_rcPict.Width()/2;
			yb = m_rcImage.CenterPoint().y+m_rcPict.Height()/2;
			
			x1 = max(xf, m_rcImage.left);
			y2 = min(yf, m_rcImage.bottom);
			x2 = x1+m_rcFrme.Width();
			y1 = y2-m_rcFrme.Height();
			m_rcFTRc.SetRect(x1, y1, x2, y2);
			
			x1 = max(xb, m_rcImage.left+ISDISP_FRMSPEC);
			y2 = min(yb, m_rcImage.bottom-ISDISP_FRMSPEC);
			x2 = x1+m_rcPict.Width();
			y1 = y2-m_rcPict.Height();
			m_rcPTRc.SetRect(x1, y1, x2, y2);
			break;
		case	DPT_RIGHTBOTTOM:						// 右下角对齐
			xf = m_rcImage.CenterPoint().x+m_rcFrme.Width()/2;
			yf = m_rcImage.CenterPoint().y+m_rcFrme.Height()/2;
			xb = m_rcImage.CenterPoint().x+m_rcPict.Width()/2;
			yb = m_rcImage.CenterPoint().y+m_rcPict.Height()/2;
			
			x2 = min(xf, m_rcImage.right);
			y2 = min(yf, m_rcImage.bottom);
			x1 = x2-m_rcFrme.Width();
			y1 = y2-m_rcFrme.Height();
			m_rcFTRc.SetRect(x1, y1, x2, y2);
			
			x2 = min(xb, m_rcImage.right-ISDISP_FRMSPEC);
			y2 = min(yb, m_rcImage.bottom-ISDISP_FRMSPEC);
			x1 = x2-m_rcPict.Width();
			y1 = y2-m_rcPict.Height();
			m_rcPTRc.SetRect(x1, y1, x2, y2);
			break;
		case	DPT_CENTER:								// 中点对齐
			m_rcFTRc.SetRect(m_rcImage.CenterPoint().x-m_rcFrme.Width()/2, 
				m_rcImage.CenterPoint().y-m_rcFrme.Height()/2,
				(m_rcImage.CenterPoint().x-m_rcFrme.Width()/2)+m_rcFrme.Width(), 
				(m_rcImage.CenterPoint().y-m_rcFrme.Height()/2)+m_rcFrme.Height());
			
			m_rcPTRc.SetRect(m_rcImage.CenterPoint().x-m_rcPict.Width()/2, 
				m_rcImage.CenterPoint().y-m_rcPict.Height()/2,
				(m_rcImage.CenterPoint().x-m_rcPict.Width()/2)+m_rcPict.Width(), 
				(m_rcImage.CenterPoint().y-m_rcPict.Height()/2)+m_rcPict.Height());
			break;
		default:
			ASSERT(0);
			break;
		}
	}
}


// 动态修正当前光标外观
BOOL CDisplay::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CRect	rc;
	CPoint	pt;
	SHORT	kbs;
	
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	
	// 判断光标是否在图象上面
	rc.IntersectRect((LPCRECT)m_rcPTRc, (LPCRECT)m_rcImage);

	kbs = GetAsyncKeyState(VK_CONTROL) & 0x8000;

	if ((kbs)||(m_iPassMark))
	{
		// 显示放大镜光标
		if (rc.PtInRect(pt))
		{
			SetCursor(m_hcHPs);
		}
		else
		{
			SetCursor(m_hcHSd);
		}
	}
	else
	{
		// 如果当前图象已被完全显示出来了，则显示标准箭头光标
		if (IsDrop() == FALSE)
		{
			SetCursor(m_hcHSd);
			return TRUE;
		}
		
		// 如果光标在图象上面，则显示手形光标
		if (rc.PtInRect(pt))
		{
			SetCursor(m_hcHFr);
		}
		else
		{
			SetCursor(m_hcHSd);
		}
	}

	return TRUE;
}


// 鼠标左键压下处理
void CDisplay::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CIseetsvcDoc*	pDoc = (CIseetsvcDoc*)GetDocument();
	LPSPEC_DATAPACK pdsp = pDoc->m_pPack;

	if (pdsp == 0)
	{
		CView::OnLButtonDown(nFlags, point);
		return;
	}

	CRect	rc;
	
	rc.IntersectRect((LPCRECT)m_rcPTRc, (LPCRECT)m_rcImage);
	
	// 判断光标是否在图象上方
	if (rc.PtInRect(point))
	{
		if ((nFlags&MK_CONTROL)||(m_iPassMark))		// 放大镜模式
		{
			_RSL(point);
		}
		else if (IsDrop())			// 如果光标在图象上面，并且图象没有被完全显示出来，则捕捉光标，开始一次拖动过程
		{
			SetCursor(m_hcHOd);
			::SetCapture((HWND)(*this));
			m_bMoveing = TRUE;
			m_cpPrePt = point;
		}
		else
		{
			;
		}
	}
	
	CView::OnLButtonDown(nFlags, point);
}


// 鼠标左键松开处理
void CDisplay::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// 如果当前处于拖动过程中，则释放光标捕捉，并恢复光标外形
	if (m_bMoveing == TRUE)
	{
		m_cpPrePt = point;
		m_bMoveing = FALSE;
		::ReleaseCapture();
		SetCursor(m_hcHFr);
	}
	
	CView::OnLButtonUp(nFlags, point);
}


// 鼠标左键双击处理
void CDisplay::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if ((nFlags&MK_CONTROL)||(m_iPassMark))		// 放大镜模式
	{
		OnLButtonDown(nFlags, point);
		return;
	}

	CIseetsvcDoc*	pDoc = (CIseetsvcDoc*)GetDocument();
	LPSPEC_DATAPACK pdsp = pDoc->m_pPack;
	
	if (pdsp == 0)
	{
		CView::OnLButtonDblClk(nFlags, point);
		return;
	}

	CMainFrame*	pMain = (CMainFrame*)AfxGetMainWnd();
	CRect		rc;

	if (m_iViewModl == ISDISP_NOL)
	{
		// 保存当前各窗口的位置及尺寸信息
		pMain->GetWindowPlacement(&m_wp);

		pMain->m_spa.GetPane(0,0)->GetWindowRect(rc);
		m_iOTW = rc.Width();
		pMain->m_spa.GetPane(0,1)->GetWindowRect(rc);
		m_iORW = rc.Width();
		pMain->m_spb.GetPane(0,0)->GetWindowRect(rc);
		m_iODW = rc.Width();
		pMain->m_spb.GetPane(0,1)->GetWindowRect(rc);
		m_iOLW = rc.Width();

		// 如果框架窗口未处于最大化状态，则使之最大化
		if (m_wp.showCmd != SW_SHOWMAXIMIZED)
		{
			WINDOWPLACEMENT	wp;

			memcpy((void*)&wp, (const void *)&m_wp, sizeof(WINDOWPLACEMENT));
			wp.showCmd = SW_SHOWMAXIMIZED;
			pMain->SetWindowPlacement(&wp);
		}
		
		// 隐藏树视、缩小列表视，扩大显示区
		pMain->GetClientRect(rc);
		pMain->m_spa.SetColumnInfo(0, ISDISP_TREE_MIN, 0);
		pMain->m_spa.SetColumnInfo(1, rc.Width(), 0);
		pMain->m_spb.SetColumnInfo(0, rc.Width()-ISDISP_LISTSS, 0);
		pMain->m_spb.SetColumnInfo(1, ISDISP_LISTSS, 0);

		m_iViewModl = ISDISP_VIEWMODL;
	}
	else
	{
		// 将各窗口尺寸恢复到正常浏览模式保存的尺寸
		pMain->SetWindowPlacement(&m_wp);

		pMain->m_spa.SetColumnInfo(0, m_iOTW, 0);
		pMain->m_spa.SetColumnInfo(1, m_iORW, 0);
		pMain->m_spb.SetColumnInfo(0, m_iODW, 0);
		pMain->m_spb.SetColumnInfo(1, m_iOLW, 0);

		m_iViewModl = ISDISP_NOL;
	}

	// 分裂窗口重新布局
	pMain->m_spa.RecalcLayout();
	pMain->m_spb.RecalcLayout();

	CView::OnLButtonDblClk(nFlags, point);
}


// 鼠标右键双击处理
void CDisplay::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	if ((nFlags&MK_CONTROL)||(m_iPassMark))		// 放大镜模式
	{
		OnRButtonDown(nFlags, point);
		return;
	}
	

	
	CView::OnRButtonDblClk(nFlags, point);
}


// 鼠标移动处理
void CDisplay::OnMouseMove(UINT nFlags, CPoint point) 
{
	// 如果当前处于拖动过程，则进行进一步的处理
	if (m_bMoveing == TRUE)
	{
		CPoint	cpt   = m_cpPrePt;
		BOOL	bMark = FALSE;
		CPoint	cpoff(0,0);
		int		i;

		ASSERT(nFlags & MK_LBUTTON);
		
		cpt -= point;
	
		i = abs(cpt.x);

		if (cpt.x < 0)								// 向右拖动
		{
			if (m_rcFTRc.left < m_rcImage.left)
			{
				i = ((m_rcFTRc.left+i) > m_rcImage.left) ? (m_rcImage.left-m_rcFTRc.left) : i;
				m_rcFTRc.OffsetRect(i, 0);
				m_rcPTRc.OffsetRect(i, 0);
				cpoff.x = i;
				bMark   = TRUE;
			}
		}
		else if (cpt.x > 0)							// 向左拖动
		{
			if (m_rcFTRc.right > m_rcImage.right)
			{
				i = ((m_rcFTRc.right-i) < m_rcImage.right) ? (m_rcFTRc.right-m_rcImage.right) : i;
				m_rcFTRc.OffsetRect(-i, 0);
				m_rcPTRc.OffsetRect(-i, 0);
				cpoff.x = -i;
				bMark   = TRUE;
			}
		}
		else										// 没有 X 轴方向的拖动
		{
			;
		}


		i = abs(cpt.y);

		if (cpt.y < 0)								// 向下拖动
		{
			if (m_rcFTRc.top < m_rcImage.top)
			{
				i = ((m_rcFTRc.top+i) > m_rcImage.top) ? (m_rcImage.top-m_rcFTRc.top) : i;
				m_rcFTRc.OffsetRect(0, i);
				m_rcPTRc.OffsetRect(0, i);
				cpoff.y = i;
				bMark   = TRUE;
			}
		}
		else if (cpt.y > 0)							// 向上拖动
		{
			if (m_rcFTRc.bottom > m_rcImage.bottom)
			{
				i = ((m_rcFTRc.bottom-i) < m_rcImage.bottom) ? (m_rcFTRc.bottom-m_rcImage.bottom) : i;
				m_rcFTRc.OffsetRect(0, -i);
				m_rcPTRc.OffsetRect(0, -i);
				cpoff.y = -i;
				bMark   = TRUE;
			}
		}
		else										// 没有 Y 轴方向的拖动
		{
			;
		}

		// 如果发生了位移，则滚动窗口中的内容
		if (bMark == TRUE)
		{
			m_cpPrePt = point;
			ScrollWindowEx(cpoff.x, cpoff.y, m_rcImage, m_rcImage, 0, 0, SW_INVALIDATE|SW_ERASE);
		}
	}
	
	CView::OnMouseMove(nFlags, point);
}


// 判断是否需要拖动才能看到全图
BOOL CDisplay::IsDrop()
{
	CRect	rc;
	
	rc.UnionRect((LPCRECT)m_rcFTRc, (LPCRECT)m_rcImage);
	
	if (rc == m_rcImage)
	{
		return FALSE;									// 无需拖动即可看到全图	
	}
	else
	{
		return TRUE;									// 需要拖动
	}
}


// 当关闭一个图象文件后，调用本函数做显示位置数据的清理
void CDisplay::ClearDispPosInfo()
{
	m_iCurrFr = 0;
	m_iInfo   = 0;

	m_rcBmpRc.SetRectEmpty();							// 图象原始尺寸
	
	m_rcPict.SetRectEmpty();							// 框中图象的缩放尺寸
	m_rcPTRc.SetRectEmpty();							// 图象的客户坐标
	m_rcFrme.SetRectEmpty();							// 包围图片的修饰框的尺寸
	m_rcFTRc.SetRectEmpty();							// 修饰框的客户坐标
}


// 判断指定的包是否是当前处理的数据包
BOOL CDisplay::IsCurrPack(LPSPEC_DATAPACK psdp)
{
	CIseetsvcDoc*	pDoc = (CIseetsvcDoc*)GetDocument();
	ASSERT(pDoc);

	return (psdp == pDoc->m_pPack);
}


// 控制进入相应插件的回调通知
LRESULT CDisplay::OnDispIntoPlug(WPARAM wParam, LPARAM lParam)
{
	// 判断是否是过期的命令
	if (!IsCurrPack((LPSPEC_DATAPACK)wParam))
	{
		return -1;
	}

	// 计算读取一幅图象所需的时间（起始时间）
	if (m_cCurrPackCnt)
	{
		m_cCurrPackCnt->BeginCount();
	}

	return 0;
}


// 数据包建立主图象的通知
LRESULT CDisplay::OnDispBuildMi(WPARAM wParam, LPARAM lParam)
{
	// 判断是否是过期的命令
	if (!IsCurrPack((LPSPEC_DATAPACK)wParam))
	{
		return -1;
	}

	CIseetsvcDoc*	pDoc = (CIseetsvcDoc*)GetDocument();
	ASSERT(pDoc);
	ASSERT(pDoc->m_pPack);

	LPINFOSTR pinfo = pDoc->m_pPack->org_img;

	// 填写新图象的尺寸、显示等信息
	pDoc->m_dInfo.info.biSize          = sizeof(wBITMAPINFOHEADER);
	pDoc->m_dInfo.info.biWidth         = pinfo->width;
	pDoc->m_dInfo.info.biHeight        = (pinfo->order == 0) ? -(long)pinfo->height : pinfo->height;
	pDoc->m_dInfo.info.biPlanes        = 1;
	pDoc->m_dInfo.info.biBitCount      = (WORD)pinfo->bitcount;
	pDoc->m_dInfo.info.biCompression   = BI_RGB;
	pDoc->m_dInfo.info.biSizeImage     = 0;
	pDoc->m_dInfo.info.biXPelsPerMeter = 0;
	pDoc->m_dInfo.info.biYPelsPerMeter = 0;
	pDoc->m_dInfo.info.biClrUsed       = 0;
	pDoc->m_dInfo.info.biClrImportant  = 0;
	
	memcpy((void*)pDoc->m_dInfo.pal, (const void *)pinfo->palette, sizeof(wRGBQUAD)*MAX_PALETTE);
	
	// 设置当前帧号（0－主图象）
	m_iCurrFr = 0;

	// 更新当前图象的各种尺寸数据
	SetPictLocInfo(TRUE);
	SetFrmeLocInfo(TRUE);

	// 如果是快速切换方式，则及时刷新显示
	if (m_iSwitch == ISDISP_SWH_FAST)
	{
		// 如果图象比显示区大时，则不清除屏幕，以避免闪烁
		if ((m_rcFrme.Width() >= m_rcImage.Width())&&(m_rcFrme.Height() >= m_rcImage.Height()))
		{
			InvalidateRect(m_rcImage, FALSE);
		}
		else
		{
			InvalidateRect(m_rcImage, TRUE);
		}

		_UpdateHeadRgn(FALSE);

		// 刷新显示
		UpdateWindow();
	}
	else
	{
		;
	}

	return 0;
}


// 数据包建立子图象通知
LRESULT CDisplay::OnDispBuildSi(WPARAM wParam, LPARAM lParam)
{
	// 判断是否是过期的命令
	if (!IsCurrPack((LPSPEC_DATAPACK)wParam))
	{
		return -1;
	}

	return 0;
}


// 数据包读写图象步进通知
LRESULT CDisplay::OnDispProcess(WPARAM wParam, LPARAM lParam)
{
	// 判断是否是过期的命令
	if (!IsCurrPack((LPSPEC_DATAPACK)wParam))
	{
		return -1;
	}

	CMainFrame		*cmf = (CMainFrame*)AfxGetMainWnd();
	LPSPEC_DATAPACK	psdp = (LPSPEC_DATAPACK)wParam;
	CString			srat;
	
	// 显示读取进度
	srat.Format("%d%%", (int)((float)(psdp->s_current*100)/(float)psdp->s_total));
	cmf->m_wndStatusBar.SetPaneText(cmf->m_wndStatusBar.CommandToIndex(IDS_TIMECNT), (LPCTSTR)srat);
	
	if (m_iSwitch == ISDISP_SWH_FAST)
	{
		// 为加快速度，只有在不缩放的情况下才进行步进刷新
		if (m_fRatio == DPTRS_REAL)
		{
			InvalidateRect(m_rcImage, FALSE);
			//UpdateWindow();//UPK
		}
	}
	else
	{
		;
	}

	return 0;
}


// 数据包读写单帧图象结束通知
LRESULT CDisplay::OnDispOver(WPARAM wParam, LPARAM lParam)
{
	// 判断是否是过期的命令
	if (!IsCurrPack((LPSPEC_DATAPACK)wParam))
	{
		return -1;
	}

	if (m_iSwitch == ISDISP_SWH_FAST)
	{
		// 刷新显示（只响应主图象的结束通知）
		if ((lParam == 0)&&(m_fRatio == DPTRS_REAL)&&(((LPSPEC_DATAPACK)wParam)->org_img->imgnumbers == 1))
		{
			InvalidateRect(m_rcImage, FALSE);
			UpdateWindow();
		}
	}
	else
	{
		;
	}

	return 0;
}


// 数据包操作结束通知（成功）
LRESULT CDisplay::OnDispOK(WPARAM wParam, LPARAM lParam)
{
	// 判断是否是过期的命令
	if (!IsCurrPack((LPSPEC_DATAPACK)wParam))
	{
		return -1;
	}

	CIseetsvcDoc	*pDoc = (CIseetsvcDoc*)GetDocument();
	ASSERT(pDoc);
	ASSERT(m_pLineBuf);

	CMainFrame		*cmf  = (CMainFrame*)AfxGetMainWnd();
	
	// 显示读取该图象所花费的时间（毫秒）
	if (m_cCurrPackCnt)
	{
		DWORD		time  = m_cCurrPackCnt->GetTime();
		CString		stime;
		
		stime.Format("%d 毫秒", time);
		if (cmf)
		{
			cmf->m_wndStatusBar.SetPaneText(cmf->m_wndStatusBar.CommandToIndex(IDS_TIMECNT), (LPCTSTR)stime);
		}
	}
	
	LPINFOSTR		pinfo = pDoc->m_pPack->cnv_img;
	SUBIMGBLOCK*	psub;

	// 判断图象的宽度是否在可处理的范围内
	if (pinfo->width >= ISDISP_MAXWIDTH)
	{
		isirw_s_wait((LPSPEC_DATAPACK)wParam, 0);

		// 释放数据包（因操作失败，该包已无用）
		isirw_free_SPEC_DATAPACK(pDoc->m_pPack);
		
		pDoc->m_pPack = 0;
		
		m_iCurrFr = 0;

		InvalidateRect(m_rcImage, FALSE);
		UpdateWindow();

		return -2;
	}

	DWORD	i, linesize, hecnt;
	void	*ppv;

	// 如果是正向图，则翻转为倒向图
	if (pinfo->order == 0)
	{
		linesize = DIBSCANLINE_DWORDALIGN(pinfo->width*pinfo->bitcount);
		hecnt    = pinfo->height/2;

		for (i=0; i<hecnt; i++)
		{
			memcpy((void*)m_pLineBuf, (const void *)pinfo->pp_line_addr[i], linesize);
			memcpy((void*)pinfo->pp_line_addr[i], (const void *)pinfo->pp_line_addr[pinfo->height-1-i], linesize);
			memcpy((void*)pinfo->pp_line_addr[pinfo->height-1-i], (const void *)m_pLineBuf, linesize);

			ppv = pinfo->pp_line_addr[i];
			pinfo->pp_line_addr[i] = pinfo->pp_line_addr[pinfo->height-1-i];
			pinfo->pp_line_addr[pinfo->height-1-i] = ppv;
		}

		pinfo->order = 1;
		pDoc->m_dInfo.info.biHeight = pinfo->height;
	}

	// 如果存在子图象，则一并翻转
	if (pinfo->imgnumbers > 1)
	{
		psub = pinfo->psubimg;

		while (psub)
		{
			if (psub->order == 0)
			{
				linesize = DIBSCANLINE_DWORDALIGN(psub->width*psub->bitcount);
				hecnt    = psub->height/2;
				
				for (i=0; i<(DWORD)hecnt; i++)
				{
					memcpy((void*)m_pLineBuf, (const void *)psub->pp_line_addr[i], linesize);
					memcpy((void*)psub->pp_line_addr[i], (const void *)psub->pp_line_addr[psub->height-1-i], linesize);
					memcpy((void*)psub->pp_line_addr[psub->height-1-i], (const void *)m_pLineBuf, linesize);
					
					ppv = psub->pp_line_addr[i];
					psub->pp_line_addr[i] = psub->pp_line_addr[psub->height-1-i];
					psub->pp_line_addr[psub->height-1-i] = ppv;
				}
				psub->order = 1;
			}
			psub = psub->next;
		}
	}

	// 等待引擎的收尾工作完毕
	isirw_s_wait((LPSPEC_DATAPACK)wParam, 0);

	// 填写图象显示用信息
	pDoc->m_dInfo.info.biSize          = sizeof(wBITMAPINFOHEADER);
	pDoc->m_dInfo.info.biWidth         = pinfo->width;
	pDoc->m_dInfo.info.biHeight        = pinfo->height;
	pDoc->m_dInfo.info.biPlanes        = 1;
	pDoc->m_dInfo.info.biBitCount      = (WORD)pinfo->bitcount;
	pDoc->m_dInfo.info.biCompression   = BI_RGB;
	pDoc->m_dInfo.info.biSizeImage     = 0;
	pDoc->m_dInfo.info.biXPelsPerMeter = 0;
	pDoc->m_dInfo.info.biYPelsPerMeter = 0;
	pDoc->m_dInfo.info.biClrUsed       = 0;
	pDoc->m_dInfo.info.biClrImportant  = 0;
	
	memcpy((void*)pDoc->m_dInfo.pal, (const void *)pinfo->palette, sizeof(wRGBQUAD)*MAX_PALETTE);

	if (m_iSwitch == ISDISP_SWH_FAST)
	{
		linesize = DIBSCANLINE_DWORDALIGN(pinfo->width*pinfo->bitcount);
		hecnt    = pinfo->height;

		if ((linesize*hecnt) <= m_lBuffSize)
		{
			memcpy((void*)m_pDispBuf, (const void*)pinfo->p_bit_data, linesize*hecnt);
		}
		else
		{
			char *ptmp;

			// 申请更大的显示缓冲区
			if (ptmp=(char*)isirw_malloc(linesize*hecnt))
			{
				isirw_free(m_pDispBuf);
				m_pDispBuf  = ptmp;
				memcpy((void*)m_pDispBuf, (const void*)pinfo->p_bit_data, linesize*hecnt);
				m_lBuffSize = linesize*hecnt;
			}
			else
			{
				// 否则只能显示部分图象
				memcpy((void*)m_pDispBuf, (const void*)pinfo->p_bit_data, m_lBuffSize);
			}
		}
	}
	else
	{
		;
	}

	// 初始化新图象的显示参数
	m_iCurrFr = 0;
	m_iInfo   = 0;

	// 初始化图象信息串
	m_sImgInfo.Format("%d×%d×%d－%d", pinfo->width, pinfo->height, pinfo->pater->org_img->bitcount, pinfo->imgnumbers);

	// 在状态条中显示图象信息串
	if (cmf)
	{
		cmf->m_wndStatusBar.SetPaneText(cmf->m_wndStatusBar.CommandToIndex(IDS_IMGINFO), (LPCTSTR)m_sImgInfo);
	}

	_UpdateHeadRgn(FALSE);

	// 刷新显示
	InvalidateRect(m_rcImage, FALSE);
	UpdateWindow();
	
	// 设可操作标志为真
	pDoc->m_bPkst = TRUE;

	// 动画显示通知
	if (pinfo->imgnumbers > 1)
	{
		DWORD	ttm = pinfo->time;

		// 判断是否是动画图象（ICO文件中可能包含多幅图象，但它不是动画）
		for (i=1, psub=pinfo->psubimg; i<pinfo->imgnumbers; i++, psub=psub->next)
		{
			ASSERT(psub);
			ttm += psub->time;
		}

		// 发送通知
		if (ttm)
		{
			m_cAniThread->PostThreadMessage(WM_DANI_NEWCOMM, (WPARAM)pDoc->m_pPack, (LPARAM)1);
		}
	}

	return 0;
}


// 数据包操作结束通知（失败）
LRESULT CDisplay::OnDispFaild(WPARAM wParam, LPARAM lParam)
{
	// 判断是否是过期的命令
	if (!IsCurrPack((LPSPEC_DATAPACK)wParam))
	{
		return -1;
	}

	// 等待引擎的收尾工作完毕
	isirw_s_wait((LPSPEC_DATAPACK)wParam, 0);

	CIseetsvcDoc	*pDoc = (CIseetsvcDoc*)GetDocument();
	ASSERT(pDoc);
	ASSERT(pDoc->m_pPack);

	// 释放数据包（因操作失败，该包已无用）
	isirw_free_SPEC_DATAPACK(pDoc->m_pPack);
	
	pDoc->m_pPack = 0;

	m_iCurrFr = 0;

	// 刷新显示
	InvalidateRect(m_rcImage, TRUE);
	UpdateWindow();

	return 0;
}


// 显示下一帧子图象
LRESULT CDisplay::OnDispAniNext(WPARAM wParam, LPARAM lParam)
{
	// 判断是否是过期的命令
	if (!IsCurrPack((LPSPEC_DATAPACK)wParam))
	{
		return -1;
	}

	LPSPEC_DATAPACK	psdp  = (LPSPEC_DATAPACK)wParam;
	ASSERT(psdp);
	LPINFOSTR		pinfo = psdp->cnv_img;
	ASSERT(pinfo);
	SUBIMGBLOCK*	pold;
	SUBIMGBLOCK*	psub;
	
	DWORD			ckey;
	DWORD			*pdwSou, *pdwDes;
	
	int				linesize;
	int				nWidth;
	int				nHeight;
	int				i, j;

	// 进行置前处理
	switch (m_iInfo)
	{
	case	0:											// 不处置
		break;
	case	1:											// 简单覆盖
		break;
	case	2:											// 背景色填充
		pold = isirw_s_get_subimg(psdp, m_iCurrFr, 1);
		nWidth  = pold->width;
		nHeight = pold->height;
		linesize = DIBSCANLINE_DWORDALIGN(pinfo->width*pinfo->bitcount);
		
		for (i=0; i<nHeight; i++)
		{
			pdwSou = ((DWORD*)pinfo->pp_line_addr[i+pold->top])+pold->left;
			pdwDes = (DWORD*)(m_pDispBuf+(pinfo->height-i-1-pold->top)*linesize)+pold->left;
			
			for (j=0; j<nWidth; j++)
			{
				*pdwDes++ = *pdwSou++;
			}
		}
		break;
	case	3:											// 恢复原图
		break;
	default:											// 未定义行为
		break;
	}

	// 设置新的帧号
	m_iCurrFr = (int)lParam;

	// 根据图象帧号刷新显示缓冲区
	if (m_iCurrFr == 0)
	{
		m_iInfo  = 0;									// 主图象缺省处置方式为不处置:)
		linesize = DIBSCANLINE_DWORDALIGN(pinfo->width*pinfo->bitcount);
		memcpy((void*)m_pDispBuf, (const void *)pinfo->p_bit_data, pinfo->height*linesize);
	}
	else
	{
		// 保存处置方式
		psub = isirw_s_get_subimg(psdp, m_iCurrFr, 1);
		ASSERT(psub);
		m_iInfo = psub->dowith;

		if (psub->colorkey == -1L)
		{
			nWidth  = psub->width;
			nHeight = psub->height;
			linesize = DIBSCANLINE_DWORDALIGN(pinfo->width*pinfo->bitcount);
			
			for (i=0; i<nHeight; i++)
			{
				pdwSou = (DWORD*)psub->pp_line_addr[i];
				pdwDes = (DWORD*)(m_pDispBuf+(pinfo->height-i-1-psub->top)*linesize);
				pdwDes = pdwDes+psub->left;
				
				for (j=0; j<nWidth; j++)
				{
					*pdwDes++ = *pdwSou++;
				}
			}
		}		
		else
		{
			ckey    = (DWORD)psub->colorkey;
			nWidth  = psub->width;
			nHeight = psub->height;
			linesize = DIBSCANLINE_DWORDALIGN(pinfo->width*pinfo->bitcount);
			
			// 透明色方式的拷贝
			for (i=0; i<nHeight; i++)
			{
				pdwSou = (DWORD*)psub->pp_line_addr[i];
				pdwDes = (DWORD*)(m_pDispBuf+(pinfo->height-i-1-psub->top)*linesize);
				pdwDes = pdwDes+psub->left;
				
				for (j=0; j<nWidth; j++)
				{
					if (*pdwSou != ckey)
					{
						*pdwDes = *pdwSou;
					}
					pdwSou++;
					pdwDes++;
				}
			}
		}
	}

	// 显示当前帧
	InvalidateRect(m_rcImage, FALSE);
	UpdateWindow();
	
	return 0;
}


// 退出收尾
void CDisplay::OnDestroy() 
{
	CView::OnDestroy();

	// 保存当前的显示设定
	AfxGetApp()->WriteProfileInt(ITSVCMAIN_RKEYSEC, ITSVCDISP_RKEY_AUTORESIZE, m_iTalAutResize);
	AfxGetApp()->WriteProfileInt(ITSVCMAIN_RKEYSEC, ITSVCDISP_RKEY_RSROT, (int)(m_fTalRatio*100.0));
	AfxGetApp()->WriteProfileInt(ITSVCMAIN_RKEYSEC, ITSVCDISP_RKEY_INITPOS, m_iInitPos);
	AfxGetApp()->WriteProfileInt(ITSVCMAIN_RKEYSEC, ITSVCDISP_RKEY_SWITCH, m_iSwitch);

	// 销毁动画延时线程
	::PostThreadMessage(m_cAniThread->m_nThreadID, WM_DANI_QUIT, 0, 0);

	::WaitForSingleObject(m_cAniThread->m_hThread, 1000);
	
	// 删除计时器类
	if (m_cCurrPackCnt)
	{
		delete m_cCurrPackCnt;
		m_cCurrPackCnt = 0;
	}

	// 释放图象翻转缓冲区
	if (m_pLineBuf)
	{
		isirw_free(m_pLineBuf);
		m_pLineBuf = 0;
	}

	// 释放显示缓冲区
	if (m_pDispBuf)
	{
		isirw_free(m_pDispBuf);
		m_pDispBuf = 0;
	}
}


// 显示类初始化
void CDisplay::OnInitialUpdate() 
{
	// 申请计时器
	m_cCurrPackCnt = new CTimeCounter;

	// 恢复上一次的显示状态
	m_iTalAutResize = AfxGetApp()->GetProfileInt(ITSVCMAIN_RKEYSEC, ITSVCDISP_RKEY_AUTORESIZE, 1);
	m_fTalRatio     = ((float)(AfxGetApp()->GetProfileInt(ITSVCMAIN_RKEYSEC, ITSVCDISP_RKEY_RSROT, 100)))/(float)100.0;
	m_iInitPos      = AfxGetApp()->GetProfileInt(ITSVCMAIN_RKEYSEC, ITSVCDISP_RKEY_INITPOS, DPT_CENTER);
	m_iSwitch       = AfxGetApp()->GetProfileInt(ITSVCMAIN_RKEYSEC, ITSVCDISP_RKEY_SWITCH, ISDISP_SWH_FAST);

	CView::OnInitialUpdate();
}



void CDisplay::OnBtn1()
{
	if (m_iAutoResize)
	{
		m_iAutoResize = FALSE;
		m_fRatio = DPTRS_REAL;
	}
	else
	{
		m_iAutoResize = TRUE;
	}
	
	SetDispLocInfo();
	
	// 更新缩放比例
	SetPictLocInfo();
	
	// 修正图象的显示位置
	SetFrmeLocInfo(TRUE);
	
	// 刷新显示
	InvalidateRect(m_rcImage, TRUE);
	UpdateWindow();
	
	return;
}


void CDisplay::OnBtn2()
{
	if (m_iPassMark == TRUE)
	{
		m_iPassMark = FALSE;
	}
	else
	{
		m_iPassMark = TRUE;
	}

	return;
}


void CDisplay::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	
	CPoint	point;

	switch (nChar)
	{
	case	VK_CONTROL:
		m_iPassMark = TRUE;
		OnSetCursor(0, 0, 0);
		break;
	case	0xbb:
	case	VK_ADD:
		// 以显示区中点为放大中心
		point = m_rcImage.CenterPoint();
		_RSL(point);
		break;
	case	0xbd:
	case	VK_SUBTRACT:
		// 以显示区中点为缩小中心
		point = m_rcImage.CenterPoint();
		_RSS(point);
		break;
	}
}

void CDisplay::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CView::OnKeyUp(nChar, nRepCnt, nFlags);

	if (nChar == VK_CONTROL)
	{
		m_iPassMark = FALSE;
		OnSetCursor(0, 0, 0);
	}
}

void CDisplay::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CIseetsvcDoc*	pDoc = (CIseetsvcDoc*)GetDocument();
	LPSPEC_DATAPACK pdsp = pDoc->m_pPack;
	
	if (pdsp == 0)
	{
		CView::OnRButtonDown(nFlags, point);
		return;
	}
	
	CRect	rc;
	
	rc.IntersectRect((LPCRECT)m_rcPTRc, (LPCRECT)m_rcImage);
	
	// 判断光标是否在图象上方
	if (rc.PtInRect(point))
	{
		if ((nFlags&MK_CONTROL)||(m_iPassMark))		// 放大镜模式
		{
			_RSS(point);
		}
		else
		{
			;
		}
	}
	
	CView::OnRButtonDown(nFlags, point);
}


BOOL CDisplay::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// 当鼠标左键或右键已被压下时，不再处理滚轮消息
	if ((nFlags & MK_LBUTTON)||(nFlags & MK_RBUTTON))
	{
		return CView::OnMouseWheel(nFlags, zDelta, pt);
	}

	CIseetsvcDoc*	pDoc = (CIseetsvcDoc*)GetDocument();
	LPSPEC_DATAPACK pdsp = pDoc->m_pPack;

	// 如果没有图象，则直接返回
	if (pdsp == 0)
	{
		return CView::OnMouseWheel(nFlags, zDelta, pt);
	}
	
	// 当图象的高度小于显示区高度时，则不再处理滚轮消息
	if (m_rcFTRc.Height() <= m_rcImage.Height())
	{
		return CView::OnMouseWheel(nFlags, zDelta, pt);
	}

	BOOL	bMark = FALSE;
	CPoint	cpoff(0,0);
	int		i = abs(zDelta);
	
	i = (i/WHEEL_DELTA)*ISDWHEEL_DELTA;					// 修改滚轮精度

	if (zDelta > 0)										// 向下拖动
	{
		if (m_rcFTRc.top < m_rcImage.top)
		{
			i = ((m_rcFTRc.top+i) > m_rcImage.top) ? (m_rcImage.top-m_rcFTRc.top) : i;
			m_rcFTRc.OffsetRect(0, i);
			m_rcPTRc.OffsetRect(0, i);
			cpoff.y = i;
			bMark   = TRUE;
		}
	}
	else if (zDelta < 0)								// 向上拖动
	{
		if (m_rcFTRc.bottom > m_rcImage.bottom)
		{
			i = ((m_rcFTRc.bottom-i) < m_rcImage.bottom) ? (m_rcFTRc.bottom-m_rcImage.bottom) : i;
			m_rcFTRc.OffsetRect(0, -i);
			m_rcPTRc.OffsetRect(0, -i);
			cpoff.y = -i;
			bMark   = TRUE;
		}
	}
	
	// 如果发生了位移，则滚动窗口中的内容
	if (bMark == TRUE)
	{
		ScrollWindowEx(cpoff.x, cpoff.y, m_rcImage, m_rcImage, 0, 0, SW_INVALIDATE|SW_ERASE);
	}
	
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


// 以指定点为中心放大图象
void CDisplay::_RSL(CPoint point)
{
	m_iAutoResize = 0;

	// 判断是否有继续放大的余地
	if (m_fRatio < DPTRS_MAX)
	{
		// 取得事发点所在图象的象素位置
		int x = (int)(((float)(point.x-m_rcPTRc.left)/m_fRatio)+(float)0.5);
		int y = (int)(((float)(point.y-m_rcPTRc.top)/m_fRatio)+(float)0.5);
		
		m_fRatio = min(DPTRS_MAX, (m_fRatio+DPTRS_STEP));
		
		// 更新图象缩放尺寸
		SetPictLocInfo();
		
		// 记录原图显示区位置
		CRect	tmp = m_rcFTRc;
		
		m_rcPTRc = m_rcPict;
		m_rcFTRc = m_rcFrme;
		
		// 计算放大后当前点的距离
		LONG sx = point.x - (LONG)(int)((float)x * m_fRatio);
		LONG sy = point.y - (LONG)(int)((float)y * m_fRatio);
		
		// 设置新的显示位置
		m_rcPTRc.OffsetRect((int)sx, (int)sy);
		m_rcFTRc.OffsetRect((int)(m_rcPTRc.CenterPoint().x - m_rcFTRc.Width()/2), (int)(m_rcPTRc.CenterPoint().y - m_rcFTRc.Height()/2));
		
		int ox, orx;
		int oy, ory;
		
		// 修正显示位置（当图象小于显示区时，将居中显示）
		if (m_rcFTRc.Width() < m_rcImage.Width())
		{
			ox  = m_rcImage.Width()/2-m_rcFTRc.Width()/2+m_rcImage.left;
			orx	= ox - m_rcFTRc.left;
			m_rcFTRc.SetRect(ox, m_rcFTRc.top, ox+m_rcFTRc.Width(), m_rcFTRc.bottom);
			m_rcPTRc.OffsetRect(orx, 0);
		}
		else
		{
			if (m_rcFTRc.left > m_rcImage.left)
			{
				ox = m_rcImage.left - m_rcFTRc.left;
			}
			else if (m_rcFTRc.right < m_rcImage.right)
			{
				ox = m_rcImage.right - m_rcFTRc.right;
			}
			else
			{
				ox = 0;
			}
			m_rcFTRc.OffsetRect(ox, 0);
			m_rcPTRc.OffsetRect(ox, 0);
		}
		
		if (m_rcFTRc.Height() < m_rcImage.Height())
		{
			oy  = m_rcImage.Height()/2-m_rcFTRc.Height()/2+m_rcImage.top;
			ory = oy - m_rcFTRc.top;
			m_rcFTRc.SetRect(m_rcFTRc.left, oy, m_rcFTRc.right, oy+m_rcFTRc.Height());
			m_rcPTRc.OffsetRect(0, ory);
		}
		else
		{
			if (m_rcFTRc.top > m_rcImage.top)
			{
				oy = m_rcImage.top - m_rcFTRc.top;
			}
			else if (m_rcFTRc.bottom < m_rcImage.bottom)
			{
				oy = m_rcImage.bottom - m_rcFTRc.bottom;
			}
			else
			{
				oy = 0;
			}
			m_rcFTRc.OffsetRect(0, oy);
			m_rcPTRc.OffsetRect(0, oy);
		}
		
		tmp.UnionRect(tmp, m_rcFTRc);
		tmp.InflateRect(1,1);
		
		// 刷新显示区
		_UpdateImageRgn(tmp);
	}
}


// 以指定点为中心缩小图象
void CDisplay::_RSS(CPoint point)
{
	m_iAutoResize = 0;
	
	if (m_fRatio > DPTRS_MIN)
	{
		// 取得事发点所在图象的象素位置
		int x = (int)(((float)(point.x-m_rcPTRc.left)/m_fRatio)+(float)0.5);
		int y = (int)(((float)(point.y-m_rcPTRc.top)/m_fRatio)+(float)0.5);
		
		m_fRatio = max(DPTRS_MIN, (m_fRatio-DPTRS_STEP));
		
		// 更新图象缩放尺寸
		SetPictLocInfo();
		
		// 记录原图显示区位置
		CRect	tmp = m_rcFTRc;
		
		m_rcPTRc = m_rcPict;
		m_rcFTRc = m_rcFrme;
		
		// 计算缩小后当前点的距离
		LONG sx = point.x - (LONG)(int)((float)x * m_fRatio);
		LONG sy = point.y - (LONG)(int)((float)y * m_fRatio);
		
		// 设置新的显示位置
		m_rcPTRc.OffsetRect((int)sx, (int)sy);
		m_rcFTRc.OffsetRect((int)(m_rcPTRc.CenterPoint().x - m_rcFTRc.Width()/2), (int)(m_rcPTRc.CenterPoint().y - m_rcFTRc.Height()/2));
		
		int ox, orx;
		int oy, ory;
		
		// 修正显示位置（当图象小于显示区时，将居中显示）
		if (m_rcFTRc.Width() < m_rcImage.Width())
		{
			ox  = m_rcImage.Width()/2-m_rcFTRc.Width()/2+m_rcImage.left;
			orx	= ox - m_rcFTRc.left;
			m_rcFTRc.SetRect(ox, m_rcFTRc.top, ox+m_rcFTRc.Width(), m_rcFTRc.bottom);
			m_rcPTRc.OffsetRect(orx, 0);
		}
		else
		{
			if (m_rcFTRc.left > m_rcImage.left)
			{
				ox = m_rcImage.left - m_rcFTRc.left;
			}
			else if (m_rcFTRc.right < m_rcImage.right)
			{
				ox = m_rcImage.right - m_rcFTRc.right;
			}
			else
			{
				ox = 0;
			}
			m_rcFTRc.OffsetRect(ox, 0);
			m_rcPTRc.OffsetRect(ox, 0);
		}
		
		if (m_rcFTRc.Height() < m_rcImage.Height())
		{
			oy  = m_rcImage.Height()/2-m_rcFTRc.Height()/2+m_rcImage.top;
			ory = oy - m_rcFTRc.top;
			m_rcFTRc.SetRect(m_rcFTRc.left, oy, m_rcFTRc.right, oy+m_rcFTRc.Height());
			m_rcPTRc.OffsetRect(0, ory);
		}
		else
		{
			if (m_rcFTRc.top > m_rcImage.top)
			{
				oy = m_rcImage.top - m_rcFTRc.top;
			}
			else if (m_rcFTRc.bottom < m_rcImage.bottom)
			{
				oy = m_rcImage.bottom - m_rcFTRc.bottom;
			}
			else
			{
				oy = 0;
			}
			m_rcFTRc.OffsetRect(0, oy);
			m_rcPTRc.OffsetRect(0, oy);
		}
		
		// 因为是缩小，所以需要合并原图象的显示区
		tmp.UnionRect(tmp, m_rcFTRc);
		tmp.InflateRect(1,1);
		
		// 刷新显示区
		_UpdateImageRgn(tmp);
	}
}


// 刷新图象显示区
void CDisplay::_UpdateImageRgn(CRect& rect)
{
	CRect	rc;

	// 取给定矩形与图象显示区的交集矩形
	rc.IntersectRect(rect, m_rcImage);

	// 刷新窗口
	InvalidateRect(rc);
	UpdateWindow();
}


// 刷新标题区
void CDisplay::_UpdateHeadRgn(BOOL rgn)
{
	if (rgn)											// 刷新整个信息区域
	{
		InvalidateRect(m_rcHeader);
		UpdateWindow();
	}
	else												// 只刷新图象标号区
	{
		CRect	rc(m_rcHeader.left, m_rcHeader.top, m_rcHeader.left+ISDISP_HEADER_W, m_rcHeader.top+ISDISP_HEADER_H);
		InvalidateRect(rc);
		UpdateWindow();
	}
}


