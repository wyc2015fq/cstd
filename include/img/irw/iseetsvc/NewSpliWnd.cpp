/********************************************************************

	NewSpliWnd.cpp

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
	本文件用途：	图像读写引擎－C语言版－VC编译器－测试软件新分裂窗口
					类实现文件。

	本文件编写人：	
					YZ				yzfree##sina.com

	本文件版本：	30122
	最后修改于：	2003-1-22

	注：以上E-Mail地址中的##请用@替换，这样做是为了抵制恶意的E-Mail
		地址收集软件。

	修正历史：

		2003-1		第一个测试版发布

********************************************************************/

#include "stdafx.h"
#include "iseetsvc.h"
#include "NewSpliWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CNewSpliWnd

IMPLEMENT_DYNCREATE(CNewSpliWnd, CSplitterWnd)

CNewSpliWnd::CNewSpliWnd()
{
	count = 0;

	rc.SetRectEmpty();
}

CNewSpliWnd::~CNewSpliWnd()
{
}


BEGIN_MESSAGE_MAP(CNewSpliWnd, CSplitterWnd)
	//{{AFX_MSG_MAP(CNewSpliWnd)
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CNewSpliWnd message handlers

BOOL CNewSpliWnd::PreCreateWindow(CREATESTRUCT& cs) 
{
	return CSplitterWnd::PreCreateWindow(cs);
}


// 新的绘制函数
void CNewSpliWnd::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rectArg)
{
	if (pDC == NULL)
	{
		CSplitterWnd::OnDrawSplitter(pDC, nType, rectArg);
		return;
	}		

	CRect		rect = rectArg;
	COLORREF	col = (COLORREF)RGB(215,255,215);
	POINT		ptst, pten;

	switch (nType)
	{
	case splitBorder:
		if ((GetPane(0, 0)->GetStyle() & WS_VSCROLL)&&(!(GetParent()->GetStyle() & WS_CHILDWINDOW)))
		{
			CSplitterWnd::OnDrawSplitter(pDC, nType, rectArg);

			count = 0;
			rc.SetRectEmpty();

			break;
		}
		
		if (count++ != (GetRowCount()*GetColumnCount()-1))
		{
			rc.UnionRect(rc, rect);
		}
		else
		{
			rc.UnionRect(rc, rect);

			// 分裂窗口中的分裂窗口不再绘制边框
			if (!(GetParent()->GetStyle() & WS_CHILDWINDOW))
			{
				pDC->Draw3dRect(rc, GetSysColor(COLOR_3DSHADOW), GetSysColor(COLOR_3DHILIGHT));
				rc.InflateRect(-1, -1);
				pDC->Draw3dRect(rc, GetSysColor(COLOR_WINDOWFRAME), GetSysColor(COLOR_3DFACE));
			}

			count = 0;
			rc.SetRectEmpty();
		}
		break;		
	case splitIntersection:
	case splitBox:
		break;											// 放弃分割条的绘制
	case splitBar:
		if ((GetPane(0, 0)->GetStyle() & WS_VSCROLL)&&(!(GetParent()->GetStyle() & WS_CHILDWINDOW)))
		{
			CSplitterWnd::OnDrawSplitter(pDC, nType, rectArg);
		}
		else
		{
			rect.InflateRect(CNSW_FILL_WIDTH_CURTAIL, CNSW_FILL_HEIGHT_CURTAIL);

			pDC->FillSolidRect(rect, col);
		
			if ((rect.Width() > CNSW_FILL_WIDTH_MINI)&&(rect.Height() > CNSW_FILL_HEIGHT_MINI))
			{
				ptst.x = (rect.right-rect.left)/2+rect.left;
				ptst.y = rect.top+CNSW_LINE_CURTAIL;

				pten.x = ptst.x;
				pten.y = rect.bottom-CNSW_LINE_CURTAIL;

				//pDC->MoveTo(ptst);
				//pDC->LineTo(pten);							// 只绘制一条分割短线
			}
		}
		break;
	default:
		ASSERT(FALSE);
	}
	
	return;
}


// 新分裂条绘制函数
void CNewSpliWnd::OnInvertTracker(const CRect& rect)
{
	ASSERT_VALID(this);
	ASSERT(!rect.IsRectEmpty());
	ASSERT((GetStyle() & WS_CLIPCHILDREN) == 0);
	
	CDC* pDC = GetDC();

	// 更换画刷样式
	CBrush* pBrush = CDC::GetHalftoneBrush();
	HBRUSH hOldBrush = NULL;

	if (pBrush != NULL)
	{
		hOldBrush = (HBRUSH)SelectObject(pDC->m_hDC, pBrush->m_hObject);
	}

	CRect	rt = rect;

	if ((GetPane(0, 0)->GetStyle() & WS_VSCROLL)&&(!(GetParent()->GetStyle() & WS_CHILDWINDOW)))
	{
		rt.InflateRect(0, 0);
	}
	else
	{
		rt.InflateRect(0, -CNSW_LINE_CURTAIL);
	}

	// 绘制拖动阴影
	pDC->PatBlt(rt.left, rt.top, rt.Width(), rt.Height(), PATINVERT);

	if (hOldBrush != NULL)
	{
		SelectObject(pDC->m_hDC, hOldBrush);
	}

	ReleaseDC(pDC);
}


BOOL CNewSpliWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}
